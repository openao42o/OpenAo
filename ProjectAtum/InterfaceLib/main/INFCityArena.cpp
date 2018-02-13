// INFCityArena.cpp: implementation of the CINFCityArena class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityArena.h"
#include "AtumApplication.h"
#include "INFCitySupply.h"
#include "INFCityBase.h"
#include "INFScrollbar.h"
#include "INFImage.h"
#include "D3dHanFont.h"
#include "ItemInfo.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "GameDataLast.h"
#include "StoreData.h"
#include "WeaponItemInfo.h"
#include "INFWindow.h"
#include "INFGameMain.h"
#include "INFItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFIcon.h"
#include "AtumDatabase.h"
#include "dxutil.h"
#include "Chat.h"
#include "Interface.h"
#include "INFGameArena.h"
#include "INFListbox.h"
#include "INFImageEx.h"	            // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "Interface.h"
#include "INFToolTip.h"

#define ARENA_START_X			(CITY_BASE_NPC_BOX_START_X)									// 윈도우 창 젤 앞단.

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

#define ARENA_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_ARENA_WINDOW_Y - 30)			// 바를 제외한 젤 윗단.

#else
#define ARENA_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_ARENA_WINDOW_Y + 20)			// 바를 제외한 젤 윗단.

#endif	

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
#define ARENA_CREATE_W					455		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 230에서 455로 수정
#define ARENA_CREATE_H					329		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 200에서 329로 수정
#else
#define ARENA_CREATE_W					230
#define ARENA_CREATE_H					200
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)
#define ARENA_START_CREATE_X			((g_pD3dApp->GetBackBufferDesc().Width - ARENA_CREATE_W ) /2 )
#define ARENA_START_CREATE_Y			((g_pD3dApp->GetBackBufferDesc().Height - ARENA_CREATE_H ) /2 )

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define ARENA_MATCH_LIST_GAP			31
#else	
#define ARENA_MATCH_LIST_GAP			20
#endif

#define ARENA_LIST_COUNT				8

#define ARENA_FONT_X					ARENA_START_X + 3
#define ARENA_FONT_Y					CITY_BASE_NPC_BOX_START_Y - SIZE_ARENA_WINDOW_Y + 2

#define ARENA_LIST_ROOM_X				ARENA_START_X + 9
#define ARENA_LIST_ROOM_Y				ARENA_START_Y + 4

#define ARENA_LIST_REFRESH_BUTTON_X		ARENA_START_X + 526
#define ARENA_LIST_REFRESH_BUTTON_Y		ARENA_START_Y + 3
#define ARENA_LIST_REFRESH_BUTTON_W		21
#define ARENA_LIST_REFRESH_BUTTON_H		19

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK

#define ARENA_INFLURENCE_LIST_X				ARENA_START_X + 45
#define ARENA_INFLURENCE_LIST_Y				ARENA_START_Y + 31
#define ARENA_INFLURENCE_LIST_W				163
#define ARENA_INFLURENCE_LIST_H				24

#define ARENA_INFLURENCE_LIST_ITEM_X		(ARENA_INFLURENCE_LIST_X+5)		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - +5 추가
#define ARENA_INFLURENCE_LIST_ITEM_Y		(ARENA_INFLURENCE_LIST_Y + ARENA_INFLURENCE_LIST_H)
#define ARENA_INFLURENCE_LIST_ITEM_W		163
#define ARENA_INFLURENCE_LIST_ITEM_H		15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 20에서 15로 수정

#define ARENA_WAR_STATE_X					ARENA_START_X + 219
#define ARENA_WAR_STATE_Y					ARENA_START_Y + 31
#define ARENA_WAR_STATE_W					163
#define ARENA_WAR_STATE_H					24

#define ARENA_WAR_STATE_ITEM_X				(ARENA_WAR_STATE_X+5)		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - +5 추가
#define ARENA_WAR_STATE_ITEM_Y				(ARENA_WAR_STATE_Y + ARENA_WAR_STATE_H)
#define ARENA_WAR_STATE_ITEM_W				163
#define ARENA_WAR_STATE_ITEM_H				15		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - 기존 20에서 15로 수정

#define ARENA_WAR_STATE_COUNT				3

#define ARENA_LIST_OPTION_H					13
// 검색 버튼.
#define ARENA_TEAM_SEARCH_BUTTON_X		ARENA_START_X + 385
#define ARENA_TEAM_SEARCH_BUTTON_Y		ARENA_START_Y + 30
#define ARENA_TEAM_SEARCH_BUTTON_W		47//38			// 2007-08-02 by dgwoo 해외이미지가 크기에 맞춰서 
#define ARENA_TEAM_SEARCH_BUTTON_H		17

#define ARENA_CREATE_BUTTON_X		ARENA_START_X + 457		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 405에서 457로 수정
#define ARENA_CREATE_BUTTON_Y		ARENA_START_Y + 325		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 320에서 325로 수정

#define ARENA_APP_BUTTON_X		ARENA_START_X + 506			// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 455에서 506으로 수정
#define ARENA_APP_BUTTON_Y		ARENA_START_Y + 325			// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 320에서 325로 수정

#define ARENA_QUICKAPP_BUTTON_X		ARENA_START_X + 554		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 505에서 554로 수정
#define ARENA_QUICKAPP_BUTTON_Y		ARENA_START_Y + 325		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 320에서 325로 수정


#define ARENA_APPCANCEL_BUTTON_X		ARENA_START_X + 603	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 555에서 603로 수정
#define ARENA_APPCANCEL_BUTTON_Y		ARENA_START_Y + 325	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 320에서 325로 수정



#define ARENA_LIST_ROOM_NUMBER_FONT_X		ARENA_START_X + 70
#define ARENA_LIST_ROOM_NUMBER_FONT_Y		ARENA_START_Y + 86

#define ARENA_LIST_ROOM_ID_FONT_X		ARENA_START_X + 167
#define	ARENA_LIST_ROOM_ID_FONT_Y		ARENA_START_Y + 86

// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
#define ARENA_LIST_ROOM_EVENT_FONT_X		(ARENA_START_X + 82)
#define	ARENA_LIST_ROOM_EVENT_FONT_Y		(ARENA_START_Y + 86)

#define ARENA_LIST_ROOM_MODE_X			ARENA_START_X + 298//233
#define ARENA_LIST_ROOM_MODE_Y			ARENA_START_Y + 87

#define ARENA_LIST_ROOM_MAP_X			ARENA_START_X + 357	// 2012-04-13 by mspark, 아레나 UI 작업
#define ARENA_LIST_ROOM_MAP_Y			ARENA_START_Y + 82	// 2012-04-13 by mspark, 아레나 UI 작업

#define ARENA_LIST_ROOM_LEVEL_X			ARENA_START_X + 433	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 372에서 433으로 수정
#define ARENA_LIST_ROOM_LEVEL_Y			ARENA_START_Y + 86

#define ARENA_LIST_ROOM_PERSON_X		ARENA_START_X + 498	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 437에서 498로 수정
#define ARENA_LIST_ROOM_PERSON_Y		ARENA_START_Y + 86

#define ARENA_LIST_ROOM_LOCK_X			ARENA_START_X + 552	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 491에서 552로 수정
#define ARENA_LIST_ROOM_LOCK_Y			ARENA_START_Y + 84

#define ARENA_LIST_ROOM_STATE_X			ARENA_START_X + 629	// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 562에서 629로 수정
#define ARENA_LIST_ROOM_STATE_Y			ARENA_START_Y + 87

#define ARENA_LIST_TOOLTIPBACK_IMAGE_W	150					// 2012-04-13 by mspark, 아레나 UI 작업
#define ARENA_LIST_TOOLTIPBACK_IMAGE_H	120					// 2012-04-13 by mspark, 아레나 UI 작업

#define ARENA_LIST_TOOLTIPMAP_IMAGE_X	32					// 2012-04-13 by mspark, 아레나 UI 작업
#define ARENA_LIST_TOOLTIPMAP_IMAGE_Y	20					// 2012-04-13 by mspark, 아레나 UI 작업

#define ARENA_LIST_TOOLTIPFONT_IMAGE_X	(ARENA_LIST_TOOLTIPBACK_IMAGE_W/2)	// 2012-04-13 by mspark, 아레나 UI 작업
#define ARENA_LIST_TOOLTIPFONT_IMAGE_Y	90									// 2012-04-13 by mspark, 아레나 UI 작업

#define ARENA_LIST_SELECT_X				ARENA_START_X + 38
#define ARENA_LIST_SELECT_Y				ARENA_START_Y + 75

#define ARENA_LIST_W					566
#define ARENA_LIST_H					215

// 2012-04-13 by mspark, 아레나 UI 작업
#define ARENA_MAP_01_NUMBER				9101
#define ARENA_MAP_02_NUMBER				9102
#define ARENA_MAP_RANDOM_NUMBER			9100
// end 2012-04-13 by mspark, 아레나 UI 작업

//--------------------------------------------------------------------------//
//							스크롤
#define ARENA_LIST_SCROLL_LINE_LENGTH	195
#define ARENA_LIST_SCROLL_WHELL_AREA_X	(ARENA_START_X + 24)
#define ARENA_LIST_SCROLL_WHELL_AREA_Y	(ARENA_START_Y + 46)
#define ARENA_LIST_SCROLL_WHELL_AREA_W	(ARENA_LIST_SCROLL_WHELL_AREA_X + 525)
#define ARENA_LIST_SCROLL_WHELL_AREA_H	(ARENA_LIST_SCROLL_WHELL_AREA_Y + 185)

#define ARENA_LIST_SCROLL_DRAG_AREA_X	(ARENA_START_X + 676) // 2012-04-13 by mspark, 아레나 UI 작업 - 기존 618에서 676으로 수정
#define ARENA_LIST_SCROLL_DRAG_AREA_Y	(ARENA_START_Y + 76)
#define ARENA_LIST_SCROLL_DRAG_AREA_W	(ARENA_LIST_SCROLL_DRAG_AREA_X + 18)
#define ARENA_LIST_SCROLL_DRAG_AREA_H	(ARENA_LIST_SCROLL_DRAG_AREA_Y + 150)

// 2007-05-15 by bhsohn 스크롤 관련 버그 수정
#define	ARENA_LIST_SCROLL_DRAG_BALL_CAP		55
//--------------------------------------------------------------------------//

#else		  

#define ARENA_INFLURENCE_LIST_X				ARENA_START_X + 45
#define ARENA_INFLURENCE_LIST_Y				ARENA_START_Y + 31
#define ARENA_INFLURENCE_LIST_W				163
#define ARENA_INFLURENCE_LIST_H				24

