// FieldIOCP.h: interface for the CFieldIOCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDIOCP_H__F0E40C2E_D76B_4426_955E_1B43B544085D__INCLUDED_)
#define AFX_FIELDIOCP_H__F0E40C2E_D76B_4426_955E_1B43B544085D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "AtumFieldDBManager.h"
#include "FieldIOCPSocket.h"
#include "FieldMapWorkspace.h"
#include "PreWinSocket.h"
#include "IMWinSocket.h"
#include "ArenaFieldWinSocket.h"	// 2007-12-26 by dhjin, 아레나 통합 -
#include "AtumParam.h"
#include "NPCScripts.h"
#include "Quest.h"
#include "MapOwnership.h"
#include "GuildWar.h"
#include "DropItemManager.h"
#include "ServicePauseManager.h"
#include "GameEventManager.h"
#include "CityWarManager.h"
#include "InflWarManager.h"
#include "CallWarpEventManager.h"		// 2006-07-21 by cmkwon
#include "GiveItemManager.h"			// 2006-08-25 by dhjin
#include "ArenaManager.h"				// 2007-04-17 by dhjin
#include "ChatBlockManager.h"			// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - 헤더파일 추가
#include "OutPostManager.h"				// 2007-08-16 by dhjin, 전진기지
#include "CityLeader.h"					// 2007-08-21 by dhjin, CityLeader
//#include "MersenneTwister.h"					// 2007-12-10 by cmkwon, 랜덤 시스템 수정
#include "WarContributionCalculation.h"		// 2008-12-22 by dhjin, 전쟁 보상 추가안
#include "WRankingManager.h"				// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
#include "QuestMatching.h"					// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
#include "InflRateManager.h"				// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
#include "InfinityManager.h"			// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
#include "InvokingItemInfo.h"	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
#include "PetDataManager.h"		// 2010-06-15 by shcho&hslee 펫시스템
#include "DissolutionItemmanager.h" // 2010-08-31 by shcho&jskim 아이템용해 시스템 -
#include "StatisticsManager.h"			// 2012-01-16 by hskim, 통계 - 화패
#include "MonthlyArmorManager.h"		// 2013-04-18 by jhseol,bckim 이달의 아머 - 해더 추가
#include "DGAEventManager.h"

#ifdef SC_LEADER_WARP_RESTRICTION
#include "WarpRestrictionManager.h"		// 2016-01-04 Future, Leader Warp Restrictions
#endif // SC_LEADER_WARP_RESTRICTION

#if S_BONUS_KILL_SYSTEM_RENEWAL
#include "BonusKillSystem.h"
#endif

#ifdef NEMERIAN_PVP_AWARD_EFFORT
#include "LastHitManager.h"
#endif // NEMERIAN_PVP_AWARD_EFFORT

class CLogWinSocket;
class CFieldParty;
class CFieldTickManager;
class CRacingDemon;
class CCityWar;
class CCouponManager;
class CHappyHourEventManager;
class CGiveItemManager;
class CFieldODBC;
class CAllGuildItemManager;
class CBossWarContributionCalculation;		// 2008-12-22 by dhjin, 전쟁 보상 추가안
class CInnovaLibrary;						// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
//class CSecurityManager;						// 2011-06-22 by hskim, 사설 서버 방지
class CMapTriggerManager;					// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
class CStatisticsManager;					// 2012-01-16 by hskim, 통계 - 화패

typedef mt_map<UID32_t, CFieldIOCPSocket*>					mtmapUID2FieldIOCPSocket;
typedef mt_map<string, CFieldIOCPSocket*, greater<string> >	mtmapName2FieldIOCPSocket, mtmapAccountName2FieldIOCPSocket;	// 2012-10-05 by jhseol, NPlay pc방 모듈 - AccountName으로 소켓 관리 멥 추가
// 2007-05-04 by cmkwon, mt_stl.h로 이동함
//typedef mt_set<string>										mtsetString;
typedef mt_map<PartyID_t, CFieldParty*>						mtmapPartyID2FieldParty;
typedef mt_map<string, INT>									mtmapUsedFameData;			// 2005-12-29 by cmkwon
typedef vector<ITEM*>										vectITEMPtr;				// 2006-08-10 by cmkwon
typedef mt_map<UID32_t, CGuildItemManager*>					mtmapAllGuildItem;
typedef vector<MONSTER2ITEM*>								vectMONSTER2ITEMPtr;		// 2006-11-21 by cmkwon
typedef mt_vector<CBossWarContributionCalculation*>			mtvectCBossWarContributionCalculation;		// 2008-12-22 by dhjin, 전쟁 보상 추가안
typedef mt_vector<CWarContributionCalculation*>				mtvectCOutPostWarContributionCalculation;		// 2008-12-22 by dhjin, 전쟁 보상 추가안
typedef mt_vector<CWarContributionCalculation*>				mtvectCStrategyPointWarContributionCalculation;		// 2008-12-22 by dhjin, 전쟁 보상 추가안

struct SARENA_USE_FIELD_SERVER_INFO
{// 2007-12-27 by dhjin, 아레나 통합 - 아레나 서버에서 사용하는 필드 서버 정보
	//USHORT		MFS_ID;
	USHORT			MFS_Port;
	CHAR			MFS_IP[SIZE_MAX_IPADDRESS];
	CHAR			MFS_Name[SIZE_MAX_SERVER_NAME];
	CIOCPSocket	*	Socket;	
};
typedef mt_vector<SARENA_USE_FIELD_SERVER_INFO>			mtvectSARENA_USE_FIELD_SERVER_INFO;

#define IS_MAP_WARP_POSSIBLE_INDEX		20000	// 2012-11-29 by jhseol, OX이벤트 맵 워프제한 - 워프가 가능한 맵인지 관리할 배열 넉넉하개 2만게
#define SM_TICK_EVENT_SUB_ITEM_EVENT	0		// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경

class CFieldIOCP : public CIOCP
{
public:
	CFieldIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	virtual ~CFieldIOCP();

	inline CFieldMapWorkspace* GetMapWorkspace();

