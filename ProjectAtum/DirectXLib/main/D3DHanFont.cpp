// D3DHanFont.cpp: implementation of the CD3DHanFont class.
//
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
// 2005-01-03 by jschoi
//#include <D3DX8.h>
#include <d3dx9.h>
#include "D3DHanFont.h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "DbgOut_C.h"
#include "FunctionLog.h"
#include "FunctionLogGFunctions.h"
#include "d3dfont.h"

extern CD3DApplication* g_pApp;

#ifdef LANGUAGE_VIETNAM
#define LANGUAGE_CHARSET VIETNAMESE_CHARSET
#else
#define LANGUAGE_CHARSET DEFAULT_CHARSET
#endif
//-----------------------------------------------------------------------------
// Custom vertex types for rendering text
//-----------------------------------------------------------------------------

struct FONT2DVERTEX { D3DXVECTOR4 p;   DWORD color ;  FLOAT tu, tv; };
struct FONT3DVERTEX { D3DXVECTOR3 p;   D3DXVECTOR3 n;   FLOAT tu, tv; };


#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline FONT2DVERTEX InitFont2DVertex( const D3DXVECTOR4& p, D3DCOLOR color,
                                      FLOAT tu, FLOAT tv )
{
    FONT2DVERTEX v;   v.p = p;   v.color = color;   v.tu = tu;   v.tv = tv;
    return v;
}

inline FONT3DVERTEX InitFont3DVertex( const D3DXVECTOR3& p, const D3DXVECTOR3& n,
                                      FLOAT tu, FLOAT tv )
{
    FONT3DVERTEX v;   v.p = p;   v.n = n;   v.tu = tu;   v.tv = tv;
    return v;
}

// 2006-04-17 by ispark, 전역으로 뺀다
//#define RED_FONT		'r'
//#define GREEN_FONT		'g'
//#define BLUE_FONT		'b'
//#define YELLOW_FONT		'y'
//#define CYAN_FONT		'c'
//#define MAGENTA_FONT	'm'
//#define WHITE_FONT		'w'
//#define ENCHANT_FONT	'e'
//#define GRAY_FONT		'q'
//#define DARKBLUE_FONT	'a'
//
//#define HFONT_ARGB(a,r,g,b) \
//    ((D3DCOLOR)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))
//
//DWORD GetFontColor( char chr )
//{
//	switch( chr )
//	{
//	case RED_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)255,(BYTE)0,(BYTE)0));
//	case GREEN_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)0,(BYTE)255,(BYTE)0));
//	case BLUE_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)0,(BYTE)0,(BYTE)255));
//	case YELLOW_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)255,(BYTE)255,(BYTE)0));
//	case CYAN_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)0,(BYTE)255,(BYTE)255));
//	case MAGENTA_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)255,(BYTE)0,(BYTE)255));
//	case WHITE_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)255,(BYTE)255,(BYTE)255));
//	case ENCHANT_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)245,(BYTE)185,(BYTE)48));
//	case GRAY_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)208,(BYTE)208,(BYTE)208));
//	case DARKBLUE_FONT:
//		return (HFONT_ARGB(0x00,(BYTE)178,(BYTE)190,(BYTE)255));
//	default:
//		return (HFONT_ARGB(0x00,(BYTE)0,(BYTE)0,(BYTE)0));
//	}
//
//}
//-----------------------------------------------------------------------------
// Name: CD3DHanFont()
// Desc: Font class constructor
//-----------------------------------------------------------------------------
CD3DHanFont::CD3DHanFont(const TCHAR* strFontName, 
						 DWORD dwHeight,
						 DWORD dwFlags , 
						 BOOL outline ,
						 DWORD dwMaxWidth , 
						 DWORD dwMaxHeight,
						 BOOL bCullText, 
						 BOOL bCullUV)
{
	FLOG( "CD3DHanFont( TCHAR* strFontName, DWORD dwHeight,DWORD dwFlags , BOOL outline ,DWORD dwMaxWidth , DWORD dwMaxHeight,BOOL bCullText, BOOL bCullUV)" );
	memset(m_strText,0x00,sizeof(m_strText));   
	_tcscpy( m_strFontName, strFontName );
    m_dwFontHeight			= dwHeight;
    m_dwFontFlags			= dwFlags;	// 글씨옵션
	m_bOutLine				= outline;	// 글씨 윤곽선
	m_bCullText				= bCullText;
	m_bCullUV				= bCullUV;

	m_dwTexWidth			= dwMaxWidth;		// 텍스쳐 최대 넓이 : 2자승단위..
	m_dwTexHeight			= dwMaxHeight;		// 텍스쳐 최대 높이 : 2자승단위..

    m_pd3dDevice			= NULL;
    m_pTexture				= NULL;
    m_pVB					= NULL;

    m_dwSavedStateBlock		= 0L;
    m_dwDrawTextStateBlock	= 0L;

	m_fPosX = 0;
	m_fPosY = 0;
	m_dwColor = 0;
	m_fTx1 = 0.0f;
	m_fTy1 = 0.0f;
	m_fTx2 = 0.0f;
	m_fTy2 = 0.0f;
	m_fWidth= 0;
	m_bReset = FALSE;

	m_bReLoadString = FALSE;

	// 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
	memset(m_strSizeCheckText,0x00,sizeof(m_strSizeCheckText));   	
	m_szCheckSize.cx = m_szCheckSize.cy = 0;
	// end 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
}