#define ARENA_INFLURENCE_LIST_ITEM_X		(ARENA_INFLURENCE_LIST_X)
#define ARENA_INFLURENCE_LIST_ITEM_Y		(ARENA_INFLURENCE_LIST_Y + ARENA_INFLURENCE_LIST_H)
#define ARENA_INFLURENCE_LIST_ITEM_W		163
#define ARENA_INFLURENCE_LIST_ITEM_H		20

#define ARENA_WAR_STATE_X					ARENA_START_X + 219
#define ARENA_WAR_STATE_Y					ARENA_START_Y + 31
#define ARENA_WAR_STATE_W					163
#define ARENA_WAR_STATE_H					24

#define ARENA_WAR_STATE_ITEM_X				(ARENA_WAR_STATE_X)
#define ARENA_WAR_STATE_ITEM_Y				(ARENA_WAR_STATE_Y + ARENA_WAR_STATE_H)
#define ARENA_WAR_STATE_ITEM_W				163
#define ARENA_WAR_STATE_ITEM_H				20

#define ARENA_WAR_STATE_COUNT				3

#define ARENA_LIST_OPTION_H					13
// 검색 버튼.
#define ARENA_TEAM_SEARCH_BUTTON_X		ARENA_START_X + 385
#define ARENA_TEAM_SEARCH_BUTTON_Y		ARENA_START_Y + 30
#define ARENA_TEAM_SEARCH_BUTTON_W		47//38			// 2007-08-02 by dgwoo 해외이미지가 크기에 맞춰서 
#define ARENA_TEAM_SEARCH_BUTTON_H		17

#define ARENA_CREATE_BUTTON_X		ARENA_START_X + 405
#define ARENA_CREATE_BUTTON_Y		ARENA_START_Y + 320

#define ARENA_APP_BUTTON_X		ARENA_START_X + 455
#define ARENA_APP_BUTTON_Y		ARENA_START_Y + 320

#define ARENA_QUICKAPP_BUTTON_X		ARENA_START_X + 505
#define ARENA_QUICKAPP_BUTTON_Y		ARENA_START_Y + 320


#define ARENA_APPCANCEL_BUTTON_X		ARENA_START_X + 555
#define ARENA_APPCANCEL_BUTTON_Y		ARENA_START_Y + 320



#define ARENA_LIST_ROOM_NUMBER_FONT_X		ARENA_START_X + 70
#define ARENA_LIST_ROOM_NUMBER_FONT_Y		ARENA_START_Y + 86

#define ARENA_LIST_ROOM_ID_FONT_X		ARENA_START_X + 167
#define	ARENA_LIST_ROOM_ID_FONT_Y		ARENA_START_Y + 86

// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
#define ARENA_LIST_ROOM_EVENT_FONT_X		(ARENA_START_X + 82)
#define	ARENA_LIST_ROOM_EVENT_FONT_Y		(ARENA_START_Y + 86)

#define ARENA_LIST_ROOM_MODE_X			ARENA_START_X + 298//233
#define ARENA_LIST_ROOM_MODE_Y			ARENA_START_Y + 87

#define ARENA_LIST_ROOM_LEVEL_X			ARENA_START_X + 372
#define ARENA_LIST_ROOM_LEVEL_Y			ARENA_START_Y + 86

#define ARENA_LIST_ROOM_PERSON_X		ARENA_START_X + 437
#define ARENA_LIST_ROOM_PERSON_Y		ARENA_START_Y + 86

#define ARENA_LIST_ROOM_LOCK_X			ARENA_START_X + 491
#define ARENA_LIST_ROOM_LOCK_Y			ARENA_START_Y + 84

#define ARENA_LIST_ROOM_STATE_X			ARENA_START_X + 562
#define ARENA_LIST_ROOM_STATE_Y			ARENA_START_Y + 87

#define ARENA_LIST_SELECT_X				ARENA_START_X + 38
#define ARENA_LIST_SELECT_Y				ARENA_START_Y + 75

#define ARENA_LIST_W					566
#define ARENA_LIST_H					215

//--------------------------------------------------------------------------//
//							스크롤
#define ARENA_LIST_SCROLL_LINE_LENGTH	195
#define ARENA_LIST_SCROLL_WHELL_AREA_X	(ARENA_START_X + 24)
#define ARENA_LIST_SCROLL_WHELL_AREA_Y	(ARENA_START_Y + 46)
#define ARENA_LIST_SCROLL_WHELL_AREA_W	(ARENA_LIST_SCROLL_WHELL_AREA_X + 525)
#define ARENA_LIST_SCROLL_WHELL_AREA_H	(ARENA_LIST_SCROLL_WHELL_AREA_Y + 185)

#define ARENA_LIST_SCROLL_DRAG_AREA_X	(ARENA_START_X + 618)
#define ARENA_LIST_SCROLL_DRAG_AREA_Y	(ARENA_START_Y + 76)
#define ARENA_LIST_SCROLL_DRAG_AREA_W	(ARENA_LIST_SCROLL_DRAG_AREA_X + 18)
#define ARENA_LIST_SCROLL_DRAG_AREA_H	(ARENA_LIST_SCROLL_DRAG_AREA_Y + 150)

// 2007-05-15 by bhsohn 스크롤 관련 버그 수정
#define	ARENA_LIST_SCROLL_DRAG_BALL_CAP		55
//--------------------------------------------------------------------------//

#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

#else		  
#define ARENA_INFLURENCE_LIST_X				ARENA_START_X + 14
#define ARENA_INFLURENCE_LIST_Y				ARENA_START_Y + 3
#define ARENA_INFLURENCE_LIST_W				103
#define ARENA_INFLURENCE_LIST_H				17

#define ARENA_INFLURENCE_LIST_ITEM_X		(ARENA_INFLURENCE_LIST_X)
#define ARENA_INFLURENCE_LIST_ITEM_Y		(ARENA_INFLURENCE_LIST_Y + ARENA_INFLURENCE_LIST_H)
#define ARENA_INFLURENCE_LIST_ITEM_W		103
#define ARENA_INFLURENCE_LIST_ITEM_H		13

#define ARENA_WAR_STATE_X					ARENA_START_X + 118
#define ARENA_WAR_STATE_Y					ARENA_START_Y + 3
#define ARENA_WAR_STATE_W					103
#define ARENA_WAR_STATE_H					17

#define ARENA_WAR_STATE_ITEM_X				(ARENA_WAR_STATE_X)
#define ARENA_WAR_STATE_ITEM_Y				(ARENA_WAR_STATE_Y + ARENA_WAR_STATE_H)
#define ARENA_WAR_STATE_ITEM_W				103
#define ARENA_WAR_STATE_ITEM_H				13

#define ARENA_WAR_STATE_COUNT				3

#define ARENA_LIST_OPTION_H					13
// 검색 버튼.
#define ARENA_TEAM_SEARCH_BUTTON_X		ARENA_START_X + 225
#define ARENA_TEAM_SEARCH_BUTTON_Y		ARENA_START_Y + 3
#define ARENA_TEAM_SEARCH_BUTTON_W		47//38			// 2007-08-02 by dgwoo 해외이미지가 크기에 맞춰서 
#define ARENA_TEAM_SEARCH_BUTTON_H		17

#define ARENA_LIST_ROOM_NUMBER_FONT_X		ARENA_START_X + 60
#define ARENA_LIST_ROOM_NUMBER_FONT_Y		ARENA_START_Y + 72

#define ARENA_LIST_ROOM_ID_FONT_X		ARENA_START_X + 67 + 80
#define	ARENA_LIST_ROOM_ID_FONT_Y		ARENA_START_Y + 72

// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
#define ARENA_LIST_ROOM_EVENT_FONT_X		(ARENA_START_X + 67)
#define	ARENA_LIST_ROOM_EVENT_FONT_Y		(ARENA_START_Y + 72)

#define ARENA_LIST_ROOM_MODE_X			ARENA_START_X + 277//233
#define ARENA_LIST_ROOM_MODE_Y			ARENA_START_Y + 76

#define ARENA_LIST_ROOM_LEVEL_X			ARENA_START_X + 348
#define ARENA_LIST_ROOM_LEVEL_Y			ARENA_START_Y + 72

#define ARENA_LIST_ROOM_PERSON_X		ARENA_START_X + 392
#define ARENA_LIST_ROOM_PERSON_Y		ARENA_START_Y + 72

#define ARENA_LIST_ROOM_LOCK_X			ARENA_START_X + 425
#define ARENA_LIST_ROOM_LOCK_Y			ARENA_START_Y + 72

#define ARENA_LIST_ROOM_STATE_X			ARENA_START_X + 480
#define ARENA_LIST_ROOM_STATE_Y			ARENA_START_Y + 72

#define ARENA_LIST_SELECT_X				ARENA_START_X + 26
#define ARENA_LIST_SELECT_Y				ARENA_START_Y + 70

#define ARENA_LIST_W					490
#define ARENA_LIST_H					168

//--------------------------------------------------------------------------//
//							스크롤
#define ARENA_LIST_SCROLL_LINE_LENGTH	195
#define ARENA_LIST_SCROLL_WHELL_AREA_X	(ARENA_START_X + 24)
#define ARENA_LIST_SCROLL_WHELL_AREA_Y	(ARENA_START_Y + 46)
#define ARENA_LIST_SCROLL_WHELL_AREA_W	(ARENA_LIST_SCROLL_WHELL_AREA_X + 525)
#define ARENA_LIST_SCROLL_WHELL_AREA_H	(ARENA_LIST_SCROLL_WHELL_AREA_Y + 185)

#define ARENA_LIST_SCROLL_DRAG_AREA_X	(ARENA_START_X + 518)
#define ARENA_LIST_SCROLL_DRAG_AREA_Y	(ARENA_START_Y + 43)
#define ARENA_LIST_SCROLL_DRAG_AREA_W	(ARENA_LIST_SCROLL_DRAG_AREA_X + 18)
#define ARENA_LIST_SCROLL_DRAG_AREA_H	(ARENA_LIST_SCROLL_DRAG_AREA_Y + 150)

// 2007-05-15 by bhsohn 스크롤 관련 버그 수정
#define	ARENA_LIST_SCROLL_DRAG_BALL_CAP		55
//--------------------------------------------------------------------------//

#endif






