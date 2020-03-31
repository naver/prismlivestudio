#pragma once
#include <Windows.h>

class CCSection {
	CRITICAL_SECTION m_cs;

public:
	CCSection() { InitializeCriticalSection(&m_cs); }
	~CCSection() { DeleteCriticalSection(&m_cs); }

	void Lock() { EnterCriticalSection(&m_cs); }
	void Unlock() { LeaveCriticalSection(&m_cs); }
};

class CAutoLockCS {
	CCSection &m_Lock;

public:
	explicit CAutoLockCS(CCSection &cs) : m_Lock(cs) { m_Lock.Lock(); }

	~CAutoLockCS() { m_Lock.Unlock(); }
};
