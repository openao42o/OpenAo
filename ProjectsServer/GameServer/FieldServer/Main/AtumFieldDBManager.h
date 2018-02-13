#ifndef __ATUM_FIELD_DB_MANAGER_H__
#define __ATUM_FIELD_DB_MANAGER_H__

#include "AtumDBManager.h"
#include "AtumProtocol.h"

class CFieldIOCP;
class CShopInfo;

struct QPARAM_DELETESTOREITEM
{
	UID32_t		AccountUniqueNumber;
	UID64_t		ItemUniqueNumber;			// STORE DB의 index
};

struct QPARAM_BUFFER
{
	INT			nSize;
	char		*pBuffer;
	UINT		GeneralParam1;
	UINT		GeneralParam2;
};

struct QPARAM_UPDATEITEMCOUNT
{
	UID64_t		ItemUniqueNumber;
	INT			Count;
};

struct QPARAM_UPDATEITEMNUM			// 2006-06-14 by cmkwon
{
	UID64_t		ItemUniqueNumber;
	INT			ItemNum;
};

struct QPARAM_UPDATE_ENDURANCE
{
	UID64_t		ItemUniqueNumber;
	float		Endurance;
};

struct QPARAM_STORE_UPDATE
{// 2006-09-19 by dhjin, 유저 인벤, 창고 저장 관련
	UID64_t		ItemUniqueNumber;
	UID32_t		CharacterUID;
	BYTE		ItemStorage;
};

struct QPARAM_STORE_UPDATE_COLORCODE
{
	UID64_t		ItemUniqueNumber;
	INT			ColorCode;
};

struct QPARAM_UPDATE_SHAPEITEMNUM		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
{
	UID64_t		ItemUID;
	INT			nShapeItemNum;

	FIXED_TERM_INFO	FixedTermShape;		// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
};

struct QPARAM_UPDATE_ITEM_USINGTIMESTAMP
{
	UID64_t		ItemUniqueNumber;
	INT			UsingTimeStamp1;
};

struct QPARAM_UPDATE_ITEM_RARE_FIX
{
	UID64_t	ItemUID;
	INT		PrefixCodeNum;	// 접두사, 없으면 0
	INT		SuffixCodeNum;	// 접미사, 없으면 0
};

struct QPARAM_INSERTDEFAULTITEMS
{
	UID32_t		CharacterUniqueNumber;
};

// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
struct QPARAM_FIXED_TERM_SHAPE
{
	UID64_t			ItemUID;
	INT16			TermType;
	INT				AppliedItemNum;
	ATUM_DATE_TIME	StartDate;
	ATUM_DATE_TIME	EndDate;
};
// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

// 2013-05-31 by jhseol,bckim 아머 컬렉션 - QPARAM_SHAPE_STATLEVEL 구조체 추가
struct QPARAM_SHAPE_STATLEVEL
{
	BYTE			ItemKind;
	UID64_t			ItemUID;
	ItemNum_t		nStatShapeItemNum;	// 소스인데스(쉐이프넘버)
	INT				nStatLevel;			// 레벨
	QPARAM_SHAPE_STATLEVEL()
	{
		memset(this,0x00,sizeof(QPARAM_SHAPE_STATLEVEL));
	}
};
// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - QPARAM_SHAPE_STATLEVEL 구조체 추가

class CFieldIOCPSocket;

struct QPARAM_EXECUTETRADE
{
	CFieldIOCPSocket *pPeerTraderSocket;
};
struct QPARAM_TRADE_MOVEITEM		// 2006-05-11 by cmkwon
{
	UID32_t		ToAccountUID;			
	UID32_t		ToCharacterUID;			
	UID32_t		FromCharacterUID;		
	UID64_t		MoveItemUID;				// 이동할 아이템의 UID
};

struct QPARAM_BUY_ITEM
{
	MSG_FC_SHOP_BUY_ITEM	MsgBuyItem;
	CShopInfo				*pShopInfo;
	SHOP_ITEM				*pShopItem;
	ITEM_GENERAL			*pMoney;
};

struct QPARAM_SELL_ITEM
{
	MSG_FC_SHOP_SELL_ITEM	MsgSellItem;
	ITEM_GENERAL			*pItemToSell;
	ITEM					*pItemInfoToSell;
	ITEM_GENERAL			*pMoney;
};

struct QPARAM_THROW_AWAY_ITEM
{
	MSG_FC_ITEM_THROW_AWAY_ITEM	MsgThrowAwayItem;
	ITEM_GENERAL				*pItemToThrowAway;
};

struct QPARAM_CHARACTER_CHANGE_UNITKIND
{
	UID32_t	CharacterUniqueNumber;
	USHORT	UnitKind;
};

struct QPARAM_CHARACTER_SAVE_CRITICAL_DATA
{
	UID32_t			CharacterUID;
	BYTE			Level;
	BYTE			CharacterMode0;		// 2005-10-06 by cmkwon, 캐릭터모드 저장을 위해
	Experience_t	Experience;
	Experience_t	DownExperience;		// 2005-03-23 by cmkwon
	INT				DownSPIOnDeath0;	// 2006-04-10 by cmkwon
	BodyCond_t		BodyCondition;
	BYTE			PetLevel;
	Experience_t	PetExperience;
	float			Position_X;
	float			Position_Y;
	float			Position_Z;
	MapIndex_t		MapIndex;
	ChannelIndex_t	ChannelIndex;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT			SP;
	SHORT			EP;
	float			CurrentHP;
	float			CurrentDP;
	SHORT			CurrentSP;
	float			CurrentEP;
	LONGLONG		TotalPlayTime;
	INT				GameContinueTimeInSecondOfToday;		// 2006-11-15 by cmkwon
	ATUM_DATE_TIME	LastGameEndDate;						// 2006-11-15 by cmkwon
	LONGLONG		PCBangTotalPlayTime;					// 2007-06-07 by dhjin, PC방 총 플레이 시간.
	INT				SecretInfoOption;				// 2008-06-23 by dhjin, EP3 유저정보옵션 -
};

struct QPARAM_CHARACTER_CHANGE_STAT
{
	UID32_t		CharacterUniqueNumber;
	BYTE		byAutoStatType;
	GEAR_STAT	GearStat1;		// 기어 스탯
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT		SP;
	SHORT		EP;
	BYTE		BonusStat;
};


struct QPARAM_CHARACTER_CHANGE_BONUSSTATPOINT		// 2005-11-15 by cmkwon, 추가함
{
	UID32_t		CharacterUniqueNumber;
	BYTE		BonusStatPoint;
};

struct QPARAM_CHARACTER_CHANGE_GUILD
{
	UID32_t	CharacterUniqueNumber;
	char	Guild[SIZE_MAX_GUILD_NAME];
};

struct QPARAM_CHARACTER_CHANGE_LEVEL
{
	UID32_t	CharacterUniqueNumber;
	BYTE	Level;
	BYTE	BonusStat;
// 2005-11-15 by cmkwon, 삭제함
//	BYTE	BonusSkillPoint;
};

struct QPARAM_CHARACTER_CHANGE_EXP
{
	UID32_t			CharacterUniqueNumber;
	Experience_t	Experience;						// 총 경험치
};

struct QPARAM_CHARACTER_CHANGE_BODYCONDITION
{
	UID32_t		CharacterUniqueNumber;
	BodyCond_t	BodyCondition;					// 앞의 32bit만 사용 <-- check: 스킬 bodycon이 정해지면 결정하기!
};

/*
struct QPARAM_CHARACTER_CHANGE_PROPENSITY
{
	UID32_t	CharacterUniqueNumber;
	INT		Propensity;
};
*/

struct QPARAM_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	UID32_t		AccountUniqueNumber;				// 2005-12-07 by cmkwon, 선택가능세력마스트 설정을 위해
	UID32_t		CharacterUniqueNumber;
	BYTE		InfluenceType0;
	BYTE		SelectableInfluenceMask0;			// 2005-12-07 by cmkwon, 선택가능세력마스트 설정을 위해
};

