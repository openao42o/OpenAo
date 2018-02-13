// INFImageFile.cpp: implementation of the CINFImageFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFImageFile.h"
#include "AtumApplication.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageFile::CINFImageFile()
{
	memset(m_szFileName, 0x00, sizeof(m_szFileName));
	memset(&m_srcInfo, 0x00, sizeof(D3DXIMAGE_INFO));
}

CINFImageFile::~CINFImageFile()
{

}

HRESULT CINFImageFile::InitDeviceObjects(char* pData, int nSize)
{
	FLOG( "CINFImageFile::InitDeviceObjects(char* pData, int nSize)" );
	strncpy(m_szFileName, pData, nSize);
	m_v2Trans = D3DXVECTOR2( 0, 0);
	return S_OK;
}

HRESULT CINFImageFile::RestoreDeviceObjects()
{
	FLOG( "CINFImageFile::RestoreDeviceObjects()" );
	// 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	//if(g_pD3dApp->m_pd3dxSprite)
	//{
	//	m_pd3dxSprite = g_pD3dApp->m_pd3dxSprite;
	//}
	if(g_pD3dApp->GetDirectSprite())
	{
		m_pd3dxSprite = g_pD3dApp->GetDirectSprite();
	}
	// end 2010-09-29 by jskim, SpriteDevice Lost 되었을 때 구조 변경
	if(!m_pd3dxSprite)
	{
		D3DXCreateSprite(g_pD3dDev , &m_pd3dxSprite);
		m_bSpriteCrate = TRUE;
	}
	if(FAILED(D3DXCreateTextureFromFileEx(g_pD3dDev, m_szFileName, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 
		0, &m_srcInfo, NULL, &m_pTexture)))
//	if(FAILED(D3DXCreateTextureFromFile( g_pD3dDev, m_szFileName, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
//		0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT, 
//		0, &SrcInfo, NULL, &m_pTexture)))
	{
		if(m_bSpriteCrate)
			SAFE_RELEASE(m_pd3dxSprite);
		return E_FAIL;
	}
	return S_OK;
}