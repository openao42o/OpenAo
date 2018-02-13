// CINFMissionInfo.cpp: implementation of the CINFMissionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFMissionInfo.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFIcon.h"
#include "QuestData.h"
#include "ShuttleChild.h"
#include "INFGameMain.h"
#include "INFScrollBar.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "StoreData.h"
#include "ItemInfo.h"
#include "CharacterChild.h"
#include "Interface.h"
#include "SceneData.h"
#include "MonsterData.h"
#include "Background.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "GlobalImage.h"
#include "Chat.h"
#include "ClientParty.h"
#include "AtumSound.h"
#include "INFImageEx.h"			// 2011. 10. 10 by jskim UI시스템 변경

#define MISSION_WINDOW_SIZE_WIDTH			590							// 미션창 최대 크기 
#define MISSION_WINDOW_SIZE_HEIGHT			396							// 미션창 최대 크기

#define	MISSION_COMPENSATIONITEM_MAX		4							// 최대 보상아이템수 4

#define	MISSION_SCROLL_LENGTH				310
#define	MISSION_SCROLL_LENGTH_FLUENCE		392
#define MISSION_SCROLL_LIST_NUMBER			17

#define MISSION_LEFT_TEXT_GAB_X				53
#define MISSION_RIGHT_TEXT_GAB_X			324
#define MISSION_RIGHT_TEXT_GAB_Y			60
#define MISSION_FONT_HEIGHT_GAB				17

#define MISSION_SELECT_MISSION_BAR_WIDTH	243
#define MISSION_SELECT_MISSION_BAR_HEIGHT	17

#define MISSION_GIVEUP_X					500
#define MISSION_GIVEUP_Y					55

#define MISSION_VIEW_ALL					0
#define MISSION_VIEW_SOLVE					1
#define MISSION_VIEW_UNSOLVE				2
#define MISSION_VIEW_INFO_POSX				(g_pD3dApp->GetBackBufferDesc().Width-390)

// 출격하기 미션맵 이동 좌표
#define MISSION_LAUNCH_SHUTTLE_X			(g_pD3dApp->GetBackBufferDesc().Width-127)
#define MISSION_LAUNCH_SHUTTLE_Y			(g_pD3dApp->GetBackBufferDesc().Height-72)
#define MISSION_MAP_MOVE_X					(g_pD3dApp->GetBackBufferDesc().Width-252)
#define MISSION_MAP_MOVE_Y					(g_pD3dApp->GetBackBufferDesc().Height-72)

// 색상 설정
#define MISSION_SELECT_COLOR				(RGB(194, 155, 0))
#define MISSION_PROGRESS_COLOR				(RGB(59, 177, 255))
#define MISSION_COMPLETE_COLOR				(RGB(215, 215, 45))
#define MISSION_IMPOSIBLE_COLOR				(RGB(184, 36, 36))
#define MISSION_POSIBLE_COLOR				(RGB(255, 255, 255))

// 에러 설정 정보 관련
#define MISSION_ERR_NONE					0
#define MISSION_ERR_LEVEL					100
#define MISSION_ERR_STAT_AT					101
#define MISSION_ERR_STAT_DE					102
#define MISSION_ERR_STAT_FU					103
#define MISSION_ERR_STAT_SO					104
#define MISSION_ERR_STAT_SH					105
#define MISSION_ERR_STAT_DO					106
#define MISSION_ERR_ITEM					107
#define MISSION_ERR_FLUENCE					108
#define MISSION_ERR_EXPERIENCE				109
#define MISSION_ERR_QUEST					110
#define MISSION_ERR_HIDEQUEST				111
#define MISSION_ERR_POSENDQUEST				112

// 경고창 
#define MISSION_WARNING_X					((g_pD3dApp->GetBackBufferDesc().Width / 2) - 225)
#define MISSION_WARNING_Y					((g_pD3dApp->GetBackBufferDesc().Height / 2) - 103)
#define MISSION_WARNING_OK_X				MISSION_WARNING_X + 139
#define MISSION_WARNING_OK_Y				MISSION_WARNING_Y + 118
#define MISSION_WARNING_CAN_X				MISSION_WARNING_X + 226
#define MISSION_WARNING_CAN_Y				MISSION_WARNING_Y + 118
#define MISSION_WARNING_INFO_X				MISSION_WARNING_X + 47
#define MISSION_WARNING_INFO_Y				MISSION_WARNING_Y + 73

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define STRING_CULL ::StringCullingUserData_ToBlank
#define MISSION_LEFT_TEXT_GAB_Y				80//82
#define MISSION_RADIO_Y						4
#define MISSION_MAX_DESC_STRING				220//35
#define MISSION_FLUNCE_MAX_DESC_STRING		312//55
#define MISSION_TOOLTIB_Y					30
#else
#define STRING_CULL ::StringCullingUserDataEx
#define MISSION_LEFT_TEXT_GAB_Y				80//82
#define MISSION_RADIO_Y						4//2
#define MISSION_MAX_DESC_STRING				220//35
#define MISSION_FLUNCE_MAX_DESC_STRING		312//55
#define MISSION_TOOLTIB_Y					30
#endif // _DEBUG_endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFMissionInfo::CINFMissionInfo()
{
	FLOG( "CINFMissionInfo()" );
	
	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		m_pMissionImg[nCount] = NULL;
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontLeft[nCount] = NULL;
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontRight[nCount] = NULL;
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount] = NULL;
		m_pMissionFontfluenceR[nCount] = NULL;
	}		
	
	m_pScrollLeftWindow  = NULL;
	m_pScrollRightWindow = NULL;
	m_pCompensation = NULL;
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		m_pMissionTarget[nCount] = NULL;

	m_PointMissionPos.x	= 0;
	m_PointMissionPos.y = 0;
	m_PointMissionPosOld.x = 0;
	m_PointMissionPosOld.y = 0;
	m_PointMissionMouse.x = 0;
	m_PointMissionMouse.y = 0;
	m_bMissionWindowStart = FALSE;
	m_nMissionGiveUp = 0;
	m_nSelectMissionTab = MISS_SCEN;
	m_nSelectMissionIndex = -1;
	m_nMissionViewCase = MISSION_VIEW_ALL;
	m_bProgressMission = FALSE;

	m_vecMissionList.clear();
	m_vecTabMissionNum.clear();

	m_vecCompensationItemName.clear();
	m_vecPossibleDesc.clear();
//	ZERO_MEMORY(m_strMissionName);
	m_bMoveMissionWindows = FALSE;
	m_nMissionButtonStateStart  = MISS_START_NOT;
	m_nMissionButtonStateEnd	= MISS_COPLETE_N;
	m_nMissionButtonMapMove		= MISS_MAP_GO_N;
	m_bButtonStateStart = FALSE;
	m_bButtonStateEnd = FALSE;

	m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_N;
	m_nMissionMapMove = MISS_MAP_MOVE_N;
	m_bLaunchShuttleState = FALSE;
	m_bMissionMapMove = FALSE;
	m_nMissionMapIndex = 0;
	m_bFlagEnterShop = FALSE;
	m_bMissionComButtonFlag = FALSE;
	m_bMissionStartButtonUse = FALSE;
	m_bButtonStateMapMove = FALSE;
	m_nVCUButton = MISS_INFLUENCE_NR;
	m_bVCUButton = FALSE;
	m_nANIButton = MISS_INFLUENCE_NR;
	m_bANIButton = FALSE;
	m_pScrollLeftfluence = NULL;
	m_pScrollRightfluence = NULL;
	m_vecVCUfluenceDesc.clear();
	m_vecANIfluenceDesc.clear();
	m_nOldMissionIndex = 0;
	m_vecOldString.clear();
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);
	m_vecErrPart.clear();
	m_vecErrQuestName.clear();
	m_vecErritem.clear();
	m_bSearchPosQuest = FALSE;
	m_bPosMissionComplete = FALSE;
	m_bViewMissionSelectFirst = FALSE;
	m_nMaxNum = 0;

	m_fVCNInflDistributionPercent = 0.0f;
	m_fANIInflDistributionPercent = 0.0f;

	m_nInflChoiceOver = INFLUENCE_TYPE_NORMAL;
	m_nInflChoiceOKButton = -1;
	m_nInflChoiceCANButton = -1;

	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i] = NULL;
		m_pImgCancelButton[i] = NULL;
	}
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i] = NULL;
	}
	m_pImgWarning = NULL;
	m_vecMissionName.clear();

	
}

CINFMissionInfo::~CINFMissionInfo()
{
	FLOG( "~CINFMissionInfo()" );
	
	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		util::del(m_pMissionImg[nCount]);	
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		util::del(m_pMissionFontLeft[nCount]);
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		util::del(m_pMissionFontRight[nCount]);
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		util::del(m_pMissionTarget[nCount]);
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		util::del(m_pMissionFontfluenceL[nCount]);
		util::del(m_pMissionFontfluenceR[nCount]);
	}

	util::del(m_pCompensation);
	util::del(m_pScrollLeftWindow);
	util::del(m_pScrollRightWindow);
	util::del(m_pScrollLeftfluence);
	util::del(m_pScrollRightfluence);

	int i;
	for(i = 0; i < 4; i++)
	{
		util::del(m_pImgOkButton[i]);
		util::del(m_pImgCancelButton[i]);
	}
	for(i = 0; i < 3; i++)
	{	
		util::del(m_pInfluenceWarning[i]);
	}
	util::del(m_pImgWarning);
	m_vecMissionName.clear();
}

HRESULT CINFMissionInfo::InitDeviceObjects()
{
	FLOG( "CINFMissionInfo::InitDeviceObjects(char* pData, int nSize)" );

	DataHeader	* pDataHeader ;
	char buf[IMAGE_MAX_COUNT][32];
	ZERO_MEMORY(buf);
	int nCount = 0;
	
	InitMission();

	wsprintf( buf[MISS_BACK],			"misbak");
	wsprintf( buf[MISS_NOM],			"misnor");
	wsprintf( buf[MISS_SCEN],			"missce");
	wsprintf( buf[MISS_SCEN_D],			"misscd");
	wsprintf( buf[MISS_CANCEL],			"misgive");
	wsprintf( buf[MISS_SEL_BACK],		"missel");
	wsprintf( buf[MISS_SEL_RADIO],		"misradn");
	wsprintf( buf[MISS_RADIO],			"misradn");
	wsprintf( buf[MISS_RADIO_S],		"radio_a");	
	
	wsprintf( buf[MISS_START_N],		"miss_c");	
	wsprintf( buf[MISS_START_O],		"miss_o");	
	wsprintf( buf[MISS_START_S],		"miss_s");	
	wsprintf( buf[MISS_START_NOT],		"missnot");	
	wsprintf( buf[MISS_COPLETE_N],		"mistarn");	
	wsprintf( buf[MISS_COPLETE_O],		"mistaro");	
	wsprintf( buf[MISS_COPLETE_S],		"mistars");	
	wsprintf( buf[MISS_COPLETE_NOT],	"mistanot");
	
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_N],	"goshn");
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_O],	"gosho");
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_S],	"goshs");
	wsprintf( buf[MISS_LAUNCH_SHUTTLE_NOT],	"goshnot");
	wsprintf( buf[MISS_MAP_MOVE_N],			"migon");
	wsprintf( buf[MISS_MAP_MOVE_O],			"migoo");
	wsprintf( buf[MISS_MAP_MOVE_S],			"migos");
	wsprintf( buf[MISS_MAP_MOVE_NOT],		"migonot");	
	wsprintf( buf[MISS_MAP_GO_N],			"mgdflt");
	wsprintf( buf[MISS_MAP_GO_O],			"mgov");
	wsprintf( buf[MISS_MAP_GO_S],			"mgpsh");
	wsprintf( buf[MISS_MAP_GO_NOT],			"mgdis");
	
	wsprintf( buf[MISS_INFLUENCE_BK],		"frc_bk");	// 세력 이미지
	wsprintf( buf[MISS_INFLUENCE_NR],		"frc_nr");
	wsprintf( buf[MISS_INFLUENCE_PS],		"frc_psh");
	wsprintf( buf[MISS_INFLUENCE_OV],		"frc_ov");


	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
	{
		m_pMissionImg[nCount] = new CINFImageEx;
		pDataHeader = FindResource(buf[nCount]);
		m_pMissionImg[nCount]->InitDeviceObjects(pDataHeader) ;
	}

	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++){
		m_pMissionFontLeft[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9,
			D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontLeft[nCount]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++){
		if(nCount == 0)
		{
			m_pMissionFontRight[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9,
				D3DFONT_ZENABLE|D3DFONT_BOLD, TRUE,1024,32);
			m_pMissionFontRight[nCount]->InitDeviceObjects(g_pD3dDev) ;
			
		}
		else
		{
			m_pMissionFontRight[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9,
				D3DFONT_ZENABLE, TRUE,1024,32);
			m_pMissionFontRight[nCount]->InitDeviceObjects(g_pD3dDev) ;
		}
	}

	m_pCompensation = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
	m_pCompensation->InitDeviceObjects(g_pD3dDev) ;

	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
	{
		m_pMissionTarget[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionTarget[nCount]->InitDeviceObjects(g_pD3dDev) ;
	}
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceL[nCount]->InitDeviceObjects(g_pD3dDev) ;

		m_pMissionFontfluenceR[nCount] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pMissionFontfluenceR[nCount]->InitDeviceObjects(g_pD3dDev) ;
	}

	// 스크롤 설정
	m_pScrollLeftWindow = new CINFScrollBar(this,
								m_PointMissionPos.x+295, 
								m_PointMissionPos.y+50, 
								MISSION_SCROLL_LENGTH,
								LEFTFONT_MAX_LINE_COUNT);
	m_pScrollLeftWindow->SetGameData( m_pGameData );
	m_pScrollLeftWindow->InitDeviceObjects();
	m_pScrollLeftWindow->SetWheelRect(
		m_PointMissionPos.x, 
		m_PointMissionPos.y,
		m_PointMissionPos.x+297,
		m_PointMissionPos.y+398);

	m_pScrollRightWindow = new CINFScrollBar(this,
								m_PointMissionPos.x+565, 
								m_PointMissionPos.y+50, 
								MISSION_SCROLL_LENGTH,
								RIGHTFONT_MAX_LINE_COUNT);
	m_pScrollRightWindow->SetGameData( m_pGameData );
	m_pScrollRightWindow->InitDeviceObjects();
	m_pScrollRightWindow->SetWheelRect(
		m_PointMissionPos.x+298,
		m_PointMissionPos.y,
		m_PointMissionPos.x+592,
		m_PointMissionPos.y+398);
	
	// 세력 선택스크롤 설정
	POINT pSize;
	pSize.x = 768; 
	pSize.y = 554;

	int nPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(pSize.x/2);
	int nPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(pSize.y/2);

	m_pScrollLeftfluence = new CINFScrollBar(this,
								nPosX+381, 
								nPosY+86, 
								MISSION_SCROLL_LENGTH_FLUENCE,
								MISSION_SCROLL_FLUENCE_MAX_LINE);
	m_pScrollLeftfluence->SetGameData( m_pGameData );
	m_pScrollLeftfluence->InitDeviceObjects();
	m_pScrollLeftfluence->SetScrollLinePos( nPosX+381, nPosY+86);
	m_pScrollLeftfluence->SetWheelRect(
		nPosX, 
		nPosY,
		nPosX+(pSize.x/2),
		nPosY+pSize.y);
	

	m_pScrollRightfluence = new CINFScrollBar(this,
								nPosX+747, 
								nPosY+86, 
								MISSION_SCROLL_LENGTH_FLUENCE,
								MISSION_SCROLL_FLUENCE_MAX_LINE);
	m_pScrollRightfluence->SetGameData( m_pGameData );
	m_pScrollRightfluence->InitDeviceObjects();
	m_pScrollRightfluence->SetScrollLinePos( nPosX+747, nPosY+86);
	m_pScrollRightfluence->SetWheelRect(
		nPosX+(pSize.x/2), 
		nPosY,
		nPosX+pSize.x,
		nPosY+pSize.y);	
	
	char sbuf[30];
	int i;
	for(i = 0; i < 4; i++)
	{

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	wsprintf(sbuf, "okb0%d", i);
#else
		wsprintf(sbuf, "shlaok0%d", i);
#endif
	
		m_pImgOkButton[i] = new CINFImageEx;
		pDataHeader = FindResource(sbuf);
		m_pImgOkButton[i]->InitDeviceObjects(pDataHeader);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(sbuf, "canb0%d", i);
#else
		wsprintf(sbuf, "shmcan0%d", i);
#endif


		m_pImgCancelButton[i] = new CINFImageEx;
		pDataHeader = FindResource(sbuf);
		m_pImgCancelButton[i]->InitDeviceObjects(pDataHeader);
	}

	wsprintf(sbuf, "inftex");
	m_pImgWarning = new CINFImageEx;
	pDataHeader = FindResource(sbuf);
	m_pImgWarning->InitDeviceObjects(pDataHeader);

	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pInfluenceWarning[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	return S_OK;
}

HRESULT CINFMissionInfo::RestoreDeviceObjects()
{
	FLOG( "CINFMissionInfo::RestoreDeviceObjects()" );

	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		m_pMissionImg[nCount]->RestoreDeviceObjects();
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontLeft[nCount]->RestoreDeviceObjects();
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontRight[nCount]->RestoreDeviceObjects();
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		m_pMissionTarget[nCount]->RestoreDeviceObjects();
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount]->RestoreDeviceObjects();
		m_pMissionFontfluenceR[nCount]->RestoreDeviceObjects();
	}

	m_pCompensation->RestoreDeviceObjects();	

	// 스크롤 설정
	m_pScrollLeftWindow->RestoreDeviceObjects();
	m_pScrollRightWindow->RestoreDeviceObjects();
	m_pScrollLeftfluence->RestoreDeviceObjects();
	m_pScrollRightfluence->RestoreDeviceObjects();

	// 스크롤 설정
	POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();
	m_nInfluencePosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(pSize.x/2);
	m_nInfluencePosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(pSize.y/2);	

	m_pScrollLeftfluence->SetScrollLinePos( m_nInfluencePosX+381, m_nInfluencePosY+86);
	m_pScrollLeftfluence->SetWheelRect(
		m_nInfluencePosX, 
		m_nInfluencePosY,
		m_nInfluencePosX+(pSize.x/2),
		m_nInfluencePosY+pSize.y);	
	
	m_pScrollRightfluence->SetScrollLinePos( m_nInfluencePosX+747, m_nInfluencePosY+86);
	m_pScrollRightfluence->SetWheelRect(
		m_nInfluencePosX+(pSize.x/2),
		m_nInfluencePosY,
		m_nInfluencePosX+pSize.x,
		m_nInfluencePosY+pSize.y);	
	
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->RestoreDeviceObjects();
		m_pImgCancelButton[i]->RestoreDeviceObjects();
	}

	m_pImgWarning->RestoreDeviceObjects();
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i]->RestoreDeviceObjects();
	}
	return S_OK;
}

