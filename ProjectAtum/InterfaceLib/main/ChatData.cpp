// ChatData.cpp: implementation of the CChatData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatData.h"
#include "AtumApplication.h"
#include "D3DHanFont.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatData::CChatData( TCHAR *str,int width, int height,DWORD dwColor)
{
	FLOG( "CChatData( TCHAR *str,int width, int height,DWORD dwColor)" );
//	m_vPosition = D3DXVECTOR2(0.0f, 0.0f);
	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,width,height);
//	m_pRenderer = pRenderer;
	m_vRenderPos = D3DXVECTOR2(20.0f, 500.0f);
	m_fLifeTime = 10.0f;
	m_dwChatColor = dwColor;
	_tcscpy(m_szString, str);
}

CChatData::~CChatData()
{
	FLOG( "~CChatData()" );
	if(m_pFontInput)
	{
		m_pFontInput->InvalidateDeviceObjects();
		m_pFontInput->DeleteDeviceObjects();
		util::del(m_pFontInput);
	}
}

HRESULT CChatData::InitDeviceObjects()
{
	FLOG( "CChatData::InitDeviceObjects()" );
	m_pFontInput->InitDeviceObjects(g_pD3dDev);
	return S_OK;
}

HRESULT CChatData::RestoreDeviceObjects()
{
	FLOG( "CChatData::RestoreDeviceObjects()" );
	m_pFontInput->RestoreDeviceObjects();
	return S_OK; 
}

HRESULT CChatData::InvalidateDeviceObjects()
{
	FLOG( "CChatData::InvalidateDeviceObjects()" );
	if(m_pFontInput)
		m_pFontInput->InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CChatData::DeleteDeviceObjects()
{
	FLOG( "CChatData::DeleteDeviceObjects()" );
	if(m_pFontInput)
	{
		m_pFontInput->DeleteDeviceObjects();
		util::del(m_pFontInput);
	}
	return S_OK;
}

void CChatData::Tick()
{
	FLOG( "CChatData::Tick()" );
	m_fLifeTime-=g_pD3dApp->GetElapsedTime();
	if(m_fLifeTime <= 0.0f || !m_bUsing)
	{
		m_bUsing=FALSE;
	}
}

void CChatData::Render()
{
	FLOG( "CChatData::Render()" );
/*	int nTempX,nTempY;
	bool bCheck1,bCheck2;
	nTempX = nTempY = 0;
	bCheck1 = bCheck2 = false;
	if(g_pD3dApp->m_pGUIGame)
	{
		if(g_pD3dApp->m_pGUIGame->m_bUnitInfoDraw)
		{
			bCheck1 = true;
		}
		if(g_pD3dApp->m_pGUIGame->m_pLeftBottom->GetState())
		{
			bCheck2 = true;
		}
	}
	if(bCheck1 && bCheck2)
	{
		nTempX  = 210.0f;
	}
	else if(!bCheck1 && bCheck2)
	{
		nTempY = 110.0f;
	}

	if(FAILED(m_pFontInput->DrawText(m_vRenderPos.x + nTempX, m_vRenderPos.y - nTempY, m_dwChatColor,m_szString, 0L)))
		m_bUsing = FALSE;
*/
}
