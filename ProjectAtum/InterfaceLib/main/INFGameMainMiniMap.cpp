// INFGameMainMiniMap.cpp: implementation of the CINFGameMainMiniMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFGameMainMiniMap.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "MonsterData.h"
#include "EnemyData.h"
#include "SceneData.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"
#include "SkinnedMesh.h"
#include "Background.h"
#include "Camera.h"
#include "ObjectChild.h"
#include "ChatMoveData.h"
#include "ObjRender.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "AtumDatabase.h"
#include "ClientParty.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "ItemInfo.h"
#include "Interface.h"
#include "ItemData.h"
#include "INFGameArena.h"
#include "INFArenaTeamInfo.h"
#include "ShuttleChild.h"	// 2008-09-26 by bhsohn 신규 인첸트 처리
// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
#include "INFImageBtn.h"	
#include "INFGameMain.h"
#include "INFImageEx.h"
#include "INFGroupManager.h"
#include "INFGroupImage.h"
// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

#include "INFCommunity.h"// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
#include "INFCommunityGuild.h"// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시

// 2015-11-02 PanKJ
#include "FreeWar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MINI_MARK_HEIGHT			40
#define BACK_START_X				(g_pD3dApp->GetBackBufferDesc().Width-140)
#define BACK_START_Y				(18 + 16)
#define BIGMAP_START_X				(g_pD3dApp->GetBackBufferDesc().Width / 2) - 256
#define BIGMAP_START_Y				(g_pD3dApp->GetBackBufferDesc().Height / 2) - 256
#define SMALLMAP_START_X			(g_pD3dApp->GetBackBufferDesc().Width - 170)
#define SMALLMAP_START_Y			193		// 2012-03-29 by mspark, 미니맵&고도 표시 위치 수정 - 기존 183에서 193으로 수정
#define SMALLMAP_SCALE				0.25f
#define BIGMAP_WIDTH				512 
#define BIGMAP_HEIGHT				512
#define MISSILE_WARNING_X			(g_pD3dApp->GetBackBufferDesc().Width-126)
#define MISSILE_WARNING_Y			18 + 30
#define SITE_START_X				(g_pD3dApp->GetBackBufferDesc().Width-98)
#define SITE_START_Y				32 + 16
#define UNIT_START_X				(g_pD3dApp->GetBackBufferDesc().Width-75)
#define UNIT_START_Y				82 + 16
#define BUTTON_SHOW_SIZE_X			52
#define BUTTON_SHOW_SIZE_Y			11
#define BUTTON_SHOW_START_X			(g_pD3dApp->GetBackBufferDesc().Width-BUTTON_SHOW_SIZE_X)
#define BUTTON_SHOW_START_Y			22 + 16
#define BUTTON_ZOOM_SIZE_X			33
#define BUTTON_ZOOM_SIZE_Y			11
#define BUTTON_ZOOM_START_X			(g_pD3dApp->GetBackBufferDesc().Width-BUTTON_ZOOM_SIZE_X)
#define BUTTON_ZOOM_START_Y			34 + 16

#define RENDER_RADIUS				55
//#define RENDER_DISTANCE				(g_pShuttleChild ? (max(g_pShuttleChild->m_fWeaponMaxDistance,400))

#define BUTTON_ZOOM					0
#define BUTTON_SHOWHIDE				1
#define BUTTON_ZOOM_HIDE			0
#define BUTTON_ZOOM_IN				1				// 큰맵 
#define BUTTON_ZOOM_OUT				2				// 작은 맵
#define BUTTON_SHOW					0
#define BUTTON_HIDE					1

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define UNIT_CENTER_POS_X			(UNIT_START_X-36)
#define UNIT_CENTER_POS_Y			(UNIT_START_Y+2)		  
#else		   
#define UNIT_CENTER_POS_X			(UNIT_START_X-4)
#define UNIT_CENTER_POS_Y			(UNIT_START_Y-4)
#endif
#define UNIT_ANIMATION_TICK_TIME	20

#define UNIT_WIDTH					30
#define UNIT_HEIGHT					40

#define BACK_IMAGE_SIZE				138.0f
#define RADAR_MINIMAP_SIZE			118.0f
#define RADAR_MINIMAP_X				(g_pD3dApp->GetBackBufferDesc().Width-129)
#define RADAR_MINIMAP_Y				44.0f

#define RADAR_BACKIMAGE_X				SMALLMAP_START_X - ( m_pBackImage->GetImgSize().x - pImageTEMP->GetImgSize().x * SMALLMAP_SCALE ) / 2

#define RADAR_RECT_SIZE				100				// 2012-11-09 by jhahn, 레이더 맵 표시 버그 수정

CINFGameMainMiniMap::CINFGameMainMiniMap(CAtumNode* pParent)
{
	m_pParent = pParent;
//	m_pMeshUnit = NULL;					// 2005-09-13 by ispark 별도 처리
//	m_pMeshParty = NULL;
//	m_pMeshArea = NULL;
	m_pBackImage = NULL;
	m_pSiteImage = NULL;
	m_pPlayerImage = NULL;
	m_pImgStateWarning = NULL;			// 2005-07-08 by ispark
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pMiniMapFrame = NULL;
#endif
	m_pMiniMapUnitImage = NULL;
	m_pMiniMapPartyImage = NULL;
	m_pArenaMapInfImg = NULL;
	m_pImgQuestTarget = NULL;
	m_pMiniMapGuildImage = NULL;		// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 미니맵 여단원
	for(int i=0;i<3;i++)
	{
		m_pUserImage[i] = NULL;
		m_pPartyImage[i] = NULL;
		m_pMonsterImage[i] = NULL;
		m_pInfluenceImage[i] = NULL;
		m_pGuildImage[i] = NULL;							// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 레이더 여단원
		m_MiniMapSubDataHeader[i] = NULL;
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<BUTTON_STATE_NUMBER;i++)
	{
		m_pButtonZoom[BUTTON_ZOOM_IN][i] = NULL;
		m_pButtonZoom[BUTTON_ZOOM_OUT][i] = NULL;
		m_pButtonZoom[BUTTON_ZOOM_HIDE][i] = NULL;
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//		m_pButtonShowHide[BUTTON_SHOW][i] = NULL;
//		m_pButtonShowHide[BUTTON_HIDE][i] = NULL;
	}
#endif
	m_pDeadImage = NULL;
	m_pLockonImage = NULL;
	m_pFontPosition = NULL;
	m_pFontQuestTarget = NULL;
	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	//m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
	//m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
	//m_bShowState = TRUE;
	m_nZoomButtonState = BUTTON_STATE_NORMAL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pServerTimeBtn = NULL;
#endif
	// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

	m_bZoomState = 0;
	//m_bShowMap = FALSE;
	m_bRestored = FALSE;
	D3DXMatrixIdentity(&m_matRotate);
	m_fAngle = 0;
	m_fCurrentTime = 0;	
	m_fMissileWarningTime = 0.0f;			// 2005-07-08 by ispark
	m_bMissileWarningRender = FALSE;

	m_fRange = 0.0f;						// 2005-08-16 by ispark
	m_fRadarBackAngle = 0.0f;				// 2005-12-06 by ispark

	m_bMiniMapState = FALSE;
	m_nTestMapPosX			= 0;
	m_nTestMapPosY			= 0;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pMiniMapBK	= NULL;
	m_pBigMapBK		= NULL;
#endif
}

CINFGameMainMiniMap::~CINFGameMainMiniMap()
{
//	util::del(m_pMeshUnit);							// 2005-09-13 by ispark 별도 처리
//	util::del(m_pMeshParty);
//	util::del(m_pMeshArea);
	util::del(m_pBackImage);
	util::del(m_pSiteImage);
	util::del(m_pPlayerImage);
	util::del(m_pImgStateWarning);					// 2005-07-08 by ispark
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pMiniMapFrame);
#endif
	util::del(m_pMiniMapUnitImage);
	util::del(m_pMiniMapPartyImage);
	util::del(m_pMiniMapGuildImage);						// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
	util::del(m_pArenaMapInfImg);
	util::del(m_pImgQuestTarget);
	for(int i=0;i<3;i++)
	{
		util::del(m_pUserImage[i]);
		util::del(m_pPartyImage[i]);
		util::del(m_pMonsterImage[i]);
		util::del(m_pInfluenceImage[i]);
		util::del(m_MiniMapSubDataHeader[i]);
		util::del(m_pGuildImage[i]);						// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<BUTTON_STATE_NUMBER;i++)
	{
		util::del(m_pButtonZoom[BUTTON_ZOOM_IN][i]);
		util::del(m_pButtonZoom[BUTTON_ZOOM_OUT][i]);
		util::del(m_pButtonZoom[BUTTON_ZOOM_HIDE][i]);
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//		util::del(m_pButtonShowHide[BUTTON_SHOW][i]);
//		util::del(m_pButtonShowHide[BUTTON_HIDE][i]);
	}
	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	util::del(m_pServerTimeBtn);
#endif
	
	util::del(m_pDeadImage);
	util::del(m_pLockonImage);
	util::del(m_pFontPosition);
	util::del(m_pFontQuestTarget);
	
//	vector<CD3DHanFont*>::iterator ith = m_vecQuestPosition.begin();
//	while(ith != m_vecQuestPosition.end())
//	{
//		util::del(*ith);
//		ith++;
//	}
//	m_vecQuestPosition.clear();

	vector<MiniMap_PosData*>::iterator itq = m_vecMosterPosition.begin();
	while(itq != m_vecMosterPosition.end())
	{
		util::del(*itq);
		itq++;
	}
	m_vecMosterPosition.clear();
							  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if( m_pMiniMapBK )
		util::del( m_pMiniMapBK );

	if( m_pBigMapBK )
		util::del( m_pBigMapBK );																  
#endif
}

