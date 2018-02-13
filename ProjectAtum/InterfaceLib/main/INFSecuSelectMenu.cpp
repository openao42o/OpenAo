// INFSecuSelectMenu.cpp: implementation of the CINFSecuSelectMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "AtumSound.h"
#include "INFSelect.h"
#include "INFSecuEditBox.h"
#include "md5_lib_src.h"

// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"

#include "INFSecuSelectMenu.h"

#define	SECU_WND_TITLEBAR_HEIGHT		20

// OK 버튼 
#define		SECURITY_WND_OK_X			120
#define		SECURITY_WND_OK_Y			130

// Cancel 버튼 
#define		SECURITY_WND_CANCEL_X		165
#define		SECURITY_WND_CANCEL_Y		130

// Edit 컨트롤 (Pass)
#define		SECURITY_WND_USERID_EDIT_X		60
#define		SECURITY_WND_USERID_EDIT_Y		55
#define		SECURITY_WND_USERID_EDIT_W		200
#define		SECURITY_WND_USERID_EDIT_H		20

// Edit 컨트롤 (확인)
#define		SECURITY_WND_USERPASS_EDIT_X		60
#define		SECURITY_WND_USERPASS_EDIT_Y		105
#define		SECURITY_WND_USERPASS_EDIT_W		200
#define		SECURITY_WND_USERPASS_EDIT_H		20

// 최대 글씨 수 
#define		MAX_STRING_LEN					8

// 어떤 컨트롤을 선택할꺼냐?
#define		SECURITY_WND_USERID_EDIT				0
#define		SECURITY_WND_USERPASS_EDIT			1

CINFSecuSelectMenu::CINFSecuSelectMenu(CINFSelect* pMain)
{
	m_pINFSelect = pMain;

	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	m_pGameData = NULL;
	m_pGruopPassmanager = NULL;
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업

	m_pSetPassBk = NULL;	

	m_nBackPosX = m_nBackPosY= 0;
	m_nCx = m_nCy = 1;

	m_bMouseLock = FALSE;
	m_ptMouseLockCap.x = m_ptMouseLockCap.y= 0;

	m_pSetOk = NULL;
	m_pSetCancel = NULL;
	m_pINFUserName = NULL;
	m_pINFSecuPassWord= NULL;

	m_nSelEdit = SECURITY_WND_USERID_EDIT;
	m_bShowWnd =FALSE;

}

CINFSecuSelectMenu::~CINFSecuSelectMenu()
{
	CINFSecuSelectMenu::DeleteDeviceObjects();
}


