// INFArenaTeamInfo.cpp: implementation of the CINFArenaTeamInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "INFArenaTeamInfo.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3dHanFont.h"
#include "INFGameArena.h"
#include "Chat.h"
#include "INFGameMain.h"

#include "INFImageEx.h"	                // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "Interface.h"
#include "INFToolTip.h"

#include "INFImageBtn.h"				// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
//--------------------------------------------------------------------------//
#define ARENA_TEAMINFO_COUNT					6		// 2012-04-13 by mspark, 아레나 UI 작업 - 기존 7에서 6으로 변경
#define ARENA_TEAMINFO_GAP						20



//#define ARENA_TEAMINFO_BUTTON_X					(g_pD3dApp->GetBackBufferDesc().Width - 326 + m_nInfoButtonX)
//#define ARENA_TEAMINFO_BUTTON_Y					(22 + m_nInfoButtonY)
#define ARENA_TEAMINFO_BUTTON_X					(m_nInfoButtonX)
#define ARENA_TEAMINFO_BUTTON_Y					(m_nInfoButtonY)
#define ARENA_TEAMINFO_BUTTON_W					177
#define ARENA_TEAMINFO_BUTTON_H					25

#define ARENA_TEAMINFO_BUTTON_LIMIT_X			(g_pD3dApp->GetBackBufferDesc().Width - (ARENA_TEAMINFO_BUTTON_W))
#define ARENA_TEAMINFO_BUTTON_LIMIT_Y			(g_pD3dApp->GetBackBufferDesc().Height - (ARENA_TEAMINFO_BUTTON_H + 40))

#define ARENA_TEAMINFO_INFO_START_X				ARENA_TEAMINFO_BUTTON_X
#define ARENA_TEAMINFO_INFO_START_Y				ARENA_TEAMINFO_BUTTON_Y + 25

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
#define ARENA_TEAMINFO_WINDOWSIZE_X				ARENA_TEAMINFO_BUTTON_X + 285
#else
#define ARENA_TEAMINFO_WINDOWSIZE_X				ARENA_TEAMINFO_BUTTON_X + 154
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#define ARENA_TEAMINFO_WINDOWSIZE_Y				ARENA_TEAMINFO_BUTTON_Y + 4
#define ARENA_TEAMINFO_WINDOWSIZE_W				18
#define ARENA_TEAMINFO_WINDOWSIZE_H				17
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define ARENA_TEAMINFO_STATE_FONT_X				ARENA_TEAMINFO_INFO_START_X + 17
#define ARENA_TEAMINFO_STATE_FONT_Y				ARENA_TEAMINFO_INFO_START_Y + 5

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
#define ARENA_TEAMINFO_USER_FONT_X				ARENA_TEAMINFO_INFO_START_X + 32
#define ARENA_TEAMINFO_USER_FONT_Y				ARENA_TEAMINFO_INFO_START_Y + 34

#define ARENA_TEAMINFO_GEAR_CASE_FONT_X			ARENA_TEAMINFO_INFO_START_X + 232
#define ARENA_TEAMINFO_GEAR_CASE_FONT_Y			ARENA_TEAMINFO_INFO_START_Y + 34

#define ARENA_TEAMINFO_USER_READY_X				ARENA_TEAMINFO_INFO_START_X
#define ARENA_TEAMINFO_USER_READY_Y				ARENA_TEAMINFO_INFO_START_Y + 35

#define ARENA_TEAMINFO_GEAR_CASE_X				ARENA_TEAMINFO_INFO_START_X + 94
#define ARENA_TEAMINFO_GEAR_CASE_Y				ARENA_TEAMINFO_INFO_START_Y + 157

#else

#define ARENA_TEAMINFO_USER_FONT_X				ARENA_TEAMINFO_INFO_START_X + 22
#define ARENA_TEAMINFO_USER_FONT_Y				ARENA_TEAMINFO_INFO_START_Y + 34

#define ARENA_TEAMINFO_USER_READY_X				ARENA_TEAMINFO_INFO_START_X + 13
#define ARENA_TEAMINFO_USER_READY_Y				ARENA_TEAMINFO_INFO_START_Y + 35
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

#define ARENA_TEAMINFO_READY_X					ARENA_TEAMINFO_INFO_START_X + 48
#define ARENA_TEAMINFO_READY_Y					ARENA_TEAMINFO_INFO_START_Y + 157
#define ARENA_TEAMINFO_APPCANCEL_X				ARENA_TEAMINFO_INFO_START_X + 97
#define ARENA_TEAMINFO_APPCANCEL_Y				ARENA_TEAMINFO_READY_Y
#define ARENA_TEAMINFO_APPCANCEL_W				19
#define ARENA_TEAMINFO_APPCANCEL_H				19

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
#define ARENA_GEAR_CASE_CREATE_W					423
#define ARENA_GEAR_CASE_CREATE_H					204

#define ARENA_GEAR_CASE_CREATE_X			((g_pD3dApp->GetBackBufferDesc().Width - ARENA_GEAR_CASE_CREATE_W ) /2 )
#define ARENA_GEAR_CASE_CREATE_Y			((g_pD3dApp->GetBackBufferDesc().Height - ARENA_GEAR_CASE_CREATE_H ) /2 )

#define ARENA_GEAR_CASE_IMAGE_X						37
#define ARENA_GEAR_CASE_IMAGE_Y						50
#define ARENA_GEAR_CASE_IMAGE_W						114
#define ARENA_GEAR_CASE_IMAGE_H						74
#define ARENA_GEAR_CASE_IMAGE_TAB					3

#define ARENA_GEAR_CASE_CREATE_BUTTON_X							158
#define ARENA_GEAR_CASE_CREATE_BUTTON_Y							150

#define ARENA_GEAR_CASE_CANCEL_BUTTON_X							ARENA_GEAR_CASE_CREATE_BUTTON_X
#define ARENA_GEAR_CASE_CANCEL_BUTTON_Y							ARENA_GEAR_CASE_CREATE_BUTTON_Y

#define ARENA_GEAR_ATTACK_CASE_NUMBER				1
#define ARENA_GEAR_DEFENCE_CASE_NUMBER				2
#define ARENA_GEAR_EVASION_CASE_NUMBER				3

#define ARENA_GEAR_ATTACK_CASE_INDEX				0
#define ARENA_GEAR_DEFENCE_CASE_INDEX				1
#define ARENA_GEAR_EVASION_CASE_INDEX				2
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

#else
#define ARENA_TEAMINFO_STATE_FONT_X				ARENA_TEAMINFO_INFO_START_X + 17
#define ARENA_TEAMINFO_STATE_FONT_Y				ARENA_TEAMINFO_INFO_START_Y + 20

#define ARENA_TEAMINFO_USER_FONT_X				ARENA_TEAMINFO_INFO_START_X + 22
#define ARENA_TEAMINFO_USER_FONT_Y				ARENA_TEAMINFO_INFO_START_Y + 50

#define ARENA_TEAMINFO_USER_READY_X				ARENA_TEAMINFO_INFO_START_X + 13
#define ARENA_TEAMINFO_USER_READY_Y				ARENA_TEAMINFO_INFO_START_Y + 50

#define ARENA_TEAMINFO_READY_X					ARENA_TEAMINFO_INFO_START_X + 23
#define ARENA_TEAMINFO_READY_Y					ARENA_TEAMINFO_INFO_START_Y + 212
#define ARENA_TEAMINFO_APPCANCEL_X				ARENA_TEAMINFO_INFO_START_X + 90
#define ARENA_TEAMINFO_APPCANCEL_Y				ARENA_TEAMINFO_READY_Y
#define ARENA_TEAMINFO_APPCANCEL_W				64
#define ARENA_TEAMINFO_APPCANCEL_H				19


#endif

//							Scroll
//--------------------------------------------------------------------------//
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_X		(ARENA_TEAMINFO_INFO_START_X + 13)
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_Y		(ARENA_TEAMINFO_INFO_START_Y + 28)
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_W		(ARENA_TEAMINFO_SCROLL_WHELL_AREA_X + 156)
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_H		(ARENA_TEAMINFO_SCROLL_WHELL_AREA_Y + 140)

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_X		(ARENA_TEAMINFO_INFO_START_X + 286)
#else
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_X		(ARENA_TEAMINFO_INFO_START_X + 156)
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#endif
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y		(ARENA_TEAMINFO_INFO_START_Y + 28)
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_W		(ARENA_TEAMINFO_SCROLL_DRAG_AREA_X + 20)
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_H		(ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y + 101)
#else
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_X		(ARENA_TEAMINFO_INFO_START_X + 13)
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_Y		(ARENA_TEAMINFO_INFO_START_Y + 50)
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_W		(ARENA_TEAMINFO_SCROLL_WHELL_AREA_X + 156)
#define	ARENA_TEAMINFO_SCROLL_WHELL_AREA_H		(ARENA_TEAMINFO_SCROLL_WHELL_AREA_Y + 140)

