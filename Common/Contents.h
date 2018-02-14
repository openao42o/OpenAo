//Copyright[2002] MasangSoft
#ifndef COMMON_CONTENTS_H_
#define COMMON_CONTENTS_H_
#pragma once

// BUILD CONFIGURATION SETTINGS

//#define _NO_HACKSHIELD
//#define COLLCHECKLAG
#define FPSIMPROVEMENTS
//#define ARESBETA
//#define SERVERCONSOLE

#ifdef _DEBUG

#define CUSTOM_OPTIMIZER_HSSON					// ini 파일 읽어서 게임에 적용
#define MULTI_LOADER_HSSON						// 클라이언트를 여러개 띄울 수 있도록 수정
#define GAMEGUARD_NOT_EXECUTE_HSSON				// 게임 가드 실행 안함
#define _ATUM_DEVELOP							// 서버용) 서버소스상 디버깅일 때 활성화 해줘야 함

#endif


#define S_CAN_SERVER_SETTING_HSSON					// 접속할 서버, 파일명 등 국가별 세팅
#define S_ARARIO_HSSON


#include "Str_CAN/StringDefineCommon.h"
#include "Str_CAN/StringDefineServer.h"
#include "Str_CAN/StringDefineClient.h"

#define GUILD_WAREHOUSE_ACCESS_HSSON			// 멤버쉽이 아니더라도 여단창고 이용가능
#define NEW_CASHSHOP_INTERFACE_HSSON			// 신규 캐쉬샵 작업
#define KOR_CASHSHOP_REFILL_WEB_HSSON			// 캐쉬샵 충전 웹페이지 추가
#define WAREHOUSE_SHARES_HSSON					// 캐릭터 간에 창고를 공유 할 수 있음
#define SC_GROWING_PARTNER_HSKIM_JHAHN			// 캐릭터 정보 창에서 파트너UI 활성화
#define S_INFINITY3_HSKIM						// 인피니티 3차 적용 (서버)
#define INFI_QUEST_JSKIM					    // 인피니티 퀘스트
#define MULTI_TARGET_JSKIM						// 멀티타켓

#define S_LOGIN_ITEM_EVENT_SHCHO			    // 로그인시 아이템 증정
#define S_EP4_HSKIM								// EP4 1차 (서버)
#define C_EPSODE4_UI_CHANGE_JSKIM				// 2011. 11. 17 by jskim EP4 UI 변경
#define C_EPSODE4_SETTING_JHAHN					// 2011. 11. 17 by jhahn EP4 트리거 시스템 
#define S_STATISTICS_HSKIM						// 화폐 통계
#define S_DELETE_DROP_ITEM_HSKIM				// 드랍된 아이템 5분뒤에 삭제
#define S_ARENA_EX_1ST_JHSEOL					// 2012-05-29 by jhseol, 아레나 추가개발 - 맵추가, 자동파티, 편대기능 강화, 판정기능 강화, 보상횟수 제한, ti_arenainfo 필드추가
#define C_ARENA_EX_1ST_MSPARK					// 2012-05-29 by mspark, 아레나 추가개발 - 맵 이미지 추가, 이미지 좌표 변경, 툴팁 추가
#define C_UI_REPAIR_ISSHIN						// 2012-06-21 by isshin UI버그 수정 - Ep4만 적용
#define S_ENEMY_INFO_JHSEOL						// 2012-07-19 by jhseol, 아템미리보기
#define C_ENEMY_INFO_ISSHIN						// 2012-06-14 by isshin 아템미리보기
#define S_TIMEITEM_LOG_HIDE_JHSEOL				// 2012-08-28 by jhseol, 시간제 아이템 관련 로그 삭제 요청

// 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
// Google translate: Renewal of War System
// True to use the "renewed war system".
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL true 

