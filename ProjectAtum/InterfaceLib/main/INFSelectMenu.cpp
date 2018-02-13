// INFSelectMenu.cpp: implementation of the CINFSelectMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFUnitCreateInfo.h"
#include "INFSelectMenu.h"
#include "AtumApplication.h"
#include "INFSelectBack.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "INFSelect.h"
#include "INFPilotFace.h"
#include "AtumSound.h"

// 2011. 10. 10 by jskim UI시스템 변경
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
// end 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define SELECT_FONT_COLOR		RGB(222, 222, 222)

#define SEL_BUTTON_STATE_UP		0
#define SEL_BUTTON_STATE_DOWN	1
#define SEL_BUTTON_STATE_NORMAL	2

#define SEL_BUTTON_NEW		0
#define SEL_BUTTON_QUIT		1
#define SEL_BUTTON_OK		2
#define SEL_BUTTON_OPTION	3
#define SEL_BUTTON_DELETE	4
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	// 2011. 10. 10 by jskim UI시스템 변경
	#define POS_SEL_CENTER_X	(g_pD3dApp->GetBackBufferDesc().Width / 2)
	#define POS_SEL_TITLE_X		( g_pD3dApp->GetBackBufferDesc().Width / 2) - (( m_PSelTitle->GetMaxPos().x - m_PSelTitle->GetMinPos().x) / 2)
	#define POS_SEL_BACK_X		( g_pD3dApp->GetBackBufferDesc().Width / 2) - (( m_PBack->GetMaxPos().x - m_PBack->GetMinPos().x) / 2)
	#define POS_SEL_BACK_Y		g_pD3dApp->GetBackBufferDesc().Height - ( m_PBack->GetMaxPos().y - m_PBack->GetMinPos().y)
	#define POS_CHAR_NAME_X		( g_pD3dApp->GetBackBufferDesc().Width / 2) - (m_pFontCharacterName->GetTexWidth() / 2)
	#define POS_CHAR_NAME_Y		g_pD3dApp->GetBackBufferDesc().Height - ( m_PBack->GetMaxPos().y - m_PBack->GetMinPos().y - 4)
	#define POS_CHAR_LEVEL_Y	g_pD3dApp->GetBackBufferDesc().Height - ( m_PBack->GetMaxPos().y - m_PBack->GetMinPos().y - 4)


	#define POS_SELBTN_OK_X		POS_SEL_BACK_X+663
	#define POS_SELBTN_OK_Y		POS_SEL_BACK_Y+64

	#define SEL_SIZE_BUTTON_X(i)	(i == 2 ? 72 : 78)
	#define SEL_SIZE_BUTTON_Y(i)	(i == 2 ? 44 : 21)

	#define POS_SEL_CHAR_FACE_X	POS_SEL_BACK_X+101
	#define POS_SEL_CHAR_FACE_Y	POS_SEL_BACK_Y+37


	#define SELECT_CHARACTER_INFO_GUILD		0	// 소속
	#define SELECT_CHARACTER_INFO_RACE		1	// 종족
	#define SELECT_CHARACTER_INFO_DODGE		2	// 회피
	#define SELECT_CHARACTER_INFO_ATTACK	3	// 공격
	#define SELECT_CHARACTER_INFO_SOUL		4	// 감응
	#define SELECT_CHARACTER_INFO_FUEL		5	// 연료
	#define SELECT_CHARACTER_INFO_DEFENSE	6	// 방어
	#define SELECT_CHARACTER_INFO_ENDURANCE	7	// 내구

	#define SELECT_CHARACTER_INFO_HP		0	// HP
	#define SELECT_CHARACTER_INFO_UTC		1	// UTC
	#define SELECT_CHARACTER_INFO_SHIELD	2	// Shield

	#define SEL_GAGE_START_X	POS_SEL_BACK_X+491
	#define SEL_GAGE_START_Y	POS_SEL_BACK_Y+69
	#define SEL_GAGE_SIZE_X		m_pGage[0]->GetImgSize().x
	#define SEL_GAGE_SIZE_Y		m_pGage[0]->GetImgSize().y

	#define POS_SEL_CHARACTER_INFO_START_X	293
	#define DISTANCE_SEL_CHARACTER_INFO_X	88
	#define DISTANCE_SEL_CHARACTER_INFO_Y	20

	#define POS_SEL_TEXT_GAGE_START_X	498
	#define DISTANCE_SEL_TEXT_GAGE_Y	21

	// 2006-03-07 by ispark, 언어에 따라 위치 수정
	#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
		#define POS_SEL_CHARACTER_INFO_START_Y	46//49
		#define POS_SEL_TEXT_GAGE_START_Y	66//58
		#define POS_SEL_CHARACTER_NAME_X	218
		#define POS_SEL_CHARACTER_NAME_Y	24
		#define POS_SEL_LEVEL_X				185
		#define POS_SEL_LEVEL_Y				26
	#else
		#define POS_SEL_CHARACTER_INFO_START_Y	48//49
		#define POS_SEL_TEXT_GAGE_START_Y	67//58
		#define POS_SEL_CHARACTER_NAME_X	218
		#define POS_SEL_CHARACTER_NAME_Y	26//28
		#define POS_SEL_LEVEL_X				195
		#define POS_SEL_LEVEL_Y				26//28
	#endif

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	#define	DISTANCE_SEL_CHARACTER_CAP_X	30		// 글씨 간격
	#define	DISTANCE_SEL_INFO_OVER_CAP_X	22		// 초과와의 글씨 간격
	#define SELECT_OVERSTAT_FONT_COLOR		RGB(255, 0, 0)		 
#else 
#define POS_SEL_TITLE_X		g_pD3dApp->GetBackBufferDesc().Width /2 - 294/2
#define POS_SEL_BACK_X		g_pD3dApp->GetBackBufferDesc().Width /2 - 750/2	
#define POS_SEL_BACK_Y		g_pD3dApp->GetBackBufferDesc().Height - 148

