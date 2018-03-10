// 2006-09-05 by cmkwon
#pragma once
#include "Contents.h"

#include "CompileUtils.h"

///////////////////////////////////////////////////////////////////////////////
// 2015-05-17 Future, Service Type Definitions

///////////////////////////////////////////////////////////////////////////////
//  SERVICE_TYPE_ENGLISH_SERVER_1		==>	캐나다	Wikigames		- Eng	2000
//  SERVICE_TYPE_VIETNAMESE_SERVER_1	==>	베트남	VTC-Intecom		- Viet	4000
//  SERVICE_TYPE_DREAMACE				==>	DreamAce
//  SERVICE_TYPE_ETERNALSKIES			==> Eternal Skies

#ifdef S_CAN_SERVER_SETTING_HSSON
#define SERVICE_TYPE_ENGLISH_SERVER_1
#define SERVICE_UID_FOR_WORLD_RANKING		2000			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define SERVICE_TYPE_VIETNAMESE_SERVER_1
#define SERVICE_UID_FOR_WORLD_RANKING		4000			// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
#endif

#ifdef S_DA_SERVER_SETTING_HSSON
#define SERVICE_TYPE_DREAMACE
#define SERVICE_UID_FOR_WORLD_RANKING		15000			// 2015-05-18 Future, random value, wrk not used atm
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON
#define SERVICE_TYPE_ETERNAL_SKIES
#define SERVICE_UID_FOR_WORLD_RANKING		15000			// 2015-05-18 Future, random value, wrk not used atm
#endif

///////////////////////////////////////////////////////////////////////////////
// Code Page
#ifdef S_VIE_SERVER_SETTING_HSSON
#define CODE_PAGE							1258
#else
#define CODE_PAGE							1252
#endif

///////////////////////////////////////////////////////////////////////////////
// Capacities & Max Level

#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
	#define NEMERIAN
	#ifdef NEMERIAN
		#define NEMERIAN_BASIC_ANTICHEAT
		#define NEMERIAN_INCREASE_HPDP_LIMIT
		//#define NEMERIAN_TEAMSPEAK
		#define NEMERIAN_UPGRADED_MEMBERSHIP
		#define NEMERIAN_NATION_BALANCE_BUFF_PENALITY
		#define NEMERIAN_AUTO_ELECTION_AND_RESET
		#define NEMERIAN_PVP_AWARD_EFFORT
		//#define PARRALELLE_WEAPON_EFFET_PROCESSING 8			// does the weapon effect have to be proccessed in parralelle way or linear way
	#endif
	#define SIZE_MAX_INITIAL_GUILD_CAPACITY				30		// 초기 길드 생성 시 가능 길드원 수, // 2006-09-06 by cmkwon, 한국 서버만 변경 (40-->30)
	#define SIZE_MAX_GUILD_CAPACITY						300		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 최대 길드원 수
	#define SIZE_MAX_ITEM_GENERAL						161		// 캐릭터의 인벤토리에 소유할 수 있는 아이템의 최대 개수(1개는 SPI 아이템의 카운트이다, 클라이언트는 60을 사용한다.), // 2006-09-06 by cmkwon, 한국 서버만 변경(61-->41)
	#define SIZE_MAX_ITEM_GENERAL_IN_STORE				101		// 창고에 소유할 수 있는 아이템의 최대 개수, // 2006-09-06 by cmkwon, 한국 서버만 변경(101-->51)
	#define CHARACTER_MAX_LEVEL							125
	#ifdef NEMERIAN_UPGRADED_MEMBERSHIP
		#define COUNT_IN_MEMBERSHIP_ADDED_INVENTORY			60
		#define COUNT_IN_MEMBERSHIP_ADDED_STORE				100
		#define COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY	100
		#define COUNT_IN_MEMBERSHIP_ADDED_HP				1000
		#define COUNT_IN_MEMBERSHIP_ADDED_DP				1000
		#define COUNT_IN_MEMBERSHIP_ADDED_EP_RATE			2
	#else
		#define COUNT_IN_MEMBERSHIP_ADDED_INVENTORY			40
		#define COUNT_IN_MEMBERSHIP_ADDED_STORE				50
		#define COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY	30
	#endif
	#ifdef NEMERIAN_PVP_AWARD_EFFORT
		#define PVP_AWARD_ASSIST_TIME						10		//time to remove a gear from assist list
		#define PVP_AWARD_REWARD_ITEMNUM					9990002 // item to give on kill
		#define PVP_AWARD_REWRD_AMOUNT_BY_KILL				10		//number of pvp point shared between attackers
	#endif
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define SIZE_MAX_INITIAL_GUILD_CAPACITY				40		// 초기 길드 생성 시 가능 길드원 수, // 2006-09-06 by cmkwon, 한국 서버만 변경 (40-->30)
#define SIZE_MAX_GUILD_CAPACITY						300		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 최대 길드원 수
#define SIZE_MAX_ITEM_GENERAL						61		// 캐릭터의 인벤토리에 소유할 수 있는 아이템의 최대 개수(1개는 SPI 아이템의 카운트이다, 클라이언트는 60을 사용한다.), // 2006-09-06 by cmkwon, 한국 서버만 변경(61-->41)
#define SIZE_MAX_ITEM_GENERAL_IN_STORE				101		// 창고에 소유할 수 있는 아이템의 최대 개수, // 2006-09-06 by cmkwon, 한국 서버만 변경(101-->51)
#define CHARACTER_MAX_LEVEL							125
#define COUNT_IN_MEMBERSHIP_ADDED_INVENTORY			20		// 2006-09-06 by cmkwon, 멥버쉽 서비스시 추가 인베토리 카운트
#define COUNT_IN_MEMBERSHIP_ADDED_STORE				20		// 2006-09-06 by cmkwon, 멥버쉽 서비스시 추가 창고 카운트
#define COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY	20		// 2006-09-06 by cmkwon, 멥버쉽 서비스시 추가 최대여단원 카운트
#endif