// 2013-02-26 by jhseol, 전쟁 시스템 리뉴얼 부가옵션 - 모선전이 있는 날에도 거점 생성이 가능
// Google translate: War system renewal option - Possibility to create a base on the day of the demonstration.
// Changes how the server determines if it is a valid time to spawn a SP.
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUB_OPTION_JHSEOL true

// 2013-03-18 by jhseol, 거점 생성시간 수정 - 거점 생성시간이 거점전 시작시간에 몰리는 현상을 수정하여 균등하게 분산되도록 함.
// Google translate: Fixed point creation time - Fixed the fact that the point creation time is gathered at the base start time, so that it is evenly distributed.
// True to add some randomness to the spawn times of SPs.
#define S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_SUMMON_TIME_VARIANCE_JHSEOL true

//#define S_SEARCHEYE_SHUTTLE_POSITION_JHSEOL		// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
//#define C_SEARCHEYE_SHUTTLE_POSITION_MSPARK		// 2012-11-29 by mspark, 서치아이 위치가 이동하는 캐릭터의 위치와 동일하도록 수정
#define S_KOR_TEST_MSWAR_BACK_TO_CITY_HSKIM		// 2012-12-05 by hskim, 모선전 시작시에 상대 세력 도시맵으로 워프 - 한국 먼저 테스트 후 전국가 반영을 위해
#define _EXTENDED_CHATCOLOR true
//#define _STAFF_WARP_PERMISSIONS
#define _SHOW_INFLUENCE_ON_GMNATION
#define _SHOW_SPEAKER_IN_CHAT
#define _REWORKED_COLORSHOP

#ifdef _REWORKED_COLORSHOP
#define DEFAULT_COLOR_PRICE	50000     // 2015-06-25 by St0rmy, in case that the coloritem is missing in SSQL Table, the user won't get DC'd when SHOP_PRICES_PER_BUILDING_NPC is defined
#define COLOR_RESET_ITEMNUM 8000000  // This item resets the colorcode to 0 when bought in colorshop (needs to have kind 60)
#endif

/////////////////////////////////////////////////////
// Cross Nation Chat
#define _CROSS_NATION_CHAT
#define _CROSS_NATION_CHAT_SPI_COST	50000

#define _REMEMBER_CHAT_COLOR true

//#define _NCP_BASED_MSWAR
#define _ARENA_WP_HH
#define _SHOW_LATENCY

//gives special buffs to outpost winners
//#define _OUTPOST_BUFFS

#define _ARENA_ADDITIONAL_SUPPLY_ITEMS

//#define _PARTY_LIMIT_MG_COUNT
#ifdef _PARTY_LIMIT_MG_COUNT
#define PARTY_LIMIT_COUNT	2
#endif

//#define C_RENDER_PARTY_MG_COUNT

#ifdef _ATUM_CLIENT
#define C_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL
//#define MULTI_LOADER_HSSON
#define RenderDbg_Release
#define _WIREFRAME
#define _NOCLIP
#define _DRAW_EVENTS
#define _CRASH_HANDLER
#define _ENHANCED_EXCEPTIONS
#define _HIDE_PERCENTAGE_ON_MAXLEVEL
//#define _ADVANCED_CONFIG
#define _DISABLE_BSTOP_AUTOHORIZONT
#define _ENHANCED_MIXING_DISPLAY
#define _SHOW_GUILD_NAME
#define _SHOW_LEADER_INFO //Need _SHOW_GUILD_NAME defined to work


//#define _WARRIOR_GIVE_PLAYTIME_ITEM
//#define _WARRIOR_CLASS_KILL_BAR //additional bar with showing current class and percent for next class
#define _WARRIOR_SHOW_SPHP_4ALL
#define _WARRIOR_ADDITIONAL_INFO //show additional info near aim (hp and ammo status)

#endif

