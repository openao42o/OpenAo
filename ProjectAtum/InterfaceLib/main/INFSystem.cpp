// INFSystem.cpp: implementation of the CINFSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFSystem.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "D3DHanFont.h"
#include "ShuttleChild.h"
#include "AtumSound.h"
#include "Camera.h"
#include "Interface.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Chat.h"
#include "StoreData.h"
#include "FxSystem.h"

#define MODE_COMBO_START_X			121
#define MODE_COMBO_SIZE_X			51
#define MODE_COMBO_SIZE_Y			13
#define MODE_COMBO_INTERVAL			20

#define MODE_CHANGE_PS_COMBO		0
//#define MODE_CHANGE_MUSIC_COMBO		2
#define MODE_CHANGE_SOUND_COMBO		1

#define MODE_CHANGE_MACHINE_COMBO	2	// 마우스 타겟 감추기
#define MODE_CHANGE_HELPDESK_COMBO	3

// 2007-07-23 by bhsohn 오퍼레이터 추가작업
//#define MODE_CHANGE_AUTOBALANCE		4	// 자동수평유지
//#define MODE_CHANGE_REVERSE_UD		5	// 상하반전
//#define MODE_CHANGE_REVERSE_LR		6	// 좌우반전
//#define MODE_CHANGE_MENU_BALACE		7	// 메뉴사용시 수평유지
//#define MODE_CHANGE_MP3_MUSIC		8	// 메뉴사용시 수평유지
#define MODE_CHANGE_OPEATOR		4	// 자동수평유지
#define MODE_CHANGE_AUTOBALANCE		5	// 자동수평유지
#define MODE_CHANGE_REVERSE_UD		6	// 상하반전
#define MODE_CHANGE_REVERSE_LR		7	// 좌우반전
#define MODE_CHANGE_MENU_BALACE		8	// 메뉴사용시 수평유지
#define MODE_CHANGE_PET_OP_BALACE	9	// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
#ifdef _WARRIOR_ADDITIONAL_INFO
	#define MODE_CHANGE_WSW_AIMINFO		10
	#define MODE_CHANGE_MP3_MUSIC		11	// 메뉴사용시 수평유지
#else
	#define MODE_CHANGE_MP3_MUSIC		10	// 메뉴사용시 수평유지
#endif
#ifdef _SHOW_GUILD_NAME
	#ifdef _WARRIOR_ADDITIONAL_INFO
		#define MODE_CHANGE_SHOW_GUILDNAME 12
	#else
		#define MODE_CHANGE_SHOW_GUILDNAME 11
	#endif
#endif

#define MAX_MODE_CHANGE				11
// end 2007-07-23 by bhsohn 오퍼레이터 추가작업


#define MODE_CHANGE_PS_0			STRMSG_C_INTERFACE_0002
#define MODE_CHANGE_PS_1			STRMSG_C_INTERFACE_0003

#define MODE_CHANGE_TARGET_0		STRMSG_C_INTERFACE_0027
#define MODE_CHANGE_TARGET_1		STRMSG_C_INTERFACE_0028

#define MODE_CHANGE_HEIGHT_0		STRMSG_C_INTERFACE_0027
#define MODE_CHANGE_HEIGHT_1		STRMSG_C_INTERFACE_0028

#define MODE_PK_MODE_0				STRMSG_C_INTERFACE_0029
#define MODE_PK_MODE_1				STRMSG_C_INTERFACE_0030

#define MODE_CHANGE_MODE_0			STRMSG_C_INTERFACE_0031
#define MODE_CHANGE_MODE_1			STRMSG_C_INTERFACE_0032

#define MODE_CHANGE_MACHINE_0		STRMSG_C_INTERFACE_0033
#define MODE_CHANGE_MACHINE_1		STRMSG_C_INTERFACE_0034

#define MODE_UNUSED					STRMSG_C_INTERFACE_0035

#define MODE_RENDER_LOWQUALITY_X	50
#define MODE_RENDER_LOWQUALITY_Y	232
#define MODE_RENDER_REFLACTIVE_X	50
#define MODE_RENDER_REFLACTIVE_Y	208
#define MODE_RENDER_HELPDESK_X		50
// 2007-07-23 by bhsohn 오퍼레이터 추가작업
//#define MODE_RENDER_HELPDESK_Y		151
#define MODE_RENDER_HELPDESK_Y		131

#define DEFAULT_FONT_COLOR			RGB(255,255,255)
#define DEFAULT_FONT_COMBO_LIST		RGB(255,255,  0)

//ydkim. 04.07.02
#define END_GAME_TIMING_ATTACK		5000			// 게임도중 로그오프 게임종료 타이밍 

#define MIN_QUALITY_RENDER_TERRAIN		3
#define MIN_QUALITY_RENDER				0

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
// 2007-07-23 by bhsohn 오퍼레이터 추가작업
//#define MODE_COMBO_START_Y			71//73
#define MODE_COMBO_START_Y			51//73
#else
// 2007-07-23 by bhsohn 오퍼레이터 추가작업
//#define MODE_COMBO_START_Y			71//73
#define MODE_COMBO_START_Y			51//73
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSystem::CINFSystem(CAtumNode* pParent)
{
	FLOG( "CINFSystem(CAtumNode* pParent)" );
	m_pLogOut = NULL;
	m_pOpLog = NULL;
	m_pOption = NULL;
	m_pMode = NULL;

	m_pParent = pParent;
	m_nSystemType = SYSTEM_SYSTEM;
	m_nSystemButtonState[0] = BUTTON_STATE_NORMAL;
	m_nSystemButtonState[1] = BUTTON_STATE_NORMAL;
	int i;
	for(i=0;i<3;i++)
	{
		m_pSystemSelectButton[i] = NULL;
		m_pSystemEndButton[i] = NULL;
	}

	m_pCombo = NULL ;
	m_pCheck = NULL ;


	for(i=0;i<OPTION_COMBO_LIST_NUMBER;i++)
	{
		m_pFontOptionList[i] = NULL;
	}
	for(i=0 ; i<OPTION_COMBO_NUMBER ;i++)
	{
		m_bShowOptionListBox[i] = FALSE;
		m_nOptionInfo[i] = -1;
		m_pFontSelectOptionComboData[i] = NULL;

	}
	memset(m_strSelectOptionComboData,0x00, OPTION_COMBO_NUMBER*OPTION_COMBO_STR_LEN);

	memset(m_strSelectModeComboData,0x00, MODE_COMBO_NUMBER*MODE_COMBO_STR_LEN);
	m_pFontModeList[0] = NULL;
	m_pFontModeList[1] = NULL;
	for(i=0;i<MODE_COMBO_NUMBER;i++)
	{
		m_pFontSelectModeComboData[i] = NULL;
		m_bShowModeListBox[i] = FALSE;
		m_nModeInfo[i] = 0;// value setting
	}

//	g_pSOption->sGammaCtrl = 256 ;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	memcpy(&m_pSOptionOld, g_pSOption, sizeof(sOPTION_SYSTEM));
	m_bQuitGameMessage = FALSE;
	m_bLogOffGameMessage = FALSE;
	m_fTickSecTime = 0.0f;
	m_fLastTickSecTime = 0.0f;
	m_bRenderToolTip = FALSE;
	m_pFontToolTip = NULL;
}

CINFSystem::~CINFSystem()
{
	FLOG( "~CINFSystem()" );
	int i;
	for(i=0;i<3;i++)
	{
		util::del(m_pSystemSelectButton[i]);
		util::del(m_pSystemEndButton[i]);
	}
	util::del(m_pLogOut);
	util::del(m_pOpLog);
	util::del(m_pOption);
	util::del(m_pMode);

	util::del(m_pCombo);
	util::del(m_pCheck);

	for(i=0;i<OPTION_COMBO_LIST_NUMBER;i++)
	{
		util::del(m_pFontOptionList[i] );
	}

	for(i=0 ; i<OPTION_COMBO_NUMBER ;i++)
	{
		util::del(m_pFontSelectOptionComboData[i] ) ;
	}
	util::del(m_pFontModeList[0]);
	util::del(m_pFontModeList[1]);
	for(i=0;i<MODE_COMBO_NUMBER;i++)
	{
		util::del(m_pFontSelectModeComboData[i]);
	}
	util::del(m_pFontToolTip);
}


