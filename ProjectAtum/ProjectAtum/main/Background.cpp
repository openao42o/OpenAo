// Background.cpp: implementation of the CBackground class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Background.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "ObjectChild.h"
#include <stdio.h>
#include "FileHeader.h"
#include "INFImage.h"
#include "QuadGround.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "Camera.h"
#include "dxutil.h"
#include "GameDataLast.h"
#include "SkinnedMesh.h"
#include "MonRender.h"
#include "TutorialSystem.h"
#include "QuestData.h"
#include "INFGameMain.h"
#include "INFMissionInfo.h"
#include "INFCityBoard.h"			// 2006-04-11 by ispark
#include "INFImageEx.h"				// 2011. 10. 10 by jskim UI시스템 변경

#define MAP_BLOCK_MAX_NUM		256

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef struct _EVENT_DATA
{
	D3DXVECTOR3 m_vPosition;
	int			m_bMove;
	BYTE		m_bPKType;
	BOOL		m_bEnableLand;
	BYTE		m_bEnvetType;
	short		m_EventwParam1;
	short		m_EventwParam2;
	short		m_EventwParam3;
	char		m_EventstrParam[40];
	BOOL		m_bUseWater;			// Water 사용
} EVENT_DATA;


CBackground::CBackground(PROJECTINFO info)//,int n)
{
	FLOG( "CBackground(PROJECTINFO info)" );
	g_pGround = this;
	memcpy(&m_projectInfo,&info,sizeof(PROJECTINFO));

	m_fSizeMap_X	= m_projectInfo.sXSize * TILE_SIZE;				// 맵의 X축 Size로 단위는 단위벡터임
	m_fSizeMap_Z	= m_projectInfo.sYSize * TILE_SIZE;				// 맵의 Z축 Size로 단위는 단위벡터임
	m_NUM_BLOCK_X	= int(m_fSizeMap_X/MAP_BLOCK_SIZE);
	m_NUM_BLOCK_Z	= int(m_fSizeMap_Z/MAP_BLOCK_SIZE);


//	m_pVBRender = new LPDIRECT3DVERTEXBUFFER8[m_projectInfo.sYSize];
	m_pVBWater = new LPDIRECT3DVERTEXBUFFER9[m_projectInfo.sYSize/2];

	// 2008. 12. 11 by ckPark 물렌더링
	m_pToRenderWaterVB	= NULL;
	// end 2008. 12. 11 by ckPark 물렌더링

	int i;
//	for(i=0;i<m_projectInfo.sYSize;i++)
//	{
//		m_pVBRender[i] = NULL;
//	}
	for(i=0;i<m_projectInfo.sYSize/2;i++)
		m_pVBWater[i] = NULL;
	m_bWaterRender = new WATERINFO[m_projectInfo.sXSize*m_projectInfo.sYSize/4];
	memset(m_bWaterRender, 0x00, sizeof(WATERINFO)*(m_projectInfo.sXSize*m_projectInfo.sYSize/4));

	m_pTileVertexArray = NULL;
	m_pTileInfo = NULL;
	m_fCheckWaterTime = 0.0f;
	m_pTileInfo = new TILEINFOCLIENT[m_projectInfo.sXSize*m_projectInfo.sYSize];
	memset(m_pTileInfo, 0x00, sizeof(TILEINFOCLIENT)*m_projectInfo.sXSize*m_projectInfo.sYSize);
	m_pTileVertexArray = new VERTEXINFO[(m_projectInfo.sXSize+1)*(m_projectInfo.sYSize+1)];
	memset(m_pTileVertexArray, 0x00, sizeof(TILEINFOCLIENT)*(m_projectInfo.sXSize+1)*(m_projectInfo.sYSize+1));
	m_bTileRender = new BOOL[m_projectInfo.sXSize*m_projectInfo.sYSize];
	for(i=0;i<WATER_TEXTURE_COUNT;i++)
		m_pWaterTexture[i] = NULL;
//	m_fCheckWaterTexAniTime = GetWaterTick(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
	// 2005-06-29 by ispark
	m_fCheckWaterTexAniTime = 0.1f;
	m_bWaterTexCont = 0;

	m_pBackgroundData = NULL;
	m_pTextureData = NULL;
	m_ppObjectList		= NULL;
	m_pBigObject = NULL;
	m_pWaterObject = NULL;// 2005-02-11 by jschoi  물 오브젝트 타입 추가
	m_pObjectMonster = NULL;
//	m_pMeshMinimap = NULL;				// 2005-09-12 by ispark 별도 처리

	D3DXVECTOR3 vPos,vVel,vUp;
	vPos = D3DXVECTOR3(0,0,0);
	vVel = D3DXVECTOR3(0,0,1);
	vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtRH( &m_mMatrix, &vPos, &(vPos + 10.0f*vVel), &vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	m_mMatrix._42 = -100;

	memset(m_lightSupport, 0x00, sizeof(D3DLIGHT9)*4);

	m_pBigObject   = new CAtumNode;
	m_pWaterObject = new CAtumNode;// 2005-02-11 by jschoi  물 오브젝트 타입 추가
	m_pObjectEvent = new CAtumNode;
	m_pObjectMonster = new CAtumNode;
	m_pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	m_pQuad = NULL;

	m_pMiniBigMapImage = NULL;
	m_pMiniBigMapCityImage = NULL;
	m_bMiniMap = FALSE;

	m_pDetailMap = NULL;
	m_bBazaar = FALSE;
}

CBackground::~CBackground()
{
	FLOG( "~CBackground()" );
	g_pGround = this;
	util::del_array(m_pTileInfo);
	util::del_array(m_pTileVertexArray);
//	util::del_array(m_pVBRender);
	util::del_array(m_pVBWater);
	util::del_array(m_bWaterRender);


	// 2008. 12. 11 by ckPark 물렌더링
	SAFE_RELEASE(m_pToRenderWaterVB);
	// end 2008. 12. 11 by ckPark 물렌더링


//	util::del(m_pMeshMinimap);				// 2005-09-12 by ispark 별도 처리
//	util::del(m_pMiniBigMapImage);
	int i;
	for(i=0;i<WATER_TEXTURE_COUNT;i++)
		SAFE_RELEASE(m_pWaterTexture[i]);

	util::del_array(m_bTileRender);

	map<INT,EFFECTINFOCLIENT *>::iterator itEff = m_mapEffectList.begin();
	while(itEff != m_mapEffectList.end())
	{
		util::del(itEff->second);
		itEff++;
	}
	m_mapEffectList.clear();

	util::del(m_pBigObject);
	util::del(m_pWaterObject);// 2005-02-11 by jschoi  물 오브젝트 타입 추가
	util::del(m_pObjectMonster);
	util::del(m_pObjectEvent);
	
	vector<MONSTERINFO*>::iterator itMon = m_vecMonsterResList.begin();
	while(itMon != m_vecMonsterResList.end())
	{
		MONSTERINFO * pMon = *itMon;
		util::del(pMon);
		itMon++;
	}
	m_vecMonsterResList.clear();

	util::del(m_pTextureData);
	util::del(m_pMiniMapDataHeader);
	util::del(m_pMiniMapCityDataHeader);

	SAFE_RELEASE(m_pDetailMap);
}

void CBackground::GetBlockAdjacentToPosition(float x, float z, float fRadius, TWO_BLOCK_INDEXES &blockIdx)
{
	short sVal;
	sVal = (short)((x + fRadius) / MAP_BLOCK_SIZE);
	blockIdx.sMaxX = (sVal >= m_NUM_BLOCK_X)?(m_NUM_BLOCK_X-1):sVal;

	sVal = (short)((z + fRadius) / MAP_BLOCK_SIZE);
	blockIdx.sMaxZ = (sVal >= m_NUM_BLOCK_Z)?(m_NUM_BLOCK_Z-1):sVal;

	sVal = (short)((x - fRadius) / MAP_BLOCK_SIZE);
	blockIdx.sMinX = (sVal <= 0)?0:sVal;

	sVal = (short)((z - fRadius) / MAP_BLOCK_SIZE);
	blockIdx.sMinZ = (sVal <= 0)?0:sVal;
}

HRESULT CBackground::InitDeviceObjects()
{
	FLOG( "CBackground::InitDeviceObjects()" );
	HRESULT hr;
	int i,j;
	char strPath[256];
	char buf[256];
	DataHeader*  pHeader;
	DataHeader*  pHeader2;	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보

	if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		m_pTextureData = new CGameData;
		wsprintf( buf, "%04d.map", m_pMapInfo->Map);
		DBGOUT("Map Channel Information : MapLoading, Texture[%s]\n", buf);
		g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_MAP, buf );
		if( strlen(strPath) > 0 )
		{
			if(m_pTextureData->SetFile(strPath,FALSE, NULL,0)== FALSE)
			{
				util::del(m_pTextureData);
				DBGOUT("------------------------>FAILED Loading Map Texture\n");
			}
		}
		else
		{
			util::del(m_pTextureData);
			return E_FAIL;
		}
	}

	/*--------------------------------------------------------------------------*/
	// 맵번호.tex에서 불러오는 부분
	// 2005-10-12 by ispark
	// 2006-06-15 by ispark, 수정
 	// Detail Map
	DataHeader *pDataHeader;
	CGameData MiniMapData;
	m_pDetailMap = NULL;
	wsprintf(buf, ".\\Res-Map\\%04d.tex", m_pMapInfo->Tex);
	MiniMapData.SetFile( buf, FALSE, NULL, 0, FALSE );
	wsprintf(buf, "%04dd",m_pMapInfo->MapIndex);
	pDataHeader = MiniMapData.FindFromFile(buf);
	if(pDataHeader)
	{
		D3DXIMAGE_INFO SrcInfo;
		if(FAILED(D3DXCreateTextureFromFileInMemoryEx(g_pD3dDev, (LPCVOID)pDataHeader->m_pData , 
			pDataHeader->m_DataSize,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 
			0, &SrcInfo, NULL, &m_pDetailMap)))
		{
			m_pDetailMap = NULL;
			return E_FAIL;
		}
		util::del(pDataHeader);
	}
	else
	{
		DBGOUT("Not Detail Data : %d\n", g_pGround->m_nMapNumber);
	}
	
	// 2005-09-06 by ispark
 	// 2D 미니맵IDS_DIRECTORY_TEXTURE
	m_bMiniMap = FALSE;
	wsprintf(buf, ".\\Res-Map\\%04d.tex",  m_pMapInfo->Tex);
	MiniMapData.SetFile( buf, FALSE, NULL, 0, FALSE );
	// 2007-03-19 by dgwoo 미니맵의 경우 Tex의 정보로 읽어온다.
	wsprintf(buf, "%04d", m_pMapInfo->Tex);
	m_pMiniMapDataHeader = MiniMapData.FindFromFile(buf);
	if(m_pMiniMapDataHeader)
	{
// 2011. 10. 10 by jskim UI시스템 변경
		m_pMiniBigMapImage = new CINFImageEx;		
		m_pMiniBigMapImage->InitDeviceObjects(m_pMiniMapDataHeader);
// end 2011. 10. 10 by jskim UI시스템 변경
//		m_pMiniBigMapImage->RestoreDeviceObjects();
		m_bMiniMap = TRUE;
	}		
	wsprintf(buf, "%d_c", g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	m_pMiniMapCityDataHeader = MiniMapData.FindFromFile(buf);
	if(m_pMiniMapCityDataHeader)
	{
// 2011. 10. 10 by jskim UI시스템 변경
		m_pMiniBigMapCityImage = new CINFImageEx;		
		m_pMiniBigMapCityImage->InitDeviceObjects(m_pMiniMapCityDataHeader);
// end 2011. 10. 10 by jskim UI시스템 변경
		m_bMiniMap = TRUE;
	}		
	/*--------------------------------------------------------------------------*/
	
	// 2005-10-12 by ispark
	// 맵 번호 CBackground가 가지기
	m_nMapNumber = g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex;

	//////////////////////////////////////////////////////////////////////////
	// 전광판
	// 2006-04-11 by ispark
	if(g_pGameMain)
		g_pGameMain->InitCityBoard(m_nMapNumber);

	//////////////////////////////////////////////////////////////////////////
	// 바자맵인가
	g_pD3dApp->m_bBazaar = m_bBazaar = FALSE;
	if(m_nMapNumber == 1001 || m_nMapNumber == 1002)
	{
		g_pD3dApp->m_bBazaar = m_bBazaar = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////

	// 2007-08-02 by bhsohn 바자맵에서 출격버튼 표현안함.
	if(g_pGameMain)
	{
		g_pGameMain->RefreshMissionUI();
	}
	// end 2007-08-02 by bhsohn 바자맵에서 출격버튼 표현안함.
	
	m_pBackgroundData = new CGameData;
	// 2005-07-11 by ispark
	// 2005-07-29 by ispark
	// 맵상 이동 가능한 최대 높이 셋팅
	// RenderMapIndex 복사 맵 적용
	//MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	if(m_pMapInfo != NULL)
	{
		//g_pScene->SetMaxAtitudeHeight(pMapInfo->ClientMaxAltitudeHeight);
		g_pScene->SetMaxAtitudeHeight(m_pMapInfo->ClientMaxAltitudeHeight);
	}
// 2007-03-15 by dgwoo
//	if(pMapInfo->RenderMapIndex == 0)
//	{
//		wsprintf( buf, "%04d.dat", g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
//	}
//	else
//	{
//		wsprintf( buf, "%04d.dat", pMapInfo->RenderMapIndex );
//	}
	if(m_pMapInfo->Dat == 0)
	{
		wsprintf( buf, "%04d.dat", g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
	}
	else
	{
		wsprintf(buf,"%04d.dat", m_pMapInfo->Dat);
	}

	DBGOUT("Map Channel information : MapLoading, Data[%s]\n", buf);
	g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_MAP, buf );
	if( strlen(strPath) > 0 )
	{
		if(m_pBackgroundData->SetFile(strPath,FALSE, NULL,0) == FALSE)
		{
			// 2006-12-19 by ispark, .dat 파일 없을 때 게임 종료
			g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
			util::del(m_pBackgroundData);
			return E_FAIL;
		}
	}
	else
	{
		util::del(m_pBackgroundData);
		return E_FAIL;// error
	}
	char* p;
	if(m_pMapInfo->RenderMapIndex == 0)
	{
		wsprintf(buf,"%04d",g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	}
	else
	{
		wsprintf( buf, "%04d", m_pMapInfo->RenderMapIndex );
	}
//	wsprintf(buf,"%04d",g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	pHeader = m_pBackgroundData->Find(buf);
	
	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
	if(m_pMapInfo->RenderMapIndex == 0)
	{
		wsprintf(buf,"ObjScale");
	}
	else
	{
		wsprintf( buf, "ObjScale");
	}
	// end 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
	// 2013-02-05 by bhsohn 맵툴 개선 메모리 버그 수정
//	pHeader2 = m_pBackgroundData->Find(buf);	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
	pHeader2 = NULL;	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
	// END 2013-02-05 by bhsohn 맵툴 개선 메모리 버그 수정

	if(pHeader)
	{
		p = pHeader->m_pData;
		p += 20;
		memcpy(&m_info,p,sizeof(MapInfo));
	}
	// 로딩
	p += sizeof(m_info);
	for(i=0;i<m_info.nVertexNumber;i++)
	{
		memcpy(&m_pTileVertexArray[i],p,sizeof(VERTEXINFO));
		p += sizeof(VERTEXINFO);
	}
	int cont = 0;
	for(i=0;i<m_info.nTileInfoNumber;i++)
	{
		memcpy(&m_pTileInfo[i],p,sizeof(TILEINFOCLIENT));
		p += sizeof(TILEINFOCLIENT);
		cont++;
		if(cont == m_projectInfo.sYSize)
		{
			cont = 0;
		}

		// 2005-04-06 by jschoi
//		if(m_pTileInfo[i].useWater)
		if(m_pTileInfo[i].dwWaterType == 1)
		{
			byte bCheckTemp = 0;
			int xi,zi;
			xi = i/m_projectInfo.sXSize;
			zi = i%m_projectInfo.sYSize;
			if(m_pTileVertexArray[xi*(m_projectInfo.sXSize+1) + zi].pos.y < m_projectInfo.fWaterHeight)
				bCheckTemp++;
			if(m_pTileVertexArray[(xi+1)*(m_projectInfo.sXSize+1) + zi].pos.y < m_projectInfo.fWaterHeight)
				bCheckTemp++;
			if(m_pTileVertexArray[xi*(m_projectInfo.sXSize+1) + zi+1].pos.y < m_projectInfo.fWaterHeight)
				bCheckTemp++;
			if(m_pTileVertexArray[(xi+1)*(m_projectInfo.sXSize+1) + zi+1].pos.y < m_projectInfo.fWaterHeight)
				bCheckTemp++;
			if(!bCheckTemp)
			{
				// 2005-04-06 by jschoi
//				m_pTileInfo[i].useWater = FALSE;
				m_pTileInfo[i].dwWaterType = 0;
			}
			m_bTileRender[i] = TRUE;
		}
		else
			m_bTileRender[i] = TRUE;
	}

	m_ppObjectList = new CAtumNode*[m_NUM_BLOCK_X];
	CAtumNode *pTmAtumNode = new CAtumNode[m_NUM_BLOCK_X*m_NUM_BLOCK_Z];
	for(i=0; i < m_NUM_BLOCK_X; i++)
	{
		m_ppObjectList[i] = pTmAtumNode + i*m_NUM_BLOCK_Z;
	}
	
	OBJECTINFOCLIENT oInfo;
	OBJECTSCALEINFO BaseScaleInfo;								// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
	BaseScaleInfo.vObjScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보
		
	char* p2;

	DBGOUT("m_ppObjectList = new CAtumNode[%d];\n", m_NUM_BLOCK_X*m_NUM_BLOCK_Z);

	int x, z;

	// 2005-04-12 by jschoi - Tutorial
	if(g_pTutorial->IsTutorialMode() == TRUE)
	{
		g_pTutorial->EndTutorial();
	}
	// 2005-04-12 by jschoi - Tutorial
	if(IS_TUTORIAL_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE)
	{
		g_pTutorial->StartTutorial();
	}

	// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
	if(pHeader2)
	{
		p2 = pHeader2->m_pData;
		p2 += 20;
	}	
	// end 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	

	if(m_info.nObjectNumber>0)
	{
		DBGOUT("---------- Map Object Count(%d) ----------\n", m_info.nObjectNumber);
		for(i=0;i<m_info.nObjectNumber ;i++)
		{
			memcpy(&oInfo,p, sizeof(OBJECTINFOCLIENT));

			// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
			if(pHeader2)
			{
				memcpy(&BaseScaleInfo,p2, sizeof(OBJECTSCALEINFO));
			}
			// end 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
			
			CObjectChild * pNode = new CObjectChild(oInfo, BaseScaleInfo);// 2006-07-05 by ispark
			
			if(g_pD3dApp->m_bShutDown)
			{
				return E_FAIL;
			}
			pNode->InitDeviceObjects();
			x = (int)(oInfo.vPos.x/MAP_BLOCK_SIZE);
			z = (int)(oInfo.vPos.z/MAP_BLOCK_SIZE);

			if(pNode->m_bIsEvent)	// 이벤트 오브젝트
			{
				pNode = (CObjectChild *)m_pObjectEvent->AddChild(pNode);

				if(	pNode->m_pObjectInfo &&
					pNode->m_pObjectInfo->ObjectRenderType == OBJECT_MONSTER) // 오브젝트 몬스터 타입이라면
				{
					// 오브젝트몬스터 오브젝트를 추가한다.
					oInfo.dwObjType = oInfo.dwObjectMonsterUniqueNumber + DEFAULT_OBJECT_MONSTER_OBJECT;
					pNode = new CObjectChild(oInfo, BaseScaleInfo);
					// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
					//g_pScene->m_pMonsterRender->InitData(oInfo.dwObjectMonsterUniqueNumber);
					g_pScene->m_pMonsterRender->InitData(oInfo.dwObjectMonsterUniqueNumber, _MONSTER_TYPE);
					//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
					if(pNode->m_pObjectInfo && pNode->m_pObjMesh)	// 해당 오브젝트가 존재하면
					{
						pNode->InitDeviceObjects();
						pNode = (CObjectChild *)m_pObjectMonster->AddChild(pNode);
					}
					else
					{
						util::del(pNode);
					}
				}
				else if(pNode->m_pObjectInfo &&
					pNode->m_bEventType == EVENT_TYPE_OBJ_BUILDING_NPC)
				{
					pNode = new CObjectChild(oInfo, BaseScaleInfo);
					pNode->InitDeviceObjects();
					pNode = (CObjectChild *)m_pBigObject->AddChild(pNode);					
				}
				else if(pNode->m_pObjectInfo &&
					pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CULLED &&
					IS_TUTORIAL_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE &&
					(pNode->m_nCode == TUTORIAL_GATE || pNode->m_nCode == TUTORIAL_LAND_GATE))
				{
					g_pTutorial->AddTutorialGateInfo(pNode);
				}
                //2011-02-08 by jhahn EP4 트리거  시스템
				else if(pNode->m_pObjectInfo && pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CULLED && pNode->m_bEventType == EVENT_TYPE_OBJECT_MONSTER && 
						pNode->m_sEventIndexTo == EVENT_TYPE2_TRIGGERWARP)
				{
					pNode = new CObjectChild(oInfo, BaseScaleInfo);
					pNode->InitDeviceObjects();
					pNode = (CObjectChild *)m_pBigObject->AddChild(pNode);
				} 
				//end 2011-02-08 by jhahn EP4 트리거  시스템

				else if (pNode->m_pObjectInfo == NULL)
				{
					_asm nop
				}
			}
			else
			{
				if( pNode->m_pObjectInfo && 
				   (pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG ||
					pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CULLED ||
					pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_CONTOUR ||
					pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_NO_COLLISION ||
					pNode->m_pObjectInfo->ObjectRenderType == OBJECT_BIG_EFFECT) ) // 1:BigObject //pNode->m_pObjectInfo->RadiusForClient > BIG_OBJECT_SIZE )
				{


					pNode = (CObjectChild *)m_pBigObject->AddChild(pNode);

					// 2005-04-12 by jschoi - Tutorial
//					if(	IS_TUTORIAL_MAP_INDEX(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == TRUE &&
//						pNode->m_nCode == TUTORIAL_GATE)
//					{
//						g_pTutorial->AddTutorialGateInfo(pNode);
//					}
				}
				else if(pNode->m_pObjectInfo && pNode->m_pObjectInfo->ObjectRenderType == OBJECT_WATER)
				{// 2005-02-11 by jschoi  물 오브젝트 타입 추가
					pNode = (CObjectChild *)m_pWaterObject->AddChild(pNode);
				}
				else // OBJECT_NORMAL, 일반오브젝트
				{
					pNode = (CObjectChild *)m_ppObjectList[x][z].AddChild(pNode);
				}
			}
			p += sizeof(OBJECTINFOCLIENT);

			// 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
			if(pHeader2)
			{
				p2 += sizeof(OBJECTSCALEINFO);
			}	
			// end 2012-05-02 by isshin 맵툴 개선 오브젝트 스케일 정보	
		}
	}

	// 2003.10.16 미니맵
//	m_pMinimap = new CINFImage;
//	DataHeader* pDataHeader = m_pBackgroundData->Find("map");
//	if(pDataHeader)
//		m_pMinimap->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	// 별도 주석처리
/*	if(m_pMeshMinimap)
	{
		m_pMeshMinimap->InvalidateDeviceObjects();
		m_pMeshMinimap->DeleteDeviceObjects();
		util::del(m_pMeshMinimap);
	}
	wsprintf( buf, ".\\Res-Obj\\%d.obj", g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
	CGameData gameData;
	if(gameData.SetFile( buf, FALSE, NULL, 0))
	{
*/		// 보안 코드
//		wsprintf(buf,"%04d00",g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
//		if(gameData.Find(buf) == NULL)
//		{
//			g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
//		}
/*		m_pMeshMinimap = new CSkinnedMesh();
		m_pMeshMinimap->InitDeviceObjects();
		m_pMeshMinimap->LoadMeshHierarchyFromMem(&gameData);
	}
*/
#ifdef _DEBUG
	else
	{
		DBGOUT("MiniMap File Is NULL.\n");
	}
#endif

	////////////////// Water Texture //////////////////////
	char szWater[30];
	if(GetWaterTypeAtMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
		strcpy( szWater, GetWaterTypeAtMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
		CGameData	* pWater = new CGameData;
		g_pD3dApp->LoadPath( strPath, IDS_DIRECTORY_MAP, szWater );
		if(pWater->SetFile(strPath,FALSE, NULL,0))
		{
			DataHeader* pWaterHeader = pWater->GetStartPosition();
			cont = 0;
			while(pWaterHeader)
			{
				if( FAILED( D3DXCreateTextureFromFileInMemory(g_pD3dDev, 
							(LPCVOID)pWaterHeader->m_pData, 
							pWaterHeader->m_DataSize,
							&(m_pWaterTexture[cont])))) 
				{
					hr = E_FAIL;
				}
				cont++;
				pWaterHeader = pWater->GetNext();
			}
			util::del( pWater);
		}
		else
			util::del( pWater);
	}
//	else 
//	{
//		util::del( pWater);
//		return E_FAIL;// error
//	}
	D3DUtil_InitMaterial( m_mtrl, 1.0f, 1.0f, 1.0f );
	D3DXMatrixIdentity(&m_mat);
	
	for(i = 0;i<m_projectInfo.sXSize/2;i++)
	{
		for(j = 0;j<m_projectInfo.sYSize/2;j++)
		{
			// 2005-04-06 by jschoi
//			if(m_pTileInfo[(2*i)*(m_projectInfo.sYSize) + (2*j)].useWater)
			if(m_pTileInfo[(2*i)*(m_projectInfo.sYSize) + (2*j)].dwWaterType == 1)
			{
				m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].useWater = TRUE;
			}
			// 2005-04-06 by jschoi
//			else if(m_pTileInfo[(2*i)*(m_projectInfo.sYSize) + (2*j)+1].useWater)
			else if(m_pTileInfo[(2*i)*(m_projectInfo.sYSize) + (2*j)+1].dwWaterType == 1)
			{
				m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].useWater = TRUE;
			}
			// 2005-04-06 by jschoi
//			else if(m_pTileInfo[(2*i+1)*(m_projectInfo.sYSize) + (2*j)].useWater)
			else if(m_pTileInfo[(2*i+1)*(m_projectInfo.sYSize) + (2*j)].dwWaterType == 1)
			{
				m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].useWater = TRUE;
			}
			// 2005-04-06 by jschoi
//			else if(m_pTileInfo[(2*i+1)*(m_projectInfo.sYSize) + (2*j+1)].useWater)
			else if(m_pTileInfo[(2*i+1)*(m_projectInfo.sYSize) + (2*j+1)].dwWaterType == 1)
			{
				m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].useWater = TRUE;
			}
			else
			{
				m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].useWater = FALSE;
				continue;
			}
			m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].waterHeight = m_projectInfo.fWaterHeight;			
			m_bWaterRender[i*(m_projectInfo.sYSize/2) + j].waterTexNumber = RANDI(0, WATER_TEXTURE_COUNT - 1);
		}
	}
