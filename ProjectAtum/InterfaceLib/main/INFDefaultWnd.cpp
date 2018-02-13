// INFDefaultWnd.cpp: implementation of the CINFDefaultWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "Interface.h"

#include "INFDefaultWnd.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupManager.h"
#include "INFGroupImage.h"

#define		WND_CAPTION_HEIGHT		20
#define		WND_CLOSE_RIGHT_X		15
#define		WND_CLOSE_RIGHT_Y       6 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFDefaultWnd::CINFDefaultWnd()
{
	m_pDefaultBkImage = NULL;
	
	m_bShowWnd = FALSE;
	m_ptBkPos.x = m_ptBkPos.y = 0;	
	m_bMove = FALSE;
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;
	
	m_nWidth = 1;
	m_nHeight  =1;

	m_pCloseBtn = NULL;

}

CINFDefaultWnd::~CINFDefaultWnd()
{
	CINFDefaultWnd::DeleteDeviceObjects();
}

HRESULT CINFDefaultWnd::InitDeviceObjects(char* i_pBkFile)
{
	if(NULL == m_pDefaultBkImage && i_pBkFile)
	{
		DataHeader* pDataHeader = g_pGameMain->FindResource(i_pBkFile);
		if(pDataHeader)
		{
			m_pDefaultBkImage = new CINFImageEx;		
			m_pDefaultBkImage->InitDeviceObjects(pDataHeader);
		}
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
			m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}
	}
#else
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
			m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		}
	}
#endif
	return S_OK ;
}

HRESULT CINFDefaultWnd::RestoreDeviceObjects()
{
	if(m_pDefaultBkImage)
	{
		m_pDefaultBkImage->RestoreDeviceObjects();

		POINT ptSize = m_pDefaultBkImage->GetImgSize();
		SetSize(ptSize.x,ptSize.y);
	}
	{		
		if(m_pCloseBtn)
		{
			m_pCloseBtn->RestoreDeviceObjects();		
		}		
	}
	return S_OK ;
}

HRESULT CINFDefaultWnd::DeleteDeviceObjects()
{
	if(m_pDefaultBkImage)
	{
		m_pDefaultBkImage->DeleteDeviceObjects();
		util::del(m_pDefaultBkImage);
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();	
		util::del(m_pCloseBtn);
	}
	return S_OK ;
}

HRESULT CINFDefaultWnd::InvalidateDeviceObjects()
{
	if(m_pDefaultBkImage)
	{
		m_pDefaultBkImage->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();			
	}
	return S_OK ;
}
void CINFDefaultWnd::Render()
{
	if(!IsShowWnd())
	{
		return;
	}
	if(m_pDefaultBkImage)
	{
		m_pDefaultBkImage->Move(m_ptBkPos.x, m_ptBkPos.y);
		m_pDefaultBkImage->Render();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->Render();
	}

}

void CINFDefaultWnd::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}

}

int CINFDefaultWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(uMsg, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(uMsg, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(uMsg, wParam, lParam);
		}
		break;	
	}

	return INF_MSGPROC_NORMAL;
}

int CINFDefaultWnd::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		if(m_bMove)
		{
			m_bMove = FALSE;
			return INF_MSGPROC_BREAK;
		}		
	}
	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{					
			// 버튼 클릭 			
			ShowWnd(FALSE);
			return  INF_MSGPROC_BREAK;
		}
	}	

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFDefaultWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}

	
	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}

	{
		if(IsMouseCaps(pt))
		{
			m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
			m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
			m_bMove = TRUE;			
			
			return INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;


}
int CINFDefaultWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	m_pCloseBtn->OnMouseMove(pt);
	
	{
		if(m_bMove)
		{
			m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
			m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
			// UI유저 지정 
			//UpdateBtnPos();
			return INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;

}

BOOL CINFDefaultWnd::IsWndRect(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;		
	
	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+m_nWidth))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+m_nHeight)))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CINFDefaultWnd::IsShowWnd()
{
	return m_bShowWnd;
}

BOOL CINFDefaultWnd::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;		
	
	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+m_nWidth))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+WND_CAPTION_HEIGHT)))
	{
		return TRUE;
	}
	return FALSE;
}

void CINFDefaultWnd::ShowWnd(BOOL bShowWnd, POINT *ptPos/*=NULL*/, int nWndWidth/*=0*/)
{
	m_bShowWnd = bShowWnd;
	if(bShowWnd && ptPos)
	{
		m_ptBkPos = (*ptPos);		
	}

}

void CINFDefaultWnd::SetSize(int i_nWidth,int i_nHeight)
{
	m_nWidth = i_nWidth;
	m_nHeight = i_nHeight;
}

POINT CINFDefaultWnd::GetBkPos()
{
	return m_ptBkPos;
}
void CINFDefaultWnd::UpdateBtnPos(int nWidth, int nHeight)
{
	POINT ptBkPos = GetBkPos();

	int nPosX, nPosY;
	nPosX = nPosY = 0;
	if(m_pCloseBtn)
	{		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		nPosX = ptBkPos.x + (nWidth - WND_CLOSE_RIGHT_X);
		nPosY = ptBkPos.y+WND_CLOSE_RIGHT_Y;
#else
		POINT ptSize = m_pCloseBtn->GetImgSize();
		nPosX = ptBkPos.x + (nWidth - ptSize.x-13);
		nPosY = ptBkPos.y+7;
#endif
		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}

}