////////////////////////////////////////////////////
// Default Stats
#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define STAT_BGEAR_ATTACK_PART						3
#define STAT_BGEAR_DEFENSE_PART						3
#define STAT_BGEAR_FUEL_PART						3
#define STAT_BGEAR_SOUL_PART						3
#define STAT_BGEAR_SHIELD_PART						3
#define STAT_BGEAR_DODGE_PART						3

#define STAT_MGEAR_ATTACK_PART						2
#define STAT_MGEAR_DEFENSE_PART						4
#define STAT_MGEAR_FUEL_PART						3
#define STAT_MGEAR_SOUL_PART						4
#define STAT_MGEAR_SHIELD_PART						3
#define STAT_MGEAR_DODGE_PART						2

#define STAT_AGEAR_ATTACK_PART						4
#define STAT_AGEAR_DEFENSE_PART						3
#define STAT_AGEAR_FUEL_PART						3
#define STAT_AGEAR_SOUL_PART						3
#define STAT_AGEAR_SHIELD_PART						4
#define STAT_AGEAR_DODGE_PART						1

#define STAT_IGEAR_ATTACK_PART						4
#define STAT_IGEAR_DEFENSE_PART						2
#define STAT_IGEAR_FUEL_PART						3
#define STAT_IGEAR_SOUL_PART						3
#define STAT_IGEAR_SHIELD_PART						2
#define STAT_IGEAR_DODGE_PART						4
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define STAT_BGEAR_ATTACK_PART						3
#define STAT_BGEAR_DEFENSE_PART						3
#define STAT_BGEAR_FUEL_PART						3
#define STAT_BGEAR_SOUL_PART						3
#define STAT_BGEAR_SHIELD_PART						3
#define STAT_BGEAR_DODGE_PART						3

#define STAT_MGEAR_ATTACK_PART						2
#define STAT_MGEAR_DEFENSE_PART						4
#define STAT_MGEAR_FUEL_PART						3
#define STAT_MGEAR_SOUL_PART						4
#define STAT_MGEAR_SHIELD_PART						3
#define STAT_MGEAR_DODGE_PART						2

#define STAT_AGEAR_ATTACK_PART						4
#define STAT_AGEAR_DEFENSE_PART						3
#define STAT_AGEAR_FUEL_PART						3
#define STAT_AGEAR_SOUL_PART						3
#define STAT_AGEAR_SHIELD_PART						4
#define STAT_AGEAR_DODGE_PART						1

#define STAT_IGEAR_ATTACK_PART						4
#define STAT_IGEAR_DEFENSE_PART						2
#define STAT_IGEAR_FUEL_PART						3
#define STAT_IGEAR_SOUL_PART						3
#define STAT_IGEAR_SHIELD_PART						2
#define STAT_IGEAR_DODGE_PART						4
#endif

///////////////////////////////////////////////////////////////////////////////
// 2006-09-15 by cmkwon, 
// 국가 소스통합 때 추가 되어야 할 부분 순서 05

