// SceneData.cpp: implementation of the CSceneData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SceneData.h"
#include "AtumApplication.h"


#include "Frustum.h"
#include "Cinema.h"
#include "Background.h"
//#include "ObjectRender.h"
#include "ObjectChild.h"
//#include "MonsterRender.h"
#include "SunRender.h"
#include "SunData.h"
#include "RainRender.h"
#include "RainData.h"
#include "ETCRender.h"
#include "Weapon.h"
#include "ItemData.h"
#include "MonsterData.h"
#include "EnemyData.h"
#include "UnitRender.h"
#include "CharacterRender.h"					// 2005-07-21 by ispark
#include "Camera.h"
#include "QuadGround.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "dxutil.h"
//#include "WeaponMonsterData.h"
#include "FieldWinSocket.h"
#include "ObjRender.h"
#include "MonRender.h"
#include "AtumDatabase.h"
#include "WeaponItemInfo.h"
#include "INFGameMain.h"
#include "INFSkill.h"
#include "FxSystem.h"
#include "TutorialSystem.h"
#include "WeaponMissileData.h"			// 2005-07-19 by ispark
#include "Water.h"
#include "INFCityBoard.h"			// 2006-04-11 by ispark
#include "Chat.h"
#include "StoreData.h"
// 2013-06-19 by bhsohn ¿¬½Â ¹öÇÁ »óÁ¡À» ¿ÀÇÂÇÑ »óÅÂ¿¡¼­ ¿¬½ÂÀÌ ±úÁú ½Ã ¿¬½Â ¹öÇÁ »óÁ¡ÀÌ Á¾·á
#include "Interface.h"		
#include "INFCityBase.h"
// END 2013-06-19 by bhsohn ¿¬½Â ¹öÇÁ »óÁ¡À» ¿ÀÇÂÇÑ »óÅÂ¿¡¼­ ¿¬½ÂÀÌ ±úÁú ½Ã ¿¬½Â ¹öÇÁ »óÁ¡ÀÌ Á¾·á

#define BIG_OBJECT_RADIUS_TO_I_DISTANCE			320
#define NORMAL_OBJECT_RADIUS_TO_I_DISTANCE		320

#define OPTION_GAMMA_PLUS 0.05f

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

float g_fAltitudeApplyMin = 600.0f;
float g_fAltitudeApplyMax = 1600.0f;
float g_fAltitudeDestRate = 0.2f;

// 2007-05-17 by bhsohn ¿ÀºêÁ§Æ® µÚ¿¡ ¼û¾úÀ»½Ã ¿¡ ´ëÇÑ Ã³ °Ë»ç Ã³¸®
#define TILE_CHECK_DISTANCE			100.0f		// 100¹ÌÅÍ
#define MAX_TILE_CHECK_CNT			100			// 100¹ÌÅÍ*100 = 10000¹ÌÅÍ(ÃÖ´ë»ç°Å¸®)
#define MIN_TILE_CHECK_DISTANCE		20.0f		// ÃÖ¼Ò °Å¸®

#define	MIN_TILE_DIFF_CHECK_HEIGHT		100.0f		// ÃÖ¼Ò ³ôÀÌ 100¹ÌÅÍ
#define	MAX_TILE_DIFF_CHECK_HEIGHT		500.0f		// ÃÖ¼Ò ³ôÀÌ 100¹ÌÅÍ
#define INVENTORY_MESSAGE_GAP			30.0f


// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
#define WATER_BUMP_TEXTURE_SIZE_X		256
#define WATER_BUMP_TEXTURE_SIZE_Y		256
// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ


struct CompareUnit
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		if(pPtr1->m_fDistanceCamera > pPtr2->m_fDistanceCamera)
		{
			return true;
		}
		return false;
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneData::CSceneData()
{
	FLOG("CSceneData::CSceneData()");
	g_pScene = this;
	m_pFrustum = NULL;
	m_pItemData = NULL;
	m_pWeaponData = NULL;
	m_pGround = NULL;		
	m_pObjectRender = NULL;
	m_pMonsterRender = NULL;
	m_pETCRender = NULL;
	m_pWater = NULL;
	m_dwFogColor = D3DCOLOR_ARGB(0,0,0,0);	
	m_fFogStartValue = WEATHER_SUNNY_FOG_START;
	m_fFogEndValue = WEATHER_SUNNY_FOG_END;	
	m_fBeforeFogStartValue = 0;
	m_fBeforeFogEndValue = 0;
	m_bFog = TRUE;			
	m_bFogStay = FALSE;		
	memset((char*)&m_light0,0x00,sizeof(D3DLIGHT9));		
	memset((char*)&m_light1,0x00,sizeof(D3DLIGHT9));		
	memset((char*)&m_light2,0x00,sizeof(D3DLIGHT9));		
	memset((char*)&m_light3,0x00,sizeof(D3DLIGHT9));		
	m_bNight = FALSE;

	m_pSunRender = NULL;
	m_pSunData = NULL;
	m_pData = NULL;
	memset((char*)m_pCreateTexture, 0x00, sizeof(DWORD)*TEXTILE_NUM);
	m_bIsRestore = TRUE;
	m_fSkyRedColor = 0;
	m_fSkyGreenColor = 0;
	m_fSkyBlueColor = 0;
	m_pRainRender = NULL;	
	m_pRainList = NULL;	
	m_byWeatherType = 0;	
	m_dwStartTime = 0;	
	m_nBaseTime = 0;	
	m_byMapType = 0;		
	m_vecEnemyBlockList = NULL;
	m_vecMonsterList = NULL;
	m_nBlockSizeX = 0;
	m_nBlockSizeY = 0;

	m_bChangeWeather = TRUE;
	m_fChangeWeatherCheckTime = 0;

//	m_fFogDestStartValue = WEATHER_SUNNY_FOG_START;
//	m_fFogDestEndValue = WEATHER_SUNNY_FOG_END;
	m_fOrgFogStartValue = WEATHER_SUNNY_FOG_START;
	m_fOrgFogEndValue = WEATHER_SUNNY_FOG_END;
	m_pCinemaData = NULL;		/// ½Ã³×¸¶ µ¥ÀÌÅÍ 

	// 2005-01-20 by jschoi 
	m_fAlphaSky = 1.0f;		// ½ºÄ«ÀÌ ¹Ú½º ³·,¹ã È¥ÇÕºñÀ²

	// 2005-07-11 by ispark
	m_nMaxAtitudeHeight = ALTITUDE_APPLY_MAX;
	m_bWaterShaderRenderFlag = FALSE;

	m_fGetItemAllDelay = 0.0f;
	m_fGetItemMessage	= 0.0f;

	m_vecAlphaEffectRender.clear();
	m_vecAlphaUnitRender.clear();
	m_vecScanData.clear();

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	m_pWaterBumpTexture		= NULL;
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

	// 2012-04-18 by mspark, ¿ÀºêÁ§Æ® ¸®¼Ò½º Á¤¸®
#ifdef C_OBJECT_RESOURCE_ORGANIZE_MSPARK
	FILE* fileMesh = _tfopen( _T("(Obj)MeshRender.txt"), _T("w") );
	fclose(fileMesh);
	
	FILE* fileMon = _tfopen( _T("(Obj)MonsterData.txt"), _T("w") );
	fclose(fileMon);
#endif
	// end 2012-04-18 by mspark, ¿ÀºêÁ§Æ® ¸®¼Ò½º Á¤¸®
}

CSceneData::~CSceneData()
{
	FLOG("CSceneData::~CSceneData()");
 	g_pScene = NULL;

	util::del( m_pFrustum );
	util::del( m_pCinemaData );	/// ½Ã³×¸¶ µ¥ÀÌÅÍ
	util::del( m_pItemData );
	util::del( m_pWeaponData );
	util::del( m_pRainRender );
	util::del( m_pRainList );
	util::del( m_pData );
	util::del( m_pSunRender );
	util::del( m_pSunData );
	util::del( m_pGround );
 	util::del( m_pObjectRender );
 	util::del( m_pMonsterRender );
 	util::del( m_pETCRender );
	util::del( m_pWater );
	if(m_vecEnemyBlockList)
	{
		for(int i = 0; i < m_nBlockSizeX;i++)
		{
			for(int j = 0;j < m_nBlockSizeY;j++)
			{
				m_vecEnemyBlockList[i*m_nBlockSizeY + j].clear();
				//util::del(m_vecEnemyBlockList[i*m_nBlockSizeY + j]);
			}
		}
		util::del_array(m_vecEnemyBlockList);
	}
	if(m_vecMonsterList)
	{
		for(int i = 0; i < m_nBlockSizeX;i++)
		{
			for(int j = 0;j < m_nBlockSizeY;j++)
			{
				m_vecMonsterList[i*m_nBlockSizeY + j].clear();
				//util::del(m_vecMonsterList[i*m_nBlockSizeY + j]);
			}
		}
		util::del_array(m_vecMonsterList);
	}
	CMapMonsterIterator itMonster = m_mapMonsterList.begin();
	while(itMonster != m_mapMonsterList.end())
	{
		util::del(itMonster->second);
		itMonster++;
	}
	m_mapMonsterList.clear();
	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
		util::del(itEnemy->second);
		itEnemy++;
	}
	m_mapEnemyList.clear();

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	SAFE_RELEASE(m_pWaterBumpTexture);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

}

HRESULT CSceneData::InitDeviceObjects()
{
	FLOG("CSceneData::InitDeviceObjects()");
//	m_pObjectRender = new CObjectRender();
	m_pObjectRender = new CObjRender();
	m_pMonsterRender = new CMonRender();
	m_pSunRender = new CSunRender();
	m_pRainRender = new CRainRender();
	m_pETCRender = new CETCRender();
	m_pFrustum = new CFrustum();

	if(m_bWaterShaderRenderFlag)
	{
		m_pWater = new CWater();
	}
	LoadCinemaFile();						/// ½Ã³×¸¶ ÆÄÀÏ ·Îµå ÇÏ±â

	return S_OK;
}

HRESULT CSceneData::RestoreDeviceObjects()
{
	FLOG("CSceneData::RestoreDeviceObjects()");
	if( g_pD3dApp->m_dwGameState == _GAME || 
		g_pD3dApp->m_dwGameState == _SHOP || 
		g_pD3dApp->m_dwGameState == _CITY || 
		g_pD3dApp->m_dwGameState == _MAPLOAD)
	{
		if(m_pGround)
			m_pGround->RestoreDeviceObjects();
		if(m_pETCRender)
			m_pETCRender->RestoreDeviceObjects();
		if(m_pWater && m_bWaterShaderRenderFlag)
			m_pWater->RestoreDeviceObjects();
		RestoreRes();
	}
	if(g_pD3dApp->m_dwGameState == _GAME || g_pD3dApp->m_dwGameState == _SHOP)
	{
		CMapMonsterIterator itMonster = m_mapMonsterList.begin();
		while(itMonster != m_mapMonsterList.end())
		{
//			itMonster->second->RestoreChat();
			itMonster->second->RestoreDeviceObjects();
			itMonster++;
		}
		CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
		while(itEnemy != m_mapEnemyList.end())
		{
//			itEnemy->second->RestoreChat();
			itEnemy->second->RestoreDeviceObjects();
			itEnemy++;
		}

		// 2004-10-20 by jschoi µð¹ÙÀÌ½º¸¦ ÀÒÀº ÈÄ¿¡´Â ¾È°³ ¼¼ÆÃÀ» ´Ù½Ã ÇØ¾ßÇÑ´Ù.
		m_bChangeWeather = TRUE;
	}

	
	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	// ¹üÇÁ ÅØ½ºÃÄ »ý¼º 256 * 256 »çÀÌÁî
	if(NULL == m_pWaterBumpTexture)
    {
		if( FAILED( g_pD3dDev->CreateTexture( WATER_BUMP_TEXTURE_SIZE_X, WATER_BUMP_TEXTURE_SIZE_Y, 1, 0,
			D3DFMT_V8U8, D3DPOOL_MANAGED, &m_pWaterBumpTexture, NULL ) ) )
		{
			SAFE_RELEASE(m_pWaterBumpTexture);
			return E_FAIL;
		}
	}
	
    D3DLOCKED_RECT lrDst;
    m_pWaterBumpTexture->LockRect( 0, &lrDst, 0, 0 );
    BYTE* pDst       = (BYTE*)lrDst.pBits;
	

	for( DWORD y=0; y<WATER_BUMP_TEXTURE_SIZE_Y; y++ )
    {
        for( DWORD x=0; x<WATER_BUMP_TEXTURE_SIZE_X; x++ )
        {
			CHAR iDu	= (CHAR)( sinf(x / (float)(WATER_BUMP_TEXTURE_SIZE_X) * D3DX_PI * 2.0f) * 16.0f);
			CHAR iDv	= (CHAR)( cosf(x / (float)(WATER_BUMP_TEXTURE_SIZE_X) * D3DX_PI * 2.0f) * 32.0f);
			
			pDst[2*x + 0] = iDu;
            pDst[2*x + 1] = iDv;
        }
        pDst += lrDst.Pitch;
    }
	
    m_pWaterBumpTexture->UnlockRect(0);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

	return S_OK;
}

HRESULT CSceneData::InvalidateDeviceObjects()
{
	FLOG("CSceneData::InvalidateDeviceObjects()");
	if( g_pD3dApp->m_dwGameState == _GAME || 
		g_pD3dApp->m_dwGameState == _SHOP || 
		g_pD3dApp->m_dwGameState == _CITY || 
		g_pD3dApp->m_dwGameState == _MAPLOAD)
	{
		InvalidateRes();
	}

	m_vecAlphaEffectRender.clear();
	m_vecAlphaUnitRender.clear();
	m_vecScanData.clear();

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	SAFE_RELEASE(m_pWaterBumpTexture);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			HRESULT CSceneData::DeleteDeviceObjects()
/// \brief		
/// \author		dhkwon
/// \date		2004-03-25 ~ 2004-03-25
/// \warning	CBackground¸¦ DeleteRes()¿¡¼­ Áö¿ìÁö ¾Ê°í ¹Û¿¡¼­ Áö¿ì´Â ÀÌÀ¯´Â
///				°ÔÀÓ ¸®½ºÅ¸Æ® ÈÄ ¹Ù·Î Àç½ÃÀÛÇÒ °æ¿ì ¸ÊÀÌ °°À» °æ¿ì¸¦ ´ëºñÇÏ¿© ¸Þ¸ð¸®¿¡¼­
///				³»¸®Áö ¾Ê°Ô ÇÏ±â À§ÇØ¼­ÀÌ´Ù.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
HRESULT CSceneData::DeleteDeviceObjects()
{
	FLOG("CSceneData::DeleteDeviceObjects()");
	DBGOUT("Pre CSceneData::DeleteDeviceObjects()\n");
 	util::del( m_pItemData );
	util::del( m_pWeaponData );

	DeleteCinemaFile();							/// ½Ã³×¸¶ ÆÄÀÏ ÀüºÎ Áö¿ì±â
	DeleteRes();
	DeleteTexTileDevice();
	if(m_pGround)
	{
		m_pGround->DeleteDeviceObjects();
	}

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	SAFE_RELEASE(m_pWaterBumpTexture);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

	DBGOUT("End CSceneData::DeleteDeviceObjects()\n");
	return S_OK;
}

void CSceneData::RenderCity()
{
	g_pD3dDev->LightEnable( 1, FALSE );
	g_pD3dDev->LightEnable( 2, FALSE );
	g_pD3dDev->LightEnable( 3, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  TRUE );
	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );	
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);


	// Vertex Fog Use
	g_pD3dDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
	g_pD3dDev->SetRenderState( D3DRS_FOGSTART,   FtoDW(m_fOrgFogStartValue) ); //
	g_pD3dDev->SetRenderState( D3DRS_FOGEND,     FtoDW(m_fOrgFogEndValue) );
	g_pD3dDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
	g_pD3dDev->SetRenderState( D3DRS_RANGEFOGENABLE, TRUE );
	g_pD3dDev->SetFVF( D3DFVF_FOGVERTEX );
	////////////////////////////////////////////////////////////////////////
//	m_dwFogColor = GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,!m_bNight);
	if(m_bNight)
	{
		m_dwFogColor = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)->NightFogColor;
	}
	else
	{
		m_dwFogColor = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)->DayFogColor;
	}

	g_pD3dDev->SetRenderState( D3DRS_FOGCOLOR,  m_dwFogColor );



	// Object Rendering List -> Render
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );

	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	
	g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
	g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x08 );
	
	vectorCObjectChildPtr::iterator itObj(m_vectorCulledObjectPtrList.begin());
	while(itObj != m_vectorCulledObjectPtrList.end())
	{
		(*itObj)->Render();
		itObj++;
	}

	// Shadow Render
	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );

	g_pD3dApp->m_pUnitRender->RenderShadow(g_pShuttleChild);

}

void CSceneData::Render()
{
	FLOG("CSceneData::Render()");
	g_pD3dDev->LightEnable( 2, FALSE );
	g_pD3dDev->LightEnable( 3, FALSE );
	g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
	g_pD3dDev->SetFVF( D3DFVF_FOGVERTEX );
	g_pD3dDev->SetRenderState( D3DRS_FOGCOLOR, m_dwFogColor );

	// Shadow Render
	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );

	if(g_pSOption->sShadowState > 0)
	{
		CVecMonsterIterator itMonsterShadowRender = m_vecMonsterShadowRenderList.begin();
		while(itMonsterShadowRender != m_vecMonsterShadowRenderList.end())
		{
			(*itMonsterShadowRender)->RenderShadow();
			itMonsterShadowRender++;
		}
	}

	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);


	// Object Rendering List -> Render
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
	g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE,  TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_FOGENABLE,  IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );

	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	
	g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL );
	g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x08 );

	g_bDetailDrawFrame = TRUE;	// 2005-01-06 by jschoi - ¼¼ºÎ ÄÃ¸µ ±â´ÉÀ» »ç¿ë
	vectorCObjectChildPtr::iterator itObj(m_vectorCulledObjectPtrList.begin());
	while(itObj != m_vectorCulledObjectPtrList.end())
	{		
//		if(g_pCharacterChild->GetPickingObj() == (*itObj)->m_pObjectInfo->RenderIndex)
		if(g_pCharacterChild->GetPickingObj() == (*itObj))
		{
			g_pD3dDev->LightEnable( 2, TRUE );
			g_pD3dDev->LightEnable( 3, TRUE );
		}		
		(*itObj)->Render();
		itObj++;

		g_pD3dDev->LightEnable( 2, FALSE );
		g_pD3dDev->LightEnable( 3, FALSE );
	}
	g_bDetailDrawFrame = FALSE;	// 2005-01-06 by jschoi - ¼¼ºÎ ÄÃ¸µ ±â´ÉÀ» º¹±¸(»ç¿ë ¾ÈÇÔ)

#ifdef _DRAW_EVENTS
	if (g_pD3dApp->m_bDrawEvents) //2015-01-29 by st0rmy, Draw events like spawn spots, warp event objects etc
	{
		// 2005-04-12 by jschoi - 이벤트 오브젝트 보여주기
		// 2005-11-16 by ispark - 관리자만 보여주기
		if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION) &&
			g_pGround->m_pObjectEvent && g_pGround->m_pObjectEvent->m_pChild)
		{
			CObjectChild* pObject = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
			while (pObject)
			{
				pObject->Render();
				pObject = (CObjectChild *)pObject->m_pNext;
			}
		}
	}
#else

#ifdef _DEBUG
	// 2005-04-12 by jschoi - ÀÌº¥Æ® ¿ÀºêÁ§Æ® º¸¿©ÁÖ±â
	// 2005-11-16 by ispark - °ü¸®ÀÚ¸¸ º¸¿©ÁÖ±â
	if(g_pD3dApp->m_bEventObjectRender && 
		COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION) && 
		g_pGround->m_pObjectEvent && g_pGround->m_pObjectEvent->m_pChild)
	{
		CObjectChild* pObject = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
		while(pObject)
		{
			pObject->Render();
			pObject = (CObjectChild *)pObject->m_pNext;
		}
	}
#endif
#endif
	
	// Shadow Render
	// 2005-01-03 by jschoi
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );		
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
//	g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);

	
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE );
	
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, FALSE );

	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_pD3dDev->SetRenderState( D3DRS_ALPHAREF,  0x00000008 );
	g_pD3dDev->SetRenderState( D3DRS_ALPHAFUNC,  D3DCMP_GREATER );

	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	g_pD3dDev->SetFVF( D3DFVF_SPRITEVERTEX );

	// 2005-08-09 by ispark
	// ±×¸²ÀÚ ·»´õ¸µÀ» À§ÇÑ ÄÚµå
	g_pD3dDev->SetRenderState (D3DRS_SLOPESCALEDEPTHBIAS, 0);
	g_pD3dDev->SetRenderState (D3DRS_DEPTHBIAS, 0);

	float fOffset = 0.0f;
	if(g_pGameMain->m_pCityBoard)
	{
		g_pGameMain->m_pCityBoard->Render(&fOffset);
	}

	// 2005-07-13 by ispark
	// Ä³¸¯ÅÍ ±×¸²ÀÚÀÎÁö À¯´Ö ±×¸²ÀÚÀÎÁö ÆÇ´Ü
	if(g_pD3dApp->m_bCharacter)
	{
		// Ä³¸¯ÅÍ ±×¸²ÀÚ´Â Z¹öÆÛ¸¦ »ç¿ëÇÑ´Ù.
		// 2006-04-07 by ispark, ¹Ø¿¡ ¼Ò½º¸¦ Áö¿ì°í À§¿¡ ¾ËÆÄ 2ÁÙ·Î¼­ ÀÌÆåÆ®¿Í Á¶È­°¡ ÀÌ·ç¾îÁ³´Ù.
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
		g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(0.0f));
		fOffset += -0.00002f;
		g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(fOffset));
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
		g_pD3dApp->m_pCharacterRender->RenderShadow();
		g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE,  TRUE );
	}
	else
	{
		if(g_pShuttleChild->m_nAlphaValue == SKILL_OBJECT_ALPHA_NONE)
		{
			g_pD3dApp->m_pUnitRender->RenderShadow(g_pShuttleChild);
		}
	}

	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );				// 2005-08-04 by ispark
	g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, 0);

	if(g_pSOption->sShadowState)
	{
		CVecEnemyIterator itEnemy = m_vecEnemyShadowRenderList.begin();
		while(itEnemy != m_vecEnemyShadowRenderList.end())
		{
			g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  FALSE );		// 2005-08-04 by ispark
			g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
			g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, 0);

			// 2005-07-13 by ispark
			// Àû Ä³¸¯ÅÍ ±×¸²ÀÚÀÎÁö À¯´Ö ±×¸²ÀÚÀÎÁö ÆÇ´Ü
			CUnitData * pData = (CUnitData *)(*itEnemy);
			CEnemyData * pEnemy = (CEnemyData *)(*itEnemy);
			if(pEnemy->m_bEnemyCharacter)
			{
				// 2006-04-07 by ispark, ¹Ø¿¡ ¼Ò½º¸¦ Áö¿ì°í À§¿¡ ¾ËÆÄ 2ÁÙ·Î¼­ ÀÌÆåÆ®¿Í Á¶È­°¡ ÀÌ·ç¾îÁ³´Ù.
				g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
				g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(0.0f));
				fOffset += -0.00002f;
				g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(fOffset));
				g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
				g_pD3dApp->m_pCharacterRender->RenderShadow(*itEnemy);
				g_pD3dDev->SetRenderState( D3DRS_ZWRITEENABLE,  TRUE );
			}
			else
			{
				if((*itEnemy)->m_nAlphaValue == SKILL_OBJECT_ALPHA_NONE)
				{
					g_pD3dApp->m_pUnitRender->RenderShadow(*itEnemy);
				}
			}

			itEnemy++; 
		}
	}


	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,  TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );	
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	g_pD3dDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, F2DW(0.0f));
	g_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, F2DW(0.0f));

/*	
	// Enemy Rendering List -> Render
	CVecEnemyIterator itEnemyRender = m_vecEnemyRenderList.begin();
	while(itEnemyRender != m_vecEnemyRenderList.end())
	{
		(*itEnemyRender)->Render();
		itEnemyRender++;
	}
	
	// Monster Rendering List -> Render
	CVecMonsterIterator itMonsterRender = m_vecMonsterRenderList.begin();
	while(itMonsterRender != m_vecMonsterRenderList.end())
	{
		(*itMonsterRender)->Render();
		itMonsterRender++;
	}
*/
	// 2006-07-29 by ispark, °³ÀÎ »óÁ¡ Picking
	m_vecAlphaUnitRender.clear();
	vector<CUnitData*>::iterator itUnit = m_vecUnitRenderList.begin();
	while(itUnit != m_vecUnitRenderList.end())
	{
		// 2006-11-16 by ispark, ¾ËÆÄ ·»´õ¸µ Ã£±â
		if((*itUnit)->m_nAlphaValue < SKILL_OBJECT_ALPHA_NONE)
		{
			m_vecAlphaUnitRender.push_back(*itUnit);
		}
		else
		{
			if(g_pCharacterChild->GetPickingBazaar() == (*itUnit))
			{
				g_pD3dDev->LightEnable( 2, TRUE );
				g_pD3dDev->LightEnable( 3, TRUE );
			}		
			(*itUnit)->Render();

			g_pD3dDev->LightEnable( 2, FALSE );
			g_pD3dDev->LightEnable( 3, FALSE );
		}

		itUnit++;
	}

	// 2005-07-22 by ispark
	if(g_pD3dApp->m_bCharacter) 
		g_pCharacterChild->Render();

	if(m_bWaterShaderRenderFlag)
	{
		m_pWater->Render();
	}
	else
	{
		RenderWater();
	}

	if(m_pRainRender)
		m_pRainRender->Render();
	if(	m_byWeatherType != WEATHER_RAINY &&			// ºñ
		m_byWeatherType != WEATHER_SNOWY &&			// ´«
		m_byWeatherType != WEATHER_CLOUDY &&		// ±¸¸§³¤
		m_byWeatherType != WEATHER_FOGGY &&			// ¾È°³³¤
		IsSunRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		if( m_pSunData && !m_bNight)	// ÅÂ¾çÀÇ ·»ÁîÇÃ·¹¾î
			m_pSunData->RenderRens();
	}	

	if(	g_pShuttleChild )
	{
		if(g_pShuttleChild->m_pSecondaryWeapon)
		{
			if(g_pShuttleChild->m_pSecondaryWeapon->GetAttackMode() == ATT_TYPE_GROUND_BOMBING_SEC ||
				g_pShuttleChild->m_pSecondaryWeapon->GetAttackMode() == ATT_TYPE_AIR_BOMBING_SEC) 
				g_pGameMain->m_pInfSkill->RenderGroundTarget();
		}		
	}

	// 2005-02-16 by jschoi - ¸Ê »ó ¿ÀºêÁ§Æ®ÀÇ ÀÌ¸§ ·»´õ¸µ
	vectorCObjectChildPtr::iterator itNameObj(m_vectorCulledObjectPtrList.begin());
	while(itNameObj != m_vectorCulledObjectPtrList.end())
	{
		(*itNameObj)->ObjectNameRender();
		itNameObj++;
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void TickScanObeject()
/// \brief		½ºÄµ ¿ÀºêÁ§Æ®ÀÇ À¯È¿½Ã°£ÀÌ ´ÙµÇ¸é »èÁ¦½ÃÅ²´Ù.
/// \author		dgwoo
/// \date		2007-02-12 ~ 2007-02-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::TickScanObeject()
{
	vector<CItemData*>::iterator it = m_vecScanData.begin();
	while(it != m_vecScanData.end())
	{
		if((*it)->m_fItemCheckTime <= 0)
		{// À¯È¿½Ã°£ÀÌ ´ÙµÇ¾î »èÁ¦ ½ÃÅ²´Ù.
			util::del(*it);
			 it = m_vecScanData.erase(it);
		}
		else
		{// 
			(*it)->Tick();
			it++;
		}
	}

}

void CSceneData::Tick()
{
	FLOG("CSceneData::Tick()");

	// 2006-07-18 by ispark, ¾ÆÀÌÅÛ ½Àµæ µô·¹ÀÌ ½Ã°£
	m_fGetItemAllDelay += g_pD3dApp->GetElapsedTime();
	if(m_fGetItemAllDelay > GET_ITEM_IN_TIME + 1.0f)
	{
		// 2ÃÊ¸¦ ³ÑÁö ¾Ê°Ô ÇÑ´Ù.
		m_fGetItemAllDelay = GET_ITEM_IN_TIME + 1.0f;
	}
	if(m_fGetItemMessage >= 0)
    {
	   m_fGetItemMessage -= g_pD3dApp->GetElapsedTime();

    }
	
	

	// 2005-04-01 by jschoi
	if (!g_pTutorial->IsTutorialMode() || g_pTutorial->IsUseShuttleTick())
	{
		if(m_pItemData) m_pItemData->Tick();
		
#ifdef PARRALELLE_WEAPON_EFFET_PROCESSING
		if (m_pWeaponData) m_pWeaponData->TickParallel();
#else
		if(m_pWeaponData) m_pWeaponData->Tick();
#endif
	}
	
	// 2010. 03. 05 by jskim ¸Ê ·Îµù Áß À©µµ¿ì È­¸é ÀüÈ¯½Ã ÁöÇü »ç¶óÁö´Â ¹ö±× ¼öÁ¤
// 	if(m_pGround)
// 	{
// 		m_pGround->Tick(g_pD3dApp->GetElapsedTime());
// 		if(m_pGround->m_pQuad  && IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) )
// 		{
// 			m_pGround->m_pQuad->Tick();
// 		}
// 	}
	if(m_pGround)
	{
		m_pGround->Tick(g_pD3dApp->GetElapsedTime());
		if(m_pGround->m_pQuad  && IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) )
		{
			if(!g_pD3dApp->IsDeviceLost())
				m_pGround->m_pQuad->Tick();
		}
 	}
	//end 2010. 03. 05 by jskim ¸Ê ·Îµù Áß À©µµ¿ì È­¸é ÀüÈ¯½Ã ÁöÇü »ç¶óÁö´Â ¹ö±× ¼öÁ¤
	CheckDay();
	CheckWeather();
	if(m_bWaterShaderRenderFlag)
		m_pWater->Tick();

	//2004 - 06 - 4 add by jsy
	//CheckObjectRenderList();


//	if(m_pSunData)
	if(	m_pSunData &&
		m_byWeatherType != WEATHER_RAINY &&			// ºñ
		m_byWeatherType != WEATHER_SNOWY &&			// ´«
//		m_byWeatherType != WEATHER_CLOUDY &&		// ±¸¸§³¤
		m_byWeatherType != WEATHER_FOGGY &&			// ¾È°³³¤
		IsSunRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		m_pSunData->Tick();
	}
	if(m_pRainList)
	{
		m_pRainList->Tick();
	}


	// 2005-04-21 by jschoi - Tutorial
	if(	g_pTutorial->IsTutorialMode() == FALSE ||
		g_pTutorial->IsUseShuttleTick() == TRUE )
	{
		int nDel = 0;
		m_vecUnitRenderList.clear();
		m_vecEnemyRenderList.clear();
		m_vecEnemyShadowRenderList.clear();
		CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
		while(itEnemy != m_mapEnemyList.end())
		{
			CEnemyData* pEnemy = itEnemy->second;
			ASSERT_ASSERT(pEnemy);
			pEnemy->Tick();
			if(!pEnemy->m_bUsing)
			{
				if(pEnemy->m_infoCharacter.CharacterInfo.ClientIndex == g_pShuttleChild->m_stObserve.ClientIndex)
				{// 2007-06-19 by dgwoo À¯Àú°¡ °­Á¦ Á¾·á³ª ÀÌµ¿½Ã ¿ÉÀú¹ö¸¦ ¾ÈÀüÇÏ°Ô ÇØÁ¦ÇÏ±â À§ÇØ.
					g_pShuttleChild->ObserveCancelUpdateInfo();
					g_pShuttleChild->ObserveEnd();
				}
				nDel = pEnemy->m_infoCharacter.CharacterInfo.ClientIndex;
			}
			itEnemy++;
		}
		if(nDel)
		{
			itEnemy = m_mapEnemyList.find(nDel);
			if(itEnemy != m_mapEnemyList.end())
			{
				DeleteFieldItemOfUnitData( itEnemy->second );
				DeleteToBlockData(itEnemy->second);
				util::del(itEnemy->second);
				m_mapEnemyList.erase(itEnemy);
			}
		}
		nDel = 0;
		m_vecMonsterRenderList.clear();
		m_vecMonsterShadowRenderList.clear();

		for (auto& monster : m_mapMonsterList)
		{
			monster.second->Tick();

			if (!monster.second->m_bUsing)
				
				nDel = monster.second->m_info.MonsterIndex;
		}

		if (nDel)
		{
   			auto itMonster = m_mapMonsterList.find(nDel);

			if (itMonster != m_mapMonsterList.end())
			{
				DeleteFieldItemOfUnitData(itMonster->second);
				DeleteToBlockData(itMonster->second);
				util::del(itMonster->second);
				m_mapMonsterList.erase(itMonster);
			}
		}		
		// 2004-12-16 by jschoi - ShuttleChildµµ Æ÷ÇÔ
		// 2005-07-28 by ispark Ä³¸¯ÅÍÀÏ¶§´Â ÂïÁö ¸¶¶ó
		if(g_pD3dApp->m_bCharacter == FALSE)
		{
			m_vecUnitRenderList.push_back((CUnitData*)g_pShuttleChild);
			sort(m_vecUnitRenderList.begin(), m_vecUnitRenderList.end(), CompareUnit() );
		}
		
		// 2007-02-12 by dgwoo ½ºÄµ°ü·Ã ¿ÀºêÁ§Æ® Tick
		TickScanObeject();
	}

	// 2005-04-22 by jschoi - Tutorial
	if(m_vecUnitRenderList.empty())
	{
		if(g_pD3dApp->m_bCharacter == FALSE)
		{
			m_vecUnitRenderList.push_back((CUnitData*)g_pShuttleChild);
		}
	}

}