// 2008-02-28 by dgwoo 보조라이트 셋팅이 비스타 검게 나오는 문제 발생.(게임 상에서 사용하지 않음)
//	// 보조라이트 셋팅 (light.ltm)
//	memset(m_lightSupport, 0x00, sizeof(D3DLIGHT9)*4);
//	pHeader = m_pBackgroundData->Find("light");
//	if(pHeader)
//	{
//		int nLightCount;
//		p = pHeader->m_pData + 20;
//		memcpy(&nLightCount,p,sizeof(nLightCount));
//		p += sizeof(int);
//		for(i=4;i<8;i++)
//		{
//			if(nLightCount > 0)
//			{
//				memcpy(&m_lightSupport[i-4],p,sizeof(D3DLIGHT9));
//				g_pD3dDev->SetLight( i, &m_lightSupport[i-4] );
//				g_pD3dDev->LightEnable( i, TRUE );
//				p += sizeof(D3DLIGHT9);
//				nLightCount--;
//				DBGOUT("Light : SetLight(%d, TRUE)\n", i);
//			}
//			else
//			{
//				g_pD3dDev->LightEnable( i, FALSE );
//				DBGOUT("Light : SetLight(%d, FALSE)\n", i);
//			}
//		}
//	}
	// 맵 이펙트 셋팅 (effect.efc)
	pHeader = m_pBackgroundData->Find("effect");
	if(pHeader)
	{
		map<INT, EFFECTINFOCLIENT *>::iterator itEff = m_mapEffectList.end();
		int nEffCont,nTemp;
		int nRegionCont1 = 0;
		int nRegionCont2 = 0;
		p = pHeader->m_pData + 20;
		memcpy(&nEffCont,p,sizeof(nEffCont));
		p += sizeof(int);
		for(i=0;i<nEffCont;i++)
		{
			EFFECTINFOCLIENT * eff = new EFFECTINFOCLIENT;
			memset(eff,0x00,sizeof(EFFECTINFOCLIENT));
			memcpy(eff,p,sizeof(EFFECTINFOCLIENT));
			eff->fCheckTime = 0.0f;
			if(eff->bRegion)
			{
				while(1)
				{
					nTemp = 99999 * (100) + nRegionCont1;
					itEff = m_mapEffectList.find(nTemp);
					if(itEff == m_mapEffectList.end())
					{
						m_mapEffectList[nTemp] = eff;
						break;
					}
				}
				nRegionCont1++;
			}
			else
			{
				int tempx = (int)(eff->vPos.x/TILE_SIZE);
				int tempz = (int)(eff->vPos.z/TILE_SIZE);
				if(tempx >= 0 && tempx < m_projectInfo.sXSize && tempz >= 0 && tempz < m_projectInfo.sYSize)
				{
					nRegionCont2 = 0;
					while(1)
					{
						nTemp = (tempx*m_projectInfo.sYSize + tempz) * (100) + nRegionCont2;
						itEff = m_mapEffectList.find(nTemp);
						if(itEff == m_mapEffectList.end())
						{
							m_mapEffectList[nTemp] = eff;
							break;
						}
						nRegionCont2++;
					}
				}
			}
			p += sizeof(EFFECTINFOCLIENT);
		}
	}
	// 튜토리얼 맵용 몬스터 소환 정보
	pHeader = m_pBackgroundData->Find("mon");
	if(pHeader)
	{
		int nMonCount;
		// 아래 임시
		p = pHeader->m_pData + 20 + m_projectInfo.sXSize*m_projectInfo.sYSize*sizeof(EVENT_DATA);
		memcpy(&nMonCount,p,sizeof(nMonCount));
		p += sizeof(int);
		for(i=0;i<nMonCount;i++)
		{
			MONSTERINFO * mon = new MONSTERINFO;
			memset(mon,0x00,sizeof(MONSTERINFO));
			memcpy(mon,p,sizeof(MONSTERINFO));
			m_vecMonsterResList.push_back(mon);
			p += sizeof(MONSTERINFO);
		}
	}
