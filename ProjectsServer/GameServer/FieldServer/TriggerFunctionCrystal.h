// TriggerFunctionCrystal.h: interface for the CTriggerFunctionCrystal class.
//
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTIONCRYSTAL_H__3DEB20C4_4877_47F0_A41A_EED0662EB3CE__INCLUDED_)
#define AFX_TRIGGERFUNCTIONCRYSTAL_H__3DEB20C4_4877_47F0_A41A_EED0662EB3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TriggerFunction.h"

#define EVENT_OPTION_NONE					0		// 옵션 - 없음
#define EVENT_OPTION_HELL_WARP_CLOSE		1		// 옵션 - 워프 닫기
#define EVENT_OPTION_HELL_BACK_MAP_WARP		2		// 옵션 - 모든 유저 Back Map 으로 워프

class CTriggerCrystalGroup;

class CTriggerFunctionCrystal : public CTriggerFunction  
{
	friend class CTriggerCrystalGroup;
	friend class CTriggerCrystalDestroyGroup;

public:
	CTriggerFunctionCrystal();
	virtual ~CTriggerFunctionCrystal();

	void OnClear();
	void OnDestroy();
	BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);

	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);

	void OnEventSkip(BOOL bCheckCreateCrystal = TRUE);		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

	BOOL OnMonsterCreate(CFieldMonster *pMonster);
	BOOL OnMonsterDead(CFieldMonster *pMonster);
	BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);
	BOOL OnIsInvincible(MonIdx_t MonsterIdx, CFieldIOCPSocket *pIOCPSocket);
	BOOL OnIsCrystal();	// 2013-01-23 by jhseol, 전쟁 시 크리스탈만 공격 불가능 하도록 수정

public:
	void SetCrystalInfo(CrystalGroupID_t CrystalGroupID, INT PeriodTime, BYTE RandomSequence);
	BOOL InsertTriggerCrystal(CTriggerCrystalGroup *pTriggerCrystalGroup);

	BOOL IsCreatedCrystal()		{ return m_bCreatedCrystal; }
	BOOL IsBelongCrystal(MonIdx_t MonsterIdx);

	void ResetLocalData();				// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
	void ResetEventData();

	void SetNextCreateTime();
	void FindFirstDamageUser();			// 누적 데미지 캐릭터 1등을 찾는다
	void FindWinnerInfluence();			// 누적 데미지 세력 1등을 찾는다

	void InsertDestroyedSequence(MonIdx_t MonsterIdx);

protected:
	BOOL RunDestroyEvent();
	BOOL SetAllShopRemainCountZero();

	void RunEvent(EventID_t EventID);
	void StepEvent(EventID_t EventID, INT EventOption = EVENT_OPTION_NONE);
	void StopEvent(EventID_t EventID);

	void MixEvent();					// 크리스탈 이벤트 섞기

	// 1차 이벤트

	void EventHellWarp(BOOL bStart, INT EventOption = EVENT_OPTION_NONE);
	void EventSummonBossMonster(BOOL bStart);
	void EventShopKit(BOOL bStart);
	void EventExp100(BOOL bStart);
	void EventShopHyperCard(BOOL bStart);
	void EventShopSeal(BOOL bStart);
	void EventDrop100(BOOL bStart);
	void EventExpSPIDrop100(BOOL bStart);

	// 2차 이벤트

	void Event2NDWarPoint(BOOL bStart);
	void Event2NDSummonBossMonster(BOOL bStart);
	void Event2NDExp150(BOOL bStart);
	void Event2NDDrop150(BOOL bStart);
	void Event2NDExpSPIDrop150(BOOL bStart);
	void Event2NDCapsuleItem(BOOL bStart);
	void Event2NDShopPrefixSuffix(BOOL bStart);
	void Event2NDShopKit(BOOL bStart);
	void Event2NDShopHyperCard(BOOL bStart);
	void Event2NDShopWepone(BOOL bStart);
	void Event2NDHellWarp(BOOL bStart, INT EventOption = EVENT_OPTION_NONE);

	void SendNotifyHappyHourEvent(float fEXPRate2, float fSPIRate2, float fEXPRepairRate2, float fDropItemRate2, float fDropRareRate2, float fWarPointRate2);
	void SendNotifyHappyHourEvent2ND(float fEXPRate2, float fSPIRate2, float fEXPRepairRate2, float fDropItemRate2, float fDropRareRate2, float fWarPointRate2);

protected:
	CrystalGroupID_t m_CrystalGroupID;
	INT					m_PeriodTime;
	BYTE				m_RandomSequence;

	BOOL m_bCreatedCrystal;

	CTriggerCrystalGroup			*m_pDefaultCrystalGroup;			// 기본 파괴될 크리스탈 그룹 정보	
	vector<CTriggerCrystalGroup *>	m_vectorTriggerCrystal;				// 초기 부팅시 설정되고 변경되지 않는다. 동기화 필요 없음

	mtvectTriggerAttackUserDamage	m_mtvectTriggerAttackUserDamage;	// 각 데미지 정보
	vectorDestroyedSequence			m_vectorDestroyedSequence;			// 파괴된 크리스탈 (순서대로 저장)

protected:
	UID32_t		m_FirstCharacterUniqueNumber;		// 1등한 캐릭터의 고유번호
	UID32_t		m_FirstGuildUniqueNumber;			// 1등한 캐릭터의 길드 번호
	BYTE		m_FirstInfluenceType;				// 1등한 캐릭터의 세력 타입

	BYTE		m_WinnerInfluenceType;				// 이긴 세력

	EventID_t	m_ProcessEventID;					// 진행중인 이벤트

	TimeUnit_t	m_NextTickEventCreate;				// 다음 이벤트 시간	- 생성 이벤트		GetTickCount()
	TimeUnit_t	m_NextTickEvent1st;					// 다음 이벤트 시작 - 첫번째 이벤트		GetTickCount()
	TimeUnit_t	m_NextTickEvent2nd;					// 다음 이벤트 시작 - 두번째 이벤트		GetTickCount()
};

#endif // !defined(AFX_TRIGGERFUNCTIONCRYSTAL_H__3DEB20C4_4877_47F0_A41A_EED0662EB3CE__INCLUDED_)
