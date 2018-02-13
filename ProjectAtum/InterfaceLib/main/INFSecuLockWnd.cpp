// INFSecuLockWnd.cpp: implementation of the CINFSecuLockWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "AtumSound.h"

#include "INFSecuMain.h"

#include "INFSecuLockWnd.h"
#include "INFGroupManager.h"
#include "INFGroupImage.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define	SECU_WND_TITLEBAR_HEIGHT		20

// Lock 버튼 
#define		SECURITY_WND_LOCK_POS_X		13
#define		SECURITY_WND_LOCK_POS_Y		35

// Lock 버튼 
#define		SECURITY_WND_SETUPPASS_POS_X		113
#define		SECURITY_WND_SETUPPASS_POS_Y		35

#define		SECURITY_WND_SETUPPASS_CAP_Y		100
#else
#define	SECU_WND_TITLEBAR_HEIGHT		20

// Lock 버튼 
#define		SECURITY_WND_LOCK_POS_X		20
#define		SECURITY_WND_LOCK_POS_Y		35

// Lock 버튼 
#define		SECURITY_WND_SETUPPASS_POS_X		120
#define		SECURITY_WND_SETUPPASS_POS_Y		35

#define		SECURITY_WND_SETUPPASS_CAP_Y		100
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSecuLockWnd::CINFSecuLockWnd(CINFSecuMain* pMain)
{
	m_pINFSecuMain = pMain;
	
	m_pLockBk = NULL;	

	m_nBackPosX = m_nBackPosY= 0;
	m_nCx = m_nCy = 1;

	m_bMouseLock = FALSE;
	m_ptMouseLockCap.x = m_ptMouseLockCap.y= 0;

	m_pLockBtn = NULL;
	m_pUnLockBtn = NULL;
	m_pLockSetupBtn = NULL;
	m_pLockChangeBtn= NULL;	
	m_bFirstSetup = FALSE;
}

CINFSecuLockWnd::~CINFSecuLockWnd()
{
	CINFSecuLockWnd::DeleteDeviceObjects();
}

HRESULT CINFSecuLockWnd::InitDeviceObjects()
{
	DeleteDeviceObjects();

	char buf[30] ;

	memset(buf, 0x00, 30);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{
		DataHeader	* pDataHeader;
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("securesys");
		m_pLockBk = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pLockBk->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
#else
	{
		if(NULL == m_pLockBk)
		{
			m_pLockBk = new CINFImageEx;
		}
		wsprintf(buf,"se_lbk");
		auto pDataHeader = g_pGameMain->FindResource(buf);
		m_pLockBk->InitDeviceObjects(pDataHeader);		
	}
#endif


	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "se_lbtn3");
		wsprintf(szDownBtn, "se_lbtn1");
		wsprintf(szSelBtn, "se_lbtn0");
		wsprintf(szDisBtn, "se_lbtn2");
		if(NULL == m_pLockBtn)
		{
			m_pLockBtn = new CINFImageBtn;
		}
		m_pLockBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "se_ulbtn3");
		wsprintf(szDownBtn, "se_ulbtn1");
		wsprintf(szSelBtn, "se_ulbtn0");
		wsprintf(szDisBtn, "se_ulbtn2");
		if(NULL == m_pUnLockBtn)
		{
			m_pUnLockBtn = new CINFImageBtn;
		}
		m_pUnLockBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "se_pbtn3");
		wsprintf(szDownBtn, "se_pbtn1");
		wsprintf(szSelBtn, "se_pbtn0");
		wsprintf(szDisBtn, "se_pbtn2");
		if(NULL == m_pLockSetupBtn)
		{
			m_pLockSetupBtn = new CINFImageBtn;
		}
		m_pLockSetupBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "sel_pcbtn3");
		wsprintf(szDownBtn, "sel_pcbtn1");
		wsprintf(szSelBtn, "sel_pcbtn0");
		wsprintf(szDisBtn, "sel_pcbtn2");
		if(NULL == m_pLockChangeBtn)
		{
			m_pLockChangeBtn = new CINFImageBtn;
		}
		m_pLockChangeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	return S_OK ;
}