HRESULT CINFMissionInfo::InvalidateDeviceObjects()
{
	FLOG( "CINFMissionInfo::InvalidateDeviceObjects()" );

	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++)
		m_pMissionImg[nCount]->InvalidateDeviceObjects();
	
	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontLeft[nCount]->InvalidateDeviceObjects();
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++)
		m_pMissionFontRight[nCount]->InvalidateDeviceObjects();
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++)
		m_pMissionTarget[nCount]->InvalidateDeviceObjects();
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++)
	{
		m_pMissionFontfluenceL[nCount]->InvalidateDeviceObjects();
		m_pMissionFontfluenceR[nCount]->InvalidateDeviceObjects();
	}

	m_pCompensation->InvalidateDeviceObjects();

	// 스크롤 설정
	m_pScrollLeftWindow->InvalidateDeviceObjects();
	m_pScrollRightWindow->InvalidateDeviceObjects();
	m_pScrollLeftfluence->InvalidateDeviceObjects();
	m_pScrollRightfluence->InvalidateDeviceObjects();	
	
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->InvalidateDeviceObjects();
		m_pImgCancelButton[i]->InvalidateDeviceObjects();
	}

	m_pImgWarning->InvalidateDeviceObjects();
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i]->InvalidateDeviceObjects();
	}
	return S_OK;
}

HRESULT CINFMissionInfo::DeleteDeviceObjects()
{
	FLOG( "CINFMissionInfo::DeleteDeviceObjects()" );
	
	int nCount = 0;
	// Image
	for(nCount=0; nCount<IMAGE_MAX_COUNT; nCount++){
		m_pMissionImg[nCount]->DeleteDeviceObjects();	
		util::del(m_pMissionImg[nCount]);
	}	

	// Font
	for(nCount=0;nCount<LEFTFONT_MAX_LINE_COUNT;nCount++){
		m_pMissionFontLeft[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontLeft[nCount]);
	}
	for(nCount=0;nCount<RIGHTFONT_MAX_LINE_COUNT;nCount++){
		m_pMissionFontRight[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontRight[nCount]);
	}
	for(nCount=0;nCount<MISSION_TARGET_LIST;nCount++){
		m_pMissionTarget[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionTarget[nCount]);
	}
	for(nCount=0;nCount<MISSION_SCROLL_FLUENCE_MAX_LINE;nCount++){
		m_pMissionFontfluenceL[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontfluenceL[nCount]);
		m_pMissionFontfluenceR[nCount]->DeleteDeviceObjects();
		util::del(m_pMissionFontfluenceR[nCount]);
	}
	m_pCompensation->DeleteDeviceObjects();
	util::del(m_pCompensation);

	// 스크롤 설정
	m_pScrollLeftWindow->DeleteDeviceObjects();
	util::del(m_pScrollLeftWindow);
	m_pScrollRightWindow->DeleteDeviceObjects();
	util::del(m_pScrollRightWindow);
	
	m_pScrollLeftfluence->DeleteDeviceObjects();
	util::del(m_pScrollLeftfluence);
	m_pScrollRightfluence->DeleteDeviceObjects();
	util::del(m_pScrollRightfluence);
	
	int i;
	for(i = 0; i < 4; i++)
	{
		m_pImgOkButton[i]->DeleteDeviceObjects();
		util::del(m_pImgOkButton[i]);
		m_pImgCancelButton[i]->DeleteDeviceObjects();
		util::del(m_pImgCancelButton[i]);
	}

	m_pImgWarning->DeleteDeviceObjects();
	util::del(m_pImgWarning);
	for(i = 0; i < 3; i++)
	{
		m_pInfluenceWarning[i]->DeleteDeviceObjects();
		util::del(m_pInfluenceWarning[i]);
	}
	return S_OK;
}

void CINFMissionInfo::Render()
{
	FLOG( "CINFMissionInfo::Render()" );

	if(TRUE == m_bInfluenceFlag)
	{	//////////////////////////////////////////////////////////////////////////
		// 세력미션 렌더링 중이면 이하 미션정보 렌더링 안함
		// 출격하기 미션맵이동 미션 정보등
		// 2005-08-16 by ydkim
		//////////////////////////////////////////////////////////////////////////
		RenderInfluenceMission();
		return;
	}

	if(m_bMissionWindowStart == TRUE)
	{
		// 처음 미션시작시 첫번미션 정보 보이기
		if(m_bViewMissionSelectFirst == TRUE)
		{
			if(m_nSelectMissionIndex == -1)
				m_nSelectMissionIndex = 0;
			
			// 할 수 있는 미션 설정(조건)
			StartPossibleMission();

			m_bViewMissionSelectFirst = FALSE;
		}

		// 배경 렌더링
		m_pMissionImg[MISS_BACK]->Move( m_PointMissionPos.x, m_PointMissionPos.y);
		m_pMissionImg[MISS_BACK]->Render();

		// 미션 탭렌더링 
		m_pMissionImg[m_nSelectMissionTab]->Move( m_PointMissionPos.x+15, m_PointMissionPos.y+24);
		m_pMissionImg[m_nSelectMissionTab]->Render();

		// 미션 라디오 버튼
		m_pMissionImg[MISS_SEL_RADIO]->Move( m_PointMissionPos.x+38,  m_PointMissionPos.y+347);
		m_pMissionImg[MISS_SEL_RADIO]->Render();
		m_pMissionImg[MISS_SEL_RADIO]->Move( m_PointMissionPos.x+98, m_PointMissionPos.y+347);
		m_pMissionImg[MISS_SEL_RADIO]->Render();
		m_pMissionImg[MISS_SEL_RADIO]->Move( m_PointMissionPos.x+188, m_PointMissionPos.y+347);
		m_pMissionImg[MISS_SEL_RADIO]->Render();
		
		switch(m_nMissionViewCase)
		{
		case MISSION_VIEW_ALL:
			m_pMissionImg[MISS_RADIO_S]->Move( m_PointMissionPos.x+36,  m_PointMissionPos.y+345);
			break;
		case MISSION_VIEW_SOLVE:
			m_pMissionImg[MISS_RADIO_S]->Move( m_PointMissionPos.x+96,  m_PointMissionPos.y+345);
			break;
		case MISSION_VIEW_UNSOLVE:
			m_pMissionImg[MISS_RADIO_S]->Move( m_PointMissionPos.x+186,  m_PointMissionPos.y+345);
			break;
		}
		m_pMissionImg[MISS_RADIO_S]->Render();

		// 미션 리스트 렌더링
		RenderMissionList();

		// 할수 있는 미션 렌더링
		// 조건 설정 현재 조건 없음
		RenderPossibleMission();

		// 스크롤 렌더링
		m_pScrollLeftWindow->Render();
		m_pScrollRightWindow->Render();
	}

	// 미션 목표 렌더링
//	RenderMissionTarget();
	
	// 출격하기 미션맵이동
	// 2006-03-24 by ispark, 파티미션일대 파티장만 버튼 보이기 추가
	// 2006-07-21 by ispark, 바잡맵에서는 미션창 열지 못한다.
	// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
	//if(g_pD3dApp->m_bCharacter == TRUE && g_pInterface->m_bShowInterface && m_bFlagEnterShop == FALSE && !g_pD3dApp->m_bBazaar)
	if(g_pD3dApp->m_bCharacter == TRUE && !g_pShuttleChild->IsObserverMode() && g_pInterface->m_bShowInterface && m_bFlagEnterShop == FALSE && !g_pD3dApp->m_bBazaar)
	{	
		// 미션 탭렌더링 
		m_pMissionImg[m_nLaunchShuttleState]->Move( MISSION_LAUNCH_SHUTTLE_X, MISSION_LAUNCH_SHUTTLE_Y);
		m_pMissionImg[m_nLaunchShuttleState]->Render();		
		
		// 미션맵 이동 (OutDoor)
		if(m_pQuestInfoProgOutDoor && m_pQuestInfoProgOutDoor->StartMapIndex>0 && 
			(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
		{
			m_pMissionImg[m_nMissionMapMove]->Move( MISSION_MAP_MOVE_X, MISSION_MAP_MOVE_Y);
			m_pMissionImg[m_nMissionMapMove]->Render();	
		}
		// 2006-03-24 by ispark, 파티미션일대 파티장만 버튼 보이기 추가
		if(m_pQuestInfoProgOutDoor && m_pQuestInfoProg && m_pQuestInfoProg->StartMapIndex>0 && m_bMissionWindowStart == TRUE && 
			(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
		{			
			// 미션 창에 미션맵이동 렌더링
			m_pMissionImg[m_nMissionButtonMapMove]->Move( m_PointMissionPos.x+430,	m_PointMissionPos.y+290);
			m_pMissionImg[m_nMissionButtonMapMove]->Render();
		}
	}
}

void CINFMissionInfo::RenderMissionList()
{
	FLOG( "CINFMissionInfo::RenderMissionList()" );

	// 선택 미션리스트
	if(m_nSelectMissionIndex >= m_pScrollLeftWindow->GetCurrentScrollIndex() && 
		m_nSelectMissionIndex < m_pScrollLeftWindow->GetCurrentScrollIndex()+LEFTFONT_MAX_LINE_COUNT)
	{
		m_pMissionImg[MISS_SEL_BACK]->Move( m_PointMissionPos.x+35, 
			m_PointMissionPos.y+80+((m_nSelectMissionIndex-m_pScrollLeftWindow->GetCurrentScrollIndex())*MISSION_SELECT_MISSION_BAR_HEIGHT));
		m_pMissionImg[MISS_SEL_BACK]->Render();
	}

	// 미션 이름 출력
	for(int i=0; i<LEFTFONT_MAX_LINE_COUNT; i++)
	{
		if(m_vecMissionList.size() <= i+m_pScrollLeftWindow->GetCurrentScrollIndex() || m_vecMissionList.size() == 0)
			break;
		
		m_pMissionImg[MISS_RADIO]->Move( m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X-12,
				m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB)+MISSION_RADIO_Y);
		m_pMissionImg[MISS_RADIO]->Render();

		if( m_nSelectMissionIndex == i+m_pScrollLeftWindow->GetCurrentScrollIndex() )
		{
			m_pMissionFontLeft[i]->DrawText(m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X,
				m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB),
				MISSION_SELECT_COLOR, (char*)m_vecMissionList[i+m_pScrollLeftWindow->GetCurrentScrollIndex()].c_str());
		}
		else
		{
			int nindex = m_vecTabMissionNum[i+m_pScrollLeftWindow->GetCurrentScrollIndex()];
			DWORD nColor = MISSION_IMPOSIBLE_COLOR;
			if(g_pQuestData->IsQuestCompleted(nindex))
			{
				nColor = MISSION_COMPLETE_COLOR;
			}
			else if(g_pQuestData->IsQuestProgress(nindex))
			{
				nColor = MISSION_PROGRESS_COLOR;
			}
			else
			{
				 CQuest* pQuest = g_pQuestData->FindQuest(nindex);
				 if(pQuest)
				 {
					 if( CheckProgressMission(pQuest) == TRUE)
						 nColor = MISSION_POSIBLE_COLOR;
				 }
			}

			m_pMissionFontLeft[i]->DrawText(m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X,
				m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB),
				nColor, (char*)m_vecMissionList[i+m_pScrollLeftWindow->GetCurrentScrollIndex()].c_str());
		}
	}
//	m_pMissionFontLeft[LEFTFONT_MAX_LINE_COUNT]
//	m_vecMissionList
}

void CINFMissionInfo::Tick()
{
	FLOG( "CINFMissionInfo::Tick()" );
	
	if(m_bSearchPosQuest == TRUE && m_pHidePosQuest)
	{	// 특정 포지션에 퀘스트가 있는경우 검색 후 실행한다.
		CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
		if(pQuestInfo == NULL)
		{
			int X = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.x);
			int Z = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z);

			if(CheckProgressMission(m_pHidePosQuest))
			{
				char buff[512];
				ZERO_MEMORY(buff);
				wsprintf(buff, STRMSG_C_050825_0001, m_pHidePosQuest->QuestName); //"미션 %s를(을) 시작하시겠습니까?"
				g_pGameMain->m_pInfWindow->AddMsgBox(buff,_Q_MISSION_START, (DWORD)m_pHidePosQuest->QuestIndex);
				m_bSearchPosQuest = FALSE;
			}
		}
	}
	// 특정지역 미션 완료
	if(m_bPosMissionComplete == TRUE)
	{
		int nMissionIndex = g_pD3dApp->SerchMissionCondition();
		if(nMissionIndex)
		{
			MissionEnd(nMissionIndex);
			m_bPosMissionComplete = FALSE;
		}
	}

	// 미션 완료 시간 표시
	// 퀘스트 남은시간 표시
	if(g_pGameMain->m_bQuestLimitTimeView)
	{	
		float fElapsedTime = g_pD3dApp->GetElapsedTime();
		// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
		float fGetQuestTimeElapse = g_pGameMain->GetQuestTimeElapse();
		fGetQuestTimeElapse -= fElapsedTime;				
		g_pGameMain->SetQuestTimeElapse(fGetQuestTimeElapse);
		if(fGetQuestTimeElapse < -5)
		{
			g_pGameMain->m_bQuestLimitTimeView = FALSE;
			// 끝 났당 미션취소 보내기
			CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
			if(pQuestInfo)
			{
				// 2005-08-02 ispark
				if(QUEST_END_TYPE_TIME_OVER == pQuestInfo->QuestEndType)
				{
					// 미션 완료 보내기
					// 2006-03-27 by ispark, 파티미션 추가
					if(pQuestInfo->IsPartyQuest())
					{
						g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(pQuestInfo->QuestIndex);
					}
					else
					{
						g_pGImage->SetImageInfo(MISSION_SUC, TRUE);
						//g_pD3dApp->SendMissionComplete();
					}
				}
				else
				{
					// 미션 취소 보내기
					g_pGImage->SetImageInfo(MISSION_FAI, TRUE);
				}
			}
		}
//		g_pGameMain->m_fTimeElapse -= fElapsedTime;				
//		if(g_pGameMain->m_fTimeElapse < 0)
//		{
//			g_pGameMain->m_fTimeElapse = 1;
//			g_pGameMain->m_nTimeSecond--;
//			
//			if(g_pGameMain->m_nTimeSecond<-5)
//			{
//				g_pGameMain->m_bQuestLimitTimeView = FALSE;
//				// 끝 났당 미션취소 보내기
//				CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
//				if(pQuestInfo)
//				{
//					// 2005-08-02 ispark
//					if(QUEST_END_TYPE_TIME_OVER == pQuestInfo->QuestEndType)
//					{
//						// 미션 완료 보내기
//						// 2006-03-27 by ispark, 파티미션 추가
//						if(pQuestInfo->IsPartyQuest())
//						{
//							g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(pQuestInfo->QuestIndex);
//						}
//						else
//						{
//							g_pGImage->SetImageInfo(MISSION_SUC, TRUE);
//							//g_pD3dApp->SendMissionComplete();
//						}
//					}
//					else
//					{
//						// 미션 취소 보내기
//						g_pGImage->SetImageInfo(MISSION_FAI, TRUE);
//					}
//				}
//			}
//		}				
	}
}

int CINFMissionInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFMissionInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	// 세력선택 미션 처리
	if(TRUE == m_bInfluenceFlag){
		return WndProcInfluence(uMsg, wParam, lParam);
	}

	// 스크롤 처리
	if(m_pScrollLeftWindow && m_pScrollRightWindow && m_bMissionWindowStart == TRUE){
		if(m_pScrollLeftWindow->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
		else if(m_pScrollRightWindow->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(m_bMissionWindowStart == TRUE)
			{
				if(m_bMoveMissionWindows == TRUE)
				{
					m_PointMissionPos.x = m_PointMissionPosOld.x + pt.x - m_PointMissionMouse.x;
					m_PointMissionPos.y = m_PointMissionPosOld.y + pt.y - m_PointMissionMouse.y;
					
					// 스크롤 설정
					m_pScrollLeftWindow->SetScrollLinePos( m_PointMissionPos.x+295, m_PointMissionPos.y+50);
					m_pScrollLeftWindow->SetWheelRect(
						m_PointMissionPos.x, 
						m_PointMissionPos.y,
						m_PointMissionPos.x+297,
						m_PointMissionPos.y+398);
					
					m_pScrollRightWindow->SetScrollLinePos( m_PointMissionPos.x+565, m_PointMissionPos.y+50);
					m_pScrollRightWindow->SetWheelRect(
						m_PointMissionPos.x+298,
						m_PointMissionPos.y,
						m_PointMissionPos.x+592,
						m_PointMissionPos.y+398);
					
					return INF_MSGPROC_BREAK;				
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+312 && pt.x<m_PointMissionPos.x+376 &&
					m_bMissionComButtonFlag)
				{
					// 미션완료
					if(m_vecTabMissionNum.size() <= 0)
						break;
					if(m_bButtonStateStart == TRUE)
						m_nMissionButtonStateStart = MISS_START_S;
					else
						m_nMissionButtonStateStart = MISS_START_O;
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+485 && pt.x<m_PointMissionPos.x+549 &&
					m_bMissionStartButtonUse == FALSE)
				{	// 미션시작
					if(m_vecTabMissionNum.size() <= 0)
						break;
					if(m_bButtonStateEnd == TRUE)
						m_nMissionButtonStateEnd = MISS_COPLETE_S;
					else
						m_nMissionButtonStateEnd = MISS_COPLETE_O;			
				}
				else if(pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+430 && pt.x<m_PointMissionPos.x+550 &&
					m_pQuestInfoProg)
				{	// 미션맵 이동 버튼(미션 창내의)
					if(m_bButtonStateMapMove == TRUE)
						m_nMissionButtonMapMove = MISS_MAP_GO_S;
					else
						m_nMissionButtonMapMove = MISS_MAP_GO_O;
				}
				else
				{				
					if(m_vecTabMissionNum.size() <= 0)
					{
						m_nMissionButtonStateStart = MISS_START_NOT;
						m_nMissionButtonStateEnd = MISS_COPLETE_NOT;	
					}
					else
					{
						m_nMissionButtonStateStart = MISS_START_N;
						m_nMissionButtonStateEnd = MISS_COPLETE_N;	
					}
					m_nMissionButtonMapMove = MISS_MAP_GO_N;					
				}
				
				// 보상아이템 툴팁설정.
				g_pGameMain->SetItemInfo( 0, 0, 0, 0);	// 2005-11-30 by ispark
				ZERO_MEMORY(m_bCompenToolTib);
				if(pt.y>m_PointMissionPos.y+330 && pt.y<m_PointMissionPos.y+357)
				{
					for(int i=0;i<COMPENS_MAX;i++)
					{
						if(pt.x>m_PointMissionPos.x+310+(i*52) && pt.x<m_PointMissionPos.x+337+(i*52))
						{
							m_bCompenToolTib[i] = TRUE;
						}
					}
				}
				// 2006-08-25 by dgwoo 미션 창안에서 인벤의 아이템정보는 무효로 한다.
				if (pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+MISSION_WINDOW_SIZE_HEIGHT &&
					pt.x>m_PointMissionPos.x && pt.x<m_PointMissionPos.x+MISSION_WINDOW_SIZE_WIDTH)
				{
					return INF_MSGPROC_BREAK;
				}
			}
			
			// 출격 및 미션맵이동
			if(pt.y>MISSION_LAUNCH_SHUTTLE_Y && pt.y<MISSION_LAUNCH_SHUTTLE_Y+30 &&
				pt.x>MISSION_LAUNCH_SHUTTLE_X && pt.x<MISSION_LAUNCH_SHUTTLE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE)
			{
				if(m_bLaunchShuttleState == TRUE)
					m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_S;
				else
					m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_O;
			}
			else
			{
				m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_N;
			}
			
			if(m_pQuestInfoProgOutDoor &&
				pt.y>MISSION_MAP_MOVE_Y && pt.y<MISSION_MAP_MOVE_Y+30 &&
				pt.x>MISSION_MAP_MOVE_X && pt.x<MISSION_MAP_MOVE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE)
			{
				if(m_bMissionMapMove == TRUE)
					m_nMissionMapMove = MISS_MAP_MOVE_S;
				else
					m_nMissionMapMove = MISS_MAP_MOVE_O;				
			}
			else
			{
				m_nMissionMapMove = MISS_MAP_MOVE_N;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_bMissionWindowStart == TRUE)
			{
				m_bButtonStateEnd = FALSE;
				m_bButtonStateStart = FALSE;

				if(pt.x>m_PointMissionPos.x	&& pt.x<m_PointMissionPos.x+565
					&& pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+20)
				{
					m_PointMissionMouse = pt;
					m_PointMissionPosOld = m_PointMissionPos;
					m_bMoveMissionWindows = TRUE;				
					g_pInterface->SetWindowOrder(WNDMissionInfo);
				}
				
				if( pt.x>m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X
					&& pt.x<m_PointMissionPos.x+MISSION_LEFT_TEXT_GAB_X+MISSION_SELECT_MISSION_BAR_WIDTH)
				{
					// 미션 선택
					for(int i=0; i<LEFTFONT_MAX_LINE_COUNT; i++)
					{
						if(m_vecMissionList.size() <= i+m_pScrollLeftWindow->GetCurrentScrollIndex())
							break;
						
						if( pt.y>m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+(i*MISSION_FONT_HEIGHT_GAB)
							&& pt.y<m_PointMissionPos.y+MISSION_LEFT_TEXT_GAB_Y+MISSION_SELECT_MISSION_BAR_HEIGHT+(i*MISSION_FONT_HEIGHT_GAB))
						{
							m_nSelectMissionIndex = i+m_pScrollLeftWindow->GetCurrentScrollIndex();
							
							// 할 수 있는 미션 설정(조건)
							g_pInterface->SetWindowOrder(WNDMissionInfo);
							StartPossibleMission();
							return INF_MSGPROC_BREAK;
						}
					}
				}			
				else if(pt.x>m_PointMissionPos.x+MISSION_GIVEUP_X && pt.x<m_PointMissionPos.x+MISSION_GIVEUP_X+54 &&
					pt.y>m_PointMissionPos.y+MISSION_GIVEUP_Y && pt.y<m_PointMissionPos.y+MISSION_GIVEUP_Y+16)
				{
					// 미션 포기
					if(m_nMissionGiveUp != 0 && m_bProgressMission == TRUE)
					{
						g_pInterface->SetWindowOrder(WNDMissionInfo);
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_QUEST_0015,_Q_QUEST_DISCARD,
							m_nMissionGiveUp);//"해당 미션을 포기하시겠습니까?."
						m_nMissionGiveUp = 0;
						m_pQuestInfoProg = NULL;
						m_pQuestInfoProgOutDoor = NULL;
					}
					// 미션 미 선택일때
					else if(m_nSelectMissionIndex == -1)
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060720_0002, _MESSAGE);
					}
					// 미션 포기 안됨
					else
					{
						g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_060720_0001, _MESSAGE);
					}

					return INF_MSGPROC_BREAK;
				}
				
				if( pt.y>m_PointMissionPos.y+342 && pt.y<m_PointMissionPos.y+362 )
				{
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					// 전체보기, 해결미션 보기, 미해결 미션보기
					if(pt.x>m_PointMissionPos.x+40 && pt.x<m_PointMissionPos.x+100){
						InitMission();
						m_nMissionViewCase = MISSION_VIEW_ALL;
						InitMissionTitle();
						m_bMissionStartButtonUse = FALSE;
						return INF_MSGPROC_BREAK;
					}
					else if (pt.x>m_PointMissionPos.x+101 && pt.x<m_PointMissionPos.x+190){
						InitMission();
						m_nMissionViewCase = MISSION_VIEW_SOLVE;
						InitMissionTitle();
						m_bMissionStartButtonUse = TRUE;
						return INF_MSGPROC_BREAK;
					}
					else if(pt.x>m_PointMissionPos.x+191 && pt.x<m_PointMissionPos.x+285){
						InitMission();
						m_nMissionViewCase = MISSION_VIEW_UNSOLVE;
						InitMissionTitle();
						m_bMissionStartButtonUse = TRUE;
						return INF_MSGPROC_BREAK;
					}
					
				}
				else if(pt.y>m_PointMissionPos.y+24 && pt.y<m_PointMissionPos.y+38 )
				{
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					// 탭설정
					if(pt.x>m_PointMissionPos.x+15 && pt.x<m_PointMissionPos.x+120){
						m_nSelectMissionTab = MISS_SCEN;
						InitMission();
						InitMissionTitle();
						return INF_MSGPROC_BREAK;
					}
					else if(pt.x>m_PointMissionPos.x+121 && pt.x<m_PointMissionPos.x+192){
						m_nSelectMissionTab = MISS_NOM;
						InitMission();
						InitMissionTitle();
						return INF_MSGPROC_BREAK;
					}
					else if(pt.x>m_PointMissionPos.x+193 && pt.x<m_PointMissionPos.x+300){
						m_nSelectMissionTab = MISS_SCEN_D;
						InitMission();
						InitMissionTitle();
						return INF_MSGPROC_BREAK;
					}
					
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+312 && pt.x<m_PointMissionPos.x+376 &&
					m_bMissionComButtonFlag)
				{
					// 미션완료
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					if(m_vecTabMissionNum.size() <= 0)
					{
						m_nMissionButtonStateStart = MISS_START_NOT;
						break;
					}					
					m_nMissionButtonStateStart = MISS_START_S;
					m_bButtonStateStart = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+485 && pt.x<m_PointMissionPos.x+549 &&
					m_bMissionStartButtonUse == FALSE)
				{	// 미션시작
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					if(m_vecTabMissionNum.size() <= 0)
					{
						m_nMissionButtonStateEnd = MISS_COPLETE_NOT;
						break;
					}
					m_nMissionButtonStateEnd = MISS_COPLETE_S;
					m_bButtonStateEnd = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if(pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+430 && pt.x<m_PointMissionPos.x+550 &&
					m_pQuestInfoProg)
				{	// 미션맵 이동 버튼(미션 창내의)
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					m_nMissionButtonMapMove = MISS_MAP_GO_S;
					m_bButtonStateMapMove = TRUE;
					return INF_MSGPROC_BREAK;
				}
				else if (pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+20 &&
					pt.x>m_PointMissionPos.x+570 && pt.x<m_PointMissionPos.x+590)
				{	// 창닫기
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					MissionProcFlag(FALSE);					
					return INF_MSGPROC_BREAK;
				}
				
				// 2005-08-02 by ispark
				// 미션 창안에서 클릭은 무효
				if (pt.y>m_PointMissionPos.y && pt.y<m_PointMissionPos.y+MISSION_WINDOW_SIZE_HEIGHT &&
					pt.x>m_PointMissionPos.x && pt.x<m_PointMissionPos.x+MISSION_WINDOW_SIZE_WIDTH)
				{
					g_pInterface->SetWindowOrder(WNDMissionInfo);
					return INF_MSGPROC_BREAK;
				}
			}
			
			// 2005-12-14 by ispark, 인터페이스가 숨겨져 있다면 밑에 클릭은 무효
			if(!g_pInterface->m_bShowInterface || m_bFlagEnterShop == TRUE)
				break;

			// 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			if(g_pShuttleChild->IsObserverMode())
			{
				break;
			}
			// end 2007-03-27 by bhsohn 옵저버 모드시 인터페이스 숨김
			
			// 출격 및 미션맵이동
			m_bLaunchShuttleState = FALSE;
			m_bMissionMapMove = FALSE;
			if (pt.y>MISSION_LAUNCH_SHUTTLE_Y && pt.y<MISSION_LAUNCH_SHUTTLE_Y+30 &&
				pt.x>MISSION_LAUNCH_SHUTTLE_X && pt.x<MISSION_LAUNCH_SHUTTLE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE &&
				!g_pGround->m_bBazaar)
			{
				g_pInterface->SetWindowOrder(WNDMissionInfo);
				m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_S;
				m_bLaunchShuttleState = TRUE;
				return INF_MSGPROC_BREAK;
			}
			// 2006-03-24 by ispark, 파티미션일대 파티장만 버튼 보이기 추가
			if(m_pQuestInfoProgOutDoor &&
				pt.y>MISSION_MAP_MOVE_Y && pt.y<MISSION_MAP_MOVE_Y+30 &&
				pt.x>MISSION_MAP_MOVE_X && pt.x<MISSION_MAP_MOVE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE &&
				(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
			{
				g_pInterface->SetWindowOrder(WNDMissionInfo);
				m_bMissionMapMove = TRUE;
				m_nMissionMapMove = MISS_MAP_MOVE_S;
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MBUTTONDOWN:
		{
			// 테스트 코드
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if(m_bMissionWindowStart == TRUE)
			{
				// 2007-07-09 by bhsohn 미션 창 띠웠을 시, 메모리 버그 나는 현상처리
				//CQuest* pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
				CQuest* pQuest = NULL;
				if(m_nSelectMissionIndex >= 0)
				{
					pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
				}				

				if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+312 && pt.x<m_PointMissionPos.x+376 &&
					m_bButtonStateStart == TRUE &&  m_bMissionComButtonFlag)
				{
					if(m_vecTabMissionNum.size() <= 0 || m_vecTabMissionNum.size()<m_nSelectMissionIndex)
						break;
					if(m_nMissionViewCase == MISSION_VIEW_SOLVE || m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
						break;
					// 미션완료
					MissionEnd(m_vecTabMissionNum[m_nSelectMissionIndex]);
					m_nMissionButtonStateStart = MISS_START_O;
				}
				else if (pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+485 && pt.x<m_PointMissionPos.x+549 &&
					m_bButtonStateEnd == TRUE &&  m_bMissionComButtonFlag == FALSE &&
					m_bMissionStartButtonUse == FALSE &&
					pQuest &&(!pQuest->IsPartyQuest() || (pQuest->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
				{	// 미션시작
					if(m_vecTabMissionNum.size() <= 0 || m_vecTabMissionNum.size()<m_nSelectMissionIndex)
						break;
					// 2006-03-28 by ispark, 미 해결탭에서 사용가능
//					if(m_nMissionViewCase == MISSION_VIEW_SOLVE || m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
					if(m_nMissionViewCase == MISSION_VIEW_SOLVE)
						break;
					MissionStart(m_vecTabMissionNum[m_nSelectMissionIndex]);
					m_nMissionButtonStateEnd = MISS_COPLETE_O;
				}
				else if(m_pQuestInfoProgOutDoor && m_pQuestInfoProgOutDoor->StartMapIndex>0 &&
					pt.y>m_PointMissionPos.y+290 && pt.y<m_PointMissionPos.y+307 &&
					pt.x>m_PointMissionPos.x+430 && pt.x<m_PointMissionPos.x+550 &&
					m_pQuestInfoProg && g_pD3dApp->m_bCharacter == TRUE &&
					pQuest && (!pQuest->IsPartyQuest() || (pQuest->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
				{	// 미션맵 이동 버튼(미션 창내의)
					// 2005-11-16 by ispark
					// 미션맵 이동 조건 검색
					if(!CheckMissionStart())
					{
						break;
					}
					m_nMissionButtonMapMove = MISS_MAP_GO_O;
					m_bMissionWindowStart = FALSE;
					g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
					g_pCharacterChild->FineObjectTakeOff();
					m_nMissionMapIndex = m_pQuestInfoProg->StartMapIndex;
				}
			}
			
			// 출격 및 미션맵이동
			if (pt.y>MISSION_LAUNCH_SHUTTLE_Y && pt.y<MISSION_LAUNCH_SHUTTLE_Y+30 &&
				pt.x>MISSION_LAUNCH_SHUTTLE_X && pt.x<MISSION_LAUNCH_SHUTTLE_X+124 &&
				m_bLaunchShuttleState == TRUE && m_bFlagEnterShop == FALSE &&
				g_pD3dApp->m_bCharacter == TRUE)
			{
				// 2005-11-16 by ispark
				// 출격 이동 조건 검색
				if(!CheckMissionStart())
				{
					break;
				}

				MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(3002);	
				MEX_QUEST_INFO* pTemp = g_pQuestData->FindCharacterQuest(pMapInfo->QuestIndexForWarp);
				if(pTemp && pTemp->QuestState == QUEST_STATE_COMPLETED)
				{
					g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
					m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_O;
					g_pCharacterChild->FineObjectTakeOff();
					m_nMissionMapIndex = 0;
				}
				else
				{
					CQuest* pQuest = g_pQuestData->FindQuest(pMapInfo->QuestIndexForWarp);
					if(pQuest)
					{
						char buf[512];
						ZERO_MEMORY(buf);
						wsprintf(buf, STRMSG_C_050818_0010, pQuest->QuestName); //"'%s' 미션 완료 후 출격가능!"
						g_pGameMain->m_pInfWindow->AddMsgBox( buf, _MESSAGE);
					}
					else
					{
						g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
						m_nLaunchShuttleState = MISS_LAUNCH_SHUTTLE_O;
						g_pCharacterChild->FineObjectTakeOff();
						m_nMissionMapIndex = 0;						
					}
				}
			}
			// 2006-03-24 by ispark, 파티미션일대 파티장만 버튼 보이기 추가
			if(m_pQuestInfoProgOutDoor && m_pQuestInfoProgOutDoor->StartMapIndex>0 &&
				pt.y>MISSION_MAP_MOVE_Y && pt.y<MISSION_MAP_MOVE_Y+30 &&
				pt.x>MISSION_MAP_MOVE_X && pt.x<MISSION_MAP_MOVE_X+124 &&
				m_bFlagEnterShop == FALSE && g_pD3dApp->m_bCharacter == TRUE &&
				(!m_pQuestInfoProgOutDoor->IsPartyQuest() || (m_pQuestInfoProgOutDoor->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)))
			{
				// 2005-11-16 by ispark
				// 미션맵 이동 조건 검색
				if(!CheckMissionStart())
				{
					break;
				}
				
				g_pD3dApp->StartFadeEffect(TRUE,3,D3DCOLOR_ARGB(0,0,0,0));
				m_nMissionMapMove = MISS_MAP_MOVE_O;
				g_pCharacterChild->FineObjectTakeOff();
				m_nMissionMapIndex = m_pQuestInfoProgOutDoor->StartMapIndex;
			}
			
			m_bMoveMissionWindows = FALSE;
			m_bButtonStateEnd = FALSE;
			m_bButtonStateStart = FALSE;
			m_bLaunchShuttleState = FALSE;
			m_bMissionMapMove = FALSE;
			m_bButtonStateMapMove = FALSE;
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFMissionInfo::StartPossibleMission()
{
	FLOG( "CINFMissionInfo::StartPossibleMission()" );

	CQuest* pQuest = NULL;
	CQuest* pQuestErr = NULL;
	m_bMissionStartButtonUse = TRUE;
	m_vecPossibleDesc.clear();
	m_vecMissionName.clear();
	m_vecCompensationItemName.clear();
//	ZERO_MEMORY(m_strMissionName);
	m_pQuestInfoProgOutDoor = NULL;
	m_pQuestInfoProg = NULL;
	m_nMissionButtonStateEnd = MISS_COPLETE_N;
	// 보상아이템 관련 추가 사항
	ZERO_MEMORY(m_nCompensation);	

	if(m_vecTabMissionNum.size()<=m_nSelectMissionIndex)
		return;
	pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
	if(pQuest == NULL)
		return;
	
	pQuestErr = g_pQuestData->GetProgressMission();
	if(CheckProgressMission(pQuest) == FALSE)
	{
		if(pQuestErr && pQuestErr->QuestIndex == pQuest->QuestIndex)
		{
		}
		else
		{
			// 에러 설명 부분 (진행할수 없는 미션)
			vector<int>::iterator itErr = m_vecErrPart.begin();
			while(itErr != m_vecErrPart.end())
			{		
				switch(*itErr)
				{
				case MISSION_ERR_NONE:
					break;
				case MISSION_ERR_LEVEL:
					{					
						// 2007-01-22 by bhsohn 미션 인터 페이스 관련 수정안
						//m_vecPossibleDesc.push_back(STRMSG_C_050825_0002); //"레벨이 부족합니다."
						char buf[512];
						ZERO_MEMORY(buf);						
						wsprintf(buf, STRMSG_C_070122_0201, pQuest->ReqLevel.Min);		//"%d레벨 부터 가능한 미션 입니다."
						m_vecPossibleDesc.push_back(buf); 
					}
					break;
				case MISSION_ERR_STAT_AT:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0003); //"스탯중 공격파트가 부족합니다."
					break;
				case MISSION_ERR_STAT_DE:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0004); //"스탯중 방어파트가 부족합니다."
					break;
				case MISSION_ERR_STAT_FU:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0005); //"스탯중 연료파트가 부족합니다."
					break;
				case MISSION_ERR_STAT_SO:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0006); //"스탯중 정신파트가 부족합니다."
					break;
				case MISSION_ERR_STAT_SH:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0007); //"스탯중 쉴드파트가 부족합니다."
					break;
				case MISSION_ERR_STAT_DO:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0008); // "스탯중 회피파트가 부족합니다."
					break;
				case MISSION_ERR_ITEM:
					{
						char buf[512];
						ZERO_MEMORY(buf);
						strcpy(buf, STRMSG_C_050825_0009); //"아이템 \\r"
						vector<string>::iterator itItem = m_vecErritem.begin();
						while(itItem != m_vecErritem.end())
						{
							strcat(buf, (char*)itItem->c_str());
							strcat(buf, " ");
							itItem++;						
						}
						strcat(buf, STRMSG_C_050825_0010); //"\\r가(이) 필요합니다."
						STRING_CULL(buf, MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
					}
					break;
				case MISSION_ERR_FLUENCE:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0011); //"세력이 맞지않습니다."
					break;
				case MISSION_ERR_EXPERIENCE:
					m_vecPossibleDesc.push_back(STRMSG_C_050825_0012); //"경험치가 부족합니다"
					break;
				case MISSION_ERR_QUEST:
					{
						char buf[512];
						ZERO_MEMORY(buf);
						strcpy(buf, STRMSG_C_050825_0013); //"미션 \\r"
						vector<string>::iterator itItem = m_vecErrQuestName.begin();
						while(itItem != m_vecErrQuestName.end())
						{
							strcat(buf, (char*)itItem->c_str());
							strcat(buf, " ");
							itItem++;						
						}
						strcat(buf, STRMSG_C_050825_0014); //"\\r을(를) 완료해야 진행 할 수 있습니다."
						STRING_CULL(buf, MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
					}
					break;
				case MISSION_ERR_HIDEQUEST:
					{
						m_vecPossibleDesc.push_back(STRMSG_C_050825_0015); //"숨겨진 미션"
						m_vecPossibleDesc.push_back(STRMSG_C_050825_0016); //"\\r특정 조건\\r을 완료 하세요"
					}
					break;
				}
				itErr++;
			}
			return;
		}
	}

	char buf[64];
	ZERO_MEMORY(buf);
	m_bProgressMission = FALSE;	
	
	// 이름 설정
	// 2006-03-27 by ispark, 문자열 처리 수정	
	char strMissionNameTemp[50] = {0,};
	int nMaxLevel = pQuest->ReqLevel.Max;
	if(pQuest->ReqLevel.Max>100)
		nMaxLevel = 100;
	wsprintf(buf, " [Lv.%d]", pQuest->ReqLevel.Min);
	int nLevelLen = strlen(buf);
//	strncpy( m_strMissionName, pQuest->QuestName, SIZE_MAX_QUEST_NAME-nLevelLen);
//	strcat( m_strMissionName, buf);
	strncpy( strMissionNameTemp, pQuest->QuestName, 50-nLevelLen);
	strcat( strMissionNameTemp, buf);
//	STRING_CULL(strMissionNameTemp, SIZE_MAX_QUEST_NAME, &m_vecMissionName, m_pMissionFontRight[0]);
	STRING_CULL(strMissionNameTemp, MISSION_MAX_DESC_STRING, &m_vecMissionName, m_pMissionFontRight[0]);

	// 보상아이템 이름설정	
	OverlapSortPayItem(pQuest);
//	m_vecCompensationItemName = pQuest->EndPayItemVector;
	if(pQuest->EndPayPropensity != 0)
	{ // 명성
		m_nCompensation[COMPENS_PRO] = pQuest->EndPayPropensity;
	}
	if(pQuest->EndPayLevel != 0)
	{ // 레벨
		m_nCompensation[COMPENS_LEV] = pQuest->EndPayLevel;
	}
	if(pQuest->EndPayExperience != 0)
	{ // 경험치
		m_nCompensation[COMPENS_EXP] = pQuest->EndPayExperience;
	}
	if(pQuest->EndPayBonusStat != 0)
	{ // 보너스 스탯
		m_nCompensation[COMPENS_BUN] = pQuest->EndPayBonusStat;
	}
	
	// 미션 결과
	MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuest->QuestIndex);
	if(pCharacterQuest)
	{
		if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
			m_bProgressMission = TRUE;
			STRING_CULL(pQuest->QuestNPCInfo.MidTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			m_vecPossibleDesc.push_back("          ");
			m_pQuestInfoProg = g_pQuestData->GetProgressMission();
		}
		else if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
			STRING_CULL(pQuest->QuestNPCInfo.SuccessTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);			
			m_vecPossibleDesc.push_back("          ");
		}
//		현재 실패 상황은 없음
//		else if()
//		{
//			pQuest->TargetMeetNPCInfo.FailTalk;
//		}
		else
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
			STRING_CULL(pQuest->QuestNPCInfo.PreTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			m_vecPossibleDesc.push_back("          ");
//			m_bMissionStartButtonUse = FALSE;
		}
	}
	else
	{		
		if(strlen(pQuest->QuestNPCInfo.PreTalk) > 1)
		{
			m_vecPossibleDesc.push_back(STRMSG_C_050726_0001); //"\\y진행상황\\y"
		STRING_CULL(pQuest->QuestNPCInfo.PreTalk, 
				MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);
			m_vecPossibleDesc.push_back("          ");
		}
		m_bMissionStartButtonUse = FALSE;
	}
	
	// 
	if(pCharacterQuest && pCharacterQuest->QuestState == QUEST_STATE_COMPLETED)
	{
		// 미션 설명
		m_vecPossibleDesc.push_back(STRMSG_C_051209_0001); //"\\y배경설명\\y"
		STRING_CULL(pQuest->QuestDescription, 
			MISSION_MAX_DESC_STRING, &m_vecPossibleDesc, m_pMissionFontRight[1]);

		// 버튼으로 인한 공백 추가
		m_vecPossibleDesc.push_back("          ");
	}

	// 미션 NUMBER (시나리오 미션 미션포기 없음)
	// 2006-07-20 by ispark, 시나리오 미션 포기 가능 한거 추가
	if((pQuest->QuestKind == QUEST_KIND_SCENARIO) && (pQuest->IsDiscardable == 0))
		m_nMissionGiveUp = 0;
	else
		m_nMissionGiveUp = pQuest->QuestIndex;
	
	// 진행중인 미션이 있는경우 다른미션 실행불가
	if(g_pQuestData->GetProgressMission())
		m_bMissionStartButtonUse = TRUE;

	// 스크롤 설정
	m_pScrollRightWindow->Reset();
	m_pScrollRightWindow->SetNumberOfData((int)m_vecPossibleDesc.size());
}

void CINFMissionInfo::InitMission()
{
	FLOG( "CINFMissionInfo::initMission()" );	

	m_vecMissionList.clear();
	m_vecTabMissionNum.clear();

	m_vecCompensationItemName.clear();
	m_vecPossibleDesc.clear();
	m_vecMissionName.clear();
//	ZERO_MEMORY(m_strMissionName);
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);
}
void CINFMissionInfo::InitMissionTitle()
{
	FLOG( "CINFMissionInfo::InitMissionTitle()" );	
	
	//////////////////////////////////////////////////////////////////////////
	// 기타 초기화
	CQuest* pQuest = NULL;
	m_vecMissionList.clear();
	m_vecTabMissionNum.clear();
	ZERO_MEMORY(m_nCompensation);
	ZERO_MEMORY(m_bCompenToolTib);

	//////////////////////////////////////////////////////////////////////////
	// 진금까지 진행, 완료한 미션들과 
	// 세력 별로 나누어진 미션들을 입력 하는 부분
	SetCompleteMissionInput();
	
	//////////////////////////////////////////////////////////////////////////
	// 탭별 분류
	// 해결, 미해결, 전부 보기 분류
	SetMissionClassiFy();		

	//////////////////////////////////////////////////////////////////////////	
	// 미션 정렬 및 다음미션 보이기
	pQuest = SetMissionSorting();

	//////////////////////////////////////////////////////////////////////////
	// 미션 정보 설정( 진행가능미션 다음미션)
	SetMissionInfo();
	
	// 선택 미션 효과
	m_nSelectMissionIndex = -1;
	m_pScrollRightWindow->Reset();
	m_pScrollLeftWindow->Reset();	

	// 스크롤 설정
	m_pScrollLeftWindow->SetScrollLinePos( m_PointMissionPos.x+295, m_PointMissionPos.y+50);
	m_pScrollLeftWindow->SetWheelRect(
		m_PointMissionPos.x, 
		m_PointMissionPos.y,
		m_PointMissionPos.x+297,
		m_PointMissionPos.y+398);
	
	m_pScrollRightWindow->SetScrollLinePos( m_PointMissionPos.x+565, m_PointMissionPos.y+50);
	m_pScrollRightWindow->SetWheelRect(
		m_PointMissionPos.x+298,
		m_PointMissionPos.y,
		m_PointMissionPos.x+592,
		m_PointMissionPos.y+398);
	m_pScrollLeftWindow->SetNumberOfData((int)m_vecMissionList.size());
}

BOOL CINFMissionInfo::MissionWindowRunState()
{
	FLOG( "CINFMissionInfo::MissionWindowRunState()" );
	
	return TRUE;
}

DWORD CINFMissionInfo::MissionProcFlag(BOOL bFlag)
{
	FLOG( "CINFMissionInfo::MissionProcFlag(BOOL bFlag)" );
	
	if(m_bInfluenceFlag == TRUE)
		return MISSION_RUN_RENDER_ERR;	// 세력선택 미션중 다른 미션 실행안됨
	// 2006-07-21 by ispark, 바잡맵에서는 미션창 열지 못한다.
	if(g_pGround->m_bBazaar)
	{
		return MISSION_RUN_RENDER_ERR;
	}

	m_PointMissionPos.x = ((g_pD3dApp->GetBackBufferDesc().Width/2)-(MISSION_WINDOW_SIZE_WIDTH/2));
	m_PointMissionPos.y = ((g_pD3dApp->GetBackBufferDesc().Height/2)-(MISSION_WINDOW_SIZE_HEIGHT/2));

	InitMission();
	
	// 미션창 닫기 버튼 
	if(FALSE == bFlag) 
	{		
		m_bMissionWindowStart = FALSE;
		if(g_pTutorial)
		{
			g_pTutorial->SetTutorialEd(FALSE);
		}
		
		return MISSION_RUN_SUCCESS;
	}

	// 미션창 열기 버튼
	if(TRUE == bFlag){
		if(FALSE == MissionWindowRunState())
			return MISSION_RUN_RENDER_ERR;
		else
		{
			InitMissionTitle();
			m_bMissionWindowStart = TRUE;
		}		
	}
	return MISSION_RUN_SUCCESS;
}
void CINFMissionInfo::RenderPossibleMission()
{
	FLOG( "CINFMissionInfo::RenderPossibleMission()" );

	// 미션 제목	
	int nMissionNameLineCount = m_vecMissionName.size();
	for(int i = 0; i < nMissionNameLineCount; i++)
	{
		m_pMissionFontRight[0]->DrawText(
			m_PointMissionPos.x+MISSION_RIGHT_TEXT_GAB_X,       
			m_PointMissionPos.y+MISSION_RIGHT_TEXT_GAB_Y+(MISSION_FONT_HEIGHT_GAB*(i+1)),
			GUI_FONT_COLOR_YM, (char*)m_vecMissionName[i].c_str());
	}
	
	// 미션 디스크립션
	int nLineCount = 0;
	vector<string>::iterator it = m_vecPossibleDesc.begin();
	while(it != m_vecPossibleDesc.end())
	{
		if(nLineCount+nMissionNameLineCount>RIGHTFONT_MAX_LINE_COUNT)
			break;
		if(nLineCount+m_pScrollRightWindow->GetCurrentScrollIndex()>m_vecPossibleDesc.size())
			break;
		if(nLineCount+nMissionNameLineCount>=RIGHTFONT_MAX_LINE_COUNT)
		{
			m_pMissionFontRight[RIGHTFONT_MAX_LINE_COUNT-1]->DrawText(m_PointMissionPos.x+MISSION_RIGHT_TEXT_GAB_X,
			m_PointMissionPos.y+MISSION_RIGHT_TEXT_GAB_Y+33+((nLineCount+nMissionNameLineCount)*MISSION_FONT_HEIGHT_GAB),
			GUI_FONT_COLOR_W, " ");
		}
		else
		{
			m_pMissionFontRight[nLineCount+1]->DrawText(m_PointMissionPos.x+MISSION_RIGHT_TEXT_GAB_X,
				m_PointMissionPos.y+MISSION_RIGHT_TEXT_GAB_Y+33+((nLineCount+nMissionNameLineCount)*MISSION_FONT_HEIGHT_GAB),
				GUI_FONT_COLOR_W, (char*)m_vecPossibleDesc[nLineCount+m_pScrollRightWindow->GetCurrentScrollIndex()].c_str());
		}
		
		nLineCount++;
		it++;
	}
	// 보상아이템 렌더링
	RenderCompensationItem();

	// 미션 시작, 종료 버튼
	if(m_bMissionComButtonFlag == TRUE )
	{
		if(m_nMissionViewCase == MISSION_VIEW_SOLVE || m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
		{
			m_nMissionButtonStateStart = MISS_START_N;
			m_nMissionButtonStateEnd = MISS_COPLETE_N;
		}
		
		m_pMissionImg[m_nMissionButtonStateStart]->Move( m_PointMissionPos.x+312, m_PointMissionPos.y+290);
		m_pMissionImg[m_nMissionButtonStateStart]->Render();
	}
	
	// 2007-07-09 by bhsohn 미션 창 띠웠을 시, 메모리 버그 나는 현상처리
	CQuest*	pQuest = NULL;	
	if(m_nSelectMissionIndex >= 0)
	{
		pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);		
	}
	// 미션 시작 버튼 특정 상황에 Disable렌더링
	// 2006-03-24 by ispark, 파티미션일대 파티장만 버튼 보이기 추가
	//CQuest*	pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[m_nSelectMissionIndex]);
	if(m_bMissionStartButtonUse == TRUE 
		|| m_nSelectMissionIndex == -1 
		|| (pQuest && pQuest->IsPartyQuest() && g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _PARTYMASTER))

	{
		m_nMissionButtonStateEnd = MISS_COPLETE_NOT;

//		Disable버튼 렌더링 안함
//		if(!m_pQuestInfoProg)
//		{
//			m_pMissionImg[m_nMissionButtonStateEnd]->Move( m_PointMissionPos.x+485,	m_PointMissionPos.y+290);
//			m_pMissionImg[m_nMissionButtonStateEnd]->Render();
//		}
	}
	else
	{
		m_pMissionImg[m_nMissionButtonStateEnd]->Move( m_PointMissionPos.x+485,	m_PointMissionPos.y+290);
		m_pMissionImg[m_nMissionButtonStateEnd]->Render();
	}	
}

void CINFMissionInfo::RenderCompensationItem()
{
	FLOG( "CINFMissionInfo::RenderCompensationItem()" );

	int nCount = 0;
	int nItemGab = m_PointMissionPos.x+310;
	int nItemGabY = m_PointMissionPos.y+330;
	char buf[64];
	ZERO_MEMORY(buf);
	CINFImageEx* pSelectIcon;
	USHORT nUnitKind;
	vector<QUEST_PAY_ITEM>::iterator it = m_vecCompensationItemName.begin();
	while(it != m_vecCompensationItemName.end())
	{
		if(nCount>=MISSION_COMPENSATIONITEM_MAX)
			break;		
		if(it->ItemNum)
		{			
			ITEM * p2 = g_pDatabase->GetServerItemInfo(it->ItemNum);
			if(p2 == NULL)
				break;
			nUnitKind = it->ToUnitKind;						// 2006-11-14 by ispark
			if(nUnitKind == UNITKIND_ALL_MASK || 0 == nUnitKind || CompareBitFlag( nUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind ))
			{				
				/*--------------------------------------------------------------------------*/
//				wsprintf(buf, "%08d", p2->ItemNum);			// 2005-08-23 by ispark
				// 2005-11-29 by ispark, SourceIndex로 변경, 스킬은 SKILL_BASE_NUM으로 한다.
				if(IS_SKILL_ITEM(p2->Kind))
				{
					// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//					wsprintf(buf, "%08d", SKILL_BASE_NUM(p2->SourceIndex));
					wsprintf(buf, "%08d", SKILL_BASE_SOURCEINDEX(p2->SourceIndex));
				}
				else
				{
					wsprintf(buf, "%08d", p2->SourceIndex);
				}
				pSelectIcon = g_pGameMain->m_pIcon->FindIcon(buf);
				pSelectIcon->Move(nItemGab, nItemGabY);
				pSelectIcon->Render();

				if(m_bCompenToolTib[nCount] == TRUE && p2->ItemNum == 7000022)
				{	// 스피일경우만 표시
					ZERO_MEMORY(buf);				
					wsprintf(buf, "SPI:%d", (int)it->Count);
					g_pGameMain->RenderPopUpWindowImage(nItemGab, nItemGabY+32,strlen(buf)*6.5, 1 );
					m_pCompensation->DrawText(nItemGab, nItemGabY+MISSION_TOOLTIB_Y, GUI_FONT_COLOR_YM, buf);
				}
				else if(m_bCompenToolTib[nCount] == TRUE)
				{	// 그외의 경우 아이템 정보 표시
					if(p2) 
					{
						g_pGameMain->SetItemInfo(0, p2->ItemNum, nItemGab, nItemGabY+32, it->Count); 
					}
					else 
					{
						g_pGameMain->SetItemInfo( 0, 0, 0, 0); 
					}
				}

//				nItemGab = nItemGab + strlen(buf)*7.5;
				ZERO_MEMORY(buf);
				nItemGab += 52;			// 아이콘 넒이
				nCount++;
			}
		}		
		it++;
	}

	for(int i=0; i<COMPENS_MAX; i++)
	{
		if(m_nCompensation[i] == 0)
			continue;
		
		char bufName[COMPENS_MAX][256];
		ZERO_MEMORY(bufName);

		switch(i)
		{
		case COMPENS_EXP:
			wsprintf(buf, "%08d", ICON_EXP_ITEMNUM);
			strcpy(bufName[COMPENS_EXP], STRMSG_C_050819_0001);	//"경험치:"
			break;
		case COMPENS_LEV:
			wsprintf(buf, "%08d", ICON_LEV_ITEMNUM);
			strcpy(bufName[COMPENS_LEV], STRMSG_C_050819_0002);	//"레벨:"
			break;
		case COMPENS_PRO:
			wsprintf(buf, "%08d", ICON_PRO_ITEMNUM);
			strcpy(bufName[COMPENS_PRO], STRMSG_C_050819_0003);	//"명성치:"
			break;
		case COMPENS_BUN:
			wsprintf(buf, "%08d", ICON_BUN_ITEMNUM);
			strcpy(bufName[COMPENS_BUN], STRMSG_C_050819_0004); //"보너스스탯:"
			break;
		}
		pSelectIcon = g_pGameMain->m_pIcon->FindIcon(buf);
		pSelectIcon->Move(nItemGab, nItemGabY);
		pSelectIcon->Render();		
		
		if(m_bCompenToolTib[nCount] == TRUE)
		{
			ZERO_MEMORY(buf);
			wsprintf(buf, "%s%d", bufName[i], m_nCompensation[i]);
			g_pGameMain->RenderPopUpWindowImage(nItemGab, nItemGabY+32,strlen(buf)*6.5, 1 );
			m_pCompensation->DrawText(nItemGab, nItemGabY+MISSION_TOOLTIB_Y, GUI_FONT_COLOR_YM, buf);
		}

		nItemGab += 52;
		nCount++;
	}
}

void CINFMissionInfo::MissionStart(DWORD dMissionIndex)
{
	FLOG( "CINFMissionInfo::MissionStart()" );
	
	if(dMissionIndex == NULL)
		return;

	// 2005-11-15 by ispark
	// 미션 시작 조건 검색
	if(!CheckMissionStart())
	{
		return;
	}

	CQuest* pQuest = g_pQuestData->FindQuest(dMissionIndex);
	if(pQuest && pQuest->QuestEndType == QUEST_END_TYPE_IMMEDIATE)
	{
		// 시작시 바로 끝나는 미션 미션창 닫지 않음.
	}	
	else
	{
		m_bMissionWindowStart = FALSE;
	}

	// 특정 포지션 완료 미션 검사
	CQuest* pQuest2 = g_pQuestData->FindPosCompleteQuest( dMissionIndex );
	if(pQuest2)
	{
		SetPosMissionComplete(TRUE);
	}

	// 세력 선택 미션
	if(pQuest->QuestEndType == QUEST_END_TYPE_SELECT_INFLUENCE)
	{
		STAGE_EFFECT_DATA stStageData;
		stStageData.nMissionIndex = dMissionIndex;
		// 2008-04-02 by bhsohn 세력선택 프롤로그 스킵가능하게 변경
		//g_pInterface->InitSoundScript("Influence.tex", &stStageData, MISSION_PRE, FALSE);
		g_pInterface->InitSoundScript("Influence.tex", &stStageData, MISSION_PRE, TRUE);
		
		return;
	}
	
	g_pQuestData->SendFieldSocketQuestRequestStart(dMissionIndex,0);	

	// 2008-06-17 by bhsohn 편대 관련 처리
	// 강제로 자유 비행으로
	if(g_pShuttleChild->m_pClientParty 
		&& g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER 
		&& g_pShuttleChild->m_pClientParty->IsFormationFlight())
	{
		g_pShuttleChild->m_pClientParty->TempPartyFormation(FLIGHT_FORM_NONE);
	}
	// end 2008-06-17 by bhsohn 편대 관련 처리
}

void CINFMissionInfo::MissionEnd(DWORD dMissionIndex)
{
	FLOG( "CINFMissionInfo::MissionEnd()" );
	
	if(dMissionIndex == NULL)
		return;

	g_pQuestData->SendFieldSocketQuestRequestSuccess(dMissionIndex);
}

void CINFMissionInfo::RenderMissionTarget()
{
	FLOG( "CINFMissionInfo::RenderMissionTarget()" );
	
	CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
	m_pQuestInfoProgOutDoor = pQuestInfo;	
	
	if(NULL != pQuestInfo)
	{
		int nLineCount = 0;		// 라인 넒이
		int nbreak = 0;		// 라인 브레이크
		char buf[128];
		int nCount = 0;
		vector<int> nVecItemNum;
		int nItemNum = 0;
		nVecItemNum.clear();
		
		// 미션 이름 표시
		m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, pQuestInfo->QuestName);
		nLineCount++;
		
		// 이하 필요 아이템
		vector<ITEM_W_COUNT>::iterator it = pQuestInfo->TargetItemVector.begin();
		while(it != pQuestInfo->TargetItemVector.end())
		{
			if(nbreak>3)
				break;
			
			if(it->Count < 0)
			{// 2007-03-15 by dgwoo 위치정보만 보이는 데이타
				it++;
				continue;
			}
			//////////////////////////////////////////////////////////////////////////	
			// 2005-08-02 ispark		
			ITEM* pItem = g_pDatabase->GetServerItemInfo(it->ItemNum);
			if(pItem)
			{				
				util::zero(buf);
				
				wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, (*it).Count); //"목표:%s %d개"

				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nCount = it->Count;
				nVecItemNum.push_back(it->ItemNum);
				nLineCount++;
				nbreak++;
			}			
#ifdef _DEBUG
			else if(NULL == pItem)
			{
				ZERO_MEMORY(buf);
				wsprintf(buf, "ItemNum : %d Check",it->ItemNum);
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
				nbreak++;
			}
#endif
			//////////////////////////////////////////////////////////////////////////
			it++;
		}
		
		// 이하 목표 몬스터
		vector<MONSTER_W_COUNT>::iterator itMonC = pQuestInfo->TargetMonsterVector.begin();
		while(itMonC != pQuestInfo->TargetMonsterVector.end())
		{
			if(0 < itMonC->Count )
			{
				MEX_MONSTER_INFO * pTempIn = g_pDatabase->CheckMonsterInfo((*itMonC).MonsterUniqueNumber);
				
				util::zero(buf);

				wsprintf( buf, STRMSG_C_090112_0203,pTempIn->MonsterName, (*itMonC).Count ); //"\\e%s\\e \\w%d개/%d개\\w"

				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
			itMonC++;
		}
		
		nbreak = 0;
		vector<ITEM_W_COUNT_MAP_AREA>::iterator it2 = pQuestInfo->TargetMonsterItemVector.begin();
		while(it2 != pQuestInfo->TargetMonsterItemVector.end())
		{
			if(nbreak>3)
				break;

			if(it2->ItemWithCount.Count < 0)
			{// 2007-03-15 by dgwoo 위치정보만 가지는 데이타 
				it2++;
				continue;
			}
			//////////////////////////////////////////////////////////////////////////			
			ITEM* pItem = g_pDatabase->GetServerItemInfo(it2->ItemWithCount.ItemNum);
			if(pItem)
			{
				m_vecOldString.clear();
				if(pQuestInfo->vecQuestDropItem.size()<0)
				{
					util::zero(buf);

					wsprintf(buf, STRMSG_C_090112_0203, pItem->ItemName, it2->ItemWithCount.Count); //"목표:%s %d개"

					m_vecOldString.push_back(buf);
					
					m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 
						20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
						buf);
					
					nCount = it2->ItemWithCount.Count;
					nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
					nbreak++;
					nLineCount++;
				}
				
				vector<QUEST_DROP_ITEM>::iterator itItem = pQuestInfo->vecQuestDropItem.begin();
				while(itItem != pQuestInfo->vecQuestDropItem.end())
				{
					QUEST_DROP_ITEM *pTemp = &*itItem;
					if(pItem->ItemNum == pTemp->ItemNumber)
					{
						MEX_MONSTER_INFO * pMonster = g_pDatabase->CheckMonsterInfo(pTemp->MonsterUniqueNumber);							
						if(pMonster)
						{
							ZERO_MEMORY(buf);
							// 2009-01-12 by bhsohn 일본 추가 수정사항
// 							wsprintf(buf, STRMSG_C_050818_0011, pItem->ItemName,pMonster->MonsterName,
// 								it2->ItemWithCount.Count); //"목표:%s %d개"
							wsprintf(buf, STRMSG_C_090112_0205, pItem->ItemName,pMonster->MonsterName,
								it2->ItemWithCount.Count); //"목표:%s %d개"
							// end 2009-01-12 by bhsohn 일본 추가 수정사항

							m_vecOldString.push_back(buf);
							
							m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 
								20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W,
								buf);
							
							nCount = it2->ItemWithCount.Count;
							nVecItemNum.push_back(it2->ItemWithCount.ItemNum);
							nbreak++;
							nLineCount++;						
						}
					}
					
					itItem++;
				}				
			}
#ifdef _DEBUG
			else if(NULL == pItem)
			{
				ZERO_MEMORY(buf);
				wsprintf(buf, "ItemNum : %d Check",it2->ItemWithCount.ItemNum);
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nbreak++;
				nLineCount++;
			}
#endif
			//////////////////////////////////////////////////////////////////////////
			it2++;
		}
		// 이하 NPC 이름
		if(strlen(pQuestInfo->TargetMeetNPCInfo.NPCName)>0)
		{
			ZERO_MEMORY(buf);			
			wsprintf(buf, STRMSG_C_050726_0003, pQuestInfo->TargetMeetNPCInfo.NPCName); // "[%s]을 찾아가라!"
			m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		
		// 이하 만나야하는 오브젝트
		// 이하 어떠한 맵의 워프게이트 통과
		if(pQuestInfo->TargetMeetObjectMapArea.MapIndex)
		{
			MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(pQuestInfo->TargetMeetObjectMapArea.MapIndex);
			if(pMapInfo)
			{
				ZERO_MEMORY(buf);			
				wsprintf(buf, STRMSG_C_050726_0004, pMapInfo->MapName); //"[%s]을 통과하라!"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
		}
		
		// 이하 목표 레벨업
		if(pQuestInfo->QuestEndType == QUEST_END_TYPE_LEVEL_UP)
		{
			ZERO_MEMORY(buf);			
			wsprintf(buf, STRMSG_C_050805_0001); // "레벨을 한단계 올리세요"
			m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
			nLineCount++;
		}
		
		// 퀘스트 남은시간 표시
		if(g_pGameMain->m_bQuestLimitTimeView)
		{
			ZERO_MEMORY(buf);			
			// 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
			//if(g_pGameMain->m_nTimeSecond >= 0)					
//			{
//				wsprintf(buf, STRMSG_C_INTERFACE_0020, g_pGameMain->m_nTimeSecond/60, g_pGameMain->m_nTimeSecond%60);//"[남은시간] %2.d분 %2.d초"
//				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
//				nLineCount++;
//			}
			if(g_pGameMain->GetQuestTimeElapse() >= 0)
			{
				int nTimeSecond = (int)g_pGameMain->GetQuestTimeElapse();
				wsprintf(buf, STRMSG_C_INTERFACE_0020, nTimeSecond/60, nTimeSecond%60);//"[남은시간] %2.d분 %2.d초"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
			// end 2008-04-07 by bhsohn 퀘스트 시간 지연되는 문제 처리
		}
		// 2007-04-10 by dgwoo 믹스 아이템이 있으면.
		if(pQuestInfo->TargetMixItemTarget.Count > 0)
		{
			ZERO_MEMORY(buf);	
			ITEM* pItem = g_pDatabase->GetServerItemInfo(pQuestInfo->TargetMixItemTarget.ItemNum);
			if(pItem != NULL)
			{
				wsprintf(buf, STRMSG_C_070410_0100,pItem->ItemName);	//"[팩토리]에서 %s(으)로 조합하라"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
		}
		
		// 퀘스트 아이템 몇개 습득
		vector<int>::iterator vecInt = nVecItemNum.begin();
		while(vecInt != nVecItemNum.end())
		{
			ITEM_GENERAL *pItem = g_pStoreData->FindItemInInventoryByItemNum(*vecInt);
			if( pItem )
			{
				ZERO_MEMORY(buf);
				wsprintf( buf, STRMSG_C_050726_0005, pItem->ItemInfo->ItemName, pItem->CurrentCount );//"\\e%s\\e \\w%d개/%d\\w"
				m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
				nLineCount++;
			}
			vecInt++;
		}
		
		// 퀘스트 몬스터를 사냥한 수
		vector<MONSTER_W_COUNT>::iterator itMon = pQuestInfo->TargetMonsterVector.begin();
		while(itMon != pQuestInfo->TargetMonsterVector.end())
		{
			vector<Quest_MonsterInfo>::iterator it = g_pQuestData->m_vecQuestMonsterCount.begin();
			while(it != g_pQuestData->m_vecQuestMonsterCount.end())
			{
				MEX_MONSTER_INFO * pTempInfo = g_pDatabase->CheckMonsterInfo((*it).nMonsterUnitkind);
				if(pTempInfo)
				{
					if((*itMon).MonsterUniqueNumber == (*it).nMonsterUnitkind)
					{
						if(0 < itMon->Count)
						{
						ZERO_MEMORY(buf);
						wsprintf( buf, STRMSG_C_051026_0001,pTempInfo->MonsterName, (*it).nMonsterCount, (*itMon).Count ); //"\\e%s\\e \\w%d개/%d개\\w"
						m_pMissionTarget[nLineCount]->DrawText(MISSION_VIEW_INFO_POSX, 20+(nLineCount*MISSION_FONT_HEIGHT_GAB), GUI_FONT_COLOR_W, buf);
						nLineCount++;
						}
					}					
				}
				it++;
			}
			itMon++;
		}		
	}
	
	if(pQuestInfo)
	{
		m_nOldMissionIndex = pQuestInfo->QuestIndex;
	}
}

int CINFMissionInfo::CheckProgressMission(CQuest *pQuestInfo)
{
	BOOL err = TRUE;
	// 에러 초기화
	m_vecErrPart.clear();
	m_vecErrQuestName.clear();
	m_vecErritem.clear();

	if (FALSE == COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuestInfo->ReqUnitKind))
	{// 가능 세력 체크
		m_vecErrPart.push_back(MISSION_ERR_FLUENCE);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqAttackPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart, pQuestInfo->ReqAttackPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_AT);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqDefensePart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart, pQuestInfo->ReqDefensePart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_DE);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqFuelPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart, pQuestInfo->ReqFuelPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_FU);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqSoulPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart, pQuestInfo->ReqSoulPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_SO);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqShieldPart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart, pQuestInfo->ReqShieldPart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_SH);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqDodgePart.Min, g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart, pQuestInfo->ReqDodgePart.Max))
	{
		m_vecErrPart.push_back(MISSION_ERR_STAT_DO);
		err = FALSE;
	}

	if (!util::in_range(pQuestInfo->ReqLevel.Min, g_pShuttleChild->m_myShuttleInfo.Level, pQuestInfo->ReqLevel.Max))	// 필요레벨
	{
		m_vecErrPart.push_back(MISSION_ERR_LEVEL);
		err = FALSE;
	}

	if (pQuestInfo->ReqExperience > g_pShuttleChild->m_myShuttleInfo.Experience)	// 필요 경험치
	{
		m_vecErrPart.push_back(MISSION_ERR_EXPERIENCE);
		err = FALSE;
	}

	int X = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.x);
	int Z = (int)(g_pD3dApp->m_pShuttleChild->m_vPos.z);
	if (pQuestInfo->ReqStartArea.MapIndex && FALSE == pQuestInfo->ReqStartArea.IsPositionInArea(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, X,Z))	// 히든 퀘스트
	{
		m_vecErrPart.push_back(MISSION_ERR_HIDEQUEST);
		err = FALSE;
	}
	