#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_X		(ARENA_TEAMINFO_INFO_START_X + 157)
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y		(ARENA_TEAMINFO_INFO_START_Y + 50)
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_W		(ARENA_TEAMINFO_SCROLL_DRAG_AREA_X + 20)
#define	ARENA_TEAMINFO_SCROLL_DRAG_AREA_H		(ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y + 101)

#endif

							  
// 2007-05-15 by bhsohn 스크롤 관련 버그 수정
#define	ARENA_TEAMINFO_SCROLL_DRAG_BALL_CAP		55
//--------------------------------------------------------------------------//



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINFArenaTeamInfo::CINFArenaTeamInfo(CAtumNode* pParent)
{
	m_pParent = pParent;
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		m_pImgTeamInfoB[i][0] = NULL;
		m_pImgTeamInfoB[i][1] = NULL;
	}
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgReadyB[i] = NULL;
		m_pImgUReadyB[i] = NULL;
		m_pImgAppCancelB[i] = NULL;
		m_pImgWinSizeB[0][i] = NULL;
		m_pImgWinSizeB[1][i] = NULL;
	}
	m_pFontID = NULL;
	m_pFontState = NULL;

	m_pScroll = NULL;
	m_pImgBG = NULL;
	m_pImgReady = NULL;

	m_nReadyB = BUTTON_STATE_NORMAL;
	m_nUReadyB = BUTTON_STATE_NORMAL;
	m_nAppCancel = BUTTON_STATE_NORMAL;

	m_bTeamInfoShow = TRUE;

	m_nLimitPerson = 0;
	m_nPerson = 0;

	m_nInfoButtonX = g_pD3dApp->GetBackBufferDesc().Width - 326;
	m_nInfoButtonY = 22;
	
	m_bInfoButton = FALSE;

	// 2007-11-22 by bhsohn 아레나 통합서버
	m_fUReadyButtonTime = ARENA_BUTTON_UREADY_GAP;

	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	m_pArenaInfoButtonImage = NULL;

	m_nReadyButtonImgMinPos.x = 0;
	m_nReadyButtonImgMinPos.y = 0;
	m_nAppCancleButtonImgMinPos.x = 0;
	m_nAppCancleButtonImgMinPos.y = 0;
	m_nArenaInfoButtonImgMaxPos.x = 0;
	m_nArenaInfoButtonImgMaxPos.y = 0;

	m_pImgGearCaseCreate = NULL;
	m_pImgGearCaseSelect = NULL;

	m_nArenaGearCaseNum = 0;
	m_nArenaGearCaseImgTabNum = 0;
	m_nArenaGearCaseImgListNum = 0;

	m_nStateCancelB = BUTTON_STATE_NORMAL;
	m_nStateCreateB = BUTTON_STATE_NORMAL;

	m_pOkCancelImage = NULL;

	m_nOkImgMinPos.x = 0;
	m_nOkImgMinPos.y = 0;
	m_nCancelImgMinPos.x = 0;
	m_nCancelImgMinPos.y = 0;
	m_nOkCancelBaseImgMaxPos.x = 0;
	m_nOkCancelBaseImgMaxPos.y = 0;

	m_nStateGearCaseChangeB = BUTTON_STATE_NORMAL;

	m_pFontGearCase = NULL;

	m_bShow = FALSE;
	// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

}
CINFArenaTeamInfo::CINFArenaTeamInfo()
{
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		m_pImgTeamInfoB[i][0] = NULL;
		m_pImgTeamInfoB[i][1] = NULL;
	}
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgReadyB[i] = NULL;
		m_pImgUReadyB[i] = NULL;
		m_pImgAppCancelB[i] = NULL;
		m_pImgWinSizeB[0][i] = NULL;
		m_pImgWinSizeB[1][i] = NULL;

	}
	m_pFontID = NULL;
	m_pFontState = NULL;

	m_pScroll = NULL;
	m_pImgBG = NULL;
	m_pImgReady = NULL;

	m_nReadyB = BUTTON_STATE_NORMAL;
	m_nUReadyB = BUTTON_STATE_NORMAL;
	m_nAppCancel = BUTTON_STATE_NORMAL;

	m_bTeamInfoShow = FALSE;

	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	m_nStateCancelB = BUTTON_STATE_NORMAL;
	m_nStateCreateB = BUTTON_STATE_NORMAL;

	m_nStateGearCaseChangeB = BUTTON_STATE_NORMAL;

	m_pFontGearCase = NULL;

	m_bShow = FALSE;
	// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
}

CINFArenaTeamInfo::~CINFArenaTeamInfo()
{

}
VOID CINFArenaTeamInfo::AddTeamUserInfo(MSG_FC_ARENA_TEAM_MEMBER_LIST *pMsg)
{
	SARENA_TEAM_CHARACTERINFO *pTeamInfo = (SARENA_TEAM_CHARACTERINFO *)((char *)pMsg + sizeof(MSG_FC_ARENA_TEAM_MEMBER_LIST));
	int i = 0;
	m_nPerson = 0;
	m_vecTeamUserInfo.clear();
	for(i = 0 ; i < pMsg->ArenaTeamCharacterNameListCount ; i++)
	{
		// 2007-11-22 by bhsohn 아레나 통합서버
		CHARACTER*	pArenaCharacterInfo = g_pD3dApp->GetArenaCharacterInfo();
		//if(pTeamInfo->ClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
		if(pTeamInfo->ClientIndex == pArenaCharacterInfo->ClientIndex)		
		{
			if(pTeamInfo->ArenaState == ARENA_STATE_READY_FINISH)
			{
				m_bReady = TRUE;
			}
			if(pTeamInfo->ArenaState == ARENA_STATE_READY)
			{
				m_fUReadyButtonTime = ARENA_BUTTON_UREADY_GAP;
				m_bReady = FALSE;
			}
		}
		// 2007-11-22 by bhsohn 아레나 통합서버
		// 팀인포		
		STRUCT_TEAM_CHARACTERINFO sTeamInfo;
		memset(&sTeamInfo, 0x00, sizeof(STRUCT_TEAM_CHARACTERINFO));
		{
			sTeamInfo.ArenaState = pTeamInfo->ArenaState;
			sTeamInfo.ClientIndex = pTeamInfo->ClientIndex;
			strncpy(sTeamInfo.CharacterName, pTeamInfo->CharacterName, SIZE_MAX_CHARACTER_NAME);
// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
			sTeamInfo.PlayCharacterType = pTeamInfo->PlayCharacterType;
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
		}
		// 이름변경 
		g_pD3dApp->ConevertArenaRenderUserName(ARENA_STATE_ARENA_GAMING, sTeamInfo.CharacterName);
		//m_vecTeamUserInfo.push_back((*pTeamInfo));
		m_vecTeamUserInfo.push_back(sTeamInfo);

		pTeamInfo++;
	}
	m_nPerson = i;
	m_pScroll->SetMaxItem(i);
	
	// 현재 상태를 갱신시켜준다.
	if(pMsg->ArenaTeamState-1 >= ARENA_BUTTON_STATE_USERIN &&		// 0
		pMsg->ArenaTeamState-1 <= ARENA_BUTTON_STATE_ENEMYSEARCH)	// 2
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		SetState(pMsg->ArenaTeamState );
#else		
		SetState(pMsg->ArenaTeamState - 1);
#endif
		
	}
}
VOID CINFArenaTeamInfo::SetState(BYTE state)
{
	m_nMainBState = state;
}
HRESULT CINFArenaTeamInfo::InitDeviceObjects()
{
	int i;
	DataHeader	* pDataHeader;
	char buf[30];	
	if(NULL == m_pScroll)
	{
		m_pScroll = new CINFArenaScrollBar;
	}
	wsprintf(buf,"c_scrlb");				
	m_pScroll->InitDeviceObjects(ARENA_TEAMINFO_COUNT, buf);

	for(i = 0 ; i < 3; i++)
	{
		wsprintf( buf, "ainfob%d0", i);
		pDataHeader = FindResource(buf);
		m_pImgTeamInfoB[i][0] = new CINFImageEx;
		m_pImgTeamInfoB[i][0]->InitDeviceObjects( pDataHeader );

		wsprintf( buf, "ainfob%d1", i);
		pDataHeader = FindResource(buf);
		m_pImgTeamInfoB[i][1] = new CINFImageEx;
		m_pImgTeamInfoB[i][1]->InitDeviceObjects( pDataHeader );
	}
	for(i = 0; i < BUTTON_STATE_NUMBER ; i++)
	{
		wsprintf(buf,"startare%d",i);
		pDataHeader = FindResource(buf);
		m_pImgReadyB[i] = new CINFImageEx;
		m_pImgReadyB[i]->InitDeviceObjects( pDataHeader );

		wsprintf(buf,"ustaare%d",i);
		pDataHeader = FindResource(buf);
		m_pImgUReadyB[i] = new CINFImageEx;
		m_pImgUReadyB[i]->InitDeviceObjects( pDataHeader );
		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(buf,"outs0%d",i);
#else		
		wsprintf(buf,"appcencel%d",i);
#endif

		pDataHeader = FindResource(buf);
		m_pImgAppCancelB[i] = new CINFImageEx;
		m_pImgAppCancelB[i]->InitDeviceObjects( pDataHeader );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경		
		pDataHeader = FindResource("w_wMax");
#else	
		wsprintf(buf,"ainfo0%d",i);
		pDataHeader = FindResource(buf);
#endif
		m_pImgWinSizeB[0][i] = new CINFImageEx;
		m_pImgWinSizeB[0][i]->InitDeviceObjects( pDataHeader );


#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		pDataHeader = FindResource("w_wMin");
#else	
		wsprintf(buf,"ainfo1%d",i);
		pDataHeader = FindResource(buf);
		
#endif
		m_pImgWinSizeB[1][i] = new CINFImageEx;
		m_pImgWinSizeB[1][i]->InitDeviceObjects( pDataHeader );

	}

	m_pFontID = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32);	//2013-05-29 by ssjung 아레나에 다른 팀을 기달리는 스트링이 짤리는 현상 수정 
	m_pFontID->InitDeviceObjects(g_pD3dDev);
	m_pFontState = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,256,32); //2013-05-29 by ssjung 아레나에 다른 팀을 기달리는 스트링이 짤리는 현상 수정 
	m_pFontState->InitDeviceObjects(g_pD3dDev);
	

	pDataHeader = FindResource("areinfo");
	m_pImgBG = new CINFImageEx;
	m_pImgBG->InitDeviceObjects( pDataHeader );

	pDataHeader = FindResource("areready");
	m_pImgReady = new CINFImageEx;
	m_pImgReady->InitDeviceObjects( pDataHeader );
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    {
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "are_info" );
		m_pRenewArenaTeamInfoBackImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pRenewArenaTeamInfoBackImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pRenewArenaTeamInfoBackImage->RestoreDeviceObjects();		
	}
