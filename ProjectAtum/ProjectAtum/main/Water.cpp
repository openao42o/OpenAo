// CWater.cpp: WaterRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Water.h"
#include "AtumApplication.h"
#include "dxutil.h"

//-----------------------------------------------------------------------------
// Function prototypes and global (or static) variables
//-----------------------------------------------------------------------------
inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWater::CWater()
{
	FLOG( "CWater()" );
	
    m_psBumpMap          = NULL;
    m_pBackgroundTexture = NULL;
    D3DXMatrixIdentity( &m_matBumpMat );

	m_fTime = 0.0f;
	m_pFilterSurface = NULL;
	m_pFilterTexture = NULL;
}

CWater::~CWater()
{
	FLOG( "~CWater()" );
	
	SAFE_RELEASE(m_pFilterTexture);
}

HRESULT CWater::InitDeviceObjects()
{
	FLOG( "CWater::InitDeviceObjects(char* pData, int nSize)" );

	g_pD3dDev->GetDeviceCaps( &m_pd3dCaps );
    // Load the texture for the background image
//    if( FAILED( D3DUtil_CreateTexture( g_pD3dDev, _T("22.tga"),
//                                       &m_pBackgroundTexture, D3DFMT_R5G6B5 ) ) )
//        return D3DAPPERR_MEDIANOTFOUND;

    // Create the bumpmap.
    m_psBumpMap = CreateBumpMap( 1, 1, D3DFMT_V8U8 );
    if( NULL == m_psBumpMap )
        return E_FAIL;
	
	return S_OK;
}

HRESULT CWater::RestoreDeviceObjects()
{
	FLOG( "CWater::RestoreDeviceObjects()" );
	
//	D3DXVECTOR3 vEyePt = g_pD3dApp->m_pCamera->GetEyePt();
//	D3DXVECTOR3 vLookatPt = g_pD3dApp->m_pCamera->GetLookatPt();
//	D3DXVECTOR3 vUpVec = g_pD3dApp->m_pCamera->GetUpVec();

	D3DXVECTOR3 vEyePt(    1500.0f, 800.0f, -1650.0f );
    D3DXVECTOR3 vLookatPt( 0.0f,   0.0f,     0.0f );
    D3DXVECTOR3 vUpVec(    0.0f,   1.0f,     0.0f );

    D3DXMATRIXA16 matWorld, matView, matProj;

    D3DXMatrixIdentity( &matWorld );
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    D3DXMatrixPerspectiveFovLH( &matProj, 1.00f, 1.0f, 1.0f, 10000.0f );
    g_pD3dDev->SetTransform( D3DTS_WORLD,      &matWorld );
    g_pD3dDev->SetTransform( D3DTS_VIEW,       &matView );
    g_pD3dDev->SetTransform( D3DTS_PROJECTION, &matProj );

    // Set any appropiate state
    g_pD3dDev->SetRenderState( D3DRS_AMBIENT,        0xffffffff );
    g_pD3dDev->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
    g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, TRUE );

    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_pD3dDev->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pD3dDev->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	return S_OK;
}

HRESULT CWater::InvalidateDeviceObjects()
{
	FLOG( "CWater::InvalidateDeviceObjects()" );	
	return S_OK;
}

HRESULT CWater::DeleteDeviceObjects()
{
	FLOG( "CWater::DeleteDeviceObjects()" );
	
	SAFE_RELEASE( m_pBackgroundTexture );
    SAFE_RELEASE( m_psBumpMap );
	
	return S_OK;
}

void CWater::Tick()
{
	FLOG( "CWater::Tick()" );

	FLOAT fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
	FLOAT r = 0.04f;
    m_matBumpMat._11 =  r * cosf( fAppTime * 90.0f );
    m_matBumpMat._12 = -r * sinf( fAppTime * 90.0f );
    m_matBumpMat._21 =  r * sinf( fAppTime * 90.0f );
    m_matBumpMat._22 =  r * cosf( fAppTime * 90.0f );
}

HRESULT CWater::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior,
                                          D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat )
{
    // Device must be able to do bumpmapping
    if( 0 == ( pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE ) )
        return E_FAIL;

	LPDIRECT3D9 pd3d9 = NULL;
    if( SUCCEEDED( g_pD3dDev->GetDirect3D( &pd3d9 ) ) )
    {
		// Accept devices that can create D3DFMT_V8U8 textures
		if( SUCCEEDED( pd3d9->CheckDeviceFormat( pCaps->AdapterOrdinal,
			pCaps->DeviceType, adapterFormat,
			0, D3DRTYPE_TEXTURE,
			D3DFMT_V8U8 ) ) )
			return S_OK;
		SAFE_RELEASE( pd3d9 );
	}
    // Else, reject the device
    return E_FAIL;
}

