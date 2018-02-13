// FxSystem.cpp: implementation of the CFxSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "FxSystem.h"
#include "KeyBoardInput.h"
#include "Camera.h"
#include "TutorialSystem.h"
#include "D3DUtil.h"
#include "DXUtil.h"


#define GAUSS_FILTER_TEX_SIZE	256
#define BLUR_FILTER_TEX_SIZE	512

// 단축매크로
#define RS   g_pD3dDev->SetRenderState
#define TSS  g_pD3dDev->SetTextureStageState
#define SAMP g_pD3dDev->SetSamplerState

// 텍스처 출력용
struct TEXVERTEX
{
	enum { FVF=D3DFVF_XYZRHW|D3DFVF_TEX1 };
	float px, py, pz, pw;
	float tx, ty;
};
TEXVERTEX g_vtx[4] =
{
	{					  0-0.5, GAUSS_FILTER_TEX_SIZE-0.5, 0, 1, 0, 1 },
	{				  	  0-0.5,					 0-0.5, 0, 1, 0, 0 },
	{ GAUSS_FILTER_TEX_SIZE-0.5, GAUSS_FILTER_TEX_SIZE-0.5, 0, 1, 1, 1 },
	{ GAUSS_FILTER_TEX_SIZE-0.5,					 0-0.5, 0, 1, 1, 0 }
};

//-----------------------------------------------------------------------------
// Name: g_szEffect
// Desc: String containing effect used to render shiny teapot.
//-----------------------------------------------------------------------------

const char g_szEffect[] = 

    "texture texSphereMap;\n"
    "matrix matWorld;\n"
    "matrix matViewProject;\n"
    "vector vecPosition;\n"

    "technique Sphere\n"
    "{\n"
        "pass P0\n"
        "{\n"

            // Vertex state
            "VertexShader =\n"
                "decl\n"
                "{\n"
                    // Decls no longer associated with vertex shaders in DX9
                "}\n"
                "asm\n"
                "{\n"
                    "vs.1.1\n"
                    "def c64, 0.25f, 0.5f, 1.0f, -1.0f\n"
        
                    "dcl_position v0\n"
                    "dcl_normal v1\n"
        
                    // r0: camera-space position
                    // r1: camera-space normal
                    // r2: camera-space vertex-eye vector
                    // r3: camera-space reflection vector
                    // r4: texture coordinates

                    // Transform position and normal into camera-space
                    "m4x4 r0, v0, c0\n"
                    "m3x3 r1.xyz, v1, c0\n"
                    "mov r1.w, c64.z\n"

                    // Compute normalized view vector
                    "add r2, c8, -r0\n"
                    "dp3 r3, r2, r2\n"
                    "rsq r3, r3.w\n"
                    "mul r2, r2, r3\n"

                    // Compute camera-space reflection vector
                    "dp3 r3, r1, r2\n"
                    "mul r1, r1, r3\n"
                    "add r1, r1, r1\n"
                    "add r3, r1, -r2\n"

                    // Compute sphere-map texture coords
                    "mad r4.w, -r3.z, c64.y, c64.y\n"
                    "rsq r4, r4.w\n"
                    "mul r4, r3, r4\n"
                    "mad r4, r4, c64.x, c64.y\n"

                    // Project position
                    "m4x4 oPos, r0, c4\n"
                    "mul oT0.xy, r4.xy, c64.zw\n"
                    "mov oT0.zw, c64.z\n"
                "};\n"

            "VertexShaderConstant4[0] = <matWorld>;\n"
            "VertexShaderConstant4[4] = <matViewProject>;\n"
            "VertexShaderConstant1[8] = <vecPosition>;\n"

            // Pixel state
            "Texture[0] = <texSphereMap>;\n"
            "AddressU[0] = Wrap;\n"
            "AddressV[0] = Wrap;\n"
            "MinFilter[0] = Linear;\n"
            "MagFilter[0] = Linear;\n"
            "ColorOp[0] = SelectArg1;\n"
            "ColorArg1[0] = Texture;\n"
        "}\n"
    "}\n";

const UINT g_cchEffect = sizeof(g_szEffect) - 1;
//-----------------------------------------------------------------------------
// Name: struct ENVMAPPEDVERTEX
// Desc: D3D vertex type for environment-mapped objects
//-----------------------------------------------------------------------------
struct ENVMAPPEDVERTEX
{
    D3DXVECTOR3 p; // Position
    D3DXVECTOR3 n; // Normal

    static const DWORD FVF;
};
const DWORD ENVMAPPEDVERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFxSystem::CFxSystem()
{
	m_fEffectTime				= 0.0f;
	// 일반 블러 필터
//	m_sTargetViewPort.X			= 0;
//	m_sTargetViewPort.Y			= 0;
//	m_sTargetViewPort.Width		= BLUR_FILTER_TEX_SIZE;
//	m_sTargetViewPort.Height	= BLUR_FILTER_TEX_SIZE;
//	m_sTargetViewPort.MinZ		= 0.0f;
//	m_sTargetViewPort.MaxZ		= 1.0f;

//	m_pRealBackBuffer			= NULL;
//	m_pRealZBuffer				= NULL;
	m_pTargetTexture[0]			= NULL;
	m_pTargetTexture[1]			= NULL;
	m_pTargetSurface[0]			= NULL;
	m_pTargetSurface[1]			= NULL;
//	m_pTargetZBuffer[0]			= NULL;
//	m_pTargetZBuffer[1]			= NULL;

	m_byAlpha					= 0;
	m_bSupportBlurFilter		= TRUE;


	
	// 가우스 블러 필터
	m_dwGaussBlurNum			= 0;
	m_bUseGaussBlur				= TRUE;
	m_pGaussFilterTexture[0]	= NULL;
	m_pGaussFilterTexture[1]	= NULL;
	m_pGaussFilterSurface[0]	= NULL;
	m_pGaussFilterSurface[1]	= NULL;

	// 셰이더
	m_pEffect					= NULL;
	m_hTechnique  				= NULL;
	m_hafWeight					= NULL;
	m_htSrcMap					= NULL;
	m_dwShaderVersion			= 0;

	// 셰이더 (메탈)
	m_pEffectMetal				= NULL;
	m_hTechniqueMetal			= NULL;
	m_hmWVP						= NULL;
	m_hmW						= NULL;
	m_hvLightDir				= NULL;
	m_hvColor					= NULL;
	m_hvEyePos					= NULL;
	m_bSupportMetal				= FALSE;
	
	// 셰이더 (환경)
	m_bSupportEvn				= FALSE;
	m_pEffectEvn				= NULL;
	m_pSphereMap				= NULL;
	
	// 기타 블러 함수
	m_bSupportGaussBlur			= TRUE;
	m_pframeCur					= NULL;

// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	//모래바람 블러
	m_bStormShader				= FALSE;	
	m_bMagmaShader				= FALSE;	
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
}