	inline CFieldIOCPSocket* GetFieldIOCPSocket(int idx);
	CFieldIOCPSocket* GetFieldIOCPSocketByCharacterUniqueNumber(UID32_t uniqueNumber, BOOL i_bPermissionArenaState = FALSE);	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
	CFieldIOCPSocket* GetFieldIOCPSocketByCharacterName(char* i_szCharacterName);
	CFieldIOCPSocket* GetFieldIOCPSocketByAccountName(char* i_szAccountName);			// 2012-10-05 by jhseol, NPlay pc방 모듈, IP로 유저 소켓 찾기
	CHARACTER* GetCharacterByArrayIndex(int idx);
	BOOL GetUnitPlanePositonByClientIndex(CFieldMapChannel *pCurMapChannel, int i_nIdx, float *o_pX, float *o_pZ);

	void ApplyBuffSkillToGuild(INT itemNum, INT guildUID);

	void SendInRangeCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, DWORD i_dwSendCounts);
	void SendInArenaCharacterMoveOK(CFieldIOCPSocket *pFIOCPSock, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, DWORD i_dwSendCounts);		// 2007-04-26 by dhjin, 아레나 맵에서의 이동을 같은 맵, 세력의 모든 유저에게 보낸다
	void SendInRangeCharacterOtherInfo(CFieldIOCPSocket *i_pFISock, CFieldMapChannel *pFieldMapChannel);