BOOL CSceneData::InitBackground()
{
	FLOG("CSceneData::InitBackground()");
	int i;
	// <Game Start>   or   <Game ReStart>  ½Ã ¿©·¯ ÅØ½ºÃÄµéÀÇ Restore
	if(m_bIsRestore)
	{
		// ¿ÀºêÁ§Æ® ¹× ¸ó½ºÅÍ ±âÅ¸ EffectÀÇ ·Îµå
		InitRes();
		RestoreRes();
		m_bIsRestore = FALSE;
	}
	FILE * readMap;
	int re = -1;
	WORKSPACE iWorkspace;
	PROJECTINFO iProject;
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_MAP, RC_MAP_WORKSPACE );
	if( strlen( strPath ) > 0 )
	{
 		readMap = fopen(strPath,"rb");
		fseek(readMap,20,SEEK_SET);
		fread(&iWorkspace,sizeof(WORKSPACE),1,readMap);
		int a = iWorkspace.numberOfProject;
		for(i=0;i<a;i++)
		{
			fread(&iProject,sizeof(PROJECTINFO),1,readMap);
			char buf[32];
			snprintf(buf, 32, "%04d", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);

			if (strcmp(iProject.strProjectName, buf) == 0)
			{
				DBGOUT("MAP Project Info[name:%s]\n   [Water Height:%.2f]\n   Day[D:%.2f,%.2f,%.2f][A:%.2f,%.2f,%.2f]\n   Night[D:%.2f,%.2f,%.2f][A:%.2f,%.2f,%.2f]\n", 
					iProject.strProjectName, iProject.fWaterHeight, 
					iProject.fDiffuseR1, iProject.fDiffuseG1, iProject.fDiffuseB1,
					iProject.fAmbientR1, iProject.fAmbientG1, iProject.fAmbientB1,
					iProject.fDiffuseR2, iProject.fDiffuseG2, iProject.fDiffuseB2,
					iProject.fAmbientR2, iProject.fAmbientG2, iProject.fAmbientB2
					);

				if (strcmp(iProject.strProjectName, "0100") == 0)
					m_byMapType = MAP_TYPE_TUTORIAL;
				else m_byMapType = MAP_TYPE_NORMAL_FIELD;

				break;
			}
		}
		fclose(readMap);
	}
	else
		return FALSE;// error
	if(re==0)
	{
		if(m_pGround)
		{
			if(m_vecEnemyBlockList)
			{
				for(int i = 0; i < m_nBlockSizeX;i++)
				{
					for(int j = 0;j < m_nBlockSizeY;j++)
					{
						m_vecEnemyBlockList[i*m_nBlockSizeY + j].clear();
						//util::del(m_vecEnemyBlockList[i*m_nBlockSizeY + j]);
					}
				}
				util::del_array(m_vecEnemyBlockList);
			}
			if(m_vecMonsterList)
			{
				for(int i = 0; i < m_nBlockSizeX;i++)
				{
					for(int j = 0;j < m_nBlockSizeY;j++)
					{
						m_vecMonsterList[i*m_nBlockSizeY + j].clear();
						//util::del(m_vecMonsterList[i*m_nBlockSizeY + j]);
					}
				}
				util::del_array(m_vecMonsterList);
			}
/*			if(m_vecObjectList)
			{
				for(int i = 0; i < m_nBlockSizeX;i++)
				{
					for(int j = 0;j < m_nBlockSizeY;j++)
					{
						m_vecObjectList[i*m_nBlockSizeY + j].clear();
					}
				}
				util::del_array(m_vecObjectList);
			}
*/		}
		m_nBlockSizeX = iProject.sXSize/3;
		m_nBlockSizeY = iProject.sYSize/3;
		if(iProject.sXSize%3)
			m_nBlockSizeX++;
		if(iProject.sYSize%3)
			m_nBlockSizeY++;
		m_vecEnemyBlockList = new CVecEnemyList[m_nBlockSizeX*m_nBlockSizeY];
		m_vecMonsterList = new CVecMonsterList[m_nBlockSizeX*m_nBlockSizeY];
//		m_vecObjectList = new CVecObjectList[m_nBlockSizeX*m_nBlockSizeY];

		int checkMapname = -1;
		if(m_pGround)
		{
			char buf[32];
			wsprintf( buf, "%04d", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
			checkMapname = strcmp(m_pGround->m_projectInfo.strProjectName, buf);
		}
	//	if(checkMapname != MAPNAME_CHECK_SUCCESS)
		{
			EnterCriticalSection(&g_pD3dApp->m_cs);
			if(m_pGround)
			{
				m_pGround->DeleteDeviceObjects();
				util::del(m_pGround);
			}
			if(m_pETCRender)
			{
				m_pETCRender->DeleteDeviceObjects();
			}
			if(m_pWater && m_bWaterShaderRenderFlag)
				m_pWater->DeleteDeviceObjects();

			m_pGround = new CBackground(iProject);//,usetex);
			if(m_pGround)
			{
				if(FAILED(m_pGround->InitDeviceObjects()))
				{
					util::del(m_pGround);
					LeaveCriticalSection(&g_pD3dApp->m_cs);
					return TRUE;							// 2006-12-19 by ispark, ´Ù¸¥ ¸Þ½ÃÁö¸¦ ¶ç¿ì±â À§ÇØ¼­ ±âº»ÀûÀÎ ¸Þ½ÃÁö¸¦ »ý·«°í TRUE°ªÀ» ÁØ´Ù.
				}
				m_pGround->RestoreDeviceObjects();
			}

			if(m_pETCRender)
				m_pETCRender->InitDeviceObjects();
			if(m_pWater && m_bWaterShaderRenderFlag)
				m_pWater->InitDeviceObjects();
			if(m_pETCRender)
				m_pETCRender->RestoreDeviceObjects();
			if(m_pWater && m_bWaterShaderRenderFlag)
				m_pWater->RestoreDeviceObjects();

			// 2007-11-20 by bhsohn ¸Ê·Îµù ÇÏ´Â ¹æ½Ä º¯°æ
			{
				g_pD3dApp->UpdateGameStartMapInfo();
			}
			// end 2007-11-20 by bhsohn ¸Ê·Îµù ÇÏ´Â ¹æ½Ä º¯°æ
			LeaveCriticalSection(&g_pD3dApp->m_cs);
			return TRUE;
			// ÀÌÇÏ Àý»è
		}

		EnterCriticalSection(&g_pD3dApp->m_cs);
		m_pGround->RestoreDeviceObjects();
		if(m_pETCRender)
		{
			m_pETCRender->InvalidateDeviceObjects();
			m_pETCRender->DeleteDeviceObjects();
			m_pETCRender->InitDeviceObjects();
			m_pETCRender->RestoreDeviceObjects();
		}
		if(m_pWater && m_bWaterShaderRenderFlag)
		{
			m_pWater->InvalidateDeviceObjects();
			m_pWater->DeleteDeviceObjects();
			m_pWater->InitDeviceObjects();
			m_pWater->RestoreDeviceObjects();
		}
		// 2007-11-20 by bhsohn ¸Ê·Îµù ÇÏ´Â ¹æ½Ä º¯°æ
		{
			g_pD3dApp->UpdateGameStartMapInfo();
		}
		// end 2007-11-20 by bhsohn ¸Ê·Îµù ÇÏ´Â ¹æ½Ä º¯°æ

		LeaveCriticalSection(&g_pD3dApp->m_cs);
		return TRUE;
	}
	else
	{
		DBGOUT("Map Loading State : ERROR [Map Number = %d]\n",g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
		g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0011);// "¸Ê·Îµù¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù. Å¬¶óÀÌ¾ðÆ®¸¦ Á¾·áÇÕ´Ï´Ù."
	}
	return FALSE;
}

HRESULT CSceneData::LoadTex()
{
	FLOG("CSceneData::LoadTex()");
	int i;
	for(i=0;i<TEXTILE_NUM;i++)
		SAFE_RELEASE(m_pCreateTexture[i]);

	DataHeader*  pHeader;
	CGameData * pData = new CGameData;
	char strPath[256];
	int nCont = 0;
	wsprintf(strPath,".\\Res-Map\\mud");
	if(pData->SetFile(strPath,FALSE, NULL,0))
	{
		char* p;
		pHeader = pData->Find(m_pGround->m_projectInfo.strProjectName);
		if(pHeader)
		{
			p = pHeader->m_pData;
			p += 20;
			memcpy(&nCont,p,sizeof(int));
			p += sizeof(int);
			for(i=0;i<nCont;i++)
			{
				int nTexnum;
				memcpy(&nTexnum,p,sizeof(int));

				wsprintf(strPath,"05%06d",nTexnum);
				pHeader = m_pData->Find(strPath);
				if(pHeader)
				{
					if(FAILED(D3DXCreateTextureFromFileInMemory( g_pD3dDev, 
																 pHeader->m_pData,  
																 pHeader->m_DataSize,  
																 &m_pCreateTexture[nTexnum])))
						return E_FAIL;
				}

				p += sizeof(int);
			}
			util::del(pData);
		}
		else
		{
			util::del(pData);
			return E_FAIL;
		}
	}
	else
	{
		util::del(pData);
		return E_FAIL;
	}

	return S_OK;
}

VOID CSceneData::InitRes()
{
	FLOG("CSceneData::InitRes()");
	////////////////////////////////////// Restart , Game Start , Warf ½Ã ¸®¼Ò½º ///////////////////////////////////
	EnterCriticalSection(&g_pD3dApp->m_cs);// by dhkwon 2002.12.16
	if(m_pObjectRender)
		m_pObjectRender->InitDeviceObjects();
	if(m_pMonsterRender)
		m_pMonsterRender->InitDeviceObjects();
//	if(m_pTraceRender)
//		m_pTraceRender->InitDeviceObjects();

	if(!m_pWeaponData)
		m_pWeaponData = new CWeapon;
	if(!m_pItemData)
		m_pItemData = new CAtumNode;

	if(m_pSunRender)
		m_pSunRender->InitDeviceObjects();
	if(!m_pSunData)
		m_pSunData = new CSunData();

	if(m_pRainRender)
		m_pRainRender->InitDeviceObjects();
	if(!m_pRainList)
		m_pRainList = new CAtumNode;
	LeaveCriticalSection(&g_pD3dApp->m_cs);
}

VOID CSceneData::RestoreRes()
{
	FLOG("CSceneData::RestoreRes()");
	////////////////////////////////////// Restart , Game Start , Warf ½Ã ¸®¼Ò½º ///////////////////////////////////
	EnterCriticalSection(&g_pD3dApp->m_cs);
	if(m_pObjectRender)
		m_pObjectRender->RestoreDeviceObjects();
	if(m_pMonsterRender)
		m_pMonsterRender->RestoreDeviceObjects();
//	if(m_pTraceRender)
//		m_pTraceRender->RestoreDeviceObjects();

	if(m_pSunRender)
		m_pSunRender->RestoreDeviceObjects();

	if(m_pRainRender)
		m_pRainRender->RestoreDeviceObjects();
	LeaveCriticalSection(&g_pD3dApp->m_cs);
}

VOID CSceneData::InvalidateRes()
{
	FLOG("CSceneData::InvalidateRes()");
	////////////////////////////////////// Restart , Game End , Warf ½Ã ¸®¼Ò½º ///////////////////////////////////
//	EnterCriticalSection(&m_cs);
	// Item À» Áö¿î´Ù.(¸Ê»óÀÇ ¶°µ¹¾Æ´Ù´Ï´Â ¾ÆÀÌÅÛµé)
	if(m_pItemData)
	{
		CAtumNode* pItem = (CAtumNode *)m_pItemData->m_pChild;
		while(pItem)
		{
			pItem->m_bUsing = FALSE;
			pItem = pItem->m_pNext;
		}
		m_pItemData->Tick();
	}
	// Weapon À» Áö¿î´Ù.
	if(m_pWeaponData)
	{
		CAtumNode* pWeapon = (CAtumNode *)m_pWeaponData->m_pChild;
		while(pWeapon)
		{
			pWeapon->m_bUsing = FALSE;
//			if(pWeapon->m_dwPartType == _MINE)
//			{
//				((CWeaponMineData *)pWeapon)->CheckDeleteMineSendData();
//			}
			pWeapon = pWeapon->m_pNext;
		}
		m_pWeaponData->Tick();
	}
	CMapMonsterIterator itMonster = m_mapMonsterList.begin();
	while(itMonster != m_mapMonsterList.end())
	{
//		itMonster->second->InvalidateChat();
		itMonster->second->InvalidateDeviceObjects();
		itMonster++;
	}
	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
//		itEnemy->second->InvalidateChat();
		itEnemy->second->InvalidateDeviceObjects();
		itEnemy++;
	}
	if(m_pObjectRender)
		m_pObjectRender->InvalidateDeviceObjects();
	if(m_pMonsterRender)
		m_pMonsterRender->InvalidateDeviceObjects();
//	if(m_pTraceRender)
//		m_pTraceRender->InvalidateDeviceObjects();
	if(m_pSunRender)
		m_pSunRender->InvalidateDeviceObjects();

	if(m_pRainRender)
		m_pRainRender->InvalidateDeviceObjects();
	if(m_pGround)
	{
		m_pGround->InvalidateDeviceObjects();
	}
	if(m_pETCRender)
	{
		m_pETCRender->InvalidateDeviceObjects();
	}
	if(m_pWater && m_bWaterShaderRenderFlag)
	{
		m_pWater->InvalidateDeviceObjects();
	}
//	LeaveCriticalSection(&m_cs);
}


VOID CSceneData::DeleteRes()
{
	FLOG("CSceneData::DeleteRes()");
	////////////////////////////////////// Restart , Game End , Warf ½Ã ¸®¼Ò½º ///////////////////////////////////
//	EnterCriticalSection(&m_cs);
	CMapMonsterIterator itMonster = m_mapMonsterList.begin();
	while(itMonster != m_mapMonsterList.end())
	{
		util::del(itMonster->second);
		itMonster++;
	}
	m_mapMonsterList.clear();
	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
		util::del(itEnemy->second);
		itEnemy++;
	}
	m_mapEnemyList.clear();

	m_vecUnitRenderList.clear();
	m_vecEnemyRenderList.clear();
	m_vecEnemyShadowRenderList.clear();
	m_vecMonsterRenderList.clear();
	m_vecMonsterShadowRenderList.clear();
	if(m_vecEnemyBlockList)
	{
		for(int i = 0; i < m_nBlockSizeX;i++)
		{
			for(int j = 0;j < m_nBlockSizeY;j++)
			{
				m_vecEnemyBlockList[i*m_nBlockSizeY + j].clear();
			}
		}
//		util::del_array(m_vecEnemyBlockList);
	}
	if(m_vecMonsterList)
	{
		for(int i = 0; i < m_nBlockSizeX;i++)
		{
			for(int j = 0;j < m_nBlockSizeY;j++)
			{
				m_vecMonsterList[i*m_nBlockSizeY + j].clear();
			}
		}
	}
/*	if(m_vecObjectList)
	{
		for(int i = 0; i < m_nBlockSizeX;i++)
		{
			for(int j = 0;j < m_nBlockSizeY;j++)
			{
				m_vecObjectList[i*m_nBlockSizeY + j].clear();
			}
		}
		util::del_array(m_vecObjectList);
	}
*/	
	m_vectorRangeObjectPtrList.clear();
	m_vectorCollisionObjectPtrList.clear();
	m_vectorCulledObjectPtrList.clear();

	if(m_pObjectRender)
	{
		m_pObjectRender->DeleteDeviceObjects();
	}
	if(m_pMonsterRender)
	{
		m_pMonsterRender->DeleteDeviceObjects();
	}
//	if(m_pTraceRender)
//		m_pTraceRender->DeleteDeviceObjects();
	if(m_pSunRender)
	{
		m_pSunRender->DeleteDeviceObjects();
	}
/*	if(m_pGround)
	{
		m_pGround->DeleteDeviceObjects();
	}
*/
	if(g_pD3dApp->m_pEffectList)
	{
		g_pD3dApp->DeleteEffectList();
		g_pD3dApp->m_pEffectList->Tick();
//		util::del(m_pEffectList);
	}

	if(m_pETCRender)
	{
		m_pETCRender->DeleteDeviceObjects();
	}
	if(m_pWater && m_bWaterShaderRenderFlag)
		m_pWater->DeleteDeviceObjects();
	if(m_pRainList)
	{
		CAtumNode* pRain = (CAtumNode *)m_pRainList->m_pChild;
		while(pRain)
		{
			pRain->m_bUsing = FALSE;
			pRain = pRain->m_pNext;
		}
		m_pRainList->Tick();
		util::del(m_pRainList);
	}
	if(m_pRainRender)
	{
		m_pRainRender->DeleteDeviceObjects();
	}
	m_bIsRestore = TRUE;
	//	LeaveCriticalSection(&m_cs);
}


VOID CSceneData::DeleteTexTileDevice()
{
	FLOG("CSceneData::DeleteTexTileDevice()");
	int i;
	for(i=0;i<TEXTILE_NUM;i++)
	{
		SAFE_RELEASE(m_pCreateTexture[i]);
	}
}

