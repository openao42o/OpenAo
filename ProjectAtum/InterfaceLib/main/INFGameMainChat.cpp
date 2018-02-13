// INFGameMainChat.cpp: implementation of the CINFGameMainChat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMainChat.h"
#include "INFGameMainWisperChat.h"
#include "AtumApplication.h"
#include "EnemyData.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark

// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "Chat.h"
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "INFCommunity.h"
#include "INFCommunityParty.h"
#include "INFCommunityGuild.h"
#include "INFCommunityReject.h"
#include "INFCommunityFriend.h"
#include "INFCommunityVOIP.h"
#include "Interface.h"
#include "INFTarget.h"
#include "AtumSound.h"
#include "ClientParty.h"
#include "ObjectChild.h"
#include "Skill.h"
#include "INFOtherCharInfo.h"
#include "dxutil.h"
#include "INFGameMainFAQ.h"
#include "INFCityBase.h"
#include "AtumDatabase.h"
#include "INFTrade.h"
#include "CInput.h"
#include "INFMotherShipManager.h"

#include "TutorialSystem.h"	// 2008-10-20 by bhsohn FAQ 기체 상태에서는 안보이게 변경

// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
#include "LocalizeSetting.h"
// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가

#include "CustomOptimizer.h"

// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageList.h"
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"		  
// end 2011. 10. 10 by jskim UI시스템 변경
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

// 2015-11-2, PanKJ FreeWar
#include "FreeWar.h"

// #ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 추가 수정 채팅
	// 채팅 창부분은 CHAT_INPUT_WINDOW_X에따라 위치가 정해진다
#define CHAT_WINDOW_X			2//25												// 채팅 입력창 
#define CHAT_WINDOW_Y			(g_pD3dApp->GetBackBufferDesc().Height - 34)//27)	// 채팅 입력창
//#define CHAT_WINDOW_W			210 
//#define CHAT_WINDOW_H			22

#define CHAT_INPUT_WINDOW_X			(CHAT_WINDOW_X+50)
#define CHAT_INPUT_WINDOW_Y			(CHAT_WINDOW_Y)
#define CHAT_INPUT_WINDOW_W			240
#define CHAT_INPUT_WINDOW_H			20


#define CHAT_CHATTAB_VIEW_FLAG_X	(CHAT_WINDOW_X+329)//329)						// 채팅창 최대화 이미지
#define CHAT_CHATTAB_VIEW_FLAG_Y	(CHAT_WINDOW_Y)//-7)							// 채팅창 최대화 이미지
#define CHAT_CHATTAB_VIEW_FLAG_W	15												// 채팅창 최대화 이미지
#define CHAT_CHATTAB_VIEW_FLAG_H	21												// 채팅창 최대화 이미지

#define CHAT_MENU_VIEW_X			(CHAT_WINDOW_X - 1)								// 채팅 메뉴 보이기
#define CHAT_MENU_VIEW_Y			(CHAT_WINDOW_Y)								// 채팅 메뉴 보이기
#define CHAT_MENU_VIEW_W			21												// 채팅 메뉴 보이기
#define CHAT_MENU_VIEW_H			22												// 채팅 메뉴 보이기

#define CHAT_MENU_LIST_START_X		(CHAT_WINDOW_X+10)							// 채팅 메뉴 시작 위치
#define CHAT_MENU_LIST_START_Y		(CHAT_WINDOW_Y-1)							// 채팅 메뉴 시작 위치

#define CHAT_MENU_LIST_W			150												// 메뉴 리스트 넒이
#define CHAT_MENU_LIST_H			18												// 메뉴 리스트 높이

#define CHAT_NMENU_BUTTON_STATE_NOM	0		
#define CHAT_NMENU_BUTTON_STATE_OVE	1
#define CHAT_NMENU_BUTTON_STATE_PUH	2

#define CHAT_CASH_BUTTON_STATE_NOM	3		
#define CHAT_CASH_BUTTON_STATE_OVE	0
#define CHAT_CASH_BUTTON_STATE_PUH	1

#define CHAT_NFONT_START_LINE_X		5												// 채팅 렌더링 시작 라인
#define CHAT_NFONT_START_LINE_Y		(CHAT_FONT_START_Y-21)								// 채팅 렌더링 시작 라인
// 기타 메뉴 관련
//#define CHAT_OTHER_MENU_VOIP		(CHAT_TAB_NUMBER)
//#define CHAT_OTHER_MENU_FAQ			(CHAT_TAB_NUMBER+1)								// 가타 FAQ 메뉴
//#define CHAT_OTHER_MENU_PAYITEM		(CHAT_TAB_NUMBER+2)								// 기타 유료 아이템

#define CHAT_HELPDESK_STRLEN		280//50											// 도움말 글자르기(픽셀값으로 변경)

#define CHAR_INF_CANDIDATE_POS_X	3
#define CHAR_INF_CANDIDATE_POS_Y	(g_pD3dApp->GetBackBufferDesc().Height-35)

#define CHAT_INPUT_LANGUAGE_X		(CHAT_WINDOW_X+21)//224
#define CHAT_INPUT_LANGUAGE_Y		(CHAT_WINDOW_Y)
#define CHAT_INPUT_LANGUAGE_WH		20

// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
#define CHAT_CHATMODE_Y				(CHAT_WINDOW_Y - 20)
#define CHAT_CHATMODE_X				(CHAT_WINDOW_X)
#define CHAT_CHATMODE_GAP			37

#define CHAT_MACRO_W				(305)
#define CHAT_MACRO_H				(223)
#define CHAT_MACRO_BAR_W			(305)
#define CHAT_MACRO_BAR_H			(27)
#define CHAT_MACRO_INPUT_1_X		(m_nMacroX + 25)
#define CHAT_MACRO_INPUT_1_Y		(m_nMacroY + 70)
#define CHAT_MACRO_INPUT_2_X		(m_nMacroX + 25)
#define CHAT_MACRO_INPUT_2_Y		(m_nMacroY + 101)
#define CHAT_MACRO_INPUT_3_X		(m_nMacroX + 25)
#define CHAT_MACRO_INPUT_3_Y		(m_nMacroY + 130)
#define CHAT_MACRO_INPUT_W			(194)
#define CHAT_MACRO_INPUT_H			(20)

#define CHAT_MACRO_APPL_1_X			(m_nMacroX + 224)
#define CHAT_MACRO_APPL_1_Y			(m_nMacroY + 68)
#define CHAT_MACRO_APPL_2_X			(m_nMacroX + 224)
#define CHAT_MACRO_APPL_2_Y			(m_nMacroY + 97)
#define CHAT_MACRO_APPL_3_X			(m_nMacroX + 224)
#define CHAT_MACRO_APPL_3_Y			(m_nMacroY + 126)
#define CHAT_MACRO_APPL_GAP_Y		29

// #ifdef GAMEFORGE4D_CHAT_MACRO_OUTPUT_TIME
#define CHAT_MACRO_OUTPUT_TIME		300.0f // 2013-07-30 by ssjung 게임 포지 일 경우 매크로 딜레이 15초 에서 5분으로 수정


#define CHAT_MACRO_OK_X				(m_nMacroX + 229)
#define CHAT_MACRO_OK_Y				(m_nMacroY + 192)
#define CHAT_MACRO_CANCEL_X			(m_nMacroX + 270)
#define CHAT_MACRO_CANCEL_Y			(m_nMacroY + 192)

#define VOICECHAT_BAR_W				305
#define VOICECHAT_BAR_H				43

#define VOICECHAT_PARTY_POS_X		(205 + m_nVCCPosX)
#define VOICECHAT_PARTY_POS_Y		(46 + m_nVCCPosY)
#define VOICECHAT_GUILD_POS_X		(205 + m_nVCCPosX)
#define VOICECHAT_GUILD_POS_Y		(68 + m_nVCCPosY)
#define VOICECHAT_CHATROOM_POS_X	(205 + m_nVCCPosX)
#define VOICECHAT_CHATROOM_POS_Y	(90 + m_nVCCPosY)
#define VOICECHAT_WHISPER_POS_X		(205 + m_nVCCPosX)
#define VOICECHAT_WHISPER_POS_Y		(112 + m_nVCCPosY)

#define VOICECHAT_INPUT_KEY_POS_X   (41 + m_nVCCPosX)
#define VOICECHAT_INPUT_KEY_POS_Y   (150 + m_nVCCPosY)
#define VOICECHAT_INPUT_FREE_POS_X  (41 + m_nVCCPosX)
#define VOICECHAT_INPUT_FREE_POS_Y  (168 + m_nVCCPosY)

#define VOICECHAT_SPK_POS_X			(m_nVCCPosX + 50)
#define VOICECHAT_SPK_POS_Y			(m_nVCCPosY + 188)
#define VOICECHAT_SPK_W				65
#define VOICECHAT_SPK_CONTROL_TIME_GAP		0.25f

#define VOICECHAT_INV_BTN_POS_X		(m_nVCCPosX + 168)
#define VOICECHAT_INV_BTN_POS_Y		(m_nVCCPosY + 216)
#define VOICECHAT_OK_BTN_POS_X		(m_nVCCPosX + 209)
#define VOICECHAT_OK_BTN_POS_Y		(m_nVCCPosY + 216)
#define VOICECHAT_CAN_BTN_POS_X		(m_nVCCPosX + 250)
#define VOICECHAT_CAN_BTN_POS_Y		(m_nVCCPosY + 216)

// 2008-08-18 by bhsohn 매크로 추가 수정
#define CHATMACRO_TRANSCHAT_COUNT	11

#define STRING_CULL ::StringCullingUserData_ToBlank
#define STRING_CULL2 ::StringCullingUserData_ToBlank
#define CHAT_STRING_LENGTH			(m_nChatBoxWidth - 11)
#define FONT_INPUT_Y				CHAT_WINDOW_Y + 2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGameMainChat::CINFGameMainChat(CAtumNode* pParent, int nWidth, int nHeight)
{
	FLOG("CINFGameMainChat(CAtumNode* pParent)");

	m_pParent = pParent;

	int i = 0, j = 0;
	m_pGameData = NULL;

	for (i = 0; i < 9; i++)
	{
		m_pChatBox[i] = NULL;
	}
	for (i = 0; i < 3; i++)
	{
		m_pScrollLine[i] = NULL;
	}
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < BUTTON_BOTH_STATE_NUMBER; j++)
		{
			m_pChatViewButton[i][j] = NULL;
			m_pImgWisperBtn[i][j] = NULL;
		}
	}

	m_pScrollButton = NULL;
	m_pChatShowImage = NULL;
	//	m_nSizeMaxButtonstate = CHAT_BUTTON_NORMAL;
	//	m_nSizeMinButtonstate = CHAT_BUTTON_NORMAL;
	m_nChatSizeBtnNormal = BUTTON_BOTH_STATE_NORMAL;

	// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
// 	m_nChatBoxWidth		  = CHAT_CHATBOX_START_DEFAULT_MIN_X+100;
// 	m_nChatBoxHeight	  = CHAT_CHATBOX_START_DEFAULT_MIN_Y;

	if (nWidth <= 0 || nHeight <= 0)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_nChatBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X + 38;
#else							   
		m_nChatBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X + 100;

#endif
		m_nChatBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
	}
	else
	{
		m_nChatBoxWidth = nWidth;
		m_nChatBoxHeight = nHeight;
	}
	// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장


	m_nChatBoxYPos = 0;

//	m_bLockSoundButton = FALSE;
	m_bShowChatBox = FALSE;
	m_bWindowSizeMax = FALSE;
	m_bWindowSizeMin = FALSE;
	m_bLockHandle = FALSE;
	m_bScrollLock = FALSE;

	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_nTransChatCount = CHATMACRO_TRANSCHAT_COUNT;

	m_nCurrentTabIndex = CHAT_TAB_NORMAL;
	memset(&m_ArrChatTab, 0x00, sizeof(ChatTab_t));
	memset(&m_ArrChatTabMode, 0x00, sizeof(ChatTab_t)*CHAT_TAB_SAVE_COUNT);
	m_nCurrentRelScrollPosY = m_nChatBoxHeight - CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT;
	m_nCurrentRelScrollOldPosY = m_nCurrentRelScrollPosY;

	// by cmkwon
	//	memset(m_nCurrentScrollHeight,0x00, CHAT_TAB_NUMBER*sizeof(int));	
	memset(m_strInputMessage, 0x00, sizeof(m_strInputMessage));

	// 2010. 04. 01 by ckPark 채팅창 메모리 초기화 버그 수정
	//memset(m_fTimeOfShowChat,0x00, CHAT_TAB_NUMBER*CHAT_NOT_SHOWBOX_LINE*sizeof(float));
	memset(m_fTimeOfShowChat, 0x00, CHAT_NOT_SHOWBOX_LINE * sizeof(float));
	// end 2010. 04. 01 by ckPark 채팅창 메모리 초기화 버그 수정


//	m_nShowTimeChatType	=	0;

	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	InitChatMsgBuff();
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

	m_pFontInput = NULL;
	m_bChatMode = FALSE;
	m_vectorINFiGameMainWisperChatPtr.clear();
	m_pSystemMsgW = NULL;
	m_nClickedWisperChatBoxVectorIndex = -1;
	m_nSmallWindowCount = 0;

	m_nWisperBoxMinimumStatusInitPositionX = 0;
	m_nWisperBoxMinimumStatusInitPositionY = 0;

	memset(m_strMyMent, 0x00, 1024);

	// 채팅 변경 2005.01.13 ydkim
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
		m_bChatCurrentSelect[i] = TRUE;
	//	m_bChatCurrentSelect[CHAT_TAB_NORMAL]	= TRUE;
	//	m_bChatCurrentSelect[CHAT_TAB_SYSTEM]	= TRUE;
	//	m_bChatCurrentSelect[CHAT_TAB_TRADE]	= FALSE;

	m_nSelectChatTab = CHAT_TAB_NORMAL;
	m_pFontGM = NULL;
	memset(m_strInputGm, 0x00, 1024);
	memset(m_strInputInfluenceLeader, 0x00, 512);
	m_vecInputGM.clear();
	m_fTimeGm = 0.0f;
	m_fTimeInfluenceLeader = 0.0f;

	// 추가수정 2005.01.21
	for (i = 0; i < CHAT_BUTTON_NUMBER3; i++)
	{
		m_pNChatFAQ[i] = NULL;								// 채팅 FAQ버튼
		m_pNChatMenuView[i] = NULL;							// 채팅 메뉴 보이기 버튼
		m_pNChatVOIPStartMenu[i] = NULL;					// 채팅 VOIP 시작 버튼
		m_pNChatVOIPEndMenu[i] = NULL;						// 채팅 VOIP 종려 버튼
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		m_pNChatCash[i] = NULL;								// 채팅 채쉬충전 버튼
	}
#endif
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		m_pNChatCashView[i] = NULL;
	}
	for (i = 0; i < INPUT_LANGUAGE_END; i++)
	{
		m_pNChatInputLanguage[i] = NULL;					// 채팅 입력 언어
	}
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		m_pNChatMenu[i] = NULL;								// 채팅 메뉴 리스트
	}
	m_pNChatInput = NULL;									// 채팅 입력창
	m_pNChatCheck = NULL;									// 채팅 채크마크	
	m_nMenuListButtonState = CHAT_NMENU_BUTTON_STATE_NOM;	// 메뉴리스트 버튼 상태

	for (i = 0; i < CHAT_OTHER_MENU_ALL; i++)
	{
		m_nOtherMenuList[i] = CHAT_NMENU_BUTTON_STATE_NOM;
	}
	m_bShowMacro = FALSE;
	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		m_pBtnChatMacro[i] = NULL;
	}
	m_bGuildQuick = FALSE;
	m_bPartyQuick = FALSE;
	m_pBtnGuildQuick = NULL;
	m_pBtnPartyQuick = NULL;
	m_pChatMacroBG = NULL;
	m_pBtnMacroOK = NULL;
	m_pBtnMacroCancel = NULL;
	m_pBtnOption = NULL;

	m_bShowMiniWisper = TRUE;
	m_bShowMiniWisperUnder = FALSE;

	m_nActMacro = -1;
	m_nMacroX = (g_pD3dApp->GetBackBufferDesc().Width - 305) / 2;
	m_nMacroY = 300;
	m_bMacroMove = FALSE;
	m_nSendMacroNum = 0;

	m_bMenuListUse = FALSE;
	m_bMenuList = FALSE;
	m_bCheckShopInfo = FALSE;
	m_bCashShopOpen = FALSE;
	m_nOtherCashList = CHAT_NMENU_BUTTON_STATE_NOM;
	m_bCashButton = FALSE;
	m_nCashButtonGab = 0;
	m_bPKOptionFlag = TRUE;
	m_bTradeFlag = TRUE;
	m_bWindowMessageFlag = TRUE;
	m_bHelpDeskRender = FALSE;
	m_bSelectInputLanguage = FALSE;

	m_nVoiceMenu = 0;

	// by bhsohn 2007-03-12 China Working
	m_nChinaCnt = 0;
	m_nMSPYMode = INPUT_LANGUAGE_CHINA;
	m_nChatMode = CHAT_TAB_ALLUSER;

	//m_bShowVoiceChatControl = FALSE;
	//m_nVCCPosX = (g_pD3dApp->GetBackBufferDesc().Width - 305) / 2;
	//m_nVCCPosY = 320;
	////m_nVolum = 9999;
	//m_pVoiceChatBG = NULL;
	//m_pBothParty = NULL;
	//m_pBothGuild = NULL;
	//m_pBothChat = NULL;
	//m_pImgSpkVolBar = NULL;
	//m_pImgRadioBtn[0] = NULL;
	//m_pImgRadioBtn[1] = NULL;
	//m_pBothWhisper = NULL;
	//m_pBtnVoiceChatInv = NULL;
	//m_pBtnVoiceChatOk = NULL;
	//m_pBtnVoiceChatCan = NULL;
	//m_bVCCMoving = FALSE;

	// 2008-08-18 by bhsohn 매크로 추가 수정
	m_bMacroIng = FALSE;
	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		memset(&m_sMacroStr[i], 0x00, sizeof(sCHATMACROSTRING));
	}
	memset(m_strTempMacro, 0x00, SIZE_MAX_CHAT_MESSAGE);
	// end 2008-08-18 by bhsohn 매크로 추가 수정



	// 2008. 12. 29 by ckPark 채팅창 최적화	

	// 각 채팅 라인 초기화
	for (i = 0; i < MAX_GAME_MAIN_CHAT_LINE; ++i)
		m_pFontLine[i] = NULL;

	// end 2008. 12. 29 by ckPark 채팅창 최적화	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBtnChatBG = NULL;
#endif

	// 2012-10-31 by jhjang 포인터 초기화 코드 추가
	for (i = 0; i < 9; i++)
	{
		m_pBtnChatMode[i] = NULL;
	}
	// end 2012-10-31 by jhjang 포인터 초기화 코드 추가
}

CINFGameMainChat::~CINFGameMainChat()
{
	FLOG("~CINFGameMainChat()");

	int i = 0, j = 0;

	//	for(i=0;i<5;i++)
	//	{
	//		for(j=0;j<2;j++)
	//		{
	//			util::del(m_pTabButton[i][j]);
	//		}
	//	}
	for (i = 0; i < 9; i++)
	{
		util::del(m_pChatBox[i]);
	}
	for (i = 0; i < 3; i++)
	{
		util::del(m_pScrollLine[i]);
	}
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < BUTTON_BOTH_STATE_NUMBER; j++)
		{
			util::del(m_pChatViewButton[i][j]);
			util::del(m_pImgWisperBtn[i][j]);
		}
	}

	util::del(m_pScrollButton);
	util::del(m_pFontInput);
	util::del(m_pChatShowImage);

	vectorINFGameMainWisperChatPtr::iterator itt(m_vectorINFiGameMainWisperChatPtr.begin());
	while (itt != m_vectorINFiGameMainWisperChatPtr.end())
	{
		util::del(*itt);
		itt++;
	}
	m_vectorINFiGameMainWisperChatPtr.clear();

	util::del(m_pSystemMsgW);

	vector<CD3DHanFont*>::iterator it = m_vecFontLine.begin();
	while (it != m_vecFontLine.end())
	{
		CD3DHanFont* pFont = *it;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
		it++;
	}
	m_vecFontLine.clear();
	util::del(m_pFontGM);

	// 추가수정 2005.01.21
	for (i = 0; i < CHAT_BUTTON_NUMBER3; i++)
	{
		util::del(m_pNChatFAQ[i]);								// 채팅 FAQ버튼
		util::del(m_pNChatMenuView[i]);							// 채팅 메뉴 보이기 버튼
		util::del(m_pNChatVOIPStartMenu[i]);						// 채팅 VOIP 시작 버튼
		util::del(m_pNChatVOIPEndMenu[i]);						// 채팅 VOIP 종료 버튼
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		util::del(m_pNChatCash[i]);								// 채팅 채쉬충전 버튼
	}
#endif
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		util::del(m_pNChatCashView[i]);
	}
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		util::del(m_pNChatCashView[i]);
	}
	for (i = 0; i < INPUT_LANGUAGE_END; i++)
	{
		util::del(m_pNChatInputLanguage[i]);						// 채팅 입력 언어
	}
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		util::del(m_pNChatMenu[i]);								// 채팅 메뉴 리스트
	}
	util::del(m_pNChatInput);									// 채팅 입력창
	util::del(m_pNChatCheck);									// 채팅 채크마크

	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		util::del(m_pBtnChatMacro[i]);
	}
	util::del(m_pChatMacroBG);
	util::del(m_pBtnMacroOK);
	util::del(m_pBtnMacroCancel);
	util::del(m_pBtnOption);

	// 2008. 12. 29 by ckPark 채팅창 최적화	

	// 각 채팅 라인 삭제
	for (i = 0; i < MAX_GAME_MAIN_CHAT_LINE; ++i)
		util::del(m_pFontLine[i]);

	// end 2008. 12. 29 by ckPark 채팅창 최적화	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pBtnChatBG);
#endif
}

void CINFGameMainChat::InitChatData()
{
	vectorINFGameMainWisperChatPtr::iterator itt(m_vectorINFiGameMainWisperChatPtr.begin());
	while (itt != m_vectorINFiGameMainWisperChatPtr.end())
	{
		util::del(*itt);
		itt++;
	}
	m_vectorINFiGameMainWisperChatPtr.clear();

	m_nCurrentTabIndex = CHAT_TAB_NORMAL;
	memset(&m_ArrChatTab, 0x00, sizeof(ChatTab_t));
	memset(&m_ArrChatTabMode, 0x00, sizeof(ChatTab_t)*CHAT_TAB_SAVE_COUNT);
	memset(m_fTimeOfShowChat, 0x00, CHAT_NOT_SHOWBOX_LINE * sizeof(float));

}

void CINFGameMainChat::AddBuffer(
	int nType, const char* strBuffer, DWORD dwColorType,
	const char* szWisperToCharacterName,
	const char* szWisperFromCharacterName,
	BYTE NameColor)
{
	FLOG("CINFGameMainChat::AddBuffer(int nType, char* strBuffer, DWORD dwColorType)");

	/////////////////////////////////////////////////////////////////////////////////////
	// 채팅 시스템 변경 05.01.21
	// 기본 타입은 (시스템 4) 으로 한다
	// 지역 채팅과 귓속 말은 0 일반 채티으로 한다
	// 그 외 기타 길드, 파티, 매매 채팅등 
	int i = 0;
	if (FALSE == this->IsValidTabIndex(nType)) { return; }

	// 귓말 채팅
	if (CHAT_TAB_NORMAL == nType || CHAT_TAB_CASH == nType)
	{
		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;
		if (m_bChatCurrentSelect[CHAT_TAB_NORMAL] == TRUE)
			m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
		m_ArrChatTabMode[CHAT_TAB_NORMAL].PushChatLine(strBuffer, dwColorType, nType);
	}
	else if (CHAT_TAB_PTOP == nType)
	{
		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;

		ProcessWisperChatAddLine(strBuffer, dwColorType, szWisperToCharacterName, szWisperFromCharacterName, NameColor);
		m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
	}
	else if (CHAT_TAB_SOUND == nType)
	{
		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;

		if (m_bChatCurrentSelect[CHAT_TAB_SYSTEM] == TRUE)
			m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
		m_ArrChatTabMode[CHAT_TAB_SYSTEM].PushChatLine(strBuffer, dwColorType, nType);
	}
	else if (CHAT_TAB_SYSTEM == nType)
	{
		if (!m_pSystemMsgW)
			return;
		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_pSystemMsgW->m_fTimeOfShowChat[i] = m_pSystemMsgW->m_fTimeOfShowChat[i - 1];
		}
		m_pSystemMsgW->m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;

		m_pSystemMsgW->AddBuffer(strBuffer, dwColorType);

	}
	// 2008-08-14 by dgwoo 거래채팅 매크로 기호기 변경.
	else if (nType == CHAT_TAB_TRADE)
	{
		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;
		if (m_bChatCurrentSelect[nType] == TRUE)
			m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
		m_ArrChatTabMode[nType].PushChatLine(strBuffer, dwColorType, nType);
		// 2008-08-18 by bhsohn 매크로 추가 수정
		if (m_bMacroIng)
		{
			if (m_nTransChatCount < CHATMACRO_TRANSCHAT_COUNT)
			{
				// 매크로 사용시 에만 변수 결정
				m_nTransChatCount++;
			}
		}
	}
	else if (nType != CHAT_TAB_HELPDESK)
	{
		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;
		if (m_bChatCurrentSelect[nType] == TRUE)
			m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
		m_ArrChatTabMode[nType].PushChatLine(strBuffer, dwColorType, nType);
	}

	// GM용
	if (nType == CHAT_TAB_GAMEMASTER)
	{
		m_bHelpDeskRender = FALSE;
		memset(m_strInputGm, 0x00, 1024);
		strncpy(m_strInputGm, strBuffer, CHAT_STRING_SIZE_GM);
		m_vecInputGM.clear();
		// 시간 초기화
		m_fTimeGm = CHAT_TIME_OF_NOT_SHOW_BOX_GM;

		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;
		m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
		for (int i = 0; i < CHAT_TAB_SAVE_COUNT; i++)
		{
			if ((i == CHAT_TAB_PARTY && g_pShuttleChild->m_pClientParty->IsParty() == TRUE) ||
				(i == CHAT_TAB_GUILD && g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber != 0))

				m_ArrChatTabMode[i].PushChatLine(strBuffer, dwColorType, nType);
		}
	}
	else if (m_bChatCurrentSelect[CHAT_TAB_HELPDESK] == TRUE && nType == CHAT_TAB_HELPDESK)
	{
		m_bHelpDeskRender = TRUE;
		memset(m_strInputGm, 0x00, 1024);
		//		strncpy(m_strInputGm, strBuffer, CHAT_STRING_SIZE_GM);
				// 시간 초기화
		m_fTimeGm = CHAT_TIME_OF_NOT_SHOW_BOX_GM;

		strncpy(m_strInputGm, strBuffer, 1024);
		m_vecInputGM.clear();
	}
	// 2006-04-21 by ispark, 지도자용
	else if (nType == CHAT_TAB_INFLUENCE_LEADER)
	{
		memset(m_strInputInfluenceLeader, 0x00, 512);
		strncpy(m_strInputInfluenceLeader, strBuffer, CHAT_STRING_SIZE_GM);
		// 시간 초기화
		m_fTimeInfluenceLeader = CHAT_TIME_OF_NOT_SHOW_BOX_GM;

		for (i = CHAT_NOT_SHOWBOX_LINE - 1; i > 0; i--)
		{
			m_fTimeOfShowChat[i] = m_fTimeOfShowChat[i - 1];
		}
		m_fTimeOfShowChat[0] = CHAT_TIME_OF_NOT_SHOW_BOX;
		m_ArrChatTab.PushChatLine(strBuffer, dwColorType, nType);
		m_ArrChatTabMode[CHAT_TAB_WAR].PushChatLine(strBuffer, dwColorType, nType);
	}
	//
	//////////////////////////////////////////////////////////////////////////////////

	if (FALSE == m_bScrollLock)
	{
		ChatTab_t *pChatTab = GetChatTabMode();
		m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP +
			pChatTab->GetScrollPositionWithRenderStartIndex(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
				, (m_nChatBoxHeight - CHATBOX_IMAGE_GAB_HEITHT_TOP) / CHAT_FONT_LINE_HEIGHT);
	}



	// 2008. 12. 29 by ckPark 채팅창 최적화	
	// 문자 추가시 컬링된 스트링 초기화
	m_listCulledString.clear();
	// end 2008. 12. 29 by ckPark 채팅창 최적화	
}

void CINFGameMainChat::ResetHanFontLine()
{
	FLOG("CINFGameMainChat::ResetHanFontLine()");
	vector<CD3DHanFont*>::iterator it = m_vecFontLine.begin();
	while (it != m_vecFontLine.end())
	{
		CD3DHanFont* pFont = *it;
		it++;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
	}
	m_vecFontLine.clear();

	int i;
	for (i = 0; i < (CHAT_CHATBOX_START_DEFAULT_MAX_Y / CHAT_FONT_LINE_HEIGHT); i++)
	{
		CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE, 1024, 32, FALSE, TRUE);	//	폰트 설정 
		pFont->InitDeviceObjects(g_pD3dDev);
		pFont->RestoreDeviceObjects();

		//m_nChatBoxHeight - 18*2 - 30

		if (m_bShowChatBox) {
			pFont->SetUV(0.0f, 0.0f, (float)(m_nChatBoxWidth - CHATBOX_IMAGE_GAB_WIDTH_BOTTOM) / 1024.0f, 1.0f);
		}
		else {
			pFont->SetUV(0.0f, 0.0f, 1.0f, 1.0f);
		}
		m_vecFontLine.push_back(pFont);
	}
	float fTemp = 1.0f;
	for (i = 0; i < CHAT_NOT_SHOWBOX_LINE; i++)
	{
		m_fTimeOfShowChat[i] = CHAT_TIME_OF_NOT_SHOW_BOX - fTemp*i;
	}
	for (i = 0; i < CHAT_TAB_CHATROOM; i++)
	{
		if (NULL != m_pBtnChatMode[i])
		{
			m_pBtnChatMode[i]->RestoreDeviceObjects();
		}
	}
	{
		if (NULL != m_pBtnGuildQuick)
		{
			m_pBtnGuildQuick->RestoreDeviceObjects();
		}
		if (NULL != m_pBtnPartyQuick)
		{
			m_pBtnPartyQuick->RestoreDeviceObjects();
		}
	}
}


HRESULT CINFGameMainChat::InitDeviceObjects()
{
	FLOG("CINFGameMainChat::InitDeviceObjects()");
	DataHeader	* pDataHeader;
	char buf[16];

	//	for(int i=0;i<5;i++)
	//	{
	//		for(int j=0;j<2;j++)
	//		{
	//			wsprintf(buf, "c_btn%d%d", i+1,j); 
	//			m_pTabButton[i][j] = new CINFImage;
	//			pDataHeader = FindResource(buf);
	//			m_pTabButton[i][j]->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
	//		}
	//	}
	// 2011. 10. 10 by jskim UI시스템 변경
	int i;
	for (i = 0; i < BUTTON_BOTH_STATE_NUMBER; i++)
	{
		m_pChatViewButton[0][i] = new CINFImageEx;
		wsprintf(buf, "c_hsbtn0%d", i);
		pDataHeader = FindResource(buf);
		m_pChatViewButton[0][i]->InitDeviceObjects(pDataHeader);

		m_pChatViewButton[1][i] = new CINFImageEx;
		wsprintf(buf, "c_hsbtn1%d", i);
		pDataHeader = FindResource(buf);
		m_pChatViewButton[1][i]->InitDeviceObjects(pDataHeader);

		m_pImgWisperBtn[0][i] = new CINFImageEx;
		wsprintf(buf, "wispbar0%d", i);
		pDataHeader = FindResource(buf);
		m_pImgWisperBtn[0][i]->InitDeviceObjects(pDataHeader);

		m_pImgWisperBtn[1][i] = new CINFImageEx;
		wsprintf(buf, "wispbar1%d", i);
		pDataHeader = FindResource(buf);
		m_pImgWisperBtn[1][i]->InitDeviceObjects(pDataHeader);
	}

	for (i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_pChatBox[i * 3 + j] = new CINFImageEx;
			wsprintf(buf, "c_c%d%d", i, j);
			pDataHeader = FindResource(buf);
			m_pChatBox[i * 3 + j]->InitDeviceObjects(pDataHeader);
		}
	}
	for (i = 0; i < 3; i++)
	{
		m_pScrollLine[i] = new CINFImageEx;
		wsprintf(buf, "c_s%d", i);
		pDataHeader = FindResource(buf);
		m_pScrollLine[i]->InitDeviceObjects(pDataHeader);

	}
	m_pScrollButton = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pScrollButton->InitDeviceObjects(pDataHeader);

	m_pChatShowImage = new CINFImageEx;
	pDataHeader = FindResource("Chatmage");
	m_pChatShowImage->InitDeviceObjects(pDataHeader);

	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE, 1024, 32, TRUE);
	m_pFontInput->InitDeviceObjects(g_pD3dDev);

	m_pFontGM = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE, 1024, 32);
	m_pFontGM->InitDeviceObjects(g_pD3dDev);

	// 추가수정 2005.01.21
	for (i = 0; i < CHAT_BUTTON_NUMBER3; i++)
	{
		m_pNChatFAQ[i] = new CINFImageEx;
		wsprintf(buf, "c_nfa%d", i);						// 채팅 FAQ버튼
		pDataHeader = FindResource(buf);
		m_pNChatFAQ[i]->InitDeviceObjects(pDataHeader);

		m_pNChatMenuView[i] = new CINFImageEx;
		wsprintf(buf, "c_nme%d", i);						// 채팅 메뉴 보이기 버튼
		pDataHeader = FindResource(buf);
		m_pNChatMenuView[i]->InitDeviceObjects(pDataHeader);

		m_pNChatVOIPStartMenu[i] = new CINFImageEx;
		wsprintf(buf, "c_von%d", i);						// 채팅 VOIP 시작 메뉴 보이기 버튼
		pDataHeader = FindResource(buf);
		m_pNChatVOIPStartMenu[i]->InitDeviceObjects(pDataHeader);

		m_pNChatVOIPEndMenu[i] = new CINFImageEx;
		wsprintf(buf, "c_vof%d", i);						// 채팅  VOIP 종료 메뉴 보이기 버튼
		pDataHeader = FindResource(buf);
		m_pNChatVOIPEndMenu[i]->InitDeviceObjects(pDataHeader);
	}
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pNChatCash[i] = new CINFImageEx;
		wsprintf(buf, "c_nca%d", i);						// 채팅 채쉬충전 버튼
		pDataHeader = FindResource(buf);
		m_pNChatCash[i]->InitDeviceObjects(pDataHeader);
