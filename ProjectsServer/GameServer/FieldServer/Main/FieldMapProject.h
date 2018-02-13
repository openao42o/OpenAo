// FieldMapProject.h: interface for the CFieldMapProject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDMAPPROJECT_H__A2E572D2_32EF_46A9_B701_F89F32D50966__INCLUDED_)
#define AFX_FIELDMAPPROJECT_H__A2E572D2_32EF_46A9_B701_F89F32D50966__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "MapProject.h"
#include "AtumProtocol.h"

#define MAX_MODULUS_NUMBER_TOTAL_NETWORK_STATE			10
#define MIN_MODULUS_NUMBER_TOTAL_NETWORK_STATE			3

typedef vector<SHOP_ITEM>	vectSHOP_ITEM;				// 2006-10-20 by cmkwon
class CShopInfo
{
public:
	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	CShopInfo()		{ m_OwnerCharacterUniqueNumber = 0; }
	virtual ~CShopInfo()	{ }
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	SHOP_ITEM *GetSHOP_ITEMPtr(int i_itemNum);
	SHOP_ITEM *GetSHOP_ITEMPtrFrombuffer(int i_itemNum);	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 

	void SetAllRemainCountForLimitedEdition(int iCount);	// 2011-10-28 by hskim, EP4 [트리거 시스템]
	void SetShopOwner(UID32_t OwnerCharacterUniqueNumber);	// 2011-10-28 by hskim, EP4 [트리거 시스템]
	UID32_t GetShopOwner();									// 2011-10-28 by hskim, EP4 [트리거 시스템]

// 2006-10-20 by cmkwon
//	map<UINT, SHOP_ITEM>	m_mapShopItem;			// ItemNum -> SHOP_ITEM
	vectSHOP_ITEM			m_vectShopItemList;		// 2006-10-20 by cmkwon
	GBUFFER					m_buffer;
	UID32_t					m_OwnerCharacterUniqueNumber;		// 2011-10-28 by hskim, EP4 [트리거 시스템]
};

typedef vector<WARP_TARGET_MAP_INFO_4_EXCHANGE>		 vectWARP_TARGET_MAP_INFO_4_EXCHANGE;		// 2007-01-29 by cmkwon

struct S_S1_CHARACTER_INFO		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 구조체 추가
{
	UID32_t			CharacterUID1;
	char			CharacterName1[SIZE_MAX_CHARACTER_NAME];
};
typedef mt_vector<S_S1_CHARACTER_INFO>			mtvectS_S1_CHARACTER_INFO;		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 -

///////////////////////////////////////////////////////////////////////////////
/// \class		CFieldMapProject
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-26 ~ 2004-03-26
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CFieldMapChannel;
class CFieldMonster;
class CCityWar;

class CODBCStatement;
class CFieldMapProject : public CMapProject
{
public:
	CFieldMapProject(CMapWorkspace * i_pMapWorkspace, PROJECTINFO * i_pProjectInfo);
	virtual ~CFieldMapProject();

	void SetNPCServerStartFlag(BOOL i_bStartFlag);
	BOOL GetNPCServerStartFlag(void);
	BOOL IsNPCServerMonsterCreated(void);		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - NPC 서버의 몬스터가 생성 되었는지 확인, 주의 : 맵에 몬스터가 없으면 영원이 FALSE

	BOOL LoadBUILDINGNPC(CODBCStatement *i_pOdbcStmt);			// 현재 맵에 속한 건물 및 담당 NPC의 정보를 loading
	BOOL LoadShopInfo(CODBCStatement *i_pOdbcStmt);			// 현재 맵에 속한 상점의 판매 아이템을 미리 loading
	BOOL LoadCityTargetWarpMap(CODBCStatement *i_pOdbcStmt);	// 도시에서 워프해나갈 수 있는 맵의 리스트를 미리 loading
	BOOL LoadWarpableUserList(CODBCStatement *i_pOdbcStmt);		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 로딩 함수 추가

	// 캐릭이 죽었을 때 가야하는 default map과 관련
	BOOL SetDeadDefaultMap(void);
	CFieldMapProject* GetDeadDefaultMapProject(void);
	MapIndex_t GetDeadDefaultMapIndex(void);