//	// 미션 완료 지역 체크
//	if (pQuestInfo->TargetMeetMapArea.MapIndex &&
//		FALSE == pQuestInfo->TargetMeetMapArea.IsPositionInArea(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, X,Z))	// 히든 퀘스트
//	{
//		m_vecErrPart.push_back(MISSION_ERR_POSENDQUEST);
//		err = FALSE;
//	}

	int i = 0;
	BOOL bReqItem = FALSE;
	// 필요 아이템 확인
	for (i = 0; i < pQuestInfo->ReqItemVector.size(); i++)
	{
		ITEM_W_COUNT tmpItemWCount = pQuestInfo->ReqItemVector[i];

		CItemInfo *pItem = g_pStoreData->FindItemInInventoryByItemNum(tmpItemWCount.ItemNum);
		if (pItem == NULL || (IS_GENERAL_ITEM(pItem->Kind) && pItem->CurrentCount < tmpItemWCount.Count))
		{
			m_vecErritem.push_back(pItem->ItemInfo->ItemName);
			bReqItem = TRUE;			
			err = FALSE;
		}
	}
	if(bReqItem == TRUE)	{
		m_vecErrPart.push_back(MISSION_ERR_ITEM);
	}

//	// 필요 몬스터 아이템 확인
//	bReqItem = FALSE;
//	for (i = 0; i < pQuestInfo->TargetMonsterItemVector.size(); i++)
//	{
//		if (pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.ItemNum != 0)
//		{
//			CItemInfo *pTargetMonsterItem = g_pStoreData->FindItemInInventoryByItemNum(pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.ItemNum);
//			
//			// Count가 0이면 꼭 있지 않아도 되는 아이템임
//			if (pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.Count == 0)
//			{
//				continue;
//			}
//			
//			// 해당 아이템을 가지지 않은 경우
//			if (pTargetMonsterItem == NULL)
//			{
//				err = FALSE;
//				bReqItem = TRUE;
//				break;
//			}
//			
//			// 해당 아이템의 개수가 부족한 경우
//			if (IS_COUNTABLE_ITEM(pTargetMonsterItem->ItemInfo->Kind)
//				&& pTargetMonsterItem->CurrentCount < pQuestInfo->TargetMonsterItemVector[i].ItemWithCount.Count)
//			{
//				err = FALSE;
//				bReqItem = TRUE;
//				break;
//			}
//		}
//	}
//	if(bReqItem == TRUE)	{
//		m_vecErrPart.push_back(MISSION_ERR_MONSTER_ITEM);
//	}

	// 필요 quest 확인
	BOOL	bReqQuest = FALSE;
	for (i = 0; i < pQuestInfo->ReqQuestIndexVector.size(); i++)
	{
		MEX_QUEST_INFO* pCharacterQuest = g_pQuestData->FindCharacterQuest(pQuestInfo->ReqQuestIndexVector[i]);
		if(pCharacterQuest == NULL || pCharacterQuest->QuestState != QUEST_STATE_COMPLETED)
		{
			if(pQuestInfo)
			{
				CQuest* pQuest = NULL;
				vector<INT>::iterator itIndex = pQuestInfo->ReqQuestIndexVector.begin();
				while(itIndex != pQuestInfo->ReqQuestIndexVector.end())
				{
					pQuest = g_pQuestData->FindQuest(*itIndex);
					if(pQuest){
						m_vecErrQuestName.push_back( pQuest->QuestName );
					}
					itIndex++;
				}
				
			}
			bReqQuest = TRUE;
			err = FALSE;
		}
	}
	if(bReqQuest == TRUE) {
		m_vecErrPart.push_back(MISSION_ERR_QUEST);
	}

	return err;
}