#endif

		// 2006-06-08 by ispark, 유료 상점 버튼 변경
		m_pNChatCashView[i] = new CINFImageEx;
		//		wsprintf(buf, "cc_sh%d", i);						// 채팅 채쉬충전 버튼
		wsprintf(buf, "shbtn100%d", i);
		pDataHeader = FindResource(buf);
		m_pNChatCashView[i]->InitDeviceObjects(pDataHeader);
	}

	for (i = 0; i < INPUT_LANGUAGE_END; i++)
	{
		m_pNChatInputLanguage[i] = new CINFImageEx;
		wsprintf(buf, "lang_%d", i + 1);
		pDataHeader = FindResource(buf);
		m_pNChatInputLanguage[i]->InitDeviceObjects(pDataHeader);
	}
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		m_pNChatMenu[i] = new CINFImageEx;
		wsprintf(buf, "chat_li%d", i);						// 채팅 메뉴 리스트
		pDataHeader = FindResource(buf);
		m_pNChatMenu[i]->InitDeviceObjects(pDataHeader);
	}

	m_pNChatInput = new CINFImageEx;
	pDataHeader = FindResource("c_nbar");						// 채팅 입력창
	m_pNChatInput->InitDeviceObjects(pDataHeader);

	m_pNChatCheck = new CINFImageEx;
	pDataHeader = FindResource("c_nchk");						// 채팅 채크마크
	m_pNChatCheck->InitDeviceObjects(pDataHeader);

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	//for(i=0; i<CHAT_TAB_NUMBER; i++)
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		m_bChatCurrentSelect[i] = g_pSOption->m_bChatCurrentSelect[i];
	}
	m_bChatCurrentSelect[CHAT_TAB_SPEAKER] = g_pInterface->GetSpeakerMode();
	// end 2007-08-07 by bhsohn 스피커 아이템 추가

	// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
	{
		char szButtonName[32];
		for (i = 0; i < CHAT_TAB_CHATROOM; i++)
		{
			wsprintf(szButtonName, "cicon_li%d", i + 1);
			if (NULL == m_pBtnChatMode[i])
			{
				m_pBtnChatMode[i] = new CINFImageBtn;
				m_pBtnChatMode[i]->EnableBtn(TRUE);
			}
			m_pBtnChatMode[i]->InitDeviceObjects(szButtonName);
		}
		wsprintf(szButtonName, "Pquick");
		if (NULL == m_pBtnPartyQuick)
		{
			m_pBtnPartyQuick = new CINFImageBtn;
		}
		m_pBtnPartyQuick->InitDeviceObjects(szButtonName);
		wsprintf(szButtonName, "Gquick");
		if (NULL == m_pBtnGuildQuick)
		{
			m_pBtnGuildQuick = new CINFImageBtn;
		}
		m_pBtnGuildQuick->InitDeviceObjects(szButtonName);

		// Setting
		m_pBtnChatMode[0]->PushButton(TRUE);
		// 아레나, 채팅방은 기본 비활성화.
//		m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_GUILD)]->EnableBtn(FALSE);
//		m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_PARTY)]->EnableBtn(FALSE);
		m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_ARENA)]->EnableBtn(FALSE);
		m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_CHATROOM)]->EnableBtn(FALSE);

		// 2008-11-24 by dgwoo 아레나 채팅 탭 활성화 안되는 문제
		CFieldWinSocket* pArenaFieldSocket = g_pD3dApp->m_pFieldWinSocket->GetArenaFieldWinSocket();
		if (pArenaFieldSocket)
		{
			// 아레나 소켓에 열결되어있으면 
			m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_ARENA)]->EnableBtn(TRUE);
		}
		// end 2008-11-24 by dgwoo 아레나 채팅 탭 활성화 안되는 문제

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("macro");
		m_pChatMacroBG = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
		m_pChatMacroBG->InitDeviceObjects(g_pD3dApp->m_pImageList);

		if (NULL == m_pBtnMacroOK)
		{
			m_pBtnMacroOK = new CINFImageBtn;
		}
		m_pBtnMacroOK->InitDeviceObjects("oks0");
		if (NULL == m_pBtnMacroCancel)
		{
			m_pBtnMacroCancel = new CINFImageBtn;
		}
		m_pBtnMacroCancel->InitDeviceObjects("cans0");
#else
		if (NULL == m_pChatMacroBG)
		{
			m_pChatMacroBG = new CINFImageEx;
		}
		pDataHeader = FindResource("chatmacro");
		m_pChatMacroBG->InitDeviceObjects(pDataHeader);

		if (NULL == m_pBtnMacroOK)
		{
			m_pBtnMacroOK = new CINFImageBtn;
		}
		m_pBtnMacroOK->InitDeviceObjects("shlaok0");
		if (NULL == m_pBtnMacroCancel)
		{
			m_pBtnMacroCancel = new CINFImageBtn;
		}
		m_pBtnMacroCancel->InitDeviceObjects("shmcan0");
#endif		
		for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
		{
			wsprintf(szButtonName, "macroappl");
			if (NULL == m_pBtnChatMacro[i])
			{
				m_pBtnChatMacro[i] = new CINFImageBtnBoth;
			}
			m_pBtnChatMacro[i]->InitDeviceObjects(szButtonName);
		}

		if (NULL == m_pBtnOption)
		{
			m_pBtnOption = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pBtnOption->InitDeviceObjects("copticon");
		m_pBtnOption->InitDeviceObjects("copticon", "STRTOOLTIP99");
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	// 음성 옵션.
//	{
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//		if (NULL == m_pBtnVoiceChatInv)
//		{
//			m_pBtnVoiceChatInv = new CINFImageBtn;
//		}
//		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//		//m_pBtnVoiceChatInv->InitDeviceObjects("invs0");
//		m_pBtnVoiceChatInv->InitDeviceObjects("invs0", "STRTOOLTIP27");
//		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//		if (NULL == m_pBtnVoiceChatOk)
//		{
//			m_pBtnVoiceChatOk = new CINFImageBtn;
//		}
//		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//		//m_pBtnVoiceChatOk->InitDeviceObjects("oks0");
//		m_pBtnVoiceChatOk->InitDeviceObjects("oks0", "STRTOOLTIP40");
//		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//		if (NULL == m_pBtnVoiceChatCan)
//		{
//			m_pBtnVoiceChatCan = new CINFImageBtn;
//		}
//		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//		//m_pBtnVoiceChatCan->InitDeviceObjects("cans0");
//		m_pBtnVoiceChatCan->InitDeviceObjects("cans0", "STRTOOLTIP41");
//		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//
//		if (NULL == m_pBothParty)
//		{
//			m_pBothParty = new CINFImageBtnBoth;
//		}
//#else
//		if (NULL == m_pBtnVoiceChatInv)
//		{
//			m_pBtnVoiceChatInv = new CINFImageBtn;
//		}
//		m_pBtnVoiceChatInv->InitDeviceObjects("voiceinv");
//		if (NULL == m_pBtnVoiceChatOk)
//		{
//			m_pBtnVoiceChatOk = new CINFImageBtn;
//		}
//		m_pBtnVoiceChatOk->InitDeviceObjects("shlaok0");
//		if (NULL == m_pBtnVoiceChatCan)
//		{
//			m_pBtnVoiceChatCan = new CINFImageBtn;
//		}
//		m_pBtnVoiceChatCan->InitDeviceObjects("shmcan0");
//
//		if (NULL == m_pBothParty)
//		{
//			m_pBothParty = new CINFImageBtnBoth;
//		}
//#endif
//		m_pBothParty->InitDeviceObjects("onoff");
//		m_pBothParty->ChangeBoth(BUTTON_BOTH_BACK);
//		if (NULL == m_pBothGuild)
//		{
//			m_pBothGuild = new CINFImageBtnBoth;
//		}
//		m_pBothGuild->InitDeviceObjects("onoff");
//		m_pBothGuild->ChangeBoth(BUTTON_BOTH_BACK);
//		if (NULL == m_pBothChat)
//		{
//			m_pBothChat = new CINFImageBtnBoth;
//		}
//		m_pBothChat->InitDeviceObjects("onoff");
//		m_pBothChat->ChangeBoth(BUTTON_BOTH_BACK);
//		if (NULL == m_pBothWhisper)
//		{
//			m_pBothWhisper = new CINFImageBtnBoth;
//		}
//		m_pBothWhisper->InitDeviceObjects("onoff");
//		m_pBothWhisper->ChangeBoth(BUTTON_BOTH_BACK);
//
//		// 2011. 10. 10 by jskim UI시스템 변경
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("voiceChat");
//		m_pVoiceChatBG = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
//		m_pVoiceChatBG->InitDeviceObjects(g_pD3dApp->m_pImageList);
//#else
//		m_pVoiceChatBG = new CINFImageEx;
//		pDataHeader = FindResource("voptBG");
//		m_pVoiceChatBG->InitDeviceObjects(pDataHeader);
//#endif
//		m_pImgSpkVolBar = new CINFImageEx;
//		pDataHeader = FindResource("scrl_b");
//		m_pImgSpkVolBar->InitDeviceObjects(pDataHeader);
//		m_pImgRadioBtn[0] = new CINFImageEx;
//		pDataHeader = FindResource("radio_b");
//		m_pImgRadioBtn[0]->InitDeviceObjects(pDataHeader);
//		m_pImgRadioBtn[1] = new CINFImageEx;
//		pDataHeader = FindResource("radio_a");
//		m_pImgRadioBtn[1]->InitDeviceObjects(pDataHeader);
//	}
	if (m_pSystemMsgW == NULL)
	{
		// 2008-06-20 by bhsohn EP3 옵션관련 처리
		//m_pSystemMsgW = new CINFSystemMsgWindow(this);
		int nStartX, nStartY, nWidth, nHeight;
		nStartX = nStartY = nWidth = nHeight = -1;
		if (g_pD3dApp->IsOptionEtc(OPTION_RADIO_INTERFACE_POS))
		{
			float fWidth = (float)g_pD3dApp->GetBackBufferDesc().Width;
			float fHeight = (float)g_pD3dApp->GetBackBufferDesc().Height;

			stcuctRateRect stRateRect = g_pD3dApp->GetOptionInterfacePOS();
			if ((stRateRect.fGameWidth == fWidth)
				&& (stRateRect.fGameHeight == fHeight))
			{
				nStartX = stRateRect.fRateX;
				nStartY = stRateRect.fRateY;
				nWidth = stRateRect.fRateWidth;
				nHeight = stRateRect.fRateHeight;
			}
			else
			{
				float fRateWidth = (fWidth / stRateRect.fGameWidth);
				float fRateHeight = (fHeight / stRateRect.fGameHeight);
				if (fRateWidth == 0)
				{
					fRateWidth = 1;
				}
				if (fRateHeight == 0)
				{
					fRateHeight = 1;
				}
				nStartX = (int)(stRateRect.fRateX*fRateWidth);
				nStartY = (int)(stRateRect.fRateY*fRateHeight);
				nWidth = (int)(stRateRect.fRateWidth*fRateWidth);
				nHeight = (int)(stRateRect.fRateHeight*fRateHeight);
			}
		}

		m_pSystemMsgW = new CINFSystemMsgWindow(this, nStartX, nStartY, nWidth, nHeight);
		m_pSystemMsgW->SetInterface(m_bChatCurrentSelect[CHAT_TAB_SYSTEM]);
		// end 2008-06-20 by bhsohn EP3 옵션관련 처리

		m_pSystemMsgW->SetGameData(m_pGameData);
		m_pSystemMsgW->InitDeviceObjects();
	}




	// 2008. 12. 29 by ckPark 채팅창 최적화
	// 각 라인 컬링 스트링 생성
	for (i = 0; i < MAX_GAME_MAIN_CHAT_LINE; ++i)
	{
		m_pFontLine[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE, 1024, 32, FALSE, TRUE);
		m_pFontLine[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontLine[i]->RestoreDeviceObjects();

		if (m_bShowChatBox)
			m_pFontLine[i]->SetUV(0.0f, 0.0f, (float)(m_nChatBoxWidth - CHATBOX_IMAGE_GAB_WIDTH_BOTTOM) / 1024.0f, 1.0f);
		else
			m_pFontLine[i]->SetUV(0.0f, 0.0f, 1.0f, 1.0f);
	}

	// end 2008. 12. 29 by ckPark 채팅창 최적화	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("BGbox");
	m_pBtnChatBG = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
#endif
	return S_OK;
}

// 2007-08-07 by bhsohn 스피커 아이템 추가
//int CINFGameMainChat::GetRealPos(int nPos)
//{
//	int nPosTab = CHAT_TAB_NUMBER-1;
//	switch(nPos)
//	{
//	case CHAT_TAB_SPEAKER:
//		{
//			nPosTab = CHAT_TAB_NUMBER-1;
//		}
//		break;
//	}
//	return nPosTab;
//}

HRESULT CINFGameMainChat::RestoreDeviceObjects()
{
	FLOG("CINFGameMainChat::RestoreDeviceObjects()");
	//	for(int i=0;i<5;i++)
	//	{
	//		for(int j=0;j<2;j++)
	//		{
	//			m_pTabButton[i][j]->RestoreDeviceObjects();
	//		}
	//	}
	int i;
	for (i = 0; i < 9; i++)
	{
		m_pChatBox[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < 3; i++)
	{
		m_pScrollLine[i]->RestoreDeviceObjects();
	}
	m_pScrollButton->RestoreDeviceObjects();
	m_pChatShowImage->RestoreDeviceObjects();
	for (i = 0; i < BUTTON_BOTH_STATE_NUMBER; i++)
	{
		m_pChatViewButton[0][i]->RestoreDeviceObjects();
		m_pChatViewButton[1][i]->RestoreDeviceObjects();
		m_pImgWisperBtn[0][i]->RestoreDeviceObjects();
		m_pImgWisperBtn[1][i]->RestoreDeviceObjects();
	}

	m_pFontInput->RestoreDeviceObjects();
	ResetHanFontLine();

	vectorINFGameMainWisperChatPtr::iterator it(m_vectorINFiGameMainWisperChatPtr.begin());
	while (it != m_vectorINFiGameMainWisperChatPtr.end())
	{
		(*it)->RestoreDeviceObjects();
		it++;
	}
	m_pSystemMsgW->RestoreDeviceObjects();
	m_pFontGM->RestoreDeviceObjects();

	// 추가수정 2005.01.21
	for (i = 0; i < CHAT_BUTTON_NUMBER3; i++)
	{
		m_pNChatFAQ[i]->RestoreDeviceObjects();								// 채팅 FAQ버튼
		m_pNChatMenuView[i]->RestoreDeviceObjects();						// 채팅 메뉴 보이기 버튼
		m_pNChatVOIPStartMenu[i]->RestoreDeviceObjects();
		m_pNChatVOIPEndMenu[i]->RestoreDeviceObjects();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		m_pNChatCash[i]->RestoreDeviceObjects();							// 채팅 채쉬충전 버튼
	}
#endif
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		m_pNChatCashView[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < INPUT_LANGUAGE_END; i++)
	{
		m_pNChatInputLanguage[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		m_pNChatMenu[i]->RestoreDeviceObjects();							// 채팅 메뉴 리스트
	}
	m_pNChatInput->RestoreDeviceObjects();									// 채팅 입력창
	m_pNChatCheck->RestoreDeviceObjects();									// 채팅 채크마크

	for (i = 0; i < CHAT_TAB_CHATROOM; i++)
	{
		m_pBtnChatMode[i]->RestoreDeviceObjects();
		m_pBtnChatMode[i]->SetBtnPosition(CHAT_CHATMODE_X + (CHAT_CHATMODE_GAP * i), CHAT_CHATMODE_Y);
	}

	m_pBtnGuildQuick->RestoreDeviceObjects();
	m_pBtnPartyQuick->RestoreDeviceObjects();


	m_pChatMacroBG->RestoreDeviceObjects();

	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		m_pBtnChatMacro[i]->RestoreDeviceObjects();
	}
	m_pBtnMacroOK->RestoreDeviceObjects();
	m_pBtnMacroCancel->RestoreDeviceObjects();
	m_pBtnOption->RestoreDeviceObjects();
	m_pBtnOption->SetBtnPosition(CHAT_OPTION_BUTTON_X, CHAT_OPTION_BUTTON_Y);


	/*{
		m_pBtnVoiceChatInv->RestoreDeviceObjects();
		m_pBtnVoiceChatOk->RestoreDeviceObjects();
		m_pBtnVoiceChatCan->RestoreDeviceObjects();

		m_pBothParty->RestoreDeviceObjects();
		m_pBothGuild->RestoreDeviceObjects();
		m_pBothChat->RestoreDeviceObjects();
		m_pBothWhisper->RestoreDeviceObjects();

		m_pVoiceChatBG->RestoreDeviceObjects();
		m_pImgSpkVolBar->RestoreDeviceObjects();
		m_pImgRadioBtn[0]->RestoreDeviceObjects();
		m_pImgRadioBtn[1]->RestoreDeviceObjects();

	}*/

	// 2007-11-05 by bhsohn 몬스터 오브젝트 체크섬 처리
	// 객체가 생성되기 전에 모든 글씨를 넣는다.
	g_pD3dApp->m_pChat->ErrorInfoAllAddChat();



	// 2008. 12. 29 by ckPark 채팅창 최적화	
	// 각 채팅 라인 복구
	for (i = 0; i < MAX_GAME_MAIN_CHAT_LINE; ++i)
		m_pFontLine[i]->RestoreDeviceObjects();

	// end 2008. 12. 29 by ckPark 채팅창 최적화	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBtnChatBG->RestoreDeviceObjects();
#endif

	return S_OK;
}

HRESULT CINFGameMainChat::DeleteDeviceObjects()
{
	FLOG("CINFGameMainChat::DeleteDeviceObjects()");
	//	for(int i=0;i<5;i++)
	//	{
	//		for(int j=0;j<2;j++)
	//		{
	//			m_pTabButton[i][j]->DeleteDeviceObjects();
	//			util::del(m_pTabButton[i][j]);
	//		}
	//	}
	int i;
	for (i = 0; i < 9; i++)
	{
		m_pChatBox[i]->DeleteDeviceObjects();
		util::del(m_pChatBox[i]);
	}
	for (i = 0; i < 3; i++)
	{
		m_pScrollLine[i]->DeleteDeviceObjects();
		util::del(m_pScrollLine[i]);
	}
	m_pScrollButton->DeleteDeviceObjects();
	m_pChatShowImage->DeleteDeviceObjects();
	util::del(m_pScrollButton);
	util::del(m_pChatShowImage);
	for (i = 0; i < BUTTON_BOTH_STATE_NUMBER; i++)
	{
		m_pChatViewButton[0][i]->DeleteDeviceObjects();
		m_pChatViewButton[1][i]->DeleteDeviceObjects();
		util::del(m_pChatViewButton[0][i]);
		util::del(m_pChatViewButton[1][i]);
		m_pImgWisperBtn[0][i]->DeleteDeviceObjects();
		m_pImgWisperBtn[1][i]->DeleteDeviceObjects();
		util::del(m_pImgWisperBtn[0][i]);
		util::del(m_pImgWisperBtn[1][i]);
	}

	m_pFontInput->DeleteDeviceObjects();
	util::del(m_pFontInput);

	vectorINFGameMainWisperChatPtr::iterator it(m_vectorINFiGameMainWisperChatPtr.begin());
	while (it != m_vectorINFiGameMainWisperChatPtr.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del(*it);
		it++;
	}
	m_vectorINFiGameMainWisperChatPtr.clear();

	m_pSystemMsgW->DeleteDeviceObjects();
	util::del(m_pSystemMsgW);

	m_pFontGM->DeleteDeviceObjects();
	util::del(m_pFontGM);

	// 추가수정 2005.01.21
	for (i = 0; i < CHAT_BUTTON_NUMBER3; i++)
	{
		m_pNChatFAQ[i]->DeleteDeviceObjects();
		m_pNChatMenuView[i]->DeleteDeviceObjects();
		util::del(m_pNChatFAQ[i]);								// 채팅 FAQ버튼
		util::del(m_pNChatMenuView[i]);								// 채팅 메뉴 보이기 버튼
		m_pNChatVOIPStartMenu[i]->DeleteDeviceObjects();
		m_pNChatVOIPEndMenu[i]->DeleteDeviceObjects();
		util::del(m_pNChatVOIPStartMenu[i]);
		util::del(m_pNChatVOIPEndMenu[i]);
	}
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pNChatCash[i]->DeleteDeviceObjects();
		util::del(m_pNChatCash[i]);								// 채팅 채쉬충전 버튼
#endif
		m_pNChatCashView[i]->DeleteDeviceObjects();
		util::del(m_pNChatCashView[i]);
	}
	for (i = 0; i < INPUT_LANGUAGE_END; i++)
	{
		m_pNChatInputLanguage[i]->DeleteDeviceObjects();			// 채팅 입력 언어
		util::del(m_pNChatInputLanguage[i]);
	}
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		m_pNChatMenu[i]->DeleteDeviceObjects();
		util::del(m_pNChatMenu[i]);								// 채팅 메뉴 리스트
	}
	m_pNChatInput->DeleteDeviceObjects();
	util::del(m_pNChatInput);									// 채팅 입력창
	m_pNChatCheck->DeleteDeviceObjects();
	util::del(m_pNChatCheck);									// 채팅 채크마크

	for (i = 0; i < CHAT_TAB_CHATROOM; i++)
	{
		if (NULL != m_pBtnChatMode[i])
		{
			m_pBtnChatMode[i]->DeleteDeviceObjects();
			util::del(m_pBtnChatMode[i]);
		}
	}
	m_pBtnPartyQuick->DeleteDeviceObjects();
	util::del(m_pBtnPartyQuick);
	m_pBtnGuildQuick->DeleteDeviceObjects();
	util::del(m_pBtnGuildQuick);

	m_pChatMacroBG->DeleteDeviceObjects();
	util::del(m_pChatMacroBG);

	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		m_pBtnChatMacro[i]->DeleteDeviceObjects();
		util::del(m_pBtnChatMacro[i]);
	}
	m_pBtnMacroOK->DeleteDeviceObjects();
	util::del(m_pBtnMacroOK);
	m_pBtnMacroCancel->DeleteDeviceObjects();
	util::del(m_pBtnMacroCancel);
	m_pBtnOption->DeleteDeviceObjects();
	util::del(m_pBtnOption);

	/*{
		m_pBtnVoiceChatInv->DeleteDeviceObjects();
		util::del(m_pBtnVoiceChatInv);
		m_pBtnVoiceChatOk->DeleteDeviceObjects();
		util::del(m_pBtnVoiceChatOk);
		m_pBtnVoiceChatCan->DeleteDeviceObjects();
		util::del(m_pBtnVoiceChatCan);

		m_pBothParty->DeleteDeviceObjects();
		util::del(m_pBothParty);
		m_pBothGuild->DeleteDeviceObjects();
		util::del(m_pBothGuild);
		m_pBothChat->DeleteDeviceObjects();
		util::del(m_pBothChat);
		m_pBothWhisper->DeleteDeviceObjects();
		util::del(m_pBothWhisper);

		m_pVoiceChatBG->DeleteDeviceObjects();
		util::del(m_pVoiceChatBG);
		m_pImgSpkVolBar->DeleteDeviceObjects();
		util::del(m_pImgSpkVolBar);
		m_pImgRadioBtn[0]->DeleteDeviceObjects();
		util::del(m_pImgRadioBtn[0]);
		m_pImgRadioBtn[1]->DeleteDeviceObjects();
		util::del(m_pImgRadioBtn[1]);

	}*/


	// 2008. 12. 29 by ckPark 채팅창 최적화	
	// 각 채팅 라인 삭제
	for (i = 0; i < MAX_GAME_MAIN_CHAT_LINE; ++i)
	{
		m_pFontLine[i]->DeleteDeviceObjects();
		util::del(m_pFontLine[i]);
	}

	// end 2008. 12. 29 by ckPark 채팅창 최적화	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if (m_pBtnChatBG)
	{
		m_pBtnChatBG->DeleteDeviceObjects();
		util::del(m_pBtnChatBG);
	}
#endif
	return S_OK;
}


HRESULT CINFGameMainChat::InvalidateDeviceObjects()
{
	FLOG("CINFGameMainChat::InvalidateDeviceObjects()");
	//	for(int i=0;i<5;i++)
	//	{
	//		for(int j=0;j<2;j++)
	//		{
	//			m_pTabButton[i][j]->InvalidateDeviceObjects();
	//		}
	//	}
	int i;
	for (i = 0; i < 9; i++)
	{
		m_pChatBox[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < 3; i++)
	{
		m_pScrollLine[i]->InvalidateDeviceObjects();
	}
	m_pScrollButton->InvalidateDeviceObjects();

	m_pChatShowImage->InvalidateDeviceObjects();
	for (i = 0; i < BUTTON_BOTH_STATE_NUMBER; i++)
	{
		m_pChatViewButton[0][i]->InvalidateDeviceObjects();
		m_pChatViewButton[1][i]->InvalidateDeviceObjects();
		m_pImgWisperBtn[0][i]->InvalidateDeviceObjects();
		m_pImgWisperBtn[1][i]->InvalidateDeviceObjects();
	}

	m_pFontInput->InvalidateDeviceObjects();
	m_pFontGM->InvalidateDeviceObjects();

	vector<CD3DHanFont*>::iterator it = m_vecFontLine.begin();
	while (it != m_vecFontLine.end())
	{
		CD3DHanFont* pFont = *it;
		it++;
		pFont->InvalidateDeviceObjects();
		pFont->DeleteDeviceObjects();
		delete pFont;
		pFont = NULL;
	}
	m_vecFontLine.clear();
	vectorINFGameMainWisperChatPtr::iterator itr(m_vectorINFiGameMainWisperChatPtr.begin());
	while (itr != m_vectorINFiGameMainWisperChatPtr.end())
	{
		(*itr)->InvalidateDeviceObjects();
		itr++;
	}
	m_pSystemMsgW->InvalidateDeviceObjects();


	// 추가수정 2005.01.21
	for (i = 0; i < CHAT_BUTTON_NUMBER3; i++)
	{
		m_pNChatFAQ[i]->InvalidateDeviceObjects();								// 채팅 FAQ버튼
		m_pNChatMenuView[i]->InvalidateDeviceObjects();						// 채팅 메뉴 보이기 버튼
		m_pNChatVOIPStartMenu[i]->InvalidateDeviceObjects();
		m_pNChatVOIPEndMenu[i]->InvalidateDeviceObjects();
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		m_pNChatCash[i]->InvalidateDeviceObjects();							// 채팅 채쉬충전 버튼
	}
#endif
	for (i = 0; i < CHAT_BUTTON_NUMBER4; i++)
	{
		m_pNChatCashView[i]->InvalidateDeviceObjects();
	}

	for (i = 0; i < INPUT_LANGUAGE_END; i++)
		m_pNChatInputLanguage[i]->InvalidateDeviceObjects();
	for (i = 0; i < CHAT_MENULIST_NUMBER; i++)
	{
		m_pNChatMenu[i]->InvalidateDeviceObjects();							// 채팅 메뉴 리스트
	}
	m_pNChatInput->InvalidateDeviceObjects();									// 채팅 입력창
	m_pNChatCheck->InvalidateDeviceObjects();									// 채팅 채크마크

	for (i = 0; i < CHAT_TAB_CHATROOM; i++)
	{
		if (NULL != m_pBtnChatMode[i])
		{
			m_pBtnChatMode[i]->InvalidateDeviceObjects();
		}
	}
	m_pBtnPartyQuick->InvalidateDeviceObjects();
	m_pBtnGuildQuick->InvalidateDeviceObjects();

	m_pChatMacroBG->InvalidateDeviceObjects();

	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		m_pBtnChatMacro[i]->InvalidateDeviceObjects();
	}
	m_pBtnMacroOK->InvalidateDeviceObjects();
	m_pBtnMacroCancel->InvalidateDeviceObjects();
	m_pBtnOption->InvalidateDeviceObjects();
	/*{
		m_pBtnVoiceChatInv->InvalidateDeviceObjects();
		m_pBtnVoiceChatOk->InvalidateDeviceObjects();
		m_pBtnVoiceChatCan->InvalidateDeviceObjects();

		m_pBothParty->InvalidateDeviceObjects();
		m_pBothGuild->InvalidateDeviceObjects();
		m_pBothChat->InvalidateDeviceObjects();
		m_pBothWhisper->InvalidateDeviceObjects();

		m_pVoiceChatBG->InvalidateDeviceObjects();
		m_pImgSpkVolBar->InvalidateDeviceObjects();
		m_pImgRadioBtn[0]->InvalidateDeviceObjects();
		m_pImgRadioBtn[1]->InvalidateDeviceObjects();

	}*/



	// 2008. 12. 29 by ckPark 채팅창 최적화	
	// 각 채팅 라인 해제
	for (i = 0; i < MAX_GAME_MAIN_CHAT_LINE; ++i)
		m_pFontLine[i]->InvalidateDeviceObjects();
	// end 2008. 12. 29 by ckPark 채팅창 최적화	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if (m_pBtnChatBG)
	{
		m_pBtnChatBG->InvalidateDeviceObjects();
	}
#endif

	return S_OK;
}

void CINFGameMainChat::TickMacro()
{
	if (m_bMacroIng)
	{
		int i;
		for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
		{
			if (m_nSendMacroNum == i)
			{
				if (m_sMacroStr[i].bAppl == TRUE)
				{
					// 2008-08-18 by bhsohn 매크로 추가 수정
					if (m_sMacroStr[i].fTime >= 0)
					{
						m_sMacroStr[i].fTime -= g_pD3dApp->GetElapsedTime();
					}

					if ((m_sMacroStr[i].fTime < 0))// &&(m_nTransChatCount >= CHATMACRO_TRANSCHAT_COUNT)) // 2012-03-30 by isshin 메크로 시스템 버그 수정 - 매크로 출력 조건(시간으로) 수정
					{// 매크로 스트링 보내기.
						m_sMacroStr[i].fTime = CHAT_MACRO_OUTPUT_TIME;
						//send 매크로 스트링 서버로 보내기.
						g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_SELL_ALL, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_sMacroStr[i].strMacro);
						m_nTransChatCount = 0;
						//////////////////////////////////////////

						// 다음 보내야 할 매크로 스트링 찾기.
						int temp = i;
						temp++;
						if (temp >= CHAT_MACRO_INPUT_COUNT)
						{
							temp = 0;
						}
						for (int j = temp; j < CHAT_MACRO_INPUT_COUNT; j++)
						{
							if (m_sMacroStr[j].bAppl == TRUE)
							{
								m_nSendMacroNum = j;
								break;
							}// 2012-03-30 by isshin 매크로 시스템 버그 수정 - 매크로 3번 미설정시 출력 문제 수정
							else if (m_sMacroStr[2].bAppl == FALSE)
							{
								m_nSendMacroNum = 0;
							}// end 2012-03-30 by isshin 매크로 시스템 버그 수정 - 매크로 3번 미설정시 출력 문제 수정
						}
					}

				}
				else
				{
					// 2008-08-18 by bhsohn 매크로 추가 수정
					m_nSendMacroNum++;
					if (m_nSendMacroNum >= CHAT_MACRO_INPUT_COUNT)
					{
						m_nSendMacroNum = 0;
					}
				}
			}

		}
	}
}
void CINFGameMainChat::Tick()
{
	FLOG("CINFGameMainChat::Tick()");
	if (!m_bChatMode)
	{
		//		for(int i=0;i<CHAT_TAB_NUMBER;i++)
		{
			for (int j = 0; j < CHAT_NOT_SHOWBOX_LINE; j++)
			{
				m_fTimeOfShowChat[j] -= g_pD3dApp->GetElapsedTime();
			}
		}
	}
	TickMacro();

	CINFGameMainWisperChat *pWisper = NULL;
	vectorINFGameMainWisperChatPtr::iterator it(m_vectorINFiGameMainWisperChatPtr.begin());
	while (it != m_vectorINFiGameMainWisperChatPtr.end())
	{
		pWisper = (*it);
		(*it)->Tick();
		it++;
	}
	m_pSystemMsgW->Tick();

	if (m_fTimeGm > 0)
	{
		m_fTimeGm -= g_pD3dApp->GetElapsedTime();
	}
	if (m_fTimeInfluenceLeader > 0)
	{
		m_fTimeInfluenceLeader -= g_pD3dApp->GetElapsedTime();
	}
	// 서버에서 상점정보가 올때까지 기다린다
	if (m_bCheckShopInfo)
		CheckShopInfo();

	// by bhsohn 2007-03-12 China Working
}

struct sort_CINFGameMainWisperChat : binary_function<CINFGameMainWisperChat*, CINFGameMainWisperChat*, bool>
{
	bool operator()(CINFGameMainWisperChat *pWisper1, CINFGameMainWisperChat *pWisper2)
	{
		return pWisper1->m_nVectorIndex < pWisper2->m_nVectorIndex;	// 오름 차순 정렬
	};
};

