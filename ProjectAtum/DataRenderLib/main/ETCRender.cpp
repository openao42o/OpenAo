// ETCRender.cpp: implementation of the CETCRender class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ETCRender.h"
#include "AtumApplication.h"
#include "Background.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Camera.h"
#include "dxutil.h"

#define CLOUD_NUMBER		3
#define SKY_BOX_SCALE				(g_pScene->m_fFogEndValue/133.0f)
#define SKY_DAY_TEX_INDEX			1
#define SKY_NIGHT_TEX_INDEX			2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CETCRender::CETCRender()
{
	FLOG( "CETCRender()" );
	m_pSky = nullptr;
	m_pSkyNight = nullptr;
	m_pCloud = nullptr;
	m_pCloudNight = nullptr;

	D3DXVECTOR3 vPos,vVel,vUp;
	vPos = D3DXVECTOR3(0,0,0);
	vVel = D3DXVECTOR3(1,0,0);
	vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtRH( &m_mMatrix, &(vPos), &vVel/*&((vPos=0) + vVel)*/, &vUp);
	D3DXMatrixInverse( &m_mMatrix, nullptr, &m_mMatrix );
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale,22.0f,22.0f,22.0f);
	m_mMatrix = mScale * m_mMatrix;
	m_fCloudAnimationTime = 0;
}

CETCRender::~CETCRender()
{
	FLOG( "~CETCRender()" );
	util::del(m_pSky);
	util::del(m_pSkyNight);
	util::del(m_pCloud);
	util::del(m_pCloudNight);
}

HRESULT CETCRender::InitDeviceObjects()
{
	FLOG( "CETCRender::InitDeviceObjects()" );
	CGameData * pData;
	char buf[256];
	
	
	// 2005-01-12 by jschoi - 낮 스카이 박스 
	pData = new CGameData;
	memset(buf, 0x00, sizeof(buf));
	m_pSky = new CSkinnedMesh(FALSE);
	m_pSky->InitDeviceObjects();
	//wsprintf( buf, ".\\Res-Map\\%s.sky", g_pGround->m_projectInfo.strProjectName );
	wsprintf( buf, ".\\Res-Map\\%d.sky", g_pGround->m_pMapInfo->Sky );

	if(m_pSky && pData->SetFile(buf,NULL,NULL,NULL) )
	{
		m_pSky->LoadMeshHierarchyFromMem( pData );
	}
	else
	{
		util::del(m_pSky); // 낮 스카이 박스 없음
	}
	delete pData;
	pData = NULL;


	// 2005-01-12 by jschoi - 밤 스카이 박스
	pData = new CGameData;
	memset(buf, 0x00, sizeof(buf));
	m_pSkyNight = new CSkinnedMesh(FALSE);
	m_pSkyNight->InitDeviceObjects();
	//wsprintf( buf, ".\\Res-Map\\%sn.sky", g_pGround->m_projectInfo.strProjectName );
	wsprintf( buf, ".\\Res-Map\\%dn.sky", g_pGround->m_pMapInfo->Nsky);

	if(m_pSkyNight && pData->SetFile(buf,NULL,NULL,NULL) )
	{
		m_pSkyNight->LoadMeshHierarchyFromMem( pData );
	}
	else
	{
		util::del(m_pSkyNight); // 밤 스카이 없음
	}
	delete pData;
	pData = NULL;


	// 2005-01-12 by jschoi - 낮 구름
	pData = new CGameData;
	memset(buf, 0x00, sizeof(buf));
	m_pCloud = new CSkinnedMesh(FALSE);
	m_pCloud->InitDeviceObjects();
	
	//wsprintf( buf, ".\\Res-Map\\%s.cld", g_pGround->m_projectInfo.strProjectName );
	wsprintf( buf, ".\\Res-Map\\%d.cld", g_pGround->m_pMapInfo->Cloud);
	if(m_pCloud && pData->SetFile(buf,NULL,NULL,NULL))
	{
		m_pCloud->LoadMeshHierarchyFromMem( pData );
	}
	else
	{
		util::del(m_pCloud); // 낮 구름 없음
	}
	delete pData;
	pData = NULL;


	// 2005-01-12 by jschoi - 밤 구름
	pData = new CGameData;
	memset(buf, 0x00, sizeof(buf));
	m_pCloudNight = new CSkinnedMesh(FALSE);
	m_pCloudNight->InitDeviceObjects();
	//wsprintf( buf, ".\\Res-Map\\%sn.cld", g_pGround->m_projectInfo.strProjectName );
	wsprintf( buf, ".\\Res-Map\\%dn.cld", g_pGround->m_pMapInfo->Cloud);
	if(m_pCloudNight && pData->SetFile(buf,NULL,NULL,NULL))
	{
		m_pCloudNight->LoadMeshHierarchyFromMem( pData );
	}
	else
	{
		util::del(m_pCloudNight);	// 밤 구름 없음
	}
	delete pData;
	pData = NULL;
	
	return S_OK;
}

