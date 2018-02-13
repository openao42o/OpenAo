// INFCommuPartyInvite.cpp: implementation of the CINFCommuPartyInvite class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "Interface.h"
#include "INFCommunity.h"
#include "INFEditBox.h"
#include "IMSocketManager.h"
#include "INFImageRadioBtn.h"
#include "Chat.h"

#include "INFCommuPartyInvite.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"    // 2011. 10. 10 by jskim UI시스템 변경
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 커뮤니티 창 옵션
#define	COMMUNITY_OPINVITEBK_X				230
#define	COMMUNITY_OPINVITEBK_Y				0
#define	COMMUNITY_OPINVITEBK_WIDTH			290
#define	COMMUNITY_OPINVITEBK_HEIGHT			278
#define	COMMUNITY_OPINVITEBK_CAPS_HEIGHT	20

// 초대
#define	COMMUNITY_OPINVITE_INVITE1_X		189
#define	COMMUNITY_OPINVITE_INVITE1_Y		67

// 새로고침
#define	COMMUNITY_OPINVITE_REFRESH_X		78
#define	COMMUNITY_OPINVITE_REFRESH_Y		227

// 초대
#define	COMMUNITY_OPINVITE_INVITE2_X		146
#define	COMMUNITY_OPINVITE_INVITE2_Y		227

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 닫기
#define	COMMUNITY_OPINVITE_START_X			86
#define	COMMUNITY_OPINVITE_START_Y			69

// 편지쓰기 유저
#define LETTER_USER_EDIT_X						53
#define LETTER_USER_EDIT_Y						72
#define LETTER_USER_EDIT_W						120
#define LETTER_USER_READ_FONT_LINE_HEIGHT		15												  
#else	   
#define	COMMUNITY_OPINVITE_CLOSE_X			213
#define	COMMUNITY_OPINVITE_CLOSE_Y			253

// 편지쓰기 유저
#define LETTER_USER_EDIT_X						53
#define LETTER_USER_EDIT_Y						68
#define LETTER_USER_EDIT_W						165
#define LETTER_USER_READ_FONT_LINE_HEIGHT		15
#endif

#define MAX_ONEPATE_USER						5

#define RECOMMEND_USER_EDIT_X						63
#define RECOMMEND_USER_EDIT_Y						129
#define RECOMMEND_USER_CAP_Y						18

// 참여 방식 라디오
#define	PARTYINVITE_JOIN_X				41
#define	PARTYINVITE_JOIN_Y				127
#define	PARTYINVITE_JOIN_CAP_Y			18

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCommuPartyInvite::CINFCommuPartyInvite(CAtumNode* pParent)
{
	m_pParent = pParent;
	
	m_bShow = FALSE;
	m_bMove = FALSE;

	m_pFormat = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pFormatControl = NULL;																	  
#endif

	m_ptCommOpBk.x = m_ptCommOpBk.y = 0;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_pCloseBtn = NULL;
	m_pInviteBtn = NULL;
	m_pSelInviteBtn = NULL;
	m_pRefreshBtn = NULL;

	m_pEditUser = NULL;		// 받는 유저
	m_vecPartyInviteInfo.clear();
	m_pFontText = NULL;
	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
	{
		m_pPartyJoint[nCnt] = NULL;
	}
}

CINFCommuPartyInvite::~CINFCommuPartyInvite()
{
	m_vecPartyInviteInfo.clear();

	util::del(m_pFormat);
		
	util::del(m_pCloseBtn);
	util::del(m_pInviteBtn);	
	util::del(m_pSelInviteBtn);
	util::del(m_pRefreshBtn);		

	util::del(m_pEditUser);		// 받는 유저
	util::del(m_pFontText);

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
	{		
		util::del(m_pPartyJoint[nCnt]);		
	}
	
}

HRESULT CINFCommuPartyInvite::InitDeviceObjects()
{
	DataHeader	* pDataHeader= NULL;
	int nCnt = 0;

	POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

	// 커뮤니티 옵션배	
	m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPINVITEBK_X;
	m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPINVITEBK_HEIGHT/2;


#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		if(NULL == m_pFormat)
		{
 			m_pFormat = new CINFImageEx;
		}		
		pDataHeader = FindResource("p_bk1");
 		m_pFormat->InitDeviceObjects(pDataHeader);
 	}							