struct QPARAM_CHARACTER_CHANGE_STATUS
{
	UID32_t	CharacterUniqueNumber;
	BYTE	Status;
};

struct QPARAM_CHARACTER_CHANGE_PKPOINT
{
	UID32_t	CharacterUniqueNumber;
	INT		Propensity;
	UINT	PKWinPoint;
	UINT	PKLossPoint;
};

struct QPARAM_CHARACTER_CHANGE_RACINGPOINT
{
	UID32_t	CharacterUniqueNumber;
	INT		RacingPoint;
};

struct QPARAM_CHARACTER_UPDATE_TOTAL_PLAY_TIME
{
	UID32_t		CharacterUniqueNumber;
	LONGLONG	TotalPlayTime;
};

struct QPARAM_CHARACTER_CHANGE_HPDPSPEP
{
	UID32_t	CharacterUniqueNumber;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT	SP;
	SHORT	EP;
	float	CurrentHP;
	float	CurrentDP;
	SHORT	CurrentSP;
	float	CurrentEP;
};

struct QPARAM_CHARACTER_CHANGE_CURRENTHPDPSPEP
{
	UID32_t	CharacterUniqueNumber;
	float	CurrentHP;
	float	CurrentDP;
	SHORT	CurrentSP;
	float	CurrentEP;
};

struct QPARAM_CHARACTER_CHANGE_MAPCHANNEL
{
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct QPARAM_CHARACTER_CHANGE_PETINFO
{
	UID32_t			CharacterUniqueNumber;
	BYTE			Level;
	Experience_t	Experience;						// 총 경험치
};

struct QPARAM_CHARACTER_CHANGE_POSITION
{
	UID32_t		CharacterUniqueNumber;
	D3DXVECTOR3	PositionVector;		// 캐릭터 좌표
};

struct QPARAM_UPDATE_QUEST
{
	UID32_t		CharacterUniqueNumber;
	INT			QuestIndex;
	BYTE		QuestState;				// QUEST_STATE_XXX
	LONGLONG	QuestProgressTimeInSecond;		// 2006-08-31 by cmkwon, 퀘스트 완료시 진행 시간(초), 퀘스트 시작시 TotalPlayTime
};

struct QPARAM_DELETE_QUEST
{
	UID32_t	CharacterUniqueNumber;
	INT		QuestIndex;
};

struct QPARAM_UPDATE_QUEST_MONSTER_COUNT
{
	UID32_t	CharacterUniqueNumber;
	INT		QuestIndex;
	INT		MonsterUniqueNumber;
	INT		Count;
};

struct QPARAM_DELETE_QUEST_MONSTER_COUNT
{
	UID32_t	CharacterUniqueNumber;
	INT		QuestIndex;
};

struct QPARAM_CHANGE_QUEST_INDEX		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
{
	UID32_t	CharacterUID;
	INT		FromQuestIndex;
	INT		ToQuestIndex;
};

struct QPARAM_GENERAL_EXEC
{
	char	QueryString[256];
};

struct QPARAM_CHANGE_ITEM_WINDOW_POSITION
{
	UID32_t	AccountUID;
	UID32_t	CharacterUID;
	UID64_t	ItemUID1;
	INT		ItemWindowIndex1;
	BYTE	ItemWear1;
	UID64_t	ItemUID2;
	INT		ItemWindowIndex2;
	BYTE	ItemWear2;
};

struct QPARAM_CASH_INSERT_PREMIUM_CARD
{
	UID32_t			nAccountUID;
	INT				nItemNum;
	ATUM_DATE_TIME	atumTimeCurrentTime;
	ATUM_DATE_TIME	atumTimeExpireTime;	
};

typedef QPARAM_CASH_INSERT_PREMIUM_CARD		QPARAM_CASH_UPDATE_PREMIUM_CARD;

struct QPARAM_CASH_DELETE_PREMIUM_CARD
{
	UID32_t			nAccountUID;
};

struct QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE
{
	BYTE			byInfluenceType0;		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - QPARAM_UPDATE_HAPPY_HOUR_EVENT_DATE 구조체에 필드 추가 함
	ATUM_DATE_TIME	atimeStartTime2;
	ATUM_DATE_TIME	atimeEndTime2;
};

struct QPARAM_UPDATE_INFLUENCE_WAR_DATA		// 2006-04-14 by cmkwon
{
	BYTE			InfluenceType0;
	//INT				MGameServerID0;
	BYTE			WartimeStage0;
	INT				ContributionPoint0;
};

// 2013-05-09 by hskim, 세력 포인트 개선
struct QPARAM_UPDATE_INFLUENCE_CONSECUTIVE_VICTORIES
{
	BYTE			InfluenceType0;
	//INT				MGameServerID0;
	SSHORT			ConsecutiveVictories;
};
// end 2013-05-09 by hskim, 세력 포인트 개선

struct QPARAM_UPDATE_OWNEROFCONFLICTAREA	// 2006-07-12 by cmkwon
{
	BYTE			InfluenceType0;			// 소유 세력
	//INT				MGameServerID0;
};

struct FIELD_DB_QUERY
{
	CFieldIOCPSocket*	pFieldIOCPSocket;
	UID32_t				nCharacterUID;
	EnumQueryType		enumQuryType;
	void*				pQueryParam;
	void*				pGeneralParam;		// General-Purpose void* Parameter
	INT64				nGeneralParam1;		// General-Purpose INT   Parameter
	INT64				nGeneralParam2;		// General-Purpose INT   Parameter
	DWORD				dwProcessedTick;	// 쿼리가 처리되는 시간

	FIELD_DB_QUERY(const DB_QUERY& rhs)
	{
		this->pFieldIOCPSocket	= (CFieldIOCPSocket*)rhs.pIOCPSocket;
		this->nCharacterUID		= rhs.nCharacterUID;
		this->enumQuryType		= rhs.enumQueryType;
		this->pQueryParam		= rhs.pQueryParam;
		this->pGeneralParam		= rhs.pGeneralParam;
		this->nGeneralParam1	= rhs.nGeneralParam1;
		this->nGeneralParam2	= rhs.nGeneralParam2;
		this->dwProcessedTick	= rhs.dwProcessedTick;
	}
};
// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 정보 구조체 추가
struct QPARAM_CHECK_LOGINEVENTITEM
{ // 통합적으로 사용한다.
	int		EventUID;
	int		AccountUID;
	int		CompleteFlag; // check를 위한 경우에는 사용하지 않는다.	
};
// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 정보 구조체 추가

// 2006-08-25 by dhjin, 이벤트 아이템 목록 받아오는 프로시저를 위해
struct QPARAM_CHECK_EVENTITEM
{
	int		AccountUID;
	int		ItemEventUID;
	int		ItemEventType;
	int		ItemNum;
	int		Count;
	int		PrefixCodeNum;			// 2007-07-25 by dhjin, 접두 옵션
	int		SuffixCodeNum;			// 2007-07-25 by dhjin, 접미 옵션
	INT		CharacterUID;			// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 -

	BOOL	UseFixedPeriod;			// 2013-02-28 by bckim, 복귀유져 버프추가
	INT		FixedPeriod_DAY;		// 2013-02-28 by bckim, 복귀유져 버프추가

	// 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
	BYTE	InsertStorage;		// 아이템을 추가할 위치 (캐릭터 or 창고)