//-----------------------------------------------------------------------------
// Name: ~CD3DHanFont()
// Desc: Font class destructor
//-----------------------------------------------------------------------------
CD3DHanFont::~CD3DHanFont()
{
	FLOG( "~CD3DHanFont()" );
    InvalidateDeviceObjects();
    DeleteDeviceObjects();
}

//-----------------------------------------------------------------------------
// Name: SetUV(float tx1, float ty1, float tx2, float ty2)
// Desc: UV 좌표 세팅 : 권동혁 추가
//-----------------------------------------------------------------------------

void CD3DHanFont::SetUV(float tx1, 
						float ty1, 
						float tx2, 
						float ty2)
{
	FLOG( "CD3DHanFont::SetUV(float tx1, float ty1, float tx2, float ty2)" );
	m_fTx1 = tx1;
	m_fTy1 = ty1;
	m_fTx2 = tx2;
	m_fTy2 = ty2;
/*
	FONT2DVERTEX* pVertices = NULL;
	m_pVB->Lock( 0, 0, (BYTE**)&pVertices, D3DLOCK_DISCARD );

	FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
	FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;

	pVertices[0].tu = m_fTx1;
	pVertices[0].tv = m_fTy2;
	pVertices[1].tu = m_fTx1;
	pVertices[1].tv = m_fTy1;
	pVertices[2].tu = m_fTx2;
	pVertices[2].tv = m_fTy2;
	pVertices[3].tu = m_fTx2;
	pVertices[3].tv = m_fTy1;
	pVertices[4].tu = m_fTx2;
	pVertices[4].tv = m_fTy2;
	pVertices[5].tu = m_fTx1;
	pVertices[5].tv = m_fTy1;

	m_pVB->Unlock();
*/
}