void CINFGameMainChat::Render()
{
	FLOG("CINFGameMainChat::Render()");

	// 시티와 도시판단후 좌표 변경 
	if (g_pD3dApp->m_dwGameState == _CITY)
	{
		// 도시이면 인터페이스 보이기
// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
//		m_nCashButtonGab = 45;
		m_nChatBoxYPos = g_pD3dApp->GetBackBufferDesc().Height - CHATBOX_CITY_SHOWCHATBOX_INIT_HEIGHT - m_nChatBoxHeight;
		g_pInterface->m_bShowInterface = TRUE;
	}
	else
	{
		if (g_pD3dApp->m_bCharacter == TRUE && !g_pD3dApp->m_bBazaar)
		{
			// 2008-05-19 by dgwoo 메인 인터페이스 위치 변경.
			//			m_nCashButtonGab = 32;
		}
		else
		{
			m_nCashButtonGab = 0;
		}
		m_nChatBoxYPos = g_pD3dApp->GetBackBufferDesc().Height - CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT - m_nChatBoxHeight;
	}

	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	// 풀스크린시 챗박스 감추기
//	if(!g_pInterface->m_bShowInterface&& m_bChatMode == TRUE)
//	{
//		m_pChatBox[4]->Move(CHAT_MENUBOX_START_X, CHAT_FONT_START_Y+CHAT_FONT_WIDTH_ENGLISH);
//		m_pChatBox[4]->SetScale(CHAT_MENUBOX_START_X+CHAT_INPUT_FONT_LENGTH, CHATBOX_IMAGE_GAB_HEITHT_TOP);
//		m_pChatBox[4]->Render();
//	}	
//	if(!g_pInterface->m_bShowInterface) m_bShowChatBox = FALSE;
//	if(m_bShowChatBox && g_pInterface->m_bShowInterface)
//	{
//		DrawChatWindow(CHAT_CHATBOX_START_X, m_nChatBoxYPos, m_nChatBoxWidth, m_nChatBoxHeight);
//	}
	// 풀스크린시 챗박스 감추기
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if (!g_pInterface->m_bShowInterface&& m_bChatMode == TRUE && !g_pShuttleChild->IsObserverMode())
	{
		m_pChatBox[4]->Move(CHAT_MENUBOX_START_X, CHAT_FONT_START_Y + CHAT_FONT_WIDTH_ENGLISH);
		m_pChatBox[4]->SetScale(CHAT_MENUBOX_START_X + CHAT_INPUT_FONT_LENGTH, CHATBOX_IMAGE_GAB_HEITHT_TOP);
		m_pChatBox[4]->Render();
	}
#endif

	if (!g_pInterface->m_bShowInterface || g_pShuttleChild->IsObserverMode())
	{
		m_bShowChatBox = FALSE;
	}
	if (m_bShowChatBox && g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode())
	{
		DrawChatWindow(CHAT_CHATBOX_START_X, m_nChatBoxYPos, m_nChatBoxWidth, m_nChatBoxHeight);
	}
	// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김

	// 채팅 박스 메뉴 (노말, 귓말, 파티, 길드, 음성)
	if (g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode())
	{
		// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
		int i;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pBtnChatBG->Move(CHAT_CHATMODE_X, CHAT_CHATMODE_Y + 2);
		m_pBtnChatBG->Render();
#endif

		for (i = 0; i < CHAT_TAB_CHATROOM; i++)
		{
			m_pBtnChatMode[i]->Render();
		}
		m_pNChatInput->Move(CHAT_WINDOW_X, CHAT_WINDOW_Y);
		m_pNChatInput->Render();

		m_pBtnOption->Render();

		// 추가 수정 채팅 창
		m_pNChatMenuView[m_nMenuListButtonState]->Move(CHAT_MENU_VIEW_X, CHAT_MENU_VIEW_Y);
		m_pNChatMenuView[m_nMenuListButtonState]->Render();

		// 2006-03-16 by ispark
		int nSelectLanguage = g_pD3dApp->m_inputkey.GetInputLanguage() - 1;
		if (nSelectLanguage < 0)
			nSelectLanguage = 0;
		m_pNChatInputLanguage[nSelectLanguage]->Move(CHAT_INPUT_LANGUAGE_X, CHAT_INPUT_LANGUAGE_Y);
		m_pNChatInputLanguage[nSelectLanguage]->Render();

		m_pChatViewButton[m_bShowChatBox][m_nChatSizeBtnNormal]->Move(CHAT_CHATTAB_VIEW_FLAG_X,
			CHAT_CHATTAB_VIEW_FLAG_Y);
		m_pChatViewButton[m_bShowChatBox][m_nChatSizeBtnNormal]->Render();				// 최대화


		// 채팅창 최소화
		if (m_bShowChatBox)
		{
			// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가
			//			m_pChatViewButton[CHAT_BOX_HIDE_IMAGE][m_nSizeMaxButtonstate]->Move(CHAT_CHATBOX_START_X+m_nChatBoxWidth-CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH,
			//				m_nChatBoxYPos + CHAT_MENUBOX_GAB_WIDTH);
			//			m_pChatViewButton[CHAT_BOX_HIDE_IMAGE][m_nSizeMaxButtonstate]->Render();			// 최소화 	

						// 창 화면에 "채팅" 표시 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			m_pChatShowImage->Move(CHAT_MENUBOX_START_X, m_nChatBoxYPos - CHAT_MENUBOX_GAB_WIDTH);
#else
			m_pChatShowImage->Move(CHAT_MENUBOX_START_X, m_nChatBoxYPos + CHAT_MENUBOX_GAB_WIDTH);
#endif
			m_pChatShowImage->Render();
		}
	}

	if (m_bChatMode)
	{
		int nShowTemp = 0;

		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
		//if(!g_pInterface->m_bShowInterface)
		if (!g_pInterface->m_bShowInterface || g_pShuttleChild->IsObserverMode())
		{
			nShowTemp = -25;
		}

		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
// 		char chatbuf[SIZE_MAX_CHAT_MESSAGE+2];
// 		memset(chatbuf,0x00,SIZE_MAX_CHAT_MESSAGE);
// 		strncpy(chatbuf,m_strInputMessage,SIZE_MAX_CHAT_MESSAGE);		

// 		chatbuf[strlen(m_strInputMessage)] = '_';
// 		chatbuf[strlen(m_strInputMessage)+1] = '\0';
//		m_pFontInput->SetTextureWidth(CHAT_MENUBOX_START_X+CHAT_INPUT_FONT_LENGTH);		
// 		m_pFontInput->SetTextureWidth(CHAT_INPUT_FONT_LENGTH);		
// 		m_pFontInput->DrawText(CHAT_FONT_START_X+nShowTemp,CHAT_FONT_START_Y + FONT_INPUT_Y,GUI_FONT_COLOR_Y,chatbuf, 0L);		
		RenderCursel(nShowTemp);
		// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	}

	ChatTab_t *pCurChatTab = GetChatTabMode();
	if (pCurChatTab->m_nStoredLineCounts <= 0)
	{
	}
	else
	{
		if (m_bShowChatBox)
		{
			// 채팅창 활성화 상태
			int nMaxRenderLineCounts = (m_nChatBoxHeight - CHATBOX_IMAGE_GAB_HEITHT_TOP) / CHAT_FONT_LINE_HEIGHT;
			int nRenderIndex = pCurChatTab->m_nRenderStartIndex;
			int nRenderedLineCounts = 0;
			int nLineCount = 1;

			// 공지 사항 렌더링
			if (m_fTimeGm > 0 && m_bHelpDeskRender == FALSE)
			{
				// 2007-08-07 by bhsohn 스피커 아이템 추가
//				m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, 
//					m_nChatBoxYPos-15,
//					COLOR_CHAT_ALL,
//					m_strInputGm);
				// end 2007-08-07 by bhsohn 스피커 아이템 추가
			}
			else if (m_fTimeGm > 0 && m_bHelpDeskRender == TRUE)
			{
				auto& vectemp = m_vecInputGM;
				if (vectemp.size() == 0) STRING_CULL2(m_strInputGm, CHAT_HELPDESK_STRLEN, &vectemp, m_pFontGM);
				int igm = 0;
				nLineCount = vectemp.size();

				for (igm = 0; igm < vectemp.size(); igm++)
				{
					m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, m_nChatBoxYPos - (nLineCount * 15), COLOR_CHAT_ALL, (char*)vectemp[igm].c_str());
					nLineCount--;
				}
				nLineCount = vectemp.size() + 1;
			}
			// 2008-05-20 by dgwoo 도움말과 세력채팅을 분리.
			//			if(m_fTimeInfluenceLeader>0 && !(m_fTimeGm>0 && m_bHelpDeskRender == TRUE))
			//			{
			//				m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, m_nChatBoxYPos - 30, 
			//					COLOR_INFLUENCE, 
			//					m_strInputInfluenceLeader);
			//			}
			if (m_fTimeInfluenceLeader > 0)
			{
				m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, m_nChatBoxYPos - (nLineCount * 15),
					COLOR_INFLUENCE,
					m_strInputInfluenceLeader);
			}

			//--------------------------------------------------------------------------//
			//   채팅창에 편대 바로가기 , 또는 여단가입창을 바로 띄울수 있는 인터페이스 버튼. 

			if (g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber == 0 &&
				m_nChatMode == CHAT_TAB_GUILD)
			{
				m_pBtnGuildQuick->SetBtnPosition(CHAT_NFONT_START_LINE_X
					, m_nChatBoxYPos + m_nChatBoxHeight - (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*(nRenderedLineCounts + 1)));
				m_pBtnGuildQuick->Render();
			}
			else if (g_pShuttleChild->m_pClientParty->IsParty() == FALSE &&
				m_nChatMode == CHAT_TAB_PARTY)
			{
				m_pBtnPartyQuick->SetBtnPosition(CHAT_NFONT_START_LINE_X
					, m_nChatBoxYPos + m_nChatBoxHeight - (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*(nRenderedLineCounts + 1)));
				m_pBtnPartyQuick->Render();
			}


			// 만약 컬링 스트링이 초기화 됬다�?다시 생성한다
			if (m_listCulledString.empty())
				BuildCulledString();

			BOOL bBreak = FALSE;	// 루프 탈출변수
			CulledStringListItor	end = m_listCulledString.end();
			for (CulledStringListItor it = m_listCulledString.begin(); it != end && !bBreak; ++it)
			{
				// 2012-11-09 by bhsohn 채팅창 Null오류 소지 제거
				if (it == end)
				{
					//					DbgOut("it != end #1 \n");
					break;
				}
				// END 2012-11-09 by bhsohn 채팅창 Null오류 소지 제거

				// 잘려진 스트링 벡터(밑에서 부터 찍을 것이기 때문에 reverse로 순회)
				std::vector< string >::reverse_iterator vEnd = (*it).rend();

				for (std::vector< string >::reverse_iterator vIt = (*it).rbegin(); vIt != vEnd && !bBreak; ++vIt)
				{
					// 최대 렌더링 한도 넘어 갈 시 루프 탈출
					if (nMaxRenderLineCounts <= nRenderedLineCounts)
					{
						bBreak = TRUE;
						break;
					}
					// 2012-11-09 by bhsohn 채팅창 Null오류 소지 제거
					if (vIt == vEnd)
					{
						bBreak = TRUE;
						//						DbgOut("it != end #1 \n");
						break;
					}
					// END 2012-11-09 by bhsohn 채팅창 Null오류 소지 제거

					// 렌더링
					m_pFontLine[nRenderedLineCounts]->DrawText(CHAT_NFONT_START_LINE_X,
						m_nChatBoxYPos + m_nChatBoxHeight
						- (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT * nRenderedLineCounts),
						pCurChatTab->GetColorType(nRenderIndex),
						(char*)((*vIt).c_str()),
						0L);
					// 렌더링 라인수 증가
					nRenderedLineCounts++;
				}

				// 저장된 라인수를 넘어 갈 경우 루프 탈출
				if (nRenderIndex == pCurChatTab->m_nStoreEndIndex
					|| nMaxRenderLineCounts <= nRenderedLineCounts)
					bBreak = TRUE;

				// 원형큐 인덱스 설정
				if (0 > --nRenderIndex)
					nRenderIndex = CHAT_BUFFER_NUMBER - 1;
			}

			// end 2008. 12. 29 by ckPark 채팅창 최적화





		}
		else
		{
			// 채팅창 비활성화

			int nMaxRenderLineCounts = CHAT_NOT_SHOWBOX_LINE;
			int nRenderIndex = pCurChatTab->m_nRenderStartIndex;
			int nRenderedLineCounts = 0;
			int nChatHeightCity = 0;
			int nLineCount = 1;

			if (g_pD3dApp->m_dwGameState == _CITY)

				nChatHeightCity = m_nChatBoxYPos + m_nChatBoxHeight;

			else nChatHeightCity = CHAT_NFONT_START_LINE_Y;//CHAT_FONT_START_Y-10;



			// 공지 사항 렌더링
			if (m_fTimeGm > 0 && m_bHelpDeskRender == FALSE)
			{
				// 2007-08-07 by bhsohn 스피커 아이템 추가
//				m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, 
//					nChatHeightCity - (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*7),
//					COLOR_CHAT_ALL,
//					m_strInputGm);
				// end 2007-08-07 by bhsohn 스피커 아이템 추가
			}
			else if (m_fTimeGm > 0 && m_bHelpDeskRender == TRUE)
			{
				auto& vectemp = m_vecInputGM;
				if (vectemp.size() == 0) STRING_CULL2(m_strInputGm, CHAT_HELPDESK_STRLEN, &vectemp, m_pFontGM);
				int igm = 0;
				nLineCount = vectemp.size();

				for (igm = 0; igm < vectemp.size(); igm++)
				{
					m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, m_nChatBoxYPos - (nLineCount * 15), COLOR_CHAT_ALL, (char*)vectemp[igm].c_str());
					nLineCount--;
				}
				nLineCount = vectemp.size() + 1;
			}
			// 2008-05-20 by dgwoo 도움말과 세력채팅을 분리.		
			//			if(m_fTimeInfluenceLeader>0 && !(m_fTimeGm>0 && m_bHelpDeskRender == TRUE))
			//			{
			//				m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X, 
			//					nChatHeightCity - (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT*8), 
			//					COLOR_INFLUENCE, 
			//					m_strInputInfluenceLeader);
			//			}
			if (m_fTimeInfluenceLeader > 0)
			{
				m_pFontGM->DrawText(CHAT_NFONT_START_LINE_X,
					m_nChatBoxYPos - (nLineCount * 15),
					COLOR_INFLUENCE,
					m_strInputInfluenceLeader);
			}



			// 만약 컬링 스트링이 초기화 됬다�?다시 생성한다
			if (m_listCulledString.empty()) BuildCulledString();

			BOOL bBreak = FALSE;	// 루프 탈출변수
			CulledStringListItor	end = m_listCulledString.end();
			for (CulledStringListItor it = m_listCulledString.begin(); it != end && !bBreak; ++it)
			{
				// 잘려진 스트링 벡터(밑에서 부터 찍을 것이기 때문에 reverse로 순회)
				std::vector< string >::reverse_iterator vEnd = (*it).rend();

				for (std::vector< string >::reverse_iterator vIt = (*it).rbegin(); vIt != vEnd && !bBreak; ++vIt)
				{
					// 2012-11-09 by bhsohn 채팅창 Null오류 소지 제거
					if (vIt == vEnd)
					{
						//						DbgOut("vIt == vEnd #1 \n");
						bBreak = TRUE;
						break;
					}
					// END 2012-11-09 by bhsohn 채팅창 Null오류 소지 제거

					// 최대 렌더링 한도 넘어 갈 시 루프 탈출
					if (nMaxRenderLineCounts <= nRenderedLineCounts)
					{
						bBreak = TRUE;
						break;
					}

					// 렌더링
					m_pFontLine[nRenderedLineCounts]->DrawText(CHAT_NFONT_START_LINE_X,
						m_nChatBoxYPos + m_nChatBoxHeight
						- (CHAT_FONT_LINE_HEIGHT + CHAT_FONT_LINE_HEIGHT * nRenderedLineCounts),
						pCurChatTab->GetColorType(nRenderIndex),
						(char*)((*vIt).c_str()),
						0L);
					// 렌더링 라인수 증가
					nRenderedLineCounts++;
				}

				// 저장된 라인수를 넘어 갈 경우 루프 탈출
				if (nRenderIndex == pCurChatTab->m_nStoreEndIndex
					|| nMaxRenderLineCounts <= nRenderedLineCounts)
					bBreak = TRUE;

				// 원형큐 인덱스 설정
				if (0 > --nRenderIndex)
					nRenderIndex = CHAT_BUFFER_NUMBER - 1;
			}

			// end 2008. 12. 29 by ckPark 채팅창 최적화	





		}
	}
	
	if (!g_pShuttleChild->IsObserverMode()
		&& g_pInterface->m_bShowInterface
		&& g_pD3dApp->m_dwGameState != _SHOP
		&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_GAMING)	// 2008-02-27 by bhsohn 통합 아레나 수정
	{

		m_pNChatCashView[m_nOtherCashList]->Move(CHAT_MENU_CASH_START_X - 3, CHAT_MENU_CASH_START_Y - m_nCashButtonGab);
		m_pNChatCashView[m_nOtherCashList]->Render();
	}

	// 후보창 리스트 (IME)
	if (SET_MAIN_CHAT_CANDIDATE == g_nRenderCandidate)
		g_pD3dApp->RenderCandidate(CHAR_INF_CANDIDATE_POS_X, CHAR_INF_CANDIDATE_POS_Y);

	// 2007-05-21 by bhsohn China IME Working
	g_pD3dApp->RenderIMEType(CHAR_INF_CANDIDATE_POS_X, CHAR_INF_CANDIDATE_POS_Y);
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
void CINFGameMainChat::RenderSystemMsg()
{
	if (m_pSystemMsgW)
	{
		m_pSystemMsgW->Render();
	}
}
//void CINFGameMainChat::RenderVoiceChatOption()
//{
//	if (!m_bShowVoiceChatControl)
//		return;
//	m_pVoiceChatBG->Move(m_nVCCPosX, m_nVCCPosY);
//	m_pVoiceChatBG->Render();
//
//	m_pBothParty->SetBtnPosition(VOICECHAT_PARTY_POS_X, VOICECHAT_PARTY_POS_Y);
//	m_pBothParty->Render();
//	m_pBothGuild->SetBtnPosition(VOICECHAT_GUILD_POS_X, VOICECHAT_GUILD_POS_Y);
//	m_pBothGuild->Render();
//	m_pBothChat->SetBtnPosition(VOICECHAT_CHATROOM_POS_X, VOICECHAT_CHATROOM_POS_Y);
//	m_pBothChat->Render();
//	m_pBothWhisper->SetBtnPosition(VOICECHAT_WHISPER_POS_X, VOICECHAT_WHISPER_POS_Y);
//	m_pBothWhisper->Render();
//
//	if (m_nVoiceInput)
//	{
//		m_pImgRadioBtn[0]->Move(VOICECHAT_INPUT_KEY_POS_X, VOICECHAT_INPUT_KEY_POS_Y);
//		m_pImgRadioBtn[0]->Render();
//		m_pImgRadioBtn[1]->Move(VOICECHAT_INPUT_FREE_POS_X, VOICECHAT_INPUT_FREE_POS_Y);
//		m_pImgRadioBtn[1]->Render();
//	}
//	else
//	{
//		m_pImgRadioBtn[1]->Move(VOICECHAT_INPUT_KEY_POS_X, VOICECHAT_INPUT_KEY_POS_Y);
//		m_pImgRadioBtn[1]->Render();
//		m_pImgRadioBtn[0]->Move(VOICECHAT_INPUT_FREE_POS_X, VOICECHAT_INPUT_FREE_POS_Y);
//		m_pImgRadioBtn[0]->Render();
//	}
//
//	m_pImgSpkVolBar->Move(VOICECHAT_SPK_POS_X + m_nPosVolum, VOICECHAT_SPK_POS_Y);
//	m_pImgSpkVolBar->Render();
//
//	m_pBtnVoiceChatInv->SetBtnPosition(VOICECHAT_INV_BTN_POS_X, VOICECHAT_INV_BTN_POS_Y);
//	m_pBtnVoiceChatInv->Render();
//	m_pBtnVoiceChatOk->SetBtnPosition(VOICECHAT_OK_BTN_POS_X, VOICECHAT_OK_BTN_POS_Y);
//	m_pBtnVoiceChatOk->Render();
//	m_pBtnVoiceChatCan->SetBtnPosition(VOICECHAT_CAN_BTN_POS_X, VOICECHAT_CAN_BTN_POS_Y);
//	m_pBtnVoiceChatCan->Render();
//}

