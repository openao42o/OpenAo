// INFGameMainUnitInfoBar.cpp: implementation of the CINFGameMainUnitInfoBar class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFGameMainUnitInfoBar.h"
#include "INFGameMainMiniMap.h" 			// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "ClientParty.h"				// 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#include "SceneData.h"
#include "D3DHanFont.h"
#include "Background.h"
#include "AtumDatabase.h"
#include "dxutil.h"
#include "PkNormalTimer.h"
#include "QuestData.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "INFWeaponInfo.h"
#include "INFGameMainChat.h"
#include "INFGroupManager.h"		// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"		// 2011. 10. 10 by jskim UI시스템 변경
#include <BonusKillSystem.h>			// 2015-06-16 by Future, Bonus Kill System Definitions

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

#ifdef _WARRIOR_CLASS_KILL_BAR
	#define WSW_BACK_KILL_BAR_X				((g_pD3dApp->GetBackBufferDesc().Width/2)-160)	
	#define WSW_BACK_KILL_BAR_Y				(g_pD3dApp->GetBackBufferDesc().Height - m_pImgMainExpBack->GetImgSize().y-55)	
#endif

	#define GAMEMAIN_GAGE_EXP_LENGTH					219//110// 미정

	#define GAMEMAIN_BACK_GAGE_UP_SIZE_X				179
	#define GAMEMAIN_BACK_GAGE_UP_SIZE_Y				48

	#define GAMEMAIN_BACK_MINIMAP_SIZE_X				145
	#define GAMEMAIN_BACK_MINIMAP_SIZE_Y				69
	#define GAMEMAIN_BACK_MINIMAP_START_X				(g_pD3dApp->GetBackBufferDesc().Width - GAMEMAIN_BACK_MINIMAP_SIZE_X)
	#define GAMEMAIN_BACK_MINIMAP_START_Y				0

	#define GAMEMAIN_BACK_UP_SIZE_Y						22	// 화면 상단
	//#define GAMEMAIN_BACK_DOWN_SIZE_Y					39	// 화면 하단
	#define GAMEMAIN_BACK_DOWN_START_Y					(g_pD3dApp->GetBackBufferDesc().Height - GAMEMAIN_BACK_DOWN_SIZE_Y)

	#define GAMEMAIN_EXP_GAGE_START_X					((g_pD3dApp->GetBackBufferDesc().Width - 219)/2)
	#define GAMEMAIN_EXP_GAGE_START_Y					(GAMEMAIN_BACK_DOWN_START_Y - 9)

	#define GAMEMAIN_UNIT_POS_START_X					(g_pD3dApp->GetBackBufferDesc().Width - 30)
	#define GAMEMAIN_UNIT_POS_START_Y					5

	//#define GAMEMAIN_BACK_GAGE_EXP_START_X				((g_pD3dApp->GetBackBufferDesc().Width - 234)/2)
	#define GAMEMAIN_BACK_GAGE_EXP_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - m_pImgMainExpBack->GetImgSize().y)		// 2011. 10. 10 by jskim UI시스템 변경

	#define GAMEMAIN_STRING_FUEL_START_X				163

	#define GAMEMAIN_STRING_EXP_START_X					g_pD3dApp->GetBackBufferDesc().Width / 2

	// 스트링 위치 정의
	#define GAMEMAIN_NEW_STRING_HP_START_X				17
	#define GAMEMAIN_NEW_STRING_HP_START_Y				80
	#define GAMEMAIN_GAGE_HP_UP_START_X					28
	#define GAMEMAIN_GAGE_HP_UP_START_Y					90 
	#define GAMEMAIN_NEW_STRING_MAX_HP_START_Y			104
	#define GAMEMAIN_NEW_STRING_DP_START_Y				80
	#define GAMEMAIN_GAGE_DP_UP_START_X					133
	#define GAMEMAIN_GAGE_DP_UP_START_Y					90
	#define GAMEMAIN_NEW_STRING_MAX_DP_START_X			108
	#define GAMEMAIN_NEW_STRING_MAX_DP_START_Y			104
	#define GAMEMAIN_GAGE_SP_UP_START_X					130
	#define GAMEMAIN_GAGE_SP_UP_START_Y					50
	#define GAMEMAIN_NEW_STRING_MAX_SP_START_Y			70


	// 메인 게이지 관련 정의

	#define GAMEMAIN_GAGE_HP_START_X					15
	#define GAMEMAIN_GAGE_HP_START_Y					29
	#define GAMEMAIN_GAGE_DP_START_X					83
	#define GAMEMAIN_GAGE_DP_START_Y					33
	#define GAMEMAIN_GAGE_SP_START_X					120
	#define GAMEMAIN_GAGE_SP_START_Y					22
	#define GAMEMAIN_GAGE_BOOSTER_START_X				9
	#define GAMEMAIN_GAGE_BOOSTER_START_Y				25
	#define GAMEMAIN_GAGE_FUEL_START_X					6
	#define GAMEMAIN_GAGE_FUEL_START_Y					5
	#define GAMEMAIN_VOICECHAT_X						(g_pD3dApp->GetBackBufferDesc().Width-32)
	#define GAMEMAIN_VOICECHAT_Y						(156)
	#define GAMEMAIN_VOICECHAT_W						26
	#define GAMEMAIN_VOICECHAT_H						26


	// SPEED 스트링 관련 정의
	#define SPEED_STRING_START_X						80	
	#define SPEED_STRING_START_Y						105
	#define SPEED_STRING_WIDTH							19
	#define SPEED_STRING_HEIGHT							17

	// 맵이름 관련 정의
	#define GAME_MAP_NAME_RENDER_X						(g_pD3dApp->GetBackBufferDesc().Width-30)
	#define GAME_MAP_NAME_RENDER_Y						4
	#define GAME_MAP_NAME_TEXT_COLOR					RGB(200,200,200)
	#define GAME_UNIT_POS_COLOR							RGB(200,200,200)

	// 레이더 위치 정의
	#define RADER_CENTER_POS_X							(g_pD3dApp->GetBackBufferDesc().Width-75)	
	#define RADER_CENTER_POS_Y							82 + 16
	#define RADER_RADIUS								64

	#define GAME_EVENT_BASICPOS_X						242
	#define GAME_EVENT_BASICPOS_Y						32

	// 2006-03-07 by ispark, 언어에 따라 위치 수정
	#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
		#define GAMEMAIN_STRING_FUEL_START_Y				72//
		#define GAMEMAIN_STRING_EXP_START_Y					g_pD3dApp->GetBackBufferDesc().Height - 12//11)
		#define GAMEMAIN_NEW_STRING_MAX_HP_START_X			25//21
		#define GAMEMAIN_NEW_STRING_DP_START_X				112//114
		#define GAMEMAIN_NEW_STRING_MAX_SP_START_X			80//130
		#define GAMEMAIN_EVENT_FONT_Y						2
	#else
		#define GAMEMAIN_STRING_FUEL_START_Y				72
		#define GAMEMAIN_STRING_EXP_START_Y					g_pD3dApp->GetBackBufferDesc().Height - 12//11)
		#define GAMEMAIN_NEW_STRING_MAX_HP_START_X			25//21
		#define GAMEMAIN_NEW_STRING_DP_START_X				112//114
		#define GAMEMAIN_NEW_STRING_MAX_SP_START_X			80//130
		#define GAMEMAIN_EVENT_FONT_Y						2//0
	#endif


	#define INFOMAIN_GIFT_ICON_X				75		// 2012-08-14 by mspark, 기프트 아이콘 위치 수정 - 기존 70에서 75로 수정
	#define INFOMAIN_GIFT_CITY_ICON_Y			130
	#define INFOMAIN_GIFT_FIELD_ICON_Y			176		// 2012-08-14 by mspark, 기프트 아이콘 위치 수정 - 기존 89에서 176으로 수정

	// 2008-03-25 by dgwoo 모선전 진행 버튼.
	#define MOTHERSHIP_BUTTON_X					(nBoldPosX+nBalonWidth)
	#define MOTHERSHIP_BUTTON_Y					(nBoldPosY)
															 
#else
#define GAMEMAIN_GAGE_EXP_LENGTH					219//110// 미정

#define GAMEMAIN_BACK_GAGE_UP_SIZE_X				179
#define GAMEMAIN_BACK_GAGE_UP_SIZE_Y				48

#define GAMEMAIN_BACK_MINIMAP_SIZE_X				145
#define GAMEMAIN_BACK_MINIMAP_SIZE_Y				69
#define GAMEMAIN_BACK_MINIMAP_START_X				(g_pD3dApp->GetBackBufferDesc().Width - GAMEMAIN_BACK_MINIMAP_SIZE_X)
#define GAMEMAIN_BACK_MINIMAP_START_Y				0

#define GAMEMAIN_BACK_UP_SIZE_Y						22	// 화면 상단
#define GAMEMAIN_BACK_DOWN_SIZE_Y					39	// 화면 하단
#define GAMEMAIN_BACK_DOWN_START_Y					(g_pD3dApp->GetBackBufferDesc().Height - GAMEMAIN_BACK_DOWN_SIZE_Y)

#define GAMEMAIN_EXP_GAGE_START_X					((g_pD3dApp->GetBackBufferDesc().Width - 219)/2)
#define GAMEMAIN_EXP_GAGE_START_Y					(GAMEMAIN_BACK_DOWN_START_Y - 9)

#define GAMEMAIN_UNIT_POS_START_X					(g_pD3dApp->GetBackBufferDesc().Width - 30)
#define GAMEMAIN_UNIT_POS_START_Y					5

#define GAMEMAIN_BACK_GAGE_EXP_START_X				((g_pD3dApp->GetBackBufferDesc().Width - 234)/2)
#define GAMEMAIN_BACK_GAGE_EXP_START_Y				(GAMEMAIN_BACK_DOWN_START_Y - 12)

#define GAMEMAIN_STRING_FUEL_START_X				22

#define GAMEMAIN_STRING_EXP_START_X					((g_pD3dApp->GetBackBufferDesc().Width)/2 - 35)

// 스트링 위치 정의
#define GAMEMAIN_NEW_STRING_HP_START_X				17
#define GAMEMAIN_NEW_STRING_HP_START_Y				80
#define GAMEMAIN_GAGE_HP_UP_START_X					21
#define GAMEMAIN_GAGE_HP_UP_START_Y					92
#define GAMEMAIN_NEW_STRING_MAX_HP_START_Y			104
#define GAMEMAIN_NEW_STRING_DP_START_Y				80
#define GAMEMAIN_GAGE_DP_UP_START_X					114
#define GAMEMAIN_GAGE_DP_UP_START_Y					92
#define GAMEMAIN_NEW_STRING_MAX_DP_START_X			108
#define GAMEMAIN_NEW_STRING_MAX_DP_START_Y			104
#define GAMEMAIN_GAGE_SP_UP_START_X					130
#define GAMEMAIN_GAGE_SP_UP_START_Y					50
#define GAMEMAIN_NEW_STRING_MAX_SP_START_Y			62


// 메인 게이지 관련 정의

#define GAMEMAIN_GAGE_HP_START_X					15
#define GAMEMAIN_GAGE_HP_START_Y					29
#define GAMEMAIN_GAGE_DP_START_X					83
#define GAMEMAIN_GAGE_DP_START_Y					29
#define GAMEMAIN_GAGE_SP_START_X					120
#define GAMEMAIN_GAGE_SP_START_Y					22
#define GAMEMAIN_GAGE_BOOSTER_START_X				9
#define GAMEMAIN_GAGE_BOOSTER_START_Y				21
#define GAMEMAIN_GAGE_FUEL_START_X					6
#define GAMEMAIN_GAGE_FUEL_START_Y					5
#define GAMEMAIN_VOICECHAT_X						(g_pD3dApp->GetBackBufferDesc().Width-32)
#define GAMEMAIN_VOICECHAT_Y						(156)
#define GAMEMAIN_VOICECHAT_W						26
#define GAMEMAIN_VOICECHAT_H						26


// SPEED 스트링 관련 정의
#define SPEED_STRING_START_X						80	
#define SPEED_STRING_START_Y						90
#define SPEED_STRING_WIDTH							19
#define SPEED_STRING_HEIGHT							17

// 맵이름 관련 정의
#define GAME_MAP_NAME_RENDER_X						(g_pD3dApp->GetBackBufferDesc().Width-30)
#define GAME_MAP_NAME_RENDER_Y						4
#define GAME_MAP_NAME_TEXT_COLOR					RGB(200,200,200)
#define GAME_UNIT_POS_COLOR							RGB(200,200,200)

// 레이더 위치 정의
#define RADER_CENTER_POS_X							(g_pD3dApp->GetBackBufferDesc().Width-75)	
#define RADER_CENTER_POS_Y							82 + 16
#define RADER_RADIUS								64

#define GAME_EVENT_BASICPOS_X						192
#define GAME_EVENT_BASICPOS_Y						32

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define GAMEMAIN_STRING_FUEL_START_Y				3//5
#define GAMEMAIN_STRING_EXP_START_Y					(GAMEMAIN_BACK_DOWN_START_Y - 13)//11)
#define GAMEMAIN_NEW_STRING_MAX_HP_START_X			25//21
#define GAMEMAIN_NEW_STRING_DP_START_X				112//114
#define GAMEMAIN_NEW_STRING_MAX_SP_START_X			135//130
#define GAMEMAIN_EVENT_FONT_Y						2
#else
#define GAMEMAIN_STRING_FUEL_START_Y				3//5
#define GAMEMAIN_STRING_EXP_START_Y					(GAMEMAIN_BACK_DOWN_START_Y - 13)//11)
#define GAMEMAIN_NEW_STRING_MAX_HP_START_X			25//21
#define GAMEMAIN_NEW_STRING_DP_START_X				112//114
#define GAMEMAIN_NEW_STRING_MAX_SP_START_X			135//130
#define GAMEMAIN_EVENT_FONT_Y						2//0
#endif


#define INFOMAIN_GIFT_ICON_X				70
#define INFOMAIN_GIFT_CITY_ICON_Y			130
#define INFOMAIN_GIFT_FIELD_ICON_Y			89

// 2008-03-25 by dgwoo 모선전 진행 버튼.
#define MOTHERSHIP_BUTTON_X					(nBoldPosX+nBalonWidth)
#define MOTHERSHIP_BUTTON_Y					(nBoldPosY)
#endif

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	#define RESTCOUNT_STRING_WIDTH				24
	#define RESTCOUNT_STRING_HEIGHT				22
	
	#define RESTCOUNT_SCALE_DEFAULT				0.8f
	#define RESTCOUNT_SCALE_MAX					1.2f

	#define KILLCOUNT_STRING_WIDTH				24
	#define KILLCOUNT_STRING_HEIGHT				22

	#define KILLCOUNT_SCALE_DEFAULT				0.8f
	#define KILLCOUNT_SCALE_MAX					1.2f
	#define KILLCOUNT_SCALE_DECREASE			0.035f

	#define KILLCOUNT2_STRING_WIDTH				29
	#define KILLCOUNT2_STRING_HEIGHT			32

	#define BONUS_EXP_RATE_POS_X				243
	#define BONUS_EXP_RATE_POS_Y				78

	#define BONUS_EXP_RATE_TEXT_POS_X			233
	#define BONUS_EXP_RATE_TEXT_POS_Y			88
	#define BONUS_EXP_RATE_TEXT_LINE_HEIGHT		22

	#define RESTKILLCOUNT_BASE_POS_X			87			// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼 - 기존 77에서 87로 변경	
	#define RESTKILLCOUNT_BASE_POS_Y			333	
	#define RESTKILLCOUNT_BASE_HEIGHT_SIZE		10	
	#define RESTKILLCOUNT_MOVE_HAPPYHOUR		34

	#define	RESTCOUNT_MAX_POSNUM				4
	#define	RESTCOUNT_MAX_COUNT					9999
	#define RESTCOUNT_BACK_POS_X				32	
	#define RESTCOUNT_BACK_POS_Y				4	
	#define RESTCOUNT_TEXT_POS_X				13	
	#define RESTCOUNT_TEXT_POS_Y				1	

	#define	KILLCOUNT_MAX_POSNUM				3
#if S_BONUS_KILL_SYSTEM_RENEWAL
	#define	KILLCOUNT_MAX_COUNT					KILLCOUNT_MAX(g_pShuttleChild->m_myShuttleInfo.UnitKind)
#else
	#define	KILLCOUNT_MAX_COUNT					30
#endif // S_BONUS_KILL_SYSTEM_RENEWAL
	#define	HIGHLIGHT_COUNT_AREA				10
	#define	KILLCOUNT_SHOW_HIGHLIGHT_COUNT		KILLCOUNT_MAX_COUNT - HIGHLIGHT_COUNT_AREA
	#define KILLCOUNT_BACK_POS_X				32	
	#define KILLCOUNT_BACK_POS_Y				4	
	#define KILLCOUNT_TEXT_POS_X				15	
	#define KILLCOUNT_TEXT_POS_Y				1	
	#define KILLCOUNT_TEXT_POS_Y				1	


	#define KILLCOUNT2_EFF_POS_X				55	
	#define KILLCOUNT2_EFF_POS_Y				25	
	#define KILLCOUNT2_TEXT_POS_X				15	
	#define KILLCOUNT2_TEXT_POS_Y				3	
	#define KILLCOUNT2_EFF_MAX_ALPHA			230
	#define KILLCOUNT2_EFF_ALPHA_CREASE			7
	#define KILLCOUNT2_SCALE					1.0f
	#define KILLCOUNT2_EFF_MAX_BLENK_COUNT		6

	#define BONUS_EXP_RATE_POPUP_RECT_LEFT		230
	#define BONUS_EXP_RATE_POPUP_RECT_TOP		78
	#define BONUS_EXP_RATE_POPUP_RECT_RIGHT		130
	#define BONUS_EXP_RATE_POPUP_RECT_BOTTOM	22	
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
	#define RESTKILLCOUNT_BUTTON_BASE_POS_X			15
	#define RESTKILLCOUNT_BUTTON_BASE_POS_Y			333	
	#define RESTKILLCOUNT_BUTTON_BASE_HEIGHT_SIZE	10

	#define RESTKILLCOUNT_BUTTON_WIDTH			10
	#define RESTKILLCOUNT_BUTTON_HEIGHT			28

	#define	RESTCOUNT_IMAGE_POS_LIMIT			145
	#define	KILLCOUNT_IMAGE_POS_LIMIT			130
	#define	RESTKILLCOUNT_IMAGE_MOVE_VALUE		10
	// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameMainUnitInfoBar::CINFGameMainUnitInfoBar(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_bWarning = FALSE;
	m_v2RaderCenter.x = RADER_CENTER_POS_X;
	m_v2RaderCenter.y = RADER_CENTER_POS_Y;
	m_v2MousePt.x = g_pD3dApp->GetBackBufferDesc().Width/2;
	m_v2MousePt.y = g_pD3dApp->GetBackBufferDesc().Height/2;


#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경// 	
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL] = NULL;
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL] = NULL;

	m_pImgBackMinimap = NULL;
#endif
	m_pFontUnitPosInfo = NULL;
	m_fWarningTime = 0;

	memset(m_fGageRate, 0x00, sizeof(float)*GAMEMAIN_GAGE_NUMBER);
	memset(m_pFontGageInfo, 0x00, sizeof(DWORD)*GAMEMAIN_GAGE_LINE_NUMBER);
#ifdef _WARRIOR_CLASS_KILL_BAR
	memset(m_szGageInfo, 0x00, GAMEMAIN_GAGE_LINE_NUMBER*128);
#else
	memset(m_szGageInfo, 0x00, GAMEMAIN_GAGE_LINE_NUMBER*GAMEMAIN_GAGE_LINE_LENGTH);
#endif
//	memset(m_pButtonShowHide, 0x00, sizeof(DWORD)*2*BUTTON_STATE_NUMBER);

	m_nButtonState = BUTTON_STATE_NORMAL;

	// 2004-12-08 by jschoi
	m_pImgMainGageBack = NULL;
	m_pImgMainExpBack = NULL;
	m_pImgMainExpBar = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgMainExpBar1 = NULL;