//	if( g_pD3dApp->m_pETCRender )
//		g_pD3dApp->m_pETCRender->InitDeviceObjects();
	// 256 사이즈의 맵일 경우 쿼드 트리로 로딩한다
//	if ( m_projectInfo.sXSize == MAP_BLOCK_MAX_NUM && m_pTextureData)
	if( IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) && 
		m_pTextureData)
	{
		m_pQuad = new CQuadGround( MAP_BLOCK_MAX_NUM );
		m_pQuad->SubDivide();		
		// 2010. 03. 05 by jskim 맵 로딩 중 윈도우 화면 전환시 지형 사라지는 버그 수정
		//m_pQuad->InitDeviceObjects();
		//end 2010. 03. 05 by jskim 맵 로딩 중 윈도우 화면 전환시 지형 사라지는 버그 수정
	}
	
	return CAtumNode::InitDeviceObjects();
}

HRESULT CBackground::RestoreDeviceObjects()
{
	FLOG("CBackground::RestoreDeviceObjects()");
	/*	g_pD3dDev->BeginStateBlock();
	g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  FALSE );
	g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   TRUE );
	g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
	g_pD3dDev->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );
	g_pD3dDev->EndStateBlock( &m_dwStateBlock );
	*/
	m_pObjectEvent->RestoreDeviceObjects();
	m_pBigObject->RestoreDeviceObjects();
	m_pWaterObject->RestoreDeviceObjects(); // 2005-02-11 by jschoi  물 오브젝트 타입 추가
	m_pObjectMonster->RestoreDeviceObjects();
	int x = (int)((m_projectInfo.sXSize * TILE_SIZE) / MAP_BLOCK_SIZE);
	int z = (int)((m_projectInfo.sYSize * TILE_SIZE) / MAP_BLOCK_SIZE);
	for (int i = 0; i<x; i++)
	{
		for (int j = 0; j<z; j++)
		{
			m_ppObjectList[i][j].RestoreDeviceObjects();
		}
	}

	if (m_bMiniMap)
	{
		if (m_pMiniBigMapImage)
			m_pMiniBigMapImage->RestoreDeviceObjects();
		if (m_pMiniBigMapCityImage)
			m_pMiniBigMapCityImage->RestoreDeviceObjects();
	}

	// 2005-09-12 by ispark 별도 처리
	/*	if(m_pMeshMinimap)
	{
	m_pMeshMinimap->RestoreDeviceObjects();
	}
	*/
	int nCheckSizeX;
	int nCheckSizeY;
	GROUNDVERTEX* v1;
	for (nCheckSizeX = 0; nCheckSizeX < m_projectInfo.sYSize / 2; nCheckSizeX++)
	{
		if (FAILED(g_pD3dDev->CreateVertexBuffer(2 * m_projectInfo.sXSize * sizeof(GROUNDVERTEX)
			, 0
			, D3DFVF_GROUNDVERTEX
			, D3DPOOL_MANAGED
			, &m_pVBWater[nCheckSizeX]
			, NULL)))
		{
			DBGOUT("ERROR:CBackground::RestoreDeviceObjects() g_pD3dDev->CreateVertexBuffer(2*%d*%d...)", m_projectInfo.sXSize, sizeof(GROUNDVERTEX));
			return E_FAIL;
		}
	}
	float fWaterHeight;
	fWaterHeight = m_projectInfo.fWaterHeight;
	for (nCheckSizeX = 0; nCheckSizeX < m_projectInfo.sYSize / 2; nCheckSizeX++)
	{
		m_pVBWater[nCheckSizeX]->Lock(0, 0, (void**)&v1, 0);

		for (nCheckSizeY = 0; nCheckSizeY < m_projectInfo.sXSize / 2; nCheckSizeY++)
		{
			v1[nCheckSizeY * 4].p =
				D3DXVECTOR3(nCheckSizeX * TILE_SIZE * 2, fWaterHeight, nCheckSizeY * TILE_SIZE * 2);
			v1[nCheckSizeY * 4].tu = 0.0f;
			v1[nCheckSizeY * 4].tv = 0.0f;
			v1[nCheckSizeY * 4].n = D3DXVECTOR3(0, 1, 0);

			v1[nCheckSizeY * 4 + 1].p =
				D3DXVECTOR3(nCheckSizeX * TILE_SIZE * 2, fWaterHeight, (nCheckSizeY + 1) * TILE_SIZE * 2);
			v1[nCheckSizeY * 4 + 1].tu = 0.0f;
			v1[nCheckSizeY * 4 + 1].tv = 1.0f;
			v1[nCheckSizeY * 4 + 1].n = D3DXVECTOR3(0, 1, 0);

			v1[nCheckSizeY * 4 + 2].p =
				D3DXVECTOR3((nCheckSizeX + 1) * TILE_SIZE * 2, fWaterHeight, (nCheckSizeY)* TILE_SIZE * 2);
			v1[nCheckSizeY * 4 + 2].tu = 1.0f;
			v1[nCheckSizeY * 4 + 2].tv = 0.0f;
			v1[nCheckSizeY * 4 + 2].n = D3DXVECTOR3(0, 1, 0);

			v1[nCheckSizeY * 4 + 3].p =
				D3DXVECTOR3((nCheckSizeX + 1) * TILE_SIZE * 2, fWaterHeight, (nCheckSizeY + 1) * TILE_SIZE * 2);
			v1[nCheckSizeY * 4 + 3].tu = 1.0f;
			v1[nCheckSizeY * 4 + 3].tv = 1.0f;
			v1[nCheckSizeY * 4 + 3].n = D3DXVECTOR3(0, 1, 0);
		}
		m_pVBWater[nCheckSizeX]->Unlock();
	}

	// 2008. 12. 11 by ckPark 물렌더링
	if (NULL == m_pToRenderWaterVB)
	{
		if (FAILED(g_pD3dDev->CreateVertexBuffer(4 *
			m_projectInfo.sXSize *
			m_projectInfo.sYSize *
			sizeof(WATERBUMPVERTEX), // GROUNDVERTEX* -> WATERBUMPVERTEX
			0,
			D3DFVF_WATERBUMPVERTEX,	// D3DFVF_GROUNDVERTEX -> D3DFVF_WATERBUMPVERTEX
			D3DPOOL_MANAGED,
			&m_pToRenderWaterVB,
			NULL)))
		{
			SAFE_RELEASE(m_pToRenderWaterVB);
			return E_FAIL;
		}
	}
	// end 2008. 12. 11 by ckPark 물렌더링


	//	if(g_pD3dApp->m_pETCRender)
	//		g_pD3dApp->m_pETCRender->RestoreDeviceObjects();

	// Map's Vertex Buffer Create and Data Loading
	if (m_pQuad)
	{
		// Quad Map Vertex Loading
		// 2010. 03. 05 by jskim 맵 로딩 중 윈도우 화면 전환시 지형 사라지는 버그 수정
		m_pQuad->InitDeviceObjects();
		//end 2010. 03. 05 by jskim 맵 로딩 중 윈도우 화면 전환시 지형 사라지는 버그 수정
		m_pQuad->RestoreDeviceObjects();
		m_pQuad->GetMinMax(0.0f, 0.0f);
		DBGOUT("m_pQuad->RestoreDeviceObjects()\n");
	}
	// Normal Map Vertex Loading
	//	GROUNDVERTEX *v;
	//	for ( nCheckSizeX = 0;nCheckSizeX < m_projectInfo.sXSize;nCheckSizeX++ )
	//	{
	//		if ( FAILED( g_pD3dDev->CreateVertexBuffer( 4 * m_projectInfo.sXSize * sizeof( GROUNDVERTEX ), 
	//			0, 
	//			D3DFVF_GROUNDVERTEX, 
	//			D3DPOOL_MANAGED, 
	//			&m_pVBRender[nCheckSizeX] ) ) )
	//		{
	//			DBGOUT("ERROR:CBackground::RestoreDeviceObjects() g_pD3dDev->CreateVertexBuffer(4*%d*%d...)",m_projectInfo.sXSize,sizeof( GROUNDVERTEX ));
	//			return E_FAIL;
	//		}
	//	}
	//	for ( nCheckSizeX=0;nCheckSizeX < m_projectInfo.sYSize;nCheckSizeX++ )
	//	{
	//		m_pVBRender[nCheckSizeX]->Lock( 0, 0, (BYTE**)&v, 0 );
	//
	//		for ( nCheckSizeY = 0;nCheckSizeY < m_projectInfo.sXSize;nCheckSizeY++ )
	//		{
	//			int nVNum;
	//			nVNum = nCheckSizeY * 4;
	//			v[nVNum].p = m_pTileVertexArray[(nCheckSizeX) * (m_projectInfo.sYSize + 1) + (nCheckSizeY)].pos;
	//			v[nVNum].tu = 0.0f;
	//			v[nVNum].tv = 0.0f;
	//			v[nVNum].n = m_pTileVertexArray[(nCheckSizeX) * (m_projectInfo.sYSize + 1) + (nCheckSizeY)].nor;
	//
	//			v[nVNum + 1].p = 
	//				m_pTileVertexArray[(nCheckSizeX) * (m_projectInfo.sYSize + 1) + (nCheckSizeY + 1)].pos;
	//			v[nVNum + 1].tu = 0.0f;
	//			v[nVNum + 1].tv = 1.0f;
	//			v[nVNum + 1].n = 
	//				m_pTileVertexArray[(nCheckSizeX) * (m_projectInfo.sYSize + 1) + (nCheckSizeY + 1)].nor;
	//
	//			v[nVNum + 2].p = 
	//				m_pTileVertexArray[(nCheckSizeX + 1) * (m_projectInfo.sYSize + 1) + (nCheckSizeY)].pos;
	//			v[nVNum + 2].tu = 1.0f;
	//			v[nVNum + 2].tv = 0.0f;
	//			v[nVNum + 2].n = 
	//				m_pTileVertexArray[(nCheckSizeX + 1) * (m_projectInfo.sYSize + 1) + (nCheckSizeY)].nor;
	//
	//			v[nVNum + 3].p = 
	//				m_pTileVertexArray[(nCheckSizeX + 1) * (m_projectInfo.sYSize + 1) + (nCheckSizeY + 1)].pos;
	//			v[nVNum + 3].tu = 1.0f;
	//			v[nVNum + 3].tv = 1.0f;
	//			v[nVNum + 3].n = 
	//				m_pTileVertexArray[(nCheckSizeX +1 ) * (m_projectInfo.sYSize + 1) + (nCheckSizeY + 1)].nor;
	//
	//		}
	//		m_pVBRender[nCheckSizeX]->Unlock();
	//	}
	/*
	for(int i=0;i<4;i++)
	{
	if(m_lightSupport[i].Type > 0 )
	{
	g_pD3dDev->SetLight(i+4, &m_lightSupport[i]);
	g_pD3dDev->LightEnable( i+4, TRUE );
	DBGOUT("라이트 디버깅 : SetLight(%d, TRUE)\n", i+4);
	}
	else
	{
	g_pD3dDev->LightEnable( i+4, FALSE );
	DBGOUT("라이트 디버깅 : SetLight(%d, FALSE)\n", i+4);
	}
	}*/

	return CAtumNode::RestoreDeviceObjects();
}

