#pragma once

/******************************************************************************
ATUM C/S Protocol 정의

* protocol header ( size of data + 암호화 정보 + seq. #), 무조건 4 bytes
----------------------------------------------------
| size(2B) | encode_flag(1bit)+N(7bit)| seq. # (1B)|
----------------------------------------------------

* 암호화된 데이터인 경우 (자세한 사항은 이 파일 아래 부분 참조)
<------------------- 4B --------------->
-----------------------------------------------------------------------
| size | encode_flag +   N    | seq. # | MSGs ... | dummy  | checksum |
| (2B) |   (1bit)    + (7bit) |  (1B)  |          | (0~3B) |   (1B)   |
-----------------------------------------------------------------------
<- XOR -><----------- XOR -------------->

* 암호화되지 않은 데이터인 경우
---------------------------------------------------
| size | encode_flag +   N    | seq. # | MSGs ... |
| (2B) |   (1bit)    + (7bit) |  (1B)  |          |
---------------------------------------------------
<----------- 0x0000 ------------>

* message header
-------------------------
| Type1(1B) | Type2(1B) |
-------------------------

작성자: 이윤원(ywlee@webcallworld.com)
작성일: 2002. 9. 23.

******************************************************************************/

#include "DefineGlobal.h"
#include "SocketHeader.h"
#include "AtumParam.h"
#include "ThreadCheck.h"

#if defined(_ATUM_CLIENT)
#include "HShield.h"
#else
#include "Security/AntiCpXSvr.h"
#endif

#include "MessageTypes.h"

constexpr auto PRE_SERVER_PORT = 15100;
constexpr auto IM_SERVER_PORT = 15101;
constexpr auto LOG_SERVER_PORT = 15102;
constexpr auto FIELD_SERVER_PORT = 15103;
constexpr auto NPC_SERVER_PORT = 15104;

const char* GetProtocolTypeString(MessageType_t msgType);
const char* GetGameLogTypeString(T1<T0::FL_LOG> msgType);
const char* GetItemUpdateTypeString(BYTE i_byItemUpdateTy);				// 2006-10-27 by cmkwon
const char* GetItemDeleteTypeString(BYTE i_byItemDeleteTy);				// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - GetItemDeleteTypeString() 추가

void PrintExchangeMsg(BYTE SendOrRecv, MessageType_t nType, char *peerIP, ENServerType st = ST_INVALID_TYPE, BYTE printLevel = PRINTLEVEL_NO_MSG);

struct MSG_FP_CASH_CHANGE_CHARACTERNAME
{
	char		szAccName[SIZE_MAX_ACCOUNT_NAME];
	char		szChangedCharName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FP_ADMIN_BLOCKACCOUNT		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
{
	SBLOCKED_ACCOUNT_INFO blockAccInfo;
};

struct MSG_FP_ADMIN_BLOCKACCOUNT_OK		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
{
	Err_t	ErrCode;		// ERR_NO_ERROR 이면 성공
	INT		AdminFieldServerClientIndex;		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - PreServer<->FieldServer 에서만 사용함
	char	AdminAccName[SIZE_MAX_ACCOUNT_NAME];
	char	BlockedAccName[SIZE_MAX_ACCOUNT_NAME];
	ATUM_DATE_TIME	atimeEndTime;				// 블럭 종료 시간
};

struct MSG_FP_ADMIN_UNBLOCKACCOUNT		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
{
	SBLOCKED_ACCOUNT_INFO blockAccInfo;
};

struct MSG_FP_ADMIN_UNBLOCKACCOUNT_OK	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
{
	Err_t	ErrCode;		// ERR_NO_ERROR 이면 성공
	INT		AdminFieldServerClientIndex;		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - PreServer<->FieldServer 에서만 사용함
	char	AdminAccName[SIZE_MAX_ACCOUNT_NAME];
	char	UnblockedAccName[SIZE_MAX_ACCOUNT_NAME];
};

struct MSG_FC_CITYWAR_START_WAR
{
	MapIndex_t			CityWarMapIndex3;
	UID32_t				OccupyGuildUID3;
	char				szOccupyGuildName3[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME		atimeCityWarEndTime;
};

struct MSG_FC_CITYWAR_MONSTER_DEAD
{
	MapIndex_t			CityWarMapIndex3;
	UID32_t				OccupyGuildUID3;
	char				szOccupyGuildName3[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_CITYWAR_END_WAR
{
	MapIndex_t			CityWarMapIndex3;
	UID32_t				OccupyGuildUID3;
	char				szOccupyGuildName3[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME		atimeNextCityWarDefaultTime;
};

struct MSG_FC_CITYWAR_GET_OCCUPYINFO_OK
{
	MapIndex_t			CurrentMapIndex3;			// 정보 MapIndex 
	MapIndex_t			CityWarMapIndex3;			// 도시점령전 MapIndex
	MapIndex_t			CityWarCityMapIndex3;		// 도시점령전 도시 MapIndex
	SCITY_OCCUPY_INFO	CityWarOccupyInfo3;			// 도시점령전 점령 정보
};

struct MSG_FC_CITYWAR_SET_SETTINGTIME
{
	ATUM_DATE_TIME		atimeSetCityWarSettingTime;
};

struct MSG_FC_CITYWAR_SET_TEX
{
	float				fSetTex;
};

struct MSG_FC_CITYWAR_SET_BRIEFING
{
	char				szSetBriefing[SIZE_MAX_CITY_BRIEFING_LENGTH];
};

struct MSG_FC_CITYWAR_BRING_SUMOFTEX_OK
{
	int					nBroughtSumOfTex;
};

struct MSG_FC_WAR_NOTIFY_INVASION					// 2005-12-27 by cmkwon
{
	MAP_CHANNEL_INDEX	MapChannelIndex0;		// 타 세력이 침입한 MapChannelIndex
};

struct MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD		// 2005-12-27 by cmkwon
{
	INT					MonsterUnitkind;		// 죽은 몬스터
	UID32_t				uidBestGuildUID;						// 2007-08-23 by cmkwon, 모선 폭파시 최고 데미지 준 여단명 표시 - GuildUID
	char				szBestGuildName[SIZE_MAX_GUILD_NAME];	// 2007-08-23 by cmkwon, 모선 폭파시 최고 데미지 준 여단명 표시 - GuildName 
};

struct MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION		// 2006-01-20 by cmkwon
{
	INT					MonsterUnitkind;		// 공격 받는 몬스터
};

struct MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA				// 2006-04-14 by cmkwon
{
	INT					SummonMonsterUnitkind;		// 소환 될 MonsterUnitKind
	INT					RemainMinute;				// 몬스터가 소환되기 위해 남은 시간(단위:분)
	INT					ContributionPoint;			// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	ATUM_DATE_TIME		SummonMonsterTime;			// 2007-02-06 by dhjin, 보스(전함) 몬스터 소환된 시간 
	BYTE				BossStep;					// 2009-03-10 by dhjin, 단계별 모선 시스템 - 모선 단계
	BYTE				BeforeWinCheck;				// 2009-03-10 by dhjin, 단계별 모선 시스템 - 전 단계 승패 여부
};

struct MSG_FC_WAR_JACO_MONSTER_SUMMON				// 2006-04-19 by cmkwon
{
	int		nBelligerence0;
};

struct MSG_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON
{// 2007-07-18 by dhjin,
	MapIndex_t		MapIndex;
	BYTE			InfluenceType;
	CHAR			MapName[SIZE_MAX_MAP_NAME];
	ATUM_DATE_TIME	SummonStrategyPointTime;
	FLOAT			HPRate;
};

struct MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST
{
	INT				StrategicPointInfoDisplayListCount;
	_ARRAY(SSTRATEGYPOINT_DISPLAY_INFO);
};

struct MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY : public SSTRATEGYPOINT_DISPLAY_INFO { };

struct MSG_SIGN_BOARD_STRING
{
	BYTE			InfluenceMask0;									// 2006-04-17 by cmkwon, 세력 마스크
	bool			IsInfluenceLeader;								// 2006-04-17 by cmkwon, TRUE:세력지도자, FALSE:운영자 혹은 관리자
	INT				StringIndex;									// 전광판 스트링 인덱스
	ATUM_DATE_TIME	SignBoardExprieATime;							// 전광판 스트링 종료 시간
	char			SingBoardString[SIZE_MAX_SIGN_BOARD_STRING];	// 전광판 스트링
};

struct MSG_FC_WAR_SIGN_BOARD_INSERT_STRING				// 2006-04-17 by cmkwon
{
	bool			IsInfluenceLeader;								// 2006-04-17 by cmkwon, TRUE:세력지도자, FALSE:운영자 혹은 관리자
	INT				StringIndex;									// 전광판 스트링 인덱스
	ATUM_DATE_TIME	SignBoardExpiryTime;							// 전광판 스트링 종료 시간
	char			SingBoardString[SIZE_MAX_SIGN_BOARD_STRING];	// 전광판 스트링
};

struct MSG_FC_WAR_SIGN_BOARD_DELETE_STRING				// 2006-04-18 by cmkwon
{
	INT				DeleteStringIndex;						// 전광판 스트링 인덱스
};

struct MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST				// 2006-04-17 by cmkwon
{
	INT		nReqStringCount;									// 전광판 스트링 개수
};

struct MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK				// 2006-04-17 by cmkwon
{
	INT		nStringCount;									// 전광판 스트링 개수
	ARRAY_(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING);			// 전광판 내용
};

struct MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK				// 2006-04-19 by cmkwon
{
	BYTE	byInfluenceType;
	int		nContributionPoint;
};

struct MSG_FC_WAR_INFLUENCE_DATA							// 2006-04-21 by cmkwon
{
	BYTE	byInfluenceType;
	float	fHPRepairRate;
	float	fDPRepairRate;
	float	fSPRepairRate;
};

struct MSG_FC_WAR_MONSTER_CREATED			// 2006-11-20 by cmkwon
{
	INT					MonsterUnitKind;
	MAP_CHANNEL_INDEX	MapChannIdx;
	ATUM_DATE_TIME		CreateTime;			// 2007-07-16 by dhjin, 생성 시간 추가
};

struct MSG_FC_WAR_MONSTER_AUTO_DESTROYED	// 2006-11-20 by cmkwon
{
	INT					MonsterUnitKind;
	MAP_CHANNEL_INDEX	MapChannIdx;
};

struct MSG_FC_WAR_MONSTER_DEAD				// 2006-11-20 by cmkwon
{
	INT					MonsterUnitKind;
	MAP_CHANNEL_INDEX	MapChannIdx;
};

struct SCONTRIBUTION_GUILD_INFO
{
	BYTE		order;
	UID32_t		GuildUID;
	char		GuildName[SIZE_MAX_GUILD_NAME];
	INT			GuildPay;
};

struct MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD
{// 2008-12-29 by dhjin, 전쟁 보상 추가안
	SCONTRIBUTION_GUILD_INFO   ContributionGuldInfo[3];
};

struct MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES
{
	INT PointBCU;
	INT PointANI;
};

struct MSG_FC_WAR_INFLUENCE_CONSECUTIVE_POINT
{
	float PVPBuffPercent;
};

struct MSG_FC_BAZAAR_CUSTOMER_INFO_OK
{
	ClientIndex_t	clientIndex0;
};

struct MSG_FC_BAZAAR_INFO_OK
{
	ClientIndex_t	clientIndex0;
	BYTE			byBazaarType;
	char			szBazaarName[SIZE_MAX_BAZAAR_NAME];
};

struct MSG_FC_BAZAAR_SELL_PUT_ITEM
{
	UID64_t			itemUID;
	int				nAmount;
	int				nEachPrice;
};


struct MSG_FC_BAZAAR_SELL_PUT_ITEM_OK
{
	UID64_t			itemUID;
};

struct MSG_FC_BAZAAR_SELL_CANCEL_ITEM
{
	UID64_t			itemUID;
};

struct MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK
{
	UID64_t			itemUID;
};

struct MSG_FC_BAZAAR_SELL_START
{
	char			szBazaarName[SIZE_MAX_BAZAAR_NAME];
};

struct MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST
{
	ClientIndex_t	clientIndex0;
};

struct SBAZAAR_SELL_ITEM
{
	int				nSellItemNum0;
	int				nSellAmount0;
	int				nSellEachPrice0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	INT				PrefixCodeNum0;				// 접두사, 없으면 0
	INT				SuffixCodeNum0;				// 접미사, 없으면 0
												// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
												//	INT				ColorCode0;					// 튜닝시 아머의 ColorCode
	INT				ShapeItemNum0;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	INT				EffectItemNum0;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	INT				CoolingTime;			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	FIXED_TERM_INFO	FixedTermShape;				// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
};

struct MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK
{
	ClientIndex_t	clientIndex0;
	int				nItemListCnts0;
	_ARRAY(SBAZAAR_SELL_ITEM);
};

struct MSG_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK
{
	UID64_t			itemUID;
	int				nEnchatCnts;
	_ARRAY(int EnchantItemNum);
};

struct MSG_FC_BAZAAR_SELL_BUY_ITEM
{
	ClientIndex_t	clientIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FC_BAZAAR_SELL_BUY_ITEM_OK
{
	ClientIndex_t	clientIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FC_BAZAAR_BUY_PUT_ITEM
{
	int				itemNum0;
	int				nAmount;
	int				nEachPrice;
};

struct MSG_FC_BAZAAR_BUY_PUT_ITEM_OK
{
	int				itemNum0;
	int				nBuyItemIndex0;
};

struct MSG_FC_BAZAAR_BUY_CANCEL_ITEM
{
	int				nBuyItemIndex0;
};

struct MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK
{
	int				nBuyItemIndex0;
};

struct MSG_FC_BAZAAR_BUY_START
{
	char			szBazaarName[SIZE_MAX_BAZAAR_NAME];
};

struct MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST
{
	ClientIndex_t	clientIndex0;
};

struct SBAZAAR_BUY_ITEM
{
	int				nBuyItemIndex0;
	int				nBuyItemNum0;
	int				nBuyAmount0;
	int				nBuyEachPrice0;
};
struct MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK
{
	ClientIndex_t	clientIndex0;
	int				nItemListCnts0;
	_ARRAY(SBAZAAR_BUY_ITEM);
};

struct MSG_FC_BAZAAR_BUY_SELL_ITEM
{
	ClientIndex_t	clientIndex0;
	int				nBuyItemIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FC_BAZAAR_BUY_SELL_ITEM_OK
{
	ClientIndex_t	clientIndex0;
	int				nBuyItemIndex0;
	UID64_t			itemUID;					// 2006-07-26 by cmkwon
	int				nAmount0;
};

struct MSG_FI_CASH_USING_GUILD
{
	UID32_t		guildUID;
	int			nIncreaseMemberCapacity;
	UID32_t		CashPrice;						// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
};

struct MSG_FI_CASH_USING_CHANGE_CHARACTERNAME
{
	UID32_t		charUID;
	char		szChangedCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FI_CASH_PREMIUM_CARD_INFO
{// 2006-09-14 by dhjin, 여단 초대시 맴버쉽 정보 필요
	UID32_t			AccountUID;
	INT				nCardItemNum1;
	//	ATUM_DATE_TIME	atumTimeUpdatedTime1;	// 수정된 시간
	ATUM_DATE_TIME	atumTimeExpireTime1;	// 만료 시간		// 2008-06-20 by dhjin, EP3 여단 수정 사항 - 만료 시간 필요
											//	float			fExpRate1;
											//	float			fDropRate1;
											//	float			fDropRareRate1;
											//	float			fExpRepairRate1;
};

struct MSG_FI_GUILD_RANK
{
	UID32_t		charUID;
	BYTE		GuildRank;
};

struct MSG_FN_CITYWAR_START_WAR
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CITYWAR_END_WAR
{
	ChannelIndex_t		ChannelIndex;
	UID32_t				OccupyGuildUID4;
};

struct MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO
{
	ChannelIndex_t		ChannelIndex;
	UID32_t				OccupyGuildUID4;
};


// STRING_128 type
#define STRING_128_DEBUG_L1		0	// 디버그용, level 1
#define STRING_128_DEBUG_L2		1	// 디버그용, level 2
#define STRING_128_DEBUG_L3		2	// 디버그용, level 3
#define STRING_128_ADMIN_CMD	3	// 관리자 명령어용
#define STRING_128_USER_ERR		4	// 사용자에게 주는 에러
#define STRING_128_USER_NOTICE	5	// 사용자에게 주는 알림
#define STRING_128_DEBUG_PRINT	6	// 디버그용, DBGOUT에 출력
#define STRING_128_SYSTEM_NOTICE	7	// 사용자에게 주는 알림		// 2012-03-30 by hskim, EP4 트리거 시스템 이벤트 알림
#define STRING_128_USER_POPUP	8	// 사용자에게 주는 팝업알림		// 2013-05-31 by jhseol,bckim 아머 컬렉션

// SendErrorMessage types
//#define T_PRE_IOCP							(MessageType_t)((T0_PRE<<8)|T1_PRE_IOCP)
//#define T_PRE_DB								(MessageType_t)((T0_PRE<<8)|T1_PRE_DB)
//#define T_IM_IOCP								(MessageType_t)((T0_IM<<8)|T1_IM_IOCP)
//#define T_IM_DB								(MessageType_t)((T0_IM<<8)|T1_IM_DB)
//#define T_FIELD_IOCP							(MessageType_t)((T0_FIELD<<8)|T1_FIELD_IOCP)
//#define T_FIELD_DB							(MessageType_t)((T0_FIELD<<8)|T1_FIELD_DB)
//#define T_NPC_IOCP							(MessageType_t)((T0_NPC<<8)|T1_NPC_IOCP)
//#define T_NPC_DB								(MessageType_t)((T0_NPC<<8)|T1_NPC_DB)
//#define T_TIMER								(MessageType_t)((T0_TIMER<<8)|T1_TIMER)
//#define T_DB									(MessageType_t)((T0_DB<<8)|T1_DB)
//#define T_NA									(MessageType_t)((T0_NA<<8)|T1_NA)		// NOT AVAILABLE PROTOCOL
//#define T_ERROR								(MessageType_t)((T0_ERROR<<8)|T1_ERROR)



constexpr bool IS_ALIVE_MSG(MessageType_t msgtype)
{
	return (HIBYTE(msgtype) >= unsigned(T0::PC_CONNECT) && HIBYTE(msgtype) <= unsigned(T0::NL_CONNECT))
		&& (LOBYTE(msgtype) == 0x03);
}

///////////////////////////////////////////////////////////////////////////////
// Structures for Data Exchange
///////////////////////////////////////////////////////////////////////////////

struct MSG_UNIT_INDEX
{
	ClientIndex_t	ClientIndex;
};

///////////////////////////////
// PC_DEFAULT_UPDATE
struct MSG_PC_DEFAULT_UPDATE_LAUNCHER_VERSION
{
	USHORT	Version[SIZE_MAX_VERSION];
};

struct MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO
{
	USHORT	UpdateVersion[SIZE_MAX_VERSION];
	char	FtpIP[SIZE_MAX_FTP_URL];
	USHORT	FtpPort;
	char	FtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	FtpPassword[SIZE_MAX_PASSWORD];
	char	LauncherFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];		// 2005-12-23 by cmkwon, 수정
};

struct MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION					// 2007-01-08 by cmkwon, C->P
{
	USHORT	Version[SIZE_MAX_VERSION];
};

struct MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO			// 2007-01-06 by cmkwon, 추가함
{
	USHORT	UpdateVersion[SIZE_MAX_VERSION];
	char	UpdateServerIP[SIZE_MAX_FTP_URL];
	USHORT	UpdateServerPort;
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD];
	char	LauncherFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];
};


///////////////////////////////
// PC_CONNECT

struct MSG_PC_CONNECT_VERSION
{
	USHORT	ClientVersion[SIZE_MAX_VERSION];
};

struct MSG_PC_CONNECT_UPDATE_INFO
{
	USHORT	OldVersion[SIZE_MAX_VERSION];		// 업데이트 시작 버전
	USHORT	UpdateVersion[SIZE_MAX_VERSION];	// 업데이트 끝 버전
	char	FtpIP[SIZE_MAX_FTP_URL];
	USHORT	FtpPort;
	char	FtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	FtpPassword[SIZE_MAX_PASSWORD];
	// 2005-12-23 by cmkwon
	//	char	FtpUpdateDir[SIZE_MAX_FTP_FILE_PATH];	// 업데이트할 파일이 있는 경로
	char	FtpUpdateDownloadDir[SIZE_MAX_FTP_FILE_PATH];	// 2005-12-23 by cmkwon, 업데이트할 파일 다운로드 경로
};

struct MSG_PC_CONNECT_REINSTALL_CLIENT
{
	USHORT	LatestVersion[SIZE_MAX_VERSION];		// 최신 버전
};


#define LOGIN_TYPE_DIRECT	((BYTE)0)
#define LOGIN_TYPE_MGAME	((BYTE)1)

struct MSG_PC_CONNECT_LOGIN
{
	BYTE	LoginType;		// LOGIN_TYPE_XXX

							// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 
							//	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	AccountName[SIZE_MAX_ORIGINAL_ACCOUNT_NAME];	// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 

	BYTE	Password[SIZE_MAX_PASSWORD_MD5];
	char	FieldServerGroupName[SIZE_MAX_SERVER_NAME];
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	//INT		MGameSEX;		// 성별 - 모름=0, 남자=1, 여자=2
	//INT		MGameYear;		// 출생년도(ex> 1976, 1981, 2000)
	char	WebLoginAuthKey[SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY];		// 2007-03-29 by cmkwon, 추가함
	char	ClientIP[SIZE_MAX_IPADDRESS];	// 2008-10-08 by cmkwon, 대만 Netpower_Tpe 외부인증 구현 - PreServer에 접속한 client IP
	char	SelectiveShutdownInfo[SIZE_MAX_SELECTIVE_SHUTDOWN_INFO];		// 2012-07-11 by hskim, 선택적 셧다운
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];	// 2016-01-09 Future, MAC Blocking
};

struct MSG_PC_CONNECT_LOGIN_OK
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];		// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 2차 계정을 보내줌

	char	FieldServerIP[SIZE_MAX_IPADDRESS];
	char	IMServerIP[SIZE_MAX_IPADDRESS];
	USHORT	FieldServerPort;
	USHORT	IMServerPort;
	bool	OpeningMoviePlay;		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]
};

struct MSG_PC_CONNECT_CLOSE
{
	INT		reason;
};

struct MSG_PC_CONNECT_SINGLE_FILE_VERSION_CHECK
{
	USHORT	DeleteFileListVersion[SIZE_MAX_VERSION];
	USHORT	NoticeVersion[SIZE_MAX_VERSION];
};		// single file들에 대한 버전 확인(deletefilelist.txt, notice.txt 등)

struct MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO
{					// 2007-01-08 by cmkwon, AU_SERVER_TYPE_XXX
	USHORT	NewDeleteFileListVersion[SIZE_MAX_VERSION];
	USHORT	NewNoticeVersion[SIZE_MAX_VERSION];
	char	FtpIP[SIZE_MAX_FTP_URL];
	USHORT	FtpPort;
	char	FtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	FtpPassword[SIZE_MAX_PASSWORD];
	char	DeleteFileListDownloadPath[SIZE_MAX_FTP_FILE_PATH];
	char	NoticeFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];
};		// single file들에 대한 업데이트 정보(deletefilelist.txt, notice.txt 등)

#ifndef MGAME_MAX_PARAM_STRING_SIZE
#define MGAME_MAX_PARAM_STRING_SIZE			50
#endif

struct MEX_SERVER_GROUP_INFO_FOR_LAUNCHER
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
	int		Crowdedness;		// 혼잡도, 0% ~ 100%
};

struct MSG_PC_CONNECT_GET_SERVER_GROUP_LIST_OK
{
	size_t		NumOfServerGroup;
	ARRAY_(MEX_SERVER_GROUP_INFO_FOR_LAUNCHER);
};	// P->Launcher

struct SGAME_SERVER_GROUP_OLD		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 이전 버전 호환을 위한 구조체
{
	char	szGameServerGroupName[SIZE_MAX_GAME_SERVER_GROUP_NAME];
	char	szPreServerIP0[SIZE_MAX_IPADDRESS];
	USHORT	usPreServerPort0;
};

struct MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK		// 2007-05-02 by cmkwon, PreServer 정보
{// 2007-05-15 by cmkwon, 구조체의 사이즈가 SIZE_MAX_PACKET 보다 작아야 한다.
	SGAME_SERVER_GROUP_OLD arrGameServerGroupList[COUNT_MAX_GAME_SERVER_GROUP_LIST];		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 이전 버전 호환을 위한 구조체 사용
};

struct SGAME_SERVER_GROUP		// 2007-05-02 by cmkwon, PreServer 정보
{
	char	szGameServerGroupName[SIZE_MAX_GAME_SERVER_GROUP_NAME];
	char	szPreServerIP0[SIZE_MAX_IPADDRESS];
	// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - Port는 기본포트를 사용함
	//	USHORT	usPreServerPort0;
	USHORT	usPreServerTab8OrderIndex;			// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 필드추가
};
using vectSGAME_SERVER_GROUP = vector<SGAME_SERVER_GROUP>;			// 2007-05-15 by cmkwon

struct MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 새로 추가한 구조체
{// 2007-05-15 by cmkwon, 구조체의 사이즈가 SIZE_MAX_PACKET 보다 작아야 한다.
	SGAME_SERVER_GROUP arrGameServerGroupList[COUNT_MAX_GAME_SERVER_GROUP_LIST];
};

struct MSG_PC_CONNECT_NETWORK_CHECK			// 2007-06-18 by cmkwon, 네트워크 상태 체크 
{
	int		nCheckCount;
};

using MSG_PC_CONNECT_NETWORK_CHECK_OK = MSG_PC_CONNECT_NETWORK_CHECK;		// 2007-06-18 by cmkwon, 네트워크 상태 체크 


struct MSG_PC_CONNECT_LOGIN_BLOCKED
{
	// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) -
	//	char			szAccountName[SIZE_MAX_ACCOUNT_NAME];				// 2007-01-10 by cmkwon
	char			szAccountName[SIZE_MAX_ORIGINAL_ACCOUNT_NAME];		// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) -

	int				nBlockedType;
	ATUM_DATE_TIME	atimeStart;
	ATUM_DATE_TIME	atimeEnd;
	char			szBlockedReasonForUser[SIZE_MAX_BLOCKED_ACCOUNT_REASON];		// 2007-01-10 by cmkwon
};

///////////////////////////////
// FN_CONNECT

struct MSG_FN_CONNECT_MAP_INFO
{
	MapIndex_t	MapIndex;
};

struct MSG_FN_CONNECT_INCREASE_CHANNEL
{
	MapIndex_t	MapIndex;
	INT			TotalChannelCount;
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};		// F->N

struct MSG_FN_CONNECT_SET_CHANNEL_STATE
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	bool				EnableChannel;	// TRUE: Enable, FALSE: Disable
	DWORD				Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};		// F->N

		///////////////////////////////
		// FC_CONNECT

struct MSG_FC_CONNECT_LOGIN
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];		// 2015-09-23 Future, added transmition of MAC Address
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

// 2005-12-08 by cmkwon
/////////////////////////////////////
//// 아이템 장착 위치(POS_XXX)
//#define POS_PROW							((BYTE)0)	// 레이더(선두 가운데)
//#define POS_PROWIN						((BYTE)1)	// 컴퓨터(중앙 좌측)
//#define POS_PROWOUT						((BYTE)2)	// 1형 무기(선두 좌측)
//#define POS_WINGIN						((BYTE)3)	//		사용안함(중앙 우측)
//#define POS_WINGOUT						((BYTE)4)	// 2형 무기(선두 우측)
//#define POS_CENTER						((BYTE)5)	// 아머(중앙 가운데)
//#define POS_REAR							((BYTE)6)	// 엔진(후미 가운데)
//#define POS_ATTACHMENT					((BYTE)7)	// 2006-03-30 by cmkwon, 무제한 악세사리 - 부착물(후미 우측-연료탱크|컨테이너계열)
//#define POS_PET							((BYTE)8)	// 2006-03-30 by cmkwon, 시간제한 악세사리(후미 좌측)
struct CHARACTER_RENDER_INFO
{
	INT		RI_Prow;			// POS_PROW의 ItemNum, 선두 (라이트계열 + 방어계열 or 레이다)
	INT		RI_ProwIn;			// 2005-03-17 by cmkwon (CPU 컴퓨터)
	INT		RI_ProwOut;			// POS_PROWOUT의 ItemNum, 선두의 바깥쪽, 장전 (1형무기 - 빔계열 or 캐논계열)
	INT		RI_WingIn;			// 2005-03-17 by cmkwon (마크)
	INT		RI_WingOut;			// POS_WINGOUT의 ItemNum, 날개의 바깥쪽, 장전(2형무기 - 로켓계열 or 미사일계열)
	INT		RI_Center;			// POS_CENTER의 ItemNum, 중앙 (방어계열 - 아머) 고려
								// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 필요 없음
								//	INT		RI_ArmorColorCode;	// 2005-12-08 by cmkwon, 아머의 색상칼라
	INT		RI_Rear;			// POS_REAR의 ItemNum, 후미 (엔진계열)

								// 2010-06-15 by shcho&hslee 펫시스템
								//INT		RI_Attachment;		// POS_ATTACHMENT의 ItemNum, 부착물 (컨테이너계열<연료탱크/기타계열> or 지도자의 광휘)
	INT		RI_AccessoryUnLimited;

	// 2010-06-15 by shcho&hslee 펫시스템
	//INT		RI_Pet;				// 2005-03-17 by cmkwon (시간제한 악세사리)
	INT		RI_AccessoryTimeLimit;

	bool	RI_Invisible;		// 2006-11-27 by dhjin, 캐릭터 보이지 않는 플래그
	INT		RI_Prow_ShapeItemNum;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 레이더 ShapeItemNum
	INT		RI_WingIn_ShapeItemNum;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 마크 ShapeItemNum
	INT		RI_Center_ShapeItemNum;			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 아머 ShapeItemNum
	INT		RI_ProwOut_ShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 1형무기 ShapeItemNum
	INT		RI_WingOut_ShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 2형무기 ShapeItemNum
	INT		RI_ProwOut_EffectItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 1형무기 탄두 EffectItemNum
	INT		RI_WingOut_EffectItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 2형무기 탄두 EffectItemNum
	INT		RI_MonsterUnitKind_ForTransformer;	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 변신상태의 몬스터 변신카드

	INT		RI_Pet;							// 2010-06-15 by shcho&hslee 펫시스템 - 펫 아이템.
	INT		RI_Pet_ShapeItemNum;			// 2010-06-15 by shcho&hslee 펫시스템 - ShapeItemNum.

											///////////////////////////////////////////////////////////////////////////////
											/// \fn			
											/// \brief		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
											/// \author		cmkwon
											/// \date		2009-08-27 ~ 2009-08-27
											/// \warning	
											///
											/// \param		
											/// \return		
											///////////////////////////////////////////////////////////////////////////////
	bool SetRenderInfoWithPOS(INT i_nPos, INT i_nItemNum, INT i_nShapeItemNum, INT i_nEffectItemNum)
	{
		switch (i_nPos)
		{

		case POS_PROW:			// 레이더 - 외형변경가능
		{
			RI_Prow = i_nItemNum;
			RI_Prow_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		case POS_PROWIN:		// CPU 컴퓨터 - 
		{
			RI_ProwIn = i_nItemNum;		// 2012-06-20 by isshin CPU 정보 버그 수정
		}
		break;

		case POS_PROWOUT:		// 1형 무기 - 외형변경가능 + 탄두이펙트변경가능
		{
			RI_ProwOut = i_nItemNum;
			RI_ProwOut_ShapeItemNum = i_nShapeItemNum;
			RI_ProwOut_EffectItemNum = i_nEffectItemNum;
		}
		break;

		case POS_WINGIN:		// 마크 - 외형변경가능
		{
			RI_WingIn = i_nItemNum;
			RI_WingIn_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		case POS_WINGOUT:		// 2형 무기 - 외형변경가능 + 탄두이펙트변경가능
		{
			RI_WingOut = i_nItemNum;
			RI_WingOut_ShapeItemNum = i_nShapeItemNum;
			RI_WingOut_EffectItemNum = i_nEffectItemNum;
		}
		break;

		case POS_CENTER:		// 아머 - 외형변경가능
		{
			RI_Center = i_nItemNum;
#ifdef _REWORKED_COLORSHOP
			if (i_nEffectItemNum)
			{
				RI_Center_ShapeItemNum = i_nEffectItemNum;
			}
			else
#endif
				RI_Center_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		case POS_REAR:			// 엔진 - 
		{
			RI_Rear = i_nItemNum;
		}
		break;

		// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
		//case POS_ATTACHMENT:	// 연료탱크 or 지도자의광휘 - 
		case POS_ACCESSORY_UNLIMITED:
		{
			//RI_Attachment	= i_nItemNum;
			RI_AccessoryUnLimited = i_nItemNum;
		}
		break;

		// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
		//case POS_PET:						// 시간제한 악세사리 - 
		case POS_ACCESSORY_TIME_LIMIT:		// 시간제한 악세사리 - 
		{
			//RI_Pet			= i_nItemNum;
			RI_AccessoryTimeLimit = i_nItemNum;
		}
		break;

		// 2010-06-15 by shcho&hslee 펫시스템
		case POS_PET:
		{
			RI_Pet = i_nItemNum;
			RI_Pet_ShapeItemNum = i_nShapeItemNum;
		}
		break;

		default:
		{
			return FALSE;
		}
		}
		return TRUE;
	};
};		// 기체를 그리기 위해 필요한 장착된 아이템 정보

struct FC_CONNECT_LOGIN_INFO
{
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t					CharacterUniqueNumber;
	USHORT					Race;
	USHORT					UnitKind;
	BYTE					PilotFace;
	BYTE					Color;
	int						RacingPoint;		// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
	CHARACTER_RENDER_INFO	CharacterRenderInfo;
	// START 2011-11-03 by shcho, yedang 셧다운제 구현 - 생일정보도 추가로 받아온다.
	bool					ShutDownMINS;
	// END 2011-11-03 by shcho, yedang 셧다운제 구현 - 생일정보도 추가로 받아온다.
	DWORD					Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FC_CONNECT_LOGIN_OK
{
	UID32_t				AccountUniqueNumber;
	BYTE				NumCharacters;
	FC_CONNECT_LOGIN_INFO	Characters[3];
	//char				VoIP1to1ServerIP[SIZE_MAX_IPADDRESS];
	//USHORT				VoIP1to1ServerPort;
	//char				VoIPNtoNServerIP[SIZE_MAX_IPADDRESS];
	//USHORT				VoIPNtoNServerPort;
	BYTE				bIsUseSecondaryPasswordSystem;			// 2007-09-13 by cmkwon, 베트남 2차패스워드 구현 - MSG_FC_CONNECT_LOGIN_OK 에 필드 추가
	BYTE				bIsSetSecondaryPassword;				// 2007-09-13 by cmkwon, 베트남 2차패스워드 구현 - MSG_FC_CONNECT_LOGIN_OK 에 필드 추가
	DWORD				Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
	// todo : remove
	[[deprecated("unused functionality")]]
	bool				BIsTestServer;							// 2012-11-27 by khkim, 테스트 서버인지 일반 서버인지 구별
	BYTE				DBNum;									// 2012-11-28 by jhseol, 게임포지 캐쉬 WEB 상점 - 서버가 사용하는 번호
};

struct MSG_FC_CONNECT_CLOSE
{
	INT		reason;
};

struct MSG_FC_CONNECT_SYNC_TIME {
	UINT	CurrentTime;	// 4시간 기준으로 지난 초
};


struct MSG_FC_CONNECT_NETWORK_CHECK		// 2008-02-15 by cmkwon, Client<->FieldServer 간 네트워크 상태 체크 - 
{
	int		nCheckCount;
	DWORD	dwClientTick;
};
// 2008-10-31 by cmkwon, 네트워크 상태 체크 관련 수정(버퍼개수도 전송) - 아래와 같이 따로 선언함
//typedef MSG_FC_CONNECT_NETWORK_CHECK	MSG_FC_CONNECT_NETWORK_CHECK_OK;	// 2008-02-15 by cmkwon, Client<->FieldServer 간 네트워크 상태 체크 - 
struct MSG_FC_CONNECT_NETWORK_CHECK_OK		// 2008-10-31 by cmkwon, 네트워크 상태 체크 관련 수정(버퍼개수도 전송) - 
{
	int		nCheckCount;
	DWORD	dwClientTick;
	int		nWriteBufferSize;		// 2008-10-31 by cmkwon, 네트워크 상태 체크 관련 수정(버퍼개수도 전송) - 
};

struct MSG_FC_CONNECT_ARENASERVER_INFO
{// 2007-12-28 by dhjin, 아레나 통합 - F -> C 
	CHAR		MainServerName[SIZE_MAX_SERVER_NAME];
	CHAR		ArenaServerName[SIZE_MAX_SERVER_NAME];
	USHORT		AFS_Port;
	USHORT		AIS_Port;
	CHAR		AFS_IP[SIZE_MAX_IPADDRESS];
	CHAR		AIS_IP[SIZE_MAX_IPADDRESS];
};

struct MSG_FC_CONNECT_ARENASERVER_LOGIN
{// 2007-12-28 by dhjin, 아레나 통합 - AF -> C 
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIdx;
	CHAR			MFSName[SIZE_MAX_SERVER_NAME];
};

struct MSG_FC_CONNECT_ARENASERVER_LOGIN_OK
{// 2007-12-28 by dhjin, 아레나 통합 - C -> AF
	CHARACTER		AFSCharacter;
	ATUM_DATE_TIME	atimeCurServerTime;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, 현재 아레나 서버 날짜 시간
};

// 2008-02-25 by dhjin, 아레나 통합 - AF -> C
struct SSERVER_GROUP_FOR_CLIENT
{
	CHAR			MFS_ServerIDName[SIZE_MAX_SERVER_NAME];
	CHAR			MFS_Name[SIZE_MAX_SERVER_NAME];
};

struct MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT
{// 아레나 통합 -
	SSERVER_GROUP_FOR_CLIENT		ServerGroupInfo[SIZE_MAX_SERVER_GROUP_COUNT];
};



///////////////////////////////
// FP_CONNECT

#define PRESERVER_AUTH_TYPE_LOGIN				0x00
// 2004-12-16 by cmkwon, 다른 필드서버로의 워프는 없으므로 삭제함
//#define PRESERVER_AUTH_TYPE_WARP_CONNECT		0x01
// 2005-07-21 by cmkwon, 다른 필드서버로의 GameStart는 없으므로 삭제함
//#define PRESERVER_AUTH_TYPE_CONNECT_GAMESTART	0x02

struct MSG_FP_CONNECT_AUTH_USER
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	SERVER_ID		FieldServerID;
	char			PrivateIP[SIZE_MAX_IPADDRESS];
	ClientIndex_t	ClientIndex;
	BYTE			AuthType;
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FP_CONNECT_AUTH_USER_OK
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			AccountUniqueNumber;
	ClientIndex_t	ClientIndex;
	BYTE			AuthType;
	//int			GalaNetAccountIDNum;							// 2006-06-01 by cmkwon, exteranl authentication DB accountID Number
	ATUM_DATE_TIME	AccountRegisteredDate;							// 2006-06-02 by cmkwon
	char			PasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];	// 2006-06-02 by cmkwon
	int				GameContinueTimeInSecondOfToday;				// 2006-11-15 by cmkwon, 오늘 하루 게임 접속 시간
	ATUM_DATE_TIME	LastGameEndDate;								// 2006-11-15 by cmkwon, 마지막 게임 종료 시간
	ATUM_DATE_TIME	Birthday;										// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 프로토콜 수정
	char			SecondaryPassword[SIZE_MAX_PASSWORD_MD5_STRING];	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - MSG_FP_CONNECT_AUTH_USER_OK 에 필드 추가
	USHORT			UserType;											// 2013-01-18 by khkim, GLog 유저 권한 
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FP_CONNECT_FIELD_CONNECT
{
	char		FieldServerGroupName[SIZE_MAX_SERVER_NAME];
	SERVER_ID	FieldServerID;
	int			NumOfMapIndex;
	bool		ArenaFieldServerCheck;	// 2007-12-26 by dhjin, 아레나 통합 - TRUE => 아레나 필드 서버
	SDBSERVER_GROUP		DBServerGroup;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	ARRAY_(MapIndex_t);					// array of MapIndexes
};

struct MSG_FP_CONNECT_FIELD_CONNECT_OK
{
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FP_CONNECT_NOTIFY_CLOSE
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	ClientIndex_t	ClientIndex;
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	SERVER_ID		FieldServerID;
	ClientIndex_t	ClientIndex;
};

struct MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK
{
	ClientIndex_t	ClientIndex;
};


struct MSG_FP_CONNECT_UPDATE_DBSERVER_GROUP		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
{
	SDBSERVER_GROUP DBServerGroup;
};

struct MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT			// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
{
	char					AccountName[SIZE_MAX_ACCOUNT_NAME];
	MSG_PC_CONNECT_LOGIN_OK PCConnectLoginOK;
};

struct MSG_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
{
	int						ErrorCode;	// 0이면 No Error, 0이 아니면 에러코드임 
	char					AccountName[SIZE_MAX_ACCOUNT_NAME];
	MSG_PC_CONNECT_LOGIN_OK PCConnectLoginOK;
};

// start 2011-12-12 by hskim, GLog 2차
//#define T_FP_GLOG_CONNECT_USER_NEW				(MessageType_t)((T0_FP_GLOG<<8)|T1_FP_GLOG_CONNECT_USER_NEW)
//#define T_FP_GLOG_CONNECT_USER_LOGIN			(MessageType_t)((T0_FP_GLOG<<8)|T1_FP_GLOG_CONNECT_USER_LOGIN)

struct MSG_FP_GLOG_CONNECT_USER_NEW
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];		// 신규 계정 유저
};

struct MSG_FP_GLOG_CONNECT_USER_LOGIN
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];		// 기존 유져 접속 추가
};
// end 2011-12-12 by hskim, GLog 2차

///////////////////////////////
// IP_CONNECT

struct MSG_IP_CONNECT_IM_CONNECT
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	SERVER_ID	IMServerID;								// 2006-05-10 by cmkwon
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
														// 2006-05-10 by cmkwon, IMServerID 변수로 변경 - IP 정보도 줘야한다.
														//	int		IMServerListenPort;
};

struct MSG_IP_GET_SERVER_GROUP_INFO_ACK
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
	UINT	IMCurrentUserCounts;
	UINT	IMMaxUserCounts;
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_IP_ADMIN_PETITION_SET_PERIOD
{// 2007-11-19 by cmkwon, 진정시스템 업데이트 - MSG_IP_ADMIN_PETITION_SET_PERIOD 구조체 추가
	BYTE			byIsImmediatOn;		// 2007-11-20 by cmkwon, 즉시 시작 플래그
	BYTE			byIsImmediatOff;	// 2007-11-20 by cmkwon, 즉시 종료 플래그
	ATUM_DATE_TIME	atStart;			// 2007-11-20 by cmkwon, 시작 날짜시간
	ATUM_DATE_TIME	atEnd;				// 2007-11-20 by cmkwon, 종료 날짜시간
};

///////////////////////////////
// FI_CONNECT

struct MSG_FI_CONNECT
{
	SERVER_ID	FieldServerID;
	int			NumOfMapIndex;
	bool		ArenaServerCheck;		// 2008-02-28 by dhjin, 아레나 통합 - 0:일반 게임서버, 1:아레나통합서버
	ARRAY_(MapIndex_t);					// array of MapIndexes
};

struct MSG_FI_CONNECT_NOTIFY_FIELDSERVER_IP
{
	UID32_t		CharacterUniqueNumber;
	SERVER_ID	FieldServerID;
	//char		IPAddress[SIZE_MAX_IPADDRESS];
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FI_CONNECT_NOTIFY_GAMEEND
{
	UID32_t	CharacterUniqueNumber;
};


///////////////////////////////////////////////////////////////////////////////
struct MSG_PP_CONNECT		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
{
	int			nServiceUID;						// 현재는 사용하지 않음, 향후 처리를 위해 추가
	int			nLanguageType;						// 
	char		szPreServerIP[SIZE_MAX_IPADDRESS];
	USHORT		nPreServerPort;
	BYTE		byIsOnlyChoiceServer;
	BYTE		byIsTestServer;
	BYTE		byIsUseExternalAuthentication;
	int			nPreServerGroupCnts;				// PreServer Count
	int			nEnableGameServerGroupCnts;			// Active ServerGroup Count
	char		szReserve[100];						// 
	char *GetWriteLogString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%d|%d|%15s|%d|%d|%d|%d|%d|%d\r\n", nServiceUID, nLanguageType, szPreServerIP, nPreServerPort
			, byIsOnlyChoiceServer, byIsTestServer, byIsUseExternalAuthentication, nPreServerGroupCnts, nEnableGameServerGroupCnts);
		return o_szLogString;
	}
};

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, 인증 서버 구현
struct MSG_PATUM_CONNECT
{
	char		szGameName[SIZE_MAX_AUTH_GAMENAME];
	char		szServerIP[SIZE_MAX_IPADDRESS];
	USHORT		nServerPort;
	char		szCurrentVer[SIZE_MAX_AUTH_CURRENTVER];
	int			nLanguageType;
	BYTE		byTestServer;
	BYTE		byUseExternalAuthentication;
	int			nPreServerGroupCnts;
	int			nEnableGameServerGroupCnts;
	char		szReserve[50];
	char *GetWriteLogString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%s|%d|%s|%d|%d|%d|%d|%d\r\n", szGameName, szServerIP, nServerPort, szCurrentVer, nLanguageType,
			byTestServer, byUseExternalAuthentication, nPreServerGroupCnts, nEnableGameServerGroupCnts);

		return o_szLogString;
	}
};

struct MSG_PATUM_CONNECT_OK
{
	char		szReserve[100];
};

struct MSG_PATUM_CONNECT_FAIL
{
	char		szReserve[100];
};

// start 2011-06-22 by hskim, 사설 서버 방지
struct MSG_PATUM_CONNECT_SHUTDOWN
{
	char		szReserve[100];
};
// end 2011-06-22 by hskim, 사설 서버 방지

#//if defined(_ATUM_SERVER)	// 2008-02-26 by cmkwon, 클라이언트에서 컴파일 오류 문제 해결
struct MSG_PP_CONNECT_OK		// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer 로 서비스 정보 전송 시스템 추가 - 
{
	char		szPreServerODBCDSN[SIZE_MAX_ODBC_CONN_STRING];		// ODBC_DSN
	char		szPreServerODBCUID[SIZE_MAX_ODBC_CONN_STRING];		// ODBC_UID
	char		szPreServerODBCPASSWORD[SIZE_MAX_ODBC_CONN_STRING];	// ODBC_PASSWD	
	char		szReserve[100];										// 
};
//#endif

struct MSG_FI_EVENT_NOTIFY_WARP
{
	UID32_t	CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;	// 변화된 워프할 새 맵 이름
	INT		nNumOfTimer;					// 같은 필드 서버로의 워프일 때는 0
	ARRAY_(TIMER_EVENT_4_EXCHANGE);
};					// 변화된 맵 이름, 남은 Timer 정보(같은 필드 서버로의 워프는 보내지 않는다), 등등

struct MSG_FI_EVENT_CHAT_BLOCK				// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 
{
	char	szBlockedCharacterName[SIZE_MAX_CHARACTER_NAME];
	int		nBlockedMinutes;
};



// Just Declare
struct TIMER_EVENT;

struct TIMER_EVENT_4_EXCHANGE
{
	TimerEventType		Type;
	//	TimeUnit_t			StartTimeStamp;			// milli-seconds
	//	TimeUnit_t			ExpireTime;				// milli-seconds
	TimeUnit_t			TimeInterval;			// milli-seconds, (ExpireTime - StartTimeStamp)
												//	CFieldIOCPSocket	*pFieldIOCPSocket;
	UID32_t				CharacterUniqueNumber;	// event를 시작한 charac이 나가고 다른 charac이 socket을 사용하는 경우를 막기 위해, IsUsing()과 CharacterUniqueNumber가 같아야 함!
												//	ClientIndex_t		ClientIndex;			// event를 시작한 charac이 나가고 다른 charac이 socket을 사용하는 경우를 막기 위해, IsUsing()과 ClientIndex가 같아야 함!
	float				FloatParam1;			// delete_item류: 남은 Endurance
	INT					IntParam1;				// delete_item류: ItemNum
												//	TIMER_EVENT_BUCKET	*pCurrentBucket;

												// operator overloading
	TIMER_EVENT_4_EXCHANGE& operator=(const TIMER_EVENT& rhs);
};

struct MSG_FI_EVENT_NOTIFY_WARP_OK
{
	UID32_t	CharacterUniqueNumber;
};			// 다른 필드 서버로의 워프인 경우만 받는다

struct MSG_FI_EVENT_GET_WARP_INFO
{
	UID32_t			CharacterUniqueNumber;
};			// Party & TimerEvent정보, 다른 필드 서버로의 워프인 경우 정보 요청

struct MSG_FI_EVENT_GET_WARP_INFO_OK
{
	UID32_t			AccountUniqueNumber;		// 다른 필드 서버로의 워프시, 인증에 필요함
	UID32_t			CharacterUniqueNumber;
	PartyID_t		PartyID;
	UID32_t			MasterCharacterUniqueNumber;
	USHORT			nPartyMembers;
	USHORT			nTimerEvents;
	ARRAY_(FI_PARTY_MEMBER_INFO);
	ARRAY_(TIMER_EVENT_4_EXCHANGE);
};		// Party & TimerEvent정보, 다른 필드 서버로의 워프인 경우 정보 주기

struct MSG_FI_CONNECT_NOTIFY_DEAD
{
	UID32_t	CharacterUniqueNumber;
	UID32_t AttackerGuildUID;
};	// F->I, check: 현재는 DEAD시 파티 탈퇴를 위해서 만들었다, 20030821, kelovon

struct MSG_FI_GET_FIELD_USER_COUNTS_ACK
{
	SERVER_ID	FieldServerID;
	UINT		AccumulatedFieldUserCounts;
	UINT		CurrentFieldUserCounts;
	UINT		MaxFieldUserCounts;
	UINT		AccumulatedFieldMonsterCounts;
	UINT		CurrentFieldMonsterCounts;
};

enum GameStartType
{
	GST_GAMESTART = 0,
	GST_CONN_GAMESTART = 1,
	GST_WARP_CONN_GAMESTART = 2,
	GST_WARP_SAME_MAP = 3,
	GST_WARP_SAME_FIELD_SERVER = 4
};

struct MSG_FI_CONNECT_NOTIFY_GAMESTART {
	UID32_t			CharacterUniqueNumber;
	GameStartType	FieldGameStartType;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// F->I, 게임 시작했을 때 IM Server에 알림, 파티 정보 확인 요청 등

struct MSG_FI_CONNECT_NOTIFY_DEAD_GAMESTART
{
	UID32_t			CharacterUniqueNumber;
};

struct MSG_FI_CONNECT_PREPARE_SHUTDOWN	// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
{
	bool			bPrepareShutdown;			// TURE면 서버다운 준비 시작, FALSE이면 서버다운 준비 해제
};

///////////////////////////////
// IC_CONNECT

struct MSG_IC_CONNECT_LOGIN
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	ServerName[SIZE_MAX_SERVER_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];			// MD5
	BYTE	LoginType;						// see below
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

#define CHAT_LOGIN_TYPE_GAME		0x00
#define CHAT_LOGIN_TYPE_MESSENGER	0x01

struct MSG_IC_CONNECT_CLOSE
{
	INT		reason;
};

/*
// check: 확인 필요
//typedef	struct
//{
//	BYTE					NumCharacters;
//	FC_CONNECT_LOGIN_INFO	Characters[3];
//} MSG_IC_CONNECT_LOGIN_OK;
//*/

///////////////////////////////
// FM_CONNECT
struct MSG_FM_CONNECT_OK
{
	char	ServerName[SIZE_MAX_SERVER_NAME];
	time_t	StartedTime;
	char	IPAddress[SIZE_MAX_IPADDRESS];
	USHORT	Port;
	USHORT	NumOfClients;
	bool	CalcBandwidth;
	USHORT	LoadedMapCounts;
};
using MSG_NM_CONNECT_OK = MSG_FM_CONNECT_OK;
using MSG_IM_CONNECT_OK = MSG_FM_CONNECT_OK;

///////////////////////////////
// PM_CONNECT
struct MSG_PM_CONNECT_OK
{
	char	ServerName[SIZE_MAX_SERVER_NAME];
	time_t	StartedTime;
	char	IPAddress[SIZE_MAX_IPADDRESS];
	USHORT	Port;
	USHORT	NumOfClients;
	bool	CalcBandwidth;
	USHORT	LoadedMapCounts;
};

///////////////////////////////
// IC_CHAT

struct MSG_IC_CHAT_BASE
{
	char	FromCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE	NameColor;
	BYTE	MessageLength;
};

using MSG_IC_CHAT_MAP = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_REGION = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_PARTY = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_GUILD = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_ALL = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_CHATROOM = MSG_IC_CHAT_BASE;

#ifdef _SHOW_INFLUENCE_ON_GMNATION
struct MSG_IC_CHAT_WAR : MSG_IC_CHAT_BASE
{
	BYTE	InfluenceID;
};
#endif

/// Cross Nation Chat
struct MSG_IC_CHAT_CNC : MSG_IC_CHAT_BASE
{
	char	InfluenceOrStaffPrefix[SIZE_MAX_INFLUENCE_OR_STAFFPREFIX];	// Like BCU or CoMa
};

struct MSG_IC_CHAT_PTOP : MSG_IC_CHAT_BASE
{
	BYTE	FromInflTy;
	char	ToCharacterName[SIZE_MAX_CHARACTER_NAME];
};


struct MSG_IC_CHAT_GET_GUILD
{
	UID32_t	CharacterUniqueNumber;
};

struct MSG_IC_CHAT_GET_GUILD_OK
{
	UID32_t	CharacterUniqueNumber;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE	ConnectionCondition;			// 미접속,메신저,게임
};

struct MSG_IC_CHAT_CHANGE_GUILD
{
	UID32_t	CharacterUniqueNumber;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	Guild[SIZE_MAX_GUILD_NAME];
	BYTE	Joined;							// 가입 여부
};

struct MSG_IC_CHAT_CHANGE_GUILD_OK
{
	UID32_t	CharacterUniqueNumber;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	Guild[SIZE_MAX_GUILD_NAME];
	BYTE	Joined;							// 가입 여부
};

struct MSG_IC_CHAT_CHANGE_PARTY
{
	UID32_t	CharacterUniqueNumber;
	char	PartyName[20];
	BYTE	Joined;			// 가입, 탈퇴
};

struct MSG_IC_CHAT_CHANGE_PARTY_OK
{
	UID32_t	CharacterUniqueNumber;
	char	PartyName[20];
	BYTE	Joined;			// 가입, 탈퇴
};

struct MSG_IC_CHAT_CHANGE_CHAT_FLAG
{
	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
	//	BitFlag8_t	bitChatType;	// see below
	BitFlag16_t	bitChatType;	// see below
};

using MSG_IC_CHAT_SELL_ALL = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_CASH_ALL = MSG_IC_CHAT_BASE;
using MSG_IC_CHAT_INFLUENCE_ALL = MSG_IC_CHAT_BASE;			// 2006-04-21 by cmkwon
using MSG_IC_CHAT_ARENA = MSG_IC_CHAT_BASE;					// 2007-05-02 by dhjin
#ifndef _SHOW_INFLUENCE_ON_GMNATION
using MSG_IC_CHAT_WAR = MSG_IC_CHAT_BASE;					// 2008-05-19 by dhjin, EP3 - 채팅 시스템 변경, 전쟁 채팅
#endif
using MSG_IC_CHAT_INFINITY = MSG_IC_CHAT_BASE;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
using MSG_FC_CHAT_CNC = MSG_IC_CHAT_CNC;

// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	#define CHAT_TYPE_UNCONDITIONAL	(BitFlag8_t)0x01	// disable 불가, (공지 사항, 유료 전체 채팅, 지역채팅)
//	#define CHAT_TYPE_SELL_ALL		(BitFlag8_t)0x02	// disable 가능, 매매 전체 채팅
//	#define CHAT_TYPE_MAP			(BitFlag8_t)0x04	// disable 가능, 맵 채팅
//	#define CHAT_TYPE_PTOP			(BitFlag8_t)0x08	// disable 가능, 1:1 채팅 
//	#define CHAT_TYPE_GUILD			(BitFlag8_t)0x10	// disable 가능, 길드 채팅
//	#define CHAT_TYPE_PARTY			(BitFlag8_t)0x20	// disable 가능, 파티 채팅
//	#define CHAT_TYPE_INFLUENCE		(BitFlag8_t)0x40	// disable 불가
//	#define CHAT_TYPE_ARENA			(BitFlag8_t)0x80	// disable 불가, ARENA 채팅
#define CHAT_TYPE_UNCONDITIONAL	(BitFlag16_t)0x0001	// disable 불가, (공지 사항, 유료 전체 채팅, 지역채팅)
#define CHAT_TYPE_SELL_ALL		(BitFlag16_t)0x0002	// disable 가능, 매매 전체 채팅
#define CHAT_TYPE_MAP			(BitFlag16_t)0x0004	// disable 가능, 맵 채팅
#define CHAT_TYPE_PTOP			(BitFlag16_t)0x0008	// disable 가능, 1:1 채팅 
#define CHAT_TYPE_GUILD			(BitFlag16_t)0x0010	// disable 가능, 길드 채팅
#define CHAT_TYPE_PARTY			(BitFlag16_t)0x0020	// disable 가능, 파티 채팅
#define CHAT_TYPE_INFLUENCE		(BitFlag16_t)0x0040	// disable 불가
#define CHAT_TYPE_ARENA			(BitFlag16_t)0x0080	// disable 불가, ARENA 채팅
#define CHAT_TYPE_WAR			(BitFlag16_t)0x0100	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경,	전쟁 채팅 
#define CHAT_TYPE_CHATROOM		(BitFlag16_t)0x0200	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경, 채팅방 
#define CHAT_TYPE_SYSTEM		(BitFlag16_t)0x0400	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경, 시스템 
#define CHAT_TYPE_HELPDESK		(BitFlag16_t)0x0800	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경, 도움말
#define CHAT_TYPE_CNC			(BitFlag16_t)0x1000 // 2014-08-03 by Future, adding Cross Nation Chat

#define COMPARE_CHATTYPE_BIT(VAR, MASK)	(((VAR) & (MASK)) != 0)
#define CLEAR_CHATTYPE_BIT(VAR, MASK)	{ (VAR) &= ~(MASK); }
#define SET_CHATTYPE_BIT(VAR, MASK)		{ (VAR) |= (MASK); }

struct MSG_IC_CHAT_POSITION
{
	UID32_t		CharacterUniqueNumber;
	AVECTOR3	PosVector;
};

enum FRIEND_TYPE
{
	FRIEND_TYPE_FRIEND = 0,
	FRIEND_TYPE_REJECT = 1
};

struct DB_FRIEND_INFO
{
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	char	FriendName[SIZE_MAX_CHARACTER_NAME];
	BYTE	FriendType;
	ATUM_DATE_TIME	RegDate;	// 2008-04-11 by dhjin, EP3 거부목록 -
};

struct FRIENDINFO
{
	char		szCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		byIsOnline;
	ATUM_DATE_TIME	RegDate;	// 2008-04-11 by dhjin, EP3 거부목록 -
};

struct MSG_IC_CHAT_FRIENDLIST_LOADING_OK
{
	int			nFriendListCounts;
	ARRAY_(FRIENDINFO);
};

struct MSG_IC_CHAT_FRIENDLIST_INSERT
{
	char szCharacterName[SIZE_MAX_CHARACTER_NAME];
};

using MSG_IC_CHAT_FRIENDLIST_INSERT_OK = FRIENDINFO;

using MSG_IC_CHAT_FRIENDLIST_DELETE = MSG_IC_CHAT_FRIENDLIST_INSERT;

using MSG_IC_CHAT_REJECTLIST_DELETE_OK = MSG_IC_CHAT_FRIENDLIST_INSERT;

struct MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY		// 2009-01-13 by cmkwon, 친구 등록시 상대방에게 알림 시스템 적용 - 
{
	char szDoerCharacName[SIZE_MAX_CHARACTER_NAME];	// 친구 등록시 상대방이 온라인 상태인 경우 알림
};

using MSG_IC_CHAT_FRIENDLIST_REFRESH_OK = MSG_IC_CHAT_FRIENDLIST_LOADING_OK;

struct MSG_IC_CHAT_REJECTLIST_LOADING_OK
{
	int			nRejectListCounts;
	ARRAY_(FRIENDINFO);
};

using MSG_IC_CHAT_REJECTLIST_INSERT = MSG_IC_CHAT_FRIENDLIST_INSERT;

//////////////////////////////////////////////////////////////////////////
// 2008-04-11 by dhjin, EP3 거부목록 - 밑과 같이 수정
// typedef MSG_IC_CHAT_FRIENDLIST_INSERT		MSG_IC_CHAT_REJECTLIST_INSERT_OK;
struct MSG_IC_CHAT_REJECTLIST_INSERT_OK
{
	char szCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	RegDate;
};

using MSG_IC_CHAT_REJECTLIST_DELETE = MSG_IC_CHAT_FRIENDLIST_INSERT;

using MSG_IC_CHAT_FRIENDLIST_DELETE_OK = MSG_IC_CHAT_FRIENDLIST_INSERT;



///////////////////////////////
// FC_CHAT
using MSG_FC_CHAT_MAP = MSG_IC_CHAT_BASE;
using MSG_FC_CHAT_REGION = MSG_IC_CHAT_REGION;
using MSG_FC_CHAT_CASH_ALL = MSG_IC_CHAT_CASH_ALL;
using MSG_FC_CHAT_ARENA = MSG_IC_CHAT_ARENA;
using MSG_FC_CHAT_OUTPOST_GUILD = MSG_IC_CHAT_BASE;	// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능
using MSG_FC_CHAT_INFINITY = MSG_IC_CHAT_INFINITY;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
using MSG_FI_CHAT_CNC = MSG_IC_CHAT_CNC;	// 2015-07-20 Future, CnC Costs Money

struct MSG_FC_CHAT_ALL_INFLUENCE		// 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 - 구조체 추가
{
	char	FromCharacterName[SIZE_MAX_CHARACTER_NAME];
	char	str256ChatMsg[SIZE_MAX_STRING_256];			// 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 - 
#ifdef _SHOW_SPEAKER_IN_CHAT
	BYTE		nInfluence;
#endif
};

///////////////////////////////
// FI_CHAT
struct MSG_FI_CHAT_MAP
{
	UID32_t	CharacterUniqueNumber;
	BYTE	MessageLength;
	//뒤에 메시지를 붙여서 보내기
	//char*	ChatMessage;
};

using MSG_FI_CHAT_REGION = MSG_FI_CHAT_MAP;

struct MSG_FI_CHAT_CHANGE_CHAT_FLAG
{
	UID32_t		CharacterUniqueNumber;
	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
	//	BitFlag8_t	ChatFlag;				// 채팅 설정 flag
	BitFlag16_t	ChatFlag;				// 채팅 설정 flag
};

using MSG_FI_CHAT_CASH_ALL = MSG_FI_CHAT_MAP;
using MSG_FI_CHAT_ARENA = MSG_FI_CHAT_MAP;		// 2007-05-02 by dhjin
using MSG_FI_CHAT_OUTPOST_GUILD = MSG_FI_CHAT_MAP;		// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능
using MSG_FI_CHAT_INFINITY = MSG_FI_CHAT_MAP;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅

struct MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK		// 2006-07-18 by cmkwon
{
	UID32_t		CharacterUniqueNumber;
	int			OnlineFriendCnts;			// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트	
};

struct MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT
{// 2008-07-11 by dhjin, EP3 친구목록 -
	int			OnlineFriendCnts;
};

///////////////////////////////
// FC_CHARACTER
struct MSG_FC_CHARACTER_GET_CHARACTER
{
	UID32_t		AccountUniqueNumber;
	UID32_t		CharacterUniqueNumber;
	// START 2011-11-03 by shcho, yedang 셧다운제 구현 - 생일정보도 추가로 받아온다.
	bool     ShutDownMINS;
	// END 2011-11-03 by shcho, yedang 셧다운제 구현 - 생일정보도 추가로 받아온다.
};

struct MSG_FC_CHARACTER_GET_CHARACTER_OK
{
	CHARACTER	Character;
	//char		FieldServerIP[SIZE_MAX_IPADDRESS];		// MSG_FC_CHARACTER_GAMESTART_OK로 이동
};

struct MSG_FC_CHARACTER_CREATE
{
	UID32_t		AccountUniqueNumber;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT		UnitKind;
	USHORT		Race;
	BYTE		AutoStatType1;
	GEAR_STAT	GearStat1;			// 기어 스탯
	BYTE		Color;
	BYTE		PilotFace;
	BYTE		InfluenceType0;		// 2005-06-23 by cmkwon, 세력 타입으로 나중에 사용 예정 필드
};

struct MSG_FC_CHARACTER_CREATE_OK
{
	CHARACTER				Character;
	CHARACTER_RENDER_INFO	CharacterRenderInfo;		// 2006-01-23 by cmkwon, 추가함
};

struct MSG_FC_CHARACTER_DELETE
{
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];	// 2007-09-13 by cmkwon, 베트남 2차패스워드 구현 - MSG_FC_CHARACTER_DELETE 에 필드 추가
};

/* This message has NO body
//typedef struct
//{
//} MSG_FC_CHARACTER_DELETE_OK;
*/

struct MSG_FC_CHARACTER_REPAIR
{
	UID32_t	CharacterUniqueNumber;
};

struct MSG_FC_CHARACTER_REPAIR_OK
{
	INT	RepairCharge;
	INT	Remainder;
};


struct MSG_FC_CHARACTER_REPAIR_ERR
{
	BYTE	ErrCode;
};

// Repair Error Code
#define ERROR_REPAIR_NOERROR			0x00
#define ERROR_REPAIR_NOT_ENOUGH_MONEY	0x01

struct MSG_FC_CHARACTER_DOCKING
{
	UINT	ItemIndex;
};

struct MSG_FC_CHARACTER_UNDOCKING
{
	UINT	 ItemIndex;
};

struct MSG_FC_CHARACTER_DOCKING_ERR
{
	BYTE	 ErrCode;
};

// Docking Error Code
#define ERROR_DOCKING_NOERROR			0x00
#define ERROR_DOCKING_NO_SUCH_ITEM		0x01
#define ERROR_DOCKING_INVALID_POSITION	0x02

struct MSG_FC_CHARACTER_GAMESTART
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
};

struct MSG_FC_CHARACTER_GAMESTART_OK
{
	ClientIndex_t	ClientIndex;
	char			FieldServerIP[SIZE_MAX_IPADDRESS];
	int				FieldServerPort;
	BYTE			CharacterMode0;
	AVECTOR3		PositionVector;
	BitFlag16_t		MapWeather;
	bool			bMemberPCBang;			// 2007-01-25 by cmkwon, 가맹 PC방 플래그
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];			// 2007-04-09 by cmkwon
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];		// 2007-04-09 by cmkwon
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];	// 2007-04-09 by cmkwon
	MAP_CHANNEL_INDEX MapInfo;				// 2007-12-12 by dhjin, 맵정보 미리 보내기
	BodyCond_t		BodyCondition;			// 2008-04-01 by cmkwon, 도시맵에서 죽은 상태의 기어 부활시켜서 캐릭터 상태로 처리 - MSG_FC_CHARACTER_GAMESTART_OK 에 추가
	float			CurrentHP;				// 2008-04-01 by cmkwon, 도시맵에서 죽은 상태의 기어 부활시켜서 캐릭터 상태로 처리 - MSG_FC_CHARACTER_GAMESTART_OK 에 추가
	float			CurrentDP;				// 2008-04-01 by cmkwon, 도시맵에서 죽은 상태의 기어 부활시켜서 캐릭터 상태로 처리 - MSG_FC_CHARACTER_GAMESTART_OK 에 추가
	float			CurrentEP;				// 2008-04-01 by cmkwon, 도시맵에서 죽은 상태의 기어 부활시켜서 캐릭터 상태로 처리 - MSG_FC_CHARACTER_GAMESTART_OK 에 추가
	float			CurrentSP;				// 2008-04-01 by cmkwon, 도시맵에서 죽은 상태의 기어 부활시켜서 캐릭터 상태로 처리 - MSG_FC_CHARACTER_GAMESTART_OK 에 추가
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];				// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FC_CHARACTER_CONNECT_GAMESTART
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];				// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FC_CHARACTER_CONNECT_GAMESTART_OK
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;
	BitFlag16_t		MapWeather;
};

struct MSG_FC_CHARACTER_DEAD_GAMESTART
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
	bool			bRebirthInCityMap;			// TRUE:세력별도시맵 부활, FALSE:현재맵에서 부활
};

struct MSG_FC_CHARACTER_OTHER_REVIVED {
	UID32_t			CharacterUniqueNumber;
};	// F -> C, 죽었다 되살아났을 때 다른 캐릭터(현재는 파티원)에게 보냄

struct MSG_FC_CHARACTER_GAMEEND
{
	ClientIndex_t	ClientIndex;
	bool			SelectCharacterView;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 케릭터 선택 창으로 이동이면 TRUE	
};

struct MSG_FC_CHARACTER_GAMEEND_OK
{
	ClientIndex_t	ClientIndex;
};

struct MSG_FC_CHARACTER_CHANGE_UNITKIND
{
	ClientIndex_t	ClientIndex;
	USHORT			UnitKind;
};

struct MSG_FC_CHARACTER_CHANGE_STAT
{
	ClientIndex_t	ClientIndex;
	GEAR_STAT		GearStat1;		// 기어 스탯
	BYTE			BonusStat;		// 남은 보너스 스탯
};
struct MSG_FC_CHARACTER_CHANGE_TOTALGEAR_STAT
{
	ClientIndex_t	ClientIndex;
	BYTE			byAutoStatType;
	GEAR_STAT		GearStat1;		// 기어 스탯
};

struct MSG_FC_CHARACTER_CHANGE_GUILD
{
	ClientIndex_t	ClientIndex;
	char			Guild[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_CHARACTER_CHANGE_LEVEL
{
	ClientIndex_t	ClientIndex;
	BYTE			Level;
	BYTE			BonusStat;
	// 2005-11-15 by cmkwon, 제거함
	//	BYTE			BonusSkillPoint;
};

struct MSG_FC_CHARACTER_CHANGE_EXP
{
	ClientIndex_t	ClientIndex;
	Experience_t	Experience;						// 총 경험치
};

struct MSG_FC_CHARACTER_CHANGE_BODYCONDITION
{
	ClientIndex_t	ClientIndex;
	BodyCond_t		BodyCondition;					// 앞의 32bit만 사용 <-- check: 스킬 bodycon이 정해지면 결정하기!
};

struct MSG_FC_CHARACTER_CHANGE_BODYCONDITION_SKILL
{
	ClientIndex_t	ClientIndex;
	BodyCond_t		SkillBodyCondition;				// 뒤의 32bit만 사용 <-- check: 스킬 bodycon이 정해지면 결정하기!
};

struct MSG_FC_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	ClientIndex_t	ClientIndex;
	BYTE			InfluenceType0;					// 
};

struct MSG_FC_CHARACTER_CHANGE_BODYCONDITION_ALL
{
	ClientIndex_t	ClientIndex;
	BodyCond_t		BodyCondition;					// 앞의 32bit만 사용 <-- check: 스킬 bodycon이 정해지면 결정하기!
};		// C->F, 강제로 BodyCondition 세팅 요청

struct MSG_FC_CHARACTER_CHANGE_PROPENSITY
{
	ClientIndex_t	ClientIndex;
	BYTE			byUpdateType;					// 2005-08-22 by cmkwon, 현재는 두개(IUT_PENALTY_ON_DEAD, IUT_PENALTY_AGEAR_FUEL_ALLIN) - IUT_XXX, ITEM_UPDATE_TYPE_XXX
	INT				Propensity;
};				// 2005-08-22 by cmkwon, 

struct MSG_FC_CHARACTER_CHANGE_STATUS
{
	ClientIndex_t	ClientIndex;
	BYTE			Status;
};

struct MSG_FC_CHARACTER_CHANGE_PKPOINT
{
	ClientIndex_t	ClientIndex;
	INT				Propensity;
	USHORT			PKWinPoint;
	USHORT			PKLossPoint;
};

struct MSG_FC_CHARACTER_CHANGE_HPDPSPEP
{
	ClientIndex_t	ClientIndex;
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

struct MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP
{
	ClientIndex_t	ClientIndex;
	float	CurrentHP;
	float	CurrentDP;
	SHORT	CurrentSP;
	float	CurrentEP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTHP
{
	ClientIndex_t	ClientIndex;
	float			CurrentHP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTDP
{
	ClientIndex_t	ClientIndex;
	float			CurrentDP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTSP
{
	ClientIndex_t	ClientIndex;
	SHORT			CurrentSP;
};

struct MSG_FC_CHARACTER_CHANGE_CURRENTEP
{
	ClientIndex_t	ClientIndex;
	float			CurrentEP;
};

struct MSG_FC_CHARACTER_CHANGE_MAPNAME
{
	ClientIndex_t		ClientIndex;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct MSG_FC_CHARACTER_CHANGE_PETINFO
{
	ClientIndex_t	ClientIndex;
	BYTE			Level;
	Experience_t	Experience;
};

struct MSG_FC_CHARACTER_CHANGE_POSITION
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;		// 캐릭터 좌표
};

struct MSG_FC_CHARACTER_USE_BONUSSTAT
{
	ClientIndex_t	ClientIndex;
	BYTE	KindOfStat;	// see below
};

struct MSG_FC_CHARACTER_USE_BONUSSTAT_OK		// 2006-09-18 by cmkwon
{
	BYTE	byReaminBonusStat;
};

struct MSG_FC_CHARACTER_DEAD_NOTIFY
{
	ClientIndex_t	ClientIndex;
	BYTE			byDamageKind;
	bool			bDeadByP2PPK;	// P2PPK 진행중 죽음
};


#define COUNT_MAX_STAT_POINT			(CHARACTER_LEVEL_110_MAX_STAT_POINT)	// 2009-12-29 by cmkwon, 캐릭터 최대 레벨 상향(110으로) - 100Lv이상은 340이 최대

// Kind of Stat, STAT_XXX
#define STAT_ATTACK_PART				((BYTE)0)	// 공격 파트
#define STAT_DEFENSE_PART				((BYTE)1)	// 방어 파트
#define STAT_FUEL_PART					((BYTE)2)	// 연료 파트
#define STAT_SOUL_PART					((BYTE)3)	// 정신 파트
#define STAT_SHIELD_PART				((BYTE)4)	// 쉴드 파트
#define STAT_DODGE_PART					((BYTE)5)	// 회피 파트
#define STAT_BONUS						((BYTE)6)	// 보너스로 받는 stat
#define STAT_ALL_PART					((BYTE)7)	// 모든 파트
#define STAT_BONUS_STAT_POINT			((BYTE)8)	// 2007-06-20 by cmkwon, 보너스 스탯 증가 로그 남기기 - 보너스로 받는 statPoint

///////////////////////////////////////////////////////////////////////////////
// 캐릭 생성시 Level 20까지의 
#define AUTOSTAT_TYPE_FREESTYLE					0	// 모든기어 자유형
#define AUTOSTAT_TYPE_BGEAR_ATTACK				1	// B-Gear 공격형
#define AUTOSTAT_TYPE_BGEAR_MULTI				2	// B-Gear 멀티형
#define AUTOSTAT_TYPE_IGEAR_ATTACK				3	// I-Gear 공격형
#define AUTOSTAT_TYPE_IGEAR_DODGE				4	// I-Gear 회피형
#define AUTOSTAT_TYPE_AGEAR_ATTACK				5	// A-Gear 공격형
#define AUTOSTAT_TYPE_AGEAR_SHIELD				6	// A-Gear 쉴드형
#define AUTOSTAT_TYPE_MGEAR_DEFENSE				7	// M-Gear 방어형
#define AUTOSTAT_TYPE_MGEAR_SUPPORT				8	// M-Gear 지원형

struct MSG_FC_CHARACTER_GET_OTHER_INFO
{
	ClientIndex_t		ClientIndex;
};

#define CITYWAR_TEAM_TYPE_NORMAL				0	// 도시점령전맵이 아니거나 도시점령전이 시작안됨
#define CITYWAR_TEAM_TYPE_ATTACKER				1	// 공격측, 도전측
#define CITYWAR_TEAM_TYPE_DEFENSER				2	// 방어측, 

struct MEX_OTHER_CHARACTER_INFO
{
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 아래와 같이 필드 순서를 정렬한다.
	BodyCond_t		BodyCondition;
	INT				Propensity;			// 2005-12-27 by cmkwon, 성향(선,악)이 아니고 명성으로 사용중
	UID32_t			CharacterUniqueNumber;
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 사용하지 않으므로 삭제
	//	INT				RacingPoint;		// Racing 결과 Point
	UID32_t			GuildUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;		// 캐릭터가 속한 맵 및 채널, 2006-01-18 by cmkwon, 추가함
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	AVECTOR3		UpVector;
	ClientIndex_t	ClientIndex;
	USHORT			Race;
	USHORT			UnitKind;
	USHORT			PKWinPoint;			// PK 승리 수치
	USHORT			PKLossPoint;		// PK 패배 수치
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	char			szCharacterMent[SIZE_STRING_32];
	BYTE			Color;				// false(0) : 여, true(1) : 남
	BYTE			PilotFace;			// 화면에 나타나는 인물 캐릭터
	BYTE			CharacterMode0;		// 2005-07-13 by cmkwon, 현재 캐릭터 상태 플래그
	BYTE			InfluenceType;		// 세력 타입, 2005-06-23 by cmkwon
	BYTE			Level1;				//
	BYTE			CityWarTeamType;	// 
	BYTE			Status;				// 신분

	BYTE			UsingReturnItem;				// 2013-02-28 by bckim, 복귀유져 버프추가

													// operator overloading
	MEX_OTHER_CHARACTER_INFO& operator=(const CHARACTER& rhs)
	{
		ClientIndex = rhs.ClientIndex;
		CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		util::strncpy(CharacterName, rhs.CharacterName, SIZE_MAX_CHARACTER_NAME);
		Color = rhs.Color;
		PilotFace = rhs.PilotFace;
		CharacterMode0 = rhs.CharacterMode;
		InfluenceType = rhs.InfluenceType;
		Race = rhs.Race;
		UnitKind = rhs.UnitKind;
		Level1 = rhs.Level;
		Propensity = rhs.Propensity;
		Status = rhs.Status;
		PKWinPoint = rhs.PKWinPoint;
		PKLossPoint = rhs.PKLossPoint;
		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 사용하지 않으므로 삭제
		//		RacingPoint				= rhs.RacingPoint;
		GuildUniqueNumber = rhs.GuildUniqueNumber;
		BodyCondition = rhs.BodyCondition;
		MapChannelIndex = rhs.MapChannelIndex;
		PositionVector = rhs.PositionVector;
		TargetVector = rhs.TargetVector*1000.0f;
		UpVector = rhs.UpVector;

		UsingReturnItem = rhs.bUsingReturnItem;		// 2013-02-28 by bckim, 복귀유져 버프추가

		return *this;
	}
};

struct MSG_FC_CHARACTER_GET_OTHER_INFO_OK
{
	MEX_OTHER_CHARACTER_INFO	CharacterInfo;
	CHARACTER_RENDER_INFO		CharacterRenderInfo;
};

struct MSG_FC_CHARACTER_GET_MONSTER_INFO_OK
{
	ClientIndex_t	MonsterIndex;
	INT				CurrentHP;
	INT				MonsterUnitKind;
	SHORT			MonsterForm;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	BodyCond_t		BodyCondition;
	INT				MaxHP;
};

struct MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];	// 대화 상대
};				// 통화 요청 시 필요함

struct MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];	// 대화 상대
	UID32_t		AccountUniqueNumber;					// 대화 상대
};			// 통화 요청 시 필요함

			// 충돌 타입
#define COLLISION_TYPE_LAND			(BYTE)0x00	// 땅, 산 등의 지형과의 충돌
#define COLLISION_TYPE_BUILDING		(BYTE)0x01	// 건물과의 충돌
#define COLLISION_TYPE_CHARACTER	(BYTE)0x02	// 다른 캐릭터와의 충돌
#define COLLISION_TYPE_MONSTER		(BYTE)0x03	// 몬스터와의 충돌
struct MSG_FC_CHARACTER_APPLY_COLLISION_DAMAGE
{
	ClientIndex_t	UnitIndex;								// 몬스터와 충돌시는 충돌 몬스터 Index, 캐릭과 충돌시는 충돌 캐릭터 Index
	SHORT			SpeedOfCollision;						// 충돌시 나의 스피드
	BYTE			CollisionType;							// 충돌 타입, see below
};


struct MSG_FC_CHARACTER_GET_OTHER_MOVE
{
	ClientIndex_t	OtherClientIndex;
};			// C -> F, 다른 유닛의 MOVE 정보를 요청함, 응답은 MSG_FC_MOVE

struct MSG_FC_CHARACTER_DELETE_OTHER_INFO
{
	ClientIndex_t	OtherClientIndex;
};		// F -> C,	클라이언트에게 다른 유닛(OTHER_INFO)을 지우라고 요청함

struct MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO {
	ClientIndex_t	ClientIndex;
};

struct MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK
{
	ClientIndex_t			ClientIndex;
	CHARACTER_RENDER_INFO	CharacterRenderInfo;
};

//typedef struct {
//	ClientIndex_t			ClientIndex;
//	USHORT					UnitState;
//} MSG_FC_CHARACTER_PUT_OTHER_EFFECT;		// F->C, 다른 유닛의 상태 정보를 보냄, 순간 이펙트 등에 사용, check: 제거됨, 20040626, kelovon
//
//#define UNIT_EFFECT_NO_EFFECT		((USHORT)0)
//#define UNIT_EFFECT_HP_UP			((USHORT)1)
//#define UNIT_EFFECT_DP_UP			((USHORT)2)
//#define UNIT_EFFECT_SP_UP			((USHORT)3)
//#define UNIT_EFFECT_EP_UP			((USHORT)4)

struct MSG_FC_CHARACTER_SHOW_EFFECT
{
	UINT			EffectIndex;	// 클라이언트에 정의된 effect의 index
};		// C->F, 주위에 자신의 이펙트 전송 요청

struct MSG_FC_CHARACTER_SHOW_EFFECT_OK
{
	ClientIndex_t	ClientIndex;
	UINT			EffectIndex;	// 클라이언트에 정의된 effect의 index
};	// F->C, 주위에 캐릭들에게 전송

struct MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR
{
	ClientIndex_t	ClientIndex;
	BYTE			ItemPosition;				// POS_XXX
};		// C->F, 해당 캐릭터의 ParamFactor 정보 요청

struct DES_PARAM_VALUE
{
	BYTE			DestParameter;				// 대상파라미터, DES_XXX
	float			ParameterValue;				// 수정파라미터
};

struct MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK
{
	ClientIndex_t	ClientIndex;
	BYTE			ItemPosition;				// POS_XXX
	INT				ItemNum;
	INT				NumOfParamValues;
	ARRAY_(DES_PARAM_VALUE);
};	// F->C, 해당 캐릭터의 ParamFactor 정보 요청 결과

struct MSG_FC_CHARACTER_SEND_PARAMFACTOR_IN_RANGE
{
	BYTE			ItemPosition;				// POS_XXX
};	// C->F, 자신의 ParamFactor를 주위에 보내도록 요청

struct MSG_FC_CHARACTER_GET_OTHER_SKILL_INFO
{
	ClientIndex_t	ClientIndex;				// 상대방의 ClientIndex
};		// C->F

struct MSG_FC_CHARACTER_GET_OTHER_SKILL_INFO_OK
{
	ClientIndex_t	ClientIndex;				// 상대방의 ClientIndex
	INT				NumOfSkillInfos;
	ARRAY_(MEX_OTHER_SKILL_INFO);
};		// F->C

struct MSG_FC_CHARACTER_SPEED_HACK_USER
{
	int				nSendMoveCounts;			// 서버로 전송한 Move 패킷 카운트
	DWORD			dwTimeGap;					// 시간(단위 ms)
};

struct MSG_FC_CHARACTER_CHANGE_CHARACTER_MENT
{
	ClientIndex_t	ClientIdx;
	char			szCharacterMent1[SIZE_STRING_32];
};

struct MSG_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK
{
	int				nMCash;						// MCash
	int				nGiftCard;					// 상품권
};

struct MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO
{
	INT				nCardItemNum1;
	ATUM_DATE_TIME	atumTimeUpdatedTime1;	// 수정된 시간
	ATUM_DATE_TIME	atumTimeExpireTime1;	// 만료 시간
	float			fExpRate1;
	float			fDropRate1;
	float			fDropRareRate1;
	float			fExpRepairRate1;
};

struct MSG_FC_CHARACTER_TUTORIAL_SKIP		// 2006-10-13 by cmkwon
{
	UID32_t				CharacterUniqueNumber;
};

struct MSG_FC_CHARACTER_TUTORIAL_SKIP_OK
{
	ClientIndex_t		ClientIndex;
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	mapChannelIdx;
};

struct MEX_OTHER_SKILL_INFO
{
	INT		SkillItemNum;						// Skill의 ItemNum
};

// 2005-07-26 by hblee : 착륙장에서 캐릭터 모드 변환.
struct MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE
{
	BYTE				CharacterMode0;
	AVECTOR3			PositionAVec3;
	AVECTOR3			TargetAVec3;
};

struct MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK
{
	ClientIndex_t		ClientIndex;
	BYTE				CharacterMode0;
	AVECTOR3			PositionAVec3;
	AVECTOR3			TargetAVec3;
};

struct MSG_FC_CHARACTER_GET_REAL_WEAPON_INFO_OK		// 2005-12-21 by cmkwon
{
	UID64_t		ItemUID0;
	float		ShotNum0;
	float		MultiNum0;
	float		ReattackTime0;
	float		RepeatTime0;
	float		Time0;
	float		RangeAngle0;
	float		BoosterAngle0;
	float		OrbitType0;
};

struct MSG_FC_CHARACTER_GET_REAL_ENGINE_INFO_OK		// 2005-12-21 by cmkwon
{
	UID64_t		ItemUID0;
	float		AbilityMin0;
	float		AbilityMax0;
	float		SpeedPenalty0;
	float		Time0;
	float		Range0;
	float		RangeAngle0;
	float		BoosterAngle0;
};

struct MSG_FC_CHARACTER_GET_REAL_TOTAL_WEIGHT_OK		// 2005-12-21 by cmkwon
{
	UID64_t		ItemUID0;				// 아머의 ItemUID
	float		Transport0;				// 무게 능력
	float		TotalWeight0;			// 현재 무게 총량
};

struct MSG_FC_CHARACTER_MEMORY_HACK_USER				// 2005-12-22 by cmkwon
{
	UID64_t		ItemUID0;				// 엔진의 ItemUID
	float		ValidMoveDistance;		// 원래 유효 최대 이동 거리 - TickGap시간 동안 가능한 최대 거리
	float		CurrentMoveDistance;	// 이동된 거리 - TickGap시간 동안 이동한 거리
	int			TickGap;				// 경과된 시간(단위:ms, ex> 1초= 1000, 0.5초= 500)
};

struct MSG_FC_CHARACTER_UPDATE_SUBLEADER
{// 2007-02-13 by dhjin, 부지도자 설정 프로시저, 몇 번째 부지도자인지 체크가 필요
	BYTE			InflType;
	BYTE			SubLeaderRank;		// 2007-10-06 by dhjin, 몇 번째 부지도자인지
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK
{// 2007-10-06 by dhjin, 부지도자 설정이 성공시 클라이언트로 전송
	BYTE			SubLeaderRank;		// 2007-10-06 by dhjin, 몇 번째 부지도자인지
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX
{// 2007-03-27 by dhjin, 옵저버 모드 상대방 인덱스 
	ClientIndex_t		TargetClientIndex;
};
using MSG_FC_CHARACTER_OBSERVER_START = MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX;
using MSG_FC_CHARACTER_OBSERVER_END = MSG_FC_CHARACTER_OBSERVER_TARGET_CHARACTERINDEX;


struct MSG_FC_CHARACTER_OBSERVER_INFO
{// 2007-03-27 by dhjin, 옵저버에게 제공되는 정보
	ClientIndex_t		ClientIndex;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	SHORT				SP;
	SHORT				EP;
	float				CurrentHP;
	float				CurrentDP;
	SHORT				CurrentSP;
	float				CurrentEP;
};

struct MSG_FC_CHARACTER_OBSERVER_REG
{// 2007-03-27 by dhjin, 옵저버 유저가 대상 유저 등록하는 구조체
	ClientIndex_t		ClientIndex;
	SHORT				nRegNum;
};

struct MSG_FC_CHARACTER_SHOW_MAP_EFFECT		// 2007-04-20 by cmkwon
{
	UINT			EffectIndex;			// 클라이언트에 정의된 effect의 index
	AVECTOR3		avec3Up;				// effect의 Up Vector, Normalize 해야함
	AVECTOR3		avec3Target;			// effect의 Target Vector, Normalize 해야함
	AVECTOR3		avec3Position;			// effect의 Positon Vector
	INT				nLifetime;				// effect의 유효시간(단위:ms) - 0 이하이면 사라지지 않은 effect 임
};

struct MSG_FC_CHARACTER_SHOW_MAP_EFFECT_OK		// 2007-04-20 by cmkwon
{
	ClientIndex_t	ClientIdx;
	UINT			EffectIndex;			// 클라이언트에 정의된 effect의 index
	AVECTOR3		avec3Up;				// effect의 Up Vector, Normalize 해야함(=실제UpVector*1000f)
	AVECTOR3		avec3Target;			// effect의 Target Vector, Normalize 해야함(=실제TargetVector*1000f)
	AVECTOR3		avec3Position;			// effect의 Positon Vector
	INT				nLifetime;				// effect의 유효시간(단위:ms) - 0 이하이면 사라지지 않은 effect 임
};

struct MSG_FC_CHARACTER_PAY_WARPOINT
{// 2007-05-16 by dhjin, WarPoint가 지급되어 전송 한다.
	INT				WarPoint;				// 2007-05-16 by dhjin, 지급된 WarPoint
	INT				TotalWarPoint;			// 2007-05-16 by dhjin, 총 WarPoint
	INT				CumulativeWarPoint;		// 2007-05-28 by dhjin, 누적 WarPoint
	bool			UseItemFlag;			// 2010-08-27 by shcho&&jskim, WARPOINT 증가 아이템 구현 - 아이템 사용획득(TRUE)인지 게임에서 획득(FALSE)인지 구분
	bool			ShowSystemMessage;		// 2015-07-22 Future, make System message hideable
};

struct MSG_FC_CHARACTER_WATCH_INFO
{// 2007-06-19 by dhjin, 관전자에게 제공되는 정보
	ClientIndex_t		ClientIndex;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float				CurrentHP;
	float				CurrentDP;
};

struct MSG_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER
{// 2008-01-11 by dhjin, 아레나 통합 - 
	ClientIndex_t		ClientIndex;
};

struct MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER
{// 2008-01-31 by dhjin, 아레나 통합 - 
	ClientIndex_t		ClientIndex;
};

struct MSG_FC_CHARACTER_GET_USER_INFO
{// 2008-06-20 by dhjin, EP3 유저정보옵션 -
	UID32_t				TargetCharcterUID;
};

struct MSG_FC_CHARACTER_GET_USER_INFO_OK
{// 2008-06-20 by dhjin, EP3 유저정보옵션 -
	BYTE				PilotFace;						// 화면에 나타나는 인물 케릭터
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];		// 유닛(캐릭터) 이름
	USHORT				UnitKind;						// 유닛의 종류	
	UID32_t				GuildUID;
	char				GuildName[SIZE_MAX_GUILD_NAME];	// 길드 이름
	BYTE				Level;
	MAP_CHANNEL_INDEX	MapChannelIndex;
	INT					Propensity;						// 명성치, 성향(선, 악)
	ATUM_DATE_TIME		LastStartedTime;				// 최종 게임 시작 시간
	char				NickName[SIZE_MAX_CHARACTER_NAME];			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
};

// 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부
struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET
{
	UID32_t				TargetCharcterUID;
};
struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK
{
	bool				bItemInfoSecret;
};
// end 2012-07-12 by isshin 아템미리보기 On / Off 기능 - 공개 여부

// 2012-06-05 by jhseol, 아템미리보기 - 클라요청 패킷 추가
struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO
{
	UID32_t				TargetCharcterUID;
};
// end 2012-06-05 by jhseol, 아템미리보기 - 클라요청 패킷 추가

struct MSG_FC_CHARACTER_CHANGE_INFO_OPTION_SECRET
{// 2008-06-20 by dhjin, EP3 유저정보옵션 -
	INT					SecretInfoOption;
};

struct MSG_FC_CHARACTER_CHANGE_NICKNAME		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
{
	UID32_t				CharacUID;
	char				NickName[SIZE_MAX_CHARACTER_NAME];
};
using MSG_FC_CHARACTER_CHANGE_NICKNAME_OK = MSG_FC_CHARACTER_CHANGE_NICKNAME;	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

struct MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX		// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
{
	UID32_t				CharacUID;
	MapIndex_t			nStartCityMapIdx;
};
using MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK = MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX;	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 


struct MSG_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
{
	UID32_t				CharacUID;
	int					nRacingPoint;
};

// 2012-10-10 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
struct MSG_FC_CHARACTER_KILLCOUNT
{
	INT nCount;
};		// C->F

		//#if S_BONUSEXPSYSTEM_RENEWAL
struct MSG_FC_CHARACTER_RESTCOUNT
{
	INT nKillCount;
	INT nRestCount;
};		// C->F
		//#endif // S_BONUSEXPSYSTEM_RENEWAL

struct MSG_FC_CHARACTER_BONUS_EXP_RATE
{
	INT nPartyRate;
	INT nFriendRate;
	INT nGuildRate;
};		// C->F
		// end 2012-10-10 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
		///////////////////////////////////////////////////////////////////////////////
		// FN CHARACTER
struct MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	BYTE				CharacterMode0;
};

struct MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	BYTE				InfluenceType0;				// 
};

struct MSG_FN_CLIENT_GAMEEND_OK : public MSG_FC_CHARACTER_GAMEEND_OK
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_UNITKIND : public MSG_FC_CHARACTER_CHANGE_UNITKIND
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_BODYCONDITION : public MSG_FC_CHARACTER_CHANGE_BODYCONDITION
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_HPDPSPEP : public MSG_FC_CHARACTER_CHANGE_HPDPSPEP
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP : public MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_CURRENTHP : public MSG_FC_CHARACTER_CHANGE_CURRENTHP
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_CURRENTSP : public MSG_FC_CHARACTER_CHANGE_CURRENTSP
{
	ChannelIndex_t		ChannelIndex;
};		// check: 필요한가? 철민씨에게 확인 요!

struct MSG_FN_CHARACTER_CHANGE_CURRENTEP : public MSG_FC_CHARACTER_CHANGE_CURRENTEP
{
	ChannelIndex_t		ChannelIndex;
};		// check: 필요한가? 철민씨에게 확인 요!
struct MSG_FN_CHARACTER_CHANGE_MAPNAME : public MSG_FC_CHARACTER_CHANGE_MAPNAME
{
	ChannelIndex_t		ChannelIndex;
};
struct MSG_FN_CHARACTER_CHANGE_PETINFO : public MSG_FC_CHARACTER_CHANGE_PETINFO
{
	ChannelIndex_t		ChannelIndex;
};
struct MSG_FN_CHARACTER_CHANGE_POSITION : public MSG_FC_CHARACTER_CHANGE_POSITION
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_CHARACTER_CHANGE_STEALTHSTATE
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	bool				bStealthState2;
};

struct MSG_FN_CHARACTER_CHANGE_INVISIBLE
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		ClientIndex;
	bool				bInvisible;
};

///////////////////////////////////////////////////////////////////////////////
// FC_MOVE ( Field server <-> Client)
struct MSG_FC_MOVE
{
	ClientIndex_t	ClientIndex;
	USHORT			TimeGap;
	BYTE			DistanceGap;		// Client와 같이 삭제할 예정임
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	AVECTOR3		UpVector;
};

struct MSG_FC_MOVE_BIT_FLAG					// 2007-03-29 by cmkwon
{
	bool			CharacterMode0 : 1;		// 
	bool			Invisible0 : 1;			// 인비지블 스킬 사용 상태
	bool			ChargingSkill : 1;		// 2007-04-02 by cmkwon, A-Gear 차징 스킬 사용 상태
	bool			HyperShot : 1;			// A-Gear 하이퍼샷 스킬 사용 상태
											// 2012-12-06 by jhseol, 서치아이 범위 캐릭터 중심으로 변경
	bool			bSearchEye_1 : 1;			// 일반 서치아이
	bool			bSearchEye_2 : 1;			// 강화 서치아이
	bool			bUsingBarialSkill : 1;		// 2013-01-29 by jhseol, 베리어 스킬 이펙트 버그
	bool			bUsingInvicibleSkill : 1;	// 2013-02-06 by jhseol, M기어 무적스킬 이벡트 버그

	MSG_FC_MOVE_BIT_FLAG() :
		CharacterMode0 { false },
		Invisible0 { false },
		ChargingSkill { false },
		HyperShot { false },
		bSearchEye_1 { false },
		bSearchEye_2 { false },
		bUsingBarialSkill { false },
		bUsingInvicibleSkill { false }
	{ }
};

// 2012-12-06 by jhseol, 서치아이 범위 캐릭터 중심으로 변경
struct UsingItemBitFlag
{
	bool			bSearchEye_1;			// 일반 서치아이
	bool			bSearchEye_2;			// 강화 서치아이
	bool			bUsingBarialSkill;		// 2013-01-29 by jhseol, 베리어 스킬 이펙트 버그
	bool			bUsingInvicibleSkill;	// 2013-02-06 by jhseol, M기어 무적스킬 이벡트 버그
	UsingItemBitFlag()
	{
		bSearchEye_1 = FALSE;				// 초기값 미사용으로 셋팅 (FALSE : 미사용, TRUE : 사용)
		bSearchEye_2 = FALSE;				// 초기값 미사용으로 셋팅 (FALSE : 미사용, TRUE : 사용)
		bUsingBarialSkill = FALSE;			// 2013-01-29 by jhseol, 베리어 스킬 이펙트 버그
		bUsingInvicibleSkill = FALSE;		// 2013-02-06 by jhseol, M기어 무적스킬 이벡트 버그
	}
};
// end 2012-12-06 by jhseol, 서치아이 범위 캐릭터 중심으로 변경

struct MSG_FC_MOVE_OK
{
	ClientIndex_t	ClientIndex;
	// 2007-03-29 by cmkwon, 아래와 같이 비트플래그 변수를 하나 추가함
	//	BYTE			CharacterMode0;		// 2005-07-29 by cmkwon
	MSG_FC_MOVE_BIT_FLAG moveBitFlag;	// 2007-03-29 by cmkwon
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	AVECTOR3		UpVector;
};

struct MSG_FC_MISSILE_MOVE_OK
{
	UINT			ItemNum;						// 공격 무기의 종류
	ClientIndex_t	MonsterIndex;					// 공격 몬스터의 인덱스
	INT				WeaponIndex;					// 공격 무기 인덱스(몬스터에 한해 유일함)
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FC_MOVE_LOCKON
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_LOCKON_OK
{
	char			AttackCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	AttackIndex;		// 2005-10-11 by cmkwon
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_UNLOCKON
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_UNLOCKON_OK
{
	char			AttackCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	AttackIndex;		// 2005-10-12 by cmkwon
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_MOVE_LANDING
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		Position;
};

struct MSG_FC_MOVE_LANDING_OK
{
	AVECTOR3	Position;
};

struct MSG_FC_MOVE_LANDING_DONE
{
	AVECTOR3	Position;
};		// C->F, 착륙 완료를 알림

struct MSG_FC_MOVE_TAKEOFF
{
	ClientIndex_t		ClientIndex;
	AVECTOR3	Position;
};

struct MSG_FC_MOVE_TAKEOFF_OK
{
	AVECTOR3	Position;
};

using MSG_FC_MOVE_TARGET = MSG_UNIT_INDEX;

struct MSG_FC_MOVE_WEAPON_VEL
{
	D3DXVECTOR3		WeaponVel;
	D3DXVECTOR3		PetLeftVel;
	D3DXVECTOR3		PetRightVel;
};		// C->F, 무기의 방향의 움직임 전송

struct MSG_FC_MOVE_WEAPON_VEL_OK
{
	ClientIndex_t	ClientIndex;
	D3DXVECTOR3		WeaponVel;
	D3DXVECTOR3		PetLeftVel;
	D3DXVECTOR3		PetRightVel;
};	// F->C_in_range, 무기의 방향의 움직임 전송

struct MSG_FC_MOVE_ROLLING
{
	AVECTOR3		PositionAVec3;
	AVECTOR3		TargetAVec3;
	AVECTOR3		UpAVec3;
	BYTE			byLeftDirectionFlag;		// 좌측 롤링 플래그
};
struct MSG_FC_MOVE_ROLLING_OK
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionAVec3;
	AVECTOR3		TargetAVec3;
	AVECTOR3		UpAVec3;
	BYTE			byLeftDirectionFlag;		// 좌측 롤링 플래그
};

struct MSG_FC_MOVE_HACKSHIELD_CRCReqMsg			// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 
{
	_AHNHS_TRANS_BUFFER		stRequestBuf;
};
struct MSG_FC_MOVE_HACKSHIELD_CRCAckMsg			// 2008-04-03 by cmkwon, 핵쉴드 서버 연동 시스템 수정 - 
{
	_AHNHS_TRANS_BUFFER		stResponseBuf;
};

struct MSG_FC_MOVE_HACKSHIELD_HACKING_CLIENT	// 2006-06-05 by cmkwon
{
	long			lHackingClinetCode;			// ERR_AHNHS_XXXXXX	
	char			szErrString[SIZE_STRING_128];			// 2006-10-20 by cmkwon, 추가함(해킹 프로그램명)
};

struct MEX_TARGET_INFO
{
	AVECTOR3		TargetPosition;			// 공격 클라이언트의 화면에서의 타켓 포지션
	ClientIndex_t	TargetIndex;			// 공격 대상 ClientIndex or MonterIndex, 0이면 ItemFieldIndex만 유효
	UINT			TargetItemFieldIndex;	// 공격 대상에 부착된 아이템이면 TargetIndex 유효, 아니면 TargetIndex는 0
	USHORT			MultiTargetIndex;		// 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

public:
	void SetNullTarget()
	{
		TargetIndex = 0;
		TargetItemFieldIndex = 0;
		MultiTargetIndex = 0;				// 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
	}
};

struct MSG_FC_BATTLE_ATTACK
{
	MEX_TARGET_INFO	TargetInfo;
	AVECTOR3		FirePosition;		// 무기의 발사 위치
	BYTE			AttackType;			// 공격 타입, ATT_TYPE_XXX, see below
	UINT			SkillNum;			// 스킬 사용 시 사용
};					// C->F

					// ATT_TYPE_XXX, 1~100: 1형 공격, 101~200: 2형 공격
#define	ATT_TYPE_NONE			((BYTE)0)	// Attack Type 없음
					// 1형 공격
#define	ATT_TYPE_GENERAL_PRI		((BYTE)1)	// 1형 무기 일반 공격
#define	ATT_TYPE_GROUND_BOMBING_PRI	((BYTE)2)	// 1형 지상 폭격 모드
#define	ATT_TYPE_AIR_BOMBING_PRI	((BYTE)3)	// 1형 공중 폭격 모드
#define	ATT_TYPE_SEIGE_PRI			((BYTE)4)	// 1형 시즈 모드
#define	ATT_TYPE_SPLASH_PRI			((BYTE)5)	// 1형 Splash 공격 모드
#define	ATT_TYPE_END_PRI			((BYTE)100)	// 1형 무기 공격 끝, 실제로 안 쓰임
					// 2형 공격
#define ATT_TYPE_GENERAL_SEC		((BYTE)101)	// 2형 무기 일반 공격
#define ATT_TYPE_GROUND_BOMBING_SEC	((BYTE)102)	// 2형 지상 폭격 모드
#define ATT_TYPE_AIR_BOMBING_SEC	((BYTE)103)	// 2형 공중 폭격 모드
#define ATT_TYPE_SIEGE_SEC			((BYTE)104)	// 2형 시즈 모드
#define	ATT_TYPE_END_SEC			((BYTE)200)	// 2형 무기 공격 끝, 실제로 안 쓰임

					// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
#define ATT_TYPE_PET_ATK			ATT_TYPE_PET_GENERAL
#define ATT_TYPE_PET_GENERAL		((BYTE)201) // 펫 공격(일반 공격)
#define ATT_TYPE_PET_END			((BYTE)300)	// 펫 공격 끝

#define IS_PET_ATT_TPYE(_ATT_TYPE)			(util::in_range(ATT_TYPE_PET_START, _ATT_TYPE, ATT_TYPE_PET_END))
					// END 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리
#define IS_PRIMARY_ATT_TYPE(_ATT_TYPE)		(util::in_range(ATT_TYPE_GENERAL_PRI, _ATT_TYPE, ATT_TYPE_END_PRI))
#define IS_SECONDARY_ATT_TYPE(_ATT_TYPE)	(util::in_range(ATT_TYPE_GENERAL_SEC, _ATT_TYPE, ATT_TYPE_END_SEC))

					///////////////////////////////////////////////////////////////////////////////
					// 2007-06-04 by cmkwon
struct SATTACK_PARAMETER
{
	UID16_t			WeaponIndex;					// 탄두 고유번호 - 2형 무기만 처리됨
	DWORD			dwShotTick;						// 2007-06-08 by cmkwon, 발사 Tick
	ITEM			*pWeaponItemInfo;				// 공격 무기 아이템 정보
	float			fAttackProbability;				// 공격 확률
	float			fPierceAttackProbability;		// 공격 피어스율
	float			fAttack;						// 공격력
	float			fMaxAttack;						// 최대공격력	
	ClientIndex_t	TargetIndex;					// 2010-04-05 by cmkwon, 인피2차 M2M 2형 무기 보완 처리 - 
	BYTE			AttackType;						// 2010-04-05 by cmkwon, 인피2차 M2M 2형 무기 보완 처리 - 
	USHORT			MultiTargetIndex;				// 2011-04-04 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
};
using mtvectSATTACK_PARAMETER = mt_vector<SATTACK_PARAMETER>;		// 2007-06-07 by cmkwon
using vectSATTACK_PARAMETER = vector<SATTACK_PARAMETER>;			// 2010-04-05 by cmkwon, 인피2차 M2M 2형 무기 보완 처리 - 

struct MSG_FC_BATTLE_ATTACK_OK
{
	ClientIndex_t	AttackIndex;
	MEX_TARGET_INFO	TargetInfo;
	AVECTOR3		FirePosition;		// 무기의 발사 위치
	BYTE			AttackType;			// 공격 타입, ATT_TYPE_XXX
	UID16_t			WeaponIndex;		// 클라이언트에서 발사된 총알의 인덱스, 서버에서 생성, CUID16Generator 사용
	UINT			ItemNum;			// 무기의 ItemNum
	USHORT			RemainedBulletFuel;	// 남은 총알(혹은 Fuel)의 수
	UINT			SkillNum;			// 스킬 사용 시 사용
	ClientIndex_t	DelegateClientIdx;	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 간 2형 무기 폭팔 처리 할 위임 클라이언트
};				// F->C_in_range

struct MSG_FC_BATTLE_ATTACK_FIND
{
	ClientIndex_t	AttackIndex;
	MEX_TARGET_INFO	TargetInfo;
	UINT			ItemNum;			// 무기의 ItemNum, 몬스터의 2형 공격에만 사용됨
	UID16_t			WeaponIndex;		// 클라이언트에서 발사된 총알의 인덱스, 서버에서 생성, CUID16Generator 사용
	BYTE			AttackType;			// 공격 타입, ATT_TYPE_XXX
};			// C->F

struct MSG_FC_BATTLE_ATTACK_FIND_OK
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;			// 공격 대상 ClientIndex or MonterIndex, 0이면 ItemFieldIndex만 유효
	UINT			TargetItemFieldIndex;	// 공격 대상에 부착된 아이템이면 TargetIndex 유효, 아니면 TargetIndex는 0
	UID16_t			WeaponIndex;		// 클라이언트에서 발사된 총알의 인덱스, 서버에서 생성, CUID16Generator 사용
	BYTE			AttackType;			// 공격 타입, ATT_TYPE_XXX
};			// F->C_in_range

			// 마인류 처리
struct MSG_FC_BATTLE_DROP_MINE
{
	BYTE			NumOfMines;
	ClientIndex_t	TargetIndex;		// Mine을 쏠때 Target이 있으면 설정된다(서버는 클라이언트로 전달만 하면됨)
	ARRAY_(AVECTOR3);					// MINE이 떨어질 위치
};				// C->F

struct MSG_FC_BATTLE_DROP_MINE_OK
{
	ClientIndex_t	AttackIndex;		// MINE 공격자
	ClientIndex_t	TargetIndex;		// Mine을 쏠때 Target이 있으면 설정된다(서버는 클라이언트로 전달만 하면됨)
	UINT			ItemFieldIndex;		// 습득 전까지 서버가 임시로 관리하는 마인 번호
	INT				ItemNum;			// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
	AVECTOR3		DropPosition;		// MINE이 떨어질 위치
	USHORT			RemainedBulletFuel;	// 남은 총알(혹은 Fuel)의 수
};			// F->C_in_range, 아이템 보여주기

struct MSG_FC_BATTLE_MINE_ATTACK
{
	UINT			ItemFieldIndex;		// 서버가 임시로 관리하는 마인 번호
	MEX_TARGET_INFO	TargetInfo;			// 피공격자
};			// C->F

struct MSG_FC_BATTLE_MINE_ATTACK_OK
{
	UINT			ItemFieldIndex;		// 서버가 임시로 관리하는 마인 번호
	MEX_TARGET_INFO	TargetInfo;			// 피공격자
};			// F->C_in_range

struct MSG_FC_BATTLE_MINE_ATTACK_FIND
{
	UINT			ItemFieldIndex;		// 서버가 임시로 관리하는 마인 번호
	MEX_TARGET_INFO	TargetInfo;			// 피공격자
};		// C->F

struct MSG_FC_BATTLE_MINE_ATTACK_FIND_OK
{
	UINT			ItemFieldIndex;		// 서버가 임시로 관리하는 마인 번호
	MEX_TARGET_INFO	TargetInfo;			// 피공격자
};	// F->C_in_range

	// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 아래와 같이 수정함
	//typedef struct  {
	//	USHORT			BulletCount;		// 무기의 reload된 총알의 최종 개수
	//} MSG_FC_BATTLE_PRI_BULLET_RELOADED;
	//
	//typedef struct  {
	//	USHORT			BulletCount;		// 무기의 reload된 총알의 최종 개수
	//} MSG_FC_BATTLE_SEC_BULLET_RELOADED;

	// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - BULLET_RECHARGE_TYPE_XXX 정의 추가
#define BULLET_RECHARGE_TYPE_NORMAL			((BYTE)0)
#define BULLET_RECHARGE_TYPE_REPAIR_SHOP	((BYTE)1)
#define BULLET_RECHARGE_TYPE_BULLET_ITEM	((BYTE)2)
#define BULLET_RECHARGE_TYPE_ADMIN_COMMAND	((BYTE)3)
struct  MSG_FC_BATTLE_PRI_BULLET_RELOADED
{
	USHORT			BulletCount;		// 무기의 reload된 총알의 최종 개수
	USHORT			RechargeCount;		// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 추가된 필드
	BYTE			RechargeType;		// 2007-08-07 by cmkwon, 1형/2형 무기 총알 충전 아이템 구현 - 추가된 필드(BULLET_RECHARGE_TYPE_XXX)
};
using MSG_FC_BATTLE_SEC_BULLET_RELOADED = MSG_FC_BATTLE_PRI_BULLET_RELOADED;

// Kind of Damages: 이펙트를 표시하기 위해 사용한다. check: 아래분류는 재정의되어야 한다.
// DAMAGEKIND_XXX
#define DAMAGEKIND_NO_DAMAGE	(BYTE)0x00
#define DAMAGEKIND_NORMAL		(BYTE)0x01	// 0 < DAMAGE < 100
#define DAMAGEKIND_CRITICAL		(BYTE)0x02	// 피공격자의 방어력 무시
#define DAMAGEKIND_ADD_DAMAGE	(BYTE)0x03	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 추가 타격치 아이템
#define DAMAGEKIND_REFLECTION	(BYTE)0x04	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 데미지 반사
#define DAMAGEKIND_PET			(BYTE)0x05	// 2010-11-01 by jskim, 펫 데미지 View 변경

struct MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM
{
	ClientIndex_t	TargetIndex;			// 해당 아이템을 소유한 ClientIndex
	UINT			TargetItemFieldIndex;
};		// 기체가 달고 있는 아이템(DUMMY 류 등)이 터지는 경우

struct MSG_FC_BATTLE_ATTACK_HIDE_ITEM
{
	ClientIndex_t	TargetIndex;			// 해당 아이템을 소유한 ClientIndex
	UINT			TargetItemFieldIndex;
};			// 기체가 달고 있는 아이템(DUMMY 류 등)이 그냥 사라지는 경우(Time Out 등로 인해...)

struct MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND
{
	ClientIndex_t	TargetIndex;			// 해당 아이템을 소유한 ClientIndex
	BYTE			ItemKind;
	UINT			TargetItemFieldIndex;
};	// With KIND, 기체가 달고 있는 아이템(FIXER 류 등)이 터지는 경우

struct MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND
{
	ClientIndex_t	TargetIndex;			// 해당 아이템을 소유한 ClientIndex
	BYTE			ItemKind;
	UINT			TargetItemFieldIndex;
};	// With KIND, 기체가 달고 있는 아이템(FIXER 류 등)이 그냥 사라지는 경우(Time Out 등로 인해...)

	// 번들류 처리
struct MSG_FC_BATTLE_DROP_BUNDLE
{
	UID64_t			ItemUniqueNumber;
	ClientIndex_t	AttackClientIndex;
	ClientIndex_t	TargetClientIndex;
	AVECTOR3		DropPosition;
};

struct MSG_FC_BATTLE_DROP_BUNDLE_OK
{
	UID64_t			ItemUniqueNumber;
	ClientIndex_t	AttackClientIndex;
	ClientIndex_t	TargetClientIndex;
	INT				ItemNum;			// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
	AVECTOR3		DropPosition;
	USHORT			NumOfBullet;		// 발사할 탄체의 개수
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK
{
	UID64_t			BundleItemUniqueNumber;	// 번들의 ItemUniqueNumber
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	AVECTOR3		TargetPosition;
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK_RESULT
{
	INT				ItemNum;				// bundle의 링크 아이템(미사일 등)의 ItemNum
	ClientIndex_t	AttackIndex;
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	AVECTOR3		TargetPosition;
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK_ITEM
{
	UID64_t			BundleItemUniqueNumber;	// 번들의 ItemUniqueNumber
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	UINT			TargetItemFieldIndex;	// 대상 아이템
	AVECTOR3		TargetPosition;
};

struct MSG_FC_BATTLE_BUNDLE_ATTACK_ITEM_RESULT
{
	INT				ItemNum;				// bundle의 링크 아이템(미사일 등)의 ItemNum
	ClientIndex_t	AttackIndex;
	AVECTOR3		AttackPosition;
	ClientIndex_t	TargetIndex;
	UINT			TargetItemFieldIndex;	// 대상 아이템
	AVECTOR3		TargetPosition;
};

// check: 필요하면 살림(현재 NO BODY임), kelovon, 20030917
//typedef struct
//{
//} MSG_FC_BATTLE_TOGGLE_SHIELD;

struct MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT
{
	ClientIndex_t	AttackIndex;		// shield를 가동한 캐릭터
	BYTE			IsOn;				// 0(FALSE): off, 1(TRUE): on
	INT				ItemNum;
};	// F->C, SHIELD류 부착중이면 MSG_FC_CHARACTER_GET_OTHER_INFO_OK보낸 후 이 MSG를 붙여보낸다.

	// check: 필요하면 살림(현재 NO BODY임), kelovon, 20040517
	//typedef struct
	//{
	//} MSG_FC_BATTLE_TOGGLE_DECOY;

struct MSG_FC_BATTLE_TOGGLE_DECOY_OK
{
	ClientIndex_t	AttackIndex;	// decoy를 가동한 캐릭터
	BYTE			IsOn;			// 0(FALSE): off, 1(TRUE): on
	INT				ItemNum;
};	// F->C, DECOY류 부착중이면 MSG_FC_CHARACTER_GET_OTHER_INFO_OK보낸 후 이 MSG를 붙여보낸다.

struct MSG_FC_BATTLE_SHIELD_DAMAGE
{
	ClientIndex_t	TargetIndex;		// shield를 가동시키고 있는 캐릭터
	AVECTOR3		CollisionPosition;	// 충돌 위치
};

// 더미(DUMMY)류
struct MSG_FC_BATTLE_DROP_DUMMY
{
	UID64_t		ItemUniqueNumber;
	//	BYTE		NumOfDummies;			// check: 사라짐. 20030930, kelovon with jinking
	//	ARRAY_(AVECTOR3);					// DUMMY의 위치(기체에 대한 상대 좌표), check: 사라짐. 20030930, kelovon with jinking
};				// 더미는 한번에 다 쏜다.

				// 2007-06-21 by cmkwon, 체프 하나의 메시지로 모두 전송 - 아래와 같이 구조체 수정
				//typedef struct
				//{
				//	ClientIndex_t	AttackIndex;		// DUMMY를 발동한 기체
				//	UINT			ItemFieldIndex;		// 습득 전까지 서버가 임시로 관리하는 DUMMY 번호
				//	INT				ItemNum;			// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
				////	AVECTOR3		DropPosition;		// 기체에 대한 상대 좌표, check: 사라짐. 20030930, kelovon with jinking
				//} MSG_FC_BATTLE_DROP_DUMMY_OK;			// 아이템 보여주기,  DUMMY류 부착중이면 MSG_FC_CHARACTER_GET_OTHER_INFO_OK보낸 후 이 MSG를 붙여보낸다.
struct MSG_FC_BATTLE_DROP_DUMMY_OK		// 2007-06-21 by cmkwon, 체프 하나의 메시지로 모두 전송 -
{
	ClientIndex_t	AttackIndex;		// DUMMY를 발동한 기체
	INT				ItemNum;			// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
	INT				DummyCounts;		// 2007-06-21 by cmkwon, 체프 하나의 메시지로 모두 전송 -
	_ARRAY(UINT ItemFieldIndex);		// 2007-06-21 by cmkwon, 체프 하나의 메시지로 모두 전송 - DummyCounts 만큼 붙여서 전송
};

// 픽서(FIXER)류
struct MSG_FC_BATTLE_DROP_FIXER
{
	UID64_t			ItemUniqueNumber;
	ClientIndex_t	TargetIndex;
};				// 한 번에 개수만큼 다 쏨

struct MSG_FC_BATTLE_DROP_FIXER_OK
{
	ClientIndex_t	AttackIndex;		// FIXER를 쏜 기체
	ClientIndex_t	TargetIndex;
	UINT			ItemFieldIndex;		// 습득 전까지 서버가 임시로 관리하는 FIXER류 번호
	INT				ItemNum;			// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
};			// 각각에 대해 전송, 아이템 보여주기, FIXER류 부착중이면 MSG_FC_CHARACTER_GET_OTHER_INFO_OK보낸 후 이 MSG를 붙여보낸다.

			// check: 필요하면 살림(현재 NO BODY임), kelovon, 20030612
			//typedef struct
			//{
			//} MSG_FC_BATTLE_REQUEST_PK;			// C->F, client의 PK 요청

struct MSG_FC_BATTLE_REQUEST_PK_OK
{
	ClientIndex_t	ClientIndex;		// PK를 요청한 client
};			// F->C, pk 요청 승낙

			// check: 필요하면 살림(현재 NO BODY임), kelovon, 20030612
			//typedef struct
			//{
			//} MSG_FC_BATTLE_CANCEL_PK;			// F->C, PK 해제

struct MSG_FC_BATTLE_REQUEST_P2P_PK
{
	ClientIndex_t	TargetClientIndex;		// 피요청자
	int				AdditionalParameter;	// 0 => 'normal' duel, 1 => with restrictions
};				// C->F, 일대일 PK 요청

struct MSG_FC_BATTLE_REQUEST_P2P_PK_OK
{
	ClientIndex_t	SourceClientIndex;		// 요청자
	int				AdditionalParameter;	// 0 => 'normal' duel, 1 => with restrictions
};			// F->C, 일대일 PK 요청

struct MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// 요청자
};		// C->F, 일대일 PK 승낙

struct MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK
{
	ClientIndex_t	PeerClientIndex;		// 상대방
};	// F->C, 일대일 PK 승낙, 양 쪽으로 보냄, 클라이언트는 받으면 PK 시작

struct MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// 요청자
};		// C->F, 일대일 PK 거절

struct MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK
{
	ClientIndex_t	TargetClientIndex;		// 피요청자
};	// F->C, 일대일 PK 거절

struct MSG_FC_BATTLE_SURRENDER_P2P_PK
{
	ClientIndex_t	TargetClientIndex;		// 피요청자
};			// C->F, 일대일 PK 항복

struct MSG_FC_BATTLE_SURRENDER_P2P_PK_OK
{
	ClientIndex_t	SourceClientIndex;		// 요청자
};		// F->C, 일대일 PK 항복

struct MSG_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// 요청자
};	// C->F, 일대일 PK 항복 승낙, 이에 대한 응답은 MSG_FC_BATTLE_END_P2P_PK로 처리

	/*
	struct MSG_FC_BATTLE_ACCEPT_SURRENDER_P2P_PK_OK
	{
	ClientIndex_t	TargetClientIndex;		// 피요청자
	};	// F->C, 일대일 PK 항복 승낙
	*/

struct MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK
{
	ClientIndex_t	SourceClientIndex;		// 요청자
};	// C->F, 일대일 PK 항복 거절

struct MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK
{
	ClientIndex_t	TargetClientIndex;		// 피요청자
};	// F->C, 일대일 PK 항복 거절

struct MSG_FC_BATTLE_END_P2P_PK
{
	ClientIndex_t	PeerClientIndex;		// 상대방의 ClientIndex
	USHORT			EndType;				// 결투 종료 타입, BATTLE_END_XXX
};					// PK 종료

					// 전투 종료 타입, BATTLE_END_XXX
#define BATTLE_END_WIN			(USHORT)0x0000	// 승리
#define BATTLE_END_DEFEAT		(USHORT)0x0001	// 패배
#define BATTLE_END_TIE			(USHORT)0x0002	// 무승부
#define BATTLE_END_END			(USHORT)0x0003	// 그냥 종료되었습니다(이유불문)
#define BATTLE_END_TIME_LIMITE	(USHORT)0x0004	// 시간제한
#define BATTLE_END_SURRENDER	(USHORT)0x0005	// 항복

struct MSG_FC_BATTLE_SHOW_DAMAGE
{
	ClientIndex_t	TargetIndex;	// TargetIndex
	USHORT			AmountDamage;	// 데미지 량
	BYTE			DamageKind;		// DAMAGEKIND_XXX
	BYTE			byIsPrimaryWeapon;		// 2008-12-03 by cmkwon, 데미지 정보에 1형,2형 정보 추가 - 
	USHORT			MultiTargetIndex;		// 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
};		// F->C, 공격 데미지량을 표시함



struct MSG_FC_BATTLE_ATTACK_EVASION		// 2005-12-12 by cmkwon
{
	ClientIndex_t	AttackIndex;
	MEX_TARGET_INFO	TargetInfo;
	UINT			ItemNum;			// 무기의 ItemNum, 몬스터의 2형 공격에만 사용됨
	UID16_t			WeaponIndex;		// 클라이언트에서 발사된 총알의 인덱스, 서버에서 생성, CUID16Generator 사용
	BYTE			AttackType;			// 공격 타입, ATT_TYPE_XXX
};

using MSG_FC_BATTLE_ATTACK_EVASION_OK = MSG_FC_BATTLE_ATTACK_EVASION;	// 2005-12-12 by cmkwon

struct MSG_FC_BATTLE_DELETE_DUMMY_OK		// 2006-12-04 by dhjin
{
	ClientIndex_t	AttackIndex;
	UINT			ItemFieldIndex;
};

struct MSG_FC_BATTLE_EXPLODE_DUMMY_OK		// 2006-12-04 by dhjin
{
	ClientIndex_t	AttackIndex;
	UINT			ItemFieldIndex;
};


///////////////////////////////////////////////////////////////////////////////
// FN_BATTLE (Field server <-> NPC server)

struct MSG_FN_BATTLE_ATTACK_PRIMARY
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// 무기 타입(각 무기종류,스킬종류)
	USHORT			WeaponIndex;		// 남은 총알(혹은 Fuel)의 수, Fuel인 경우에는 x10하여 계산함
	AVECTOR3		TargetPosition;		// 공격 클라이언트의 화면에서의 타켓 포지션
	USHORT			MultiTargetIndex;	// 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
};

struct MSG_FN_BATTLE_ATTACK_RESULT_PRIMARY
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// 무기 타입(각 무기종류,스킬종류)
	USHORT			WeaponIndex;		// 남은 총알(혹은 Fuel)의 수, Fuel인 경우에는 x10하여 계산함
	AVECTOR3		TargetPosition;		// 공격 클라이언트의 화면에서의 타켓 포지션
	BYTE			DamageKind;
};

struct MSG_FN_BATTLE_ATTACK_SECONDARY
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// 무기 타입(각 무기종류,스킬종류)
	USHORT			WeaponIndex;		// 남은 총알(혹은 Fuel)의 수, Fuel인 경우에는 x10하여 계산함
	AVECTOR3		TargetPosition;		// 공격 클라이언트의 화면에서의 타켓 포지션
	USHORT			MultiTargetIndex;	// 2011-03-21 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
	BYTE			Distance;
	BYTE			SecAttackType;		// 2형 무기 공격 타입: SEC_ATT_TYPE_XXX, see below
	AVECTOR3		AttackPosition;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터 위치
};

struct MSG_FN_BATTLE_ATTACK_RESULT_SECONDARY
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	UINT			WeaponItemNumber;	// 무기 타입(각 무기종류,스킬종류)
	USHORT			WeaponIndex;		// 남은 총알(혹은 Fuel)의 수, Fuel인 경우에는 x10하여 계산함
	AVECTOR3		TargetPosition;		// 공격 클라이언트의 화면에서의 타켓 포지션
	BYTE			DamageKind;
	BYTE			Distance;
	BYTE			SecAttackType;		// 2형 무기 공격 타입: SEC_ATT_TYPE_XXX
};

struct MSG_FN_BATTLE_ATTACK_FIND
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	AVECTOR3		TargetPosition;		// 공격 클라이언트의 화면에서의 타켓 포지션
	USHORT			WeaponIndex;		// 클라이언트에서 발사된 총알의 인덱스
	ChannelIndex_t	ChannelIndex;
};

struct MSG_FN_BATTLE_ATTACK_FIND_RESULT
{
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	USHORT			WeaponIndex;		// 클라이언트에서 발사된 총알의 인덱스
	BYTE			DamageKind;
};

struct MSG_FN_BATTLE_DROP_FIXER
{
	ChannelIndex_t	ChannelIndex;		// check_cmkwon, 추가하기~, 20040330, kelovon
	ClientIndex_t	AttackIndex;		// Attack Character
	ClientIndex_t	TargetIndex;		// Target Monster
	INT				ItemNum;			// Fixer의 ItemNum
};				// F -> N

struct MSG_FN_BATTLE_DROP_FIXER_OK
{
	ChannelIndex_t	ChannelIndex;		// check_cmkwon, 추가하기~, 20040330, kelovon
	ClientIndex_t	AttackIndex;		// Attack Character
	ClientIndex_t	TargetIndex;		// Target Monster
	UINT			ItemFieldIndex;
	INT				ItemNum;			// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
};			// N -> F

struct MSG_FN_BATTLE_ATTACK_HIDE_ITEM_W_KIND : public MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND
{
	ChannelIndex_t	ChannelIndex;
};

struct MSG_FN_BATTLE_SET_ATTACK_CHARACTER
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;
	float			DamageAmount;
	BYTE			ItemKind;
};

///////////////////////////////////////////////////////////////////////////////
// Party(편대, 파티) 관련 프로토쿨
///////////////////////////////////////////////////////////////////////////////

// IMServer에서 FieldServer로 넘겨주어야 하는 정보
struct MEX_FIELD_PARTY_INFO
{
	INT		nTotalPartyMember;			// 총 파티원 수
	BYTE	lowestMemberLevel;			// 최하 파티원 level, check: FieldServer가 IMServer로 사용자의 level 정보 등 update하는 부분 구현해야 함! 20031101, kelovon
};

// 파티 생성
struct MSG_IC_PARTY_CREATE
{
	UID32_t		CharacterUniqueNumber;		// 생성자(파티장) 번호
	SPARTY_INFO	PartyInfo;					// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	DWORD		Padding[SIZE_MAX_PACKET_PADDING];					// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_FI_PARTY_CREATE_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;	// 생성자(파티장) 번호
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
	BYTE					ExpDistributeType;		// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 경험치 분배 타잎
	BYTE					ItemDistributeType;		// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 아이템 분배 타잎
};

struct MSG_FC_PARTY_CREATE_OK
{
	PartyID_t	PartyID;
};

#ifdef NEMERIAN_TEAMSPEAK
typedef struct
{
	unsigned long long ChannelID;
} MSG_FC_TEAMSPEAK_MOVE_CLIENT;
#endif

// 파티원 초대
struct MSG_FC_PARTY_REQUEST_INVITE
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];		// 초대할 상대방
};

struct MSG_FC_PARTY_REQUEST_INVITE_QUESTION
{
	PartyID_t	PartyID;
	char		MasterCharacterName[SIZE_MAX_CHARACTER_NAME];	// 파티장
};

using MSG_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA = MSG_FC_PARTY_REQUEST_INVITE_QUESTION;	// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 파티에 가입시킬 클라에게 파티 가입 준비 패킷 보내기.

struct MSG_FC_PARTY_ACCEPT_INVITE
{
	PartyID_t	PartyID;
};

struct MSG_FI_PARTY_ACCEPT_INVITE_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;		// 새로 가입할 파티원
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;				// 이 MSG가 F->I일 때는 무시함
};						// F->I, I->F 둘 다 쓰임

						//////////////////////////////////////////////////////////////////////
						// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 팀번호 보내기
struct MSG_FI_PARTY_ARENA_TEAM_NUM
{
	UID32_t					CharacterUniqueNumber;		/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : IM에서 사용자 소켓 찾을때 쓰는 CUID
	int						TeamNum;					/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : IM에서 같은 팀인지 체크할 팀 번호
};
// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 팀번호 보내기


struct IMPartyMember;

struct IM_PARTY_MEMBER_INFO
{
	UID32_t		CharacterUniqueNumber;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		NameColor;
	BYTE		PilotFace;
	USHORT		UnitKind;
	USHORT		Race;
	BYTE		Level;
	//char		MapName[SIZE_MAX_MAP_NAME];	// IS_VALID_MAP_NAME()이 FALSE이면, 파티 소속이나 게임중이 아닌 캐릭임(죽어서 나간 캐릭터 등)
	MAP_CHANNEL_INDEX	MapChannelIndex;	// IsValid()이 FALSE이면, 파티 소속이나 게임중이 아닌 캐릭임(죽어서 나간 캐릭터 등)
	EN_CHECK_TYPE	VoipType;		// 2008-07-16 by dhjin, EP3 - Voip 정보

									// operator overloading
	IM_PARTY_MEMBER_INFO& operator=(const IMPartyMember& rhs);
	inline IM_PARTY_MEMBER_INFO& operator=(const CHARACTER& rhs)
	{
		this->CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		this->PilotFace = rhs.PilotFace;
		this->UnitKind = rhs.UnitKind;
		this->Race = rhs.Race;
		this->Level = rhs.Level;
		util::strncpy(this->CharacterName, rhs.CharacterName, SIZE_MAX_CHARACTER_NAME);
		NameColor = rhs.Color;
		this->MapChannelIndex = rhs.MapChannelIndex;
		return *this;
	}
};										// IM Server가 항상 유지해야 하는 정보

struct MSG_IC_PARTY_ACCEPT_INVITE_OK
{
	PartyID_t				PartyID;
	IM_PARTY_MEMBER_INFO	IMPartyMemberInfo;
};

struct MSG_FC_PARTY_REJECT_INVITE
{
	PartyID_t	PartyID;
};

struct MSG_FC_PARTY_REJECT_INVITE_OK
{
	PartyID_t	PartyID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];		// 초대했던 상대방
};

// 파티원 정보(from IM Server)
struct MSG_IC_PARTY_GET_MEMBER
{
	UID32_t		CharacterUniqueNumber;
};					// 같은 파티인지는 서버에서 확인한다

struct MSG_IC_PARTY_PUT_MEMBER
{
	IM_PARTY_MEMBER_INFO	IMPartyMemberInfo;
};

struct MSG_IC_PARTY_GET_ALL_MEMBER
{
	PartyID_t	PartyID;
};

struct MSG_IC_PARTY_PUT_ALL_MEMBER
{
	PartyID_t	PartyID;
	UID32_t		MasterUniqueNumber;			// 파티장의 CharacterUniqueNumber
	UINT		nNumOfPartyMembers;
	ARRAY_(IM_PARTY_MEMBER_INFO);
};

// 파티원 정보(from Field Server)
struct FIELD_PARTY_MEMBER_INFO
{
	UID32_t			CharacterUniqueNumber;
	ClientIndex_t	ClientIndex;
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	//char			MapName[SIZE_MAX_MAP_NAME];	// IS_VALID_MAP_NAME()이 FALSE이면, 파티 소속이나 게임중이 아닌 캐릭임(죽어서 나간 캐릭터 등)
	MAP_CHANNEL_INDEX	MapChannelIndex;	// IsValid()이 FALSE이면, 파티 소속이나 게임중이 아닌 캐릭임(죽어서 나간 캐릭터 등)
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float			CurrentHP;
	float			CurrentDP;
	SHORT			SP;
	SHORT			CurrentSP;
	SHORT			EP;
	float			CurrentEP;

	// operator overloading
	FIELD_PARTY_MEMBER_INFO& operator=(const CHARACTER& rhs)
	{
		this->CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		this->ClientIndex = rhs.ClientIndex;
		this->HP = rhs.HP;
		this->CurrentHP = rhs.CurrentHP;
		this->DP = rhs.DP;
		this->CurrentDP = rhs.CurrentDP;
		this->SP = rhs.SP;
		this->CurrentSP = rhs.CurrentSP;
		this->EP = rhs.EP;
		this->CurrentEP = rhs.CurrentEP;
		util::strncpy(this->CharacterName, rhs.CharacterName, SIZE_MAX_CHARACTER_NAME);
		this->MapChannelIndex = rhs.MapChannelIndex;
		return *this;
	}
};

struct MSG_FC_PARTY_GET_MEMBER
{
	UID32_t		CharacterUniqueNumber;
};					// 같은 파티인지는 서버에서 확인한다

struct MSG_FC_PARTY_PUT_MEMBER
{
	FIELD_PARTY_MEMBER_INFO	FieldMemberInfo;
};

struct MSG_FC_PARTY_GET_ALL_MEMBER
{
	PartyID_t	PartyID;
};

struct MSG_FC_PARTY_PUT_ALL_MEMBER
{
	UID32_t		MasterUniqueNumber;			// 파티장의 CharacterUniqueNumber
	UINT		nNumOfPartyMembers;
	ARRAY_(MSG_FC_PARTY_PUT_MEMBER);
};

// 파티원 정보 업데이트
struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL
{
	UID32_t		CharacterUniqueNumber;
	BYTE		Level;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float		CurrentHP;
	float		CurrentDP;
	SHORT		SP;
	SHORT		CurrentSP;
	SHORT		EP;
	float		CurrentEP;
	BodyCond_t	BodyCondition;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL
{
	UID32_t		CharacterUniqueNumber;
	BYTE		Level;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP
{
	UID32_t		CharacterUniqueNumber;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
#else
	SHORT	HP;
#endif
	float		CurrentHP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP
{
	UID32_t		CharacterUniqueNumber;
	float		CurrentHP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP
{
	UID32_t		CharacterUniqueNumber;
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		DP;
#else
	SHORT	DP;
#endif
	float		CurrentDP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP
{
	UID32_t		CharacterUniqueNumber;
	float		CurrentDP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP
{
	UID32_t		CharacterUniqueNumber;
	SHORT		SP;
	SHORT		CurrentSP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP
{
	UID32_t		CharacterUniqueNumber;
	SHORT		CurrentSP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP
{
	UID32_t		CharacterUniqueNumber;
	SHORT		EP;
	float		CurrentEP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP
{
	UID32_t		CharacterUniqueNumber;
	float		CurrentEP;
};

struct MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION
{
	UID32_t		CharacterUniqueNumber;
	BodyCond_t	BodyCondition;
};

struct MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME
{
	UID32_t		CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};		// 워프시 맵 이름 업데이트

		// 추방
struct MSG_IC_PARTY_BAN_MEMBER
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
};

struct MSG_IC_PARTY_BAN_MEMBER_OK
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
};

struct MSG_FI_PARTY_BAN_MEMBER_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};

// 탈퇴
struct MSG_IC_PARTY_LEAVE
{
	PartyID_t	PartyID;
};

struct MSG_IC_PARTY_LEAVE_OK
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
	bool		bMoveToArena;
};

struct MSG_FI_PARTY_LEAVE_OK
{
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};

// 파티장 권한 양도
struct MSG_IC_PARTY_TRANSFER_MASTER
{
	PartyID_t	PartyID;
	UID32_t		OldMasterCharacterUniqueNumber;
	UID32_t		NewMasterCharacterUniqueNumber;
};

struct MSG_IC_PARTY_TRANSFER_MASTER_OK
{
	PartyID_t	PartyID;
	UID32_t		OldMasterCharacterUniqueNumber;
	UID32_t		NewMasterCharacterUniqueNumber;
};

struct MSG_FI_PARTY_TRANSFER_MASTER_OK
{
	PartyID_t	PartyID;
	UID32_t		OldMasterCharacterUniqueNumber;
	UID32_t		NewMasterCharacterUniqueNumber;
};

// 해산
struct MSG_IC_PARTY_DISMEMBER
{
	PartyID_t	PartyID;
};

struct MSG_IC_PARTY_DISMEMBER_OK
{
	PartyID_t	PartyID;
};

struct MSG_FI_PARTY_DISMEMBER_OK
{
	PartyID_t	PartyID;
};

// 편대 비행 요청
struct MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION
{
	PartyID_t	PartyID;
	BYTE		Formation;				// 편대 비행 형태, see below
};	// Cm -> I

	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - AtumParam.h로 옮김
	// #define FLIGHT_FORM_NONE				(BYTE)0	// 편대 비행 안 함
	// #define FLIGHT_FORM_2_COLUMN			(BYTE)1	// 이렬 종대, 이렬 종대 모양으로 두 줄로 나란히 선 모양이다
	// #define FLIGHT_FORM_2_LINE				(BYTE)2	// 이렬 횡대, 이렬 횡대 모양으로 두 줄로 나란히 선 모양이다
	// #define FLIGHT_FORM_TRIANGLE			(BYTE)3	// 삼각 편대, 삼각형 모양으로 상단부터 1, 2, 3개의 유닛이 위치한다
	// #define FLIGHT_FORM_INVERTED_TRIANGLE	(BYTE)4	// 역삼각 편대, 역 삼각형 모양으로 상단부터 3, 2, 1개의 유닛이 위치한다
	// #define FLIGHT_FORM_BELL				(BYTE)5	// 종 형태, 종 모양으로 상단부터 1, 3, 2개의 유닛이 위치한다
	// #define FLIGHT_FORM_INVERTED_BELL		(BYTE)6	// 역종 형태, 역종 모양으로 상단부터 2, 3, 1개의 유닛이 위치한다
	// #define FLIGHT_FORM_X					(BYTE)7 // X자 형태
	// #define FLIGHT_FORM_STAR				(BYTE)8	// 별 형태

struct MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK
{
	PartyID_t	PartyID;
	BYTE		Formation;					// 편대 비행 형태
};	// I -> C, 파티장에게도 보냄

struct MSG_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK
{
	PartyID_t	PartyID;
	BYTE		Formation;					// 편대 비행 형태
};	// I -> C, 파티장에게도 보냄

struct MSG_IC_PARTY_GET_FLIGHT_POSITION
{
	UID32_t		CharacterUniqueNumber;
};			// C -> I -> Cm

struct MSG_IC_PARTY_CHANGE_FLIGHT_POSITION
{
	UID32_t		CharacterUniqueNumber;
	BYTE		FlightPosition;				// 자기 자신의 편대 비행 위치
};		// Cm -> I -> C

struct MSG_FI_PARTY_CHANGE_FLIGHT_POSITION
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
	BYTE		FlightPosition;				// 자기 자신의 편대 비행 위치
};		// Cm -> I -> C

		// 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
#define FORMATION_SKILL_NULL	0	// 포메이션 값을 사용하지 않음
#define FORMATION_SKILL_ON		1	// 포메이션 사용
#define FORMATION_SKILL_OFF		2	// 포메이션 사용 중이 아님
		// END 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정

struct MSG_IC_PARTY_CANCEL_FLIGHT_POSITION
{
	// 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
	bool		Formation_On_Off;
	// end 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
	UID32_t		CharacterUniqueNumber;
};		// C -> I -> Cm

struct MSG_FI_PARTY_CANCEL_FLIGHT_POSITION
{
	PartyID_t	PartyID;
	UID32_t		CharacterUniqueNumber;
};		// C -> I -> Cm

struct MSG_IC_PARTY_MEMBER_INVALIDATED
{
	UID32_t		CharacterUniqueNumber;
};			// I -> C, 파티원이 비정상적으로 게임에서 튕겼을 때 전송

struct MSG_IC_PARTY_MEMBER_REJOINED
{
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// I -> C, 파티원이 다시 게임을 시작하였을 때 전송, 자신은 제외함

struct MSG_IC_PARTY_UPDATE_ITEM_POS {
	UID32_t			CharacterUniqueNumber;	// 아이템 창작이 갱신된 파티원
	BYTE			ItemPosition;			// POS_XXX
	INT				ItemNum;
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 변경
	//	INT				ColorCode;				// 2005-12-08 by cmkwon, 아머 색상 튜닝 정보
	INT				nShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	INT				nEffectItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
};				// I->C, 파티원이 아이템 장착을 수정했을 때 전송

struct MSG_IC_PARTY_ALL_FLIGHT_POSITION
{
	UID32_t			AllPartyMemberCharacterUniqueNumber[SIZE_MAX_PARTY_MEMBER - 1];
};

struct MSG_IC_PARTY_LIST_INFO
{// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 
	PartyID_t		StartNum;
};

struct MSG_IC_PARTY_JOIN_FREE
{// 2008-06-03 by dhjin, EP3 편대 수정 - 편대 자유 참여
	PartyID_t		PartyNum;
	CHAR			PartyPW[SIZE_MAX_TEAM_PW];
};

struct SPARTY_LIST_INFO
{// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 OK
	PartyID_t	PartyNum;
	CHAR		PartyName[SIZE_MAX_PARTY_NAME];
	BYTE		ExpDistributeType;
	BYTE		ItemDistributeType;
	bool		PartyLock;
	CHAR		PartyPW[SIZE_MAX_TEAM_PW];
	INT			MinLevel;
	INT			MaxLevel;
	CHAR		PartyMasterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		Membercount;
	bool		VoipCheck; // 2017-01-16 panoskj todo : Remove
#ifdef C_RENDER_PARTY_MG_COUNT
	INT			MGCount;
#endif
};

struct MSG_IC_PARTY_LIST_INFO_OK
{// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 OK
	INT				PartyInfoTotalCount;	// 2008-06-02 by dhjin, 파티 최대 목록
	INT				PartyInfoListCount;		// 2008-06-02 by dhjin,
	_ARRAY(SPARTY_LIST_INFO);
};

struct MSG_IC_PARTY_CHANGE_INFO
{// 2008-06-04 by dhjin, EP3 편대 수정 - 편대 자유 참여
	SPARTY_INFO	PartyInfo;
};
using MSG_IC_PARTY_INFO = MSG_IC_PARTY_CHANGE_INFO;

struct SRECOMMENDATION_MEMBER_INFO
{
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t		CharacterUID;
};

struct MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK
{// 2008-06-04 by dhjin, EP3 편대 수정 - 추천 케릭터 요청
	UINT		Count;
	ARRAY_(SRECOMMENDATION_MEMBER_INFO);
};


// check: FI_EVENT_GET_WARP_INFO(파티 정보+이벤트 정보)로 대체한다. 순수 파티 정보만 받아와야할 필요성이 생기면 살린다. 20031006.
// 다른 필드 서버(파티 정보를 가지지 않은)로의 워프시, IM 서버로부터 정보를 받아 파티 정보 생성
//typedef struct
//{
//	UID32_t			CharacterUniqueNumber;
//} MSG_FI_PARTY_GET_PARTY_INFO;
//
//typedef struct
//{
//	UID32_t			AccountUniqueNumber;		// 다른 필드 서버로의 워프시, 인증에 필요함
//	UID32_t			CharacterUniqueNumber;
//	PartyID_t		PartyID;
//	UID32_t			MasterCharacterUniqueNumber;
//	int				NumOfPartyMembers;
//	ARRAY_(FI_PARTY_MEMBER_INFO);
//} MSG_FI_PARTY_GET_PARTY_INFO_OK;

struct FI_PARTY_MEMBER_INFO
{
	UID32_t			CharacterUniqueNumber;
	//	char			CurrentMapName[SIZE_MAX_MAP_NAME];	// check: 다른 필드서버에 있는 파티원은 관리하지 않기로 하면서 제거, 20031010, 확정&구현되면 삭제 가능
};	// Field Server와 IM Server 사이에 사용하는 파티원 정보

	// 편대 비행중인 파티원 워프
struct MSG_FC_PARTY_REQUEST_PARTY_WARP
{
	int				nPartyMembers;				// 같이 워프할 파티원의 수, 편대장 제외
	ARRAY_(UID32_t);							// 파티원 character uniquenumber의 array
};

struct MSG_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	int					nPartyMembers;				// 같이 워프할 파티원의 수, 편대장 제외
	ARRAY_(UID32_t);								// 파티원 character uniquenumber의 array
};

//typedef struct
//{
//	INT				ObjectIndex;				// Object의 Index
//	int				nPartyMembers;				// 같이 워프할 파티원의 수, 편대장 제외
//	ARRAY_(UID32_t);							// 파티원 character uniquenumber의 array
//} MSG_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT;
struct MSG_FC_PARTY_REQUEST_PARTY_OBJECT_EVENT
{
	DWORD			ObjectType;
	AVECTOR3		ObjectPosition;
	int				nPartyMembers;				// 같이 워프할 파티원의 수, 편대장 제외
	ARRAY_(UID32_t);							// 파티원 character uniquenumber의 array
};

// 파티원의 MOVE 정보 요청
struct MSG_FC_PARTY_GET_OTHER_MOVE
{
	UID32_t			OtherCharacterUniqueNumber;
};

// 게임에서 나갔다가 들어왔을 때 편대 유지 관련
struct MSG_IC_PARTY_PUT_LAST_PARTY_INFO
{
	PartyID_t	PartyID;				// 최근에 몸담았던 파티가 로긴하였을 때 존재하므로, 그에 대한 준비 요청
};		// I -> C, 파티원이 다시 게임을 시작하였을 때 전송, 자기 자신에게만 보냄

		// 파티전 관련
struct MSG_FC_PARTY_BATTLE_START
{
	PartyID_t		PeerPartyID;				// 상대 PartyID
	ClientIndex_t	PeerPartyMasterClientIndex;	// 파티장의 ClientIndex
	SHORT			nPeerPartyMemberToBattle;	// 파티전에 참여할 상대 파티원의 수
	ARRAY_(PEER_PARTY_MEMBER);
};					// F->C, 상대 파티의 정보를 보낸다.

struct PEER_PARTY_MEMBER
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
};

struct MSG_FC_PARTY_BATTLE_END
{
	PartyID_t		PeerPartyID;				// 상대방의 ClientIndex
	USHORT			EndType;					// 결투 종료 타입, BATTLE_END_XXX
};						// F->C, 파티전 결과

struct MSG_FI_PARTY_NOTIFY_BATTLE_PARTY
{
	PartyID_t		PartyID1;					// 파티 1
	PartyID_t		PeerPartyID1;				// 파티 1의 대상 파티
	PartyID_t		PartyID2;					// 파티 2
	PartyID_t		PeerPartyID2;				// 파티 2의 대상 파티
};				// F->I, 파티전을 알림

struct MSG_FI_PARTY_NOTIFY_BATTLE_PARTY_OK
{
	PartyID_t		PartyID1;					// 파티 1
	PartyID_t		PeerPartyID1;				// 파티 1의 대상 파티
	PartyID_t		PartyID2;					// 파티 2
	PartyID_t		PeerPartyID2;				// 파티 2의 대상 파티
};			// I->F, 파티전을 알림에 대한 ACK

struct MSG_FC_PARTY_PUT_ITEM_OTHER {
	INT				ItemNum;					// 아이템의 종류
	USHORT			Amount;						// 아이템의 개수
	ClientIndex_t	ClientIndex;
};					// F->C, 다른 파티원의 아이템 취득 정보 전송

struct MSG_FI_PARTY_ADD_MEMBER {
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;	// 추가할 파티원
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};						// I->F, 파티원을 추가하라고 Field Server 알림

struct MSG_FI_PARTY_DELETE_MEMBER {
	PartyID_t				PartyID;
	UID32_t					CharacterUniqueNumber;	// 제거할 파티원
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};					// I->F, 파티원을 제거하라고 Field Server 알림

struct MSG_FI_PARTY_UPDATE_ITEM_POS {
	UID32_t			CharacterUniqueNumber;	// 아이템 창작이 갱신된 파티원
	BYTE			ItemPosition;			// POS_XXX
	INT				ItemNum;
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 변경
	//	INT				ColorCode;				// 2005-12-08 by cmkwon, 아머 색상 튜닝 정보
	INT				nShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	INT				nEffectItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
};				// F->I, 파티원이 아이템 장착을 수정했을 때 전송

struct MSG_FI_PARTY_ALL_FLIGHT_POSITION
{
	PartyID_t		PartyID;
	UID32_t			AllPartyMemberCharacterUniqueNumber[SIZE_MAX_PARTY_MEMBER - 1];
};

struct MSG_FI_PARTY_UPDATE_PARTY_INFO
{
	PartyID_t				PartyID;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
};	// I->F, 파티 정보를 업데이트

struct MSG_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE
{// 2008-06-04 by dhjin, EP3 편대 수정 - 경험치 분배 방식 변경 
	PartyID_t	PartyID;
	BYTE		ExpDistributeType;
};

struct MSG_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE
{// 2008-06-04 by dhjin, EP3 편대 수정 - 아이템 분배 방식 변경 
	PartyID_t	PartyID;
	BYTE		ItemDistributeType;
};

struct MSG_FI_PARTY_CHANGE_FORMATION_SKILL		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
{
	UID32_t		PartyMasterCharcUID;	// PartyMaster CharacterUID
	BYTE		Is_On_or_Off;			// TRUE is ON, FALSE is Off
	BYTE		FlightFormation;		// 
};

///////////////////////////////////////////////////////////////////////////////
// FI_CHARACTER
struct MSG_FI_CHARACTER_DELETE_CHARACTER
{
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	UID32_t	GuildUniqueNumber;
};

struct MSG_FI_CHARACTER_CHANGE_LEVEL
{
	UID32_t	CharacterUniqueNumber;
	BYTE	Level;
};

struct MSG_FI_CHARACTER_UPDATE_GUILD_INFO
{
	UID32_t	CharacterUniqueNumber;
	char	GuildName[SIZE_MAX_GUILD_NAME];	// 길드 이름
	UID32_t	GuildUniqueNumber;				// 길드 번호, 0이면 길드 없음
	bool	GuildDelete;					// 2006-09-29 by dhjin, 길드 삭제 정보, 1->삭제
};


// 2007-10-06 by dhjin, 부지도자가 여단 생성시 InflWarManager 클래스에 부지도자 여단을 설정.
struct MSG_FI_CREATE_GUILD_BY_SUBLEADER
{
	unsigned char	Influence;
	int		SubLeaderRank;
	UID32_t CharacterUID;
};

struct MSG_FI_CHARACTER_UPDATE_MAP_CHANNEL
{
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannel;
};

struct MSG_FI_CHARACTER_CHANGE_INFLUENCE_TYPE
{
	UID32_t				CharacterUID;
	BYTE				InfluenceType0;					// 
};

struct MSG_FI_UPDATE_SUBLEADER
{// 2007-02-14 by dhjin, 부지도자 설정 시 IM서버로 정보 전송.
	UID32_t				CharacterUID;
	BYTE				SubLeaderNum;
	BYTE				InfluenceType;
};

///////////////////////////////////////////////////////////////////////////////
// FC_GUILD (Cleint <-> Field Server)
struct MSG_FC_GUILD_GET_MAP_OWNER_INFO
{
	MapIndex_t	MapIndex;
};			// C->F, 맵 소유 정보 요청

struct MSG_FC_GUILD_GET_MAP_OWNER_INFO_OK
{
	char	DefenderGuildName[SIZE_MAX_GUILD_NAME];	// 점령 길드 이름
	BYTE	NumOfCallengerGuilds;					// 도전 길드 수
};		// F->C, 맵 소유 정보 요청 결과

struct MSG_FC_GUILD_REQUEST_GUILD_WAR
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
};			// C->F, 여단전 요청

struct MSG_FC_GUILD_REQUEST_GUILD_WAR_RESULT
{
	INT		Order;							// 순번(1부터 시작), 0 이하이면 신청 실패
};	// F->C, 여단전 요청 결과

struct MSG_FC_GUILD_GET_CHALLENGER_GUILD
{
	UID32_t	DefenderGuildUniqueNumber;		// 점령 길드 고유 번호
};		// C->F, 여단전 요청 대기 길드 리스트 요청

struct MSG_FC_GUILD_GET_CHALLENGER_GUILD_OK
{
	BYTE	NumOfCallengerGuilds;			// 도전 길드 이름 개수(우선 순위 순서)
	ARRAY_(char[SIZE_MAX_GUILD_NAME]);
};		// F->C, 여단전 요청 대기 길드 리스트 요청 결과

struct MSG_FC_GUILD_GET_WAR_INFO
{
	UID32_t		GuildUID;				// 자신이 속한 길드의 UID
};			// C->F, 여단전 정보를 요청

struct MSG_FC_GUILD_GET_WAR_INFO_OK
{
	MAP_CHANNEL_INDEX	MapChannel;		// 여단전이 일어나는 맵
	UID32_t				PeerGuildUID;	// 상대 길드의 길드 번호
};			// F->C, 여단전 정보를 전송

struct MSG_FC_GUILD_SUMMON_MEMBER
{
	UID32_t				uidGuildUID;	// 길드 UID
	MAP_CHANNEL_INDEX	MapChannel;		// 여단장이 있는 맵
	AVECTOR3			PositionVector;	// 여단장의 좌표
};
struct MSG_FC_GUILD_SUMMON_MEMBER_OK
{
	UID32_t				uidGuildUID;	// 길드 UID
	MAP_CHANNEL_INDEX	MapChannel;		// 여단장이 있는 맵
	AVECTOR3			PositionVector;	// 여단장의 좌표
};

///////////////////////////////////////////////////////////////////////////////
// FI_GUILD (Field Server <-> IM Server)
struct MSG_FI_GUILD_NOTIFY_START_WAR
{
	UID32_t		DefenderGuildUID;
	UID32_t		ChallengerGuildUID;
	MAP_CHANNEL_INDEX	WarMapChannel;
};	// F->I

struct MSG_FI_GUILD_NOTIFY_END_WAR
{
	USHORT		WarEndType;			// BATTLE_END_XXX, BATTLE_END_WIN or BATTLE_END_DEFEAT이면 승패 있음
	UID32_t		WinnerGuildUID;
	UID32_t		LoserGuildUID;
	MAP_CHANNEL_INDEX	WarMapChannel;
};		// F->I

struct MSG_FI_GUILD_DELETE_GUILD
{
	UID32_t		DeleteGuildUID;
};
using MSG_FI_GUILD_REG_DELETE_GUILD = MSG_FI_GUILD_DELETE_GUILD;		// 2007-11-09 by dhjin

struct MSG_FI_GUILD_ADD_GUILD_FAME	// 2005-12-27 by cmkwon
{
	UID32_t		guildUID;		// 
	int			addValues;		// 1 or 100
};

struct MSG_FI_GUILD_OUTPOST
{// 2008-05-21 by dhjin, EP3 - 여단 수정 사항 - 전진기지 관련
	UID32_t		GuildUID;
	MapIndex_t	MapIndex;
};

///////////////////////////////////////////////////////////////////////////////
// IC_GUILD (Cleint <-> IM Server)
struct MSG_IC_GUILD_CREATE
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
};						// C->I, 길드 생성 요청

struct MSG_IC_GUILD_CREATE_OK
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t	GuildUniqueNumber;				// 길드 고유번호
	char	GuildCommanderUniqueNumber;		// 여단장
};					// I->C, 길드 생성 결과

struct MSG_IC_GUILD_GET_GUILD_INFO
{
	UID32_t	GuildUniqueNumber;				// 길드 고유번호
};				// C->I, 길드 정보 요청

				// GUILD_STATE_XXX
#define GUILD_STATE_NORMAL				((BYTE)0)	// 정상 길드 상태
#define GUILD_STATE_DISMEMBER_READY		((BYTE)1)	// 길드 해체 대기 상태
#define GUILD_STATE_IN_GUILD_WAR		((BYTE)2)	// 길드전 상태
#define GUILD_STATE_CITYWAR				((BYTE)3)	// 도시점령전 상태

				///////////////////////////////////////////////////////////////////////////////
				// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - GUILD_MARK_STATE_XXX 정의
#define GUILD_MARK_STATE_NONE					((BYTE)0)	// 마크 없는 상태
#define GUILD_MARK_STATE_WAITING_PERMISSION		((BYTE)1)	// 마크 허용을 기다리는 상태, 게임상에서는 사용 불가
#define GUILD_MARK_STATE_NORMAL					((BYTE)2)	// 마크 사용 중인 상태
const char *GetStringGuildMarkState(BYTE i_byGuildMarkState, bool i_bForUser = false);


struct MSG_IC_GUILD_GET_GUILD_INFO_OK
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t	GuildUniqueNumber;				// 길드 고유번호
	UID32_t	GuildCommanderUniqueNumber;		// 여단장
	INT		GuildMemberCapacity;			// 길드 인원 제한
	BYTE	NumOfGuildMemberInfo;
	BYTE	GuildState;						// GUILD_STATE_XXX
	UINT	GuildMarkVersion;
	INT		WarWinPoint;					// 길드전 승수
	INT		WarLossPoint;					// 길드전 패수
	char    Notice[SIZE_MAX_NOTICE];		// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	MapIndex_t GuildOutPostCityMapIndex;	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	INT		GuildTotalFame;					// 2008-06-05 by dhjin, EP3 - 여단 수정 사항			
	INT		GuildMonthlyFame;				// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	INT		GuildTotalFameRank;				// 2008-06-05 by dhjin, EP3 - 여단 수정 사항	
	INT		GuildMonthlyFameRank;			// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	bool    GuildMemberShip;					// 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	ATUM_DATE_TIME GuildMemberShipExpireTime;	// 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	ARRAY_(MEX_GUILD_MEMBER_INFO);
};			// I->C, 길드 정보 결과

			// 길드원 계급, GUILD_RANK_XXX, BYTE
#define GUILD_RANK_PRIVATE_NULL			((BYTE)0)	// 무소속 대대원
#define GUILD_RANK_COMMANDER			((BYTE)1)	// 비행여단장, 길드장
#define GUILD_RANK_SUBCOMMANDER			((BYTE)2)	// 부여단장				// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
#define GUILD_RANK_SQUAD_LEADER_1		((BYTE)3)	// 제 1 비행대대장
#define GUILD_RANK_PRIVATE_1			((BYTE)4)	// 제 1 비행대대원
#define GUILD_RANK_SQUAD_LEADER_2		((BYTE)5)	// 제 2 비행대대장
#define GUILD_RANK_PRIVATE_2			((BYTE)6)	// 제 2 비행대대원
#define GUILD_RANK_SQUAD_LEADER_3		((BYTE)7)	// 제 3 비행대대장
#define GUILD_RANK_PRIVATE_3			((BYTE)8)	// 제 3 비행대대원
#define GUILD_RANK_SQUAD_LEADER_4		((BYTE)9)	// 제 4 비행대대장
#define GUILD_RANK_PRIVATE_4			((BYTE)10)	// 제 4 비행대대원
#define GUILD_RANK_SQUAD_LEADER_5		((BYTE)11)	// 제 5 비행대대장
#define GUILD_RANK_PRIVATE_5			((BYTE)12)	// 제 5 비행대대원

			// check: MSG_IC_GUILD_GET_GUILD_INFO로 통합, 필요하면 살림, 20040520, kelovon
			//typedef struct
			//{
			//	UID32_t	GuildUniqueNumber;		// 길드 고유번호
			//} MSG_IC_GUILD_GET_MEMBER;		// C->I, 길드원 정보 요청

class CGuildMember;

struct MEX_GUILD_MEMBER_INFO
{
	char	MemberName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	MemberUniqueNumber;
	BYTE	GuildRank;				// GUILD_RANK_XXX
	BYTE	IsOnline;				// 1: TRUE, 0: FALSE
	INT		UnitKind;				// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	BYTE	MemberLevel;			// 2008-05-20 by dhjin, EP3 - 여단 수정 사항	
	EN_CHECK_TYPE	VoipType;		// 2008-07-16 by dhjin, EP3 - Voip 정보

#ifdef _ATUM_IM_SERVER
									// operator overloading
	MEX_GUILD_MEMBER_INFO& operator=(const CGuildMember& rhs);
#endif // _ATUM_IM_SERVER
};

// check: MSG_IC_GUILD_GET_GUILD_INFO_OK로 통합, 필요하면 살림, 20040520, kelovon
//typedef struct
//{
//	UID32_t	GuildUniqueNumber;		// 길드 고유번호
//	BYTE	NumOfGuildMemberInfo;
//	ARRAY_(MEX_GUILD_MEMBER_INFO);
//} MSG_IC_GUILD_GET_MEMBER_OK;		// I->C, 길드원 정보 결과

class CGuild;

struct MEX_OTHER_GUILD_INFO
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t	GuildUniqueNumber;
	UINT	GuildMarkVersion;

#ifdef _ATUM_IM_SERVER
	// operator overloading
	MEX_OTHER_GUILD_INFO& operator=(const CGuild& rhs);
#endif // _ATUM_IM_SERVER
};

struct MSG_IC_GUILD_GET_OTHER_GUILD_INFO
{
	UID32_t	GuildUniqueNumber;
};

struct MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK
{
	MEX_OTHER_GUILD_INFO		OtherGuildMemberInfo;
};

struct MSG_IC_GUILD_REQUEST_INVITE
{
	char	InviteeCharacterName[SIZE_MAX_CHARACTER_NAME];		// 초대할 상대방 아이디
};		// C->I, 가입 권유, 서버에 요청

struct MSG_IC_GUILD_REQUEST_INVITE_QUESTION
{
	UID32_t	GuildUniqueNumber;										// 길드 고유 번호
	char	GuildName[SIZE_MAX_GUILD_NAME];							// 길드 이름
	char	GuildCommanderCharacterName[SIZE_MAX_CHARACTER_NAME];	// 길드장 아이디
};		// I->C, 가입 권유, 대상자에게 전송

struct MSG_IC_GUILD_ACCEPT_INVITE
{
	UID32_t	GuildUniqueNumber;		// 길드 고유 번호, MSG_IC_GUILD_REQUEST_INVITE_QUESTION의 값 
};		// C->I, 가입 승낙, 서버에 요청

struct MSG_IC_GUILD_ACCEPT_INVITE_OK
{
	MEX_GUILD_MEMBER_INFO	MemberInfo;		// 새 길드원의 정보
};			// I->C, 가입 승낙, 대상자 및 길드원에게 전송

struct MSG_IC_GUILD_REJECT_INVITE
{
	UID32_t	GuildUniqueNumber;		// 길드 고유 번호, MSG_IC_GUILD_REQUEST_INVITE_QUESTION의 값
};		// C->I, 가입 거부, 서버에 요청

struct MSG_IC_GUILD_REJECT_INVITE_OK
{
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];	// 가입 거부한 character의 이름
};	// I->C, 가입 거부, 대상자에게 전송

struct MSG_IC_GUILD_BAN_MEMBER
{
	UID32_t	MemberUniqueNumber;
};			// C->I, 길드원 추방

struct MSG_IC_GUILD_BAN_MEMBER_OK
{
	UID32_t	MemberUniqueNumber;
};		// I->C, 길드원 추방 결과

struct MSG_IC_GUILD_LEAVE
{
	UID32_t	GuildUniqueNumber;
};				// C->I, 여단 탈퇴

struct MSG_IC_GUILD_LEAVE_OK
{
	UID32_t	MemberUniqueNumber;
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];				// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};			// I->C, 여단 탈퇴 결과

struct MSG_IC_GUILD_DISMEMBER
{
	UID32_t	GuildUniqueNumber;
};			// C->I, 여단 해체
using MSG_FC_GUILD_DISMEMBER = MSG_IC_GUILD_DISMEMBER;
using MSG_FI_GUILD_DISMEMBER = MSG_IC_GUILD_DISMEMBER;


struct MSG_IC_GUILD_DISMEMBER_OK
{
	char	GuildName[SIZE_MAX_GUILD_NAME];
};		// I->C, 여단 해체 결과

		// GUILD_MEMBER_STATE_XXX
#define GUILD_MEMBER_STATE_OFFLINE		((BYTE)0)
#define GUILD_MEMBER_STATE_ONLINE		((BYTE)1)

struct MSG_IC_GUILD_SET_MEMBER_STATE
{
	UID32_t	MemberUniqueNumber;
	BYTE	GuildMemberState;		// GUILD_MEMBER_STATE_XXX
};	// I->C, 길드원의 상태 변화(ONLINE, OFFLINE 등)

struct MSG_IC_GUILD_CANCEL_DISMEMBER
{
	UID32_t	GuildUniqueNumber;
};		// C->I, 여단 해체 취소 요청

struct MSG_IC_GUILD_CANCEL_DISMEMBER_OK
{
	BYTE	GuildState;					// GUILD_STATE_XXX
};		// I->C, 여단 해체 취소 결과

struct MSG_IC_GUILD_GET_DISMEMBER_DATE
{
	UID32_t	GuildUniqueNumber;
};		// C->I, 길드 해산 시간 요청

struct MSG_IC_GUILD_GET_DISMEMBER_DATE_OK
{
	ATUM_DATE_TIME	DismemberDateTime;
};	// C->I, 길드 해산 시간 결과

struct MSG_IC_GUILD_CHANGE_GUILD_NAME
{
	char	NewGuildName[SIZE_MAX_GUILD_NAME];
};		// C->I, 여단 이름 변경 요청

struct MSG_IC_GUILD_CHANGE_GUILD_NAME_OK
{
	char	NewGuildName[SIZE_MAX_GUILD_NAME];
};	// I->C, 여단 이름 변경 결과

struct MSG_IC_GUILD_GET_GUILD_MARK
{
	UID32_t	GuildUniqueNumber;
};			// C->I, 여단 문양 요청

			// MSG_SEQUENCE_XXX
#define MSG_SEQUENCE_SINGLE		((BYTE)0)	// 단독 MSG
#define MSG_SEQUENCE_BEGIN		((BYTE)1)	// LONG MSG의 시작
#define MSG_SEQUENCE_MIDDLE		((BYTE)2)	// LONG MSG의 중간
#define MSG_SEQUENCE_END		((BYTE)3)	// LONG MSG의 끝

struct MSG_IC_GUILD_GET_GUILD_MARK_OK
{
	UID32_t	GuildUniqueNumber;
	UINT	GuildMarkVersion;
	UINT	SizeOfGuildMark;			// 길드 문양의 size <= SIZE_MAX_GUILD_MARK_IMAGE
	ARRAY_(char);						// 길드 문양, image(bmp, gif, jpg, ...)
};		// I->C, 여단 문양 결과

struct MSG_IC_GUILD_SET_GUILD_MARK
{
	UINT	SizeOfGuildMark;			// 길드 문양의 size <= SIZE_MAX_GUILD_MARK_IMAGE
	ARRAY_(char);						// 길드 문양, image(bmp, gif, jpg, ...)
};			// C->I, 여단 문양 성정 요청

struct MSG_IC_GUILD_SET_GUILD_MARK_OK
{
	UINT	GuildMarkVersion;
	UINT	SizeOfGuildMark;			// 길드 문양의 size <= SIZE_MAX_GUILD_MARK_IMAGE
	ARRAY_(char);						// 길드 문양, image(bmp, gif, jpg, ...)
};		// I->C, 여단 문양 성정 결과

struct MSG_IC_GUILD_SET_RANK
{
	UID32_t	MemberUniqueNumber;
	BYTE	GuildRank;					// GUILD_RANK_XXX
};				// C->I, 계급 설정

struct MSG_IC_GUILD_SET_RANK_OK
{
	UID32_t	MemberUniqueNumber;
	BYTE	GuildRank;					// GUILD_RANK_XXX
};				// I->C, 계급 설정 결과

struct MSG_IC_GUILD_CHANGE_GUILD_STATE
{
	BYTE	GuildState;					// GUILD_STATE_XXX
};		// I->C, 여단 상태 전송

struct MSG_IC_GUILD_LOADING_GUILD_DONE
{
	UID32_t	GuildUniqueNumber;
};		// I->C, 서버측에서 길드 정보 로딩 완료 알림

struct MSG_IC_GUILD_WAR_READY
{
	UID32_t		PeerGuildUID;			// 상대 길드
	INT			WaitingTime;			// 대기 시간, 여기 명시된 시간 후에 여단전 시작
};				// I->C, 여단전 시작 대기 상태를 알림

struct MSG_IC_GUILD_START_WAR
{
	UID32_t		PeerGuildUID;			// 상대 길드
};				// I->C, 여단전 시작을 알림

struct MSG_IC_GUILD_END_WAR
{
	USHORT		WarEndType;				// 결과, BATTLE_END_XXX
	char		PeerGuildName[SIZE_MAX_GUILD_NAME];	// 상대 길드 이름
};					// I->C, 여단전 종료를 알림

struct MSG_IC_GUILD_UPDATE_WAR_POINT
{
	INT			WarWinPoint;			// 길드전 승수
	INT			WarLossPoint;			// 길드전 패수
};

struct MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY
{
	UID32_t		guildUID;				// 길드 UID
	INT			nMemberCapacity;		// 여단 제한 인원
	INT			IncreaseCapacity;		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
};

struct MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK
{
	UID32_t	GuildUniqueNumber;				// 길드 고유번호
	BYTE	NumOfGuildMemberInfo;
	ARRAY_(MEX_GUILD_MEMBER_INFO);
};

struct MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY		// 2006-08-09 by cmkwon
{
	USHORT		WarEndType;								// 결과, BATTLE_END_XXX
	char		WinerGuildName[SIZE_MAX_GUILD_NAME];	// 승리 길드 이름
	char		LoserGuildName[SIZE_MAX_GUILD_NAME];	// 패배 길드 이름
};

struct MSG_IC_GUILD_MEMBER_LEVEL_UP
{// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단원 레벨업 관련
	UID32_t		CharacterUID;
	INT			Level;
};

struct MSG_IC_GUILD_NEW_COMMANDER
{// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
	UID32_t		NewCommanderUID;
};

struct MSG_IC_GUILD_NOTICE_WRITE
{// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
	char		Notice[SIZE_MAX_NOTICE];
};
using MSG_IC_GUILD_NOTICE_WRITE_OK = MSG_IC_GUILD_NOTICE_WRITE;

struct MSG_IC_GUILD_GET_INTRODUCTION_OK
{// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 관리
	char		GuildIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_GET_APPLICANT_OK
{// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 관리
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		UnitKind;
	INT			Level;
};

struct MSG_IC_GUILD_GET_SELF_INTRODUCTION
{// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 
	UID32_t		CharacterUID;
};

struct MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK
{// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 
	char		GuildName[SIZE_MAX_GUILD_NAME];
	char		SelfIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_SEARCH_INTRODUCTION_OK
{// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
	UID32_t			GuildUID;
	char			GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	WriteDate;
	char			GuildIntroduction[SIZE_MAX_NOTICE];
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_GUILD_UPDATE_INTRODUCTION
{// 2008-05-28 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 작성 
	char			GuildIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_UPDATE_SELFINTRODUCTION
{// 2008-05-28 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성
	UID32_t			TargetGuildUID;
	char			SelfIntroduction[SIZE_MAX_NOTICE];
};

struct MSG_IC_GUILD_CHANGE_FAME_RANK
{// 2008-06-10 by dhjin, EP3 - 여단 수정 사항 - 여단 명성 변경
	INT		GuildTotalFameRank;
	INT		GuildMonthlyFameRank;
};

struct MSG_IC_GUILD_APPLICANT_INVITE
{// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		UnitKind;
	INT			Level;
};

struct MSG_IC_GUILD_APPLICANT_REJECT_INVITE
{// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 거부 
	UID32_t		CharacterUID;
	char		GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_IC_GUILD_CHANGE_MEMBERSHIP
{// I->C, // 2008-06-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 맴버쉽 정보 전송
	bool			MemberShip;
	ATUM_DATE_TIME	MemberShipExpireTime;
};

///////////////////////////////////////////////////////////////////////////////
// FC_QUEST
struct MEX_QUEST_INFO						// Character의 퀘스트 정보
{
	INT				QuestIndex;				// 퀘스트 번호
	BYTE			QuestState;				// 완료, 진행중
	LONGLONG		QuestPlayTimeStamp;		// 퀘스트 시작 시의 TotalPlayTime
	//char			szCityWarServerGroupName[SIZE_MAX_SERVER_NAME];
};

struct MEX_QUEST_MONSTER_COUNT						// Character의 퀘스트 정보
{
	INT				QuestIndex;				// 퀘스트 번호
	INT				MonsterUniqueNumber;
	INT				Count;
};

struct MSG_FC_QUEST_REQUEST_START
{
	INT			QuestIndex;
	BYTE		QuestStartType;			// QUEST_START_TYPE_XXX
	BYTE		PassQuest;				// 퀘스트를 그냥 넘김, CQuest::IsDiscardable이 TRUE일 때만 처리함
	AVECTOR3	AVec3QuestPosition;		// 퀘스트 시작시 좌표, 2005-08-29 by cmkwon
	char		szCouponNumber[SIZE_MAX_COUPON_NUMBER];
};		// C->F, Quest 시작을 요청

struct MSG_FC_QUEST_REQUEST_START_RESULT
{
	MEX_QUEST_INFO	MexQuestInfo;	// 퀘스트 정보
	float			fVCNInflDistributionPercent;	// 2006-02-08 by cmkwon, 바이제니유 정규군 세력분포 - 세력선택 미션시만 사용됨
	float			fANIInflDistributionPercent;	// 2006-02-08 by cmkwon, 알링턴 정규군 - 세력선택 미션시만 사용됨
};	// F->C, Quest 시작을 승인, Client는 이 MSG를 받으면 Pre NPCTalk을 로딩한다

struct MSG_FC_QUEST_ACCEPT_QUEST
{
	INT		QuestIndex;
};	// C->F, Quest를 받아들임

struct MSG_FC_QUEST_CANCEL_QUEST
{
	INT		QuestIndex;
};	// C->F, Quest를 거절함(거절할 수 없는 Quest도 존재함)

	// check: no body, 필요하면 살림
	//typedef struct {
	//} MSG_FC_QUEST_LEAVE_NPC;		// C->F, NPC를 떠남
	//
	//typedef struct {
	//} MSG_FC_QUEST_LEAVE_NPC_OK;	// F->C, NPC를 떠남에 대한 ACK

struct MSG_FC_QUEST_REQUEST_SUCCESS
{
	INT			QuestIndex;
	INT			QuestResult;			// 퀘스트의 성적 혹은 점수, 0이면 무시, 10~13(A~D)이면 넘겨주기, check: 현재는 받은대로 넘겨주기, 20040224, kelovon
	INT			SpentTimeInSeconds;		// 퀘스트 시작 후 지난 시간, 20041129, kelovon, 서버에서는 클라이언트 시간을 전적으로 믿음
	AVECTOR3	AVec3QuestPosition;		// 퀘스트 완료시 좌표, 2005-08-29 by cmkwon
};	// C->F, Quest 결과 검증을 요청

struct QUEST_PAY_ITEM_INFO
{
	INT		ItemNum;					// 2007-07-09 by dhjin, 보상 아이템 번호 
	INT		ItemCount;					// 2007-07-09 by dhjin, 보상 아이템 수
};

struct MSG_FC_QUEST_REQUEST_SUCCESS_RESULT
{
	INT		QuestIndex;
	bool	IsSuccessful;
	INT		QuestResult;		// 퀘스트의 성적 혹은 점수, 0이면 무시, 10~13(A~D)이면 넘겨주기, check: 현재는 받은대로 넘겨주기, 20040224, kelovon
	INT		ExpOfCompensation;			// 2007-03-06 by cmkwon, 퀘스트 완료시 실제로 추가된 보상 경험치
	BYTE	BonusStatOfCompensation;	// 2007-07-09 by dhjin, BonusStat 보상 정보 
	INT		QuestPayInfoListCount;		// 2007-07-09 by dhjin, 아이템 보상 수
	ARRAY_(QUEST_PAY_ITEM_INFO);
};	// F->C, Quest 결과(완료)를 알림, Client는 이 MSG를 받으면 quest를 종료하고 After NPCTalk을 로딩한다

struct MSG_FC_QUEST_PUT_ALL_QUEST
{
	float			fVCNInflDistributionPercent;	// 2006-02-08 by cmkwon, 바이제니유 정규군 세력분포 - 세력선택 미션시만 사용됨
	float			fANIInflDistributionPercent;	// 2006-02-08 by cmkwon, 알링턴 정규군 - 세력선택 미션시만 사용됨
	INT		NumOfQuest;
	ARRAY_(MEX_QUEST_INFO);		// QuestIndex의 array, see below
};			// F->C, 완료되거나 진행중인 모든 퀘스트를 전송, 게임 시작 시 아이템 전송 후 이어서 보냄

struct MSG_FC_QUEST_DISCARD_QUEST
{
	INT		QuestIndex;
};		// C->F, 이미 시작된 퀘스트를 포기함

struct MSG_FC_QUEST_DISCARD_QUEST_OK
{
	INT		QuestIndex;
	BYTE	IsDiscarded;			// 퀘스트 포기 성공 여부, 1: TRUE, 0:FALSE
};	// F->C, 이미 시작된 퀘스트를 포기함에 대한 결과

struct MSG_FC_QUEST_MOVE_QUEST_MAP
{
	INT		QuestIndex0;
};

struct MSG_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT
{
	INT		NumOfMonsterCount;
	ARRAY_(MEX_QUEST_MONSTER_COUNT);
};	// 2005-10-25 by cmkwon

using MSG_FC_QUEST_UPDATE_MONSTER_COUNT = MEX_QUEST_MONSTER_COUNT; 	// 2005-10-25 by cmkwon

using MSG_FC_QUEST_REQUEST_SUCCESS_CHECK = MSG_FC_QUEST_REQUEST_SUCCESS;			// 2006-03-24 by cmkwon

using MSG_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT = MSG_FC_QUEST_REQUEST_SUCCESS_RESULT;	// 2006-03-24 by cmkwon

struct MSG_FC_QUEST_REQUEST_PARTY_WARP			// 2006-10-16 by cmkwon
{
	UID32_t				callerCharacterUID;			// 호출자 CharcterUID
	DWORD				dwRemainTime;				// 단위:ms(ex> 1000 <== 1초)
	MAP_CHANNEL_INDEX	warpMapChannIndex;
};

struct MSG_FC_QUEST_REQUEST_PARTY_WARP_ACK		// 2006-10-16 by cmkwon
{
	UID32_t				callerCharacterUID;
	MAP_CHANNEL_INDEX	warpMapChannIndex;
};

///////////////////////////////////////////////////////////////////////////////
// FC_MONSTER (Client <-> Field server)

using MSG_FC_MONSTER_CREATED = MSG_FC_CHARACTER_GET_MONSTER_INFO_OK;

struct MSG_FC_MONSTER_MOVE_OK
{
	ClientIndex_t	MonsterIndex;
	ClientIndex_t	TargetIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FC_MONSTER_HIDE
{
	ClientIndex_t	MonsterIndex;
};

using MSG_FC_MONSTER_SHOW = MSG_FC_MONSTER_HIDE;

struct MSG_FC_MONSTER_CHANGE_HP
{
	ClientIndex_t	MonsterIndex;
	INT				CurrentHP;
};					// F -> C, 몬스터의 현재 HP를 전송함

struct MSG_FC_MONSTER_CHANGE_BODYCONDITION
{
	ClientIndex_t	MonsterIndex;
	BodyCond_t		BodyCondition;					// 앞의 32bit만 사용 <-- check: 스킬 bodycon이 정해지면 결정하기!
};

struct MSG_FC_MONSTER_SKILL_USE_SKILL
{
	ClientIndex_t		MonsterIndex;				// 스킬을 사용한 몬스터
	ClientIndex_t		ClientIndex;				// 몬스터 스킬의 종료를 알려줄 클라이언트 인덱스
	INT					SkillItemNum;				// 스킬의 ItemNum
};

// start 2011-05-17 by hskim, 인피니티 3차 - 시네마 몬스터 교체 기능
struct MSG_FC_MONSTER_CHANGE_INDEX
{
	ClientIndex_t	MonsterIndex;
	INT				ChangeMonsterUnitKind;
};					// F -> C, 몬스터 교체 메시지 전달
					// end 2011-05-17 by hskim, 인피니티 3차 - 시네마 몬스터 교체 기능

struct MSG_FC_MONSTER_SKILL_END_SKILL
{
	ClientIndex_t	MonsterIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FC_MONSTER_SUMMON_MONSTER
{
	ClientIndex_t	clientIdx;
	INT				SummonMonsterUnitKind;			// 몬스터 고유번호
	INT				nSummonCounts;
	BYTE			byMonsterTargetType2;
	int				nTargetTypeData2;
	AVECTOR3		PositionAVector;
};
struct MSG_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY
{
	ClientIndex_t	monsterIdx;
};
struct MSG_FC_MONSTER_TUTORIAL_MONSTER_DELETE
{
	ClientIndex_t	monsterIdx;
};

///////////////////////////////////////////////////////////////////////////////
// FN_MONSTER (Field server <-> NPC server)
struct MSG_FN_MAPPROJECT_START_OK
{
	USHORT		CreateChannelCounts;
	bool		bCityWarStarted;
	UID32_t		CityWarOccupyGuildUID;
	ARRAY_(MSG_FN_CONNECT_SET_CHANNEL_STATE);
};

struct MSG_FN_MONSTER_CREATE
{
	ChannelIndex_t			ChannelIndex;
	ClientIndex_t			MonsterIndex;				// 인덱스
	INT						MonsterUnitKind;				// 몬스터 고유번호
	BYTE					MonsterTargetType1;
	int						TargetTypeData1;
	ClientIndex_t			CltIdxForTargetType1;
	BodyCond_t				BodyCondition;					// 몬스터 BodyCondition
	AVECTOR3				PositionVector;					// 몬스터 좌표
	AVECTOR3				TargetVector;					//
	BYTE					ObjectMonsterType;
	MONSTER_BALANCE_DATA	MonsterBalanceData;				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보 추가.
};	// check: 불필요한 정보를 없애야 하지 않을까? 확인 요망!

struct MSG_FN_MONSTER_CREATE_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
};

struct MSG_FN_MONSTER_DELETE
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
	bool			CinemaDelete;		// 2011-05-30 by hskim, 인피니티 3차 - 몬스터 삭제 클라이언트 반영 - 시네마에서 삭제한경우 TRUE
};

// start 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가
struct MSG_FN_MONSTER_CHANGE_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
	INT				ChangeMonsterUnitKind;
};
// end 2011-05-11 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 변경 기능 추가

// start 2011-05-30 by hskim, 인피니티 3차 - 몬스터 삭제 클라이언트 반영
struct MSG_FC_MONSTER_CINEMA_DELETE_NOTIFY
{
	ClientIndex_t	MonsterIdx;
};
// end 2011-05-30 by hskim, 인피니티 3차 - 몬스터 삭제 클라이언트 반영

struct MSG_FN_MONSTER_MOVE
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
	ClientIndex_t	TargetIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	USHORT			usSendRange;
};

struct MSG_FN_MOVE_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MEX_CHARACTER_1
{
	USHORT			Race;						// 종족
	USHORT			UnitKind;
	BYTE			InfluenceType0;				// 2005-12-27 by cmkwon
	BYTE			CharacterMode0;				// 2005-07-15 by cmkwon
	UID32_t			CharacterUniqueNumber;
	UID32_t			GuildUID;
	BodyCond_t		BodyCondition;				// 상태, bit flag 사용
	float			CurrentHP;					// 에너지
	AVECTOR3		PositionVector;				// 캐릭터 좌표

	MEX_CHARACTER_1 &operator=(const CHARACTER &rhs)
	{
		Race = rhs.Race;
		UnitKind = rhs.UnitKind;
		InfluenceType0 = rhs.InfluenceType;
		CharacterMode0 = rhs.CharacterMode;
		CharacterUniqueNumber = rhs.CharacterUniqueNumber;
		GuildUID = rhs.GuildUniqueNumber;
		BodyCondition = rhs.BodyCondition;
		CurrentHP = rhs.CurrentHP;
		PositionVector = rhs.PositionVector;
		return *this;
	};
};
struct MSG_FN_CLIENT_GAMESTART_OK
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
	MEX_CHARACTER_1	mexCharacter;
	UID32_t			GuildMasterCharUID;
	bool			bStealthState1;
	bool			bInvisible;			// 2006-11-27 by dhjin, 캐릭터 안보이기 플래그
};

struct MSG_FN_GET_CHARACTER_INFO
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
};

using MSG_FN_GET_CHARACTER_INFO_OK = MSG_FN_CLIENT_GAMESTART_OK;

struct MSG_FN_ADMIN_SUMMON_MONSTER
{
	ChannelIndex_t			ChannelIndex;
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT						MonsterUnitKind;
	BYTE					MonsterTargetType1;
	int						TargetTypeData1;
	ClientIndex_t			CltIdxForTargetType1;
	INT						NumOfMonster;
	AVECTOR3				Position;

	// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.)
	MONSTER_BALANCE_DATA	MonsterBalanceData;

};

struct MSG_FN_MONSTER_CHANGE_HP
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;
	float				CurrentHP;
};

struct MSG_FN_MONSTER_CHANGE_BODYCONDITION : MSG_FC_CHARACTER_CHANGE_BODYCONDITION
{
	ChannelIndex_t		ChannelIndex;
};

struct MSG_FN_MONSTER_SKILL_USE_SKILL
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;				// 스킬을 사용한 몬스터
	ClientIndex_t		ClientIndex;				// 몬스터 스킬의 종료를 알려줄 클라이언트 인덱스
	INT					SkillItemNum;				// 스킬의 ItemNum
};

struct MSG_FN_MONSTER_SKILL_END_SKILL
{
	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;
	AVECTOR3			PositionVector;
	AVECTOR3			TargetVector;
};

struct MSG_FN_MONSTER_AUTO_DESTROYED		// 2006-04-17 by cmkwon
{
	MAP_CHANNEL_INDEX	MapChannIdx;				// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - MSG_FN_MONSTER_AUTO_DESTROYED 에 필드 추가
													// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - MapChannIdx 로 추가
													//	ChannelIndex_t		ChannelIndex;
	ClientIndex_t		MonsterIndex;
	BodyCond_t			BodyCondition;				// 2007-11-26 by cmkwon, 몬스터 자동삭제 메시지 TCP로 전송(N->F) - MSG_FN_MONSTER_AUTO_DESTROYED 에 필드 추가
};

struct MSG_FN_MONSTER_STRATEGYPOINT_INIT		// 2006-11-20 by cmkwon
{
	ChannelIndex_t		ChannelIndex;
	int					bVCNMapInflTyforInit;	// 2007-09-16 by dhjin, Belligerence 검색으로 수정으로 인하여 오브젝트 Bell값으로 수정 // 2006-11-21 by cmkwon, 초기화 할 맵이 VCN인지 여부(TRUE=IS_MAP_INFLUENCE_VCN, FALSE=IS_MAP_INFLUENCE_ANI)
	bool				bCreateFlag;			// 초기화 후에 소환 여부 플래그
	bool				bInfluenceBoss;			// 2007-08-30 by dhjin, 모선전시 전략포인트 소환인지 체크 비트 0 : FASLE, 1 : TRUE
};

struct MSG_FN_MONSTER_STRATEGYPOINT_SUMMON
{
	MapIndex_t			MapIndex;
};

struct MSG_FN_MONSTER_OUTPOST_INIT
{// 2007-08-24 by dhjin, 전진기지 맵 몬스터 초기화 맵 소유에 맞게 몬스터를 소환해야함 
	MAP_CHANNEL_INDEX	mapChann;
	int					bell1;			// 기본값이 -1 이어야 함
	int					bell2;			// 기본값이 -1 이어야 함
	int					bell3;			// 기본값이 -1 이어야 함
};

struct MSG_FN_MONSTER_OUTPOST_RESET_SUMMON
{// 2007-08-24 by dhjin, 전진기지 몬스터 소환, 주의~!! 보호막을 파괴한 세력 소유로 소환해야한다.
	MAP_CHANNEL_INDEX	mapChann;
	int					bell1;			// 기본값이 -1 이어야 함
};
using MSG_FN_MONSTER_TELEPORT_SUMMON = MSG_FN_MONSTER_OUTPOST_RESET_SUMMON;

struct MSG_MONSTER_SUMMON_BY_BELL
{// 2007-09-19 by cmkwon, 구조체 추가
	int					MonsterBell;	// 기본값이 -1 이어야 함
};



struct MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER		// 2006-04-18 by cmkwon
{
	BYTE			Belligerence0;				// 
	bool			IsSummonJacoMonster;		// 
};

struct MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL		// TCP:F->N, // 2007-08-22 by cmkwon, 해당 맵채널 몬스터 모두 삭제하기 기능 추가
{
	MAP_CHANNEL_INDEX	mapChann;
	bool				bAllFlag;		// FALSE가 아니면 아래의 필드들에 상관없이 모든 몬스터가 삭제됨
	int					bell1;			// 기본값이 -1 이어야 함
	int					bell2;			// 기본값이 -1 이어야 함
	int					excludeBell1;	// 기본값이 -1 이어야 함
	int					excludeBell2;	// 기본값이 -1 이어야 함
	bool				bNotCreateMonster;	// 몬스터 생성 금지 0 => False 1 => True
};

struct MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE		// TCP:F->N, // 2007-08-29 by dhjin, 해당 맵채널 몬스터 모두 생성가능하게 설정
{
	MAP_CHANNEL_INDEX	mapChann;
};


struct MSG_FN_MISSILE_MOVE
{
	ChannelIndex_t	ChannelIndex;
	UINT			ItemNum;						// 공격 무기의 종류
	ClientIndex_t	MonsterIndex;					// 공격 몬스터의 인덱스
	INT				WeaponIndex;					// 공격 무기 인덱스(몬스터에 한해 유일함)
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
};

struct MSG_FN_MONSTER_HPRECOVERY
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	MonsterIndex;
	UINT			RecoveryHP;
};


struct MSG_FN_MONSTER_HIDE
{
	ChannelIndex_t	ChannelIndex;
	ClientIndex_t	ClientIndex;
};
using MSG_FN_MONSTER_SHOW = MSG_FN_MONSTER_HIDE;

///////////////////////////////////////////////////////////////////////////////
// FC_EVENT (Field server <-> Client)

// 워프 타입: WARP_TYPE_XXX
#define WARP_TYPE_SAME_MAP				(BYTE)0x00	// 같은 맵으로의 워프
#define WARP_TYPE_SAME_FIELD_SERVER		(BYTE)0x01	// 다른 맵(같은 필드 서버)으로의 워프
#define WARP_TYPE_OTHER_FIELD_SERVER	(BYTE)0x02	// 다른 맵(다른 필드 서버)으로의 워프

struct MSG_FC_EVENT_WARP
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	SERVER_ID			FieldServerID;
	USHORT				WarpAreaIndex;
};			// F->C, 다른 필드 서버로 워프함

struct MSG_FC_EVENT_WARP_SAME_MAP
{
	AVECTOR3			PositionVector;
	BYTE				CharacterMode0;		// 2005-07-27 by cmkwon
};				// F->C, 같은 필드 서버 & 같은 맵으로 워프함

struct MSG_FC_EVENT_WARP_SAME_FIELD_SERVER
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	BitFlag16_t			MapWeather;
	AVECTOR3			PositionVector;
	BYTE				CharacterMode0;		// 2005-07-27 by cmkwon
};		// F->C, 같은 필드 서버 & 다른 맵으로 워프함

struct MSG_FC_EVENT_OTHER_WARPED
{
	ClientIndex_t		ClientIndex;
	MapIndex_t			WarpMapIndex;	// 2005-11-01 by cmkwon, 이동 할 MapIndex
};

struct MSG_FC_EVENT_WARP_CONNECT
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	Password[SIZE_MAX_PASSWORD_MD5_STRING];		// MD5
	char	PrivateIP[SIZE_MAX_IPADDRESS];
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	UINT	PartyNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
	USHORT	WarpAreaIndex;
};

struct MSG_FC_EVENT_WARP_CONNECT_OK
{
	ClientIndex_t	ClientIndex;
	AVECTOR3		PositionVector;
	AVECTOR3		TargetVector;
	BitFlag16_t		MapWeather;
};

struct MSG_FC_EVENT_ENTER_BUILDING
{
	BUILDINGNPC		BuildingNPCInfo;
	// 2006-02-08 by cmkwon
	//	float			fCityWarTexRate;				//
	float			fInflDistributionTexPercent;	// 2006-02-08 by cmkwon, 세력분포 세금
};				// F->C

struct MSG_FC_EVENT_ENTER_BUILDING_OK
{
	UINT		BuildingIndex;
	bool		SendShopItemList;
};			// C->F

struct MSG_FC_EVENT_LEAVE_BUILDING
{
	UINT		BuildingIndex;
};				// C->F

struct MSG_FC_EVENT_LEAVE_BUILDING_OK
{
	AVECTOR3	PositionVector;
	AVECTOR3	TargetVector;
};			// F->C

struct MSG_FC_EVENT_REQUEST_WARP
{
	ClientIndex_t	ClientIndex;
};				// C->F, 파티가 있으면 파티장만 요청 가능, 나머지는 요청 불가

struct MSG_FC_EVENT_REJECT_WARP
{
	Err_t			Reason;
};					// F->C, check: 필요하게 되면 구체적인 MSG 내용 정하기!


					//typedef struct
					//{
					//	INT				ObjectIndex;			// Object의 Index
					//} MSG_FC_EVENT_REQUEST_OBJECT_EVENT;		// F->C, Object에 의한 Event를 요청

struct MSG_FC_EVENT_REQUEST_OBJECT_EVENT
{
	DWORD			ObjectType;
	AVECTOR3		ObjectPosition;
};		// F->C, Object에 의한 Event를 요청

struct MSG_FC_EVENT_CHANGE_WEATHER
{
	BitFlag16_t		MapWeather;
};

#define WEATHER_DEFAULT		(BitFlag16_t)0x0000	// 기본
#define WEATHER_SUNNY		(BitFlag16_t)0x0001	// 맑음
#define WEATHER_RAINY		(BitFlag16_t)0x0002	// 비
#define WEATHER_SNOWY		(BitFlag16_t)0x0004	// 눈
#define WEATHER_CLOUDY		(BitFlag16_t)0x0008	// 흐림
#define WEATHER_FOGGY		(BitFlag16_t)0x0010	// 안개

struct MEX_CHANNEL_INFO
{
	ChannelIndex_t	ChannelIndex;
	INT				Crowdedness;		// 혼잡도, 0% ~ 100%??, check: 정확한 방식 결정해야 함
};

struct  MSG_FC_EVENT_SUGGEST_CHANNELS
{
	MapIndex_t		MapIndex;
	INT				WarpTargetIndex;	// 클라이언트는 이 정보를 MSG_FC_EVENT_SELECT_CHANNEL에 그대로 복사해서 서버에 넘겨주어야 함
	INT				NumOfChannels;		// 선택 가능한 채널의 수
	ARRAY_(MEX_CHANNEL_INFO);
};		// F->C, 선택 가능한 채널을 제시함

struct MSG_FC_EVENT_SELECT_CHANNEL {
	ClientIndex_t		ClientIndex;
	INT					WarpTargetIndex;	// 클라이언트는 이 정보를 MSG_FC_EVENT_SUGGEST_CHANNELS로부터 그대로 복사해서 서버에 넘겨주어야 함
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// C->F, 선택한 채널을 전송함

struct MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY
{
	ClientIndex_t		ClientIndex;
	INT					WarpTargetIndex;	// 클라이언트는 이 정보를 MSG_FC_EVENT_SUGGEST_CHANNELS로부터 그대로 복사해서 서버에 넘겨주어야 함
	MAP_CHANNEL_INDEX	MapChannelIndex;
	int					nPartyMembers;		// 같이 워프할 파티원의 수, 편대장 제외
	ARRAY_(UID32_t);						// 파티원 character uniquenumber의 array
};	// C->F, 선택한 채널을 전송함

struct MSG_FC_EVENT_REQUEST_RACING_WARP
{
	ClientIndex_t		ClientIndex;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};	// C->F, 레이싱맵으로의 워프 요청


struct MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct WARP_TARGET_MAP_INFO_4_EXCHANGE
{
	MapIndex_t	MapIndex;
	INT			TargetIndex;	// EVENTINFO에 존재하는 TargetIndex
	char		TargetName[SIZE_MAX_WARP_TARGET_NAME];	// 타켓의 이름(ex: 도시상단, 도시하단)
	INT			Fee;			// 워프 요금
};

struct MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK
{
	INT		NumOfTargetMaps;
	ARRAY_(WARP_TARGET_MAP_INFO_4_EXCHANGE);
};

struct MSG_FC_EVENT_REQUEST_SHOP_WARP
{
	MapIndex_t	MapIndex;
	INT			TargetIndex;			// EVENTINFO에 존재하는 TargetIndex	
};

struct MSG_FC_EVENT_CHARACTERMODE_ENTER_BUILDING
{
	INT			nBuildingIndex0;
};

struct MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST			// 2006-07-21 by cmkwon
{
	DWORD				dwCallWarpEventID0;
	DWORD				dwSupportTermTick0;
	char				szUserMessage[SIZE_STRING_128];
};

struct MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST_ACK		// 2006-07-21 by cmkwon
{
	DWORD				dwCallWarpEventID0;
};


///////////////////////////////////////////////////////////////////////////////
// FN_EVENT (Field server <-> NPC server)
using MSG_FN_EVENT_OTHER_WARPED = MSG_FC_EVENT_OTHER_WARPED;

///////////////////////////////////////////////////////////////////////////////
// FP_EVENT (Field server <-> Pre server)
struct MSG_FP_EVENT_NOTIFY_WARP
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
	SERVER_ID		FieldServerID;
	UID32_t			CharacterUniqueNumber;
};		// F --> P

struct MSG_FP_EVENT_NOTIFY_WARP_OK
{
	UID32_t			CharacterUniqueNumber;
};		// P --> F

struct MSG_FP_EVENT_ENTER_SELECT_SCREEN
{
	char			AccountName[SIZE_MAX_ACCOUNT_NAME];
};		// F->P, Client가 캐릭터 선택 창에 들어옴

struct MSG_FP_EVENT_GAME_STARTED
{
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};			// F->P, Client가 게임을 시작함(맵으로 들어옴)

struct MSG_FP_EVENT_MAP_CHANGED
{
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;
};				// F->P, Client가 맵을 이동함

				// T0_FP_MONITOR
//struct MSG_FP_MONITOR_SET_MGAME_EVENT_TYPE
//{
//	MGAME_EVENT_t		enMGameEvent;
//};



///////////////////////////////////////////////////////////////////////////////
// FC_STORE
struct MSG_FC_STORE_GET_ITEM
{
	UID32_t	AccountUniqueNumber;
	UID32_t	PossessCharacter;		// CharacterUniqueNumber 
	BYTE	ItemStorage0;			// 0(ITEM_IN_CHARACTER):캐릭터인벤, 1(ITEM_IN_STORE):창고
	bool	bSendToClient;			// Client에 전송할지 여부, WARP_CONNECT시는 전송 불필요
	bool	bMainGameStartFlag;		// 2012-10-08 by khkim, GLog - 게임시작시 인지를 확인할 변수
};

struct MSG_FC_STORE_PUT_ITEM_HEADER
{
	UID32_t	PossessCharacter;		// CharacterUniqueNumber
	BYTE	ItemStorage0;			// 2006-01-05 by cmkwon, 0(ITEM_IN_CHARACTER):캐릭터인벤, 1(ITEM_IN_STORE):창고
};

struct MSG_FC_STORE_PUT_ITEM
{
	int		BytesToRead;			// 뒤에 붙는 아이템의 총 바이트수, 주의가 필요함
	ARRAY_(ITEM_XXX);
};

struct MSG_FC_STORE_PUT_ITEM_DONE
{
	UINT	NumOfItem;
	BYTE	ItemStorage0;			// 2006-01-05 by cmkwon, 0(ITEM_IN_CHARACTER):캐릭터인벤, 1(ITEM_IN_STORE):창고
};

struct MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE	// 2012-06-05 by jhseol, 아템미리보기 - 클라요청 패킷 추가
{
	struct ItemEnahcntInfo
	{
		// 2013-01-24 by jhseol, 아템미리보기 패킷 사이즈 초과로 인한 버그 수정
		UID64_t		TargetItemUniqueNumber;
		INT			TargetItemNum;				// 대상 아이템의 ItemNum
		INT			EnchantItemNum;				// enchant로 쓰인 아이템의 ItemNum
												// end 2013-01-24 by jhseol, 아템미리보기 패킷 사이즈 초과로 인한 버그 수정
		int			EnchantCount;
	};
	ItemEnahcntInfo ItemEnchant[6][8];
};

struct MSG_FC_STORE_MOVE_ITEM
{
	UID64_t		ItemUniqueNumber;		// STORE Talbe의 인덱스, DB paramemter는 (STORE_ITEM*)임
	BYTE		FromItemStorage;		// 0(ITEM_IN_CHARACTER):캐릭터인벤, 1(ITEM_IN_STORE):창고
	BYTE		ToItemStorage;			// 0(ITEM_IN_CHARACTER):창고->캐릭터인벤, 1(ITEM_IN_STORE):캐릭터인벤->창고
	INT			Count;					// 동일 아이템의 개수
};

struct MSG_FC_STORE_MOVE_ITEM_OK
{
	UID64_t		ItemUniqueNumber;
	UID32_t		FromPossessCharacter;	// CharacterUniqueNumber or 0
	UID32_t		ToPossessCharacter;		// CharacterUniqueNumber or 0
	INT			Count;					// 동일 아이템의 개수
};

struct MSG_FC_STORE_LOG_GUILD_ITEM
{// 2006-09-27 by dhjin, C --> F 여단 로그 정보 요청
	UID32_t		AccountUID;
	UID32_t		CharacterUID;
};

struct MSG_FC_STORE_LOG_GUILD_ITEM_OK
{// 2006-09-27 by dhjin, F --> C 여단 로그 정보 전송
	T1<T0::FL_LOG>	LogType;
	ATUM_DATE_TIME	Time;
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				ItemNum;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				ItemChangeCount;
};

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
struct MSG_FI_GUILD_BONUS_EXP_RATE
{
	INT				nBonusExpRate;
	UID32_t			CharacterUID; // 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
};
// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼


// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - IDT_XXX, ITEM_DELETE_TYPE_XXX
#define IDT_GENERAL						((BYTE)0)	// 일반 용도
#define IDT_EXPIRE_TIME					((BYTE)1)	// 유효 시간/기간 경과
#define IDT_INVALID_ITEMNUM				((BYTE)2)	// 유효하지 않은 ItemNum		// 2009-11-20 by cmkwon, 소유 아이템 로딩시 유효하지 않은 아이템 처리 수정 - 
#define IDT_COUNTABLEITEM_ZERO			((BYTE)3)	// 카운터블 아이템 Count 0		// 2009-11-20 by cmkwon, 소유 아이템 로딩시 유효하지 않은 아이템 처리 수정 - 


// IUT_XXX, ITEM_UPDATE_TYPE_XXX
#define IUT_GENERAL						((BYTE)0)	// 일반 용도
#define IUT_DROP_ITEM					((BYTE)1)	// 드랍 아이템
#define IUT_MIXING						((BYTE)2)	// 아이템 조합
#define IUT_TRADE						((BYTE)3)	// 거래
#define IUT_SHOP						((BYTE)4)	// 상점 구매 아이템, 판매 금액 등
#define IUT_QUEST						((BYTE)5)	// 퀘스트에 의한 삽입
#define IUT_ADMIN						((BYTE)6)	// 관리, 개발용
#define IUT_BULLET						((BYTE)7)	// 총알 업데이트
#define IUT_SKILL						((BYTE)8)	// 스킬 아이템
#define IUT_LOADING						((BYTE)9)	// 게임 시작 시 로딩
#define IUT_AUCTION						((BYTE)10)	// 경매 관련
#define IUT_ENCHANT						((BYTE)11)	// 인챈트
#define IUT_USE_ITEM					((BYTE)12)	// 아이템 사용
#define IUT_MGAME_EVENT					((BYTE)13)	// MGameEvent
#define IUT_USE_ENERGY					((BYTE)14)	// 에너지류 아이템 사용
#define IUT_EXPIRE_CARD_ITEM			((BYTE)15)	// 시간제한 아이템 사용 시간 만료
#define IUT_PENALTY_ON_DEAD				((BYTE)16)	// 캐릭터 죽을 때의 패널티
#define IUT_PENALTY_AGEAR_FUEL_ALLIN	((BYTE)17)	// 캐릭터 죽을 때의 패널티
#define IUT_INFLUENCEWAR_KILLER_BONUS	((BYTE)18)	// 세력전에서 킬러 보너스
#define IUT_BONUS_ITEM					((BYTE)19)	// 2006-04-26 by cmkwon, 보너스 아이템
#define IUT_BAZAAR_SELL					((BYTE)20)	// 개인 판매 상점 거래
#define IUT_BAZAAR_BUY					((BYTE)21)	// 개인 구입 상점 거래
#define IUT_RANDOMBOX					((BYTE)22)	// 2006-08-10 by cmkwon, 랜덤박스 사용
#define IUT_GIVEEVENTITEM				((BYTE)23)	// 2006-08-25 by dhjin, 이벤트 아이템 사용
#define IUT_GUILD_STORE					((BYTE)24)	// 2006-09-25 by cmkwon, 여단 창고
#define IUT_EXPIRE_ITEM					((BYTE)25)	// 2006-09-29 by cmkwon, 아이템 만료
#define IUT_STORE						((BYTE)26)	// 2006-10-26 by cmkwon, 창고 이용
#define IUT_STORE_FEE					((BYTE)27)	// 2006-10-26 by cmkwon, 창고 이용료
#define IUT_ARENA_ITEM					((BYTE)28)	// 2007-06-04 by dhjin, 아레나 전용 아이템
#define IUT_TUTORIAL_PAY_ITEM			((BYTE)29)	// 2007-07-24 by dhjin, Tutorial 지급 아이템
#define IUT_EXPEDIENCYFUND_PAYBACK		((BYTE)30)	// 2007-08-22 by dhjin, 판공비 환급
#define IUT_GIVEEVENTITEM_COUPONEVENT	((BYTE)31)	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
#define IUT_LUCKY_ITEM					((BYTE)32)	// 2008-11-10 by dhjin, 럭키머신
#define IUT_WAR_CONTRIBUTION			((BYTE)33)	// 2008-12-23 by dhjin, 전쟁 보상 추가안
#define IUT_WAR_CONTRIBUTION_LEADER		((BYTE)34)	// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 지도자
#define IUT_WAR_CONTRIBUTION_GUILD		((BYTE)35)	// 2008-12-23 by dhjin, 전쟁 보상 추가안 - 길드
#define IUT_DISSOLUTION_ITEM			((BYTE)36)	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리
#define IUT_DEPENDENCY_ITEM				((BYTE)37)	// 2011-09-20 by hskim, 파트너 시스템 2차 - 종속적 아이템 처리 (펫 삭제시 소켓 아이템 같이 삭제)
#define IUT_WARP						((BYTE)38)	// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 - 워프 입장 조건에 의한 아이템 삭제

// start 2012-11-21 by khkim, GLog 2차
#define GLOG_WARPOINTSHOP				((BYTE)39)	// 워포 상점 
#define GLOG_INFINITYOBSHOP				((BYTE)40)	// 오브	상점
#define GLOG_CASHSHOP					((BYTE)41)	// 캐쉬 상점
#define GLOG_DEFAULT_ZERO				((BYTE)0) 	// 디폴트 0값 사용시
// end 2012-11-21 by khkim, GLog 2차

#define IUT_CITYWAR_BRING_SUMOFTEX		((BYTE)100)	// 도시점령전 세금

struct MSG_FC_STORE_INSERT_ITEM
{
	UID32_t			FromCharacterUniqueNumber;		// From Possess, 혹은 FromCharacterUniqueNumber
	BYTE			ItemInsertionType;				// 아이템 삽입 타입, IUT_XXX
	ITEM_GENERAL	ItemGeneral;
};

struct MSG_FC_STORE_DELETE_ITEM
{
	UID64_t			ItemUniqueNumber;	// 지울 아이템
	BYTE			ItemDeletionType;	// 아이템 삭제 타입, IUT_XXX
};

struct MSG_FC_STORE_UPDATE_ITEM_COUNT
{
	UID64_t			ItemUniqueNumber;	// Update할 아이템
	INT				NewCount;			// Count의 새 값
	BYTE			ItemUpdateType;		// 아이템 수정 타입, IUT_XXX
};

struct MSG_FC_STORE_UPDATE_ENDURANCE
{
	UID64_t			ItemUniqueNumber;
	float			Endurance;
};

struct MSG_FC_STORE_UPDATE_POSSESS
{
	UID64_t			ItemUniqueNumber;
	UID32_t			FromPossess;
	UID32_t			ToPossess;
};

struct MSG_FC_STORE_UPDATE_RARE_FIX
{
	UID64_t	ItemUID;
	INT		PrefixCodeNum;	// 접두사, 없으면 0
	INT		SuffixCodeNum;	// 접미사, 없으면 0
};

struct MSG_FC_STORE_INSERT_USINGITEM
{
	INT				ItemNum;
	UID64_t			ItemUID;						// 2006-04-24 by cmkwon
	BYTE			ItemInsertionType;				// 아이템 삽입 타입, IUT_XXX
	INT				nRemainSecond;
};

struct MSG_FC_STORE_DELETE_USINGITEM
{
	INT				ItemNum;
	BYTE			ItemDeletionType;	// 아이템 삭제 타입, IUT_XXX
};

struct MSG_FC_STORE_UPDATE_USINGITEM
{
	UID64_t			ItemUID0;
	INT				UsingTimeStamp0;	// 지금까지 사용된 시간(단위:초)
};

struct MSG_FC_STORE_EXPIRE_USINGITEM		// 2006-10-11 by cmkwon
{
	UID64_t			ItemUID0;
};

struct MSG_FC_STORE_UPDATE_ITEMSTORAGE
{
	//	UID32_t			PossessCharacter;		// 2006-09-18 by dhjin, 선택된 케릭터
	UID64_t			ItemUniqueNumber;
	UID32_t			FromItemStorage;
	UID32_t			ToItemStorage;
};

struct MSG_FC_STORE_UPDATE_ITEMNUM		// 2006-06-14 by cmkwon
{
	UID64_t			ItemUniqueNumber;
	INT				ItemNum;
	BYTE			ItemUpdateType;		// 아이템 수정 타입, IUT_XXX
};

///////////////////////////////////////////////////////////////////////////////
// 2006-09-04 by dhjin, 퀵 슬롯 관련 
#define QUICKTABCOUNT		3
#define QUICKSLOTCOUNT		10			// 2008-06-19 by dhjin, EP3 - 8 -> 10으로 수정  

struct SQUICKSLOT_INFO
{
	bool		Delete;
	int			arrItemNum_[QUICKSLOTCOUNT];
};

struct MSG_FC_STORE_REQUEST_QUICKSLOT_OK
{
	SQUICKSLOT_INFO		POS[QUICKTABCOUNT];
};

struct MSG_FC_STORE_SAVE_QUICKSLOT
{
	SQUICKSLOT_INFO		POS[QUICKTABCOUNT];
	int					SlotPurpose;	// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯
};

///////////////////////////////////////////////////////////////////////////////
// FC_ITEM
struct MSG_FC_ITEM_SHOW_ITEM
{
	UINT			ItemFieldIndex;				// 습득 전까지 서버가 임시로 관리하는 번호
	INT				ItemNum;					// 클라이언트에 아이템의 종류를 보여주기 위해 보냄
	UID32_t			FirstCharacterUID;			// 아이템 습득 가능한 첫번째 캐릭터
	INT				Amount;						// 아이템의 개수
	AVECTOR3		Position;
	BYTE			DropItemType;				// 떨어진 아이템의 종류(습득용 아이템, 공격용(마인류) 아이템 등등)
};

#define DROP_ITEM_TYPE_PICKUP		(BYTE)0x00		// 습득용 아이템
#define DROP_ITEM_TYPE_ATTACK_MINE	(BYTE)0x01		// 공격용 마인류 아이템

struct MSG_FC_ITEM_HIDE_ITEM
{
	UINT			ItemFieldIndex;				// 습득 전까지 서버가 임시로 관리하는 번호
	ClientIndex_t	ClientIndex;				// 아이템을 먹은 character의 client index
};

struct MSG_FC_ITEM_GET_ITEM
{
	ClientIndex_t	ClientIndex;
	UINT			ItemFieldIndex;					// 습득 전까지 서버가 임시로 관리하는 번호
};

struct MSG_FC_ITEM_GET_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UINT			ItemFieldIndex;				// 습득 전까지 서버가 임시로 관리하는 번호
	INT				ItemNum;					// 아이템의 종류
	INT				Amount;						// 아이템의 개수
	INT				SizeOfItem;					// sizeof(해당Item)
	ARRAY_(ITEM_XXX);							// ITEM_XXX가 온다
};

struct MSG_FC_ITEM_GET_ITEM_FAIL
{
	ClientIndex_t	ClientIndex;
	UINT			ItemFieldIndex;				// 습득 전까지 서버가 임시로 관리하는 번호
};

struct MSG_FC_ITEM_PUT_ITEM
{
	UINT			ItemFieldIndex;				// 습득 전까지 서버가 임시로 관리하는 번호
	STRUCT_(ITEM_XXX);							// 이후에 해당 ITEM_XXXX가 온다
};

// 2005-11-15 by cmkwon, 삭제함
//typedef struct
//{
//	ClientIndex_t	ClientIndex;
//	BYTE			KindOfSkill;				// check: 의미 파악 및 이름 정의 필요
//} MSG_FC_ITEM_BONUSSKILLPOINT;
//
//typedef struct
//{
//	ClientIndex_t	ClientIndex;
//	BYTE			OldKindOfSkill;				// check: 의미 파악 및 이름 정의 필요
//	BYTE			NewKindOfSkill;				// check: 의미 파악 및 이름 정의 필요
//	BYTE			RemainedAmountOfSkill;		// check: 변수 type 확인해야 함!
//} MSG_FC_ITEM_BONUSSKILLPOINT_OK;

struct MSG_FC_ITEM_CHANGE_WINDOW_POSITION
{
	UID32_t			CharacterUniqueNumber;
	UID64_t			FromItemUniqueNumber;	// 게임 유저가 이동할 아이템
	INT				FromItemWindowIndex;	// 게임 유저가 이동할 아이템의 기존 위치
	UID64_t			ToItemUniqueNumber;		// 이동 위치(destination)에 들어 있는 아이템(존재한다면), 없다면 0
	INT				ToItemWindowIndex;		// 이동 위치(destination)의 ItemWindowIndex
};

struct MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK
{
	UID64_t			UniqueNumber;			// 게임 유저가 이동한 아이템
	INT				ItemWindowIndex;		// 게임 유저가 이동한 아이템의 새 위치
	BYTE			Wear;					// 게임 유저가 이동한 아이템의 wear 상태
	UID64_t			UniqueNumberDest;		// 이동 위치(destination)에 들어 있던 기존 아이템(존재한다면), 없다면 0
	INT				ItemWindowIndexDest;	// 이동 위치(destination)에 들어 있던 기존 아이템의 새 위치(존재한다면), 없다면 POS_INVALID_POSITION
	BYTE			WearDest;				// 이동 위치(destination)에 들어 있던 기존 아이템 wear 상태(존재한다면), 없다면 0
};

// start 2011-09-20 by hskim, 파트너 시스템 2차
struct MSG_FC_ITEM_NOTIFY_WINDOW_POSITION
{
	UID64_t			UniqueNumber;			// 서버가 이동한 아이템
	INT				ItemWindowIndex;		// 서버가 이동한 아이템의 위치
	BYTE			Wear;					// 서버가 이동한 아이템의 wear 상태

											// 서버에서 이 메시지 사용시 CalcInventoryHiddenItemCounts() 함수를 꼭 호출해주어야함 (숨겨진 아이템 숫자 갱신)

};		// F -> C, 아이템의 ItemWindowIndex 변경될시에 클라이언트에게 알려준다
		// end 2011-09-20 by hskim, 파트너 시스템 2차

struct MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST
{
	INT				NumOfItems;
	ARRAY_(ITEM_WINDOW_INFO);
};

struct ITEM_WINDOW_INFO
{
	UID64_t			ItemUniqueNumber;
	BYTE			ItemKind;
	BYTE			Wear;
	INT				ItemWindowIndex;
};

struct MSG_FC_ITEM_THROW_AWAY_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	INT				Amount;					// 버리는 개수
};

struct MSG_FC_ITEM_THROW_AWAY_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	INT				RemainedNumOfItem;		// 버린 후 남아있는 개수. 0 이면 완전히 버리기.
};

struct MSG_FC_ITEM_USE_ENERGY
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	INT				nParam1;				//
};
struct MSG_FC_ITEM_USE_ENERGY_OK
{
	ClientIndex_t	ClientIndex;
	INT				ItemNum;
};

struct MSG_FC_ITEM_USE_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
	char			str256ChatMsg[SIZE_MAX_STRING_256];		// 2007-08-09 by cmkwon, 모든 세력에 채팅 전송하기 - 
};
struct MSG_FC_ITEM_USE_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	INT				ItemNum;
};

struct MSG_FC_ITEM_DELETE_ITEM_ADMIN
{
	UINT			ItemFieldIndex;	// 습득 전까지 서버가 임시로 관리하는 번호
	AVECTOR3		DropPosition;	// 아이템의 위치
};

struct MSG_FC_ITEM_DELETE_DROP_ITEM
{
	UINT			ItemFieldIndex;	// 습득 전까지 서버가 임시로 관리하는 번호
	AVECTOR3		DropPosition;	// 아이템(마인)의 위치
};		// F->C, 자신이 뿌린 아이템(마인등)을 지울 때 쓰임

struct MSG_FC_ITEM_UPDATE_ITEM_POS {
	ClientIndex_t	ClientIndex;
	BYTE			ItemPosition;	// POS_XXX
	INT				ItemNum;
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 변경
	//	INT				ColorCode;		// 2005-12-08 by cmkwon, 아머 색상 튜닝 정보
	INT				nShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	INT				nEffectItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
	INT				nPetLevel;		// 2012-12-05 by hskim, 파트너 기간제 외형 변경
};		// F->C, 아이템 장착(전)을 갱신함, 아이템 장착을 바꾸면 주위에 전송함

struct MSG_FC_ITEM_USE_ENCHANT
{
	UID64_t			EnchantItemUniqueNumber;	// 인챈트 아이템
	UID64_t			TargetItemUniqueNumber;		// 인챈트를 바를 대상 아이템
	UID64_t			AttachItemUniqueNumber;		// 추가 아이템
	UID64_t			IncreaseProbabilityItemUID;	// 2009-01-19 by dhjin, 인첸트 확률 증가, 10인첸 파방 카드 - 인첸트 확률 증가 카드
	UID64_t			EnchantItemUniqueNumber2;	// 인챈트 아이템2 // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - SuffixCard ItemUID 
};

struct MSG_FC_ITEM_USE_ENCHANT_OK
{
	// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (빠진부분추가) - 
	//INT				ItemNum;					// 인챈트 아이템
	INT				EnchantItemNum;			// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (빠진부분추가) - 
	INT				SuffixRareItemNum;		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (빠진부분추가) - 
	bool			bSuccessFlag;			// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (빠진부분추가) - 
};					// 인챈트 완료를 보냄

struct MSG_FC_ITEM_PUT_ENCHANT
{
	ENCHANT			Enchant;
};

struct MSG_FC_ITEM_DELETE_ALL_ENCHANT
{
	UID64_t			ItemUniqueNumber;			// 인챈트를 삭제할 아이템	
};

struct MSG_FC_ITEM_MIX_ITEMS
{
	INT				NumOfItems;
	INT				nMixCounts;		// 2008-03-17 by cmkwon, 조합 시스템 기능 추가 - 
	ARRAY_(ITEM_UNIQUE_NUMBER_W_COUNT);
};		// C->F, 조합할 아이템의 리스트를 전송

struct MSG_FC_ITEM_MIX_ITEMS_RESULT
{
	Err_t			Err;		// ERR_XXX, ERROR_NO_ERROR이면 성공
};	// F->C, 아이템 조합 결과를 전송

struct MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON
{
	UID64_t			ItemUniqueNumber;
	char			szCharacterName10[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY
{
	ClientIndex_t	guildSummonClientIdx;
};

struct MSG_FC_ITEM_USE_CARDITEM_RESTORE
{
	UID64_t			ItemUniqueNumber;
	char			szCharacterName10[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY
{
	ClientIndex_t	restoreClientIdx;
};

struct MSG_FC_ITEM_USE_CARDITEM_GUILD
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	UID32_t			guildUID;
};
struct MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	INT				notifyItemNum;
	INT				summonMonsterUnitKind;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME
{
	UID64_t			ItemUniqueNumber;
	char			szChangeCharacterName[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGE_NATION_REQUEST	// 2015-12-17 Future, Nation Change Card
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	char			szChangedCharacterName[SIZE_MAX_CHARACTER_NAME];
	char			szOriginCharacterName[SIZE_MAX_CHARACTER_NAME];
};
struct MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE
{
	UID64_t			ItemUniqueNumber;
	INT				skillItemNum;
};
struct MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	INT				notifyItemNum;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE
{
	UID64_t			ItemUniqueNumber;
	BYTE			pilotFace;
};
struct MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY
{
	ClientIndex_t	notifyClientIdx;
	INT				notifyItemNum;
	BYTE			pilotFace;
};

struct MSG_FC_ITEM_USE_INFLUENCE_BUFF				// 2006-04-21 by cmkwon
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_INFLUENCE_BUFF_OK			// 2006-04-21 by cmkwon
{
	// 2009-01-05 by dhjin, 미션마스터 - 편대 버프 아이템 추가 - 밑과 같이 변경
	//	UID64_t			ItemUniqueNumber;
	char			ItemUseCharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				ItemNum;
};
struct MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT			// 2006-04-21 by cmkwon
{
	UID64_t			ItemUniqueNumber;
};
struct MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK		// 2006-04-21 by cmkwon
{
	UID64_t			ItemUniqueNumber;
};

// 2006-08-10 by cmkwon
#define RANDOMBOX_RESULT_FAIL				0		// 실패
#define RANDOMBOX_RESULT_ITEM				1		// 아이템
#define RANDOMBOX_RESULT_SPI				2		// Money(SPI)
#define RANDOMBOX_RESULT_EXP				3		// 경험치
#define RANDOMBOX_RESULT_SKILL_SUPPORT_ITEM	4		// 스킬 보조 아이템, 2006-09-29 by cmkwon 추가함, ITEM_ATTR_SKILL_SUPPORT_ITEM
#define RANDOMBOX_RESULT_CREDITS			5		// 2015-08-03 Future, added credit Randombox
#define RANDOMBOX_RESULT_KIND_COUNT			6		// 랜덤박스 결과 종류 개수

struct MSG_FC_ITEM_USE_RANDOMBOX		// 2006-08-10 by cmkwon
{
	UID64_t			ItemUID;
};
struct MSG_FC_ITEM_USE_RANDOMBOX_OK		// 2006-08-10 by cmkwon
{
	char			szCharacterName0[SIZE_MAX_CHARACTER_NAME];	// 사용자 CharacterName
	INT				nRandomBoxResult;							// RANDOMBOX_RESULT_XXX
	INT				nResultItemNum0;							// 생성된 ItemNum
	INT64			n64ResultCounts;							//
	INT				nPrefixCodeNum0;							// 접두사, 없으면 0
	INT				nSuffixCodeNum0;							// 접미사, 없으면 0
};

using MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM = MSG_FC_ITEM_USE_ITEM;		// 2006-09-29 by cmkwon
using MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK = MSG_FC_ITEM_USE_ITEM_OK;	// 2006-09-29 by cmkwon

																		///////////////////////////////////////////////////////////////////////////////////////
																		// start 2011-09-20 by hskim, 파트너 시스템 2차

struct MSG_FC_ITEM_USE_PET_SOCKET_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

struct MSG_FC_ITEM_USE_PET_SOCKET_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

struct MSG_FC_ITEM_CANCEL_PET_SOCKET_ITEM
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

struct MSG_FC_ITEM_CANCEL_PET_SOCKET_ITEM_OK
{
	ClientIndex_t	ClientIndex;
	UID64_t			ItemUniqueNumber;
};

// end 2011-09-20 by hskim, 파트너 시스템 2차
///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 2008-11-04 by dhjin, 럭키머신
// 2009-03-03 by dhjin, 럭키머신 수정안 - 밑과 같이 변경
// typedef MSG_FC_ITEM_USE_RANDOMBOX		MSG_FC_ITEM_USE_LUCKY_ITEM;
struct MSG_FC_ITEM_USE_LUCKY_ITEM
{
	UID64_t			ItemUID;
	INT				MachineNum;
};

struct LUCKY_ITEM_SIMPLEINFO
{
	SHORT			SlotNum;
	INT				MysteryItemDropNum;
	INT				Itemnum;
	INT64			n64ResultCounts;							//
	INT				nPrefixCodeNum0;							// 접두사, 없으면 0
	INT				nSuffixCodeNum0;							// 접미사, 없으면 0
};

struct MSG_FC_ITEM_USE_LUCKY_ITEM_OK
{
	LUCKY_ITEM_SIMPLEINFO	LuckyItemSimpleInfo[MaxPayLuckyMachineItem];
};


struct MSG_FC_ITEM_CHANGED_SHAPEITEMNUM			// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
{
	UINT64				nItemUID;
	INT					nShapeItemNum;

	FIXED_TERM_INFO		FixedTermShape;			// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
};

struct MSG_FC_ITEM_CHANGED_EFFECTITEMNUM		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
{
	UINT64			nItemUID;
	INT				nEffectItemNum;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// FC_SHOP
struct MSG_FC_SHOP_PUT_ITEM_HEADER
{
	UINT		BuildingIndex;
};

struct MSG_FC_SHOP_PUT_ITEM
{
	USHORT		BytesToRead;			// 뒤에 붙는 아이템들의 총 바이트수
	ARRAY_(SHOP_ITEM);
};

#define UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION		-1		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
#define SOLD_OUT_REMAIN_COUNT_FOR_LIMITED_EDITION		0		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 

struct SHOP_ITEM
{
	UINT		ItemNum;
	char		ItemName[SIZE_MAX_ITEM_NAME];
	USHORT		MinTradeQuantity;
	INT			Price;
	BYTE		ItemKind;
	BYTE		CashShopIndex;		// 2009-01-28 by cmkwon, 캐쉬?수정(추천탭,신상품 추가) - SHOP_ITEM에 추가, 하위 4비트는 탭인덱스, 상위 4비트는 비트 플래그로 사용
	INT			RemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 

													// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 매진여부
	bool IsSoldOutShopItem(void)
	{
		if (SOLD_OUT_REMAIN_COUNT_FOR_LIMITED_EDITION != RemainCountForLimitedEdition)
		{
			return FALSE;
		}
		return TRUE;
	}
	bool IsLimitedEditionShopItem(void)
	{
		if (SOLD_OUT_REMAIN_COUNT_FOR_LIMITED_EDITION > RemainCountForLimitedEdition)
		{
			return FALSE;
		}
		return TRUE;
	}

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// 현재 추가되는건 개인만 쓸 수 있는 1회성 상점에 한정 판매 기능(DB 저장이 필요없는)이 들어감 (크리스탈 시스템)
	// 만일 여러 사람이 동시에 사용하는 한정 판매 기능이 필요하다면 동기화 및 추가 기능이 들어가야함

	bool IsAvailableShopItem(int Amount)
	{
		if (UNLIMITED_REMAIN_COUNT_FOR_LIMITED_EDITION == RemainCountForLimitedEdition)
		{
			return TRUE;
		}

		if (Amount <= RemainCountForLimitedEdition)
		{
			return TRUE;
		}

		return FALSE;
	}
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
};

struct MSG_FC_SHOP_PUT_ITEM_DONE
{
	UINT		NumOfItem;
};

struct MSG_FC_SHOP_GET_ITEMINFO
{
	INT			ItemNum;
};

struct MSG_FC_SHOP_GET_ITEMINFO_OK
{
	ITEM		ItemInfo;
};

struct MSG_FC_SHOP_BUY_ITEM
{
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			Amount;					// 구매할 아이템의 개수
};

struct MSG_FC_SHOP_BUY_ITEM_OK
{
	INT			RemainedMoney;			// 구매 후 남은 돈
	INT			SizeOfItem;				// sizeof(해당Item)의 총합
	ARRAY_(ITEM_XXX);						// ITEM_XXX가 온다
};

#ifdef S_WEB_CASHSHOP_SERVER_MODULE_HSKIM
// 2013-03-13 by hskim, 웹 캐시 상점
// TODO : remove referencing to this module
//#define T_FC_SHOP_AUTH_KEY		(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_CHARACTER_XIGNCODE_S_ACK_PACKET)
#endif

struct MSG_FC_SHOP_AUTH_KEY
{
	char AuthenticationKey[SIZE_MAX_WEB_AUTHENTICATION_KEY];
	INT ServerID;
};
// end 2013-03-13 by hskim, 웹 캐시 상점

// 2011-12-26 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
struct MSG_FC_SHOP_BUY_ITEM_UPDATE
{
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			RemainCountForLimitedEdition;
};
// 2011-12-26 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

struct MSG_FC_SHOP_SELL_ITEM
{
	UINT		BuildingIndex;
	UID64_t		ItemUniqueNumber;
	BYTE		ItemKind;
	INT			Amount;					// 판매할 아이템의 개수
};

struct MSG_FC_SHOP_SELL_ITEM_OK
{
	UID64_t		ItemUniqueNumber;
	INT			RemainedNumOfItem;		// 판매 후 남아 있는 수량(0이면 삭제, Countable Item 인 경우 의미 가짐)
	INT			RemainedMoney;			// 판매 후 남은 돈
};

struct MSG_FC_SHOP_GET_USED_ITEM_PRICE
{
	UID64_t		ItemUniqueNumber;
	INT			Amount;
};

struct MSG_FC_SHOP_GET_USED_ITEM_PRICE_OK
{
	UID64_t		ItemUniqueNumber;
	UINT		Price;
};

struct MSG_FC_SHOP_GET_SHOP_ITEM_LIST
{
	INT			BuildingIndex;
};	// C->F, 상점에서 파는 아이템의 리스트를 요청, 응답은 T_FC_SHOP_PUT_ITEM_XXX

struct MSG_FC_SHOP_REQUEST_REPAIR
{
	INT			DesParam;			// 수리할 부분(DES_HP, DES_DP, DES_EP, DES_SP, DES_BULLET_01, DES_BULLET_02)
	INT			Count;				// 어느 양만큼 수리할 것인지
};		// C->F, 기체 수리 요청

struct MSG_FC_SHOP_REQUEST_REPAIR_OK
{
	INT			DesParam;			// 수리할 부분(DES_HP, DES_DP, DES_EP, DES_SP, DES_BULLET_01, DES_BULLET_02)
	INT			Count;				// 수리한 양
	INT			RepairCost;			// 수리 비용
};

struct MSG_FC_SHOP_BUY_CASH_ITEM
{
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			Amount;					// 구매할 아이템의 개수
	UID32_t		GiveTargetCharacterUID;	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 0 이면 자신이 구입, 0이 아니면 해당 캐릭터에게 선물하기
};
struct MSG_FC_SHOP_BUY_CASH_ITEM_OK
{// 2007-11-13 by cmkwon, 선물하기 기능 추가 - MSG_FC_SHOP_BUY_CASH_ITEM_OK 구조체 추가
	UINT		ItemNum;
	INT			Amount;					// 구매할 아이템의 개수
	UID32_t		GiveTargetCharacterUID;	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 0 이면 자신이 구입, 0이 아니면 해당 캐릭터에게 선물하기
	char		GiveTargetCharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	INT			RemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
};

struct MSG_FC_SHOP_BUY_COLOR_ITEM	// 2005-12-06 by cmkwon
{
	UINT		BuildingIndex;
	UINT		ItemNum;
};

struct MSG_FC_SHOP_BUY_COLOR_ITEM_OK	// 2005-12-06 by cmkwon
{
	UID64_t		ItemUID64;
	INT			ColorCode;
};

struct MSG_FC_SHOP_BUY_WARPOINT_ITEM
{// 2007-06-13 by dhjin, WarPoint 샵 
	UINT		BuildingIndex;
	UINT		ItemNum;
	INT			Amount;					// 구매할 아이템의 개수
};

struct MSG_FC_SHOP_BUY_WARPOINT_ITEM_OK
{// 2007-06-13 by dhjin, WarPointItem 구입하고 난 정보.
	INT				PayWarPoint;			// 2007-06-13 by dhjin, 차감된 WarPoint
	INT				WarPoint;				// 2007-06-13 by dhjin, 총 WarPoint
};

struct MSG_FC_SHOP_CHECK_GIVE_TARGET
{// 2007-11-13 by cmkwon, 선물하기 기능 추가 - MSG_FC_SHOP_CHECK_GIVE_TARGET 구조체 추가
	char			GiveTargetCharName[SIZE_MAX_CHARACTER_NAME];	// 선물받는 캐릭터명
	INT				GiveItemNum;
};

struct MSG_FC_SHOP_CHECK_GIVE_TARGET_OK
{// 2007-11-13 by cmkwon, 선물하기 기능 추가 - MSG_FC_SHOP_CHECK_GIVE_TARGET_OK 구조체 추가
	char			GiveTargetCharName[SIZE_MAX_CHARACTER_NAME];	// 선물받는 캐릭터명
	UID32_t			GiveTargetCharUID;
	char			GiveTargetGuildName[SIZE_MAX_GUILD_NAME];
	USHORT			GiveTargetUnitKind;
	BYTE			GiveTargetLevel;
};

// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
struct MSG_FC_SHOP_CLOSE
{
	UINT BuildingIndex;
};
// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

// 2013-05-09 by hskim, 세력 포인트 개선
struct MSG_FC_SHOP_INFLUENCE_BUFF
{
	INT	nConsecutiveVictories;		// 연승 포인트
};

struct MSG_FC_SHOP_INFLUENCE_BUFF_OK
{
	INT	nLastSkillNum;
};
// end 2013-05-09 by hskim, 세력 포인트 개선

struct SGIVE_TARGET_CHARACTER
{// 2007-11-13 by cmkwon, 선물하기 기능 추가 - SGIVE_TARGET_CHARACTER 구조체 추가
	char			AccountName0[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			AccountUID0;
	INT				ConnectingServerGroupID;
	INT				MembershipItemNum;
	ATUM_DATE_TIME	MembershipExpireTime;
	char			CharacterName0[SIZE_MAX_CHARACTER_NAME];
	UID32_t			CharacterUID0;
	BYTE			InfluenceType0;
	char			GuildName0[SIZE_MAX_GUILD_NAME];
	USHORT			UnitKind0;
	BYTE			Level0;
	INT				BuyItemNum;								// 구매 할 ItemNum
};


///////////////////////////////////////////////////////////////////////////////
// FC_TRADE
struct MSG_FC_TRADE_REQUEST_TRADE
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// 피요청자
};

struct MSG_FC_TRADE_REQUEST_TRADE_OK
{
	char			TradeSource[SIZE_MAX_CHARACTER_NAME];	// 요청자
	UID32_t			TradeSourceCharacterUniqueNumber;		// 요청자의 character unique number
};

struct MSG_FC_TRADE_CANCEL_REQUEST
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// 피요청자
};

struct MSG_FC_TRADE_CANCEL_REQUEST_OK
{
	char			TradeSource[SIZE_MAX_CHARACTER_NAME];	// 요청자
	UID32_t			TradeSourceCharacterUniqueNumber;		// 요청자의 character unique number
};

struct MSG_FC_TRADE_ACCEPT_TRADE
{
	UID32_t			TradeSourceCharacterUniqueNumber;		// 요청자의 character unique number
};

struct MSG_FC_TRADE_ACCEPT_TRADE_OK
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// 피요청자
	UID32_t			TradeTargetCharacterUniqueNumber;		// 피요청자의 character unique number
};

struct MSG_FC_TRADE_REJECT_TRADE
{
	UID32_t			TradeSourceCharacterUniqueNumber;		// 요청자의 character unique number
};

struct MSG_FC_TRADE_REJECT_TRADE_OK
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// 피요청자
	UID32_t			TradeTargetCharacterUniqueNumber;		// 피요청자의 character unique number
};

struct MSG_FC_TRADE_REJECT_TRADING
{
	char			TradeTarget[SIZE_MAX_CHARACTER_NAME];	// 피요청자
};								// 거래중이므로 거래를 할 수가 없다

struct MSG_FC_TRADE_SHOW_TRADE_WINDOW
{
	UID32_t			PeerTradeCharacterUniqueNumber;			// 거래 상대의 character unique number
};

struct MSG_FC_TRADE_TRANS_ITEM
{
	UID32_t			CharacterUniqueNumber;					// 아이템 올린 사람
	UID64_t			ItemUniqueNumber;						// 올린 아이템
	INT				ItemNum;								// 아이템 종류
	INT				Amount;									// 올린 개수(에너지류만 해당, 나머지는 1)
};

struct MSG_FC_TRADE_TRANS_ITEM_OK
{
	UID32_t			CharacterUniqueNumber;					// 아이템 올린 사람
	ITEM_GENERAL	TradeItem;								// 이 아이템의 count는 최종 개수
	INT				NumOfEnchants;							// 아이템의 총 인챈트 개수
	ARRAY_(INT);											// EnchantItemNum의 Array
};

struct MSG_FC_TRADE_SEE_ITEM
{
	UID32_t			CharacterUniqueNumber;					// 아이템 올린 사람
	UID64_t			ItemUniqueNumber;						// 올린 아이템
	INT				ItemNum;								// 아이템 종류
	INT				Amount;									// 올린 개수(countable item은 최종 개수, 나머지는 1)
};

struct MSG_FC_TRADE_SEE_ITEM_OK
{
	UID32_t			CharacterUniqueNumber;					// 아이템 올린 사람
	STRUCT_(ITEM_XXX);										// 이 아이템의 count는 최종 개수
};

struct MSG_FC_TRADE_OK_TRADE
{
	UID32_t			CharacterUniqueNumber;					// 자신
};

struct MSG_FC_TRADE_OK_TRADE_OK
{
	UID32_t			CharacterUniqueNumber;					// 상대방
};

struct MSG_FC_TRADE_CANCEL_TRADE
{
	UID32_t			CharacterUniqueNumber;					// 상대방
};

struct MSG_FC_TRADE_CANCEL_TRADE_OK
{
	UID32_t			CharacterUniqueNumber;					// 상대방
};

using MSG_FC_TRADE_INSERT_ITEM = MSG_FC_STORE_INSERT_ITEM;
using MSG_FC_ENEMY_ITEM_INFO = MSG_FC_STORE_INSERT_ITEM;		// 2012-06-05 by jhseol, 아템미리보기 - 클라요청 패킷 추가
using MSG_FC_TRADE_DELETE_ITEM = MSG_FC_STORE_DELETE_ITEM;
using MSG_FC_TRADE_UPDATE_ITEM_COUNT = MSG_FC_STORE_UPDATE_ITEM_COUNT;

struct MSG_FC_TRADE_OK_TRADE_NOTIFY		// 2008-11-21 by cmkwon, 거래 승인 확인 시스템 구현 - 
{
	UID32_t			CharacterUniqueNumber;					// 거래 승인 버튼을 실행한 캐릭터
};

enum COUNTDOWN_TYPE
{
	COUNTDOWN_TYPE_P2P_WAR = 0,
	COUNTDOWN_TYPE_PARTY_WAR = 1,
	COUNTDOWN_TYPE_GUILD_WAR = 2,
	COUNTDOWN_TYPE_RACING = 3,
	COUNTDOWN_TYPE_INVALID = 4
};
// T0_FC_COUNTDOWN
struct MSG_FC_COUNTDOWN_START
{
	int		nWaitingSecond;
	BYTE	byCountdownType;
};

struct MSG_FC_COUNTDOWN_DONE
{
	BYTE	byCountdownType;
};


// T0_FC_OBJECT
struct MSG_FC_OBJECT_CHANGE_BODYCONDITION
{
	INT			nObjectCodeNum;
	AVECTOR3	AVec3Position;
	BodyCond_t	bodyCondition;
};

using MSG_FC_OBJECT_CHANGE_BODYCONDITION_OK = MSG_FC_OBJECT_CHANGE_BODYCONDITION;

///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정

struct MSG_TRADE_PET_DATA
{
	UID64_t			CreatedPetUID;

	char			PetName[SIZE_MAX_PET_NAME];

	INT				PetIndex;
	INT				PetLevel;

	Experience_t	PetExp;

	// 클라에서 필요한 정보 받아서 처리 by hskim

};
// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// FC_AUCTION
struct MSG_FC_AUCTION_REGISTER_ITEM
{
	UID32_t		ItemUID;
	INT			Price;
};			// C->F, 경매 아이템 등록

struct MSG_FC_AUCTION_REGISTER_ITEM_OK
{
	UID32_t		ItemUID;
	INT			Price;
};		// F->C, 경매 아이템 등록 결과

struct MSG_FC_AUCTION_CANCEL_REGISTER
{
	UID32_t		ItemUID;
};		// C->F, 경매 아이템 등록 취소

struct MSG_FC_AUCTION_CANCEL_REGISTER_OK
{
	UID32_t		ItemUID;
};	// F->C, 경매 아이템 등록 취소 결과

struct MSG_FC_AUCTION_BUY_ITEM
{
	UID32_t		ItemUID;
};				// C->F, 경매 아이템 구매

struct MSG_FC_AUCTION_BUY_ITEM_OK
{
	UID32_t		ItemUID;
};			// F->C, 경매 아이템 구매 결과

struct MSG_FC_AUCTION_GET_ITEM_LIST
{
	UID32_t		CharacterUID;			// 자신이 등록한 경매 아이템을 요청할 때는 자신의 CharacterUID, 아니면 0
	BYTE		AuctionKind;			// 요청할 경매 아이템 종류, AUCTION_KIND_XXX
};			// C->F, 경매 아이템 목록 요청

			// AUCTION_KIND_XXX
#define AUCTION_KIND_MY_ITEM	((BYTE)0)	// 자신이 등록한 아이템
#define AUCTION_KIND_ATTACH		((BYTE)1)	// 장착류 0 ~ 17, 22, 25 ~ 26
#define AUCTION_KIND_CARD		((BYTE)2)	// 카드류 21, 27
#define AUCTION_KIND_ETC		((BYTE)3)	// 기타류 18 ~ 20, 23 ~ 24

template<typename T> bool IS_AUCTION_KIND_ATTACH(T _ITEM_KIND)
{
	return IS_WEAPON(_ITEM_KIND)
		|| _ITEM_KIND == ITEMKIND_DEFENSE
		|| _ITEM_KIND == ITEMKIND_SUPPORT
		|| _ITEM_KIND == ITEMKIND_TANK
		|| _ITEM_KIND == ITEMKIND_RADAR
		|| _ITEM_KIND == ITEMKIND_COMPUTER;
}

template<typename T> bool IS_AUCTION_KIND_CARD(T _ITEM_KIND)
{
	return _ITEM_KIND == ITEMKIND_ENCHANT || _ITEM_KIND == ITEMKIND_GAMBLE;
}

template<typename T> bool IS_AUCTION_KIND_ETC(T _ITEM_KIND)
{
	return _ITEM_KIND == ITEMKIND_ENERGY
		|| _ITEM_KIND == ITEMKIND_INGOT
		|| _ITEM_KIND == ITEMKIND_CARD
		|| _ITEM_KIND == ITEMKIND_BULLET
		|| _ITEM_KIND == ITEMKIND_QUEST;
}

struct MSG_FC_AUCTION_INSERT_ITEM
{
	BYTE			AuctionKind;		// AUCTION_KIND_XXX
	char			SellerCharacterName[SIZE_MAX_CHARACTER_NAME];
	ITEM_GENERAL	AuctionItemGeneral;	// 경매 아이템
	int				AuctionPrice;		// 경매 가격
	ATUM_DATE_TIME	AuctionStartDate;	// 경매 시작 시간
};			// F->C, 경매 아이템 전송용

struct MSG_FC_AUCTION_PUT_ENCHANT
{
	BYTE			AuctionKind;		// AUCTION_KIND_XXX
	ENCHANT			AuctionItemEnchant;
};			// F->C, 경매 아이템의 인챈트 정보 전송용

			///////////////////////////////////////////////////////////////////////////////
			// FC_SKILL
struct MSG_FC_SKILL_USE_SKILL_POINT
{
	UINT		KindOfSkill;
};

struct MSG_FC_SKILL_USE_SKILL_POINT_OK
{
	UINT		KindOfSkill;
	BYTE		UsedPoints;
	BYTE		RemainedPoints;
};

struct MSG_FC_SKILL_SETUP_SKILL
{
	INT			ItemNum;
};

struct MSG_FC_SKILL_SETUP_SKILL_OK
{
	BYTE		ItemUpdateType;		// IUT_SHOP, IUT_LOADING, IUT_SKILL
	ITEM_SKILL	ItemSkill;
};

struct MSG_FC_SKILL_USE_SKILL
{
	ItemID_t		SkillItemID;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;	// target이 없는 skill인 경우 0
	UID32_t			TargetCharUID;	// 2005-11-24 by cmkwon, target있지만 TargetIndex가 0일때 사용됨, TargetIndex가 편대원 1명 소환 스킬시 사용한다.
};

struct MSG_FC_SKILL_USE_SKILL_OK
{
	ItemID_t		SkillItemID;
	ClientIndex_t	AttackIndex;
	ClientIndex_t	TargetIndex;	// target이 없는 skill인 경우 0
	ATUM_DATE_TIME	UseTime;		// 2006-11-17 by dhjin, 2차 스킬 사용 시간
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
	float			penality;
#endif
};

struct MSG_FC_SKILL_CANCEL_SKILL
{
	ItemID_t		SkillItemID;			// 종료되는 스킬 정보
	INT				AttackSkillItemNum0;	// 2006-12-12 by cmkwon, 현재 스킬을 종료되게 하는 공격스킬 아이템넘버 
};

struct MSG_FC_SKILL_CANCEL_SKILL_OK
{
	ClientIndex_t	ClientIndex;
	ItemID_t		SkillItemID;
	INT				AttackSkillItemNum0;	// 2006-12-12 by cmkwon, 현재 스킬을 종료되게 하는 공격스킬 아이템넘버
};

struct MSG_FC_SKILL_INVALIDATE_SKILL
{
	ItemID_t		SkillItemID;
	ClientIndex_t	ClientIndex;
};	// skill 사용 중지(시간 제한이 있을 경우)

struct MSG_FC_SKILL_PREPARE_USE
{
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_PREPARE_USE_OK
{
	ClientIndex_t	ClientIndex;
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_CANCEL_PREPARE
{
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_CANCEL_PREPARE_OK
{
	ClientIndex_t	ClientIndex;
	ItemID_t		SkillItemID;
};

struct MSG_FC_SKILL_CONFIRM_USE			// 2005-12-02 by cmkwon
{
	char				szAttackCharacterName[SIZE_MAX_CHARACTER_NAME];	// 스킬 사용자의 CharacterName;
	UID32_t				AttackCharacterUID;		// 스킬 사용자 CharacterUID
	UID32_t				TargetCharacterUID;		// 스킬 타겟 CharacterUID
	int					UsingSkillItemNum;		// 사용 스킬 ItemNum
	MAP_CHANNEL_INDEX	MapChannelIndex;		// 스킬 사용자의 MapChannelIndex
	int					SkillConfirmUseUID;		// 2009-04-06 by cmkwon, 콜오브 히어로 스킬 시스템 변경 - 해당 요청의 UID
};

struct MSG_FC_SKILL_CONFIRM_USE_ACK		// 2005-12-02 by cmkwon
{
	bool				bYesOrNo;				// 수락 여부
	UID32_t				AttackCharacterUID;		// 스킬 사용자 CharacterUID
	UID32_t				TargetCharacterUID;		// 스킬 타겟 CharacterUID
	int					UsingSkillItemNum;		// 사용 스킬 ItemNum
	int					SkillConfirmUseUID;		// 2009-04-06 by cmkwon, 콜오브 히어로 스킬 시스템 변경 - 해당 요청의 UID
};



///////////////////////////////////////////////////////////////////////////////
// FN_SKILL
using MSG_FN_SKILL_USE_SKILL = MSG_FC_SKILL_USE_SKILL;
struct MSG_FN_SKILL_USE_SKILL_OK : public MSG_FC_SKILL_USE_SKILL_OK
{
	ChannelIndex_t		ChannelIndex;
};

///////////////////////////////////////////////////////////////////////////////
// FC_SYNC
//typedef struct
//{
//	INT		ItemNum;
//} MSG_FC_SYNC_SKILL_REUSE_OK;

///////////////////////////////////////////////////////////////////////////////
// FC_INFO
struct MSG_FC_INFO_GET_MONSTER_INFO
{
	INT				MonsterUnitKind;	// 몬스터 고유 번호
};

struct MEX_MONSTER_INFO
{
	INT				MonsterUnitKind;					// 몬스터 고유번호
	char			MonsterName[SIZE_MAX_MONSTER_NAME];	// 몬스터 이름
	BYTE			Level;								// 몬스터의 Level
	INT				HP;									// 만피
	BYTE			Size;								// 필드 서버는 SizeForClient를 loading,  NPC 서버는 SizeForServer를 loading
	BYTE			Belligerence;						// 호전성, // 2005-12-28 by cmkwon 추가함
	BYTE			AlphaBlending;						// alpha blending 여부, TRUE(1), FALSE(0), client측 사용을 위해 추가, 20030616
	USHORT			RenderIndex;
	float			ScaleValue;
	BYTE			TextureIndex;
	UINT			SourceIndex;
	BitFlag64_t		MPOption;			// 2010-01-11 by cmkwon, 몬스터 MPOption 64bit로 변경 - 기존(BYTE)
	BYTE			ClickEvent;							// 2007-09-05 by dhjin, 몬스터 클릭 이벤트 추가
	char			PortraitFileName[SIZE_MAX_FILE_NAME];	// 2010-03-31 by dhjin, 인피니티(기지방어) - 몬스터 초상화 파일

															// operator overloading
	MEX_MONSTER_INFO& operator=(const MONSTER_INFO& rhs)
	{
		this->MonsterUnitKind = rhs.MonsterUnitKind;
		util::strncpy(this->MonsterName, rhs.MonsterName, SIZE_MAX_MONSTER_NAME);
		this->Level = rhs.Level;
		this->HP = rhs.MonsterHP;
		this->Size = rhs.Size;
		this->Belligerence = rhs.Belligerence;				// 호전성
		this->AlphaBlending = rhs.AlphaBlending;
		this->RenderIndex = rhs.RenderIndex;
		this->ScaleValue = rhs.ScaleValue;
		this->TextureIndex = rhs.TextureIndex;
		this->SourceIndex = rhs.SourceIndex;
		this->MPOption = rhs.MPOption;
		this->ClickEvent = rhs.ClickEvent;				// 2007-09-05 by dhjin
		util::strncpy(this->PortraitFileName, rhs.PortraitFileName, SIZE_MAX_FILE_NAME);	// 2010-03-31 by dhjin, 인피니티(기지방어) - 몬스터 초상화 파일

		return *this;
	}
};

struct MSG_FC_INFO_GET_MONSTER_INFO_OK
{
	MEX_MONSTER_INFO	MonsterInfo;
};

struct MSG_FC_INFO_GET_MAPOBJECT_INFO
{
	INT				Code;
};

struct MSG_FC_INFO_GET_MAPOBJECT_INFO_OK
{
	MAPOBJECTINFO	MapObjectInfo;
};

struct MSG_FC_INFO_GET_ITEM_INFO
{
	INT				ItemNum;
};

struct MSG_FC_INFO_GET_ITEM_INFO_OK
{
	ITEM			ItemInfo;
};

struct MSG_FC_INFO_GET_RARE_ITEM_INFO
{
	INT				CodeNum;
};

struct MSG_FC_INFO_GET_RARE_ITEM_INFO_OK
{
	RARE_ITEM_INFO	RareItemInfo;
};

struct MSG_FC_INFO_GET_BUILDINGNPC_INFO
{
	INT				BuildingIndex;
};

struct MSG_FC_INFO_GET_BUILDINGNPC_INFO_OK
{
	BUILDINGNPC		BuildingNPCInfo;
};

struct MSG_FC_INFO_GET_SIMPLE_ITEM_INFO
{
	INT				ItemNum;
};

struct MEX_ITEM_INFO
{
	INT			ItemNum;						// 아이템 고유번호, 장착 아이템일 때 (ITEM_BASE*)
	BYTE		Kind;							// 아이템 종류(기관포, 빔, 로켓, 스킬.....), ITEMKIND_XXX
	float		AbilityMin;						// 아이템최소성능
	float		AbilityMax;						// 아이템최대성능
	USHORT		ReqUnitKind;					// 필요유닛종류
	BYTE		ReqMinLevel;					// 필요 최저 레벨
	BYTE		ReqMaxLevel;					// 필요 최저 레벨
	float		HitRate;						// 명중확률(0~255), // 2010-07-19 by dhjin, 확률 수식 변경
	BYTE		Defense;						// 방어력
	BYTE		SpeedPenalty;					// 스피드페널티, 이동속도에미치는 영향(-:감소)
	USHORT		Range;							// 공격범위, 엔진류인 경우에는 부스터 가동 시 속도
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	UINT		SellingPrice;
#else
	UINT		Price;							// 최소 거래 수량의 가격
	UINT		CashPrice;						// 최소 거래 수량의 현금 가격
#endif
												// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 
												// 	BYTE		DestParameter1;					// 대상파라미터1
												// 	float		ParameterValue1;				// 수정파라미터1
												// 	BYTE		DestParameter2;					// 대상파라미터2
												// 	float		ParameterValue2;				// 수정파라미터2
												// 	BYTE		DestParameter3;					// 대상파라미터3
												// 	float		ParameterValue3;				// 수정파라미터3
												// 	BYTE		DestParameter4;					// 대상파라미터4
												// 	float		ParameterValue4;				// 수정파라미터4
	DestParam_t	ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];	// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)) - // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 
	float		ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];	// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 

	UINT		ReAttacktime;					// 재 공격시간(ms)
	INT			Time;							// 지속 시간(스킬류 등)
	USHORT		RepeatTime;						// 2006-12-08 by cmkwon, 추가함(무기류에서는 남은 총알 수로 사용, 나머지는 개수, 시간형 스킬류에선 남은 시간, 나머지 스킬은 사용 여부)
	float		RangeAngle;						// 범위각도(0 ~ PI)
	BYTE		MultiTarget;					// 동시에 잡을 수 있는 타겟의 수
	USHORT		ExplosionRange;					// 폭발반경(폭발 시 데미지의 영향이 미치는 반경)
	USHORT		ReactionRange;					// 반응반경(마인 등이 반응하는 반경)
	BYTE		ShotNum;						// 점사 수,	점사 시 발사 수를 나타낸다.
	BYTE		MultiNum;						// 동시 발사 탄 수,	1번 발사에 몇발이 동시에 나가느냐
	USHORT		AttackTime;						// 공격시간, 공격을 하기 위해 필요한 시간
	BYTE		ReqSP;							// SP 소모량(스킬)
	BYTE		OrbitType;						// 미사일, 로켓 등의 궤적
	BitFlag64_t	ItemAttribute;					// 아이템의 속성, ITEM_ATTR_XXX
	FLOAT		BoosterAngle;					// 부스터시에 유닛의 회전각, 현재는 엔진에만 사용

												// operator overloading
	MEX_ITEM_INFO& operator=(const ITEM& rhs);
};
struct MSG_FC_INFO_GET_SIMPLE_ITEM_INFO_OK
{
	MEX_ITEM_INFO	ItemInfo;
};
struct MSG_FC_INFO_GET_ENCHANT_COST
{
	INT				EnchantItemNum;
};		// C->F, 인챈트 가격을 요청
struct MSG_FC_INFO_GET_ENCHANT_COST_OK
{
	INT				Cost;
};	// F->C, 인챈트 가격을 전송

struct MSG_FC_INFO_GET_CURRENT_MAP_INFO		// 2007-04-06 by cmkwon, 추가함
{
	MAP_CHANNEL_INDEX	mapChannelIdx0;			// 2007-04-06 by cmkwon
												// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
												//UINT				checkSum0;				// 2007-04-06 by cmkwon, map이 checksum 결과
	int					nFileSize;			// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	BYTE				byDigest[32];		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
};

struct MSG_FC_INFO_GET_CURRENT_MAP_INFO_OK
{
	BYTE			IsPKMap;				// 2005-02-15 by cmkwon, 아직 사용하지 않음
	BYTE			IsCityWarStarted;		// 도시점령전 시작 플래그
	BYTE			byCityWarTeamType;		//
};

enum
{
	HAPPYEV_STATE_TYPE_END = 0,			// 일반 해피아워 이벤트 종료됨
	HAPPYEV_STATE_TYPE_START = 1,			// 일반 해피아워 이벤트 시작됨
	HAPPYEV_STATE_TYPE_STARTING = 2,			// 일반 해피아워 이벤트 진행중
	PCBANG_HAPPYEV_STATE_TYPE_END = 10,			// PCBang 해피아워 이벤트 종료됨
	PCBANG_HAPPYEV_STATE_TYPE_START = 11,			// PCBang 해피아워 이벤트 시작됨
	PCBANG_HAPPYEV_STATE_TYPE_STARTING = 12,			// PCBang 해피아워 이벤트 진행중
	GAME_EVENT_GROUP_MOTHERSHIP_END = 20,		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	GAME_EVENT_GROUP_MOTHERSHIP_START = 21,		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	GAME_EVENT_GROUP_MOTHERSHIP_STARTING = 22,		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	GAME_EVENT_GROUP_ITEM_END = 30,		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	GAME_EVENT_GROUP_ITEM_START = 31,		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	GAME_EVENT_GROUP_ITEM_STARTING = 32,		// 2008-05-20 by cmkwon, 모든 이벤트(HappyHoure,MotherShip,Item) 그룹 동시에 가능하게 - 
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_END = 40,		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_START = 41,		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_STARTING = 42,		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_END = 50,		// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_START = 51,		// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_STARTING = 52		// 2012-05-08 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템 2차
};
struct MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK
{
	BYTE			byStateType;			// 0(종료됨), 1(시작됨), 2(진행중)
	BYTE			byInfluenceType4;		// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK 구조체에 추가된 필드
	ATUM_DATE_TIME	StartATime;
	ATUM_DATE_TIME	EndATime;
	float			fEXPRate2;
	float			fSPIRate2;
	float			fEXPRepairRate2;
	float			fDropItemRate2;
	float			fDropRareRate2;
	float			fWarPointRate2;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
};
struct MSG_FC_INFO_GET_GAME_EVENT_INFO_OK
{
	BYTE			byStateType;			// 0(종료됨), 1(시작됨), 2(진행중)
	int				nGameEventType;			// 이벤트 타입
	float			fRate;					// 이벤트 Rate
	int				nRemainMinute;			// 남은 시간(단위:분)
};

struct MSG_FC_INFO_GET_SERVER_DATE_TIME_OK		// 2006-10-11 by cmkwon
{
	ATUM_DATE_TIME	atimeCurServerTime;		// 2006-10-11 by cmkwon, 현재 서버 날짜 시간
};

struct MSG_FC_INFO_CHECK_RESOBJ_CHECKSUM		// 2007-05-28 by cmkwon
{
	int			nItemNum0;
	char		szObjFileName[SIZE_MAX_RESOBJ_FILE_NAME];
	// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
	//UINT		uiCheckSum0;
	int			nFileSize0;
	BYTE		byDigest[32];		// 2009-05-29 by cmkwon, Hash알고리즘 추가(SHA256) - 
};


///////////////////////////////////////////////////////////////////////////////
// FC_REQUEST - 캐릭터간의 요청, 수락, 거절 등에 쓰임, general-purpose
// REQUEST_TYPE_XXX
#define REQUEST_TYPE_NULL			((BYTE)0)
#define REQUEST_TYPE_TRADE			((BYTE)1)	// 거래
#define REQUEST_TYPE_PARTY_INVITE	((BYTE)2)	// 파티 초대
#define REQUEST_TYPE_GUILD_INVITE	((BYTE)3)	// 드 초대
#define REQUEST_TYPE_P2P_PK			((BYTE)4)	// 1대 1 결투
#define REQUEST_TYPE_PARTY_BATTLE	((BYTE)5)	// 파티간 결투
#define REQUEST_TYPE_GUILD_WAR		((BYTE)6)	// 여단전 신청, 같은 맵에만 있으면 가능

struct MSG_FC_REQUEST_REQUEST
{
	ClientIndex_t	TargetClientIndex;	// 피요청자
	char			TargetCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};				// C->F, 요청

struct MSG_FC_REQUEST_REQUEST_OK
{
	ClientIndex_t	SourceClientIndex;	// 요청자
	char			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};			// F->C, 요청을 전달

struct MSG_FC_REQUEST_ACCEPT_REQUEST
{
	ClientIndex_t	SourceClientIndex;	// 요청자
	char			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};		// C->F, 승낙

struct MSG_FC_REQUEST_ACCEPT_REQUEST_OK
{
	ClientIndex_t	PeerClientIndex;	// 상대방
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};		// F->C, 승낙을 전달, 양(혹은 한) 쪽으로 보냄

struct MSG_FC_REQUEST_REJECT_REQUEST
{
	ClientIndex_t	SourceClientIndex;	// 요청자
	char			SourceCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};		// C->F, 거절

struct MSG_FC_REQUEST_REJECT_REQUEST_OK
{
	ClientIndex_t	PeerClientIndex;	// 상대방
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};		// F->C, 거절을 전달, 양(혹은 한) 쪽으로 보냄

struct MSG_FC_REQUEST_CANCEL_REQUEST
{
	ClientIndex_t	PeerClientIndex;	// 상대방
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// ClientIndex가 0인 경우 이 필드 참조
	BYTE			RequestType;
};		// C->F, 요청 취소됨, 양(혹은 한) 쪽으로 보냄

		///////////////////////////////////////////////////////////////////////////////
		// FC_CITY - 도시용 프로토콜
struct MSG_FC_CITY_GET_BUILDING_LIST
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

struct CITY_BUILDING_INFO_4_EXCHANGE
{
	UINT	BuildingIndex;	// 건물(가게 등) 고유 번호
	BYTE	BuildingKind;
};

struct MSG_FC_CITY_GET_BUILDING_LIST_OK
{
	INT		NumOfBuildings;
	ARRAY_(CITY_BUILDING_INFO_4_EXCHANGE);
};

struct MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
};

// 2004-11-10 by cmkwon, 위에서 선언함
//struct WARP_TARGET_MAP_INFO_4_EXCHANGE
//{
//	MapIndex_t	MapIndex;
//	INT			TargetIndex;	// EVENTINFO에 존재하는 TargetIndex
//	char		TargetName[SIZE_MAX_WARP_TARGET_NAME];	// 타켓의 이름(ex: 도시상단, 도시하단)
//	INT			Fee;			// 워프 요금
//};

struct MSG_FC_CITY_GET_WARP_TARGET_MAP_LIST_OK
{
	INT		NumOfTargetMaps;
	ARRAY_(WARP_TARGET_MAP_INFO_4_EXCHANGE);
};

struct MSG_FC_CITY_REQUEST_ENTER_BUILDING
{
	INT		BuildingIndex;
};	// C->F, 상점 들어갈 것을 요청

struct MSG_FC_CITY_REQUEST_ENTER_BUILDING_OK
{
	INT		BuildingIndex;
	// 2006-02-08 by cmkwon
	//	float	fCityWarTexRate;			//
	float	fInflDistributionTexPercent;	// 2006-02-08 by cmkwon, 세력분포 세금
};	// F->C, 상점 진입 완료를 알림

struct MSG_FC_CITY_REQUEST_WARP
{
	MapIndex_t	MapIndex;
	INT			TargetIndex;			// EVENTINFO에 존재하는 TargetIndex	
};				// C->F, 도시에서 워프해서 나가기 위한 요청

struct MSG_FC_CITY_CHECK_WARP_STATE_OK
{
	Err_t		Err;				// ERR_NO_ERROR이면 성공
};	// F->C, 워프 가능한 상태인지에 대한 결과

	///////////////////////////////////////////////////////////////////////////////
	// FC_TIMER - 타이머 관리용 프로토콜
	///////////////////////////////////////////////////////////////////////////////

	// TimerEventType 의 종류, TE_TYPE_XXX
#define TE_TYPE_TEST					(TimerEventType)0
	//#define TE_TYPE_SYNC_PRI				(TimerEventType)1
	//#define TE_TYPE_SYNC_SEC				(TimerEventType)2
	//#define TE_TYPE_SYNC_SKILL			(TimerEventType)3
	//#define TE_TYPE_END_SKILL				(TimerEventType)4		// check: 클라이언트의 요청에 의해 삭제함, 해당 시간 경과 후 클라이언트에서 CANCEL_SKILL 전송, 20041005, kelovon with jschoi
	//#define TE_TYPE_DELETE_MINE			(TimerEventType)5	// check: 마인 지우기는 클라이언트에서 모두 관리하기로 하면서 제거함, 20041118, kelovon with dhkwon
#define TE_TYPE_DELETE_DUMMY			(TimerEventType)6
#define TE_TYPE_DELETE_FIXER			(TimerEventType)7
#define TE_TYPE_DELETE_DECOY			(TimerEventType)8		// 디코이는 TimeOut발생 시 CurrentCount를 줄이고, CurrentCount=0까지 반복함
#define TE_TYPE_GRADUAL_SHIELD_UP		(TimerEventType)9		// 2-2형 무기 쉴드, interval은 무조건 1000 ms
#define TE_TYPE_RECOVER_HP				(TimerEventType)10		// 착륙했을 때만 채움(5초당 3)
#define TE_TYPE_RECOVER_DP				(TimerEventType)11		// 착륙했을 때만 채움
#define TE_TYPE_RECOVER_SP				(TimerEventType)12
#define TE_TYPE_GRADUAL_HP_UP			(TimerEventType)13		// interval은 무조건 1000 ms, ITEM_GENERAL.Endurance가 0이 될 때까지 반복함
#define TE_TYPE_GRADUAL_DP_UP			(TimerEventType)14		// interval은 무조건 1000 ms, ITEM_GENERAL.Endurance가 0이 될 때까지 반복함
#define TE_TYPE_GRADUAL_SP_UP			(TimerEventType)15		// interval은 무조건 1000 ms, ITEM_GENERAL.Endurance가 0이 될 때까지 반복함, not used yet: SP는 자동으로 회복됨, TE_TYPE_RECOVER_SP를 사용
#define TE_TYPE_GRADUAL_EP_UP			(TimerEventType)16		// interval은 무조건 1000 ms, ITEM_GENERAL.Endurance가 0이 될 때까지 반복함
#define TE_TYPE_IMMEDIATE_HP_UP			(TimerEventType)17		// 사용시 바로 채우고 Time 동안 재사용이 안된다
#define TE_TYPE_IMMEDIATE_DP_UP			(TimerEventType)18		// 사용시 바로 채우고 Time 동안 재사용이 안된다
#define TE_TYPE_IMMEDIATE_SP_UP			(TimerEventType)19		// 사용시 바로 채우고 Time 동안 재사용이 안된다
#define TE_TYPE_IMMEDIATE_EP_UP			(TimerEventType)20		// 사용시 바로 채우고 Time 동안 재사용이 안된다
#define TE_TYPE_REQUEST_TIMEOUT			(TimerEventType)21		// 거래, 파티전, 일대일대결 등의 요청에 대한 TimeOut
#define TE_TYPE_DECREASE_SP				(TimerEventType)22		// TOGGLE형 스킬 사용 시, SP를 ReqSP만큼 소모, SP 소진 시 스킬 자동 취소
#define TE_TYPE_DO_MINUTELY_WORK		(TimerEventType)23		// 1분에 한번씩 발행하는 타이머
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
#define TE_TYPE_DOTSKILL_STEALING		(TimerEventType)24		// Stealing 스킬 - SP감소 (SP가 소진 되어도 계속 적용 되어야 한다.)
#define TE_TYPE_DOTSKILL_DRAIN			(TimerEventType)25		// Drain 스킬 - 피격대상에 HP감소시켜 공격대상에게 채워준다.
#define TE_TYPE_DOTSKILL_TIME_BOMB		(TimerEventType)26		// TimeBomb 스킬 - 피 대상에게 시간 종료시 현재 피에 %로 데미지를 입힌다.
#define TE_TYPE_FIXED_TERM_SHAPE		(TimerEventType)27		// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)


#define TIMER_DOT_INTERVAL					1000


#define TIMER_DECOY_TIME_CHECK_INTERVAL		1000
#define TIMER_GRADUAL_UP_INTERVAL			1000	// gradual up에 사용되는 interval, DELETE_DECOY에도 사용함
#define _DEFAULT_TIME_GRANULARITY			100		// milli-seconds
#define _MAX_TIMER_INTERVAL_TIME			150000	// maximum 150 seconds, check: 조정해야 함
#define SIZE_MAX_TIMER_EVENT_MEMPOOL		1000
#define _TIMER_TICK							_DEFAULT_TIME_GRANULARITY/3
#define TIMER_REPEAT_INFINITE				MAX_INT_VALUE
#define TIMER_DECREASE_SP_INTERVAL			1000				// 1초
#define TIMER_DO_MINUTELY_WORK				60*1000				// 1분


#define MANAGE_MAX_TIME_FIXED_TERM_MINUTELY		10080				// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형) - 1달 60분 * 24시간 * 7일 (기간제 관리는 7일이하 남은 것만 처리)

struct MEX_TIMER_EVENT
{
	TimerEventType		Type;
	UID32_t				TimerUID;				// 타이머 아이디
	TimeUnit_t			StartTimeStamp;			// milli-seconds
	TimeUnit_t			ExpireTime;				// milli-seconds
	TimeUnit_t			TimeInterval;			// milli-seconds, (ExpireTime - StartTimeStamp)
	UID32_t				CharacterUniqueNumber;	// event를 시작한 charac이 나가고 다른 charac이 socket을 사용하는 경우를 막기 위해, IsUsing()과 CharacterUniqueNumber가 같아야 함!
	ClientIndex_t		ClientIndex;			// event를 시작한 charac이 나가고 다른 charac이 socket을 사용하는 경우를 막기 위해, IsUsing()과 ClientIndex가 같아야 함!
	UINT				nGeneralParam1;			// end_skill: ITEM_SKILL* ,	mine: CMapBlock*,	delete_item류: ITEM_GENERAL*, GRADUAL_SHIELD_UP: HI(ItemUID), GRADUAL_XXX_UP: ITEM*, DELETE_DUMMY: FIELD_DUMMY*
	UINT				nGeneralParam2;			// end_skill: ,				mine: DROPMINE*,	delete_item류:              , GRADUAL_SHIELD_UP: LO(ItemUID)
	UINT				nGeneralParam3;			// end_skill: ,				mine: CFieldIOCP*,	delete_item류:              , GRADUAL_SHIELD_UP: ItemNum
	bool				bRepeatable;			// repeat해야 하는지 여부
	INT					nRemainedRepeatCount;	// 남은 횟수, ~0(0xFFFFFFFF): 무한 반복, 0이하: 정지, 1이상: 회수만큼 반복
};

struct MSG_FC_TIMER_START_TIMER
{
	MEX_TIMER_EVENT		TimerEvent;
};			// F->C, TIMER_EVENT 시작

struct MSG_FC_TIMER_STOP_TIMER
{
	UID32_t				TimerUID;
};			// F->C, TIMER_EVENT 정지

struct MSG_FC_TIMER_UPDATE_TIMER
{
	MEX_TIMER_EVENT		TimerEvent;
};		// F->C, TIMER_EVENT 갱신(시간 연장)

struct MSG_FC_TIMER_PAUSE_TIMER
{
	UID32_t				TimerUID;
};			// F->C, TIMER_EVENT 일시 정지

struct MSG_FC_TIMER_CONTINUE_TIMER
{
	UID32_t				TimerUID;
};		// F->C, TIMER_EVENT 재시작

struct MSG_FC_TIMER_TIMEOUT
{
	MEX_TIMER_EVENT		TimerEvent;
};				// C->F, 시간이 다 됨을 알림

				///////////////////////////////////////////////////////////////////////////////
				// FC_CLIENT_REPORT
struct MSG_FC_CLIENT_REPORT
{
	char			ClientPrivateIP[SIZE_MAX_IPADDRESS];	// 사설망인 경우 private ip
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	char			Message[SIZE_MAX_CLIENT_REPORT];
};		// C -> F

///////////////////////////////////////////////////////////////////////////////
// STRING128 - DEBUG Message
struct MSG_FC_STRING_128
{
	BYTE			PrintType;		// STR128_PRINT_TYPE_XXX
	char			String[128];
};
struct MSG_FC_STRING_256
{
	BYTE			PrintType;		// STR128_PRINT_TYPE_XXX
	char			String[256];
};
struct MSG_FC_STRING_512
{
	BYTE			PrintType;		// STR128_PRINT_TYPE_XXX
	char			String[512];
};

struct MSF_FC_WORLD_NOTIFICATION
{
	char	String[512];
	bool	Notification;
};

#define STR128_PRINT_TYPE_CHAT		(BYTE)0x00	// 채팅창에 출력
#define STR128_PRINT_TYPE_STATIC	(BYTE)0x01	// 고정 text로 출력
#define STR128_PRINT_TYPE_DBGOUT	(BYTE)0x02	// 클라이언트 DBGOUT 출력 - // 2012-02-13 by hskim, 몬스터 아이템 드랍 테스트 명령어
#define STR128_PRINT_TYPE_NOTICE	(BYTE)0x03	// 클라이언트 공지 스크롤에 올림 - // 2012-03-30 by hskim, EP4 트리거 시스템 이벤트 알림
#define STR128_PRINT_TYPE_POPUP		(BYTE)0x04	// 클라이언트 공지 팝업으루 뛰움 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - STR128_PRINT_TYPE_POPUP 추가

using MSG_IC_STRING_128 = MSG_FC_STRING_128;
using MSG_IC_STRING_256 = MSG_FC_STRING_256;
using MSG_IC_STRING_512 = MSG_FC_STRING_512;

///////////////////////////////////////////////////////////////////////////////
// FI_ADMIN
struct MSG_FI_ADMIN_GET_CHARACTER_INFO
{
	BYTE	Type;			// see below
	UID32_t	AdminCharacterUniqueNumber;
	UID32_t	CharacterUniqueNumber;
};

#define ADMIN_GET_CHARAC_TYPE_SEND	 (BYTE)0	// 그냥 전송용
#define ADMIN_GET_CHARAC_TYPE_MOVETO (BYTE)1	// 워프용

struct MSG_FI_ADMIN_GET_CHARACTER_INFO_OK
{
	BYTE		Type;		// see above
	UID32_t		AdminCharacterUniqueNumber;
	char		AccountName[SIZE_MAX_ACCOUNT_NAME];
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
	MAP_CHANNEL_INDEX	MapChannelIndex;
	AVECTOR3	Position;
	BYTE		Level;
};

struct MSG_FI_ADMIN_CALL_CHARACTER
{
	UID32_t			CharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;			// admin의 맵이름
	AVECTOR3		Position;						// admin의 좌표
};	// I -> F

struct MSG_FI_ADMIN_MOVETO_CHARACTER
{
	UID32_t			AdminCharacterUniqueNumber;
	MAP_CHANNEL_INDEX	MapChannelIndex;			// Target의 맵이름
	AVECTOR3		Position;						// Target의 좌표
};

struct MSG_FI_ADMIN_CHANGE_WEATHER
{
	MapIndex_t		MapIndex;		// MAP_INDEX_ALL 이면 모든 맵 의미
	BitFlag16_t		MapWeather;
	bool			OnOff;
};

///////////////////////////////////////////////////////////////////////////////
// IC_ADMIN
struct MSG_IC_ADMIN_CALL_CHARACTER
{
	char				CharacterName[SIZE_MAX_CHARACTER_NAME];
	MAP_CHANNEL_INDEX	MapChannelIndex;				// admin의 맵이름
	AVECTOR3			Position;						// admin의 좌표
};	// I -> C

struct MSG_IC_ADMIN_GET_SERVER_STAT_OK
{
	INT					TotalUserCount;				// 서버군 전체의 사용자 수
	INT					BCUUserCount;
	INT					ANIUSerCount;
};					// I -> C

struct MSG_IC_ADMIN_CALL_GUILD
{
	char				GuildName[SIZE_MAX_GUILD_NAME];
	MAP_CHANNEL_INDEX	MapChannelIndex;				// admin의 맵이름
	AVECTOR3			Position;						// admin의 좌표
};	// I -> C

struct MSG_IC_ADMIN_CALLGM_INFO_OK		// 2006-05-08 by cmkwon
{
	int		nCallGMTotalCount;
};

#define SIZE_MAX_CALLGM_LIST					1000			// 2006-05-08 by cmkwon
#define SIZE_MAX_CALLGM_BRING_COUNT				10				// 2006-05-08 by cmkwon
struct SCALLGM			// 2006-05-08 by cmkwon
{
	UID32_t			CallerCharacterUID;
	BYTE			CallerInflTy;			// 2007-11-19 by cmkwon, 진정시스템 업데이트 - SCALLGM 구조체에 세력 필드 추가
	char			szCallerCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	atimeCallTime;
	char			szCallGMDescription[SIZE_MAX_CHAT_MESSAGE];
};
struct MSG_IC_ADMIN_CALLGM_VIEW_OK		// 2006-05-08 by cmkwon
{
	int		nCallGMCount;
	ARRAY_(SCALLGM);
};
using MSG_IC_ADMIN_CALLGM_BRING_OK = MSG_IC_ADMIN_CALLGM_VIEW_OK;		// 2006-05-08 by cmkwon


																		///////////////////////////////////////////////////////////////////////////////
																		// FC_ADMIN
struct MSG_FC_ADMIN_GET_SERVER_STAT_OK
{
	MAP_CHANNEL_INDEX	CurrentMapChannelIndex;		// 현재 맵
	INT					CurrentUserCount;			// 현재 맵의 사용자 수
	INT					CurrentBCUUserCount;
	INT					CurrentANIUserCount;
};					// F -> C

					///////////////////////////////////////////////////////////////////////////////
					// ERROR
struct MSG_ERROR
{
	MessageType_t	MsgType;	// 에러가 일어났을 때 처리중이었던 Message Type
	Err_t			ErrorCode;			// 2 bytes
	bool			CloseConnection;	// Client에서 연결을 끊을지에 대한 여부
	INT				ErrParam1;			// general-purpose err parameter 1
	INT				ErrParam2;			// general-purpose err parameter 2
	USHORT			StringLength;		// 2 bytes
										//char*	String;				// error string
};

///////////////////////////////////////////////////////////////////////////////
// Log Server Protocol (T0: 0x20~)
///////////////////////////////////////////////////////////////////////////////
/*
- Item:		습득(거래,줍는경우), 아이템창->창고,창고->아이템창, 소유권분실(거래시 상대정보, 바닥에 떨어뜨릴 경우 좌표)
- Skill:	습득, 소유권 분실
- Level:	Up(n), down(-n)
- Exp:		Up(n), down(-n)
- Stat:		6개(AttackPart, DefensePart, FuelPart, SoulPart, ShieldPart, DodgePart), type,n
- EndGame:	접속시간, 종료시간, IP, PlayTime, 좌표
*/


// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
// enum EN_ENCHANT_LOGTYPE
// {
// 	ENCHANT_LOGTYPE_FAIL		= 0,
// 	ENCHANT_LOGTYPE_SUCCESS		= 1,
// 	ENCHANT_LOGTYPE_TARGETITEM	= 2,
// 	ENCHANT_LOGTYPE_ATTACHITEM	= 3,
// 	ENCHANT_LOGTYPE_INCREASE_PROBABILITY = 4		// 2009-01-19 by dhjin, 인첸트 확률 증가, 10인첸 파방 카드
// };
///////////////////////////////////////////////////////////////////////////////
// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
enum EN_ENCHANT_LOGTYPE
{
	ENCHANT_LOGTYPE_UNKNOWN = 0,		// 
	ENCHANT_LOGTYPE_ENCHANT_FAIL = 100,		// 인챈트 실패
	ENCHANT_LOGTYPE_ENCHANT_SUCCESS = 101,		// 인챈트 성공
	ENCHANT_LOGTYPE_ENCHANT_INIT_FAIL = 110,		// 인챈트 초기화 성공
	ENCHANT_LOGTYPE_ENCHANT_INIT_SUCCESS = 111,
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_FAIL = 120,		// 접두 겜블 성공
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_SUCCESS = 121,
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_FAIL = 122,		// 접미 겜블 성공
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_SUCCESS = 123,
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_FAIL = 130,		// 접두 겜블 초기화 성공
	ENCHANT_LOGTYPE_GAMBLE_PREFIX_INIT_SUCCESS = 131,
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_FAIL = 132,		// 접미 겜블 초기화 성공
	ENCHANT_LOGTYPE_GAMBLE_SUFFIX_INIT_SUCCESS = 133
};
char *GetEnchatLogTypeString(int i_nEnchantLogTy);		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
int GetEnchantLogType(ITEM *i_pEnchantItemInfo, bool i_bSuccessFlag);



// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
enum EN_TRY_ENCHANT_SUCCESS_LOGTYPE
{
	TRY_ENCHANT_FAIL = 0,
	TRY_ENCHANT_SUCCESS = 1,
	TRY_ENCHANT_PREVENT_5 = 5,
	TRY_ENCHANT_PREVENT_10 = 10
};
char *GetTryEnchantSuccessTypeString(int i_nSuccessType);
// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완


// CONNECTION LOG(0x00~)
//#define T1_FL_LOG_LOGIN							0x00	// 0 - 로그인
//#define T1_FL_LOG_LOGOUT						0x01	// 1 - 로그아웃
// USER LOG(0x10~)
//#define T1_FL_LOG_LEVEL							0x10	// 16 - 레벨 변화
//#define T1_FL_LOG_EXP							0x11	// 17 - 경험치 변화
//#define T1_FL_LOG_STAT							0x12	// 18 - 스탯 사용
//#define T1_FL_LOG_GAME_START					0x13	// 19 - 게임 시작
//#define T1_FL_LOG_GAME_END						0x14	// 20 - 게임 종료
//#define T1_FL_LOG_QUEST_COMPLETION				0x15	// 21 - 퀘스트 완료
//#define T1_FL_LOG_PKWIN							0x16	// 22 - 1:1 결투 승리
//#define T1_FL_LOG_PKLOSS						0x17	// 23 - 1:1 결투 패배
//#define T1_FL_LOG_DEAD							0x18	// 24 - 기체 폭파
//#define T1_FL_LOG_WARP							0x19	// 25 - 워프
//#define T1_FL_LOG_SPEED_HACK					0x1A	// 26 - 스피드 핵
//#define T1_FL_LOG_CREATE_CHARACTER				0x1B	// 27 - 캐릭터 생성
//#define T1_FL_LOG_DELETE_CHARACTER				0x1C	// 28 - 캐릭터 삭제
//#define T1_FL_LOG_MEMORY_HACK					0x1D	// 29 - 메모리핵, 2005-12-20 by cmkwon
//#define T1_FL_LOG_PvEWIN						0x1E	// 30 - 세력전에서 상대 세력 캐릭터를 죽임, 2006-03-06 by cmkwon
//#define T1_FL_LOG_PvELOSS						0x1F	// 31 - 세력전에서 상대 세력에 의해 죽음, 2006-03-06 by cmkwon
// ITEM LOG(0x20~)
//#define T1_FL_LOG_ITEM_CHARACTER_ITEM			0x20	// 32 - 모든 소유 아이템 저장
//#define T1_FL_LOG_ITEM_GET_ITEM					0x21	// 33 - Drop Item 습득
//#define T1_FL_LOG_ITEM_TRADE_SEND				0x22	// 34 - 아이템 거래: 주기
//#define T1_FL_LOG_ITEM_TRADE_RECV				0x23	// 35 - 아이템 거래: 받기
//#define T1_FL_LOG_ITEM_THROWAWAY_ITEM			0x24	// 36 - 아이템 버리기
//#define T1_FL_LOG_ITEM_BUY_ITEM					0x25	// 37 - 아이템 구매
//#define T1_FL_LOG_ITEM_SELL_ITEM				0x26	// 38 - 아이템 판매
//#define T1_FL_LOG_ITEM_USE_ITEM					0x27	// 39 - 아이템 사용
//#define T1_FL_LOG_ITEM_USE_ENERGY				0x28	// 40 - 에너지류 아이템 사용
//#define T1_FL_LOG_ITEM_USE_ENCHANT				0x29	// 41 - 인챈트류 아이템 사용
//#define T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN		0x2A	// 42 - 운영툴에 의한 습득
//#define T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN		0x2B	// 43 - 운영툴에 의한 제거
//#define T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND		0x2C	// 44 - 명령어에 의한 습득
//#define T1_FL_LOG_ITEM_INSERT_TO_STORE			0x2D	// 45 - 창고에 삽입
//#define T1_FL_LOG_ITEM_DRAW_FROM_STORE			0x2E	// 46 - 창고에서 꺼내기
//#define T1_FL_LOG_ITEM_STORE_ITEM				0x2F	// 47 - 창고 소유 아이템 저장, 창고 시작 아이템
//#define T1_FL_LOG_ITEM_USE_MIX					0x30	// 48 - 조합
//#define T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN	0x31	// 49 - 캐릭명 변경시 오리지날 캐릭명
//#define T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW	0x32	// 50 - 캐릭명 변경시 새로운 캐릭명
//#define T1_FL_LOG_ITEM_BAZAAR_SEND				0x33	// 51 - 개인상점 거래: 주기, 2006-07-27 by cmkwon
//#define T1_FL_LOG_ITEM_BAZAAR_RECV				0x34	// 52 - 개인상점 거래: 받기, 2006-07-27 by cmkwon
//#define T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE		0x35	// 53 - 여단 창고에 삽입, 2006-09-27 by dhjin
//#define T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE		0x36	// 54 - 여단 창고에서 꺼내기, 2006-09-27 by dhjin
//#define T1_FL_LOG_ITEM_SPI						0x37	// 55 - 캐릭터 죽을때 다운 SPI Log, 2006-10-27 by cmkwon
//#define T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT	0x38	// 56 - ItemEvent에 의한 아이템 추가, 2007-01-11 by cmkwon
//#define T1_FL_LOG_QUEST_COMPENSATION			0x39	// 57 - 퀘스트 보상, 2007-01-16 by dhjin
//#define T1_FL_LOG_WARPOINT						0x3A	// 58 - WarPoint 변경, 2007-05-15 by dhjin
//#define T1_FL_LOG_ARENA_TEAM_MATCH				0x3B	// 59 - Arena 팀 매칭, 2007-06-11 by dhjin
//#define T1_FL_LOG_TUTORIAL_COMPLETION			0x3C	// 60 - Tutorial, 2007-07-06 by dhjin
//#define T1_FL_LOG_OUTPOST_START					0x3D	// 61 - OutPost시작 로그, 2007-10-02 by dhjin
//#define T1_FL_LOG_OUTPOST_END					0x3E	// 62 - OutPost종료 로그, 2007-10-02 by dhjin
//#define T1_FL_LOG_EXPEDIENCYFUND				0x3F	// 63 - 판공비 증/감 로그, 2007-10-02 by dhjin

//#define T1_FL_LOG_ITEM_BUY_CASH_PRICE			0x46	// 70 - // 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
//#define T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT 0x49 // 73 - // 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
//#define T1_FL_LOG_ITEM_ENCHANT_BY_ADMIN			0x47	// 71 - 아이템 인챈트 증가 Log 추가
//#define T1_FL_LOG_ITEM_DISENCHANT_BY_ADMIN		0x48	// 72 - 아이템 인챈트 감소 Log 추가
// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능. End 

//#define T1_FL_LOG_ITEM_TRY_ENCHANT_INFO			0x4B	// 75 - // 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
//#define T1_FL_LOG_ITEM_DISSOLUTION_ITEM			0x4C	// 76 - // 2013-08-05 by bckim, 용해 대상아이템 로그 추가


// ETC LOG(0x40~)
//#define T1_FL_LOG_SERVER_INFO_MAP				0x40	// 64 - 맵별 동접수
//#define T1_FL_LOG_SERVER_INFO_TOTAL				0x41	// 65 - 
//#define T1_FL_LOG_HACKING_LOG					0x42	// 66 - 
//#define T1_FL_LOG_LIVE_DELETED_CHARACTER		0x43	// 67 - 복구된 케릭터, 2007-02-22 by dhjin
//#define T1_FL_LOG_EVENT_PARTICIPATION_RATE		0x44	// 68 - // 2010-06-01 by shcho, GLogDB 관련 -

// MONSTER LOG(0x50~)
//#define T1_FL_LOG_MONSTER_BOSS					0x50	// 80 - 보스급 몬스터
//#define T1_FL_LOG_MS_WAR						0x51	// 81 - // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
//#define T1_FL_LOG_SP_WAR						0x52	// 82 - // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
//#define T1_FL_LOG_WAR_CONTRIBUTION				0x53	// 83 - // 2008-12-23 by dhjin, 전쟁 보상 추가안
//#define T1_FL_LOG_WAR_CONTRIBUTION_GEAR			0x54	// 84 - // 2008-12-23 by dhjin, 전쟁 보상 추가안

//
//#define T1_FL_LOG_SERVER_INTEGRAION							0x80	// 128 - 서버 통합 관련 - 캐릭터 이전 처리 함
//#define T1_FL_LOG_SERVER_INTEGRAION_GUILD_STORE_ITEMS		0x81	// 129 - 서버 통합 관련 - 여단 창고 아이템 이전 처리함, // 2007-02-15 by cmkwon
//#define T1_FL_LOG_SERVER_INTEGRAION_ONLY_CASH_STORE_ITEMS	0x82	// 130 - 서버 통합 관련 - 캐쉬 아이템만 통합서버군 DB 로 이동, // 2008-02-28 by cmkwon, 예당 서버군 통합 캐쉬 아이템만 이동하는 프로시저

//#define T1_FL_LOG_INFLWAR_START							0x90	// 144 - // 2007-10-16 by cmkwon, 로그 추가 - 세력전 로그
//#define T1_FL_LOG_INFLWAR_END							0x91	// 145 - // 2007-10-16 by cmkwon, 로그 추가 - 세력전 로그
//#define T1_FL_LOG_OUTPOSTWAR_RESET_START				0x95	// 149 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY				0x96	// 150 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE				0x97	// 151 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY			0x98	// 152 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME				0x99	// 153 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그

//#define T1_FL_LOG_POLL_VOTE								0xA0	// 160 - // 2007-11-08 by dhjin, 투표 로그 
//#define T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE			0xA1	// 161 - // 2007-11-08 by dhjin, 지도자 출마 탈퇴 로그

//#define T1_FL_LOG_DISMEMBER_GUILD						0xA2	// 162 - // 2007-11-09 by dhjin
//#define T1_FL_LOG_NOTIFY_MSG_DELETE						0xA3	// 163 - // 2007-11-28 by cmkwon, 통지시스템 구현 -
//#define T1_FL_LOG_USE_COUPON							0xA4	// 164 - // 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - 쿠폰 사용된 로그

//#define T1_FL_LOG_ITEM_GIFT_SEND						0xB0	// 176 - // 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 : 주기
//#define T1_FL_LOG_ITEM_GIFT_RECV						0xB1	// 177 - // 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 : 받기
//#define T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM			0xB2	// 178 - // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 지워진 아이템
//#define T1_FL_LOG_ITEM_REARRANGE_ITEM					0xB3	// 179 - // 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 합쳐진 아이템
//#define T1_FL_LOG_ITEM_ATTACH_ITEM						0xB4	// 180 - // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 장착
//#define T1_FL_LOG_ITEM_DETACH_ITEM						0xB5	// 181 - // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 장착해제
//#define T1_FL_LOG_ITEM_DELETE							0xB6	// 182 - // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 아이템 삭제됨(유효시간 경과)

//#define T1_FL_LOG_LUCKY_DROP							0xC0	// 192 - // 2009-03-31 by dhjin, 럭키머신 로그
//#define T1_FL_LOG_INFINITY_START						0xC1	// 193 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
//#define T1_FL_LOG_INFINITY_START_ITEM					0xC2	// 194 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
//#define T1_FL_LOG_INFINITY_CINEMA						0xC3	// 195 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
//#define T1_FL_LOG_MONSTER_SKILL							0xC4	// 196 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
//#define T1_FL_LOG_HPACTION_TALK							0xC5	// 197 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
//#define T1_FL_LOG_INFINITY_FIN							0xC6	// 198 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
//#define T1_FL_LOG_INFINITY_FIN_ITEM						0xC7	// 199 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
//#define T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER		0xC8	// 200 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
//#define T1_FL_LOG_INFINITY_LEAVE_ITEM					0xC9	// 201 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보

//#define T1_FL_LOG_BLOCK_ACCOUNT							0xD0	// 208 - // 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 계정 블럭
//#define T1_FL_LOG_UNBLOCK_ACCOUNT						0xD1	// 209 - // 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 계정 블럭 해제

//#define T1_FL_LOG_PET_LEVEL								0xD5	// 213 - // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경

//#define T1_FL_LOG_CONNECT_USER_NEW						0xD6	// 214 - // 2012-01-08 by hskim, GLog 2차 - 유저 신규 접속
//#define T1_FL_LOG_CONNECT_USER_LOGIN					0xD7	// 215 - // 2012-01-08 by hskim, GLog 2차 - 유저 접속

//#define T1_FL_LOG_STATISTICS_MONEY						0xD8	// 216 - // 2012-01-16 by hskim, 통계 - 화패

//#define T1_FL_LOG_FIXED_TERM_SHAPE_START				0xD9	// 217 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
//#define T1_FL_LOG_FIXED_TERM_SHAPE_END					0xDA	// 218 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

// 2013-1-02 by khkim, GLog
//#define T1_FL_LOG_ACCOUNTCONNECT						0xDB		 
//#define T1_FL_LOG_CHARDATA								0xDC	
//#define	T1_FL_LOG_ITEM									0xDD
//#define	T1_FL_LOG_ITEMSTATE								0xDE
//#define	T1_FL_LOG_SERVER								0xDF
// end 2013-1-02 by khkim, GLog


//#define T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE			0xE0	// 224 - // 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
//#define T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT			0xE1	// 225 - // 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가

//#define T1_FL_LOG_COLLECTION_STATE						0xE2	// 226 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
//#define T1_FL_LOG_COLLECTION_SHAPE_CHANGE				0xE3	// 227 - // 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
//#define T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN			0xE4	// 228 - // 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가

//#define T1_FL_LOG_START_FIELD_SERVER			0xFF	// 255 - FieldServer Start



//#define T_FL_LOG_LOGIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LOGIN)				// 로그인
//#define T_FL_LOG_LOGOUT					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LOGOUT)			// 로그아웃
//#define T_FL_LOG_LEVEL					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LEVEL)				// 레벨 변화
//#define T_FL_LOG_EXP					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_EXP)				// 경험치 변화
//#define T_FL_LOG_STAT					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_STAT)				// 스탯 사용
//#define T_FL_LOG_GAME_START				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_GAME_START)		// 게임 시작
//#define T_FL_LOG_GAME_END				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_GAME_END)			// 게임 종료
//#define T_FL_LOG_QUEST_COMPLETION		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_QUEST_COMPLETION)	// 퀘스트 완료
//#define T_FL_LOG_QUEST_COMPENSATION		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_QUEST_COMPENSATION)// 퀘스트 보상
//#define T_FL_LOG_PKWIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PKWIN)				// 1:1 결투 승리
//#define T_FL_LOG_PKLOSS					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PKLOSS)			// 1:1 결투 패배
//#define T_FL_LOG_DEAD					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_DEAD)				// 기체 폭파
//#define T_FL_LOG_WARP					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WARP)				// 워프
//#define T_FL_LOG_SPEED_HACK				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SPEED_HACK)		// 스피드 핵
//#define T_FL_LOG_CREATE_CHARACTER		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_CREATE_CHARACTER)	// 캐릭터 생성
//#define T_FL_LOG_DELETE_CHARACTER		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_DELETE_CHARACTER)	// 캐릭터 삭제
//#define T_FL_LOG_MEMORY_HACK			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MEMORY_HACK)		// 29 - 메모리핵, 2005-12-20 by cmkwon
//#define T_FL_LOG_PvEWIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PvEWIN)			// 30 - 세력전에서 상대 세력 캐릭터를 죽임, 2006-03-06 by cmkwon
//#define T_FL_LOG_PvELOSS				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PvELOSS)			// 31 - 세력전에서 상대 세력에 의해 죽음, 2006-03-06 by cmkwon

//#define T_FL_LOG_ITEM_CHARACTER_ITEM	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_CHARACTER_ITEM)	// 모든 소유 아이템 저장
//#define T_FL_LOG_ITEM_GET_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_GET_ITEM)		// Drop Item 습득
//#define T_FL_LOG_ITEM_TRADE_SEND		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_TRADE_SEND)	// 아이템 거래: 주기
//#define T_FL_LOG_ITEM_TRADE_RECV		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_TRADE_RECV)	// 아이템 거래: 받기
//#define T_FL_LOG_ITEM_THROWAWAY_ITEM	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_THROWAWAY_ITEM)	// 아이템 버리기

//#define T_FL_LOG_ITEM_TRY_ENCHANT_INFO	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_TRY_ENCHANT_INFO)	// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
//#define T_FL_LOG_ITEM_DISSOLUTION_ITEM	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DISSOLUTION_ITEM)	// 2013-08-05 by bckim, 용해 대상아이템 로그 추가

//#define T_FL_LOG_ITEM_BUY_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BUY_ITEM)		// 아이템 구매

//#define T_FL_LOG_ITEM_BUY_CASH_PRICE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BUY_CASH_PRICE)	// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
//#define T_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT)	// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
//#define T_FL_LOG_ITEM_INITIALIZED_GUILD_STORE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_INITIALIZED_GUILD_STORE)	// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
//#define T_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT)	// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가

//#define T_FL_LOG_ITEM_SELL_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_SELL_ITEM)	// 아이템 판매
//#define T_FL_LOG_ITEM_USE_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_ITEM)		// 아이템 사용
//#define T_FL_LOG_ITEM_USE_ENERGY		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_ENERGY)	// 에너지류 아이템 사용
//#define T_FL_LOG_ITEM_USE_ENCHANT		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_ENCHANT)	// 인챈트류 아이템 사용
//#define T_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN)		// 운영툴에 의한 습득
//#define T_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN)		// 운영툴에 의한 제거
//#define T_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND)	// 명령어에 의한 습득
//#define T_FL_LOG_ITEM_INSERT_TO_STORE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_INSERT_TO_STORE)	// 창고에 삽입
//#define T_FL_LOG_ITEM_DRAW_FROM_STORE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DRAW_FROM_STORE)	// 창고에서 꺼내기
//#define T_FL_LOG_ITEM_STORE_ITEM		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_STORE_ITEM)		// 창고 소유 아이템 저장, 창고 시작 아이템
//#define T_FL_LOG_ITEM_USE_MIX			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_MIX)			// 조합
//#define T_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN)	// 캐릭명변경
//#define T_FL_LOG_ITEM_BAZAAR_SEND		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BAZAAR_SEND)	// 개인상점 거래: 주기
//#define T_FL_LOG_ITEM_BAZAAR_RECV		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_BAZAAR_RECV)	// 개인상점 거래: 받기
//#define T_FL_LOG_ITEM_INSERT_TO_GUILDSTORE			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_INSERT_TO_GUILDSTORE)		// 2006-09-27 by dhjin, 여단 창고 로그 남기기
//#define T_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DRAW_FROM_GUILDSTORE)		// 2006-09-27 by dhjin, 여단 창고 로그 남기기
//#define T_FL_LOG_ITEM_SPI							(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_SPI)							// 55 - SPI Log, 2006-10-27 by cmkwon
//#define T_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT)		// 56 - ItemEvent에 의한 아이템 추가, 2007-01-11 by cmkwon
//#define T_FL_LOG_WARPOINT							(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WARPOINT)						// 58 - WarPoint 변경, 2007-05-15 by dhjin
//#define T_FL_LOG_ARENA_TEAM_MATCH					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ARENA_TEAM_MATCH)				// 59 - Arena 팀 매칭, 2007-06-11 by dhjin
//#define T_FL_LOG_TUTORIAL_COMPLETION				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_TUTORIAL_COMPLETION)			// 60 - Tutorial, 2007-07-06 by dhjin
//#define T_FL_LOG_OUTPOST_START			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOST_START)				// 61 - OutPost시작 로그, 2007-10-02 by dhjin
//#define T_FL_LOG_OUTPOST_END			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOST_END)				// 62 - OutPost종료 로그, 2007-10-02 by dhjin
//#define T_FL_LOG_EXPEDIENCYFUND			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_EXPEDIENCYFUND)			// 63 - 판공비 증/감 로그, 2007-10-02 by dhjin

//#define T_FL_LOG_SERVER_INFO_MAP		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SERVER_INFO_MAP)
//#define T_FL_LOG_SERVER_INFO_TOTAL		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SERVER_INFO_TOTAL)
//#define T_FL_LOG_HACKING_LOG			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_HACKING_LOG)

//#define T_FL_LOG_MONSTER_BOSS			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MONSTER_BOSS)			// 보스급 몬스터
//#define T_FL_LOG_MS_WAR					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MS_WAR)			// 81 - // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
//#define T_FL_LOG_SP_WAR					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_SP_WAR)			// 82 - // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
//#define T_FL_LOG_WAR_CONTRIBUTION		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WAR_CONTRIBUTION)			// 83  - // 2008-12-23 by dhjin, 전쟁 보상 추가안
//#define T_FL_LOG_WAR_CONTRIBUTION_GEAR	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_WAR_CONTRIBUTION_GEAR)		// 84  - // 2008-12-23 by dhjin, 전쟁 보상 추가안

//#define T_FL_LOG_INFLWAR_START					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFLWAR_START)					// 144 - // 2007-10-16 by cmkwon, 로그 추가 - 세력전 로그
//#define T_FL_LOG_INFLWAR_END					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFLWAR_END)					// 145 - // 2007-10-16 by cmkwon, 로그 추가 - 세력전 로그
//#define T_FL_LOG_OUTPOSTWAR_RESET_START			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_RESET_START)		// 151 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T_FL_LOG_OUTPOSTWAR_RESET_DESTROY		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_RESET_DESTROY)		// 154 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T_FL_LOG_OUTPOSTWAR_RESET_COMPLETE		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_RESET_COMPLETE)		// 152 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY)	// 153 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그
//#define T_FL_LOG_OUTPOSTWAR_SET_NEXTTIME		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_OUTPOSTWAR_SET_NEXTTIME)		// 156 - // 2007-10-16 by cmkwon, 로그 추가 - 전진기지전 로그

//#define T_FL_LOG_POLL_VOTE						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_POLL_VOTE)						// 160 - // 2007-11-08 by dhjin, 투표 로그
//#define T_FL_LOG_POLL_DELETE_LEADERCANDIDATE	(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_POLL_DELETE_LEADERCANDIDATE)	// 161 - // 2007-11-08 by dhjin, 지도자 출마 탈퇴 로그

//#define T_FL_LOG_DISMEMBER_GUILD				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_DISMEMBER_GUILD)		// 162 - // 2007-11-09 by dhjin, 여단 해체로그
//#define T_FL_LOG_NOTIFY_MSG_DELETE				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_NOTIFY_MSG_DELETE)		// 163 - // 2007-11-28 by cmkwon, 통지시스템 구현 -
//#define T_FL_LOG_USE_COUPON						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_USE_COUPON)			// 164 - // 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - 쿠폰 사용된 로그

//#define T_FL_LOG_ITEM_ATTACH_ITEM				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_ATTACH_ITEM)		// 180 - // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 장착
//#define T_FL_LOG_ITEM_DETACH_ITEM				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DETACH_ITEM)		// 181 - // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 장착해제
//#define T_FL_LOG_ITEM_DELETE					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ITEM_DELETE)			// 182 - // 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 삭제됨

//#define T_FL_LOG_LUCKY_DROP						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_LUCKY_DROP)			// 192 - // 2009-03-31 by dhjin, 럭키머신 로그

//#define T_FL_LOG_INFINITY_START					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_START)			// 193 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
//#define T_FL_LOG_INFINITY_START_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_START_ITEM)		// 194 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
//#define T_FL_LOG_INFINITY_CINEMA				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_CINEMA)			// 195 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
//#define T_FL_LOG_MONSTER_SKILL					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_MONSTER_SKILL)				// 196 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
//#define T_FL_LOG_HPACTION_TALK					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_HPACTION_TALK)				// 197 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
//#define T_FL_LOG_INFINITY_FIN					(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_FIN)				// 198 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
//#define T_FL_LOG_INFINITY_FIN_ITEM				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_FIN_ITEM)			// 199 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보
//#define T_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER)	// 200 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
//#define T_FL_LOG_INFINITY_LEAVE_ITEM			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_INFINITY_LEAVE_ITEM)		// 201 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 인피 탈퇴시 아이템 복사 정보

//#define T_FL_LOG_PET_LEVEL						(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_PET_LEVEL)					// 213 - // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경

//#define T_FL_LOG_CONNECT_USER_NEW				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_CONNECT_USER_NEW)			// 214 - // 2012-01-08 by hskim, GLog 2차 - 유저 신규 접속
//#define T_FL_LOG_CONNECT_USER_LOGIN				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_CONNECT_USER_LOGIN)		// 215 - // 2012-01-08 by hskim, GLog 2차 - 유저 접속

//#define T_FL_LOG_STATISTICS_MONEY				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_STATISTICS_MONEY)			// 216 - // 2012-01-16 by hskim, 통계 - 화패

//#define T_FL_LOG_FIXED_TERM_SHAPE_START			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_FIXED_TERM_SHAPE_START)	// 217 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
//#define T_FL_LOG_FIXED_TERM_SHAPE_END			(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_FIXED_TERM_SHAPE_END)		// 218 - // 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

//#define T_FL_LOG_START_FIELD_SERVER				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_START_FIELD_SERVER)		// FieldServer Start

// start 2012-10-08 by khkim, GLog
//#define T_FL_LOG_ACCOUNTCONNECT					(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_ACCOUNTCONNECT)			// 218		2012-10-08 by khkim, GLog
//#define T_FL_LOG_CHARDATA						(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_CHARDATA)					// 219		
//#define T_FL_LOG_ITEM							(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_ITEM)						// 220
//#define T_FL_LOG_ITEMSTATE						(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_ITEMSTATE)				// 221	
//#define T_FL_LOG_SERVER							(MessageType_t)((T0_FL_LOG<<8)| T1_FL_LOG_SERVER)					// 222
// end 2012-10-08 by khkim, GLog

//#define T_FL_LOG_COLLECTION_STATE				(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_COLLECTION_STATE)			// 226		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
//#define T_FL_LOG_COLLECTION_SHAPE_CHANGE		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_COLLECTION_SHAPE_CHANGE)	// 227		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - GameLog 추가
//#define T_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN)	// 228 - // 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가

////////////////////////////////
// CONNECTION LOG(0x00~)
struct MSG_FL_LOG_LOGIN
{
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	BYTE	IPAddress[4];
	CHAR	ServerGroupName[SIZE_MAX_SERVER_NAME];
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];	// 2015-09-23 Future, added logging of MAC Address
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%u|%s|%u.%u.%u.%u|%s %s\r\n", GetGameLogTypeString(i_nLogType), AccountUniqueNumber, AccountName
			, IPAddress[0], IPAddress[1], IPAddress[2], IPAddress[3], ServerGroupName, MACAddress);
		return o_szLogString;
	}
};					// 로그인

struct MSG_FL_LOG_LOGOUT
{
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	BYTE	IPAddress[4];
	CHAR	ServerGroupName[SIZE_MAX_SERVER_NAME];
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];	// 2015-09-23 Future, added logging of MAC Address
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%u|%s|%u.%u.%u.%u|%s %s\r\n", GetGameLogTypeString(i_nLogType), AccountUniqueNumber, AccountName
			, IPAddress[0], IPAddress[1], IPAddress[2], IPAddress[3], ServerGroupName, MACAddress);
		return o_szLogString;
	}
};				// 로그아웃

				////////////////////////////////
				// USER LOG(0x10~)
struct FL_USER_LOG_BASE
{
	UID32_t		CharacterUniqueNumber;
	MapIndex_t	CurrentMapIndex;
	ChannelIndex_t	CurrentChannIdx;		// 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 - FL_USER_LOG_BASE 에 필드 추가
	AVECTOR3	CurrentPosition;
	char *GetLogBaseString(char *o_szLogString)
	{
		// 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 - 게임 로그 내용 수정
		sprintf(o_szLogString, "%d|%d|%d|(%d, %d, %d)", CharacterUniqueNumber, CurrentMapIndex, CurrentChannIdx, CurrentPosition.x, CurrentPosition.y, CurrentPosition.z);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_LEVEL : public FL_USER_LOG_BASE
{
	BYTE		FromLevel;
	BYTE		ToLevel;
	LONGLONG	TotalPlayTime;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), FromLevel, ToLevel, TotalPlayTime);
		return o_szLogString;
	}
};					// 레벨 변화

struct MSG_FL_LOG_EXP : public FL_USER_LOG_BASE
{
	float		AcquiredExperience;	// 습득 경험치
	LONGLONG	CharacterExperiece;	// 최종 경험치, Experience_t이나 DB에는 BIGINT로 저장
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%.0f|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), AcquiredExperience, CharacterExperiece);
		return o_szLogString;
	}
};					// 경험치 변화

struct MSG_FL_LOG_STAT : public FL_USER_LOG_BASE
{
	BYTE	StatKind;				// STAT_XXX, 스탯의 종류
	BYTE	RemainedBonusStat;		// 쓰고 남은 보너스 스탯
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|0\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), StatKind, RemainedBonusStat);
		return o_szLogString;
	}
};					// 스탯 사용

struct MSG_FL_LOG_GAME_START : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	LONGLONG	TotalPlayTime;		// 총 게임 시간
	UID32_t		PCBangUID;			// 2007-01-22 by dhjin, 가맹 PCBang UID
	INT			GameContinueTimeOfToday;	// 2007-11-07 by cmkwon, 오늘게임지속시간 게임로그에 추가 - 필드 추가
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		// 2007-11-07 by cmkwon, 오늘게임지속시간 게임로그에 추가 - GameContinueTimeOfToday 추가
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|0|%I64d|%u|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName
			, AccountUniqueNumber, AccountName, TotalPlayTime, PCBangUID, GameContinueTimeOfToday);
		return o_szLogString;
	}
};			// 게임 시작

struct MSG_FL_LOG_GAME_END : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	UINT	PlayTime;				// 이전 게임 시작 후 게임 시간
	LONGLONG	TotalPlayTime;		// 총 게임 시간
	UINT		PCBangPlayTime;		// 2006-12-18 by cmkwon, 가맹 PCBang에서 게임시간
	UID32_t		PCBangUID;			// 2007-01-22 by dhjin, 가맹 PCBang UID
	INT			GameContinueTimeOfToday;	// 2007-11-07 by cmkwon, 오늘게임지속시간 게임로그에 추가 - 필드 추가
	ATUM_DATE_TIME	LoginDate;								// 2010-06-01 by shcho, GLogDB 관련 -		
	CHAR			CharacterRace[SIZE_MAX_GLOG_CHARACTER_RACE_NAME];		// 2010-06-01 by shcho, GLogDB 관련 -
	CHAR			CharacterClass[SIZE_MAX_GLOG_CHARACTER_CLASS_NAME];		// 2010-06-01 by shcho, GLogDB 관련 -
	Experience_t	PlayExp;											// 2010-06-01 by shcho, GLogDB 관련 -
	Experience_t	TotalExp;											// 2010-06-01 by shcho, GLogDB 관련 -
	Lv_t			CharacterLevel;										// 2010-06-01 by shcho, GLogDB 관련 -
	CHAR			MostStayedInZoneName[SIZE_MAX_MAP_NAME];			// 2010-06-01 by shcho, GLogDB 관련 -	

	UID32_t			CharacterUniqueNumber;								// 2011-12-12 by hskim, GLog 2차
	USHORT			Race;												// 2011-12-12 by hskim, GLog 2차
	USHORT			UnitKind;											// 2011-12-12 by hskim, GLog 2차
	BYTE			InfluenceType;										// 2011-12-12 by hskim, GLog 2차

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		// 2007-11-07 by cmkwon, 오늘게임지속시간 게임로그에 추가 - GameContinueTimeOfToday 추가
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|%u|%I64d|%u|%u|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName
			, AccountUniqueNumber, AccountName, PlayTime, TotalPlayTime, PCBangPlayTime, PCBangUID, GameContinueTimeOfToday);
		return o_szLogString;
	}
};				// 게임 종료

struct MSG_FL_LOG_QUEST_COMPLETION : public FL_USER_LOG_BASE
{
	INT		QuestIndex;				// 퀘스트 인덱스
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), QuestIndex);
		return o_szLogString;
	}
};		// 퀘스트 완료


struct MSG_FL_LOG_QUEST_COMPENSATION : public FL_USER_LOG_BASE		// 2007-04-18 by cmkwon
{
	INT					QuestIndex;				// 2007-04-18 by cmkwon
	INT					ExpOfCompensation;
	INT					BonusStatOfCompensation;
	INT					LenOfCompensation;		// 보상 아이템 수 (경험치, SPI제외)
	ARRAY_(char);								// 보상 아이템
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString, char *i_szCompensationItemList)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%d|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), QuestIndex, ExpOfCompensation, BonusStatOfCompensation, i_szCompensationItemList);
		return o_szLogString;
	}
};	// 2007-01-16 by dhjin, 보상 아이템 로그


struct MSG_FL_LOG_PKWIN : public FL_USER_LOG_BASE
{
	UID32_t	PeerCharacterUniqueNumber;	// 상대 캐릭터
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};						// 1:1 결투 승리

struct MSG_FL_LOG_PKLOSS : public FL_USER_LOG_BASE
{
	UID32_t	PeerCharacterUniqueNumber;	// 상대 캐릭터
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};					// 1:1 결투 패배

					// DAMAGE_BY_XXX
#define DAMAGE_BY_COLLISION		((BYTE)0)
#define DAMAGE_BY_MONSTER		((BYTE)1)
#define DAMAGE_BY_PK			((BYTE)2)
#define DAMAGE_BY_FUEL_ALLIN	((BYTE)3)
#define DAMAGE_BY_NA			((BYTE)10)	// 알 수 없거나 불필요한 경우

struct MSG_FL_LOG_DEAD : public FL_USER_LOG_BASE
{
	INT			DamageKind;			// DAMAGE_BY_XXX, 최종 데미지 종류(PK, 충돌, 몬스터 공격 등)
	ItemNum_t	ItemNum;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 무기 아이템 정보
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), DamageKind, ItemNum);
		return o_szLogString;
	}
};					// 기체 폭파

struct MSG_FL_LOG_WARP : public FL_USER_LOG_BASE
{
	MapIndex_t	TargetMapIndex;		// 워프할 맵
	ChannelIndex_t	TargetChannIdx;		// 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 - MSG_FL_LOG_WARP 에 필드 추가
	AVECTOR3	TargetPosition;		// 워프할 맵의 위치
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		// 2007-11-30 by cmkwon, 같은맵에 채널이 다를 경우도 워프 처리 - 게임 로그 수정
		sprintf(o_szLogString, "%s|%s|%d|%d|(%d,%d,%d)\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), TargetMapIndex, TargetChannIdx, TargetPosition.x, TargetPosition.y, TargetPosition.z);
		return o_szLogString;
	}
};					// 워프, 워프 시작 시 로그 남김

enum SPEEDHACK_CHECK_TYPE
{
	SPEEDHACK_CHECK_TYPE_BY_SERVER = 0,
	SPEEDHACK_CHECK_TYPE_BY_CLIENT = 1
};

struct MSG_FL_LOG_SPEED_HACK : public FL_USER_LOG_BASE
{
	BYTE	CheckType;
	INT 	ReceivedMoveCounts;
	INT		EngineItemNum;
	INT		ServerPastTime;
	INT		ClientPastTime;
	INT		PastDistance;
	INT		CurrentSpeed;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%d|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CheckType, ReceivedMoveCounts
			, EngineItemNum, ServerPastTime, ClientPastTime, PastDistance, CurrentSpeed);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_CREATE_CHARACTER : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	USHORT	UnitKind;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|%d|1\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName, AccountUniqueNumber
			, AccountName, UnitKind);
		return o_szLogString;
	}
};		// 캐릭터 생성

struct MSG_FL_LOG_DELETE_CHARACTER : public FL_USER_LOG_BASE
{
	CHAR	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	CHAR	AccountName[SIZE_MAX_ACCOUNT_NAME];
	USHORT	UnitKind;
	USHORT	Level;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%u|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), CharacterName, AccountUniqueNumber
			, AccountName, UnitKind, Level);
		return o_szLogString;
	}
};		// 캐릭터 삭제

struct MSG_FL_LOG_MEMORY_HACK : public FL_USER_LOG_BASE		// 2005-12-20 by cmkwon
{
	int		nItemKind;			// 체크 아이템 종류(1형무기, 2형무기, 엔진, 아머)
	int		nItemNum0;			// ItemNum
	UID64_t	uid64ItemUID;		// Item UniqueNumber
	int		nMemHackCheckType;	// 체크 타입(CHECK_TYPE_XXXXXX)
	float	fValidValue;		// 유효한 값
	float	fCurrentValue;		// 현재 값
	int		nParam1;			// 체??타입에 따라 의미가 다르다
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%d|%d|%I64d|%d|%5.2f|%5.2f|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), nItemKind, nItemNum0
			, uid64ItemUID, nMemHackCheckType, fValidValue, fCurrentValue, nParam1);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_PvEWIN : public FL_USER_LOG_BASE		// 2006-03-06 by cmkwon
{
	UID32_t	PeerCharacterUniqueNumber;	// 상대 캐릭터
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};						// 1:1 결투 승리

struct MSG_FL_LOG_PvELOSS : public FL_USER_LOG_BASE		// 2006-03-06 by cmkwon
{
	UID32_t	PeerCharacterUniqueNumber;	// 상대 캐릭터
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%u\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), PeerCharacterUniqueNumber);
		return o_szLogString;
	}
};					// 1:1 결투 패배


					///////////////////////////////////////////////////////////////////////////////
					// ITEM LOG(0x20~)
struct FL_ITEM_LOG_BASE
{
	UID32_t		CharacterUniqueNumber;
	MapIndex_t	CurrentMapIndex;
	char *GetLogItemBaseString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%u|%d", CharacterUniqueNumber, CurrentMapIndex);
		return o_szLogString;
	}
};

struct ITEM_FOR_LOG
{
	UID64_t		ItemUniqueNumber;
	INT			ItemNum;
	INT			CurrentCount;
	char *GetLogItemString(char *o_szLogString)
	{
		sprintf(o_szLogString, "%I64d|%d|%d", ItemUniqueNumber, ItemNum, CurrentCount);
		return o_szLogString;
	}

#ifdef _ATUM_SERVER
	// operator overloading
	ITEM_FOR_LOG& operator=(const ITEM_GENERAL& rhs)
	{
		this->ItemUniqueNumber = rhs.UniqueNumber;
		this->ItemNum = rhs.ItemNum;
		this->CurrentCount = rhs.CurrentCount;

		return *this;
	}
#endif // _ATUM_SERVER
};

struct MSG_FL_LOG_ITEM_CHARACTER_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// 변경 개수, 			// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - MSG_FL_LOG_ITEM_STORE_ITEM 에 필드 추가
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 
		//		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};	// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 창고-->인벤 으로 카운터블 아이템 이동시 인벤에 해당 아이템 추가 및 개수 변경시 저장, // (게임 시작 시 등에)모든 소유 아이템 저장


	// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그 
struct MSG_FL_LOG_ITEM_CHARACTER_ITEM_GUILDSTORE_RESULT : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;
	UID32_t			GuildUID;	/// 길드 UID
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];	/// 길드 이름 

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%d|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), GuildUID, GuildName, Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};
// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그.End



struct MSG_FL_LOG_ITEM_GET_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	UID32_t			StoreCharacterUID;			// 2006-11-01 by cmkwon
	INT				ChangeCount;
	BYTE			ItemUpdateType;				// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - IUT_XXX
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
		//sprintf(o_szLogString, "%s|%s|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), StoreCharacterUID, Item4Log.GetLogItemString(szItem), ChangeCount);
		sprintf(o_szLogString, "%s|%s|%d|%s|%d|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), StoreCharacterUID, Item4Log.GetLogItemString(szItem), ChangeCount, GetItemUpdateTypeString(ItemUpdateType));
		return o_szLogString;
	}
};			// Drop Item 습득

			// 2013-08-05 by bckim, 용해 대상아이템 로그 추가
struct MSG_FL_LOG_ITEM_DISSOLUTION_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;
	BYTE			ItemUpdateType;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount, GetItemUpdateTypeString(ItemUpdateType));
		return o_szLogString;
	}
};
// End. 2013-08-05 by bckim, 용해 대상아이템 로그 추가

struct MSG_FL_LOG_ITEM_GET_ITEM_2 : public FL_ITEM_LOG_BASE		// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 새로 추가함
{
	ITEM_FOR_LOG	Item4Log;
	UID32_t			StoreCharacterUID;			// 2006-11-01 by cmkwon
	INT				ChangeCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), StoreCharacterUID, Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};

// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 아래와 같이 MSG_FL_LOG_ITEM_GET_ITEM_2 로 재정의 함.
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN;		// 운영툴에 의한 습득
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN;		// 운영툴에 의한 제거
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND;	// 명령어에 의한 습득
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_INSERT_TO_STORE;		// 창고에 삽입
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_DRAW_FROM_STORE;		// 창고에서 꺼내기
using MSG_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 운영툴에 의한 습득
using MSG_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 운영툴에 의한 제거
using MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND = MSG_FL_LOG_ITEM_GET_ITEM_2;	// 명령어에 의한 습득
using MSG_FL_LOG_ITEM_INSERT_TO_STORE = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 창고에 삽입
using MSG_FL_LOG_ITEM_DRAW_FROM_STORE = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 창고에서 꺼내기

using MSG_FL_LOG_ITEM_GET_WITH_SAME_ACCOUNT = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가

struct MSG_FL_LOG_ITEM_TRADE_SEND : public FL_ITEM_LOG_BASE
{
	UID32_t			PeerCharacterUniqueNumber;	// 거래 상대
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// 거래 상대
	ITEM_FOR_LOG	Item4Log;					// 거래 아이템
	INT				ChangeCount;				// 거래 개수
	BYTE			bGiftItem;					// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 로그 플래그 변수
	BYTE			bRearrangeItem;				// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	BYTE			bStoreItem;					// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - FALSE 이면 인벤, FALSE 가 아니면 캐릭터창고
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 로그 플래그 변수
		// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
		sprintf(o_szLogString, "%s|%s|%u|%s|%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), PeerCharacterUniqueNumber
			, PeerCharacterName, Item4Log.GetLogItemString(szItem), ChangeCount, bGiftItem, bRearrangeItem, bStoreItem);
		return o_szLogString;
	}
};					// 아이템 거래: 주기

struct MSG_FL_LOG_ITEM_TRADE_RECV : public FL_ITEM_LOG_BASE
{
	UID32_t			PeerCharacterUniqueNumber;	// 거래 상대
	char			PeerCharacterName[SIZE_MAX_CHARACTER_NAME];	// 거래 상대
	ITEM_FOR_LOG	Item4Log;					// 아이템
	INT				ChangeCount;				// 거래 개수
	BYTE			bGiftItem;					// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 로그 플래그 변수
	BYTE			bRearrangeItem;				// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	BYTE			bStoreItem;					// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - FALSE 이면 인벤, FALSE 가 아니면 캐릭터창고
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 선물하기 로그 플래그 변수
		// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
		sprintf(o_szLogString, "%s|%s|%u|%s|%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), PeerCharacterUniqueNumber
			, PeerCharacterName, Item4Log.GetLogItemString(szItem), ChangeCount, bGiftItem, bRearrangeItem, bStoreItem);
		return o_szLogString;
	}
};					// 아이템 거래: 받기

					// 2007-06-05 by cmkwon, 레어정보/인챈트정보도 저장하기
struct MSG_FL_LOG_ITEM_THROWAWAY_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// 거래 개수
	INT				PrefixCodeNum;			// 2007-06-05 by cmkwon
	INT				SuffixCodeNum;			// 2007-06-05 by cmkwon
	INT				EnchatItemNumCount;		// 2007-06-05 by cmkwon, EnchantItemNum List Count
	_ARRAY(INT);							// 2007-06-05 by cmkwon, EnchantItemNum List
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount
			, PrefixCodeNum, SuffixCodeNum);
		return o_szLogString;
	}
};	// 아이템 버리기

	// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
struct MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				Success;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				EnchantCount;
	_ARRAY(INT);
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%d|%d|%d", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem),
			GetTryEnchantSuccessTypeString(Success), EnchantCount, PrefixCodeNum, SuffixCodeNum);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO_STRING
{
	MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO TryEnchantInfo;
	char szCardList[1024];
};
// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완


struct MSG_FL_LOG_ITEM_BUY_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// 거래 개수
	INT				RemainedMoney;	// 남은 돈
	bool			IsCashItem;		// 2006-06-21 by cmkwon, 유료 아이템
	INT				nRemainCountForLimitedEdition;		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	INT				CashItemPrice;							// 2010-06-01 by shcho, GLogDB 관련 -
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];		// 2010-06-01 by shcho, GLogDB 관련 -
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME]; // 2010-06-01 by shcho, GLogDB 관련 -
	INT				CharacterLevel;							// 2010-06-01 by shcho, GLogDB 관련 - GLogDB필드가 int 형이므로 Int로 받는다.
	CHAR			ItemName[SIZE_MAX_ITEM_NAME];			// 2010-06-01 by shcho, GLogDB 관련 -

	bool			GLog;									// 2011-12-12 by hskim, GLog 2차
	UID32_t			AccountUniqueNumber;					// 2011-12-12 by hskim, GLog 2차
	INT				ItemNum;								// 2011-12-12 by hskim, GLog 2차

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ChangeCount, RemainedMoney, IsCashItem, nRemainCountForLimitedEdition);
		return o_szLogString;
	}
};			// 아이템 구매


			// 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가
struct MSG_FL_LOG_ITEM_BUY_CASH_PRICE : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;								// 아이템 고유넘버, 아이템번호
	UID32_t			AccountUniqueNumber;					// 계정 고유 넘버 
	INT				nAmount;								// 아이템 구매수량 
	INT				nType;									// 지불 타입.

	INT64			nSPI;
	INT64			nWarPoint;
	INT64			nCash;
	INT64			nInfinity1;
	INT64			nInfinity2;
	INT64			nInfinity3;
	INT64			nInfinity4;
	INT64			nInfinity5;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		//						%s|%s|%d|%d   |%d|%A|%T|%S   |%W   |%C   |%1   |%2   |%3   |%4   |%5|														
		sprintf(o_szLogString, "%s|%s|%d|%I64d|%d|%d|%d|%I64d|%I64d|%I64d|%I64d|%I64d|%I64d|%I64d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp)
			, AccountUniqueNumber, Item4Log.ItemUniqueNumber, Item4Log.ItemNum, nAmount, nType
			, nSPI, nWarPoint, nCash, nInfinity1, nInfinity2, nInfinity3, nInfinity4, nInfinity5);
		return o_szLogString;
	}
};
// end 2012-10-16 by bckim, 캐쉬아이템 지불정보 Log 추가


// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그
struct MSG_FL_LOG_ITEM_INITIALIZED_GUILD_STORE
{
	// 계정UID/계정이름/케릭터UID/케릭터이름/아이템UID/아이템넘버/저장소/아이템종류/전달개수/길드넘버/길드이름
	INT			AccountUniqueNumber;
	CHAR		AccountName[SIZE_MAX_ACCOUNT_NAME];
	INT			CharacterUniqueNumber;
	CHAR		CharacterName[SIZE_MAX_CHARACTER_NAME];

	INT64		ItemUniqueNumber;
	INT			ItemNum;
	INT			Kind;
	INT			CurrentCount;

	INT			GuildUniqueNumber;
	CHAR		GuildName[SIZE_MAX_GUILD_NAME];

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%s|%I64d|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, GuildUniqueNumber, GuildName, AccountUniqueNumber, AccountName, CharacterUniqueNumber, CharacterName, ItemUniqueNumber, ItemNum, CurrentCount);
		return o_szLogString;
	}
};
// 2012-12-20 by bckim, 세력초기화시 길드창고아이템이 길드장에게 옮겨지는 아이템 로그. End



struct MSG_FL_LOG_ITEM_SELL_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// 거래 개수
	INT				RemainedMoney;	// 남은 돈
	INT				PrefixCodeNum;			// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 접두
	INT				SuffixCodeNum;			// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 접미
	INT				EnchatItemNumCount;		// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - EnchantItemNum List Count
	_ARRAY(INT);							// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - EnchantItemNum List
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2007-07-20 by cmkwon, 판매아이템 로그 추가 - 접두/접미 추가
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d|%d", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ChangeCount, RemainedMoney, PrefixCodeNum, SuffixCodeNum);
		return o_szLogString;
	}
};		// 아이템 판매

struct MSG_FL_LOG_ITEM_USE_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		return o_szLogString;
	}
};			// 아이템 사용

struct MSG_FL_LOG_ITEM_USE_ENERGY : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		return o_szLogString;
	}
};		// 에너지류 아이템 사용

struct MSG_FL_LOG_ITEM_USE_ENCHANT : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				EnchantLogType;	// ENCHANT_LOGTYPE_FAIL, ENCHANT_LOGTYPE_SUCCESS, ENCHANT_LOGTYPE_TARGETITEM, ENCHANT_LOGTYPE_ATTACHITEM
	INT				nTargetItemPrefixCodeNum;	// 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 - 필드추가
	INT				nTargetItemSuffixCodeNum;	// 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 - 필드추가
	INT				nTargetItemEnchantCnt;		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	INT				nEnchantItemKind;			// 인챈트 아이템의 ItemKind // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	INT				nEnchantItemNum;			// 인챈트 아이템의 ItemNum // 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	bool			bSuccessFlag;				// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	UID64_t			arrAddedItemUID[2];			// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 
	INT				arrAddedItemNum[2];			// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];

		// 2007-07-30 by cmkwon, 갬블/인챈트 로그에 접두/접미 정보 저장 - 게임 로그 수정됨
		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
		//sprintf(o_szLogString, "%s|%s|%s|%d|%d|%d\r\n"
		//	, GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
		//	, EnchantLogType, nTargetItemPrefixCodeNum, nTargetItemSuffixCodeNum);
		///////////////////////////////////////////////////////////////////////////////
		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
		// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - (추가 수정) - 
		sprintf(o_szLogString, "%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|%I64d|%d|%I64d|%d\r\n"
			, GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, GetEnchatLogTypeString(EnchantLogType), nTargetItemPrefixCodeNum, nTargetItemSuffixCodeNum, nTargetItemEnchantCnt, nEnchantItemKind, nEnchantItemNum, bSuccessFlag
			, arrAddedItemUID[0], arrAddedItemNum[0], arrAddedItemUID[1], arrAddedItemNum[1]);
		return o_szLogString;
	}
};		// 인챈트류 아이템 사용

struct MSG_FL_LOG_ITEM_STORE_ITEM : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ChangeCount;	// 변경 개수, 			// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - MSG_FL_LOG_ITEM_STORE_ITEM 에 필드 추가
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 
		//		sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem), ChangeCount);
		return o_szLogString;
	}
};		// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 인벤-->창고 로 카운터블 아이템 이동시 창고에 해당 아이템 추가 및 개수 변경시 저장, // 창고 소유 아이템 저장, 창고 시작 아이템


struct MSG_FL_LOG_ITEM_USE_MIX : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	bool						IsSuccess;										// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
	ITEM_UID_W_ITEMNUM_COUNT	arrSourceItemList[COUNT_ITEM_MIXING_SOURCE];	// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 아래와 같이 수정 함
		//sprintf(o_szLogString, "%s|%s|%s|-1\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem));
		sprintf(o_szLogString, "%s|%s|%s|%s|%I64d|%d|%d|%I64d|%d|%d|%I64d|%d|%d|%I64d|%d|%d|%I64d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp),
			((IsSuccess) ? "SUCCESS" : "FAIL"), Item4Log.GetLogItemString(szItem)  // 2013-07-02 by bckim, 조합로그 보완 
			, arrSourceItemList[0].ItemUID, arrSourceItemList[0].ItemNum, arrSourceItemList[0].Count, arrSourceItemList[1].ItemUID, arrSourceItemList[1].ItemNum, arrSourceItemList[1].Count
			, arrSourceItemList[2].ItemUID, arrSourceItemList[2].ItemNum, arrSourceItemList[2].Count, arrSourceItemList[3].ItemUID, arrSourceItemList[3].ItemNum, arrSourceItemList[3].Count
			, arrSourceItemList[4].ItemUID, arrSourceItemList[4].ItemNum, arrSourceItemList[4].Count);
		return o_szLogString;
	}
};			// 조합 정보, CurrentCount( 0이면 실패 1이면 성공)

struct MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	char			szOriginCharName[SIZE_MAX_CHARACTER_NAME];
	char			szNewCharName[SIZE_MAX_CHARACTER_NAME];
	DWORD			Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, szOriginCharName, szNewCharName);
		return o_szLogString;
	}
};

using MSG_FL_LOG_ITEM_BAZAAR_SEND = MSG_FL_LOG_ITEM_TRADE_SEND;		// 2006-07-28 by cmkwon
using MSG_FL_LOG_ITEM_BAZAAR_RECV = MSG_FL_LOG_ITEM_TRADE_RECV;		// 2006-07-28 by cmkwon

struct MSG_FL_LOG_ITEM_SPI : public FL_ITEM_LOG_BASE			// 2006-10-27 by cmkwon
{
	ITEM_FOR_LOG	Item4Log;
	BYTE			ItemUpdateType;	// 2006-10-27 by cmkwon, IUT_XXXX
	INT				ChangeCount;	// 변경 개수
	INT				RemainedMoney;	// 남은 돈
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, GetItemUpdateTypeString(ItemUpdateType), ChangeCount, RemainedMoney);
		return o_szLogString;
	}
};

// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
//typedef MSG_FL_LOG_ITEM_GET_ITEM		MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT;		// 56 - ItemEvent에 의한 아이템 추가, 2007-01-11 by cmkwon
using MSG_FL_LOG_ITEM_GET_ITEM_BY_ITEMEVENT = MSG_FL_LOG_ITEM_GET_ITEM_2;		// 56 - ItemEvent에 의한 아이템 추가, 2007-01-11 by cmkwon

struct MSG_FL_LOG_WARPOINT
{// 58 - WarPoint 변경, 2007-05-15 by dhjin
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				ChangeWarPoint;
	INT				TotalWarPoint;
	INT				CumulativeWarPoint;
	BYTE			WPUpdateType;				// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - MSG_FL_LOG_WARPOINT에 추가
	INT				ArenaPlayCount;				// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 플레이한 아레나 횟수 로그 추가
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2009-01-22 by cmkwon, 전쟁 관련 게임 로그 수정 - 아래와 같이 추가함.
		//sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%d|%d\r\n"
		//	, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, ChangeWarPoint, TotalWarPoint, CumulativeWarPoint);
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 플레이한 아레나 횟수 로그 추가
		//sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%d|%d|%s\r\n"
		//	, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, ChangeWarPoint, TotalWarPoint, CumulativeWarPoint, GetWPUTString(WPUpdateType));
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%d|%d|%s|ArenaPlayCount(%d)\r\n"
			, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, ChangeWarPoint, TotalWarPoint, CumulativeWarPoint, GetWPUTString(WPUpdateType), ArenaPlayCount);
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 플레이한 아레나 횟수 로그 추가
		return o_szLogString;
	}
};

struct MSG_FL_LOG_ARENA_TEAM_MATCH
{// 59 - Arena 팀 매칭, 2007-06-11 by dhjin
	BYTE			TeamMode;
	BYTE			TeamSize;
	BYTE			StartLevel;
	BYTE			EndLevel;
	MapIndex_t		ArenaMap;	// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 로그 추가
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 로그 추가
		//		sprintf(o_szLogString, "%s|%d|%d|%d~%d\r\n", GetGameLogTypeString(i_nLogType), TeamMode, TeamSize, StartLevel, EndLevel);
		sprintf(o_szLogString, "%s|%d|%d|%d~%d|%d\r\n", GetGameLogTypeString(i_nLogType), TeamMode, TeamSize, StartLevel, EndLevel, ArenaMap);
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵 로그 추가
		return o_szLogString;
	}
};

struct MSG_FL_LOG_TUTORIAL_COMPLETION
{// 60 - Tutorial, 2007-07-06 by dhjin
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	INT				TutorialNum;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d\r\n"
			, GetGameLogTypeString(i_nLogType), AccountUID, AccountName, CharacterUID, CharacterName, TutorialNum);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_GUILD_STORE
{// 2006-09-27 by dhjin, 여단 창고 로그 남기는 구조체
	UID32_t			GuildUID;
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t			AccountUID;
	CHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t			CharacterUID;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID64_t			ItemUID;
	INT				ItemNum;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	INT				ItemResultCount;
	INT				ItemChangeCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d|%s|%d|%s|%I64d|%d|%d|%d|%d|%d\r\n"
			, GetGameLogTypeString(i_nLogType), GuildUID, GuildName, AccountUID, AccountName, CharacterUID, CharacterName
			, ItemUID, ItemNum, PrefixCodeNum, SuffixCodeNum, ItemResultCount, ItemChangeCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_OUTPOST_START
{// 61 - OutPost시작 로그, 2007-10-02 by dhjin
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_OUTPOST_END
{// 62 - OutPost종료 로그, 2007-10-02 by dhjin
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	CHAR				PossessGuildName[SIZE_MAX_GUILD_NAME];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%s\r\n"
			, GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, PossessGuildName);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_EXPEDIENCYFUND
{// 63 - 판공비 증/감 로그, 2007-10-02 by dhjin
	MapIndex_t		MapIndex;
	UID32_t			CharacterUID;	// 2007-10-16 by cmkwon, 로그 추가 - 추가
	UID32_t			GuildUID;		// 2007-10-16 by cmkwon, 로그 추가 - 추가
	bool			Increase;		// FALSE : 감소, TRUE : 증가
	INT				Count;
	INT				ExpediencyFundVRate;	// 2007-10-16 by cmkwon, 로그 추가 - EXPEDIENCYFUND_RATE_VALUE 로 나눈것이 실제 징수율
	INT				TotalCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2007-10-16 by cmkwon, 로그 추가 - 수정함
		sprintf(o_szLogString, "%s|%d|%u|%u|%d|%d|%.3f|%d\r\n"
			, GetGameLogTypeString(i_nLogType), MapIndex, CharacterUID, GuildUID, Increase, Count, ((float)ExpediencyFundVRate) / EXPEDIENCYFUND_RATE_VALUE, TotalCount);
		return o_szLogString;
	}
};

////////////////////////////////
// ETC LOG(0x40~)
struct MSG_FL_LOG_SERVER_INFO_MAP
{
	MapIndex_t		MapIndex;
	ChannelIndex_t	ChannelIndex;
	INT				ClientCount;
	INT				MonsterCount;
	char			MapName[SIZE_MAX_MAP_NAME];						// 2010-06-01 by shcho, GLogDB 관련 -
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), MapIndex, ChannelIndex, ClientCount, MonsterCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_SERVER_INFO_TOTAL
{
	INT				ClientCount;
	INT				MonsterCount;
	bool			bGlogUpdate;		// 2010-06-01 by shcho, GLogDB 관련 -
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|0|0|%d|%d\r\n", GetGameLogTypeString(i_nLogType), ClientCount, MonsterCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_HACKING_LOG : public FL_USER_LOG_BASE
{
	char			AccountName0[SIZE_MAX_ACCOUNT_NAME];
	long			hackingCode;
	char			szErrString[SIZE_STRING_128];			// 2006-10-20 by cmkwon, 추가함(해킹 프로그램명)
	char			IPAddress[SIZE_MAX_IPADDRESS];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%s|%s\r\n", GetGameLogTypeString(i_nLogType), AccountName0, GetLogBaseString(szTemp), hackingCode, IPAddress, szErrString);
		return o_szLogString;
	}
};

////////////////////////////////
// MONSTER LOG(0x50~)
struct MSG_FL_LOG_MONSTER_BOSS
{
	MAP_CHANNEL_INDEX	MapChannel;
	INT					MonsterUnitKind;
	ATUM_DATE_TIME		CreatedTime;
	ATUM_DATE_TIME		DeadTime;
	char				AttackUser[SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD][SIZE_MAX_CHARACTER_NAME];
	INT					LenOfDropItemList;		// Drop Item List string의 길이
	ARRAY_(char);								// Drop Item List string
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString, char *i_szDropItemList)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\r\n", GetGameLogTypeString(i_nLogType), MapChannel.MapIndex, MapChannel.ChannelIndex, MonsterUnitKind
			, CreatedTime.GetDateTimeString().GetBuffer(), DeadTime.GetDateTimeString().GetBuffer()
			, AttackUser[0], AttackUser[1], AttackUser[2], AttackUser[3], AttackUser[4], AttackUser[5]
			, AttackUser[6], AttackUser[7], AttackUser[8], AttackUser[9], i_szDropItemList);
		return o_szLogString;
	}
};	// 보스급 몬스터



struct MSG_FL_LOG_INFLWAR_START
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			MothershipCreatedMapIdx;
	BYTE				InfluenceType2;
	INT					MonsterUnitKind;
	BYTE				MonBell;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), MothershipCreatedMapIdx, InfluenceType2, MonsterUnitKind, MonBell);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_INFLWAR_END
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			MothershipCreatedMapIdx;
	BYTE				InfluenceType2;
	INT					MonsterUnitKind;
	BYTE				MonBell;
	bool				DeadFlag;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), MothershipCreatedMapIdx, InfluenceType2, MonsterUnitKind, MonBell, DeadFlag);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_RESET_START
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	BYTE				InflTy4Reset;
	UID32_t				GuildUID4Reset;
	UID32_t				ResetUserGuildUID;
	UID32_t				ResetUserCharacterUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%d|%u|%u|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, InflTy4Reset, GuildUID4Reset, ResetUserGuildUID, ResetUserCharacterUID);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_RESET_DESTROY
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	BYTE				InflTy4Reset;
	UID32_t				GuildUID4Reset;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%d|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, InflTy4Reset, GuildUID4Reset);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_RESET_COMPLETE
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID);
		return o_szLogString;
	}
};

#define SIZE_MAX_OUTPOSTWAR_DAMAGELIST			10		// 2007-10-16 by cmkwon, 로그 추가 - 
struct MSG_FL_LOG_OUTPOSTWAR_PROTECTOR_DESTROY
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	BYTE				InflTy4Reset;
	UID32_t				GuildUID4Reset;
	UID32_t				ArrGuildUID[SIZE_MAX_OUTPOSTWAR_DAMAGELIST];
	float				ArrDamage[SIZE_MAX_OUTPOSTWAR_DAMAGELIST];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2007-10-16 by cmkwon, 데미지 리스트를 외부에서 만들어 이어서 붙인다
		sprintf(o_szLogString, "%s|%d|%d|%u|%d|%u", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, InflTy4Reset, GuildUID4Reset);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_OUTPOSTWAR_SET_NEXTTIME
{// 2007-10-16 by cmkwon, 로그 추가 -
	MapIndex_t			OutPostMapIdx;
	BYTE				PossessInflTy;
	UID32_t				PossessGuildUID;
	UID32_t				UserGuildUID;
	UID32_t				UserCharacterUID;
	ATUM_DATE_TIME		NextTime;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%u|%u|%u|%s\r\n", GetGameLogTypeString(i_nLogType), OutPostMapIdx, PossessInflTy, PossessGuildUID, UserGuildUID, UserCharacterUID, NextTime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};

struct MSG_FL_LOG_POLL_VOTE
{// 2007-11-08 by dhjin
	UID32_t				VoteCharacterUID;
	UID32_t				CharacterUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType), VoteCharacterUID, CharacterUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_POLL_DELETE_LEADERCANDIDATE
{// 2007-11-08 by dhjin
	UID32_t				CharacterUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d\r\n", GetGameLogTypeString(i_nLogType), CharacterUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_DISMEMBER_GUILD
{// 2007-11-09 by dhjin
	UID32_t				GuildUID;
	char				GuildName[SIZE_MAX_GUILD_NAME];
	UID32_t				GuildCommanderUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GuildUID, GuildName, GuildCommanderUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_NOTIFY_MSG_DELETE : public SNOTIFY_MSG		// 2007-11-28 by cmkwon, 통지시스템 구현 -
{
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%I64d|%u|%d|%s|%u|%s|%s\r\n", GetGameLogTypeString(i_nLogType), NotifyMsgUID, CharacterUID, NotifyMsgType, NotifyMsgString, SenderCharacterUID, SenderCharacterName, CreateTime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};
struct MSG_FL_LOG_USE_COUPON : public FL_USER_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: 쿠폰 사용 게임 로그에 추가 - 
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];
	char CouponNumber[SIZE_MAX_COUPON_NUMBER];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), AccountName, CouponNumber);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_ITEM_ATTACH_ITEM : public FL_ITEM_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
{
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeSec;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeSec);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_ITEM_DETACH_ITEM : public FL_ITEM_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
{
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeSec;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeSec);
		return o_szLogString;
	}
};
struct MSG_FL_LOG_ITEM_DELETE : public FL_ITEM_LOG_BASE		// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
{
	ITEM_FOR_LOG	Item4Log;
	INT				ItemDeleteType;
	ATUM_DATE_TIME	atCreatedTime;			// 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - MSG_FL_LOG_ITEM_DELETE
	INT				nUsedTime;				// 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - MSG_FL_LOG_ITEM_DELETE
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		// 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - 
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%s|%s|%d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, GetItemDeleteTypeString(ItemDeleteType), atCreatedTime.GetDateTimeString().GetBuffer(), nUsedTime);
		return o_szLogString;
	}
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
struct MSG_FL_LOG_MS_WAR
{
	BYTE			AttInfluence;
	INT				MonsterUID;
	INT				ContributionPoint;
	ATUM_DATE_TIME	MSWarStartTime;
	BYTE			WinInfluence;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), AttInfluence, MonsterUID, ContributionPoint, MSWarStartTime.GetDateTimeString().GetBuffer(), WinInfluence);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_SP_WAR
{
	BYTE			AttInfluence;
	INT				SPSummonMapIndex;
	ATUM_DATE_TIME	SPWarStartTime;
	BYTE			WinInfluence;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType), AttInfluence, SPSummonMapIndex, SPWarStartTime.GetDateTimeString().GetBuffer(), WinInfluence);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_WAR_CONTRIBUTION
{
	UID32_t		CharacterUID;
	MapIndex_t	nMapIndex;
	UID64_t		nContribution;
	INT			nPay;
	BYTE		byPayType;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%I64d|%d|%d\r\n", GetGameLogTypeString(i_nLogType), CharacterUID, nMapIndex, nContribution, nPay, byPayType);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_WAR_CONTRIBUTION_GEAR
{
	UID64_t			nTotalBGearContribution;
	UID64_t			nTotalIGearContribution;
	UID64_t			nTotalAGearContribution;
	UID64_t			nTotalMGearContribution;
	INT				nBGearCount;
	INT				nIGearCount;
	INT				nAGearCount;
	INT				nMGearCount;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%I64d|%I64d|%I64d|%I64d|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, nTotalBGearContribution, nTotalIGearContribution, nTotalAGearContribution, nTotalMGearContribution
			, nBGearCount, nIGearCount, nAGearCount, nMGearCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_LUCKY_DROP
{// 2009-03-31 by dhjin, 럭키머신 로그
	INT				MysteryItemDropNum;
	INT				DropItemNum;
	ATUM_DATE_TIME	Starttime;				// 럭키머신 시작 시간, 2008-11-04 by dhjin, 럭키머신
	ATUM_DATE_TIME	Endtime;				// 럭키머신 종료 시간, 2008-11-04 by dhjin, 럭키머신
	INT				CountPerPeriod;			// 기간동안 드랍될 최대 아이템 수, 2008-11-04 by dhjin, 럭키머신
	INT				DropCount;				// 기간동안 현재 드랍된 아이템 수, 2008-11-04 by dhjin, 럭키머신

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%s|%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, MysteryItemDropNum, DropItemNum, Starttime.GetDateTimeString().GetBuffer(), Endtime.GetDateTimeString().GetBuffer()
			, CountPerPeriod, DropCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_START {
	// 193 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작 관련 정보 
	InfinityCreateUID_t		InfinityCreateUID;		// 해당 방의 고유 번호
	//USHORT					MainServerID;			// 입장한 캐릭터의 서버
	BYTE					InfluenceType;			// 입장한 캐릭터의 세력
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];		//	캐릭터명
	UID32_t					CharacterUID;			// (캐릭터 UID)
	Lv_t					Level;					// 입장한 캐릭터의 레벨
	USHORT					UnitKind;				// 입장한 캐릭터의 기어종류
	Experience_t			Experience;				// 경험치량
	float					CurrentHP;				// HP
	float					CurrentDP;				// DP
	SHORT					CurrentSP;				// SP

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%I64d|%d|%s(%d)|%d|%d|%f|%f|%f|%d\r\n", GetGameLogTypeString(i_nLogType)
			, InfinityCreateUID, InfluenceType, CharacterName, CharacterUID, Level, UnitKind, Experience, CurrentHP, CurrentDP, CurrentSP);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_START_ITEM : public FL_ITEM_LOG_BASE {
	// 194 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 시작시 아이템 복사 정보
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeMSec;
	UID64_t			MainSvrItemUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeMSec, MainSvrItemUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_CINEMA {
	// 195 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 연출 로그
	InfinityCreateUID_t		InfinityCreateUID;		// 해당 방의 고유 번호
	MSec_t					StartTime;
	EffectIdx_t				EffectIdx;
	ObjectIdx_t				ObjectIdx;
	ObjectNum_t				ChangeObjectIdx;
	MonIdx_t				MonsterIdx;
	MSec_t					CameraTremble;
	CinemaTalk_t			CinemaTalk[SIZE_MAX_CINEMATALK_DESCRIPTION];
	QuestIndex_t			QuestIndex;		// 2011-03-09 by hskim, 인피니티 3차 - 시네마 퀘스트 인덱스 설정 추가 -

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%I64d|%d|%d|%d|%d|%d|%d|%s|%d\r\n", GetGameLogTypeString(i_nLogType)
			, InfinityCreateUID, StartTime, EffectIdx, ObjectIdx, ChangeObjectIdx, MonsterIdx, CameraTremble, CinemaTalk, QuestIndex);		// 2011-03-09 by hskim, 인피니티 3차 - 시네마 퀘스트 인덱스 설정 추가
		return o_szLogString;
	}
};

struct MSG_FL_LOG_MONSTER_SKILL {
	// 196 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 스킬 사용시
	MonIdx_t				MonsterIdx;
	ItemNum_t				MonsterSkillNum;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, MonsterIdx, MonsterSkillNum);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_HPACTION_TALK {
	// 197 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 몬스터 대화 사용시
	MonIdx_t				MonsterIdx;
	HPTalk_t				HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%d|%s\r\n", GetGameLogTypeString(i_nLogType)
			, MonsterIdx, HPTalk);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_FIN {
	// 198 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료 관련 정보 
	InfinityCreateUID_t		InfinityCreateUID;		// 해당 방의 고유 번호
	//USHORT					MainServerID;			// 입장한 캐릭터의 서버
	BYTE					InfluenceType;			// 입장한 캐릭터의 세력
	char					CharacterName[SIZE_MAX_CHARACTER_NAME];		//	캐릭터명
	UID32_t					CharacterUID;			// (캐릭터 UID)
	Lv_t					Level;					// 입장한 캐릭터의 레벨
	USHORT					UnitKind;				// 입장한 캐릭터의 기어종류
	Experience_t			Experience;				// 경험치량
	float					CurrentHP;				// HP
	float					CurrentDP;				// DP
	SHORT					CurrentSP;				// SP
	bool					Clear;					// 인피니티 필드 클리어 유무
	KillCnt_t				KillCount;				// 몬스터 사망을 하게한 수(막타 횟수)
	DeathCnt_t				DeathCount;				// 클리어 하기까지 사망 횟수

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%I64d|%d|%s(%d)|%d|%d|%f|%f|%f|%d|%d|%d|%d\r\n", GetGameLogTypeString(i_nLogType)
			, InfinityCreateUID, InfluenceType, CharacterName, CharacterUID, Level, UnitKind, Experience, CurrentHP, CurrentDP, CurrentSP, Clear, KillCount, DeathCount);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_FIN_ITEM : public FL_ITEM_LOG_BASE {
	// 199 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 아이템 복사 정보 
	ITEM_FOR_LOG	Item4Log;
	INT				ReaminTimeMSec;
	UID64_t			MainSvrItemUID;
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ReaminTimeMSec, MainSvrItemUID);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_INFINITY_FIN_ALIVE_KEY_MONSTER {
	// 200 - // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가, 종료시 살아있는 몬스터 정보
	MonIdx_t				MonsterIdx;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString) {
		sprintf(o_szLogString, "%s|%d\r\n", GetGameLogTypeString(i_nLogType)
			, MonsterIdx);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_PET_LEVEL : public FL_USER_LOG_BASE {
	// 213 - // 2011-09-15 by hskim, 파트너 시스템 2차 - 파트너 레벨 변경
	BYTE		FromPetLevel;
	BYTE		ToPetLevel;
	LONGLONG	TotalPlayTime;
	// 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - 파트너 로그 보안
	UID64_t		UniqueNumber;
	Experience_t	PetTotalExp;
	Experience_t	PetAcquisitionExp;
	// 2012-10-04 by hskim, 한국 자체 서비스 (가비아 IDC) - 파트너 로그 보안
	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024];
		sprintf(o_szLogString, "%s|%s|%I64d|%f|%f|%d|%d|%I64d\r\n", GetGameLogTypeString(i_nLogType), GetLogBaseString(szTemp), UniqueNumber, PetTotalExp, PetAcquisitionExp, FromPetLevel, ToPetLevel, TotalPlayTime);
		return o_szLogString;
	}
};					// 파트너 레벨 변화

					// start 2011-12-12 by hskim, GLog 2차
struct MSG_FL_LOG_CONNECT_USER_NEW
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%s\r\n", GetGameLogTypeString(i_nLogType), AccountName);
		return o_szLogString;
	}
};

struct MSG_FL_LOG_CONNECT_USER_LOGIN
{
	char AccountName[SIZE_MAX_ACCOUNT_NAME];

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%s\r\n", GetGameLogTypeString(i_nLogType), AccountName);
		return o_szLogString;
	}
};
// end 2011-12-12 by hskim, GLog 2차

// start 2012-10-08 by khkim, GLog
struct MSG_FL_LOG_ACCOUNTCONNECT
{
	SHORT		LogType;
	UID64_t		UserSerial;
	UID64_t		IP;
	//int			GameServerID;

	MSG_FL_LOG_ACCOUNTCONNECT()
	{
		LogType = NULL;
		UserSerial = NULL;
		IP = NULL;
		//GameServerID = NULL;
	}
};

struct MSG_FL_LOG_CHARDATA
{
	SHORT		LogType;
	UID64_t		UserSerial;
	UID64_t		CharSerial;
	SHORT		Class;
	SHORT		Lv;
	UID64_t		Exp;
	UID64_t		GameMoney;
	UID64_t		Playtime;
	//int			GameServerID;

	MSG_FL_LOG_CHARDATA()
	{
		LogType = NULL;
		UserSerial = NULL;
		CharSerial = NULL;
		Class = NULL;
		Lv = NULL;
		Exp = NULL;
		GameMoney = NULL;
		Playtime = NULL;
		//GameServerID = NULL;
	}
};

struct MSG_FL_LOG_ITEM
{
	SHORT		LogType;
	UID64_t		ItemSerial;
	UID64_t		Count1;
	UID64_t		Count2;
	UID64_t		CharSerial;
	int			CharLv;

	MSG_FL_LOG_ITEM()
	{
		LogType = NULL;
		ItemSerial = NULL;
		Count1 = NULL;
		Count2 = NULL;
		CharSerial = NULL;
		CharLv = NULL;
//		GameServerID = NULL;
	}
};

struct MSG_FL_LOG_ITEMSTATE
{
	SHORT		LogType;
	UID64_t		ItemSerial;
	UID64_t		Count;
	UID64_t		CharSerial;
	int			CharLv;
	//int			GameServerID;

	MSG_FL_LOG_ITEMSTATE()
	{
		LogType = NULL;
		ItemSerial = NULL;
		Count = NULL;
		CharSerial = NULL;
		CharLv = NULL;
		//GameServerID = NULL;
	}
};

struct MSG_FL_LOG_SERVER
{
	SHORT		LogType;

	MSG_FL_LOG_SERVER() : LogType { 0 } { }
};
// end 2012-10-08 by khkim, GLog 2차

// start 2012-01-16 by hskim, 통계 - 화패
struct MSG_FL_LOG_STATISTICS_MONEY
{
	BYTE					FunctionType;
	StatisticsMoneyUnit_t	SPI;
	StatisticsMoneyUnit_t	WarPoint;
	StatisticsMoneyUnit_t	SystemOrb;
	StatisticsMoneyUnit_t	QuasarOrb;
	StatisticsMoneyUnit_t	ClusterOrb;
	StatisticsMoneyUnit_t	GalaxyOrb;
	StatisticsMoneyUnit_t	UniverseOrb;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|\r\n", GetGameLogTypeString(i_nLogType));
		return o_szLogString;
	}
};
// end 2012-01-16 by hskim, 통계 - 화패

// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형) 
struct MSG_FL_LOG_FIXED_TERM_SHAPE_START : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ShapeItemNum;		// 외형 번호
	INT				ApplyItemNum;		// 외형 변경 아이템 번호
	ATUM_DATE_TIME	Starttime;			// 외형 변경 시작 시간
	ATUM_DATE_TIME	Endtime;			// 외형 변경 종료 시간

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ShapeItemNum, ApplyItemNum, Starttime.GetDateTimeString().GetBuffer(), Endtime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};

struct MSG_FL_LOG_FIXED_TERM_SHAPE_END : public FL_ITEM_LOG_BASE
{
	ITEM_FOR_LOG	Item4Log;
	INT				ShapeItemNum;		// 외형 변경 아이템 번호
	INT				ApplyItemNum;		// 외형 변경 아이템 번호
	ATUM_DATE_TIME	Starttime;			// 외형 변경 시작 시간
	ATUM_DATE_TIME	Endtime;			// 외형 변경 종료 시간

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		char szTemp[1024], szItem[1024];
		sprintf(o_szLogString, "%s|%s|%s|%d|%d|%s|%s\r\n", GetGameLogTypeString(i_nLogType), GetLogItemBaseString(szTemp), Item4Log.GetLogItemString(szItem)
			, ShapeItemNum, ApplyItemNum, Starttime.GetDateTimeString().GetBuffer(), Endtime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};
// end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

///////////////////////////////////////////////////////////////////////////////
// Monitor Server Protocol (T0: 0x30~)
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////
// T0_FM_MONITOR - Field Server Monitor
//#define T1_FM_GET_NUM_CLIENTS			0x00	// No Body
//#define T1_FM_GET_NUM_CLIENTS_OK		0x01
//#define T1_FM_SHUTDOWN					0x02	// No Body
//#define T1_FM_SHUTDOWN_OK				0x03	// No Body
//#define T1_FM_PING						0x04	// No Body
//#define T1_FM_PING_OK					0x05	// No Body
//#define T1_FM_PRINTMAP					0x06	// No Body
//#define T1_FM_SAVE_MAPBLOCK_INFO		0x07	// No Body
//#define T1_FM_PRINT_DEBUG_MSG			0x08	// No Body
//#define T1_FM_SEND_CHANNEL_INFO			0x09	// F->M
//#define T1_FM_CHANGE_BANDWIDTH_FLAG		0x0A	// No Body
//#define T1_FM_CHANGE_BANDWIDTH_FLAG_OK	0x0B	// 변경된 플래그값
//#define T1_FM_SET_MSG_PRINT_LEVEL		0x0C
//#define T1_FM_GET_CHARACTER_INFO		0x0D
//#define T1_FM_GET_CHARACTER_INFO_OK		0x0E
//#define T1_FM_GET_MAP_USER_COUNTS		0x0F	// No Body
//#define T1_FM_GET_MAP_USER_COUNTS_ACK	0x10
//#define T1_FM_RELOAD_ITEM_INFO			0x11	// No Body
//#define T1_FM_RELOAD_ITEM_INFO_OK		0x12	// No Body
//#define T1_FM_RESET_NPC_SERVER			0x13	// No Body
//#define T1_FM_RESET_NPC_SERVER_OK		0x14	// No Body
//#define T1_FM_SERVER_THREAD_STATUS		0x15
//#define T1_FM_SET_SERVICE_STATE			0x16
//#define T1_FM_SET_SERVICE_STATE_OK		0x17	// No Body
//#define T1_FM_RELOAD_SERVER_INFO		0x18
//#define T1_FM_RELOAD_SERVER_INFO_OK		0x19	// No Body
//#define T1_FM_INCREASE_CHANNEL			0x1A	// M->F, Channel 수 증가
//#define T1_FM_INCREASE_CHANNEL_OK		0x1B	// F->M, Channel 수 증가 결과
//#define T1_FM_SET_CHANNEL_STATE			0x1C	// M->F, Channel Enable/Disable
//#define T1_FM_SET_CHANNEL_STATE_OK		0x1D	// F->M, Channel Enable/Disable 결과
//#define T1_FM_CITYWAR_CHANGE_WAR_TIME		0x20		// M->F
//#define T1_FM_CITYWAR_CHANGED_OCCUPY_INFO	0x21	// F->M

//#define T_FM_GET_NUM_CLIENTS			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_NUM_CLIENTS)
//#define T_FM_GET_NUM_CLIENTS_OK			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_NUM_CLIENTS_OK)
//#define T_FM_SHUTDOWN					(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SHUTDOWN)
//#define T_FM_SHUTDOWN_OK				(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SHUTDOWN_OK)
//#define T_FM_PING						(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PING)
//#define T_FM_PING_OK					(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PING_OK)
//#define T_FM_PRINTMAP					(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PRINTMAP)
//#define T_FM_SAVE_MAPBLOCK_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SAVE_MAPBLOCK_INFO)
//#define T_FM_PRINT_DEBUG_MSG			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_PRINT_DEBUG_MSG)
//#define T_FM_SEND_CHANNEL_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SEND_CHANNEL_INFO)		// F->M
//#define T_FM_CHANGE_BANDWIDTH_FLAG		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CHANGE_BANDWIDTH_FLAG)
//#define T_FM_CHANGE_BANDWIDTH_FLAG_OK	(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_FM_SET_MSG_PRINT_LEVEL		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_MSG_PRINT_LEVEL)
//#define T_FM_GET_CHARACTER_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_CHARACTER_INFO)
//#define T_FM_GET_CHARACTER_INFO_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_CHARACTER_INFO_OK)
//#define T_FM_GET_MAP_USER_COUNTS		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_MAP_USER_COUNTS)
//#define T_FM_GET_MAP_USER_COUNTS_ACK	(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_GET_MAP_USER_COUNTS_ACK)
//#define T_FM_RELOAD_ITEM_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_ITEM_INFO)
//#define T_FM_RELOAD_ITEM_INFO_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_ITEM_INFO_OK)
//#define T_FM_RESET_NPC_SERVER			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RESET_NPC_SERVER)
//#define T_FM_RESET_NPC_SERVER_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RESET_NPC_SERVER_OK)
//#define T_FM_SERVER_THREAD_STATUS		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SERVER_THREAD_STATUS)
//#define T_FM_SET_SERVICE_STATE			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_SERVICE_STATE)
//#define T_FM_SET_SERVICE_STATE_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_SERVICE_STATE_OK)	// No Body
//#define T_FM_RELOAD_SERVER_INFO			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_SERVER_INFO)
//#define T_FM_RELOAD_SERVER_INFO_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_RELOAD_SERVER_INFO_OK)	// No Body
//#define T_FM_INCREASE_CHANNEL			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_INCREASE_CHANNEL)		// M->F, Channel 수 증가
//#define T_FM_INCREASE_CHANNEL_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_INCREASE_CHANNEL_OK)	// F->M, Channel 수 증가 결과
//#define T_FM_SET_CHANNEL_STATE			(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_CHANNEL_STATE)		// M->F, Channel Enable/Disable
//#define T_FM_SET_CHANNEL_STATE_OK		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_SET_CHANNEL_STATE_OK)	// F->M, Channel Enable/Disable 결과
//#define T_FM_CITYWAR_CHANGE_WAR_TIME		(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CITYWAR_CHANGE_WAR_TIME)		// M->F
//#define T_FM_CITYWAR_CHANGED_OCCUPY_INFO	(MessageType_t)((T0_FM_MONITOR<<8)|T1_FM_CITYWAR_CHANGED_OCCUPY_INFO)	// F->M


struct MSG_FM_GET_NUM_CLIENTS_OK
{
	USHORT	NumOfClients;
};
using MSG_PM_GET_NUM_CLIENTS_OK = MSG_FM_GET_NUM_CLIENTS_OK;
using MSG_IM_GET_NUM_CLIENTS_OK = MSG_FM_GET_NUM_CLIENTS_OK;

struct MSG_PM_RELOAD_VERSION_INFO
{
	USHORT	LauncherVersion[4];
	USHORT	DeleteFileListVersion[4];
	USHORT	NoticeVersion[4];
	INT		NumOfClientVersions;
	ARRAY_(USHORT[8]);
};

struct MSG_FM_PRINTMAP
{
	MAP_CHANNEL_INDEX		MapChannelIndex;
};

using MSG_NM_PRINTMAP = MSG_FM_PRINTMAP;

struct MSG_FM_SAVE_MAPBLOCK_INFO
{
	MAP_CHANNEL_INDEX		MapChannelIndex;
};

using PMSG_FM_SAVE_MAPBLOCK_INFO = MSG_FM_SAVE_MAPBLOCK_INFO*;

using MSG_NM_SAVE_MAPBLOCK_INFO = MSG_FM_SAVE_MAPBLOCK_INFO;

struct MSG_FM_PRINT_DEBUG_MSG
{
	int		IntParam1;
	int		IntParam2;
	float	FloatParam1;
	float	FloatParam2;
	char	StringParam[100];
};
using MSG_IM_PRINT_DEBUG_MSG = MSG_FM_PRINT_DEBUG_MSG;

#ifndef _ATUM_CLIENT

struct MSG_FM_SEND_CHANNEL_INFO
{
	PROJECTINFO			projectInfo;
	bool				bCityWarMap;
	SCITY_OCCUPY_INFO	cityWarOccupyInfo;
	ChannelIndex_t		ChannelIndex;	// 채널의 인덱스
	bool				IsEnabled;		// 채널의 활성화 여부에 대한 정보
};			// F->M

struct MSG_NM_SEND_CHANNEL_INFO
{
	PROJECTINFO		projectInfo;
	ChannelIndex_t	ChannelIndex;	// 채널의 인덱스
	bool			IsEnabled;		// 채널의 활성화 여부에 대한 정보
	UINT			LimitMonsterCount;
};

#endif // _ATUM_CLIENT

struct MSG_FM_CHANGE_BANDWIDTH_FLAG_OK
{
	bool	bChagedFlag;
};

using PMSG_FM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK*;

using MSG_PM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK;
using MSG_NM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK;
using MSG_IM_CHANGE_BANDWIDTH_FLAG_OK = MSG_FM_CHANGE_BANDWIDTH_FLAG_OK;

struct MSG_FM_SET_MSG_PRINT_LEVEL
{
	BYTE	Level;
};
using MSG_PM_SET_MSG_PRINT_LEVEL = MSG_FM_SET_MSG_PRINT_LEVEL;
using MSG_NM_SET_MSG_PRINT_LEVEL = MSG_FM_SET_MSG_PRINT_LEVEL;
using MSG_IM_SET_MSG_PRINT_LEVEL = MSG_FM_SET_MSG_PRINT_LEVEL;

struct MSG_FM_GET_CHARACTER_INFO
{
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t			CharacterUniqueNumber;
	ClientIndex_t	ClientIndex;
};

struct MSG_FM_GET_CHARACTER_INFO_OK
{
	CHARACTER		CharacterInfo;
};

struct MSG_FM_GET_MAP_USER_COUNTS
{
	MAP_CHANNEL_INDEX	MapChannIndex;
};

struct MSG_FM_GET_MAP_USER_COUNTS_ACK
{
	MAP_CHANNEL_INDEX	MapChannIndex;
	UINT		AccumulatedUserCounts;
	UINT		CurrentUserCounts;
	UINT		MaxUserCounts;
	UINT		AccumulatedMonsterCounts;
	UINT		CurrentMonsterCounts;
};

struct MSG_FM_SERVER_THREAD_STATUS
{
	DWORD	dwThreadId;
	char	szThreadComment[SIZE_MAX_THREAD_COMMENT];
	DWORD	dwTimeGap;
};

// SERVER_SERVICE_TYPE_XXX
#define SERVER_SERVICE_TYPE_QUEST		((BYTE)0)	// 퀘스트 서비스

struct MSG_FM_SET_SERVICE_STATE
{
	BYTE	ServiceType;	// SERVER_SERVICE_TYPE_XXX
	bool	ServiceState;	// TRUE: Start Service, FALSE: Stop Service
};

// SERVER_INFO_TYPE_XXX
#define SERVER_INFO_TYPE_QUEST			((BYTE)0)	// 퀘스트

struct MSG_FM_RELOAD_SERVER_INFO
{
	BYTE	InfoType;		// SERVER_INFO_TYPE_XXX
};

struct MSG_FM_INCREASE_CHANNEL
{
	MapIndex_t	MapIndex;
};	// M->F, Channel 수 증가

struct MSG_FM_INCREASE_CHANNEL_OK
{
	MapIndex_t	MapIndex;
	INT			TotalChannelCount;
};	// F->M, Channel 수 증가 결과

struct MSG_FM_SET_CHANNEL_STATE
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	bool				EnableChannel;		// TRUE: Enable, FALSE: Disable
};		// M->F, Channel Enable/Disable

struct MSG_FM_SET_CHANNEL_STATE_OK
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	bool				IsEnabled;			// TRUE: Enable, FALSE: Disable
};	// F->M, Channel Enable/Disable 결과
struct MSG_FM_CITYWAR_CHANGE_WAR_TIME
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	ATUM_DATE_TIME		CityWarATimeDefault;
	ATUM_DATE_TIME		CityWarATimeSetting;
};
struct MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	SCITY_OCCUPY_INFO	cityWarOccupyInfo;
};
///////////////////////////////////////////////////////////////////////////////
// PRE Server Monitor
//#define T1_PM_GET_NUM_CLIENTS			0x00	// No Body
//#define T1_PM_GET_NUM_CLIENTS_OK		0x01	// # of client
//#define T1_PM_SHUTDOWN					0x02	// No Body
//#define T1_PM_SHUTDOWN_OK				0x03	// No Body
//#define T1_PM_CLOSE						0x04	// No Body
//#define T1_PM_CLOSE_OK					0x05	// No Body
//#define T1_PM_PING						0x06	// No Body
//#define T1_PM_PING_OK					0x07	// No Body
//#define T1_PM_RELOAD_VERSION_INFO		0x08	// M->P
//#define T1_PM_RELOAD_VERSION_INFO_OK	0x09	// No Body
//#define T1_PM_CHANGE_BANDWIDTH_FLAG		0x0A	// No Body
//#define T1_PM_CHANGE_BANDWIDTH_FLAG_OK	0x0B	// 변경된 플래그값
//#define T1_PM_SET_MSG_PRINT_LEVEL		0x0C
//#define T1_PM_PAUSE_SERVICE				0x0D
//#define T1_PM_PAUSE_SERVICE_OK			0x0E
//#define T1_PM_START_SERVICE				0x0F
//#define T1_PM_START_SERVICE_OK			0x10
//#define T1_PM_GET_SERVER_GROUP_INFO		0x11
//#define T1_PM_GET_SERVER_GROUP_INFO_ACK	0x12
//#define T1_PM_SET_LIMIT_GROUP_USER_COUNTS	0x13
//#define T1_PM_SERVER_THREAD_STATUS			0x14
//#define T1_PM_RELOAD_BLOCKED_ACCOUNTS		0x15	// No Body
//#define T1_PM_RELOAD_BLOCKED_ACCOUNTS_OK	0x16	// No Body
//#define T1_PM_SET_MGAME_EVENT_TYPE			0x17
//#define T1_PM_CHANGE_ENABLE_SERVER_GROUP	0x18
//#define T1_PM_PREPARE_SHUTDOWN				0x19	// 2006-08-04 by cmkwon, 

//#define T1_PM_AUTO_UPDATE_FTP_SERVER_SETTING		0x20	// 2007-01-06 by cmkwon
//#define T1_PM_AUTO_UPDATE_HTTP_SERVER_SETTING		0x21	// 2007-01-06 by cmkwon
//#define T1_PM_RELOAD_VERSION_INFO_HEADER			0x22	// M->P
//#define T1_PM_RELOAD_VERSION_INFO_LIST				0x23	// M->P
//#define T1_PM_RELOAD_VERSION_INFO_DONE				0x24	// M->P

//#define T_PM_GET_NUM_CLIENTS			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_NUM_CLIENTS)			// No Body
//#define T_PM_GET_NUM_CLIENTS_OK			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_NUM_CLIENTS_OK)
//#define T_PM_SHUTDOWN					(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SHUTDOWN)					// No Body
//#define T_PM_SHUTDOWN_OK				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SHUTDOWN_OK)				// No Body
//#define T_PM_CLOSE						(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CLOSE)						// No Body
//#define T_PM_CLOSE_OK					(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CLOSE_OK)					// No Body
//#define T_PM_PING						(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PING)						// No Body
//#define T_PM_PING_OK					(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PING_OK)					// No Body
//#define T_PM_RELOAD_VERSION_INFO		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO)		// M->P
//#define T_PM_RELOAD_VERSION_INFO_OK		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_OK)	// No Body
//#define T_PM_CHANGE_BANDWIDTH_FLAG		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CHANGE_BANDWIDTH_FLAG)		// No Body
//#define T_PM_CHANGE_BANDWIDTH_FLAG_OK	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_PM_SET_MSG_PRINT_LEVEL		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SET_MSG_PRINT_LEVEL)
//#define T_PM_PAUSE_SERVICE				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PAUSE_SERVICE)
//#define T_PM_PAUSE_SERVICE_OK			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PAUSE_SERVICE_OK)
//#define T_PM_START_SERVICE				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_START_SERVICE)
//#define T_PM_START_SERVICE_OK			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_START_SERVICE_OK)
//#define T_PM_GET_SERVER_GROUP_INFO		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_SERVER_GROUP_INFO)
//#define T_PM_GET_SERVER_GROUP_INFO_ACK	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_GET_SERVER_GROUP_INFO_ACK)
//#define T_PM_SET_LIMIT_GROUP_USER_COUNTS	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SET_LIMIT_GROUP_USER_COUNTS)
//#define T_PM_SERVER_THREAD_STATUS		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SERVER_THREAD_STATUS)
//#define T_PM_RELOAD_BLOCKED_ACCOUNTS	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_BLOCKED_ACCOUNTS)		// No Body
//#define T_PM_RELOAD_BLOCKED_ACCOUNTS_OK	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_BLOCKED_ACCOUNTS_OK)	// No Body
//#define T_PM_SET_MGAME_EVENT_TYPE		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_SET_MGAME_EVENT_TYPE)
//#define T_PM_CHANGE_ENABLE_SERVER_GROUP	(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_CHANGE_ENABLE_SERVER_GROUP)
//#define T_PM_PREPARE_SHUTDOWN			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_PREPARE_SHUTDOWN)		// 2006-08-04 by cmkwon

//#define T_PM_AUTO_UPDATE_FTP_SERVER_SETTING			(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_AUTO_UPDATE_FTP_SERVER_SETTING)	// 2007-01-06 by cmkwon, P->M
//#define T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING		(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_AUTO_UPDATE_HTTP_SERVER_SETTING)	// 2007-01-06 by cmkwon, P->M
//#define T_PM_RELOAD_VERSION_INFO_HEADER				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_HEADER)		// 2007-01-09 by cmkwon, M->P
//#define T_PM_RELOAD_VERSION_INFO_LIST				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_LIST)			// 2007-01-09 by cmkwon, M->P
//#define T_PM_RELOAD_VERSION_INFO_DONE				(MessageType_t)((T0_PM_MONITOR<<8)|T1_PM_RELOAD_VERSION_INFO_DONE)			// 2007-01-09 by cmkwon, M->P

struct MSG_PM_GET_SERVER_GROUP_INFO
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
};

struct MSG_PM_GET_SERVER_GROUP_INFO_ACK
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	bool		bEnableServerGroup;
	SERVER_ID	IMServerID;
	UINT		AccumulatedGroupUserCounts;
	UINT		CurrentGroupUserCounts;
	UINT		MaxGroupUserCounts;
	UINT		LimitGroupUserCounts;
};

struct MSG_PM_SET_LIMIT_GROUP_USER_COUNTS
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	UINT		LimitGroupUserCounts;
};

using MSG_PM_SERVER_THREAD_STATUS = MSG_FM_SERVER_THREAD_STATUS;

//struct MSG_PM_SET_MGAME_EVENT_TYPE
//{
//	MGAME_EVENT_t		enMGameEvent;
//};

struct MSG_PM_CHANGE_ENABLE_SERVER_GROUP
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
	bool	bEnableServerGroup;
};

struct MSG_PM_PREPARE_SHUTDOWN				// 2006-08-04 by cmkwon
{
	char	ServerGroupName[SIZE_MAX_SERVER_NAME];
};

struct MSG_PM_AUTO_UPDATE_HTTP_SERVER_SETTING		// 2007-01-06 by cmkwon, Http Auto Update server 구현 추가
{						// 2007-01-06 by cmkwon, AU_SERVER_TYPE_XXX
	char		szDownloadHttpServerIP[SIZE_MAX_FTP_URL];
	USHORT		nDownloadHttpServerPort;
	char		szDownloadHttpServerAccountName[SIZE_MAX_ACCOUNT_NAME];
	char		szDownloadHttpServerPassword[SIZE_MAX_PASSWORD];
	char		szClientHttpUpdateDownloadDir[SIZE_MAX_FTP_FILE_PATH];
	char		szLauncherFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char		szDeleteFileListDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char		szNoticeFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char		szVersionListFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
};

struct MSG_PM_RELOAD_VERSION_INFO_HEADER			// 2007-01-09 by cmkwon
{
	USHORT	LauncherVersion[4];
	USHORT	DeleteFileListVersion[4];
	USHORT	NoticeVersion[4];
};

struct MSG_PM_RELOAD_VERSION_INFO_LIST				// 2007-01-09 by cmkwon
{
	INT		NumOfClientVersions;
	ARRAY_(USHORT[8]);
};


//////////////////////////////////
// NPC Server Monitor
//#define T1_NM_GET_NUM_MONSTERS				0x00	// No Body
//#define T1_NM_GET_NUM_MONSTERS_OK			0x01
//#define T1_NM_SHUTDOWN						0x02	// No Body
//#define T1_NM_SHUTDOWN_OK					0x03	// No Body
//#define T1_NM_PING							0x04	// No Body
//#define T1_NM_PING_OK						0x05	// No Body
//#define T1_NM_PRINTMAP						0x06	// No Body
//#define T1_NM_SAVE_MAPBLOCK_INFO			0x07	// No Body
//#define T1_NM_SEND_CHANNEL_INFO				0x08	//
//#define T1_NM_CHANGE_BANDWIDTH_FLAG			0x0A	// No Body
//#define T1_NM_CHANGE_BANDWIDTH_FLAG_OK		0x0B	// 변경된 플래그값
//#define T1_NM_SET_MSG_PRINT_LEVEL			0x0C
//#define T1_NM_GET_MAP_USER_COUNTS			0x0D	// No Body
//#define T1_NM_GET_MAP_USER_COUNTS_ACK		0x0E
//#define T1_NM_SET_LIMIT_MONSTER_COUNT		0x0F
//#define T1_NM_SET_LIMIT_MONSTER_COUNT_ACK	0x10
//#define T1_NM_SERVER_THREAD_STATUS			0x11
//#define T1_NM_SET_CHANNEL_STATE_OK			0x12	// N->M, Channel Enable/Disable 결과

//#define T_NM_GET_NUM_MONSTERS				(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_NUM_MONSTERS)
//#define T_NM_GET_NUM_MONSTERS_OK			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_NUM_MONSTERS_OK)
//#define T_NM_SHUTDOWN						(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SHUTDOWN)
//#define T_NM_SHUTDOWN_OK					(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SHUTDOWN_OK)
//#define T_NM_PING							(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_PING)
//#define T_NM_PING_OK						(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_PING_OK)
//#define T_NM_PRINTMAP						(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_PRINTMAP)
//#define T_NM_SAVE_MAPBLOCK_INFO				(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SAVE_MAPBLOCK_INFO)
//#define T_NM_SEND_CHANNEL_INFO				(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SEND_CHANNEL_INFO)
//#define T_NM_CHANGE_BANDWIDTH_FLAG			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_CHANGE_BANDWIDTH_FLAG)
//#define T_NM_CHANGE_BANDWIDTH_FLAG_OK		(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_NM_SET_MSG_PRINT_LEVEL			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_MSG_PRINT_LEVEL)
//#define T_NM_GET_MAP_USER_COUNTS			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_MAP_USER_COUNTS)
//#define T_NM_GET_MAP_USER_COUNTS_ACK		(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_GET_MAP_USER_COUNTS_ACK)
//#define T_NM_SET_LIMIT_MONSTER_COUNT		(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_LIMIT_MONSTER_COUNT)
//#define T_NM_SET_LIMIT_MONSTER_COUNT_ACK	(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_LIMIT_MONSTER_COUNT_ACK)
//#define T_NM_SERVER_THREAD_STATUS			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SERVER_THREAD_STATUS)
//#define T_NM_SET_CHANNEL_STATE_OK			(MessageType_t)((T0_NM_MONITOR<<8)|T1_NM_SET_CHANNEL_STATE_OK)	// N->M, Channel Enable/Disable 결과

struct MSG_NM_GET_NUM_MONSTERS_OK
{
	USHORT	NumOfMonsters;
};

using MSG_NM_GET_MAP_USER_COUNTS = MSG_FM_GET_MAP_USER_COUNTS;

using MSG_NM_GET_MAP_USER_COUNTS_ACK = MSG_FM_GET_MAP_USER_COUNTS_ACK;

struct MSG_NM_SET_LIMIT_MONSTER_COUNT
{
	MAP_CHANNEL_INDEX	MapChannelIndex;
	UINT		SetLimitMonsterCount;
};

using MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK = MSG_NM_SET_LIMIT_MONSTER_COUNT;
using MSG_NM_SERVER_THREAD_STATUS = MSG_FM_SERVER_THREAD_STATUS;
using MSG_NM_SET_CHANNEL_STATE_OK = MSG_FM_SET_CHANNEL_STATE_OK;

//////////////////////////////////////////////////////////////////////////
// IM Server Monitor
//#define T1_IM_GET_NUM_CLIENTS			0x00	// No Body
//#define T1_IM_GET_NUM_CLIENTS_OK		0x01
//#define T1_IM_SHUTDOWN					0x02	// No Body
//#define T1_IM_SHUTDOWN_OK				0x03	// No Body
//#define T1_IM_PING						0x04	// No Body
//#define T1_IM_PING_OK					0x05	// No Body
//#define T1_IM_PRINTMAP					0x06	// No Body
//#define T1_IM_PRINT_DEBUG_MSG			0x08	// No Body
//#define T1_IM_CHANGE_BANDWIDTH_FLAG		0x0A	// No Body
//#define T1_IM_CHANGE_BANDWIDTH_FLAG_OK	0x0B	// 변경된 플래그값
//#define T1_IM_SET_MSG_PRINT_LEVEL		0x0C
//#define T1_IM_GET_TOTAL_USER_COUNTS		0x0D	// No Body
//#define T1_IM_GET_TOTAL_USER_COUNTS_ACK	0x0E
//#define T1_IM_SERVER_THREAD_STATUS		0x0F

//#define T_IM_GET_NUM_CLIENTS			(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_NUM_CLIENTS)
//#define T_IM_GET_NUM_CLIENTS_OK			(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_NUM_CLIENTS_OK)
//#define T_IM_SHUTDOWN					(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SHUTDOWN)
//#define T_IM_SHUTDOWN_OK				(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SHUTDOWN_OK)
//#define T_IM_PING						(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PING)
//#define T_IM_PING_OK					(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PING_OK)
//#define T_IM_PRINTMAP					(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PRINTMAP)
//#define T_IM_PRINT_DEBUG_MSG			(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_PRINT_DEBUG_MSG)
//#define T_IM_CHANGE_BANDWIDTH_FLAG		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_CHANGE_BANDWIDTH_FLAG)
//#define T_IM_CHANGE_BANDWIDTH_FLAG_OK	(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_CHANGE_BANDWIDTH_FLAG_OK)
//#define T_IM_SET_MSG_PRINT_LEVEL		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SET_MSG_PRINT_LEVEL)
//#define T_IM_GET_TOTAL_USER_COUNTS		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_TOTAL_USER_COUNTS)
//#define T_IM_GET_TOTAL_USER_COUNTS_ACK	(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_GET_TOTAL_USER_COUNTS_ACK)
//#define T_IM_SERVER_THREAD_STATUS		(MessageType_t)((T0_IM_MONITOR<<8)|T1_IM_SERVER_THREAD_STATUS)

struct MSG_FIELDSERVER_USERCOUNTS
{
	char	FieldIP[SIZE_MAX_IPADDRESS];
	USHORT	FieldListenPort;
	UINT	AccumulatedFieldUserCounts;
	UINT	CurrentFieldUserCounts;
	UINT	MaxFieldUserCounts;
	UINT	AccumulatedFieldMonsterCounts;
	UINT	CurrentFieldMonsterCounts;
};

struct MSG_IM_GET_TOTAL_USER_COUNTS_ACK
{
	UINT	AccumulatedTotalUserCounts;
	UINT	CurrentTotalUserCounts;
	UINT	MaxTotalUserCounts;
};

using MSG_IM_SERVER_THREAD_STATUS = MSG_FM_SERVER_THREAD_STATUS;

//////////////////////////////////////////////////////////////////////////
// PreServer <-> admin tool 
//#define T1_PA_ADMIN_CONNECT				0x00
//#define T1_PA_ADMIN_CONNECT_OK			0x01
//#define T1_PA_ADMIN_CHANGE_SERVER_STATE	0x02
//#define T1_PA_ADMIN_GET_ACCOUNT_INFO	0x03
//#define T1_PA_ADMIN_GET_ACCOUNT_INFO_OK	0x04
//#define T1_PA_ADMIN_DISCONNECT_USER		0x05
//#define T1_PA_ADMIN_DISCONNECT_USER_OK	0x06
//#define T1_PA_ADMIN_BLOCK_ACCOUNT		0x07
//#define T1_PA_ADMIN_BLOCK_ACCOUNT_OK	0x08
//#define T1_PA_ADMIN_UNBLOCK_ACCOUNT		0x09
//#define T1_PA_ADMIN_UNBLOCK_ACCOUNT_OK	0x0A
//#define T1_PA_ADMIN_GET_SERVER_INFO		0x0B
//#define T1_PA_ADMIN_GET_SERVER_INFO_OK	0x0C
//#define T1_PA_ADMIN_GET_ACCOUNT_LIST	0x0D
//#define T1_PA_ADMIN_GET_ACCOUNT_LIST_OK	0x0E
//#define T1_PA_ADMIN_RELOAD_HAPPYEV		0x0F		// A->P, No body, 2006-08-28 by cmkwon
//#define T1_PA_ADMIN_RELOAD_ITEMEV		0x10		// A->P, No body, 2006-08-31 by dhjin
//#define T1_PA_ADMIN_UPDATE_PCBANGLIST	0x11		// A->P, No body, 2007-01-22 by dhjin
//#define T1_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME	0x12		// A->P, No body, 2007-03-06 by dhjin
//#define T1_PA_ADMIN_PETITION_SET_PERIOD	0x13			// 2007-11-19 by cmkwon, 진정시스템 업데이트 - A->P
//#define T1_PA_ADMIN_SET_DBSERVER_GROUP		0x14			// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - A->P
//#define T1_PA_ADMIN_SET_DBSERVER_GROUP_OK	0x15			// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - P->A
//#define T1_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM	0x16		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - A->P, 각 서버군 자동 공지 리로드
//#define T1_PA_ADMIN_RELOAD_WORLDRANKING			0x17		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - A->P, 
//#define T1_PA_ADMIN_RELOAD_INFLUENCERATE		0x18		// A->P // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
//#define T1_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE					0x1B		
//#define T1_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE			0x1C
// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
//#define T1_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE						0x1D		// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전, 거점전 셋팅 실시간 적용

//#define T_PA_ADMIN_CONNECT				(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_CONNECT)				// A->P
//#define T_PA_ADMIN_CONNECT_OK			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_CONNECT_OK)			// P->A
//#define T_PA_ADMIN_CHANGE_SERVER_STATE	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_CHANGE_SERVER_STATE)	// P->A
//#define T_PA_ADMIN_GET_ACCOUNT_INFO		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_INFO)		// A->P
//#define T_PA_ADMIN_GET_ACCOUNT_INFO_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_INFO_OK)	// P->A
//#define T_PA_ADMIN_DISCONNECT_USER		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_DISCONNECT_USER)		// A->P
//#define T_PA_ADMIN_DISCONNECT_USER_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_DISCONNECT_USER_OK)	// P->A
//#define T_PA_ADMIN_BLOCK_ACCOUNT		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_BLOCK_ACCOUNT)			// A->P
//#define T_PA_ADMIN_BLOCK_ACCOUNT_OK		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_BLOCK_ACCOUNT_OK)		// P->A
//#define T_PA_ADMIN_UNBLOCK_ACCOUNT		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UNBLOCK_ACCOUNT)		// A->P
//#define T_PA_ADMIN_UNBLOCK_ACCOUNT_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UNBLOCK_ACCOUNT_OK)	// P->A
//#define T_PA_ADMIN_GET_SERVER_INFO		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_SERVER_INFO)		// A->P, no body
//#define T_PA_ADMIN_GET_SERVER_INFO_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_SERVER_INFO_OK)	// P->A
//#define T_PA_ADMIN_GET_ACCOUNT_LIST		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_LIST)		// A->P
//#define T_PA_ADMIN_GET_ACCOUNT_LIST_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_GET_ACCOUNT_LIST_OK)	// P->A
//#define T_PA_ADMIN_RELOAD_HAPPYEV		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_HAPPYEV)		// A->P, No body, 2006-08-28 by cmkwon
//#define T_PA_ADMIN_RELOAD_ITEMEV		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_ITEMEV)			// A->P, No body, 2006-08-31 by dhjin
//#define T_PA_ADMIN_UPDATE_PCBANGLIST	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UPDATE_PCBANGLIST)		// A->P, No body, 2007-01-22 by dhjin
//#define T_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME)		// A->P, No body, 2007-01-22 by dhjin
//#define T_PA_ADMIN_PETITION_SET_PERIOD	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_PETITION_SET_PERIOD)	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - A->P
//#define T_PA_ADMIN_SET_DBSERVER_GROUP		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_SET_DBSERVER_GROUP)	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - A->P
//#define T_PA_ADMIN_SET_DBSERVER_GROUP_OK	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_SET_DBSERVER_GROUP_OK)	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - P->A
//#define T_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM	(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM)	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - A->P, 각 서버군 자동 공지 리로드
//#define T_PA_ADMIN_RELOAD_WORLDRANKING			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_WORLDRANKING)			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - A->P, 
//#define T_PA_ADMIN_RELOAD_INFLUENCERATE			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_RELOAD_INFLUENCERATE)		// A->P // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
//#define T_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE			(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE)		// A->P
//#define T_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE		(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE)	// A->P
// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
//#define T_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE				(MessageType_t)((T0_PA_ADMIN<<8)|T1_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE)	// A->P	// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

struct MSG_PA_ADMIN_CONNECT
{
	char	UID[SIZE_MAX_ACCOUNT_NAME];
	char	PWD[SIZE_MAX_PASSWORD];
	DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
};

struct MSG_PA_ADMIN_CONNECT_OK
{
	// 2006-04-15 by cmkwon, 필드 변경
	//	BYTE	AuthOK;				// 0: FALSE, 1:TRUE
	USHORT	AccountType0;		// 계정 권한
	//DWORD	Padding[SIZE_MAX_PACKET_PADDING];		// 2011-07-21 by hskim, 인증 서버 구현 - 기존 서버와 호환 안되도록 구조체 크기 바꿈
	//int		IsMasang;	// 2012-06-16 by jhseol, 아레나 추가개발part2 - 전달 : 마상 내부가 아니면 사용 불가
};

struct MSG_PA_ADMIN_CHANGE_SERVER_STATE
{
	SERVER_ID	ServerID;
	INT			ServerType;		// ENServerType, ST_XXX
	BYTE		ServerState;	// SERVER_STATE_XXX
};

struct MSG_PA_ADMIN_GET_ACCOUNT_INFO
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct CAccountInfo;

struct MEX_ACCOUNT_INFO_PRE_SERVER
{
	MEX_ACCOUNT_INFO_PRE_SERVER()
	{
		memset(this, 0x00, sizeof(MEX_ACCOUNT_INFO_PRE_SERVER));
	}

	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t				AccountUniqueNumber;
	char				PublicClientIP[SIZE_MAX_IPADDRESS];
	char				PrivateClientIP[SIZE_MAX_IPADDRESS];
	char				CurrentCharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CurrentCharacterUniqueNumber;
	char				CurrentServerGroup[SIZE_MAX_SERVER_NAME];
	SERVER_ID			CurrentFieldServerID;
	MAP_CHANNEL_INDEX	CurrentMapChannel;
	INT					ClientState;

#ifdef _ATUM_PRE_SERVER
	// operator overloading
	MEX_ACCOUNT_INFO_PRE_SERVER& operator=(const CAccountInfo& rhs);
#endif // _ATUM_IM_SERVER
};

struct MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK
{
	BYTE	IsOnline;			// 0: FALSE, 1:TRUE
	BYTE	IsBlocked;			// 0: FALSE, 1:TRUE
	SBLOCKED_ACCOUNT_INFO		BlockedAccountInfo;
	MEX_ACCOUNT_INFO_PRE_SERVER	AccountInfo;
};

struct MSG_PA_ADMIN_DISCONNECT_USER
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_PA_ADMIN_DISCONNECT_USER_OK
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
};

using MSG_PA_ADMIN_BLOCK_ACCOUNT = SBLOCKED_ACCOUNT_INFO;

using MSG_PA_ADMIN_BLOCK_ACCOUNT_OK = SBLOCKED_ACCOUNT_INFO;


// typedef struct
// {
// 	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
// } MSG_PA_ADMIN_UNBLOCK_ACCOUNT;
using MSG_PA_ADMIN_UNBLOCK_ACCOUNT = SBLOCKED_ACCOUNT_INFO;	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 

struct MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
};

// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
struct MSG_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t	AccountUniqueNumber;
	USHORT	OldAccountType;
	USHORT	NewAccountType;
};

struct MSG_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t	AccountUniqueNumber;
	UID32_t	CharacterUniqueNumber;
	USHORT	OldAccountType;
	USHORT	NewAccountType;
};
// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

//typedef struct
//{
//} MSG_PA_ADMIN_GET_SERVER_INFO;

#define SERVER_STATE_NOT_ACTIVE	((BYTE)0)
#define SERVER_STATE_NORMAL		((BYTE)1)
#define SERVER_STATE_ABNORMAL	((BYTE)2)

// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - MEX_SERVER_GROUP_INFO_FOR_ADMIN 로 대체 함
// struct MEX_SERVER_INFO_FOR_ADMIN
// {
// 	char		ServerName[SIZE_MAX_SERVER_NAME];
// 	INT			ServerType;							// ENServerType, ST_XXX
// 	SERVER_ID	ServerID;
// 	INT			UserCount;						
// 	BYTE		ServerState;						// SERVER_STATE_XXX
// };

struct MEX_SERVER_GROUP_INFO_FOR_ADMIN				// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
{
	char		ServerName[SIZE_MAX_SERVER_NAME];
	SERVER_ID	IMServerID;
	INT			UserCount;
	BYTE		IMServerState;						// SERVER_STATE_XXX
	SERVER_ID	FieldServerID;
	BYTE		FieldServerState;					// SERVER_STATE_XXX
	int			ServerGroupID;						// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	int			LimitUserCount;						// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	bool		LockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
};

struct MSG_PA_ADMIN_GET_SERVER_INFO_OK
{
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	//	INT		NumOfServers;
	//	ARRAY_(MEX_SERVER_INFO_FOR_ADMIN);
	INT		NumOfServerGroups;						// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	ARRAY_(MEX_SERVER_GROUP_INFO_FOR_ADMIN);		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
};

struct MSG_PA_ADMIN_GET_ACCOUNT_LIST
{
	char	ServerName[SIZE_MAX_SERVER_NAME];
};

struct MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN
{
	char	AccountName[SIZE_MAX_ACCOUNT_NAME];
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK
{
	size_t		NumOfAccounts;
	ARRAY_(MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN);
};


using MSG_PA_ADMIN_PETITION_SET_PERIOD = MSG_IP_ADMIN_PETITION_SET_PERIOD;	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 

struct MSG_PA_ADMIN_SET_DBSERVER_GROUP		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
	int			LimitUserCount;
	bool		LockCreateCharacterForNewAccount;
};

using MSG_PA_ADMIN_SET_DBSERVER_GROUP_OK = MSG_PA_ADMIN_SET_DBSERVER_GROUP;		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

struct MSG_PA_ADMIN_RELOAD_INFLUENCERATE	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
{
	char		ServerGroupName[SIZE_MAX_SERVER_NAME];
};

//////////////////////////////////////////////////////////////////////////
// IMServer <-> admin tool 
//#define T1_IA_ADMIN_CONNECT				0x00
//#define T1_IA_ADMIN_CONNECT_OK			0x01
//#define T1_IA_ADMIN_CHANGE_SERVER_STATE	0x02
//#define T1_IA_ADMIN_GET_CHARACTER		0x03
//#define T1_IA_ADMIN_GET_CHARACTER_OK	0x04
//#define T1_IA_ADMIN_CHAT_ALL			0x05
//#define T1_IA_ADMIN_GET_SERVER_INFO		0x06
//#define T1_IA_ADMIN_GET_SERVER_INFO_OK	0x07

//#define T_IA_ADMIN_CONNECT				(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CONNECT)				// A->I
//#define T_IA_ADMIN_CONNECT_OK			(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CONNECT_OK)			// I->A
//#define T_IA_ADMIN_CHANGE_SERVER_STATE	(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CHANGE_SERVER_STATE)	// I->A
//#define T_IA_ADMIN_GET_CHARACTER		(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_CHARACTER)			// A->I
//#define T_IA_ADMIN_GET_CHARACTER_OK		(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_CHARACTER_OK)		// I->A
//#define T_IA_ADMIN_CHAT_ALL				(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_CHAT_ALL)				// A->I
//#define T_IA_ADMIN_GET_SERVER_INFO		(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_SERVER_INFO)		// A->I, No Body
//#define T_IA_ADMIN_GET_SERVER_INFO_OK	(MessageType_t)((T0_IA_ADMIN<<8)|T1_IA_ADMIN_GET_SERVER_INFO_OK)	// I->A

struct MSG_IA_ADMIN_CONNECT
{
	char	UID[SIZE_MAX_ACCOUNT_NAME];
	char	PWD[SIZE_MAX_PASSWORD];
};

struct MSG_IA_ADMIN_CONNECT_OK
{
	// 2006-04-15 by cmkwon, 필드 변경
	//	BYTE	AuthOK;				// 0: FALSE, 1:TRUE
	USHORT	AccountType0;		// 계정 권한
};

using MSG_IA_ADMIN_CHANGE_SERVER_STATE = MSG_PA_ADMIN_CHANGE_SERVER_STATE;

struct MSG_IA_ADMIN_GET_CHARACTER
{
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IA_ADMIN_GET_CHARACTER_OK
{
	BYTE	IsOnline;			// 0: FALSE, 1:TRUE
};

using MSG_IA_ADMIN_CHAT_ALL = MSG_IC_CHAT_ALL;

struct MSG_IA_ADMIN_GET_SERVER_INFO_OK
{
	INT			UserCount;
	BYTE		ServerState;	// SERVER_STATE_XXX
};

//////////////////////////////////////////////////////////////////////////
// FieldServer <-> admin tool 
//#define T1_FA_ADMIN_CONNECT				0x00
//#define T1_FA_ADMIN_CONNECT_OK			0x01
//#define T1_FA_ADMIN_CHANGE_SERVER_STATE	0x02

//#define T_FA_ADMIN_CONNECT				(MessageType_t)((T0_FA_ADMIN<<8)|T1_FA_ADMIN_CONNECT)			// A->F
//#define T_FA_ADMIN_CONNECT_OK			(MessageType_t)((T0_FA_ADMIN<<8)|T1_FA_ADMIN_CONNECT_OK)		// F->A
//#define T_FA_ADMIN_CHANGE_SERVER_STATE	(MessageType_t)((T0_FA_ADMIN<<8)|T1_FA_ADMIN_CHANGE_SERVER_STATE)	// F->A

struct MSG_FA_ADMIN_CONNECT
{
	char	UID[SIZE_MAX_ACCOUNT_NAME];
	char	PWD[SIZE_MAX_PASSWORD];
};

struct MSG_FA_ADMIN_CONNECT_OK
{
	// 2006-04-15 by cmkwon, 필드 변경
	//	BYTE	AuthOK;				// 0: FALSE, 1:TRUE
	USHORT	AccountType0;		// 계정 권한
};

using MSG_FA_ADMIN_CHANGE_SERVER_STATE = MSG_PA_ADMIN_CHANGE_SERVER_STATE;

// IMServer -> Client
using MSG_IC_COUNTDOWN_START = MSG_FC_COUNTDOWN_START;

// Client -> IMServer
using MSG_IC_COUNTDOWN_DONE = MSG_FC_COUNTDOWN_DONE;

//struct MSG_IC_VOIP_SET
//{// 2008-06-17 by dhjin, EP3 VOIP - 설정
//	EN_CHECK_TYPE	VoipType;
//};
//
//struct MSG_IC_VOIP_SET_OK
//{// 2008-06-17 by dhjin, EP3 VOIP - 설정
//	UID32_t	CharacterUID;
//	EN_CHECK_TYPE	VoipType;
//};

//////////////////////////////////////////////////////////////////////////
// 2008-06-16 by dhjin, EP3 채팅방 -
struct MSG_IC_CHATROOM_CREATE
{// 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 생성
	CHAR	ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	bool	ChatRoomLock;
	CHAR	ChatRoomPW[SIZE_MAX_TEAM_PW];
	BYTE	ChatRoomMaxCount;
};

struct MSG_IC_CHATROOM_CREATE_OK
{// 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 생성
	INT			ChatRoomNum;
};

struct MSG_IC_CHATROOM_LIST_INFO
{// 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 리스트 
	INT			StartNum;
};

struct SCHATROOM_LIST_INFO
{// 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 리스트  OK
	INT			ChatRoomNum;
	CHAR		ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	bool		ChatRoomLock;
	CHAR		ChatRoomMasterName[SIZE_MAX_CHARACTER_NAME];
	BYTE		Membercount;
	BYTE		MaxMembercount;
};

struct MSG_IC_CHATROOM_LIST_INFO_OK
{// 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 리스트  OK
	INT			ChatRoomInfoTotalCount;	// 2008-06-17 by dhjin, 채팅방 최대 목록
	INT			ChatRoomInfoListCount;		// 2008-06-17 by dhjin,
	_ARRAY(SCHATROOM_LIST_INFO);
};

struct MSG_IC_CHATROOM_REQUEST_INVITE
{// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 초대 
	INT		ChatRoomNum;
	char	InviteeCharacterName[SIZE_MAX_CHARACTER_NAME];		// 초대할 상대방 아이디
};

struct MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION
{// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 초대자에게 정보 전송
	INT		ChatRoomNum;
	char	ChatRoomMasterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_ACCEPT_INVITE
{// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 초대 승락
	INT		ChatRoomNum;
};

struct MSG_IC_CHATROOM_ACCEPT_INVITE_OK
{// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 초대 승락 유저 정보 전송
	UID32_t	CharacterUID;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_JOIN
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 자유 참여
	INT		ChatRoomNum;
	bool    ChatRoomLock;
	char	ChatRoomPW[SIZE_MAX_TEAM_PW];
};

struct MSG_IC_CHATROOM_REJECT_INVITE
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 초대 취소
	INT		ChatRoomNum;
};

struct MSG_IC_CHATROOM_LEAVE
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 나가기
	INT		ChatRoomNum;
	char	LeaveCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_LEAVE_OK
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 나가기
	UID32_t	CharacterUID;
	char	LeaveCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_BAN
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 추방 
	INT		ChatRoomNum;
	UID32_t	BanCharacterUID;
	char	BanCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_BAN_OK
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 추방 
	UID32_t	BanCharacterUID;
	char	BanCharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_NAME
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 이름 변경 
	INT		ChatRoomNum;
	char	ChatRoomName[SIZE_MAX_CHATROOM_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_NAME_OK
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 이름 변경 
	char	ChatRoomName[SIZE_MAX_CHATROOM_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_MASTER
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 방장 변경
	INT		ChatRoomNum;
	UID32_t	NewCharacterUID;
};

struct MSG_IC_CHATROOM_CHANGE_MASTER_OK
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 방장 변경
	UID32_t	NewCharacterUID;
	char	CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_CHANGE_LOCK_PW
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 잠금 변경 
	INT		ChatRoomNum;
	bool    ChatRoomLock;
	char	ChatRoomPW[SIZE_MAX_TEAM_PW];
};

struct MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 잠금 변경
	bool    ChatRoomLock;
};

struct MSG_IC_CHATROOM_CHANGE_MAX_MEMBER
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 최대 인원수 변경
	INT		ChatRoomNum;
	BYTE	ChatRoomMaxMember;
};

struct MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK
{// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 최대 인원수 변경
	BYTE	ChatRoomMaxMember;
};

struct MSG_IC_CHATROOM_MEMBER_INFO
{// 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 맴버 정보
	INT		ChatRoomNum;
};

struct SCHATROOM_MEMBER_INFO
{/// 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 맴버 정보
	UID32_t		CharacterUID;
	char		CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_IC_CHATROOM_MEMBER_INFO_OK
{/// 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 맴버 정보
	INT			ChatRoomNum;
	INT			ChatRoomInfoListCount;
	UID32_t		MasterCharacterUID;
	char		ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	bool		ChatRoomLock;
	char		ChatRoomPW[SIZE_MAX_TEAM_PW];
	INT			ChatRoomMaxCount;
	_ARRAY(SCHATROOM_MEMBER_INFO);
};


///////////////////////////////////////////////////////////////////////////////
// T0_FC_RACING
struct MSG_FC_RACING_RACINGLIST_REQUEST_ACK
{
	BYTE		byRacingListCounts;
	ARRAY_(MAP_CHANNEL_INDEX);
};

struct MSG_FC_RACING_RACINGINFO_REQUEST
{
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_RACINGINFO_REQUEST_ACK
{
	MAP_CHANNEL_INDEX	mapChannelIndex;
	BYTE				byMaxUserCounts;				// 최대 참가자수
	BYTE				byMinUserCounts;				// 최소 참가자수
	BYTE				byRacingGameNumber;				// 진행되는 게임 넘버
	int					nEntryFee;						// 참가비(단위:spi)
	int					nLimitTimeForRacing;			// Racing이 진행되는 제한시간(단위:초)
	int					nRamainTimeForJoinRacing;		// 참가 가능 시간까지 남은 시간(단위:초)	
};

struct MSG_FC_RACING_RACINGNOTIFY
{
	MAP_CHANNEL_INDEX	mapChannIdx;
	DWORD				dwRamainTimeForJoinRacing;		// 참가 가능 시간까지 남은 시간(단위:초)
};

struct MSG_FC_RACING_JOIN_ENABLE
{
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_JOIN_REQUEST
{
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_JOIN_REQUEST_ACK
{
	BYTE				byJoinedFlag : 1;					// 1 bit 사용, 레이싱에 참여 되었는지의 플래그
	BYTE				byJoinedOrder : 7;				// 7 bit 사용, 레이싱에 참여된 순서(ex, 8명이면 1부터 8까지)
	MAP_CHANNEL_INDEX	mapChannIdx;
};

struct MSG_FC_RACING_COUNTDOWN
{
	BYTE				byCountDowns;
};

struct MSG_FC_RACING_CHECKPOINT_CHECK
{
	BYTE				byCheckPointIndex;
	DWORD				dwObjectType;
	AVECTOR3			PositionAVector3;
};

struct MSG_FC_RACING_CHECKPOINT_CHECK_ACK
{
	BYTE				byCheckedFlag : 1;
	BYTE				byCheckPointIndex : 7;
	BYTE				byRanking;
};

struct MSG_FC_RACING_FINISHED
{
	BYTE				byRanking;
};

struct MSG_FC_RACING_OTHER_FINISHED
{
	char				szCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE				byRanking;
};

struct RankingInfo
{
	char				szCharacterName[SIZE_MAX_CHARACTER_NAME];
	BYTE				byRanking;
	DWORD				dwElapsedTime;
};

struct MSG_FC_RACING_FINALRANKING
{
	BYTE				byRankingInfoCounts;		// Ranking 정보 카운트
	ARRAY_(RankingInfo);
};

//////////////////////////////////////////////////////////////////////////
// 2007-04-17 by dhjin
// T0_FC_ARENA
//#define T_FC_ARENA_PLAY_CHARACTER_TYPE		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_PLAY_CHARACTER_TYPE)			// c->f(a)	// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
//#define T_FC_ARENA_REQUEST_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_TEAM)					// C->F
//#define T_FC_ARENA_REQUEST_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_TEAM_OK)				// F->C
//#define T_FC_ARENA_CREATE_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_CREATE_TEAM)					// C->F
//#define T_FC_ARENA_CREATE_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_CREATE_TEAM_OK)				// F->C
//#define T_FC_ARENA_ENTER_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_TEAM)					// C->F
//#define T_FC_ARENA_ENTER_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_TEAM_OK)					// F->C
//#define T_FC_ARENA_TEAM_MEMBER_LIST			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_MEMBER_LIST)				// F->C(n)
//#define T_FC_ARENA_REAVE_TEAM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REAVE_TEAM)					// C->F
//#define T_FC_ARENA_REAVE_TEAM_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REAVE_TEAM_OK)					// F->C
//#define T_FC_ARENA_TEAM_READY				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_READY)					// F->C(n)
//#define T_FC_ARENA_TEAM_READY_FINISH		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_READY_FINISH)				// C->F
//#define T_FC_ARENA_TEAM_READY_FINISH_CANCEL	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_READY_FINISH_CANCEL)		// C->F
//#define T_FC_ARENA_TEAM_MATCHING			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_TEAM_MATCHING)					// F->C(n)
//#define T_FC_ARENA_ENTER_ROOM				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_ROOM)					// F->C(n)
//#define T_FC_ARENA_ENTER_ROOM_WARP			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ENTER_ROOM_WARP)				// C->F
//#define T_FC_ARENA_ROOM_WAR_START			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_START)				// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_INFO			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_INFO)					// F->C(n)	
//#define T_FC_ARENA_ROOM_WAR_FINISH_HEADER	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH_HEADER)		// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_FINISH			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH)				// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_FINISH_DONE		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH_DONE)			// F->C(n)
//#define T_FC_ARENA_ROOM_WAR_FINISH_DRAW		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_ROOM_WAR_FINISH_DRAW)				// F->C(n)
//#define T_FC_ARENA_FINISH_WARP				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_FINISH_WARP)					// C->F
//#define T_FC_ARENA_FINISH_WARP_OK			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_FINISH_WARP_OK)				// F->C(n)
//#define T_FC_ARENA_REQUEST_CREATE_TEAMINFO  (MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_CREATE_TEAMINFO)		// C->F
//#define T_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK  (MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK)	// F->C
//#define T_FC_ARENA_BEFORE_ROOM_WAR_FINISH	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_BEFORE_ROOM_WAR_FINISH)		// F->C
//#define T_FC_ARENA_REQUEST_OTHER_TEAM_INFO  (MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_OTHER_TEAM_INFO)	// C->F
//#define T_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK)	// F->C
//#define T_FC_ARENA_GM_COMMAND_INFO_HEADER	(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_GM_COMMAND_INFO_HEADER)	// F->C
//#define T_FC_ARENA_GM_COMMAND_INFO			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_GM_COMMAND_INFO)	// F->C
//#define T_FC_ARENA_GM_COMMAND_INFO_DONE		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_GM_COMMAND_INFO_DONE)	// F->C
//#define T_FC_ARENA_QUICK_ENTER_TEAM			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_QUICK_ENTER_TEAM)	// C->F
//#define T_FC_ARENA_WATCH_READY				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_READY)		// F->C
//#define T_FC_ARENA_WATCH_START				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_START)		// F->C
//#define T_FC_ARENA_WATCH_END				(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_END)			// F->C
//#define T_FC_ARENA_WATCH_REMOVE_USER		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_WATCH_REMOVE_USER)			// F->C
// 2008-01-08 by dhjin, 아레나 통합 - 
//#define T_FC_ARENA_POSSIBLE_GAMESTART		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_POSSIBLE_GAMESTART)			// F->C
//#define T_FC_ARENA_CHARACTER_GAMESTART		(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_CHARACTER_GAMESTART)			// C->F
//#define T_FC_ARENA_USE_CITYWARPITEM			(MessageType_t)((T0_FC_ARENA<<8)|T1_FC_ARENA_USE_CITYWARPITEM)				// F->C



struct MSG_FC_ARENA_REQUEST_TEAM
{
	BYTE				ArenaMode;			// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	BYTE				RequestTeamState;	// 2007-06-05 by dhjin, 팀 정보 요청 플래그 1 : 모두 보기, 2 : 대기팀. 3 : 상대팀 보기
	ClientIndex_t		ClientIndex;		// 2007-04-17 by dhjin, 요청한 클라이언트
	INT					StartTeamNum;		// 2007-04-27 by dhjin, 한 번에 요청하여 보내주는 팀 시작 번호 ex)11 => 11번째 방부터 ARENA_MAX_REQUEST_TEAM까지 전송
	MapIndex_t			ArenaMap;			// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
};

struct SARENA_TEAM_INFO
{
	bool			TeamLock;				// 2007-04-17 by dhjin, 팀 공개 여부  0 : 공개, 1: 비공개 
	bool			TeamEvent;				// 2008-03-10 by dhjin, 아레나 통합 - 이벤트팀인지 0: 일반팀 1: 이벤트팀
	BYTE			TeamSize;				// 2007-04-17 by dhjin, 팀 인원 제한 
	BYTE			CurrentTeamSize;		// 2007-04-25 by dhjin, 현재 팀 인원
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	BYTE			ArenaState;				// 2007-06-05 by dhjin, 팀의 아레나 상태
	BYTE			TeamStartLevel;			// 2007-06-05 by dhjin, 팀의 시작 레벨
	BYTE			TeamEndLevel;			// 2007-06-05 by dhjin, 팀의 끝 레벨
	INT				TeamNum;				// 2007-04-17 by dhjin, 팀 번호
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		// 2007-04-17 by dhjin, 방장 이름
	MapIndex_t		ArenaMap;				// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
};

struct MSG_FC_ARENA_REQUEST_TEAM_OK
{
	INT				ArenaTeamInfoTotalCount;	// 2007-04-27 by dhjin, 팀 최대 목록
	INT				ArenaTeamInfoListCount;		// 2007-04-19 by dhjin,
	BYTE			RequetTeamState;			// 2007-06-07 by dhjin, 팀 정보 요청 플래그 1 : 모두 보기, 2 : 대기팀. 3 : 상대팀 보기 
	MapIndex_t		ArenaMap;					// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
	_ARRAY(SARENA_TEAM_INFO);
};

struct MSG_FC_ARENA_CREATE_TEAM
{
	bool			TeamLock;				// 2007-04-17 by dhjin, 팀 공개 여부 0 : 공개, 1: 비공개
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	BYTE			TeamSize;				// 2007-04-17 by dhjin, 팀 인원 제한 
	ClientIndex_t	ClientIndex;			// 2007-04-17 by dhjin, 요청한 클라이언트
	CHAR			TeamPW[SIZE_MAX_TEAM_PW];	// 2007-04-17 by dhjin, 팀 비밀 번호
	MapIndex_t		ArenaMap;				// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
};

struct MSG_FC_ARENA_CREATE_TEAM_OK
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	BYTE			TeamSize;				// 2007-04-17 by dhjin, 팀 인원 제한 
	INT				TeamNum;				// 2007-04-17 by dhjin, 생성된 팀 번호
	MapIndex_t		ArenaMap;				// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
};

struct MSG_FC_ARENA_ENTER_TEAM
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	ClientIndex_t	ClientIndex;			// 2007-04-17 by dhjin, 요청한 클라이언트
	CHAR			TeamPW[SIZE_MAX_TEAM_PW];	// 2007-04-17 by dhjin, 팀 비밀 번호
	INT				TeamNum;				// 2007-04-17 by dhjin, 입장하려는 팀 번호	
};

struct MSG_FC_ARENA_ENTER_TEAM_OK
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	BYTE			TeamSize;				// 2007-04-17 by dhjin, 팀 인원 제한 
	INT				TeamNum;				// 2007-04-17 by dhjin, 생성된 팀 번호
};

struct SARENA_TEAM_CHARACTERINFO
{
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-04-17 by dhjin, 팀원들의 이름
	INT				PlayCharacterType;		// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
};

struct MSG_FC_ARENA_TEAM_MEMBER_LIST
{
	BYTE			ArenaTeamState;						// 2007-05-07 by dhjin, 아레나 팀 상태
	BYTE			ArenaTeamCharacterNameListCount;	// 2007-04-19 by dhjin
	_ARRAY(SARENA_TEAM_CHARACTERINFO);
};

struct MSG_FC_ARENA_PLAY_CHARACTER_TYPE
{
	INT				PlayCharacterType;		// 2012-06-08 by jhseol, 아레나 추가개발part2 - 케릭터 : 타입 복사
};

struct MSG_FC_ARENA_REAVE_TEAM
{
	BYTE			ArenaMode;				// 2007-04-17 by dhjin, 아레나 방식   1 : DeathMatch, 2 : Round, so on..
	ClientIndex_t	ClientIndex;			// 2007-04-17 by dhjin, 요청한 클라이언트
	INT				TeamNum;				// 2007-04-17 by dhjin, 탈퇴하려는 팀 번호
};

struct MSG_FC_ARENA_TEAM_READY
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태, 2 : ARENA_STATE_READY
	INT				TeamNum;				// 2007-04-17 by dhjin, 팀 번호
};

struct MSG_FC_ARENA_ENTER_ROOM
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태, 3 : ARENA_STATE_WARREADY
	BYTE			VCNDeathCount;			// 2007-04-17 by dhjin, 바이제니유 데스매치 포인트
	BYTE			ANIDeathCount;			// 2007-04-17 by dhjin, 알링턴 데스매치 포인트
	BYTE			PlayLimitedTime;		// 2007-05-04 by dhjin, 경기 제한 시간
	INT				TeamNum;				// 2007-04-17 by dhjin, 팀 번호
	ATUM_DATE_TIME	StartRoomTime;			// 2007-04-17 by dhjin, 방 생성 시간
	ATUM_DATE_TIME	ArenaServerTime;		// 2009-09-10 by jwlee, 아레나 서버 시간 오차 버그 수정 - 
};

struct MSG_FC_ARENA_ENTER_ROOM_WARP
{// 2008-02-21 by dhjin, 아레나 통합 - 
	bool			CheckCommand;			// 2008-02-21 by dhjin, '/아레나이동' 명령어 사용인지 체크
};

struct MSG_FC_ARENA_ROOM_WAR_START
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태, 4 : ARENA_STATE_WARING
	ATUM_DATE_TIME	Time;					// 2007-04-17 by dhjin, 대전 시작 시간
};

struct MSG_FC_ARENA_ROOM_WAR_INFO
{
	BYTE			ArenaMode;
	BYTE			VCNDeathCount;			// 2007-04-17 by dhjin, 바이제니유 데스매치 포인트
	BYTE			ANIDeathCount;			// 2007-04-17 by dhjin, 알링턴 데스매치 포인트
											//	ATUM_DATE_TIME	Time;					// 2007-04-17 by dhjin, 대전 남은 시간
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH
{
	BYTE			Influence;				// 2007-04-17 by dhjin, 세력 
	BYTE 			ShootingDown;			// 2007-04-17 by dhjin, 격추수 
	BYTE 			SufferingAttack;		// 2007-04-17 by dhjin, 피격수 
	BYTE			LostPoint;				// 2007-06-01 by dhjin, LostPoint
	USHORT			UnitKind;				// 2007-04-17 by dhjin, 기어 종류 
	CHAR			CharacterLevelRank[SIZE_MAX_LEVELRANK];	// 2007-04-17 by dhjin, 유저 레벨 등급
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-04-17 by dhjin, 유저 이름
	FLOAT			Average;				// 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보
	INT				TeamAverage;			// 2012-06-08 by jhseol, 아레나 추가개발part2 - HP/DP 정보
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태, 5 : ARENA_STATE_WARFINISH
	ATUM_DATE_TIME	FinishTime;				// 2007-04-17 by dhjin, 대전 종료 시간
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH_DONE
{
	BYTE			Issue;					// 2007-04-20 by dhjin, 아레나 승패 여부, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	BYTE			EndState;				// 2007-05-16 by dhjin, 아레나 종료 타입, 1 : ARENA_END_TYPE_POINT, 2 : ARENA_END_TYPE_TIME, 3 : ARENA_END_TYPE_GIVEUP
	INT				InflPoint;				// 2007-04-17 by dhjin, 세력 포인트 보상값
	INT				WarPoint;				// 2007-04-17 by dhjin, WarPoint 보상값
	INT				TotalWarPoint;			// 2007-05-28 by dhjin, WP
	INT				CumulativeWarPoint;		// 2007-05-28 by dhjin, CumulativeWP
	INT				ArenaWin;				// 2007-06-07 by dhjin, 아레나 승패 전적 승
	INT				ArenaLose;				// 2007-06-07 by dhjin, 아레나 승패 전적 패
};

struct MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW
{
	BYTE			ArenaMode;
	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태, 5 : ARENA_STATE_WARFINISH
	BYTE			Issue;					// 2007-04-20 by dhjin, 아레나 승패 여부, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	BYTE			EndState;				// 2007-05-16 by dhjin, 아레나 종료 타입, 1 : ARENA_END_TYPE_POINT, 2 : ARENA_END_TYPE_TIME, 3 : ARENA_END_TYPE_GIVEUP
	INT				InflPoint;				// 2007-04-17 by dhjin, 세력 포인트 보상값
	INT				WarPoint;				// 2007-04-17 by dhjin, WarPoint 보상값
	ATUM_DATE_TIME	FinishTime;				// 2007-04-17 by dhjin, 대전 종료 시간
};

//struct SARENA_ROOM_FINISH_INFO
//{
//	BYTE			Influence;				// 2007-04-17 by dhjin, 세력 
//// 	BYTE			Rank;					// 2007-04-17 by dhjin, 격추수에 의한 랭킹
//	BYTE 			ShootingDown;			// 2007-04-17 by dhjin, 격추수 
//	BYTE 			SufferingAttack;		// 2007-04-17 by dhjin, 피격수 
//	USHORT			UnitKind;				// 2007-04-17 by dhjin, 기어 종류 
//	CHAR			CharacterLevelRank[SIZE_MAX_LEVELRANK];	// 2007-04-17 by dhjin, 유저 레벨 등급
//	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];	// 2007-04-17 by dhjin, 유저 이름
//};
//
//struct MSG_FC_ARENA_ROOM_WAR_FINISH
//{
//	BYTE			ArenaMode;
//	BYTE			ArenaState;				// 2007-04-17 by dhjin, 팀의 아레나 상태, 5 : ARENA_STATE_WARFINISH
//	BYTE			Issue;					// 2007-04-20 by dhjin, 아레나 승패 여부, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
//	INT				InflPoint;				// 2007-04-17 by dhjin, 세력 포인트 보상값
//	INT				WarPoint;				// 2007-04-17 by dhjin, WarPoint 보상값
//	ATUM_DATE_TIME	FinishTime;				// 2007-04-17 by dhjin, 대전 종료 시간
//	BYTE			ArenaRoomFinishInfoDeathMatchListCount; // 2007-04-19 by dhjin
//	_ARRAY(SARENA_ROOM_FINISH_INFO);
//};

struct MSG_FC_ARENA_FINISH_WARP_OK
{
	MAP_CHANNEL_INDEX	MapChannelIndex;	// 2007-04-17 by dhjin, 맵 채널 정보
};

struct SARENA_REQUEST_CREATE_TEAMINFO
{
	BYTE 	ArenaMode;				// 아레나 모드 1 : DeathMatch, 2 : Round
	BYTE 	ReqMemberNum; 			// 팀 인원 제한
	MapIndex_t	ArenaMap;			// 2012-04-12 by jhseol, 아레나 추가개발 - 선택한 아레나 맵
};

struct MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK
{
	BYTE	ArenaRequetCreateTeamInfoListCount; // 2007-04-26 by dhjin
	_ARRAY(SARENA_REQUEST_CREATE_TEAMINFO);
};

// 2007-05-23 by dhjin, 상대 팀 정보 전송
struct SARENA_REQUEST_OTHER_TEAM_INFO_OK
{
	BYTE	MemberCount;			// 팀 인원수
	INT		TeamCount;				// 팀 수
};

struct MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK
{
	BYTE 	ArenaMode;				// 아레나 모드 1 : DeathMatch, 2 : Round
	BYTE	ArenaOtherTeamListCount;
	_ARRAY(SARENA_REQUEST_OTHER_TEAM_INFO_OK);
};

// 2007-05-25 by dhjin, GM명령어 출력을 위한 구조체
struct SARENA_GM_COMMAND_INFO
{
	BYTE 	ArenaMode;							// 아레나 모드 1 : DeathMatch, 2 : Round
	BYTE	Level;								// 팀 레벨
	BYTE	MemberCount;						// 팀 인원수
	INT		TeamCount;							// 팀 수
};
using MSG_FC_ARENA_GM_COMMAND_INFO = SARENA_GM_COMMAND_INFO;

struct MSG_FC_ARENA_GM_COMMAND_INFO_DONE
{
	INT	ReadyUserTotalCount;
	INT	MatchIngUserTotalCount;
	INT	ArenaFightIngUserTotalCount;
};

struct MSG_FC_ARENA_WATCH_READY
{
	BYTE			ArenaState;			// 2007-06-18 by dhjin, 팀의 아레나 상태
};

struct SARENA_WATCH_START_USER_INFO
{
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT		HP;
	INT		DP;
#else
	SHORT	HP;
	SHORT	DP;
#endif
	float			CurrentHP;
	float			CurrentDP;
	BYTE			Influence;
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		// 2007-06-15 by dhjin, 유저 이름
};

struct MSG_FC_ARENA_WATCH_START
{
	BYTE			VCNDeathCount;			// 2007-06-18 by dhjin, 바이제니유 데스매치 포인트
	BYTE			ANIDeathCount;			// 2007-06-18 by dhjin, 알링턴 데스매치 포인트
	INT				Time;					// 2007-06-18 by dhjin, 대전 남은 시간 (초)
	INT				ArenaWatchStartUserInfoListCount;		// 2007-06-15 by dhjin,
	_ARRAY(SARENA_WATCH_START_USER_INFO);
};

struct MSG_FC_ARENA_WATCH_REMOVE_USER
{
	ClientIndex_t	ClientIndex;			// 2007-06-18 by dhjin, 아레나에서 나간 유저
};

// 2008-01-08 by dhjin, 아레나 통합 - 
struct MSG_FC_ARENA_POSSIBLE_GAMESTART
{// 2008-01-08 by dhjin, 아레나 케릭터 정보를 전송한다.
	BitFlag16_t		MapWeather;
	CHARACTER		AFSCharacter;
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];
	bool			IsPCBangUser;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
};

struct MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK
{// 2008-01-15 by dhjin, 아레나 경기 종료 후 필드 서버 게임 시 미리 필요한 정보를 여기서 전송한다.
	BitFlag16_t		MapWeather;
	BYTE			CharacterMode0;
	bool			bMemberPCBang;
	CHARACTER		Character;
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];
	BYTE			bIsLockSecondPW;			// 2008-08-26 by cmkwon, 베트남 VTC-Intecom_Viet 2차 패스워드 시스템 버그 수정 - 2차 패스워드 설정 정보
};

//////////////////////////////////////////////////////////////////////////
// 2007-07-06 by dhjin, Tutorial
//#define T_FC_TUTORIAL_START				(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_START)			// C->F
//#define T_FC_TUTORIAL_START_OK			(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_START_OK)		// F->C
//#define T_FC_TUTORIAL_COMPLETE			(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_COMPLETE)		// C->F
//#define T_FC_TUTORIAL_COMPLETE_OK		(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_COMPLETE_OK)		// F->C
//#define T_FC_TUTORIAL_END				(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_END)				// C->F
//#define T_FC_TUTORIAL_END_OK			(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_END_OK)			// F->C
//#define T_FC_TUTORIAL_WARP				(MessageType_t)((T0_FC_TUTORIAL<<8)|T1_FC_TUTORIAL_WARP)			// C->F

struct STUTORIAL_LIST_INFO
{// 2007-07-06 by dhjin, Tutorial 완료 정보 여부 구조체
	INT		TutorialNum;				// 2007-07-06 by dhjin, Tutorial 번호 
	BYTE 	TutorialComplete;			// 2007-07-06 by dhjin, Tutorial 완료 여부, 0 : FALSE, 1 : TRUE
};

struct MSG_FC_TUTORIAL_START_OK
{// 2007-07-06 by dhjin, Tutorial 완료 정보 메세지
	INT		TutorialInfoListCount;		// 2007-07-06 by dhjin, Tutorial 정보 리스트 수
	_ARRAY(STUTORIAL_LIST_INFO);
};

struct MSG_FC_TUTORIAL_COMPLETE
{// 2007-07-06 by dhjin, Tutorial 완료 번호
	INT		TutorialNum;
};

struct MSG_FC_TUTORIAL_COMPLETE_OK
{// 2007-07-06 by dhjin, Tutorial 완료 번호
	INT		TutorialNum;
	INT		TutorialItemNum1;
	INT		TutorialItemCount1;
	INT		TutorialItemNum2;
	INT		TutorialItemCount2;
	INT		TutorialItemNum3;
	INT		TutorialItemCount3;
};


//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, OUTPOST
//#define T_FC_OUTPOST_WAR_BEFORE				(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_BEFORE)			// F->C(N)
//#define T_FC_OUTPOST_WAR_START 				(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_START)				// F->C(N)
//#define T_FC_OUTPOST_PROTECTOR_DESTROY		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_PROTECTOR_DESTROY)		// F->C(N)
//#define T_FC_OUTPOST_RESET_CHECK_START		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_CHECK_START)		// C->F
//#define T_FC_OUTPOST_RESET_CHECK_START_OK	(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_CHECK_START_OK)	// F->C
//#define T_FC_OUTPOST_RESET_START			(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_START)			// C->F
//#define T_FC_OUTPOST_RESET_START_OK 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_START_OK)		// F->C(N)
//#define T_FC_OUTPOST_RESET_DESTROY	 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_DESTROY)			// F->C
//#define T_FC_OUTPOST_RESET_SUCCESS	 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_RESET_SUCCESS)			// F->C
//#define T_FC_OUTPOST_WAR_END		 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_END)				// F->C(N)
//#define T_FC_OUTPOST_NEXTWAR_INFO_REQUEST		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_INFO_REQUEST)			// C->F
//#define T_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK 	(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK)		// F->C(N)
//#define T_FC_OUTPOST_NEXTWAR_SET_TIME 		(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_SET_TIME)			// C->F
//#define T_FC_OUTPOST_NEXTWAR_SET_TIME_OK	(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_NEXTWAR_SET_TIME_OK)			// F->C
//#define T_FC_OUTPOST_WAR_INFO				(MessageType_t)((T0_FC_OUTPOST<<8)|T1_FC_OUTPOST_WAR_INFO)					// F->C


struct MSG_FC_OUTPOST_WAR_BEFORE
{// 2007-08-13 by dhjin, OutPost 시작 전 남은 시간 정보 전송
	BYTE	RemainTime;
	BYTE	Influence;
	INT		MapIndex;
};

struct MSG_FC_OUTPOST_WAR_START
{// 2007-08-13 by dhjin, OutPost 시작 정보 전송
	BYTE			OutPostState;
	BYTE			Influence;
	INT				MapIndex;
	ATUM_DATE_TIME	WarStartTime;
	ATUM_DATE_TIME	WarEndTime;
};

struct MSG_FC_OUTPOST_PROTECTOR_DESTROY
{// 2007-08-13 by dhjin, OutPost 보호막 파괴 전송
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_OUTPOST_RESET_START
{// 2007-08-16 by dhjin, 중앙처리장치 제어 시작
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_OUTPOST_RESET_START_OK
{// 2007-08-16 by dhjin, 중앙처리장치 제어 시작
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	ResetStartTime;
	ATUM_DATE_TIME	ResetEndTime;
};

struct MSG_FC_OUTPOST_RESET_DESTROY
{// 2007-08-13 by dhjin, 중앙처리장치 파괴됨
	INT		MapIndex;
};

struct MSG_FC_OUTPOST_RESET_SUCCESS
{// 2007-08-13 by dhjin, 중앙처리장치 제어 성공 
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_OUTPOST_WAR_END
{// 2007-08-13 by dhjin, 전진기지전 종료
	BYTE	Influence;
	INT		MapIndex;
	UID32_t	GuildUID;
	CHAR	GuildName[SIZE_MAX_GUILD_NAME];
};

struct MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK
{// 2007-08-13 by dhjin, 전진기지전 시간 설정 정보 전송
	ATUM_DATE_TIME	    OutPostNextWarTime;
	INT					OutPostNextWarSelectTimeChoice;
	INT					OutPostNextWarTimeListCount;
	_ARRAY(ATUM_DATE_TIME);
};

struct MSG_FC_OUTPOST_NEXTWAR_SET_TIME
{// 2007-08-13 by dhjin, 전진기지전 다음 시간 설정
	INT		OutPostNextWarSelectTimeChoice;
};

struct MSG_FC_OUTPOST_NEXTWAR_SET_TIME_OK
{// 2007-09-06 by dhjin, 전진기지전 다음 시간 설정 정보를 모든 유저에게 보내준다.
	MapIndex_t		MapIndex;
	ATUM_DATE_TIME	OutPostNextWarTime;
};

struct SOUTPOST_WAR_INFO
{// 2007-09-14 by dhjin, 유저 접속 시 전쟁기지 전쟁이 시작 중이면 전쟁기지 정보를 유저에게 보내준다.
	BYTE			OutPostState;
	BYTE			OutPostResetIngInfluence;			// 2007-10-04 by dhjin, 리셋 중인 세력
	MapIndex_t		MapIndex;
	INT				OutPostWarResetRamainSecondTime;	// 2007-10-04 by dhjin, 리셋 완료까지 남은 시간
	ATUM_DATE_TIME	OutPostWarStartTime;
	ATUM_DATE_TIME  OutPostWarEndTime;
	//	ATUM_DATE_TIME	OutPostWarResetStartTime;
	//	ATUM_DATE_TIME	OutPostWarResetEndTime;
};

struct MSG_FC_OUTPOST_WAR_INFO
{// 2007-09-14 by dhjin, 유저 접속 시 전쟁기지 전쟁이 시작 중이면 전쟁기지 정보를 유저에게 보내준다.
	INT					OutPostWarInfoListCount;
	_ARRAY(SOUTPOST_WAR_INFO);
};


//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, 공지사항 (주의~! FC 프로토콜이 많이 남아 있지 않아서 의미가 비슷한 T_FC_INFO에 우선 할당하여 진행)
//#define T_FC_INFO_NOTICE_REQUEST		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REQUEST)			// C->F
//#define T_FC_INFO_NOTICE_REQUEST_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REQUEST_OK)		// F->C
//#define T_FC_INFO_NOTICE_REG 			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REG)			// C->F
//#define T_FC_INFO_NOTICE_REG_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_REG_OK)		// F->C
//#define T_FC_INFO_NOTICE_MODIFY 		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_MODIFY)			// C->F
//#define T_FC_INFO_NOTICE_MODIFY_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_NOTICE_MODIFY_OK)		// F->C

struct MSG_FC_INFO_NOTICE_REQUEST
{// 2007-08-13 by dhjin, 공지사항 요청
	BYTE	NoticeState;
	INT		MapIndex;
};

struct MSG_FC_INFO_NOTICE_REQUEST_OK
{// 2007-08-13 by dhjin, 공지사항 요청 전송
	CHAR	NoticeString[SIZE_MAX_NOTICE];
};

struct MSG_FC_INFO_NOTICE_REG
{// 2007-08-13 by dhjin, 공지사항 등록
	UID32_t	GuildUID;
	CHAR	NoticeString[SIZE_MAX_NOTICE];
};

struct MSG_FC_INFO_NOTICE_MODIFY
{// 2007-08-13 by dhjin, 공지사항 수정
	UID32_t	GuildUID;
	CHAR	NoticeString[SIZE_MAX_NOTICE];
};

//////////////////////////////////////////////////////////////////////////
// 2007-08-13 by dhjin, 판공비 (주의~! FC 프로토콜이 많이 남아 있지 않아서 의미가 비슷한 T_FC_INFO에 우선 할당하여 진행)
//#define T_FC_INFO_EXPEDIENCYFUND_REQUEST 		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_REQUEST)			// C->F
//#define T_FC_INFO_EXPEDIENCYFUND_REQUEST_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_REQUEST_OK)		// F->C
//#define T_FC_INFO_EXPEDIENCYFUND_PAYBACK 		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_PAYBACK)			// C->F
//#define T_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK)		// F->C
//#define T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD)	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로토콜 추가, C->F
//#define T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD_OK)	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로토콜 추가, F->C
//#define T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD)	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로토콜 추가, C->F
//#define T_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD_OK)	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로토콜 추가, F->C
//#define T_FC_INFO_SECONDARYPASSWORD_LOCK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_LOCK)				// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로토콜 추가, C->F
//#define T_FC_INFO_SECONDARYPASSWORD_LOCK_OK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SECONDARYPASSWORD_LOCK_OK)			// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - 프로토콜 추가, F->C

struct MSG_FC_INFO_EXPEDIENCYFUND_REQUEST
{// 2007-08-13 by dhjin, 판공비 정보 요청
	INT 	MapIndex;
};

struct MSG_FC_INFO_EXPEDIENCYFUND_REQUEST_OK
{// 2007-08-13 by dhjin, 판공비 정보 전송
	INT		CumulativeExpediencyFund;
	INT		ExpediencyFund;
	INT		ExpediencyFundRate;
};

struct MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK
{// 2007-08-13 by dhjin, 판공비 신청 요청
	INT	 ExpediencyFundPayBackCount;
};

struct MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK
{// 2007-08-13 by dhjin, 판공비 신청 처리 정보 전송
	INT		CumulativeExpediencyFund;
	INT		ExpediencyFund;
	INT		ExpediencyFundPayBackCount;
	INT		ExpediencyFundRate;
};

struct MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD
{// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
	char	NewSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};
struct MSG_FC_INFO_SECONDARYPASSWORD_CHECK_PASSWORD
{// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};
struct MSG_FC_INFO_SECONDARYPASSWORD_LOCK
{// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -	
	BYTE	bIsUnlockFlag;		// FALSE이면 Lock, FALSE가 아니면 Unlock
	char	CurrentSecPassword[SIZE_MAX_PASSWORD_MD5_STRING];
};
struct MSG_FC_INFO_SECONDARYPASSWORD_LOCK_OK
{// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 -	
	BYTE	bIsUnlockFlag;		// FALSE이면 Lock, FALSE가 아니면 Unlock
};


//////////////////////////////////////////////////////////////////////////
// 2007-09-06 by dhjin, 텔레포트 클릭하여 빌딩을 시작한다.
//#define T_FC_EVENT_CLICK_TELEPORT			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_CLICK_TELEPORT)		// 2007-09-06 by dhjin, C->F
//#define T_FC_EVENT_CLICK_TELEPORT_OK		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_CLICK_TELEPORT_OK)		// 2008-04-22 by dhjin, F->C 모선전 정보 표시 기획안 - 텔레포트 빌딩 완료 시간
//#define T_FC_EVENT_TELEPORT_BUILDCOMPLETE	(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_TELEPORT_BUILDCOMPLETE)		// 2007-09-19 by dhjin, F->C(n)
//#define T_FC_EVENT_TELEPORT_DESTROY			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_TELEPORT_DESTROY)		// 2007-09-19 by dhjin, F->C(n)

struct MSG_FC_EVENT_CLICK_TELEPORT
{// 2007-09-06 by dhjin, 텔레포트 클릭
	INT 			MapIndex;
	ClientIndex_t	TargetIndex;
};

struct MSG_FC_EVENT_CLICK_TELEPORT_OK
{// 2008-04-22 by dhjin, 모선전 정보 표시 기획안 - 텔레포트 빌딩 완료 시간
	INT				MapIndex;
	ATUM_DATE_TIME 	StartTime;
	ATUM_DATE_TIME 	EndTime;
};

struct MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE
{// 2007-09-06 by dhjin, 텔레포트 완성
	MapIndex_t 		MapIndex;
};

struct MSG_FC_EVENT_TELEPORT_DESTROY
{// 2007-09-20 by cmkwon, 텔레포트 수정 - 
	MapIndex_t 		MapIndex;
	bool			bInactivateByOtherTeleport;		// 2007-09-20 by cmkwon, 텔레포트 수정 - 필드추가, FALSE이면 공격에 의한 폭파, TRUE이면 다른 Teleport 생성으로 인한 비활성화
};

///////////////////////////////////////////////////////////////////////////////
// 2007-11-28 by cmkwon, 통지시스템 구현 -
//#define T_FC_EVENT_NOTIFY_MSG_GET			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_NOTIFY_MSG_GET)	// 2007-11-28 by cmkwon, 통지시스템 구현 - C->F
//#define T_FC_EVENT_NOTIFY_MSG_GET_OK		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_NOTIFY_MSG_GET_OK)	// 2007-11-28 by cmkwon, 통지시스템 구현 - F->C
//#define T_FC_EVENT_NOTIFY_MSG_DELETE		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_NOTIFY_MSG_DELETE)	// 2007-11-28 by cmkwon, 통지시스템 구현 - C->F

struct MSG_FC_EVENT_NOTIFY_MSG_GET_OK		// 2007-11-28 by cmkwon, 통지시스템 구현 -
{
	UID64_t			NotifyMsgUID;				//	
	UID32_t			CharacterUID;				// 통지메시지 받을 캐릭터 UID, 0 이면 모든 캐릭터에게 전송(0 인것은 유저가 지울 수 없다)
	BYTE			NotifyMsgType;				// NOTIFY_MSG_TYPE_XXX
	char			NotifyMsgString[SIZE_MAX_NOTIFY_MSG_STRING];
};

struct MSG_FC_EVENT_NOTIFY_MSG_DELETE		// 2007-11-28 by cmkwon, 통지시스템 구현 -
{
	UID64_t			NotifyMsgUID;				//	
};

///////////////////////////////////////////////////////////////////////////////
// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
//#define T_FC_EVENT_COUPON_EVENT_INFO			(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_COUPON_EVENT_INFO)				// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - F->C
//#define T_FC_EVENT_COUPON_EVENT_USE_COUPON		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_COUPON_EVENT_USE_COUPON)		// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - C->F
//#define T_FC_EVENT_COUPON_EVENT_USE_COUPON_OK	(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_COUPON_EVENT_USE_COUPON_OK)	// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - F->C
struct MSG_FC_EVENT_COUPON_EVENT_INFO				// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
{
	int		ItemEventUID;
	int		ItemEventType;
};
struct MSG_FC_EVENT_COUPON_EVENT_USE_COUPON			// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
{
	int		ItemEventUID;
	char	CouponNumber[SIZE_MAX_COUPON_NUMBER];
};
struct MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK		// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
{
	int		ItemEventUID;
};



//////////////////////////////////////////////////////////////////////////
// 2007-10-29 by dhjin, Poll시스템 관련
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST)			// C->F, 지도자 후보 리스트 요청
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER	(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER)		// F->C, 지도자 후보 리스트 요청
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK)		// F->C, 지도자 후보 리스트 요청
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE	(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_DONE)		// F->C, 지도자 후보 리스트 요청
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO)			// C->F, 지도자 후보 정보 요청
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK)		// F->C, 지도자 후보 정보 요청
//#define T_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK			(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK)		// F->C, 지도자 후보 정보 요청
//#define T_FC_CITY_POLL_REG_LEADER_CANDIDATE						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REG_LEADER_CANDIDATE)					// C->F, 지도자 후보 등록
//#define T_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK)				// F->C, 지도자 후보 등록
//#define T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_DELETE_LEADER_CANDIDATE)				// C->F, 지도자 후보 탈퇴
//#define T_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK				(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_DELETE_LEADER_CANDIDATE_OK)				// F->C, 지도자 후보 탈퇴
//#define T_FC_CITY_POLL_VOTE										(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_VOTE)									// C->F, 지도자 후보에게 투표
//#define T_FC_CITY_POLL_VOTE_OK									(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_VOTE_OK)								// F->C, 지도자 후보에게 투표
//#define T_FC_CITY_POLL_REQUEST_POLL_DATE						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_POLL_DATE)						// C->F, 선거 기간 요청
//#define T_FC_CITY_POLL_REQUEST_POLL_DATE_OK						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_REQUEST_POLL_DATE_OK)					// F->C, 선거 기간 요청
//#define T_FC_CITY_POLL_LEADER_ELECTION_INFO						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_POLL_LEADER_ELECTION_INFO)					// F->C, 선거 결과 전송

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER
{// 2007-10-29 by dhjin, 지도자 후보 리스트 전송
	bool			RegLeaderCandidate;								// 지도자 후보 출마 한 정보, TURE = 출마
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK
{// 2007-10-29 by dhjin, 지도자 후보 리스트 정보
	INT				LeaderCandidateNum;							//	지도자 후보 번호
	UID32_t			CharacterUID;								//	지도자 후보 UID
	UID32_t			GuildUID;									//  지도자 후보 길드 UID
	bool			DeleteCandidate;							//  이번 선거 기간에 후보 탈퇴 했는지 여부, TRUE = 탈퇴
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  지도자 후보 이름
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  지도자 후보 길드명
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO
{// 2007-10-29 by dhjin, 지도자 후보 정보 전송
	INT				LeaderCandidateNum;							//	지도자 후보 번호
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK
{// 2007-10-29 by dhjin, 지도자 후보 정보 전송
	BYTE			PilotFace;									//  지도자 후보 얼굴
	BYTE			Level;										//	지도자 후보 Level
	INT				GuildFame;									//	지도자 후보 여단 명성
	UID32_t			AccountUID;									//	지도자 후보 계정 UID
	UID32_t			CharacterUID;								//	지도자 후보 UID
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  지도자 후보 이름
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  지도자 후보 길드명
	CHAR			CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];//  지도자 후보 공약
};

struct MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK
{// 2007-11-01 by dhjin, 지도자 후보 정보 전송 여단 마크
	INT				GuildMarkSize;								//  지도자 후보 여단 마크 크기
	UID32_t			GuildUID;									//  지도자 후보 길드 UID
	ARRAY_(char);												//	지도자 후보 여단 마크
};

struct MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE
{// 2007-10-29 by dhjin, 지도자 후보 등록
	UID32_t			CharacterUID;								//	지도자 후보 UID
	UID32_t			GuildUID;									//  지도자 후보 길드 UID
	CHAR			CampaignPromises[SIZE_MAX_CAMPAIGNPROMISES];//  지도자 후보 공약
};

struct MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK
{// 2007-10-29 by dhjin, 지도자 후보 등록
	INT				LeaderCandidateNum;							//	지도자 후보 번호
};

struct MSG_FC_CITY_POLL_DELETE_LEADER_CANDIDATE
{// 2007-10-29 by dhjin, 지도자 후보 탈퇴
	UID32_t			CharacterUID;								//	지도자 후보 UID
};

struct MSG_FC_CITY_POLL_VOTE
{// 2007-10-29 by dhjin, 투표
	INT				LeaderCandidateNum;							//	지도자 후보 번호
	UID32_t			CharacterUID;								//	지도자 후보 UID
	UID32_t			VoteCharacterUID;							//	투표자 UID
};

struct MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK
{// 2007-10-29 by dhjin, 선거 일정
	ATUM_DATE_TIME	ApplicationStartDate;		// 후보 신청 시작 날
	ATUM_DATE_TIME	ApplicationEndDate;			// 후보 시청 끝나는 날
	ATUM_DATE_TIME	VoteStartDate;				// 투표 시작 날
	ATUM_DATE_TIME	VoteEndDate;				// 투표 마지막 날
	ATUM_DATE_TIME	Election;					// 선출일
};

struct MSG_FC_CITY_POLL_LEADER_ELECTION_INFO
{// 2007-11-16 by dhjin, 선거 결과
	INT				LeaderCandidateNum;							//	지도자 후보 번호
	CHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];		//  지도자 후보 이름
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];				//  지도자 후보 길드명
	INT				Percent;									//  지도자 득표율
};

//////////////////////////////////////////////////////////////////////////
// 2007-12-03 by dhjin, 전장 정보 
//#define T_FC_CITY_WARINFO_INFLUENCE						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_INFLUENCE)		// C->F, 세력 정보
//#define T_FC_CITY_WARINFO_INFLUENCE_OK					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_INFLUENCE_OK)	// F->C, 세력 정보
//#define T_FC_CITY_WARINFO_OUTPOST						(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_OUTPOST)			// C->F, 전진기지 정보 
//#define T_FC_CITY_WARINFO_OUTPOST_OK					(MessageType_t)((T0_FC_CITY<<8)|T1_FC_CITY_WARINFO_OUTPOST_OK)		// F->C, 전진기지 정보 

struct MSG_FC_CITY_WARINFO_INFLUENCE_OK
{// 2007-12-03 by dhjin, 세력 정보 
	INT			VCNInfluencePoint;
	INT			ANIInfluencePoint;
	CHAR		VCNInfluenceLeader[SIZE_MAX_CHARACTER_NAME];
	CHAR		VCNInfluenceSubLeader1[SIZE_MAX_CHARACTER_NAME];
	CHAR		VCNInfluenceSubLeader2[SIZE_MAX_CHARACTER_NAME];
	CHAR		ANIInfluenceLeader[SIZE_MAX_CHARACTER_NAME];
	CHAR		ANIInfluenceSubLeader1[SIZE_MAX_CHARACTER_NAME];
	CHAR		ANIInfluenceSubLeader2[SIZE_MAX_CHARACTER_NAME];
};

struct SCITY_WARINFO_OUTPOST
{// 2007-12-03 by dhjin, 전진기지 정보 
	BYTE			Influence;
	MapIndex_t		MapIndex;
	UID32_t			GuildUID;
	CHAR			GuildCommander[SIZE_MAX_CHARACTER_NAME];
	CHAR			GuildName[SIZE_MAX_GUILD_NAME];
	ATUM_DATE_TIME	OutpostDate;
};

struct MSG_FC_CITY_WARINFO_OUTPOST_OK
{// 2007-12-03 by dhjin, 전진기지전 정보
	INT					CityWarInfoOutpostListCount;
	_ARRAY(SCITY_WARINFO_OUTPOST);
};

//#define T_FC_INFO_GET_GUILDMARK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_GET_GUILDMARK)			// 2007-12-07 by dhjin, 여단 마크 C->F
//#define T_FC_INFO_GET_GUILDMARK_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_GET_GUILDMARK_OK)		// 2007-12-07 by dhjin, 여단 마크 F->C
struct MSG_FC_INFO_GET_GUILDMARK
{// 2007-12-07 by dhjin
	UID32_t			GuildUID;
};

struct MSG_FC_INFO_GET_GUILDMARK_OK
{// 2007-12-07 by dhjin, 여단 마크 전송
	INT				GuildMarkSize;
	INT				GuildMarkVersion;
	UID32_t			GuildUID;
	ARRAY_(char);
};

// 2007-12-27 by dhjin, 아레나 통합 - 아레나 프로토콜 MainField <=> ArenaField 
//#define T_FtoA_MFSINFO						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_MFSINFO)			// 2007-12-27 by dhjin, 아레나 서버에 필드 서버 등록 및 필드 서버 정보 전송, MF->AF
//#define T_FtoA_MFSINFO_OK					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_MFSINFO_OK)		// 2007-12-27 by dhjin, 아레나 서버에 필드 서버 등록 및 필드 서버 정보 전송, AF->MF
//#define T_FtoA_ALIVE						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ALIVE)				// 2007-12-27 by dhjin,
//#define T_FtoA_AUTH_CHARACTER				(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_AUTH_CHARACTER)	// 2007-12-27 by dhjin, 아레나 서버에서 유효한 유저인지 필드 서버로 확인 요청, AF->MF
//#define T_FtoA_AUTH_CHARACTER_OK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_AUTH_CHARACTER_OK)	// 2007-12-27 by dhjin, 아레나 서버에서 유효한 유저인지 필드 서버로 확인 요청, MF->AF
//#define T_FtoA_ARENA_STATE_CHANGE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_STATE_CHANGE)	// 2008-01-03 by dhjin, 필드 서버에 유저 아레나 상태값 변경, AF->MF
//#define T_FtoA_ARENA_TEAM_MATCHING			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_TEAM_MATCHING)	// 2008-01-04 by dhjin, 필드 서버로 매칭 정보를 전송한다, AF->MF
//#define T_FtoA_ARENA_TEAM_MATCHING_OK		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_TEAM_MATCHING_OK)	// 2008-01-04 by dhjin, 필드 서버로 매칭 정보를 전송한다, MF->AF
//#define T_FtoA_ARENA_SERVER_PAY				(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_SERVER_PAY)		// 2008-01-10 by dhjin, 필드 서버로 서버 아레나 보상 전송, AF->MF
//#define T_FtoA_ARENA_CHARACTER_PAY			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTER_PAY)	// 2008-01-10 by dhjin, 필드 서버로 케릭터 아레나 보상 전송, AF->MF
//#define T_FtoA_ARENA_CHARACTER_DISCONNECT	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTER_DISCONNECT)	// 2008-01-15 by dhjin, 필드 서버로 케릭터 아레나 보상 전송, AF->MF
//#define T_FtoA_ARENA_CHARACTERARENAINFO_UPDATA	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTERARENAINFO_UPDATA)	// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AFS -> MFS CharacterArena UUpdata 요청 패킷
//#define T_FtoA_ARENA_CHARACTER_RESET_RELOAD	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_ARENA_CHARACTER_RESET_RELOAD)	// 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.
struct MSG_FtoA_MFSINFO
{
	USHORT		MFS_Port;
	CHAR		MFS_Name[SIZE_MAX_SERVER_NAME];
	CHAR		MFS_IP[SIZE_MAX_IPADDRESS];
};

struct MSG_FtoA_AUTH_CHARACTER
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_AUTH_CHARACTER_OK
{
	bool			ErrorCheck;		// TRUE -> Error
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	CHAR			MFSCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	MFSClientIndex;
	UID32_t			AFSCharacterUID;
	CHAR			AFSCharacterName[SIZE_MAX_CHARACTER_NAME];
	ClientIndex_t	AFSClientIndex;
	MessageType_t	ErrorMsgType;			// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	Err_t			ErrorCode;				// 2010-04-05 by cmkwon, 인피 재입장 카드 관련 시스템 수정 - 
	bool			ShutDownMins; // 2011-11-03 by shcho, yedang 셧다운제 구현 - 아레나서버 셧다운제 인증용
	SArenaPlayInfo	ArenaPlayInfo;			// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : 아레나 플레이 정보를 저장할 구조체 변수.
};

struct MSG_FtoA_ARENA_STATE_CHANGE
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	BYTE			AFSArenaState;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_ARENA_TEAM_MATCHING
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID32_t			AFSCharacterUID;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_ARENA_TEAM_MATCHING_OK
{
	ClientIndex_t	AFSClientIndex;
	UID32_t			AccountUID;
	UID32_t			AFSCharacterUID;
};

struct MSG_FtoA_ARENA_SERVER_PAY
{
	BYTE			Influence;
	INT				PayInfluencePoint;
};

struct MSG_FtoA_ARENA_CHARACTER_PAY
{
	BYTE			Issue;					// 2007-04-20 by dhjin, 아레나 승패 여부, 1 : ARENA_ISSUE_WIN, 2 : ARENA_ISSUE_LOSE, 3 : ARENA_ISSUE_DRAW
	INT				PayWarPoint;
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_ARENA_CHARACTER_DISCONNECT
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	UID32_t			ArenaCharacterUID;	// 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
};

// 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.
struct MSG_FtoA_ARENA_CHARACTER_RESET_RELOAD
{
	UID32_t			AccountUID;
	UID32_t			ArenaCharacterUID;
};
// end 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.

//////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : MF CharacterArena 업데이트 패킷
struct MSG_FtoA_ARENA_CHARACTERARENAINFO_UPDATA
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	SArenaPlayInfo	ArenaInfo;
};
// end 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : MF CharacterArena 업데이트 패킷

//////////////////////////////////////////////////////////////////////////
// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
//#define T_FC_INFO_MSWARINFO_DISPLAY					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_DISPLAY)					// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 전송 F->C
//#define T_FC_INFO_MSWARINFO_DISPLAY_OPTION			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_DISPLAY_OPTION)			// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션 전송 C->F
//#define T_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK)			// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션 전송 F->C(n)
//#define T_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST)		// 2015-05-01 by killburne
//#define T_FC_INFO_STRATEGICPOINTINFO_DISPLAY		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_STRATEGICPOINTINFO_DISPLAY)			// 2015-05-02 by killburne
struct SMSWARINFO_DISPLAY
{// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 유저에게 필요한 정보 표시 데이타
	MapIndex_t		MapIndex;
	INT				MapInfluenceType;
	INT				HPRate;
	INT				TelePortState;
	ATUM_DATE_TIME	TelePortBuildingStartTime;
	ATUM_DATE_TIME	TelePortBuildingEndTime;
};

struct MSG_FC_INFO_MSWARINFO_DISPLAY
{// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 유저에게 필요한 정보 표시 데이타
	INT				MSWarInfoDisPlayListCount;
	_ARRAY(SMSWARINFO_DISPLAY);
};

struct MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION
{// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션
	SHORT			MSWarOptionType;
	UID32_t			CharacterUID;
	ClientIndex_t	ClientIndex;
};

struct MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK
{// 2008-03-26 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 옵션
	SHORT			MSWarOptionType;
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 
//#define T_FC_INFO_MSWARINFO_RESULT				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_RESULT)	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 결과 정보 C->F
//#define T_FC_INFO_MSWARINFO_RESULT_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_MSWARINFO_RESULT_OK)	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 결과 정보 F->C
//#define T_FC_INFO_SPWARINFO_RESULT				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT)	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 결과 정보 C->F
//#define T_FC_INFO_SPWARINFO_RESULT_OK_HEADER	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT_OK_HEADER)	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 결과 정보 F->C
//#define T_FC_INFO_SPWARINFO_RESULT_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT_OK)	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 결과 정보 F->C
//#define T_FC_INFO_SPWARINFO_RESULT_OK_DONE		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_SPWARINFO_RESULT_OK_DONE)	// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 결과 정보 F->C
struct MSG_FC_INFO_MSWARINFO_RESULT_OK
{// 2008-04-02 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 결과 정보 F->C
	INT				MSWarInfoResultListCount;
	_ARRAY(SMSWAR_INFO_RESULT);
};

//////////////////////////////////////////////////////////////////////////
// 2008-04-29 by dhjin, EP3 편지 시스템 - 
//#define T_FC_CHAT_LETTER_REQUEST_TITLE			(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE)			// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 요청 C->F
//#define T_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER (MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE_OK_HEADER)	// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송 F->C
//#define T_FC_CHAT_LETTER_REQUEST_TITLE_OK		(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE_OK)			// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송 F->C
//#define T_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE	(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_REQUEST_TITLE_OK_DONE)	// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송 F->C
//#define T_FC_CHAT_LETTER_READ					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_READ)						// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 읽기 요청 C->F
//#define T_FC_CHAT_LETTER_READ_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_READ_OK)					// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 읽기 완료 F->C
//#define T_FC_CHAT_LETTER_DELETE					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_DELETE)					// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 지우기 요청 C->F
//#define T_FC_CHAT_LETTER_DELETE_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_DELETE_OK)				// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 지우기 완료 F->C
//#define T_FC_CHAT_LETTER_SEND					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_SEND)						// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 C->F
//#define T_FC_CHAT_LETTER_SEND_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_SEND_OK)					// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 완료 F->C 
//#define T_FC_CHAT_LETTER_RECEIVE				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_LETTER_RECEIVE)					// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 받음 F->C
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE			(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE)				// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 요청 C->F
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER (MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_HEADER)	// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 전송 F->C
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK		(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK)			// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 전송 F->C
//#define T_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE	(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK_DONE)		// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 리스트 전송 F->C
//#define T_FC_CHAT_ALLLETTER_READ					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_READ)						// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기 요청 C->F
//#define T_FC_CHAT_ALLLETTER_READ_OK					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_READ_OK)					// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기 완료 F->C
//#define T_FC_CHAT_ALLLETTER_DELETE					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_DELETE)					// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 지우기 요청 C->F
//#define T_FC_CHAT_ALLLETTER_DELETE_OK				(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_DELETE_OK)					// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 지우기 완료 F->C
//#define T_FC_CHAT_ALLLETTER_SEND					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_SEND)						// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 전송 C->F
//#define T_FC_CHAT_ALLLETTER_SEND_OK					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_SEND_OK)					// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 전송 완료 F->C 
//#define T_FC_CHAT_ALLLETTER_RECEIVE					(MessageType_t)((T0_FC_CHAT<<8)|T1_FC_CHAT_ALLLETTER_RECEIVE)					// 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 전송 받음 F->C

struct MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK
{// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 리스트 전송
	UID64_t			LetterUID;
	CHAR			SendCharacterName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME  SendDate;
	bool			CheckRead;
	CHAR			Title[SIZE_MAX_LETTER_TITLE];
};
using MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK = MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK;

struct MSG_FC_CHAT_LETTER_READ
{// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 내용 요청
	UID64_t			LetterUID;
	bool			CheckRead;
};
using MSG_FC_CHAT_ALLLETTER_READ = MSG_FC_CHAT_LETTER_READ;

struct MSG_FC_CHAT_LETTER_READ_OK
{// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 내용 전송
	UID64_t			LetterUID;
	CHAR			Content[SIZE_MAX_LETTER_CONTENT];
};
using MSG_FC_CHAT_ALLLETTER_READ_OK = MSG_FC_CHAT_LETTER_READ_OK;

struct MSG_FC_CHAT_LETTER_DELETE
{// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 지우기 
	UID64_t			LetterUID;
};
using MSG_FC_CHAT_ALLLETTER_DELETE = MSG_FC_CHAT_LETTER_DELETE;

struct MSG_FC_CHAT_LETTER_DELETE_OK
{// 2008-04-29 by dhjin, EP3 편지 시스템 - 편지 지우기 완료
	UID64_t			LetterUID;
};
using MSG_FC_CHAT_ALLLETTER_DELETE_OK = MSG_FC_CHAT_LETTER_DELETE_OK;

struct MSG_FC_CHAT_LETTER_SEND
{// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 보내기
	INT				SendCharacterUID;
	CHAR			RecvCharacterName[SIZE_MAX_CHARACTER_NAME];
	CHAR			Title[SIZE_MAX_LETTER_TITLE];
	CHAR			Content[SIZE_MAX_LETTER_CONTENT];
};
using MSG_FC_CHAT_ALLLETTER_SEND = MSG_FC_CHAT_LETTER_SEND;

struct MSG_FC_CHAT_LETTER_RECEIVE
{// 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 전송 받음
	CHAR				SendCharacterName[SIZE_MAX_CHARACTER_NAME];
};
using MSG_FC_CHAT_ALLLETTER_RECEIVE = MSG_FC_CHAT_LETTER_RECEIVE;

//////////////////////////////////////////////////////////////////////////
// 2008-08-18 by dhjin, 세력마크이벤트 
//#define T_FC_EVENT_INFLUENCEMARK		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_INFLUENCEMARK)		
//#define T_FC_EVENT_INFLUENCEMARKEND		(MessageType_t)((T0_FC_EVENT<<8)|T1_FC_EVENT_INFLUENCEMARKEND)
struct MSG_FC_EVENT_INFLUENCEMARK
{
	BYTE			Influence;		// 세력마크이벤트세력
};

//////////////////////////////////////////////////////////////////////////
// 2008-12-02 by dhjin, 미션마스터
//#define T_FC_QUEST_REQUEST_MISSIONMASTER_HELP			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_REQUEST_MISSIONMASTER_HELP)		// 2008-12-02 by dhjin, C -> F, 미션마스터 요청
//#define T_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE	(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE)	// 2008-12-02 by dhjin, F -> C(n), 랜덤으로 뽑힌 미션마스터에게 요청
//#define T_FC_QUEST_MISSIONMASTER_HELP_INVITE			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_INVITE)			// 2008-12-02 by dhjin, C -> F, 미션마스터 요청 승락 
//#define T_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK)		// 2008-12-02 by dhjin, F -> C, 미션마스터 요청 승락
//#define T_FC_QUEST_MISSIONMASTER_HELP_REJECT			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_REJECT)			// 2008-12-02 by dhjin, C -> F, 미션마스터 요청 거절 
//#define T_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK)		// 2008-12-02 by dhjin, F -> C, 미션마스터 요청 거절
struct MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP
{
	INT				QuestIdx;
};

struct MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE
{
	ClientIndex_t	ClientIdx;
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, 미션마스터 요청한 케릭터
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
	PartyID_t		PartyID;
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE
{
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, 미션마스터 요청한 케릭터
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK
{
	ClientIndex_t	ClientIdx;
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, 미션마스터 케릭터
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT
{
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, 미션마스터 요청한 케릭터
};

struct MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK
{
	ClientIndex_t	ClientIdx;
	UID32_t			CharacterUID;		// 2008-12-02 by dhjin, 미션마스터 케릭터
	char			CharacterName[SIZE_MAX_CHARACTER_NAME];
};

//////////////////////////////////////////////////////////////////////////
// 2009-01-12 by dhjin, 선전 포고 -
//#define T_FC_INFO_DECLARATION_MSWAR_INFO		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_DECLARATION_MSWAR_INFO)		// 2009-01-12 by dhjin, 선전 포고 - 선전포고 정보 요청 C->F
//#define T_FC_INFO_DECLARATION_MSWAR_INFO_OK		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_DECLARATION_MSWAR_INFO_OK)	// 2009-01-12 by dhjin, 선전 포고 - 선전포고 정보 전송 F->C
//#define T_FC_INFO_DECLARATION_MSWAR_SET			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_DECLARATION_MSWAR_SET)	// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 C->F
//#define T_FI_INFO_DECLARATION_MSWAR_SET_OK		(MessageType_t)((T0_FI_INFO<<8)|T1_FI_INFO_DECLARATION_MSWAR_SET_OK)	// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 F->I
//#define T_IC_INFO_DECLARATION_MSWAR_SET_OK		(MessageType_t)((T0_IC_INFO<<8)|T1_IC_INFO_DECLARATION_MSWAR_SET_OK)	// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 I->C
struct MSG_FC_INFO_DECLARATION_MSWAR_INFO_OK
{// 2009-01-12 by dhjin, 선전 포고 - 선전포고 정보 전송 F->C
	INT								DeclarationOfWarListCount;
	SDECLARATION_OF_WAR_FORBID_TIME	ForbidTime;
	_ARRAY(SDECLARATION_OF_WAR);
};

struct MSG_FC_INFO_DECLARATION_MSWAR_SET
{// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 C->F
	BYTE			MSWarStep;
	ATUM_DATE_TIME	MSWarStartTime;
	bool			GiveUp;
};

struct MSG_FI_INFO_DECLARATION_MSWAR_SET_OK
{// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 F->C
	BYTE			Influence;		// 선전 포고한 지도자 세력.
	ATUM_DATE_TIME	MSWarStartTime;
	bool			GiveUp;
	BYTE			SelectCount;
};
using MSG_IC_INFO_DECLARATION_MSWAR_SET_OK = MSG_FI_INFO_DECLARATION_MSWAR_SET_OK;


///////////////////////////////////////////////////////////////////////////////
// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
//#define T_FC_INFO_WRK_GET_SERVICE_INFO					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO)			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - C->F
//#define T_FC_INFO_WRK_GET_SERVICE_INFO_OK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO_OK)			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
//#define T_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE)	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
//#define T_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SERVICE_INFO_OK_DONE)	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
//#define T_FC_INFO_WRK_GET_RANKER_LIST					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_RANKER_LIST)				// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - C->F
//#define T_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK)	// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
//#define T_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK)		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
//#define T_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK			(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK)		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
//#define T_FC_INFO_WRK_GET_SELF_RANKING					(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SELF_RANKING)			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - C->F
//#define T_FC_INFO_WRK_GET_SELF_RANKING_OK				(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_WRK_GET_SELF_RANKING_OK)			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - F->C
struct MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK
{
	//INT		ServiceUID;
	char	ServiceName[SIZE_MAX_SERVICE_NAME];
	INT		SymbolImageSize;
};
struct MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE
{
	//INT		ServiceUID;
	BYTE	SymbolImageIndex;
	BYTE	SymbolImage[1000];
};
struct MSG_FC_INFO_WRK_GET_RANKER_LIST
{
	BYTE	byRankingType;			// RANKING_TYPE_XXX
	INT		UnitKind;
	BYTE	byScope;				// RANKING_SCOPE_XXX
	BYTE	byStartRank;
	BYTE	byReqRankerCnt;
};
struct MSG_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK
{
	BYTE	byStartRank;
	BYTE	byRankerCnt;
	_ARRAY(SWRK_LEVEL);
};
struct MSG_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK
{
	BYTE	byStartRank;
	BYTE	byRankerCnt;
	_ARRAY(SWRK_FAME);

};
struct MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK
{
	BYTE	byStartRank;
	BYTE	byRankerCnt;
	_ARRAY(SWRK_PVP);
};
struct MSG_FC_INFO_WRK_GET_SELF_RANKING_OK
{
	UID32_t	CharacUID;
	INT		LevelRanking;
	INT		FameRanking;
	INT		PvPRanking;
};



///////////////////////////////////////////////////////////////////////////////
// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
//#define T_FC_QUEST_INSERT_QUEST			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_INSERT_QUEST)	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - F->C
//#define T_FC_QUEST_DELETE_QUEST			(MessageType_t)((T0_FC_QUEST<<8)|T1_FC_QUEST_DELETE_QUEST)	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - F->C

struct MSG_FC_QUEST_INSERT_QUEST
{
	INT		QuestIndex;
	BYTE	QuestState;				// 완료, 진행중
};
struct MSG_FC_QUEST_DELETE_QUEST
{
	INT		QuestIndex;
};

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
//#define T_FN_MONSTER_HPTALK							(MessageType_t)((T0_FN_MONSTER<<8)|T1_FN_MONSTER_HPTALK)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - , N->F
struct MSG_FN_MONSTER_HPTALK {
	MAP_CHANNEL_INDEX				ChannelIndex;
	ClientIndex_t					MonsterIndex;				// 인덱스
	INT								MonsterUnitKind;			// 몬스터 고유번호
	HPValueRate_t					HPValueRate;
	eHPACTION_TALK_IMPORTANCE		HPTalkImportance;
	MSec_t							HPCameraTremble;
	ClientIndex_t					TargetIndex;				// 대상에 관련된 대사가 있다면 설정한다.	
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};

//#define T_FC_MONSTER_HPTALK							(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_HPTALK)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - , F->C(n)
struct MSG_FC_MONSTER_HPTALK {
	ClientIndex_t					MonsterIndex;				// 인덱스
	INT								MonsterUnitKind;			// 몬스터 고유번호
	eHPACTION_TALK_IMPORTANCE		HPTalkImportance;
	MSec_t							HPCameraTremble;
	ClientIndex_t					TargetIndex;				// 대상에 관련된 대사가 있다면 설정한다.
	HPTalk_t						HPTalk[SIZE_MAX_HPTALK_DESCRIPTION];
};

//#define T_FN_BATTLE_ATTACK_SKILL					(MessageType_t)((T0_FN_BATTLE<<8)|T1_FN_BATTLE_ATTACK_SKILL)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - N -> F
struct MSG_FN_BATTLE_ATTACK_SKILL
{
	MAP_CHANNEL_INDEX	MapInfo;
	ClientIndex_t		MonsterIndex;				// 스킬을 사용한 몬스터
	ClientIndex_t		ClientIndex;				// 클라이언트 인덱스
	INT					SkillItemNum;				// 스킬의 ItemNum
	AVECTOR3			PositionVector;
	AVECTOR3			TargetVector;
	float				fIncreasePowerRatio;		// 스킬의 파워(데미지 or 버프/디버프 관련 효과) 증가율.		// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
};

//#define T_FC_MONSTER_SKILL							(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_SKILL)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - , F->C(n)
struct MSG_FC_MONSTER_SKILL {
	ClientIndex_t		MonsterIndex;				// 스킬을 사용한 몬스터
	ClientIndex_t		ClientIndex;				// 클라이언트 인덱스
	INT					SkillItemNum;				// 스킬의 ItemNum
	AVECTOR3			PositionVector;
	AVECTOR3			TargetVector;
};

//#define T_FC_BATTLE_MONSTER_BARRIER_USING		(MessageType_t)((T0_FC_BATTLE<<8)|T1_FC_BATTLE_MONSTER_BARRIER_USING)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
struct MSG_FC_BATTLE_MONSTER_BARRIER_USING {
	ClientIndex_t		MonsterIndex;				// 스킬을 사용한 몬스터
};

//#define T_FC_BATTLE_MONSTER_BARRIER_USE			(MessageType_t)((T0_FC_BATTLE<<8)|T1_FC_BATTLE_MONSTER_BARRIER_USE)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C(n)
struct MSG_FC_BATTLE_MONSTER_BARRIER_USE {
	ClientIndex_t		MonsterIndex;				// 스킬을 사용한 몬스터
	INT					SkillItemNum;
};

//#define T_FC_CHARACTER_DEBUFF_DOT_INFO			(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_INFO)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - C -> F
//#define T_FC_CHARACTER_DEBUFF_DOT_INFO_OK		(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_INFO_OK)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
//#define T_FC_CHARACTER_DEBUFF_DOT_APPLYING		(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_APPLYING)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
//#define T_FC_CHARACTER_DEBUFF_DOT_RELEASE		(MessageType_t)((T0_FC_CHARACTER<<8)|T1_FC_CHARACTER_DEBUFF_DOT_RELEASE)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - F -> C
struct MSG_FC_CHARACTER_DEBUFF_DOT_INFO {
	UID32_t			CharacterUID;
};

struct MSG_DEBUFF_INFO {
	INT					ItemNum;						// 아이템 고유번호, 장착 아이템일 때 (ITEM_BASE*)
};

struct MSG_DOT_INFO {
	INT					ItemNum;						// 아이템 고유번호, 장착 아이템일 때 (ITEM_BASE*)
};

struct MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK {
	UID32_t				CharacterUID;
	INT					DebuffCount;
	INT					DotCount;
	_ARRAY(MSG_DEBUFF_INFO);
	_ARRAY(MSG_DOT_INFO);
};
struct MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING {
	UID32_t				CharacterUID;
	INT					SkillItemNum;
};

struct MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE {
	UID32_t				CharacterUID;
	INT					SkillItemNum;
};


//#define T_FC_INFINITY_MODE_LIST					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MODE_LIST)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, C -> F
//#define T_FC_INFINITY_MODE_LIST_OK				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MODE_LIST_OK)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, F -> C
struct INFINITY_MODE_INFO {
	MapIndex_t		InfinityMapIdx;
	InfiModeUID_t	InfinityModeUID;
	eINFINITY_MODE	InfinityMode;
	Lv_t			MinLv;
	Lv_t			MaxLv;
	bool			Join;
	Minute_t		TimePenaltyValue;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가
	INT				MinMemberCount;			// 2009-09-09 ~ 2010-01-22 by dhjin, 인피니티 - 최소,최대인원 정보 추가
	INT				MaxMemberCount;			// 2009-09-09 ~ 2010-01-22 by dhjin, 인피니티 - 최소,최대인원 정보 추가
	INT				InfinityDifficultyStep; // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 설정된 난이도 정보 추가.
	Penalty_t		HPPenaltyValue;			// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
};
struct MSG_FC_INFINITY_MODE_LIST_OK {
	INT				InfinityModeListCount;
	_ARRAY(INFINITY_MODE_INFO);
};

//#define T_FC_INFINITY_READY_LIST					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_LIST)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, C -> F
//#define T_FC_INFINITY_READY_LIST_OK					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_LIST_OK)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 리스트 요청, F -> C
struct MSG_FC_INFINITY_READY_LIST {
	MapIndex_t		InfinityMapIdx;
	eINFINITY_MODE	InfinityMode;
};

struct INFINITY_READY_LIST {
	InfinityCreateUID_t	InfinityCreateUID;
	CHAR				MasterName[SIZE_MAX_CHARACTER_NAME];
	CHAR				InfinityTeamName[SIZE_MAX_PARTY_NAME];
	INT					PlayingRoomMemberCount;
	INT					MaxMemberCount;
	INT					DifficultLevel;
};
struct MSG_FC_INFINITY_READY_LIST_OK {
	INT				InfinityPlayingListCount;
	_ARRAY(INFINITY_READY_LIST);
};

//#define T_FC_INFINITY_CREATE						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CREATE)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 생성 요청, C -> F
//#define T_FC_INFINITY_CREATE_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CREATE_OK)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 생성 요청, F -> C
struct MSG_FC_INFINITY_CREATE {
	InfiModeUID_t	InfinityModeUID;
	eINFINITY_MODE	InfinityMode;
	MapIndex_t		MapIndex;
	CHAR			InfinityTeamName[SIZE_MAX_PARTY_NAME];
	INT				InfinityDifficultyLevel;
};
struct MSG_FC_INFINITY_CREATE_OK {
	InfinityCreateUID_t	InfinityCreateUID;

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	INT					IntinityDifficultyLevel;
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

};

//#define T_FC_INFINITY_JOIN							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN)						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 가입 요청, C -> F
//#define T_FC_INFINITY_JOIN_REQUEST_MASTERUSER		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_REQUEST_MASTERUSER)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 방장에거 가입자 승인 여부 요청, F -> C
//#define T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 방장에거 가입자 승인 여부 요청, C -> F
//#define T_FC_INFINITY_JOIN_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_OK)						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 가입 요청, F -> C
//#define T_FC_INFINITY_MEMBER_INFO_LIST				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MEMBER_INFO_LIST)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맴버 정보, C -> F
//#define T_FC_INFINITY_MEMBER_INFO_LIST_OK			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MEMBER_INFO_LIST_OK)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맴버 정보, F -> C
struct MSG_FC_INFINITY_JOIN {
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
	InfiModeUID_t		InfinityModeUID;		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
};
struct MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER {
	ClientIndex_t		ReQuestClientIdx;
	CHAR				ReQuestCharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				Gear;
	BYTE				Lv;
};
struct MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK {
	ClientIndex_t		ReQuestClientIdx;
	bool				bAccept;
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};
struct MSG_FC_INFINITY_JOIN_OK {
	ClientIndex_t		JoinClientIdx;
	CHAR				JoinCharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				Gear;
	BYTE				Lv;
	bool				bAccept;
};
struct INFINITY_MEMBER_INFO_LIST {
	ClientIndex_t		ClientIdx;
	CHAR				CharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT				Gear;
	BYTE				Lv;
	eINFINITY_STATE		State;
};
struct MSG_FC_INFINITY_MEMBER_INFO_LIST_OK {
	ClientIndex_t		MasterClientIdx;
	INT					InfinityMemberListCount;
	_ARRAY(INFINITY_MEMBER_INFO_LIST);
};

//#define T_FC_INFINITY_CHANGE_MASTER					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_MASTER)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 마스터 변경, C -> F
//#define T_FC_INFINITY_CHANGE_MASTER_OK				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_MASTER_OK)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 마스터 변경, F -> C
//#define T_FC_INFINITY_LEAVE							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_LEAVE)					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 탈퇴, C -> F
//#define T_FC_INFINITY_LEAVE_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_LEAVE_OK)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 탈퇴, F -> C
//#define T_FC_INFINITY_BAN							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_BAN)						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 추방, C -> F
//#define T_FC_INFINITY_BAN_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_BAN_OK)					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 추방, F -> C
struct MSG_FC_INFINITY_CHANGE_MASTER {
	ClientIndex_t		ChangeMasterClientIdx;
};
struct MSG_FC_INFINITY_CHANGE_MASTER_OK {
	ClientIndex_t		ChangeMasterClientIdx;
};
struct MSG_FC_INFINITY_LEAVE_OK {
	ClientIndex_t		LeaveClientIdx;
};
struct MSG_FC_INFINITY_BAN {
	ClientIndex_t		BanClientIdx;
};
struct MSG_FC_INFINITY_BAN_OK {
	ClientIndex_t		BanClientIdx;
};

//#define T_FtoA_INFINITY_IMPUTE_LIST					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_IMPUTE_LIST)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 귀속 정보 전송, MFS -> AFS
struct MSG_FtoA_INFINITY_IMPUTE_LIST {
	ClientIndex_t	AFSClientIndex;
	INT				InfinityImputeListCount;
	bool			bHaveReentryTicket;			// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
	_ARRAY(INFINITY_IMPUTE);
};

//#define T_FC_INFINITY_READY							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디 버튼 누름, C -> F
//#define T_FC_INFINITY_READY_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_OK)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디 버튼 누름, F -> C
//#define T_FC_INFINITY_READY_CANCEL					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_CANCEL)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디취소 버튼 누름, C -> F
//#define T_FC_INFINITY_READY_CANCEL_OK				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_CANCEL_OK)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 래디취소 버튼 누름, F -> C
//#define T_FC_INFINITY_READY_CANCEL_ALL_OK			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_CANCEL_ALL_OK)	// // 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 인피니티 - 난이도 변경시 모든 래디 취소. ( AFS -> C )
struct MSG_FC_INFINITY_READY_OK {
	ClientIndex_t		ReadyClientIdx;
};
struct MSG_FC_INFINITY_READY_CANCEL_OK {
	ClientIndex_t		ReadyCancelClientIdx;
};

// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.) - 인피니티 방 난이도 변경시 모두 래디 취소 패킷.
struct MSG_FC_INFINITY_READY_CANCEL_ALL_OK
{
	InfinityCreateUID_t	InfinityCreateUID;
};
// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)

//#define T_FC_INFINITY_START							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_START)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 버튼 누름(인피 시작), C -> F
//#define T_FtoA_INFINITY_START						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 MainFieldServer로 알림, AFS -> MFS
//#define T_FtoA_INFINITY_START_OK					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START_OK)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 시작 MainFieldSever준비 완료 전송, MFS -> AFS
//#define T_FC_INFINITY_MAP_LOADING					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MAP_LOADING)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맵 로딩 상태임을 알림, F -> C  <-- 주의!!!! 삭제 대기중!
//#define T_FC_INFINITY_MAP_LOADED					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MAP_LOADED)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맵 로딩 완료, C -> F
//#define T_FtoA_INFINITY_IMPUTE						(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_IMPUTE)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 귀속 정보 전송, AFS -> MFS
//#define T_FC_INFINITY_ENTER							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_ENTER)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 맵 입장 허가, F -> C
struct MSG_FtoA_INFINITY_START {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID32_t			AFSCharacterUID;
	ClientIndex_t	AFSClientIndex;
};
struct MSG_FtoA_INFINITY_START_OK {
	ClientIndex_t	AFSClientIndex;
	bool			bIsPCBangClient;
};
struct MSG_FC_INFINITY_ENTER {
	ChannelIndex_t	InfinityChannelIdx;
	D3DXVECTOR3		CharacterPosition;
	ATUM_DATE_TIME	StartTime;
	MSec_t			LimitTime;
};
struct MSG_FtoA_INFINITY_IMPUTE {
	InfiModeUID_t	InfinityModeUID;
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	bool			CheckExistImpute;
};


//#define T_FC_INFINITY_CINEMA						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CINEMA)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 시네마 정보 전송, F -> C
using MSG_FC_INFINITY_CINEMA = CINEMAINFO;

struct MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE		// 2010-03-31 by dhjin, 인피니티(기지방어) -
{
	MAP_CHANNEL_INDEX		mapChann;
	INT						MonsterUnitKind;
	SummonCnt_t				MonsterSummonCount;			// 2010-03-31 by dhjin, 인피니티(기지방어) -	
	Distance_t				MaxRandomDistance;			// 2010-03-31 by dhjin, 인피니티(기지방어) -
	MONSTER_BALANCE_DATA	MonsterBalanceInfo;			// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 밸런스 정보(확장 정보 대입).
};

// start 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제/변경 기능 추가
struct MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY
{
	MAP_CHANNEL_INDEX		mapChann;
	INT						MonsterUnitKind;
};

struct MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE
{
	MAP_CHANNEL_INDEX		mapChann;					// 채널 맵 번호
	INT						MonsterUnitKind;			// 적용할 몬스터 번호
	INT						ChangeMonsterUnitKind;		// 변경될 몬스터 번호
};
// end 2011-04-28 by hskim, 인피니티 3차 - 시네마 관련 기능 추가 - 해당 맵채널 특정 몬스터 삭제/변경 기능 추가

// start 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작
struct MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN
{
	MAP_CHANNEL_INDEX		mapChann;
	INT						iMonsterUnitKind;
	bool					bRegen;
	MONSTER_BALANCE_DATA	MonsterBalanceInfo;			// 2011-06-21 by hskim 몬스터 밸런스 정보
};
// end 2011-06-02 인피니티 3차 - 스텝 6 - 주기적 소환 기능 제작

//#define T_FC_INFINITY_SUCCESS_FIN					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SUCCESS_FIN)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 성공 종료, F -> C
//#define T_FC_INFINITY_FAIL_FIN						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_FAIL_FIN)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 실패 종료, F -> C
//#define T_FC_INFINITY_FIN_OK						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_FIN_OK)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 종료 확인 후 마을(Main서버로 돌아감), C -> F
//#define T_FC_INFINITY_READY_FINISH_MAINSVR_START	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_READY_FINISH_MAINSVR_START)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버로 돌아갈 준비가 완료 됨, F -> C

//#define T_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_INSERT_ITEM_HEADER			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_INSERT_ITEM_HEADER)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_INSERT_ITEM					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_INSERT_ITEM)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_INSERT_ITEM_DONE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_INSERT_ITEM_DONE)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_ITEM_HEADER			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM_HEADER)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_ITEM					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_ITEM_DONE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM_DONE)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_DELETE_ITEM					(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_DELETE_ITEM)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_UPDATE_USER_INFO			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_USER_INFO)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 유저 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_READY_FINISH_MAINSVR_START  (MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_READY_FINISH_MAINSVR_START)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버로 돌아갈 준비가 완료 됨, MFS -> AFS
//#define T_FtoA_INFINITY_LOG							(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_LOG)						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 로그 전송, AFS -> MFS
//#define T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_USING_TIME_LIMIT_ITEM)				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 MainFieldServer로 아이템 동기화 위해 전송, AFS -> MFS
//#define T_FtoA_INFINITY_COME_BACK_POSTWORK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_COME_BACK_POSTWORK)		// 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 후 작업 처리 명령, AFS -> MFS
//#define T_FtoA_INFINITY_COME_BACK_PREWORK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_COME_BACK_PREWORK)		// 2011-09-29 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 돌아가기 위한 사전 작업 처리 명령, AFS -> MFS	
//#define T_FtoA_INFINITY_UPDATE_ITEM_PET				(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_ITEM_PET)			// 2011-09-30 by hskim, 파트너 시스템 2차 - 인피 MainFieldServer로 파트너 아이템 동기화 위해 전송, AFS -> MFS

struct MSG_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	INT				AllItemCount;
};
struct MSG_FtoA_INFINITY_INSERT_ITEM {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	INT				ItemNum;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	BYTE			Wear;
	INT				ShapeItemNum;
	INT				UsingTimeStamp;
	ATUM_DATE_TIME	CreatedTime;
	INT				CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
};
struct MSG_FtoA_INFINITY_UPDATE_ITEM
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID64_t			MainSvrItemUID;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				UsingTimeStamp;
	INT				CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
};

// start 2011-09-30 by hskim, 파트너 시스템 2차
struct MSG_FtoA_INFINITY_UPDATE_ITEM_PET
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID64_t			MainSvrItemUID;

	tPET_CURRENTINFO	PetCurrentData;		// Arena -> Field 로 전송후 Field 서버에서는 PetSocketItemUID 및 pItemPetSocket 의 데이터는 적용하지 않는다
};
// end 2011-09-30 by hskim, 파트너 시스템 2차

struct MSG_FtoA_INFINITY_DELETE_ITEM {
	//	INT InfinityDeleteItemInfoListCount;
	//	_ARRAY(INFINITY_DELETE_ITEM_INFO);
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	UID64_t			MainSvrItemUID;
};
struct MSG_FtoA_INFINITY_UPDATE_USER_INFO {
	UID32_t				AccountUID;
	UID32_t				MFSCharacterUID;
	ClientIndex_t		MFSClientIndex;
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
struct MSG_FtoA_INFINITY_READY_FINISH_MAINSVR_START {
	ClientIndex_t	AFSClientIndex;
};

struct MSG_FtoA_INFINITY_LOG {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	InfiModeUID_t	InfinityModeUID;
	DeathCnt_t		DeathCount;
	KillCnt_t		KillCount;
	ATUM_DATE_TIME	StartTime;
	ATUM_DATE_TIME	EndTime;
	Experience_t	AllEXP;								// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 종료시 최대 경험치 추가
};

struct MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	INT				ItemNum;
	INT				ItemWindowIndex;
	INT				CurrentCount;
	BYTE			Wear;
	INT				ShapeItemNum;
	INT				UsingTimeStamp;
	ATUM_DATE_TIME	CreatedTime;
	UID64_t			MainSvrItemUID;
	INT				CoolingTimeStamp;			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
};

// start 2011-09-29 by hskim, 파트너 시스템 2차
struct MSG_FtoA_INFINITY_COME_BACK_PREWORK
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
};
// end 2011-09-29 by hskim, 파트너 시스템 2차

// start 2012-01-16 by hskim, 통계 - 화패
struct MSG_FtoA_INFINITY_COME_BACK_POSTWORK
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;

	SSTATISTICS_MONEY_UNIT DeltaMoney;
};
// end 2012-01-16 by hskim, 통계 - 화패

struct MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT {
	MAP_CHANNEL_INDEX	mapChann;
	ObjectIdx_t			DeleteObjectUID;
	ObjectNum_t			NewObjectNum;
};
struct MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT {
	MAP_CHANNEL_INDEX	mapChann;
};

//#define T_FC_INFINITY_TENDER_DROPITEM_INFO		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_DROPITEM_INFO)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터에게서 드랍된 Tender 아이템, F -> C
//#define T_FC_INFINITY_TENDER_START				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_START)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender 시작, F -> C
//#define T_FC_INFINITY_TENDER_PUT_IN_TENDER		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_PUT_IN_TENDER)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 유저 Tender 버튼 누름, C -> F
//#define T_FC_INFINITY_TENDER_PUT_IN_TENDER_OK	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_PUT_IN_TENDER_OK)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 유저 Tender 버튼 누름 결과, F -> C
//#define T_FC_INFINITY_TENDER_RESULT				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_RESULT)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 입찰자 결과, F -> C
//#define T_FC_INFINITY_TENDER_RESULT_TIMEOVER	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_TIMEOVER)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender 시간이 지났지만 입찰자가 없다, F -> C
//#define T_FC_INFINITY_TENDER_RESULT_ALLGIVEUP	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_TENDER_ALLGIVEUP)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 입찰자 모두 포기, F -> C
struct MSG_FC_INFINITY_TENDER_DROPITEM_INFO {
	UINT			ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
	ItemNum_t		TenderDropItemNum;
	INT				Count;
	INT				PrefixCodeNum;
	INT				SuffixCodeNum;
	ATUM_DATE_TIME	TenderStartTime;		// 입찰 시작 시간.
};
struct MSG_FC_INFINITY_TENDER_START {
	UINT			ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
	ItemNum_t		TenderItemNum;
	INT				Count;
};
struct MSG_FC_INFINITY_TENDER_PUT_IN_TENDER {
	ItemNum_t			TenderDropItemNum;	// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 메세지 추가
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
	UINT				ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
	bool				GiveUp;
};
struct MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK {
	UINT				ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
	ClientIndex_t		DiceUserIdx;			// 입찰 유저
	DiceCnt_t			DiceCount;				// 주사위 결과 값.
};
struct MSG_FC_INFINITY_TENDER_RESULT {
	UINT				ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
	ClientIndex_t		DiceUserIdx;			// 입찰 유저
	DiceCnt_t			DiceCount;				// 주사위 결과 값.
	ItemNum_t			TenderItemNum;
	INT					Count;
};
struct MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER {
	UINT				ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
};
struct MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP {
	UINT				ItemFieldIndex;			// 습득 전까지 서버가 임시로 관리하는 번호
};

//#define T_FC_INFO_APPLY_RESISTANCE_ITEM	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_APPLY_RESISTANCE_ITEM)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 저항 아이템 적용 정보 전송, F -> C(n)
struct MSG_FC_INFO_APPLY_RESISTANCE_ITEM {
	ClientIndex_t		ResistanceUserIdx;
	ItemNum_t			SkillNum;			// 2009-09-09 ~ 2010-01-19 by dhjin, 인피니티 - 저항된 스킬 정보 추가
	bool				OnceResistanceItem;	// 2009-09-09 ~ 2010-01-19 by dhjin, 인피니티 - 한 번만 저항하고 없어지는 저항 아이템 추가
};

//#define T_FC_SHOP_INFINITY_ITEM_HEADER		(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_ITEM_HEADER)	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점
//#define T_FC_SHOP_INFINITY_ITEM				(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_ITEM)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점
//#define T_FC_SHOP_INFINITY_ITEM_DONE		(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_ITEM_DONE)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점
using MSG_FC_SHOP_INFINITY_ITEM = INFINITY_SHOP_INFO;
//#define T_FC_SHOP_INFINITY_BUY_ITEM			(MessageType_t)((T0_FC_SHOP<<8)|T1_FC_SHOP_INFINITY_BUY_ITEM)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 상점, 구매
struct MSG_FC_SHOP_INFINITY_BUY_ITEM {
	InfinityShopUID_t		InfinityShopUID;
	int						Amount;
};

//#define T_FC_INFINITY_CHANGE_LIMITTIME				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_LIMITTIME)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 사망시 패널티 추가, F -> C
struct MSG_FC_INFINITY_CHANGE_LIMITTIME {
	char			DeadCharactarName[SIZE_MAX_CHARACTER_NAME];		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 죽은 유저 이름 정보 전송 추가
	MSec_t			LimitTime;
};

// start 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능 
//#define T_FC_INFINITY_SET_LIMITTIME				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SET_LIMITTIME)		// 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능
struct MSG_FC_INFINITY_SET_LIMITTIME {
	MSec_t			LimitTime;		// ms
};
// end 2011-05-30 by hskim, 인피니티 3차 - 플레이 시간 재설정 기능

//#define T_FC_INFINITY_JOIN_CANCEL							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_CANCEL)							// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 입장 취소
//#define T_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 입장 취소
struct MSG_FC_INFINITY_JOIN_CANCEL {
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};
struct MSG_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER {
	ClientIndex_t		JoinCancelClientIdx;
};

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
//#define T_FC_INFINITY_REQUEST_RESTART_BY_DISCONNECT					(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_REQUEST_RESTART_BY_DISCONNECT)			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, MFS -> C
//#define T_FC_INFINITY_RESTART_BY_DISCONNECT							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_RESTART_BY_DISCONNECT)					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, C -> AFS
//#define T_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT				(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT)		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리, C -> AFS
//#define T_FC_INFINITY_DELETED_CINEMA_HEADER							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DELETED_CINEMA_HEADER)					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
//#define T_FC_INFINITY_DELETED_CINEMA								(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DELETED_CINEMA)							// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
//#define T_FC_INFINITY_DELETED_CINEMA_DONE							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DELETED_CINEMA_DONE)						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
//#define T_FC_INFINITY_ENTER_BY_DISCONNECT							(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_ENTER_BY_DISCONNECT)						// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
using MSG_FC_INFINITY_DELETED_CINEMA = CINEMAINFO;
struct MSG_FC_INFINITY_ENTER_BY_DISCONNECT {
	eINFINITY_MODE		InfinityMode;
	Minute_t			TimePenaltyValue;
	ChannelIndex_t		InfinityChannelIdx;
	D3DXVECTOR3			CharacterPosition;
	ATUM_DATE_TIME		StartTime;
	MSec_t				LimitTime;
	MSec_t				DecreaseTimeByDead;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	InfinityCreateUID_t	InfinityCreateUID;	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	ClientIndex_t		MasterClientIdx;	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	Penalty_t			HPPenaltyValue;			// 2011-06-14 by hskim, 인피니티 3차 - 패널티 기능 추가 (HP 및 시간 동시 지원을 위해)
};

//#define T_FtoA_INFINITY_STATE_CHANGE			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_STATE_CHANGE)		// 2009-09-09 ~ 2010-01-20 by dhjin, 인피니티 - 인피 상태 값을 메인서버로 전송한다.
struct MSG_FtoA_INFINITY_STATE_CHANGE {
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	eINFINITY_STATE	AFSInfinityState;
	ClientIndex_t	AFSClientIndex;
};

// start 2011-05-17 by hskim, 인피니티 3차 - 시네마 연출
//#define T_FC_INFINITY_MOVIE						(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_MOVIE)				// 2011-05-17 by hskim, 인피니티 3차 - 시네마 연출, F -> C

struct MSG_FC_INFINITY_MOVIE
{
	INT			UniqueNumber;			// 연출 번호
	USHORT		Command;				// MOVIE_COMMAND_START, MOVIE_COMMAND_END
};
// end 2011-05-17 by hskim, 인피니티 3차 - 시네마 연출

//#define T_FI_PARTY_AUTO_CREATE						(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_AUTO_CREATE)					// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 생성 요청, F -> I
//#define T_FI_PARTY_AUTO_CREATE_OK					(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_AUTO_CREATE_OK)				// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 생성 완료, I -> F
//#define T_FC_PARTY_AUTO_CREATED						(MessageType_t)((T0_FC_PARTY<<8)|T1_FC_PARTY_AUTO_CREATED)					// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 생성 완료 알림, F -> C
//#define T_IC_PARTY_GET_AUTO_PARTY_INFO				(MessageType_t)((T0_IC_PARTY<<8)|T1_IC_PARTY_GET_AUTO_PARTY_INFO)			// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 정보 요청, C -> I
//#define T_IC_PARTY_GET_AUTO_PARTY_INFO_OK			(MessageType_t)((T0_IC_PARTY<<8)|T1_IC_PARTY_GET_AUTO_PARTY_INFO_OK)		// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 정보 전송, I -> C
struct MSG_FI_PARTY_AUTO_CREATE {
	INT				PartyMemberListCount;
	UID32_t			MasterUID;
	_ARRAY(UID32_t);
};
struct MSG_FI_PARTY_AUTO_CREATE_OK {
	PartyID_t				PartyID;
	MEX_FIELD_PARTY_INFO	FieldPartyInfo;
	INT						PartyMemberListCount;
	UID32_t					MasterUID;
	_ARRAY(UID32_t);
};
struct MSG_FC_PARTY_AUTO_CREATED {
	PartyID_t				PartyID;
};
//////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 자동으로 만들 파티의 정보.
struct MSG_FC_PARTY_AUTO_CREATED_THE_ARENA {
	SPARTY_INFO				PartyInfo;
};
// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 자동으로 만들 파티의 정보.

struct MSG_IC_PARTY_GET_AUTO_PARTY_INFO {
	PartyID_t				PartyID;
};
struct MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK {
	PartyID_t				PartyID;
	SPARTY_INFO				PartyInfo;
	UID32_t					MasterUniqueNumber;			// 파티장의 CharacterUniqueNumber
	UINT					nNumOfPartyMembers;
	ARRAY_(IM_PARTY_MEMBER_INFO);
};

//#define T_FI_PARTY_DISCONNECT_LEAVE_OK		(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_DISCONNECT_LEAVE_OK)	// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 강제 종료 유저 파티에서 탈퇴처리!
using MSG_FI_PARTY_DISCONNECT_LEAVE_OK = MSG_FI_PARTY_LEAVE_OK;

///////////////////////////////////////////////////////////////////////////////
// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
using vectITEM_W_COUNT = vector<ITEM_W_COUNT>;
using mtvectITEM_W_COUNT = mt_vector<ITEM_W_COUNT>;

///////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
//#define T_FC_ITEM_USE_INVOKING_WEAR_ITEM	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_USE_INVOKING_WEAR_ITEM)	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 사용, C->F
struct MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM {
	ItemUID_t				InvokingWearItemUID;
};

//#define T_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM)	// C->F, 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템 지속 시간 완료
struct MSG_FC_ITEM_EXPIRE_TIME_INVOKING_WEAR_ITEM {
	ItemUID_t				InvokingWearItemUID;
};

//#define T_FC_INFO_APPLY_DESTPARAM		(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_APPLY_DESTPARAM)			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, DestParam 적용 유저 정보 전송
struct MSG_FC_INFO_APPLY_DESTPARAM {
	ClientIndex_t			ApplyClientIdx;
	DestParam_t				ApplyDestParam;
	EffectIdx_t				ApplyEffectIdx;
	ItemUID_t				ApplyItemUID;
};

//#define T_FC_INFO_APPLY_DESTPARAM_LIST	(MessageType_t)((T0_FC_INFO<<8)|T1_FC_INFO_APPLY_DESTPARAM_LIST)			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, DestParam 적용 유저 정보 전송
struct APPLY_DESTPARAM_INFO {
	DestParam_t				ApplyDestParam;
	EffectIdx_t				ApplyEffectIdx;
};
struct MSG_FC_INFO_APPLY_DESTPARAM_LIST {
	ClientIndex_t			ApplyClientIdx;
	ItemUID_t				ApplyItemUID;
	UINT					ApplyDestParamCnt;
	ARRAY_(APPLY_DESTPARAM_INFO);
};

//#define T_FC_ITEM_END_COOLINGTIME_ITEM		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_END_COOLINGTIME_ITEM)	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 종료 C->F
struct MSG_FC_ITEM_END_COOLINGTIME_ITEM {
	ItemUID_t				ItemUID;
};

//#define T_FC_ITEM_END_COOLINGTIME_ITEM_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_END_COOLINGTIME_ITEM_OK)	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 종료 F->C
struct MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK {
	ItemUID_t				ItemUID;
};

//#define T_FC_ITEM_GET_COOLINGTIME_INFO		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_GET_COOLINGTIME_INFO)	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 정보 요청 C->F
//#define T_FC_ITEM_GET_COOLINGTIME_INFO_OK	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_GET_COOLINGTIME_INFO_OK)	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 쿨타임 정보 요청 F->C
struct MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK {
	UID64_t			ItemUID;
	INT				CoolingTimeStamp;		// 지금까지 사용된 시간(단위:초)
};

//#define T_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF)	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 버프 올리세요 F->C
struct MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF {
	ItemUID_t				ItemUID;
};

///////////////////////////////////////////////////////////////////////////////
// 2010-03-31 by dhjin, 인피니티(기지방어) -
//#define T_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP)		// 2010-03-31 by dhjin, 인피니티(기지방어) - 인피 사망시 패널티, F -> C
struct MSG_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP {
	char			DeadCharactarName[SIZE_MAX_CHARACTER_NAME];
	MonIdx_t		MonsterNum;
	ClientIndex_t	MonsterIdx;
	float			CurrentHP;
};

//#define T_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR					(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR)		// 2010-03-31 by dhjin, 인피니티(기지방어) - F -> C
struct MSG_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR
{
	MonIdx_t		MonsterNum;
	ClientIndex_t	MonsterIdx;
	float			CurrentHP;

	// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - MAX HP 추가.
	float			MaxHP;
};


//#define T_FC_ITEM_UPDATE_TRANSFORMER_OK				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_UPDATE_TRANSFORMER_OK)	// F->C(n), // 2010-03-18 by cmkwon, 몬스터변신 구현 - 
struct MSG_FC_ITEM_UPDATE_TRANSFORMER_OK		// 2010-03-18 by cmkwon, 몬스터변신 구현 - 
{
	UID32_t			CharacUID;
	ClientIndex_t	ClientIdx;
	INT				MonsterUnitKind;
};

//#define T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK			(MessageType_t)((T0_FI_PARTY<<8)|T1_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK)	// F->I, // 2010-03-18 by cmkwon, 몬스터변신 구현 - 
using MSG_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK = MSG_FC_ITEM_UPDATE_TRANSFORMER_OK;	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 

																					//#define T_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK			(MessageType_t)((T0_IC_PARTY<<8)|T1_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK)	// I -> C(n), // 2010-03-18 by cmkwon, 몬스터변신 구현 - 
using MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK = MSG_FC_ITEM_UPDATE_TRANSFORMER_OK;	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 

																					///////////////////////////////////////////////////////////////////////////////
																					// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
																					//#define T_FtoA_INFINITY_START_CHECK			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START_CHECK)		// AF->MF, // 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
																					//#define T_FtoA_INFINITY_START_CHECK_ACK		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_START_CHECK_ACK)	// MF->AF, // 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
struct MSG_FtoA_INFINITY_START_CHECK		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
{
	UID32_t			AccountUID;
	UID32_t			MFSCharacterUID;
	ClientIndex_t	MFSClientIndex;
	ClientIndex_t	AFSClientIndex;
	bool			bCheckReentryTicket;	// 
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};

struct MSG_FtoA_INFINITY_START_CHECK_ACK		// 2010-03-23 by cmkwon, 인피니티 입장 캐쉬 아이템 구현 - 
{
	Err_t			ErrorCode;
	UID32_t			AccountUID;
	ClientIndex_t	AFSClientIndex;
	eINFINITY_MODE		InfinityMode;
	InfinityCreateUID_t	InfinityCreateUID;
};

//#define T_FN_BATTLE_ATTACK_SKILL_CANCEL					(MessageType_t)((T0_FN_BATTLE<<8)|T1_FN_BATTLE_ATTACK_SKILL_CANCEL)				// 2010-03-31 by dhjin, 인피니티(기지방어) - N -> F
struct MSG_FN_BATTLE_ATTACK_SKILL_CANCEL
{
	MAP_CHANNEL_INDEX	MapInfo;
	ClientIndex_t		MonsterIndex;
	INT					SkillItemNum;				// 스킬의 ItemNum
};

//#define T_FC_MONSTER_SKILL_CANCEL							(MessageType_t)((T0_FC_MONSTER<<8)|T1_FC_MONSTER_SKILL_CANCEL)			// 2010-03-31 by dhjin, 인피니티(기지방어) - , F->C(n)
struct MSG_FC_MONSTER_SKILL_CANCEL {
	ClientIndex_t		MonsterIndex;
	INT					SkillItemNum;				// 스킬의 ItemNum
};

//#define T_FtoA_UPDATE_ITEM_NOTI			(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_UPDATE_ITEM_NOTI)		//	MFS -> AFS // 2010-03-31 by dhjin, 인피니티 입장 캐쉬 아이템 구현 - 
struct MSG_FtoA_UPDATE_ITEM_NOTI {
	ClientIndex_t		AFSClientIdx;
	ItemNum_t			ItemNum;
};

//#define T_FtoA_INFINITY_UPDATE_USER_MAP_INFO	(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_INFINITY_UPDATE_USER_MAP_INFO)	//	AFS -> MFS // 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
struct MSG_FtoA_INFINITY_UPDATE_USER_MAP_INFO
{
	UID32_t				AccountUID;
	UID32_t				MFSCharacterUID;
	ClientIndex_t		MFSClientIndex;
	MapIndex_t			InfinityMapIndex;				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
	ChannelIndex_t		InfinityChannelIndex;			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 팅긴 유저 재접속 처리
};


// 2010-05-04 by shcho, 인피니티 난이도 조절 start
//#define T_FC_INFINITY_DIFFICULTY_LIST		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DIFFICULTY_LIST) // C -> AFS // 2010-05-04 by shcho, 난이도 정보 목록 요청
//#define T_FC_INFINITY_DIFFICULTY_LIST_OK		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_DIFFICULTY_LIST_OK) // AFS -> C // 2010-05-04 by shcho, 클라이언트 난이도 정보 목록 요청에 응답
struct MSG_INFINITY_DIFFICULTY_LIST_OK
{
	int Difficulty_List_count; //리스트가 몇 개인지 보냄.
	_ARRAY(INFINITY_DIFFICULTY_BONUS_INFO);
};
// 2010-05-04 by shcho, 인피니티 난이도 조절 end


// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
//#define T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL		(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL)		// C -> AFS // 2010-05-24 by hsLee, 난이도 변경 요청.
//#define T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK	(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK)	// AFS -> C // 2010-05-24 by hsLee, 난이도 변경 요청 결과.

struct MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL
{
	INT	InfinityDifficultyLevel;
};

// 2010. 06. 01 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가 + 난이도 변경 결과 패킷 변경.)
struct MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK
{
	bool bUpdate;									// 클라이언트 메시지 처리를 위한 Flag.
	INT InfinityDifficultyLevel;
};
// End 2010. 06. 01 by hsLee 인피니티 필드 2차 난이도 조절. (에디트 박스 추가 + 난이도 변경 결과 패킷 변경.)

// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

// 2010-06-01 by shcho, GLogDB 관련 -
//#define T_FL_LOG_EVENT_PARTICIPATION_RATE		(MessageType_t)((T0_FL_LOG<<8)|T1_FL_LOG_EVENT_PARTICIPATION_RATE)			// F -> L // 2010-06-01 by shcho, GLogDB 관련 -
struct MSG_FL_LOG_EVENT_PARTICIPATION_RATE
{
	ATUM_DATE_TIME		StartTime;
	ATUM_DATE_TIME		EndTime;
	INT					ParticipationCount;
	USHORT				ParticipationRate;
	CHAR				Description[SIZE_MAX_GLOG_EVENT_DESCRIPTION];
};

// 2010-06-01 by shcho, PC방 권한 획득(캐쉬) 아이템 - 
//#define T_FC_ITEM_HOMEPREMIUM_INFO			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_HOMEPREMIUM_INFO)	// F->C, // 2010-06-01 by shcho, PC방 권한 획득(캐쉬) 아이템 -
struct MSG_FC_ITEM_HOMEPREMIUM_INFO
{
	bool bUse; // 0:FALSE  1:TRUE
};

///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
//
// 기존 
//
//
//#define T_FC_ITEM_PET_HEADER				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_HEADER)			// 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가 
//#define T_FC_ITEM_PET						(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET)					// 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가 
//#define T_FC_ITEM_PET_BASEDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_BASEDATA_OK)		// 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가 
//#define T_FC_ITEM_PET_SKILLDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SKILLDATA_OK)	// 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가 
//#define T_FC_ITEM_PET_SOCKETDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SOCKETDATA_OK)	// 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가 
//#define T_FC_ITEM_PET_DONE					(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_DONE)			// 2010-06-15 by shcho&hslee 펫시스템 - 클라이언트 패킷 전송 커맨드 추가 
//
// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 수치 정보 구조체
// struct MSG_FC_ITEM_PET_BASEDATA
// {
// 	BYTE			ItemUpdateType;										// 아이템 수정 타입, IUT_XXX
// 
// 	UID64_t			CreatedPetUID;
// 	
// 	bool			EnableEditPetName;
// 	bool			EnableLevelUp;
// 	
// 	char			szPetName[SIZE_MAX_PET_NAME];
// 	
// 	INT				PetIndex;
// 	INT				PetLevel;
// 	
// 	Experience_t	PetExp;
// 	
// 	FLOAT			Stamina;
// 	
// 	INT				SourceIndex_Field;
// 	INT				SourceIndex_City;
// 
// 	ItemNum_t		ItemIndex_PetSkill[SIZE_MAX_PETSKILLITEM];
// 	
// //	ITEM			sItem_PetSkill[SIZE_MAX_PETSKILLITEM];
// //	ITEM			sItem_PetSocketItem[SIZE_MAX_PETSOCKETITEM];
// };
// 
// 
// // 2010-06-15 by shcho&hslee 펫시스템 - 팻 스킬 정보 구조체
// struct MSG_FC_ITEM_PET_SKILLDATA
// {
// 	UID64_t			CreatePetUID;
// 
// 	INT				iSlotIndex;
// 
// 	ITEM			sItem_PetSkill;
// };
// 
// 
// // 2010-06-15 by shcho&hslee 펫시스템 - 팻 소켓 정보 구조체
// struct MSG_FC_ITEM_PET_SOCKETDATA
// {
// 	UID64_t			CreatePetUID;
// 	
// 	INT				iSlotIndex;
// 	
// 	ITEM			sItem_PetSocket;
// };

/////////
// 수정

//#define T_FC_ITEM_PET_HEADER				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_HEADER)
//#define T_FC_ITEM_PET						(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET)	
//#define T_FC_ITEM_PET_BASEDATA_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_BASEDATA_OK)
//#define T_FC_ITEM_PET_DONE					(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_DONE)

//#define T_FC_ITEM_PET_SET_NAME				(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_NAME)
//#define T_FC_ITEM_PET_SET_NAME_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_NAME_OK)
//#define T_FC_ITEM_PET_SET_EXP_RATIO			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_EXP_RATIO)
//#define T_FC_ITEM_PET_SET_EXP_RATIO_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_EXP_RATIO_OK)
//#define T_FC_ITEM_PET_CHANGE_LEVEL			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_CHANGE_LEVEL)
//#define T_FC_ITEM_PET_CHANGE_EXP			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_CHANGE_EXP)

//#define T_FC_ITEM_PET_SET_SOCKET			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_SOCKET)
//#define T_FC_ITEM_PET_SET_SOCKET_OK			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_SOCKET_OK)
//#define T_FC_ITEM_PET_SET_KIT_SLOT			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_SLOT)
//#define T_FC_ITEM_PET_SET_KIT_SLOT_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_SLOT_OK)
//#define T_FC_ITEM_PET_SET_AUTOSKILL_SLOT	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_AUTOSKILL_SLOT)
//#define T_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK	(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_PET_SET_KIT_AUTOSKILL_SLOT_OK)

struct MSG_FC_ITEM_PET_BASEDATA
{
	BYTE			ItemUpdateType;										// 아이템 수정 타입, IUT_XXX
	UID64_t			CreatedPetUID;
	char			PetName[SIZE_MAX_PET_NAME];
	INT				PetIndex;
	INT				PetLevel;
	Experience_t	PetExp;
	BYTE			PetExpRatio;
	BYTE			PetEnableSocketCount;

	UID64_t			PetSocketItemUID[SIZE_MAX_PETSOCKET];

	SPET_KIT_SLOT_DATA			PetKitHP;
	SPET_KIT_SLOT_DATA			PetKitShield;
	SPET_KIT_SLOT_DATA			PetKitSP;

	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;
};

// 이름 변경

struct MSG_FC_ITEM_PET_SET_NAME
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	char			PetName[SIZE_MAX_PET_NAME];			// 변경될 파트너 이름
};

struct MSG_FC_ITEM_PET_SET_NAME_OK
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	char			PetName[SIZE_MAX_PET_NAME];			// 변경될 파트너 이름
};

// 경험치 획득 비율 변경

struct MSG_FC_ITEM_PET_SET_EXP_RATIO
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	BYTE			ExpRatio;							// 경험치 획득 비율
};

struct MSG_FC_ITEM_PET_SET_EXP_RATIO_OK
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	BYTE			ExpRatio;							// 경험치 획득 비율
};

// 경험치 증가 및 레벨 업

struct MSG_FC_ITEM_PET_CHANGE_LEVEL
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	BYTE			Level;								// 파트너 현재 레벨
};

struct MSG_FC_ITEM_PET_CHANGE_EXP
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	Experience_t	Experience;							// 총 경험치
};

// 소켓 아이템 설정

struct MSG_FC_ITEM_PET_SET_SOCKET
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	INT				SocketIndex;						// 소켓 번호
	UID64_t			PetSocketItemUID;					// 소켓 아이템 유니크 번호
};

struct MSG_FC_ITEM_PET_SET_SOCKET_OK
{
	UID64_t			ItemUniqueNumber;					// 파트너 아이템 UID
	INT				SocketIndex;						// 소켓 번호
	UID64_t			PetSocketItemUID;					// 소켓 아이템 유니크 번호
};

// 키트 슬롯 설정

struct MSG_FC_ITEM_PET_SET_KIT_SLOT
{
	UID64_t				ItemUniqueNumber;				// 파트너 아이템 UID
	INT					SocketIndex;					// 소켓 번호
	UID64_t				PetSocketItemUID;				// 소켓 아이템 UID
	SPET_KIT_SLOT_DATA	PetKitHP;						// HP 키트 슬롯 정보
	SPET_KIT_SLOT_DATA	PetKitShield;					// Shield 키트 슬롯 정보
	SPET_KIT_SLOT_DATA	PetKitSP;						// SP 키트 슬롯 정보
};

struct MSG_FC_ITEM_PET_SET_KIT_SLOT_OK
{
	UID64_t				ItemUniqueNumber;				// 파트너 아이템 UID
	INT					SocketIndex;					// 소켓 번호
	UID64_t				PetSocketItemUID;				// 소켓 아이템 UID
	SPET_KIT_SLOT_DATA	PetKitHP;						// HP 키트 슬롯 정보
	SPET_KIT_SLOT_DATA	PetKitShield;					// Shield 키트 슬롯 정보
	SPET_KIT_SLOT_DATA	PetKitSP;						// SP 키트 슬롯 정보
};

// 오토 스킬 슬롯 설정

struct MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT
{
	UID64_t						ItemUniqueNumber;		// 파트너 아이템 UID
	INT							SocketIndex;			// 소켓 번호
	UID64_t						PetSocketItemUID;		// 소켓 아이템 UID
	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;			// 오토 스킬 정보
};

struct MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK
{
	UID64_t						ItemUniqueNumber;		// 파트너 아이템 UID
	INT							SocketIndex;			// 소켓 번호
	UID64_t						PetSocketItemUID;		// 소켓 아이템 UID
	SPET_AUTOSKILL_SLOT_DATA	PetAutoSkill;			// 오토 스킬 정보
};

// end 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현
///////////////////////////////////////////////////////////////////////////////////////

//#define T_FC_INFINITY_SKIP_ENDING_CINEMA			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SKIP_ENDING_CINEMA)		// 인피니티 거점 방어 엔딩 시네마 연출 스킵 요청. 2010. 07. 27 by hsLee.
//#define T_FC_INFINITY_SKIP_ENDING_CINEMA_OK			(MessageType_t)((T0_FC_INFINITY<<8)|T1_FC_INFINITY_SKIP_ENDING_CINEMA_OK)	// 인피니티 거점 방어 엔딩 시네마 연출 스킵 결과. 2010. 07. 27 by hsLee.

struct MSG_FC_INFINITY_SKIP_ENDING					// 인피니티 시네마 연출 스킵 패킷.
{
	InfinityCreateUID_t	InfinityCreateUID;			// 인피니티 파티 UID.
	bool				bNormalEnding;				// 정상 연출 종료 & 스킵 요청 구별 인자.
};

struct MSG_FC_INFINITY_SKIP_ENDING_CINEMA_OK		// 인피니티 시네마 연출 스킵 결과 패킷.
{
	bool			EnableSkip;								// 연출 스킵 적용 구별 인자.
	bool			NormalEnding;							// 정상 연출 종료 & 스킵 요청 구별 인자.

	char			szCharName[SIZE_MAX_CHARACTER_NAME];	// 스킵일 경우 System Message처리에 사용할 요청자 캐릭터 이름.

	ATUM_DATE_TIME	sUpdateTenderItemStartTime;			// 입찰 아이템의 입찰 제한 시간 갱신값. (연출 스킵이 적용된 시각).
};

// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리
//#define T_FC_DISSOLVED_ITEM			(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_DISSOLUTIONITEM)		// client -> field
//#define T_FC_DISSOLVED_ITEM_OK		(MessageType_t)((T0_FC_ITEM<<8)|T1_FC_ITEM_DISSOLUTIONITEM_OK)	// field  -> client

struct MSG_FC_DISSOLVED_ITEM // 용해 할 아이템 정보 구조체 client -> field
{
	UID64_t UniqueNumber;
	INT	Itemnum;
};

struct MSG_FC_DISSOLVED_SUCCESS_AND_FAILED // 용해된 아이템 처리 결과
{
	bool Item_Success;		// 용해 성공 또는 실패
	bool Dissolved_success;
};
// END 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 시스템 패킷 처리

// 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
//#define T_FtoA_LOG_INFINITY_USER_GET_TENDERITEM		(MessageType_t)((T0_MF_TO_AF<<8)|T1_FtoA_LOG_INFINITYUSER_GET_TENDERITEM) // Field <- Arena

struct MSG_FtoA_INFINITY_TENDER_ITEM
{
	INT			TenderItemNum;		// 획득한 아이템 넘버
	DiceCnt_t	GDicecount;			// 획득한 주사위 카운트
	UID64_t		AccountUID;			// 계정 UID
	CHAR		AccountName[SIZE_MAX_ACCOUNT_NAME];	// 계정 이름
	UID64_t		CharacterUID;						// 케릭터 UID
	CHAR		CharacterName[SIZE_MAX_CHARACTER_NAME];	// 케릭터 이름
	INT			MFSClientIdx;

};
// END 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, 인증 서버 구현
//#define T_PAUTH_CONNECT_LOGIN						(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN)
//#define T_PAUTH_CONNECT_LOGIN_OK					(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN_OK)
//#define T_PAUTH_CONNECT_LOGIN_FAIL					(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN_FAIL)
//#define T_PAUTH_CONNECT_LOGIN_SHUTDOWN				(MessageType_t)((T0_PAUTH_CONNECT<<8)|T1_PAUTH_CONNECT_LOGIN_SHUTDOWN)		// 2011-06-22 by hskim, 사설 서버 방지

struct SSERVER_AUTHENTICATION_ACCOUNT
{
	UINT64	UniqueNumber;
	char	szGameName[SIZE_MAX_AUTH_GAMENAME];
	char	szServerIP[SIZE_MAX_IPADDRESS];
	char	szServerNetmask[SIZE_MAX_IPADDRESS];
	char	szPublisher[SIZE_MAX_AUTH_PUBLISHER];
	int		iAllow;
	int		iShutdown;
	char	szResourceFileName[SIZE_MAX_AUTH_RESOURCE_FILE_NAME];
};

struct QPARAM_AUTHENTICATION_ACCOUNT
{
	bool	bResult;
	char	szGameName[SIZE_MAX_AUTH_GAMENAME];
	char	szServerPublicIP[SIZE_MAX_IPADDRESS];
	char	szServerPrivateIP[SIZE_MAX_IPADDRESS];
	char	szCurrentVer[SIZE_MAX_AUTH_CURRENTVER];
	int		nLanguageType;
	BYTE	byTestServer;
	USHORT	nServerPort;
	BYTE	byUseExternalAuthentication;
	int		nPreServerGroupCnts;
	int		nEnableGameServerGroupCnts;

	bool	bAccept;
	bool	bShutdown;
	char	szAcceptComment[SIZE_MAX_AUTH_ACCEPT_COMMENT];
	SSERVER_AUTHENTICATION_ACCOUNT AuthAccount;
};

///////////////////////////////////////////////////////////////////////////////
// 2011-06-22 by hskim, 사설 서버 방지

// start 2011-06-22 by hskim, 사설 서버 방지
//#define T_IP_AUTHENTICATION_SHUTDOWN			(MessageType_t)((T0_IP_CONNECT<<8)|T1_IP_AUTHENTICATION_SHUTDOWN)	// PreServer 가 Authentication 서버에 인증되지 않았다면 서버 종료
//#define T_FP_AUTHENTICATION_SHUTDOWN			(MessageType_t)((T0_FP_CONNECT<<8)|T1_FP_AUTHENTICATION_SHUTDOWN)	// PreServer 가 Authentication 서버에 인증되지 않았다면 서버 종료

struct MSG_IP_AUTHENTICATION_SHUTDOWN
{
	bool	bFlag;
};

struct MSG_FP_AUTHENTICATION_SHUTDOWN
{
	bool	bFlag;
};
// end 2011-06-22 by hskim, 사설 서버 방지

// 2012-07-11 by hskim, 선택적 셧다운
//#define T_IP_SELECTIVE_SHUTDOWN_NOTICE	(MessageType_t)((T0_IP_CONNECT<<8)|T1_IP_SELECTIVE_SHUTDOWN_NOTICE)

struct MSG_IP_SELECTIVE_SHUTDOWN_NOTICE
{
	UID32_t AccountUID;
	char m_szAccountName[SIZE_MAX_ACCOUNT_NAME];
	int nRemainingTime;
};
// end 2012-07-11 by hskim, 선택적 셧다운

// start 2011-11-03 by shcho, yedang 셧다운제 구현 - 
//#define T_FC_SHUTDOWNMINS_USER_ALTER			(MessageType_t)((T0_FC_CONNECT<<8)|T1_FC_CONNECT_SHUTDOWNMINS_USER_ALTER)	// nobody
//#define T_FC_SHUTDOWNMINS_USER_ENDGAME			(MessageType_t)((T0_FC_CONNECT<<8)|T1_FC_CONNECT_SHUTDOWNMINS_USER_ENDGAME)	// nobody

// end 2011-11-03 by shcho, yedang 셧다운제 구현 - 

// start 2011-10-18 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
//#define T_FC_TRIGGER_MAP_BUFF_SKILL				(MessageType_t)((T0_FC_TRIGGER<<8)|T1_FC_TRIGGER_MAP_BUFF_SKILL)

struct MSG_FC_TRIGGER_MAP_BUFF_SKILL
{
	ClientIndex_t		ClientIndex;				// 클라이언트 인덱스
	INT					SkillItemNum;				// 스킬의 ItemNum
	INT					AddTime;					// 스킬 적용 시간에 추가되는 값 (ms)
};
// end 2011-10-18 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
struct MSG_FC_OBJECT_CHANGE
{
	ObjectIdx_t			ObjectIdx;
	ObjectNum_t			ChangeObjectIdx;
};
using mtvectObjectChange = mt_vector<MSG_FC_OBJECT_CHANGE>;

//#define T_FC_SET_LIMITTIME						(MessageType_t)((T0_FC_TRIGGER<<8)|T1_FC_SET_LIMITTIME)

struct MSG_FC_SET_LIMITTIME
{
	MSec_t			LimitTime;		// ms
};

using mtvectCharacterUID = mt_vector<UID32_t>;
// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템



// start 2012-10-08 by khkim, GLog
#define GLOG_AC_LT_LOGIN						0x01	// 1 - 로그인	
#define GLOG_AC_LT_LOGOUT						0x02	// 2 - 로그아웃 

#define GLOG_CD_LT_GAMESTART					0x01	// 1 - 게임 시작
#define GLOG_CD_LT_GAMEEND						0x02	// 2 - 게임 종료
#define GLOG_CD_LT_CHARACTERCREATE				0x03	// 3 - 캐릭터 생성
#define GLOG_CD_LT_CHARACTERDELETE				0x04	// 4 - 캐릭터 삭제
#define GLOG_CD_LT_CHARACTLEVELUP				0x05	// 5 - 레벨업

#define GLOG_SL_LT_SERVERON						0x01	// 1 - 서버 ON
#define GLOG_SL_LT_SERVEROFF					0x02	// 2 - 서버 OFF

#define	GLOG_IL_LT_DROP							0x01	// 1 - 드랍
#define GLOG_IL_LT_QUEST						0x02	// 2 - 퀘스트
#define GLOG_IL_LT_SHOP_BUY						0x03	// 3 - 상점			(구입)
#define GLOG_IL_LT_SHOP_SELL					0x04	// 4 - 상점			(판매)
#define GLOG_IL_LT_WARPOINTSHOP_BUY				0x05	// 5 - 워포상점		(구입)
#define GLOG_IL_LT_INFINITYOBSHOP_BUY			0x06	// 6 - 오브상점		(구입)
#define GLOG_IL_LT_CASHSHOP_BUY					0x07	// 7 - 캐쉬상점		(구입)
#define GLOG_IL_LT_LUCKYMACHINE_GET				0x08	// 8 - 럭키머신		(받음)
#define GLOG_IL_LT_LUCKYMACHINE_USE				0x09	// 9 - 럭키머신		(사용)

#define GLOG_IL_LT_BAZAARBUY_GET				0x0A	// 10 - 개인상점BUY	(받음)
#define GLOG_IL_LT_BAZAARBUY_GIVE				0x0B	// 11 - 개인상점BUY	(주기)
#define GLOG_IL_LT_BAZAARSELL_GET				0x0C	// 12 - 개인상점SELL(받음)
#define GLOG_IL_LT_BAZAARSELL_GIVE				0x0D	// 13 - 개인상점SELL(주기)
#define GLOG_IL_LT_TREADGIVE					0x0E	// 14 - 거래		(주기)
#define GLOG_IL_LT_TREADGET						0x0F	// 15 - 거래		(받음)
#define GLOG_IL_LT_THROWAWAY					0x10	// 16 - 버리기 
#define GLOG_IL_LT_MIXING_GET					0x11	// 17 - 조합		(받음)
#define GLOG_IL_LT_MIXING_USE					0x12	// 18 - 조합		(사용)
#define GLOG_IL_LT_ENCHANT_UP					0x13	// 19 - 인첸트		(성공)
#define GLOG_IL_LT_ENCHANT_DWON					0x14	// 20 - 인첸트		(실패)
#define GLOG_IL_LT_ENCHANT_DESTROY				0x15	// 21 - 인첸트		(파괴)
#define GLOG_IL_LT_ENCHANT_BEGINNING			0x16    // 22 - 인첸트		(초기화) 
#define GLOG_IL_LT_ENCHANT_USEITEM				0x17	// 23 - 인첸트시 사용한 아이템
#define GLOG_IL_LT_RANDOMBOX					0x18	// 24 - 캡슐형 아이템
#define GLOG_IL_LT_DISSOLUTION_GETITEM			0x19	// 25 - 용해
#define GLOG_IL_LT_SKILL						0x1A	
#define GLOG_IL_LT_LOADING						0x1B
#define GLOG_IL_LT_AUCTION						0x1C
#define GLOG_IL_LT_USE_GETITEM					0x1D
#define GLOG_IL_LT_USE_USEITEM					0x1E
#define GLOG_IL_LT_MGAME_EVENT					0x1F
#define GLOG_IL_LT_USE_ENERGY					0x20
#define GLOG_IL_LT_EXPIRE_CARD_ITEM				0x21
#define GLOG_IL_LT_PENALTY_ON_DEAD				0x22
#define GLOG_IL_LT_PENALTY_AGEAR_FUEL_ALLIN		0x23
#define GLOG_IL_LT_INFLUENCEWAR_KILLER_BONUS	0x24
#define GLOG_IL_LT_BONUS_ITEM					0x25
#define GLOG_IL_LT_ADMIN						0x26
#define GLOG_IL_LT_GIVEEVENTITEM				0x27
#define GLOG_IL_LT_GUILD_STORE					0x28
#define GLOG_IL_LT_EXPIRE_ITEM					0x29
#define GLOG_IL_LT_STORE						0x2A
#define GLOG_IL_LT_STORE_FEE					0x2B
#define GLOG_IL_LT_ARENA_ITEM					0x2C
#define GLOG_IL_LT_TUTORIAL_PAY_ITEM			0x2D
#define GLOG_IL_LT_EXPEDIENCYFUND_PAYBACK		0x2E
#define GLOG_IL_LT_GIVEEVENTITEM_COUPONEVENT	0x2F
#define GLOG_IL_LT_LUCKY_ITEM					0x30
#define GLOG_IL_LT_WAR_CONTRIBUTION				0x31
#define GLOG_IL_LT_WAR_CONTRIBUTION_LEADER		0x32
#define GLOG_IL_LT_WAR_CONTRIBUTION_GUILD		0x33
#define GLOG_IL_LT_BULLET						0x34
#define GLOG_IL_LT_DISSOLUTION_USEITEM			0x35
#define GLOG_IL_LT_DEPENDENCY_ITEM				0x36
#define GLOG_IL_LT_WARP							0x37
#define GLOG_IL_LT_CITYWAR_BRING_SUMOFTEX		0x38

#define GLOG_ISL_GAMESTART						0x01	// 1 - 게임 시작
#define GLOG_ISL_CHARACTLEVELUP					0x02	// 2 - 레벨업
// end 2012-10-08 by khkim, GLog

struct MSG_FC_CHARACTER_XIGNCODE_C_ACK_CODE
{
	ULONG Code;
};
// END 2012-10-17 by bhsohn XignCode작업

////////////////////////////////////////////////////////////////////////////
// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
struct MSG_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE
{
	char DBName[20];
};
using MSG_FP_ADMIN_STRATRGYPOINT_INFO_CHANGE = MSG_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE;
// end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

#define TIMESECOND_USER_NOTICE_MSG_SEND_TIC		3	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 유저 알림 MSG 재전송 주기 시간 3초

// 2013-04-18 by jhseol,bckim 이달의 아머 - 이달의 아머 변경 사항 전달 패킷
struct MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO
{
	UID64_t			ItemUID;			// 아이템 UID
	ItemNum_t		nOptionItemNum;		// 옵션 아이템
	ATUM_DATE_TIME	atEndDate;			// 이벤트 종료 기간
};

//#define T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT	(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT)	// F->C, // 컬렉션에 표시할 이달의아머 이벤트 정보 초기화 패킷
//#define T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO	(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO)	// F->C, // 컬렉션에 표시할 이달의아머 이벤트 정보 패킷

struct MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO
{
	ItemNum_t		CollectionShapeNum;			// 외형 번호
	ItemNum_t		nOptionItemNum;				// 옵션 아이템
	ATUM_DATE_TIME	atEndDate;					// 이벤트 종료 기간
};
// end 2013-04-18 by jhseol,bckim 이달의 아머 - 이달의 아머 변경 사항 전달 패킷

// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 패킷 추가 : 메세지
// 패킷 선언부분
//#define T_FC_COLLECTION_INFO					(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_INFO)			// F->C, // 컬렉션 정보 패킷
//#define T_FC_COLLECTION_SHAPE_CHANGE			(MessageType_t)((T0_FC_COLLECTION<<8)|T1_FC_COLLECTION_SHAPE_CHANGE)	// C->F, // 외형 변경 요청 패킷

// MSG 선언부분
using MSG_FC_COLLECTION_INFO = COLLECTION_INFO;				// 컬렉션 정보 패킷
using MSG_FC_COLLECTION_SHAPE_CHANGE = COLLECTION_SHAPE_CHANGE;		// 외형 변경 요청 패킷

#define GAMELOG_COLLECTION_STATE_ADD			1	// 컬렉션 등록
#define GAMELOG_COLLECTION_STATE_ENCHANT		2	// 컬렉션 인첸트
#define GAMELOG_COLLECTION_STATE_TIME_START		3	// 컬렉션 적용(사용):타이머 ON
#define GAMELOG_COLLECTION_STATE_TIME_END		4	// 컬렉션 해제(사용종료):타이머 OFF
struct MSG_FL_LOG_COLLECTION_STATE
{
	BYTE				CollectionState;
	COLLECTION_INFO		CollectionInfo;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%d|%d(%d)|%d|%d(%s)\r\n", GetGameLogTypeString(i_nLogType), CollectionInfo.CollectionType, CollectionState, CollectionInfo.AccountUID, CollectionInfo.CharacterUID
			, CollectionInfo.ShapeNum, CollectionInfo.ShapeItemNum, CollectionInfo.EnchantLevel, CollectionInfo.RemainSeconds, CollectionInfo.EndTime.GetDateTimeString().GetBuffer());
		return o_szLogString;
	}
};

struct MSG_FL_LOG_COLLECTION_SHAPE_CHANGE
{
	BYTE				CollectionType;
	UID32_t				AccountUID;
	UID32_t				CharacterUID;
	UID64_t				ItemUID;
	ItemNum_t			BeforeShapeNum;
	ItemNum_t			AfterShapeNum;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d|%I64d|%d->%d\r\n", GetGameLogTypeString(i_nLogType), CollectionType, AccountUID, CharacterUID, ItemUID, BeforeShapeNum, AfterShapeNum);
		return o_szLogString;
	}
};
// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 패킷 추가 : 메세지

// 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가
struct MSG_FL_LOG_ACCOUNT_INFL_CHANGE_BY_ADMIN
{
	UID32_t				AccountUID;
	UID32_t				CharacterUID;
	BYTE				BeforeInfl;
	BYTE				AfterInfl;
	UID32_t				AdminCharacterUID;

	char *GetWriteLogString(T1<T0::FL_LOG> i_nLogType, char *o_szLogString)
	{
		sprintf(o_szLogString, "%s|%d|%d|%d->%d|%d\r\n", GetGameLogTypeString(i_nLogType), AccountUID, CharacterUID, BeforeInfl, AfterInfl, AdminCharacterUID);
		return o_szLogString;
	}
};
// end 2013-07-26 by jhseol, 타 계정 세력변경 - GameLog 추가

#include "AtumProtocolX.h"