//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCityArena::CINFCityArena(CAtumNode* pParent, BUILDINGNPC* pBuilding, CGameData *pGameData)
{
	m_pParent = pParent;	// CGameMain*
	m_pGameData = pGameData;
	m_pBuildingInfo = pBuilding;

	m_nTab = 0;
	m_BArenaMode = ARENAMODE_DEATHMATCH;
	
	m_nTeamInfoB = BUTTON_STATE_NORMAL;
	m_nRefresh = BUTTON_STATE_NORMAL;
	m_nSearch = BUTTON_STATE_NORMAL;

	m_pArenaCreate = NULL;
	m_nSelectIndex = -1;

	m_BRequestTeamState = ARENA_REQUEST_TEAM_STATE_OUR;
	

	if(m_pArenaCreate == NULL)
	{
		m_pArenaCreate = new CINFArenaCreate(this, m_pBuildingInfo);
		m_pArenaCreate->SetGameData(m_pGameData);
//		m_pArenaCreate->InitDeviceObjects();
//		m_pArenaCreate->RestoreDeviceObjects();
	}

	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리



	m_pBoldFont = NULL;

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01 = NULL;
	m_pImgArenaMap02 = NULL;
	m_pImgArenaMapRandom = NULL;
	m_pImgArenaMapShadow = NULL;
	m_pFontArenaMapName = NULL;

	m_bMouseOverMap = FALSE;

	m_ptMousePos.x = 0;
	m_ptMousePos.y = 0;

	m_nCurrentStayIndex = -1;
	// end 2012-04-13 by mspark, 아레나 UI 작업

}

CINFCityArena::~CINFCityArena()
{

}
HRESULT CINFCityArena::DeleteDeviceObjects()
{
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgRefreshB[i]->DeleteDeviceObjects();
		m_pImgSearch[i]->DeleteDeviceObjects();
		util::del(m_pImgRefreshB[i]);
		util::del(m_pImgSearch[i]);

	}
	for(i = 0 ; i < ARENA_ROOM_LOCKMODE ; i++)
	{
		m_pImgLock[i]->DeleteDeviceObjects();
		util::del(m_pImgLock[i]);
	}
	for(i = 0 ; i < ARENA_WAR_MODE_END ; i++)
	{
		m_pImgArenaMode[i]->DeleteDeviceObjects();
		util::del(m_pImgArenaMode[i]);
	}
	m_pLBoxInf->DeleteDeviceObjects();
	m_pLBoxState->DeleteDeviceObjects();
	m_pScroll->DeleteDeviceObjects();
	m_pFontRoomNum->DeleteDeviceObjects();
	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
	m_pBoldFont->DeleteDeviceObjects();	 

	m_pFontUserID->DeleteDeviceObjects();
	m_pFontPerson->DeleteDeviceObjects();
	m_pFontState->DeleteDeviceObjects();
	m_pFontLevel->DeleteDeviceObjects();
	
	util::del(m_pLBoxInf);
	util::del(m_pLBoxState);
	util::del(m_pScroll);
	util::del(m_pFontRoomNum);
	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
	util::del(m_pBoldFont);	

	util::del(m_pFontUserID);
	util::del(m_pFontPerson);
	util::del(m_pFontState);
	util::del(m_pFontLevel);

	
	m_pImgSelectTeam->DeleteDeviceObjects();
	m_pImgAreRoomList->DeleteDeviceObjects();
	m_pImgAreFont->DeleteDeviceObjects();
	util::del(m_pImgSelectTeam);
	util::del(m_pImgAreRoomList);
	util::del(m_pImgAreFont);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewArenaBackImage->DeleteDeviceObjects();
	util::del(m_pRenewArenaBackImage);

	if( m_pArenaCreateBtn )
	{
		m_pArenaCreateBtn->DeleteDeviceObjects();
		util::del( m_pArenaCreateBtn );
	}
	
	if( m_pArenaAppBtn )
	{
		m_pArenaAppBtn->DeleteDeviceObjects();
		util::del( m_pArenaAppBtn );
	}
	
	if( m_pArenaAppCancelBtn )
	{
		m_pArenaAppCancelBtn->DeleteDeviceObjects();
		util::del( m_pArenaAppCancelBtn );
	}
	
	if( m_pArenaQuickAppBtn )
	{
		m_pArenaQuickAppBtn->DeleteDeviceObjects();
		util::del( m_pArenaQuickAppBtn );
	}

#endif
	m_pArenaCreate->DeleteDeviceObjects();

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01->DeleteDeviceObjects();
	m_pImgArenaMap02->DeleteDeviceObjects();
	m_pImgArenaMapRandom->DeleteDeviceObjects();
	m_pImgArenaMapShadow->DeleteDeviceObjects();
	m_pFontArenaMapName->DeleteDeviceObjects();

	util::del(m_pImgArenaMap01);
	util::del(m_pImgArenaMap02);
	util::del(m_pImgArenaMapRandom);
	util::del(m_pImgArenaMapShadow);
	util::del(m_pFontArenaMapName);
	// end 2012-04-13 by mspark, 아레나 UI 작업


	return S_OK;
}
HRESULT CINFCityArena::InitDeviceObjects()
{
	int i;
	DataHeader	* pDataHeader;
	char buf[64];
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		wsprintf( buf, "refresh%d", i);
		pDataHeader = FindResource(buf);
		m_pImgRefreshB[i] = new CINFImageEx;
		m_pImgRefreshB[i]->InitDeviceObjects( pDataHeader );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	wsprintf( buf, "Faq_df%d", i);
#else
		wsprintf( buf, "searchb%d", i);
#endif	

	
		pDataHeader = FindResource(buf);
		m_pImgSearch[i] = new CINFImageEx;
		m_pImgSearch[i]->InitDeviceObjects( pDataHeader );


	}
	for(i = 0 ; i < ARENA_ROOM_LOCKMODE ; i++)
	{
		wsprintf( buf, "lock%d", i);
		pDataHeader = FindResource(buf);
		m_pImgLock[i] = new CINFImageEx;
		m_pImgLock[i]->InitDeviceObjects( pDataHeader );
	}
	for(i = 0 ; i < ARENA_WAR_MODE_END ; i++)
	{
		wsprintf( buf, "amode%d", i);
		pDataHeader = FindResource(buf);
		m_pImgArenaMode[i] = new CINFImageEx;
		m_pImgArenaMode[i]->InitDeviceObjects( pDataHeader );
	}
	//--------------------------------------------------------------------------//
	// 리스트 박스 설정.
	if(NULL == m_pLBoxInf)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pLBoxInf = new CINFListBox("cbarenaba","cbamode");
#else			 
		m_pLBoxInf = new CINFListBox("cbarena","cbarenab");
#endif

		m_pLBoxInf->AddElement(STRMSG_C_080225_0200);
		m_pLBoxInf->AddElement(STRMSG_C_080225_0201);
		// 2007-11-22 by bhsohn 아레나 통합서버
//		if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_ANI))
//			m_pLBoxInf->m_nSelect = 1;
		m_pLBoxInf->UpdateItem();
		m_pLBoxInf->SetMainArea(ARENA_INFLURENCE_LIST_X,ARENA_INFLURENCE_LIST_Y,ARENA_INFLURENCE_LIST_W,ARENA_INFLURENCE_LIST_H);
		m_pLBoxInf->SetElementArea(ARENA_INFLURENCE_LIST_ITEM_X,ARENA_INFLURENCE_LIST_ITEM_Y,ARENA_INFLURENCE_LIST_ITEM_W,ARENA_INFLURENCE_LIST_ITEM_H);
		m_pLBoxInf->SetBGPos(ARENA_INFLURENCE_LIST_ITEM_X,ARENA_INFLURENCE_LIST_ITEM_Y,ARENA_INFLURENCE_LIST_ITEM_W,ARENA_INFLURENCE_LIST_ITEM_H);		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - m_pLBoxInf->SetBGPos(...) 추가
		m_pLBoxInf->InitDeviceObjects();
	}
	if(NULL == m_pLBoxState)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pLBoxState = new CINFListBox("cbarenaba","cbamode");
#else			 
		m_pLBoxState = new CINFListBox("cbarena","cbarenab");