CFxSystem::~CFxSystem()
{
	// 일반 블러 필터
	SAFE_RELEASE(m_pTargetTexture[0]);
	SAFE_RELEASE(m_pTargetTexture[1]);
	SAFE_RELEASE(m_pTargetSurface[0]);
	SAFE_RELEASE(m_pTargetSurface[1]);

	// 가우스 블러 필터
	SAFE_RELEASE(m_pGaussFilterTexture[0]);
	SAFE_RELEASE(m_pGaussFilterTexture[1]);
	SAFE_RELEASE(m_pGaussFilterTexture[0]);
	SAFE_RELEASE(m_pGaussFilterTexture[1]);
	
}


void CFxSystem::InitDeviceObjects()
{
	return; // DevX

	// 일반 블러 필터
	D3DCAPS9 d3dCaps;
	g_pD3dDev->GetDeviceCaps(&d3dCaps);
	if(	d3dCaps.StretchRectFilterCaps | D3DPTFILTERCAPS_MINFLINEAR &&
		d3dCaps.StretchRectFilterCaps | D3DPTFILTERCAPS_MAGFLINEAR &&
		d3dCaps.DevCaps2 | D3DDEVCAPS2_CAN_STRETCHRECT_FROM_TEXTURES)
	{
		m_bSupportBlurFilter = TRUE;
	}
	else
	{
		m_bSupportBlurFilter = FALSE;
	}
	
	
	if( m_bSupportBlurFilter == TRUE &&
		d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1,1) &&
		d3dCaps.PixelShaderVersion >= D3DPS_VERSION(2,0) )
	{
		m_dwShaderVersion = 2;
		m_bSupportGaussBlur = TRUE;
	}
	else if( m_bSupportBlurFilter == TRUE &&
		d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1,1) &&
		d3dCaps.PixelShaderVersion >= D3DPS_VERSION(1,1) )
	{
		m_dwShaderVersion = 1;
		m_bSupportGaussBlur = TRUE;
	}
	else
	{
		m_dwShaderVersion = 0;
		m_bSupportGaussBlur = FALSE;
	}
	
// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
#ifdef C_EPSODE4_SETTING_JHAHN
	{
		HRESULT hr;
		
		// 텍스처 읽기
		D3DXCreateTextureFromFile(g_pD3dDev, "Res-Eff/circle.bmp", &m_pTex);
		D3DXCreateTextureFromFile(g_pD3dDev, "Res-Eff/Dummy.bmp", &m_pDummyTex);
		// 셰이더 읽기
		LPD3DXBUFFER pErr;
		FILE *fp;
		fp = fopen("Res-Eff/hlsl.fx","rb");
		fseek( fp, 0L, SEEK_END );
		long size = ftell( fp );
		fseek( fp, 0L, SEEK_SET );
		char* pSrc = new char[size];
		fread( pSrc, size, sizeof( char ), fp );
		fclose( fp );
		
		
		if( FAILED(hr = D3DXCreateEffect(g_pD3dDev,pSrc,size,NULL,NULL,0,NULL,&m_pEffect2,&pErr)))
		
//		if( FAILED( hr = D3DXCreateEffectFromFile(g_pD3dDev, "Res-Eff/hlsl.fx2", NULL, NULL, D3DXSHADER_DEBUG , NULL, &m_pEffect2, &pErr )))
		{
		
			MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer() , "ERROR", MB_OK);
		
			 
		}
		m_hTechnique2 = m_pEffect2->GetTechniqueByName( "TShader" );
		m_htSrcTex2   = m_pEffect2->GetParameterByName( NULL, "SrcTex" );
		m_htBlurTex2  = m_pEffect2->GetParameterByName( NULL, "BlurTex" );
		m_htBlendTex2 = m_pEffect2->GetParameterByName( NULL, "BlendTex" );
	//	m_htDummyTex2 = m_pEffect2->GetParameterByName( NULL, "DummyTex" );

	}