VOID CSceneData::SetShuttleLandState(CAtumData * pNode)
{
	FLOG("CSceneData::SetShuttleLandState(CAtumData * pNode)");
	D3DXMATRIX mat;
	D3DXVECTOR3 dir, vSide;//, orig;
	int x = ((int)pNode->m_vPos.x)/TILE_SIZE;
	int z = ((int)pNode->m_vPos.z)/TILE_SIZE;
	if(m_pGround->m_pTileInfo[x*m_pGround->m_projectInfo.sYSize + z].bEnableLand)
	{
/*		dir = D3DXVECTOR3(0,-1,0);
		vSide = D3DXVECTOR3(0,0,1);
		D3DXMatrixLookAtLH(&mat,&pNode->m_vPos,&(pNode->m_vPos + dir),&vSide);
		float fTemp1 = pNode->m_vPos.y - g_pD3dApp->m_pObjectRender->CheckCollMesh(mat,pNode->m_vPos);//+ 5.0f;
		float fTemp2 = m_pGround->CheckHeightMap(pNode->m_vPos);
		if(fTemp2 < fTemp1)
			pNode->m_vPos.y =  fTemp1;
		else
			pNode->m_vPos.y = fTemp2;
*/		pNode->m_dwState = _LANDED;
		((CShuttleChild *)pNode)->m_bIsAir      = FALSE;
		((CShuttleChild *)pNode)->m_bFirstStart = TRUE;
	}

}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::ApplyFogDistanceAsHeight( float fOriginStart , float fOriginEnd )
/// \brief		ÀÏÁ¤ °íµµ¿¡ µû¸¥ ½Ã¾ß Á¶Àý
/// \author		dhkwon
/// \date		2004-06-18 ~ 2004-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSceneData::ApplyFogDistanceAsHeight( float fOriginStart , float fOriginEnd )
{
	// ÀÏÁ¤ °íµµ ÀÌ»ó ÀÎÁö Ã¼Å©
/*	float fAltitudeApplyMin = ALTITUDE_APPLY_MIN + g_pGround->m_projectInfo.fWaterHeight;

	if( g_pShuttleChild->m_vPos.y <= fAltitudeApplyMin )
		return FALSE;

	float fAltitudeApplyMax	= ALTITUDE_APPLY_MAX + g_pGround->m_projectInfo.fWaterHeight;

	//°íµµ Àû¿ë ½ÃÀÛ °ú ³¡ ÀÇ Â÷ÀÌ
	float fDifferDistance =   g_pShuttleChild->m_vPos.y - fAltitudeApplyMin;
	if( fDifferDistance > (fAltitudeApplyMax-fAltitudeApplyMin) )
		fDifferDistance = (fAltitudeApplyMax-fAltitudeApplyMin);

	//°íµµ¿¡ µû¸¥ ™VÆÃÇÒ Æ÷±× ½ÃÀÛ ¸¶Áö¸·
//	float fDestStart, fDestEnd;


	float fDestStart, fDestEnd, fApplyDistanceRate;
	fApplyDistanceRate = (fDifferDistance / (fAltitudeApplyMax - fAltitudeApplyMin));
	fDestStart = fOriginStart - fOriginStart * fApplyDistanceRate;
	if( fDestStart < 0 )
		fDestStart = 0.0f;
	fDestEnd = fOriginEnd - ( fOriginEnd * ALTITUDE_DEST_RATE ) * fApplyDistanceRate * fApplyDistanceRate;

//	fDestStart = fOriginStart - fOriginStart * ( fDifferDistance / (fAltitudeApplyMax - fAltitudeApplyMin)) * 
//					( fDifferDistance / (fAltitudeApplyMax - fAltitudeApplyMin));
//	if( fDestStart < 0 )
//		fDestStart = 0.0f;
//	fDestEnd = fOriginEnd - ( fOriginEnd * ALTITUDE_DEST_RATE ) * (fDifferDistance / (fAltitudeApplyMax - fAltitudeApplyMin)) * (fDifferDistance / (fAltitudeApplyMax - fAltitudeApplyMin) );
	if( fDestEnd < 10.0f )
		fDestEnd = 10.0f;

	m_fFogDestStartValue = fDestStart;
	m_fFogDestEndValue = fDestEnd;

	return TRUE;
*/
//	if(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 2001)
//	{
//		m_fFogDestStartValue = fOriginStart;
//		m_fFogDestEndValue = fOriginEnd;
//		return TRUE;
//	}

	float fAltitudeApplyMin = g_fAltitudeApplyMin + g_pGround->m_projectInfo.fWaterHeight;

	if( g_pShuttleChild->m_vPos.y <= fAltitudeApplyMin )
		return FALSE;

	float fAltitudeApplyMax	= g_fAltitudeApplyMax + g_pGround->m_projectInfo.fWaterHeight;

	
	float fDifferDistance =   g_pShuttleChild->m_vPos.y - fAltitudeApplyMin;
	if( fDifferDistance > (fAltitudeApplyMax-fAltitudeApplyMin) )
		fDifferDistance = (fAltitudeApplyMax-fAltitudeApplyMin);

	float fDestStart, fDestEnd, fApplyDistanceRate;
	fApplyDistanceRate = (fDifferDistance / (fAltitudeApplyMax - fAltitudeApplyMin));
	
	fDestStart = fOriginStart - fOriginStart * fApplyDistanceRate;
	if( fDestStart < 0 )
		fDestStart = 0.0f;

	fDestEnd = fOriginEnd - ( fOriginEnd * g_fAltitudeDestRate ) * fApplyDistanceRate * fApplyDistanceRate;
	if( fDestEnd < 10.0f )
		fDestEnd = 10.0f;

//	m_fFogDestStartValue = fDestStart;
//	m_fFogDestEndValue = fDestEnd;


	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::CheckWeather()
/// \brief		³¯¾¾¿¡ µû¸¥ ¾È°³ °Å¸®¸¦ ¼¼ÆÃÇÑ´Ù.
/// \author		jschoi
/// \date		2004-10-20 ~ 2004-10-20
/// \warning	±âÁ¸¿¡´Â ¾È°³ °Å¸®¿Í ¾È°³ »ö»ó ¸ðµÎ¸¦ ¼¼ÆÃÇßÁö¸¸
///				CheckWeather() ¿¡¼­ ¾È°³ °Å¸®¸¦ ´ã´çÇÏ°í
///				CheckDay() ¿¡¼­ ¾È°³ »ö»óÀ» ´ã´çÇÑ´Ù.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CSceneData::CheckWeather()
{
	FLOG("CSceneData::CheckWeather()");
	if(!m_bChangeWeather)
	{
		return;
	}
	// 2004-10-20 by jschoi ¾Æ·¡ÄÚµå´Â ³¯¾¾ º¯È­°¡ ÁøÇàÁßÀÏ¶§ µ¿ÀÛÇÑ´Ù.
	
	const float fTotalChangeWeatherTime = 5.0f;	// ³¯¾¾ÀÇ º¯È­´Â 10ÃÊ µ¿¾È ÀÌ·ç¾îÁø´Ù.
	if(m_fChangeWeatherCheckTime == 0.0f)
	{
		m_fChangeWeatherCheckTime = fTotalChangeWeatherTime;
		m_fBeforeFogStartValue = m_fFogStartValue;
		m_fBeforeFogEndValue = m_fFogEndValue;
	}
	
	float fTargetFogStartValue, fTargetFogEndValue;

	switch(m_byWeatherType)
	{
	case WEATHER_DEFAULT:
		{
			fTargetFogStartValue = m_fOrgFogStartValue;
			fTargetFogEndValue = m_fOrgFogEndValue;
		}
		break;
	case WEATHER_SUNNY:
		{
			fTargetFogStartValue = m_fOrgFogStartValue;
			fTargetFogEndValue = m_fOrgFogEndValue;
		}
		break;
	case WEATHER_RAINY:
		{
			fTargetFogStartValue = WEATHER_RAINY_FOG_START;
			fTargetFogEndValue = WEATHER_RAINY_FOG_END;
		}
		break;
	case WEATHER_SNOWY:
		{
			fTargetFogStartValue = WEATHER_SNOWY_FOG_START;
			fTargetFogEndValue = WEATHER_SNOWY_FOG_END;
		}
		break;
	case WEATHER_CLOUDY:
		{
			fTargetFogStartValue = WEATHER_CLOUDY_FOG_START;
			fTargetFogEndValue = WEATHER_CLOUDY_FOG_END;
		}
		break;
	case WEATHER_FOGGY:
		{
			fTargetFogStartValue = WEATHER_FOGGY_FOG_START;
			fTargetFogEndValue = WEATHER_FOGGY_FOG_END;
		}
		break;
	default:
		break;
	}

	m_fChangeWeatherCheckTime -= g_pD3dApp->GetElapsedTime();
	
	m_fFogStartValue = fTargetFogStartValue*(1 - m_fChangeWeatherCheckTime/fTotalChangeWeatherTime)
						+ m_fBeforeFogStartValue*(m_fChangeWeatherCheckTime/fTotalChangeWeatherTime);
	m_fFogEndValue = fTargetFogEndValue*(1 - m_fChangeWeatherCheckTime/fTotalChangeWeatherTime)
						+ m_fBeforeFogEndValue*(m_fChangeWeatherCheckTime/fTotalChangeWeatherTime);

	if(m_fChangeWeatherCheckTime <= 0)
	{
//		m_bChangeWeather = FALSE;
		m_fChangeWeatherCheckTime = 0;
		m_fFogStartValue = fTargetFogStartValue;
		m_fFogEndValue = fTargetFogEndValue;
	}


	static float fSnowCheckTime = 0.0f;
	if(fSnowCheckTime >= 0.0f)
		fSnowCheckTime -= g_pD3dApp->GetElapsedTime();
	DWORD dwFogColor = 0;
	switch(m_byWeatherType)
	{
	case WEATHER_DEFAULT:
	{
/*			m_fFogStartValue += ((200.0f + (g_pSOption->sTerrainRender-8)*0.25f*TILE_SIZE) - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
			m_fFogEndValue += ((650.0f + (g_pSOption->sTerrainRender-8)*1.99f*TILE_SIZE) - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
			DWORD dwColor;
			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
			BYTE bRed, bGreen, bBlue;
			bRed = (BYTE)(dwColor >> 16);
			bGreen = (BYTE)(dwColor >> 8);
			bBlue = (BYTE)(dwColor);
			dwFogColor = D3DCOLOR_ARGB(0x00,(BYTE)(bRed + (m_fSkyRedColor*255.0f*0.4f - bRed)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bGreen + (m_fSkyGreenColor*255.0f*0.4f - bGreen)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bBlue + (m_fSkyBlueColor*255.0f*0.4f - bBlue)*g_pD3dApp->GetElapsedTime()));
*/
/*			m_fFogStartValue += ((300.0f + (g_pSOption->sTerrainRender-8)*0.3f*TILE_SIZE) - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
			m_fFogEndValue += ((650.0f + (g_pSOption->sTerrainRender-8)*1.99f*TILE_SIZE) - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
			DWORD dwColor;
			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
			BYTE bRed, bGreen, bBlue;
			bRed = (BYTE)(dwColor >> 16);
			bGreen = (BYTE)(dwColor >> 8);
			bBlue = (BYTE)(dwColor);
			dwFogColor = D3DCOLOR_ARGB(0x00,(BYTE)(bRed + (m_fSkyRedColor*255.0f*0.4f - bRed)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bGreen + (m_fSkyGreenColor*255.0f*0.4f - bGreen)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bBlue + (m_fSkyBlueColor*255.0f*0.4f - bBlue)*g_pD3dApp->GetElapsedTime()));
*/			m_fFogStartValue = WEATHER_DEFAULT_FOG_START;
			m_fFogEndValue = WEATHER_DEFAULT_FOG_END;
			dwFogColor = D3DCOLOR_ARGB(0,55,60,90);
			m_bChangeWeather = FALSE;
	}
		break;
	case WEATHER_SUNNY:
		{
//			ApplyFogDistanceAsHeight( WEATHER_SUNNY_FOG_START , WEATHER_SUNNY_FOG_END );
			ApplyFogDistanceAsHeight( m_fOrgFogStartValue , m_fOrgFogEndValue );
//			m_fFogStartValue += (m_fFogDestStartValue - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
//			m_fFogEndValue += (m_fFogDestEndValue - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
//			dwFogColor = GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, !m_bNight);
//			DWORD dwColor;
//			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
//			BYTE bRed, bGreen, bBlue;
//			bRed = (BYTE)(dwColor >> 16);
//			bGreen = (BYTE)(dwColor >> 8);
//			bBlue = (BYTE)(dwColor);
//			dwFogColor = D3DCOLOR_ARGB(0x00,(BYTE)(bRed + (m_fSkyRedColor*255.0f - bRed)*g_pD3dApp->GetElapsedTime())
//				,(BYTE)(bGreen + (m_fSkyGreenColor*255.0f - bGreen)*g_pD3dApp->GetElapsedTime())
//				,(BYTE)(bBlue + (m_fSkyBlueColor*255.0f - bBlue)*g_pD3dApp->GetElapsedTime()));
		}
		break;
	case WEATHER_RAINY:
		{
			ApplyFogDistanceAsHeight( WEATHER_RAINY_FOG_START , WEATHER_RAINY_FOG_END );
//			m_fFogStartValue += (m_fFogDestStartValue - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
//			m_fFogEndValue += (m_fFogDestEndValue - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
			// Rain Test
			if(fSnowCheckTime <= 0.0f)
			{
				fSnowCheckTime = SNOW_CHECKTIME;
				CRainData * pRain;
				D3DXVECTOR3 vPos,vVel,vUp,vTemp;
				vUp = D3DXVECTOR3(0,1,0);
				float fVelRate;
				vPos = g_pD3dApp->m_pCamera->GetEyePt() + 200.0f*vUp + 250.0f*g_pD3dApp->m_pCamera->GetViewDir();
				for(int i =0; i < MAX_SNOW_AMOUNT*10;i++)
				{
					vTemp = vPos;
					
					// todo : here is the rain speed
					
					// fVelRate = RANDI(350, 450);
					
					fVelRate = RANDI(250, 350);
					
					vTemp.x += RANDI(-250, 250);
					vTemp.z += RANDI(-250, 250);

					vVel.x = RANDI(-50, 50);
					vVel.y = -500.0f;
					vVel.z = RANDI(-50, 50);
					D3DXVec3Normalize(&vVel,&vVel);
					pRain = new CRainData(vTemp,fVelRate,vVel);
					pRain = (CRainData *)m_pRainList->AddChild(pRain);
				}
			}
			DWORD dwColor;
			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
			BYTE bRed, bGreen, bBlue;
			bRed = (BYTE)(dwColor >> 16);
			bGreen = (BYTE)(dwColor >> 8);
			bBlue = (BYTE)(dwColor);
			dwFogColor = D3DCOLOR_ARGB(0x00,(BYTE)(bRed + (m_fSkyRedColor*255.0f*0.4f - bRed)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bGreen + (m_fSkyGreenColor*255.0f*0.4f - bGreen)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bBlue + (m_fSkyBlueColor*255.0f*0.4f - bBlue)*g_pD3dApp->GetElapsedTime()));
		}
		break;
	case WEATHER_SNOWY:
		{
			ApplyFogDistanceAsHeight( WEATHER_SNOWY_FOG_START , WEATHER_SNOWY_FOG_END );
//			m_fFogStartValue += (m_fFogDestStartValue - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
//			m_fFogEndValue += (m_fFogDestEndValue - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
			// Snow Test
			if(fSnowCheckTime <= 0.0f)
			{
				fSnowCheckTime = SNOW_CHECKTIME;
				CAppEffectData * pEff;
				D3DXVECTOR3 vPos,vVel,vUp,vTemp;
				vUp = D3DXVECTOR3(0,1,0);
				float fVelRate;
				vPos = g_pD3dApp->m_pCamera->GetEyePt() + 200.0f*vUp + 350.0f*g_pD3dApp->m_pCamera->GetViewDir();
				for(int i =0; i < MAX_SNOW_AMOUNT;i++)
				{
					vTemp = vPos;
					fVelRate = rand()%50 + 20;

					vTemp.x += rand()%400 - 200;
					vTemp.z += rand()%400 - 200;

					vVel.x = rand()%100 - 50;
					vVel.y = -500.0f;
					vVel.z = rand()%100 - 50;
					D3DXVec3Normalize(&vVel,&vVel);
					pEff = new CAppEffectData(RC_EFF_SNOW,vTemp,fVelRate,vVel);
					pEff = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEff);
				}
			}
			DWORD dwColor;
			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
			BYTE bRed, bGreen, bBlue;
			bRed = (BYTE)(dwColor >> 16);
			bGreen = (BYTE)(dwColor >> 8);
			bBlue = (BYTE)(dwColor);
			dwFogColor = D3DCOLOR_ARGB(0x00,(BYTE)(bRed + (m_fSkyRedColor*255.0f*0.4f - bRed)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bGreen + (m_fSkyGreenColor*255.0f*0.4f - bGreen)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bBlue + (m_fSkyBlueColor*255.0f*0.4f - bBlue)*g_pD3dApp->GetElapsedTime()));
		}
		break;
	case WEATHER_CLOUDY:
		{
			ApplyFogDistanceAsHeight( WEATHER_CLOUDY_FOG_START , WEATHER_CLOUDY_FOG_END );
//			m_fFogStartValue += (m_fFogDestStartValue - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
//			m_fFogEndValue += (m_fFogDestEndValue - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
			DWORD dwColor;
			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
			BYTE bRed, bGreen, bBlue;
			bRed = (BYTE)(dwColor >> 16);
			bGreen = (BYTE)(dwColor >> 8);
			bBlue = (BYTE)(dwColor);
			dwFogColor = D3DCOLOR_ARGB(0x00,(BYTE)(bRed + (m_fSkyRedColor*255.0f*0.4f - bRed)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bGreen + (m_fSkyGreenColor*255.0f*0.4f - bGreen)*g_pD3dApp->GetElapsedTime())
				,(BYTE)(bBlue + (m_fSkyBlueColor*255.0f*0.4f - bBlue)*g_pD3dApp->GetElapsedTime()));
		}
		break;
	case WEATHER_FOGGY:
		{
			ApplyFogDistanceAsHeight( WEATHER_FOGGY_FOG_START , WEATHER_FOGGY_FOG_END );
//			m_fFogStartValue += (m_fFogDestStartValue - m_fFogStartValue)*g_pD3dApp->GetElapsedTime();
			if(m_fFogStartValue < 0.0f)
				m_fFogStartValue = 0.0f;
//			m_fFogEndValue += (m_fFogDestEndValue - m_fFogEndValue)*g_pD3dApp->GetElapsedTime();
			DWORD dwColor;
			g_pD3dDev->GetRenderState( D3DRS_FOGCOLOR,  &dwColor );
			BYTE bRed, bGreen, bBlue;
			bRed = (BYTE)(dwColor >> 16);
			bGreen = (BYTE)(dwColor >> 8);
			bBlue = (BYTE)(dwColor);
			dwFogColor = D3DCOLOR_ARGB(0x00,
				(BYTE)(bRed + (200 - bRed)*g_pD3dApp->GetElapsedTime()),
				(BYTE)(bGreen + (200 - bGreen)*g_pD3dApp->GetElapsedTime()),
				(BYTE)(bBlue + (200 - bBlue)*g_pD3dApp->GetElapsedTime()));
		}
		break;
	}
	// ÀÓ½Ã ÄÚµå ³ªÁß¿¡ ¸Ê ¿¡µðÅÍ¿¡¼­ °¡Á®¿È
	if (g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3067)
	{
		m_fFogStartValue = 200.0f;
		m_fFogEndValue = 2200.0f;
	}
	else if(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3003)
	{
		m_fFogStartValue = 1600.0f;
		m_fFogEndValue = 2400.0f;
	}
	else if(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 4001)
	{
		m_fFogStartValue = 200.0f;
		m_fFogEndValue = 1000.0f;
	}


// 	m_dwFogColor = dwFogColor;

	///////////////////////// FOG //////////////////////////////////////////
    g_pD3dDev->SetRenderState( D3DRS_FOGENABLE, IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
	// Vertex Fog Use
    g_pD3dDev->SetRenderState( D3DRS_FOGTABLEMODE,   D3DFOG_NONE );
    g_pD3dDev->SetRenderState( D3DRS_FOGSTART,  FtoDW(m_fFogStartValue) ); //
    g_pD3dDev->SetRenderState( D3DRS_FOGEND,    FtoDW(m_fFogEndValue) );
    g_pD3dDev->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_LINEAR );
    g_pD3dDev->SetRenderState( D3DRS_RANGEFOGENABLE, TRUE );
    g_pD3dDev->SetFVF( D3DFVF_FOGVERTEX );
	////////////////////////////////////////////////////////////////////////
    g_pD3dDev->SetRenderState( D3DRS_FOGCOLOR,  m_dwFogColor );
}
#ifdef _DEBUG_MAPSETTING
VOID CSceneData::CheckDay()
{
}
#else
VOID CSceneData::CheckDay()
{
	FLOG("CSceneData::CheckDay()");
	const DWORD dwTime = 1;			// ½Ã°£À» dwTime¹è ¸¸Å­ »¡¸® Èå¸£°Ô ÇÑ´Ù.				DEFAULT = 1
	const DWORD dwChangingTime = 1;	// ¹ã ³· º¯È­½Ã°£À» dwChangingTime¹è ¿À·¡ Áö¼ÓµÇ°Ô ÇÑ´Ù.DEFAULT = 1

//	int nH = 4, nM = 60, nS = 60; // nH:½Ã°£ nM:ºÐ nS:ÃÊ ÀÇ ·® ¼ÂÆÃÇÒ¼ö ÀÖ´Ù.(¿¹> ÇÏ·ç->4½Ã°£ 1½Ã°£->60ºÐ 1ºÐ->60ÃÊ)-°ÔÀÓ»óÀÇ ½Ã°£
	int nTime = (GetTickCount() - m_dwStartTime)/1000 + m_nBaseTime;
	nTime = (nTime*dwTime)%(TIME_HOUR*TIME_MINUTE*TIME_SECOND);

// 2004-10-20 by jschoi
	float fChangingCheckTime;
	float fChangingTime = 10 * dwChangingTime * TIME_SECOND;
	DWORD dwDayFogColor,dwNightFogColor;
	BYTE byDayRed,byDayGreen,byDayBlue,byNightRed,byNightGreen,byNightBlue,byFogRed,byFogGreen,byFogBlue;
//	dwDayFogColor = GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,TRUE);
//	dwNightFogColor = GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,FALSE);
	dwDayFogColor = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)->DayFogColor;
	dwNightFogColor = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)->NightFogColor;
	
//	if(g_pSOption->sReflactive)
	{
		byDayRed = (BYTE)(dwDayFogColor >> 16);
		byDayGreen = (BYTE)(dwDayFogColor >> 8);
		byDayBlue = (BYTE)(dwDayFogColor);
		byNightRed = (BYTE)(dwNightFogColor >> 16);
		byNightGreen = (BYTE)(dwNightFogColor >> 8);
		byNightBlue = (BYTE)(dwNightFogColor);

		float fBlur = ((float)g_pD3dApp->m_pFxSystem->GetSourceAlpha()/255)*0.2f;
		
		byDayRed -= byDayRed*fBlur;
		byDayGreen -= byDayGreen*fBlur;
		byDayBlue -= byDayBlue*fBlur;
		byNightRed -= byNightRed*fBlur;
		byNightGreen -= byNightGreen*fBlur;
		byNightBlue -= byNightBlue*fBlur;

		dwDayFogColor = D3DCOLOR_ARGB(0,byDayRed,byDayGreen,byDayBlue);
		dwNightFogColor = D3DCOLOR_ARGB(0,byNightRed,byNightGreen,byNightBlue);
	}

	if( nTime >= 0 && nTime < TIME_MINUTE*TIME_SECOND )
	{ // ¹ã
		if(g_pShuttleChild->GetCurrentBodyCondition() & ~BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, TRUE );
		}
		if( nTime < fChangingTime )
		{ // ¹ãÀÌ µÈÁö 10ºÐÀÌ³»
			fChangingCheckTime = (float)nTime;

			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fDiffuseR2*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fDiffuseG2*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fDiffuseB2*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fAmbientR2*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fAmbientG2*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fAmbientB2*(fChangingCheckTime/fChangingTime);

			m_fAlphaSky = 1.0f - fChangingCheckTime/fChangingTime;	// ¹ã, ³· ½ºÄ«ÀÌ ¹Ú½º È¥ÇÕ ºñÀ² 
			
			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			byDayRed =		(BYTE)(dwDayFogColor >> 16);
			byDayGreen =	(BYTE)(dwDayFogColor >> 8);
			byDayBlue =		(BYTE)(dwDayFogColor);
			byNightRed =	(BYTE)(dwNightFogColor >> 16);
			byNightGreen =	(BYTE)(dwNightFogColor >> 8);
			byNightBlue =	(BYTE)(dwNightFogColor);
			byFogRed = byDayRed*(1.0f - fChangingCheckTime/fChangingTime) + byNightRed*(fChangingCheckTime/fChangingTime);
			byFogGreen = byDayGreen*(1.0f - fChangingCheckTime/fChangingTime) + byNightGreen*(fChangingCheckTime/fChangingTime);	
			byFogBlue =	byDayBlue*(1.0f - fChangingCheckTime/fChangingTime) + byNightBlue*(fChangingCheckTime/fChangingTime);	
			m_dwFogColor = D3DCOLOR_ARGB(0x00,byFogRed,byFogGreen,byFogBlue);
		}
		else
		{ // ¹ã 10ºÐÀÌ Áö³­ ÀÌÈÄ
			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR2;
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG2;
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB2;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2;
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2;
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2;

			m_fAlphaSky = 0.0f;	// ¹ã, ³· ½ºÄ«ÀÌ ¹Ú½º È¥ÇÕ ºñÀ² 

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			m_dwFogColor = dwNightFogColor;
		}
//		m_bNight = TRUE;
		m_light0.Specular.r  = 0.4f;
		m_light0.Specular.g  = 0.4f;
		m_light0.Specular.b  = 0.4f;
	}
	else
	{ // ³·
		if(g_pShuttleChild->GetCurrentBodyCondition() & BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, FALSE );
		}
		if( nTime >= TIME_MINUTE*TIME_SECOND && nTime < TIME_MINUTE*TIME_SECOND + fChangingTime )
		{ // ³·ÀÌ µÈÁö 10ºÐÀÌ³»
			fChangingCheckTime = (float)(nTime - TIME_MINUTE*TIME_SECOND);
			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fDiffuseR1*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fDiffuseG1*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fDiffuseB1*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fAmbientR1*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fAmbientG1*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fAmbientB1*(fChangingCheckTime/fChangingTime);

			m_fAlphaSky = fChangingCheckTime/fChangingTime;	// ¹ã, ³· ½ºÄ«ÀÌ ¹Ú½º È¥ÇÕ ºñÀ² 

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			byDayRed =		(BYTE)(dwDayFogColor >> 16);
			byDayGreen =	(BYTE)(dwDayFogColor >> 8);
			byDayBlue =		(BYTE)(dwDayFogColor);
			byNightRed =	(BYTE)(dwNightFogColor >> 16);
			byNightGreen =	(BYTE)(dwNightFogColor >> 8);
			byNightBlue =	(BYTE)(dwNightFogColor);
			byFogRed = byNightRed*(1.0f - fChangingCheckTime/fChangingTime) + byDayRed*(fChangingCheckTime/fChangingTime);
			byFogGreen = byNightGreen*(1.0f - fChangingCheckTime/fChangingTime) + byDayGreen*(fChangingCheckTime/fChangingTime);	
			byFogBlue =	byNightBlue*(1.0f - fChangingCheckTime/fChangingTime) + byDayBlue*(fChangingCheckTime/fChangingTime);	
			m_dwFogColor = D3DCOLOR_ARGB(0x00,byFogRed,byFogGreen,byFogBlue);
		}
		else
		{ // ³· 10ºÐÀÌ Áö³­ ÀÌÈÄ
			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR1;
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG1;
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB1;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1;
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1;
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1;

			m_fAlphaSky = 1.0f;	// ¹ã, ³· ½ºÄ«ÀÌ ¹Ú½º È¥ÇÕ ºñÀ² 

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			m_dwFogColor = dwDayFogColor;
		}
//		m_bNight = FALSE;
		m_light0.Specular.r  = 1.0f;
		m_light0.Specular.g  = 1.0f;
		m_light0.Specular.b  = 1.0f;
	}

	// 2004-10-22 by jschoi
	// ¾îµÎ¿öÁö±â Á÷Àü¿¡ ¹Ù·Î ´ÞÀÌ ¶á´Ù.
	// ¼­¼­È÷ ¹à¾ÆÁö¸é¼­ Áß°£Âë ¹à¾ÆÁö¸é ÅÂ¾çÀÌ ¶á´Ù.	
	if(	(nTime >= 0) &&											
		(nTime < (TIME_MINUTE*TIME_SECOND + fChangingTime/2)))	
	{
		m_bNight = TRUE;
	}
	else
	{
		m_bNight = FALSE;
	}


	// 2005-04-18 by jschoi - Tutorial
	if(g_pTutorial->IsTutorialMode() == TRUE)
	{// ¹«Á¶°Ç ÇÑ ³·ÀÌ´Ù.
		if(g_pShuttleChild->GetCurrentBodyCondition() & BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, FALSE );
		}
		m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR1;
		m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG1;
		m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB1;
		m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1;
		m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1;
		m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1;
		m_fAlphaSky = 1.0f;
		ChangeGammaOption( g_pSOption->sGammaCtrl );
		m_dwFogColor = dwDayFogColor;

		m_light0.Specular.r  = 1.0f;
		m_light0.Specular.g  = 1.0f;
		m_light0.Specular.b  = 1.0f;
		m_bNight = FALSE;
	}


//	if(nTime >= 0 && nTime < 1*TIME_MINUTE*TIME_SECOND)	// ¹ã
//	{
//		if(!m_bNight)
//		{
//			if(	m_fSkyRedColor != m_pGround->m_projectInfo.fDiffuseR2 ||
//				m_fSkyGreenColor != m_pGround->m_projectInfo.fDiffuseG2 ||
//				m_fSkyBlueColor != m_pGround->m_projectInfo.fDiffuseB2)
//			{
//				if(g_pShuttleChild->GetCurrentBodyCondition() & ~BODYCON_NIGHTFLY_MASK)
//				{
//					g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, TRUE );
//				}
//				if(m_fSkyRedColor > m_pGround->m_projectInfo.fDiffuseR2)
//					m_fSkyRedColor -= (m_pGround->m_projectInfo.fDiffuseR1 - 
//					m_pGround->m_projectInfo.fDiffuseR2)/TIME_DAY_CHANGE;
//				if(m_fSkyRedColor < m_pGround->m_projectInfo.fDiffuseR2)
//					m_fSkyRedColor = m_pGround->m_projectInfo.fDiffuseR2;
//				if(m_fSkyGreenColor > m_pGround->m_projectInfo.fDiffuseG2)
//					m_fSkyGreenColor -= (m_pGround->m_projectInfo.fDiffuseG1 - 
//					m_pGround->m_projectInfo.fDiffuseG2)/TIME_DAY_CHANGE;
//				if(m_fSkyGreenColor < m_pGround->m_projectInfo.fDiffuseG2)
//					m_fSkyGreenColor = m_pGround->m_projectInfo.fDiffuseG2;
//				if(m_fSkyBlueColor > m_pGround->m_projectInfo.fDiffuseB2)
//					m_fSkyBlueColor -= (m_pGround->m_projectInfo.fDiffuseB1 - 
//					m_pGround->m_projectInfo.fDiffuseB2)/TIME_DAY_CHANGE;
//				if(m_fSkyBlueColor < m_pGround->m_projectInfo.fDiffuseB2)
//					m_fSkyBlueColor = m_pGround->m_projectInfo.fDiffuseB2;
//
//				m_light0.Diffuse.r = m_fSkyRedColor;
//				m_light0.Diffuse.g = m_fSkyGreenColor;
//				m_light0.Diffuse.b = m_fSkyBlueColor;
//
//				if(m_light0.Ambient.r > m_pGround->m_projectInfo.fAmbientR2)
//					m_light0.Ambient.r -= (m_pGround->m_projectInfo.fAmbientR1 - 
//					m_pGround->m_projectInfo.fAmbientR2)/TIME_DAY_CHANGE;
//				if(m_light0.Ambient.r < m_pGround->m_projectInfo.fAmbientR2)
//					m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2;
//				if(m_light0.Ambient.g > m_pGround->m_projectInfo.fAmbientG2)
//					m_light0.Ambient.g -= (m_pGround->m_projectInfo.fAmbientG1 - 
//					m_pGround->m_projectInfo.fAmbientG2)/TIME_DAY_CHANGE;
//				if(m_light0.Ambient.g < m_pGround->m_projectInfo.fAmbientG2)
//					m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2;
//				if(m_light0.Ambient.b > m_pGround->m_projectInfo.fAmbientB2)
//					m_light0.Ambient.b -= (m_pGround->m_projectInfo.fAmbientB1 - 
//					m_pGround->m_projectInfo.fAmbientB2)/TIME_DAY_CHANGE;
//				if(m_light0.Ambient.b < m_pGround->m_projectInfo.fAmbientB2)
//					m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2;
//			}
//			else
//			{
//				m_bNight = TRUE;//¹ã
//			}
//		}
//	}
//	else if(nTime >= 1*TIME_MINUTE*TIME_SECOND && nTime < TIME_HOUR*TIME_MINUTE*TIME_SECOND)
//	{
//		if(m_bNight)
//		{
//			if(m_fSkyRedColor != m_pGround->m_projectInfo.fDiffuseR1 ||
//				m_fSkyGreenColor != m_pGround->m_projectInfo.fDiffuseG1 ||
//				m_fSkyBlueColor != m_pGround->m_projectInfo.fDiffuseB1)
//			{
//				if(g_pShuttleChild->GetCurrentBodyCondition() & BODYCON_NIGHTFLY_MASK)
//				{
//					g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, FALSE );
//				}
//				if(m_fSkyRedColor < m_pGround->m_projectInfo.fDiffuseR1)
//					m_fSkyRedColor += (m_pGround->m_projectInfo.fDiffuseR1 - 
//					m_pGround->m_projectInfo.fDiffuseR2)*g_pD3dApp->GetElapsedTime()/TIME_DAY_CHANGE;
//				if(m_fSkyRedColor > m_pGround->m_projectInfo.fDiffuseR1)
//					m_fSkyRedColor = m_pGround->m_projectInfo.fDiffuseR1;
//				if(m_fSkyGreenColor < m_pGround->m_projectInfo.fDiffuseG1)
//					m_fSkyGreenColor += (m_pGround->m_projectInfo.fDiffuseG1 - 
//					m_pGround->m_projectInfo.fDiffuseG2)*g_pD3dApp->GetElapsedTime()/TIME_DAY_CHANGE;
//				if(m_fSkyGreenColor > m_pGround->m_projectInfo.fDiffuseG1)
//					m_fSkyGreenColor = m_pGround->m_projectInfo.fDiffuseG1;
//				if(m_fSkyBlueColor < m_pGround->m_projectInfo.fDiffuseB1)
//					m_fSkyBlueColor += (m_pGround->m_projectInfo.fDiffuseB1 - 
//					m_pGround->m_projectInfo.fDiffuseB2)*g_pD3dApp->GetElapsedTime()/TIME_DAY_CHANGE;
//				if(m_fSkyBlueColor > m_pGround->m_projectInfo.fDiffuseB1)
//					m_fSkyBlueColor = m_pGround->m_projectInfo.fDiffuseB1;
//
//				m_light0.Diffuse.r = m_fSkyRedColor;
//				m_light0.Diffuse.g = m_fSkyGreenColor;
//				m_light0.Diffuse.b = m_fSkyBlueColor;
//
//				if(m_light0.Ambient.r < m_pGround->m_projectInfo.fAmbientR1)
//					m_light0.Ambient.r += (m_pGround->m_projectInfo.fAmbientR1 - 
//					m_pGround->m_projectInfo.fAmbientR2)*g_pD3dApp->GetElapsedTime()/TIME_DAY_CHANGE;
//				if(m_light0.Ambient.r > m_pGround->m_projectInfo.fAmbientR1)
//					m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1;
//				if(m_light0.Ambient.g < m_pGround->m_projectInfo.fAmbientG1)
//					m_light0.Ambient.g += (m_pGround->m_projectInfo.fAmbientG1 - 
//					m_pGround->m_projectInfo.fAmbientG2)*g_pD3dApp->GetElapsedTime()/TIME_DAY_CHANGE;
//				if(m_light0.Ambient.g > m_pGround->m_projectInfo.fAmbientG1)
//					m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1;
//				if(m_light0.Ambient.b < m_pGround->m_projectInfo.fAmbientB1)
//					m_light0.Ambient.b += (m_pGround->m_projectInfo.fAmbientB1 - 
//					m_pGround->m_projectInfo.fAmbientB2)*g_pD3dApp->GetElapsedTime()/TIME_DAY_CHANGE;
//				if(m_light0.Ambient.b > m_pGround->m_projectInfo.fAmbientB1)
//					m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1;
//			}
//			else
//			{
//				m_bNight = FALSE;
//			}
//		}
//	}

	//	ÀÓ½ÃÄÚµå - ³ªÁß¿¡ ¸Ê ¿¡µðÅÍ¿¡¼­ µ¥ÀÌÅÍ¸¦ °¡Á®¿Ã °Í.. - 2004-07-06 - jschoi
