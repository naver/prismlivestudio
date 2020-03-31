#pragma once
#include <Windows.h>
#include <string>
#include <QObject>
#include <QTimerEvent>

// %APPDATA%\PRISMLiveStudio\dump
class PLSBlockDump : public QObject {
	Q_OBJECT

	PLSBlockDump();

public:
	static unsigned __stdcall CheckThread(void *pParam);

	static PLSBlockDump *Instance();

	virtual ~PLSBlockDump();

	void StartMonitor();
	void StopMonitor();

protected:
	virtual void timerEvent(QTimerEvent *event);

	void InitSavePath();
	void CheckThreadInner();
	bool IsHandleSigned(const HANDLE &hEvent, DWORD dwMilliSecond);
	bool IsBlockState(DWORD preHeartbeat, DWORD currentTime);
	void SaveDumpFile(int index);

private:
	std::wstring dumpDirectory = L""; // include "\" at the tail

	int heartbeatTimer = 0;
	DWORD preHeartbeatTime = GetTickCount();

	HANDLE checkBlockThread = 0;
	HANDLE threadExitEvent = 0;
};