HRESULT CBackground::InvalidateDeviceObjects()
{
	FLOG( "CBackground::InvalidateDeviceObjects()" );
	if(m_dwStateBlock)
	{
//		g_pD3dDev->DeleteStateBlock(m_dwStateBlock);
		m_dwStateBlock = 0;
	}
	m_pObjectEvent->InvalidateDeviceObjects();
	m_pBigObject->InvalidateDeviceObjects();
	m_pWaterObject->InvalidateDeviceObjects();// 2005-02-11 by jschoi  물 오브젝트 타입 추가
	m_pObjectMonster->InvalidateDeviceObjects();
	int x = (int)((m_projectInfo.sXSize * TILE_SIZE)/MAP_BLOCK_SIZE);
	int z = (int)((m_projectInfo.sYSize * TILE_SIZE)/MAP_BLOCK_SIZE);
	int i;
	for( i=0; i<x; i++ )
	{
		for( int j=0; j<z; j++ )
		{
			m_ppObjectList[i][j].InvalidateDeviceObjects();
		}
	}
//	for(i=0; i<m_projectInfo.sYSize; i++)
//	{
//		SAFE_RELEASE( m_pVBRender[i] );
//	}
	for(i=0; i<m_projectInfo.sYSize/2; i++)
		SAFE_RELEASE( m_pVBWater[i] );


	// 2008. 12. 11 by ckPark 물렌더링
	SAFE_RELEASE(m_pToRenderWaterVB);
	// end 2008. 12. 11 by ckPark 물렌더링



	// 2005-09-12 by ispark 별도 처리
/*	if(m_pMeshMinimap)
	{
		m_pMeshMinimap->InvalidateDeviceObjects();
	}
*/
	if(m_pMiniBigMapImage)
	{
		m_pMiniBigMapImage->InvalidateDeviceObjects();
		if(m_pMiniBigMapCityImage)
			m_pMiniBigMapCityImage->InvalidateDeviceObjects();
	}
	
//	if(g_pD3dApp->m_pETCRender)
//		g_pD3dApp->m_pETCRender->InvalidateDeviceObjects();

	
 

	if ( m_pQuad )
	{
		m_pQuad->InvalidateDeviceObjects();
	}

	return CAtumNode::InvalidateDeviceObjects();
}