#define POS_SELBTN_NEW_X	POS_SEL_BACK_X+612 
#define POS_SELBTN_NEW_Y	POS_SEL_BACK_Y+58
#define POS_SELBTN_QUIT_X	POS_SEL_BACK_X+666 
#define POS_SELBTN_QUIT_Y	POS_SEL_BACK_Y+81
#define POS_SELBTN_OK_X		POS_SEL_BACK_X+548
#define POS_SELBTN_OK_Y		POS_SEL_BACK_Y+62
#define POS_SELBTN_OPTION_X	POS_SEL_BACK_X+612 
#define POS_SELBTN_OPTION_Y	POS_SEL_BACK_Y+81
#define POS_SELBTN_DELETE_X	POS_SEL_BACK_X+666 
#define POS_SELBTN_DELETE_Y	POS_SEL_BACK_Y+58

#define SEL_SIZE_BUTTON_X(i)	(i == 2 ? 52 : 54)
#define SEL_SIZE_BUTTON_Y(i)	(i == 2 ? 36 : 23)

#define POS_SEL_CHAR_FACE_X	POS_SEL_BACK_X+28
#define POS_SEL_CHAR_FACE_Y	POS_SEL_BACK_Y+26


#define SELECT_CHARACTER_INFO_GUILD		0	// 소속
#define SELECT_CHARACTER_INFO_RACE		1	// 종족
#define SELECT_CHARACTER_INFO_DODGE		2	// 회피
#define SELECT_CHARACTER_INFO_ATTACK	3	// 공격
#define SELECT_CHARACTER_INFO_SOUL		4	// 감응
#define SELECT_CHARACTER_INFO_FUEL		5	// 연료
#define SELECT_CHARACTER_INFO_DEFENSE	6	// 방어
#define SELECT_CHARACTER_INFO_ENDURANCE	7	// 내구

#define SELECT_CHARACTER_INFO_HP		0	// HP
#define SELECT_CHARACTER_INFO_UTC		1	// UTC
#define SELECT_CHARACTER_INFO_SHIELD	2	// Shield

#define SEL_GAGE_START_X	POS_SEL_BACK_X+387
#define SEL_GAGE_START_Y	POS_SEL_BACK_Y+58
#define SEL_GAGE_SIZE_X		126
#define SEL_GAGE_SIZE_Y		11

#define POS_SEL_CHARACTER_INFO_START_X	210
#define DISTANCE_SEL_CHARACTER_INFO_X	88
#define DISTANCE_SEL_CHARACTER_INFO_Y	18

#define POS_SEL_TEXT_GAGE_START_X	387
#define DISTANCE_SEL_TEXT_GAGE_Y	21

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define POS_SEL_CHARACTER_INFO_START_Y	47//49
#define POS_SEL_TEXT_GAGE_START_Y	56//58
#define POS_SEL_CHARACTER_NAME_X	218
#define POS_SEL_CHARACTER_NAME_Y	24
#define POS_SEL_LEVEL_X				185
#define POS_SEL_LEVEL_Y				26
#else
#define POS_SEL_CHARACTER_INFO_START_Y	47//49
#define POS_SEL_TEXT_GAGE_START_Y	56//58
#define POS_SEL_CHARACTER_NAME_X	218
#define POS_SEL_CHARACTER_NAME_Y	26//28
#define POS_SEL_LEVEL_X				185
#define POS_SEL_LEVEL_Y				26//28
#endif

// 2007-05-15 by bhsohn 기어 스탯 관련 처리
#define	DISTANCE_SEL_CHARACTER_CAP_X	30		// 글씨 간격
#define	DISTANCE_SEL_INFO_OVER_CAP_X	22		// 초과와의 글씨 간격
#define SELECT_OVERSTAT_FONT_COLOR		RGB(255, 0, 0)

#endif


CINFSelectMenu::CINFSelectMenu(CAtumNode* pParent)
{
	FLOG( "CINFSelectMenu(CAtumNode* pParent)" );
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_pGameData = NULL;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_GruopSelectmanager = NULL;				
	memset(*m_pButton, 0x00, SEL_BUTTON_NUMBER*4);												  
#else
	m_pBack = NULL;
	m_pTitle = NULL;
	memset(*m_pButton, 0x00, SEL_BUTTON_NUMBER*2);
#endif


	m_nButtonState[SEL_BUTTON_NEW]		= SEL_BUTTON_STATE_NORMAL;
	m_nButtonState[SEL_BUTTON_QUIT]		= SEL_BUTTON_STATE_NORMAL;
	m_nButtonState[SEL_BUTTON_OK]		= SEL_BUTTON_STATE_NORMAL;
	m_nButtonState[SEL_BUTTON_OPTION]	= SEL_BUTTON_STATE_NORMAL;
	m_nButtonState[SEL_BUTTON_DELETE]	= SEL_BUTTON_STATE_NORMAL;

	m_fButtonPos[SEL_BUTTON_NEW][0] = 0.0f;
	m_fButtonPos[SEL_BUTTON_NEW][1] = 0.0f;
	m_fButtonPos[SEL_BUTTON_QUIT][0] = 0.0f;
	m_fButtonPos[SEL_BUTTON_QUIT][1] = 0.0f;
	m_fButtonPos[SEL_BUTTON_OK][0] = 0.0f;
	m_fButtonPos[SEL_BUTTON_OK][1] = 0.0f;
	m_fButtonPos[SEL_BUTTON_OPTION][0] = 0.0f;
	m_fButtonPos[SEL_BUTTON_OPTION][1] = 0.0f;
	m_fButtonPos[SEL_BUTTON_DELETE][0] = 0.0f;
	m_fButtonPos[SEL_BUTTON_DELETE][1] = 0.0f;

	m_pFontLevel = NULL;
	m_pFontCharacterName = NULL;
	memset(m_pFontCharacterInfo, 0x00, sizeof(DWORD)*SEL_CHARACTER_INFO_NUMBER);
	memset(m_pFontGage, 0x00, sizeof(DWORD)*SEL_GAGE_NUMBER);

	m_nLevelPos[0] = 0;
	m_nLevelPos[1] = 0;
	m_nCharacterNamePos[0] = 0;
	m_nCharacterNamePos[1] = 0;
	memset(*m_nCharacterInfoPos, 0x00, sizeof(DWORD)*SEL_CHARACTER_INFO_NUMBER*2);
	memset(*m_nGagePos, 0x00, sizeof(DWORD)*SEL_GAGE_NUMBER*2);

	m_pGage[0] = NULL;
	m_pGage[1] = NULL;
	m_pGage[2] = NULL;

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	m_pFontOverStat = NULL;
	
}