#if defined (S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define MSBILLING_DB_SERVER_IP						"127.0.0.1"
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define MSBILLING_DB_SERVER_IP						"127.0.0.1140"
#endif

#define MSBILLING_DB_SERVER_PORT					9979

// Kor_Masang51		==> 1
// Kor_ETRI			==> 1
// Eng_Gala-Net		==> 상관 없음
// Viet_VTC-Intecom	==> 상관 없음
#define MSBILLING_GAMEUID							1

// 2006-09-22 by dhjin
// Kor_Masang51		==> 201			// 2006-10-23 by cmkwon, 변경(101-->201)
// Kor_ETRI			==> 201			// 2006-10-23 by cmkwon, 변경(101-->201)
// Eng_Gala-Net		==> 201			// 2006-10-23 by cmkwon, 변경(101-->201)
// Viet_VTC-Intecom	==> 201			// 2006-10-23 by cmkwon, 변경(101-->201)
#define COUNT_IN_MEMBERSHIP_GUILDSTORE				201		// 2006-09-22 by dhjin, 멤버쉽 서비스시 여단 창고 카운트


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
// Homepage Domain

#define STRMSG_S_GAMEHOMEPAGE_DOMAIN			"http://ao.nemerian.com"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - ,"http://www.ace-onlines.com"

#define STRMSG_REPFB "Please report this error at http://ao.nemerian.com/bugtracker via PM."


///////////////////////////////////////////////////////////////////////////////
// 2006-05-22 by cmkwon, 비번 MD5로 인코딩시에 원래 비번 앞에 추가될 스트링 - 
#define MD5_PASSWORD_ADDITIONAL_STRING			""
#define EXT_AUTH_GAME_NAME						"SCO"		// 2006-05-22 by cmkwon

// 국가 소스통합 때 추가 되어야 할 부분 순서 07

#ifdef S_CAN_SERVER_SETTING_HSSON	// Wikigames
// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - MS140번 서버만 테스트를 위해 IP 를 틀리게(121.134.11.) 설정 함
// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP1		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP2		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP3		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 본섭 Private IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP4		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 본섭 Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP5		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 테섭 Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP6		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP7		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP8		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP9		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP10		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define IS_SCADMINTOOL_CONNECTABLE_IP(ip)		( 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP1,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP1)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP2,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP2)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP3,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP3)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP4,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP4)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP5,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP5)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP6,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP6)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP7,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP7)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP8,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP8)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP9,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP9)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP10,cstrlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP10)) )

///////////////////////////////////////////////////////////////////////////////
// 2006-05-02 by cmkwon, Launcher URL
#define LAUNCHER_WEB_URL						"http://ao.nemerian.com/launcher/"
#define TESTSERVER_LAUNCHER_WEB_URL				"http://ao.nemerian.com/launcher/"


///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
#define BILLING_DBSERVER_DATABASE_NAME			"MS_Billing"
#define BILLING_DBSERVER_USER_ID				"atum"
// Kor_Masang51		==> callweb
// Kor_Yedang		==> 2006-12-25 by cmkwon, 수정함
#define BILLING_DBSERVER_USER_PWD				"callweb"
#endif // S_CAN_SERVER_SETTING_HSSON

#ifdef S_VIE_SERVER_SETTING_HSSON
// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - MS140번 서버만 테스트를 위해 IP 를 틀리게(121.134.11.) 설정 함
// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP1		"127.0.0.1"	// 2007-01-03 by cmkwon, 내부 테섭군
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP2		"127.0.0.1"	// 2007-01-03 by cmkwon, 프로그램팀군
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP3		"127.0.0.1"		// 2008-05-19 by cmkwon, 마상 모두, // 2007-01-03 by cmkwon, 기획,운영팀군
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP4		"127.0.0.1"	// 2008-05-19 by cmkwon, Hoa 요청, // 2007-01-03 by cmkwon, 사장님,본부장님
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP5		"127.0.0.1"	// 2008-04-28 by cmkwon, VTC-Intecom Monitor PC1
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP6		"127.0.0.1"		// 2008-04-28 by cmkwon, VTC-Intecom Monitor PC1(<==222.255.15.252)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP7		"127.0.0.1"			// 2007-03-06 by cmkwon, VTC-Intecom Monitor PC2(delete 203.162.1.223)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP8		"127.0.0.1"	// 2007-01-03 by cmkwon, Masang Remote PC in VTC
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP9		"127.0.0.1"		// 2007-01-03 by cmkwon, VTC Main Pre Server public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP10		"10.10.1."			// 2007-01-03 by cmkwon, VTC Server private IP
#define IS_SCADMINTOOL_CONNECTABLE_IP(ip)		( 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP1,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP1)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP2,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP2)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP3,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP3)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP4,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP4)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP5,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP5)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP6,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP6)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP7,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP7)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP8,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP8)) \
	|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP9,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP9)) \