//	void SendInRangeBattleAttackResult(ClientIndex_t idx, BYTE* pData, int Size, CFieldMapChannel *pFieldMapChannel);
//	void SendInRangeBattleAttackFindResult(ClientIndex_t idx, BYTE* pData, int Size, CFieldMapChannel *pFieldMapChannel);
	void SendInRangeChangeBodyCondition(ClientIndex_t idx, CFieldMapChannel *pFieldMapChannel, int i_nRangeSize);

	void SendInRangeMonsterMoveOK(CFieldMonster *i_pFMonster, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, USHORT i_usSendRange);

	// general-purpose function
	void SendInRangeMessageAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, ClientIndex_t clientIndexToExclude=0, BOOL i_bCharacterFlag=TRUE);
	void SendInRangeMessageExactAroundPosition(D3DXVECTOR3 pos, BYTE *pData, int Size, int i_fRadius, CFieldMapChannel *pFieldMapChannel, ClientIndex_t i_ExcludeCliIdx=0, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCP::SendInRangeMessageAroundCharacter#, 인자추가(i_nStartCityMapIdx)
	void SendInRangeMessageAroundCharacter(ClientIndex_t idx, BYTE *pData, int Size, CFieldMapChannel *pFieldMapChannel, BOOL bIncludeMe, int i_nRangeSize, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	// 
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - SendInRangeClientIndexVector, 인자추가(i_nStartCityMapIdx)
	int SendInRangeClientIndexVector(vectorClientIndex *i_pVectorCIdx, BYTE *i_pData, int i_nDataLen, ClientIndex_t i_excludeIdx=0, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	int SendInRangeClientIndexVectorAroundPosition(D3DXVECTOR3 pos, vectorClientIndex *i_pVectorCIdx, BYTE *i_pData, int i_nDataLen, CFieldMapChannel *pFieldMapChannel, int i_nRangeSize, BOOL *o_pbExistUnitInMapChannel=NULL);
	int SendString128ToClientIndexVector(vectorClientIndex *i_pVectorCIdx, BYTE string128_type, char *szString);

	// 모든 클라이언트에게 Message 보내기
	
	// 2008-09-09 by cmkwon, /세력소환 명령어 인자 리스트에 기어타입 추가 - 
	//void SendMessageToAllClients(BYTE *pData, int Size, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, BYTE i_byMinLevel=0, BYTE i_byMaxLevel=0, MAP_CHANNEL_INDEX *i_pExcludeMapChannel=NULL, BOOL i_bOnlyPCBangClient=FALSE, BOOL i_bArenaState=FALSE);
	void SendMessageToAllClients(BYTE *pData, int Size, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, BYTE i_byMinLevel=0, BYTE i_byMaxLevel=0, MAP_CHANNEL_INDEX *i_pExcludeMapChannel=NULL, BOOL i_bOnlyPCBangClient=FALSE, BOOL i_bArenaState=FALSE, USHORT i_usReqUnitKind=UNITKIND_ALL_MASK);

	void SendString128ToAllClients(BYTE string128_type, char *szString, BOOL i_bWriteLog=FALSE, BYTE i_byInfluenceMask= INFLUENCE_TYPE_ALL_MASK);
	//Send Notification to all players
	void SendWorldNotification(char* szString, BOOL bSpecialChat/*Write it into Leadechat or not?*/);
	int SendMessageToGuildMembers(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID);
	int SendGuildSummonMessage(BYTE *pData, int Size, UID32_t i_guildUID, UID32_t i_excludeCharUID, CFieldMapProject *i_pFMPro);
	void SendMessageToCityClients(BYTE *pData, int Size, BYTE i_byInflTy);

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
	BOOL Send2PreServerW(BYTE *i_pData, int i_nDataLen);

	// 2005-12-03 by cmkwon, IMServer로 보내기
	BOOL Send2IMServer(void *i_pData, int i_nDataLen);
	
	BOOL Send2NPCServerByTCP(BYTE *i_pbyData, int i_nDataLen);	// 2007-08-22 by cmkwon, 함수명 변경(<-Send2NPCServer)

	// 2008-02-25 by cmkwon, Pre,IM Server 로 Alive Packet 전송하는 함수 추가 - 
	void Send2PreAndIMOfAlivePacket(void);

	BOOL OpenUDPPortForOtherServer(void);
	BOOL CloseUDPPortForOtherServer(void);

	// 시간 반환
	void GetCurrentServerTimeInAtumDateTime(ATUM_DATE_TIME &o_AtumDateTime);

	// Item
	void LoadItemInfo(BOOL bReload = FALSE);
	void ReloadIteminfo();		// 아이템 정보만 업데이트함. 아이템이 추가되거나 제거된 경우는 적용 안 됨
	ITEM* GetItemInfo(int ItemNum);
	ITEM* GetBulletItemInfoByCaliber(BYTE i_byCaliber);
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	void ResetAllBuffSkill(BYTE InflType);
#endif
	int InsertItemAllCharacter(INT i_nItemNum, INT i_nItemCount, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, BOOL bExceptArenaMapUser = FALSE, BOOL bPCBang = FALSE);			// 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - PC방 유저 아이템 지급 추가 	// 2007-05-16 by dhjin, 아레나 체크 변수 추가.
	int InsertTimeLimiteItemAllCharacterSameMapChannel(ITEM *i_pTimeLimiteItemInfo, MAP_CHANNEL_INDEX i_mapChanIdx, BYTE* i_pMsgData, int i_nMsgSize, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK);			// 2009-01-05 by dhjin, 미션마스터 - 편대 버프 아이템 추가
	int InsertTimeLimiteItemPartyMember(ITEM *i_pTimeLimiteItemInfo, CFieldIOCPSocket *i_pUseItemFISoc, BYTE* i_pMsgData, int i_nMsgSize);			// 2009-01-05 by dhjin, 미션마스터 - 편대 버프 아이템 추가
	ITEM * CheckAllItemInfo(void);
	BOOL CheckItemInfo(ITEM *i_pItemInfo);
	ITEM * GetRandomItemFromRandomBoxItemList(void);
	ITEM * GetRandomSkillSupportItem(void);

	// 2013-05-09 by hskim, 세력 포인트 개선	
	BOOL DeleteBuffSkillAllCharacter(BYTE i_byInfluenceMask, DestParam_t desParam);
	BOOL InsertBuffSkillAllCharacter(BYTE i_byInfluenceMask, INT nItemNum);
	// end 2013-05-09 by hskim, 세력 포인트 개선

	// 2008-08-26 by cmkwon, ItemAttribute 추가 - 패키지(Package) 아이템, 함수 인자, 리턴값 수정
	//MYSTERY_ITEM_DROP * GetMysteryDropItem(CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum);
	BOOL GetMysteryDropItem(vectMYSTERY_ITEM_DROPPtr *o_pMysteryDropItemList, CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, BOOL i_bPackageItem=FALSE);
	BOOL GetLuckyDropItem(CFieldIOCPSocket *i_pFISoc, INT i_nMysteryItemDropNum, LUCKYITEM_DROP_INFO * o_pLuckyItemInfo, MSG_FL_LOG_LUCKY_DROP * o_pLuckyDropLog);	// 2009-03-31 by dhjin, 럭키머신 로그 - 로그를 만든다. // 2008-11-04 by dhjin, 럭키머신
	void CheckLuckyItemPeriod(ATUM_DATE_TIME * i_pCurrentTime);		// 2008-11-04 by dhjin, 럭키머신, 기간이 끝났는지 체크하여 새로운 기간을 설정한다.

	Err_t CheckMysteryItemDropRequirements(CFieldIOCPSocket *i_pFISoc, MYSTERY_ITEM_DROP *i_pMysteryItemDrop);


	// Rare Item - prefix or suffix
	RARE_ITEM_INFO* GetRareItemInfo(INT nCodeNum);
	void GetReqLevelApplyRare(int *o_pMinLevel, int *o_pMaxLevel, ITEM_GENERAL *i_pGItem);


	// Quest
	BOOL LoadQuestInfo(BOOL i_bReload = FALSE);
	void CleanQuestInfo();
	CQuest *GetQuestInfo(int i_nQuestIndex);
	BOOL CheckAndInitQuestInfo(CQuest *i_pQuestInfo);
	CQuest* GetFirstScenarioQuestInfoByInfluenceType(BYTE i_byInflTy);
	void SendAllQuestList(CFieldIOCPSocket *i_pTOFISock, BYTE i_byInflTy, char *i_szTxt=NULL);	// 2009-10-15 by cmkwon, 퀘스트리스트 보내기시에 퀘스트명으로 검색 기능 추가 - 


	// Monster
	MONSTER_INFO *GetMonsterInfo(int i_nMonsterUniqueNumber);
	MONSTER_INFO *GetMonsterInfo(char *i_szMonsterName);
	void LoadMonster2Item();
	void CleanMonster2Item();
	MONSTER2ITEM *GetMonster2Item(int MonsterUniqueNumber);

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	void CreateDropItems(listMonsterDropItem *o_plistMonsterDropItem, CFieldMonster *i_pDeadFMon, CFieldIOCPSocket *i_pFISockTopDamage, float i_fPartyBonus, DropType_t i_dropType = DROP_TYPE_DEAD_MONSTER);
	void TestDropItems(MONSTER_INFO *pMonsterInfo, mapMonsterDropItemTest *o_pmapMonsterDropItemTest);		// 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어

	void GetDropItemList(vectMONSTER2ITEMPtr *o_pVectDropItemList, int i_nMonsterUnitKind);

	CMapProject *GetMapProjectByMapIndex(MapIndex_t MapIndex);
	CFieldMapProject *GetFieldMapProjectByMapIndex(MapIndex_t MapIndex);
	CFieldMapChannel *GetFieldMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX i_mapChannIdx);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 인자추가(MapIndex_t i_nStartCityMapIdx)
	CFieldMapProject *GetCityFieldMapProjectPtrByInfluenceW(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	// Log 관련 함수
	void SendLogMessageServerInfo(void);

	// map 관련
/*
	inline char* GetDefaultMapName();
	inline CFieldMapProject* GetDefaultFieldMapProject();
*/
	// Item_Mixing_Info관련
	// 2008-03-17 by cmkwon, 조합 시스템 기능 추가 - 인자추가(INT i_nMixCnts)
	ITEM_MIXING_INFO* CheckAndGetItemMixingInfo(ITEM_W_COUNT_CHECK *i_arrItemWithCount, INT i_nItemWithCount, INT i_nMixCnts);
	ITEM_MIXING_INFO* CheckAndGetItemFixMixingInfo(ITEM_W_COUNT_CHECK *i_arrItemWithCount, INT i_nItemWithCount, INT i_nMixCnts, ITEM_MIXING_INFO *i_pItemMixingInfo);		// 2012-03-16 by hskim, DB 조합식 정리 (고정값 수식으로 변경)

	// 게임 유저들의 데이터를 모두 저장함
	void CharacterDataSaveRoutine();

	// 2004-03-22 by cmkwon, 패킷 레벨 시스템 관련
	BOOL CheckPacketPriority(DWORD i_dwSendCounts, DWORD i_dwModulusNumber);

	// 1분에 한번씩 처리할 것 구현(CFieldTickManager에서 호출함) 
	void OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime);
	void OnDoSecondlyWorkFIOCP(ATUM_DATE_TIME *pDateTime);

	// 1시간에 한번씩 처리할 것 구현(CFieldTickManager에서 호출함) 
	void OnDoHourlyWorkIOCP(ATUM_DATE_TIME *pDateTime);

	// 파티관련
	void DeleteFieldParty(CFieldParty *i_pFParty);
	void AdjustDeletedFieldParties(void);

	// 도시점령전 관련
	void OnCityWarStartFieldIOCP(CCityWar *i_pCityWar);
	void OnCityWarEndFieldIOCP(CCityWar *i_pCityWar, vectCITYWAR_QUEST *i_pCityWarQuestResultList);
	void OnCityWarInfoFieldIOCP(CCityWar *i_pCityWar, int i_nRemainMinute);
	void OnCityWarQuestMonsterCreated(MapIndex_t i_mapIdx);
	void OnCityWarQuestMonsterDead(CCityWar *i_pCityWar);
	void SetCityWarTeamTypeByMapIndex(CCityWar *i_pCityWar, CFieldMapProject *i_pFMPro);

	// 명성 포인트 관련
	BOOL GetP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName);
	INT CountP2PPKFamePoint(const char *szAttackCharName, const char *szDeadCharName);
	void ResetAllP2PPKFamePoint(void);

	// 2006-02-08 by cmkwon, 세력전 분포 관련
	void AddInfluenceTypeCount(BYTE i_byInflType);
	int GetInfluenceTypeDistirbutionPercent(BYTE i_byInflType);
	float GetInfluenceTypeDistirbutionTexPercent(BYTE i_byInflType);
	int ChangeMapInfluenceType2AllConflictAreaW(BYTE i_byDeadMonsterBelligerence, BOOL i_bAdd);	// 2009-01-12 by dhjin, 선전 포고 - 세력 추가 비트

	// 2006-02-09 by cmkwon, 킬마크 관련
	ITEM * GetKillMarkItemPtr(BYTE i_byCharInflTy, int i_byCharLevel);

	// 2006-03-02 by cmkwon
	void InsertCharacterDropItem(BYTE i_byInflMask, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item);
	void InsertCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask, CHARACTER2ITEM i_Character2Item);
	CHARACTER2ITEMLIST * FindCharacter2Item(BYTE i_byInflty, USHORT i_UnitKindMask);

	// 2006-04-17 by cmkwon, 전광판 시스템
	BOOL SignBoardInsertString(MSG_SIGN_BOARD_STRING *i_pSignBoardString);
	BOOL SignBoardDeleteString(int i_nStringIdx, BOOL i_bInflLeader);
	void SendSignBoardStringList(CFieldIOCPSocket *i_pFISocket, int i_nReqStringIndex=-1);
	void SignBoardCheckExpireTime(ATUM_DATE_TIME *i_pCurATime);

	// 2006-04-20 by cmkwon, 세력지도자
	void Send2LeaderANDSubLeader(BYTE i_byInflTy, BYTE *i_pData, int i_nDataLen, CFieldMapChannel *i_pExcludeFMChann);

// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 사용하지 않음
//	// 2006-06-05 by cmkwon
//	BOOL SecurityInit(void);
//	void SecurityClean(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon
	BOOL AddCallWarpEventRequest(SCALL_WARP_EVENT *i_pCallWarpEvent);
	Err_t CheckCallWarpEventRequestAck(SCALL_WARP_EVENT *o_pCallWarpEvent, DWORD i_dwCallWarpID, BOOL i_bAddUserCount=FALSE);

	// 2006-08-22 by cmkwon
	// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - SendGameEventMessage() 인자 추가
	void SendGameEventMessage(CFieldIOCPSocket *pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy=T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK);

	//////////////////////////////////////////////////////////////////////
	// 2012-10-05 by jhseol, NPlay PC방 모듈 - 해피아워 정보 가져오기
	void LoadGameEventMessage(CFieldIOCPSocket *pToFISock, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* msg);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-25 by cmkwon
	CAllGuildItemManager			*m_pAllGuildItemManager;		// 2006-09-20 by dhjin
	mtmapAllGuildItem * GetmtmapAllGuildItemPtrW(void);
	CGuildItemManager *GetGuildItemMangerNoLockW(UID32_t i_GuildUID);
	BOOL InsertGuildItemManagerNoLockW(CGuildItemManager *i_pGuildManager);

	// 2006-11-21 by cmkwon, 전략포인트 몬스터 관련
	void InitStrategyPointOccupyW(BOOL i_bVCNMapInflTyForInit);

	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldIOCP::SendWarPointToInflClient()에 인자(, BYTE i_byWPUpdateTy=WPUT_MOTHERSHIPWAR) 추가
	void SendWarPointToInflClient(BYTE i_byInflTy, int i_nWarPoint, BYTE i_byWPUpdateTy=WPUT_MOTHERSHIPWAR);		// 2007-05-15 by dhjin, WarPoint를 세력전 승리 세력에게 지급한다.
	void SendWarPointToInflClientByDGA(int i_nWarPoint, BYTE i_bInfl = INFLUENCE_TYPE_ALL_MASK);

	// 2007-05-28 by cmkwon
	// 2009-10-06 by cmkwon, 베트남 게임 가드 X-TRAP으로 변경 - CFieldIOCP::GetAllFileNameList# 수정, 인자 추가 (i_szPrefixFileName)
	static BOOL GetAllFileNameList(vectString *i_pVectFileNameList, const char *i_szDir, char *i_szPrefixFileName=NULL);

	// 2008-09-08 by cmkwon, SCMonitor에서 ReloadVersionInfo시에 일부 체크섬파일(.\Res-Tex\*.*)도 리로드하기 - 
	//BOOL LoadResObjCheckList(void);
	BOOL LoadResObjCheckList(BOOL i_bReloadOnlyRexTexDirectory=FALSE);

	SRESOBJ_CHECKSUM * GetResObjCheckSum(char *i_szFileName);

	// 2007-07-23 by cmkwon, 추가함
	STUTORIAL_INFO *GetTutorialInfo(INT i_nTutorialUID);

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 함수 추가
	void SetUnusableSpeakerItem(BOOL i_bFlag);
	BOOL GetUnusableSpeakerItem(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
	void SetPrepareShutdown(BOOL i_bPrepareShutdown);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-24 by dhjin, OutPost
	void OnOutPostProtectorDestroyW(int i_nMapIndex);		// 2007-08-24 by dhjin, 전진기지 보호막 파괴시 처리 
	void OnOutPostResetDestroyW(int i_nMapIndex);			// 2007-08-24 by dhjin, 전진기지 중앙처리장치 파괴시 처리

	//////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by dhjin, ActionByLevel
	INT GetPollPointActionByLevel(BYTE i_nLevel);			// 2007-10-31 by dhjin, 레벨에 따른 pollpoint 얻어오기

	void PayBossMonsterSuccessDefend(BYTE i_byInfluence);			// 2007-11-07 by dhjin, 모선전 방어 성공 시 보상 지급 변경으로 남아 있는 전략포인트 수 만큼 보상 일괄 지급

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-07 by cmkwon, 레어 시스템 수정 -
	// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 인자 추가(, RARE_ITEM_INFO *i_nLastRareItemInfo/*=NULL*/)
	int GetRandomRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nReqUseTypeMask, CFieldIOCPSocket *i_pFISoc=NULL, RARE_ITEM_INFO *i_pLastRareItemInfo=NULL);
	BOOL CheckFixRareCode(ITEM *i_pTargetItemInfo, BOOL i_bIsPrefix, int i_nFixedCodeNum);		// 2011-10-20 by hskim, EP4 [Free to play] - 접두/접미 고정 옵션

	// 2007-12-10 by cmkwon, 랜덤 시스템 수정 - CFieldIOCP::GetRandInt32() 추가
	UINT GetRandInt32(UINT i_uiMin, UINT i_uiMax);

	// 2007-12-27 by dhjin, 아레나 통합 -
	BOOL InsertFieldServerInfoForArenaServer(SARENA_USE_FIELD_SERVER_INFO * i_pFieldServerInfo);	// 2007-12-27 by dhjin, 아레나 통합 - 필드 서버 정보를 추가
	CIOCPSocket* GetMFSSockForArenaServer() const { return m_SARENA_USE_FIELD_SERVER_INFO.Socket; }	// 2007-12-27 by dhjin, 아레나 통합 - 필드 서버 소켓 가져오기
	void SendArenaServerPay(BYTE i_byInfluence, INT i_nPayInfluencePoint);					// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버로 보상 전송 
	void ConvertArenaRenderUserName(CHAR * i_pArenaCharacterName, CHAR * o_pSendArenaCharacterName);					// 2008-02-21 by dhjin, 아레나 통합 - 아레나 서버에서 유저 이름 '\xxx' 띠고 보여주기

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	int								m_nLimitUserCount;						// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	BOOL							m_bLockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

	void SendAllLetterByInfluence(BYTE i_byInfluence, SLETTER_INFO * i_pAllLetterInfo);	// 2008-05-09 by dhjin, EP3 편지 시스템 - 접속해 있는 같은 세력 유저에게 전체 편지 추가

	///////////////////////////////////////////////////////////////////////////////
	// 2008-10-28 by cmkwon, 대회서버 시스템 수정(MotherShip 소환시 상대 세력 도시맵으로 이동) - 
	int WarpAllInfluenceUsersToCity(BYTE i_byInflTy, BOOL i_bIncludeDiedCharacter=FALSE, BOOL bExclusionMapByMSWar = FALSE);		// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프
	BOOL IsWarpExclusionMapByMSWar(BYTE byInflTy, int nMapInfluenceType);		// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