CINFSelectMenu::~CINFSelectMenu()
{
	FLOG( "~CINFSelectMenu()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	util::del( m_GruopSelectmanager );
#else
	util::del( m_pBack );
	util::del( m_pTitle );
#endif
	int i;
	for(i=0;i<SEL_BUTTON_NUMBER;i++)
	{
		util::del(m_pButton[i][0]);
		util::del(m_pButton[i][1]);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		util::del(m_pButton[i][2]);
		util::del(m_pButton[i][3]);															  
#endif
	}
	for(i=0;i<SEL_CHARACTER_INFO_NUMBER; i++)
		util::del( m_pFontCharacterInfo[i] );
	for(i=0;i<SEL_GAGE_NUMBER; i++)
	{
		util::del( m_pFontGage[i] );
		util::del( m_pGage[i] );
	}

	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	util::del(m_pFontOverStat);
}


HRESULT CINFSelectMenu::InitDeviceObjects()
{
	FLOG( "CINFSelectMenu::InitDeviceObjects()" );
	
	m_pFontLevel = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),11, D3DFONT_ZENABLE | D3DFONT_BOLD, FALSE,128,32);
	m_pFontLevel->InitDeviceObjects(g_pD3dDev);
	m_pFontCharacterName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),11, D3DFONT_ZENABLE | D3DFONT_BOLD, FALSE,256,32);
	m_pFontCharacterName->InitDeviceObjects(g_pD3dDev);
	int i;
	for(i=0;i<SEL_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,128,32);
		m_pFontCharacterInfo[i]->InitDeviceObjects(g_pD3dDev);
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	{
		m_pFontOverStat = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE | D3DFONT_BOLD,  FALSE,128,32);
		m_pFontOverStat->InitDeviceObjects(g_pD3dDev);
	}

	for(i=0;i<SEL_GAGE_NUMBER; i++)
	{
		m_pFontGage[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,128,32);
		m_pFontGage[i]->InitDeviceObjects(g_pD3dDev);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
 	DataHeader * pDataHeader;
	m_GruopSelectmanager = new CINFGroupManager( g_pD3dApp->m_pImageList, m_pGameData );
	m_GruopSelectmanager->InitDeviceObjects();
	pDataHeader = m_GruopSelectmanager->FindResource("seltitle");
	m_PSelTitle = m_GruopSelectmanager->GetGroupImage( pDataHeader );
	m_PSelTitle->InitDeviceObjects( g_pD3dApp->m_pImageList );
	//m_PSelTitle->RestoreDeviceObjects();
	
	pDataHeader = m_GruopSelectmanager->FindResource("selinfo");
	m_PBack = m_GruopSelectmanager->GetGroupImage( pDataHeader );
	m_PBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
	//m_PBack->RestoreDeviceObjects();

	pDataHeader = m_GruopSelectmanager->FindResource("selbtn");
	m_PControlSelbtn = m_GruopSelectmanager->GetGroupImage( pDataHeader );
#else
 	m_pBack = new CINFImageEx;
	DataHeader * pDataHeader = FindResource("selback");
 	m_pBack->InitDeviceObjects(pDataHeader);

 	m_pTitle = new CINFImageEx;
	pDataHeader = FindResource("seltitle");
 	m_pTitle->InitDeviceObjects(pDataHeader);
#endif

	char buf[32];
	for(i=0;i<SEL_BUTTON_NUMBER;i++)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		for(int j=0;j<5;j++)
		{
#else	  
		for(int j=0;j<2;j++)
		{
#endif
			wsprintf( buf, "selbtn%d%d",i,j);
			m_pButton[i][j] =new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
			pDataHeader = FindResource(buf);

			if(pDataHeader)
				m_pButton[i][j]->InitDeviceObjects( pDataHeader );
//			else
//				ERROR Reporting ( exit )
		}
	}
	m_pGage[0] = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("selHP");
	m_pGage[0]->InitDeviceObjects( pDataHeader );
	m_pGage[1] = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("selSP");
	m_pGage[1]->InitDeviceObjects( pDataHeader );
	m_pGage[2] = new CINFImageEx;		// 2011. 10. 10 by jskim UI시스템 변경
	pDataHeader = FindResource("selDP");
	m_pGage[2]->InitDeviceObjects( pDataHeader );

	return S_OK ;
}


