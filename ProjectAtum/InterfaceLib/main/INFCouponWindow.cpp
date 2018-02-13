// INFCouponWindow.cpp: implementation of the CINFCouponWindow class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFCouponWindow.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3dHanFont.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경
#include "INFGameMain.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef C_EPSODE4_UI_CHANGE_JSKIM

#define	COUPON_WINDOW_OK_X				(m_ptWindow.x + 108)
#define	COUPON_WINDOW_OK_Y				(m_ptWindow.y + 148)
#define	COUPON_WINDOW_CANCEL_X			(m_ptWindow.x + 163)
#define	COUPON_WINDOW_CANCEL_Y			(m_ptWindow.y + 148)

#else
#define	COUPON_WINDOW_OK_X				(m_ptWindow.x + 210)
#define	COUPON_WINDOW_OK_Y				(m_ptWindow.y + 148)
#define	COUPON_WINDOW_CANCEL_X			(m_ptWindow.x + 256)
#define	COUPON_WINDOW_CANCEL_Y			(m_ptWindow.y + 148)

#endif

#define	COUPON_WINDOW_CLOSE_X			(m_ptWindow.x + 280)
#define	COUPON_WINDOW_CLOSE_W			26
#define	COUPON_WINDOW_CLOSE_H			18
#define	COUPON_WINDOW_NUMBER_X			(m_ptWindow.x + 46)
#define	COUPON_WINDOW_NUMBER_Y			(m_ptWindow.y + 92)
#define COUPON_WINDOW_W					307
#define COUPON_WINDOW_H					183


CINFCouponWindow::CINFCouponWindow()
{

}

CINFCouponWindow::~CINFCouponWindow()
{

}

CINFCouponWindow::CINFCouponWindow(CAtumNode* pParent)
{
	m_pParent			= pParent;
	m_bShow				= FALSE;
	m_pImgOkB			= NULL;
	m_pImgCancelB		= NULL;
	m_pImgBackGround	= NULL;
	m_pFont				= NULL;
	memset(&m_ptWindow,0x00,sizeof(POINT));
	InitCouponNumber();

}

void CINFCouponWindow::InitCouponNumber()
{
	memset(&m_strCouponNumber,0x00,MAX_COUPON_NUMBER_SIZE);
	m_strCouponNumber[0] = '_';

}
void CINFCouponWindow::SetString(char * i_pChar,INT nSize)
{
	strncpy(m_strCouponNumber,i_pChar,nSize);
	m_strCouponNumber[nSize] = '_';
	m_strCouponNumber[nSize+1] = NULL;
}
void CINFCouponWindow::SetActive(MSG_FC_EVENT_COUPON_EVENT_INFO* pMsg)
{
	m_CouponInfo.ItemEventType = pMsg->ItemEventType;
	m_CouponInfo.ItemEventUID = pMsg->ItemEventUID;
}
void CINFCouponWindow::SendCouponNumber()
{
	INT nSize = strlen(m_strCouponNumber);
	if(nSize > 1)
	{
		MSG_FC_EVENT_COUPON_EVENT_USE_COUPON sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		strncpy(sMsg.CouponNumber,m_strCouponNumber,nSize-1);
		sMsg.ItemEventUID = m_CouponInfo.ItemEventUID;
		g_pD3dApp->m_pFieldWinSocket->SendMsg(T_FC_EVENT_COUPON_EVENT_USE_COUPON,(char*)&sMsg,sizeof(sMsg));
		g_pD3dApp->m_bRequestEnable = FALSE;
	}
}

HRESULT CINFCouponWindow::InitDeviceObjects()
{
	DataHeader	* pDataHeader;

	char szButtonName[32];
	{
		wsprintf(szButtonName, "okb0");
		if(NULL == m_pImgOkB)
		{
			m_pImgOkB = new CINFImageBtn;
		}
		m_pImgOkB->InitDeviceObjects(szButtonName);	
	}
	{
		wsprintf(szButtonName, "canb0");
		if(NULL == m_pImgCancelB)
		{
			m_pImgCancelB = new CINFImageBtn;
		}
		m_pImgCancelB->InitDeviceObjects(szButtonName);	
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("coupon");	
	m_pImgBackGround = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pImgBackGround->InitDeviceObjects( g_pD3dApp->m_pImageList );

	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("ok_cancel");
	m_pCouponControl = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);	

#else
	if(NULL == m_pImgBackGround)
	{
		m_pImgBackGround = new CINFImageEx;
		pDataHeader = FindResource("couponW");
		m_pImgBackGround->InitDeviceObjects(pDataHeader);
	}
#endif
	if(NULL == m_pFont)
	{
		m_pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,512,32);
		m_pFont->InitDeviceObjects(g_pD3dDev);
	}


	return S_OK;
}
HRESULT CINFCouponWindow::RestoreDeviceObjects()
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	POINT pBkSize;
	pBkSize.x = m_pImgBackGround->GetMaxPos().x - m_pImgBackGround->GetMinPos().x;
	int nSizeX = m_pCouponControl->GetFindControlTargetofMinPos("okb00").x - m_pCouponControl->GetFindControlTargetofMinPos("canb00").x;// 2011-02-08 by jhahn EP4 용해  UI 변경
	
	m_pImgOkB->RestoreDeviceObjects();
				

	
	m_pImgOkB->SetBtnPosition( COUPON_WINDOW_OK_X, COUPON_WINDOW_OK_Y - 12);	
	m_pImgCancelB->RestoreDeviceObjects();
	m_pImgCancelB->SetBtnPosition( COUPON_WINDOW_CANCEL_X, COUPON_WINDOW_CANCEL_Y - 12);