void CINFMissionInfo::RefreshMission()
{
	m_nSelectMissionIndex = -1;
	m_nMissionViewCase = MISSION_VIEW_ALL;
	m_pScrollRightWindow->Reset();
	m_pScrollLeftWindow->Reset();
	
	InitMission();
	InitMissionTitle();
}

void CINFMissionInfo::SetInfluenceMission(BOOL nFlag, float fANIPercent, float fVANPercent)
{
	// 세력선택 미션 설정부분 ( TRUE 설정시 다른미션 실행 불가, 출격하기 불가)
	m_fANIInflDistributionPercent = fANIPercent;
	m_fVCNInflDistributionPercent = fVANPercent;
	if(nFlag)
	{
		MissionProcFlag(FALSE);
		InitInfluenceMission();	
	}
	else
	{
		m_bInfluenceFlag = FALSE;
	}
}

void CINFMissionInfo::InitInfluenceMission()
{
	m_bInfluenceFlag = TRUE;

	m_nInfluencePosX = (g_pD3dApp->GetBackBufferDesc().Width/2);
	m_nInfluencePosY = (g_pD3dApp->GetBackBufferDesc().Height/2);
	
	POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();

	m_nInfluencePosX = m_nInfluencePosX-(pSize.x/2);
	m_nInfluencePosY = m_nInfluencePosY-(pSize.y/2);

	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0007);	// "[설명]"
	STRING_CULL(STRMSG_C_050818_0001, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[0]);	
	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0008);	// "[배경]"
	STRING_CULL(STRMSG_C_050818_0002, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[0]);	
	m_vecVCUfluenceDesc.push_back(STRMSG_C_050818_0009);	//"[Aerial Cry]"
	STRING_CULL(STRMSG_C_050818_0003, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecVCUfluenceDesc, m_pMissionFontfluenceL[0]);	

	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0007);	// "[설명]"
	STRING_CULL(STRMSG_C_050818_0004, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[0]);	
	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0008);	// "[배경]"
	STRING_CULL(STRMSG_C_050818_0005, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[0]);		
	m_vecANIfluenceDesc.push_back(STRMSG_C_050818_0009);	//"[Aerial Cry]"
	STRING_CULL(STRMSG_C_050818_0006, MISSION_FLUNCE_MAX_DESC_STRING, &m_vecANIfluenceDesc, m_pMissionFontfluenceR[0]);
	
	m_pScrollLeftfluence->SetNumberOfData(m_vecVCUfluenceDesc.size());
	m_pScrollRightfluence->SetNumberOfData(m_vecANIfluenceDesc.size());
	
}

