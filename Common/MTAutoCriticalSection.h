#pragma once

#include "MTCriticalSection.h"

class CMTAutoCriticalSection : public CMTCriticalSection  
{
public:
	CMTAutoCriticalSection() = default;
	explicit CMTAutoCriticalSection(CRITICAL_SECTION *i_pCritSec) : CMTCriticalSection { i_pCritSec } { Enter(); }
	virtual ~CMTAutoCriticalSection() { if (m_pCriticalSection) Leave(); }

	void UnlockMTAutoCriticalSection() { if (m_pCriticalSection) Leave(); m_pCriticalSection = nullptr;}
};