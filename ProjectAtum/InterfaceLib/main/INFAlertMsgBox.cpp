// INFAlertMsgBox.cpp: implementation of the CINFAlertMsgBox class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "INFImageBtn.h"

#include "INFAlertMsgBox.h"
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define OK_POS_X			137
#define OK_POS_Y			117								// 2012-06-15 by isshin 세력선택시 경고창 - OK/CANCEL 설정

#define CANCEL_POS_X		226
#define CANCEL_POS_Y		117								// 2012-06-15 by isshin 세력선택시 경고창 - OK/CANCEL 설정
#else
#define OK_POS_X			163
#define OK_POS_Y			136

#define CANCEL_POS_X		252
#define CANCEL_POS_Y		136
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFAlertMsgBox::CINFAlertMsgBox()
{
	m_nPosX = m_nPosY = 0;
	m_pImgWarning = NULL;
	m_pInfWarning = NULL;
	m_vecString.clear();

	m_bShowPopup = FALSE;

	m_pOkBtn = NULL;	
	m_pCancelBtn = NULL;	

}

CINFAlertMsgBox::~CINFAlertMsgBox()
{
	ResetContent();
	util::del(m_pInfWarning);
	util::del(m_pImgWarning);
	if(m_pOkBtn)
	{
		m_pOkBtn->DeleteDeviceObjects();	
		util::del(m_pOkBtn);
	}
	if(m_pCancelBtn)
	{
		m_pCancelBtn->DeleteDeviceObjects();	
		util::del(m_pCancelBtn);
	}
}

HRESULT CINFAlertMsgBox::InitDeviceObjects()
{
	{
		DataHeader	* pDataHeader ;
		char sbuf[30];
		wsprintf(sbuf, "inftex");
		m_pImgWarning = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(sbuf);
		m_pImgWarning->InitDeviceObjects( pDataHeader );
	}
	{
		m_pInfWarning = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pInfWarning->InitDeviceObjects(g_pD3dDev) ;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "okb03");			// 2012-06-15 by isshin 세력선택시 경고창 - OK/CANCEL 설정
		wsprintf(szDownBtn, "okb01");
		wsprintf(szSelBtn, "okb00");		// 2012-06-15 by isshin 세력선택시 경고창 - OK/CANCEL 설정
		wsprintf(szDisBtn, "okb02");
		if(NULL == m_pOkBtn)
		{
			m_pOkBtn = new CINFImageBtn;
		}
		m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "canb03");		// 2012-06-15 by isshin 세력선택시 경고창 - OK/CANCEL 설정
		wsprintf(szDownBtn, "canb01");
		wsprintf(szSelBtn, "canb00");		// 2012-06-15 by isshin 세력선택시 경고창 - OK/CANCEL 설정
		wsprintf(szDisBtn, "canb02");
		if(NULL == m_pCancelBtn)
		{
			m_pCancelBtn = new CINFImageBtn;
		}
		m_pCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);	
	}