#endif
	m_pImgMainSpeedNum = NULL;
	m_pImgMainMinus = NULL;
	m_pImgMainWarning = NULL;
	m_pImgGeneralPremiumCard = NULL;

	// 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가

	m_bPremiumToolTip = FALSE;

	// end 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가

	m_pImgSuperPremiumCard = NULL;
	m_pImgHappyHourEvent = NULL;
	m_pImgPCHappyHourEvent = NULL;
	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	m_pImgPCHomePremiumEvent = NULL;
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	m_pImgVoiceChatIcon = NULL;

	int i;
	for(i = 0; i < GAMEMAIN_EVENT_RECOVERY_MAX; i++)
	{
		m_pImgEventRecovery[i] = NULL;
	}
	for( i = 0; i < GIFT_ICON_IMG_COUNT; i++)
	{
		m_pImgGiftIcon[i] = NULL;
	}

	memset(m_pImgMainGageHP,0x00,sizeof(DWORD)*GAMEMAIN_GAGE_HP_MAX_GRADE);
	memset(m_pImgMainGageDP,0x00,sizeof(DWORD)*GAMEMAIN_GAGE_DP_MAX_GRADE);
	memset(m_pImgMainGageSP,0x00,sizeof(DWORD)*GAMEMAIN_GAGE_SP_MAX_GRADE);
	memset(m_pImgMainGageBooster,0x00,sizeof(DWORD)*GAMEMAIN_GAGE_BOOSTER_MAX_GRADE);
	memset(m_pImgMainGageFuel,0x00,sizeof(DWORD)*GAMEMAIN_GAGE_FUEL_MAX_GRADE);
	memset(m_pImgMainIconFuel,0x00,sizeof(DWORD)*2);
	memset(m_pImgPing, 0x00, sizeof(DWORD) * PING_DISPLAY_COUNT);

	m_pFontMapName = NULL;
	m_pFontCurrentEvent = NULL;
	m_pFontEventHappyHour = NULL;
	m_pFontEventRecovery = NULL;
	m_pFontDecaToolTip = NULL;							// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
	m_bRenderEvent = FALSE;
	m_nHappyHourX = 0;
	m_nHappyHourY = 0;
	m_bRenderEventRecovery = FALSE;
	
	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	//m_BHappyHourEvent = 0x00;
	m_byHappyHourInfluence = 0x00;
	m_bPcBangHappyHour = FALSE;
	// end 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경

	m_bMemberPCBang = FALSE;	
	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	m_pMemberPremiumEvent = FALSE;
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업

	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	memset(&m_HappyEventInfo, 0x00, sizeof(EVENT_INFO));				//세력 진형
	memset(&m_HappyEventAllInfluInfo, 0x00, sizeof(EVENT_INFO));		// 전체 진형
	memset(&m_PCHappyEventInfo, 0x00, sizeof(EVENT_INFO));				// pc방 
	m_bHappyHourAllInfluence = FALSE;

	m_vecGiftMsg.clear();
	m_fGiftTime = 0.0f;

	// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
	m_bMotherHappyHour = FALSE;
	memset(&m_MotherShipEventInfo, 0x00, sizeof(EVENT_INFO));

	m_bItemHappyHour = FALSE;
	memset(&m_ItemEventInfo, 0x00, sizeof(EVENT_INFO));
	// end 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가

	// 2008-10-20 by bhsohn VOIP 버그 수정
	m_pImgVoiceSPKIcon = NULL;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pGageBarInfo = NULL;
#endif
	
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	m_pImgBonusExpRate = NULL;
	m_pImgRestCount = NULL;
	m_pImgKillCount = NULL;
	m_pImgKillCount2 = NULL;
	m_pImgKillEff	 = NULL;
	m_pGroupImgRestCountBack = NULL;
	m_pGroupImgKillCountBack = NULL;


	m_bChangeKillCount = false;
	m_fKillCountScale = KILLCOUNT_SCALE_DEFAULT;
	m_nKillCount = 0;
	m_nRestCount = 0;

	m_bRenderBonusExpRateText = FALSE;
	m_nPartyRate = 0;
	m_nFriendRate = 0;
	m_nGuildRate = 0;
	
	m_bCompleteKillCount = false;
	m_nCompleteKillCountBlank = 0;
	m_nCompleteKillCountAlpha = 0;
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	m_pImgRestCountOnBtn = NULL;
	m_pImgRestCountOffBtn = NULL;
	m_pImgKillCountOnBtn = NULL;
	m_pImgKillCountOffBtn = NULL;

	m_nStateRestCountOnBtn = BUTTON_STATE_NORMAL;
	m_nStateRestCountOffBtn = BUTTON_STATE_NORMAL;
	m_nStateKillCountOnBtn = BUTTON_STATE_NORMAL;
	m_nStateKillCountOffBtn = BUTTON_STATE_NORMAL;

	m_nRestCountPosX = 0;
	m_nRestCountPosY = 0;
	m_nKillCountPosX = 0;
	m_nKillCountPosY = 0;

	m_bCheckRestCountON = true;
	m_bCheckKillCountON = true;

	m_bCheckRestCountBtnClick = false;

	m_fPlusRestCountPosX = 0;
	m_fPlusKillCountPosX = 0;

	// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
	m_bRestCountToolTip = FALSE;
	m_bKillCountToolTip = FALSE;
	// end 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시

#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
}

CINFGameMainUnitInfoBar::~CINFGameMainUnitInfoBar()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]);
	util::del(m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]);

	util::del(m_pImgBackMinimap);
#endif
	util::del(m_pFontUnitPosInfo);

	int i=0;
	for(i=0;i<GAMEMAIN_GAGE_LINE_NUMBER;i++)
	{
		util::del(m_pFontGageInfo[i]);
	}	

//	for(i=0;i<BUTTON_STATE_NUMBER;i++)
//	{
//		util::del(m_pButtonShowHide[0][i]);
//		util::del(m_pButtonShowHide[1][i]);
//	}

	// 2004-12-08 by jschoi
	util::del(m_pImgMainGageBack);
	util::del(m_pImgMainExpBack);
	util::del(m_pImgMainExpBar);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pImgMainExpBar1);
#endif
	util::del(m_pImgMainSpeedNum);
	util::del(m_pImgMainMinus);
	util::del(m_pImgMainWarning);
	util::del(m_pImgGeneralPremiumCard);
	util::del(m_pImgSuperPremiumCard);
	util::del(m_pImgHappyHourEvent);
	util::del(m_pImgPCHappyHourEvent);
	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	util::del(m_pImgPCHomePremiumEvent);
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	util::del(m_pImgVoiceChatIcon);
	for(i = 0; i < GAMEMAIN_EVENT_RECOVERY_MAX; i++)
	{
		util::del(m_pImgEventRecovery[i]);
	}
	for(i = 0; i < GIFT_ICON_IMG_COUNT; i++)
	{
		util::del(m_pImgGiftIcon[i]);
	}
	for(i=0;i<GAMEMAIN_GAGE_HP_MAX_GRADE;i++)
	{
		util::del(m_pImgMainGageHP[i]);
		util::del(m_pImgMainGageDP[i]);
		util::del(m_pImgMainGageSP[i]);
	}
	for(i=0;i<GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;i++)
	{
		util::del(m_pImgMainGageBooster[i]);
	}
	for(i=0;i<GAMEMAIN_GAGE_FUEL_MAX_GRADE;i++)
	{
		util::del(m_pImgMainGageFuel[i]);
	}
	for(i=0;i<2;i++)
	{
		util::del(m_pImgMainIconFuel[i]);
	}

	//2015-08-15 by St0rmy, better Ping Display
	for (i = 0; i < PING_DISPLAY_COUNT; i++)
	{
		util::del(m_pImgPing[i]);
	}

	util::del(m_pFontMapName);
	util::del(m_pFontCurrentEvent);
	util::del(m_pFontEventHappyHour);
	util::del(m_pFontEventRecovery);
	util::del(m_pFontDecaToolTip);						// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
	m_vecGiftMsg.clear();
					  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del( m_pGageBarInfo );																  
#endif

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	util::del(m_pImgBonusExpRate);
	util::del(m_pImgRestCount);
	util::del(m_pImgKillCount);
	util::del(m_pImgKillCount2);
	util::del(m_pImgKillEff);
	util::del(m_pGroupImgRestCountBack);
	util::del(m_pGroupImgKillCountBack);
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	util::del(m_pImgRestCountOnBtn);
	util::del(m_pImgRestCountOffBtn);
	util::del(m_pImgKillCountOnBtn);
	util::del(m_pImgKillCountOffBtn);
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
}

#ifdef _WARRIOR_CLASS_KILL_BAR
int nWSWArrClass[] = { 300, 900, 1800, 3000, 4500, 6300, 8400, 10800, 13500, 16500, 20000, 24000, 28500, 33500, 39000, 45000, 51700, 59100, 67200, 76000, 86000, 98000, 112000, 129000, 149000 };
char *szWSWArrClass[] = { "Private", "Private First Class","Corporal","Sergeant", "Staff Sergeant", "Sergeant First Class",
							"Master Sergeant","Second Lieutenant", "First Lieutenant","Captain","Major - I","Major - II",
							"Lieutenant Colonel - I", "Lieutenant Colonel - II", "Colonel - I", "Colonel - II", "Brigadier Gerenal - I",
							"Brigadier Gerenal - II", "Major General - I", "Major General - II", "Lieutenant General - I", "Lieutenant General - II",
							"General - I", "General - II", "Marshal","Marshal" };

int GetCurrentKillClass()
{
	int nWSWCurFame = g_pShuttleChild->m_myShuttleInfo.Propensity;
	if (nWSWCurFame < nWSWArrClass[0])
	{
		return 0;
	}
	if (nWSWCurFame >= nWSWArrClass[sizeof(nWSWArrClass) - 1])
	{
		return nWSWArrClass[sizeof(nWSWArrClass) - 1];
	}
	for (int i = 0; i < sizeof(nWSWArrClass); i++)
	{
		if (nWSWCurFame >= nWSWArrClass[i] && nWSWCurFame < nWSWArrClass[i + 1])
		{
			return i + 1;
		}
	}

	return 0;
}

char *GetClassString(int Class)
{
	return szWSWArrClass[Class];
}
float GetCurrentKillBarRate(int Class)
{
	if (Class == 25)
	{
		return 100.0f;
	}

	if (Class > 0)
	{
		int nWSWCurFame = g_pShuttleChild->m_myShuttleInfo.Propensity;
		int nTempprogress = nWSWCurFame - nWSWArrClass[Class - 1];
		int nWSWBarMaxVal = nWSWArrClass[Class] - nWSWArrClass[Class - 1];
		float fKillRateWSW = (float)nTempprogress / nWSWBarMaxVal;
		return fKillRateWSW;
	}
	else
	{
		int nWSWCurFame = g_pShuttleChild->m_myShuttleInfo.Propensity;
		int nWSWBarMaxVal = nWSWArrClass[Class];
		float fKillRateWSW = (float)nWSWCurFame / nWSWBarMaxVal;
		return fKillRateWSW;
	}
	return 0.0f;
}
#endif

void CINFGameMainUnitInfoBar::UpdateAllGage()
{

	SetHP((float)g_pShuttleChild->m_myShuttleInfo.CurrentHP, (float)g_pShuttleChild->m_myShuttleInfo.HP);
	SetDP((float)g_pShuttleChild->m_myShuttleInfo.CurrentDP, (float)g_pShuttleChild->m_myShuttleInfo.DP);
	SetBURN((float)g_pShuttleChild->m_fCurrentBURN, (float)g_pShuttleChild->m_fBURN);
	SetSP((float)g_pShuttleChild->m_myShuttleInfo.CurrentSP, (float)g_pShuttleChild->m_myShuttleInfo.SP);
	SetEP((float)g_pShuttleChild->m_myShuttleInfo.CurrentEP, (float)g_pShuttleChild->m_myShuttleInfo.EP);
	SetSpeed( g_pShuttleChild->m_fShuttleSpeed, 
			g_pShuttleChild->m_fRealShuttleSpeedMin, 
			g_pShuttleChild->m_fRealShuttleSpeedBoosterOn );
}

void CINFGameMainUnitInfoBar::SetHP(float fCurrent, float fMax)
{
	if(fMax< fCurrent)
		fCurrent = fMax;
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_HP], STRMSG_C_INTERFACE_0022);//"에너지"
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	wsprintf(m_szGageInfo[GAMEMAIN_GAGE_HP_UP], "%4d/", (int)fCurrent);
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_HP], "%d", (int)fMax);
#else
	wsprintf(m_szGageInfo[GAMEMAIN_GAGE_HP_UP], "%4d", (int)fCurrent);
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_HP], "/%d", (int)fMax);
#endif
	m_fGageRate[GAMEMAIN_GAGE_HP_UP] = fCurrent/fMax;
}

void CINFGameMainUnitInfoBar::SetDP(float fCurrent, float fMax)
{
	if(fMax< fCurrent)
		fCurrent = fMax;
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_DP], STRMSG_C_INTERFACE_0023);//"쉴드"
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	wsprintf(m_szGageInfo[GAMEMAIN_GAGE_DP_UP], "%d/", (int)fCurrent);
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_DP], "%d", (int)fMax);
#else
	wsprintf(m_szGageInfo[GAMEMAIN_GAGE_DP_UP], "%d", (int)fCurrent);
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_DP], "/%d", (int)fMax);
#endif
	m_fGageRate[GAMEMAIN_GAGE_DP_UP] = fCurrent/fMax;
}

void CINFGameMainUnitInfoBar::SetBURN(float fCurrent, float fMax)
{
	m_fGageRate[GAMEMAIN_GAGE_BURN_UP] = fCurrent/fMax;
}

void CINFGameMainUnitInfoBar::SetSP(float fCurrent, float fMax)
{
	if(fMax< fCurrent)
		fCurrent = fMax;
	wsprintf(m_szGageInfo[GAMEMAIN_GAGE_SP_UP], "%4d/", (int)fCurrent);
	wsprintf(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_SP], "%4d", (int)fMax);
	m_fGageRate[GAMEMAIN_GAGE_SP_UP] = fCurrent/fMax;
}

void CINFGameMainUnitInfoBar::SetEP(float fCurrent, float fMax)
{
	if(fMax< fCurrent)
		fCurrent = fMax;
	wsprintf(m_szGageInfo[GAMEMAIN_GAGE_EP_UP], "%d/%d", (int)fCurrent, (int)fMax);
	m_fGageRate[GAMEMAIN_GAGE_EP_UP] = fCurrent/fMax;
}
#ifdef _WARRIOR_CLASS_KILL_BAR
#define STR_GAGEINFO_FAME "Level [\\g%d\\g] \\y[Class]: %s\\y, %5.2f%%"
#define STR_GAGEINFO_FAME_MAX "Level \\g[%d]\\g \\e[Class]: %s\\e"
#endif
void CINFGameMainUnitInfoBar::SetFame()
{
#ifdef _WARRIOR_CLASS_KILL_BAR
	float fExp;
	if (g_pShuttleChild->m_myShuttleInfo.Level == CHARACTER_MAX_LEVEL)
	{
		int nCurrentClass = GetCurrentKillClass();
		float m_fClassBarRate = GetCurrentKillBarRate(nCurrentClass);
		fExp = m_fClassBarRate * 100.0f;

		memset(&m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], 0x00, sizeof(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP])); //Clearing charbuffer

		if (nCurrentClass < 25)
		{
			sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], STR_GAGEINFO_FAME, nCurrentClass, GetClassString(nCurrentClass), fExp);
		}
		else
		{
			sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], STR_GAGEINFO_FAME_MAX, nCurrentClass, GetClassString(nCurrentClass));
		}
		m_fGageRate[GAMEMAIN_GAGE_EXP_UP] = fExp / 100.0f;
	}
#endif
}

void CINFGameMainUnitInfoBar::SetEXP(float fExp)
{
	auto perce = fExp;

	// 2006-02-07 by ispark
	perce = min(99.99f, perce);
	perce = max(0.00f, perce);

#ifdef _HIDE_PERCENTAGE_ON_MAXLEVEL
	if (g_pShuttleChild->m_myShuttleInfo.Level == CHARACTER_MAX_LEVEL)
	{
#ifdef _WARRIOR_CLASS_KILL_BAR

		auto nCurrentClass = GetCurrentKillClass();
		perce = GetCurrentKillBarRate(nCurrentClass) * 100.0f;

		memset(&m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], 0x00, sizeof(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP])); //Clearing charbuffer

		auto level = g_pShuttleChild->m_myShuttleInfo.Level;

		if(nCurrentClass < 25)
			
			sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], STR_GAGEINFO_FAME, level, GetClassString(nCurrentClass), perce);
		
		else sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], STR_GAGEINFO_FAME_MAX, level, GetClassString(nCurrentClass));
#else
		sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], "Level %d", g_pShuttleChild->m_myShuttleInfo.Level);
#endif
	}
	else
	{
		sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], "Level %d  %5.2f%%", g_pShuttleChild->m_myShuttleInfo.Level, perce);
	}
#else
	sprintf(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP], "Lv.%d  %5.2f%%",g_pShuttleChild->m_myShuttleInfo.Level, perce);
#endif

	m_fGageRate[GAMEMAIN_GAGE_EXP_UP] = perce / 100.0f;
}

void CINFGameMainUnitInfoBar::SetSpeed(float fCurrent, float fMin, float fMax)
{
	if(fMin > 0)
	{
		fMin = 0;// 최소 값은 0으로 한다.음수인 경우를 대비해서 이 값을 사용한다.
	}
	m_fGageRate[GAMEMAIN_GAGE_SPEED_UP] = (fCurrent-fMin)/(fMax-fMin);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::SetHappyHourEvent(BOOL bStart)
/// \brief		이벤트 관련 아이콘들의 위치를 셋팅해주는 함수.
/// \author		dgwoo
/// \date		2007-01-25 ~ 2007-01-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::SetHappyHourEventIconPos()
{
	// 해피아워 이벤트.
	m_nHappyHourX = g_pD3dApp->GetBackBufferDesc().Width - GAME_EVENT_BASICPOS_X;

	// PC방 가맹점.
	m_nPCHappyHourX = g_pD3dApp->GetBackBufferDesc().Width - 64;
}

HRESULT CINFGameMainUnitInfoBar::InitDeviceObjects()
{
	ASSERT_ASSERT(m_pGameData);
	// 2011. 10. 10 by jskim UI시스템 변경	  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	DataHeader *pDataHeader;

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("Gage");
	m_pGageBarInfo = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pGageBarInfo->InitDeviceObjects( g_pD3dApp->m_pImageList );			  
#else													   
	DataHeader *pDataHeader = m_pGameData->Find("mnupbkpx");
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL] = new CINFImageEx;
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->InitDeviceObjects(pDataHeader);

	pDataHeader = m_pGameData->Find("mndnbkpx");
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL] = new CINFImageEx;
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->InitDeviceObjects(pDataHeader);

	pDataHeader = m_pGameData->Find("mnMinibk");
	m_pImgBackMinimap = new CINFImageEx;
	m_pImgBackMinimap->InitDeviceObjects(pDataHeader);
#endif

	char buf[64];
	int i=0;
	for(i=0;i<GAMEMAIN_GAGE_LINE_NUMBER;i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pFontGageInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 7, D3DFONT_ZENABLE, TRUE, 256, 32);
#else
		m_pFontGageInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
#endif
		m_pFontGageInfo[i]->InitDeviceObjects(g_pD3dDev) ;
	}	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pFontUnitPosInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 7, D3DFONT_ZENABLE, FALSE, 256, 32);
#else																										
	m_pFontUnitPosInfo = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, FALSE, 256, 32);
#endif

	m_pFontUnitPosInfo->InitDeviceObjects(g_pD3dDev) ;


	// 2004-12-08 by jschoi
	pDataHeader = m_pGameData->Find("MainGB");
	m_pImgMainGageBack = new CINFImageEx;
	m_pImgMainGageBack->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("expbac");
	m_pImgMainExpBack = new CINFImageEx;
	m_pImgMainExpBack->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("expbar");
	m_pImgMainExpBar = new CINFImageEx;
	m_pImgMainExpBar->InitDeviceObjects( pDataHeader );
													 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = m_pGameData->Find("expbar1");
	m_pImgMainExpBar1 = new CINFImageEx;
	m_pImgMainExpBar1->InitDeviceObjects( pDataHeader );										  