#ifndef _DEBUG
//#define _WARRIOR_ENABLE_OLD_ANTICHEAT
//#define _WARRIOR_ENABLE_VMVARE_CHECK
//#define _WARRIOR_ENABLE_DL_HACKSHIELD
// todo : check what ill do with these anticheats
//#define _WARRIOR_ENABLE_NEW_ANTICHEAT
#endif

#define S_MINI_DUMP_HSKIM

//#ifdef _ATUM_SERVER
//#define _AUTO_DGA
//#define _NATION_BUFFS
//#endif

#define S_ACCESS_INTERNAL_SERVER_HSSON


//#define S_GLOG_HSSON


#if 0
#define S_ARENA_NOT_INFO						// 아레나에서는 할 수 없게 함
#endif

//#define SC_SHUT_DOWNMIN_SHCHO
//#define S_GM_COMMAND_USE_SHCHO

#ifdef _ATUM_CLIENT

#define	S_TRIGGER_OUTPOST_JHSEOL	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
//#define	S_SKILL_NUMBERING_RENEWAL_JHSEOL	// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정 - 서버용
//#define	C_SKILL_NUMBERING_RENEWAL_BHSOHN	// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정 - 클라용

#define S_TRIGGER_SYSTEM_EX					// 2013-07-08 by jhseol, 트리거 시스템 확장 - 기존 트리거에 전진기지 트리거의 제어기 기능을 추가
#endif

#if 0
#define S_MANAGER_ADMIN_HSSON					// 게임 마스터도 계정 블럭/언블럭 시킬 수 있도록 수정
#endif

// 2013-07-08 게임포지 추가
//#define S_HACK_SHIELD_AUTO_UPDATE_HSKIM			// 핵쉴드 자동 업데이트 사용

#define S_LAUNCHER_USE_ID_PASSWORD_HSKIM

//#define S_ADMIN_CHARACTER_LOGIN_IP_CHECK		// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 특수계정은 등록된 아이피가 아니면 접속 차단

//#define C_OBJECT_RESOURCE_ORGANIZE_MSPARK		// 2012-04-18 by mspark, 오브젝트 리소스 정리
//#define C_INTERFACE_RESOURCE_ORGANIZE_ISSHIN	// 2012-04-23 by isshin, 인터페이스 리소스 정리

// 유저 스크린샷 모드 지원 (means: "Support for user screen shot mode")
#define C_USER_SCREENSHOTMODE_ISSHIN true
// True to ignore Ctrl key
#define C_USER_CTRL_KEY_LOCK false
// True to enable `g_pSOption->sTerrainEffectRender` option (otherwise it is ignored)
#define C_USER_EFFECT_CONTROL false
// True to give item 7037060 "Glory of Influence" to everyone after ms wars
#define S_MS_WAR_FIN_SUPPLY_ITEM_ADD true

//#define S_FULL_LOG_JHSEOL

#define CHECK_SUM_ON

//#define S_DISABLE_CHECKSUM_RESOURCE
//#define S_OPTION_PARTNER__JHAHN

// ep4-2차
#define S_BONUS_KILL_SYSTEM_RENEWAL false // 2015-06-11 by Future, Bonus System by Kills

#define S_BONUSEXPSYSTEM_RENEWAL false // Counters by Mob Kills (3600 & 300)


#define S_ADDITIONAL_EXPERIENCE			// 2012-11-12 by hskim, 휴식 경험치
#define C_WARNING_APPEARANCE_CHANGE  // 2012-10-30 by mspark, 외형 변경 경고 메시지 추가

#if defined(DEACTICATION_ALL)				// 모든국가 사용안함
#define S_ARENA_EX_1ST_RESTORE_JHSEOL		// 2012-09-14 by jhseol, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#define C_ARENA_EX_1ST_RESTORE_MSPARK		// 2012-09-14 by mspark, 아레나 추가개발 restore - 리뉴얼 적용 상태에서 자신의 케릭터로만 플레이 하도록 수정
#endif

//#define SC_SHUT_DOWNMIN_JHAHN

