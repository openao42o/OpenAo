// INFCommunityGuildManager.cpp: implementation of the CINFCommunityGuildManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "INFScrollBar.h"
#include "INFComboBox.h"
#include "INFGameMain.h"
#include "INFCommunity.h"
#include "GameDataLast.h"
#include "INFImageBtn.h"
#include "INFArenaScrollBar.h"
#include "AtumSound.h"
#include "INFEditBox.h"
#include "ShuttleChild.h"
#include "Chat.h"
#include "IMSocketManager.h"
#include "INFCurselEditBox.h"		// 2008-10-29 by bhsohn 에디트 박스 형태 변경

#include "INFCommunityGuildManager.h"

#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI시스템 변경
#include "INFToolTip.h"				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#include "Interface.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 커뮤니티 창 옵션
#define	COMMUNITY_MANANERBK_X				230
#define	COMMUNITY_MANANERBK_Y				0
#define	COMMUNITY_MANANERBK_WIDTH			485
#define	COMMUNITY_MANANERBK_HEIGHT			280
#define	COMMUNITY_MANANERBK_CAPS_HEIGHT	20

// 여단 소개
#define GUILD_INTRO_EDIT_X						20
#define GUILD_INTRO_EDIT_Y						50
//#define GUILD_INTRO_EDIT_W						140
#define GUILD_INTRO_EDIT_W						130
#define GUILD_INTRO_FONT_LINE_HEIGHT			15
#define GUILD_INTRO_FONT_MOUSE_HEIGHT			185
#define	MAX_GUILD_INTRO_LINE					13

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 여단 소개
#define MEMBER_INTRO_EDIT_X						203
#define MEMBER_INTRO_EDIT_Y						175
#define MEMBER_INTRO_EDIT_W						202
#define MEMBER_INTRO_FONT_LINE_HEIGHT			15
#define MEMBER_INTRO_FONT_MOUSE_HEIGHT			78
#define	MAX_MEMBER_INTRO_LINE					5


// 지원자 포지션 스크롤
#define MEMBER_LIST_EDIT_X						202
#define MEMBER_LIST_EDIT_Y						71
#define MEMBER_LIST_EDIT_W						202
#define MEMBER_LIST_FONT_LINE_HEIGHT			15
#define	MAX_MEMBER_LIST_LINE					5												  
#else 
// 여단 소개
#define MEMBER_INTRO_EDIT_X						197
#define MEMBER_INTRO_EDIT_Y						164
#define MEMBER_INTRO_EDIT_W						202
#define MEMBER_INTRO_FONT_LINE_HEIGHT			15
#define MEMBER_INTRO_FONT_MOUSE_HEIGHT			78
#define	MAX_MEMBER_INTRO_LINE					5


// 지원자 포지션 스크롤
#define MEMBER_LIST_EDIT_X						190
#define MEMBER_LIST_EDIT_Y						63
#define MEMBER_LIST_EDIT_W						202
#define MEMBER_LIST_FONT_LINE_HEIGHT			15
#define	MAX_MEMBER_LIST_LINE					5
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommunityGuildManager::CINFCommunityGuildManager(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pFormat = NULL;
	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pRegesterBtn = NULL;
	m_pCancelBtn = NULL;
	m_pAcceptBtn = NULL;			// 수락
	m_pRefuBtn = NULL;				// 거절

	m_pEditGuildIntro = NULL;
	m_pEditMemberIntro = NULL;	

	m_pScrollGuildIntro = NULL;
	m_pScrollMemberIntro = NULL;
	m_pScrollMemberList = NULL;

	m_pImgSelect = NULL;
	m_nSelMemberList = -1;
	m_vecGuildApplicantInfo.clear();	// 지원자 

	m_pFontSel = NULL;	

	m_pCloseBtn = NULL;
}

CINFCommunityGuildManager::~CINFCommunityGuildManager()
{
	util::del(m_pFormat);
		
	util::del(m_pRegesterBtn);
	util::del(m_pCancelBtn);	

	util::del(m_pAcceptBtn);			// 수락
	util::del(m_pRefuBtn );				// 거절

	util::del(m_pEditGuildIntro);	
	util::del(m_pEditMemberIntro);

	util::del(m_pScrollGuildIntro);
	util::del(m_pScrollMemberIntro);
	util::del(m_pScrollMemberList);

	util::del(m_pImgSelect);	

	util::del(m_pFontSel);

	m_vecGuildApplicantInfo.clear();	// 지원자 

	util::del(m_pCloseBtn);
	
}

HRESULT CINFCommunityGuildManager::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;
	
	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();
	
	// 커뮤니티 옵션배	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_MANANERBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_MANANERBK_HEIGHT/2;
	
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(NULL == m_pFormat)
		{
 			m_pFormat = new CINFImageEx;	
		}		
		pDataHeader = FindResource("c_mabk");
 		m_pFormat->InitDeviceObjects(pDataHeader);
 	}
#else
	if(m_pFormat == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("B_adm");	
		m_pFormat = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pFormat->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pFormat->RestoreDeviceObjects();
	}  