HRESULT CBackground::DeleteDeviceObjects()
{
	FLOG( "CBackground::DeleteDeviceObjects()" );
	m_pObjectEvent->DeleteDeviceObjects();
	util::del(m_pObjectEvent);
	m_pBigObject->DeleteDeviceObjects();
	util::del(m_pBigObject);
	m_pWaterObject->DeleteDeviceObjects();// 2005-02-11 by jschoi  물 오브젝트 타입 추가
	util::del(m_pWaterObject);
	m_pObjectMonster->DeleteDeviceObjects();
	util::del(m_pObjectMonster);
	int x = (int)((m_projectInfo.sXSize * TILE_SIZE)/MAP_BLOCK_SIZE);
	int z = (int)((m_projectInfo.sYSize * TILE_SIZE)/MAP_BLOCK_SIZE);
	int i;
	for( i=0; i<x; i++ )
	{
		for( int j=0; j<z; j++ )
		{
			m_ppObjectList[i][j].DeleteDeviceObjects();
		}
	}
	util::del_array(m_ppObjectList[0]);
	util::del_array(m_ppObjectList);
//	if (m_ppObjectList)
//	{
//		util::del_array(m_ppObjectList[0]);
//		util::del_array(m_ppObjectList);
//	}
	for(i=0; i<WATER_TEXTURE_COUNT; i++)
		SAFE_RELEASE( m_pWaterTexture[i] );
	util::del(m_pBackgroundData);
	util::del(m_pTextureData);

	// 2005-09-12 by ispark 별도 처리
/*	if(m_pMeshMinimap)
	{
		m_pMeshMinimap->DeleteDeviceObjects();
		util::del(m_pMeshMinimap);
	}
*/
	if(m_pMiniBigMapImage)
	{
		m_pMiniBigMapImage->DeleteDeviceObjects();
		util::del(m_pMiniBigMapImage);
		if(m_pMiniBigMapCityImage)
		{
			m_pMiniBigMapCityImage->DeleteDeviceObjects();
			util::del(m_pMiniBigMapCityImage);
		}
	}
//	if(g_pD3dApp->m_pETCRender)
//		g_pD3dApp->m_pETCRender->DeleteDeviceObjects();

//	if(m_pBigObject)
//		util::del(m_pBigObject);
//	if(m_pObjectEvent)
//		util::del(m_pObjectEvent);

	// 2008. 12. 11 by ckPark 물렌더링
	SAFE_RELEASE(m_pToRenderWaterVB);
	// end 2008. 12. 11 by ckPark 물렌더링

	if ( m_pQuad )
	{
		m_pQuad->DeleteDeviceObjects();
		util::del( m_pQuad );
	}

	SAFE_RELEASE(m_pDetailMap);
	return CAtumNode::DeleteDeviceObjects();
}

