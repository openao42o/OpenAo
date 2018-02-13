#ifndef _ATUM_WEB_TICK_MANAGER_H_
#define _ATUM_WEB_TICK_MANAGER_H_

#include "TickManager.h"

class CFieldWebIOCP;

class CFieldWebTickManager : public CTickManager
{
public:
	CFieldWebTickManager(CFieldWebIOCP *i_pFieldWebIOCP, DWORD i_nTickInterval = 1000);
	virtual ~CFieldWebTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) {}
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 한달에 한 번 씩 해야 할 일

public:
	CFieldWebIOCP *m_pFieldWebIOCP1;
};

#endif // _ATUM_WEB_TICK_MANAGER_H_