//	BOOL IsEnableSelectInfluence(BYTE i_byInflTy);
	BOOL IsSelectableInfluence(BYTE i_byInflTy, BYTE i_byLv);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	mapINT2ItemSkillPtr				m_PartyFormationItemSkillList;
	int ArrangePartyFormationSkill(mapINT2ItemSkillPtr *o_pPartyItemSkillList, ez_map<INT, ITEM*> *i_pItemInfoList);
	ITEM_SKILL * SearchPartyFormationSkill(INT i_nItemNum);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	CInflRateManager				m_InflRateMan;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	ezmapINT2vectINT				m_mapItemParamOverlapList;
	int ResetItemParamOverlapList(vectSITEM_PARAM_OVERLAP *i_pParamOverlapList);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	mtvectITEM_W_COUNT				m_LimitedEItemList;
	void LimitedEReset(void);
	ITEM_W_COUNT *LimitedEFindItemInfoNoLock(INT i_nItemNum);
	BOOL LimitedEGetItemWCount(vectITEM_W_COUNT *o_pItemCntList);
	void LimitedEUpdateItemCount(INT i_nItemNum, INT i_nRemainCnt);
	void LimitedEUpdateShopItemW(UINT i_nBuildIdx, CFieldMapProject *i_pFMapPro);
	BOOL IsCashShopItemNoLock(INT nItemNum);		// 2013-03-13 by hskim, 웹 캐시 상점

	void GetUserCountByInfluence(int &aniCount, int &bcuCount);
	// 2013-03-29 by jhseol, 아이템 이벤트
	BOOL ItemEventSubTypeCheck(UID32_t i_nAUID, UID32_t i_nCUID, SITEM_EVENT_INFO i_SITEM_EVENT_INFO, DestParam_t i_SkipSubEventType = ITEM_EVENT_DES_EMPTY);
	void InsertTickEvent(INT			i_TickEventType,
		INT				i_nTimeIntervalInSeconds,
		INT				i_nTickEventParam1 = 0,
		INT				i_nTickEventParam2 = 0,
		void			*i_vpTickEventParam1 = NULL,
		void			*i_vpTickEventParam2 = NULL);
	// end 2013-03-29 by jhseol, 아이템 이벤트

	//////////////////////////////////////////////////////////////////////////
	// 2015-11-25 Future, Multiple IP Restriction
#ifdef S_IP_UNIQUE_CONNECTION
	BOOL MultipleIPsRestricted();
	BOOL EnableMultipleIPRestriction(CFieldIOCPSocket* pCaller, BOOL enable = TRUE);
#endif // S_IP_UNIQUE_CONNECTION

protected:
	CFieldMapWorkspace				*m_pFieldMapWorkspace;
// 2007-02-12 by cmkwon, 필요없음	vector<CFieldIOCPSocket*>		m_vecMapIOCPSocket;

	//////////////////////////////////////////////////////////////////////////
	// 2015-11-25 Future, Multiple IP Restriction
