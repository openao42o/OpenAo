// INFImageList.cpp: implementation of the CINFImageList class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "INFImageList.h"
#include "AtumApplication.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageList::CINFImageList()
{

}

CINFImageList::~CINFImageList()
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	
	for(it; it!=m_vecTextureInfo.end(); it++)
	{
		util::del( it->second );
	}
	m_vecTextureInfo.clear();
}

HRESULT CINFImageList::InitDeviceObjects()
{
	return S_OK;
}

HRESULT CINFImageList::RestoreDeviceObjects()
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end() )
	{
		LPDIRECT3DTEXTURE9 pTexture;
	    D3DXIMAGE_INFO SrcInfo;
		if(FAILED(D3DXCreateTextureFromFileInMemoryEx( g_pD3dDev, (LPCVOID)it->second->pGameData->m_pData, it->second->pGameData->m_DataSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
			0, &SrcInfo, NULL, &pTexture)))
		{
			DBGOUT("이미지를 열수가 없습니다.");
			//return FALSE;
		}
		it->second->srcInfo = SrcInfo;
		it->second->lpTexture = pTexture;
		AttachImage( it->second->pGameData );
		it++;
	}
	return S_OK;
}

HRESULT CINFImageList::InvalidateDeviceObjects()
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end() )
	{
		if( it->second->lpTexture )
		{
			it->second->lpTexture->Release();
			it->second->lpTexture = NULL;
		}
		it++;
	}
	return S_OK;
}

HRESULT CINFImageList::DeleteDeviceObjects()
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end() )
	{
		if( it->second->lpTexture )
		{
			it->second->lpTexture->Release(); // 2016-01-08 exception at this point (2)
			it->second->lpTexture = NULL;
			util::del( it->second );
		}
		it++;
	}
	m_vecTextureInfo.clear();
	return S_OK;
}

sTEXTUREINFO* CINFImageList::AddImage( DataHeader* pGameData, LPDIRECT3DTEXTURE9 pTexture, D3DXIMAGE_INFO& srcInfo )
{
		sTEXTUREINFO* pTextureInfo = new sTEXTUREINFO;
		pTextureInfo->lpTexture = pTexture;
		pTextureInfo->srcInfo = srcInfo;
		pTextureInfo->pGameData = pGameData;
		
		m_vecTextureInfo.push_back( make_pair( &pGameData->m_FileName[0], pTextureInfo ) );
		return pTextureInfo;
	}
// 2015-10-21 PanKJ excessive calls to memmove originating from this function, when shutting down
BOOL CINFImageList::DelImage( sTEXTUREINFO* pTextureInfo )
{
	if(pTextureInfo != NULL)
	{
		vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
		while(it != m_vecTextureInfo.end() )
		{
			if( pTextureInfo == it->second ) 
			{
				if( it->second->nCount > 0 )
				{
					char temp[256];
					sprintf(temp, "%d군데에서 사용되는 곳이 있습니다. ", it->second->nCount);
					DBGOUT(temp);
					return FALSE;
				}

				it->second->lpTexture->Release(); // 2016-01-08 exception at this point (2)
				it->second->lpTexture = NULL;
				it->second->pGameData = NULL;
				util::del( it->second );				
				m_vecTextureInfo.erase(it);
				break;
			}
			it++;
		}
	}	
	else
	{
		DBGOUT("Error DelImage()");
	}
	return TRUE;
}

sTEXTUREINFO* CINFImageList::CreateImage( DataHeader* pGameData )
{
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO SrcInfo;
	
	sTEXTUREINFO* pTextureInfo = FindImage( pGameData );
	
	if( pGameData->m_pData && pGameData->m_DataSize )
	{
	if( pTextureInfo == NULL )
	{
		if(FAILED(D3DXCreateTextureFromFileInMemoryEx( g_pD3dDev, (LPCVOID)pGameData->m_pData, pGameData->m_DataSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
			0, &SrcInfo, NULL, &pTexture)))
		{
			DBGOUT("이미지를 열수가 없습니다.");
			return FALSE;
		}
		pTextureInfo = AddImage( pGameData, pTexture, SrcInfo );
		}
// 		else
// 		{
// 			int a =0;
// 		}
		AttachImage( pGameData );
	}	
	return pTextureInfo;
}

void CINFImageList::AttachImage( DataHeader* pGameData )
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end() )
	{
		if( it->second->pGameData == pGameData )
		{
			it->second->nCount++;
			break;
		}
		it++;
	}
}

void CINFImageList::DetachImage( DataHeader* pGameData )
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end() )
	{
		if( it->second->pGameData == pGameData )
		{
			it->second->nCount--;
			if( it->second->nCount < 1 )
			{
				DelImage( it->second );
				break;
			}
		}
		it++;
	}	
}

sTEXTUREINFO*	CINFImageList::FindImage( DataHeader* pGameData )
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end() )
	{
		if( it->second->pGameData == pGameData ) 
		{
			return it->second;
		}
		it++;
	}
	return NULL;
}

sTEXTUREINFO* CINFImageList::FindImage( char* strName )
{
	vector< pair< string, sTEXTUREINFO* > >::iterator it = m_vecTextureInfo.begin();
	while(it != m_vecTextureInfo.end()  )
	{
		if( !strcmp( it->first.c_str(), strName ) )
		{
			return it->second;
		}
		it++;
	}
	return NULL;
}
