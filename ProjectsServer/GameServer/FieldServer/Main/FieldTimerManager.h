#ifndef _FIELD_TIMER_MANAGER_H_
#define _FIELD_TIMER_MANAGER_H_

#include "FieldDataManager.h"
#include "AtumProtocol.h"

///////////////////////////////////////////////////////////////////////////////
// 2006-04-21 by cmkwon, 변경(착륙 상태에서만 3초당 27씩 회복)
#define HP_RECOVERY_INTERVAL			3000	// ms
#define HP_MIN_RECOVERY_INTERVAL		250		// ms // 2006-12-13 by cmkwon, 최소값

///////////////////////////////////////////////////////////////////////////////
// 2006-04-21 by cmkwon, 변경(착륙 상태에서 3초당 27씩 회복, 비행 상태에서는 3초당 14씩 회복)
#define DP_RECOVERY_INTERVAL			3000	// ms
#define DP_MIN_RECOVERY_INTERVAL		250		// ms // 2006-12-13 by cmkwon, 최소값
#define DP_RECOVERY_PENALTY_SPEED		50

///////////////////////////////////////////////////////////////////////////////
// 2006-04-21 by cmkwon, 변경(상태와 상관없이 3초당 2씩 회복)
// 2006-09-27 by cmkwon, 변경(3초당 3씩으로 변경)
#define SP_RECOVERY_INTERVAL			3000	// ms
#define SP_MIN_RECOVERY_INTERVAL		250		// ms // 2006-12-13 by cmkwon, 최소값

#define EP_CONSUMPTION_VALUE			5.0f	// 2005-08-04 by cmkwon, 연료소모량을 반으로 줄인다(10 ==> 5)
#define EP_CONSUMPTION_DISTANCE			30000
#define EP_CONSUMPTION_TIME				60000	// 60초 = 1분

typedef mt_list<MEX_TIMER_EVENT>	mtlistMexTimerEvent;

class CFieldItemManager;

class CFieldTimerManager : public CFieldDataManager  
{
public:
	CFieldTimerManager();

	void ResetTimerData();

	// TimerEvent 결과 처리 함수
	void HandleTimerEventRecoverHP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventRecoverDP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventRecoverSP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDecreaseSP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);

	void HandleTimerEventDeleteMine(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDeleteDummy(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDeleteFixer(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDeleteDecoy(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventSequential(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventRequestTimeOut(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventTest(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);		// 테스트 및 디버깅용
	void HandleTimerEventDoMinutelyWork(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	
	void HandleTimerEventDotSkillStealing(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);	// Stealing 스킬 - SP감소 (SP가 소진 되어도 계속 적용 되어야 한다.)
	void HandleTimerEventDotSkillDrain(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);	// Drain 스킬 - 피격대상에 HP감소시켜 공격대상에게 채워준다.
	void HandleTimerEventDotSkillTimeBomb(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);	// TimeBomb 스킬

	void HandleTimerFixedTermShape(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);			// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

	// 타이머 관련 함수
	UID32_t StartTimerField(TimerEventType i_type, TimeUnit_t i_nInterval, UINT i_nParam1 = 0, UINT i_nParam2 = 0, UINT i_nParam3 = 0, INT i_nRepeatCount = 0);
	void StartTimerFieldNotCreateTimerUID(TimerEventType i_type, TimeUnit_t i_nInterval, UID32_t i_nTimerUID, UINT i_nParam1 = 0, UINT i_nParam2 = 0, UINT i_nParam3 = 0, INT i_nRepeatCount = 0);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 외부에서 TimerUID 입력 받아 사용하는 함수 추가
	BOOL StopTimerField(UID32_t i_nTimerUID);
	UID32_t UpdateTimerField(UID32_t i_nTimerUID);		// 주의: 0이 return되면 해당 TimerEvent는 내부 함수에서 delete됨!
	UID32_t PauseTimerField(UID32_t i_nTimerUID);
	UID32_t ContinueTimerField(UID32_t i_nTimerUID);

	int PauseAllTimerEvents(TIMER_EVENT_4_EXCHANGE *ArrTimerEvent4Exchange = NULL);		// pause가 가능한 TimerEvent의 개수를 반환
	int ContinueAllTimerEvents();	// continue된 TimerEvent의 개수를 반환

	const MEX_TIMER_EVENT* GetTimerEventByEventID(UID32_t i_nTimerUID);
	const MEX_TIMER_EVENT* GetTimerEventByItemNum(INT i_nItemNum);
	BOOL DeleteTimerEventByTimerUID(UID32_t i_nTimerUID);

	BOOL CheckRecoveryTimer(TimerEventType TEType, BOOL i_bFlagCheck=TRUE);

public:
	// TimerEvent 관련
	CUID32Generator			m_TimerUIDGenerator;
	mtlistMexTimerEvent		m_listWatingTimerEvents;	// 대기중인 Timer Event의 리스트

	// Timer 관련
	DWORD					m_dwDoMinutelyCounts;

	BOOL					m_bRecoverHPTimerRunning;
	DWORD					m_dwRecoverHPRunningTick;		// 2005-12-01 by cmkwon
	UINT					m_uiHPTimerUID;					// 2006-09-27 by cmkwon

	BOOL					m_bRecoverDPTimerRunning;
	DWORD					m_dwRecoverDPRunningTick;		// 2005-10-26 by cmkwon
	UINT					m_uiDPTimerUID;					// 2006-09-27 by cmkwon

	BOOL					m_bRecoverSPTimerRunning;
	DWORD					m_dwRecoverSPRunningTick;		// 2005-12-01 by cmkwon
	UINT					m_uiSPTimerUID;					// 2006-09-27 by cmkwon
	
	BOOL					m_bDoMinutelyWorkRunning;
	BOOL					m_bImmediateHPTimerRunning;
	BOOL					m_bImmediateDPTimerRunning;
	BOOL					m_bImmediateSPTimerRunning;
	BOOL					m_bImmediateEPTimerRunning;
	
	int						m_nRemainedTimeOfGradualHPUP;	// 0이면 중지(or cancel)함
	int						m_nRemainedTimeOfGradualDPUP;	// 0이면 중지(or cancel)함
	int						m_nRemainedTimeOfGradualSPUP;	// 0이면 중지(or cancel)함
	int						m_nRemainedTimeOfGradualEPUP;	// 0이면 중지(or cancel)함


	// timer event의 중복을 막기 위함
	UID32_t					m_TEIDGradualHPUP;	// GradualHPUP의 중복을 막기 위함

	// DP 소모 계산용
	float					m_fSumDistanceForDPConsumption;		// DP 소모용 캐릭터의 이동 거리의 합
	float					m_fDPRecoveryWeight;				// 공격시 회복 시간을 줄이기 위해 사용

	// EP 소모 계산용
	float					m_fSumDistanceForEPConsumption;		// EP 소모용 캐릭터의 이동 거리의 합
	TimeUnit_t				m_EPConsumptionTimeStamp;			// EP 소모용 시간

};

#endif // _FIELD_TIMER_MANAGER_H_