#endif


		m_pLBoxState->AddElement(STRMSG_C_070608_0102);
		m_pLBoxState->AddElement(STRMSG_C_070608_0103);
		m_pLBoxState->UpdateItem();
		m_pLBoxState->SetMainArea(ARENA_WAR_STATE_X,ARENA_WAR_STATE_Y,ARENA_WAR_STATE_W,ARENA_WAR_STATE_H);
		m_pLBoxState->SetElementArea(ARENA_WAR_STATE_ITEM_X,ARENA_WAR_STATE_ITEM_Y,ARENA_WAR_STATE_ITEM_W,ARENA_WAR_STATE_ITEM_H);
		m_pLBoxState->SetBGPos(ARENA_WAR_STATE_ITEM_X,ARENA_WAR_STATE_ITEM_Y,ARENA_WAR_STATE_ITEM_W,ARENA_WAR_STATE_ITEM_H);		// 2012-03-29 by mspark, 콤보 박스 클릭 시 보이는 리스트 배경 이미지 간격 수정 - m_pLBoxState->SetBGPos(...) 추가
		m_pLBoxState->InitDeviceObjects();

	}
	//--------------------------------------------------------------------------//

	char szScBall[30];	
	if(NULL == m_pScroll)
	{
		m_pScroll = new CINFArenaScrollBar;
	}
	//wsprintf(szScBk,"arescroll");
	wsprintf(szScBall,"c_scrlb");				

	m_pScroll->InitDeviceObjects(ARENA_LIST_COUNT, szScBall);

	
	m_pFontRoomNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontRoomNum->InitDeviceObjects(g_pD3dDev);
	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
	m_pBoldFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE|D3DFONT_BOLD, FALSE,128,32);
	m_pBoldFont->InitDeviceObjects(g_pD3dDev);	

	m_pFontUserID = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontUserID->InitDeviceObjects(g_pD3dDev);
	m_pFontPerson = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontPerson->InitDeviceObjects(g_pD3dDev);
	m_pFontState = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontState->InitDeviceObjects(g_pD3dDev);
	m_pFontLevel = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontLevel->InitDeviceObjects(g_pD3dDev);


	pDataHeader = FindResource("selectteam");
	m_pImgSelectTeam = new CINFImageEx;
	m_pImgSelectTeam->InitDeviceObjects( pDataHeader );
	
	pDataHeader = FindResource("matchtab");
	m_pImgAreRoomList = new CINFImageEx;
	m_pImgAreRoomList->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("arenafont");
	m_pImgAreFont = new CINFImageEx;
	m_pImgAreFont->InitDeviceObjects( pDataHeader );

	m_pArenaCreate->InitDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    {
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "arena" );
		m_pRenewArenaBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewArenaBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewArenaBackImage->RestoreDeviceObjects();
		
		
	}
	
	{

		m_pArenaCreateBtn = new CINFImageBtn;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
		//m_pArenaCreateBtn->InitDeviceObjects( "cres03", "cres01", "cres00", "cres02" );
		m_pArenaCreateBtn->InitDeviceObjects( "cres03", "cres01", "cres00", "cres02","STRTOOLTIP47" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#else	
		m_pArenaCreateBtn->InitDeviceObjects( "if_rsc3", "if_rsc1", "if_rsc0", "if_rsc2" );
#endif

		// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
// 		m_pArenaAppBtn = new CINFImageBtn;	
// 		m_pArenaAppBtn->InitDeviceObjects( "pars03", "pars01", "pars00", "pars02" );
// 			
// 		m_pArenaQuickAppBtn = new CINFImageBtn;	
// 		m_pArenaQuickAppBtn->InitDeviceObjects( "hurrytm3", "hurrytm1", "hurrytm0", "hurrytm2" );
// 		
// 		m_pArenaAppCancelBtn = new CINFImageBtn;	
// 		m_pArenaAppCancelBtn->InitDeviceObjects( "cancb3", "cancb1", "cancb0", "cancb2" );
		m_pArenaAppBtn = new CINFImageBtn;	
		m_pArenaAppBtn->InitDeviceObjects( "pars03", "pars01", "pars00", "pars02","STRTOOLTIP48" );
	
		m_pArenaQuickAppBtn = new CINFImageBtn;	
		m_pArenaQuickAppBtn->InitDeviceObjects( "hurrytm3", "hurrytm1", "hurrytm0", "hurrytm2","STRTOOLTIP80" );
		
		m_pArenaAppCancelBtn = new CINFImageBtn;	
		m_pArenaAppCancelBtn->InitDeviceObjects( "cancb3", "cancb1", "cancb0", "cancb2","STRTOOLTIP81" );
		// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	}
#endif
	
	// 2012-04-13 by mspark, 아레나 UI 작업
	pDataHeader = FindResource("aremap01");
	m_pImgArenaMap01 = new CINFImageEx;
	m_pImgArenaMap01->InitDeviceObjects( pDataHeader );
	
	pDataHeader = FindResource("aremap02");
	m_pImgArenaMap02 = new CINFImageEx;
	m_pImgArenaMap02->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("are_random");
	m_pImgArenaMapRandom = new CINFImageEx;
	m_pImgArenaMapRandom->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("bigslot");
	m_pImgArenaMapShadow = new CINFImageEx;
	m_pImgArenaMapShadow->InitDeviceObjects(pDataHeader);

	m_pFontArenaMapName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE|D3DFONT_BOLD,  TRUE,512,32);
	m_pFontArenaMapName->InitDeviceObjects(g_pD3dDev);
	// end 2012-04-13 by mspark, 아레나 UI 작업
	
	
	return S_OK;
}
HRESULT CINFCityArena::RestoreDeviceObjects()
{
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgRefreshB[i]->RestoreDeviceObjects();
		m_pImgSearch[i]->RestoreDeviceObjects();
	}
	for(i = 0 ; i < ARENA_ROOM_LOCKMODE ; i++)
	{
		m_pImgLock[i]->RestoreDeviceObjects();
	}
	for(i = 0 ; i < ARENA_WAR_MODE_END ; i++)
	{
		m_pImgArenaMode[i]->RestoreDeviceObjects();
	}

	RECT rc;

	// 휠마우스 위치 영역.
	rc.left		= ARENA_LIST_SCROLL_WHELL_AREA_X;
	rc.top		= ARENA_LIST_SCROLL_WHELL_AREA_Y;
	rc.bottom	= ARENA_LIST_SCROLL_WHELL_AREA_H;
	rc.right	= ARENA_LIST_SCROLL_WHELL_AREA_W;
	m_pScroll->SetMouseWhellRect(rc);

	// 마우스 클릭 후 드레그 가능 영역.
	rc.left		= ARENA_LIST_SCROLL_DRAG_AREA_X;
	rc.top		= ARENA_LIST_SCROLL_DRAG_AREA_Y;
    // 2007-05-15 by bhsohn 스크롤 관련 버그 수정
	rc.bottom	= ARENA_LIST_SCROLL_DRAG_AREA_H+ARENA_LIST_SCROLL_DRAG_BALL_CAP;
	rc.right	= ARENA_LIST_SCROLL_DRAG_AREA_W;
	m_pScroll->SetMouseBallRect(rc);

	// 스크롤 인식 범위.
	m_pScroll->SetPosition(ARENA_LIST_SCROLL_DRAG_AREA_X, ARENA_LIST_SCROLL_DRAG_AREA_Y
		, (ARENA_LIST_SCROLL_DRAG_AREA_W)-(ARENA_LIST_SCROLL_DRAG_AREA_X), 
		(ARENA_LIST_SCROLL_DRAG_AREA_H)-(ARENA_LIST_SCROLL_DRAG_AREA_Y));
	m_pScroll->RestoreDeviceObjects();					


	m_pLBoxInf->RestoreDeviceObjects();
	m_pLBoxState->RestoreDeviceObjects();
	m_pFontRoomNum->RestoreDeviceObjects();
	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
	m_pBoldFont->RestoreDeviceObjects();	 

	m_pFontUserID->RestoreDeviceObjects();
	m_pFontPerson->RestoreDeviceObjects();
	m_pFontState->RestoreDeviceObjects();
	m_pFontLevel->RestoreDeviceObjects();

	m_pImgSelectTeam->RestoreDeviceObjects();
	m_pImgAreRoomList->RestoreDeviceObjects();
	m_pImgAreFont->RestoreDeviceObjects();

	m_pArenaCreate->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewArenaBackImage->RestoreDeviceObjects();

	if( m_pArenaCreateBtn )
		m_pArenaCreateBtn->RestoreDeviceObjects();

	if( m_pArenaAppBtn )
		m_pArenaAppBtn->RestoreDeviceObjects();

	if( m_pArenaAppCancelBtn )
		m_pArenaAppCancelBtn->RestoreDeviceObjects();

	if( m_pArenaQuickAppBtn )
		m_pArenaQuickAppBtn->RestoreDeviceObjects();

#endif

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01->RestoreDeviceObjects();
	m_pImgArenaMap02->RestoreDeviceObjects();
	m_pImgArenaMapRandom->RestoreDeviceObjects();
	m_pImgArenaMapShadow->RestoreDeviceObjects();
	m_pFontArenaMapName->RestoreDeviceObjects();
	// end 2012-04-13 by mspark, 아레나 UI 작업


	return S_OK;
}
HRESULT CINFCityArena::InvalidateDeviceObjects()
{
	int i;
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgRefreshB[i]->InvalidateDeviceObjects();
		m_pImgSearch[i]->InvalidateDeviceObjects();
	}
	for(i = 0 ; i < ARENA_ROOM_LOCKMODE ; i++)
	{
		m_pImgLock[i]->InvalidateDeviceObjects();

	}
	for(i = 0 ; i < ARENA_WAR_MODE_END ; i++)
	{
		m_pImgArenaMode[i]->InvalidateDeviceObjects();
	}
	m_pLBoxInf->InvalidateDeviceObjects();
	m_pLBoxState->InvalidateDeviceObjects();
	m_pScroll->InvalidateDeviceObjects();
	m_pFontRoomNum->InvalidateDeviceObjects();
	// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
	m_pBoldFont->InvalidateDeviceObjects();
	
	m_pFontUserID->InvalidateDeviceObjects();
	m_pFontPerson->InvalidateDeviceObjects();
	m_pFontLevel->InvalidateDeviceObjects();
	m_pFontState->InvalidateDeviceObjects();

	
	m_pImgSelectTeam->InvalidateDeviceObjects();
	m_pImgAreRoomList->InvalidateDeviceObjects();
	m_pImgAreFont->InvalidateDeviceObjects();

	m_pArenaCreate->InvalidateDeviceObjects();
												   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewArenaBackImage->InvalidateDeviceObjects();

	if( m_pArenaCreateBtn )
		m_pArenaCreateBtn->InvalidateDeviceObjects();
	if( m_pArenaAppBtn )
		m_pArenaAppBtn->InvalidateDeviceObjects();
	if( m_pArenaAppCancelBtn )
		m_pArenaAppCancelBtn->InvalidateDeviceObjects();
	if( m_pArenaQuickAppBtn )
		m_pArenaQuickAppBtn->InvalidateDeviceObjects();