#endif

	
	{
		// 해제
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
		if(NULL == m_pCancelBtn)
		{
			m_pCancelBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP13");		
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	
	{
		// 등록
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "Oapp3");
		wsprintf(szDownBtn, "Oapp1");
		wsprintf(szSelBtn, "Oapp0");
		wsprintf(szDisBtn, "Oapp2");
		if(NULL == m_pRegesterBtn)
		{
			m_pRegesterBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRegesterBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pRegesterBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP56");		
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	
	
	{
		// 수락
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");															  
#else 
		wsprintf(szUpBtn, "c_acc3");
		wsprintf(szDownBtn, "c_acc1");
		wsprintf(szSelBtn, "c_acc0");
		wsprintf(szDisBtn, "c_acc2");
#endif
		if(NULL == m_pAcceptBtn)
		{
			m_pAcceptBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pAcceptBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pAcceptBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP43");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	{
		// 거절
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");															  
#else							  
		wsprintf(szUpBtn, "c_ref3");
		wsprintf(szDownBtn, "c_ref1");
		wsprintf(szSelBtn, "c_ref0");
		wsprintf(szDisBtn, "c_ref2");
#endif
		if(NULL == m_pRefuBtn)
		{
			m_pRefuBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRefuBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pRefuBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP44");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	{
		if(NULL == m_pEditGuildIntro)
		{
			// 2008-10-29 by bhsohn 에디트 박스 형태 변경
			//m_pEditGuildIntro = new CINFEditBox;
			m_pEditGuildIntro = new CINFCurselEditBox;			
		}
		POINT ptPos = {GUILD_INTRO_EDIT_X, GUILD_INTRO_EDIT_Y};		
		m_pEditGuildIntro->InitDeviceObjects(9, ptPos, GUILD_INTRO_EDIT_W, TRUE, GUILD_INTRO_FONT_LINE_HEIGHT, 
										FALSE, GUILD_INTRO_FONT_MOUSE_HEIGHT);				
		m_pEditGuildIntro->SetStringMaxBuff(SIZE_MAX_NOTICE);		
		m_pEditGuildIntro->SetOnePageItemCnt(MAX_GUILD_INTRO_LINE);	// 2008-10-29 by bhsohn 에디트 박스 형태 변경
	}
	{
		char  szScBall[30];	
		if(NULL == m_pScrollGuildIntro)
		{
			m_pScrollGuildIntro = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollGuildIntro->InitDeviceObjects(MAX_GUILD_INTRO_LINE, szScBall);
	}
	
	{
		if(NULL == m_pEditMemberIntro)
		{
			m_pEditMemberIntro = new CINFEditBox;
		}
		POINT ptPos = {MEMBER_INTRO_EDIT_X, MEMBER_INTRO_EDIT_Y};		
		m_pEditMemberIntro->InitDeviceObjects(9, ptPos, MEMBER_INTRO_EDIT_W, TRUE, MEMBER_INTRO_FONT_LINE_HEIGHT, 
										FALSE, MEMBER_INTRO_FONT_MOUSE_HEIGHT);				
		m_pEditMemberIntro->SetStringMaxBuff(SIZE_MAX_LETTER_CONTENT);		
	}	
	
	{
		char  szScBall[30];	
		if(NULL == m_pScrollMemberIntro)
		{
			m_pScrollMemberIntro = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollMemberIntro->InitDeviceObjects(MAX_MEMBER_INTRO_LINE, szScBall);
	}
	
	{
		char  szScBall[30];	
		if(NULL == m_pScrollMemberList)
		{
			m_pScrollMemberList = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");		
		m_pScrollMemberList->InitDeviceObjects(MAX_MEMBER_INTRO_LINE, szScBall);
	}
	{			
		pDataHeader = m_pGameData->Find("c_masel");	
		if(NULL == m_pImgSelect)
		{
			m_pImgSelect = new CINFImageEx;
		}
		m_pImgSelect->InitDeviceObjects( pDataHeader );
	}
	if(NULL == m_pFontSel)
	{
		m_pFontSel = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontSel->InitDeviceObjects(g_pD3dDev);
	}	

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");															  
#else	
		wsprintf(szUpBtn, "xclose3");
		wsprintf(szDownBtn, "xclose1");
		wsprintf(szSelBtn, "xclose0");
		wsprintf(szDisBtn, "xclose2");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	return S_OK ;
}

HRESULT CINFCommunityGuildManager::RestoreDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->RestoreDeviceObjects();
	}
	if(m_pRegesterBtn)
	{
		m_pRegesterBtn->RestoreDeviceObjects();						
	}
	if(m_pCancelBtn)
	{
		m_pCancelBtn->RestoreDeviceObjects();						
	}
	
	if(m_pAcceptBtn)
	{
		m_pAcceptBtn->RestoreDeviceObjects();						
	}
	if(m_pRefuBtn)
	{
		m_pRefuBtn->RestoreDeviceObjects();						
	}

	if(m_pEditGuildIntro)
	{
		m_pEditGuildIntro->RestoreDeviceObjects();		
	}
	if(m_pScrollGuildIntro)
	{
		// 스크롤
		m_pScrollGuildIntro->RestoreDeviceObjects();		
	}
	if(m_pEditMemberIntro)
	{
		m_pEditMemberIntro->RestoreDeviceObjects();		
	}	
	if(m_pScrollMemberIntro)
	{
		m_pScrollMemberIntro->RestoreDeviceObjects();		
	}
	if(m_pScrollMemberList)
	{
		m_pScrollMemberList->RestoreDeviceObjects();		
	}

	if(m_pImgSelect)
	{
		m_pImgSelect->RestoreDeviceObjects();
	}
	
	if(m_pFontSel)
	{
		m_pFontSel->RestoreDeviceObjects();
	}
	
	if(m_pCloseBtn)
	{	
		m_pCloseBtn->RestoreDeviceObjects();				
	}

	// UI유저 지정 
	UpdateUIPos();

	//TestDB();//Test용
	
	return S_OK ;
}

HRESULT CINFCommunityGuildManager::DeleteDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->DeleteDeviceObjects();
		util::del(m_pFormat );
	}
	if(m_pRegesterBtn)
	{		
		m_pRegesterBtn->DeleteDeviceObjects();
		util::del(m_pRegesterBtn);
	}
	if(m_pCancelBtn)
	{		
		m_pCancelBtn->DeleteDeviceObjects();
		util::del(m_pCancelBtn);
	}	
	if(m_pAcceptBtn)
	{		
		m_pAcceptBtn->DeleteDeviceObjects();
		util::del(m_pAcceptBtn);
	}
	if(m_pRefuBtn)
	{		
		m_pRefuBtn->DeleteDeviceObjects();
		util::del(m_pRefuBtn);
	}
	if(m_pEditGuildIntro)
	{
		m_pEditGuildIntro->DeleteDeviceObjects();
		util::del(m_pEditGuildIntro);		
	}
	if(m_pScrollGuildIntro)
	{
		m_pScrollGuildIntro->DeleteDeviceObjects();
		util::del(m_pScrollGuildIntro);
	}
	if(m_pEditMemberIntro)
	{
		m_pEditMemberIntro->DeleteDeviceObjects();
		util::del(m_pEditMemberIntro);		
	}	
	if(m_pScrollMemberIntro)
	{
		m_pScrollMemberIntro->DeleteDeviceObjects();
		util::del(m_pScrollMemberIntro);		
	}
	if(m_pScrollMemberList)
	{
		m_pScrollMemberList->DeleteDeviceObjects();
		util::del(m_pScrollMemberList);		
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->DeleteDeviceObjects();
		util::del(m_pImgSelect);
	}
	if(m_pFontSel)
	{
		m_pFontSel->DeleteDeviceObjects();	
		util::del(m_pFontSel);
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}

	return S_OK ;
}

HRESULT CINFCommunityGuildManager::InvalidateDeviceObjects()
{
	if(m_pFormat)
	{
		m_pFormat->InvalidateDeviceObjects();
	}
	if(m_pRegesterBtn)
	{		
		m_pRegesterBtn->InvalidateDeviceObjects();		
	}
	if(m_pCancelBtn)
	{		
		m_pCancelBtn->InvalidateDeviceObjects();		
	}
	if(m_pAcceptBtn)
	{		
		m_pAcceptBtn->InvalidateDeviceObjects();		
	}
	if(m_pRefuBtn)
	{		
		m_pRefuBtn->InvalidateDeviceObjects();		
	}
	if(m_pEditGuildIntro)
	{
		m_pEditGuildIntro->InvalidateDeviceObjects();
	}
	if(m_pScrollGuildIntro)
	{
		m_pScrollGuildIntro->InvalidateDeviceObjects();
	}
	if(m_pEditMemberIntro)
	{
		m_pEditMemberIntro->InvalidateDeviceObjects();
	}	
	if(m_pScrollMemberIntro)
	{
		m_pScrollMemberIntro->InvalidateDeviceObjects();
	}
	if(m_pScrollMemberList)
	{
		m_pScrollMemberList->InvalidateDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->InvalidateDeviceObjects();
	}
	if(m_pFontSel)
	{
		m_pFontSel->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();	
	}

	return S_OK ;
}

void CINFCommunityGuildManager::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	POINT ptBkPos = m_ptCommOpBk;
	if(m_pFormat)
	{		
		m_pFormat->Move(ptBkPos.x, ptBkPos.y);
		m_pFormat->Render() ;
	}
	
	m_pRegesterBtn->Render();		
	m_pCancelBtn->Render();		
	m_pAcceptBtn->Render();					// 수락
	m_pRefuBtn->Render();					// 거절

	m_pScrollGuildIntro->Render();	// 여단소개 스크롤
	m_pEditGuildIntro->Tick();
	m_pEditGuildIntro->Render(m_pScrollGuildIntro->GetScrollStep(), MAX_GUILD_INTRO_LINE);

	
	m_pScrollMemberIntro->Render();	// 자기소개 스크롤
	m_pEditMemberIntro->Tick();
	m_pEditMemberIntro->Render(m_pScrollMemberIntro->GetScrollStep(), MAX_MEMBER_INTRO_LINE);	

	// 지원자 관리
	{
		m_pScrollMemberList->Render();		
		RenderMemberList();
	}

	m_pCloseBtn->Render();	
	
}

void CINFCommunityGuildManager::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}

}