|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP10,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP10)) )

///////////////////////////////////////////////////////////////////////////////
// 2006-05-02 by cmkwon, Launcher URL
#define LAUNCHER_WEB_URL						"http://aresonline.in/launcher"		// 2007-10-05 by cmkwon, 수정(기존 "http://caoboi.vtc.vn/notice.asp")
#define TESTSERVER_LAUNCHER_WEB_URL				"http://aresonline.in/launcher"		// 2007-10-05 by cmkwon, 수정(기존 "http://caoboi.vtc.vn/notice.asp")


///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
#define BILLING_DBSERVER_DATABASE_NAME			"MS_Billing"
#define BILLING_DBSERVER_USER_ID				"atum"
// Kor_Masang51		==> callweb
// Kor_Yedang		==> 2006-12-25 by cmkwon, 수정함
#define BILLING_DBSERVER_USER_PWD				"callweb"
#endif

#ifdef S_DA_SERVER_SETTING_HSSON	// Wikigames
// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - MS140번 서버만 테스트를 위해 IP 를 틀리게(121.134.11.) 설정 함
// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP1		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP2		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP3		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 본섭 Private IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP4		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 본섭 Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP5		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 테섭 Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP6		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP7		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP8		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP9		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP10		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define IS_SCADMINTOOL_CONNECTABLE_IP(ip)		( 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP1,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP1)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP2,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP2)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP3,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP3)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP4,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP4)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP5,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP5)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP6,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP6)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP7,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP7)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP8,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP8)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP9,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP9)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP10,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP10)) )

///////////////////////////////////////////////////////////////////////////////
// 2006-05-02 by cmkwon, Launcher URL
#define LAUNCHER_WEB_URL						"http://aresonline.in/launcher/"
#define TESTSERVER_LAUNCHER_WEB_URL				"http://aresonline.in/launcher/"


///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
#define BILLING_DBSERVER_DATABASE_NAME			"MS_Billing"
#define BILLING_DBSERVER_USER_ID				"atum"
// Kor_Masang51		==> callweb
// Kor_Yedang		==> 2006-12-25 by cmkwon, 수정함
#define BILLING_DBSERVER_USER_PWD				"callweb"
#endif // S_DA_SERVER_SETTING_HSSON

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Wikigames
// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - MS140번 서버만 테스트를 위해 IP 를 틀리게(121.134.11.) 설정 함
// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP1		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP2		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP3		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 본섭 Private IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP4		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 본섭 Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP5		"127.0.0.1"		// 2008-08-05 by cmkwon, WikiGames_Eng 게임 정보 수정 - 테섭 Public IP
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP6		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP7		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP8		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP9		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define SCADMINTOOL_CONNECTABLE_PREFIX_IP10		"127.0.0.1"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define IS_SCADMINTOOL_CONNECTABLE_IP(ip)		( 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP1,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP1)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP2,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP2)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP3,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP3)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP4,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP4)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP5,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP5)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP6,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP6)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP7,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP7)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP8,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP8)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP9,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP9)) \
												|| 0 == strncmp((ip),SCADMINTOOL_CONNECTABLE_PREFIX_IP10,strlen(SCADMINTOOL_CONNECTABLE_PREFIX_IP10)) )

///////////////////////////////////////////////////////////////////////////////
// 2006-05-02 by cmkwon, Launcher URL
#define LAUNCHER_WEB_URL						"http://COMMING_SOON/launcher/"
#define TESTSERVER_LAUNCHER_WEB_URL				"http://COMMING_SOON/launcher/"


///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
#define BILLING_DBSERVER_DATABASE_NAME			"MS_Billing"
#define BILLING_DBSERVER_USER_ID				"atum"
// Kor_Masang51		==> callweb
// Kor_Yedang		==> 2006-12-25 by cmkwon, 수정함
#define BILLING_DBSERVER_USER_PWD				"callweb"
#endif // S_ETRS_SERVER_SETTING_HSSON

///////////////////////////////////////////////////////////////////////////////
// 2007-02-13 by cmkwon
// 국가 소스통합 때 추가 되어야 할 부분 순서 08