	QPARAM_CHECK_EVENTITEM()
	{
		memset(this, 0x00, sizeof(QPARAM_CHECK_EVENTITEM));
		InsertStorage = ITEM_IN_CHARACTER;	// 기본값을 캐릭터로 잡는다.
	}
	// end 2013-04-22 by jhseol, 아이템 이벤트 - 서브타입 추가구조 시스템 보안
};
typedef mt_vector<QPARAM_CHECK_EVENTITEM>			mtvectQPARAM_CHECK_EVENTITEM;		// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 

// 2006-08-25 by dhjin, 이벤트 아이템 지급 받은 기록을 하는 프로시저를 위해
struct QPARAM_INSERT_EVENTITEM
{
	int				AccountUID;
	int			    CharacterUID;
	int				ItemEventUID;
	ATUM_DATE_TIME  InsertItemTime;		// 2007-12-18 by dhjin, 이벤트 아이템 지급 시간 버그 수정
};

struct QPARAM_CheckCouponEvent			// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
{
	char	AccName[SIZE_MAX_ACCOUNT_NAME];
	char	CouponNumber[SIZE_MAX_COUPON_NUMBER];
	int		ItemEventUID;
};

struct QPARAM_GET_QUICKSLOT
{// 2006-09-04 by dhjin, 퀵 슬롯 정보 얻어오는 프로시저를 위해
	int				AccountUID;
	int			    CharacterUID;
	int				SlotPurpose;		// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯
};

struct QPARAM_DELETE_QUICKSLOT
{// 2006-09-04 by dhjin, 퀵 슬롯 삭제 프로시저를 위해
	int				AccountUID;
	int			    CharacterUID;
	int				TabIdx;
	int				SlotPurpose;		// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯
};

struct QPARAM_UPDATE_QUICKSLOT
{// 2006-09-04 by dhjin, 퀵 슬롯 업데이트 프로시저를 위해
	int				AccountUID;
	int			    CharacterUID;
	int				TabIdx;
	int				arrItemNum_[QUICKSLOTCOUNT];
	int				SlotPurpose;		// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯
};

struct QPARAM_UPDATE_GUILDSTORE
{// 2006-09-20 by dhjin, 여단 창고 업데이트 프로시저 QPARAM
	UID64_t			ItemUID;
	UID32_t			AccountUID;
	UID32_t			Possess;
	BYTE			ItemStorage;
	INT				Count;
};

struct QPARAM_INSERT_GUILDSTORE
{// 2006-09-20 by dhjin, 여단 창고 추가 프로시저 QPARAM
	UID32_t			GuildUID;
	INT				ItemNum;
	INT				Count;
};

struct QPARAM_DELETE_GUILDSTORE
{// 2006-09-20 by dhjin, 여단 창고 삭제 프로시저 QPARAM
	UID64_t			ItemUID;
	UID32_t			GuildUID;
	BYTE			byItemStorage;			// ITEM_IN_XXX
	BOOL			SendMSG;	
};

struct QPARAM_GET_GUILDSTORE
{// 2006-09-20 by dhjin, 여단 창고 얻어오는 프로시저 QPARAM
	UID32_t			GuildUID;
};

struct QPARAM_UPDATE_SKILLUSETIME
{// 2006-12-06 by dhjin, 스킬 사용 시간 업데이트하는 프로시저 QPARAM
	UID64_t			ItemUID;
	ATUM_DATE_TIME	UseTime;
};

struct QPARAM_UPDATE_SUBLEADER
{// 2007-02-13 by dhjin, 부지도자 설정 프로시저 QPARAM
	BYTE			InflType;
	BYTE			SubLeaderRank;		// 2007-10-06 by dhjin, 몇 번째 부지도자인지
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE			OrderNormalRace;	 // 2008-08-21 by dhjin, 일반, 특수 계정의 부지도자 임명 제한
};

//////////////////////////////////////////////////////////////////////////
// 2007-02-28 by dhjin, 전략포인트 랜덤 생성 
struct QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_COUNT
{// 2007-02-28 by dhjin, 전략포인트 생성 정보 업데이트
	MapIndex_t		MapIndex;
	BYTE			SummonCount;
};

struct QPARAM_UPDATE_STRATEGYPOINTSUMMONINFO_TIME
{// 2007-02-28 by dhjin, 전략포인트 생성 정보 업데이트
	MapIndex_t		MapIndex;
	ATUM_DATE_TIME	SummonTime;
	BOOL			SummonAttribute;		// 2008-04-08 by dhjin, 소환 가능 시간 설정 - 
};

struct QPARAM_DELETE_INSERT_STRATEGYPOINTSUMMONINFO
{// 2007-02-28 by dhjin, 전략포인트 생성 정보 업데이트
	MapIndex_t		MapIndex;
};

struct QPARAM_JAMBOREE_ENTRANTS			// 2007-04-10 by cmkwon, 대회서버군 DB(atum2_db_20)에 캐릭터 정보 복사
{
	char		szCharName0[SIZE_MAX_CHARACTER_NAME];
	BYTE		byInflTy;				// 2008-04-15 by cmkwon, 대회서버(JamboreeServer)는 시스템 수정 - "/대회서버참가자" 명령어 수정
	MapIndex_t	MapIndex;				// 2008-04-15 by cmkwon, 대회서버(JamboreeServer)는 시스템 수정 - "/대회서버참가자" 명령어 수정
};

struct QPARAM_UPDATE_WARPOINT
{// 2007-05-15 by dhjin, 변경되는 WarPoint 양
	INT			TotalWarPoint;
	INT			CumulativeWarPoint;
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
};

struct QPARAM_UPDATE_ARENA_RESULT
{// 2007-06-07 by dhjin, Arena결과 업데이트
	INT			TotalWarPoint;
	INT			CumulativeWarPoint;
	INT			ArenaWin;
	INT			ArenaLose;
	UID32_t		AccountUID;
	UID32_t		CharacterUID;	
};

struct QPARAM_UPDATE_ARENA_DISCONNECT
{// 2007-06-07 by dhjin, Arena 강제종료 업데이트
	INT			ArenaDisConnect;
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
};

///////////////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트
struct QPARAM_UPDATE_ARENA_CHARACTERARENA
{
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
	SArenaPlayInfo	ArenaInfo;
};
// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트


//////////////////////////////////////////////////////////////////////////
// 2007-07-06 by dhjin, Tutorial
struct QPARAM_INSERT_TUTORIAL_COMPLETE
{// 2007-07-06 by dhjin, Tutorial 완료 저장
	UID32_t		CharacterUID;		
	INT			TutorialUID;					// Tutorial 번호 
};


///////////////////////////////////////////////////////////////////////////////
// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - DB Query 구조체 추가
struct QPARAM_Insert2WarpableUserList		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 -
{
	INT			MapIndex;
	BYTE		byInfluenceType0;
	char		CharacterName0[SIZE_MAX_CHARACTER_NAME];
};

struct QPARAM_DeleteWarpableUser			// 2007-08-30 by cmkwon, 회의룸 시스템 구현 -
{
	INT			MapIndex;
	UID32_t		CharacterUID0;
};


//////////////////////////////////////////////////////////////////////////
// 2007-08-21 by dhjin, OutPost
struct QPARAM_SET_NPCPOSSESSION_OUTPOST
{// 2007-08-21 by dhjin, 전진기지 NPC소유로 변경
	INT				MapIndex;
	ATUM_DATE_TIME	GetTime;
	ATUM_DATE_TIME	NextWarTime;
};

struct QPARAM_SET_GUILDPOSSESSION_OUTPOST
{// 2007-08-21 by dhjin, 전진기지 NPC소유로 변경
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			GuildUID;
	ATUM_DATE_TIME	GetTime;
	ATUM_DATE_TIME	NextWarTime;
};

struct QPARAM_SET_OUTPOST_NEXTWAR_TIME
{// 2007-08-21 by dhjin, 전진기지 다음 전쟁 시간 설정
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			GuildUID;
	ATUM_DATE_TIME	NextWarTime;
};

//////////////////////////////////////////////////////////////////////////
// 2007-08-22 by dhjin, CityLeader
struct QPARAM_INIT_EXPEDIENCY_FUND
{// 2007-08-22 by dhjin, 판공비 초기화 설정
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
	INT				ExpediencyFundCumulative;
	INT				ExpediencyFund;
	INT				ExpediencyRate;
};

struct QPARAM_SET_EXPEDIENCY_FUND
{// 2007-08-22 by dhjin, 판공비 정보 설정
	INT				MapIndex;
	INT				ExpediencyFundCumulative;
	INT				ExpediencyFund;
};

struct QPARAM_SET_EXPEDIENCY_FUNDPAYBACK
{// 2007-08-22 by dhjin, 판공비 환급 정보
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
	INT				ExpediencyFund;
};

struct QPARAM_SET_EXPEDIENCY_FUNDRATE
{// 2007-08-22 by dhjin, 판공비율 정보 설정
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
	INT				ExpediencyRate;
};

struct QPARAM_DELETE_CITYLEADER_INFO
{// 2007-08-22 by dhjin, 도시 정보 삭제
	INT				MapIndex;
	BYTE			Influence;
	UID32_t			CharacterUID;
};

struct QPARAM_REG_NOTICE
{// 2007-08-22 by dhjin, 공지사항 등록
	INT				MapIndex;
	UID32_t			CharacterUID;
	CHAR			Notice[SIZE_MAX_NOTICE];
};
typedef QPARAM_REG_NOTICE		QPARAM_MODIFY_NOTICE;

struct QPARAM_UPDATE_SECONDARY_PASSWORD
{// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로시저 호출을 위한 구조체 추가
	UID32_t			AccountUID;
	char			NewSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};

//////////////////////////////////////////////////////////////////////////
// 2007-10-30 by dhjin, Poll관련
struct QPARAM_INSERT_LEADER_CANDIDATE
{// 2007-10-30 by dhjin, 후보자 등록
	BYTE	Influence;
	INT		ReqGuildMemberNum;
	INT		ReqGuildFame;
	UID32_t	AccountUID;
	UID32_t	CharacterUID;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	CHAR	CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];
};

