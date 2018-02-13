#ifndef _ATUM_FIELD_MAP_CHANNEL_H_
#define _ATUM_FIELD_MAP_CHANNEL_H_

#include "MapChannel.h"
#include "FieldMapProject.h"
#include "FieldMonster.h"

typedef mt_map<UID32_t, CFieldIOCPSocket*>					mtmapUID2FieldIOCPSocket;

///////////////////////////////////////////////////////////////////////////////
/// \class		CFieldMapChannel
///
/// \brief		Map의 실제 데이타 관리: Characters, Monsters, DropItems 등
/// \author		kelovon
/// \version
/// \date		2004-03-24 ~ 2004-03-24
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CRacing;
class CCityWar;
class CTriggerMapBuffManager;		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
class CMapTriggerManager;			// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
class CTriggerFunction;				// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
class CFieldMapChannel : public CMapChannel
{
public:
	CFieldMapChannel(CFieldMapWorkspace *i_pWorkspace, CFieldMapProject *i_pProject, ChannelIndex_t i_nChannelIndex);
	virtual ~CFieldMapChannel();

	int GetExactCharacterIndexes(vector<ClientIndex_t> *o_pClientIndexVector, D3DXVECTOR3 *i_pVec3Pos, float i_fRadius, ClientIndex_t i_nClientIndexToExclude = INVALID_CLIENT_INDEX);
	int GetExactMonsterIndexes(vector<ClientIndex_t> *o_pClientIndexVector, D3DXVECTOR3 *i_pVec3Pos, float i_fRadius);

	// DropItem 제거 관련 함수
	void DeleteDropItem(float x, float z, ITEM_GENERAL* pStoreItem);
	void DeleteDropItem(D3DXVECTOR3 pos, ITEM_GENERAL* pStoreItem);	// y좌표는 무시됨
	void DeleteDropItem(CMapBlock *pBlock, ITEM_GENERAL* pStoreItem);