#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define	SG_BOX_FONT_FACENAME						"Tahoma"					// 2007-02-12 by cmkwon, 글씨체
#define	SG_BOX_FONT_CHARSET							ANSI_CHARSET			// 2007-02-12 by cmkwon, 캐릭터셋
#define	SG_BOX_FONT_WEIGHT							FW_BOLD					// 2007-02-12 by cmkwon, 글씨체 두깨
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define	SG_BOX_FONT_FACENAME					"Tahoma"		// 2007-02-12 by cmkwon, 글씨체
#define	SG_BOX_FONT_CHARSET						VIETNAMESE_CHARSET		// 2007-02-12 by cmkwon, 캐릭터셋
#define	SG_BOX_FONT_WEIGHT						FW_BOLD					// 2007-02-12 by cmkwon, 글씨체 두깨
#endif


// 국가 소스통합 때 추가 되어야 할 부분 순서 09
#if defined(S_CAN_SERVER_SETTING_HSSON) && defined(S_ACCESS_INTERNAL_SERVER_HSSON)

#ifdef S_EP4_TEST_SERVER_HSKIM
	#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"4C4C0B04631231243124123123017144D3F0B31184C1D511409"	 //	IP 61.39.170. 238 - // 2013-03-18 by hskim, EP4 전달용 서버 추가
#elif defined NEMERIAN
	#ifdef GUSSLOCAL
		#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F567E4A1D01"	// IP 192.168.10.142 - 캐나다 내부 테섭
	#else
		#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F567E4A1D01"	// IP 192.168.10.142 - 캐나다 내부 테섭
	#endif /* GUSSLOCAL */
#else
	#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"4C4C0B04631231243124123123017144D3F0B31184C1D511409" //authoriseds.dreamace.org
	//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F48664F"	// Local
	//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A1C534F7F4752066D537E4A1D01"	// IP 127.0.0.1142 - 마상 내부 캐나다 테섭
#endif
#ifdef NEMERIAN
	#define REGISTRY_BASE_PATH						"Nemerian"
	#define EXE_1_FILE_NAME							"Nemerian.exe"
	#define LAUNCHER_FILE_NAME						"Launcher.atm"

		// Win XP Executable name. Used for Server only
	#define CLIENT_EXEUTE_FILE_NAME_XP				"AONXP.atm"

		// Determination of Executable name by Client Configuration
	#ifdef _WIN_XP
		#define CLIENT_EXEUTE_FILE_NAME					CLIENT_EXEUTE_FILE_NAME_XP
	#else
		#define CLIENT_EXEUTE_FILE_NAME					"game.dat"
	#endif

	#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, 수정함

	#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#else
	#define REGISTRY_BASE_PATH						"Ares"
	#define EXE_1_FILE_NAME							"Ares.exe"
	#define LAUNCHER_FILE_NAME						"Launcher.atm"
	
	// Win XP Executable name. Used for Server only
	#define CLIENT_EXEUTE_FILE_NAME_XP				"ACEonlin3.atm"

	// Determination of Executable name by Client Configuration
#ifdef _WIN_XP
	#define CLIENT_EXEUTE_FILE_NAME					CLIENT_EXEUTE_FILE_NAME_XP
#else
	#define CLIENT_EXEUTE_FILE_NAME					"game.dat"
#endif
	
	#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, 수정함

	#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#endif // NEMERIAN
#endif // Wikigames

// Dream Ace
#if defined(S_DA_SERVER_SETTING_HSSON) && defined(S_ACCESS_INTERNAL_SERVER_HSSON)
#ifdef S_EP4_TEST_SERVER_HSKIM
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"121848507940481966557E4A1F"	 //	IP 61.39.170. 238 - // 2013-03-18 by hskim, EP4 전달용 서버 추가
#else
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F4869"	// IP 66.207.198.252 - 캐나다 내부 테섭
//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A1C534F7F4752066D537E4A1D01"	// IP 127.0.0.1142 - 마상 내부 캐나다 테섭
#endif

#define REGISTRY_BASE_PATH						"ACEonline"
#define EXE_1_FILE_NAME							"ACEonline.exe"
#define LAUNCHER_FILE_NAME						"Launcher.atm"

// Win XP Executable name. Used for Server only
#define CLIENT_EXEUTE_FILE_NAME_XP				"ACEonlin3.atm"

// Determination of Executable name by Client Configuration
#ifdef _WIN_XP
#define CLIENT_EXEUTE_FILE_NAME					CLIENT_EXEUTE_FILE_NAME_XP
#else
#define CLIENT_EXEUTE_FILE_NAME					"game.dat"
#endif

