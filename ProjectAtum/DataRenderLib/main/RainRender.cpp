// RainRender.cpp: implementation of the CRainRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RainRender.h"
#include "AtumApplication.h"
#include "RainData.h"
#include "dxutil.h"
#include "SceneData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define D3DFVF_RAINVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

CRainRender::CRainRender()
{
	FLOG( "CRainRender()" );
	m_pVB = NULL;
}

CRainRender::~CRainRender()
{
	FLOG( "~CRainRender()" );
}

HRESULT CRainRender::InitDeviceObjects()
{
	FLOG( "CRainRender::InitDeviceObjects()" );
	return S_OK;
}

HRESULT CRainRender::RestoreDeviceObjects()
{
	FLOG( "CRainRender::RestoreDeviceObjects()" );
	if( FAILED( g_pD3dDev->CreateVertexBuffer(2 * sizeof(RAINVERTEX),
		0, D3DFVF_RAINVERTEX, D3DPOOL_MANAGED, &m_pVB,NULL ) ) )
		return E_FAIL;
	RAINVERTEX* pVertices;
	if(FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, NULL)))
		return E_FAIL;
	pVertices[0].pos = D3DXVECTOR3(0,35,0);pVertices[0].color=0x00999999; 
	pVertices[1].pos = D3DXVECTOR3(0,0,0);pVertices[1].color=0x99BBBBFF;
	m_pVB->Unlock();
	return S_OK;
}

HRESULT CRainRender::InvalidateDeviceObjects()
{
	FLOG( "CRainRender::InvalidateDeviceObjects()" );
	SAFE_RELEASE(m_pVB);
	return S_OK;
}

HRESULT CRainRender::DeleteDeviceObjects()
{
	FLOG( "CRainRender::DeleteDeviceObjects()" );
	return S_OK;
}

void CRainRender::Render()
{
	FLOG( "CRainRender::Render()" );
	g_pD3dDev->SetTexture( 0, NULL );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   TRUE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  TRUE );
    g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
	g_pD3dDev->SetFVF( D3DFVF_RAINVERTEX );
//	g_pD3dDev->SetTransform( D3DTS_WORLD, &mat );

	CRainData * pData = (CRainData *)g_pScene->m_pRainList->m_pChild;
	while(pData)
	{
		
		g_pD3dDev->SetTransform( D3DTS_WORLD, &pData->m_mMatrix );
		g_pD3dDev->SetStreamSource( 0, m_pVB,0, sizeof(RAINVERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_LINELIST, 0, 1 );
		pData = (CRainData *)pData->m_pNext;
	}
//	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  FALSE );
//	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

}