#define C_CANADA_HACKSHEILD_JHAHN // 2012-09-17 by jhahn	캐나다 핵쉴드 제거 버전

#ifdef DEVX
#define S_ITEM_VIEW_UNCONDITIONAL_ON		// 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
#define C_ITEM_VIEW_UNCONDITIONAL_ON		// 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
#endif

#ifdef _ATUM_CLIENT
// S_SERVER_CRASH_MSGBOX_REMOVE_MSPARK 와 조합해서 사용이 되면, 만일 지속적인 에러가 발생시 라이브 서버에
// 하드 디스크가 바닥나는 상황도 발생 할 수 있음
// 메시지 박스가 안보이고 파일을 각각 다르게 생성하는 조합을 해당 퍼블리셔에 전달할때에는
// 예상되는 문제점에 대해 꼭 알려 주세요 (파일을 주기적으로 확인 해야한다는것을...)
#define S_SERVER_CRASH_FILENAME_ADD_INFO	// 2013-05-28 by hskim, 미니 덤프 파일 이름에 시간 추가
#endif


#if 0
#define C_CLIENT_LIVE_TIME // 2012-11-07 by bhsohn 베트남 패킷 처리
#endif

//#define S_KOR_BUSAN_MONITORTOOL_MAKE

// 2013-06-25 캐나다 선물기능 활성화 요청으로 인한 컨텐츠 주석처리
//#if defined(WIKIGAMES_ENG) || defined(DEVX)
//#define S_DELETE_BUY_CASH_GIVE				// 2012-11-12 by hskim, 캐나다 선물하기 기능 삭제
//#define C_CASHSHOP_IMAGE_REMOVE // 2012-11-16 by mspark, 캐나다 선물, 충전 기능 제거
//#endif

// True to disable prepare shutdown cmd
#define S_DISABLE_PREPARE_SHUTDOWN true

//#define S_GLOG_3ND_KHK
//#define S_KOR_TEST_GLOG_CHARDATA

#define S_AUTO_BLOCK_SYSTEM_HSKIM				// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
// 2013-04-12 일본적용 // 2013-04-29 캐나다적용

#define S_DB_FULL_LOG_JHSEOL		// 2013-01-04 by jhseol, DB 풀로그 남기기

#ifndef INTECOM_VIET
#define C_LONGTIME_WINDOW_ON		// 2012-12-06 by bhsohn 장기간 window 킨후, 게임접속시 게임 멈추어 있는 현상 처리
#endif

#ifdef INTECOM_VIET
#define C_ARENA_PASSWORD_ALPHABET  // 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 
#endif

#ifndef INTECOM_VIET
#define C_MOUSEFOCUS_BACKGROUND_NOTRESET  // 2012-12-17 by jhjang 게임이 백그라운드일때, 마우스 포커스를 초기화 하지 않도록 수정
#endif

#ifndef INTECOM_VIET
#define C_JOIN_CHATROOM_MESSAGE		// 2013-01-08 by mspark, 채팅방에 새로운 유저가 입장했을 경우 시스템 메시지 추가
#endif

#if defined(_ATUM_CLIENT) && !defined(INTECOM_VIET)
#define C_BAZZER_NOT_EXIT // 2013-01-08 by jhjang 개인상점을 연 상태에서 클라이언트 멈춰있을때 강제 종료 막기 추가
#endif

//#define C_FALL_OF_BOOSTER_USE_STOP_MSPARK

#ifndef INTECOM_VIET
#define C_AGEAR_REFINERYSHOP_CLOSE_MSPARK		// 2013-02-04 by mspark, A기어 보급상점에 랜딩 시 이동 버튼을 누를경우, 보급상점 창이 닫히도록 작업
#endif

// 2012-11-23 by jhjang 게임포지 웹 상점 통합 작업
//#define C_WEB_CASHSHOP
//#define S_WEB_CASHSHOP_JHSEOL
#define S_REMANING_PROCESS_REMOVE_MSPARK