#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, 수정함

#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#endif // Dream Ace

// Eternal Skies
#if defined(S_ETRS_SERVER_SETTING_HSSON) && defined(S_ACCESS_INTERNAL_SERVER_HSSON)
#ifdef S_EP4_TEST_SERVER_HSKIM
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"121848507940481966557E4A1F"	 //	IP 61.39.170. 238 - // 2013-03-18 by hskim, EP4 전달용 서버 추가
#else
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A14544F7F455E066F4869"	// IP 66.207.198.252 - 캐나다 내부 테섭
//#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A1C534F7F4752066D537E4A1D01"	// IP 127.0.0.1142 - 마상 내부 캐나다 테섭
#endif

#define REGISTRY_BASE_PATH						"EternalSkies"
#define EXE_1_FILE_NAME							"ACEonline.exe"
#define LAUNCHER_FILE_NAME						"Launcher.atm"

// Win XP Executable name. Used for Server only
#define CLIENT_EXEUTE_FILE_NAME_XP				"ACEonlin3.atm"

// Determination of Executable name by Client Configuration
#ifdef _WIN_XP
#define CLIENT_EXEUTE_FILE_NAME					CLIENT_EXEUTE_FILE_NAME_XP
#else
#define CLIENT_EXEUTE_FILE_NAME					"game.dat"
#endif

#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, 수정함

#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#endif // Eternal Skies

#if defined(S_VIE_SERVER_SETTING_HSSON) && defined(S_ACCESS_INTERNAL_SERVER_HSSON)

#ifdef S_EP4_TEST_SERVER_HSKIM
	// 117.103.198.170								==> "1A1C514F7F4355066F5F685518040E"
	#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"4F4C48022F1C0447370D38144754590F0F0F41515A2B"	 // 2012-04-09 by hskim, EP4 전달용 서버 추가
#else
// 2011-08-16 by shcho, 베트남 IP변경됨 - preServer(기존:222.235.15.54 -> 변경:117.103.198.155)
// 222.255.15.54								==>	"191F544F7C4653066F537E4E1D"
// 117.103.198.155								==> "1A1C514F7F4355066F5F685518060B"
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"4F4C48022F1C0447370D38144754590F0F0F41515A2B"
#endif

#define REGISTRY_BASE_PATH						"Ares"
#define EXE_1_FILE_NAME							"Ares.exe"
#define LAUNCHER_FILE_NAME						"Launcher.atm"

#define CLIENT_EXEUTE_FILE_NAME_XP				"PhiDo1.atm"
#define CLIENT_EXEUTE_FILE_NAME					"PhiDoi.atm"
#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, 수정함

#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#elif defined(S_VIE_SERVER_SETTING_HSSON)
// 117.103.194.77					        ==>	"1A1C514F7F4355066F5F64551E04"		// 2011-08-16 by shcho, 베트남 IP변경됨 
// MasangTest(61.39.170.143)				==> "1D1C4852775D571F6E48614F1A"		// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)

// 2012-10-15 by bckim, 베트남 EP4 신서버 빌드 패치요청 관련 정보 추가 업무
// 변경전 : 117.103.194.77 (1A1C514F7F4355066F5F64551E04) --> 변경후 : 117.103.194.68 (1A1C514F7F4355066F5F64551F0B)  
#define CHOICE_PRE_SERVER_IP_OR_DOMAIN_IN_XOR	"1A1C514F7F4355066F5F64551F0B"
// end 2012-10-15 by bckim, 베트남 EP4 신서버 빌드 패치요청 관련 정보 추가 업무 

#define REGISTRY_BASE_PATH						"Ares"
#define EXE_1_FILE_NAME							"Ares.exe"
#define LAUNCHER_FILE_NAME						"Launcher.atm"

#define CLIENT_EXEUTE_FILE_NAME_XP				"PhiDo1.atm"
#define CLIENT_EXEUTE_FILE_NAME					"PhiDoi.atm"
#define URL_REGISTER_PAGE						"reg.asp"				// 2006-04-05 by cmkwon, 수정함

#define WEB_START_REGISTRY_VALUE_NAME			"InstallPath"
#endif