void CINFGameMainChat::DrawChatWindow(int x, int y, int cx, int cy)
{
	FLOG("CINFGameMainChat::DrawChatWindow(int x, int y, int cx, int cy)");

	m_pChatBox[0]->Move(x, y);
	m_pChatBox[0]->Render();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nPosX = x + m_pChatBox[0]->GetImgSize().x;
	int nImageSizeX = (CHAT_CHATBOX_START_DEFAULT_MIN_X - m_pChatBox[0]->GetImgSize().x - m_pChatBox[2]->GetImgSize().x) / 2;

	m_pChatBox[1]->Move(nPosX, y);
	m_pChatBox[1]->SetRect(0, 0, nImageSizeX, m_pChatBox[1]->GetImgSize().y);
	m_pChatBox[1]->SetScale(1, 1);
	m_pChatBox[1]->Render();

	nPosX += nImageSizeX;
	m_pChatBox[1]->Move(nPosX, y);
	m_pChatBox[1]->SetRect(nImageSizeX, 0, nImageSizeX + 1, m_pChatBox[1]->GetImgSize().y);
	m_pChatBox[1]->SetScale(cx - nImageSizeX - nPosX, 1);
	m_pChatBox[1]->Render();

	m_pChatBox[1]->Move(cx - nImageSizeX, y);
	m_pChatBox[1]->SetRect(m_pChatBox[1]->GetImgSize().x - nImageSizeX, 0, m_pChatBox[1]->GetImgSize().x, m_pChatBox[1]->GetImgSize().y);
	m_pChatBox[1]->SetScale(1, 1);
	m_pChatBox[1]->Render();

	m_pChatBox[2]->Move(x + cx, y);
	m_pChatBox[2]->Render();

	m_pChatBox[3]->Move(x, y + m_pChatBox[0]->GetImgSize().y);
	m_pChatBox[3]->SetScale(1, (float)(cy - m_pChatBox[0]->GetImgSize().y) / m_pChatBox[3]->GetImgSize().y);
	m_pChatBox[3]->Render();

	m_pChatBox[4]->Move(x + m_pChatBox[3]->GetImgSize().x, y + m_pChatBox[1]->GetImgSize().y);
	//m_pChatBox[4]->SetScale((cx-CHATBOX_IMAGE_GAB_WIDTH_MIDDLE-CHATBOX_IMAGE_GAB_WIDTH_MIDDLE)+CHATBOX_IMAGE_GAB_HEITHT_TOP, cy-CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[4]->SetRect(0, 0, cx - m_pChatBox[3]->GetImgSize().x + m_pChatBox[2]->GetImgSize().x, cy - m_pChatBox[1]->GetImgSize().y);
	m_pChatBox[4]->Render();

	m_pChatBox[5]->Move(x + (cx + m_pChatBox[2]->GetImgSize().x - m_pChatBox[5]->GetImgSize().x), y + m_pChatBox[2]->GetImgSize().y);
	m_pChatBox[5]->SetScale(1, (float)(cy - m_pChatBox[0]->GetImgSize().y) / m_pChatBox[3]->GetImgSize().y);
	m_pChatBox[5]->Render();

	m_pChatBox[6]->Move(x, y + cy);
	m_pChatBox[6]->Render();

	nPosX = x + m_pChatBox[6]->GetImgSize().x;
	nImageSizeX = (CHAT_CHATBOX_START_DEFAULT_MIN_X - m_pChatBox[6]->GetImgSize().x - m_pChatBox[8]->GetImgSize().x) / 2;

	m_pChatBox[7]->Move(nPosX, y + cy);
	m_pChatBox[7]->SetRect(0, 0, nImageSizeX, m_pChatBox[7]->GetImgSize().y);
	m_pChatBox[7]->SetScale(1, 1);
	m_pChatBox[7]->Render();

	nPosX += nImageSizeX;
	m_pChatBox[7]->Move(nPosX, y + cy);
	m_pChatBox[7]->SetRect(nImageSizeX, 0, nImageSizeX + 1, m_pChatBox[7]->GetImgSize().y);
	m_pChatBox[7]->SetScale(cx - nImageSizeX - nPosX, 1);
	m_pChatBox[7]->Render();

	m_pChatBox[7]->Move(cx - nImageSizeX, y + cy);
	m_pChatBox[7]->SetRect(m_pChatBox[1]->GetImgSize().x - nImageSizeX, 0, m_pChatBox[7]->GetImgSize().x, m_pChatBox[7]->GetImgSize().y);
	m_pChatBox[7]->SetScale(1, 1);
	m_pChatBox[7]->Render();

	//m_pChatBox[7]->Move(x+m_pChatBox[6]->GetImgSize().x,y+cy);
	//m_pChatBox[7]->SetScale((float)(cx - m_pChatBox[0]->GetImgSize().x) / m_pChatBox[7]->GetImgSize().x, 1);
	//m_pChatBox[7]->Render();

	m_pChatBox[8]->Move(x + cx, y + cy);
	m_pChatBox[8]->Render();
#else					
	m_pChatBox[1]->Move(x + CHATBOX_IMAGE_GAB_WIDTH_TOP, y);
	m_pChatBox[1]->SetScale(cx - CHATBOX_IMAGE_GAB_WIDTH_TOP, 1);
	m_pChatBox[1]->Render();
	m_pChatBox[2]->Move(x + cx, y);
	m_pChatBox[2]->Render();

	m_pChatBox[3]->Move(x, y + CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[3]->SetScale(1, cy - CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[3]->Render();
	m_pChatBox[4]->Move(x + CHATBOX_IMAGE_GAB_WIDTH_MIDDLE, y + CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[4]->SetScale((cx - CHATBOX_IMAGE_GAB_WIDTH_MIDDLE - CHATBOX_IMAGE_GAB_WIDTH_MIDDLE) + CHATBOX_IMAGE_GAB_HEITHT_TOP, cy - CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[4]->Render();
	m_pChatBox[5]->Move(x + (cx - CHATBOX_IMAGE_GAB_WIDTH_MIDDLE) + CHATBOX_IMAGE_GAB_HEITHT_TOP, y + CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[5]->SetScale(1, cy - CHATBOX_IMAGE_GAB_HEITHT_TOP);
	m_pChatBox[5]->Render();

	m_pChatBox[6]->Move(x, y + cy);
	m_pChatBox[6]->Render();
	m_pChatBox[7]->Move(x + CHATBOX_IMAGE_GAB_WIDTH_BOTTOM, y + cy);
	m_pChatBox[7]->SetScale(cx - CHATBOX_IMAGE_GAB_WIDTH_BOTTOM, 1);
	m_pChatBox[7]->Render();
	m_pChatBox[8]->Move(x + cx - CHATBOX_IMAGE_GAB_HEITHT_MIDDLE, y + cy);
	m_pChatBox[8]->Render();
#endif

	ChatTab_t * pChatTab = GetChatTabMode();
	if (m_nCurrentRelScrollPosY < CHATBOX_IMAGE_GAB_HEITHT_TOP)
	{
		m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP;
		pChatTab->SetRenderStartIndexWithScrollPositon(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
			, m_nCurrentRelScrollPosY - CHATBOX_IMAGE_GAB_HEITHT_TOP);
	}
	else if (m_nCurrentRelScrollPosY > cy - CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
	{
		m_nCurrentRelScrollPosY = cy - CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT;
		pChatTab->SetRenderStartIndexWithScrollPositon(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
			, m_nCurrentRelScrollPosY - CHATBOX_IMAGE_GAB_HEITHT_TOP);
	}
	m_pScrollLine[CHAT_SELECT_IMAGE_1]->Move(cx + CHATBOX_SCROLLLINE_IMAGE_GAP_X
		, m_nChatBoxYPos + CHATBOX_IMAGE_GAB_WIDTH_TOP + CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH);
	m_pScrollLine[CHAT_SELECT_IMAGE_1]->SetScale(1.0f, cy - CHATBOX_IMAGE_GAB_HEITHT_TOP - (CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH * 2));
	m_pScrollLine[CHAT_SELECT_IMAGE_1]->Render();
	m_pScrollButton->Move(cx + CHATBOX_SCROLL_IMAGE_GAP_X
		, m_nChatBoxYPos + m_nCurrentRelScrollPosY);

	m_pScrollButton->Render();
}
int CINFGameMainChat::WndProcWhisperChat(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bSortFlag = FALSE;
	int nReted = 0;
	CINFGameMainWisperChat *pWisper = NULL;
	vectorINFGameMainWisperChatPtr::iterator it(m_vectorINFiGameMainWisperChatPtr.begin());
	while (it != m_vectorINFiGameMainWisperChatPtr.end())
	{
		pWisper = (*it);
		if (bSortFlag)
		{
			pWisper->m_nVectorIndex--;
			it++;
		}
		else
		{
			nReted = pWisper->WndProc(uMsg, wParam, lParam);
			switch (nReted)
			{
			case INF_MSGPROC_BREAK:
			{
				g_nRenderCandidate = SET_WHISPER_CHAT_CANDIDATE;
				return INF_MSGPROC_BREAK;
			}
			case INF_MSGPROC_CLOSE:
			{
				bSortFlag = TRUE;
				pWisper->InvalidateDeviceObjects();
				pWisper->DeleteDeviceObjects();
				util::del(pWisper);
				m_vectorINFiGameMainWisperChatPtr.erase(it);
				return INF_MSGPROC_BREAK;
			}
			break;
			default:
			{
				it++;
			}
			}
		}
	}
	if (bSortFlag)
	{
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
//int	CINFGameMainChat::WndProcVoiceOption(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (FALSE == m_bShowVoiceChatControl)
//	{
//		return INF_MSGPROC_NORMAL;
//	}
//	switch (uMsg)
//	{
//	case WM_MOUSEMOVE:
//	{
//		POINT pt;
//		pt.x = LOWORD(lParam);
//		pt.y = HIWORD(lParam);
//		CheckMouseReverse(&pt);
//		if (pt.x > m_nVCCPosX &&
//			pt.x < m_nVCCPosX + m_pVoiceChatBG->GetImgSize().x &&
//			pt.y > m_nVCCPosY &&
//			pt.y < m_nVCCPosY + m_pVoiceChatBG->GetImgSize().y)
//		{
//			m_pBothParty->OnMouseMove(pt);
//			m_pBothGuild->OnMouseMove(pt);
//			m_pBothChat->OnMouseMove(pt);
//			m_pBothWhisper->OnMouseMove(pt);
//			m_pBtnVoiceChatInv->OnMouseMove(pt);
//			m_pBtnVoiceChatOk->OnMouseMove(pt);
//			m_pBtnVoiceChatCan->OnMouseMove(pt);
//		}
//		if (m_bVCCMoving)
//		{
//			m_nVCCPosX += (pt.x - m_ptVCCOldPos.x);
//			m_nVCCPosY += (pt.y - m_ptVCCOldPos.y);
//			if (m_nVCCPosX < 0)
//				m_nVCCPosX = 0;
//			else if (m_nVCCPosX > g_pD3dApp->GetBackBufferDesc().Width - VOICECHAT_BAR_W)
//				m_nVCCPosX = g_pD3dApp->GetBackBufferDesc().Width - VOICECHAT_BAR_W;
//			if (m_nVCCPosY < 0)
//				m_nVCCPosY = 0;
//			else if (m_nVCCPosY > g_pD3dApp->GetBackBufferDesc().Height - VOICECHAT_BAR_H)
//				m_nVCCPosY = g_pD3dApp->GetBackBufferDesc().Height - VOICECHAT_BAR_H;
//			m_ptVCCOldPos.x = pt.x;
//			m_ptVCCOldPos.y = pt.y;
//
//		}
//		if (m_bVolumCon)
//		{// 볼륨 크기 컨트롤중.
//			m_nPosVolum += (pt.x - m_ptOldVolumPos.x);
//			if (m_nPosVolum > VOICECHAT_SPK_W)
//			{
//				m_nPosVolum = VOICECHAT_SPK_W;
//			}
//			else if (m_nPosVolum < 0)
//			{
//				m_nPosVolum = 0;
//			}
//			DWORD nVolum;
//			float fVolum = ((float)m_nPosVolum / (float)VOICECHAT_SPK_W)*100.0f;
//			nVolum = fVolum;
//
//			m_ptOldVolumPos = pt;
//			SetVoiceChatVolum(nVolum);
//
//		}
//	}
//	break;
//	case WM_LBUTTONDOWN:
//	{
//		POINT pt;
//		pt.x = LOWORD(lParam);
//		pt.y = HIWORD(lParam);
//		CheckMouseReverse(&pt);
//		if (pt.x > m_nVCCPosX &&
//			pt.x < m_nVCCPosX + m_pVoiceChatBG->GetImgSize().x &&
//			pt.y > m_nVCCPosY &&
//			pt.y < m_nVCCPosY + m_pVoiceChatBG->GetImgSize().y)
//		{
//			m_pBothParty->OnLButtonDown(pt);
//			m_pBothGuild->OnLButtonDown(pt);
//			m_pBothChat->OnLButtonDown(pt);
//			m_pBothWhisper->OnLButtonDown(pt);
//			m_pBtnVoiceChatInv->OnLButtonDown(pt);
//
//
//			if (pt.x > m_nVCCPosX &&
//				pt.x < m_nVCCPosX + VOICECHAT_BAR_W &&
//				pt.y > m_nVCCPosY &&
//				pt.y < m_nVCCPosY + VOICECHAT_BAR_H)
//			{
//				m_bVCCMoving = TRUE;
//				m_ptVCCOldPos = pt;
//			}
//
//			//if (m_pBtnVoiceChatOk->OnLButtonDown(pt))
//			//{
//			//}
//			//if (m_pBtnVoiceChatCan->OnLButtonDown(pt))
//			//{
//			//}
//			//if (pt.x > VOICECHAT_SPK_POS_X + m_nPosVolum &&
//			//	pt.x < VOICECHAT_SPK_POS_X + m_nPosVolum + m_pImgSpkVolBar->GetImgSize().x &&
//			//	pt.y > VOICECHAT_SPK_POS_Y &&
//			//	pt.y < VOICECHAT_SPK_POS_Y + m_pImgSpkVolBar->GetImgSize().y
//			//	&& (g_pGameMain->GetVoiceType() != VOICE_NONE))	// 2008-10-20 by bhsohn VOIP 버그 수정
//			//{
//			//	m_bVolumCon = TRUE;
//			//	m_ptOldVolumPos = pt;
//			//}
//			g_pInterface->SetWindowOrder(WNDVoiceWnd);
//
//			return INF_MSGPROC_BREAK;
//		}
//	}
//	break;
//	case WM_LBUTTONUP:
//	{
//		POINT pt;
//		pt.x = LOWORD(lParam);
//		pt.y = HIWORD(lParam);
//		CheckMouseReverse(&pt);
//		m_bVCCMoving = FALSE;
//
//		if (m_bVolumCon)
//		{// 볼륨 크기 컨트롤중.				
//			if (m_nPosVolum > VOICECHAT_SPK_W)
//			{
//				m_nPosVolum = VOICECHAT_SPK_W;
//			}
//			else if (m_nPosVolum < 0)
//			{
//				m_nPosVolum = 0;
//			}
//			DWORD nVolum;
//			float fVolum = ((float)m_nPosVolum / (float)VOICECHAT_SPK_W)*100.0f;
//			nVolum = fVolum;
//
//			g_pGameMain->SetVoiceVolum(nVolum);
//			m_bVolumCon = FALSE;
//		}
//		if (pt.x > m_nVCCPosX &&
//			pt.x < m_nVCCPosX + m_pVoiceChatBG->GetImgSize().x &&
//			pt.y > m_nVCCPosY &&
//			pt.y < m_nVCCPosY + m_pVoiceChatBG->GetImgSize().y)
//		{
//			if (m_pBothParty->OnLButtonDown(pt))
//			{
//				if (m_pBothParty->GetBoth() == BUTTON_BOTH_FRONT)
//					EndVoiceParty();
//				else
//					SendVoiceParty();
//			}
//			if (m_pBothGuild->OnLButtonDown(pt))
//			{
//				if (m_pBothGuild->GetBoth() == BUTTON_BOTH_FRONT)
//					EndVoiceGuild();
//				else
//					SendVoiceGuild();
//
//			}
//			if (m_pBothChat->OnLButtonDown(pt))
//			{
//				//						m_pBothGuild->ChangeBoth(TRUE);
//				//						m_pBothParty->ChangeBoth(TRUE);
//				//						m_pBothWhisper->ChangeBoth(TRUE);
//				//						m_pBothChat->ChangeBoth();
//			}
//			if (m_pBothWhisper->OnLButtonDown(pt))
//			{
//				// 2008-10-20 by bhsohn VOIP 버그 수정
////						m_pBothGuild->ChangeBoth(TRUE);
////						m_pBothChat->ChangeBoth(TRUE);
////						m_pBothParty->ChangeBoth(TRUE);
////						if(m_pBothWhisper->GetBoth() == BUTTON_BOTH_FRONT)
////						{
////							// 음성채팅 종료.
////							SetVOIPExit();
////						}else
////						{
////							// 2008-07-15 by dgwoo 여기선 on시킬수 없으며 off만 가능하다.
////							//OnSoundButtonClick();
////						}
//						// 2009-01-19 by bhsohn VOIP 1:1통화 종료 안되는 문제 해결
//				SetVOIPExit();
//				// end 2009-01-19 by bhsohn VOIP 1:1통화 종료 안되는 문제 해결
//			}
//			if (m_pBtnVoiceChatInv->OnLButtonUp(pt))
//			{
//				OnSoundButtonClick();
//			}
//			if (m_pBtnVoiceChatOk->OnLButtonUp(pt))
//			{
//				ShowVoiceChatControl(FALSE);
//			}
//			if (m_pBtnVoiceChatCan->OnLButtonUp(pt))
//			{
//				ShowVoiceChatControl(FALSE);
//				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//				g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
//				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
//				//--------------------------------------------------------------------------//
////					COPYDATASTRUCT	copyData;
////					DWORD			nVolum = 0;
////					memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
////					AV_MSG_WM_COPYDATA voIP(_1to1Call, AV_MT_GET_SPK_VOLUME, 0);
////					copyData.dwData = (DWORD)voIP;
////					copyData.lpData = NULL;
////					copyData.cbData = 0;
////					::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);
//					//--------------------------------------------------------------------------//
//			}
//			return INF_MSGPROC_BREAK;
//		}
//
//
//	}
//	break;
//	}
//	return INF_MSGPROC_NORMAL;
//}
int	CINFGameMainChat::WndProcMacro(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (FALSE == m_bShowMacro)
	{
		return INF_MSGPROC_NORMAL;
	}
	int i;
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);

		if (m_bMacroMove)
		{
			m_nMacroX += (pt.x - m_nMacroOldX);
			m_nMacroY += (pt.y - m_nMacroOldY);
			if (m_nMacroX < 0)
				m_nMacroX = 0;
			else if (m_nMacroX > g_pD3dApp->GetBackBufferDesc().Width - CHAT_MACRO_W)
				m_nMacroX = g_pD3dApp->GetBackBufferDesc().Width - CHAT_MACRO_W;
			if (m_nMacroY < 0)
				m_nMacroY = 0;
			else if (m_nMacroY > g_pD3dApp->GetBackBufferDesc().Height - CHAT_MACRO_H)
				m_nMacroY = g_pD3dApp->GetBackBufferDesc().Height - CHAT_MACRO_H;
			m_nMacroOldX = pt.x;
			m_nMacroOldY = pt.y;
		}

		for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
		{
			m_pBtnChatMacro[i]->OnMouseMove(pt);
		}
		m_pBtnMacroOK->OnMouseMove(pt);
		m_pBtnMacroCancel->OnMouseMove(pt);

	}
	break;
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);
		if (pt.x > m_nMacroX &&
			pt.x < m_nMacroX + m_pChatMacroBG->GetImgSize().x &&
			pt.y > m_nMacroY &&
			pt.y < m_nMacroY + m_pChatMacroBG->GetImgSize().y)
		{
			if (pt.x > m_nMacroX &&
				pt.x < m_nMacroX + CHAT_MACRO_BAR_W &&
				pt.y > m_nMacroY &&
				pt.y < m_nMacroY + CHAT_MACRO_BAR_H)
			{
				m_bMacroMove = TRUE;
				m_nMacroOldX = pt.x;
				m_nMacroOldY = pt.y;
			}

			for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
			{
				m_pBtnChatMacro[i]->OnLButtonDown(pt);
			}
			m_pBtnMacroOK->OnLButtonDown(pt);
			m_pBtnMacroCancel->OnLButtonDown(pt);
			g_pInterface->SetWindowOrder(WNDMacroWnd);
			if (pt.x > m_nMacroX &&
				pt.x < m_nMacroX + CHAT_MACRO_W &&
				pt.y > m_nMacroY &&
				pt.y < m_nMacroY + CHAT_MACRO_H)
			{
				return INF_MSGPROC_BREAK;
			}

		}

	}
	break;
	case WM_LBUTTONUP:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);
		m_nActMacro = -1;
		m_bMacroMove = FALSE;
		if (pt.x > m_nMacroX &&
			pt.x < m_nMacroX + CHAT_MACRO_BAR_W &&
			pt.y > m_nMacroY &&
			pt.y < m_nMacroY + CHAT_MACRO_BAR_H)
		{
			m_bMacroMove = FALSE;
			m_nMacroOldX = 0;
			m_nMacroOldX = 0;
		}

		for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
		{
			if (m_pBtnChatMacro[i]->OnLButtonUp(pt))
			{
				if (strlen(m_strTempMacro) > 0)
				{
					m_pBtnChatMacro[i]->ChangeBoth();
					if (m_pBtnChatMacro[i]->GetBoth() == BUTTON_BOTH_FRONT)
					{
						m_sMacroStr[i].bAppl = FALSE;
					}
					else
					{
						m_sMacroStr[i].bAppl = TRUE;
						memcpy(m_sMacroStr[i].strMacro, m_strTempMacro, SIZE_MAX_CHAT_MESSAGE);
						m_sMacroStr[i].fTime = CHAT_MACRO_OUTPUT_TIME;
					}
				}
				return INF_MSGPROC_BREAK;
			}
			else if (pt.x > CHAT_MACRO_INPUT_1_X &&
				pt.x < CHAT_MACRO_INPUT_1_X + CHAT_MACRO_INPUT_W &&
				pt.y > CHAT_MACRO_INPUT_1_Y + (CHAT_MACRO_APPL_GAP_Y * i) &&
				pt.y < CHAT_MACRO_INPUT_1_Y + CHAT_MACRO_INPUT_H + (CHAT_MACRO_APPL_GAP_Y * i))
			{// 입력창 클릭시 
				m_nActMacro = i;
				memset(m_strTempMacro, 0x00, SIZE_MAX_CHAT_MESSAGE);
				//memset(g_pD3dApp->m_inputkey.m_full_str,0x00,SIZE_MAX_CHAT_MESSAGE);
				g_pD3dApp->CleanText();

				// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
				InitChatMsgBuff();
				// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

				return INF_MSGPROC_BREAK;
			}

		}
		if (m_pBtnMacroOK->OnLButtonUp(pt))
		{
			StartMacro();
			m_bShowMacro = FALSE;
			g_pD3dApp->m_bChatMode = FALSE;
			return INF_MSGPROC_BREAK;
		}
		if (m_pBtnMacroCancel->OnLButtonUp(pt))
		{
			EndMacro();
			m_bShowMacro = FALSE;
			g_pD3dApp->m_bChatMode = FALSE;
			return INF_MSGPROC_BREAK;
		}


	}
	break;
	case WM_KEYDOWN:
	{
		if (m_nActMacro >= 0)
		{
			g_pD3dApp->m_bChatMode = TRUE;

			if (wParam == VK_ESCAPE)
			{
				if (m_nActMacro >= 0)
				{
					g_pD3dApp->m_bChatMode = FALSE;
					m_bChatMode = FALSE;
					g_pD3dApp->CleanText();

					// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					InitChatMsgBuff();

					memset(m_strTempMacro, 0x00, SIZE_MAX_CHAT_MESSAGE);


					return INF_MSGPROC_BREAK;
				}
				return INF_MSGPROC_NORMAL;
			}

			return INF_MSGPROC_NORMAL;
		}
	}
	break;
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:
	case WM_CHAR:
	{
		if (m_nActMacro >= 0)
		{
			int bFlag = FALSE;

			if (INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
				bFlag = TRUE;
			//				if(g_pD3dApp->m_inputkey.m_str_pos > 57-SIZE_MAX_CHARACTER_NAME)
			if (g_pD3dApp->m_inputkey.m_str_pos > SIZE_MAX_CHAT_MESSAGE - SIZE_MAX_CHARACTER_NAME)
			{
				g_pD3dApp->m_inputkey.m_str_pos = SIZE_MAX_CHAT_MESSAGE - SIZE_MAX_CHARACTER_NAME + 1;
				//					g_pD3dApp->m_inputkey.m_str_pos = 58-SIZE_MAX_CHARACTER_NAME;
				g_pD3dApp->m_inputkey.m_full_str[g_pD3dApp->m_inputkey.m_str_pos] = NULL;
			}
			if (strlen(g_pD3dApp->m_inputkey.m_full_str) != 0)
			{
				memset(m_strTempMacro, 0x00, SIZE_MAX_CHAT_MESSAGE);
				if (wParam == 34 && g_pD3dApp->m_pShuttleChild->m_bOldPToPPos != 200 && g_pD3dApp->m_inputkey.m_str_pos == 1)
				{
					strncpy(g_pD3dApp->m_inputkey.m_full_str + 1,
						g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos],
						strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]));
					g_pD3dApp->m_inputkey.m_full_str[strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]) + 1] = ' ';
					g_pD3dApp->m_inputkey.m_str_pos += strlen(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]) + 1;
				}
				// 2007-11-05 by dgwoo SIZE_MAX_CHAT_MESSAGE사이즈가 넘어가면 백스페이스 이벤트 발생.
				strncpy(m_strTempMacro, g_pD3dApp->m_inputkey.m_full_str, strlen(g_pD3dApp->m_inputkey.m_full_str));
				if (strlen(g_pD3dApp->m_inputkey.m_full_str) >= SIZE_MAX_CHAT_MESSAGE)
				{
					keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
				}

			}
			else
			{
				memset(m_strTempMacro, 0x00, SIZE_MAX_CHAT_MESSAGE);
			}
			if (TRUE == bFlag)
				return INF_MSGPROC_BREAK;
		}
	}
	break;
	}
	return INF_MSGPROC_NORMAL;
}
int CINFGameMainChat::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEWHEEL:
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
		CheckMouseReverse(&pt);

		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
		//if(g_pInterface->m_bShowInterface && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
		if (g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode() && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
		{
			if (m_bShowChatBox &&
				pt.x > CHAT_CHATBOX_START_X &&
				pt.x < CHAT_CHATBOX_START_X + m_nChatBoxWidth &&
				pt.y > m_nChatBoxYPos &&
				pt.y < m_nChatBoxYPos + m_nChatBoxHeight)
			{
				ChatTab_t *pChatTab = GetChatTabMode();
				if ((int)wParam > 0)
				{
					pChatTab->DecreaseRenderStartIndex();
					m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP +
						pChatTab->GetScrollPositionWithRenderStartIndex(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
							, (m_nChatBoxHeight - CHATBOX_IMAGE_GAB_HEITHT_TOP) / CHAT_FONT_LINE_HEIGHT);

					// 2008. 12. 29 by ckPark 채팅창 최적화	
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화	

					return INF_MSGPROC_BREAK;
				}
				else
				{
					pChatTab->IncreaseRenderStartIndex();
					m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP +
						pChatTab->GetScrollPositionWithRenderStartIndex(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
							, (m_nChatBoxHeight - CHATBOX_IMAGE_GAB_HEITHT_TOP) / CHAT_FONT_LINE_HEIGHT);

					// 2008. 12. 29 by ckPark 채팅창 최적화	
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화	

					return INF_MSGPROC_BREAK;
				}
			}
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		CheckMouseReverse(&pt);

		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
		//if(g_pInterface->m_bShowInterface)
		if (g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode())
		{
			if (pt.x > WISPERBOX_MINIMUM_WINDOW_BUTTON_X &&
				pt.x < WISPERBOX_MINIMUM_WINDOW_BUTTON_X + WISPERBOX_MINIMUM_WINDOW_WIDTH &&
				pt.y > WISPERBOX_MINIMUM_WINDOW_BUTTON_Y &&
				pt.y < WISPERBOX_MINIMUM_WINDOW_BUTTON_Y + WISPERBOX_MINIMUM_WINDOW_HEIGHT)
			{
				m_bShowMiniWisperUnder = TRUE;
			}
			else
			{
				m_bShowMiniWisperUnder = FALSE;
			}
			// 메뉴 선택 유료 아이템
			if (g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE &&
				g_pD3dApp->m_dwGameState != _SHOP
				&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_GAMING)	// 2008-02-27 by bhsohn 통합 아레나 수정)
			{
				if (pt.x > CHAT_MENU_CASH_START_X - 3 &&
					pt.x < CHAT_MENU_CASH_START_X + CHAT_MENU_CASH_START_W - 3 &&
					pt.y > CHAT_MENU_CASH_START_Y - m_nCashButtonGab &&
					pt.y < CHAT_MENU_CASH_START_Y - m_nCashButtonGab + CHAT_MENU_CASH_START_H)
				{
					if (m_bCashButton)
						m_nOtherCashList = CHAT_CASH_BUTTON_STATE_PUH;
					else
						m_nOtherCashList = CHAT_CASH_BUTTON_STATE_OVE;
				}
				else
				{
					m_nOtherCashList = CHAT_CASH_BUTTON_STATE_NOM;
				}
			}
			m_pBtnOption->OnMouseMove(pt);
			if (!m_bMenuListUse)
			{
				BOOL bMouse = FALSE;
				for (int i = 1; i <= CHAT_TAB_CHATROOM; i++)
				{
					if (m_pBtnChatMode[i - 1]->OnMouseMove(pt))
					{
						char strTemp[32];
						memset(strTemp, 0x00, 32);
						bMouse = TRUE;
						switch (i)
						{
						case CHAT_TAB_ALLUSER:
							memcpy(strTemp, STRMSG_C_080519_0100, 32);
							break;
						case CHAT_TAB_NORMAL:
							memcpy(strTemp, STRMSG_C_080519_0101, 32);
							break;
						case CHAT_TAB_WAR:
							memcpy(strTemp, STRMSG_C_080519_0102, 32);
							break;
						case CHAT_TAB_TRADE:
							memcpy(strTemp, STRMSG_C_080519_0103, 32);
							break;
						case CHAT_TAB_MAP:
							memcpy(strTemp, STRMSG_C_080519_0104, 32);
							break;
						case CHAT_TAB_GUILD:
							memcpy(strTemp, STRMSG_C_080519_0105, 32);
							break;
						case CHAT_TAB_PARTY:
							memcpy(strTemp, STRMSG_C_080519_0106, 32);
							break;
						case CHAT_TAB_ARENA:
							memcpy(strTemp, STRMSG_C_080519_0107, 32);
							break;
						case CHAT_TAB_CHATROOM:
							memcpy(strTemp, STRMSG_C_080519_0108, 32);
							break;
						}
						g_pGameMain->SetToolTip(pt.x, pt.y, strTemp);

					}
				}
				if (bMouse)
				{
					return INF_MSGPROC_BREAK;
				}
				else
				{
					g_pGameMain->SetToolTip(0, 0, NULL);
				}
			}



			// 메뉴 선택(FAQ, 유료 아이템)
			m_nVoiceMenu = 0;
			//				if( m_bMenuListUse &&
			//					g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
			//				{
			//					for(int i=CHAT_TAB_NUMBER; i<CHAT_TAB_NUMBER+CHAT_OTHER_MENU_ALL; i++)
			//					{
			//						if( pt.x > CHAT_MENU_LIST_START_X &&
			//							pt.x < CHAT_MENU_LIST_START_X+CHAT_MENU_LIST_W && 
			//							pt.y > CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)) && 
			//							pt.y < CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1))+CHAT_MENU_LIST_H )
			//						{
			//							m_nOtherMenuList[i-CHAT_TAB_NUMBER] = CHAT_NMENU_BUTTON_STATE_OVE;
			//
			//							switch(i) 
			//							{
			//							case CHAT_OTHER_MENU_VOIP:
			//								{
			//									m_nVoiceMenu = 1;
			//								}
			//								break;
			//							}
			//						}
			//						else
			//						{
			//							m_nOtherMenuList[i-CHAT_TAB_NUMBER] = CHAT_NMENU_BUTTON_STATE_NOM;
			//						}
			//						
			//					}
			//				}

							// 최소화 부분 선택 05.01.21
			if (pt.x > CHAT_CHATTAB_VIEW_FLAG_X &&
				pt.x < CHAT_CHATTAB_VIEW_FLAG_X + CHAT_CHATTAB_VIEW_FLAG_W &&
				pt.y > CHAT_CHATTAB_VIEW_FLAG_Y && pt.y < CHAT_CHATTAB_VIEW_FLAG_Y + CHAT_CHATTAB_VIEW_FLAG_H &&
				g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
			{
				m_nChatSizeBtnNormal = BUTTON_BOTH_STATE_UP;
			}
			else
			{
				m_nChatSizeBtnNormal = BUTTON_BOTH_STATE_NORMAL;
			}

			// 채팅 메뉴리스트 선택 05.01.21
			if (pt.x > CHAT_MENU_VIEW_X &&
				pt.x < CHAT_MENU_VIEW_X + CHAT_MENU_VIEW_W &&
				pt.y > CHAT_MENU_VIEW_Y && pt.y < CHAT_MENU_VIEW_Y + CHAT_MENU_VIEW_H &&
				g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
			{
				if (m_bMenuList)
				{
					m_nMenuListButtonState = CHAT_NMENU_BUTTON_STATE_PUH;
				}
				else if (m_nMenuListButtonState == CHAT_NMENU_BUTTON_STATE_NOM)
				{
					m_nMenuListButtonState = CHAT_NMENU_BUTTON_STATE_OVE;
				}
			}
			else
			{
				m_nMenuListButtonState = CHAT_NMENU_BUTTON_STATE_NOM;
			}

			// 최대화 부분 선택	
			if (pt.x > CHAT_CHATBOX_START_X + m_nChatBoxWidth - CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH &&
				pt.x < CHAT_CHATBOX_START_X + m_nChatBoxWidth - CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH + CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH &&
				pt.y > m_nChatBoxYPos + CHAT_MENUBOX_GAB_WIDTH &&
				pt.y < m_nChatBoxYPos + CHAT_MENUBOX_GAB_WIDTH + CHATBOX_IMAGE_GAB_MINIMIZE_HEIGHT &&
				g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
			{
				if (m_bWindowSizeMax)
				{
					//						m_nSizeMaxButtonstate = CHAT_BUTTON_DOWN;
				}
				else
				{
					//						m_nSizeMaxButtonstate = CHAT_BUTTON_UP;
				}
			}
			else
			{
				//					m_nSizeMaxButtonstate = CHAT_BUTTON_NORMAL;				
			}

			// 채팅 화면 스크롤
			if (m_bScrollLock)
			{
				int nPointGap = pt.y - m_ptMouse.y;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if (m_nCurrentRelScrollOldPosY + nPointGap < CHATBOX_IMAGE_GAB_HEITHT_TOP + 5)
				{
					m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP + 5;
				}
#else			  
				if (m_nCurrentRelScrollOldPosY + nPointGap < CHATBOX_IMAGE_GAB_HEITHT_TOP)
				{
					m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP;
				}

#endif
				else if (m_nCurrentRelScrollOldPosY + nPointGap > m_nChatBoxHeight - CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
				{
					m_nCurrentRelScrollPosY = m_nChatBoxHeight - CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT;
				}
				else
				{
					m_nCurrentRelScrollPosY = m_nCurrentRelScrollOldPosY + nPointGap;
				}
				ChatTab_t *pChatTab = GetChatTabMode();
				pChatTab->SetRenderStartIndexWithScrollPositon(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
					, m_nCurrentRelScrollPosY - CHATBOX_IMAGE_GAB_HEITHT_TOP);


				// 2008. 12. 29 by ckPark 채팅창 최적화	
				m_listCulledString.clear();
				// end 2008. 12. 29 by ckPark 채팅창 최적화	
			}

			// 마우스 포인트 바꾸기
			if (m_bShowChatBox &&
				pt.x > CHAT_CHATBOX_START_X + m_nChatBoxWidth &&
				pt.x < CHAT_CHATBOX_START_X + m_nChatBoxWidth + CHATBOX_IMAGE_GAB_WIDTH_TOP &&
				pt.y > m_nChatBoxYPos &&
				pt.y < m_nChatBoxYPos + CHATBOX_IMAGE_GAB_HEITHT_TOP)
			{	// 채팅박스 Right and Bottom 사이즈 조정을 클릭

				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = TRUE;
				g_pInterface->m_pTarget->SetMouseType(LEFTBOTTOM_ARROW_IMAGE);
				return INF_MSGPROC_BREAK;
			}
			else
			{
				g_pInterface->m_pTarget->m_bWisperChatBoxMouseFlag = FALSE;
				//					g_pInterface->m_pTarget->SetMouseType(NO_ARROW_IMAGE);
			}
			if (m_bShowChatBox)
			{
				if (g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber == 0 &&
					m_nChatMode == CHAT_TAB_GUILD)
				{
					m_pBtnGuildQuick->OnMouseMove(pt);
				}
				else if (g_pShuttleChild->m_pClientParty->IsParty() == FALSE &&
					m_nChatMode == CHAT_TAB_PARTY)
				{
					m_pBtnPartyQuick->OnMouseMove(pt);
				}
			}
			if (m_bLockHandle)
			{
				m_nChatBoxWidth += pt.x - m_ptMouse.x;
				m_nChatBoxHeight += m_ptMouse.y - pt.y;
				if (m_nChatBoxWidth > CHAT_CHATBOX_START_DEFAULT_MAX_X) m_nChatBoxWidth = CHAT_CHATBOX_START_DEFAULT_MAX_X;
				if (m_nChatBoxWidth < CHAT_CHATBOX_START_DEFAULT_MIN_X) m_nChatBoxWidth = CHAT_CHATBOX_START_DEFAULT_MIN_X;
				if (m_nChatBoxHeight > CHAT_CHATBOX_START_DEFAULT_MAX_Y) m_nChatBoxHeight = CHAT_CHATBOX_START_DEFAULT_MAX_Y;
				if (m_nChatBoxHeight < CHAT_CHATBOX_START_DEFAULT_MIN_Y) m_nChatBoxHeight = CHAT_CHATBOX_START_DEFAULT_MIN_Y;
				m_ptMouse = pt;
				ChatTab_t *pChatTab = GetChatTabMode();
				m_nCurrentRelScrollPosY = CHATBOX_IMAGE_GAB_HEITHT_TOP +
					pChatTab->GetScrollPositionWithRenderStartIndex(m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
						, (m_nChatBoxHeight - CHATBOX_IMAGE_GAB_HEITHT_TOP) / CHAT_FONT_LINE_HEIGHT);

				// 2008. 12. 29 by ckPark 채팅창 최적화	
				m_listCulledString.clear();
				// end 2008. 12. 29 by ckPark 채팅창 최적화

				// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
				stcuctRateRect	stRateRect;
				memset(&stRateRect, 0x00, sizeof(stcuctRateRect));
				stRateRect.fRateWidth = m_nChatBoxWidth;
				stRateRect.fRateHeight = m_nChatBoxHeight;
				g_pD3dApp->SetChatInterfacePOS(&stRateRect);
				// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
			}

		}
		else
		{
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
		//if(g_pInterface->m_bShowInterface && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
		// 2009-02-16 by bhsohn 마우스 숨기기시, 채팅창 클릭 안되는 버그 수정
		//if(g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode() && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
		BOOL bShowMenu = TRUE;
		if (g_pInterface->m_pTarget->GetShowTargetCodition() == TRUE
			&& !g_pD3dApp->IsPossibleWindowMove())
		{
			bShowMenu = FALSE;
		}
		if (g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode()
			&& bShowMenu)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if (pt.x > WISPERBOX_MINIMUM_WINDOW_BUTTON_X &&
				pt.x < WISPERBOX_MINIMUM_WINDOW_BUTTON_X + WISPERBOX_MINIMUM_WINDOW_WIDTH &&
				pt.y > WISPERBOX_MINIMUM_WINDOW_BUTTON_Y &&
				pt.y < WISPERBOX_MINIMUM_WINDOW_BUTTON_Y + WISPERBOX_MINIMUM_WINDOW_HEIGHT)
			{
				// 2010. 03. 16 by jskim 귓말창 버그 수정
				if (m_nSmallWindowCount)
					//end 2010. 03. 16 by jskim 귓말창 버그 수정
					m_bShowMiniWisper = !m_bShowMiniWisper;
			}


			if (g_pD3dApp->m_dwGameState != _SHOP
				&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_GAMING)	// 2008-02-27 by bhsohn 통합 아레나 수정)
			{
				// 메뉴 선택 유료 아이템
				if (pt.x > CHAT_MENU_CASH_START_X &&
					pt.x < CHAT_MENU_CASH_START_X + CHAT_MENU_CASH_START_W &&
					pt.y > CHAT_MENU_CASH_START_Y - m_nCashButtonGab &&
					pt.y < CHAT_MENU_CASH_START_Y - m_nCashButtonGab + CHAT_MENU_CASH_START_H &&
					g_pShuttleChild->GetIsUseInterface())						// 2006-06-12 by ispark, 비행중일때는 막는다.
				{
					if (!g_pGameMain->m_pTrade->m_bTrading)
					{
						m_bCashButton = TRUE;
						m_nOtherCashList = CHAT_CASH_BUTTON_STATE_PUH;
					}
					return INF_MSGPROC_BREAK;
				}
				else
				{
					m_nOtherCashList = CHAT_CASH_BUTTON_STATE_NOM;
				}
			}
			m_pBtnOption->OnLButtonDown(pt);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 메뉴 선택 
//				if(pt.y>CHAT_MENUBOX_START_Y && pt.y<CHAT_MENUBOX_START_Y+CHAT_MENUBOX_WIDTH+CHAT_MENUBOX_HEITHT)
//				{
//					for(int i=0;i<CHAT_MENUBOX_SELECT_COUNT;i++)
//					{
//						if(pt.x>CHAT_MENUBOX_START_X+CHAT_MENUBOX_GAB_WIDTH+CHAT_MENUBOX_WIDTH*i 
//							&& pt.x<CHAT_MENUBOX_START_X+CHAT_MENUBOX_GAB_WIDTH+CHAT_MENUBOX_WIDTH*i+CHAT_MENUBOX_WIDTH-CHAT_MENUBOX_GAB_WIDTH)
//						{
//							//m_nCurrentTabIndex = i;
//							m_bChatCurrentSelect[m_nSelectChatTab][i] = !m_bChatCurrentSelect[m_nSelectChatTab][i];
//							if(m_nCurrentTabIndex == CHAT_TAB_SOUND) m_bLockSoundButton = TRUE;
//
//							return INF_MSGPROC_BREAK;
//						}
//					}
//				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
			if (!m_bMenuListUse)
			{
				for (int i = 0; i < CHAT_TAB_CHATROOM; i++)
				{
					m_pBtnChatMode[i]->OnLButtonDown(pt);
				}
			}

			// 채팅창 최대화 
			if (pt.x > CHAT_CHATTAB_VIEW_FLAG_X &&
				pt.x < CHAT_CHATTAB_VIEW_FLAG_X + CHAT_CHATTAB_VIEW_FLAG_W &&
				pt.y > CHAT_CHATTAB_VIEW_FLAG_Y && pt.y < CHAT_CHATTAB_VIEW_FLAG_Y + CHAT_CHATTAB_VIEW_FLAG_H)
			{
				m_nChatSizeBtnNormal = BUTTON_BOTH_STATE_UP;
				m_bWindowSizeMin = TRUE;
				m_bShowChatBox = !m_bShowChatBox;
				m_pSystemMsgW->SetInterface(m_bShowChatBox);
				SetWisperChatShowInterface(m_bShowChatBox);
				m_bMenuListUse = FALSE;
				return INF_MSGPROC_BREAK;
			}
			else
			{
				m_nChatSizeBtnNormal = BUTTON_BOTH_STATE_NORMAL;
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 입력 언어 선택
			if (pt.x > CHAT_INPUT_LANGUAGE_X && pt.x < CHAT_INPUT_LANGUAGE_X + CHAT_INPUT_LANGUAGE_WH &&
				pt.y > CHAT_INPUT_LANGUAGE_Y && pt.y < CHAT_INPUT_LANGUAGE_Y + CHAT_INPUT_LANGUAGE_WH)
			{
				m_bSelectInputLanguage = TRUE;
				return INF_MSGPROC_BREAK;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 화면 크기조절 
			if (m_bShowChatBox &&
				pt.x > CHAT_CHATBOX_START_X + m_nChatBoxWidth &&
				pt.x < CHAT_CHATBOX_START_X + m_nChatBoxWidth + CHATBOX_IMAGE_GAB_WIDTH_TOP &&
				pt.y > m_nChatBoxYPos &&
				pt.y < m_nChatBoxYPos + CHATBOX_IMAGE_GAB_HEITHT_TOP)
			{
				m_bLockHandle = TRUE;
				m_ptMouse = pt;
				return INF_MSGPROC_BREAK;
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 스크롤바 선택 
			if (m_bShowChatBox
				&& pt.x >= m_nChatBoxWidth + CHATBOX_SCROLL_IMAGE_GAP_X
				&& pt.x < m_nChatBoxWidth + CHATBOX_SCROLL_IMAGE_GAP_X + CHATBOX_SCROLL_IMAGE_SIZE_WIDTH
				&& pt.y >= m_nChatBoxYPos + m_nCurrentRelScrollPosY
				&& pt.y < m_nChatBoxYPos + m_nCurrentRelScrollPosY + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT)
			{
				m_bScrollLock = TRUE;
				m_ptMouse = pt;
				m_nCurrentRelScrollOldPosY = m_nCurrentRelScrollPosY;
				return INF_MSGPROC_BREAK;
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 모드 선택 해제
			if (pt.x > CHAT_INPUT_WINDOW_X && pt.x<CHAT_INPUT_WINDOW_X + CHAT_INPUT_WINDOW_W &&
				pt.y>CHAT_INPUT_WINDOW_Y && pt.y < CHAT_INPUT_WINDOW_Y + CHAT_INPUT_WINDOW_H)
			{
				if (m_bChatMode == FALSE)
				{
					g_pD3dApp->CleanText();

					// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
					InitChatMsgBuff();
					// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
				}
				g_pD3dApp->m_bChatMode = TRUE;
				m_bChatMode = TRUE;
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_INPUT_CHAT, g_pShuttleChild->m_vPos, FALSE);

				return INF_MSGPROC_BREAK;
			}

			////////////////////////////////////////////////////////////////////////////////////////
			// 메뉴 리스트 보이기
			if (pt.x > CHAT_MENU_VIEW_X &&
				pt.x < CHAT_MENU_VIEW_X + CHAT_MENU_VIEW_W &&
				pt.y > CHAT_MENU_VIEW_Y && pt.y < CHAT_MENU_VIEW_Y + CHAT_MENU_VIEW_H &&
				(g_pShuttleChild->m_bUnitStop == TRUE || g_pD3dApp->m_bCharacter == TRUE || g_pShuttleChild->m_bLandedMove == TRUE
					|| (IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE && g_pShuttleChild->m_bIsAir == FALSE)))	// 2013-01-16 by mspark, A기어 랜딩 모드 시 채팅 메뉴 버튼 클릭되도록 수정
			{
				m_nMenuListButtonState = CHAT_NMENU_BUTTON_STATE_PUH;
				m_bMenuList = TRUE;
				return INF_MSGPROC_BREAK;
			}
			// 메뉴 선택
			if (m_bMenuListUse)
			{
				// 음성 통신.
				if (pt.x > CHAT_MENU_LIST_START_X &&
					pt.x < CHAT_MENU_LIST_START_X + CHAT_MENU_LIST_W &&
					pt.y > CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*CHAT_MENULIST_NUMBER) &&
					pt.y < CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*CHAT_MENULIST_NUMBER) + CHAT_MENU_LIST_H)
				{
					//ShowVoiceChatControl(TRUE);
					m_bMenuListUse = FALSE;
					//						COPYDATASTRUCT	copyData;
					//						DWORD			nVolum = 0;
					//						memset(&copyData, 0x00, sizeof(COPYDATASTRUCT));
					//						AV_MSG_WM_COPYDATA voIP(_1to1Call, AV_MT_GET_SPK_VOLUME, 0);
					//						copyData.dwData = (DWORD)voIP;
					//						copyData.lpData = NULL;
					//						copyData.cbData = 0;
					//						::SendMessage(g_pD3dApp->m_VOIPState.m_hWndAtumVoIPClient, WM_COPYDATA, (WPARAM)g_pD3dApp->GetHwnd(), (LPARAM)&copyData);


											// 2008. 12. 29 by ckPark 채팅창 최적화	
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화


					return INF_MSGPROC_BREAK;
				}


				// 스피커 버튼.
				if (pt.x > CHAT_MENU_LIST_START_X &&
					pt.x < CHAT_MENU_LIST_START_X + CHAT_MENU_LIST_W &&
					pt.y > CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER - CHAT_TAB_SPEAKER - 1)) &&
					pt.y < CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER - CHAT_TAB_SPEAKER - 1)) + CHAT_MENU_LIST_H)
				{
					m_bChatCurrentSelect[CHAT_TAB_SPEAKER] = !m_bChatCurrentSelect[CHAT_TAB_SPEAKER];
					g_pInterface->SetSpeakerMode(m_bChatCurrentSelect[CHAT_TAB_SPEAKER]);
					g_pInterface->SaveOptionFile();
					if (FALSE == m_bChatCurrentSelect[CHAT_TAB_SPEAKER])
					{
						// GM명령어가 아닌것은 다 지운다.
						g_pGameMain->DeleteNotGmMsg();
					}

					// 2008. 12. 29 by ckPark 채팅창 최적화	
					m_listCulledString.clear();
					// end 2008. 12. 29 by ckPark 채팅창 최적화

					return INF_MSGPROC_BREAK;
				}

				for (int i = CHAT_TAB_NORMAL; i < CHAT_MENULIST_NUMBER; i++)
				{
					if (pt.x > CHAT_MENU_LIST_START_X &&
						pt.x < CHAT_MENU_LIST_START_X + CHAT_MENU_LIST_W &&
						//pt.y > CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)) && 
						//pt.y < CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1))+CHAT_MENU_LIST_H )
						pt.y > CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H *(CHAT_MENULIST_NUMBER - i)) &&
						pt.y < CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER - i)) + CHAT_MENU_LIST_H)
					{
						char buffer[SIZE_MAX_PACKET];
						INIT_MSG(MSG_IC_CHAT_CHANGE_CHAT_FLAG, T_IC_CHAT_CHANGE_CHAT_FLAG, pS2Msg, buffer);
						pS2Msg->bitChatType = 0xFFFF;			// 모두 켜진것으로 기본 설정함

						m_bChatCurrentSelect[i] = !m_bChatCurrentSelect[i];
						g_pSOption->m_bChatCurrentSelect[i] = m_bChatCurrentSelect[i];
						g_pInterface->SaveOptionFile();
						for (int j = 0; j < CHAT_MENULIST_NUMBER; j++)
						{
							switch (j)
							{
							case CHAT_TAB_ARENA:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_ARENA);
								}
							}
							break;

							case CHAT_TAB_PARTY:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_PARTY);
								}
							}
							break;
							case CHAT_TAB_GUILD:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_GUILD);
								}
							}
							break;
							case CHAT_TAB_MAP:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_MAP);
								}
							}
							break;
							case CHAT_TAB_TRADE:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_SELL_ALL);
								}
							}
							break;
							case CHAT_TAB_PTOP:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_PTOP);
								}
							}
							break;
							case CHAT_TAB_WAR:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_WAR);
								}
							}
							break;
							case CHAT_TAB_CHATROOM:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_CHATROOM);
								}
							}
							break;
							case CHAT_TAB_SYSTEM:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_SYSTEM);
									m_pSystemMsgW->m_bShowChatBox = FALSE;
								}
								else
								{
									m_pSystemMsgW->m_bShowChatBox = TRUE;
								}
							}
							break;
							case CHAT_TAB_HELPDESK:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_HELPDESK);
								}
							}
							break;
							case CHAT_TAB_CNC:
							{
								if (FALSE == m_bChatCurrentSelect[j])
								{
									CLEAR_CHATTYPE_BIT(pS2Msg->bitChatType, CHAT_TYPE_CNC);
								}
							}
							}
						}
						g_pIMSocket->Write(buffer, MSG_SIZE(MSG_IC_CHAT_CHANGE_CHAT_FLAG));
						return INF_MSGPROC_BREAK;
					}
				}
			}
			// 메뉴 선택(FAQ, 유료 아이템)
			int nState = FALSE;
			//				if(m_bMenuListUse)
			//				{
			//					BOOL bBreakProc = FALSE;
			//					for(int i=CHAT_TAB_NUMBER; i<CHAT_TAB_NUMBER+CHAT_OTHER_MENU_ALL; i++)
			//					{
			//						if( pt.x > CHAT_MENU_LIST_START_X &&
			//							pt.x < CHAT_MENU_LIST_START_X+CHAT_MENU_LIST_W && 
			//							pt.y > CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)) && 
			//							pt.y < CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1))+CHAT_MENU_LIST_H )
			//						{
			//							m_nOtherMenuList[i-CHAT_TAB_NUMBER] = CHAT_NMENU_BUTTON_STATE_PUH;
			//							nState		= TRUE;
			//							bBreakProc	= TRUE;
			//							switch(i) 
			//							{
			//							case CHAT_OTHER_MENU_VOIP:
			//								{
			//									m_nVoiceMenu = 2;
			//								}
			//								break;
			//							}
			//						}
			//						else
			//						{
			//							m_nOtherMenuList[i-CHAT_TAB_NUMBER] = CHAT_NMENU_BUTTON_STATE_NOM;
			//						}
			//					}
			//					if(bBreakProc)
			//					{
			//						return INF_MSGPROC_BREAK;
			//					}
			//				}
			//				if(!nState)	m_bMenuListUse = FALSE;
							//
							/////////////////////////////////////////////////////////////////////////////////////////////////				
							// 2005-09-14 by ispark
							// 창안에 마우스 클릭시 무효
			if (m_bShowChatBox)
			{
				if (g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber == 0 &&
					m_nChatMode == CHAT_TAB_GUILD)
				{
					m_pBtnGuildQuick->OnLButtonDown(pt);
				}
				else if (g_pShuttleChild->m_pClientParty->IsParty() == FALSE &&
					m_nChatMode == CHAT_TAB_PARTY)
				{
					m_pBtnPartyQuick->OnLButtonDown(pt);
				}
			}
			if (m_bShowChatBox &&
				pt.x > CHAT_CHATBOX_START_X &&
				pt.x < CHAT_CHATBOX_START_X + m_nChatBoxWidth + CHATBOX_IMAGE_GAB_WIDTH_TOP &&
				pt.y > m_nChatBoxYPos &&
				pt.y < m_nChatBoxYPos + m_nChatBoxHeight)
			{
				return INF_MSGPROC_BREAK;
			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		int i = 0;

		// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
		//if(g_pInterface->m_bShowInterface && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
		// 2009-02-16 by bhsohn 마우스 숨기기시, 채팅창 클릭 안되는 버그 수정
		//if(g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode() && g_pInterface->m_pTarget->GetShowTargetCodition() == FALSE)
		BOOL bShowMenu = TRUE;
		if (g_pInterface->m_pTarget->GetShowTargetCodition() == TRUE
			&& !g_pD3dApp->IsPossibleWindowMove())
		{
			bShowMenu = FALSE;
		}
		if (g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode()
			&& bShowMenu)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			//if (m_nCurrentTabIndex == CHAT_TAB_SOUND);

			if (g_pD3dApp->m_dwGameState != _SHOP
				&& g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_GAMING)	// 2008-02-27 by bhsohn 통합 아레나 수정)
			{
				// 메뉴 선택 유료 아이템
				if (pt.x > CHAT_MENU_CASH_START_X &&
					pt.x < CHAT_MENU_CASH_START_X + CHAT_MENU_CASH_START_W &&
					pt.y > CHAT_MENU_CASH_START_Y - m_nCashButtonGab &&
					pt.y < CHAT_MENU_CASH_START_Y - m_nCashButtonGab + CHAT_MENU_CASH_START_H)
				{
					// 2007-12-05 by bhsohn 이륙중에 캐쉬샵 못열게 함.
					BOOL bCanEnterCashShop = TRUE;
					const auto& myShuttle = g_pShuttleChild->GetMyShuttleInfo();

					if (IS_CITY_MAP_INDEX(myShuttle.MapChannelIndex.MapIndex)
						&& (FALSE == g_pD3dApp->IsMyShuttleCharacter()))
					{
						bCanEnterCashShop = FALSE;
					}
					// 2008-06-24 by bhsohn 바자상점과 캐쉬상점 동시에 뜨우기 불가능하게 함
					else if (g_pInterface->IsBazarOpen())
					{
						bCanEnterCashShop = FALSE;
					}
					// end 2008-06-24 by bhsohn 바자상점과 캐쉬상점 동시에 뜨우기 불가능하게 함

					//if(m_bCashButton)
					if (m_bCashButton && bCanEnterCashShop)
					{
						// 2007-10-18 by bhsohn 예당은 캐쉬샵 보이지 않음
						// 2009-03-03 by bhsohn 일본 캐쉬 상점 버튼 표시
//#ifndef YEDANG_RELEASE
//#if defined(YEDANG_RELEASE) 
//#else
						EnterCashShop();
						//#endif
													// end 2007-10-18 by bhsohn 예당은 캐쉬샵 보이지 않음
						m_nOtherCashList = CHAT_CASH_BUTTON_STATE_OVE;
						m_bCashButton = FALSE;
						return INF_MSGPROC_BREAK;
					}
				}
			}
			m_bCashButton = FALSE;

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 2008-05-08 by dgwoo 채팅 시스템 변경 소스 추가.
			if (m_pBtnOption->OnLButtonUp(pt))
			{
				ActiveChatOption(TRUE);
			}
			if (!m_bMenuListUse)
			{
				int nPushBtn = -1;
				for (int i = 0; i < CHAT_TAB_CHATROOM; i++)
				{
					if (m_pBtnChatMode[i]->OnLButtonUp(pt))
					{// 채팅 state 변경.
						nPushBtn = i;

					}
				}
				if (nPushBtn != -1)
				{
					for (i = 0; i < CHAT_TAB_CHATROOM; i++)
					{
						if (nPushBtn == i)
						{
							ChangeChatMode(i);
							m_pBtnChatMode[i]->PushButton(TRUE);
						}
						else
						{
							m_pBtnChatMode[i]->PushButton(FALSE);
						}
					}
				}
				else
				{
					m_pBtnChatMode[m_nChatMode - 1]->PushButton(TRUE);
				}

				// 2008. 12. 29 by ckPark 채팅창 최적화	
				m_listCulledString.clear();
				// end 2008. 12. 29 by ckPark 채팅창 최적화
			}





			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 채팅 입력 언어 선택
			if (pt.x > CHAT_INPUT_LANGUAGE_X && pt.x < CHAT_INPUT_LANGUAGE_X + CHAT_INPUT_LANGUAGE_WH &&
				pt.y > CHAT_INPUT_LANGUAGE_Y && pt.y < CHAT_INPUT_LANGUAGE_Y + CHAT_INPUT_LANGUAGE_WH &&
				m_bSelectInputLanguage)
			{
				m_bSelectInputLanguage = FALSE;
				g_pD3dApp->m_inputkey.ChangeInputLanguage(g_pD3dApp->GetHwnd());
				return INF_MSGPROC_BREAK;
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// 최대화 최소화 버튼 상태
			if (pt.x > CHAT_CHATBOX_START_X + m_nChatBoxWidth - CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH &&
				pt.x < CHAT_CHATBOX_START_X + m_nChatBoxWidth - CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH + CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH &&
				pt.y > m_nChatBoxYPos + CHAT_MENUBOX_GAB_WIDTH &&
				pt.y < m_nChatBoxYPos + CHAT_MENUBOX_GAB_WIDTH + CHATBOX_IMAGE_GAB_MINIMIZE_HEIGHT)
			{
				//					m_nSizeMaxButtonstate = CHAT_BUTTON_UP;
				m_bWindowSizeMax = FALSE;
			}
			else
			{
				m_bWindowSizeMax = FALSE;
				//					m_nSizeMaxButtonstate = CHAT_BUTTON_NORMAL;
			}

			// 메뉴 리스트 보이기
			if (pt.x > CHAT_MENU_VIEW_X &&
				pt.x < CHAT_MENU_VIEW_X + CHAT_MENU_VIEW_W &&
				pt.y > CHAT_MENU_VIEW_Y && pt.y < CHAT_MENU_VIEW_Y + CHAT_MENU_VIEW_H &&
				(g_pShuttleChild->m_bUnitStop == TRUE || g_pD3dApp->m_bCharacter == TRUE || g_pShuttleChild->m_bLandedMove == TRUE
					|| (IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE && g_pShuttleChild->m_bIsAir == FALSE)))	// 2013-01-16 by mspark, A기어 랜딩 모드 시 채팅 메뉴 버튼 클릭되도록 수정
			{
				if (m_bMenuList)
				{
					g_pInterface->SetWindowOrder(WNDMainChatStartMenu);
					m_bMenuListUse = !m_bMenuListUse;
				}
				m_nMenuListButtonState = CHAT_NMENU_BUTTON_STATE_OVE;
			}
			m_bMenuList = FALSE;


			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (m_bShowChatBox)
			{
				if (g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber == 0 &&
					m_nChatMode == CHAT_TAB_GUILD)
				{
					if (m_pBtnGuildQuick->OnLButtonUp(pt))
					{
						if (g_pShuttleChild->GetIsUseInterface())
						{
							g_pGameMain->m_pCommunity->ShowCommunityWindow(TRUE, COMMUNITY_GUILD);
						}
						else
						{
						}

					}
				}
				else if (g_pShuttleChild->m_pClientParty->IsParty() == FALSE &&
					m_nChatMode == CHAT_TAB_PARTY)
				{
					if (m_pBtnPartyQuick->OnLButtonUp(pt))
					{
						if (g_pShuttleChild->GetIsUseInterface())
						{
							g_pGameMain->m_pCommunity->ShowCommunityWindow(TRUE);
						}
						else
						{
						}

					}
				}
			}

			if (m_bLockHandle)
			{
				m_bLockHandle = FALSE;
			}
			m_bScrollLock = FALSE;
		}
	}
	break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
		{
			if (m_bChatMode)
			{
				g_pD3dApp->m_bChatMode = FALSE;
				m_bChatMode = FALSE;
				g_pD3dApp->CleanText();

				memset(m_strInputMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);

				// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
				InitChatMsgBuff();

				return INF_MSGPROC_BREAK;
			}
			return INF_MSGPROC_NORMAL;
		}

		if (lParam == WM_KEYDOWN_LPARAM_T)
		{
			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			//if(g_pInterface->m_bShowInterface && m_bChatMode == FALSE && g_pD3dApp->m_bChatMode == FALSE)
			if (g_pInterface->m_bShowInterface && !g_pShuttleChild->IsObserverMode() && m_bChatMode == FALSE && g_pD3dApp->m_bChatMode == FALSE)
			{
				if (m_bShowChatBox == FALSE)
				{
					//						m_nSizeMinButtonstate = CHAT_NMENU_BUTTON_STATE_PUH;
					m_bWindowSizeMin = TRUE;
					m_bShowChatBox = TRUE;
					m_pSystemMsgW->SetInterface(TRUE);
					SetWisperChatShowInterface(TRUE);
					return INF_MSGPROC_BREAK;
				}
				else
				{
					//						m_nSizeMaxButtonstate = CHAT_BUTTON_DOWN;
					m_pSystemMsgW->SetInterface(FALSE);
					SetWisperChatShowInterface(FALSE);
					m_bWindowSizeMax = TRUE;
					m_bShowChatBox = FALSE;
					m_nCurrentRelScrollPosY = m_nChatBoxHeight - CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT;
					ChatTab_t *pChatTab = GetChatTabMode();
					pChatTab->SetRenderStartIndexWithScrollPositon(
						m_nChatBoxHeight - (CHATBOX_IMAGE_GAB_HEITHT_TOP + CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT),
						m_nCurrentRelScrollPosY - CHATBOX_IMAGE_GAB_HEITHT_TOP);
					return INF_MSGPROC_BREAK;
				}
			}

		}
		if (wParam == VK_RETURN)
		{
			// 2008-07-01 by dgwoo Enter키로 1형무기 무한발사 막음.
			g_pShuttleChild->m_bLButtonState = FALSE;
			g_pShuttleChild->m_bRButtonState = FALSE;
			m_bChatMode = !m_bChatMode;
			g_pD3dApp->m_bChatMode = m_bChatMode;

			// 2005-04-26 by jschoi - 채팅 타입 저장
			auto strChatType = '\0';

			// 2007-02-23 by bhsohn 공개 채팅창 버그수정
			bool bBlankMsg = false;
			// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

			char strLastColorChar = NULL;
			char lastextcolor[6] { };

			if (_REMEMBER_CHAT_COLOR)
			{
				if (m_strInputMessage[0] != '\0' && m_strInputMessage[1] != '\0')
				{
					if (m_strInputMessage[0] == '\\')
					{
						if (CheckFontColor(m_strInputMessage[1])) strLastColorChar = m_strInputMessage[1];
						
						else if (m_strInputMessage[1] == '#' &&
							strlen(m_strInputMessage) >= 9 &&
							m_strInputMessage[8] == '#')
						{
							strLastColorChar = '#';
							memcpy(lastextcolor, m_strInputMessage + 2, 6);
						}
					}
					else if (m_strInputMessage[1] == '\\' && m_strInputMessage[2] != '\0')
					{
						if (CheckFontColor(m_strInputMessage[2])) strLastColorChar = m_strInputMessage[2];
						
						else if (m_strInputMessage[2] == '#' &&
							strlen(m_strInputMessage) >= 10 &&
							m_strInputMessage[9] == '#')
						{
							strLastColorChar = '#';
							memcpy(lastextcolor, m_strInputMessage + 3, 6);
						}
					}
					else
					{
						strLastColorChar = NULL;
					}
				}
			}

			if (!m_bChatMode)
			{
				bool bSendSpecialChat = false;

				if (m_strInputMessage[0] == CHAT_ALL)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 공지 채팅
					if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
					{
						if (strlen(m_strInputMessage) > 1)
						{
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_ALL, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
							bSendSpecialChat = TRUE;
						}
						// 2007-02-23 by bhsohn 공개 채팅창 버그수정
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = TRUE;
						}
						// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

						// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						//strChatType = '~';
						strChatType = CHAT_ALL;
						// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
					}
				}

				else if (m_strInputMessage[0] == CHAT_GUILD)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 길드 채팅
					// 2008-07-21 by bhsohn 아레나에서 여단  안되는 문제 해결
					//if(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0)
					//CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
					// 2008-10-20 by bhsohn VOIP 버그 수정
					//if(pMainInfo && pMainInfo->GuildUniqueNumber != 0)
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						UID32_t	uGuildUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber;
						if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
						{
							CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
							if (pMainInfo)
							{
								uGuildUniqueNumber = pMainInfo->GuildUniqueNumber;
							}
						}
						if (uGuildUniqueNumber != 0)
						{
							if (strlen(m_strInputMessage) > 1)
							{
								g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_GUILD, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
								bSendSpecialChat = TRUE;
							}
							// 2007-02-23 by bhsohn 공개 채팅창 버그수정
							else if (strlen(m_strInputMessage) == 1)
							{
								bBlankMsg = TRUE;
							}
							// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

							// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
							//strChatType = '@'; 
							strChatType = CHAT_GUILD;
							// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						}
#ifdef FREEWAR_
					}
