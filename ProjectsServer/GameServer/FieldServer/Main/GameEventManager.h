// GameEventManager.h: interface for the CGameEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEEVENTMANAGER_H__90AE5331_2500_4855_A8B8_E9FCF5D4D444__INCLUDED_)
#define AFX_GAMEEVENTMANAGER_H__90AE5331_2500_4855_A8B8_E9FCF5D4D444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EN_GAME_EVENT_TYPE
{
	GAME_EVENT_TYPE_EXP				= 0,
	GAME_EVENT_TYPE_SPI				= 1,
	GAME_EVENT_TYPE_EXP_NO_DOWN		= 2,
	GAME_EVENT_TYPE_DROP_ITEM		= 3,
	GAME_EVENT_TYPE_DROP_REAR_ITEM	= 4,
	GAME_EVENT_TYPE_WARPOINT		= 5,				// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	GAME_EVENT_TYPE_MAX_COUNT		= 6
};

///////////////////////////////////////////////////////////////////////////////
// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
enum EN_GAME_EVENT_GROUP
{
	GAME_EVENT_GROUP_HAPPYHOUR		= 0,
	GAME_EVENT_GROUP_MOTHERSHIP		= 1,
	GAME_EVENT_GROUP_ITEM			= 2,
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM		= 3,	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND	= 4,	// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	GAME_EVENT_GROUP_MAX_COUNT			= 5		
};

struct SGameEvent
{
	BOOL			bUsingFlag;
	BOOL			bSendEventEndMessage;
	ATUM_DATE_TIME	atimeStartedTime;				
	ATUM_DATE_TIME	atimeEndTime;					
	DWORD			dwTimeLimitMinute;
	float			fApplyRate;
	BYTE			byInfluenceMask;				// 2006-04-12 by cmkwon, 이벤트가 적용될 세력(INFLUENCE_TYPE_XXX)
	INT				MinLevel;						// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	INT				MaxLevel;						// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -
	MapIndex_t		MapIndex;						// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - 0 이면 적용 하지 않음
};

typedef SGameEvent (*SGAEM_EVENT_PPTR)[GAME_EVENT_TYPE_MAX_COUNT];

class CFieldIOCP;
class CFieldIOCPSocket;
class CGameEventManager  
{
public:
	CGameEventManager();
	~CGameEventManager();

	void InitEventManager(CFieldIOCP *i_pFIOCP);
// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 사용 필요 없어진 함수 임
//	BOOL IsDoingAllGameEvent(BYTE i_byInflTy);
	BOOL IsProgressGameEventGroup(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR, SGameEvent **o_ppUsingGameEv=NULL);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 

	void OnDoMinutelyWorkGameEventTime(ATUM_DATE_TIME *pDateTime);
	BOOL StartGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute, int i_nMinLevel, int i_nMaxLevel, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR, MapIndex_t i_Mapindex = 0);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 인자 추가. // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - MinLevel, MaxLevel
	void EndAllGameEvent(BOOL i_bEndNotify=FALSE, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	BOOL EndGameEvent(EN_GAME_EVENT_TYPE i_eventType, BOOL i_bSendEndMsg, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	BOOL GetGameEventInfoString(char *o_szInfo, BYTE *o_pbyInflMask, EN_GAME_EVENT_TYPE i_eventType, BOOL i_bAdminFlag=FALSE, BYTE i_byInflType=INFLUENCE_TYPE_ALL_MASK);
	void SenGameEventGroupMotherShip8ItemInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-23 by cmkwon
	SGameEvent *GetPCBangGameEventPtr(EN_GAME_EVENT_TYPE i_eventType);
	SGameEvent *GetPCBangUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType);
	BOOL StartPCBangGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute);
	BOOL EndPCBangGameEvent(void);

// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 시스템 변경으로 함수명과 함수 수정함
//	float GetExperienceRate(BYTE i_byInflType, BOOL i_bPCBangClient);			// 경치 이벤트
//	float GetSPIRate(BYTE i_byInflType, BOOL i_bPCBangClient);					// 스피 이벤트
//	float GetExpRepairRate(BYTE i_byInflType, BOOL i_bPCBangClient);			// 경치복구 이벤트
//	float GetDropItemRate(BYTE i_byInflType, BOOL i_bPCBangClient);
//	float GetDropRareItemRate(BYTE i_byInflType, BOOL i_bPCBangClient);
//	float GetWarPointRate(BYTE i_byInflType, BOOL i_bPCBangClient);				// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 
	SGameEvent *GetArrGameEventByInflType(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 

// 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 - 아래의 CGameEventManager::GetTotalEventPlusRate() 함수로 대체함
//	float GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient);
	float GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -	

	float GetExperiencePlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -	// 2007-10-30 by cmkwon, 경치 이벤트
	float GetSPIPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);			// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -		// 2007-10-30 by cmkwon, 스피 이벤트
	float GetExpRepairPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -	// 2007-10-30 by cmkwon, 경치복구 이벤트
	float GetDropItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -		// 2007-10-30 by cmkwon, 드랍아이템
	float GetDropRareItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -		// 2007-10-30 by cmkwon, 드랍아이템에 레어붙을 확률
	float GetWarPointPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - // 2008-03-14 by dhjin, Level별 해피아워 이벤트 구현 -		// 2007-10-30 by cmkwon, 워포인트 이벤트 관련 추가
	BOOL EndGameEventAllByInfluenceType(BYTE i_byInflMask, BOOL i_bSendEndMsg=FALSE, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	
protected:	
	SGameEvent *GetGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	SGameEvent *GetUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	SGAEM_EVENT_PPTR GetGameEventGroupArray(EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);					// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 	

protected:
	SGameEvent			m_arrGameEvent[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];	// 2007-10-30 by cmkwon, 0=Normal, 1=VCN, 2=ANI, 3=ALL
	SGameEvent			m_PCBangGameEvent[GAME_EVENT_TYPE_MAX_COUNT];						// 2006-08-23 by cmkwon, PCBang 이벤트 설정
	SGameEvent			m_arrGameEventofMotherShip[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 세력 버프용 멤버 변수 추가 2007-10-30 by cmkwon, 0=Normal, 1=VCN, 2=ANI, 3=ALL
	SGameEvent			m_arrGameEventofItem[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];			// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 세력 버프용 멤버 변수 추가 2007-10-30 by cmkwon, 0=Normal, 1=VCN, 2=ANI, 3=ALL
	SGameEvent			m_arrGameEventofCrystal[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];			// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	SGameEvent			m_arrGameEventofCrystal2ND[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];	// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	
public:
	CFieldIOCP			*m_pFieldIOCP10;
	
	static char *GetGameEventTypeString(int i_nEvType);
	static float		ms_fDefaultExpRate;
	static float		ms_fDefaultSPIRate;
	static float		ms_fDefaultExpRepareRate;
	static float		ms_fDefaultDropItemRate;
	static float		ms_fDefaultDropRareItemRate;						
	static float		ms_fDefaultWarPointRate;							// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가			
};



#endif // !defined(AFX_GAMEEVENTMANAGER_H__90AE5331_2500_4855_A8B8_E9FCF5D4D444__INCLUDED_)