#endif

	// 2012-04-13 by mspark, 아레나 UI 작업
	m_pImgArenaMap01->InvalidateDeviceObjects();
	m_pImgArenaMap02->InvalidateDeviceObjects();
	m_pImgArenaMapRandom->InvalidateDeviceObjects();
	m_pImgArenaMapShadow->InvalidateDeviceObjects();
	m_pFontArenaMapName->InvalidateDeviceObjects();
	// end 2012-04-13 by mspark, 아레나 UI 작업

	return S_OK;
}
void CINFCityArena::Render()
{
	char buf[64] = { 0, };
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pRenewArenaBackImage->Move(ARENA_LIST_ROOM_X - 1 ,ARENA_LIST_ROOM_Y - 24);
	m_pRenewArenaBackImage->Render();
#else
	m_pImgAreRoomList->Move(ARENA_LIST_ROOM_X,ARENA_LIST_ROOM_Y);
	m_pImgAreRoomList->Render();
#endif
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//	m_pImgAreFont->Move(ARENA_FONT_X,ARENA_FONT_Y);
//	m_pImgAreFont->Render();

#else  
	m_pImgAreFont->Move(ARENA_FONT_X,ARENA_FONT_Y);
	m_pImgAreFont->Render();

#endif

//	m_pImgRefreshB[m_nRefresh]->Move(ARENA_LIST_REFRESH_BUTTON_X,ARENA_LIST_REFRESH_BUTTON_Y);
//	m_pImgRefreshB[m_nRefresh]->Render();
	m_pImgSearch[m_nSearch]->Move(ARENA_TEAM_SEARCH_BUTTON_X,ARENA_TEAM_SEARCH_BUTTON_Y);
	m_pImgSearch[m_nSearch]->Render();

	m_pLBoxInf->Render();
	m_pLBoxState->Render();


	m_pScroll->Render();


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pArenaCreateBtn->SetBtnPosition(ARENA_CREATE_BUTTON_X + 40,ARENA_CREATE_BUTTON_Y);
	m_pArenaCreateBtn->Render();

	m_pArenaAppBtn->SetBtnPosition(ARENA_APP_BUTTON_X + 40,ARENA_APP_BUTTON_Y);
	m_pArenaAppBtn->Render();

	m_pArenaQuickAppBtn->SetBtnPosition(ARENA_QUICKAPP_BUTTON_X + 40,ARENA_QUICKAPP_BUTTON_Y);
	m_pArenaQuickAppBtn->Render();

	m_pArenaAppCancelBtn->SetBtnPosition(ARENA_APPCANCEL_BUTTON_X + 40,ARENA_APPCANCEL_BUTTON_Y);
	m_pArenaAppCancelBtn->Render();

#endif



	SIZE size;
	int i=0;
	// Temp
	
	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//vector<SARENA_TEAM_INFO>::iterator it = m_vecTeamList.begin();
	vector<STRUCT_TEAM_INFO>::iterator it = m_vecTeamList.begin();

	int nCnt = 0;
	for(nCnt = 0;nCnt < m_pScroll->GetScrollStep(); nCnt++)
	{
		if(it == m_vecTeamList.end())
		{
			break;
		}
		// 스크롤 스텝
		it++;
	}
	for(i = 0 ; i < ARENA_LIST_COUNT ; i++)
	{
		if(it == m_vecTeamList.end())
		{// 리스트가 없을시에 통과.
			break;
		}
		// 방번호.
		wsprintf(buf,"%d",it->TeamNum);
		size = m_pFontRoomNum->GetStringSize(buf);
		m_pFontRoomNum->DrawText(ARENA_LIST_ROOM_NUMBER_FONT_X - size.cx,
			ARENA_LIST_ROOM_NUMBER_FONT_Y + (ARENA_MATCH_LIST_GAP * i),
			GUI_FONT_COLOR_W,
			buf);

		// 유저 ID
		if(ARENA_REQUEST_TEAM_STATE_OTHER == m_BRequestTeamState)
			wsprintf(buf,STRMSG_C_070612_0100);
		else
			wsprintf(buf,"%s",it->CharacterName);
		size = m_pFontUserID->GetStringSize(buf);
		// 2008-03-10 by bhsohn 이벤트 아레나방 관련처리
		float fNamePosX = ARENA_LIST_ROOM_ID_FONT_X - (size.cx/2);
		if(it->TeamEvent)
		{			
			float fEventPosX = ARENA_LIST_ROOM_EVENT_FONT_X;
			char szEventbuf[64];
			wsprintf(szEventbuf,STRMSG_C_080225_0206);
			m_pBoldFont->DrawText(fEventPosX,
				ARENA_LIST_ROOM_EVENT_FONT_Y + (ARENA_MATCH_LIST_GAP * i),
				GUI_FONT_COLOR_W,
				szEventbuf);
			// 위치갱신
			SIZE sizeEvent = m_pBoldFont->GetStringSize(szEventbuf);
			fNamePosX = (sizeEvent.cx/2) + ARENA_LIST_ROOM_ID_FONT_X - (size.cx/2);
		}		

		//m_pFontRoomNum->DrawText(ARENA_LIST_ROOM_ID_FONT_X - (size.cx/2),
		m_pFontRoomNum->DrawText(fNamePosX,
			ARENA_LIST_ROOM_ID_FONT_Y + (ARENA_MATCH_LIST_GAP * i),
			GUI_FONT_COLOR_W,
			buf);

		// 모드.
		// it->Arenamode 1 : 데스매치 2 : 라운드아레나.
		m_pImgArenaMode[it->ArenaMode-1]->Move(ARENA_LIST_ROOM_MODE_X - (m_pImgArenaMode[it->ArenaMode-1]->GetImgSize().x / 2) 
			,ARENA_LIST_ROOM_MODE_Y + (ARENA_MATCH_LIST_GAP * i));
		m_pImgArenaMode[it->ArenaMode-1]->Render();

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
		// 2012-04-13 by mspark, 아레나 UI 작업
		if(it->ArenaMap == ARENA_MAP_01_NUMBER)
		{
			m_pImgArenaMap01->Move(ARENA_LIST_ROOM_MAP_X, ARENA_LIST_ROOM_MAP_Y + (ARENA_MATCH_LIST_GAP * i));
			m_pImgArenaMap01->SetScale(0.45f, 0.45f);
			m_pImgArenaMap01->Render();
		}		
		
		if(it->ArenaMap == ARENA_MAP_02_NUMBER)
		{
			m_pImgArenaMap02->Move(ARENA_LIST_ROOM_MAP_X, ARENA_LIST_ROOM_MAP_Y + (ARENA_MATCH_LIST_GAP * i));
			m_pImgArenaMap02->SetScale(0.45f, 0.45f);
			m_pImgArenaMap02->Render();
		}

		if(it->ArenaMap == ARENA_MAP_RANDOM_NUMBER)
		{
			m_pImgArenaMapRandom->Move(ARENA_LIST_ROOM_MAP_X, ARENA_LIST_ROOM_MAP_Y + (ARENA_MATCH_LIST_GAP * i));
			m_pImgArenaMapRandom->SetScale(0.45f, 0.45f);
			m_pImgArenaMapRandom->Render();
		}		
		// 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

		// 레벨 
		wsprintf(buf,"%d-%d",it->TeamStartLevel,it->TeamEndLevel);
		size = m_pFontLevel->GetStringSize(buf);
		m_pFontLevel->DrawText(ARENA_LIST_ROOM_LEVEL_X - (size.cx/2),
			ARENA_LIST_ROOM_LEVEL_Y + (ARENA_MATCH_LIST_GAP * i),
			GUI_FONT_COLOR_W,
			buf);


		// 인원수.
		wsprintf(buf,"%d/%d",it->CurrentTeamSize,it->TeamSize);
		size = m_pFontPerson->GetStringSize(buf);
		m_pFontPerson->DrawText(ARENA_LIST_ROOM_PERSON_X - (size.cx/2),
			ARENA_LIST_ROOM_PERSON_Y + (ARENA_MATCH_LIST_GAP * i),
			GUI_FONT_COLOR_W,
			buf);
		// 공개 여부.
		m_pImgLock[it->TeamLock]->Move(ARENA_LIST_ROOM_LOCK_X,ARENA_LIST_ROOM_LOCK_Y + (ARENA_MATCH_LIST_GAP * i));
		m_pImgLock[it->TeamLock]->Render();

		// 상태.
		if(it->ArenaState == ARENA_STATE_JOINING 
			|| it->ArenaState == ARENA_STATE_READY)
		{
			wsprintf(buf,STRMSG_C_070607_0100);
			size = m_pFontState->GetStringSize(buf);
			m_pFontState->DrawText(ARENA_LIST_ROOM_STATE_X - (size.cx/2),
				ARENA_LIST_ROOM_STATE_Y + (ARENA_MATCH_LIST_GAP * i),
				GUI_FONT_COLOR_G,
				buf);
		}
		else if(it->ArenaState == ARENA_STATE_READY_FINISH)
		{
			wsprintf(buf,STRMSG_C_070607_0101);
			size = m_pFontState->GetStringSize(buf);
			m_pFontState->DrawText(ARENA_LIST_ROOM_STATE_X - (size.cx/2),
				ARENA_LIST_ROOM_STATE_Y + (ARENA_MATCH_LIST_GAP * i),
				GUI_FONT_COLOR_Y,
				buf);
		}
		else if(it->ArenaState > ARENA_STATE_READY_FINISH)
		{
			wsprintf(buf,STRMSG_C_070607_0102);
			size = m_pFontState->GetStringSize(buf);
			m_pFontState->DrawText(ARENA_LIST_ROOM_STATE_X - (size.cx/2),
				ARENA_LIST_ROOM_STATE_Y + (ARENA_MATCH_LIST_GAP * i),
				GUI_FONT_COLOR_R,
				buf);
		}

		// 현재 선택한 목록인가.
		if(m_nSelectIndex == i)
		{
			m_pImgSelectTeam->Move(ARENA_LIST_SELECT_X,ARENA_LIST_SELECT_Y + (ARENA_MATCH_LIST_GAP * i));
			m_pImgSelectTeam->Render();
		}
		it++;
	}

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
	// 2012-04-13 by mspark, 아레나 UI 작업
	RenderArenaListTooltip();	
	// end 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

	if(m_pArenaCreate)
		m_pArenaCreate->Render();

}

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
// 2012-04-13 by mspark, 아레나 UI 작업
void CINFCityArena::RenderArenaListTooltip()
{
	char buf[64] = { 0, };
	SIZE size;
	int i=0;

	vector<STRUCT_TEAM_INFO>::iterator iter = m_vecTeamList.begin();
	
	for(i = 0 ; i < ARENA_LIST_COUNT ; i++)
	{
		if(iter == m_vecTeamList.end())
		{// 리스트가 없을시에 통과.
			break;
		}
		
		if(m_bMouseOverMap == TRUE && m_nCurrentStayIndex == i)
		{				
			if(iter->ArenaMap == ARENA_MAP_01_NUMBER) 
			{
				g_pGameMain->m_pInfWindow->RenderCenterWindow( m_ptMousePos.x, m_ptMousePos.y, ARENA_LIST_TOOLTIPBACK_IMAGE_W, ARENA_LIST_TOOLTIPBACK_IMAGE_H, FALSE );
				
				m_pImgArenaMapShadow->Move(m_ptMousePos.x + ARENA_LIST_TOOLTIPMAP_IMAGE_X - 3.0f , m_ptMousePos.y + ARENA_LIST_TOOLTIPMAP_IMAGE_Y - 3.0f);
				m_pImgArenaMapShadow->SetScale(1.0f, 1.0f);
				m_pImgArenaMapShadow->Render();

				m_pImgArenaMap01->Move(m_ptMousePos.x + ARENA_LIST_TOOLTIPMAP_IMAGE_X, m_ptMousePos.y + ARENA_LIST_TOOLTIPMAP_IMAGE_Y);
				m_pImgArenaMap01->SetScale(1.0f, 1.0f);
				m_pImgArenaMap01->Render();
				
				wsprintf(buf,"%s", STRMSG_C_120413_0001);
				size = m_pFontLevel->GetStringSize(buf);
				m_pFontArenaMapName->DrawText(m_ptMousePos.x + ARENA_LIST_TOOLTIPFONT_IMAGE_X - (size.cx/2) - 3,
					m_ptMousePos.y + ARENA_LIST_TOOLTIPFONT_IMAGE_Y,
					GUI_FONT_COLOR_W,
					buf);
			}
			
			if(iter->ArenaMap == ARENA_MAP_02_NUMBER)
			{
				g_pGameMain->m_pInfWindow->RenderCenterWindow( m_ptMousePos.x, m_ptMousePos.y, ARENA_LIST_TOOLTIPBACK_IMAGE_W, ARENA_LIST_TOOLTIPBACK_IMAGE_H, FALSE );
				
				m_pImgArenaMapShadow->Move(m_ptMousePos.x + ARENA_LIST_TOOLTIPMAP_IMAGE_X - 3.0f , m_ptMousePos.y + ARENA_LIST_TOOLTIPMAP_IMAGE_Y - 3.0f);
				m_pImgArenaMapShadow->SetScale(1.0f, 1.0f);
				m_pImgArenaMapShadow->Render();

				m_pImgArenaMap02->Move(m_ptMousePos.x + ARENA_LIST_TOOLTIPMAP_IMAGE_X, m_ptMousePos.y + ARENA_LIST_TOOLTIPMAP_IMAGE_Y);
				m_pImgArenaMap02->SetScale(1.0f, 1.0f);
				m_pImgArenaMap02->Render();
				
				wsprintf(buf,"%s", STRMSG_C_120413_0002);
				size = m_pFontLevel->GetStringSize(buf);
				m_pFontArenaMapName->DrawText(m_ptMousePos.x + ARENA_LIST_TOOLTIPFONT_IMAGE_X - (size.cx/2) - 3,
					m_ptMousePos.y + ARENA_LIST_TOOLTIPFONT_IMAGE_Y,
					GUI_FONT_COLOR_W,
					buf);
			}
			
			if(iter->ArenaMap == ARENA_MAP_RANDOM_NUMBER)
			{
				g_pGameMain->m_pInfWindow->RenderCenterWindow( m_ptMousePos.x, m_ptMousePos.y, ARENA_LIST_TOOLTIPBACK_IMAGE_W, ARENA_LIST_TOOLTIPBACK_IMAGE_H, FALSE );
				
				m_pImgArenaMapShadow->Move(m_ptMousePos.x + ARENA_LIST_TOOLTIPMAP_IMAGE_X - 3.0f , m_ptMousePos.y + ARENA_LIST_TOOLTIPMAP_IMAGE_Y - 3.0f);
				m_pImgArenaMapShadow->SetScale(1.0f, 1.0f);
				m_pImgArenaMapShadow->Render();

				m_pImgArenaMapRandom->Move(m_ptMousePos.x + ARENA_LIST_TOOLTIPMAP_IMAGE_X, m_ptMousePos.y + ARENA_LIST_TOOLTIPMAP_IMAGE_Y);
				m_pImgArenaMapRandom->SetScale(1.0f, 1.0f);
				m_pImgArenaMapRandom->Render();
				
				wsprintf(buf,"%s", STRMSG_C_120413_0003);
				size = m_pFontLevel->GetStringSize(buf);
				m_pFontArenaMapName->DrawText(m_ptMousePos.x + ARENA_LIST_TOOLTIPFONT_IMAGE_X - (size.cx/2) - 3,
					m_ptMousePos.y + ARENA_LIST_TOOLTIPFONT_IMAGE_Y,
					GUI_FONT_COLOR_W,
					buf);
			}
		}
		iter++;
	}
}
// end 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