int CINFCommunityGuildManager::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWindow())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(TRUE == m_pEditGuildIntro->OnLButtonDown(pt))
			{
				// 2008-10-29 by bhsohn 에디트 박스 형태 변경
				//OnClickEditCtl(m_pEditGuildIntro);
				OnClickEditCtl(NULL);
				m_pEditGuildIntro->EnableEdit(TRUE, TRUE);
				
				m_pEditGuildIntro->SetFirstLine();	// 가장 위라인으로 이동
				int nMaxStep = m_pScrollGuildIntro->GetMaxStepCnt();
				m_pScrollGuildIntro->SetMaxItem(nMaxStep);
					// end 2008-10-29 by bhsohn 에디트 박스 형태 변경

				return INF_MSGPROC_BREAK;
			}
			
//			if(TRUE == m_pEditMemberIntro->OnLButtonDown(pt))
//			{				
//				OnClickEditCtl(m_pEditMemberIntro);
//				return INF_MSGPROC_BREAK;
//			}
			// 전체를 뺵�?받는다.
			OnClickEditCtl(NULL);

			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}

			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommOpBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommOpBk.y;
				m_bMove = TRUE;
				return INF_MSGPROC_BREAK;
			}
						

			{
				if(TRUE == m_pRegesterBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			/*{
				if(TRUE == m_pCancelBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}*/
			{
				if(TRUE == m_pAcceptBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				if(TRUE == m_pRefuBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}			
			
			{
				BOOL bClick = m_pScrollGuildIntro->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollGuildIntro->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			
			{
				BOOL bClick = m_pScrollMemberIntro->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollMemberIntro->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			
			{
				BOOL bClick = m_pScrollMemberList->IsMouseBallPos(pt);
				if(bClick)
				{
					m_pScrollMemberList->SetMouseMoveMode(TRUE);
					return INF_MSGPROC_BREAK;
				}
			}
			


			if(OnClickMemberList(pt))
			{
				return INF_MSGPROC_BREAK;
			}

			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+COMMUNITY_MANANERBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+COMMUNITY_MANANERBK_HEIGHT)))
			{
				// 윈도우 영역위에 있다.
				return  INF_MSGPROC_BREAK;
			}
			
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMove)
			{
				m_ptCommOpBk.x = pt.x - m_ptCommOpMouse.x;
				m_ptCommOpBk.y = pt.y - m_ptCommOpMouse.y;				
				// UI유저 지정 
				UpdateUIPos();
				return INF_MSGPROC_BREAK;
			}
			
			m_pRegesterBtn->OnMouseMove(pt);
			m_pCancelBtn->OnMouseMove(pt);	
			m_pAcceptBtn->OnMouseMove(pt);				// 수락
			m_pRefuBtn->OnMouseMove(pt);				// 거절				
			m_pCloseBtn->OnMouseMove(pt);

			{
				if(m_pScrollGuildIntro->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollGuildIntro->IsMouseScrollPos(pt))
					{
						m_pScrollGuildIntro->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollGuildIntro->SetScrollPos(pt);
						
						return INF_MSGPROC_BREAK;
					}
				}
			}
			
			{
				if(m_pScrollMemberIntro->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollMemberIntro->IsMouseScrollPos(pt))
					{
						m_pScrollMemberIntro->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollMemberIntro->SetScrollPos(pt);
						
						return INF_MSGPROC_BREAK;
					}
				}
			}
			
			{
				if(m_pScrollMemberList->GetMouseMoveMode())
				{
					if(FALSE == m_pScrollMemberList->IsMouseScrollPos(pt))
					{
						m_pScrollMemberList->SetMouseMoveMode(FALSE);
					}
					else
					{
						m_pScrollMemberList->SetScrollPos(pt);
						
						return INF_MSGPROC_BREAK;
					}
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
			if(m_bMove)
			{
				m_bMove = FALSE;
				return INF_MSGPROC_BREAK;
			}
						
			{
				if(TRUE == m_pRegesterBtn->OnLButtonUp(pt))
				{			
					OnClickGuildIntoduce();		// 여단 소개 등록
					// 버튼위에 마우스가 있다.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			/*{
				if(TRUE == m_pCancelBtn->OnLButtonUp(pt))
				{						
					OnClickGuildIntoduceCancel();	// 여단 소개 해제
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}*/			
			{
				if(TRUE == m_pAcceptBtn->OnLButtonUp(pt))
				{	
					OnClickAccept();
					// 버튼위에 마우스가 있다.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{					
					// 버튼 클릭 	
					ShowWindow(FALSE); // 숨기기
					return  INF_MSGPROC_BREAK;
				}
			}
			{
				if(TRUE == m_pRefuBtn->OnLButtonUp(pt))
				{			
					OnClickRef();
					// 버튼위에 마우스가 있다.					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				BOOL bClick = m_pScrollGuildIntro->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollGuildIntro->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
			
			{
				BOOL bClick = m_pScrollMemberIntro->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollMemberIntro->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
			
			{
				BOOL bClick = m_pScrollMemberList->GetMouseMoveMode();
				if(bClick)
				{
					m_pScrollMemberList->SetMouseMoveMode(FALSE);
					return INF_MSGPROC_BREAK;
				}				
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);			
			BOOL bClick = FALSE;
			
			bClick = m_pScrollGuildIntro->IsMouseWhellPos(pt);
			if(bClick)		
			{			
				m_pScrollGuildIntro->OnMouseWheel(wParam, lParam);

				return INF_MSGPROC_BREAK;
			}
			
			bClick = m_pScrollMemberIntro->IsMouseWhellPos(pt);
			if(bClick)		
			{			
				m_pScrollMemberIntro->OnMouseWheel(wParam, lParam);

				return INF_MSGPROC_BREAK;
			}
			
			{
				bClick = m_pScrollMemberList->IsMouseWhellPos(pt);
				if(bClick)		
				{			
					m_pScrollMemberList->OnMouseWheel(wParam, lParam);
					
					return INF_MSGPROC_BREAK;
				}
				
			}
		}
		break;	
	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
	case WM_KEYDOWN:
	// 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
	case WM_KEYUP:
	// end 2010. 04. 12 by ckPark 편지쓰기 관련 에디트 박스 기능 개선
		{
			BOOL bRtn = FALSE;

			if(m_pEditMemberIntro->IsEditMode())
			{
				int nMaxLine = 10;
				CINFEditBox* pEdit = NULL;
				CINFArenaScrollBar*	 pScroll = NULL;

				pEdit = m_pEditMemberIntro;
				pScroll = m_pScrollMemberIntro;
				nMaxLine = MAX_MEMBER_INTRO_LINE;
				bRtn = OnIMEEvent(uMsg, wParam, lParam, nMaxLine, pEdit, pScroll);
			}						
			// 2008-10-29 by bhsohn 에디트 박스 형태 변경
			else if(m_pEditGuildIntro->IsEditMode())
			{				
				int nLienCnt = -1;
				int nArrowScroll = 0;
				bRtn = FALSE;
				BOOL bStrCat =FALSE;
				bRtn = m_pEditGuildIntro->WndProc(uMsg, wParam, lParam, m_pScrollGuildIntro->GetScrollStep(), 
					&nLienCnt, &nArrowScroll, &bStrCat);
				
				if(nLienCnt < 0)
				{
					// 아무 처리도 안했다.
					return INF_MSGPROC_NORMAL;
				}			
				int nMaxStep = 0;			
				int nScrollStep = 0;
				switch(uMsg)
				{
				case WM_KEYDOWN:
					{
						switch(wParam)
						{
						case VK_LEFT:	// 왼쪽으로 이동
						case VK_UP:
						case VK_RIGHT:
						case VK_DOWN:
							{
								// 방향키를 눌러서 스크롤 위치 변경 
								if(nLienCnt > MAX_GUILD_INTRO_LINE 
									&& nArrowScroll !=0)
								{
									nMaxStep = m_pScrollGuildIntro->GetMaxStepCnt();
									nScrollStep = m_pScrollGuildIntro->GetScrollStep() + nArrowScroll;
									nScrollStep = max(0, nScrollStep);						
									nScrollStep = min(nMaxStep, nScrollStep);						
									m_pScrollGuildIntro->SetScrollStep(nScrollStep);
								}
								
								return INF_MSGPROC_NORMAL;
							}
							break;
						case VK_RETURN:
							{
								return INF_MSGPROC_BREAK;
							}
							break;
						}
					}
					break;
				}
				
				nMaxStep = max(nLienCnt, MAX_GUILD_INTRO_LINE);
				nScrollStep = nMaxStep - MAX_GUILD_INTRO_LINE;
				if(nScrollStep < 0)
				{
					nScrollStep = 0;
				}						
				if(m_pEditGuildIntro->IsLastPos())
				{
					// 가장 마지막 라인이냐?					
					
					// 최대 아이템을 정해주고
					m_pScrollGuildIntro->SetMaxItem(nMaxStep);					
					// 스트링을 뒤에 붙이는 방식이 아니다.
					// 스크롤 위치는 가장 아래
					m_pScrollGuildIntro->SetScrollStep(nScrollStep);								
				}					
				else
				{
					m_pScrollGuildIntro->SetOnlyMaxItem(nMaxStep);
					m_pEditGuildIntro->RefreshEndCurselPos();	// 마지막위치 갱신
				}
				
				if(bRtn)
				{
					return INF_MSGPROC_BREAK;
				}					
				
			}
			if(bRtn)
			{
				return INF_MSGPROC_BREAK;
			}			
		}
		break;
		// end 2008-04-04 by bhsohn Ep3 커뮤니티 창
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommunityGuildManager::ShowWindow(BOOL bShow)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// 컨트롤이 움직인다.
	if(bShow)
	{
		POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

		// 커뮤니티 옵션배	
		m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_MANANERBK_X;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_MANANERBK_HEIGHT/2;

		RqAPPLICANTList();	// 지원자 관리 리스트 요청
		// UI유저 지정 
		UpdateUIPos();
	}
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	else
	{	
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;	
	}
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
}

BOOL CINFCommunityGuildManager::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommunityGuildManager::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_MANANERBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_MANANERBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommunityGuildManager::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nPosX, nPosY;
	nPosX = nPosY = 0;
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		nPosX = ptBkPos.x + 60;
		nPosY = ptBkPos.y + 260;		
		m_pRegesterBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 110;
		nPosY = ptBkPos.y + 260;		
		m_pCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	{
		nPosX = ptBkPos.x + 270;
		nPosY = ptBkPos.y + 260;		
		m_pAcceptBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 320;
		nPosY = ptBkPos.y + 260;		
		m_pRefuBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCloseBtn)
	{
		nPosX = ptBkPos.x + 428;
		nPosY = ptBkPos.y + 3;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}											  
#else 
	{
		nPosX = ptBkPos.x + 30;
		nPosY = ptBkPos.y + 255;		
		m_pRegesterBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 97;
		nPosY = ptBkPos.y + 255;		
		m_pCancelBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	{
		nPosX = ptBkPos.x + 237;
		nPosY = ptBkPos.y + 255;		
		m_pAcceptBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + 304;
		nPosY = ptBkPos.y + 255;		
		m_pRefuBtn->SetBtnPosition(nPosX, nPosY);	
	}
	if(m_pCloseBtn)
	{
		nPosX = ptBkPos.x + 410;
		nPosY = ptBkPos.y + 5;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
#endif

	{
		nPosX = ptBkPos.x + GUILD_INTRO_EDIT_X;
		nPosY = ptBkPos.y + GUILD_INTRO_EDIT_Y;		
		m_pEditGuildIntro->SetPos(nPosX, nPosY);	
	}
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		ptScrollPos.x += 168;
		ptScrollPos.y += 56;

		m_pScrollGuildIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,171);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 170;
		m_pScrollGuildIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 180;
		m_pScrollGuildIntro->SetMouseBallRect(rcMousePos);
	}
	{
		nPosX = ptBkPos.x + MEMBER_INTRO_EDIT_X;
		nPosY = ptBkPos.y + MEMBER_INTRO_EDIT_Y;		
		m_pEditMemberIntro->SetPos(nPosX, nPosY);	
	}
	

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 419;
		ptScrollPos.y += 178;

		// 스크롤 height = 이미지 길이의 - 34
		m_pScrollMemberIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,49);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 60;
		m_pScrollMemberIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 70;
		m_pScrollMemberIntro->SetMouseBallRect(rcMousePos);
	}
	
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 419;
		ptScrollPos.y += 59;
		
		// 스크롤 height = 이미지 길이의 - 34
		m_pScrollMemberList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,62);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 71;
		m_pScrollMemberList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 81;
		m_pScrollMemberList->SetMouseBallRect(rcMousePos);
	}	
#else  
		ptScrollPos.x += 165;
		ptScrollPos.y += 56;

		m_pScrollGuildIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,156);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 170;
		m_pScrollGuildIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 180;
		m_pScrollGuildIntro->SetMouseBallRect(rcMousePos);
	}
	{
		nPosX = ptBkPos.x + MEMBER_INTRO_EDIT_X;
		nPosY = ptBkPos.y + MEMBER_INTRO_EDIT_Y;		
		m_pEditMemberIntro->SetPos(nPosX, nPosY);	
	}
	

	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;

		ptScrollPos.x += 406;
		ptScrollPos.y += 173;

		// 스크롤 height = 이미지 길이의 - 34
		m_pScrollMemberIntro->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,39);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 60;
		m_pScrollMemberIntro->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 70;
		m_pScrollMemberIntro->SetMouseBallRect(rcMousePos);
	}
	
	{
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptBkPos;
		
		ptScrollPos.x += 406;
		ptScrollPos.y += 54;
		
		// 스크롤 height = 이미지 길이의 - 34
		m_pScrollMemberList->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,51);
		rcMouseWhell.left		= ptScrollPos.x - 300;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 71;
		m_pScrollMemberList->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 81;
		m_pScrollMemberList->SetMouseBallRect(rcMousePos);
	}