/*	if (g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3067)
	{
		m_light0.Diffuse.r = 0.64f;
		m_light0.Diffuse.g = 0.80f;
		m_light0.Diffuse.b = 0.77f;
	}
	else if(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex == 3003)
	{
		m_light0.Diffuse.r = 0.60f;
		m_light0.Diffuse.g = 0.59f;
		m_light0.Diffuse.b = 0.70f;
	}
*/	
	// ¿©±â±îÁö ÀÓ½ÃÄÚµå 
	
	m_light0.Direction = SetLightDirection();
	g_pD3dDev->SetLight( 0, &m_light0 );
//	DBGOUT("LIGHT[D:%.2f,%.2f,%.2f][A:%.2f,%.2f,%.2f]\n", 
//		m_light0.Diffuse.r, m_light0.Diffuse.g, m_light0.Diffuse.b, 
//		m_light0.Ambient.r, m_light0.Ambient.g, m_light0.Ambient.b);
}
#endif
VOID CSceneData::SetDay()
{
	FLOG("CSceneData::SetDay()");
//	int nH = 4, nM = 60, nS = 60; // nH:½Ã°£ nM:ºÐ nS:ÃÊ ÀÇ ·® ¼ÂÆÃÇÒ¼ö ÀÖ´Ù.(¿¹> ÇÏ·ç->4½Ã°£ 1½Ã°£->60ºÐ 1ºÐ->60ÃÊ)-°ÔÀÓ»óÀÇ ½Ã°£
	int nTime = (GetTickCount() - m_dwStartTime)/1000 + m_nBaseTime;
	nTime = nTime%(TIME_HOUR*TIME_MINUTE*TIME_SECOND);

// 2004-10-20 by jschoi
	float fChangingCheckTime;
	float fChangingTime = 5 * TIME_SECOND;
	DWORD dwDayFogColor,dwNightFogColor;
	BYTE byDayRed,byDayGreen,byDayBlue,byNightRed,byNightGreen,byNightBlue,byFogRed,byFogGreen,byFogBlue;
//	dwDayFogColor = GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,TRUE);
//	dwNightFogColor = GetFogColor(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex,FALSE);
	dwDayFogColor = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)->DayFogColor;
	dwNightFogColor = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)->NightFogColor;
	
	if( nTime >= 0 && nTime < TIME_MINUTE*TIME_SECOND )
	{ // ¹ã
		if(g_pShuttleChild->GetCurrentBodyCondition() & ~BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, TRUE );
		}
		if( nTime < fChangingTime )
		{ // ¹ãÀÌ µÈÁö 5ºÐÀÌ³»
			fChangingCheckTime = (float)nTime;

			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fDiffuseR2*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fDiffuseG2*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fDiffuseB2*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fAmbientR2*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fAmbientG2*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1*(1.0f - (fChangingCheckTime/fChangingTime)) + m_pGround->m_projectInfo.fAmbientB2*(fChangingCheckTime/fChangingTime);

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			byDayRed =		(BYTE)(dwDayFogColor >> 16);
			byDayGreen =	(BYTE)(dwDayFogColor >> 8);
			byDayBlue =		(BYTE)(dwDayFogColor);
			byNightRed =	(BYTE)(dwNightFogColor >> 16);
			byNightGreen =	(BYTE)(dwNightFogColor >> 8);
			byNightBlue =	(BYTE)(dwNightFogColor);
			byFogRed = byDayRed*(1.0f - fChangingCheckTime/fChangingTime) + byNightRed*(fChangingCheckTime/fChangingTime);
			byFogGreen = byDayGreen*(1.0f - fChangingCheckTime/fChangingTime) + byNightGreen*(fChangingCheckTime/fChangingTime);	
			byFogBlue =	byDayBlue*(1.0f - fChangingCheckTime/fChangingTime) + byNightBlue*(fChangingCheckTime/fChangingTime);	
			m_dwFogColor = D3DCOLOR_ARGB(0x00,byFogRed,byFogGreen,byFogBlue);
		}
		else
		{ // ¹ã 5ºÐÀÌ Áö³­ ÀÌÈÄ
			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR2;
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG2;
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB2;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2;
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2;
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2;

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			m_dwFogColor = dwNightFogColor;
		}
//		m_bNight = TRUE;
		m_light0.Specular.r  = 0.4f;
		m_light0.Specular.g  = 0.4f;
		m_light0.Specular.b  = 0.4f;
	}
	else
	{ // ³·
		if(g_pShuttleChild->GetCurrentBodyCondition() & BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, FALSE );
		}
		if( nTime >= TIME_MINUTE*TIME_SECOND && nTime < TIME_MINUTE*TIME_SECOND + fChangingTime )
		{ // ³·ÀÌ µÈÁö 5ºÐÀÌ³»
			fChangingCheckTime = (float)(nTime - TIME_MINUTE*TIME_SECOND);
			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fDiffuseR1*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fDiffuseG1*(fChangingCheckTime/fChangingTime);
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fDiffuseB1*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fAmbientR1*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fAmbientG1*(fChangingCheckTime/fChangingTime);
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2*(1.0f - fChangingCheckTime/fChangingTime) + m_pGround->m_projectInfo.fAmbientB1*(fChangingCheckTime/fChangingTime);

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			byDayRed =		(BYTE)(dwDayFogColor >> 16);
			byDayGreen =	(BYTE)(dwDayFogColor >> 8);
			byDayBlue =		(BYTE)(dwDayFogColor);
			byNightRed =	(BYTE)(dwNightFogColor >> 16);
			byNightGreen =	(BYTE)(dwNightFogColor >> 8);
			byNightBlue =	(BYTE)(dwNightFogColor);
			byFogRed = byNightRed*(1.0f - fChangingCheckTime/fChangingTime) + byDayRed*(fChangingCheckTime/fChangingTime);
			byFogGreen = byNightGreen*(1.0f - fChangingCheckTime/fChangingTime) + byDayGreen*(fChangingCheckTime/fChangingTime);	
			byFogBlue =	byNightBlue*(1.0f - fChangingCheckTime/fChangingTime) + byDayBlue*(fChangingCheckTime/fChangingTime);	
			m_dwFogColor = D3DCOLOR_ARGB(0x00,byFogRed,byFogGreen,byFogBlue);
		}
		else
		{ // ³· 5ºÐÀÌ Áö³­ ÀÌÈÄ
			m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR1;
			m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG1;
			m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB1;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1;
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1;
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1;

			// 2004-11-15 by ydkim
//			if(g_pSOption->sGammaCtrl > 0)
				ChangeGammaOption( g_pSOption->sGammaCtrl );

			m_dwFogColor = dwDayFogColor;
		}
//		m_bNight = FALSE;
		m_light0.Specular.r  = 1.0f;
		m_light0.Specular.g  = 1.0f;
		m_light0.Specular.b  = 1.0f;
	}

	// 2004-10-22 by jschoi
	// ¾îµÎ¿öÁö±â Á÷Àü¿¡ ¹Ù·Î ´ÞÀÌ ¶á´Ù.
	// ¼­¼­È÷ ¹à¾ÆÁö¸é¼­ Áß°£Âë ¹à¾ÆÁö¸é ÅÂ¾çÀÌ ¶á´Ù.	
	if(	(nTime >= 0) &&											
		(nTime < (TIME_MINUTE*TIME_SECOND + fChangingTime/2)))	
	{
		m_bNight = TRUE;
	}
	else
	{
		m_bNight = FALSE;
	}
	m_light0.Direction = SetLightDirection();
	g_pD3dDev->SetLight( 0, &m_light0 );
//	int nH = 4, nM = 5, nS = 5; // nH:½Ã°£ nM:ºÐ nS:ÃÊ ÀÇ ·® ¼ÂÆÃÇÒ¼ö ÀÖ´Ù.(¿¹> ÇÏ·ç->4½Ã°£ 1½Ã°£->60ºÐ 1ºÐ->60ÃÊ)-°ÔÀÓ»óÀÇ ½Ã°£
/*	int nTime = (GetTickCount() - m_dwStartTime)/1000 + m_nBaseTime;
	nTime = nTime%(TIME_HOUR*TIME_MINUTE*TIME_SECOND);
	if(nTime >= 0 && nTime < 1*TIME_MINUTE*TIME_SECOND)
	{
		if(g_pShuttleChild->GetCurrentBodyCondition() & ~BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, TRUE );
		}
		float fChangeTime = (float)(TIME_DAY_CHANGE - nTime);
		if(fChangeTime >= 0)
		{ // ³·¿¡¼­ ¹ãÀ¸·Î ¹Ù²î´ÂÁß
			m_fSkyRedColor = m_pGround->m_projectInfo.fDiffuseR2 + 
				(m_pGround->m_projectInfo.fDiffuseR1 - m_pGround->m_projectInfo.fDiffuseR2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_fSkyGreenColor = m_pGround->m_projectInfo.fDiffuseG2 + 
				(m_pGround->m_projectInfo.fDiffuseG1 - m_pGround->m_projectInfo.fDiffuseG2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_fSkyBlueColor = m_pGround->m_projectInfo.fDiffuseB2 + 
				(m_pGround->m_projectInfo.fDiffuseB1 - m_pGround->m_projectInfo.fDiffuseB2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Diffuse.r = m_fSkyRedColor;
			m_light0.Diffuse.g = m_fSkyGreenColor;
			m_light0.Diffuse.b = m_fSkyBlueColor;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2 + 
				(m_pGround->m_projectInfo.fAmbientR1 - m_pGround->m_projectInfo.fAmbientR2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2 + 
				(m_pGround->m_projectInfo.fAmbientG1 - m_pGround->m_projectInfo.fAmbientG2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2 + 
				(m_pGround->m_projectInfo.fAmbientB1 - m_pGround->m_projectInfo.fAmbientB2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Direction = SetLightDirection();
			g_pD3dDev->SetLight( 0, &m_light0 );
			m_bNight = FALSE;
		}
		else
		{ // ¹ã
			m_fSkyRedColor = m_pGround->m_projectInfo.fDiffuseR2;
			m_fSkyGreenColor = m_pGround->m_projectInfo.fDiffuseG2;
			m_fSkyBlueColor = m_pGround->m_projectInfo.fDiffuseB2;
			m_light0.Diffuse.r = m_fSkyRedColor;
			m_light0.Diffuse.g = m_fSkyGreenColor;
			m_light0.Diffuse.b = m_fSkyBlueColor;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR2;
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG2;
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB2;
			m_light0.Direction = SetLightDirection();
			g_pD3dDev->SetLight( 0, &m_light0 );
			m_bNight = TRUE;
		}
	}
	else if(nTime >= 1*TIME_MINUTE*TIME_SECOND && nTime < TIME_HOUR*TIME_MINUTE*TIME_SECOND)
	{
		if(g_pShuttleChild->GetCurrentBodyCondition() & BODYCON_NIGHTFLY_MASK)
		{
			g_pD3dApp->SendFieldSocketChangeCharacterBodyCondition( BODYCON_NIGHTFLY_MASK, FALSE );
		}
		float fChangeTime = (float)(TIME_DAY_CHANGE - (nTime - 1*TIME_MINUTE*TIME_SECOND));
		if(fChangeTime >= 0)
		{ // ¹ã¿¡¼­ ³·À¸·Î ¹Ù²î´ÂÁß
			m_fSkyRedColor = m_pGround->m_projectInfo.fDiffuseR1 - 
				(m_pGround->m_projectInfo.fDiffuseR1 - m_pGround->m_projectInfo.fDiffuseR2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_fSkyGreenColor = m_pGround->m_projectInfo.fDiffuseG1 - 
				(m_pGround->m_projectInfo.fDiffuseG1 - m_pGround->m_projectInfo.fDiffuseG2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_fSkyBlueColor = m_pGround->m_projectInfo.fDiffuseB1 - 
				(m_pGround->m_projectInfo.fDiffuseB1 - m_pGround->m_projectInfo.fDiffuseB2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Diffuse.r = m_fSkyRedColor;
			m_light0.Diffuse.g = m_fSkyGreenColor;
			m_light0.Diffuse.b = m_fSkyBlueColor;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1 - 
				(m_pGround->m_projectInfo.fAmbientR1 - m_pGround->m_projectInfo.fAmbientR2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1 - 
				(m_pGround->m_projectInfo.fAmbientG1 - m_pGround->m_projectInfo.fAmbientG2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1 - 
				(m_pGround->m_projectInfo.fAmbientB1 - m_pGround->m_projectInfo.fAmbientB2)*
				(fChangeTime/TIME_DAY_CHANGE);
			m_light0.Direction = SetLightDirection();
			g_pD3dDev->SetLight( 0, &m_light0 );
			m_bNight = TRUE;
		}
		else
		{ // ³·
			m_fSkyRedColor = m_pGround->m_projectInfo.fDiffuseR1;
			m_fSkyGreenColor = m_pGround->m_projectInfo.fDiffuseG1;
			m_fSkyBlueColor = m_pGround->m_projectInfo.fDiffuseB1;
			m_light0.Diffuse.r = m_fSkyRedColor;
			m_light0.Diffuse.g = m_fSkyGreenColor;
			m_light0.Diffuse.b = m_fSkyBlueColor;
			m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1;
			m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1;
			m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1;
			m_light0.Direction = SetLightDirection();
			g_pD3dDev->SetLight( 0, &m_light0 );
			m_bNight = FALSE;
		}
	}
	::SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, !m_bNight );
#ifdef _DEBUG
	char buf[256];
	sprintf( buf, "LIGHT DIRECTION : [%.1f, %.1f, %.1f]\n", m_light0.Direction.x, m_light0.Direction.y, m_light0.Direction.z);
	DBGOUT(buf);
#endif
	*/
}

D3DXVECTOR3 CSceneData::SetLightDirection()
{
	FLOG("CSceneData::SetLightDirection()");
//#ifdef _DEBUG
	D3DXVECTOR3 vDirection = GetMapDirection(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, !m_bNight);
	D3DXVec3Normalize( &vDirection, &vDirection );
	return vDirection;
//#endif
/*
//	int nH = 4, nM = 5, nS = 5; // nH:½Ã°£ nM:ºÐ nS:ÃÊ ÀÇ ·® ¼ÂÆÃÇÒ¼ö ÀÖ´Ù.(¿¹> ÇÏ·ç->4½Ã°£ 1½Ã°£->60ºÐ 1ºÐ->60ÃÊ)-°ÔÀÓ»óÀÇ ½Ã°£
	int nTime = (GetTickCount() - m_dwStartTime) + m_nBaseTime*1000;
	nTime = nTime%(TIME_HOUR*TIME_MINUTE*TIME_SECOND*1000);
	D3DXMATRIX mat;
	D3DXVECTOR3 vVel = D3DXVECTOR3(1,0,0);
	D3DXVECTOR3 vDir;
	float fAngle;
	if(nTime >= 0 && nTime < 1*TIME_MINUTE*TIME_SECOND*1000)
	{
//		vDir = D3DXVECTOR3(0.0f,0.8f,-1.0f);
		vDir = D3DXVECTOR3(0.0f,-1.0f,1.0f);
		D3DXVec3Normalize(&vDir,&vDir);
//		fAngle = ((float)nTime)*((4.5f)/(float)(1*TIME_MINUTE*TIME_SECOND*1000));
		fAngle = ((float)nTime/(float)(1*TIME_MINUTE*TIME_SECOND*1000))*(PI/4);
	}
	else if(nTime >= 1*TIME_MINUTE*TIME_SECOND*1000 && nTime < TIME_HOUR*TIME_MINUTE*TIME_SECOND*1000)
	{
//		vDir = D3DXVECTOR3(0.3f,0.8f,-1.0f);
		vDir = D3DXVECTOR3(0.0f,-1.0f,1.0f);
		D3DXVec3Normalize(&vDir,&vDir);
//		fAngle = ((float)(nTime - 1*TIME_MINUTE*TIME_SECOND*1000))*((4.5f)/(float)((TIME_HOUR-1)*TIME_MINUTE*TIME_SECOND*1000));
		fAngle = ((float)(nTime - 1*TIME_MINUTE*TIME_SECOND*1000)/(float)((TIME_HOUR-1)*TIME_MINUTE*TIME_SECOND*1000))*(PI/4);
	}
//	D3DXMatrixRotationAxis(&mat,&vVel,-fAngle);
	D3DXMatrixRotationAxis(&mat,&vVel,fAngle);
	D3DXVec3TransformCoord(&vDir,&vDir,&mat);
	D3DXVec3Normalize(&vDir,&vDir);
	return vDir;
*/
}

int CSceneData::CheckMove()
{
	int Oldx,Oldy,Curx,Cury;
	Oldx = g_pD3dApp->m_pShuttleChild->m_vOldPos.x/(MAP_BLOCK_SIZE/2);
	Oldy = g_pD3dApp->m_pShuttleChild->m_vOldPos.z/(MAP_BLOCK_SIZE/2);
	Curx = g_pD3dApp->m_pShuttleChild->m_vPos.x/(MAP_BLOCK_SIZE/2);
	Cury = g_pD3dApp->m_pShuttleChild->m_vPos.z/(MAP_BLOCK_SIZE/2);

	if(Curx%2)
	{
		//¿À¸¥ÂÊ
		if(Cury%2)
		{
			//¾Æ·¡
			return BOTTOMRIGHT;
		}
		else
		{
			//À§
			return TOPRIGHT;
		}
	}
	else
	{
		//¿ÞÂÊ
		if(Cury%2)
		{
			//¾Æ·¡
			return BOTTOMLEFT;
		}
		else
		{
			// À§
			return TOPLEFT;
		}
	}

	return -1;	// error
}

void CSceneData::CalcCollisionRange(int nMoveType)
{
	// Ãæµ¹ Ã³¸® ¿µ¿ª °è»ê
//	int nTestNormal=0, nTestBig=0;

	m_vectorCollisionObjectPtrList.clear();
	TWO_BLOCK_INDEXES blockIdx;
	int x, z;
	
	switch(nMoveType)
	{
	case TOPLEFT:
		blockIdx.sMinX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE)-1);
		blockIdx.sMaxX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE));
		blockIdx.sMinZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE)-1);
		blockIdx.sMaxZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE));
		break;
	case TOPRIGHT:
		blockIdx.sMinX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE));
		blockIdx.sMaxX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE)+1);
		blockIdx.sMinZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE)-1);
		blockIdx.sMaxZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE));
		break;
	case BOTTOMLEFT:
		blockIdx.sMinX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE)-1);
		blockIdx.sMaxX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE));
		blockIdx.sMinZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE));
		blockIdx.sMaxZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE)+1);
		break;
	case BOTTOMRIGHT:
		blockIdx.sMinX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE));
		blockIdx.sMaxX = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE)+1);
		blockIdx.sMinZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE));
		blockIdx.sMaxZ = (int)((g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE)+1);
		break;
	}	

	int nMaxX = (int)m_pGround->m_fSizeMap_X/MAP_BLOCK_SIZE;
	int nMaxZ = (int)m_pGround->m_fSizeMap_Z/MAP_BLOCK_SIZE;
	if(blockIdx.sMinX<0)
		blockIdx.sMinX=0;
	if(blockIdx.sMaxX>=nMaxX)
		blockIdx.sMaxX=nMaxX-1;
	if(blockIdx.sMinZ<0)
		blockIdx.sMinZ=0;
	if(blockIdx.sMaxZ>=nMaxZ)
		blockIdx.sMaxZ=nMaxZ-1;

   	x = blockIdx.sMinX;
	while(x <= blockIdx.sMaxX)
	{
		z = blockIdx.sMinZ;
		while(z <= blockIdx.sMaxZ)
		{
			CObjectChild * pObj = (CObjectChild *)m_pGround->m_ppObjectList[x][z].m_pChild;

			while(pObj)
			{
				
				if(pObj->m_pObjectInfo)
				{	
					float fRadius = 0;
					if(pObj->m_pObjMesh)
					{
						fRadius = pObj->m_pObjMesh->m_fRadius;
					}
					if(D3DXVec3Length(&(pObj->m_vOriPos - g_pShuttleChild->m_vPos)) - fRadius < NORMAL_OBJECT_RADIUS_TO_I_DISTANCE)
					{
						m_vectorCollisionObjectPtrList.push_back(pObj);	
					}
				}
				pObj = (CObjectChild *)pObj->m_pNext;
			}
			z++;
		}
		x++;
	}
	
//	nTestNormal = m_vectorCollisionObjectPtrList.size();

	// 2005-02-11 by jschoi  Big Object ¸¦ Ãæµ¹ ¸®½ºÆ®¿¡ Ãß°¡
	CObjectChild * pBigObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;
	
	while ( pBigObj )
	{
		if(pBigObj->m_pObjectInfo)
		{	
			float fRadius = 0;
			if(pBigObj->m_pObjMesh)
			{
				fRadius = pBigObj->m_pObjMesh->m_fRadius;
			}	
			
			if( g_pShuttleChild && pBigObj->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_NO_COLLISION &&		//2013-05-22 by ssjung Ä³³ª´Ù ÀÍ¼Á¼Ç ¿À·ù ¿¹¹æ ÄÚµå Ãß°¡
				pBigObj->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_CONTOUR &&
				D3DXVec3Length(&(pBigObj->m_vOriPos - g_pShuttleChild->m_vPos)) - fRadius < BIG_OBJECT_RADIUS_TO_I_DISTANCE)
			{		
				
				// 2005-04-12 by jschoi - Tutorial
				if(g_pTutorial && g_pTutorial->IsTutorialMode() == TRUE )		//2013-05-22 by ssjung Ä³³ª´Ù ÀÍ¼Á¼Ç ¿À·ù ¿¹¹æ ÄÚµå Ãß°¡
				{
					if( pBigObj->m_nCode == TUTORIAL_GATE )
					{
						if(	g_pTutorial->GetChapter() == L1 && 
							g_pTutorial->IsEnableTutorialGate(pBigObj->m_vPos))
						{
							m_vectorCollisionObjectPtrList.push_back(pBigObj);
						}
					}
					else
					{
						m_vectorCollisionObjectPtrList.push_back(pBigObj);
					}
				}
				else
				{
					m_vectorCollisionObjectPtrList.push_back(pBigObj);
				}
			}
		}
		pBigObj = (CObjectChild *)pBigObj->m_pNext;
	}	

	// Water Object ¸¦ Ãæµ¹ ¸®½ºÆ®¿¡ Ãß°¡
	CObjectChild* pWaterObj = (CObjectChild*)m_pGround->m_pWaterObject->m_pChild;
	
	while ( pWaterObj )
	{
		if(pWaterObj->m_pObjectInfo)
		{	
			float fRadius = 0;
			if(pWaterObj->m_pObjMesh)
			{
				fRadius = pWaterObj->m_pObjMesh->m_fRadius;
			}	
		
		if( pWaterObj->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_NO_COLLISION &&
			pWaterObj->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_CONTOUR &&
			D3DXVec3Length(&(pWaterObj->m_vOriPos - g_pShuttleChild->m_vPos)) - fRadius < BIG_OBJECT_RADIUS_TO_I_DISTANCE)
		{		
			m_vectorCollisionObjectPtrList.push_back(pWaterObj);
		}
		}
		pWaterObj = (CObjectChild *)pWaterObj->m_pNext;
	}	


	// 2004-11-29 by jschoi - ¿ÀºêÁ§Æ® ¸ó½ºÅÍ ¿ÀºêÁ§Æ®¸¦ Ãæµ¹ ¸®½ºÆ®¿¡ Ãß°¡
	CObjectChild * pObjectMonster = (CObjectChild *)m_pGround->m_pObjectMonster->m_pChild;
	
	while ( pObjectMonster )
	{	
		if(	pObjectMonster->m_pObjectInfo && 
			pObjectMonster->m_bEnableObjectMonsterObject == TRUE)
		{	
			float fRadius = 0;
			if(	pObjectMonster->m_pObjMesh	)
			{
				fRadius = pObjectMonster->m_pObjMesh->m_fRadius;
			}	
			
			if(D3DXVec3Length(&(pObjectMonster->m_vOriPos - g_pShuttleChild->m_vPos)) - fRadius < BIG_OBJECT_RADIUS_TO_I_DISTANCE)
			{		
				m_vectorCollisionObjectPtrList.push_back(pObjectMonster);
			}
		}
		pObjectMonster = (CObjectChild *)pObjectMonster->m_pNext;
	}	


}