HRESULT CINFGameMainMiniMap::InitDeviceObjects()
{
	char buf[64];
	DataHeader *pDataHeader = m_pGameData->Find("mmback");
	m_pBackImage = new CINFImageEx;
	m_pBackImage->InitDeviceObjects( pDataHeader );
	pDataHeader = m_pGameData->Find("mmsite");
	m_pSiteImage = new CINFImageEx;
	m_pSiteImage->InitDeviceObjects( pDataHeader);
	pDataHeader = m_pGameData->Find("mmaxis");
	m_pPlayerImage = new CINFImageEx;
	m_pPlayerImage->InitDeviceObjects( pDataHeader );


	// 2005-07-08 by ispark
	// 미사일 경고 이미지
	wsprintf(buf, "mwarning");
	pDataHeader = m_pGameData->Find(buf);
	m_pImgStateWarning = new CINFImageEx;
	m_pImgStateWarning->InitDeviceObjects(pDataHeader );


	// 2007-02-26 by dgwoo 퀘스트 타겟에 위치 표시 이미지.
	wsprintf(buf,"qtarget");
	pDataHeader = m_pGameData->Find(buf);
	m_pImgQuestTarget = new CINFImageEx;
	m_pImgQuestTarget->InitDeviceObjects(pDataHeader );
	

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("minimap");
	m_pMiniMapBK = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
	m_pMiniMapBK->InitDeviceObjects( g_pD3dApp->m_pImageList );
#endif


	for(int i=0;i<3;i++)
	{
		wsprintf( buf, "muser%d", i+1);
		pDataHeader = m_pGameData->Find(buf);
		m_pUserImage[i] = new CINFImageEx;
		m_pUserImage[i]->InitDeviceObjects( pDataHeader );
		wsprintf( buf, "mparty%d", i+1);
		pDataHeader = m_pGameData->Find(buf);
		m_pPartyImage[i] = new CINFImageEx;
		m_pPartyImage[i]->InitDeviceObjects( pDataHeader );
		wsprintf( buf, "mmon%d", i+1);
		pDataHeader = m_pGameData->Find(buf);
		m_pMonsterImage[i] = new CINFImageEx;
		m_pMonsterImage[i]->InitDeviceObjects( pDataHeader );
		// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
		wsprintf( buf, "mguild%d", i+1);
		pDataHeader = m_pGameData->Find(buf);
		m_pGuildImage[i] = new CINFImageEx;
		m_pGuildImage[i]->InitDeviceObjects( pDataHeader );
		// end 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
		wsprintf( buf, "iuser%d", i+1);
		pDataHeader = m_pGameData->Find(buf);
		m_pInfluenceImage[i] = new CINFImageEx;
		m_pInfluenceImage[i]->InitDeviceObjects( pDataHeader );
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<BUTTON_STATE_NUMBER;i++)
	{
		wsprintf( buf, "mmzin%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pButtonZoom[BUTTON_ZOOM_IN][i] = new CINFImage;
		m_pButtonZoom[BUTTON_ZOOM_IN][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
		wsprintf( buf, "mmzout%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pButtonZoom[BUTTON_ZOOM_OUT][i] = new CINFImage;
		m_pButtonZoom[BUTTON_ZOOM_OUT][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
		wsprintf( buf, "mmzhide%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pButtonZoom[BUTTON_ZOOM_HIDE][i] = new CINFImage;
		m_pButtonZoom[BUTTON_ZOOM_HIDE][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
		wsprintf( buf, "mmshow%02d", i);
		pDataHeader = m_pGameData->Find(buf);

// 		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
// //		m_pButtonShowHide[BUTTON_SHOW][i] = new CINFImage;
// //		m_pButtonShowHide[BUTTON_SHOW][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
// //		wsprintf( buf, "mmhide%02d", i);
// //		pDataHeader = m_pGameData->Find(buf);
// //		m_pButtonShowHide[BUTTON_HIDE][i] = new CINFImage;
// //		m_pButtonShowHide[BUTTON_HIDE][i]->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );
	}
	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "mmhide03");
		wsprintf(szDownBtn, "mmhide01");
		wsprintf(szSelBtn, "mmhide00");
		wsprintf(szDisBtn, "mmhide02");
		if(NULL == m_pServerTimeBtn)
		{
			m_pServerTimeBtn = new CINFImageBtn;
		}
		m_pServerTimeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		
	}
#endif
	// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

	pDataHeader = m_pGameData->Find("mdead");
	m_pDeadImage = new CINFImageEx;
	m_pDeadImage->InitDeviceObjects( pDataHeader );
	pDataHeader = m_pGameData->Find("mlockon");
	m_pLockonImage = new CINFImageEx;
	m_pLockonImage->InitDeviceObjects( pDataHeader );
	
	m_pFontPosition = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pFontPosition->InitDeviceObjects(g_pD3dDev);

	m_pFontQuestTarget = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pFontQuestTarget->InitDeviceObjects(g_pD3dDev);
	// 2005-09-13 by ispark		별도 처리
/*	CGameData gameData;
	if(gameData.SetFile( ".\\Res-Obj\\arrow.obj", FALSE, NULL, 0))
	{
		m_pMeshUnit = new CSkinnedMesh();
		m_pMeshUnit->InitDeviceObjects();
		m_pMeshUnit->LoadMeshHierarchyFromMem(&gameData);
	}

#ifdef _DEBUG
	else
	{
		DBGOUT("Can't Find (arrow.obj) File\n");
	}
#endif
	CGameData gameDataParty;
	if(gameDataParty.SetFile( ".\\Res-Obj\\arrow2.obj", FALSE, NULL, 0))
	{
		m_pMeshParty = new CSkinnedMesh();
		m_pMeshParty->InitDeviceObjects();
		m_pMeshParty->LoadMeshHierarchyFromMem(&gameDataParty);
	}
 
#ifdef _DEBUG
	else
	{
		DBGOUT("Can't Find (arrow2.obj) File\n");
	}
#endif
	CGameData gameDataa;
	if(gameDataa.SetFile( ".\\Res-Obj\\area.obj", FALSE, NULL, 0))
	{
		m_pMeshArea = new CSkinnedMesh();
		m_pMeshArea->InitDeviceObjects();
		m_pMeshArea->LoadMeshHierarchyFromMem(&gameDataa);
	}
#ifdef _DEBUG
	else
	{
		DBGOUT("Can't Find (area.obj) File \n");
	}
#endif
*/
	// 2005-09-09 by ispark
	// 미니맵 틀
	CGameData MiniMapData;
	MiniMapData.SetFile( ".\\Res-Tex\\mini.tex", FALSE, NULL, 0, FALSE );
	m_MiniMapSubDataHeader[0] = MiniMapData.FindFromFile("minmap");
	if(m_MiniMapSubDataHeader[0])
	{
// 		m_pMiniMapFrame = new CINFImageEx;
// 		m_pMiniMapFrame->InitDeviceObjects(m_MiniMapSubDataHeader[0]);
// 		m_pMiniMapFrame->RestoreDeviceObjects();					  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("minimapZI");
		m_pBigMapBK = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pBigMapBK->InitDeviceObjects( g_pD3dApp->m_pImageList );								  
#else
		m_pMiniMapFrame = new CINFImageEx;
		m_pMiniMapFrame->InitDeviceObjects(m_MiniMapSubDataHeader[0]);
		m_pMiniMapFrame->RestoreDeviceObjects();
#endif
	}
	m_MiniMapSubDataHeader[1] = MiniMapData.FindFromFile("gear1");
	if(m_MiniMapSubDataHeader[1])
	{
		m_pMiniMapUnitImage = new CINFImageEx;
		m_pMiniMapUnitImage->InitDeviceObjects(m_MiniMapSubDataHeader[1]);
		m_pMiniMapUnitImage->RestoreDeviceObjects();
	}
	m_MiniMapSubDataHeader[2] = MiniMapData.FindFromFile("gear2");
	if(m_MiniMapSubDataHeader[2])
	{
		m_pMiniMapPartyImage = new CINFImageEx;
		m_pMiniMapPartyImage->InitDeviceObjects(m_MiniMapSubDataHeader[2]);
		m_pMiniMapPartyImage->RestoreDeviceObjects();
	}
	// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
	m_MiniMapSubDataHeader[3] = MiniMapData.FindFromFile("gear3");
	if(m_MiniMapSubDataHeader[3])
	{
		m_pMiniMapGuildImage = new CINFImageEx;
		m_pMiniMapGuildImage->InitDeviceObjects(m_MiniMapSubDataHeader[3]);
		m_pMiniMapGuildImage->RestoreDeviceObjects();
	}// end 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시	

	pDataHeader = MiniMapData.FindFromFile("gear3");
	if(pDataHeader)
	{
		m_pArenaMapInfImg = new CINFImageEx;
		m_pArenaMapInfImg->InitDeviceObjects(pDataHeader);
		m_pArenaMapInfImg->RestoreDeviceObjects();
	}
	// 2005-09-21 by ispark
	// 레이더 미니맵
	m_pRadarMiniMap = new CINFImageEx;
	m_pRadarMiniMap->InitDeviceObjectsEx();
	return S_OK;
}

HRESULT CINFGameMainMiniMap::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		m_pBackImage->RestoreDeviceObjects();
		m_pSiteImage->RestoreDeviceObjects();
		m_pPlayerImage->RestoreDeviceObjects();
		m_pImgStateWarning->RestoreDeviceObjects();			// 2005-07-08 by ispark
		m_pImgQuestTarget->RestoreDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pMiniMapFrame->RestoreDeviceObjects();
#endif
		m_pMiniMapUnitImage->RestoreDeviceObjects();
		m_pMiniMapPartyImage->RestoreDeviceObjects();
		m_pArenaMapInfImg->RestoreDeviceObjects();
		m_pMiniMapGuildImage->RestoreDeviceObjects();		// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
		for(int i=0;i<3;i++)
		{
			m_pUserImage[i]->RestoreDeviceObjects();
			m_pPartyImage[i]->RestoreDeviceObjects();
			m_pMonsterImage[i]->RestoreDeviceObjects();
			m_pInfluenceImage[i]->RestoreDeviceObjects();
			m_pGuildImage[i]->RestoreDeviceObjects();		// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
		}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		for(i=0;i<BUTTON_STATE_NUMBER;i++)
		{
			m_pButtonZoom[BUTTON_ZOOM_IN][i]->RestoreDeviceObjects();
			m_pButtonZoom[BUTTON_ZOOM_OUT][i]->RestoreDeviceObjects();
			m_pButtonZoom[BUTTON_ZOOM_HIDE][i]->RestoreDeviceObjects();
			// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//			m_pButtonShowHide[BUTTON_SHOW][i]->RestoreDeviceObjects();
//			m_pButtonShowHide[BUTTON_HIDE][i]->RestoreDeviceObjects();
		}
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(m_pServerTimeBtn)
		{
			m_pServerTimeBtn->RestoreDeviceObjects();						
			m_pServerTimeBtn->SetBtnPosition(BUTTON_SHOW_START_X, BUTTON_SHOW_START_Y);	
		}
#endif
		// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

		m_pDeadImage->RestoreDeviceObjects();
		m_pLockonImage->RestoreDeviceObjects();
		m_pFontPosition->RestoreDeviceObjects();
		m_pFontQuestTarget->RestoreDeviceObjects();

//		vector<CD3DHanFont*>::iterator ith = m_vecQuestPosition.begin();
//		while(ith != m_vecQuestPosition.end())
//		{
//			(*ith)->RestoreDeviceObjects();
//			ith++;
//		}

		// 2005-09-21 by ispark
		// 레이더 미니맵
		InitRadarMap();
		
		// 2005-09-13 by ispark 별도 처리
/*		m_pMeshUnit->RestoreDeviceObjects();
		m_pMeshParty->RestoreDeviceObjects();
		m_pMeshArea->RestoreDeviceObjects();
*/													  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
        m_pMiniMapBK->RestoreDeviceObjects();
		m_pBigMapBK->RestoreDeviceObjects();
#endif
		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFGameMainMiniMap::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		m_pBackImage->InvalidateDeviceObjects();
		m_pSiteImage->InvalidateDeviceObjects();
		m_pPlayerImage->InvalidateDeviceObjects();
		m_pImgStateWarning->InvalidateDeviceObjects();		// 2005-07-08 by ispark
		m_pImgQuestTarget->InvalidateDeviceObjects();		// 2007-02-26 by dgwoo
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pMiniMapFrame->InvalidateDeviceObjects();
#endif
		m_pMiniMapUnitImage->InvalidateDeviceObjects();
		m_pMiniMapPartyImage->InvalidateDeviceObjects();
		m_pArenaMapInfImg->InvalidateDeviceObjects();
		m_pMiniMapGuildImage->InvalidateDeviceObjects();	// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
		for(int i=0;i<3;i++)
		{
			m_pUserImage[i]->InvalidateDeviceObjects();
			m_pPartyImage[i]->InvalidateDeviceObjects();
			m_pMonsterImage[i]->InvalidateDeviceObjects();
			m_pInfluenceImage[i]->InvalidateDeviceObjects();
			m_pGuildImage[i]->InvalidateDeviceObjects();// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시
		}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		for(i=0;i<BUTTON_STATE_NUMBER;i++)
		{
			m_pButtonZoom[BUTTON_ZOOM_IN][i]->InvalidateDeviceObjects();
			m_pButtonZoom[BUTTON_ZOOM_OUT][i]->InvalidateDeviceObjects();
			m_pButtonZoom[BUTTON_ZOOM_HIDE][i]->InvalidateDeviceObjects();
			// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//			m_pButtonShowHide[BUTTON_SHOW][i]->InvalidateDeviceObjects();
//			m_pButtonShowHide[BUTTON_HIDE][i]->InvalidateDeviceObjects();
		}
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
		if(m_pServerTimeBtn)
		{
			m_pServerTimeBtn->InvalidateDeviceObjects();		
		}
#endif
		// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가

		m_pDeadImage->InvalidateDeviceObjects();
		m_pLockonImage->InvalidateDeviceObjects();
		m_pFontPosition->InvalidateDeviceObjects();
		m_pFontQuestTarget->InvalidateDeviceObjects();

//		vector<CD3DHanFont*>::iterator ith = m_vecQuestPosition.begin();
//		while(ith != m_vecQuestPosition.end())
//		{
//			(*ith)->InvalidateDeviceObjects();
//			ith++;
//		}

		// 2005-09-21 by ispark
		// 레이더 미니맵
		m_pRadarMiniMap->InvalidateDeviceObjects();

		// 2005-09-13 by ispark 별도 처리
/*		m_pMeshUnit->InvalidateDeviceObjects();
		m_pMeshParty->InvalidateDeviceObjects();
		m_pMeshArea->InvalidateDeviceObjects();
*/		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
        m_pMiniMapBK->InvalidateDeviceObjects();
		m_pBigMapBK->InvalidateDeviceObjects();	
#endif
		m_bRestored = FALSE;
	}
	return S_OK;
}

HRESULT CINFGameMainMiniMap::DeleteDeviceObjects()
{
	m_pBackImage->DeleteDeviceObjects();
	m_pSiteImage->DeleteDeviceObjects();
	m_pPlayerImage->DeleteDeviceObjects();
	m_pImgStateWarning->DeleteDeviceObjects();
	m_pImgQuestTarget->DeleteDeviceObjects();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pMiniMapFrame->DeleteDeviceObjects();
#endif
	m_pMiniMapUnitImage->DeleteDeviceObjects();
	m_pMiniMapPartyImage->DeleteDeviceObjects();
	m_pArenaMapInfImg->DeleteDeviceObjects();
	m_pMiniMapGuildImage->DeleteDeviceObjects();			// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
	util::del(m_pBackImage);
	util::del(m_pSiteImage);
	util::del(m_pPlayerImage);
	util::del(m_pImgStateWarning);						// 2005-07-08 by ispark
	util::del(m_pImgQuestTarget);							// 2007-02-26 by dgwoo
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del(m_pMiniMapFrame);
#endif
	util::del(m_pMiniMapUnitImage);
	util::del(m_pMiniMapPartyImage);
	util::del(m_pArenaMapInfImg);
	util::del(m_pMiniMapGuildImage);						// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
	for(int i=0;i<3;i++)
	{
		m_pUserImage[i]->DeleteDeviceObjects();
		m_pPartyImage[i]->DeleteDeviceObjects();
		m_pMonsterImage[i]->DeleteDeviceObjects();
		m_pInfluenceImage[i]->DeleteDeviceObjects();
		m_pGuildImage[i]->DeleteDeviceObjects();
		util::del(m_pUserImage[i]);
		util::del(m_pPartyImage[i]);
		util::del(m_pMonsterImage[i]);
		util::del(m_pInfluenceImage[i]);
		util::del(m_pGuildImage[i]);
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<BUTTON_STATE_NUMBER;i++)
	{
		m_pButtonZoom[BUTTON_ZOOM_IN][i]->DeleteDeviceObjects();
		m_pButtonZoom[BUTTON_ZOOM_OUT][i]->DeleteDeviceObjects();
		m_pButtonZoom[BUTTON_ZOOM_HIDE][i]->DeleteDeviceObjects();
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//		m_pButtonShowHide[BUTTON_SHOW][i]->DeleteDeviceObjects();
//		m_pButtonShowHide[BUTTON_HIDE][i]->DeleteDeviceObjects();
		util::del(m_pButtonZoom[BUTTON_ZOOM_IN][i]);
		util::del(m_pButtonZoom[BUTTON_ZOOM_OUT][i]);
		util::del(m_pButtonZoom[BUTTON_ZOOM_HIDE][i]);
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//		util::del(m_pButtonShowHide[BUTTON_SHOW][i]);
//		util::del(m_pButtonShowHide[BUTTON_HIDE][i]);
	}
	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	if(m_pServerTimeBtn)
	{
		m_pServerTimeBtn->DeleteDeviceObjects();	
		util::del(m_pServerTimeBtn);
	}
#endif
	// end 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
	m_pDeadImage->DeleteDeviceObjects();
	m_pLockonImage->DeleteDeviceObjects();
	m_pFontPosition->DeleteDeviceObjects();
	m_pFontQuestTarget->DeleteDeviceObjects();

//	vector<CD3DHanFont*>::iterator ith = m_vecQuestPosition.begin();
//	while(ith != m_vecQuestPosition.end())
//	{
//		(*ith)->DeleteDeviceObjects();
//		util::del(*ith);
//		ith++;
//	}

	util::del(m_pDeadImage);
	util::del(m_pLockonImage);
	util::del(m_pFontPosition);
	util::del(m_pFontQuestTarget);

	// 2005-09-21 by ispark
	// 레이더 미니맵
	util::del(m_pRadarMiniMap);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pMiniMapBK->DeleteDeviceObjects();
	util::del(m_pMiniMapBK);

	m_pBigMapBK->DeleteDeviceObjects();
	util::del(m_pBigMapBK);
#endif
	// 2005-09-13 by ispark 별도 처리
/*	m_pMeshUnit->DeleteDeviceObjects();
	util::del(m_pMeshUnit);
	m_pMeshParty->DeleteDeviceObjects();
	util::del(m_pMeshParty);
	m_pMeshArea->DeleteDeviceObjects();
	util::del(m_pMeshArea);
*/	return S_OK;
}

void CINFGameMainMiniMap::Render()
{
	// 상점에선 기타 정보 렌더링을 안한다
	if(g_pD3dApp->m_dwGameState == _SHOP)
		return;

	if(!g_pGround)				//2013-05-22 by ssjung 캐나다 익셉션 오류 예방 코드 추가
		return;

	CheckRadar();									// 2005-08-16 by ispark

	// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//	if(m_bShowState)
	{
		// Show 버튼
//		m_pButtonShowHide[BUTTON_HIDE][m_nButtonState[BUTTON_SHOWHIDE]]->Move( BUTTON_SHOW_START_X, BUTTON_SHOW_START_Y );
//		m_pButtonShowHide[BUTTON_HIDE][m_nButtonState[BUTTON_SHOWHIDE]]->Render();

		// Zoom 버튼
//		if(m_bZoomState == BUTTON_ZOOM_OUT)
//		{
//			m_pButtonZoom[BUTTON_ZOOM_OUT][m_nButtonState[BUTTON_ZOOM]]->Move( BUTTON_ZOOM_START_X, BUTTON_ZOOM_START_Y );
//			m_pButtonZoom[BUTTON_ZOOM_OUT][m_nButtonState[BUTTON_ZOOM]]->Render();
//		}
//		else if(m_bZoomState == BUTTON_ZOOM_IN)
//		{
//			m_pButtonZoom[BUTTON_ZOOM_IN][m_nButtonState[BUTTON_ZOOM]]->Move( BUTTON_ZOOM_START_X, BUTTON_ZOOM_START_Y );
//			m_pButtonZoom[BUTTON_ZOOM_IN][m_nButtonState[BUTTON_ZOOM]]->Render();
//		}
//		else if(m_bZoomState == BUTTON_ZOOM_HIDE)
//		{
//			m_pButtonZoom[BUTTON_ZOOM_HIDE][m_nButtonState[BUTTON_ZOOM]]->Move( BUTTON_ZOOM_START_X, BUTTON_ZOOM_START_Y );
//			m_pButtonZoom[BUTTON_ZOOM_HIDE][m_nButtonState[BUTTON_ZOOM]]->Render();
//		}
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pServerTimeBtn->Render();
		
		if(m_bZoomState == BUTTON_ZOOM_OUT)
		{
			m_pButtonZoom[BUTTON_ZOOM_OUT][m_nZoomButtonState]->Move( BUTTON_ZOOM_START_X, BUTTON_ZOOM_START_Y );
			m_pButtonZoom[BUTTON_ZOOM_OUT][m_nZoomButtonState]->Render();
		}
		else if(m_bZoomState == BUTTON_ZOOM_IN)
		{
			m_pButtonZoom[BUTTON_ZOOM_IN][m_nZoomButtonState]->Move( BUTTON_ZOOM_START_X, BUTTON_ZOOM_START_Y );
			m_pButtonZoom[BUTTON_ZOOM_IN][m_nZoomButtonState]->Render();
		}
		else if(m_bZoomState == BUTTON_ZOOM_HIDE)
		{
			m_pButtonZoom[BUTTON_ZOOM_HIDE][m_nZoomButtonState]->Move( BUTTON_ZOOM_START_X, BUTTON_ZOOM_START_Y );
			m_pButtonZoom[BUTTON_ZOOM_HIDE][m_nZoomButtonState]->Render();
		}
#endif

		// 레이다 미니맵
		RenderRadarMap();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		// 2013-02-15 by bhsohn 레이다 위치 수정
		// 레이다 본체
//		m_pBackImage->Move( SMALLMAP_START_X - 9  , BACK_START_Y  );
		m_pBackImage->Move( SMALLMAP_START_X -19, BACK_START_Y-10 );
		// END 2013-02-15 by bhsohn 레이다 위치 수정
		m_pBackImage->Rotate( m_pBackImage->GetImgSize().x/2.0f, m_pBackImage->GetImgSize().y/2.0f, m_fRadarBackAngle);
		m_pBackImage->Render();
	
		// 시야 삼각형
		// 2013-02-15 by bhsohn 레이다 위치 수정
//		m_pSiteImage->Move( SMALLMAP_START_X - 10 + m_pBackImage->GetImgSize().x /2 - m_pSiteImage->GetImgSize().x /2, BACK_START_Y + 15 );
		m_pSiteImage->Move( SMALLMAP_START_X - 19 + m_pBackImage->GetImgSize().x /2 - m_pSiteImage->GetImgSize().x /2, BACK_START_Y + 15 );
		// END 2013-02-15 by bhsohn 레이다 위치 수정
		m_pSiteImage->Render();

		// 레이다 중앙 - Unit Pos
		// 2013-02-15 by bhsohn 레이다 위치 수정
//		m_pPlayerImage->Move( SMALLMAP_START_X - 10 + m_pBackImage->GetImgSize().x /2 - m_pPlayerImage->GetImgSize().x /2, BACK_START_Y + 66);
		m_pPlayerImage->Move( SMALLMAP_START_X - 19 + m_pBackImage->GetImgSize().x /2 - m_pPlayerImage->GetImgSize().x /2, BACK_START_Y + 66);
		// END 2013-02-15 by bhsohn 레이다 위치 수정
		m_pPlayerImage->Render();
#else				 
		m_pBackImage->Move( BACK_START_X, BACK_START_Y );
		m_pBackImage->Rotate( BACK_IMAGE_SIZE/2.0f, BACK_IMAGE_SIZE/2.0f, m_fRadarBackAngle);
		m_pBackImage->Render();
	
		// 시야 삼각형
		m_pSiteImage->Move( SITE_START_X, SITE_START_Y );
		m_pSiteImage->Render();

		// 레이다 중앙 - Unit Pos
		m_pPlayerImage->Move( UNIT_START_X, UNIT_START_Y );
		m_pPlayerImage->Render();
#endif

		if(m_bZoomState)
		{
			RenderMap();
		}

		// 2005-08-16 by ispark
		if(g_pShuttleChild->m_pRadarItemInfo != NULL)
		{
			RenderMonster();
			RenderEnemy();
		}
	}
//	else
//	{
//		// Hide 버튼
//		m_pButtonShowHide[BUTTON_SHOW][m_nButtonState[BUTTON_SHOWHIDE]]->Move( BUTTON_SHOW_START_X, BUTTON_SHOW_START_Y );
//		m_pButtonShowHide[BUTTON_SHOW][m_nButtonState[BUTTON_SHOWHIDE]]->Render();
//	}

	// 2005-07-08 by ispark
	// 미사일 경고 UI 및 사운드
	// 2005-08-16 by ispark
	if(g_pShuttleChild->m_pRadarItemInfo != NULL)
	{
		if(g_pShuttleChild->IsMissileWarning())
		{
			if(m_bMissileWarningRender)
			{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				m_pImgStateWarning->Move(SMALLMAP_START_X , BACK_START_Y + 9);
#else													   
				m_pImgStateWarning->Move(MISSILE_WARNING_X, MISSILE_WARNING_Y);
#endif
				m_pImgStateWarning->Render();

				if(0.5f < m_fMissileWarningTime)
				{
					m_fMissileWarningTime = 0.0f;
					m_bMissileWarningRender = !m_bMissileWarningRender;
				}
			}
			else
			{
				if(0.2f < m_fMissileWarningTime)
				{
					m_fMissileWarningTime = 0.0f;
					m_bMissileWarningRender = !m_bMissileWarningRender;
				}
			}

			m_fMissileWarningTime += g_pD3dApp->GetElapsedTime();
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_MISSILE_WARNNING, g_pShuttleChild->m_vPos, FALSE);
		}
		else
		{
			g_pD3dApp->m_pSound->StopD3DSound(SOUND_MISSILE_WARNNING);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		dgwoo
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::RenderMap()
{
	ASSERT_ASSERT(g_pGround);
	if(g_pGround->m_bMiniMap)
	{
		if(m_bZoomState == BUTTON_ZOOM_IN)
		{
			RenderBigMap();
		}
		else if(m_bZoomState == BUTTON_ZOOM_OUT)
		{
			RenderSmallMap();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void RenderSmallMap()
/// \brief		고도위치에 작은맵을 출력.
/// \author		dgwoo
/// \date		2007-02-21 ~ 2007-02-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::RenderSmallMap()
{
		// 2D map
	float fMiniMapX, fMiniMapY;
	fMiniMapX = SMALLMAP_START_X;
	fMiniMapY = SMALLMAP_START_Y;


	// 2006-04-08 by ispark, 도시 전용 맵 추가
	CINFImageEx* pMiniBigMapImageTEMP = NULL;
	float fMyPosX = 0.0f;
	float fMyPosY = 0.0f;
	if(g_pD3dApp->m_bCharacter)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
		GetMiniSmallMapCityPos(g_pShuttleChild->m_vPos, &fMyPosX, &fMyPosY);
	}

	if(pMiniBigMapImageTEMP == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
		// 2D Unit
		fMyPosX = 0.0f;
		fMyPosY = 0.0f;
		GetMiniSmallMapPos(g_pShuttleChild->m_vPos, &fMyPosX, &fMyPosY);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pMiniMapBK->Move(fMiniMapX - ( m_pMiniMapBK->GetMaxPos().x - ( pMiniBigMapImageTEMP->GetImgSize().x * SMALLMAP_SCALE ) )/2
					 , fMiniMapY - ( m_pMiniMapBK->GetMaxPos().y - ( pMiniBigMapImageTEMP->GetImgSize().y * SMALLMAP_SCALE ) )/2 );
	m_pMiniMapBK->Render();
#endif
	m_pMiniMapUnitImage->Move(fMiniMapX + fMyPosX, 
								fMiniMapY + fMyPosY);
	if(pMiniBigMapImageTEMP)
	{
		pMiniBigMapImageTEMP->Move(fMiniMapX, fMiniMapY);
		pMiniBigMapImageTEMP->Render();
	}

	m_pMiniMapUnitImage->Rotate(UNIT_WIDTH/8, UNIT_HEIGHT/8, m_fAngle);
	m_pMiniMapUnitImage->Render();

	// 2007-02-26 by dgwoo 맵에 퀘스트몬스터 위치를 표시한다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 퀘스트 수행지역
	vector<MiniMap_PosData*>::iterator	it  = m_vecMosterPosition.begin();

	while(it != m_vecMosterPosition.end())
	{
		POINT pt = m_pImgQuestTarget->GetImgSize();

		int tempX = ((*it)->nMonsterPosX/4 - ( pt.x / 8));
		int tempY = ((*it)->nMonsterPosY/4 - ( pt.y / 8));

		m_pImgQuestTarget->Move(fMiniMapX + tempX,
									fMiniMapY + tempY);
		m_pImgQuestTarget->Render();
		
		it++;
	}
	//////////////////////////////////////////////////////////////////////////
	// 아레나 맵에서 자신의 세력의 위치를 표시.
	if(g_pInterface->IsArenaStart())
	{
		CUnitData * pUnit = NULL;
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		//vector<SARENA_TEAM_CHARACTERINFO>::iterator it = 
		vector<STRUCT_TEAM_CHARACTERINFO>::iterator it = 
			g_pInterface->m_pGameArena->m_pINFArenaTeamInfo->m_vecTeamUserInfo.begin();
		
		for(;it != g_pInterface->m_pGameArena->m_pINFArenaTeamInfo->m_vecTeamUserInfo.end(); it++)
		{
			if(it->ClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
				continue;
			pUnit = g_pScene->FindUnitDataByClientIndex(it->ClientIndex);
			if(	pUnit == NULL)
				continue;


			D3DXVECTOR2 vArenaVel;
			float fArenaAngle;
			
			vArenaVel.x = pUnit->m_vVel.x;
			vArenaVel.y = pUnit->m_vVel.z;

			D3DXVec2Normalize(&vArenaVel,&vArenaVel);
			fArenaAngle = ACOS(D3DXVec2Dot(&vArenaVel,&(D3DXVECTOR2(0,1))));
			if(vArenaVel.x > 0 )
				fArenaAngle *= -1;
			
			float fEnemyPosX = 0.0f;
			float fEnemyPosY = 0.0f;

//			if(g_pD3dApp->m_bCharacter)
//			{
//				GetMiniBigMapCityPos((*itParty)->m_pEnemyData->m_vPos, &fEnemyPosX, &fEnemyPosY);
//			}

			if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				fEnemyPosX = 0.0f;
				fEnemyPosY = 0.0f;
				GetMiniSmallMapPos(pUnit->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(fEnemyPosX >= 0 &&
				fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
				fEnemyPosY >= 0 &&
				fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
			{
			
				m_pArenaMapInfImg->Move(fMiniMapX + fEnemyPosX, 
											fMiniMapY + fEnemyPosY);
				m_pArenaMapInfImg->Rotate(UNIT_WIDTH/8, UNIT_HEIGHT/8, fArenaAngle);
				m_pArenaMapInfImg->Render();
			}
		}

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  party
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	while(itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		if((*itParty)->m_pEnemyData)
		{
			D3DXVECTOR2 vPartyVel;
			float fPartyAngle;
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			//char EnemyName[SIZE_MAX_CHARACTER_NAME];
			char EnemyName[SIZE_MAX_ARENA_FULL_NAME];

			vPartyVel.x = (*itParty)->m_pEnemyData->m_vVel.x;
			vPartyVel.y = (*itParty)->m_pEnemyData->m_vVel.z;
			wsprintf(EnemyName,"%s",(*itParty)->m_ImPartyMemberInfo.CharacterName);

			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), EnemyName);

			D3DXVec2Normalize(&vPartyVel,&vPartyVel);
			fPartyAngle = ACOS(D3DXVec2Dot(&vPartyVel,&(D3DXVECTOR2(0,1))));
			if(vPartyVel.x > 0 )
				fPartyAngle *= -1;
			
			float fEnemyPosX = 0.0f;
			float fEnemyPosY = 0.0f;

			if(g_pD3dApp->m_bCharacter)
			{
				GetMiniSmallMapCityPos((*itParty)->m_pEnemyData->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}

			if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				fEnemyPosX = 0.0f;
				fEnemyPosY = 0.0f;
				GetMiniSmallMapPos((*itParty)->m_pEnemyData->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(fEnemyPosX >= 0 &&
				fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
				fEnemyPosY >= 0 &&
				fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
			{
				
				m_pMiniMapPartyImage->Move(fMiniMapX + fEnemyPosX, 
											fMiniMapY + fEnemyPosY);
				m_pMiniMapPartyImage->Rotate(UNIT_WIDTH/8, UNIT_HEIGHT/8, fPartyAngle);
				m_pMiniMapPartyImage->Render();
			}
		}
		itParty++;
	}

	// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 미니맵 여단원 표시
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Guild
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		PARTYENEMYINFO*	pPartyEnemyInfo = g_pShuttleChild->m_pClientParty->PartyFindMemberByClientIndex(itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex);
		structGuildInfo* pGuildEnemyInfo = g_pGameMain->m_pCommunity->GetGuild()->GetGuildMemberInfo(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber);
		
		if(!pPartyEnemyInfo && pGuildEnemyInfo)
		{
			D3DXVECTOR2 vGuildVel;
			float fGuildAngle;
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			//char EnemyName[SIZE_MAX_CHARACTER_NAME];
			char EnemyName[SIZE_MAX_ARENA_FULL_NAME];
			
			vGuildVel.x = itEnemy->second->m_vVel.x;
			vGuildVel.y = itEnemy->second->m_vVel.z;
			//wsprintf(EnemyName,"%s",(*itGuInfo)->MemberName);
			wsprintf(EnemyName,"%s",itEnemy->second->GetEnemyInfo().CharacterInfo.CharacterName);
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), EnemyName);
			
			D3DXVec2Normalize(&vGuildVel,&vGuildVel);
			fGuildAngle = ACOS(D3DXVec2Dot(&vGuildVel,&(D3DXVECTOR2(0,1))));
			if(vGuildVel.x > 0 )
				fGuildAngle *= -1;
			
			float fEnemyPosX = 0.0f;
			float fEnemyPosY = 0.0f;
			
			if(g_pD3dApp->m_bCharacter)
			{
				GetMiniSmallMapCityPos(itEnemy->second->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				fEnemyPosX = 0.0f;
				fEnemyPosY = 0.0f;
				GetMiniSmallMapPos(itEnemy->second->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(fEnemyPosX >= 0 &&
				fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
				fEnemyPosY >= 0 &&
				fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
			{
				
				m_pMiniMapGuildImage->Move(fMiniMapX + fEnemyPosX, 
					fMiniMapY + fEnemyPosY);
				m_pMiniMapGuildImage->Rotate(UNIT_WIDTH/8, UNIT_HEIGHT/8, fGuildAngle);
				m_pMiniMapGuildImage->Render();
			}
		}
		itEnemy++;
	}// end 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 미니맵 여단원 표시

}
void CINFGameMainMiniMap::RenderOperation()
{
	// 2D map
	float fMiniMapX, fMiniMapY;
	fMiniMapX = BIGMAP_START_X;
	fMiniMapY = BIGMAP_START_Y;

	CUnitData * pUnit = NULL;
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		if(itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
			continue;
		pUnit = g_pScene->FindUnitDataByClientIndex(itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex);
		if(	pUnit == NULL)
			continue;


		D3DXVECTOR2 vArenaVel;
		float fArenaAngle;
		
		vArenaVel.x = pUnit->m_vVel.x;
		vArenaVel.y = pUnit->m_vVel.z;

		D3DXVec2Normalize(&vArenaVel,&vArenaVel);
		fArenaAngle = ACOS(D3DXVec2Dot(&vArenaVel,&(D3DXVECTOR2(0,1))));
		if(vArenaVel.x > 0 )
			fArenaAngle *= -1;
		
		float fEnemyPosX = 0.0f;
		float fEnemyPosY = 0.0f;

		if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
		{
			fEnemyPosX = 0.0f;
			fEnemyPosY = 0.0f;
			GetMiniBigMapPos(pUnit->m_vPos, &fEnemyPosX, &fEnemyPosY);
		}
		
		if(fEnemyPosX >= 0 &&
			fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
			fEnemyPosY >= 0 &&
			fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
		{
			if(IS_VCN_INFLUENCE_TYPE(itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType))
			{
				if(g_pShuttleChild->m_stObserve.ClientIndex == itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex)
				{
					m_pMiniMapPartyImage->Move(fMiniMapX + fEnemyPosX, 
												fMiniMapY + fEnemyPosY);
					m_pMiniMapPartyImage->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, fArenaAngle);
					m_pMiniMapPartyImage->Render();
				}else
				{
					m_pMiniMapUnitImage->Move(fMiniMapX + fEnemyPosX, 
												fMiniMapY + fEnemyPosY);
					m_pMiniMapUnitImage->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, fArenaAngle);
					m_pMiniMapUnitImage->Render();
				}
			}else if(IS_ANI_INFLUENCE_TYPE(itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType))
			{
				if(g_pShuttleChild->m_stObserve.ClientIndex == itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex)
				{
					m_pMiniMapPartyImage->Move(fMiniMapX + fEnemyPosX, 
												fMiniMapY + fEnemyPosY);
					m_pMiniMapPartyImage->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, fArenaAngle);
					m_pMiniMapPartyImage->Render();
				}else
				{
					m_pArenaMapInfImg->Move(fMiniMapX + fEnemyPosX, 
												fMiniMapY + fEnemyPosY);
					m_pArenaMapInfImg->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, fArenaAngle);
					m_pArenaMapInfImg->Render();
				}
			}
		}
				
		itEnemy++;
	}
}
void CINFGameMainMiniMap::RenderBigMap()
{

	// 2D map
	float fMiniMapX, fMiniMapY;
	fMiniMapX = BIGMAP_START_X;
	fMiniMapY = BIGMAP_START_Y;

	// 2006-04-08 by ispark, 도시 전용 맵 추가
	CINFImage* pMiniBigMapImageTEMP = NULL;
	float fMyPosX = 0.0f;
	float fMyPosY = 0.0f;
	if(g_pD3dApp->m_bCharacter)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
		GetMiniBigMapCityPos(g_pShuttleChild->m_vPos, &fMyPosX, &fMyPosY);
	}

	if(pMiniBigMapImageTEMP == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
		// 2D Unit
		fMyPosX = 0.0f;
		fMyPosY = 0.0f;
		GetMiniBigMapPos(g_pShuttleChild->m_vPos, &fMyPosX, &fMyPosY);
	}
	
	m_pMiniMapUnitImage->Move(fMiniMapX + fMyPosX, 
								fMiniMapY + fMyPosY);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경	
	m_pBigMapBK->Move(fMiniMapX - ( m_pBigMapBK->GetMaxPos().x - pMiniBigMapImageTEMP->GetImgSize().x ) /2
					, fMiniMapY - ( m_pBigMapBK->GetMaxPos().y - pMiniBigMapImageTEMP->GetImgSize().y ) /2 );
	m_pBigMapBK->Render();
#endif

	if(pMiniBigMapImageTEMP)
	{
		pMiniBigMapImageTEMP->Move(fMiniMapX, fMiniMapY);
		pMiniBigMapImageTEMP->Render();
	}
	// 2D Frame
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pMiniMapFrame->Move(fMiniMapX - 10.0f, fMiniMapY - 16.5f);
	m_pMiniMapFrame->Render();
#endif

	// 관전 모드일때 모든 유저의 위치를 표시해준다.
	if(g_pShuttleChild->IsOperation())
	{
		RenderOperation();
		return;
	}
	// 관전 모드.
	//--------------------------------------------------------------------------//


	m_pMiniMapUnitImage->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, m_fAngle);
	m_pMiniMapUnitImage->Render();

	
	//////////////////////////////////////////////////////////////////////////
	// 아레나 맵에서 자신의 세력의 위치를 표시.
	if(g_pInterface->IsArenaStart())
	{
		CUnitData * pUnit = NULL;
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		//vector<SARENA_TEAM_CHARACTERINFO>::iterator it = 
		vector<STRUCT_TEAM_CHARACTERINFO>::iterator it = 		
			g_pInterface->m_pGameArena->m_pINFArenaTeamInfo->m_vecTeamUserInfo.begin();
		
		for(;it != g_pInterface->m_pGameArena->m_pINFArenaTeamInfo->m_vecTeamUserInfo.end(); it++)
		{
			if(it->ClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
				continue;
			pUnit = g_pScene->FindUnitDataByClientIndex(it->ClientIndex);
			if(	pUnit == NULL)
				continue;


			D3DXVECTOR2 vArenaVel;
			float fArenaAngle;
			
			vArenaVel.x = pUnit->m_vVel.x;
			vArenaVel.y = pUnit->m_vVel.z;

			// 2012-05-22 by isshin 레이더 및 미니맵 파티,여단원 표시
			char TeamName[SIZE_MAX_ARENA_FULL_NAME];
			wsprintf(TeamName,"%s",it->CharacterName);
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), TeamName);
			// end 2012-05-22 by isshin 레이더 및 미니맵 파티,여단원 표시

			D3DXVec2Normalize(&vArenaVel,&vArenaVel);
			fArenaAngle = ACOS(D3DXVec2Dot(&vArenaVel,&(D3DXVECTOR2(0,1))));
			if(vArenaVel.x > 0 )
				fArenaAngle *= -1;
			
			float fEnemyPosX = 0.0f;
			float fEnemyPosY = 0.0f;

//			if(g_pD3dApp->m_bCharacter)
//			{
//				GetMiniBigMapCityPos((*itParty)->m_pEnemyData->m_vPos, &fEnemyPosX, &fEnemyPosY);
//			}

			if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				fEnemyPosX = 0.0f;
				fEnemyPosY = 0.0f;
				GetMiniBigMapPos(pUnit->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(fEnemyPosX >= 0 &&
				fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
				fEnemyPosY >= 0 &&
				fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
			{
			
				m_pArenaMapInfImg->Move(fMiniMapX + fEnemyPosX, 
											fMiniMapY + fEnemyPosY);
				m_pArenaMapInfImg->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, fArenaAngle);
				m_pArenaMapInfImg->Render();

				// 2012-05-22 by isshin 레이더 및 미니맵 파티,여단원 표시
				int nTeamNameX = (int)(fMiniMapX + fEnemyPosX);
				int nTeamNameY = (int)(fMiniMapY + fEnemyPosY);
				m_pFontPosition->DrawText(nTeamNameX,
											nTeamNameY,
											GUI_FONT_COLOR,
											TeamName);
				// end 2012-05-22 by isshin 레이더 및 미니맵 파티,여단원 표시
			}
		}

	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  party
#ifdef FREEWAR_
	if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
#endif
	vector<PARTYENEMYINFO*>::iterator itParty = g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.begin();
	while (itParty != g_pShuttleChild->m_pClientParty->m_vecPartyEnemyInfo.end())
	{
		if ((*itParty)->m_pEnemyData)
		{
			D3DXVECTOR2 vPartyVel;
			float fPartyAngle;
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			//char EnemyName[SIZE_MAX_CHARACTER_NAME];
			char EnemyName[SIZE_MAX_ARENA_FULL_NAME];
			memset(EnemyName, 0x00, SIZE_MAX_ARENA_FULL_NAME);

			vPartyVel.x = (*itParty)->m_pEnemyData->m_vVel.x;
			vPartyVel.y = (*itParty)->m_pEnemyData->m_vVel.z;
			wsprintf(EnemyName, "%s", (*itParty)->m_ImPartyMemberInfo.CharacterName);

			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), EnemyName);

			D3DXVec2Normalize(&vPartyVel, &vPartyVel);
			fPartyAngle = ACOS(D3DXVec2Dot(&vPartyVel, &(D3DXVECTOR2(0, 1))));
			if (vPartyVel.x > 0)
				fPartyAngle *= -1;

			float fEnemyPosX = 0.0f;
			float fEnemyPosY = 0.0f;

			if (g_pD3dApp->m_bCharacter)
			{
				GetMiniBigMapCityPos((*itParty)->m_pEnemyData->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}

			if (g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				fEnemyPosX = 0.0f;
				fEnemyPosY = 0.0f;
				GetMiniBigMapPos((*itParty)->m_pEnemyData->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}

			if (fEnemyPosX >= 0 &&
				fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
				fEnemyPosY >= 0 &&
				fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
			{

				m_pMiniMapPartyImage->Move(fMiniMapX + fEnemyPosX,
					fMiniMapY + fEnemyPosY);
				m_pMiniMapPartyImage->Rotate(UNIT_WIDTH / 2, UNIT_HEIGHT / 2, fPartyAngle);
				m_pMiniMapPartyImage->Render();


				// 2006-08-18 by dgwoo 파티원들간의 아이디를 표시
				int nPartyNameX = (int)(fMiniMapX + fEnemyPosX);
				int nPartyNameY = (int)(fMiniMapY + fEnemyPosY);
				m_pFontPosition->DrawText(nPartyNameX,
					nPartyNameY,
					GUI_FONT_COLOR,
					EnemyName);
			}
		}
		itParty++;
#ifdef FREEWAR_
	}
#endif
	}

	// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 빅맵 여단원 표시
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Guild
#ifdef FREEWAR_
	if (!isFreeForAllMap(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
	{
#endif
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		PARTYENEMYINFO*	pPartyEnemyInfo = g_pShuttleChild->m_pClientParty->PartyFindMemberByClientIndex(itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex);
		structGuildInfo* pGuildEnemyInfo = g_pGameMain->m_pCommunity->GetGuild()->GetGuildMemberInfo(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber);
		
		if(!pPartyEnemyInfo && pGuildEnemyInfo)
		{
			D3DXVECTOR2 vGuildVel;
			float fGuildAngle;
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			//char EnemyName[SIZE_MAX_CHARACTER_NAME];
			char EnemyName[SIZE_MAX_ARENA_FULL_NAME];
			memset(EnemyName, 0x00, SIZE_MAX_ARENA_FULL_NAME);
			
			vGuildVel.x = itEnemy->second->m_vVel.x;
			vGuildVel.y = itEnemy->second->m_vVel.z;
			//wsprintf(EnemyName,"%s",(*itParty)->m_ImPartyMemberInfo.CharacterName);
			wsprintf(EnemyName,"%s",itEnemy->second->GetEnemyInfo().CharacterInfo.CharacterName);
			// 2008-02-26 by bhsohn 통합 아레나 관련 버그 수정
			g_pD3dApp->ConevertArenaRenderUserName(g_pD3dApp->GetArenaState(), EnemyName);
			
			D3DXVec2Normalize(&vGuildVel,&vGuildVel);
			fGuildAngle = ACOS(D3DXVec2Dot(&vGuildVel,&(D3DXVECTOR2(0,1))));
			if(vGuildVel.x > 0 )
				fGuildAngle  *= -1;
			
			float fEnemyPosX = 0.0f;
			float fEnemyPosY = 0.0f;
			
			if(g_pD3dApp->m_bCharacter)
			{
				GetMiniBigMapCityPos(itEnemy->second->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				fEnemyPosX = 0.0f;
				fEnemyPosY = 0.0f;
				GetMiniBigMapPos(itEnemy->second->m_vPos, &fEnemyPosX, &fEnemyPosY);
			}
			
			if(fEnemyPosX >= 0 &&
				fEnemyPosX <= BIGMAP_WIDTH - UNIT_WIDTH &&
				fEnemyPosY >= 0 &&
				fEnemyPosY <= BIGMAP_HEIGHT - UNIT_HEIGHT)
			{
				
				m_pMiniMapGuildImage->Move(fMiniMapX + fEnemyPosX, 
					fMiniMapY + fEnemyPosY);
				m_pMiniMapGuildImage->Rotate(UNIT_WIDTH/2, UNIT_HEIGHT/2, fGuildAngle);
				m_pMiniMapGuildImage->Render();
				
				
				// 2006-08-18 by dgwoo 파티원들간의 아이디를 표시
				int nGuildNameX = (int)(fMiniMapX + fEnemyPosX);
				int nGuildNameY = (int)(fMiniMapY + fEnemyPosY);
				m_pFontPosition->DrawText(nGuildNameX,
					nGuildNameY,
					GUI_FONT_COLOR,
					EnemyName);
			}
		}
		itEnemy++;
#ifdef FREEWAR_
	}
#endif
	}// end 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 빅맵 여단원 표시

	
	//////////////////////////////////////////////////////////////////////////
	// 2007-02-26 by dgwoo 빅맵에 퀘스트몬스터 위치를 표시한다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 퀘스트 수행지역
	vector<MiniMap_PosData*>::iterator	it  = m_vecMosterPosition.begin();
	//vector<CD3DHanFont*>::iterator		ith = m_vecQuestPosition.begin();

	while(it != m_vecMosterPosition.end())
	{
		POINT pt = m_pImgQuestTarget->GetImgSize();
		int tempX = ((*it)->nMonsterPosX - ( pt.x / 2));
		int tempY = ((*it)->nMonsterPosY - ( pt.y / 2));

		//int length = (strlen((*it)->sQuestMonsterName))/2;
		SIZE length = m_pFontQuestTarget->GetStringSize((*it)->sQuestMonsterName);
		m_pFontQuestTarget->DrawText(fMiniMapX + (*it)->nMonsterPosX - (length.cx/2),
										fMiniMapY + (*it)->nMonsterPosY,
										GUI_FONT_COLOR_W,
										(*it)->sQuestMonsterName,
										0L);

		
		m_pImgQuestTarget->Move(fMiniMapX + tempX,
									fMiniMapY + tempY);
		m_pImgQuestTarget->Render();
		
//		D3DXMATRIX temppos;// = g_pShuttleChild->m_mMatrix;
//		D3DXMatrixIdentity(&temppos);
//		D3DXMatrixScaling(&matScale,0.005f,0.005f,0.005f);
//		
//		temppos._41 = (*it)->nMonsterPosX;
//		temppos._43 = (*it)->nMonsterPosY;
//
//		D3DXMATRIX matTemp1 = temppos * matScale;
//		mat = temppos;
//		mat._41 = matTemp1._41;
//		//mat._42 = 2;
//		mat._43 = matTemp1._43;
//		D3DXMatrixScaling(&matScale,2.05f,2.05f,2.05f);
//		mat = matScale * mat * matTrans;	
//		m_pMeshArea->SetWorldMatrix(mat);
//		m_pMeshArea->AnotherTexture(1);
//		m_pMeshArea->Render();
		// 2005-01-04 by jschoi
//			g_pD3dDev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE );
//			g_pD3dDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);

//		D3DXVECTOR3 vcPos(0,0,0);
//		int nX, nY, nW;
//		int length = (strlen((*it)->sQuestMonsterName)*6)/2;
//		D3DXVec3TransformCoord( &vcPos, &vcPos, &mat);
//		g_pD3dApp->CalcObjectSourceScreenCoords(vcPos, g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height, 
//			nX, nY, nW);			
//		m_vecQuestPosition[0]->DrawText(nX-length,nY-30,GUI_FONT_COLOR_Y,(*it)->sQuestMonsterName, 0L);
		
		it++;
//		ith++;
	}
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////


}

void CINFGameMainMiniMap::RenderMonster()
{
	int nOtherPosX,nOtherPosY;
	D3DXVECTOR2 vPos, vAnother;
	vPos.x = g_pD3dApp->m_pShuttleChild->m_vPos.x;
	vPos.y = g_pD3dApp->m_pShuttleChild->m_vPos.z;
	float fRangeTemp;

	if(g_pD3dApp->m_bCharacter)
	{
		fRangeTemp = 170;
	}
	if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		fRangeTemp = m_fRange;
	}

	CMapMonsterIterator itMonster = g_pScene->m_mapMonsterList.begin();
	while(itMonster != g_pScene->m_mapMonsterList.end())
	{
		float fLength = D3DXVec3Length(&(g_pShuttleChild->m_vPos - itMonster->second->m_vPos));
		if( fLength < fRangeTemp &&											// 2005-08-16 by ispark
			ATTACK_AVAILABLE_STATE(itMonster->second->m_dwState))
		{
			// 몬스터가 유닛보다 위에 있는지 아래 있는지 검사해서 이미지 인덱스를 결정한다.
			int index = 0;
			if(g_pShuttleChild->m_vPos.y < itMonster->second->m_vPos.y-MINI_MARK_HEIGHT)
				index = 1;
			else if(g_pShuttleChild->m_vPos.y > itMonster->second->m_vPos.y+MINI_MARK_HEIGHT)
				index = 2;
			// 몬스터와 유닛의 상대 좌표를 계산해서 유닛과 같이 회전하고 화면상 위치를 잡는다.
			vAnother.x = itMonster->second->m_vPos.x;
			vAnother.y = itMonster->second->m_vPos.z;
			D3DXVECTOR2 vVel;
			D3DXVec2Normalize(&vVel, &(vPos - vAnother));
			vVel = vVel * ( (float)RENDER_RADIUS * (fLength/fRangeTemp) );	// 2005-08-16 by ispark
			D3DXVECTOR3 vDistance = D3DXVECTOR3(vVel.x, 0, vVel.y);
			D3DXVec3TransformCoord(&vDistance,&vDistance,&m_matRotate);

			nOtherPosX = UNIT_CENTER_POS_X - vDistance.x;
			nOtherPosY = UNIT_CENTER_POS_Y + vDistance.z;

			// 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)

// 			m_pMonsterImage[index]->Move(nOtherPosX, nOtherPosY);
// 			m_pMonsterImage[index]->Render();
			if( ((*itMonster).second)->m_pMonsterInfo->Belligerence != BELL_INFINITY_DEFENSE_MONSTER )
			{
				m_pMonsterImage[index]->Move(nOtherPosX, nOtherPosY);
				m_pMonsterImage[index]->Render();
			}
			else
			{
				m_pUserImage[index]->Move(nOtherPosX, nOtherPosY);
				m_pUserImage[index]->Render();
			}

			// end 2010. 03. 03 by ckPark 인피니티 필드 2차(입장 UI변경)
			//////////강제락온 그리기///////////////////
			if(g_pD3dApp->m_pShuttleChild->m_pOrderTarget == itMonster->second)
			{
				if(index == 0)
				{
					m_pLockonImage->Move( nOtherPosX, nOtherPosY);
				}
				else if(index == 1)
				{
					m_pLockonImage->Move( nOtherPosX, nOtherPosY-3);
				}
				else if(index == 2)
				{
					m_pLockonImage->Move( nOtherPosX, nOtherPosY+3);
				}
				m_pLockonImage->Render();
			}
		}
		itMonster++;
	}
}

void CINFGameMainMiniMap::RenderEnemy()
{
	int nOtherPosX,nOtherPosY;
	D3DXVECTOR2 vPos, vAnother;
	vPos.x = g_pD3dApp->m_pShuttleChild->m_vPos.x;
	vPos.y = g_pD3dApp->m_pShuttleChild->m_vPos.z;
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	float fRangeTemp;

	if(g_pD3dApp->m_bCharacter)
	{
		fRangeTemp = 170;		// 캐릭터는 레이더 거리랑 관계없이 강제 셋팅
	}
	if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		fRangeTemp = m_fRange;
	}

	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		float fLength = D3DXVec3Length(&(g_pShuttleChild->m_vPos - itEnemy->second->m_vPos));
		if( fLength < fRangeTemp && itEnemy->second->m_bSetPosition == TRUE)
		{
			// 2006-11-29 by dgwoo 투명인 상태에는 레이다에서 표시를 빼준다.
			//if(itEnemy->second->m_infoCharacter.CharacterRenderInfo.RI_Invisible == TRUE && 
			// 2007-02-08 by dgwoo 안보이는 유저만 레이다에서 빼준다.
			if(itEnemy->second->m_nAlphaValue == SKILL_OBJECT_ALPHA_OTHER_INFLUENCE &&
#ifdef FREEWAR_
				(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex) ||	// 상대편 이면
#else
				(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType) ||	// 상대편 이면
#endif
				itEnemy->second->IsPkEnable()))
			{
				itEnemy++;
				continue;
			}

			int index = 0;
			if(g_pShuttleChild->m_vPos.y < itEnemy->second->m_vPos.y-MINI_MARK_HEIGHT)
				index = 1;
			else if(g_pShuttleChild->m_vPos.y > itEnemy->second->m_vPos.y+MINI_MARK_HEIGHT)
				index = 2;

			vAnother.x = itEnemy->second->m_vPos.x;
			vAnother.y = itEnemy->second->m_vPos.z;
			D3DXVECTOR2 vVel;
			D3DXVec2Normalize(&vVel, &(vPos - vAnother));
			vVel = vVel * ( (float)RENDER_RADIUS * (fLength/fRangeTemp) );
			D3DXVECTOR3 vDistance = D3DXVECTOR3(vVel.x, 0, vVel.y);
			D3DXVec3TransformCoord(&vDistance,&vDistance,&m_matRotate);

			nOtherPosX = UNIT_CENTER_POS_X - vDistance.x;
			nOtherPosY = UNIT_CENTER_POS_Y + vDistance.z;

			// 2008-09-26 by bhsohn 신규 인첸트 처리			
			{
				int nMinPosX = BACK_START_X + 25 ;
				int nMaxPosX = BACK_START_X + BACK_IMAGE_SIZE -25;
				if(nOtherPosX < nMinPosX )
				{				
					nOtherPosX = nMinPosX;
				}
				else if(nOtherPosX > nMaxPosX )
				{
					nOtherPosX = nMaxPosX;
				}
			}
			{
				int nMinPosY = BACK_START_Y + 25;
				int nMaxPosY = BACK_START_Y + BACK_IMAGE_SIZE - 23;
				if(nOtherPosY < BACK_START_Y )
				{				
					nOtherPosY = BACK_START_Y;
				}
				else if(nOtherPosY > nMaxPosY )
				{
					nOtherPosY = nMaxPosY;
				}
			}
			// end 2008-09-26 by bhsohn 신규 인첸트 처리

			// 레이다에 파티원 그리기
			PARTYENEMYINFO*	pPartyEnemyInfo = g_pShuttleChild->m_pClientParty->PartyFindMemberByClientIndex(itEnemy->second->m_infoCharacter.CharacterInfo.ClientIndex);
			structGuildInfo* pGuildEnemyInfo = g_pGameMain->m_pCommunity->GetGuild()->GetGuildMemberInfo(itEnemy->second->m_infoCharacter.CharacterInfo.CharacterUniqueNumber);// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 여단원 채크
			if(pPartyEnemyInfo)
			{// 파티원인 경우
				m_pPartyImage[index]->Move(nOtherPosX, nOtherPosY);
				m_pPartyImage[index]->Render();
			}
			// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 여단원인 경우
			else if(!pPartyEnemyInfo && pGuildEnemyInfo)
			{
				m_pGuildImage[index]->Move(nOtherPosX, nOtherPosY);
				m_pGuildImage[index]->Render();
			}// end 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 여단원인 경우
			else
			{// 파티원이 아닌경우
//				if(g_pShuttleChild->IsInfluenceWarType(itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType) == TRUE)
#ifdef FREEWAR_
				if (!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType, 1, g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex))
#else
				if(!IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType, itEnemy->second->m_infoCharacter.CharacterInfo.InfluenceType))
#endif
				{
					m_pInfluenceImage[index]->Move(nOtherPosX, nOtherPosY);
					m_pInfluenceImage[index]->Render();	
				}
				else
				{
					m_pUserImage[index]->Move(nOtherPosX, nOtherPosY);
					m_pUserImage[index]->Render();
				}					
			}
			
			//////////강제락온 그리기///////////////////
			if(g_pD3dApp->m_pShuttleChild->m_pTarget == itEnemy->second)
			{
				if(index == 0)
				{
					m_pLockonImage->Move( nOtherPosX, nOtherPosY);
				}
				else if(index == 1)
				{
					m_pLockonImage->Move( nOtherPosX, nOtherPosY-3);
				}
				else if(index == 2)
				{
					m_pLockonImage->Move( nOtherPosX, nOtherPosY+3);
				}
				m_pLockonImage->Render();
			}
		}
		itEnemy++;
	}
}

void CINFGameMainMiniMap::Tick()
{
	if(!g_pGround)				//2013-05-22 by ssjung 캐나다 익셉션 오류 예방 코드 추가
		return;

	// 유닛 좌표가 Z축에 각도 얼마나 기울어져 있는가? 그리고 그 매트릭스 구하기
	D3DXVECTOR2 vUnitVel;
	vUnitVel.x = g_pShuttleChild->m_vVel.x;
	vUnitVel.y = g_pShuttleChild->m_vVel.z;
	D3DXVec2Normalize(&vUnitVel,&vUnitVel);
	m_fAngle = ACOS(D3DXVec2Dot(&vUnitVel,&(D3DXVECTOR2(0,1))));
	if(vUnitVel.x > 0 )
		m_fAngle *= -1;
	D3DXMatrixRotationY(&m_matRotate, m_fAngle);

	// 레이더 본체 각도
	m_fRadarBackAngle = ACOS(D3DXVec2Dot(&vUnitVel,&(D3DXVECTOR2(0,-1))));
	if(vUnitVel.x > 0 )
		m_fRadarBackAngle *= -1;
	// 2005-09-13 by ispark 별도 처리
	if(m_bZoomState)
	{
		m_fCurrentTime += g_pD3dApp->GetElapsedTime()*UNIT_ANIMATION_TICK_TIME;
		SetMonsterMiniMap();
	}

	// 레이더 미니맵
	// 2006-09-04 by ispark, 미니맵 상태 체크
	if((g_pD3dApp->m_bCharacter && (g_pGround->m_pMiniBigMapCityImage || g_pGround->m_pMiniBigMapImage)) ||
		(g_pD3dApp->m_bCharacter == FALSE && g_pGround->m_pMiniBigMapImage))
	{
		m_bMiniMapState = TRUE;
	}
	else
	{
		m_bMiniMapState = FALSE;
	}

	TickRadarMap();
}

int CINFGameMainMiniMap::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//			if(GetButtonStateOnMouse(pt, BUTTON_SHOW_START_X, BUTTON_SHOW_START_Y, BUTTON_SHOW_SIZE_X, BUTTON_SHOW_SIZE_Y))
//			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
//			}
//			else 
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경			
			m_pServerTimeBtn->OnMouseMove(pt);
#endif

			if(GetButtonStateOnMouse(pt, BUTTON_ZOOM_START_X,BUTTON_ZOOM_START_Y, BUTTON_ZOOM_SIZE_X, BUTTON_ZOOM_SIZE_Y))
			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_UP;
				m_nZoomButtonState = BUTTON_STATE_UP;
			}
			else
			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
				m_nZoomButtonState = BUTTON_STATE_NORMAL;
			}
			//--------------------------------------------------------------------------//
			// 2006-08-25 by dgwoo 아이템 정보창이 뜨는것을 막는다.
			CINFImage* pMiniBigMapImageTEMP = NULL;
			if(g_pD3dApp->m_bCharacter)
			{
				pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
			}
			if(pMiniBigMapImageTEMP == NULL || g_pD3dApp->m_bCharacter == FALSE)
			{
				pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
			}
// 2007-02-22 by dgwoo
//			if(pMiniBigMapImageTEMP)
//			{
//				if(pt.x > BIGMAP_START_X - (UNIT_WIDTH/2) && pt.x < BIGMAP_START_X + BIGMAP_WIDTH + (UNIT_WIDTH/2) &&
//					pt.y > BIGMAP_START_Y - (UNIT_HEIGHT/2)&& pt.y < BIGMAP_START_Y + BIGMAP_HEIGHT + (UNIT_HEIGHT/2) &&
//					m_bZoomState)
//				{
//					return INF_MSGPROC_BREAK;
//				}
//			}
			//--------------------------------------------------------------------------//
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			

			// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//			if(GetButtonStateOnMouse(pt, BUTTON_SHOW_START_X, BUTTON_SHOW_START_Y, BUTTON_SHOW_SIZE_X, BUTTON_SHOW_SIZE_Y))
//			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_DOWN;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
//				return INF_MSGPROC_BREAK;
//			}
//			else 
			
			{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if(TRUE == m_pServerTimeBtn->OnLButtonDown(pt))
				{
					// 버튼위에 마우스가 있다.
					return  INF_MSGPROC_BREAK;
				}		
#endif
			}
			if(GetButtonStateOnMouse(pt, BUTTON_ZOOM_START_X,BUTTON_ZOOM_START_Y, BUTTON_ZOOM_SIZE_X, BUTTON_ZOOM_SIZE_Y))
			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_DOWN;
				m_nZoomButtonState = BUTTON_STATE_DOWN;
				return INF_MSGPROC_BREAK;
			}
			else
			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
				m_nZoomButtonState = BUTTON_STATE_NORMAL;
			}

			if(m_bZoomState)
			{
				float fMiniMapX, fMiniMapY;
				fMiniMapX = (g_pD3dApp->GetBackBufferDesc().Width / 2) - 256;
				fMiniMapY = (g_pD3dApp->GetBackBufferDesc().Height / 2) - 256;
				CINFImage* pMiniBigMapImageTEMP = NULL;
				if(g_pD3dApp->m_bCharacter)
				{
					pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
				}
				
				if(pMiniBigMapImageTEMP == NULL || g_pD3dApp->m_bCharacter == FALSE)
				{
					pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
				}
// 2007-02-21 by dgwoo 작은 맵으로 소스 수정중 필요없는 부분으로 보임.
//				if(pMiniBigMapImageTEMP)
//				{
//					POINT pMap = pMiniBigMapImageTEMP->GetImgSize();
//					if( pt.x >= fMiniMapX && pt.x <=fMiniMapX+pMap.x 
//						&& pt.y >= fMiniMapY && pt.y <=fMiniMapY+pMap.y)
//					{
//						g_pInterface->SetWindowOrder(WNDMiniMap);
//						return INF_MSGPROC_BREAK;
//					}
//				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//			if(GetButtonStateOnMouse(pt, BUTTON_SHOW_START_X, BUTTON_SHOW_START_Y, BUTTON_SHOW_SIZE_X, BUTTON_SHOW_SIZE_Y))
//			{
//				if(m_nButtonState[BUTTON_SHOWHIDE] == BUTTON_STATE_DOWN)
//				{
//					OnButtonClicked(BUTTON_SHOWHIDE);
//				}
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
//				// 2008-03-27 by dgwoo 무기를 발사하는중에 이벤트 발생시 총알이 계속 나간다.
//				//return INF_MSGPROC_BREAK;
//				return INF_MSGPROC_NORMAL;
//			}
//			else 

			{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if(TRUE == m_pServerTimeBtn->OnLButtonUp(pt))
				{	
					OnClickServerTime();
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
					return  INF_MSGPROC_BREAK;
				}
#endif
			}
			if(GetButtonStateOnMouse(pt, BUTTON_ZOOM_START_X,BUTTON_ZOOM_START_Y, BUTTON_ZOOM_SIZE_X, BUTTON_ZOOM_SIZE_Y))
			{
				// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//				if(m_nButtonState[BUTTON_ZOOM] == BUTTON_STATE_DOWN)
				if(m_nZoomButtonState == BUTTON_STATE_DOWN)
				{
					OnButtonClicked(BUTTON_ZOOM);
				}				
				m_nZoomButtonState = BUTTON_STATE_UP;
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_UP;
				// 2008-03-27 by dgwoo 무기를 발사하는중에 이벤트 발생시 총알이 계속 나간다.
				//return INF_MSGPROC_BREAK;
				return INF_MSGPROC_NORMAL;
			}
			else
			{
//				m_nButtonState[BUTTON_SHOWHIDE] = BUTTON_STATE_NORMAL;
//				m_nButtonState[BUTTON_ZOOM] = BUTTON_STATE_NORMAL;
				m_nZoomButtonState = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_KEYDOWN:
		{
			// 2005-04-07 by jschoi - Tutorial
			if(g_pTutorial->IsTutorialMode() == TRUE)
				return INF_MSGPROC_NORMAL;

			if((wParam == VK_F4 || 
				lParam == WM_KEYDOWN_LPARAM_M) && 
				g_pD3dApp->m_bChatMode==FALSE &&
				g_pD3dApp->m_dwGameState != _SHOP)
			{
				//m_bZoomState = (m_bZoomState+1)%3;
				OnButtonClicked(BUTTON_ZOOM);
				if(m_bZoomState)
				{
					vector<MiniMap_PosData*>::iterator itq = m_vecMosterPosition.begin();				
					while(itq != m_vecMosterPosition.end())				
					{
						util::del(*itq);				
						itq++;				
					}				
					g_pInterface->SetWindowOrder(WNDMiniMap);
					m_vecMosterPosition.clear();	
					SetMonsterMiniMap();								
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_OK, D3DXVECTOR3(0,0,0), FALSE);
				}
				else
				{
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_CANCEL, D3DXVECTOR3(0,0,0), FALSE);
				}
				
			}
			if(m_bZoomState == BUTTON_ZOOM_IN				// 큰맵일때 
				&& wParam == VK_ESCAPE)						// esc키를 먹게한다.
			{
				m_bZoomState = 0;
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_CANCEL, D3DXVECTOR3(0,0,0), FALSE);
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainMiniMap::InitMapSet()
/// \brief		작은 맵인지 큰맵인지 구분하여 새로 셋팅해준다.
/// \author		dgwoo
/// \date		2007-02-22 ~ 2007-02-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::InitMapSet()
{
	CINFImage* pMiniBigMapImageTEMP = NULL;
	if(g_pD3dApp->m_bCharacter)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
	}
	else
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
	}
	if(pMiniBigMapImageTEMP)			
	{
		if(m_bZoomState == BUTTON_ZOOM_IN)
		{
			pMiniBigMapImageTEMP->SetScale(1.0f,1.0f);
			m_pMiniMapUnitImage->SetScale(1.f,1.f);
			m_pMiniMapPartyImage->SetScale(1.f,1.f);
			m_pArenaMapInfImg->SetScale(1.f,1.f);
			m_pImgQuestTarget->SetScale(1.f,1.f);
			m_pMiniMapGuildImage->SetScale(1.f,1.f);							// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
		}
		else if(m_bZoomState == BUTTON_ZOOM_OUT)
		{
			pMiniBigMapImageTEMP->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
			m_pMiniMapUnitImage->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
			m_pMiniMapPartyImage->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
			m_pArenaMapInfImg->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
			m_pImgQuestTarget->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
			m_pMiniMapGuildImage->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);		// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
		}
	}
}

