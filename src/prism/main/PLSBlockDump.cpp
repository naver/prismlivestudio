#include "PLSBlockDump.h"
#include "liblog.h"
#include "log/module_names.h"
#include "PLSPlatformApi/PLSPlatformApi.h"
#include <process.h>
#include <shlobj_core.h>
#include <dbghelp.h>
#include <tchar.h>

#pragma comment(lib, "dbghelp.lib")

#define HEARTBEAT_INTERVAL 500  // in milliseconds
#define HEARTBEAT_TIMEOUT 3000  // in milliseconds
#define SAVE_DUMP_INTERVAL 5000 // in milliseconds
#define MAX_BLOCK_DUMP_COUNT 5

unsigned __stdcall PLSBlockDump::CheckThread(void *pParam)
{
	PLSBlockDump *self = reinterpret_cast<PLSBlockDump *>(pParam);
	self->CheckThreadInner();
	return 0;
}

PLSBlockDump *PLSBlockDump::Instance()
{
	static PLSBlockDump ins;
	return &ins;
}

PLSBlockDump::PLSBlockDump()
{
	threadExitEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	InitSavePath();
	//StartMonitor(); // Here should not invoke StartMonitor() because QT may not be ready
}

PLSBlockDump::~PLSBlockDump()
{
	StopMonitor();
	::CloseHandle(threadExitEvent);
}

void PLSBlockDump::StartMonitor()
{
#ifdef _DEBUG
	PLS_INFO(MAINFRAME_MODULE, "[PLSBlockDump] Ignore block dump in DEBUG mode.");
	return;
#endif

	if (!heartbeatTimer) {
		preHeartbeatTime = GetTickCount();
		heartbeatTimer = this->startTimer(HEARTBEAT_INTERVAL);
		assert(heartbeatTimer > 0);
	}

	if (!checkBlockThread) {
		::ResetEvent(threadExitEvent);
		checkBlockThread = (HANDLE)_beginthreadex(0, 0, CheckThread, this, 0, 0);
	}
}

void PLSBlockDump::StopMonitor()
{
#ifdef _DEBUG
	return;
#endif

	if (heartbeatTimer) {
		killTimer(heartbeatTimer);
		heartbeatTimer = 0;
	}

	if (checkBlockThread) {
		::SetEvent(threadExitEvent);
		::WaitForSingleObject(checkBlockThread, INFINITE);
		::CloseHandle(checkBlockThread);
		checkBlockThread = 0;
	}
}

void PLSBlockDump::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == heartbeatTimer) {
		preHeartbeatTime = GetTickCount();
	}
}

void PLSBlockDump::InitSavePath()
{
	QString temp = pls_get_user_path("PRISMLiveStudio/dump/");
	dumpDirectory = temp.toStdWString();

	int err = SHCreateDirectoryEx(0, dumpDirectory.c_str(), 0);
	if (err != ERROR_SUCCESS) {
		PLS_WARN(MAINFRAME_MODULE, "Failed to create directory for saving block dump. error:%d", err);
	}
}

void PLSBlockDump::CheckThreadInner()
{
	preHeartbeatTime = GetTickCount();

	bool isBlocked = false;
	int fileIndex = 1;
	int dumpCount = 0;
	DWORD preDumpTime = 0;

	while (!IsHandleSigned(threadExitEvent, HEARTBEAT_INTERVAL)) {
		bool blocked = IsBlockState(preHeartbeatTime, GetTickCount());
		if (blocked != isBlocked) {
			isBlocked = blocked;
			if (blocked) {
				PLS_WARN(MAINFRAME_MODULE, "[PLSBlockDump] UI thread is blocked");
			} else {
				PLS_INFO(MAINFRAME_MODULE, "[PLSBlockDump] UI thread recovered");
			}
		}

		if (!blocked) {
			preDumpTime = 0;
			dumpCount = 0;
			continue;
		}

		if (dumpCount < MAX_BLOCK_DUMP_COUNT) {
			DWORD dumpInterval = dumpCount * SAVE_DUMP_INTERVAL;
			if (GetTickCount() - preDumpTime < dumpInterval) {
				continue;
			}

			SaveDumpFile(fileIndex);
			preDumpTime = GetTickCount();
			++fileIndex;
			++dumpCount;
		}
	}
}

bool PLSBlockDump::IsHandleSigned(const HANDLE &hEvent, DWORD dwMilliSecond)
{
	if (!hEvent)
		return false;

	DWORD res = WaitForSingleObject(hEvent, dwMilliSecond);
	return (res == WAIT_OBJECT_0);
}

bool PLSBlockDump::IsBlockState(DWORD preHeartbeat, DWORD currentTime)
{
	if (currentTime <= preHeartbeat) {
		return false; // normal state
	}

	DWORD heartbeatSpace = (currentTime - preHeartbeat);
	if (heartbeatSpace < HEARTBEAT_TIMEOUT) {
		return false; // normal state
	}

	return true; // blocked
}

void PLSBlockDump::SaveDumpFile(int index)
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	wchar_t path[MAX_PATH] = {0};
	swprintf_s(path, _T("%sBLOCK_%04d%02d%02d-%02d%02d%02d_%u(%d).dmp"), dumpDirectory.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, GetCurrentProcessId(), index);

	HANDLE lhDumpFile = CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (lhDumpFile && lhDumpFile != INVALID_HANDLE_VALUE) {
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, (MINIDUMP_TYPE)MiniDumpWithFullMemoryInfo, NULL, NULL, NULL);
		CloseHandle(lhDumpFile);
	}
}