VOID CSceneData::CheckObjectRenderList(BOOL bCheckRange)
{
	FLOG("CSceneData::CheckObjectRenderList()");

	BOOL bIsMoveRange = TRUE;

	int nMoveType ;

	if(g_pD3dApp->m_pShuttleChild->m_ptOldPoint.x == (int)g_pD3dApp->m_pShuttleChild->m_vPos.x/MAP_BLOCK_SIZE
		&& g_pD3dApp->m_pShuttleChild->m_ptOldPoint.y == (int)g_pD3dApp->m_pShuttleChild->m_vPos.z/MAP_BLOCK_SIZE)
	{
		bIsMoveRange = FALSE;		
	}

	if( bIsMoveRange || bCheckRange == FALSE ) // »ç°¢¿µ¿ªÀ» ¹þ¾î³µÀ» °æ¿ì
	{
		m_vectorRangeObjectPtrList.clear();

		TWO_BLOCK_INDEXES blockIdx;
		int x, z;
//		int nViewRange = m_fFogStartValue > MAP_BLOCK_SIZE ? m_fFogStartValue : MAP_BLOCK_SIZE;


		// 2005-04-29 by jschoi
		float fOptionDistance;
		if(g_pTutorial->IsTutorialMode() == TRUE)
		{
			fOptionDistance = m_fFogEndValue;
		}
		else
		{
			fOptionDistance = (m_fFogEndValue*g_pSOption->sTerrainRender)/MAX_OPTION_VALUE;
		}

		int nViewRange = fOptionDistance > MAP_BLOCK_SIZE ?
						 fOptionDistance : MAP_BLOCK_SIZE;

		m_pGround->GetBlockAdjacentToPosition(g_pD3dApp->m_pShuttleChild->m_vPos.x, 
			g_pD3dApp->m_pShuttleChild->m_vPos.z, 
			nViewRange/*SIZE_OBJECT_VISIBLERECT*/, 
			blockIdx);			//

		// add by frustum
		x = blockIdx.sMinX;
		while(x <= blockIdx.sMaxX)
		{
			z = blockIdx.sMinZ;
			while(z <= blockIdx.sMaxZ)
			{
				CObjectChild * pObj = (CObjectChild *)m_pGround->m_ppObjectList[x][z].m_pChild;

					while(pObj)
					{

						if(	pObj->m_pObjectInfo )//&& 
//							pObj->m_pObjectInfo->Collision )
//							(/*pObj->m_bCheckColl ||*/ g_pD3dApp->m_bDegree != 0 || g_pD3dApp->m_dwGameState != _GAME) )
						{
							m_vectorRangeObjectPtrList.push_back(pObj);
						}
						pObj = (CObjectChild *)pObj->m_pNext;
					}
				z++;
			}
			x++;
		}
		
		// 2004.06.08 syjun,jschoi
		// ÀýµÎÃ¼ ÄÃ¸µÇÒ ¸®½ºÆ®¿¡ BigObject Ãß°¡
		CObjectChild * pBigObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;

		while ( pBigObj )
		{
			if(pBigObj->m_pObjectInfo)
			{
				// 2005-04-12 by jschoi - Tutorial
				if(	g_pTutorial->IsTutorialMode() == TRUE )
				{
					if( pBigObj->m_nCode == TUTORIAL_GATE)
					{
						if(	g_pTutorial->GetLesson() == L1 && 
							g_pTutorial->IsEnableTutorialGate(pBigObj->m_vPos))
						{
							m_vectorRangeObjectPtrList.push_back(pBigObj);
						}
					}
					else
					{
						m_vectorRangeObjectPtrList.push_back(pBigObj);
					}
				}
				else
				{
					m_vectorRangeObjectPtrList.push_back(pBigObj);
				}
			}
			pBigObj = (CObjectChild *)pBigObj->m_pNext;
		}	
			

		// 2005-02-11 by jschoi  ÀýµÎÃ¼ ÄÃ¸µÇÒ ¸®½ºÆ®¿¡ ¹° ¿ÀºêÁ§Æ® Ãß°¡
		CObjectChild * pWaterObj = (CObjectChild *)m_pGround->m_pWaterObject->m_pChild;

		while ( pWaterObj )
		{
			if(pWaterObj->m_pObjectInfo)
			{
				m_vectorRangeObjectPtrList.push_back(pWaterObj);
			}
			pWaterObj = (CObjectChild*)pWaterObj->m_pNext;
		}	
		

		// 2004-11-29 by jschoi - ¿ÀºêÁ§Æ® ¸ó½ºÅÍ¸¦ ÀýµÎÃ¼ ÄÃ¸µÇÒ ¸®½ºÆ®¿¡ Ãß°¡
		CObjectChild * pObjectMonster = (CObjectChild *)m_pGround->m_pObjectMonster->m_pChild;

		while ( pObjectMonster )
		{
			if(	pObjectMonster->m_pObjectInfo &&
				pObjectMonster->m_bEnableObjectMonsterObject == TRUE)
			{
				m_vectorRangeObjectPtrList.push_back(pObjectMonster);
			}
			pObjectMonster = (CObjectChild *)pObjectMonster->m_pNext;
		}	

	}
	
	nMoveType = CheckMove();

	CalcCollisionRange(nMoveType);

	m_vectorCulledObjectPtrList.clear();

	// 2004.06.08 syjun,jschoi
	// ¸ðµç BigObject¿Í ¿µ¿ª ÄÃ¸µµÈ ¸®½ºÆ®¸¦ ÀýµÎÃ¼ ÄÃ¸µ
	vectorCObjectChildPtr::iterator itObj = m_vectorRangeObjectPtrList.begin();
	while(itObj != m_vectorRangeObjectPtrList.end())
	{
		ASSERT_ASSERT((*itObj));
		float fRadius = 0;
		if((*itObj)->m_pObjMesh)
		{
			fRadius = (*itObj)->m_pObjMesh->m_fRadius;
		}
		if(	(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_EFFECT &&
			D3DXVec3Length(&((*itObj)->m_vOriPos - g_pCamera->m_vCamCurrentPos)) - fRadius < m_fFogEndValue )
		{
			m_vectorCulledObjectPtrList.push_back(*itObj);
			itObj++;
			continue;
		}
		BOOL bCheckIn = g_pFrustum->CheckSphere( (*itObj)->m_vOriPos.x, (*itObj)->m_vOriPos.y, (*itObj)->m_vOriPos.z, fRadius );
		if( bCheckIn )
		{
			if((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG)
			{
				m_vectorCulledObjectPtrList.push_back(*itObj);
			}
			else if(((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CULLED ||
					(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_NO_COLLISION ||
					(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CONTOUR) &&
					D3DXVec3Length(&((*itObj)->m_vOriPos - g_pCamera->m_vCamCurrentPos)) - fRadius < m_fFogEndValue)
			{
				if((*itObj)->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_CONTOUR
					|| FALSE == g_pSOption->sLowQuality)
				{// 2005-07-11 by ispark, ÃÖ¼Ò ÇÁ·¹ÀÓÀÏ¶§ ¿Ü°¢ ¿ÀºêÁ§Æ® ·»´õ¸µ Á¦¿Ü
					m_vectorCulledObjectPtrList.push_back(*itObj);
				}
			}
			else if(((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_NORMAL ||
					(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_SHOP_NPC) &&
					D3DXVec3Length(&((*itObj)->m_vOriPos - g_pCamera->m_vCamCurrentPos)) - fRadius < m_fFogEndValue)
			{
				m_vectorCulledObjectPtrList.push_back(*itObj);
			}
			else if((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_MONSTER_OBJECT &&
					(*itObj)->m_bEnableObjectMonsterObject == TRUE )
			{
				m_vectorCulledObjectPtrList.push_back(*itObj);
			}
		}
//		if( bCheckIn && 
//			((*itObj)->m_pObjectInfo->RadiusForClient > BIG_OBJECT_SIZE || 
//			D3DXVec3Length(&((*itObj)->m_vOriPos - g_pCamera->m_vCamCurrentPos)) - fRadius<m_fFogEndValue))
//		{
//			m_vectorCulledObjectPtrList.push_back((*itObj));
//		}
		itObj++;
	}


	g_pShuttleChild->m_ptOldPoint.x = g_pShuttleChild->m_vPos.x/TILE_SIZE;
	g_pShuttleChild->m_ptOldPoint.y = g_pShuttleChild->m_vPos.z/TILE_SIZE;
	
	// 2004-12-15 by jschoi - °Å¸®¿¡ µû¶ó¼­ Á¤·Ä
	sort(m_vectorCulledObjectPtrList.begin(), m_vectorCulledObjectPtrList.end(), CompareUnit() );
}	

VOID CSceneData::SetObjectRenderList()
{
	FLOG("CSceneData::SetObjectRenderList()");
	
	int nMoveType;
	m_vectorRangeObjectPtrList.clear();
	TWO_BLOCK_INDEXES blockIdx;
	int x, z;
//	int nVal  = ((int)m_fFogEndValue) % MAP_BLOCK_SIZE;
	int nViewRange = m_fFogStartValue > MAP_BLOCK_SIZE ? m_fFogStartValue : MAP_BLOCK_SIZE;

	m_pGround->GetBlockAdjacentToPosition(g_pD3dApp->m_pShuttleChild->m_vPos.x, 
		g_pD3dApp->m_pShuttleChild->m_vPos.z, 
		nViewRange/*SIZE_OBJECT_VISIBLERECT*/, 
		blockIdx);			// 2016-01-08 exception at this point, m_pGround == nullptr

	// add by frustum
	x = blockIdx.sMinX;
	while(x <= blockIdx.sMaxX)
	{
		z = blockIdx.sMinZ;
		while(z <= blockIdx.sMaxZ)
		{
			CObjectChild * pObj = (CObjectChild *)m_pGround->m_ppObjectList[x][z].m_pChild;
			while(pObj)
			{
				if( pObj->m_pObjectInfo)
				{
					m_vectorRangeObjectPtrList.push_back(pObj);
				}
				pObj = (CObjectChild *)pObj->m_pNext;
			}
			z++;
		}
		x++;
	}
	
	// 2004.06.08 syjun,jschoi
	// ÀýµÎÃ¼ ÄÃ¸µÇÒ ¸®½ºÆ®¿¡ BigObject Ãß°¡
	CObjectChild * pBigObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;
	
	while ( pBigObj )
	{
		if(pBigObj->m_pObjectInfo)
		{
			m_vectorRangeObjectPtrList.push_back(pBigObj);
		}
		pBigObj = (CObjectChild *)pBigObj->m_pNext;
	}	
	
	// 2005-02-11 by jschoi  ÀýµÎÃ¼ ÄÃ¸µÇÒ ¸®½ºÆ®¿¡ ¹° ¿ÀºêÁ§Æ® Ãß°¡
	CObjectChild * pWaterObj = (CObjectChild *)m_pGround->m_pWaterObject->m_pChild;
	
	while ( pWaterObj )
	{
		if(pWaterObj->m_pObjectInfo)
		{
			m_vectorRangeObjectPtrList.push_back(pWaterObj);
		}
		pWaterObj = (CObjectChild *)pWaterObj->m_pNext;
	}	

	// 2004-11-29 by jschoi - ¿ÀºêÁ§Æ® ¸ó½ºÅÍ¸¦ ÀýµÎÃ¼ ÄÃ¸µÇÒ ¸®½ºÆ®¿¡ Ãß°¡ 
	CObjectChild * pObjectMonster = (CObjectChild *)m_pGround->m_pObjectMonster->m_pChild;
	
	while ( pObjectMonster )
	{
		if(	pObjectMonster->m_pObjectInfo &&
			pObjectMonster->m_bEnableObjectMonsterObject == TRUE )
		{
			m_vectorRangeObjectPtrList.push_back(pObjectMonster);
		}
		pObjectMonster = (CObjectChild *)pObjectMonster->m_pNext;
	}	


	nMoveType = CheckMove();

	CalcCollisionRange(nMoveType);

	m_vectorCulledObjectPtrList.clear();
	
	// 2004.06.08 syjun,jschoi
	// ¸ðµç BigObject¿Í ¿µ¿ª ÄÃ¸µµÈ ¸®½ºÆ®¸¦ ÀýµÎÃ¼ ÄÃ¸µ
	vectorCObjectChildPtr::iterator itObj = m_vectorRangeObjectPtrList.begin();
	while(itObj != m_vectorRangeObjectPtrList.end())
	{
		float fRadius = 0;
		if((*itObj)->m_pObjMesh)
		{
			fRadius = (*itObj)->m_pObjMesh->m_fRadius;
		}
		BOOL bCheckIn = g_pFrustum->CheckSphere( (*itObj)->m_vOriPos.x, (*itObj)->m_vOriPos.y, (*itObj)->m_vOriPos.z, fRadius );
		if( bCheckIn )
		{
			if((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG)
			{
				m_vectorCulledObjectPtrList.push_back(*itObj);
			}
			else if(((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CULLED ||
					(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_NO_COLLISION ||
					(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CONTOUR) &&
					D3DXVec3Length(&((*itObj)->m_vOriPos - g_pShuttleChild->m_vPos)) - fRadius<m_fFogEndValue)
			{
				if((*itObj)->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_CONTOUR
					|| FALSE == g_pSOption->sLowQuality)
				{// 2005-07-11 by ispark, ÃÖ¼Ò ÇÁ·¹ÀÓÀÏ¶§ ¿Ü°¢ ¿ÀºêÁ§Æ® ·»´õ¸µ Á¦¿Ü
					m_vectorCulledObjectPtrList.push_back(*itObj);
				}
			}
			else if(((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_NORMAL ||
					(*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_SHOP_NPC) &&
					D3DXVec3Length(&((*itObj)->m_vOriPos - g_pShuttleChild->m_vPos)) - fRadius<m_fFogEndValue)
			{
				m_vectorCulledObjectPtrList.push_back(*itObj);
			}
			else if((*itObj)->m_pObjectInfo->ObjectRenderType == OBJECT_MONSTER_OBJECT &&
					(*itObj)->m_bEnableObjectMonsterObject == TRUE )
			{
				m_vectorCulledObjectPtrList.push_back(*itObj);
			}

		}
//		if( bCheckIn && 
//			((*itObj)->m_pObjectInfo->RadiusForClient > BIG_OBJECT_SIZE || 
//			D3DXVec3Length(&((*itObj)->m_vOriPos - g_pCamera->m_vCamCurrentPos)) - fRadius<m_fFogEndValue))
//		{
//			m_vectorCulledObjectPtrList.push_back((*itObj));
//		}
		itObj++;
	}

	return;
}	

VOID CSceneData::ChangeResource()
{
	FLOG("CSceneData::ChangeResource()");
	util::del(m_pData);
	m_pData = new CGameData;
	char strPath[256];
	wsprintf(strPath,".\\Res-Map\\tileset%d%02d",g_pD3dApp->m_bDegree,m_pGround->m_projectInfo.useTileSetIndex);
	if( strlen(strPath) > 0 )
	{
		m_pData->SetFile(strPath,FALSE, NULL,0);
	}
//	LoadTex();
}

VOID CSceneData::DeleteObjectList(int nIndex)
{
//	FLOG("CSceneData::DeleteObjectList(int nIndex)");
//	CMapObjectIterator itObj = m_mapObjectList.find(nIndex);
//	if(itObj != m_mapObjectList.end())
//	{
//		m_mapObjectList.erase(itObj);
//	}
}

VOID CSceneData::InsertToBlockData(CAtumNode * pNode)
{
	FLOG("CSceneData::InsertToBlockData(CAtumNode * pNode)");
	if(pNode->m_dwPartType == _ENEMY)
	{
		CEnemyData * pEnemy = (CEnemyData *)pNode;
		int nNew = pEnemy->m_ptCurRegion.x*m_nBlockSizeY + pEnemy->m_ptCurRegion.y;
		if(pEnemy->m_ptCurRegion.x >= 0 && pEnemy->m_ptCurRegion.x < m_nBlockSizeX 
			&& pEnemy->m_ptCurRegion.y >= 0 && pEnemy->m_ptCurRegion.y < m_nBlockSizeY)
		{
			m_vecEnemyBlockList[nNew].push_back(pEnemy);
		}
		else
		{
			DBGOUT("Critical Error : InsertToBlockData(Enemy)\n");
		}
	}
	else if(pNode->m_dwPartType == _MONSTER)
	{
		CMonsterData * pMonster = (CMonsterData *)pNode;
		int nNew = pMonster->m_ptCurRegion.x*m_nBlockSizeY + pMonster->m_ptCurRegion.y;
		if(pMonster->m_ptCurRegion.x >= 0 && pMonster->m_ptCurRegion.x < m_nBlockSizeX 
			&& pMonster->m_ptCurRegion.y >= 0 && pMonster->m_ptCurRegion.y < m_nBlockSizeY)
		{
			m_vecMonsterList[nNew].push_back(pMonster);
		}
		else
		{
			DBGOUT("Critical Error  : InsertToBlockData(Monster)\n");
		}
	}
}

VOID CSceneData::ChangeToBlockData(CAtumNode * pNode)
{
	FLOG("CSceneData::ChangeToBlockData(CAtumNode * pNode)");
	bool bChange = false;
	if(pNode->m_dwPartType == _ENEMY)
	{
		CEnemyData * pEnemy = (CEnemyData *)pNode;
		int nOld = pEnemy->m_ptOldRegion.x*m_nBlockSizeY + pEnemy->m_ptOldRegion.y;
		int nNew = pEnemy->m_ptCurRegion.x*m_nBlockSizeY + pEnemy->m_ptCurRegion.y;
		if(nOld < m_nBlockSizeX*m_nBlockSizeY)
		{
			CVecEnemyIterator it = m_vecEnemyBlockList[nOld].begin();
			while(it != m_vecEnemyBlockList[nOld].end())
			{
				if(*it == pEnemy)
				{
					m_vecEnemyBlockList[nOld].erase(it);
					bChange = true;
					break;
				}
				it++;
			}
			if(!bChange)
			{
				DBGOUT("Critical Error: ChangeToBlockData(Enemy)\n");
			}
			if(nNew < m_nBlockSizeX*m_nBlockSizeY)
			{
				m_vecEnemyBlockList[nNew].push_back(pEnemy);
			}
		}
	}
	else if(pNode->m_dwPartType == _MONSTER)
	{
		CMonsterData * pMonster = (CMonsterData *)pNode;
		int nOld = pMonster->m_ptOldRegion.x*m_nBlockSizeY + pMonster->m_ptOldRegion.y;
		int nNew = pMonster->m_ptCurRegion.x*m_nBlockSizeY + pMonster->m_ptCurRegion.y;
		if(nOld < m_nBlockSizeX*m_nBlockSizeY)
		{
			CVecMonsterIterator it = m_vecMonsterList[nOld].begin();
			while(it != m_vecMonsterList[nOld].end())
			{
				if(*it == pMonster)
				{
					m_vecMonsterList[nOld].erase(it);
					bChange = true;
					break;
				}
				it++;
			}
			if(!bChange)
			{
				DBGOUT("Critical Error : ChangeToBlockData(Monster)\n");
			}
			if(nNew < m_nBlockSizeX*m_nBlockSizeY)
			{
				m_vecMonsterList[nNew].push_back(pMonster);
			}
		}
	}
}

VOID CSceneData::DeleteToBlockData(CAtumNode * pNode)
{
	FLOG("CSceneData::DeleteToBlockData(CAtumNode * pNode)");
	// ¿þÆùÁß ÀÌ Ä³¸¯À» ÇâÇÏ´Â °Íµé
	CWeapon * pWeapon = (CWeapon *)m_pWeaponData->m_pChild;
	while(pWeapon)
	{
		if(pWeapon->m_pTarget == pNode)
		{
			pWeapon->m_pTarget = NULL;
		}
		else if(pWeapon->m_pAttacker == pNode)
		{
			// 2005-08-03 by ispark
			// Àû ¹Ì»çÀÏÀ» ³Ö¾î¶ó
			pWeapon->m_pAttacker = NULL;
			pWeapon->m_bUsing = FALSE;
		}
		pWeapon = (CWeapon *)pWeapon->m_pNext;
	}
	// shuttleÀÇ Å¸°ÙÀÎ °æ¿ì
	if(g_pShuttleChild->m_pOrderTarget && 
		g_pShuttleChild->m_pOrderTarget == pNode)
	{
		g_pShuttleChild->m_pOrderTarget = NULL;
	}
	if(g_pShuttleChild->m_pTarget && 
		g_pShuttleChild->m_pTarget == pNode)
	{
		g_pShuttleChild->m_pTarget = NULL;
	}
	// À¯´Ö¿¡ ÀåÂøµÈ ¾ÆÀÌÅÛ
	DeleteEffect( pNode );
	if(pNode->m_dwPartType == _ENEMY)
	{
		CEnemyData * pEnemy = (CEnemyData *)pNode;
		int nDel = pEnemy->m_ptCurRegion.x*m_nBlockSizeY + pEnemy->m_ptCurRegion.y;
		if(nDel < m_nBlockSizeY*m_nBlockSizeX)
		{
			CVecEnemyIterator it = m_vecEnemyBlockList[nDel].begin();
			while(it != m_vecEnemyBlockList[nDel].end())
			{
				if(*it == pEnemy)
				{
					m_vecEnemyBlockList[nDel].erase(it);
					return;
				}
				it++;
			}
		}
		DBGOUT("Critical Error : DeleteToBlockData(Enemy)\n");
	}
	else if(pNode->m_dwPartType == _MONSTER)
	{
		CMonsterData * pMonster = (CMonsterData *)pNode;
		int nDel = pMonster->m_ptCurRegion.x*m_nBlockSizeY + pMonster->m_ptCurRegion.y;
		if(nDel < m_nBlockSizeY*m_nBlockSizeX)
		{
			CVecMonsterIterator it = m_vecMonsterList[nDel].begin();
			while(it != m_vecMonsterList[nDel].end())
			{
				if(*it == pMonster)
				{
					m_vecMonsterList[nDel].erase(it);
					return;
				}
				it++;
			}
		}
		DBGOUT("Critical Error : DeleteToBlockData(Monster)\n");
	}
}

VOID CSceneData::SetupLights()
{
	FLOG("CSceneData::SetupLights()");
	// 0:ÁÖ¶óÀÌÆ® 1:ÀÌÆåÆ®¿ë¶óÀÌÆ®(Á¤¸é¿¡¼­ ¹à°Ô) 2:¸ó½ºÅÍ µ¥¹ÌÁö½Ã 3:¼ÅÆ²¿ë ¶óÀÌÆ®(À§¿¡¼­ ¾Æ·¡·Î)
	// Èò»öÀÇ Á¡±¤¿ø ¼³Á¤
 	D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0);
	/////////   light0   /////////////////// ÀüÃ¼¸¦ ºñÃá´Ù.
    ZeroMemory( &m_light0, sizeof(D3DLIGHT9) );
    m_light0.Type       = D3DLIGHT_DIRECTIONAL;
	if(m_pGround)
	{
		m_light0.Diffuse.r = m_pGround->m_projectInfo.fDiffuseR1;
		m_light0.Diffuse.g = m_pGround->m_projectInfo.fDiffuseG1;
		m_light0.Diffuse.b = m_pGround->m_projectInfo.fDiffuseB1;
		m_light0.Ambient.r = m_pGround->m_projectInfo.fAmbientR1;
		m_light0.Ambient.g = m_pGround->m_projectInfo.fAmbientG1;
		m_light0.Ambient.b = m_pGround->m_projectInfo.fAmbientB1;
		m_light0.Specular.r  = 1.0f;
		m_light0.Specular.g  = 1.0f;
		m_light0.Specular.b  = 1.0f;
	}
	else
	{
		m_light0.Diffuse.r  = 1.0f;
		m_light0.Diffuse.g  = 1.0f;
		m_light0.Diffuse.b  = 1.0f;
		m_light0.Ambient.r  = 0.3f;
		m_light0.Ambient.g  = 0.3f;
		m_light0.Ambient.b  = 0.5f;
		m_light0.Specular.r  = 1.0f;
		m_light0.Specular.g  = 1.0f;
		m_light0.Specular.b  = 1.0f;
	}
    m_light0.Direction  = D3DXVECTOR3( -1, -0.1f, 0 );
 	g_pD3dDev->SetLight( 0, &m_light0 );	  
    //2007-02-28 dgwoo ºñ½ºÅ¸¿¡¼­ µð¹ÙÀÌ½º ¸®¼Â½Ã Á¶¸íÀ» (À¯È¿,¹«È¿) °ªÀ» º¯°æÇØÁà¾ß Àç ¼ÂÆÃµÈ´Ù.
    //g_pD3dDev->LightEnable( 0, TRUE );
	g_pD3dDev->LightEnable( 0, FALSE); 

	/////////   light1   ///////////////////->Çìµå¶óÀÌÆ®
	ZeroMemory( &m_light1, sizeof(D3DLIGHT9) );
	m_light1.Type         = D3DLIGHT_SPOT;
	m_light1.Diffuse.r    = 0.9f;
	m_light1.Diffuse.g    = 0.9f;
	m_light1.Diffuse.b    = 0.9f;
	m_light1.Ambient.r    = 0.0f;
	m_light1.Ambient.g    = 0.0f;
	m_light1.Ambient.b    = 0.0f;
	m_light1.Specular.r   = 1.0f;
	m_light1.Specular.g   = 1.0f;
	m_light1.Specular.b   = 1.0f;
    m_light1.Position	  = D3DXVECTOR3( 0, 0, 0 );
    m_light1.Direction    = D3DXVECTOR3( 0, 1, 0 );
    m_light1.Theta        = 0.5f;
    m_light1.Phi          = 1.0f;
    m_light1.Falloff      = 1.0f;
    m_light1.Attenuation0 = 1.0f;
	m_light1.Range		  = 0.0f;
 	g_pD3dDev->SetLight( 1, &m_light1 );
	g_pD3dDev->LightEnable( 1, FALSE );
	/////////   light2   ///////////////////->ÀÌÆåÆ® ¶óÀÌÆ®
	ZeroMemory( &m_light2, sizeof(D3DLIGHT9) );
	m_light2.Type       = D3DLIGHT_DIRECTIONAL;
	m_light2.Diffuse.r  = 1.0f;
	m_light2.Diffuse.g  = 1.0f;
	m_light2.Diffuse.b  = 1.0f;
	m_light2.Ambient.r  = 1.0f;
	m_light2.Ambient.g  = 1.0f;
	m_light2.Ambient.b  = 1.0f;
	m_light2.Direction = D3DXVECTOR3( -1, 0, 0 );
	/////////   light3   ///////////////////->¼ÅÆ² ºñÃâ¶§ »ç¿ë - À§¿¡¼­ ¾Æ·¡·Î ºñÃá´Ù.
	ZeroMemory( &m_light3, sizeof(D3DLIGHT9) );
	m_light3.Type       = D3DLIGHT_DIRECTIONAL;
	m_light3.Diffuse.r  = 180.0f/255.0f;
	m_light3.Diffuse.g  = 180.0f/255.0f;
	m_light3.Diffuse.b  = 180.0f/255.0f;
	m_light3.Ambient.r  = 15.0f/255.0f;
	m_light3.Ambient.g  = 20.0f/255.0f;
	m_light3.Ambient.b  = 25.0f/255.0f;
	m_light3.Direction = D3DXVECTOR3( 0, -1, 0 );
 	g_pD3dDev->SetLight( 3, &m_light3 );
	g_pD3dDev->LightEnable( 3, FALSE );

	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::CreateWeaponMonsterMissile(MSG_FC_MISSILE_MOVE_OK* pMsg)
/// \brief		Monster Weapon missileÀ» »ý¼ºÇÑ´Ù.»ç¿ëÇÏÁö ¾ÊÀ½
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	CreateWeaponMonsterSecondary¿Í ÄÚµå°¡ °ÅÀÇ °°´Ù. °ð ÇÏ³ª·Î ÇÕÃÄ¾ß ÇÑ´Ù.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::CreateWeaponMonsterMissile(MSG_FC_MISSILE_MOVE_OK* pMsg)
{
//	FLOG("CSceneData::CreateMonsterMissileWeapon(MSG_FC_MISSILE_MOVE_OK* pMsg)");
//	if(m_pWeaponData)
//	{
//		CMapMonsterIterator itMonster = m_mapMonsterList.find(pMsg->MonsterIndex);
//		if(itMonster != m_mapMonsterList.end())
//		{
//			if(pMsg->ItemNum == 7900005)
//			{
//				CWeaponMonsterData * pWeapon = new CWeaponMonsterData(pMsg,itMonster->second);
//				m_pWeaponData->AddChild(pWeapon);
//			}
//		}
//	}
}
/*
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::CreateWeaponMonsterSecondary(MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg)
/// \brief		¸ó½ºÅÍ 2Çü ¹«±â¸¦ »ý¼ºÇÑ´Ù.
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	Áö¿ï°Í!
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::CreateWeaponMonsterSecondary(MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg)
{
//	FLOG("CSceneData::CreateWeaponMonsterSecondary(MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg)");
//	if(m_pWeaponData)
//	{
//		CMapMonsterIterator itMonster = m_mapMonsterList.find(pMsg->AttackIndex);
//		if(itMonster != m_mapMonsterList.end())
//		{
//			if(pMsg->WeaponItemNumber == 7900005)
//			{
//				CWeaponMonsterData * pWeapon = new CWeaponMonsterData(pMsg,itMonster->second);
//				m_pWeaponData->AddChild(pWeapon);
//			}
//		}
//	}
}
*/

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		dgwoo
/// \date		2007-02-09 ~ 2007-02-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::AddFieldItemScanObject(CAtumNode * pUnitData, float fCheckTime, UINT uObjId, DWORD dwPartType, 
										D3DXVECTOR3	vVel, D3DXVECTOR3 vUp, D3DXVECTOR3 *i_pPos/*=NULL*/)
{
	if(pUnitData)
	{
		// 2007-04-19 by bhsohn ¼­Ä¡¾ÆÀÌ ¾ÆÀÌÅÛ Ãß°¡
		//CItemData* pItemData = new CItemData(pUnitData,fCheckTime);
		CItemData* pItemData = new CItemData(pUnitData,fCheckTime, uObjId, dwPartType,
												vVel, vUp, i_pPos);		
		// end 2007-04-19 by bhsohn ¼­Ä¡¾ÆÀÌ ¾ÆÀÌÅÛ Ãß°¡

		m_vecScanData.push_back(pItemData);
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::AddFieldItemItemSHowItem( MSG_FC_ITEM_SHOW_ITEM *pMsg )
/// \brief		ÇÊµå¿¡ ¾ÆÀÌÅÛ »ý¼º(¾Æ·¡ ÇÔ¼öµé ÇÕÃÄ¾ß ÇÔ)
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::AddFieldItemItemSHowItem( MSG_FC_ITEM_SHOW_ITEM *pMsg )
{
	FLOG("CSceneData::AddFieldItemItemSHowItem( MSG_FC_ITEM_SHOW_ITEM *pMsg )");
//#ifdef _DEBUG
//	char buf[128];
//	ITEM* pItem = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
//	wsprintf(buf,"¾ÆÀÌÅÛ [%s,%d] (%d°³)°¡ ¶³¾îÁ³½À´Ï´Ù.",pItem ? pItem->ItemName : "Á¤º¸¾øÀ½", pMsg->ItemNum, pMsg->Amount);
//	g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_SKILL_USE);
//#endif
	if(m_pItemData)
	{
		CItemData *pItem = FindFieldItemByFieldIndex( pMsg->ItemFieldIndex );
		if(!pItem)
		{
			pItem = new CItemData(pMsg);
			m_pItemData->AddChild(pItem);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::AddFieldItemBattleDropDummyOk( CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg )
/// \brief		MSG_FC_BATTLE_DROP_DUMMY_OK ÀÎ °æ¿ì Item »ý¼º
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CSceneData::AddFieldItemBattleDropDummyOk( CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg )
//{
//	FLOG("CSceneData::AddFieldItemBattleDropDummyOk( CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg )");
//	if(m_pItemData)
//	{
//		CItemData *pItem = FindFieldItemByFieldIndex( pMsg->ItemFieldIndex );
//		if(!pItem)
//		{
//			pItem = new CItemData(pUnitData, pMsg);
//			m_pItemData->AddChild(pItem);
//		}
//	}
//}

void CSceneData::AddFieldItemBattleDropFixerOk(CAtumNode *pTarget,CAtumNode *pAttack,MSG_FC_BATTLE_DROP_FIXER_OK* pMsg)
{
	FLOG("CSceneData::AddFieldItemBattleDropFixerOk(CAtumNode *pTarget,CAtumNode *pAttack,...");
	if(m_pItemData)
	{
		CItemData *pItemData = FindFieldItemByFieldIndex( pMsg->ItemFieldIndex );
		if(!pItemData)
		{
			pItemData = new CItemData( pTarget, (CAtumData*)pAttack,pMsg );
			m_pItemData->AddChild(pItemData);
			ITEM *pItem = g_pDatabase->GetServerItemInfo(pItemData->m_nItemNum);
			if(pMsg->TargetIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
			{// ³»°¡ ÇÈ¼­ÀÇ Å¸ÄÏÀÌ¸é..
				// 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter1,pItem->ParameterValue1);
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter2,pItem->ParameterValue2);
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter3,pItem->ParameterValue3);
// 				g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter4,pItem->ParameterValue4);
				int nArrParamCnt = 0;
				for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
				{
					g_pShuttleChild->SetParamFactorDesParam(g_pShuttleChild->m_paramFactor,
															pItem->ArrDestParameter[nArrParamCnt],
															pItem->ArrParameterValue[nArrParamCnt]);

				}				
				// end 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
			}
		}
	}
}

void CSceneData::DeleteFieldItemBattleDropFixerOk(UINT nItemFieldIndex)
{
  	if(m_pItemData)
	{
		CItemData *pItemData = FindFieldItemByFieldIndex( nItemFieldIndex );
		if(pItemData)
		{
			ITEM *pItem = g_pDatabase->GetServerItemInfo(pItemData->m_nItemNum);	
			// 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
// 			g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter1,pItem->ParameterValue1);
// 			g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter2,pItem->ParameterValue2);
// 			g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter3,pItem->ParameterValue3);
// 			g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,pItem->DestParameter4,pItem->ParameterValue4);
			int nArrParamCnt = 0;
			for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
			{
				g_pShuttleChild->ReleaseParamFactorDesParam(g_pShuttleChild->m_paramFactor,
										pItem->ArrDestParameter[nArrParamCnt],
										pItem->ArrParameterValue[nArrParamCnt]);

			}			
			// end 2009-04-21 by bhsohn ¾ÆÀÌÅÛ DesParamÃß°¡
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData * CSceneData::FindFieldItemByFieldIndex( UINT nFieldIndex )
/// \brief		FieldIndex·Î °Ë»ö
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData * CSceneData::FindFieldItemByFieldIndex( UINT nFieldIndex )
{
	FLOG("CSceneData::FindFieldItemByFieldIndex( UINT nFieldIndex )");

	if (m_pItemData == nullptr) return nullptr;

	CItemData * pItem = (CItemData *)m_pItemData->m_pChild; // 2016-01-08 exception at this point, m_pItemData == nullptr
	while(pItem)
	{
		if(pItem->m_nItemIndex == nFieldIndex)
		{
			return pItem;
		}
		pItem = (CItemData *)pItem->m_pNext;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData* CSceneData::FindFieldItemBy2DDistance( D3DXVECTOR2 vPos, float fDist )
/// \brief		2D»ó °Å¸®·Î °Ë»ö(Å¬¸¯)
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData* CSceneData::FindFieldItemBy2DDistance( D3DXVECTOR2 vPos, float fDist )
{
	FLOG("CSceneData::FindFieldItemBy2DDistance( D3DXVECTOR2 vPos, float fDist )");
	CItemData * pItem = (CItemData *)m_pItemData->m_pChild;
	while(pItem)
	{
		D3DXVECTOR2 vItemPos = D3DXVECTOR2(pItem->m_nObjScreenX,pItem->m_nObjScreenY);
		float fTemp = D3DXVec2Length(&(vItemPos - vPos));
		if(fTemp < fDist)
		{
			return pItem;
		}
		pItem = (CItemData *)pItem->m_pNext;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::SendItemGetItemAll()
/// \brief		ÁÖº¯ ¾ÆÀÌÅÛ ½Àµæ ¸Þ½ÃÁö
/// \author		dhkwon
/// \date		2004-06-18 ~ 2004-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::SendItemGetItemAll()
{
	// 2006-07-18 by ispark, ½Ã°£ µô·¹ÀÌ. Å°¸¦ ´©¸¥ ÈÄ 1ÃÊµ¿¾ÈÀº Å° ÀÔ·Â Á¦ÇÑ
	if(m_fGetItemAllDelay < GET_ITEM_IN_TIME)
	{
		return;
	}

	m_fGetItemAllDelay = 0.0f;

	// ÁÖº¯ ¾ÆÀÌÅÛ ÃÑ Á¶»ç
	CItemData * pItem = (CItemData *)m_pItemData->m_pChild;
	vector<CItemData *> vecItemData;
	while(pItem)
	{
		vecItemData.push_back(pItem);
		pItem = (CItemData *)pItem->m_pNext;
	}
	BOOL bSendGetItem = FALSE;

	// ·»´ý 2°³ °áÁ¤
	for(int i=0;i<GET_ITEM_NUMBER_IN_TICK;i++)
	{
		if(0 >= vecItemData.size())
		{
			// 2008-09-02 by dgwoo ÁÖº¯¿¡ ¾ÆÀÌÅÛÀÌ ¾øÀ»°æ¿ì Ã³¸®.
			bSendGetItem = TRUE;
			break;
		}

		int nIndex = RANDI(0, vecItemData.size() - 1);
		pItem = vecItemData[nIndex];

		if(pItem->m_dwPartType == _ITEMFIELD && pItem->m_bIsRender)
		{
			// 2009. 11. 3 by jskim Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) ¾ÆÀÌÅÛ Ãß°¡ ±¸Çö
			//if(g_pStoreData->GetTotalUseInven() < CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1) - 1, pMainInfo->GetAddedPermanentInventoryCount())
			// 2009. 12. 17 by jskim Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) Àç¼öÁ¤
// 			CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
// 			if(g_pStoreData->GetTotalUseInven() < CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, pMainInfo->GetAddedPermanentInventoryCount()) - 1)
 			if(g_pStoreData->GetTotalUseInven() < CAtumSJ::GetMaxInventorySize((BOOL)g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1, g_pShuttleChild->m_myShuttleInfo.GetAddedPermanentInventoryCount()) - 1)

			//end 2009. 12. 17 by jskim Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) Àç¼öÁ¤
			//end 2009. 11. 3 by jskim Ä³½¬(ÀÎº¥/Ã¢°í È®Àå) ¾ÆÀÌÅÛ Ãß°¡ ±¸Çö
			{// ÀÎº¥Åä¸®¿¡ ¿©À¯°¡ ÀÖÀ»°æ¿ì´Â ¼­¹ö·Î ¹Ù·Î ¿äÃ».
				MSG_FC_ITEM_GET_ITEM sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
				sMsg.ItemFieldIndex = pItem->m_nItemIndex;
				g_pFieldWinSocket->SendMsg(T_FC_ITEM_GET_ITEM, (char*)&sMsg, sizeof(sMsg) );
				bSendGetItem = TRUE;
			}
			else if(IS_COUNTABLE_ITEM(pItem->m_byKind))
			{// Ä«¿îÅÍºí ¾ÆÀÌÅÛÀÏ°æ¿ì ÇöÀç ÀÎº¥¿¡ ÀÖ´ÂÁö Ã¼Å©ÈÄ ¿äÃ».
				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByItemNum(pItem->m_nItemNum);
				if(pItemInfo)
				{
					MSG_FC_ITEM_GET_ITEM sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
					sMsg.ItemFieldIndex = pItem->m_nItemIndex;
					g_pFieldWinSocket->SendMsg(T_FC_ITEM_GET_ITEM, (char*)&sMsg, sizeof(sMsg) );
					bSendGetItem = TRUE;
				}
			}
//			else
//			{
//				g_pD3dApp->m_pChat->CreateChatChild(STRERR_ERROR_0022,COLOR_ERROR);
//			}

		}

		vecItemData.erase(find(vecItemData.begin(), vecItemData.end(), pItem));
//		if(!pItem)
//			break;
//		if(pItem->m_dwPartType == _ITEMFIELD && pItem->m_bIsRender)
//		{
//			MSG_FC_ITEM_GET_ITEM sMsg;
//			memset(&sMsg,0x00,sizeof(sMsg));
//			sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//			sMsg.ItemFieldIndex = pItem->m_nItemIndex;
//			g_pFieldWinSocket->SendMsg(T_FC_ITEM_GET_ITEM, (char*)&sMsg, sizeof(sMsg) );
//		}
//		pItem = (CItemData *)pItem->m_pNext;
	}
	// 2008-10-14 by dgwoo 'q'¿¬Å¸½Ã ÀÎº¥ÀÌ ²ËÂ÷Áö ¾Ê¾Ò´Âµ¥ ¸Þ½ÃÁö°¡ ¶ß´Â ¹ö±×.
	//if(!bSendGetItem)
	if(!bSendGetItem && pItem && pItem->m_bIsRender)
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_ERROR_0022,COLOR_ERROR);

	vecItemData.clear();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::DeleteItemInFieldItem( int x, int y )
/// \brief		°ü¸®ÀÚ¿¡ ÀÇÇÑ ÇÊµå À§ÀÇ ¾ÆÀÌÅÛ »èÁ¦
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::SendItemDeleteItemAdmin( int x, int y )
{
	FLOG("SendItemDeleteItemAdmin( int x, int y )");
	CItemData * pItem = FindFieldItemBy2DDistance( D3DXVECTOR2(x,y), GET_INFO_DISTANCE_OF_2D );
	if(pItem)
	{
		MSG_FC_ITEM_DELETE_ITEM_ADMIN sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		sMsg.ItemFieldIndex = pItem->m_nItemIndex;
		sMsg.DropPosition = pItem->m_vPos;
		g_pD3dApp->m_pFieldWinSocket->SendMsg( T_FC_ITEM_DELETE_ITEM_ADMIN, (char*)&sMsg, sizeof(sMsg) );
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::DeleteFieldItemOfUnitData( CUnitData* pUnitData )
/// \brief		À¯´Öµé¿¡ ¿¬°áµÈ DUMMY, FIXER, WeaponÀ» »èÁ¦ÇÑ´Ù.
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::DeleteFieldItemOfUnitData( CUnitData* pUnitData )
{
	FLOG("CSceneData::DeleteFieldItemOfUnitData( CUnitData* pUnitData )");

	if (m_pItemData == nullptr) return;

	CItemData * pItem = (CItemData *)m_pItemData->m_pChild; // 2016-01-08 exception at this point, m_pItemData == nullptr
	while(pItem)
	{
		if(pItem->m_pParent == pUnitData)
		{
			pItem->m_bUsing = FALSE;
		}
		// 2006-01-17 by ispark
		else if(pItem->m_pTarget == pUnitData)
		{
			pItem->m_bUsing = FALSE;
		}
		pItem = (CItemData *)pItem->m_pNext;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::DeleteFieldItemOfFieldIndex( UINT nFieldIndex )
/// \brief		nFieldIndex¸¦ Áö¿î´Ù.
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	FieldSocketBattleAttackHideItem
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::DeleteFieldItemOfFieldIndex( UINT nFieldIndex )
{
	FLOG("CSceneData::DeleteFieldItemOfFieldIndex( UINT nFieldIndex )");
	CItemData * pItem = FindFieldItemByFieldIndex( nFieldIndex );
	if(pItem)
	{
		pItem->m_bUsing = FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::ExplodingFieldItemOfFieldIndex( UINT nFieldIndex )
/// \brief		nFieldIndexfmf ExplodingÀ¸·Î ¸¸µç´Ù.
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	FieldSocketBattleAttackExplodeItem
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::ExplodingFieldItemOfFieldIndex( UINT nFieldIndex )
{
	FLOG("CSceneData::ExplodingFieldItemOfFieldIndex( UINT nFieldIndex )");
	CItemData * pItem = FindFieldItemByFieldIndex( nFieldIndex );
	if(pItem)
	{
		pItem->m_dwState = _EXPLODING;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData* CSceneData::FindUnitDataByClientIndex( ClientIndex_t nIndex )
/// \brief		ClientIndex¸¦ ÀÌ¿ëÇØ Shuttle,Enemy,MonsterÁß¿¡¼­ UnitData¸¦ Ã£´Â´Ù.
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CUnitData* CSceneData::FindUnitDataByClientIndex( ClientIndex_t nIndex )
{
	FLOG("CSceneData::FindUnitDataByClientIndex( ClientIndex_t nIndex )");
	if( g_pD3dApp->m_pShuttleChild == NULL)
	{
		return NULL;
	}
	if(nIndex == g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex)
	{
		return g_pD3dApp->m_pShuttleChild;
	}
	else if( IS_CHARACTER_CLIENT_INDEX(nIndex) )//pMsg->AttackIndex < 10000)
	{
		CMapEnemyIterator itEnemy = m_mapEnemyList.find(nIndex);
		if(itEnemy != m_mapEnemyList.end())
		{
			return itEnemy->second;
		}
	}
	else if(IS_MONSTER_CLIENT_INDEX(nIndex))
	{
		CMapMonsterIterator itMonster = m_mapMonsterList.find(nIndex);
		if(itMonster != m_mapMonsterList.end())
		{
			return itMonster->second;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CSceneData::FindClientIndexByUnitData( CUnitData* pUnit )
/// \brief		UnitData¸¦ ÀÌ¿ëÇØ ClientIndex¸¦ Ã£´Â´Ù.
/// \author		dhkwon
/// \date		2004-07-22 ~ 2004-07-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CSceneData::FindClientIndexByUnitData( CUnitData* pUnit )
{
	if( pUnit->m_dwPartType == _SHUTTLE )
	{
		return g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	}
	else if( pUnit->m_dwPartType == _ENEMY || pUnit->m_dwPartType == _ADMIN )
	{
		return ((CEnemyData*)pUnit)->m_infoCharacter.CharacterInfo.ClientIndex;
	}
	else if( pUnit->m_dwPartType == _MONSTER )
	{
		return ((CMonsterData*)pUnit)->m_info.MonsterIndex;
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData * CSceneData::FindFieldItemByPartTypeAndParent( DWORD dwPartType, CAtumNode* pParent )
/// \brief		Field itemÁß¿¡ UnitData¿¡ ºÙÀº ItemÁß¿¡ dwPartyType(_DUMMY,_FIXER)°ú °°Àº °Í ÇÏ³ª¸¦ ¸®ÅÏÇÑ´Ù.
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData * CSceneData::FindFieldItemByPartTypeAndParent( DWORD dwPartType, CAtumNode* pParent )
{
	FLOG("CSceneData::FindFieldItemByPartTypeAndParent( DWORD dwPartType, CAtumNode* pParent )");
	CItemData * pItem = (CItemData *)m_pItemData->m_pChild;
	while(pItem)
	{
		if(pItem->m_dwPartType == dwPartType && pItem->m_pParent == pParent)
		{
			return pItem;
		}
		pItem = (CItemData *)pItem->m_pNext;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData * CSceneData::FindFieldItemByParent( CAtumNode* pParent )
/// \brief		
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData * CSceneData::FindFieldItemByParent( CAtumNode* pParent )
{
	FLOG("CSceneData::FindFieldItemByParent( CAtumNode* pParent )");
	CItemData * pItem = (CItemData *)m_pItemData->m_pChild;
	while(pItem)
	{
		if(pItem->m_pParent == pParent)
		{
			return pItem;
		}
		pItem = (CItemData *)pItem->m_pNext;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int	CSceneData::GetDummyCountOfUnit(CAtumData* pUnit)
/// \brief		À¯´ÖÀÇ ´õ¹Ì °¹¼ö¸¦ ÆÄ¾ÇÇÑ´Ù.
/// \author		dhkwon
/// \date		2004-05-31 ~ 2004-05-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CSceneData::GetDummyCountOfUnit(CAtumData* pUnit)
{
	int nCount = 0;
	CItemData * pItem = (CItemData *)m_pItemData->m_pChild;
	while(pItem)
	{
		if(pItem->m_pParent == pUnit && pItem->m_dwPartType == _DUMMY)
		{
			nCount++;
		}
		pItem = (CItemData *)pItem->m_pNext;
	}
	return nCount;
}

CAppEffectData* CSceneData::FindEffect(int nType)
{
	CAppEffectData* pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
	while(pEffect)
	{
		if(pEffect->m_nType == nType)
		{
			break;
		}
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return pEffect;

}

CAppEffectData* CSceneData::FindEffect(int nType, CUnitData *pParent)
{
	CAppEffectData* pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
	while(pEffect)
	{
		if(pEffect->m_nType == nType && pEffect->m_pParent == pParent)
		{
			break;
		}
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
	return pEffect;

}

void CSceneData::DeleteEffect(CAtumNode* pParent)
{
	CAppEffectData* pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->m_pChild;
	while(pEffect)
	{
		if(pEffect->m_pParent == pParent)
		{
			pEffect->m_bUsing = FALSE;
		}
		pEffect = (CAppEffectData*)pEffect->m_pNext;
	}
}

CCinema * CSceneData::LoadCinemaData(char* szFileName, int nFileName)
{
	// 2006-03-06 by ispark, µðÆúÆ® ÆÐÅÏ Ãß°¡
	/// »õ·Î¿î ½Ã³×¸¶ Å¬·¡½º »ý¼º
	CCinema* pCinemaData = new CCinema;

	if(nFileName != 0)
	{
		/// m_pCinemaData·Î ºÎÅÍ szFileName¿¡ ´ëÇÑ µ¥ÀÌÅÍ¸¦ °¡Á®¿È
		DataHeader* pDataHeader = m_pCinemaData->Find(szFileName);
		if(pDataHeader == NULL)
		{
			util::del(pCinemaData);
			return NULL;
		}
		/// »õ·Î ¸¸µç ½Ã³×¸¶ Å¬·¡½º¿¡ DataHeader¿¡ ´ã±ä µ¥ÀÌÅÍ·Î ÆÐÅÏ »ý¼º
		pCinemaData->InitData(pDataHeader->m_pData);
	}
	else
	{
		pCinemaData->InitDefaultData();		
	}
	/// ÆÐÅÏÀÌ Á¤ÀÇµÈ ½Ã³×¸¶ Å¬·¡½º ¸®ÅÏ
	return pCinemaData;
}

void CSceneData::LoadCinemaFile()
{
	util::del( m_pCinemaData );
	m_pCinemaData = new CGameData;
	char strPath[256];
	wsprintf(strPath,".\\Res-Tex\\cinema.tex");
	if(strlen(strPath)>0)
	{
		m_pCinemaData->SetFile(strPath,FALSE,NULL,0);
	}
}

void CSceneData::DeleteCinemaFile()
{
	// ½Ã³×¸¶ µ¥ÀÌÅÍ ÀüºÎ¸¦ Áö¿î´Ù.
	util::del( m_pCinemaData );
}

//void CSceneData::SetExceptAllUnitTarget()
//{
//	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
//	while(itEnemy != m_mapEnemyList.end())
//	{
//		itEnemy->second->m_nEnemyTypeSecondary = ENEMYDATA_ENEMYLIST;
//		itEnemy++;
//	}
//	CMapMonsterIterator itMonster = m_mapMonsterList.begin();
//	while(itMonster != m_mapMonsterList.end())
//	{
//		itMonster->second->m_nMonsterTypeSecondary = MONSTERDATA_MONSTERLIST;
//		itMonster++;
//	}
//
//}

void CSceneData::SetPKSettingGuildWar( int nPeerGuildUniqueNumber , BOOL bPK)
{
	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
		if(itEnemy->second->m_infoCharacter.CharacterInfo.GuildUniqueNumber == nPeerGuildUniqueNumber)
		{
//			itEnemy->second->m_bAttackEnemy = bPK;
			itEnemy->second->SetPkState( PK_GVG, bPK );
		}
		itEnemy++;
	}

}

CEnemyData * CSceneData::GetEnemyCharaterID(D3DXVECTOR2 vPos1)
{
	float fLength = 50.0f;
	map<INT,CEnemyData *>::iterator itEnemy = m_mapEnemyList.begin();
	CEnemyData * pEnemy = NULL;
	while(itEnemy != m_mapEnemyList.end())
	{
		if( itEnemy->second->m_nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pEnemy = itEnemy->second;
			}
		}
		itEnemy++;
	}

	return pEnemy;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CMonsterData * CSceneData::GetMonsterInfo2D(D3DXVECTOR2 vPos1)
/// \brief		ÁÖÀ§¿¡ ÀÖ´Â ¸ó½ºÅÍÀÇ Á¤º¸Áß vPos1¿¡ °ª¿¡ ÇØ´çÇÏ´Â ¸ó½ºÅÍ Á¤º¸¸¦ ¹ÝÈ¯.
/// \author		dgwoo
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CMonsterData * CSceneData::GetMonsterInfo2D(D3DXVECTOR2 vPos1)
{
	float fLength = 50.0f;
	map<INT,CMonsterData *>::iterator itMonster = m_mapMonsterList.begin();
	CMonsterData * pMonster = NULL;
	while(itMonster != m_mapMonsterList.end())
	{// °Ë»ö½Ã Å¬¸¯À» ÀÎ½ÄÇÏ´Â ¸ó½ºÅÍÀÇ °æ¿ì¸¸ ¸®ÅÏ.
		if( itMonster->second->m_nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(itMonster->second->m_nObjScreenX,itMonster->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pMonster = itMonster->second;
			}
		}
		itMonster++;
	}

	return pMonster;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CEnemyData * CSceneData::GetPickEnemy(D3DXVECTOR2 vPos)
/// \brief		Ä³¸¯ÅÍ¿ë Picking
/// \author		ispark
/// \date		2006-08-01 ~ 2006-08-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CEnemyData * CSceneData::GetPickEnemy(D3DXVECTOR2 vPos)
{
	float fLength = 70.0f;
	CEnemyData * pEnemyTemp = NULL;
	CEnemyData * pEnemy = NULL;
	CVecEnemyList::iterator itEnemy = m_vecEnemyRenderList.begin();
	int x = 0, y = 0, w = 0;
	while(itEnemy != m_vecEnemyRenderList.end())
	{
		pEnemy = (*itEnemy);

		D3DXVECTOR3 vPosTemp = pEnemy->m_vPos;
		int nPilotNum = pEnemy->GetPilotNum();
		float fChaLeng = (GetCharacterHeight(nPilotNum) + 1.0f) / 2.0f;
		vPosTemp.y += fChaLeng;
		g_pD3dApp->CalcObjectSourceScreenCoords(vPosTemp, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
			x, y, w );

		if( w > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(x, y);
			float fLengthTemp = D3DXVec2Length(&(vPos-vPos2));
			if(fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pEnemyTemp = pEnemy;
			}
		}

		itEnemy++;
	}

	return pEnemyTemp;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CObjectChild * CSceneData::FindWarpGateByPosition( D3DXVECTOR3 vPos )
/// \brief		°¡Àå °¡±î¿î Big object¸¦ ¸®ÅÏÇÑ´Ù. 
/// \author		dhkwon
/// \date		2004-11-08 ~ 2004-11-08
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::FindWarpGateByPosition( D3DXVECTOR3 vPos , float fCloseDistance )
/// \brief		fCloseDistance º¸´Ù ÀÛÀº °¡Àå °¡±î¿î ¿öÇÁ°ÔÀÌÆ®¸¦ ¸®ÅÏÇÑ´Ù.
/// \author		jschoi
/// \date		2005-03-08 ~ 2005-03-08
/// \warning	
///
/// \param		µÎ¹øÂ° ÀÎÀÚ µðÆúÆ® °ª : RANGE_OF_VISION
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild * CSceneData::FindWarpGateByPosition( D3DXVECTOR3 vPos , float fCloseDistance )
{
	CObjectChild* pWarpGate = NULL;
	CObjectChild* pObj = (CObjectChild*)g_pGround->m_pBigObject->m_pChild;
	while( pObj != NULL )
	{
		if( pObj->m_pObjectInfo &&
			(pObj->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM || pObj->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM_2 || // 2008-06-24 by dgwoo ¿öÇÁ °ÔÀÌÆ® Ãß°¡.
			pObj->m_pObjectInfo->Code == WARP_GATE_OBJECT_NUM_3))  // 2012-06-13 by jhahn ¿öÇÁ °ÔÀÌÆ® Ãß°¡2.
		{
			float fDistance = D3DXVec3Length(&(pObj->m_vPos - vPos ) );
			if( fDistance < fCloseDistance )
			{
				fCloseDistance = fDistance;
				pWarpGate = pObj;
			}
		}
		pObj = (CObjectChild*)pObj->m_pNext;
	}
	return pWarpGate;
}

void CSceneData::ChangeGammaOption( int nGamma )
{
	// ¿É¼Ç ¼³Á¤(°¨¸¶)
	// 2004-11-15 by ydkim
	
	float fGammaPlus = nGamma * OPTION_GAMMA_PLUS;
	
	//m_light0.Diffuse.r += fGammaPlus;
	//m_light0.Diffuse.g += fGammaPlus;
	//m_light0.Diffuse.b += fGammaPlus;

	m_light0.Ambient.r += fGammaPlus;
	m_light0.Ambient.g += fGammaPlus;
	m_light0.Ambient.b += fGammaPlus;

	//if((m_light0.Diffuse.r) > 1.0f) m_light0.Diffuse.r = 1.0f;
	//if((m_light0.Diffuse.g) > 1.0f) m_light0.Diffuse.g = 1.0f;
	//if((m_light0.Diffuse.b) > 1.0f) m_light0.Diffuse.b = 1.0f;
	
	if((m_light0.Ambient.r) > 1.0f) m_light0.Ambient.r = 1.0f;
	if((m_light0.Ambient.g) > 1.0f) m_light0.Ambient.g = 1.0f;
	if((m_light0.Ambient.b) > 1.0f) m_light0.Ambient.b = 1.0f;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::FindEventObjectByIndex(UINT nEventObjectIndex)
/// \brief		ÀÌº¥Æ® ¿ÀºêÁ§Æ® ÀÎµ¦½º·Î °¡Àå °¡±î¿î ÇØ´ç ÀÌº¥Æ® ¿ÀºêÁ§Æ®¸¦ Ã£´Â´Ù.
/// \author		jschoi
/// \date		2004-11-26 ~ 2004-11-26
/// \warning	°°Àº ¿ÀºêÁ§Æ® ÀÎµ¦½º°¡ ÀÖ´Â°æ¿ì °¡Àå °¡±î¿î ÀÌº¥Æ® ¿ÀºêÁ§Æ®¸¦ ¸®ÅÏÇÑ´Ù.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild* CSceneData::FindEventObjectByIndex(D3DXVECTOR3 vPos, UINT nEventObjectIndex)
{
	float fDistance = 10000.0f;
	CObjectChild * pResultObjEvent = NULL;
	CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while(pObjEvent)
	{
		if(pObjEvent->m_sEventIndexFrom == nEventObjectIndex)
		{
			float fTempDistance = D3DXVec3Length(&(pObjEvent->m_vPos - vPos));
			if(fDistance > fTempDistance)
			{
				fDistance = fTempDistance;
				pResultObjEvent = pObjEvent;
			}
		}
		pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
	}
	return pResultObjEvent;
}

CObjectChild* CSceneData::FindEventObjectByWarp()
{
	CObjectChild * pResultObjEvent = NULL;
	CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while(pObjEvent)
	{
		if(EVENT_TYPE_WARP == pObjEvent->m_bEventType)
		{
			pResultObjEvent = pObjEvent;
		}
		pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
	}
	return pResultObjEvent;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::FindEventObjectByIndex(UINT nEventObjectIndex)
/// \brief		ÀÎµ¦½º·Î ¿ÀºêÁ§Æ®¸¦ Ã£´Â´Ù
/// \author		ydkim
/// \date		2005-07-20
/// \warning
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild * CSceneData::FindObjectByIndex(UINT nEventObjectIndex)
{
	CObjectChild * pResultObjEvent = NULL;
	CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while(pObjEvent)
	{
		if(pObjEvent->m_sEventIndexFrom == nEventObjectIndex)
		{
			pResultObjEvent = pObjEvent;
		}
		pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
	}
	return pResultObjEvent;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::FindRandomObjectByEventType(BYTE bObjectType, BOOL bRand)
/// \brief		ÀÌº¥Æ® Å¸ÀÔÀ¸·Î Ã£±â, ¶ÇÇÑ °°Àº Å¸ÀÔÀÌ ¿©·¯°Ô ÀÖÀ» °æ¿ì ·£´ýÀ¸·Î »Ì¾ÆÁØ´Ù.
/// \author		ispark
/// \date		2006-05-19 ~ 2006-05-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild * CSceneData::FindRandomObjectByEventType(BYTE bObjectType, BOOL bRand)
{
	vector<CObjectChild*> vtObjectInfoPtr;
	CObjectChild * pResultObjEvent = NULL;
	CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while(pObjEvent)
	{
		if(pObjEvent->m_bEventType == bObjectType)
		{
			if(bRand)
			{
				vtObjectInfoPtr.push_back(pObjEvent);
			}
			else
			{
				return pObjEvent;
			}
		}
		pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
	}

	if(vtObjectInfoPtr.empty())
	{
		return NULL;
	}

	///////////////////////////////////////////////////////////////////////////////
	// °°Àº EventIndex°¡ ¿©·¯°³ ÀÏ¶§ ·£´ýÀ¸·Î ¼³Á¤
	int i = RANDI(0, vtObjectInfoPtr.size() - 1);
	CObjectChild * pTempObjEvent = vtObjectInfoPtr[i];
	vtObjectInfoPtr.clear();
	return pTempObjEvent;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::FindEventObjectByTypeAndPosition(BYTE bObjectType, D3DXVECTOR3 vPos)
/// \brief		ÇØ´ç À§Ä¡¿¡ ÇØ´çÇÏ´Â ÀÌº¥Æ® ¿ÀºêÁ§Æ®¸¦ Ã£´Â´Ù.
/// \author		jschoi
/// \date		2004-11-26 ~ 2004-11-26
/// \warning	¿ÀºêÁ§Æ® Å¸ÀÔÀÌ ÀÏÄ¡ÇÏ°í °Å¸®°¡ 10 ÀÌ³»ÀÌ¾î¾ß Ã£Àº°É·Î ÇÑ´Ù.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild * CSceneData::FindEventObjectByTypeAndPosition(BYTE bObjectType, D3DXVECTOR3 vPos, float fDist)
{
	CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while(pObjEvent)
	{
		float dist = D3DXVec3Length(&(pObjEvent->m_vPos - vPos));
		if(pObjEvent->m_bEventType == bObjectType && dist < fDist)
		{
			return pObjEvent;
		}
		else
		{
			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}
	return NULL;
}

// 2010. 05. 27 by jskim ½Ã³×¸¶ Àû¿ë Ä«¸Þ¶ó ±¸Çö
CObjectChild * CSceneData::FindEventObjectByTypeAndPositionIndex(BYTE bObjectType, short sEventIndex)
{
	CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while(pObjEvent)
	{
		if(pObjEvent->m_bEventType == bObjectType && pObjEvent->m_sEventIndexTo == sEventIndex)
		{
			return pObjEvent;
		}
		else
		{
			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::FindMapObjectByCodeAndPosition(int nCode, D3DXVECTOR3 vPos)
/// \brief		ÇØ´çÀ§Ä¡¿¡ ÇØ´ç ÄÚµåÀÇ ¸Ê ¿ÀºêÁ§Æ®¸¦ Ã£´Â´Ù.
/// \author		jschoi
/// \date		2004-11-27 ~ 2004-11-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CObjectChild * CSceneData::FindMapObjectByCodeAndPosition(int nCode, D3DXVECTOR3 vPos)
{
	//28.11.2015 by St0rmy, Fixed Warp/MapLoad DC/Crash
	if (!g_pGround)
		return NULL;
	int x = (int)((g_pGround->m_projectInfo.sXSize * TILE_SIZE) / MAP_BLOCK_SIZE);
	int z = (int)((g_pGround->m_projectInfo.sYSize * TILE_SIZE) / MAP_BLOCK_SIZE);
	CObjectChild* pMapObj = NULL;
	for( int i=0; i<x; i++ )
	{
		for( int j=0; j<z; j++ )
		{
			pMapObj = (CObjectChild *)g_pGround->m_ppObjectList[i][j].m_pChild;
			while(pMapObj)
			{
				if(pMapObj->m_nCode == nCode && D3DXVec3Length(&(pMapObj->m_vPos - vPos)) < 10.0f)
				{
					return pMapObj;
				}
				else
				{
					pMapObj = (CObjectChild *)pMapObj->m_pNext;
				}
			}
		}
	}

	pMapObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_nCode == nCode && D3DXVec3Length(&(pMapObj->m_vPos - vPos)) < 10.0f )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	

	pMapObj = (CObjectChild *)m_pGround->m_pWaterObject->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_nCode == nCode && D3DXVec3Length(&(pMapObj->m_vPos - vPos)) < 10.0f )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	

	pMapObj = (CObjectChild *)m_pGround->m_pObjectMonster->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_nCode == nCode && D3DXVec3Length(&(pMapObj->m_vPos - vPos)) < 10.0f )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	


	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ChangeAllEnemyCityWarTeamType(BYTE byCityWarTeamType)
/// \brief		¸ðµç EnemyÀÇ µµ½ÃÁ¡·ÉÀü ÆÀÀ» ¼ÂÆÃÇÑ´Ù.(¾ÆÀÌµð »öÀ¸·Î ÇÇ¾Æ½Äº°À» À§ÇØ)
/// \author		jschoi
/// \date		2005-05-16 ~ 2005-05-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::ChangeAllEnemyCityWarTeamType(BYTE byCityWarTeamType)
{
	map<INT,CEnemyData *>::iterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
		itEnemy->second->m_infoCharacter.CharacterInfo.CityWarTeamType = byCityWarTeamType;
		itEnemy++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::SetFogLevel(DWORD dwFogColor, float fFogStart, float fFogEnd)
/// \brief		¾È°³¸¦ ¼ÂÆÃÇÑ´Ù.
/// \author		ispark
/// \date		2005-06-29 ~ 2005-06-29
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CSceneData::SetFogLevel(DWORD dwFogColor, float fFogStart, float fFogEnd)
{ 
	m_dwFogColor = dwFogColor; 
	m_fOrgFogStartValue = fFogStart; 
	m_fOrgFogEndValue = fFogEnd; 
	DBGOUT("m_dwFogColor = %d \n m_fOrgFogStartValue = %f \n m_fOrgFogEndValue = %f \n",dwFogColor,fFogStart,fFogEnd);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::SetMaxAtitudeHeight(int i_nMaxHeight)
/// \brief		¸Ê»ó ÀÌµ¿ °¡´ÉÇÑ ÃÖ´ë ³ôÀÌ ¼ÂÆÃ
/// \author		ispark
/// \date		2005-07-11 ~ 2005-07-11
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
void CSceneData::SetMaxAtitudeHeight(int i_nMaxHeight)
{
	m_nMaxAtitudeHeight = i_nMaxHeight;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::RenderWarter()
/// \brief		¸Ê»óÀÇ ¹°ÀÇ ·»´õ¸µ
/// \author		ydkim
/// \date		2005-07-26 by ydkim
/// \warning	
///
/// \param		
/// \return		void
///////////////////////////////////////////////////////////////////////////////
#if 1 // 2015-10-19 PanKJ Water Workaround
void CSceneData::RenderWater()
{
	// 2005-02-11 by jschoi  ¹° ¿ÀºêÁ§Æ® ·»´õ¸µ
	CObjectChild* pWaterObj = (CObjectChild*)g_pGround->m_pWaterObject->m_pChild;

	while (pWaterObj)
	{
		float fRadius = 0;

		if (pWaterObj->m_pObjMesh) fRadius = pWaterObj->m_pObjMesh->m_fRadius;

		BOOL bCheckIn = g_pFrustum->CheckSphere(pWaterObj->m_vOriPos.x, pWaterObj->m_vOriPos.y, pWaterObj->m_vOriPos.z, fRadius);
		
		if (bCheckIn) pWaterObj->Render();

		pWaterObj = (CObjectChild*)pWaterObj->m_pNext;
	}

	// Water
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, 1.0f, 1.0f, 1.0f);
	g_pD3dDev->SetMaterial(&mtrl);
	// 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®
	//if(g_pSOption->sLowQuality)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex));
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	//g_pD3dDev->SetFVF( D3DFVF_GROUNDVERTEX );
	g_pD3dDev->SetFVF(D3DFVF_WATERBUMPVERTEX);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

	g_pD3dDev->SetTransform(D3DTS_WORLD, &mat);
	D3DXVECTOR3 vView;// = m_pCamera->GetEyePt();
	D3DXVECTOR3 vDir = g_pD3dApp->m_pCamera->GetViewDir();
	D3DXVec3Normalize(&vDir, &vDir);
	int cont1, cont2;
	int tempx, tempz;
	vView = g_pD3dApp->m_pCamera->GetEyePt();

	/// 2004.06.07 jschoi
	/// Water Region
	int nWaterCont = (int)(m_fFogEndValue / (TILE_SIZE * 2));
	vView += 2 * TILE_SIZE*nWaterCont*vDir;
	tempx = (int)((vView.x / TILE_SIZE) / 2);
	tempz = (int)((vView.z / TILE_SIZE) / 2);
	cont1 = 0;
	cont2 = 0;

	/// 2004.06.07 jschoi
	/// ¹° ÀÌ¹ÌÁö¿¡ ÀýµÎÃ¼ Àû¿ë
	g_pFrustum->Construct(g_pD3dDev);
	int nWaterHeight = m_pGround->m_projectInfo.fWaterHeight;
	int nWaterRadius = TILE_SIZE * 4;
	// 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®
	//if(g_pSOption->sLowQuality)
	{
		g_pD3dDev->LightEnable(2, FALSE);
		g_pD3dDev->LightEnable(3, FALSE);
	}
	// end 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	g_pD3dDev->SetTexture(0, m_pWaterBumpTexture);			// ¹üÇÁ ÅØ½ºÃÄ 0¹ø
	g_pD3dDev->SetTexture(1, m_pGround->m_pWaterTexture[0]);	// 40°³ ÅØ½ºÃÄÁß Ã¹¹øÂ° ¹° ÅØ½ºÃÄ 1¹ø

	// ¹üÇÁ¸Ê ÅØ½ºÃÄ ½ºÅ×ÀÌÁö ¼³Á¤
	g_pD3dDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT00, F2DW(1.0f / 16.0f));
	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT11, F2DW(1.0f / 16.0f));

	// ·¦ÇÎ ¸ðµå WRAPÀ¸·Î º¯°æ
	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ¹° ÅØ½ºÃÄ ½ºÅ×ÀÌÁö ¼³Á¤
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ¹° ÅØ½ºÃÄ ÅØ½ºÃÄ ¸ÅÆ®¸¯½º ¼³Á¤
	D3DXMATRIXA16 matTex;
	D3DXMatrixIdentity(&matTex);
	matTex._31 = timeGetTime() % 20000 / 20000.0f;
	g_pD3dDev->SetTransform(D3DTS_TEXTURE1, &matTex);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	// ¹üÇÁ ¸ÊÇÎ°ú ÅØ½ºÃÄ ¸ÅÆ®¸¯½º°¡ µ¿½Ã¿¡ ¾È¸Ô¾î¼­
	// ¹öÅØ½º ¹öÆÛ¿¡ Á÷Á¢ ÅØ½ºÃÄ ÁÂÇ¥¸¦ ¼öÁ¤ÇÑ´Ù
	float tu2;
	tu2 = timeGetTime() % 1000 / 1000.0f;

	int		nRenderWaterTileCount = 0;	// ±×·ÁÁÙ ¹° Å¸ÀÏ °¹¼ö
	// ÃÖÁ¾ÀûÀ¸·Î ±×·ÁÁÙ ¹öÅØ½º ¹öÆÛ Æ÷ÀÎÅÍ
	WATERBUMPVERTEX* v;						//	GROUNDVERTEX -> WATERBUMPVERTEX

	// ÃÖÁ¾Àû ±×¸± ¹° ¹öÅØ½º ¹öÆÛ ¶ô
	m_pGround->m_pToRenderWaterVB->Lock(0, 0, (void**)&v, 0); // 2016-01-08 exception at this point, probably m_pGround == nullptr (3)
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ

	auto fWaterHeight = m_pGround->m_projectInfo.fWaterHeight;

	for (int i = tempx - nWaterCont; i < tempx + nWaterCont + 1; i++)
	{
		if (cont1 < nWaterCont / 2)
			cont2 = cont1;
		else if (cont1 < nWaterCont * 2 + 1 - nWaterCont / 2)
			cont2 = nWaterCont / 2;
		else
			cont2 = nWaterCont * 2 - cont1;

		if (i >= 0 && i < m_pGround->m_projectInfo.sXSize / 2)
		{
			for (int j = tempz - (nWaterCont / 2 + cont2) - 1; j < tempz + (nWaterCont / 2 + cont2) + 2; j++)
			{
				if (j >= 0 && j < m_pGround->m_projectInfo.sYSize / 2)
				{
					int k = (i*m_pGround->m_projectInfo.sYSize / 2 + j);
					if (m_pGround->m_bWaterRender[k].useWater)
					{
						/// 2004.06.07 jschoi ÀýµÎÃ¼ Àû¿ë
						if (g_pFrustum->CheckSphere(i * TILE_SIZE * 2 + TILE_SIZE, nWaterHeight, j * TILE_SIZE * 2 + TILE_SIZE, nWaterRadius))
						{
							int nWaterTexNumber = m_pGround->m_bWaterRender[k].waterTexNumber + m_pGround->m_bWaterTexCont;

							if (nWaterTexNumber >= WATER_TEXTURE_COUNT) nWaterTexNumber -= WATER_TEXTURE_COUNT;

							// 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®
							//							if(g_pSOption->sLowQuality)
							//							{
							//								nWaterTexNumber = 0;
							//							}
							// end 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®

							// ÃÖÁ¾ÀûÀ¸·Î ±×¸± ¹öÅØ½º ¹öÆÛ¿¡ °ª ´ëÀÔ
							v[0].p = D3DXVECTOR3(i * TILE_SIZE * 2, fWaterHeight, j * TILE_SIZE * 2);
							v[0].n = D3DXVECTOR3(0, 1, 0);
							v[0].tu = 0.0f;
							v[0].tv = 0.0f;
							v[0].tu2 = tu2;			// ÅØ½ºÃÄ ÁÂÇ¥µµ º¯°æ
							v[0].tv2 = 0.0f;

							v[1].p = D3DXVECTOR3(i * TILE_SIZE * 2, fWaterHeight, (j + 1) * TILE_SIZE * 2);
							v[1].n = D3DXVECTOR3(0, 1, 0);
							v[1].tu = 0.0f;
							v[1].tv = 1.0f;
							v[1].tu2 = tu2;
							v[1].tv2 = 1.0f;

							v[2].p = D3DXVECTOR3((i + 1) * TILE_SIZE * 2, fWaterHeight, j * TILE_SIZE * 2);
							v[2].n = D3DXVECTOR3(0, 1, 0);
							v[2].tu = 1.0f;
							v[2].tv = 0.0f;
							v[2].tu2 = tu2 + 1.0f;
							v[2].tv2 = 0.0f;

							v[3].p = D3DXVECTOR3((i + 1) * TILE_SIZE * 2, fWaterHeight, (j + 1) * TILE_SIZE * 2);
							v[3].n = D3DXVECTOR3(0, 1, 0);
							v[3].tu = 1.0f;
							v[3].tv = 1.0f;
							v[3].tu2 = tu2 + 1.0f;
							v[3].tv2 = 1.0f;

							// ÃÖÁ¤ÀûÀ¸·Î ±×¸± ¹öÅØ½º ¹öÆÛ Æ÷ÀÎÅÍ Áõ°¡
							v += 4;
							// ±×¸®´Â ¹° Å¸ÀÏ °¹¼ö Áõ°¡
							++nRenderWaterTileCount;
							// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ
						}
					}
				}
			}
		}
		cont1++;
	}


	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	// ÃÖÁ¾ÀûÀ¸·Î ±×¸± ¹° ¹öÅØ½º ¹öÆÛ ¾ð¶ô
	m_pGround->m_pToRenderWaterVB->Unlock();

	// Å¸ÀÏ °¹¼ö * 4¸¸Å­ ·£´õ¸µ
	g_pD3dDev->SetStreamSource(0, m_pGround->m_pToRenderWaterVB, 0, sizeof(WATERBUMPVERTEX));

	// 2009. 02. 11 by ckPark ¹° ·»´õ¸µ Å¸ÀÏº°·Î ·»´õ¸µ
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, nRenderWaterTileCount * 4);
	for (int i = 0; i < nRenderWaterTileCount; ++i) g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	// end 2009. 02. 11 by ckPark ¹° ·»´õ¸µ Å¸ÀÏº°·Î ·»´õ¸µ

	// ÅØ½ºÃÄ ÇØÁ¦
	g_pD3dDev->SetTexture(0, 0);
	g_pD3dDev->SetTexture(1, 0);

	// ÅØ½ºÃÄ ¸ÅÆ®¸¯½º ÇØÁ¦
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	// 1¹ø ¸ÖÆ¼ ÅØ½ºÃÄ ÇØÁ¦	
	// 2009-01-15 by bhsohn ATIºñµð¿À Ä«µå¿¡¼­ ÀÌ ¿É¼ÇÀ» ¾È¾²¸é ±úÁø´Ù.
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// ÅØ½ºÃÄ ÁÂÇ¥ ¿ø·¡´ë·Î º¯°æ
	g_pD3dDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	// 	g_pD3dDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,   D3DTADDRESS_WRAP );
	//  g_pD3dDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,   D3DTADDRESS_WRAP );

	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ
}
#else
void CSceneData::RenderWater()
{
	// 2005-02-11 by jschoi  ¹° ¿ÀºêÁ§Æ® ·»´õ¸µ
	CObjectChild* pWaterObj = (CObjectChild*)g_pGround->m_pWaterObject->m_pChild;
	while (pWaterObj)
	{
		float fRadius = 0;
		if (pWaterObj->m_pObjMesh)
		{
			fRadius = pWaterObj->m_pObjMesh->m_fRadius;
		}
		BOOL bCheckIn = g_pFrustum->CheckSphere(pWaterObj->m_vOriPos.x, pWaterObj->m_vOriPos.y, pWaterObj->m_vOriPos.z, fRadius);
		if (bCheckIn)
		{
			pWaterObj->Render();
		}
		pWaterObj = (CObjectChild*)pWaterObj->m_pNext;
	}

	// Water
	D3DMATERIAL9 mtrl;
	D3DUtil_InitMaterial(mtrl, 1, 1, 1);
	g_pD3dDev->SetMaterial(&mtrl);
	// 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®
	//if(g_pSOption->sLowQuality)
	{
		g_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		g_pD3dDev->SetRenderState(D3DRS_FOGENABLE, IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex));
		g_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);


	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	//g_pD3dDev->SetFVF( D3DFVF_GROUNDVERTEX );
	g_pD3dDev->SetFVF(D3DFVF_WATERBUMPVERTEX);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ



	g_pD3dDev->SetTransform(D3DTS_WORLD, &mat);
	D3DXVECTOR3 vView;// = m_pCamera->GetEyePt();
	D3DXVECTOR3 vDir = g_pD3dApp->m_pCamera->GetViewDir();
	D3DXVec3Normalize(&vDir, &vDir);
	int i, j, cont1, cont2;
	int tempx, tempz;
	vView = g_pD3dApp->m_pCamera->GetEyePt();

	/// 2004.06.07 jschoi
	/// Water Region
	int nWaterCont = (int)(m_fFogEndValue / (TILE_SIZE * 2));
	vView += 2 * TILE_SIZE*nWaterCont*vDir;
	tempx = (int)((vView.x / TILE_SIZE) / 2);
	tempz = (int)((vView.z / TILE_SIZE) / 2);
	cont1 = 0;
	cont2 = 0;

	/// 2004.06.07 jschoi
	/// ¹° ÀÌ¹ÌÁö¿¡ ÀýµÎÃ¼ Àû¿ë
	g_pFrustum->Construct(g_pD3dDev);
	int nWaterHeight = m_pGround->m_projectInfo.fWaterHeight;
	int nWaterRadius = TILE_SIZE * 4;
	// 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®
	//if(g_pSOption->sLowQuality)
	{
		g_pD3dDev->LightEnable(2, FALSE);
		g_pD3dDev->LightEnable(3, FALSE);
	}
	// end 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®

	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	g_pD3dDev->SetTexture(0, m_pWaterBumpTexture);			// ¹üÇÁ ÅØ½ºÃÄ 0¹ø
	g_pD3dDev->SetTexture(1, m_pGround->m_pWaterTexture[0]);	// 40°³ ÅØ½ºÃÄÁß Ã¹¹øÂ° ¹° ÅØ½ºÃÄ 1¹ø

	// ¹üÇÁ¸Ê ÅØ½ºÃÄ ½ºÅ×ÀÌÁö ¼³Á¤
	g_pD3dDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	g_pD3dDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT00, F2DW(1.0f / 16.0f));
	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
	g_pD3dDev->SetTextureStageState(0, D3DTSS_BUMPENVMAT11, F2DW(1.0f / 16.0f));

	// ·¦ÇÎ ¸ðµå WRAPÀ¸·Î º¯°æ
	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ¹° ÅØ½ºÃÄ ½ºÅ×ÀÌÁö ¼³Á¤
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	g_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// ¹° ÅØ½ºÃÄ ÅØ½ºÃÄ ¸ÅÆ®¸¯½º ¼³Á¤
	D3DXMATRIXA16 matTex;
	D3DXMatrixIdentity(&matTex);
	matTex._31 = timeGetTime() % 20000 / 20000.0f;
	g_pD3dDev->SetTransform(D3DTS_TEXTURE1, &matTex);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	// ¹üÇÁ ¸ÊÇÎ°ú ÅØ½ºÃÄ ¸ÅÆ®¸¯½º°¡ µ¿½Ã¿¡ ¾È¸Ô¾î¼­
	// ¹öÅØ½º ¹öÆÛ¿¡ Á÷Á¢ ÅØ½ºÃÄ ÁÂÇ¥¸¦ ¼öÁ¤ÇÑ´Ù
	float tu2;
	tu2 = timeGetTime() % 1000 / 1000.0f;

	int		nRenderWaterTileCount = 0;	// ±×·ÁÁÙ ¹° Å¸ÀÏ °¹¼ö
	// ÃÖÁ¾ÀûÀ¸·Î ±×·ÁÁÙ ¹öÅØ½º ¹öÆÛ Æ÷ÀÎÅÍ
	WATERBUMPVERTEX* v;						//	GROUNDVERTEX -> WATERBUMPVERTEX

	// ÃÖÁ¾Àû ±×¸± ¹° ¹öÅØ½º ¹öÆÛ ¶ô
	m_pGround->m_pToRenderWaterVB->Lock(0, 0, (void**)&v, 0);
	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ



	for (i = tempx - nWaterCont; i < tempx + nWaterCont + 1; i++)
	{
		if (cont1 < nWaterCont / 2)
			cont2 = cont1;
		else if (cont1 < nWaterCont * 2 + 1 - nWaterCont / 2)
			cont2 = nWaterCont / 2;
		else
			cont2 = nWaterCont * 2 - cont1;

		if (i >= 0 && i<m_pGround->m_projectInfo.sXSize / 2)
		{
			// g_pD3dDev->SetStreamSource( 0, m_pGround->m_pVBWater[i],0, sizeof(GROUNDVERTEX) );
			for (j = tempz - (nWaterCont / 2 + cont2) - 1; j < tempz + (nWaterCont / 2 + cont2) + 2; j++)
			{
				if (j >= 0 && j<m_pGround->m_projectInfo.sYSize / 2)
				{
					int k = (i*m_pGround->m_projectInfo.sYSize / 2 + j);
					if (m_pGround->m_bWaterRender[k].useWater)
					{
						/// 2004.06.07 jschoi ÀýµÎÃ¼ Àû¿ë
						if (g_pFrustum->CheckSphere(i*TILE_SIZE * 2 + TILE_SIZE, nWaterHeight, j*TILE_SIZE * 2 + TILE_SIZE, nWaterRadius))
						{
							int nWaterTexNumber = m_pGround->m_bWaterRender[k].waterTexNumber + m_pGround->m_bWaterTexCont;
							if (nWaterTexNumber >= WATER_TEXTURE_COUNT)
								nWaterTexNumber -= WATER_TEXTURE_COUNT;
							// 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®
							//							if(g_pSOption->sLowQuality)
							//							{
							//								nWaterTexNumber = 0;
							//							}
							// end 2008-12-04 by bhsohn ÃÖ¼ÒÈ­ ¸ðµå½Ã, ¹° ·£´õ¸µ Ã³¸®


							// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
							// °¢ Å¸ÀÏº°·Î ÅØ½ºÃÄ ¹Ù²ã°¡¸é¼­ ·»´õ¸µ ÇÏ´Â °Íµé ÇÑ¹ø ¹¶ÃÄ¼­ ±×¸°´Ù
							// 						g_pD3dDev->SetTexture(0,m_pGround->m_pWaterTexture[nWaterTexNumber]);
							//						g_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, j*4, 2 );
							// 							
							//GROUNDVERTEX* vWaterVertices;
							//m_pGround->m_pVBWater[i]->Lock( 0, 0, (void**)&vWaterVertices, 0 );

							// ÃÖÁ¾ÀûÀ¸·Î ±×¸± ¹öÅØ½º ¹öÆÛ¿¡ °ª ´ëÀÔ
							//v[0].p		= vWaterVertices[j*4 + 0].p;
							//v[0].n		= vWaterVertices[j*4 + 0].n;
							//v[0].tu		= vWaterVertices[j*4 + 0].tu;
							//v[0].tv		= vWaterVertices[j*4 + 0].tv;
							v[0].tu2 = tu2;			// ÅØ½ºÃÄ ÁÂÇ¥µµ º¯°æ
							v[0].tv2 = 0.0f;

							//v[1].p		= vWaterVertices[j*4 + 1].p;
							//v[1].n		= vWaterVertices[j*4 + 1].n;
							//v[1].tu		= vWaterVertices[j*4 + 1].tu;
							//v[1].tv		= vWaterVertices[j*4 + 1].tv;
							v[1].tu2 = tu2;
							v[1].tv2 = 1.0f;

							//v[2].p		= vWaterVertices[j*4 + 2].p;
							//v[2].n		= vWaterVertices[j*4 + 2].n;
							//v[2].tu		= vWaterVertices[j*4 + 2].tu;
							//v[2].tv		= vWaterVertices[j*4 + 2].tv;
							v[2].tu2 = tu2 + 1.0f;
							v[2].tv2 = 0.0f;

							//v[3].p		= vWaterVertices[j*4 + 3].p;
							//v[3].n		= vWaterVertices[j*4 + 3].n;
							//v[3].tu		= vWaterVertices[j*4 + 3].tu;
							//v[3].tv		= vWaterVertices[j*4 + 3].tv;
							v[3].tu2 = tu2 + 1.0f;
							v[3].tv2 = 1.0f;

							//m_pGround->m_pVBWater[i]->Unlock();

							// ÃÖÁ¤ÀûÀ¸·Î ±×¸± ¹öÅØ½º ¹öÆÛ Æ÷ÀÎÅÍ Áõ°¡
							v += 4;
							// ±×¸®´Â ¹° Å¸ÀÏ °¹¼ö Áõ°¡
							++nRenderWaterTileCount;
							// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ
						}
					}
				}
			}
		}
		cont1++;
	}


	// 2008. 12. 11 by ckPark ¹°·»´õ¸µ
	// ÃÖÁ¾ÀûÀ¸·Î ±×¸± ¹° ¹öÅØ½º ¹öÆÛ ¾ð¶ô
	m_pGround->m_pToRenderWaterVB->Unlock();

	// Å¸ÀÏ °¹¼ö * 4¸¸Å­ ·£´õ¸µ
	g_pD3dDev->SetStreamSource(0, m_pGround->m_pToRenderWaterVB, 0, sizeof(WATERBUMPVERTEX));

	// 2009. 02. 11 by ckPark ¹° ·»´õ¸µ Å¸ÀÏº°·Î ·»´õ¸µ
	//g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, nRenderWaterTileCount * 4);
	for (i = 0; i<nRenderWaterTileCount; ++i)
		g_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	// end 2009. 02. 11 by ckPark ¹° ·»´õ¸µ Å¸ÀÏº°·Î ·»´õ¸µ

	// ÅØ½ºÃÄ ÇØÁ¦
	g_pD3dDev->SetTexture(0, 0);
	g_pD3dDev->SetTexture(1, 0);

	// ÅØ½ºÃÄ ¸ÅÆ®¸¯½º ÇØÁ¦
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	// 1¹ø ¸ÖÆ¼ ÅØ½ºÃÄ ÇØÁ¦	
	// 2009-01-15 by bhsohn ATIºñµð¿À Ä«µå¿¡¼­ ÀÌ ¿É¼ÇÀ» ¾È¾²¸é ±úÁø´Ù.
	g_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	g_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	// ÅØ½ºÃÄ ÁÂÇ¥ ¿ø·¡´ë·Î º¯°æ
	g_pD3dDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	g_pD3dDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	// 	g_pD3dDev->SetSamplerState( 1, D3DSAMP_ADDRESSU,   D3DTADDRESS_WRAP );
	//  g_pD3dDev->SetSamplerState( 1, D3DSAMP_ADDRESSV,   D3DTADDRESS_WRAP );

	// end 2008. 12. 11 by ckPark ¹°·»´õ¸µ
}
#endif

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSceneData::ChangeEnemyCharacterMode()
/// \brief		Àû Ä³¸¯ÅÍ¿Í À¯´Ö ¸ðµå¸¦ ¼­·Î ¹Ù²Û´Ù. Character -> Unit, Unit -> Character
/// \author		ispark
/// \date		2005-07-28 ~ 2005-07-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::ChangeEnemyCharacterMode(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK* pMsg)
{
	CMapEnemyIterator itEnemy = m_mapEnemyList.find(pMsg->ClientIndex);
	if(itEnemy != m_mapEnemyList.end() )
	{
		CEnemyData *pEnemy = itEnemy->second;
		
		if(pMsg->CharacterMode0 == FALSE)
		{
			// ±â¾î¸ðµå
			pEnemy->m_bEnemyCharacter = FALSE;
			// 2008-07-14 by bhsohn Ä³¸¯ÅÍ »óÅÂ ¹ö±× ¼öÁ¤
			pEnemy->m_infoCharacter.CharacterInfo.CharacterMode0 = 0;
		}
		else
		{
			// Ä³¸¯ÅÍ ¸ðµå
			pEnemy->m_bEnemyCharacter = TRUE;
			// 2008-07-14 by bhsohn Ä³¸¯ÅÍ »óÅÂ ¹ö±× ¼öÁ¤
			pEnemy->m_infoCharacter.CharacterInfo.CharacterMode0 = 1;

			// Ä³¸¯ÅÍ ¸ðµå´Â Upº¤ÅÍ ¹Ø¿¡²¨ ¼ÂÆÃ
			pEnemy->m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		pEnemy->m_vPos = A2DX(pMsg->PositionAVec3);
		pEnemy->m_vVel = A2DX(pMsg->TargetAVec3);
		D3DXVec3Normalize(&pEnemy->m_vVel, &pEnemy->m_vVel);
		pEnemy->Init();		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void ChangeObjectBodyCondition(INT nCode, BodyCond_t body);
/// \brief		°°Àº ¿ÀºêÁ§Æ®ÀÇ ¹øÈ£ÀÇ ¹ÙµðÄÁµð¼Ç°ªÀ» º¯°æÇÑ´Ù.
/// \author		dgwoo
/// \date		2007-04-23 ~ 2007-04-23
/// \warning	
///
/// \param		
/// \return		¿ÀºêÁ§Æ®¹øÈ£, º¯°æÇÒ ¹ÙµðÄÁµð¼Ç
///////////////////////////////////////////////////////////////////////////////
void CSceneData::ChangeObjectBodyCondition(INT nCode, BodyCond_t body)
{
	CObjectChild * pObj = (CObjectChild *)g_pGround->m_pBigObject->m_pChild;
	while(pObj)
	{
		if(pObj->m_nCode == nCode)
		{ 
			pObj->ChangeBodyconditionEvent(body);
		}
		pObj = (CObjectChild *)pObj->m_pNext;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			COLLISION_RESULT IsObjectCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos);
/// \brief		// Á¡ »çÀÌ¿¡ ¿ÀºêÁ§Æ®°¡ ÀÖ´ÂÁö Ã¼Å©
/// \author		// 2007-05-17 by bhsohn ¿ÀºêÁ§Æ® µÚ¿¡ ¼û¾úÀ»½Ã ¿¡ ´ëÇÑ Ã³ °Ë»ç Ã³¸®
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSceneData::IsObjectCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos, D3DXVECTOR3	vEnemyUp)
{
	BOOL bCheckObject = FALSE;	
	D3DXVECTOR3	vTargetPos;
	D3DXVECTOR3 vVel, vSide,vUpTemp;
	D3DXMATRIX mat;		
	FLOAT fDistance = D3DXVec3Length(&(vEmenyPos - vMyShuttlePos));
	
	vVel = vEmenyPos - vMyShuttlePos;
	D3DXVec3Normalize(&vVel, &vVel);
	
	vTargetPos = vMyShuttlePos;	
	
	D3DXVec3Cross(&vSide, &vEnemyUp, &vVel);
	D3DXVec3Cross(&vUpTemp, &vVel, &vSide);
	D3DXMatrixLookAtLH( &mat, &vTargetPos, &(vTargetPos+vVel), &vUpTemp);		
	
	FLOAT fDist = CheckCollRenderListRangeObject(mat, vTargetPos, fDistance);		
	if(fDist < fDistance)
	{
		bCheckObject = TRUE;		
		//DBGOUT("Ãæµ¹ÀÌ´å[%s] ~\n", m_infoCharacter.CharacterInfo.CharacterName);			
	}	
	return bCheckObject;
	
}

float CSceneData::CheckCollRenderListRangeObject(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fMovingDistance)
{	
	float fDist, fDistResult;
	fDist = fDistResult = DEFAULT_COLLISION_DISTANCE;
	vectorCObjectChildPtr::iterator itObj(m_vectorCulledObjectPtrList.begin());
	while(itObj != m_vectorCulledObjectPtrList.end())
	{
		CObjectChild * pObject = *itObj;
		// 2009-04-21 by bhsohn Ãæµ¹Ã¼Å© ¾ÈµÇ´Â Å¸ÀÔ ¿ÀºêÁ§Æ®¸é ¹«Á¶°Ç Å¸ÄÏÆÃ µÇ°Ô ¼öÁ¤
		BOOL bCheck = TRUE;
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh)
		{
			if((FALSE == pObject->m_pObjectInfo->Collision)
				||(pObject->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_EFFECT))
			{
				bCheck = FALSE;
			}			
		}
		// end 2009-04-21 by bhsohn Ãæµ¹Ã¼Å© ¾ÈµÇ´Â Å¸ÀÔ ¿ÀºêÁ§Æ®¸é ¹«Á¶°Ç Å¸ÄÏÆÃ µÇ°Ô ¼öÁ¤
		if(pObject && pObject->m_pObjectInfo && pObject->m_pObjMesh && 			
			//pObject->m_pObjectInfo->ObjectRenderType != OBJECT_BIG_EFFECT // 2008-10-17 by bhsohn Å¸ÄÏÆÃ ½Ã½ºÅÛ¿¡¼­ Åõ¸í ¿ÀºêÁ§Æ®´Â Ã¼Å© ÇÏÁö ¾È°Ô º¯°æ
			bCheck)// 2009-04-21 by bhsohn Ãæµ¹Ã¼Å© ¾ÈµÇ´Â Å¸ÀÔ ¿ÀºêÁ§Æ®¸é ¹«Á¶°Ç Å¸ÄÏÆÃ µÇ°Ô ¼öÁ¤			
		{
			float fRadius = pObject->m_pObjMesh->m_fRadius;
			if( D3DXVec3Length(&(pObject->m_vOriPos - vPos)) < fRadius * 2.0f + fMovingDistance)
			{
//				pObject->m_pObjMesh->Tick(pObject->m_fCurrentTime);
//				pObject->m_pObjMesh->SetWorldMatrix(pObject->m_mMatrix);
				fDistResult = pObject->m_pObjMesh->CheckCollision(mat,vPos,DEFAULT_COLLISION_DISTANCE,FALSE,FALSE).fDist;
				if(fDist > fDistResult)
				{
					fDist = fDistResult;
				}
			}			
		}
		itObj++;
	}
	return fDist;


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			COLLISION_RESULT IsObjectCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos);
/// \brief		// Á¡ »çÀÌ¿¡ ÁöÇü°¡ ÀÖ´ÂÁö Ã¼Å©
/// \author		// 2007-05-17 by bhsohn ¿ÀºêÁ§Æ® µÚ¿¡ ¼û¾úÀ»½Ã ¿¡ ´ëÇÑ Ã³ °Ë»ç Ã³¸®
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSceneData::IsTileCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos)
{
	BOOL bCheckObject = FALSE;	
	// ÁöÇü°ú Ãæµ¹ °Ë»ç
	if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
	{		
		return bCheckObject;
	}
	
	D3DXVECTOR3	vTargetPos;
	D3DXVECTOR3 vVel, vSide,vUpTemp;
	D3DXMATRIX mat;			
	FLOAT fDistance = D3DXVec3Length(&(vEmenyPos - vMyShuttlePos));
	FLOAT fTargetDistance = 0;
	FLOAT fMovingDistance = TILE_CHECK_DISTANCE;

	if(fDistance < MIN_TILE_CHECK_DISTANCE)
	{
		return bCheckObject;
	}
	
	vVel = vEmenyPos - vMyShuttlePos;
	D3DXVec3Normalize(&vVel, &vVel);

	if(0 == D3DXVec3Length(&vVel))
	{
		return bCheckObject;
	}
	
	vTargetPos = vMyShuttlePos;	
	DWORD dwCnt = 0;
	for(dwCnt = 0; ; dwCnt++)	
	{
		vTargetPos += vVel*fMovingDistance;
		fTargetDistance = D3DXVec3Length(&(vTargetPos - vMyShuttlePos));			

		if(fTargetDistance > fDistance)
		{
			break;
		}
		// ¹Ù´Ú Ãæµ¹
		float fHeight = 0.0f;
		D3DXVECTOR3 vNor;
		g_pGround->CheckCollMap(vTargetPos, &fHeight, &vNor);
		if(fHeight >= vTargetPos.y)
		{
			bCheckObject = TRUE;
			break;
		}
		long lDiff = abs(fHeight - vTargetPos.y);		
		if(lDiff < MIN_TILE_DIFF_CHECK_HEIGHT)
		{
			// Ã¼Å©·çÆ¾°ú ¸ÊÀÇ ³ôÀÌ Â÷°¡ 100¹ÌÅÍ ¸¸
			fMovingDistance /= 2;
			if(fMovingDistance < MIN_TILE_CHECK_DISTANCE)
			{
				// ÃÖ¼Ò´Â Å¸ÀÏÀÇ ¹Ý
				fMovingDistance = MIN_TILE_CHECK_DISTANCE;
			}	
		}
		else if(lDiff > MAX_TILE_DIFF_CHECK_HEIGHT)
		{
			fMovingDistance = TILE_CHECK_DISTANCE;
		}			
	}		
	return bCheckObject;	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::DeleteRenderEnemy(ClientIndex_t nIndex)
/// \brief		
/// \author		// 2007-06-13 by bhsohn ¸Þ¸ð¸® ¹ö±× µð¹ö±ë
/// \date		2007-06-13 ~ 2007-06-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::DeleteRenderEnemy(ClientIndex_t nIndex)
{
	CVecEnemyIterator it = m_vecEnemyRenderList.begin();
	while( it != m_vecEnemyRenderList.end() )
	{
		CEnemyData* pEnemy = (CEnemyData*)(*it);
		ENEMYINFO stuEnemyinfo = pEnemy->GetEnemyInfo();
		if(stuEnemyinfo.CharacterInfo.ClientIndex == nIndex)
		{
			it = m_vecEnemyRenderList.erase(it);
			continue;
		}
		it++;	
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSceneData::ReLoadEnemyRenderList()
/// \brief		
/// \author		// 2007-08-29 by bhsohn ÃÖ¼Ò ÇÁ·¹ÀÓ½Ã ±âº» ¾Æ¸Ó¸¸ ·ÎµùÇÏ°Ô²û º¯°æ
/// \date		2007-08-29 ~ 2007-08-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::ReLoadEnemyRenderList()
{
	g_pD3dApp->m_pUnitRender->DeleteDeviceObjects();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-14 by bhsohn EP3 °ü·Ã Ã³¸®
/// \date		2008-07-14 ~ 2008-07-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSceneData::GetEmemyCharacterUniqueNumber(char* pName, UID32_t* o_CharacterUniqueNumber)
{
	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
		if(0 == strcmp(pName, itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName))
		{
			(*o_CharacterUniqueNumber) = itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber;
			return TRUE;
		}
		itEnemy++;
	}
	(*o_CharacterUniqueNumber) = 0;
	return FALSE;
}

// 2012-07-19 by isshin ¾ÆÅÛ¹Ì¸®º¸±â
ClientIndex_t CSceneData::GetEmemyCharacterClientIndex(char* pName)
{
	ClientIndex_t TargetEnemyClientIndex = 0;
	CMapEnemyIterator itEnemy = m_mapEnemyList.begin();
	while(itEnemy != m_mapEnemyList.end())
	{
		if(0 == strcmp(pName, itEnemy->second->m_infoCharacter.CharacterInfo.CharacterName))
		{
			TargetEnemyClientIndex = itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex;			
			return TargetEnemyClientIndex;
		}
		itEnemy++;
	}	
	return TargetEnemyClientIndex;
}
// end 2012-07-19 by isshin ¾ÆÅÛ¹Ì¸®º¸±â

void CSceneData::InvectoryFullMessage()
{
	if(m_fGetItemMessage < 0.0f)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRERR_ERROR_0022,COLOR_ERROR);
		m_fGetItemMessage = INVENTORY_MESSAGE_GAP;
	}
}


// 2009. 11. 02 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ÀÎ½ºÅÏ½º ´øÁ¯ ½Ã½ºÅÛ
CObjectChild*	CSceneData::FindMapObjectByIndex( int nIndex )
{
	int x = (int)((g_pGround->m_projectInfo.sXSize * TILE_SIZE) / MAP_BLOCK_SIZE);
	int z = (int)((g_pGround->m_projectInfo.sYSize * TILE_SIZE) / MAP_BLOCK_SIZE);
	CObjectChild* pMapObj = NULL;
	for( int i=0; i<x; i++ )
	{
		for( int j=0; j<z; j++ )
		{
			pMapObj = (CObjectChild *)g_pGround->m_ppObjectList[i][j].m_pChild;
			while( pMapObj )
			{
				if( pMapObj->m_sEventIndexFrom == nIndex )
				{
					return pMapObj;
				}
				else
				{
					pMapObj = (CObjectChild *)pMapObj->m_pNext;
				}
			}
		}
	}
	
	pMapObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_sEventIndexFrom == nIndex )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	
	
	pMapObj = (CObjectChild *)m_pGround->m_pWaterObject->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_sEventIndexFrom == nIndex )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	
	
	pMapObj = (CObjectChild *)m_pGround->m_pObjectMonster->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_sEventIndexFrom == nIndex )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	
	

	// 2011-02-08 by jhahn EP4 Æ®¸®°Å  ½Ã½ºÅÛ ¿ÀºêÁ§Æ® º¯°æ	
	pMapObj = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;
	while ( pMapObj )
	{
		if( pMapObj->m_sEventIndexFrom == nIndex )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	
	// 2011-02-08 by jhahn EP4 Æ®¸®°Å  ½Ã½ºÅÛ ¿ÀºêÁ§Æ® º¯°æ	
	
	return NULL;
}
// end 2009. 11. 02 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå ÀÎ½ºÅÏ½º ´øÁ¯ ½Ã½ºÅÛ

// 2011-11-01 by jhahn EP4 Æ®¸®°Å ½Ã½ºÅÛ 
CObjectChild*	CSceneData::FindMapObjectEvendByIndex( int nIndex )
{

	CObjectChild* pMapObj = (CObjectChild *)m_pGround->m_pObjectEvent->m_pChild;

	while ( pMapObj )
	{
		if( pMapObj->m_sEventIndexFrom == nIndex )
		{
			return pMapObj;
		}
		else
		{
			pMapObj = (CObjectChild *)pMapObj->m_pNext;
		}
	}	
	return NULL;
}	 
//end 2011-11-01 by jhahn EP4 Æ®¸®°Å ½Ã½ºÅÛ 
// 2010. 03. 15 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷(¿¬Ãâ Ã³¸®)
void	CSceneData::ChangeSkyBox( char* szSkyBoxName )
{
	m_pETCRender->ChangeSkyBox( szSkyBoxName );
}

void	CSceneData::ChangeEventObjectBodyCondition( short nEventParam, BodyCond_t body )
{
	CObjectChild* pObj = FindMapObjectByIndex( nEventParam );
	if( pObj && pObj->m_pCharacterInfo )
	{
		pObj->m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(0xffffffffffffffff);
		pObj->ChangeBodyconditionEvent( body );
	}
}
// end 2010. 03. 15 by ckPark ÀÎÇÇ´ÏÆ¼ ÇÊµå 2Â÷(¿¬Ãâ Ã³¸®)

// 2010. 10. 05 by jskim ¸Ê·Îµù ±¸Á¶ º¯°æ
void	CSceneData::StepBackground_Step1()
{
	if(m_bIsRestore)
	{
		InitRes();
	}
}

void	CSceneData::StepBackground_Step2()
{
	if(m_bIsRestore)
	{
		RestoreRes();
		m_bIsRestore = FALSE;
	}
}

void	CSceneData::StepBackground_Step3()
{
	int i;
	FILE * readMap;
	int re = -1;
	WORKSPACE iWorkspace;
	char strPath[256];
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_MAP, RC_MAP_WORKSPACE );
	if( strlen( strPath ) > 0 )
	{
 		readMap = fopen(strPath,"rb");
		fseek(readMap,20,SEEK_SET);
		fread(&iWorkspace,sizeof(WORKSPACE),1,readMap);
		int a = iWorkspace.numberOfProject;
		for(i=0;i<a;i++)
		{
			fread(&m_prProject,sizeof(PROJECTINFO),1,readMap);
			char buf[32];
			wsprintf( buf, "%04d", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
			re = strcmp( m_prProject.strProjectName, buf );

			if(re==0)
			{
				DBGOUT("MAP Project Info[name:%s]\n   [Water Height:%.2f]\n   Day[D:%.2f,%.2f,%.2f][A:%.2f,%.2f,%.2f]\n   Night[D:%.2f,%.2f,%.2f][A:%.2f,%.2f,%.2f]\n", 
					m_prProject.strProjectName, m_prProject.fWaterHeight, 
					m_prProject.fDiffuseR1, m_prProject.fDiffuseG1, m_prProject.fDiffuseB1,
					m_prProject.fAmbientR1, m_prProject.fAmbientG1, m_prProject.fAmbientB1,
					m_prProject.fDiffuseR2, m_prProject.fDiffuseG2, m_prProject.fDiffuseB2,
					m_prProject.fAmbientR2, m_prProject.fAmbientG2, m_prProject.fAmbientB2
					);

				// map type °áÁ¤
				int nTemp = atoi(m_prProject.strProjectName);
				if(nTemp > 1000)
				{
					// 2006-07-19 by ispark, ¿¾³¯ µµ½Ã¸¦ ÆÇ´ÜÇÏ±â À§ÇÑ°Å »èÁ¦
//					m_byMapType =  nTemp / 1000;
					m_byMapType = MAP_TYPE_NORMAL_FIELD;
				}
				else
				{
					if(0 == strcmp(m_prProject.strProjectName,"0100"))
						m_byMapType = MAP_TYPE_TUTORIAL;
//					else if(0 == strcmp(iProject.strProjectName,"0106"))
//						m_byMapType = MAP_TYPE_CITY;
					else
						m_byMapType = MAP_TYPE_NORMAL_FIELD;
				}
				break;
			}
		}
		fclose(readMap);
	}
	else
	{
		return;
	}
	return;		
}

void	CSceneData::StepBackground_Step4()
{
	//step 4
	if(m_pGround)
	{
		if(m_vecEnemyBlockList)
		{
			for(int i = 0; i < m_nBlockSizeX;i++)
			{
				for(int j = 0;j < m_nBlockSizeY;j++)
				{
					m_vecEnemyBlockList[i*m_nBlockSizeY + j].clear();
				}
			}
			util::del_array(m_vecEnemyBlockList);
		}
		if(m_vecMonsterList)
			{
			for(int i = 0; i < m_nBlockSizeX;i++)
			{
				for(int j = 0;j < m_nBlockSizeY;j++)
				{
					m_vecMonsterList[i*m_nBlockSizeY + j].clear();
				}
			}
			util::del_array(m_vecMonsterList);
		}
	}
	m_nBlockSizeX = m_prProject.sXSize/3;
	m_nBlockSizeY = m_prProject.sYSize/3;
	if(m_prProject.sXSize%3)
		m_nBlockSizeX++;
	if(m_prProject.sYSize%3)
		m_nBlockSizeY++;
	m_vecEnemyBlockList = new CVecEnemyList[m_nBlockSizeX*m_nBlockSizeY];
	m_vecMonsterList = new CVecMonsterList[m_nBlockSizeX*m_nBlockSizeY];
}

void	CSceneData::StepBackground_Step5()
{
	int checkMapname = -1;
	if(m_pGround)
	{
		char buf[32];
		wsprintf( buf, "%04d", g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
		checkMapname = strcmp(m_pGround->m_projectInfo.strProjectName, buf);
	}
	if(m_pGround)
	{
		m_pGround->DeleteDeviceObjects();
		util::del(m_pGround);
	}
	if(m_pETCRender)
	{
		m_pETCRender->DeleteDeviceObjects();
	}
	if(m_pWater && m_bWaterShaderRenderFlag)
		m_pWater->DeleteDeviceObjects();
	
	m_pGround = new CBackground(m_prProject);

	if(m_pGround)
	{
		if(FAILED(m_pGround->InitDeviceObjects()))
		{
			util::del(m_pGround);
		}
		else m_pGround->RestoreDeviceObjects();
	}
		
	if(m_pETCRender)
		m_pETCRender->InitDeviceObjects();
	if(m_pWater && m_bWaterShaderRenderFlag)
		m_pWater->InitDeviceObjects();
	if(m_pETCRender)
		m_pETCRender->RestoreDeviceObjects();
	if(m_pWater && m_bWaterShaderRenderFlag)
		m_pWater->RestoreDeviceObjects();
	{
		g_pD3dApp->UpdateGameStartMapInfo();
	}
}
// end 2010. 10. 05 by jskim ¸Ê·Îµù ±¸Á¶ º¯°æ

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-07 by bhsohn ¼¼·ÂÆ÷ÀÎÆ® °³¼±¾È ½Ã½ºÅÛ
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSceneData::ShowObjectChild_ByType(BOOL bShow, BYTE bEventType, INT nBuildingKind)
{
#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN			
	if(!m_pGround)
	{
		return;
	}
	// stObject = g_pScene->m_pObjectRender->GetPickObject(pt->x, pt->y);
	{
		CObjectChild * pBigObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;
		
		while ( pBigObj )
		{
			if(!pBigObj->m_pObjectInfo)
			{
				pBigObj = (CObjectChild *)pBigObj->m_pNext;
				continue;
			}	
						
			//EVENT_TYPE_OBJ_BUILDING_NPC
			if(bEventType == pBigObj->m_bEventType)
			{
				INT nShopIndex = pBigObj->m_sEventIndexTo;
				BUILDINGNPC* pBuilding = g_pDatabase->GetBuildingNPCInfo( nShopIndex );
				if(pBuilding && (nBuildingKind == pBuilding->BuildingKind))
				{
					pBigObj->SetShowNode(bShow);
				}				
			}
			
			pBigObj = (CObjectChild *)pBigObj->m_pNext;
		}	
	}
	{
		CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;		
		while(pObjEvent)
		{
			if(!pObjEvent->m_pObjectInfo)
			{
				pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
				continue;
			}
			if(bEventType == pObjEvent->m_bEventType)
			{
				INT nShopIndex = pObjEvent->m_sEventIndexTo;				
				BUILDINGNPC* pBuilding = g_pDatabase->GetBuildingNPCInfo( nShopIndex );
				if(pBuilding && (nBuildingKind == pBuilding->BuildingKind))
				{
					pObjEvent->SetShowNode(bShow);
				}				
			}

			
			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}
#endif
}
//////////////////////////// ¿ÀºêÁ§Æ® ¹øÈ£·Î ¼û±â±â ////////////////////////////
void CSceneData::ShowObjectChild_ByIndex(BOOL bShow, INT nCodeNum)
{
	if(!m_pGround)
	{
		return;
	}
	
	{
		CObjectChild * pBigObj = (CObjectChild *)m_pGround->m_pBigObject->m_pChild;
		
		while ( pBigObj )
		{
			if(!pBigObj->m_pObjectInfo)
			{
				pBigObj = (CObjectChild *)pBigObj->m_pNext;
				continue;
			}					
			if(nCodeNum == pBigObj->m_nCode)
			{
				pBigObj->SetShowNode(bShow);									
			}			
			pBigObj = (CObjectChild *)pBigObj->m_pNext;
		}	
	}
	{
		CObjectChild * pObjEvent = (CObjectChild *)g_pGround->m_pObjectEvent->m_pChild;		
		while(pObjEvent)
		{
			if(!pObjEvent->m_pObjectInfo)
			{
				pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
				continue;
			}
			if(nCodeNum == pObjEvent->m_nCode)
			{
				pObjEvent->SetShowNode(bShow);
			}				
			pObjEvent = (CObjectChild *)pObjEvent->m_pNext;
		}
	}
}
void CSceneData::UpdateInflBuffNPCObject()
{
#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN
	if(!g_pShuttleChild || !g_pD3dApp)
	{
		DBGOUT("CSceneData::ShowInflBuffNPC Error \n");
		return;
	}		
	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES*	pVitory = g_pD3dApp->GetInflConVictorites();	
	INT myInfluenceType = g_pShuttleChild->GetMyShuttleInfo().InfluenceType;
	INT nVitoryPoint = 0;
	if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_VCN))
	{		
		nVitoryPoint = pVitory->PointBCU;
	}
	else if(COMPARE_INFLUENCE(myInfluenceType, INFLUENCE_TYPE_ANI))
	{		
		nVitoryPoint = pVitory->PointANI;
	}	
	BOOL bShowNPC = FALSE;
	if(nVitoryPoint >= 2)
	{
		// 2½ÂºÎÅÍ SHOW
		bShowNPC = TRUE;
	}
#ifdef _DEBUG
//	bShowNPC = TRUE;
#endif
	DBGOUT("CSceneData::ShowInflBuffNPC [%d]\n", bShowNPC);

	//////////////// NPCº¸ÀÏÁö ¿©ºÎ ////////////////
	{
		ShowObjectChild_ByType(bShowNPC, EVENT_TYPE_OBJ_BUILDING_NPC, BUILDINGKIND_INFLBUFF_SHOP);
	}

	//////////////// º°ÀÌ º¸ÀÏÁö ¿©ºÎ ////////////////	
	{
		int nShowIdx = nVitoryPoint-1;
		nShowIdx = max(0, nShowIdx);
		int nCnt = 0;

		const INT MAX_STAR_WIN_COUNT = 8;
		INT nBCUStarIndex[MAX_STAR_WIN_COUNT] = 
		{
			0,			//
			3221600,	// 2¿¬½Â
			3221700,	// 3¿¬½Â
			3221800,	// 4¿¬½Â
			3221900,	// 5¿¬½Â
			3222000,	// 6¿¬½Â
			3222100,	// 7¿¬½Â
			3222200		// 8¿¬½Â
		};
		INT nANIStarIndex[MAX_STAR_WIN_COUNT] = 
		{
			0,			//
			3222300,	// 2¿¬½Â
			3222400,	// 3¿¬½Â
			3222500,	// 4¿¬½Â
			3222600,	// 5¿¬½Â
			3222700,	// 6¿¬½Â
			3222800,	// 7¿¬½Â
			3222900		// 8¿¬½Â
		};		

		for(nCnt = 1; nCnt <= nShowIdx; nCnt++)
		{
			INT nStartObjIndex = 0;
			if(IS_VCN_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
			{		
				nStartObjIndex = nBCUStarIndex[nCnt];
			}
			else if(IS_ANI_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
			{		
				nStartObjIndex = nANIStarIndex[nCnt];
			}	
			else
			{
				continue;
			}
			if(0 == nStartObjIndex)
			{
				continue;
			}
			ShowObjectChild_ByIndex(bShowNPC, nStartObjIndex);
		}
		for(nCnt = (nShowIdx+1); nCnt < MAX_STAR_WIN_COUNT; nCnt++)
		{
			INT nStartObjIndex = 0;
			if(IS_VCN_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
			{		
				nStartObjIndex = nBCUStarIndex[nCnt];
			}
			else if(IS_ANI_CITY_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
			{		
				nStartObjIndex = nANIStarIndex[nCnt];
			}	
			else
			{
				continue;
			}
			if(0 == nStartObjIndex)
			{
				continue;
			}
			ShowObjectChild_ByIndex(FALSE, nStartObjIndex);
		}
	}
	// 2013-06-19 by bhsohn ¿¬½Â ¹öÇÁ »óÁ¡À» ¿ÀÇÂÇÑ »óÅÂ¿¡¼­ ¿¬½ÂÀÌ ±úÁú ½Ã ¿¬½Â ¹öÇÁ »óÁ¡ÀÌ Á¾·á
	//¿¬½Â ¹öÇÁ »óÁ¡À» ¿ÀÇÂÇÑ »óÅÂ¿¡¼­ ¿¬½ÂÀÌ ±úÁú ½Ã ¿¬½Â ¹öÇÁ »óÁ¡ÀÌ Á¾·á
	if(!bShowNPC)
	{
		if( g_pInterface && g_pInterface->m_pCityBase && g_pInterface->m_pCityBase->GetCurrentBuildingNPC()
			&& (BUILDINGKIND_INFLBUFF_SHOP == g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind ))
		{
			g_pInterface->m_pCityBase->CloseCurrentEnterBuilding();	// »ç¿ëÁßÀÌ´ø »óÁ¡ ´Ý±â
		}

	}
	// END 2013-06-19 by bhsohn ¿¬½Â ¹öÇÁ »óÁ¡À» ¿ÀÇÂÇÑ »óÅÂ¿¡¼­ ¿¬½ÂÀÌ ±úÁú ½Ã ¿¬½Â ¹öÇÁ »óÁ¡ÀÌ Á¾·á
#endif
}