#endif
	//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	if(m_bSupportBlurFilter == TRUE && m_bSupportGaussBlur == TRUE)
	{
		// 셰이더읽기
		if( FAILED( CreateEffectFromCompiledFile(g_pD3dDev, "Res-Eff/gb.obj",m_pEffect) ) )
		{
			m_dwShaderVersion = 0;
			m_bSupportGaussBlur = FALSE;
			return;
		}
		m_hTechnique = m_pEffect->GetTechniqueByName( "TShader" );
		m_hafWeight = m_pEffect->GetParameterByName( NULL, "weight" );
		m_htSrcMap  = m_pEffect->GetParameterByName( NULL, "SrcMap" );
		m_pEffect->SetFloat("MAP_WIDTH",  GAUSS_FILTER_TEX_SIZE);	// 폭   설정
		m_pEffect->SetFloat("MAP_HEIGHT", GAUSS_FILTER_TEX_SIZE);	// 높이 설정

		float fDiv;
		float m_tbl[8];
		
		if(m_dwShaderVersion == 2)
		{
			fDiv = 4.0f;
			m_tbl[0] = 0.1f/fDiv;
			m_tbl[1] = 0.358f/fDiv;
			m_tbl[2] = 0.773f/fDiv;
			m_tbl[3] = 1.0f/fDiv;
			m_tbl[4] = 1.0f/fDiv;
			m_tbl[5] = 0.773f/fDiv;
			m_tbl[6] = 0.358f/fDiv;
			m_tbl[7] = 0.1f/fDiv;
		}
		else
		{
			fDiv = 2.0f;
			m_tbl[0] = 0.358f/fDiv;
			m_tbl[1] = 0.773f/fDiv;
			m_tbl[2] = 0.773f/fDiv;
			m_tbl[3] = 0.358f/fDiv;
			m_tbl[4] = 0.0f;
			m_tbl[5] = 0.0f;
			m_tbl[6] = 0.0f;
			m_tbl[7] = 0.0f;
		}

		if(m_pEffect) m_pEffect->SetFloatArray(m_hafWeight
											, m_tbl, 8);	
	}

	// 메탈 쉐이더
	if(m_bSupportMetal == TRUE)
	{
		LPD3DXBUFFER pErr;
		if( FAILED( D3DXCreateEffectFromFile(
			g_pD3dDev, "Res-Test/hlsl.fx", NULL, NULL, 
			D3DXSHADER_DEBUG , NULL, &m_pEffectMetal, &pErr ))){
			MessageBox( NULL, (LPCTSTR)pErr->GetBufferPointer()
				, "ERROR", MB_OK);
			return;
		}
		else{
			m_hTechniqueMetal	= m_pEffectMetal->GetTechniqueByName( "TShader" );
			m_hmWVP				= m_pEffectMetal->GetParameterByName( NULL, "mWVP" );
			m_hmW				= m_pEffectMetal->GetParameterByName( NULL, "mW" );
			m_hvLightDir		= m_pEffectMetal->GetParameterByName( NULL, "vLightDir" );
			m_hvColor			= m_pEffectMetal->GetParameterByName( NULL, "vColor" );
			m_hvEyePos			= m_pEffectMetal->GetParameterByName( NULL, "vEyePos" );
		}
		SAFE_RELEASE(pErr);
	}

	// 쉐이더 환경
	if(m_bSupportEvn == TRUE)
	{
		DWORD pError;
		if( FAILED( D3DUtil_CreateTexture( g_pD3dDev, _T("Res-Test/spheremap.bmp"), &m_pSphereMap ) ) )
		{
			// 쉐이더 에러발생
			pError = D3DAPPERR_MEDIANOTFOUND;	
		}

		if( FAILED( D3DXCreateEffect( g_pD3dDev, g_szEffect, g_cchEffect, NULL, NULL, 0, NULL, &m_pEffectEvn, NULL ) ) )
			pError = E_FAIL;
	}
}

void CFxSystem::RestoreDeviceObjects()
{
	return; // DevX

	D3DPRESENT_PARAMETERS d3dpp = g_pD3dApp->GetPresentParameters();

// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	if (FAILED(g_pD3dDev->CreateDepthStencilSurface(g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
		D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pMapZ2, NULL)))
		return ;
	if (FAILED(g_pD3dDev->CreateTexture(g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 1, 
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pOriginalTex2, NULL)))
		return ;
	if (FAILED(m_pOriginalTex2->GetSurfaceLevel(0, &m_pOriginalSurf2)))
		return ;
	// 뭉개기위한 것들
	for(int i=0;i<2;i++)
	{
		if (FAILED(g_pD3dDev->CreateTexture(SMALL_WIDTH, SMALL_HEIGHT, 1,D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,
			&m_pPostTex2[i], NULL)))
			return ;
		
		if (FAILED(m_pPostTex2[i]->GetSurfaceLevel(0, &m_pPostSurf2[i])))
			return ;
	}
#ifdef C_EPSODE4_SETTING_JHAHN
	m_pEffect2->OnResetDevice();
