// INFSecuPassWnd.cpp: implementation of the CINFSecuPassWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "INFSecuMain.h"

#include "INFSecuPassWnd.h"
#include "INFWindow.h"

#define	SECURITY_WND_TITLEBAR_HEIGHT		20

#define		SECURITU_WND_WIDTH					252
#define		SECURITU_WND_HEIGHT					100

// OK 버튼 
#define		SECURITY_WND_OK_X			83
#define		SECURITY_WND_OK_Y			68

// Cancel 버튼 
#define		SECURITY_WND_CANCEL_X		128
#define		SECURITY_WND_CANCEL_Y		68

// Edit 컨트롤 (Pass)
#define		SECURITY_WND_PASS_EDIT_X		19
#define		SECURITY_WND_PASS_EDIT_Y		48
#define		SECURITY_WND_PASS_EDIT_W		200
#define		SECURITY_WND_PASS_EDIT_H		20



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSecuPassWnd::CINFSecuPassWnd(CINFSecuMain* pMain)
{
	m_pINFSecuMain = pMain;

	for(int i=0;i<9;i++)
		m_pBoxImage[i] = NULL;

	m_nBackPosX = m_nBackPosY= 0;
	m_nCx = m_nCy = 1;

	m_bMouseLock = FALSE;
	m_ptMouseLockCap.x = m_ptMouseLockCap.y= 0;

	m_pSetOk = NULL;
	m_pSetCancel = NULL;
	m_pINFPassEditBox = NULL;

	m_pTxtFont = NULL;

	m_nUnLockMode = -1;
	

}

CINFSecuPassWnd::~CINFSecuPassWnd()
{
	CINFSecuPassWnd::DeleteDeviceObjects();
}


HRESULT CINFSecuPassWnd::InitDeviceObjects()
{
	DeleteDeviceObjects();
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			char buf[12];
			wsprintf(buf, "box%d%d",i,j);
			m_pBoxImage[i*3+j] = new CINFImageEx;
			DataHeader* pDataHeader = g_pGameMain->FindResource(buf);
			m_pBoxImage[i*3+j]->InitDeviceObjects(pDataHeader);
		}
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
		if(NULL == m_pSetOk)
		{
			m_pSetOk = new CINFImageBtn;
		}
		m_pSetOk->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
		if(NULL == m_pSetCancel)
		{
			m_pSetCancel = new CINFImageBtn;
		}
		m_pSetCancel->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#else
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "shlaok00");
		wsprintf(szDownBtn, "shlaok01");
		wsprintf(szSelBtn, "shlaok03");
		wsprintf(szDisBtn, "shlaok02");
		if(NULL == m_pSetOk)
		{
			m_pSetOk = new CINFImageBtn;
		}
		m_pSetOk->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "shmcan00");
		wsprintf(szDownBtn, "shmcan01");
		wsprintf(szSelBtn, "shmcan03");
		wsprintf(szDisBtn, "shmcan02");
		if(NULL == m_pSetCancel)
		{
			m_pSetCancel = new CINFImageBtn;
		}
		m_pSetCancel->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#endif
	{
		if(NULL == m_pINFPassEditBox)
		{
			m_pINFPassEditBox = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_WND_PASS_EDIT_X, SECURITY_WND_PASS_EDIT_Y};
		m_pINFPassEditBox->InitDeviceObjects(9, ptPos, SECURITY_WND_PASS_EDIT_W, TRUE, SECURITY_WND_PASS_EDIT_H);		
	}

	{
		m_pTxtFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pTxtFont->InitDeviceObjects(g_pD3dDev) ;
	}

	return S_OK ;
}

HRESULT CINFSecuPassWnd::RestoreDeviceObjects()
{
	m_nCx = SECURITU_WND_WIDTH;
	m_nCy = SECURITU_WND_HEIGHT;

	m_nBackPosX = g_pD3dApp->GetBackBufferDesc().Width/2-m_nCx/2;
	m_nBackPosY = g_pD3dApp->GetBackBufferDesc().Height/2-m_nCy/2;
	
		
	for(int i=0;i<9;i++)
	{
		m_pBoxImage[i]->RestoreDeviceObjects();
	}	

	{
		m_pSetOk->RestoreDeviceObjects();
		m_pSetCancel->RestoreDeviceObjects();
		m_pINFPassEditBox->RestoreDeviceObjects();		
		m_pTxtFont->RestoreDeviceObjects();	
	}
	// 버튼 위치 업데이트 
	UpdateBtnPos(m_nBackPosX, m_nBackPosY);	
	
	return S_OK ;
}