#else
	if(m_pFormat == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("F_inv");	
		m_pFormat = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pFormat->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pFormat->RestoreDeviceObjects();
	}
	if(m_pFormatControl == NULL)
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("F_invbtn1");	
		m_pFormatControl = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	}
#endif		
	{
		// 초대
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "invs03");
		wsprintf(szDownBtn, "invs01");
		wsprintf(szSelBtn, "invs00");
		wsprintf(szDisBtn, "invs02");															  
#else  
		wsprintf(szUpBtn, "c_sbt03");
		wsprintf(szDownBtn, "c_sbt01");
		wsprintf(szSelBtn, "c_sbt00");
		wsprintf(szDisBtn, "c_sbt02");
#endif
		if(NULL == m_pInviteBtn)
		{
			m_pInviteBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pInviteBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pInviteBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP57");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	
	{
		// 초대
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "invs03");
		wsprintf(szDownBtn, "invs01");
		wsprintf(szSelBtn, "invs00");
		wsprintf(szDisBtn, "invs02");															  
#else		   
		wsprintf(szUpBtn, "c_sbt03");
		wsprintf(szDownBtn, "c_sbt01");
		wsprintf(szSelBtn, "c_sbt00");
		wsprintf(szDisBtn, "c_sbt02");
#endif
		if(NULL == m_pSelInviteBtn)
		{
			m_pSelInviteBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pSelInviteBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pSelInviteBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP57");		
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
	
	{
		// 새로고침
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "refs03");
		wsprintf(szDownBtn, "refs01");
		wsprintf(szSelBtn, "refs00");
		wsprintf(szDisBtn, "refs02");															  
#else					   
		wsprintf(szUpBtn, "c_pr3");
		wsprintf(szDownBtn, "c_pr1");
		wsprintf(szSelBtn, "c_pr0");
		wsprintf(szDisBtn, "c_pr2");		
#endif
		if(NULL == m_pRefreshBtn)
		{
			m_pRefreshBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pRefreshBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pRefreshBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP46");		
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	{
		// 닫기
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "outs03");
		wsprintf(szDownBtn, "outs01");
		wsprintf(szSelBtn, "outs00");
		wsprintf(szDisBtn, "outs02");															  
#else	
		wsprintf(szUpBtn, "shnpc063");
		wsprintf(szDownBtn, "shnpc061");
		wsprintf(szSelBtn, "shnpc060");
		wsprintf(szDisBtn, "shnpc062");

#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP90");
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}

	{
		if(NULL == m_pEditUser)
		{
			m_pEditUser = new CINFEditBox;
		}
		POINT ptPos = {LETTER_USER_EDIT_X, LETTER_USER_EDIT_Y};
		m_pEditUser->InitDeviceObjects(9, ptPos, LETTER_USER_EDIT_W, TRUE, LETTER_USER_READ_FONT_LINE_HEIGHT);				
		m_pEditUser->SetStringMaxBuff(SIZE_MAX_CHARACTER_NAME);
	}
	{
		m_pFontText = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontText->InitDeviceObjects(g_pD3dDev);
	}
	{
		char chRadioOff[30], chRadioOn[30];
		wsprintf(chRadioOff, "radio_17b");
		wsprintf(chRadioOn, "radio_17a");
		
		for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
		{		
			if(NULL == m_pPartyJoint[nCnt])
			{		
				m_pPartyJoint[nCnt] = new CINFImageRadioBtn;
			}				
			m_pPartyJoint[nCnt]->InitDeviceObjects(chRadioOff, chRadioOn);
		}		
	}
	
	
	return S_OK ;
}

HRESULT CINFCommuPartyInvite::RestoreDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->RestoreDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();						
	}
	if(m_pInviteBtn)
	{
		m_pInviteBtn->RestoreDeviceObjects();						
	}
	if(m_pSelInviteBtn)
	{
		m_pSelInviteBtn->RestoreDeviceObjects();
	}
	if(m_pRefreshBtn)
	{
		m_pRefreshBtn->RestoreDeviceObjects();						
	}

	if(m_pEditUser)
	{
		m_pEditUser->RestoreDeviceObjects();		
	}
	if(m_pFontText)
	{
		m_pFontText->RestoreDeviceObjects();
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
	{	
		if(m_pPartyJoint[nCnt])
		{
			m_pPartyJoint[nCnt]->RestoreDeviceObjects();			
		}
	}

	// UI유저 지정 
	UpdateUIPos();

	
	return S_OK ;
}