#endif

	// 렌더링 상태설정
    g_pD3dDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
    g_pD3dDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    g_pD3dDev->SetRenderState( D3DRS_ZENABLE,        TRUE );
    g_pD3dDev->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );
    
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
    g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
    g_pD3dDev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	if(m_bSupportBlurFilter)
	{
		// 일반 블러 필터
//		g_pD3dDev->CreateDepthStencilSurface(BLUR_FILTER_TEX_SIZE,BLUR_FILTER_TEX_SIZE,d3dpp.AutoDepthStencilFormat,D3DMULTISAMPLE_NONE,0,TRUE,&m_pTargetZBuffer[0],NULL);
		g_pD3dDev->CreateTexture(BLUR_FILTER_TEX_SIZE,BLUR_FILTER_TEX_SIZE,1,D3DUSAGE_RENDERTARGET,d3dpp.BackBufferFormat,D3DPOOL_DEFAULT,&m_pTargetTexture[0],NULL);
		m_pTargetTexture[0]->GetSurfaceLevel(0,&m_pTargetSurface[0]);

//		g_pD3dDev->CreateDepthStencilSurface(BLUR_FILTER_TEX_SIZE,BLUR_FILTER_TEX_SIZE,d3dpp.AutoDepthStencilFormat,D3DMULTISAMPLE_NONE,0,TRUE,&m_pTargetZBuffer[1],NULL);
		g_pD3dDev->CreateTexture(BLUR_FILTER_TEX_SIZE,BLUR_FILTER_TEX_SIZE,1,D3DUSAGE_RENDERTARGET,d3dpp.BackBufferFormat,D3DPOOL_DEFAULT,&m_pTargetTexture[1],NULL);
		m_pTargetTexture[1]->GetSurfaceLevel(0,&m_pTargetSurface[1]);

		if(m_bSupportGaussBlur)
		{
			// 가우스 블러 필터
			g_pD3dDev->CreateTexture(
				GAUSS_FILTER_TEX_SIZE,GAUSS_FILTER_TEX_SIZE,1,D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&m_pGaussFilterTexture[0],NULL);

			m_pGaussFilterTexture[0]->GetSurfaceLevel(0,&m_pGaussFilterSurface[0]);

			g_pD3dDev->CreateTexture(
				GAUSS_FILTER_TEX_SIZE,GAUSS_FILTER_TEX_SIZE,1,D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&m_pGaussFilterTexture[1],NULL);
			
			m_pGaussFilterTexture[1]->GetSurfaceLevel(0,&m_pGaussFilterSurface[1]);
			m_pEffect->OnResetDevice();
		}
	}

	// 메탈
	if(m_pEffectMetal)
	{
		m_pEffectMetal->OnResetDevice();

		// 렌더링 상태설정
		RS( D3DRS_ZENABLE,        TRUE );
		RS( D3DRS_LIGHTING, FALSE );
    
		SAMP( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		SAMP( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		SAMP( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		SAMP( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	}

	// 쉐이더 환경
	if(m_bSupportEvn == TRUE)
	{
		m_pEffectEvn->OnResetDevice();
		m_pEffectEvn->SetTexture( "texSphereMap", m_pSphereMap );
	}
}

void CFxSystem::InvalidateDeviceObjects()
{
	return; // DevX

// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	 if( m_pEffect2 != NULL ) m_pEffect2->OnLostDevice();
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
	

	 SAFE_RELEASE(m_pTex);
	 SAFE_RELEASE(m_pDummyTex);
	 SAFE_RELEASE(m_pPostTex2[0]);	// 텍스처
	 SAFE_RELEASE(m_pPostTex2[1]);	// 텍스처
	 SAFE_RELEASE(m_pOriginalTex2);	// 텍스처
	 SAFE_RELEASE(m_pMapZ2);		// 깊이버퍼	 
	 SAFE_RELEASE(m_pOriginalSurf2);// 표면
	 SAFE_RELEASE(m_pPostSurf2[0]);	// 표면
	 SAFE_RELEASE(m_pPostSurf2[1]);	// 표면

	if(m_bSupportBlurFilter)
	{
		// 일반 블러 필터
//		SAFE_RELEASE(m_pTargetZBuffer[0]);
//		SAFE_RELEASE(m_pTargetZBuffer[1]);
		SAFE_RELEASE(m_pTargetTexture[0]);
		SAFE_RELEASE(m_pTargetTexture[1]);
		SAFE_RELEASE(m_pTargetSurface[0]);
		SAFE_RELEASE(m_pTargetSurface[1]);
		
//		SAFE_RELEASE(m_pRealBackBuffer);
//		SAFE_RELEASE(m_pRealZBuffer);
		
		if(m_bSupportGaussBlur)
		{
			// 셰이더
			if( m_pEffect != NULL ) m_pEffect->OnLostDevice();

			// 가우스 블러 필터
			SAFE_RELEASE(m_pGaussFilterTexture[0]);
			SAFE_RELEASE(m_pGaussFilterTexture[1]);
			SAFE_RELEASE(m_pGaussFilterSurface[0]);
			SAFE_RELEASE(m_pGaussFilterSurface[1]);
		}
	}

	// 셰이더
    if( m_pEffectMetal != NULL ) m_pEffectMetal->OnLostDevice();
	// 쉐이더 환경
	if( m_pEffectEvn != NULL ) m_pEffectEvn->OnLostDevice();
}

void CFxSystem::DeleteDeviceObjects()
{
	return; // DevX

	if(m_bSupportGaussBlur)
	{
		// 셰이더
		SAFE_RELEASE( m_pEffect );
		SAFE_RELEASE( m_pEffectMetal );		
		SAFE_RELEASE( m_pEffectEvn );
		
// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가		
		SAFE_RELEASE( m_pEffect );
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
//		SAFE_RELEASE( m_pPSPower );
	}
}

void CFxSystem::BeginTargetSurface()
{
//	g_pD3dDev->GetRenderTarget(0,&m_pRealBackBuffer);
//	g_pD3dDev->GetDepthStencilSurface(&m_pRealZBuffer);
//	g_pD3dDev->GetViewport(&m_sRealViewPort);
//
//	g_pD3dDev->SetRenderTarget(0,m_pTargetSurface[0]);
//	g_pD3dDev->SetDepthStencilSurface(m_pTargetZBuffer[0]);
//	g_pD3dDev->SetViewport(&m_sTargetViewPort);
}

void CFxSystem::EndTargetSurface()
{
//	g_pD3dDev->SetRenderTarget(0,m_pRealBackBuffer);
//	g_pD3dDev->SetDepthStencilSurface(m_pRealZBuffer);
//	g_pD3dDev->SetViewport(&m_sRealViewPort);
//	SAFE_RELEASE(m_pRealBackBuffer);
//	SAFE_RELEASE(m_pRealZBuffer);
}

void CFxSystem::MakeTargetSurface()
{
	if(m_bSupportBlurFilter)
	{
		DWORD dwTargetSurfaceNum;
		dwTargetSurfaceNum = 0;
		LPDIRECT3DSURFACE9 pBackBuffer;
		g_pD3dDev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
		HRESULT hResult;
		hResult = g_pD3dDev->StretchRect(pBackBuffer,NULL,m_pTargetSurface[dwTargetSurfaceNum],NULL,D3DTEXF_LINEAR);
		pBackBuffer->Release();
	}
}

void CFxSystem::DrawTargetSurface()
{
	if(m_bSupportBlurFilter)
	{
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
		
		typedef struct {FLOAT p[4]; D3DCOLOR color; FLOAT t[2];} TVERTEX;
		DWORD dwBackBufferWidth = g_pD3dApp->GetBackBufferDesc().Width;
		DWORD dwBackBufferHeight = g_pD3dApp->GetBackBufferDesc().Height;

		m_byAlpha = 255 - g_pSOption->sContrast * 28;

		// 2005-04-18 by jschoi - Tutorial
		if(	g_pTutorial->IsTutorialMode() == TRUE && 
			g_pTutorial->IsRenderInterface() == FALSE)
		{
			m_byAlpha = 0;
		}

		TVERTEX Vertex1[4] = {
			{0,0,0.1f,1.0f,D3DCOLOR_RGBA(m_byAlpha,m_byAlpha,m_byAlpha,m_byAlpha),0,0,},
			{dwBackBufferWidth,0,0.1f,1.0f,D3DCOLOR_RGBA(m_byAlpha,m_byAlpha,m_byAlpha,m_byAlpha),1,0,},
			{dwBackBufferWidth,dwBackBufferHeight,0.1f,1.0f,D3DCOLOR_RGBA(m_byAlpha,m_byAlpha,m_byAlpha,m_byAlpha),1,1,},
			{0,dwBackBufferHeight,0.1f,1.0f,D3DCOLOR_RGBA(m_byAlpha,m_byAlpha,m_byAlpha,m_byAlpha),0,1,},
		};
		
		g_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SUBTRACT);
		g_pD3dDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		g_pD3dDev->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
		
		g_pD3dDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		
		DWORD dwTargetSurfaceNum;

		dwTargetSurfaceNum = 0;

		g_pD3dDev->SetTexture(0,m_pTargetTexture[dwTargetSurfaceNum]);
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		g_pD3dDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,Vertex1,sizeof(TVERTEX));
	}
}

void CFxSystem::Tick(float fElapsedTime)
{
	// 2005-01-11 by jschoi - 테스트용 코드
//#ifdef _DEBUG
//	if(m_bSupportBlurFilter)
//	{
//		if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_ADD) && m_byAlpha <= 251)
//		{
//			m_byAlpha += 4;
//		}
//		if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SUBTRACT) && m_byAlpha >= 4)
//		{
//			m_byAlpha -= 4;
//		}
//	}
//
//
//
////	if(m_bSupportGaussBlur)
////	{
////		if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_ADD) && m_dwGaussBlurNum < 1)
////		{
////			m_dwGaussBlurNum++;
////		}
////		if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_SUBTRACT) && m_dwGaussBlurNum > 0 )
////		{
////			m_dwGaussBlurNum--;
////		}
////	}
//	if(m_bSupportBlurFilter == TRUE && m_bSupportGaussBlur == TRUE)
//	{
//		if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_LEFT))
//		{
//			m_bUseGaussBlur = TRUE;
//		}
//		if(g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_RIGHT))
//		{
//			m_bUseGaussBlur = FALSE;
//		}
//	}
//
//#endif
}
// 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
void CFxSystem::MakeSandStromSurface()
{


	LPDIRECT3DSURFACE9 pOldBackBuffer2, pOldZBuffer2;
	D3DVIEWPORT9 oldViewport2;
	
	g_pD3dDev->GetRenderTarget(0, &pOldBackBuffer2);
	g_pD3dDev->GetDepthStencilSurface(&pOldZBuffer2);
	g_pD3dDev->GetViewport(&oldViewport2);

	g_pD3dDev->SetRenderTarget(0, m_pOriginalSurf2);
	g_pD3dDev->SetDepthStencilSurface(m_pMapZ2);

	LPDIRECT3DSURFACE9 pBackBuffer;
	g_pD3dDev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
	HRESULT hResult;
	hResult = g_pD3dDev->StretchRect(pBackBuffer,NULL,m_pOriginalSurf2,NULL,D3DTEXF_LINEAR);
	pBackBuffer->Release();

	// 뷰포트 변경
	D3DVIEWPORT9 viewport = {0,0, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 0.0f,1.0f}; 
	g_pD3dDev->SetViewport(&viewport);
	
	// 렌더링타겟 클리어
//	g_pD3dDev->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0L);

	{
		m_pEffect2->SetTechnique( m_hTechnique2 );
		m_pEffect2->Begin( NULL, 0 );

		
		m_pEffect2->BeginPass(0); // D3D DEVX
		
		//-------------------------------------------------
		// 축소버퍼 복사
		//-------------------------------------------------
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE, FALSE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );
		
		g_pD3dDev->SetFVF( D3DFVF_XYZ | D3DFVF_TEX4 );
		
		float u0 = 0 + 0.5f/SMALL_WIDTH;
		float u1 = 1 + 0.5f/SMALL_WIDTH;
		float v0 = 0 + 0.5f/SMALL_HEIGHT;
		float v1 = 1 + 0.5f/SMALL_HEIGHT;
		float dw = 0.25f/SMALL_WIDTH;
		float dh = 0.25f/SMALL_HEIGHT;
		T4VERTEX VertexSmall[4] = {
			// x      y     z    w   u0    v0    u1    v1    u2    v2    u3    v3
			{-1.0f, +1.0f, 0.1f, u0-dw,v0-dh,u0+dw,v0-dh,u0-dw,v0+dh,u0+dw,v0+dh,},
			{+1.0f, +1.0f, 0.1f, u1-dw,v0-dh,u1+dw,v0-dh,u1-dw,v0+dh,u1+dw,v0+dh,},
			{+1.0f, -1.0f, 0.1f, u1-dw,v1-dh,u1+dw,v1-dh,u1-dw,v1+dh,u1+dw,v1+dh,},
			{-1.0f, -1.0f, 0.1f, u0-dw,v1-dh,u0+dw,v1-dh,u0-dw,v1+dh,u0+dw,v1+dh,},
		};
		g_pD3dDev->SetRenderTarget(0, m_pPostSurf2[0]);
		m_pEffect2->SetTexture(m_htSrcTex2, m_pOriginalTex2);
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, VertexSmall, sizeof( T4VERTEX ) );
		//-------------------------------------------------
		// 뭉개자!
		//-------------------------------------------------
		dw = 1.0f/SMALL_WIDTH;
		dh = 1.0f/SMALL_HEIGHT;
		T4VERTEX Vertex4[4] = {
			// x      y     z     u0 v0  u1   v1 u2  v2    u3    v3
			{-1.0f, +1.0f, 0.1f,  0, 0, 0+dw, 0, 0, 0+dh, 0+dw, 0+dh,},
			{+1.0f, +1.0f, 0.1f,  1, 0, 1+dw, 0, 1, 0+dh, 1+dw, 0+dh,},
			{+1.0f, -1.0f, 0.1f,  1, 1, 1+dw, 1, 1, 1+dh, 1+dw, 1+dh,},
			{-1.0f, -1.0f, 0.1f,  0, 1, 0+dw, 1, 0, 1+dh, 0+dw, 1+dh,},
		};
		g_pD3dDev->SetRenderTarget(0, m_pPostSurf2[1]);
		m_pEffect2->SetTexture(m_htSrcTex2, m_pPostTex2[0]);
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex4, sizeof( T4VERTEX ) );
		
		g_pD3dDev->SetRenderTarget(0, m_pPostSurf2[0]);
		m_pEffect2->SetTexture(m_htSrcTex2, m_pPostTex2[1]);
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex4, sizeof( T4VERTEX ) );
		
		g_pD3dDev->SetRenderTarget(0, m_pPostSurf2[1]);
		m_pEffect2->SetTexture(m_htSrcTex2, m_pPostTex2[0]);
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex4, sizeof( T4VERTEX ) );
		
		m_pEffect2->EndPass(); // D3D DEVX

		//-----------------------------------------------------
		// 렌더링타겟 복구
		//-----------------------------------------------------
		g_pD3dDev->SetRenderTarget(0, pOldBackBuffer2);
		g_pD3dDev->SetDepthStencilSurface(pOldZBuffer2);
		g_pD3dDev->SetViewport(&oldViewport2);
		pOldBackBuffer2->Release();
		pOldZBuffer2->Release();
		
		// 긫긞긲?궻긏깏귺