VOID CINFCityArena::CreateRoom()
{
	// 아레나에 참가한 상태라면 리턴.
	if(g_pInterface->m_pGameArena)
	{
		if(g_pInterface->m_pGameArena->IsArenaGameState(ARENA_GAME_STATE_NORMAL))
		{
			return;
		}
	}
	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
		return;
	}
	// end 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	// 2008-03-10 by bhsohn 아레나 관련 버그 수정
	if(TRUE == g_pInterface->IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0207,COLOR_ERROR);//"이미 아레나 대기방에 참여중입니다."
		return;	
	}
	// end 2008-03-10 by bhsohn 아레나 관련 버그 수정

	m_pArenaCreate->ShowCreateWindow();
	m_pArenaCreate->SetPos(ARENA_START_CREATE_X,ARENA_START_CREATE_Y);
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_CREATE_TEAMINFO,NULL,0);
}
VOID CINFCityArena::Init()
{
	if(m_pArenaCreate)
		m_pArenaCreate->m_bShow = FALSE;
}

BOOL CINFCityArena::App()
{
	// 아레나에 참가한 상태라면 리턴.
	if(g_pInterface->m_pGameArena)
	{
		if(g_pInterface->m_pGameArena->IsArenaGameState(ARENA_GAME_STATE_NORMAL))
		{
			return FALSE;
		}
	}
	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
		return FALSE;
	}
	// end 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	// 2008-03-10 by bhsohn 아레나 관련 버그 수정
	if(TRUE == g_pInterface->IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0207,COLOR_ERROR);//"이미 아레나 대기방에 참여중입니다."
		return FALSE;	
	}
	// end 2008-03-10 by bhsohn 아레나 관련 버그 수정

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//SARENA_TEAM_INFO * TeamInfo = GetSelectListTeam();
	STRUCT_TEAM_INFO* TeamInfo = GetSelectListTeam();
	if(TeamInfo == NULL)
		return FALSE;
	if(TeamInfo->TeamLock)
	{// 비번이 있을경우.
		g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_070430_0100, _Q_ARENA_PASSWORD);
	}else
	{// 비번이 없을경우.
		// 2007-11-22 by bhsohn 아레나 통합서버
//		MSG_FC_ARENA_ENTER_TEAM sMsg;
//		sMsg.ArenaMode = TeamInfo->ArenaMode;
//		sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//		sMsg.TeamNum = TeamInfo->TeamNum;
//		memset(sMsg.TeamPW,0x00,sizeof(sMsg.TeamPW));
//		g_pFieldWinSocket->SendMsg(T_FC_ARENA_ENTER_TEAM,(char*)&sMsg,sizeof(sMsg));
		SendArenaEnterRoom(NULL);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFCityArena::EnemyInfo()
/// \brief		상대팀정보 요청.
/// \author		dgwoo
/// \date		2007-05-25 ~ 2007-05-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFCityArena::EnemyInfo()
{
	MSG_FC_ARENA_REQUEST_TEAM sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_ARENA_REQUEST_TEAM));
	
	sMsg.ArenaMode = m_BArenaMode;
	// 2007-11-22 by bhsohn 아레나 통합서버
	//sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	CHARACTER*	pArenaCharacterInfo = g_pD3dApp->GetArenaCharacterInfo();
	sMsg.ClientIndex = pArenaCharacterInfo->ClientIndex;
	// end 2007-11-22 by bhsohn 아레나 통합서버

	m_BRequestTeamState = ARENA_REQUEST_TEAM_STATE_OTHER;
	sMsg.RequestTeamState = m_BRequestTeamState;
	sMsg.StartTeamNum = -1;
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_TEAM,(char*)&sMsg,sizeof(sMsg));

	// 2007-11-22 by bhsohn 아레나 통합서버
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_ARENA_REQUEST_TEAM);		
		vecUnLockMsg.push_back(T_FC_ARENA_REQUEST_TEAM_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_ARENA_REQUEST_TEAM, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	
	// end 2007-11-22 by bhsohn 아레나 통합서버
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFCityArena::QuickApp()
/// \brief		빠른 신청.
/// \author		dgwoo
/// \date		2007-06-07 ~ 2007-06-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFCityArena::QuickApp()
{
	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
		return;
	}
	// end 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	// 2008-03-10 by bhsohn 아레나 관련 버그 수정
	if(TRUE == g_pInterface->IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0207,COLOR_ERROR);//"이미 아레나 대기방에 참여중입니다."
		return ;	
	}
	// end 2008-03-10 by bhsohn 아레나 관련 버그 수정
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_QUICK_ENTER_TEAM,NULL,0);
}
// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
//SARENA_TEAM_INFO* CINFCityArena::GetSelectListTeam()
STRUCT_TEAM_INFO* CINFCityArena::GetSelectListTeam()
{
	if(m_nSelectIndex < 0)
		return NULL;
	int nStep = m_pScroll->GetScrollStep();
	nStep = nStep + m_nSelectIndex;
	if(m_vecTeamList.size() <= nStep)
		return NULL;
	return &m_vecTeamList[nStep];
	
}

VOID CINFCityArena::AppCancel()
{
	if(m_pArenaCreate)
	{
		m_pArenaCreate->m_bShow = FALSE;
	}
	// 2007-11-22 by bhsohn 아레나 통합서버
//	MSG_FC_ARENA_REAVE_TEAM sMsg;
//	sMsg.ArenaMode = m_BArenaMode;
//	sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	if(g_pInterface->m_pGameArena->GetTeamNum() >= 0)
	{// 방번호가 없을때는 오류.
//		sMsg.TeamNum = g_pInterface->m_pGameArena->GetTeamNum();
//		g_pFieldWinSocket->SendMsg(T_FC_ARENA_REAVE_TEAM,(char*)&sMsg,sizeof(sMsg));
		g_pInterface->m_pGameArena->SendArenaLeaveTeam(m_BArenaMode);

	}
	
}
VOID CINFCityArena::ReFresh()
{
	// 2007-11-22 by bhsohn 아레나 통합서버
//	MSG_FC_ARENA_REQUEST_TEAM sMsg;
//	sMsg.ArenaMode = m_BArenaMode;					
//	//sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//	sMsg.ClientIndex = pArenaCharacterInfo->ClientIndex;
//	sMsg.RequestTeamState = m_BRequestTeamState;
//	sMsg.StartTeamNum = -1;
//	m_vecTeamList.clear();
//	g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_TEAM,(char*)&sMsg,sizeof(sMsg));
	m_vecTeamList.clear();
	SendArenaRequestTeam(-1);	
}

BYTE CINFCityArena::ChangeTab(BYTE BMode)
{
	m_nTab = BMode;
	return BMode;
}
VOID CINFCityArena::AllModePersonInfo(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK *pMsg)
{
	SARENA_REQUEST_CREATE_TEAMINFO *pTeamInfo = (SARENA_REQUEST_CREATE_TEAMINFO *)((char *)pMsg + sizeof(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK));
	int i = 0;
	m_pArenaCreate->m_nMode = 0;
	m_pArenaCreate->ClearAllPersons();
	for(i = 0 ; i < pMsg->ArenaRequetCreateTeamInfoListCount ; i++)
	{
		m_pArenaCreate->AddAllPersons(pTeamInfo);
		pTeamInfo++;
	}
	// 현재 등록된 인원수를 모드에 맞게 다시 셋팅해준다.
	m_pArenaCreate->SetPersons(m_pArenaCreate->m_nSelectMode);

}
void CINFCityArena::TestDB()
{
	int nItemLen = 10;
	long lBuffLen = sizeof(MSG_FC_ARENA_REQUEST_TEAM_OK)+(sizeof(SARENA_TEAM_INFO)*nItemLen);
	MSG_FC_ARENA_REQUEST_TEAM_OK *pMsg = (MSG_FC_ARENA_REQUEST_TEAM_OK *)malloc(lBuffLen);
	memset(pMsg, 0x00, lBuffLen);

	SARENA_TEAM_INFO *pTeamInfo = (SARENA_TEAM_INFO *)((char *)pMsg + sizeof(MSG_FC_ARENA_REQUEST_TEAM_OK));

	pMsg->ArenaTeamInfoTotalCount = nItemLen;
	pMsg->ArenaTeamInfoListCount = nItemLen;

	for(int nCnt = 0;nCnt < nItemLen;nCnt++)
	{
		pTeamInfo->TeamLock = 0;
		pTeamInfo->TeamSize = 6;
		pTeamInfo->CurrentTeamSize = 1;
		pTeamInfo->ArenaMode = 1;		
		pTeamInfo->TeamNum = 0;
		wsprintf(pTeamInfo->CharacterName, "Team%d", nCnt);
		pTeamInfo++;
	}
	ArenaTeamListInput(pMsg);
	free(pMsg);
}

