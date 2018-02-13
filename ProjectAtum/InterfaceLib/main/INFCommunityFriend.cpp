// INFCommunityFriend.cpp: implementation of the CINFCommunityFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCommunityFriend.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "INFGameMain.h"
#include "GameDataLast.h"
// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//#include "INFScrollBar.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"
#include "AtumSound.h"

#include "INFCommunity.h"


#include "D3DHanFont.h"
#include "INFWindow.h"
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
//#include "IMSocket.h"

#include "INFGameMainChat.h"
#include "dxutil.h"

#include "Chat.h"

#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"					 // 2011. 10. 10 by jskim UI시스템 변경

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//#define MAX_FRIEND_NUMBER					10	// 화면상 라인 수
//#define MAX_FRIEND_LIST						50  // 저장할수 있는 최대 친구 수
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define SCROLL_START_X						197
#define SCROLL_START_Y						80
#define SCROLL_LINE_LENGTH					172

#define LIST_BOX_START_X					19
#define LIST_BOX_START_Y					97
#define LIST_BOX_SIZE_X						171
#define LIST_BOX_SIZE_Y						154
#define LIST_BOX_INTERVAL					17

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//#define LIST_BOX_NAME_START_X				22
#define LIST_BOX_NAME_START_X				256
#define LIST_BOX_NAME_START_Y				123

#define LIST_FRIEND_START_X					395
#define LIST_FRIEND_START_Y					74
// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

#define BACK_START_X						13
#define BACK_START_Y						25

#define FRIEND_MEMBER_ONLINE_COLOR			GUI_FONT_COLOR
#define FRIEND_MEMBER_OFFLINE_COLOR			RGB(222, 0, 0)

// 친구 설정
#define ADD_FRIEND_START_X					BACK_START_X + 106
#define ADD_FRIEND_START_Y					BACK_START_Y + 226
#define ADD_FRIEND_WIDTH					35
#define ADD_FRIEND_HEIGHT					15

#define FRIEND_STATE_ONLINE					TRUE
#define FRIEND_STATE_OFFLINE				FALSE

#define SELECT_FRIEND_NAME_X				18
#define SELECT_FRIEND_NAME_Y				79
#define SELECT_FRIEND_NAME_WIDTH			171
#define SELECT_FRIEND_NAME_HEIGHT			17
#define SELECT_FRIEND_MACRO_X				19
#define SELECT_FRIEND_MACRO_Y				63

#else 
#define SCROLL_START_X						197
#define SCROLL_START_Y						55
#define SCROLL_LINE_LENGTH					172

#define LIST_BOX_START_X					19
#define LIST_BOX_START_Y					72
#define LIST_BOX_SIZE_X						171
#define LIST_BOX_SIZE_Y						154
#define LIST_BOX_INTERVAL					17

// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//#define LIST_BOX_NAME_START_X				22
#define LIST_BOX_NAME_START_X				258
#define LIST_BOX_NAME_START_Y				123

#define LIST_FRIEND_START_X					381
#define LIST_FRIEND_START_Y					73
// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

#define BACK_START_X						13
#define BACK_START_Y						30

#define FRIEND_MEMBER_ONLINE_COLOR			GUI_FONT_COLOR
#define FRIEND_MEMBER_OFFLINE_COLOR			RGB(222, 0, 0)

// 친구 설정
#define ADD_FRIEND_START_X					BACK_START_X + 106
#define ADD_FRIEND_START_Y					BACK_START_Y + 201
#define ADD_FRIEND_WIDTH					35
#define ADD_FRIEND_HEIGHT					15

#define FRIEND_STATE_ONLINE					TRUE
#define FRIEND_STATE_OFFLINE				FALSE

#define SELECT_FRIEND_NAME_X				18
#define SELECT_FRIEND_NAME_Y				54
#define SELECT_FRIEND_NAME_WIDTH			171
#define SELECT_FRIEND_NAME_HEIGHT			17
#define SELECT_FRIEND_MACRO_X				19
#define SELECT_FRIEND_MACRO_Y				38

#endif
#define ON_OFF_LINE_WIDTH					130

#define BOTTON_STATE_0						0
#define BOTTON_STATE_1						1
#define BOTTON_STATE_2						2
#define BOTTON_STATE_3						3
#define BOTTON_STATE_4						4

#define REFRESH_TIME_DELAY					5.0f

// 2012-10-17 by jhjang 해피아워 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	#define FRIEND_BONUS						1.5f
	#define MAX_FRIEND_BONUS					70.0f
#else
	#define FRIEND_BONUS						0.5f
#endif //S_BONUSEXPSYSTEM_RENEWAL
// end 2012-10-17 by jhjang 해피아워 시스템 리뉴얼