// 국가 소스통합 때 추가 되어야 할 부분 순서 10
#ifdef S_CAN_SERVER_SETTING_HSSON	// Wikigames
#define STRMSG_WINDOW_TEXT							"Nemerian"
#define STRMSG_REG_STRING_CLIENT_VERSION			"NemerianVersion"
#define STRMSG_REG_STRING_REGISTRYKEY_NAME			"Nemerian"
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define STRMSG_WINDOW_TEXT							"PhiDoi"
#define STRMSG_REG_STRING_CLIENT_VERSION			"PhiDoiVersion"
#define STRMSG_REG_STRING_REGISTRYKEY_NAME			"VTC Game"
#endif

#ifdef S_DA_SERVER_SETTING_HSSON	// Dream Ace
#define STRMSG_WINDOW_TEXT							"Ares"
#define STRMSG_REG_STRING_CLIENT_VERSION			"AresVersion"
#define STRMSG_REG_STRING_REGISTRYKEY_NAME			"Ares"
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Eternal Skies
#define STRMSG_WINDOW_TEXT							"Ares"
#define STRMSG_REG_STRING_CLIENT_VERSION			"AresVersion"
#define STRMSG_REG_STRING_REGISTRYKEY_NAME			"Ares"				// 2008-07-31 by cmkwon, Yedang-Global_Eng 를 Wikigames_Eng 로 변경 함 - 
#endif


///////////////////////////////////////////////////////////////////////////////
// 2007-06-27 by cmkwon, 중국 방심취 시스템 수정 - 미성년자 관련
// Kor		- 만 20세
// China	- 만 18세
#define ADULT_YEARS									20			// 2007-06-29 by cmkwon,

///////////////////////////////////////////////////////////////////////////////
// 2007-07-06 by cmkwon, SCAdminTool에서 OnlyServerAdmin관련 수정 - 계정 정보
#define SCADMINTOOL_ONLY_SERVER_ADMIN_ACCOUNT_NAME		"SC_moniter"
#define SCADMINTOOL_ONLY_SERVER_ADMIN_PASSWORD			"cowboyWkd"

///////////////////////////////////////////////////////////////////////////////
// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 상위 URL, 우측 하단 URL
#define EXE1_URL_1										"http://notice.aceonline.com.cn/ace2.htm"
#define EXE1_URL_2										"http://notice.aceonline.com.cn/ace1.htm"


///////////////////////////////////////////////////////////////////////////////
// 2008-12-19 by cmkwon, 한국 Yedang 핵쉴드 모니터링 서버 설정 추가 - IP가 ""로 설정되면 모니터링 서버를 사용하지 않는 것임, 현재는 Masang140과 Yedang만 사용 할 것임
// 국가 소스통합 때 추가 되어야 할 부분 순서 11

#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define GAME_GUARD_MONITORING_SERVER_IP					"127.0.0.1"
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define GAME_GUARD_MONITORING_SERVER_IP					""
#endif



///////////////////////////////////////////////////////////////////////////////
// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 월드랭킹 DB 서버 정보
// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
// 국가 소스통합 때 추가 되어야 할 부분 순서 12

#ifdef S_CAN_SERVER_SETTING_HSSON	// Wikigames
#define WRK_DBSERVER_IP							"127.0.0.1"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define WRK_DBSERVER_PORT						9911
#define WRK_DBSERVER_DATABASE_NAME				"atum2_db_1"
#define WRK_DBSERVER_ID							"atum"
#define WRK_DBSERVER_PWD						"efinos76/*"
#define WRK_DBSERVER_IP_FOR_TEST_SERVER			"127.0.0.1"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#define WRK_DBSERVER_PORT_FOR_TEST_SERVER		1433					// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define WRK_DBSERVER_IP							"127.0.0.1131"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define WRK_DBSERVER_PORT						1433
#define WRK_DBSERVER_DATABASE_NAME				"atum2_db_WorldRanking"
#define WRK_DBSERVER_ID							"atum"
#define WRK_DBSERVER_PWD						"callweb"
#define WRK_DBSERVER_IP_FOR_TEST_SERVER			"127.0.0.1140"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#define WRK_DBSERVER_PORT_FOR_TEST_SERVER		9979					// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#endif

#ifdef S_DA_SERVER_SETTING_HSSON	// Dream Ace
#define WRK_DBSERVER_IP							"127.0.0.1"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define WRK_DBSERVER_PORT						9911
#define WRK_DBSERVER_DATABASE_NAME				"atum2_db_1"
#define WRK_DBSERVER_ID							"atum"
#define WRK_DBSERVER_PWD						"efinos76/*"
#define WRK_DBSERVER_IP_FOR_TEST_SERVER			"127.0.0.1"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#define WRK_DBSERVER_PORT_FOR_TEST_SERVER		1433					// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Eternal Skies
#define WRK_DBSERVER_IP							"127.0.0.1"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)
#define WRK_DBSERVER_PORT						9911
#define WRK_DBSERVER_DATABASE_NAME				"atum2_db_1"
#define WRK_DBSERVER_ID							"atum"
#define WRK_DBSERVER_PWD						"efinos76/*"
#define WRK_DBSERVER_IP_FOR_TEST_SERVER			"127.0.0.1"				// 2009-12-28 by cmkwon, 마상회사 IP 변경 - 기존(127.0.0.1)// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#define WRK_DBSERVER_PORT_FOR_TEST_SERVER		1433					// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 
#endif

