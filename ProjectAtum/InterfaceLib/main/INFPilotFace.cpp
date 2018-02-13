// INFPilotFace.cpp: implementation of the CINFPilotFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFPilotFace.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "dxutil.h"
#include "INFImageEx.h"										  // 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFPilotFace::CINFPilotFace()
{
	FLOG( "CINFPilotFace()" );
	m_pGameData = NULL;
	m_bRestored = FALSE;

}

CINFPilotFace::~CINFPilotFace()
{
	FLOG( "~CINFPilotFace()" );
	map<int, CINFImageEx*>::iterator it = m_mapFace.begin();
	while(it != m_mapFace.end())
	{
		CINFImageEx* pImage = it->second;
		util::del(pImage);
		it++;
	}
	m_mapFace.clear();
	util::del(m_pGameData);

}
int CINFPilotFace::GetPilotFaceNumber()
{
	FLOG( "CINFPilotFace::GetPilotFaceNumber()" );
	return m_pGameData->GetTotalNumber();
}

CINFImageEx* CINFPilotFace::FindPilotImage(int nType)
{
	FLOG( "CINFPilotFace::FindPilotImage(int nType)" );
	map<int, CINFImageEx*>::iterator it = m_mapFace.find(nType);
	if(it != m_mapFace.end())
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
		m_mapFace[nType] = pFace;
		return pFace;
	}
	return NULL;
}

CINFImageEx* CINFPilotFace::GetStartImage()
{
	FLOG( "CINFPilotFace::GetStartImage()" );
	if(!m_pGameData)
		return NULL;
	DataHeader* pHeader = m_pGameData->GetStartPosition();

	if(pHeader)
	{
		int b = atoi(pHeader->m_FileName);
		map<int, CINFImageEx*>::iterator it = m_mapFace.find(b);
		if(it != m_mapFace.end())
		{
			return it->second;
		}
		CINFImageEx* pFace = new CINFImageEx;
		pFace->InitDeviceObjects(pHeader);
		pFace->RestoreDeviceObjects();
		m_mapFace[(BYTE)b] = pFace;
		return pFace;
	}
	return NULL;
}

CINFImageEx* CINFPilotFace::GetNextImage()
{
	FLOG( "CINFPilotFace::GetNextImage()" );
	if(!m_pGameData)
		return NULL;
	DataHeader* pHeader = m_pGameData->GetNext();
	if(pHeader)
	{
		int b = atoi(pHeader->m_FileName);
		map<int, CINFImageEx*>::iterator it = m_mapFace.find(b);
		if(it != m_mapFace.end())
		{
			return it->second;
		}
		CINFImageEx* pFace = new CINFImageEx;
		pFace->InitDeviceObjects(pHeader);
		pFace->RestoreDeviceObjects();
		m_mapFace[b] = pFace;
		return pFace;
	}
	else
	{
		return GetStartImage();
	}
	return NULL;
}


HRESULT CINFPilotFace::InitDeviceObjects(char* strTexture)
{
	FLOG( "CINFPilotFace::InitDeviceObjects(char* strTexture)" );
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, strTexture);
	SetResourceFile(strPath);

	return S_OK;
}

HRESULT CINFPilotFace::RestoreDeviceObjects()
{
	FLOG( "CINFPilotFace::RestoreDeviceObjects()" );
	if(!m_bRestored)
	{
		map<int, CINFImageEx*>::iterator it = m_mapFace.begin();
		while(it != m_mapFace.end())
		{
			CINFImageEx* pImage = it->second;
			pImage->RestoreDeviceObjects();
			it++;
		}
		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFPilotFace::InvalidateDeviceObjects()
{
	FLOG( "CINFPilotFace::InvalidateDeviceObjects()" );
	if(m_bRestored)
	{
		map<int, CINFImageEx*>::iterator it = m_mapFace.begin();
		while(it != m_mapFace.end())
		{
			CINFImageEx* pImage = it->second;
			pImage->InvalidateDeviceObjects();
			it++;
		}
		m_bRestored = FALSE;
	}
	return S_OK;
}

HRESULT CINFPilotFace::DeleteDeviceObjects()
{
	FLOG( "CINFPilotFace::DeleteDeviceObjects()" );
	map<int, CINFImageEx*>::iterator it = m_mapFace.begin();
	while(it != m_mapFace.end())
	{
		CINFImageEx* pImage = it->second;
		pImage->DeleteDeviceObjects();
		util::del(pImage);
		it++;
	}
	m_mapFace.clear();

	util::del(m_pGameData);

	return S_OK;
}

void CINFPilotFace::Render()
{
	FLOG( "CINFPilotFace::Render()" );

}

HRESULT CINFPilotFace::SetResourceFile(char* szFileName)
{
	FLOG( "CINFPilotFace::SetResourceFile(char* szFileName)" );
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	m_pGameData->SetFile(szFileName, FALSE, NULL, 0);

	return S_OK;
}