//		g_pD3dDev->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00404080, 1.0f, 0L );
		
		//-----------------------------------------------------
		// 블러한 것을 붙인다
		//-----------------------------------------------------
		FLOAT w = (FLOAT)oldViewport2.Width;
		FLOAT h = (FLOAT)oldViewport2.Height;
		T3VERTEX Vertex3[4] = {
			//   x    y   z    rhw  u0 v0 u1 v1 u2 v2
			{ 0.0f,   0, 0.1f, 1.0f, 0, 0, 0, 0, 0, 0, },
			{    w,   0, 0.1f, 1.0f, 1, 0, 1, 0, 1, 0, },
			{    w,   h, 0.1f, 1.0f, 1, 1, 1, 1, 1, 1, },
			{ 0.0f,   h, 0.1f, 1.0f, 0, 1, 0, 1, 0, 1, },
		};
 		m_pEffect2->BeginPass(1); // D3D DEVX
 		m_pEffect2->SetTexture( m_htSrcTex2,   m_pOriginalTex2 );
 		m_pEffect2->SetTexture( m_htBlurTex2,  m_pPostTex2[1] );
 		m_pEffect2->SetTexture( m_htBlendTex2, m_pTex );
		m_pEffect2->SetTexture( m_htDummyTex2, m_pDummyTex );
		
 		g_pD3dDev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX4 );
 		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex3, sizeof( T3VERTEX ) );
		m_pEffect2->EndPass(); // D3D DEVX
		m_pEffect2->End();
		
	}
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

}
//end 2011-10-10 by jhahn EP4 트리거 시스템	화산재 이펙트 추가
void CFxSystem::MakeGaussFilterSurface()
{
	if(m_bSupportGaussBlur == TRUE && m_bUseGaussBlur == TRUE)
	{

		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		
		LPDIRECT3DSURFACE9	p = NULL;
		g_pD3dDev->GetRenderTarget( 0, &p );



		// STEP 1 : m_pGaussFilterSurface[1] 에 현재 이미지를 가져온다.
		LPDIRECT3DSURFACE9 pBackBuffer;
		g_pD3dDev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
		HRESULT hResult;
		hResult = g_pD3dDev->StretchRect(pBackBuffer,NULL,m_pGaussFilterSurface[0],NULL,D3DTEXF_LINEAR);

		pBackBuffer->Release();


		// STEP 2 : m_pGaussFilterSurface[1] 에 m_pGaussFilterSurface[0] 을 이용하여 X방향 블러 이미지를 만든다.
		//-------------------------------------------------
		// 셰이더 설정
		//-------------------------------------------------
		m_pEffect->SetTechnique( m_hTechnique ); // 2016-01-08 exception at this point, wtf this is not used
		m_pEffect->Begin( NULL, 0 );

		g_pD3dDev->SetRenderTarget(0, m_pGaussFilterSurface[1]);
		g_pD3dDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
		if(m_dwShaderVersion == 2)
		{
			m_pEffect->BeginPass( 2 ); // D3D DEVX
		}
		else
		{
			m_pEffect->BeginPass( 0 ); // D3D DEVX
		}

		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

		typedef struct {FLOAT p[3];FLOAT tu, tv;} VERTEX;

		VERTEX Vertex1[4] = {
			//   x      y     z      tu tv
			{{  1.0f, -1.0f, 0.1f},   1, 1,},
			{{ -1.0f, -1.0f, 0.1f},   0, 1,},
			{{ -1.0f,  1.0f, 0.1f},   0, 0,},
			{{  1.0f,  1.0f, 0.1f},   1, 0,},
		};
		g_pD3dDev->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );
		m_pEffect->SetTexture(m_htSrcMap, m_pGaussFilterTexture[0]);
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
						, 2, Vertex1, sizeof( VERTEX ) );
		
		m_pEffect->EndPass(); // D3D DEVX

		// STEP 3 : m_pGaussFilterSurface[0] 에 m_pGaussFilterSurface[1] 을 이용하여 Y방향 블러 이미지를 만든다.
		g_pD3dDev->SetRenderTarget(0, m_pGaussFilterSurface[0]);
		g_pD3dDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

		if(m_dwShaderVersion == 2)
		{
			m_pEffect->BeginPass( 3 ); // D3D DEVX
		}
		else
		{
			m_pEffect->BeginPass( 1 ); // D3D DEVX
		}

		m_pEffect->SetTexture(m_htSrcMap, m_pGaussFilterTexture[1]);
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLEFAN
						, 2, Vertex1, sizeof( VERTEX ) );

		m_pEffect->EndPass(); // D3D DEVX
		m_pEffect->End();

		

		// RESULT : m_pGaussFilterSurface[0] 에 현재 이미지에서 밝은 부분만 X,Y 방향으로 블러링한 이미지 생성
		// STEP 2, 과 STEP 3 를 반복할수록 블러가 더 강하게 발생


		g_pD3dDev->SetRenderTarget(0, p);
		SAFE_RELEASE(p);
	}
}