void CINFMissionInfo::RenderInfluenceMission()
{
	m_pMissionImg[MISS_INFLUENCE_BK]->Move(m_nInfluencePosX, m_nInfluencePosY);
	m_pMissionImg[MISS_INFLUENCE_BK]->Render();
	
	// 버튼 렌더링
	m_pMissionImg[m_nVCUButton]->Move(m_nInfluencePosX+133, m_nInfluencePosY+489);
	m_pMissionImg[m_nVCUButton]->Render();

	m_pMissionImg[m_nANIButton]->Move(m_nInfluencePosX+509, m_nInfluencePosY+489);
	m_pMissionImg[m_nANIButton]->Render();

	for(int i=0; i<MISSION_SCROLL_FLUENCE_MAX_LINE; i++)
	{
		if(m_vecVCUfluenceDesc.size()>i+m_pScrollLeftfluence->GetCurrentScrollIndex())
		{	// 정부군 정보
			m_pMissionFontfluenceL[i]->DrawText(m_nInfluencePosX+40,m_nInfluencePosY+130+(i*MISSION_FONT_HEIGHT_GAB),GUI_FONT_COLOR_W,
				(char*)m_vecVCUfluenceDesc[i+m_pScrollLeftfluence->GetCurrentScrollIndex()].c_str());
		}

		if(m_vecANIfluenceDesc.size()>i+m_pScrollRightfluence->GetCurrentScrollIndex())
		{	// 반란군 정보
			m_pMissionFontfluenceR[i]->DrawText(m_nInfluencePosX+410,m_nInfluencePosY+130+(i*MISSION_FONT_HEIGHT_GAB),GUI_FONT_COLOR_W, 
				(char*)m_vecANIfluenceDesc[i+m_pScrollRightfluence->GetCurrentScrollIndex()].c_str());
		}
	}

	m_pScrollLeftfluence->Render();
	m_pScrollRightfluence->Render();

	// 2006-02-09 by ispark, 경고시
	if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
	{
		int nDistributionPercent = 0;
		m_pImgWarning->Move(MISSION_WARNING_X, MISSION_WARNING_Y);
		m_pImgWarning->Render();

		char chInfluenceName[20];
		if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN)
		{
			sprintf(chInfluenceName, STRMSG_C_060210_0000);
			nDistributionPercent = m_fVCNInflDistributionPercent;
		}
		if(m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
		{
			sprintf(chInfluenceName, STRMSG_C_060210_0001);
			nDistributionPercent = m_fANIInflDistributionPercent;
		}

		char MessageInfluenceWarning[1024];
		sprintf(MessageInfluenceWarning, STRMSG_C_060208_0001, (int)m_fVCNInflDistributionPercent, (int)m_fANIInflDistributionPercent); //"현재의 세력분포 상황은 바이제니유가 \\e%d\\e%%, 알링턴이 \\e%d\\e%%입니다."
		m_pInfluenceWarning[0]->DrawText(MISSION_WARNING_INFO_X, MISSION_WARNING_INFO_Y, GUI_FONT_COLOR_W, MessageInfluenceWarning);
		sprintf(MessageInfluenceWarning, STRMSG_C_060209_0000, chInfluenceName, (int)nDistributionPercent - 50); //"\\y%s\\y 세력의 상점이용요금에 \\r%d\\r%%가 더 비싼 상황입니다."
		m_pInfluenceWarning[1]->DrawText(MISSION_WARNING_INFO_X, MISSION_WARNING_INFO_Y + 15, GUI_FONT_COLOR_W, MessageInfluenceWarning);
		sprintf(MessageInfluenceWarning, STRMSG_C_060209_0001, chInfluenceName); //""그래도 \\y%s\\y 세력을 선택하시겠습니까?"
		m_pInfluenceWarning[2]->DrawText(MISSION_WARNING_INFO_X, MISSION_WARNING_INFO_Y + 30, GUI_FONT_COLOR_W, MessageInfluenceWarning);

		if(m_nInflChoiceOKButton >= 0)
		{
			m_pImgOkButton[m_nInflChoiceOKButton]->Move(MISSION_WARNING_OK_X, MISSION_WARNING_OK_Y);
			m_pImgOkButton[m_nInflChoiceOKButton]->Render();
		}
		else if(m_nInflChoiceCANButton >= 0)
		{
			m_pImgCancelButton[m_nInflChoiceCANButton]->Move(MISSION_WARNING_CAN_X, MISSION_WARNING_CAN_Y);
			m_pImgCancelButton[m_nInflChoiceCANButton]->Render();
		}
	}

}