VOID CINFCityArena::ArenaTeamListInput(MSG_FC_ARENA_REQUEST_TEAM_OK *pMsg)
{
	SARENA_TEAM_INFO *pTeamInfo = (SARENA_TEAM_INFO*)((char *)pMsg + sizeof(MSG_FC_ARENA_REQUEST_TEAM_OK));
// 2007-10-25 by dgwoo
//	m_vecTeamList.clear();
	m_nTeamTotalCount =  pMsg->ArenaTeamInfoTotalCount;
	m_BRequestTeamState = pMsg->RequetTeamState;
	int i = 0;
	for(i = 0 ; i < pMsg->ArenaTeamInfoListCount ; i++)
	{
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		STRUCT_TEAM_INFO sTeamInfo;
		memset(&sTeamInfo, 0x00, sizeof(STRUCT_TEAM_INFO));
		{			
			sTeamInfo.TeamLock = pTeamInfo->TeamLock;
			sTeamInfo.TeamEvent = pTeamInfo->TeamEvent;
			sTeamInfo.TeamSize = pTeamInfo->TeamSize;			
			sTeamInfo.CurrentTeamSize = pTeamInfo->CurrentTeamSize;		
			sTeamInfo.ArenaMode = pTeamInfo->ArenaMode;		
			sTeamInfo.ArenaState = pTeamInfo->ArenaState;					
			sTeamInfo.TeamStartLevel = pTeamInfo->TeamStartLevel;					
			sTeamInfo.TeamEndLevel = pTeamInfo->TeamEndLevel;					
			sTeamInfo.TeamNum = pTeamInfo->TeamNum;								
			strncpy(sTeamInfo.CharacterName, pTeamInfo->CharacterName, SIZE_MAX_CHARACTER_NAME);
			sTeamInfo.ArenaMap = pTeamInfo->ArenaMap;	// 2012-04-13 by mspark, 아레나 UI 작업
		}		
		g_pD3dApp->ConevertArenaRenderUserName(ARENA_STATE_ARENA_GAMING, sTeamInfo.CharacterName);
		//m_vecTeamList.push_back((*pTeamInfo));
		m_vecTeamList.push_back(sTeamInfo);
		// end 2007-11-22 by bhsohn 아레나 통합서버
		
		
		pTeamInfo++;
	}
	m_pScroll->SetOnlyMaxItem(m_nTeamTotalCount);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFCityArena::IsInfluenceEqual()
/// \brief		리스트창의 선택한 값이 나와 같은 세력이라면. 
/// \author		dgwoo
/// \date		2007-06-11 ~ 2007-06-11
/// \warning	
///
/// \param		
/// \return		같은세력 : TRUE    ,    다른 세력 : FALSE
///////////////////////////////////////////////////////////////////////////////
BOOL CINFCityArena::IsInfluenceEqual()
{
	// 2007-11-22 by bhsohn 아레나 통합서버
//	BOOL bBCUInfSelect = FALSE;
//	BOOL bBCUInfCha	= FALSE;
//	if(m_pLBoxInf->GetSelect() == 0)
//		bBCUInfSelect = TRUE;
//	if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_VCN))
//		bBCUInfCha = TRUE;
//	if(bBCUInfSelect == bBCUInfCha)
//		return TRUE;
//	else
//		return FALSE;
	// 아군이 처음으로 바뀌었다.
	if(m_pLBoxInf->GetSelect() == 0)
	{
		return TRUE;
	}
	return FALSE;
}
VOID CINFCityArena::RequestTeamList()
{
	m_vecTeamList.clear();
	// 2007-10-26 by dgwoo 갱신시 스크롤 초기화.
	m_pScroll->SetMaxItem(1);		
	m_nTeamTotalCount = 0;

	if(IsInfluenceEqual())
	{
		// 2007-11-22 by bhsohn 아레나 통합서버
		// 같은 세력이면.
//		MSG_FC_ARENA_REQUEST_TEAM sMsg;
//		sMsg.ArenaMode = m_BArenaMode;
//		sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
		if(m_pLBoxState->GetSelect() == 0)
		{
			m_BRequestTeamState = ARENA_REQUEST_TEAM_STATE_OUR;
			
		}else
		{
			m_BRequestTeamState = ARENA_REQUEST_TEAM_STATE_JOINING;
		}
//		sMsg.RequestTeamState = m_BRequestTeamState;
//		sMsg.StartTeamNum = -1;
//		g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_TEAM,(char*)&sMsg,sizeof(sMsg));
		SendArenaRequestTeam(-1);
		// end 2007-11-22 by bhsohn 아레나 통합서버
	}else
	{// 다른 세력이면.
		EnemyInfo();
	}
}

int CINFCityArena::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	BOOL bClick = FALSE;	
	bClick = m_pScroll->IsMouseWhellPos(pt);
	if(bClick)		
	{
		int nScrollStep = m_pScroll->OnMouseWheel(wParam, lParam);

		// 팀 목록 리스트를 새로 받아야 할 경우를 체크한다.
		int nListSize = m_vecTeamList.size();
		if(nListSize < m_nTeamTotalCount)
		{
			if(nListSize <= nScrollStep + ARENA_LIST_COUNT)
			{// 팀 목록중 가장 아랫 부분이다. 방목록을 새로 요청하도록한다.
				// 2007-11-22 by bhsohn 아레나 통합서버
//				MSG_FC_ARENA_REQUEST_TEAM sMsg;
//				sMsg.ArenaMode = m_BArenaMode;
//				sMsg.RequestTeamState = m_BRequestTeamState;
//				sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//				sMsg.StartTeamNum = m_vecTeamList[nListSize-1].TeamNum;
//				g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_TEAM,(char*)&sMsg,sizeof(sMsg));				
				SendArenaRequestTeam(m_vecTeamList[nListSize-1].TeamNum);
				// end 2007-11-22 by bhsohn 아레나 통합서버
			}
		}
		return INF_MSGPROC_BREAK;
	}
	return INF_MSGPROC_NORMAL;	
}
int CINFCityArena::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	int nCnt = 0;
	BOOL bClick = FALSE;	
	bClick = m_pScroll->IsMouseBallPos(pt);
	if(bClick)		
	{
		m_pScroll->SetMouseMoveMode(TRUE);
		return INF_MSGPROC_BREAK;
		
	}	
	return INF_MSGPROC_NORMAL;
}
int CINFCityArena::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	int nCnt = 0;
	BOOL bClick = FALSE;	
	bClick = m_pScroll->GetMouseMoveMode();
	if(bClick)		
	{
		m_pScroll->SetMouseMoveMode(FALSE);			
	}		
	return INF_MSGPROC_NORMAL;
}

