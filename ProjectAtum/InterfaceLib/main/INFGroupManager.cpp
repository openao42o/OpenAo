// INFGroupManager.cpp: implementation of the CINFGroupManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "AtumApplication.h"
#include "INFGroupManager.h"
#include "INFGroupImage.h"
#include "INFImageEx.h"
#include "INFControl.h"
#include "GameDataLast.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFGroupManager::CINFGroupManager( CINFImageList* pUIImageInfo, CGameData* pInterfaceData )
{
	m_pGameData = NULL;
	m_pUIImageInfo = pUIImageInfo;
	m_pInterfaceData = pInterfaceData;
}

CINFGroupManager::~CINFGroupManager()
{

}

HRESULT CINFGroupManager::InitDeviceObjects()
{
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_TEXTURE, "group.tex");
	
	if( m_pGameData )
	{
		util::del( m_pGameData );
	}
	m_pGameData = new CGameData;
	m_pGameData->SetFile( strPath, FALSE, NULL, 0 );
	return S_OK;
}

HRESULT CINFGroupManager::RestoreDeviceObjects()
{
	return S_OK;
}

HRESULT CINFGroupManager::DeleteDeviceObjects()
{
	if( m_pGameData )
		util::del( m_pGameData );
	return S_OK;
}

HRESULT CINFGroupManager::InvalidateDeviceObjects()
{
	return S_OK;
}

CINFGroupImage* CINFGroupManager::GetGroupImage( DataHeader* pGameData )
{
	int nPoint = 0;
	IMAGEFILEMAIN Gm, Im;
	CImageFileHeader ImageHader;
	CINFGroupImage* pCreateGroup = new CINFGroupImage;
	pCreateGroup->InitDeviceObjects( m_pUIImageInfo );

	memcpy(&Gm, &pGameData->m_pData[nPoint], sizeof(IMAGEFILEMAIN));
	nPoint+= sizeof(IMAGEFILEMAIN);

	memcpy(&ImageHader, &pGameData->m_pData[nPoint], sizeof(CImageFileHeader));
		nPoint+= sizeof(CImageFileHeader);

	for(int j=0; j< ImageHader.ImageCount; j++)
	{
		memcpy(&Im, &pGameData->m_pData[nPoint], sizeof(IMAGEFILEMAIN));
		nPoint+= sizeof(IMAGEFILEMAIN);
		
 		if(Im.m_bType == CONTROL_TYPE )
 		{
 			CINFControl* pControl = new CINFControl;
 			pControl->SetType( CONTROL_TYPE );
 			pControl->SetControlName( LPSTR(LPCTSTR(Im.m_strName)) );
 			pControl->SetTargetName( LPSTR(LPCTSTR(Im.m_sTargetName)) );
 			pControl->SetMinPos( D3DXVECTOR3( Im.m_v3ConMinPos.x, Im.m_v3ConMinPos.y, 0 ) );
 			pControl->SetMaxPos( D3DXVECTOR3( Im.m_v3ConMaxPos.x, Im.m_v3ConMaxPos.y, 0 ) );
			pCreateGroup->AttachImage( pControl );
 		}
 		else
 		{
 			CINFImageEx* pImage = new CINFImageEx;
 			pImage->SetType( IMAGE_TYPE );
			DataHeader* pImageData = m_pInterfaceData->Find( Im.m_strName );
 			pImage->InitDeviceObjects( pImageData );
 			//pImage->RestoreDeviceObjects();
 			
 			pImage->SetTransVector( D3DXVECTOR2( Im.m_v3Trans.x, Im.m_v3Trans.y ) );
 			pImage->SetCenterVector( D3DXVECTOR2( Im.m_v3Center.x, Im.m_v3Center.y ) );
 			pImage->SetScale( Im.m_v3Scaling.x, Im.m_v3Scaling.y );
 			pImage->SetAngle( Im.m_fAngle );
 			pImage->SetScaleIsPercent( Im.m_fScalePercent );
 			pImage->SetFileName(Im.m_strName);
 			pImage->SetRect( &Im.m_rRect );
 			pCreateGroup->AttachImage( pImage );
 		}
		pCreateGroup->SetTransVector( D3DXVECTOR2( Gm.m_v3Trans.x, Gm.m_v3Trans.y ));		
	}
	return pCreateGroup;
}

DataHeader* CINFGroupManager::FindResource(char* szRcName )
{
	FLOG( "CINFGameMain::FindResource(char* szRcName)" );
	DataHeader* pHeader = NULL;
	if(m_pGameData)
	{
		pHeader = m_pGameData->Find( szRcName );
	}

	// 2011-12-14 hsson, 리소스 파일이 없을 경우 경고 메시지 추가
	if( NULL == pHeader )
	{
		char cBuf[256] = {0, };
		sprintf( cBuf, "%s <- 리소스가 없습니다.", szRcName );

		MessageBox( NULL, cBuf, "", MB_OK );
	}
	// end 2011-12-14 hsson, 리소스 파일이 없을 경우 경고 메시지 추가

	return pHeader;
}