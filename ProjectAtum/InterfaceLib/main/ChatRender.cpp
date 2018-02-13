// ChatRender.cpp: implementation of the CChatRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatRender.h"
#include "AtumApplication.h"
#include "ChatData.h"
#include "D3DHanFont.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatRender::CChatRender()
{
	FLOG( "CChatRender()" );
	m_pFontInput = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,1024,32);

}

CChatRender::~CChatRender() 
{
	FLOG( "~CChatRender()" );
	util::del(m_pFontInput);
}

HRESULT CChatRender::InitDeviceObjects()
{
	FLOG( "CChatRender::InitDeviceObjects()" );
	m_pFontInput->InitDeviceObjects(g_pD3dDev);

	return TRUE;
}

HRESULT CChatRender::RestoreDeviceObjects()
{
	FLOG( "CChatRender::RestoreDeviceObjects()" );
	m_pFontInput->RestoreDeviceObjects();
	return TRUE; 
}

HRESULT CChatRender::InvalidateDeviceObjects()
{
	FLOG( "CChatRender::InvalidateDeviceObjects()" );
	m_pFontInput->InvalidateDeviceObjects();
	return TRUE;
}

HRESULT CChatRender::DeleteDeviceObjects()
{
	FLOG( "CChatRender::DeleteDeviceObjects()" );
	m_pFontInput->DeleteDeviceObjects();
	return TRUE;
}

void CChatRender::Tick(D3DXVECTOR2 vStartPos)
{ 
	FLOG( "CChatRender::Tick(D3DXVECTOR2 vStartPos)" );
	m_vRenderPos = vStartPos;
}

void CChatRender::Render(CChatData *pChatData)
{
	FLOG( "CChatRender::Render(CChatData *pChatData)" );
	m_pFontInput->DrawText(m_vRenderPos.x, m_vRenderPos.y, GUI_FONT_COLOR, pChatData->m_szString, 0L);
	m_vRenderPos.y -= 20.0f;
}