#ifdef S_IP_UNIQUE_CONNECTION
	BOOL							m_bMultipleIPRestriction;
#endif // S_IP_UNIQUE_CONNECTION

public:
	SERVER_ID						m_FieldServerID;
	CAtumFieldDBManager				*m_pAtumDBManager;
	CFieldTickManager				*m_pFieldTickManager;			// Tick manager
	//CSecurityManager				*m_pSecurityManager;			// 2011-06-22 by hskim, 사설 서버 방지
	CStatisticsManager				*m_pStatisticsManager;			// 2012-01-16 by hskim, 통계 - 화패
	CPreWinSocket					*m_pPreWinSocket;
	CIMWinSocket					*m_pIMWinSocket;
	CArenaFieldWinSocket			*m_pArenaFieldWinSocket;		// 2007-12-26 by dhjin, 아레나 통합 - 
	ez_map<INT, ITEM*>				m_mapItemInfo;
	multimap<BYTE, ITEM*>			m_multiMapCaliber2Bullet;		// 구경(Caliber) -> 총알 아이템(ITEMKIND_BULLET)
	ez_map<int, MONSTER_INFO>		m_mapMonsterInfo;
	multimap<int, MONSTER2ITEM*>	m_mapMonster2Item;
	ez_map<INT, CQuest*>			m_mapQuestInfoFromScript;
	ez_map<INT, ENCHANT_INFO>		m_mapEnchantInfo;
	vector<ITEM_MIXING_INFO>		m_vectorItemMixingInfo;
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;
	ez_map<BYTE, vector<RARE_ITEM_INFO* > > m_mapLevel2RareItemInfo;

	mtmapUID2FieldIOCPSocket		m_mapCharacterUniqueNumber;
	mtmapName2FieldIOCPSocket		m_mapCharacterName;
	mtmapAccountName2FieldIOCPSocket	m_mapAccountName;			// 2012-10-05 by jhseol, NPlay pc방 모듈 - AccountName으로 소켓 관리 멥
	mtsetString						m_mtsetAccountName;

	// Party
	mtmapPartyID2FieldParty			m_MapParties;
	mt_vector<CFieldParty*>			m_mtvectorDeletedFieldParties;

	// map 관련
	MapIndex_t						m_DefaultMapIndex;		// 서버 전체의 기본 맵, 캐릭터 생성 시 기본 위치
	CFieldMapProject*				m_pDefaultMapProject;	// 서버 전체의 기본 맵, 캐릭터 생성 시 기본 위치

	// NPC Scripts 관련
	ez_map<NPCID_t, NPCScript>		m_NpcScriptsMap;

	// 마인류
	float							SIZE_MAX_MINE_REACTION_RANGE;		// 마인류의 MAX 반응 범위

	UINT							m_uiAccumulatedUserCounts;
	UINT							m_uiMaxUserCounts;
	UINT							m_uiAccumulatedMonsterCounts;
	
	CFieldIOCPSocket				*m_pNPCTCPSocket;				// NPC와의 TCP Connection	

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 사용하지 않는 부분임
//	CMapOwnershipManager			m_MapOwnershipManager;			// 도시(맵) 소유권 관련, 공성/경제 등	

	CGuildWarMananger				m_GuildWarManager;				// 길드전 관리자	
	CDropItemManager				m_DropItemManager;				// 드랍 아이템 관리자	
	CServicePauseManager			m_ServicePauseManager;			// 각종 서비스 일시 정지 관리자
	CRacingDemon					*m_pRacingDemon;				// 레이싱 게임을 처리할 데몬	
	CGameEventManager				m_gameEventManager;				// 게임 이벤트 관리

	CCityWarManager					m_cityWarManager;				// 도시점령전 관련
// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 삭제됨
//	CCouponManager					*m_pCouponManager;				// 쿠폰 관리자
	CHappyHourEventManager			*m_pHappyHourEventManager;		// Happy Hour Event 관리자

	CDGAEventManager				*m_pDGAEventManager;

	CGiveItemManager				*m_pGiveItemManager;			// 2006-08-24 by dhjin,

	CFieldODBC						*m_pFieldODBC;					// 

	mtmapUsedFameData				m_mtmapUsedFameDataList;			// 2005-12-29 by cmkwon, 사용된 명성 정보로 하루에 한번씩 초기화 된다.

	// 2006-02-07 by cmkwon, 세력전 분포 관련
	int								m_nNormalInfluenceCount;		// 2006-02-07 by cmkwon, 바이제니유 일반군 카운트 - 실제 필요한 수치가 아니므로 정확한 값이 아니다.
	int								m_nVCNInfluenceCount;			// 2006-02-07 by cmkwon, 바이제니유 정규군 카운트
	int								m_nANIInfluenceCount;			// 2006-02-07 by cmkwon, 알링턴 정규군 카운트
	
	// 2006-02-09 by cmkwon, 킬마크 관련
	mtvectorItemPtr					m_mtvectItemPtrKillMarks;		// 2006-02-09 by cmkwon, 킬마크 아이템 리스트

	vectCHARACTER2ITEMLIST			m_vectCharacter2ItemList;		// 2006-03-02 by cmkwon, 

	// 2006-04-12 by cmkwon, 세력전 보강 시스템
	CInflWarManager					m_InflWarManager;

	// 2006-04-17 by cmkwon, 전광판 시스템
	INT								m_nSignBoardStringIndex;		// 2006-04-17 by cmkwon
	mtvectMSG_SIGN_BOARD_STRINGList	m_mtvectSignBoardStringList;	// 2006-04-17 by cmkwon

	// 2006-06-29 by cmkwon, 
	BOOL							m_bInitHackShield;				// 2006-06-29 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon
	CCallWarpEventManager			m_callWarpEventManager;			// 2006-07-21 by cmkwon

	BOOL							m_bPrepareShutDown;				// 2006-08-04 by cmkwon, 서버 다운 진행상태

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-10 by cmkwon, RandomBox
	vectITEMPtr						m_vectITEMPtr4RandomBox;		// 2006-08-10 by cmkwon, 랜덤박스 아이템을 위한 벡터
	vectITEMPtr						m_vectITEMPtrSkillSupportItems;	// 2006-09-29 by cmkwon, 스킬 보조 아이템 리스트

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-17 by cmkwon, 하루 시간 제한 시스템 관련
	BOOL							m_bUseTimeLimiteSysteOfToday;	// 2006-11-17 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// 2007-04-10 by cmkwon, 대회서버 DB 초기화 명령어 인증숫자
	int								m_nJamboreeInitCommandVerifyNumber;

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, 아레나 관련
	CArenaManager					m_ArenaManager;


	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-11 by cmkwon, 미스테리캡슐 아이템 시스템
	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, 럭키머신 multimap -> mt_multimap으로 변경
