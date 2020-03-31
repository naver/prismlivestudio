#include "prism-handle-wrapper.h"
#include <assert.h>

HANDLE CHandleWrapper::GetAlreadyMap(const char *pName)
{
	return OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, pName);
}

HANDLE CHandleWrapper::GetMap(const char *pName, unsigned size, bool *pNewCreate)
{
	HANDLE handle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, pName);
	if (handle) {
		if (pNewCreate)
			*pNewCreate = (GetLastError() != ERROR_ALREADY_EXISTS);
	} else {
		handle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, pName);
		if (pNewCreate)
			*pNewCreate = false;
	}

	assert(handle);
	return handle;
}

HANDLE CHandleWrapper::GetAlreadyEvent(const char *pName)
{
	return OpenEventA(EVENT_ALL_ACCESS, false, pName);
}

HANDLE CHandleWrapper::GetEvent(const char *pName, bool bNeedManualReset, bool *pNewCreate)
{
	HANDLE handle = CreateEventA(NULL, bNeedManualReset, false, pName);
	if (handle) {
		if (pNewCreate)
			*pNewCreate = (GetLastError() != ERROR_ALREADY_EXISTS);
	} else {
		handle = OpenEventA(EVENT_ALL_ACCESS, false, pName);
		if (pNewCreate)
			*pNewCreate = false;
	}

	assert(handle);
	return handle;
}

HANDLE CHandleWrapper::GetMutex(const char *pName, bool *pNewCreate)
{
	HANDLE handle = CreateMutexA(NULL, false, pName);
	if (handle) {
		if (pNewCreate)
			*pNewCreate = (GetLastError() != ERROR_ALREADY_EXISTS);
	} else {
		handle = OpenMutexA(MUTEX_ALL_ACCESS, false, pName);
		if (pNewCreate)
			*pNewCreate = false;
	}

	assert(handle);
	return handle;
}

bool CHandleWrapper::IsMutexAlready(const char *pName)
{
	HANDLE handle = OpenMutexA(MUTEX_ALL_ACCESS, false, pName);
	if (IsHandleValid(handle)) {
		CloseHandleEx(handle);
		return true;
	} else {
		return false;
	}
}

bool CHandleWrapper::IsHandleValid(const HANDLE &hHandle)
{
	return (hHandle && (hHandle != INVALID_HANDLE_VALUE));
}

bool CHandleWrapper::IsHandleSigned(const HANDLE &hHandle, DWORD dwMilliSecond)
{
	if (!hHandle) {
		assert(hHandle);
		return false;
	}

	return (WAIT_OBJECT_0 == WaitForSingleObject(hHandle, dwMilliSecond));
}

void CHandleWrapper::CloseHandleEx(HANDLE &hHandle)
{
	if (IsHandleValid(hHandle)) {
		::CloseHandle(hHandle);
		hHandle = 0;
	}
}

void CHandleWrapper::WaitThreadEnd(HANDLE &hThread, DWORD dwMilliSecond)
{
	if (!IsHandleValid(hThread))
		return;

	if (!IsHandleSigned(hThread, dwMilliSecond)) {
		TerminateThread(hThread, 1);
		assert(false && "Timeout to wait for HANDLE");
	}

	::CloseHandle(hThread);
	hThread = 0;
}