#endif
	
}
BOOL CINFCommunityGuildManager::OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, int nMaxLine, CINFEditBox* pEdit, CINFArenaScrollBar*	 pScroll)
{
	int nLienCnt = -1;
	BOOL bRtn = TRUE;
	bRtn = pEdit->WndProc(uMsg, wParam, lParam, &nLienCnt, TRUE);
	if(nLienCnt < 0)
	{
		// 아무 처리도 안했다.
		return FALSE;
	}				
	
	int nMaxStep = max(nLienCnt, nMaxLine);			
	
	// 최대 아이템을 정해주고
	pScroll->SetMaxItem(nMaxStep);
	
	int nScrollStep = nMaxStep - nMaxLine;
	if(nScrollStep < 0)
	{
		nScrollStep = 0;
	}
	
	// 스크롤 위치는 가장 아래
	pScroll->SetScrollStep(nScrollStep);		
	return TRUE;	
}

void CINFCommunityGuildManager::OnClickEditCtl(CINFEditBox*	pSelEdit)
{
	if(m_pEditGuildIntro->IsEditMode())
	{
		m_pEditGuildIntro->BackupTxtString();
	}
	if(m_pEditMemberIntro->IsEditMode())
	{
		m_pEditMemberIntro->BackupTxtString();
	}

	m_pEditGuildIntro->EnableEdit(FALSE, FALSE);		// 여단 소개
	m_pEditMemberIntro->EnableEdit(FALSE, FALSE);		// 자기소개
	if(pSelEdit)
	{
		pSelEdit->EnableEdit(TRUE, TRUE);
	}
}
BOOL CINFCommunityGuildManager::OnClickMemberList(POINT pt)
{
	POINT ptBkPos = m_ptCommOpBk;
	int nStartX,nStartY;
	nStartX = ptBkPos.x + MEMBER_LIST_EDIT_X;
	nStartY = 0;
	if(!((nStartX <= pt.x)
		&&(pt.x <= nStartX + MEMBER_LIST_EDIT_W)))
	{
		m_nSelMemberList = -1;
		return FALSE; 
	}
	int nItemCnt = 0;
	int nSelMemberList = -1;
	for(nItemCnt = 0;nItemCnt < MAX_MEMBER_LIST_LINE; nItemCnt++)
	{
		nStartY = ptBkPos.y + MEMBER_LIST_EDIT_Y +(nItemCnt*MEMBER_LIST_FONT_LINE_HEIGHT);
		if((nStartY <= pt.y)&&(pt.y <= nStartY + MEMBER_LIST_FONT_LINE_HEIGHT))
		{
			nSelMemberList= nItemCnt;
			break;			
		}
	}	
	
	if(nSelMemberList >= 0)
	{
		int nItenLen = (int)m_vecGuildApplicantInfo.size();
		int nSelItem = nSelMemberList+m_pScrollMemberList->GetScrollStep();
		if(nSelItem < nItenLen)
		{
			m_nSelMemberList = nSelItem;
			UID32_t	CharacterUID = m_vecGuildApplicantInfo[nSelItem].CharacterUID;
			RqMemberInfo(CharacterUID);	// 유저 정보를 요청한다.
			return TRUE; 
		}
	}	
	m_nSelMemberList = -1;
	return FALSE;
}