struct QPARAM_UPDATE_LEADERPOLLCOUNT
{// 2007-11-06 by dhjin, 투표 점수 업데이트
	INT		LeaderCandidateNum;				//	후보자 번호
	BYTE	Influence;						//	후보자 세력
	INT		PollCount;						//	투표 점수
};

struct QPARAM_INSERT_VOTER_LIST
{// 2007-10-31 by dhjin, 투표자 등록
	UID32_t		AccountUID;						//	투표자 계정 UID
	UID32_t		CharacterUID;					//	투표자 UID
	INT			LeaderCandidateNum;				//	투표자가 투표한 후보자 번호
};

struct QPARAM_UPDATE_ConnectingServerGroupID
{// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기
	UID32_t		AccountUID;						//	계정 UID
	INT			ServerGroupID;					// 2007-11-06 by cmkwon, 0이면 접속 종료
};

struct QPARAM_GiveStoreItem
{// 2007-11-27 by cmkwon, 선물하기 로그 수정 - QPARAM_GiveStoreItem 추가
	UID32_t			SenderCharUID;
	char			SenderCharacterName[SIZE_MAX_CHARACTER_NAME];	
	ITEM_GENERAL	itemG;
};

//////////////////////////////////////////////////////////////////////////
// 2007-12-28 by dhjin, 아레나 통합 - 
struct QPARAM_ARENA_UPDATE_CHARACTERINFO
{// 2007-12-28 by dhjin, 아레나 통합 - 케릭터 정보 업데이트
	CHAR			NewArenaCharacterName[SIZE_MAX_CHARACTER_NAME]; 
	ClientIndex_t	AFSClientIndex;
};

struct QPARAM_ARENA_COPY_DB_INFO
{// 2007-12-28 by dhjin, 아레나 통합 - 아레나 DB에 정보 복사
	UID32_t			ArenaCharacterUID;
	UID32_t			CharacterUID;
	UID32_t			AccountUID;
	ClientIndex_t	AFSClientIndex;
	BOOL			InfinityCopy;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피니티 이용인지 체크 비트 
};

typedef MSG_FC_SHOP_CHECK_GIVE_TARGET	QPARAM_CHECK_GiveTarget;	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 

//////////////////////////////////////////////////////////////////////////
// 2008-04-10 by dhjin, 모선전 정보 표시 기획안 - 
struct QPARAM_UPDATE_MSWAR_OPTION_TYPE
{
	BYTE		InfluenceType;
	SHORT		MSWarOptionType;
};

struct QPARAM_INSERT_MSWAR_LOG
{// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
	BYTE			AttInfluence;
	INT				MonsterUID;
	INT				ContributionPoint;
	ATUM_DATE_TIME	MSWarStartTime;
	BYTE			WinInfluence;	
};

struct QPARAM_INSERT_SPWAR_LOG
{// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
	BYTE			AttInfluence;
	INT				SPSummonMapIndex;
	ATUM_DATE_TIME	SPWarStartTime;
	BYTE			WinInfluence;	
};

// 2008-11-04 by dhjin, 럭키머신
struct QPARAM_UPDATE_LUCKYITEM_DROPCOUNT
{
	INT				MysteryItemDropNum;
	INT				DropItemNum;
	INT				DropCount;
};

