// FieldIOCPSocket.h: interface for the CFieldIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDIOCPSOCKET_H__6FB6A18E_0C74_4E46_AB75_2ECA21C4204B__INCLUDED_)
#define AFX_FIELDIOCPSOCKET_H__6FB6A18E_0C74_4E46_AB75_2ECA21C4204B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCP.h"
#include "FieldMapChannel.h"
#include "AtumSJ.h"
#include "Quest.h"
#include "GeneralRequestHandler.h"
#include "FieldItemManager.h"
#include "MTCriticalSection.h"
#include "FieldSkillManager.h"
#include "FieldTimerManager.h"
#include "CountdownChecker.h"
#include "FieldCharacterQuest.h"
#include "GuildDamageManager.h"

#include "BazaarManager.h"				// 2006-07-26 by cmkwon
#include "MemoryHackHistory.h"			// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
#include "SpeedHackHistory.h"			// 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현

#include "BillingWikigames.h"				// 2008-08-07 by cmkwon, Wikigames_Eng 빌링 수정 - 

#include "LetterManager.h"				// 2008-04-23 by dhjin, EP3 편지 시스템 - 

#include "InvokingItemManager.h"			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템


#include "ArenaManager.h"
#include "ArenaTeam.h"

#include "ArenaDeathMatchRoom.h"	// 2012-07-10 by jhseol, 아레나 판정 - 데미지 누적

#include "ArmorCollection.h"		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - #include "ArmorCollection.h" 추가

#ifdef NEMERIAN_PVP_AWARD_EFFORT
#include "LastHitManager.h"
#endif

enum CS_State
{
	CS_NOTCONNECT		= 0,			// Client가 접속하지 않은 상태
	CS_CONNECTED		= 1,			// Client가 Socket 접속한 상태
	CS_RECVLOGIN		= 2,			// Client가 AccountName을 가지고 Login시도중(Pre Server에서 확인중)
	CS_LOGINED			= 3,			// Client가 AccountName을 가지고 Login 된 상태
	CS_RECVGETCHARACTER	= 4,			// Client가 Login을 하고 선택한 Character 정보를 받기 위한 처리중(DB 확인중)
	CS_GOTCHARACTER		= 5,			// Client가 Login을 하고 선택한 Character 정보를 얻은 상태
	CS_PLAYING			= 6,			// Client가 선택한 Character로 Game을 시작한 상태
	CS_WARP_PROCESSING	= 7,			// Client가 워프 처리중
	CS_ARENASERVER_PLAYING	= 8,		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - CS_ARENA_PLAYING => CS_ARENASERVER_PLAYING 변경 // 2008-01-04 by dhjin, 아레나 통합 - 아레나 대전 중이다.
};

enum EnumWarpType
{
	WT_SAME_MAP					= 0,
	WT_DIFF_MAP_SAME_SERVER		= 1,
	WT_DIFF_MAP_DIFF_SERVER		= 2
};

struct TradeItem
{
	UID64_t			ItemUniqueNumber;
	int				Count;
	ITEM_GENERAL	*pStoreItem;

	TradeItem(): ItemUniqueNumber(0), Count(0), pStoreItem(NULL) {}

#ifdef _ATUM_SERVER
	void* operator new(size_t size) { return VMemPool::vmObjectNew(size); }

	void operator delete(void* p) { VMemPool::vmObjectDelete(p, sizeof(TradeItem)); };
#endif
};

struct SPEED_HACK_DATA
{
	int			nServerTimeGap;
	int			nSumClientTimeGap;
	int			nRecvMoveCounts;
	int			nSumServerDistanceGap;
	int			nCurrentSpeed;

	void ResetSPEED_HACK_DATA()
	{
		nServerTimeGap			= 0;
		nSumClientTimeGap		= 0;
		nRecvMoveCounts			= 0;
		nSumServerDistanceGap	= 0;
		nCurrentSpeed			= 0;
	}
};

constexpr auto	ARRAYSIZE_SPEED_HACK_DATA = 3;

constexpr auto CHARACTER_SPEED_HACK_CHECK_TERM = 10000;


class CFieldIOCP;


struct FIELD_DUMMY
{
	UINT		ItemFieldIndex;
	float		Endurance1;			// 남은 내구도
	INT			ItemNum;
	ITEM		*pItemInfo;			// 아이템의 정보

	FIELD_DUMMY(UINT i_uiItemFieldIdx, ITEM *i_pItemInfo)
	{
		ItemFieldIndex	= i_uiItemFieldIdx;

		if (i_pItemInfo)
		{
			Endurance1	= i_pItemInfo->Endurance;
			ItemNum		= i_pItemInfo->ItemNum;
			pItemInfo	= i_pItemInfo;
		}
	}

#ifdef _ATUM_SERVER
	void * operator new(size_t size) { return VMemPool::vmObjectNew(size); }

	void operator delete(void* p) { VMemPool::vmObjectDelete(p, sizeof(FIELD_DUMMY)); };
#endif
};

enum EnumQueryType;

enum EnumResetVarType
{
	RVT_CONSTRUCTOR = 0,
	RVT_GAME_END = 1,
	RVT_ON_CLOSE = 2
};

// FMTT_XXX, Fieldserver Message Transfer Target
constexpr BitFlag16_t FMTT_CHARACTER_IN_RANGE = 0x0001;	// 주위의 캐릭터
constexpr BitFlag16_t FMTT_PARTY_IN_RANGE = 0x0002;	// 같은 맵의 주위의 파티원
constexpr BitFlag16_t FMTT_PARTY_IN_MAP = 0x0004;	// 같은 맵의 모든 파티원, FMTT_PARTY_IN_RANGE 포함
constexpr BitFlag16_t FMTT_GUILD_IN_RANGE = 0x0008;	// 같은 맵의 주위의 길드원
constexpr BitFlag16_t FMTT_GUILD_IN_MAP = 0x0010;	// 같은 맵의 모든 길드원, FMTT_GUILD_IN_RANGE 포함
constexpr BitFlag16_t FMTT_P2P_PK = 0x0020;	// 1:1 전투 상대

class CFieldMonster;
class CGuildItemManager;
class CPCBangIPManager;

typedef mt_map<UINT, FIELD_DUMMY*>			mtmapItemFieldIndex2FieldDummy;
typedef mt_vector<FIELD_DUMMY*>				mtvectFieldDummy;
typedef mt_set<DROPMINE*>					mtsetDropMine;
typedef mt_map<UID64_t, TradeItem>			mtmapUID2TradeItem;
typedef mt_vector<CHARACTER>				mtvectCharacter;
typedef mt_map<INT, CFieldCharacterQuest>	mtmapQuestIndex2FieldCharacterQuest;
typedef mt_vector<CFieldIOCPSocket*>		mtvectObersverCharacter;
typedef mt_vector<CFieldIOCPSocket*>		mtvectWatchCharacter;
typedef mt_vector<STUTORIAL_LIST_INFO>		mtvectTutorialList;
typedef mt_map<MapIndex_t, MapIndex_t>		mtmapMapIdx2MapIdx; 
typedef vector<CFieldIOCPSocket*>			vectCFieldIOCPSocket;
typedef mt_vector<CFieldIOCPSocket*>		mtvectCFieldIOCPSocket;
typedef vector<MOSTSTAYED_MAP_INFO>			vectStayedMapInfo;

class CFieldIOCPSocket : public CIOCPSocket
{
	friend class CPreWinSocket;
	friend class CIMWinSocket;
	friend class CArenaFieldWinSocket;	
	friend class CFieldIOCP;
	friend class CAtumFieldDBManager;
	friend class CFieldParty;
	friend class CAtumLogSender;
	friend class CGeneralRequestHandler;
	friend class CFieldItemManager;
	friend class CRacing;
	friend class CFieldTimerManager;
	friend class CArenaDeathMatchRoom;
	friend class CArmorCollection;

public:
	CFieldIOCPSocket();
	virtual ~CFieldIOCPSocket();

	CMTCriticalSection	m_mtCritSecForGameEndRoutine;
	CHARACTER			m_character;						// 접속한 Client의 Character정보 구조체
	UID32_t				m_nArenaCharacterUID;				// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.
	char				m_szMACAddress[SIZE_MAX_MAC_ADDRESS];	// 2015-09-23 Future, adding of MAC Address Logging

	INT					m_nPlayCharacterType;	// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
	UsingItemBitFlag	m_structUsingItemBitFlag;			// 2012-12-06 by jhseol, 서치아이 범위 캐릭터 중심으로 변경
	SARENA_INFO			m_ArenaInfo;						// 2007-04-17 by dhjin, Arena 정보 값
	BYTE				m_GuildRank;							// 2006-09-25 by dhjin, 유저의 길드 랭킹 정보
	char				m_szCharacterMent[SIZE_STRING_32];	// 캐릭터 멘트
	BOOL				m_bStealthState;
	BOOL				m_bStealthStateBySkill;				// 2013-05-09 by hskim, 세력 포인트 개선						
	INT					m_nPlusHP;
	INT					m_nPlusDP;
	BOOL				m_bDeadReasonByPK;					// 1:1 대결로 인해 죽은 상태
	BYTE				m_byCityWarTeamType;				// 도시점령전시 공격측, 방어측, 관람측 구분
	BYTE				m_bySelectableInfluenceMask;		// 선택 가능한 세력Mask, 하나의 계정에는 하나의 세력만 선택가능
	Experience_t		m_ExperienceDelayStore;				// 2006-05-21 by cmkwon, 경험치 지연 저장 시스템
	ATUM_DATE_TIME		m_AccountRegisteredDate;			// 2006-06-02 by cmkwon
	char				m_AccountPasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];	// 2006-06-02 by cmkwon
	BOOL				m_bPreServerAuthenticated;			// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - 게임서버 로그인/로그아웃 처리, PreServer 인증 플래그 변수 추가

	mtvectFC_CONNECT_LOGIN_INFO m_mtvectAllCharacterInfo;   // 2006-09-15 by dhjin, 계정에 모든 케릭터 정보
	mtvectCharacter		m_mtvectCharacterCache;		// GetCharacter한 정보를 저장하기 위한 vector
	int					m_nNumCharacters;			// 접속한 Client의 모든 Character 수(최대 3, SIZE_MAX_NUM_CHARACTER)
	CS_State			m_CSClientState;			// 접속한 Client의 상태 정보
	BOOL				m_bFlagDBStore;				// 종료시에 DB에 Client의 종료 위치정보를 DB에 저장하기 위한 플래그
	DWORD				m_dwConnectTime;			// Client가 접속한 시간(초단위)
	//DWORD				m_dwTickLastAttackPrimary;	// Primary 마지막 공격 시간(Tick단위)
	//DWORD				m_dwTickLastAttackSecondary;// Secondary 마지막 공격 시간.
	BOOL				m_bNotifyCloseFlag;
	EVENTINFO			*m_pCurrentEventInfo;		// 다른 필드 서버로의 워프 시 정보를 전달하기 위해 사용 -> used @ CPreWinSocket::OnRecvdPacket()의 case T_FP_EVENT_NOTIFY_WARP_OK
	USHORT				m_nCurrentWarpAreaIndex;	// 다른 필드 서버로의 워프 시 WarpArea 정보를 전달하기 위해 사용 -> used @ ResEventWarpConnect, check: 사용하는가? 확인하기...
	CFieldParty			*m_pFieldParty;
	BOOL				m_bFieldServerChangeFlag;	// 다른 필드서버로의 워프나 ConnectGameStart시 close에서 사용하기 위해 정의

	CRITICAL_SECTION	m_criticalLevelAndExperience;	// 캐릭터의 경험치와 레벨의 동기화를 위해

	INT					m_nMapMoveCount;				// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

	// Map 관리 위해 추가, added by ywlee 20021029
	CFieldMapChannel	*m_pCurrentFieldMapChannel;		// Character가 속해 있는 맵에 대한 pointer, initialize값 할당
	CFieldMapProject	*m_pFieldMapProjectForNPC;

	int					m_nOnlineEachOtherFriendCnts;			// 2006-07-18 by cmkwon, 서로 등록된 친구리스트에서 온라인 친구 카운트

	int					m_nGuildBonusExpRate;			// 2012-10-04 by jhjang, 여단에 현재 접속중인 온라인 단원 카운트
	int					m_nKillCount;					// 2012-10-04 by jhjang, 킬카운트 보너스 카운트
	bool				m_bIsPayKillCountBonusItem;		// 2012-10-04 by jhjang, 킬카운트 보너스 카운트
	int					m_nInSamePartyCount;			// 편대 경험치 보너스.


	CFieldItemManager			m_ItemManager;
	ITEM						m_arrAttachItems[SIZE_MAX_POS];
	ITEM						&m_ItemProw;			// 2008-09-23 by dhjin, 레이더 // 선두 (라이트계열 + 방어계열)
	ITEM						&m_ItemProwIn;			// Primary 장착, 선두의 안쪽, (컴퓨터 계열)
	ITEM						&m_ItemProwOut;			// Primary 장전, 선두의 바깥쪽, 장착 (빔계열 or 캐논계열)
	ITEM						&m_ItemWingIn;			// Secondary 장착, 날개의 안쪽 , (현재 안 쓰임, 20041111, kelovon)
	ITEM						&m_ItemWingOut;			// Secondary 장전, 날개의 바깥쪽, 장착(로켓계열 or 미사일계열)
	ITEM						&m_ItemCenter;			// 아머 장착
	ITEM						&m_ItemRear;			// 엔진계열


	ITEM						&m_ItemAccessoryUnLimited;	// 2006-03-30 by cmkwon, 무제한 악세사리 - 부착물(후미 우측-연료탱크|컨테이너계열)
	ITEM						&m_ItemAccessoryTimeLimit;	// 2006-03-30 by cmkwon, 시간제한 악세사리(후미 좌측)

	ITEM						&m_ItemPet;				// 2010-06-15 by shcho&hslee 펫시스템 - 소유한 펫 정보

	CUID16Generator				m_WeaponIndexGenerator;	// MSG_FC_BATTLE_ATTACK_OK의 WeaponIndex 할당
	
	// 스킬 관리
	CFieldSkillManager			m_SkillManager;

	// 타이머 관리
	CFieldTimerManager			m_TimerManager;
	
	CArmorCollection			m_ArmorCollection;		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - m_ArmorCollection 선언
	
	Experience_t				m_InfCurrentAllEXP;
	
	LONG						m_nInsertingStoreItemCounts;
	DWORD						m_dwLastTickInsertingStoreItem;


	mtmapItemFieldIndex2FieldDummy	m_mapFieldDummy;

	// 마인류 관리용
	mtsetDropMine				m_setCharacDropMine;

	// 기체를 그리기 위한 정보 저장 - 장착 아이템 등
	CHARACTER_RENDER_INFO		m_UnitRenderInfo;

	// 현재 들어가 있는 건물 저장
	BUILDINGNPC					*m_pCurrentBuildingNPC;


	int							m_nCrystalAttackCount;				// 2012-12-15 by jhseol, 전쟁 시스템 리뉴얼 - 전쟁시 트리거 무적상태(일시정지) 크리스탈 공격횟수 저장

	ATUM_DATE_TIME				m_atAccountLastBuyDate;				// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
	ATUM_DATE_TIME				m_atCharacterLastBuyDate;			// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
	ATUM_DATE_TIME				m_atLoginDate;						// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
	
	BOOL						m_bProcessingStoreGetItem;			// 2012-12-11 by hskim, 베트남 아이템 복사 버그 보안 (DB 처리가 끝날때까지 관련 패킷은 무시)
																	// 해당 m_bProcessingStoreGetItem 는 QT_GetStoreItem / QT_GetGuildStoreItem 실행시에 항상 TRUE 변경되어야 함
																	// 해당 변수가 FALSE 시점은 DB 상에 처리가 다끝났을 경우에 적용함
																	// 2012-12-11 by hskim, 베트남 아이템 복사 버그 보안 (DB 처리가 끝날때까지 관련 패킷은 무시)		
																	// 길드 관련 버그가 추가로 발견됨
																	// 해당 길드 아이템을 인벤토리로 가져오는 부분의 QT_DeleteGuildStoreItem / QT_UpdateGuildStoreItem 에도 해당 부분 TRUE 변경 되도록 처리됨

	CParamFactor				m_ParamFactor;

