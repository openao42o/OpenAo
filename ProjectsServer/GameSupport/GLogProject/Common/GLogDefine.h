#pragma once

#define		GLOG_DATA_MAX_CNT				10010

#define		GLOG_UPDATE_TIME_MSEC			600000 // 1000 * 60 * 10 (600000): 10분
//#define		GLOG_UPDATE_TIME_MSEC			10000 // 1000 * 10 (10000): 10초

#define		GLOG_SOURCE_DB_CONNECT			L"Provider=SQLOLEDB.1;Persist Security Info=False;User ID=%s ;pwd=%s; Initial Catalog=GLog;Data Source=%s;Network Address=%s,%s; Network Library=dbmssocn"
//#define		GLOG_DEST_DB_CONNECT			L"Provider=SQLOLEDB.1;Persist Security Info=False;User ID=Glog ;pwd=wlfhrmqlalfqjsgh; Initial Catalog=OCSA;Data Source=61.39.170.131;Network Address=61.39.170.131,1433; Network Library=dbmssocn"		// 2011-12-12 by hskim, GLog 2차

// start 2012-10-08 bckim, GLog 3차 추가

// TEST_DEST_DATABASE
//#define		GLOG_DEST_DB_CONNECT			L"Provider=SQLOLEDB.1;Persist Security Info=False;User ID=atum ;pwd=callweb; Initial Catalog=GLOG_AO;Data Source=115.144.35.22;Network Address=115.144.35.22,9979; Network Library=dbmssocn"

// INNER_DEST_DATABASE
//#define		GLOG_DEST_DB_CONNECT			L"Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;pwd=dpdltma^7*41; Initial Catalog=GLOG_AO;Data Source=115.144.35.82;Network Address=115.144.35.82,9979; Network Library=dbmssocn"
																											  
// OUTER_DEST_DATABASE
#define		GLOG_DEST_DB_CONNECT			L"Provider=SQLOLEDB.1;Persist Security Info=False;User ID=glog_master ;pwd=dnpq34tjqltm() ; Initial Catalog=GLOG_AO;Data Source=192.168.4.14;Network Address=192.168.4.14,9979; Network Library=dbmssocn"	

// end 2012-10-08 bckim, GLog 3차 추가



// For ACEOnline
#define		FILE_PATH_SERVER_INFO_UNI		L"../../config/global.cfg"	// L"../Config/Global.cfg"
#define		FILE_PATH_SERVER_INFO_MBC		"../../config/Global.cfg"	//"../Config/Global.cfg" // ".\..\..\config\Global.cfg"

// start 2011-12-12 by hskim, GLog 2차
#define		FILE_PATH_CONFIG_ROOT_UNI		L"./config_root.cfg"
#define		FILE_NAME_GLOBAL_CFG_UNI		L"./global.cfg"		
// end 2011-12-12 by hskim, GLog 2차

#define		STR_XOR_KEY_STRING_DB_ID_PWD	"@34ns%<<fdsa(Tflsd!sndsa^#)fndsla$nvsa$fndsla&nfdsJak(fnldsa!#F"	// 2008-09-01 by cmkwon, global.cfg 파일에 DB ID/PWD 암호화 - 
#define		SOURCE_DB_ID					"$LOG_SERVER_GROUP_ODBC_UID"
#define		SOURCE_DB_PASSWORD				"$LOG_SERVER_GROUP_ODBC_PASSWD"
#define		SOURCE_DB_IP					"$LOG_SERVER_GROUP_DB_SERVER_IP"
#define		SOURCE_DB_PORT					"$LOG_SERVER_GROUP_DB_SERVER_PORT"
// For ACEOnline End

#define		GLOG_APP_NAME					L"GLogClient"


#define		SIZE_MAX_DB_USER_ID				128
#define		SIZE_MAX_DB_USER_PWD			128
#define		SIZE_MAX_ODBC_CONN_STRING		128

// DB 보다 1 더 높게 잡는다.
#define		SIZE_MAX_ACCOUNT_NAME			128
#define		SIZE_MAX_GAME_PUBLISHER_NAME	128
#define		SIZE_MAX_GAME_NAME				128
#define		SIZE_MAX_GAME_SERVER_IP			64
#define		SIZE_MAX_BIRTHDAY				64
#define		SIZE_MAX_CHARACTER_NAME			128
#define		SIZE_MAX_ITEM_NAME				128
#define		SIZE_MAX_GAME_SERVER_NAME		128
#define		SIZE_MAX_RACE_NAME				128
#define		SIZE_MAX_CLASS_NAME				128
#define		SIZE_MAX_MOST_STAYED_IN_ZONE_NAME	128
#define		SIZE_MAX_ZONE_NAME				128
#define		SIZE_MAX_DESCRIPTION			512