#endif

	pDataHeader = m_pGameData->Find("speedm");
	m_pImgMainSpeedNum = new CINFImageEx;
	m_pImgMainSpeedNum->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("sMin");
	m_pImgMainMinus = new CINFImageEx;
	m_pImgMainMinus->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("warn");
	m_pImgMainWarning = new CINFImageEx;
	m_pImgMainWarning->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("capri");
	m_pImgGeneralPremiumCard = new CINFImageEx;
	m_pImgGeneralPremiumCard->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("caspri");
	m_pImgSuperPremiumCard = new CINFImageEx;
	m_pImgSuperPremiumCard->InitDeviceObjects( pDataHeader );
	
	pDataHeader = m_pGameData->Find("ehapp_h");
	m_pImgHappyHourEvent = new CINFImageEx;
	m_pImgHappyHourEvent->InitDeviceObjects( pDataHeader );
	
	pDataHeader = m_pGameData->Find("epchapp");
	m_pImgPCHappyHourEvent = new CINFImageEx;
	m_pImgPCHappyHourEvent->InitDeviceObjects( pDataHeader );
	
	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	pDataHeader = m_pGameData->Find("home_pre");
	m_pImgPCHomePremiumEvent = new CINFImageEx;
	m_pImgPCHomePremiumEvent->InitDeviceObjects( pDataHeader );
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업

	pDataHeader = m_pGameData->Find("spkicon");
	m_pImgVoiceSPKIcon = new CINFImageEx;
	m_pImgVoiceSPKIcon->InitDeviceObjects( pDataHeader );

	if(m_pImgVoiceChatIcon == NULL)
	{
		pDataHeader = m_pGameData->Find("voiceicon");
		m_pImgVoiceChatIcon = new CINFImageEx;
		m_pImgVoiceChatIcon->InitDeviceObjects( pDataHeader );
	}
	

	for(i=0;i<GAMEMAIN_GAGE_HP_MAX_GRADE;i++)
	{
		wsprintf( buf, "ene%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgMainGageHP[i] = new CINFImageEx;
		m_pImgMainGageHP[i]->InitDeviceObjects( pDataHeader );

		wsprintf( buf, "she%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgMainGageDP[i] = new CINFImageEx;
		m_pImgMainGageDP[i]->InitDeviceObjects( pDataHeader );

		wsprintf( buf, "skil%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgMainGageSP[i] = new CINFImageEx;
		m_pImgMainGageSP[i]->InitDeviceObjects( pDataHeader );	
	}

	for(i=0;i<GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;i++)
	{
		wsprintf( buf, "Bost%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgMainGageBooster[i] = new CINFImageEx;
		m_pImgMainGageBooster[i]->InitDeviceObjects( pDataHeader );	
	}

	for(i=0;i<GAMEMAIN_GAGE_FUEL_MAX_GRADE;i++)
	{
		wsprintf( buf, "fue%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgMainGageFuel[i] = new CINFImageEx;
		m_pImgMainGageFuel[i]->InitDeviceObjects( pDataHeader );	
	}

	for(i=0;i<2;i++)
	{
		wsprintf( buf, "fuelG%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgMainIconFuel[i] = new CINFImageEx;
		m_pImgMainIconFuel[i]->InitDeviceObjects( pDataHeader );	
	}

	for(i = 0; i < GAMEMAIN_EVENT_RECOVERY_MAX; i++)
	{
		if(m_pImgEventRecovery)
		{
			wsprintf( buf, "growth%d", i+1);
			pDataHeader = m_pGameData->Find(buf);
			if(pDataHeader)
			{
				m_pImgEventRecovery[i] = new CINFImageEx;
				m_pImgEventRecovery[i]->InitDeviceObjects( pDataHeader );
			}
		}
	}
	for(i=0;i<GIFT_ICON_IMG_COUNT;i++)
	{
		wsprintf( buf, "gift%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgGiftIcon[i] = new CINFImageEx;
		m_pImgGiftIcon[i]->InitDeviceObjects( pDataHeader );	
	}

	//2015-08-15 by St0rmy, better Ping Display
	for (i = 0; i < PING_DISPLAY_COUNT; i++)
	{
		wsprintf(buf, "ping%d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgPing[i] = new CINFImageEx;
		m_pImgPing[i]->InitDeviceObjects(pDataHeader);
	}
	
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	if(m_pImgBonusExpRate == NULL)
	{
		pDataHeader = m_pGameData->Find("bonus_e");
		m_pImgBonusExpRate = new CINFImageEx;
		m_pImgBonusExpRate->InitDeviceObjects( pDataHeader );
	}
 	if(m_pImgRestCount == NULL)
 	{
 		pDataHeader = m_pGameData->Find("rest_num");
 		m_pImgRestCount = new CINFImageEx;
 		m_pImgRestCount->InitDeviceObjects( pDataHeader );
 	}
	if(m_pImgKillCount == NULL)
	{
		pDataHeader = m_pGameData->Find("kill_num");
		m_pImgKillCount = new CINFImageEx;
		m_pImgKillCount->InitDeviceObjects( pDataHeader );
 	}
	if(m_pImgKillCount2 == NULL)
	{
		pDataHeader = m_pGameData->Find("kill_num2");
		m_pImgKillCount2 = new CINFImageEx;
		m_pImgKillCount2->InitDeviceObjects( pDataHeader );
	}
	if(m_pImgKillEff == NULL)
	{
		pDataHeader = m_pGameData->Find("kill_e");
		m_pImgKillEff = new CINFImageEx;
		m_pImgKillEff->InitDeviceObjects( pDataHeader );
	}


	if(m_pGroupImgRestCountBack == NULL)
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "restexp" );
		m_pGroupImgRestCountBack = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pGroupImgRestCountBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
	if(m_pGroupImgKillCountBack == NULL)
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "killcount" );
		m_pGroupImgKillCountBack = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pGroupImgKillCountBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	char szUpBtn[30] = {'\0', }, szDownBtn[30] = {'\0', }, szSelBtn[30] = {'\0', }, szDisBtn[30] = {'\0', };
	wsprintf(szUpBtn, "kill_on3");
	wsprintf(szDownBtn, "kill_on1");
	wsprintf(szSelBtn, "kill_on0");
	wsprintf(szDisBtn, "kill_on2");

	if(NULL == m_pImgRestCountOnBtn)
	{
		m_pImgRestCountOnBtn= new CINFImageBtn;		
		m_pImgRestCountOnBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP22");
	}

	if(NULL == m_pImgKillCountOnBtn)
	{
		m_pImgKillCountOnBtn= new CINFImageBtn;		
		m_pImgKillCountOnBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP22");
	}

	wsprintf(szUpBtn, "kill_off3");
	wsprintf(szDownBtn, "kill_off1");
	wsprintf(szSelBtn, "kill_off0");
	wsprintf(szDisBtn, "kill_off2");
	
	if(NULL == m_pImgRestCountOffBtn)
	{
		m_pImgRestCountOffBtn= new CINFImageBtn;		
		m_pImgRestCountOffBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP23");
	}
	
	if(NULL == m_pImgKillCountOffBtn)
	{
		m_pImgKillCountOffBtn= new CINFImageBtn;		
		m_pImgKillCountOffBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP23");
	}
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
	for (int i = 0; i < 10; i++)
	{
		char buf[16];
		wsprintf(buf, "B%d", i);
		m_fpsNumbers[i] = new CINFImageEx;
		pDataHeader = FindResource(buf);
		m_fpsNumbers[i]->InitDeviceObjects(pDataHeader);
	}
	
	m_pFontMapName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontMapName->InitDeviceObjects(g_pD3dDev);

	m_pFontCurrentEvent = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontCurrentEvent->InitDeviceObjects(g_pD3dDev);
	
	m_pFontEventHappyHour = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontEventHappyHour->InitDeviceObjects(g_pD3dDev);
	
	m_pFontEventRecovery = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontEventRecovery->InitDeviceObjects(g_pD3dDev);

	m_pFontDecaToolTip = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,1024,32);				// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
	m_pFontDecaToolTip->InitDeviceObjects(g_pD3dDev);

	return S_OK;
}

HRESULT CINFGameMainUnitInfoBar::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->RestoreDeviceObjects();
		m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->RestoreDeviceObjects();
		m_pImgBackMinimap->RestoreDeviceObjects();
#endif
		m_pFontUnitPosInfo->RestoreDeviceObjects();
		int i=0;
		for(i=0;i<GAMEMAIN_GAGE_LINE_NUMBER;i++)
		{
			m_pFontGageInfo[i]->RestoreDeviceObjects() ;
		}		

		// 2004-12-08 by jschoi
		m_pImgMainGageBack->RestoreDeviceObjects();
		m_pImgMainExpBack->RestoreDeviceObjects();
		m_pImgMainExpBar->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgMainExpBar1->RestoreDeviceObjects(); 
#endif
		m_pImgMainSpeedNum->RestoreDeviceObjects();
		m_pImgMainMinus->RestoreDeviceObjects();
		m_pImgMainWarning->RestoreDeviceObjects();
		m_pImgGeneralPremiumCard->RestoreDeviceObjects();
		m_pImgSuperPremiumCard->RestoreDeviceObjects();
		m_pImgHappyHourEvent->RestoreDeviceObjects();
		m_pImgPCHappyHourEvent->RestoreDeviceObjects();
		// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
		m_pImgPCHomePremiumEvent->RestoreDeviceObjects();
		//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
		m_pImgVoiceChatIcon->RestoreDeviceObjects();
		m_pImgVoiceSPKIcon->RestoreDeviceObjects();
		//m_pImgVoiceChatIcon->SetBtnPosition(GAMEMAIN_VOICECHAT_X,GAMEMAIN_VOICECHAT_Y);
		for(i = 0; i < GAMEMAIN_EVENT_RECOVERY_MAX; i++)
		{
			if(m_pImgEventRecovery[i])
				m_pImgEventRecovery[i]->RestoreDeviceObjects();
		}
		for(i = 0; i < GIFT_ICON_IMG_COUNT; i++)
		{
			if(m_pImgGiftIcon[i])
				m_pImgGiftIcon[i]->RestoreDeviceObjects();
		}
		for (i = 0; i < PING_DISPLAY_COUNT; i++)
		{
			if (m_pImgPing[i])
				m_pImgPing[i]->RestoreDeviceObjects();
		}
		for(i=0;i<GAMEMAIN_GAGE_HP_MAX_GRADE;i++)
		{
			m_pImgMainGageHP[i]->RestoreDeviceObjects();
			m_pImgMainGageDP[i]->RestoreDeviceObjects();
			m_pImgMainGageSP[i]->RestoreDeviceObjects();	
		}

		for(i=0;i<GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;i++)
		{
			m_pImgMainGageBooster[i]->RestoreDeviceObjects();	
		}

		for(i=0;i<GAMEMAIN_GAGE_FUEL_MAX_GRADE;i++)
		{
			m_pImgMainGageFuel[i]->RestoreDeviceObjects();	
		}
		
		for(i=0;i<2;i++)
		{
			m_pImgMainIconFuel[i]->RestoreDeviceObjects();
		}

		if(m_pFontMapName)
	         m_pFontMapName->RestoreDeviceObjects();

		if(m_pFontCurrentEvent)
			m_pFontCurrentEvent->RestoreDeviceObjects();
		if(m_pFontEventHappyHour)
			m_pFontEventHappyHour->RestoreDeviceObjects();
		if(m_pFontEventRecovery)
			m_pFontEventRecovery->RestoreDeviceObjects();		
		if(m_pFontDecaToolTip)							
			m_pFontDecaToolTip->RestoreDeviceObjects();						// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
			

		// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		if(m_pImgBonusExpRate)
			m_pImgBonusExpRate->RestoreDeviceObjects();
		if(m_pImgRestCount)
			m_pImgRestCount->RestoreDeviceObjects();
		if(m_pImgKillCount)
			m_pImgKillCount->RestoreDeviceObjects();
		if(m_pImgKillCount2)
			m_pImgKillCount2->RestoreDeviceObjects();
		if(m_pImgKillEff)
			m_pImgKillEff->RestoreDeviceObjects();
		if(m_pGroupImgRestCountBack)
			m_pGroupImgRestCountBack->RestoreDeviceObjects();	  
		if(m_pGroupImgKillCountBack)
			m_pGroupImgKillCountBack->RestoreDeviceObjects();	
#endif //S_BONUSEXPSYSTEM_RENEWAL
		// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		if(m_pImgRestCountOnBtn)
		{
			m_pImgRestCountOnBtn->RestoreDeviceObjects();			
		}
		if(m_pImgRestCountOffBtn)
		{
			m_pImgRestCountOffBtn->RestoreDeviceObjects();
		}
		if(m_pImgKillCountOnBtn)
		{
			m_pImgKillCountOnBtn->RestoreDeviceObjects();
		}
		if(m_pImgKillCountOffBtn)
		{
			m_pImgKillCountOffBtn->RestoreDeviceObjects();
		}
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		for (int i = 0; i < 10; i++)
		{
			m_fpsNumbers[i]->RestoreDeviceObjects();
		}

		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFGameMainUnitInfoBar::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->InvalidateDeviceObjects();
		m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->InvalidateDeviceObjects();
		m_pImgBackMinimap->InvalidateDeviceObjects();
#endif
		m_pFontUnitPosInfo->InvalidateDeviceObjects();
		int i=0;
		for(i=0;i<GAMEMAIN_GAGE_LINE_NUMBER;i++)
		{
			m_pFontGageInfo[i]->InvalidateDeviceObjects() ;
		}

		// 2004-12-08 by jschoi
		m_pImgMainGageBack->InvalidateDeviceObjects();
		m_pImgMainExpBack->InvalidateDeviceObjects();
		m_pImgMainExpBar->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgMainExpBar1->InvalidateDeviceObjects();
#endif
		m_pImgMainSpeedNum->InvalidateDeviceObjects();
		m_pImgMainMinus->InvalidateDeviceObjects();
		m_pImgMainWarning->InvalidateDeviceObjects();
		m_pImgGeneralPremiumCard->InvalidateDeviceObjects();
		m_pImgSuperPremiumCard->InvalidateDeviceObjects();
		m_pImgHappyHourEvent->InvalidateDeviceObjects();
		m_pImgPCHappyHourEvent->InvalidateDeviceObjects();
		// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
		m_pImgPCHomePremiumEvent->InvalidateDeviceObjects();
		//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
		m_pImgVoiceChatIcon->InvalidateDeviceObjects();
		m_pImgVoiceSPKIcon->InvalidateDeviceObjects();
		for(i = 0; i < GAMEMAIN_EVENT_RECOVERY_MAX; i++)
		{
			if(m_pImgEventRecovery[i])
				m_pImgEventRecovery[i]->InvalidateDeviceObjects();
		}
		for(i = 0; i < GIFT_ICON_IMG_COUNT; i++)
		{
			if(m_pImgGiftIcon[i])
				m_pImgGiftIcon[i]->InvalidateDeviceObjects();
		}
		for (i = 0; i < PING_DISPLAY_COUNT; i++)
		{
			if (m_pImgPing[i])
				m_pImgPing[i]->InvalidateDeviceObjects();
		}
		for(i=0;i<GAMEMAIN_GAGE_HP_MAX_GRADE;i++)
		{
			m_pImgMainGageHP[i]->InvalidateDeviceObjects();
			m_pImgMainGageDP[i]->InvalidateDeviceObjects();
			m_pImgMainGageSP[i]->InvalidateDeviceObjects();	
		}

		for(i=0;i<GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;i++)
		{
			m_pImgMainGageBooster[i]->InvalidateDeviceObjects();	
		}

		for(i=0;i<GAMEMAIN_GAGE_FUEL_MAX_GRADE;i++)
		{
			m_pImgMainGageFuel[i]->InvalidateDeviceObjects();	
		}
		
		for(i=0;i<2;i++)
		{
			m_pImgMainIconFuel[i]->InvalidateDeviceObjects();
		}

		if(m_pFontMapName )
			m_pFontMapName->InvalidateDeviceObjects();

		if(m_pFontCurrentEvent)
			m_pFontCurrentEvent->InvalidateDeviceObjects();
		if(m_pFontEventHappyHour)
			m_pFontEventHappyHour->InvalidateDeviceObjects();
		if(m_pFontEventRecovery)
			m_pFontEventRecovery->InvalidateDeviceObjects();		
		if(m_pFontDecaToolTip)
			m_pFontDecaToolTip->InvalidateDeviceObjects();				// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		if(m_pGageBarInfo)
			m_pGageBarInfo->InvalidateDeviceObjects();											  
#endif

		// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		if(m_pImgBonusExpRate)
			m_pImgBonusExpRate->InvalidateDeviceObjects();
		if(m_pImgRestCount)
			m_pImgRestCount->InvalidateDeviceObjects();
		if(m_pImgKillCount)
			m_pImgKillCount->InvalidateDeviceObjects();
		if(m_pImgKillCount2)
			m_pImgKillCount2->InvalidateDeviceObjects();
		if(m_pImgKillEff)
			m_pImgKillEff->InvalidateDeviceObjects();
			
		if(m_pGroupImgRestCountBack)
			m_pGroupImgRestCountBack->InvalidateDeviceObjects();
		if(m_pGroupImgKillCountBack)
			m_pGroupImgKillCountBack->InvalidateDeviceObjects();
#endif //S_BONUSEXPSYSTEM_RENEWAL
		// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		if(m_pImgRestCountOnBtn)
		{
			m_pImgRestCountOnBtn->InvalidateDeviceObjects();			
		}
		if(m_pImgRestCountOffBtn)
		{
			m_pImgRestCountOffBtn->InvalidateDeviceObjects();
		}
		if(m_pImgKillCountOnBtn)
		{
			m_pImgKillCountOnBtn->InvalidateDeviceObjects();
		}
		if(m_pImgKillCountOffBtn)
		{
			m_pImgKillCountOffBtn->InvalidateDeviceObjects();
		}
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		for (int i = 0; i < 10; i++)
		{
			m_fpsNumbers[i]->InvalidateDeviceObjects();
		}
		m_bRestored = FALSE;
	}
	return S_OK;
}

HRESULT CINFGameMainUnitInfoBar::DeleteDeviceObjects()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->DeleteDeviceObjects();
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->DeleteDeviceObjects();
	m_pImgBackMinimap->DeleteDeviceObjects();
#endif
	m_pFontUnitPosInfo->DeleteDeviceObjects();

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]);
	util::del(m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]);
	util::del(m_pImgBackMinimap);
#endif
	util::del(m_pFontUnitPosInfo);

	int i=0;
	for(i=0;i<GAMEMAIN_GAGE_LINE_NUMBER;i++)
	{
		m_pFontGageInfo[i]->DeleteDeviceObjects() ;
		util::del(m_pFontGageInfo[i]);
	}	

	// 2004-12-08 by jschoi
	m_pImgMainGageBack->DeleteDeviceObjects();
	util::del(m_pImgMainGageBack);
	m_pImgMainExpBack->DeleteDeviceObjects();
	util::del(m_pImgMainExpBack);
	m_pImgMainExpBar->DeleteDeviceObjects();
	util::del(m_pImgMainExpBar);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgMainExpBar1->DeleteDeviceObjects();
	util::del(m_pImgMainExpBar1);																  