int CINFCityArena::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	if(m_pScroll->GetMouseMoveMode())
	{
		if(FALSE == m_pScroll->IsMouseScrollPos(pt))
		{// 스크롤 잡고 있는걸 해제한다.
			m_pScroll->SetMouseMoveMode(FALSE);
			return INF_MSGPROC_NORMAL;			
		}
		m_pScroll->SetScrollPos(pt);
		int nScrollStep = m_pScroll->GetScrollStep();
		
		// 팀 목록 리스트를 새로 받아야 할 경우를 체크한다.
		int nListSize = m_vecTeamList.size();
		if(nListSize < m_nTeamTotalCount)
		{
			if(nListSize <= nScrollStep + ARENA_LIST_COUNT)
			{// 팀 목록중 가장 아랫 부분이다. 방목록을 새로 요청하도록한다.
				// 2007-11-22 by bhsohn 아레나 통합서버
//				MSG_FC_ARENA_REQUEST_TEAM sMsg;
//				sMsg.ArenaMode = m_BArenaMode;					
//				sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//				sMsg.StartTeamNum = m_vecTeamList[nListSize-1].TeamNum;
//				g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_TEAM,(char*)&sMsg,sizeof(sMsg));
				SendArenaRequestTeam(m_vecTeamList[nListSize-1].TeamNum);
			}
		}
		return INF_MSGPROC_BREAK;			
	}
	return INF_MSGPROC_NORMAL;
}
void CINFCityArena::Tick()
{

	if(m_pArenaCreate)
		m_pArenaCreate->Tick();
}
int CINFCityArena::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pArenaCreate->m_bShow)
	{
		if(INF_MSGPROC_BREAK == m_pArenaCreate->WndProc(uMsg,wParam,lParam))
			return INF_MSGPROC_BREAK;
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
		case WM_MOUSEWHEEL:
			{
				return OnMouseWheel(wParam,lParam);
			}
			break;

		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
											  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if(m_pArenaCreateBtn->OnMouseMove(pt))
					return INF_MSGPROC_BREAK;
				if(m_pArenaAppBtn->OnMouseMove(pt))
					return INF_MSGPROC_BREAK;
				if(m_pArenaAppCancelBtn->OnMouseMove(pt))
					return INF_MSGPROC_BREAK;
				if(m_pArenaQuickAppBtn->OnMouseMove(pt))
					return INF_MSGPROC_BREAK;
#endif


				m_pLBoxInf->MouseMove(pt);
				m_pLBoxState->MouseMove(pt);

				if(pt.x > ARENA_LIST_REFRESH_BUTTON_X &&
					pt.x < ARENA_LIST_REFRESH_BUTTON_X + ARENA_LIST_REFRESH_BUTTON_W &&
					pt.y > ARENA_LIST_REFRESH_BUTTON_Y &&
					pt.y < ARENA_LIST_REFRESH_BUTTON_Y + ARENA_LIST_REFRESH_BUTTON_H)
				{
					m_nRefresh = BUTTON_STATE_UP;
				}
				else
				{
					m_nRefresh = BUTTON_STATE_NORMAL;
				}
				if(pt.x > ARENA_TEAM_SEARCH_BUTTON_X &&
					pt.x < ARENA_TEAM_SEARCH_BUTTON_X + ARENA_TEAM_SEARCH_BUTTON_W &&
					pt.y > ARENA_TEAM_SEARCH_BUTTON_Y &&
					pt.y < ARENA_TEAM_SEARCH_BUTTON_Y + ARENA_TEAM_SEARCH_BUTTON_H)
				{
					m_nSearch = BUTTON_STATE_UP;
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP4");
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
				}
				else
				{
					m_nSearch = BUTTON_STATE_NORMAL;
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				}

// 2012-05-29 by mspark, 아레나 통합(전달)
#ifdef C_ARENA_EX_1ST_MSPARK
				// 2012-04-13 by mspark, 아레나 UI 작업
				m_ptMousePos.x = pt.x;
				m_ptMousePos.y = pt.y;

				if(pt.x > ARENA_LIST_SELECT_X &&
					pt.x < ARENA_LIST_SELECT_X + ARENA_LIST_W &&
					pt.y > ARENA_LIST_SELECT_Y &&
					pt.y < ARENA_LIST_SELECT_Y + ARENA_LIST_H)
				{
					int temp = ARENA_LIST_SELECT_Y;
					temp = (pt.y - temp);
					temp = (int)temp / ARENA_MATCH_LIST_GAP;
					if(temp >= 0 &&
						temp < ARENA_LIST_COUNT)
					{
						m_nCurrentStayIndex = temp;
					}

					if(pt.x > ARENA_LIST_ROOM_MAP_X &&
						pt.x < ARENA_LIST_ROOM_MAP_X + 40 &&
						pt.y > ARENA_LIST_ROOM_MAP_Y + (ARENA_MATCH_LIST_GAP * m_nCurrentStayIndex) &&
						pt.y < ARENA_LIST_ROOM_MAP_Y + 25 + (ARENA_MATCH_LIST_GAP * m_nCurrentStayIndex))
					{
						m_bMouseOverMap = TRUE;
					}
					else
					{
						m_bMouseOverMap = FALSE;
					}
				}
				else
				{
					m_bMouseOverMap = FALSE;
				}
				// 2012-04-13 by mspark, 아레나 UI 작업
#endif
// end 2012-05-29 by mspark, 아레나 통합(전달)

				return OnMouseMove(wParam,lParam);

			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if(m_pArenaCreateBtn->OnLButtonDown(pt))
					return INF_MSGPROC_BREAK;
				
				if(m_pArenaAppBtn->OnLButtonDown(pt))
					return INF_MSGPROC_BREAK;
				
				if(m_pArenaAppCancelBtn->OnLButtonDown(pt))
					return INF_MSGPROC_BREAK;
				
				if(m_pArenaQuickAppBtn->OnLButtonDown(pt))
					return INF_MSGPROC_BREAK;

#endif
				
				if(1 == m_pLBoxInf->LButtonDown(pt))
				{
					if(IsInfluenceEqual())
					{// 같은 세력이면.
						m_pLBoxState->SetMainButtonEnisable();
					}
					else
					{
						m_pLBoxState->SetMainButtonDisable();
						m_pLBoxState->SetSelectItem(1);
						m_pLBoxState->m_nSelect = 0;
					}
				}
				m_pLBoxState->LButtonDown(pt);
					
				if(pt.x > ARENA_LIST_REFRESH_BUTTON_X &&
					pt.x < ARENA_LIST_REFRESH_BUTTON_X + ARENA_LIST_REFRESH_BUTTON_W &&
					pt.y > ARENA_LIST_REFRESH_BUTTON_Y &&
					pt.y < ARENA_LIST_REFRESH_BUTTON_Y + ARENA_LIST_REFRESH_BUTTON_H)
				{
					m_nRefresh = BUTTON_STATE_DOWN;
				}
				else
				{
					m_nRefresh = BUTTON_STATE_NORMAL;
				}
				if(pt.x > ARENA_TEAM_SEARCH_BUTTON_X &&
					pt.x < ARENA_TEAM_SEARCH_BUTTON_X + ARENA_TEAM_SEARCH_BUTTON_W &&
					pt.y > ARENA_TEAM_SEARCH_BUTTON_Y &&
					pt.y < ARENA_TEAM_SEARCH_BUTTON_Y + ARENA_TEAM_SEARCH_BUTTON_H)
				{
					m_nSearch = BUTTON_STATE_DOWN;
				}
				else
				{
					m_nSearch = BUTTON_STATE_NORMAL;
				}
				// 방 선택.
				if(pt.x > ARENA_LIST_SELECT_X &&
					pt.x < ARENA_LIST_SELECT_X + ARENA_LIST_W &&
					pt.y > ARENA_LIST_SELECT_Y &&
					pt.y < ARENA_LIST_SELECT_Y + ARENA_LIST_H)
				{
					int temp = ARENA_LIST_SELECT_Y;
					temp = (pt.y - temp);
					temp = (int)temp / ARENA_MATCH_LIST_GAP;
					if(temp >= 0 &&
						temp < ARENA_LIST_COUNT)
					{
						m_nSelectIndex = temp;
						
					}
				}
				return OnLButtonDown(wParam,lParam);
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				if( m_pArenaCreateBtn->OnLButtonUp(pt) )
				{
					CreateRoom();
				}
				if( m_pArenaAppBtn->OnLButtonUp(pt) )
				{
					App();
				}
				if( m_pArenaAppCancelBtn->OnLButtonUp(pt) )
				{
					AppCancel();
				}
				if( m_pArenaQuickAppBtn->OnLButtonUp(pt) )
				{
					QuickApp();
				}

#endif
				


				if(pt.x > ARENA_TEAM_SEARCH_BUTTON_X &&
					pt.x < ARENA_TEAM_SEARCH_BUTTON_X + ARENA_TEAM_SEARCH_BUTTON_W &&
					pt.y > ARENA_TEAM_SEARCH_BUTTON_Y &&
					pt.y < ARENA_TEAM_SEARCH_BUTTON_Y + ARENA_TEAM_SEARCH_BUTTON_H)
				{
					RequestTeamList();					
				}
				else
				{
					m_nSearch = BUTTON_STATE_NORMAL;
				}

				if(pt.x > ARENA_LIST_REFRESH_BUTTON_X &&
					pt.x < ARENA_LIST_REFRESH_BUTTON_X + ARENA_LIST_REFRESH_BUTTON_W &&
					pt.y > ARENA_LIST_REFRESH_BUTTON_Y &&
					pt.y < ARENA_LIST_REFRESH_BUTTON_Y + ARENA_LIST_REFRESH_BUTTON_H)
				{
					m_nRefresh = BUTTON_STATE_UP;
//					ReFresh();
				}
				else
				{
					m_nRefresh = BUTTON_STATE_NORMAL;
				}

				return OnLButtonUp(wParam,lParam);

			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				
				// 방 선택.
				if(pt.x > ARENA_LIST_SELECT_X &&
					pt.x < ARENA_LIST_SELECT_X + ARENA_LIST_W &&
					pt.y > ARENA_LIST_SELECT_Y &&
					pt.y < ARENA_LIST_SELECT_Y + ARENA_LIST_H)
				{
					App();
//					int temp = ARENA_LIST_SELECT_Y;
//					temp = (pt.y - temp);
//					temp = (int)temp / ARENA_MATCH_LIST_GAP;
//					if(temp >= 0 &&
//						temp < ARENA_LIST_COUNT)
//					{
//						m_nSelectIndex = temp;
//					}
				}
				return OnLButtonDown(wParam,lParam);

			}
			break;

	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 아레나 팀 요청
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
void CINFCityArena::SendArenaRequestTeam(int nStartTeamNum)
{	
	// 2008-04-24 by bhsohn 아레나 상점에서 팀 정보를 요청하고 마우스로 스크롤을 조작하면 클라이언트 종료되는 현상
	if(g_pD3dApp->IsLockMode() == FALSE)
	{
		return;
	}
	// end 2008-04-24 by bhsohn 아레나 상점에서 팀 정보를 요청하고 마우스로 스크롤을 조작하면 클라이언트 종료되는 현상
	CHARACTER*	pArenaCharacterInfo = g_pD3dApp->GetArenaCharacterInfo();
	MSG_FC_ARENA_REQUEST_TEAM sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_ARENA_REQUEST_TEAM));

	sMsg.ArenaMode = m_BArenaMode;					
	//sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	sMsg.ClientIndex = pArenaCharacterInfo->ClientIndex;
	sMsg.RequestTeamState = m_BRequestTeamState;
	sMsg.StartTeamNum = nStartTeamNum;	
	sMsg.ArenaMap = 0;		// 2012-04-13 by mspark, 아레나 UI 작업
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_REQUEST_TEAM,(char*)&sMsg,sizeof(sMsg));
	
	// 2007-11-22 by bhsohn 아레나 통합서버	
	// Lock
	{
		vector<MessageType_t> vecUnLockMsg;
		vecUnLockMsg.clear();
		vecUnLockMsg.push_back(T_FC_ARENA_REQUEST_TEAM);		
		vecUnLockMsg.push_back(T_FC_ARENA_REQUEST_TEAM_OK);		
		// 하나만 매칭 되어도 락이 풀린다. 
		g_pD3dApp->EnterMultiLock(TRUE, T_FC_ARENA_REQUEST_TEAM, vecUnLockMsg, MULTI_LOCK_ONE_MATCHING);
	}
	// end 2007-11-22 by bhsohn 아레나 통합서버
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 아레나 참가
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-03 ~ 2008-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////	
void CINFCityArena::SendArenaEnterRoom(char* TeamPW)
{
	// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
		return;
	}
	// end 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
	// 2008-03-10 by bhsohn 아레나 관련 버그 수정
	if(TRUE == g_pInterface->IsArenaGameState(ARENA_GAME_STATE_TEAMINFO))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0207,COLOR_ERROR);//"이미 아레나 대기방에 참여중입니다."
		return;	
	}
	// end 2008-03-10 by bhsohn 아레나 관련 버그 수정

	// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
	//SARENA_TEAM_INFO * TeamInfo = GetSelectListTeam();
	STRUCT_TEAM_INFO* TeamInfo = GetSelectListTeam();

	if(TeamInfo == NULL)
	{
		return ;
	}
	CHARACTER*	pArenaCharacterInfo = g_pD3dApp->GetArenaCharacterInfo();
	MSG_FC_ARENA_ENTER_TEAM sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_ARENA_ENTER_TEAM));

	sMsg.ArenaMode = TeamInfo->ArenaMode;
	sMsg.ClientIndex = pArenaCharacterInfo->ClientIndex;
	sMsg.TeamNum = TeamInfo->TeamNum;

	if(TeamPW)
	{
		util::strncpy(sMsg.TeamPW, TeamPW, SIZE_MAX_TEAM_PW);
	}
	
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_ENTER_TEAM,(char*)&sMsg,sizeof(sMsg));
}