#endif
	
	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "are_btn2" );
		m_pArenaInfoButtonImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pArenaInfoButtonImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pArenaInfoButtonImage->RestoreDeviceObjects();		
	}

	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		wsprintf(buf,"Gtype_btn%d",i);
		
		pDataHeader = FindResource(buf);
		m_pImgGearCaseChangeB[i] = new CINFImageEx;
		m_pImgGearCaseChangeB[i]->InitDeviceObjects( pDataHeader );
	}	

	m_nGearCaseChangeImgMinPos.x = m_pArenaInfoButtonImage->GetFindControlTargetofMinPos("Gtype_btn0").x;
	m_nGearCaseChangeImgMinPos.y = m_pArenaInfoButtonImage->GetFindControlTargetofMinPos("Gtype_btn0").y;
	m_nReadyButtonImgMinPos.x = m_pArenaInfoButtonImage->GetFindControlTargetofMinPos("wpmove00").x;
	m_nReadyButtonImgMinPos.y = m_pArenaInfoButtonImage->GetFindControlTargetofMinPos("wpmove00").y;
	m_nAppCancleButtonImgMinPos.x = m_pArenaInfoButtonImage->GetFindControlTargetofMinPos("outs00").x;
	m_nAppCancleButtonImgMinPos.y = m_pArenaInfoButtonImage->GetFindControlTargetofMinPos("outs00").y;
	m_nArenaInfoButtonImgMaxPos.x = m_pArenaInfoButtonImage->GetFindControlTargetofMaxPos("Gtype_btn0").x;
	m_nArenaInfoButtonImgMaxPos.y = m_pArenaInfoButtonImage->GetFindControlTargetofMaxPos("Gtype_btn0").y;

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "Gtype" );
		m_pImgGearCaseCreate = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pImgGearCaseCreate->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pImgGearCaseCreate->RestoreDeviceObjects();		
	}
	
	pDataHeader = FindResource("Gtypesel");
	m_pImgGearCaseSelect = new CINFImageEx;
	m_pImgGearCaseSelect->InitDeviceObjects( pDataHeader );

	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		wsprintf(buf,"okb0%d",i);
		
		pDataHeader = FindResource(buf);
		m_pImgCreateB[i] = new CINFImageEx;
		m_pImgCreateB[i]->InitDeviceObjects( pDataHeader );
		
		wsprintf(buf,"canb0%d",i);

		pDataHeader = FindResource(buf);
		m_pImgCancelB[i] = new CINFImageEx;
		m_pImgCancelB[i]->InitDeviceObjects( pDataHeader );			
	}

	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource( "ok_cancel" );
		m_pOkCancelImage = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pOkCancelImage->InitDeviceObjects( g_pD3dApp->m_pImageList );
		m_pOkCancelImage->RestoreDeviceObjects();		
	}

	m_nOkImgMinPos.x = m_pOkCancelImage->GetFindControlTargetofMinPos("okb00").x;
	m_nOkImgMinPos.y = m_pOkCancelImage->GetFindControlTargetofMinPos("okb00").y;
	m_nCancelImgMinPos.x = m_pOkCancelImage->GetFindControlTargetofMinPos("canb00").x;
	m_nCancelImgMinPos.y = m_pOkCancelImage->GetFindControlTargetofMinPos("canb00").y;
	m_nOkCancelBaseImgMaxPos.x = m_pOkCancelImage->GetFindControlTargetofMaxPos("okb00").x;
	m_nOkCancelBaseImgMaxPos.y = m_pOkCancelImage->GetFindControlTargetofMaxPos("okb00").y;	

	m_pFontGearCase = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,128,32);
	m_pFontGearCase->InitDeviceObjects(g_pD3dDev);
	// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	

	
	return S_OK;
}
HRESULT CINFArenaTeamInfo::RestoreDeviceObjects()
{
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		m_pImgTeamInfoB[i][0]->RestoreDeviceObjects();
		m_pImgTeamInfoB[i][1]->RestoreDeviceObjects();
	}
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgReadyB[i]->RestoreDeviceObjects();
		m_pImgUReadyB[i]->RestoreDeviceObjects();
		m_pImgAppCancelB[i]->RestoreDeviceObjects();
		m_pImgWinSizeB[0][i]->RestoreDeviceObjects();
		m_pImgWinSizeB[1][i]->RestoreDeviceObjects();
	}

	m_pFontID->RestoreDeviceObjects();
	m_pFontState->RestoreDeviceObjects();

	m_pImgBG->RestoreDeviceObjects();
	m_pImgReady->RestoreDeviceObjects();
	
	SetMouseRect();

	m_pScroll->RestoreDeviceObjects();	
							  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pRenewArenaTeamInfoBackImage->RestoreDeviceObjects();		
#endif
    
	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	m_pArenaInfoButtonImage->RestoreDeviceObjects();

	m_pImgGearCaseCreate->RestoreDeviceObjects();
	m_pImgGearCaseSelect->RestoreDeviceObjects();

	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgCreateB[i]->RestoreDeviceObjects();
		m_pImgCancelB[i]->RestoreDeviceObjects();
	}

	m_pOkCancelImage->RestoreDeviceObjects();

	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgGearCaseChangeB[i]->RestoreDeviceObjects();
	}

	m_pFontGearCase->RestoreDeviceObjects();
	// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
    
	return S_OK;

}
VOID CINFArenaTeamInfo::SetMouseRect()
{
	RECT rc;
	// 휠마우스 위치 영역.
	rc.left		= ARENA_TEAMINFO_SCROLL_WHELL_AREA_X;
	rc.top		= ARENA_TEAMINFO_SCROLL_WHELL_AREA_Y;
	rc.bottom	= ARENA_TEAMINFO_SCROLL_WHELL_AREA_H;
	rc.right	= ARENA_TEAMINFO_SCROLL_WHELL_AREA_W;
	m_pScroll->SetMouseWhellRect(rc);

	// 마우스 클릭 후 드레그 가능 영역.
	rc.left		= ARENA_TEAMINFO_SCROLL_DRAG_AREA_X;
	rc.top		= ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y; 
    // 2007-05-15 by bhsohn 스크롤 관련 버그 수정
	rc.bottom	= ARENA_TEAMINFO_SCROLL_DRAG_AREA_H+ARENA_TEAMINFO_SCROLL_DRAG_BALL_CAP;
	rc.right	= ARENA_TEAMINFO_SCROLL_DRAG_AREA_W;
	m_pScroll->SetMouseBallRect(rc);

	// 스크롤 인식 범위.
	m_pScroll->SetPosition(ARENA_TEAMINFO_SCROLL_DRAG_AREA_X, ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y, 
		(ARENA_TEAMINFO_SCROLL_DRAG_AREA_W)-(ARENA_TEAMINFO_SCROLL_DRAG_AREA_X), 
		(ARENA_TEAMINFO_SCROLL_DRAG_AREA_H)-(ARENA_TEAMINFO_SCROLL_DRAG_AREA_Y));
}

