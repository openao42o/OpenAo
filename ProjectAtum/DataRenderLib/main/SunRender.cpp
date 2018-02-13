// SunRender.cpp: implementation of the CSunRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunRender.h"
#include "SunData.h"
#include "D3DUtil.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "Camera.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSunRender::CSunRender()
{
	FLOG( "CSunRender()" );
	int i;
	m_pVBLens = NULL;
	for(i=0;i<5;i++)
		m_pTexture[i] = NULL;
}

CSunRender::~CSunRender()
{
	FLOG( "~CSunRender()" );

}

HRESULT CSunRender::InitDeviceObjects()
{
	FLOG( "CSunRender::InitDeviceObjects()" );
	HRESULT hr;
	char strPath[256];
	int cont = 0;
	CGameData	* pSun = new CGameData;
	wsprintf(strPath,".\\Res-Tex\\lens");
	if(pSun->SetFile(strPath,FALSE, NULL,0))
	{
		DataHeader* pSunHeader = pSun->GetStartPosition();
		cont = 0;
		while(pSunHeader)
		{
			if( FAILED( D3DXCreateTextureFromFileInMemory(g_pD3dDev, (LPCVOID)pSunHeader->m_pData, pSunHeader->m_DataSize, &(m_pTexture[cont])))) 
				hr = E_FAIL;
			cont++;
			pSunHeader = pSun->GetNext();
		}
		util::del( pSun);
	}
	else 
	{
		util::del( pSun);
		return E_FAIL;// error
	}

	return S_OK;
}

HRESULT CSunRender::RestoreDeviceObjects()
{
	FLOG( "CSunRender::RestoreDeviceObjects()" );
/*	g_pD3dDev->BeginStateBlock();
 	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pD3dDev->EndStateBlock( &m_dwStateBlock );
*/	float hsx,hsy;
	SPRITEVERTEX* v;
	// 버퍼설정 m_pVBLens
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITEVERTEX ),
		0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &m_pVBLens,NULL ) ) )
		return E_FAIL;
	m_pVBLens->Lock( 0, 0, (void**)&v, 0 );
	hsx = 1.0f;
	hsy = 1.0f;
	v[0].p = D3DXVECTOR3(-hsx,-hsy,0);	v[0].tu=0.0f; v[0].tv=1.0f;		v[0].color = 0x33333333;
	v[1].p = D3DXVECTOR3(-hsx,hsy,0);	v[1].tu=0.0f; v[1].tv=0.0f;		v[1].color = 0x33333333;
	v[2].p = D3DXVECTOR3(hsx,-hsy,0);	v[2].tu=1.0f; v[2].tv=1.0f;		v[2].color = 0x33333333;
	v[3].p = D3DXVECTOR3(hsx,hsy,0);	v[3].tu=1.0f; v[3].tv=0.0f;		v[3].color = 0x33333333;
	m_pVBLens->Unlock();
	DBGOUT("Lens flare Setting OK\n");
	return S_OK;
}

HRESULT CSunRender::InvalidateDeviceObjects()
{
	FLOG( "CSunRender::InvalidateDeviceObjects()" );
	if(m_dwStateBlock)
	{
//		g_pD3dDev->DeleteStateBlock(m_dwStateBlock);
		m_dwStateBlock = 0;
	}
	SAFE_RELEASE(m_pVBLens);
	return S_OK;
}

HRESULT CSunRender::DeleteDeviceObjects()
{
	FLOG( "CSunRender::DeleteDeviceObjects()" );
	for(int i=0;i<5;i++)
		SAFE_RELEASE(m_pTexture[i]);
	return S_OK;
}

void CSunRender::RenderRens(CAtumNode * pNode)
{
	FLOG( "CSunRender::RenderRens(CAtumNode * pNode)" );
	// 2006-02-18 by ispark, 렌즈 플레이어 버텍스 버퍼가 설정 되어 있지 않으면 셋팅
	if(m_pVBLens == NULL)
	{
		RestoreDeviceObjects();	
	}
	D3DMATERIAL9 materialOrg;
	g_pD3dDev->GetMaterial(&materialOrg);
	// 렌즈 플레어 그리기
	CSunData * pData = (CSunData *)pNode;
//	g_pD3dDev->ApplyStateBlock( m_dwStateBlock );
 	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );
	g_pD3dDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	D3DMATERIAL9 mtrl;
	D3DXMATRIX matScale, mBillboard;
	mBillboard = g_pD3dApp->m_pCamera->GetBillboardMatrix();
	D3DUtil_InitMaterial( mtrl, pData->m_fMaterialValue, pData->m_fMaterialValue, pData->m_fMaterialValue );
	g_pD3dDev->SetMaterial( &mtrl );
	for(int i=0;i<10;i++)
	{
		g_pD3dDev->SetTexture( 0, m_pTexture[pData->m_bTexture[i]]);
		D3DXMatrixScaling(&matScale,pData->m_fScale[i],pData->m_fScale[i],pData->m_fScale[i]);
		matScale = matScale*mBillboard;
		matScale._41 = pData->m_vLensPos[i].x;
		matScale._42 = pData->m_vLensPos[i].y;
		matScale._43 = pData->m_vLensPos[i].z;
		g_pD3dDev->SetTransform( D3DTS_WORLD, &matScale );
		g_pD3dDev->SetStreamSource( 0, m_pVBLens,0, sizeof(SPRITEVERTEX) );
		g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	}
	g_pD3dDev->SetMaterial( &materialOrg );
}

