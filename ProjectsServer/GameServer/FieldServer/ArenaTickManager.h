// ArenaTickManager.h: interface for the CArenaTickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENATICKMANAGER_H__ED1FDFDA_5533_4DD7_B44A_F9954C8A611A__INCLUDED_)
#define AFX_ARENATICKMANAGER_H__ED1FDFDA_5533_4DD7_B44A_F9954C8A611A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"
#include "ArenaManager.h"

class CArenaTickManager  : public CTickManager
{
public:
	CArenaTickManager(CArenaManager *i_pArenaManager, DWORD i_nTickInterval = ARENA_TICK_INTERVAL);
	virtual ~CArenaTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);
public:
	CArenaManager			*m_pArenaManager;
};

#endif // !defined(AFX_ARENATICKMANAGER_H__ED1FDFDA_5533_4DD7_B44A_F9954C8A611A__INCLUDED_)