void CINFArenaTeamInfo::TestDB()
{
	int nItemLen = 10;
	long lBuffLen = sizeof(MSG_FC_ARENA_TEAM_MEMBER_LIST)+(sizeof(SARENA_TEAM_CHARACTERINFO)*nItemLen);
	MSG_FC_ARENA_TEAM_MEMBER_LIST *pMsg = (MSG_FC_ARENA_TEAM_MEMBER_LIST *)malloc(lBuffLen);
	memset(pMsg, 0x00, lBuffLen);

	SARENA_TEAM_CHARACTERINFO *pTeamInfo = (SARENA_TEAM_CHARACTERINFO *)((char *)pMsg + sizeof(MSG_FC_ARENA_TEAM_MEMBER_LIST));

	pMsg->ArenaTeamCharacterNameListCount = nItemLen;
	pMsg->ArenaTeamState = ARENA_BUTTON_STATE_USERIN;

	for(int nCnt = 0;nCnt < nItemLen;nCnt++)
	{
		pTeamInfo->ArenaState = ARENA_STATE_READY_FINISH;
		pTeamInfo->ClientIndex = 100;
		wsprintf(pTeamInfo->CharacterName, "Team%d", nCnt);
		pTeamInfo++;
	}	
	AddTeamUserInfo(pMsg);
	free(pMsg);

}
HRESULT CINFArenaTeamInfo::DeleteDeviceObjects()
{
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		m_pImgTeamInfoB[i][0]->DeleteDeviceObjects();
		m_pImgTeamInfoB[i][1]->DeleteDeviceObjects();
		util::del(m_pImgTeamInfoB[i][0]);
		util::del(m_pImgTeamInfoB[i][1]);
	}
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgReadyB[i]->DeleteDeviceObjects();
		m_pImgUReadyB[i]->DeleteDeviceObjects();
		m_pImgAppCancelB[i]->DeleteDeviceObjects();
		m_pImgWinSizeB[0][i]->DeleteDeviceObjects();
		m_pImgWinSizeB[1][i]->DeleteDeviceObjects();

		util::del(m_pImgReadyB[i]);
		util::del(m_pImgUReadyB[i]);
		util::del(m_pImgAppCancelB[i]);
		util::del(m_pImgWinSizeB[0][i]);
		util::del(m_pImgWinSizeB[1][i]);
	}
	m_pFontID->DeleteDeviceObjects();
	m_pFontState->DeleteDeviceObjects();
	util::del(m_pFontID);
	util::del(m_pFontState);

	m_pScroll->DeleteDeviceObjects();
	m_pImgBG->DeleteDeviceObjects();
	m_pImgReady->DeleteDeviceObjects();
	util::del(m_pScroll);
	util::del(m_pImgBG);
	util::del(m_pImgReady);
					 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pRenewArenaTeamInfoBackImage->DeleteDeviceObjects();						 
    util::del(m_pRenewArenaTeamInfoBackImage);