	// moster 관련
	CFieldMonster *GetFieldMonster(ClientIndex_t i_MonsterIndex, int i_nTestIdx);
	BOOL FieldOnMonsterCreate(MSG_FN_MONSTER_CREATE *pMonsterCreate);
	void FieldDeleteAllMonster(void);
	BOOL FieldOnMonsterDelete(MSG_FN_MONSTER_DELETE *pMonsterDelete);
	BOOL FieldOnMonsterMove(MSG_FN_MONSTER_MOVE *pMove);
	BOOL FieldOnGetCharacterInfo(MSG_FN_GET_CHARACTER_INFO * i_pGetCInfo);
	BOOL FieldOnMissileMove(MSG_FN_MISSILE_MOVE * i_pMissileMove);
	BOOL FieldOnMonsterHPRecovery(MSG_FN_MONSTER_HPRECOVERY * i_pHPRecovery);
	BOOL FieldOnMonsterHide(MSG_FN_MONSTER_HIDE * i_pMonHide);
	BOOL FieldOnMonsterShow(MSG_FN_MONSTER_SHOW * i_pMonShow);
	BOOL FieldOnBattleAttackPrimary(MSG_FN_BATTLE_ATTACK_PRIMARY * i_pAttackPri);
	BOOL FieldOnBattleAttackSecondary(MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSec);
	BOOL FieldOnBattleAttackFind(MSG_FN_BATTLE_ATTACK_FIND * i_pAttackFind);
	BOOL FieldOnMonsterChangeBodycondition(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChangeBody);
	BOOL FieldOnMonsterSkillUseSkill(MSG_FN_MONSTER_SKILL_USE_SKILL * i_pUseSkill);
	BOOL FieldOnMonsterSkillEndSkill(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	BOOL FieldSendMonsterChangeInfo(CFieldMonster * i_ptmFMonster, int i_nChangeType);
	BOOL FieldOnMonsterAutoDestroyed(MSG_FN_MONSTER_AUTO_DESTROYED * i_pAutoDestroyed);
	BOOL FieldOnMonsterChangeOk(MSG_FN_MONSTER_CHANGE_OK *pMonsterChangeOk);		// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가

	void OnMonsterDeadFieldMapChannel(CFieldMonster *i_pFMonster);
// 2005-12-17 by cmkwon, 죽은 후 AttackTime 적용을 위해 NPC Server 에서 처리하도록  수정함
//	void BattleAttackOnMonsterDead(CFieldMonster *i_pFMonster);

	// NPC 서버 관련
	BOOL Send2NPCServerW(BYTE *pData, int nSize);

	template<template<MessageType_t> typename msg_t, MessageType_t t>
	bool Send2NPCServerW(msg_t<t>& msg) { return m_pFieldMapProject->Send2NPCServer(reinterpret_cast<BYTE*>(&msg), msg.size()); }


	BOOL SendBodyCondition2NPCServer(ClientIndex_t clientIndex, BodyCond_t bodyCon);

	// 같은 채널에 있는 캐릭터들 모두에게 MSG 보내기
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldMapChannel::SendMessageToAllInChannel#, 인자추가(i_nStartCityMapIdx)
	void SendMessageToAllInChannel(BYTE *buffer, int size, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	void SendString128ToAllInChannel(BYTE string128_type, char *szString);

	// 현재 채널에 있는 캐릭터를 모두 특정 도시맵으로 워프시킨다

	// 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정
	//BOOL WarpToCityMapFieldMapChannel(MapIndex_t i_cityMapIndex, UID32_t i_excludeGuildUID);
	BOOL WarpToCityMapFieldMapChannel(MapIndex_t i_cityMapIndex, UID32_t i_excludeGuildUID,	MapIndex_t i_currentMapIndex = 0);
	// End. 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

	BOOL WarpToCityMapFieldMapChannelByInfluence(BYTE i_byexcludeInfuence);		// 2007-08-20 by dhjin, 제외 세력 빼고 모두 마을로 워프.
	BOOL WarpToCityMapFieldMapChannelByGuildUID(UID32_t i_nexcludeGuildUID);	// 2007-08-20 by dhjin, 제외 길드 빼고 모두 마을로 워프.
	BOOL WarpToOutPostCityMapFieldMapChannelByGuildUID(UID32_t i_nGuildUID, int i_nMapindex);	// 2007-08-20 by dhjin, 전진기지 소유 여단원 전진기지 도시 맵으로 워프.
	BOOL WarpToCityMap(UID32_t i_excludeCharacterUID);


	// 세력전 관련
	int InsertItemInMap(INT i_nItemNum, INT i_nItemCount, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, UID32_t i_CharacterUniqueNumber = 0);		// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차

	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldMapChannel::AddWarPointInMap()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가
	int AddWarPointInMap(INT i_nAddWarPoint, BOOL i_bEventBonus, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, BYTE i_byWPUpdateTy=WPUT_GENERAL);				// 2007-06-14 by cmkwon, 세력전 보상 수정 - 
	int AddWarPointInMapByGuildUID(INT i_nAddWarPoint, BOOL i_bEventBonus, UID32_t i_GuildUID, BYTE i_byWPUpdateTy=WPUT_GENERAL);			// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	int AddWarPointInMapByCharacterUID(INT i_nAddWarPoint, BOOL i_bEventBonus, UID32_t i_CharUID, BYTE i_byWPUpdateTy=WPUT_GENERAL);		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템

	// CMapProject의 wrapper 함수
	inline EVENTINFO* GetTileEventInfoW(const D3DXVECTOR3 *pPosVector3);

	// CFieldMapProject의 wrapper 함수
	CFieldMapProject* GetDeadDefaultMapProjectW() {			return m_pFieldMapProject->GetDeadDefaultMapProject(); }
	MapIndex_t GetDeadDefaultMapIndexW() {					return m_pFieldMapProject->GetDeadDefaultMapIndex(); }
	inline D3DXVECTOR3 GetDefaultPointW() {					return m_pFieldMapProject->GetDefaultPoint(); } // position vector가 잘 못 되었을 때 혹은 죽었을 때, 기본 위치를 알기 위해
	BOOL IsCheckUserCountForWarp(CFieldIOCPSocket *i_pFISock);

	EVENTINFO *GetEventInfoByObjectTypeAndDistanceW(DWORD i_dwObjType, D3DXVECTOR3 *i_pVec3Position);		// 2011-12-19 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 변경된 교체 오브젝트의 고정된 추가 인덱스 값

	///////////////////////////////////////////////////////////////////////////////
	/// \fn			EVENTINFO *GetEventInfoByMinimumDistanceW(EventType_t i_eventTy, D3DXVECTOR3 *i_pVec3Position)
	/// \brief		
	/// \author		cmkwon
	/// \date		2005-12-12 ~ 2005-12-12
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	EVENTINFO *GetEventInfoByMinimumDistanceW(EventType_t i_eventTy, D3DXVECTOR3 *i_pVec3Position)
	{
		return m_pFieldMapProject->GetEventInfoByMinimumDistance(i_eventTy, i_pVec3Position);
	}

	///////////////////////////////////////////////////////////////////////////
	// Property
	DWORD GetModulusNumberTotalNetworkState(void){		return m_dwModulusNumberTotalNetworkState;}
	DWORD GetMaxUserCountsChannel(void){					return m_dwMaxUserCountsChannel;}
	
	void AddMapSendTraffic(int nSendBytes, int nSendCounts);

	// 맵의 Bandwidth 계산
	void CalcMapTotalTraffic(void);
	void LockMapTraffic(void){			EnterCriticalSection(&m_crtlMapTraffic);};
	void UnlockMapTraffic(void){		LeaveCriticalSection(&m_crtlMapTraffic);};

	INT GetCrowdedness(void);
	int GetUserCounts(BYTE i_byInflTyMask, BYTE i_byMinLevel, BYTE i_byMaxLevel);

	void ProcessRacing(struct tm *i_pRealTime);
	void InRacingWaitingRoom(CFieldIOCPSocket *i_pFSocket);	
	void OutRacingSystem(CFieldIOCPSocket *i_pFSocket);
	int JoinRacing(CFieldIOCPSocket *i_pFSocket);
	BOOL IsJoinedUser(CFieldIOCPSocket *i_pFSocket);
	BOOL CheckPointCheck(CFieldIOCPSocket *i_pFSocket, BYTE i_byCheckPointIndex);

	BOOL MakeRacingMessage(MessageType_t i_msgType, void *o_pMsg);

	void SetCityWarFieldMapChannel(CCityWar *i_pCityWar);
	BOOL MakeMSG_FC_CITYWAR_GET_OCCUPYINFO_OK(MSG_FC_CITYWAR_GET_OCCUPYINFO_OK *o_pOccupyInfo, UID32_t i_guildUID);
	Err_t SetCityWarSettingTimeFieldMapChannel(ATUM_DATE_TIME *i_pSetTime, UID32_t i_guildUID, UID32_t i_charUID);
	Err_t SetCityWarTexFieldMapChannel(float i_fSetTex, UID32_t i_guildUID, UID32_t i_charUID);
	Err_t SetCityWarBriefingFieldMapChannel(char *i_szSetBriefing, UID32_t i_guildUID, UID32_t i_charUID);
	Err_t CityWarBringSumOfTex(int *o_pnBringTex, UID32_t i_guildUID, UID32_t i_charUID);
	float CityWarGetTexPercent(int i_nBuildingIdx, UID32_t i_guildUID);
	int CityWarAddTex(int i_nTex);

	///////////////////////////////////////////////////////////////////////////////	
	// 2006-11-21 by cmkwon, 전략포인트 몬스터 관련
	void SetLastTickStrategyPointMonsterDeaded(DWORD i_dwTick);
	BOOL IsOccupyStrategyPointMonster(void);

	//////////////////////////////////////////////////////////////////////////
	// 2007-09-05 by dhjin, 텔레포트 관련 추가
	void SetTelePortState(int i_nTelePortState);		// 2007-09-05 by dhjin, 텔레포트 상태 값 설정.
	int GetTelePortState();								// 2007-09-05 by dhjin, 텔레포트 상태 값 얻어오기.
	BOOL IsInvincibleTelePortState(void);				// 2007-09-20 by cmkwon, 텔레포트 수정 - 
	void SetTelePortSummonFirst(BOOL i_bTelePortSummonFirst);	// 2007-11-06 by dhjin, 텔레포트 처음 소환 된 상태인지 체크
	BOOL GetTelePortSummonFirst();						// 2007-11-06 by dhjin. 텔레포트 처음 소환 된 상태인지 값 얻어오기

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-10 by dhjin, 아레나 통합 - 
	void InitmapCharacterUniqueNumberMapProject();		// 2008-03-10 by dhjin, 아레나 통합 - 현재 맵에서 게임중인 캐릭터들의 리스트 초기화
	void TeamArenaLeaveCommand(BYTE i_byInflMask);		// 2008-03-12 by dhjin, 아레나 통합 - 현재 맵에 지정 된 세력 유저 모두를 메인 서버로 보낸다.

	//////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, 인피니티(기지방어) -	
	void CreateKeyMonster(CFieldMonster	*i_pKeyMonster);

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
//	void DeleteKeyMonster(CMonster	*i_pKeyMonster);
	void OnInfinityMonsterDead(CFieldMonster *i_pFMon);

	BOOL SetKeyMonsterClientIdx_AliveForGameClear(CFieldMonster	*i_pKeyMonster);	
	BOOL PenaltyKeyMonster_AliveForGameClear(char * i_szCharacterName, int i_PenaltyValue);
	void DeadKeyMonsterClientIdx_AliveForGameClear();
	void SendKeyMonsterHP_AliveForGameClear(CFieldIOCPSocket * i_pFISoc);

	// start 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
	BOOL InitTriggerMapBuffManager();		
	BOOL IsEnableTriggerMapBuff();			
	CTriggerMapBuffManager *GetTriggerMapBuffManager()		{ return m_pTriggerMapBuffManager; }
	// end 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	void ChangeObject(ObjectIdx_t DeleteObjectUID, ObjectNum_t NewObjectNum);
	void SendChangeObjectList(CFieldIOCPSocket *pFISoc);

	void CreateMonster(INT MonsterUnitKind, SummonCnt_t SummonCount = 1, Distance_t RandomDistance = 0);
	void SetAllMonsterDeadItemPay(TimeUnit_t TimeLimit, ItemNum_t	ItemNum);
	void ClearAllMonsterDeadItemPay();

	void SendTimeLimit(CFieldIOCPSocket *pFISoc);
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	void OnDoMinutelyWorkFieldMapChannel(void);		// 2012-03-05 by hskim, 드랍 아이템 일정 시간후 삭제
	
	///////////////////////////////////////////////////////////////////////////
	// virtual
	virtual BOOL InitMapChannel(void);

#ifdef _DEBUG
	// 디버깅용
	char* DBG_GET_CHARACTER_NAME(int idx);
	float DBG_GET_X_POSITION(int idx);
	float DBG_GET_Y_POSITION(int idx);
	float DBG_GET_Z_POSITION(int idx);
	void DBG_PrintAllUnitPosition(void);
#endif // _DEBUG_endif

	void ApplySkillToAllInChannel(ItemNum_t i_nSkillItemNum, BYTE i_nInfluenceMask=INFLUENCE_TYPE_ALL_MASK);	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 맵에 모든 유저에게 스킬 적용

public:
	CFieldMapWorkspace	*m_pFieldMapWorkspace;
	CFieldMapProject	*m_pFieldMapProject;
	CFieldIOCP			*m_pFieldIOCPMapChannel;

	mtmapUID2FieldIOCPSocket	m_mapCharacterUniqueNumberMapProject;	// 현재 맵에서 게임중인 캐릭터들의 리스트

protected:
	EN_NETWORK_STATE		m_enTotalNetworkState;						// IOCPServer Network State 와 Map Network State 을 통합 한것
	DWORD					m_dwModulusNumberTotalNetworkState;

	DWORD					m_dwMaxUserCountsChannel;					// 2004-11-22 by cmkwon, 채널별 MaxUserCounts <--// 채널별로 MaxBandwidth를 따로 설정할수 있게 채널별로 멤버 변수를 둔다
	CRITICAL_SECTION		m_crtlMapTraffic;
	STrafficInfo			m_MapTraffic[2];

	CFieldMonster			*m_ArrFieldMonster;
	CRacing					*m_pRacing;

	INT						m_TelePortState;						// 2007-09-05 by dhjin, 텔레포트 시스템 상태값.
	DWORD					m_dwTeleportStateChangedTick;			// 2007-09-20 by cmkwon, 텔레포트 수정 - 
	BOOL					m_TelePortSummonFirst;					// 2007-11-06 by dhjin, 텔레포트가 소환 되면 초기값을 TRUE로 설정한다.

	CFieldMonster			*pKeyMonster_AliveForGameClear;			// 2010-03-31 by dhjin, 인피니티(기지방어) - 해당 몬스터가 죽으면 인피니티 종료

public:
	CCityWar				*m_pRefCityWar1;

	DWORD					m_dwLastTickVCNInfluenceAttackedMsg;	// 2005-12-28 by cmkwon, 바이제니유가 공격당하는 메시지를 보낸 마시작 시간
	DWORD					m_dwLastTickANIInfluenceAttackedMsg;	// 2005-12-28 by cmkwon, 알링턴이 공격당하는 메시지를 보낸 마시작 시간

	DWORD					m_dwLastTickStrategyPointMonsterDeaded;	// 2006-11-21 by cmkwon, 전략포인트 몬스터 폭파 시간
	BOOL					m_bStrategyPointMonsterDeaded;			// 2007-01-25 by cmkwon, 전략포인트 몬스터 폭파 플래그

	ClientIndex_t			m_ChangeBodyConMonsterIndex;			// 2007-09-10 by dhjin,	바디 컨디션이 BODYCON_MON_BUILDING => BODYCON_MON_BUILDED 상태가 필요한 몬스터 정보 저장. 차후 변경한다. 지금은 한 맵에 한 몬스터 뿐이없다.

	int						m_iBCUUserCount;
	int						m_iANIUserCount;
	int						m_iAverageLevelBCU;
	int						m_iAverageLevelANI;

protected:
	CTriggerMapBuffManager	*m_pTriggerMapBuffManager;				// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	STRIGGER_ALL_MONSTER_DEAD_ITEM_PAY	m_AllMonsterDeadItemPay;

	vector<CTriggerFunction *>	m_vectorTriggerFunction;			// 초기 설정 이후 변경되지 않는다 동기화 필요 없음
	mtvectObjectChange			m_mtVectObjectChange;
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
};

EVENTINFO* CFieldMapChannel::GetTileEventInfoW(const D3DXVECTOR3 *pPosVector3)
{
	return m_pFieldMapProject->GetTileEventInfo(pPosVector3);
}


#endif //_ATUM_MAP_FIELD_CHANNEL_H_