#endif
	m_pImgMainSpeedNum->DeleteDeviceObjects();
	util::del(m_pImgMainSpeedNum);
	m_pImgMainMinus->DeleteDeviceObjects();
	util::del(m_pImgMainMinus);
	for(i=0;i<GAMEMAIN_GAGE_HP_MAX_GRADE;i++)
	{
		m_pImgMainGageHP[i]->DeleteDeviceObjects();
		util::del(m_pImgMainGageHP[i]);
		m_pImgMainGageDP[i]->DeleteDeviceObjects();
		util::del(m_pImgMainGageDP[i]);
		m_pImgMainGageSP[i]->DeleteDeviceObjects();	
		util::del(m_pImgMainGageSP[i]);
	}

	for(i=0;i<GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;i++)
	{
		m_pImgMainGageBooster[i]->DeleteDeviceObjects();	
		util::del(m_pImgMainGageBooster[i]);
	}

	for(i=0;i<GAMEMAIN_GAGE_FUEL_MAX_GRADE;i++)
	{
		m_pImgMainGageFuel[i]->DeleteDeviceObjects();	
		util::del(m_pImgMainGageFuel[i]);
	}

	for(i=0;i<2;i++)
	{
		m_pImgMainIconFuel[i]->DeleteDeviceObjects();
		util::del(m_pImgMainIconFuel[i]);
	}

	if(m_pFontMapName ) 
	{
		m_pFontMapName->DeleteDeviceObjects() ;
		util::del(m_pFontMapName ) ; 
	}

	if(m_pFontCurrentEvent)
	{
		m_pFontCurrentEvent->DeleteDeviceObjects() ;
		util::del(m_pFontCurrentEvent ) ; 
	}

	if(m_pFontEventHappyHour)
	{
		m_pFontEventHappyHour->DeleteDeviceObjects() ;
		util::del(m_pFontEventHappyHour ) ; 
	}
	if(m_pFontEventRecovery)
	{
		m_pFontEventRecovery->DeleteDeviceObjects();
		util::del(m_pFontEventRecovery);
	}
	if(m_pFontDecaToolTip)
	{
		m_pFontDecaToolTip->DeleteDeviceObjects();						// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
		util::del(m_pFontDecaToolTip);
	}
	for(i = 0; i < GAMEMAIN_EVENT_RECOVERY_MAX; i++)
	{
		if(m_pImgEventRecovery[i])
		{
			m_pImgEventRecovery[i]->DeleteDeviceObjects();
			util::del(m_pImgEventRecovery[i]);
		}
	}
	for(i = 0; i < GIFT_ICON_IMG_COUNT; i++)
	{
		if(m_pImgGiftIcon[i])
		{
			m_pImgGiftIcon[i]->DeleteDeviceObjects();
			util::del(m_pImgGiftIcon[i]);
		}
	}	
	for (i = 0; i < PING_DISPLAY_COUNT; i++)
	{
		if (m_pImgPing[i])
		{
			m_pImgPing[i]->DeleteDeviceObjects();
			util::del(m_pImgPing[i]);
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pGageBarInfo->DeleteDeviceObjects();
	util::del(m_pGageBarInfo);																  
#endif
	
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	if(m_pImgRestCount)
	{
		m_pImgRestCount->DeleteDeviceObjects();
		util::del(m_pImgRestCount);
	}

	if(m_pImgKillCount)
	{
		m_pImgKillCount->DeleteDeviceObjects();
		util::del(m_pImgKillCount);
	}
	if(m_pImgKillCount2)
	{
		m_pImgKillCount2->DeleteDeviceObjects();
		util::del(m_pImgKillCount2);
	}
	if(m_pImgKillEff)
	{
		m_pImgKillEff->DeleteDeviceObjects();
		util::del(m_pImgKillEff);
	}
	if(m_pGroupImgRestCountBack)
	{
		m_pGroupImgRestCountBack->DeleteDeviceObjects();
		util::del(m_pGroupImgRestCountBack);
	}
	if(m_pGroupImgKillCountBack)
	{
		m_pGroupImgKillCountBack->DeleteDeviceObjects();
		util::del(m_pGroupImgKillCountBack);
	}
#endif //S_BONUSEXPSYSTEM_RENEWAL
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	if(m_pImgRestCountOnBtn)
	{
		m_pImgRestCountOnBtn->DeleteDeviceObjects();
		util::del(m_pImgRestCountOnBtn);
	}
	if(m_pImgRestCountOffBtn)
	{
		m_pImgRestCountOffBtn->DeleteDeviceObjects();
		util::del(m_pImgRestCountOffBtn);
	}
	if(m_pImgKillCountOnBtn)
	{
		m_pImgKillCountOnBtn->DeleteDeviceObjects();
		util::del(m_pImgKillCountOnBtn);
	}
	if(m_pImgKillCountOffBtn)
	{
		m_pImgKillCountOffBtn->DeleteDeviceObjects();
		util::del(m_pImgKillCountOffBtn);
	}
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
	for (int i = 0; i < 10; i++)
	{
		m_fpsNumbers[i]->DeleteDeviceObjects();
		util::del(m_fpsNumbers[i]);
	}

	return S_OK;
}


void CINFGameMainUnitInfoBar::Render()//RenderCityUpInfo() 에서 같은 코드 사용 : 수정시 주의할것
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT BGPos = m_pGageBarInfo->GetFindControlTargetofMinPos("MainGB");
 	m_pImgMainGageBack->Move(BGPos.x + GAMEMAIN_POS_X,BGPos.y + GAMEMAIN_POS_Y);
 	m_pImgMainGageBack->Render();
// 
	//wsw render kill bar count
#ifdef _WARRIOR_CLASS_KILL_BAR
	if (g_pShuttleChild->m_myShuttleInfo.Level < CHARACTER_MAX_LEVEL)
	{
		m_pImgMainExpBack->Move(WSW_BACK_KILL_BAR_X, WSW_BACK_KILL_BAR_Y);

		m_pImgMainExpBack->SetScale(WSW_BACK_KILL_BAR_X - ((g_pD3dApp->GetBackBufferDesc().Width / 2.0f) - 474), 1);
		m_pImgMainExpBack->Render();
	}
#endif
// 	// 경험치 배경
 	m_pImgMainExpBack->Move( 0 , GAMEMAIN_BACK_GAGE_EXP_START_Y );
	m_pImgMainExpBack->SetScale( g_pD3dApp->GetBackBufferDesc().Width, 1 );
 	m_pImgMainExpBack->Render();
// 
 	RenderMainGage();
// 
 	RenderSpeed();
// 	// gage string
 	RenderGageString();
// 	
 	const UINT nMapPosX = g_pD3dApp->GetBackBufferDesc().Width - 110;
 	const UINT nMapPosY = 20;
 	RenderMapPosition(nMapPosX,nMapPosY);
 
 	UINT nStartX = g_pD3dApp->GetBackBufferDesc().Width;
 	UINT nStartY = 4;
 
	MAP_INFO* mapname = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
 	if(mapname)
 	{
 		nStartX -= (strlen(mapname->MapName)*6 + 30);
 		ShowMapNameTitle(nStartX,nStartY);
 	}
 
 	nStartX -= 110;
 	RenderCurrentEvent(nStartX, nStartY);		//도시 점령전이나 PK지역 렌더링.
	// 2013-03-26 by bhsohn 멤버쉽 정보가 캐시샵 버튼과 해피아워 로고 뒤에 있음
//	RenderPremiumCard(FALSE);					// 일반
	// END 2013-03-26 by bhsohn 멤버쉽 정보가 캐시샵 버튼과 해피아워 로고 뒤에 있음
	RenderHappyHourEvent(FALSE);
	RenderEventRecovery();						// 2006-04-24 by ispark, 이벤트 회복율
	// 2013-03-21 by bhsohn 해피아워 스트링 툴팁 순서 문제 처리
//	RenderStringBar();							// 2006-06-08 by ispark, 이벤트 설명
	// END 2013-03-21 by bhsohn 해피아워 스트링 툴팁 순서 문제 처리
	RenderGiftIcon(FALSE);
	// end 2011. 10. 10 by jskim UI시스템 변경					
#else
	// down window - 아래 까만줄
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->Move(0, GAMEMAIN_BACK_DOWN_START_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->SetScale(g_pD3dApp->GetBackBufferDesc().Width, GAMEMAIN_BACK_DOWN_SIZE_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->Render();
	
	// 상점에선 기타 정보 렌더링을 안한다
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return;

	// up window - 위에 까만줄
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->Move(GAMEMAIN_BACK_GAGE_UP_SIZE_X, 0);
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->SetScale(g_pD3dApp->GetBackBufferDesc().Width-GAMEMAIN_BACK_GAGE_UP_SIZE_X-GAMEMAIN_BACK_MINIMAP_SIZE_X,GAMEMAIN_BACK_UP_SIZE_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->Render();
	
	
	// 메인 배경
	m_pImgMainGageBack->Move(0,0);
	m_pImgMainGageBack->Render();

	// 경험치 배경
	m_pImgMainExpBack->Move(GAMEMAIN_BACK_GAGE_EXP_START_X,GAMEMAIN_BACK_GAGE_EXP_START_Y);
	m_pImgMainExpBack->Render();

	RenderMainGage();

	RenderSpeed();

	// minimap back
	m_pImgBackMinimap->Move(GAMEMAIN_BACK_MINIMAP_START_X, GAMEMAIN_BACK_MINIMAP_START_Y);
	m_pImgBackMinimap->Render();

	// gage string
	RenderGageString();
	
	const UINT nMapPosX = g_pD3dApp->GetBackBufferDesc().Width - 110;
	const UINT nMapPosY = 20;
	RenderMapPosition(nMapPosX,nMapPosY);

	UINT nStartX = g_pD3dApp->GetBackBufferDesc().Width;
	UINT nStartY = 4;

	MAP_INFO* mapname = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	if(mapname)
	{
		nStartX -= (strlen(mapname->MapName)*6 + 30);
		ShowMapNameTitle(nStartX,nStartY);
	}

	nStartX -= 110;
	RenderCurrentEvent(nStartX, nStartY);		//도시 점령전이나 PK지역 렌더링.


	RenderPremiumCard(FALSE);					// 일반
	RenderHappyHourEvent(FALSE);
	RenderEventRecovery();						// 2006-04-24 by ispark, 이벤트 회복율
	RenderStringBar();							// 2006-06-08 by ispark, 이벤트 설명
	RenderGiftIcon(FALSE);
#endif
	// 2006-04-25 by ispark, 해피 아우어 스트링을 함수 안으로 옮겼다.	

	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL

	if(g_pGameMain && g_pGameMain->m_pMiniMap && 
		(g_pGameMain->m_pMiniMap->IsRenderBigMap()))
	{
		RenderRestKillCount(false);
	}
	else
	{
		RenderRestKillCount(true);
	}

	RenderBonusExpRate();
#endif // S_BONUSEXPSYSTEM_RENEWAL
	int startX = (g_pD3dApp->GetBackBufferDesc().Width - 40);
	int nValue = (int)round(g_pD3dApp->GetFPS());
	bool display = false;
	const int numDigits = NumDigits(nValue);

	for (int i = 0; i < numDigits; i++)
	{
		int nNum = nValue % 10;

#ifdef _SHOW_LATENCY
		m_fpsNumbers[nNum]->Move(startX + (m_fpsNumbers[nNum]->GetImgSize().x - 3) *(1 - i), 140);
#else
		m_fpsNumbers[nNum]->Move(startX + (m_fpsNumbers[nNum]->GetImgSize().x - 3) *(1 - i), 160);
#endif
		m_fpsNumbers[nNum]->Render();

		nValue /= 10;
	}

#ifdef _SHOW_LATENCY
	//Get Ping
	float fPing = g_pD3dApp->m_fLatency;
	//2015-08-15 by St0rmy, better Ping Display
	if (fPing <= PING_LIMIT_GOOD) //0-50 ms
	{
		if (m_pImgPing[PING_STATUS_GOOD]) //Prevent errors when not loaded or NULL
		{
			m_pImgPing[PING_STATUS_GOOD]->Move(PING_DISPLAY_POS_X, PING_DISPLAY_POS_Y);
			m_pImgPing[PING_STATUS_GOOD]->Render();
		}
	}
	else if (fPing <= PING_LIMIT_MEDIUM) //50-100 ms
	{
		if (m_pImgPing[PING_STATUS_MEDIUM]) //Prevent errors when not loaded or NULL
		{
			m_pImgPing[PING_STATUS_MEDIUM]->Move(PING_DISPLAY_POS_X, PING_DISPLAY_POS_Y);
			m_pImgPing[PING_STATUS_MEDIUM]->Render();
		}
	}
	else if (fPing <= PING_LIMIT_BAD) //100-150 ms
	{
		if (m_pImgPing[PING_STATUS_BAD]) //Prevent errors when not loaded or NULL
		{
			m_pImgPing[PING_STATUS_BAD]->Move(PING_DISPLAY_POS_X, PING_DISPLAY_POS_Y);
			m_pImgPing[PING_STATUS_BAD]->Render();
		}
	}
	else if (fPing > PING_LIMIT_BAD) //150-999 ms
	{
		if (m_pImgPing[PING_STATUS_WORST]) //Prevent errors when not loaded or NULL
		{
			m_pImgPing[PING_STATUS_WORST]->Move(PING_DISPLAY_POS_X, PING_DISPLAY_POS_Y);
			m_pImgPing[PING_STATUS_WORST]->Render();
		}
	}
	//2015-08-15 by St0rmy, end
#endif
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
}
void CINFGameMainUnitInfoBar::RenderGiftIcon(BOOL i_bCity)
{
	if(m_vecGiftMsg.size())
	{	
// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
//		if(i_bCity)
//			m_nGiftY = g_pD3dApp->GetBackBufferDesc().Height - INFOMAIN_GIFT_CITY_ICON_Y;
//		else
			m_nGiftY = g_pD3dApp->GetBackBufferDesc().Height - INFOMAIN_GIFT_FIELD_ICON_Y;
//		if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//			m_nGiftY -= GAME_EVENT_BASICPOS_Y;
		m_ptGift = m_pImgGiftIcon[0]->GetImgSize();
		m_nGiftY -= m_ptGift.y;
		m_nGiftX = g_pD3dApp->GetBackBufferDesc().Width - INFOMAIN_GIFT_ICON_X;
		// 깜빡임 처리.(0.7초에 한번씩)
		m_fGiftTime += g_pD3dApp->GetElapsedTime();
		float ftime = m_fGiftTime * 10;
		ftime = (int)ftime % 14;
		if(ftime > 7)
		{
			m_pImgGiftIcon[0]->Move(m_nGiftX, m_nGiftY);
			m_pImgGiftIcon[0]->Render();
		}
		else
		{
			m_pImgGiftIcon[1]->Move(m_nGiftX, m_nGiftY);
			m_pImgGiftIcon[1]->Render();
		}
	}	

}
void CINFGameMainUnitInfoBar::HideRender()
{
	// up window - 위에 까만줄
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->Move(GAMEMAIN_BACK_GAGE_UP_SIZE_X, 0);
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->SetScale(g_pD3dApp->GetBackBufferDesc().Width-GAMEMAIN_BACK_GAGE_UP_SIZE_X-GAMEMAIN_BACK_MINIMAP_SIZE_X,GAMEMAIN_BACK_UP_SIZE_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->Render();
#endif
	
	// 2004-12-08 by jschoi 
	// 메인 배경
	m_pImgMainGageBack->Move(0,0);
	m_pImgMainGageBack->Render();

	RenderMainGage(FALSE);

	RenderSpeed();

	// minimap back
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBackMinimap->Move(GAMEMAIN_BACK_MINIMAP_START_X, GAMEMAIN_BACK_MINIMAP_START_Y);
	m_pImgBackMinimap->Render();
#endif

	// gage string
	RenderGageString(FALSE);

	const UINT nMapPosX = g_pD3dApp->GetBackBufferDesc().Width - 110;
	const UINT nMapPosY = 20;
	RenderMapPosition(nMapPosX,nMapPosY);

	UINT nStartX = g_pD3dApp->GetBackBufferDesc().Width;
	UINT nStartY = 4;

	MAP_INFO* mapname = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	if(mapname)
	{
		nStartX -= (strlen(mapname->MapName)*6 + 30);
		ShowMapNameTitle(nStartX,nStartY);
	}

	nStartX -= 110;
	RenderCurrentEvent(nStartX, nStartY);
	
}

void CINFGameMainUnitInfoBar::RenderCityUpInfo()
{
	// up window
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->Move(GAMEMAIN_BACK_GAGE_UP_SIZE_X, 0);
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->SetScale(g_pD3dApp->GetBackBufferDesc().Width - GAMEMAIN_BACK_GAGE_UP_SIZE_X,GAMEMAIN_BACK_UP_SIZE_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_UP_PIXEL]->Render();
	// down window - 아래 까만줄
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->Move(0, GAMEMAIN_BACK_DOWN_START_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->SetScale(g_pD3dApp->GetBackBufferDesc().Width, GAMEMAIN_BACK_DOWN_SIZE_Y);
	m_pImgBackPixel[GAMEMAIN_BACK_DOWN_PIXEL]->Render();
#endif
	
	// 2004-12-08 by jschoi 
	// 메인 배경
	m_pImgMainGageBack->Move(0,0);
	m_pImgMainGageBack->Render();

	// 경험치 배경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgMainExpBack->Move( 0, GAMEMAIN_BACK_GAGE_EXP_START_Y);
#else 
	m_pImgMainExpBack->Move(GAMEMAIN_BACK_GAGE_EXP_START_X,GAMEMAIN_BACK_GAGE_EXP_START_Y);
#endif
	m_pImgMainExpBack->Render();

	RenderMainGage();

	RenderSpeed();

	// gage string
	RenderGageString();


	UINT nStartX = g_pD3dApp->GetBackBufferDesc().Width;
	UINT nStartY = 4;

//	if(RenderCurrentEvent())
//	{
//		nStartX = g_pD3dApp->GetBackBufferDesc().Width - 160;
//	}
//	else
//	{
//		nStartX = g_pD3dApp->GetBackBufferDesc().Width - 30;
//	}
//
//	nStartY = 4;


	// 맵이름 표시
	MAP_INFO* mapname = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	if(mapname)
	{
		nStartX -= (strlen(mapname->MapName)*6 + 30);
		ShowMapNameTitle(nStartX,nStartY);
	}

	// 2005-02-18 by jschoi - Premium Card Render
	RenderPremiumCard(TRUE);	// 도시
	// Render HappyHour
	RenderHappyHourEvent(TRUE);
	// 2006-08-29 by dgwoo RenderTextEvent
	RenderTextHappyHourEvent();
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	RenderBonusExpRate();
#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
}

///////////////////////////////////////////////////////////////////////////////
/// \class		void CINFGameMainUnitInfoBar::RenderTextHappyHourEvent()
/// \brief		해피아워 이벤트 관련 텍스트를 찍는다.
///
/// \author		dgwoo
/// \version	
/// \date		2006-08-29 ~ 2006-08-29
/// \warning	
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::RenderTextHappyHourEvent()
{

	int Height = 0;
	int leng = 0;
	vector<string>::iterator itU = m_vecEventHappyHour.begin();
	while (itU != m_vecEventHappyHour.end())
	{
		if(strlen((char*)(*itU).c_str())>leng)
		{
			leng = strlen((char*)(*itU).c_str());
		}
		itU++;
	}
	leng = leng*6.5;

	if(m_bRenderEvent == TRUE)
	{
		int nIdex = 0;
		vector<string>::iterator it = m_vecEventHappyHour.begin();
		while (it != m_vecEventHappyHour.end())
		{
			g_pGameMain->RenderPopUpWindowImage(m_nHappyHourX, m_nHappyHourY-20-(nIdex*20), leng, 1);
			m_pFontEventHappyHour->DrawText(m_nHappyHourX, m_nHappyHourY-20-(nIdex*20), GUI_FONT_COLOR_Y, (char*)(*it).c_str());
			
			it++;
			nIdex++;
		}
	}
}

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CINFGameMainUnitInfoBar::RenderBonusExpRate()
{
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	int nBonusExpX = g_pD3dApp->GetBackBufferDesc().Width - BONUS_EXP_RATE_POS_X;
	int nBonusExpY = g_pD3dApp->GetBackBufferDesc().Height - BONUS_EXP_RATE_POS_Y;
			
	m_pImgBonusExpRate->Move(nBonusExpX, nBonusExpY);
	m_pImgBonusExpRate->Render();
	if(m_bRenderBonusExpRateText)
	{
		RenderBonusExpRateText();
	}
#endif // S_BONUSEXPSYSTEM_RENEWAL
}
void CINFGameMainUnitInfoBar::RenderBonusExpRateText()
{
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	int nBonusExpX = g_pD3dApp->GetBackBufferDesc().Width - BONUS_EXP_RATE_TEXT_POS_X;
	int nBonusExpY = g_pD3dApp->GetBackBufferDesc().Height - BONUS_EXP_RATE_TEXT_POS_Y;

	char strExpRate[3][256];
	// 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	double fRate = 0.0f;
	sprintf(strExpRate[0],STRMSG_C_121013_0001,(double)m_nPartyRate);
	sprintf(strExpRate[1],STRMSG_C_121013_0002,(double)m_nGuildRate);
	fRate = ((double)m_nFriendRate)/10.0;
	sprintf(strExpRate[2],STRMSG_C_121013_0003,fRate);
	// end 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	int nMaxLineLength = 0,i;
	for(i = 0 ; i < 3 ; i++)
	{
		int temp = strlen(strExpRate[i]);
		nMaxLineLength	= max(nMaxLineLength,temp);
	}
	
	for(i = 0 ; i < 3 ; i++)
	{
		g_pGameMain->RenderPopUpWindowImage(nBonusExpX, nBonusExpY-(BONUS_EXP_RATE_TEXT_LINE_HEIGHT * i), nMaxLineLength * 6.5f, 1);
		m_pFontEventHappyHour->DrawText(nBonusExpX, nBonusExpY-(BONUS_EXP_RATE_TEXT_LINE_HEIGHT * i) - 2, GUI_FONT_COLOR_Y, strExpRate[i]);

	}
#endif //S_BONUSEXPSYSTEM_RENEWAL
}
void CINFGameMainUnitInfoBar::RenderRestKillCount(BOOL bIsMinimap)
{
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	int nRestCount = m_nRestCount;
	int nKillCount = m_nKillCount;

	int nMaxNum;
	UINT nPosX,nPosY,nNum;
	UINT nMapPosX;
 	UINT nMapPosY;

	nMapPosX = g_pD3dApp->GetBackBufferDesc().Width - RESTKILLCOUNT_BASE_POS_X;
	nMapPosY = RESTKILLCOUNT_BASE_POS_Y;

	// 휴식 경험치 카운트
	if(nRestCount != 0)
	{
		// 자리수 계산
		nMaxNum = GetMaxSize(nRestCount);
		if(nMaxNum > RESTCOUNT_MAX_POSNUM)
		{
			nMaxNum = RESTCOUNT_MAX_POSNUM;
			nRestCount = RESTCOUNT_MAX_COUNT;
		}

		// 배경 랜더
		//nPosX = nMapPosX - RESTCOUNT_MAX_POSNUM*RESTCOUNT_STRING_WIDTH*RESTCOUNT_SCALE_DEFAULT + RESTCOUNT_BACK_POS_X;
		nPosX = nMapPosX - RESTCOUNT_MAX_POSNUM*RESTCOUNT_STRING_WIDTH*RESTCOUNT_SCALE_DEFAULT + RESTCOUNT_BACK_POS_X + m_fPlusRestCountPosX;	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		nPosY = nMapPosY - RESTCOUNT_BACK_POS_Y;

		m_pGroupImgRestCountBack->Move(nPosX,nPosY);
		m_pGroupImgRestCountBack->Render();		

		// 숫자 랜더
		//nPosX = nMapPosX - nMaxNum*RESTCOUNT_STRING_WIDTH*RESTCOUNT_SCALE_DEFAULT + RESTCOUNT_TEXT_POS_X;
		nPosX = nMapPosX - nMaxNum*RESTCOUNT_STRING_WIDTH*RESTCOUNT_SCALE_DEFAULT + RESTCOUNT_TEXT_POS_X + m_fPlusRestCountPosX;	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		nPosY = nMapPosY + RESTCOUNT_TEXT_POS_Y;

		while(nMaxNum > 0)
		{
			nNum = CutOffFirstNumber(nRestCount,nMaxNum);
			m_pImgRestCount->Move(nPosX,nPosY);
			m_pImgRestCount->SetRect(nNum*RESTCOUNT_STRING_WIDTH,0,(nNum+1)*RESTCOUNT_STRING_WIDTH,RESTCOUNT_STRING_HEIGHT);	
			m_pImgRestCount->SetScale(RESTCOUNT_SCALE_DEFAULT,RESTCOUNT_SCALE_DEFAULT);
			m_pImgRestCount->Render();		
			nPosX += RESTCOUNT_STRING_WIDTH*RESTCOUNT_SCALE_DEFAULT; 
			nMaxNum--;
		}

		// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		m_nRestCountPosX  = g_pD3dApp->GetBackBufferDesc().Width - RESTKILLCOUNT_BUTTON_BASE_POS_X;
		m_nRestCountPosY  = nMapPosY - RESTCOUNT_BACK_POS_Y;
		
		if(m_bCheckRestCountON)
		{
			m_pImgRestCountOffBtn->SetBtnPosition(m_nRestCountPosX,m_nRestCountPosY);
			m_pImgRestCountOffBtn->Render();
		}
		else
		{
			m_pImgRestCountOnBtn->SetBtnPosition(m_nRestCountPosX,m_nRestCountPosY);
			m_pImgRestCountOnBtn->Render();
		}
		// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼

		// 킬카운트와의 간격 처리
		nMapPosY += RESTCOUNT_STRING_HEIGHT + RESTKILLCOUNT_BASE_HEIGHT_SIZE;
	}	

	// 킬 카운트
	if(nKillCount != 0)
	{
		// 자리수 계산
		nMaxNum = GetMaxSize(nKillCount);
		if(nMaxNum > KILLCOUNT_MAX_POSNUM)
		{
			nMaxNum = KILLCOUNT_MAX_POSNUM;
			nKillCount = KILLCOUNT_MAX_COUNT;
		}
		
		// 배경 랜더 위치 계산
		//nPosX = nMapPosX - RESTCOUNT_MAX_POSNUM*RESTCOUNT_STRING_WIDTH*KILLCOUNT_SCALE_DEFAULT + KILLCOUNT_BACK_POS_X;
		nPosX = nMapPosX - RESTCOUNT_MAX_POSNUM*RESTCOUNT_STRING_WIDTH*KILLCOUNT_SCALE_DEFAULT + KILLCOUNT_BACK_POS_X + m_fPlusKillCountPosX;	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		nPosY = nMapPosY - KILLCOUNT_BACK_POS_Y;
		
		// 300 카운트 시의 숫자 랜더 처리
		if(m_bCompleteKillCount && m_nCompleteKillCountBlank < KILLCOUNT2_EFF_MAX_BLENK_COUNT)
		{
			// 깜박임 이펙트 랜더
			m_pImgKillEff->Move(nPosX-KILLCOUNT2_EFF_POS_X,nPosY-KILLCOUNT2_EFF_POS_Y);
			m_pImgKillEff->SetColor(D3DCOLOR_ARGB(m_nCompleteKillCountAlpha,255,255,255));
			m_pImgKillEff->Render();		

			if(m_nCompleteKillCountBlank == KILLCOUNT2_EFF_MAX_BLENK_COUNT-1) // 킬 카운트가 사라질때 같이 사라지게 한다.
			{
				m_pGroupImgKillCountBack->SetColor(D3DCOLOR_ARGB(m_nCompleteKillCountAlpha,255,255,255));
			}
			// 배경 랜더
			m_pGroupImgKillCountBack->Move(nPosX,nPosY);
			m_pGroupImgKillCountBack->Render();		


			// 숫자 랜더
			nKillCount = KILLCOUNT_MAX_COUNT;
			//nPosX = nMapPosX - nMaxNum*KILLCOUNT2_STRING_WIDTH + KILLCOUNT2_TEXT_POS_X;
			nPosX = nMapPosX - nMaxNum*KILLCOUNT2_STRING_WIDTH + KILLCOUNT2_TEXT_POS_X + m_fPlusKillCountPosX;	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
			nPosY = nMapPosY - KILLCOUNT2_TEXT_POS_Y;

			while(nMaxNum > 0)
			{
				nNum = CutOffFirstNumber(nKillCount,nMaxNum);
				m_pImgKillCount2->Move(nPosX,nPosY);
				m_pImgKillCount2->SetColor(D3DCOLOR_ARGB(m_nCompleteKillCountAlpha,255,255,255));
				m_pImgKillCount2->SetRect(nNum*KILLCOUNT2_STRING_WIDTH,0,(nNum+1)*KILLCOUNT2_STRING_WIDTH,KILLCOUNT2_STRING_HEIGHT);	
				if(m_nCompleteKillCountBlank == KILLCOUNT2_EFF_MAX_BLENK_COUNT-1)
				{
					m_pImgKillCount2->SetColor(D3DCOLOR_ARGB(m_nCompleteKillCountAlpha,255,255,255));
				}
				m_pImgKillCount2->Render();		
				nPosX += KILLCOUNT2_STRING_WIDTH; 
				nMaxNum--;
			}
		}
		// 300 미만일때의 숫자 랜더 처리
 		else if(!m_bCompleteKillCount)
 		{
			// 배경 랜더
			m_pGroupImgKillCountBack->Move(nPosX,nPosY);
			m_pGroupImgKillCountBack->SetColor(D3DCOLOR_ARGB(255,255,255,255));
			m_pGroupImgKillCountBack->Render();		

			// 숫자 랜더
			//nPosX = nMapPosX - nMaxNum*RESTCOUNT_STRING_WIDTH*m_fKillCountScale + KILLCOUNT_TEXT_POS_X;
			nPosX = nMapPosX - nMaxNum*RESTCOUNT_STRING_WIDTH*m_fKillCountScale + KILLCOUNT_TEXT_POS_X + m_fPlusKillCountPosX;	// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
			nPosY = nMapPosY + KILLCOUNT_TEXT_POS_Y;
			
			while(nMaxNum > 0)
			{
				nNum = CutOffFirstNumber(nKillCount,nMaxNum);
				m_pImgKillCount->Move(nPosX,nPosY);
				m_pImgKillCount->SetRect(nNum*RESTCOUNT_STRING_WIDTH,0,(nNum+1)*RESTCOUNT_STRING_WIDTH,RESTCOUNT_STRING_HEIGHT);	
				m_pImgKillCount->SetScale(m_fKillCountScale,m_fKillCountScale);
				m_pImgKillCount->Render();		
				nPosX += RESTCOUNT_STRING_WIDTH*m_fKillCountScale; 
				nMaxNum--;
			}
		}

		// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		m_nKillCountPosX  = g_pD3dApp->GetBackBufferDesc().Width - RESTKILLCOUNT_BUTTON_BASE_POS_X;
		m_nKillCountPosY  = nMapPosY - RESTCOUNT_BACK_POS_Y;
		
		if(m_bCheckKillCountON)
		{
			m_pImgKillCountOffBtn->SetBtnPosition(m_nKillCountPosX,m_nKillCountPosY);
			m_pImgKillCountOffBtn->Render();
		}
		else
		{
			m_pImgKillCountOnBtn->SetBtnPosition(m_nKillCountPosX,m_nKillCountPosY);
			m_pImgKillCountOnBtn->Render();
		}
		// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
	}	
#endif //S_BONUSEXPSYSTEM_RENEWAL
}
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CINFGameMainUnitInfoBar::RenderGageString(BOOL bShowAll)
{
	if(m_szGageInfo[GAMEMAIN_GAGE_HP_UP])
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{					

// 2011. 10. 10 by jskim UI시스템 변경
// 		m_pFontGageInfo[GAMEMAIN_NEW_STRING_HP]->DrawText( GAMEMAIN_NEW_STRING_HP_START_X, 
// 														GAMEMAIN_NEW_STRING_HP_START_Y,
// 														GUI_FONT_COLOR,
// 														m_szGageInfo[GAMEMAIN_NEW_STRING_HP],0L);
		SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_HP_UP]->GetStringSize(m_szGageInfo[GAMEMAIN_GAGE_HP_UP]);
		m_pFontGageInfo[GAMEMAIN_GAGE_HP_UP]->DrawText( (INT)(GAMEMAIN_GAGE_HP_UP_START_X - ( size.cx / 2 ) ), 
														GAMEMAIN_GAGE_HP_UP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_HP_UP],0L);
		size = m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_HP]->GetStringSize(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_HP]);
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_HP]->DrawText(  (INT)(GAMEMAIN_GAGE_HP_UP_START_X - ( size.cx / 2 ) ), 
														GAMEMAIN_GAGE_HP_UP_START_Y + 10,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_HP],0L);
	}

	if(m_szGageInfo[GAMEMAIN_GAGE_DP_UP])
	{
// 		m_pFontGageInfo[GAMEMAIN_NEW_STRING_DP]->DrawText( GAMEMAIN_NEW_STRING_DP_START_X, 
// 														GAMEMAIN_NEW_STRING_DP_START_Y,
// 														GUI_FONT_COLOR,
// 														m_szGageInfo[GAMEMAIN_NEW_STRING_DP],0L);
		SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_DP_UP]->GetStringSize(m_szGageInfo[GAMEMAIN_GAGE_DP_UP]);
		m_pFontGageInfo[GAMEMAIN_GAGE_DP_UP]->DrawText( (INT)(GAMEMAIN_GAGE_DP_UP_START_X - ( size.cx / 2 ) ), 
														GAMEMAIN_GAGE_DP_UP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_DP_UP],0L);
		size = m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_DP]->GetStringSize(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_DP]);
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_DP]->DrawText( (INT)(GAMEMAIN_GAGE_DP_UP_START_X - ( size.cx / 2 ) ), 
														GAMEMAIN_GAGE_DP_UP_START_Y + 10,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_DP],0L);
	}

	if(m_szGageInfo[GAMEMAIN_GAGE_SP_UP])
	{
// 		m_pFontGageInfo[GAMEMAIN_GAGE_SP_UP]->DrawText( GAMEMAIN_GAGE_SP_UP_START_X, 
// 														GAMEMAIN_GAGE_SP_UP_START_Y,
// 														GUI_FONT_COLOR,
// 														m_szGageInfo[GAMEMAIN_GAGE_SP_UP],0L);	
		SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_SP_UP]->GetStringSize(m_szGageInfo[GAMEMAIN_GAGE_SP_UP]);
		m_pFontGageInfo[GAMEMAIN_GAGE_SP_UP]->DrawText( (INT)( GAMEMAIN_NEW_STRING_MAX_SP_START_X - ( size.cx /2 ) ), 
														GAMEMAIN_NEW_STRING_MAX_SP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_SP_UP],0L);
		size = m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_SP]->GetStringSize(m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_SP]);
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_SP]->DrawText( (INT)( GAMEMAIN_NEW_STRING_MAX_SP_START_X - ( size.cx /2 ) ), 
														GAMEMAIN_NEW_STRING_MAX_SP_START_Y + 10,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_SP],0L);
	}

	if(m_szGageInfo[GAMEMAIN_GAGE_EP_UP])
	{
		SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_EP_UP]->GetStringSize(m_szGageInfo[GAMEMAIN_GAGE_EP_UP]);
		m_pFontGageInfo[GAMEMAIN_GAGE_EP_UP]->DrawText( (INT)( GAMEMAIN_STRING_FUEL_START_X - ( size.cx / 2 ) ), 
														GAMEMAIN_STRING_FUEL_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_EP_UP],0L);
	}	

	if(	bShowAll &&
		m_szGageInfo[GAMEMAIN_GAGE_EXP_UP])
	{
		SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->GetStringSize(m_szGageInfo[GAMEMAIN_GAGE_EXP_UP]);
		m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->DrawText( (INT)( GAMEMAIN_STRING_EXP_START_X - ( size.cx / 2 ) ), 
														GAMEMAIN_STRING_EXP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_EXP_UP],0L);
	}