HRESULT CINFSelectMenu::RestoreDeviceObjects()
{
	FLOG( "CINFSelectMenu::RestoreDeviceObjects()" );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_PSelTitle->RestoreDeviceObjects();
	m_PBack->RestoreDeviceObjects();
#else
	m_pBack->RestoreDeviceObjects();
	m_pTitle->RestoreDeviceObjects();
#endif

	m_pFontLevel->RestoreDeviceObjects() ;
	m_pFontCharacterName->RestoreDeviceObjects();
	m_nLevelPos[0] = POS_SEL_BACK_X+POS_SEL_LEVEL_X;
	m_nLevelPos[1] = POS_SEL_BACK_Y+POS_SEL_LEVEL_Y;
	m_nCharacterNamePos[0] = POS_SEL_BACK_X+POS_SEL_CHARACTER_NAME_X;
	m_nCharacterNamePos[1] = POS_SEL_BACK_Y+POS_SEL_CHARACTER_NAME_Y;
	int i;
	for(i=0;i<SEL_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->RestoreDeviceObjects();
		m_nCharacterInfoPos[i][0] = POS_SEL_BACK_X+POS_SEL_CHARACTER_INFO_START_X+DISTANCE_SEL_CHARACTER_INFO_X*(i%2);
		m_nCharacterInfoPos[i][1] = POS_SEL_BACK_Y+POS_SEL_CHARACTER_INFO_START_Y+DISTANCE_SEL_CHARACTER_INFO_Y*(i/2);
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	{
		m_pFontOverStat->RestoreDeviceObjects();
	}
	for(i=0;i<SEL_GAGE_NUMBER; i++)
	{
		m_pFontGage[i]->RestoreDeviceObjects();
		m_pGage[i]->RestoreDeviceObjects();
		m_nGagePos[i][0] = POS_SEL_BACK_X+POS_SEL_TEXT_GAGE_START_X;
		m_nGagePos[i][1] = POS_SEL_BACK_Y+POS_SEL_TEXT_GAGE_START_Y+DISTANCE_SEL_TEXT_GAGE_Y*i;
	}
	


	for(i=0;i<SEL_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->RestoreDeviceObjects();
		m_pButton[i][1]->RestoreDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pButton[i][2]->RestoreDeviceObjects();
		m_pButton[i][3]->RestoreDeviceObjects();
#endif
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT pMinPos = m_PControlSelbtn->GetFindControlTargetofMinPos( "selbtn00" );
	m_fButtonPos[SEL_BUTTON_NEW][0] = POS_SELBTN_OK_X + pMinPos.x;
	m_fButtonPos[SEL_BUTTON_NEW][1] = POS_SELBTN_OK_Y + pMinPos.y;

	pMinPos = m_PControlSelbtn->GetFindControlTargetofMinPos( "selbtn10" );
	m_fButtonPos[SEL_BUTTON_QUIT][0] = POS_SELBTN_OK_X + pMinPos.x;
	m_fButtonPos[SEL_BUTTON_QUIT][1] = POS_SELBTN_OK_Y + pMinPos.y;

	pMinPos = m_PControlSelbtn->GetFindControlTargetofMinPos( "selbtn20" );
	m_fButtonPos[SEL_BUTTON_OK][0] = POS_SELBTN_OK_X + pMinPos.x;
	m_fButtonPos[SEL_BUTTON_OK][1] = POS_SELBTN_OK_Y + pMinPos.y;

	pMinPos = m_PControlSelbtn->GetFindControlTargetofMinPos( "selbtn30" );
	m_fButtonPos[SEL_BUTTON_OPTION][0] = POS_SELBTN_OK_X + pMinPos.x;
	m_fButtonPos[SEL_BUTTON_OPTION][1] = POS_SELBTN_OK_Y + pMinPos.y;

	pMinPos = m_PControlSelbtn->GetFindControlTargetofMinPos( "selbtn40" );
	m_fButtonPos[SEL_BUTTON_DELETE][0] = POS_SELBTN_OK_X + pMinPos.x;
	m_fButtonPos[SEL_BUTTON_DELETE][1] = POS_SELBTN_OK_Y + pMinPos.y;
	

	// 2011. 10. 18 by jskim EP4 UI변경 - select창 변경
	m_GruopSelectmanager->RestoreDeviceObjects();

// 	DataHeader * pDataHeader;
// 	util::del(m_PSelTitle);
// 	pDataHeader = m_GruopSelectmanager->FindResource("seltitle");
// 	m_PSelTitle = m_GruopSelectmanager->GetGroupImage( pDataHeader );
// 	m_PSelTitle->InitDeviceObjects( g_pD3dApp->m_pImageList );
// 	m_PSelTitle->RestoreDeviceObjects();
// 		
// 	util::del(m_PBack);
// 	pDataHeader = m_GruopSelectmanager->FindResource("selinfo");	
// 	m_PBack = m_GruopSelectmanager->GetGroupImage( pDataHeader );
// 	m_PBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
// 	m_PBack->RestoreDeviceObjects();
#else 
	m_fButtonPos[SEL_BUTTON_NEW][0] = POS_SELBTN_NEW_X;
	m_fButtonPos[SEL_BUTTON_NEW][1] = POS_SELBTN_NEW_Y;
	m_fButtonPos[SEL_BUTTON_QUIT][0] = POS_SELBTN_QUIT_X;
	m_fButtonPos[SEL_BUTTON_QUIT][1] = POS_SELBTN_QUIT_Y;
	m_fButtonPos[SEL_BUTTON_OK][0] = POS_SELBTN_OK_X;
	m_fButtonPos[SEL_BUTTON_OK][1] = POS_SELBTN_OK_Y;
	m_fButtonPos[SEL_BUTTON_OPTION][0] = POS_SELBTN_OPTION_X;
	m_fButtonPos[SEL_BUTTON_OPTION][1] = POS_SELBTN_OPTION_Y;
	m_fButtonPos[SEL_BUTTON_DELETE][0] = POS_SELBTN_DELETE_X;
	m_fButtonPos[SEL_BUTTON_DELETE][1] = POS_SELBTN_DELETE_Y;
#endif
	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFSelectMenu::InvalidateDeviceObjects()
{
	FLOG( "CINFSelectMenu::InvalidateDeviceObjects()" );
	m_pFontLevel->InvalidateDeviceObjects() ;
	m_pFontCharacterName->InvalidateDeviceObjects();
	int i;
	for(i=0;i<SEL_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->InvalidateDeviceObjects();
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	{
		m_pFontOverStat->InvalidateDeviceObjects();
	}

	for(i=0;i<SEL_GAGE_NUMBER; i++)
	{
		m_pFontGage[i]->InvalidateDeviceObjects();
		m_pGage[i]->InvalidateDeviceObjects();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_PSelTitle->InvalidateDeviceObjects();
 	m_PBack->InvalidateDeviceObjects();
#else
	m_pBack->InvalidateDeviceObjects();
	m_pTitle->InvalidateDeviceObjects();
#endif
	for(i=0;i<SEL_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->InvalidateDeviceObjects();
		m_pButton[i][1]->InvalidateDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pButton[i][2]->InvalidateDeviceObjects();
		m_pButton[i][3]->InvalidateDeviceObjects();												  
#endif
	}
	m_bRestored = FALSE;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_GruopSelectmanager->InvalidateDeviceObjects();
	m_PSelTitle->InvalidateDeviceObjects();
	m_PBack->InvalidateDeviceObjects();
#endif
	return S_OK ;
}

HRESULT CINFSelectMenu::DeleteDeviceObjects()
{
	FLOG( "CINFSelectMenu::DeleteDeviceObjects()" );
	m_pFontLevel->DeleteDeviceObjects() ;
	util::del(m_pFontLevel);
	m_pFontCharacterName->DeleteDeviceObjects();
	util::del(m_pFontCharacterName);
	int i;
	for(i=0;i<SEL_CHARACTER_INFO_NUMBER; i++)
	{
		m_pFontCharacterInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontCharacterInfo[i]);
	}
	// 2007-05-15 by bhsohn 기어 스탯 관련 처리
	{
		m_pFontOverStat->DeleteDeviceObjects();
		util::del(m_pFontOverStat);		
	}
	for(i=0;i<SEL_GAGE_NUMBER; i++)
	{
		m_pFontGage[i]->DeleteDeviceObjects();
		m_pGage[i]->DeleteDeviceObjects();
		util::del(m_pFontGage[i]);
		util::del(m_pGage[i]);
	}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pBack->DeleteDeviceObjects();
	util::del(m_pBack);
	m_pTitle->DeleteDeviceObjects();
	util::del(m_pTitle);
#endif
	for(i=0;i<SEL_BUTTON_NUMBER;i++)
	{
		m_pButton[i][0]->DeleteDeviceObjects();
		m_pButton[i][1]->DeleteDeviceObjects();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		m_pButton[i][2]->DeleteDeviceObjects();
		m_pButton[i][3]->DeleteDeviceObjects();
#endif
		util::del(m_pButton[i][0]);
		util::del(m_pButton[i][1]);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		util::del(m_pButton[i][2]);
		util::del(m_pButton[i][3]);															  
#endif
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_GruopSelectmanager->DeleteDeviceObjects();
	m_PSelTitle->DeleteDeviceObjects();
	m_PBack->DeleteDeviceObjects();
	util::del( m_GruopSelectmanager );
	util::del( m_PSelTitle );
	util::del( m_PBack );
#endif
	return S_OK ;
}

void CINFSelectMenu::Tick()
{
	FLOG( "CINFSelectMenu::Tick()" );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
// 	m_PSelTitle->Tick();
// 	m_PBack->Tick();
#endif
}

void CINFSelectMenu::Render()
{
	FLOG( "CINFSelectMenu::Render()" );
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pTitle->Move(POS_SEL_TITLE_X, 0);
	m_pTitle->Render();
	m_pBack->Move(POS_SEL_BACK_X, POS_SEL_BACK_Y);
	m_pBack->Render();
#else
	m_PSelTitle->Move( POS_SEL_TITLE_X ,0 );
	m_PSelTitle->Render();

	m_PBack->Move( POS_SEL_BACK_X ,POS_SEL_BACK_Y );
	m_PBack->Render();
#endif
	for(int i=0;i<SEL_BUTTON_NUMBER;i++)
	{
		if(m_nButtonState[i] != SEL_BUTTON_STATE_NORMAL)
		{
			m_pButton[i][m_nButtonState[i]]->Move(m_fButtonPos[i][0],m_fButtonPos[i][1]);
			m_pButton[i][m_nButtonState[i]]->Render();
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		else
		{
			m_pButton[i][3]->Move(m_fButtonPos[i][0],m_fButtonPos[i][1]);
			m_pButton[i][3]->Render();
		}	
#endif
	}

	auto parent = static_cast<CINFSelect*>(m_pParent);

	auto& unit = parent->m_character[parent->GetCurrentSelectIndex()];
	
	if (unit.CharacterUniqueNumber > 0)
	{
		auto pFace = parent->m_pPilotFace->FindPilotImage(unit.PilotFace);

		if (pFace)
		{
			pFace->Move(POS_SEL_CHAR_FACE_X,POS_SEL_CHAR_FACE_Y);
			pFace->Render();
		}

		auto name_buffer = unit.GetCharacterNameForRender();
		auto name = name_buffer.data();

		char buf[256];
		wsprintf(buf, "%d", unit.Level);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM

		m_pFontLevel->DrawText(POS_SEL_CENTER_X - 15, POS_CHAR_LEVEL_Y + 40, SELECT_FONT_COLOR, buf, 0L);

		auto temp = GetStringBuffLen(name);
		temp = GetStringBuffPos(name, temp);

		auto size = m_pFontCharacterName->GetStringSize(name);
		m_pFontCharacterName->DrawText(( g_pD3dApp->GetBackBufferDesc().Width / 2) - (size.cx / 2), POS_CHAR_NAME_Y, SELECT_FONT_COLOR, name, 0L);

#else
		m_pFontLevel->DrawText(m_nLevelPos[0], m_nLevelPos[1], SELECT_FONT_COLOR, buf, 0L);
		m_pFontCharacterName->DrawText(m_nCharacterNamePos[0], m_nCharacterNamePos[1], SELECT_FONT_COLOR, name, 0L);
#endif

		if(COMPARE_RACE(unit.Race, RACE_DECA))
			strcpy( buf, STRMSG_C_SELECT_0001);
		if(COMPARE_RACE(unit.Race, RACE_BATTALUS))
			strcpy( buf, STRMSG_C_SELECT_0002);
		if(COMPARE_RACE(unit.Race, RACE_DEMO))
			strcpy( buf, STRMSG_C_SELECT_0003);
		if(COMPARE_RACE(unit.Race, RACE_GAMEMASTER))
			strcpy( buf, STRMSG_C_SELECT_0004);
		if(COMPARE_RACE(unit.Race, RACE_OPERATION))
			strcpy( buf, STRMSG_C_SELECT_0005);
		if(COMPARE_RACE(unit.Race, RACE_GUEST))
			strcpy( buf, STRMSG_C_SELECT_0006);
		if(COMPARE_RACE(unit.Race, RACE_MONITOR))
			strcpy( buf, STRMSG_C_SELECT_0007);

		m_pFontCharacterInfo[SELECT_CHARACTER_INFO_RACE]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_RACE][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_RACE][1], SELECT_FONT_COLOR, buf, 0L);
		if(unit.GuildUniqueNumber != 0)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM	
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_GUILD]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_GUILD][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_GUILD][1], SELECT_FONT_COLOR, unit.GuildName, 0L);
#else
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_GUILD]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_GUILD][0]-18, m_nCharacterInfoPos[SELECT_CHARACTER_INFO_GUILD][1], SELECT_FONT_COLOR, unit.GuildName, 0L);
#endif

		if(unit.TotalGearStat.AttackPart <= COUNT_MAX_STAT_POINT)
		{				
			if(unit.TotalGearStat.AttackPart == COUNT_MAX_STAT_POINT)
			{
				wsprintf( buf, "\\e%d", unit.TotalGearStat.AttackPart);
			}
			else
			{
				wsprintf( buf, "%d", unit.TotalGearStat.AttackPart);
			}
				
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_ATTACK]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ATTACK][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ATTACK][1], SELECT_FONT_COLOR, buf, 0L);
		}
		else
		{
			float fX, fY;
			fX = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ATTACK][0]-DISTANCE_SEL_CHARACTER_CAP_X;
			fY = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ATTACK][1];
			wsprintf( buf, "\\e%d", COUNT_MAX_STAT_POINT);
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_ATTACK]->DrawText(fX, fY, SELECT_FONT_COLOR, buf, 0L);

			fX += DISTANCE_SEL_INFO_OVER_CAP_X;
			wsprintf( buf, "(+%d)", unit.TotalGearStat.AttackPart - COUNT_MAX_STAT_POINT);
			m_pFontOverStat->DrawText(fX, fY, SELECT_OVERSTAT_FONT_COLOR, buf, 0L);				
		}
			
		// 방어 스탯
		if(unit.TotalGearStat.DefensePart <= COUNT_MAX_STAT_POINT)
		{
			if(unit.TotalGearStat.DefensePart == COUNT_MAX_STAT_POINT)
			{
				wsprintf( buf, "\\e%d", unit.TotalGearStat.DefensePart);
			}
			else
			{
				wsprintf( buf, "%d", unit.TotalGearStat.DefensePart);
			}
				
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_ENDURANCE]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ENDURANCE][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ENDURANCE][1], SELECT_FONT_COLOR, buf, 0L);
		}
		else
		{
			float fX, fY;
			fX = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ENDURANCE][0]-DISTANCE_SEL_CHARACTER_CAP_X;
			fY = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_ENDURANCE][1];

			wsprintf( buf, "\\e%d", COUNT_MAX_STAT_POINT);
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_ENDURANCE]->DrawText(fX, fY, SELECT_FONT_COLOR, buf, 0L);				

			fX += DISTANCE_SEL_INFO_OVER_CAP_X;
			wsprintf( buf, "(+%d)", unit.TotalGearStat.DefensePart - COUNT_MAX_STAT_POINT);
			m_pFontOverStat->DrawText(fX, fY, SELECT_OVERSTAT_FONT_COLOR, buf, 0L);				
		}
			
		// 연료 스탯
		if(unit.TotalGearStat.FuelPart <= COUNT_MAX_STAT_POINT)
		{
			if(unit.TotalGearStat.FuelPart == COUNT_MAX_STAT_POINT)
			{
				wsprintf( buf, "\\e%d", unit.TotalGearStat.FuelPart);
			}
			else
			{
				wsprintf( buf, "%d", unit.TotalGearStat.FuelPart);
			}
				
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_FUEL]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_FUEL][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_FUEL][1], SELECT_FONT_COLOR, buf, 0L);
		}
		else
		{
			float fX, fY;
			fX = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_FUEL][0]-DISTANCE_SEL_CHARACTER_CAP_X;
			fY = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_FUEL][1];

			wsprintf( buf, "\\e%d", COUNT_MAX_STAT_POINT);
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_FUEL]->DrawText(fX, fY, SELECT_FONT_COLOR, buf, 0L);				

			fX += DISTANCE_SEL_INFO_OVER_CAP_X;
			wsprintf( buf, "(+%d)", unit.TotalGearStat.FuelPart - COUNT_MAX_STAT_POINT);
			m_pFontOverStat->DrawText(fX, fY, SELECT_OVERSTAT_FONT_COLOR, buf, 0L);				
		}
			
		// 정신 스탯
		if(unit.TotalGearStat.SoulPart <= COUNT_MAX_STAT_POINT)
		{
			if(unit.TotalGearStat.SoulPart == COUNT_MAX_STAT_POINT)
			{
				wsprintf( buf, "\\e%d", unit.TotalGearStat.SoulPart);
			}
			else
			{
				wsprintf( buf, "%d", unit.TotalGearStat.SoulPart);
			}
				
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_SOUL]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_SOUL][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_SOUL][1], SELECT_FONT_COLOR, buf, 0L);
		}
		else
		{
			float fX, fY;
			fX = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_SOUL][0]-DISTANCE_SEL_CHARACTER_CAP_X;
			fY = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_SOUL][1];

			wsprintf( buf, "\\e%d", COUNT_MAX_STAT_POINT);
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_SOUL]->DrawText(fX, fY, SELECT_FONT_COLOR, buf, 0L);				

			fX += DISTANCE_SEL_INFO_OVER_CAP_X;
			wsprintf( buf, "(+%d)", unit.TotalGearStat.SoulPart - COUNT_MAX_STAT_POINT);
			m_pFontOverStat->DrawText(fX, fY, SELECT_OVERSTAT_FONT_COLOR, buf, 0L);				
		}
			
		// 회피 스탯
		if(unit.TotalGearStat.DodgePart <= COUNT_MAX_STAT_POINT)
		{
			if(unit.TotalGearStat.DodgePart == COUNT_MAX_STAT_POINT)
			{
				wsprintf( buf, "\\e%d", unit.TotalGearStat.DodgePart);
			}
			else
			{
				wsprintf( buf, "%d", unit.TotalGearStat.DodgePart);
			}
				
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_DODGE]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DODGE][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DODGE][1], SELECT_FONT_COLOR, buf, 0L);
		}
		else
		{
			float fX, fY;
			fX = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DODGE][0]-DISTANCE_SEL_CHARACTER_CAP_X;
			fY = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DODGE][1];

			wsprintf( buf, "\\e%d", COUNT_MAX_STAT_POINT);
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_DODGE]->DrawText(fX, fY, SELECT_FONT_COLOR, buf, 0L);				

			fX += DISTANCE_SEL_INFO_OVER_CAP_X;
			wsprintf( buf, "(+%d)", unit.TotalGearStat.DodgePart - COUNT_MAX_STAT_POINT);
			m_pFontOverStat->DrawText(fX, fY, SELECT_OVERSTAT_FONT_COLOR, buf, 0L);				
		}
			
		// 쉴드 스탯
		if(unit.TotalGearStat.ShieldPart <= COUNT_MAX_STAT_POINT)
		{
			if(unit.TotalGearStat.ShieldPart == COUNT_MAX_STAT_POINT)
			{
				wsprintf( buf, "\\e%d", unit.TotalGearStat.ShieldPart);
			}
			else
			{
				wsprintf( buf, "%d", unit.TotalGearStat.ShieldPart);
			}
				
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_DEFENSE]->DrawText(m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DEFENSE][0], m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DEFENSE][1], SELECT_FONT_COLOR, buf, 0L);
		}
		else
		{
			float fX, fY;
			fX = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DEFENSE][0]-DISTANCE_SEL_CHARACTER_CAP_X;
			fY = m_nCharacterInfoPos[SELECT_CHARACTER_INFO_DEFENSE][1];

			wsprintf( buf, "\\e%d", COUNT_MAX_STAT_POINT);
			m_pFontCharacterInfo[SELECT_CHARACTER_INFO_DEFENSE]->DrawText(fX, fY, SELECT_FONT_COLOR, buf, 0L);				

			fX += DISTANCE_SEL_INFO_OVER_CAP_X;
			wsprintf( buf, "(+%d)", unit.TotalGearStat.ShieldPart - COUNT_MAX_STAT_POINT);
			m_pFontOverStat->DrawText(fX, fY, SELECT_OVERSTAT_FONT_COLOR, buf, 0L);				
		}			
		// end 2007-05-15 by bhsohn 기어 스탯 관련 처리


		m_pGage[0]->Move(SEL_GAGE_START_X, SEL_GAGE_START_Y);
		m_pGage[0]->SetRect(0,0,SEL_GAGE_SIZE_X * (unit.CurrentHP/(float)unit.HP),SEL_GAGE_SIZE_Y);
		m_pGage[0]->Render();
			
		sprintf( buf, "%.0f/%d", unit.CurrentHP, unit.HP);
		m_pFontGage[SELECT_CHARACTER_INFO_HP]->DrawText(m_nGagePos[SELECT_CHARACTER_INFO_HP][0], m_nGagePos[SELECT_CHARACTER_INFO_HP][1], SELECT_FONT_COLOR, buf, 0L);
		m_pGage[1]->Move(SEL_GAGE_START_X, SEL_GAGE_START_Y+DISTANCE_SEL_TEXT_GAGE_Y);
		m_pGage[1]->SetRect(0,0,SEL_GAGE_SIZE_X * (unit.CurrentSP/(float)unit.SP),SEL_GAGE_SIZE_Y);
		m_pGage[1]->Render();
			
		wsprintf( buf, "%d/%d", unit.CurrentSP, unit.SP);
		m_pFontGage[SELECT_CHARACTER_INFO_UTC]->DrawText(m_nGagePos[SELECT_CHARACTER_INFO_UTC][0], m_nGagePos[SELECT_CHARACTER_INFO_UTC][1], SELECT_FONT_COLOR, buf, 0L);
		m_pGage[2]->Move(SEL_GAGE_START_X, SEL_GAGE_START_Y+DISTANCE_SEL_TEXT_GAGE_Y*2);
		m_pGage[2]->SetRect(0,0,SEL_GAGE_SIZE_X * (unit.CurrentDP/(float)unit.DP),SEL_GAGE_SIZE_Y);
		m_pGage[2]->Render();
			
		sprintf( buf, "%.0f/%d", unit.CurrentDP, unit.DP);
		m_pFontGage[SELECT_CHARACTER_INFO_SHIELD]->DrawText(m_nGagePos[SELECT_CHARACTER_INFO_SHIELD][0], m_nGagePos[SELECT_CHARACTER_INFO_SHIELD][1], SELECT_FONT_COLOR, buf, 0L);
	}
}