#ifndef INTECOM_VIET
#define S_BUGFIX_DUPLICATE_ITEM_HSKIM		// 2013-02-21 by hskim, 베트남 아이템 복사 버그 보안 (DB 처리가 끝날때까지 관련 패킷은 무시) - 삭제 주기를 1시간 (3600000) 에서 26.5시간으로 변경 (90000000)
#endif

//#define S_MONTHL_CASH_BUY_GIFT_JHSEOL				// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
//#define S_ITEM_EVENT_SUB_TYPE_JHSEOL				// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
//// 2013-04-19 한국적용
//
//	#if defined(TEST140) || defined(YEDANG_KOR)
//	// 2013-06-14 한국적용
	#define S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
//	#define C_ITEM_EVENT_RETURN_USER_BHSOHN				// 2013-03-06 by bhsohn 복귀 유저 시스템
//	#endif
//#define S_WEB_CASHSHOP_SERVER_MODULE_HSKIM			// 2013-03-13 by hskim, 웹 캐시 상점
//#define S_DIRECT_DB_ITEM_INSERT_SECURITY_HSKIM		// 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지

// True to allow multiple intances of the server to run at once
#define S_DISABLE_ALONE_RUN_MODE_HSKIM true

// 2013-06-10 한국 추가
//#define S_UPGRADE_INFLUENCE_POINT_HSKIM				// 2013-05-09 by hskim, 세력 포인트 개선
//#define C_UPGRADE_INFLUENCE_POINT_BHSOHN	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템

// 2013-07-09 캐나다, 아르헨, 게임포지 추가	// 2013-07-11 일본 추가 
#define S_ADMINTOOL_ENCHANT_DELETE_MODIFY_BCKIM		// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능

#define S_ADMINTOOL_CASHSHOP_REALIGNMENT_BCKIM		// 2013-02-05 by bckim, 캐시샵 추천탭 정렬순서부여

#ifndef INTECOM_VIET
#define C_ABUSE_FILTER_RELAX	// 2013-04-03 by ssjung 캐나다 욕설 필터 완화
#endif

//#define C_ITEM_EVENT_NOTIFY_MSG_CHANGE_JHSEOL				// 2013-04-08 by jhseol, 이벤트 아이템 지급 알림방법 변경
// 2013-04-19 한국적용

#define SC_PARTNER_SHAPE_CHANGE_HSKIM				// 2012-12-03 by hskim, 파트너 기간제 외형 변경

// 2013-05-23 한국적용
//#define SC_BUFF_PENALTY_JHSEOL_BCKIM					// 2013-05-07 by jhseol,bckim 버프 패널티

//#if defined(_ATUM_SERVER) && !defined(INTECOM_VIET)
//	#define S_B_GEAR_DAMAGE_PROBABILITY_INFO		// 2013-05-06 by bckim, B기어 타겟 데미지 확인
//#endif

#ifndef INTECOM_VIET
#define C_INGAME_MIX_ITEM		// 2013-02-20 by bhsohn 인게임 조합창 처리
#endif

#ifndef INTECOM_VIET
// 2013-06-18 한국적용
#define SC_COLLECTION_ARMOR_JHSEOL_BCKIM		// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 컨텐츠 선언

#if defined(TEST140) || defined(YEDANG_KOR) || defined(WIKIGAMES_ENG) || defined(DEVX)
// 2013-06-18 한국적용
#define S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM			// 2013-04-18 by jhseol,bckim 이달의 아머 - 컨텐츠 선언
#define C_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM
#endif
#endif

#ifndef INTECOM_VIET
#define C_SHOP_ITEMCNT_INIT_BHSOHN	// 2013-06-20 by bhsohn 상점에서 아이템 구매후 숫자 초기화 코드 추가
#endif