void CFxSystem::DrawGaussFilterSurface()
{
	if(m_bSupportGaussBlur == TRUE && m_bUseGaussBlur == TRUE)
	{
		DWORD dwWidth = g_pD3dApp->GetBackBufferDesc().Width;
		DWORD dwHeight = g_pD3dApp->GetBackBufferDesc().Height;

		TEXVERTEX vtx[4] =
		{
			{        -0.5f, dwHeight-0.5f, 0.1f, 1.0f, 0.0f, 1.0f },
			{        -0.5f,         -0.5f, 0.1f, 1.0f, 0.0f, 0.0f },
			{ dwWidth-0.5f, dwHeight-0.5f, 0.1f, 1.0f, 1.0f, 1.0f },
			{ dwWidth-0.5f,         -0.5f, 0.1f, 1.0f, 1.0f, 0.0f }
		};
		
		BYTE byFilterDegree = (BYTE)(g_pSOption->sFilter * 16);

		// 2005-04-18 by jschoi - Tutorial
		if(	g_pTutorial->IsTutorialMode() == TRUE && 
			g_pTutorial->IsRenderInterface() == FALSE &&
			byFilterDegree < 160)
		{
			byFilterDegree = 160;
		}

		g_pD3dDev->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(byFilterDegree, 0, 0, 0) ); 
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );


		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );


		g_pD3dDev->SetTexture( 0, m_pGaussFilterTexture[0] );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );


		g_pD3dDev->SetFVF( TEXVERTEX::FVF );
		g_pD3dDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(TEXVERTEX) );
		g_pD3dDev->SetTexture( 0, NULL );
		g_pD3dDev->SetTexture( 1, NULL );

		
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	}
}