struct QPARAM_UPDATE_LUCKYITEM_STARTTIME
{
	INT				MysteryItemDropNum;
	ATUM_DATE_TIME	StartTime;		
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고
struct QPARAM_UPDATE_START_DECLARATION_OF_WAR
{
	BYTE			Influence;
	BYTE			MSWarStep;
	INT				NCP;
	INT				MSNum;
	MapIndex_t		MSAppearanceMap;
};

struct QPARAM_UPDATE_END_DECLARATION_OF_WAR
{
	BYTE			Influence;
	BYTE			MSWarStep;
	ATUM_DATE_TIME	MSWarEndTime;
	BYTE			MSWarEndState;
};

struct QPARAM_UPDATE_MSWAR_START_TIME
{
	BYTE			Influence;
	BYTE			MSWarStep;
	ATUM_DATE_TIME	MSWarStartTime;
	BYTE			SelectCount;
	BOOL			GiveUp;
};

///////////////////////////////////////////////////////////////////////////////
// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
#define	DB_THREAD_INDEX_0				0
#define	DB_THREAD_INDEX_1				1	// 2008-04-30 by cmkwon, QT_UpdateDBServerGroup, QT_CheckConnectableAccount
#define	DB_THREAD_INDEX_2				2
#define	DB_THREAD_INDEX_3				3
#define	DB_THREAD_INDEX_4				4
#define	DB_THREAD_INDEX_5				5
#define	DB_THREAD_INDEX_6				6
#define	DB_THREAD_INDEX_7				7
#define	DB_THREAD_INDEX_8				8
#define	DB_THREAD_INDEX_9				9

//////////////////////////////////////////////////////////////////////////
// 2008-04-24 by dhjin, EP3 편지 시스템 - 
struct QPARAM_READ_LETTER
{
	UID64_t		LetterUID;
	UID32_t		RecvCharacterUID;
};

typedef QPARAM_READ_LETTER		QPARAM_DELETE_LETTER;
typedef QPARAM_DELETE_LETTER	QPARAM_DELETE_READALLLETTER;
typedef QPARAM_READ_LETTER		QPARAM_READ_ALLLETTER;

struct QPARAM_SEND_LETTER
{
	CHAR		RecvCharacterName[SIZE_MAX_CHARACTER_NAME];
	CHAR		Title[SIZE_MAX_LETTER_TITLE];
	CHAR		Content[SIZE_MAX_LETTER_CONTENT];
};

struct QPARAM_DELETE_ALL_LETTER
{
	UID64_t		AllLetterUID;
};

struct QPARAM_SEND_ALLLETTER
{
	CHAR		Title[SIZE_MAX_LETTER_TITLE];
	CHAR		Content[SIZE_MAX_LETTER_CONTENT];
};

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
struct QPARAM_INSERT_INFINITY {
	UID32_t			AccountUID;
	UID32_t			CharacterUID;
	InfiModeUID_t	InfinityModeUID;
	ATUM_DATE_TIME	StartTime;
};
struct QPARAM_UPDATE_INFINITY {
	UID32_t			AccountUID;
	UID32_t			CharacterUID;
	InfiModeUID_t	InfinityModeUID;
};

struct QPARAM_CHARACTER_SAVE_DATA_INFINITY_FIN {
	UID32_t				CharacterUID;
	GEAR_STAT			GearStat;						// 기어 스탯
	BYTE				Level;							//
	Experience_t		Experience;						//
	Experience_t		DownExperience;					// 캐릭이 죽을때 떨어진 경험치
	INT					DownSPIOnDeath;					// 2006-04-10 by cmkwon, 캐릭이 죽을때 떨어진 SPI
	BYTE				BonusStat;						// 추가 2002.12.13
	BYTE				BonusStatPoint;					// 2005-11-15 by cmkwon, 레벨업이 아닌 다른방법으로 받은 보너스 스탯 포인트, BonusSkillPoint를 변경함 // 추가 2002.12.13
	LONGLONG			TotalPlayTime;					// 초단위
	ATUM_DATE_TIME		LevelUpTime;					// 2006-12-18 by dhjin, 레벨업 시간
	INT					WarPoint;						// 2007-04-17 by dhjin, WP
	INT					CumulativeWarPoint;				// 2007-05-28 by dhjin, 누적WP
	LONGLONG			PCBangTotalPlayTime;			// 2007-06-07 by dhjin, PC방 총 플레이 시간, 초단위
	INT					SecretInfoOption;				// 2008-06-23 by dhjin, EP3 유저정보옵션 -
	MapIndex_t			InfinityMapIndex;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	ChannelIndex_t		InfinityChannelIndex;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
};

struct QPARAM_INFINITY_FIN_UPDATE_ITEM {
	UID64_t			MainSvrItemUID;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				UsingTimeStamp;
	INT				CoolingTimeStamp;					// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
};

struct QPARAM_INFINITY_INSERT_LOG {
	InfiModeUID_t	InfinityModeUID;
	DeathCnt_t		DeathCount;
	KillCnt_t		KillCount;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	Experience_t	AllEXP;								// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 종료시 최대 경험치 추가
};

struct QPARAM_INFINITY_RESET_IMPUTE_SERVERSTART {
	InfiModeUID_t	InfinityModeUID;
	ATUM_DATE_TIME	StartTime;
};

struct QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP {	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	UID64_t		ItemUniqueNumber;
	INT			CoolingTimeStamp;
};

struct QPARAM_INFINITY_UPDATE_USER_MAP_INFO		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
{
	UID32_t				CharacterUID;
	MapIndex_t			InfinityMapIndex;
	ChannelIndex_t		InfinityChannelIndex;
};

// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
struct QPARAM_INFINITY_LOG_USER_GET_TENDERITEM_INFO // 체크
{
	INT			TenderItemNum;		// 획득한 아이템 넘버
	DiceCnt_t	GDicecount;			// 획득한 주사위 카운트
	UID64_t		AccountUID;			// 계정 UID
	CHAR		AccountName[SIZE_MAX_ACCOUNT_NAME];	// 계정 이름
	UID64_t		CharacterUID;						// 케릭터 UID
	CHAR		CharacterName[SIZE_MAX_CHARACTER_NAME];	// 케릭터 이름
};								   
// END 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장

// Start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 로그인 이벤트 삽입 정보
struct QPARAM_ITEM_LOGIN_EVENT
{
	INT EventUniqueNumber;
	INT AccountUniqueNumber; 
	INT CompleteFlag;
};
// End 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 로그인 이벤트 삽입 정보

/////////////////////////////////////////////////////////////
// start 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현

struct QPARAM_PET_UPDATE_INFINITY_FIN
{
	UID64_t			UniqueNumber;
	char			PetName[SIZE_MAX_PET_NAME];
	INT				PetLevel;
	Experience_t	PetExp;
	BYTE			PetExpRatio;
	BYTE			PetEnableSocketCount;

	SPET_KIT_SLOT_DATA	PetKitHP;
	SPET_KIT_SLOT_DATA	PetKitShield;
	SPET_KIT_SLOT_DATA	PetKitSP;

	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;
};

struct QPARAM_PET_SET_NAME
{
	UID64_t			ItemUniqueNumber;
	char			PetName[SIZE_MAX_PET_NAME];
};

struct QPARAM_PET_SET_EXP_RATIO
{
	UID64_t			ItemUniqueNumber;
	BYTE			ExpRatio;
};

struct QPARAM_PET_SET_LEVEL
{
	UID64_t			ItemUniqueNumber;
	INT				Level;
};

struct QPARAM_PET_SET_EXP
{
	UID64_t			ItemUniqueNumber;
	Experience_t	Exp;
};

struct QPARAM_PET_SET_SOCKET
{
	UID64_t			ItemUniqueNumber;
	UID64_t			PetSocketItemUID[SIZE_MAX_PETSOCKET];
};

// 2015-06-22 Future, upgrading Pets with sockets
struct QPARAM_UPGRADE_PET
{
	UID64_t			OldItemUniqueNumber;
	UID32_t			UpgradedPetItemUID;
};

struct QPARAM_PET_SET_KIT_SLOT
{
	UID64_t				ItemUniqueNumber;
	SPET_KIT_SLOT_DATA	PetKitHP;
	SPET_KIT_SLOT_DATA	PetKitShield;
	SPET_KIT_SLOT_DATA	PetKitSP;
};

struct QPARAM_PET_SET_AUTOSKILL_SLOT
{
	UID64_t						ItemUniqueNumber;
	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;
};

struct QPARAM_PET_CHANGE_SOCKET_OWNER
{
	UID64_t		ItemUniqueNumber;
	UID32_t		CharacterUID;
};

// end 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
/////////////////////////////////////////////////////////////

// start 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
struct QPARAM_STORE_GET_ITEM_ONE
{
	UID64_t		ItemUniqueNumber;
	BOOL		bSendToClient;			// Client Inventory 에 전송할지 여부 (추후 확장시 Type 값으로 변경해서 처리)
};
// end 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

// start 2012-08-27 by hskim, 베트남 인첸트 핵 관련 추가 보안 처리 (코드상의 문제점은 발견되지 않았으나 만일의 경우에는 로그를 남긴다) (동기화 DB 접속을 시도해서 성능 저하가 있음 : 필요없다고 판단되면 삭제 진행)
struct QPARAM_GET_ENCHANT_COUNT
{
	UID64_t		ItemUniqueNumber;
	INT			EnchantCount;
};
// end 2012-08-27 by hskim, 베트남 인첸트 핵 관련 추가 보안 처리 (코드상의 문제점은 발견되지 않았으나 만일의 경우에는 로그를 남긴다) (동기화 DB 접속을 시도해서 성능 저하가 있음 : 필요없다고 판단되면 삭제 진행)

#if S_BONUSEXPSYSTEM_RENEWAL
// 2012-10-07 by hskim, 휴식 경험치
struct QPARAM_ADDEXP_REST_EXPERIENCE_COUNT
{
	UID32_t		AccountUID;
	INT			RestExperienceCount;
};
// end 2012-10-07 by hskim, 휴식 경험치
#endif // S_BONUSEXPSYSTEM_RENEWAL

#if S_BONUS_KILL_SYSTEM_RENEWAL
// 2015-06-11 Future, Kill Count System
struct QPARAM_ADDKILL_REST_KILL_COUNT
{
	UID32_t		AccountUID;
	INT			RestKillCount;
};
// end 2015-06-11 Future, Kill Count System
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.
struct QPARAM_ARENA_CHARACTER_RESET
{
	UID32_t			AccountUID;
	UID32_t			CharacterUID;
	UID32_t			ArenaCharacterUID;
};
// end 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.

///////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
struct QPARAM_STRATEGYPOINT_SUMMON_INFO
{
	MapIndex_t		MapIndex;
	BYTE			SummonCount;
	ATUM_DATE_TIME	SummonTime;
	BOOL			SummonAttribute;
};
// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
struct QPARAM_TEMPORARY_SYSTEM_INFOMATION
{
	INT				Type;
	UID64_t			UID;
	ATUM_DATE_TIME	Date;
	UID64_t			Data1;
	INT				Data2;
	CHAR			Data3[SIZE_MAX_TEMP_SYSTEM_STRING];
	CHAR			Data4[SIZE_MAX_TEMP_SYSTEM_STRING];
};

struct QPARAM_GET_STORE_EXTENSION
{
	UID64_t			ItemUID;
	UID64_t			Gesture;
};

struct QPARAM_INSERT_STORE_EXTENSION
{
	UID64_t			ItemUID;
	UID64_t			Gesture;
};
// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지

#ifdef S_IP_UNIQUE_CONNECTION
struct QPARAM_GET_CHARACTER_MULTIPLE_IP_STATUS
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			AdminIssuerCharacterUID;
};
#endif // S_IP_UNIQUE_CONNECTION