#endif

	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	m_pArenaInfoButtonImage->DeleteDeviceObjects();
	util::del(m_pArenaInfoButtonImage);

	m_pImgGearCaseCreate->DeleteDeviceObjects();
	m_pImgGearCaseSelect->DeleteDeviceObjects();	

	util::del(m_pImgGearCaseCreate);
	util::del(m_pImgGearCaseSelect);

	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgCreateB[i]->DeleteDeviceObjects();
		m_pImgCancelB[i]->DeleteDeviceObjects();		
		util::del(m_pImgCreateB[i]);
		util::del(m_pImgCancelB[i]);
	}

	m_pOkCancelImage->DeleteDeviceObjects();
	util::del(m_pOkCancelImage);

	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgGearCaseChangeB[i]->DeleteDeviceObjects();
		util::del(m_pImgGearCaseChangeB[i]);
	}

	m_pFontGearCase->DeleteDeviceObjects();
	util::del(m_pFontGearCase);
	// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업


	return S_OK;

}
HRESULT CINFArenaTeamInfo::InvalidateDeviceObjects()
{
	int i;
	for(i = 0 ; i < 3 ; i++)
	{
		m_pImgTeamInfoB[i][0]->InvalidateDeviceObjects();
		m_pImgTeamInfoB[i][1]->InvalidateDeviceObjects();
	}
	for(i = 0 ; i < BUTTON_STATE_NUMBER ; i++)
	{
		m_pImgReadyB[i]->InvalidateDeviceObjects();
		m_pImgUReadyB[i]->InvalidateDeviceObjects();
		m_pImgAppCancelB[i]->InvalidateDeviceObjects();
		m_pImgWinSizeB[0][i]->InvalidateDeviceObjects();
		m_pImgWinSizeB[1][i]->InvalidateDeviceObjects();
		

	}
	m_pFontID->InvalidateDeviceObjects();
	m_pFontState->InvalidateDeviceObjects();

	m_pScroll->InvalidateDeviceObjects();
	m_pImgBG->InvalidateDeviceObjects();
	m_pImgReady->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
    m_pRenewArenaTeamInfoBackImage->InvalidateDeviceObjects();	
#endif
    
	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	m_pArenaInfoButtonImage->InvalidateDeviceObjects();

	m_pImgGearCaseCreate->InvalidateDeviceObjects();
	m_pImgGearCaseSelect->InvalidateDeviceObjects();

	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgCreateB[i]->InvalidateDeviceObjects();
		m_pImgCancelB[i]->InvalidateDeviceObjects();
	}

	m_pOkCancelImage->InvalidateDeviceObjects();

	for(i = 0; i < BUTTON_STATE_NUMBER; i++)
	{
		m_pImgGearCaseChangeB[i]->InvalidateDeviceObjects();
	}

	m_pFontGearCase->InvalidateDeviceObjects();
	// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
    
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFArenaTeamInfo::MoveInfoButton(POINT pt)
/// \brief		버튼을 이동할 경우.
/// \author		dgwoo
/// \date		2007-06-01 ~ 2007-06-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFArenaTeamInfo::MoveInfoButton(POINT pt)
{
	m_nInfoButtonX += pt.x - m_ptOld.x;
	m_nInfoButtonY += pt.y - m_ptOld.y;
	if(m_nInfoButtonX < 0)
		m_nInfoButtonX = 0;
	if(m_nInfoButtonX > ARENA_TEAMINFO_BUTTON_LIMIT_X)
		m_nInfoButtonX = ARENA_TEAMINFO_BUTTON_LIMIT_X;
	if(m_nInfoButtonY < 0)
		m_nInfoButtonY = 0;
	if(m_nInfoButtonY > ARENA_TEAMINFO_BUTTON_LIMIT_Y)
		m_nInfoButtonY = ARENA_TEAMINFO_BUTTON_LIMIT_Y;
	m_ptOld = pt;
	SetMouseRect();
	//DbgOut("MoveInfoButton X = %d, Y = %d\n",m_nInfoButtonX, m_nInfoButtonY);
}
void CINFArenaTeamInfo::Render()
{
	int i;
	



	

	if(m_bTeamInfoShow)
	{// 팀정보를 보여준다.(현재 팀state, 유저명, ready상태등)
		// 배경.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
        m_pRenewArenaTeamInfoBackImage->Move(ARENA_TEAMINFO_BUTTON_X,ARENA_TEAMINFO_BUTTON_Y);
		m_pRenewArenaTeamInfoBackImage->Render();
#else			   
    m_pImgTeamInfoB[m_nMainBState][m_nMainBUnder]->Move(ARENA_TEAMINFO_BUTTON_X,ARENA_TEAMINFO_BUTTON_Y);
	m_pImgTeamInfoB[m_nMainBState][m_nMainBUnder]->Render();

	m_pImgWinSizeB[m_bTeamInfoShow][m_nWinSizeB]->Move(ARENA_TEAMINFO_WINDOWSIZE_X,ARENA_TEAMINFO_WINDOWSIZE_Y);
	m_pImgWinSizeB[m_bTeamInfoShow][m_nWinSizeB]->Render();
		m_pImgBG->Move(ARENA_TEAMINFO_INFO_START_X,ARENA_TEAMINFO_INFO_START_Y);
		m_pImgBG->Render();
#endif
		

	
		
		// team state
		char buf[64];
		switch(m_nMainBState)
		{
			case ARENA_BUTTON_STATE_USERIN:
				{
					wsprintf(buf,STRMSG_C_070502_0100,m_nPerson,m_nLimitPerson);
				}
				break;
			case ARENA_BUTTON_STATE_READY:
				{
					wsprintf(buf,STRMSG_C_070502_0101,m_nPerson,m_nLimitPerson);
				}
				break;
			case ARENA_BUTTON_STATE_ENEMYSEARCH:
				{
					wsprintf(buf,STRMSG_C_070502_0102,m_nPerson,m_nLimitPerson);
				}
				break;
		}
		// 2007-05-28 by dgwoo 세력전이 진행중일 경우는 모든 진행을 멈춘다.(모선전, 거점전)
		if(g_pD3dApp->IsWarMotherShip())
		{
			memset(buf,0x00,64);
			wsprintf(buf,STRMSG_C_070502_0103,m_nPerson,m_nLimitPerson);
		}
		m_pFontState->DrawText(ARENA_TEAMINFO_STATE_FONT_X ,
			ARENA_TEAMINFO_STATE_FONT_Y,
			GUI_FONT_COLOR_W,
			buf);

		// User Name and State
		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
		//vector<SARENA_TEAM_CHARACTERINFO>::iterator it = m_vecTeamUserInfo.begin();
		vector<STRUCT_TEAM_CHARACTERINFO>::iterator it = m_vecTeamUserInfo.begin();
		
		int nCnt = 0;
		for(nCnt = 0;nCnt < m_pScroll->GetScrollStep(); nCnt++)
		{
			if(it == m_vecTeamUserInfo.end())
			{
				break;
			}
			// 스크롤 스텝
			it++;
		}
		for(i = 0; i < ARENA_TEAMINFO_COUNT; i++)
		{
			if(it == m_vecTeamUserInfo.end())
			{
				break;
			}
			wsprintf(buf,"%s",it->CharacterName);
			m_pFontID->DrawText(ARENA_TEAMINFO_USER_FONT_X,
				ARENA_TEAMINFO_USER_FONT_Y + (i * ARENA_TEAMINFO_GAP),
				GUI_FONT_COLOR_W,
				buf);

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
			if(it->PlayCharacterType == ARENA_GEAR_ATTACK_CASE_NUMBER)
			{
				wsprintf(buf,"%s",STRMSG_C_120604_0001);
			}
			else if(it->PlayCharacterType == ARENA_GEAR_DEFENCE_CASE_NUMBER)
			{
				wsprintf(buf,"%s",STRMSG_C_120604_0002);
			}
			else if(it->PlayCharacterType == ARENA_GEAR_EVASION_CASE_NUMBER)
			{
				wsprintf(buf,"%s",STRMSG_C_120604_0003);
			}
			SIZE size;
			size = m_pFontGearCase->GetStringSize(buf);
			m_pFontGearCase->DrawText(ARENA_TEAMINFO_GEAR_CASE_FONT_X - (INT)(size.cx/2),
				ARENA_TEAMINFO_USER_FONT_Y + (i * ARENA_TEAMINFO_GAP),
				GUI_FONT_COLOR_W,
				buf);	
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

			if(it->ArenaState == ARENA_STATE_READY_FINISH)
			{// 준비 버튼을 눌렀을경우.
				m_pImgReady->Move(ARENA_TEAMINFO_USER_READY_X,
					ARENA_TEAMINFO_USER_READY_Y + (i * ARENA_TEAMINFO_GAP));
				m_pImgReady->Render();
			}
			it++;
		}
		// Button
// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
		m_pImgGearCaseChangeB[m_nStateGearCaseChangeB]->Move(ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x, ARENA_TEAMINFO_READY_Y + m_nGearCaseChangeImgMinPos.y);
		m_pImgGearCaseChangeB[m_nStateGearCaseChangeB]->Render();

		if(m_bReady)
		{// 준비 해제 버튼을 보여줌.
			m_pImgUReadyB[m_nReadyB]->Move(ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x, ARENA_TEAMINFO_READY_Y + m_nReadyButtonImgMinPos.y);
			m_pImgUReadyB[m_nReadyB]->Render();			
		}
		else
		{//	준비 버튼을 보여줌.
			m_pImgReadyB[m_nUReadyB]->Move(ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x, ARENA_TEAMINFO_READY_Y + m_nReadyButtonImgMinPos.y);
			m_pImgReadyB[m_nUReadyB]->Render();
		}
		m_pImgAppCancelB[m_nAppCancel]->Move(ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x, ARENA_TEAMINFO_READY_Y + m_nAppCancleButtonImgMinPos.y);
		m_pImgAppCancelB[m_nAppCancel]->Render();

#else

		if(m_bReady)
		{// 준비 해제 버튼을 보여줌.
			m_pImgUReadyB[m_nReadyB]->Move(ARENA_TEAMINFO_READY_X,ARENA_TEAMINFO_READY_Y);
			m_pImgUReadyB[m_nReadyB]->Render();
				  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
m_pImgTeamInfoB[m_nMainBState][m_nMainBUnder]->Move(ARENA_TEAMINFO_BUTTON_X+1,ARENA_TEAMINFO_BUTTON_Y+2);
			m_pImgTeamInfoB[m_nMainBState][m_nMainBUnder]->Render();
#endif
			
		}
		else
		{//	준비 버튼을 보여줌.
			m_pImgReadyB[m_nUReadyB]->Move(ARENA_TEAMINFO_READY_X,ARENA_TEAMINFO_READY_Y);
			m_pImgReadyB[m_nUReadyB]->Render();
		}
		m_pImgAppCancelB[m_nAppCancel]->Move(ARENA_TEAMINFO_APPCANCEL_X,ARENA_TEAMINFO_APPCANCEL_Y);
		m_pImgAppCancelB[m_nAppCancel]->Render();
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
		
		// Scroll
		m_pScroll->Render();

	}  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
else
	{
		m_pImgTeamInfoB[0][1]->Move(ARENA_TEAMINFO_BUTTON_X,ARENA_TEAMINFO_BUTTON_Y+1);
		m_pImgTeamInfoB[0][1]->Render();
	}
	
	
	m_pImgWinSizeB[m_bTeamInfoShow][m_nWinSizeB]->Move(ARENA_TEAMINFO_WINDOWSIZE_X,ARENA_TEAMINFO_WINDOWSIZE_Y);
	m_pImgWinSizeB[m_bTeamInfoShow][m_nWinSizeB]->Render();
#endif
	
// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
	if(m_bShow)
	{
		m_pImgGearCaseCreate->Move(ARENA_GEAR_CASE_CREATE_X, ARENA_GEAR_CASE_CREATE_Y);
		m_pImgGearCaseCreate->Render();
		
		m_pImgGearCaseSelect->Move(ARENA_GEAR_CASE_CREATE_X + ARENA_GEAR_CASE_IMAGE_X + ( (ARENA_GEAR_CASE_IMAGE_W * m_nArenaGearCaseImgTabNum) + ARENA_GEAR_CASE_IMAGE_TAB * m_nArenaGearCaseImgTabNum ), ARENA_GEAR_CASE_CREATE_Y + ARENA_GEAR_CASE_IMAGE_Y);
		m_pImgGearCaseSelect->Render();
		
		m_pImgCreateB[m_nStateCreateB]->Move(ARENA_GEAR_CASE_CREATE_X + ARENA_GEAR_CASE_CREATE_BUTTON_X + m_nOkImgMinPos.x, ARENA_GEAR_CASE_CREATE_Y + ARENA_GEAR_CASE_CREATE_BUTTON_Y + m_nOkImgMinPos.y);
		m_pImgCreateB[m_nStateCreateB]->Render();
		
		m_pImgCancelB[m_nStateCancelB]->Move(ARENA_GEAR_CASE_CREATE_X + ARENA_GEAR_CASE_CANCEL_BUTTON_X + m_nCancelImgMinPos.x, ARENA_GEAR_CASE_CREATE_Y + ARENA_GEAR_CASE_CANCEL_BUTTON_Y + m_nCancelImgMinPos.y);
		m_pImgCancelB[m_nStateCancelB]->Render();
	}	
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
	
}
void CINFArenaTeamInfo::Tick()
{
	FLOAT fElapsedTime = g_pD3dApp->GetCheckElapsedTime();
	m_fArenaButtonGap += fElapsedTime;
	m_fUReadyButtonTime -= fElapsedTime;
	if(m_bReady == FALSE 
		&& m_fUReadyButtonTime < 0)
	{// 준비 해제 상태로 오래있을경우 자동 신청취소된다.
		AppCancel(FALSE);

		// 2007-11-22 by bhsohn 아레나 통합서버
		m_fUReadyButtonTime = ARENA_BUTTON_UREADY_GAP;
	}
}
int	CINFArenaTeamInfo::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);
	BOOL bClick = FALSE;	
	bClick = m_pScroll->IsMouseWhellPos(pt);
	if(bClick)		
	{
		m_pScroll->OnMouseWheel(wParam, lParam);
		return INF_MSGPROC_BREAK;
	}		
	return INF_MSGPROC_NORMAL;	

}
int	CINFArenaTeamInfo::OnLButtonDown(WPARAM wParam, LPARAM lParam)
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
int	CINFArenaTeamInfo::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	BOOL bClick = FALSE;	
	bClick = m_pScroll->GetMouseMoveMode();
	if(bClick)		
	{
		m_pScroll->SetMouseMoveMode(FALSE);			
	}		
	return INF_MSGPROC_NORMAL;

}
int	CINFArenaTeamInfo::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);

	int nCnt = 0;
	if(m_pScroll->GetMouseMoveMode())
	{
		if(FALSE == m_pScroll->IsMouseScrollPos(pt))
		{
			m_pScroll->SetMouseMoveMode(FALSE);
			return INF_MSGPROC_NORMAL;			
		}
		m_pScroll->SetScrollPos(pt);
		return INF_MSGPROC_BREAK;			
	}
	return INF_MSGPROC_NORMAL;

}
int CINFArenaTeamInfo::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEWHEEL:
			{
				if(m_bTeamInfoShow)
				return OnMouseWheel(wParam,lParam);
			}
			break;
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				
				if(!m_bShow)	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
				{					
				if( m_bInfoButton)
//					pt.x > ARENA_TEAMINFO_BUTTON_X &&
//					pt.x < ARENA_TEAMINFO_BUTTON_X + ARENA_TEAMINFO_BUTTON_W &&
//					pt.y > ARENA_TEAMINFO_BUTTON_Y &&
//					pt.y < ARENA_TEAMINFO_BUTTON_Y + ARENA_TEAMINFO_BUTTON_H)
				{
					MoveInfoButton(pt);
					return INF_MSGPROC_BREAK;
				}
// 2007-06-01 by dgwoo 버튼이 아니므로 버튼 이미지가 필요없다.
//				if(pt.x > ARENA_TEAMINFO_BUTTON_X &&
//					pt.x < ARENA_TEAMINFO_BUTTON_X + ARENA_TEAMINFO_BUTTON_W &&
//					pt.y > ARENA_TEAMINFO_BUTTON_Y &&
//					pt.y < ARENA_TEAMINFO_BUTTON_Y + ARENA_TEAMINFO_BUTTON_H)
//				{
//					m_nMainBUnder = 1;
//				}
//				else
//				{
//					m_nMainBUnder = 0;
//				}
		
				if(pt.x > ARENA_TEAMINFO_WINDOWSIZE_X &&
					pt.x < ARENA_TEAMINFO_WINDOWSIZE_X + ARENA_TEAMINFO_WINDOWSIZE_W &&
					pt.y > ARENA_TEAMINFO_WINDOWSIZE_Y &&
					pt.y < ARENA_TEAMINFO_WINDOWSIZE_Y + ARENA_TEAMINFO_WINDOWSIZE_H)
				{
					m_nWinSizeB = BUTTON_STATE_UP;
				}
				else
				{
					m_nWinSizeB = BUTTON_STATE_NORMAL;
				}

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
					char buf[64] = { 0, };
					
					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nGearCaseChangeImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nGearCaseChangeImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						if(!m_bReady)
						{
							m_nStateGearCaseChangeB = BUTTON_STATE_UP;												
							// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
							if(m_bTeamInfoShow)
							{
								g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP5");
							}
							return INF_MSGPROC_BREAK;					
						}
					}
					else
					{
						m_nStateGearCaseChangeB = BUTTON_STATE_NORMAL;
					}

					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nReadyButtonImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nReadyButtonImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						if(m_bReady)
						{// 
							m_nReadyB = BUTTON_STATE_UP;
							// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
							// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
							if(m_bTeamInfoShow)
							{
								g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP79");
							}
							// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
							
						}
						else
						{// 
							m_nUReadyB = BUTTON_STATE_UP;
							// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
							// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
							if(m_bTeamInfoShow)
							{
								g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP95");
							}
							// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
							
						}
					}
					else
					{
						m_nReadyB = BUTTON_STATE_NORMAL;
						m_nUReadyB = BUTTON_STATE_NORMAL;
					}
					// AppCancel 신청 취소 버튼.
					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nAppCancleButtonImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nAppCancleButtonImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						m_nAppCancel = BUTTON_STATE_UP;
						// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
						// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
						if(m_bTeamInfoShow)
						{
							g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP81");
						}
						// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					}
					else
					{
						m_nAppCancel = BUTTON_STATE_NORMAL;
					}