void CINFCommunityGuildManager::AddMemberList(UID32_t CharacterUID, char *pUser, USHORT UnitKind, int nLevel)
{
	structGuildApplicantInfo struInfo;
	memset(&struInfo, 0x00 ,sizeof(structGuildApplicantInfo));

	struInfo.CharacterUID = CharacterUID;			// 캐릭 UID
	strncpy(struInfo.chUser, pUser, SIZE_MAX_ARENA_FULL_NAME);	// 보낸 유저
	
	struInfo.UnitKind = UnitKind;
	((CINFCommunity*)m_pParent)->GetUnitKindString(UnitKind, struInfo.chUnitKind);	// 유닛 종류
	wsprintf(struInfo.chLevel, "%d", nLevel);		// 레벨정보

	m_vecGuildApplicantInfo.push_back(struInfo);

	if(m_pScrollMemberList)
	{
		m_pScrollMemberList->SetMaxItem((int)m_vecGuildApplicantInfo.size());
	}
}
void CINFCommunityGuildManager::DelMemberList(UID32_t CharacterUID)
{
	vector<structGuildApplicantInfo>::iterator it = m_vecGuildApplicantInfo.begin();	// 지원자 정보	
	while(it != m_vecGuildApplicantInfo.end())
	{
		structGuildApplicantInfo stTmp = (*it);
		if(stTmp.CharacterUID == CharacterUID)
		{
			it = m_vecGuildApplicantInfo.erase(it);
			continue;
		}
		it++;
	}
	if(m_pScrollMemberList)
	{
		m_pScrollMemberList->SetMaxItem((int)m_vecGuildApplicantInfo.size());
	}
	m_nSelMemberList = -1;
	if(m_pEditMemberIntro)
	{
		char chBlank[16];
		memset(chBlank, 0x00, 16);
		// 2008-10-29 by bhsohn 에디트 박스 형태 변경
		//int nMaxLineNum = m_pEditMemberIntro->SetString(chBlank, 16);
		int nMaxLineNum = m_pEditMemberIntro->SetString(chBlank, 16, TRUE);
		m_pEditMemberIntro->BackupTxtString();
		m_pScrollMemberIntro->SetMaxItem(nMaxLineNum);
	}
}