HRESULT CINFSystem::InitDeviceObjects()
{
	FLOG( "CINFSystem::InitDeviceObjects()" );
	DataHeader	* pDataHeader ;
 // 2011. 10. 10 by jskim UI시스템 변경
		m_pSystemSelectButton[0] = new CINFImageEx;
	pDataHeader = FindResource("Selover");
	m_pSystemSelectButton[0]->InitDeviceObjects(pDataHeader) ;

		m_pSystemSelectButton[1] = new CINFImageEx;
	pDataHeader = FindResource("Selpush");
	m_pSystemSelectButton[1]->InitDeviceObjects(pDataHeader) ;

		m_pSystemSelectButton[2] = new CINFImageEx;
	pDataHeader = FindResource("Seldis");
	m_pSystemSelectButton[2]->InitDeviceObjects(pDataHeader) ;

		m_pSystemEndButton[0] = new CINFImageEx;
	pDataHeader = FindResource("enover");
	m_pSystemEndButton[0]->InitDeviceObjects(pDataHeader) ;

		m_pSystemEndButton[1] = new CINFImageEx;
	pDataHeader = FindResource("enpush");
	m_pSystemEndButton[1]->InitDeviceObjects(pDataHeader) ;

		m_pSystemEndButton[2] = new CINFImageEx;
	pDataHeader = FindResource("endis");
	m_pSystemEndButton[2]->InitDeviceObjects(pDataHeader) ;

		m_pLogOut = new CINFImageEx;
	pDataHeader = FindResource("logout");
	m_pLogOut->InitDeviceObjects(pDataHeader) ;

		m_pOpLog = new CINFImageEx;
	pDataHeader = FindResource("Op_Log");
	m_pOpLog->InitDeviceObjects(pDataHeader) ;

		m_pOption = new CINFImageEx;
	pDataHeader = FindResource("option");
	m_pOption->InitDeviceObjects(pDataHeader) ;

		m_pMode = new CINFImageEx;
	pDataHeader = FindResource("mode");
	m_pMode->InitDeviceObjects(pDataHeader) ;

		m_pCombo = new CINFImageEx ;
	pDataHeader = FindResource("s_combo");
	m_pCombo->InitDeviceObjects(pDataHeader) ;
	
		m_pCheck = new CINFImageEx ;
	pDataHeader = FindResource("CHECK");
	m_pCheck->InitDeviceObjects(pDataHeader) ;

	int i;
	for(i=0;i<OPTION_COMBO_LIST_NUMBER;i++)
	{
		m_pFontOptionList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontOptionList[i]->InitDeviceObjects(g_pD3dDev);
	}

	for(i=0 ; i<OPTION_COMBO_NUMBER ;i++)
	{
		m_pFontSelectOptionComboData[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontSelectOptionComboData[i]->InitDeviceObjects(g_pD3dDev);
	}
	m_pFontModeList[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
	m_pFontModeList[0]->InitDeviceObjects(g_pD3dDev);
	m_pFontModeList[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
	m_pFontModeList[1]->InitDeviceObjects(g_pD3dDev);
	for(i=0;i<MODE_COMBO_NUMBER;i++)
	{
		m_pFontSelectModeComboData[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontSelectModeComboData[i]->InitDeviceObjects(g_pD3dDev);
	}

	m_wGamma = 256 ;
	m_pFontToolTip = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontToolTip->InitDeviceObjects(g_pD3dDev);

	return S_OK ;
}

HRESULT CINFSystem::RestoreDeviceObjects()
{
	FLOG( "CINFSystem::RestoreDeviceObjects()" );
	int i;
	for(i=0;i<3;i++)
	{
		m_pSystemSelectButton[i]->RestoreDeviceObjects();
		m_pSystemEndButton[i]->RestoreDeviceObjects();
	}
	if(m_pLogOut )
		m_pLogOut->RestoreDeviceObjects();
	if(m_pOpLog )
		m_pOpLog->RestoreDeviceObjects();
	if(m_pOption )
		m_pOption->RestoreDeviceObjects();
	if(m_pMode )
		m_pMode->RestoreDeviceObjects();

	if(m_pCombo )
		m_pCombo->RestoreDeviceObjects();
	
	if(m_pCheck )
		m_pCheck->RestoreDeviceObjects();

	for(i=0;i<OPTION_COMBO_LIST_NUMBER;i++)
	{
		if(m_pFontOptionList[i] )
			m_pFontOptionList[i]->RestoreDeviceObjects();
	}

	for(i=0 ; i<OPTION_COMBO_NUMBER ;i++)
	{
		if(m_pFontSelectOptionComboData[i] )
			m_pFontSelectOptionComboData[i]->RestoreDeviceObjects();
	}

	m_pFontModeList[0]->RestoreDeviceObjects();
	m_pFontModeList[1]->RestoreDeviceObjects();
	for(i=0;i<MODE_COMBO_NUMBER;i++)
	{
		m_pFontSelectModeComboData[i]->RestoreDeviceObjects();
	}
	m_pFontToolTip->RestoreDeviceObjects();

	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFSystem::DeleteDeviceObjects()
{
	FLOG( "CINFSystem::DeleteDeviceObjects()" );
	int i;
	for(i=0;i<3;i++)
	{
		m_pSystemSelectButton[i]->DeleteDeviceObjects();
		util::del(m_pSystemSelectButton[i] );
		m_pSystemEndButton[i]->DeleteDeviceObjects();
		util::del(m_pSystemEndButton[i] );
	}
	if(m_pLogOut )
	{
		m_pLogOut->DeleteDeviceObjects();
		util::del(m_pLogOut );
	}
	if(m_pOpLog )
	{
		m_pOpLog->DeleteDeviceObjects();
		util::del(m_pOpLog );
	}
	if(m_pOption )
	{
		m_pOption->DeleteDeviceObjects();
		util::del(m_pOption );
	}
	if(m_pMode )
	{
		m_pMode->DeleteDeviceObjects();
		util::del(m_pMode );
	}
	if(m_pCombo )
	{
		m_pCombo->DeleteDeviceObjects();
		util::del(m_pCombo );
	}

	if(m_pCheck )
	{
		m_pCheck->DeleteDeviceObjects();
		util::del(m_pCheck );
	}
	for(i=0;i<OPTION_COMBO_LIST_NUMBER;i++)
	{
		if(m_pFontOptionList[i] )
			m_pFontOptionList[i]->DeleteDeviceObjects();
		util::del(m_pFontOptionList[i]  );

	}
	for(i=0 ; i<OPTION_COMBO_NUMBER ;i++)
	{
		if(m_pFontSelectOptionComboData[i] )
			m_pFontSelectOptionComboData[i]->DeleteDeviceObjects();
			util::del(m_pFontSelectOptionComboData[i]  );
	}

	m_pFontModeList[0]->DeleteDeviceObjects();
	util::del(m_pFontModeList[0]  );
	m_pFontModeList[1]->DeleteDeviceObjects();
	util::del(m_pFontModeList[1]  );
	for(i=0;i<MODE_COMBO_NUMBER;i++)
	{
		m_pFontSelectModeComboData[i]->DeleteDeviceObjects();
		util::del(m_pFontSelectModeComboData[i]  );
	}

	if(m_pFontToolTip)
	{
		m_pFontToolTip->DeleteDeviceObjects();
		util::del(m_pFontToolTip);
	}
	m_bInvalidated = FALSE;

	return S_OK ;
}


HRESULT CINFSystem::InvalidateDeviceObjects()
{
	FLOG( "CINFSystem::InvalidateDeviceObjects()" );
	int i;
	for(i=0;i<3;i++)
	{
		m_pSystemSelectButton[i]->InvalidateDeviceObjects();
		m_pSystemEndButton[i]->InvalidateDeviceObjects();
	}
	if(m_pLogOut )
		m_pLogOut->InvalidateDeviceObjects();
	if(m_pOpLog )
		m_pOpLog->InvalidateDeviceObjects();
	if(m_pOption )
		m_pOption->InvalidateDeviceObjects();
	if(m_pMode )
		m_pMode->InvalidateDeviceObjects();

	if(m_pCombo )
		m_pCombo->InvalidateDeviceObjects();
	if(m_pCheck )
		m_pCheck->InvalidateDeviceObjects();
	for(i=0;i<OPTION_COMBO_LIST_NUMBER;i++)
	{
		if(m_pFontOptionList[i] )
			m_pFontOptionList[i]->InvalidateDeviceObjects();
	}

	for(i=0 ; i<OPTION_COMBO_NUMBER ;i++)
	{
		if(m_pFontSelectOptionComboData[i] )
			m_pFontSelectOptionComboData[i]->InvalidateDeviceObjects();
	}

	m_pFontModeList[0]->InvalidateDeviceObjects();
	m_pFontModeList[1]->InvalidateDeviceObjects();
	for(i=0;i<MODE_COMBO_NUMBER;i++)
	{
		m_pFontSelectModeComboData[i]->InvalidateDeviceObjects();
	}
	m_pFontToolTip->InvalidateDeviceObjects();

	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	return S_OK ;
}


void CINFSystem::Tick()
{
	FLOG( "CINFSystem::Tick()" );
	
	if(TRUE == m_bQuitGameMessage)
	{
		DWORD dwCurTick = timeGetTime();
		if(dwCurTick - m_fLastTickSecTime < END_GAME_TIMING_ATTACK) 
		{
			if(dwCurTick - m_fLastTickSecTime > m_fTickSecTime)
			{
				m_fTickSecTime += 1000.0f;

				int tempTime = ((END_GAME_TIMING_ATTACK-m_fTickSecTime)/1000)+1;
				char message[256];
				
				if(TRUE == m_bLogOffGameMessage)
					wsprintf(message,STRMSG_C_INTERFACE_0036, tempTime);//"로그오프까지 \\r%d초\\r 남았습니다."
				else
					wsprintf(message,STRMSG_C_INTERFACE_0037, tempTime);//"게임종료까지 \\r%d초\\r 남았습니다."

//				g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);
				g_pGameMain->m_pInfWindow->SetMessageBoxTitleChange(message, _Q_QUIT_MESSAGE);				
			}
		}
		else
		{
			// 2008-10-30 by bhsohn 게임 종료 페이드 아웃시 락시스템 적용
			g_pD3dApp->EnterLock(FALSE);	// FALSE :Lock  TRUE : UnLock	
			// end 2008-10-30 by bhsohn 게임 종료 페이드 아웃시 락시스템 적용
			
			char message[256]; 
			wsprintf(message,STRMSG_C_INTERFACE_0038); //"종료."
			g_pD3dApp->m_pChat->CreateChatChild(message, COLOR_SYSTEM);
			if(FALSE == m_bLogOffGameMessage)
			{
				g_pD3dApp->m_bClientQuit = TRUE;	// GAME END 메세지 처리후 종료하기 위해 셋팅
				m_bLogOffGameMessage = FALSE;
			}
			
			m_bQuitGameMessage = FALSE;
			m_fTickSecTime = 0.0f;
			m_fLastTickSecTime = 0.0f;

			// 시네마
			if( g_pD3dApp->m_dwGameState == _CITY )
			{
				SendFieldSocketCharacterGameEnd();
			}
			else
			{
				if( g_pShuttleChild->InitCinemaCamera(PATTERN_CAMERA_GAME_END) == TRUE )
				{
					g_pD3dApp->StartFadeEffect(FALSE,1,D3DCOLOR_ARGB(0,0,0,0));
					g_pShuttleChild->m_nEventType = EVENT_GAME_END;
					g_pShuttleChild->ChangeUnitState( _WARP );
					g_pInterface->SaveCharacterFile(SLOT_PURPOSE_MAIN);				// 2012-06-21 by jhseol, 아레나 추가개발part2 - 아레나 전용 퀵슬롯 파라미터 추가 ( 0 = 메인, 1 = 아레나 )
				}
			}
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_M_A_GEAR );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_B_I_GEAR );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFSystem::OptionQualityCombo()
/// \brief		최소프레임 설정.
/// \author		dgwoo
/// \date		2007-01-18 ~ 2007-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFSystem::OptionQualityCombo()
{
	g_pSOption->sLowQuality = !g_pSOption->sLowQuality;
	UpdateQuality(g_pSOption->sLowQuality);
	g_pInterface->SaveOptionFile();

	// 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	// 최소 프레임 모드를 키면 리로드한다. 	
	g_pD3dApp->ReLoadEnemyRenderList();		
	// end 2007-08-29 by bhsohn 최소 프레임시 기본 아머만 로딩하게끔 변경
	return TRUE;
}


void CINFSystem::Render()
{
	FLOG( "CINFSystem::Render()" );
	int nWindowPosY = g_pGameMain->m_nRightWindowY;
//	int nWindowPosY = POS_RIGHT_WINDOW_Y;
//	if(nWindowPosY < SIZE_ROLL_Y )
//		nWindowPosY = SIZE_ROLL_Y;
//	if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//		nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
	if(m_pOpLog )
	{
		m_pOpLog->Move(POS_RIGHT_WINDOW_X+8, nWindowPosY + 5);
		m_pOpLog->Render() ;
	}
	switch(m_nSystemType)
	{
	case SYSTEM_SYSTEM:
		{
			if(m_pLogOut )
			{
				m_pLogOut->Move(POS_RIGHT_WINDOW_X+23, nWindowPosY + 30);
				m_pLogOut->Render() ;
			}
			// 버튼 : 로그아웃
			if(m_nSystemButtonState[0] != BUTTON_STATE_NORMAL)
			{
				m_pSystemSelectButton[m_nSystemButtonState[0]]->Move(POS_RIGHT_WINDOW_X+73, nWindowPosY + 116);
				m_pSystemSelectButton[m_nSystemButtonState[0]]->Render();
			}
			// 버튼 : 초대
			if(m_nSystemButtonState[1] != BUTTON_STATE_NORMAL)
			{
				m_pSystemEndButton[m_nSystemButtonState[1]]->Move(POS_RIGHT_WINDOW_X+73, nWindowPosY + 144);
				m_pSystemEndButton[m_nSystemButtonState[1]]->Render();
			}
		}
		break;
	case SYSTEM_OPTION:
		{
			char    *TempSelectComboData;
			SIZE	nStrSize;
			if(m_pOption )
			{
				m_pOption->Move(POS_RIGHT_WINDOW_X+23, nWindowPosY + 30);
				m_pOption->Render() ;
			}

			// 2007-12-21 by dgwoo 창모드 지원.
			//wsprintf( m_strSelectOptionComboData[0], "%d*%d", g_pD3dApp->GetBackBufferDesc().Width, g_pD3dApp->GetBackBufferDesc().Height);
			if(g_pD3dApp->m_IsFullMode)
			{
				wsprintf( m_strSelectOptionComboData[0], "%d*%d", g_pD3dApp->m_nWidth,g_pD3dApp->m_nHeight);
			}
			else
			{
				wsprintf( m_strSelectOptionComboData[0], "%d*%d%s", g_pD3dApp->m_nWidth,g_pD3dApp->m_nHeight,STRMSG_C_071221_0100);
			}
			nStrSize = m_pFontSelectOptionComboData[0]->GetStringSize(m_strSelectOptionComboData[0]);
			

			if(m_strSelectOptionComboData[0][0])
			{
				m_pFontSelectOptionComboData[0]->DrawText(POS_RIGHT_WINDOW_X+OPTION_RESOLUTION_X-nStrSize.cx,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*0,DEFAULT_FONT_COLOR, m_strSelectOptionComboData[0], 0L);
			}

			//지형 시야
			if(g_pSOption->sTerrainRender == 0)
				TempSelectComboData= "0" ;
			if(g_pSOption->sTerrainRender == 1)
				TempSelectComboData= "1" ;
			if(g_pSOption->sTerrainRender == 2)
				TempSelectComboData= "2" ;
			if(g_pSOption->sTerrainRender == 3)
				TempSelectComboData= "3" ;
			if(g_pSOption->sTerrainRender == 4)
				TempSelectComboData= "4" ;
			if(g_pSOption->sTerrainRender == 5)
				TempSelectComboData= "5" ;
			if(g_pSOption->sTerrainRender == 6)
				TempSelectComboData= "6" ;
			if(g_pSOption->sTerrainRender == 7)
				TempSelectComboData= "7" ;
			if(g_pSOption->sTerrainRender == 8)
				TempSelectComboData= "8" ;
			if(g_pSOption->sTerrainRender == 9)
				TempSelectComboData= "9" ;
			m_pFontSelectOptionComboData[1]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
				nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*1,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			//유닛 디테일 시야
			if(g_pSOption->sUnitDetail == 0)
				TempSelectComboData= "0" ;
			if(g_pSOption->sUnitDetail == 1)
				TempSelectComboData= "1" ;
			if(g_pSOption->sUnitDetail == 2)
				TempSelectComboData= "2" ;
			if(g_pSOption->sUnitDetail == 3)
				TempSelectComboData= "3" ;
			if(g_pSOption->sUnitDetail == 4)
				TempSelectComboData= "4" ;
			if(g_pSOption->sUnitDetail == 5)
				TempSelectComboData= "5" ;
			if(g_pSOption->sUnitDetail == 6)
				TempSelectComboData= "6" ;
			if(g_pSOption->sUnitDetail == 7)
				TempSelectComboData= "7" ;
			if(g_pSOption->sUnitDetail == 8)
				TempSelectComboData= "8" ;
			if(g_pSOption->sUnitDetail == 9)
				TempSelectComboData= "9" ;
			m_pFontSelectOptionComboData[2]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
				nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*2,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);
			
			//그림자			
			if(g_pSOption->sShadowState == 0)
				TempSelectComboData = "0" ;
			if(g_pSOption->sShadowState == 1)
				TempSelectComboData = "1" ;
			if(g_pSOption->sShadowState == 2)
				TempSelectComboData = "2" ;
			if(g_pSOption->sShadowState == 3)
				TempSelectComboData = "3" ;
			if(g_pSOption->sShadowState == 4)
				TempSelectComboData = "4" ;
			if(g_pSOption->sShadowState == 5)
				TempSelectComboData = "5" ;
			if(g_pSOption->sShadowState == 6)
				TempSelectComboData = "6" ;
			if(g_pSOption->sShadowState == 7)
				TempSelectComboData = "7" ;
			if(g_pSOption->sShadowState == 8)
				TempSelectComboData = "8" ;
			if(g_pSOption->sShadowState == 9)
				TempSelectComboData = "9" ;
			m_pFontSelectOptionComboData[3]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*3,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			//이펙트	

			if(g_pSOption->sTerrainEffectRender == 0)
				TempSelectComboData= "0" ;
			if(g_pSOption->sTerrainEffectRender == 1)
				TempSelectComboData= "1" ;
			if(g_pSOption->sTerrainEffectRender == 2)
				TempSelectComboData= "2" ;
			if(g_pSOption->sTerrainEffectRender == 3)
				TempSelectComboData= "3" ;
			if(g_pSOption->sTerrainEffectRender == 4)
				TempSelectComboData= "4" ;
			if(g_pSOption->sTerrainEffectRender == 5)
				TempSelectComboData= "5" ;
			if(g_pSOption->sTerrainEffectRender == 6)
				TempSelectComboData= "6" ;
			if(g_pSOption->sTerrainEffectRender == 7)
				TempSelectComboData= "7" ;
			if(g_pSOption->sTerrainEffectRender == 8)
				TempSelectComboData= "8" ;
			if(g_pSOption->sTerrainEffectRender == 9)
				TempSelectComboData= "9" ;
			m_pFontSelectOptionComboData[4]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
				nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*4,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			//감마
			if(g_pSOption->sGammaCtrl == 5)
				TempSelectComboData= "5" ;
			if(g_pSOption->sGammaCtrl == 4)
				TempSelectComboData= "4" ;
			if(g_pSOption->sGammaCtrl == 3)
				TempSelectComboData= "3" ;
			if(g_pSOption->sGammaCtrl == 2)
				TempSelectComboData= "2" ;
			if(g_pSOption->sGammaCtrl == 1)
				TempSelectComboData= "1" ;
			if(g_pSOption->sGammaCtrl == 0)
				TempSelectComboData= "0" ;
			if(g_pSOption->sGammaCtrl == -1)
				TempSelectComboData= "-1" ;
			if(g_pSOption->sGammaCtrl == -2)
				TempSelectComboData= "-2" ;
			if(g_pSOption->sGammaCtrl == -3)
				TempSelectComboData= "-3" ;
			if(g_pSOption->sGammaCtrl == -4)
				TempSelectComboData= "-4" ;
			m_pFontSelectOptionComboData[5]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
				nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*5,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			//대비
			if(g_pSOption->sContrast == 0)
				TempSelectComboData= "0" ;
			if(g_pSOption->sContrast == 1)
				TempSelectComboData= "1" ;
			if(g_pSOption->sContrast == 2)
				TempSelectComboData= "2" ;
			if(g_pSOption->sContrast == 3)
				TempSelectComboData= "3" ;
			if(g_pSOption->sContrast == 4)
				TempSelectComboData= "4" ;
			if(g_pSOption->sContrast == 5)
				TempSelectComboData= "5" ;
			if(g_pSOption->sContrast == 6)
				TempSelectComboData= "6" ;
			if(g_pSOption->sContrast == 7)
				TempSelectComboData= "7" ;
			if(g_pSOption->sContrast == 8)
				TempSelectComboData= "8" ;
			if(g_pSOption->sContrast == 9)
				TempSelectComboData= "9" ;
			
			if(!g_pD3dApp->m_pFxSystem->IsRenderBlurFilter())
			{
				m_pFontSelectOptionComboData[6]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT-23,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*6,DEFAULT_FONT_COLOR, STRMSG_C_INTERFACE_0039, 0L);//"지원안됨"
			}
			else if(g_pSOption->sContrast == 0)
			{
				m_pFontSelectOptionComboData[6]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT-23,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*6,DEFAULT_FONT_COLOR, MODE_UNUSED, 0L);
			}
			else
			{
				m_pFontSelectOptionComboData[6]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*6,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);				
			}
			
			
			
			//필터
			if(g_pSOption->sFilter == 0)
				TempSelectComboData= "0" ;
			if(g_pSOption->sFilter == 1)
				TempSelectComboData= "1" ;
			if(g_pSOption->sFilter == 2)
				TempSelectComboData= "2" ;
			if(g_pSOption->sFilter == 3)
				TempSelectComboData= "3" ;
			if(g_pSOption->sFilter == 4)
				TempSelectComboData= "4" ;
			if(g_pSOption->sFilter == 5)
				TempSelectComboData= "5" ;
			if(g_pSOption->sFilter == 6)
				TempSelectComboData= "6" ;
			if(g_pSOption->sFilter == 7)
				TempSelectComboData= "7" ;
			if(g_pSOption->sFilter == 8)
				TempSelectComboData= "8" ;
			if(g_pSOption->sFilter == 9)
				TempSelectComboData= "9" ;
			if( !g_pD3dApp->m_pFxSystem->IsRenderGaussBlur())
			{
				m_pFontSelectOptionComboData[7]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT-23,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*7,DEFAULT_FONT_COLOR, STRMSG_C_INTERFACE_0039, 0L);//"지원안됨"
			}
			else if(g_pSOption->sFilter == 0)
			{
				m_pFontSelectOptionComboData[7]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT-23,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*7,DEFAULT_FONT_COLOR, MODE_UNUSED, 0L);				
			}
			else
			{
				m_pFontSelectOptionComboData[7]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_SORT,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*7,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);				
			}
			

			// 최소 사양

			if(g_pSOption->sLowQuality == 0)
				TempSelectComboData = "ON" ;
			if(g_pSOption->sLowQuality == 1)
				TempSelectComboData = "OFF" ;
			
			RenderComboOptionList(nWindowPosY);
			RenderCheckOption(nWindowPosY);

		}
		break;
	case SYSTEM_MODE:
		{
			char    *TempSelectComboData ;

			m_pMode->Move(POS_RIGHT_WINDOW_X+23, nWindowPosY + 30);
			m_pMode->Render();
			
			// 시점 
			if(g_pSOption->sEyePt == FALSE)
				TempSelectComboData = MODE_CHANGE_PS_1 ;
			if(g_pSOption->sEyePt == TRUE)
				TempSelectComboData = MODE_CHANGE_PS_0 ;
			
			m_pFontSelectOptionComboData[0]->DrawText(POS_RIGHT_WINDOW_X+MODE_COMBO_START_X+13,
				nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*0,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			//배경음 조절
			if(g_pSOption->sSoundVolume == -10000)
				TempSelectComboData= "0" ;
			if(g_pSOption->sSoundVolume == -3500)
				TempSelectComboData= "1" ;
			if(g_pSOption->sSoundVolume == -3000)
				TempSelectComboData= "2" ;
			if(g_pSOption->sSoundVolume == -2500)
				TempSelectComboData= "3" ;
			if(g_pSOption->sSoundVolume == -2000)
				TempSelectComboData= "4" ;
			if(g_pSOption->sSoundVolume == -1500)
				TempSelectComboData= "5" ;
			if(g_pSOption->sSoundVolume == -1000)
				TempSelectComboData= "6" ;
			if(g_pSOption->sSoundVolume == -600)
				TempSelectComboData= "7" ;
			if(g_pSOption->sSoundVolume == -300)
				TempSelectComboData= "8" ;
			if(g_pSOption->sSoundVolume == 0)
				TempSelectComboData= "9" ;
			
			m_pFontSelectOptionComboData[1]->DrawText(POS_RIGHT_WINDOW_X+MODE_COMBO_START_X+OPTION_COMBO_SORT,
				nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*1,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			// 조작
//			if(g_pSOption->sHandle == 0)
//				TempSelectComboData = MODE_CHANGE_MACHINE_0 ;
//			if(g_pSOption->sHandle == 1)
//				TempSelectComboData = MODE_CHANGE_MACHINE_1 ;
			
//			m_pFontSelectOptionComboData[3]->DrawText(POS_RIGHT_WINDOW_X+MODE_COMBO_START_X+8,
//				nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*2,DEFAULT_FONT_COLOR, TempSelectComboData, 0L);

			RenderComboModeList(nWindowPosY);
			RenderCheckOption(nWindowPosY);
			
			// 타겟 보이기 관련 툴팁 설명
			if(m_bRenderToolTip == TRUE){
				char strBuf[256];
				ZERO_MEMORY(strBuf);
				wsprintf(strBuf, STRMSG_C_050518_0003);	//"A 기어는 사용 불가능"	
				g_pGameMain->RenderPopUpWindowImage(POS_RIGHT_WINDOW_X-100,
					nWindowPosY+MODE_COMBO_START_Y+(1*MODE_COMBO_INTERVAL)+12, strlen(strBuf)*6.5, 1);
				m_pFontToolTip->DrawText(POS_RIGHT_WINDOW_X-100, nWindowPosY+MODE_COMBO_START_Y+(1*MODE_COMBO_INTERVAL)+12,
					GUI_FONT_COLOR,strBuf);
			}
		}		
		break;
	}
}
		
			

			/*
			wsprintf( m_strSelectModeComboData[0], "%d*%d", g_pD3dApp->m_nWidth, g_pD3dApp->m_nHeight);
			if(m_strSelectModeComboData[0][0])
			{
				m_pFontSelectOptionComboData[0]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X,
					nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*0,RGB(128,128,128), m_pFontSelectOptionComboData[0], 0L);
			}

			for(int i=0;i<MODE_COMBO_NUMBER;i++)
			{
				if(m_strSelectModeComboData[i][0])
				{
					m_pFontSelectModeComboData[i]->DrawText(POS_RIGHT_WINDOW_X+MODE_COMBO_START_X,
						nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*i,RGB(128,128,128), m_strSelectModeComboData[i], 0L);
				}
				else if(m_strSelectModeComboData[i][1])
				{
					//시점					

					if(g_pSOption->sEyePt == 0)
						TempSelectComboData = MODE_CHANGE_PS_0 ;
					if(g_pSOption->sEyePt == 1)
						TempSelectComboData = MODE_CHANGE_PS_1 ;

					m_pFontSelectOptionComboData[1]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X,
						nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*1,RGB(128,128,128), TempSelectComboData, 0L);
					
					RenderComboOptionList(nWindowPosY);
				}
				else if(m_strSelectModeComboData[i][2])
				{
					//배경음 조절
					if(g_pSOption->sMusicVolume == -10000)
						TempSelectComboData= "0" ;
					if(g_pSOption->sMusicVolume == -3500)
						TempSelectComboData= "1" ;
					if(g_pSOption->sMusicVolume == -3000)
						TempSelectComboData= "2" ;
					if(g_pSOption->sMusicVolume == -2500)
						TempSelectComboData= "3" ;
					if(g_pSOption->sMusicVolume == -2000)
						TempSelectComboData= "4" ;
					if(g_pSOption->sMusicVolume == -1500)
						TempSelectComboData= "5" ;
					if(g_pSOption->sMusicVolume == -1000)
						TempSelectComboData= "6" ;
					if(g_pSOption->sMusicVolume == -600)
						TempSelectComboData= "7" ;
					if(g_pSOption->sMusicVolume == -300)
						TempSelectComboData= "8" ;
					if(!g_pSOption->sMusicVolume)
						TempSelectComboData= "9" ;
					
					m_pFontSelectOptionComboData[0]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X,
						nWindowPosY+OPTION_COMBO_MIDDLE_Y+OPTION_COMBO_INTERVAL*1,RGB(128,128,128), TempSelectComboData, 0L);					
					RenderComboOptionList(nWindowPosY);

					break;
				}
				else if(m_strSelectModeComboData[i][3])
				{
					//효과음
					if(g_pSOption->sSoundVolume == -10000)
						TempSelectComboData= "0" ;
					if(g_pSOption->sSoundVolume == -3500)
						TempSelectComboData= "1" ;
					if(g_pSOption->sSoundVolume == -3000)
						TempSelectComboData= "2" ;
					if(g_pSOption->sSoundVolume == -2500)
						TempSelectComboData= "3" ;
					if(g_pSOption->sSoundVolume == -2000)
						TempSelectComboData= "4" ;
					if(g_pSOption->sSoundVolume == -1500)
						TempSelectComboData= "5" ;
					if(g_pSOption->sSoundVolume == -1000)
						TempSelectComboData= "6" ;
					if(g_pSOption->sSoundVolume == -600)
						TempSelectComboData= "7" ;
					if(g_pSOption->sSoundVolume == -300)
						TempSelectComboData= "8" ;
					if(!g_pSOption->sSoundVolume)
						TempSelectComboData= "9" ;
					
					m_pFontSelectOptionComboData[1]->DrawText(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X,
						nWindowPosY+OPTION_COMBO_MIDDLE_Y+OPTION_COMBO_INTERVAL*0,RGB(128,128,128), TempSelectComboData, 0L);
					RenderComboOptionList(nWindowPosY);
					
					break;
				}
				else if(m_strSelectModeComboData[i][4])
				{
					//조작
					if(g_pSOption->sHandle)
						strcpy(m_strSelectModeComboData[3], MODE_CHANGE_MACHINE_0);
					else
						strcpy(m_strSelectModeComboData[3],  MODE_CHANGE_MACHINE_0);

					m_pFontSelectModeComboData[i]->DrawText(POS_RIGHT_WINDOW_X+MODE_COMBO_START_X,
					nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*i,RGB(128,128,128), m_strSelectModeComboData[i], 0L);

					RenderComboModeList(nWindowPosY);
				}
				else if(m_strSelectModeComboData[i][5])
				{
					//도움말
					if(g_pSOption->sHelpDesk)
						strcpy(m_strSelectModeComboData[4], MODE_CHANGE_PS_0);
					else
						strcpy(m_strSelectModeComboData[4], MODE_CHANGE_PS_1);

					m_pFontSelectModeComboData[i]->DrawText(POS_RIGHT_WINDOW_X+MODE_COMBO_START_X,
					nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*i,RGB(128,128,128), m_strSelectModeComboData[i], 0L);

					RenderComboModeList(nWindowPosY);
				}					
			}			
		} */	


void CINFSystem::RenderComboModeList(int nWindowPosY)
{
	FLOG( "CINFSystem::RenderComboModeList(int nWindowPosY)" );

	if(m_bShowModeListBox[MODE_CHANGE_PS_COMBO])
	{
		int x = POS_RIGHT_WINDOW_X+MODE_COMBO_START_X;
		int y = nWindowPosY+MODE_COMBO_START_Y;
		RenderComboModeListSingle(0, MODE_CHANGE_PS_0, x, y+MODE_COMBO_SIZE_Y*1);
		RenderComboModeListSingle(1, MODE_CHANGE_PS_1, x, y+MODE_COMBO_SIZE_Y*2);		
	}
	else if(m_bShowModeListBox[MODE_CHANGE_SOUND_COMBO])
	{
		int x = POS_RIGHT_WINDOW_X+MODE_COMBO_START_X;
		int y = nWindowPosY+MODE_COMBO_START_Y+MODE_COMBO_INTERVAL*1;
		RenderComboModeListSingle(0, "0", x, y+OPTION_COMBO_HEIGHT*1);
		RenderComboModeListSingle(1, "1", x, y+OPTION_COMBO_HEIGHT*2);
		RenderComboModeListSingle(2, "2", x, y+OPTION_COMBO_HEIGHT*3);
		RenderComboModeListSingle(3, "3", x, y+OPTION_COMBO_HEIGHT*4);
		RenderComboModeListSingle(4, "4", x, y+OPTION_COMBO_HEIGHT*5);
		RenderComboModeListSingle(5, "5", x, y+OPTION_COMBO_HEIGHT*6);
		RenderComboModeListSingle(6, "6", x, y+OPTION_COMBO_HEIGHT*7);
		RenderComboModeListSingle(7, "7", x, y+OPTION_COMBO_HEIGHT*8);
		RenderComboModeListSingle(8, "8", x, y+OPTION_COMBO_HEIGHT*9);
		RenderComboModeListSingle(9, "9", x, y+OPTION_COMBO_HEIGHT*10);
	}

	
}

int CINFSystem::ModeWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSystem::ModeWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
			m_bRenderToolTip = FALSE;

			if( pt.x>POS_RIGHT_WINDOW_X+32 &&
				pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
				pt.y>nWindowPosY+MODE_COMBO_START_Y+(MODE_CHANGE_MACHINE_COMBO*MODE_COMBO_INTERVAL) &&
				pt.y<nWindowPosY+MODE_COMBO_START_Y+(MODE_CHANGE_MACHINE_COMBO*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
			{
				if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE )
				{
					m_bRenderToolTip = TRUE;
				}
			}
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			
			int i;
			for(i=0; i<MODE_COMBO_NUMBER;i++)
			{
				if(m_bShowModeListBox[i])
				{
					if(!m_bShowModeListBox[MODE_CHANGE_MACHINE_COMBO])
						return ModeComboProc(i, pt.x, pt.y, nWindowPosY);
				}
			}


			// 2007-07-23 by bhsohn 오퍼레이터 추가작업
			//for(i=0 ;i<OPTION_COMBO_NUMBER ;i++)
			for(i=0 ;i<MAX_MODE_CHANGE;i++)				
			{
				if( i == MODE_CHANGE_MACHINE_COMBO )
				{ // 타겟 보이기 감추기
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{
						if(IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == FALSE )
						{
							g_pSOption->sHandle = !g_pSOption->sHandle;
							g_pInterface->SaveOptionFile();
						}
						else
						{
							g_pSOption->sHandle = TRUE;
						}
					}
				}
				else if( i == MODE_CHANGE_HELPDESK_COMBO )
				{ // 헬프기능
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{						
						g_pSOptionCharacter->sHelpDesk = !g_pSOptionCharacter->sHelpDesk;		
//						g_pInterface->SaveCharacterFile();			// 2006-09-05 by ispark
					}
				}
				// 2007-07-23 by bhsohn 오퍼레이터 추가작업
				else if( i == MODE_CHANGE_OPEATOR )
				{
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{	
						int nOpMode = g_pInterface->GetOperatorMode();
						if(g_pInterface->GetOperatorMode())
						{
							nOpMode = FALSE;
						}
						else
						{
							nOpMode = TRUE;
						}
						g_pInterface->SetOperatorMode(nOpMode);
						g_pInterface->SaveOptionFile();
					}					
				}
				// end 2007-07-23 by bhsohn 오퍼레이터 추가작업
				else if( i == MODE_CHANGE_REVERSE_LR )
				{ // 상하 반전
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{						
						g_pSOption->sMouseReversLeft = !g_pSOption->sMouseReversLeft;
						g_pInterface->SaveOptionFile();
						if(!g_pD3dApp->IsWindowMode())
						{
							CheckMouseReverse(&pt);
							SetCursorPos(pt.x,pt.y);
						}
					}
				}
				else if( i == MODE_CHANGE_REVERSE_UD )
				{ // 좌우 반전
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{							
						g_pSOption->sMouseReversUp = !g_pSOption->sMouseReversUp;
						g_pInterface->SaveOptionFile();
						if(!g_pD3dApp->IsWindowMode())
						{
							CheckMouseReverse(&pt);
							SetCursorPos(pt.x,pt.y);
						}
					}
				}
				else if( i == MODE_CHANGE_AUTOBALANCE )
				{ // 자동 수평유지
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{						
						g_pSOption->sAutoBalance = !g_pSOption->sAutoBalance;		
						g_pInterface->SaveOptionFile();
					}
				}
				// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
				//else if( i == MODE_CHANGE_MENU_BALACE )
				else if( i == MODE_CHANGE_PET_OP_BALACE )
				// end 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
				{ // 메뉴사용 수평유지
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{						
						// 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
						//g_pSOption->sMenuBalance = !g_pSOption->sMenuBalance;
						g_pSOption->sPetOPBalance = !g_pSOption->sPetOPBalance;
						// end 2010. 10. 26 by jskim 펫 오퍼레이터 옵션 처리
						g_pInterface->SaveOptionFile();
					}
				}
#ifdef _WARRIOR_ADDITIONAL_INFO
				else if (i == MODE_CHANGE_WSW_AIMINFO)
				{
					if (pt.x>POS_RIGHT_WINDOW_X + 32 &&
						pt.x<POS_RIGHT_WINDOW_X + 186 &&
						pt.y>nWindowPosY + MODE_COMBO_START_Y + (i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY + MODE_COMBO_START_Y + (i*MODE_COMBO_INTERVAL) + MODE_COMBO_SIZE_Y)
					{
						
						g_pSOption->sWSWAddAIMInfo = !g_pSOption->sWSWAddAIMInfo;

						g_pInterface->SaveOptionFile();
					}
				}
#endif
#ifdef _SHOW_GUILD_NAME
				else if (i == MODE_CHANGE_SHOW_GUILDNAME)
				{
					if (pt.x>POS_RIGHT_WINDOW_X + 32 &&
						pt.x<POS_RIGHT_WINDOW_X + 186 &&
						pt.y>nWindowPosY + MODE_COMBO_START_Y + (i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY + MODE_COMBO_START_Y + (i*MODE_COMBO_INTERVAL) + MODE_COMBO_SIZE_Y)
					{

						g_pSOption->sShowGuildName = !g_pSOption->sShowGuildName;

						g_pInterface->SaveOptionFile();
					}
				}
#endif
				else if( i == MODE_CHANGE_MP3_MUSIC )
				{ // 뮤직플레이어 사용
					if( pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{						
						g_pSOption->sMp3Player = !g_pSOption->sMp3Player;
						g_pGameMain->m_bMp3PlayFlag = g_pSOption->sMp3Player;
						g_pInterface->SaveOptionFile();
					}
				}				
				else
				{
					if( pt.x>POS_RIGHT_WINDOW_X+118 &&
						pt.x<POS_RIGHT_WINDOW_X+186 &&//15:콤보박스 역삼각형버튼
						pt.y>nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL) &&
						pt.y<nWindowPosY+MODE_COMBO_START_Y+(i*MODE_COMBO_INTERVAL)+MODE_COMBO_SIZE_Y)
					{
						m_bShowModeListBox[i] = !m_bShowModeListBox[i];
					}
					else
					{
						m_bShowModeListBox[i] = FALSE;
					}
				}
			}	
			
			// 2005-08-02 by ispark
			// 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}			
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int CINFSystem::SystemWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSystem::SystemWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			if(pt.x > POS_RIGHT_WINDOW_X+73  &&
			   pt.x < POS_RIGHT_WINDOW_X+73 + SIZE_SYSTEM_BUTTON_X)
			{
				if(pt.y > nWindowPosY + 116 && pt.y < nWindowPosY + 116 + SIZE_SYSTEM_BUTTON_Y)
				{
					if(m_nSystemButtonState[0] != BUTTON_STATE_DOWN)
						m_nSystemButtonState[0] = BUTTON_STATE_UP;
				}
				else
				{
					m_nSystemButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if(pt.y > nWindowPosY + 144 && pt.y < nWindowPosY + 144 + SIZE_SYSTEM_BUTTON_Y)
				{
					if(m_nSystemButtonState[1] != BUTTON_STATE_DOWN)
						m_nSystemButtonState[1] = BUTTON_STATE_UP;
				}
				else
				{
					m_nSystemButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}
			else
			{
				for(int i=0;i<2;i++)
					m_nSystemButtonState[i] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			if(pt.x > POS_RIGHT_WINDOW_X+73  &&
			   pt.x < POS_RIGHT_WINDOW_X+73 + SIZE_SYSTEM_BUTTON_X)
			{
				if(pt.y > nWindowPosY + 116 && pt.y < nWindowPosY + 116 + SIZE_SYSTEM_BUTTON_Y)
				{
					m_nSystemButtonState[0] = BUTTON_STATE_DOWN;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_CANCEL, D3DXVECTOR3(0,0,0), FALSE);
				}
				else
				{
					m_nSystemButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if(pt.y > nWindowPosY + 144 && pt.y < nWindowPosY + 144 + SIZE_SYSTEM_BUTTON_Y)
				{
					m_nSystemButtonState[1] = BUTTON_STATE_DOWN;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_BUTTON_CANCEL, D3DXVECTOR3(0,0,0), FALSE);
				}
				else
				{
					m_nSystemButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}

			// 2005-08-03 by ispark
			// 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			if( g_pShuttleChild->m_nEventType != EVENT_GAME_END &&
				pt.x > POS_RIGHT_WINDOW_X+73  &&
			    pt.x < POS_RIGHT_WINDOW_X+73 + SIZE_SYSTEM_BUTTON_X)
			{
				if(pt.y > nWindowPosY + 116 && pt.y < nWindowPosY + 116 + SIZE_SYSTEM_BUTTON_Y)
				{
					if(m_nSystemButtonState[0] == BUTTON_STATE_DOWN)
					{
						OnSystemButtonClick(0);
					}
					m_nSystemButtonState[0] = BUTTON_STATE_UP;
				}
				else
				{
					m_nSystemButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if(pt.y > nWindowPosY + 144 && pt.y < nWindowPosY + 144 + SIZE_SYSTEM_BUTTON_Y)
				{
					if(m_nSystemButtonState[1] == BUTTON_STATE_DOWN)
					{
						OnSystemButtonClick(1);
					}
					m_nSystemButtonState[1] = BUTTON_STATE_UP;
				}
				else
				{
					m_nSystemButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int CINFSystem::OptionWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSystem::OptionWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
//			POINT pt = MAKEPOINT(lParam);

		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;
//			int nWindowPosY = POS_RIGHT_WINDOW_Y;
//			if(nWindowPosY < SIZE_ROLL_Y )
//				nWindowPosY = SIZE_ROLL_Y;
//			if(nWindowPosY > RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y)
//				nWindowPosY = RIGHT_WINDOW_MAX_Y-SIZE_NORMAL_WINDOW_Y;
			int i;
			for(i=0; i<(OPTION_COMBO_NUMBER-1);i++)
			{
				if(m_bShowOptionListBox[i])
				{					
					return OptionComboProc(i, pt.x, pt.y, nWindowPosY);
				}
			}


			for(i=1 ;i<OPTION_COMBO_NUMBER ;i++)//	if(nOption == OPTION_MODE_COMBO) 는 아직 지원하지 않는다.
			{
				int tempY = 70;
				
				if(i == OPTION_QUALITY_COMBO)
					tempY += 4;
				if(i == OPTION_QUALITY_COMBO )
				{
					if(pt.x>POS_RIGHT_WINDOW_X+32 &&
						pt.x<POS_RIGHT_WINDOW_X+186
						&& pt.y>nWindowPosY+tempY+(i*20) && pt.y<nWindowPosY+tempY+(i*20)+13)
					{
//						if(i == OPTION_REFLACTIVE_COMBO)
//						{
//							g_pSOption->sReflactive = !g_pSOption->sReflactive;
//							g_pInterface->SaveOptionFile();
//							break;
//						}
						if(i == OPTION_QUALITY_COMBO)
						{
							OptionQualityCombo();

							break;
						}						
					}
					else
					{
						m_bShowOptionListBox[i] = FALSE;
					}
				}
				else
				{
					if(pt.x>POS_RIGHT_WINDOW_X+120 &&
						pt.x<POS_RIGHT_WINDOW_X+186
						&& pt.y>nWindowPosY+tempY+(i*20) && pt.y<nWindowPosY+tempY+(i*20)+13)
					{						
						m_bShowOptionListBox[i] = !m_bShowOptionListBox[i];
					}
					else
					{
						m_bShowOptionListBox[i] = FALSE;
					}
				}
			}		

			// 2005-08-02 by ispark
			// 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				return INF_MSGPROC_BREAK;
			}

			//for( i=0 ;i<2 ;i++)
			//{
			//	if(pt.x>POS_RIGHT_WINDOW_X+110 &&
			//		pt.x<POS_RIGHT_WINDOW_X+186
			//		&& pt.y>nWindowPosY+180+(i+1)*20 && pt.y<nWindowPosY+180+(i+1)*20+13)
			//	{
			//		m_bShowOptionListBox[i+5] = !m_bShowOptionListBox[i+5];
			//	}
			//	else
			//	{
			//		m_bShowOptionListBox[i+5] = FALSE;
			//	}
			//}


		}
		break;
	case WM_LBUTTONUP:
		{
	//		POINT pt = MAKEPOINT(lParam);
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

void CINFSystem::OnSystemButtonClick(int nButton)
{
	FLOG( "CINFSystem::OnSystemButtonClick(int nButton)" );
	DWORD dwCurTick = timeGetTime();
	
	// 2004-12-17 by jschoi - 워프중에는 종료를 할 수 없다.
	if( g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_OUT ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_WARP_IN  ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_CITY_OUT ||
		g_pShuttleChild->m_nCurrentPatternNumber == PATTERN_UNIT_CITY_IN  )
	{
		return;
	}
	// 2006-08-02 by ispark, 개인 상점
	if(g_pInterface->m_pBazaarShop)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060802_0003,COLOR_ERROR);// "개인상점 이용중에는 사용 할 수 없습니다."
		return;
	}
	
	switch(nButton)
	{
	case 0://선택
		{
			m_bQuitGameMessage = TRUE;
			m_bLogOffGameMessage = TRUE;
			m_fLastTickSecTime = dwCurTick;
			g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);
			g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_INTERFACE_0040, _Q_QUIT_MESSAGE);//"로그오프까지 5초 남았습니다."
			
//			if(dwCurTick - g_pShuttleChild->GetLastAttackTick() < END_GAME_TIMING_ATTACK) 
//			{ 
//				char message[256]; 
//				wsprintf(message,"전투시에는 로그오프 할 수 없습니다."); 
//				g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM);	
//				return;			 
//			} 
//			g_pD3dApp->m_bClientQuit = FALSE;
		}
		break;
	case 1://종료
		{
//			if(dwCurTick - g_pShuttleChild->GetLastAttackTick() < END_GAME_TIMING_ATTACK) 
//			{ 
//				char message[256]; 
//				wsprintf(message,"전투시에는 종료 할 수 없습니다."); 
//				g_pD3dApp->m_pChat->CreateChatChild(message,COLOR_SYSTEM); 
//				return;
//			} 
//			g_pD3dApp->m_bClientQuit = TRUE;	// GAME END 메세지 처리후 종료하기 위해 셋팅
			m_bQuitGameMessage = TRUE;
			m_fLastTickSecTime = dwCurTick;
			g_pGameMain->RightWindowShow(FALSE,g_pGameMain->m_nRightWindowInfo);
			g_pGameMain->LeftWindowShow(FALSE,g_pGameMain->m_nLeftWindowInfo);
			g_pGameMain->m_pInfWindow->AddMsgBox(STRMSG_C_INTERFACE_0041, _Q_QUIT_MESSAGE);//"게임종료까지 5초 남았습니다."
			return;
		}
		break;
	}
	// 시네마
//	if( g_pD3dApp->m_dwGameState == _CITY )
//	{
//		SendFieldSocketCharacterGameEnd();
//	}
//	else
//	{
//		if( g_pShuttleChild->InitCinemaCamera(PATTERN_CAMERA_GAME_END) == TRUE )
//		{
//			g_pD3dApp->StartFadeEffect(FALSE,1,D3DCOLOR_ARGB(0,0,0,0));
//			g_pShuttleChild->m_nEventType = EVENT_GAME_END;
//			g_pShuttleChild->ChangeUnitState( _WARP );
//		}
//	}
//	g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_M_A_GEAR );
//	g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
//	g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_B_I_GEAR );
//	g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );
}

void CINFSystem::SendFieldSocketCharacterGameEnd()
{
	if(g_pD3dApp->m_dwGameState != _WAITING)
	{
		g_pStoreData->SendFieldSocketItemIndexInfo();
		MSG_FC_CHARACTER_GAMEEND	sMsg;
		// 2007-11-22 by bhsohn 아레나 통합서버
		//sMsg.ClientIndex = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.ClientIndex;
		CHARACTER* pMainInfo = g_pD3dApp->GetMFSMyShuttleInfo();
		sMsg.ClientIndex =pMainInfo->ClientIndex;

		// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		sMsg.SelectCharacterView	= m_bLogOffGameMessage;	// 로그오프 여부
		// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

		g_pFieldWinSocket->SendMsg(T_FC_CHARACTER_GAMEEND, (char*)&sMsg, sizeof(sMsg) );
		g_pD3dApp->ChangeGameState(_WAITING);
	}
}

int CINFSystem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSystem::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			CheckMouseReverse(&pt);
			int nWindowPosY = g_pGameMain->m_nRightWindowY;

//			// 우선 순위 지정
//			if(pt.x>POS_RIGHT_WINDOW_X && pt.x<219 && pt.y > g_pGameMain->m_nLeftWindowY && pt.y <g_pGameMain->m_nLeftWindowY +275)
//				g_pInterface->SetWindowOrder(WNDLeftRightWindow);

			if(pt.y>nWindowPosY + 30 && pt.y<nWindowPosY + 46)
			{
				// ydkim 2005.09.29 옵션 리스트 삭제			
				int i;
				for(i=0; i<MODE_COMBO_NUMBER; i++){
					m_bShowModeListBox[i] = FALSE;
				}
				for(i=0; i<OPTION_COMBO_NUMBER; i++){
					m_bShowOptionListBox[i] = FALSE;
				}

				if(pt.x>POS_RIGHT_WINDOW_X+23 &&
				   pt.x<POS_RIGHT_WINDOW_X+60 )
				{
					m_nSystemType = SYSTEM_SYSTEM;
//					return INF_MSGPROC_NORMAL;
					return INF_MSGPROC_BREAK;			// 2005-08-03 by ispark
				}
				if(pt.x>POS_RIGHT_WINDOW_X+61 &&
				   pt.x<POS_RIGHT_WINDOW_X+104 )
				{
					m_nSystemType = SYSTEM_OPTION;
//					return INF_MSGPROC_NORMAL;
					return INF_MSGPROC_BREAK;			// 2005-08-03 by ispark
				}
				if(pt.x>POS_RIGHT_WINDOW_X+105 &&
				   pt.x<POS_RIGHT_WINDOW_X+158 )
				{
					m_nSystemType = SYSTEM_MODE;
//					return INF_MSGPROC_NORMAL;
					return INF_MSGPROC_BREAK;			// 2005-08-03 by ispark
				}
			}

			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>POS_RIGHT_WINDOW_X &&
				pt.x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
			{
				g_pInterface->SetWindowOrder(WNDLeftRightWindow);
			}

		}
		break;
	case WM_RBUTTONDOWN:
		{
			// 2007-11-28 by bhsohn 스크린 샷 모드 추가
//#ifdef _SCREENSHOT
			//OptionQualityCombo();			
//#endif
			if(g_pInterface->IsScreenShotMode())
			{
				OptionQualityCombo();
			}
			// end 2007-11-28 by bhsohn 스크린 샷 모드 추가
	
//			m_wGamma += 8 ;
//			UpdateGamma() ;
		}
		break;


	case WM_LBUTTONUP:
		{
		}
		break;
	}
	switch(m_nSystemType)
	{
	case SYSTEM_SYSTEM:
		{
			if(SystemWndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;				// 2005-08-03 by ispark			
		}
		break;
	case SYSTEM_OPTION:
		{
			if(OptionWndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;				// 2005-08-03 by ispark
		}
		break;
	case SYSTEM_MODE:
		{
			if(ModeWndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
				return INF_MSGPROC_BREAK;				// 2005-08-03 by ispark
		}
		break;
	}
	return INF_MSGPROC_NORMAL;

}

int CINFSystem::OptionComboProc(int nOption, int x, int y, int nWindowPosY)
{
	FLOG( "CINFSystem::OptionComboProc(int nOption, int x, int y, int nWindowPosY)" );
	if(nOption == OPTION_MODE_COMBO)
		return INF_MSGPROC_NORMAL;
	if(x>POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X &&
		x<POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+OPTION_COMBO_WIDTH)
	{		
		int index = (y-nWindowPosY-OPTION_COMBO_START_Y-(OPTION_COMBO_INTERVAL*nOption)
			-OPTION_COMBO_HEIGHT)/OPTION_COMBO_HEIGHT;
		if( index>=0 && index<OPTION_COMBO_LIST_NUMBER &&
			y>nWindowPosY+OPTION_COMBO_START_Y+(OPTION_COMBO_INTERVAL*nOption)+OPTION_COMBO_HEIGHT)
		{
			//if(nOption == OPTION_SHADOW_COMBO && index > 1)
			//{
			//	m_bShowOptionListBox[nOption] = FALSE;
			//	return INF_MSGPROC_NORMAL;
			//}
			m_nOptionInfo[nOption] = index;
			GetOptionString(m_strSelectOptionComboData[nOption], nOption, index);
			g_pInterface->SaveOptionFile();
		}		
	}

	m_bShowOptionListBox[nOption] = FALSE;					// 2005-10-28 by ispark 창 닫음

	// 2005-08-02 by ispark
	// 창안에 마우스 클릭시 무효
	if((y>nWindowPosY &&
		y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
		x>POS_RIGHT_WINDOW_X &&
		x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X) ||
		m_nOptionInfo[nOption] >= 0)
	{
		return INF_MSGPROC_BREAK;
	}

	return INF_MSGPROC_NORMAL;
}

int CINFSystem::ModeComboProc(int nOption, int x, int y, int nWindowPosY)
{
	FLOG( "CINFSystem::ModeComboProc(int nOption, int x, int y, int nWindowPosY)" );
	if(x>POS_RIGHT_WINDOW_X+MODE_COMBO_START_X &&
		x<POS_RIGHT_WINDOW_X+MODE_COMBO_START_X+MODE_COMBO_SIZE_X)
	{
		int index = (y-nWindowPosY-MODE_COMBO_START_Y-(MODE_COMBO_INTERVAL*nOption)
					 -MODE_COMBO_SIZE_Y)/MODE_COMBO_SIZE_Y;
		if( index>=0 && index<OPTION_COMBO_LIST_NUMBER &&
			y>nWindowPosY+MODE_COMBO_START_Y+(OPTION_COMBO_INTERVAL*nOption)+OPTION_COMBO_HEIGHT)
		{
			m_nModeInfo[nOption] = index;
			GetModeString(m_strSelectModeComboData[nOption], nOption, index);
			g_pInterface->SaveOptionFile();
		}
	}

	m_bShowModeListBox[nOption] = FALSE;					// 2005-10-28 by ispark 창 닫음

	// 2005-08-02 by ispark
	// 창안에 마우스 클릭시 무효
	if(y>nWindowPosY &&
		y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
		x>POS_RIGHT_WINDOW_X &&
		x<POS_RIGHT_WINDOW_X + SIZE_NORMAL_WINDOW_X)
	{
		return INF_MSGPROC_BREAK;
	}

	return INF_MSGPROC_NORMAL;
}

#define MODE_CAMERA_TYPE_FPS		1
#define MODE_CAMERA_TYPE_NORMAL		0

#define MODE_SHUTTLE_TYPE_MOUSE		1
#define MODE_SHUTTLE_TYPE_KEYBOARD	0

void CINFSystem::GetModeString(char* strData, int nOption, int index)
{
	FLOG( "CINFSystem::GetModeString(char* strData, int nOption, int index)" );
	switch(nOption)
	{
	case MODE_CHANGE_PS_COMBO:// 1인칭, 3인칭
		{
			if(index == 0)
			{
				strcpy(strData, MODE_CHANGE_PS_0);
				g_pSOption->sEyePt = TRUE ;
				if(g_pShuttleChild->m_dwState == _LANDED || g_pShuttleChild->m_bIsAir == FALSE)
				{
					g_pShuttleChild->m_bLandingOldCamtypeFPS = TRUE;
				}
				else
				{
					g_pD3dApp->m_pCamera->SetCamType( MODE_CAMERA_TYPE_FPS );
				}
			}
			else if(index == 1)
			{
				strcpy(strData, MODE_CHANGE_PS_1);
				g_pSOption->sEyePt = FALSE ;
				if(g_pShuttleChild->m_bLandingOldCamtypeFPS != TRUE)
				{
					g_pD3dApp->m_pCamera->SetCamType( MODE_CAMERA_TYPE_NORMAL );
				}
				
			}
		}
		break;
//	case MODE_CHANGE_MUSIC_COMBO:// 음악볼륨
//		{
//			if(index == 0)
//			{
//				g_pSOption->sMusicVolume = -10000 ;
//				strcpy(strData, OPTION_MUSIC_0);
//			}
//			else if(index == 1)
//			{
//				g_pSOption->sMusicVolume = -3500 ;
//				strcpy(strData, OPTION_MUSIC_1);
//			}
//			else if(index == 2)
//			{
//				g_pSOption->sMusicVolume = -3000 ;
//				strcpy(strData, OPTION_MUSIC_2);
//			}
//			else if(index == 3)
//			{
//				g_pSOption->sMusicVolume = -2500 ;
//				strcpy(strData, OPTION_MUSIC_3);
//			}
//			else if(index == 4)
//			{
//				g_pSOption->sMusicVolume = -2000 ;
//				strcpy(strData, OPTION_MUSIC_4);
//			}
//			else if(index == 5)
//			{
//				g_pSOption->sMusicVolume = -1500 ;
//				strcpy(strData, OPTION_MUSIC_5);
//			}
//			else if(index == 6)
//			{
//				g_pSOption->sMusicVolume = -1000 ;
//				strcpy(strData, OPTION_MUSIC_6);
//			}
//			else if(index == 7)
//			{
//				g_pSOption->sMusicVolume = -600 ;
//				strcpy(strData, OPTION_MUSIC_7);
//			}
//			else if(index == 8)
//			{
//				g_pSOption->sMusicVolume = -300 ;
//				strcpy(strData, OPTION_MUSIC_8);
//			}
//			else if(index == 9)
//			{
//				g_pSOption->sMusicVolume = 0 ;
//				strcpy(strData, OPTION_MUSIC_9);
//			}
//
//			g_pD3dApp->m_pSound->SetAtumMusicVolume(g_pSOption->sMusicVolume) ;
//		}
//		break;
	case MODE_CHANGE_SOUND_COMBO:// 음악볼륨
		{
			if(index == 0)
			{
				g_pSOption->sSoundVolume = -10000 ;
				strcpy(strData, OPTION_SOUND_0);
			}
			else if(index == 1)
			{
				g_pSOption->sSoundVolume = -3500 ;
				strcpy(strData, OPTION_SOUND_1);
			}
			else if(index == 2)
			{
				g_pSOption->sSoundVolume = -3000 ;
				strcpy(strData, OPTION_SOUND_2);
			}
			else if(index == 3)
			{
				g_pSOption->sSoundVolume = -2500 ;
				strcpy(strData, OPTION_SOUND_3);
			}
			else if(index == 4)
			{
				g_pSOption->sSoundVolume = -2000 ;
				strcpy(strData, OPTION_SOUND_4);
			}
			else if(index == 5)
			{
				g_pSOption->sSoundVolume = -1500 ;
				strcpy(strData, OPTION_SOUND_5);
			}
			else if(index == 6)
			{
				g_pSOption->sSoundVolume = -1000 ;
				strcpy(strData, OPTION_SOUND_6);
			}
			else if(index == 7)
			{
				g_pSOption->sSoundVolume = -600 ;
				strcpy(strData, OPTION_SOUND_7);
			}
			else if(index == 8)
			{
				g_pSOption->sSoundVolume = -300 ;
				strcpy(strData, OPTION_SOUND_8);
			}
			else if(index == 9)
			{
				g_pSOption->sSoundVolume = 0 ;
				strcpy(strData, OPTION_SOUND_9);
			}
		}
		break;
	case MODE_CHANGE_MACHINE_COMBO:// 디폴트, 조이스틱 ( 아직 지원 안함)
		{
//			if(index == 0)
//			{
//				g_pSOption->sHandle = FALSE ;
//				strcpy(strData, MODE_CHANGE_MACHINE_0);
//			}
//			else if(index == 1)
//			{
//				g_pSOption->sHandle = TRUE ;
//				strcpy(strData, MODE_CHANGE_MACHINE_1);
//			}
		}
		break;
	}
}

void CINFSystem::GetOptionString(char* strData, int nOption, int index)
{
	FLOG( "CINFSystem::GetOptionString(char* strData, int nOption, int index)" );
	switch(nOption)
	{
	case OPTION_MODE_COMBO:
		{
			if(index == 0) strcpy(strData, OPTION_MODE_0);
			else if(index == 1) strcpy(strData, OPTION_MODE_1);
			else if(index == 2) strcpy(strData, OPTION_MODE_2);
			else if(index == 3) strcpy(strData, OPTION_MODE_3);
			else if(index == 4) strcpy(strData, OPTION_MODE_4);
			else if(index == 5) strcpy(strData, OPTION_MODE_5);
			else if(index == 6) strcpy(strData, OPTION_MODE_6);
			else if(index == 7) strcpy(strData, OPTION_MODE_7);
			else if(index == 8) strcpy(strData, OPTION_MODE_8);
			else if(index == 9) strcpy(strData, OPTION_MODE_9);
		}
		break;
	case OPTION_SHADOW_COMBO:
		{
			if(index == 0)
			{
				g_pSOption->sShadowState = 0 ;
				strcpy(strData, OPTION_SHADOW_0);
			}
			else if(index == 1)
			{
				g_pSOption->sShadowState = 1 ;
				strcpy(strData, OPTION_SHADOW_1);
			}
			else if(index == 2)
			{
				g_pSOption->sShadowState = 2 ;
				strcpy(strData, OPTION_SHADOW_2);
			}
			else if(index == 3)
			{
				g_pSOption->sShadowState = 3 ;
				strcpy(strData, OPTION_SHADOW_3);
			}
			else if(index == 4)
			{
				g_pSOption->sShadowState = 4 ;
				strcpy(strData, OPTION_SHADOW_4);
			}
			else if(index == 5)
			{
				g_pSOption->sShadowState = 5 ;
				strcpy(strData, OPTION_SHADOW_5);
			}
			else if(index == 6)
			{
				g_pSOption->sShadowState = 6 ;
				strcpy(strData, OPTION_SHADOW_6);
			}
			else if(index == 7)
			{
				g_pSOption->sShadowState = 7 ;
				strcpy(strData, OPTION_SHADOW_7);
			}
			else if(index == 8)
			{
				g_pSOption->sShadowState = 8 ;
				strcpy(strData, OPTION_SHADOW_8);
			}
			else if(index == 9)
			{
				g_pSOption->sShadowState = 9 ;
				strcpy(strData, OPTION_SHADOW_9);
			}
		}
		break;
	case OPTION_GAMMA_COMBO:
		{
			if(index == 0)
			{
				g_pSOption->sGammaCtrl = 5 ;
				strcpy(strData, OPTION_GAMMA_0);
			}
			else if(index == 1)
			{
				g_pSOption->sGammaCtrl = 4 ;
				strcpy(strData, OPTION_GAMMA_1);
			}
			else if(index == 2)
			{
				g_pSOption->sGammaCtrl = 3 ;
				strcpy(strData, OPTION_GAMMA_2);
			}
			else if(index == 3)
			{
				g_pSOption->sGammaCtrl = 2 ;
				strcpy(strData, OPTION_GAMMA_3);
			}
			else if(index == 4)
			{
				g_pSOption->sGammaCtrl = 1;
				strcpy(strData, OPTION_GAMMA_4);
			}
			else if(index == 5)
			{
				g_pSOption->sGammaCtrl = 0;
				strcpy(strData, OPTION_GAMMA_5);
			}
			else if(index == 6)
			{
				g_pSOption->sGammaCtrl = -1;
				strcpy(strData, OPTION_GAMMA_6);
			}
			else if(index == 7)
			{
				g_pSOption->sGammaCtrl = -2;
				strcpy(strData, OPTION_GAMMA_7);
			}
			else if(index == 8)
			{
				g_pSOption->sGammaCtrl = -3;
				strcpy(strData, OPTION_GAMMA_8);
			}
			else if(index == 9)
			{
				g_pSOption->sGammaCtrl = -4;
				strcpy(strData, OPTION_GAMMA_9);
			}

		//	g_pSOption->sGammaCtrl = atoi(m_strSelectOptionComboData[nOption]);
			UpdateGamma(g_pSOption->sGammaCtrl);
		}
		break;
	case OPTION_OBJ_COMBO:
		{
			if(index == 0)
			{
				g_pSOption->sTerrainEffectRender = 0 ;
				strcpy(strData, OPTION_EFFECT_0);
			}
			else if(index == 1)
			{
				g_pSOption->sTerrainEffectRender = 1 ;
				strcpy(strData, OPTION_EFFECT_1);
			}
			else if(index == 2)
			{
				g_pSOption->sTerrainEffectRender = 2 ;
				strcpy(strData, OPTION_EFFECT_2);
			}
			else if(index == 3)
			{
				g_pSOption->sTerrainEffectRender = 3 ;
				strcpy(strData, OPTION_EFFECT_3);
			}
			else if(index == 4)
			{
				g_pSOption->sTerrainEffectRender = 4 ;
				strcpy(strData, OPTION_EFFECT_4);
			}
			else if(index == 5)
			{
				g_pSOption->sTerrainEffectRender = 5 ;
				strcpy(strData, OPTION_EFFECT_5);
			}
			else if(index == 6)
			{
				g_pSOption->sTerrainEffectRender = 6 ;
				strcpy(strData, OPTION_EFFECT_6);
			}
			else if(index == 7)
			{
				g_pSOption->sTerrainEffectRender = 7 ;
				strcpy(strData, OPTION_EFFECT_7);
			}
			else if(index == 8)
			{
				g_pSOption->sTerrainEffectRender = 8 ;
				strcpy(strData, OPTION_EFFECT_8);
			}
			else if(index == 9)
			{
				g_pSOption->sTerrainEffectRender = 9 ;
				strcpy(strData, OPTION_EFFECT_9);
			}

	}
		break;
	case OPTION_TERRAIN_COMBO:
		{
			if(index == 0)
			{
				g_pSOption->sTerrainRender = 0 ;
				strcpy(strData, OPTION_TERRAIN_0);
			}
			else if(index == 1)
			{
				g_pSOption->sTerrainRender = 1 ;
				strcpy(strData, OPTION_TERRAIN_1);
			}
			else if(index == 2)
			{
				g_pSOption->sTerrainRender = 2 ;
				strcpy(strData, OPTION_TERRAIN_2);
			}
			else if(index == 3)
			{
				g_pSOption->sTerrainRender = 3 ;
				strcpy(strData, OPTION_TERRAIN_3);
			}
			else if(index == 4)
			{
				g_pSOption->sTerrainRender = 4 ;
				strcpy(strData, OPTION_TERRAIN_4);
			}
			else if(index == 5)
			{
				g_pSOption->sTerrainRender = 5 ;
				strcpy(strData, OPTION_TERRAIN_5);
			}
			else if(index == 6)
			{
				g_pSOption->sTerrainRender = 6 ;
				strcpy(strData, OPTION_TERRAIN_6);
			}
			else if(index == 7)
			{
				g_pSOption->sTerrainRender = 7 ;
				strcpy(strData, OPTION_TERRAIN_7);
			}
			else if(index == 8)
			{
				g_pSOption->sTerrainRender = 8 ;
				strcpy(strData, OPTION_TERRAIN_8);
			}
			else if(index == 9)
			{
				g_pSOption->sTerrainRender = 9 ;
				strcpy(strData, OPTION_TERRAIN_9);
			}

			::SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
		}
		break;
	case OPTION_UNITDET_COMBO:
		{			
			if(index == 0)
			{
				g_pSOption->sUnitDetail = 0 ;
				strcpy(strData, OPTION_UNITDETAIL_0);
			}
			else if(index == 1)
			{
				g_pSOption->sUnitDetail = 1 ;
				strcpy(strData, OPTION_UNITDETAIL_1);
			}
			else if(index == 2)
			{
				g_pSOption->sUnitDetail = 2 ;
				strcpy(strData, OPTION_UNITDETAIL_2);
			}
			else if(index == 3)
			{
				g_pSOption->sUnitDetail = 3 ;
				strcpy(strData, OPTION_UNITDETAIL_3);
			}
			else if(index == 4)
			{
				g_pSOption->sUnitDetail = 4 ;
				strcpy(strData, OPTION_UNITDETAIL_4);
			}
			else if(index == 5)
			{
				g_pSOption->sUnitDetail = 5 ;
				strcpy(strData, OPTION_UNITDETAIL_5);
			}
			else if(index == 6)
			{
				g_pSOption->sUnitDetail = 6 ;
				strcpy(strData, OPTION_UNITDETAIL_6);
			}
			else if(index == 7)
			{
				g_pSOption->sUnitDetail = 7 ;
				strcpy(strData, OPTION_UNITDETAIL_7);
			}
			else if(index == 8)
			{
				g_pSOption->sUnitDetail = 8 ;
				strcpy(strData, OPTION_UNITDETAIL_8);
			}
			else if(index == 9)
			{
				g_pSOption->sUnitDetail = 9 ;
				strcpy(strData, OPTION_UNITDETAIL_9);
			}
		}
		break;
	case OPTION_CONTRAST:
		{			
			if(index == 0)
			{
				g_pSOption->sContrast = 0 ;
				strcpy(strData, OPTION_CONTRAST_0);
			}
			else if(index == 1)
			{
				g_pSOption->sContrast = 1 ;
				strcpy(strData, OPTION_CONTRAST_1);
			}
			else if(index == 2)
			{
				g_pSOption->sContrast = 2 ;
				strcpy(strData, OPTION_CONTRAST_2);
			}
			else if(index == 3)
			{
				g_pSOption->sContrast = 3 ;
				strcpy(strData, OPTION_CONTRAST_3);
			}
			else if(index == 4)
			{
				g_pSOption->sContrast = 4 ;
				strcpy(strData, OPTION_CONTRAST_4);
			}
			else if(index == 5)
			{
				g_pSOption->sContrast = 5 ;
				strcpy(strData, OPTION_CONTRAST_5);
			}
			else if(index == 6)
			{
				g_pSOption->sContrast = 6 ;
				strcpy(strData, OPTION_CONTRAST_6);
			}
			else if(index == 7)
			{
				g_pSOption->sContrast = 7 ;
				strcpy(strData, OPTION_CONTRAST_7);
			}
			else if(index == 8)
			{
				g_pSOption->sContrast = 8 ;
				strcpy(strData, OPTION_CONTRAST_8);
			}
			else if(index == 9)
			{
				g_pSOption->sContrast = 9 ;
				strcpy(strData, OPTION_CONTRAST_9);
			}
		}
		break;
	case OPTION_FILTER:
		{			
			if(index == 0)
			{
				g_pSOption->sFilter = 0 ;
				strcpy(strData, OPTION_FILTER_0);
			}
			else if(index == 1)
			{
				g_pSOption->sFilter = 1 ;
				strcpy(strData, OPTION_FILTER_1);
			}
			else if(index == 2)
			{
				g_pSOption->sFilter = 2 ;
				strcpy(strData, OPTION_FILTER_2);
			}
			else if(index == 3)
			{
				g_pSOption->sFilter = 3 ;
				strcpy(strData, OPTION_FILTER_3);
			}
			else if(index == 4)
			{
				g_pSOption->sFilter = 4 ;
				strcpy(strData, OPTION_FILTER_4);
			}
			else if(index == 5)
			{
				g_pSOption->sFilter = 5 ;
				strcpy(strData, OPTION_FILTER_5);
			}
			else if(index == 6)
			{
				g_pSOption->sFilter = 6 ;
				strcpy(strData, OPTION_FILTER_6);
			}
			else if(index == 7)
			{
				g_pSOption->sFilter = 7 ;
				strcpy(strData, OPTION_FILTER_7);
			}
			else if(index == 8)
			{
				g_pSOption->sFilter = 8 ;
				strcpy(strData, OPTION_FILTER_8);
			}
			else if(index == 9)
			{
				g_pSOption->sFilter = 9 ;
				strcpy(strData, OPTION_FILTER_9);
			}
		}
		break;
	case OPTION_QUALITY_COMBO:
		{
			if(index == 0)
			{
				g_pSOption->sLowQuality = 0 ;
				strcpy(strData, OPTION_LOW_QUALITY_OFF);
			}
			else if(index == 1)
			{
				g_pSOption->sLowQuality = 1 ;
				strcpy(strData, OPTION_LOW_QUALITY_ON);
			}
		}
		break;
	}
}

void CINFSystem::RenderComboOptionList(int nWindowPosY)
{
	FLOG( "CINFSystem::RenderComboOptionList(int nWindowPosY)" );
	if(m_bShowOptionListBox[0])
	{
		// 해상도
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*0;
		RenderComboOptionListSingle(0, OPTION_MODE_0, x, y+OPTION_COMBO_HEIGHT*1);
		RenderComboOptionListSingle(1, OPTION_MODE_1, x, y+OPTION_COMBO_HEIGHT*2);
		RenderComboOptionListSingle(2, OPTION_MODE_2, x, y+OPTION_COMBO_HEIGHT*3);
		RenderComboOptionListSingle(3, OPTION_MODE_3, x, y+OPTION_COMBO_HEIGHT*4);
		RenderComboOptionListSingle(4, OPTION_MODE_4, x, y+OPTION_COMBO_HEIGHT*5);
		RenderComboOptionListSingle(5, OPTION_MODE_5, x, y+OPTION_COMBO_HEIGHT*6);
		RenderComboOptionListSingle(6, OPTION_MODE_6, x, y+OPTION_COMBO_HEIGHT*7);
		RenderComboOptionListSingle(7, OPTION_MODE_7, x, y+OPTION_COMBO_HEIGHT*8);
		RenderComboOptionListSingle(8, OPTION_MODE_8, x, y+OPTION_COMBO_HEIGHT*9);
		RenderComboOptionListSingle(9, OPTION_MODE_9, x, y+OPTION_COMBO_HEIGHT*10);
	}
	else if(m_bShowOptionListBox[1])
	{
		// 지형 시야
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*1;
		RenderComboOptionListSingle(0, OPTION_TERRAIN_0, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);
		RenderComboOptionListSingle(1, OPTION_TERRAIN_1, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
		RenderComboOptionListSingle(2, OPTION_TERRAIN_2, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
		RenderComboOptionListSingle(3, OPTION_TERRAIN_3, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);
		RenderComboOptionListSingle(4, OPTION_TERRAIN_4, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
		RenderComboOptionListSingle(5, OPTION_TERRAIN_5, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);
		RenderComboOptionListSingle(6, OPTION_TERRAIN_6, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);
		RenderComboOptionListSingle(7, OPTION_TERRAIN_7, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
		RenderComboOptionListSingle(8, OPTION_TERRAIN_8, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);
		RenderComboOptionListSingle(9, OPTION_TERRAIN_9, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
	}
	else if(m_bShowOptionListBox[2])
	{
		// 유닛 디테일
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*2;
		RenderComboOptionListSingle(0, OPTION_UNITDETAIL_0, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);
		RenderComboOptionListSingle(1, OPTION_UNITDETAIL_1, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
		RenderComboOptionListSingle(2, OPTION_UNITDETAIL_2, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
		RenderComboOptionListSingle(3, OPTION_UNITDETAIL_3, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);
		RenderComboOptionListSingle(4, OPTION_UNITDETAIL_4, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
		RenderComboOptionListSingle(5, OPTION_UNITDETAIL_5, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);
		RenderComboOptionListSingle(6, OPTION_UNITDETAIL_6, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);
		RenderComboOptionListSingle(7, OPTION_UNITDETAIL_7, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
		RenderComboOptionListSingle(8, OPTION_UNITDETAIL_8, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);
		RenderComboOptionListSingle(9, OPTION_UNITDETAIL_9, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
	}
	else if(m_bShowOptionListBox[3])
	{
		// 그림자
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*3;
		RenderComboOptionListSingle(0, OPTION_SHADOW_0, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);		
		RenderComboOptionListSingle(1, OPTION_SHADOW_1, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
		RenderComboOptionListSingle(2, OPTION_SHADOW_2, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
		RenderComboOptionListSingle(3, OPTION_SHADOW_3, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);		
		RenderComboOptionListSingle(4, OPTION_SHADOW_4, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
		RenderComboOptionListSingle(5, OPTION_SHADOW_5, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);
		RenderComboOptionListSingle(6, OPTION_SHADOW_6, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);		
		RenderComboOptionListSingle(7, OPTION_SHADOW_7, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
		RenderComboOptionListSingle(8, OPTION_SHADOW_8, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);
		RenderComboOptionListSingle(9, OPTION_SHADOW_9, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
	}
	else if(m_bShowOptionListBox[4])
	{
		// 오브젝트 이펙트
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*4;
		RenderComboOptionListSingle(0, OPTION_EFFECT_0, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);
		RenderComboOptionListSingle(1, OPTION_EFFECT_1, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
		RenderComboOptionListSingle(2, OPTION_EFFECT_2, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
		RenderComboOptionListSingle(3, OPTION_EFFECT_3, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);
		RenderComboOptionListSingle(4, OPTION_EFFECT_4, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
		RenderComboOptionListSingle(5, OPTION_EFFECT_5, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);	
		RenderComboOptionListSingle(6, OPTION_EFFECT_6, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);
		RenderComboOptionListSingle(7, OPTION_EFFECT_7, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
		RenderComboOptionListSingle(8, OPTION_EFFECT_8, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);	
		RenderComboOptionListSingle(9, OPTION_EFFECT_9, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
	}	
	else if(m_bShowOptionListBox[5])
	{
		// 감마
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*5;
		RenderComboOptionListSingle(0, "5", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);
		RenderComboOptionListSingle(1, "4", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
		RenderComboOptionListSingle(2, "3", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
		RenderComboOptionListSingle(3, "2", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);
		RenderComboOptionListSingle(4, "1", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
		RenderComboOptionListSingle(5, "0", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);
		RenderComboOptionListSingle(6, "-1", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);
		RenderComboOptionListSingle(7, "-2", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
		RenderComboOptionListSingle(8, "-3", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);
		RenderComboOptionListSingle(9, "-4", x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
	}
	else if(m_bShowOptionListBox[6])
	{
		if(g_pD3dApp->m_pFxSystem->IsRenderBlurFilter())
		{
			// 대비
			int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
			int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*6;
			RenderComboOptionListSingle(0, MODE_UNUSED, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);
			RenderComboOptionListSingle(1, OPTION_CONTRAST_1, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
			RenderComboOptionListSingle(2, OPTION_CONTRAST_2, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
			RenderComboOptionListSingle(3, OPTION_CONTRAST_3, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);
			RenderComboOptionListSingle(4, OPTION_CONTRAST_4, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
			RenderComboOptionListSingle(5, OPTION_CONTRAST_5, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);
			RenderComboOptionListSingle(6, OPTION_CONTRAST_6, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);
			RenderComboOptionListSingle(7, OPTION_CONTRAST_7, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
			RenderComboOptionListSingle(8, OPTION_CONTRAST_8, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);
			RenderComboOptionListSingle(9, OPTION_CONTRAST_9, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
		}
	}
	else if(m_bShowOptionListBox[7])
	{
		if(g_pD3dApp->m_pFxSystem->IsRenderGaussBlur())
		{
			// 필터
			int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
			int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*7;
			RenderComboOptionListSingle(0, MODE_UNUSED, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*1);
			RenderComboOptionListSingle(1, OPTION_FILTER_1, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*2);
			RenderComboOptionListSingle(2, OPTION_FILTER_2, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*3);
			RenderComboOptionListSingle(3, OPTION_FILTER_3, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*4);
			RenderComboOptionListSingle(4, OPTION_FILTER_4, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*5);
			RenderComboOptionListSingle(5, OPTION_FILTER_5, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*6);
			RenderComboOptionListSingle(6, OPTION_FILTER_6, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*7);
			RenderComboOptionListSingle(7, OPTION_FILTER_7, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*8);
			RenderComboOptionListSingle(8, OPTION_FILTER_8, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*9);
			RenderComboOptionListSingle(9, OPTION_FILTER_9, x+OPTION_COMBO_SORT, y+OPTION_COMBO_HEIGHT*10);
		}
	}
	else if(m_bShowOptionListBox[8])
	{
		// 최소프레임
		int x = POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X;
		int y = nWindowPosY+OPTION_COMBO_START_Y+OPTION_COMBO_INTERVAL*8;
		RenderComboOptionListSingle(0, OPTION_LOW_QUALITY_OFF, x, y+OPTION_COMBO_HEIGHT*1);
		RenderComboOptionListSingle(1, OPTION_LOW_QUALITY_ON,  x, y+OPTION_COMBO_HEIGHT*2);
	}
}

void CINFSystem::RenderComboOptionListSingle(int i, char* strText, int x, int y)
{
	FLOG( "CINFSystem::RenderComboOptionListSingle(int i, char* strText, int x, int y)" );

	m_pCombo->Move(x-OPTION_COMBO_SORT, y);
	m_pCombo->Render();
	if(0 == strcmp(strText, MODE_UNUSED))	
		m_pFontOptionList[i]->DrawText(x-OPTION_COMBO_SORT,y,DEFAULT_FONT_COMBO_LIST,strText, 0L);
	else
		m_pFontOptionList[i]->DrawText(x,y,DEFAULT_FONT_COMBO_LIST,strText, 0L);
}

void CINFSystem::RenderComboModeListSingle(int i, char* strText, int x, int y)
{
	FLOG( "CINFSystem::RenderComboModeListSingle(int i, char* strText, int x, int y)" );
	
	int nSort = 0;
	
	if(m_bShowModeListBox[MODE_CHANGE_PS_COMBO])
		nSort = 10;
//	else if(m_bShowModeListBox[MODE_CHANGE_MUSIC_COMBO])
//		nSort = OPTION_COMBO_SORT;
	else if(m_bShowModeListBox[MODE_CHANGE_SOUND_COMBO])
		nSort = OPTION_COMBO_SORT;
	else if(m_bShowModeListBox[MODE_CHANGE_MACHINE_COMBO])
		nSort = 8;

	m_pCombo->Move(x, y);
	m_pCombo->Render();
	m_pFontOptionList[i]->DrawText(x+nSort,y,DEFAULT_FONT_COMBO_LIST,strText, 0L);

}

void CINFSystem::UpdateGamma(int nGama)
{
	FLOG( "CINFSystem::UpdateGamma(int nGama)" );
    D3DGAMMARAMP	D3dGammaRamp;
    WORD			dwGamma = 0;

	if(nGama == 5)
		nGama = 256;
	else if(nGama == 4)
		nGama = 256;
	else if(nGama == 3)
		nGama = 256;
	else if(nGama == 2)
		nGama = 256;
	else if(nGama == 1)
		nGama = 256;
	else if(nGama == 0)
		nGama = 256;
	else if(nGama == -1)
		nGama = 216;
	else if(nGama == -2)
		nGama = 192;
	else if(nGama == -3)
		nGama = 176;
	else
		nGama = 168;

    ZeroMemory( &D3dGammaRamp, sizeof(D3dGammaRamp) );
	// 2005-01-04 by jschoi
//	g_pD3dDev->GetGammaRamp(&D3dGammaRamp ) ;
	g_pD3dDev->GetGammaRamp(0, &D3dGammaRamp ) ;

    for( int iColor = 0; iColor < 256; iColor++ )
    {
        D3dGammaRamp.red[iColor]   = dwGamma;
        D3dGammaRamp.green[iColor] = dwGamma;
        D3dGammaRamp.blue[iColor]  = dwGamma;

        dwGamma += (WORD)nGama ;
    }

	// 2005-01-04 by jschoi
//	g_pD3dDev->SetGammaRamp( D3DSGR_NO_CALIBRATION  , &D3dGammaRamp ) ;
	g_pD3dDev->SetGammaRamp(0, D3DSGR_NO_CALIBRATION  , &D3dGammaRamp ) ;
}

void CINFSystem::UpdateQuality(int nQuality)
{
	if(nQuality)
	{
		// 최소프레임을 유지할것인가

		g_pSOption->sLowQuality				= FALSE;
		memcpy(&m_pSOptionOld, g_pSOption, sizeof(sOPTION_SYSTEM));
		
		g_pSOption->sLowQuality				= TRUE;
		g_pSOption->sTerrainRender			= MIN_QUALITY_RENDER;
		g_pSOption->sTerrainEffectRender	= MIN_QUALITY_RENDER;
//		g_pSOption->sObjectViewDistance		= MIN_QUALITY_RENDER;
		g_pSOption->sShadowState			= MIN_QUALITY_RENDER;
		g_pSOption->sUnitDetail				= MIN_QUALITY_RENDER;
		g_pSOption->sContrast				= MIN_QUALITY_RENDER;
		g_pSOption->sFilter					= MIN_QUALITY_RENDER;
//		g_pSOption->sReflactive				= FALSE;

		SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
	}
	else
	{
		// 말것인가
		memcpy(g_pSOption, &m_pSOptionOld, sizeof(sOPTION_SYSTEM));
		g_pSOption->sLowQuality				= FALSE;
		SetFogLevel( g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex, TRUE );
	}

}

void CINFSystem::RenderCheckOption(int nWindowPosY)
{
	// 체크옵션 렌더링

	if(m_nSystemType == SYSTEM_OPTION)
	{
		
//		if(g_pSOption->sReflactive)
//		{
//			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_REFLACTIVE_X, nWindowPosY+MODE_RENDER_REFLACTIVE_Y);
//			m_pCheck->Render();
//		}
		if(g_pSOption->sLowQuality)
		{
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_LOWQUALITY_X, nWindowPosY+MODE_RENDER_LOWQUALITY_Y);
			m_pCheck->Render();
		}
		
	}
	else if(m_nSystemType == SYSTEM_MODE)
	{	
		// 마우스 포인트 보이기
		if(g_pSOption->sHandle || IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind) == TRUE)
		{
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y-40);
			m_pCheck->Render();
		}
		// 도움말
		if(g_pSOptionCharacter->sHelpDesk)
		{
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y-20);
			m_pCheck->Render();
		}
		// 2007-07-23 by bhsohn 오퍼레이터 추가작업
		if(g_pInterface->GetOperatorMode())
		{						
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y);
			m_pCheck->Render();
		}
		// end 2007-07-23 by bhsohn 오퍼레이터 추가작업
		// 자동 수평유지
		if(g_pSOption->sAutoBalance)
		{
			// 2007-07-23 by bhsohn 오퍼레이터 추가작업
			//m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y);
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+20);
			m_pCheck->Render();
		}		
		// 상하 반전 
		if(g_pSOption->sMouseReversUp)
		{
			// 2007-07-23 by bhsohn 오퍼레이터 추가작업
			//m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+20);
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+40);
			m_pCheck->Render();
		}
		// 좌우 반전
		if(g_pSOption->sMouseReversLeft)
		{
			// 2007-07-23 by bhsohn 오퍼레이터 추가작업
			//m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+40);
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+60);
			m_pCheck->Render();
		}		
		// 메뉴사용시 수평유지
		if(g_pSOption->sMenuBalance)
		{
			// 2007-07-23 by bhsohn 오퍼레이터 추가작업
			//m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+60);
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+80);
			m_pCheck->Render();
		}
		// 뮤직플레이어
		if(g_pSOption->sMp3Player)
		{
			// 2007-07-23 by bhsohn 오퍼레이터 추가작업
			//m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+80);
			m_pCheck->Move(POS_RIGHT_WINDOW_X+OPTION_COMBO_START_X+MODE_RENDER_HELPDESK_X, nWindowPosY+MODE_RENDER_HELPDESK_Y+100);
			m_pCheck->Render();
		}	
	}	
}

void CINFSystem::UpdateReflactive(int Reflactive)
{
}
