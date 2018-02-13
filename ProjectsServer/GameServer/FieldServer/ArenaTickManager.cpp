// ArenaTickManager.cpp: implementation of the CArenaTickManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArenaTickManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaTickManager::CArenaTickManager(CArenaManager *i_pArenaManager, DWORD i_nTickInterval /*= ARENA_TICK_INTERVAL */)
: CTickManager(i_nTickInterval)
{
	m_pArenaManager = i_pArenaManager;
}

CArenaTickManager::~CArenaTickManager()
{

}


void CArenaTickManager::DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent)
{
#ifdef _DEBUG
	DBGOUT(STRMSG_S_F2NOTIFY_0141, pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
#endif
	
	return;
}

void CArenaTickManager::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
}

void CArenaTickManager::DoDailyWork(ATUM_DATE_TIME *pDateTime)
{

}

void CArenaTickManager::DoHourlyWork(ATUM_DATE_TIME *pDateTime)
{

}

void CArenaTickManager::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	m_pArenaManager->DoMinutelyWorkArena(pDateTime);
}

void CArenaTickManager::DoSecondlyWork(ATUM_DATE_TIME *pDateTime)
{

}

void CArenaTickManager::DoMonthlyWork(ATUM_DATE_TIME *pDateTime)
{

}