//	mmapINT2MYSTERY_ITEM_DROP	m_mmapMysteryItemDropList;
	mtmapINT2MYSTERY_ITEM_DROP	m_mtmapMysteryItemDropList;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-28 by cmkwon
	mapstring2SRESOBJ_CHECKSUM		m_mapResObjCheckSumList;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-11 by cmkwon, 아레나블럭 시스템 구현 - m_arenaBlockManager 추가
	CChatBlockManager				m_arenaBlockManager;

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-11 by dhjin, Tutorial
	vectSTutorialInfo				m_vectTutorial;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-24 by cmkwon, 스피커아이템 사용 가능/금지 설정 기능 추가 - 변수추가
	BOOL							m_bUnusableSpeakerItem;

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-16 by dhjin, 전진기지
	COutPostManager					m_OutPostManager;

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, CityLeader
	CCityLeader						m_CityLeader;

	// 2007-10-29 by dhjin, 레벨에 따른 여러 행동들 정의 값
	vectSACTION_BY_LEVEL_DB			m_vectSACTION_BY_LEVEL_DB;

	// 2007-12-10 by cmkwon, 랜덤 시스템 수정	
	//MTRand							m_mtRandManager;	// 2007-12-10 by cmkwon
	//mt_lock							m_lockRandManager;	// 2007-12-10 by cmkwon

	// 2007-12-27 by dhjin, 아레나 통합 - 아레나 서버에서 사용하는 필드 서버 정보
	SARENA_USE_FIELD_SERVER_INFO	m_SARENA_USE_FIELD_SERVER_INFO;	

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-25 by dhjin, 태국 PC방 IP정보 로딩
	INT								m_nPCBangMinTick;
	INT								m_nMaxPCBangMinTick;

#ifdef NEMERIAN_PVP_AWARD_EFFORT
	LastHitManager *m_pLastHitManager;