// end 2011. 10. 10 by jskim UI시스템 변경 
#else
	{
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_HP]->DrawText( GAMEMAIN_NEW_STRING_HP_START_X, 
														GAMEMAIN_NEW_STRING_HP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_HP],0L);		
		m_pFontGageInfo[GAMEMAIN_GAGE_HP_UP]->DrawText( GAMEMAIN_GAGE_HP_UP_START_X, 
														GAMEMAIN_GAGE_HP_UP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_HP_UP],0L);
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_HP]->DrawText( GAMEMAIN_NEW_STRING_MAX_HP_START_X, 
														GAMEMAIN_NEW_STRING_MAX_HP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_HP],0L);
	}
	if(m_szGageInfo[GAMEMAIN_GAGE_DP_UP])
	{
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_DP]->DrawText( GAMEMAIN_NEW_STRING_DP_START_X, 
														GAMEMAIN_NEW_STRING_DP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_DP],0L);		
		m_pFontGageInfo[GAMEMAIN_GAGE_DP_UP]->DrawText( GAMEMAIN_GAGE_DP_UP_START_X, 
														GAMEMAIN_GAGE_DP_UP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_DP_UP],0L);
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_DP]->DrawText( GAMEMAIN_NEW_STRING_MAX_DP_START_X, 
														GAMEMAIN_NEW_STRING_MAX_DP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_DP],0L);
	}

	if(m_szGageInfo[GAMEMAIN_GAGE_SP_UP])
	{
		m_pFontGageInfo[GAMEMAIN_GAGE_SP_UP]->DrawText( GAMEMAIN_GAGE_SP_UP_START_X, 
														GAMEMAIN_GAGE_SP_UP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_SP_UP],0L);		
		m_pFontGageInfo[GAMEMAIN_NEW_STRING_MAX_SP]->DrawText( GAMEMAIN_NEW_STRING_MAX_SP_START_X, 
														GAMEMAIN_NEW_STRING_MAX_SP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_NEW_STRING_MAX_SP],0L);
	}

	if(m_szGageInfo[GAMEMAIN_GAGE_EP_UP])
	{
		m_pFontGageInfo[GAMEMAIN_GAGE_EP_UP]->DrawText( GAMEMAIN_STRING_FUEL_START_X, 
														GAMEMAIN_STRING_FUEL_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_EP_UP],0L);
	}	

	if(	bShowAll &&
		m_szGageInfo[GAMEMAIN_GAGE_EXP_UP])
	{
		m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->DrawText( GAMEMAIN_STRING_EXP_START_X, 
														GAMEMAIN_STRING_EXP_START_Y,
														GUI_FONT_COLOR,
														m_szGageInfo[GAMEMAIN_GAGE_EXP_UP],0L);
	}
#endif
}


void CINFGameMainUnitInfoBar::Tick()
{
	const float RENDER_TIME = 0.5f;
	SetSpeed(g_pShuttleChild->m_fShuttleSpeed, 
			g_pShuttleChild->m_fRealShuttleSpeedMin, 
			g_pShuttleChild->m_fRealShuttleSpeedBoosterOn );
	m_fWarningTime += g_pD3dApp->GetElapsedTime();
	if(m_fWarningTime > RENDER_TIME)
	{
		m_bWarning = !m_bWarning;
		m_fWarningTime -= RENDER_TIME;
	}	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pWeaponInfo->Tick();		// 2011. 10. 10 by jskim UI시스템 변경				  
#endif
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	if(m_bChangeKillCount)
	{
		m_fKillCountScale -= KILLCOUNT_SCALE_DECREASE;
		if(m_fKillCountScale <= KILLCOUNT_SCALE_DEFAULT)
		{
			m_fKillCountScale = KILLCOUNT_SCALE_DEFAULT;
			m_bChangeKillCount = false;
		}
	}	
	if(m_bCompleteKillCount)
	{
		if(m_nCompleteKillCountBlank < KILLCOUNT2_EFF_MAX_BLENK_COUNT)
		{
			if((m_nCompleteKillCountBlank%2) == 0) // 짝수일때 알파값 상승
			{
				if(m_nCompleteKillCountAlpha > KILLCOUNT2_EFF_MAX_ALPHA - KILLCOUNT2_EFF_ALPHA_CREASE)
				{
					m_nCompleteKillCountAlpha = KILLCOUNT2_EFF_MAX_ALPHA;
					m_nCompleteKillCountBlank++;
				}
				else
				{
					m_nCompleteKillCountAlpha += KILLCOUNT2_EFF_ALPHA_CREASE;
				}
			}
			else								// 홀수일때 알파값 하락
			{
				if(((int)m_nCompleteKillCountAlpha) < 0 + KILLCOUNT2_EFF_ALPHA_CREASE)
				{
					m_nCompleteKillCountAlpha = 0;
					m_nCompleteKillCountBlank++;
				}
				else
				{
					m_nCompleteKillCountAlpha -= KILLCOUNT2_EFF_ALPHA_CREASE;
				}
			}
		}
		else	// 300 킬 카운트 표현 로직 종료.
		{
			m_fKillCountScale = KILLCOUNT_SCALE_DEFAULT;
			m_bCompleteKillCount = false;
			if(m_nKillCount == KILLCOUNT_MAX_COUNT)
			{
				m_nKillCount = 0;
			}
		}
	}

#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	if(m_bCheckRestCountBtnClick)
	{
		RestCountImageOnOff();
	}

	if(m_bCheckKillCountBtnClick)
	{
		KillCountImageOnOff();
	}
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
}

int CINFGameMainUnitInfoBar::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(WndProcCity(uMsg, wParam, lParam) == INF_MSGPROC_BREAK )
		return INF_MSGPROC_BREAK;
	
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(pt.x > m_nGiftX &&
				pt.x < m_nGiftX + m_ptGift.x &&
				pt.y > m_nGiftY &&
				pt.y < m_nGiftY + m_ptGift.y)
			{
				CreateMsgBox();
				return INF_MSGPROC_BREAK;
			}

			//if(g_pGameMain->GetVoiceType() != VOICE_NONE &&
			//	pt.x > GAMEMAIN_VOICECHAT_X &&
			//	pt.x < GAMEMAIN_VOICECHAT_X + GAMEMAIN_VOICECHAT_W &&
			//	pt.y > GAMEMAIN_VOICECHAT_Y &&
			//	pt.y < GAMEMAIN_VOICECHAT_Y + GAMEMAIN_VOICECHAT_H)
			//{
			//	g_pGameMain->m_pChat->ShowVoiceChatControl(TRUE);
			//	return INF_MSGPROC_BREAK;
			//}

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
			if(m_nRestCount != 0 && (g_pShuttleChild->m_bUnitStop || g_pD3dApp->m_bCharacter))		// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
			{
				if(m_bCheckRestCountON)
				{
					if(TRUE == m_pImgRestCountOffBtn->OnLButtonDown(pt))
					{						
						return  INF_MSGPROC_BREAK;
					}
				}
				else
				{
					if(TRUE == m_pImgRestCountOnBtn->OnLButtonDown(pt))
					{						
						return  INF_MSGPROC_BREAK;
					}
				}
			}
			if(m_nKillCount != 0 && (g_pShuttleChild->m_bUnitStop || g_pD3dApp->m_bCharacter))		// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
			{
				if(m_bCheckKillCountON)
				{
					if(TRUE == m_pImgKillCountOffBtn->OnLButtonDown(pt))
					{						
						return  INF_MSGPROC_BREAK;
					}
				}
				else
				{
					if(TRUE == m_pImgKillCountOnBtn->OnLButtonDown(pt))
					{						
						return  INF_MSGPROC_BREAK;
					}
				}
			}		
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			int x,y;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// panoskj todo: recheck this part
			//if(pt.x > GAMEMAIN_VOICECHAT_X &&
			//	pt.x < GAMEMAIN_VOICECHAT_X + GAMEMAIN_VOICECHAT_W &&
			//	pt.y > GAMEMAIN_VOICECHAT_Y &&
			//	pt.y < GAMEMAIN_VOICECHAT_Y + GAMEMAIN_VOICECHAT_H)
			//{// 음성아이콘 툴바 표시.
			//	char szTemp[128];
			//	memset(szTemp,0x00,128);
			//	/*switch(g_pGameMain->GetVoiceType())
			//	{
			//	case VOICE_ONE:
			//		{
			//			sprintf(szTemp,STRMSG_C_080519_0113);
			//		}
			//		break;
			//	case VOICE_PARTY:
			//		{
			//			sprintf(szTemp,STRMSG_C_080519_0110);
			//		}
			//		break;
			//	case VOICE_GUILD:
			//		{
			//			sprintf(szTemp,STRMSG_C_080519_0111);
			//		}
			//		break;
			//	default:
			//		{*/
			//			g_pGameMain->SetToolTip(0,0,NULL);
			//		//}
			//		//break;
			//	//}
			//	g_pGameMain->SetToolTip(pt.x,pt.y,szTemp);
			//}
			//else
			//{
				g_pGameMain->SetToolTip(0,0,NULL);
			//}


			if(IsHappyHourEvent())
			{
				m_bRenderEvent = FALSE;
				
				int x,y;

				x = g_pD3dApp->GetBackBufferDesc().Width - GAME_EVENT_BASICPOS_X;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				y = g_pD3dApp->GetBackBufferDesc().Height - 74;
				// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
				y -= RESTKILLCOUNT_MOVE_HAPPYHOUR;
#endif // S_BONUSEXPSYSTEM_RENEWAL
				// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
//				if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//					y -= GAME_EVENT_BASICPOS_Y;

				if( pt.x > x &&
					pt.x < x + 130 && pt.y > y && pt.y < y + 22)
				{
					m_bRenderEvent = TRUE;
				}
#else
				y = g_pD3dApp->GetBackBufferDesc().Height - 54;
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
				y -= RESTKILLCOUNT_MOVE_HAPPYHOUR;
#endif // S_BONUSEXPSYSTEM_RENEWAL

//				if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//					y -= GAME_EVENT_BASICPOS_Y;

				if( pt.x > x &&
					pt.x < x + 120 && pt.y > y && pt.y < y + 12)
				{
					m_bRenderEvent = TRUE;
				}
#endif
			}
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
			//int x,y;
			RECT BonusExpRateRect;
			BonusExpRateRect.left = g_pD3dApp->GetBackBufferDesc().Width - BONUS_EXP_RATE_POPUP_RECT_LEFT;
			BonusExpRateRect.top = g_pD3dApp->GetBackBufferDesc().Height - BONUS_EXP_RATE_POPUP_RECT_TOP;
			BonusExpRateRect.right = BonusExpRateRect.left + BONUS_EXP_RATE_POPUP_RECT_RIGHT;
			BonusExpRateRect.bottom = BonusExpRateRect.top + BONUS_EXP_RATE_POPUP_RECT_BOTTOM;

			if( BonusExpRateRect.left < pt.x  && pt.x < BonusExpRateRect.right && 
				BonusExpRateRect.top < pt.y && pt.y < BonusExpRateRect.bottom)
			{
				m_bRenderBonusExpRateText = TRUE;
				// 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
				if(g_pShuttleChild && g_pShuttleChild->m_pClientParty)
				{
					m_nPartyRate = (CAtumSJ::GetPartyBonusExpWeight(g_pShuttleChild->m_pClientParty->GetInSameMapPartyMemberCount()+1)-1.0f) * 100.0f;
				}
				// end 2012-10-17 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
			}
			else
			{
				m_bRenderBonusExpRateText = FALSE;
			}