void CINFGameMainMiniMap::OnButtonClicked(int nButton)
{
	switch(nButton)
	{
	case BUTTON_ZOOM:
		{
			m_bZoomState = (m_bZoomState+1)%3;
			
			CINFImage* pMiniBigMapImageTEMP = NULL;
			if(g_pD3dApp->m_bCharacter)
			{
				pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
			}
			else
			{
				pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
			}
			if(pMiniBigMapImageTEMP)			
			{
				if(m_bZoomState == BUTTON_ZOOM_IN)
				{
					pMiniBigMapImageTEMP->SetScale(1.0f,1.0f);
					m_pMiniMapUnitImage->SetScale(1.f,1.f);
					m_pMiniMapPartyImage->SetScale(1.f,1.f);
					m_pArenaMapInfImg->SetScale(1.f,1.f);
					m_pImgQuestTarget->SetScale(1.f,1.f);
					m_pMiniMapGuildImage->SetScale(1.f,1.f);							// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
				}
				else if(m_bZoomState == BUTTON_ZOOM_OUT)
				{
					pMiniBigMapImageTEMP->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
					m_pMiniMapUnitImage->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
					m_pMiniMapPartyImage->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
					m_pArenaMapInfImg->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
					m_pImgQuestTarget->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);
					m_pMiniMapGuildImage->SetScale(SMALLMAP_SCALE,SMALLMAP_SCALE);		// 2012-04-09 by isshin 레이더 및 미니맵 파티,여단원 표시 - 맵 여단원 표시
				}
			}
		}
		break;
		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
//	case BUTTON_SHOWHIDE:
//		{
//			m_bShowState = !m_bShowState;
//		}
//		break;
	}
}