int	CINFMissionInfo::WndProcInfluence(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 스크롤 처리
	if(m_pScrollLeftfluence && m_pScrollRightfluence){
		if(m_pScrollLeftfluence->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
		else if(m_pScrollRightfluence->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK){
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);			
			
			// 버튼 초기화
			m_nVCUButton = MISS_INFLUENCE_NR;
			m_nANIButton = MISS_INFLUENCE_NR;

			// 2006-02-09 by ispark, 경고시
			m_nInflChoiceOKButton = -1;
			m_nInflChoiceCANButton = -1;
			if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
			{
				if(pt.x > MISSION_WARNING_OK_X && pt.x < MISSION_WARNING_OK_X + 38 &&
					pt.y > MISSION_WARNING_OK_Y && pt.y < MISSION_WARNING_OK_Y + 17)
				{
					m_nInflChoiceOKButton = 0;
				}
				else if(pt.x > MISSION_WARNING_CAN_X && pt.x < MISSION_WARNING_CAN_X + 38 &&
					pt.y > MISSION_WARNING_CAN_Y && pt.y < MISSION_WARNING_CAN_Y + 17)
				{
					m_nInflChoiceCANButton = 0;
				}
				break;
			}
			
			if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
				pt.x>m_nInfluencePosX+133 && pt.x<m_nInfluencePosX+253)
			{
				if(m_bVCUButton)
					m_nVCUButton = MISS_INFLUENCE_PS;
				else
					m_nVCUButton = MISS_INFLUENCE_OV;
			}
			else if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
				pt.x>m_nInfluencePosX+509 && pt.x<m_nInfluencePosX+629)
			{
				if(m_bANIButton)
					m_nANIButton = MISS_INFLUENCE_PS;
				else
					m_nANIButton = MISS_INFLUENCE_OV;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			// 2006-02-09 by ispark, 경고시
			if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
			{
				if(pt.x > MISSION_WARNING_OK_X && pt.x < MISSION_WARNING_OK_X + 38 &&
					pt.y > MISSION_WARNING_OK_Y && pt.y < MISSION_WARNING_OK_Y + 17)
				{
					m_nInflChoiceOKButton = 1;
				}
				else if(pt.x > MISSION_WARNING_CAN_X && pt.x < MISSION_WARNING_CAN_X + 38 &&
					pt.y > MISSION_WARNING_CAN_Y && pt.y < MISSION_WARNING_CAN_Y + 17)
				{
					m_nInflChoiceCANButton = 1;
				}
				return INF_MSGPROC_BREAK;
			}

			m_bVCUButton = FALSE;
			m_bANIButton = FALSE;
			if(pt.y>m_nInfluencePosY && pt.y<m_nInfluencePosY+pSize.y &&
				pt.x>m_nInfluencePosX && pt.x<m_nInfluencePosX+pSize.x)
			{
				if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+133 && pt.x<m_nInfluencePosX+253)
				{
					m_bVCUButton = TRUE;
					m_nVCUButton = MISS_INFLUENCE_PS;					
				}
				else if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+509 && pt.x<m_nInfluencePosX+629)
				{
					m_bANIButton = TRUE;
					m_nANIButton = MISS_INFLUENCE_PS;					
				}
				
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			POINT pSize = m_pMissionImg[MISS_INFLUENCE_BK]->GetImgSize();
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// 2006-02-09 by ispark, 경고시
			if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN || m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
			{
				m_nInflChoiceOKButton = -1;
				m_nInflChoiceCANButton = -1;
				if(pt.x > MISSION_WARNING_OK_X && pt.x < MISSION_WARNING_OK_X + 38 &&
					pt.y > MISSION_WARNING_OK_Y && pt.y < MISSION_WARNING_OK_Y + 17)
				{
					if(m_nInflChoiceOver == INFLUENCE_TYPE_VCN)
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_VCN);
					}
					else if(m_nInflChoiceOver == INFLUENCE_TYPE_ANI)
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_ANI);
					}
				}
				else if(pt.x > MISSION_WARNING_CAN_X && pt.x < MISSION_WARNING_CAN_X + 38 &&
					pt.y > MISSION_WARNING_CAN_Y && pt.y < MISSION_WARNING_CAN_Y + 17)
				{
					m_nInflChoiceOver = INFLUENCE_TYPE_NORMAL;
				}
				
				return INF_MSGPROC_BREAK;
			}
				
			if(pt.y>m_nInfluencePosY && pt.y<m_nInfluencePosY+pSize.y &&
				pt.x>m_nInfluencePosX && pt.x<m_nInfluencePosX+pSize.x)
			{
				if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+133 && pt.x<m_nInfluencePosX+253 &&
					m_bVCUButton == TRUE)
				{
					// 세력 선택
					// 2006-02-08 by ispark, 세력 퍼센트 추가
					if(m_fVCNInflDistributionPercent > 50.0f)
					{
						if(g_pD3dApp->m_pSound)
						{
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
						}						
						m_nInflChoiceOver = INFLUENCE_TYPE_VCN;
					}
					else
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_VCN);
					}
					m_nVCUButton = MISS_INFLUENCE_OV;
				}
				else if(pt.y>m_nInfluencePosY+489 && pt.y<m_nInfluencePosY+525 &&
					pt.x>m_nInfluencePosX+509 && pt.x<m_nInfluencePosX+629 &&
					m_bANIButton == TRUE)
				{
					// 세력 선택
					// 2006-02-08 by ispark, 세력 퍼센트 추가
					if(m_fANIInflDistributionPercent > 50.0f)
					{
						if(g_pD3dApp->m_pSound)
						{
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OPEN_MESSAGE_BOX, g_pShuttleChild->m_vPos, FALSE);
						}
						m_nInflChoiceOver = INFLUENCE_TYPE_ANI;
					}
					else
					{
						CQuest *pQuestInfo = g_pQuestData->GetProgressMission();
						g_pQuestData->SendFieldSocketQuestRequestSuccess(pQuestInfo->QuestIndex,INFLUENCE_TYPE_ANI);
					}
					m_nANIButton = MISS_INFLUENCE_OV;					
				}

				m_bVCUButton = FALSE;
				m_bANIButton = FALSE;
				return INF_MSGPROC_BREAK;
			}

			m_bVCUButton = FALSE;
			m_bANIButton = FALSE;
		}
		break;
//	case WM_MBUTTONDOWN:
//		{
//			// 테스트 코드
//			SetInfluenceMission(!m_bInfluenceFlag);
//		}
//		break;
	}
	return INF_MSGPROC_NORMAL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFMissionInfo::CheckMissionStart()
/// \brief		미션 수행을 위한 조건 검색
/// \author		ispark
/// \date		2005-11-16 ~ 2005-11-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFMissionInfo::CheckMissionStart()
{
	CMapItemWindowInventoryIterator it;

	// 2006-11-07 by ispark, 레이더는 경고로만 출력
	it = g_pStoreData->m_mapItemWindowPosition.find(POS_PROW);
	if(it == g_pStoreData->m_mapItemWindowPosition.end())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051229_0101,COLOR_SYSTEM); // "레이더가 장착되어 있지 않습니다."
	}

	//*--------------------------------------------------------------------------*//
	// 엔진이 없다면 미션 출발 못함
	it = g_pStoreData->m_mapItemWindowPosition.find(POS_REAR);
	if(it == g_pStoreData->m_mapItemWindowPosition.end())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051115_0002,COLOR_SYSTEM); // "엔진 미장착으로 실행할 수 없습니다."
		return FALSE;
	}

	it = g_pStoreData->m_mapItemWindowPosition.find(POS_CENTER);
	if(it == g_pStoreData->m_mapItemWindowPosition.end())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051206_0001,COLOR_SYSTEM); // "아머 미장착으로 실행할 수 없습니다."
		return FALSE;
	}

	// 2005-12-07 by ispark, 세력 선택 미션일 경우 파티상태가 아닐때 클릭 가능
	if(m_nSelectMissionIndex != -1 &&											// 미션 서택이 없다면 넘어감
		m_vecTabMissionNum[m_nSelectMissionIndex] == 112 && 
		g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType != _NOPARTY)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051207_0001,COLOR_SYSTEM); // "편대상태에서는 세력을 선택할 수 없습니다."
		return FALSE;
	}

	// 2006-10-17 by ispark, 죽어 있을 경우 출격 못하게
	if(g_pShuttleChild->CheckUnitState() == BREAKDOWN)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061017_0001,COLOR_SYSTEM); // "출격 할 수 없습니다."
		return FALSE;
	}

	// 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리
	if(g_pInterface->IsBazarOpen())
	{		
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_061017_0001,COLOR_SYSTEM); // "출격 할 수 없습니다."
		return FALSE;
	}	
	// end 2007-07-09 by bhsohn 출격과 바자상점 동시 사용시 문제점 처리

	return TRUE;
}