#endif
				}

				else if (m_strInputMessage[0] == CHAT_ARENA)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 아레나 채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (strlen(m_strInputMessage) > 1)
						{
							g_pIMSocket->SendChat(T_IC_CHAT_ARENA, g_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
							bSendSpecialChat = TRUE;
						}
						// 2007-02-23 by bhsohn 공개 채팅창 버그수정
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = TRUE;
						}
						// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

						// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						//strChatType = '%';
						strChatType = CHAT_ARENA;
						// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
					}
#ifdef FREEWAR_
				}
#endif


				else if (m_strInputMessage[0] == CHAT_WAR)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 전쟁 채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (strlen(m_strInputMessage) > 1)
						{
							g_pIMSocket->SendChat(T_IC_CHAT_WAR, g_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
							bSendSpecialChat = true;
						}
						// 2007-02-23 by bhsohn 공개 채팅창 버그수정
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = true;
						}
						// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

						// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						//strChatType = '^';
						strChatType = CHAT_WAR;
						// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
					}
#ifdef FREEWAR_
				}
#endif
#ifdef _CROSS_NATION_CHAT
				else if (m_strInputMessage[0] == CHAT_CNC)
				{
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (strlen(m_strInputMessage) > 1)
						{
							// Send Chat
							g_pIMSocket->SendChat(T_IC_CHAT_CNC, g_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);	// Send Message to IM Socket
							bSendSpecialChat = TRUE;
						}
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = TRUE;
						}
						// Set Chatting Type
						strChatType = CHAT_CNC;
					}
#ifdef FREEWAR_
				}
#endif
#endif
				else if (m_strInputMessage[0] == CHAT_PARTY)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 편대 채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY)
						{
							if (strlen(m_strInputMessage) > 1)
							{
								g_pIMSocket->SendChat(T_IC_CHAT_PARTY, g_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
								bSendSpecialChat = TRUE;
							}
							// 2007-02-23 by bhsohn 공개 채팅창 버그수정
							else if (strlen(m_strInputMessage) == 1)
							{
								bBlankMsg = TRUE;
							}
							// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

							// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
							//strChatType = '#';
							strChatType = CHAT_PARTY;
							// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						}
#ifdef FREEWAR_
					}
#endif
				}

				else if (m_strInputMessage[0] == CHAT_MAP)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 맵전체 채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (strlen(m_strInputMessage) > 1)
						{
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_MAP, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
							bSendSpecialChat = TRUE;
						}
						// 2007-02-23 by bhsohn 공개 채팅창 버그수정
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = TRUE;
						}
						// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

						// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						//strChatType = '!';
						strChatType = CHAT_MAP;
						// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
#ifdef FREEWAR_
					}
#endif
				}

				else if (m_strInputMessage[0] == CHAT_SELL_ALL)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 매매 채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (strlen(m_strInputMessage) > 1)
						{
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_SELL_ALL, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
							bSendSpecialChat = TRUE;
						}
						// 2007-02-23 by bhsohn 공개 채팅창 버그수정
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = TRUE;
						}
						// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

						// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						//strChatType = '$';
						strChatType = CHAT_SELL_ALL;
						// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
#ifdef FREEWAR_
					}
#endif
				}

				else if (m_strInputMessage[0] == CHAT_CHATROOM)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 유료 맵전체 채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						if (strlen(m_strInputMessage) > 1)
						{
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_CHATROOM, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
							bSendSpecialChat = TRUE;
						}
						// 2007-02-23 by bhsohn 공개 채팅창 버그수정
						else if (strlen(m_strInputMessage) == 1)
						{
							bBlankMsg = TRUE;
						}
						// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

						// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						//strChatType = '&';
						strChatType = CHAT_CHATROOM;
						// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
#ifdef FREEWAR_
					}
#endif
				}

				else if (m_strInputMessage[0] == CHAT_PTOP)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 1:1 귓속말 @채팅
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
						char name[SIZE_MAX_CHARACTER_NAME];
						memset(name, 0x00, SIZE_MAX_CHARACTER_NAME);
						int n = 1;
						bool bIsPToP = false;
						while (1)
						{
							if (m_strInputMessage[n] == ' ' || m_strInputMessage[n] == NULL)
							{
								bIsPToP = TRUE;
								break;
							}

							// 2007.04.24 by bhsohn China IME Working
							//#ifdef LANGUAGE_CHINA
							//							if(m_strInputMessage[n] == -95)
							//							{
							//								bIsPToP = TRUE;
							//								break;
							//							}
							//#endif
							n++;
							if (n == SIZE_MAX_CHARACTER_NAME)
								break;
						}
						if (bIsPToP && n != 1)
						{
							// 2006-09-28 by ispark, 내용이 없다면 메세지 보내지 말것
							if (strlen(m_strInputMessage + 1 + n) == 0)
							{
								break;
							}

							// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{
								// 아레나 게임중에는 귓속말 제한
								break;
							}

							memcpy(name, m_strInputMessage + 1, (n - 1));

							// 2008-07-14 by bhsohn EP3 관련 처리
							if (g_pGameMain->IsRejectUser(name))
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207, COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
								break;
							}
							// 2008-06-20 by bhsohn EP3 옵션관련 처리
							if (!g_pD3dApp->IsOptionEtc(OPTION_RADIO_WHISPER))
							{
								// 귓속말 사용안함.
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080716_0203, COLOR_ERROR);//"귓속말 거부 상태 입니다."
								break;
							}
							// end 2008-06-20 by bhsohn EP3 옵션관련 처리
							// end 2008-07-14 by bhsohn EP3 관련 처리
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_PTOP, name, m_strInputMessage + 1 + n);
							bSendSpecialChat = TRUE;
							bool bCheckChat = false;
							for (int i = 0; i < MAX_CHAT_PTOP_CONT; i++)
							{
								if (!_stricmp(g_pD3dApp->m_pShuttleChild->m_strChatPToP[i], name))
								{
									bCheckChat = true;
									g_pD3dApp->m_pShuttleChild->m_bOldPToPPos = i;
									break;
								}
							}
							if (!bCheckChat)
							{
								util::zero(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bCurPToPPos]);
								
								strcpy_s(g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bCurPToPPos], name);
								
								g_pD3dApp->m_pShuttleChild->m_bOldPToPPos = g_pD3dApp->m_pShuttleChild->m_bCurPToPPos;
								
								++g_pD3dApp->m_pShuttleChild->m_bCurPToPPos %= MAX_CHAT_PTOP_CONT;
							}
						}
#ifdef FREEWAR_
					}
#endif
				}

				else if (m_strInputMessage[0] == CHAT_INFLUENCE_ALL)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 공지 채팅
#ifdef FREEWAR_
					// restrict chat in free for all maps
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) ||
						COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
#endif
						if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER | RACE_INFLUENCE_SUBLEADER_MASK)
							|| g_pGameMain->m_pCommunity->GetGuild()->GetMyGuildRank() == GUILD_RANK_COMMANDER)
						{
							if (strlen(m_strInputMessage) > 1)
							{
								g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_INFLUENCE_ALL, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage + 1);
								bSendSpecialChat = TRUE;
							}
							// 2007-02-23 by bhsohn 공개 채팅창 버그수정
							else if (strlen(m_strInputMessage) == 1)
							{
								bBlankMsg = TRUE;
							}
							// end 2007-02-23 by bhsohn 공개 채팅창 버그수정

							// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
							//strChatType = '*';
							strChatType = CHAT_INFLUENCE_ALL;
							// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
						}

				}

				else if (m_strInputMessage[0] == CHAT_COMMAND)
					// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
				{// 기타 게임의 명령어
					// 옵저버 모드시에는 모든 명령어를 막는다.
					if (g_pShuttleChild->IsObserverMode())
					{
						break;
					}
					bool bSendChat = FALSE;
					char chatOrder[256];
					memset(chatOrder, 0x00, 256);
					char name[SIZE_MAX_CHARACTER_NAME];
					memset(name, 0x00, SIZE_MAX_CHARACTER_NAME);
					int n = 1;
					if (strlen(m_strInputMessage) > 1)
					{
						while (true)
						{
							if (m_strInputMessage[n] == ' ')		// || m_strInputMessage[n] == '\n')
								break;

							n++;
							if (n == strlen(m_strInputMessage))	//SIZE_MAX_CHAT_MESSAGE)
								break;
						}
					}
					// 						memcpy(chatOrder,m_strInputMessage+1,n-1);
					memcpy(chatOrder, m_strInputMessage, n);

					int re;

					if (!bSendChat)
					{
						re = stricmp(chatOrder, STRCMD_CS_COMMAND_COMEON);	//"/오기"
						if (re == 0)
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
								MSG_IC_ADMIN_CALL_CHARACTER sMsg;
								memset(&sMsg, 0x00, sizeof(sMsg));
								char buffer[SIZE_MAX_PACKET];
								strcpy(sMsg.CharacterName, name);
								sMsg.MapChannelIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex;
								sMsg.Position = g_pD3dApp->m_pShuttleChild->m_vPos;
								int nType = T_IC_ADMIN_CALL_CHARACTER;
								memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
								memcpy(buffer + SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
								g_pD3dApp->m_pIMSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
								bSendChat = TRUE;
							}
						}
					}
					if (!bSendChat)
					{
						re = stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE);//"/이동"
						if (re == 0)
						{
							if (g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER &&
								COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								// 워프를 요청
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
								g_pShuttleChild->m_pClientParty->FSendPartyRequestPartyWarpWithMapName(name);
								//									MSG_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME sMsg;
								//									memset(&sMsg,0x00,sizeof(sMsg));
								//									char buffer[SIZE_MAX_PACKET];
								//									memset(buffer,0x00,SIZE_MAX_PACKET);
								//									int nType = T_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME;
								//									memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
								//									int cont = 0;
								//									for(int i=0;i<MAX_PARTY_NUMBER - 1;i++)
								//									{
								//										if(g_pD3dApp->m_pShuttleChild->m_nCharactUniqueNumber[i])
								//										{
								//											memcpy(buffer+SIZE_FIELD_TYPE_HEADER+sizeof(sMsg)+(cont)*sizeof(UINT), &g_pD3dApp->m_pShuttleChild->m_nCharactUniqueNumber[i], sizeof(UINT));
								//											cont++;
								//										}
								//									}
								//									sMsg.MapChannelIndex.MapIndex = atoi(name);
								//									sMsg.nPartyMembers = cont;
								//									memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
								//									g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + cont*sizeof(UINT));
								bSendChat = TRUE;
							}
						}
					}
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_C_COMMAND_BATTLE) || !stricmp(chatOrder, STRCMD_C_COMMAND_COMBAT))//"/결투","/대결"
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{
								// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 									//"아레나에서는 이용할 수 없습니다."
// 									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

								if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

								// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정

							// 결투를 요청
							memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
							CEnemyData * pPK = NULL;
							CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
							while (itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
							{
								if (!stricmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName, name))
								{
									pPK = itEnemy->second;
								}
								itEnemy++;
							}
							if (pPK)
							{
								MSG_FC_BATTLE_REQUEST_P2P_PK sMsg;
								memset(&sMsg, 0x00, sizeof(sMsg));
								sMsg.TargetClientIndex = pPK->m_infoCharacter.CharacterInfo.ClientIndex;
								sMsg.AdditionalParameter = g_pD3dApp->m_nPVPoption;
								char buffer[SIZE_MAX_PACKET];
								memset(buffer, 0x00, SIZE_MAX_PACKET);
								int nType = T_FC_BATTLE_REQUEST_P2P_PK;
								memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
								memcpy(buffer + SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
								g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
								bSendChat = TRUE;
								g_fSendMovePacket = max(g_fSendMovePacket, 20.0f);
							}
						}
					}
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, "/fightmode"))
						{
							if (g_pD3dApp->m_nPVPoption == 0) g_pD3dApp->m_nPVPoption = 1;
							else g_pD3dApp->m_nPVPoption = 0;
							
							bSendChat = FALSE;
						}
					}
					if (!bSendChat)
					{
						re = stricmp(chatOrder, STRCMD_C_COMMAND_SURRENDER);//"/항복"
						if (re == 0)
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{

								// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 									//"아레나에서는 이용할 수 없습니다."
// 									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

								if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

								// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정

							// 항복을 요청
							memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
							CEnemyData * pPK = NULL;
							CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
							while (itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
							{
								if (!stricmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName, name))// &&
								{
									pPK = itEnemy->second;
								}
								itEnemy++;
							}
							if (pPK)
							{
								MSG_FC_BATTLE_SURRENDER_P2P_PK sMsg;
								memset(&sMsg, 0x00, sizeof(sMsg));
								char buffer[SIZE_MAX_PACKET];
								int nType = T_FC_BATTLE_SURRENDER_P2P_PK;
								sMsg.TargetClientIndex = pPK->m_infoCharacter.CharacterInfo.ClientIndex;
								memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
								memcpy(buffer + SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
								g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
								bSendChat = TRUE;
							}
						}
					}
					// 2007-10-06 by dgwoo 부사령관 부의장 설정 팝업창.
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_CS_COMMAND_SUBLEADER1_0) || !stricmp(chatOrder, STRCMD_CS_COMMAND_SUBLEADER1_1))
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{

								// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 									//"아레나에서는 이용할 수 없습니다."
// 									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

								if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

								// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정

							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER)
								|| COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION))
							{
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
								if (IS_ANI_INFLUENCE_TYPE(g_pShuttleChild->m_myShuttleInfo.InfluenceType))
								{
									wsprintf(chatOrder, STRMSG_C_070214_0100, name, STRCMD_ANI_INFLUENCE_SUBLEADER_1);
								}
								else
								{
									wsprintf(chatOrder, STRMSG_C_070214_0100, name, STRCMD_VCN_INFLUENCE_SUBLEADER_1);
								}

								g_pGameMain->m_pInfWindow->AddMsgBox(chatOrder, _Q_UPDATE_SUBLEADER_1, 0, 0, 0, 0, name);
							}
						}
					}
					// 2007-10-06 by dgwoo 참모총장 작전본부장 설정 팝업창.
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_CS_COMMAND_SUBLEADER2_0) || !stricmp(chatOrder, STRCMD_CS_COMMAND_SUBLEADER2_1))
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{

								// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 									//"아레나에서는 이용할 수 없습니다."
// 									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

								if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

								// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템


								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정

							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER)
								|| COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION))
							{
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
								if (IS_ANI_INFLUENCE_TYPE(g_pShuttleChild->m_myShuttleInfo.InfluenceType))
								{
									wsprintf(chatOrder, STRMSG_C_070214_0100, name, STRCMD_ANI_INFLUENCE_SUBLEADER_2);
								}
								else
								{
									wsprintf(chatOrder, STRMSG_C_070214_0100, name, STRCMD_VCN_INFLUENCE_SUBLEADER_2);
								}

								g_pGameMain->m_pInfWindow->AddMsgBox(chatOrder, _Q_UPDATE_SUBLEADER_2, 0, 0, 0, 0, name);
							}
						}
					}

					// 2007-03-20 by dgwoo 옵저버 모드.
					if (!bSendChat)
					{
						if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION)
							|| COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_GAMEMASTER))
						{
							// 옵저버 종료.
							if (!stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_END_1)
								|| !stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_END_0))
							{
								g_pShuttleChild->m_bObserve = FALSE;
								g_pShuttleChild->m_nAlphaValue = SKILL_OBJECT_ALPHA_NONE;
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070403_0102, COLOR_ERROR);	//"옵저버 모드가 시작되었습니다."
								bSendChat = TRUE;
							}
							// 옵저버 시작.
							if (!stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_START_1)
								|| !stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_START_0))
							{
								g_pShuttleChild->m_bObserve = TRUE;
								g_pShuttleChild->m_nAlphaValue = SKILL_OBJECT_ALPHA_OTHER_INFLUENCE;
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070403_0101, COLOR_ERROR);	//"옵저버 모드가 종료되었습니다."
								// 2007-04-04 by dgwoo 시작시 셔틀의 정보를 저장해둔다.
								g_pShuttleChild->m_stObserve.ClientInfo.DP = g_pShuttleChild->m_myShuttleInfo.DP;
								g_pShuttleChild->m_stObserve.ClientInfo.HP = g_pShuttleChild->m_myShuttleInfo.HP;
								g_pShuttleChild->m_stObserve.ClientInfo.EP = g_pShuttleChild->m_myShuttleInfo.EP;
								g_pShuttleChild->m_stObserve.ClientInfo.SP = g_pShuttleChild->m_myShuttleInfo.SP;
								g_pShuttleChild->m_stObserve.ClientInfo.CurrentDP = g_pShuttleChild->m_myShuttleInfo.CurrentDP;
								g_pShuttleChild->m_stObserve.ClientInfo.CurrentHP = g_pShuttleChild->m_myShuttleInfo.CurrentHP;
								g_pShuttleChild->m_stObserve.ClientInfo.CurrentEP = g_pShuttleChild->m_myShuttleInfo.CurrentEP;
								g_pShuttleChild->m_stObserve.ClientInfo.CurrentSP = g_pShuttleChild->m_myShuttleInfo.CurrentSP;
								//--------------------------------------------------------------------------//

								bSendChat = TRUE;
							}
						}
					}
					// 2008-02-15 by bhsohn 네트워크 속도 체크
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRMSG_C_080215_0201))//"/NetCheck"								
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								char chParam[256];
								memset(chParam, 0x00, 256);

								strncpy(chParam, m_strInputMessage + n + 1, strlen(m_strInputMessage));
								// 네트워크 체크 모드 시작
								g_pD3dApp->StartNetCheck(chParam);
								bSendChat = TRUE;
							}
						}
					}
					// end 2008-02-15 by bhsohn 네트워크 속도 체크
					// 2008-06-19 by dgwoo 모선전정보옵션 창 띄우기.
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_COMMAND_WAR_OPTION_0)  //"/모선전정보옵션"								
							|| !stricmp(chatOrder, STRCMD_COMMAND_WAR_OPTION_1))			// 2008-10-21 by dgwoo 모선전정보옵션 문 추가.
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER | RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								g_pGameMain->GetINFMotherShipManager()->ShowWindowOption();
								bSendChat = TRUE;
							}
						}
					}


					// 2008-02-27 by bhsohn 통합 아레나 수정
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_CS_COMMAND_CHANGEINFL_0)
							|| !stricmp(chatOrder, STRCMD_CS_COMMAND_CHANGEINFL_1))//"/세력변경"
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								if (g_pFieldWinSocket->IsFCConnectedArenaServer())
								{

									// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 										//"아레나에서는 이용할 수 없습니다."
// 										g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

									if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
										g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
									else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
										g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

									// end end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

									bSendChat = TRUE;
								}
							}
						}
					}
					// end 2008-02-27 by bhsohn 통합 아레나 수정

					// 2007-11-22 by bhsohn 아레나 통합서버
					// 아레나 이동
					if (!bSendChat)
					{
						if ((!stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAMOVE_0))
							|| (!stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAMOVE_1)))//"/ArenaMove"
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR)
								&& (ARENA_STATE_ARENA_GAMING != g_pD3dApp->GetArenaState()))
							{
								// 아레나맵으로 이동
								g_pD3dApp->StartGmConnectAenenaServer();
								bSendChat = TRUE;
							}
						}

					}
					// end 2007-11-22 by bhsohn 아레나 통합서버

					// 2008-11-13 by bhsohn 조이스틱 작업
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRMSG_C_081120_0204))//"/조이스틱사용"
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								BOOL bUse = g_pD3dApp->IsUseJoyStick();
								bUse ^= TRUE;
								g_pD3dApp->SetUseJoyStick(bUse);

								bSendChat = TRUE;
							}
						}
					}
					// end 2008-11-13 by bhsohn 조이스틱 작업

					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_C_COMMAND_PARTYCOMBAT) || !stricmp(chatOrder, STRCMD_C_COMMAND_PARTYBATTLE) || !stricmp(chatOrder, STRCMD_C_COMMAND_PARTYWAR))//"/편대대결""/편대결투""/편대전투"
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{

								// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 									//"아레나에서는 이용할 수 없습니다."
// 									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

								if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

								// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정
							// 2008-06-20 by bhsohn EP3 옵션관련 처리
							if (!g_pD3dApp->IsOptionEtc(OPTION_RADIO_FORMATIONWAR))
							{
								break;
							}
							// end 2008-06-20 by bhsohn EP3 옵션관련 처리

							if (g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
							{
								// 편대대결을 요청
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));
								CEnemyData * pPK = NULL;
								CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
								while (itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
								{
									if (!stricmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName, name))// &&
										//itEnemy->second == g_pD3dApp->m_pShuttleChild->m_pTarget)
									{
										pPK = itEnemy->second;
									}
									itEnemy++;
								}
								// 2008-07-14 by bhsohn EP3 관련 처리
								if (pPK && g_pGameMain->IsRejectUser(pPK->m_infoCharacter.CharacterInfo.CharacterName))
								{
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207, COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
									break;
								}
								// end 2008-07-14 by bhsohn EP3 관련 처리
								if (pPK)
								{
									MSG_FC_REQUEST_REQUEST sMsg;
									memset(&sMsg, 0x00, sizeof(sMsg));
									char buffer[SIZE_MAX_PACKET];
									int nType = T_FC_REQUEST_REQUEST;
									sMsg.TargetClientIndex = pPK->m_infoCharacter.CharacterInfo.ClientIndex;
									sMsg.RequestType = REQUEST_TYPE_PARTY_BATTLE;
									memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
									memcpy(buffer + SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
									g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
									bSendChat = TRUE;

									// 편대전 요청시 편대장 메뉴 사용 불가 세팅
									g_pGameMain->m_pCommunity->GetParty()->SetEnablePartyMenu(FALSE);
								}
							}
							else
							{
								bSendChat = TRUE;
							}
						}
					}
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_C_COMMAND_GUILDBATTLE) //"/여단전투"
							|| !stricmp(chatOrder, STRCMD_C_COMMAND_GUILDCOMBAT)//"/여단대결"
							|| !stricmp(chatOrder, STRCMD_C_COMMAND_GUILDWAR))//"/여단전쟁"
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{
								// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 									//"아레나에서는 이용할 수 없습니다."
// 									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

								if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								else if (g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY)
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322, COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

								// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정
							// 2008-06-20 by bhsohn EP3 옵션관련 처리
							if (!g_pD3dApp->IsOptionEtc(OPTION_RADIO_GUILD_WAR))
							{
								break;
							}
							// end 2008-06-20 by bhsohn EP3 옵션관련 처리

							MSG_IC_GUILD_GET_GUILD_INFO_OK * pGuild = g_pGameMain->m_pCommunity->GetGuild()->GetGuildInfo();
							if (g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber != 0 && pGuild)
							{
								if (pGuild->GuildCommanderUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
								{
									memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));

									// 2008-07-14 by bhsohn EP3 관련 처리										
									if (g_pGameMain->IsRejectUser(name))
									{
										g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207, COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
									}
									// end 2008-07-14 by bhsohn EP3 관련 처리									

									//if( strlen(name) > 0 )
									if (strlen(name) > 0 && !g_pGameMain->IsRejectUser(name))
									{
										MSG_FC_REQUEST_REQUEST sMsg;
										memset(&sMsg, 0x00, sizeof(sMsg));
										sMsg.RequestType = REQUEST_TYPE_GUILD_WAR;
										strcpy(sMsg.TargetCharacterName, name);
										g_pFieldWinSocket->SendMsg(T_FC_REQUEST_REQUEST, (char*)&sMsg, sizeof(sMsg));
									}
								}
								else
								{
									g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_GUILD_0034, COLOR_CHAT_GUILD);//"여단전은 여단장만 신청할 수 있습니다."
								}
							}
							bSendChat = TRUE;
						}
					}
					if (!bSendChat)
					{
						re = stricmp(chatOrder, STRCMD_C_COMMAND_GUILDSURRENDER);//"/여단전항복"
						if (re == 0)
						{
							// 2008-02-27 by bhsohn 통합 아레나 수정
							if (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
							{
								//"아레나에서는 이용할 수 없습니다."
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203, COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
								break;
							}
							// end 2008-02-27 by bhsohn 통합 아레나 수정

							if (g_pShuttleChild->m_pGuildWarInfo)
							{
								MSG_IC_GUILD_GET_GUILD_INFO_OK * pGuild = g_pGameMain->m_pCommunity->GetGuild()->GetGuildInfo();
								if (pGuild &&
									pGuild->GuildCommanderUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
								{
									g_pIMSocket->SendMsg(T_IC_GUILD_SURRENDER_GUILD_WAR, NULL, 0);
								}
							}
							bSendChat = TRUE;
						}
					}
					if (!bSendChat)
					{
						if (stricmp(chatOrder, STRCMD_C_COMMAND_NAME) == 0)//"/호칭"
						{
							MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo = g_pGameMain->m_pCommunity->GetGuild()->GetGuildInfo();
							if (pInfo && pInfo->GuildCommanderUniqueNumber == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
							{
								char name[30], level[30];
								memset(name, 0x00, sizeof(name));
								memset(level, 0x00, sizeof(level));
								int n2 = n + 1;
								if (strlen(m_strInputMessage) > 1)
								{
									while (true)
									{
										if (m_strInputMessage[n2] == ' ')// || m_strInputMessage[n] == '\n')
											break;

										n2++;
										if (n2 == strlen(m_strInputMessage))//SIZE_MAX_CHAT_MESSAGE)
											break;
									}
								}
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage) - n - (strlen(m_strInputMessage) - n2 - 1));
								memcpy(level, m_strInputMessage + n2 + 1, strlen(m_strInputMessage) - n2);

								CEnemyData * pEnemy = NULL;
								CMapEnemyIterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
								while (itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
								{
									if (!stricmp(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName, name))
									{
										pEnemy = itEnemy->second;
									}
									itEnemy++;
								}
								if (pEnemy)
								{
									MSG_IC_GUILD_SET_RANK sMsg;
									sMsg.GuildRank = atoi(level);
									sMsg.MemberUniqueNumber = pEnemy->m_infoCharacter.CharacterInfo.CharacterUniqueNumber;
									g_pIMSocket->SendMsg(T_IC_GUILD_SET_RANK, (char*)&sMsg, sizeof(sMsg));
								}
							}
							bSendChat = TRUE;

						}
					}
#ifdef _DEBUG
					if (!bSendChat)
					{
						if (stricmp(chatOrder, STRCMD_C_COMMAND_WARP) == 0)//"/워프"
						{
							CObjectChild* pWarpgate = g_pScene->FindWarpGateByPosition(g_pShuttleChild->m_vPos);
							if (pWarpgate)
							{
								pWarpgate->ChangeBodycondition(BODYCON_WARP_MASK);
							}
							if (g_pD3dApp->m_pEffectList)
							{
								CAppEffectData *pData = new CAppEffectData(g_pShuttleChild, RC_EFF_OTHER_WARP, D3DXVECTOR3(0, 0, 0));
								g_pD3dApp->m_pEffectList->AddChild(pData);
							}
							bSendChat = TRUE;
						}
					}
					// 2004-11-22 by jschoi
					// 패시브를 제외한 모든 사용중인 스킬 해제
					if (!bSendChat)
					{
						if (stricmp(chatOrder, STRCMD_C_COMMAND_CANCELSKILL) == 0)//"/스킬취소"
						{
							if (g_pShuttleChild->m_pSkill)
							{
								g_pShuttleChild->m_pSkill->ReleaseAllUsingSkillWithOutPermanent();
								// 2007-04-05 by bhsohn 빅붐 버그 처리
								// 과거 스킬초기화
								g_pShuttleChild->m_pSkill->InitPreSkillItemNum();
							}
							bSendChat = TRUE;
						}
					}
#endif
					if (!bSendChat)
					{
						if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR) &&
							stricmp(chatOrder, STRCMD_C_COMMAND_INITCHAT) == 0)//"/채팅창초기화"
						{
							InitChatData();
							bSendChat = TRUE;
						}
					}
					// 2006-07-25 by ispark
					// 2006-08-01 by ispark, 명령어 STRCMD_CS_COMMAND_GUILDCOMEON_1 추가
					if (!bSendChat)
					{
						if (!stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDCOMEON) ||//"/여단오기"
							!stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDCOMEON_1))
						{
							if (COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
							{
								memcpy(name, m_strInputMessage + n + 1, strlen(m_strInputMessage));

								MSG_IC_ADMIN_CALL_GUILD sMsg;
								strcpy(sMsg.GuildName, name);
								sMsg.MapChannelIndex = g_pShuttleChild->m_myShuttleInfo.MapChannelIndex;
								sMsg.Position = g_pShuttleChild->m_vPos;
								g_pIMSocket->SendMsg(T_IC_ADMIN_CALL_GUILD, (char*)&sMsg, sizeof(sMsg));

								bSendChat = TRUE;
							}
						}
					}

					// 2006-04-18 by ispark
					if (!bSendChat)
					{
						if (strlen(chatOrder) == strlen(m_strInputMessage) &&
							stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_0) == 0)
						{
							g_pGameMain->DrawSignBoardString();
						}
					}

					if (!bSendChat)
					{
#ifndef ARESBETA
						auto nRace = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race;

						bSendChat = TRUE;
						// 2005-11-02 by ispark
						// 운영자에게도 관리자 권한을 준다. 단, 거래는 제외(이부분은 다른곳에서)
						if (COMPARE_RACE(nRace, RACE_OPERATION))
						{
							bSendChat = FALSE;
						}
						else if (COMPARE_RACE(nRace, RACE_GAMEMASTER))
						{
							if (stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE) == 0 ||				//"/이동" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_POWERUP) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_POWERUP_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_KICK_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_KICK_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_TOGGLE_CNC) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENT_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENT_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENT_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAMOVE_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAMOVE_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_START_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_START_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_END_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_END_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_STRATEGYPOINTINFO_0) == 0 ||				//"/멘트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STRATEGYPOINTINFO_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHANGEINFL_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHANGEINFL_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_0) == 0 ||		//"/모든아템버리기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_1) == 0 ||		//"/모든아템지우기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_2) == 0 ||		//"/delAllItem"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM) == 0 ||	//"/아템" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_1) == 0 ||	//"/item" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_INFObyNAME) == 0 ||			//"/정보"
								stricmp(chatOrder, STRCMD_CS_COMMAND_INFObyNAME_1) == 0 ||		//"/info"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SKILLALL) == 0 ||			//"/모든스킬" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_SKILLALL_1) == 0 ||			//"/allSkill" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_LIST) == 0 ||				//"/리스트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_LIST_1) == 0 ||				//"/list"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND) == 0 ||			//"/보내기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND_1) == 0 ||			//"/senduser"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MONSUMMON) == 0 ||			//"/소환"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MONSUMMON_1) == 0 ||			//"/summon"
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRALL) == 0 ||			//"/만피" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRALL_1) == 0 ||			//"/repairAll" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVINCIBILITY) == 0 ||	//"/무적" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVINCIBILITY_1) == 0 ||	//"/invincibility" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVISIABLE) == 0 ||		//"/투명" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVISIABLE_1) == 0 ||	//"/invisiable" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GOUSER) == 0 ||				//"/가기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GOUSER_1) == 0 ||			//"/go"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEON) == 0 ||				//"/오기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEON_1) == 0 ||			//"/comeon"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STATINIT) == 0 ||			//"/스탯초기화"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STATINIT_1) == 0 ||			//"/initStatus"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COORDINATE) == 0 ||			//"/좌표" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_COORDINATE_1) == 0 ||		//"/coordinate" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMEEVENT) == 0 ||			//"/이벤트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMEEVENT_1) == 0 ||			//"/event" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBID) == 0 ||			//"/채금" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBID_1) == 0 ||		//"/forbidChat" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_0) == 0 ||				//"/멘트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_2) == 0 ||				//"/ment" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSEBATTLE) == 0 ||			//"/결투거부" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSETRADE) == 0 ||			//"/거래거부" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_WEATHERSET) == 0 ||			//"/날씨"
								stricmp(chatOrder, STRCMD_CS_COMMAND_WEATHERSET_1) == 0 ||		//"/weather"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBIDRELEASE) == 0 ||	//"/채금해제"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_1) == 0 ||	//"/releaseChat"
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEVELSET) == 0 ||			//"/레벨"
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEVELSET_1) == 0 ||			//"/level"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMETIME) == 0 ||			//"/시간"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMETIME_1) == 0 ||			//"/Time"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSINFOperMAP) == 0 ||		//"/서버맵"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSINFOperMAP_1) == 0 ||	//"/serverMap"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STEALTH) == 0 ||				//"/스텔스"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STEALTH_1) == 0 ||			//"/stealth"
								stricmp(chatOrder, STRCMD_CS_COMMAND_RETURNALL) == 0 ||			//"/모두귀환"
								stricmp(chatOrder, STRCMD_CS_COMMAND_RETURNALL_1) == 0 ||			//"/returnAll"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDCOMEON) == 0 ||			//"/여단오기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDCOMEON_1) == 0 ||		//"/comeonGuild"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEONINFL_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEONINFL_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEONINFL_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDSEND) == 0 ||			//"/여단보내기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDSEND_1) == 0 ||			//"/sendGuild"
								stricmp(chatOrder, STRCMD_CS_COMMAND_BULLETCHARGE) == 0 ||		//"/총알" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_BULLETCHARGE_1) == 0 ||		//"/chargeBullet" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0 ||		//"/?"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_1) == 0 ||		//"/help"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_2) == 0)			//"/명령어"
							{
								bSendChat = FALSE;
							}
						}
						else if (COMPARE_RACE(nRace, RACE_MONITOR))
						{
							if (stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE) == 0 ||				//"/이동" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_KICK_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_KICK_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_TOGGLE_CNC) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_UsableSpeakerItem_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_UsableSpeakerItem_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_UsableSpeakerItem_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_NEWACCOUNTBLOCK_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENT_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENT_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENT_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAMOVE_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ARENAMOVE_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_START_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_START_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_END_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_END_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_OBSERVER_REG_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_STRATEGYPOINTINFO_0) == 0 ||				//"/멘트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STRATEGYPOINTINFO_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHANGEINFL_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHANGEINFL_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_0) == 0 ||		//"/모든아템버리기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_1) == 0 ||		//"/모든아템지우기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_2) == 0 ||		//"/delAllItem"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM) == 0 ||	//"/아템" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_1) == 0 ||	//"/item" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_INFObyNAME) == 0 ||			//"/정보"
								stricmp(chatOrder, STRCMD_CS_COMMAND_INFObyNAME_1) == 0 ||		//"/info"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SKILLALL) == 0 ||			//"/모든스킬" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_SKILLALL_1) == 0 ||			//"/allSkill" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_LIST) == 0 ||				//"/리스트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_LIST_1) == 0 ||				//"/list"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND) == 0 ||			//"/보내기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND_1) == 0 ||			//"/senduser"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MONSUMMON) == 0 ||			//"/소환"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MONSUMMON_1) == 0 ||			//"/summon"
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRALL) == 0 ||			//"/만피" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRALL_1) == 0 ||			//"/repairAll" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVINCIBILITY) == 0 ||	//"/무적" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVINCIBILITY_1) == 0 ||	//"/invincibility" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVISIABLE) == 0 ||		//"/투명" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVISIABLE_1) == 0 ||	//"/invisiable" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GOUSER) == 0 ||				//"/가기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GOUSER_1) == 0 ||			//"/go"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEON) == 0 ||				//"/오기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEON_1) == 0 ||			//"/comeon"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STATINIT) == 0 ||			//"/스탯초기화"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STATINIT_1) == 0 ||			//"/initStatus"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COORDINATE) == 0 ||			//"/좌표" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_COORDINATE_1) == 0 ||		//"/coordinate" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMEEVENT) == 0 ||			//"/이벤트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMEEVENT_1) == 0 ||			//"/event" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBID) == 0 ||			//"/채금" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBID_1) == 0 ||		//"/forbidChat" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_0) == 0 ||				//"/멘트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_2) == 0 ||				//"/ment" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSEBATTLE) == 0 ||			//"/결투거부" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSETRADE) == 0 ||			//"/거래거부" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_WEATHERSET) == 0 ||			//"/날씨"
								stricmp(chatOrder, STRCMD_CS_COMMAND_WEATHERSET_1) == 0 ||		//"/weather"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBIDRELEASE) == 0 ||	//"/채금해제"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_1) == 0 ||	//"/releaseChat"
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEVELSET) == 0 ||			//"/레벨"
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEVELSET_1) == 0 ||			//"/level"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMETIME) == 0 ||			//"/시간"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMETIME_1) == 0 ||			//"/Time"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSINFOperMAP) == 0 ||		//"/서버맵"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSINFOperMAP_1) == 0 ||	//"/serverMap"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STEALTH) == 0 ||				//"/스텔스"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STEALTH_1) == 0 ||			//"/stealth"
								stricmp(chatOrder, STRCMD_CS_COMMAND_RETURNALL) == 0 ||			//"/모두귀환"
								stricmp(chatOrder, STRCMD_CS_COMMAND_RETURNALL_1) == 0 ||			//"/returnAll"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDCOMEON) == 0 ||			//"/여단오기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDCOMEON_1) == 0 ||		//"/comeonGuild"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEONINFL_0) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEONINFL_1) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMEONINFL_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDSEND) == 0 ||			//"/여단보내기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GUILDSEND_1) == 0 ||			//"/sendGuild"
								stricmp(chatOrder, STRCMD_CS_COMMAND_BULLETCHARGE) == 0 ||		//"/총알" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_BULLETCHARGE_1) == 0 ||		//"/chargeBullet" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0 ||		//"/?"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_1) == 0 ||		//"/help"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_2) == 0)			//"/명령어"
							{
								bSendChat = FALSE;
							}
						}
						else if (COMPARE_RACE(nRace, RACE_GUEST))
						{
							if (stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE) == 0 ||				//"/이동" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE_1) == 0 ||				//"/move"
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU) == 0 ||			//"/누구" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU_1) == 0 ||			//"/whoareYou"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_0) == 0 ||				//"/멘트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_2) == 0 ||				//"/ment" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSEBATTLE) == 0 ||			//"/결투거부"
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSETRADE) == 0 ||			//"/거래거부"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATPTOPFLAG) == 0 ||		//"/귓말"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATPTOPFLAG_1) == 0 ||		//"/whisperChat"
								stricmp(chatOrder, STRMSG_C_050810_0001) == 0 ||					// "/창끄기"
								stricmp(chatOrder, STRMSG_C_050810_0002) == 0 ||					// "/창켜기"
								// 2006-05-26 by ispark
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_0) == 0 ||			//"/CallGM"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_1) == 0 ||			//"/도우미"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_2) == 0 ||			//"/도움말"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_0) == 0 ||			//"/전광판"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_1) == 0 ||			//"/Noticeboard"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_DEL_0) == 0 ||		//"/전광판"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_DEL_1) == 0 ||		//"/Noticeboard"
								// 2007-10-23 by bhsohn /입장허가, /입장불가 명령어 사용가능하게 함.
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_0) == 0 ||//"/입장허가"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_1) == 0 ||//"/EntrancePermission"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_2) == 0 ||//"/EntrancePermission"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_0) == 0 ||	//"/입장불가"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_1) == 0 ||	 //"/EntrancePermissionDeny"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_2) == 0)		 //"/EntrancePermissionDeny"

							{
								bSendChat = FALSE;
							}
						}
						else if (COMPARE_RACE(nRace, RACE_DEMO))
						{
							if (stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE) == 0 ||				//"/이동" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MOVE_1) == 0 ||				//"/move"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND) == 0 ||			//"/보내기" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND_1) == 0 ||			//"/senduser"
								stricmp(chatOrder, STRCMD_CS_COMMAND_INFObyNAME) == 0 ||			//"/정보"
								stricmp(chatOrder, STRCMD_CS_COMMAND_INFObyNAME_1) == 0 ||		//"/info"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMETIME) == 0 ||			//"/시간" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_GAMETIME_1) == 0 ||			//"/Time"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MONSUMMON) == 0 ||			//"/소환"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MONSUMMON_1) == 0 ||			//"/summon"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SKILLALL) == 0 ||			//"/모든스킬" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_SKILLALL_1) == 0 ||			//"/allSkill" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_0) == 0 ||		//"/모든아템버리기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_1) == 0 ||		//"/모든아템지우기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMDELALL_2) == 0 ||		//"/delAllItem"
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM) == 0 ||	//"/아템" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM_1) == 0 ||	//"/item" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_BULLETCHARGE) == 0 ||		//"/총알" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_BULLETCHARGE_1) == 0 ||		//"/chargeBullet" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRALL) == 0 ||			//"/만피" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRALL_1) == 0 ||			//"/repairAll" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRbyPARAM) == 0 ||		//"/반피" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_REPAIRbyPARAM_1) == 0 ||		//"/repairParam" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVINCIBILITY) == 0 ||	//"/무적" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERINVINCIBILITY_1) == 0 ||	//"/invincibility" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEVELSET) == 0 ||			//"/레벨" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEVELSET_1) == 0 ||			//"/level"
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHO) == 0 ||					//"/누구있지"
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHO_1) == 0 ||				//"/who" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU) == 0 ||			//"/누구" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU_1) == 0 ||			//"/whoareYou"
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND) == 0 ||			//"/보내기" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_USERSEND_1) == 0 ||			//"/senduser"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GOUSER) == 0 ||				//"/가기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_GOUSER_1) == 0 ||			//"/go"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATPTOPFLAG) == 0 ||		//"/귓말"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CHATPTOPFLAG_1) == 0 ||		//"/whisperChat"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_0) == 0 ||				//"/멘트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_1) == 0 ||				//"/맨트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_2) == 0 ||				//"/ment" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSEBATTLE) == 0 ||			//"/결투거부"
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSETRADE) == 0 ||			//"/거래거부"  
								stricmp(chatOrder, STRCMD_CS_COMMAND_WEATHERSET) == 0 ||		//"/날씨"
								stricmp(chatOrder, STRCMD_CS_COMMAND_WEATHERSET_1) == 0 ||		//"/weather"
								stricmp(chatOrder, STRMSG_C_050810_0001) == 0 ||					// "/창끄기"
								stricmp(chatOrder, STRMSG_C_050810_0002) == 0 ||					// "/창켜기"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_0) == 0 ||			//"/CallGM"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_1) == 0 ||			//"/도우미"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_2) == 0 ||			//"/도움말"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_0) == 0 ||			//"/전광판"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_1) == 0 ||			//"/Noticeboard"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_DEL_0) == 0 ||		//"/전광판"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_DEL_1) == 0 ||		//"/Noticeboard"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_0) == 0 ||//"/입장허가"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_1) == 0 ||//"/EntrancePermission"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_2) == 0 ||//"/EntrancePermission"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_0) == 0 ||	//"/입장불가"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_1) == 0 ||	 //"/EntrancePermissionDeny"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_2) == 0)		 //"/EntrancePermissionDeny"

							{
								bSendChat = FALSE;
							}
						}
						else
						{
							if (stricmp(chatOrder, STRCMD_CS_COMMAND_STRATEGYPOINTINFO_0) == 0 ||				//"/멘트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_STRATEGYPOINTINFO_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_0) == 0 ||				//"/멘트"
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_1) == 0 ||				//"/맨트" 
								stricmp(chatOrder, STRCMD_CS_COMMAND_MENT_2) == 0 ||				//"/ment" 
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSEBATTLE) == 0 ||			//"/결투거부"
								stricmp(chatOrder, STRCMD_C_COMMAND_REFUSETRADE) == 0 ||			//"/거래거부"
								stricmp(chatOrder, STRMSG_C_050810_0001) == 0 ||					// "/창끄기"
								stricmp(chatOrder, STRMSG_C_050810_0002) == 0 ||					// "/창켜기"
								// 2006-05-22 by ispark
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0 ||		//"/?"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_1) == 0 ||		//"/help"
								stricmp(chatOrder, STRCMD_CS_COMMAND_COMMANDLIST_2) == 0 ||		//"/명령어"
								// 2006-05-26 by ispark
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_0) == 0 ||			//"/CallGM"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_1) == 0 ||			//"/도우미"
								stricmp(chatOrder, STRCMD_CS_COMMAND_CALLGM_2) == 0 ||			//"/도움말"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_0) == 0 ||			//"/전광판"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_1) == 0 ||			//"/Noticeboard"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_DEL_0) == 0 ||		//"/전광판"
								stricmp(chatOrder, STRCMD_CS_COMMAND_SIGNBOARD_DEL_1) == 0 ||		//"/Noticeboard"
								stricmp(chatOrder, STRCMD_CS_COMMAND_PLAYTIME_0) == 0 ||				//"/PlayTime"	
								stricmp(chatOrder, STRCMD_CS_COMMAND_PLAYTIME_1) == 0 ||				//"/이용시간"
								// 2007-10-23 by bhsohn /입장허가, /입장불가 명령어 사용가능하게 함.
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_0) == 0 ||//"/입장허가"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_1) == 0 ||//"/EntrancePermission"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermission_2) == 0 ||//"/EntrancePermission"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_0) == 0 ||	//"/입장불가"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_1) == 0 ||	 //"/EntrancePermissionDeny"
								stricmp(chatOrder, STRCMD_CS_COMMAND_EntrancePermissionDeny_2) == 0 ||
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEADER_FORBID_WARP_0) == 0 ||		// 2016-01-03 Future, leader warp restriction
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEADER_FORBID_WARP_1) == 0 ||		// 2016-01-03 Future, leader warp restriction
								stricmp(chatOrder, STRCMD_CS_COMMAND_LEADER_RELEASE_WARP) == 0)			// 2016-01-03 Future, leader warp restriction


							{

								bSendChat = FALSE;
							}
						}
