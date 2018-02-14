// start 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템]
#ifndef _CONFIG_HACK_SHIELD_H_
#define _CONFIG_HACK_SHIELD_H_

//////////////////////////////////////////////////////////////////////
// 핵 쉴드 관련 코드
// 2006-06-05 by ispark
// 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템] - 공통 코드 정리
//////////////////////////////////////////////////////////////////////
#ifdef YEDANG_RELEASE
	#ifdef _TEST_SERVER
		#define HACKSHIELD_LICENCE_CODE				"CB6F70F30592F7F1B875DE18"
		#define HACKSHIELD_GAME_CODE				4704
	#else
		#define HACKSHIELD_LICENCE_CODE				"B93AB8A64E034A207193E95A"
		#define HACKSHIELD_GAME_CODE				4703
	#endif
#elif defined(LANGUAGE_CHINA)
	#ifdef _TEST_SERVER
		#define HACKSHIELD_LICENCE_CODE				"CB6F70F30592F7F1B875DE18"
		#define HACKSHIELD_GAME_CODE				4704
#else
		#define HACKSHIELD_LICENCE_CODE				"B93AB8A64E034A207193E95A"
		#define HACKSHIELD_GAME_CODE				4703
	#endif
// 2008-09-19 by bhsohn Canada HackShield
#elif defined(WORLD_RELEASE)
	#ifdef _TEST_SERVER
		#define HACKSHIELD_LICENCE_CODE				"CB6F70F30592F7F1B875DE18"
		#define HACKSHIELD_GAME_CODE				4704
#else
		#define HACKSHIELD_LICENCE_CODE				"B93AB8A64E034A207193E95A"
		#define HACKSHIELD_GAME_CODE				4703
	#endif
// end 2008-09-19 by bhsohn Canada HackShield
// 2007-09-27 by bhsohn 베트남 실행파일명 변경
// 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
// #elif defined(LANGUAGE_VIETNAM)
// 	#ifdef _TEST_SERVER
// 		#define HACKSHIELD_LICENCE_CODE				"B9D640260C6056590B0CF72A"
// 		#define HACKSHIELD_GAME_CODE				4705
// #else
// 		#define HACKSHIELD_LICENCE_CODE				"EDAA54877F02A804257C85CE"
// 		#define HACKSHIELD_GAME_CODE				4705
// 	#endif
// end 2009. 09. 21 by jskim 게임가드 XTRAP 적용(배트남)
// 2008-04-30 by bhsohn 태국 버전 추가
#elif defined(LANGUAGE_THAI)
	#ifdef _TEST_SERVER
		#define HACKSHIELD_LICENCE_CODE				"DE36C7646C016856143FD7C4"
		#define HACKSHIELD_GAME_CODE				4708
#else
		#define HACKSHIELD_LICENCE_CODE				"DE36C7646C016856143FD7C4"
		#define HACKSHIELD_GAME_CODE				4708
	#endif
// 2008-10-14 by dgwoo gameforge4d 핵쉴드 추가.
#elif defined(GAMEFORGE_RELEASE)
	#ifdef _TEST_SERVER
		#define HACKSHIELD_LICENCE_CODE				"2025A0437939566CC1DCF4B1"
		#define HACKSHIELD_GAME_CODE				4712 
#else
		#define HACKSHIELD_LICENCE_CODE				"2025A0437939566CC1DCF4B1"
		#define HACKSHIELD_GAME_CODE				4712
	#endif
#else
	#ifdef _TEST_SERVER
		#define HACKSHIELD_LICENCE_CODE				"B3B2C96BB276934430BB47D6"
		#define HACKSHIELD_GAME_CODE				4702
	#else
		#define HACKSHIELD_LICENCE_CODE				"702E5F0DC87A9A812DF8A07D"
		#define HACKSHIELD_GAME_CODE				4701
	#endif

#endif

#define HACKSHIELD_FILE_PATH_DBG			"\\HShield\\EhSvc.dll"	
#define HACKSHIELD_FILE_PATH				"HShield\\EhSvc.dll"	

#endif // _CONFIG_HACK_SHIELD_H_
// end 2012-03-21 by hskim, 핵쉴드 기능 추가 [자동 업데이트/옵션 추가/모니터링 시스템]