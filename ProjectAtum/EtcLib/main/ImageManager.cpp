// CImageManager.cpp: implementation of the CImageManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageManager.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageManager::CImageManager()
{
	FLOG( "CImageManager()" );

//	g_pImgManager = this;
	
//	m_pImgBack		= NULL;	
//	m_pFontTitle	= NULL;
}

CImageManager::~CImageManager()
{
	FLOG( "~CImageManager()" );
	
//	util::del(m_pImgBack);
//	util::del(m_pFontTitle);
}

HRESULT CImageManager::InitDeviceObjects()
{
	FLOG( "CImageManager::InitDeviceObjects(char* pData, int nSize)" );

//	DataHeader	* pDataHeader ;
//	char buf[32];
//	memset(buf, 0x00, 32);
//	wsprintf( buf, "shmback");
//	
//	m_pImgBack = new CINFImage;
//	pDataHeader = FindResource(buf);
//	m_pImgBack->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize) ;
//
//	m_pFontTitle = new CD3DHanFont(_T("±¼¸²Ã¼"),9, D3DFONT_ZENABLE,  FALSE,256,32);
//	m_pFontTitle->InitDeviceObjects(g_pD3dDev) ;
	
	return S_OK;
}


HRESULT CImageManager::RestoreDeviceObjects(DWORD dwType)
{
	FLOG( "CImageManager::RestoreDeviceObjects(DWORD dwType)" );

	int nType = SetGameStateInit(dwType);

	map<DWORD,ImageInfo>::iterator it = m_mapImgBank.find(nType);
	if(it != m_mapImgBank.end())
	{
		if(it->second.dwImageType == nType)
		{
			it->second.pINFImage->RestoreDeviceObjects();
		}

	}
	
	return S_OK;
}

HRESULT CImageManager::InvalidateDeviceObjects(DWORD dwType)
{
	FLOG( "CImageManager::InvalidateDeviceObjects(DWORD dwType)" );

//	m_pImgBack->InvalidateDeviceObjects();
//	m_pFontTitle->InvalidateDeviceObjects();
	int nType = SetGameStateInit(dwType);

	map<DWORD,ImageInfo>::iterator it = m_mapImgBank.find(nType);
	if(it != m_mapImgBank.end())
	{
		if(it->second.dwImageType == nType)
		{
			it->second.pINFImage->InvalidateDeviceObjects();
		}

	}
	
	return S_OK;
}

HRESULT CImageManager::DeleteDeviceObjects(DWORD dwType)
{
	FLOG( "CImageManager::DeleteDeviceObjects(DWORD dwType)" );
	
//	m_pImgBack->DeleteDeviceObjects();
//	m_pFontTitle->DeleteDeviceObjects();
//
//	util::del(m_pImgBack);
//	util::del(m_pFontTitle);
	return S_OK;
}


void CImageManager::Render()
{
	FLOG( "CImageManager::Render()" );	
}

//void CImageManager::Tick()
//{
//	FLOG( "CImageManager::Tick()" );
//}
//
//int CImageManager::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	FLOG( "CImageManager::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
//	switch(uMsg)
//	{
//	case WM_MOUSEMOVE:
//		{
//		}
//		break;
//	case WM_LBUTTONDOWN:
//		{
//		}
//		break;
//	case WM_LBUTTONUP:
//		{
//		}
//		break;
//	}
//	return INF_MSGPROC_NORMAL;
//}


int	CImageManager::SetGameStateInit( DWORD dwType )
{
	FLOG( "SetGameStateInit( DWORD dwType )" );	

	if( dwType == _SELECT || dwType == _CREATE )
	{
		return IMAGE_START;
	}
	else if( dwType == _GAME || dwType == _SHOP || dwType == _CITY || dwType == _SCRIPT )
	{
		return IMAGE_MAIN;
	}

	return IMAGE_GLOBAL;
}