void CBackground::Render()
{
	FLOG( "CBackground::Render()" );
	// 2007-07-03 by dgwoo if 위로 뺌 프리스카 맵에서 광원이 아래에 위치하는 버그.
	g_pD3dDev->SetLight( 0, &g_pScene->m_light0 );
	g_pD3dDev->LightEnable( 0, TRUE );


	if ( m_pQuad )
	{
//		g_pD3dDev->ApplyStateBlock( m_dwStateBlock );
//		g_pD3dDev->SetLight( 0, &g_pScene->m_light0 );
//		g_pD3dDev->LightEnable( 0, TRUE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );
		g_pD3dDev->SetRenderState( D3DRS_ALPHABLENDENABLE,  FALSE );
		g_pD3dDev->SetRenderState( D3DRS_ZENABLE,   TRUE );
		g_pD3dDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	    g_pD3dDev->SetRenderState( D3DRS_FOGENABLE, IsFogEnableMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) );
		g_pD3dDev->SetFVF( D3DFVF_FOGVERTEX );
		g_pD3dDev->SetRenderState( D3DRS_FOGCOLOR,  g_pScene->m_dwFogColor );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

//		// 렌더 상태 셋팅
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);



		// 2009. 01. 06 by ckPark 지형 최적화
		
		D3DXMATRIX mat;
		D3DXMatrixIdentity( &mat );
		g_pD3dDev->SetTransform( D3DTS_WORLD, &mat );
		g_pD3dDev->SetFVF( D3DFVF_GROUNDVERTEX );
		
		
		D3DXMATRIXA16	matWorld;
		D3DXMatrixIdentity( &matWorld );
		
		matWorld._11 = 5;
		matWorld._22 = 5;
		
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
		g_pD3dDev->SetTransform( D3DTS_TEXTURE1, &matWorld );
		
		g_pD3dDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		g_pD3dDev->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		g_pD3dDev->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		g_pD3dDev->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0번 텍스처 : 0번 텍스처 인덱스 사용
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );		// 1번 텍스처 : 0번 텍스처 인덱스 사용
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pD3dDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		
		// end 2009. 01. 06 by ckPark 지형 최적화




		m_pQuad->Render();	