//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initializes device-dependent objects, including the vertex buffer used
//       for rendering text and the texture map which stores the font image.
//-----------------------------------------------------------------------------
HRESULT CD3DHanFont::InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )
{
	FLOG( "CD3DHanFont::InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice )" );

   // Keep a local copy of the device
    m_pd3dDevice = pd3dDevice;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CD3DHanFont::RestoreDeviceObjects()
{
	FLOG( "CD3DHanFont::RestoreDeviceObjects()" );
    HRESULT hr;

 
    // Establish the font and texture size
    m_fTextScale  = 1.0f; // Draw fonts into texture without scaling


    // If requested texture is too big, use a smaller texture and smaller font,
    // and scale up when rendering.
    D3DCAPS9 d3dCaps;
    m_pd3dDevice->GetDeviceCaps( &d3dCaps );

    if( m_dwTexWidth > d3dCaps.MaxTextureWidth )
    {
        m_fTextScale = (FLOAT)d3dCaps.MaxTextureWidth / (FLOAT)m_dwTexWidth;
        m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
    }

    // Create a new texture for the font
    SAFE_RELEASE( m_pTexture ); // 2016-01-08 exception at this point
	hr = m_pd3dDevice->CreateTexture( m_dwTexWidth, m_dwTexHeight, 1,
                                      0, D3DFMT_A4R4G4B4,
                                    D3DPOOL_MANAGED, &m_pTexture,NULL );
    if( FAILED(hr) )
        return hr;


    // Create vertex buffer for the letters
   SAFE_RELEASE( m_pVB );
   if( FAILED( hr = m_pd3dDevice->CreateVertexBuffer( /*MAX_NUM_VERTICES*/6*sizeof(FONT2DVERTEX),
                                                       D3DUSAGE_WRITEONLY, D3DFVF_FONT2DVERTEX,
                                                       D3DPOOL_MANAGED, &m_pVB,NULL ) ) )
    {
        return hr;
    }

    // Create the state blocks for rendering text
 //   for( UINT which=0; which<2; which++ )
//    {
/*        m_pd3dDevice->BeginStateBlock();
//        m_pd3dDevice->SetTexture( 0, m_pTexture );

        if ( D3DFONT_ZENABLE & m_dwFontFlags )
            m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
        else
            m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

        m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
        m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
        m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
        m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
        m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
        m_pd3dDevice->SetRenderState( D3DRS_EDGEANTIALIAS,    FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
        m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
        
		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
        m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
        m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

//        if( which==0 )
//            m_pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
//        else
            m_pd3dDevice->EndStateBlock( &m_dwDrawTextStateBlock );
//    }
*/
	if(strlen(m_strText))
	{
		m_bReset = TRUE;
		SetText(0, 0, m_strText, m_dwColor);
	}
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DHanFont::InvalidateDeviceObjects()
{
	FLOG( "CD3DHanFont::InvalidateDeviceObjects()" );
    SAFE_RELEASE( m_pVB );

    // Delete the state blocks
//	if( m_pd3dDevice )
//	{
//        if( m_dwSavedStateBlock )
//            m_pd3dDevice->DeleteStateBlock( m_dwSavedStateBlock );
//        if( m_dwDrawTextStateBlock )
//            m_pd3dDevice->DeleteStateBlock( m_dwDrawTextStateBlock );
//	}

    m_dwSavedStateBlock    = 0L;
    m_dwDrawTextStateBlock = 0L;
//	memset(m_strText,0x00,sizeof(m_strText));			// 2006-05-15 by ispark
    SAFE_RELEASE( m_pTexture ); // 2016-01-08 exception at this point, CMipMap::Release !!!

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CD3DHanFont::DeleteDeviceObjects()
{
	FLOG( "CD3DHanFont::DeleteDeviceObjects()" );
    m_pd3dDevice = NULL;
	memset(m_strText,0x00,sizeof(m_strText));   
	memset(m_strFontName,0x00,sizeof(m_strFontName));

	// 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
	memset(m_strSizeCheckText,0x00,sizeof(m_strSizeCheckText));   	
	m_szCheckSize.cx =  m_szCheckSize.cy = 0;		
	// end 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws 2D text
//-----------------------------------------------------------------------------
HRESULT CD3DHanFont::DrawText( FLOAT sx, 
							  FLOAT sy, 
							  DWORD dwColor,
							  TCHAR* strText, 
							  DWORD dwFlags,
							  RECT*	i_pFillRect/*=NULL*/)// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
{
    HRESULT hr;
	
	if (!m_pd3dDevice) return E_FAIL;

	if (!strText) return S_OK;

	if (*strText=='\0')  return S_OK;
	

	if(FAILED(SetText(sx,sy,strText, dwColor, i_pFillRect)))
	{
		DBGOUT("%s\n",strText);
		return E_FAIL;
	}

	// Setup renderstate
 //   m_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,         0x08 );
    m_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
    m_pd3dDevice->SetRenderState( D3DRS_FILLMODE,   D3DFILL_SOLID );
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE,   D3DCULL_CCW );
    m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_CLIPPING,         TRUE );
		// 2005-01-04 by jschoi
//       m_pd3dDevice->SetRenderState( D3DRS_EDGEANTIALIAS,    FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND,      FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,        FALSE );
    
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		// 2005-01-03 by jschoi
//        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
//        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
//        m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );

	// 2005-03-10 by jschoi
	if(dwFlags & D3DFONT_NOTFILTERED)
	{
		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);
		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
	}

    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
    m_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
//    m_pd3dDevice->ApplyStateBlock( m_dwDrawTextStateBlock );
	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,   FALSE );
	m_pd3dDevice->SetTexture( 0, m_pTexture );
	m_pd3dDevice->SetFVF( D3DFVF_FONT2DVERTEX );
	m_pd3dDevice->SetPixelShader( NULL );
	m_pd3dDevice->SetStreamSource( 0, m_pVB,0, sizeof(FONT2DVERTEX) );

	// 2005-03-10 by jschoi
	if(dwFlags & D3DFONT_NOTFILTERED)
	{
		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	}

	// Set filter states
//    if( dwFlags & D3DFONT_FILTERED )
//    {
		// 2005-01-03 by jschoi
//       m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//       m_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);

//    }

	// 2005-01-06 by jschoi - Set Filter States - 현재 사용 안함.
//	if( dwFlags & D3DFONT_FILTERED )
//	{
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
//	}
//	else
//	{
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_NONE);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_NONE);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
//	}

	
//	if( dwNumTriangles > 0 )
	m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ); // 2016-01-08 exception at this point, update dirty portion !!!


	// 2005-01-06 by jschoi
