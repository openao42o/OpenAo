// MonitorTickManager.h: interface for the CMonitorTickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORTICKMANAGER_H__13683692_4749_476F_8093_A587139ABF42__INCLUDED_)
#define AFX_MONITORTICKMANAGER_H__13683692_4749_476F_8093_A587139ABF42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"

class CMonitorServerNode;

class CMonitorTickManager : public CTickManager  
{
public:
	CMonitorTickManager();
	virtual ~CMonitorTickManager();

	BOOL InitTickManager(CMonitorServerNode *i_pMonitorServerNode);
	void CleanTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) {}
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

public:
	HANDLE		m_hFile;
	CMonitorServerNode	*m_pMonitorServerNode;
};

#endif // !defined(AFX_MONITORTICKMANAGER_H__13683692_4749_476F_8093_A587139ABF42__INCLUDED_)