private:
	// DB Log용
	DWORD						m_nGameStartTimeInSeconds;			// 초단위

	// PK 모드 관련
	BOOL						m_bPKMode;					// PK 가능 여부, PK 가능 지역일 때 클라이언트의 요청에 의해 TRUE로 설정
	ClientIndex_t				m_peerP2PPKClientIndex;		// 1대1 PK 진행시 할당, 기본값: INVALID_CLIENT_INDEX
	BOOL						m_bP2PPKStarted;			// 1대1 PK가 시작되었는지 여부, (PK 요청 중 상태는 제외!)
	INT							m_nP2PPKPreferredMode;		// 1대1 PK가 시작되었는지 여부, (PK 요청 중 상태는 제외!)
	INT							m_nP2PPKRequestedMode;		// 1대1 PK가 시작되었는지 여부, (PK 요청 중 상태는 제외!)

	// 거래
	UID32_t						m_peerTraderCharacterUniqueNumber;	// 거래하는 상대방, 값이 INVALID_UNIQUE_NUMBER이면 거래중 아님
	BOOL						m_bOKTradeChecked;					// 거래 완료가 왔을 때 TRUE, 두 거래자가 모두 TRUE이면 거래 수행
	mt_lock						m_lockTrade;						// 거래를 위한 lock, 직접 사용 안 함
	mt_lock						*m_pCurrentLockTrade;				// 거래에 실제로 사용되는 lock
	mtmapUID2TradeItem			m_mapTradeItem;

	mtvectClientIndex_t			m_mtvectAttackTargetMonsterIndex;		// 자신이 공격 했던 MonsterIndex


	BitFlag16_t					m_enableChatFlag16;

	BYTE						m_STRING_128_print_level;

	BOOL						m_bSendMessgeTypeDBG;

	DWORD						m_nOldTimeStampDBG;

	mtmapUID2EnchantVector		m_mapEnchant;		// (ItemUniqueNumber) -> (ENCHANT List)


	mtmapQuestIndex2FieldCharacterQuest	m_mapQuest;	// lock 처리 확실히!


	USHORT						m_backupRace;


	DWORD						m_dwSendCountsMoveOK;
	DWORD						m_dwSendCountsAttack;


	DWORD						m_dwLastTickSpeedHackCheck;			// Speed Hack 체크를 한 마지막 Tick
	SPEED_HACK_DATA				m_arrSpeedHackCheck[ARRAYSIZE_SPEED_HACK_DATA];

	CGeneralRequestHandler		m_RequestHandler;


	CFieldMapChannel			*m_pRacingFieldMapChannel;
	BYTE						m_byRacingNextCheckPointNumber;
	BYTE						m_byRacingRanking;


	CCountdownChecker			m_countdownChecker;


	UID32_t						m_uidAttackerGuildUID;


	DWORD						m_dwLastTickRolling;		// 롤링을 사용한 tick


	ITEM						*m_pGiveKillMarkItemPtr2Killer;		// 2006-02-09 by cmkwon, 타세력 유저에 의해 적었을때 그 유저에게 줘야할 킬마크

	int							m_nDeletedPacketCount;	// 2008-03-13 by cmkwon, 대규모 전쟁시 클라이언트 팅기는거 수정 - 임시용

	vectCFieldIOCPSocket		m_VecMissionMasterIOCPSocket;		// 2008-12-22 by dhjin, 미션마스터

#if S_BONUSEXPSYSTEM_RENEWAL
	INT							m_nRestExperienceCount;				// 2012-10-07 by hskim, 휴식 경험치
#endif

	CMemoryHackHistory			m_MemoryHackHistory;				// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
	CSpeedHackHistory			m_SpeedHackHistory;					// 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현



// 2015-10-04 Future, decking detection
#ifdef S_ANTI_DECK
	mt_lock			m_lockLastHit;
	ENEMY_LAST_HIT	m_LastHit;
#endif 

public:
	static CFieldIOCP			*ms_pFieldIOCP;



	void InitFieldIOCPSocket();

	BOOL IsTrivialMessageType(MessageType_t msgType);

#ifdef _DEBUG
	BOOL SendAddData(BYTE* pData, int nSize);  // check: EVENT 관련 MSG 못 보내는 등의 문제 있음! 좋은 방법 찾아야 함.