HRESULT CINFCommuPartyInvite::DeleteDeviceObjects()
{
	if(m_pFormat )
	{
		m_pFormat->DeleteDeviceObjects();
		util::del(m_pFormat );
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	if(m_pInviteBtn)
	{		
		m_pInviteBtn->DeleteDeviceObjects();
		util::del(m_pInviteBtn);
	}
	if(m_pSelInviteBtn)
	{
		m_pSelInviteBtn->DeleteDeviceObjects();
		util::del(m_pSelInviteBtn);
	}
	if(m_pRefreshBtn)
	{		
		m_pRefreshBtn->DeleteDeviceObjects();
		util::del(m_pRefreshBtn);
	}
	if(m_pEditUser)
	{
		m_pEditUser->DeleteDeviceObjects();
		util::del(m_pEditUser);		
	}
	if(m_pFontText)
	{
		m_pFontText->DeleteDeviceObjects();
		util::del(m_pFontText);
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
	{		
		if(m_pPartyJoint[nCnt])
		{
			m_pPartyJoint[nCnt]->DeleteDeviceObjects();	
			util::del(m_pPartyJoint[nCnt]);
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pFormatControl->DeleteDeviceObjects();
	util::del( m_pFormatControl );															  
#endif
	return S_OK ;
}

HRESULT CINFCommuPartyInvite::InvalidateDeviceObjects()
{
	if(m_pFormat)
	{
		m_pFormat->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{		
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
	if(m_pInviteBtn)
	{		
		m_pInviteBtn->InvalidateDeviceObjects();		
	}
	if(m_pSelInviteBtn)
	{		
		m_pSelInviteBtn->InvalidateDeviceObjects();		
	}
	if(m_pRefreshBtn)
	{
		m_pRefreshBtn->InvalidateDeviceObjects();		
	}
	if(m_pEditUser)
	{
		m_pEditUser->InvalidateDeviceObjects();
	}
	if(m_pFontText)
	{
		m_pFontText->InvalidateDeviceObjects();
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
	{		
		if(m_pPartyJoint[nCnt])
		{
			m_pPartyJoint[nCnt]->InvalidateDeviceObjects();
		}
	}


	return S_OK ;
}

void CINFCommuPartyInvite::Render()
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
	
	m_pCloseBtn->Render();		
	m_pInviteBtn->Render();		
	m_pRefreshBtn->Render();		
	m_pSelInviteBtn->Render();		

	m_pEditUser->Tick();
	m_pEditUser->Render();

	int nCnt =0;
	int nInviteUser = (int)m_vecPartyInviteInfo.size();
	for(nCnt =0; nCnt < MAX_INVITE_USER; nCnt++)
	{	
		if(nCnt >= nInviteUser)
		{
			break;
		}		
		m_pPartyJoint[nCnt]->Render();
	}	

	RenderUserMem();
}

void CINFCommuPartyInvite::Tick()
{
	if(!IsShowWindow())
	{
		return;
	}

}

int CINFCommuPartyInvite::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			if(IsMouseCaps(pt))
			{
				m_ptCommOpMouse.x = pt.x - m_ptCommOpBk.x;
				m_ptCommOpMouse.y = pt.y - m_ptCommOpBk.y;
				m_bMove = TRUE;
				return INF_MSGPROC_BREAK;
			}
						
			{
				if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
				{					
					return  INF_MSGPROC_BREAK;
				}		
			}
			{
				if(TRUE == m_pInviteBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}		
			{
				if(TRUE == m_pRefreshBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			
			{
				if(TRUE == m_pSelInviteBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
			}
			int nCnt = 0;
			for(nCnt = 0;nCnt <MAX_INVITE_USER;nCnt++)
			{					
				if(TRUE == m_pPartyJoint[nCnt]->OnLButtonDown(pt))
				{						
					OnClickJoint(nCnt);
					return  INF_MSGPROC_BREAK;
				}		
			}

			if((pt.x >= m_ptCommOpBk.x && (pt.x <= m_ptCommOpBk.x+COMMUNITY_OPINVITEBK_WIDTH))
				&& (pt.y >= m_ptCommOpBk.y && (pt.y <= m_ptCommOpBk.y+COMMUNITY_OPINVITEBK_HEIGHT)))
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
			
			m_pCloseBtn->OnMouseMove(pt);
			m_pInviteBtn->OnMouseMove(pt);	
			m_pRefreshBtn->OnMouseMove(pt);		
			m_pSelInviteBtn->OnMouseMove(pt);		

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
				if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
				{			
					// 버튼위에 마우스가 있다.
					ShowWindow(FALSE);					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			{
				if(TRUE == m_pInviteBtn->OnLButtonUp(pt))
				{			
					char chUserName[SIZE_MAX_CHARACTER_NAME];
					m_pEditUser->GetString(chUserName, SIZE_MAX_CHARACTER_NAME);
					// 2008-07-14 by bhsohn EP3 관련 처리
					if(g_pGameMain->IsRejectUser(chUserName))
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207,COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
						g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
						return INF_MSGPROC_BREAK;
					}

					((CINFCommunity*)m_pParent)->PartySendInviteUser(chUserName);	// 유저 초대
					
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}			
			{
				if(TRUE == m_pRefreshBtn->OnLButtonUp(pt))
				{				
					RqInvitePartyInfo();	// 다시 요청
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
				}
			}
			
			{
				if(TRUE == m_pSelInviteBtn->OnLButtonUp(pt))
				{		
					OnClickSelInvite();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);											
					return  INF_MSGPROC_BREAK;				
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
		{
			if(m_pEditUser->WndProc(uMsg, wParam, lParam, NULL, TRUE))
			{
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFCommuPartyInvite::ShowWindow(BOOL bShow)
{
	m_bShow = bShow;

	m_bMove = FALSE;		// 컨트롤이 움직인다.
	if(bShow)
	{
		POINT ptBkPos = ((CINFCommunity*)m_pParent)->GetCommunityBkPos();

		// 커뮤니티 옵션배	
		m_ptCommOpBk.x = ptBkPos.x + COMMUNITY_OPINVITEBK_X;
		m_ptCommOpBk.y = (g_pD3dApp->GetBackBufferDesc().Height/2) - COMMUNITY_OPINVITEBK_HEIGHT/2;

		m_pEditUser->EnableEdit(TRUE, TRUE);

		// UI유저 지정 
		UpdateUIPos();
		
		RqInvitePartyInfo();
	}
	else
	{
		m_pEditUser->EnableEdit(FALSE, FALSE);
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
}

BOOL CINFCommuPartyInvite::IsShowWindow()
{
	return m_bShow;
}

BOOL CINFCommuPartyInvite::IsMouseCaps(POINT ptPos)
{
	if((ptPos.x >= m_ptCommOpBk.x && (ptPos.x <= m_ptCommOpBk.x+COMMUNITY_OPINVITEBK_WIDTH))
		&& (ptPos.y >= m_ptCommOpBk.y && (ptPos.y <= m_ptCommOpBk.y+COMMUNITY_OPINVITEBK_CAPS_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;

}

void CINFCommuPartyInvite::UpdateUIPos()
{
	POINT ptBkPos = m_ptCommOpBk;

	int nCnt = 0;
	int nPosX, nPosY;
	nPosX = nPosY = 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	{
		POINT sPos = m_pFormatControl->GetFindControlTargetofMinPos("cans01");
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_START_X + sPos.x;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_START_Y + sPos.y;
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		POINT sPos = m_pFormatControl->GetFindControlTargetofMinPos("invs00");
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_START_X + sPos.x;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_START_Y + sPos.y;	
		m_pInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	{
		POINT sPos = m_pFormatControl->GetFindControlTargetofMinPos("refs00");
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_START_X + sPos.x;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_START_Y + sPos.y;		
		m_pRefreshBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{
		POINT sPos = m_pFormatControl->GetFindControlTargetofMinPos("invs00_1");
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_START_X + sPos.x;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_START_Y + sPos.y;		
		m_pSelInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}	
#else		   
	{
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_CLOSE_X;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_CLOSE_Y;		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);	
	}
	{
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_INVITE1_X;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_INVITE1_Y;		
		m_pInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}	
	{
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_REFRESH_X;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_REFRESH_Y;		
		m_pRefreshBtn->SetBtnPosition(nPosX, nPosY);	
	}
	
	{
		nPosX = ptBkPos.x + COMMUNITY_OPINVITE_INVITE2_X;
		nPosY = ptBkPos.y + COMMUNITY_OPINVITE_INVITE2_Y;		
		m_pSelInviteBtn->SetBtnPosition(nPosX, nPosY);	
	}
#endif

	{
		nPosX = ptBkPos.x + LETTER_USER_EDIT_X;
		nPosY = ptBkPos.y + LETTER_USER_EDIT_Y;		
		m_pEditUser->SetPos(nPosX, nPosY);
	}	

	{		
		// 참여 방식 라디오		
		for(nCnt = 0;nCnt < MAX_INVITE_USER;nCnt++)
		{
			nPosX = ptBkPos.x + PARTYINVITE_JOIN_X;
			nPosY = ptBkPos.y + PARTYINVITE_JOIN_Y+ (nCnt*PARTYINVITE_JOIN_CAP_Y);		
			m_pPartyJoint[nCnt]->SetPosition(nPosX, nPosY, 60);		
		}		
	}
}

void CINFCommuPartyInvite::OnClickInviteBtn()
{	
}

void CINFCommuPartyInvite::RqInvitePartyInfo()
{
	m_vecPartyInviteInfo.clear();
	int nCnt =0;
	for(nCnt =0; nCnt < MAX_INVITE_USER; nCnt++)
	{			
		m_pPartyJoint[nCnt]->SetRadioBtn(FALSE);		
	}	
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_IC_PARTY_RECOMMENDATION_MEMBER);		
		vecUnLockMsg.push_back(T_IC_PARTY_RECOMMENDATION_MEMBER_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(FALSE, T_IC_PARTY_RECOMMENDATION_MEMBER, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}

	g_pIMSocket->SendMsg( T_IC_PARTY_RECOMMENDATION_MEMBER, NULL, NULL);		
	
}

void CINFCommuPartyInvite::AddInvitePartyInfo(SRECOMMENDATION_MEMBER_INFO* pMember)
{
	SRECOMMENDATION_MEMBER_INFO stTmp;	
	memcpy(&stTmp, pMember, sizeof(SRECOMMENDATION_MEMBER_INFO));

	m_vecPartyInviteInfo.push_back(stTmp);

}
void CINFCommuPartyInvite::RenderUserMem()
{
	POINT ptBkPos = m_ptCommOpBk;

	vector<SRECOMMENDATION_MEMBER_INFO>::iterator itUser = m_vecPartyInviteInfo.begin();
	int nCnt = 0;
	for(nCnt =0; nCnt < MAX_ONEPATE_USER;nCnt++)
	{
		if(itUser == m_vecPartyInviteInfo.end())
		{
			break;
		}
		///////////////////////////////////////////////////////////////////////////
		/// start 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 추천케릭터에 출려되는 캐릭명중 아레나 표시 스트링 삭제 "\61"
		char szCharName[SIZE_MAX_ARENA_FULL_NAME];
		util::strncpy(szCharName, itUser->CharacterName, SIZE_MAX_ARENA_FULL_NAME);	
		g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), szCharName);
		/// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 추천케릭터에 출려되는 캐릭명중 아레나 표시 스트링 삭제 "\61"
		///////////////////////////////////////////////////////////////////////////
//		SRECOMMENDATION_MEMBER_INFO stTmp = (*itUser);
		m_pFontText->DrawText(ptBkPos.x+RECOMMEND_USER_EDIT_X, 
					ptBkPos.y+RECOMMEND_USER_EDIT_Y+ (RECOMMEND_USER_CAP_Y*nCnt), 
					GUI_FONT_COLOR, szCharName);
//					stTmp.CharacterName);

		itUser++;
	}
}

void CINFCommuPartyInvite::OnClickJoint(int nIdx)
{
	int nInviteUser = (int)m_vecPartyInviteInfo.size();
	if( nIdx < 0
		&& nIdx >= nInviteUser)
	{
		return;
	}	
	BOOL bCheck = m_pPartyJoint[nIdx]->GetRadioBtn();
	bCheck ^= TRUE;
	m_pPartyJoint[nIdx]->SetRadioBtn(bCheck);
}
void CINFCommuPartyInvite::OnClickSelInvite()
{
	int nCnt =0;
	int nInviteUser = (int)m_vecPartyInviteInfo.size();
	for(nCnt =0; nCnt < MAX_INVITE_USER; nCnt++)
	{	
		if(nCnt >= nInviteUser)
		{
			break;
		}		
		if(m_pPartyJoint[nCnt]->GetRadioBtn())
		{
			SRECOMMENDATION_MEMBER_INFO stInfo = m_vecPartyInviteInfo[nCnt];
			((CINFCommunity*)m_pParent)->PartySendInviteUser(stInfo.CharacterName);	// 유저 초대
		}
	}
}