void CINFGameMainMiniMap::SetMonsterMiniMap()
{
	MiniMap_PosData		*MonsterPos  = NULL;
	MEX_MONSTER_INFO	*monstername = NULL;
	m_vecMosterPosition.clear();
	map<int, MEX_QUEST_INFO*>::iterator it = g_pQuestData->m_mapCharacterQuest.begin();	
	while(it != g_pQuestData->m_mapCharacterQuest.end())
	{
		if(it->second->QuestState == QUEST_STATE_IN_PROGRESS )
		{
			//CQuest* FindQuest
			CQuest* quest_temp = g_pQuestData->FindQuest(it->second->QuestIndex);
			vector<ITEM_W_COUNT_MAP_AREA>::iterator it = quest_temp->TargetMonsterItemVector.begin();
			while(it != quest_temp->TargetMonsterItemVector.end())
			{
				if(it->MapArea.MapIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)
				{
					if(it->MapArea.X > 0 && it->MapArea.Y > 0)
					{
						monstername = g_pDatabase->CheckMonsterInfo(it->MonsterUniqueNumber);
						
						MonsterPos = new MiniMap_PosData;
						MonsterPos->nQuestIndex = quest_temp->QuestIndex;
						MonsterPos->nMonsterPosX = 510 - (it->MapArea.X * 2);
						MonsterPos->nMonsterPosY = (it->MapArea.Y * 2);
						memset(MonsterPos->sQuestMonsterName,0x00, sizeof(MonsterPos->sQuestMonsterName[0])*64);
						wsprintf(MonsterPos->sQuestMonsterName,"%s",monstername->MonsterName );
						m_vecMosterPosition.push_back(MonsterPos);
						
//						CD3DHanFont*	Temphan;
//						Temphan = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
//						Temphan->InitDeviceObjects(g_pD3dDev);
//						Temphan->RestoreDeviceObjects();
//						m_vecQuestPosition.push_back(Temphan);
					}				
				}
				it++;
			}			
			vectMONSTER_W_COUNT::iterator itr =  quest_temp->TargetMonsterVector.begin();
			for(;itr != quest_temp->TargetMonsterVector.end();itr++)
			{
				if(itr->MapArea.MapIndex == g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex)
				{		
					if(itr->MapArea.X > 0 && itr->MapArea.Y > 0)
					{
						monstername = g_pDatabase->CheckMonsterInfo(itr->MonsterUniqueNumber);
						MonsterPos = new MiniMap_PosData;
						MonsterPos->nQuestIndex = quest_temp->QuestIndex;
						MonsterPos->nMonsterPosX = 510 - (itr->MapArea.X * 2);
						MonsterPos->nMonsterPosY = (itr->MapArea.Y * 2);
						memset(MonsterPos->sQuestMonsterName, 0x00, sizeof(MonsterPos->sQuestMonsterName[0])*64);
						wsprintf(MonsterPos->sQuestMonsterName,"%s",monstername->MonsterName);
						m_vecMosterPosition.push_back(MonsterPos);
					}
				}

			}
		}
		
		it++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainMiniMap::CheckRadar()
/// \brief		레이더 체크
/// \author		ispark
/// \date		2005-08-16 ~ 2005-08-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::CheckRadar()
{
	if(g_pShuttleChild->m_pRadarItemInfo == NULL)
	{
		m_fRange = 0.0f;
	}
	else
	{
		// 2008-09-26 by bhsohn 신규 인첸트 처리
		//m_fRange = (float)g_pShuttleChild->m_pRadarItemInfo->ItemInfo->Range;
		float fEnchantInfo = g_pShuttleChild->GetEnchantDesParam(g_pShuttleChild->m_pRadarItemInfo, 
			DES_RADAR_OBJECT_DETECT_RANGE);
		m_fRange = (float)g_pShuttleChild->m_pRadarItemInfo->ItemInfo->Range;
		if(fEnchantInfo > 0)
		{
			m_fRange += fEnchantInfo;
		}
		// end 2008-09-26 by bhsohn 신규 인첸트 처리
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainMiniMap::RenderRadarMap()
/// \brief		레이더 미니맵 렌더링
/// \author		ispark
/// \date		2005-09-16 ~ 2005-09-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::RenderRadarMap()
{
	D3DXVECTOR2 vUnitVel;
	float fAngle;
	vUnitVel.x = g_pShuttleChild->m_vVel.x;
	vUnitVel.y = g_pShuttleChild->m_vVel.z;
	D3DXVec2Normalize(&vUnitVel,&vUnitVel);
	fAngle = ACOS(D3DXVec2Dot(&vUnitVel,&(D3DXVECTOR2(0,-1))));
	if(vUnitVel.x > 0 )
		fAngle *= -1;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx* pImageTEMP = NULL;
	if( g_pD3dApp->m_bCharacter )
	{
		pImageTEMP = g_pGround->m_pMiniBigMapCityImage;
	}
	
	if(pImageTEMP == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		pImageTEMP = g_pGround->m_pMiniBigMapImage;
	}
	
	if( m_pRadarMiniMap && pImageTEMP )
	{
		m_pRadarMiniMap->Move( SMALLMAP_START_X - ( m_pRadarMiniMap->GetImgSize().x * 1.1f - pImageTEMP->GetImgSize().x * SMALLMAP_SCALE ) / 2, RADAR_MINIMAP_Y - 2 );
		m_pRadarMiniMap->Rotate( m_pRadarMiniMap->GetImgSize().x * 1.1f/2.0f, m_pRadarMiniMap->GetImgSize().y * 1.1f/2.0f, fAngle);
		m_pRadarMiniMap->SetScale(1.1f, 1.1f);
		m_pRadarMiniMap->Render();
	}
#else 
	m_pRadarMiniMap->Move( RADAR_MINIMAP_X, RADAR_MINIMAP_Y );
	m_pRadarMiniMap->Rotate( RADAR_MINIMAP_SIZE/2.0f, RADAR_MINIMAP_SIZE/2.0f, fAngle);
	m_pRadarMiniMap->Render();
#endif
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainMiniMap::InitRadarMap()	
/// \brief		레이더 미니맵 초기화
/// \author		ispark
/// \date		2005-09-21 ~ 2005-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
HRESULT CINFGameMainMiniMap::InitRadarMap()
{
	CGameData AlphaData;
	AlphaData.SetFile( ".\\Res-Tex\\mini.tex", FALSE, NULL, 0, FALSE );
	DataHeader *pDataHeader = AlphaData.FindFromFile("alpha");
	if(pDataHeader)
	{
		BYTE *pTexData = (BYTE*)pDataHeader->m_pData + 18;
		for(int i = 0; i < RADAR_MINIMAP_SIZE; i++)
		{
			for(int j = 0; j < RADAR_MINIMAP_SIZE; j++)
			{
				m_byAlphaMiniMap[i][j] = pTexData[(i * 118 * 4) + (j * 4) + 3];
			}
		}

	}

	m_pRadarMiniMap->RestoreDeviceObjectsEx(RADAR_MINIMAP_SIZE, RADAR_MINIMAP_SIZE);

	util::del(pDataHeader);

	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainMiniMap::TickRadarMap()
/// \brief		레이더 미니맵 실시간 생성
/// \author		ispark
/// \date		2005-09-23 ~ 2005-09-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::TickRadarMap()
{
	// 레이더를 착용하지 않았다면 기본 레이더 바탕색을 칠한다.
	// 기본 바탕색 RGB(100, 117, 117)
	// 2006-08-29 by dgwoo g_pD3dApp->m_bBazaar추가
	// 2006-09-04 by ispark, g_pD3dApp->m_bBazaar 삭제, m_bMiniMapState로 변경
	if(m_fRange <= 0 || g_pGround->m_bMiniMap == FALSE || m_bMiniMapState == FALSE)
	{
		D3DLOCKED_RECT CopyTexRect;
		HRESULT hr = 0;
		hr = m_pRadarMiniMap->GetTexture()->LockRect(0, &CopyTexRect, 0, 0);

		BYTE* pCopyRow = (BYTE*)CopyTexRect.pBits;
		for(int i = 0; i < RADAR_MINIMAP_SIZE; i++)
		{
			for(int j = 0; j < RADAR_MINIMAP_SIZE; j++)
			{
				*pCopyRow++ = 117;			// B
				*pCopyRow++ = 117;			// G
				*pCopyRow++ = 100;			// R
				*pCopyRow++ = m_byAlphaMiniMap[i][j];
			}
		}

		m_pRadarMiniMap->GetTexture()->UnlockRect(0);
		return;
	}

	// 맵 범위를 벗어난 위치라면 필요한 부분만 텍스쳐를 가져오고 나머지는 
	// 다른색으로 채운다. 그 작업을 밑 소스에서 하고 있다.

	D3DLOCKED_RECT TexRect, CopyTexRect;
	RECT rt, rtReal;
	int nRadius;
	float fIndex, fRangeTemp;

	if(g_pD3dApp->m_bCharacter)
	{
		fRangeTemp = 1100;							// 캐릭터는 레이더 거리랑 관계없이 강제 셋팅
	}
	if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		fRangeTemp = m_fRange;
	}

	nRadius = (int)(fRangeTemp / 20);						// 레이더 범위 반지름
	fIndex = (nRadius * 2) / RADAR_MINIMAP_SIZE;		// 맵 텍스쳐 찍을 인덱스 증가값

	float fMyPosX = 0.0f;
	float fMyPosY = 0.0f;
	if(g_pD3dApp->m_bCharacter)
	{
		GetMiniBigMapCityPos(g_pShuttleChild->m_vPos, &fMyPosX, &fMyPosY);
	}

	if(g_pGround->m_pMiniBigMapCityImage == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		fMyPosX = 0.0f;
		fMyPosY = 0.0f;
		GetMiniBigMapPos(g_pShuttleChild->m_vPos, &fMyPosX, &fMyPosY);
	}
	
	rt.left = fMyPosX - nRadius + 20;
	rt.top = fMyPosY - nRadius + 20;
	rt.right = fMyPosX + nRadius + 20;
	rt.bottom = fMyPosY + nRadius + 20;
	rtReal = rt;

	if(0 > rt.left)
		rt.left = 0;
	if(0 > rt.top)
		rt.top = 0;
	if(512 < rt.right)
		rt.right = 512;
	if(512 < rt.bottom)
		rt.bottom = 512;

	// 2011-03-25 by hsson 레이더 거리에 따라서 미션맵에서 팅기던 문제 예외처리로 수정
	if(0 > rtReal.left)
		rtReal.left = 0;
	if(0 > rtReal.top)
		rtReal.top = 0;
	if(512 < rtReal.right)
		rtReal.right = 512;
	if(512 < rtReal.bottom)
		rtReal.bottom = 512;
	// end 2011-03-25 by hsson 레이더 거리에 따라서 미션맵에서 팅기던 문제 예외처리로 수정


	CINFImage* pMiniBigMapImageTEMP = NULL;
	if(g_pD3dApp->m_bCharacter)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapCityImage;
	}

	if(pMiniBigMapImageTEMP == NULL || g_pD3dApp->m_bCharacter == FALSE)
	{
		pMiniBigMapImageTEMP = g_pGround->m_pMiniBigMapImage;
	}

	// 2006-08-08 by ispark, 그래도 없으면
	if(pMiniBigMapImageTEMP == NULL)
	{
		return;
	}

	HRESULT hr = 0;
	hr = m_pRadarMiniMap->GetTexture()->LockRect(0, &CopyTexRect, 0, 0);
	if(FAILED(pMiniBigMapImageTEMP->GetTexture()->LockRect(0, &TexRect, &rt, D3DLOCK_READONLY)))
		return;

	BYTE* pCopyRow = (BYTE*)CopyTexRect.pBits;
	BYTE* pTexRow = (BYTE*)TexRect.pBits;
	int tx, ty;
	for(int i = 0; i < RADAR_MINIMAP_SIZE; i++)
	{
		for(int j = 0; j < RADAR_MINIMAP_SIZE; j++)
		{
// 			if((0 > (rtReal.top + (int)(i * fIndex))) || (512 < (rtReal.top + (int)(i * fIndex))) || 
// 				(0 > (rtReal.left + (int)(j * fIndex))) || (512 < (rtReal.left + (int)(j * fIndex))))
			// 2012-11-09 by jhahn, 레이더 맵 표시 버그 수정
			if((0 > (rtReal.top + (int)(i * fIndex))) || (512 < (rtReal.top + (int)(i * fIndex))) || 
				(0 > (rtReal.left + (int)(j * fIndex))) || (512 < (rtReal.left + (int)(j * fIndex)))||
				rt.right + j < RADAR_RECT_SIZE || rt.bottom + i < RADAR_RECT_SIZE)
			// end 2012-11-09 by jhahn, 레이더 맵 표시 버그 수정
			{
				*pCopyRow++ = 0;
				*pCopyRow++ = 0;
				*pCopyRow++ = 0;
				*pCopyRow++ = m_byAlphaMiniMap[i][j];
			}
			else
			{
				ty = i;
				tx = j;
// 				if(rtReal.top < 0)
// 					ty = rtReal.top + i;
// 				if(rtReal.left < 0)
// 					tx = rtReal.left + j;
				// 2012-11-09 by jhahn, 레이더 맵 표시 버그 수정
				if(rtReal.top <= 0)
					ty = i - (RADAR_RECT_SIZE - rtReal.bottom);
				if(rtReal.left <= 0)
					tx = j - (RADAR_RECT_SIZE - rtReal.right);
				// end 2012-11-09 by jhahn, 레이더 맵 표시 버그 수정

				*pCopyRow++ = pTexRow[((int)(ty * fIndex) * 512 * 4) + ((int)(tx * fIndex) * 4)];
				*pCopyRow++ = pTexRow[((int)(ty * fIndex) * 512 * 4) + ((int)(tx * fIndex) * 4) + 1];
				*pCopyRow++ = pTexRow[((int)(ty * fIndex) * 512 * 4) + ((int)(tx * fIndex) * 4) + 2];
				*pCopyRow++ = m_byAlphaMiniMap[i][j];
//				*pCopyRow++ = 255;
			}
		}
	}

	m_pRadarMiniMap->GetTexture()->UnlockRect(0);
	pMiniBigMapImageTEMP->GetTexture()->UnlockRect(0);
	pMiniBigMapImageTEMP = NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFGameMainMiniMap::GetMiniBigMapCityPos(D3DXVECTOR3 vPos, float *fPosx, float *fPosy)
/// \brief		도시별 현 좌표 셋팅
/// \author		ispark
/// \date		2006-05-04 ~ 2006-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::GetMiniBigMapCityPos(D3DXVECTOR3 vPos, float *fPosx, float *fPosy)
{
//	*fPosx = (((4019 - vPos.x) * 512) / 2283)	X:맵의 X의 시작 위치 Y:맵 크기 Z: 캐릭터가 다닐수 있는 범위 2011-10-26 by jhahn 도시맵 미니 맵 보정
//	*fPosy = (((vPos.z - X) * Y) / Z);			X:맵의 Z의 시작 위치 Y:맵 크기 Z: 캐릭터가 다닐수 있는 범위 2011-10-26 by jhahn 도시맵 미니 맵 보정
	// 바이제니뉴
	if(g_pGround->m_nMapNumber == 2001)
	{
	
#ifdef C_EPSODE4_SETTING_JHAHN
		// 2011-10-26 by jhahn 도시맵 미니 맵 보정
		*fPosx = (((4019 - vPos.x) * 512) / 2283);		// *fPosx = (((4019 - vPos.x) * 512) / 2283);   
		*fPosy = (((vPos.z - 4101) * 512) / 2283);		// *fPosy = (((vPos.z - X) * Y) / Z);			
		//end 2011-10-26 by jhahn 도시맵 미니 맵 보정
#else
		*fPosx = (((1726 - vPos.x) * 512) / 1726) - 20;
		*fPosy = (((vPos.z - 4267) * 512) / 1726) - 20;
#endif

	}
	else if(g_pGround->m_nMapNumber == 1001)
	{
		*fPosx = (((2791 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1756) * 512) / 1042) - 20;
	}
	
	// 알링턴
	if(g_pGround->m_nMapNumber == 2002)
	{

#ifdef C_EPSODE4_SETTING_JHAHN
		// 2011-10-26 by jhahn 도시맵 미니 맵 보정
		*fPosx = (((4081 - vPos.x) * 512) / 2283);
		*fPosy = (((vPos.z - 3958) * 512) / 2283);
		//end 2011-10-26 by jhahn 도시맵 미니 맵 보정
#else
		*fPosx = (((1912 - vPos.x) * 512) / 1912) - 20;
		*fPosy = (((vPos.z - 4275) * 512) / 1912) - 20;
#endif
		
	}
	else if(g_pGround->m_nMapNumber == 1002)
	{
		*fPosx = (((2795 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1762) * 512) / 1042) - 20;
	}

	// 프리스카
	if(g_pGround->m_nMapNumber == 2003)
	{
		*fPosx = (((10236 - vPos.x) * 512) / 1330) - 10;//20;
		*fPosy = (((vPos.z - 4456) * 512) / 1330) - 20;
	}
	// 2007-09-10 by dgwoo 전진기지(바크시티상공)
	if(g_pGround->m_nMapNumber == 2004)
	{
//		*fPosx = (((5525 - vPos.x) * 512) / 793) - 20;
//		*fPosy = (((vPos.z - 8470) * 512) / 782) - 20;
		*fPosx = (((5525 - vPos.x) * 512) / 793) - 20;
		*fPosy = (((vPos.z - 8500) * 512) / 782) - 20;
	}
	// 2007-12-03 by dgwoo 전진기지 (2차 전진기지)
	if(g_pGround->m_nMapNumber == 2005)
	{
		*fPosx = (((2890 - vPos.x) * 512) / 620) - 20;
		*fPosy = (((vPos.z - 2376) * 512) / 620) - 20;

	}
	// 2008-10-06 by dgwoo 캐슬코로나 맵.
	if(g_pGround->m_nMapNumber == 2006)
	{
		*fPosx = (((5524 - vPos.x) * 512) / 793) - 20;
		*fPosy = (((vPos.z - 4802) * 512) / 782) - 20;

	}
	// 2013-03-14 by bhsohn 2007맵 관련 미니맵 사이즈 추가
	if(g_pGround->m_nMapNumber == 2007)
	{		
		// 2004맵과 동일하게 처리
		*fPosx = (((5525 - vPos.x) * 512) / 793) - 20;
		*fPosy = (((vPos.z - 8500) * 512) / 782) - 20;
	}
	// END 2013-03-14 by bhsohn 2007맵 관련 미니맵 사이즈 추가
	
}
void CINFGameMainMiniMap::GetMiniSmallMapCityPos(D3DXVECTOR3 vPos, float *fPosx, float *fPosy)
{

//	*fPosx = (((4019 - vPos.x) * 512) / 2283)	X:맵의 X의 시작 위치 Y:맵 크기 Z: 캐릭터가 다닐수 있는 범위 2011-10-26 by jhahn 도시맵 미니 맵 보정
//	*fPosy = (((vPos.z - X) * Y) / Z);			X:맵의 Z의 시작 위치 Y:맵 크기 Z: 캐릭터가 다닐수 있는 범위 2011-10-26 by jhahn 도시맵 미니 맵 보정

	// 바이제니뉴
	if(g_pGround->m_nMapNumber == 2001)
	{
#ifdef C_EPSODE4_SETTING_JHAHN
		// 2011-10-26 by jhahn 도시맵 미니 맵 보정
		*fPosx = (((4019 - vPos.x) * 512) / 2283);
		*fPosy = (((vPos.z - 4101) * 512) / 2283);
		//end 2011-10-26 by jhahn 도시맵 미니 맵 보정
#else
		*fPosx = (((1726 - vPos.x) * 512) / 1726) - 20;
		*fPosy = (((vPos.z - 4267) * 512) / 1726) - 20;
#endif
	}

	else if(g_pGround->m_nMapNumber == 1001)
	{
		*fPosx = (((2791 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1756) * 512) / 1042) - 20;
	}
	
	// 알링턴
	if(g_pGround->m_nMapNumber == 2002)
	{
#ifdef C_EPSODE4_SETTING_JHAHN
		// 2011-10-26 by jhahn 도시맵 미니 맵 보정
		*fPosx = (((4081 - vPos.x) * 512) / 2039);
		*fPosy = (((vPos.z - 4058) * 512) / 2039);
		//end 2011-10-26 by jhahn 도시맵 미니 맵 보정
#else
		*fPosx = (((1912 - vPos.x) * 512) / 1912) - 20;
		*fPosy = (((vPos.z - 4275) * 512) / 1912) - 20;
#endif
	}
	else if(g_pGround->m_nMapNumber == 1002)
	{
		*fPosx = (((2795 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1762) * 512) / 1042) - 20;
	}

	// 프리스카
	if(g_pGround->m_nMapNumber == 2003)
	{
		*fPosx = (((10236 - vPos.x) * 512) / 1330) - 10;//20;
		*fPosy = (((vPos.z - 4456) * 512) / 1330) - 20;
	}
	// 2007-09-10 by dgwoo 전진기지(바크시티상공)
	if(g_pGround->m_nMapNumber == 2004)
	{
//		*fPosx = (((5525 - vPos.x) * 512) / 793) - 20;
//		*fPosy = (((vPos.z - 8470) * 512) / 782) - 20;
		*fPosx = (((5525 - vPos.x) * 512) / 793) - 20;
		*fPosy = (((vPos.z - 8500) * 512) / 782) - 20;
	}
	// 2007-12-03 by dgwoo 전진기지 (2차 전진기지)
	if(g_pGround->m_nMapNumber == 2005)
	{
		*fPosx = (((2890 - vPos.x) * 512) / 620) - 10;
		*fPosy = (((vPos.z - 2376) * 512) / 620) - 10;

	}
	// 2008-10-06 by dgwoo 캐슬코로나 맵.
	if(g_pGround->m_nMapNumber == 2006)
	{
		*fPosx = (((5524 - vPos.x) * 512) / 793) - 20;
		*fPosy = (((vPos.z - 4802) * 512) / 782) - 20;

	}
	// 2013-03-14 by bhsohn 2007맵 관련 미니맵 사이즈 추가
	if(g_pGround->m_nMapNumber == 2007)
	{
		// 2004맵와 동일하게 처리
		*fPosx = (((5525 - vPos.x) * 512) / 793) - 20;
		*fPosy = (((vPos.z - 8500) * 512) / 782) - 20;
	}
	// END 2013-03-14 by bhsohn 2007맵 관련 미니맵 사이즈 추가


	*fPosx = (*fPosx / 4);
	*fPosy = (*fPosy / 4);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFGameMainMiniMap::GetMiniBigMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy)
/// \brief		기어 미니맵 좌표
/// \author		ispark
/// \date		2006-09-04 ~ 2006-09-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::GetMiniBigMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy)
{
	// 바이제니뉴
	if(g_pGround->m_nMapNumber == 1001)
	{
		*fPosx = (((2791 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1756) * 512) / 1042) - 20;
	}
	// 알링턴
	else if(g_pGround->m_nMapNumber == 1002)
	{
		*fPosx = (((2795 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1762) * 512) / 1042) - 20;
	}
	else
	{
		*fPosx = 492 - (vPos.x / 20);
		*fPosy = (vPos.z / 20) - 20;
	}
}
void CINFGameMainMiniMap::GetMiniSmallMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy)
{
	// 바이제니뉴
	if(g_pGround->m_nMapNumber == 1001)
	{
		*fPosx = (((2791 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1756) * 512) / 1042) - 20;
	}
	// 알링턴
	else if(g_pGround->m_nMapNumber == 1002)
	{
		*fPosx = (((2795 - vPos.x) * 512) / 1036) - 20;
		*fPosy = (((vPos.z - 1762) * 512) / 1042) - 20;
	}
	else
	{
		*fPosx = (492 - (vPos.x / 20)) / 4;
		*fPosy = ((vPos.z / 20) - 20) / 4;
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		서버 시간 보기 버튼
/// \author		// 2008-11-3 by bhsohn 서버 시간 보기 버튼 추가
/// \date		2008-11-3 ~ 2008-11-3
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFGameMainMiniMap::OnClickServerTime()
{
	if(g_pGameMain)
	{
		g_pGameMain->OnClickServerTime();
	}

}