#else
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "shlaok00");
		wsprintf(szDownBtn, "shlaok01");
		wsprintf(szSelBtn, "shlaok03");
		wsprintf(szDisBtn, "shlaok02");
		if(NULL == m_pOkBtn)
		{
			m_pOkBtn = new CINFImageBtn;
		}
		m_pOkBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "shmcan00");
		wsprintf(szDownBtn, "shmcan01");
		wsprintf(szSelBtn, "shmcan03");
		wsprintf(szDisBtn, "shmcan02");
		if(NULL == m_pCancelBtn)
		{
			m_pCancelBtn = new CINFImageBtn;
		}
		m_pCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#endif	
		
	return S_OK;
}
HRESULT CINFAlertMsgBox::RestoreDeviceObjects()
{
	m_pImgWarning->RestoreDeviceObjects();
	m_pInfWarning->RestoreDeviceObjects();
	
	{			
		m_pOkBtn->RestoreDeviceObjects();			
	}
	{			
		m_pCancelBtn->RestoreDeviceObjects();			
	}
	
	return S_OK;
}
HRESULT CINFAlertMsgBox::DeleteDeviceObjects()
{
	m_pImgWarning->DeleteDeviceObjects();
	util::del(m_pImgWarning);
	
	m_pInfWarning->DeleteDeviceObjects();
	util::del(m_pInfWarning);

	{
		m_pOkBtn->DeleteDeviceObjects();	
		util::del(m_pOkBtn);
	}
	{
		m_pCancelBtn->DeleteDeviceObjects();	
		util::del(m_pCancelBtn);
	}
		
	return S_OK;
}
HRESULT CINFAlertMsgBox::InvalidateDeviceObjects()
{
	m_pImgWarning->InvalidateDeviceObjects();
	m_pInfWarning->InvalidateDeviceObjects();
	m_pOkBtn->InvalidateDeviceObjects();		
	m_pCancelBtn->InvalidateDeviceObjects();		
	
	return S_OK;
}
void CINFAlertMsgBox::Render()
{
	if(FALSE == IsShowPopup() )
	{
		return;
	}
	m_pImgWarning->Move(m_nPosX, m_nPosY);
	m_pImgWarning->Render();

	vector<stMsgBoxString*>::iterator it = m_vecString.begin();
	while(it != m_vecString.end())
	{
		stMsgBoxString* pString = (*it);
		m_pInfWarning->DrawText(pString->nStartX, pString->nStartY, 
								GUI_FONT_COLOR_W, 
								pString->strTxt);
		it++;
	}
	m_pOkBtn->Render();
	m_pCancelBtn->Render();

}
void CINFAlertMsgBox::Tick()
{
	if(FALSE == IsShowPopup() )
	{
		return;
	}
}

void CINFAlertMsgBox::AddString(int nStartX, int nStartY, char* pTxt)
{
	stMsgBoxString* struTmp = new stMsgBoxString ;
	struTmp->nStartX = nStartX;
	struTmp->nStartY = nStartY;
	strncpy(struTmp->strTxt, pTxt, strlen(pTxt)+1);

	m_vecString.push_back(struTmp);
}

void CINFAlertMsgBox::ResetContent()
{
	vector<stMsgBoxString*>::iterator it = m_vecString.begin();
	while(it != m_vecString.end())
	{
		util::del(*it);
		it++;
	}
	m_vecString.clear();
}

void CINFAlertMsgBox::ShowAlertWnd(BOOL bShow, int nStartX, int nStartY)
{
	m_bShowPopup = bShow;
	if(bShow)
	{
		m_nPosX = nStartX;
		m_nPosY = nStartY;
	}	
	int nPosX = m_nPosX + OK_POS_X;
	int nPosY = m_nPosY + OK_POS_Y;		
	m_pOkBtn->SetBtnPosition(nPosX, nPosY);

	nPosX = m_nPosX + CANCEL_POS_X;
	nPosY = m_nPosY + CANCEL_POS_Y;	
	m_pCancelBtn->SetBtnPosition(nPosX, nPosY);
	
}


int CINFAlertMsgBox::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowPopup() )
	{
		return INF_MSGPROC_NORMAL;
	}
	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	
	return INF_MSGPROC_NORMAL;	

}
int CINFAlertMsgBox::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowPopup() )
	{
		return INF_MSGPROC_NORMAL;
	}

	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	BOOL bClick = FALSE;	
	
	{		
		if(TRUE == m_pOkBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}	
	}
	{		
		if(TRUE == m_pCancelBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}	
	}

	

	return INF_MSGPROC_NORMAL;	

}
int CINFAlertMsgBox::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowPopup() )
	{
		return INF_MSGPROC_NORMAL;
	}

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	m_pOkBtn->OnMouseMove(pt);
	m_pCancelBtn->OnMouseMove(pt);
	
	return INF_MSGPROC_NORMAL;	
	
}
int CINFAlertMsgBox::OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL *o_bOk)
{
	if(FALSE == IsShowPopup() )
	{
		return INF_MSGPROC_NORMAL;
	}
	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		if(TRUE == m_pOkBtn->OnLButtonUp(pt))
		{					
			(*o_bOk) = TRUE;
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);				
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pCancelBtn->OnLButtonUp(pt))
		{					
			(*o_bOk) = FALSE;
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);				
			return  INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;		
}

BOOL CINFAlertMsgBox::IsShowPopup()
{
	return m_bShowPopup;
}