#endif
	BOOL SendAddDataBuffer(GBUFFER_SIZE_MAX_PACKET &i_gbuffer, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST);

	// Property Functions
	void SetCurrentFieldMapChannel(CFieldMapChannel *i_pCurrentFieldMapChannel);
	CFieldMapChannel* GetCurrentFieldMapChannel();
	CFieldMapProject * GetCurrentFieldMapProject(void);
	CFieldMapChannel *GetFieldMapChannel(MAP_CHANNEL_INDEX *i_pMapChannelIndex, BOOL i_bFindOtherChannel = FALSE, BOOL i_bFindDefaultMapChannel = FALSE);
	void SetSelectableInfluenceMask(BYTE i_byMask);
	BYTE GetSelectableInfluenceMask(void);
	void SetAccountInfo(MSG_FP_CONNECT_AUTH_USER_OK *i_pFPAuthUserOK);


	void ResetAllVariables(EnumResetVarType type);										// 생성자, OnClose, GameEnd 등에서 모든 멤버 변수 리셋하기

	inline BOOL IsUsingRestrict(ClientIndex_t ClientIndex, UID32_t CharacterUniqueNumber);	// CFieldIOCPSocket의 인스턴스가 사용 중인지의 여부를 리턴, restrict checking by kelovon

	// 캐릭터 
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	float GetPlusRateExp(INT nInSamePartyCount = 1);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	float GetPlusRateSPI(void);
	float GetPlusRateDropItem(void);
	float GetPlusRateDropRareItem(void);
	float GetPlusRateExpRepair(void);
	float GetHPRepairRateFlighting(void);
	float GetHPRepairTimeSubtractRate(void);
	float GetDPRepairTimeSubtractRate(void);
	float GetSPRepairTimeSubtractRate(void);
	float GetPlusWarPointRate(void);						// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가		

	// 캐릭터 관련
	const CHARACTER* GetCharacter() const { return &m_character; }
	CS_State GetClientState() const { return m_CSClientState; }

	inline void SetClientState(CS_State csState);
	void CharacterGameEndRoutine(void);
	BOOL CharacterSaveCriticalData(BOOL i_bSynchExec = FALSE);
	BOOL CharacterGameStartRoutine(GameStartType type);
	BOOL CheckCurrentMapOnGameStart(void);	// 2007-12-26 by cmkwon, CurrentMap 체크 시스템 수정 - CFieldIOCPSocket::CheckCurrentMapOnGameStart() 추가
	void CharacterDeadRoutine(BYTE damageType, CFieldMonster *i_pAttackMonster=NULL, CFieldIOCPSocket *i_pAttackUser=NULL);	// 캐릭터가 죽고 GameEndRoutine() 혹은 DEAD_GAMESTART 하기 이전(사이)에 처리될 사항들
	BOOL CharacterDeadGameStartRoutine(BOOL i_bNotify, int i_nRecoverHP=0, int i_nRecoverDP=0, int i_nRecoverSP=0, int i_nRecoverEP=0);		// 캐릭터 부활 처리
	void OnWarpDone(EnumWarpType warpType);		// 워프 완료 후 게임 시작 시 처리 사항
	LONGLONG GetTotalPlayTimeInSeconds();		// 총 게임시간 반환
	LONGLONG GetCurrentPlayTimeInSeconds();		// 직전 게임 시작 이후 현재까지의 게임 시간 반환
	LONGLONG GetPCBangTotalPlayTimeInSeconds(); // 2007-06-07 by dhjin, PC방 총 플레이 시간 반환
	BOOL CompareCharacterName(char *i_szCharacterName);
	CFieldMapProject *GetDefaultCityMapProject(void);
	INT GetDownSPICountOnDead(void);
	void CharacterChangeModeRoutine(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 인자추가(MapIndex_t i_nStartCityMapIdx)
	CFieldMapProject *GetCityFieldMapProjectPtr(BYTE i_byInfluence=INFLUENCE_TYPE_UNKNOWN, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	CFieldMapChannel *GetCityFieldMapChannelPtr(BYTE i_byInfluence=INFLUENCE_TYPE_UNKNOWN, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	CFieldMapProject *GetGardenFieldMapProjectPtr(BYTE i_byInfluence=INFLUENCE_TYPE_UNKNOWN, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);


	void ProcessSPIPenaltyOnDead(BYTE i_nItemUpdateType=IUT_PENALTY_ON_DEAD);
	BOOL IsCityCurrentFieldMapChannel(void);

	// 2010-04-22 by cmkwon, 아레나 플레이 상태도 유효한 캐릭터로 처리 - 
	BOOL IsValidCharacter(BOOL i_bLiveCheck=TRUE, BOOL i_bPermissionArenaState=FALSE);

	Err_t IsEnableWarp(int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex=0, BOOL i_bCancelTrade=FALSE, CFieldMapProject *i_pTargetFieldMapProject = NULL);	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 // 2010-02-01 by cmkwon, 거래시 미션맵이동 워프 버그 수정 -
	Err_t IsEnableWarpToCityMap(void);

	Err_t ClearCharacterForWarp(void);	// 2009-02-05 by cmkwon, 전쟁시 강제 워프 관련 처리 수정 - 

	// 2008-01-10 by cmkwon, IsCheckInfluenceTypeANDSocketType() 버그 수정 - IsCheckInfluenceTypeANDSocketType() 인자 추가
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - IsCheckInfluenceTypeANDSocketType, 인자추가(i_nStartCityMapIdx)
	BOOL IsCheckInfluenceTypeANDSocketType(BYTE i_bySenderInfluenceTyMask, int i_nOnlySocketTy=ST_ALL, BOOL i_bGameMasterOK=TRUE, MapIndex_t i_nStartCityMapIdx=0);
	BOOL IsCheckLevel(BYTE i_byMinLevel, BYTE i_byMaxLevel);

// 2008-09-12 by cmkwon, "/명성" 명령어 추가 - 
//	void AddCharacterFame(int i_nAddFameValues=1);
	void AddCharacterFame(int i_nAddCharacterFameValues=1, int i_nAddGuildFameValues=1);
	UINT AddCharacterWPRewardByKill();
	UINT AddCharacterSPIRewardByKill();
	BOOL AddCredits(INT amount);	// 2015-08-04 Future, adding of credits

	BOOL IsGoToInfluenceCity(BYTE i_byCharInfluenceTy, INT i_nMapInfluenceTy);
	BOOL IsValidGuild(void);
	void StoreDB_QPARAM_CHARACTER_CHANGE_STAT(void);
	void StoreExperienceDelayStore(void);

	// 캐릭터 속성 변경
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	Experience_t ChangeExperience(Experience_t fChangeValue, BOOL i_bApplyPartner = FALSE, BOOL i_bApplyPlusRate=TRUE, BOOL i_bCheckValidGameUser=TRUE, INT nInSamePartyMember=1, CFieldMonster* nMonsterLevel = NULL);		// 2011-09-02 by hskim, 파트너 시스템 2차
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	void ChangeLevel(int nLevelChangeValue, int nPercentageOfExp = 0);
	void CheckAndUpdateStatus();				// 신분 변화 확인하기
	void ChangeCurrentHP(float i_fChangeValHP, BYTE i_nDamageType= DAMAGE_BY_NA, ClientIndex_t i_SkillUseClientIdx=0);
	void ChangeCurrentDP(float i_fChangeValDP, ClientIndex_t i_SkillUseClientIdx=0);
	void ChangeCurrentSP(float i_fChangeValSP);
	void DecreaseCurrentSP(int i_nChangeValSP, INT i_nSkillBaseNum=0);
	void ChangeCurrentEP(float i_fChangeValEP);
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	void ChangeHP(UINT i_nHP);
	void ChangeDP(UINT i_nDP);
	UINT GetCharacterTotalHP(void);
	UINT GetCharacterTotalDP(void);
#else
	void ChangeHP(USHORT i_nHP);
	void ChangeDP(USHORT i_nDP);
	USHORT GetCharacterTotalHP(void);
	USHORT GetCharacterTotalDP(void);
#endif	
	void ChangeSP(USHORT i_nSP, BOOL i_bStore2DB=FALSE);
	void ChangeEP(USHORT i_nEP, BOOL i_bStore2DB=FALSE);
	USHORT GetCharacterTotalEP(void);
	USHORT GetCharacterTotalSP(void); //// 2010-08-26 by shcho&jsKim, 밤 아이템 구현

	float DecreaseCharacterHPDP(float i_fChangeValue, BYTE i_enumDamageType, BOOL i_bCheckRecovery=TRUE
		, CFieldMonster *i_pAttackMonster=NULL, CFieldIOCPSocket *i_pAttackCharacter=NULL);

// 2010-03-05 by cmkwon, 발동류 관련으로구조 변경 - 
//	void CheckArmorState(void);					// 아머 장착 여부에 따른 적용 사항 확인
//	void DistributeBonusStatByAutoStatType(void);
//	void CheckArmor_SendTotalStat(BOOL i_bRecalculateComputerItem=FALSE);

	//void ApplyArmorParamFactor(void);			// 아머의 ParamFactor 적용
	void CheckComputerState(ITEM *i_pItemInfo, BOOL i_bAttach, BOOL i_bSendChangeInfo=TRUE);	// 컴퓨터 아이템 장착 여부에 따른 적용 사항 확인, i_bAttach가 TRUE이 장착 시, FALSE 해제 시
	void CheckAllState(INT SataCount, BOOL i_bAttach, BOOL i_bSendChangeInfo=TRUE);		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - CheckAllState() 선언

	// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 게임 리스타트시에 CurrentDP에 아머의 추가 DP만큼 빠지는 버그 수정 
	void ApplyComputerGearStat(DestParam_t desParam, float paramValue, BOOL i_bSendInfo=TRUE);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)

	BOOL ChangeStat(BYTE i_nStatKind, Stat_t i_nChangeValue);
	Err_t ResetStat(int i_nStat);
	void SendStat_UpdateDB(BOOL i_bSendGearStat=TRUE, BOOL i_bSendTotalGearStat=TRUE, BOOL i_bUpdateDB=FALSE);
	void UpdateDBtoBonusStatPoint(void);
	BOOL ChangeCharacterInfluenceType(BYTE i_byInfluenceTy, BOOL i_bAdminCommand=FALSE);
	void ApplyParamFactorWithAccesoryTimeLimiteItem(ITEM *i_pItemInfo);
	void ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItemW(void);
	void ArenaWarpSetHPDPSPEP();								// 2007-05-11 by dhjin, 아레나 맵으로 이동시 처리 해야되는 HP,DP,SP,EP
	void ArenaWarpCitySetHPDPSPEP(BOOL i_bSendClient=TRUE);		// 2007-05-11 by dhjin, 아레나 맵에서 마을로 이동시 처리 해야되는 HP,DP,SP,EP
	void RearrangeCharacterTatalStat(BOOL i_bSendGearStat=TRUE, BOOL i_bSendTotalGearStat=TRUE, BOOL i_bUpdateDB=FALSE);		// 2010-03-05 by cmkwon, 발동류 관련으로구조 변경 - 

	// 기체를 그리기 위한 정보 loading
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - InitRenderInfo()로 함수명 변경
	//void UpdateRenderInfo();									// RenderInfo를 모두 갱신함
	void InitRenderInfo(void);
	BOOL UpdateItemRenderInfoByPOS(INT *o_pnItemNum, INT *o_pnShapeItemNum, INT *o_pnEffectItemNum, INT i_nPos, INT *o_pnPetLevel);		// 2012-12-05 by hskim, 파트너 기간제 외형 변경 - // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 추가함.

	void UpdateItemRenderInfo(INT WinPos, BOOL bSendMsg);		// Item과 관련된 RenderInfo만을 갱신함

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 
	BOOL UpdateItemRenderInfoForTransformer(INT i_nMonUnitKind, BOOL i_bAttachFlag, BOOL i_bSendToAround=TRUE);

	// body condition 관련
	static void SetBodyConditionBit(BodyCond_t &VAR, BodyCond_t MASK);
	static void ClearBodyConditionBit(BodyCond_t &VAR, BodyCond_t MASK);
//	static BOOL CompareBodyConditionBit(UINT &VAR, UINT MASK);

	// NPC와 맵 관련
	inline BOOL InitializeNPCMap(CMapProject *pmapProject, map<int, MONSTER> *mapMonsterParam);

	// Event 처리: WarpPoint가 주어지면 그 위치로 처리한다.
	EventResult_t HandleEvent(EVENTINFO *pEventInfo, const ChannelIndex_t ChannelIndex = 0, BYTE i_byCharacterMode=FALSE);
	EventResult_t WarpToMap(const MAP_CHANNEL_INDEX &MapChannel);
	EventResult_t WarpToMap(const MAP_CHANNEL_INDEX &MapChannel, D3DXVECTOR3 *pPosition, BYTE i_byCharacterMode=FALSE);
	EventResult_t WarpToMap(const MAP_CHANNEL_INDEX &MapChannel, int TargetIndex);
	EventResult_t WarpToMap(const MapIndex_t MapIndex, const ChannelIndex_t ChannelIndex);
	EventResult_t WarpToMap(const MapIndex_t MapIndex, const ChannelIndex_t ChannelIndex, D3DXVECTOR3 *pPosition);
	EventResult_t WarpToMap(const MapIndex_t MapIndex, const ChannelIndex_t ChannelIndex, int TargetIndex);
	EventResult_t WarpToCityMap(void);
	EventResult_t WarpToQuestMap(MapIndex_t i_targetMapIndex);
	BOOL SendAllChannelState(MapIndex_t i_nMapIndex, EVENTINFO *i_pEventInfo);
	BOOL SendAllChannelState(CFieldMapProject *i_pFieldMapProject, EVENTINFO *i_pEventInfo);
	BOOL SendAllChannelState(MapIndex_t i_nMapIndex, INT i_nWarpTargetIndex);
	BOOL SendAllChannelState(CFieldMapProject *i_pFieldMapProject, INT i_nWarpTargetIndex);
	EventResult_t SetWarpTargetPosition(CFieldMapProject *i_pMapProject, EVENTINFO *i_pEventInfo, AVECTOR3 *o_pWarpTargetPosition);
	D3DXVECTOR3 GetWarpTargetPosition(CFieldMapProject *i_pMapProject, EVENTINFO *i_pEventInfo);

	void ApplyMapEntranceCondition(vectorMapEntranceCondition *pEntranceCondition);		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	
	// 대상의 유효성 검사
	BOOL CheckValidAttackTargetCharacter(CFieldMapChannel *i_pFMChann, ClientIndex_t i_targetIndex);	// 공격 대상 캐릭터의 유효성 검사
	BOOL CheckValidAttackTargetCharacter(CFieldMapChannel *i_pFMChann, ClientIndex_t i_targetIndex, CFieldIOCPSocket *&o_pTargetSocket);	// 공격 대상 캐릭터의 유효성 검사
	BOOL CheckValidAttackTargetCharacter(CFieldMapChannel *i_pFMChann, ClientIndex_t i_targetIndex, CFieldIOCPSocket *&o_pTargetSocket, CHARACTER *&o_pTargetCharacter);	// 공격 대상 캐릭터의 유효성 검사
	BOOL CheckValidAttackTargetMonster(BYTE i_byAttackCharInfluenceTy, ClientIndex_t i_nTargetIndex, ChannelIndex_t i_nChannelIndex, CFieldMonster *&o_pTargetMonster);	// 공격 대상 몬스터의 유효성 검사

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - 
	BOOL CheckValidM2M(CFieldMapChannel *i_pFMChann, ClientIndex_t i_AttackerIdx, CFieldMonster **o_ppAttackMon, ClientIndex_t i_TargetIdx, CFieldMonster **o_ppTargetMon);	// M to M 공격 관련 유효성 체크

	static int CalcSplashDamageKindByMonsterAllAttack(ITEM *i_pAttackItem, float i_fDistance, int i_nAddExplosionRange);

	static void ProcessSplashDamageC2All(CFieldIOCPSocket *i_pAttFSock, SATTACK_PARAMETER *i_pAttParam, float i_fRadius, D3DXVECTOR3 *i_pVec3TargetPos);		// 2006-11-29 by dhjin, 
	static void ProcessSplashDamageMonsterAllAttack(MSG_FN_BATTLE_ATTACK_PRIMARY *i_pAttackPri, CFieldIOCPSocket *i_pAttackMonSock, CFieldMonster *pAttackMon, ITEM *pAttackItem);

	static float CalcDamageOfAttackNew(BYTE *o_pDamageKind, enumAttackToTarget i_attType, float i_fDamageWeight,
						  CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, SATTACK_PARAMETER *i_pAttParam,
						  CFieldIOCPSocket *i_pTargetSocket, void *i_pTargetUnit, 
						  float fSkillAppliedAttackRange = 0.0f, float fActualAttackDistance = 0.0f);

	static void OnMonsterDead(CFieldIOCPSocket *pAttackSocket, CHARACTER *pAttackCharacter,
					  CFieldIOCPSocket *pTargetSocket, CFieldMonster *pTargetMonster,
					  ITEM *pAttackItem);	// C2M인 경우만 처리함

	static void OnInfluenceBossMonsterCreated(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnInfluenceBossMonsterAutoDestroyed(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnInfluenceBossMonsterDead(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann, CGuildDamageManager *i_pGDamageManager);

	static BOOL IsValidStrategyPointMonster(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnStrategyPointMonsterCreated(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnStrategyPointMonsterAutoDestroyed(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);
	static void OnStrategyPointMonsterDead(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);

	static void OnTeleportMonsterCreated(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);		// 2007-09-20 by cmkwon, 텔레포트 소환 관련 수정
	static void OnTeleportMonsterDead(CFieldMonster *i_pFMon, CFieldMapChannel *i_pFMapChann);			// 2007-09-20 by cmkwon, 텔레포트 소환 관련 수정
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	static void ResetAllBuff(BYTE InflType);
#endif

	// 2006-12-04 by dhjin
	static BOOL IsValidCharacterEnemy(CFieldIOCPSocket *i_pAttFISoc, CFieldIOCPSocket *i_pTarFISoc, BOOL i_bTargetLiveCheck=TRUE);
	static BOOL IsValidCharacterFriend(CFieldIOCPSocket *i_pAttFISoc, CFieldIOCPSocket *i_pTarFISoc, BOOL i_bTargetLiveCheck=TRUE);

	// 2007-02-12 by cmkwon
	static BOOL IsAllSameMap(CFieldMapChannel *pFMChann, vector<CFieldIOCPSocket*> *i_pvectFISockList);

	BOOL OnPrimaryAttack( BYTE i_Atktype );					// 1형 무기 발사시 처리해야 할 것들, check: currentBulletCount 불필요하다고 결정되면 완전해 제거하기, 20031203, kelovon
	
	int OnSecondaryAttack( int shotCount, BYTE i_atktype = NULL );	// 2형 무기 발사시 처리해야 할 것들: shotnum, multinum, reattacktime, HP회복량감소, 탄통 등 처리, currentBulletCount = -1이면 처리 안 함, check: currentBulletCount 불필요하다고 결정되면 완전해 제거하기, 20031203, kelovon
	int RechargeBullet(ITEM_GENERAL *pItemWeapon, BOOL bBulletConsumption = TRUE, BYTE i_byRechargeBulletTy=BULLET_RECHARGE_TYPE_NORMAL);		// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 인자 추가, // 탄창 채움, returns(recharge된 총알의 수, 0이면 recharge 안 됨)
	void SaveCurrentBulletCount(ITEM_GENERAL *pItemWeapon);				// 현재의 총알 수를 DB에 저장함

	BYTE CalcDamageKind(float fDistance, float fDistanceVar = 0.0f);	// 아이템을 공격할 때는 fDistanceVar를 증가시킴

	int SupplyBullet(ITEM_GENERAL *i_pWeaponItem, ITEM *i_pBulletItem, int i_nSupplyCount);		// 탄창 채움, returns(recharge된 총알의 수, 0이면 recharge 안 됨)


	// virtual functions

	// 2008-03-11 by cmkwon, 유저 팅김 현상 수정을 위해 - 
	BOOL PreSendAddData(BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList);
	void OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts) override;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-19 by cmkwon, 대규모 전쟁시 클라이언트 랙(lack) 현상 수정 - 
	void SaveDataOfWriteBufList();
	
	BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL) override;
	BOOL OnRecvdPacketUDP(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	BOOL OnRecvdPacketTCP(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	
	void OnConnect() override;
	void OnClose(int reason) override;
	BOOL OnError(int errCode) override;


	BOOL ResCreateCharacter(CHARACTER *pCharacter, int nErr = 0, CHARACTER_RENDER_INFO *i_pRenderInfo=NULL);
	BOOL ResDeleteCharacter(MSG_FC_CHARACTER_DELETE *pMsgCharacterDelete, int nErr = 0);
	BOOL ResGetCharacterInfoByName(CHARACTER *pCharacter, int nErr = 0);
	BOOL ResGetAllCharacterInfoByID(MSG_FC_CONNECT_LOGIN_OK *pMsgConnectIDOK, SCASH_PREMEIUM_CARD_INFO *i_pCashCardInfo, BYTE i_bySelectableInfluenceMask, int nErr = 0);



	float GetCurrentLoadagePercentage(float i_fWeightPlus = 0.0f);					// 적재율 반환
	float GetTradeItemWeight();														// 거래 아이템의 무게 반환
	void SetSkillUseTime(UID64_t i_skillUID, ATUM_DATE_TIME i_atUseTime);			// 2007-02-07 by dhjin, 스킬 사용 시간 저장.



	BOOL AttachItem(int position, ITEM_GENERAL *pItem, BOOL bCheckWear = FALSE);

	void DetachItem(int position);

	Err_t CheckAttachableItem(INT NewPosition, ITEM_GENERAL *i_pItemGen);
	BOOL CheckItemWindowPosition(INT NewPosition, ITEM* itemInfo);
	void SwapItem(ITEM *pa, ITEM *pb);
	void SetWear(ITEM_GENERAL* pItem);
	void UpdateWearItemPointer(UID64_t MoveItemUniqueNumber, UID64_t DestItemUniqueNumber, int MovePos, int DestPos);

	// 2010-03-11 by cmkwon, ResetAllSkillAndEnchant함수 호출 구조 변경(called OnAttach or OnDetach) - 
	void OnUpdateWearItem(UID64_t MoveItemUniqueNumber, UID64_t DestItemUniqueNumber, int MovePos, int DestPos);

	ITEM *GetAttachedItem(BYTE i_nPosition);
	void OnAttachItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_nTargetItemWindowIndex);
	void OnDetachItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_nSourceItemWindowIndex, BOOL i_bSendChangeInfo=TRUE, BOOL i_bDeleteUsingTimeLimitedItem=TRUE);
	ITEM_GENERAL *GetAttachItemGeneralByPosition(int i_position);

	// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldIOCPSocket::ProcessItemChangeWindowPosition# 인자추가(i_bNoValidCheck)
	ProcessResult ProcessItemChangeWindowPosition(MSG_FC_ITEM_CHANGE_WINDOW_POSITION *i_pItemChangeWindowPos, BOOL i_bDeleteUsingTimeLimitedItem=TRUE, BOOL i_bNoValidCheck=TRUE);

	Err_t ApplyCardItem(ITEM *i_pItemInfo, BOOL i_bUseFlag=FALSE, char *i_szChatMsg=NULL);	// 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 - 인자추가(, char *i_szChatMsg=NULL)
	void ReleaseCardItem(ITEM *i_pItemInfo);

	// 2008-01-31 by cmkwon, 시간 제한 아이템 사용 관련 시스템 수정 - CFieldIOCPSocket::InsertTimeLimiteItem() 수정
	BOOL InsertTimeLimiteItem(ITEM *i_pTimeLimiteItemInfo, BOOL i_bCheckAlreadyUsing8Apply=TRUE);

	// 2013-05-09 by hskim, 세력 포인트 개선
	bool GetStealthState() const { return m_bStealthState || m_bStealthStateBySkill; }

	BOOL ApplyBuffSkill(INT nItemNum);
	BOOL ReleaseBuffSkill(INT nItemNum);
	BOOL ReleaseBuffSkillByDesParam(DestParam_t desSame);

	BOOL ApplyBuffSkillByTurnAround();		// 역전의 버프 적용 (해당자만)
	// 2013-05-09 by hskim, 세력 포인트 개선

	BOOL IsTransferItem(ITEM_GENERAL *i_pItemG, BOOL i_bBazaar=FALSE);
	BOOL IsTransferItem(ITEM *i_pItemInfo, BOOL i_bBazaar=FALSE);

	// 드랍 아이템 관련
	void SendDropItems(CMapBlock *pBlock);				// check: DROPITEM과 DROPMINE을 함께 뿌리자
	void SendDropItemsAroundPosition(D3DXVECTOR3 &pos);

	// 마인 관련
	void ClearAllCharacterMines();						// 캐릭터의 뿌려놓은 모든 마인을 제거한다
	BOOL IsValidDropMine(DROPMINE *i_pDropMine, CMapBlock *&o_pMapBlock);

	// 아이템 총족 사항 확인
	Err_t CheckItemRequirements(ITEM *i_pItemInfo);
	Err_t CheckGeneralItemRequirements(ITEM_GENERAL *i_pGItem);
	Err_t CheckSkillItemRequirements(ITEM *i_pSItemInfo);

	// 접두사 접미사 관련
	static void ApplyRareItem(CFieldMonster *i_pMonster, MONSTER2ITEM *i_pMonster2Item, float i_fPlusRateDropRareItem, CFieldIOCP *i_pFieldIOCP, ITEM_GENERAL *io_pItem);

	// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 인자추가(, RARE_ITEM_INFO *i_pLastRareItemInfo/*=NULL*/)
	BOOL ApplyGambleRareFix(ITEM *i_pGambleItemInfo, ITEM_GENERAL *io_pItem, RARE_ITEM_INFO *i_pLastRareItemInfo=NULL);

	BOOL ApplyGambleRareFix(ITEM_GENERAL *io_pItemG, int i_n100KPrefixProbability, int i_n100KSuffixProbability);

	// 각종 캐릭터 제어 함수
	BOOL StopItemKindShield(ITEM *pItemShield);
	BOOL StopItemKindDecoy(ITEM *pItemDecoy);

	// 2007-12-04 by cmkwon, 예당 선물하기 라이브러리 수정 - CashItemMoneyPay() 인자 추가, UID32_t i_giveTargetCharUID=0
	// 2008-12-18 by cmkwon, 일본 Arario 외부인증, 빌링 모듈 적용하기 - 인자 추가(, int i_nCurTotalMoney=0)
	Err_t CashItemMoneyPay(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pGiftCard, ITEM *i_pItem = NULL, int itemPrice = 0, int i_nTotalPrice = 0, UID32_t i_giveTargetCharUID = 0, int i_nCurTotalMoney = 0);
	constexpr static bool IsChargeUser() { return true; }
	constexpr static bool  IsValidGameUser(Experience_t* o_pExptMaxEXP = nullptr, Experience_t i_CurEXP = 0.0f) { return true; }


	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-25 by cmkwon, 지원 언어/서비스 추가시 꼭 추가 되어야 하는 사항 - [서비스-필수] 빌링 함수 선언

	// 2012-08-17 by jhseol, 마상 BPSoft 빌링시스템
	Err_t CashItem_BPSOFT(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pFreeCash, ITEM *i_pItem=NULL);
	Err_t GetCashPay_BPSOFT(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pFreeCash);
	Err_t BuyCashItem_BPSOFT(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pFreeCash, ITEM *i_pItem);
	void GetWebAccountNum(char* i_szAccountName, UID32_t* o_WebAccountNumber, int* o_nErrNum); // 2012-10-18 by jhseol, BPSoft 빌링 모듈 - 빌링(=WEB)DB 유저 씨리얼 넘버 가져오기
	// end 2012-08-17 by jhseol, 마상 BPSoft 빌링시스템


#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Err_t CashItem_Wikigames(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pGiftCard, ITEM *i_pItem = NULL, int itemPrice = 0, int i_nTotalPrice = 0, UID32_t i_giveTargetCharUID = 0);
#else
	Err_t CashItem_Wikigames(INT *o_pTotalMoney, INT *o_pMCash, INT *o_pGiftCard, ITEM *i_pItem = NULL, int i_nTotalPrice = 0, UID32_t i_giveTargetCharUID = 0);
#endif // SHOP_PRICES_PER_BUILDING_NPC

	
	int CashItem_Wikigames_DirectConnect(SWIKIBILL_REQ_DATA *i_pReqData, SWIKIBILL_RET_DATA *o_pRetData);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-06 by cmkwon, 베트남 게임 가드 X-TRAP으로 변경 - 
	static BOOL				ms_XTrapUsingFlag;


	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-01 by cmkwon, 멤버쉬 구입 가능 최대 일수 - 나라별로 다르다
	static INT GetMaxBuyableDayOfMembership(void);

	// 2006-02-09 by cmkwon, 킬마크 관련
	ITEM *GetGiveKillMarkItemPtr2Killer(void);
	void ResetGiveKillMarkItemPtr2Killer(void);

	// quest
protected:

public:
	Err_t CheckQuestStartRequirements(CQuest *pQuestInfo, MSG_FC_QUEST_REQUEST_START *i_pQuestStart, BOOL i_bProcessPartyMembers/*=TRUE*/);		// 퀘스트 시작 필요 조건 확인
	CFieldCharacterQuest* GetCharacterQuestInProgress(void);			// 진행중인 퀘스트 반환.
	Err_t CheckQuestCompetionRequirements(CFieldCharacterQuest *i_pFCharQuest, MSG_FC_QUEST_REQUEST_SUCCESS *i_pQuestSuccess
											, MapIndex_t i_nMapIndex, INT i_nWarpTargetIndex, INT i_nAttMonster=0, BOOL i_bByUser=TRUE);	// 퀘스트 성공 여부(완료 조건) 확인
	ProcessResult CheckQuestRequestSuccessAndSendResult(CFieldCharacterQuest *i_pFCharQuest, MSG_FC_QUEST_REQUEST_SUCCESS *i_pQuestSuccess
											, BOOL i_bSendErrorMessage=TRUE, MapIndex_t i_nMapIndex=0, INT i_nWarpTargetIndex=0
											, INT i_nAttMonster=0, BOOL i_bOnlyCompletionCheck=FALSE);
	void CheckSuccessAllQuestByWarpTargetIndex(MapIndex_t i_nMapIndex, INT i_nWarpTargetIndex);
	void CheckSuccessAllQuestByAttackMonsterNum(INT i_nAttackMonsterNum);
	
	// 2007-12-06 by cmkwon, 시간제한 퀘스트 서버에서 시간 체크하게 수정 - CheckSuccessProgressQuestByTimeLimited() 추가
	void CheckSuccessProgressQuestByTimeLimited(void);
	
	BOOL ProcessQuestResult(int *o_pnExpOfCompensation, CQuest *pQuestInfo, int i_nQuestResultParam=0
							, MSG_FC_QUEST_REQUEST_SUCCESS_RESULT * o_pQuestResult = NULL
							, QUEST_PAY_ITEM_INFO * o_QuestPayItemInfo = NULL);
	BOOL ProcessQuestStartAction(MapIndex_t *o_pWarpMapIndex, BOOL *o_pbPartyWarp, CQuest *pQuestInfo);
	BOOL InsertQuestPayItem(QUEST_PAY_ITEM *i_pQuestPayItem);
	BOOL IsExistQuestCityWar(void);
	BOOL CityWarQuestResult(INT i_questIdx, BOOL i_bSuccess);
	BOOL IsCheckQuestComplited(INT i_questIdx);
	BOOL GMQuestCompletion(int i_questIdx=-1);
	BOOL GMQuestCompletion(CQuest *i_pQuestInfo);
	BOOL GMQuestAdjustByInfluenceType(BYTE i_byInflTy);
	CFieldCharacterQuest * GetFieldCharacterQuestPtrByQuestIndex(INT i_questIdx);

	void OnGetAllQuest(mtmapQuestIndex2FieldCharacterQuest *i_pmtmapCharacterQuest);

	// 2009-01-13 by cmkwon, 미션 보상을 파티원 모두에게 주기 명령어 추가 - 인자 추가(, BOOL i_bAttacker=TRUE)
	BOOL AddCharacterQuestMonsterCount(int i_monUniqueNumber, BOOL i_bTopAttacker, BOOL i_bAttacker=TRUE);
	BOOL IsQuestStateInProgressByQuestIndex(int i_quesIndex);
	BOOL CheckQuestCompetionRequirementsByQuestIndex(int i_quesIndex, MSG_FC_QUEST_REQUEST_SUCCESS *i_pRequestSuccess);
	Err_t CheckQuestStartByQuestIndex(int *o_pnErrParam1, MSG_FC_QUEST_REQUEST_START *i_pQuestRequestStart, BOOL i_bProcessPartyMembers=TRUE);
	
	// 거래
	CFieldIOCPSocket* SendTradeCanceledAndReset(CFieldIOCPSocket *pPeerSocket, BOOL bSendToPeer);
	Err_t TradeCheckEnableInsertItems(CFieldIOCPSocket *i_pPeerFISock);
	void TradeInsertItems(CFieldIOCPSocket *i_pPeerFISock);
	// 2010-06-15 by shcho&hslee 펫시스템 - 거래 시 펫 정보 처리
	void TradePetData(UID64_t i_tradeUniqeNum, CFieldIOCPSocket *i_pPeerFISock);
	// end 2010-06-15 by shcho&hslee 펫시스템 - 거래 시 펫 정보 처리

	// PK
	CFieldIOCPSocket* SendP2PPKEndAndReset(CFieldIOCPSocket *pPeerSocket, BOOL bSendToPeer);
	static void OnP2PPKEnd(CFieldIOCPSocket *pWinnerSocket, CFieldIOCPSocket *pLoserSocket);	// 일대일 결투 결과 처리, 변수까지 모두 Reset함

	// 2010-03-10 by cmkwon, 인피에서 시간제한 액세서리 추가 관련 버그 수정 - 
	BOOL InitCharacterInventoryItem(BOOL i_bStartFromArena=FALSE);
	BOOL ResetAllSkillAndEnchant();	// 모든 skill & enchant & rare fix 적용
//	BOOL InsertItemListByCharacterFirstGameStartFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, vectItemSkillPtr *i_pvectItemSkillPtrList);	// 2007-12-21 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	
	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 정보관련 벡터 받아오는 인자 추가
	BOOL CFieldIOCPSocket::InsertItemListByCharacterFirstGameStartFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, vectItemSkillPtr *i_pvectItemSkillPtrList , vecPetCurrentInfo *i_vectPetCurInfo);	// 2007-12-21 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리

// 2008-01-31 by cmkwon, 사용하지 않는 함수 주석 처리
// 	BOOL InsertItemListFromDB(MSG_FC_STORE_GET_ITEM* i_pStoreGetItem, vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, vectItemSkillPtr *i_pvectItemSkillPtrList=NULL);
// 	BOOL InsertCharacterItemListFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList);	// 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	void SendCharacterItemList();			// 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리

	// 2008-01-10 by cmkwon, 개인 창고 버그 수정 - CFieldIOCPSocket::InsertStoreItemListFromDB() 에 인자 추가
	BOOL InsertStoreItemListFromDB(vectITEM_GENERALPtr *i_pvectItemGPtrList, vectENCHANT *i_pvectEnchantList, UID32_t i_possessCharacterUID, vecPetCurrentInfo *i_vectPetCurInfo);		// 2012-01-14 by hskim, 파트너 시스템 2차 - 창고에서 꺼낼 때 클라 튕기는 문제 수정 - // 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	
	void SendStoreItemList();				// 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	BOOL InsertSkillListFromDB(vectItemSkillPtr *i_pvectItemSkillPtrList);			// 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	void SendSkillItemList();				// 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	void SendQuestList();					// 2007-12-14 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리
	void SendPetItemList();					// 2010-06-15 by shcho&hslee 펫시스템 - 처음 게임 시작시 클라이언트 패킷 전송 함수 추가 
	void SendInsertPetItemInfo(BYTE ItemInsertionType, tPET_CURRENTINFO* pcurrentPetInfo);			// 2010-06-15 by shcho&hslee 펫시스템 - 펫 아이템 추가시 클라이언트 패킷 전송 함수 추가 
	

	// enchant
	float GetEnchantValue(ITEM *pTargetItem, BYTE desParam); 

	BOOL ApplyEnchant(ITEM_GENERAL *pTargetItemG);			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	BOOL ApplyDestParam8Enchant8RareOfAttachedItems(void);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	BOOL ItemOptionInitialization(ITEM_GENERAL *pItemGeneral);	// 2010-11-12 by khkim 메모리핵:14(무게) 로그 오류 수정

	BOOL InsertEnchantToItem(ITEM_GENERAL *pTargetItem, ENCHANT &enchant);
	BOOL InsertEnchantToItem(ITEM_GENERAL *pTargetItem, vectENCHANT *i_pvectEnchant, BOOL bSendToClient=TRUE);
	BOOL DeleteAllEnchantToItem(UID64_t i_itemUID);
	void PrintEnchant(UID64_t ItemUniqueNumber = 0, BOOL i_bWriteSystemLog = FALSE);	// ItemUniqueNumber 가 0이면 모두 출력
	BOOL PopAllEnchantByItemUID(vectENCHANT *o_pvectEnchat, UID64_t i_itemUID);

	vectENCHANT *GetVectEnchantListPtr(UID64_t i_itemUID);

	// skill
	BOOL SetParamFactor(DestParam_t desParam, float paramValue, ClientIndex_t i_SkillUseClientIdx=0);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL UnsetParamFactor(DestParam_t desParam, float paramValue);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL SetParamFactor(CParamFactor *i_pParamFactor, DestParam_t desParam, float paramValue, ClientIndex_t i_SkillUseClientIdx=0);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	BOOL UnsetParamFactor(CParamFactor *i_pParamFactor, DestParam_t desParam, float paramValue);		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
	
	const CParamFactor* GetParamFactor() const { return &m_ParamFactor; }

	bool CheckSKILLInvincible() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_Invincible; }
	bool CheckSKILLBarrier() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_Barrier; }
	bool CheckSKILLChainRolling() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_ROLLING_TIME; }

	bool IsEnableRepairDP();
	float CalcSKILLBigBoom() const { return GetParamFactor()->pf_SKILL_Big_Boom_damage; }

	bool IsDamageDistributionToPartyMember() const { return GetParamFactor()->pfb_BitFlag.pfb_SKILL_DamageDistribution; }

	// 2007-12-13 by cmkwon, 무제한 악세사리 다중기능 구현 - Current 값을 변경 해주는 기능은 제거됨
	BOOL SetParamFactor_POS_ATTACHMENT(BYTE desParam, float paramValue);	
	BOOL UnsetParamFactor_POS_ATTACHMENT(BYTE desParam, float paramValue);
	USHORT GetParamValue_POS_ATTACHMENT(BYTE desParam);

	// Rare Item의 Suffix & Prefix 적용, 장전된 아이템(무기)에 대해서만 해당됨
	BOOL ApplyRareFix(ITEM_GENERAL *pItemGeneral);

	// 파티
	void PartyGetMemberClientIdxListExcludeMe(vectorClientIndex *o_pVectClientIndex, ClientIndex_t i_excludeCltIdx=0, BOOL i_bOnlySameMapChannel=FALSE, BOOL i_bOnlyNoBodyConEventHandle=FALSE);
	UID32_t GetPartyMasterUniqueNumber();
	BOOL IsPartyMaster(void);
	BOOL IsSamePartyByClientIndex(ClientIndex_t clientIndex);
	BOOL IsSamePartyByCharacterUniqueNumber(UID32_t CharacterUniqueNumber);
	BOOL IsSameParty(CFieldIOCPSocket *pSocket);
	void OnPartyBattleMemberDead(BOOL bImediateLoss = FALSE);	// 파티전에서 전사 혹은 강제 종료시, 즉시 패배 처리를 할 지 여부 전달
	BOOL IsPartyBattleState(void);								// 파티전을 하고 있는가?
	static void OnPartyBattleEnd(CFieldParty *pWinnerParty, CFieldParty *pLoserParty);

	// 스피드핵 체크
	void CheckSpeedHack(DWORD i_dwClientTickGap, float i_fServerDistaceGap);
	BOOL WriteLogMessageSpeedHack(BYTE i_bCheckType);

	// 2005-12-21 by cmkwon, 메모리핵 체크
	void CheckMemoryHackWeapon(ITEM_GENERAL *i_pItemGen, MSG_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK *i_pRealInfo);
	void CheckMemoryHackEngine(ITEM_GENERAL *i_pItemGen, MSG_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK *i_pRealInfo);
	void CheckMemoryHackTotalWeight(ITEM_GENERAL *i_pItemGen, MSG_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK *i_pRealInfo);
	BOOL CheckMemoryHackByMemoryHackCheckTy(ITEM_GENERAL *i_pItemGen, int i_nMemoryHackCheckTy, float i_fValidValue, float i_fCurrentValue, int i_nParam1=0);

	// TargetMonsterIndex 관련
	void InsertAttackTargetIndexVector(ClientIndex_t i_Index);
	void DeleteAttackTargetIndexVector(ClientIndex_t i_Index);
	void DeleteMeFromMonsterAttackedList(void);

	void IncrementInsertingStoreCounts(void);
	void DecrementInsertingStoreCounts(void);


	// 2005-12-28 by cmkwon, 세력전쟁 관련 함수
	void InfluenceWarCheckInvasion(void);