void CINFMissionInfo::SetCompleteMissionInput()
{
	//////////////////////////////////////////////////////////////////////////	
	// 세력별 퀘스트 분류후 완료 퀘스트를 가져온다
	// 세력이 틀려도 완료된 퀘스트는 화면에 표시한다	
	// 같은 세력의 미션을 찾는 부분
	vector<int> vecQuestInflue;
	vecQuestInflue = g_pQuestData->GetRaceQuestIndex(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
	
	// 전체 미션을 가져오는 부분
	vector<int> vecQuestTemp;
	vecQuestTemp = g_pQuestData->GetRaceQuestIndex(INFLUENCE_TYPE_ALL_MASK);	
	m_vecQuestIndex.clear();	// 실제 창에 보여주는 미션 인덱스

	vector<int>::iterator itQuest = vecQuestTemp.begin();
	while(itQuest != vecQuestTemp.end())
	{
		if(g_pQuestData->IsQuestCompleted(*itQuest))
		{
			// 비효율 적인 방법이지만 기존의 방식을 바꾸지 않고 세력이 틀리더라도 
			// 완료 미션을 보여주기 위한 방법
			// 후에 교체 요망
			BOOL nQFlag = TRUE;
			vector<int>::iterator itQuestSolve = vecQuestInflue.begin();
			while(itQuestSolve != vecQuestInflue.end())
			{
				if(*itQuest == *itQuestSolve)
				{
					nQFlag = FALSE;
				}

				itQuestSolve++;
			}
			if(nQFlag) 
				m_vecQuestIndex.push_back(*itQuest);
		}
		itQuest++;
	}
	// 마찬가지 교체 요망
	vector<int>::iterator itQuestSolve2 = vecQuestInflue.begin();
	while(itQuestSolve2 != vecQuestInflue.end())
	{
		m_vecQuestIndex.push_back(*itQuestSolve2);
		itQuestSolve2++;
	}
	//
	//////////////////////////////////////////////////////////////////////////
	// 2007-04-16 by dgwoo 이하 미션 정열해야 하는 부분.
	//////////////////////////////////////////////////////////////////////////
}

void CINFMissionInfo::SetMissionClassiFy()
{
	int nQuestKind = 0;
	CQuest* pQuest = NULL;
	char strTemp[512];
	ZERO_MEMORY(strTemp);
	m_bMissionStartButtonUse = FALSE;
	m_pQuestInfoProg = NULL;
	vector<int>::iterator it = m_vecQuestIndex.begin();
	while(it != m_vecQuestIndex.end())
	{
		pQuest = g_pQuestData->FindQuest(*it);

		//////////////////////////////////////////////////////////////////////////
		// 탭설정 부분 (탭선택으로 미션 종류 명칭을 정한다)
		if(m_nSelectMissionTab == MISS_NOM)
		{
			nQuestKind = QUEST_KIND_NORMAL; // 일반 Quest
			if(FALSE == COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqUnitKind))
			{
				it++;
				continue;
			}
		}
		else if(m_nSelectMissionTab == MISS_SCEN)
		{
			nQuestKind = QUEST_KIND_SCENARIO; // 시나리오 Quest
			if(CheckProgressMission(pQuest) == FALSE)
			{
				it++;
				continue;
			}
		}
		else if(m_nSelectMissionTab == MISS_SCEN_D)
		{
//			nQuestKind = QUEST_KIND_COLLECTION; // 컬렉션 Quest
			nQuestKind = QUEST_KIND_SIDESTORY;  // 컬렉션 Quest
		}
	

		//////////////////////////////////////////////////////////////////////////		
		// 미션 입력 부분
		if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_SOLVE && g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
		// 해결 미션
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
		}
		// 2006-03-28 by ispark, 미 해결 미션 수정(할 수 있는 미션까지)
//		else if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_UNSOLVE && g_pQuestData->IsQuestProgress(pQuest->QuestIndex)){
		else if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_UNSOLVE && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
		{
		// 미 해결 미션
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
		}
		else if(nQuestKind == pQuest->QuestKind && m_nMissionViewCase == MISSION_VIEW_ALL)	{
		// 미션 전부보기
			ZERO_MEMORY(strTemp);
			
			if(g_pQuestData->IsQuestProgress(pQuest->QuestIndex))
				wsprintf(strTemp, STRMSG_C_050805_0002,pQuest->QuestName); // "%s [미해결]"
			else if(g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
				wsprintf(strTemp, STRMSG_C_050805_0003,pQuest->QuestName);
			else
				wsprintf(strTemp, "%s",pQuest->QuestName);

			if( MISS_SCEN == m_nSelectMissionTab  
				|| MISS_SCEN_D == m_nSelectMissionTab
				|| MISS_NOM == m_nSelectMissionTab)// && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
			{
				m_vecMissionList.push_back(strTemp);
				m_vecTabMissionNum.push_back(pQuest->QuestIndex);
			}
// 2007-02-23 by dgwoo
//			else
//			{
//				m_vecMissionList.push_back(strTemp);
//				m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//			}
		}
		it++;
	}	
}
// 2007-02-23 by dgwoo 원래 소스임.
//CQuest* CINFMissionInfo::SetMissionSorting()
//{
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 시나리오 미션 다음 시나리오 1개 더보여주기 ( 기타 툴팁설정 )
//	// 2005-08-22 by ydkim
//	// 제일 하위 미션착기 -검색-
//	// ?은 부분으로 GOTO문 사용 후에 수정요망
//	CQuest* pQuest = NULL;
//	m_bProgressMission = FALSE;
//	if(g_pQuestData->GetProgressMission())
//		m_bMissionStartButtonUse = TRUE;
//
//	m_nMaxNum = m_vecQuestIndex[0];							// 제일 초반 미션인덱스.
//QUESTSERCH:
//	pQuest = g_pQuestData->FindQuest(m_nMaxNum);					// 퀘스트 정보 로딩
//	if(pQuest)
//	{
//		// 검색 시작
//		int nNextIndex = pQuest->NextQuestIndex;
//		vector<int>::iterator itSer = m_vecTabMissionNum.begin();
//		while(itSer != m_vecTabMissionNum.end())
//		{
//			if(nNextIndex == *itSer)
//			{
//				m_nMaxNum = nNextIndex;
//				goto QUESTSERCH;
//			}
//			else if(nNextIndex == 0)
//			{
//				pQuest = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
//				if(pQuest)
//				{
//					m_nMaxNum = pQuest->QuestIndex;
//					goto QUESTSERCH;
//				}
//			}
//			else if(nNextIndex == -1)
//			{
//				return NULL;
//			}
//
//			itSer++;
//		}		
//	}
//	
//	return pQuest;
//	// 검색 종료
//	//////////////////////////////////////////////////////////////////////////
//}

CQuest* CINFMissionInfo::SetMissionSorting()
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// 시나리오 미션 다음 시나리오 1개 더보여주기 ( 기타 툴팁설정 )
	// 2005-08-22 by ydkim
	// 제일 하위 미션착기 -검색-
	// ?은 부분으로 GOTO문 사용 후에 수정요망
	CQuest* pQuest = NULL;
	int nNextIndex = 0;
	m_bProgressMission = FALSE;
	if(g_pQuestData->GetProgressMission())
		m_bMissionStartButtonUse = TRUE;

	m_nMaxNum = m_vecQuestIndex[0];							// 제일 초반 미션인덱스.


// 2007-02-23 by dgwoo		
	vector<int>::iterator itSer = m_vecQuestIndex.begin();
	while(itSer != m_vecQuestIndex.end())
	{
		pQuest = g_pQuestData->FindQuest(*itSer);
		if(CheckProgressMission(pQuest))
		{
			nNextIndex = pQuest->NextQuestIndex;
			}
		else
		{
			if(nNextIndex == 0)
			{
				pQuest = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
				if(pQuest)
				{
					m_nMaxNum = 0;
				
				}
			}
			else if(nNextIndex == -1)
			{
				return NULL;
			}
			else
			{
				m_nMaxNum = nNextIndex;
			}
			return pQuest;
		}
			itSer++;
		}		
	
	return pQuest;
	// 검색 종료
	//////////////////////////////////////////////////////////////////////////
}
void CINFMissionInfo::SetMissionInfo()
{
	char strTemp[512];
	ZERO_MEMORY(strTemp);
	CQuest* pQuest = NULL;
	CQuest* pQuestFirstInflu = NULL;
	if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL))
		pQuestFirstInflu = NULL;
	else 
		pQuestFirstInflu = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
	
	int nIndexMax = 0;

//	if(m_vecTabMissionNum.size()>0)
//		nIndexMax = m_nMaxNum;
		nIndexMax = m_nMaxNum;
	if(nIndexMax <= 0)
	{
	}
	else if(m_nMissionViewCase == MISSION_VIEW_ALL && m_nSelectMissionTab == MISS_SCEN)
	{
		// 첫번째 미션처리
		pQuest = g_pQuestData->FindQuest(nIndexMax);
		wsprintf(strTemp, "%s",pQuest->QuestName);
		
		m_vecMissionList.push_back(strTemp);
		m_vecTabMissionNum.push_back(nIndexMax);
		pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);

		if(pQuest)
		{
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
			nIndexMax = 0;
		}		

	}

	if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN && m_nMissionViewCase == MISSION_VIEW_ALL)
	{
		if(g_pQuestData->GetProgressMission() && g_pQuestData->GetProgressMission()->QuestIndex == nIndexMax
			&& pQuestFirstInflu && g_pQuestData->GetProgressMission()->QuestIndex != pQuestFirstInflu->QuestIndex )
		{			
		}
		else
		{
			pQuest = g_pQuestData->FindQuest(nIndexMax);
			if(pQuest)
			{
				int nProM = 0;
				if(g_pQuestData->GetProgressMission())
					nProM = g_pQuestData->GetProgressMission()->QuestIndex;

				if(pQuestFirstInflu && (pQuestFirstInflu->QuestIndex == pQuest->QuestIndex || 
					pQuestFirstInflu->QuestIndex == nProM) && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
					pQuest = g_pQuestData->FindQuest(pQuest->QuestIndex);
				else
					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
				if(pQuest)
				{
					// 진행할수 있는 미션
					if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType)){
						m_vecMissionList.push_back(pQuest->QuestName);
						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
					}
					else{						
						if(pQuestFirstInflu){
							m_vecMissionList.push_back(pQuestFirstInflu->QuestName);
							m_vecTabMissionNum.push_back(pQuestFirstInflu->QuestIndex);
						}						
					}
					
					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
					if(pQuest
						&& COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
					{
						// 다음 미션
						m_vecMissionList.push_back(pQuest->QuestName);
						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
					}
				}
			}
		}
	}
	// 2006-03-28 by ispark, 미 해결 미션탭에 다음에 할 미션 내용 추가
	else if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN && m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
	{
		// 미 해결 미션탭에 들어간 내용중에 마지막 번호를 가져와서 그 다음 미션 정보를 가져온다.
		pQuest = g_pQuestData->FindQuest(nIndexMax);
		if(pQuest && COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
		{
			// 다음 미션
			m_vecMissionList.push_back(pQuest->QuestName);
			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
		}
	}
	// 시나리오 1개더보여주기.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}

//void CINFMissionInfo::SetMissionInfo()
//{
//	char strTemp[512];
//	ZERO_MEMORY(strTemp);
//	CQuest* pQuest = NULL;
//	CQuest* pQuestFirstInflu = NULL;
//	if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, INFLUENCE_TYPE_NORMAL))
//		pQuestFirstInflu = NULL;
//	else 
//		pQuestFirstInflu = g_pQuestData->FindFirstScenarioQuest(g_pShuttleChild->m_myShuttleInfo.InfluenceType);
//	
//	int nIndexMax = 0;
//
//	if(m_vecTabMissionNum.size()>0)
//		nIndexMax = m_nMaxNum;
//	else if(m_nMissionViewCase == MISSION_VIEW_ALL && m_nSelectMissionTab == MISS_SCEN)
//	{
//		// 첫번째 미션처리
//		pQuest = g_pQuestData->FindQuest(m_vecQuestIndex[0]);
//		wsprintf(strTemp, "%s",pQuest->QuestName);
//		
//		m_vecMissionList.push_back(strTemp);
//		m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//		pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//
//		if(pQuest)
//		{
//			m_vecMissionList.push_back(pQuest->QuestName);
//			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//			nIndexMax = 0;
//		}
//
//	}
//
//	if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN)// && m_nMissionViewCase == MISSION_VIEW_ALL)
//	{
//		if(g_pQuestData->GetProgressMission() && g_pQuestData->GetProgressMission()->QuestIndex == nIndexMax
//			&& pQuestFirstInflu && g_pQuestData->GetProgressMission()->QuestIndex != pQuestFirstInflu->QuestIndex )
//		{			
//		}
//		else
//		{
//			pQuest = g_pQuestData->FindQuest(nIndexMax);
//			if(pQuest)
//			{
//				int nProM = 0;
//				if(g_pQuestData->GetProgressMission())
//					nProM = g_pQuestData->GetProgressMission()->QuestIndex;
//
//				if(pQuestFirstInflu && (pQuestFirstInflu->QuestIndex == pQuest->QuestIndex || 
//					pQuestFirstInflu->QuestIndex == nProM) && !g_pQuestData->IsQuestCompleted(pQuest->QuestIndex))
//					pQuest = g_pQuestData->FindQuest(pQuest->QuestIndex);
//				else
//					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//				if(pQuest)
//				{
//					// 진행할수 있는 미션
//					if(COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType)){
//						m_vecMissionList.push_back(pQuest->QuestName);
//						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//					}
//					else{						
//						if(pQuestFirstInflu){
//							m_vecMissionList.push_back(pQuestFirstInflu->QuestName);
//							m_vecTabMissionNum.push_back(pQuestFirstInflu->QuestIndex);
//						}						
//					}
//					
//					pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//					if(pQuest
//						&& COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
//					{
//						// 다음 미션
//						m_vecMissionList.push_back(pQuest->QuestName);
//						m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//					}
//				}
//			}
//		}
//	}
//	// 2006-03-28 by ispark, 미 해결 미션탭에 다음에 할 미션 내용 추가
//	else if(nIndexMax != 0 && m_nSelectMissionTab == MISS_SCEN && m_nMissionViewCase == MISSION_VIEW_UNSOLVE)
//	{
//		// 미 해결 미션탭에 들어간 내용중에 마지막 번호를 가져와서 그 다음 미션 정보를 가져온다.
//		int nindex = m_vecTabMissionNum.size() - 1;
//		pQuest = g_pQuestData->FindQuest(m_vecTabMissionNum[nindex]);
//		pQuest = g_pQuestData->FindQuest(pQuest->NextQuestIndex);
//		if(pQuest && COMPARE_INFLUENCE(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pQuest->ReqInfluenceType))
//		{
//			// 다음 미션
//			m_vecMissionList.push_back(pQuest->QuestName);
//			m_vecTabMissionNum.push_back(pQuest->QuestIndex);
//		}
//	}
//	// 시나리오 1개더보여주기.
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFMissionInfo::SortPayItem(CQuest *pQuest)
/// \brief		미션 지급 아이템 중복 삭제
/// \author		ispark
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMissionInfo::OverlapSortPayItem(CQuest *pQuest)
{
	m_vecCompensationItemName.clear();

	vector<QUEST_PAY_ITEM>::iterator it = pQuest->EndPayItemVector.begin();
	for(; it != pQuest->EndPayItemVector.end(); it++)
	{
		int nCount = 0;
		QUEST_PAY_ITEM* pPayItem = &*it;
		vector<QUEST_PAY_ITEM>::iterator it2 = m_vecCompensationItemName.begin();

// 2006-11-14 by ispark, 아이템 중복을 삭제하고 그대신 지급 유닛종류로 변경
//		for(; it2 != m_vecCompensationItemName.end(); it2++)
//		{
//			QUEST_PAY_ITEM* pPayItem2 = it2;
//			if(pPayItem->ItemNum != 7000022 &&				// SPI는 중복 삭제 안해야 한다.
//				pPayItem->ItemNum != pPayItem2->ItemNum)	// ItemNum이 달라야 한다.
//			{
//				nCount++;
//			}
//		}
//
//		// 똑같은 것이 없다.
//		if(m_vecCompensationItemName.size() == nCount)
		{
			m_vecCompensationItemName.push_back(*pPayItem);
		}
	}
}