// 2013-06-27 한국적용
//#define S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL			// 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그

#ifndef INTECOM_VIET
// 2013-06-28 캐나다적용
#define S_ADMINTOOL_IP_SEARCH_JHSEOL			// 2013-06-28 by jhseol, 캐나다 어드민툴 IP조회 문제
#endif

#define S_CANNOT_DO_GIFT_UNDER_LEVEL_BCKIM		// 2013-07-08 by bckim, 캐나다요청 레벨70 미만은 선물하기 불가

#define C_FOCUS_MISS_CLIENT_NO_CLOSE		// 2013-06-28 by ssjung 포커스를 잃었을 시 1분후 클라이언트가 종료되는 현상 캐나다 예외처리

//#define C_SERVER_DOWN_ALARM

#ifndef INTECOM_VIET
#define C_LEAST_LV_GIFT			// 2013-07-09 by bhsohn 70 레벨 이상만 선물하기 가능하게 수정
#endif

//#define S_MULTI_RESOLUTION_JHSEOL 				// 2013-07-30 by jhseol, 멀티해상도 지원

#ifndef INTECOM_VIET
#define C_DECA_TOOLTIP			
#endif

#define GAMEFORGE4D_CHAT_MACRO_OUTPUT_TIME	// 2013-07-30 by ssjung 게임 포지 일 경우 매크로 딜레이 15초 에서 5분으로 수정

//#define _WARRIOR_GIVE_PLAYTIME_ITEM
/////////////////////////////////////////////////////////////
// 2015-05-25 Future, new Shop System (Prices per shop, not per Item)
#if defined(ETERNAL_SKIES)  || defined(WIKIGAMES_ENG) || defined(DEVX) // add your servers here
// todo : check how this can be fixed, it is buggy with current config	

//#define SHOP_PRICES_PER_BUILDING_NPC
#endif

// 2015-06-21 Future, ScreenShots as PNG
#define SCREENSHOT_FILE_FORMAT_JPG

//#define _AUTO_COH

////////////////////////////////////////////////////////////
// WP Reward per Kill
#define _WP_REWARD_PER_KILL
#define WP_REWARD_KILL_MIN_COUNT	1
#define WP_REWARD_KILL_MAX_COUNT	5

#define _SPI_REWARD_PER_KILL
#define SPI_REWARD_KILL_MIN_COUNT	500
#define SPI_REWARD_KILL_MAX_COUNT	2000

#define _EXTENDED_KILL_MESSAGE	// 2015-07-22 Future, extended message like: Future was shot down. Reward ... SPI ... WP
#define _WORLDNOTIFICATION_LEVEL
#define _WORLDNOTIFICATION_ENCHANT

////////////////////////////////////////////////////////////
// 2015-09-25 Future, Inactivity Checks
#define C_DISCONNECT_INACTIVE_PLAYERS
#define DISCONNECT_INACTIVE_PLAYERS_AFTER_SECONDS	3600	// 30 mins * 60 seconds
#define DISCONNECT_INACTIVE_PLAYERS_WARNING_TIME	600		// 5 mins * 60 seconds

#define EP1_CHARACTER

///////////////////////////////////////////////////////////
// 2015-10-04 Future, Anti-Decking
#define S_ANTI_DECK
#define ANTI_DECK_HIT_RECOGNITION_TIME_SEC	10	// Time after which a hit will be counted towards a decking kill in seconds

#define FREEWAR_

// todo : (panoskj) add this again
//#define _Parameter_ADDITIONAL

#define NEWSTRING_ITEMINFO

// 2015-11-24 Future, Ip unique connections to Field Server
#define S_IP_UNIQUE_CONNECTION

// 2016-01-03 Future, leader warp restriction
#define SC_LEADER_WARP_RESTRICTION
#define SC_LEADER_WARP_RESTRICTION_TIME_IN_MINUTES		30
#endif // COMMON_CONTENTS_H_