//		// 렌더 상태 복원
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
//		g_pD3dDev->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
		
	}

}

void CBackground::Tick(float fElapsedTime)
{
	FLOG( "CBackground::Tick(float fElapsedTime)" );
	if(m_fCheckWaterTexAniTime >= 0.0f)
		m_fCheckWaterTexAniTime -= fElapsedTime;
	if(m_fCheckWaterTexAniTime < 0.0f)
	{
//		m_fCheckWaterTexAniTime = GetWaterTick(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex );
		// 2005-06-29 by ispark
		m_fCheckWaterTexAniTime = 0.1f;
		m_bWaterTexCont++;
		if(m_bWaterTexCont >= WATER_TEXTURE_COUNT)
			m_bWaterTexCont = 0;
	}
//	int nEffectCont = 0;
//	int nTemp;
//	map<INT,EFFECTINFOCLIENT*>::iterator itEff = NULL;
//	while(1)
//	{
//		nTemp = 99999*(100) + nEffectCont;
//		itEff = m_mapEffectList.find(nTemp);
//		if(itEff != m_mapEffectList.end())
//		{
//			EffectTick(itEff->second);
//			nEffectCont++;
//		}
//		else
//			break;
//	}

	
	if(g_pTutorial->IsTutorialMode() == FALSE || g_pTutorial->IsUseShuttleTick() == TRUE)
	{
		vectorCObjectChildPtr::iterator itObj(g_pScene->m_vectorRangeObjectPtrList.begin());
		while(itObj != g_pScene->m_vectorRangeObjectPtrList.end())
		{
			(*itObj)->Tick();
			itObj++;
		}
	}
	
// 2004-06-04, cmkwon, Object Render 수정
//	// Object
//	int nDeleteNumber = 0;
//	CMapObjectIterator itObj = g_pD3dApp->m_pScene->m_mapObjectList.begin();
//	while(itObj != g_pD3dApp->m_pScene->m_mapObjectList.end())
//	{
//		itObj->second->Tick();
//		if(!itObj->second->m_bCheckRenderDist)
//			nDeleteNumber = itObj->second->m_nObjectCont;
//		itObj++;
//	}
//	// 렌더링 영역에서 제외되는 오브젝트가 있을때
//	if(nDeleteNumber)
//	{
//		g_pD3dApp->m_pScene->DeleteObjectList(nDeleteNumber);
//	}

//	if(m_pBigObject)
//	{
//		m_pBigObject->Tick();
//	}
	
	if(m_pObjectEvent)
	{
		m_pObjectEvent->Tick();
//		CObjectChild * pObj = (CObjectChild *)m_pObjectEvent->m_pChild;
//		while(pObj)
//		{
//			pObj->Tick();
//			pObj = (CObjectChild *)pObj->m_pNext;
//		}
	}
//	D3DXVECTOR3 vView = g_pD3dApp->m_pCamera->GetEyePt();
//	D3DXVECTOR3 vDir = g_pD3dApp->m_pCamera->GetViewDir();
//	D3DXVec3Normalize(&vDir,&vDir);
//	vView += MAP_BLOCK_SIZE*(g_pSOption->sObjectRender-2)*vDir;
//	int i,j;
//	int tempx, tempz;
//	tempx = (int)(vView.x / MAP_BLOCK_SIZE);
//	tempz = (int)(vView.z / MAP_BLOCK_SIZE);
//	int cont1 = 0;
//	int cont2 = 0;
//
//
//	for(i = tempx-g_pSOption->sObjectRender;i < tempx+g_pSOption->sObjectRender+1; i++)
//					{
//		if(cont1 < g_pSOption->sObjectRender/2)
//			cont2 = cont1;
//		else if(cont1 < g_pSOption->sObjectRender*2 + 1 - g_pSOption->sObjectRender/2)
//			cont2 = g_pSOption->sObjectRender/2;
//		else
//			cont2 = g_pSOption->sObjectRender*2 - cont1;
//		
//		if(i>=0 && i<m_projectInfo.sXSize)
//		{
//			for(j = tempz-(g_pSOption->sObjectRender/2+cont2);j < tempz+(g_pSOption->sObjectRender/2+cont2)+1; j++)
//			{
//				if(j>=0 && j<m_projectInfo.sYSize)
//				{
//					int k = (i*m_projectInfo.sYSize+j);
////					CObjectChild * pObject = (CObjectChild *)m_pObject[k].m_pChild;
////					while(pObject)
////					{
//						nEffectCont = 0;
//						while(1)
//						{
//							nTemp = k*(100) + nEffectCont;
//							itEff = m_mapEffectList.find(nTemp);
//							if(itEff != m_mapEffectList.end())
//							{
//								EffectTick(itEff->second);
//								nEffectCont++;
//					}
//							else
//								break;
//						}
////						pObject = (CObjectChild *)pObject->m_pNext;
////					}
//				}
//			}
//		}
//		cont1++;
//	}
//	m_pObjectEvent->Tick();
}

void CBackground::EffectTick(EFFECTINFOCLIENT *info)
{
	FLOG( "CBackground::EffectTick(EFFECTINFOCLIENT *info)" );
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	if(info->bRegion)
	{// 전체 이펙트
		if(info->bLoop)
		{// 루핑인가(한번만 실행)
			if(info->fCheckTime > 0.0f)
				info->fCheckTime -= fElapsedTime;
			if(info->fCheckTime <= 0.0f && info->fCheckTime > -1000.0f)
			{// 생성
				info->fCheckTime = -1000.0f;
				CAppEffectData * pEffect = new CAppEffectData(info->nType,info->vPos);
				pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEffect);
			}
		}
		else
		{// 순간인가(재발동시간마다 생성)
			if(info->fCheckTime > 0.0f)
				info->fCheckTime -= fElapsedTime;
			if(info->fCheckTime <= 0.0f)
			{// 생성
				info->fCheckTime = info->fReTime;
				CAppEffectData * pEffect = new CAppEffectData(info->nType,info->vPos);
				pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEffect);
			}
		}
	}
	else
	{// 부분 이펙트
		int tempx = (int)(info->vPos.x/TILE_SIZE);
		int tempz = (int)(info->vPos.z/TILE_SIZE);
		if(tempx >= 0 && tempx < m_projectInfo.sXSize && tempz >= 0 && tempz < m_projectInfo.sYSize)
		{// 현재 렌더링 영역인가
			if(info->bLoop)
			{// 루핑인가(한번만 실행)
				if(info->fCheckTime > 0.0f)
					info->fCheckTime -= fElapsedTime;
				if(info->fCheckTime <= 0.0f && info->fCheckTime > -1000.0f)
				{// 생성
					info->fCheckTime = -1000.0f;
					CAppEffectData * pEffect = new CAppEffectData(info->nType,info->vPos);
					pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEffect);
				}
			}
			else
			{// 순간인가(재발동시간마다 생성)
				if(info->fCheckTime > 0.0f)
					info->fCheckTime -= fElapsedTime;
				if(info->fCheckTime <= 0.0f)
				{// 생성
					info->fCheckTime = info->fReTime;
					CAppEffectData * pEffect = new CAppEffectData(info->nType,info->vPos);
					pEffect = (CAppEffectData *)g_pD3dApp->m_pEffectList->AddChild(pEffect);
				}
			}
		}
	}
}