#ifdef NEMERIAN_PVP_AWARD_EFFORT
	void InfluenceWarBonus2Killer(CFieldIOCPSocket *i_pFISockDeaded, unsigned int killMarkQty = 0);
#else
	void InfluenceWarBonus2Killer(CFieldIOCPSocket *i_pFISockDeaded);
#endif
	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-05 by cmkwon
	BOOL					m_Security_bCheckVersion;						// 2006-06-05 by cmkwon
// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 사용하지 않음
//	unsigned char			m_Security_pbyGuidReqInfo[SIZEOF_GUIDREQINFO];	// 2006-06-05 by cmkwon
//
//	// 2008-03-24 by cmkwon, 핵쉴드 2.0 적용 - 
//	//unsigned long *			m_Security_plCRCInfo;							// 2006-06-05 by cmkwon
//	HSHIELD_CLIENT_CONTEXT	m_Security_ClientContext;		// 2008-03-24 by cmkwon, 핵쉴드 2.0 적용 - 추가됨

	BOOL					m_Security_bSendCRCReqMsg;						// 2006-06-08 by cmkwon
// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 사용하지 않음
//	unsigned char			m_Security_pbyReqInfo[SIZEOF_REQINFO];			// 2006-06-05 by cmkwon
//	unsigned long			m_Security_ulOption;
//	Err_t SecurityCheckVersion(MSG_FC_MOVE_HACKSHIELD_GuidAckMsg *i_pGuidAckMsg=NULL);
//	Err_t SecurityCheckClient_1(MSG_FC_MOVE_HACKSHIELD_CRCAckMsg *i_pCRCAckMsg=NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-04 by cmkwon, 태국 게임가드 Apex로 변경 - 
	static BOOL				ms_bUsingSecuritySystem;
	BOOL					m_bInitSecurityClient;
	BOOL GetInitSecurityClient(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 

	static AHNHS_SERVER_HANDLE	ms_hAhnHSServerHandle;
	static BOOL SecurityServerInit(void);
	static void SecurityServerClean(void);

	AHNHS_CLIENT_HANDLE			m_hAhnHSClientHandle;
	Err_t SecurityClientInit(void);
	void SecurityClientClean(void);
	// 2009-03-09 by cmkwon, 일본 Arario nProtect에 CS인증 적용하기 - 인자추가(, BOOL i_bCheckTerm=TRUE)
	Err_t SecurityClientCheck(MSG_FC_MOVE_HACKSHIELD_CRCAckMsg *i_pCRCAckMsg=NULL, BOOL i_bCheckTerm=TRUE);

	DWORD					m_dwSetTickSecurity_bSendCRCReqMsg;				// 2006-10-21 by cmkwon, 테스트

	DWORD					m_dwReadyTickforSecurityCheck;					// 2009-03-09 by cmkwon, 일본 Arario nProtect에 CS인증 적용하기 -
	static int				ms_nPeriodMinutesforSecurityCheck;				// 2009-03-09 by cmkwon, 일본 Arario nProtect에 CS인증 적용하기 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon, 
	mtvectActionInfo			m_mtvectActionInfoAggroList;
	void AddAggroList(ActionInfo *i_pActionInfoAggro);
	BOOL GetAggroList(vectActionInfo *o_pvectActionInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-26 by cmkwon, 개인 상점
	CBazaarManager					m_bazaarManager;				// 2006-07-26 by cmkwon
	BOOL MakeMSG_FC_BAZAAR_INFO_OKW(MSG_FC_BAZAAR_INFO_OK *o_pBazaarInfoOK);
	BOOL SendBazaarItemList(CFieldIOCPSocket *i_pFISock, BYTE i_byBazaarTy);
	void SendEnchatItemNumList(CFieldIOCPSocket *i_pFISock, vectUID64_t *i_pItemUIDList);
	BOOL BazaarInsertItem(CFieldIOCPSocket *i_pFromFISock, ITEM_GENERAL *i_pInsertItemGen, int i_nItemCnts, BYTE i_nItemUpdateType=IUT_BAZAAR_SELL);

	BOOL CheckCharacterUIDFormAccount(UID32_t i_CharactUID);		// 2006-09-15 by dhjin, 계정에 케릭터가 존재하는지 체크
	BOOL DeleteCharacterUIDFormAccount(UID32_t i_CharactUID);		// 2006-09-15 by dhjin, 계정에 케릭터 삭제.
	FC_CONNECT_LOGIN_INFO * GetFC_CONNECT_LOGIN_INFONoLock(UID32_t i_CharactUID);		// 2006-10-18 by cmkwon

	// START 2011-11-03 by shcho, yedang 셧다운제 구현 - 체크 함수 추가
	BOOL CheckCharacterUIDFormSHOTDOWNMINS(UID32_t i_CharactUID);
	// END 2011-11-03 by shcho, yedang 셧다운제 구현 - 체크 함수 추가

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
	BYTE GetAddedPermanentInventoryCountOfOthersCharacter(UID32_t i_CharactUID);
	BOOL SetAddedPermanentInventoryCountOfOthersCharacter(UID32_t i_CharactUID, int i_nRacingPoint);


	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-28 by cmkwon
	CHARACTER *GetCharacterCacheNoLock(UID32_t i_charUID);
	BOOL UpdateCharacterCacheLock(UID32_t i_charUID, CHARACTER *i_pCharac);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-09 by cmkwon
	BOOL InsertEventItem(int i_itemNum, int i_nCnts, BYTE i_nItemUpdateType, int i_nPrefixCodeNum = 0, int i_nSuffixCodeNum = 0, BYTE i_enumStorage=ITEM_IN_CHARACTER);	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안

	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-16 by cmkwon
	BOOL						m_bQuestRequestWarp;					// 2006-10-16 by cmkwon
	DWORD						m_dwLastQuestRequestWarpTick;			// 2006-10-16 by cmkwon
	MAP_CHANNEL_INDEX			m_beforeMapChannIdxQuestRequestWarp;	// 2006-10-16 by cmkwon, 파티 워프 이전의 MapChannelIndex

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-08 by cmkwon
	BOOL						m_bIsInvisibleCharacter;				// 2006-11-08 by cmkwon
	BOOL IsInvisibleCharacter(void);

	//////////////////////////////////////////////////////////////////////////
	// 2006-11-24 by dhjin
	BOOL						m_bNoDamage;

	//////////////////////////////////////////////////////////////////////////
	// 2012-06-14 by jhseol, 아레나 추가개발part2 - 무적 : 아레나 리스폰 후 무적 시간
	ATUM_DATE_TIME				m_atimeArenaNoDamage;
	BOOL						m_bArenaNoDamage;
	// end 2012-06-14 by jhseol, 아레나 추가개발part2 - 무적 : 아레나 리스폰 후 무적 시간

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-15 by cmkwon
	ATUM_DATE_TIME		m_atmeGameStartTime;						// 2006-11-15 by cmkwon, 게임 시작 시간
	int					m_nGameContinueTimeInSecondOfToday;			// 2006-11-15 by cmkwon, 오늘 게임 지속 시간
	int					m_nGameContinueTimeInSecondOfToday4Viet;	// 2007-10-23 by cmkwon, 베트남 게임 지속 시간 시스템  수정 - 선택창으로 갔다가 게임 시작시 처음 접속시 지속시간으로 설정하기 위한 변수
	ATUM_DATE_TIME		m_atimeLastGameEndDate;						// 2006-11-15 by cmkwon, 마지막 게임 종료 날짜
	DWORD				m_dwSendPlayTimeInfoLastTick;				// 2007-06-28 by cmkwon, 중국 방심취관련(게임 시간 알림 구현) - 
	int	GetCurGameContinueTimeInSecondOfToday(void);
	void GetPlayTimeInfoString(char *o_szPlayTimeString);
	//BOOL GPGetApplyRateByGamePlayingTime(float *o_pfRetRate);		// 2007-08-13 by cmkwon, 인자명 수정(<--i_pfRetRate), // 2007-06-14 by cmkwon, 게임 지속 시간 제한 기본 시스템으로 수정
	BOOL GPInitGameStartTime(void);									// 2007-06-19 by cmkwon, 게임 지속 시간 수정 - 중국 5시간 후 접속 처리

	// 2007-06-27 by cmkwon, 중국 방심취 시스템 수정 - 미성년자 관련
	BOOL				m_bIsAdult;			
	BOOL IsAdultsUser(void);
	BOOL SetAdults(ATUM_DATE_TIME i_atBirthday);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-12-04 by dhjin, 체프 관련 
	mtvectFieldDummy		m_mtvectFieldDummy;
	UINT					m_uiDummyItemFieldIndex;
	BOOL					m_bDummyCheck;
	FIELD_DUMMY* FindFieldDummyNoLock(UINT i_ItemFieldIndex);
	BOOL DeleteFieldDummy(UINT i_ItemFieldIndex);
	void DeleteAllFieldDummy(void);
	BOOL ApplyFieldDummyDamage(float i_fChangeValue, BYTE i_enumDamageType);
	BOOL Make_MSG_FC_BATTLE_DROP_DUMMY_OK(BYTE *o_pSendBuf, int *o_pnSendByte, int i_nBufSize);

	//////////////////////////////////////////////////////////////////////////
	// 2007-03-28 by dhjin, 옵저버 대상이 유요한 유저인지 체크.
	BOOL ObserverTargetCharacterCheck(CFieldIOCPSocket *i_pOTFISock);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-19 by dhjin, 관전자 대상이 유효한 유저인지 체크.
	BOOL WatchTargetCharacterCheck(CFieldIOCPSocket *i_pOTFISock);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-03-29 by cmkwon
	MSG_FC_MOVE_BIT_FLAG Get_MSG_FC_MOVE_BIT_FLAG(void);

	mtvectObersverCharacter m_mtvectObersverCharacter;		// 2007-03-27 by dhjin, 자신을 보고 있는 모든 옵저버 유저 정보.
	BOOL AddObserver(CFieldIOCPSocket *i_pObserverFISoc);
	BOOL PopObserver(CFieldIOCPSocket *i_pObserverFISoc);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-19 by dhjin, 관전자 관련 추가.
	mtvectWatchCharacter m_mtvectWatchCharacter;
	BOOL AddWatchUser(CFieldIOCPSocket *i_pObserverFISoc);
	BOOL PopWatchUser(CFieldIOCPSocket *i_pObserverFISoc);

	const char* GetCharacterLevelRank() const;							// 2007-04-24 by dhjin, 케릭터 레벨 랭크 가져오는 함수
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-04 by dhjin, 아레나 전용 아이템 지급에 관련된 함수 추가.
	BOOL InsertArenaItem(int i_itemNum, int i_nCnts, BYTE i_nItemUpdateType = IUT_ARENA_ITEM);	// 2007-06-04 by dhjin, 아레나 전용 아이템 지급

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-08 by cmkwon, 2형무기 공격확률,공격력,피어스율 적용 시스템 수정
	mtvectSATTACK_PARAMETER		m_mtvectSAttackParameterList;		// 2007-06-08 by cmkwon, 2형 T_FC_BATTLE_ATTACK시의 정보
	BOOL APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam);
	SATTACK_PARAMETER *APFindAttackParameterNoLock(UID16_t i_WeaponIndex);
	BOOL APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex);
	void APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex, enumAttackToTarget eAttackToTarget, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선

	// 2007-06-13 by cmkwon, 예당 7월 PC방 프로모션(콩이벤트 구현) - 추가됨
	BOOL WarriorPlaytimeEventCheck(int nCurrTick); //warriorsw 2015-05-14
	BOOL WarriorsUpdatePlayTime(int nAccID, int Value);//warriorsw 2015-05-14
	BOOL WarriorPlaytimeEvent(int nItemNum, int nItemCount, int nCurrTick); //warriorsw 2015-05-14
	void OnDoMinutelyWorkByTimerManager(void);

	// 2007-06-14 by cmkwon, 세력전 보상 수정 - 
	// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - CFieldIOCPSocket::WPAddWarPoint()에 인자(, BYTE i_byWPUpdateTy=WPUT_GENERAL) 추가
	void WPAddWarPoint(INT i_nAddWarPoint, BOOL i_bEventBonus, BOOL i_bSend2Client = TRUE, BYTE i_byWPUpdateTy = WPUT_GENERAL, BOOL i_bCalcWarPoint = TRUE);
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-15 by dhjin, 관전
	void WatchReady();		// 2007-06-15 by dhjin, 관전 시작 전에 준비 사항.
	void WatchEnd();		// 2007-06-19 by dhjin, 관전 종료 명령어 입력.

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial
	mtvectTutorialList			m_mtvectTutorialList;						// 2007-07-06 by dhjin, Tutorial 완료 정보
	void InitTutorial(vectTutorial * i_pvectSTutorialCompleteInfo);		// 2007-07-06 by dhjin, Tutorial 완료 정보를 셋팅한다.
	INT	MakeMSG_FC_TUTORIAL_START_OK(STUTORIAL_LIST_INFO * o_pTutorialListInfo);		// 2007-07-06 by dhjin, Tutorial 메세지 만들기
	ITEM* GetTutorialWeaponInfo(BYTE i_byAttackType);			// 2007-07-16 by dhjin, 각 기어 튜토리얼 무기 셋팅
	STUTORIAL_LIST_INFO *GetTutorialDataNoLock(INT i_nTutorialNum);	// 2007-07-23 by cmkwon

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-27 by dhjin, 판공비 관련
	static void SetExpediencyFundW(BYTE i_byInfluence, INT i_nSPICount);	// 2007-08-27 by dhjin, 판공비 업데이트 함수

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - CFieldIOCPSocket 에 멤버변수 멤버함수 추가
	BOOL						m_bUnlockSecondaryPassword;								// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - CFieldIOCPSocket 에 멤버 변수 추가, 잠금 상태
	char						m_szSecondaryPassword[SIZE_MAX_PASSWORD_MD5_STRING];	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - CFieldIOCPSocket 에 멤버 변수 추가, 2차패스워드
	void SetSecondaryPassword(char *i_szSecPass);
	bool IsSettingSecondaryPassword() const { return *m_szSecondaryPassword != '\0'; }
	BOOL CompareSecondaryPassword(char *i_szSecPass);
	BOOL CheckSecondaryPasswordLock(void);

	//////////////////////////////////////////////////////////////////////////
	// 2007-09-14 by dhjin, WarPoint 계산 
	INT	 CalcWarPoint(INT i_nWarPoint, BOOL i_bEventBonus);				// 2007-09-14 by dhjin, WarPoint 이벤트 적용 계산

	// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - 게임서버 로그인/로그아웃 처리, PreServer 인증 플래그 변수 추가
	bool GetPreServerAuthenticated() const { return m_bPreServerAuthenticated; }
	void SetPreServerAuthenticated(bool i_bFlag, bool i_bUpdateDB = false) { m_bPreServerAuthenticated = i_bFlag; }

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	SGIVE_TARGET_CHARACTER		m_giveTargetCharacter;
	void OnCheckGiveTarget(SGIVE_TARGET_CHARACTER *i_pGiveTargetCharacter);			// 2007-11-13 by cmkwon, 선물하기 기능 추가 - CFieldIOCPSocket::OnCheckGiveTarget() 추가
	Err_t GiveItem(ITEM *i_pItem, int i_nCnts, BYTE i_nItemUpdateType);	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-28 by cmkwon, 통지시스템 구현 -
	mtvectSNOTIFY_MSG			m_mtvectNotifyMsgList;					// 2007-11-28 by cmkwon, 통지시스템 구현 -
	void OnLoadNotifyMsg(mtvectSNOTIFY_MSG *i_pmtvectNotifyMsgList);	// 2007-11-28 by cmkwon, 통지시스템 구현 -
	BOOL CheckDeleteNotifyMsg(SNOTIFY_MSG *o_pNotifyMsg, UID64_t i_notifyMsgUID);				// 2007-11-28 by cmkwon, 통지시스템 구현 -
	void InsertNotifyMsg(SNOTIFY_MSG *o_pNotifyMsg);					// 2007-12-20 by cmkwon, 통지시스템 버그 수정 - 자신의 통지리스트에 통지 추가
	static BOOL InsertNotifyMsg2Anyone(SNOTIFY_MSG *i_pNotifyMsg, CFieldIOCPSocket *i_pDoerFISoc);		// 2007-12-20 by cmkwon, 통지시스템 버그 수정 - CFieldIOCPSocket::InsertNotifyMsg() 함수명 변경, // 2007-11-28 by cmkwon, 통지시스템 구현 -

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
	mtvectQPARAM_CHECK_EVENTITEM	m_mtvectCouponEventList;
	BOOL FindCouponEvent(int i_nItemEventUID);
	BOOL PopCouponEvent(QPARAM_CHECK_EVENTITEM *o_pCouponEventInfo, int i_nItemEventUID);
	BOOL AddCouponEvent(QPARAM_CHECK_EVENTITEM *i_pCouponEventInfo);
	void SendCouponEvent();		// 2008-04-28 by dhjin, 아레나 통합 - 쿠폰 정보 전송 

	// 2008-01-03 by dhjin, 아레나 통합 - MFS와 AFS 케릭터 정보 매칭 저장
	SAFS_NEED_MFSINFO	m_AfsNeedMfsInfo;
	BOOL SendMFSArenaStateChange(BYTE i_byArenaState);			// 2008-01-03 by dhjin, 아레나 통합 - 필드서버에 아레나 상태값 변경 
	void JobBeforeArenaPlay();			// 2008-01-04 by dhjin, 아레나 통합 - 아레나 대전을 위해 필드서버에서 해야되는 일 
	BOOL SetDataArenaGameStart();		// 2008-01-09 by dhjin, 아레나 통합 - 아레나 시작 전 CFieldIOCPSocket에 설정 해야되는 값 처리
	BOOL SetDataReadyArenaGameStart();		// 2008-01-31 by dhjin, 아레나 통합 - 아레나 시작 전 CFieldIOCPSocket에 설정 해야되는 값 처리, 클라이언트 시작 도와줌
	void SendArenaCharacterPay(BYTE i_byIssue, INT i_nPayWarPoint);		// 2008-01-10 by dhjin, 아레나 통합 - 필드 서버로 보상 전송 
	void SendArenaCharacterDisConnect();						// 2008-01-15 by dhjin, 아레나 통합 - 필드 서버로 강제 종료 전송
	void OnCharacterReadyGameStartFromArenaToMainServer();				// 2008-01-31 by dhjin, 아레나 통합 - 아레나 종료 후 필드 서버 시작 처리, 클라이언트 시작 도와주기
	void OnCharacterGameStartFromArenaToMainServer();				// 2008-01-10 by dhjin, 아레나 통합 - 아레나 종료 후 필드 서버 시작 처리
	void ResetWearItem();										// 2008-01-29 by dhjin, 아레나 통합 - 장착 아이템 정보 초기화
	void ResetAllSkill();								// 2008-02-25 by dhjin, 아레나 통합 - 모든 스킬 초기화
	void ResetTimeLimitSkill();							// 2008-02-25 by dhjin, 아레나 통합 - 시간제한형 스킬 초기화
	void ResetToggleSkill();							// 2008-02-25 by dhjin, 아레나 통합 - 토글 스킬형 초기화
	void ResetChargingSkill();							// 2008-02-25 by dhjin, 아레나 통합 - 차징 스킬형 초기화
	void ResetFieldDummySkill();						// 2008-02-25 by dhjin, 아레나 통합 - 더미 스킬 초기화
	void InitUserInfoForArenaEnding();					// 2008-03-11 by dhjin, 아레나 통합 - 아레나 종료시 아레나 서버에 유저 정보 초기화 필요한 부분 처리
	void SendUserInfoForArenaEnding();					// 2011-10-17 by hskim, 파트너 시스템 2차 - 아레나 종료시 아레나 서버에서 메인 서버로 데이터 전송 처리
	//////////////////////////////////////////////////////////////////////////
	// 2008-04-03 by dhjin, 모선전, 거점전 정보창 기획안 - 
	void SendMSWarInfoResult(vectSMSWAR_INFO_RESULT * i_pVectMSWarInfoResult);	// 2008-04-03 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 정보 전송
	INT MakeMSG_FC_INFO_MSWARINFO_RESULT_OK(SMSWAR_INFO_RESULT *o_pMSWarInfoResult, vectSMSWAR_INFO_RESULT * i_pVectMSWarInfoResult);
	void SendSPWarInfoResult(vectSSPWAR_INFO_RESULT * i_pVectSPWarInfoResult);	// 2008-04-03 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 정보 전송
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	void SetDiffVictory(int DiffVictory);
#endif

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-23 by dhjin, EP3 편지 시스템 - 
	CLetterManager		m_LetterManager;
	void GetDBAllLetter();				// 2008-04-23 by dhjin, EP3 편지 시스템 - DB에서 편지 가져오기
	
	void SendUserInfoBySecretOption(SUSER_INFO_OPTION * i_pUserInfoOption);		// 2008-06-23 by dhjin, EP3 유저정보옵션 - 유저 정보 전송

	///////////////////////////////////////////////////////////////////////////////
	// 2008-06-20 by cmkwon, EP3 백맵 시스템 수정(맵당 백맵을 2개 설정 할 수 있다) - 
	mtmapMapIdx2MapIdx		m_mtmapBackMapIndex;		
	MapIndex_t GetBackMapIndex(CFieldMapProject *i_pCurFMPro);
	void UpdateBackMapIndex(MapIndex_t i_nCurMapIndex, MapIndex_t i_nBackMapIndex);

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-18 by dhjin, 1초 텀 시스템 기획안
	DWORD	m_dwMSTermSystem;
	BOOL CheckOneSecondTermSystem();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-10-21 by cmkwon, 마지막에 선택된 레어는 다시 나오지 않게 수정 - 
	// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	RARE_ITEM_INFO		*m_pLastSelectedPrefixRareItemInfo;	// 갬블시 마지막으로 선택된 레어 정보 
	RARE_ITEM_INFO		*m_pLastSelectedSuffixRareItemInfo;	// 갬블시 마지막으로 선택된 레어 정보 

	//////////////////////////////////////////////////////////////////////////
	// 2008-11-10 by dhjin, 럭키머신
	// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 기존 소스 수정 - 
	Err_t LuckyItemWin(INT i_buildingIdx, ITEM * i_pItem, INT i_nMachineNum, BOOL * o_pbDeleteCoinItem);		// 2009-03-03 by dhjin, 럭키머신 수정안 - i_nMachineNum추가 

	// 2009-05-27 by cmkwon, 럭키머신/해피머신 인벤 요구 개수 다르게 처리 - 함수 헤더파일에 추가
	int GetNeededInventoryCount(ITEM * i_pItem, INT i_nMachineNum, UINT i_nBuildingIdx);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-11-28 by cmkwon, 대만 Netpower_Tpe XignCode(게임가드) 적용 - 
	static BOOL				ms_bUsingXignCode;
	static BOOL IsInitializedSecurityServer(void);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-12-08 by dhjin, 미션마스터
	BOOL CheckQuestComplete(INT i_nQuestIdx);
	BOOL m_bMissionMaster;
	void MissionMasterQuestResult(CQuest *pQuestInfo);		// 2008-12-09 by dhjin, 미션마스터 - 미션마스터 보상 지급

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, 파티원 Move 처리 시스템 수정 - 
	mtvectCFieldIOCPSocket		m_mtvectCFISockPartyMemberList;		// 2008-12-30 by cmkwon, 자신을 포함한 모든 파티원 리스트
	static BOOL ArrangeAllPartyMember(vectCFieldIOCPSocket *i_pvectCFISockList);
	BOOL ArrangePartyMemberList(vectCFieldIOCPSocket *i_pvectCFISockList);
	BOOL GetPartyMemberList(vectCFieldIOCPSocket *o_pvectCFISockList);
	void CleanPartyMemberList(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-09 by cmkwon, 일본 Arario nProtect에 CS인증 적용하기 - CFieldIOCPSocket에 멤버 변수 추가
	static BOOL				ms_bUsingNProtect;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	int						m_nPartyFormationSkillNum;
	void SetPartyFormationSkillNum(int i_nSkillNum);
	int GetPartyFormationSkillNum(void);
	ITEM_SKILL * FindPartyFormationSkill(BYTE i_byFormationTy);
	void ChangePartyFormationSkill(vectCFieldIOCPSocket *i_pvectPartyMemList, BOOL i_bOnOff, BYTE i_byFormationTy);
	BOOL ApplyPartyFormationSkill(ITEM_SKILL *i_pSkill);
	BOOL ReleasePartyFormationSkill(int i_nTemp=0);
	void ApplyItemDesParam(ITEM *i_pItem);
	void ReleaseItemDesParam(ITEM *i_pItem);
	void DamageDistributionToPartyMember(float i_fChangeValue, BYTE i_enumDamageType, BOOL i_bCheckRecovery=TRUE
		, CFieldMonster *i_pAttackMonster=NULL, CFieldIOCPSocket *i_pAttackCharacter=NULL, MSG_FC_BATTLE_SHOW_DAMAGE *i_pShowDamage=NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	BOOL IsChangeItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);
	BOOL IsChangeShapeItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);
	BOOL IsChangeEffectItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);
	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-01 by cmkwon, 그래픽 리소스 변경 관련 초기화 기능 구현 - 아래의 2개 함수 추가
	BOOL IsInitShapeItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);	// 2009-10-01 by cmkwon, 그래픽 리소스 변경 관련 초기화 기능 구현 - 
	BOOL IsInitEffectItemNumMix(ITEM_GENERAL **o_ppMainItemG, ITEM_GENERAL **o_ppSubItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);	// 2009-10-01 by cmkwon, 그래픽 리소스 변경 관련 초기화 기능 구현 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-02-11 by cmkwon, 조합시 외형,이펙트 유지 시스템 구현 - 
	BOOL IsKeepShape8EffectMix(ITEM_GENERAL **o_ppMainItemG, ITEM_UNIQUE_NUMBER_W_COUNT *i_arrSourceItem, INT i_nSourceItemCnt, INT i_nMixCnts, ITEM *i_pTargetItemInfo);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	mt_vectorInfinityImpute		m_mtVecInfiImpute;						// 2009-09-09 ~ 2010 by dhjin, 인피니티 -  인피니티 귀속 정보
	INFINITY_PLAYING_INFO		m_InfinityPlayingInfo;					// 2009-09-09 ~ 2010 by dhjin, 인피니티 -  인피니티 중일때 필요한 정보 저장	
	void GetDBInfinityImpute();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 가져오기