HRESULT CINFSecuSelectMenu::InitDeviceObjects()
{
	DeleteDeviceObjects();

	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업	
	DataHeader * pDataHeaderBackRect;
	m_pGruopPassmanager = new CINFGroupManager( g_pD3dApp->m_pImageList, m_pINFSelect->m_pGameData ); // select.tex 사용 위해 m_pINFSelect->m_pGameData 넣어줌
	if(m_pGruopPassmanager)
	{
		m_pGruopPassmanager->InitDeviceObjects();
	}	
	pDataHeaderBackRect = m_pGruopPassmanager->FindResource("Cdel");
	m_pSetPassBack = m_pGruopPassmanager->GetGroupImage( pDataHeaderBackRect );
	if(m_pSetPassBack)
	{
	m_pSetPassBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}	
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업

	DataHeader	* pDataHeader = NULL;
	char buf[30] = {0, };

	memset(buf, 0x00, 30);

	{
		if(NULL == m_pSetPassBk)
		{
			m_pSetPassBk = new CINFImageEx;
		}
		wsprintf(buf,"se_chdelbk");
		pDataHeader = m_pINFSelect->FindResource(buf);
		m_pSetPassBk->InitDeviceObjects(pDataHeader);		
	}
// 	{	
// 		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
// 		wsprintf(szUpBtn, "shlaok00");
// 		wsprintf(szDownBtn, "shlaok01");
// 		wsprintf(szSelBtn, "shlaok03");
// 		wsprintf(szDisBtn, "shlaok02");
// 		if(NULL == m_pSetOk)
// 		{
// 			m_pSetOk = new CINFImageBtn;
// 		}
// 		m_pSetOk->InitDeviceObjects_LoadSelect(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
// 	}
// 	
// 	{
// 		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
// 		wsprintf(szUpBtn, "shmcan00");
// 		wsprintf(szDownBtn, "shmcan01");
// 		wsprintf(szSelBtn, "shmcan03");
// 		wsprintf(szDisBtn, "shmcan02");
// 		if(NULL == m_pSetCancel)
// 		{
// 			m_pSetCancel = new CINFImageBtn;
// 		}
// 		m_pSetCancel->InitDeviceObjects_LoadSelect(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
// 	}
	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	{	
		char szUpBtn[30] = {0, }, szDownBtn[30] = {0, }, szSelBtn[30] = {0, }, szDisBtn[30] = {0, };		
		wsprintf(szUpBtn, "Appnor");
		wsprintf(szDownBtn, "Apppush");
		wsprintf(szSelBtn, "Appover");
		wsprintf(szDisBtn, "Appdis");
		if(NULL == m_pSetOk)
		{
			m_pSetOk = new CINFImageBtn;
		}
		m_pSetOk->InitDeviceObjects_LoadSelect(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	
	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	{
		char szUpBtn[30] = {0, }, szDownBtn[30] = {0, }, szSelBtn[30] = {0, }, szDisBtn[30] = {0, };		
		wsprintf(szUpBtn, "Cannor");
		wsprintf(szDownBtn, "Canpush");
		wsprintf(szSelBtn, "Canover");
		wsprintf(szDisBtn, "Candis");
		if(NULL == m_pSetCancel)
		{
			m_pSetCancel = new CINFImageBtn;
		}
		m_pSetCancel->InitDeviceObjects_LoadSelect(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업

	{
		if(NULL == m_pINFUserName)
		{
			m_pINFUserName = new CINFEditBox;
		}
		
		POINT ptPos = {SECURITY_WND_USERID_EDIT_X, SECURITY_WND_USERID_EDIT_Y};
		m_pINFUserName->InitDeviceObjects(9, ptPos, SECURITY_WND_USERID_EDIT_W, TRUE, SECURITY_WND_USERID_EDIT_H);		
	}
	
	{
		if(NULL == m_pINFSecuPassWord)
		{
			m_pINFSecuPassWord = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_WND_USERPASS_EDIT_X, SECURITY_WND_USERPASS_EDIT_Y};
		m_pINFSecuPassWord->InitDeviceObjects(9, ptPos, SECURITY_WND_USERPASS_EDIT_W, TRUE, SECURITY_WND_USERPASS_EDIT_H);		
	}
	
	m_pINFUserName->EnableEdit(TRUE, FALSE);

	m_nSelEdit = SECURITY_WND_USERID_EDIT;

	//m_pINFUserName->SetMaxStringLen(MAX_STRING_LEN);
	m_pINFSecuPassWord->SetMaxStringLen(MAX_STRING_LEN);

	return S_OK ;
}

HRESULT CINFSecuSelectMenu::RestoreDeviceObjects()
{
	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	if(m_pGruopPassmanager)
	{
		m_pGruopPassmanager->RestoreDeviceObjects();
	}
	if(m_pSetPassBack)
	{
	m_pSetPassBack->RestoreDeviceObjects();
	}
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	{	
		m_pSetPassBk->RestoreDeviceObjects();		
		POINT ptBk = m_pSetPassBk->GetImgSize();
				
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_nBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_nBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2);
		}
		m_nCx = ptBk.x;
		m_nCy = ptBk.y;
	}
	{
		m_pSetOk->RestoreDeviceObjects();
		m_pSetCancel->RestoreDeviceObjects();
		m_pINFUserName->RestoreDeviceObjects();		
		m_pINFSecuPassWord->RestoreDeviceObjects();		
	}
	// 버튼 위치 업데이트 
	UpdateBtnPos(m_nBackPosX, m_nBackPosY);	
	
	return S_OK ;
}

HRESULT CINFSecuSelectMenu::DeleteDeviceObjects()
{
	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	if(m_pGruopPassmanager)
	{
		m_pGruopPassmanager->DeleteDeviceObjects();
		util::del(m_pGruopPassmanager);	
	}
	if(m_pSetPassBack)
	{
		m_pSetPassBack->DeleteDeviceObjects();
		util::del(m_pSetPassBack);
	}
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업

	if(m_pSetPassBk)
	{
		m_pSetPassBk->DeleteDeviceObjects();	
		util::del(m_pSetPassBk);
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
	if(m_pINFUserName)
	{
		m_pINFUserName->DeleteDeviceObjects();	
		util::del(m_pINFUserName);
	}
	
	if(m_pINFSecuPassWord)
	{
		m_pINFSecuPassWord->DeleteDeviceObjects();	
		util::del(m_pINFSecuPassWord);
	}
	
	return S_OK ;
}

HRESULT CINFSecuSelectMenu::InvalidateDeviceObjects()
{
	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	if(m_pGruopPassmanager)
	{
		m_pGruopPassmanager->InvalidateDeviceObjects();
	}
	if(m_pSetPassBack)
	{
	m_pSetPassBack->InvalidateDeviceObjects();
	}
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	m_pSetPassBk->InvalidateDeviceObjects();
	m_pSetOk->InvalidateDeviceObjects();		
	m_pSetCancel->InvalidateDeviceObjects();		
	m_pINFUserName->InvalidateDeviceObjects();		
	m_pINFSecuPassWord->InvalidateDeviceObjects();		
	return S_OK ;
}

void	CINFSecuSelectMenu::Render()
{
	if(FALSE == IsShowWnd() )
	{
		return ;
	}
	// 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	if(m_pSetPassBack)
	{
		m_pSetPassBack->Move(m_nBackPosX, m_nBackPosY);
		m_pSetPassBack->Render();
	}
	// end 2012-02-27 by mspark, 2차 암호 설정이 되어 있고, [기어 선택화면]에서 캐릭터 삭제 시 삭제창 작업
	{
		m_pSetPassBk->Move(m_nBackPosX,m_nBackPosY);
		m_pSetPassBk->Render();
	}
	m_pSetOk->Render();
	m_pSetCancel->Render();
	{		
		m_pINFUserName->Render();
		m_pINFSecuPassWord->Render();
	}
}
void	CINFSecuSelectMenu::Tick()
{
	if(FALSE == IsShowWnd() )
	{
		return ;
	}
	m_pINFUserName->Tick();
	m_pINFSecuPassWord->Tick();
}

int CINFSecuSelectMenu::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}
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

	case WM_IME_STARTCOMPOSITION:
//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{
			switch(m_nSelEdit)
			{
			case SECURITY_WND_USERID_EDIT:
				{
					if(m_pINFUserName->WndProc(uMsg, wParam, lParam))
					{
						return INF_MSGPROC_BREAK;
					}
				}
				break;			
			}			
		}
		break;
	case WM_KEYDOWN:
		{
			switch(m_nSelEdit)
			{
			case SECURITY_WND_USERPASS_EDIT:
				{
					if(m_pINFSecuPassWord->WndProc(uMsg, wParam, lParam))
					{
						return INF_MSGPROC_BREAK;
					}
				}
				break;
			}			
		}
		break;
	}	
	return INF_MSGPROC_NORMAL;		
}

