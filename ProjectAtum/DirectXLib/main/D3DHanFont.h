// D3DHanFont.h: interface for the CD3DHanFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DHANFONT_H__A9C46610_34BE_44B5_9EEA_A6D9D2DC313E__INCLUDED_)
#define AFX_D3DHANFONT_H__A9C46610_34BE_44B5_9EEA_A6D9D2DC313E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <tchar.h>
// 2005-01-03 by jschoi
//#include <D3D8.h>
#include <d3d9.h>
#include <imm.h>


// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004

// 2005-01-04 by jschoi
// Font rendering flags
//#define D3DFONT_CENTERED    0x0001
//#define D3DFONT_TWOSIDED    0x0002
//#define D3DFONT_FILTERED    0x0004

#define D3DFONT_NOTFILTERED		0x0008	// 2005-03-10 by jschoi


class CD3DHanFont  
{
public:
    CD3DHanFont(const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L , BOOL outline =FALSE ,DWORD dwMaxWidth=256L, DWORD dwMaxHeight=32,BOOL bCullText=FALSE, BOOL bCullUV=FALSE);
    ~CD3DHanFont();
	
	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
//	HRESULT SetText( FLOAT sx, FLOAT sy,TCHAR* texts, DWORD color);
// 	HRESULT DrawText( FLOAT x, FLOAT y, DWORD dwColor, 
//                       TCHAR* strText, DWORD dwFlags=0L );
	HRESULT SetText( FLOAT sx, FLOAT sy,TCHAR* texts, DWORD color, RECT* i_pFillRect=NULL);
	HRESULT DrawText( FLOAT x, FLOAT y, DWORD dwColor, 
                      TCHAR* strText, DWORD dwFlags=0L, RECT* i_pFillRect=NULL);
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

    HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();

	void SetUV(float tx1, float ty1, float tx2, float ty2);
	void SetTextureWidth(float fWidth) { m_fWidth = fWidth; }
	SIZE GetStringSize(TCHAR *strText);

	DWORD GetTexWidth() { return m_dwTexWidth; }
	DWORD GetTexHeight() { return m_dwTexHeight; }
	BOOL GetReset() { return m_bReset; }

	void SetTexWidth(DWORD dwWidth) { m_dwTexWidth = dwWidth; }
	void SetTexHeight(DWORD dwHeight) { m_dwTexHeight = dwHeight; }

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	void SetReLoadString(BOOL bReset);	

private:
	// 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가
	void DrawFillRect(RECT* i_pFillRect);
	// end 2009-03-18 by bhsohn 채팅창 커서 이동 시스템 추가

	// 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
public:
	LPDIRECT3DTEXTURE9      m_pTexture;   // The d3d texture for this font

private:
    DWORD   m_dwTexWidth;                 // Texture dimensions
    DWORD   m_dwTexHeight;		

    DWORD   m_dwFontHeight;
    DWORD   m_dwFontFlags;
	
	LPDIRECT3DDEVICE9       m_pd3dDevice; // A D3DDevice used for rendering
    //LPDIRECT3DTEXTURE9      m_pTexture;   // The d3d texture for this font
	LPDIRECT3DVERTEXBUFFER9 m_pVB;        // VertexBuffer for rendering text

    FLOAT   m_fTextScale;
    FLOAT   m_fTexCoords[1][4];
    // Stateblocks for setting and restoring render states
    DWORD   m_dwSavedStateBlock;
    DWORD   m_dwDrawTextStateBlock;
	TCHAR   m_strFontName[80];

	BOOL	m_bOutLine;
	BOOL	m_bCullText;
	BOOL	m_bCullUV;
//	TCHAR	m_strText[SIZE_MAX_CHAT_MESSAGE + SIZE_MAX_CHARACTER_NAME+4];
	TCHAR	m_strText[256];

	FLOAT	m_fPosX;
	FLOAT	m_fPosY;
	DWORD	m_dwColor;
	FLOAT	m_fTx1, m_fTy1, m_fTx2, m_fTy2;
	FLOAT	m_fWidth;
	BOOL	m_bReset;

	// 2007-08-07 by bhsohn 스피커 아이템 추가
	BOOL	m_bReLoadString;

	// 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
	TCHAR	m_strSizeCheckText[256];
	SIZE	m_szCheckSize;
	// end 2008-09-19 by bhsohn CD3DHanFont::GetStringSize 속도 개선
};

#endif // !defined(AFX_D3DHANFONT_H__A9C46610_34BE_44B5_9EEA_A6D9D2DC313E__INCLUDED_)