//	if( !(dwFlags & D3DFONT_FILTERED) )
//	{
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//		m_pd3dDevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
//	}

	D3DVERTEXBUFFER_DESC pDesc;
	if(m_pVB)
		hr=m_pVB->GetDesc(&pDesc);

    // Restore the modified renderstates
 //   m_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );

    return S_OK;
}



HRESULT CD3DHanFont::SetText(FLOAT sx, 
							 FLOAT sy,
							 TCHAR *texts, 
							 DWORD color, 
							 RECT*	i_pFillRect/*=NULL*/)// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
{
	FLOG( "CD3DHanFont::SetText( FLOAT sx, FLOAT sy,TCHAR *texts, DWORD color)" );
//	HRESULT hr;

	bool bReset = FALSE;
	// 2007-08-07 by bhsohn 스피커 아이템 추가
	if(strcmp(m_strText,texts) || color != m_dwColor || m_bReset || m_bReLoadString) 
	{
		// 2007-08-07 by bhsohn 스피커 아이템 추가
		m_bReLoadString = FALSE;

		m_dwColor = color;
		if(m_bReset == FALSE)
		{
			memset(m_strText,0x00,sizeof(m_strText));   
			strcpy(m_strText,texts);
		}
		m_bReset = FALSE;

	  // Prepare to create a bitmap
		DWORD*      pBitmapBits;
		BITMAPINFO bmi;
		ZeroMemory( &bmi.bmiHeader,  sizeof(BITMAPINFOHEADER) );
		bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth       =  (int)m_dwTexWidth;
		bmi.bmiHeader.biHeight      = -(int)m_dwTexHeight;
		bmi.bmiHeader.biPlanes      = 1;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biBitCount    = 32;

		// Create a DC and a bitmap for the font
		HBITMAP hbmBitmap = CreateDIBSection( g_pApp->GetHDC(), &bmi, DIB_RGB_COLORS,
											  (VOID**)&pBitmapBits, NULL, 0 );
		if(!hbmBitmap || !pBitmapBits)
		{
//			DBGOUT("ERROR : CD3DHanFont::SetText( FLOAT sx, FLOAT sy,TCHAR *texts, DWORD color) CreateDIBSection Failed\n");
			return E_FAIL;
		}
		SetMapMode( g_pApp->GetHDC(), MM_TEXT);

		// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
		// antialiased font, but this is not guaranteed.
		INT nHeight    = -MulDiv( m_dwFontHeight, 
			(INT)(GetDeviceCaps(g_pApp->GetHDC(), LOGPIXELSY) * m_fTextScale), 72 );
		DWORD dwBold   = (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FALSE;
		DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;
// 2008-04-15 by dgwoo ClearType 설정시 글씨가 깨지는 버그로 게임시 강제로 글씨 옵션을 변경.
//		HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
//							  FALSE, FALSE, LANGUAGE_CHARSET, OUT_DEFAULT_PRECIS,
//							  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
//							  FIXED_PITCH , m_strFontName );
		HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
							  FALSE, FALSE, LANGUAGE_CHARSET, OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
							  FIXED_PITCH , m_strFontName );
		if( NULL==hFont )
			return E_FAIL;

		HGDIOBJ hOldBitmap = SelectObject( g_pApp->GetHDC(), hbmBitmap );
		HGDIOBJ hOldFont = SelectObject( g_pApp->GetHDC(), hFont );

		DWORD x = 0;
		DWORD y = 0;
    
		// 글씨의 싸이즈 측정.
		SIZE size;
		int  iStringLength = lstrlen(m_strText);
		// add by jsy *******************************************//

		// 2006-02-03 by ispark, 영문에 맞지 않는 관계로 밑에서 다시 계산한다.
		GetTextExtentPoint32( g_pApp->GetHDC(), m_strText, iStringLength, &size );

		int xCharSize = size.cx / iStringLength;
		
		size.cx+=10;
		size.cy+=10;
		

		SetBkColor(   g_pApp->GetHDC(), 0xFF000000 );
		SetTextAlign( g_pApp->GetHDC(), TA_TOP|TA_LEFT );

/*
		// 폰트 윤곽선 처리.
		if (m_bOutLine==TRUE)
		{
			SetBkMode(g_pApp->GetHDC(),TRANSPARENT);
			SetTextColor( g_pApp->GetHDC(), RGB(1,1,1));
			ExtTextOut( g_pApp->GetHDC(), x, y+1, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
			ExtTextOut( g_pApp->GetHDC(), x+1, y, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
			ExtTextOut( g_pApp->GetHDC(), x+2, y+1, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
			ExtTextOut( g_pApp->GetHDC(), x+1, y+2, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
		}
*/
		// Set text properties

		// Loop through all printable character and output them to the bitmap..
		// Meanwhile, keep track of the corresponding tex coords for each character.
		SetTextAlign( g_pApp->GetHDC(), TA_TOP|TA_LEFT );
		//SetTextColor( g_pApp->GetHDC(), (COLORREF)color );

		int chrTotalCount = 0;
		int chrBitCount   = 0;
		int totalBitCount = 0;
		int chrStartPos   = 0;
		int chrFirstPos	  = 0;
		char totalString[256];
		char bitString[256];
		DWORD bitFirstColor = color;
		DWORD bitColor;
		
		int prebitlength = 0;

		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
		DrawFillRect(i_pFillRect); // 커서 위치를 그린다.
		// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
		
		strcpy( totalString , m_strText );
		//memset( chrCount,0, sizeof(chrCount) );

		while( true )
		{
			// 2009-01-22 by bhsohn Japan IME
			if(chrTotalCount >= 256 || chrStartPos >= 256)
			{
				break;
			}
			// end 2009-01-22 by bhsohn Japan IME
			if( totalString[chrTotalCount] == 0 )
			{
				if( chrStartPos > 0 )
				{
					strcpy( bitString, &totalString[chrStartPos]  );

					if (m_bOutLine==TRUE)
					{
						SetBkMode(g_pApp->GetHDC(),TRANSPARENT);
						SetTextColor( g_pApp->GetHDC(), RGB(1,1,1));
						ExtTextOut( g_pApp->GetHDC(), x+(prebitlength), y+1, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
						ExtTextOut( g_pApp->GetHDC(), x+1+(prebitlength), y, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
						ExtTextOut( g_pApp->GetHDC(), x+2+(prebitlength), y+1, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
						ExtTextOut( g_pApp->GetHDC(), x+1+(prebitlength), y+2, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
					}

					SetTextColor( g_pApp->GetHDC(), (COLORREF)bitFirstColor );
				
//					ExtTextOut( g_pApp->GetHDC(), (x+1)+(totalBitCount*xCharSize), y+1,
//								ETO_OPAQUE, NULL, bitString, chrBitCount, NULL );
					ExtTextOut( g_pApp->GetHDC(), (x+1)+(prebitlength), y+1,
								ETO_OPAQUE, NULL, bitString, chrBitCount, NULL );

					SIZE tsize;
					int nstrlen = strlen(bitString);
					GetTextExtentPoint32( g_pApp->GetHDC(), bitString, nstrlen, &tsize );
					prebitlength += tsize.cx;
					
					break;
				}

				if (m_bOutLine==TRUE)
				{
					SetBkMode(g_pApp->GetHDC(),TRANSPARENT);
					SetTextColor( g_pApp->GetHDC(), RGB(1,1,1));
					ExtTextOut( g_pApp->GetHDC(), x, y+1, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
					ExtTextOut( g_pApp->GetHDC(), x+1, y, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
					ExtTextOut( g_pApp->GetHDC(), x+2, y+1, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
					ExtTextOut( g_pApp->GetHDC(), x+1, y+2, ETO_CLIPPED, NULL, m_strText, iStringLength, NULL );
				}
				SetTextColor( g_pApp->GetHDC(), (COLORREF)color );
				ExtTextOut( g_pApp->GetHDC(), x+1, y+1, ETO_OPAQUE, NULL, m_strText, iStringLength, NULL );
				break;
			}
			
			if( totalString[chrTotalCount] == '\\' )
			{
#ifdef _EXTENDED_CHATCOLOR
				int nPos = 0;
				if (totalString[chrTotalCount + 1] == '#')
				{
					bitColor = 0;
					char tmp[7];
					bool hasHash = false;
					util::zero(tmp, sizeof(tmp[0]) * 7);
					tmp[6] = '\0';
					for (int i = 2; totalString[chrTotalCount + i] != NULL && i <= 8; i++)
					{
						if (i > 2 && totalString[chrTotalCount + i] == '#')
						{
							hasHash = true;
							break;
						}
						else if (nPos < 6)
						{
							tmp[nPos++] = totalString[chrTotalCount + i];
						}
						else
						{
							break;
						}
					}
					if (nPos == 6 && hasHash)
					{
						bitColor = GetFontColor(tmp);
						nPos++;
					}
					else
					{
						nPos = 0;
					}
				}
				else
				{
					bitColor = GetFontColor(totalString[chrTotalCount + 1]);
				}
#else
				bitColor = GetFontColor(totalString[chrTotalCount + 1]);
#endif
				if( bitColor != 0 )
				{
					if( chrFirstPos == 0 && chrTotalCount == 0)
					{
#ifdef _EXTENDED_CHATCOLOR
						chrFirstPos = chrTotalCount + nPos + 2;
						chrStartPos = chrTotalCount + nPos + 2;
						chrBitCount = -(nPos+1);
#else
						chrFirstPos = chrTotalCount + 2;
						chrStartPos = chrTotalCount + 2;
						chrBitCount = -1;
#endif
						
						bitFirstColor = bitColor;//GetFontColor( totalString[chrTotalCount+1] );

						chrTotalCount++;
						continue;
					}
					
					totalString[chrTotalCount] = 0;
					strcpy( bitString, &totalString[chrStartPos]  );

					if (m_bOutLine==TRUE)
					{
						SetBkMode(g_pApp->GetHDC(),TRANSPARENT);
						SetTextColor( g_pApp->GetHDC(), RGB(1,1,1));
						ExtTextOut( g_pApp->GetHDC(), x+(prebitlength), y+1, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
						ExtTextOut( g_pApp->GetHDC(), x+1+(prebitlength), y, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
						ExtTextOut( g_pApp->GetHDC(), x+2+(prebitlength), y+1, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
						ExtTextOut( g_pApp->GetHDC(), x+1+(prebitlength), y+2, ETO_CLIPPED, NULL, bitString, chrBitCount, NULL );
					}


					SetTextColor( g_pApp->GetHDC(), (COLORREF)bitFirstColor );


					ExtTextOut( g_pApp->GetHDC(), (x+1)+(prebitlength), y+1,
								ETO_OPAQUE, NULL, bitString, chrBitCount, NULL );

					

					if( bitFirstColor == bitColor )
						bitFirstColor = color;
					else
						bitFirstColor = bitColor;


#ifdef _EXTENDED_CHATCOLOR
					chrStartPos = chrTotalCount+nPos+2;
					totalBitCount += chrBitCount;
					chrBitCount = -(nPos+2);
#else
					chrStartPos = chrTotalCount+2;
					totalBitCount += chrBitCount;
					chrBitCount = -2;
#endif

					SIZE tsize;
					int nstrlen = strlen(bitString);
					GetTextExtentPoint32( g_pApp->GetHDC(), bitString, nstrlen, &tsize );
					prebitlength += tsize.cx;
				}
			}

			chrTotalCount++;
			chrBitCount++;
		}


		m_fTexCoords[0][0] = ((FLOAT)(x+0))/m_dwTexWidth;
		m_fTexCoords[0][1] = ((FLOAT)(y+0))/m_dwTexHeight;
		m_fTexCoords[0][2] = ((FLOAT)(x+0+size.cx))/m_dwTexWidth;
		m_fTexCoords[0][3] = ((FLOAT)(y+0+size.cy))/m_dwTexHeight;

		SIZE sizeLen = GetStringSize(m_strText);
		float fgap = size.cx - sizeLen.cx;

		if(m_bCullText && size.cx > (m_fWidth + fgap))
		{
			m_fTexCoords[0][0] = ((FLOAT)(size.cx-(m_fWidth + fgap)))/m_dwTexWidth;
		}

		x += size.cx+1;

		//////////////////////////임시코드//////////////////////
		if(m_fTexCoords[0][2]>1.0f)
		{
			m_fTexCoords[0][2] = 1.0f;
		}

		// Lock the surface and write the alpha values for the set pixels
		D3DLOCKED_RECT d3dlr;
		if(m_pTexture)
		{
			m_pTexture->LockRect( 0, &d3dlr, 0, 0 );
			BYTE* pDstRow = (BYTE*)d3dlr.pBits;
			WORD* pDst16;
			//unsigned short mcolor=0x0000;

			// 32비트를 16비트 형태로 변환.
			for( y=0; y < m_dwTexHeight; y++ )
			{
				pDst16 = (WORD*)pDstRow;
				for( x=0; x < m_dwTexWidth; x++ )
				{
					int xxx = (pBitmapBits[m_dwTexWidth*y + x] & 0x00ffffff) > 0; unsigned short mcolor = 0xf000; //if ((pBitmapBits[m_dwTexWidth*y + x]) & 0x00ffffff) // this branching is probably slowing the loop down...
					//{
						//mcolor |= (unsigned short)(((pBitmapBits[m_dwTexWidth*y + x]) & 0xf0000000) >> 20);
						mcolor |= (unsigned short)(((pBitmapBits[m_dwTexWidth*y + x]) & 0x00f00000) >> 12);
						mcolor |= (unsigned short)(((pBitmapBits[m_dwTexWidth*y + x]) & 0x0000f000) >> 8);
						mcolor |= (unsigned short)(((pBitmapBits[m_dwTexWidth*y + x]) & 0x000000f0) >> 4);
					//}
					//else mcolor=0x0000;
					*pDst16++ = xxx * mcolor;
				}
				pDstRow += d3dlr.Pitch;
			}

			// Done updating texture, so clean up used objects
			m_pTexture->UnlockRect(0);
		}
		SelectObject( g_pApp->GetHDC(), hOldBitmap );
		SelectObject( g_pApp->GetHDC(), hOldFont );
		DeleteObject( hbmBitmap );
		DeleteObject( hFont );
		bReset = TRUE;
	}
	if(sx != m_fPosX || sy != m_fPosY)
		bReset = TRUE;
	if(bReset && m_pVB)
	{
		m_fPosX = sx;
		m_fPosY = sy;
		// Fill vertex buffer
		FONT2DVERTEX* pVertices = NULL;
//		DWORD         dwNumTriangles = 0;
		// 2005-01-04 by jschoi
		m_pVB->Lock( 0, 0, (void**)&pVertices, 0 );

		FLOAT tx1,ty1,tx2,ty2;
		if(!m_bCullUV)
		{
			tx1 = m_fTexCoords[0][0];
			ty1 = m_fTexCoords[0][1];
			tx2 = m_fTexCoords[0][2];
			ty2 = m_fTexCoords[0][3];
		}
		else
		{
			tx1 = m_fTx1;
			ty1 = m_fTy1;
			tx2 = m_fTx2;
			ty2 = m_fTy2;
		}

		FLOAT w = (tx2-tx1) *  m_dwTexWidth / m_fTextScale;
		FLOAT h = (ty2-ty1) * m_dwTexHeight / m_fTextScale;
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(m_fPosX+0-0.5f,m_fPosY+h-0.5f,0.9f,1.0f) ,0, tx1, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(m_fPosX+0-0.5f,m_fPosY+0-0.5f,0.9f,1.0f) ,0, tx1, ty1 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(m_fPosX+w-0.5f,m_fPosY+h-0.5f,0.9f,1.0f) ,0, tx2, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(m_fPosX+w-0.5f,m_fPosY+0-0.5f,0.9f,1.0f) ,0, tx2, ty1 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(m_fPosX+w-0.5f,m_fPosY+h-0.5f,0.9f,1.0f) ,0, tx2, ty2 );
		*pVertices++ = InitFont2DVertex( D3DXVECTOR4(m_fPosX+0-0.5f,m_fPosY+0-0.5f,0.9f,1.0f) ,0, tx1, ty1 );

		// Unlock and render the vertex buffer
		m_pVB->Unlock();
	}

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 커서 위치를 그린다.
/// \author		// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
/// \date		2009-03-18 ~ 2009-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CD3DHanFont::DrawFillRect(RECT* i_pFillRect)
{
	if(NULL == i_pFillRect)
	{
		return;
	}
	
	int nStartPos = GetStringBuffPos(m_strText, i_pFillRect->left);
	int nEndPos = GetStringBuffPos(m_strText, i_pFillRect->right);

	if((-1 == nStartPos) || (-1 == nEndPos))
	{
		return;
	}
		
	int nCopyLen = nEndPos - nStartPos;

	if(nCopyLen <= 0)
	{
		return;
	}
	RECT rcRender;
	
	
	char chPreString[256];
	ZERO_MEMORY(chPreString);
	
	char chRectString[256];
	ZERO_MEMORY(chRectString);

	SIZE sizePre, sizeRect;
	strncpy(chPreString, m_strText, nStartPos);
	strncpy(chRectString, &m_strText[nStartPos], nCopyLen);		
	
	GetTextExtentPoint32( g_pApp->GetHDC(), chPreString, strlen(chPreString), &sizePre );
	GetTextExtentPoint32( g_pApp->GetHDC(), chRectString, strlen(chRectString), &sizeRect );

	rcRender.left = sizePre.cx;
	rcRender.top = 0;
	rcRender.right = rcRender.left+sizeRect.cx;
	rcRender.bottom = rcRender.top+sizeRect.cy;
	
	FillRect(g_pApp->GetHDC(), &rcRender, (HBRUSH)(COLOR_GRAYTEXT));
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SIZE GetStringSize(HDC hdc, TCHAR strText, char* FontStyle)
/// \brief		글자 길이 알아오기
/// \author		ispark
/// \date		2006-03-06 ~ 2006-03-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SIZE CD3DHanFont::GetStringSize(TCHAR *strText)
{
	// 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
	if(!strcmp(m_strSizeCheckText, strText))
	{
		return m_szCheckSize;
	}	
	// end 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선

	// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
	// antialiased font, but this is not guaranteed.
	INT nHeight    = -MulDiv( m_dwFontHeight, 
		(INT)(GetDeviceCaps(g_pApp->GetHDC(), LOGPIXELSY) * m_fTextScale), 72 );
	DWORD dwBold   = (m_dwFontFlags&D3DFONT_BOLD)   ? FW_BOLD : FALSE;
	DWORD dwItalic = (m_dwFontFlags&D3DFONT_ITALIC) ? TRUE    : FALSE;

	HFONT hFont    = CreateFont( nHeight, 0, 0, 0, dwBold, dwItalic,
						  FALSE, FALSE, LANGUAGE_CHARSET, OUT_DEFAULT_PRECIS,
						  CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
						  FIXED_PITCH , m_strFontName );
	if( NULL==hFont )
	{
		SIZE errsize;
		errsize.cx = errsize.cy = 0;
		return errsize;
	}

	HGDIOBJ hOldFont = SelectObject(g_pApp->GetHDC(), hFont);

	DWORD x = 0;
	DWORD y = 0;

	// 색깔 빼기
	char strTemp[2048]; *strTemp = '\0'; // = { 0, }; // 2015-10-21 PanKJ reducing unnecessary calls
	int nCount = 0; /* 2015-10-21 PanKJ*/ int strTextLen = lstrlen(strText);
	for (int i = 0; i < strTextLen; i++)
	{
#ifdef _EXTENDED_CHATCOLOR
		if (strText[i] == '\\')
		{
			int nPos = 0;
			if (strText[i+1] == '#')
			{
				bool hasHash = false;

				for (int a = 2; strText[i + a] != NULL && a <= 8; a++)
				{
					if (a > 2 && strText[i + a] == '#')
					{
						hasHash = true;
						break;
					}
					else if (nPos < 6)
					{
						nPos++;
					}
					else
					{
						break;
					}
				}
				if (nPos == 6 && hasHash)
				{
					i += nPos + 2;
					nPos++;
				}
				else
				{
					nPos = 0;
				}
			}
		}
#else
		if(strText[i] == '\\' && 
		   i + 1 < strTextLen &&
			CheckFontColor(strText[i+1]))
		{
			i += 1;
		}
#endif
		else
		{
			strTemp[nCount] = strText[i];
			nCount++;
		}
	}

	SIZE size;

	GetTextExtentPoint32(g_pApp->GetHDC(), strTemp, nCount, &size);

	SelectObject(g_pApp->GetHDC(), hOldFont);
	DeleteObject(hFont);

	// 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
	strncpy(m_strSizeCheckText, strText, 255);
	m_szCheckSize = size;		
	// end 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선

	return size;
}

// 2007-08-07 by bhsohn 스피커 아이템 추가
void CD3DHanFont::SetReLoadString (BOOL bReset)
{
	m_bReLoadString = bReset;
}