struct QPARAM_NATION_RESET
{
	UID64_t			ItemUID;
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	BYTE			Nation;
};


class CInflWarManager;
class CInflWarData;
class CGiveItemManager;
class CGuildItemManager;
class CArenaManager;
class CPCBangIPManager;
class COutPostManager;
class CCityLeader;
class CAtumFieldDBManager : public CAtumDBManager
{
public:
	CAtumFieldDBManager(CFieldIOCP *pFieldIOCP);

	void MakeAndEnqueueQuery(EnumQueryType type, CFieldIOCPSocket* pFieldIOCPSocket, UID32_t i_uidAccountUID, void *pMsg, void* i_pGeneralParam = NULL, INT64 i_nGeneralParam1 = 0, INT64 i_nGeneralParam2 = 0);	// Asynchronous Excution

	BOOL CheckClientState(CIOCPSocket *i_pSocket);	// DB Query Processing 이전에 클라이언트의 상태가 바른지를 확인!
	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog 보완

	static BOOL LoadInfluenceWarData(BYTE *o_pOwnerInflOfConflictArea, CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager);
	static BOOL InsertInfluenceWarData(CODBCStatement *i_pODBCStmt, SINFLUENCE_WAR_DATA *i_pSINFLUENCE_WAR_DATA);
	static BOOL LoadDeclarationOfWarInfo(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager);			// 2009-01-12 by dhjin, 선전 포고
	