#else
	m_pImgOkB->RestoreDeviceObjects();
	m_pImgOkB->SetBtnPosition(COUPON_WINDOW_OK_X,COUPON_WINDOW_OK_Y);
	
	m_pImgCancelB->RestoreDeviceObjects();
	m_pImgCancelB->SetBtnPosition(COUPON_WINDOW_CANCEL_X,COUPON_WINDOW_CANCEL_Y);
#endif
	
	

	m_pImgBackGround->RestoreDeviceObjects();
	m_pFont->RestoreDeviceObjects();

	return S_OK;
}
HRESULT CINFCouponWindow::DeleteDeviceObjects()
{
	m_pImgOkB->DeleteDeviceObjects();
	m_pImgCancelB->DeleteDeviceObjects();
	util::del(m_pImgOkB);
	util::del(m_pImgCancelB);

	m_pImgBackGround->DeleteDeviceObjects();
	util::del(m_pImgBackGround);
	m_pFont->DeleteDeviceObjects();
	util::del(m_pFont);
	return S_OK;
}
HRESULT CINFCouponWindow::InvalidateDeviceObjects()
{
	m_pImgOkB->InvalidateDeviceObjects();
	m_pImgCancelB->InvalidateDeviceObjects();

	m_pImgBackGround->InvalidateDeviceObjects();
	m_pFont->InvalidateDeviceObjects();
	return S_OK;
}
void CINFCouponWindow::SetWindowPoint(POINT i_ptWindow)
{
	m_ptWindow = i_ptWindow;
	m_pImgOkB->SetBtnPosition(COUPON_WINDOW_OK_X,COUPON_WINDOW_OK_Y-12);
	m_pImgCancelB->SetBtnPosition(COUPON_WINDOW_CANCEL_X,COUPON_WINDOW_CANCEL_Y-12);

}
void CINFCouponWindow::Render()
{
	if(!m_bShow)
	{
		return;
	}

	m_pImgBackGround->Move(m_ptWindow.x,m_ptWindow.y);
	m_pImgBackGround->Render();
	m_pFont->DrawText(COUPON_WINDOW_NUMBER_X,COUPON_WINDOW_NUMBER_Y,GUI_FONT_COLOR_W,m_strCouponNumber);

	m_pImgOkB->Render();
	m_pImgCancelB->Render();
}
void CINFCouponWindow::Tick()
{
	if(!m_bShow)
	{
		return;
	}

}
void CINFCouponWindow::SetShow(BOOL i_bShow)
{
	m_bShow = i_bShow;
	g_pD3dApp->m_bChatMode = m_bShow;
	g_pD3dApp->CleanText();
	InitCouponNumber();
	
}
int CINFCouponWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bShow)
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pImgOkB->OnLButtonUp(pt))
			{
				SendCouponNumber();
				SetShow(FALSE);
				return INF_MSGPROC_BREAK;
			}
			if(m_pImgCancelB->OnLButtonUp(pt))
			{
				SetShow(FALSE);
				return INF_MSGPROC_BREAK;
			}
			if(pt.x > COUPON_WINDOW_CLOSE_X &&
				pt.y > m_ptWindow.y &&
				pt.x < (COUPON_WINDOW_CLOSE_X + COUPON_WINDOW_CLOSE_W) &&
				pt.y < (m_ptWindow.y + COUPON_WINDOW_CLOSE_H))
			{
				SetShow(FALSE);
				return INF_MSGPROC_BREAK;
			}
			if(pt.x > m_ptWindow.x &&
				pt.y > m_ptWindow.y &&
				pt.x < m_ptWindow.x + COUPON_WINDOW_W &&
				pt.y < m_ptWindow.y + COUPON_WINDOW_H)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_pImgOkB->OnLButtonDown(pt);
			m_pImgCancelB->OnLButtonDown(pt);
			if(pt.x > m_ptWindow.x &&
				pt.y > m_ptWindow.y &&
				pt.x < m_ptWindow.x + COUPON_WINDOW_W &&
				pt.y < m_ptWindow.y + COUPON_WINDOW_H)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_pImgOkB->OnMouseMove(pt);
			m_pImgCancelB->OnMouseMove(pt);
		}
		break;
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{
			if(INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
			{
			}
			INT nTextSize = 0;
			InitCouponNumber();
			nTextSize = strlen(g_pD3dApp->m_inputkey.m_full_str);
			if(nTextSize > 0 &&
				nTextSize < MAX_COUPON_NUMBER_SIZE)
			{				
				SetString(g_pD3dApp->m_inputkey.m_full_str,nTextSize);
			}	
			if(nTextSize >= MAX_COUPON_NUMBER_SIZE)
			{
				keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
			}			
			return INF_MSGPROC_BREAK;
		}
		break;

	}
	
	return INF_MSGPROC_NORMAL;
}