//------------------------------------------------------------------------------
// Name:	CreatePSFromBinFile
// Desc:	loads a binary *.pso file 
//			and creates a pixel shader
//------------------------------------------------------------------------------
HRESULT CFxSystem::CreatePSFromCompiledFile (LPDIRECT3DDEVICE9 pd3dDevice,TCHAR* strPSPath,LPDIRECT3DPIXELSHADER9& pPS)
{
	char szBuffer[128];		// debug output
	DWORD*	pdwPS;			// pointer to address space of the calling process
	HANDLE hFile, hMap;		// handle file and handle mapped file
    TCHAR tchTempVSPath[512];	// temporary file path
	HRESULT hr;				// error 

    if( FAILED( hr = DXUtil_FindMediaFileCb( tchTempVSPath,sizeof(tchTempVSPath), strPSPath ) ) )
        return D3DAPPERR_MEDIANOTFOUND;
	
	hFile = CreateFile(tchTempVSPath, GENERIC_READ,0,0,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,0);

	if(hFile != INVALID_HANDLE_VALUE) 
	{
		if(GetFileSize(hFile,0) > 0) 
			hMap = CreateFileMapping(hFile,0,PAGE_READONLY,0,0,0);
		else
		{
			CloseHandle(hFile);
			return E_FAIL;		
		}
	}	
	else
		return E_FAIL;	
	
	// maps a view of a file into the address space of the calling process
	pdwPS = (DWORD *)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
		
	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pdwPS, &pPS);
	if ( FAILED(hr) )
	{
		OutputDebugString( "Failed to create Pixel Shader, errors:\n" );
		OutputDebugString( szBuffer );
		OutputDebugString( "\n" );
	  return hr;
	}
	
	UnmapViewOfFile(pdwPS);
	CloseHandle(hMap);
	CloseHandle(hFile);
	
  return S_OK;
}