///////////////////////////////////////////////////////////////////////////////
// DB 테이블 추가시 추가되어야 되는곳!!!!!!!!!!!!!!!!!!!

class CAccountData
{
public:
	CAccountData(void) {util::zero(this, sizeof(CAccountData));}
	~CAccountData(void) {}

	INT64			UID;					
	TCHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];					// 계정 이름      	
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];		// 게임서비스사 이름     
	TCHAR			GameName[SIZE_MAX_GAME_NAME];						// 게임 이름                                            
	TCHAR			GameServerIP[SIZE_MAX_GAME_SERVER_IP];				// 게임서버IP (서비스지역 표시를 위해(ex:한국,미국등등))
	TCHAR			Birthday[SIZE_MAX_BIRTHDAY];						// 유저 생년월일                                        
	bool			Sex;												// 유저 성별, 0 : 남자, 1 : 여자                    
	COleDateTime	RegDate;											// 계정 생성 날짜
};

class CBuyCashItemData
{
public:
	CBuyCashItemData(void)	{util::zero(this,sizeof(CBuyCashItemData));}
	~CBuyCashItemData(void) {}

	INT64			UID;	
	TCHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];					//-- 계정 이름
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];		//-- 게임서비스사 이름
	TCHAR			GameName[SIZE_MAX_GAME_NAME];						//-- 게임 이름
	TCHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];				//-- 구매한 케릭터 이름
	INT				CharacterLevel;										//-- 구매한 케릭터 레벨
	TCHAR			ItemName[SIZE_MAX_ITEM_NAME];						//-- 구매한 아이템 이름
	INT				CashCost;											//-- 구매한 아이템 개당 유료 가격
	INT				ItemCount;											//-- 구매한 아이템 수
	COleDateTime	BuyDate;											//-- 구매 날짜
};

class CCharacterPlayTimeData
{
public:
	CCharacterPlayTimeData(void)	{util::zero(this,sizeof(CCharacterPlayTimeData));}
	~CCharacterPlayTimeData(void)	{}

	INT64			UID;					
	TCHAR			AccountName[SIZE_MAX_ACCOUNT_NAME];						//-- 계정 이름
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];			//-- 게임서비스사 이름
	TCHAR			GameName[SIZE_MAX_GAME_NAME];							//-- 게임 이름
	TCHAR			GameServerName[SIZE_MAX_GAME_SERVER_NAME];				//-- 게임서버 이름
	TCHAR			CharacterName[SIZE_MAX_CHARACTER_NAME];					//-- 종료한 케릭터 이름
	COleDateTime	LoginDate;												//-- 케릭터 접속 날짜
	COleDateTime	LogoutDate;												//-- 케릭터 종료 날짜
	INT64			TotalPlayTime;											//-- 케릭터 생성일부터 지금까지 플레이 시간
	INT				PlayExp;												//-- 케릭터 접속부터 종료까지 획득 경험치
	INT64			TotalExp;												//-- 케릭터 생성일부터 지금까지 획득 경험치
	TCHAR			Race[SIZE_MAX_RACE_NAME];								//-- 케릭터 종족
	TCHAR			Class[SIZE_MAX_CLASS_NAME];								//-- 케릭터 종류
	INT				Level;													//-- 케릭터 종료 시 레벨
	TCHAR			MostStayedInZoneName[SIZE_MAX_MOST_STAYED_IN_ZONE_NAME];	//-- 케릭터가 접속하여 종료시까지 제일 많이 머물렀던 지역 이름
};

class CConnectTotalUserCountData
{
public:
	CConnectTotalUserCountData(void)	{util::zero(this,sizeof(CConnectTotalUserCountData));}
	~CConnectTotalUserCountData(void)	{}

	INT64			UID;
	COleDateTime	Date;													//-- 저장 시간
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];			//-- 게임서비스사 이름
	TCHAR			GameName[SIZE_MAX_GAME_NAME];							//-- 게임 이름
	TCHAR			GameServerName[SIZE_MAX_GAME_SERVER_NAME];				//-- 게임서버 이름
	INT				TotalUserCount;											//-- 최대 동접자 수

};