#endif // S_BONUSEXPSYSTEM_RENEWAL
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

			if (g_bEventRecovery)
			{
				m_bRenderEventRecovery = FALSE;

				int nAddY = 100;

#if !(S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL)
				nAddY -= RESTKILLCOUNT_MOVE_HAPPYHOUR;
#endif
				if(!g_pD3dApp->m_bCharacter)
					nAddY+=30;

				x = g_pD3dApp->GetBackBufferDesc().Width - 200;
				y = g_pD3dApp->GetBackBufferDesc().Height - nAddY;

				if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
					y -= GAME_EVENT_BASICPOS_Y;

				if( pt.x > x &&
					pt.x < x + 44 && pt.y > y && pt.y < y + 20)
				{
					m_bRenderEventRecovery = TRUE;
				}
			}

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
			y = RESTKILLCOUNT_BASE_POS_Y - RESTCOUNT_BACK_POS_Y;
			if(m_nRestCount != 0 && (g_pShuttleChild->m_bUnitStop || g_pD3dApp->m_bCharacter))		// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
			{
				if(m_bCheckRestCountON)
				{
// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
#ifdef C_DECA_TOOLTIP
					m_bRestCountToolTip = FALSE;
					x = g_pD3dApp->GetBackBufferDesc().Width - (RESTCOUNT_IMAGE_POS_LIMIT + RESTKILLCOUNT_BUTTON_WIDTH);
					if( pt.x > x &&
						pt.x < x + RESTCOUNT_IMAGE_POS_LIMIT - RESTKILLCOUNT_BUTTON_WIDTH 
						&& pt.y > y && pt.y < y + RESTKILLCOUNT_BUTTON_HEIGHT)
					{
						m_bRestCountToolTip = TRUE;
					}
#endif
// end 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
					m_pImgRestCountOffBtn->OnMouseMove(pt);
				}
				else
				{
					m_pImgRestCountOnBtn->OnMouseMove(pt);
				}
				y += RESTCOUNT_STRING_HEIGHT + RESTKILLCOUNT_BASE_HEIGHT_SIZE;
			}
// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
			if(m_nKillCount != 0 && (g_pShuttleChild->m_bUnitStop || g_pD3dApp->m_bCharacter))
			{
				if(m_bCheckKillCountON)
				{
#ifdef C_DECA_TOOLTIP
						m_bKillCountToolTip = FALSE;
						x = g_pD3dApp->GetBackBufferDesc().Width - (KILLCOUNT_IMAGE_POS_LIMIT + RESTKILLCOUNT_BUTTON_WIDTH);
						if( pt.x > x &&	pt.x < x + KILLCOUNT_IMAGE_POS_LIMIT - RESTKILLCOUNT_BUTTON_WIDTH 
							&& pt.y > y && pt.y < y + RESTKILLCOUNT_BUTTON_HEIGHT)
						{
							m_bKillCountToolTip = TRUE;
						}
#endif
// end 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
					m_pImgKillCountOffBtn->OnMouseMove(pt);
				}
				else
				{
					m_pImgKillCountOnBtn->OnMouseMove(pt);
				}				
			}			
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#ifdef _SHOW_LATENCY
			//2015-08-15 by St0rmy, better Ping Display
			if (pt.x > PING_DISPLAY_POS_X &&
				pt.x < PING_DISPLAY_POS_X + PING_DISPLAY_SIZE_X &&
				pt.y > PING_DISPLAY_POS_Y &&
				pt.y < PING_DISPLAY_POS_Y + PING_DISPLAY_SIZE_Y)
			{
				char szTemp[128];
				//g_pD3dApp->m_fLatency = 0.0f;
				if (g_pD3dApp->m_fLatency * 1000.0f <= 100)
				{
					sprintf(szTemp, STRMSG_C_PING_TOOLTIP, (g_pD3dApp->m_fLatency * 1000.0f) - (g_pD3dApp->m_fLatency * 1000.0f)/3);
				}
				else if (g_pD3dApp->m_fLatency * 1000.0f <= 250)
				{
					sprintf(szTemp, STRMSG_C_PING_TOOLTIP, (g_pD3dApp->m_fLatency * 1000.0f) - (g_pD3dApp->m_fLatency * 1000.0f) / 3 + (g_pD3dApp->m_fLatency * 1000.0f) / 12);
				}
				else if (g_pD3dApp->m_fLatency * 1000.0f <= 350)
				{
					sprintf(szTemp, STRMSG_C_PING_TOOLTIP, (g_pD3dApp->m_fLatency * 1000.0f) - (g_pD3dApp->m_fLatency * 1000.0f) / 4 + (g_pD3dApp->m_fLatency * 1000.0f) / 46);
				}
				else if (g_pD3dApp->m_fLatency * 1000.0f <= 450)
				{
					sprintf(szTemp, STRMSG_C_PING_TOOLTIP, (g_pD3dApp->m_fLatency * 1000.0f) - (g_pD3dApp->m_fLatency * 1000.0f) / 3 + (g_pD3dApp->m_fLatency * 1000.0f) / 9);
				}
				else if (g_pD3dApp->m_fLatency * 1000.0f <= 550)
				{
					sprintf(szTemp, STRMSG_C_PING_TOOLTIP, (g_pD3dApp->m_fLatency * 1000.0f) - (g_pD3dApp->m_fLatency * 1000.0f) / 3 + (g_pD3dApp->m_fLatency * 1000.0f) / 9);
				}
				else if (g_pD3dApp->m_fLatency * 1000.0f <= 650)
				{
					sprintf(szTemp, STRMSG_C_PING_TOOLTIP, g_pD3dApp->m_fLatency * 1000.0f);
				}
				g_pGameMain->SetToolTip(pt.x - 85, pt.y + 5, szTemp);
				return INF_MSGPROC_BREAK;
			}
#endif
		}
		break;
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
				if(m_nRestCount != 0 && (g_pShuttleChild->m_bUnitStop || g_pD3dApp->m_bCharacter))		// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
				{
					if(m_bCheckRestCountON)
					{
						if(TRUE == m_pImgRestCountOffBtn->OnLButtonUp(pt))
						{
							m_bCheckRestCountON = !m_bCheckRestCountON;
							m_bCheckRestCountBtnClick = true;
						}
					}
					else
					{
						if(TRUE == m_pImgRestCountOnBtn->OnLButtonUp(pt))
						{
							m_bCheckRestCountON = !m_bCheckRestCountON;
							m_bCheckRestCountBtnClick = true;
						}
					}
				}
				if(m_nKillCount != 0 && (g_pShuttleChild->m_bUnitStop || g_pD3dApp->m_bCharacter))			// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
				{
					if(m_bCheckKillCountON)
					{
						if(TRUE == m_pImgKillCountOffBtn->OnLButtonUp(pt))
						{
							m_bCheckKillCountON = !m_bCheckKillCountON;
							m_bCheckKillCountBtnClick = true;
						}
					}
					else
					{
						if(TRUE == m_pImgKillCountOnBtn->OnLButtonUp(pt))
						{
							m_bCheckKillCountON = !m_bCheckKillCountON;
							m_bCheckKillCountBtnClick = true;
						}
					}
				}				
#endif
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
		}
		break;
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFGameMainUnitInfoBar::WndProcCity(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_v2MousePt.x = pt.x;
			m_v2MousePt.y = pt.y;
			m_bRenderEvent = FALSE;
			
			if (IsHappyHourEvent())
			{
				auto x = CHAT_MENU_CASH_START_X - 177;

				auto y = g_pD3dApp->GetBackBufferDesc().Height - 99;

				if (pt.x > x && pt.x < x + 120 &&

					pt.y > y && pt.y < y + 12)
					
					m_bRenderEvent = TRUE;
			}


			// 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			int	x	= g_pD3dApp->GetBackBufferDesc().Width - 104;
			int	y	= g_pD3dApp->GetBackBufferDesc().Height - 52 - GAME_EVENT_BASICPOS_Y;
#else
			int	x	= g_pD3dApp->GetBackBufferDesc().Width - 64;
			int	y	= g_pD3dApp->GetBackBufferDesc().Height - 82

			if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
				y -= GAME_EVENT_BASICPOS_Y;
#endif
			if(m_bMemberPCBang)
			{
				POINT	pt = m_pImgGeneralPremiumCard->GetImgSize();
				y -= pt.y;
			}

			RECT membership;
			membership.left		= x;
			membership.top		= y;
			membership.right	= x + m_pImgGeneralPremiumCard->GetImgSize().x;
			membership.bottom	= y + m_pImgGeneralPremiumCard->GetImgSize().y;

 			if(PtInRect(&membership, pt))
				m_bPremiumToolTip = TRUE;
			else
				m_bPremiumToolTip = FALSE;

			// end 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가
		}
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
#ifdef _WARRIOR_ADDITIONAL_INFO
	#define SWTARGET_MOUSE_SCALE		((float)g_pD3dApp->GetBackBufferDesc().Width/1524.0f)
	#define SWFIRE_ANGLE_START_X			(g_pD3dApp->GetBackBufferDesc().Width/2 - 45*SWTARGET_MOUSE_SCALE)
	#define SWFIRE_ANGLE_START_Y			(g_pD3dApp->GetBackBufferDesc().Height/2 - 45*SWTARGET_MOUSE_SCALE)
#define SWFIRE_NEW_POS_X					(g_pD3dApp->GetBackBufferDesc().Width/2 - 45);
#define SWFIRE_NEW_POS_Y					(g_pD3dApp->GetBackBufferDesc().Height/2 - 45;
#endif

void CINFGameMainUnitInfoBar::RenderMainGage(BOOL bShowAll)
{
	UINT i;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT Pos = { 0, 0 };
	if(m_fGageRate[GAMEMAIN_GAGE_HP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_HP_UP]*GAMEMAIN_GAGE_HP_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_HP_MAX_GRADE){i--;}
#ifdef _WARRIOR_ADDITIONAL_INFO
			m_pImgMainGageHP[i]->SetScale(1, 1);
			m_pImgMainGageHP[i]->SetColor(D3DCOLOR_ARGB(200, 255, 0, 0));
#endif
		i = min(i, GAMEMAIN_GAGE_HP_MAX_GRADE-1); // 2013-06-17 by bhsohn 체력게이지 메모리 버그 수정

		if(i > 0)
		{
			Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("ene00");		// 2011. 10. 10 by jskim UI시스템 변경
			m_pImgMainGageHP[i]->Move( GAMEMAIN_POS_X + Pos.x , GAMEMAIN_POS_Y + Pos.y );
			m_pImgMainGageHP[i]->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
			if (g_pSOption->sWSWAddAIMInfo)
			{
				if ((g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex > 2500) && (g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _CITY || g_pD3dApp->m_dwGameState == _SHOP))
				{
					m_pImgMainGageHP[i]->Move(SWFIRE_ANGLE_START_X + (10 * SWTARGET_MOUSE_SCALE*0.6), SWFIRE_ANGLE_START_Y + (11 * SWTARGET_MOUSE_SCALE*0.6));
					m_pImgMainGageHP[i]->SetScale(SWTARGET_MOUSE_SCALE*0.6, SWTARGET_MOUSE_SCALE*0.6);
					m_pImgMainGageHP[i]->SetColor(D3DCOLOR_ARGB(80, 255, 0, 0));
					m_pImgMainGageHP[i]->Render();
				}
			}
#endif
		}
	}
	if(m_fGageRate[GAMEMAIN_GAGE_DP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_DP_UP]*GAMEMAIN_GAGE_DP_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_DP_MAX_GRADE){i--;}

		i = min(i, GAMEMAIN_GAGE_DP_MAX_GRADE-1); // 2013-06-17 by bhsohn 체력게이지 메모리 버그 수정

		if(i > 0)
		{
			Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("she00");		// 2011. 10. 10 by jskim UI시스템 변경
			m_pImgMainGageDP[i]->Move( GAMEMAIN_POS_X + Pos.x , GAMEMAIN_POS_Y + Pos.y );
#ifdef _WARRIOR_ADDITIONAL_INFO
				m_pImgMainGageDP[i]->SetScale(1, 1);
				m_pImgMainGageDP[i]->SetColor(D3DCOLOR_ARGB(200, 0, 150, 255));
#endif
			m_pImgMainGageDP[i]->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
			if (g_pSOption->sWSWAddAIMInfo)
			{
				if ((g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex > 2500) && (g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _CITY || g_pD3dApp->m_dwGameState == _SHOP))
				{
					m_pImgMainGageDP[i]->Move(SWFIRE_ANGLE_START_X + (75 * SWTARGET_MOUSE_SCALE*0.6), SWFIRE_ANGLE_START_Y + (11 * SWTARGET_MOUSE_SCALE*0.6));
					m_pImgMainGageDP[i]->SetScale(SWTARGET_MOUSE_SCALE*0.6, SWTARGET_MOUSE_SCALE*0.6);
					m_pImgMainGageDP[i]->SetColor(D3DCOLOR_ARGB(80, 0, 150, 255));
					m_pImgMainGageDP[i]->Render();
				}
			}
#endif
		}
	}
	if(m_fGageRate[GAMEMAIN_GAGE_SP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_SP_UP]*GAMEMAIN_GAGE_SP_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_SP_MAX_GRADE){i--;}

		i = min(i, GAMEMAIN_GAGE_SP_MAX_GRADE-1); // 2013-06-17 by bhsohn 체력게이지 메모리 버그 수정

		if(i > 0)
		{
			Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("skil00");		// 2011. 10. 10 by jskim UI시스템 변경
			m_pImgMainGageSP[i]->Move( GAMEMAIN_POS_X + Pos.x, GAMEMAIN_POS_Y + Pos.y );
			m_pImgMainGageSP[i]->Render();
		}
	}
	if(m_fGageRate[GAMEMAIN_GAGE_BURN_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_BURN_UP]*GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_BOOSTER_MAX_GRADE){i--;}

		i = min(i, GAMEMAIN_GAGE_BOOSTER_MAX_GRADE-1); // 2013-06-17 by bhsohn 체력게이지 메모리 버그 수정

		Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("Bost00");		// 2011. 10. 10 by jskim UI시스템 변경
		m_pImgMainGageBooster[i]->Move(GAMEMAIN_POS_X + Pos.x , GAMEMAIN_POS_Y + Pos.y );
#ifdef _WARRIOR_ADDITIONAL_INFO
		m_pImgMainGageBooster[i]->SetScale(1, 1);
		m_pImgMainGageBooster[i]->SetColor(D3DCOLOR_ARGB(200, 0, 150, 255));
#endif
		m_pImgMainGageBooster[i]->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
		if (g_pSOption->sWSWAddAIMInfo)
		{
			if ((g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex > 2500) && (g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _CITY || g_pD3dApp->m_dwGameState == _SHOP))
			{
				m_pImgMainGageBooster[i]->Move(SWFIRE_ANGLE_START_X, SWFIRE_ANGLE_START_Y);
				m_pImgMainGageBooster[i]->SetScale(SWTARGET_MOUSE_SCALE*0.6, SWTARGET_MOUSE_SCALE*0.6);
				m_pImgMainGageBooster[i]->SetColor(D3DCOLOR_ARGB(255, 0, 255, 255));
				m_pImgMainGageBooster[i]->Render();
			}
		}
#endif
	}
	if(m_fGageRate[GAMEMAIN_GAGE_EP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_EP_UP]*GAMEMAIN_GAGE_FUEL_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_FUEL_MAX_GRADE){i--;}

		i = min(i, GAMEMAIN_GAGE_FUEL_MAX_GRADE-1); // 2013-06-17 by bhsohn 체력게이지 메모리 버그 수정

		Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("fue00");		// 2011. 10. 10 by jskim UI시스템 변경
		m_pImgMainGageFuel[i]->Move(GAMEMAIN_POS_X + Pos.x , GAMEMAIN_POS_Y + Pos.y );
#ifdef _WARRIOR_ADDITIONAL_INFO
		m_pImgMainGageFuel[i]->SetScale(1, 1);
		//m_pImgMainGageFuel[i]->SetColor(D3DCOLOR_ARGB(200, 0, 150, 255));
#endif
		m_pImgMainGageFuel[i]->Render();
#ifdef _WARRIOR_ADDITIONAL_INFO
		if (g_pSOption->sWSWAddAIMInfo)
		{
			if ((g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex > 2500) && (g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _CITY || g_pD3dApp->m_dwGameState == _SHOP))
			{
				m_pImgMainGageFuel[i]->Move(SWFIRE_ANGLE_START_X + (75 * SWTARGET_MOUSE_SCALE*0.6), SWFIRE_ANGLE_START_Y + (SWTARGET_MOUSE_SCALE*0.6));
				m_pImgMainGageFuel[i]->SetScale(SWTARGET_MOUSE_SCALE*0.6, SWTARGET_MOUSE_SCALE*0.6);
				//m_pImgMainGageFuel[i]->SetColor(D3DCOLOR_ARGB(80, 0, 150, 255));
				m_pImgMainGageFuel[i]->Render();
			}
		}
#endif
	}
	if(m_fGageRate[GAMEMAIN_GAGE_EP_UP]>0.2f || !m_bWarning)
	{

		Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("fuelG00");		// 2011. 10. 10 by jskim UI시스템 변경
		m_pImgMainIconFuel[0]->Move(GAMEMAIN_POS_X + Pos.x, GAMEMAIN_POS_Y + Pos.y);
		m_pImgMainIconFuel[0]->Render();
	}
	else
	{
		Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("fuelG00"); 		// 2011. 10. 10 by jskim UI시스템 변경
		m_pImgMainIconFuel[1]->Move( GAMEMAIN_POS_X + Pos.x, GAMEMAIN_POS_Y + Pos.y );
		m_pImgMainIconFuel[1]->Render();
	}

	// 2008-05-29 by dgwoo 메인 인터페이스 변경 기획.
// 2011. 10. 10 by jskim UI시스템 변경
	Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("firstweapon"); 		
	g_pGameMain->m_pWeaponInfo->RenderLeftWeapon( Pos.x, Pos.y );  // 1형 무기
	Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("secondweapon"); 
	g_pGameMain->m_pWeaponInfo->RenderRightWeapon( Pos.x, Pos.y ); // 2형 무기
	
	//--------------------------------------------------------------------------//
#ifdef _WARRIOR_CLASS_KILL_BAR
	if (g_pShuttleChild->m_myShuttleInfo.Level < CHARACTER_MAX_LEVEL)
	{
		if (bShowAll)
		{
			int nCurrentClass = GetCurrentKillClass();
			float m_fClassBarRate = GetCurrentKillBarRate(nCurrentClass);
			if (nCurrentClass != 25)
			{
				m_pImgMainExpBar->Move(WSW_BACK_KILL_BAR_X, WSW_BACK_KILL_BAR_Y);
				m_pImgMainExpBar->SetRect(0, 0, (float)((WSW_BACK_KILL_BAR_X - ((g_pD3dApp->GetBackBufferDesc().Width / 2.0f) - 474)) * m_fClassBarRate), g_pD3dApp->GetBackBufferDesc().Height);
				m_pImgMainExpBar->Render();
				char szTempBarStr[1024];
				sprintf(szTempBarStr, "Lv. [\\g%d\\g] Class: \\a%s\\a, %5.2f%%", nCurrentClass, GetClassString(nCurrentClass), (m_fClassBarRate*100.0f));
				SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->GetStringSize(szTempBarStr);
				m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->DrawText((INT)(GAMEMAIN_STRING_EXP_START_X - (size.cx / 2)),
					WSW_BACK_KILL_BAR_Y,
					GUI_FONT_COLOR,
					szTempBarStr, 0L);
			}
			else
			{
				m_pImgMainExpBar->Move(WSW_BACK_KILL_BAR_X, WSW_BACK_KILL_BAR_Y);
				m_pImgMainExpBar->SetRect(0, 0, (float)((WSW_BACK_KILL_BAR_X - ((g_pD3dApp->GetBackBufferDesc().Width / 2.0f) - 474)) * 0.0f), g_pD3dApp->GetBackBufferDesc().Height);
				m_pImgMainExpBar->Render();
				char szTempBarStr[1024];
				sprintf(szTempBarStr, "Lv. [\\g%d\\g] Class: \\e%s\\e", nCurrentClass, GetClassString(nCurrentClass));
				SIZE size = m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->GetStringSize(szTempBarStr);
				m_pFontGageInfo[GAMEMAIN_GAGE_EXP_UP]->DrawText((INT)(GAMEMAIN_STRING_EXP_START_X - (size.cx / 2)),
					WSW_BACK_KILL_BAR_Y,
					GUI_FONT_COLOR,
					szTempBarStr, 0L);
			}

		}
	}