HRESULT CINFSecuPassWnd::DeleteDeviceObjects()
{
	for(int i=0;i<9;i++)
	{
		if(m_pBoxImage[i])
		{
			m_pBoxImage[i]->DeleteDeviceObjects();
			util::del(m_pBoxImage[i] ) ;
		}
	}
	if(m_pSetOk)
	{
		m_pSetOk->DeleteDeviceObjects();	
		util::del(m_pSetOk);
	}
	if(m_pSetCancel)
	{
		m_pSetCancel->DeleteDeviceObjects();	
		util::del(m_pSetCancel);
	}
	
	if(m_pINFPassEditBox)
	{
		m_pINFPassEditBox->DeleteDeviceObjects();	
		util::del(m_pINFPassEditBox);
	}
	if(m_pTxtFont)
	{
		m_pTxtFont->DeleteDeviceObjects();
		util::del(m_pTxtFont);
	}
	
	return S_OK ;
}

HRESULT CINFSecuPassWnd::InvalidateDeviceObjects()
{
	for(int i=0;i<9;i++)
	{
		m_pBoxImage[i]->InvalidateDeviceObjects();	
	}
	m_pSetOk->InvalidateDeviceObjects();		
	m_pSetCancel->InvalidateDeviceObjects();		
	m_pINFPassEditBox->InvalidateDeviceObjects();		
	m_pTxtFont->InvalidateDeviceObjects();
	
	return S_OK ;
}

void	CINFSecuPassWnd::Render()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	g_pGameMain->m_pInfWindow->RenderCenterWindow(m_nBackPosX, m_nBackPosY, m_nCx, m_nCy);
#else
	RenderCenterWindow(m_nBackPosX, m_nBackPosY, m_nCx, m_nCy, FALSE);
#endif
	
	RenderString(m_nBackPosX, m_nBackPosY);
	{
		m_pSetOk->Render();
		m_pSetCancel->Render();
		
		m_pINFPassEditBox->Render();
	}
}

void CINFSecuPassWnd::RenderString(int nBackPosX, int nBackPosY)
{
	SIZE czStr = m_pTxtFont->GetStringSize(STRMSG_C_070917_0201);
	int nPosX = m_nBackPosX + (m_nCx/2 - czStr.cx/2) ;
	int nPosY = m_nBackPosY + SECURITY_WND_TITLEBAR_HEIGHT;

	m_pTxtFont->DrawText(nPosX, nPosY, 
				GUI_FONT_COLOR_W, 
				STRMSG_C_070917_0201);
}


void	CINFSecuPassWnd::Tick()
{
	m_pINFPassEditBox->Tick();
}

int CINFSecuPassWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_KEYDOWN:
		{
			if(m_pINFPassEditBox->WndProc(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;		
	return INF_MSGPROC_BREAK;
}
void CINFSecuPassWnd::RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose)
{
	FLOG( "CINFWindow::RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose)" );
	m_pBoxImage[0]->Move(x,y);
	m_pBoxImage[0]->Render();
	m_pBoxImage[1]->Move(x+19, y);
	m_pBoxImage[1]->SetScale(cx-38, 1);
	m_pBoxImage[1]->Render();
	m_pBoxImage[2]->Move(x+cx-19,y);
	m_pBoxImage[2]->Render();

	m_pBoxImage[3]->Move(x, y+19);
	m_pBoxImage[3]->SetScale(1, cy-38);
	m_pBoxImage[3]->Render();
	m_pBoxImage[4]->Move(x+19, y+19);
	m_pBoxImage[4]->SetScale(cx-38,cy-38);
	m_pBoxImage[4]->Render();
	m_pBoxImage[5]->Move(x+cx-19, y+19);
	m_pBoxImage[5]->SetScale(1, cy-38);
	m_pBoxImage[5]->Render();

	m_pBoxImage[6]->Move(x,y+cy-19);
	m_pBoxImage[6]->Render();
	m_pBoxImage[7]->Move(x+19,y+cy-19);
	m_pBoxImage[7]->SetScale(cx-38, 1);
	m_pBoxImage[7]->Render();
	m_pBoxImage[8]->Move(x+cx-19,y+cy-19);
	m_pBoxImage[8]->Render();
//	if(bRenderClose)
//	{
//		m_pXclose->Move(x+cx-14,y+6);
//		m_pXclose->Render();
//	}
}


// 버튼 위치 업데이트 
void CINFSecuPassWnd::UpdateBtnPos(int nBackPosX, int nBackPosY)
{
	int nPosX, nPosY;
	{
		nPosX = nBackPosX + SECURITY_WND_OK_X;
		nPosY = nBackPosY + SECURITY_WND_OK_Y;		
		m_pSetOk->SetBtnPosition(nPosX, nPosY);
	}
	{
		nPosX = nBackPosX + SECURITY_WND_CANCEL_X;
		nPosY = nBackPosY + SECURITY_WND_CANCEL_Y;		
		m_pSetCancel->SetBtnPosition(nPosX, nPosY);
	}
	{			
		nPosX = nBackPosX + SECURITY_WND_PASS_EDIT_X;
		nPosY = nBackPosY + SECURITY_WND_PASS_EDIT_Y;
		m_pINFPassEditBox->SetPos(nPosX, nPosY);
	}
	
}

int CINFSecuPassWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
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
		if(TRUE == m_pINFPassEditBox->OnLButtonDown(pt))
		{				
			//RefreshEditCtl(SECURITY_WND_PASS_EDIT, m_pINFPassEditBox);			
			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}	

	{
		if(TRUE == m_pSetOk->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pSetCancel->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	if(pt.y < (m_nBackPosY+SECURITY_WND_TITLEBAR_HEIGHT) )
	{
		m_bMouseLock = TRUE;
		
		m_ptMouseLockCap.x = pt.x - m_nBackPosX;
		m_ptMouseLockCap.y = pt.y - m_nBackPosY;	
	}

	return INF_MSGPROC_BREAK;	
}
int CINFSecuPassWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	m_pSetOk->OnMouseMove(pt);	
	m_pSetCancel->OnMouseMove(pt);	


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
int CINFSecuPassWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	{
		if(TRUE == m_pSetOk->OnLButtonUp(pt))
		{				
			OnClickOk();

			// 버튼 클릭 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pSetCancel->OnLButtonUp(pt))
		{			
			OnClickCancel();

			// 버튼 클릭 			
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


void CINFSecuPassWnd::ShowSetPassWnd(int nUnLockMode)
{		
	m_nUnLockMode = nUnLockMode;
	
	ClearEdit();
	m_pINFPassEditBox->EnableEdit(TRUE);	
}
void CINFSecuPassWnd::OnCloseInfWnd()
{
	ClearEdit();
	m_pINFPassEditBox->EnableEdit(FALSE);
}

void CINFSecuPassWnd::OnClickCancel()
{
	m_pINFSecuMain->ShowSecurityWnd(SECURITY_MODE_PASS_WND,  FALSE);
}

void CINFSecuPassWnd::OnClickOk()
{
	char chPass[32];
	memset(chPass, 0x00, 32);
	m_pINFPassEditBox->GetString(chPass, 32);
	if(0 == strlen(chPass))		
	{
		g_pGameMain->CreateChatChild(STRMSG_C_070917_0203, COLOR_ERROR);// "\\y보안 비밀번호를 잘못 입력하셨습니다."
		return;
	}

	switch(m_nUnLockMode)
	{
	case SECURITY_MODE_PASS_LOCK:
		{
			m_pINFSecuMain->SendCheckPassword(chPass, FALSE);
		}
		break;
	case SECURITY_MODE_PASS_UNLOCK:
		{
			m_pINFSecuMain->SendCheckPassword(chPass, TRUE);			
		}
		break;
	case SECURITY_MODE_PASS_SENCOND_CANCEL:
		{
			m_pINFSecuMain->SendSecndPassWordCancel(chPass);			
		}
		break;
	}	

	
}

void CINFSecuPassWnd::ClearEdit()
{	
	m_pINFPassEditBox->InitString();
}