HRESULT CFxSystem::CreateEffectFromCompiledFile(
						LPDIRECT3DDEVICE9 pd3dDevice, 
						TCHAR* strFilePath,
						LPD3DXEFFECT& pEffect )
{
	// 셰이더 읽기
	HRESULT hr;
	TCHAR strTotalPath[512];
    if( FAILED( hr = DXUtil_FindMediaFileCb( strTotalPath,sizeof(strTotalPath), strFilePath ) ) )
        return D3DAPPERR_MEDIANOTFOUND;

	FILE *fp;
	if( NULL == (fp = fopen(strTotalPath,"rb")) ) return -1;
	fseek( fp, 0L, SEEK_END );
	long size = ftell( fp );
	fseek( fp, 0L, SEEK_SET );
	char* pSrc = new char[size];
	fread( pSrc, size, sizeof( char ), fp );
	fclose( fp );
	
	LPD3DXBUFFER pErr = NULL;
	if( FAILED(hr = D3DXCreateEffect(pd3dDevice,pSrc,size,NULL,NULL,0,NULL,&pEffect,&pErr)))
	{
		// 셰이더 읽기 실패
		MessageBox(NULL,(LPCTSTR)pErr->GetBufferPointer(),"Error",MB_OK);
	}

	SAFE_RELEASE(pErr);
	delete[] pSrc;

	return hr;
}

void CFxSystem::DrawMetalFilterBegin(D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProj, UINT ipattr, DWORD nType)
{
	if(nType != _SHUTTLE)
	{
		m_pframeCur->pMesh->DrawSubset( ipattr );
		return;
	}

	D3DXMATRIX m, mT, mR;
	D3DXVECTOR4 v, light_pos, eye_pos;

	m_pEffectMetal->SetTechnique( m_hTechnique );
	m_pEffectMetal->Begin( NULL, 0 );
	m_pEffectMetal->BeginPass( 0 ); // D3D DEVX
	g_pD3dDev->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL );
	
	//-------------------------------------------------
	// 비행모델 렌더
	//-------------------------------------------------
	TSS( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	TSS( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	RS( D3DRS_FOGENABLE, FALSE);
	if( m_pEffectMetal != NULL ) 
	{
		// 로컬-월드 변환행렬
		m_pEffectMetal->SetMatrix( m_hmW, &mWorld );
		// 로컬-투영 변환행렬
		m = mWorld * mView * mProj;
		m_pEffectMetal->SetMatrix( m_hmWVP, &m );
		
		// 광원방향
		light_pos = D3DXVECTOR4( -0.577f, -0.577f, -0.577f,0);
		D3DXMatrixInverse( &m, NULL, &mWorld);
		D3DXVec4Transform( &v, &light_pos, &m );
		D3DXVec3Normalize( (D3DXVECTOR3 *)&v, (D3DXVECTOR3 *)&v );
		v.w = -0.3f;// 환경광의 강도
		m_pEffectMetal->SetVector( m_hvLightDir, &v );
		
		// 시점
		m = mWorld * mView;
		D3DXMatrixInverse( &m, NULL, &m);
		v = D3DXVECTOR4( g_pCamera->m_vCamCurrentPos.x, g_pCamera->m_vCamCurrentPos.y, g_pCamera->m_vCamCurrentPos.z, 0);
		D3DXVec4Transform( &v, &v, &m );
		m_pEffectMetal->SetVector( m_hvEyePos, &v );
		
		D3DMATERIAL9 *pMtrl = m_pframeCur->rgMaterials;
		
		for( int i=0; i<m_pframeCur->cMaterials; i++ ) {
			v.x = 0.486f * pMtrl->Diffuse.r / 1.3f;
			v.y = 0.433f * pMtrl->Diffuse.g / 1.3f;
			v.z = 0.185f * pMtrl->Diffuse.b / 1.3f;
			
			m_pEffectMetal->SetVector( g_pD3dApp->m_pFxSystem->m_hvColor, &v );			
			m_pframeCur->pMesh->DrawSubset( ipattr );
			
			pMtrl++;
		}
		
		//////////////////////////////////////////////////////////////////////////		
		TSS( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		TSS( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	}

	m_pEffectMetal->EndPass(); // D3D DEVX
	m_pEffectMetal->End(); // D3D DEVX
}

void CFxSystem::DrawEvnFilterBegin(D3DXMATRIX mWorld, D3DXMATRIX mView, D3DXMATRIX mProj, UINT ipattr, DWORD nType)
{
	if(nType != _SHUTTLE)
	{
		m_pframeCur->pMesh->DrawSubset( ipattr );
		return;
	}
	
	// Set transform state
	D3DXMATRIXA16 matViewProject;
	D3DXMatrixMultiply( &matViewProject, &mView, &mProj );
	
	D3DXMATRIXA16 matViewInv;
	D3DXMatrixInverse( &matViewInv, NULL, &mView );
	D3DXVECTOR4 vecPosition( matViewInv._41, matViewInv._42, matViewInv._43, 1.0f );
	
	m_pEffectEvn->SetMatrix( "matWorld", &mWorld );
	m_pEffectEvn->SetMatrix( "matViewProject", &matViewProject );
	m_pEffectEvn->SetVector( "vecPosition", &vecPosition );	
	
	RS( D3DRS_FOGENABLE, FALSE);
	
	UINT uPasses;
	m_pEffectEvn->Begin( &uPasses, 0 );
	
	for( UINT iPass = 0; iPass < uPasses; iPass++ )
	{
		m_pEffectEvn->BeginPass( iPass );	// D3D DEVX	
		// 렌더링 오브젝트

		m_pframeCur->pMesh->DrawSubset( ipattr );

		m_pEffectEvn->EndPass();	// D3D DEVX	
	}
	
	m_pEffectEvn->End();
}