///////////////////////////////////////////////////////////////////////////////
// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
#define MAX_INFLUENCE_PERCENT			53		// 최대 5% 차이까지만 세력 선택이 가능한다.


///////////////////////////////////////////////////////////////////////////////
// 2009-05-12 by cmkwon, (일본요청) 일본만 전진 기지전 주기 7일로 수정 - 전진기지전 주기 서비스별로 다르게 설정
// 국가 소스통합 때 추가 되어야 할 부분 순서 13
#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
#define	OUTPOST_NEXTWARGAP				7
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define	OUTPOST_NEXTWARGAP				5
#endif


///////////////////////////////////////////////////////////////////////////////
// 2009-07-08 by cmkwon, 전쟁 관련 정의 위치 이동(LocalizationDefineCommon.h) - 
// 국가 소스통합 때 추가 되어야 할 부분 순서 14

#ifdef S_CAN_SERVER_SETTING_HSSON	// Wikigames
#define	OUTPOST_WARTIME					120			// 전진기지전 진행 시간			==> 마상 10분, 나머지는 120분
#define OUTPOST_WARTIME_FOR_TESTSERVER	60			// 테섭은 전진기지전 진행 시간	==> 마상 10분, 나머지는 60분
#define PAY_MINIMUN_COUNT				10			// 모선전,전진기지전,거점전 개인 보상을 위한 최소 인원수 ==> 마상 1명, 나머지는 10명
#endif

#ifdef S_VIE_SERVER_SETTING_HSSON
#define	OUTPOST_WARTIME					90			// 전진기지전 진행 시간			==> 마상 10분, 나머지는 120분
#define OUTPOST_WARTIME_FOR_TESTSERVER	90			// 테섭은 전진기지전 진행 시간	==> 마상 10분, 나머지는 60분
#define PAY_MINIMUN_COUNT				10			// 모선전,전진기지전,거점전 개인 보상을 위한 최소 인원수 ==> 마상 1명, 나머지는 10명
#endif

#ifdef S_DA_SERVER_SETTING_HSSON	// Dream Ace
#define	OUTPOST_WARTIME					60			// 전진기지전 진행 시간			==> 마상 10분, 나머지는 120분
#define OUTPOST_WARTIME_FOR_TESTSERVER	60			// 테섭은 전진기지전 진행 시간	==> 마상 10분, 나머지는 60분
#define PAY_MINIMUN_COUNT				10			// 모선전,전진기지전,거점전 개인 보상을 위한 최소 인원수 ==> 마상 1명, 나머지는 10명
#endif

#ifdef S_ETRS_SERVER_SETTING_HSSON	// Eternal Skies
#define	OUTPOST_WARTIME					60			// 전진기지전 진행 시간			==> 마상 10분, 나머지는 120분
#define OUTPOST_WARTIME_FOR_TESTSERVER	60			// 테섭은 전진기지전 진행 시간	==> 마상 10분, 나머지는 60분
#define PAY_MINIMUN_COUNT				10			// 모선전,전진기지전,거점전 개인 보상을 위한 최소 인원수 ==> 마상 1명, 나머지는 10명
#endif

///////////////////////////////////////////////////////////////////////////////
// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
#define SIZE_MAX_ADDABLE_INVENTORY_COUNT		50		// 기본과 프리미엄을 제외한 추가로 가능한 최대 인벤 추가 개수
#define SIZE_MAX_ADDABLE_STORE_COUNT			50		// 기본과 프리미엄을 제외한 추가로 가능한 최대 창고 추가 개수


////////////////////////////////////////////////////////////////////////////////
// 컨텐츠 버젼 관리용 디파인. by hsLee. 
#define __CONTENTS_SHOW_INFINITY_DIFFICULTY_EDIT_WND__		// 인피니티 난이도 조정 UI 보이기.	2010. 07. 27. by hsLee.
////////////////////////////////////////////////////////////////////////////////