// 2006-03-07 by ispark, 언어에 따라 위치 수정
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
//#define LIST_BOX_NAME_START_Y				55//57
//#else
//#define LIST_BOX_NAME_START_Y				55//57
//#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityFriend::CINFCommunityFriend(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_pBack = NULL;
	m_pImgSelect			= NULL;	
	m_pScroll				= NULL;

	m_bAddFriendClick		= FALSE;
	m_bDeleteFriendClick	= FALSE;
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//m_nFriendSelect			= 0;
	m_nFriendSelect			= -1;
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	//m_bDeleteOK				= FALSE;
	m_nButtonState			= 0;	
	
//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i] = NULL;
//	}

	m_fRefreshTime			= REFRESH_TIME_DELAY;
	m_bWisperChatSend		= FALSE;

	int i=0;
	for(i=0; i<MAX_FRIEND_NUMBER; i++)
	{
		m_pFontFriend[i] = NULL;
	}	

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_pRegisterFriend = NULL ;	// 등록
	m_pDelFriend = NULL ;		// 삭제

	// 2008-10-24 by bhsohn 친구, 거부 리스트 49명만 되는 현상 처리
	m_vecFriend.clear();	

	// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
	memset(m_szRqFriendName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
	// end 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
	
}

CINFCommunityFriend::~CINFCommunityFriend()
{
	util::del(m_pBack);
	util::del(m_pImgSelect);
	util::del(m_pScroll);
	
//	for(int i=0; i<4; i++)
//	{
//		util::del(m_pImgButton[i]);
//	}
	int i=0;
	for(i=0; i<MAX_FRIEND_NUMBER; i++)
	{
		util::del(m_pFontFriend[i]);
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	util::del(m_pRegisterFriend);	// 적용
	util::del(m_pDelFriend);	
	
}

HRESULT CINFCommunityFriend::InitDeviceObjects()
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//DataHeader *pDataHeader = m_pGameData->Find("frback");
	DataHeader *pDataHeader;

	//pDataHeader = m_pGameData->Find("frselect");
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_pBack == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("commu_Fri");	
		m_pBack = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pBack->RestoreDeviceObjects();
	}
#else 
    pDataHeader = m_pGameData->Find("commubk1");
 	m_pBack = new CINFImageEx;
 	m_pBack->InitDeviceObjects( pDataHeader );
#endif

	pDataHeader = m_pGameData->Find("friselect");	
	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//	m_pScroll = new CINFScrollBar(this,