class CConnectZoneUserCountData
{
public:
	CConnectZoneUserCountData(void)			{util::zero(this,sizeof(CConnectZoneUserCountData));}
	~CConnectZoneUserCountData(void)		{}

	INT64			UID;
	COleDateTime	Date;													//-- 저장 시간
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];			//-- 게임서비스사 이름
	TCHAR			GameName[SIZE_MAX_GAME_NAME];							//-- 게임 이름
	TCHAR			GameServerName[SIZE_MAX_GAME_SERVER_NAME];				//-- 게임서버 이름
	TCHAR			ZoneName[SIZE_MAX_ZONE_NAME];							//-- 게임 지역 이름
	INT				ZoneUserCount;											//-- 해당 게임 지역에 최대 동접자 수
};

class CEventData
{
public:
	CEventData(void)			{util::zero(this,sizeof(CEventData));}
	~CEventData(void)			{}

	INT64			UID;
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];			//-- 게임서비스사 이름 
	TCHAR			GameName[SIZE_MAX_GAME_NAME];							//-- 게임 이름
	TCHAR			GameServerName[SIZE_MAX_GAME_SERVER_NAME];				//-- 게임서버 이름     
	COleDateTime	StartDate;												//-- 이벤트 시작 날짜
	COleDateTime	EndDate;												//-- 이벤트 종료 날짜
	TCHAR			Description[SIZE_MAX_DESCRIPTION];						//-- 이벤트 설명
};

class CEventParticipationRateData
{
public:
	CEventParticipationRateData(void)			{util::zero(this,sizeof(CEventParticipationRateData));}
	~CEventParticipationRateData(void)			{}

	INT64			UID;
	TCHAR			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];			//-- 게임서비스사 이름 
	TCHAR			GameName[SIZE_MAX_GAME_NAME];							//-- 게임 이름
	TCHAR			GameServerName[SIZE_MAX_GAME_SERVER_NAME];				//-- 게임서버 이름     
	COleDateTime	StartDate;												//-- 이벤트 시작 날짜
	COleDateTime	EndDate;												//-- 이벤트 종료 날짜
	INT				ParticipationCount;										//-- 이벤트 참여 총인원수
	SHORT			ParticipationRate;										//-- 이벤트 참여율 [0%~100%]
	TCHAR			Description[SIZE_MAX_DESCRIPTION];						//-- 이벤트 설명
};

// start 2011-12-12 by hskim, GLog 2차
#define	SIZE_MAX_USER_ID				256
#define	SIZE_MAX_CHAR_ONE				2
#define	SIZE_MAX_IP_ADDRESS				21
#define SIZE_MAX_LOCATION				51
#define SIZE_MAX_COUNTRY				51
#define SIZE_MAX_PUBLISHER				101
#define SIZE_MAX_GAME_SERVER_INFO		101
#define SIZE_MAX_GOOD_NAME				129
#define SIZE_MAX_CHAR_NAME				101

class CAceUserData
{
public:
	CAceUserData(void)				{ util::zero(this,sizeof(CAceUserData)); }
	~CAceUserData(void)				{}

	INT64			UID;
	TCHAR			us_id[SIZE_MAX_USER_ID];
	COleDateTime	us_birthday;
	TCHAR			us_sex[SIZE_MAX_CHAR_ONE];
	COleDateTime	us_regdate;
	TCHAR			us_delete[SIZE_MAX_CHAR_ONE];
	TCHAR			us_ip[SIZE_MAX_IP_ADDRESS];
	TCHAR			us_location[SIZE_MAX_LOCATION];
	TCHAR			us_country[SIZE_MAX_COUNTRY];
	TCHAR			us_publisher[SIZE_MAX_PUBLISHER];
	TCHAR			us_gameserver[SIZE_MAX_GAME_SERVER_INFO];
};

class CAceConnectUserData
{
public:
	CAceConnectUserData(void)		{ util::zero(this,sizeof(CAceConnectUserData)); }
	~CAceConnectUserData(void)		{}

	INT64			UID;
	COleDateTime	c_date;
	BYTE			c_world;
	INT				c_unique;
	INT				c_new;
	INT				c_times;
	INT				c_del;
	TCHAR			c_gameserver[SIZE_MAX_GAME_SERVER_INFO];
	TCHAR			c_publisher[SIZE_MAX_PUBLISHER];
};