#else					
				if(pt.x > ARENA_TEAMINFO_READY_X &&
					pt.x < ARENA_TEAMINFO_READY_X + ARENA_TEAMINFO_APPCANCEL_W &&
					pt.y > ARENA_TEAMINFO_READY_Y &&
					pt.y < ARENA_TEAMINFO_READY_Y + ARENA_TEAMINFO_APPCANCEL_H)
				{
					if(m_bReady)
					{// 
						m_nReadyB = BUTTON_STATE_UP;
						// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
						// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
						if(m_bTeamInfoShow)
						{
							g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP79");
						}
						// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	

						}
						else
					{// 
						m_nUReadyB = BUTTON_STATE_UP;
						// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
						// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
						if(m_bTeamInfoShow)
						{
							g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP95");
						}
						// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	

					}
				}
				else
				{
					m_nReadyB = BUTTON_STATE_NORMAL;
					m_nUReadyB = BUTTON_STATE_NORMAL;
				}
				// AppCancel 신청 취소 버튼.
				if(pt.x > ARENA_TEAMINFO_APPCANCEL_X &&
					pt.x < ARENA_TEAMINFO_APPCANCEL_X + ARENA_TEAMINFO_APPCANCEL_W &&
					pt.y > ARENA_TEAMINFO_APPCANCEL_Y &&
					pt.y < ARENA_TEAMINFO_APPCANCEL_Y + ARENA_TEAMINFO_APPCANCEL_H)
				{
					m_nAppCancel = BUTTON_STATE_UP;
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					// 2012-12-10 by bhsohn 아레나 대기창을 최소화 한 후, 툴팁 나오는 버그수정
					if(m_bTeamInfoShow)
					{
						g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP81");
					}
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				}
				else
				{
					m_nAppCancel = BUTTON_STATE_NORMAL;
				}
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
				}				

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
				if(m_bShow)
				{
					if(pt.y > ARENA_GEAR_CASE_CREATE_Y && pt.y < ARENA_GEAR_CASE_CREATE_Y + ARENA_GEAR_CASE_CREATE_H)
					{
						if(pt.x > ARENA_GEAR_CASE_CREATE_X && pt.x < ARENA_GEAR_CASE_CREATE_X + ARENA_GEAR_CASE_CREATE_W)
				{
					if(pt.y > ARENA_GEAR_CASE_CREATE_BUTTON_Y + ARENA_GEAR_CASE_CREATE_Y + m_nOkImgMinPos.y &&
						pt.y < ARENA_GEAR_CASE_CREATE_BUTTON_Y + ARENA_GEAR_CASE_CREATE_Y + m_nOkImgMinPos.y + m_nOkCancelBaseImgMaxPos.y)					
					{
						if(pt.x > ARENA_GEAR_CASE_CREATE_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nOkImgMinPos.x &&
							pt.x < ARENA_GEAR_CASE_CREATE_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nOkImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCreateB = BUTTON_STATE_UP;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_GEAR_CASE_CANCEL_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nCancelImgMinPos.x &&
							pt.x < ARENA_GEAR_CASE_CANCEL_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nCancelImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCancelB = BUTTON_STATE_UP;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}
						
					}
					else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
					
					char buf[64] = { 0, };
					
					if(pt.y > ARENA_GEAR_CASE_IMAGE_Y + ARENA_GEAR_CASE_CREATE_Y &&
						pt.y < ARENA_GEAR_CASE_IMAGE_Y + ARENA_GEAR_CASE_IMAGE_H + ARENA_GEAR_CASE_CREATE_Y)
					{
						if(pt.x > ARENA_GEAR_CASE_IMAGE_X + ARENA_GEAR_CASE_CREATE_X &&
							pt.x < ARENA_GEAR_CASE_IMAGE_X + ARENA_GEAR_CASE_IMAGE_W + ARENA_GEAR_CASE_CREATE_X)
						{
							wsprintf( buf, "%s", STRMSG_C_120604_0001);
							g_pGameMain->SetToolTip( pt.x, pt.y, buf);
							return INF_MSGPROC_BREAK;
						}
						
						if(pt.x > ARENA_GEAR_CASE_IMAGE_X + ARENA_GEAR_CASE_IMAGE_W + ARENA_GEAR_CASE_IMAGE_TAB + ARENA_GEAR_CASE_CREATE_X &&
							pt.x < ARENA_GEAR_CASE_IMAGE_X + (ARENA_GEAR_CASE_IMAGE_W * 2) + ARENA_GEAR_CASE_IMAGE_TAB + ARENA_GEAR_CASE_CREATE_X)
						{
							wsprintf( buf, "%s", STRMSG_C_120604_0002);
							g_pGameMain->SetToolTip( pt.x, pt.y, buf);
							return INF_MSGPROC_BREAK;
						}
						
						if(pt.x > ARENA_GEAR_CASE_IMAGE_X + (ARENA_GEAR_CASE_IMAGE_W * 2) + (ARENA_GEAR_CASE_IMAGE_TAB * 2) + ARENA_GEAR_CASE_CREATE_X &&
							pt.x < ARENA_GEAR_CASE_IMAGE_X + (ARENA_GEAR_CASE_IMAGE_W * 3) + (ARENA_GEAR_CASE_IMAGE_TAB * 2) + ARENA_GEAR_CASE_CREATE_X)
						{
							wsprintf( buf, "%s", STRMSG_C_120604_0003);
							g_pGameMain->SetToolTip( pt.x, pt.y, buf);
							return INF_MSGPROC_BREAK;
						}						
					}

							return INF_MSGPROC_BREAK;
						}						
					}
				}				
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

				return OnMouseMove(wParam,lParam);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(!m_bShow)	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
				{
				// Main Button
				if(pt.x > ARENA_TEAMINFO_BUTTON_X &&
					pt.x < ARENA_TEAMINFO_BUTTON_X + ARENA_TEAMINFO_BUTTON_W &&
					pt.y > ARENA_TEAMINFO_BUTTON_Y &&
					pt.y < ARENA_TEAMINFO_BUTTON_Y + ARENA_TEAMINFO_BUTTON_H)
				{
					//m_bTeamInfoShow = !m_bTeamInfoShow;
					m_bInfoButton = TRUE;
					m_ptOld = pt;
					return INF_MSGPROC_BREAK;
				}
				if(pt.x > ARENA_TEAMINFO_WINDOWSIZE_X &&
					pt.x < ARENA_TEAMINFO_WINDOWSIZE_X + ARENA_TEAMINFO_WINDOWSIZE_W &&
					pt.y > ARENA_TEAMINFO_WINDOWSIZE_Y &&
					pt.y < ARENA_TEAMINFO_WINDOWSIZE_Y + ARENA_TEAMINFO_WINDOWSIZE_H)
				{
					m_nWinSizeB = BUTTON_STATE_DOWN;
					return INF_MSGPROC_BREAK;
				}
				
// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
					if(!m_bReady)
					{
						if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x &&
							pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
							pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nGearCaseChangeImgMinPos.y &&
							pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nGearCaseChangeImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
						{
							m_nStateGearCaseChangeB = BUTTON_STATE_DOWN;
							return INF_MSGPROC_BREAK;					
						}
					}

				// Ready Button
					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nReadyButtonImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nReadyButtonImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						if(m_bReady)
						{// 
							m_nReadyB = BUTTON_STATE_DOWN;
						}
						else
						{// 
							m_nUReadyB = BUTTON_STATE_DOWN;
						}
						return INF_MSGPROC_BREAK;
					}
					// AppCancel 신청 취소 버튼.
					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nAppCancleButtonImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nAppCancleButtonImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						m_nAppCancel = BUTTON_STATE_DOWN;
						return INF_MSGPROC_BREAK;
					}