void CINFCommunityGuildManager::RenderMemberList()
{
	POINT ptBkPos = m_ptCommOpBk;
	int nStartX,nStartY;
	int nSelRenderItem = m_nSelMemberList- m_pScrollMemberList->GetScrollStep();
	nStartX = nStartY = 0;

	// 선택한 아이템
	{
		if(nSelRenderItem >= 0 && nSelRenderItem < MAX_MEMBER_LIST_LINE )
		{
			nStartX = ptBkPos.x + MEMBER_LIST_EDIT_X;
			nStartY = ptBkPos.y + MEMBER_LIST_EDIT_Y +(nSelRenderItem*MEMBER_LIST_FONT_LINE_HEIGHT);
			m_pImgSelect->Move(nStartX, nStartY);
			m_pImgSelect->Render();
		}
	}

	// 실질적인 정보
	
	vector<structGuildApplicantInfo>::iterator it = m_vecGuildApplicantInfo.begin();	// 지원자 정보
	int nCnt = 0;
	for(nCnt = 0;nCnt < m_pScrollMemberList->GetScrollStep();nCnt++)
	{
		if(it != m_vecGuildApplicantInfo.end())
		{
			it++;
		}
	}
	
	char chBuff[SIZE_MAX_ARENA_FULL_NAME];
	memset(chBuff, 0x00, SIZE_MAX_ARENA_FULL_NAME);
	
	for(nCnt = 0; nCnt < MAX_MEMBER_LIST_LINE;nCnt++)
	{
		if(it == m_vecGuildApplicantInfo.end())
		{
			break;
		}
		structGuildApplicantInfo stTmp = (*it);

		nStartX = ptBkPos.x + MEMBER_LIST_EDIT_X;
		nStartY = ptBkPos.y + MEMBER_LIST_EDIT_Y +(nCnt*MEMBER_LIST_FONT_LINE_HEIGHT);


		// 아이디
		nStartX += 4;
		strncpy(chBuff, stTmp.chUser, SIZE_MAX_ARENA_FULL_NAME);
		g_pGameMain->TextReduce(m_pFontSel, 90, chBuff);
		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, chBuff);

		// 기어
		nStartX += 115;
		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, stTmp.chUnitKind);

		// 레벨
		nStartX += 62;
		m_pFontSel->DrawText(nStartX, nStartY, GUI_FONT_COLOR, stTmp.chLevel);

		it++;
	}
}

