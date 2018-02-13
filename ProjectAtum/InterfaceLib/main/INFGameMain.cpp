// INFGameMain.cpp: implementation of the CInterfaceGameMain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMain.h"
#include "QuestData.h"
#include "AtumApplication.h"

#include "GameDataLast.h"
#include "INFWindow.h"
#include "INFWeaponInfo.h"
// 2008-09-22 by bhsohn EP3 캐릭터 창
//#include "INFCharacterInfo.h"
#include "INFCharacterInfoExtend.h"
#include "RangeTime.h"
#include "INFInven.h"
// 2008-03-19 by bhsohn Ep3옵션창
//#include "INFSystem.h"
#include "INFOptionSystem.h"

#include "INFTrade.h"
#include "INFGameMainChat.h"
#include "INFIcon.h"
//#include "INFShop.h"
#include "INFUnitNameInfo.h"
#include "INFItemInfo.h"
//#include "INFNotice.h"
#include "INFGameMainUnitInfoBar.h"
#include "INFGameMainQSlot.h"
#include "INFGameMainMinimap.h"
#include "INFCommunity.h"
#include "INFUnitState.h"
#include "INFAttackDirection.h"
#include "INFGameHelpDesk.h"

#include "StoreData.h"
#include "EnemyData.h"
#include "MonsterData.h"
#include "INFCommunityParty.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "D3DHanFont.h"
#include "Interface.h"
#include "AtumDatabase.h"
#include "Chat.h"
#include "INFCityBase.h"

// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "INFImage.h"
#include "Skill.h"
#include "AtumSound.h"
#include "ClientParty.h"
#include "INFQuest.h"
#include "INFSkill.h"
#include "INFGameCountDown.h"
#include "INFDamageView.h"
#include "INFOtherCharInfo.h"
#include "dxutil.h"
#include "INFGameMainFAQ.h"
#include "INFMenuList.h"
#include "INFCommunityGuild.h"
#include "INFTarget.h"
#include "INFMp3Player.h"
#include "TutorialSystem.h"
#include "INFMissionInfo.h"
// 2007-06-12 by bhsohn 미션 인터페이스 수정안
#include "INFMissionMain.h"
#include "FieldWinSocket.h"
#include "GlobalImage.h"
#include "INFCommunityVOIP.h"
#include "ItemInfo.h"
#include "Background.h"
#include "INFCityBoard.h"
#include "INFGameMainOutPost.h"

// 2007-09-12 by bhsohn 2차 암호 시스템 구현
#include "INFSecuMain.h"
#include "INFChangeCharactor.h"
#include "INFCouponWindow.h"
// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
#include "INFMotherShipManager.h"
#include "INFCityLeader.h"
#include "INFCommunityReject.h"

// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
#include "INFCityOutPost.h"

#include "INFGameMainSysMsg.h"	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

#include "INFOpJoystick.h"		// 2008-11-13 by bhsohn 조이스틱 작업

#include "INFWorldRankWnd.h"	// 2009-02-13 by bhsohn 월드 랭킹 시스템
#include "INFItemMixWnd.h"		// 2013-02-20 by bhsohn 인게임 조합창 처리
#include "INFItemMenuList.h"	// 2013-02-26 by bhsohn 인게임 조합 검색 처리

#include "INFArmorCollectWnd.h"		// 2013-05-28 by bhsohn 아머 컬렉션 시스템

#include "INFInfinity.h"		// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.

// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
// end 2011. 10. 10 by jskim UI시스템 변경
// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#include "INFToolTip.h"
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

#include <BonusKillSystem.h>			// 2015-06-16 by Future, Bonus Kill System Definitions

#define IMG_UNIT_LOCK_SCALE		((float)g_pD3dApp->GetBackBufferDesc().Width/1024.0f)
#define IMG_UNIT_LOCK_SIZE_X	118
#define IMG_UNIT_LOCK_SIZE_Y	36
#define IMG_UNIT_LOCK_START_X	(g_pD3dApp->GetBackBufferDesc().Width-IMG_UNIT_LOCK_SIZE_X*IMG_UNIT_LOCK_SCALE)/2
#define IMG_UNIT_LOCK_START_Y	3


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define GAMEMAIN_BUTTON_START_X			(g_pD3dApp->GetBackBufferDesc().Width-275)
#define GAMEMAIN_BUTTON_START_Y			(g_pD3dApp->GetBackBufferDesc().Height-53)
#define GAMEMAIN_BK_START_X				(g_pD3dApp->GetBackBufferDesc().Width-335)
#define GAMEMAIN_BK_START_Y				(g_pD3dApp->GetBackBufferDesc().Height-53)
#define GAMEMAIN_BUTTON_SIZE_X			47//57
#define GAMEMAIN_BUTTON_SIZE_Y			18//18
#define GAMEMAIN_BUTTON_INTERVAL		55//60

#define GAME_BUTTON_CHECK_ON					0.5f

#define GAMEMAIN_FONT_LINE_HEIGHT				15							// 한 라인의 높이
#define GAMEMAIN_FONT_WIDTH_ENGLISH				6							// 영문 글자 WIDTH

#define GAMEMAIN_MISSION_TIME_CHECK				5
// 고도계설정
#define GAMEMAIN_ALTIMETER_X (g_pD3dApp->GetBackBufferDesc().Width-32)
#define GAMEMAIN_ALTIMETER_Y 188										// 2007-02-22 by dgwoo 위치 수정.165	// 2012-03-29 by mspark, 미니맵&고도 표시 위치 수정 - 기존 178에서 188로 수정

// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
#define GAMEMAIN_SERVER_TIME_X (g_pD3dApp->GetBackBufferDesc().Width-168)
#define GAMEMAIN_SERVER_TIME_Y 313
#define GAMEMAIN_SERVER_TIME_WIDTH (185)


// 2007-07-04 by bhsohn 오퍼레이터 추가
#define FAQ_BTN_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-330)
#define FAQ_BTN_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-53)
#define	FAQ_BLING_TIME		(1.0f)

#define COUPON_BTN_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-75)
#define COUPON_BTN_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-190)
#define COUPON_WINDOW_POS_X		((g_pD3dApp->GetBackBufferDesc().Width - 307)/2)
#define COUPON_WINDOW_POS_Y		((g_pD3dApp->GetBackBufferDesc().Height - 183)/2)	   
#else
#define GAMEMAIN_BUTTON_START_X			(g_pD3dApp->GetBackBufferDesc().Width-251)
#define GAMEMAIN_BUTTON_START_Y			(g_pD3dApp->GetBackBufferDesc().Height-22)
#define GAMEMAIN_BUTTON_SIZE_X			47//57
#define GAMEMAIN_BUTTON_SIZE_Y			18//18
#define GAMEMAIN_BUTTON_INTERVAL		50//60

#define GAME_BUTTON_CHECK_ON					0.5f

#define GAMEMAIN_FONT_LINE_HEIGHT				15							// 한 라인의 높이
#define GAMEMAIN_FONT_WIDTH_ENGLISH				6							// 영문 글자 WIDTH

#define GAMEMAIN_MISSION_TIME_CHECK				5
// 고도계설정
#define GAMEMAIN_ALTIMETER_X (g_pD3dApp->GetBackBufferDesc().Width-32)
#define GAMEMAIN_ALTIMETER_Y 183										// 2007-02-22 by dgwoo 위치 수정.165

// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
#define GAMEMAIN_SERVER_TIME_X (g_pD3dApp->GetBackBufferDesc().Width-168)
#define GAMEMAIN_SERVER_TIME_Y 313
#define GAMEMAIN_SERVER_TIME_WIDTH (185)


// 2007-07-04 by bhsohn 오퍼레이터 추가
#define FAQ_BTN_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-287)
#define FAQ_BTN_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-35)
#define	FAQ_BLING_TIME		(1.0f)

#define COUPON_BTN_POS_X		(g_pD3dApp->GetBackBufferDesc().Width-322)
#define COUPON_BTN_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height-35)
#define COUPON_WINDOW_POS_X		((g_pD3dApp->GetBackBufferDesc().Width - 307)/2)
#define COUPON_WINDOW_POS_Y		((g_pD3dApp->GetBackBufferDesc().Height - 183)/2)
#endif

// 2009. 10. 14 by jskim 프리스카 제거
// 튜토리얼 버튼 추가
#define TUTORIAL_BUTTON_X		(g_pD3dApp->GetBackBufferDesc().Width - 73)		// 2012-03-29 by mspark, 레벨 1~10에서 보여지는 튜토리얼 버튼 위치 수정 - 기존 64에서 73으로 변경
#define TUTORIAL_BUTTON_Y		326												// 2012-03-29 by mspark, 레벨 1~10에서 보여지는 튜토리얼 버튼 위치 수정 - 기존 315에서 326으로 변경
//end 2009. 10. 14 by jskim 프리스카 제거
// 2007-07-20 by bhsohn 튜토리얼 수정
//#define	VCN_FIRST_QUEST_IDX		113		// 바이제니유 첫번? 퀘스트
//#define	ANI_FIRST_QUEST_IDX		501		// 알링턴 첫번? 퀘스트
#define	SELECT_FIRST_QUEST_IDX		112		// 세력선택 퀘스트


// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define FONTTOOLTIP_Y			17
#else
#define FONTTOOLTIP_Y			17//15
#endif

// 2007-09-05 by bhsohn 전진 기지전
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif

// 2008-10-20 by bhsohn VOIP 버그 수정
#define		VOLUME_SET_MIN			1
#define		VOLUME_SET_MAX			100

// 2008-11-13 by bhsohn 조이스틱 작업
#define JOYSTICK_WND_POS_X		(g_pD3dApp->GetBackBufferDesc().Width/2-427)
#define JOYSTICK_WND_POS_Y		(g_pD3dApp->GetBackBufferDesc().Height/2-213)
// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#define TUTORIAL_BUTTON_Y_ADDCOUNT	34
//#define KILLCOUNT_MAX				300
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
typedef struct
{	
	DWORD	dwKeyValue;
	int		nKey;	
} structNumTable;

structNumTable m_stucTable[] = 
{
	{0x000b0001, 0},
	{0x00020001, 1},
	{0x00030001, 2},
	{0x00040001, 3},
	{0x00050001, 4},
	
	{0x00060001, 5},
	{0x00070001, 6},
	{0x00080001, 7},
	{0x00090001, 8},
	{0x000a0001, 9},
	
	// Num패드
	{0x00520001, 0},
	{0x004f0001, 1},
	{0x00500001, 2},
	{0x00510001, 3},
	{0x004b0001, 4},
	
	{0x004c0001, 5},
	{0x004d0001, 6},
	{0x00470001, 7},
	{0x00480001, 8},
	{0x00490001, 9}	
};
// end 2009-01-30 by bhsohn 바자 상점 숫자 입력 방식 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
CINFGameMain::CINFGameMain(CAtumNode* pParent, CGameData* pGameData)
#else
CINFGameMain::CINFGameMain(CAtumNode* pParent)
#endif
{
	FLOG( "CINFGameMain(CAtumNode* pParent)" );
	g_pGameMain = this;
	m_pGameData = NULL;
	m_pUnitInfoBar = NULL ;
	m_pInfWindow = NULL ;
//	m_pParty = NULL ;
	m_pCommunity = NULL ;
	m_pWeaponInfo = NULL ;

	m_pTrade  = NULL;
	m_pInven  = NULL;
	m_pSystem  = NULL;
	m_pCharacterInfo  = NULL;

	m_pChat = NULL;
	m_pQuickSlot = NULL;
	m_pMiniMap = NULL;
	m_pIcon = NULL;
//	m_pShop = NULL;
	m_pUnitNameInfo = NULL;
	m_pQuest = NULL;
	m_pMission = NULL;
	m_pItemInfo = NULL;
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	m_pEquipItemInfo = NULL;	// 현재 장착한 아이템 툴팁
	m_bShowMyEqTooltip = FALSE;
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
//	m_pNotice = NULL;
	m_pUnitState = NULL;
	m_pAttackDirection = NULL;
	m_pInfSkill = NULL;
	m_pGameCountDown = NULL;
	m_pDamageView = NULL;
	m_pOtherCharInfo = NULL;
	m_pInfGameMainFaq =NULL;
	m_pMenuList = NULL;
	m_pMp3Player = NULL;
	
	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	//m_pMissionInfo = NULL;
	m_pMissionMain = NULL;
	
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	m_pINFWorldRankWnd = NULL;
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	
	// 2013-02-20 by bhsohn 인게임 조합창 처리
	m_pINFItemMixWnd = NULL;
	// END 2013-02-20 by bhsohn 인게임 조합창 처리

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	m_pINFArmorCollectWnd = NULL;		
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	// 2007-08-28 by dgwoo 전진기지전 class추가.
	m_pChangeCharactor = NULL;
	m_pCouponWindow = NULL;
	m_pOutPost = NULL;

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	m_pINFSpeakeMsg = NULL;

	//m_pCommunityVOIP = NULL;
	m_pCityBoard = NULL;

	m_nLeftWindowState = LEFT_WEAPON_WINDOW_INIT ;
	m_bLeftWindowModeChage = FALSE;
	m_nLeftWindowInfo = LEFT_WINDOW_WEAPON;
	m_nLeftWeaponInfoPosX = 0;
	m_nLeftWindowPosY = DEAULT_WINDOW_POS_Y;
	m_nLeftWindowScaleSize = SIZE_WEAPON_Y;

	m_nRightWindowState = RIGHT_WEAPON_WINDOW_INIT ;
	m_bRightWindowModeChage = FALSE;
	m_nRightWindowInfo = RIGHT_WINDOW_WEAPON;
	m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width - SIZE_ROLL_X;
	m_nRightWindowPosY = DEAULT_WINDOW_POS_Y;
	m_nRightWindowScaleSize = SIZE_WEAPON_Y;

	m_pParent = pParent;


	m_pFontToolTip = NULL;
	m_pFontTimeLimit	= NULL;
	m_pFontDrawMent = NULL;
	memset(m_strToolTip,0x00,sizeof(m_strToolTip));
	m_ptToolTip.x = 0;
	m_ptToolTip.y = 0;
	m_bShowHelp = FALSE;
	memset(m_pHelp, 0x00, sizeof(DWORD)*HELP_NUM);
//	m_pImgUnitLock = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	memset(m_pImgButton, 0x00, sizeof(DWORD)*GAMEMAIN_BUTTON_NUMBER);							  
#else 
	memset(m_pImgButton, 0x00, sizeof(DWORD)*GAMEMAIN_BUTTON_NUMBER*BUTTON_STATE_NUMBER);
	memset(m_nButtonState, 0x00, sizeof(int)*GAMEMAIN_BUTTON_NUMBER);
#endif
	m_pImgMission = NULL;

	m_pSelectIcon = NULL;
	int i;
	for(i=0; i<3; i++)
	{
		m_pImgTextPopUp[i] = NULL;
	}

	m_pImgTextBg = NULL;

	m_nIconPosX = 0;
	m_nIconPosY = 0;

	m_nLeftWindowY = DEAULT_WINDOW_POS_Y;
	m_nRightWindowY = DEAULT_WINDOW_POS_Y;
	m_bRestored = FALSE;

	m_pHelpDesk		= NULL;
	m_bHelpDeskFlag = FALSE;
	m_bQuestView	= FALSE;	

	m_bQuestLimitTimeView	= FALSE;
	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
//	m_fTimeElapse			= 0.0f;
//	m_nTimeSecond			= GAMEMAIN_TIMESET_ZERO;
	m_fQuestTimeElapse		=GAMEMAIN_TIMESET_ZERO;

//	memset(m_strNPCName, 0x00, sizeof(m_strNPCName));
	
	m_bFAQProsFlag			= FALSE;
	m_bMenuListFlag			= FALSE;
	m_bMenuLock				= FALSE;
	m_bEnterBuilingCashShop = FALSE;
	m_bSendCashItemBuy		= FALSE;
	
	// 2005-03-03 by jschoi
	m_pRequestEnableBack		= NULL;
	m_pRequestEnableTriangle	= NULL;
	m_pAltiMeterGage			= NULL;
	m_pAltiMeterPin				= NULL;

	m_bChangeMousePoint			= FALSE;
	m_bQSlotIconFlag			= FALSE;
	m_nQSlotPosX				= 0;
	m_nQSlotPosY				= 0;
	m_bMp3PlayFlag				= TRUE;
	m_nTimeLimitMissionOldTime  = GAMEMAIN_TIMESET_ZERO;
	m_nLeftWindowInfo			= 0;				// 현재 임시 1 이면 인벤토리
	m_bVOIPFlag = FALSE;

	for(i=0; i<SIZE_MAX_POS; i++)
	{
		m_nItemSourceNum[i] = 0;		
		m_pItemInfoWear[i] = NULL;
		m_pITEMG[i] = NULL;
	}
	m_bUseShopItem = FALSE;
	m_nArmorColor = 0;
	m_nLeaveBuildingIndex = 0;
	m_ptGageBack.x = 0;
	m_ptGageBack.y = 0;
	m_ptGagePin.x = 0;
	m_ptGagePin.y = 0;

	m_stSelectItem.pSelectItem = NULL;
	m_stSelectItem.pSelectIcon = NULL;
	m_stSelectItem.bySelectType = ITEM_NOT_POS;

	m_pImgStack					= NULL;
	for(i = 0; i < 2; i++)
	{
		m_bKnowON[i] = FALSE;
	}
	m_bKnowTwinkle = FALSE;
	m_fTimeElapseButtonCheck	= GAME_BUTTON_CHECK_ON;	
	
	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	m_nMultiSelCnt = 0;	
	for(i = 0; i < MAX_MULTI_SEL;i++)
	{
		m_stMultiSelectItem[i].pSelectItem = NULL;
		m_stMultiSelectItem[i].pSelectIcon = NULL;
		m_stMultiSelectItem[i].bySelectType = ITEM_NOT_POS;
	}

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	m_pFaqBtn = NULL;	
	m_pCouponBtn = NULL;
	// 2009. 10. 14 by jskim 프리스카 제거
	m_pTut_SelBtn = NULL;
	//end 2009. 10. 14 by jskim 프리스카 제거
	// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합
	m_bShowStatOpWnd = FALSE;

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	m_pINFSecuMain = NULL;

	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	m_pMotherShipC = NULL;

	// 2015-05-01 by killburne
	m_pStrategicPointDisplay = NULL;

	m_bBuyMessage = FALSE;

	// 2008-10-14 by bhsohn VOIP개선
//	m_bVOIPSpeaking = FALSE;

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	m_pINFGameMainSysMsg = NULL;
	
	// 2008-11-13 by bhsohn 조이스틱 작업
	m_pINFOpJoystick = NULL;	
	// end 2008-11-13 by bhsohn 조이스틱 작업


//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_GruopImagemanager = NULL;																	  
//#endif
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pGameData = pGameData;		 
#endif

	m_nAddPosY = 0;    // 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
}

CINFGameMain::~CINFGameMain()
{
	FLOG( "~CINFGameMain()" );
	g_pGameMain = NULL;

//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_GruopImagemanager);															  
//#endif
	util::del(m_pUnitInfoBar) ;
	util::del(m_pInfWindow) ;
//	util::del(m_pParty);
	util::del(m_pCommunity);
	util::del(m_pWeaponInfo ) ;

	util::del(m_pTrade) ;
	util::del(m_pInven) ;
	util::del(m_pSystem) ;
	util::del(m_pCharacterInfo) ;

	util::del(m_pChat) ;
	util::del(m_pQuickSlot);
	util::del(m_pMiniMap);
	util::del(m_pIcon) ;
//	util::del(m_pShop) ;
	util::del(m_pUnitNameInfo);
	util::del(m_pQuest) ;
	util::del(m_pMission) ;
	util::del(m_pItemInfo) ;
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	util::del(m_pEquipItemInfo);	// 현재 장착한 아이템 툴팁
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

//	util::del(m_pNotice) ;
	util::del(m_pUnitState) ;
	util::del(m_pAttackDirection) ;
	util::del(m_pGameCountDown) ;
	util::del(m_pDamageView) ;
	util::del(m_pOtherCharInfo) ;
	util::del(m_pInfGameMainFaq) ;
	util::del(m_pMenuList) ;

	util::del(m_pFontToolTip) ;
	util::del(m_pFontTimeLimit) ;
	util::del(m_pFontDrawMent) ;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pGameData = NULL;
#else
	util::del(m_pGameData) ;
#endif
	util::del(m_pInfSkill);
	util::del(m_pMp3Player);
	
	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
	//util::del(m_pMissionInfo);
	util::del(m_pMissionMain);
	
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	util::del(m_pINFWorldRankWnd);
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	
	// 2013-02-20 by bhsohn 인게임 조합창 처리
	util::del(m_pINFItemMixWnd);
	// END 2013-02-20 by bhsohn 인게임 조합창 처리
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	util::del(m_pINFArmorCollectWnd);		
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	// 2007-08-28 by dgwoo 전진기지전 class추가.
	util::del(m_pOutPost);
	util::del(m_pChangeCharactor);
	util::del(m_pCouponWindow);

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	util::del(m_pINFGameMainSysMsg);

	// 2008-11-13 by bhsohn 조이스틱 작업
	util::del(m_pINFOpJoystick);	
	// end 2008-11-13 by bhsohn 조이스틱 작업

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	util::del(m_pINFSpeakeMsg);	
	
	//util::del(m_pCommunityVOIP);

//	util::del(m_pImgUnitLock) ;
	int i;
	for(i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		util::del( m_pImgButton[i] );															  
#else 
		for(int j=0;j<BUTTON_STATE_NUMBER;j++)
		{
			util::del(m_pImgButton[i][j]);
		}
#endif
	}
	util::del(m_pImgMission);
	util::del(m_pHelpDesk);
	for(i=0; i<3; i++)
	{
		util::del(m_pImgTextPopUp[i]);
	}

	util::del( m_pImgTextBg );
	
	// 2005-03-03 by jschoi
	util::del(m_pRequestEnableBack);
	util::del(m_pRequestEnableTriangle);
	util::del(m_pAltiMeterGage);
	util::del(m_pAltiMeterPin);
	
	for(i=0;i<SIZE_MAX_POS;i++)
	{
		if(m_pItemInfoWear[i] != NULL)	{
		util::del( m_pItemInfoWear[i]);
		}
		if(m_pITEMG[i] != NULL)	{
			util::del( m_pITEMG[i]);
		}
	}

	util::del(m_pCityBoard);

	m_stSelectItem.pSelectItem = NULL;
	m_stSelectItem.pSelectIcon = NULL;
	m_stSelectItem.bySelectType = 0;

	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	m_nMultiSelCnt = 0;	
	for(i = 0; i < MAX_MULTI_SEL;i++)
	{
		m_stMultiSelectItem[i].pSelectItem = NULL;
		m_stMultiSelectItem[i].pSelectIcon = NULL;
		m_stMultiSelectItem[i].bySelectType = 0;
	}
	// end 2007-03-02 by bhsohn 다중 선택 추가 보안

	util::del(m_pImgStack);

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	util::del(m_pFaqBtn);	 
	util::del(m_pCouponBtn);
	// end 2007-07-04 by bhsohn 오퍼레이터 추가
	// 2009. 10. 14 by jskim 프리스카 제거
	util::del(m_pTut_SelBtn);
	//end 2009. 10. 14 by jskim 프리스카 제거

	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	util::del(m_pMotherShipC);

	// 2015-05-01 by killburne
	util::del(m_pStrategicPointDisplay);

	// 2008. 12. 16 by ckPark 아이템 사용 지연
	m_listItemDelay.clear();
	// end 2008. 12. 16 by ckPark 아이템 사용 지연


	// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
	m_listSkillDelay.clear();
	// end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연

	// 2009. 08. 21 by ckPark 캐쉬아이템 선물하기에 블럭계정 체크
	memset( m_szCashGiftSendCharacterName, 0, sizeof( m_szCashGiftSendCharacterName ) );
	// end 2009. 08. 21 by ckPark 캐쉬아이템 선물하기에 블럭계정 체크

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	ResetToolTipEx();
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
}



HRESULT CINFGameMain::InitDeviceObjects()
{
	FLOG( "CINFGameMain::InitDeviceObjects()" );

	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "interface.tex");
	SetResourceFile(strPath);
#endif
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_GruopImagemanager = new CINFGroupManager( g_pD3dApp->m_pImageList, m_pGameData );
	m_GruopImagemanager->InitDeviceObjects();
//#endif
	
	m_pUnitInfoBar = new CINFGameMainUnitInfoBar(this) ;
	m_pUnitInfoBar->SetGameData(m_pGameData);
	m_pUnitInfoBar->InitDeviceObjects() ;

	m_pInfWindow = new CINFWindow(this) ;
	m_pInfWindow->SetGameData(m_pGameData);
	m_pInfWindow->InitDeviceObjects() ;

	m_pWeaponInfo = new CINFWeaponInfo(this) ;
	m_pWeaponInfo->SetGameData(m_pGameData);
	m_pWeaponInfo->InitDeviceObjects() ;

	m_pCommunity = new CINFCommunity(this) ;
	m_pCommunity->SetGameData(m_pGameData);
	m_pCommunity->InitDeviceObjects() ;

	m_pTrade = new CINFTrade(this) ;
	m_pTrade->SetGameData(m_pGameData);
	m_pTrade->InitDeviceObjects() ;

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	//m_pInven = new CINFInven(this) ;
	m_pInven = new CINFInvenExtend(this) ;
	
	m_pInven->SetGameData(m_pGameData);
	m_pInven->InitDeviceObjects() ;

	// 2008-03-19 by bhsohn Ep3옵션창
	//m_pSystem = new CINFSystem(this) ;
	m_pSystem = new CINFOptionSystem(this) ;
	m_pSystem->SetGameData(m_pGameData);
	m_pSystem->InitDeviceObjects() ;
	// end 2008-03-19 by bhsohn Ep3옵션창

	// 2008-09-22 by bhsohn EP3 캐릭터 창
	//m_pCharacterInfo = new CINFCharacterInfo(this) ;
	m_pCharacterInfo = new CINFCharacterInfoExtend(this) ;

	m_pCharacterInfo->SetGameData(m_pGameData);
	m_pCharacterInfo->InitDeviceObjects() ;

//	m_pShop = new CINFShop(this) ;
//	m_pShop->SetGameData(m_pGameData);
//	m_pShop->InitDeviceObjects() ;

	m_pMission = new CINFMission(this) ;
	m_pMission->SetGameData(m_pGameData);
	m_pMission->InitDeviceObjects() ;

	m_pUnitNameInfo = new CINFUnitNameInfo(this) ;
	m_pUnitNameInfo->SetGameData(m_pGameData);
	m_pUnitNameInfo->InitDeviceObjects() ;

	m_pIcon = new CINFIcon() ;
	m_pIcon->InitDeviceObjects() ;

	m_pFontToolTip = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,1024,32);		 //2008-12-19 dgwoo 툴팁 사이즈 늘림.
	m_pFontToolTip->InitDeviceObjects(g_pD3dDev);

	m_pFontTimeLimit = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontTimeLimit->InitDeviceObjects(g_pD3dDev);
	
	m_pFontDrawMent = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,512,32);
	m_pFontDrawMent->InitDeviceObjects(g_pD3dDev);

	int nWidth, nHeight;
	nWidth = nHeight = -1;		
	if(g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_POS))
	{
		float fWidth = (float)g_pD3dApp->GetBackBufferDesc().Width;
		float fHeight = (float)g_pD3dApp->GetBackBufferDesc().Height;
		
		stcuctRateRect stRateRect = g_pD3dApp->GetOptionInterfacePOS();
		stcuctRateRect stChatRect = g_pD3dApp->GetChatInterfacePOS();
		if((stRateRect.fGameWidth == fWidth)
			&&(stRateRect.fGameHeight == fHeight))
		{
			nWidth	= stChatRect.fRateWidth;
			nHeight	= stChatRect.fRateHeight;
		}
		else
		{				
			float fRateWidth = (fWidth/stRateRect.fGameWidth);
			float fRateHeight = (fHeight/stRateRect.fGameHeight);
			if(fRateWidth == 0 )
			{					
				fRateWidth = 1;
			}
			if(fRateHeight == 0)
			{					
				fRateHeight = 1;
			}
			nWidth	= (int)(stChatRect.fRateWidth*fRateWidth);
			nHeight	= (int)(stChatRect.fRateHeight*fRateHeight);	
		}				
	}

	m_pChat = new CINFGameMainChat(this, nWidth, nHeight);
	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장


	m_pChat->SetGameData(m_pGameData);
	m_pChat->InitDeviceObjects() ;

	m_pQuickSlot = new CINFGameMainQSlot(this) ;
	m_pQuickSlot->SetGameData(m_pGameData);
	m_pQuickSlot->InitDeviceObjects() ;

	m_pMiniMap = new CINFGameMainMiniMap(this) ;
	m_pMiniMap->SetGameData(m_pGameData);

	m_pMiniMap->InitDeviceObjects() ;

	m_pItemInfo = new CINFItemInfo(m_pParent);
	m_pItemInfo->SetGameData(m_pGameData);
	m_pItemInfo->InitDeviceObjects() ;
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	m_pEquipItemInfo = new CINFItemInfo(m_pParent);
	m_pEquipItemInfo->SetMyEquipItem(TRUE);
	m_pEquipItemInfo->SetGameData(m_pGameData);
	m_pEquipItemInfo->InitDeviceObjects() ;	
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

//	m_pNotice = new CINFNotice() ;
//	m_pNotice->InitDeviceObjects() ;

	m_pUnitState = new CINFUnitState(this) ;
	m_pUnitState->SetGameData(m_pGameData);
	m_pUnitState->InitDeviceObjects() ;

	m_pAttackDirection = new CINFAttackDirection(this) ;
	m_pAttackDirection->SetGameData(m_pGameData);
	m_pAttackDirection->InitDeviceObjects() ;

	DataHeader * pDataHeader = NULL;


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgButton[GAMEMAIN_BUTTON_INVEN] =		new CINFImageBtn;
	m_pImgButton[GAMEMAIN_BUTTON_INFO] =		new CINFImageBtn;
	m_pImgButton[GAMEMAIN_BUTTON_COMMUNITY] =	new CINFImageBtn;
	m_pImgButton[GAMEMAIN_BUTTON_TRADE] =		new CINFImageBtn;
	m_pImgButton[GAMEMAIN_BUTTON_SYSTEM] =		new CINFImageBtn;

// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
// 	m_pImgButton[GAMEMAIN_BUTTON_INVEN]->InitDeviceObjects( "mnbtn03" , "mnbtn00", "mnbtn00", "mnbtn00");
// 	m_pImgButton[GAMEMAIN_BUTTON_INFO]->InitDeviceObjects( "mnbtn13" , "mnbtn10", "mnbtn10", "mnbtn10" );
// 	m_pImgButton[GAMEMAIN_BUTTON_COMMUNITY]->InitDeviceObjects( "mnbtn23" , "mnbtn20", "mnbtn20", "mnbtn20" );
// 	m_pImgButton[GAMEMAIN_BUTTON_TRADE]->InitDeviceObjects( "mnbtn33" , "mnbtn30", "mnbtn30", "mnbtn30" );
// 	m_pImgButton[GAMEMAIN_BUTTON_SYSTEM]->InitDeviceObjects( "mnbtn43" , "mnbtn40", "mnbtn40", "mnbtn40" );
	m_pImgButton[GAMEMAIN_BUTTON_INVEN]->InitDeviceObjects( "mnbtn03" , "mnbtn00", "mnbtn00", "mnbtn00","STRTOOLTIP102" );
	m_pImgButton[GAMEMAIN_BUTTON_INFO]->InitDeviceObjects( "mnbtn13" , "mnbtn10", "mnbtn10", "mnbtn10","STRTOOLTIP103" );
	m_pImgButton[GAMEMAIN_BUTTON_COMMUNITY]->InitDeviceObjects( "mnbtn23" , "mnbtn20", "mnbtn20", "mnbtn20","STRTOOLTIP104" );
	m_pImgButton[GAMEMAIN_BUTTON_TRADE]->InitDeviceObjects( "mnbtn33" , "mnbtn30", "mnbtn30", "mnbtn30","STRTOOLTIP105" );
	m_pImgButton[GAMEMAIN_BUTTON_SYSTEM]->InitDeviceObjects( "mnbtn43" , "mnbtn40", "mnbtn40", "mnbtn40","STRTOOLTIP106" );
// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#endif

	int i;
	for(i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
	{

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgButton[i]->SetAnimation( SCALE_TYPE, OVER_TYPE, FALSE, 1.0f, 1.0f, 1.0f );
		m_pImgButton[i]->SetAnimation( SCALE_TYPE, CLICK_TYPE, FALSE, 0.0f, 1.0f, 1.0f );
		m_pImgButton[i]->SetAniScale(0.8f, 0.8f);	 
#else							
		for(int j=0;j<BUTTON_STATE_NUMBER;j++)
		{
			char buf[32];
			wsprintf(buf, "mnbtn%d%d", i,j);
			pDataHeader = m_pGameData->Find( buf );
			m_pImgButton[i][j] = new CINFImage;
			m_pImgButton[i][j]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
		}
#endif
	}
	// 2007-07-04 by bhsohn 오퍼레이터 추가

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_faq3");
		wsprintf(szDownBtn, "op_faq0");
		wsprintf(szSelBtn, "op_faq0");
		wsprintf(szDisBtn, "op_faq0");
		if(NULL == m_pFaqBtn)
		{
			m_pFaqBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pFaqBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		m_pFaqBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn, "STRTOOLTIP101");		
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		m_pFaqBtn->SetAnimation( SCALE_TYPE, OVER_TYPE, FALSE, 1.0f, 1.0f, 1.0f );
		m_pFaqBtn->SetAnimation( SCALE_TYPE, CLICK_TYPE, FALSE, 0.0f, 1.0f, 1.0f );
		m_pFaqBtn->SetAniScale(0.8f, 0.8f);
	}													   
#else	 
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "op_faq3");
		wsprintf(szDownBtn, "op_faq1");
		wsprintf(szSelBtn, "op_faq0");
		wsprintf(szDisBtn, "op_faq2");
		if(NULL == m_pFaqBtn)
		{
			m_pFaqBtn = new CINFImageBtn;
		}
		m_pFaqBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#endif
			  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pImgButtonBK = new CINFImageEx;
	pDataHeader = m_pGameData->Find( "mnbtnBG" );
	m_pImgButtonBK->InitDeviceObjects( pDataHeader );	
#endif
	// 2009. 10. 14 by jskim 프리스카 제거
	{
		char tulUpBtn[30], tulDownBtn[30], tulSelBtn[30], tulDisBtn[30];
		wsprintf(tulUpBtn, "Tut_sel3");
		wsprintf(tulDownBtn, "Tut_sel1");
		wsprintf(tulSelBtn, "Tut_sel0");
		wsprintf(tulDisBtn, "Tut_sel2");
		if(NULL == m_pTut_SelBtn)
		{
			m_pTut_SelBtn = new CINFImageBtn;
		}
		m_pTut_SelBtn->InitDeviceObjects(tulUpBtn, tulDownBtn, tulSelBtn, tulDisBtn);		
	}
	//end 2009. 10. 14 by jskim 프리스카 제거
	
	// end 2007-07-04 by bhsohn 오퍼레이터 추가
	{
		if(NULL == m_pCouponBtn)
		{
			m_pCouponBtn = new CINFImageBtn;
		}
		m_pCouponBtn->InitDeviceObjects("couponB");
		m_pCouponBtn->EnableBtn(FALSE);
		m_pCouponBtn->ShowWindow(FALSE);
	}
	

	char buf[32];
	for(i=0; i<3; i++)
	{
		wsprintf(buf, "boxtip%d",i);
		pDataHeader = m_pGameData->Find( buf );
		m_pImgTextPopUp[i] = new CINFImageEx;
		m_pImgTextPopUp[i]->InitDeviceObjects( pDataHeader );
	}


	wsprintf(buf, "boxtip1");
	pDataHeader = m_pGameData->Find( buf );

	if ( pDataHeader )
	{
		m_pImgTextBg = new CINFImageEx;
		m_pImgTextBg->InitDeviceObjects( pDataHeader );
	}

	pDataHeader = m_pGameData->Find( "mnbtn70" );
	m_pImgMission = new CINFImageEx;
	m_pImgMission->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("mnbtn71");
	m_pImgStack = new CINFImageEx;
	m_pImgStack->InitDeviceObjects( pDataHeader );

	m_pQuest = new CINFQuest(this);
	m_pQuest->SetGameData(m_pGameData);
	m_pQuest->InitDeviceObjects();
	
	m_pInfSkill = new CINFSkill();
	m_pInfSkill->SetGameData(m_pGameData);
	m_pInfSkill->InitDeviceObjects();

	m_pGameCountDown = new CINFGameCountDown();

	m_pDamageView = new CINFDamageView();
	m_pDamageView->SetGameData( m_pGameData );
	m_pDamageView->InitDeviceObjects();

	m_pMp3Player = new CINFMp3Player();
	m_pMp3Player->SetGameData( m_pGameData );
	m_pMp3Player->InitDeviceObjects();
	m_pMp3Player->FindMp3File();

	
	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//	m_pMissionInfo = new CINFMissionInfo();
//	m_pMissionInfo->SetGameData( m_pGameData );
//	m_pMissionInfo->InitDeviceObjects();
	{
		m_pMissionMain = new CINFMissionMain();
		m_pMissionMain->SetGameData( m_pGameData );
		m_pMissionMain->InitDeviceObjects();
	}
	
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	CWorldRankManager*	pWorldRankManager = g_pD3dApp->GetWorldRankManager();
	if(pWorldRankManager->GetUseWorldRanking())
	{
		if(NULL == m_pINFWorldRankWnd)
		{
			m_pINFWorldRankWnd = new CINFWorldRankWnd();		
			m_pINFWorldRankWnd->InitDeviceObjects();
		}
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템

	// 2013-02-20 by bhsohn 인게임 조합창 처리
#if defined(C_INGAME_MIX_ITEM)
	{
		if(NULL == m_pINFItemMixWnd)
		{
			m_pINFItemMixWnd = new CINFItemMixWnd(this);
			m_pINFItemMixWnd->SetGameData(m_pGameData);
			m_pINFItemMixWnd->InitDeviceObjects();
		}
	}	
#endif
	// END 2013-02-20 by bhsohn 인게임 조합창 처리

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	{
		if(NULL == m_pINFArmorCollectWnd)
		{
			m_pINFArmorCollectWnd = new CINFArmorCollectWnd(this);
			m_pINFArmorCollectWnd->SetGameData(m_pGameData);
			m_pINFArmorCollectWnd->InitDeviceObjects();
		}
	}
#endif
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템


	{// 2007-11-21 by dgwoo
		m_pChangeCharactor = new CINFChangeCharactor(this);
		m_pChangeCharactor->SetGameData(m_pGameData);
		m_pChangeCharactor->InitDeviceObjects();
	}
	{
		m_pCouponWindow = new CINFCouponWindow(this);
		m_pCouponWindow->SetGameData(m_pGameData);
		m_pCouponWindow->InitDeviceObjects();
	}
	// 2007-08-28 by dgwoo 전진기지전 class추가.
	{
		m_pOutPost = new CINFGameMainOutPost();
		m_pOutPost->SetGameData(m_pGameData);
		m_pOutPost->InitDeviceObjects();
	}


	// 2007-08-07 by bhsohn 스피커 아이템 추가
	{
		m_pINFSpeakeMsg = new CINFSpeakeMsg;
		m_pINFSpeakeMsg->SetGameData( m_pGameData );
		m_pINFSpeakeMsg->InitDeviceObjects();		
	}
	// end 2007-08-07 by bhsohn 스피커 아이템 추가
	
	m_pOtherCharInfo = new CINFOtherCharInfo(this);
	m_pOtherCharInfo->SetGameData( m_pGameData );
	m_pOtherCharInfo->InitDeviceObjects();

	m_pInfGameMainFaq = new CINFGameMainFAQ();
	m_pInfGameMainFaq->SetGameData( m_pGameData );
	m_pInfGameMainFaq->InitDeviceObjects();
	
	m_pMenuList = new CINFMenuList();
	m_pMenuList->SetGameData( m_pGameData );
	m_pMenuList->InitDeviceObjects();
	
	// 2005-03-03 by jschoi
	m_pRequestEnableBack = new CINFImageEx;
	pDataHeader = FindResource("rq_enb");
	m_pRequestEnableBack->InitDeviceObjects( pDataHeader );

	m_pRequestEnableTriangle = new CINFImageEx;
	pDataHeader = FindResource("rq_ent");
	m_pRequestEnableTriangle->InitDeviceObjects( pDataHeader);

	m_pAltiMeterGage = new CINFImageEx;
	pDataHeader = FindResource("altig");
	m_pAltiMeterGage->InitDeviceObjects( pDataHeader );

	m_pAltiMeterPin = new CINFImageEx;
	pDataHeader = FindResource("altip");
	m_pAltiMeterPin->InitDeviceObjects(pDataHeader);
	
	// 2005-10-10 by ispark
	//m_pCommunityVOIP = new CINFCommunityVOIP();
	//m_pCommunityVOIP->SetGameData(m_pGameData);
	//m_pCommunityVOIP->InitDeviceObjects();

	// 2006-04-19 by ispark
	m_pCityBoard = new CINFCityBoard;
	if(g_pGround)
		m_pCityBoard->InitDeviceObjects(g_pGround->m_nMapNumber);	

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	InitSecurityPassMode();

	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	{
		m_pMotherShipC		= new CINFMotherShipManager(m_pGameData);
		//m_pMotherShipC->SetGameData(m_pGameData);
		m_pMotherShipC->InitDeviceObjects();
	}
	// end 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	// 2015-05-01 by killburne
	{
		m_pStrategicPointDisplay = new CINFStrategicPointManager();
		m_pStrategicPointDisplay->SetGameData(m_pGameData);
		m_pStrategicPointDisplay->InitDeviceObjects();
	}
	{
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(NULL == m_pINFGameMainSysMsg)
		{
			m_pINFGameMainSysMsg = new CINFGameMainSysMsg(this);
			m_pINFGameMainSysMsg->SetGameData(m_pGameData);
			m_pINFGameMainSysMsg->InitDeviceObjects();
		}
	}

	// 2008-11-13 by bhsohn 조이스틱 작업
	{
		if(NULL == m_pINFOpJoystick)
		{
			m_pINFOpJoystick = new CINFOpJoystick(this);
			m_pINFOpJoystick->SetGameData(m_pGameData);
			m_pINFOpJoystick->InitDeviceObjects();
		}
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업


	// 2008-06-20 by bhsohn EP3 옵션관련 처리	
	{
		// 인터페이스 숨기기
		BOOL bShowInter = !g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_HIDE);

		g_pInterface->SetShowInterface(bShowInter);
	}
	// end 2008-06-20 by bhsohn EP3 옵션관련 처리

	ShowSysMsgWnd(TRUE);//2012-03-15 by jhahn 서버 시간 보여주기

	return S_OK ;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void InitWarp()
/// \brief		워프 통과시 클래스에서 해줘야할 것들.
/// \author		dgwoo
/// \date		2007-02-22 ~ 2007-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::InitWarp()
{
	m_pMiniMap->InitMapSet();
}

void CINFGameMain::InitHelpSystem(BOOL bShow)
{
	m_bHelpDeskFlag = bShow;
	if(bShow)
	{
		if(m_pHelpDesk)
		{
			m_pHelpDesk->InvalidateDeviceObjects();
			m_pHelpDesk->DeleteDeviceObjects();
			util::del(m_pHelpDesk);
		}
		m_pHelpDesk = new CINFGameHelpDesk(this);
		m_pHelpDesk->SetGameData(m_pGameData);
	DBGOUT("---------- Help InitDeviceObjects() ----------\n");
		m_pHelpDesk->InitDeviceObjects();
	DBGOUT("---------- Help RestoreDeviceObjects() ----------\n");
		m_pHelpDesk->RestoreDeviceObjects();
	DBGOUT("---------- Help StartHelpDesk() ----------\n");
		m_pHelpDesk->StartHelpDesk();

		// 2007-01-26 by bhsohn 신규유저 미션 인터페이스	
		// 신규 유저 도움말을 띠운다.		
		UpdateLV1NoviceUserHelper();		
	}
	else
	{
		if(m_pHelpDesk)
		{
			m_pHelpDesk->InvalidateDeviceObjects();
			m_pHelpDesk->DeleteDeviceObjects();
			util::del(m_pHelpDesk);
		}
	}
}
HRESULT CINFGameMain::RestoreDeviceObjects()
{
	FLOG( "CINFGameMain::RestoreDeviceObjects()" );
	if(!m_bRestored)
	{
		if(m_pUnitInfoBar )
			m_pUnitInfoBar->RestoreDeviceObjects() ;
		
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(m_pINFGameMainSysMsg)
		{
			m_pINFGameMainSysMsg->RestoreDeviceObjects() ;
		}
		// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

		// 2008-11-13 by bhsohn 조이스틱 작업
		if(m_pINFOpJoystick)
		{
			m_pINFOpJoystick->RestoreDeviceObjects() ;
		}
		// end 2008-11-13 by bhsohn 조이스틱 작업
	
		if(m_pInfWindow )
			m_pInfWindow->RestoreDeviceObjects() ;
		if(m_pWeaponInfo )
			m_pWeaponInfo->RestoreDeviceObjects() ;

		if(m_pCommunity)
			m_pCommunity->RestoreDeviceObjects() ;
		if(m_pTrade && !m_pTrade->m_bRestored )
			m_pTrade->RestoreDeviceObjects() ;
		if(m_pInven && !m_pInven->m_bRestored )
			m_pInven->RestoreDeviceObjects() ;
		if(m_pSystem && !m_pSystem->m_bRestored )
			m_pSystem->RestoreDeviceObjects() ;
		if(m_pCharacterInfo && !m_pCharacterInfo->m_bRestored )
			m_pCharacterInfo->RestoreDeviceObjects() ;
//		if(m_pShop && !m_pShop->m_bRestored )
//			m_pShop->RestoreDeviceObjects() ;

		if(m_pMission )
			m_pMission->RestoreDeviceObjects() ;
		if(m_pIcon )
			m_pIcon->RestoreDeviceObjects() ;
		if(m_pUnitNameInfo )
			m_pUnitNameInfo->RestoreDeviceObjects() ;
		if(m_pChat )
			m_pChat->RestoreDeviceObjects() ;
		if(m_pQuickSlot )
			m_pQuickSlot->RestoreDeviceObjects() ;
		if(m_pMiniMap )
			m_pMiniMap->RestoreDeviceObjects() ;
		if(m_pItemInfo )
			m_pItemInfo->RestoreDeviceObjects() ;
		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
		if(m_pEquipItemInfo)
		{			
			m_pEquipItemInfo->RestoreDeviceObjects();
		}
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
//		if(m_pNotice )
//			m_pNotice->RestoreDeviceObjects() ;
		if(m_pUnitState )
			m_pUnitState->RestoreDeviceObjects() ;
		if(m_pAttackDirection )
			m_pAttackDirection->RestoreDeviceObjects() ;
		if(m_pFontToolTip)
			m_pFontToolTip->RestoreDeviceObjects();
		if(m_pFontTimeLimit)
		    m_pFontTimeLimit->RestoreDeviceObjects();
		if(m_pFontDrawMent)
		    m_pFontDrawMent->RestoreDeviceObjects();
		if(m_bShowHelp)
		{
			for(int i=0;i<HELP_NUM;i++)
			{
				if(m_pHelp[i])
					m_pHelp[i]->RestoreDeviceObjects();
			}
		}
		if(m_pSelectIcon)
			m_pSelectIcon->RestoreDeviceObjects();
		for(int i=0; i<3; i++)
		{
			if(m_pImgTextPopUp[i])
				m_pImgTextPopUp[i]->RestoreDeviceObjects();
		}

		if ( m_pImgTextBg )
			m_pImgTextBg->RestoreDeviceObjects();


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		if(m_pImgButtonBK)
			m_pImgButtonBK->RestoreDeviceObjects();												  
#endif

//		if(m_pImgUnitLock)
//			m_pImgUnitLock->RestoreDeviceObjects();
		for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pImgButton[i]->RestoreDeviceObjects();
			m_pImgButton[i]->SetBtnPosition(GAMEMAIN_BUTTON_START_X + GAMEMAIN_BUTTON_INTERVAL * i, GAMEMAIN_BUTTON_START_Y);
			m_pImgButton[i]->SetLineUp( CENTER_BOTTOM_LINEUP );									  
#else			
			for(int j=0;j<BUTTON_STATE_NUMBER;j++)
			{
				m_pImgButton[i][j]->RestoreDeviceObjects();
			}
#endif
		}

		// 2007-07-04 by bhsohn 오퍼레이터 추가
		{				
			m_pFaqBtn->RestoreDeviceObjects();		
			m_pFaqBtn->SetBtnPosition(FAQ_BTN_POS_X, FAQ_BTN_POS_Y);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pFaqBtn->SetLineUp( CENTER_BOTTOM_LINEUP );										  
#endif
		}
		// 2009. 10. 14 by jskim 프리스카 제거
		m_pTut_SelBtn->RestoreDeviceObjects();
		m_pTut_SelBtn->SetBtnPosition(TUTORIAL_BUTTON_X, TUTORIAL_BUTTON_Y);
		//end 2009. 10. 14 by jskim 프리스카 제거
		// end 2007-07-04 by bhsohn 오퍼레이터 추가
		{
			m_pCouponBtn->RestoreDeviceObjects();
			m_pCouponBtn->SetBtnPosition(COUPON_BTN_POS_X,COUPON_BTN_POS_Y);
		}

		m_pImgMission->RestoreDeviceObjects();
		m_pImgStack->RestoreDeviceObjects();
		if(m_pInfSkill)
			m_pInfSkill->RestoreDeviceObjects();
		if(m_pDamageView)
			m_pDamageView->RestoreDeviceObjects();
		if(m_pOtherCharInfo)
			m_pOtherCharInfo->RestoreDeviceObjects();
		if(m_pInfGameMainFaq)
			m_pInfGameMainFaq->RestoreDeviceObjects();
		if(m_pMenuList)
			m_pMenuList->RestoreDeviceObjects();

		// 2005-03-03 by jschoi
		m_pRequestEnableBack->RestoreDeviceObjects();
		m_pRequestEnableTriangle->RestoreDeviceObjects();

		// 2006-04-11 by ispark
		if(m_pCityBoard)
			m_pCityBoard->RestoreDeviceObjects();

		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
		m_pMotherShipC->RestoreDeviceObjects();

		// 2015-05-01 by killburne
		m_pStrategicPointDisplay->RestoreDeviceObjects();

		// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
		ResetToolTipEx();
		// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

		m_bRestored = TRUE;
	}

	if(m_pHelpDesk )
		m_pHelpDesk->RestoreDeviceObjects();
	if(m_pQuest)
			m_pQuest->RestoreDeviceObjects();
	if(m_pMp3Player)
		m_pMp3Player->RestoreDeviceObjects();
	
	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//	if(m_pMissionInfo)
//		m_pMissionInfo->RestoreDeviceObjects();
	if(m_pMissionMain)
		m_pMissionMain->RestoreDeviceObjects();
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pINFWorldRankWnd)
	{	
		m_pINFWorldRankWnd->RestoreDeviceObjects();
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	
	// 2013-02-20 by bhsohn 인게임 조합창 처리
	if(m_pINFItemMixWnd)
	{		
		m_pINFItemMixWnd->RestoreDeviceObjects();
	}	
	// END 2013-02-20 by bhsohn 인게임 조합창 처리

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pINFArmorCollectWnd)
	{		
		m_pINFArmorCollectWnd->RestoreDeviceObjects();
	}	
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	// 2007-08-28 by dgwoo 전진기지전 class추가.
	if(m_pChangeCharactor)
		m_pChangeCharactor->RestoreDeviceObjects();
	if(m_pCouponWindow)
		m_pCouponWindow->RestoreDeviceObjects();
	if(m_pOutPost)
		m_pOutPost->RestoreDeviceObjects();

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	if(m_pINFSpeakeMsg)
	{
		m_pINFSpeakeMsg->RestoreDeviceObjects();		 
	}
	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	if(m_pINFSecuMain)
	{
		m_pINFSecuMain->RestoreDeviceObjects();		
	}
	// end 2007-09-12 by bhsohn 2차 암호 시스템 구현

	//if(m_pCommunityVOIP) m_pCommunityVOIP->RestoreDeviceObjects();

	m_pAltiMeterGage->RestoreDeviceObjects();
	m_pAltiMeterPin->RestoreDeviceObjects();
	
	m_ptGageBack = m_pAltiMeterGage->GetImgSize();
	m_ptGagePin = m_pAltiMeterPin->GetImgSize();
	
	return S_OK ;
}

HRESULT CINFGameMain::DeleteDeviceObjects()
{
	FLOG( "CINFGameMain::DeleteDeviceObjects()" );
	if(m_pUnitInfoBar ) {
		m_pUnitInfoBar->DeleteDeviceObjects() ;
		util::del(m_pUnitInfoBar ) ; }

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	if(m_pINFGameMainSysMsg)
	{
		m_pINFGameMainSysMsg->DeleteDeviceObjects() ;
		util::del(m_pINFGameMainSysMsg) ;
	}
	// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

	// 2008-11-13 by bhsohn 조이스틱 작업
	if(m_pINFOpJoystick)
	{
		m_pINFOpJoystick->DeleteDeviceObjects() ;
		util::del(m_pINFOpJoystick) ;
	}
	// end 2008-11-13 by bhsohn 조이스틱 작업

	if(m_pInfWindow ) {
		m_pInfWindow->DeleteDeviceObjects() ;
		util::del(m_pInfWindow ) ; }
//	if(m_pParty ) {
//		m_pParty->DeleteDeviceObjects() ;
//		util::del(m_pParty); }
	if(m_pCommunity ) {
		m_pCommunity->DeleteDeviceObjects() ;
		util::del(m_pCommunity); }

	if(m_pWeaponInfo ) {
		m_pWeaponInfo->DeleteDeviceObjects() ;
		util::del(m_pWeaponInfo ) ; }

	if(m_pTrade ) {
		m_pTrade->DeleteDeviceObjects() ;
		util::del(m_pTrade ) ; }

	if(m_pInven ) {
		m_pInven->DeleteDeviceObjects() ;
		util::del(m_pInven ) ; }

	if(m_pSystem ) {
		m_pSystem->DeleteDeviceObjects() ;
		util::del(m_pSystem ) ; }

	if(m_pCharacterInfo ) {
		m_pCharacterInfo->DeleteDeviceObjects() ;
		util::del(m_pCharacterInfo ) ; }

	if(m_pChat ) {
		m_pChat->DeleteDeviceObjects() ;
		util::del(m_pChat ) ; }

	if(m_pQuickSlot ) {
		m_pQuickSlot->DeleteDeviceObjects() ;
		util::del(m_pQuickSlot ) ; }

	if(m_pMiniMap ) {
		m_pMiniMap->DeleteDeviceObjects() ;
		util::del(m_pMiniMap ) ; }
	if(m_pItemInfo ) {
		m_pItemInfo->DeleteDeviceObjects() ;
		util::del(m_pItemInfo ) ; }
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	if(m_pEquipItemInfo)
	{			
		m_pEquipItemInfo->DeleteDeviceObjects() ;
		util::del(m_pEquipItemInfo) ;		
	}
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

//	if(m_pNotice ) {
//		m_pNotice->DeleteDeviceObjects() ;
//		util::del(m_pNotice ) ; }

	if(m_pUnitState ) {
		m_pUnitState->DeleteDeviceObjects() ;
		util::del(m_pUnitState ) ; }

	if(m_pAttackDirection ) {
		m_pAttackDirection->DeleteDeviceObjects() ;
		util::del(m_pAttackDirection ) ; }

//	if(m_pShop ) {
//		m_pShop->DeleteDeviceObjects() ;
//		util::del(m_pShop ) ; }
	if(m_pQuest )
		m_pQuest->DeleteDeviceObjects() ;
	util::del(m_pQuest ) ;
	if(m_pMission ) {
		m_pMission->DeleteDeviceObjects() ;
		util::del(m_pMission ) ; }

	if(m_pUnitNameInfo ) {
		m_pUnitNameInfo->DeleteDeviceObjects() ;
		util::del(m_pUnitNameInfo ) ; }

	if(m_pMp3Player) {
		m_pMp3Player->DeleteDeviceObjects();
		util::del(m_pMp3Player);	}

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//	if(m_pMissionInfo) {
//		m_pMissionInfo->DeleteDeviceObjects();
//		util::del(m_pMissionInfo);}
	if(m_pMissionMain) {
		m_pMissionMain->DeleteDeviceObjects();
		util::del(m_pMissionMain);}	
	// 2009-02-13 by bhsohn 월드 랭킹 시스템
	if(m_pINFWorldRankWnd)
	{	
		m_pINFWorldRankWnd->DeleteDeviceObjects();
		util::del(m_pINFWorldRankWnd);
	}
	// end 2009-02-13 by bhsohn 월드 랭킹 시스템
	// 2013-02-20 by bhsohn 인게임 조합창 처리
	if(m_pINFItemMixWnd)
	{			
		m_pINFItemMixWnd->DeleteDeviceObjects();
		util::del(m_pINFItemMixWnd);
	}	
	// END 2013-02-20 by bhsohn 인게임 조합창 처리
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pINFArmorCollectWnd)
	{		
		m_pINFArmorCollectWnd->DeleteDeviceObjects();
		util::del(m_pINFArmorCollectWnd);
	}	
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	if(m_pChangeCharactor)
	{
		m_pChangeCharactor->DeleteDeviceObjects();
		util::del(m_pChangeCharactor);
	}
	if(m_pCouponWindow)
	{
		m_pCouponWindow->DeleteDeviceObjects();
		util::del(m_pCouponWindow);
	}
	// 2007-08-28 by dgwoo 전진기지전 class추가.
	if(m_pOutPost)
	{
		m_pOutPost->DeleteDeviceObjects();
		util::del(m_pOutPost);
	}


	// 2007-08-07 by bhsohn 스피커 아이템 추가
	if(m_pINFSpeakeMsg) {
		m_pINFSpeakeMsg->DeleteDeviceObjects();
		util::del(m_pINFSpeakeMsg);}	

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	if(m_pINFSecuMain)
	{
		m_pINFSecuMain->DeleteDeviceObjects();
		util::del(m_pINFSecuMain);
	}
	// end 2007-09-12 by bhsohn 2차 암호 시스템 구현
	

	if(m_pFontToolTip ) {
		m_pFontToolTip->DeleteDeviceObjects() ;
		util::del(m_pFontToolTip ) ; }

	if(m_pFontTimeLimit ) {
		m_pFontTimeLimit->DeleteDeviceObjects() ;
		util::del(m_pFontTimeLimit ) ; }
	
	if(m_pFontDrawMent ) {
		m_pFontDrawMent->DeleteDeviceObjects() ;
		util::del(m_pFontDrawMent ) ; }

	if(m_pIcon ) {
		m_pIcon->DeleteDeviceObjects() ;
		util::del(m_pIcon ) ; }
	if(m_bShowHelp)
	{
		for(int i=0;i<HELP_NUM;i++)
		{
			if(m_pHelp[i])
			{
				m_pHelp[i]->DeleteDeviceObjects();
				util::del(m_pHelp[i] );
			}
		}
	}
	if(m_pSelectIcon)
	{
		m_pSelectIcon->DeleteDeviceObjects();
		util::del(m_pSelectIcon);
	}
	for(int i=0; i<3; i++)
	{

		if ( m_pImgTextPopUp[i] )
			m_pImgTextPopUp[i]->DeleteDeviceObjects();

		util::del(m_pImgTextPopUp[i]);
	}

	if ( m_pImgTextBg )
		m_pImgTextBg->DeleteDeviceObjects();

	util::del( m_pImgTextBg );


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( m_pImgButtonBK )
	{
		m_pImgButtonBK->DeleteDeviceObjects();
		util::del(m_pImgButtonBK);
	}																							  
#endif

//	if(m_pImgUnitLock ) {
//		m_pImgUnitLock->DeleteDeviceObjects() ;
//		util::del(m_pImgUnitLock ); }
	for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pImgButton[i]->DeleteDeviceObjects();
		util::del(m_pImgButton[i]);															  
#else			 
		for(int j=0;j<BUTTON_STATE_NUMBER;j++)
		{
			m_pImgButton[i][j]->DeleteDeviceObjects();
			util::del(m_pImgButton[i][j]); 
		}
#endif
	}

	// 2007-07-04 by bhsohn 오퍼레이터 추가
	if(m_pFaqBtn)
	{
		m_pFaqBtn->DeleteDeviceObjects();	
		util::del(m_pFaqBtn);
	}
	// end 2007-07-04 by bhsohn 오퍼레이터 추가
	// 2009. 10. 14 by jskim 프리스카 제거
	if(m_pTut_SelBtn)
	{
		m_pTut_SelBtn->DeleteDeviceObjects();
		util::del(m_pTut_SelBtn);
	}
	//end 2009. 10. 14 by jskim 프리스카 제거
	if(m_pCouponBtn)
	{
		m_pCouponBtn->DeleteDeviceObjects();
		util::del(m_pCouponBtn);
	}


	m_pImgMission->DeleteDeviceObjects();
	util::del(m_pImgMission); 
	m_pImgStack->DeleteDeviceObjects();
	util::del(m_pImgStack);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pGameData = NULL;
#else
	util::del(m_pGameData) ;
#endif


	if(m_pHelpDesk) 
	{
		m_pHelpDesk->DeleteDeviceObjects() ;
		util::del(m_pHelpDesk) ; 
	}
	
	if(m_pInfSkill)
	{
		m_pInfSkill->DeleteDeviceObjects();
		util::del(m_pInfSkill);
	}
	if(m_pDamageView)
	{
		m_pDamageView->DeleteDeviceObjects();
		util::del(m_pDamageView);
	}
	if(m_pOtherCharInfo)
	{
		m_pOtherCharInfo->DeleteDeviceObjects();
		util::del(m_pOtherCharInfo);
	}
	if(m_pInfGameMainFaq)
	{
		m_pInfGameMainFaq->DeleteDeviceObjects();
		util::del(m_pInfGameMainFaq);
	}
	if(m_pMenuList)
	{
		m_pMenuList->DeleteDeviceObjects();
		util::del(m_pMenuList);
	}

	// 2005-03-03 by jschoi
	m_pRequestEnableBack->DeleteDeviceObjects();
	m_pRequestEnableTriangle->DeleteDeviceObjects();
	util::del(m_pRequestEnableBack);
	util::del(m_pRequestEnableTriangle);

	m_pAltiMeterGage->DeleteDeviceObjects();
	m_pAltiMeterPin->DeleteDeviceObjects();
	util::del(m_pAltiMeterGage);
	util::del(m_pAltiMeterPin);

	//if(m_pCommunityVOIP)
	//{
	//	m_pCommunityVOIP->DeleteDeviceObjects();
	//	util::del(m_pCommunityVOIP);
	//}

	// 2006-04-11 by ispark
	if(m_pCityBoard)
		m_pCityBoard->DeleteDeviceObjects();
	util::del(m_pCityBoard);

	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	if(m_pMotherShipC)
	{
		m_pMotherShipC->DeleteDeviceObjects();
		util::del(m_pMotherShipC);
	}
	if (m_pStrategicPointDisplay)
	{
		m_pStrategicPointDisplay->DeleteDeviceObjects();
		util::del(m_pStrategicPointDisplay);
	}
	// end 2008-04-04 by bhsohn 모선전 관리 클래스 추가

	// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
	ResetToolTipEx();
	// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

	return S_OK ;
}


HRESULT CINFGameMain::InvalidateDeviceObjects()
{
	FLOG( "CINFGameMain::InvalidateDeviceObjects()" );
	if(m_bRestored)
	{
		if(m_pUnitInfoBar )
			m_pUnitInfoBar->InvalidateDeviceObjects() ;
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(m_pINFGameMainSysMsg)
		{
			m_pINFGameMainSysMsg->InvalidateDeviceObjects() ;			
		}
		// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

		// 2008-11-13 by bhsohn 조이스틱 작업
		if(m_pINFOpJoystick)
		{
			m_pINFOpJoystick->InvalidateDeviceObjects() ;			
		}
		// end 2008-11-13 by bhsohn 조이스틱 작업
	
		if(m_pInfWindow )
			m_pInfWindow->InvalidateDeviceObjects() ;
		if(m_pWeaponInfo )
			m_pWeaponInfo->InvalidateDeviceObjects() ;


//		if(m_pParty && m_pParty->m_bRestored )
//			m_pParty->InvalidateDeviceObjects() ;
		if(m_pCommunity )
			m_pCommunity->InvalidateDeviceObjects() ;
		if(m_pTrade && m_pTrade->m_bRestored)
			m_pTrade->InvalidateDeviceObjects() ;
		if(m_pInven && m_pInven->m_bRestored )
			m_pInven->InvalidateDeviceObjects() ;
		if(m_pSystem && m_pSystem->m_bRestored )
			m_pSystem->InvalidateDeviceObjects() ;
		if(m_pCharacterInfo && m_pCharacterInfo->m_bRestored )
			m_pCharacterInfo->InvalidateDeviceObjects() ;
//		if(m_pShop && m_pShop->m_bRestored )
//			m_pShop->InvalidateDeviceObjects() ;

		if(m_pChat )
			m_pChat->InvalidateDeviceObjects() ;
		if(m_pQuickSlot )
			m_pQuickSlot->InvalidateDeviceObjects() ;
		if(m_pMiniMap )
			m_pMiniMap->InvalidateDeviceObjects() ;
		if(m_pUnitNameInfo )
			m_pUnitNameInfo->InvalidateDeviceObjects() ;
		if(m_pQuest )
			m_pQuest->InvalidateDeviceObjects() ;
		if(m_pMp3Player)
			m_pMp3Player->InvalidateDeviceObjects();
		
		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//		if(m_pMissionInfo)
//			m_pMissionInfo->InvalidateDeviceObjects();
		if(m_pMissionMain)
			m_pMissionMain->InvalidateDeviceObjects();
		// 2009-02-13 by bhsohn 월드 랭킹 시스템
		if(m_pINFWorldRankWnd)
		{	
			m_pINFWorldRankWnd->InvalidateDeviceObjects();			
		}
		// end 2009-02-13 by bhsohn 월드 랭킹 시스템
		// 2013-02-20 by bhsohn 인게임 조합창 처리
		if(m_pINFItemMixWnd)
		{			
			m_pINFItemMixWnd->InvalidateDeviceObjects();						
		}	
		// END 2013-02-20 by bhsohn 인게임 조합창 처리
		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
		if(m_pINFArmorCollectWnd)
		{		
			m_pINFArmorCollectWnd->InvalidateDeviceObjects();			
		}	
		// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

		if(m_pChangeCharactor)
			m_pChangeCharactor->InvalidateDeviceObjects();
		if(m_pCouponWindow)
			m_pCouponWindow->InvalidateDeviceObjects();
		// 2007-08-28 by dgwoo 전진기지전 class추가.
		if(m_pOutPost)
		{
			m_pOutPost->InvalidateDeviceObjects();
		}
		// 2007-08-07 by bhsohn 스피커 아이템 추가
		if(m_pINFSpeakeMsg) 
		{
			m_pINFSpeakeMsg->InvalidateDeviceObjects();
		}
		// end 2007-08-07 by bhsohn 스피커 아이템 추가

		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
		if(m_pINFSecuMain)
		{
			m_pINFSecuMain->InvalidateDeviceObjects();
		}
		// end 2007-09-12 by bhsohn 2차 암호 시스템 구현
		

		if(m_pMission )
			m_pMission->InvalidateDeviceObjects() ;
		if(m_pIcon )
			m_pIcon->InvalidateDeviceObjects() ;
		if(m_pFontTimeLimit )
			m_pFontTimeLimit->InvalidateDeviceObjects() ;
		if(m_pFontDrawMent )
			m_pFontDrawMent->InvalidateDeviceObjects() ;
		if(m_pFontToolTip )
			m_pFontToolTip->InvalidateDeviceObjects() ;
		if(m_pItemInfo && m_pItemInfo->m_bRestored )
			m_pItemInfo->InvalidateDeviceObjects() ;
		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
		if(m_pEquipItemInfo&& m_pEquipItemInfo->m_bRestored )
		{			
			m_pEquipItemInfo->InvalidateDeviceObjects();
		}
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

//		if(m_pNotice  )
//			m_pNotice->InvalidateDeviceObjects() ;
		if(m_pUnitState  )
			m_pUnitState->InvalidateDeviceObjects() ;
		if(m_pAttackDirection  )
			m_pAttackDirection->InvalidateDeviceObjects() ;

		if(m_bShowHelp)
		{
			for(int i=0;i<HELP_NUM;i++)
			{
				if(m_pHelp[i])
					m_pHelp[i]->InvalidateDeviceObjects();
			}
		}
		if(m_pSelectIcon)
			m_pSelectIcon->InvalidateDeviceObjects();
		for(int i=0; i<3; i++)
		{
			if (m_pImgTextPopUp[i] )
				m_pImgTextPopUp[i]->InvalidateDeviceObjects();
		}
		
		if ( m_pImgTextBg )
			m_pImgTextBg->InvalidateDeviceObjects();


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		if( m_pImgButtonBK )
		{
			m_pImgButtonBK->InvalidateDeviceObjects();
		}											  
#endif

		for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
		{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pImgButton[i]->InvalidateDeviceObjects();
#else 
			for(int j=0;j<BUTTON_STATE_NUMBER;j++)
			{
				m_pImgButton[i][j]->InvalidateDeviceObjects();
			}
#endif																								  

		}

		// 2007-07-04 by bhsohn 오퍼레이터 추가
		if(m_pFaqBtn)
		{
			m_pFaqBtn->InvalidateDeviceObjects();		
		}	
		// end 2007-07-04 by bhsohn 오퍼레이터 추가
		// 2009. 10. 14 by jskim 프리스카 제거
		if(m_pTut_SelBtn)
		{
			m_pTut_SelBtn->InvalidateDeviceObjects();
		}
		//end 2009. 10. 14 by jskim 프리스카 제거
		if(m_pCouponBtn)
		{
			m_pCouponBtn->InvalidateDeviceObjects();
		}
		

		m_pImgMission->InvalidateDeviceObjects();
		m_pImgStack->InvalidateDeviceObjects();

		if(m_pHelpDesk )
		{
			m_pHelpDesk->InvalidateDeviceObjects();
		}
		
		if(m_pInfSkill)
			m_pInfSkill->InvalidateDeviceObjects();
		if(m_pDamageView)
			m_pDamageView->InvalidateDeviceObjects();
		if(m_pOtherCharInfo)
			m_pOtherCharInfo->InvalidateDeviceObjects();
		if(m_pInfGameMainFaq)
			m_pInfGameMainFaq->InvalidateDeviceObjects();
		if(m_pMenuList)
			m_pMenuList->InvalidateDeviceObjects();

		// 2005-03-03 by jschoi
		m_pRequestEnableBack->InvalidateDeviceObjects();
		m_pRequestEnableTriangle->InvalidateDeviceObjects();

		m_pAltiMeterGage->InvalidateDeviceObjects();
		m_pAltiMeterPin->InvalidateDeviceObjects();

		// 2006-04-19 by ispark
		if(m_pCityBoard)
			m_pCityBoard->InvalidateDeviceObjects();

		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
		m_pMotherShipC->InvalidateDeviceObjects();

		// 2015-05-01 by killburne
		m_pStrategicPointDisplay->InvalidateDeviceObjects();

		// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
		ResetToolTipEx();
		// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.

		m_bRestored = FALSE;
	}
	return S_OK ;
}

BOOL CINFGameMain::ChangeLeftWindowMode()
{
	FLOG( "CINFGameMain::ChangeLeftWindowMode()" );
	switch(m_nLeftWindowState)
	{
	case LEFT_WEAPON_WINDOW_INIT:// 무기 정보 보여주기(완료)
		{
			m_pWeaponInfo->m_nLeftWeaponInfoPosX = 0;
			m_nLeftWindowScaleSize = SIZE_WEAPON_Y;
			m_pInfWindow->m_bLeftWindow = FALSE ;
			m_bLeftWindowModeChage = FALSE;
			m_nLeftWindowState = LEFT_WEAPON_WINDOW_END;
			return FALSE;
		}
		break;
	case LEFT_WEAPON_WINDOW_HIDING:// 무기 정보 줄이기
		{
			m_pWeaponInfo->m_nLeftWeaponInfoPosX -= INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime();
			if(m_pWeaponInfo->m_nLeftWeaponInfoPosX<-100)
			{
				//윈도우 보여주기
				m_pInfWindow->m_bLeftWindow = TRUE ;
				m_pWeaponInfo->m_nLeftWeaponInfoPosX = -100 ;
				m_nLeftWindowState = LEFT_WINDOW_SHOWING ;
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);
			}
		}
		break;
	case LEFT_WINDOW_HIDING:// 윈도우 창 줄이기
		{
			m_nLeftWindowScaleSize -= INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime() ;
			
			if(m_nLeftWindowScaleSize < SIZE_WEAPON_Y)
			{
				if(m_nLeftWindowInfo == LEFT_WINDOW_WEAPON)
				{
					// 무기 창 늘리기
					m_nLeftWindowState = LEFT_WEAPON_WINDOW_SHOWING;
					m_pInfWindow->m_bLeftWindow = FALSE ;
					m_nLeftWindowScaleSize = SIZE_WEAPON_Y ;
				}
				else
				{
					//윈도우 보여주기
					m_pInfWindow->m_bLeftWindow = TRUE ;
					m_pWeaponInfo->m_nLeftWeaponInfoPosX = -100 ;
					m_nLeftWindowState = LEFT_WINDOW_SHOWING ;
				}
			}
		}
		break;
	case LEFT_WEAPON_WINDOW_SHOWING:// 무기 창 늘리기
		{
			m_pWeaponInfo->m_nLeftWeaponInfoPosX += INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime();
			if(m_pWeaponInfo->m_nLeftWeaponInfoPosX>0)
			{
				// 무기창 늘리기 완료로 이동
				m_pWeaponInfo->m_nLeftWeaponInfoPosX = 0 ;
				//m_nLeftWindowState = LEFT_WINDOW_INIT ;
				m_nLeftWindowState = LEFT_WEAPON_WINDOW_INIT ;
			}
		}
		break;
	case LEFT_WINDOW_SHOWING:// 윈도우 창 늘리기
		{
			m_pInfWindow->m_bLeftWindow = TRUE ;
			m_nLeftWindowScaleSize += INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime() ;
			if(m_nLeftWindowScaleSize > SIZE_NORMAL_WINDOW_Y)
			{
				// 윈도우 창 늘리기 완료로 이동
				m_nLeftWindowScaleSize = SIZE_NORMAL_WINDOW_Y ;
				m_nLeftWindowState = LEFT_WINDOW_INIT ;
			}
		}
		break;
	case LEFT_WINDOW_INIT:// 윈도우 창 늘리기(완료)
		{
			m_nLeftWindowScaleSize = SIZE_NORMAL_WINDOW_Y ;
			m_nLeftWindowState = LEFT_WINDOW_END ;
			return FALSE;
		}
		break;
	}
	return TRUE;
}


BOOL CINFGameMain::ChangeRightWindowMode()
{
	FLOG( "CINFGameMain::ChangeRightWindowMode()" );
	switch(m_nRightWindowState)
	{
	case RIGHT_WEAPON_WINDOW_INIT:// 무기 정보 보여주기(완료)
		{
			m_pWeaponInfo->m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width - SIZE_WEAPON_X;
			m_nRightWindowScaleSize = SIZE_WEAPON_Y;
			m_pInfWindow->m_bRightWindow = FALSE ;
			m_bRightWindowModeChage = FALSE;
			m_nRightWindowState = RIGHT_WEAPON_WINDOW_END;
			return FALSE;
		}
		break;
	case RIGHT_WEAPON_WINDOW_HIDING:// 무기 정보 줄이기
		{
			m_pWeaponInfo->m_nRightWeaponInfoPosX += INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime();
			if(m_pWeaponInfo->m_nRightWeaponInfoPosX > g_pD3dApp->GetBackBufferDesc().Width)
			{
				//윈도우 보여주기
				m_pInfWindow->m_bRightWindow = TRUE ;
				m_pWeaponInfo->m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width ;
				m_nRightWindowState = RIGHT_WINDOW_SHOWING ;
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);
			}
		}
		break;
	case RIGHT_WINDOW_HIDING:// 윈도우 창 줄이기
		{
/*			if(m_pSystem && m_pSystem->m_bRestored)
			{
				m_pSystem->InvalidateDeviceObjects();
			}
			if(m_pCharacterInfo && m_pCharacterInfo->m_bRestored)
			{
				m_pCharacterInfo->InvalidateDeviceObjects();
			}
*/			m_nRightWindowScaleSize -= INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime() ;
			if(m_nRightWindowScaleSize < SIZE_WEAPON_Y)
			{
				if(m_nRightWindowInfo == LEFT_WINDOW_WEAPON)
				{
					// 무기 창 늘리기
					m_nRightWindowState = RIGHT_WEAPON_WINDOW_SHOWING;
					m_pInfWindow->m_bRightWindow = FALSE ;
					m_nRightWindowScaleSize = SIZE_WEAPON_Y ;
				}
				else
				{
					//윈도우 보여주기
					m_pInfWindow->m_bRightWindow = TRUE ;
					m_pWeaponInfo->m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width ;
					m_nRightWindowState = RIGHT_WINDOW_SHOWING ;
				}
			}
		}
		break;
	case RIGHT_WEAPON_WINDOW_SHOWING:// 무기 창 늘리기
		{
			m_pWeaponInfo->m_nRightWeaponInfoPosX -= INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime();
			if(m_pWeaponInfo->m_nRightWeaponInfoPosX < g_pD3dApp->GetBackBufferDesc().Width - SIZE_ROLL_X)
			{
				// 무기창 늘리기 완료로 이동
				m_pWeaponInfo->m_nRightWeaponInfoPosX = g_pD3dApp->GetBackBufferDesc().Width - SIZE_ROLL_X ;
				//m_nRightWindowState = RIGHT_WINDOW_INIT ;
				m_nRightWindowState = RIGHT_WEAPON_WINDOW_INIT ;
			}
		}
		break;
	case RIGHT_WINDOW_SHOWING:// 윈도우 창 늘리기
		{
			m_pInfWindow->m_bRightWindow = TRUE ;
			m_nRightWindowScaleSize += INF_WINDOW_MOVING_VEL*g_pD3dApp->GetElapsedTime() ;
			if(m_nRightWindowScaleSize > SIZE_NORMAL_WINDOW_Y)
			{
				// 윈도우 창 늘리기 완료로 이동
				m_nRightWindowScaleSize = SIZE_NORMAL_WINDOW_Y ;
				m_nRightWindowState = RIGHT_WINDOW_INIT ;
			}
		}
		break;
	case RIGHT_WINDOW_INIT:// 윈도우 창 늘리기(완료)
		{
			m_nRightWindowScaleSize = SIZE_NORMAL_WINDOW_Y ;
			m_nRightWindowState = RIGHT_WINDOW_END ;
			return FALSE;
		}
		break;
	}
	return TRUE;
}

void CINFGameMain::Tick()
{
	FLOG( "CINFGameMain::Tick()" );

         // 2005-08-02 ispark
	if(m_pHelpDesk
		&& m_pHelpDesk->m_bRenderMissionItemInfo)
	{
		m_pHelpDesk->m_fHelpTextMission -= g_pD3dApp->GetElapsedTime();
		if(m_pHelpDesk->m_fHelpTextMission<0)
		{
			m_pHelpDesk->m_fHelpTextMission = 5.0f;
			m_pHelpDesk->m_bRenderMissionItemInfo = FALSE;
		}
	}

	TickDrawWindowInfo();

	// 도시, 게임 공통

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pMp3Player && m_bMp3PlayFlag == TRUE)
		m_pMp3Player->Tick();	
#endif
	if(m_pInfWindow)
		m_pInfWindow->Tick();
	if(m_pUnitInfoBar)
	{
		m_pUnitInfoBar->Tick();
	}
	if(m_pChat)
		m_pChat->Tick();
	if(m_pQuickSlot)
	{
		m_pQuickSlot->Tick();
	}
//	if(m_pNotice)
//		m_pNotice->Tick();
	if(m_pUnitState)
	{
		m_pUnitState->Tick();
	}
	if(m_pAttackDirection)
	{
		m_pAttackDirection->Tick();
	}
	// 2007-11-22 by dgwoo
	if(m_pChangeCharactor)
	{
		m_pChangeCharactor->Tick();
	}
	if(m_pCouponWindow)
		m_pCouponWindow->Tick();
	if(m_bHelpDeskFlag && g_pSOptionCharacter->sHelpDesk)	
		m_pHelpDesk->Tick();
	if(m_pGameCountDown)
		m_pGameCountDown->Tick();	
	if(m_pOtherCharInfo && m_pOtherCharInfo->m_bRenderInfo)
		m_pOtherCharInfo->Tick();
	if(m_pInfGameMainFaq && m_bFAQProsFlag)
		m_pInfGameMainFaq->Tick();
	//if(m_pCommunityVOIP && m_bVOIPFlag)
	//	m_pCommunityVOIP->Tick();
	if(m_pMenuList)
		m_pMenuList->Tick();


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(int i=0; i< GAMEMAIN_BUTTON_NUMBER; i++)
	{
		if(m_pImgButton[i])
			m_pImgButton[i]->Tick();
	}

	if(m_pFaqBtn)
		m_pFaqBtn->Tick();
#endif
		
	switch(m_nLeftWindowInfo)
	{
	case LEFT_WINDOW_WEAPON:
		{
			m_pInven->Tick() ;
		}
		break;
	case LEFT_WINDOW_INVEN:
		{
			m_pInven->Tick() ;
		}
		break;
	case LEFT_WINDOW_TRANS:
		{
			m_pTrade->Tick() ;
			m_pInven->Tick() ;
		}
		break;
	case LEFT_WINDOW_PARTY:
		{
			m_pCommunity->Tick();
		}
		break;
	case LEFT_WINDOW_SHOP:
		{
//			m_pShop->Tick();
			m_pInven->Tick() ;
		}
		break;
	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pCommunity)
	{
		m_pCommunity->Tick();
	}

// 2007-10-22 by dgwoo 종료시 시간이 안가는 버그발생 예방 차원에서 위치 수정.
//	// 시스템 타임
//	m_pSystem->Tick() ;
	
	// 2008-09-22 by bhsohn EP3 캐릭터 창
//	switch(m_nRightWindowInfo)
//	{
//	case RIGHT_WINDOW_WEAPON:
//		{
//			m_pWeaponInfo->Tick();
//		}
//		break;
//	case RIGHT_WINDOW_INFO:
//		{
//			m_pCharacterInfo->Tick() ;
//		}
//		break;
//	case RIGHT_WINDOW_OPTION:
//		{			
//		}
//		break;
//	}	
	if(m_pCharacterInfo->IsShowWnd())
	{
		m_pCharacterInfo->Tick() ;
	}
	// end 2008-09-22 by bhsohn EP3 캐릭터 창

	// 미션이 들어 왔을때 깜박이기
	m_fTimeElapseButtonCheck -= g_pD3dApp->GetElapsedTime();
	if(m_fTimeElapseButtonCheck < 0)
	{
		m_fTimeElapseButtonCheck = GAME_BUTTON_CHECK_ON;
	}

	if(m_bKnowON[LOW_LEVEL_STAT_HELP] || m_bKnowON[LOW_LEVEL_MISSION_HELP])
	{
		if(m_fTimeElapseButtonCheck == GAME_BUTTON_CHECK_ON)
		{
			m_bKnowTwinkle = !m_bKnowTwinkle;			
		}
	}

	// 2006-04-11 by ispark
	if(m_pCityBoard)
		m_pCityBoard->Tick(g_pD3dApp->GetElapsedTime());

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	TickSpeakerMsg();

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	TickSecuritySystem();

	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	m_pMotherShipC->Tick();

	// 2015-05-01 by killburne
	m_pStrategicPointDisplay->Tick();


	// 2009. 01. 23 by ckPark 인터페이스 껐을 시 포탈/스킬 사용지연 체크

// 	// 2008. 12. 16 by ckPark 아이템 사용 지연
// 	TickItemDelay();	
// 	// end 2008. 12. 16 by ckPark 아이템 사용 지연
// 
// 
// 	// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
// 	TickSkillDealy();
// 	// end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연

	// end 2009. 01. 23 by ckPark 인터페이스 껐을 시 포탈/스킬 사용지연 체크

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	m_nAddPosY = 0;
	if(m_pUnitInfoBar->GetRestCount() > 0)// 휴식 경험치 카운트가 있다면, 튜토리얼 버튼을 밑으로
	{
		m_nAddPosY += TUTORIAL_BUTTON_Y_ADDCOUNT;
		m_pTut_SelBtn->SetBtnPosition(TUTORIAL_BUTTON_X, TUTORIAL_BUTTON_Y+m_nAddPosY);
	}
	if(m_pUnitInfoBar->GetKillCount() > 0)// 킬 카운트가 있다면, 튜토리얼 버튼을 밑으로
	{
		m_nAddPosY += TUTORIAL_BUTTON_Y_ADDCOUNT;
		m_pTut_SelBtn->SetBtnPosition(TUTORIAL_BUTTON_X, TUTORIAL_BUTTON_Y+m_nAddPosY);
	}
#endif // S_BONUSEXPSYSTEM_RENEWAL
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

	UpdateServerTimeString();	// 2012-12-10 by jhjang 시간 표시 수정
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	TickArmorTimeCheck();		// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
#endif
}

void CINFGameMain::RenderLeftRightWindow()
{
	switch(m_nLeftWindowInfo)
	{
	case LEFT_WINDOW_WEAPON:
		{
// 2008-05-29 by dgwoo 메인 인터페이스 변경 기획.
//			if(m_pWeaponInfo )
//				m_pWeaponInfo->RenderLeftWeapon() ;
		}
		break;
	case LEFT_WINDOW_INVEN:
		{
			if(m_pInven && !m_pInven->m_bRestored)
			{
				m_pInven->RestoreDeviceObjects();
			}
			if(!m_bLeftWindowModeChage && m_pInven)
			{
				m_pInven->Render() ;
			}

		}
		break;
	case LEFT_WINDOW_TRANS:
		{
			if(m_pTrade && !m_pTrade->m_bRestored)
			{
				m_pTrade->RestoreDeviceObjects();
			}
			if(m_pInven && !m_pInven->m_bRestored)
			{
				m_pInven->RestoreDeviceObjects();
			}

			if(!m_bLeftWindowModeChage && m_pInven && m_pTrade)
			{
				m_pTrade->Render() ;
				m_pInven->Render() ;
			}
		}
		break;
	case LEFT_WINDOW_PARTY:
		{
//			if(m_pParty && !m_pParty->m_bRestored)
//			{
//				m_pParty->RestoreDeviceObjects();
//			}
/*			if(m_pCommunity && !m_pCommunity->m_bRestored)
			{
				m_pCommunity->RestoreDeviceObjects();
			}
*/			if(!m_bLeftWindowModeChage && m_pCommunity)
			{
//				m_pParty->Render();
				m_pCommunity->Render();
			}
		}
		break;
	case LEFT_WINDOW_SHOP:
		{
//			if(m_pShop && !m_pShop->m_bRestored)
//			{
//				m_pShop->RestoreDeviceObjects();
//			}
//			if(m_pInven && !m_pInven->m_bRestored)
//			{
//				m_pInven->RestoreDeviceObjects();
//			}
//			if(!m_bLeftWindowModeChage && m_pShop)
//			{
//				m_pShop->Render();
//				m_pInven->Render() ;
//			}
		}
		break;
	}

	switch(m_nRightWindowInfo)
	{
	case RIGHT_WINDOW_WEAPON:
		{
// 2008-05-29 by dgwoo 메인 인터페이스 변경 기획.	
//			if(m_pWeaponInfo )
//				m_pWeaponInfo->RenderRightWeapon() ;
		}
		break;
	case RIGHT_WINDOW_INFO:
		{
			// 2008-09-22 by bhsohn EP3 캐릭터 창
//			if(m_pCharacterInfo && !m_pCharacterInfo->m_bRestored)
//			{
//				m_pCharacterInfo->RestoreDeviceObjects();
//			}
//
//			if(!m_bRightWindowModeChage && m_pCharacterInfo)
//			{
//				m_pCharacterInfo->Render() ;
//			}

		}
		break;
	case RIGHT_WINDOW_OPTION:
		{
			if(m_pSystem && !m_pSystem->m_bRestored)
			{
				m_pSystem->RestoreDeviceObjects();
			}

			if(!m_bRightWindowModeChage && m_pSystem)
			{
				m_pSystem->Render() ;
			}
		}
		break;
	}

	// 2008-03-19 by bhsohn Ep3옵션창
	if(m_pSystem && m_pSystem->IsOptionShow())
	{
		if(!m_pSystem->m_bRestored)
		{
			m_pSystem->RestoreDeviceObjects();
		}
		m_pSystem->Render();
	}
	// end 2008-03-19 by bhsohn Ep3옵션창


}

void CINFGameMain::Render()
{
	FLOG( "CINFGameMain::Render()" );

	if(g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _SHOP )
	{
		if(m_pChat)	m_pChat->Render();		

		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().PartyID != 0) 
			m_pCommunity->GetParty()->RenderPartyFrame();
		
		if(g_pD3dApp->m_dwGameState != _SHOP)
		{
		}
		
		if(m_pMission && m_pMission->m_bShow)	m_pMission->Render();
		if(m_pUnitState)
		{
			if(g_pD3dApp->m_pShuttleChild->m_bMouseLock)
			{
//				m_pUnitState->SetState( INF_US_LOCK_MASK );
			}
			else
			{
				m_pUnitState->OffState( INF_US_LOCK_MASK );
			}
		}

		if(m_pAttackDirection)	m_pAttackDirection->Render();
		if(m_pUnitState)		m_pUnitState->Render();
	}

	if(m_pOutPost)
	{
		m_pOutPost->Render();
	}
	// 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
// 	if(m_pChangeCharactor)
// 	{
// 		m_pChangeCharactor->Render();
// 	}
	// END 2013-04-05 by bhsohn 케릭터 변경 창 윈도우 순서 정렬 처리
	if(m_pCouponWindow)
	{
		m_pCouponWindow->Render();
	}

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	// 2008-04-23 by bhsohn 스피커 아이템 랜더링 시점 변경
	//RenderSpeakerMsg();

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	RenderSecuritySystem();


	if(m_pGameCountDown)
	{
		m_pGameCountDown->Render();
	}
	// 도움말 보이기
	if(m_bHelpDeskFlag && g_pSOptionCharacter->sHelpDesk)
	{			
		m_pHelpDesk->Render();		
	}

	// 퀘스트 아이템 습득 표시
	if(m_pHelpDesk && m_pHelpDesk->m_bRenderMissionItemInfo)
	{
		m_pHelpDesk->DrawHelpWindow((g_pD3dApp->GetBackBufferDesc().Width /2)-((strlen(m_pHelpDesk->m_strMissionItem)*7)/2)+30, 120, 
			m_pHelpDesk->m_nHelpDeskWidth+(HELPDESK_WINDOW_TEXT_VIEW_GAP*2),
			m_pHelpDesk->m_nHelpDeskHeight+HELPDESK_WINDOW_TEXT_VIEW_GAP);
	}

	// 고도계 렌더링
	// 2006-01-05 by ispark, 상점에선 기타 정보 렌더링을 안한다
	if(g_pD3dApp->m_dwGameState != _SHOP)
	{
	    ProcessAltiMeter(g_pGround->m_projectInfo.fWaterHeight, g_pScene->m_nMaxAtitudeHeight, g_pShuttleChild->m_vPos.y );
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
	{
		m_pImgButton[i][m_nButtonState[i]]->Move(GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*i, GAMEMAIN_BUTTON_START_Y);
		m_pImgButton[i][m_nButtonState[i]]->Render();
	}
#endif
	// 2007-07-19 by dgwoo 기존의 위치에서 변경.
	RenderOnInfo();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경;
	m_pImgButtonBK->Move(GAMEMAIN_BK_START_X, GAMEMAIN_BK_START_Y);
	m_pImgButtonBK->Render();

	for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++)
	{
		m_pImgButton[i]->Render();
	}
#endif
	// 2007-07-04 by bhsohn 오퍼레이터 추가

	m_pFaqBtn->Render();
	m_pCouponBtn->Render();
	// 2009. 10. 14 by jskim 프리스카 제거
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if (myShuttleInfo.InfluenceType == 1 &&
		IS_CITY_MAP_INDEX(myShuttleInfo.MapChannelIndex.MapIndex) &&
		g_pShuttleChild->m_dwState == _LANDED) 
	{

		if (g_pD3dApp->m_dwGameState != _SHOP)
			
			m_pTut_SelBtn->Render();	
	}	

	if (m_pOtherCharInfo && m_pOtherCharInfo->m_bRenderInfo)

	    m_pOtherCharInfo->Render();
	
	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	if (m_pMotherShipC) m_pMotherShipC->Render();
	
	if (m_pStrategicPointDisplay) m_pStrategicPointDisplay->Render();
	// end 2008-04-04 by bhsohn 모선전 관리 클래스 추가

	// 2008-04-23 by bhsohn 스피커 아이템 랜더링 시점 변경
	// 스피커 아이템을 가장 마지막에 랜더링해야지 가장 위에 보인다.
	RenderSpeakerMsg();

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	RenderSysMsgWnd();	// 서버 시간보기
}

void CINFGameMain::RenderHelp()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경;
	if(m_pHelp[0])
	{
		m_pHelp[0]->Move(g_pD3dApp->GetBackBufferDesc().Width/2 - m_pHelp[0]->GetImgSize().x/2,g_pD3dApp->GetBackBufferDesc().Height/2 - m_pHelp[0]->GetImgSize().y/2);
		m_pHelp[0]->Render();
	}
#else
	if(m_pHelp[0])
	{
		m_pHelp[0]->Move(g_pD3dApp->GetBackBufferDesc().Width/2-359,g_pD3dApp->GetBackBufferDesc().Height/2-150);
		m_pHelp[0]->Render();
	}
	if(m_pHelp[1])
	{
		m_pHelp[1]->Move(g_pD3dApp->GetBackBufferDesc().Width/2,g_pD3dApp->GetBackBufferDesc().Height/2-150);
		m_pHelp[1]->Render();
	}
#endif
}
int CINFGameMain::WndProcLeftRightWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
		if(!m_bLeftWindowModeChage)
		{
			switch(m_nLeftWindowInfo)
			{
		case LEFT_WINDOW_INVEN:
			{
				if(m_pInven){					
						if(m_pInven->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
							return INF_MSGPROC_BREAK;
					}
				}
				break;
			case LEFT_WINDOW_TRANS:
				{
				if(m_pTrade){
						if(m_pTrade->WndProc(uMsg, wParam, lParam) ==  INF_MSGPROC_BREAK)
							return INF_MSGPROC_BREAK;
					}
				if(m_pInven){
						if(m_pInven->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
							return INF_MSGPROC_BREAK;
					}
				}
				break;
			case LEFT_WINDOW_PARTY:
				{
				if(m_pCommunity){
						if(m_pCommunity->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
							return INF_MSGPROC_BREAK;
					}
				}
				break;
			case LEFT_WINDOW_SHOP:
				{
				}
				break;
			}
		}
		if(!m_bRightWindowModeChage)
		{
			switch(m_nRightWindowInfo)
			{
			case RIGHT_WINDOW_INFO:
				{
					// 2008-09-22 by bhsohn EP3 캐릭터 창
//					if(m_pCharacterInfo)
//					{
//						if(m_pCharacterInfo->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
//							return INF_MSGPROC_BREAK;
//					}
					// end 2008-09-22 by bhsohn EP3 캐릭터 창
				}
				break;
			case RIGHT_WINDOW_OPTION:
				{
				if(m_pSystem){					
						if(m_pSystem->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
							return INF_MSGPROC_BREAK;
					}
				}
				break;
			}
		}

		// 2008-03-19 by bhsohn Ep3옵션창
		if(m_pSystem)
		{					
			if(m_pSystem->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		// end 2008-03-19 by bhsohn Ep3옵션창

	return INF_MSGPROC_NORMAL;
}

int CINFGameMain::WndProcKeyDown(WPARAM wParam, LPARAM lParam)
{
	// 2005-12-09 by ispark
	// 키 사용 여부 체크 
	if(CheckKeyUseState(wParam, lParam, TRUE, FALSE) ||
		g_pD3dApp->m_bChatMode == TRUE ||
		g_pD3dApp->m_dwGameState == _SHOP || 
		m_nLeftWindowInfo == LEFT_WINDOW_TRANS ||
		g_pGameMain->m_pTrade->m_bTrading)
		return INF_MSGPROC_BREAK;

	// 2008-12-04 by bhsohn 무한으로 총알 나가는 현상 처리
	if(g_pShuttleChild && g_pShuttleChild->IsWeapongShotting())
	{
		if(VK_F2 != wParam)
		{
			// 총알 쏘면서 편대 대형 참여만 가능하다.
			return INF_MSGPROC_NORMAL;
		}
		
	}
	// end 2008-12-04 by bhsohn 무한으로 총알 나가는 현상 처리
	// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
// 	if(g_pShuttleChild->m_ShowInterface)
// 	{
// 		switch(wParam)
// 		{
// 		case VK_ESCAPE:
// 			{
// 				g_pShuttleChild->DeletePattern();	
// 				break;
// 			}
// 		}			
// 		return INF_MSGPROC_BREAK;
// 	}
	//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
	// 사용 가능하면 어떤 내용인지 밑에서 구현
	switch(wParam)
	{
	case VK_F1:
		{
//			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
//				break;
			
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE)
//				break;

			ToggleHelpImage();
		}
		break;
	case VK_F2:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE)
//				break;

			// 2004-11-26 by jschoi - _GAME 상태가 아니거나 워프중에는 대형에 참여할 수 없다.
			if( g_pD3dApp->m_dwGameState != _GAME ||
				g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
				g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN 
				|| IS_CITY_MAP_INDEX(g_pD3dApp->GetMyShuttleMapChannelIndex().MapIndex)) // 2007-06-26 by bhsohn 마을에서 편대 비행 되는 버그 수정
			{
				DBGOUT("Current Game State (%d) , and PatternType is (%d), So Can't join FormationFlight.\n",g_pD3dApp->m_dwGameState,g_pShuttleChild->m_nCurrentPatternNumber);
				break;
			}
			if( g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType && 
				!g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingPositionType && 
				g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMEMBER &&
				g_pShuttleChild->m_dwState == _NORMAL &&
				g_pShuttleChild->m_bAttackMode != _SIEGE &&
				(FALSE == IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) || g_pShuttleChild->m_bIsAir))
			{
				vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
				while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
				{
					if(	(*itParty)->m_pEnemyData &&
						(*itParty)->m_bPartyType == _PARTYMASTER)
					{

//				CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
//				while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
//				{
//					if(itEnemy->second->m_infoParty.bPartyType == _PARTYMASTER)
//					{
						if( D3DXVec3Length(&((*itParty)->m_pEnemyData->m_vPos - g_pShuttleChild->m_vPos)) < 300.0f &&
							(*itParty)->m_pEnemyData->m_dwState == _NORMAL)
						{
//							MSG_IC_PARTY_GET_FLIGHT_POSITION sMsg;
//							memset(&sMsg,0x00,sizeof(sMsg));
//							char buffer[SIZE_MAX_PACKET];
//							int nType = T_IC_PARTY_GET_FLIGHT_POSITION;
//							sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
//							memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
//							memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
//							g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
							g_pShuttleChild->m_pClientParty->ISendPartyGetFlightPosition();
							
//							g_pD3dApp->m_pShuttleChild->m_infoParty.bFormationFlyingPositionType = 5;
//							g_pD3dApp->m_pShuttleChild->m_bFormationFlight = TRUE;
							// 2005-10-24 by ispark
							// 편대비행 신청일 때 기어 정지 푼다.
							g_pShuttleChild->m_bUnitStop = FALSE;
							g_pShuttleChild->m_nBooster = BOOSTER_TYPE_MIN;
							g_pShuttleChild->m_bPenaltyUnitStop = FALSE;
							g_pShuttleChild->m_bMouseLock = FALSE;
							g_pShuttleChild->m_bMouseMoveLock = FALSE;
							g_pD3dApp->m_bFixUnitDirMode = FALSE;
							g_pInterface->m_pGameMain->InitShowWindow();
							// 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달	
							g_pShuttleChild->SendFieldSocketChangeBodyCondition( g_pShuttleChild->m_myShuttleInfo.ClientIndex, BODYCON_SET_OR_CLEAR_MASK );
							//end 2010. 03. 30 by jskim 유닛 상태 바디컨디션 서버 전달
							g_pShuttleChild->m_ptDirOld.x = 0;
							g_pShuttleChild->m_ptDirOld.y = 0;
						}
						else if((*itParty)->m_pEnemyData->m_dwState != _NORMAL)
						{
							char buf[256];
							wsprintf(buf,STRMSG_C_PARTY_0042);//"편대장이 편대비행을 할 수 없는 상태입니다."
							g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
						}
						else
						{
							char buf[256];
							wsprintf(buf,STRMSG_C_PARTY_0043);//"편대장으로부터 거리가 너무 멀리 떨어져 있습니다."
							g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
						}
						break;
					}
//					itEnemy++;
					itParty++;
				}
			}
			else if(!g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingType)
			{//
				char buf[256];
				wsprintf(buf,STRMSG_C_PARTY_0044);//"편대 비행 대형 선택이 잘못되었습니다."
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
			}
			else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bFormationFlyingPositionType)
			{
				char buf[256];
				wsprintf(buf,STRMSG_C_PARTY_0045);//"편대 비행 포지션이 잘못되었습니다."
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
			}
			else if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _PARTYMEMBER)
			{
				char buf[256];
				wsprintf(buf,STRMSG_C_PARTY_0046);//"현재 편대원이 아닙니다. 확인해 주시기 바랍니다."
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
			}
			else
			{
				char buf[256];
				wsprintf(buf,STRMSG_C_PARTY_0047);//"편대 비행을 할 수 없는 상태입니다."
				g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SYSTEM);
			}
		}
		break;
	case VK_ESCAPE :
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
			// 2005-08-08 by ispark - 정지시에는 허용
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE || 
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 g_pShuttleChild->m_bUnitStop == FALSE))
//				return INF_MSGPROC_BREAK;
			// 2007-04-02 by dgwoo 옵저버 모드일경우와 튜토리얼 모드에서는 esc키의 작동을 막는다.
			// 
			if(g_pShuttleChild->IsObserverMode() || g_pTutorial->IsTutorialMode())
				break;
			SetItemInfo(0,0,0,0);
			if( g_pD3dApp->m_dwGameState == _CITY && 
				g_pInterface->m_pCityBase->GetCurrentBuildingNPC() )
			{
				break;
			}
			
			// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//			if(TRUE == m_pMissionInfo->GetMissionWindowState())
//			{
//				m_pMissionInfo->MissionProcFlag(FALSE);
//				break;
//			}
			if(IsShowTreeWnd())
			{
				ShowMissionTreeWnd(FALSE, TRUE);
				break;
			}				

			if(IsShowing())
			{
				if(m_pTrade->m_bTrading && m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
				{
					m_pInfWindow->AddMsgBox(STRMSG_C_TRADE_0001, _Q_TRADE_CANCEL);//"거래를 취소하시겠습니까?"
				}
				else if(g_pD3dApp->m_dwGameState == _SHOP)
				{
					//LeftWindowShow(FALSE,LEFT_WINDOW_SHOP);
//					m_pShop->OnCancelButtonClicked();
				}
				else
				{
					LeftWindowShow(FALSE,m_nLeftWindowInfo);
				}
				RightWindowShow(FALSE,m_nRightWindowInfo);
				if(m_bShowHelp)
				{
					ToggleHelpImage();
				}
				// 2008-08-22 by bhsohn EP3 인벤토리 처리
				if(m_pCharacterInfo && m_pCharacterInfo->IsShowWnd())
				{
					m_pCharacterInfo->ShowCharacterInfoWnd(FALSE, NULL);	
					//g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);	// 2013-01-03 by mspark, 메인 메뉴를 클릭하였을 경우 사운드가 겹쳐서 들리던 문제 수정 - 주석 닮
				}
				// end 2008-08-22 by bhsohn EP3 인벤토리 처리
			}
			else if(m_bQuestView && g_pSOptionCharacter->sHelpDesk == TRUE )
			{
				m_bQuestView = FALSE;
			}
			else if(m_pOtherCharInfo->m_bRenderInfo)
			{
				m_pOtherCharInfo->m_bRenderInfo = FALSE;
			}
			else if(g_pGameMain->m_pChat->m_bMenuListUse == TRUE)
			{
				g_pGameMain->m_pChat->m_bMenuListUse = FALSE;
			}
			else if(m_bMenuListFlag == TRUE)
			{
				m_bMenuListFlag = FALSE;
			}
			// 2007-09-12 by bhsohn 2차 암호 시스템 구현
			else if(m_pINFSecuMain && m_pINFSecuMain->IsShowSecurityWnd())
			{
				m_pINFSecuMain->AllHideSecurityWnd();
			}
			// end 2007-09-12 by bhsohn 2차 암호 시스템 구현
			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			else if(m_pCommunity && m_pCommunity->IsShowCommunityWindow())
			{
				// 커뮤니티 창 닫기
				OnClickCommunity();				
			}
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
			// 2008-08-22 by bhsohn EP3 인벤토리 처리
			else if(m_pInven && m_pInven->IsShowInven())
			{
				// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그 
				// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				//if(g_INFCnt==0)
				if(g_pD3dApp->GetINFCnt() == 0)
				{
					g_pGameMain->m_bChangeMousePoint = FALSE;
				}				
				// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그 
				m_pInven->ShowInven(NULL, NULL, TRUE);	
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			}
			// end 2008-08-22 by bhsohn EP3 인벤토리 처리
			// 2008-08-22 by bhsohn EP3 인벤토리 처리
			else if(m_pCharacterInfo && m_pCharacterInfo->IsShowWnd())
			{
				m_pCharacterInfo->ShowCharacterInfoWnd(FALSE, NULL);	
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			}
			// end 2008-08-22 by bhsohn EP3 인벤토리 처리
			else if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() == NULL)
			{
				// 2008-03-19 by bhsohn Ep3옵션창
				//RightWindowShow(TRUE, RIGHT_WINDOW_OPTION);
				PopupSystemOptinWnd();
			}
			
			// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
			//m_pItemInfo->ShowItemInfo(FALSE);
			ShowItemInfo(FALSE);
			// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

//			g_pD3dApp->m_pShuttleChild->m_bMouseLock = FALSE;	// 2005-10-07 by ispark 삭제 ESC키로 인한 회전 안돼는 버그 
			m_pUnitState->OffState( INF_US_LOCK_MASK );
		}
		break;
	case VK_TAB :
		{
//					LeftWindowShow(TRUE, LEFT_WINDOW_INVEN);
		}
		break;
	case VK_F3:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE)
//				break;

			if(g_pD3dApp->m_dwGameState == _GAME)
			{
				// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				//g_INFCnt++;
				g_pD3dApp->AddINFCnt();
				// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				g_pGameMain->m_bChangeMousePoint = TRUE;								
				// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				m_pInfWindow->AddMsgBox( STRMSG_C_PK_0004, _Q_BATTLE_NAME );//"1:1대결을 할 유닛을 입력하시오."
				
			}
		}
		break;
	case VK_F5:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
			// 2005-08-08 by ispark - 정지시에는 허용
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE || 
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 g_pShuttleChild->m_bUnitStop == FALSE))
//				break;
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김


			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
				break;
			if(m_nLeftWindowInfo == LEFT_WINDOW_INVEN)
			{
				SetItemInfo(0,0,0,0);
			}
			LeftWindowShow(TRUE, LEFT_WINDOW_INVEN);
		}
		break;
	case VK_F6:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-04 by ispark
			// 2005-08-08 by ispark - 정지시에는 허용
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE || 
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 g_pShuttleChild->m_bUnitStop == FALSE))
//				break;
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
				break;
			if(m_nLeftWindowInfo == RIGHT_WINDOW_INFO)
			{
				SetItemInfo(0,0,0,0);
			}
			RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
		}
		break;
	case VK_F7:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
			// 2005-08-08 by ispark - 정지시에는 허용
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE || 
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 g_pShuttleChild->m_bUnitStop == FALSE))
//				break;
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
				break;
			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			//LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
			OnClickCommunity();
			
		}
		break;
	case VK_F8:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
			// 2005-08-08 by ispark - 정지시에는 허용
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE || 
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 g_pShuttleChild->m_bUnitStop == FALSE))
//				break;
			// 2005-11-02 by ispark
			// 운영자는 거래가 불가능하다.
			if(g_pShuttleChild->m_dwPartType == _ADMIN)
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051102_0001, COLOR_ERROR);//"현재 유저는 운영자이므로 거래가 불가능합니다."
				break;
			}
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
				break;
			if(!m_pTrade->m_bTrading)
			{
				// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				//g_pGameMain->m_bChangeMousePoint = TRUE;
				// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				//g_INFCnt++;
				g_pD3dApp->AddINFCnt();
				// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				g_pGameMain->m_bChangeMousePoint = TRUE;
				// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0002, _Q_TRADE_INPUT_NAME);//"거래할 유닛을 입력하시오"
			}
			else
			{
				m_pInfWindow->AddMsgBox(STRMSG_C_TRADE_0003, _Q_TRADE_CANCEL);//"거래를 취소하시겠습니까?"
			}
			//LeftWindowShow(TRUE, LEFT_WINDOW_TRANS);
		}
		break;
	case VK_F9:
		{
			// 2005-04-07 by jschoi - Tutorial
			// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
			// 2005-08-08 by ispark - 정지시에는 허용
			// 2005-08-09 by ispark - 삭제
//			if(g_pTutorial->IsTutorialMode() == TRUE || 
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 g_pShuttleChild->m_bUnitStop == FALSE))
//				break;
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
				break;
			// 2008-03-19 by bhsohn Ep3옵션창
			//RightWindowShow(TRUE, RIGHT_WINDOW_OPTION);
			PopupSystemOptinWnd();
		}
		break;
	case VK_F11:
		{
			// Show/Hide Admin Info
			USHORT nRace = g_pShuttleChild->m_myShuttleInfo.Race;
			/*if(COMPARE_RACE(nRace,RACE_STAFF))		// 2014-08-09 Future, adding Staff on suspicion
			{
			g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pShuttleChild->m_myShuttleInfo.CharacterName, STRCMD_CS_COMMAND_BRINGCALLGM_1 );
			}*/
			SHORT KeyState = GetAsyncKeyState(VK_SHIFT);
			if (COMPARE_RACE(nRace, RACE_OPERATION) && (1 << 16) & KeyState)
			{
#ifdef _NOCLIP
				g_pD3dApp->m_bNoclip = !g_pD3dApp->m_bNoclip;
#endif
			}
			//2015-01-29 by st0rmy, Draw Events if Ctrl+F11 pressed
			else if (COMPARE_RACE(nRace, RACE_OPERATION) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
#ifdef _DRAW_EVENTS
				g_pD3dApp->m_bDrawEvents = !g_pD3dApp->m_bDrawEvents;
#endif
			}
			else if (COMPARE_RACE(nRace, RACE_OPERATION)) //2014-09-03 st0rmy added wireframe mode for SA
			{
#ifdef _WIREFRAME
				g_pD3dApp->m_bWireframe = !g_pD3dApp->m_bWireframe;
#endif
			}
		}
		break;
		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	case VK_SHIFT:
		{
			ShowMyEqItemTooltip();
		}
		break;
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁		
	}
	return INF_MSGPROC_NORMAL;
	
}

int CINFGameMain::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFGameMain::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	
	if( g_pInterface->m_bShowInterface &&
		WndProcButtonMouseMessage( uMsg, wParam , lParam) == INF_MSGPROC_BREAK)
	{
		return INF_MSGPROC_BREAK;
	}

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( m_pMp3Player && m_bMp3PlayFlag == TRUE &&
		g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE &&
		g_pInterface->m_bShowInterface == TRUE)
	{
		if(m_pMp3Player->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}
#endif


	if(m_bHelpDeskFlag && g_pSOptionCharacter->sHelpDesk && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE )
	{
		if(m_pHelpDesk->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}

	if(m_pOtherCharInfo && m_pOtherCharInfo->m_bRenderInfo && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
	{
		if(m_pOtherCharInfo->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}
	// 2006-04-25 by ispark
	if(m_pInfSkill)
	{
		if(m_pInfSkill->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// 2007-09-12 by bhsohn 2차 암호 시스템 구현
	if(m_pINFSecuMain)
	{
		if(m_pINFSecuMain->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}
	// end 2007-09-12 by bhsohn 2차 암호 시스템 구현

	if(m_pCouponWindow)
	{
		if(m_pCouponWindow->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	if(m_pMotherShipC)
	{
		if(GetSummonMotherShipCnt() && m_pMotherShipC->WndProc(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
		// 2008-04-08 by dgwoo interface class에 위치 변경.
//		if(m_pMotherShipC->WndProcOption(uMsg,wParam,lParam) == INF_MSGPROC_BREAK)
//		{
//			return INF_MSGPROC_BREAK;
//		}
	}

	// 2015-05-01 by killburne
	if (m_pStrategicPointDisplay)
	{
		if (m_pStrategicPointDisplay->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}

	// end 2008-04-04 by bhsohn 모선전 관리 클래스 추가
	
	if(g_pD3dApp->m_dwGameState == _CITY && g_pInterface->m_pCityBase)
	{
		if(m_pInfGameMainFaq && m_bFAQProsFlag)
		{
			if(m_pInfGameMainFaq->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		if(m_pUnitInfoBar )
		{
			if(m_pUnitInfoBar->WndProcCity(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}		
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(m_pINFGameMainSysMsg)
		{
			if(m_pINFGameMainSysMsg->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		// city state가 격납고인 경우
		// 2005-11-28 by ispark, 채팅과 순서 바꿈
		if(!g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
		{
			if( WndProcLeftRightWindow(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		if(m_pChat)
		{
			if(m_pChat->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		if(m_pQuickSlot)
		{
			if(m_pQuickSlot->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		switch(uMsg)
		{
		case WM_KEYDOWN:
			{
				if(WndProcKeyDown( wParam, lParam ) == INF_MSGPROC_BREAK)
					return INF_MSGPROC_BREAK;

				// 2005-04-07 by jschoi - Tutorial
				// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
				// 2005-08-08 by ispark - 정지시에는 허용
				// 2005-08-09 by ispark - 삭제
//				if(g_pTutorial->IsTutorialMode() == TRUE || 
//					(g_pD3dApp->m_bCharacter == FALSE && 
//					 g_pShuttleChild->m_bUnitStop == FALSE))
//				if(CheckKeyUseState(lParam, TRUE, FALSE))
//					return INF_MSGPROC_BREAK;

				if(!g_pD3dApp->m_bChatMode && 
					g_pTutorial->IsTutorialMode() == FALSE &&
					g_pInterface->m_bShowInterface == TRUE)
				{
					switch(lParam)
					{
					case WM_KEYDOWN_LPARAM_I:// inventory
						{
							// 2005-04-11 by jschoi - Tutorial
							// 2005-07-19 by ispark
							// 창이 보이지 않을 때 I,V키를 썼을때 멈추는 버그 수정
//							if(g_pTutorial->IsTutorialMode() == FALSE)
							LeftWindowShow(TRUE, LEFT_WINDOW_INVEN);								
						}
						break;
					case WM_KEYDOWN_LPARAM_V:// 캐릭터 창
						{
							// 2005-04-11 by jschoi - Tutorial
							// 2005-07-19 by ispark
							// 창이 보이지 않을 때 I,V키를 썼을때 멈추는 버그 수정
//							if(g_pTutorial->IsTutorialMode() == FALSE)
							RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
							g_pGameMain->m_bQuestView = FALSE;
							
//							if(m_bMissionON)
//							{
//								m_bMissionON = FALSE;								
//								m_pQuest->m_nQuestIndex = 0;
//							}

							// 2006-08-22 by ispark, 스탯 도움말
							if(m_bKnowON[LOW_LEVEL_STAT_HELP])
							{	
								m_bKnowON[LOW_LEVEL_STAT_HELP] = FALSE;
							}
							
							// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합							
							if(m_bShowStatOpWnd)
							{
								HideOpStatChange();
							}							
						}
						break;
					case WM_KEYDOWN_LPARAM_P:
						{
							// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//							if(TRUE == m_pMissionInfo->GetMissionWindowState())
//							{
//								m_pMissionInfo->MissionProcFlag(FALSE);
//								m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//							}
//							else
//							{
//								m_pMissionInfo->MissionProcFlag(TRUE);
//								m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//							}
							BOOL bShowTree = IsShowTreeWnd();
							bShowTree ^= TRUE;
							ShowMissionTreeWnd(bShowTree, TRUE);
							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
							
						}
						break;
					}
				}
			}
			break;
			// 2007-09-12 by bhsohn 2차 암호 시스템 구현
		case WM_SYSKEYDOWN:
			{
				if(WndProcSysKeyDown( wParam, lParam ) == INF_MSGPROC_BREAK)
					return INF_MSGPROC_BREAK;					
			}
			break;
			// end 2007-09-12 by bhsohn 2차 암호 시스템 구현		
		}
	}
	else
	{
		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
		//if( m_pUnitInfoBar && g_pInterface->m_bShowInterface &&
		if( !g_pShuttleChild->IsObserverMode() && m_pUnitInfoBar && g_pInterface->m_bShowInterface &&
			g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE )
		{
			if(m_pUnitInfoBar->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(m_pINFGameMainSysMsg)
		{
			if(m_pINFGameMainSysMsg->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

		if(m_pMission && m_pMission->m_bShow)
		{
			if(m_pMission->WndProc( uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}		
		if(m_pChat)
		{
			if(m_pChat->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}

		// 2009. 02. 18 by ckPark 마우스 안보이기시 퀵슬롯 탭 안먹는 버그
// 		if(m_pQuickSlot && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
// 		{
// 			if(m_pQuickSlot->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
// 				return INF_MSGPROC_BREAK;
// 		}

		if(m_pQuickSlot && g_pD3dApp->m_dwGameState == _GAME)
		{
			if(m_pQuickSlot->WndProc(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;
		}
		// end 2009. 02. 18 by ckPark 마우스 안보이기시 퀵슬롯 탭 안먹는 버그

//		if( WndProcLeftRightWindow(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
//			return INF_MSGPROC_BREAK;
		switch(uMsg)
		{
		case WM_KEYDOWN:
			{
				// 2008-10-20 by bhsohn VOIP 버그 수정
				if(g_pD3dApp->m_dwGameState == _SHOP)
				{
					// 상점상태에서도 Z키 먹게 변경
					switch(lParam)
					{
					case WM_KEYDOWN_LPARAM_Z:
						{
							if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() == TRUE ||
								g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == TRUE)
							{
								//ChangeVOIPMode(); 2016-09-20 removing VOIP
								return INF_MSGPROC_BREAK;
							}
						}
						break;
					}

					// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
					switch(wParam)
					{
					case VK_SHIFT:
						{
							ShowMyEqItemTooltip();
						}
						break;
					}
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				}
				// end 2008-10-20 by bhsohn VOIP 버그 수정

				if( WndProcKeyDown( wParam, lParam ) == INF_MSGPROC_BREAK)
					return INF_MSGPROC_BREAK;

//				if(CheckKeyUseState(lParam, TRUE, FALSE))
//					return INF_MSGPROC_BREAK;

				// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
				if(!g_pD3dApp->m_bChatMode &&
					g_pTutorial->IsTutorialMode() == FALSE &&
					g_pInterface->m_bShowInterface == TRUE && 
					!g_pShuttleChild->IsObserverMode()) 
				{
					switch(lParam)
					{
					case WM_KEYDOWN_LPARAM_I:// inventory
						{
							LeftWindowShow(TRUE, LEFT_WINDOW_INVEN);
						}
						break;
					case WM_KEYDOWN_LPARAM_V:// 캐릭터 창
						{
							RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
							g_pGameMain->m_bQuestView = FALSE;

//							if(m_bMissionON)
//							{
//								m_bMissionON = FALSE;								
//								m_pQuest->m_nQuestIndex = 0;
//							}

							// 2006-08-22 by ispark, 스탯 도움말
							if(m_bKnowON[LOW_LEVEL_STAT_HELP])
							{
								m_bKnowON[LOW_LEVEL_STAT_HELP] = FALSE;
							}
							// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합							
							if(m_bShowStatOpWnd)
							{
								HideOpStatChange();
							}							
						}
						break;
					case WM_KEYDOWN_LPARAM_P:
						{
							// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//							if(TRUE == m_pMissionInfo->GetMissionWindowState())
//							{
//								m_pMissionInfo->MissionProcFlag(FALSE);
//								m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//							}
//							else
//							{
//								// 2007-05-11 by bhsohn 아레나 버그수정
//								// 아레나 중엔 미션 버튼 금지
//								if(TRUE == g_pInterface->IsArenaStart())
//								{
//									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070529_0100,COLOR_ITEM);
//									return INF_MSGPROC_NORMAL;
//								}
//								g_pInterface->SetWindowOrder(WNDMissionInfo);
//								m_pMissionInfo->MissionProcFlag(TRUE);
//								m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//							}
							BOOL bShowTree = IsShowTreeWnd();
							if(bShowTree)
							{
								ShowMissionTreeWnd(FALSE, TRUE);								
							}
							else
							{
								if(TRUE == g_pInterface->IsArenaStart() || ARENA_STATE_ARENA_GAMING == g_pD3dApp->GetArenaState())	
									// 2013-08-05 by ssjung 유저가 아레나 서버일때 미션 버튼 비활성화(추가처리)
								{
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070529_0100,COLOR_ITEM);
									return INF_MSGPROC_NORMAL;
								}
								g_pInterface->SetWindowOrder(WNDMissionInfo);
								ShowMissionTreeWnd(TRUE, TRUE);								
							}							
							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
						}
						break;
					}
				}
			}
			break;
			// 2007-09-12 by bhsohn 2차 암호 시스템 구현
		case WM_SYSKEYDOWN:
			{
				if(WndProcSysKeyDown( wParam, lParam ) == INF_MSGPROC_BREAK)
					return INF_MSGPROC_BREAK;					
			}
			break;
			// end 2007-09-12 by bhsohn 2차 암호 시스템 구현		
		}
	}
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	if(WM_LBUTTONUP == uMsg)
	{
		if(m_stSelectItem.pSelectItem)
		{
			// 모든 선택 초기화
			SetToolTip(NULL, 0, 0);
			m_pInven->SetItemInfo(NULL, 0, 0);
			m_pInven->SetMultiSelectItem(NULL);	// 다중 처리 제거
			m_pInven->SetSelectItem(NULL);
		}
	}
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-10 by bhsohn 시스템 창 우선순위 추가
/// \date		2008-07-10 ~ 2008-07-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pChat)
	{
		if(m_pChat->WndProcSystemMsg(uMsg, wParam, lParam)==INF_MSGPROC_BREAK)
			return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}

int CINFGameMain::WndProcButtonMouseMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 2007-09-05 by dgwoo 알트누르고 몬스터 클릭시 인식하는 몬스터의 경우 이벤트 발생.
	// 알트키 시 메뉴 리스트 출력
	if(GetAsyncKeyState(VK_MENU))
	// 2009. 12. 03 by jskim 도시에서 출격 중에 알트키를 클릭 못하도록 수정
	//m_bMenuLock = TRUE;
	{
		const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
		if(IS_CITY_MAP_INDEX(myShuttleInfo.MapChannelIndex.MapIndex) && g_pShuttleChild->m_dwState != _LANDED)
		{
			m_bMenuLock = FALSE;
		}
		else
		{
		m_bMenuLock = TRUE;	
		}
	}
	//end 2009. 12. 03 by jskim 도시에서 출격 중에 알트키를 클릭 못하도록 수정
	else
		m_bMenuLock = FALSE;

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{	
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
			BOOL bShowMenu = TRUE;
			if(g_pInterface->m_pTarget->GetShowTargetCodition() == TRUE 
				&& !g_pD3dApp->IsPossibleWindowMove())
			{
				bShowMenu = FALSE;
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			for(int i=0; i < GAMEMAIN_BUTTON_NUMBER; i++)
			{
				m_pImgButton[i]->OnMouseMove(pt);
			}																					  

			// end 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
#else
			if( pt.y>GAMEMAIN_BUTTON_START_Y &&
				pt.y<GAMEMAIN_BUTTON_START_Y+GAMEMAIN_BUTTON_SIZE_Y &&
				//g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE) 
				bShowMenu)// 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
			{
				int index = (pt.x - GAMEMAIN_BUTTON_START_X)/GAMEMAIN_BUTTON_INTERVAL;
				if( index >= 0 && 
					index < GAMEMAIN_BUTTON_NUMBER &&
					pt.x < GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*index+GAMEMAIN_BUTTON_SIZE_X)
				{
					for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
					{
						if(i != index)
						{
							m_nButtonState[i] = BUTTON_STATE_NORMAL;
						}
					}
					if(m_nButtonState[index] != BUTTON_STATE_DOWN) m_nButtonState[index] = BUTTON_STATE_UP;
				}
			}
			else
			{
				for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) m_nButtonState[i] = BUTTON_STATE_NORMAL;
			}
#endif			
			// 2007-07-04 by bhsohn 오퍼레이터 추가
			{
				m_pFaqBtn->OnMouseMove(pt);		
			}
			// end 2007-07-04 by bhsohn 오퍼레이터 추가			
			{
				m_pCouponBtn->OnMouseMove(pt);
			}
			// 2009. 10. 14 by jskim 프리스카 제거
			m_pTut_SelBtn->OnMouseMove(pt);
			//end 2009. 10. 14 by jskim 프리스카 제거
		}
		break;
	case WM_LBUTTONDOWN:
		{
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
			BOOL bShowMenu = TRUE;
			if(g_pInterface->m_pTarget->GetShowTargetCodition() == TRUE 
				&& !g_pD3dApp->IsPossibleWindowMove())
			{
				bShowMenu = FALSE;
			}
			// end 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
  			if( pt.y>GAMEMAIN_BUTTON_START_Y &&
  				pt.y<GAMEMAIN_BUTTON_START_Y+m_pImgButtonBK->GetImgSize().y &&
 				//pInterface->m_pTarget->GetShowTargetCodition() == FALSE
 				bShowMenu ) // 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
 			{
 				int index = (pt.x - GAMEMAIN_BUTTON_START_X)/GAMEMAIN_BUTTON_INTERVAL;
//  				if( index >= 0 && 
//  					index < GAMEMAIN_BUTTON_NUMBER &&
//  					pt.x < GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*index+GAMEMAIN_BUTTON_SIZE_X)
				if( pt.x>GAMEMAIN_BUTTON_START_X &&
  					pt.x<GAMEMAIN_BUTTON_START_X+m_pImgButtonBK->GetImgSize().x)
				{
// 					// 2005-08-04 by ispark
// 					// 비행중일때 키를 막음
// 					// 2005-08-08 by ispark, 기어정지시에는 허용
// 					// 2006-08-09 by ispark, 상점 이용시에는 버튼 사용 금지 
 					if(//!g_pShuttleChild->GetIsUseInterface() || 
 						g_pD3dApp->m_dwGameState == _SHOP ||
 						g_pGameMain->m_pTrade->m_bTrading)
 						return INF_MSGPROC_BREAK;
// 
// 					// 2007-05-11 by bhsohn 아레나 버그수정
// 					// 아레나 일때는 미션버튼이 먹지 않게 함.
 					if(index == 3 && TRUE == g_pInterface->IsArenaStart())
 					{
 						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070529_0100,COLOR_ITEM);
 						return INF_MSGPROC_BREAK;
 					}
// 					
					for(int i=0; i < GAMEMAIN_BUTTON_NUMBER; i++)
					{
	 						if(m_pImgButton[i]->GetBtnState() == 3 && TRUE == g_pInterface->IsArenaStart())
	 						{
	 							g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070529_0100,COLOR_ITEM);
	 							return INF_MSGPROC_BREAK;
	 						}

						if(m_pImgButton[i]->OnLButtonDown(pt))
						{
							BOOL bShowTree = IsShowTreeWnd();
							//ShowMissionTreeWnd(bShowTree, TRUE);
							// 2013-01-03 by mspark, 메인 메뉴를 클릭하였을 경우 사운드가 겹쳐서 들리던 문제 수정
							ShowMissionTreeWnd(bShowTree, FALSE);
							// end 2013-01-03 by mspark, 메인 메뉴를 클릭하였을 경우 사운드가 겹쳐서 들리던 문제 수정
 						}
 					}
 				} 
			}
// // 					for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
// // 					{
// 						//m_nButtonState[i] = BUTTON_STATE_NORMAL;
// 					//}
// 					//m_nButtonState[index] = BUTTON_STATE_DOWN;
// 					if(index == 3)
// 					{						
// 						m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
// //						g_pGameMain->m_bQuestView = FALSE;
// //						m_pQuest->m_nQuestIndex = 0;		
// 						//m_nButtonState[index] = BUTTON_STATE_NORMAL;
// 						
// 						// 2007-06-12 by bhsohn 미션 인터페이스 수정안
// //						if(TRUE == m_pMissionInfo->GetMissionWindowState())
// //						{
// //							m_pMissionInfo->MissionProcFlag(FALSE);
// //							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
// //						}
// //						else
// //						{
// //							m_pMissionInfo->MissionProcFlag(TRUE);
// //							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
// //						}
// 						BOOL bShowTree = IsShowTreeWnd();
// 						bShowTree ^= TRUE;
// 						ShowMissionTreeWnd(bShowTree, TRUE);
// 						m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
// 
// 					}
// 
// 					// 2006-08-22 by ispark, 스탯 도움말
// 					if(index == 1)
// 					{
// 						m_bKnowON[LOW_LEVEL_STAT_HELP] = FALSE;
// 					}					
// 					return INF_MSGPROC_BREAK;
// 				}
// 			}
#else		
			if( pt.y>GAMEMAIN_BUTTON_START_Y &&
				pt.y<GAMEMAIN_BUTTON_START_Y+GAMEMAIN_BUTTON_SIZE_Y &&
				//g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE
				bShowMenu ) // 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
			{
				int index = (pt.x - GAMEMAIN_BUTTON_START_X)/GAMEMAIN_BUTTON_INTERVAL;
				if( index >= 0 && 
					index < GAMEMAIN_BUTTON_NUMBER &&
					pt.x < GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*index+GAMEMAIN_BUTTON_SIZE_X)
				{
					// 2005-08-04 by ispark
					// 비행중일때 키를 막음
					// 2005-08-08 by ispark, 기어정지시에는 허용
					// 2006-08-09 by ispark, 상점 이용시에는 버튼 사용 금지 
					if(!g_pShuttleChild->GetIsUseInterface() || 
						g_pD3dApp->m_dwGameState == _SHOP ||
						g_pGameMain->m_pTrade->m_bTrading)
						return INF_MSGPROC_BREAK;

					// 2007-05-11 by bhsohn 아레나 버그수정
					// 아레나 일때는 미션버튼이 먹지 않게 함.
					if(index == 3 && TRUE == g_pInterface->IsArenaStart())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070529_0100,COLOR_ITEM);
						return INF_MSGPROC_BREAK;
					}
					
					for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
					{
						m_nButtonState[i] = BUTTON_STATE_NORMAL;
					}
					m_nButtonState[index] = BUTTON_STATE_DOWN;
					if(index == 3)
					{						
						m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//						g_pGameMain->m_bQuestView = FALSE;
//						m_pQuest->m_nQuestIndex = 0;		
						m_nButtonState[index] = BUTTON_STATE_NORMAL;
						
						// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//						if(TRUE == m_pMissionInfo->GetMissionWindowState())
//						{
//							m_pMissionInfo->MissionProcFlag(FALSE);
//							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//						}
//						else
//						{
//							m_pMissionInfo->MissionProcFlag(TRUE);
//							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
//						}
						BOOL bShowTree = IsShowTreeWnd();
						bShowTree ^= TRUE;
						ShowMissionTreeWnd(bShowTree, TRUE);
						m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;

					}

					// 2006-08-22 by ispark, 스탯 도움말
					if(index == 1)
					{
						m_bKnowON[LOW_LEVEL_STAT_HELP] = FALSE;
					}					
					return INF_MSGPROC_BREAK;
				}
			}
#endif		
			// 2007-07-04 by bhsohn 오퍼레이터 추가
			{
				if(TRUE == m_pFaqBtn->OnLButtonDown(pt))
				{			
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			// end 2007-07-04 by bhsohn 오퍼레이터 추가
			{
				if(m_pCouponBtn->OnLButtonDown(pt))
				{
					return INF_MSGPROC_BREAK;
				}
			}
			// 2009. 10. 14 by jskim 프리스카 제거
			{
				if(g_pD3dApp->m_bCharacter)	// 2012-12-05 by mspark, 비행시에 튜토리얼 버튼 위치 클릭 시 미사일 발사 되지 않던 버그 수정
				{
					if(m_pTut_SelBtn->OnLButtonDown(pt))
					{
						return INF_MSGPROC_BREAK;
					}
				}	
			}
			//end 2009. 10. 14 by jskim 프리스카 제거
		}
		break;
	case WM_LBUTTONUP:
		{
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				return INF_MSGPROC_NORMAL;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt.x,pt.y);

			// 2007-07-04 by bhsohn 오퍼레이터 추가
			{
				if(TRUE == m_pFaqBtn->OnLButtonUp(pt))
				{	
					m_pChat->OnClickFAQ();
					// 버튼 클릭 
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			// end 2007-07-04 by bhsohn 오퍼레이터 추가
			// 2009. 10. 14 by jskim 프리스카 제거
			{
				if(TRUE == m_pTut_SelBtn->OnLButtonUp(pt))
				{	
					// 2009. 10. 28 by jskim 세력변경 후에도 튜토리얼 버튼은 사라지나 클릭 되는 버그 수정, 스트링 수정
					//g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070716_0100, _Q_TUTORIAL_START);		//"튜토리얼 모드로 이동하시겠습니까?"
					const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
					if(myShuttleInfo.InfluenceType == 1 && IS_CITY_MAP_INDEX(myShuttleInfo.MapChannelIndex.MapIndex) && g_pShuttleChild->m_dwState == _LANDED) 
					{
						//g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070716_0100, _Q_TUTORIAL_START);		//"튜토리얼 모드로 이동하시겠습니까?"
						// 2009. 10. 30 by jskim SHOP에 들어갔을때도 사라지게 수정 
						if(g_pD3dApp->m_dwGameState != _SHOP)
						{
							g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_070716_0100, _Q_TUTORIAL_START);		//"튜토리얼 모드로 이동하시겠습니까?"
						}
						//end 2009. 10. 30 by jskim SHOP에 들어갔을때도 사라지게 수정 
						return  INF_MSGPROC_BREAK;						
					}					
					//end 2009. 10. 28 by jskim 세력변경 후에도 튜토리얼 버튼은 사라지나 클릭 되는 버그 수정, 스트링 수정
				}
			}
			//end 2009. 10. 14 by jskim 프리스카 제거
			{
				if(m_pCouponBtn->OnLButtonUp(pt) &&
					(g_pShuttleChild->m_nBooster == BOOSTER_TYPE_AIRBREAK || g_pD3dApp->IsMyShuttleCharacter()))
				{//쿠폰 입력 윈도우 생성.
					ShowCouponWindow(TRUE);
				}
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경			
 			if( pt.y>GAMEMAIN_BUTTON_START_Y &&
 				pt.y<GAMEMAIN_BUTTON_START_Y+GAMEMAIN_BUTTON_SIZE_Y)
 			{
// 				int index = (pt.x - GAMEMAIN_BUTTON_START_X)/GAMEMAIN_BUTTON_INTERVAL;
// 				// 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
 				BOOL bShowMenu = TRUE;
 				if(g_pInterface->m_pTarget->GetShowTargetCodition() == TRUE 
 					&& !g_pD3dApp->IsPossibleWindowMove())
 				{
 					bShowMenu = FALSE;
 				}
			}
// 				// end 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
// 				if( index >= 0 && 
// 					index < GAMEMAIN_BUTTON_NUMBER &&
// 					pt.x < GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*index+GAMEMAIN_BUTTON_SIZE_X &&
// 					//g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE &&
// 					bShowMenu == TRUE && // 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
// 					g_pD3dApp->m_dwGameState != _SHOP)	// 2005-12-06 by ispark
// 				{
// // 					if(m_nButtonState[index] == BUTTON_STATE_DOWN)
// // 					{
// // 						if(m_nButtonState[3] == BUTTON_STATE_DOWN)
// // 						{
// // 							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
// // 							
// // 							g_pGameMain->m_bQuestView = FALSE;
// // 							m_pQuest->m_nQuestIndex = 0;
// // 						}
 						
// // 						for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
// // 							m_nButtonState[i] = BUTTON_STATE_NORMAL;
// // 						m_nButtonState[index] = BUTTON_STATE_UP;
// // 						return INF_MSGPROC_BREAK;
// // 					}
// // 					for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
// // 						m_nButtonState[i] = BUTTON_STATE_NORMAL;
// // 					m_nButtonState[index] = BUTTON_STATE_UP;
// 				}
// 			}

			///////////////////////////////////////////////////////
			// 메뉴 리스트 삭제

			if(g_pShuttleChild->m_bUnitStop || g_pShuttleChild->m_dwState == _LANDED 
				|| (IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE && g_pShuttleChild->m_bIsAir == FALSE) 
				|| g_pD3dApp->m_bCharacter)	// 2012-11-23 by mspark, 비행시에 메인버튼 클릭 방지
			{
				for(int i=0; i < GAMEMAIN_BUTTON_NUMBER; i++)
				{
					if(m_pImgButton[i]->OnLButtonUp(pt))
					{
						if(i == 3)
						{
							if(ARENA_STATE_ARENA_GAMING == g_pD3dApp->GetArenaState()) // 2013-05-02 by ssjung 유저가 아레나 서버일때 미션 버튼 비활성화
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070529_0100,COLOR_ITEM);  // 2013-08-05 by ssjung 유저가 아레나 서버일때 미션 버튼 비활성화(추가처리)
								continue;
							}

							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
							//						g_pGameMain->m_bQuestView = FALSE;
							//						m_pQuest->m_nQuestIndex = 0;		
							//m_nButtonState[index] = BUTTON_STATE_NORMAL;
							
							// 2007-06-12 by bhsohn 미션 인터페이스 수정안
							//						if(TRUE == m_pMissionInfo->GetMissionWindowState())
							//						{
							//							m_pMissionInfo->MissionProcFlag(FALSE);
							//							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
							//						}
							//						else
							//						{
							//							m_pMissionInfo->MissionProcFlag(TRUE);
							//							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
							//						}
							
							// 2013-04-02 by ssjung 미션을 클릭 하였을때 레이더뒤로 레이어가 덮히는 현상 수정
// 							BOOL bShowTree = IsShowTreeWnd();
// 							bShowTree ^= TRUE;
// 							ShowMissionTreeWnd(bShowTree, TRUE);
// 							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
							
							BOOL bShowTree = IsShowTreeWnd();
							if(bShowTree)
							{
								ShowMissionTreeWnd(FALSE, TRUE);								
							}
							else
							{
								g_pInterface->SetWindowOrder(WNDMissionInfo);
								ShowMissionTreeWnd(TRUE, TRUE);							
							}							
							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
						}
						else
						{
							OnGameMainButtonClicked(i);
						}					
					}
				}
			}			
#else 
			if( pt.y>GAMEMAIN_BUTTON_START_Y &&
				pt.y<GAMEMAIN_BUTTON_START_Y+GAMEMAIN_BUTTON_SIZE_Y)
			{
				int index = (pt.x - GAMEMAIN_BUTTON_START_X)/GAMEMAIN_BUTTON_INTERVAL;
				// 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
				BOOL bShowMenu = TRUE;
				if(g_pInterface->m_pTarget->GetShowTargetCodition() == TRUE 
					&& !g_pD3dApp->IsPossibleWindowMove())
				{
					bShowMenu = FALSE;
				}
				// end 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
				if( index >= 0 && 
					index < GAMEMAIN_BUTTON_NUMBER &&
					pt.x < GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*index+GAMEMAIN_BUTTON_SIZE_X &&
					//g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE &&
					bShowMenu == TRUE && // 2009-02-02 by bhsohn 마우스 없어지는 버그 수정
					g_pD3dApp->m_dwGameState != _SHOP)	// 2005-12-06 by ispark
				{
					if(m_nButtonState[index] == BUTTON_STATE_DOWN)
					{
						if(m_nButtonState[3] == BUTTON_STATE_DOWN)
						{
							m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
							
							g_pGameMain->m_bQuestView = FALSE;
							m_pQuest->m_nQuestIndex = 0;
						}
						OnGameMainButtonClicked(index);
						for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
							m_nButtonState[i] = BUTTON_STATE_NORMAL;
						m_nButtonState[index] = BUTTON_STATE_UP;
						return INF_MSGPROC_BREAK;
					}
					for(int i=0;i<GAMEMAIN_BUTTON_NUMBER;i++) 
						m_nButtonState[i] = BUTTON_STATE_NORMAL;
					m_nButtonState[index] = BUTTON_STATE_UP;
				}
			}
#endif
			if(m_bMenuListFlag)
			{
				BOOL bFlag = FALSE;

				if( pt.x>(m_pMenuList->m_nMenuListStartX)
					&& pt.x<(m_pMenuList->m_nMenuListStartX+MENU_LIST_WIDTH))
				{
					// 버튼
					for(int i=0; i<MENU_LIST_ALL_COUNT; i++)
					{
						if( pt.y>(m_pMenuList->m_nMenuListStartY+(i*MENU_LIST_HEIGHT))
							&& pt.y<m_pMenuList->m_nMenuListStartY+MENU_LIST_HEIGHT+(i*MENU_LIST_HEIGHT))
						{
							// 메세지 처리
							bFlag = TRUE;													
						}
					}
				}
				if(!bFlag)
				{
					m_bMenuListFlag = FALSE;
				}				
			}

			// 메뉴 리스트 타캐릭 아이디 가져오기
			if(m_bMenuLock)
			{
				// 2007-10-24 by dgwoo 몬스터를 먼저 클릭 인식할수 있도록 변경.
				CMonsterData * pMon = NULL;
				pMon = g_pScene->GetMonsterInfo2D(vPos1);
				if(pMon)
				{
					MonsterAltClick(pMon);
				}
				else
				{
					CEnemyData * pTp = NULL;
					// 캐릭터 alt + click 시 단축키.
					pTp = g_pScene->GetEnemyCharaterID(vPos1);
					if(pTp)
					{
						if(CheckPopUpMenu(vPos1, pTp))
						{
							EnemyAltClick(pTp,pt);
						}
					}
				}
			}
			//
			////////////////////////////////////////////////////
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMain::MonsterAltClick(CMonsterData * pMon)
/// \brief		몬스터를 alt + click시 처리.
/// \author		dgwoo
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::MonsterAltClick(CMonsterData * pMon)
{
	// 클릭을 할수있는 몬스터만 인식하여 이벤트 처리한다.
	if(pMon->m_pMonsterInfo->ClickEvent == 0)
	{
		return;
	}
	// 2007-09-06 by dgwoo 전진기지는 
	if(GetSummonMotherShipCnt() > 0)
	{// 모선전 중이므로 지도자 부지도자 만을 인식하도록 한다.
		
		if(BELL_OUTPOST_RESET_VCN == pMon->m_pMonsterInfo->Belligerence
			|| BELL_OUTPOST_RESET_ANI == pMon->m_pMonsterInfo->Belligerence)
		{
			// 몬스터 타입이 각인이다.
			if(m_pOutPost->IsOutPosting() && g_pGameMain->m_pCommunity->GetGuild()->GetMyGuildRank() == GUILD_RANK_COMMANDER)
			{
				// 전진기지전이며, 길드장만 클릭가능
				MSG_FC_OUTPOST_RESET_START sMsg;
				sMsg.TargetIndex = pMon->m_info.MonsterIndex;
				g_pFieldWinSocket->SendMsg(T_FC_OUTPOST_RESET_START,(char*)&sMsg,sizeof(sMsg));	
			}			
		}		
		else 
		{
			// 텔레포트 몬스터이다.
			if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_INFLUENCE_LEADER) ||
				COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_INFLUENCE_SUBLEADER_MASK))
			{//지도자 && 부지도자.
				MSG_FC_EVENT_CLICK_TELEPORT sMsg;
				sMsg.TargetIndex = pMon->m_info.MonsterIndex;
				sMsg.MapIndex = g_pShuttleChild->GetMyShuttleInfo().MapChannelIndex.MapIndex;
				g_pFieldWinSocket->SendMsg(T_FC_EVENT_CLICK_TELEPORT,(char*)&sMsg,sizeof(sMsg));	
			}
		}
		// end 2008-09-08 by bhsohn 모선전시, 전진기지 각인 안되는 문제 해결
	}
	else if(m_pOutPost->IsOutPosting())
	{// 전진기지전이 진행중이므로 여단장만 인식하도록 한다.
		// 2008-09-08 by bhsohn 모선전시, 전진기지 각인 안되는 문제 해결
		if(BELL_OUTPOST_RESET_VCN == pMon->m_pMonsterInfo->Belligerence
			|| BELL_OUTPOST_RESET_ANI == pMon->m_pMonsterInfo->Belligerence)
		{			
			if(g_pGameMain->m_pCommunity->GetGuild()->GetMyGuildRank() == GUILD_RANK_COMMANDER)
			{// 여단장이면  
				MSG_FC_OUTPOST_RESET_START sMsg;
				sMsg.TargetIndex = pMon->m_info.MonsterIndex;
				g_pFieldWinSocket->SendMsg(T_FC_OUTPOST_RESET_START,(char*)&sMsg,sizeof(sMsg));	
			}
		}
		// end 2008-09-08 by bhsohn 모선전시, 전진기지 각인 안되는 문제 해결
	}
}
void CINFGameMain::EnemyAltClick(CEnemyData * pTp,POINT pt)
{
	m_pMenuList->m_vecpTargetEnemy = pTp;
//					m_pMenuList->m_vecpTargetEnemy = g_pGameMain->GetEnemyCharaterID(pt);
	strcpy(m_pMenuList->m_strOtherName,pTp->m_infoCharacter.CharacterInfo.CharacterName);
	if(strlen(m_pMenuList->m_strOtherName)>=1)// 2005-12-06 by ispark, 아이디 하나포함해서 이상
	{
		// 길드 등록 길마만 사용가능
		if(m_pCommunity->GetGuild() &&
			m_pCommunity->GetGuild()->GetGuildInfo() &&
			m_pCommunity->GetGuild()->GetGuildInfo()->GuildCommanderUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber )
		{
			m_pMenuList->m_bFALSEMenu[MENU_LIST_INGU] = FALSE;
		}
		else
		{
			m_pMenuList->m_bFALSEMenu[MENU_LIST_INGU] = TRUE;
		}
		
		// 파티 등록 파장만 사용가능
		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
		{
			m_pMenuList->m_bFALSEMenu[MENU_LIST_INPA] = FALSE;
		}
		else
		{
			m_pMenuList->m_bFALSEMenu[MENU_LIST_INPA] = TRUE;
		}

		// 2006-09-27 by ispark, 도시에서 PVP 금지
		if(g_pD3dApp->m_bCharacter == TRUE)
		{
			m_pMenuList->m_bFALSEMenu[MENU_LIST_INPV] = TRUE;
		}
		else
		{
			m_pMenuList->m_bFALSEMenu[MENU_LIST_INPV] = FALSE;
		}

		// 2011. 01. 19 by jskim 배트남 PK 없애기 
#ifdef LANGUAGE_VIETNAM
		m_pMenuList->m_bFALSEMenu[MENU_LIST_INPV] = TRUE;
#endif
		// end 2011. 01. 19 by jskim 배트남 PK 없애기

		g_pInterface->SetWindowOrder(WNDMenuList);
		m_bMenuListFlag = TRUE;						
		m_pMenuList->m_nMenuListStartX = pt.x;
		m_pMenuList->m_nMenuListStartY = pt.y;
		m_pMenuList->MenuListSort();
		
		// 2012-07-12 by isshin 아템미리보기 On / Off 기능
		#ifdef	C_ENEMY_INFO_ISSHIN		
		if(m_pMenuList->GetItemInfoSecretDone() == TRUE)
		{
			MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET sMsg;
			sMsg.TargetCharcterUID = pTp->m_infoCharacter.CharacterInfo.CharacterUniqueNumber;
			g_pFieldWinSocket->SendMsg(T_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET,(char*)&sMsg,sizeof(sMsg));
			m_pMenuList->SetItemInfoSecretDone(FALSE);
		}		
		// end 2012-07-12 by isshin 아템미리보기 On / Off 기능
		#endif
	}
	else
		m_bMenuListFlag = FALSE;
}

void CINFGameMain::OnGameMainButtonClicked(int nButton)
{
	switch(nButton)
	{
	case GAMEMAIN_BUTTON_INVEN:
		{
			if(m_nLeftWindowInfo == LEFT_WINDOW_INVEN)
			{
				SetItemInfo(0,0,0,0);
			}
			LeftWindowShow(TRUE, LEFT_WINDOW_INVEN);
		}
		break;
	case GAMEMAIN_BUTTON_INFO:
		{	
			if(m_nRightWindowInfo != RIGHT_WINDOW_INFO)
			{				
				// 2008-09-22 by bhsohn EP3 캐릭터 창
				m_pCharacterInfo->SetCharacterInfoType(CHARACTER_INFO);
				RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
			}
			else
			{
				RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
			}
		}
		break;
	case GAMEMAIN_BUTTON_COMMUNITY:
		{
			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			//LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
			OnClickCommunity();
		}
		break;
	case GAMEMAIN_BUTTON_TRADE:
		{
			if(m_nRightWindowInfo != RIGHT_WINDOW_INFO)
			{
				// 2007-06-07 by dgwoo 필요 없음.
				//m_pCharacterInfo->m_nCharacterInfoType = CHARACTER_ARENA;
				RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
			}
			else
			{
				RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
			}
//			if(!m_pTrade->m_bTrading)
//			{
//				m_pInfWindow->AddMsgBox( "거래할 유닛을 입력하시오", _Q_TRADE_INPUT_NAME);
//			}
//			else
//			{
//				m_pInfWindow->AddMsgBox("거래를 취소하시겠습니까?", _Q_TRADE_CANCEL);
//			}
		}
		break;
	case GAMEMAIN_BUTTON_SYSTEM:
		{
			// 2008-03-19 by bhsohn Ep3옵션창
			//RightWindowShow(TRUE, RIGHT_WINDOW_OPTION);
			PopupSystemOptinWnd();
		}
		break;
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \class		
/// \brief		nWindow - 현재 컨트롤할 윈도우창..무기,인벤,파티 등등..
///
/// \author		dgwoo
/// \version	
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::LeftWindowShow(BOOL bHide, int nWindow)
{
	FLOG( "CINFGameMain::LeftWindowShow(BOOL bHide, int nWindow)");
	// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	if(bHide && LEFT_WINDOW_INVEN == nWindow)
	{
		// 2파 패스워드 활성하면 인벤 안열리게 함.
		if(IsUseSecondPassword())
		{
			char ErrorMsgMissionList[256];
			wsprintf(ErrorMsgMissionList, STRMSG_C_070917_0213);
			g_pD3dApp->m_pChat->CreateChatChild(ErrorMsgMissionList, COLOR_ERROR); //"\\y보안 시스템이 활성화 되었습니다."
			return;
		}
		// 2008-12-04 by bhsohn 무한으로 총알 나가는 현상 처리
		if(g_pShuttleChild && g_pShuttleChild->IsWeapongShotting())
		{
			// 총알 쏘고있을떄는 메뉴가 안열리게
		//	return;		  // 2012-09-20 by jhahn A기어 인벤 버튼 클릭 가능 
		}
		// end 2008-12-04 by bhsohn 무한으로 총알 나가는 현상 처리
	}
	// end 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	
	// 2008-08-22 by bhsohn EP3 인벤토리 처리	
	//g_pShuttleChild->m_bInvenRender = FALSE;
	
	g_pShuttleChild->InitInvenMesh();

	// 죽었을때 I,V 키 사용을 막는다
	vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
	while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
	{
		CINFMessageBox* pMsgBox = *it;
		if(	_Q_DEAD_RETURN == pMsgBox->m_nMsgType || _Q_USE_SUPER_RESTORE == pMsgBox->m_nMsgType ||
			_Q_USE_NORMAL_RESTORE == pMsgBox->m_nMsgType || _Q_REVIVE_WAIT_5SECOND == pMsgBox->m_nMsgType )
		{
			return;
		}
		it++;
	}

//	m_nLeftWindowY = DEAULT_WINDOW_POS_Y;
//	m_nLeftWindowY = (300.0f*(float)g_pD3dApp->GetBackBufferDesc().Height / 600.0f) - (SIZE_WEAPON_Y / 2);
	m_nLeftWindowY = DEAULT_WINDOW_POS_Y - m_nLeftWindowScaleSize/2;
	g_pGameMain->m_pInven->InitInven();							// 2005-12-08 by ispark, 초기화
	
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
//	if(nWindow == LEFT_WINDOW_INVEN)
//	{		
//		m_pInfWindow->m_nLeftWindowInfo = 1;
//	}
//	else if(nWindow == LEFT_WINDOW_TRANS)
//	{		
//		m_pInfWindow->m_nLeftWindowInfo = 1;
//	}
//	else
//		m_pInfWindow->m_nLeftWindowInfo = 0;

	m_pInfWindow->m_nLeftWindowInfo = 0;
	
	switch(nWindow)
	{
	case LEFT_WINDOW_INVEN:
		{
			BOOL bShowWnd = m_pInven->IsShowInven();
			bShowWnd ^= TRUE;		
			if(bShowWnd)
			{

				
				// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				g_pGameMain->m_bChangeMousePoint = TRUE;
				// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				POINT ptItem;
				ptItem.x = g_pD3dApp->GetBackBufferDesc().Width/2 - m_pInven->GetBkSize().x / 2;
				ptItem.y = g_pD3dApp->GetBackBufferDesc().Height/2 - m_pInven->GetBkSize().y / 2; 	
                
                m_pInven->ShowInven(&ptItem, NULL, TRUE);	
#else 
				POINT ptItem, ptEq;
				ptItem.x = INVEN_ITEM_START_X;
				ptItem.y = INVEN_ITEM_START_Y;
				
				ptEq.x = INVEN_EQ_START_X;
				ptEq.y = INVEN_EQ_START_Y;
				
				m_pInven->ShowInven(&ptItem, &ptEq, TRUE);	
#endif
				
				
				
				g_pInterface->SetWindowOrder(WNDInvenWnd);
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);
			}
			else
			{
				m_pInven->ShowInven(NULL, NULL, TRUE);	

				// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				//if(g_INFCnt==0)
				if(g_pD3dApp->GetINFCnt() == 0)
				{
					g_pGameMain->m_bChangeMousePoint = FALSE;
				}
				// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
				// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);

			}
		}
		break;
	case LEFT_WINDOW_TRANS:
		{
			POINT ptItem, ptEq;			
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			ptItem.x = 30;
			ptItem.y = (DEAULT_WINDOW_POS_Y-327);
			ptEq.x = 426;
			ptEq.y = (DEAULT_WINDOW_POS_Y-400);
#else
			ptItem.x = 0;
			ptItem.y = (DEAULT_WINDOW_POS_Y-127);
			ptEq.x = 426;
			ptEq.y = (DEAULT_WINDOW_POS_Y-400);
#endif
			
			m_pInven->ShowInven(&ptItem, &ptEq);	
			
			g_pInterface->SetWindowOrder(WNDInvenWnd);
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);			
		}
		break;
	}	
	
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리

	g_pInterface->m_pCityBase->CloseAllMessageBox();
	// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
//	if(g_pGameMain->m_pInven->m_pDeleteItem != NULL)
//		g_pGameMain->m_pInven->m_pDeleteItem = NULL;
	g_pGameMain->m_pInven->InitDeleteItemInfo();
	
// 2006-07-27 by ispark, 선택 아이템 초기화
	g_pGameMain->m_pQuickSlot->m_nSelectSlotNumber = -1;
	g_pGameMain->m_pQuickSlot->m_pSelectItem.pItem = NULL;
//	g_pGameMain->m_pInven->m_pSelectItem = NULL;
//	g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//	g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//	g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
	POINT ptNull;
	ptNull.x = ptNull.y = 0;
	SetSelectItem(NULL, ptNull, ITEM_NOT_POS);

	if( m_nLeftWindowInfo != nWindow &&
		g_pD3dApp->m_dwGameState == _CITY &&
		g_pInterface->m_pCityBase->GetCurrentBuildingNPC() != NULL)
	{
		g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
	}
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	//if(m_nLeftWindowInfo != nWindow)
	if(m_nLeftWindowInfo != nWindow && (LEFT_WINDOW_INVEN != nWindow))
	{
		if(m_nLeftWindowInfo == LEFT_WINDOW_WEAPON)
		{
			m_nLeftWindowState = LEFT_WEAPON_WINDOW_HIDING ;
			m_nLeftWindowInfo = nWindow;
			m_bLeftWindowModeChage = TRUE;
//			g_pD3dApp->m_bFixUnitDirMode = TRUE;	// 유닛 방향 고정 모드
		}
		else if(bHide)// hiding animation
		{
			m_nLeftWindowState = LEFT_WINDOW_HIDING ;
			m_nLeftWindowInfo = nWindow;
			m_bLeftWindowModeChage = TRUE;
			g_pD3dApp->m_bChatMode = FALSE;
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);
		}
		else
		{
			m_nLeftWindowInfo = nWindow;
		}
	}
	else if(m_nLeftWindowInfo != LEFT_WINDOW_WEAPON)// weapon
	{
		m_nLeftWindowState = LEFT_WINDOW_HIDING ;
		m_nLeftWindowInfo = LEFT_WINDOW_WEAPON;
		m_bLeftWindowModeChage = TRUE;
		SetToolTip(0,0,NULL);
		SetItemInfo(0,0,0,0);
		m_pInfWindow->m_nLeftWindowInfo = 0;
		m_pInfWindow->m_bLeftWindow = FALSE;
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
//		g_pD3dApp->m_bFixUnitDirMode = FALSE;	// 유닛 방향 고정 모드
	}

	// 상점에서는 사용금지
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return;
	// 셔틀방향 초기화 
	g_pShuttleChild->MirrorTurnOrig();	
}
void CINFGameMain::RightWindowShow(BOOL bHide, int nWindow)
{
	FLOG( "CINFGameMain::RightWindowShow(BOOL bHide, int nWindow)" );

	// 2007-01-29 by bhsohn 신규유저 미션 인터페이스
	if((TRUE == bHide)&&(RIGHT_WINDOW_INFO == nWindow))
	{		
		if(m_bKnowON[LOW_LEVEL_STAT_HELP])
		{
			m_bKnowON[LOW_LEVEL_STAT_HELP] = FALSE;
		}
	}
	// end 2007-01-29 by bhsohn 신규유저 미션 인터페이스

	// 2008-09-22 by bhsohn EP3 캐릭터 창
	if((TRUE == bHide) && RIGHT_WINDOW_INFO == nWindow)
	{
		BOOL bShow = m_pCharacterInfo->IsShowWnd();
		bShow ^= TRUE;
		m_pCharacterInfo->ShowCharacterInfoWnd(bShow);		
	}
	// end 2008-09-22 by bhsohn EP3 캐릭터 창
	
	// 죽었을때 I,V 키 사용을 막는다
	vector<CINFMessageBox*>::iterator it = g_pGameMain->m_pInfWindow->m_vecMessageBox.begin();
	while(it != g_pGameMain->m_pInfWindow->m_vecMessageBox.end())
	{
		CINFMessageBox* pMsgBox = *it;
		if(	_Q_DEAD_RETURN == pMsgBox->m_nMsgType || _Q_USE_SUPER_RESTORE == pMsgBox->m_nMsgType ||
			_Q_USE_NORMAL_RESTORE == pMsgBox->m_nMsgType || _Q_REVIVE_WAIT_5SECOND == pMsgBox->m_nMsgType )
		{
			return;
		}
		it++;
	}
	g_pInterface->m_pCityBase->CloseAllMessageBox();

	// 2007-06-20 by bhsohn 아이템 삭제시, 메모리 버그 수정
//	if(g_pGameMain->m_pInven->m_pDeleteItem != NULL)
//		g_pGameMain->m_pInven->m_pDeleteItem = NULL;
	g_pGameMain->m_pInven->InitDeleteItemInfo();
	
	// 2006-07-27 by ispark, 선택 아이템 초기화
	g_pGameMain->m_pQuickSlot->m_nSelectSlotNumber = -1;
	g_pGameMain->m_pQuickSlot->m_pSelectItem.pItem = NULL;
//	g_pGameMain->m_pInven->m_pSelectItem = NULL;
//	g_pGameMain->m_pInven->m_pSelectIcon = NULL;
//	g_pGameMain->m_pCharacterInfo->m_pSelectSkill = NULL;
//	g_pGameMain->m_pCharacterInfo->m_pSelectIcon = NULL;
	POINT ptNull;
	ptNull.x = ptNull.y = 0;
	SetSelectItem(NULL, ptNull, ITEM_NOT_POS);
	
	if( m_nRightWindowInfo != nWindow &&
		g_pD3dApp->m_dwGameState == _CITY &&
		g_pInterface->m_pCityBase->GetCurrentBuildingNPC() != NULL)
	{
		g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
	}
	if(m_nRightWindowInfo != nWindow)
	{
		if(m_nRightWindowInfo == RIGHT_WINDOW_WEAPON)
		{
			// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
			// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			//g_INFCnt++;
			g_pD3dApp->AddINFCnt();
			// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			g_pGameMain->m_bChangeMousePoint = TRUE;
			// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
			m_nRightWindowState = RIGHT_WEAPON_WINDOW_HIDING ;
			m_nRightWindowInfo = nWindow;
			m_bRightWindowModeChage = TRUE;
//			g_pD3dApp->m_bFixUnitDirMode = TRUE;	// 유닛 방향 고정 모드
		}
		else if(bHide)
		{

			m_nRightWindowState = RIGHT_WINDOW_HIDING ;
			m_nRightWindowInfo = nWindow;
			m_bRightWindowModeChage = TRUE;
			g_pD3dApp->m_bChatMode = FALSE;
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
		}
		else
		{
			m_nRightWindowInfo = nWindow;
		}
	}
	else if(m_nRightWindowInfo != RIGHT_WINDOW_WEAPON)
	{
		// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
		// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		//g_INFCnt--;
		//if(g_INFCnt==0)
		if(g_pD3dApp->RemoveINFCnt() == 0)
		{
			g_pGameMain->m_bChangeMousePoint = FALSE;
		}		
		// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
		// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
		m_nRightWindowState = RIGHT_WINDOW_HIDING ;
		m_nRightWindowInfo = RIGHT_WINDOW_WEAPON;
		m_bRightWindowModeChage = TRUE;
		
		SetToolTip(0,0,NULL);
		SetItemInfo(0,0,0,0);
		
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
	}
	
	// 상점에서는 사용금지
	if(g_pD3dApp->m_dwGameState == _SHOP)
	{
		m_nLeftWindowInfo = LEFT_WINDOW_SHOP;
		m_nRightWindowState = RIGHT_WINDOW_HIDING ;
		m_bRightWindowModeChage = TRUE;
		SetToolTip(0,0,NULL);
		SetItemInfo(0,0,0,0);

		g_pGameMain->m_pInfWindow->m_nLeftWindowInfo = 0;
		return;
	}
	// 셔틀방향 초기화 
	g_pShuttleChild->MirrorTurnOrig();	
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

}
/*
void CINFGameMain::OnButtonClick(int nButton)
{
	FLOG( "CINFGameMain::OnButtonClick(int nButton)" );
	switch(nButton)
	{
	case 0:// info
		{
			RightWindowShow(TRUE, RIGHT_WINDOW_INFO);
		}
		break;
	case 1:// inven
		{
			LeftWindowShow(TRUE, LEFT_WINDOW_INVEN);

		}
		break;
	case 2:// party
		{
			LeftWindowShow(TRUE, LEFT_WINDOW_PARTY);
		}
		break;
	case 3:// trans
		{
			if(!m_pTrade->m_bTrading)
			{
				m_pInfWindow->AddMsgBox( "거래할 유닛을 입력하시오", _Q_TRADE_INPUT_NAME);
			}
			else
			{
				m_pInfWindow->AddMsgBox( "거래를 취소하시겠습니까?", _Q_TRADE_CANCEL);
			}
//			LeftWindowShow(TRUE, LEFT_WINDOW_TRANS);
		}
		break;
	case 4:// option
		{
			RightWindowShow(TRUE, RIGHT_WINDOW_OPTION);
		}
		break;
	}

}
*/


void CINFGameMain::SetToolTip(int x, int y, char* strToolTip)
{
	FLOG( "CINFGameMain::SetToolTip(int x, int y, char* strToolTip)" );
	if(strToolTip)
	{
		memset ( m_strToolTip , 0 , sizeof( m_strToolTip ) );

		// 2009. 05. 28 by ckPark 툴팁스트링 메모리 넘어가는 에러 체크
		//strcpy(m_strToolTip, strToolTip);
		memset( m_strToolTip, 0, sizeof(m_strToolTip) );
		strncpy(m_strToolTip, strToolTip, 127);
		// end 2009. 05. 28 by ckPark 툴팁스트링 메모리 넘어가는 에러 체크

		if(strlen(m_strToolTip)*6+x > g_pD3dApp->GetBackBufferDesc().Width)
		{
			x = g_pD3dApp->GetBackBufferDesc().Width - strlen(m_strToolTip)*6;
		}
	}
	else
	{
		m_strToolTip[0] = NULL;
	}
	m_ptToolTip.x = x;
	m_ptToolTip.y = y;
}


// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
/****************************************************************
**
**	개행 적용 툴팁 설정.
**
**	Create Info :	2010. 05. 03.	hsLee.
**
*****************************************************************/
void CINFGameMain :: SetToolTipEx ( int x , int y , char* pszToolTip , PRECT prcBound /*= NULL*/ )
{

	FLOG ( "CINFGameMain :: SetToolTipEx ( int x , int y , char* pszToolTip )" );

	if ( NULL == pszToolTip || pszToolTip[0] == 0 )
	{
		ResetToolTipEx();
	}
	else
	{
		if ( strcmp ( m_szSave_ToolTip , pszToolTip ) )
		{
			ResetToolTipEx();

			strncpy ( m_szSave_ToolTip , pszToolTip ,  min( strlen( pszToolTip ) , sizeof( m_szSave_ToolTip ) - 1 ) );

			//STRING_CULL( m_szSave_ToolTip , 1024 , &m_vecToolTip , m_pFontToolTip );
			StringCullingUserDataEx ( m_szSave_ToolTip , 1024 , &m_vecToolTip , m_pFontToolTip , TRUE );

			if ( m_vecToolTip.size() > 0 )
			{
				vector < string >::iterator it = m_vecToolTip.begin();

				for ( ; it != m_vecToolTip.end(); it++ )
				{
					m_iToolTipMaxWid = max ( m_iToolTipMaxWid , m_pFontToolTip->GetStringSize( (char*)it->c_str() ).cx );
				}
			}

			m_bRenderToolTipEx = TRUE;
		}
		else
		{
			m_bRenderToolTipEx = TRUE;
		}

		m_dwFrameToolTip[1] = m_dwFrameToolTip[0];

		if ( m_iToolTipMaxWid+x > g_pD3dApp->GetBackBufferDesc().Width )
			x = g_pD3dApp->GetBackBufferDesc().Width - m_iToolTipMaxWid;

		m_ptToolTipEx.x = x;
		m_ptToolTipEx.y	= y;

		if ( prcBound )
			m_rcToolTipEx = *prcBound;
	}

}


/******************************************************
**
**	툴팁 버퍼 초기화. ( 개행 툴팁 맴버. )
**
**	Create Info :	2010. 05. 03.	hsLee.
**
*******************************************************/
void CINFGameMain :: ResetToolTipEx ( void )
{

	m_vecToolTip.clear();

	memset ( m_szSave_ToolTip , 0x00 , sizeof( m_szSave_ToolTip ) );

	m_iToolTipMaxWid = 0;
	m_bRenderToolTipEx = FALSE;

	SetRectEmpty ( &m_rcToolTipEx );

}
// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//void CINFGameMain::SetItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount/*=0*/, BOOL bShowMyEq/*=TRUE*/)
void CINFGameMain::SetItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount/*=0*/, BOOL bShowMyEq/*=TRUE*/, 
							   int nLinkItem/*=0*/,  BOOL bArmorCollect /*FALSE*/, BOOL bInven /*FALSE*/) // 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시	
							   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{
	FLOG( "CINFGameMain::SetItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y)" );
	if(nItemNum != 0)
	{
		ITEM *pITEM = g_pDatabase->GetServerItemInfo(nItemNum);
		if(pITEM == NULL)
		{
			// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
			//m_pItemInfo->SetItemInfoNormal(NULL,0,0);			
			SetItemInfoNormal(NULL,0,0);			
			// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
			return;
		}

		if(nUniqueNumber != 0)
		{
			if(IS_SKILL_ITEM(pITEM->Kind) == FALSE)
			{
				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( nUniqueNumber );
				if(pItemInfo)
				{
					// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
					//m_pItemInfo->SetItemInfoUser( pItemInfo, x, y );
					SetItemInfoUser( pItemInfo, x, y, bShowMyEq,0,bInven); // 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색		 
					// 2013-06-26 by ssjung 인벤토리 추가 툴팁 
					// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				}
			}
			else
			{
				// SKILL
//				ITEM* pSKILL = g_pShuttleChild->m_pSkill->FindItem( nItemNum );
//				if(pSKILL)
//				{
					// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
					//m_pItemInfo->SetItemInfoNormal( pITEM, x, y, FALSE, nItemCount );
					SetItemInfoNormal( pITEM, x, y, FALSE, nItemCount, bShowMyEq,0, bArmorCollect);	// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
					// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
// 				}
			}
		}
		else
		{
//			if(pITEM)
//			{
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//m_pItemInfo->SetItemInfoNormal( pITEM, x, y, TRUE, nItemCount );
				SetItemInfoNormal( pITEM, x, y, TRUE, nItemCount, bShowMyEq, nLinkItem,bArmorCollect);// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
				// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
//			}
		}
	}
	else
	{
		//m_pInfWindow->SetItemInfo(NULL,NULL,NULL,0,0);
		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
		//m_pItemInfo->SetItemInfoNormal(NULL,0,0);
		SetItemInfoNormal(NULL,0,0);
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	}
}

// 2012-06-14 by isshin 아템미리보기
void CINFGameMain::SetEnemyItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y, int nItemCount/*=0*/, BOOL bShowMyEq/*=TRUE*/)
{
	FLOG( "CINFGameMain::SetEnemyItemInfo(LONGLONG nUniqueNumber, int nItemNum, int x, int y)" );
	if(nItemNum != 0)
	{
		ITEM *pITEM = g_pDatabase->GetServerItemInfo(nItemNum);
		if(pITEM == NULL)
		{		
			SetItemInfoNormal(NULL,0,0);
			return;
		}
		
		if(nUniqueNumber != 0)
		{
			if(IS_SKILL_ITEM(pITEM->Kind) == FALSE)
			{
				CItemInfo* pItemInfo = g_pStoreData->FindItemInEnemyByUniqueNumber( nUniqueNumber );
				if(pItemInfo)
				{
					// 2013-06-21 by ssjung 적의 유저정보에서 아머컬렉션 정보가 보이지 않도록 수정
					pItemInfo->FixedTermShape.nStatLevel = 0;		
					pItemInfo->FixedTermShape.bActive = FALSE;
					// end 2013-06-21 by ssjung 적의 유저정보에서 아머컬렉션 정보가 보이지 않도록 수정

					SetItemInfoUser( pItemInfo, x, y, bShowMyEq);
				}
			}
			else
			{
				SetItemInfoNormal( pITEM, x, y, FALSE, nItemCount, bShowMyEq );				
			}
		}
		else
		{
			SetItemInfoNormal( pITEM, x, y, TRUE, nItemCount, bShowMyEq);			
		}
	}
	else
	{		
		SetItemInfoNormal(NULL,0,0);	
	}
}// end 2012-06-14 by isshin 아템미리보기

// 2010. 04. 21 by jskim 신규 럭키 머신 구현
void CINFGameMain::SetItemInfo1(CItemInfo* pItemInfo, int nItemNum, int x, int y, int nItemCount/*=0*/, BOOL bShowMyEq/*=TRUE*/)
{
	FLOG( "CINFGameMain::SetItemInfo(CItemInfo* pItemInfo, int nItemNum, int x, int y)" );
	if(nItemNum != 0)
	{
		ITEM *pITEM = g_pDatabase->GetServerItemInfo(nItemNum);
		if(pITEM == NULL)
		{
			SetItemInfoNormal(NULL,0,0);			
			return;
		}
		
		if(IS_SKILL_ITEM(pITEM->Kind) == FALSE)
		{
			if(pItemInfo)
			{
				SetItemInfoUser( pItemInfo, x, y, bShowMyEq);
			}
			else
			{
				SetItemInfoNormal( pITEM, x, y, FALSE, nItemCount, bShowMyEq );
			}
		}
		else
		{
			SetItemInfoNormal( pITEM, x, y, TRUE, nItemCount, bShowMyEq);
		}
	}
	else
	{
		SetItemInfoNormal(NULL,0,0);
	}
}
//end 2010. 04. 21 by jskim 신규 럭키 머신 구현

void CINFGameMain::ToggleHelpImage()
{
	FLOG( "CINFGameMain::ToggleHelpImage()" );
	m_bShowHelp = !m_bShowHelp;
	if(m_bShowHelp)
	{
		for(int i=0;i<HELP_NUM;i++)
		{
			util::del(m_pHelp[i]);
			char buf[64];
			wsprintf(buf, "help%02d", i);
			m_pHelp[i] = new CINFImageEx;
			DataHeader * pDataHeader = FindResource(buf);
			m_pHelp[i]->InitDeviceObjects( pDataHeader );
			m_pHelp[i]->RestoreDeviceObjects();
		}
	}
	else
	{
		for(int i=0;i<HELP_NUM;i++)
		{
			if(m_pHelp[i])
			{
				m_pHelp[i]->InvalidateDeviceObjects();
				m_pHelp[i]->DeleteDeviceObjects();
				util::del(m_pHelp[i]);
			}
		}

	}
}
void CINFGameMain::SetNotice(int index, int x, int y)
{
	if(m_pHelpDesk && m_pHelpDesk->m_nHelpDeskIndex != index)
	{
		m_pHelpDesk->SetHelpDeskBox( x, y, 400, index, TRUE, 10 );
		m_pHelpDesk->m_fHelpTextCheckTime = HELP_TEXT_CHECK_TIME;
	}
//	if(m_pNotice)
//	{
//		m_pNotice->SetNotice(strNotice);
//	}
}
void CINFGameMain::SetNotice(char* szString, int x, int y)
{
	if(m_pHelpDesk)
	{
		m_pHelpDesk->SetHelpDeskBox( x, y, 400, 0, FALSE, 10, szString );
		m_pHelpDesk->m_fHelpTextCheckTime = HELP_TEXT_CHECK_TIME;
	}
}

HRESULT CINFGameMain::SetResourceFile(char* szFileName)
{
	FLOG( "CINFGameMain::SetResourceFile(char* szFileName)" );
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	m_pGameData->SetFile(szFileName, FALSE, NULL, 0);

	return S_OK;
}

DataHeader * CINFGameMain::FindResource(const char* szRcName)
{
	FLOG( "CINFGameMain::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pGameData)
	{
		pHeader = m_pGameData->Find(szRcName);
	}
	return pHeader;
}

// 2007-07-04 by bhsohn 오퍼레이터 추가
DataHeader * CINFGameMain::FindResource_LoadOp(char* szRcName)
{	
	return g_pInterface->FindResource_LoadOp(szRcName);
}


void CINFGameMain::SetQuestLimitTime(int limitTime, int PlayTime, char* MissionName)
{	
	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리	
	//m_nTimeSecond = limitTime * 60;	
	SetQuestTimeElapse(limitTime * 60);
// 2008-07-24 by dgwoo 8레벨 퀘스트에서 Faq창이 뜨는 버그 수정.
//	wsprintf(m_strNPCName,"%s", MissionName);	

	if(PlayTime)
	{
		int resTime = (limitTime*60) - (g_pShuttleChild->m_myShuttleInfo.TotalPlayTime - PlayTime);
		// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리	
		//m_nTimeSecond = resTime;	
		SetQuestTimeElapse(resTime);
	}

	// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
	//m_nTimeLimitMissionOldTime = m_nTimeSecond;
	m_nTimeLimitMissionOldTime = (int)GetQuestTimeElapse();
	
	m_bQuestLimitTimeView = TRUE;	
}


CEnemyData * CINFGameMain::GetEnemyCharaterID(POINT pt)
{
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(pt.x,pt.y);

	float fLength = 50.0f;
	map<INT,CEnemyData *>::iterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	CEnemyData * pEnemy = NULL;
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
		if( itEnemy->second->m_nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pEnemy = itEnemy->second;
			}
		}
		itEnemy++;
	}

	return pEnemy;
}

void CINFGameMain::RenderPopUpWindowImage(int x, int y, int cx, int cy)
{
	//FLOG( "CINFGameMain::RenderPopUpWindowImage(int x, int y, int cx, int cy, int cstringLen)" );
	FLOG( "CINFGameMain::RenderPopUpWindowImage(int x, int y, int cx, int cy)" );

	m_pImgTextPopUp[0]->Move(x-7, y-2);
	m_pImgTextPopUp[0]->Render();

	m_pImgTextPopUp[1]->Move(x, y-2);
	m_pImgTextPopUp[1]->SetScale(cx, 1);
	m_pImgTextPopUp[1]->Render();

	m_pImgTextPopUp[2]->Move(x+cx, y-2);
	m_pImgTextPopUp[2]->Render();
}

/************************************************************
**
**	툴팁의 BG 출력. ( Image, Round Box )
**
**	Create Info :	2010. 05. 03.	hsLee
**
*************************************************************/
void CINFGameMain :: RenderToolTipBg ( int ix , int iy , int iWid , int iHgt )
{

	if ( iWid <= 0 || iHgt <= 0 || NULL == m_pImgTextBg )
		return;

	FLOG( "CINFGameMain :: RenderToolTipBg ( int ix , int iy , int iWid , int iHgt )" );

	float fScly = .0f;
	float fOrgHgt = m_pImgTextBg->GetImgSize().y - 2;

	iWid += 7;
	
	if ( fOrgHgt <= .0f )
		return;

	fScly = (float)iHgt / fOrgHgt;

	if ( fScly <= 0.1f )
		return;

	float fPosx = (float)ix - 2.0f;
	float fPosy = (float)iy - 2.0f;

	m_pImgTextBg->Move ( fPosx , fPosy );
	m_pImgTextBg->SetScale ( (float)iWid , fScly );

	m_pImgTextBg->Render();

}

char *CINFGameMain::GetMapNameByIndex(int nMapIndex)
{
	return g_pDatabase->GetMapInfo(nMapIndex)->MapName;
}
void CINFGameMain::RenderRequestEnable(float fAniTime)
{
	DWORD dwColor = 0x00FFFFFF;
	DWORD dwAlpha;

	if(fAniTime < 0)
	{
		return;
	}
	else if(fAniTime <= REQUEST_ENABLE_TIME/2.0f)
	{
		dwAlpha = fAniTime*2.0f * 255;	
	}
	else
	{
		dwAlpha = (2.0f - fAniTime*2.0f) * 255;	
	}

	dwAlpha = dwAlpha << 24;
	dwColor |= dwAlpha;

	m_pRequestEnableBack->Move((g_pD3dApp->GetBackBufferDesc().Width - REQUEST_ENABLE_BOX_WIDTH)/2,
								(g_pD3dApp->GetBackBufferDesc().Height - REQUEST_ENABLE_BOX_HEIGHT)/2);
	m_pRequestEnableBack->Render();
	m_pRequestEnableTriangle->SetColor(dwColor);
	m_pRequestEnableTriangle->Move((g_pD3dApp->GetBackBufferDesc().Width - REQUEST_ENABLE_BOX_WIDTH)/2,
									(g_pD3dApp->GetBackBufferDesc().Height - REQUEST_ENABLE_BOX_HEIGHT)/2);
	m_pRequestEnableTriangle->Render();
}

void CINFGameMain::SetMentMessage(int x, int y, char *strMent)
{	
	// 멘트 관련 렌더링
	m_pFontDrawMent->DrawText(x,y,GUI_FONT_COLOR,strMent);
}
void CINFGameMain::SetGuildName(int x, int y, char *strGuild, DWORD dwFontColor)
{
	m_pFontDrawMent->DrawText(x, y, dwFontColor, strGuild);
}
// 2013-03-06 by bhsohn 복귀 유저 시스템
int CINFGameMain::GetMentMessageSize(char *strMent)
{	
	// 멘트 관련 렌더링	
	return m_pFontDrawMent->GetStringSize(strMent).cx;
}
// END 2013-03-06 by bhsohn 복귀 유저 시스템

void CINFGameMain::RenderQSlotIcon()
{
	CINFIcon* pIconInfo = m_pIcon;

	// 2005-11-28 by ispark, 여기서 IconName은 SourceIndex이다.
	pIconInfo->SetIcon(m_pQuickSlot->m_pQSlotInfo[m_pQuickSlot->m_nCurrentTab][m_pQuickSlot->m_nSelectSlotNumber].IconName, 
						m_nQSlotPosX,
						m_nQSlotPosY, 1.0f);
	pIconInfo->Render();
}

DWORD CINFGameMain::GetCurrentColor(float fTime, DWORD dwType, float fTotal, float fDelay)
{	// fTime 전체 시간, dwType 알파타입, fTotal 페이드 인아웃 타임, fDelay 지연 시간
	DWORD dwColor = 0x00FFFFFF;
	DWORD dwAlpha;

	if(dwType == ALPHA_WAVE)
	{// 깜빡깜빡
		float fAlpha;
		fAlpha =  (fTime - (((int)(fTime/fTotal))*fTotal))/fTotal;
		if(fAlpha >= 0.5f)
		{
			dwAlpha = 255 * (2.0f - fAlpha*2.0f);
		}
		else
		{
			dwAlpha = 255 * (2.0f - (1.0f - fAlpha)*2.0f);
		}
	}
	else if(dwType == ALPHA_DELAY_WAVE)
	{
		float fAlpha;
		fTime = fTime - (((int)(fTime/fTotal))*fTotal);
		float fGap = (fTotal - fDelay)/2.0f;	// fGap + fDelay + fGap = fTotal
		
		if(fTime < fGap)
		{
			// 2006-07-18 by dgwoo 점점 진해진다.
			fAlpha =  fTime / fGap;
			dwAlpha = 255 * fAlpha;
		}
		else if(fTime >= fGap && fTime <= fGap + fDelay)
		{
			// 2006-07-18 by dgwoo 선명한 글씨 유지
			dwAlpha = 255;
		}
		else
		{
			// 2006-07-18 by dgwoo 점점 흐려진다.
			fAlpha =  (fTime - fDelay -fGap) / fGap;
			dwAlpha = 255 * (1.0f - fAlpha);			
		}

//		fAlpha =  (fTime - (((int)(fTime/fTotal))*fTotal))/fTotal;
//		if(fAlpha >= 0.5f)
//		{
//			dwAlpha = 255 * (2.0f - fAlpha*2.0f);
//		}
//		else
//		{
//			dwAlpha = 255 * (2.0f - (1.0f - fAlpha)*2.0f);
//		}		
	}
	else if(dwType == ALPHA_FADE_IN)
	{
		if(fTime < 0.0f)
		{
			dwAlpha = 0;
		}
		else if(fTime < fTotal)
		{
			dwAlpha = 255 * (fTime/fTotal);
		}
		else if(fTime > fTotal)
		{
			dwAlpha = 255;
		}
	}
	else if(dwType == ALPHA_FADE_OUT)
	{
		if(fTime < 0.0f)
		{
			dwAlpha = 255;
		}
		else if(fTime < fTotal)
		{
			dwAlpha = 255 * (1.0f - (fTime/fTotal));
		}
		else if(fTime > fTotal)
		{
			dwAlpha = 0;
		}
	}

	dwAlpha = dwAlpha << 24;
	dwColor |= dwAlpha;
	return dwColor;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		ispark
/// \date		2005-07-20 ~ 2005-07-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderToolTip()
{
	// 2006-03-06 by ispark, 글자 길이 가져오기
	if(m_strToolTip[0])
	{
		int sizeX = m_pFontToolTip->GetStringSize(m_strToolTip).cx;
		RenderPopUpWindowImage(m_ptToolTip.x+10, m_ptToolTip.y-15, sizeX, 1);
//		RenderPopUpWindowImage(m_ptToolTip.x+10, m_ptToolTip.y-15, strlen(m_strToolTip)*6.5, 1);
		m_pFontToolTip->DrawText(m_ptToolTip.x+10, m_ptToolTip.y-FONTTOOLTIP_Y, GUI_FONT_COLOR,m_strToolTip,0L);
	}
}


// 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.
/**********************************************************************
**
**	툴팁 출력. (개행 지원.)
**
**	Create Info :	2010. 05. 03.	hsLee.
**
***********************************************************************/
void CINFGameMain :: RenderToolTipEx( void )
{

	if ( TRUE != m_bRenderToolTipEx )
	{
		if ( m_vecToolTip.size() > 0 && m_dwFrameToolTip[0] - m_dwFrameToolTip[1] > 30 )
			ResetToolTipEx();

		return;
	}

	if ( m_vecToolTip.size() > 0 )
	{
		RenderToolTipBg (m_ptToolTipEx.x+10 , m_ptToolTipEx.y-15 , m_iToolTipMaxWid , (int)(m_vecToolTip.size() * m_pFontToolTip->GetStringSize( (char*)m_vecToolTip[0].c_str() ).cy) );
		
		vector < string >::iterator it = m_vecToolTip.begin();

		int iCnt = 0;
		for ( ; it != m_vecToolTip.end(); it++ )
		{
			m_pFontToolTip->DrawText(m_ptToolTipEx.x+10, m_ptToolTipEx.y-FONTTOOLTIP_Y + m_pFontToolTip->GetStringSize( (char*)it->c_str() ).cy * iCnt , GUI_FONT_COLOR , (char*)it->c_str() , 0L );
			iCnt++;
		}
	}

	if ( !IsRectEmpty ( &m_rcToolTipEx ) )
	{
		POINT pt;
			GetCursorPos( &pt );
			ScreenToClient ( g_pD3dApp->GetHwnd() , &pt );

		if ( !PtInRect ( &m_rcToolTipEx , pt ) )
			m_bRenderToolTipEx = FALSE;
	}

	if ( m_dwFrameToolTip[0] - m_dwFrameToolTip[1] > 10 )
		m_bRenderToolTipEx = FALSE;

	m_dwFrameToolTip[0]++;


}
// End. 2010. 04. 28 by hsLee 인피니티 필드 2차 난이도 조절.


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMain::InitShowWindow()
/// \brief		화면에 보여질 윈도우창 기본으로 바꾸는 함수( 초기화 , 윈도우창 초기화.)
/// \author		ispark
/// \date		2005-08-05 ~ 2005-08-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::InitShowWindow()
{
	// 오른쪽 창
	RightWindowShow(FALSE,m_nRightWindowInfo);
//	RightWindowShow(FALSE, RIGHT_WINDOW_OPTION);

	// 왼쪽 창
	LeftWindowShow(FALSE,m_nLeftWindowInfo);
	
	// F1키 사용중이면 지운다.
	if(m_bShowHelp == TRUE)
	{
		ToggleHelpImage();
	}

	//m_pMiniMap->SetZoomState(TRUE);
	m_bQuestView = FALSE;
	m_pOtherCharInfo->m_bRenderInfo = FALSE;
	g_pGameMain->m_pChat->m_bMenuListUse = FALSE;
	m_bMenuListFlag = FALSE;
	
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	//m_pItemInfo->ShowItemInfo(FALSE);
	ShowItemInfo(FALSE);
	// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁

	g_pD3dApp->m_pShuttleChild->m_bMouseLock = FALSE;
	m_pUnitState->OffState( INF_US_LOCK_MASK );

	// 2008-01-17 by dgwoo 맵 이동시 창 초기화.
	g_pGameMain->m_pCouponWindow->SetShow(FALSE);			// 쿠폰창 초기화.


	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//	if(g_pTutorial->GetTutorialEd() == FALSE)			// 2005-10-17 by ispark 방금 튜토리얼을 끝낸 상태라면 미션창을 닫으면 안됀�?
//		m_pMissionInfo->MissionProcFlag(FALSE);
	// 방금 튜토리얼을 끝낸 상태라면 미션창을 닫으면 안됀�?
	if(g_pTutorial->GetTutorialEd() == FALSE)			
		ShowMissionTreeWnd(FALSE, FALSE);

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	BOOL bClick = FALSE;
	if(m_pInven->IsShowInven())
	{
		bClick = TRUE;
	}	

	// 2009. 09. 24 by jskim 필드 창고 캐쉬 아이템
	//m_pInven->ShowInven(NULL, NULL, bClick); // 열려있다면 닫는다.
	//m_pCharacterInfo->ShowCharacterInfoWnd(FALSE, NULL);
	// end 2008-08-22 by bhsohn EP3 인벤토리 처리	
	if(g_pInterface->m_pCityBase-> GetCurrentEnterBuildingIndex()!=FIELD_STORE_SHOP_BUILDING_INDEX)
	{
		m_pInven->ShowInven(NULL, NULL, bClick); // 열려있다면 닫는다.
		m_pCharacterInfo->ShowCharacterInfoWnd(FALSE, NULL);

		// 2013-03-18 by bhsohn 팩토리 조합 추가
		// B눌렀을시 창을 닫자
		OnClickItemMixWnd(FALSE, 0, 0);
		// END 2013-03-18 by bhsohn 팩토리 조합 추가

		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
		OnClickArmorCollectionWnd(FALSE, 0, 0);
		// END 2013-05-28 by bhsohn 아머 컬렉션 시스템
	}	

	structInvenPosInfo tempInfo = m_pInven->GetInvenPosInfo();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	tempInfo.ptItemIvenPos.x = g_pD3dApp->GetBackBufferDesc().Width/2 - m_pInven->GetBkSize().x / 2;
	tempInfo.ptItemIvenPos.y = g_pD3dApp->GetBackBufferDesc().Height/2 - m_pInven->GetBkSize().y / 2;
#else	  
	tempInfo.ptItemIvenPos.x = INVEN_ITEM_START_X;
	tempInfo.ptItemIvenPos.y = INVEN_ITEM_START_Y;
#endif
	tempInfo.ptEqIvenPos.x = INVEN_EQ_START_X;
	tempInfo.ptEqIvenPos.y = INVEN_EQ_START_Y;
	m_pInven->SetInvenPosInfo(&tempInfo);
	//end 2009. 09. 24 by jskim 필드 창고 캐쉬 아이템


	// 2008-10-20 by bhsohn FAQ 기체 상태에서는 안보이게 변경
	m_bFAQProsFlag			= FALSE;
	// end 2008-10-20 by bhsohn FAQ 기체 상태에서는 안보이게 변경

	// 2008-11-06 by bhsohn 마우스 가두기 모드 보완
	g_pD3dApp->RefreshMouseMode();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMain::CheckKeyUseState(int nFlage, BOOL bAllUse, BOOL bOneUse)
/// \brief		키 사용 체크, 두번째 BOOL형은 체크 사용 허용, 세번째 BOOL형은 세부적인 사용 허용 
/// \author		ispark
/// \date		2005-08-09 ~ 2005-08-09
/// \warning	
///
/// \param		
/// \return		BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::CheckKeyUseState(int nwParamFlage, int nlParamFlage, BOOL bUse, BOOL bOneUse)
{
	// 키를 막는 내용들
	// 2005-04-07 by jschoi - Tutorial
	// 2005-08-04 by ispark - 비행중일때 일때 키를 막는다.
	// 2005-08-08 by ispark - 정지시에는 허용

	if(bUse == TRUE)
	{
		// wParam키
		switch(nwParamFlage) 
		{
		case VK_ESCAPE:

			if(g_pShuttleChild->WarpSkipInOutAirPort())
				return TRUE;

			// 2010. 05. 27 by jskim 시네마 적용 카메라 구현
			if ( g_pShuttleChild->IsShowCinema() )
			{
				// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
				g_pInterface->m_pInfinity->SendPacket_SkipEndingCinema();

				//g_pShuttleChild->DeletePattern();
				return TRUE;
			}

			//end 2010. 05. 27 by jskim 시네마 적용 카메라 구현
			if(	// 2007-07-10 by dgwoo 튜토리얼 모드에서도 esc키를 먹게한다.
				g_pTutorial->IsTutorialMode() == FALSE && 
				!g_pShuttleChild->GetIsUseInterface())
//				(g_pD3dApp->m_bCharacter == FALSE && 
//				 (g_pShuttleChild->m_bUnitStop == FALSE && g_pShuttleChild->m_bLandedMove == FALSE)))
				 return TRUE;
			
			// 2008-03-04 by bhsohn 시즈모드후 게임종료면 멈추는 현상처리
			// 시즈모드에서 esc를 막는다
			if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind)
				&&(g_pShuttleChild->m_bAttackMode == _SIEGE))
			{
				return TRUE;
			}

			break;

		case VK_F1:
			if(g_pTutorial->IsTutorialMode() == TRUE)
				return TRUE;
			break;

		case VK_F2:
			if(g_pTutorial->IsTutorialMode() == TRUE)
				return TRUE;
			break;

		case VK_F3:	// PVP
			if(g_pTutorial->IsTutorialMode() == TRUE)
				return TRUE;

			if(g_pD3dApp->m_bBazaar)			// 2006-07-25 by ispark, 바잡맵에서는 PVP 띄우지 않기
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0002,COLOR_ERROR);// "현재 맵에서는 사용 할 수 없습니다."
				return TRUE;
			}
			break;

		case VK_F5:
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface())
				 return TRUE;
			break;

		case VK_F6:
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface())
				 return TRUE;
			break;

		case VK_F7:
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface())
				 return TRUE;
			break;

		case VK_F8:	// 거래
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface())
				return TRUE;
			
			if(g_pD3dApp->m_bBazaar)			// 2006-07-25 by ispark, 바잡맵에서는 PVP 띄우지 않기
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0002,COLOR_ERROR);// "현재 맵에서는 사용 할 수 없습니다."
				return TRUE;
			}
			break;

		case VK_F9:
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface())
				 return TRUE;
			break;

		case VK_TAB:
			break;
		}
			
		// lParam키
		switch(nlParamFlage)
		{
		case WM_KEYDOWN_LPARAM_I:
		case WM_KEYDOWN_LPARAM_V:
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface() ||
				 m_bVOIPFlag == TRUE)
				 return TRUE;
			break;
		case WM_KEYDOWN_LPARAM_P:
			if(g_pTutorial->IsTutorialMode() == TRUE || 
				!g_pShuttleChild->GetIsUseInterface() ||
				 m_bVOIPFlag == TRUE ||
				 g_pD3dApp->m_bBazaar)		// 2006-07-21 by ispark, 바잡맵에서는 미션창 띄우지 않기
				 return TRUE;
			break;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		무효화 영역 지정 함수( 처리 프로세서에 마지막부분에 위치 )
///				후에 정리후에 다시 작성
/// \author		by ydkim
/// \date		2005-08-16 
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::InvalidWndRect(int x, int y, int nSizeW, int nSizeH)
{


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		상점에서 아이템을 착용해 보기
///
/// \author		by ydkim
/// \date		2005-12-1
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ChangeItemByShop(int nItemNum, int nUnitColor)
{
	ITEM *item = NULL;
	item = g_pDatabase->GetServerItemInfo(nItemNum);
	if(item == NULL)
		return;

	ITEM_GENERAL *pITEMG = NULL;
	CItemInfo *pItemInfo = NULL;
	// 아머일 경우 다르게 표현한다.
//	if(ITEMKIND_DEFENSE == item->Kind)
//	{
//	}
#ifdef _REWORKED_COLORSHOP
	if (IS_PRIMARY_WEAPON(item->Kind) || IS_SECONDARY_WEAPON(item->Kind) || ITEMKIND_DEFENSE == item->Kind || ITEMKIND_COLOR_ITEM == item->Kind)
#else
	if(IS_PRIMARY_WEAPON(item->Kind) || IS_SECONDARY_WEAPON(item->Kind) || ITEMKIND_DEFENSE == item->Kind)
#endif
	{		
#ifdef _REWORKED_COLORSHOP
		if (item->ItemNum == COLOR_RESET_ITEMNUM)
			return;
#endif
		// 상점에서 아이템을 착용해보기 위한 부분
		pITEMG							= new ITEM_GENERAL;
		pITEMG->Wear					= WEAR_ATTACHED;
		pITEMG->CurrentCount			= (IS_CHARGABLE_ITEM(item->Kind)?item->Charging:1);

		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		//pITEMG->ScarcityNumber			= 0;
		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		pITEMG->NumOfEnchants			= 0;
		if (ITEMKIND_COLOR_ITEM == item->Kind)
		{
			//Simulate Armor
			pITEMG->ColorCode = item->SourceIndex;
			pITEMG->ShapeItemNum = item->SourceIndex;
			pITEMG->Kind = ITEMKIND_DEFENSE;
		}
		else
		{
			pITEMG->ColorCode = 0;
		}
		pITEMG->UsingTimeStamp			= ITEM_NOT_USING;
		pITEMG->UsingStartTime.Reset();		
		pITEMG->ItemInfo				= item;
		if (ITEMKIND_COLOR_ITEM != item->Kind)
			pITEMG->Kind					= item->Kind;
		pITEMG->ItemNum					= item->ItemNum;
		pITEMG->UniqueNumber			= 0;	
		pITEMG->ItemWindowIndex			= POS_ITEMWINDOW_OFFSET;
		
		pItemInfo = new CItemInfo((ITEM_GENERAL*)pITEMG);		
		g_pStoreData->UpdateWearItemData( pItemInfo, TRUE );

		if(ITEMKIND_DEFENSE == item->Kind)
		{
			g_pShuttleChild->SetInvenAmorColor(0);					// 2006-04-08 by ispark, 기어때문에 색깔을 기본 색깔로 셋팅
		}
	}
	else if(nUnitColor != -1)
	{
		// 칼라 상점에서 칼라구입 시 칼라를 바꿀때.
		// 2006-02-17 by ispark
//		g_pD3dApp->SetUnitArmorColorNum(nUnitColor);
		g_pShuttleChild->SetInvenAmorColor(nUnitColor);
		return;
	}
	else
	{
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 현재는 1형, 2형 아머 정보만 수정 가능
	// 추후 착용가능 아이템이 생기면 여기서 수정 및 변경 (착용해보기)
	// 기존 착용아이템 정보 삭제
	if(IS_PRIMARY_WEAPON(pItemInfo->Kind) && m_pItemInfoWear[POS_PROWOUT])
	{	// 1형 정보 수정
		util::del( m_pItemInfoWear[POS_PROWOUT]);
		m_pItemInfoWear[POS_PROWOUT] = NULL;
		util::del( m_pITEMG[POS_PROWOUT]);
		m_pITEMG[POS_PROWOUT] = NULL;

		m_pITEMG[POS_PROWOUT] = pITEMG;
		m_pItemInfoWear[POS_PROWOUT] = pItemInfo;
	}
	else if(IS_SECONDARY_WEAPON(pItemInfo->Kind) && m_pItemInfoWear[POS_WINGOUT])
	{
		// 2형 정보 수정
		util::del( m_pItemInfoWear[POS_WINGOUT]);
		m_pItemInfoWear[POS_WINGOUT] = NULL;
		util::del( m_pITEMG[POS_WINGOUT]);
		m_pITEMG[POS_WINGOUT] = NULL;

		m_pITEMG[POS_WINGOUT] = pITEMG;
		m_pItemInfoWear[POS_WINGOUT] = pItemInfo;
	}
	else if(ITEMKIND_DEFENSE == pItemInfo->Kind)
	{
		// 아머 정보 수정
		util::del( m_pItemInfoWear[POS_CENTER]);
		m_pItemInfoWear[POS_CENTER] = NULL;
		util::del( m_pITEMG[POS_CENTER]);
		m_pITEMG[POS_CENTER] = NULL;

		m_pITEMG[POS_CENTER] = pITEMG;
		m_pItemInfoWear[POS_CENTER] = pItemInfo;
	}
	//
	//////////////////////////////////////////////////////////////////////////	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		전에 착용하고 있던 아이템 정보 백업
///				백업 루트(m_nItemSourceNum)
/// \author		by ydkim
/// \date		2005-12-1
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::BackUpWearItem()
{
	// 상점 진입시 아이템 정보 백업
	// FindItemInInventoryByItemNum( INT nItemNum );	
	CItemInfo* pTempItem = NULL;

	for(int i=0; i<SIZE_MAX_POS; i++)
	{
		pTempItem = g_pStoreData->FindItemInInventoryByWindowPos( i );
		if(pTempItem)
		{
			// 2006-08-24 by ispark, 유니크 넘버로 변경
//			m_nItemSourceNum[i]	= pTempItem->ItemNum;
			m_nItemSourceNum[i]	= pTempItem->UniqueNumber;

			if(i == POS_CENTER)
			{
				m_nArmorColor = max(1, pTempItem->ColorCode%100);				
//				g_pD3dApp->SetUnitArmorColorNum(m_nArmorColor);			// 2006-02-17 by ispark
			}
		}
	}

	m_bUseShopItem = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		상점 착용아이템 리셋 
///				리셋(m_pItemInfoWear, m_pITEMG)
/// \author		by ydkim
/// \date		2005-12-5
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ResetWearItem(int nPos)
{
	if(nPos == -1)
{
	for(int i=0; i<SIZE_MAX_POS; i++)
	{
		util::del( m_pItemInfoWear[i]);
		m_pItemInfoWear[i] = NULL;
		util::del( m_pITEMG[i]);
		m_pITEMG[i] = NULL;
		m_nItemSourceNum[i] = 0;
		m_nArmorColor = 0;
		}
	}
	else
	{
		util::del( m_pItemInfoWear[nPos]);
		m_pItemInfoWear[nPos] = NULL;
		util::del( m_pITEMG[nPos]);
		m_pITEMG[nPos] = NULL;
		m_nItemSourceNum[nPos] = 0;
		m_nArmorColor = 0;
		
	}
}

void CINFGameMain::TickDrawWindowInfo()
{
	// 2005-04-29 by ydkim 도움말을 보여줄것인가?
//	if(TRUE == g_pTutorial->GetShowHelpDesk() && TRUE == IS_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) )
	if(TRUE == g_pTutorial->GetShowHelpDesk())
	{		
		g_pGameMain->InitHelpSystem(TRUE);
		g_pTutorial->SetShowHelpDesk(FALSE);
	}
	
	m_nLeftWindowY = DEAULT_WINDOW_POS_Y - m_nLeftWindowScaleSize/2;
	if(m_nLeftWindowY < SIZE_ROLL_Y )
		m_nLeftWindowY = SIZE_ROLL_Y;
	if(m_pInfWindow->m_bLeftWindow)
	{
		m_nLeftWindowY = GetPositionInvenScreenMode();
	}
	else
	{
		if(m_nLeftWindowY > LEFT_WINDOW_MAX_Y-SIZE_WEAPON_Y)
			m_nLeftWindowY = LEFT_WINDOW_MAX_Y-SIZE_WEAPON_Y;
	}
	m_nRightWindowY = DEAULT_WINDOW_POS_Y - m_nRightWindowScaleSize/2;
	if(m_nRightWindowY < SIZE_ROLL_Y )
		m_nRightWindowY = SIZE_ROLL_Y;
	if(m_pInfWindow->m_bRightWindow)
	{
// 2007-12-22 by dgwoo 창모드 지원 (m_nLeftWindowY 와 같은 Y값을 가지게 한다.)
//		if(m_nRightWindowY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//			m_nRightWindowY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
		m_nRightWindowY = GetPositionInvenScreenMode();
	}
	else
	{
		if(m_nRightWindowY > RIGHT_WINDOW_MAX_Y-SIZE_WEAPON_Y)
			m_nRightWindowY = RIGHT_WINDOW_MAX_Y-SIZE_WEAPON_Y;
	}
	
	if( g_pD3dApp->m_dwGameState == _CITY ||
		g_pD3dApp->m_dwGameState == _SHOP)
	{
		if(!g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
		{
			if(m_bLeftWindowModeChage)
			{
				m_bLeftWindowModeChage = ChangeLeftWindowMode();
			}
			if(m_bRightWindowModeChage)
			{
				m_bRightWindowModeChage = ChangeRightWindowMode();
			}
		}
	}
	else
	{
		if(m_bLeftWindowModeChage)
		{
			m_bLeftWindowModeChage = ChangeLeftWindowMode();
		}
		if(m_bRightWindowModeChage)
		{
			m_bRightWindowModeChage = ChangeRightWindowMode();
		}
		if(m_pMiniMap)
			m_pMiniMap->Tick();
	}
}

void CINFGameMain::ProcessAltiMeter(float fMinHeight, float fMaxHeight, float fMyPosHeight)
{
	//////////////////////////////////////////////////////////////////////////
	// ydkim 05.12.13
	// 고도계 설정은 물높이에서 부터 지정된 최고 높이로 설정한다
	// 고도게이지는 고도게이지 이미지에서 일정부분 뺀부분부터 시작한다
	// 추후 이미지 교체가 있으면 이미지 공백도 같이 설정이 필요한다
	//////////////////////////////////////////////////////////////////////////

	float fGageGabBottom		= 23.0f;										// 게이지 이미지 여백 (아래)
	float fAltitudeApplyMin		= fMinHeight;									// 최저높이 설정 
	float fAltitudeApplyMax		= fMaxHeight+fAltitudeApplyMin;					// 최고높이 설정
	float fAltitudeApplyImgH	= m_ptGageBack.y - fGageGabBottom+9;			// 이미지 높이
	float fAltitudeApplyMyPos	= fMyPosHeight;									// 내위치
	
	float Temp  = fAltitudeApplyMax/fMyPosHeight;
	if(Temp<=0) Temp = fAltitudeApplyMax;
	Temp = fAltitudeApplyImgH-(fAltitudeApplyImgH/Temp);

	RenderAltiMeter((int)GAMEMAIN_ALTIMETER_X, (int)GAMEMAIN_ALTIMETER_Y, (int)Temp);
}

void CINFGameMain::RenderAltiMeter(int x, int y, int nPos)
{
	m_pAltiMeterGage->Move(x, y);
	m_pAltiMeterGage->Render();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pAltiMeterPin->Move(x - ( m_pAltiMeterPin->GetImgSize().x - m_pAltiMeterGage->GetImgSize().x ) /2, y+nPos);
#else			
	m_pAltiMeterPin->Move(x-5, y+nPos);
#endif
	m_pAltiMeterPin->Render();
}


void CINFGameMain::DrawSignBoardString()
{
	if(m_pCityBoard)
		m_pCityBoard->DrawSignBoardString();
}

void CINFGameMain::InitCityBoard(int nMapNumber)
{
	if(m_pCityBoard)
	{
		m_pCityBoard->InitDeviceObjects(nMapNumber);
		m_pCityBoard->RestoreDeviceObjects();
	}
}

void CINFGameMain::InitChangeInfluenceBoard()
{
	if(m_pCityBoard)
	{
		m_pCityBoard->DelAllString();
		InitCityBoard(g_pGround->m_nMapNumber);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInterface::SetSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, POINT pt, BYTE byType)
/// \brief		선택 아이템
/// \author		ispark
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		TRUE : 빈 아이템 및 다른 아이템    FALSE : 같거나 없다
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, POINT pt, BYTE byType)
{
	FLOG( "CINFInven::SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)" );
	
	if(pDisplayInfo && m_stSelectItem.pSelectItem)
	{
		return;
	}

	if(pDisplayInfo)
	{
		m_stSelectItem.pSelectIcon = m_pIcon->FindIcon(pDisplayInfo->IconName);
		m_stSelectItem.pSelectItem = pDisplayInfo;
		m_stSelectItem.bySelectType = byType;
		m_stSelectItem.ptIcon = pt;

		UpdateIconSelectItem(byType);

		// set tooltip
		// 2006-08-22 by dgwoo 툴팁 삭제.
//        POINT pt;
//        GetCursorPos( &pt );
//        ScreenToClient( g_pD3dApp->GetHwnd(), &pt );
//		CheckMouseReverse(&pt);
//		SetToolTip(pt.x, pt.y, pDisplayInfo->Name);
	}
	else
	{
		m_stSelectItem.pSelectItem = NULL;
		m_stSelectItem.pSelectIcon = NULL;
		m_stSelectItem.bySelectType = ITEM_NOT_POS;
		m_stSelectItem.ptIcon.x = 0;
		m_stSelectItem.ptIcon.y = 0;

		// 2008-08-22 by bhsohn EP3 인벤토리 처리
		// 2006-07-27 by ispark, 인벤토리 버튼 초기화
		//m_pInven->m_bSelectItem = 0;

		// 2007-03-02 by bhsohn 다중 선택 추가 보안
		ClearMultiSelectItem();
	}

	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void ClearMultiSelectItem()
/// \brief		선택 아이템
/// \author		// 2007-03-02 by bhsohn 다중 선택 추가 보안
/// \date		2007-03-05 ~ 2007-03-05
/// \warning	
///
/// \param		
/// \return		TRUE : 빈 아이템 및 다른 아이템    FALSE : 같거나 없다
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ClearMultiSelectItem()
{
	int i = 0;
	m_nMultiSelCnt = 0;	
	for(i = 0; i < MAX_MULTI_SEL;i++)
	{
		m_stMultiSelectItem[i].pSelectItem = NULL;
		m_stMultiSelectItem[i].pSelectIcon = NULL;
		m_stMultiSelectItem[i].bySelectType = ITEM_NOT_POS;
		m_stMultiSelectItem[i].ptIcon.x = 0;
		m_stMultiSelectItem[i].ptIcon.y = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMain::RenderSelectItem()
/// \brief		선택된 아이템 렌더
/// \author		ispark
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderSelectItem()
{
	// 2007-03-02 by bhsohn 다중 선택 추가 보안
	if(m_nMultiSelCnt > 0) 
	{
		RenderMultiSelectItem();
		return;		
	}
	// end 2007-03-02 by bhsohn 다중 선택 추가 보안

	if(m_stSelectItem.pSelectIcon)
	{
        POINT ptCursor;
        GetCursorPos( &ptCursor );
        ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
		CheckMouseReverse(&ptCursor);		

		m_stSelectItem.pSelectIcon->Scaling(1.0f, 1.0f);
		m_stSelectItem.pSelectIcon->Move(ptCursor.x - m_stSelectItem.ptIcon.x, ptCursor.y - m_stSelectItem.ptIcon.y);
		m_stSelectItem.pSelectIcon->Render();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMain::RenderMultiSelectItem()
/// \brief		다중 아이템 랜더링 
/// \author		// 2007-03-02 by bhsohn 다중 선택 추가 보안
/// \date		2007-03-02 ~ 2007-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderMultiSelectItem()
{	
	POINT ptCursor;
    GetCursorPos( &ptCursor );
    ScreenToClient( g_pD3dApp->GetHwnd(), &ptCursor );
	CheckMouseReverse(&ptCursor);	
	int nCnt = 0;
	float fXStart, fYStart;
	float fX, fY;
	fX = fY = 0;
	fXStart = m_stMultiSelectItem[0].ptIcon.x;
	fYStart = m_stMultiSelectItem[0].ptIcon.x;
	for(nCnt = MAX_MULTI_SEL-1;nCnt >= 0 ;nCnt--)
	{
		if(NULL == m_stMultiSelectItem[nCnt].pSelectIcon)
		{
			continue;
		}
		fX = ptCursor.x - fXStart + nCnt*5;
		fY = ptCursor.y - fYStart + nCnt*5;
		m_stMultiSelectItem[nCnt].pSelectIcon->Scaling(1.0f, 1.0f);
		m_stMultiSelectItem[nCnt].pSelectIcon->Move(fX, fY);
		m_stMultiSelectItem[nCnt].pSelectIcon->Render();
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFGameMain::CheckPopUpMenu(D3DXVECTOR2 vPos, CEnemyData* pTp)
/// \brief		팝업 메뉴 체크
/// \author		ispark
/// \date		2006-08-01 ~ 2006-08-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::CheckPopUpMenu(D3DXVECTOR2 vPos, CEnemyData* pTp)
{
	// 2005-08-25 by ispark
	// 기어가 캐릭터를 ALT + LBUTTONDOWN 눌러 팝업창 나오지 않게 함(캐릭터 -> 기어 포함)
	// 2005-09-12 by ispark
	// 캐릭터끼리 메뉴는 일단 막음
//					if((g_pD3dApp->m_bCharacter == TRUE || pTp->m_bEnemyCharacter == TRUE))
//						break;
	// 2005-11-03 by ispark
	// 서로 다른 캐릭터 상태끼리 안됨
	if((g_pD3dApp->m_bCharacter == FALSE && pTp->m_infoCharacter.CharacterInfo.CharacterMode0 == TRUE) ||
		(g_pD3dApp->m_bCharacter == TRUE && pTp->m_infoCharacter.CharacterInfo.CharacterMode0 == FALSE))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051208_0103, COLOR_ERROR);// "서로 다른 캐릭터 상태는 선택을 할 수 없습니다."
		return FALSE;
	}

	// 2006-01-07 by ispark, 세력 비교
#ifdef FREEWAR_
	if (!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTp->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
#else
	if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pTp->m_infoCharacter.CharacterInfo.InfluenceType))
#endif
	{
// 2007-10-24 by dgwoo 전진기지전시 시스템 클릭이 안되는걸로 인식되는 문제로 스트링 뺌.
//		// 2006-01-07 by ispark, 서로 다른 세력끼리 선택 안됨
//		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051208_0102, COLOR_ERROR);// "서로 다른 세력끼리 선택을 할 수 없습니다."
		return FALSE;
	}

	// 2006-08-01 by ispark, 개인상점 이용중인가 비교
	if(pTp->m_byBazaarType > 0)
	{
		// 상대방이 개인상점 이용중면 안됨
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060801_0010, COLOR_ERROR);// "상대방이 개인상점을 이용중에는 선택할 수 없습니다."
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMain::RenderOnInfo()
/// \brief		스탯과 미션 정보 알림
/// \author		ispark
/// \date		2006-08-22 ~ 2006-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderOnInfo()
{
	if(m_bKnowON[LOW_LEVEL_MISSION_HELP] && m_bKnowTwinkle)
	{//미션도움말 및 버튼.
		m_pImgMission->Move(GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*3, GAMEMAIN_BUTTON_START_Y);
		m_pImgMission->Render();

		if(m_pHelpDesk->m_stLowLevelHelp[LOW_LEVEL_MISSION_HELP].bLowLevelHelp)
		{
			m_pHelpDesk->m_pLowLevelHelpflash->Move(GAMEMAIN_BUTTON_START_X - 1 + GAMEMAIN_BUTTON_INTERVAL*3, GAMEMAIN_BUTTON_START_Y - 11);
			m_pHelpDesk->m_pLowLevelHelpflash->Render();
		}
	}

	// 2006-08-21 by ispark
	if(m_bKnowON[LOW_LEVEL_STAT_HELP] && m_bKnowTwinkle)
	{//스탯도움말 및 버튼.
		m_pImgStack->Move(GAMEMAIN_BUTTON_START_X+GAMEMAIN_BUTTON_INTERVAL*1, GAMEMAIN_BUTTON_START_Y);
		m_pImgStack->Render();

		if(m_pHelpDesk->m_stLowLevelHelp[LOW_LEVEL_STAT_HELP].bLowLevelHelp)
		{
			m_pHelpDesk->m_pLowLevelHelpflash->Move(GAMEMAIN_BUTTON_START_X - 1 + GAMEMAIN_BUTTON_INTERVAL*1, GAMEMAIN_BUTTON_START_Y - 11);
			m_pHelpDesk->m_pLowLevelHelpflash->Render();
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			StartNoviceUserHelper
/// \brief		신규 유저 인터페이스 관련 신규 처리를 한다. 
/// \author		bhsohn
/// \date		2007-01-26 ~ 2007-01-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::StartNoviceUserHelper()
{
	if(NULL == m_pHelpDesk)
	{
		return;
	}

	// 2007-06-12 by bhsohn 미션 인터페이스 수정안
//	m_bKnowON[LOW_LEVEL_MISSION_HELP] = TRUE;	
//	m_pHelpDesk->SetLowLevelStringbyType(LOW_LEVEL_MISSION_HELP);

	const CHARACTER* pShuttleInfo= g_pShuttleChild->GetShuttleInfo();		

	BOOL bBonusStat = FALSE;

	//g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.BonusStat
	if(pShuttleInfo->BonusStat > 0)
	{		
		// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합
		ShowOpStatChange(pShuttleInfo->BonusStat);
//		m_bKnowON[LOW_LEVEL_STAT_HELP] = TRUE;
//		m_pHelpDesk->SetLowLevelStringbyType(LOW_LEVEL_STAT_HELP, pShuttleInfo->BonusStat);
	}			
}

void CINFGameMain::EndNoviceUserHelper()
{
    m_bKnowON[LOW_LEVEL_MISSION_HELP] = FALSE;
	m_bKnowON[LOW_LEVEL_STAT_HELP] = FALSE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			IsNoviceUserMode()
/// \brief		초급자 모드 인지 판단한다.
/// \author		bhsohn
/// \date		2007-01-26 ~ 2007-01-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsNoviceUserMode()
{	
	BOOL bNoviceUser = FALSE;	
	if(NULL == g_pShuttleChild)
	{
		return bNoviceUser;
	}
	BYTE byInfluenceType = g_pShuttleChild->GetShuttleInfo()->InfluenceType;	

	// 바이제니유 일반군(중립)이면 신규유저로 인식
	if(byInfluenceType & INFLUENCE_TYPE_NORMAL)
	{
		if((!IsMyLevelQuestComplete(g_pShuttleChild->GetShuttleInfo())))
		{
			bNoviceUser = TRUE;
			return bNoviceUser;
		}		
	}	
	return bNoviceUser;

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			IsMyLevelQuestComplete()
/// \brief		내 렙이 맞는 퀘스트를 완료했냐?
/// \author		bhsohn
/// \date		2007-01-26 ~ 2007-01-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsMyLevelQuestComplete(const CHARACTER* pShuttleInfo)
{
	BOOL bComplete = FALSE;

	// 전체 미션을 가져오는 부분
	vector<int> vecQuestTemp;
	vecQuestTemp = g_pQuestData->GetRaceQuestIndex(INFLUENCE_TYPE_ALL_MASK);	

	vector<int>::iterator itQuest = vecQuestTemp.begin();
	while(itQuest != vecQuestTemp.end())
	{
		CQuest* pQuest = g_pQuestData->FindQuest(*itQuest);
		if(NULL == pQuest)
		{
			itQuest++;
			continue;
		}
		// 최소레벨 퀘스트
		if(pShuttleInfo->Level == pQuest->ReqLevel.Min)
		{
			bComplete = g_pQuestData->IsQuestCompleted(*itQuest);			
		}		
		itQuest++;
	}

	return bComplete;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void UpdateNewUserHelper()
/// \brief		신규유저 인터페이스 부분 수정
/// \author		bhsohn
/// \date		2007-01-26 ~ 2007-01-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::UpdateNoviceUserHelper()
{
	if(IsNoviceUserMode())					
	{
		StartNoviceUserHelper();
	}
	else
	{
		EndNoviceUserHelper();
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void UpdateLV1NoviceUserHelper()
/// \brief		레벨1이면 무조건 띠운다.
/// \author		bhsohn
/// \date		2007-01-26 ~ 2007-01-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::UpdateLV1NoviceUserHelper()
{		
	const CHARACTER* pShuttleInfo= g_pShuttleChild->GetShuttleInfo();		
	BYTE byInfluenceType = pShuttleInfo->InfluenceType;	

	// 바이제니유 일반군(중립)이면 신규유저로 인식	
	if(byInfluenceType & INFLUENCE_TYPE_NORMAL)
	{
		StartNoviceUserHelper();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤에서 다중 아이템 선택
/// \author		bhsohn
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetMultiSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon)
{
	g_pD3dApp->SetMultiSelectItem(pDisplayInfo, pSelectItem, ptIcon, ITEM_INVEN_POS);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		창고에서 다중 아이템 처리
/// \author		bhsohn
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetMultiSelectCityStoreItem(CItemInfo* pItemInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, INVEN_DISPLAY_INFO  *pDisSelectItem)
{			
	
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	//g_pD3dApp->SetMultiSelectCityStoreItem(pItemInfo, pSelectItem, ptIcon, ITEM_STORE_POS);
	g_pD3dApp->SetMultiSelectCityStoreItem(pItemInfo, pSelectItem, ptIcon, ITEM_STORE_POS, pDisSelectItem);
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
}

void CINFGameMain::UpdateIconSelectItem(BYTE byType)
{
	deque<stMultiSelectItem>* pVector = g_pD3dApp->GetMultiSelVector(byType);
	if(NULL == pVector)
	{
		return;
	}		
	ClearMultiSelectItem();
	int nCnt = 0;
	deque<stMultiSelectItem>::iterator it = pVector->begin();
	while(it != pVector->end())
	{			
		if(nCnt >= MAX_MULTI_SEL)
		{
			break;
		}
		
		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		
		//AddMultiSelectItem(it->SourceIndex, it->bySelectType, it->ptIcon);
		AddMultiSelectItem( it->SourceIndex, it->bySelectType, it->ptIcon, it->szIconName );

		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		it++;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInterface::SetSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, POINT pt, BYTE byType)
/// \brief		선택 아이템
/// \author		// 2007-03-02 by bhsohn 다중 선택 추가 보안
/// \date		2007-03-05 ~ 2007-03-05
/// \warning	
///
/// \param		
/// \return		TRUE : 빈 아이템 및 다른 아이템    FALSE : 같거나 없다
///////////////////////////////////////////////////////////////////////////////


// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//void CINFGameMain::AddMultiSelectItem(int nSourceIndex, BYTE bySelectType, POINT ptIcon)
void CINFGameMain::AddMultiSelectItem( int nSourceIndex, BYTE bySelectType, POINT ptIcon, char* szIconName )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

{
	FLOG( "CINFInven::SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo)" );

	// 아이템 갯수 
	if(m_nMultiSelCnt >= MAX_MULTI_SEL)
	{
		return;
	}	
	int nItemIdx = m_nMultiSelCnt;


	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

// 	char buf[64];
// 	wsprintf(buf, "%08d", nSourceIndex);
// 	CINFImage* pIcon = m_pIcon->FindIcon(buf);

	CINFImageEx* pIcon = m_pIcon->FindIcon( szIconName );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	if(NULL == pIcon)
	{
		return;
	}
	m_stMultiSelectItem[nItemIdx].pSelectIcon	= pIcon;	
	m_stMultiSelectItem[nItemIdx].bySelectType	= bySelectType;
	m_stMultiSelectItem[nItemIdx].ptIcon		= ptIcon;
	m_nMultiSelCnt++;

	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		창고에서의 다중 선택 
/// \author		bhsohn
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::GetCityStoreMultiSelectItem()
{
	return g_pD3dApp->GetCityStoreMultiSelectItem();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤에서 창고로 아이템 이동
/// \author		// 2007-03-02 by bhsohn 다중 선택 추가 보안
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::PopupMultiItemSelect()
{
	GUI_BUILDINGNPC* pNpc = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
	if(NULL == pNpc)
	{
		return;
	}
	// 다중 아이템 팝업 
	
	char buf[MAX_PATH];	
	char szMoney[80];
	int nMultiPopupId = 0;	

	memset(buf, 0x00, MAX_PATH);	
	memset(szMoney, 0x00, 80);

	if(IS_ITEM_SHOP_TYPE(pNpc->buildingInfo.BuildingKind)
		|| IS_WARPOINT_SHOP_TYPE(pNpc->buildingInfo.BuildingKind))
	{	
		// 상점들
		g_pD3dApp->GetMultiItemSPI(_Q_SHOP_MULTI_SELL_ITEM, szMoney);		
		nMultiPopupId = _Q_SHOP_MULTI_SELL_ITEM;
		wsprintf( buf, STRMSG_C_070212_0202, szMoney);
	}
	else if(IS_STORE_SHOP_TYPE(pNpc->buildingInfo.BuildingKind))
	{	
		// 창고
		g_pD3dApp->GetMultiItemSPI(_Q_STORE_MULTI_PUT_ITEM, szMoney);		// 돈 텍스트를 얻어온다. 
		nMultiPopupId = _Q_STORE_MULTI_PUT_ITEM ;
		wsprintf( buf, STRMSG_C_070212_0201, szMoney);
	}
	else
	{
		return;
	}
	

	if(m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ITEM))
	{
		m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ITEM);
	}
	else if(m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ENERGY))
	{
		m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ENERGY);
	}	
	else if(m_pInfWindow->IsExistMsgBox(_Q_SHOP_MULTI_SELL_ITEM ))
	{
		m_pInfWindow->DeleteMsgBox(_Q_SHOP_MULTI_SELL_ITEM );
	}								
	else if(m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_PUT_ITEM ))
	{
		m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_PUT_ITEM );
	}								
	else if(m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_GET_ITEM ))
	{
		m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_GET_ITEM );
	}
	
	m_pInfWindow->AddMsgBox(buf, nMultiPopupId, NULL);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		창고에서 인벤로 아이템 이동
/// \author		// 2007-03-02 by bhsohn 다중 선택 추가 보안
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::PopupStoreMultiItemSelect()
{	
	if(m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ITEM))
	{
		m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ITEM);
	}
	else if(m_pInfWindow->IsExistMsgBox(_Q_SHOP_SELL_ENERGY))
	{
		m_pInfWindow->DeleteMsgBox(_Q_SHOP_SELL_ENERGY);
	}	
	else if(m_pInfWindow->IsExistMsgBox(_Q_SHOP_MULTI_SELL_ITEM ))
	{
		m_pInfWindow->DeleteMsgBox(_Q_SHOP_MULTI_SELL_ITEM );
	}								
	else if(m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_PUT_ITEM ))
	{
		m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_PUT_ITEM );
	}								
	else if(m_pInfWindow->IsExistMsgBox(_Q_STORE_MULTI_GET_ITEM ))
	{
		m_pInfWindow->DeleteMsgBox(_Q_STORE_MULTI_GET_ITEM );
	}
	
	m_pInfWindow->AddMsgBox(STRMSG_C_070212_0205, _Q_STORE_MULTI_GET_ITEM, NULL);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 스킬이 사용중이냐
/// \author		// 2007-03-22 by bhsohn 아이콘 크기 버그 수정
/// \date		2007-03-22 ~ 2007-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsDoingSkill(int nSourceIndex)
{
	if(NULL == m_pInfSkill)
	{
		return FALSE;
	}
	return m_pInfSkill->IsDoingSkill(nSourceIndex);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 스킬이 사용중이냐
/// \author		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
/// \date		2007-03-27 ~ 2007-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::Render_AltiMeter()
{
	// 게이지 표시
	ProcessAltiMeter(g_pGround->m_projectInfo.fWaterHeight, 
		g_pScene->m_nMaxAtitudeHeight, 
		g_pShuttleChild->m_vPos.y );
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		미션 컨트롤 랜더링
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderMission()
{
	if(NULL == m_pMissionMain)
	{
		return;
	}	
	m_pMissionMain->Render();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		미션 컨트롤 메시지 처리
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcMiss(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_pMissionMain)
	{
		return INF_MSGPROC_NORMAL;
	}	
	// 2007-11-01 by bhsohn 상점 이용중에 출격 버튼 막음
	else if(g_pD3dApp->m_dwGameState == _SHOP)
	{
		return INF_MSGPROC_NORMAL;
	}
	// end 2007-11-01 by bhsohn 상점 이용중에 출격 버튼 막음
	return m_pMissionMain->WndProc(uMsg, wParam, lParam);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM * CINFGameMain::GetServerItemInfo(int nItemNum)
{
	return g_pDatabase->GetServerItemInfo(nItemNum);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImageEx* CINFGameMain::FindIcon(char* strName)
{
	return m_pIcon->FindIcon(strName);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetWindowOrder(int nWndIndex)
{
	g_pInterface->SetWindowOrder(nWndIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::CreateChatChild(TCHAR *str,int type, int chatType/* = CHAT_TAB_SYSTEM*/, char* szWisperToCharacterName, char *szWisperFromCharacterName)
{	
	g_pD3dApp->m_pChat->CreateChatChild(str,
											type, 
											chatType, 
											szWisperToCharacterName, 
											szWisperFromCharacterName); 
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MEX_MONSTER_INFO * CINFGameMain::CheckMonsterInfo(int nMonType)
{
	return g_pDatabase->CheckMonsterInfo(nMonType);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MAP_INFO * CINFGameMain::GetMapInfo(MapIndex_t nMapIndex)
{
	return g_pDatabase->GetMapInfo(nMapIndex);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderMissionINGInfo()
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->RenderMissionINGInfo();	
}
void CINFGameMain::SetMissionEnterShop(BOOL bEnterShopState)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->SetMissionEnterShop(bEnterShopState);

}
void CINFGameMain::MissionStart(DWORD dMissionIndex)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->MissionStart(dMissionIndex);
}


BOOL CINFGameMain::IsBazaar()
{
	// 2007-12-27 by bhsohn 게임 시작에 관련 부분 수정
	if(NULL == g_pGround)
	{
		return FALSE;		
	}
	// end 2007-12-27 by bhsohn 게임 시작에 관련 부분 수정
	return g_pGround->m_bBazaar;
}

void CINFGameMain::TickMissonMain()
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->Tick();
}
// 게임을 처음 접속한 유저
void CINFGameMain::FirstUserShowMissionTreeWnd()
{	
	// 2007-07-20 by bhsohn 10레벨 튜토리얼은 전체를 다 해야한다.
	g_pInterface->ShowOpUserHelper(TRUE, OPERATOR_USER_HELPER_FREESKA, NULL);	
}

void CINFGameMain::ShowMissionTreeWnd(BOOL bFlag, BOOL bSoundPlay)
{
	// 바잡맵에서는 미션창 열지 못한다.
	if(g_pGameMain->IsBazaar())
	{
		return ;
	}
	if(bFlag && g_pTutorial)
	{
		g_pTutorial->SetTutorialEd(FALSE);		
	}
	m_pMissionMain->ShowTreeWnd(bFlag, bSoundPlay);
}

BOOL CINFGameMain::IsShowTreeWnd()
{
	if(NULL == m_pMissionMain)
	{
		return FALSE;
	}	
	return m_pMissionMain->IsShowTreeWnd();
}

void CINFGameMain::SetMissionPosMissionSearchStart(BOOL nstate)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->SetPosMissionSearchStart(nstate);
}
void CINFGameMain::SetMissionPosMissionInfo(CQuest* pQuest)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->SetPosMissionInfo(pQuest);	
}

void CINFGameMain::SetMissionPosMissionComplete(BOOL bFlag)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->SetPosMissionComplete(bFlag);
}

BOOL CINFGameMain::GetInfluenceMission()
{
	if(NULL == m_pMissionMain)
	{
		return FALSE;
	}	
	return m_pMissionMain->GetInfluenceMission();	
}
void CINFGameMain::SetInfluenceMission(BOOL nFlag, float fANIPercent, float fVANPercent)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->SetInfluenceMission(nFlag, fANIPercent, fVANPercent);
}

void CINFGameMain::RefreshMission(BOOL bClick)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	INT nQuestIdx = 0;
	CQuest* pProgressMission = g_pQuestData->GetProgressMission();
	if(pProgressMission)
	{
		nQuestIdx = pProgressMission->QuestIndex;
	}
	m_pMissionMain->RefreshMission(bClick, nQuestIdx);

}
void CINFGameMain::SelectNextMission(INT nQuestIndex)
{
	if(NULL == m_pMissionMain)
	{
		return ;
	}	
	m_pMissionMain->SelectNextMission(nQuestIndex);

}

void CINFGameMain::GoWarpMapChange(int nQuestMapIdx)
{
	// 2007-07-04 by bhsohn 오퍼레이터 추가
	g_pInterface->ShowOpUserHelper(FALSE, 0, NULL);
		
	const auto& myShuttle = GetMyShuttleInfo();

	// 2007.09.19 by bhsohn 전진 기지 출격 처리
	if(IS_OUTPOST_CITY_MAP_INDEX(myShuttle.MapChannelIndex.MapIndex))
	{
		m_pMissionMain->SetMissionMapQuestIdx(nQuestMapIdx);
		BOOL bGoMissionMap = g_pShuttleChild->SendGoMissionMap();				
		
		if(FALSE == bGoMissionMap)
		{
			// 미션맵 안 보내면 출격 발동
			CObjectChild * pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_CHARACTERMODE_WARP, g_pCharacterChild->m_vPos, 10240.0f);
			if(pCurrentObjEvent)
			{
				g_pShuttleChild->SendFieldSocketRequestEventObjectWarpIn(pCurrentObjEvent);
			}
			// 2008-04-22 by bhsohn 전진기지 출격 안되는 문제 해결
			else
			{
				// EVENT_TYPE_CHARACTERMODE_WARP 을 못찾으면 EVENT_TYPE_CHARACTERMODE_DIRECTLY_WARP 를 찾는다.
				pCurrentObjEvent = g_pScene->FindEventObjectByTypeAndPosition(EVENT_TYPE_CHARACTERMODE_DIRECTLY_WARP, g_pCharacterChild->m_vPos, 10240.0f);
				if(pCurrentObjEvent)
				{
					g_pShuttleChild->SendFieldSocketRequestEventObjectWarpIn(pCurrentObjEvent);
				}				
			}
			// end 2008-04-22 by bhsohn 전진기지 출격 안되는 문제 해결
		}
	}
	else
	{
		// 미션맵이 0이면 그냥 출격
		// 미션맵으로 이동
		g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
		g_pCharacterChild->FineObjectTakeOff();	
		m_pMissionMain->SetMissionMapQuestIdx(nQuestMapIdx);
	}
	// end 2007.09.19 by bhsohn 전진 기지 출격 처리

	// 2008-06-17 by bhsohn 편대 관련 처리
	if(g_pShuttleChild 
		&& g_pShuttleChild->m_pClientParty 
		&& g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER 
		&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}

}

void CINFGameMain::AddMsgBox(char* strMsg, int nType, DWORD dwData1, DWORD dwData2, int nLiveTime,UID64_t UniqueNumber, char* strName)
{
	m_pInfWindow->AddMsgBox(strMsg, nType, dwData1, dwData2, nLiveTime,UniqueNumber, strName);
}

// 캐릭정보
const CHARACTER& CINFGameMain::GetMyShuttleInfo() const
{
	return g_pShuttleChild->GetMyShuttleInfo();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 레벨업시 팝업 윈도우 Show
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::UpdateMissionPopWnd(BYTE byUpLevel, int nSuccessQuestIdx)
{
	int nPossibleQuestIdx = 0;
	if(FALSE == m_pMissionMain->IsLevelSecnQuest(byUpLevel, nSuccessQuestIdx, &nPossibleQuestIdx))
	{
		return;
	}	

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업
	BOOL bShowFirstOperation = FALSE;
	if(g_pD3dApp->IsMyShuttleCharacter() == TRUE)
	{
		// 오퍼레이터를 띠운다.
		// 2007-07-20 by bhsohn 튜토리얼 수정
		const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
		// 바이제니유
		if((COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_VCN))
			&&(SELECT_FIRST_QUEST_IDX == nSuccessQuestIdx))
		{		
			bShowFirstOperation = TRUE;
			g_pInterface->ShowOpFirstInfluenceQuest(INFLUENCE_TYPE_VCN);			
		}
		else if((COMPARE_INFLUENCE(myShuttleInfo.InfluenceType, INFLUENCE_TYPE_ANI))
			&&(SELECT_FIRST_QUEST_IDX == nSuccessQuestIdx))
		{			
			bShowFirstOperation = TRUE;
			g_pInterface->ShowOpFirstInfluenceQuest(INFLUENCE_TYPE_ANI);			
		}
	}		
	if(FALSE == bShowFirstOperation )
	{
		m_pMissionMain->ShowMissionPopWnd(TRUE, nPossibleQuestIdx);			
		m_pMissionMain->SetFirstPossibleQuestIdx(0);
	}
	else 
	{
		m_pMissionMain->SetFirstPossibleQuestIdx(nPossibleQuestIdx);
		
	}
}

// 처음 마을 들어온 유저가 오퍼레이션 닫기 버튼 클릭
void CINFGameMain::OnClickFirstUserCloseBtn()
{
	if(m_pMissionMain->GetFirstPossibleQuestIdx() != 0)
	{
		m_pMissionMain->ShowMissionPopWnd(TRUE, m_pMissionMain->GetFirstPossibleQuestIdx());	
	}

}
int CINFGameMain::GetMissionMapQuestIdx()
{
	if(NULL == m_pMissionMain)
	{
		return 0;
	}
	return m_pMissionMain->GetMissionMapQuestIdx();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 레벨업시 팝업 윈도우 Show
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetImageInfo(int nImageIndex, BOOL bMissionImg)
{
	g_pGImage->SetImageInfo(nImageIndex, bMissionImg);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		FAQ버튼 깜빡임
/// \author		// 2007-07-04 by bhsohn 오퍼레이터 추가
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetFaqBtnBlingBling(BOOL bSet)
{	
	m_pFaqBtn->SetBtnBlingBling(bSet, FAQ_BLING_TIME);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		브리핑 룸 클릭
/// \author		// 2007-06-12 by bhsohn 미션 인터페이스 수정안
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickBriefingRoom()
{
	BOOL bShowTree = IsShowTreeWnd();
	bShowTree ^= TRUE;
	ShowMissionTreeWnd(bShowTree, TRUE);
	g_pInterface->SetWindowOrder(WNDMissionInfo);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		튜토리얼 
/// \author		// 2007-07-04 by bhsohn 오퍼레이터 추가
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::NextTutirialState()
{
	g_pTutorial->NextState();
}
void CINFGameMain::ChangeChapterReady()
{
	g_pTutorial->ChangeChapterReady();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		튜토리얼 
/// \author		// 2007-07-04 by bhsohn 오퍼레이터 추가
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::InitTutorialLesson()
{
	g_pTutorial->ChangeLesson(L0);

}

BOOL CINFGameMain::IsTutorialMode()
{
	return g_pTutorial->IsTutorialMode();
}

void CINFGameMain::ShowOpBossMonsterSummon(BYTE byBelligerence, int nHour, int nMinute)
{	
	g_pInterface->ShowOpBossMonsterSummon(byBelligerence, nHour, nMinute);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-07-27 by bhsohn 스탯 추가 된 도움말 오퍼레이터에 통합
/// \date		2007-07-27 ~ 2007-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::ShowOpStatChange(int nStat)
{
	if(!IS_NORMAL_INFLUENCE_TYPE(g_pGameMain->GetMyShuttleInfo().InfluenceType))
	{
		return;
	}
	char chHelpString[1024];
	memset(chHelpString, 0x00, 1024);
	wsprintf(chHelpString, STRMSG_C_060823_0000, nStat);
	g_pInterface->ShowOpWndTxt(chHelpString);

	m_bShowStatOpWnd = TRUE;
	
}
VOID CINFGameMain::HideOpStatChange()
{
	m_bShowStatOpWnd = FALSE;
	g_pInterface->ShowOpWnd(FALSE);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-02 by bhsohn 바자맵에서 출격버튼 표현안함.
/// \date		2007-08-02 ~ 2007-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::RefreshMissionUI()
{
	if(NULL == m_pMissionMain)
	{
		return;
	}
	m_pMissionMain->RefreshMissionUI();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn 스피커 아이템 추가
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::RenderSpeakerMsg()
{
	if(NULL == m_pINFSpeakeMsg)
	{
		return;
	}
	m_pINFSpeakeMsg->Render();

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn 스피커 아이템 추가
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::TickSpeakerMsg()
{
	if(NULL == m_pINFSpeakeMsg)
	{
		return;
	}
	m_pINFSpeakeMsg->Tick();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn 스피커 아이템 추가
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::AddSpeakerMsgString(char* pTxt, BOOL bGm, BOOL bGmColor)
{
	if(NULL == m_pINFSpeakeMsg)
	{
		return;
	}
	m_pINFSpeakeMsg->AddSpeakerMsgString(pTxt, bGm,bGmColor);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-08-07 by bhsohn 스피커 아이템 추가
/// \date		2007-08-07 ~ 2007-08-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::DeleteNotGmMsg()
{
	if(NULL == m_pINFSpeakeMsg)
	{
		return;
	}
	m_pINFSpeakeMsg->DeleteNotGmMsg();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-05 by bhsohn 전진 기지전
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::StringCulling(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont)
{
	STRING_CULL(pLineString, nCullStringNum, vecStorage, pHanFont);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::InitSecurityPassMode()
{	
	if(FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem())
	{
		return;
	}
	if(NULL == m_pINFSecuMain)
	{
		m_pINFSecuMain = new CINFSecuMain;		
		m_pINFSecuMain->SetGameData( m_pGameData );
		m_pINFSecuMain->InitDeviceObjects();		
	}
	// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	g_pD3dApp->SetSecondPassLock(g_pD3dApp->IsSetSecondaryPassword());	// 로그오프하고 처음 에는 무조건 락상태이다.

	// 설정이 안되어있으면 Lock상태로 무조건 띠운다.
//	if(FALSE == g_pD3dApp->IsSetSecondaryPassword())
//	{			
//		m_pINFSecuMain->ShowSecurityWnd(SECURITY_MODE_LOCK, TRUE);
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::RenderSecuritySystem()
{
	if(NULL == m_pINFSecuMain)
	{
		return;
	}
	m_pINFSecuMain->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::TickSecuritySystem()
{
	if(NULL == m_pINFSecuMain)
	{
		return;
	}
	m_pINFSecuMain->Tick();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickF10Btn()
{
	if((FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem()) 
		||(NULL == m_pINFSecuMain))
	{
		return;
	}
	// 2008-08-25 by bhsohn 아레나 이용시, 2차 패스워드 문제 해결
	else if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
	{
		return;
	}
	// end 2008-08-25 by bhsohn 아레나 이용시, 2차 패스워드 문제 해결
	
	// 2007-09-27 by bhsohn 2차 패스워드 추가 기획안
	if(GetAsyncKeyState(VK_CONTROL))
	{
		if(m_pINFSecuMain->IsShowWnd(SECURITY_MODE_LOCK))
		{
			return;
		}
		if(TRUE == g_pD3dApp->IsSetSecondaryPassword())
		{
			//"보안 시스템을 해제하시겠습니까?"
			m_pInfWindow->AddMsgBox(STRMSG_C_070928_0201, _Q_SECOND_PASS_CANCEL_MSG);
		}
	}
	else
	{
		if(m_pInfWindow->IsExistMsgBox(_Q_SECOND_PASS_CANCEL_MSG))
		{
			return;
		}
		BOOL bShowLock = m_pINFSecuMain->IsShowWnd(SECURITY_MODE_LOCK);
		bShowLock ^= TRUE;
		if(bShowLock)
		{
			if(m_pINFSecuMain->IsShowWnd(SECURITY_MODE_PASS_WND))
			{
				return;
			}
			m_pINFSecuMain->ShowSecurityWnd(SECURITY_MODE_LOCK, bShowLock);
		}
		else
		{
			m_pINFSecuMain->AllHideSecurityWnd();		
		}
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcSysKeyDown(WPARAM wParam, LPARAM lParam)
{
	if(CheckSysKeyUseState(wParam, lParam, TRUE, FALSE) ||
		g_pD3dApp->m_bChatMode == TRUE ||
		g_pD3dApp->m_dwGameState == _SHOP || 
		m_nLeftWindowInfo == LEFT_WINDOW_TRANS ||
		g_pGameMain->m_pTrade->m_bTrading)
	{
		return INF_MSGPROC_BREAK;
	}

	switch(wParam)
	{
	case VK_F10:
		{			
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}	
			if(((CInterface*)m_pParent)->m_bShowInterface == FALSE)
				break;
			OnClickF10Btn();
		}
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::CheckSysKeyUseState(int nwParamFlage, int nlParamFlage, BOOL bUse, BOOL bOneUse)
{
	if(bUse == FALSE)
	{
		return FALSE;
	}
	switch(nlParamFlage) 
	{
	case VK_F10:
		if(g_pTutorial->IsTutorialMode() == TRUE || 
			!g_pShuttleChild->GetIsUseInterface())
		{
			return TRUE;
		}
		break;
	}
	return FALSE;	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		패스워드 맞음
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::FieldSecondLockOk(BOOL bIsUnlockFlag, BOOL bSuccess)
{
	// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
	BYTE bLockFlag = FALSE;
	if(bSuccess)
	{
		bLockFlag = !bIsUnlockFlag;		
	}
	else
	{
		bLockFlag = TRUE;
	}
	g_pD3dApp->SetSecondPassLock(bLockFlag);
	// end 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함

	if((FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem()) 
		||(NULL == m_pINFSecuMain))
	{
		// UnLock
		g_pD3dApp->EnterLock(TRUE);
		return;
	}
	
	m_pINFSecuMain->FieldSecondLockOk(bIsUnlockFlag, bSuccess);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CAtumApplication::FieldSocketSecondPasswordUpdateOk(BOOL bSuccess)
/// \brief		2차 패스 워드 업데이트
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
VOID CINFGameMain::FieldSocketSecondPasswordUpdateOk(BOOL bSuccess, BOOL bFirstUseSecondPassword)
{
	if((FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem()) 
		||(NULL == m_pINFSecuMain))
	{
		// UnLock
		g_pD3dApp->EnterLock(TRUE);
		return;
	}
	m_pINFSecuMain->FieldSocketSecondPasswordUpdateOk(bSuccess, bFirstUseSecondPassword);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			int GetPassChangeMode()
/// \brief		2차 패스 워드 업데이트
/// \author		// 2007-09-27 by bhsohn 2차 패스워드 추가 기획안
/// \date		2007-09-27 ~ 2007-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
int CINFGameMain::GetPassChangeMode()
{
	if((FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem()) 
		||(NULL == m_pINFSecuMain))
	{		
		return -1;
	}
	return m_pINFSecuMain->GetPassChangeMode();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-09-12 by bhsohn 2차 암호 시스템 구현
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFGameMain::ShowSecondPassword()
{
	if((FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem()) 
		||(NULL == m_pINFSecuMain))
	{		
		return ;
	}
	m_pINFSecuMain->ShowSecurityWnd(SECURITY_MODE_PASS_WND, TRUE, SECURITY_MODE_PASS_SENCOND_CANCEL);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		최대 총알 수를 얻어온다. 프리미엄 유저는 2배의 총알수 (베트남은 예외)
/// \author		// 2007-10-15 by bhsohn 총알 아이템 추가 처리
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::GetMaxBulletItem(int nBullet)
{
	int nMaxCharge = nBullet;
#ifndef LANGUAGE_VIETNAM
	// 베트남은 2배 시스템 사용 안함.
	if(g_pD3dApp->IsPrimiumUser())
	{
		nMaxCharge = (2*nBullet);
	}
#endif
	return nMaxCharge;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		거래 상에 죽었을시, 키 안먹는 문제처리
/// \author		// 2007-10-16 by bhsohn 거래 상에 죽었을시, 키 안먹는 문제처리
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetTradeInfo(BOOL bTrade)
{
	if(NULL == m_pTrade)
	{
		return;
	}
	m_pTrade->SetTradeInfo(bTrade);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMain::ShowCouponWindow(BOOL i_bShow)
/// \brief		쿠폰창을 보이게 설정.
/// \author		dgwoo
/// \date		2008-01-15 ~ 2008-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ShowCouponWindow(BOOL i_bShow)
{
	POINT pt;
	pt.x = COUPON_WINDOW_POS_X;
	pt.y = COUPON_WINDOW_POS_Y;
	m_pCouponWindow->SetShow(i_bShow);
	m_pCouponWindow->SetWindowPoint(pt);
	
}
void CINFGameMain::SetCouponActive(MSG_FC_EVENT_COUPON_EVENT_INFO* pMsg)
{
	m_pCouponWindow->SetActive(pMsg);
	m_pCouponBtn->EnableBtn(TRUE);
	m_pCouponBtn->ShowWindow(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainChat::GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER])
/// \brief		채팅전체 정보를 얻어온다.
/// \author		// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵�?
/// \date		2008-02-27 ~ 2008-02-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER])
{
	if(m_pChat)
	{
		m_pChat->GetAllChatList(pArrChatLine);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		옵션창 생성
/// \author		// 2008-03-19 by bhsohn Ep3옵션창
/// \date		2008-03-19 ~ 2008-03-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::PopupSystemOptinWnd()
{
	// 2008-04-08 by bhsohn 팅기는 문제
	if(NULL == m_pSystem)
	{
		return;
	}
	// end 2008-04-08 by bhsohn 팅기는 문제
	BOOL bShow = m_pSystem->IsOptionShow();
	bShow ^= TRUE;
	m_pSystem->ShowOptionWnd(bShow);

	if(bShow)
	{
		// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그 
		//g_INFCnt++;
		g_pGameMain->m_bChangeMousePoint = TRUE;
		// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
		g_pInterface->SetWindowOrder(WNDLeftRightWindow);
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MENU, g_pShuttleChild->m_vPos, FALSE);			
	}
	else
	{
#ifndef NEMERIAN
		// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그 
		if(g_pGameMain->m_pUnitState->m_dwState==0)
		{
#endif
			//g_INFCnt--;
			// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
			//if(g_INFCnt==0)
			if(g_pD3dApp->GetINFCnt() == 0)
			{
				g_pGameMain->m_bChangeMousePoint = FALSE;
			}
			// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
#ifndef NEMERIAN
	}
#endif

		// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그 
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::CloseCurrentEnterBuilding()
{
	if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC() != NULL)
	{
		g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_CLOSE);
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-03-27 by bhsohn 미션성공창이 떠있는 상태에서는 상점이 안열리게 변경
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetForceCloseShop(BOOL bForceClose)
{
	g_pInterface->m_pCityBase->SetForceCloseShop(bForceClose);		
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::DestroyMotherShipInfo(BYTE byBelligerence)
{
	m_pMotherShipC->DestroyMotherShipInfo(byBelligerence);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SummonMotherShipInfo(BYTE byBelligerence, 
										ATUM_DATE_TIME  *pSummonMonsterTime, 
										INT	 nContributionPoint, 
										INT SummonMonsterUnitkind)
{
	m_pMotherShipC->SummonMotherShipInfo(byBelligerence, pSummonMonsterTime, nContributionPoint, SummonMonsterUnitkind);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::GetSummonMotherShipCnt()
{
	if(NULL == m_pMotherShipC)
	{
		return 0;
	}
	return m_pMotherShipC->GetSummonMotherShipCnt();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::GetMotherShipInfl(UINT i_nMotherShipInf)
{
	return m_pMotherShipC->GetMotherShipInfl(i_nMotherShipInf);	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::SetWarPointMapInfo(SMSWARINFO_DISPLAY * pWarInfo)
{
	return m_pMotherShipC->SetWarPointMapInfo(pWarInfo);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetStrategicPointInfo(SSTRATEGYPOINT_DISPLAY_INFO* pStrInfo)
{
	m_pStrategicPointDisplay->SetStrategicPoint(pStrInfo);
}

void CINFGameMain::DeleteStrategicPointInfo(MapIndex_t mapIndex)
{
	m_pStrategicPointDisplay->RemoveStrategicPoint(mapIndex);
}

void CINFGameMain::ResetStrategicPointDisplay()
{
	m_pStrategicPointDisplay->ResetAllStrategicPoints();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn 모선전 관리 클래스 추가
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetMSWarInfoDisplayOption(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK* pMsg)
{
	m_pMotherShipC->SetMSWarInfoDisplayOption(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
/// \date		2008-04-07 ~ 2008-04-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFGameMain::GetQuestTimeElapse()
{
	return m_fQuestTimeElapse;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
/// \date		2008-04-07 ~ 2008-04-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetQuestTimeElapse(float fGetQuestTimeElapse)
{
	m_fQuestTimeElapse = fGetQuestTimeElapse;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::UpdatePlayingMotherShipInfo()
{
	if(NULL == m_pMotherShipC)
	{
		return;
	}
	GUI_BUILDINGNPC* pBuilding = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
	if(NULL == pBuilding)
	{
		return;
	}
	int nBuildingKind = pBuilding->buildingInfo.BuildingKind;
	if((BUILDINGKIND_CITYLEADER_LEADER != nBuildingKind)
		&&(BUILDINGKIND_CITYLEADER_OUTPOST != nBuildingKind))	// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
	{
		return;
	}
	CMapCityShopIterator it = g_pInterface->m_pCityBase->m_mapCityShop.find(nBuildingKind);
	if(it == g_pInterface->m_pCityBase->m_mapCityShop.end() || NULL == it->second)
	{
		return;
	}
	
	// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
	//CINFCityLeader * pCityLeader = ((CINFCityLeader*)it->second);
	int nCnt =0;

	BYTE byBelligerence=0;
	BYTE AttInfluence=0;
	ATUM_DATE_TIME timeSummon, timeSummonEnd;
	INT  nContributionPoint=0;
	INT  SummonMonsterUnitkind=0;

	char buf[256];
	memset(buf, 0x00, 256);	
	
	memset(&timeSummon, 0x00, sizeof(ATUM_DATE_TIME));
	memset(&timeSummonEnd, 0x00, sizeof(ATUM_DATE_TIME));
	

	for(nCnt =0;nCnt < m_pMotherShipC->GetSummonMotherShipCnt();nCnt++)
	{
		m_pMotherShipC->GetMotherShipInfo(nCnt, 
											&byBelligerence, 
											&timeSummon, 
											&nContributionPoint, 
											&SummonMonsterUnitkind);

		// 몬스터 이름 
		memset(buf, 0x00, 256);
		MEX_MONSTER_INFO* pMonsterInfo = g_pDatabase->CheckMonsterInfo(SummonMonsterUnitkind);
		if(pMonsterInfo)
		{		
			util::strncpy(buf,pMonsterInfo->MonsterName,SIZE_MAX_MONSTER_NAME);			
		}

		// 공격 세력
		AttInfluence = INFLUENCE_TYPE_VCN;
		if(byBelligerence == BELL_INFLUENCE_VCN)
		{
			// 맵세력과 반대가 공격 세력이다.
			AttInfluence = INFLUENCE_TYPE_ANI;
		}		

		// 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
		if(BUILDINGKIND_CITYLEADER_LEADER == nBuildingKind)
		{
			CINFCityLeader * pCityLeader = ((CINFCityLeader*)it->second);
			pCityLeader->AddWarInfoMothership(AttInfluence,	// 공격세력
								0,		// 승리세력(0:진행중)
								buf,				// 모선이름
								nContributionPoint,				// 세력포인트
								timeSummon,	// 시작시간
								timeSummonEnd);	//종료시간	
		}
		else if(BUILDINGKIND_CITYLEADER_OUTPOST == nBuildingKind)
		{
			CINFCityOutPost * pCityLeader = ((CINFCityOutPost*)it->second);
			pCityLeader->AddWarInfoMothership(AttInfluence,	// 공격세력
								0,		// 승리세력(0:진행중)
								buf,				// 모선이름
								nContributionPoint,				// 세력포인트
								timeSummon,	// 시작시간
								timeSummonEnd);	//종료시간	
			
		}
		// end 2008-10-16 by bhsohn 전진기지 모선전, 거점전 정보창
	}
}

void CINFGameMain::SetMotherShipTeleportTime(MSG_FC_EVENT_CLICK_TELEPORT_OK *pMsg)
{
	m_pMotherShipC->SetMotherShipSearchTeleportTime(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickCommunity()
{
	BOOL bShow = m_pCommunity->IsShowCommunityWindow();
	bShow ^= TRUE;
	m_pCommunity->ShowCommunityWindow(bShow);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderCommunity()
{
	if(!m_pCommunity)
	{
		return;
	}
	m_pCommunity->Render();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcCommunityWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_pCommunity)
	{
		return INF_MSGPROC_NORMAL;
	}	
	return m_pCommunity->WndProc(uMsg, wParam, lParam);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::TextReduce(CD3DHanFont *pFont, int nCutPixelX, char* buf)
{
	int nBufSize = pFont->GetStringSize(buf).cx;
	if(nBufSize < nCutPixelX)
	{
		return;
	}
	char szDstBuff[256];
	char chDummy[8];
	memset(szDstBuff, 0x00, 256);
	memset(chDummy, 0x00, 8);
	sprintf(chDummy, "..");
	
	int nCnt =0;
	int nCheckPoint=  strlen(buf);

	for(nCnt =0;nCnt < strlen(buf); nCnt++)
	{
		if(nCheckPoint <= 0)
		{
			// 처음위치
			break;
		}
		char	*pNextStr = CharPrev(buf, &buf[nCheckPoint]);		
		if(NULL == pNextStr)
		{
			break;
		}
		int	nStringLen = pNextStr - (buf);		
		nCheckPoint = nStringLen;

		strncpy(szDstBuff, buf, nStringLen);		
		szDstBuff[nStringLen] = NULL;
		strcat(szDstBuff, chDummy);		// 뒤에 ".."을 붙인다.

		nBufSize = pFont->GetStringSize(szDstBuff).cx;

		if(nBufSize < nCutPixelX)
		{
			strncpy(buf, szDstBuff, strlen(szDstBuff));		
			buf[strlen(szDstBuff)] = NULL;			
			break;
		}
	}
}
// 2016-07-19 panoskj disabled this
//void CINFGameMain::SetVoiceVolum(DWORD i_nVolum)
//{
//	//// 볼륨 1~100
//	if(i_nVolum > VOLUME_SET_MAX)
//	{
//		i_nVolum = VOLUME_SET_MAX;
//	}
//	else if(i_nVolum < VOLUME_SET_MIN)
//	{
//		i_nVolum = VOLUME_SET_MIN;
//	}
//
//	if(GetVoiceType() == VOICE_NONE)
//		return;
//	if(GetVoiceVolum() == i_nVolum)
//	{
//		return;
//	}
//	//--------------------------------------------------------------------------//
//	DBGOUT("SetVoiceVolum(1~100 : %d)\n",i_nVolum);
//	COPYDATASTRUCT	copyData;
//	DWORD			nVolum = i_nVolum;
//	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//	AVCallType	nCallType = _NtoNCall;
//	if(GetVoiceType() == VOICE_ONE)
//	{
//		nCallType = _1to1Call;
//	}
//	AV_MSG_WM_COPYDATA voIP(nCallType, AV_MT_SET_SPK_VOLUME, nVolum);
//	copyData.dwData = (DWORD)voIP;
//	copyData.lpData = NULL;
//	copyData.cbData = 0;
//	::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//	//--------------------------------------------------------------------------//
//
//	// 2008-10-20 by bhsohn VOIP 버그 수정
//	m_stVOIP.nVolum = i_nVolum;
//	
//}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-10-14 by bhsohn VOIP개선
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CINFGameMain::InitSetVoiceVolum(DWORD i_nVolum)
//{
//	m_stVOIP.nVolum = i_nVolum;
//	//// 볼륨 1~100
//	if(m_stVOIP.nVolum > VOLUME_SET_MAX)
//	{
//		m_stVOIP.nVolum = VOLUME_SET_MAX;
//	}
//	else if(m_stVOIP.nVolum < VOLUME_SET_MIN)
//	{
//		m_stVOIP.nVolum = VOLUME_SET_MIN;
//	}
//
//	if(m_pChat)
//	{
//		m_pChat->UpdateVoiceVolumScroll();		
//	}
//	
//}

//void CINFGameMain::SetVoiceType(BYTE i_byType)
//{
//	if(m_stVOIP.byVoiceType == i_byType)
//		return;
//	m_stVOIP.byVoiceType = i_byType;
//	EN_CHECK_TYPE tempType;
//	MSG_IC_VOIP_SET sMsg;
//	switch(i_byType)
//	{
//	case VOICE_ONE:
//		{
//			tempType = CHECK_TYPE_VOIP_1to1;
//		}
//		break;
//	case VOICE_PARTY:
//		{
//			tempType = CHECK_TYPE_VOIP_PARTY;
//		}
//		break;
//	case VOICE_GUILD:
//		{
//			tempType = CHECK_TYPE_VOIP_GUILD;
//		}
//		break;
//	default:
//		{
//			tempType = CHECK_TYPE_VOIP_NONE;
//		}
//		break;
//	}
//	DBGOUT("SetVoiceType(%d)\n",i_byType);
//	sMsg.VoipType = tempType;
//	g_pIMSocket->SendMsg(T_IC_VOIP_SET,(char*)&sMsg,sizeof(MSG_IC_VOIP_SET));
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-07 by bhsohn 2차 패스워드 활성화시, 인벤 안열리게 함
/// \date		2008-07-07 ~ 2008-07-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsUseSecondPassword()
{
	if((FALSE == g_pD3dApp->IsUseSecondaryPasswordSystem()) 
		||(NULL == m_pINFSecuMain))
	{
		// 2파 패스 워드 시스템을 사용안함.
		return FALSE;
	}
	// 2008-08-25 by bhsohn 아레나 이용시, 2차 패스워드 문제 해결
	else if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
	{
		return FALSE;	// 아레나 에서는 2차 패스워드 사용안함.
	}
	// end 2008-08-25 by bhsohn 아레나 이용시, 2차 패스워드 문제 해결
	else if(g_pD3dApp->IsSecondPassLock())
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-14 by bhsohn EP3 관련 처리
/// \date		2008-07-14 ~ 2008-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsRejectUser(char* pRejectName)
{
	if(!m_pCommunity || NULL == m_pCommunity->GetReject())				// 2013-06-10 by ssjung 캐나다 익셉션 오류 안전 코드 첨부
		return FALSE;
	
	vector<Reject_t>::iterator itReject = m_pCommunity->GetReject()->m_vecReject.begin();
	while(itReject != m_pCommunity->GetReject()->m_vecReject.end())
	{
		if(0 == stricmp(pRejectName, itReject->szRejectName))
		{
			return TRUE;
		}
		itReject++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderMirror(POINT *pMirrorPos/*=NULL*/)
{
	if(NULL == m_pInven)
	{
		return;
	}
	m_pInven->RenderMirror(pMirrorPos);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤에 보일지 여부 판단
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsEquipInvenShow()
{
	if(NULL == m_pInven)
	{
		return FALSE;
	}
	return m_pInven->IsEquipInvenShow();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤에 보일지 여부 판단
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
D3DXMATRIX	CINFGameMain::GetInvenMatInven()
{
	if(NULL == m_pInven)
	{
		D3DXMATRIX MatInven;
		D3DXMatrixIdentity(&MatInven);
		return MatInven;
	}
	return m_pInven->GetInvenMatInven();

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤창 랜더링
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderInvenWnd()
{
	if(m_pInven && !m_pInven->m_bRestored)
	{
		m_pInven->RestoreDeviceObjects();
	}
	if(m_pInven)
	{
		m_pInven->Render() ;
	}
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤창 메시지창
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcInvenWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pInven && m_pInven->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
	{
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤창 메시지창
/// \author		// 2008-08-22 by bhsohn EP3 인벤토리 처리
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2013-03-18 by bhsohn 팩토리 조합 추가
POINT CINFGameMain::ShowShopInven(int nBuildingKind, BOOL bShowInven/*=FALSE*/)
{	
	POINT ptItem, ptEq;
	ptItem.x = ptItem.y = ptEq.x = ptEq.y = 0;
	
	if(IS_SKILL_SHOP_TYPE(nBuildingKind)
		|| IS_CASH_SHOP_TYPE(nBuildingKind)
		|| (BUILDINGKIND_REFINERY == nBuildingKind)
		|| (IS_OUTPOST_SHOP_TYPE(nBuildingKind))
		|| (IS_WARP_SHOP_TYPE(nBuildingKind))
		|| (IS_MISSION_SHOP_TYPE(nBuildingKind))
		|| (BUILDINGKIND_INFLBUFF_SHOP == nBuildingKind ) // 2013-05-07 by bhsohn 세력포인트 개선안 시스템
		|| (BUILDINGKIND_ARENA == nBuildingKind))		
	{
		m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);			
	}
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	else if((BUILDINGKIND_LUCKY_OPTION_MACHINE == nBuildingKind) && !bShowInven)
	{
		m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);
	}
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	// 2009-03-04 by bhsohn 럭키 머신 수정안
	else if((BUILDINGKIND_LUCKY== nBuildingKind) && !bShowInven)
	{
		m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);
	}
	// end 2009-03-04 by bhsohn 럭키 머신 수정안
	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	else if( ( BUILDINGKIND_INFINITY == nBuildingKind ) && !bShowInven )
	{
		// 인피니티 npc는 인벤을 보여주지 않는다
		m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);
	}
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	else if( ( BUILDINGKIND_DISSOLUTION == nBuildingKind ) && !bShowInven )
	{
		m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);
	}
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	else
	{
		// 보통 상점
		ptItem.x = INVEN_ITEM_SHOP_START_X;
		ptItem.y = INVEN_ITEM_SHOP_START_Y;
		ptEq.x = INVEN_EQ_SHOP_START_X;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		ptEq.y = INVEN_ITEM_SHOP_START_Y;														  
#else 
		ptEq.y = INVEN_EQ_SHOP_START_Y;
#endif
		if(g_pD3dApp->GetHeight() < RESOLUTION_FULL_H_768)
		{
			ptEq.x = (INVEN_ITEM_SHOP_START_X-294);
			ptEq.y = INVEN_ITEM_SHOP_START_Y;			
		}

		// 2013-03-18 by bhsohn 팩토리 조합 추가
#ifdef C_INGAME_MIX_ITEM
		if( BUILDINGKIND_FACTORY == nBuildingKind )
		{
			ptItem.x -= 115;
			ptEq.x -= 115;
		}
#endif
		// END 2013-03-18 by bhsohn 팩토리 조합 추가

		if(ptEq.x < 0)
		{
			ptEq.x = 0;
		}
		if(ptEq.y < 0)
		{
			ptEq.y = 0;
		}

		m_pInven->ShowInven(&ptItem, &ptEq, FALSE, TRUE);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM	
		// 2008-11-24 by dgwoo 럭키 머신
		if(BUILDINGKIND_LUCKY == nBuildingKind)
		{
			m_pInven->ShowInvenAndMirror(TRUE);
		}
		// 2010. 04. 21 by jskim 신규 럭키 머신 구현
		else if(BUILDINGKIND_LUCKY_OPTION_MACHINE== nBuildingKind)
		{
			m_pInven->ShowInvenAndMirror(TRUE);
		}
		//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
		// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
				        // 2011. 10. 10 by jskim UI시스템 변경
		else if(BUILDINGKIND_DISSOLUTION == nBuildingKind)
		{
			m_pInven->ShowInvenAndMirror(TRUE);
		}
#endif
		// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
		
	}	
	return ptItem;// 2013-03-18 by bhsohn 팩토리 조합 추가
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		케릭터창 랜더링
/// \author		// 2008-09-22 by bhsohn EP3 캐릭터 창
/// \date		2008-09-23 ~ 2008-09-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderCharacterWnd()
{
	if(m_pCharacterInfo && !m_pCharacterInfo->m_bRestored)
	{
		m_pCharacterInfo->RestoreDeviceObjects();
	}
	
	if(!m_bRightWindowModeChage && m_pCharacterInfo)
	{
		m_pCharacterInfo->Render() ;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief		Enter처리를 위한 컬링함수 ("\\n 이라는 스트링이 찍히므로 잘 처리해야함)
/// \author		// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
/// \date		2008-07-24 ~ 2008-07-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::StringCullingUserData_ToNormal( char* strTalk, int nCullPixel, vector<string> *vecDstString, CD3DHanFont* pFont, BOOL bChRt /*= FALSE*/)
{
	int nCheckPoint=  0;
	char* pCheckPoint = NULL;
	char chCullingBuff[2048], chCullingTmpBuff[2048];	
	char chColorSave = 0;
	BOOL bBreak = FALSE;	
	BOOL bFindBlank = FALSE;	
	int nBlankPos = 0;	
	BOOL bFindCrtn = FALSE;		// \\n 을 찾기 위한 변수
	char chCharRtn[8] = "\\n";
	
	int nCullingPos=0;
	int nLastStartPos = 0;	

	int nCullingLen =0;
	int nLastCullingLen = 0;	

	int nOldCheckPoint = 0;

	ZERO_MEMORY(chCullingBuff);
	ZERO_MEMORY(chCullingTmpBuff);
	
	//for(int nStrPos = 0;nStrPos <= strlen(strTalk); nStrPos++)
	while(TRUE)
	{
		char	*pNextStr = CharNext(&strTalk[nCheckPoint]);
		if(NULL == pNextStr)
		{
			break;
		}
		nOldCheckPoint = nCheckPoint;
		int	nNextLen = pNextStr - (strTalk + nCheckPoint);

		bFindBlank = FALSE;
		
		nCheckPoint += nNextLen;		

		nCullingLen = pNextStr - (strTalk + nLastStartPos);
		strncpy(chCullingBuff, &strTalk[nLastStartPos], nCullingLen);

		bFindCrtn = FALSE;		// \\n 을 찾기 위한 변수 초기화

		if(NULL == pNextStr[0] || (NULL == pNextStr))
		{
			bFindBlank = TRUE;			
			bBreak = TRUE;			
		}		
		else if(nCullingLen >= 2)
		{
			if(('\\' == chCullingBuff[nCullingLen-2]) && ('n' == chCullingBuff[nCullingLen-1]))
			{
				bFindCrtn = TRUE;		// \\n 을 찾기 위한 변수			
			}			
		}

		int nBufSize = pFont->GetStringSize(chCullingBuff).cx;
		if(nBufSize >= nCullPixel || bFindCrtn)
		{			
			// 길이 추가
			int nStringSize = strlen(chCullingBuff);
			memset(chCullingBuff, 0x00, nStringSize+1);
			if(nLastCullingLen)
			{
				// 다시 메모리를 다시 복사해온다.
				strncpy(chCullingBuff, &strTalk[nLastStartPos], nLastCullingLen);				
				nCheckPoint = nCullingPos;
				// 컬링 성공
				nLastStartPos = nCullingPos;						
			}
			else
			{	
				nLastCullingLen = pNextStr - (strTalk + nLastStartPos);

				// 공백이 없다.
				strncpy(chCullingBuff, &strTalk[nLastStartPos], nLastCullingLen);				
				
				nLastStartPos = nCheckPoint;				
				nCheckPoint = nCheckPoint;
			}		

			chColorSave = StringCullingAddVector_ToNormal(chColorSave, chCullingBuff, vecDstString);
			ZERO_MEMORY(chCullingBuff);				
			
			if(bBreak)
			{
				// 공백을 못찾고 최대 픽셀수를 넘었다.
				nCullingLen = pNextStr - (strTalk + nLastStartPos);
				if(nCullingLen)
				{
					strncpy(chCullingBuff, &strTalk[nLastStartPos], nCullingLen);			
					chColorSave = StringCullingAddVector_ToNormal(chColorSave, chCullingBuff, vecDstString);
					ZERO_MEMORY(chCullingBuff);	
				}
				
			}
			bFindBlank = FALSE;			
			
			nLastCullingLen = 0;
			nCullingPos = 0;
		}		
		else if(bBreak)
		{
			// 공백을 못찾고 최대 픽셀수를 넘었다.
			nCullingLen = pNextStr - (strTalk + nLastStartPos);
			if(nCullingLen)
			{
				strncpy(chCullingBuff, &strTalk[nLastStartPos], nCullingLen);			
				chColorSave = StringCullingAddVector_ToNormal(chColorSave, chCullingBuff, vecDstString);			
			}
		}
		else if(bFindBlank)
		{	
			nLastCullingLen = nCullingLen;
			nCullingPos = nCheckPoint;
		}		

		if(bBreak)
		{
			break;
		}

	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief		Enter처리를 위한 컬링함수 ("\\n 이라는 스트링이 찍히므로 잘 처리해야함)
/// \author		// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
/// \date		2008-07-24 ~ 2008-07-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char CINFGameMain::StringCullingAddVector_ToNormal(char chColorSave, char* pString, vector<string> *vecDstString)
{
	char chCullingTmpBuff[2048];
	// 2008-07-24 by bhsohn 컬링 관련 버그 수정
	char chFindColor = GetFindColorOnTxt(pString);				

	ZERO_MEMORY(chCullingTmpBuff);
	// 2008-07-24 by bhsohn 컬링 관련 버그 수정
	//if(0 == chFindColor && chColorSave )
	if(chColorSave )
	{
		// 칼러가 없으면 강제로 칼라 넣어줌
		strncpy(chCullingTmpBuff, pString, 2048);
		sprintf(pString, "\\%c%s", chColorSave, chCullingTmpBuff);
		chFindColor = GetFindColorOnTxt(pString);				
		chColorSave = chFindColor;
	}
	else if('n' == chFindColor)
	{
		chColorSave = 0;	// \\n 은 색이 아니다.
	}
	else if(0 != chFindColor )
	{
		// 라인컬러 저장
		chColorSave = chFindColor;
	}

	// 컬링을 원하는 픽셀보다 큰지 체크한다.
	vecDstString->push_back(pString);

	return chColorSave;	
}
						  
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL StringCullingUserData_ToBlank( char* strTalk, int nCullPixel, vector<string> *vecStorage, CD3DHanFont* pHanFont)
/// \brief		Enter처리를 위한 컬링함수 ("\\n 이라는 스트링이 찍히므로 잘 처리해야함)
/// \author		// 2008-10-13 by bhsohn 편지 읽기 Return 안되는 문제 처리
/// \date		2008-07-24 ~ 2008-07-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char CINFGameMain::DelCharacterRetrn(char* o_pTxt)
{
	int nCnt = 0;
	int nRtnPos = -1;
	int nStringLen = strlen(o_pTxt);
	BOOL bCharcterRtn = FALSE;
	
	for(nCnt = 0;nCnt < nStringLen;nCnt++)
	{
		if('\\' == o_pTxt[nCnt])
		{
			bCharcterRtn = TRUE;
		}
		else if(bCharcterRtn)
		{
			bCharcterRtn = FALSE;
			if('n' == o_pTxt[nCnt])
			{
				nRtnPos = nCnt;
			}
		}		
	}
	if(nRtnPos >= 1)
	{
		o_pTxt[nRtnPos-1] = NULL;
		o_pTxt[nRtnPos] = NULL;
	}
	
	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	int nIdx = strlen(o_pTxt)-1;	// 마지막 스트링값
	if(nIdx < 0)
    {
		nIdx = 0;
    }
	return o_pTxt[nIdx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-10-15 by bhsohn 리소스 메모리 보호 기능 추가
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::CreateChatChild_OperationMode(TCHAR *str,int type, int chatType/* = CHAT_TAB_SYSTEM*/, char* szWisperToCharacterName, char *szWisperFromCharacterName)
{
	// 관리자가 아니면 찍지 말아라
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if (!COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		return FALSE;
	}
	if(g_pD3dApp && g_pD3dApp->m_pChat)
	{
		g_pD3dApp->m_pChat->CreateChatChild(str,type, chatType, szWisperToCharacterName, szWisperFromCharacterName);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-10-14 by bhsohn VOIP개선
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2016-07-19 panoskj disabled this
//void CINFGameMain::ChangeVOIPMode()
//{
//	BOOL bGetVOIPSpeaking = GetVOIPSpeaking();
//	bGetVOIPSpeaking^= TRUE;	
//
//	if(bGetVOIPSpeaking)
//	{
//		COPYDATASTRUCT	copyData;
//		memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//		AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK, 0);
//		copyData.dwData = (DWORD)voIP;
//		copyData.lpData = NULL;
//		copyData.cbData = 0;
//		::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, 
//			WM_COPYDATA, 
//			(WPARAM)g_pD3dApp->GetHwnd(), 
//			(LPARAM)&copyData);
//	}
//	else
//	{
//		ReleaseVOIPMode();		
//	}	
//}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		VOIP종료
/// \author		// 2008-10-14 by bhsohn VOIP개선
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2016-07-19 panoskj disabled this
//void CINFGameMain::ReleaseVOIPMode()
//{
//	BOOL bGetVOIPSpeaking = GetVOIPSpeaking();
//	if(!bGetVOIPSpeaking)
//	{
//		return;
//	}
//	COPYDATASTRUCT	copyData;
//	memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
//	AV_MSG_WM_COPYDATA voIP(_NtoNCall, AV_MT_TALK_CANCEL, 0);
//	copyData.dwData = (DWORD)voIP;
//	copyData.lpData = NULL;
//	copyData.cbData = 0;
//	::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-10-14 by bhsohn VOIP개선
/// \date		2008-10-15 ~ 2008-10-15
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////////
//void CINFGameMain::SetVOIPSpeaking(BOOL i_bVOIPSpeaking)
//{
//	m_bVOIPSpeaking = i_bVOIPSpeaking;
//}
/////////////////////////////////////////////////////////////////////////////////
///// \fn			
///// \brief		
///// \author		// 2008-10-14 by bhsohn VOIP개선
///// \date		2008-10-15 ~ 2008-10-15
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CINFGameMain::GetVOIPSpeaking()
//{
//	return m_bVOIPSpeaking;
//}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
/// \date		2008-11-3 ~ 2008-11-3
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickServerTime()
{
	if(NULL == m_pINFGameMainSysMsg)
	{
		return;
	}

	BOOL bShow = m_pINFGameMainSysMsg->IsShowWnd();
	bShow ^= TRUE;
	ShowSysMsgWnd(bShow);

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
/// \date		2008-11-3 ~ 2008-11-3
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ShowSysMsgWnd(BOOL bShowWnd)
{
	if(NULL == m_pINFGameMainSysMsg)
	{
		return;
	}
	POINT ptPos = m_pINFGameMainSysMsg->GetWndPos();
	if(0 >= ptPos.x && 0 >= ptPos.y)
	{
		ptPos.x = GAMEMAIN_SERVER_TIME_X;
		ptPos.y = GAMEMAIN_SERVER_TIME_Y;
	}	

	int nCapWidth = GAMEMAIN_SERVER_TIME_WIDTH;

	if(bShowWnd)
	{
		ATUM_DATE_TIME curServerTime = GetServerDateTime();

		string szStrBuf;
		curServerTime.GetLocalString_YYYYMMDD(curServerTime.Year,
			curServerTime.Month, 
			curServerTime.Day, 
			szStrBuf);	
		int nHour = curServerTime.Hour;
		int nMinute = curServerTime.Minute;

		char chTimeString[MAX_PATH];
		char chAmPm[32];
		// 2008. 11. 21 서버 시간 보여주기, 박스 수정
		char chTime[256];
		memset(chTimeString, 0x00, MAX_PATH);

		wsprintf(chAmPm, STRMSG_C_081103_0200);	//"AM"
		if(nHour > 12)
		{
			nHour -= 12;
			wsprintf(chAmPm, STRMSG_C_081103_0201);	//"PM"
		}

		// 2008. 11. 21 서버 시간 보여주기, 박스 수정
		wsprintf(chTimeString, "%s", (char*)szStrBuf.c_str());
		wsprintf(chTime, "%s %02d:%02d", chAmPm, nHour, nMinute);
// 		wsprintf(chTimeString, "%s %s %02d:%02d", 
// 			(char*)szStrBuf.c_str(), 
// 			chAmPm,
// 			nHour, 
// 			nMinute);
		
		
		m_pINFGameMainSysMsg->InitString();		
		m_pINFGameMainSysMsg->SetString(STRMSG_C_081103_0202);
		m_pINFGameMainSysMsg->SetString(chTimeString);
		// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
		m_pINFGameMainSysMsg->SetString(chTime);

	}
	
	m_pINFGameMainSysMsg->ShowWnd(bShowWnd, &ptPos, nCapWidth);
}
// 2012-03-16 by jhahn F키로 숨겨도 시계가 보이게 변경
void CINFGameMain::RenderSysTimer()
{
	RenderSysMsgWnd();

}
//end 2012-03-16 by jhahn F키로 숨겨도 시계가 보이게 변경
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
/// \date		2008-11-3 ~ 2008-11-3
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderSysMsgWnd()
{
	if(NULL == m_pINFGameMainSysMsg )
	{
		return;
	}
	else if(!m_pINFGameMainSysMsg->IsShowWnd() )
	{
		return;
	}

	// 2012-12-10 by jhjang 시간 표시 수정
// 	ATUM_DATE_TIME curServerTime = GetServerDateTime();
// 
// 	string szStrBuf;
// 	curServerTime.GetLocalString_YYYYMMDD(curServerTime.Year,
// 		curServerTime.Month, 
// 		curServerTime.Day, 
// 		szStrBuf,
// 		GetLanguageType());	
// 	int nHour = curServerTime.Hour;
// 	int nMinute = curServerTime.Minute;
// 	
// 	char chTimeString[MAX_PATH];
// 	char chAmPm[32];
// 	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
// 	char chTime[256];
// 
// 	memset(chTimeString, 0x00, MAX_PATH);
// 	
// 	// 2009. 05. 28 by ckPark 서버 시간 AM PM 버그 수정
// // 	wsprintf(chAmPm, STRMSG_C_081103_0200);	//"AM"
// // 	if(nHour > 12)
// // 	{
// // 		nHour -= 12;
// // 		wsprintf(chAmPm, STRMSG_C_081103_0201);	//"PM"
// // 	}
// 
// 	if( 0 <= nHour && nHour <= 11)
// 		wsprintf(chAmPm, STRMSG_C_081103_0200);	// AM
// 	else
// 		wsprintf(chAmPm, STRMSG_C_081103_0201); // PM
// 	// end 2009. 05. 28 by ckPark 서버 시간 AM PM 버그 수정
// 	
// 	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
// 	wsprintf(chTimeString, "%s", (char*)szStrBuf.c_str());
// 	wsprintf(chTime, "%s %02d:%02d", chAmPm, nHour, nMinute);
// 
// 	m_pINFGameMainSysMsg->UpdateString(1, chTimeString);
// 	m_pINFGameMainSysMsg->UpdateString(2, chTime);
// // 	wsprintf(chTimeString, "%s %s %02d:%02d", 
// // 		(char*)szStrBuf.c_str(), 
// // 		chAmPm,
// // 		nHour, 
// // 		nMinute);
// // 	m_pINFGameMainSysMsg->UpdateString(1, chTimeString);
	// end 2012-12-10 by jhjang 시간 표시 수정

	m_pINFGameMainSysMsg->Render();

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn EP3-2 에서 다중 선택 판매 안되는 버그 수정
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ClearSetIconInfo()
{
	POINT ptNull;
	ptNull.x = ptNull.y = 0;
	SetSelectItem(NULL, ptNull, ITEM_NOT_POS);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ShowOpJoystick(BOOL bShow)
{
	if(NULL == m_pINFOpJoystick)
	{
		return;
	}
	if(bShow)
	{
		POINT ptPos;

		ptPos.x = JOYSTICK_WND_POS_X;
		ptPos.y = JOYSTICK_WND_POS_Y;

		// 최우선 순위로
		g_pInterface->SetWindowOrder(WNDOpJoystick);// 조이스틱 옵션
		m_pINFOpJoystick->ShowWnd(TRUE, &ptPos);
	}
	else
	{
		m_pINFOpJoystick->ShowWnd(FALSE, NULL);
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::IsShowOpJoystick()
{
	if(NULL == m_pINFOpJoystick)
	{
		return FALSE;
	}
	return m_pINFOpJoystick->IsShowWnd();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcOpJoystickWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_pINFOpJoystick)
	{
		return INF_MSGPROC_NORMAL;
	}	
	return m_pINFOpJoystick->WndProc(uMsg, wParam, lParam);	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderOpJoystickWnd()
{
	if(!m_pINFOpJoystick)
	{
		return;
	}	
	m_pINFOpJoystick->Render();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		다른 이름으로 조이스틱 정보 저장
/// \author		// 2008-11-13 by bhsohn 조이스틱 작업
/// \date		2008-11-13 ~ 2008-11-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SaveAsJoysticSetupInfo(char* pSaveAsFilename)
{
	if(!m_pINFOpJoystick)
	{
		return;
	}
	m_pINFOpJoystick->SaveAsJoysticSetupInfo(pSaveAsFilename);

}



// 2008. 12. 16 by ckPark 아이템 사용 지연
///////////////////////////////////////////////////////////////////////////////
/// \fn		: CINFGameMain::PushDelayItem
/// \brief	: 사용할 지연 아이템을 큐에 넣는다
///			  아이템이 널이거나 이미 아이템 사용대기중일 경우
///			  FALSE 리턴
/// \author	: ckPark
/// \date	: 16:12:2008   14:59
/// \warning: 
/// \param	: const CItemInfo * pItemInfo
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMain::PushDelayItem(CItemInfo* pItemInfo)
{
	// 아이템 정보가 잘못됬�?경우
	if(!pItemInfo)
		return FALSE;
	
	// 리스트를 전부 순회하며
	std::list<ITEM_DELAY>::iterator end = m_listItemDelay.end();
	for(std::list<ITEM_DELAY>::iterator it = m_listItemDelay.begin(); it != end; ++it)
	{
		// 이미 같은 유니크넘버로 리스트에 있을 경우
		if( (*it).nItemUniqueNumber == pItemInfo->UniqueNumber )
		{
			// 이미 사용중입니다
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081216_0303, COLOR_ITEM);
			return FALSE;
		}
	}
	
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	// 만약 도시 맵일 경우
	if(IS_CITY_MAP_INDEX(myShuttleInfo.MapChannelIndex.MapIndex))
	{
		// 리스트에 푸쉬하지 않고 바로 보낸다
		g_pGameMain->m_pInven->SendUseItem( static_cast<CItemInfo*>(pItemInfo) );
	}
	else
	{
		// 도시 맵이 아닌 경우
		ITEM_DELAY	item_delay;
		item_delay.nItemUniqueNumber	= pItemInfo->UniqueNumber;
		item_delay.nItemUseTime			= timeGetTime();
		item_delay.nItemDelayTime		= pItemInfo->GetItemInfo()->AttackTime;
		
		// 리스트에 푸쉬
		m_listItemDelay.push_back(item_delay);
	}

	// xx아이템을 발동 중입니다
	char szBuff[256];
	sprintf(szBuff, STRMSG_C_081216_0301, pItemInfo->GetItemInfo()->ItemName);
	g_pD3dApp->m_pChat->CreateChatChild(szBuff, COLOR_ITEM);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CINFGameMain::ReleaseDelayItem
/// \brief	: 딜레이아이템 리스트를 클리어한다( ex> 죽었을 때)
/// \author	: ckPark
/// \date	: 16:12:2008   16:34
/// \warning: 
/// \param	: void
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ReleaseDelayItem(void)
{
	m_listItemDelay.clear();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn		: CINFGameMain::TickItemDelay(void)
/// \brief	: 딜레이아이템 리스트를 클리어한다( ex> 죽었을 때)
/// \author	: ckPark
/// \date	: 16:12:2008   16:34
/// \warning: 
/// \param	: void
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::TickItemDelay(void)
{
	if(m_listItemDelay.empty())
	{
		return;
	}
	// 현재 시간
	DWORD	dwCurTime = timeGetTime();
	
	std::list<ITEM_DELAY>::iterator end		= m_listItemDelay.end();
	std::list<ITEM_DELAY>::iterator it		= m_listItemDelay.begin();
	while(it != end)
	{
		// 현재시간이 아이템 딜레이시간 + 사용한 시간 보다 적을 경우 다음 아이템으로
		if(dwCurTime < (*it).nItemDelayTime + (*it).nItemUseTime)
			++it;
		else
		{
			// 현재시간이 아이템 딜레이시간 + 사용한 시간 보다 클 경우 아이템을 실제 사용한다
			// 유니크 넘버로 인벤에서 아이템을 찾는다
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( (*it).nItemUniqueNumber );
			// 인벤에서 찾았을 경우만
			if(pItemInfo)
			{
				// 아이템 사용 패킷 전송
				g_pGameMain->m_pInven->SendUseItem( static_cast<CItemInfo*>( pItemInfo ) );
				
				// xx아이템을 사용합니다.
				char szBuff[256];
				sprintf(szBuff, STRMSG_C_081216_0302, pItemInfo->GetItemInfo()->ItemName);
				g_pD3dApp->m_pChat->CreateChatChild(szBuff, COLOR_ITEM);
			}
			
			// 딜레이 시간이 지난 아이템은 서버로 패킷을 보내든 안보내든 삭제한다
			it = m_listItemDelay.erase(it);
		}
	}
}
// end 2008. 12. 16 by ckPark 아이템 사용 지연







// 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연
BOOL CINFGameMain::PushDelaySkill(CSkillInfo* pSkillInfo, UID32_t nTargetUniqueNumber)
{
	if(!pSkillInfo)
		return FALSE;

	std::list<SKILL_DELAY>::iterator end = m_listSkillDelay.end();
	for(std::list<SKILL_DELAY>::iterator it = m_listSkillDelay.begin(); it != end; ++it)
	{
		if( (*it).pSkillInfo == pSkillInfo )
		{
			// 이미 사용중입니다
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081216_0303, COLOR_ITEM);
			return FALSE;
		}
	}

	SKILL_DELAY		skill_delay;

	skill_delay.pSkillInfo				= pSkillInfo;
	skill_delay.nSkillUseTime			= timeGetTime();
	skill_delay.nSkillDelayTime			= pSkillInfo->ItemInfo->AttackTime;
	skill_delay.nTargetUniqueNumber		= nTargetUniqueNumber;

	m_listSkillDelay.push_back(skill_delay);

	return TRUE;
}

void CINFGameMain::ReleaseDelaySkill(void)
{
	m_listSkillDelay.clear();
}

void CINFGameMain::TickSkillDealy(void)
{
	if( m_listSkillDelay.empty( ) )
		return;

	DWORD dwCurTime = timeGetTime();

	std::list<SKILL_DELAY>::iterator end	= m_listSkillDelay.end();
	std::list<SKILL_DELAY>::iterator it		= m_listSkillDelay.begin();

	while(it != end)
	{
		if(dwCurTime < (*it).nSkillDelayTime + (*it).nSkillUseTime )
			++it;
		else
		{
			// 2009. 04. 06 by ckPark 콜오브히어로시 다른 타겟팅 스킬 못쓰는 문제
			g_pShuttleChild->m_pSkill->SetCharacterUID( (*it).nTargetUniqueNumber );
			g_pGameMain->m_pCharacterInfo->SendUseSkill( (*it).pSkillInfo );
			//g_pD3dApp->m_pShuttleChild->m_pSkill->SendConfirmSkillUse( (*it).pSkillInfo, (*it).nTargetUniqueNumber );
			// end 2009. 04. 06 by ckPark 콜오브히어로시 다른 타겟팅 스킬 못쓰는 문제
			it = m_listSkillDelay.erase( it );
			break;
		}
	}
}
// end 2008. 12. 23 by ckPark 스킬 사용 지연 사용 지연


// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
CINFImageEx*	CINFGameMain::FindBigIcon( int nItemnum )
{
	return m_pItemInfo->FindBigIcon( nItemnum );
}
// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


// 2010. 02. 11 by ckPark 발동류 장착아이템
void	CINFGameMain::PushInvokeWearItem( UID64_t nItemUID )
{
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( nItemUID );
	// 아이템 정보가 없거나 쿨타임이 0이하일 경우는 리스트에서 제외한다
	if( !pItemInfo || pItemInfo->GetCoolElapsedTime() <= 0 )
	{
		PopInvokeWearItem( nItemUID );
		return;
	}

	std::list<INVOKEITEM_COOLTIME>::iterator it = m_listInvokeItemCoolTime.begin();
	while( it != m_listInvokeItemCoolTime.end() )
	{
		// 이미 존재하는 쿨타임 리스트에 대해서는 값을 갱신만 한다
		if( nItemUID == (*it).nItemUID )
		{
			(*it).nItemUID				= nItemUID;
			(*it).nPrevTickTime			= timeGetTime();
			(*it).bSendEndCoolTime		= FALSE;
			return;
		}

		++it;
	}

	// 쿨타임 리스트에 추가
	INVOKEITEM_COOLTIME item;
	item.nItemUID				= nItemUID;
	item.nPrevTickTime			= timeGetTime();
	item.bSendEndCoolTime		= FALSE;
	m_listInvokeItemCoolTime.push_back( item );
}

void	CINFGameMain::PopInvokeWearItem( UID64_t nItemUID )
{
	// 쿨타임 리스트에 제거
	std::list<INVOKEITEM_COOLTIME>::iterator it = m_listInvokeItemCoolTime.begin();
	while( it != m_listInvokeItemCoolTime.end() )
	{
		if( nItemUID == (*it).nItemUID )
		{
			it = m_listInvokeItemCoolTime.erase( it );
			return;
		}

		++it;
	}
}

void	CINFGameMain::TickInvokeWearItem( void )
{
	std::list<INVOKEITEM_COOLTIME>::iterator it = m_listInvokeItemCoolTime.begin();
	while( it != m_listInvokeItemCoolTime.end() )
	{
		CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( (*it).nItemUID );
		if( pItemInfo
			&& (*it).nItemUID == pItemInfo->UniqueNumber
			&& pItemInfo->Wear == WEAR_ATTACHED
			&& (pItemInfo->ItemInfo->InvokingDestParamID || pItemInfo->ItemInfo->InvokingDestParamIDByUse) )
		{
			INT nElapsedTime		= pItemInfo->GetCoolElapsedTime();
			DWORD nCurTime			= timeGetTime();
			nElapsedTime			+= (nCurTime - (*it).nPrevTickTime);
			pItemInfo->SetCoolElapsedTime( nElapsedTime );
			(*it).nPrevTickTime		= nCurTime;

			// 만약 쿨타임이 다지났다면 서버로 전송
			// 연속 2번 보내지 않는다
			if( !((*it).bSendEndCoolTime) && (nElapsedTime > pItemInfo->ItemInfo->ReAttacktime ) )
			{
				MSG_FC_ITEM_END_COOLINGTIME_ITEM msg;
				msg.ItemUID	= pItemInfo->UniqueNumber;
				g_pFieldWinSocket->SendMsg( T_FC_ITEM_END_COOLINGTIME_ITEM, (char*)(&msg), sizeof( MSG_FC_ITEM_END_COOLINGTIME_ITEM ) );

				(*it).bSendEndCoolTime	= TRUE;
			}
		}

		++it;
	}
}

void	CINFGameMain::SetItemCoolStart( UID64_t nItemUID )
{
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( nItemUID );
	if( pItemInfo )
	{
		if( pItemInfo->ItemInfo->ReAttacktime == 0 )
			return;
		
		// 쿨타임을 1(0.001초)로 셋팅함으로 쿨타임을 시작한다
		// 0은 쿨타임이 아예없다는 뜻....
		pItemInfo->SetCoolElapsedTime( 1 );

		// 장착된 아이템이면 쿨타임리스트에 추가한다
		if( pItemInfo && pItemInfo->Wear == WEAR_ATTACHED )
			PushInvokeWearItem( nItemUID );
	}
}

void	CINFGameMain::SetItemCoolEnd( UID64_t nItemUID )
{
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( nItemUID );
	// 쿨타임을 0으로 셋팅함으로 쿨타임 종료
	// 0은 쿨타임이 아예없다는 뜻....
	if( pItemInfo )
		pItemInfo->SetCoolElapsedTime( 0 );

	// 쿨타임 리스트에 제거
	PopInvokeWearItem( nItemUID );
}

void	CINFGameMain::RefreshInvokeItemTickTime( UID64_t nItemUID )
{
	std::list<INVOKEITEM_COOLTIME>::iterator it = m_listInvokeItemCoolTime.begin();
	// 맨 마지막 쿨타임 계산 시간을 설정
	while( it != m_listInvokeItemCoolTime.end() )
	{
		if( (*it).nItemUID == nItemUID )
		{
			(*it).nPrevTickTime	= timeGetTime();
			return;
		}

		++it;
	}
}
// end 2010. 02. 11 by ckPark 발동류 장착아이템


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
/// \date		2009-01-30 ~ 2009-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::GetKeyDownToNumber(LPARAM i_lParam)
{
	int nLen = sizeof(m_stucTable)/sizeof(structNumTable);
	int nCnt =0;
	int nRtn = -1;
	for(nCnt =0;nCnt<nLen;nCnt++)
	{
		if(i_lParam == m_stucTable[nCnt].dwKeyValue)
		{
			nRtn = m_stucTable[nCnt].nKey;
			break;
		}
		
	}
	return nRtn;
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2012-12-10 by jhjang 아레나 암호 영문도 가능하게 입력 수정 
/// \date		2012-12-10 ~ 2012-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char CINFGameMain::GetKeyDownToNumberAndAlphabet(WPARAM wParam)
{
	if((0x30 <= wParam && wParam <= 0x39) ||	//키 값의 영역이 0~9까지일때
		(0x41 <= wParam && wParam <= 0x5A) ||	//키 값이 A~Z일때
		(0x61 <= wParam && wParam <= 0x7A))		//키 값이 a~z일때
	{
		return wParam;
	}
	return -1;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, BOOL bShowMyEq/*=TRUE*/, int nLinkItem/*=0*/, BOOL bInven /*FALSE*/)  // 2013-06-26 by ssjung 인벤토리 추가 툴팁 
{	
	// 일반 툴팁
	m_pItemInfo->SetItemInfoUser( pItemInfo, x, y ,0,bInven);	   // 2013-06-26 by ssjung 인벤토리 추가 툴팁 

	m_bShowMyEqTooltip = bShowMyEq; // 내 장비 보이는지 여부
	if(NULL == pItemInfo)
	{
		m_pEquipItemInfo->SetItemInfoUser( pItemInfo, x, y);
	}
	else if(GetAsyncKeyState(VK_SHIFT) && bShowMyEq)
	{			
		ITEM* pITEM = pItemInfo->GetItemInfo();
		CItemInfo* pMyEqITEM = GetItem_To_MyEqITEM(pITEM);
		if(NULL == pMyEqITEM)
		{
			// 초기화
			m_pEquipItemInfo->SetItemInfoUser( NULL, 0, 0);
			RefreshItemTooltipPos();
			return;
		}
		
		if(pMyEqITEM)
		{
			int nPosX = m_pItemInfo->GetMaxLength();
			if(nPosX > 0)
			{
				nPosX += 12;	// 12만큼 더 가자
			}
			nPosX += x;	
			int nPosY = y;	
			m_pEquipItemInfo->SetItemInfoUser( pMyEqITEM, nPosX, nPosY);						
		}
	}
	else
	{
		// 초기화
		m_pEquipItemInfo->SetItemInfoUser( NULL, 0, 0);
	}
	RefreshItemTooltipPos();
	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ShowItemInfo(BOOL bShow)
{
	m_pItemInfo->ShowItemInfo(bShow);
	m_pEquipItemInfo->ShowItemInfo(bShow);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::SetItemInfoNormal( ITEM* pITEM, int x, int y, BOOL bShop, int nItemCount /*= 0*/, BOOL bShowMyEq/*=TRUE*/, int nLinkItem/*=0*/, BOOL ArmorCollect /*FALSE*/)// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
{	
	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//	m_pItemInfo->SetItemInfoNormal( pITEM, x, y, bShop, nItemCount );
	m_pItemInfo->SetItemInfoNormal( pITEM, x, y, bShop, nItemCount, nLinkItem,ArmorCollect);// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

	m_bShowMyEqTooltip = bShowMyEq; // 내 장비 보이는지 여부

	if(NULL == pITEM)
	{
		m_pEquipItemInfo->SetItemInfoNormal( pITEM, x, y, bShop, nItemCount );
	}
	else if(GetAsyncKeyState(VK_SHIFT) && bShowMyEq)
	{
		CItemInfo* pMyEqITEM = GetItem_To_MyEqITEM(pITEM);
		if(NULL == pMyEqITEM)
		{
			m_pEquipItemInfo->SetItemInfoNormal( NULL, 0, 0, 0, 0);
			RefreshItemTooltipPos();
			return;
		}

		int nPosX = m_pItemInfo->GetMaxLength();
		if(nPosX > 0)
		{
			nPosX += 12;	// 12만큼 더 가자
		}
		nPosX += x;	
		int nPosY = y;		
		
		m_pEquipItemInfo->SetItemInfoNormal( pMyEqITEM->GetItemInfo(), nPosX, nPosY, bShop, nItemCount );
	}
	else
	{
		m_pEquipItemInfo->SetItemInfoNormal( NULL, 0, 0, 0, 0);
	}
	RefreshItemTooltipPos();
	

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::ShowMyEqItemTooltip()
{
	if(NULL == m_pItemInfo)
	{
		return;
	}
	CItemInfo*	pItemInfo = m_pItemInfo->GetRefItemInfo();	
	if(NULL == pItemInfo)
	{
		return;
	}

	ITEM* pITEM = pItemInfo->GetItemInfo();
	CItemInfo* pMyEqITEM = GetItem_To_MyEqITEM(pITEM);
	if((NULL == pMyEqITEM)
		||(!m_bShowMyEqTooltip))
	{
		// 초기화
		m_pEquipItemInfo->SetItemInfoUser( NULL, 0, 0);
		RefreshItemTooltipPos();
		return;
	}
	
	POINT ptPos = m_pItemInfo->GetItemInfoPos();
	if(pMyEqITEM)
	{
		int nPosX = m_pItemInfo->GetMaxLength();
		if(nPosX > 0)
		{
			nPosX += 12;	// 12만큼 더 가자
		}
		nPosX += ptPos.x;	
		int nPosY = ptPos.y;	
		m_pEquipItemInfo->SetItemInfoUser( pMyEqITEM, nPosX, nPosY);		
	}
	RefreshItemTooltipPos();

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderItemInfo()
{
	if(m_pItemInfo)
	{
		m_pItemInfo->Render();
	}
	if(m_pEquipItemInfo)
	{
		m_pEquipItemInfo->Render();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
/////////////////////////////////////////////////////////////////////////////// 
CItemInfo* CINFGameMain::GetItem_To_MyEqITEM(ITEM* i_pItem)
{
	if(NULL == i_pItem)
	{
		return NULL;
	}

	int nWindowPos = 0;
	
	if(IS_PRIMARY_WEAPON(i_pItem->Kind))
	{
		// 1형 무기(선두 좌측)	--> 좌측 1번
		nWindowPos = POS_PROWOUT;
	}
	else if(IS_SECONDARY_WEAPON(i_pItem->Kind))
	{		
		// 2형 무기(선두 우측)	--> 우측 1번
		nWindowPos = POS_WINGOUT;

	}
	else if(ITEMKIND_DEFENSE == i_pItem->Kind)
	{
		// 아머(중앙 가운데)	--> 좌측 4번
		nWindowPos = POS_CENTER;
	}
	else if(ITEMKIND_SUPPORT == i_pItem->Kind)
	{
		// 엔진(후미 가운데)	--> 우측 3번
		nWindowPos = POS_REAR;

	}
	else if(ITEMKIND_RADAR == i_pItem->Kind)
	{
		// 레이더(선두 가운데)	--> 좌측 2번
		nWindowPos = POS_PROW;
	}
	else if(ITEMKIND_COMPUTER == i_pItem->Kind)
	{		
		// 컴퓨터(중앙 좌측)	--> 우측 2번
		nWindowPos = POS_PROWIN;
	}
	else if(ITEMKIND_ACCESSORY_UNLIMITED == i_pItem->Kind)
	{
		// 무제한 악세사리 - 부착물(후미 우측-연료탱크|컨테이너계열) --> 좌측 3번
		nWindowPos = POS_ACCESSORY_UNLIMITED;
	}
	else if(ITEMKIND_ACCESSORY_TIMELIMIT == i_pItem->Kind)
	{
		// 시간제한 악세사리(후미 좌측)	--> 우측 4번
		nWindowPos = POS_ACCESSORY_TIME_LIMIT;
	}
	else
	{
		return NULL;
	}

	// 인벤에서 찾는다.
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos(nWindowPos);
	if(NULL == pItemInfo)
	{
		return NULL;
	}
	return pItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RefreshItemTooltipPos()
{
	if(!m_pEquipItemInfo || !m_pEquipItemInfo->IsShowItemInfo())
	{
		return;
	}
	// 크기 먼제 처리
	
	{
		SIZE szItemInfo = m_pItemInfo->GetItemInfoTooltipSize();
		SIZE szMyEqItemInfo = m_pEquipItemInfo->GetItemInfoTooltipSize();
		if(szItemInfo.cx > szMyEqItemInfo.cx)
		{
			szMyEqItemInfo.cx = szItemInfo.cx;
		}
		else if(szItemInfo.cx < szMyEqItemInfo.cx)
		{
			szItemInfo.cx = szMyEqItemInfo.cx;
		}

		if(szItemInfo.cy > szMyEqItemInfo.cy)
		{
			szMyEqItemInfo.cy = szItemInfo.cy;
		}
		else if(szItemInfo.cy < szMyEqItemInfo.cy)
		{
			szItemInfo.cy = szMyEqItemInfo.cy;
		}
		m_pItemInfo->SetItemInfoTooltipSize(szItemInfo);
		m_pEquipItemInfo->SetItemInfoTooltipSize(szMyEqItemInfo);
	}

	// 위치 갱신
	{
		POINT ptItemInfo = m_pItemInfo->GetItemInfoPos();
		SIZE szItemInfo = m_pItemInfo->GetItemInfoTooltipSize();
		
		POINT ptMyEqPos = m_pEquipItemInfo->GetItemInfoPos();				
		SIZE szMyEqItemInfo = m_pEquipItemInfo->GetItemInfoTooltipSize();

		if((ptItemInfo.x + szItemInfo.cx) > ptMyEqPos.x)
		{
			ptMyEqPos.x = ptItemInfo.x - szMyEqItemInfo.cx;
		}
		
		if((ptItemInfo.y + szItemInfo.cy) > g_pD3dApp->GetBackBufferDesc().Height)
		{
			ptItemInfo.y = g_pD3dApp->GetBackBufferDesc().Height - szItemInfo.cy;
		}
		if((ptMyEqPos.y + szMyEqItemInfo.cy) > g_pD3dApp->GetBackBufferDesc().Height)
		{
			ptMyEqPos.y = g_pD3dApp->GetBackBufferDesc().Height - szMyEqItemInfo.cy;
		}

		m_pItemInfo->SetItemInfoPos(ptItemInfo);
		m_pEquipItemInfo->SetItemInfoPos(ptMyEqPos);
	}

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-16 ~ 2009-02-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickWorldRankBoard()
{
	if(NULL == m_pINFWorldRankWnd)
	{
		return;
	}
	BOOL bShow = m_pINFWorldRankWnd->IsShowWnd();
	bShow = TRUE;	
	m_pINFWorldRankWnd->ShowWnd(bShow);		

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-16 ~ 2009-02-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMain::WndProcWorldRank(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if(NULL == m_pINFWorldRankWnd)
	{
		return INF_MSGPROC_NORMAL;
	}
	return m_pINFWorldRankWnd->WndProc(uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-16 ~ 2009-02-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RenderWorldRank()
{
	if(NULL == m_pINFWorldRankWnd)
	{
		return;
	}
	m_pINFWorldRankWnd->Render();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-13 by bhsohn 월드 랭킹 시스템
/// \date		2009-02-16 ~ 2009-02-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::RqWorldRankInfo()
{
	if(NULL == m_pINFWorldRankWnd)
	{
		return;
	}
	m_pINFWorldRankWnd->RqWorldRankInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-02-20 by bhsohn 인게임 조합창 처리
/// \date		2013-02-20 ~ 2013-02-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickItemMixWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum)
{
	if(NULL == m_pINFItemMixWnd)
	{
		return;
	}		
	POINT ptInvenItem = {0,0};
	POINT ptMixWndPos = {0,0};

	if(bShow)
	{		
		int nCap = 15;
		ptInvenItem.x = g_pD3dApp->GetBackBufferDesc().Width/2 - (m_pInven->GetBkSize().x + m_pINFItemMixWnd->GetBkSize().x)/2;
		ptInvenItem.x -= nCap; // 15픽셀정도 띠우자
		ptInvenItem.y = g_pD3dApp->GetBackBufferDesc().Height/2 - m_pInven->GetBkSize().y / 2; 	
		m_pInven->SetBkPos(ptInvenItem);	

		ptMixWndPos.x = ptInvenItem.x+m_pInven->GetBkSize().x;
		ptMixWndPos.x += nCap;
		ptMixWndPos.y = g_pD3dApp->GetBackBufferDesc().Height/2 - m_pInven->GetBkSize().y / 2; 	
	}		

	m_pINFItemMixWnd->ShowWnd(bShow, nShowItemNum, uItemUniNum, &ptMixWndPos);
	
}

int CINFGameMain::WndProcItemMixWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if(NULL == m_pINFItemMixWnd)
	{
		return INF_MSGPROC_NORMAL;
	}
	return m_pINFItemMixWnd->WndProc(uMsg, wParam, lParam);
}

void CINFGameMain::RenderItemMixWnd()
{
	if(NULL == m_pINFItemMixWnd)
	{
		return;
	}
	m_pINFItemMixWnd->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
/// \date		2013-05-28 ~ 2013-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMain::OnClickArmorCollectionWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum)
{
	if(NULL == m_pINFArmorCollectWnd)
	{
		return;
	}		
	POINT ptInvenItem = {0,0};
	POINT ptMixWndPos = {0,0};
	
	if(bShow)
	{		
		if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
		{
			m_pINFArmorCollectWnd->ShowWnd(FALSE, 0, 0, NULL);
			return;
		}

		const int ARROR_COLLECT_WIDTH	= 762;
		const int ARROR_COLLECT_HEIGHT	= 645;

		m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);			
		
		ptMixWndPos.x = g_pD3dApp->GetBackBufferDesc().Width/2 - ARROR_COLLECT_WIDTH/2;
		ptMixWndPos.y = g_pD3dApp->GetBackBufferDesc().Height/2 - ARROR_COLLECT_HEIGHT/2 - 20;
	}		
	
	m_pINFArmorCollectWnd->ShowWnd(bShow, nShowItemNum, uItemUniNum, &ptMixWndPos);
	
}

int CINFGameMain::WndProcArmorCollectionWnd(UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if(NULL == m_pINFArmorCollectWnd)
	{
		return INF_MSGPROC_NORMAL;
	}
	return m_pINFArmorCollectWnd->WndProc(uMsg, wParam, lParam);
}

void CINFGameMain::RenderArmorCollectionWnd()
{
	if(NULL == m_pINFArmorCollectWnd)
	{
		return;
	}
	m_pINFArmorCollectWnd->Render();
}

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CINFGameMain::SetKillCount(INT nCount) // 킬 카운트 설정
{
	if(NULL == m_pUnitInfoBar) return;
	m_pUnitInfoBar->SetKillCount(nCount);

#if S_BONUS_KILL_SYSTEM_RENEWAL
	if (nCount == KILLCOUNT_MAX(g_pShuttleChild->m_myShuttleInfo.UnitKind))
#else
	if (nCount == 30)
#endif // S_BONUS_KILL_SYSTEM_RENEWAL
	{
		g_pD3dApp->SetPlayEffect(RC_EFF_KILL_COUNT_MAX,0);
	}
}
void CINFGameMain::SetRestCount(INT nCount) // 휴식 경험치 설정
{
	if(NULL == m_pUnitInfoBar) return;
	m_pUnitInfoBar->SetRestCount(nCount);
}
void CINFGameMain::SetBonusExpRate(INT nPartyRate,INT nFriendRate,INT nGuildRate) // 경험치 보너스 비율 설정
{
	if(NULL == m_pUnitInfoBar) return;
	m_pUnitInfoBar->SetBonusExpRate(nPartyRate,nFriendRate,nGuildRate);
}
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// 2012-12-10 by jhjang 시간 표시 수정
void CINFGameMain::UpdateServerTimeString()
{
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	
	int nHour = curServerTime.Hour;
	int nMinute = curServerTime.Minute;
	
	if(curServerTime.Year != m_OldServerTime.Year
		|| curServerTime.Month != m_OldServerTime.Month
		|| curServerTime.Day != m_OldServerTime.Day)
	{
		string szStrBuf;
		curServerTime.GetLocalString_YYYYMMDD(curServerTime.Year,
			curServerTime.Month, 
			curServerTime.Day, 
			szStrBuf);	
		char chDateString[MAX_PATH];
		memset(chDateString, 0x00, MAX_PATH);
		wsprintf(chDateString, "%s", (char*)szStrBuf.c_str());

		m_pINFGameMainSysMsg->UpdateString(1, chDateString);
	}

	if(curServerTime.Hour != m_OldServerTime.Hour
		|| curServerTime.Minute != m_OldServerTime.Minute)
	{
		char chAmPm[32];
		// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
		char chTimeString[MAX_PATH];
		if( 0 <= nHour && nHour <= 11)
			wsprintf(chAmPm, STRMSG_C_081103_0200);	// AM
		else
			wsprintf(chAmPm, STRMSG_C_081103_0201); // PM
		// end 2009. 05. 28 by ckPark 서버 시간 AM PM 버그 수정
		
		// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
		wsprintf(chTimeString, "%s %02d:%02d", chAmPm, nHour, nMinute);
		m_pINFGameMainSysMsg->UpdateString(2, chTimeString);
		// 2009. 05. 28 by ckPark 서버 시간 AM PM 버그 수정
	}
	m_OldServerTime = curServerTime;
}
// end 2012-12-10 by jhjang 시간 표시 수정


// 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
void CINFGameMain::TickArmorTimeCheck()
{	
	CItemInfo* pItemInfo = NULL;			
	if(!m_pInven || !m_pInven->m_pWearDisplayInfo[POS_CENTER])
	{
		return;
	}
	
	int nUniqueNumber = m_pInven->m_pWearDisplayInfo[POS_CENTER]->pItem->UniqueNumber;
	if(nUniqueNumber != 0)
	{
		pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( nUniqueNumber );
	}
	
	if(!pItemInfo || !pItemInfo->FixedTermShape.bActive || !pItemInfo->FixedTermShape.nStatLevel || SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL <= pItemInfo->FixedTermShape.nStatLevel)
//	if(!pItemInfo || pItemInfo->FixedTermShape.bIsEnd || !pItemInfo->FixedTermShape.nStatLevel || 10 <= pItemInfo->FixedTermShape.nStatLevel)
	{
		return;
	}		
	
	{
		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = ( pItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds() );
		
		int m_nDay	= (nRemainSecond ) / 86400;
		int m_nHour	= (nRemainSecond ) % 86400 / 3600;
		int m_nMin	= (nRemainSecond ) % 86400 % 3600 / 60;
		int m_nSec	= (nRemainSecond ) % 86400 % 3600 % 60 / 1;
		
		int nStrarttime = ( pItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - pItemInfo->FixedTermShape.StartDate.GetTimeInSeconds() );
		
		if ((0 == m_nDay ) && (0 == m_nHour) && m_nMin )
		{
			if(5 == m_nMin || 3 == m_nMin || 1 == m_nMin)
			{
				if(!pItemInfo->GetLimitTimeFlag())
				{
					g_pD3dApp->LookChangeLimitTimeMSG();
					pItemInfo->SetLimitTimeFlag(TRUE);
				}
			}				
			else
			{
				pItemInfo->SetLimitTimeFlag(FALSE);	
			}
		}
	}
}
// end 2013-05-09 by ssjung 외형 변경 기간만료 경고 메시지 구현
														 
// 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
INT CINFGameMain::GetArmorCollectType()
{
	return m_pINFArmorCollectWnd->GetSelUnitCollectType();
}						  
// end 2013-06-12 by ssjung 아머 컬렉션 툴팁 표시
#endif

// 2015-09-25 Future, inactivity kick
#ifdef C_DISCONNECT_INACTIVE_PLAYERS
void CINFGameMain::InitInactivityKick(int nTimeTillKick)
{
	// Check if inactivity kick was initated already
	if (m_bShowingInactivityDCWarning || !m_pInfWindow)
		return;

	// Setup the message
	char inactivityMsg[128];
	sprintf_s(inactivityMsg, sizeof(inactivityMsg), STRMSG_C_INACTIVITY_0001
		, ((DISCONNECT_INACTIVE_PLAYERS_AFTER_SECONDS - DISCONNECT_INACTIVE_PLAYERS_WARNING_TIME) / 60) + 1);

	// Add the inactivity kick window
	m_pInfWindow->AddTimeMsgBox(inactivityMsg, _Q_AUTO_KICK, DISCONNECT_INACTIVE_PLAYERS_WARNING_TIME, STRMSG_C_INACTIVITY_0002);

	// Play some button sound
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0, 0, 0), FALSE);

	// Set inactivity warning showing
	m_bShowingInactivityDCWarning = true;
}

void CINFGameMain::CancelInactivityKick()
{
	// Check if inactivity kick was initated already
	if (!m_bShowingInactivityDCWarning || !m_pInfWindow)
		return;

	// Remove the message
	m_pInfWindow->DeleteMsgBox(_Q_AUTO_KICK);

	// Set inactivity warning showing
	m_bShowingInactivityDCWarning = false;
}

#endif // C_DISCONNECT_INACTIVE_PLAYERS

BOOL CINFGameMain::ShowOnScreenDisplay()
{
	if (IS_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		return FALSE;
	}
	if (COMPARE_BODYCON_BIT(g_pShuttleChild->m_pCharacterInfo->m_nCurrentBodyCondition, BODYCON_LANDED_MASK | BODYCON_LANDING_MASK))
	{
		return FALSE;
	}
	if (g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _CITY || g_pD3dApp->m_dwGameState == _SHOP)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//2016-01-04 by St0rmy, Fix City Fly bug
bool CINFGameMain::IsShowRanking()
{
	return m_pINFWorldRankWnd->IsShowWnd();
}