#endif

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, 럭키머신
	vectSLUCKY_MACHINE				m_vectLuckyMachine;

	// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 기존 소스 수정 - 
	int GetLuckyMachineItemList(vectSLUCKY_MACHINEPtr *o_pLuckyMachineItemList, INT i_buildingIdx, INT i_machineNum, INT i_coinItemNum);
	INT GetLuckyMachineKind(INT i_buildingIdx, INT i_machineNum, INT i_coinItemNum);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-08 by dhjin, 미션마스터
	void SearchMissionMaster(int i_nQuestIdx, CFieldIOCPSocket * i_pFISoc, vector<CFieldIOCPSocket*> * o_pVecMissionMasterIOCPSocket);
	void SendHelpMissionMaster(vector<CFieldIOCPSocket*> * i_pVecMissionMasterIOCPSocket, BYTE* i_pMsgData, int i_nMsgSize);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-22 by dhjin, 전쟁 보상 추가안
	mtvectCBossWarContributionCalculation			m_mtvectCBossWarContributionCalculation;
	mtvectCOutPostWarContributionCalculation		m_mtvectCOutPostWarContributionCalculation;		
	mtvectCStrategyPointWarContributionCalculation	m_mtvectCStrategyPointWarContributionCalculation;	
	void WarContributionCalculation(CFieldIOCPSocket * i_pFISoc, UID32_t i_nValue);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 기여도 계산하여 추가한다.
	UID32_t WarContributionCalculationByGear(USHORT i_sUnitKind, UID32_t i_nValue);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 각 기어별 기여도 계산
	void WarDeathContributionCalculation(CFieldIOCPSocket * i_pFISoc);					// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 유저 죽은 카운트 추가
	void WarOtherInflStayTime(CFieldIOCPSocket * i_pFISoc, BOOL i_bCheckStart);			// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 상대세력에 머문시간
	void WarStartOtherInflStayTimeForAllUser();											// 2009-01-06 by dhjin, 전쟁 보상 추가안 - 전쟁 시작시 상대세력에 이미 있던 유저들 머문시간 설정
	void InsertCBossWarContributionCalculation(MapIndex_t i_nMapIndex, int i_nStartWarContributionPoint);
	void InsertCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex);
	void InsertCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex);
	void DeleteCBossWarContributionCalculation(MapIndex_t i_nMapIndex);
	void DeleteCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex);
	void DeleteCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex);
	void PayCBossWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl);	// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 모선전 승리세력에게 보상 지급
	void PaySendBossWarContribution(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 모선전 개인 보상
	void PaySendBossWarContributionLeader(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 지도자 보상
	void PaySendBossWarContributionGuild(CBossWarContributionCalculation * i_pBossWarContribution, BYTE i_byWinInfl);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 길드 보상
	void PaySendBossWarContributionGuildProcess(UID32_t i_nGuildUID, UID32_t i_nPay, UID64_t i_nContribution, MapIndex_t i_nMapIndex, BYTE i_byPayGrade);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 길드 보상 처리
	void PayCOutPostWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl);	// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 전진기지 승리세력에게 보상 지급
	void PayCStrategyPointWarContributionCalculation(MapIndex_t i_nMapIndex, BYTE i_byWinInfl);	// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 전략포인트 승리세력에게 보상 지급
	void PaySendWarContribution(CWarContributionCalculation * i_pWarContribution, BYTE i_byWinInfl, int i_nDefinePayWarType, BYTE i_byWarType);		// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 개인 보상

	// 2010-06-01 by shcho, GLogDB 관련 -
	void SendBossGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime);
	void SendStrategyPointGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime);
	void SendOutPostGLogEventParticipationRate(MapIndex_t i_nMapIndex, INT i_nCnt, ATUM_DATE_TIME * i_pWarStartTime);


	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	CWRankingManager			m_WRankingManager;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	CQuestMatching				m_QuestMatching;
	BOOL						m_bWarningStateForSelectInfluence;

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 -
	CInfinityManager			m_InfinityManager;
	void ResetInfinityByResetCycle(ATUM_DATE_TIME *i_pCurDateTime);
	void ResetInfinityByResetCycleByServerStart();

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	//vectorTenderInfo			m_TenderInfoList;
	//BOOL CheckTenderItem(ItemNum_t i_CheckItemNum);

	vectorInfinityShopInfo		m_InfinityShopInfoList;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	CInvokingItemInfo			m_InvokingItemInfo;			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	BOOL SetTenderItemList(vectorTenderInfo *i_pTenderItemList);

	// 2010-08-05 by dhjin, 버닝맵 - 
	BurningMapInfoList			m_BurningMapList;
	BURNING_MAP_INFO * GetBurningMapInfo(MapIndex_t i_nMapIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-06-15 by shcho&hslee 펫시스템
	CPetDataManager		m_cPetDataManager;

	///////////////////////////////////////////////////////////////////////////////
	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 클래스 추가
	CDissolutionItemManager  m_cDissolutionItemManager;

	///////////////////////////////////////////////////////////////////////////////
	// 2011-11-03 by shcho, yedang 셧다운제 구현 - 셧다운 시킬 유저 정보 컨테이너
	vectShutDownUserData m_ShutDownData;
	void FindShutdownUserAndStartShutDownRoutin(ATUM_DATE_TIME *i_pCurDateTime); // 셧다운 진행 함수

	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	CMapTriggerManager		*m_pMapTriggerManager;

	// 2013-03-13 by hskim, 웹 캐시 상점
	BOOL					m_bServerActive;
	BOOL					IsServerOn()		{ return (TRUE == m_bServerActive && FALSE == m_bPrepareShutDown); }
	// end 2013-03-13 by hskim, 웹 캐시 상점

	// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	UID64_t					m_MaximumItemUID;			// 적용 당시의 최대 아이템 번호
	UID64_t					GetItemSecurityMaximunUID()		{ return m_MaximumItemUID; }
	void					SetItemSecurityMaximunUID(UID64_t MaximumItemUID)		{ m_MaximumItemUID = MaximumItemUID; }

	void					LoadItemSecurityMaximunUID();
	UID64_t					GeneratorItemSecurityCheckSum(UID64_t nUniqueNumber, INT ItemCount);
	// end 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	
	// 2015-11-24 Future, checks for IP availability
	bool					PeerIPAvailable(CFieldIOCPSocket* pCallingSocket);

	// 2016-01-04 Future, Leader Warp Restriction
#ifdef SC_LEADER_WARP_RESTRICTION
	CWarpRestrictionManager	m_WarpRestrictionManager;
#endif // SC_LEADER_WARP_RESTRICTION

public:
	int* GetUserLevelList(int i_UserLevelList[10]);
	int GetPCBangUserCount();										// 2007-01-25 by dhjin, 현재 플레이 하고 있는 PC방 유저수
	virtual BOOL IOCPInit(void);									// CAtumDBManager 를 먼저 초기화 하기 위해 virtual로 구현
	virtual void IOCPClean(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);
	virtual void CalcTotalTrafficInfo(void);
	BOOL LoadFieldServerDataDebug(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-08 by cmkwon, 서버에서 맵로딩시 AlivePacket 전송 추가 - 
	virtual void SendAlivePacket2OtherServers(void);

	// 2012-11-29 by jhseol, OX이벤트 맵 워프제한
	BOOL m_bIsMapWarpPossible[IS_MAP_WARP_POSSIBLE_INDEX];		// DB에 저장 하질 않아 서버가 시작되면 초기화 됨
	BYTE ReversalMapWarpPossible(int i_nMapindex);				// 해당맵의 워프 On/Off 를 반전해주는 함수
	BOOL IsMapWarpPossible(int i_nMapindex);					// 해당맵의 워프 가능을 확인하는 함수
	// end 2012-11-29 by jhseol, OX이벤트 맵 워프제한

	///////////////////////////////////////////////////////////////////////////////
	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 워프제한
	void SetNextMapWarpPossible(MapIndex_t i_nMapIndex, BOOL i_bWarpPossible);
	BOOL GetNextMapWarpPossible(MapIndex_t i_nMapIndex);
	BOOL m_bTriggerNextMapWarpPossible[IS_MAP_WARP_POSSIBLE_INDEX];		// 해당맵의 워프 가능을 저장할 배열
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 워프제한

	// 2013-04-18 by jhseol,bckim 이달의 아머 - m_pMonthlyArmorManager 선언
	CMonthlyArmorManager	*m_pMonthlyArmorManager;
	// end 2013-04-18 by jhseol,bckim 이달의 아머 - m_pMonthlyArmorManager 선언

	// 2015-06-13 Future, Cashing of KillCount in case of User disconnect
#if S_BONUS_KILL_SYSTEM_RENEWAL
	mt_map<UID32_t, KILLCOUNT_CACHE_DATA>	m_mtmapCharacterKillCounts;
	void CacheCharacterKillCount(UID32_t characterUID, INT count);		// Caches Killcount of a Character
	INT GetCachedKillCount(UID32_t characterUID);						// Gets Cached Killcount for a Character
	void CleanKillCountCache();											// Cleans expired Cache Data
	void CleanKillCountCacheFromCharacter(UID32_t fromCharacterUID);	// 2015-12-26 Future, clean cached killcount from a specific character
#endif // S_BONUS_KILL_SYSTEM_RENEWAL
	// end 2015-06-13 Future, Cashing of KillCount in case of User disconnect
};

CFieldIOCPSocket* CFieldIOCP::GetFieldIOCPSocket(int idx)
{
	return (CFieldIOCPSocket*)GetIOCPSocket(idx);
}

CFieldMapWorkspace* CFieldIOCP::GetMapWorkspace()
{
	return m_pFieldMapWorkspace;
}

/*
char* CFieldIOCP::GetDefaultMapName()
{
	return m_DefaultMapName;
}

CFieldMapProject* CFieldIOCP::GetDefaultFieldMapProject()
{
	return m_pDefaultMapProject;
}
*/

#endif // !defined(AFX_FIELDIOCP_H__F0E40C2E_D76B_4426_955E_1B43B544085D__INCLUDED_)