#endif
	if(	bShowAll &&
		m_fGageRate[GAMEMAIN_GAGE_EXP_UP]>0)
	{
		m_pImgMainExpBar->Move( 0, GAMEMAIN_BACK_GAGE_EXP_START_Y );
		m_pImgMainExpBar->SetRect(0,0, (float)( g_pD3dApp->GetBackBufferDesc().Width * m_fGageRate[GAMEMAIN_GAGE_EXP_UP] ), g_pD3dApp->GetBackBufferDesc().Height);

		m_pImgMainExpBar->Render();
// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
		m_pImgMainExpBar1->Move( (int)( g_pD3dApp->GetBackBufferDesc().Width * m_fGageRate[GAMEMAIN_GAGE_EXP_UP] ), GAMEMAIN_BACK_GAGE_EXP_START_Y );
//end 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
		m_pImgMainExpBar1->Render();
	}
	if(m_fGageRate[GAMEMAIN_GAGE_HP_UP]<0.35f && m_bWarning)
	{ 
		Pos = m_pGageBarInfo->GetFindControlTargetofMinPos("ene00");
		m_pImgMainWarning->Move(GAMEMAIN_POS_X + 2 + Pos.x, GAMEMAIN_POS_Y + Pos.y);
		m_pImgMainWarning->Render();
	}	   
#else
	if(m_fGageRate[GAMEMAIN_GAGE_HP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_HP_UP]*GAMEMAIN_GAGE_HP_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_HP_MAX_GRADE){i--;}

		if(i > 0)
		{
			m_pImgMainGageHP[i]->Move(GAMEMAIN_GAGE_HP_START_X, GAMEMAIN_GAGE_HP_START_Y);
			m_pImgMainGageHP[i]->Render();
		}
	}
	if(m_fGageRate[GAMEMAIN_GAGE_DP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_DP_UP]*GAMEMAIN_GAGE_DP_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_DP_MAX_GRADE){i--;}

		if(i > 0)
		{
			m_pImgMainGageDP[i]->Move(GAMEMAIN_GAGE_DP_START_X, GAMEMAIN_GAGE_DP_START_Y);
			m_pImgMainGageDP[i]->Render();
		}
	}
	if(m_fGageRate[GAMEMAIN_GAGE_SP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_SP_UP]*GAMEMAIN_GAGE_SP_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_SP_MAX_GRADE){i--;}

		if(i > 0)
		{
			m_pImgMainGageSP[i]->Move(GAMEMAIN_GAGE_SP_START_X, GAMEMAIN_GAGE_SP_START_Y);
			m_pImgMainGageSP[i]->Render();
		}
	}
	if(m_fGageRate[GAMEMAIN_GAGE_BURN_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_BURN_UP]*GAMEMAIN_GAGE_BOOSTER_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_BOOSTER_MAX_GRADE){i--;}
		m_pImgMainGageBooster[i]->Move(GAMEMAIN_GAGE_BOOSTER_START_X, GAMEMAIN_GAGE_BOOSTER_START_Y);
		m_pImgMainGageBooster[i]->SetScale(1.125,1.125);
		m_pImgMainGageBooster[i]->Render();
	}
	if(m_fGageRate[GAMEMAIN_GAGE_EP_UP]>0)
	{
		i = m_fGageRate[GAMEMAIN_GAGE_EP_UP]*GAMEMAIN_GAGE_FUEL_MAX_GRADE;
		if(i>=GAMEMAIN_GAGE_FUEL_MAX_GRADE){i--;}
		m_pImgMainGageFuel[i]->Move(GAMEMAIN_GAGE_FUEL_START_X,GAMEMAIN_GAGE_FUEL_START_Y);
		m_pImgMainGageFuel[i]->Render();
	}
	if(m_fGageRate[GAMEMAIN_GAGE_EP_UP]>0.2f || !m_bWarning)
	{
		m_pImgMainIconFuel[0]->Move(GAMEMAIN_GAGE_FUEL_START_X,GAMEMAIN_GAGE_FUEL_START_Y);
		m_pImgMainIconFuel[0]->Render();
	}
	else
	{
		m_pImgMainIconFuel[1]->Move(GAMEMAIN_GAGE_FUEL_START_X,GAMEMAIN_GAGE_FUEL_START_Y);
		m_pImgMainIconFuel[1]->Render();
	}
	if(VOICE_NONE != g_pGameMain->GetVoiceType())
	{
		if(g_pGameMain->GetVOIPSpeaking())
		{
			m_pImgVoiceSPKIcon->Move(GAMEMAIN_VOICECHAT_X,GAMEMAIN_VOICECHAT_Y);
			m_pImgVoiceSPKIcon->Render();
		}
		else
		{
			m_pImgVoiceChatIcon->Move(GAMEMAIN_VOICECHAT_X,GAMEMAIN_VOICECHAT_Y);
			m_pImgVoiceChatIcon->Render();
		}		
	}


	// 2008-05-29 by dgwoo 메인 인터페이스 변경 기획.
	g_pGameMain->m_pWeaponInfo->RenderLeftWeapon();
	g_pGameMain->m_pWeaponInfo->RenderRightWeapon();

	//--------------------------------------------------------------------------//
	



	if(	bShowAll &&
		m_fGageRate[GAMEMAIN_GAGE_EXP_UP]>0)
	{
		m_pImgMainExpBar->Move(GAMEMAIN_EXP_GAGE_START_X, GAMEMAIN_EXP_GAGE_START_Y);
		m_pImgMainExpBar->SetRect(0,0,GAMEMAIN_GAGE_EXP_LENGTH*m_fGageRate[GAMEMAIN_GAGE_EXP_UP],9);
		m_pImgMainExpBar->Render();
	}
	if(m_fGageRate[GAMEMAIN_GAGE_HP_UP]<0.35f && m_bWarning)
	{
		m_pImgMainWarning->Move(GAMEMAIN_GAGE_HP_START_X+2, GAMEMAIN_GAGE_HP_START_Y);
		m_pImgMainWarning->Render();
	}
#endif
}
// end 2011. 10. 10 by jskim UI시스템 변경
void CINFGameMainUnitInfoBar::RenderSpeed()
{
	// 2005-05-31 by jschoi - 유닛 상하 중력 적용
//	int nSpeed = (int)g_pShuttleChild->m_fShuttleSpeed;
	// 2005-12-12 by ispark, 캐릭터 상태에서는 무조건... 0
	int nSpeed = 0;
	if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
		nSpeed = 0;
	else
	{
		// 2010. 05. 17 by jskim A기어 지상 속도 1 적은 버그 수정
		//nSpeed = (int)(g_pShuttleChild->m_fShuttleSpeed - g_pShuttleChild->m_fTempGravity);
		//nSpeed = (int)((g_pShuttleChild->m_fShuttleSpeed - g_pShuttleChild->m_fTempGravity) + 1);
		//end. 05. 17 by jskim A기어 지상 속도 1 적은 버그 수정
		nSpeed = (int)((g_pShuttleChild->m_fShuttleSpeed - g_pShuttleChild->m_fTempGravity));

		if ( g_pShuttleChild->m_fShuttleSpeed > .0f )
			nSpeed += 1;
	}


	int nMaxNum = GetMaxSize(nSpeed);
	if(nMaxNum > 3)
	{
		nMaxNum = 3;
		nSpeed = 999;
	}
	UINT nPosX,nPosY,nNum;
	nPosX = SPEED_STRING_START_X - nMaxNum*8;
	nPosY = SPEED_STRING_START_Y;

	if(nSpeed < 0)
	{
		nPosX -= 7;
		m_pImgMainMinus->Move(nPosX,nPosY);
		m_pImgMainMinus->Render();		
		nPosX += 10;
		nSpeed = abs(nSpeed);
	}

	while(nMaxNum > 0)
	{
		nNum = CutOffFirstNumber(nSpeed,nMaxNum);
		m_pImgMainSpeedNum->Move(nPosX,nPosY);
		m_pImgMainSpeedNum->SetRect(nNum*SPEED_STRING_WIDTH,0,(nNum+1)*SPEED_STRING_WIDTH,SPEED_STRING_HEIGHT);	
		m_pImgMainSpeedNum->Render();		
		nPosX += 16; 
		nMaxNum--;
	}
}

int CINFGameMainUnitInfoBar::GetMaxSize(int nNum)
{
	int nMaxSize = 0;
	int nNumber = abs(nNum);
	if(nNumber >= 1000)
	{
		nMaxSize = 4;
	}
	else if(nNumber >= 100)
	{
		nMaxSize = 3;
	}
	else if(nNumber >= 10)
	{
		nMaxSize = 2;
	}
	else
	{
		nMaxSize = 1;
	}
	return nMaxSize;
}

int CINFGameMainUnitInfoBar::CutOffFirstNumber(int& nNum,int nMaxNum)
{
	int nResult = 0;
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	if(nMaxNum == 4)
	{
		nResult = nNum/1000;
		nNum = nNum%1000;
		return nResult;
	}
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
	else if(nMaxNum == 3)
	{
		nResult = nNum/100;
		nNum = nNum%100;
		return nResult;
	}
	else if(nMaxNum == 2)
	{
		nResult = nNum/10;
		nNum = nNum%10;
		return nResult;
	}
	else if(nMaxNum == 1)
	{
		return nNum;
	}
	return 0;
}

void CINFGameMainUnitInfoBar::ShowMapNameTitle(UINT x, UINT y)
{
	char buf[256];
	memset(buf, 0x00, 256);
	MAP_INFO* mapname = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	if(mapname)
	{
		strncpy(buf, mapname->MapName, sizeof(mapname->MapName));
		wsprintf(buf, "%s(%d)", mapname->MapName,g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.ChannelIndex);
		m_pFontMapName->DrawText(x + 12 , y-3, GAME_MAP_NAME_TEXT_COLOR, buf);
	}
}

void CINFGameMainUnitInfoBar::RenderMapPosition(UINT x, UINT y)
{
	char buf[64];
	int nHeightPerTile;

	if(IS_SPACE_MAP_INDEX(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE)
	{
		nHeightPerTile = (g_pD3dApp->m_pShuttleChild->m_vPos.y-g_pGround->m_projectInfo.fWaterHeight+ALTITUDE_SAPCE_ALTITUDE)/TILE_SIZE;
	}
	else
	{
		nHeightPerTile = (g_pD3dApp->m_pShuttleChild->m_vPos.y-g_pGround->m_projectInfo.fWaterHeight)/TILE_SIZE;
	}

	wsprintf(buf, "\\cX:%3d\\c \\gY:%3d\\g \\pH:%2d\\p",(int)(g_pD3dApp->m_pShuttleChild->m_vPos.x/TILE_SIZE),
								 (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z/TILE_SIZE),
								 nHeightPerTile);

	m_pFontUnitPosInfo->DrawText( x + 30 , y-4, GAME_UNIT_POS_COLOR, buf, 0L);
}

BOOL CINFGameMainUnitInfoBar::RenderCurrentEvent(UINT x, UINT y)
{
	if(g_pShuttleChild->IsWarpGateZone())
	{
		return FALSE;
	}
	
	if(	g_pShuttleChild && 
		g_pShuttleChild->m_pPkNormalTimer && 
		g_pShuttleChild->m_pPkNormalTimer->IsCityWar())
	{
		m_pFontCurrentEvent->DrawText(x, y, GUI_FONT_COLOR_YM, STRMSG_C_INTERFACE_0024);//"도시점령전 진행중"
		return TRUE;	// 도시점령전 중
	}

	if( g_pShuttleChild &&
		IsPkEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		m_pFontCurrentEvent->DrawText(x+20, y, RGB(255,54,0), STRMSG_C_INTERFACE_0025);//"PK 가능 지역"
		return TRUE;	// PK 가능 맵
	}

	return FALSE;	// 아무런 이벤트가 없다.
}

BOOL CINFGameMainUnitInfoBar::IsRenderMapName()
{
	float fRadius;
	fRadius = D3DXVec2Length(&(m_v2MousePt - m_v2RaderCenter));
	if(fRadius > RADER_RADIUS)
	{
		return TRUE;
	}
	return FALSE;
}

void CINFGameMainUnitInfoBar::RenderPremiumCard(BOOL bIsCity)
{
	int x,y;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	x = g_pD3dApp->GetBackBufferDesc().Width - 104;
#else
	x = g_pD3dApp->GetBackBufferDesc().Width - 64;
#endif

//	if(bIsCity)
//		y = g_pD3dApp->GetBackBufferDesc().Height - 107;
//	else
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		y = g_pD3dApp->GetBackBufferDesc().Height - 52 - GAME_EVENT_BASICPOS_Y;

// 	if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
// 		y -= GAME_EVENT_BASICPOS_Y;
#else
		y = g_pD3dApp->GetBackBufferDesc().Height - 82;

	if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
		y -= GAME_EVENT_BASICPOS_Y;

	if(m_bMemberPCBang)
	{
		POINT	pt = m_pImgGeneralPremiumCard->GetImgSize();
		y -= pt.y;
	}
#endif

	// 2006-07-05 by ispark, 프리미엄 개념 변경
	if(g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1)
	{
		m_pImgGeneralPremiumCard->Move(x,y);
		m_pImgGeneralPremiumCard->Render();

		// 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가

		if(m_bPremiumToolTip)
		{
			char szBuff[MAX_PATH];
			int curSec	= GetServerDateTime().GetTimeInSeconds();
			int	endSec	= g_pD3dApp->GetPrimiumCardInfo()->atumTimeExpireTime1.GetTimeInSeconds();

			int leftDay		= (endSec - curSec) / 86400;
			int leftHour	= (endSec - curSec) % 86400 / 3600;
			int leftMin		= (endSec - curSec) % 86400 % 3600 / 60;

			if(leftDay <= 0 && leftHour <= 0 && leftMin <= 100)
				sprintf(szBuff, STRMSG_C_090316_0302, leftMin);			// "[멤버십 잔여시간] 00분 남음"
			else
				sprintf(szBuff, STRMSG_C_090316_0301, leftDay, leftHour);	// "[멤버십 잔여시간] 00일 00시간 남음"

			POINT	drawPt;
			SIZE	strSize = m_pFontEventRecovery->GetStringSize(szBuff);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			drawPt.x	= g_pD3dApp->GetBackBufferDesc().Width - strSize.cx;
#else
			drawPt.x	= x - strSize.cx;
#endif
			drawPt.y	= y - strSize.cy;

			g_pGameMain->RenderPopUpWindowImage(drawPt.x, drawPt.y + strSize.cy * 0.1f, strSize.cx, 1);
			m_pFontEventRecovery->DrawText(drawPt.x, drawPt.y, GUI_FONT_COLOR_Y, szBuff);
		}

		// end 2009. 03. 16 by ckPark 멤버쉽 툴팁 추가
	}

}

void CINFGameMainUnitInfoBar::RenderHappyHourEvent(BOOL bIsCity)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(IsHappyHourEvent())
	{	
		m_nHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 77;
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		m_nHappyHourY -= RESTKILLCOUNT_MOVE_HAPPYHOUR;
#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	
		m_pImgHappyHourEvent->Move(m_nHappyHourX, m_nHappyHourY);
		m_pImgHappyHourEvent->Render();
	}
	
	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
 	if(m_pMemberPremiumEvent)
 	{
		m_nPCHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 122;
		m_pImgPCHomePremiumEvent->Move(m_nPCHappyHourX - 277,m_nPCHappyHourY + 41);
		m_pImgPCHomePremiumEvent->Render();
	}
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
 	else if(m_bMemberPCBang)
 	{
		m_nPCHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 122;

		m_pImgPCHappyHourEvent->Move(m_nPCHappyHourX - 277,m_nPCHappyHourY + 37);	// 2012-03-29 by mspark, 가맹PC방 이미지 위치 수정 - 기존 41에서 37로 수정
		m_pImgPCHappyHourEvent->Render();		
	}
#else
	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	//if(m_BHappyHourEvent)
	if(IsHappyHourEvent())
	{	
// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
//		if(bIsCity)
//			m_nHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 99;
//		else
			m_nHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 57;
//		if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//			m_nHappyHourY -= GAME_EVENT_BASICPOS_Y;

		m_pImgHappyHourEvent->Move(m_nHappyHourX, m_nHappyHourY);
		m_pImgHappyHourEvent->Render();
	}

	// 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	if(m_pMemberPremiumEvent)
	{
		m_nPCHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 87;
		m_pImgPCHomePremiumEvent->Move(m_nPCHappyHourX,m_nPCHappyHourY);
		m_pImgPCHomePremiumEvent->Render();
	}
	//end 2010. 06. 08 by jskim 홈프리미엄 UI 작업
	else if(m_bMemberPCBang)
	{
// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
//		if(bIsCity)
//			m_nPCHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 107;
//		else
			m_nPCHappyHourY = g_pD3dApp->GetBackBufferDesc().Height - 82;
		
//		if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//			m_nPCHappyHourY -= GAME_EVENT_BASICPOS_Y;
		
		m_pImgPCHappyHourEvent->Move(m_nPCHappyHourX,m_nPCHappyHourY);
		m_pImgPCHappyHourEvent->Render();		
	}
#endif
}