#endif
					}
					if (!bSendChat)
					{

						if (stricmp(chatOrder, STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUMRANGE) == 0)
						{
							char szAddItemRange[256] = { 0, };

							char szAddItemStartNum[256] = { 0, };
							char szAddItemEndNum[256] = { 0, };
							char szAddItemPCS[256] = { 0, };

							GetStrPara(szAddItemStartNum, m_strInputMessage, 1);
							GetStrPara(szAddItemEndNum, m_strInputMessage, 2);
							GetStrPara(szAddItemPCS, m_strInputMessage, 3);

							if (strlen(szAddItemStartNum) && strlen(szAddItemEndNum))
							{

								int iStartItemIdx = atoi(szAddItemStartNum);
								int iEndItemIdx = atoi(szAddItemEndNum);
								int iItemPCS = atoi(szAddItemPCS);

								if (iStartItemIdx > iEndItemIdx)
									swap(iStartItemIdx, iEndItemIdx);

								for (int iRange = iStartItemIdx; iRange < iEndItemIdx; ++iRange)
								{

									if (NULL == g_pDatabase->GetServerItemInfo(iRange))
										continue;

									sprintf(szAddItemRange, "%s %d", STRCMD_CS_COMMAND_ITEMINSERTbyITEMNUM, iRange);

									if (iItemPCS > 0)
									{
										strcat(szAddItemRange, " ");
										strcat(szAddItemRange, szAddItemPCS);
									}

									g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, szAddItemRange);
								}
							}

							m_strInputMessage[0] = 0;

						}

						// 다른캐릭터의 정보를 가져온다
						if (stricmp(chatOrder, STRCMD_CS_COMMAND_WHOAREYOU) == 0)
						{
							char	buffOther[128];
							memset(buffOther, 0x00, 128);
							strncpy(buffOther, &m_strInputMessage[6], strlen(m_strInputMessage));
							// 2008-07-14 by bhsohn EP3 관련 처리
							//g_pGameMain->m_pOtherCharInfo->GetOtherCharInfo(buffOther);
						}
						if (strlen(m_strInputMessage))
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage);


						// 다른캐릭터의 정보를 가져온다
						if (stricmp(chatOrder, STRCMD_C_COMMAND_REFUSEBATTLE) == 0)
						{
							m_bPKOptionFlag = !m_bPKOptionFlag;
							if (m_bPKOptionFlag)
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PK_0005, COLOR_SYSTEM);//"결투 거부가 해제 되었습니다."
							}
							else
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_PK_0006, COLOR_SYSTEM);//"결투 거부 상태입니다."
							}
						}
						else if (stricmp(chatOrder, STRCMD_C_COMMAND_REFUSETRADE) == 0)
						{
							// 다른캐릭터의 정보를 가져온다
							m_bTradeFlag = !m_bTradeFlag;
							if (m_bTradeFlag)
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_TRADE_0016, COLOR_SYSTEM);//"거래 거부가 해제 되었습니다."
							}
							else
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_TRADE_0017, COLOR_SYSTEM);//"거래 거부 상태입니다."
							}
						}
						else if (stricmp(chatOrder, STRMSG_C_050810_0001) == 0 ||			// "/창끄기"
							stricmp(chatOrder, STRMSG_C_050810_0002) == 0)			// "/창켜기"
						{
							// 창끄기, 창켜기 옵션
							if (stricmp(chatOrder, STRMSG_C_050810_0001) == 0)
							{
								m_bWindowMessageFlag = TRUE;
							}
							else
							{
								m_bWindowMessageFlag = FALSE;
							}

							g_pGameMain->m_pInfWindow->SetMessageFlag(m_bWindowMessageFlag);
							if (!m_bWindowMessageFlag)
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_050810_0003, COLOR_SYSTEM);//"창끄기가 해제 되었습니다."
							}
							else
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_050810_0004, COLOR_SYSTEM);//"창끄기가 설정 되었습니다."
							}
						}
					}
				}

				else if (!bSendSpecialChat && strlen(m_strInputMessage) > 0 && !bBlankMsg)
				{
#ifdef FREEWAR_
					if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) || COMPARE_RACE(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_MONITOR))
					{
#endif
#ifdef CUSTOM_OPTIMIZER_HSSON
						if (g_cCustomOptimizer.m_bAddItems)
						{
							const int nMaxAddIndex = 10;
							char cBuf[nMaxAddIndex][SIZE_MAX_CHAT_MESSAGE] = { 0, };

							int i = 0;
							int j = 0;
							for (i = 0; i < nMaxAddIndex; ++i)
							{
								strcpy(cBuf[i], "/아템 ");
								CopyMemory(&cBuf[i][6], &m_strInputMessage[j], 9);
								j += 9;
							}

							for (i = 0; i < nMaxAddIndex; ++i)
							{
								g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, cBuf[i]);
							}
						}
						else
							// 2011. 10. 24 by hsson 채팅 안되던 버그 수정
#endif // CUSTOM_OPTIMIZER_HSSON
								// end 2011. 10. 24 by hsson 채팅 안되던 버그 수정
						{
							g_pD3dApp->m_pIMSocket->SendChat(T_IC_CHAT_REGION, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, m_strInputMessage);
						}
#ifdef FREEWAR_
					}
#endif

				}
				
				g_pD3dApp->CleanText();

				// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
				InitChatMsgBuff();

				util::zero(m_strInputMessage);

				if (strChatType != '\0')
				{
					g_pD3dApp->m_inputkey.m_full_str[0] = strChatType;
					g_pD3dApp->m_inputkey.m_str_pos = 1;
					m_strInputMessage[0] = strChatType;

					if (_REMEMBER_CHAT_COLOR && strLastColorChar != NULL)
					{
						g_pD3dApp->m_inputkey.m_full_str[1] = '\\';
						g_pD3dApp->m_inputkey.m_full_str[2] = strLastColorChar;
						g_pD3dApp->m_inputkey.m_str_pos = 3;
						m_strInputMessage[1] = '\\';
						m_strInputMessage[2] = strLastColorChar;

						if (strLastColorChar == '#')
						{
							memcpy(g_pD3dApp->m_inputkey.m_full_str + 3, lastextcolor, 6);
							g_pD3dApp->m_inputkey.m_full_str[9] = '#';

							g_pD3dApp->m_inputkey.m_str_pos = 10;

							memcpy(m_strInputMessage + 3, lastextcolor, 6);
							m_strInputMessage[9] = '#';
						}
					}

				}
				else if (_REMEMBER_CHAT_COLOR && strLastColorChar != NULL)
				{
					g_pD3dApp->m_inputkey.m_full_str[0] = '\\';
					g_pD3dApp->m_inputkey.m_full_str[1] = strLastColorChar;
					g_pD3dApp->m_inputkey.m_str_pos = 2;
					m_strInputMessage[0] = '\\';
					m_strInputMessage[1] = strLastColorChar;

					if (strLastColorChar == '#')
					{
						memcpy(g_pD3dApp->m_inputkey.m_full_str + 2, lastextcolor, 6);
						g_pD3dApp->m_inputkey.m_full_str[8] = '#';

						g_pD3dApp->m_inputkey.m_str_pos = 9;

						memcpy(m_strInputMessage + 2, lastextcolor, 6);
						m_strInputMessage[8] = '#';
					}
				}
			}
			
			return INF_MSGPROC_NORMAL;
		}
		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
		if (OnKeyDownCursel(wParam, lParam) == INF_MSGPROC_BREAK)
		
			return INF_MSGPROC_BREAK;

	}
	break;

	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	case WM_KEYUP:
	
	if (OnKeyUpFunc(wParam, lParam) == INF_MSGPROC_BREAK)
			
		return INF_MSGPROC_BREAK;

	break;

	case WM_IME_STARTCOMPOSITION:
	//case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:
	case WM_CHAR:
	{
		if (m_bChatMode)
		{
			int bFlag = FALSE;
			g_nRenderCandidate = SET_MAIN_CHAT_CANDIDATE;

			if (INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
				bFlag = TRUE;
			/*--------------------------------------------------------------------------*/

			// 2009. 06. 29 by ckPark 전각문자 입력시 채팅기호 계속 남는 문제

			//if(strlen(g_pD3dApp->m_inputkey.m_full_str)!=0)
			if (strlen(g_pD3dApp->m_inputkey.m_full_str) != 0 || uMsg == WM_IME_STARTCOMPOSITION)

				// end 2009. 06. 29 by ckPark 전각문자 입력시 채팅기호 계속 남는 문제

			{
				memset(m_strInputMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);
				if (wParam == '"' && g_pD3dApp->m_pShuttleChild->m_bOldPToPPos != 200 && g_pD3dApp->m_inputkey.m_str_pos == 1)
				{
					char tempName[SIZE_MAX_CHARACTER_NAME + 2] = { 0, };
					wsprintf(tempName, "%s ", g_pD3dApp->m_pShuttleChild->m_strChatPToP[g_pD3dApp->m_pShuttleChild->m_bOldPToPPos]);

					int nNameSize = strlen(tempName);
					wchar_t wTmName[SIZE_MAX_CHARACTER_NAME];
					memset(wTmName, 0x00, sizeof(wTmName[0])*SIZE_MAX_CHARACTER_NAME);
					MultiByteToWideChar(g_input.GetCodePage(), 0, tempName, nNameSize + 1, wTmName, nNameSize + 1);
					for (int j = 0; j < wcslen(wTmName); j++)
					{
						g_input.SetInputWideChar(wTmName[j]);
					}

					g_pD3dApp->m_inputkey.SetGameText(g_pD3dApp->m_inputkey.GetInputLanguage());

				}
				
				if (m_bCaretCursel == TRUE)
				{
					BlockCleanChat(m_ptCurselPos.x, m_ptSelCurselPos.x);
				}

				if (strlen(m_strBkInputMessage) >= SIZE_MAX_CHAT_MESSAGE)
				{
					keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
				}

				m_bCaretCursel = FALSE;
			}
			else
			{
				// 2006-09-28 by ispark 특수 채팅 타입 남기기
				CheckChatTypeMsg(wParam, lParam);
			}
			/*--------------------------------------------------------------------------*/
			SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
			strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
			//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_INPUT_CHAT, g_pShuttleChild->m_vPos, FALSE);
			if (TRUE == bFlag)
				return INF_MSGPROC_BREAK;
		}
	}
	break;
	case WM_IME_NOTIFY:
	{
		// 2006-03-24 by ispark, 일반 설정도 여기서 한다.
		if (INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
			return INF_MSGPROC_BREAK;
	}

	default: break;
	}

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
int CINFGameMainChat::WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WndProcWhisperChat(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
	{
		return INF_MSGPROC_BREAK;
	}
	if (m_pSystemMsgW->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
	{
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;
}
//
//void CINFGameMainChat::OnSoundButtonClick()
//{
//	FLOG("CINFGameMainChat::OnSoundButtonClick()");
//	//		if( g_pShuttleChild->m_pClientParty->GetNtoNCallState() == FALSE &&
//	//			g_pGameMain->m_pCommunity->GetGuild()->GetNtoNCallState() == FALSE)
//	if (g_pGameMain->GetVoiceType() == VOICE_NONE)
//	{
//		if (!m_strInputMessage[0])
//		{
//			((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0018, _Q_VOIP_NAME);//"음성교신할 상대방 아이디를 입력하십시요."
//			g_pD3dApp->m_VOIPState.nVOIPState = _VOIP_TRY_REGISTER;	// 이름 물어보는 동안 요청하지 못하게 한다.
//		}
//		else
//		{
//		}
//	}
//
//	else//1:통화 시도 중지 2:통화 종료
//	{
//		if (g_pGameMain->GetVoiceType() == VOICE_PARTY)
//		{
//			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0012, _Q_VOIP_USING);//"편대 음성통화를 하고 있습니다"
//		}
//		else if (g_pGameMain->GetVoiceType() == VOICE_GUILD)
//		{
//			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0013, _Q_VOIP_USING);//"여단 음성통화를 하고 있습니다"
//		}
//		else if (g_pGameMain->GetVoiceType() == VOICE_ONE)
//		{
//			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_VOIP_0011, _Q_VOIP_USING);//"1:1 음성통화를 하고 있습니다"
//		}
//	}
//
//}

BOOL CINFGameMainChat::IsValidTabIndex(int i_nTabIdx)
{
	if (0 <= i_nTabIdx
		&& CHAT_TAB_NUMBER_ALL > i_nTabIdx)
	{
		return TRUE;
	}
	return FALSE;
}

void CINFGameMainChat::ProcessWisperChatAddLine(const char* strBuffer, DWORD i_dwColorType, const char* szWisperToCharacterName, const char *szWisperFromCharacterName, BYTE NameColor)
{
	if (!strBuffer ||
		!szWisperToCharacterName ||
		!szWisperFromCharacterName)
	
		return;
	

	///////////////////////////////////////////////////////////////////////////////
	// 자신이 전송한 채팅 처리 - 팝업창을 찾으면 채팅 버퍼에 추가하고 없으면 처리가 필요없음
	if (0 == strncmp(szWisperFromCharacterName, g_pShuttleChild->m_myShuttleInfo.CharacterName, SIZE_MAX_CHARACTER_NAME))
	{
		vectorINFGameMainWisperChatPtr::iterator it(m_vectorINFiGameMainWisperChatPtr.begin());
		while (it != m_vectorINFiGameMainWisperChatPtr.end())
		{
			if (0 == _stricmp(szWisperToCharacterName, (*it)->m_szWisperCharacterName))
			{
				(*it)->AddBuffer(strBuffer, i_dwColorType);
				break;
			}
			it++;
		}
		return;
	}

	// 2006-11-07 by ispark, 거절 리스트에 있는 사람이 나한테 걸었을 때

	for (auto& reject : g_pGameMain->m_pCommunity->GetReject()->m_vecReject)

		if (0 == _stricmp(szWisperFromCharacterName, reject.szRejectName))
		
			return;


	///////////////////////////////////////////////////////////////////////////////
	// 다른 사람이 전송한 1:1 채팅 처리
	CINFGameMainWisperChat *pWisperChat = nullptr;

	for (auto ptr : m_vectorINFiGameMainWisperChatPtr)
	{
		if (0 == _stricmp(szWisperFromCharacterName, ptr->m_szWisperCharacterName))
		{
			pWisperChat = ptr;

			break;
		}
	}

	if (!pWisperChat)
	{
		pWisperChat = new CINFGameMainWisperChat(this);

		if (NameColor != 0)
		{
			auto hexcol = ColorConvert8toHexStr(NameColor);

			strcpy(pWisperChat->m_szWisperCharacterNameForRender, hexcol.data());

			strcat(pWisperChat->m_szWisperCharacterNameForRender, szWisperFromCharacterName);

			strcat(pWisperChat->m_szWisperCharacterNameForRender, hexcol.data());
		}

		else strncpy(pWisperChat->m_szWisperCharacterNameForRender, szWisperFromCharacterName, SIZE_MAX_CHARACTER_NAME);

		strncpy(pWisperChat->m_szWisperCharacterName, szWisperFromCharacterName, SIZE_MAX_CHARACTER_NAME);

		pWisperChat->SetGameData(m_pGameData);
		pWisperChat->InitDeviceObjects();
		pWisperChat->RestoreDeviceObjects();
		pWisperChat->m_nVectorIndex = WhisperChatSmallWindowCountPlus();
		if (g_pGameMain->m_pCommunity->GetFriend()->IsWisperChatSend())
		{
			pWisperChat->m_bShowChatBox = FALSE;
			pWisperChat->m_bChatMode = TRUE;
			g_pGameMain->m_pCommunity->GetFriend()->SetWisperChatSend(FALSE);
		}

		m_vectorINFiGameMainWisperChatPtr.push_back(pWisperChat);
		sort(m_vectorINFiGameMainWisperChatPtr.begin(), m_vectorINFiGameMainWisperChatPtr.end(), sort_CINFGameMainWisperChat());
	}

	pWisperChat->AddBuffer(strBuffer, i_dwColorType);
	pWisperChat->m_bNewMassage = TRUE;
}

void CINFGameMainChat::SetWisperBoxMinimumStatusInitPosition(int i_nPosX, int i_nPosY)
{
	m_nWisperBoxMinimumStatusInitPositionX = i_nPosX;
	m_nWisperBoxMinimumStatusInitPositionY = i_nPosY;
}

int CINFGameMainChat::ChangeCurrentLineIndex(int CurrentScrollHeight, int ChatBoxHeight, int NumberOfLine, int ScrollbarLength)
{
	int tempBuff = 0;
	int ResultLine = 0;

	ResultLine = CurrentScrollHeight * (NumberOfLine - 1) / ScrollbarLength;
	return ResultLine;
}

void CINFGameMainChat::ViewChatMenuList()
{
	// 보이고자 하는 메뉴 리스트
	// 1. 시스템
	// 2. 대전
	// 3. 편대
	// 4. 여단
	// 5. 매매
	// 6. 일반
	// 7. VOIP						// 2005-10-10 by ispark
	// 8. FAQ
	// 9. 유료 아이템 구입
	//
	// 5번까지 기본 메뉴 6,7번은 선택 메뉴
	// 리스트 추가는 i 으로 한다
	{
		m_pNChatVOIPStartMenu[m_nVoiceMenu]->Move(CHAT_MENU_LIST_START_X,
			CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER)));
		m_pNChatVOIPStartMenu[m_nVoiceMenu]->Render();
	}

	// 기본 채팅 메뉴 리스트 6개
	// 2007-08-07 by bhsohn 스피커 아이템 추가
	m_pNChatMenu[CHAT_TAB_SPEAKER]->Move(CHAT_MENU_LIST_START_X, CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER - CHAT_TAB_SPEAKER - 1)));
	m_pNChatMenu[CHAT_TAB_SPEAKER]->Render();
	if (m_bChatCurrentSelect[CHAT_TAB_SPEAKER] == TRUE)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pNChatCheck->Move(CHAT_MENU_LIST_START_X + 9, CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER - CHAT_TAB_SPEAKER - 1)));
		m_pNChatCheck->Render();
#else
		m_pNChatCheck->Move(CHAT_MENU_LIST_START_X + 10, CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER - CHAT_TAB_SPEAKER - 1)));
		m_pNChatCheck->Render();
