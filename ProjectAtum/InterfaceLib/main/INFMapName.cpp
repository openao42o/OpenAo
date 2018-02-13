// INFMapName.cpp: implementation of the CINFMapName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMapName.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"         // 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMapName::CINFMapName()
{
	FLOG( "CINFMapName()" );
	
	m_pGameData = NULL;
	m_bRestored = FALSE;
}

CINFMapName::~CINFMapName()
{
	FLOG( "~CINFMapName()" );
	
	map<int, CINFImageEx*>::iterator it = m_mapMapName.begin();
	while(it != m_mapMapName.end())
	{
		CINFImage* pImage = it->second;
		util::del(pImage);
		it++;
	}
	m_mapMapName.clear();
	util::del(m_pGameData);
}

HRESULT CINFMapName::InitDeviceObjects(char* strTexture)
{
	FLOG( "CINFMapName::InitDeviceObjects(char* pData, int nSize)" );

	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, strTexture);
	SetResourceFile(strPath);
	
	return S_OK;
}

HRESULT CINFMapName::RestoreDeviceObjects()
{
	FLOG( "CINFMapName::RestoreDeviceObjects()" );

	if(!m_bRestored)
	{
		map<int, CINFImageEx*>::iterator it = m_mapMapName.begin();
		while(it != m_mapMapName.end())
		{
			CINFImageEx* pImage = it->second;
			pImage->RestoreDeviceObjects();
			it++;
		}
		m_bRestored = TRUE;
	}

	return S_OK;
}

HRESULT CINFMapName::InvalidateDeviceObjects()
{
	FLOG( "CINFMapName::InvalidateDeviceObjects()" );

	if(m_bRestored)
	{
		map<int, CINFImageEx*>::iterator it = m_mapMapName.begin();
		while(it != m_mapMapName.end())
		{
			CINFImageEx* pImage = it->second;
			pImage->InvalidateDeviceObjects();
			it++;
		}
		m_bRestored = FALSE;
	}

	return S_OK;
}

HRESULT CINFMapName::DeleteDeviceObjects()
{
	FLOG( "CINFMapName::DeleteDeviceObjects()" );

	map<int, CINFImageEx*>::iterator it = m_mapMapName.begin();
	while(it != m_mapMapName.end())
	{
		CINFImageEx* pImage = it->second;
		pImage->DeleteDeviceObjects();
		util::del(pImage);
		it++;
	}
	m_mapMapName.clear();

	util::del(m_pGameData);
	
	return S_OK;
}

HRESULT CINFMapName::SetResourceFile(char* szFileName)
{
	FLOG( "CINFMapName::SetResourceFile(char* szFileName)" );
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	m_pGameData->SetFile(szFileName, FALSE, NULL, 0);

	return S_OK;
}

CINFImageEx* CINFMapName::FindMapNameImage(int nType)
{
	FLOG( "CINFMapName::FindMapNameImage(int nType)" );
	map<int, CINFImageEx*>::iterator it = m_mapMapName.find(nType);
	if(it != m_mapMapName.end())
	{
		return it->second;
	}

	if(!m_pGameData)
		return NULL;
	char buf[20];
	wsprintf(buf, "%04d",nType);
	DataHeader* pHeader;
	pHeader = m_pGameData->Find(buf);
	if(pHeader)
	{
		CINFImageEx* pFace = new CINFImageEx;
		pFace->InitDeviceObjects(pHeader);
		pFace->RestoreDeviceObjects();
		m_mapMapName[nType] = pFace;
		return pFace;
	}
	return NULL;
}

void CINFMapName::Render()
{
	FLOG( "CINFMapName::Render()" );	
}

void CINFMapName::Tick()
{
	FLOG( "CINFMapName::Tick()" );
}