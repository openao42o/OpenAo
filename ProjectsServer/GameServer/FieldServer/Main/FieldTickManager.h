#ifndef _FIELD_TICK_MANAGER_H_
#define _FIELD_TICK_MANAGER_H_

#include "TickManager.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"

#define FIELD_DEFAULT_TICK_INTERVAL			500		// 0.5 second

// event types
#define FIELD_TICK_EVENT_END_GUILD_WAR			0

class CFieldTickManager : public CTickManager
{
public:
	CFieldTickManager(CFieldIOCP *i_pFieldIOCP, DWORD i_nTickInterval = FIELD_DEFAULT_TICK_INTERVAL);
	virtual ~CFieldTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// 하루에 한 번 씩 해야 할 일
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// 매 시간마다 한 번 씩 해야 할 일
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// 매 분마다 한 번 씩 해야 할 일
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// 매 초마다 한 번 씩 해야 할 일
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일
#ifdef NEMERIAN_AUTO_ELECTION_AND_RESET
	void DBInitAllInflWarData();
	void DBInitAllMonthlyGuildFame();
	void SetPlayerAsLeader();
	void SetElectionResult();
	void SetNewElectionDate(ATUM_DATE_TIME *pDateTime);
	void GetPollDate();

private:
	int m_TotalVoteCount;
	int m_LeaderUID;
	int m_LeaderInflTy;
	int m_LeaderAccountUID;
	int m_ElectionMonth;
	int m_ElectionYear;
	mtvectSLEADER_CANDIDATE		m_mtvectSLEADER_CANDIDATE;
#endif

public:
	CFieldIOCP		*m_pFieldIOCP6;
};

#endif // _ATUM_FIELD_TICK_MANAGER_H_