#endif
	}




	int i;
	//for(int i=0; i<CHAT_TAB_NUMBER; i++)
	for (i = CHAT_TAB_NORMAL; i < CHAT_MENULIST_NUMBER; i++)
	{
		//m_pNChatMenu[i]->Move(CHAT_MENU_LIST_START_X, CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)));
		m_pNChatMenu[i]->Move(CHAT_MENU_LIST_START_X, CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H *(CHAT_MENULIST_NUMBER - i)));
		m_pNChatMenu[i]->Render();
		if (m_bChatCurrentSelect[i] == TRUE)
		{
			//m_pNChatCheck->Move(CHAT_MENU_LIST_START_X+10, CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)));
			m_pNChatCheck->Move(CHAT_MENU_LIST_START_X + 10, CHAT_MENU_LIST_START_Y - (CHAT_MENU_LIST_H *(CHAT_MENULIST_NUMBER - i)));
			m_pNChatCheck->Render();
		}
	}

	{
		//		int nSPPosTab = GetRealPos(CHAT_TAB_SPEAKER);	
		//		m_pNChatMenu[nSPPosTab]->Move(CHAT_MENU_LIST_START_X, CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)));
		//		m_pNChatMenu[nSPPosTab]->Render();			
		//		if(m_bChatCurrentSelect[CHAT_TAB_SPEAKER] == TRUE)
		//		{
		//			m_pNChatCheck->Move(CHAT_MENU_LIST_START_X+10, CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+1)));
		//			m_pNChatCheck->Render();
		//		}		
		//		i++;
	}
	// end 2007-08-07 by bhsohn 스피커 아이템 추가

	// 추가 메뉴 리스트
	// VOIP	// 2005-10-10 by ispark
//	if(g_pD3dApp->m_VOIPState.nVOIPState == _VOIP_NOTLOGIN)

//	else if(g_pD3dApp->m_VOIPState.nVOIPState != _VOIP_NOTLOGIN)
//	{
//		m_pNChatVOIPEndMenu[m_nVoiceMenu]->Move(CHAT_MENU_LIST_START_X, 
//			CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(CHAT_MENULIST_NUMBER)));
//		m_pNChatVOIPEndMenu[m_nVoiceMenu]->Render();
//	}

	// 2007-07-30 by bhsohn FAQ버튼 제거
//	 FAQ
//		m_pNChatFAQ[m_nOtherMenuList[1]]->Move(CHAT_MENU_LIST_START_X, 
//			CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+2)));
//		m_pNChatFAQ[m_nOtherMenuList[1]]->Render();
	// end 2007-07-30 by bhsohn FAQ버튼 제거

//#ifndef _TEST_SERVER	

//	m_pNChatCash[m_nOtherMenuList[1]]->Move(CHAT_MENU_LIST_START_X, 
//		CHAT_MENU_LIST_START_Y-(CHAT_MENU_LIST_H*(i+2)));
//	m_pNChatCash[m_nOtherMenuList[1]]->Render();	
//#endif
}

void CINFGameMainChat::EnterCashShop()
{
	BOOL	bFlag = FALSE;
	if (!m_bCashShopOpen)
	{
		if (g_pD3dApp->m_dwGameState == _GAME)
		{
			// 2006-06-26 by ispark, 도시,필드 모두 불필요 하다는 기획에 따라서 메세지 창을 지움
//			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_SHOP_0014, //"몬스터의 공격을 받을수있습니다 상점에 들어가시겠습니까?"
//			_Q_CASHITEM_SHOP_OPEN);
			m_bCashShopOpen = TRUE;
			//			EnterCashShop();

		}
		else
			m_bCashShopOpen = TRUE;

		if (m_bCashShopOpen)
			bFlag = TRUE;
	}
	else
	{
		bFlag = TRUE;
	}

	if (bFlag)
	{
		//DBGOUT("SEND--------------CASH_SHOP_BUILDING_INDEX\n");

		if (g_pD3dApp->m_dwGameState != _CITY)
		{
			g_pD3dApp->SendFieldSocketCityGetBuildingList(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex);
		}
		GUI_BUILDINGNPC* pBuilding = g_pInterface->m_pCityBase->FindBuildingNPC(CASH_SHOP_BUILDING_INDEX);
		if (pBuilding)
		{
			BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
			if (pInfo)
			{
				memcpy(&pBuilding->buildingInfo, pInfo, sizeof(BUILDINGNPC));
			}

			// 2006-06-01 by ispark, 모든 창 닫는다.
			g_pGameMain->InitShowWindow();
			// 2006-06-02 by ispark, 상점 직접 셋팅
			g_pD3dApp->ChangeGameState(_SHOP);

			g_pInterface->m_pCityBase->CurrentChangeGuiBuilding(pBuilding);
			g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_MAIN);
			g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-07-07 by ispark, 메세지 응답을 기다린다.
		}
		else
		{
			m_bCheckShopInfo = TRUE;
		}
		m_bCashShopOpen = FALSE;
	}
}