HRESULT CINFSecuLockWnd::RestoreDeviceObjects()
{
	{	
		m_pLockBk->RestoreDeviceObjects();		
		POINT ptBk = m_pLockBk->GetImgSize();
				
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_nBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_nBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2) - SECURITY_WND_SETUPPASS_CAP_Y;
		}
		m_nCx = ptBk.x;
		m_nCy = ptBk.y;
	}
	{
		m_pLockBtn->RestoreDeviceObjects();
		m_pUnLockBtn->RestoreDeviceObjects();
		m_pLockSetupBtn->RestoreDeviceObjects();
		m_pLockChangeBtn->RestoreDeviceObjects();
	}

	UpdateBtnPos(m_nBackPosX, m_nBackPosY);	
	
	return S_OK ;
}

HRESULT CINFSecuLockWnd::DeleteDeviceObjects()
{
	if(m_pLockBk)
	{
		m_pLockBk->DeleteDeviceObjects();	
		util::del(m_pLockBk);
	}
	if(m_pLockBtn)
	{
		m_pLockBtn->DeleteDeviceObjects();	
		util::del(m_pLockBtn);
	}
	
	if(m_pUnLockBtn)
	{
		m_pUnLockBtn->DeleteDeviceObjects();	
		util::del(m_pUnLockBtn);
	}
	if(m_pLockSetupBtn)
	{
		m_pLockSetupBtn->DeleteDeviceObjects();	
		util::del(m_pLockSetupBtn);
	}
	if(m_pLockChangeBtn)
	{
		m_pLockChangeBtn->DeleteDeviceObjects();	
		util::del(m_pLockChangeBtn);
	}

	return S_OK ;
}

HRESULT CINFSecuLockWnd::InvalidateDeviceObjects()
{
	m_pLockBk->InvalidateDeviceObjects();
	m_pLockBtn->InvalidateDeviceObjects();		
	m_pUnLockBtn->InvalidateDeviceObjects();		
	m_pLockSetupBtn->InvalidateDeviceObjects();		
	m_pLockChangeBtn->InvalidateDeviceObjects();		
	
	return S_OK ;
}

void	CINFSecuLockWnd::Render()
{
	{
		m_pLockBk->Move(m_nBackPosX,m_nBackPosY);
		m_pLockBk->Render();
	}
	
	m_pLockBtn->Render();
	m_pUnLockBtn->Render();
	m_pLockSetupBtn->Render();
	m_pLockChangeBtn->Render();
	
}
void	CINFSecuLockWnd::Tick()
{

}

int CINFSecuLockWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;	
	return  INF_MSGPROC_BREAK;
}

int CINFSecuLockWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	if((m_nBackPosX < pt.x && pt.x < (m_nBackPosX+m_nCx)  )
		&&(m_nBackPosY < pt.y && pt.y < (m_nBackPosY+m_nCy)  ))
	{
	}
	else
	{
		return INF_MSGPROC_NORMAL;
	}

	{
		if(TRUE == m_pLockBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pUnLockBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pLockSetupBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pLockChangeBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	if(pt.y < (m_nBackPosY+SECU_WND_TITLEBAR_HEIGHT) )
	{
		m_bMouseLock = TRUE;
		
		m_ptMouseLockCap.x = pt.x - m_nBackPosX;
		m_ptMouseLockCap.y = pt.y - m_nBackPosY;	
	}

	return INF_MSGPROC_BREAK;	
}
int CINFSecuLockWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	m_pLockBtn->OnMouseMove(pt);	
	m_pUnLockBtn->OnMouseMove(pt);	
	m_pLockSetupBtn->OnMouseMove(pt);	
	m_pLockChangeBtn->OnMouseMove(pt);	


	if(FALSE == m_bMouseLock )
	{
		return INF_MSGPROC_NORMAL;
	}

	m_nBackPosX = pt.x - m_ptMouseLockCap.x;
	m_nBackPosY = pt.y - m_ptMouseLockCap.y;

	// X 좌표 보정
	if(m_nBackPosX < 0)
	{
		m_nBackPosX = 0;
	}
	else if((m_nBackPosX+m_nCx) > g_pD3dApp->GetBackBufferDesc().Width)
	{
		m_nBackPosX = g_pD3dApp->GetBackBufferDesc().Width - m_nCx;
	}
	
	// Y 좌표 보정
	if(m_nBackPosY < 0)
	{
		m_nBackPosY = 0;
	}
	else if((m_nBackPosY+m_nCy) > g_pD3dApp->GetBackBufferDesc().Height)
	{
		m_nBackPosY = g_pD3dApp->GetBackBufferDesc().Height - m_nCy;
	}	

	// 버튼 위치 업데이트 
	UpdateBtnPos(m_nBackPosX, m_nBackPosY);	

	return INF_MSGPROC_BREAK;	
}
void CINFSecuLockWnd::UpdateBtnPos(int nBackPosX, int nBackPosY)
{
	int nPosX, nPosY;

	{
		nPosX = nBackPosX + SECURITY_WND_LOCK_POS_X;
		nPosY = nBackPosY + SECURITY_WND_LOCK_POS_Y;		
		m_pLockBtn->SetBtnPosition(nPosX, nPosY);
	}
	
	{
		nPosX = nBackPosX + SECURITY_WND_LOCK_POS_X;
		nPosY = nBackPosY + SECURITY_WND_LOCK_POS_Y;		
		m_pUnLockBtn->SetBtnPosition(nPosX, nPosY);
	}
	{
		nPosX = nBackPosX + SECURITY_WND_SETUPPASS_POS_X;
		nPosY = nBackPosY + SECURITY_WND_SETUPPASS_POS_Y;		
		m_pLockSetupBtn->SetBtnPosition(nPosX, nPosY);
	}
	
	{
		nPosX = nBackPosX + SECURITY_WND_SETUPPASS_POS_X;
		nPosY = nBackPosY + SECURITY_WND_SETUPPASS_POS_Y;		
		m_pLockChangeBtn->SetBtnPosition(nPosX, nPosY);
	}
}

int CINFSecuLockWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	{
		if(TRUE == m_pLockBtn->OnLButtonUp(pt))
		{		
			// 처음 접속하여 락클릭 
			if(FALSE == g_pD3dApp->IsSetSecondaryPassword())
			{
				g_pGameMain->CreateChatChild(STRMSG_C_070917_0204, COLOR_SYSTEM); // "\\y보안 비밀번호를 먼저 설정하셔야 합니다."
			}
			else
			{
				// 버튼 클릭 			
				m_pINFSecuMain->OnClickLock(SECURITY_MODE_PASS_LOCK);
			}
			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pUnLockBtn->OnLButtonUp(pt))
		{			
			m_pINFSecuMain->OnClickLock(SECURITY_MODE_PASS_UNLOCK);
			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	
	{
		if(TRUE == m_pLockSetupBtn->OnLButtonUp(pt))
		{				
			// 버튼 클릭 
			OnClickSetupPass();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pLockChangeBtn->OnLButtonUp(pt))
		{				
			// 버튼 클릭 			
			OnClickSetupChangePass();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	if(FALSE == m_bMouseLock )
	{
		return INF_MSGPROC_NORMAL;
	}
	m_bMouseLock = FALSE;
	return INF_MSGPROC_BREAK;	
}

void CINFSecuLockWnd::InitBtn()
{
	if(TRUE == m_bFirstSetup)
	{
		return;
	}
	m_bFirstSetup = TRUE;
	
	BOOL bLock = FALSE;
	if(TRUE == g_pD3dApp->IsUseSecondaryPasswordSystem())	
	{
		if(FALSE == g_pD3dApp->IsSetSecondaryPassword())
		{			
			bLock = TRUE;		
		}			
		// 2008-08-25 by bhsohn 아레나 이용시, 2차 패스워드 문제 해결
		else
		{
			if(FALSE == g_pD3dApp->IsSecondPassLock())
			{
				bLock = TRUE;
			}
		}
		// end 2008-08-25 by bhsohn 아레나 이용시, 2차 패스워드 문제 해결
	}	
	
	m_pLockBtn->ShowWindow(bLock);
	m_pUnLockBtn->ShowWindow(!bLock);

	m_pLockSetupBtn->ShowWindow(bLock);
	m_pLockChangeBtn->ShowWindow(!bLock);
}

void CINFSecuLockWnd::OnClickSetupPass()
{
	m_pINFSecuMain->OnClickSetupPass();
}
void CINFSecuLockWnd::OnClickSetupChangePass()
{
	m_pINFSecuMain->OnClickSetupChangePass();
}



// Lock버튼 업데이트
void CINFSecuLockWnd::UpdateLockBtn(BOOL bLock)
{
	m_pLockBtn->ShowWindow(bLock);
	m_pUnLockBtn->ShowWindow(!bLock);
}

void CINFSecuLockWnd::SetFirstMode(BOOL bFirstLoad)
{
	m_bFirstSetup = bFirstLoad;
}
