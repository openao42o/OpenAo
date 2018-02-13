// INFCommunityReject.cpp: implementation of the CINFCommunityReject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCommunityReject.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "INFGameMain.h"
#include "GameDataLast.h"
#include "INFScrollBar.h"
#include "D3DHanFont.h"
#include "INFWindow.h"
// 2007-11-22 by bhsohn 아레나 통합서버
#include "IMSocketManager.h"
#include "INFArenaScrollBar.h"
#include "INFCommunity.h"
#include "INFImageBtn.h"
#include "INFEditBox.h"
#include "AtumSound.h"
//#include "IMSocket.h"

#include "dxutil.h"

#include "Chat.h"

#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"							  // 2011. 10. 10 by jskim UI시스템 변경


#define SCROLL_START_X						197
#define SCROLL_START_Y						55
#define SCROLL_LINE_LENGTH					172

#define LIST_BOX_START_X					19
#define LIST_BOX_START_Y					72
#define LIST_BOX_SIZE_X						171
#define LIST_BOX_SIZE_Y						154
#define LIST_BOX_INTERVAL					17

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//#define LIST_BOX_NAME_START_X				22
#define LIST_BOX_NAME_START_X				244
#define LIST_BOX_NAME_START_Y				191													  
#else
// 2008-04-04 by bhsohn Ep3 커뮤니티 창
//#define LIST_BOX_NAME_START_X				22
#define LIST_BOX_NAME_START_X				247
#define LIST_BOX_NAME_START_Y				198
#endif

#define BACK_START_X						13
#define BACK_START_Y						30

#define REJECT_MEMBER_ONLINE_COLOR			RGB(0, 0, 222)
#define REJECT_MEMBER_OFFLINE_COLOR			RGB(222, 0, 0)

// 친구 설정
#define ADD_REJECT_START_X					BACK_START_X + 106
#define ADD_REJECT_START_Y					BACK_START_Y + 201
#define ADD_REJECT_WIDTH					35
#define ADD_REJECT_HEIGHT					15

#define SELECT_REJECT_NAME_X				18
#define SELECT_REJECT_NAME_Y				54
#define SELECT_REJECT_NAME_WIDTH			171
#define SELECT_REJECT_NAME_HEIGHT			17
#define SELECT_REJECT_MACRO_X				19
#define SELECT_REJECT_MACRO_Y				38

#define BOTTON_STATE_0						0
#define BOTTON_STATE_1						1
#define BOTTON_STATE_2						2
#define BOTTON_STATE_3						3
#define BOTTON_STATE_4						4

// 2006-03-07 by ispark, 언어에 따라 위치 수정
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
//#define LIST_BOX_NAME_START_Y				55//57
//#else
//#define LIST_BOX_NAME_START_Y				55//57
//#endif
// Edit 컨트롤 (Pass)
#define		REJECT_WND_USERID_EDIT_X		355
#define		REJECT_WND_USERID_EDIT_Y		100
#define		REJECT_WND_USERID_EDIT_W		100
#define		REJECT_WND_USERID_EDIT_H		20


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityReject::CINFCommunityReject(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_pBack = NULL;

	m_pImgSelect			= NULL;	
	m_pScroll				= NULL;

	m_bAddRejectClick		= FALSE;
	m_bDeleteRejectClick	= FALSE;
	//m_nRejectSelect			= 0;
	m_nRejectSelect			= -1;
	//m_bDeleteOK				= FALSE;
	m_nButtonState			= 0;

//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i] = NULL;
//	}
	int i=0;
	for(i=0; i<MAX_REJECT_NUMBER; i++)
	{
		m_pFontReject[i] = NULL;
	}

// 2006-03-07 by ispark, Define에 따라 분류
//	switch (PRIMARYLANGID(g_input.GetLangId()))
//	{
//	case LANG_KOREAN:
//		//한국어
//		{
//#ifdef LANGUAGE_ENGLISH
//			m_nRejectRenderX = ADD_REJECT_START_X;
//			m_nRejectRenderY = ADD_REJECT_START_Y;
//#else
//			m_nRejectRenderX = ADD_REJECT_START_X+12;
//			m_nRejectRenderY = ADD_REJECT_START_Y-1;
//#endif
//		}
//		break;
//	case LANG_JAPANESE:
//		{
//			m_nRejectRenderX = ADD_REJECT_START_X;
//			m_nRejectRenderY = ADD_REJECT_START_Y;
//		}
//		break;
//	default:
//		{
//			m_nRejectRenderX = ADD_REJECT_START_X;
//			m_nRejectRenderY = ADD_REJECT_START_Y;
//		}
//
//	}
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	m_nRejectRenderX = ADD_REJECT_START_X;
	m_nRejectRenderY = ADD_REJECT_START_Y;
#else
	m_nRejectRenderX = ADD_REJECT_START_X+12;
	m_nRejectRenderY = ADD_REJECT_START_Y-1;
#endif

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	m_pRegisterReject = NULL ;	// 등록
	m_pDelReject = NULL ;		// 삭제
	m_pINFUserName = NULL;

	// 2008-10-24 by bhsohn 친구, 거부 리스트 49명만 되는 현상 처리
	m_vecReject.clear();
}