LPDIRECT3DTEXTURE9 CWater::CreateBumpMap( DWORD dwWidth, DWORD dwHeight,
                                                     D3DFORMAT d3dBumpFormat )
{
    LPDIRECT3DTEXTURE9 psBumpMap;
	D3DPRESENT_PARAMETERS d3dpp = g_pD3dApp->GetPresentParameters();

    // Check if the device can create the format
    D3DFORMAT adapterFormat = d3dpp.BackBufferFormat;
	
	LPDIRECT3D9 pd3d9 = NULL;
    
	
    if( SUCCEEDED( g_pD3dDev->GetDirect3D( &pd3d9 ) ) )
    {
		if( FAILED( pd3d9->CheckDeviceFormat( m_pd3dCaps.AdapterOrdinal,
											   m_pd3dCaps.DeviceType,
											   adapterFormat,
											   0, D3DRTYPE_TEXTURE, d3dBumpFormat ) ) )
			return NULL;
		SAFE_RELEASE( pd3d9 );
	}

    // Create the bump map texture
    if( FAILED( g_pD3dDev->CreateTexture( dwWidth, dwHeight, 1, 0 /* Usage */,
                                             d3dBumpFormat, D3DPOOL_MANAGED,
                                             &psBumpMap, NULL ) ) )
        return NULL;

    // Lock the surface and write in some bumps for the waves
    D3DLOCKED_RECT d3dlr;
    psBumpMap->LockRect( 0, &d3dlr, 0, 0 );
    CHAR* pDst = (CHAR*)d3dlr.pBits;
    CHAR  iDu, iDv;

    for( DWORD y=0; y<dwHeight; y++ )
    {
        CHAR* pPixel = pDst;

        for( DWORD x=0; x<dwWidth; x++ )
        {
            FLOAT fx = x/(FLOAT)dwWidth - 0.5f;
            FLOAT fy = y/(FLOAT)dwHeight - 0.2f;

            FLOAT r = sqrtf( fx*fx + fy*fy );

            iDu  = (CHAR)( 64 * cosf( 300.0f * r ) * expf( -r * 5.0f ) );
            iDu += (CHAR)( 32 * cosf( 150.0f * ( fx + fy ) ) );
            iDu += (CHAR)( 16 * cosf( 140.0f * ( fx * 0.85f - fy ) ) );

            iDv  = (CHAR)( 64 * sinf( 300.0f * r ) * expf( -r * 5.0f ) );
            iDv += (CHAR)( 32 * sinf( 150.0f * ( fx + fy ) ) );
            iDv += (CHAR)( 16 * sinf( 140.0f * ( fx * 0.85f - fy ) ) );

            *pPixel++ = iDu;
            *pPixel++ = iDv;
        }
        pDst += d3dlr.Pitch*2;
    }
    psBumpMap->UnlockRect(0);

    return psBumpMap;
}

HRESULT CWater::SetEMBMStates(LPDIRECT3DTEXTURE9*      Texture, int i, int j)
{
    // Set up texture stage 0's states for the bumpmap
    g_pD3dDev->SetTexture( 0, m_psBumpMap );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_ADDRESSU,   D3DTADDRESS_CLAMP );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_ADDRESSV,   D3DTADDRESS_CLAMP );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT00,   F2DW( m_matBumpMat._11 ) );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT01,   F2DW( m_matBumpMat._12 ) );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT10,   F2DW( m_matBumpMat._21 ) );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_BUMPENVMAT11,   F2DW( m_matBumpMat._22 ) );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_BUMPENVLSCALE,  F2DW(0.8f) );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f) );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BUMPENVMAPLUMINANCE );

    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	
    g_pD3dDev->SetTexture( 1, *Texture );
    g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

//	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
//	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
//	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
//	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );	
	
//	D3DXMATRIXA16 mat;
//	mat._11 = 0.8f; mat._12 = 0.0f; mat._13 = 0.0f;
//	mat._21 = 0.0f; mat._22 = 0.8f; mat._23 = 0.0f;
//	mat._31 = 0.5f; mat._32 =-0.5f; mat._33 = 1.0f;
//	mat._41 = 0.0f; mat._42 = 0.0f; mat._43 = 0.0f;
//	
//	g_pD3dDev->SetTransform( D3DTS_TEXTURE1, &mat );
//	g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3|D3DTTFF_PROJECTED );
//	g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION | 1 );
	
	
    return S_OK;
}