	// 2007-04-25 by dhjin, ARENA 관련
	static BOOL DBLoadArenaInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager);
	static BOOL DBLoadArenaMapInfo(CODBCStatement *i_pODBCStmt, CArenaManager *i_pArenaManager);

	// 2007-07-06 by dhjin, Tutorial
	static BOOL DBLoadTutorialInfo(CODBCStatement *i_pODBCStmt, vectSTutorialInfo *i_pvectTutorialInfo);			// 2007-07-06 by dhjin, Tutorial 정보 얻어오기

	static BOOL DBLoadInitializedGuildList(CODBCStatement *i_pODBCStmt);	// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
	
	// 2007-08-16 by dhjin, 전진기지 관련
	static BOOL DBLoadOutPostInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager);					// 2007-08-16 by dhjin, 전진기지 정보 얻어오기
	static BOOL DBLoadOutPostNextWarTimeInfo(CODBCStatement *i_pODBCStmt, COutPostManager *i_pOutPostManager);					// 2007-08-21 by dhjin, 전진기지 전쟁 시간 정보 얻어오기
	
	// 2007-08-21 by dhjin, CityLeader 관련
	static BOOL DBLoadCityLeaderInfo(CODBCStatement *i_pODBCStmt, CCityLeader *i_pCityLeader);		// 2007-08-21 by dhjin, CityLeader 정보 얻어오기

	// 2008-11-10 by dhjin, 럭키머신
	static BOOL DBLoadLuckyMachine(CODBCStatement *i_pODBCStmt, vectSLUCKY_MACHINE * o_pvectLuckyMachine);
	
	// 2007-10-29 by dhjin, 레벨에 따른 여러 행동들
	static BOOL DBLoadActionByLevel(CODBCStatement *i_pODBCStmt, vectSACTION_BY_LEVEL_DB * i_pvectSACTION_BY_LEVEL_DB);
	
	// 2007-10-29 by dhjin, 선거 일정
	static BOOL	DBLoadPollDate(CODBCStatement *i_pODBCStmt, CInflWarManager *i_pInflWarManager);

	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	static BOOL DBLoadDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *io_pDBServerGroup);
	static BOOL DBUpdateDBServerGroup(CODBCStatement *i_pODBCStmt, SDBSERVER_GROUP *i_pDBServerGroup);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	
	static BOOL DBLoadCinemaInfo(CODBCStatement *i_pODBCStmt, vectorCinemaInfo * o_pvectCinemaInfo);	// 2009-09-09 ~ 2010-02-26 by dhjin, 인피니티 - *.cfg파일 추가
	static BOOL DBLoadRevisionInfo(CODBCStatement *i_pODBCStmt, vectorRevisionInfo * o_pvectRevisionInfo);
	static BOOL DBLoadInfinityMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinityMonsterInfo * o_pvectInfinityMonsterInfo);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	virtual BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

	BOOL DQP_DailyJob(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	constexpr static bool DQP_UpdatePCBangList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }		// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - 
	constexpr static bool DQP_ReloadWRKServiceList(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	constexpr static bool DQP_ReloadWRKLevel(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	constexpr static bool DQP_ReloadWRKFame(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	constexpr static bool DQP_ReloadWRKPVP(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) { return false; }		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	BOOL DQP_LoadInfluenceRate(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);					// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	static BOOL DBLoad_TenderList(CODBCStatement *i_pODBCStmt, vectorTenderInfo *o_pTenderItemList);

	///////////////////////////////////////////////////////////////////////////////
	// // 2010-05-13 by shcho, 인피니티 난이도 조절 -
	static BOOL DBLoadInfinityDifficultInfo(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Bonus* o_vectorDifficultyInfo);
	static BOOL DBLoadInfinityDifficultMonsterInfo(CODBCStatement *i_pODBCStmt, vectorInfinity_DifficultyInfo_Monster* o_vectorDifficultyMonsterInfo);


private:

	BOOL GetAttachedItems( CHARACTER_RENDER_INFO *io_pRenderInfo, SQLHSTMT i_hstmt, UID32_t i_characterUID );
	// 2010-06-15 by shcho&hslee 펫시스템 - 등록된 아이템 정보 전송 함수 추가 		
	void SendItemData( CFieldIOCPSocket *pFieldIOCPSocket, BYTE ItemInsertionType, ITEM_GENERAL	*pItem, tPET_CURRENTINFO *pcurrentData = NULL );
	
	///////////////////////////////////////////////////////////////////////////
	// Query Processing Functions
	///////////////////////////////////////////////////////////////////////////
	void QP_CreateCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetAccountUniqueNumber(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetCharacterInfoByName(FIELD_DB_QUERY q, SQLHSTMT hstmt);
// 2005-07-21 by cmkwon, 다른 필드서버로의 GameStart는 없으므로 삭제함
//	void QP_ConnectGameStart(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetAllCharacterInfoByID(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_SaveCharacterCriticalData(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeUnitKind(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeStat(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeBonusStatPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeGuild(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeExp(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeBodyCondition(FIELD_DB_QUERY q, SQLHSTMT hstmt);
//	void QP_ChangePropensity(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void QP_ChangeInfluenceType(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeStatus(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangePKPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);
// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 사용하지 않으므로 제거
//	void QP_ChangeRacingPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateTotalPlayTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateLastStartedTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeHPDPSPEP(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeCurrentHPDPSPEP(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeMapChannel(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangePetInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangePosition(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// store(item) table 생성 후에 관리함
	//void QP_ChangeDockingItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// For Event
// 2004-12-16 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//	void QP_EventWarpConnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// For Item
	void QP_GetStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_NewGetStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemPossess(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemStorage(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertStoreItemSkill(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStoreItemSkillUseTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2006-11-17 by dhjin
	void QP_UpdateItemCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2006-06-14 by cmkwon
	void QP_UpdateEndurance(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_StoreUpdateColorCode(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateShapeItemNum(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 

	void QP_UpdateItemUsingTimeStamp(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateItemRareFix(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertDefaultItems(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ChangeItemPosition(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateWindowItemList(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ReloadAllEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadOneItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertLoginItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
	void QP_CheckEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertEventItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateEventItemFixedPeriod(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2013-02-28 by bckim, 복귀유져 버프추가
	void QP_CheckCouponEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
	void QP_GetGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetLogGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_AllDeleteGuildStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_Insert2WarpableUserList(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 입장허가 추가
	void QP_DeleteWarpableUser(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 입장허가 삭제
	//void QP_UPDATE_ConnectingServerGroupID(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - 추가 함
	void QP_GiveStoreItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	void QP_GetLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-04-24 by dhjin, EP3 편지 시스템 - DB에서 편지 가져오기
	void QP_ReadLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-04-24 by dhjin, EP3 편지 시스템 - 편지 읽기
	void QP_DeleteLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-04-24 by dhjin, EP3 편지 시스템 - 편지 삭제
	void QP_SendLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 보내기
	void QP_GetAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 편지 시스템 - DB에서 전체 편지 가져오기
	void QP_SendAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 보내기
	void QP_ReadAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기
	void QP_DeleteAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 삭제
	void QP_DeleteOldAllLetter(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-09 by dhjin, EP3 편지 시스템 - 오래된 전체 편지 삭제

	// For Enchant
	void QP_InsertEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteAllEnchant(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// For Quest
// 2007-12-21 by dhjin, 게임 시작 루틴 DB값 설정과 Client전송 부분 분리 - 밑에 함수로 변경
//	void QP_GetAllQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 사용자의 진행중이거나 완료된 모든 quest를 loading
	void GetAllQuest(CFieldIOCPSocket *pIOCPSocket, SQLHSTMT hstmt);		// 사용자의 진행중이거나 완료된 모든 quest를 loading
	void QP_InsertQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 사용자의 quest를 삽입
	void QP_UpdateQuestState(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// Quest 진행 상태 업데이트
	void QP_DeleteQuest(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// Quest 지우기
	void QP_UpdateQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 몬스터 카운트 추가 및 수정
	void QP_DeleteQuestMonsterCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 몬스터 카운트 지우기

	// For Auction
	void QP_AuctionGetItemList(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 경매 아이템 목록 요청
	void QP_AuctionRegisterItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 경매 아이템 등록
	void QP_AuctionCancelRegister(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 경매 아이템 등록 취소
	void QP_AuctionBuyItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 경매 아이템 구매

	// For Trade
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 사용하지 않는 함수
	//void QP_ExecuteTrade(FIELD_DB_QUERY q, SQLHSTMT hstmt_mc);

	void QP_TradeMoveItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 사용하지 않는 함수
	//BOOL MoveTradeItemDB(CFieldIOCPSocket *pFromSocket, CFieldIOCPSocket *pToSocket, SQLHSTMT hstmt_mc, QPARAM_EXECUTETRADE *&pParam, FIELD_DB_QUERY q);
	//BOOL MoveTradeItemField(CFieldIOCPSocket *pFromSocket, CFieldIOCPSocket *pToSocket, QPARAM_EXECUTETRADE *&pParam);

	// General Execution
	void QP_GeneralExec(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 유료화 프리미엄 카드
	void QP_CashInsertPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_CashUpdatePremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_CashDeletePremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// Happy Hour Evnet
	void QP_LoadHappyHourEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateHappyHourEventDate(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateInfluenceWarData(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateOwnerOfConflictArea(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateSubleader(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2007-02-13 by dhjin, 부지도자

	//void QP_JamboreeInit(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2007-04-10 by cmkwon

	// 2006-08-31 by dhjin,  ItemEvent
	void QP_LoadItemEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	//void QP_UpdatePCBangList(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-01-22 by dhjin, PCBang

	// 2006-09-04 by dhjin, 퀵 슬롯
	void QP_Get_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_Delete_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_Update_QuickSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2007-02-28 by dhjin, 전략포인트 한 주기에 랜덤 생성
	void QP_LoadStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfoBySummon(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfoBySummonTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfoByNewSummonRange(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadStrategyPointSummonRange(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadStrategyPointNotSummonTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_LoadStrategyPointNotSummonTimeByAdminTool(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	//////////////////////////////////////////////////////////////////////////
	// 2007-06-07 by dhjin, 아레나 관련 
	void QP_UpdateWarPoint(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-04-25 by dhjin, WarPoint관련
	void QP_UpdateArenaResult(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-06-07 by dhjin, 아레나 결과 업데이트
	void QP_UpdateArenaDisConnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-06-07 by dhjin, 아레나 강제종료 업데이트
	void QP_MF_Updata_CharacterArena(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트
	

	//////////////////////////////////////////////////////////////////////////
	// 2007-07-06 by dhjin, Tutorial
	void QP_LoadTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-07-06 by dhjin, 유저의 Tutorial 정보 얻어오기
	void QP_InsertTutorialComplete(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-07-06 by dhjin, 유저의 Tutorial 완료 정보 저장

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-21 by dhjin, OutPost
	void QP_SetNPCPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-21 by dhjin, 전진기지 NPC소유로 변경
	void QP_SetGuildPossessionOutPost(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-21 by dhjin, 전진기지 여단 소유로 변경
	void QP_SetOutPostNextWarTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-21 by dhjin, 다음 전진기지전 시간 변경
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-08-22 by dhjin, CityLeader
	void QP_InitExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, 판공비 정보 추가
	void QP_SetExpediencyFund(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, 판공비 정보 설정
	void QP_SetExpediencyFundPayBack(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-08-22 by dhjin, 판공비 환급 설정
	void QP_SetExpediencyFundRate(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2007-08-22 by dhjin, 판공비율 정보 설정
	void QP_DeleteCityLeaderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, 도시 정보 삭제
	void QP_RegNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, 공지사항 등록
	void QP_ModifyNotice(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-08-22 by dhjin, 공지사항 수정.삭제
	void QP_UpdateSecondaryPassword(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 2차패스워드 업데이트 프로시저 호출 함수

	//////////////////////////////////////////////////////////////////////////
	// 2007-10-29 by dhjin, poll
	void QP_LoadVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-29 by dhjin, 투표자 리스트 로딩
	void QP_LoadLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-29 by dhjin, 지도자 후보 리스트 로딩
	void QP_SelectLeaderCandidateInfoByRealTimeVariable(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-30 by dhjin, 지도자 후보 실시간으로 변하는 정보 DB에서 가져와 유저에게 전송한다.
	void QP_InsertLeaderCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-30 by dhjin, 지도자 후보 등록
	void QP_UpdateLeaderDeleteCandidate(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-30 by dhjin, 지도자 후보 탈퇴
	void QP_UpdateLeaderPollCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2007-10-31 by dhjin, 투표자 획득 표 업데이트
	void QP_InsertVoterList(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-10-31 by dhjin, 투표자 등록 업데이트

	void QP_CheckGiveTarget(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-13 by cmkwon, 선물하기 기능 추가 -
	void QP_UpdatePilotFace(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-21 by cmkwon, PilotFace 변경 카드 구현 - 

	void QP_InsertNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-28 by cmkwon, 통지시스템 구현 -
	void QP_GetNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-28 by cmkwon, 통지시스템 구현 -
	void QP_DeleteNotifyMsg(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-11-28 by cmkwon, 통지시스템 구현 -

	void QP_GetGuildMark(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2007-12-07 by dhjin, 여단 마크 얻어오기

	//////////////////////////////////////////////////////////////////////////
	// 2007-12-28 by dhjin, 아레나 통합 - 
	void QP_ArenaUpdateCharacterInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ArenaGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ArenaCopyDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_ArenaStartGetCharacter(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	//////////////////////////////////////////////////////////////////////////
	// 2008-04-02 by dhjin,	모선전, 거점전 정보창 기획안 -
	void QP_GetLogMSWarInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_GetLogSPWarInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	void QP_GetWarVictoryDiff(FIELD_DB_QUERY q, SQLHSTMT hstmt);
#endif
	void QP_UpdateMSWarOptionType(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertMSWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
	void QP_InsertSPWarLog(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	void QP_UpdateDBServerGroup(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_CheckConnectableAccount(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void QP_GetUserInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2008-06-23 by dhjin, EP3 유저정보옵션 - 다른 유저 정보 요청

	//////////////////////////////////////////////////////////////////////////
	// 2008-08-19 by dhjin, MySQL포팅 문제로 MySQL에서 지원하지 않는 MSSQL에 Job관련 작업을 여기서 처리한다.
	void QP_DailyJob(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2008-11-04 by dhjin, 럭키머신
	void QP_UpdateLuckyItemDropCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateLuckyItemStarttime(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	// 2009-01-12 by dhjin, 선전 포고
	void QP_UpdateStartDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateEndDeclarationOfWar(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateMSWarStartTime(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void QP_UpdateNickName(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	void QP_GetSelfRanking(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

	void QP_ChangeItemWithItemMatching(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

	void QP_ChangeStartCityMapIndex(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	void QP_ChangeAddedInventoryCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	void QP_LoadInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	인피 귀속 정보 가져오기
	void QP_InsertInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	인피 귀속 정보 추가
	void QP_UpdateInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	인피 완료
	void QP_ResetInfinityImpute(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 	해당 인피 리셋으로 정보 삭제
	void QP_ArenaCopyInfinityDBInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 아레나DB에 복사하기
	void QP_CharacterSaveDataInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 케릭터 정보 수정
	void QP_InfinityFinUpdateItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 정보 수정
	void QP_InfinityFinInsertItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 추가
	void QP_InfinityInsertLog(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 로그
	void QP_CharacterSaveDataInfinityFinByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 케릭터 정보 수정
	void QP_InfinityFinUpdateItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 정보 수정
	void QP_InfinityFinInsertItemByDisconnect(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 추가ㅣ
	void QP_ResetInfinityImputeByServerStart(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 서버 시작시 인피 리셋
	void QP_InfinityComBackPostWork(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2012-01-16 by hskim, 통계 - 화패

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
//	void QP_LoadTenderInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender

	void QP_CashLoadPremiumCard(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 프리미엄 정보 가져오기
	void QP_LoadInfinityShopInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	void QP_UpdateItemCoolingTimeStamp(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	void QP_InfinityUpdateUserMapInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
	void QP_LoadBurningMap(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2010-08-05 by dhjin, 버닝맵 -
	void QP_Log_UserGetTenderItem(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장	

	/////////////////////////////////////////////////////////////
	// start 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현

	void QP_PetUpdateInfinityFin(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_PetSetName(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetExpRatio(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_PetSetLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetExp(FIELD_DB_QUERY q, SQLHSTMT hstmt);

	void OP_PetSetSocket(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetKitSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetSetAutoSkillSlot(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void OP_PetChangeSocketOwner(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
	void OP_StoreGetItemOne(FIELD_DB_QUERY q, SQLHSTMT hstmt);					// 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

	// end 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
	/////////////////////////////////////////////////////////////

	void OP_UpgradPet(FIELD_DB_QUERY q, SQLHSTMT hstmt);						// 2015-06-22 Future, upgrading of Pets with sockets

	/////////////////////////////////////////////////////////////
	// 2012-04-12 by jhseol, 아레나 추가개발 - 아레나 플레이 카운트 확인 함수 실행 
	void QP_ArenaPlayCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-04-12 by jhseol, 아레나 추가개발 - 아레나 플레이 카운트 확인 함수 실행 

	// start 2012-08-27 by hskim, 베트남 인첸트 핵 관련 추가 보안 처리 (코드상의 문제점은 발견되지 않았으나 만일의 경우에는 로그를 남긴다) (동기화 DB 접속을 시도해서 성능 저하가 있음 : 필요없다고 판단되면 삭제 진행)
	void QP_GetEnchantCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-08-27 by hskim, 베트남 인첸트 핵 관련 추가 보안 처리 (코드상의 문제점은 발견되지 않았으나 만일의 경우에는 로그를 남긴다) (동기화 DB 접속을 시도해서 성능 저하가 있음 : 필요없다고 판단되면 삭제 진행)

#if S_BONUSEXPSYSTEM_RENEWAL
	// 2012-10-07 by hskim, 휴식 경험치
	void QP_GetAddExpRestExperienceCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_SetAddExpRestExperienceCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-10-07 by hskim, 휴식 경험치
#endif // S_BONUS_KILL_SYSTEM_RENEWAL

//#if S_BONUS_KILL_SYSTEM_RENEWAL
//	// 2015-06-11 Future, Kill Count System
//	void QP_GetAddKillRestKillCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
//	void QP_SetAddKillRestKillCount(FIELD_DB_QUERY q, SQLHSTMT hstmt);
//	// end 2015-06-11 Future, Kill Count System
//#endif // S_BONUS_KILL_SYSTEM_RENEWAL

	// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
	void QP_GetFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_InsertFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void QP_DeleteFixedTerm(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

	void CAtumFieldDBManager::QP_GetShapeStatLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-05-31 by jhseol,bckim 아머 컬렉션 - QP_GetShapeStatLevel 추가
	void CAtumFieldDBManager::QP_InsertShapeStatLevel(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - QP_InsertShapeStatLevel 추가

	void QP_ArenaCharacterReset(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.

	/////////////////////////////////////////////////////////////
	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
	void QP_LoadRenewalStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	void m_functionLoadRenewalStrategyPointSummonTimeByDB(SQLHSTMT hstmt);
	void m_functionLoadRenewalStrategyPointSummonMapInfoByDB(SQLHSTMT hstmt);
	void m_functionLoadRenewalStrategyPointSummonSettingInfoByDB(SQLHSTMT hstmt);
	void QP_UpdateStrategyPointSummonInfo(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

	// 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.
	void QP_ArenaCharacterReload(FIELD_DB_QUERY q, SQLHSTMT hstmt);
	BOOL m_functionArenaCharacterLoad(CFieldIOCPSocket *i_pFISock, SQLHSTMT hstmt);	// 아레나 서버에서 케릭터 정보를 읽어오는 함수. 다중 호출되는 기능이기에 함수로 빼놓음
	// end 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.


	void QP_LoadCashBuyDate(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
	void QP_InsertCashBuyDate(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

	void QP_LoadMonthlyArmorEvent(FIELD_DB_QUERY q, SQLHSTMT hstmt);	// 2013-04-18 by jhseol,bckim 이달의 아머 - 이달의 아머 이벤트 로드
	void QP_UpdateInfluenceConsecutiveVictorites(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-05-09 by hskim, 세력 포인트 개선

	void QP_GetTemporarySystemInfomation(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	void QP_GetStoreExtension(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
	void QP_InsertStoreExtension(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지

	void QP_CollectionArmorListLoad(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 아머 컬렉션 로드
	void QP_CollectionArmorUpdate(FIELD_DB_QUERY q, SQLHSTMT hstmt);			// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 아머 컬렉션 저장

	void QP_AccountInflChange(FIELD_DB_QUERY q, SQLHSTMT hstmt);				// 2013-07-26 by jhseol, 타 계정 세력변경

#ifdef S_IP_UNIQUE_CONNECTION
	void QP_GetCharacterMultipleIPStatus(FIELD_DB_QUERY q, SQLHSTMT hstmt);		// 2015-11-24 Future, Multiple IP Restriction
#endif // S_IP_UNIQUE_CONNECTION

	void QP_NationTransfer(FIELD_DB_QUERY dbquery, SQLHSTMT hstmt);				// 2015-12-17 Future, Nation Change Card

	void ProcessLogMessagesField(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, char *logstring, int ConnInd, CFieldIOCPSocket *pIOCPSocket, FIELD_DB_QUERY q);
public:
	CFieldIOCP	*m_pFieldIOCP8;
};

#endif