#else
					// Ready Button
				if(pt.x > ARENA_TEAMINFO_READY_X &&
					pt.x < ARENA_TEAMINFO_READY_X + ARENA_TEAMINFO_APPCANCEL_W &&
					pt.y > ARENA_TEAMINFO_READY_Y &&
					pt.y < ARENA_TEAMINFO_READY_Y + ARENA_TEAMINFO_APPCANCEL_H)
				{
					if(m_bReady)
					{// 
						m_nReadyB = BUTTON_STATE_DOWN;
						}
						else
					{// 
						m_nUReadyB = BUTTON_STATE_DOWN;
					}
					return INF_MSGPROC_BREAK;
				}
				// AppCancel 신청 취소 버튼.
				if(pt.x > ARENA_TEAMINFO_APPCANCEL_X &&
					pt.x < ARENA_TEAMINFO_APPCANCEL_X + ARENA_TEAMINFO_APPCANCEL_W &&
					pt.y > ARENA_TEAMINFO_APPCANCEL_Y &&
					pt.y < ARENA_TEAMINFO_APPCANCEL_Y + ARENA_TEAMINFO_APPCANCEL_H)
				{
					m_nAppCancel = BUTTON_STATE_DOWN;
					return INF_MSGPROC_BREAK;
				}
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
				}
				

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
				if(m_bShow)
				{
					if(pt.y > ARENA_GEAR_CASE_CREATE_Y && pt.y < ARENA_GEAR_CASE_CREATE_Y + ARENA_GEAR_CASE_CREATE_H)
					{
						if(pt.x > ARENA_GEAR_CASE_CREATE_X && pt.x < ARENA_GEAR_CASE_CREATE_X + ARENA_GEAR_CASE_CREATE_W)
				{
					if(pt.y > ARENA_GEAR_CASE_IMAGE_Y + ARENA_GEAR_CASE_CREATE_Y &&
						pt.y < ARENA_GEAR_CASE_IMAGE_Y + ARENA_GEAR_CASE_IMAGE_H + ARENA_GEAR_CASE_CREATE_Y)
					{
						if(pt.x > ARENA_GEAR_CASE_IMAGE_X + ARENA_GEAR_CASE_CREATE_X &&
							pt.x < ARENA_GEAR_CASE_IMAGE_X + ARENA_GEAR_CASE_IMAGE_W + ARENA_GEAR_CASE_CREATE_X)
						{
							m_nArenaGearCaseImgTabNum = ARENA_GEAR_ATTACK_CASE_INDEX;
						}
						
						if(pt.x > ARENA_GEAR_CASE_IMAGE_X + ARENA_GEAR_CASE_IMAGE_W + ARENA_GEAR_CASE_IMAGE_TAB + ARENA_GEAR_CASE_CREATE_X &&
							pt.x < ARENA_GEAR_CASE_IMAGE_X + (ARENA_GEAR_CASE_IMAGE_W * 2) + ARENA_GEAR_CASE_IMAGE_TAB + ARENA_GEAR_CASE_CREATE_X)
						{
							m_nArenaGearCaseImgTabNum = ARENA_GEAR_DEFENCE_CASE_INDEX;
						}
						
						if(pt.x > ARENA_GEAR_CASE_IMAGE_X + (ARENA_GEAR_CASE_IMAGE_W * 2) + (ARENA_GEAR_CASE_IMAGE_TAB * 2) + ARENA_GEAR_CASE_CREATE_X &&
							pt.x < ARENA_GEAR_CASE_IMAGE_X + (ARENA_GEAR_CASE_IMAGE_W * 3) + (ARENA_GEAR_CASE_IMAGE_TAB * 2) + ARENA_GEAR_CASE_CREATE_X)
						{
							m_nArenaGearCaseImgTabNum = ARENA_GEAR_EVASION_CASE_INDEX;
						}						
					}
					
					if(pt.y > ARENA_GEAR_CASE_CREATE_BUTTON_Y + ARENA_GEAR_CASE_CREATE_Y + m_nOkImgMinPos.y &&
						pt.y < ARENA_GEAR_CASE_CREATE_BUTTON_Y + ARENA_GEAR_CASE_CREATE_Y + m_nOkImgMinPos.y + m_nOkCancelBaseImgMaxPos.y)
					{
						if(pt.x > ARENA_GEAR_CASE_CREATE_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nOkImgMinPos.x &&
							pt.x < ARENA_GEAR_CASE_CREATE_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nOkImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCreateB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_GEAR_CASE_CANCEL_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nCancelImgMinPos.x &&
							pt.x < ARENA_GEAR_CASE_CANCEL_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nCancelImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCancelB = BUTTON_STATE_DOWN;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}
						
					}
					else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
					}
							
							return INF_MSGPROC_BREAK;
						}
					}					
				}				
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

				return OnLButtonDown(wParam,lParam);
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(!m_bShow)	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
				{
				m_bInfoButton = FALSE;
				if(pt.x > ARENA_TEAMINFO_WINDOWSIZE_X &&
					pt.x < ARENA_TEAMINFO_WINDOWSIZE_X + ARENA_TEAMINFO_WINDOWSIZE_W &&
					pt.y > ARENA_TEAMINFO_WINDOWSIZE_Y &&
					pt.y < ARENA_TEAMINFO_WINDOWSIZE_Y + ARENA_TEAMINFO_WINDOWSIZE_H)
				{
					m_bTeamInfoShow = !m_bTeamInfoShow;
					return INF_MSGPROC_BREAK;
				}

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
					if(!m_bReady)
					{
						if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x &&
							pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nGearCaseChangeImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
							pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nGearCaseChangeImgMinPos.y &&
							pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nGearCaseChangeImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
						{						
							m_nStateGearCaseChangeB = BUTTON_STATE_UP;
							m_bShow = TRUE;
							return INF_MSGPROC_BREAK;
						}
					}
					
					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nReadyButtonImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nReadyButtonImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nReadyButtonImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						ClickReadyButton();
						return INF_MSGPROC_BREAK;
					}
					// AppCancel 신청 취소 버튼.
					if(pt.x > ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x &&
						pt.x < ARENA_TEAMINFO_GEAR_CASE_X + m_nAppCancleButtonImgMinPos.x + m_nArenaInfoButtonImgMaxPos.x &&
						pt.y > ARENA_TEAMINFO_GEAR_CASE_Y + m_nAppCancleButtonImgMinPos.y &&
						pt.y < ARENA_TEAMINFO_GEAR_CASE_Y + m_nAppCancleButtonImgMinPos.y + m_nArenaInfoButtonImgMaxPos.y)
					{
						AppCancel();
						return INF_MSGPROC_BREAK;
					}
