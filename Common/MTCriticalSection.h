#pragma once

class CMTCriticalSection
{
public:
	explicit CMTCriticalSection(CRITICAL_SECTION* i_pCritSec) : m_criticalSection { }, m_pCriticalSection { i_pCritSec } { }

	CMTCriticalSection() : CMTCriticalSection { nullptr } { InitializeCriticalSection(&m_criticalSection); }

	virtual ~CMTCriticalSection() { if (m_pCriticalSection == nullptr) DeleteCriticalSection(&m_criticalSection); }

	void Enter()
	{
		if (m_pCriticalSection) EnterCriticalSection(m_pCriticalSection);
		else EnterCriticalSection(&m_criticalSection);
	}

	void Leave()
	{
		if (m_pCriticalSection) LeaveCriticalSection(m_pCriticalSection);
		else LeaveCriticalSection(&m_criticalSection);
	}

	//- If the critical section is successfully entered or the current
	//thread already owns the critical section, the return value is nonzero.
	//- If another thread already owns the critical section,
	//the return value is zero.
	BOOL Try()
	{
		if (m_pCriticalSection) return TryEnterCriticalSection(m_pCriticalSection);
		return TryEnterCriticalSection(&m_criticalSection);
	}


	CRITICAL_SECTION *GetCriticalSectionPtr() { return &m_criticalSection; }

protected:
	CRITICAL_SECTION		m_criticalSection;
	PCRITICAL_SECTION		m_pCriticalSection;
};