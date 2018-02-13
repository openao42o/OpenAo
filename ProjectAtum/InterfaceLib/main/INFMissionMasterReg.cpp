// CINFMissionMasterReg.cpp: implementation of the CINFMissionMasterReg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameDataLast.h"
#include "INFMissionMasterReg.h"
#include "FieldWinSocket.h"
#include "INFImage.h"
#include "INFGameMain.h"
#include "INFCommunity.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"

#define MISSIONMASTER_WINDOW_BASE_POS_X				((g_pD3dApp->GetBackBufferDesc().Width - 517)/2)
#define MISSIONMASTER_WINDOW_BASE_POS_Y				((g_pD3dApp->GetBackBufferDesc().Height - 245)/2)

#define MISSIONMASTER_OK_BTN_POS_X					(m_ptWindow.x + 193)
#define MISSIONMASTER_OK_BTN_POS_Y					(m_ptWindow.y + 209)
#define MISSIONMASTER_CANCEL_BTN_POS_X				(m_ptWindow.x + 261)
#define MISSIONMASTER_CANCEL_BTN_POS_Y				(m_ptWindow.y + 209)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionMasterReg::CINFMissionMasterReg()
{
	m_pOK				= NULL;
	m_pCancel			= NULL;
	m_pImgBG			= NULL;
	m_ptWindow.x		= MISSIONMASTER_WINDOW_BASE_POS_X;
	m_ptWindow.y		= MISSIONMASTER_WINDOW_BASE_POS_Y;

	m_bShow = FALSE;
}

CINFMissionMasterReg::~CINFMissionMasterReg()
{

}
void CINFMissionMasterReg::SetWindowPos(POINT i_ptPos)
{
	m_ptWindow = i_ptPos;
}
void CINFMissionMasterReg::SetWindowShow(BOOL i_bShow)
{
	m_ptWindow.x		= MISSIONMASTER_WINDOW_BASE_POS_X;
	m_ptWindow.y		= MISSIONMASTER_WINDOW_BASE_POS_Y;
	m_bShow = i_bShow;
}
HRESULT CINFMissionMasterReg::InitDeviceObjects()
{
	DataHeader	* pDataHeader = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(NULL == m_pImgBG)
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("MMaster");
		m_pImgBG = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pImgBG->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}

	if(NULL == m_pOK)
	{
		m_pOK = new CINFImageBtn;
	}
	m_pOK->InitDeviceObjects("okb0");
	if(NULL == m_pCancel)
	{
		m_pCancel = new CINFImageBtn;
	}
	m_pCancel->InitDeviceObjects("canb0");
#else
	if(NULL == m_pImgBG)
	{
		m_pImgBG = new CINFImageEx;
	}
	pDataHeader = g_pGameMain->FindResource("MMregW");
	m_pImgBG->InitDeviceObjects(pDataHeader);

	if(NULL == m_pOK)
	{
		m_pOK = new CINFImageBtn;
	}
	m_pOK->InitDeviceObjects("Ook");
	if(NULL == m_pCancel)
	{
		m_pCancel = new CINFImageBtn;
	}
	m_pCancel->InitDeviceObjects("lr_can");
#endif

	return S_OK;
}
HRESULT CINFMissionMasterReg::RestoreDeviceObjects()
{
	m_pImgBG->RestoreDeviceObjects();
	m_pImgBG->Move(MISSIONMASTER_WINDOW_BASE_POS_X,MISSIONMASTER_WINDOW_BASE_POS_Y);
	m_pCancel->RestoreDeviceObjects();
	m_pCancel->SetBtnPosition(MISSIONMASTER_CANCEL_BTN_POS_X,MISSIONMASTER_CANCEL_BTN_POS_Y);
	m_pOK->RestoreDeviceObjects();
	m_pOK->SetBtnPosition(MISSIONMASTER_OK_BTN_POS_X,MISSIONMASTER_OK_BTN_POS_Y);
	
	return S_OK;
}
HRESULT CINFMissionMasterReg::DeleteDeviceObjects()
{
	m_pImgBG->DeleteDeviceObjects();
	m_pCancel->DeleteDeviceObjects();
	m_pOK->DeleteDeviceObjects();
	return S_OK;
}
HRESULT CINFMissionMasterReg::InvalidateDeviceObjects()
{
	m_pImgBG->InvalidateDeviceObjects();
	m_pCancel->InvalidateDeviceObjects();
	m_pOK->InvalidateDeviceObjects();
	return S_OK;
}
void CINFMissionMasterReg::Render()
{
	if(!m_bShow)
		return;
	m_pImgBG->Render();
	m_pOK->Render();
	m_pCancel->Render();

}
void CINFMissionMasterReg::Tick()
{
	if(!m_bShow)
		return;

}

int CINFMissionMasterReg::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bShow)
		return INF_MSGPROC_NORMAL;
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	switch(uMsg)
	{
	
	case WM_LBUTTONDOWN:
		{
			m_pOK->OnLButtonDown(pt);
			m_pCancel->OnLButtonDown(pt);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if(m_pOK->OnMouseMove(pt))
			{
				return INF_MSGPROC_NORMAL;
			}
			if(m_pCancel->OnMouseMove(pt))
			{
				return INF_MSGPROC_NORMAL;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if(m_pOK->OnLButtonUp(pt))
			{
				m_bShow = FALSE;
				g_pGameMain->m_pCommunity->SetMissionMasterOption(TRUE);
				return INF_MSGPROC_NORMAL;
			}
			if(m_pCancel->OnLButtonUp(pt))
			{
				m_bShow = FALSE;
				return INF_MSGPROC_NORMAL;
			}			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}