//								SCROLL_START_X, 
//								SCROLL_START_Y+g_pGameMain->m_nLeftWindowY,
//								SCROLL_LINE_LENGTH,
//								MAX_FRIEND_NUMBER);
//	m_pScroll->SetGameData( m_pGameData );
//	m_pScroll->InitDeviceObjects();
	{
		char  szScBall[30];	
		if(NULL == m_pScroll)
		{
			m_pScroll = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pScroll->InitDeviceObjects(MAX_FRIEND_NUMBER, szScBall);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

//	pDataHeader = m_pGameData->Find("Regover");
//	m_pImgButton[0] = new CINFImage;
//	m_pImgButton[0]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//	pDataHeader = m_pGameData->Find("Detover");
//	m_pImgButton[2] = new CINFImage;
//	m_pImgButton[2]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//	pDataHeader = m_pGameData->Find("Regpush");
//	m_pImgButton[1] = new CINFImage;
//	m_pImgButton[1]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
//	pDataHeader = m_pGameData->Find("Detpush");
//	m_pImgButton[3] = new CINFImage;
//	m_pImgButton[3]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );

	for(int i=0; i<MAX_FRIEND_NUMBER; i++)
	{
		m_pFontFriend[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
		m_pFontFriend[i]->InitDeviceObjects(g_pD3dDev);
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		wsprintf(szUpBtn, "Oapp3");
		wsprintf(szDownBtn, "Oapp1");
		wsprintf(szSelBtn, "Oapp0");
		wsprintf(szDisBtn, "Oapp2");
		if(NULL == m_pRegisterFriend)
		{
			m_pRegisterFriend = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			//m_pRegisterFriend->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			m_pRegisterFriend->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP56");
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		}
		
	}
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경			
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");															  
#else 
		wsprintf(szUpBtn, "ocancel3");
		wsprintf(szDownBtn, "ocancel1");
		wsprintf(szSelBtn, "ocancel0");
		wsprintf(szDisBtn, "ocancel2");
#endif
		if(NULL == m_pDelFriend)
		{
			m_pDelFriend = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			//m_pDelFriend->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);	
			m_pDelFriend->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP12");
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		}
		
	}	
	//TestDBFriendList();
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	return S_OK;
}

HRESULT CINFCommunityFriend::RestoreDeviceObjects()
{
	m_pBack->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();	
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 배경위치
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	// 스크롤 위치
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		// 스크롤
		m_pScroll->RestoreDeviceObjects();

		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 462;
		ptScrollPos.y += 123;
		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,180);
		rcMouseWhell.left		= ptScrollPos.x - 220;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 210;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 210;											  
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	if(m_pRegisterFriend)
	{
		m_pRegisterFriend->RestoreDeviceObjects();		
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 370;
		nPosY = ptBkPos.y + 357;		
		m_pRegisterFriend->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pDelFriend)
	{
		m_pDelFriend->RestoreDeviceObjects();		
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 386;
		nPosY = ptBkPos.y + 419;		
		m_pDelFriend->SetBtnPosition(nPosX, nPosY);	
	}											 
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
#else
	{
		// 스크롤
		m_pScroll->RestoreDeviceObjects();

		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 462;
		ptScrollPos.y += 123;

		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,180);
		rcMouseWhell.left		= ptScrollPos.x - 220;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 240;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 270;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	if(m_pRegisterFriend)
	{
		m_pRegisterFriend->RestoreDeviceObjects();		
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 317;
		nPosY = ptBkPos.y + 357;		
		m_pRegisterFriend->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pDelFriend)
	{
		m_pDelFriend->RestoreDeviceObjects();		
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 386;
		nPosY = ptBkPos.y + 357;		
		m_pDelFriend->SetBtnPosition(nPosX, nPosY);	
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
#endif

//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i]->RestoreDeviceObjects();
//	}

	int i=0;
	for(i=0; i<MAX_FRIEND_NUMBER; i++)
	{
		m_pFontFriend[i]->RestoreDeviceObjects();
	}

	return S_OK;
}
HRESULT CINFCommunityFriend::InvalidateDeviceObjects()
{
	m_pBack->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();
	m_pScroll->InvalidateDeviceObjects();

//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i]->InvalidateDeviceObjects();
//	}

	int i=0;
	for(i=0; i<MAX_FRIEND_NUMBER; i++)
	{
		m_pFontFriend[i]->InvalidateDeviceObjects();
	}
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pRegisterFriend)
	{		
		m_pRegisterFriend->InvalidateDeviceObjects();
	}
	if(m_pDelFriend)
	{		
		m_pDelFriend->InvalidateDeviceObjects();
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창


	return S_OK;
}

HRESULT CINFCommunityFriend::DeleteDeviceObjects()
{
	m_pBack->DeleteDeviceObjects();
	util::del(m_pBack);
	m_pImgSelect->DeleteDeviceObjects();
	util::del(m_pImgSelect);
	m_pScroll->DeleteDeviceObjects();
	util::del(m_pScroll);

//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i]->DeleteDeviceObjects();
//		util::del(m_pImgButton[i]);
//	}

	int i=0;
	for(i=0; i<MAX_FRIEND_NUMBER; i++)
	{
		m_pFontFriend[i]->DeleteDeviceObjects();
		util::del(m_pFontFriend[i]);
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pRegisterFriend)
	{		
		m_pRegisterFriend->DeleteDeviceObjects();
		util::del(m_pRegisterFriend);
	}
	if(m_pDelFriend)
	{		
		m_pDelFriend->DeleteDeviceObjects();
		util::del(m_pDelFriend);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	return S_OK;
}

void CINFCommunityFriend::Tick()
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//	if(m_pScroll)
//	{
//		m_pScroll->SetNumberOfData( m_vecFriend.size());
//		m_pScroll->SetScrollLinePos( SCROLL_START_X, g_pGameMain->m_nLeftWindowY+SCROLL_START_Y);
//		m_pScroll->SetWheelRect(LIST_BOX_START_X, 
//			g_pGameMain->m_nLeftWindowY+LIST_BOX_START_Y,
//			LIST_BOX_START_X+LIST_BOX_SIZE_X,
//	   	    g_pGameMain->m_nLeftWindowY+LIST_BOX_START_Y+LIST_BOX_SIZE_Y);
//	}
//	if(m_bDeleteOK)
//	{
//		int Index = m_pScroll->GetCurrentScrollIndex();				
//		//DeleteFriendList(m_vecFriend[m_nFriendSelect+Index-1].szFriendName);
//		
//		MSG_IC_CHAT_FRIENDLIST_DELETE sMsg;
//		strncpy(sMsg.szCharacterName, m_vecFriend[m_nFriendSelect+Index-1].szFriendName, SIZE_MAX_CHARACTER_NAME);
//		//sMsg.szCharacterName = m_vecFriend[m_nFriendSelect+Index-1].szFriendName;
//		g_pIMSocket->SendMsg( T_IC_CHAT_FRIENDLIST_DELETE, (char*)&sMsg, sizeof(sMsg) );
//		
//		m_bDeleteOK = FALSE;
//	}

	m_fRefreshTime -= g_pD3dApp->GetElapsedTime();
	if(m_fRefreshTime < 0)
	{
		m_fRefreshTime = REFRESH_TIME_DELAY;
		g_pIMSocket->WriteMessageType( T_IC_CHAT_FRIENDLIST_REFRESH);
	}
}

void CINFCommunityFriend::Render(POINT ptPos)
{
	int nWindowPosY = ptPos.y;
	//int nScrollIndex = m_pScroll->GetCurrentScrollIndex();
	//int nCurrentSelectWindowIndex = m_pScroll->GetCurrentSelectWindowIndex();

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//m_pBack->Move(BACK_START_X, nWindowPosY + BACK_START_Y );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBack->Move(ptPos.x, nWindowPosY + BACK_START_Y);
#else
	m_pBack->Move(ptPos.x, nWindowPosY);
#endif
	m_pBack->Render();

	
	if(m_nFriendSelect >= 0)
	{
		int nRenderLine = m_nFriendSelect - m_pScroll->GetScrollStep();
		if(nRenderLine >= 0 && nRenderLine < MAX_FRIEND_NUMBER)
		{
			int nSelPosX = ptPos.x + LIST_BOX_NAME_START_X -2 ;		
			int nSelPosY = ptPos.y + LIST_BOX_NAME_START_Y + (LIST_BOX_INTERVAL*nRenderLine)- 1;
			m_pImgSelect->Move(nSelPosX, nSelPosY);
			m_pImgSelect->Render();
		}	
	}

//	if(m_nButtonState)
//	{
//		if(m_nButtonState == BOTTON_STATE_1 || m_nButtonState == BOTTON_STATE_2)
//		{
//			m_pImgButton[m_nButtonState-1]->Move(ADD_FRIEND_START_X, ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY);
//			m_pImgButton[m_nButtonState-1]->Render();
//		}
//		else if(m_nButtonState == BOTTON_STATE_3 || m_nButtonState == BOTTON_STATE_4)
//		{
//			m_pImgButton[m_nButtonState-1]->Move(ADD_FRIEND_START_X + 36, ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY);
//			m_pImgButton[m_nButtonState-1]->Render();
//		}
//	}	
	///if(m_pScroll->GetCurrentSelectWindowIndex() >= 0)
	//{
	///	m_pImgSelect->Move(LIST_BOX_START_X+1, nWindowPosY + LIST_BOX_START_Y+LIST_BOX_INTERVAL*m_pScroll->GetCurrentSelectWindowIndex()+1);
	//	m_pImgSelect->Render();
	//}	

	int i=0;
	m_pScroll->Render();	
	////////////////////////////////////////////////////////////////////////////////////////
//
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_pRegisterFriend->Render();	
	m_pDelFriend->Render();	

	RenderFriendList(ptPos);
//	int Index = m_pScroll->GetCurrentScrollIndex()+1;
//	int count = m_vecFriend.size() - Index;
//
//	for(int loop=0; loop<MAX_FRIEND_NUMBER; loop++)
//	{
//		if(Index+loop > m_vecFriend.size()) return;	
//
//		m_pFontFriend[loop]->DrawText( LIST_BOX_NAME_START_X, 
//			nWindowPosY +            LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*loop, 
//			(loop+1) == m_nFriendSelect ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
//			m_vecFriend[Index+loop-1].szFriendName, 0L );
//
//		if(m_vecFriend[Index+loop-1].bOnline == FRIEND_STATE_ONLINE)
//		{
//			m_pFontFriend[loop]->DrawText( LIST_BOX_NAME_START_X + ON_OFF_LINE_WIDTH, 
//				nWindowPosY + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*loop,
//				(loop+1) == m_nFriendSelect ? GUI_SELECT_FONT_COLOR : FRIEND_MEMBER_ONLINE_COLOR,
//				"ON",0L);
//		}
//		else
//		{
//			m_pFontFriend[loop]->DrawText( LIST_BOX_NAME_START_X + ON_OFF_LINE_WIDTH, 
//				nWindowPosY + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*loop,
//				(loop+1) == m_nFriendSelect ? GUI_SELECT_FONT_COLOR : FRIEND_MEMBER_OFFLINE_COLOR,
//				"OFF",0L);
//		}
//
//	}
//
/////////////////////////////////////////////////////////////////////////////////////////	
}

int CINFCommunityFriend::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//	if(m_pScroll)
//	{
//		if(m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
//		{
//			return INF_MSGPROC_BREAK;
//		}
//	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);		

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//			if(pt.x >= ADD_FRIEND_START_X 
//				&& pt.x < ADD_FRIEND_START_X + ADD_FRIEND_WIDTH
//				&& pt.y >= ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY
//				&& pt.y < ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY +ADD_FRIEND_HEIGHT)
//			{	//  친구 추가
//				m_nButtonState = BOTTON_STATE_1;
//			}			
//			else if(pt.x >= ADD_FRIEND_START_X + 36
//				&& pt.x < ADD_FRIEND_START_X + ADD_FRIEND_WIDTH + 36
//				&& pt.y >= ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY
//				&& pt.y < ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY +ADD_FRIEND_HEIGHT)
//			{	// 친구 삭제
//				m_nButtonState = BOTTON_STATE_3;
//			}
//			else
//			{
//				m_nButtonState = BOTTON_STATE_0;				
//			}			
			m_pRegisterFriend->OnMouseMove(pt);
			m_pDelFriend->OnMouseMove(pt);
			{
				if(m_pScroll->GetMouseMoveMode())
				{
					if(FALSE == m_pScroll->IsMouseScrollPos(pt))
					{
						m_pScroll->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScroll->SetScrollPos(pt);
						return INF_MSGPROC_BREAK;
					}
				}
			}
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//			if(pt.x >= ADD_FRIEND_START_X 
//				&& pt.x < ADD_FRIEND_START_X + ADD_FRIEND_WIDTH
//				&& pt.y >= ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY
//				&& pt.y < ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY +ADD_FRIEND_HEIGHT)
//			{	//  친구 추가 클릭				
//				m_bAddFriendClick = TRUE;
//				m_nButtonState = BOTTON_STATE_2;
//				return INF_MSGPROC_BREAK;
//			}			
//			else if(pt.x >= ADD_FRIEND_START_X + 36
//				&& pt.x < ADD_FRIEND_START_X + ADD_FRIEND_WIDTH + 36
//				&& pt.y >= ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY
//				&& pt.y < ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY +ADD_FRIEND_HEIGHT)
//			{	// 친구 삭제 클릭
//				
//				m_bDeleteFriendClick = TRUE;
//				m_nButtonState = BOTTON_STATE_4;
//				return INF_MSGPROC_BREAK;
//			}
//			else
//			{
//				m_nButtonState = BOTTON_STATE_0;
//			}
//			// 친구 목록 클릭			
//			for(int list=1; list<MAX_FRIEND_NUMBER+1; list++)
//			{
//				if(pt.x >= SELECT_FRIEND_NAME_X 
//					&& pt.x < SELECT_FRIEND_NAME_X + SELECT_FRIEND_NAME_WIDTH
//					&& pt.y >= SELECT_FRIEND_NAME_Y + g_pGameMain->m_nLeftWindowY
//					&& pt.y < SELECT_FRIEND_NAME_Y + g_pGameMain->m_nLeftWindowY +(SELECT_FRIEND_NAME_HEIGHT*list))
//				{
//					m_nFriendSelect = 	list;
//					break;
//				}
//				else
//				{
//					m_nFriendSelect = 	0;					
//				}
//			}			
			POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
			{
				if(TRUE == m_pRegisterFriend->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pDelFriend->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				int nSelCursel = -1;
				int nCnt = 0;
				

				for(nCnt = 0; nCnt<MAX_FRIEND_NUMBER; nCnt++)
				{
					int nCheckPosX = (ptBkPos.x + LIST_BOX_NAME_START_X);
					int nCheckPosY = (ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nCnt);										
					if(pt.x >= nCheckPosX 
						&& pt.x < (nCheckPosX + SELECT_FRIEND_NAME_WIDTH)
						&& pt.y >= nCheckPosY
						&& pt.y < (nCheckPosY +LIST_BOX_INTERVAL))
					{
						nSelCursel = nCnt;						
						break;
					}					
				}
				if(nSelCursel>=0)
				{
					m_nFriendSelect = m_pScroll->GetScrollStep()+nSelCursel;					 
				}
				else
				{
					// 아무것도 선택안됨
					m_nFriendSelect = -1;
				}
			}
			
			{
				BOOL bClick = m_pScroll->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScroll->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창


			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
			// 창안에 마우스 클릭시 무효			
			if((pt.y >= ptBkPos.y )
				&&(pt.y <= (ptBkPos.y + 404))
				&&(pt.x >= ptBkPos.x)
				&&(pt.x <= (ptBkPos.x+518)))
			{
				return INF_MSGPROC_BREAK;
			}
//			if(pt.y>g_pGameMain->m_nLeftWindowY &&
//				pt.y<g_pGameMain->m_nLeftWindowY+SIZE_NORMAL_WINDOW_X &&
//				pt.x>0 &&
//				pt.x<SIZE_NORMAL_WINDOW_X)
//			{
//				return INF_MSGPROC_BREAK;
//			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//			for(int list=1; list<MAX_FRIEND_NUMBER+1; list++)
//			{
//				if(pt.x >= SELECT_FRIEND_NAME_X 
//					&& pt.x < SELECT_FRIEND_NAME_X + SELECT_FRIEND_NAME_WIDTH
//					&& pt.y >= SELECT_FRIEND_NAME_Y + g_pGameMain->m_nLeftWindowY
//					&& pt.y < SELECT_FRIEND_NAME_Y + g_pGameMain->m_nLeftWindowY +(SELECT_FRIEND_NAME_HEIGHT*list))
//				{
//					int Index = m_pScroll->GetCurrentScrollIndex();
//					if(m_vecFriend.size() < list+Index-1)
//					{
//						break;
//					}
//					else if(m_vecFriend[list+Index-1].szFriendName && m_vecFriend[m_nFriendSelect+Index-1].bOnline == TRUE)
//					{						
//						m_bWisperChatSend = TRUE;
//						g_pGameMain->m_pChat->ProcessWisperChatAddLine(" ", COLOR_CHAT_PTOP, m_vecFriend[m_nFriendSelect+Index-1].szFriendName, m_vecFriend[m_nFriendSelect+Index-1].szFriendName);
//						break;
//					}
//				}
//			}
			// 2009-01-14 by bhsohn 친구 리스트 귓속말 기능 추가
			{				
				int nFriendIdx = m_nFriendSelect;
				if((nFriendIdx>=0)
					&& (nFriendIdx < (int)m_vecFriend.size()))
					
				{
					if(m_vecFriend[nFriendIdx].bOnline)
					{
						SetWisperChatSend(TRUE);						
						g_pGameMain->m_pChat->ProcessWisperChatAddLine(" ", COLOR_CHAT_PTOP, 
							m_vecFriend[nFriendIdx].szFriendName, 
							m_vecFriend[nFriendIdx].szFriendName);						
						return INF_MSGPROC_BREAK;
					}					
				}				
			}
			// end 2009-01-14 by bhsohn 친구 리스트 귓속말 기능 추가
		}
		break;
	case WM_LBUTTONUP:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//			if(pt.x >= ADD_FRIEND_START_X 
//				&& pt.x < ADD_FRIEND_START_X + ADD_FRIEND_WIDTH
//				&& pt.y >= ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY
//				&& pt.y < ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY +ADD_FRIEND_HEIGHT)
//			{	//  친구 추가 클릭
//				if(m_bAddFriendClick)
//				{
//					m_nButtonState = BOTTON_STATE_0;
//					// 2008-02-27 by bhsohn 통합 아레나 수정
//					if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
//					{
//						//"아레나에서는 이용할 수 없습니다."
//						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
//						break;
//					}
//					// end 2008-02-27 by bhsohn 통합 아레나 수정
//					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_COMMUNITY_0001 , _Q_ADD_FRIEND);//"친구로 추가할 아이디를 입력하십시요."
//				}
//				break;
//			}			
//			if( m_bDeleteFriendClick
//				&& pt.x >= ADD_FRIEND_START_X + 36
//				&& pt.x < ADD_FRIEND_START_X + ADD_FRIEND_WIDTH + 36
//				&& pt.y >= ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY
//				&& pt.y < ADD_FRIEND_START_Y + g_pGameMain->m_nLeftWindowY +ADD_FRIEND_HEIGHT)
//			{	// 친구 삭제 클릭
//				char buf[64];
//				
//				int Index = m_pScroll->GetCurrentScrollIndex();
//				if(m_vecFriend.size()-Index > m_nFriendSelect-1)
//				{
//					m_nButtonState = BOTTON_STATE_0;
//					// 2008-02-27 by bhsohn 통합 아레나 수정
//					if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
//					{
//						//"아레나에서는 이용할 수 없습니다."
//						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
//						break;
//					}
//					// end 2008-02-27 by bhsohn 통합 아레나 수정
//
//					wsprintf(buf, STRMSG_C_COMMUNITY_0002,m_vecFriend[m_nFriendSelect+Index-1].szFriendName);//"%s를(을) 삭제 하시겠습니까?"
//					g_pGameMain->m_pInfWindow->AddMsgBox(buf , _Q_DELETE_FRIEND);
//				}
//
//				break;						
//			}				
			{
				if(TRUE == m_pRegisterFriend->OnLButtonUp(pt))
				{		
					// 버튼 클릭 		
					OnClickFriendRegist();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				if(TRUE == m_pDelFriend->OnLButtonUp(pt))
				{		
					// 버튼 클릭 			
					OnClickFriendDel();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}

			{
				BOOL bClick = m_pScroll->GetMouseMoveMode();
				if(bClick)
				{
					m_pScroll->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
			// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
		}
		break;
		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);
			BOOL bClick = m_pScroll->IsMouseWhellPos(pt);
			if(bClick)		
			{			
				m_pScroll->OnMouseWheel(wParam, lParam);	
				return INF_MSGPROC_BREAK;
			}
		}
		break;	
		// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	}	
	return INF_MSGPROC_NORMAL;
}

void CINFCommunityFriend::OnButtonClicked(int nButton)
{
/*	switch(nButton)
	{
	}
*/
}

void CINFCommunityFriend::DeleteFriendList(char *FriendName)
{
	vector<Friend_t>::iterator it = m_vecFriend.begin();
	while(it != m_vecFriend.end())
	{
		if(strcmp( it->szFriendName, FriendName ) == 0)
		{
			m_vecFriend.erase(it);
			break;
		}

		it++;
	}
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
void CINFCommunityFriend::RenderFriendList(POINT ptBkPos)
{
	vector<Friend_t>::iterator it = m_vecFriend.begin();
	int nCnt=0;	
	for(nCnt=0; nCnt < m_pScroll->GetScrollStep();nCnt++)
	{
		if(it != m_vecFriend.end())
		{			
			it++;
		}
	}		
	
	int nLine = 0;	
	int nX,nY;	
	nX = nY = 0;
	int nRenderLine = m_nFriendSelect - m_pScroll->GetScrollStep();

	int nOnFriendCnt = GetFriendListOnCount();

	//if(nOnFriendCnt > 0)
	{
		char chTmpBuff[128];
		// 2012-10-17 by jhjang 해피아워 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
		float fFirendBuf = min(nOnFriendCnt*FRIEND_BONUS,MAX_FRIEND_BONUS);
#else 
		float fFirendBuf = nOnFriendCnt*FRIEND_BONUS;
#endif//S_BONUSEXPSYSTEM_RENEWAL
		// end 2012-10-17 by jhjang 해피아워 시스템 리뉴얼
		sprintf(chTmpBuff, "%.2f%%", fFirendBuf);
		m_pFontFriend[0]->DrawText( ptBkPos.x + LIST_FRIEND_START_X, 
									ptBkPos.y + LIST_FRIEND_START_Y, 
										GUI_FONT_COLOR,
										chTmpBuff, 0L );
	}

	while(it != m_vecFriend.end())
	{
		Friend_t friendInto = (*it);
		if(nLine >= MAX_FRIEND_NUMBER)
		{
			break;
		}
		m_pFontFriend[nLine]->DrawText( ptBkPos.x + LIST_BOX_NAME_START_X, 
										ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nLine, 
										GUI_FONT_COLOR,
										friendInto.szFriendName, 0L );

		if(friendInto.bOnline == FRIEND_STATE_ONLINE)
		{
			m_pFontFriend[nLine]->DrawText( ptBkPos.x + LIST_BOX_NAME_START_X + ON_OFF_LINE_WIDTH, 
				ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nLine,
									FRIEND_MEMBER_ONLINE_COLOR,
				"ON",0L);
		}
		else
		{
			m_pFontFriend[nLine]->DrawText( ptBkPos.x + LIST_BOX_NAME_START_X + ON_OFF_LINE_WIDTH, 
				ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nLine,
									FRIEND_MEMBER_OFFLINE_COLOR,
				"OFF",0L);
		}

		nLine++;		
		it++;
	}
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
void CINFCommunityFriend::TestDBFriendList()
{
	Friend_t temp_friend;
	int nCnt = 0;
	for(nCnt = 0;nCnt < 20;nCnt++)
	{
		memset(&temp_friend, 0x00, sizeof(Friend_t));

		wsprintf(temp_friend.szFriendName, "Friend%d", nCnt);
		temp_friend.bOnline = (nCnt%2);		
		AddFriendList(temp_friend);
	}
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
void CINFCommunityFriend::AddFriendList(Friend_t friendInfo)
{
	Friend_t temp_friend;
	memcpy(&temp_friend, &friendInfo,sizeof(Friend_t));
	m_vecFriend.push_back(temp_friend);

	m_pScroll->SetMaxItem(m_vecFriend.size());
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
void CINFCommunityFriend::ResetFriendList()
{
	m_vecFriend.clear();	
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
void CINFCommunityFriend::OnClickFriendRegist()
{	
	if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
	{
		
		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 		//"아레나에서는 이용할 수 없습니다."
// 		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."

		if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
		else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

		return ;
	}	
	g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_COMMUNITY_0001 , _Q_ADD_FRIEND);//"친구로 추가할 아이디를 입력하십시요."
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
void CINFCommunityFriend::OnClickFriendDel()
{
	if(m_nFriendSelect< 0)
	{
		return;
	}
	
	int nFriendIdx = m_nFriendSelect;
	if(nFriendIdx >= m_vecFriend.size())
	{
		return;
	}
	
	if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
	{
		
		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

// 		//"아레나에서는 이용할 수 없습니다."
// 		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
		if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_ARENA )
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
		else if( g_pD3dApp->m_ArenaSocketType == CAtumApplication::ARENASOCKET_INFINITY )
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_091103_0322,COLOR_ERROR);//"\y인피니티 필드에서는 사용 할 수 없습니다\y"

		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

		return ;
	}
	
	// 2009. 06. 25 by ckPark 친구 삭제시 튕기던 버그
	
	//char buf[64];
	char buf[256];
	
	// end 2009. 06. 25 by ckPark 친구 삭제시 튕기던 버그

	wsprintf(buf, STRMSG_C_COMMUNITY_0002,m_vecFriend[nFriendIdx].szFriendName);//"%s를(을) 삭제 하시겠습니까?"
	g_pGameMain->m_pInfWindow->AddMsgBox(buf , _Q_DELETE_FRIEND);	
	
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
void CINFCommunityFriend::SendFriendDelete()
{
	if(m_nFriendSelect< 0)
	{
		return;
	}
	
	int nFriendIdx = m_nFriendSelect;
	if(nFriendIdx >= m_vecFriend.size())
	{
		return;
	}	
	MSG_IC_CHAT_FRIENDLIST_DELETE sMsg;
	strncpy(sMsg.szCharacterName, m_vecFriend[nFriendIdx].szFriendName, SIZE_MAX_CHARACTER_NAME);	
	g_pIMSocket->SendMsg( T_IC_CHAT_FRIENDLIST_DELETE, (char*)&sMsg, sizeof(sMsg) );
	
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
BOOL CINFCommunityFriend::IsWisperChatSend()
{
	return m_bWisperChatSend;
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
void CINFCommunityFriend::SetWisperChatSend(BOOL bWisperChatSend)
{
	m_bWisperChatSend = bWisperChatSend;
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
void CINFCommunityFriend::FriendListRefresh(char* szFriendName, BYTE byIsOnline)
{
	vector<Friend_t>::iterator it = m_vecFriend.begin();
	while(it != m_vecFriend.end())
	{
		if(0 == strncmp(szFriendName, it->szFriendName, SIZE_MAX_CHARACTER_NAME))
		{
			it->bOnline			= byIsOnline;
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		접속해 있는 친구 리스트
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCommunityFriend::GetFriendListOnCount()
{
	return g_pD3dApp->GetFriendListOnCount();	
}

void CINFCommunityFriend::UpdateBtnPos()
{
	// 배경위치
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 459;
		ptScrollPos.y += 122;
		
		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,199);
		rcMouseWhell.left		= ptScrollPos.x - 210;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 210;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 210;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 317;
		nPosY = ptBkPos.y + 342;
		m_pRegisterFriend->SetBtnPosition(nPosX, nPosY);	
	}
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 386;
		nPosY = ptBkPos.y + 342;		
		m_pDelFriend->SetBtnPosition(nPosX, nPosY);	
	}
#else
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 462;
		ptScrollPos.y += 123;
		
		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,180);
		rcMouseWhell.left		= ptScrollPos.x - 220;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 240;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 270;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 317;
		nPosY = ptBkPos.y + 357;		
		m_pRegisterFriend->SetBtnPosition(nPosX, nPosY);	
	}
	{
		int nPosX, nPosY;
		nPosX = ptBkPos.x + 386;
		nPosY = ptBkPos.y + 357;		
		m_pDelFriend->SetBtnPosition(nPosX, nPosY);	
	}

#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCommunityFriend::IsMyFriend(char* i_pUser)
{
	vector<Friend_t>::iterator it = m_vecFriend.begin();
	while(it != m_vecFriend.end())
	{
		if(0 == stricmp(i_pUser, it->szFriendName))	// 2015-12-04 Future, fixed bugged Friend checking
		{
			return TRUE;
		}
		it++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityFriend::SetRqFriend(char* i_pUser)
{
	strncpy(m_szRqFriendName, i_pUser, SIZE_MAX_CHARACTER_NAME);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-12 by bhsohn 친구 등록 당하는 유저 메시지창 기능 추가
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char* CINFCommunityFriend::GetRqFriend()
{
	return m_szRqFriendName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-14 by bhsohn 친구 리스트 귓속말 기능 추가
/// \date		2009-01-14 ~ 2009-01-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityFriend::RefreshFriendInfo()
{
	m_fRefreshTime	= 2.0f;
}