CINFCommunityReject::~CINFCommunityReject()
{
	util::del(m_pBack);
	util::del(m_pImgSelect);
	util::del(m_pScroll);

//	for(int i=0; i<4; i++)
//	{
//		util::del(m_pImgButton[i]);
//	}
	int i=0;
	for(i=0; i<MAX_REJECT_NUMBER; i++)
	{
		util::del(m_pFontReject[i]);
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	util::del(m_pRegisterReject);	// 적용
	util::del(m_pDelReject);	
	util::del(m_pINFUserName);
}

HRESULT CINFCommunityReject::InitDeviceObjects()
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//DataHeader *pDataHeader = m_pGameData->Find("rjback");
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 	DataHeader *pDataHeader = m_pGameData->Find("commubk5");
// 	m_pBack = new CINFImage;
// 	m_pBack->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
    DataHeader* pDataHeader;
	if(m_pBack == NULL)
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("commu_R");	
		m_pBack = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pBack->RestoreDeviceObjects();
	}
#else 
	DataHeader *pDataHeader = m_pGameData->Find("commubk5");
	m_pBack = new CINFImageEx;
	m_pBack->InitDeviceObjects( pDataHeader );
#endif
	//pDataHeader = m_pGameData->Find("frselect");
	pDataHeader = m_pGameData->Find("rejselect");	
	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );

//	m_pScroll = new CINFScrollBar(this,
//								SCROLL_START_X, 
//								SCROLL_START_Y+g_pGameMain->m_nLeftWindowY,
//								SCROLL_LINE_LENGTH,
//								MAX_REJECT_NUMBER);
//	m_pScroll->SetGameData( m_pGameData );
//	m_pScroll->InitDeviceObjects();
	{
		char  szScBall[30];	
		if(NULL == m_pScroll)
		{
			m_pScroll = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pScroll->InitDeviceObjects(MAX_REJECT_NUMBER, szScBall);
	}
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

	for(int i=0; i<MAX_REJECT_NUMBER; i++)
	{
		m_pFontReject[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
		m_pFontReject[i]->InitDeviceObjects(g_pD3dDev);
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];				
		wsprintf(szUpBtn, "Oapp3");
		wsprintf(szDownBtn, "Oapp1");
		wsprintf(szSelBtn, "Oapp0");
		wsprintf(szDisBtn, "Oapp2");
		if(NULL == m_pRegisterReject)
		{
			m_pRegisterReject = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			//m_pRegisterReject->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			m_pRegisterReject->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP56");		
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
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
		if(NULL == m_pDelReject)
		{
			m_pDelReject = new CINFImageBtn;
			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
			//m_pDelReject->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
			m_pDelReject->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP12");
			// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		}
		
	}	
	{
		if(NULL == m_pINFUserName)
		{
			m_pINFUserName = new CINFEditBox;
		}
		
		POINT ptPos = {REJECT_WND_USERID_EDIT_X, REJECT_WND_USERID_EDIT_Y};
		m_pINFUserName->InitDeviceObjects(9, 
											ptPos, 
											REJECT_WND_USERID_EDIT_W, 
											TRUE, 
											REJECT_WND_USERID_EDIT_H, 
											TRUE);		
		m_pINFUserName->SetStringMaxBuff(SIZE_MAX_CHARACTER_NAME-1);
	}
	//TestDBRejectList();
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창	

	return S_OK;
}

HRESULT CINFCommunityReject::RestoreDeviceObjects()
{
	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	// 배경위치
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	int nPosX, nPosY;

	{
		// 스크롤
		m_pScroll->RestoreDeviceObjects();
		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		ptScrollPos.x += 470;
		ptScrollPos.y += 191;

		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,151);  
#else 
		ptScrollPos.x += 471;
		ptScrollPos.y += 202;

		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,110);
#endif
		rcMouseWhell.left		= ptScrollPos.x - 220;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 160;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 140;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	if(m_pRegisterReject)
	{
		m_pRegisterReject->RestoreDeviceObjects();				
		nPosX = ptBkPos.x + 403;
		nPosY = ptBkPos.y + 129;		
		m_pRegisterReject->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pDelReject)
	{
		m_pDelReject->RestoreDeviceObjects();		
		
		nPosX = ptBkPos.x + 403;
		nPosY = ptBkPos.y + 357;		
		m_pDelReject->SetBtnPosition(nPosX, nPosY);	
	}
	{
		m_pINFUserName->RestoreDeviceObjects();		
		nPosX = ptBkPos.x + REJECT_WND_USERID_EDIT_X;
		nPosY = ptBkPos.y + REJECT_WND_USERID_EDIT_Y;
		m_pINFUserName->SetPos(nPosX, nPosY);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창

	m_pBack->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();
	

//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i]->RestoreDeviceObjects();
//	}
	int i=0;
	for(i=0; i<MAX_REJECT_NUMBER; i++)
	{
		m_pFontReject[i]->RestoreDeviceObjects();
	}

	return S_OK;
}

HRESULT CINFCommunityReject::InvalidateDeviceObjects()
{
	m_pBack->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();
	m_pScroll->InvalidateDeviceObjects();


//	for(int i=0; i<4; i++)
//	{
//		m_pImgButton[i]->InvalidateDeviceObjects();
//	}
	int i=0;
	for(i=0; i<MAX_REJECT_NUMBER; i++)
	{
		m_pFontReject[i]->InvalidateDeviceObjects();
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pRegisterReject)
	{		
		m_pRegisterReject->InvalidateDeviceObjects();
	}
	if(m_pDelReject)
	{		
		m_pDelReject->InvalidateDeviceObjects();
	}
	if(m_pINFUserName)
	{
		m_pINFUserName->InvalidateDeviceObjects();		
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창


	return S_OK;
}

HRESULT CINFCommunityReject::DeleteDeviceObjects()
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
	for(i=0; i<MAX_REJECT_NUMBER; i++)
	{
		m_pFontReject[i]->DeleteDeviceObjects();
		util::del(m_pFontReject[i]);
	}

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	if(m_pRegisterReject)
	{		
		m_pRegisterReject->DeleteDeviceObjects();
		util::del(m_pRegisterReject);
	}
	if(m_pDelReject)
	{		
		m_pDelReject->DeleteDeviceObjects();
		util::del(m_pDelReject);
	}
	if(m_pINFUserName)
	{
		m_pINFUserName->DeleteDeviceObjects();	
		util::del(m_pINFUserName);
	}
	// end 2008-04-04 by bhsohn Ep3 커뮤니티 창


	return S_OK;
}

void CINFCommunityReject::Tick()
{
//	if(m_pScroll)
//	{
//		m_pScroll->SetNumberOfData( m_vecReject.size());
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
//		MSG_IC_CHAT_REJECTLIST_DELETE sMsg;
//		strncpy(sMsg.szCharacterName, m_vecReject[m_nRejectSelect+Index-1].szRejectName, SIZE_MAX_CHARACTER_NAME);
//		//sMsg.szCharacterName = m_vecFriend[m_nFriendSelect+Index-1].szFriendName;
//		g_pIMSocket->SendMsg( T_IC_CHAT_REJECTLIST_DELETE, (char*)&sMsg, sizeof(sMsg) );
//		
//		m_bDeleteOK = FALSE;
//	}
	m_pINFUserName->Tick();

}

void CINFCommunityReject::Render(POINT ptPos)
{
	int nWindowPosY = ptPos.y;
//	int nScrollIndex = m_pScroll->GetCurrentScrollIndex();
//	int nCurrentSelectWindowIndex = m_pScroll->GetCurrentSelectWindowIndex();

	// 2008-04-04 by bhsohn Ep3 커뮤니티 창
	//m_pBack->Move(BACK_START_X, nWindowPosY + BACK_START_Y );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBack->Move(ptPos.x, ptPos.y + 25);														  
#else 

	m_pBack->Move(ptPos.x, ptPos.y);
#endif
	m_pBack->Render();

		
	if(m_nRejectSelect >= 0)
	{
		int nRenderLine = m_nRejectSelect - m_pScroll->GetScrollStep();
		if(nRenderLine >= 0 && nRenderLine < MAX_REJECT_NUMBER)
		{
			int nSelPosX = ptPos.x + LIST_BOX_NAME_START_X -6 ;		
			int nSelPosY = ptPos.y + LIST_BOX_NAME_START_Y + (LIST_BOX_INTERVAL*nRenderLine)- 1;
			m_pImgSelect->Move(nSelPosX, nSelPosY);
			m_pImgSelect->Render();
		}			
	}
//	if(m_nButtonState)
//	{
//		if(m_nButtonState == BOTTON_STATE_1 || m_nButtonState == BOTTON_STATE_2)
//		{
//			m_pImgButton[m_nButtonState-1]->Move(m_nRejectRenderX, m_nRejectRenderY + g_pGameMain->m_nLeftWindowY);
//			m_pImgButton[m_nButtonState-1]->Render();
//		}
//		else if(m_nButtonState == BOTTON_STATE_3 || m_nButtonState == BOTTON_STATE_4)
//		{
//			m_pImgButton[m_nButtonState-1]->Move(m_nRejectRenderX + 36, m_nRejectRenderY + g_pGameMain->m_nLeftWindowY);
//			m_pImgButton[m_nButtonState-1]->Render();
//		}
//	}
	///if(m_pScroll->GetCurrentSelectWindowIndex() >= 0)
	//{
	///	m_pImgSelect->Move(LIST_BOX_START_X+1, nWindowPosY + LIST_BOX_START_Y+LIST_BOX_INTERVAL*m_pScroll->GetCurrentSelectWindowIndex()+1);
	//	m_pImgSelect->Render();
	//}	

	// 친구이름 렌더링
	COLORREF fontColor;
	//if(nGuildState == GUILD_STATE_NORMAL)
	//{
		fontColor = GUI_FONT_COLOR;
	//}
	//else
	//{
	//	fontColor = GUI_FONT_COLOR_DISMEMBER_READY;
	//}

	int i=0;
	m_pScroll->Render();	
	m_pRegisterReject->Render();	
	m_pDelReject->Render();	
	m_pINFUserName->Render();

	RenderRejectList(ptPos);
	/*
	vector<Reject_t>::iterator it = m_vecReject.begin();
	while(it != m_vecReject.end())
	{
		if(nScrollIndex==0)
		{
			it->pFontReject->DrawText( LIST_BOX_NAME_START_X, 
				nWindowPosY +            LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*i, 
				(i+1) == m_nRejectSelect ? GUI_SELECT_FONT_COLOR : fontColor,
				it->szRejectName, 0L );
			i++;

			if(i == MAX_REJECT_NUMBER)
			{
				return;
			}
		}
		else
		{
			nScrollIndex--;
		}
		it++;		
	}*/

//	int Index = m_pScroll->GetCurrentScrollIndex()+1;
//	int count = m_vecReject.size() - Index;
//
//	for(int loop=0; loop<MAX_REJECT_NUMBER; loop++)
//	{
//		if(Index+loop > m_vecReject.size()) return;	
//
//		m_pFontReject[loop]->DrawText( LIST_BOX_NAME_START_X, 
//			nWindowPosY +            LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*loop, 
//			(loop+1) == m_nRejectSelect ? GUI_SELECT_FONT_COLOR : fontColor,
//			m_vecReject[Index+loop-1].szRejectName, 0L );
//
//	}
}

int CINFCommunityReject::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	
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

//			if(pt.x >= m_nRejectRenderX 
//				&& pt.x < m_nRejectRenderX + ADD_REJECT_WIDTH
//				&& pt.y >= m_nRejectRenderY + g_pGameMain->m_nLeftWindowY
//				&& pt.y < m_nRejectRenderY + g_pGameMain->m_nLeftWindowY +ADD_REJECT_HEIGHT)
//			{	//  친구 추가
//				m_nButtonState = BOTTON_STATE_1;
//			}			
//			else if(pt.x >= m_nRejectRenderX + 36
//				&& pt.x < m_nRejectRenderX + ADD_REJECT_WIDTH + 36
//				&& pt.y >= m_nRejectRenderY + g_pGameMain->m_nLeftWindowY
//				&& pt.y < m_nRejectRenderY + g_pGameMain->m_nLeftWindowY +ADD_REJECT_HEIGHT)
//			{	// 친구 삭제
//				m_nButtonState = BOTTON_STATE_3;
//			}
//			else
//			{
//				m_nButtonState = BOTTON_STATE_0;				
//			}
			m_pRegisterReject->OnMouseMove(pt);
			m_pDelReject->OnMouseMove(pt);

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
		}
		break;
	case WM_LBUTTONDOWN:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

//			if(pt.x >= m_nRejectRenderX 
//				&& pt.x < m_nRejectRenderX + ADD_REJECT_WIDTH
//				&& pt.y >= m_nRejectRenderY + g_pGameMain->m_nLeftWindowY
//				&& pt.y < m_nRejectRenderY + g_pGameMain->m_nLeftWindowY +ADD_REJECT_HEIGHT)
//			{	//  거부 추가 클릭				
//				m_bAddRejectClick = TRUE;
//				m_nButtonState = BOTTON_STATE_2;
//				return INF_MSGPROC_BREAK;
//			}			
//			else if(pt.x >= m_nRejectRenderX + 36
//				&& pt.x < m_nRejectRenderX + ADD_REJECT_WIDTH + 36
//				&& pt.y >= m_nRejectRenderY + g_pGameMain->m_nLeftWindowY
//				&& pt.y < m_nRejectRenderY + g_pGameMain->m_nLeftWindowY +ADD_REJECT_HEIGHT)
//			{	// 친구 삭제 클릭
//				
//				m_bDeleteRejectClick = TRUE;
//				m_nButtonState = BOTTON_STATE_4;				
//				return INF_MSGPROC_BREAK;
//			}
//			else
//			{
//				m_nButtonState = BOTTON_STATE_0;
//			}
//
//			// 친구 목록 클릭
//			for(int list=1; list<MAX_REJECT_NUMBER+1; list++)
//			{
//				if(pt.x >= SELECT_REJECT_NAME_X 
//					&& pt.x < SELECT_REJECT_NAME_X + SELECT_REJECT_NAME_WIDTH
//					&& pt.y >= SELECT_REJECT_NAME_Y + g_pGameMain->m_nLeftWindowY
//					&& pt.y < SELECT_REJECT_NAME_Y + g_pGameMain->m_nLeftWindowY +(SELECT_REJECT_NAME_HEIGHT*list))
//				{
//					m_nRejectSelect = 	list;
//					break;
//				}
//				else
//				{
//					m_nRejectSelect = 	0;					
//				}
//			}
			POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
			{
				if(TRUE == m_pINFUserName->OnLButtonDown(pt))
				{				
					m_pINFUserName->EnableEdit(TRUE, TRUE);
					
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}
				else
				{
					//  에디트 컨트롤 닫음
					m_pINFUserName->EnableEdit(FALSE, FALSE);
				}
			}
			{
				if(TRUE == m_pRegisterReject->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pDelReject->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				int nSelCursel = -1;
				int nCnt = 0;				

				for(nCnt = 0; nCnt<MAX_REJECT_NUMBER; nCnt++)
				{
					int nCheckPosX = (ptBkPos.x + LIST_BOX_NAME_START_X);
					int nCheckPosY = (ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nCnt);										
					if(pt.x >= nCheckPosX 
						&& pt.x < (nCheckPosX + SELECT_REJECT_NAME_WIDTH)
						&& pt.y >= nCheckPosY
						&& pt.y < (nCheckPosY +SELECT_REJECT_NAME_HEIGHT))
					{
						nSelCursel = nCnt;						
						break;
					}					
				}				 
				if(nSelCursel>=0)
				{
					m_nRejectSelect = m_pScroll->GetScrollStep()+nSelCursel;					 
				}
				else
				{
					// 아무것도 선택안됨
					m_nRejectSelect = -1;
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

			// 2005-08-26 by ispark
			// 창안에 마우스 클릭시 무효
			// 창안에 마우스 클릭시 무효			
			if((pt.y >= ptBkPos.y )
				&&(pt.y <= (ptBkPos.y + 404))
				&&(pt.x >= ptBkPos.x)
				&&(pt.x <= (ptBkPos.x+518)))
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

//			if(pt.x >= m_nRejectRenderX 
//				&& pt.x < m_nRejectRenderX + ADD_REJECT_WIDTH
//				&& pt.y >= m_nRejectRenderY + g_pGameMain->m_nLeftWindowY
//				&& pt.y < m_nRejectRenderY + g_pGameMain->m_nLeftWindowY +ADD_REJECT_HEIGHT)
//			{	
//				// 2008-02-27 by bhsohn 통합 아레나 수정
//				if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING )
//				{
//					//"아레나에서는 이용할 수 없습니다."
//					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0203,COLOR_ERROR);//"아레나에서는 이용할 수 없습니다."
//					m_nButtonState = BOTTON_STATE_0;
//					return INF_MSGPROC_NORMAL;
//				}
//				// end 2008-02-27 by bhsohn 통합 아레나 수정
//
//				//  친구 추가 클릭
//				if(m_bAddRejectClick)
//				{					
//					g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_COMMUNITY_0003 , _Q_ADD_REJECT);//"거부 목록에 추가할 아이디를 입력하십시요."
//				}
//				m_nButtonState = BOTTON_STATE_0;
//				break;
//			}
//			if( m_bDeleteRejectClick
//				&& pt.x >= m_nRejectRenderX + 36
//				&& pt.x < m_nRejectRenderX + ADD_REJECT_WIDTH + 36
//				&& pt.y >= m_nRejectRenderY + g_pGameMain->m_nLeftWindowY
//				&& pt.y < m_nRejectRenderY + g_pGameMain->m_nLeftWindowY +ADD_REJECT_HEIGHT)
//			{	// 친구 삭제 클릭
//				// 2006-06-15 by ispark, 밑에 buf 크기가 잘못 되어서 Release 모드에서 죽는 버그 있었다.
//				char buf[1024];
//				
//				int Index = m_pScroll->GetCurrentScrollIndex();
//				if(m_vecReject.size()-Index > m_nRejectSelect-1)
//				{					
//					wsprintf(buf, STRMSG_C_COMMUNITY_0004,m_vecReject[m_nRejectSelect+Index-1].szRejectName);//"%s를(을) 거부목록에서 삭제 하시겠습니까?"
//					g_pGameMain->m_pInfWindow->AddMsgBox(buf , _Q_DELETE_REJECT);
//				}
//				m_nButtonState = BOTTON_STATE_0;
//				break;						
//			}

			{
				if(TRUE == m_pRegisterReject->OnLButtonUp(pt))
				{					
					OnClickReject();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
			}
			
			{
				if(TRUE == m_pDelReject->OnLButtonUp(pt))
				{					
					SendDeleteRejectList();
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
	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{
			if(m_pINFUserName->WndProc(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}			
		}
		break;	
	}	
	return INF_MSGPROC_NORMAL;
}

void CINFCommunityReject::OnButtonClicked(int nButton)
{
/*	switch(nButton)
	{
	}
*/
}
/*
void CINFCommunityReject::SetHanFont()
{
	
	Reject_t tempReject;
		
	memset(tempReject.szRejectName, 0x00, SIZE_MAX_REJECT_NAME);	
	memcpy(tempReject.szRejectName, RejectName, strlen(RejectName));
	tempReject.pFontReject	= new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
	tempReject.pFontReject->InitDeviceObjects(g_pD3dDev);
	tempReject.pFontReject->RestoreDeviceObjects();

	m_vecReject.push_back(tempReject);

	if(m_vecReject.size() > 10)
	{
		m_pScroll->SetIncreaseCurrent(1);
	}

	vector<Reject_t>::iterator it = m_vecReject.begin();
	while(it != m_vecReject.end())
	{
		if(it->pFontReject)
		{
			it->pFontReject->InvalidateDeviceObjects();	
			it->pFontReject->DeleteDeviceObjects();
		}
		it++;
	}
	
	it = m_vecReject.begin();
	while(it != m_vecReject.end())
	{
		it->pFontReject = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
		it->pFontReject->InitDeviceObjects(g_pD3dDev); 
		it->pFontReject->RestoreDeviceObjects(); 
		it++;
	}
}
*/
void CINFCommunityReject::DeleteRejectList(char *RejectName)
{
	vector<Reject_t>::iterator it = m_vecReject.begin();
	while(it != m_vecReject.end())
	{
		if(stricmp( it->szRejectName, RejectName ) == 0)
		{
			m_vecReject.erase(it);
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
void CINFCommunityReject::TestDBRejectList()
{
	int nCnt =0;
	Reject_t struRejTmp;
	for(nCnt =0;nCnt < 20; nCnt++)
	{
		memset(&struRejTmp, 0x00, sizeof(Reject_t));
		wsprintf(struRejTmp.szRejectName, "What%d", nCnt);
		AddRejectList(struRejTmp);
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
void CINFCommunityReject::AddRejectList(Reject_t struRej)
{
	Reject_t struRejTmp;
	memcpy(&struRejTmp, &struRej, sizeof(Reject_t));
	m_vecReject.push_back(struRejTmp);

	m_pScroll->SetMaxItem(m_vecReject.size());

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
void CINFCommunityReject::InitRejectList()
{
	m_vecReject.clear();
	m_pScroll->SetMaxItem(m_vecReject.size());

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
void CINFCommunityReject::RenderRejectList(POINT ptBkPos)
{
	vector<Reject_t>::iterator it = m_vecReject.begin();
	int nCnt=0;	
	for(nCnt=0; nCnt < m_pScroll->GetScrollStep();nCnt++)
	{
		if(it != m_vecReject.end())
		{			
			it++;
		}
	}		
	
	int nLine = 0;	
	int nX,nY;	
	nX = nY = 0;	
	int nRenderLine = m_nRejectSelect - m_pScroll->GetScrollStep();
	char chTimeBuff[256];
	memset(chTimeBuff, 0x00, 256);

	while(it != m_vecReject.end())
	{		
		if(nLine >= MAX_REJECT_NUMBER)
		{
			break;
		}
		Reject_t RejectInfo = (*it);
		m_pFontReject[nLine]->DrawText( ptBkPos.x + LIST_BOX_NAME_START_X, 
										ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nLine, 
										(nLine == nRenderLine) ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
										RejectInfo.szRejectName, 0L );		

		wsprintf(chTimeBuff, "%04d.%02d.%02d", RejectInfo.atimeRegTime.Year, 
												RejectInfo.atimeRegTime.Month, 
												RejectInfo.atimeRegTime.Day);
		m_pFontReject[nLine]->DrawText( ptBkPos.x + LIST_BOX_NAME_START_X + 110, 
										ptBkPos.y + LIST_BOX_NAME_START_Y + LIST_BOX_INTERVAL*nLine, 
										(nLine == nRenderLine) ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
										chTimeBuff, 0L );		

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
void CINFCommunityReject::OnClickReject()
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

	MSG_IC_CHAT_REJECTLIST_INSERT sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_CHAT_REJECTLIST_INSERT));
	m_pINFUserName->GetString(sMsg.szCharacterName, SIZE_MAX_CHARACTER_NAME);	
	g_pIMSocket->SendMsg( T_IC_CHAT_REJECTLIST_INSERT, (char*)&sMsg, sizeof(sMsg) );
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		컨트롤 초기화
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityReject::InitEditBox()
{
	if(m_pINFUserName&&m_pINFUserName->IsEditMode())
	{
		m_pINFUserName->EnableEdit(FALSE, FALSE);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		컨트롤 초기화
/// \author		// 2008-04-04 by bhsohn Ep3 커뮤니티 창
/// \date		2008-04-04 ~ 2008-04-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityReject::SendDeleteRejectList()
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

	if(m_nRejectSelect < 0)
	{
		return;
	}
	if(m_nRejectSelect >= m_vecReject.size())
	{
		return;
	}
	MSG_IC_CHAT_REJECTLIST_DELETE sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_CHAT_REJECTLIST_DELETE));
	strncpy(sMsg.szCharacterName, m_vecReject[m_nRejectSelect].szRejectName, SIZE_MAX_CHARACTER_NAME);	
	g_pIMSocket->SendMsg( T_IC_CHAT_REJECTLIST_DELETE, (char*)&sMsg, sizeof(sMsg) );

	// 초기화
	m_pINFUserName->EnableEdit(FALSE, FALSE);
	
	//m_bDeleteOK = FALSE;
}

void CINFCommunityReject::UpdateBtnPos()
{
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	int nPosX, nPosY;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		// 스크롤
		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 470;
		ptScrollPos.y += 191;

		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,151);
		rcMouseWhell.left		= ptScrollPos.x - 220;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 160;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 140;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	if(m_pRegisterReject)
	{		
		nPosX = ptBkPos.x + 439;
		nPosY = ptBkPos.y + 124;		
		m_pRegisterReject->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pDelReject)
	{		
		nPosX = ptBkPos.x + 439;
		nPosY = ptBkPos.y + 342;		
		m_pDelReject->SetBtnPosition(nPosX, nPosY);	
	}	
#else
	{
		// 스크롤
		
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 471;
		ptScrollPos.y += 202;

		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,110);
		rcMouseWhell.left		= ptScrollPos.x - 220;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 160;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 140;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
	if(m_pRegisterReject)
	{		
		nPosX = ptBkPos.x + 403;
		nPosY = ptBkPos.y + 129;		
		m_pRegisterReject->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pDelReject)
	{		
		nPosX = ptBkPos.x + 403;
		nPosY = ptBkPos.y + 357;		
		m_pDelReject->SetBtnPosition(nPosX, nPosY);	
	}
#endif
	{		
		nPosX = ptBkPos.x + REJECT_WND_USERID_EDIT_X;
		nPosY = ptBkPos.y + REJECT_WND_USERID_EDIT_Y;
		m_pINFUserName->SetPos(nPosX, nPosY);
	}	
	
}