void CINFSelectMenu::RenderGage(CINFImage* pImage, int x, int y, float width, float height)
{
	FLOG( "CINFSelectMenu::RenderGage(CINFImage* pImage, int x, int y, float width, float height)" );
	if(width > 0)
	{
		if(width>2)
		{
			pImage->Move(x+1, y);
			pImage->SetScale(width-2, height);
			pImage->Render();
		}
		pImage->Move(x, y+1);
		pImage->SetScale(1, height-2);
		pImage->Render();
		if(width>1)
		{
			pImage->Move(x+width-1, y+1);
			pImage->SetScale(1, height-2);
			pImage->Render();
		}
	}
	
}


int CINFSelectMenu::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFSelectMenu::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			for(int i=0; i<SEL_BUTTON_NUMBER; i++)
			{
				if( pt.x > m_fButtonPos[i][0] && pt.x < m_fButtonPos[i][0] + SEL_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + SEL_SIZE_BUTTON_Y(i) )
				{
					m_nButtonState[i] = SEL_BUTTON_STATE_DOWN;
					g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				}
				else
				{
					m_nButtonState[i] = SEL_BUTTON_STATE_NORMAL;
				}
			}

		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			for(int i=0; i<SEL_BUTTON_NUMBER; i++)
			{
				if( pt.x > m_fButtonPos[i][0] && pt.x < m_fButtonPos[i][0] + SEL_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + SEL_SIZE_BUTTON_Y(i) )
				{
					if(m_nButtonState[i] == SEL_BUTTON_STATE_DOWN)
					{
						m_nButtonState[i] = SEL_BUTTON_STATE_UP;
						return OnButtonClicked(i);
					}
					else
					{
						m_nButtonState[i] = SEL_BUTTON_STATE_NORMAL;
					}
				}
				else
				{
					m_nButtonState[i] = SEL_BUTTON_STATE_NORMAL;
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			BOOL bChangeMouseState = FALSE;
			for(int i=0; i<SEL_BUTTON_NUMBER; i++)
			{
				if( pt.x > m_fButtonPos[i][0] && pt.x < m_fButtonPos[i][0] + SEL_SIZE_BUTTON_X(i) &&
					pt.y > m_fButtonPos[i][1] && pt.y < m_fButtonPos[i][1] + SEL_SIZE_BUTTON_Y(i) )
				{
					if(m_nButtonState[i] != SEL_BUTTON_STATE_DOWN)
					{
						m_nButtonState[i] = SEL_BUTTON_STATE_UP;
					}
					((CINFSelect*)m_pParent)->SetMouseState( SELECT_MOUSE_STATE_UP );
					bChangeMouseState = TRUE;
				}
				else
				{
					m_nButtonState[i] = SEL_BUTTON_STATE_NORMAL;
					if(!bChangeMouseState) {
						((CINFSelect*)m_pParent)->SetMouseState( SELECT_MOUSE_STATE_NORMAL ); }
				}
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int CINFSelectMenu::OnButtonClicked(int i)
{
	FLOG( "CINFSelectMenu::OnButtonClicked(int i)" );
	switch(i)
	{
	case SEL_BUTTON_NEW:
		{
			if(((CINFSelect*)m_pParent)->GetCharacterNumber()<3)
			{				
				m_nButtonState[SEL_BUTTON_NEW] = SEL_BUTTON_STATE_NORMAL;
				((CINFSelect*)m_pParent)->m_pSelectBack->ChangeMode(CREATE_MODE);
				((CINFSelect*)m_pParent)->m_pSelectBack->m_bInitEffectItem = false;
				g_pD3dApp->DeleteEffectList();
				g_pD3dApp->ChangeGameState(_CREATE);

				// 2007-07-04 by bhsohn 오퍼레이터 추가
				((CINFSelect*)m_pParent)->InitNPCAlphaColor();
			}
		}
		break;
	case SEL_BUTTON_QUIT:
		{
            SendMessage( g_pD3dApp->GetHwnd(), WM_CLOSE, 0, 0 );
			//g_pD3dApp->ChangeGameState(_ENDGAME);
		}
		break;
	case SEL_BUTTON_OK:
		{		
			// 2009. 10. 14 by jskim 프리스카 제거
			((CINFSelect*)m_pParent)->GameStart(((CINFSelect*)m_pParent)->m_character[((CINFSelect*)m_pParent)->GetCurrentSelectIndex()].Material);
			//end 2009. 10. 14 by jskim 프리스카 제거
		}
		break;
	case SEL_BUTTON_OPTION:
		{
			((CINFSelect*)m_pParent)->ShowOption(TRUE);
		}
		break;
	case SEL_BUTTON_DELETE:
		{
			// 2007-09-12 by bhsohn 2차 암호 시스템 구현
			//((CINFSelect*)m_pParent)->DeleteUnit();
			if((TRUE == g_pD3dApp->IsUseSecondaryPasswordSystem())
				&& (TRUE == g_pD3dApp->IsSetSecondaryPassword()))
			{
				((CINFSelect*)m_pParent)->ShowSecondPassword();
			}
			else
			{
				((CINFSelect*)m_pParent)->DeleteUnit();
			}			
		}
		break;
	}
	return INF_MSGPROC_BREAK;
}