void CINFGameMainUnitInfoBar::RenderEventRecovery()
{
	if(TRUE == g_bEventRecovery)
	{
		int nCount = ((int)(g_fEventRecoveryRate * 100.0f) / 5.0f) - 1;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		int nEventX = g_pD3dApp->GetBackBufferDesc().Width - 196 ;
		int nEventY = g_pD3dApp->GetBackBufferDesc().Height - 102;	// 2012-08-14 by mspark, 이벤트 회복율 이미지 위치 수정 - 기존 94에서 102로 수정
#else
		int nEventX = g_pD3dApp->GetBackBufferDesc().Width - 105;
		int nEventY = g_pD3dApp->GetBackBufferDesc().Height - 83;
#endif
	// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		nEventY -= RESTKILLCOUNT_MOVE_HAPPYHOUR;
#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
//		if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//			nEventY -= GAME_EVENT_BASICPOS_Y;

		// 2009. 08. 21 by ckPark 이벤트 회복률 단계 버그 수정
		if( nCount > GAMEMAIN_EVENT_RECOVERY_MAX - 1 )
			nCount = GAMEMAIN_EVENT_RECOVERY_MAX - 1;
		if( nCount < 0 )
			nCount = 0;
		// end 2009. 08. 21 by ckPark 이벤트 회복률 단계 버그 수정

		if(m_pImgEventRecovery[nCount])
		{
			m_pImgEventRecovery[nCount]->Move(nEventX, nEventY);
			m_pImgEventRecovery[nCount]->Render();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::RenderStringBar()
/// \brief		이벤트 설명
/// \author		ispark
/// \date		2006-06-08 ~ 2006-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::RenderStringBar()
{
	// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
	//if(m_BHappyHourEvent)
	if(IsHappyHourEvent())
	{
		// 2006-04-25 by ispark, 마우스 위치일때 보여주는 스트링
		if(m_bRenderEvent == TRUE)
		{
			// 2006-04-24 by ispark, 수정
//			int Height = 0;
			int nRenderX = 0;
			int leng = 0;
			vector<string>::iterator itU = m_vecEventHappyHour.begin();
			while (itU != m_vecEventHappyHour.end())
			{
				int nlengTemp = 0;
				nlengTemp = m_pFontEventHappyHour->GetStringSize((char*)(*itU).c_str()).cx;
//				if(strlen((char*)(*itU).c_str())>leng)
				if(nlengTemp > leng)
				{
					leng = nlengTemp + 10;
//					leng = strlen((char*)(*itU).c_str());
				}
				itU++;
			}
//			leng = leng*6.5;
			nRenderX = m_nHappyHourX + 10;
			if(nRenderX + leng > g_pD3dApp->GetBackBufferDesc().Width)
			{
				nRenderX = g_pD3dApp->GetBackBufferDesc().Width - leng;
			}

			int nIdex = 0;
			vector<string>::iterator it = m_vecEventHappyHour.begin();
			while (it != m_vecEventHappyHour.end())
			{
				g_pGameMain->RenderPopUpWindowImage(nRenderX, m_nHappyHourY-20-(nIdex*20), leng, 1);
				m_pFontEventHappyHour->DrawText(nRenderX, m_nHappyHourY-20-(nIdex*20)-GAMEMAIN_EVENT_FONT_Y, GUI_FONT_COLOR_Y, (char*)(*it).c_str());
				
				it++;
				nIdex++;
			}

		}
	}	

	if(TRUE == g_bEventRecovery)
	{
		int nEventX = g_pD3dApp->GetBackBufferDesc().Width - 88;
		int nEventY = g_pD3dApp->GetBackBufferDesc().Height - 83;
// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
//		if(g_pD3dApp->m_bCharacter && !g_pGround->m_bBazaar)
//			nEventY -= GAME_EVENT_BASICPOS_Y;

		if(TRUE == m_bRenderEventRecovery)
		{
			char buf[128] = {0,};

			// 2009. 08. 21 by ckPark 이벤트 회복률 단계 버그 수정
			if( g_fEventRecoveryRate > 0.25f )
				g_fEventRecoveryRate = 0.25f;
			if( g_fEventRecoveryRate < 0.0f )
				g_fEventRecoveryRate = 0.0f;
			// end 2009. 08. 21 by ckPark 이벤트 회복률 단계 버그 수정			

			sprintf(buf, STRMSG_C_060424_0002, (int)(g_fEventRecoveryRate * 100.0f));
			int leng = m_pFontEventRecovery->GetStringSize(buf).cx + 10;
			if(nEventX + leng > g_pD3dApp->GetBackBufferDesc().Width)
			{
				nEventX = g_pD3dApp->GetBackBufferDesc().Width - leng;
			}
			g_pGameMain->RenderPopUpWindowImage(nEventX, nEventY-13, leng, 1);
			m_pFontEventRecovery->DrawText(nEventX, nEventY-GAMEMAIN_EVENT_FONT_Y-13, GUI_FONT_COLOR_Y, buf);
		}
	}

// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
#ifdef C_DECA_TOOLTIP
	int nY = RESTKILLCOUNT_BASE_POS_Y - RESTCOUNT_BACK_POS_Y;
	if(m_bRestCountToolTip)
		DecaToolTipFunction(TRUE, nY);
	
	if(m_bKillCountToolTip)
	{
		if(m_nRestCount)
			nY += (RESTCOUNT_STRING_HEIGHT*2 + RESTKILLCOUNT_BASE_HEIGHT_SIZE + RESTCOUNT_BACK_POS_Y + 13);
		DecaToolTipFunction(FALSE, nY);
	}
#endif
// end 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFGameMainUnitInfoBar::IsHappyHourEvent()
/// \brief		
/// \author		// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainUnitInfoBar::IsHappyHourEvent()
{	
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if(m_bPcBangHappyHour)
	{
		return TRUE;
	}
	else if(TRUE == m_bHappyHourAllInfluence)
	{
		return TRUE;
	}	
	else if(COMPARE_INFLUENCE(m_byHappyHourInfluence, myShuttleInfo.InfluenceType))
	{
		return TRUE;
	}	
	// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
	else if(m_bMotherHappyHour || m_bItemHappyHour)
	{
		return TRUE;
	}
	// end 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFGameMainUnitInfoBar::IsHappyHourEvent()
/// \brief		
/// \author		// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainUnitInfoBar::IsHappyHourPcBang()
{
	return m_bPcBangHappyHour;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::SetHappyHourInfluence(BYTE byInfluence)
/// \brief		
/// \author		// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::SetHappyHourInfluence(BOOL bLevelup, BOOL bHappyHour, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg)
{
	BYTE byInfluence = pMsg->byInfluenceType4;

	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// 각 세력별 해피 아우어 인포
	EVENT_INFO* pEventInfo = NULL;
	BOOL bAllInfluence = FALSE;
	if(byInfluence == INFLUENCE_TYPE_ALL_MASK)
	{
		bAllInfluence = TRUE;
		pEventInfo = &m_HappyEventAllInfluInfo;
	}
	else if(COMPARE_INFLUENCE(byInfluence, myShuttleInfo.InfluenceType))
	{
		pEventInfo = &m_HappyEventInfo;		
	}
	else
	{
		return;
	}	
	// 변수 초기화	
	memset(pEventInfo, 0x00, sizeof(EVENT_INFO));

	if(bHappyHour)
	{		
		if(bAllInfluence)
		{
			m_bHappyHourAllInfluence = TRUE;			
		}
		else
		{
			m_byHappyHourInfluence = byInfluence;

		}
		// 전체 세력 해피 아워 인포
		SetHappyHourInfo(bLevelup, pMsg, pEventInfo);
	}
	else
	{
		if(bAllInfluence)
		{
			m_bHappyHourAllInfluence = FALSE;			
		}
		else
		{
			m_byHappyHourInfluence  = 0;		
		}		
	}

	// 해피 아우어 툴팁 갱신 
	UpdateHappyHourTooltip();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::SetHappyHourInfo(BYTE byInfluence)
/// \brief		
/// \author		// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::SetHappyHourInfo(BOOL bLevelup, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg, EVENT_INFO* pEventInfo)
{
	char szNotice[1024];
	memset(szNotice, 0x00, 1024);
	
	
	if(pMsg->fEXPRate2 > 0.0f)
	{		
		if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		{
			sprintf(szNotice, STRMSG_C_EVENT_0003,(pMsg->fEXPRate2)*100);//"몬스터 경험치 %.0f%% 증가"
			g_pGameMain->CreateChatChild(szNotice ,COLOR_SYSTEM);			
		}
		
		pEventInfo->fEXPRate = (pMsg->fEXPRate2);		
	}
	if(pMsg->fDropRareRate2 > 0.0f)
	{
		if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		{
			sprintf(szNotice, STRMSG_C_EVENT_0004,(pMsg->fDropRareRate2)*100);//"레어옵션 드롭율 %.0f%% 증가"
			g_pGameMain->CreateChatChild(szNotice ,COLOR_SYSTEM);	
		}
		
		pEventInfo->fDropRareRate = (pMsg->fDropRareRate2) ;
		
	}
	if(pMsg->fEXPRepairRate2 > 0.0f )
	{
		if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		{
			sprintf(szNotice, STRMSG_C_EVENT_0005,(pMsg->fEXPRepairRate2)*100);//"죽었을 경우 패널티 %.0f%% 감소"
			g_pGameMain->CreateChatChild(szNotice ,COLOR_SYSTEM);	
		}
		
		pEventInfo->fEXPRepairRate = (pMsg->fEXPRepairRate2) ;		
	}
	if(pMsg->fDropItemRate2 > 0.0f )
	{
		if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		{
			sprintf(szNotice, STRMSG_C_EVENT_0006,(pMsg->fDropItemRate2)*100);//"아이템 드롭율 %.0f%% 증가"
			g_pGameMain->CreateChatChild(szNotice ,COLOR_SYSTEM);	
		}
		
		pEventInfo->fDropItemRate = (pMsg->fDropItemRate2);

	}
	if(pMsg->fSPIRate2 > 0.0f )
	{
		if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		{
			sprintf(szNotice, STRMSG_C_EVENT_0007,(pMsg->fSPIRate2)*100);//"스피량 %.0f%% 증가"
			g_pGameMain->CreateChatChild(szNotice ,COLOR_SYSTEM);	
		}
		
		pEventInfo->fSPIRate = (pMsg->fSPIRate2);
		
	}
	if(pMsg->fWarPointRate2 > 0.0f )
	{// 전쟁 포인트.
		if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
		{
			sprintf(szNotice, STRMSG_C_070626_0100,(pMsg->fWarPointRate2)*100);//"전쟁 포인트 %d%% 증가"
			g_pGameMain->CreateChatChild(szNotice ,COLOR_SYSTEM);	
		}
		
		pEventInfo->fWarPointRate = (pMsg->fWarPointRate2);
		
	}	
	if(!bLevelup) // 레벨업이 아닐떄만 채팅에 표시 // 2008-03-14 by bhsohn 레벨업에 따른 해피아우어 처리
	{
		g_pGameMain->CreateChatChild(STRMSG_C_EVENT_0008 ,COLOR_SYSTEM);//"좋은 시간 되시기 바랍니다. ^^"	
	}
}

void CINFGameMainUnitInfoBar::UpdateHappyHourTooltip()
{
	char szNotice[1024];
	memset(szNotice, 0x00, 1024);
	float fTmp = 0.0f;

	{
		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
		//fTmp = m_HappyEventInfo.fEXPRate+m_HappyEventAllInfluInfo.fEXPRate+m_PCHappyEventInfo.fEXPRate;
		fTmp = m_HappyEventInfo.fEXPRate+m_HappyEventAllInfluInfo.fEXPRate+m_PCHappyEventInfo.fEXPRate
				+m_MotherShipEventInfo.fEXPRate+m_ItemEventInfo.fEXPRate;

		if(fTmp > 0)
		{
			sprintf(szNotice, STRMSG_C_EVENT_0003,(fTmp)*100);//"몬스터 경험치 %.0f%% 증가"
			m_vecEventHappyHour.push_back(szNotice);
		}
	}
	{
		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
		//fTmp = m_HappyEventInfo.fDropRareRate +m_HappyEventAllInfluInfo.fDropRareRate+m_PCHappyEventInfo.fDropRareRate;
		fTmp = m_HappyEventInfo.fDropRareRate +m_HappyEventAllInfluInfo.fDropRareRate+m_PCHappyEventInfo.fDropRareRate
				+m_MotherShipEventInfo.fDropRareRate+m_ItemEventInfo.fDropRareRate;
		if(fTmp)
		{
			sprintf(szNotice, STRMSG_C_EVENT_0004,(fTmp)*100);//"레어옵션 드롭율 %.0f%% 증가"
			m_vecEventHappyHour.push_back(szNotice);
		}
		
	}

	{
		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
		//fTmp = m_HappyEventInfo.fEXPRepairRate +m_HappyEventAllInfluInfo.fEXPRepairRate+m_PCHappyEventInfo.fEXPRepairRate;
		fTmp = m_HappyEventInfo.fEXPRepairRate +m_HappyEventAllInfluInfo.fEXPRepairRate+m_PCHappyEventInfo.fEXPRepairRate
				+m_MotherShipEventInfo.fEXPRepairRate+m_ItemEventInfo.fEXPRepairRate;

		// 패널티는 100%가 넘을수 없다. 
		if(fTmp > 1.0f)
		{
			fTmp = 1.0f;
		}
		
		if(fTmp > 0)
		{
			sprintf(szNotice, STRMSG_C_EVENT_0005,(fTmp)*100);//"죽었을 경우 패널티 %.0f%% 감소"
			m_vecEventHappyHour.push_back(szNotice);
		}
	}

	{
		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
		//fTmp = m_HappyEventInfo.fDropItemRate +m_HappyEventAllInfluInfo.fDropItemRate+m_PCHappyEventInfo.fDropItemRate;
		fTmp = m_HappyEventInfo.fDropItemRate +m_HappyEventAllInfluInfo.fDropItemRate+m_PCHappyEventInfo.fDropItemRate
				+m_MotherShipEventInfo.fDropItemRate+m_ItemEventInfo.fDropItemRate;
		if(fTmp>0)
		{
			sprintf(szNotice, STRMSG_C_EVENT_0006,(fTmp)*100);//"아이템 드롭율 %.0f%% 증가"
			m_vecEventHappyHour.push_back(szNotice);
		}
	}

	{
		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
		//fTmp = m_HappyEventInfo.fSPIRate +m_HappyEventAllInfluInfo.fSPIRate+m_PCHappyEventInfo.fSPIRate;
		fTmp = m_HappyEventInfo.fSPIRate +m_HappyEventAllInfluInfo.fSPIRate+m_PCHappyEventInfo.fSPIRate
				+m_MotherShipEventInfo.fSPIRate+m_ItemEventInfo.fSPIRate;

		if(fTmp > 0)
		{
			sprintf(szNotice, STRMSG_C_EVENT_0007,(fTmp)*100);//"스피량 %.0f%% 증가"
			m_vecEventHappyHour.push_back(szNotice);
		}
	}

	{
		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
		//fTmp = m_HappyEventInfo.fWarPointRate +m_HappyEventAllInfluInfo.fWarPointRate+m_PCHappyEventInfo.fWarPointRate;
		fTmp = m_HappyEventInfo.fWarPointRate +m_HappyEventAllInfluInfo.fWarPointRate+m_PCHappyEventInfo.fWarPointRate
				+m_MotherShipEventInfo.fWarPointRate +m_ItemEventInfo.fWarPointRate;

		if(fTmp >0)
		{
			sprintf(szNotice, STRMSG_C_070626_0100,(fTmp)*100);//"스피량 %.0f%% 증가"
			m_vecEventHappyHour.push_back(szNotice);
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::SetHappyHourPcBang(BOOL bPcBang)
/// \brief		
/// \author		// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::SetHappyHourPcBang(BOOL bLevelup, BOOL bPcBang, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg)
{
	m_bPcBangHappyHour = bPcBang;

	char szNotice[1024];
	memset(szNotice, 0x00, 1024);
	memset(&m_PCHappyEventInfo,0x00,sizeof(EVENT_INFO));
	
	if(m_bPcBangHappyHour)
	{
		SetHappyHourInfo(bLevelup, pMsg, &m_PCHappyEventInfo);
	}	

	// 해피 아우어 툴팁 갱신 
	UpdateHappyHourTooltip();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::SetHappyHourMotherShip(BOOL bPcBang)
/// \brief		// 모선전 해피 아우어
/// \author		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::SetHappyHourMotherShip(BOOL bLevelup, BOOL bStart, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg)
{
	m_bMotherHappyHour = bStart;

	char szNotice[1024];
	memset(szNotice, 0x00, 1024);
	memset(&m_MotherShipEventInfo,0x00,sizeof(EVENT_INFO));
	
	if(m_bMotherHappyHour)
	{
		SetHappyHourInfo(bLevelup, pMsg, &m_MotherShipEventInfo);
	}	

	// 해피 아우어 툴팁 갱신 
	UpdateHappyHourTooltip();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::SetHappyHourItem(BOOL bPcBang)
/// \brief		아이템 해피 아우어
/// \author		// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::SetHappyHourItem(BOOL bLevelup, BOOL bStart, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg)
{
	m_bItemHappyHour = bStart;

	char szNotice[1024];
	memset(szNotice, 0x00, 1024);
	memset(&m_ItemEventInfo,0x00,sizeof(EVENT_INFO));
	
	if(m_bItemHappyHour)
	{
		SetHappyHourInfo(bLevelup, pMsg, &m_ItemEventInfo);
	}	

	// 해피 아우어 툴팁 갱신 
	UpdateHappyHourTooltip();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::InitHappyHourInfo()
/// \brief		
/// \author		// 2007-11-01 by bhsohn 해피 아우어 이벤트 처리 방식 변경
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::InitHappyHourInfo()
{
	m_vecEventHappyHour.clear(); // 툴팁 정보 초기화

	memset(&m_HappyEventInfo, 0x00, sizeof(EVENT_INFO));				//세력 진형
	memset(&m_HappyEventAllInfluInfo, 0x00, sizeof(EVENT_INFO));		// 전체 진형
	memset(&m_PCHappyEventInfo, 0x00, sizeof(EVENT_INFO));				// pc방 
	
	m_bHappyHourAllInfluence = FALSE;
	m_byHappyHourInfluence = 0x00;
	m_bPcBangHappyHour = FALSE;

	// 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가
	m_bMotherHappyHour = FALSE;
	memset(&m_MotherShipEventInfo, 0x00, sizeof(EVENT_INFO));

	m_bItemHappyHour = FALSE;
	memset(&m_ItemEventInfo, 0x00, sizeof(EVENT_INFO));
	// end 2008-05-21 by bhsohn 모선전, 아이템 해피 아우어 추가

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::AddGiftMsg(UINT i_NotifyMsgUID,UINT i_CharacterUID,char * NotifyString)
/// \brief		선물 메시지를 추가한다.
/// \author		dgwoo
/// \date		2007-11-28 ~ 2007-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::AddGiftMsg(UINT i_nNotifyMsgUID,UINT i_nCharacterUID,char * i_cNotifyString)
{
	GIFT_NOTIFY_MSG stGiftMsg;
	stGiftMsg.NotifyMsgUID = i_nNotifyMsgUID;
	stGiftMsg.CharacterUID = i_nCharacterUID;
	memcpy(stGiftMsg.NotifyMsgString,i_cNotifyString,SIZE_MAX_NOTIFY_MSG_STRING);
	m_vecGiftMsg.push_back(stGiftMsg);
	g_pGameMain->CreateChatChild(STRMSG_C_071129_0100 ,COLOR_ITEM);	
	m_fGiftTime = 0.0f;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainUnitInfoBar::CreateMsgBox()
/// \brief		메시지 박스를 띄운다.
/// \author		dgwoo
/// \date		2007-11-28 ~ 2007-11-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainUnitInfoBar::CreateMsgBox()
{
	if(m_vecGiftMsg.size())
	{
		vector<GIFT_NOTIFY_MSG>::iterator it = m_vecGiftMsg.begin();
		g_pGameMain->m_pInfWindow->AddMsgBox(it->NotifyMsgString,_Q_GIFT_ITEM_RING_IN,0,0,0,it->NotifyMsgUID);	
	}
}
void CINFGameMainUnitInfoBar::DeleteGiftMsg()
{
	if(m_vecGiftMsg.size())
	{
		vector<GIFT_NOTIFY_MSG>::iterator it = m_vecGiftMsg.begin();
		m_vecGiftMsg.erase(it);
	}
	else
	{
		m_fGiftTime = 0.0f;
	}
}

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CINFGameMainUnitInfoBar::SetRestCount(INT nRestCount)
{
	m_nRestCount = nRestCount;
}
void CINFGameMainUnitInfoBar::SetKillCount(INT nKillCount)
{
	m_nKillCount = nKillCount;
	if(m_nKillCount == 30)
	{
		m_bCompleteKillCount = true;
		m_nCompleteKillCountBlank = 0;
		m_nCompleteKillCountAlpha = 0;
		m_fKillCountScale = KILLCOUNT2_SCALE;
	}
	else if(m_nKillCount > KILLCOUNT_SHOW_HIGHLIGHT_COUNT)
	{
		m_bChangeKillCount = true;
		m_fKillCountScale = KILLCOUNT_SCALE_MAX;
	}

}
void CINFGameMainUnitInfoBar::SetBonusExpRate(INT nPartyRate,INT nFriendRate,INT nGuildRate)
{
//	m_fPartyRate = fPartyRate;
	m_nFriendRate = nFriendRate;
	m_nGuildRate = nGuildRate;
}
// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼
void CINFGameMainUnitInfoBar::RestCountImageOnOff()
{
	if(!m_bCheckRestCountON && m_fPlusRestCountPosX <= RESTCOUNT_IMAGE_POS_LIMIT)
	{
		m_fPlusRestCountPosX = m_fPlusRestCountPosX + RESTKILLCOUNT_IMAGE_MOVE_VALUE;
	}
	else if(m_bCheckRestCountON && m_fPlusRestCountPosX > 0)
	{
		m_fPlusRestCountPosX = m_fPlusRestCountPosX - RESTKILLCOUNT_IMAGE_MOVE_VALUE;
	}
}

void CINFGameMainUnitInfoBar::KillCountImageOnOff()
{
	if(!m_bCheckKillCountON && m_fPlusKillCountPosX <= KILLCOUNT_IMAGE_POS_LIMIT)
	{
		m_fPlusKillCountPosX = m_fPlusKillCountPosX + RESTKILLCOUNT_IMAGE_MOVE_VALUE;
	}
	else if(m_bCheckKillCountON && m_fPlusKillCountPosX > 0)
	{
		m_fPlusKillCountPosX = m_fPlusKillCountPosX - RESTKILLCOUNT_IMAGE_MOVE_VALUE;
	}
}
// end 2012-11-16 by mspark, 데카의 기운 표시 리뉴얼


// 2013. 04. 04 by ssjung 보너스 EXP , 맴버쉽 에 마우스를 가져다 댔을 때 시스템 메시지를 표시 안하게 수정
BOOL CINFGameMainUnitInfoBar::GetRenderBonusExpRateTextOnOff()
{
	return m_bRenderBonusExpRateText;
}
BOOL CINFGameMainUnitInfoBar::GetRenderPremiumToolTipTextOnOff()
{
	return m_bPremiumToolTip;
}
// 2013. 04. 04 by ssjung 보너스 EXP, 멤버쉽 에 마우스를 가져다 댔을 때 시스템 메시지를 표시 안하게 수정

// 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시
void CINFGameMainUnitInfoBar::DecaToolTipFunction(BOOL nMode, int nY)	//TRUE면 데카의 축복 FALSE면 데카의 기운 
{
#ifdef C_DECA_TOOLTIP
	char buf[128] = {0,};
	int nX = g_pD3dApp->GetBackBufferDesc().Width;
	
	if(nMode)
		sprintf(buf,STRMSG_C_130627_0001);	//"\\gBlessing of Decca\\g provides 35% bonus EXP until expired. Rest overnight to regain your blessing."

	else
		sprintf(buf,STRMSG_C_130627_0002, 30);	// "\\eStrength of Decca\\e: Defeat %d players without logging off to receive a special war reward."

	int leng = m_pFontDecaToolTip->GetStringSize(buf).cx + 10;
	if(nX + leng > g_pD3dApp->GetBackBufferDesc().Width)
	{
		nX = g_pD3dApp->GetBackBufferDesc().Width - leng;
	}
	g_pGameMain->RenderPopUpWindowImage(nX, nY-13, leng, 1);
	m_pFontDecaToolTip->DrawText(nX, nY-GAMEMAIN_EVENT_FONT_Y-13, GUI_FONT_COLOR_W, buf);
#endif
}
// end 2013-07-15 by ssjung 캐나다 데카의 축복, 기운 관련 툴팁 표시