int CINFSecuSelectMenu::OnLButtonDown(WPARAM wParam, LPARAM lParam)
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
		if(TRUE == m_pINFUserName->OnLButtonDown(pt))
		{				
			RefreshEditCtl(SECURITY_WND_USERID_EDIT);			
			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{		
		if(TRUE == m_pINFSecuPassWord->OnLButtonDown(pt))
		{			
			RefreshEditCtl(SECURITY_WND_USERPASS_EDIT);						
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
	
	if(pt.y < (m_nBackPosY+SECU_WND_TITLEBAR_HEIGHT) )
	{
		m_bMouseLock = TRUE;
		
		m_ptMouseLockCap.x = pt.x - m_nBackPosX;
		m_ptMouseLockCap.y = pt.y - m_nBackPosY;	
	}

	return INF_MSGPROC_BREAK;	
}
int CINFSecuSelectMenu::OnMouseMove(WPARAM wParam, LPARAM lParam)
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
void CINFSecuSelectMenu::UpdateBtnPos(int nBackPosX, int nBackPosY)
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
		nPosX = nBackPosX + SECURITY_WND_USERID_EDIT_X;
		nPosY = nBackPosY + SECURITY_WND_USERID_EDIT_Y;
		m_pINFUserName->SetPos(nPosX, nPosY);
	}
	{		
		nPosX = nBackPosX + SECURITY_WND_USERPASS_EDIT_X;
		nPosY = nBackPosY + SECURITY_WND_USERPASS_EDIT_Y;
		m_pINFSecuPassWord->SetPos(nPosX, nPosY);
	}
}