void CINFGameMainChat::CheckShopInfo()
{
	GUI_BUILDINGNPC* pBuilding = g_pInterface->m_pCityBase->FindBuildingNPC(CASH_SHOP_BUILDING_INDEX);
	if (pBuilding)
	{
		BUILDINGNPC* pInfo = g_pDatabase->GetServerBuildingNPCInfo(pBuilding->buildingInfo.BuildingIndex);
		if (pInfo)
		{
			memcpy(&pBuilding->buildingInfo, pInfo, sizeof(BUILDINGNPC));
		}
		// 2006-06-01 by ispark, 모든 창 닫는다.
		g_pGameMain->InitShowWindow();
		// 2006-06-02 by ispark, 상점 직접 셋팅
		g_pD3dApp->ChangeGameState(_SHOP);

		g_pInterface->m_pCityBase->CurrentChangeGuiBuilding(pBuilding);
		g_pInterface->m_pCityBase->OnCityNPCButtonDown(CITY_NPC_BUTTON_MAIN);
		g_pD3dApp->m_bRequestEnable = FALSE;			// 2006-07-07 by ispark, 메세지 응답을 기다린다.
		m_bCheckShopInfo = FALSE;
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainChat::SetCleanChat()
/// \brief		채팅 불활성과 내용 클리어
/// \author		ispark
/// \date		2005-11-16	
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::SetCleanChat()
{
	if (m_bChatMode)
	{
		m_bChatMode = FALSE;
		g_pD3dApp->m_bChatMode = FALSE;
		g_pD3dApp->CleanText();

		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
		InitChatMsgBuff();
		// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

		memset(m_strInputMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainChat::RenderWisperChat()
/// \brief		채팅창 귓말 렌더링
/// \author		ispark
/// \date		2006-08-01 ~ 2006-08-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::RenderWisperChat()
{
	if (m_nSmallWindowCount > 0)
	{
		m_pImgWisperBtn[m_bShowMiniWisper][m_bShowMiniWisperUnder]->Move(WISPERBOX_MINIMUM_WINDOW_BUTTON_X, WISPERBOX_MINIMUM_WINDOW_BUTTON_Y);
		m_pImgWisperBtn[m_bShowMiniWisper][m_bShowMiniWisperUnder]->Render();
	}

	if (!m_bShowMiniWisper) return;
		
	SetWisperBoxMinimumStatusInitPosition(
		WISPERBOX_MINIMUM_WINDOW_START_X,
		WISPERBOX_MINIMUM_WINDOW_START_Y);

	if (m_nClickedWisperChatBoxVectorIndex >= 0 &&
		m_nClickedWisperChatBoxVectorIndex < m_vectorINFiGameMainWisperChatPtr.size())
	{
		EnterCriticalSection(&g_pD3dApp->m_cs);

		auto itwi = m_vectorINFiGameMainWisperChatPtr.begin();
		while (itwi != m_vectorINFiGameMainWisperChatPtr.end())
		{
			auto pWisper = *itwi;
			if (pWisper->m_bPresentClickedFlag)
			{
				pWisper->m_nVectorIndex = 0;
				pWisper->SetPresentClickedFlag(TRUE);
			}
			++itwi;
		}

		LeaveCriticalSection(&g_pD3dApp->m_cs);
		//end 2010. 03. 16 by jskim 귓말창 버그 수정

		m_nClickedWisperChatBoxVectorIndex = -1;

		sort(
			m_vectorINFiGameMainWisperChatPtr.begin(),
			m_vectorINFiGameMainWisperChatPtr.end(),
			sort_CINFGameMainWisperChat());
	}

	auto rit = m_vectorINFiGameMainWisperChatPtr.rbegin();
	while (rit != m_vectorINFiGameMainWisperChatPtr.rend())
	{
		auto pWisper = *rit;
		pWisper->Render();
		++rit;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainChat::SetWisperChatShowInterface(BOOL i_bInterface)
/// \brief		귀속말 창 일괄적으로 보이도록, 또는 안보이도록 처리.
/// \author		dgwoo
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::SetWisperChatShowInterface(BOOL i_bInterface)
{
	CINFGameMainWisperChat *pWisper = NULL;
	vectorINFGameMainWisperChatPtr::iterator itwi(m_vectorINFiGameMainWisperChatPtr.begin());
	while (m_vectorINFiGameMainWisperChatPtr.end() != itwi)
	{
		pWisper = (*itwi);
		pWisper->SetInterface(i_bInterface);
		itwi++;
	}
}
void CINFGameMainChat::RenderMacro()
{
	if (!m_bShowMacro)
		return;
	m_pChatMacroBG->Move(m_nMacroX, m_nMacroY);
	m_pChatMacroBG->Render();
	int i;
	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		if (m_nActMacro == i)
		{
			char chatbuf[SIZE_MAX_CHAT_MESSAGE + 2];
			memset(chatbuf, 0x00, SIZE_MAX_CHAT_MESSAGE);
			strncpy(chatbuf, m_strTempMacro, SIZE_MAX_CHAT_MESSAGE);
			chatbuf[strlen(m_strTempMacro)] = '_';
			chatbuf[strlen(m_strTempMacro) + 1] = '\0';
			m_pFontInput->SetTextureWidth(CHAT_MACRO_INPUT_W);
			m_pFontInput->DrawText(CHAT_MACRO_INPUT_1_X, CHAT_MACRO_INPUT_1_Y + (CHAT_MACRO_APPL_GAP_Y*i), GUI_FONT_COLOR, chatbuf, 0L);
		}
		else if (m_sMacroStr[i].bAppl)
		{
			m_pFontInput->DrawText(CHAT_MACRO_INPUT_1_X, CHAT_MACRO_INPUT_1_Y + (CHAT_MACRO_APPL_GAP_Y*i), GUI_FONT_COLOR, m_sMacroStr[i].strMacro, 0L);
		}

		m_pBtnChatMacro[i]->SetBtnPosition(CHAT_MACRO_APPL_1_X, CHAT_MACRO_APPL_1_Y + (CHAT_MACRO_APPL_GAP_Y * i));
		m_pBtnChatMacro[i]->Render();

	}
	m_pBtnMacroOK->SetBtnPosition(CHAT_MACRO_OK_X, CHAT_MACRO_OK_Y);
	m_pBtnMacroOK->Render();
	m_pBtnMacroCancel->SetBtnPosition(CHAT_MACRO_CANCEL_X, CHAT_MACRO_CANCEL_Y);

	m_pBtnMacroCancel->Render();


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFGameMainChat::CheckChatTypeMsg(WPARAM wParam, LPARAM lParam)
/// \brief		마지막에 쓴 채팅 종류에 따라 셋팅   used for remember last color thingy
/// \author		ispark / panoskj
/// \date		2006-09-28 / 2017-01-12	
///////////////////////////////////////////////////////////////////////////////
BOOL CINFGameMainChat::CheckChatTypeMsg(WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_RETURN &&
		(ChatModeChack(m_strInputMessage[0]) || (_REMEMBER_CHAT_COLOR && m_strInputMessage[0] == '\\')))
	{
		wchar_t wTmName[16] { };

		MultiByteToWideChar(g_input.GetCodePage(), 0, m_strInputMessage, strlen(m_strInputMessage), wTmName, 16);

		for (int j = 0; j < wcslen(wTmName); j++)
		
			g_input.SetInputWideChar(wTmName[j]);
		
		g_pD3dApp->m_inputkey.SetGameText(g_pD3dApp->m_inputkey.GetInputLanguage());

		return TRUE;
	}
	return FALSE;
	//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		도움말 버튼 클릭
/// \author		// 2007-07-04 by bhsohn 오퍼레이터 추가
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::OnClickFAQ()
{
	// 2008-10-20 by bhsohn FAQ 기체 상태에서는 안보이게 변경
	if (!g_pGameMain->m_bFAQProsFlag)
	{
		if (g_pTutorial->IsTutorialMode() == TRUE ||
			!g_pShuttleChild->GetIsUseInterface() ||
			g_pGameMain->m_bVOIPFlag == TRUE ||
			g_pD3dApp->m_bChatMode == TRUE ||
			g_pD3dApp->m_dwGameState == _SHOP ||
			g_pGameMain->m_nLeftWindowInfo == LEFT_WINDOW_TRANS ||
			g_pGameMain->m_pTrade->m_bTrading)
		{
			return;
		}
	}
	// end 2008-10-20 by bhsohn FAQ 기체 상태에서는 안보이게 변경

	g_pInterface->SetWindowOrder(WNDGameMainFaq);
	g_pGameMain->m_bFAQProsFlag = !g_pGameMain->m_bFAQProsFlag;
	//	if(g_pGameMain->m_bFAQProsFlag)
	//	{
	//		g_pD3dApp->m_bFixUnitDirMode = TRUE;	// 유닛 방향 고정 모드
	//	}
	//	else 
	//	{
	//		g_pD3dApp->m_bFixUnitDirMode = FALSE;	// 유닛 방향 고정 모드
	//	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-02-27 by bhsohn 아레나 이동시 채팅정보 뺵�?
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER])
{
	m_ArrChatTab.GetAllChatList(pArrChatLine);

}
INT CINFGameMainChat::WhisperChatSmallWindowCountPlus()
{
	INT nWindowCnt = m_nSmallWindowCount;
	m_nSmallWindowCount++;
	if (nWindowCnt < 0)
		return 0;
	//DBGOUT("WhisperChatSmallWindowCountPlus(m_nSmallWindowCount = %d)\n",m_nSmallWindowCount);
	return nWindowCnt;
}
INT CINFGameMainChat::WhisperChatSmallWindowCountMinus(INT i_nIndex)
{
	INT nWindowCnt = m_nSmallWindowCount;

	CINFGameMainWisperChat *pWisper = NULL;
	vectorINFGameMainWisperChatPtr::iterator it(m_vectorINFiGameMainWisperChatPtr.begin());
	// 2010. 03. 16 by jskim 귓말창 버그 수정
	EnterCriticalSection(&g_pD3dApp->m_cs);
	//end 2010. 03. 16 by jskim 귓말창 버그 수정
	while (it != m_vectorINFiGameMainWisperChatPtr.end())
	{
		pWisper = (*it);
		if (pWisper->m_nVectorIndex > i_nIndex)
		{
			pWisper->m_nVectorIndex--;
		}
		it++;
	}
	// 2010. 03. 16 by jskim 귓말창 버그 수정
	LeaveCriticalSection(&g_pD3dApp->m_cs);
	//end 2010. 03. 16 by jskim 귓말창 버그 수정

	m_nSmallWindowCount--;
	//DBGOUT("WhisperChatSmallWindowCountMinus(m_nSmallWindowCount = %d)\n",m_nSmallWindowCount);
	return nWindowCnt;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD CINFGameMainChat::ChangeChatMode(DWORD i_nChatMode)
/// \brief		활성화중인 채팅 모드를 변경.
/// \author		dgwoo
/// \date		2008-05-09 ~ 2008-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CINFGameMainChat::ChangeChatMode(DWORD i_nChatMode)
{
	DWORD oldChatMode = m_nChatMode;
	char strChatType = '\0';
	m_nChatMode = i_nChatMode + 1;
	switch (m_nChatMode)
	{
	case CHAT_TAB_ALLUSER:
		break;
	case CHAT_TAB_WAR:
		strChatType = '^';
		break;
	case CHAT_TAB_TRADE:
		strChatType = '$';
		break;
	case CHAT_TAB_MAP:
		strChatType = '!';
		break;
	case CHAT_TAB_GUILD:
	{
		strChatType = '@';
		if (g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber == 0 &&
			m_bGuildQuick == FALSE)
		{
			m_bGuildQuick = TRUE;
			char strtemp[SIZE_MAX_ERROR_CHAT_MESSAGE] = { 0, };
			wsprintf(strtemp, STRMSG_C_080605_0101, STRMSG_WINDOW_TEXT, STRMSG_WINDOW_TEXT);
			m_ArrChatTabMode[CHAT_TAB_GUILD].PushChatLine(strtemp, COLOR_ITEM);
			m_ArrChatTabMode[CHAT_TAB_GUILD].PushChatLine("  ", COLOR_ITEM);
			m_ArrChatTabMode[CHAT_TAB_GUILD].PushChatLine("  ", COLOR_ITEM);
		}
	}
	break;
	case CHAT_TAB_PARTY:
	{
		strChatType = '#';
		if (g_pShuttleChild->m_pClientParty->IsParty() == FALSE	&&
			m_bPartyQuick == FALSE)
		{
			m_bPartyQuick = TRUE;
			char strtemp[SIZE_MAX_ERROR_CHAT_MESSAGE] = { 0, };
			wsprintf(strtemp, STRMSG_C_080605_0100, STRMSG_WINDOW_TEXT, STRMSG_WINDOW_TEXT);
			m_ArrChatTabMode[CHAT_TAB_PARTY].PushChatLine(strtemp, COLOR_ITEM);
			m_ArrChatTabMode[CHAT_TAB_PARTY].PushChatLine("  ", COLOR_ITEM);
			m_ArrChatTabMode[CHAT_TAB_PARTY].PushChatLine("  ", COLOR_ITEM);
		}
	}
	break;
	case CHAT_TAB_ARENA:
		strChatType = '%';
		break;
	case CHAT_TAB_CHATROOM:
		strChatType = '&';
		break;
	default:
		break;

	}
	//	if(strChatType != '\0')
	{
		g_pD3dApp->m_inputkey.m_full_str[0] = strChatType;
		g_pD3dApp->m_inputkey.m_str_pos = 1;
		m_strInputMessage[0] = strChatType;
	}

	return oldChatMode;
}
ChatTab_t * CINFGameMainChat::GetChatTabMode()
{
	if (m_nChatMode == CHAT_TAB_ALLUSER)
	{
		return &m_ArrChatTab;
	}
	return &m_ArrChatTabMode[m_nChatMode];
}

void CINFGameMainChat::ActiveChatOption(BOOL i_bActive)
{
	switch (m_nChatMode)
	{
	case CHAT_TAB_TRADE:
	{
		m_bShowMacro = i_bActive;
		if (m_bShowMacro)
		{
			g_pInterface->SetWindowOrder(WNDMacroWnd);
		}
	}
	break;
	case CHAT_TAB_GUILD:
	{
		if (g_pShuttleChild->GetMyShuttleInfo().GuildUniqueNumber != 0)
		{
			g_pGameMain->m_pCommunity->ShowCommunityWindow(TRUE, COMMUNITY_GUILD);
		}
	}
	break;
	case CHAT_TAB_PARTY:
	{
		if (g_pShuttleChild->m_pClientParty->IsParty() == TRUE)
		{
			g_pGameMain->m_pCommunity->ShowCommunityWindow(TRUE, COMMUNITY_PARTY);
		}
	}
	break;
	default:
		break;
	}

	return;
}
void CINFGameMainChat::StartMacro()
{
	m_bMacroIng = TRUE;
	m_nSendMacroNum = 0;

	// 2008-08-18 by bhsohn 매크로 추가 수정
	m_nTransChatCount = CHATMACRO_TRANSCHAT_COUNT;

	int i;
	for (i = 0; i < CHAT_MACRO_INPUT_COUNT; i++)
	{
		if (m_sMacroStr[i].bAppl == TRUE)
		{
			m_sMacroStr[i].fTime = 0;// 2012-03-30 by isshin 매크로 시스템 버그 수정 - 매크로 작동시 바로 출력

			// 시작 위치 지정
			m_nSendMacroNum = i;
			break;
		}
	}
}
void CINFGameMainChat::EndMacro()
{
	m_bMacroIng = FALSE;
	m_nSendMacroNum = 0;
}
//void CINFGameMainChat::SetVoiceChatVolum(DWORD i_nVolum)
//{// 볼륨 조절하는 함수를 계속 호출하면 속도가 많이 느려짐  제어할수있도록 구현.
//	if (m_fVolumConTimeGap > VOICECHAT_SPK_CONTROL_TIME_GAP)
//	{
//		m_fVolumConTimeGap = 0;
//		g_pGameMain->SetVoiceVolum(i_nVolum);
//	}
//	m_fVolumConTimeGap += g_pD3dApp->GetElapsedTime();
//	m_nVolum = i_nVolum;
//}
void CINFGameMainChat::SetChatRoomAct(BOOL i_bActive)
{
	m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_CHATROOM)]->EnableBtn(i_bActive);
}
void CINFGameMainChat::SetChatArenaAct(BOOL i_bActive)
{
	m_pBtnChatMode[ChangeChatButtonNumber(CHAT_TAB_ARENA)]->EnableBtn(i_bActive);
}

BOOL CINFGameMainChat::IsChatingMode()
{
	return m_bChatMode;
}


// 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장
stcuctRateRect CINFGameMainChat::GetChatWndRect(void)
{
	stcuctRateRect	stRateRect;
	memset(&stRateRect, 0x00, sizeof(stcuctRateRect));

	stRateRect.fRateWidth = m_nChatBoxWidth;
	stRateRect.fRateHeight = m_nChatBoxHeight;

	return stRateRect;
}
// end 2009. 02. 03 by ckPark 채팅창 인터페이스 크기 저장


// 2008. 12. 29 by ckPark 채팅창 최적화
///////////////////////////////////////////////////////////////////////////////
/// \fn		: CINFGameMainChat::BuildCulledString
/// \brief	: 각 라인별로 잘려진 문자열을 생성
/// \author	: ckPark
/// \date	: 29:12:2008   20:55
/// \warning: 
/// \param	: void
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CINFGameMainChat::BuildCulledString(void)
{
	// 일단 리스트 초기화
	m_listCulledString.clear();

	// 최대 렌더링 라인수 계산
	int nMaxRenderLineCounts = (m_nChatBoxHeight - CHATBOX_IMAGE_GAB_HEITHT_TOP)
		/ CHAT_FONT_LINE_HEIGHT;

	// 현재 탭 얻어오기
	ChatTab_t *pCurChatTab = GetChatTabMode();
	// 탭에 저장된 문자열이 없을 경우 리턴
	if (0 >= pCurChatTab->m_nStoredLineCounts)
		return;

	int nRenderIndex = pCurChatTab->m_nRenderStartIndex;
	int nRenderedLineCounts = 0;

	vector<CD3DHanFont*>::iterator it = m_vecFontLine.begin();
	while (it != m_vecFontLine.end())
	{
		CD3DHanFont* pFont = *it;


		// 각 한줄을 짤라서 벡터로 나눠서 저장한다
		vector<string> vecChatMessage;
		STRING_CULL(pCurChatTab->GetChatLine(nRenderIndex), CHAT_STRING_LENGTH, &vecChatMessage, pFont);
		m_listCulledString.push_back(vecChatMessage);

		nRenderedLineCounts += vecChatMessage.size();

		// 저장한도나 최대 렌더링 갯수 넘어가면 루프 탈출
		if (nRenderIndex == pCurChatTab->m_nStoreEndIndex
			|| nMaxRenderLineCounts <= nRenderedLineCounts)
			break;

		// 원형큐 인덱스 설정
		if (--nRenderIndex < 0)
			nRenderIndex = CHAT_BUFFER_NUMBER - 1;
		++it;
	}
}
// end 2008. 12. 29 by ckPark 채팅창 최적화

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMainChat::OnKeyUpFunc(WPARAM wParam, LPARAM lParam)
{
	if (!IsChatingMode())
	{
		return INF_MSGPROC_NORMAL;
	}
	char text[SIZE_MAX_STRING_1024];
	ZERO_MEMORY(text);

	switch (wParam)
	{
	case VK_SHIFT:
	{
		m_bShiftClick = FALSE;
		//DBGOUT("m_bCaretCursel = FALSE; \n");
	}
	break;
	case VK_CONTROL:
	{
		int strlenComp = g_input.GetComp(text, sizeof(text));
		if (strlenComp > 0)
		{
			// 압축중이넨 리턴
			return INF_MSGPROC_NORMAL;
		}
		m_bControlCursel = FALSE;

	}
	break;
	}
	return INF_MSGPROC_NORMAL;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMainChat::OnKeyDownCursel(WPARAM wParam, LPARAM lParam)
{
	if (!IsChatingMode()) return INF_MSGPROC_NORMAL;

	switch (wParam)
	{
	case VK_CONTROL:
	{
		auto nRace = g_pD3dApp->m_pShuttleChild->GetMyShuttleInfo().Race;

		if (!C_USER_CTRL_KEY_LOCK || COMPARE_RACE(nRace, RACE_OPERATION) || COMPARE_RACE(nRace, RACE_GAMEMASTER))
		{
			m_bControlCursel = TRUE;

			return INF_MSGPROC_BREAK;
		}

		break;
	}
	case VK_SHIFT:

		if (INF_MSGPROC_BREAK == OnKeyDownFunc(wParam, lParam))
			
			return INF_MSGPROC_BREAK;
		
		return INF_MSGPROC_NORMAL;
	
	default:
		
		break;
	}


	char text[1024] { };

	auto strlenComp = g_input.GetComp(text, sizeof(text));
	
	if (strlenComp > 0) return INF_MSGPROC_NORMAL;


	switch (wParam)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_HOME:
	case VK_END:
	{
		if (!m_bShiftClick)
		{
			// 쉬프트가 클릭 안되어있다.
			m_bCaretCursel = FALSE;
		}
		else
		{
			// 쉬프트가 클릭 되어있다.
			m_bCaretCursel = TRUE;
		}
		return OnKeyDownArrow(wParam, lParam);
	}
	break;
	case VK_BACK:
	case VK_DELETE:
	{
		// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
		// 			if(!m_bShiftClick)
		// 			{
		// 				// 쉬프트가 클릭 안되어있다.
		// 				m_bCaretCursel = FALSE;
		// 			}
		// 			// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		// 			else 
		// 			{
		// 				// 쉬프트가 클릭 되어있다.
		// 				m_bCaretCursel = TRUE;
		// 			}
		// 			// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
		return OnKeyDownFunc(wParam, lParam);
	}
	break;
	case 'c':
	case 'C':
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			if (m_bControlCursel)
			{
				char chCurselSting[SIZE_MAX_CHAT_MESSAGE];
				ZERO_MEMORY(chCurselSting);
				GetCurselString(chCurselSting);
				if (strlen(chCurselSting) > 0)
				{
					SetClipBoard(chCurselSting);
					// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
					m_ptSelCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				}
				// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				m_bCaretCursel = FALSE;
				//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				return INF_MSGPROC_BREAK;
			}
		}
	}
	break;
	case 'a':
	case 'A':
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			if (m_bControlCursel && strlen(m_strBkInputMessage) > 0)
			{
				m_bCaretCursel = TRUE;

				// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				// 채팅모드를 재외한 블럭 지정
				// 				m_ptCurselPos.x = m_ptCurselPos.y = 0;
				// 				m_ptSelCurselPos.x = GetStringBuffLen(m_strBkInputMessage);
				// 				
				// 				memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);		// IME초기화 전에 값을 가지고 있는다.
				// 				memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	
				// 				
				// 				strncpy(m_strNextBackupMessage, m_strBkInputMessage, (SIZE_MAX_CHAT_MESSAGE));
				// 				
				// 				g_pD3dApp->CleanText();
				// 				
				// 				// 갱신 버퍼 초기화
				// 				SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);			
				// 				
				// 				// 마지막 버퍼에 저장
				// 				strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);


				if (ChatModeChack(m_strBkInputMessage[0]))
				{
					m_ptSelCurselPos.x = m_ptSelCurselPos.y = GetStringBuffPos(m_strBkInputMessage, 1);
				}
				else
				{
					m_ptSelCurselPos.x = m_ptSelCurselPos.y = 0;
				}
				m_ptCurselPos.x = GetStringBuffLen(m_strBkInputMessage);
				// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안

				memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);		// IME초기화 전에 값을 가지고 있는다.
				memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	

				// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				strncpy(m_strPreBackupMessage, m_strBkInputMessage, (SIZE_MAX_CHAT_MESSAGE));
				// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안

				g_pD3dApp->CleanText();

				// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
				//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화				
				return INF_MSGPROC_BREAK;
			}
		}
	}
	break;
	// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 잘라내기 기능 구현
	case 'X':
	case 'x':
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			if (m_bControlCursel)
			{
				char chCurselSting[SIZE_MAX_CHAT_MESSAGE];
				ZERO_MEMORY(chCurselSting);
				GetCurselString(chCurselSting);
				if (strlen(chCurselSting) > 0)
				{
					SetClipBoard(chCurselSting);

					if (m_bCaretCursel == TRUE)
					{
						// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
						// 						if(m_ptSelCurselPos.x != -1)
						// 						{
						// 							int  chLenTmp=0;
						// 							chLenTmp = m_ptCurselPos.x-m_ptSelCurselPos.x;						
						// 							BlockCleanChat(chLenTmp);						
						// 						}
						BlockCleanChat(m_ptCurselPos.x, m_ptSelCurselPos.x);
						m_bCaretCursel = FALSE;
						//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
					}



				}
				return INF_MSGPROC_BREAK;
			}
		}
	}
	break;
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	case 'v':
	case 'V':
	{
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			int nIMELen = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);

			// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
			// 블럭 지정 상태에서 다른 문자도 입력되도록 수정
			// 			if(m_bCaretCursel)
			// 			{
			// 				return INF_MSGPROC_NORMAL;
			// 			}
			if (m_bControlCursel)
			{
				// 블럭지정 삭제
				if (m_bCaretCursel == TRUE)
				{
					// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
					// 					if(m_ptSelCurselPos.x != -1)
					// 					{
					// 						int  chLenTmp=0;
					// 						chLenTmp = m_ptCurselPos.x-m_ptSelCurselPos.x;						
					// 						BlockCleanChat(chLenTmp);	
					BlockCleanChat(m_ptCurselPos.x, m_ptSelCurselPos.x);
					//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				}
				// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안		
				if (nIMELen > 0)
				{
					//return INF_MSGPROC_NORMAL;
					g_pD3dApp->CleanText(); // IME버퍼 초기화
				}
				// 커서가 없을때만 수정가능
				char chCurselSting[SIZE_MAX_CHAT_MESSAGE];
				ZERO_MEMORY(chCurselSting);

				// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				// 선택된 커서 초기화 
				//m_ptSelCurselPos.x = -1;
				// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				PasteFromClipBoard(chCurselSting, SIZE_MAX_CHAT_MESSAGE - 1);
				if (strlen(chCurselSting) <= 0)
				{
					return INF_MSGPROC_NORMAL;
				}
				// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				int nStartPos = GetStringBuffPos(m_strBkInputMessage, m_ptCurselPos.x);

				if (-1 == nStartPos)
				{
					return INF_MSGPROC_NORMAL;
				}

				// 				char chTmp[SIZE_MAX_CHAT_MESSAGE];
				// 				ZERO_MEMORY(chTmp);

				if (nStartPos > 0)
				{
					strncpy(m_strPreBackupMessage, m_strBkInputMessage, nStartPos);
				}
				// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				// 				else
				// 				{
				// 					// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				// 					//m_strPreBackupMessage[0] = NULL;
				// 					if(ChatModeChack(m_strInputMessage[0]))
				// 					{	
				// 						strncpy(m_strPreBackupMessage,m_strInputMessage,SIZE_MAX_CHAT_MESSAGE);					
				// 					}
				// 					else
				// 					{
				// 						strncpy(m_strPreBackupMessage, m_strBkInputMessage, nStartPos);
				// 					}
				// 					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				// 				}
				// 
				// 				if((strlen(m_strPreBackupMessage)+strlen(chCurselSting)) < SIZE_MAX_CHAT_MESSAGE )
				// 				{
				// 					strcat(m_strPreBackupMessage, chCurselSting);
				// 				}
				// 				
				// 				strcpy(m_strNextBackupMessage, &m_strBkInputMessage[nStartPos]);
				// 				
				// 				// 갱신 버퍼 초기화
				// 				SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
				// 				
				// 				// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				// 				// 선택된 커서 위치 받아오기
				// 				m_ptCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
				// 				// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				// 				// 마지막 버퍼에 저장
				// 				strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
				// 				if(ChatModeChack(m_strInputMessage[0]) != ChatModeChack(m_strPreBackupMessage[0]))
				// 				{	
				// 					char chrtmp[256];
				// 					memset(chrtmp,0x00,256);
				// 					wsprintf(chrtmp,"%c%s",m_strInputMessage[0],m_strPreBackupMessage);
				// 					strcpy(m_strPreBackupMessage, chrtmp);
				// 				}
				int mLen = strlen(m_strPreBackupMessage) + strlen(chCurselSting) + strlen(m_strNextBackupMessage);
				if (mLen < SIZE_MAX_CHAT_MESSAGE)
				{
					strcat(m_strPreBackupMessage, chCurselSting);
				}
				else
				{
					if (strlen(m_strPreBackupMessage) + strlen(chCurselSting) < SIZE_MAX_CHAT_MESSAGE)
					{
						strcat(m_strPreBackupMessage, chCurselSting);
					}
					else
					{
						int nFindLen = GetStringBuffLen(chCurselSting);
						char chrtmp[256];
						for (int i = 1; i <= nFindLen; i++)
						{
							memset(chrtmp, 0x00, 256);
							int nFindPos = GetStringBuffPos(chCurselSting, nFindLen - i);
							strncpy(chrtmp, chCurselSting, nFindPos);
							if (strlen(m_strPreBackupMessage) + strlen(chrtmp) < SIZE_MAX_CHAT_MESSAGE)
							{
								strcat(m_strPreBackupMessage, chrtmp);
								m_ptCurselPos.x += GetStringBuffLen(chrtmp);
								break;
							}
						}
					}
					if (strlen(m_strPreBackupMessage) + strlen(m_strNextBackupMessage) >= SIZE_MAX_CHAT_MESSAGE)
					{
						int nFindLen = GetStringBuffLen(m_strNextBackupMessage);
						char chrtmp[256];

						for (int i = 1; i <= nFindLen; i++)
						{
							memset(chrtmp, 0x00, 256);
							int nFindPos = GetStringBuffPos(m_strNextBackupMessage, nFindLen - i);
							strncpy(chrtmp, m_strNextBackupMessage, nFindPos);
							if (strlen(m_strPreBackupMessage) + strlen(chrtmp) < SIZE_MAX_CHAT_MESSAGE)
							{
								strcpy(m_strNextBackupMessage, chrtmp);
								m_ptCurselPos.x += GetStringBuffLen(chrtmp);
								break;
							}
						}
					}
				}
				//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
				return INF_MSGPROC_BREAK;
			}
		}

	}
	break;
	}
	return INF_MSGPROC_NORMAL;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		화살표 클릭
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMainChat::OnKeyDownArrow(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_LEFT:
	{
		// 			if(m_bCaretCursel 
		// 				&& (m_ptSelCurselPos.x == m_ptCurselPos.x) 
		// 				&& (m_ptSelCurselPos.y == m_ptCurselPos.y))
		// 			{
		// 				// 처음 커서 지정
		// 				m_ptSelCurselPos.x++;
		// 
		// 				int nStrLen = GetStringBuffLen(m_strBkInputMessage)+1;
		// 				if(m_ptSelCurselPos.x > nStrLen)
		// 				{
		// 					m_ptSelCurselPos.x = nStrLen;
		// 				}			
		// 				
		// 			}
					// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
					// 채팅 모드 문자로 이동 못하게 한다
		if (ChatModeChack(m_strBkInputMessage[0]) && m_ptCurselPos.x < 2)
		{
			m_ptCurselPos.x = 1;
		}
		else
		{
			m_ptCurselPos.x--;
		}
		// 커서 위치 적용(WM_CHAR함수로 이동함)			
// 			int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
// 			if(nIMELen > 0)
// 			{
// 				m_ptCurselPos.x += nIMELen;
// 			}
			// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		if (m_ptCurselPos.x < 0)
		{
			m_ptCurselPos.x = 0;
		}
	}
	break;
	case VK_RIGHT:
	{
		int nStrLen = GetStringBuffLen(m_strBkInputMessage);
		// 			// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		// 			if(m_ptCurselPos.x <= nStrLen - 1 )
		// 			{
		// 				if(2 != nStrPos ||
		// 					nStrCusPos + nStrPos < SIZE_MAX_CHAT_MESSAGE-1)
		// 				{	
		// 						int aa = GetStringBuffPos(m_strBkInputMessage,nStrLen);
		// 						m_strBkInputMessage[aa+1]='\0';
		m_ptCurselPos.x++;
		// 				}
		// 			}
					// 커서가 마지막 위치를 넘었을 경우 자신의 커서 위치를 업데이트 한다.			
				//	int nStrPos = GetStringBuffPos(m_strBkInputMessage, nStrLen - 1);

				//	char nChar[SIZE_MAX_CHAT_MESSAGE];
					//strcpy(nChar,&m_strBkInputMessage[nStrPos]);			

		// 			if(0 > nChar[0] &&
		// 				GetStringBuffPos(m_strBkInputMessage, m_ptCurselPos.x) >=  GetStringBuffPos(m_strBkInputMessage, nStrLen - 1) )
		// 			{
		// 				if(strlen(m_strBkInputMessage) < SIZE_MAX_CHAT_MESSAGE - 1)
		// 				{
		// 					m_ptCurselPos.x++;
		// 				}
		// 				else
		// 				{
		// 					m_ptCurselPos.x = nStrLen - 1;
		// 				}
		// 			}
		// 			else if(m_ptCurselPos.x < nStrLen)
		// 			{
		// 				m_ptCurselPos.x++;
		// 			}
		// 		}

		// 			if(strlen(m_strNextBackupMessage) > 0)
		// 			{
		// 				int nIMELen	 = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
		// 				if(nIMELen > 0)
		// 				{
		// 					m_ptCurselPos.x += nIMELen;
		// 				}
		// 			}
					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		if (m_ptCurselPos.x > nStrLen)
		{
			m_ptCurselPos.x = nStrLen;
		}
	}
	break;
	case VK_HOME:
	{
		// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		// 채팅 모드 문자로 커서를 이동 못하게 한다.
		if (ChatModeChack(m_strBkInputMessage[0]))
		{
			m_ptCurselPos.x = GetStringBuffPos(m_strBkInputMessage, 1);
		}
		else
		{
			m_ptCurselPos.x = 0;
		}
		// 채팅 모드 문자로 커서를 이동 못하게 한다.
		// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	}
	break;
	case VK_END:
	{
		int nStrLen = GetStringBuffLen(m_strBkInputMessage);
		// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		//m_ptCurselPos.x = nStrLen;
		// 커서를 맨끝으로 이동시킬 때 마지막 문자의 값이 쓰레기값이 들어있으면 커서를 그 전으로 이동시킨다.

// 			int nStrCusPos = GetStringBuffPos(m_strBkInputMessage, m_ptCurselPos.x);
// 
// 			if(m_ptCurselPos.x < nStrLen - 1 )
// 			{
// 				if(strlen(m_strBkInputMessage) == SIZE_MAX_CHAT_MESSAGE-1)
// 				{	
// 					m_ptCurselPos.x = nStrLen - 1;		
// 				}
// 				else
		{
			m_ptCurselPos.x = nStrLen;
		}
		//			}
		// 			int nStrPos = GetStringBuffPos(m_strBkInputMessage, nStrLen - 1);
		// 			
		// 			char nChar[SIZE_MAX_CHAT_MESSAGE];
		// 			strcpy(nChar,&m_strBkInputMessage[nStrPos]);			
		// 			
		// 			if(0 > nChar[0] &&
		// 				GetStringBuffPos(m_strBkInputMessage, m_ptCurselPos.x) >=  GetStringBuffPos(m_strBkInputMessage, nStrLen - 1) )
		// 			{
		// 				int aa = strlen(m_strBkInputMessage);
		// 				if(strlen(m_strBkInputMessage) < SIZE_MAX_CHAT_MESSAGE-1)
		// 				{
		// 					m_ptCurselPos.x = nStrLen;
		// 				}
		// 				else
		// 				{
		// 					m_ptCurselPos.x = nStrLen - 1;
		// 				}
		// 				
		// 			}
		// 			else if(m_ptCurselPos.x < nStrLen)
		// 			{
		// 				m_ptCurselPos.x = nStrLen;
		// 			}			
	}
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	break;
	default:
	{
		return INF_MSGPROC_NORMAL;
	}
	break;
	}

	// 	DBGOUT("m_ptCurselPos[%d][%d] m_ptSelCurselPos[%d][%d] \n", m_ptCurselPos.x, m_ptCurselPos.y, 
	// 														m_ptSelCurselPos.x, m_ptSelCurselPos.y);

	int nFindPos = GetStringBuffPos(m_strBkInputMessage, m_ptCurselPos.x);

	if (-1 == nFindPos)
	{
		return INF_MSGPROC_NORMAL;
	}
	memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);		// IME초기화 전에 값을 가지고 있는다.
	memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	

	if (nFindPos > 0)
	{
		strncpy(m_strPreBackupMessage, m_strBkInputMessage, nFindPos);
	}
	strcpy(m_strNextBackupMessage, &m_strBkInputMessage[nFindPos]);

	g_pD3dApp->CleanText(); // IME버퍼 초기화


	// 마지막 버퍼에 저장
	strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);

	return INF_MSGPROC_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		Back, del키 입력
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFGameMainChat::OnKeyDownFunc(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_BACK:
	{
		// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		// 블럭단위 삭제 구현
		// 			if((0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
		// 				&& (strlen(m_strPreBackupMessage) > 0))
		// 			{
		// 				int nStrLen = GetStringBuffLen(m_strPreBackupMessage)-1;
		// 				if(nStrLen >= 0)
		// 				{
		// 					int nFindBackPos = GetStringBuffPos(m_strPreBackupMessage, nStrLen);
		// 					if(nFindBackPos >= 0)
		// 					{
		// 						charF chTmp[SIZE_MAX_CHAT_MESSAGE];
		// 						ZERO_MEMORY(chTmp);
		// 						if(nFindBackPos > 0)
		// 						{
		// 							strncpy(chTmp, m_strPreBackupMessage, nFindBackPos);
		// 						}
		// 						strncpy(m_strPreBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
		// 						
		// 						// 갱신 버퍼 초기화
		// 						SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
		// 						
		// 						m_ptCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
		// 
		// 						// 마지막 버퍼에 저장
// 						strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
		int  chLenTmp = 0;
		char chTmp[SIZE_MAX_CHAT_MESSAGE];
		ZERO_MEMORY(chTmp);

		if (0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
		{
			// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
// 				if(m_ptSelCurselPos.x != -1)
// 				{   // 블럭 작업 
// 					chLenTmp = m_ptCurselPos.x-m_ptSelCurselPos.x;
// 					BlockCleanChat(chLenTmp);
// 				}
			if (m_bCaretCursel == TRUE)
			{   // 블럭 작업 					
				BlockCleanChat(m_ptCurselPos.x, m_ptSelCurselPos.x);
			}
			//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
			else
			{
				// 블럭이 없을때 작업
				int nStrLen = GetStringBuffLen(m_strPreBackupMessage) - 1;
				if (nStrLen >= 0)
				{
					int nFindBackPos = GetStringBuffPos(m_strPreBackupMessage, nStrLen);
					if (nFindBackPos >= 0)
					{
						char chTmp[SIZE_MAX_CHAT_MESSAGE];
						ZERO_MEMORY(chTmp);
						if (nFindBackPos > 0)
						{
							strncpy(chTmp, m_strPreBackupMessage, nFindBackPos);
						}
						strncpy(m_strPreBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
						// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
//  							SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
//   							m_ptCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
//   							strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
							// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
							//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
					}
					return INF_MSGPROC_BREAK;

				}
			}
		}
		return INF_MSGPROC_NORMAL;
	}
	break;
	case VK_DELETE:
	{
		// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		// 블럭단위 삭제 구현
// 			int nStringLen = GetStringBuffLen(m_strBkInputMessage);	
// 			if((m_ptCurselPos.x != nStringLen)			// 마지막 위치냐?
// 				&& (0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
// 				&& (strlen(m_strNextBackupMessage) > 0))
// 			{
// 				int nFindNevtPos = GetStringBuffPos(m_strNextBackupMessage, 1);	// 첫번째 글자의 버퍼를 가져온다			
// 				if(nFindNevtPos >= 0)
// 				{
// 					char chTmp[SIZE_MAX_CHAT_MESSAGE];
// 					ZERO_MEMORY(chTmp);
// 					
// 					strcpy(chTmp, &m_strNextBackupMessage[nFindNevtPos]);
// 					strncpy(m_strNextBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
// 					
// 					// 갱신 버퍼 초기화
// 					SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);			
// 					
// 					// 마지막 버퍼에 저장
// 					strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);

		if (0 != strlen(g_pD3dApp->m_inputkey.m_full_str))
		{
			strcat(m_strPreBackupMessage, g_pD3dApp->m_inputkey.m_full_str);
			m_ptCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
			strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
			g_pD3dApp->CleanText();
		}

		int  chLenTmp = 0;
		char chTmp[SIZE_MAX_CHAT_MESSAGE];
		ZERO_MEMORY(chTmp);
		if (0 == strlen(g_pD3dApp->m_inputkey.m_full_str))
		{
			// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
// 					if(m_ptSelCurselPos.x != -1 )
// 					{
// 						chLenTmp = m_ptCurselPos.x-m_ptSelCurselPos.x;
//  						BlockCleanChat(chLenTmp);
// 					}
			if (m_bCaretCursel == TRUE)
			{
				BlockCleanChat(m_ptCurselPos.x, m_ptSelCurselPos.x);
				// 갱신 버퍼 초기화
				SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
				// 마지막 버퍼에 저장
				strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
			}
			//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
			else
			{
				int nFindNevtPos = GetStringBuffPos(m_strNextBackupMessage, 1);	// 첫번째 글자의 버퍼를 가져온다			
				if (nFindNevtPos >= 0)
				{
					strcpy(chTmp, &m_strNextBackupMessage[nFindNevtPos]);
					strncpy(m_strNextBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
					// 갱신 버퍼 초기화
					SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);
					// 마지막 버퍼에 저장
					strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				}
				return INF_MSGPROC_BREAK;
			}
		}
		return INF_MSGPROC_NORMAL;
	}

	break;
	case VK_SHIFT:
	{
		USHORT nRace = g_pD3dApp->m_pShuttleChild->GetMyShuttleInfo().Race;
		// 2010. 05. 25 by jskim 붙여넣기, 복사, 잘라내기 일반계정 허용
		// 			if(COMPARE_RACE(nRace,RACE_OPERATION) || COMPARE_RACE(nRace,RACE_GAMEMASTER))
		// 			{
		//end 2010. 05. 25 by jskim 붙여넣기, 복사, 잘라내기 일반계정 허용
					// 관리자만 쉬프트 기능 키자
		m_ptSelCurselPos = m_ptCurselPos;	// 커서 위치			
		m_bShiftClick = TRUE;

		return INF_MSGPROC_BREAK;
		//			}			
					// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안 
		// 			if(COMPARE_RACE(nRace,RACE_OPERATION) || COMPARE_RACE(nRace,RACE_GAMEMASTER))
		//			{
					// 관리자만 쉬프트 기능 키자
		//				m_ptSelCurselPos = m_ptCurselPos;	// 커서 위치			
		//				m_bShiftClick = TRUE;						
		//				
		//				return INF_MSGPROC_BREAK;
		//			}
					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안 
					//DBGOUT("m_bCaretCursel = TRUE;m_ptSelCurselPos[%d][%d] \n", m_ptSelCurselPos.x, m_ptSelCurselPos.y);
	}
	break;
	}
	return INF_MSGPROC_NORMAL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::InitChatMsgBuff()
{
	memset(m_strBkInputMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.
	memset(m_strPreBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.
	memset(m_strNextBackupMessage, 0x00, SIZE_MAX_CHAT_MESSAGE);	// IME초기화 전에 값을 가지고 있는다.	
	m_ptCurselPos.x = m_ptCurselPos.y = 0;					// 실제 버프 위치	
	// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	//m_ptSelCurselPos.x = m_ptSelCurselPos.y = 0;			// 커서 위치
	// m_ptSelCurselPos 위치를 선택 안되있는 상태로 설정 
	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	//m_ptSelCurselPos.x = m_ptSelCurselPos.y = -1;			// 커서 위치
	m_ptSelCurselPos.x = m_ptSelCurselPos.y = 0;			// 커서 위치
	//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	m_bShiftClick = FALSE;
	m_bCaretCursel = FALSE;
	m_bControlCursel = FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::SetChatMsgBuff(char* i_pImeChat)
{
	BOOL bLastPos = FALSE;
	int nStringLen = GetStringBuffLen(m_strBkInputMessage);
	if (m_ptCurselPos.x == nStringLen)
	{
		// 현재 마지막 위치다
		bLastPos = TRUE;
	}
	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	// 	{
	// 		// 앞에 버프를 먼저 쓰자
	// 		strncpy(m_strBkInputMessage, m_strPreBackupMessage, (SIZE_MAX_CHAT_MESSAGE-1));
	// 		
	// 		int nTmp = 0;
	// 		int nStrinTmpLen = 0;
	// 		{
	// 			nTmp = 0;
	// 			nStrinTmpLen = strlen(m_strBkInputMessage) + strlen(i_pImeChat);
	// 			if(nStrinTmpLen > (SIZE_MAX_CHAT_MESSAGE-1))
	// 			{
	// 				nTmp = (SIZE_MAX_CHAT_MESSAGE-1)- strlen(m_strBkInputMessage);								
	// 				if(nTmp > 0)
	// 				{								
	// 					// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 					//strncat(m_strBkInputMessage, i_pImeChat, nTmp);
	// 					// 문자열 마지막에 쓰레기 값 안들어가게 수정 
	// 					strncat(m_strBkInputMessage, i_pImeChat, strlen(i_pImeChat));
	// 					if(m_bShiftClick = NULL)
	// 					{
	// 						m_bShiftClick = FALSE;
	// 					}
	// 					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 				}									
	// 			}
	// 			else
	// 			{
	// 				// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 				// 채팅 모드 문자로 이동 못하게 한다.
	// 				//strcat(m_strBkInputMessage, i_pImeChat);
	// 				if(ChatModeChack(i_pImeChat[0]) &&		// 채팅 모드에 따라
	// 				    m_strBkInputMessage[0] == i_pImeChat[0] &&
	// 					m_ptCurselPos.x > 0)	
	// 				{
	// 					int chtmp = GetStringBuffPos(i_pImeChat,1);
	// 					strcpy(i_pImeChat,&i_pImeChat[chtmp]);
	// 				}
	// 				strcat(m_strBkInputMessage, i_pImeChat);
	// 				// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 			}
	// 				
	// 		}
	// 		
	// 		{
	// 			nStrinTmpLen = strlen(m_strBkInputMessage) + strlen(m_strNextBackupMessage);
	// 
	// 			if(nStrinTmpLen > (SIZE_MAX_CHAT_MESSAGE-1))
	// 			{
	// 				nTmp = (SIZE_MAX_CHAT_MESSAGE-1) - strlen(m_strBkInputMessage);								
	// 				if(nTmp > 0)
	// 				{	
	// 					//strncat(m_strBkInputMessage, m_strNextBackupMessage, nTmp);
	// 					// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 					// 문자열 마지막에 쓰레기 값 안들어가게 수정 
	// 					strncat(m_strBkInputMessage, m_strNextBackupMessage, strlen(m_strNextBackupMessage));
	// 					if(m_bShiftClick = NULL)
	// 					{
	// 						m_bShiftClick = FALSE;
	// 					}
	// 					// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	// 				}									
	// 			}
	// 			else
	// 			{
	// 				strcat(m_strBkInputMessage, m_strNextBackupMessage);
	// 			}
	// 		}		
	// 	}
	if (strlen(m_strPreBackupMessage) + strlen(i_pImeChat) + strlen(m_strNextBackupMessage) < SIZE_MAX_CHAT_MESSAGE - 1)
	{
		wsprintf(m_strBkInputMessage, "%s%s%s", m_strPreBackupMessage, i_pImeChat, m_strNextBackupMessage);
	}
	else
	{
		strcpy(m_strBkInputMessage, m_strPreBackupMessage);
		if (strlen(m_strPreBackupMessage) + strlen(i_pImeChat) < SIZE_MAX_CHAT_MESSAGE)
		{
			strcat(m_strBkInputMessage, i_pImeChat);
		}
		else
		{
			int nFindLen = GetStringBuffLen(i_pImeChat);
			char chrtmp[256];
			for (int i = 1; i <= nFindLen; i++)
			{
				memset(chrtmp, 0x00, 256);
				int nFindPos = GetStringBuffPos(i_pImeChat, nFindLen - i);
				strncpy(chrtmp, i_pImeChat, nFindPos);
				if (strlen(m_strBkInputMessage) + strlen(chrtmp) < SIZE_MAX_CHAT_MESSAGE)
				{
					strcat(m_strBkInputMessage, chrtmp);
					break;
				}
			}
		}
		if (strlen(m_strBkInputMessage) + strlen(m_strNextBackupMessage) >= SIZE_MAX_CHAT_MESSAGE)
		{
			int nFindLen = GetStringBuffLen(m_strNextBackupMessage);
			char chrtmp[256];

			for (int i = 1; i <= nFindLen; i++)
			{
				memset(chrtmp, 0x00, 256);
				int nFindPos = GetStringBuffPos(m_strNextBackupMessage, nFindLen - i);
				strncpy(chrtmp, m_strNextBackupMessage, nFindPos);
				int ss = strlen(chrtmp);
				if (strlen(m_strBkInputMessage) + strlen(chrtmp) < SIZE_MAX_CHAT_MESSAGE)
				{
					strcat(m_strBkInputMessage, chrtmp);
					// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화 - 추가
					strcpy(m_strNextBackupMessage, chrtmp);
					//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화 - 추가 

					break;
				}
			}
		}
	}
	//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	if (bLastPos)
	{
		// 마지막에 덮붙이는 식이냐?
		m_ptCurselPos.x = GetStringBuffLen(m_strBkInputMessage);
	}
	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	else
	{
		m_ptCurselPos.x = GetStringBuffLen(m_strBkInputMessage) - GetStringBuffLen(m_strNextBackupMessage);
	}
	//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::RenderCursel(int nShowTemp)
{

	int nCurPosX = 0;
	int nCurPosY = 0;
	// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	char chatCurselBuff[256];
	memset(chatCurselBuff, 0x00, 256);
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안

	strncpy(chatCurselBuff, m_strPreBackupMessage, SIZE_MAX_CHAT_MESSAGE);

	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	//if((strlen(chatCurselBuff)+strlen(g_pD3dApp->m_inputkey.m_full_str)) < (SIZE_MAX_CHAT_MESSAGE+2))
	if ((strlen(chatCurselBuff) + strlen(g_pD3dApp->m_inputkey.m_full_str)) < (SIZE_MAX_CHAT_MESSAGE))
	{
		//strcat(chatCurselBuff, g_pD3dApp->m_inputkey.m_full_str);
		//chatCurselBuff문자열에 채팅모드가 포함되어있을때 Clean
// 		if(ChatModeChack(chatCurselBuff[0]) &&	
// 			g_pD3dApp->m_inputkey.m_full_str[0] == chatCurselBuff[0])
// 		{
// 			g_pD3dApp->CleanText();  
// 		}		
		strcat(chatCurselBuff, g_pD3dApp->m_inputkey.m_full_str);
		// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	}
	else
	{
		int nFindLen = GetStringBuffLen(g_pD3dApp->m_inputkey.m_full_str);
		char chrtmp[256];
		for (int i = 1; i <= nFindLen; i++)
		{
			memset(chrtmp, 0x00, 256);
			int nFindPos = GetStringBuffPos(g_pD3dApp->m_inputkey.m_full_str, nFindLen - i);
			strncpy(chrtmp, g_pD3dApp->m_inputkey.m_full_str, nFindPos);
			if (strlen(chatCurselBuff) + strlen(chrtmp) < SIZE_MAX_CHAT_MESSAGE)
			{
				strcat(chatCurselBuff, chrtmp);
				break;
			}
		}
	}
	//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	int nCurslePos = strlen(chatCurselBuff);

	chatCurselBuff[nCurslePos] = '_';
	chatCurselBuff[nCurslePos + 1] = '\0';


	if ((strlen(chatCurselBuff) + strlen(m_strNextBackupMessage)) < (SIZE_MAX_CHAT_MESSAGE + 2))
	{
		strcat(chatCurselBuff, m_strNextBackupMessage);
	}
	// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
	else
	{
		int nFindLen = GetStringBuffLen(m_strNextBackupMessage);
		char chrtmp[256];
		for (int i = 1; i <= nFindLen; i++)
		{
			memset(chrtmp, 0x00, 256);
			int nFindPos = GetStringBuffPos(m_strNextBackupMessage, nFindLen - i);
			strncpy(chrtmp, m_strNextBackupMessage, nFindPos);
			if (strlen(chatCurselBuff) + strlen(chrtmp) < SIZE_MAX_CHAT_MESSAGE + 2)
			{
				strcat(chatCurselBuff, chrtmp);
				break;
			}
		}
	}
	// 	// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안	
	// 	int pos = GetStringBuffPos(chatCurselBuff, GetStringBuffLen(chatCurselBuff - 1));	 
	// 	int pos1 = chatCurselBuff[pos] < 0;
	// 	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화

	BOOL bOnePageOver = FALSE;
	RECT rcRender;

	rcRender.left = m_ptCurselPos.x;
	rcRender.top = m_ptCurselPos.y;
	rcRender.right = m_ptSelCurselPos.x;
	rcRender.bottom = m_ptSelCurselPos.y;

	// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	char chCurselTmp[256];
	ZERO_MEMORY(chCurselTmp);

	int nCurselPosX = m_ptCurselPos.x;
	if (m_bCaretCursel)
	{
		if (m_ptCurselPos.x > m_ptSelCurselPos.x)
		{
			nCurselPosX = m_ptSelCurselPos.x;
		}
		else if (m_ptCurselPos.x < m_ptSelCurselPos.x)
		{
			nCurselPosX = m_ptCurselPos.x;
		}
	}

	// 가장 마지막 줄의 정보가 아니다.		
	int nIMELen = GetStringBuffLen(chatCurselBuff);
	int nCurPos = GetStringBuffPos(chatCurselBuff, (nCurselPosX));
	strcpy(chCurselTmp, chatCurselBuff);
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안

	SIZE szTmp = m_pFontInput->GetStringSize(chCurselTmp);
	if (szTmp.cx > CHAT_INPUT_FONT_LENGTH)
	{
		// 이미지 크기 체크
		int nFindPrePos = nCurselPosX;
		if (nFindPrePos < 0)
		{
			nFindPrePos = 0;
		}
		nCurPos = GetStringBuffPos(chatCurselBuff, nFindPrePos);	// 1글자 전까지 복사하자			
		if (nCurPos >= 0)
		{
			//strcpy(chCurselTmp, &chatCurselBuff[nCurPos]);
			vector<string> vecChatMessage;
			STRING_CULL(chCurselTmp, CHAT_INPUT_FONT_LENGTH, &vecChatMessage, m_pFontInput);

			if (!vecChatMessage.empty())
				// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
				//{
				//	strncpy(chatCurselBuff, (char*)vecChatMessage[0].c_str(), 256);
				//	bOnePageOver = TRUE;	// 한페이지를 넘겼다.
				//}
				//	if(bOnePageOver)
				//	{
				//		rcRender.left -= nCurPos;
				//		rcRender.right -= nCurPos;
				//
				//		if(rcRender.left < 0)
				//		{
				//			rcRender.left = 0;
				//		}
				//		if(rcRender.right < 0)
				//		{
				// 			rcRender.right = 0;
				// 		}				
				//	}
			{
				// 문자 다음열 블럭 지정 추가 
				// 2013-05-14 by bhsohn Buffer OverFlow Fix
// 				int chlen[3];
// 				char chtmp[3][SIZE_MAX_CHAT_MESSAGE];
// 				int chlensum;
				const INT MAX_CHAT_LINE = 5;
				int chlen[MAX_CHAT_LINE] = { 0, };
				char chtmp[MAX_CHAT_LINE][SIZE_MAX_CHAT_MESSAGE];
				int chlensum = 0;
				// end 2013-05-14 by bhsohn Buffer OverFlow Fix


				for (int i = 0; i < vecChatMessage.size(); i++)
				{
					// 2013-05-14 by bhsohn Buffer OverFlow Fix
					if (i >= MAX_CHAT_LINE)
					{
						break;
					}
					// END 2013-05-14 by bhsohn Buffer OverFlow Fix
					chlen[i] = GetStringBuffLen((char*)(vecChatMessage[i]).c_str());
					strcpy(chtmp[i], (char*)(vecChatMessage[i]).c_str());
				}
				chlensum = chlen[0] + chlen[1];

				// 채팅 두번째 문자열
				if (rcRender.left > chlen[0] && rcRender.left <= chlensum)
				{
					strncpy(chatCurselBuff, chtmp[1], 256);
					// 오른쪽 블럭지정
					if (m_ptCurselPos.x > m_ptSelCurselPos.x)
					{
						rcRender.left = rcRender.left - chlen[0];
						rcRender.right = rcRender.right - chlen[0];
						if (rcRender.right < 0)
						{
							rcRender.right = 0;
						}
					}
					else
					{
						// 왼쪽 블럭지정
						rcRender.left = rcRender.left - chlen[0] + 1;
						if (rcRender.right > chlensum)
						{
							// 2009. 12. 03 by jskim 불럭설정 문제 수정
							//.rcRender.right = 20;
							rcRender.right = chlen[1];
							//end 2009. 12. 03 by jskim 불럭설정 문제 수정
						}
						else
						{
							rcRender.right = rcRender.right - chlen[0] + 1;
						}
					}
				}
				// 채팅 세번째 문자열
				else if (rcRender.left > chlensum)
				{
					strncpy(chatCurselBuff, chtmp[2], 256);
					if (m_ptCurselPos.x > m_ptSelCurselPos.x)
					{
						// 오른쪽 블럭지정
						rcRender.left = rcRender.left - chlensum;
						rcRender.right = rcRender.right - chlensum;
						if (rcRender.right < 0)
						{
							rcRender.right = 0;
						}
					}
					else
					{
						// 왼쪽 블럭지정
						rcRender.left = rcRender.left - chlensum + 1;
						rcRender.right = rcRender.right - chlensum + 1;
					}
				}
				else
				{
					// 채팅 첫번째 문자열
					strncpy(chatCurselBuff, chtmp[0], 256);
					if (m_ptCurselPos.x < m_ptSelCurselPos.x)
					{
						//왼쪽 블럭지정
						if (rcRender.right > chlen[0])
						{
							// 2009. 12. 03 by jskim 불럭설정 문제 수정
							//rcRender.right = 20;
							rcRender.right = chlen[0];
							//end 2009. 12. 03 by jskim 불럭설정 문제 수정
						}
						else
						{
							//오른쪽 블럭지정
							rcRender.right = m_ptSelCurselPos.x + 1;
						}
						rcRender.left = m_ptCurselPos.x + 1;
					}
				}
			}
		}
	}
	else
	{
		if (m_ptCurselPos.x < m_ptSelCurselPos.x)
		{
			rcRender.right = m_ptSelCurselPos.x + 1;
			rcRender.left = m_ptCurselPos.x + 1;
		}
	}
	// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	m_pFontInput->SetTextureWidth(CHAT_INPUT_FONT_LENGTH);
	if (m_bCaretCursel)
		// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안


	{	//블럭 있는 텍스트 
// 		if(m_ptCurselPos.x  < m_ptSelCurselPos.x )
// 		{
// 			rcRender.left++;
// 			rcRender.right++;
// 		}
		// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		NormalizeRect(&rcRender);
		m_pFontInput->SetReLoadString(TRUE);
		m_pFontInput->DrawText(CHAT_FONT_START_X + nShowTemp,
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			FONT_INPUT_Y, GUI_FONT_COLOR_Y,
#else															
			CHAT_FONT_START_Y + FONT_INPUT_Y, GUI_FONT_COLOR_Y,
#endif
			chatCurselBuff,
			0,
			&rcRender);
	}
	else
		// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
	{   //블럭 없는 텍스트						//주석 
		// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
		m_pFontInput->DrawText(CHAT_FONT_START_X + nShowTemp,
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			FONT_INPUT_Y, GUI_FONT_COLOR_Y,
#else 
			CHAT_FONT_START_Y + FONT_INPUT_Y, GUI_FONT_COLOR_Y,
#endif
			chatCurselBuff,
			0L);

	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::SetClipBoard(char* i_pStr)
{
	// 	클립보드 열고 -> 메모리생성해서 거기다 데이타 복사주해고 ->  클립보드 닫고, 메모리해제		
	// 	클립보드는 락,언락을 해주어야 합니다..푸힛~리소스를 공유하기 때문이죠.
	if (OpenClipboard(NULL))
	{
		if (EmptyClipboard())
		{
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strlen(i_pStr) + 1);
			if (hMem)
			{
				LPTSTR pClipData = (LPTSTR)GlobalLock(hMem);
				pClipData[0] = 0;
				strncpy(pClipData, i_pStr, strlen(i_pStr) + 1);

				//DBGOUT("SetClipBoard[%s] \n", pClipData);

				SetClipboardData(CF_OEMTEXT, hMem);
				GlobalUnlock(hMem);
			}
			CloseClipboard();
			GlobalFree(hMem);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::PasteFromClipBoard(char* o_pStrTxt, int nSize)
{
	if (OpenClipboard(NULL))
	{
		HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
		char strBuff[1024];
		strBuff[0] = NULL;
		if (hGlobal)
		{
			PTSTR pGlobal = (char*)GlobalLock(hGlobal);
			//lstrcpy( strBuff, pGlobal);
			strncpy(strBuff, pGlobal, nSize);
			GlobalUnlock(hGlobal);
		}
		CloseClipboard();
		strncpy(o_pStrTxt, strBuff, nSize);

		//DBGOUT("PasteFromClipBoard[%s] \n", o_pStrTxt);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 선택한 커서의 버퍼
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainChat::GetCurselString(char* o_pStrTxt)
{
	if (!m_bCaretCursel)
	{
		o_pStrTxt[0] = NULL;
		return;
	}
	else if (m_ptCurselPos.x == m_ptSelCurselPos.x)
	{
		o_pStrTxt[0] = NULL;
		return;
	}

	RECT rcRender;
	rcRender.left = m_ptCurselPos.x;
	rcRender.top = m_ptCurselPos.y;
	rcRender.right = m_ptSelCurselPos.x;
	rcRender.bottom = m_ptSelCurselPos.y;

	NormalizeRect(&rcRender);

	int nStartPos = GetStringBuffPos(m_strBkInputMessage, rcRender.left);
	int nEndPos = GetStringBuffPos(m_strBkInputMessage, rcRender.right);

	if ((-1 == nStartPos) || (-1 == nEndPos))
	{
		o_pStrTxt[0] = NULL;
		return;
	}

	int nCopyLen = nEndPos - nStartPos;

	if (nCopyLen <= 0)
	{
		o_pStrTxt[0] = NULL;
		return;
	}
	strncpy(o_pStrTxt, &m_strBkInputMessage[nStartPos], nCopyLen);
	// 	DBGOUT("CINFGameMainChat::GetCurselString nStartPos[%d] nEndPos[%d] o_pStrTxt[%s] \n", 
	// 																		nStartPos, nEndPos, o_pStrTxt);

}

// 2010. 02. 23 by jskim 채팅버그 수정 및 최적화
// 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안
// 블럭지정 삭제
// void CINFGameMainChat::BlockCleanChat(int chLen)
// {
// 	char chTmp[SIZE_MAX_CHAT_MESSAGE];
// 	ZERO_MEMORY(chTmp);
// 	int nFindNevtPos;
// 	int nStringLen = GetStringBuffLen(m_strBkInputMessage);	
// 	if(chLen < 0)
// 	{	// 왼쪽 블럭지정 상태
// 		nFindNevtPos= GetStringBuffPos(m_strNextBackupMessage, abs(chLen));	
// 		
// 		strcpy(chTmp, &m_strNextBackupMessage[nFindNevtPos]);
// 		strncpy(m_strNextBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
// 		
// 
// 		SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);			
// 		
// 		// 마지막 버퍼에 저장
// 		strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);
// 	}
// 	else if(chLen > 0)
// 	{	// 오른쪽 블럭지정 상태
// 		int nFindLen = GetStringBuffLen(m_strPreBackupMessage)-abs(m_ptCurselPos.x-m_ptSelCurselPos.x);
// 		nFindNevtPos = GetStringBuffPos(m_strPreBackupMessage,nFindLen);
// 			
// 	if(nFindLen >= 0)
// 		{
// 			strncpy(chTmp, m_strPreBackupMessage, nFindNevtPos);
// 		
// 			strncpy(m_strPreBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
// 		
// 			SetChatMsgBuff(g_pD3dApp->m_inputkey.m_full_str);			
// 		
// 			m_ptCurselPos.x = GetStringBuffLen(m_strPreBackupMessage);
// 		
// 			strncpy(m_strInputMessage, m_strBkInputMessage, SIZE_MAX_CHAT_MESSAGE);	
// 		}
// 	}	
// 	m_ptSelCurselPos.x=-1;
// 	m_bShiftClick=FALSE;
// 	m_bCaretCursel=FALSE;
// }
void CINFGameMainChat::BlockCleanChat(int LeftBlockPos, int RightBlockPos)
{
	char chTmp[SIZE_MAX_CHAT_MESSAGE];
	ZERO_MEMORY(chTmp);
	int nFindNevtPos;
	int nStringLen = GetStringBuffLen(m_strBkInputMessage);
	int BlockType = IS_BLOCK_TYPE(LeftBlockPos, RightBlockPos);
	if (BlockType == LEFT_BLOCK)
	{	// 왼쪽 블럭지정 상태
		nFindNevtPos = GetStringBuffPos(m_strNextBackupMessage, RightBlockPos - LeftBlockPos);
		strcpy(chTmp, &m_strNextBackupMessage[nFindNevtPos]);
		strncpy(m_strNextBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
	}
	else if (BlockType == RIGHT_BLOCK)
	{	// 오른쪽 블럭지정 상태
		int nFindLen = GetStringBuffLen(m_strPreBackupMessage) - abs(RightBlockPos - LeftBlockPos);
		nFindNevtPos = GetStringBuffPos(m_strPreBackupMessage, nFindLen);
		strncpy(chTmp, m_strPreBackupMessage, nFindNevtPos);
		strncpy(m_strPreBackupMessage, chTmp, SIZE_MAX_CHAT_MESSAGE);
	}
	m_ptCurselPos.x = 0;
	m_ptSelCurselPos.x = 0;
	m_bShiftClick = FALSE;
	m_bCaretCursel = FALSE;
}
//end 2010. 02. 23 by jskim 채팅버그 수정 및 최적화

// 채팅모드 체크 
bool CINFGameMainChat::ChatModeChack(char i_Chr)
{
	// 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
// 	if(i_Chr=='~' ||
// 		i_Chr=='%' ||
// 		i_Chr=='@' ||
// 		i_Chr=='#' ||
// 		i_Chr=='^' ||
// 		i_Chr=='!' ||
// 		i_Chr=='$' ||
// 		i_Chr=='*')
	if (i_Chr == CHAT_ALL
		|| i_Chr == CHAT_ARENA
		|| i_Chr == CHAT_GUILD
		|| i_Chr == CHAT_PARTY
		|| i_Chr == CHAT_WAR
		|| i_Chr == CHAT_MAP
		|| i_Chr == CHAT_SELL_ALL
		|| i_Chr == CHAT_CNC
		|| i_Chr == CHAT_INFLUENCE_ALL
		|| i_Chr == CHAT_CHATROOM)	// 2012-12-14 by jhseol, 채팅룸이 &를 기억하지 못해 매번 &를 쳐줘야 하는 문제 수정 - CHAT_CHATROOM 추가
	// end 2010. 04. 09 by ckPark 클라이언트 로컬라이즈 파일 추가
	{
		return TRUE;
	}
	return FALSE;
}
// end 2009. 07. 30 by jsKim 채팅창 버그 수정 및 기능 보안


void CINFGameMainChat::GetStrPara(char *o_szStrBuff, char *i_szString, int iParamNum /*= 0*/)
{

	if (NULL == o_szStrBuff)
		return;

	if (NULL == i_szString || 1 > strlen(i_szString))
		return;

	int iSize = strlen(i_szString);
	int i = 0;

	int iStartNum = 0;
	int iEndNum = 0;

	if (iParamNum > 0)
	{
		int iCheckCnt = 0;

		for (i = 0; i < iSize; ++i)
		{
			if (i_szString[i] == ' ')
			{
				iStartNum = i + 1;

				iCheckCnt++;

				if (iCheckCnt == iParamNum)
					break;
			}
		}

		if (iCheckCnt != iParamNum)
			return;
	}

	iEndNum = iStartNum;

	for (iEndNum = iStartNum; iEndNum < iSize; ++iEndNum)
	{
		if (iEndNum == iSize) break;

		if (i_szString[iEndNum] == ' ') break;
	}

	if (iStartNum != iEndNum)
	{
		memcpy(o_szStrBuff, &i_szString[iStartNum], iEndNum - iStartNum);
	}

}