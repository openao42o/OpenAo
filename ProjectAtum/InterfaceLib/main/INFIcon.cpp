// INFIcon.cpp: implementation of the CINFIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFIcon.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"			// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageList.h"		// 2011. 10. 10 by jskim UI시스템 변경

#define DEFAULT_ICON_IMAGE			"07900270"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFIcon::CINFIcon()
{
	FLOG( "CINFIcon()" );
	memset(m_strIconName,0x00, 20);
	m_nX = m_nY = 0;
	m_fScale = 1.0f;
	m_pGameData = NULL;
}

CINFIcon::~CINFIcon()
{
	FLOG( "~CINFIcon()" );
	map<string,CINFImageEx*>::iterator it = m_mapIcon.begin();			// 2011. 10. 10 by jskim UI시스템 변경

	while(it != m_mapIcon.end())
	{
		CINFImageEx* pImage = it->second;			// 2011. 10. 10 by jskim UI시스템 변경
		util::del (pImage);
		it++;
	}
	m_mapIcon.clear();
	util::del(m_pGameData);

}

HRESULT CINFIcon::InitDeviceObjects()
{
	FLOG( "CINFIcon::InitDeviceObjects()" );
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "item.tex");
	SetResourceFile(strPath);
	CINFImageEx* pImage;			// 2011. 10. 10 by jskim UI시스템 변경
	POINT start;start.x=0;start.y=0;
	POINT end;end.x=41;end.y=41;
	DataHeader* header = m_pGameData->GetStartPosition();
	if(!header)
		return E_FAIL;
	do 
	{
		pImage = new CINFImageEx();			// 2011. 10. 10 by jskim UI시스템 변경
		pImage->InitDeviceObjects( header );			// 2011. 10. 10 by jskim UI시스템 변경
		m_mapIcon[header->m_FileName] = pImage;
		header = m_pGameData->GetNext();
	} while(header);
	return S_OK;
}

HRESULT CINFIcon::SetResourceFile(char* szFileName)
{
	FLOG( "CINFIcon::SetResourceFile(char* szFileName)" );
	util::del(m_pGameData);
	m_pGameData = new CGameData;
	m_pGameData->SetFile(szFileName, FALSE, NULL, 0);

	return S_OK;
}

HRESULT CINFIcon::RestoreDeviceObjects()
{
	FLOG( "CINFIcon::RestoreDeviceObjects()" );
	map<string,CINFImageEx*>::iterator it = m_mapIcon.begin();			// 2011. 10. 10 by jskim UI시스템 변경

	while(it != m_mapIcon.end())
	{
		CINFImageEx* pImage = it->second;				// 2011. 10. 10 by jskim UI시스템 변경
		pImage->RestoreDeviceObjects( );
		it++;
	}

	return S_OK;
}

HRESULT CINFIcon::InvalidateDeviceObjects()
{
	FLOG( "CINFIcon::InvalidateDeviceObjects()" );
	map<string,CINFImageEx*>::iterator it = m_mapIcon.begin();				// 2011. 10. 10 by jskim UI시스템 변경

	while(it != m_mapIcon.end())
	{
		CINFImageEx* pImage = it->second;				// 2011. 10. 10 by jskim UI시스템 변경
		pImage->InvalidateDeviceObjects();
		it++;
	}
	return S_OK;
}

HRESULT CINFIcon::DeleteDeviceObjects()
{
	FLOG( "CINFIcon::DeleteDeviceObjects()" );
	map<string,CINFImageEx*>::iterator it = m_mapIcon.begin();				// 2011. 10. 10 by jskim UI시스템 변경

	while(it != m_mapIcon.end())
	{
		CINFImageEx* pImage = it->second;				// 2011. 10. 10 by jskim UI시스템 변경
		pImage->DeleteDeviceObjects();
		util::del(pImage);
		it++;
	}
	m_mapIcon.clear();
	return S_OK;
}

void CINFIcon::SetIcon(char* strName, int x, int y, float fScale )
{
	FLOG( "CINFIcon::SetIcon(char* strName, int x, int y, float fScale )" );
	strcpy(m_strIconName, strName);
	m_nX = x;
	m_nY = y;
	m_fScale = fScale;

}

CINFImageEx* CINFIcon::FindIcon(char* strName)			// 2011. 10. 10 by jskim UI시스템 변경
{
	FLOG( "CINFIcon::FindIcon(char* strName)" );
	CINFImageEx* pImage = NULL;				// 2011. 10. 10 by jskim UI시스템 변경
	map<string,CINFImageEx*>::iterator it  = m_mapIcon.find( strName );
	if(it != m_mapIcon.end())
	{
		pImage = it->second;
	}
	else
	{
		// default icon image
		it  = m_mapIcon.find( DEFAULT_ICON_IMAGE );
		if(it != m_mapIcon.end())
		{
			pImage = it->second;
		}
		
	}
	return pImage;
}

void CINFIcon::Render()
{
	FLOG( "CINFIcon::Render()" );
	// search icon
	CINFImageEx* pImage = NULL;				// 2011. 10. 10 by jskim UI시스템 변경
	map<string,CINFImageEx*>::iterator it  = m_mapIcon.find( m_strIconName );
	if(it != m_mapIcon.end())
	{
		pImage = it->second;
	}
	else
	{
		// default icon image
		it  = m_mapIcon.find( DEFAULT_ICON_IMAGE );
		if(it != m_mapIcon.end())
		{
			pImage = it->second;
		}
		
	}
	if(pImage)
	{
		// move position
		pImage->Move( m_nX, m_nY );
		pImage->SetScale( m_fScale, m_fScale );
		// render icon
		pImage->Render();
	}
	
//	CDXWnd::Render();
}

// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 상점에서 사용 가능한 아이템만 보여주기
POINT CINFIcon::GetIconSize()
{
	CINFImageEx* pImage = NULL;				// 2011. 10. 10 by jskim UI시스템 변경
	map<string,CINFImageEx*>::iterator it  = m_mapIcon.find( m_strIconName );
	if(it != m_mapIcon.end())
	{
		pImage = it->second;
	}
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	if(pImage)
		return pImage->GetImgSize();
	else
	{
		POINT temp;
		temp.x = 28;
		temp.y = 28;
		return temp;
	}
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조	
}
//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 상점에서 사용 가능한 아이템만 보여주기.