class CAceItemBuyData
{
public:
	CAceItemBuyData(void)				{ util::zero(this,sizeof(CAceItemBuyData)); }
	~CAceItemBuyData(void)			{}

	INT64			UID;
	COleDateTime	ib_date;
	INT				ib_totalCost;
	INT				ib_CashCost;
	INT				us_no;
	TCHAR			us_id[SIZE_MAX_USER_ID];
	INT				ib_good_id;
	TCHAR			ib_good_name[SIZE_MAX_GOOD_NAME];
	TCHAR			ib_publisher[SIZE_MAX_PUBLISHER];
	TCHAR			ib_charName[SIZE_MAX_CHAR_NAME];
	INT				ib_charLevel;
	INT				ib_itemCount;
	TCHAR			ib_gameserver[SIZE_MAX_GAME_SERVER_INFO];
};

class CAceConcurrentUserData
{
public:
	CAceConcurrentUserData(void)		{ util::zero(this,sizeof(CAceConcurrentUserData)); }
	~CAceConcurrentUserData(void)		{}

	INT64			UID;
	BYTE			cu_world;
	COleDateTime	cu_date;
	INT				cu_count;
	TCHAR			cu_publisher[SIZE_MAX_PUBLISHER];
	TCHAR			cu_gameServer[SIZE_MAX_GAME_SERVER_INFO];
};

class CAceUserPlaytimeData
{
public:
	CAceUserPlaytimeData(void)		{ util::zero(this,sizeof(CAceUserPlaytimeData)); }
	~CAceUserPlaytimeData(void)		{}

	INT64			UID;
	INT				up_id;
	COleDateTime	up_login;
	COleDateTime	up_logout;
	INT				up_time;
	INT				us_no;
	INT				up_charID;
	TCHAR			up_country[SIZE_MAX_CHAR_ONE];
	TCHAR			up_race[SIZE_MAX_CHAR_ONE];
	TCHAR			up_class[SIZE_MAX_CHAR_ONE];
	FLOAT			up_exp;
	TCHAR			up_gameserver[SIZE_MAX_GAME_SERVER_INFO];
	TCHAR			up_publisher[SIZE_MAX_PUBLISHER];
};
// end 2011-12-12 by hskim, GLog 2차

// start 2012-10-08 by bckim, GLog 3차

class CAccountConnectData
{
public:
	CAccountConnectData(void)		{ util::zero(this,sizeof(CAccountConnectData)); }
	~CAccountConnectData(void)		{}

	INT64			Number;
	COleDateTime	LogDate;
	//COleDateTimeSpan LogDate;
	INT				LogType;
	INT64			UserSerial;
	INT64			IP;
	INT				GameServerID;

};

class CCharData
{
public:
	CCharData(void)		{ util::zero(this,sizeof(CCharData)); }
	~CCharData(void)		{}

	INT64			Number;
	COleDateTime	LogDate;
	//COleDateTimeSpan LogDate;
	INT				LogType;
	INT64			UserSerial;

	INT64			CharSerial;
	INT				Class;
	INT				Lv;
	INT64			Exp;
	INT64			GameMoney;
	INT64			Playtime;
	INT				GameServerID;
};

class CItemLog
{
public:
	CItemLog(void)		{ util::zero(this,sizeof(CItemLog)); }
	~CItemLog(void)		{}

	INT64			Number;
	COleDateTime	LogDate;
	INT				LogType;
	INT64			ItemSerial;
	INT64			Count1;

	INT64			Count2;
	INT64			CharSerial;
	INT64			SCharSerial;
	INT				GameServerID;
};

class CItemStateLog
{
public:
	CItemStateLog(void)		{ util::zero(this,sizeof(CItemStateLog)); }
	~CItemStateLog(void)		{}

	INT64			Number;
	COleDateTime	LogDate;
	INT				LogType;
	INT64			ItemSerial;
	INT64			Count;

	INT64			UserSerial;
	INT64			CharSerial;
	INT				GameServerID;

};

class CServerLog
{
public:
	CServerLog(void)		{ util::zero(this,sizeof(CServerLog)); }
	~CServerLog(void)		{}

	INT64			Number;
	COleDateTime	LogDate;	// smalldatetime 
	//COleDateTimeSpan LogDate;  //datetime
	INT				LogType;
	INT				GameServerID;

};

// end 2012-10-08 by bckim, GLog 3차
