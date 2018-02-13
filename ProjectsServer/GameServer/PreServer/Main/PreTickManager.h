#include "PreIOCP.h"
#include "TickManager.h"

class CPreTickManager : public CTickManager
{
public:
	explicit CPreTickManager(CPreIOCP *i_pPreIOCP, DWORD i_nTickInterval = 1000) : CTickManager { i_nTickInterval }, m_pPreIOCP1 { i_pPreIOCP } { }
	
	~CPreTickManager() = default;

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) override { }
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime) override { }	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime) override { }		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime) override { }		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime) override { m_pPreIOCP1->OnDoMinutelyWorkIOCP(pDateTime); }		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime) override { }		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime) override { }	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

	CPreIOCP* m_pPreIOCP1;
};
