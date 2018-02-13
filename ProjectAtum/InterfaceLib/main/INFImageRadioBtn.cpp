// INFImageRadioBtn.cpp: implementation of the CINFImageRadioBtn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "Interface.h"

#include "INFImageRadioBtn.h"
#include "INFImageEx.h"							   // 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageRadioBtn::CINFImageRadioBtn()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt <MAX_RADIO_STATUS;nCnt++)
	{
		m_pImageRadio[nCnt] = NULL;
	}			
	
	m_fPosX = m_fPosY = 0;
	m_fWidth = m_fHeight = 1;

	m_nRadioStatus = RADIO_STATUS_OFF;
	
	m_bShowWnd = TRUE; // 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
}

CINFImageRadioBtn::~CINFImageRadioBtn()
{
	CINFImageRadioBtn::DeleteDeviceObjects();
}

HRESULT CINFImageRadioBtn::InitDeviceObjects(char* pRadioup, char* pRadioSel)
{
	DataHeader	* pDataHeader = NULL;
	
	// Off
	{
		m_pImageRadio[RADIO_STATUS_OFF] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(pRadioup);
		m_pImageRadio[RADIO_STATUS_OFF]->InitDeviceObjects(pDataHeader);		
	}

	// On
	{
		m_pImageRadio[RADIO_STATUS_ON] = new CINFImageEx;
		pDataHeader = g_pGameMain->FindResource(pRadioSel);
		m_pImageRadio[RADIO_STATUS_ON]->InitDeviceObjects(pDataHeader);		
	}	

	return S_OK;
}
HRESULT CINFImageRadioBtn::InitDeviceObjects_LoadSelect(char* pRadioup, char* pRadioSel)
{
	DataHeader	* pDataHeader = NULL;
	
	// Off
	{
		m_pImageRadio[RADIO_STATUS_OFF] = new CINFImageEx;
		pDataHeader = g_pInterface->FindResource_LoadSelect(pRadioup);
		m_pImageRadio[RADIO_STATUS_OFF]->InitDeviceObjects(pDataHeader);		
	}

	// On
	{
		m_pImageRadio[RADIO_STATUS_ON] = new CINFImageEx; 
		pDataHeader = g_pInterface->FindResource_LoadSelect(pRadioSel);
		m_pImageRadio[RADIO_STATUS_ON]->InitDeviceObjects(pDataHeader);		
	}	

	return S_OK;
}

HRESULT CINFImageRadioBtn::RestoreDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt <MAX_RADIO_STATUS;nCnt++)
	{
		m_pImageRadio[nCnt]->RestoreDeviceObjects();
	}			
	return S_OK;
}

HRESULT CINFImageRadioBtn::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt <MAX_RADIO_STATUS;nCnt++)
	{
		if(m_pImageRadio[nCnt])
		{
			m_pImageRadio[nCnt]->DeleteDeviceObjects();
			util::del(m_pImageRadio[nCnt]);
		}
	}			
	
	return S_OK;
}

HRESULT CINFImageRadioBtn::InvalidateDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt <MAX_RADIO_STATUS;nCnt++)
	{
		m_pImageRadio[nCnt]->InvalidateDeviceObjects();
	}			
	return S_OK;
}

void CINFImageRadioBtn::Render()
{
	// 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
	if(!m_bShowWnd)
	{
		return;
	}
	// END 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
	
	// 버튼 
	m_pImageRadio[m_nRadioStatus]->Move(m_fPosX, m_fPosY);
	m_pImageRadio[m_nRadioStatus]->Render();		
}

void CINFImageRadioBtn::SetPosition(float i_fPosX, float i_fPosY, float i_fStrWidth)
{
	POINT ptRadio;
	ptRadio = m_pImageRadio[RADIO_STATUS_OFF]->GetImgSize();	
	m_fPosX		= i_fPosX;
	m_fPosY		= i_fPosY;		
	
	m_fHeight	= ptRadio.y;
	m_fWidth	= i_fStrWidth;		// 버튼과 글씨와의 간격 	
}

void CINFImageRadioBtn::SetRadioBtn(BOOL i_bSelRadio)
{
	if(i_bSelRadio)
	{
		m_nRadioStatus = RADIO_STATUS_ON;
	}
	else
	{
		m_nRadioStatus = RADIO_STATUS_OFF;
	}
}
int CINFImageRadioBtn::GetRadioBtn()		// 라디오 버튼 상태 
{
	return m_nRadioStatus;
}

BOOL CINFImageRadioBtn::OnLButtonDown(POINT pt)
{
	// 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
	if(!m_bShowWnd)
	{
		return FALSE;
	}
	// END 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행

	if((m_fPosX <= pt.x)
		&&(pt.x <= (m_fPosX+m_fWidth)))
	{		
		if((m_fPosY <= pt.y)
			&&(pt.y <= (m_fPosY+m_fHeight)))
		{
			return TRUE;
		}
	}
	return FALSE;
}
// 2012-10-17 by bhsohn 아이템 미리보기 무조건 ON으로 진행
void CINFImageRadioBtn::ShowWindow(BOOL	bShowWnd)
{
	m_bShowWnd = bShowWnd;
}