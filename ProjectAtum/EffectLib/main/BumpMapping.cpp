// BumpMapping.cpp: implementation of the CBumpMapping class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AtumApplication.h"
#include "DXUtil.h"
#include "d3dutil.h"
#include "dxerr9.h"
#include "BumpMapping.h"



//-------------------------------------------------------------
// 정점선언
//-------------------------------------------------------------
D3DVERTEXELEMENT9 decl[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,	0},
	{0, 24, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0},
	{0, 36, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBumpMapping::CBumpMapping()
{
	m_pDecl						= NULL;
	m_pNormalMap				= NULL;

	m_pEffect					= NULL;
	m_hTechnique  				= NULL;
	m_hmWVP  					= NULL;
	m_hvLightDir  				= NULL;
	m_hvColor	  				= NULL;
	m_hvEyePos					= NULL;
	m_htDecaleTex  				= NULL;
	m_htNormalMap				= NULL;
}

CBumpMapping::~CBumpMapping()
{

}



HRESULT CBumpMapping::InitDeviceObjects()
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9	pHeightTexture;
    D3DSURFACE_DESC desc;

	// 법선맵 생성
    D3DUtil_CreateTexture( g_pD3dDev,// 높이맵 읽기
		_T("height_test.bmp"), &pHeightTexture );
    pHeightTexture->GetLevelDesc(0,&desc);// 텍스처 정보 얻기
    D3DXCreateTexture(g_pD3dDev, desc.Width, desc.Height, 0, 0, 
        D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pNormalMap);// 텍스처 생성
    D3DXComputeNormalMap(m_pNormalMap,	// 법선맵 생성
        pHeightTexture, NULL, 0, D3DX_CHANNEL_RED, 1.0f);
    SAFE_RELEASE( pHeightTexture );		// 필요없어진 리소스 해제

	// 정점선언 오브젝트 생성
	if( FAILED( hr = g_pD3dDev->CreateVertexDeclaration(
										decl, &m_pDecl )))
		return DXTRACE_ERR ("CreateVertexDeclaration", hr);
	
	
	// 셰이더 읽기
    LPD3DXBUFFER pErr=NULL;
    if( FAILED( hr = D3DXCreateEffectFromFile(
                g_pD3dDev, "hlsl.fx", NULL, NULL, 
                0 , NULL, &m_pEffect, &pErr ))){
        // 셰이더 읽기 실패
        MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer()
                    , "ERROR", MB_OK);
    }else{
		m_hTechnique = m_pEffect->GetTechniqueByName( "TShader" );
		m_hmWVP      = m_pEffect->GetParameterByName( NULL, "mWVP" );
		m_hvLightDir = m_pEffect->GetParameterByName( NULL, "vLightDir" );
		m_hvColor    = m_pEffect->GetParameterByName( NULL, "vColor" );
		m_hvEyePos   = m_pEffect->GetParameterByName( NULL, "vEyePos" );
		m_htDecaleTex= m_pEffect->GetParameterByName( NULL, "DecaleTex" );
		m_htNormalMap= m_pEffect->GetParameterByName( NULL, "NormalMap" );
    }
    SAFE_RELEASE(pErr);

    return S_OK;
}

HRESULT CBumpMapping::RestoreDeviceObjects()
{
	// 이펙트
	if(m_pEffect) m_pEffect->OnResetDevice();

	return S_OK;
}


HRESULT CBumpMapping::Render(LPDIRECT3DTEXTURE9 m_pTexture,int m_nTriangleNumber)
{
	if( m_pEffect != NULL ) 
	{
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,        TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );

		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

		//-------------------------------------------------
		// 셰이더 설정
		//-------------------------------------------------
		m_pEffect->SetTechnique( m_hTechnique );
		m_pEffect->Begin( NULL, 0 );
		m_pEffect->Pass( 0 );
		

		//-------------------------------------------------
		// 주전자 렌더
		//-------------------------------------------------
		D3DXMATRIX m,matWorld,matView,matProj;
		g_pD3dDev->GetTransform(D3DTS_WORLD		, &matWorld);
		g_pD3dDev->GetTransform(D3DTS_VIEW		, &matView);
		g_pD3dDev->GetTransform(D3DTS_PROJECTION, &matProj);
		
		// 로컬-투영 변환행렬
		m = matWorld * matView * matProj;
		m_pEffect->SetMatrix( m_hmWVP, &m );

		// 광원의 방향(로컬좌표계)
		D3DXVECTOR4 light_pos = D3DXVECTOR4( -0.577f, -0.577f, -0.577f,0);
		D3DXVECTOR4 v;
		D3DXMatrixInverse( &m, NULL, &matWorld);
		D3DXVec4Transform( &v, &light_pos, &m );
		D3DXVec3Normalize( (D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v );
		v.w = -1.0f;		// 환경광 강도
		m_pEffect->SetVector( m_hvLightDir, &v );
		
		// 시점(로컬좌표계)
		m = matWorld * matView;
		D3DXMatrixInverse( &m, NULL, &m);
		v = D3DXVECTOR4( 0, 0, 0, 1);
		D3DXVec4Transform( &v, &v, &m );
		m_pEffect->SetVector( m_hvEyePos, &v );

		// 법선맵
		m_pEffect->SetTexture( m_htNormalMap, m_pNormalMap );
		// 정점선언
		g_pD3dDev->SetVertexDeclaration( m_pDecl );

		v.x = 0.5f;
		v.y = 0.5f;
		v.z = 0.5f;

		m_pEffect->SetVector( m_hvColor, &v );
		
		m_pEffect->SetTexture( m_htDecaleTex, m_pTexture );
		
			g_pD3dDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0,
				0, 
				((64 + 1) * (64 + 1)), 
				0, 
				m_nTriangleNumber );

		m_pEffect->End();
	}
    return S_OK;
}


HRESULT CBumpMapping::InvalidateDeviceObjects()
{
	// 셰이더
    if( m_pEffect != NULL ) m_pEffect->OnLostDevice();

    return S_OK;
}




//-------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: InitDeviceObjects() 에서 생성한 오브젝트 해제
//-------------------------------------------------------------
HRESULT CBumpMapping::DeleteDeviceObjects()
{
	SAFE_RELEASE( m_pEffect );      // 셰이더
	SAFE_RELEASE( m_pDecl );		// 정점선언
	SAFE_RELEASE( m_pNormalMap );
	
    return S_OK;
}