HRESULT CETCRender::RestoreDeviceObjects()
{
	FLOG( "CETCRender::RestoreDeviceObjects()" );

	if(m_pSky)
		m_pSky->RestoreDeviceObjects();
	if(m_pSkyNight)
		m_pSkyNight->RestoreDeviceObjects();
	if(m_pCloud)
		m_pCloud->RestoreDeviceObjects();
	if(m_pCloudNight)
		m_pCloudNight->RestoreDeviceObjects();

	return S_OK;
}

HRESULT CETCRender::InvalidateDeviceObjects()
{
	FLOG( "CETCRender::InvalidateDeviceObjects()" );

	if(m_pSky)
		m_pSky->InvalidateDeviceObjects();
	if(m_pSkyNight)
		m_pSkyNight->InvalidateDeviceObjects();
	if(m_pCloud)
		m_pCloud->InvalidateDeviceObjects();
	if(m_pCloudNight)
		m_pCloudNight->InvalidateDeviceObjects();

	return S_OK;
}

HRESULT CETCRender::DeleteDeviceObjects()
{
	FLOG( "CETCRender::DeleteDeviceObjects()" );
	if(m_pSky)
	{
		m_pSky->DeleteDeviceObjects();
		util::del(m_pSky);
	}
	if(m_pSkyNight)
	{
		m_pSkyNight->DeleteDeviceObjects();
		util::del(m_pSkyNight);
	}
	if(m_pCloud)
	{
		m_pCloud->DeleteDeviceObjects();
		util::del(m_pCloud);
	}
	if(m_pCloudNight)
	{
		m_pCloudNight->DeleteDeviceObjects();
		util::del(m_pCloudNight);
	}
	return S_OK;
}