#else					
				if(pt.x > ARENA_TEAMINFO_READY_X &&
					pt.x < ARENA_TEAMINFO_READY_X + ARENA_TEAMINFO_APPCANCEL_W &&
					pt.y > ARENA_TEAMINFO_READY_Y &&
					pt.y < ARENA_TEAMINFO_READY_Y + ARENA_TEAMINFO_APPCANCEL_H)
				{
					ClickReadyButton();
					return INF_MSGPROC_BREAK;
				}
				// AppCancel 신청 취소 버튼.
				if(pt.x > ARENA_TEAMINFO_APPCANCEL_X &&
					pt.x < ARENA_TEAMINFO_APPCANCEL_X + ARENA_TEAMINFO_APPCANCEL_W &&
					pt.y > ARENA_TEAMINFO_APPCANCEL_Y &&
					pt.y < ARENA_TEAMINFO_APPCANCEL_Y + ARENA_TEAMINFO_APPCANCEL_H)
				{
					AppCancel();
					return INF_MSGPROC_BREAK;
				}
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
				}
				

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
				if(m_bShow)
				{
					if(pt.y > ARENA_GEAR_CASE_CREATE_Y && pt.y < ARENA_GEAR_CASE_CREATE_Y + ARENA_GEAR_CASE_CREATE_H)
					{
						if(pt.x > ARENA_GEAR_CASE_CREATE_X && pt.x < ARENA_GEAR_CASE_CREATE_X + ARENA_GEAR_CASE_CREATE_W)
						{
					if(pt.y > ARENA_GEAR_CASE_CREATE_BUTTON_Y + ARENA_GEAR_CASE_CREATE_Y + m_nOkImgMinPos.y &&
						pt.y < ARENA_GEAR_CASE_CREATE_BUTTON_Y + ARENA_GEAR_CASE_CREATE_Y + m_nOkImgMinPos.y + m_nOkCancelBaseImgMaxPos.y)
					{
						if(pt.x > ARENA_GEAR_CASE_CREATE_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nOkImgMinPos.x &&
							pt.x < ARENA_GEAR_CASE_CREATE_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nOkImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCreateB = BUTTON_STATE_UP;
							m_nArenaGearCaseImgListNum = m_nArenaGearCaseImgTabNum;
							SendPlayCharacterType();
							m_bShow = FALSE;
						}
						else
						{
							m_nStateCreateB = BUTTON_STATE_NORMAL;
						}
						if(pt.x > ARENA_GEAR_CASE_CANCEL_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nCancelImgMinPos.x &&
							pt.x < ARENA_GEAR_CASE_CANCEL_BUTTON_X + ARENA_GEAR_CASE_CREATE_X + m_nCancelImgMinPos.x + m_nOkCancelBaseImgMaxPos.x)
						{
							m_nStateCancelB = BUTTON_STATE_UP;
							m_bShow = FALSE;
						}
						else
						{
							m_nStateCancelB = BUTTON_STATE_NORMAL;
						}
						
					}
					else
					{
						m_nStateCreateB = BUTTON_STATE_NORMAL;
						m_nStateCancelB = BUTTON_STATE_NORMAL;
							}

							return INF_MSGPROC_BREAK;
						}
					}
				}				
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업

				return OnLButtonUp(wParam,lParam);
			}
			break;
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFArenaTeamInfo::AppCancel(BOOL bButton)
/// \brief		아레나 신청을 취소한다.
/// \author		dgwoo
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		bButton : 버튼으로 누른상태인가? 기본은 버튼으로 한다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
VOID CINFArenaTeamInfo::AppCancel(BOOL bButton)
{
	// 2007-05-11 by bhsohn 아레나 버그수정
	if(bButton
		&& !m_bTeamInfoShow)
	{// 팀인포가 보이는 상태가 아니라면 버튼인식을 하지 않는다.
		return;
	}

	// 2007-11-22 by bhsohn 아레나 통합서버
//	MSG_FC_ARENA_REAVE_TEAM sMsg;
//	sMsg.ArenaMode = ((CINFGameArena*)m_pParent)->m_nArenaMode;
//	sMsg.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
	if(((CINFGameArena*)m_pParent)->GetTeamNum() >= 0)
	{// 방번호가 없을때는 오류.
//		sMsg.TeamNum = ((CINFGameArena*)m_pParent)->GetTeamNum();
//		g_pFieldWinSocket->SendMsg(T_FC_ARENA_REAVE_TEAM,(char*)&sMsg,sizeof(sMsg));
		((CINFGameArena*)m_pParent)->SendArenaLeaveTeam(((CINFGameArena*)m_pParent)->m_nArenaMode);

	}else
	{
		DbgOutA("T_FC_ARENA_REAVE_TEAM (not Team Number)\n");
	}
}

VOID CINFArenaTeamInfo::ClickReadyButton()
{
	if(!m_bTeamInfoShow)
	{// 팀인포가 보이는 상태가 아니라면 버튼인식을 하지 않는다.
		return;
	}
	if(m_fArenaButtonGap > ARENA_BUTTON_GAP)
	{
		if(m_bReady)
		{// 준비 해제.
			g_pFieldWinSocket->SendMsg(T_FC_ARENA_TEAM_READY_FINISH_CANCEL,NULL,NULL);
		}else
		{
			// 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경
			if(TRUE == g_pD3dApp->IsDoingInfluenceWar())
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070427_0104,COLOR_ERROR); //"세력전이 진행 중이므로 참가할 수 없습니다."
				return;
			}
			// end 2008-03-07 by bhsohn 세력전시, 아레나 이용 못하게 변경

			// 준비 
			g_pFieldWinSocket->SendMsg(T_FC_ARENA_TEAM_READY_FINISH,NULL,NULL);
		}
		m_fArenaButtonGap = 0;
	}else
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_070510_0100,COLOR_ERROR);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 레뒤 상태 체크
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2007-12-17 ~ 2007-12-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFArenaTeamInfo::IsArenaReady()
{
	return m_bReady;
}

// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
#ifdef C_ARENA_EX_1ST_MSPARK
VOID CINFArenaTeamInfo::SendPlayCharacterType()
{	
	MSG_FC_ARENA_PLAY_CHARACTER_TYPE sMsg;
	memset(&sMsg, 0x00, sizeof(MSG_FC_ARENA_PLAY_CHARACTER_TYPE));
	
	if(m_nArenaGearCaseImgTabNum == ARENA_GEAR_ATTACK_CASE_INDEX)
	{
		m_nArenaGearCaseNum = ARENA_GEAR_ATTACK_CASE_NUMBER;
	}
	else if(m_nArenaGearCaseImgTabNum == ARENA_GEAR_DEFENCE_CASE_INDEX)
	{
		m_nArenaGearCaseNum = ARENA_GEAR_DEFENCE_CASE_NUMBER;
	}
	else if(m_nArenaGearCaseImgTabNum == ARENA_GEAR_EVASION_CASE_INDEX)
	{
		m_nArenaGearCaseNum = ARENA_GEAR_EVASION_CASE_NUMBER;
	}
	
	sMsg.PlayCharacterType = m_nArenaGearCaseNum;
	
	g_pFieldWinSocket->SendMsg(T_FC_ARENA_PLAY_CHARACTER_TYPE,(char*)&sMsg,sizeof(sMsg));
}
#endif
// end 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