int CINFSecuSelectMenu::OnLButtonUp(WPARAM wParam, LPARAM lParam)
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

// 닫기 버튼 클릭
void CINFSecuSelectMenu::OnCloseInfWnd()
{
	g_pD3dApp->CleanText();

	m_pINFUserName->InitString();		
	m_pINFUserName->EnableEdit(FALSE, FALSE);

	m_pINFSecuPassWord->InitString();		
	m_pINFSecuPassWord->EnableEdit(FALSE);
}

// Show Pass 윈도우 
void CINFSecuSelectMenu::ShowSetPassWnd()
{	
	ShowSecutiryWnd(TRUE);
	
	m_nSelEdit = SECURITY_WND_USERID_EDIT;
	m_pINFUserName->EnableEdit(TRUE, FALSE);	
}

// Ok버튼 클릭 
void CINFSecuSelectMenu::OnClickOk()
{
	char chUserName[256], chPassWord[256];
	memset(chUserName, 0x00, 256);
	memset(chPassWord, 0x00, 256);
	m_pINFUserName->GetString(chUserName, 256);
	m_pINFSecuPassWord->GetString(chPassWord, 256);

	const int nSelectIndex = g_pSelect->GetCurrentSelectIndex();	
	
	// 이름이 동일해야지만 2차 패스워드를 요청한다. 
	char* pCharacterName = g_pSelect->GetSelectCharcterName(nSelectIndex);
	if(0 == stricmp(pCharacterName, chUserName))
	{
		MD5 MD5_instance;
		unsigned char md5_string[16];
		char szEncodedString[33];
		
		MD5_instance.MD5Encode(chPassWord, md5_string);
		MD5_instance.MD5Binary2String(md5_string, szEncodedString);
		
		g_pD3dApp->SendDeleteCharcter(szEncodedString);		
	}
	else
	{
		ShowSecutiryWnd(FALSE);
		m_pINFSelect->AddMsgBox( STRMSG_C_070917_0207, _MESSAGE );		
	}

}

void CINFSecuSelectMenu::OnClickCancel()
{
	ShowSecutiryWnd(FALSE);	
}

void CINFSecuSelectMenu::RefreshEditCtl(int nEditMode)
{
	if(nEditMode == m_nSelEdit)
	{		
		return;
	}	
	
	if(nEditMode == SECURITY_WND_USERID_EDIT)
	{		
		m_pINFUserName->EnableEdit(TRUE, TRUE);
		m_pINFSecuPassWord->EnableEdit(FALSE);
	}
	else
	{
		m_pINFUserName->EnableEdit(FALSE, TRUE);
		m_pINFSecuPassWord->EnableEdit(TRUE);
	}	
	m_nSelEdit = nEditMode;
}

BOOL CINFSecuSelectMenu::IsShowWnd()
{
	return m_bShowWnd;
}
void CINFSecuSelectMenu::ShowSecutiryWnd(BOOL bShowWnd)
{
	m_bShowWnd = bShowWnd;	
	// 초기화
	OnCloseInfWnd();
	if(bShowWnd)
	{
		g_pSelect->m_bDelete = TRUE;	
	}
	else
	{
		g_pSelect->m_bDelete = FALSE;	
	}
}