//	void SetInfinityImpute(InfiModeUID_t i_ninfiModeUID, ImputeUID_t i_nImputeUID);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속
	BOOL SetDataReadyInfinityGameStart();		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 전 CFieldIOCPSocket에 설정 해야되는 값 처리, 클라이언트 시작 도와줌
	BOOL SetDataInfinityGameStart();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 전 CFieldIOCPSocket에 설정 해야되는 값 처리
	BOOL SetInfinityMap(MapIndex_t i_MapIdx, ChannelIndex_t i_ChannelIdx);						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 전 맵 설정
	void InfinityFin();					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료로 Main서버로 전송할 부분 전송
	void ArenaFin();					// 2011-10-17 by hskim, 파트너 시스템 2차 - 아레나 종료로 Main서버로 전송할 부분 전송
	INT	m_InfinityUpdateAllItemCount;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료로 Main서버에서 업데이트 해야될 아이템 수
	ClientIndex_t	m_AFSClientIndex;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - ArenaSvr ClientIdx 저장
	BOOL InfinityResetImpute(InfiModeUID_t i_ninfiModeUID);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 귀속 정보 리셋
	void InfinityFinReloadAllItem();					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 업데이트 종료로 모든 아이템 다시 불러오기!!
	Lv_t GetLvCheckRevision();							// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 보정 상황인지 체크
	static float CalcDamageOfAttackMonsterToMonster(float i_fDamageWeight,
											CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, SATTACK_PARAMETER *i_pAttParam,	void *i_pTargetUnit);// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터간 전투
	static void OnMonsterDeadByMonster(CFieldMonster *pTargetMonster, ITEM *pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터에 의한 사망

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - 
	ClientIndex_t				m_nLastMonsterIndexAttack2Me;			// 나를 공격하는 마지막 몬스터의 MonsterIndex
	ClientIndex_t				m_nTargetMonsterIndex;					// 내가 공격하는 MonsterIndex
	void SetLastMonsterIndexAttack2Me(ClientIndex_t i_MonIdx);			// 
	ClientIndex_t GetLastMonsterIndexAttack2Me(void);					// 
	void SetTargetMonsterIndex(ClientIndex_t i_MonIdx);					
	ClientIndex_t GetTargetMonsterIndex(void);					
	void GetAttackedMonsterIndex(setClientIndex_t *o_pMonsterIdxList);
 	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	BOOL MonsterAttackSecM2C(CFieldMonster * i_pAttackMonster, MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSecMsgInfo, ITEM * i_pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 일반 공격 처리 M->C
	BOOL MonsterAttackSecM2M(CFieldMonster * i_pAttackMonster, MSG_FN_BATTLE_ATTACK_SECONDARY * i_pAttackSecMsgInfo, ITEM * i_pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 일반 공격 처리 M->M

// 2010-03-16 by cmkwon, 인피2차 MtoM, MtoC 타겟 변경 관련 수정 - CFieldIOCPSocket::ProcessSplashDamageMonsterAllAttack#로 통합
//	static void ProcessRangeDamageMonsterAllAttack(MSG_FN_BATTLE_ATTACK_PRIMARY *i_pAttackPri, CFieldIOCPSocket *i_pAttackMonSock, CFieldMonster *i_pAttackMon, ITEM *i_pAttackItem);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 범위 공격

	BOOL GetDelegateClientIdxByMonsterAttackSecM2M(CFieldMonster * i_pAttackMonster, ClientIndex_t * o_pDelegateClientIdx);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 간 2형 공격 판단 해줄 클라이언트 위임자 찾기

	// start 2011-04-18 by hskim, 인피니티 3차 - 주포 공격 방식 변경
	BOOL GetDelegateClientIdx(CFieldMonster * i_pAttackMonster, ClientIndex_t * o_pDelegateClientIdx);
	// end 2011-04-18 by hskim, 인피니티 3차 - 주포 공격 방식 변경

	BOOL IsApplyItemIgnoreDefence();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 방어력 무시 아이템 사용 체크	
	BOOL IsApplyItemIgnoreAvoid();				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 회피력 무시 아이템 사용 체크
	ParamValue_t GetParamResistance();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 저항 아이템 사용 체크
	ParamValue_t GetParamOnceResistance();		// 2009-09-09 ~ 2010-01-19 by dhjin, 인피니티 - 한 번만 저항하고 없어지는 저항 아이템 추가
	ParamValue_t GetParamAddAttack();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 타격치 아이템 사용 체크
	ParamValue_t GetParamAddAttackSec();		// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 절대값 타격치 아이템 고급무기용(기무와 분류)
	ParamValue_t GetParamReduceDamage();		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 데미지 절대값 감소 아이템 사용 체크
	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 절대값 타격치 아이템 고급무기용(기무와 분류), 밑과 같이 수정
//	static void AddAttackDamage(CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, void *i_pTargetUnit, SATTACK_PARAMETER *i_pAttParam);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 추가 타격치 아이템 적용
	static void AddAttackDamage(CFieldIOCPSocket *i_pAttackSocket, void* i_pAttackUnit, void *i_pTargetUnit, ParamValue_t i_AddAttackDamage, SATTACK_PARAMETER *i_pAttParam);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 추가 타격치 아이템 적용
	BOOL GetInfinityShopInfo(InfinityShopUID_t i_InfinityShopUID, INFINITY_SHOP_INFO * o_pInfinityShopItem);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	Err_t CheckBuyShopItemInsertNoLock(ItemNum_t i_ItemNum, UID64_t i_Amount);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점, 상점에서 아이템 구매 시 구매 될 아이템 체크 사항
	Err_t CheckBuyShopItemDeleteNoLock(ItemNum_t i_ItemNum, UID64_t i_Amount);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점, 상점에서 아이템 구매 시 지불 될 아이템 체크 사항
	Err_t CheckInfinityShopItem(INFINITY_SHOP_INFO * i_pInfinityShopItem, INT i_Amount);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점, 아이템 체크 사항
	ItemNum_t GetCurrentAttackItemNum() const { return m_CurrentAttackItemNum; }		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
	ItemNum_t m_CurrentAttackItemNum;					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
	void InfinityFinCharacterRoutine();			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 인피니티에서 유저 종료 시 처리 되어야 되는 것들
	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	ITEM *CheckItemParamOverlap(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum=0);
	ITEM *CheckItemParamOverlapOfAttachedItems(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum);

	BOOL SendMFSInfinityStateChange(eINFINITY_STATE i_byInfintiyState);	// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다.
	Err_t SendInfinityModeInfo();		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, 모드 관련 정보 전송(입장 가능 여부 체크하여 전송)

	///////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	Err_t ApplyInvokingItem(ITEM_GENERAL * i_pItemG);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 적용
	Err_t EnchantApplyInvokingItem(ITEM_GENERAL * i_pTargetItemG, ITEM * i_pEnchantItem);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 인첸트 적용
	void SendApplyDestParamInfo(ClientIndex_t i_ApplyClientIdx, INVOKING_ITEM_DESTPARAM_TYPE * i_pApplyDestParamInfo);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 관련 데스파람 적용 정보 전송
	void SendApplyDestParamListInfo(CFieldIOCPSocket * i_pApplyFISoc, InvokingItemDestParamTypeList * i_pInvokingItemDestParamTypeList); // 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 발동 관련 데스파람 적용 정보 전송
	Err_t ApplyInvokingItemByUse(ITEM_GENERAL *i_pItemG);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 사용
	void GameEndAllInvokingEquipItem();				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 게임 종료시 처리.
	void ApplyInvokingItemDrain(float i_fRetDamage, CFieldIOCPSocket *i_pAttackFISoc, CFieldIOCPSocket *i_pTargetFISoc=NULL, CFieldMonster * i_pTargetMonster=NULL);					// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 드레인 처리
	void ApplyInvokingItemReflection(float i_fRetDamage, CFieldIOCPSocket *i_pTargetFISoc, CFieldIOCPSocket *i_pAttackFISoc=NULL, CFieldMonster * i_pAttackMonster=NULL);				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 데미지 반사 처리
	void IncreaseCurrentHPDP(float i_fChangeVal);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, HP -> DP 회복
	BOOL CheckDestParamEnchantItemNoLock(ITEM_GENERAL * i_pEnchantItemG, ITEM_GENERAL * i_pTargetItemG);	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 인첸트시 데스파람이 모두 만족하는지 체크

	CInvokingItemManager			m_InvokingItemManager;		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	EntranceCount_t InfinityGetEntranceCount(InfiModeUID_t i_infiModeUID);
	void Check8SendAFSUpdateItem(ITEM_GENERAL * i_pItemG);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	void UserApplyByMonsterSkill(MSG_FN_BATTLE_ATTACK_SKILL * i_pRMsg, CFieldMonster * i_pFMonster, ITEM * i_pSkillItemDefault);				// 2010-03-31 by dhjin, 인피니티(기지방어) - 몬스터가 사용한 스킬 처리 (유저만)
	void ProcessingHitInvokeSkill(ITEM * i_pAttackItem, ClientIndex_t i_AttackIdx);			// 명중시 발동되는 스킬 처리
	void ApplyHitInvokeSkill(ItemNum_t i_SkillNum, ClientIndex_t i_AttackIdx);					// 명중시 발동되는 스킬 적용

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피2차 M2M 2형 무기 보완 처리 - 
	void ProcessM2MOverLifeTimeAttackParameter(CFieldMonster *i_pAttFMon, SATTACK_PARAMETER *i_pAttParam, CFieldMapChannel *i_pCurFMapChann);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	void InfinityInitImputeList(vectINFINITY_IMPUTE *i_pImputeList);
	void InfinityAddImpute(InfiModeUID_t i_infiModeUID, BOOL i_bUpdate2DB=FALSE);
	INFINITY_IMPUTE *InfinityGetImputeInfoNoLock(InfiModeUID_t i_infiModeUID);
	BOOL InfinitySendImputeList2AFS(void);
	BOOL InfinitySendAddDataFromAFS2MFS(BYTE * pData, int nSize);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	void InfinityOnCharacterGameEndRoutine(void);
	void ArenaOnCharacterGameEndRoutine(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	Err_t TryEnchantItem(BOOL *o_pbSuccessFlag, ITEM_GENERAL *i_pTargetItemG, int i_nEnchantCnt, ITEM_GENERAL *i_pEnchatItemG, ITEM_GENERAL *i_pPreventDestructionItemG, ITEM_GENERAL *i_pProbabilityIncreaseItemG);
	Err_t TryGambleItem(BOOL *o_pbSuccessFlag, ITEM_GENERAL *i_pTargetItemG, int i_nEnchantCnt, ITEM_GENERAL *i_pGambleCard1ItemG, ITEM_GENERAL *i_pGambleCard2ItemG);

	// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
	BOOL GetAllEnchantListByItemUID(vectENCHANT *o_pvectEnchat, UID64_t i_itemUID);
	BOOL IsHyperCard(ENCHANT_INFO *i_pEnchantInfo);			
	BOOL ExsistHyperCardIn10Lv(UID64_t i_nItemUID);			
	// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

	///////////////////////////////////////////////////////////////////////////////
	// 2010-05-12 by cmkwon, 거래 이용한 아이템 메모리 버그 수정(K0002953) - 거래중인 아이템 여부 리턴
	BOOL CheckTradingItem(UID64_t i_itemUID);
	///////////////////////////////////////////////////////////////////////////////
	// 2010-06-01 by shcho, PC방 권한 획득(캐쉬) 아이템 - 
	void SetHomepremium(BOOL i_bUse);	
	///////////////////////////////////////////////////////////////////////////////
	// 2010-08-27 by shcho&&jskim, WARPOINT 증가 아이템 구현
	BOOL SetWarPoint(INT i_nAddWarPoint);

	////////////////////////////////////////////
	// 2011-09-20 by hskim, 파트너 시스템 2차
	Err_t PetCancelSocketItem(ITEM *i_pItemInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어
	BOOL TestMonsterDropItems(int MonsterIndex, int TestCount);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-06-01 by shcho, GLogDB 관련 -
	Experience_t		m_GameStartEXP;						// 2010-06-01 by shcho, GLogDB 관련 -
	ATUM_DATE_TIME		m_StartStayedMapTime;				// 2010-06-01 by shcho, GLogDB 관련 -
	vectStayedMapInfo	m_vectStayedMapInfo;				// 2010-06-01 by shcho, GLogDB 관련 -
	
	MOSTSTAYED_MAP_INFO* FindStayedMapInfo(MapIndex_t i_MapIndex);
	const char * GetStayedMapName();			

	// 2010-08-05 by dhjin, 버닝맵 - 
	Err_t	CheckPossibleBurningMapWarp(MapIndex_t i_nMapIdx);		// 버닝맵 워프가 가능한지 조건 체크
	BOOL	IsGetOutBurningMap(MapIndex_t i_nMapIdx);				// 버닝맵에서 나가버렷~!!

	//eCONNECT_PUBLISHER	m_eOtherPublisherConncect;				// 2010-11 by dhjin, 아라리오 채널링 로그인.

	// start 2012-01-16 by hskim, 통계 - 화패
	SSTATISTICS_MONEY_UNIT m_StatisticsInfinityMoney;
	SSTATISTICS_MONEY_UNIT GetCurrentMoney();

	void StatisticsInfinityStart();								// 인피니티 들어간후 통계 선작업 - 현재 ORG 및 SPI 저장
	SSTATISTICS_MONEY_UNIT StatisticsInfinityEnd();				// 인피니티 끝날때   통계 후작업 - 저장했던 ORG 및 SPI 비교후 차이점 리턴
	// end 2012-01-16 by hskim, 통계 - 화패

	void ArenaPlayInfoUpdataToMF();	// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 아레나 플레이 정보 업데이트 내용을 메인 필드서버에게 전달하기

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	// 2012-10-19 by jhjang 해피아워 경험치 리뉴얼 - 킬카운트 300일때 바로 지급하게 수정

#if S_BONUS_KILL_SYSTEM_RENEWAL
	bool RefreshKillCount();
#else
	bool RefreshKillCount(CFieldMonster* pFieldMonster);
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

	// end 2012-10-19 by jhjang 해피아워 경험치 리뉴얼 - 킬카운트 300일때 바로 지급하게 수정
	void SetKillCountEventItem(bool bIsPayKillCountBonusItem);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2013-03-13 by hskim, 웹 캐시 상점
	//void ResetCashAuthenticationKey()					{ memset(m_szAuthenticationKey, 0, sizeof(m_szAuthenticationKey)); }
	//void GenCashAuthenticationKey();
	//void SetCashAuthenticationKey(char *pAuthKey)		{ util::strncpy(m_szAuthenticationKey, pAuthKey, sizeof(m_szAuthenticationKey)); }
	//BOOL IsCashAuthenticationKey(char *pAuthKey)		{ return 0 == strncmp(m_szAuthenticationKey,pAuthKey, sizeof(m_szAuthenticationKey)); }
	//char *GetCashAuthenticationKey()					{ return m_szAuthenticationKey; }
	// end 2013-03-13 by hskim, 웹 캐시 상점

	// 2015-09-23 Future, added logging of MAC Address
	const char* GetMACAddress() { return m_szMACAddress; }

	// 2015-10-04 Future, added decking detection
#ifdef S_ANTI_DECK
	void SetLastHit(UID32_t attackerUID);
	void ResetLastHit();
#endif // S_ANTI_DECK

	// 2015-11-24 Future, Multiple IP Restriction
#ifdef S_IP_UNIQUE_CONNECTION
	void ResGetMultipleIpStatus(BOOL status, UID32_t kickIssuingCharacterID = 0);
#endif // S_IP_UNIQUE_CONNECTION

	// 2015-12-17 Future, Nation Change Card
	void ResSwitchNation(BOOL res, UID64_t itemNum);

	// 2016-01-03 Future, leader warp restriction
#ifdef SC_LEADER_WARP_RESTRICTION
	BOOL RestrictWarp(UINT forMinutes, BOOL townUser = TRUE);
	BOOL ReleaseWarpRestriction();
	BOOL IsWarpRestricted();
	void SendWarpRestrictionError();
#endif // SC_LEADER_WARP_RESTRICTION

	void PreventCityFlying();

public:
	void Send2MeCharacterOtherInfo(vectorClientIndex *i_pVectCltIndexList);
	void SendCharacterOtherInfo(int idx, BOOL i_bSendInRange = FALSE);
	void SendCharacterInfo(MessageType_t msgType);
	Err_t SendInfinityDifficulty_Info(void);

	// 자신의 주위에 메시지 전송
	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CFieldIOCPSocket::SendInRangeMessageAroundMe#, 인자추가(i_nStartCityMapIdx)
	void SendInRangeMessageAroundMe(BYTE *pData, int Size, BOOL bIncludeMe, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX);
	void SendInRangeMessageExactAroundMe(BYTE *pData, int Size, int i_radius, BOOL bIncludeMe, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK);

	template<MessageType_t msgtype>
	void SendInRangeMessageAroundMe(MessageData<msgtype>& msg, bool bIncludeMe, BYTE i_byInfluenceMask=INFLUENCE_TYPE_ALL_MASK, MapIndex_t i_nStartCityMapIdx=VCN_CITY_MAP_INDEX)
	{
		this->SendInRangeMessageExactAroundMe(reinterpret_cast<BYTE*>(&msg), msg.size(), bIncludeMe, i_byInfluenceMask, i_nStartCityMapIdx);
	}

	// 다른 이에게 자신의 정보를 보냄
	void SendParamFactorToOthers(BYTE i_nItemPosition, ClientIndex_t i_ToClientIndex, BOOL i_bSendInRange);

	void SendString128(BYTE string128_type, const char* pFormat, ...);
	void SendString128Static(BYTE string128_type, char* pFormat, ...);
	//Send Notification to User
	void SendNotification(BOOL bSpecialChat/*Write it into Leadechat or not?*/, char* pFormat, ...);
private:
	/*
	ProcessResult Process_FM_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_GET_NUM_CLIENTS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_PING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_PRINTMAP(const char* pPacket, int nLength, int &nBytesUsed);
	*/
	ProcessResult Process_FC_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CONNECT_NETWORK_CHECK(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-02-15 by cmkwon, Client<->FieldServer 간 네트워크 상태 체크 - 
	ProcessResult Process_FC_CONNECT_ARENASERVER_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-12-28 by dhjin,  아레나 통합 -
	ProcessResult Process_FC_CONNECT_ARENASERVER_TO_IMSERVER(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-03-03 by dhjin,  아레나 통합 -
	ProcessResult Process_FC_CHARACTER_GET_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_RENDER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_APPLY_COLLISION_DAMAGE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SHOW_EFFECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_OTHER_PARAMFACTOR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SPEED_HACK_USER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_CASH_MONEY_COUNT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_TUTORIAL_SKIP(const char* pPacket, int nLength, int &nBytesUsed);
	// 2005-07-26 by hblee : 착륙장에서 캐릭터 모드 변환.
	ProcessResult Process_FC_CHARACTER_CHANGE_CHARACTER_MODE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_FALLING_BY_FUEL_ALLIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_WARP_BY_AGEAR_LANDING_FUEL_ALLIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_MEMORY_HACK_USER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_UPDATE_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed);				// 2007-02-14 by dhjin
	// 2007-03-27 by dhjin, 옵저버 모드
	ProcessResult Process_FC_CHARACTER_OBSERVER_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_OBSERVER_END(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_SHOW_MAP_EFFECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-04-20 by cmkwon

	// 2008-01-10 by dhjin,  아레나 통합 - 
	ProcessResult Process_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER(const char* pPacket, int nLength, int &nBytesUsed);	
	ProcessResult Process_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER(const char* pPacket, int nLength, int &nBytesUsed);	

	// 2008-06-23 by dhjin, EP3 유저정보옵션 -
	ProcessResult Process_FC_CHARACTER_GET_USER_INFO(const char* pPacket, int nLength, int &nBytesUsed);	
	ProcessResult Process_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET(const char* pPacket, int nLength, int &nBytesUsed);	
	ProcessResult Process_FC_CHARACTER_CHANGE_NICKNAME(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

	ProcessResult Process_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX(const char* pPacket, int nLength, int &nBytesUsed);	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 

	ProcessResult Process_FC_CHARACTER_DEBUFF_DOT_INFO(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
	
	ProcessResult Process_FC_CHARACTER_GET_USER_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-06-05 by jhseol, 아템미리보기 - 살펴볼 케릭터 UID
	ProcessResult Process_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-07-12 by jhseol, 아템미리보기 On / Off 기능 - 공개 여부
	
// 2004-12-16 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//	ProcessResult Process_FC_EVENT_WARP_CONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_WARP_SAME_MAP_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_WARP_SAME_FIELD_SERVER_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_ENTER_BUILDING_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_LEAVE_BUILDING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_OBJECT_EVENT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_SELECT_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_SELECT_CHANNEL_WITH_PARTY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_RACING_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_REQUEST_SHOP_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_CHARACTERMODE_ENTER_BUILDING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_CALL_WARP_EVENT_REQUEST_ACK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_CLICK_TELEPORT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_EVENT_NOTIFY_MSG_GET(const char* pPacket, int nLength, int &nBytesUsed);		// 2007-11-28 by cmkwon, 통지시스템 구현 -
	ProcessResult Process_FC_EVENT_NOTIFY_MSG_DELETE(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-11-28 by cmkwon, 통지시스템 구현 -
	ProcessResult Process_FC_EVENT_COUPON_EVENT_USE_COUPON(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 

	ProcessResult Process_FC_CHARACTER_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CONNECT_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_DEAD_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_GAMEEND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_REPAIR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_DOCKING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_UNDOCKING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_CHANGE_BODYCONDITION_ALL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHARACTER_USE_BONUSSTAT(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_LOCKON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_UNLOCKON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_LANDING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_LANDING_DONE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_TAKEOFF(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_TARGET(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_WEAPON_VEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_ROLLING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_HACKSHIELD_GuidAckMsg(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_HACKSHIELD_CRCAckMsg(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MOVE_HACKSHIELD_HACKING_CLIENT(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FC_BATTLE_ATTACK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ATTACK_FIND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_DROP_MINE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_MINE_ATTACK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_MINE_ATTACK_FIND(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_BATTLE_CHANGE_TARGET(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_DROP_DUMMY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_DROP_FIXER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_TOGGLE_SHIELD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_TOGGLE_DECOY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ATTACK_EVASION(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_BATTLE_DROP_BUNDLE(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_BATTLE_BUNDLE_ATTACK(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_BATTLE_BUNDLE_ATTACK_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-12-27 by cmkwon, 사용하지 않음
//	ProcessResult Process_FC_BATTLE_REQUEST_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_REQUEST_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ACCEPT_REQUEST_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_REJECT_REQUEST_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_SURRENDER_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BATTLE_REJECT_SURRENDER_P2P_PK(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_STORE_GET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_STORE_MOVE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_STORE_EXPIRE_USINGITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_STORE_REQUEST_QUICKSLOT(const char* pPacket, int nLength, int &nBytesUsed);	// 2006-09-04 by dhjin
	ProcessResult Process_FC_STORE_SAVE_QUICKSLOT(const char* pPacket, int nLength, int &nBytesUsed);		// 2006-09-04 by dhjin
	ProcessResult Process_FC_STORE_LOG_GUILD_ITEM(const char* pPacket, int nLength, int &nBytesUsed);			// 2006-09-27 by dhjin, 여단 로그 요청 받음

	ProcessResult Process_FC_ITEM_GET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_DELETE_ITEM_ADMIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_DELETE_DROP_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_CHANGE_WINDOW_POSITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_UPDATE_WINDOW_ITEM_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_THROW_AWAY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_ENERGY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_ENCHANT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_MIX_ITEMS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_GUILDSUMMON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_RESTORE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_GUILD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_MONSTERSUMMON(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_INFLUENCE_BUFF(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_INFLUENCE_GAMEEVENT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_RANDOMBOX(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_SKILL_SUPPORT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_USE_LUCKY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-11-04 by dhjin, 럭키머신
// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 기존 소스 수정 - 
//	ProcessResult Process_FC_ITEM_USE_LUCKY_ITEM_WIN(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-11-04 by dhjin, 럭키머신
	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리
	ProcessResult Process_FC_ITEM_USE_PET_SOCKET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-09-20 by hskim, 파트너 시스템 2차
	ProcessResult Process_FC_ITEM_CANCEL_PET_SOCKET_ITEM(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-09-20 by hskim, 파트너 시스템 2차
	ProcessResult Process_FC_DISSOLVED_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	// END 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리

	// 2015-12-17 Future, Nation Change Card
	ProcessResult Process_FC_ITEM_USE_CARDITEM_CHANGE_NATION_REQUEST(const char* pPacket, int nLength, int& nBytesUsed);

	ProcessResult Process_FC_INFO_GET_MONSTER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_MAPOBJECT_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_SIMPLE_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_RARE_ITEM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_BUILDINGNPC_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_ENCHANT_COST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_CURRENT_MAP_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_SERVER_DATE_TIME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - C->F
	ProcessResult Process_FC_INFO_CHECK_RESOBJ_CHECKSUM(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_REQUEST_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_REQUEST_ACCEPT_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_REQUEST_REJECT_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_CITY_GET_BUILDING_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_REQUEST_ENTER_BUILDING(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-11-10 by cmkwon, Process_FC_EVENT_REQUEST_SHOP_WARP, Process_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST로 대체함
//	ProcessResult Process_FC_CITY_GET_WARP_TARGET_MAP_LIST(const char* pPacket, int nLength, int &nBytesUsed);
//	ProcessResult Process_FC_CITY_REQUEST_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_CHECK_WARP_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REG_LEADER_CANDIDATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_DELETE_LEADER_CANDIDATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_VOTE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_POLL_REQUEST_POLL_DATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_WARINFO_INFLUENCE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITY_WARINFO_OUTPOST(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_QUEST_REQUEST_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_ACCEPT_QUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_CANCEL_QUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_REQUEST_SUCCESS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_DISCARD_QUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_MOVE_QUEST_MAP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_REQUEST_SUCCESS_CHECK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_REQUEST_PARTY_WARP_ACK(const char* pPacket, int nLength, int &nBytesUsed);		// 2006-10-16 by cmkwon

	ProcessResult Process_FC_SHOP_GET_ITEMINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_SELL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_GET_USED_ITEM_PRICE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_GET_SHOP_ITEM_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_REQUEST_REPAIR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_CASH_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_COLOR_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_BUY_WARPOINT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_CHECK_GIVE_TARGET(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-11-13 by cmkwon, 선물하기 기능 추가 -
	ProcessResult Process_FC_SHOP_INFLUENCE_BUFF(const char* pPacket, int nLength, int &nBytesUsed);		// 2013-05-09 by hskim, 세력 포인트 개선

	// Party
	ProcessResult Process_FC_PARTY_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_GET_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_GET_ALL_MEMBER(const char* pPacket, int nLength, int &nBytesUsed);	// check: 필요할 때 구현하기, 20031005
	ProcessResult Process_FC_PARTY_REQUEST_PARTY_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_PARTY_GET_OTHER_MOVE(const char* pPacket, int nLength, int &nBytesUsed);

	// Guild
	ProcessResult Process_FC_GUILD_GET_MAP_OWNER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_REQUEST_GUILD_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_GET_WAR_INFO(const char* pPacket, int nLength, int &nBytesUsed);
// 2004-12-10 by cmkwon, IM Server에서 처리한다, 프로토콜 삭제함
//	ProcessResult Process_FC_GUILD_SURRENDER_GUILD_WAR(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_SUMMON_MEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_GUILD_DISMEMBER(const char* pPacket, int nLength, int &nBytesUsed);

	// Trade
	ProcessResult Process_FC_TRADE_REQUEST_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_CANCEL_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_ACCEPT_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_REJECT_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_TRANS_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_SEE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_OK_TRADE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TRADE_CANCEL_TRADE(const char* pPacket, int nLength, int &nBytesUsed);

	// Countdown
	ProcessResult Process_FC_COUNTDOWN_DONE(const char* pPacket, int nLength, int &nBytesUsed);

	// Object
	ProcessResult Process_FC_OBJECT_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);

	// Auction
	ProcessResult Process_FC_AUCTION_REGISTER_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_AUCTION_CANCEL_REGISTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_AUCTION_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_AUCTION_GET_ITEM_LIST(const char* pPacket, int nLength, int &nBytesUsed);

	// Skill
	ProcessResult Process_FC_SKILL_USE_SKILLPOINT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_SETUP_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_USE_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CANCEL_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_PREPARE_USE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CANCEL_PREPARE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CONFIRM_USE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SKILL_CONFIRM_USE_ACK(const char* pPacket, int nLength, int &nBytesUsed);

	// Timer
	ProcessResult Process_FC_TIMER_TIMEOUT(const char* pPacket, int nLength, int &nBytesUsed);

	// MONSTER
	ProcessResult Process_FC_MONSTER_SKILL_END_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MONSTER_SUMMON_MONSTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_MONSTER_TUTORIAL_MONSTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	
	// T0_FC_RACING
	ProcessResult Process_FC_RACING_RACINGLIST_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_RACING_RACINGINFO_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_RACING_JOIN_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_RACING_CHECKPOINT_CHECK(const char* pPacket, int nLength, int &nBytesUsed);

	// T0_FC_CITYWAR
	ProcessResult Process_FC_CITYWAR_GET_OCCUPYINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_SET_SETTINGTIME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_SET_TEX(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_SET_BRIEFING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CITYWAR_BRING_SUMOFTEX(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FC_WAR_REQ_SIGN_BOARD_STRING_LIST(const char* pPacket, int nLength, int &nBytesUsed);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-26 by cmkwon, 개인 상점
	ProcessResult Process_FC_BAZAAR_SELL_PUT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_CANCEL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_REQUEST_ITEMLIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_SELL_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_PUT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_CANCEL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_REQUEST_ITEMLIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_BAZAAR_BUY_SELL_ITEM(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-04-17 by dhjin, ARENA 관련
	ProcessResult Process_FC_ARENA_PLAY_CHARACTER_TYPE(const char* pPacket, int nLength, int &nBytesUsed);		// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
	ProcessResult Process_FC_ARENA_REQUEST_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_CREATE_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_ENTER_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_REAVE_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_TEAM_READY_FINISH(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_TEAM_READY_FINISH_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_ENTER_ROOM_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_FINISH_WARP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_REQUEST_CREATE_TEAMINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_REQUEST_OTHER_TEAM_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ARENA_QUICK_ENTER_TEAM(const char* pPacket, int nLength, int &nBytesUsed);
	// 2008-01-08 by dhjin, 아레나 통합 - 	
	ProcessResult Process_FC_ARENA_CHARACTER_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed);


	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial
	ProcessResult Process_FC_TUTORIAL_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TUTORIAL_COMPLETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TUTORIAL_END(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_TUTORIAL_WARP(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-20 by dhjin, OutPost
//	ProcessResult Process_FC_OUTPOST_RESET_CHECK_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_OUTPOST_RESET_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_OUTPOST_NEXTWAR_INFO_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_OUTPOST_NEXTWAR_SET_TIME(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-22 by dhjin, Notice
	ProcessResult Process_FC_INFO_NOTICE_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_NOTICE_REG(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_NOTICE_MODIFY(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-22 by dhjin, 판공비
	ProcessResult Process_FC_INFO_EXPEDIENCYFUND_REQUEST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_EXPEDIENCYFUND_PAYBACK(const char* pPacket, int nLength, int &nBytesUsed);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 추가된 프로토콜 처리 함수 추가
	ProcessResult Process_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_SECONDARYPASSWORD_LOCK(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FC_INFO_GET_GUILDMARK(const char* pPacket, int nLength, int &nBytesUsed);

	// NPC
	ProcessResult Process_FN_MAPPROJECT_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_GET_CHARACTER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_CHANGE_OK(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
	ProcessResult Process_FN_MISSILE_MOVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_HPRECOVERY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_HIDE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SHOW(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_ATTACK_PRIMARY(const char* pPacket, int nLength, int &nBytesUsed
		, MSG_FN_BATTLE_ATTACK_PRIMARY *pObjectMonsterAttackPrimary=NULL
		, CFieldMapProject *i_pFMapProject=NULL);
	ProcessResult Process_FN_BATTLE_ATTACK_SECONDARY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_ATTACK_FIND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_DROP_FIXER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_BATTLE_ATTACK_HIDE_ITEM_W_KIND(const char* pPacket, int nLength, int &nBytesUsed);
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	ProcessResult Process_FN_BATTLE_ATTACK_SKILL(const char* pPacket, int nLength, int &nBytesUsed);

	ProcessResult Process_FN_MONSTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SKILL_USE_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_SKILL_END_SKILL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_MONSTER_AUTO_DESTROYED(const char* pPacket, int nLength, int &nBytesUsed);
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	ProcessResult Process_FN_MONSTER_HPTALK(const char* pPacket, int nLength, int &nBytesUsed);
	// 2010-03-31 by dhjin, 인피니티(기지방어) -
	ProcessResult Process_FN_BATTLE_ATTACK_SKILL_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	
	// Monitor	
	ProcessResult Process_FM_PRINTMAP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SAVE_MAPBLOCK_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_GET_MAP_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SET_SERVICE_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_RELOAD_SERVER_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_INCREASE_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_SET_CHANNEL_STATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FM_CITYWAR_CHANGE_WAR_TIME(const char* pPacket, int nLength, int &nBytesUsed);

	// Admin
	ProcessResult Process_FC_ADMIN_GET_SERVER_STAT(const char* pPacket, int nLength, int &nBytesUsed);

	// Admin용 함수
	BOOL CheckCommandExecutability(USHORT i_nAllowedRace, BOOL i_bAllowAdminTool = FALSE);
	BOOL HandleAdminCommands(const char *cmd);

	// Debug용 함수들
	ProcessResult Process_FM_PRINT_DEBUG_MSG(const char* pPacket, int nLength, int &nBytesUsed);

	// Client Report
	ProcessResult Process_FC_CLIENT_REPORT(const char* pPacket, int nLength, int &nBytesUsed);

	// 2007-12-27 by dhjin, 아레나 통합 -	
	ProcessResult Process_FtoA_MFSINFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_AUTH_CHARACTER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_TEAM_MATCHING_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTER_RESET_RELOAD(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 -
	ProcessResult Process_FC_INFO_MSWARINFO_DISPLAY_OPTION(const char* pPacket, int nLength, int &nBytesUsed);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
	ProcessResult Process_FC_INFO_MSWARINFO_RESULT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_SPWARINFO_RESULT(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by dhjin, EP3 편지 시스템 -
	ProcessResult Process_FC_CHAT_LETTER_REQUEST_TITLE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_LETTER_READ(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_LETTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_LETTER_SEND(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_REQUEST_TITLE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_READ(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_CHAT_ALLLETTER_SEND(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2008-12-08 by dhjin, 미션마스터
	ProcessResult Process_FC_QUEST_REQUEST_MISSIONMASTER_HELP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_MISSIONMASTER_HELP_INVITE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_QUEST_MISSIONMASTER_HELP_REJECT(const char* pPacket, int nLength, int &nBytesUsed);

	//////////////////////////////////////////////////////////////////////////
	// 2009-01-12 by dhjin, 선전 포고 -	
	ProcessResult Process_FC_INFO_DECLARATION_MSWAR_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_DECLARATION_MSWAR_SET(const char* pPacket, int nLength, int &nBytesUsed);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	ProcessResult Process_FC_INFO_WRK_GET_SERVICE_INFO(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_WRK_GET_RANKER_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFO_WRK_GET_SELF_RANKING(const char* pPacket, int nLength, int &nBytesUsed);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	ProcessResult Process_FC_INFINITY_MODE_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_READY_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_CREATE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_JOIN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_MEMBER_INFO_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_CHANGE_MASTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_LEAVE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_BAN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_INFINITY_IMPUTE_LIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_READY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_READY_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_INFINITY_START_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_MAP_LOADED(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_FIN_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_INFINITY_READY_FINISH_MAINSVR_START(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_TENDER_PUT_IN_TENDER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_SHOP_INFINITY_BUY_ITEM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_JOIN_CANCEL(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_INFINITY_RESTART_BY_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	ProcessResult Process_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	ProcessResult Process_FC_ITEM_USE_INVOKING_WEAR_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 사용
	ProcessResult Process_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 지속 시간 완료
	ProcessResult Process_FC_ITEM_END_COOLINGTIME_ITEM(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 쿨타임 종료
	ProcessResult Process_FC_ITEM_GET_COOLINGTIME_INFO(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 정보 요청 C->F

	ProcessResult Process_FtoA_INFINITY_START_CHECK_ACK(const char* pPacket, int nLength, int &nBytesUsed);		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	ProcessResult Process_FtoA_UPDATE_ITEM_NOTI(const char* pPacket, int nLength, int &nBytesUsed);				// 2010-03-31 by dhjin, 인피니티 입장 캐쉬 아이템 구현 -  
	
	//ProcessResult Process_FC_Difficulty_INFO(const char* pPacket, int nLength, int &nBytesUsed);				// 2010-05-13 by shcho, 인피니티 난이도 조절 - 

	ProcessResult Process_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL ( const char* pPacket , int nLength , int &nByteUsed );		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 인티니티 방의 난이도 변경 요청.

	ProcessResult Process_FC_INFINITY_SKIP_ENDINGCINEMA ( const char* pPacket , int nLength , int &nByteUsed );			// 인피니티 엔딩 시네마 스킵 요청 C->F // 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

	
	/////////////////////////////////////////////////////////////
	// start 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현

	ProcessResult Process_FC_ITEM_PET_SET_NAME(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_PET_SET_EXP_RATIO(const char* pPacket, int nLength, int &nBytesUsed);
	
	ProcessResult Process_FC_ITEM_PET_SET_SOCKET(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_PET_SET_KIT_SLOT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FC_ITEM_PET_SET_AUTOSKILL_SLOT(const char* pPacket, int nLength, int &nBytesUsed);

	BOOL PetChangeLevel(int Level, float PercentageOfExp = NULL);
	BOOL PetChangeExperience(Experience_t PetExperience, BOOL bAbs = FALSE);
	void SendPetInfo(MessageType_t msgType, tPET_CURRENTINFO *pPetCurrentInfo);

	// end 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
	//////////////////////////////////////////////////////////
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	ProcessResult Process_FI_GUILD_BOUNUEXP_RATE(const char* pPacket, int nLength, int &nBytesUsed);


	ProcessResult Process_FC_COLLECTION_SHAPE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 패킷 받기

	void SendToClient_KillCount();
	void SendToClient_RestAndKillCount();

	void SendToClient_BonusExpRate();
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 디버그용
	void PRINT_DEBUG_MESSAGE(char*& msg);
	mt_lock	PRINT_DEBUG_MESSAGE_LOCK;
	void DBGOUT2(BitFlag8_t bType, char* pFormat, ...);
	void StartTestDBG(int nParam1, int nParam2, int nParam3, const char *szParam1, const char *szParam2, const char *szParam3);

public:
	// Send Error Msg
	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE, BOOL i_bWriteSystemLog=TRUE);
	void SendNetworkErrorMessage(int i_nWriteBufCounts, int i_nBadNetworkContinueTime);
	void SendErrorMessageFromFieldDB(EnumQueryType qType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);

	void ADDString(string * o_pszString, int i_int)
	{// 2007-01-17 by dhjin, 스트링 더해주는 함수, 우선 FieldIOCPSocket에 추가함
		char temp[1028];
		util::zero(temp,1024);

		itoa(i_int, temp, 10);
		*o_pszString += temp;
	}

	struct loopdata
	{
		bool running;
		bool pleaseStop;
		bool pleaseDel;

		unsigned interval;
		unsigned hpamount;
		unsigned dpamount;

		loopdata(unsigned interval, unsigned hpamount, unsigned dpamount) :
			running { false },
			pleaseStop { false },
			pleaseDel { false },
			interval { interval },
			hpamount { hpamount },
			dpamount { dpamount } { }
	};

	struct syncdata
	{
		loopdata* data;

		syncdata() : data { nullptr } { }

		~syncdata();

		bool running() const { return data && data->running; }
	};

	syncdata dploopdata { };

	bool CheckDPLoop();
};

// 2008-12-22 by dhjin, 미션마스터 - 상단으로 올림.
// typedef vector<CFieldIOCPSocket*>			vectCFieldIOCPSocket;					// 2007-02-12 by cmkwon



// CIOCPSocket의 인스턴스가 사용 중인지의 여부를 리턴, restrict checking by kelovon
BOOL CFieldIOCPSocket::IsUsingRestrict(ClientIndex_t ClientIndex, UID32_t CharacterUniqueNumber)
{
	return (m_bUsing
			&& (ClientIndex == m_character.ClientIndex)
			&& (CharacterUniqueNumber == m_character.CharacterUniqueNumber)
			&& (GetClientState() == CS_PLAYING));
}


void CFieldIOCPSocket::SetClientState(CS_State csState)
{
	m_CSClientState = csState;
}

// check: MapChannel 도입중, 20040324, kelovon, NPC와의 연결을 채널마다 하나씩 두도록 수정해야 함
BOOL CFieldIOCPSocket::InitializeNPCMap(CMapProject *pmapProject, map<int, MONSTER> *mapMonsterParam)
{
	m_pFieldMapProjectForNPC = reinterpret_cast<CFieldMapProject*>(pmapProject);
	m_pFieldMapProjectForNPC->SetIOCPSocketMapSession(this);
	return TRUE;
}

#endif // !defined(AFX_FIELDIOCPSOCKET_H__6FB6A18E_0C74_4E46_AB75_2ECA21C4204B__INCLUDED_)