void CINFCommunityGuildManager::TestDB()
{
	int nCnt =0;
	char chUser[SIZE_MAX_ARENA_FULL_NAME];					// 보낸 유저	
	memset(chUser, 0x00, SIZE_MAX_ARENA_FULL_NAME);	
	for(nCnt =0;nCnt < 20;nCnt++)
	{
		wsprintf(chUser, "User%d", nCnt);
		//AddMemberList(chUser, UNITKIND_BT01, 80+nCnt);
	}
	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 지원자 관리 리스트 요청
/// \author		// 2008-06-03 by bhsohn EP3 여단 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuildManager::RqAPPLICANTList()
{	
	// 지원자 정보 초기화
	{
		m_vecGuildApplicantInfo.clear();
		if(m_pScrollMemberList)
		{
			m_pScrollMemberList->SetMaxItem((int)m_vecGuildApplicantInfo.size());
		}
	}	

	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_GET_APPLICANT);		
		vecUnLockMsg.push_back(T_IC_GUILD_GET_APPLICANT_OK_DONE);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_GET_APPLICANT, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	g_pIMSocket->SendMsg(T_IC_GUILD_GET_APPLICANT, NULL, NULL);	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 여단 소개를 언어온다.
/// \author		// 2008-06-03 by bhsohn EP3 여단 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuildManager::RqGuildGetIntodution()
{	
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_GET_INTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_GET_INTRODUCTION_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_GET_INTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	g_pIMSocket->SendMsg(T_IC_GUILD_GET_INTRODUCTION, NULL, NULL);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 여단 소개 등록
/// \author		// 2008-06-03 by bhsohn EP3 여단 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuildManager::OnClickGuildIntoduce()
{
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_UPDATE_INTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_UPDATE_INTRODUCTION_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_UPDATE_INTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	MSG_IC_GUILD_UPDATE_INTRODUCTION sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_GUILD_UPDATE_INTRODUCTION));

	m_pEditGuildIntro->GetString(sMsg.GuildIntroduction, SIZE_MAX_NOTICE);
	g_pIMSocket->SendMsg(T_IC_GUILD_UPDATE_INTRODUCTION, (char*)&sMsg, sizeof(sMsg));

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 여단 소개 등록
/// \author		// 2008-06-03 by bhsohn EP3 여단 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuildManager::OnClickGuildIntoduceCancel()
{
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_GUILD_DELETE_INTRODUCTION);		
		vecUnLockMsg.push_back(T_IC_GUILD_DELETE_INTRODUCTION_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_GUILD_DELETE_INTRODUCTION, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}	
	g_pIMSocket->SendMsg(T_IC_GUILD_DELETE_INTRODUCTION, NULL, NULL);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 여단 소개 등록 갱신
/// \author		// 2008-06-03 by bhsohn EP3 여단 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuildManager::UpdateGuildIntoduce(char* pNotice)
{
	if(m_pEditGuildIntro)
	{
		// 2008-10-29 by bhsohn 에디트 박스 형태 변경
		//int nMaxLineNum = m_pEditGuildIntro->SetString(pNotice, SIZE_MAX_NOTICE);
		int nMaxLineNum = m_pEditGuildIntro->SetString(pNotice, SIZE_MAX_NOTICE, 0, TRUE);

		m_pEditGuildIntro->BackupTxtString();
		m_pScrollGuildIntro->SetMaxItem(nMaxLineNum);		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 유저 정보를 요청한다.
/// \author		// 2008-06-03 by bhsohn EP3 여단 관련 처리
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFCommunityGuildManager::RqMemberInfo(UID32_t	CharacterUID)
{
	((CINFCommunity*)m_pParent)->RqMemberInfo(COMMUNITY_MEMINFO_MANAGER, CharacterUID);
	
}

void CINFCommunityGuildManager::IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg)
{
	if(m_pEditMemberIntro)
	{
		// 2008-10-29 by bhsohn 에디트 박스 형태 변경
		//int nMaxLineNum = m_pEditMemberIntro->SetString(pMsg->SelfIntroduction, SIZE_MAX_NOTICE);
		int nMaxLineNum = m_pEditMemberIntro->SetString(pMsg->SelfIntroduction, SIZE_MAX_NOTICE, TRUE);

		m_pEditMemberIntro->BackupTxtString();
		m_pScrollMemberIntro->SetMaxItem(nMaxLineNum);

	}

}
void CINFCommunityGuildManager::OnClickAccept()
{
	RqAccept();
}
void CINFCommunityGuildManager::RqAccept()
{
	int nItenLen = (int)m_vecGuildApplicantInfo.size();
	int nSelItem = m_nSelMemberList;
	if(nSelItem <0 || nSelItem >= nItenLen)
	{
		return;
	}
	structGuildApplicantInfo stTmp = m_vecGuildApplicantInfo[nSelItem];			

	MSG_IC_GUILD_APPLICANT_INVITE sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_GUILD_APPLICANT_INVITE));

	sMsg.CharacterUID = stTmp.CharacterUID;
	strncpy(sMsg.CharacterName, stTmp.chUser, SIZE_MAX_CHARACTER_NAME);
	sMsg.UnitKind = stTmp.UnitKind;
	sMsg.Level = atoi(stTmp.chLevel);

	g_pIMSocket->SendMsg(T_IC_GUILD_APPLICANT_INVITE, (char*)&sMsg, sizeof(sMsg));

	DelMemberList(sMsg.CharacterUID);
}

void CINFCommunityGuildManager::OnClickRef()
{
	RqRef();
}

void CINFCommunityGuildManager::RqRef()
{
	int nItenLen = (int)m_vecGuildApplicantInfo.size();
	int nSelItem = m_nSelMemberList;
	if(nSelItem <0 || nSelItem >= nItenLen)
	{
		return;
	}
	structGuildApplicantInfo stTmp = m_vecGuildApplicantInfo[nSelItem];			
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();

	MSG_IC_GUILD_APPLICANT_REJECT_INVITE sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_IC_GUILD_APPLICANT_REJECT_INVITE));	
	sMsg.CharacterUID = stTmp.CharacterUID;
	strncpy(sMsg.GuildName, myShuttleInfo.GuildName, SIZE_MAX_GUILD_NAME);
	g_pIMSocket->SendMsg(T_IC_GUILD_APPLICANT_REJECT_INVITE, (char*)&sMsg, sizeof(sMsg));

	DelMemberList(sMsg.CharacterUID);
}