BOOL CBackground::IntersectTriangle( const D3DXVECTOR3& orig
									, const D3DXVECTOR3& dir
									, D3DXVECTOR3& v0
									, D3DXVECTOR3& v1
									, D3DXVECTOR3& v2
									, FLOAT* t)//, FLOAT* u, FLOAT* v )
{
	FLOG( "CBackground::IntersectTriangle( const D3DXVECTOR3& orig,const D3DXVECTOR3& dir, D3DXVECTOR3& v0,D3DXVECTOR3& v1, D3DXVECTOR3& v2,FLOAT* t)" );
	float u,v;
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    u = D3DXVec3Dot( &tvec, &pvec );
    if( u < 0.0f || u > det )
        return FALSE;

	// Prepare to test V parameter
	D3DXVECTOR3 qvec;
	D3DXVec3Cross( &qvec, &tvec, &edge1 );

	// Calculate V parameter and test bounds
    v = D3DXVec3Dot( &dir, &qvec );
    if( v < 0.0f || u + v > det )
        return FALSE;

	// Calculate t, scale parameters, ray intersects triangle
	*t = D3DXVec3Dot( &edge2, &qvec );
	FLOAT fInvDet = 1.0f / det;
	*t *= fInvDet;
//	*u *= fInvDet;
//	*v *= fInvDet;

    return TRUE;
}

float CBackground::CheckHeightMap(D3DXVECTOR3 vPos)
{
	FLOG( "CBackground::CheckHeightMap(D3DXVECTOR3 vPos)" );
	// 지형과 충돌 검사
	if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
	{
		// 2006-06-07 by ispark, 동굴맵에서는 맵 체크 안함
		return 0.0f;
	}
//	FLOAT fBary1, fBary2;
	FLOAT fDist;
	int i,j;
	i = (int)(vPos.x/TILE_SIZE);
	j = (int)(vPos.z/TILE_SIZE);
	D3DXVECTOR3 v,dir;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < m_projectInfo.sXSize && j >= 0 && j < m_projectInfo.sYSize)
	{
		BOOL bIsWater = g_pD3dApp->m_pShuttleChild->CheckIsWater(vPos);
		if(IntersectTriangle(vPos,dir,m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+j].pos,
			m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+(j+1)].pos,
			m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			v = vPos + dir*fDist;
			if(bIsWater)
			{
				if(v.y > m_projectInfo.fWaterHeight)
				{
					return v.y;
				}
				else
				{
					return m_projectInfo.fWaterHeight;
				}
			}
			else
			{
				return v.y;
			}
		}
		else if(IntersectTriangle(vPos,dir,m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+j+1].pos,
			m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+(j)].pos,
			m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			v = vPos + dir*fDist;
			if(bIsWater)
			{
				if(v.y > m_projectInfo.fWaterHeight)
				{
					return v.y;
				}
				else
				{
					return m_projectInfo.fWaterHeight;
				}
			}
			else
			{
				return v.y;
			}
		}
	}
	else
	{
		// 좌표가 맵체크에서 벗어났을때 터지게 만든다.
		return 1000.0f;
	}
	// 좌표가 맵체크에서 벗어났을때 터지게 만든다.
	return 1000.0f;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CBackground::CheckCollMap(D3DXVECTOR3 vPos, float *fHeight, D3DXVECTOR3 * vNor)
/// \brief		맵 충돌 높이와 노말값을 가져온다.
/// \author		ispark
/// \date		2006-05-06 ~ 2006-05-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBackground::CheckCollMap(D3DXVECTOR3 vPos, float *fHeight, D3DXVECTOR3 * vNor)
{
	FLOG( "CBackground::CheckCollMap(D3DXVECTOR3 vPos, float *fHeight, D3DXVECTOR3 * vNor)" );
	// 지형과 충돌 검사
	if(IsTileMapRenderEnable(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) == FALSE)
	{
		// 2006-06-07 by ispark, 동굴맵에서는 맵 체크 안함
		*fHeight = 0.0f;
		return;
	}
//	FLOAT fBary1, fBary2;
	FLOAT fDist;
	int i,j;
	i = (int)(vPos.x/TILE_SIZE);
	j = (int)(vPos.z/TILE_SIZE);
	D3DXVECTOR3 v,dir;
	dir = D3DXVECTOR3(0,-1,0);
	if(i >= 0 && i < m_projectInfo.sXSize && j >= 0 && j < m_projectInfo.sYSize)
	{
		BOOL bIsWater = g_pD3dApp->m_pShuttleChild->CheckIsWater(vPos);
		if(IntersectTriangle(vPos,dir,m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+j].pos,
			m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+(j+1)].pos,
			m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+(j)].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			v = vPos + dir*fDist;
			
			// 법선을 구하자.
			D3DXVECTOR3 vTempNormal;
			D3DXVECTOR3 vVec1,vVec2,vVec3;
			D3DXVECTOR3 vCross1,vCross2;
			vVec1 = m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+j].pos;
			vVec2 = m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+(j+1)].pos;
			vVec3 = m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+(j)].pos;
			vCross1 = vVec2 - vVec1;
			vCross2 =  vVec3 - vVec1;
			D3DXVec3Cross(&vTempNormal,&vCross1,&vCross2);
			D3DXVec3Normalize(vNor,&vTempNormal);
//			DBGOUT("1.지형 노말 %f %f %f(%f)\n", (*vNor).x, (*vNor).y, (*vNor).z, fDist);

			if(bIsWater)
			{
				if(v.y > m_projectInfo.fWaterHeight)
				{
					*fHeight = v.y;
					return;
				}
				else
				{
					*fHeight = m_projectInfo.fWaterHeight;
					return;
				}
			}
			else
			{
				*fHeight = v.y;
				return;
			}
		}
		else if(IntersectTriangle(vPos,dir,m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+j+1].pos,
			m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+(j)].pos,
			m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+j+1].pos,
			&fDist))//, &fBary1, &fBary2 ))
		{
			v = vPos + dir*fDist;

			// 법선을 구하자.
			D3DXVECTOR3 vTempNormal;
			D3DXVECTOR3 vVec1,vVec2,vVec3;
			D3DXVECTOR3 vCross1,vCross2;
			vVec1 = m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+(j)].pos;
			vVec2 = m_pTileVertexArray[i*(m_projectInfo.sYSize+1)+j+1].pos;
			vVec3 = m_pTileVertexArray[(i+1)*(m_projectInfo.sYSize+1)+j+1].pos;
			vCross1 = vVec2 - vVec1;
			vCross2 =  vVec3 - vVec1;
			D3DXVec3Cross(&vTempNormal,&vCross1,&vCross2);
			D3DXVec3Normalize(vNor,&vTempNormal);
//			DBGOUT("2.지형 노말 %f %f %f(%f)\n", (*vNor).x, (*vNor).y, (*vNor).z, fDist);

			if(bIsWater)
			{
				if(v.y > m_projectInfo.fWaterHeight)
				{
					*fHeight = v.y;
					return;
				}
				else
				{
					*fHeight = m_projectInfo.fWaterHeight;
					return;
				}
			}
			else
			{
				*fHeight = v.y;
				return;
			}
		}
	}
	else
	{
		// 좌표가 맵체크에서 벗어났을때 터지게 만든다.
		*fHeight = 1000.0f;
		return;
	}
	// 좌표가 맵체크에서 벗어났을때 터지게 만든다.
	*fHeight = 1000.0f;
	return;
}