	// Warp 관련
	D3DXVECTOR3 GetDefaultPoint(void); // position vector가 잘 못 되었을 때 혹은 죽었을 때, 기본 위치를 알기 위해
	EVENTINFO* GetEventInfoByWarpIndex(int WarpIndex);
	D3DXVECTOR3 GetRandomWarpPoint(int EventAreaIndex);
	void GetRandomPositionInRadius(D3DXVECTOR3 *In, D3DXVECTOR3 *Out, float Radius);
	EVENTINFO *GetEventInfoByObjectTypeAndDistance(DWORD i_dwObjType, D3DXVECTOR3 *i_pVec3Position);
	OBJECTINFOSERVER *GetRandomObjectInfoServerPtrByEventIndex(short i_sEventIndex);
	EVENTINFO *GetEventInfoByMinimumDistance(EventType_t i_eventTy, D3DXVECTOR3 *i_pVec3Position);
	int GetRandomWarpTargetEventIndex(void);

	WARP_TARGET_MAP_INFO_4_EXCHANGE *GetWarpInfoByTargetMapIndex(MapIndex_t i_mapIdx);


	BOOL ResetAllChannel(void);
	BOOL FieldOnMonsterCreateW(MSG_FN_MONSTER_CREATE * i_pMonCreate);
	BOOL FieldOnMonsterDeleteW(MSG_FN_MONSTER_DELETE * i_pMonDelete);
	BOOL FieldOnMonsterMoveW(MSG_FN_MONSTER_MOVE * i_pMonMove);
	BOOL FieldOnGetCharacterInfoW(MSG_FN_GET_CHARACTER_INFO * i_pGetCInfo);
	BOOL FieldOnMissileMoveW(MSG_FN_MISSILE_MOVE * i_pMissileMove);
	BOOL FieldOnMonsterHPRecoveryW(MSG_FN_MONSTER_HPRECOVERY * i_pHPRecovery);
	BOOL FieldOnMonsterHideW(MSG_FN_MONSTER_HIDE * i_pMonHide);
	BOOL FieldOnMonsterShowW(MSG_FN_MONSTER_SHOW * i_pMonShow);
	BOOL FieldOnBattleAttackPrimaryW(MSG_FN_BATTLE_ATTACK_PRIMARY * i_pAttackPri);
	BOOL FieldOnBattleAttackSecondaryW(MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSec);
	BOOL FieldOnBattleAttackFindW(MSG_FN_BATTLE_ATTACK_FIND * i_pAttackFind);
	BOOL FieldOnMonsterChangeBodyconditionW(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChangeBody);
	BOOL FieldOnMonsterSkillUseSkillW(MSG_FN_MONSTER_SKILL_USE_SKILL * i_pUseSkill);
	BOOL FieldOnMonsterSkillEndSkillW(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	BOOL FieldOnMonsterAutoDestroyedW(MSG_FN_MONSTER_AUTO_DESTROYED * i_pAutoDestroyed);
	BOOL FieldOnMonsterChangeOkW(MSG_FN_MONSTER_CHANGE_OK * i_pMonChangeOk);		// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가

	CFieldMonster * GetFieldMonsterW(ChannelIndex_t i_channIdx, ClientIndex_t i_monsterIdx, int i_nTestIdx);

	// 이동 가능 좌표인지 체크
	BOOL CheckCharacterPosition(D3DXVECTOR3 *pPosVector3, BOOL bFlagChangeableHeight);

	// 날씨 변경
	BOOL ChangeWeather(BitFlag16_t weather, BOOL bOnOff);

	// NPC 서버에 전송
	BOOL Send2NPCServer(BYTE *pData, int nSize);
	BOOL SendDelayBuffer2NPCFerver(void);

	// 같은 맵에 있는 캐릭터들 모두에게 MSG 보내기
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - SendMessageToAllInAllMapChannels, 인자추가(i_nStartCityMapIdx)
	void SendMessageToAllInAllMapChannels(BYTE *buffer, int size, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	void SendSummonMessageToAllChannels(MSG_FN_ADMIN_SUMMON_MONSTER *i_pSummonMonster);

	CFieldMapChannel *GetFieldMapChannelByIndex(int i_nIndex, BOOL i_bOnlyEnabledChannel=FALSE);
	CFieldMapChannel *GetRandomFieldMapChannel(BOOL i_bOnlyEnabledChannel);
	CFieldMapChannel *GetFirstFieldMapChannel(BOOL i_bOnlyEnabledChannel);
// 2004-12-07 by cmkwon, 채널 실시간 증가가 없어 필요없는 함수
//	CFieldMapChannel *GetLastFieldMapChannel(BOOL i_bOnlyEnabledChannel);
	void MakeMsg_AllChannelState(int *o_pChannelCounts, MSG_FN_CONNECT_SET_CHANNEL_STATE *o_pChannelState);
	void OnDoMinutelyWorkFieldMapProject(void);
	void OnDoSecondlyWorkFieldMapProject(ATUM_DATE_TIME *pDateTime);
	BOOL IsMultiMapChannel(BYTE i_byCharInflTy);

	///////////////////////////////////////////////////////////////////////////
	// virtual
	// 초기화 관련, 2004-03-26 by cmkwon
	virtual BOOL InitMapProject(void);

	// 채널 관련, added by kelovon, 20040324
	virtual BOOL InitializeMapChannels(int i_nChannel);
	virtual BOOL CleanMapChannels();
	virtual BOOL IncreaseMapChannels(int i_nChannelToAdd);

	void SetCityWarFieldMapProject(CCityWar *i_pCityWar);
	BYTE GetCityWarTeamTypeByGuildUID(UID32_t i_guildUID);
	BOOL IsCityWarStarted(void); 
// 2005-12-28 by cmkwon, 사용하지 않음
//	BOOL IsCheckQuestForWarp(CFieldIOCPSocket *i_pFISock);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - CFieldMapProject에 멤버 함수 추가
	S_S1_CHARACTER_INFO *FindInWarpableUserList(UID32_t i_charUID);
	BOOL FindInWarpableUserListByCharacterName(S_S1_CHARACTER_INFO *o_ps1CharInfo, char *i_szCharName);
	BOOL InsertWarpableUserList(UID32_t i_charUID, char *i_szCharName);
	BOOL PopInWarpableUserListByCharacterName(S_S1_CHARACTER_INFO *o_ps1CharInfo, char *i_szCharName);
	BOOL IsWarpableUser(UID32_t i_charUID, USHORT i_usRace);
	BOOL SendWarpableUserList(CFieldIOCPSocket *i_pFISoc);
	static INT GetConferenceRoomMapIndex(BYTE i_byCharInflTy);

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	// 맵 입장 1회 제한
	void SetEnterOneTimeLimited(BOOL bEnable);							// 사용 여부 설정
	BOOL IsEnableEnterOneTimeLimited();									// 사용 여부 확인
	void InsertEnterOneTimeLimited(UID32_t CharacterUID);				// 입장 캐릭터 리스트 추가
	BOOL IsEnterOneTimeLimited(UID32_t CharacterUID);					// 기존에 입장한적 있는지 확인
	BOOL IsGameStartNotAllowedUser(UID32_t CharacterUID);				// 게임 시작시 허용되지 않은 유저인지 확인
	void ResetEnterOneTimeLimited();									// 기존 기록 삭제

	// 소속 여단만 입장
	void SetEnterGuildLimited(BOOL bEnable, UID32_t GuildUID = 0);		// 사용 여부 설정
	BOOL IsEnterGuildLimited(UID32_t GuildUID);							// 사용 여부 확인

	// 특정 캐릭만 입장
	void SetEnterCharacterLimited(BOOL bEnable, UID32_t CharacterUID = 0);	// 사용 여부 설정
	BOOL IsEnterCharacterLimited(UID32_t CharacterUID);						// 사용 여부 확인

	// 소속 세력만 입장
	void SetEnterInfluenceLimited(BOOL bEnable, BYTE InfluenceType = 0);	// 사용 여부 설정
	BOOL IsEnterInfluenceLimited(BYTE InfluenceType);						// 사용 여부 확인

	// 맵 최대 입장 인원 
	void SetEnterMaxiumUserLimited(BOOL bEnable);						// 사용 여부 설정
	BOOL IsEnterMaxiumUserLimited();									// 사용 여부 확인
	INT GetCurrentUser();												// 현재 맵의 전체 유저 숫자

	// 맵 입장 조건 확인
	BOOL IsEnterCondition(CFieldIOCPSocket *i_pFISoc);					// 입장 조건 화인

	Err_t IsEnableWarpByMapProject(CFieldIOCPSocket *i_pFISoc, BOOL bGameStart = FALSE);					// 워프 추가 검사
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// 2015-05-26 Future, Futures Shop Functions
	CShopInfo* GetShopInfoByBuildingNPC(UINT buildingIndex);
	CShopInfo* GetCashShopInfo();

public:
	CFieldMapProject			*m_pDeadDefaultMapProject;					// 캐릭이 죽었을 때 가야하는 default map과 관련
	char						m_szUserFriendlyMapName[SIZE_MAX_MAP_NAME];	// 사용자가 읽을 수 있는 맵 이름
	BitFlag16_t					m_Weather;									// 날씨, WEATHER_XXX

	ez_map<UINT, BUILDINGNPC*>	m_mapBuilding2Building;					// BuildingIndex -> BUILDINGNPC, 건물 및 담당 NPC의 정보, 종료시 memory 해제 필요!
	ez_map<UINT, BUILDINGNPC*>	m_mapNPC2Building;						// NPCIndex -> BUILDINGNPC, 건물 및 담당 NPC의 정보, 종료시 memory 해제 불필요!
	GBUFFER_SIZE_MAX_PACKET		m_buffGetBuildingListOK;				// Previously created message buffer for MSG_FC_CITY_GET_BUILDING_LIST_OK
	GBUFFER_SIZE_MAX_PACKET		m_buffGetWarpTargetMapListOK;			// Previously created message buffer for MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK

// 2007-01-29 by cmkwon, 아래와 같이 수정함
//	list<WARP_TARGET_MAP_INFO_4_EXCHANGE>	m_listCityWarpTargetMapInfo;	// 도시맵일 경우 위프해나갈 수 있는 맵의 정보
	vectWARP_TARGET_MAP_INFO_4_EXCHANGE		m_vectCityWarpInfoList;			// 2007-01-29 by cmkwon

	BOOL						m_bNPCServerStart;			// NPC Server 가 준비 되었는지의 플래그로 T_FN_MAPPROJECT_START_OK_ACK를 받은 후 TRUE로 된다.
	BOOL						m_bNPCServerMonsterCreated;	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - NPC Server 에 몬스터 소환 메시지를 보내도 되는지 확인.. 맨 처음 몬스터 생성 패킷이 도착후 TRUE 됨
	CCityWar					*m_pRefCityWar2;

	int							m_nNextWarpTargetEventIndex;			// 2006-08-03 by cmkwon, 
	CFieldMapWorkspace			*m_pFieldMapWorkspace;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-30 by cmkwon, 회의룸 시스템 구현 -
	mtvectS_S1_CHARACTER_INFO	m_mtvectWarpableUserList;		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 멤버 변수 추가

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	BOOL LimitedEUpdate(UINT i_nBuildIdx, vectITEM_W_COUNT *i_pLimitedItemList);

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
public:
	BOOL						m_bEnableEnterOneTimeLimited;		// 맵 입장 1회 제한
	mtvectCharacterUID			m_mtVectEnterOneTimeLimited;		// 맵 입장 1회 제한

	BOOL						m_bEnableEnterGuildLimited;			// 맵 입장 길드 제한
	UID32_t						m_EnterGuildLimited;				// 맵 입장 길드 제한

	BOOL						m_bEnableEnterCharacterLimited;		// 맵 입장 캐릭 제한
	UID32_t						m_EnterCharacterLimited;			// 맵 입장 캐릭 제한

	BOOL						m_bEnableEnterInfluenceLimited;		// 맵 입장 세력 제한
	UID32_t						m_EnterInfluenceLimited;			// 맵 입장 세력 제한

	BOOL						m_bEnterMaxiumUserLimited;			// 맵 최대 입장 가능 숫자 제한 사용 여부

	vectorMapEntranceCondition	m_EntranceCondition;				// 맵 입장시 필요한 요구 사항들 (현재는 아이템) - 초기에 설정함 동기화 필요 없음
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
private:
	map<UINT, CShopInfo*>		m_mapShopInfo;								// BuildingIndex -> CShopInfo
	CShopInfo* m_CashShopInfo;
};

#endif // !defined(AFX_FIELDMAPPROJECT_H__A2E572D2_32EF_46A9_B701_F89F32D50966__INCLUDED_)