void CETCRender::SkyRender()
{
	FLOG( "CETCRender::SkyRender()" );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	D3DXVECTOR3 vPos,vVel,vUp;
	vPos = D3DXVECTOR3(0,0,0);
	vVel = D3DXVECTOR3(1,0,0);
	vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtRH( &m_mMatrix, &vPos, &(vPos + vVel), &vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	D3DXMATRIX mScale;
	D3DXMatrixScaling(&mScale,SKY_BOX_SCALE,SKY_BOX_SCALE,SKY_BOX_SCALE);
	m_mMatrix = mScale * m_mMatrix;

	//  하늘높이의 기준은 기어가 아니라 카메라다 2004-08-04 jschoi
	m_mMatrix._41 = g_pCamera->GetEyePt().x;
	m_mMatrix._42 = g_pCamera->GetEyePt().y;
	m_mMatrix._43 = g_pCamera->GetEyePt().z;

	// 2005-01-12 by jschoi - 스카이 박스 렌더링
	D3DLIGHT9 lightTemp = g_pScene->m_light0;
	lightTemp.Direction = D3DXVECTOR3(0,1,0);
	g_pD3dDev->SetLight( 0, &lightTemp );
    g_pD3dDev->SetRenderState( D3DRS_FOGENABLE, IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
	g_pD3dDev->SetFVF( D3DFVF_FOGVERTEX );
	g_pD3dDev->SetRenderState( D3DRS_FOGCOLOR,  g_pScene->m_dwFogColor );

	if(m_pSky && g_pScene->m_fAlphaSky != 0.0f)
	{
		m_pSky->Tick(0.0f);
		m_pSky->SetWorldMatrix(m_mMatrix);
		m_pSky->Render(); 
	}

	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ); 
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );

	if(m_pSkyNight && g_pScene->m_fAlphaSky != 1.0f)
	{
		g_pD3dDev->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((BYTE)(g_pScene->m_fAlphaSky*255), 0, 0, 0) ); 
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );

		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVSRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );

		m_pSkyNight->Tick(0.0f);
		m_pSkyNight->SetWorldMatrix(m_mMatrix);
		m_pSkyNight->Render(); 
	}

	// 2005-01-12 by jschoi - 구름 렌더링
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );	
	if(m_pCloud  && g_pScene->m_fAlphaSky != 0.0f)
	{
		g_pD3dDev->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((BYTE)((1.0f - g_pScene->m_fAlphaSky)*255), 0, 0, 0) ); 
		g_pD3dDev->SetTextureStageState ( 0 , D3DTSS_ALPHAOP , D3DTOP_SUBTRACT ); 
		g_pD3dDev->SetTextureStageState ( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE ); 
		g_pD3dDev->SetTextureStageState ( 0 , D3DTSS_ALPHAARG2 , D3DTA_TFACTOR ); 

		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		
		m_mMatrix._42 = g_pCamera->GetEyePt().y;
		m_fCloudAnimationTime += g_pD3dApp->GetElapsedTime();
		m_pCloud->Tick(m_fCloudAnimationTime);
		m_pCloud->SetWorldMatrix(m_mMatrix);
		m_pCloud->Render();
	}

	if(m_pCloudNight  && g_pScene->m_fAlphaSky != 1.0f)
	{
		g_pD3dDev->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB((BYTE)(g_pScene->m_fAlphaSky*255), 0, 0, 0) ); 
		g_pD3dDev->SetTextureStageState ( 0 , D3DTSS_ALPHAOP , D3DTOP_SUBTRACT ); 
		g_pD3dDev->SetTextureStageState ( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE ); 
		g_pD3dDev->SetTextureStageState ( 0 , D3DTSS_ALPHAARG2 , D3DTA_TFACTOR ); 

		g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		m_mMatrix._42 = g_pCamera->GetEyePt().y;
		m_fCloudAnimationTime += g_pD3dApp->GetElapsedTime();
		m_pCloudNight->Tick(m_fCloudAnimationTime);
		m_pCloudNight->SetWorldMatrix(m_mMatrix);
		m_pCloudNight->Render();
	}
	
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	
//	g_pD3dDev->SetLight( 0, &g_pScene->m_light0 );
}

// 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)
void	CETCRender::ChangeSkyBox( char* szSkyBoxName )
{
	// 낮 스카이 박스와 밤 스카이 박스 둘다 바꿔준다
	
	CGameData * pData;
	char buf[256];

	pData = new CGameData;
	memset(buf, 0x00, sizeof(buf));
	util::del( m_pSky );
	m_pSky = new CSkinnedMesh(FALSE);
	m_pSky->InitDeviceObjects();
	sprintf( buf, ".\\Res-Map\\%s.sky", szSkyBoxName );

	if(m_pSky && pData->SetFile(buf,NULL,NULL,NULL) )
		m_pSky->LoadMeshHierarchyFromMem( pData );
	else
		util::del(m_pSky); // 낮 스카이 박스 없음
	util::del( pData );

	pData = new CGameData;
	memset(buf, 0x00, sizeof(buf));
	util::del( m_pSkyNight );
	m_pSkyNight = new CSkinnedMesh(FALSE);
	m_pSkyNight->InitDeviceObjects();
	sprintf( buf, ".\\Res-Map\\%s.sky", szSkyBoxName );

	if(m_pSkyNight && pData->SetFile(buf,NULL,NULL,NULL) )
		m_pSkyNight->LoadMeshHierarchyFromMem( pData );
	else
		util::del(m_pSkyNight); // 밤 스카이 없음
	util::del( pData );
}
// end 2010. 03. 15 by ckPark 인피니티 필드 2차(연출 처리)