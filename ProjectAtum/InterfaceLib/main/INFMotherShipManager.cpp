// INFMotherShipManager.cpp: implementation of the CINFMotherShipManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"

#include "INFMotherShipManager.h"
#include "ShuttleChild.h"
#include "Chat.h"
#include "StoreData.h"// 2008-08-19 by bhsohn 세력 마크 시스템 추가
#include "INFWindow.h"
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"											// 2011. 10. 10 by jskim UI시스템 변경
#include "INFToolTip.h"			// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
#include "Interface.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define MOTHERSHIP_IMPORTANT_GAP			53
#define MOTHERSHIP_BUTTON_IMPORTANT_GAP		20

#define MOTHERSHIP_OPTION_ATT_COUNT			5
#define MOTHERSHIP_OPTION_DEF_COUNT			2

#define MOTHERSHIP_OPTION_CHECK_ATT_X		(m_nOptionPosX + 25)
#define MOTHERSHIP_OPTION_CHECK_ATT_Y		(m_nOptionPosY + 42)
#define MOTHERSHIP_OPTION_CHECK_ATT_GAP		17

#define MOTHERSHIP_OPTION_CHECK_DEF_X		(m_nOptionPosX + 25)
#define MOTHERSHIP_OPTION_CHECK_DEF_Y		(m_nOptionPosY + 155)
#define MOTHERSHIP_OPTION_CHECK_DEF_GAP		17

#define MOTHERSHIP_OPTION_CHECK_ORDER_X		(m_nOptionPosX + 56)
#define MOTHERSHIP_OPTION_CHECK_ORDER_Y		(m_nOptionPosY + 194)

#define MOTHERSHIP_OPTION_CHECK_W			20

#define MOTHERSHIP_OPTION_OK_X				(m_nOptionPosX + 75)
#define MOTHERSHIP_OPTION_OK_Y				(m_nOptionPosY + 220)
#define MOTHERSHIP_OPTION_CANCEL_X			(m_nOptionPosX + 121)
#define MOTHERSHIP_OPTION_CANCEL_Y			(m_nOptionPosY + 220)

#define MOTHERSHIP_OPTION_BG_W					(234)
#define MOTHERSHIP_OPTION_BG_H					(251)

#define MOTHERSHIP_OPTON_BAR_W					MOTHERSHIP_OPTION_BG_W
#define MOTHERSHIP_OPTON_BAR_H					20

#define MOTHERSHIP_OPTION_CHANGE_GAP			60.0f


// 2007-02-06 by bhsohn 모선전 진행 사항 체크
// 모선이 출격한 첫 진형
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_X					187
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y					45//22

#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_X					187
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y					100//77

#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_INFLUENCE_START_X		185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_INFLUENCE_START_Y		(GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y + 3)//25
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_TIME_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_TIME_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y + 18)//40
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_AREA_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_AREA_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y + 33)//55

#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_INFLUENCE_START_X		185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_INFLUENCE_START_Y		(GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y + 3)//80
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_TIME_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_TIME_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y + 18)//95
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_AREA_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_AREA_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y + 33)//110

#define GAMEMAIN_MOTHERSHIP_INFLUENCE_WIDTH						145
#define GAMEMAIN_MOTHERSHIP_INFLUENCE_EACH_HEIGHT				15		// 각각의 글씨 높이 
#define GAMEMAIN_MOTHERSHIP_INFLUENCE_CAP_HEIGHT				10		// 진행 표시시 여백 
#else
#define MOTHERSHIP_IMPORTANT_GAP			53
#define MOTHERSHIP_BUTTON_IMPORTANT_GAP		20

#define MOTHERSHIP_OPTION_ATT_COUNT			5
#define MOTHERSHIP_OPTION_DEF_COUNT			2

#define MOTHERSHIP_OPTION_CHECK_ATT_X		(m_nOptionPosX + 25)
#define MOTHERSHIP_OPTION_CHECK_ATT_Y		(m_nOptionPosY + 42)
#define MOTHERSHIP_OPTION_CHECK_ATT_GAP		17

#define MOTHERSHIP_OPTION_CHECK_DEF_X		(m_nOptionPosX + 25)
#define MOTHERSHIP_OPTION_CHECK_DEF_Y		(m_nOptionPosY + 155)
#define MOTHERSHIP_OPTION_CHECK_DEF_GAP		17

#define MOTHERSHIP_OPTION_CHECK_ORDER_X		(m_nOptionPosX + 56)
#define MOTHERSHIP_OPTION_CHECK_ORDER_Y		(m_nOptionPosY + 194)

#define MOTHERSHIP_OPTION_CHECK_W			20

#define MOTHERSHIP_OPTION_OK_X				(m_nOptionPosX + 75)
#define MOTHERSHIP_OPTION_OK_Y				(m_nOptionPosY + 220)
#define MOTHERSHIP_OPTION_CANCEL_X			(m_nOptionPosX + 121)
#define MOTHERSHIP_OPTION_CANCEL_Y			(m_nOptionPosY + 220)

#define MOTHERSHIP_OPTION_BG_W					(234)
#define MOTHERSHIP_OPTION_BG_H					(251)

#define MOTHERSHIP_OPTON_BAR_W					MOTHERSHIP_OPTION_BG_W
#define MOTHERSHIP_OPTON_BAR_H					20

#define MOTHERSHIP_OPTION_CHANGE_GAP			60.0f


// 2007-02-06 by bhsohn 모선전 진행 사항 체크
// 모선이 출격한 첫 진형
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_X					179
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y					45//22

#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_X					179
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y					100//77

#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_INFLUENCE_START_X		185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_INFLUENCE_START_Y		(GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y + 3)//25
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_TIME_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_TIME_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y + 18)//40
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_AREA_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE1_AREA_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y + 33)//55

#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_INFLUENCE_START_X		185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_INFLUENCE_START_Y		(GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y + 3)//80
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_TIME_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_TIME_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y + 18)//95
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_AREA_START_X				185
#define GAMEMAIN_MOTHERSHIP_INFLUENCE2_AREA_START_Y				(GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y + 33)//110

#define GAMEMAIN_MOTHERSHIP_INFLUENCE_WIDTH						145
#define GAMEMAIN_MOTHERSHIP_INFLUENCE_EACH_HEIGHT				15		// 각각의 글씨 높이 
#define GAMEMAIN_MOTHERSHIP_INFLUENCE_CAP_HEIGHT				10		// 진행 표시시 여백 
#endif

// 모선전 테두리
#define ID_MOTHERSHIP_BALON_TLH				6
#define ID_MOTHERSHIP_BALON_TLW				8
#define ID_MOTHERSHIP_BALON_TMH				6
#define ID_MOTHERSHIP_BALON_TMW				1
#define ID_MOTHERSHIP_BALON_TRH				6
#define ID_MOTHERSHIP_BALON_TRW				6

#define ID_MOTHERSHIP_BALON_MLH				1
#define ID_MOTHERSHIP_BALON_MLW				8
#define ID_MOTHERSHIP_BALON_MMH				1
#define ID_MOTHERSHIP_BALON_MMW				1
#define ID_MOTHERSHIP_BALON_MRH				1
#define ID_MOTHERSHIP_BALON_MRW				6

#define ID_MOTHERSHIP_BALON_BLH				6
#define ID_MOTHERSHIP_BALON_BLW				8
#define ID_MOTHERSHIP_BALON_BMH				6
#define ID_MOTHERSHIP_BALON_BMW				1
#define ID_MOTHERSHIP_BALON_BRH				6
#define ID_MOTHERSHIP_BALON_BRW				6

// end 2007-02-06 by bhsohn 모선전 진행 사항 체크


CINFMotherShipManager::CINFMotherShipManager(CGameData * pData)
{
	m_pGameData				= pData;

	m_nShowNum1				= 0;
	m_nShowNum0				= 0;


	m_fShowTime				= 0.0f;

	m_nButtonState1			= BUTTON_STATE_NORMAL;
	m_nButtonState0			= BUTTON_STATE_NORMAL;

	m_bShow1				= FALSE;
	m_bShow0				= FALSE;

	m_nButtonPosX1			= 0;
	m_nButtonPosY1			= 0;
	m_nButtonPosX0			= 0;
	m_nButtonPosY0			= 0;
	m_pImgMotherShipBCUButton		= NULL;
	m_pImgMotherShipANIButton		= NULL;

	m_bOptionAct			= FALSE;

	m_pImgOptionBG			= NULL;
	m_pImgCheckB[0]			= NULL;
	m_pImgCheckB[1]			= NULL;
	m_pImgOKB				= NULL;
	m_pImgCancelB			= NULL;

	m_nOptionPosX			= 0;
	m_nOptionPosY			= 0;

	m_bLButtonClick			= FALSE;

	stSTRONGHOLD_INFO  _MAPORDER_BCU[] = 
	{
		{	10,		3067},
		{	20,		3004},
		{	30,		3005},
		{	40,		3006},
		{	50,		3063},	
		{	60,		3064 },		// 2008-06-27 by dgwoo °AA¡ Ao¿ª Aß°¡.
		{	70,		3019 },		// 2008-06-27 by dgwoo °AA¡ Ao¿ª Aß°¡.
		{	80,		6007 },		// 2008-06-27 by dgwoo °AA¡ Ao¿ª Aß°¡.
		{	90,		3029},		// 2008-06-27 by dgwoo °AA¡ Ao¿ª Aß°¡.
		{	100,	4010},
		{	110,	3028},
		{	120,	3025},
		{	130,	7001 },
		{	140,	7003 },
		{	150,	3068 },
		{	160,	3024 },
		{	170,	3035 }
	};

	stSTRONGHOLD_INFO  _MAPORDER_ANI[] = 
	{
		{	10,		3014},
		{	20,		3013},
		{	30,		4006},
		{	40,		3009},
		{	50,		3007},
		{	60,		3016 },
		{	70,		3020 },
		{	80,		6008 },
		{	90,		3031},		// 2008-06-27 by dgwoo °AA¡ Ao¿ª Aß°¡.
		{	100,	3015},
		{	110,	3045},
		{   120,    3022},
		{	130,	7002 },
		{	140,	7004 },
		{	150,	3069 },
		{	160,	3021 },
		{	170,	3036 }
	};

	// 2009. 07. 10 by ckPark 일본 거점 9개로 조정
	
	
	DWORD nCount = sizeof(_MAPORDER_BCU)/sizeof(stSTRONGHOLD_INFO);
	int i;
	for(i = 0 ; i < nCount ; i++)
	{
		// 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기
		if(g_pGameMain->GetMapInfo(_MAPORDER_BCU[i].MapIndex)->MapIndex == 0)
			continue;
		// end 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기
		
		
		CINFImportantInfo* pImportInfo = new CINFImportantInfo;
		pImportInfo->SetGameData(pData);
		pImportInfo->SetMapNum(_MAPORDER_BCU[i].MapIndex);
		m_vecImportBCU.push_back(pImportInfo);
		
	}
	nCount = sizeof(_MAPORDER_ANI)/sizeof(stSTRONGHOLD_INFO);
	for(i = 0 ; i < nCount ; i++)
	{
		// 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기
		if(g_pGameMain->GetMapInfo(_MAPORDER_ANI[i].MapIndex)->MapIndex == 0)
			continue;
		// end 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기


		CINFImportantInfo* pImportInfo = new CINFImportantInfo;
		pImportInfo->SetGameData(pData);
		pImportInfo->SetMapNum(_MAPORDER_ANI[i].MapIndex);
		m_vecImportANI.push_back(pImportInfo);
	}

	// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	int nCnt = 0;
	for(nCnt = 0; nCnt < MOTHERSHIP_MAX_INFLUENCE;nCnt++)
	{
		memset(m_struMotherShipInfo[nCnt].szInfluence, 0x00, MAX_PATH);
		memset(m_struMotherShipInfo[nCnt].szReMainTime, 0x00, MAX_PATH);
		memset(m_struMotherShipInfo[nCnt].szArea, 0x00, MAX_PATH);		
		m_struMotherShipInfo[nCnt].byBelligerence = 0;
		m_struMotherShipInfo[nCnt].fMotherShipRemainTime = 0;

		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
		m_struMotherShipInfo[nCnt].nContributionPoint = 0;
		m_struMotherShipInfo[nCnt].SummonMonsterUnitkind = 0;
		memset(&m_struMotherShipInfo[nCnt].SummonMonsterTime, 0x00, sizeof(ATUM_DATE_TIME));
		// end 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경

		memset(&m_struPreAddMotherShip[nCnt], 0x00 ,sizeof(PREADD_MOTHERSHIP_INFO));
	}


	for(nCnt = 0;nCnt < MOTHERSHIP_MAXLINE;nCnt++)
	{
		m_pFontMotherShipInfo[nCnt]= NULL;
	}
	for(nCnt=0; nCnt<MOTHERSHIP_INFO_BALON; nCnt++)
	{
		m_pBalonChat[nCnt] = NULL;
	}
	for(nCnt = 0; nCnt < MOTHERSHIP_MAX_INFLUENCE; nCnt++)
	{
		m_bSummonMotherShipInfo[nCnt] = FALSE;		// 현재 소환돼있는 모선			
	}	
	m_nSummonMotherShipCnt = 0;
	m_nPreAddMothershipCnt = 0;	
	// end 2007-02-06 by bhsohn 모선전 진행 사항 체크	

	// 2007-02-15 by bhsohn 모선 자동 소멸시 이벤트 받음.
	memset(&m_sizeMotherShipInfo, 0x00, sizeof(SIZE));
    m_bFirstOptionSetting = FALSE;
	m_fTimeOptionChange = -1.0f;



	// 2009. 01. 12 by ckPark 선전 포고 시스템
	m_pWarDeclareWindow		= NULL;
	// end 2009. 01. 12 by ckPark 선전 포고 시스템
}
CINFMotherShipManager::CINFMotherShipManager()
{
}
CINFMotherShipManager::~CINFMotherShipManager()
{
	vecINFImportantInfoIter it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		util::del((*it));
		it = m_vecImportBCU.erase(it);
	}
	it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		util::del((*it));
		it = m_vecImportANI.erase(it);
	}
	// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	int i =0;
	for(i=0;i<MOTHERSHIP_MAXLINE;i++)
	{
		util::del(m_pFontMotherShipInfo[i]);
	}
	for(i=0; i<MOTHERSHIP_INFO_BALON; i++)
	{
		util::del(m_pBalonChat[i]);
	}
	// end 2007-02-06 by bhsohn 모선전 진행 사항 체크

}
void CINFMotherShipManager::AdventMotherShipBCU(SMSWARINFO_DISPLAY * pInfo)
{
	vecINFImportantInfoIter it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		if((*it)->m_nMapNum == pInfo->MapIndex)
		{
			(*it)->m_bMapInfluence		= pInfo->MapInfluenceType;
			(*it)->SetState(pInfo->TelePortState,pInfo->HPRate);
			if(pInfo->TelePortState == TELEPORT_STATE_BUILDING)
			{
				(*it)->SetActiveTime(pInfo->TelePortBuildingStartTime,pInfo->TelePortBuildingEndTime);
			}
			break;
		}
		it++;
	}
}

void CINFMotherShipManager::AdventMotherShipANI(SMSWARINFO_DISPLAY * pInfo)
{
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		if((*it)->m_nMapNum == pInfo->MapIndex)
		{
			(*it)->m_bMapInfluence		= pInfo->MapInfluenceType;
			(*it)->SetState(pInfo->TelePortState,pInfo->HPRate);
			if(pInfo->TelePortState == TELEPORT_STATE_BUILDING)
			{
				(*it)->SetActiveTime(pInfo->TelePortBuildingStartTime,pInfo->TelePortBuildingEndTime);
			}
			break;
		}
		it++;
	}
}
void CINFMotherShipManager::ResetAdventBCU()
{
	vecINFImportantInfoIter it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->m_fHP				= MS_WAR_HP_RATE;
		(*it)->m_bMapInfluence		= 0;
		(*it)->SetState(TELEPORT_STATE_NOT_SUMMON);
		it++;
	}
	m_bShow0 = TRUE;

}
void CINFMotherShipManager::ResetAdventANI()
{
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->m_fHP				= MS_WAR_HP_RATE;
		(*it)->m_bMapInfluence		= 0;
		(*it)->SetState(TELEPORT_STATE_NOT_SUMMON);
		it++;
	}
	m_bShow1 = TRUE;
}

void CINFMotherShipManager::DestroyMotherShipBCU()
{
	m_nButtonPosX0 = 0;
	m_nButtonPosY0 = 0;
	m_bShow0 = FALSE;
}
void CINFMotherShipManager::DestroyMotherShipANI()
{
	m_nButtonPosX1 = 0;
	m_nButtonPosY1 = 0;
	m_bShow1 = FALSE;
}
HRESULT CINFMotherShipManager::InitDeviceObjects()
{
	
	if(NULL == m_pImgMotherShipBCUButton)
	{
		m_pImgMotherShipBCUButton = new CINFImageBtnBoth;
	}
	m_pImgMotherShipBCUButton->InitDeviceObjects("infoMB");
	m_pImgMotherShipBCUButton->ChangeBoth(!m_bShow0);
	if(NULL == m_pImgMotherShipANIButton)
	{
		m_pImgMotherShipANIButton = new CINFImageBtnBoth;
	}
	m_pImgMotherShipANIButton->InitDeviceObjects("infoMB");
	m_pImgMotherShipANIButton->ChangeBoth(!m_bShow1);

	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->InitDeviceObjects();
		it++;
	}
	it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->InitDeviceObjects();
		it++;
	}
	//--------------------------------------------------------------------------//
	//						Option 
	DataHeader	* pDataHeader;
	char buf[64];

	if(NULL == m_pImgOKB)
	{
		m_pImgOKB = new CINFImageBtn;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgOKB->InitDeviceObjects("oks0","STRTOOLTIP9");
#else
	m_pImgOKB->InitDeviceObjects("shlaok0");
#endif

	if(NULL == m_pImgCancelB)
	{
		m_pImgCancelB = new CINFImageBtn;
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgCancelB->InitDeviceObjects("cans0","STRTOOLTIP41");
#else
	m_pImgCancelB->InitDeviceObjects("shmcan0");
#endif
	if(NULL == m_pImgCheckB[0])
	{
		wsprintf(buf,"radio_17a");
		pDataHeader = FindResource(buf);
		if(pDataHeader == NULL)
		{
			return S_FALSE;
		}
		m_pImgCheckB[0] = new CINFImageEx;
		m_pImgCheckB[0]->InitDeviceObjects( pDataHeader );
		
		wsprintf(buf,"radio_17b");
		pDataHeader = FindResource(buf);
		if(pDataHeader == NULL)
		{
			return S_FALSE;
		}
		m_pImgCheckB[1] = new CINFImageEx;
		m_pImgCheckB[1]->InitDeviceObjects( pDataHeader );
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("MS_option");
	m_pImgOptionBG = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pImgOptionBG->InitDeviceObjects( g_pD3dApp->m_pImageList );
#else
	wsprintf(buf,"mopt");
	pDataHeader = FindResource(buf);
	if(pDataHeader == NULL)
	{
		return S_FALSE;
	}
	m_pImgOptionBG = new CINFImageEx;
	m_pImgOptionBG->InitDeviceObjects( pDataHeader );
#endif
	{
		// 2007-02-06 by bhsohn 모선전 진행 사항 체크
		int i =0;
		for(i=0;i<MOTHERSHIP_MAXLINE;i++)
		{		
			m_pFontMotherShipInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE, TRUE, 256, 32);
			m_pFontMotherShipInfo[i]->InitDeviceObjects(g_pD3dDev) ;
		}	
		
		char szImage[16];
		for(i=0; i<MOTHERSHIP_INFO_BALON; i++)
		{		
			memset(szImage, 0x00, 16);		
			wsprintf(szImage,"c_bal%d",i);
			
			m_pBalonChat[i] = new CINFImageEx;
			pDataHeader = FindResource(szImage);
			m_pBalonChat[i]->InitDeviceObjects(pDataHeader ) ;		
		}	
		// end 2007-02-06 by bhsohn 모선전 진행 사항 체크
	}


	// 2009. 01. 12 by ckPark 선전 포고 시스템
	m_pWarDeclareWindow	= new CINFWarDeclare(m_pGameData);
	m_pWarDeclareWindow->InitDeviceObjects();
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	m_bFirstOptionSetting = FALSE;
	
	return S_OK ;
}
HRESULT CINFMotherShipManager::RestoreDeviceObjects()
{
	m_pImgMotherShipBCUButton->RestoreDeviceObjects();	
	m_pImgMotherShipANIButton->RestoreDeviceObjects();	
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->RestoreDeviceObjects();
		it++;
	}
	it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->RestoreDeviceObjects();
		it++;
	}
		if(NULL == m_pImgOKB)
	{
		m_pImgOKB = new CINFImageBtn;
	}


	m_pImgOKB->RestoreDeviceObjects();
	m_pImgOKB->SetBtnPosition(MOTHERSHIP_OPTION_OK_X,MOTHERSHIP_OPTION_OK_Y);
	m_pImgCancelB->RestoreDeviceObjects();
	m_pImgCancelB->SetBtnPosition(MOTHERSHIP_OPTION_CANCEL_X,MOTHERSHIP_OPTION_CANCEL_Y);
	
	m_pImgCheckB[0]->RestoreDeviceObjects();
	m_pImgCheckB[1]->RestoreDeviceObjects();
	m_pImgOptionBG->RestoreDeviceObjects();
	
	{
		// 2007-02-06 by bhsohn 모선전 진행 사항 체크
		int i=0;
		for(i=0;i<MOTHERSHIP_MAXLINE;i++)
		{
			m_pFontMotherShipInfo[i]->RestoreDeviceObjects() ;
		}
		for(i=0; i<MOTHERSHIP_INFO_BALON; i++)
		{
			m_pBalonChat[i]->RestoreDeviceObjects();
		}
		// end 2007-02-06 by bhsohn 모선전 진행 사항 체크
	}

	// 2009. 01. 12 by ckPark 선전 포고 시스템
	m_pWarDeclareWindow->RestoreDeviceObjects();
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	return S_OK ;
}
HRESULT CINFMotherShipManager::DeleteDeviceObjects()
{
	m_pImgMotherShipBCUButton->DeleteDeviceObjects();	
	util::del(m_pImgMotherShipBCUButton);
	m_pImgMotherShipANIButton->DeleteDeviceObjects();	
	util::del(m_pImgMotherShipANIButton);
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del((*it));
		it++;
	}
	it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->DeleteDeviceObjects();
		util::del((*it));
		it++;
	}
	m_pImgOKB->DeleteDeviceObjects();
	util::del(m_pImgOKB);
	m_pImgCancelB->DeleteDeviceObjects();
	util::del(m_pImgCancelB);
	
	m_pImgCheckB[0]->DeleteDeviceObjects();
	util::del(m_pImgCheckB[0]);
	m_pImgCheckB[1]->DeleteDeviceObjects();
	util::del(m_pImgCheckB[1]);
	m_pImgOptionBG->DeleteDeviceObjects();
	util::del(m_pImgOptionBG);
	{
		// 2007-02-06 by bhsohn 모선전 진행 사항 체크
		int i=0;
		for(i=0;i<MOTHERSHIP_MAXLINE;i++)
		{		
			m_pFontMotherShipInfo[i]->DeleteDeviceObjects() ;
			util::del(m_pFontMotherShipInfo[i]);
		}
		for(i=0; i<MOTHERSHIP_INFO_BALON; i++)
		{
			m_pBalonChat[i]->DeleteDeviceObjects();
			util::del(m_pBalonChat[i]);
		}
		// end 2007-02-06 by bhsohn 모선전 진행 사항 체크
	}

	// 2009. 01. 12 by ckPark 선전 포고 시스템
	if(m_pWarDeclareWindow)
	{
		m_pWarDeclareWindow->DeleteDeviceObjects();
		util::del(m_pWarDeclareWindow);
	}
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	return S_OK ;
}
HRESULT CINFMotherShipManager::InvalidateDeviceObjects()
{
	m_pImgMotherShipBCUButton->InvalidateDeviceObjects();	
	m_pImgMotherShipANIButton->InvalidateDeviceObjects();	
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->InvalidateDeviceObjects();
		it++;
	}
	it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->InvalidateDeviceObjects();
		it++;
	}

	m_pImgOKB->InvalidateDeviceObjects();
	m_pImgCancelB->InvalidateDeviceObjects();
	
	m_pImgCheckB[0]->InvalidateDeviceObjects();
	m_pImgCheckB[1]->InvalidateDeviceObjects();
	m_pImgOptionBG->InvalidateDeviceObjects();
	{
		// 2007-02-06 by bhsohn 모선전 진행 사항 체크
		int i=0;
		for(i=0;i<MOTHERSHIP_MAXLINE;i++)
		{
			m_pFontMotherShipInfo[i]->InvalidateDeviceObjects() ;
		}
		for(i=0; i<MOTHERSHIP_INFO_BALON; i++)
		{
			m_pBalonChat[i]->InvalidateDeviceObjects();
		}
		// end 2007-02-06 by bhsohn 모선전 진행 사항 체크
	}

	// 2009. 01. 12 by ckPark 선전 포고 시스템
	m_pWarDeclareWindow->InvalidateDeviceObjects();
	// end 2009. 01. 12 by ckPark 선전 포고 시스템

	return S_OK ;
}
BOOL CINFMotherShipManager::SetButtonPos1(UINT i_nPosX,UINT i_nPosY)
{

	m_nButtonPosX1 = i_nPosX;
	m_nButtonPosY1 = i_nPosY;
	m_pImgMotherShipANIButton->SetBtnPosition(m_nButtonPosX1,m_nButtonPosY1);

	// 이미지위치를 셋팅.

	UINT nPosX = i_nPosX + MOTHERSHIP_BUTTON_IMPORTANT_GAP;
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->SetImportantPos(nPosX,i_nPosY);
		nPosX += MOTHERSHIP_IMPORTANT_GAP;
		it++;
	}

	return TRUE;
}
BOOL CINFMotherShipManager::SetButtonPos0(UINT i_nPosX,UINT i_nPosY)
{
	m_nButtonPosX0 = i_nPosX;
	m_nButtonPosY0 = i_nPosY;
	m_pImgMotherShipBCUButton->SetBtnPosition(m_nButtonPosX0,m_nButtonPosY0);

	// 이미지위치를 셋팅.
	UINT nPosX = i_nPosX + MOTHERSHIP_BUTTON_IMPORTANT_GAP;
	vecINFImportantInfoIter it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->SetImportantPos(nPosX,i_nPosY);
		nPosX += MOTHERSHIP_IMPORTANT_GAP;
		it++;
	}
	return TRUE;
}
void CINFMotherShipManager::RenderOption()
{
	if(!m_bOptionAct)
	{
		return;
	}
	m_pImgOptionBG->Move(m_nOptionPosX,m_nOptionPosY);
	m_pImgOptionBG->Render();
	CINFImage* pImg;
	// 활성화 완료.
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_ATT_ACTIVATED))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_ATT_X,MOTHERSHIP_OPTION_CHECK_ATT_Y);
	pImg->Render();

	// 활성화 진행중 
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_ATT_X,
		MOTHERSHIP_OPTION_CHECK_ATT_Y + MOTHERSHIP_OPTION_CHECK_ATT_GAP);
	pImg->Render();
	// 활성화 시간 표시.
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_ATT_ACTIVE_TIME))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_ATT_X,
		MOTHERSHIP_OPTION_CHECK_ATT_Y+(MOTHERSHIP_OPTION_CHECK_ATT_GAP*2));
	pImg->Render();

	// 텔레포트 HP표시.
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_ATT_TELEPORT_HP))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_ATT_X,
		MOTHERSHIP_OPTION_CHECK_ATT_Y+(MOTHERSHIP_OPTION_CHECK_ATT_GAP*3));
	pImg->Render();

	// 거점 생존여부(공)
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_ATT_STRATEGYPOINT_LIVE))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_ATT_X,
		MOTHERSHIP_OPTION_CHECK_ATT_Y+(MOTHERSHIP_OPTION_CHECK_ATT_GAP*4));
	pImg->Render();

	// 거점 HP표시(방)
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_HP))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_DEF_X,
		MOTHERSHIP_OPTION_CHECK_DEF_Y);
	pImg->Render();

	// 거점 생존여부(방)
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_DEF_STRATEGYPOINT_LIVE))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_DEF_X,
		MOTHERSHIP_OPTION_CHECK_DEF_Y+(MOTHERSHIP_OPTION_CHECK_DEF_GAP));
	pImg->Render();
	
	// 지도자 적용.
	if(IS_MS_WAR_OPTION_TYPE(m_nTempOption,MS_WAR_OPTION_TYPE_LEADER))
		pImg = m_pImgCheckB[0];
	else
		pImg = m_pImgCheckB[1];
	pImg->Move(MOTHERSHIP_OPTION_CHECK_ORDER_X,
		MOTHERSHIP_OPTION_CHECK_ORDER_Y);
	pImg->Render();

	m_pImgOKB->SetBtnPosition(MOTHERSHIP_OPTION_OK_X,MOTHERSHIP_OPTION_OK_Y);
	m_pImgCancelB->SetBtnPosition(MOTHERSHIP_OPTION_CANCEL_X,MOTHERSHIP_OPTION_CANCEL_Y);
	m_pImgOKB->Render();
	m_pImgCancelB->Render();
}
void CINFMotherShipManager::Render()
{
// 2008-04-04 by dgwoo GameMain에서 렌더링.
//	if(m_bOptionAct)
//	{
//		RenderOption();
//	}
	// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	// 모선전 관련 정보를 랜더링 한다.
	RenderMothershipInfo();
	// end 2007-02-06 by bhsohn 모선전 진행 사항 체크

	int nCount = 0;
	if((m_nButtonPosX0 != 0) && (m_nButtonPosY0 != 0))
	{
		m_pImgMotherShipBCUButton->Render();
		if(m_bShow0)
		{
			vecINFImportantInfoIter it = m_vecImportBCU.begin();
			while(it != m_vecImportBCU.end())
			{
				if(m_nShowNum0 <= nCount)
				{
					break;
				}
				(*it)->Render();
				nCount++;
				it++;
			}
		}
	}

	if((m_nButtonPosX1 != 0) && (m_nButtonPosY1 != 0))
	{
		m_pImgMotherShipANIButton->Render();
		if(m_bShow1)
		{		
			nCount = 0;
			vecINFImportantInfoIter it = m_vecImportANI.begin();
			while(it != m_vecImportANI.end())
			{
				if(m_nShowNum1 <= nCount)
				{
					break;
				}
				(*it)->Render();
				nCount++;
				it++;
			}
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFMotherShipManager::TickShowIng()
/// \brief		하나의 거점정보가 하나씩 보여지도록.
/// \author		dgwoo
/// \date		2008-03-31 ~ 2008-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::TickShowIng()
{
	if(m_fShowTime > SLIDE_TIME)
	{
		m_fShowTime = 0;
		if(m_bShow0)
		{// Show를 시킬 경우 체크한다.
			if(m_vecImportBCU.size() > m_nShowNum0)
			{
				m_nShowNum0++;
			}
		}
		else
		{
			m_nShowNum0 = 0;
//			if(m_nShowNum0 <= 0)
//			{ 
//				m_nShowNum0 = 0;
//			}
//			else
//			{
//				m_nShowNum0--;
//			}
		}
		if(m_bShow1)
		{// Show를 시킬 경우 체크한다.
			if(m_vecImportANI.size() > m_nShowNum1)
			{
				m_nShowNum1++;
			}
		}
		else
		{
			m_nShowNum1 = 0;
//			if(m_nShowNum1 <= 0)
//			{ 
//				m_nShowNum1 = 0;
//			}
//			else
//			{
//				m_nShowNum1--;
//			}
		}
	}
}
void CINFMotherShipManager::Tick()
{
	m_fShowTime += g_pD3dApp->GetElapsedTime();
	m_fTimeOptionChange -= g_pD3dApp->GetElapsedTime();
	if(m_fTimeOptionChange < 0)
		m_fTimeOptionChange = -1.0f;
	TickShowIng();
	
	vecINFImportantInfoIter it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->Tick();
		it++;
	}
	it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->Tick();
		it++;
	}
	// 2007-02-06 by bhsohn 모선전 진행 사항 체크
	TickMotherShipInfo();
	// end 2007-02-06 by bhsohn 모선전 진행 사항 체크
}
USHORT CINFMotherShipManager::SetMotherShipOption(USHORT i_Option)
{
	USHORT OldOption = m_nOption;

	vecINFImportantInfoIter it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		(*it)->SetOption(i_Option);
		it++;
	}

	it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		(*it)->SetOption(i_Option);
		it++;
	}
	m_nOption = i_Option;
	return OldOption;
}
void CINFMotherShipManager::ShowWindowOption()
{
	if (!COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_INFLUENCE_LEADER | RACE_GAMEMASTER | RACE_OPERATION | RACE_MONITOR))
	{
		return;
	}
	m_bOptionAct		= TRUE; 
	m_nOptionPosX = (g_pD3dApp->GetBackBufferDesc().Width - MOTHERSHIP_OPTION_BG_W)/2;
	m_nOptionPosY = (g_pD3dApp->GetBackBufferDesc().Height - MOTHERSHIP_OPTION_BG_H)/2;


	m_nTempOption = m_nOption; 
}
void CINFMotherShipManager::HideWindowOption()
{
	m_bOptionAct		= FALSE;
	// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
	g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현

}
UINT CINFMotherShipManager::OptionWindowEvent(POINT &i_pt)
{
	UINT bTempOption = m_nTempOption;//m_nOption;
	int i;
	if(i_pt.x > MOTHERSHIP_OPTION_CHECK_ATT_X &&
		i_pt.x < MOTHERSHIP_OPTION_CHECK_ATT_X + MOTHERSHIP_OPTION_CHECK_W)
	{
		// ATT
		for(i = 0 ; i < MOTHERSHIP_OPTION_ATT_COUNT ; i++)
		{
			if(i_pt.y > MOTHERSHIP_OPTION_CHECK_ATT_Y &&
				i_pt.y < MOTHERSHIP_OPTION_CHECK_ATT_Y + (MOTHERSHIP_OPTION_CHECK_ATT_GAP*(i+1)))
			{
				break;
			}
		}
		if(i < MOTHERSHIP_OPTION_ATT_COUNT)
			bTempOption ^= (1 << (i+1));

		// DEF
		for(i = 0 ; i < MOTHERSHIP_OPTION_DEF_COUNT ; i++)
		{
			if(i_pt.y > MOTHERSHIP_OPTION_CHECK_DEF_Y &&
				i_pt.y < MOTHERSHIP_OPTION_CHECK_DEF_Y + (MOTHERSHIP_OPTION_CHECK_DEF_GAP*(i+1)))
			{
				break;
			}
		}
		if(i < MOTHERSHIP_OPTION_DEF_COUNT)
			bTempOption ^= (1 << (i+MOTHERSHIP_OPTION_ATT_COUNT+1));
	}




	// 지휘부 
	if(i_pt.x > MOTHERSHIP_OPTION_CHECK_ORDER_X &&
		i_pt.x < MOTHERSHIP_OPTION_CHECK_ORDER_X + MOTHERSHIP_OPTION_CHECK_W &&
		i_pt.y > MOTHERSHIP_OPTION_CHECK_ORDER_Y &&
		i_pt.y < MOTHERSHIP_OPTION_CHECK_ORDER_Y + MOTHERSHIP_OPTION_CHECK_DEF_GAP)
	{
		bTempOption ^= MS_WAR_OPTION_TYPE_LEADER;
	}

	return bTempOption;
}
int	CINFMotherShipManager::WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_bOptionAct)
	{
		return INF_MSGPROC_NORMAL;
	}

	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				if(m_bLButtonClick)
				{
					m_nOptionPosX += pt.x - m_ptOldPoint.x;
					m_nOptionPosY += pt.y - m_ptOldPoint.y;
					POINT ptImg = m_pImgOptionBG->GetImgSize();
					
					if(m_nOptionPosX < 0)
					{
						m_nOptionPosX = 0;
					}
					else if(m_nOptionPosX + ptImg.x > g_pD3dApp->GetWidth())
					{
						m_nOptionPosX = g_pD3dApp->GetWidth() - ptImg.x;
					}
					if(m_nOptionPosY < 0)
					{
						m_nOptionPosY = 0;
					}
					else if(m_nOptionPosY + ptImg.y > g_pD3dApp->GetHeight())
					{
						m_nOptionPosY = g_pD3dApp->GetHeight() - ptImg.y;
					}
					m_ptOldPoint = pt;
				}

				if(m_pImgOKB->OnMouseMove(pt))
				{
				}
				if(m_pImgCancelB->OnMouseMove(pt))
				{
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				if(pt.x > m_nOptionPosX &&
					pt.y > m_nOptionPosY &&
					pt.x < m_nOptionPosX + MOTHERSHIP_OPTON_BAR_W &&
					pt.y < m_nOptionPosY + MOTHERSHIP_OPTON_BAR_H)
				{
					m_bLButtonClick = TRUE;
					m_ptOldPoint = pt;
				}
				if(m_pImgOKB->OnLButtonDown(pt))
				{
				}
				if(m_pImgCancelB->OnLButtonDown(pt))
				{
				}
				m_nTempOption = OptionWindowEvent(pt);
				if(pt.x > m_nOptionPosX &&
					pt.y > m_nOptionPosY &&
					pt.x < m_nOptionPosX + m_pImgOptionBG->GetImgSize().x &&
					pt.y < m_nOptionPosY + m_pImgOptionBG->GetImgSize().y)
					return INF_MSGPROC_BREAK;
				return INF_MSGPROC_BREAK;
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				m_bLButtonClick = FALSE;
				if(m_pImgOKB->OnLButtonUp(pt))
				{
					HideWindowOption();
					SendMotherShipOption();
					return INF_MSGPROC_BREAK;
				}
				if(m_pImgCancelB->OnLButtonUp(pt))
				{
					HideWindowOption();
					return INF_MSGPROC_BREAK;
				}
				return INF_MSGPROC_BREAK;
			}
			break;
	}

	return INF_MSGPROC_NORMAL;
}

int	CINFMotherShipManager::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(m_pImgMotherShipBCUButton->OnMouseMove(pt))
				{
				}
				if(m_pImgMotherShipANIButton->OnMouseMove(pt))
				{
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				if(m_pImgMotherShipBCUButton->OnLButtonDown(pt))
				{
					return INF_MSGPROC_BREAK;
				}
				if(m_pImgMotherShipANIButton->OnLButtonDown(pt))
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
				if(m_pImgMotherShipBCUButton->OnLButtonUp(pt))
				{
					m_bShow0 = !m_bShow0;
					m_pImgMotherShipBCUButton->ChangeBoth(m_bShow0);
					return INF_MSGPROC_BREAK;
				}
				if(m_pImgMotherShipANIButton->OnLButtonUp(pt))
				{
					m_bShow1 = !m_bShow1;
					m_pImgMotherShipANIButton->ChangeBoth(m_bShow1);
					return INF_MSGPROC_BREAK;
				}
			}
			break;
	}

	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void RenderMothershipInfo()
/// \brief		모선전 관련 정보를 랜더링 한다.
/// \author		bhsohn
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::RenderMothershipInfo()
{
	if(m_nSummonMotherShipCnt <= 0 )
	{
		return;
	}
	POINT ptPos[MOTHERSHIP_MAXLINE] =
	{
		{GAMEMAIN_MOTHERSHIP_INFLUENCE1_INFLUENCE_START_X,	GAMEMAIN_MOTHERSHIP_INFLUENCE1_INFLUENCE_START_Y},
		{GAMEMAIN_MOTHERSHIP_INFLUENCE1_TIME_START_X,	GAMEMAIN_MOTHERSHIP_INFLUENCE1_TIME_START_Y},
		{GAMEMAIN_MOTHERSHIP_INFLUENCE1_AREA_START_X,	GAMEMAIN_MOTHERSHIP_INFLUENCE1_AREA_START_Y},

		{GAMEMAIN_MOTHERSHIP_INFLUENCE2_INFLUENCE_START_X,	GAMEMAIN_MOTHERSHIP_INFLUENCE2_INFLUENCE_START_Y},
		{GAMEMAIN_MOTHERSHIP_INFLUENCE2_TIME_START_X,	GAMEMAIN_MOTHERSHIP_INFLUENCE2_TIME_START_Y},
		{GAMEMAIN_MOTHERSHIP_INFLUENCE2_AREA_START_X,	GAMEMAIN_MOTHERSHIP_INFLUENCE2_AREA_START_Y}

	};
	DWORD dwColorList[MOTHERSHIP_MAXLINE]	= 
	{
		GUI_FONT_COLOR_YM, GUI_FONT_COLOR, GUI_FONT_COLOR,
		GUI_FONT_COLOR_YM, GUI_FONT_COLOR, GUI_FONT_COLOR
	};	

	if(0 == m_sizeMotherShipInfo.cx )
	{
		m_sizeMotherShipInfo = GetMotherShipInfoFontWidth();
	}	

	int nCnt, nCntSub;
	nCnt = nCntSub = 0;

	int nBalonWidth,nBalonHeight;
	int nItemCnt = 0;
	int nPos = 0;
	nBalonWidth = m_sizeMotherShipInfo.cx;
	nBalonHeight = 0;	
	int nBoldPosX = GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_X;
	int nBoldPosY = GAMEMAIN_MOTHERSHIP_INFLUENCE1_START_Y;

	for(nCnt = 0;nCnt < m_nSummonMotherShipCnt; nCnt++)	
	{		
		nItemCnt =0;
		
		// 진형
		m_pFontMotherShipInfo[nCnt]->DrawText( ptPos[nPos].x, ptPos[nPos].y,
			dwColorList[nPos], m_struMotherShipInfo[nCnt].szInfluence, 0L);		
		nItemCnt++;
		nPos++;
		
		// 남은 시간
		m_pFontMotherShipInfo[nCnt]->DrawText( ptPos[nPos].x, ptPos[nPos].y,
			dwColorList[nPos], m_struMotherShipInfo[nCnt].szReMainTime, 0L);		
		nItemCnt++;
		nPos++;

		// 지역 정보
		m_pFontMotherShipInfo[nCnt]->DrawText( ptPos[nCnt].x, ptPos[nPos].y,
			dwColorList[nPos], m_struMotherShipInfo[nCnt].szArea, 0L);		
		nItemCnt++;
		nPos++;
	
	nBalonHeight = (nItemCnt*GAMEMAIN_MOTHERSHIP_INFLUENCE_EACH_HEIGHT)+GAMEMAIN_MOTHERSHIP_INFLUENCE_CAP_HEIGHT;

		if(nCnt > 0)
		{// 두번째 모선.
			nBoldPosX = GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_X;
			nBoldPosY = GAMEMAIN_MOTHERSHIP_INFLUENCE2_START_Y;
		}

		if(m_struMotherShipInfo[nCnt].byBelligerence == BELL_INFLUENCE_VCN)
		{// 바이제니유.
			SetButtonPos0(nBoldPosX+nBalonWidth+2,nBoldPosY+2);
		}
		else if(m_struMotherShipInfo[nCnt].byBelligerence == BELL_INFLUENCE_ANI)
		{
			SetButtonPos1(nBoldPosX+nBalonWidth+2,nBoldPosY+2);
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		RenderMothershipInfoBalon(nBoldPosX - 8,
#else
		RenderMothershipInfoBalon(nBoldPosX,
#endif
									nBoldPosY,
								nBalonWidth, nBalonHeight);
	}

	// 2008-03-27 by dgwoo 모선전 세부 거점의 정보.
	//m_pMotherShipC->Render();

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void TickMotherShipInfo()
/// \brief		모선전 관련 시간 정보 갱신
/// \author		bhsohn
/// \date		2007-02-07 ~ 2007-02-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::TickMotherShipInfo()
{
	// PreAdd추가할것이 있냐?
	if(m_nPreAddMothershipCnt > 0)
	{
		int nMaxCnt = m_nPreAddMothershipCnt;
		for(int nCnt = 0;nCnt < m_nPreAddMothershipCnt; nCnt++)
		{			
			UpdateMotherShipInfo(m_struPreAddMotherShip[nCnt].byBelligerence, 
									&m_struPreAddMotherShip[nCnt].SummonMonsterTime,
									m_struPreAddMotherShip[nCnt].nContributionPoint, 
									m_struPreAddMotherShip[nCnt].SummonMonsterUnitkind);
			
			memset(&m_struPreAddMotherShip[nCnt], 0x00, sizeof(PREADD_MOTHERSHIP_INFO));
			nMaxCnt--;
		}
		m_nPreAddMothershipCnt = nMaxCnt;
		return;
	}
	if(m_nSummonMotherShipCnt <= 0 )
	{
		// 모선이 안떠 있다.
		return;
	}

	// 2007-02-12 by bhsohn 포커스 잃어 먹었을 떄의 문제 처리
	FLOAT fElaseTime = g_pD3dApp->GetCheckElapsedTime();	
	int nSummonCnt = m_nSummonMotherShipCnt-1;
	int nHour, nMinute, nSecond;
	for(int nSummonIdx = nSummonCnt; nSummonIdx >= 0 ; nSummonIdx--)
	{
		m_struMotherShipInfo[nSummonIdx].fMotherShipRemainTime -= fElaseTime;
		if(m_struMotherShipInfo[nSummonIdx].fMotherShipRemainTime < 0)
		{			
			m_struMotherShipInfo[nSummonIdx].fMotherShipRemainTime = 0;
			// 모선전 종료
			DestroyMotherShipInfo(m_struMotherShipInfo[nSummonIdx].byBelligerence);
			continue;
		}
		
		// 남은 시간 		
		nSecond = m_struMotherShipInfo[nSummonIdx].fMotherShipRemainTime;	
		nHour = (nSecond/3600);
		nSecond -= (nHour*3600);
		nMinute = (nSecond/60);		

		// 2007-07-10 by bhsohn 모선전 5분미만시 표시 스트링 관련 버그 수정
//		if(nMinute >= 5)
//		{
//			wsprintf(m_struMotherShipInfo[nSummonIdx].szReMainTime, STRMSG_C_070206_0202, nHour, nMinute);	
//		}
//		else
//		{
//			wsprintf(m_struMotherShipInfo[nSummonIdx].szReMainTime, STRMSG_C_070206_0203);				
//		}
		if(0 == nHour && nMinute < 5)
		{
			wsprintf(m_struMotherShipInfo[nSummonIdx].szReMainTime, STRMSG_C_070206_0203);
		}
		else
		{
			wsprintf(m_struMotherShipInfo[nSummonIdx].szReMainTime, STRMSG_C_070206_0202, nHour, nMinute);	
		}
		// end 2007-07-10 by bhsohn 모선전 5분미만시 표시 스트링 관련 버그 수정
	}
	//m_pMotherShipC->Tick();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void RenderMothershipInfoBalon(int x, int y, int cx, int cy);
/// \brief		모션전 관련 외곽 테두리 처리
/// \author		bhsohn
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::RenderMothershipInfoBalon(int x, int y, int cx, int cy)
{	

	m_pBalonChat[0]->Move(x,y);
	m_pBalonChat[0]->Render();
	m_pBalonChat[1]->Move(x+ID_MOTHERSHIP_BALON_TLW,y);
	m_pBalonChat[1]->SetScale(cx-(ID_MOTHERSHIP_BALON_TLW), 1);
	m_pBalonChat[1]->Render();
	m_pBalonChat[2]->Move(x+cx - ID_MOTHERSHIP_BALON_TLW+ID_MOTHERSHIP_BALON_TRW+(ID_MOTHERSHIP_BALON_TMW*2),y);
	m_pBalonChat[2]->Render();

	m_pBalonChat[3]->Move(x, y+ID_MOTHERSHIP_BALON_TLH);
	m_pBalonChat[3]->SetScale(1, cy - (ID_MOTHERSHIP_BALON_TLH + ID_MOTHERSHIP_BALON_BLH));
	m_pBalonChat[3]->Render();
	m_pBalonChat[4]->Move(x+ID_MOTHERSHIP_BALON_MLW, y+ID_MOTHERSHIP_BALON_MRW);
	m_pBalonChat[4]->SetScale(cx-(ID_MOTHERSHIP_BALON_TLW), cy - (ID_MOTHERSHIP_BALON_TLH+ID_MOTHERSHIP_BALON_BLH));
	m_pBalonChat[4]->Render();
	m_pBalonChat[5]->Move(x+cx - ID_MOTHERSHIP_BALON_MLW+ID_MOTHERSHIP_BALON_MRW+(ID_MOTHERSHIP_BALON_MMW*2), y+ID_MOTHERSHIP_BALON_TRH);
	m_pBalonChat[5]->SetScale(1, cy - (ID_MOTHERSHIP_BALON_TRH + ID_MOTHERSHIP_BALON_BRH));
	m_pBalonChat[5]->Render();

	m_pBalonChat[6]->Move(x, y+cy-ID_MOTHERSHIP_BALON_BLH);
	m_pBalonChat[6]->Render();
	m_pBalonChat[7]->Move(x + ID_MOTHERSHIP_BALON_BLW, y+cy-ID_MOTHERSHIP_BALON_BLH);
	m_pBalonChat[7]->SetScale(cx-(ID_MOTHERSHIP_BALON_BLW), 1);
	m_pBalonChat[7]->Render();
	m_pBalonChat[8]->Move(x+cx-ID_MOTHERSHIP_BALON_BLW+ID_MOTHERSHIP_BALON_BRW+(ID_MOTHERSHIP_BALON_BMW*2), y+cy-ID_MOTHERSHIP_BALON_BLH);
	m_pBalonChat[8]->Render();
	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFMotherShipManager::SetWarPointMapInfo(SMSWARINFO_DISPLAY * pWarInfo)
/// \brief		모선전시 거점 정보 
/// \author		dgwoo
/// \date		2008-03-27 ~ 2008-03-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFMotherShipManager::SetWarPointMapInfo(SMSWARINFO_DISPLAY * pWarInfo)
{
 	if(IS_MAP_INFLUENCE_VCN(pWarInfo->MapInfluenceType))
	{// 바이제니유 맵.
		AdventMotherShipBCU(pWarInfo);
	}
	else if(IS_MAP_INFLUENCE_ANI(pWarInfo->MapInfluenceType))
	{// 알링턴 맵.
		AdventMotherShipANI(pWarInfo);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void SummonMotherShipInfo(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA* pMsg, MEX_MONSTER_INFO* pMonsterInfo)
/// \brief		모선 소환시 들어온다. 
/// \author		bhsohn
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::SummonMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, 
												 INT	 nContributionPoint, 
												 INT SummonMonsterUnitkind)
{
	// 현재 서버의 시간 
	ATUM_DATE_TIME curServerTime = GetServerDateTime();

	BOOL bAddPre = FALSE;
	if( (0 == curServerTime.Year)
		&&( 0 == curServerTime.Month)
		&&( 0 == curServerTime.Day)
		&&( 0 == curServerTime.Minute))
	{
		bAddPre = TRUE;
	}
	if(!bAddPre)
	{
		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
		UpdateMotherShipInfo(byBelligerence, pSummonMonsterTime, nContributionPoint, SummonMonsterUnitkind);		
	}
	else
	{
		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
		AddPreMotherShipInfo(byBelligerence, pSummonMonsterTime, nContributionPoint, SummonMonsterUnitkind);		
	}

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void AddPreMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime)
/// \brief		모선 소환 추가 못한 부분 업데이트
/// \author		bhsohn
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::AddPreMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, INT nContributionPoint, INT SummonMonsterUnitkind)
{
	m_nPreAddMothershipCnt++;
	if(m_nPreAddMothershipCnt > MOTHERSHIP_MAX_INFLUENCE)
	{
		// 추가 항목 에러
		m_nPreAddMothershipCnt = MOTHERSHIP_MAX_INFLUENCE;
		return;
	}
	int nIdx = m_nPreAddMothershipCnt -1;
	if(nIdx < 0)
	{
		return;
	}
	m_struPreAddMotherShip[nIdx].byBelligerence = byBelligerence;
	memcpy(&m_struPreAddMotherShip[nIdx].SummonMonsterTime, pSummonMonsterTime, sizeof(ATUM_DATE_TIME));	

	// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
	m_struPreAddMotherShip[nIdx].nContributionPoint = nContributionPoint;
	m_struPreAddMotherShip[nIdx].SummonMonsterUnitkind = SummonMonsterUnitkind;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void UpdateMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime)
/// \brief		모선 소환정보 업데이트
/// \author		bhsohn
/// \date		2007-02-06 ~ 2007-02-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::UpdateMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, 
												INT	 nContributionPoint, INT SummonMonsterUnitkind)
{	
	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	int nOldMotherCnt = m_nSummonMotherShipCnt; //과거에 모선

	char szInfluence[MAX_PATH], szReMainTime[MAX_PATH], szArea[MAX_PATH];	
	
	if(byBelligerence == BELL_INFLUENCE_VCN)
	{
		// 바이제니유
		if(m_bSummonMotherShipInfo[MOTHERSHIP_INFLUENCE_VCN])
		{
			return;
		}
		m_bSummonMotherShipInfo[MOTHERSHIP_INFLUENCE_VCN] = TRUE;
		
		// 2008-03-28 by dgwoo 바이제니유 모선이 추가되었을때 
		ResetAdventBCU();
		
		wsprintf(szInfluence, STRMSG_C_070206_0201, STRMSG_C_060210_0000);		
		wsprintf(szArea, STRMSG_C_070206_0204);
	}
	else
	{
		// 알링턴 
		if(m_bSummonMotherShipInfo[MOTHERSHIP_INFLUENCE_ANI])
		{
			return;
		}
		m_bSummonMotherShipInfo[MOTHERSHIP_INFLUENCE_ANI] = TRUE;

		// 2008-03-28 by dgwoo 알링턴 모선이 추가되었을때 
		ResetAdventANI();
		
		wsprintf(szInfluence, STRMSG_C_070206_0201, STRMSG_C_060210_0001);		
		wsprintf(szArea, STRMSG_C_070206_0205);
	}

	// 현재 서버의 시간 
	ATUM_DATE_TIME curServerTime = GetServerDateTime();
	// 모선전 종료 시간 계산 
	ATUM_DATE_TIME timeMotherShipEnd = (*pSummonMonsterTime);	
	
	// 모선전은 종료 2시간 경과
	timeMotherShipEnd.AddDateTime(0, 0, 0, MOTHERSHIP_LIVETIME_HOUR, 0, 0);	

	if(timeMotherShipEnd <curServerTime)
	{
		// 모선전 종료 시간 초과 
		return;
	}
	// 모선전 남은 시간 
	int nSecondRemainMotherShip = timeMotherShipEnd.GetTimeInSeconds() - curServerTime.GetTimeInSeconds();
	if(nSecondRemainMotherShip < 0)
	{
		return;
	}	
	// 2시간을 넘길수 없다. 
	if(nSecondRemainMotherShip > MOTHERSHIP_LIVETIME_SECOND)
	{
		nSecondRemainMotherShip  = MOTHERSHIP_LIVETIME_SECOND;
	}
	

	// 남은 시간 
	int nHour, nMinute;
	int nSecondRemainMotherShipTmp = nSecondRemainMotherShip;	
	nHour = (nSecondRemainMotherShipTmp/3600);
	nSecondRemainMotherShipTmp -= (nHour*3600);
	nMinute = (nSecondRemainMotherShipTmp/60);

	// 2007-07-23 by bhsohn 오퍼레이터 추가작업	
	g_pGameMain->ShowOpBossMonsterSummon(byBelligerence, nHour, nMinute);
	// end 2007-07-23 by bhsohn 오퍼레이터 추가작업
	
	// 2007-07-10 by bhsohn 모선전 5분미만시 표시 스트링 관련 버그 수정
//	if(nMinute >= 5)
//	{
//		wsprintf(szReMainTime, STRMSG_C_070206_0202, nHour, nMinute);	
//	}
//	else
//	{
//		wsprintf(szReMainTime, STRMSG_C_070206_0203);				
//	}
	if(0 == nHour && nMinute < 5)
	{
		wsprintf(szReMainTime, STRMSG_C_070206_0203);				
	}
	else
	{
		wsprintf(szReMainTime, STRMSG_C_070206_0202, nHour, nMinute);	
	}
	// end 2007-07-10 by bhsohn 모선전 5분미만시 표시 스트링 관련 버그 수정
	
	m_nSummonMotherShipCnt++;
	if(m_nSummonMotherShipCnt >MOTHERSHIP_MAX_INFLUENCE)
	{
		m_nSummonMotherShipCnt  = MOTHERSHIP_MAX_INFLUENCE;		
	}	

	// 현재 스티링의 위치를 판단한다.
	int nSummonIdx = m_nSummonMotherShipCnt-1;
	if(nSummonIdx>=0 && nSummonIdx < MOTHERSHIP_MAX_INFLUENCE)
	{
		m_struMotherShipInfo[nSummonIdx].fMotherShipRemainTime = nSecondRemainMotherShip ;
		// 현재 진형 인덱스 
		m_struMotherShipInfo[nSummonIdx].byBelligerence = byBelligerence;

		// 진형
		strncpy(m_struMotherShipInfo[nSummonIdx].szInfluence, szInfluence, MAX_PATH);

		// 남은시간 
		strncpy(m_struMotherShipInfo[nSummonIdx].szReMainTime, szReMainTime, MAX_PATH);		

		// 지역 정보
		strncpy(m_struMotherShipInfo[nSummonIdx].szArea, szArea, MAX_PATH);		

		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
		// 세력포인트 
		m_struMotherShipInfo[nSummonIdx].nContributionPoint = nContributionPoint;
		// 세력포인트 
		m_struMotherShipInfo[nSummonIdx].SummonMonsterTime = (*pSummonMonsterTime);	
		// 몬스터 종류
		m_struMotherShipInfo[nSummonIdx].SummonMonsterUnitkind = SummonMonsterUnitkind;		
	}			

	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	if(0 == nOldMotherCnt && GetSummonMotherShipCnt() == 1)
	{
		// 모선이 처음 떴으므로 마크 갱신		
		g_pStoreData->UpdateInfluenceMark(); // 본인 기체
		g_pStoreData->UpdateEnemyInfluenceMark(); // 적 기체

	}
	// end 2008-08-19 by bhsohn 세력 마크 시스템 추가
}

void CINFMotherShipManager::DestroyMotherShipInfo(BYTE byBelligerence)
{	
	int nInfluence = 0;
	if(byBelligerence == BELL_INFLUENCE_VCN)
	{
		nInfluence = MOTHERSHIP_INFLUENCE_VCN;
		DestroyMotherShipBCU();
	}
	else
	{
		nInfluence = MOTHERSHIP_INFLUENCE_ANI;
		DestroyMotherShipANI();
	}
	if(!m_bSummonMotherShipInfo[nInfluence])
	{
		// 모선이 떠있지 않는 모선이면 리턴한다. 
		return;
	}
	int nFindInfluence = -1;
	for(int nCnt = MOTHERSHIP_MAX_INFLUENCE-1;nCnt >= 0 ; nCnt--)
	{
		if(m_struMotherShipInfo[nCnt].byBelligerence == byBelligerence)
		{
			nFindInfluence = nCnt;
			break;
		}
	}
	if(nFindInfluence < 0)
	{
		return;
	}
	
	int nSummonIdx = m_nSummonMotherShipCnt -1;
	if(nSummonIdx < 0)
	{
		return;
	}
	int nSrcIdx, nDstIdx;
	nSrcIdx = nDstIdx =0;	
	
	if(0 == nFindInfluence )
	{
		if(1 == m_nSummonMotherShipCnt )
		{			
			nDstIdx =0;
			memset(&m_struMotherShipInfo[nDstIdx], 0x00, sizeof(MOTHERSHIP_INFO));
		}
		else if(2 == m_nSummonMotherShipCnt )
		{
			// 2줄로 있으면서 첫번째 줄 제거
			nSrcIdx = 1;
			nDstIdx = 0;
			memcpy(&m_struMotherShipInfo[nDstIdx], &m_struMotherShipInfo[nSrcIdx], sizeof(MOTHERSHIP_INFO));
			memset(&m_struMotherShipInfo[nSrcIdx], 0x00, sizeof(MOTHERSHIP_INFO));
		}
	}
	else if(1 == nFindInfluence )
	{
		nDstIdx =1;
		memset(&m_struMotherShipInfo[nDstIdx], 0x00, sizeof(MOTHERSHIP_INFO));
	}
	
	m_bSummonMotherShipInfo[nInfluence] = FALSE;

	m_nSummonMotherShipCnt--;
	if(m_nSummonMotherShipCnt < 0)
	{
		m_nSummonMotherShipCnt = 0;
	}	
	// 2008-08-19 by bhsohn 세력 마크 시스템 추가
	if(GetSummonMotherShipCnt() == 0)
	{
		// 모선이 없어졌으므로 갱신		
		g_pStoreData->RestoreInfluenceMark();
		g_pStoreData->UpdateEnemyInfluenceMark(); // 적기체
	}
	// end 2008-08-19 by bhsohn 세력 마크 시스템 추가
}
// end 2007-02-06 by bhsohn 모선전 진행 사항 체크
///////////////////////////////////////////////////////////////////////////////
/// \fn			int CINFMotherShipManager::GetSummonMotherShipCnt()
/// \brief		
/// \author		// 2007-07-04 by bhsohn 오퍼레이터 추가
/// \date		2007-07-04 ~ 2007-07-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFMotherShipManager::GetSummonMotherShipCnt()
{
	return m_nSummonMotherShipCnt;
}
BOOL CINFMotherShipManager::GetMotherShipInfl(UINT i_nMotherShipInf)
{
	return m_bSummonMotherShipInfo[i_nMotherShipInf];
}
void CINFMotherShipManager::SetMSWarInfoDisplayOption(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK* pMsg)
{
	m_fTimeOptionChange = MOTHERSHIP_OPTION_CHANGE_GAP;
	if(pMsg->MSWarOptionType != m_nOption &&
		m_bFirstOptionSetting)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080619_0100,COLOR_ITEM);
	}
	m_bFirstOptionSetting = TRUE;	

	SetMotherShipOption(pMsg->MSWarOptionType);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		모선전 관련 글씨 크기 계산
/// \author		bhsohn
/// \date		2007-02-15 ~ 2007-02-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SIZE CINFMotherShipManager::GetMotherShipInfoFontWidth()
{
	SIZE sizeMaxWidth;		

	sizeMaxWidth.cx = GAMEMAIN_MOTHERSHIP_INFLUENCE_WIDTH;
	sizeMaxWidth.cy = GAMEMAIN_MOTHERSHIP_INFLUENCE_CAP_HEIGHT;	
	
	int nMaxString=0;
	char chMaxTxt[MAX_PATH];
	char chTmp[MAX_PATH];
	
	memset(chMaxTxt, 0x00, MAX_PATH);
	memset(chTmp, 0x00, MAX_PATH);

	// 바이제니유
	wsprintf(chTmp, STRMSG_C_070206_0201, STRMSG_C_060210_0000);
	strncpy(chMaxTxt, chTmp, MAX_PATH);
	nMaxString = strlen(chTmp);

	// 알링턴 
	wsprintf(chTmp, STRMSG_C_070206_0201, STRMSG_C_060210_0001);
	if(nMaxString < strlen(chTmp))		
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);		
	}

	// 남은 시간
	wsprintf(chTmp, STRMSG_C_070206_0202, 2, 0);
	if(nMaxString < strlen(chTmp))		
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);		
	}
	
	// 남은 시간 5분 미만
	wsprintf(chTmp, STRMSG_C_070206_0203);
	if(nMaxString < strlen(chTmp))		
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);		
	}

	// "진행 지역   타일런트 정글"
	wsprintf(chTmp, STRMSG_C_070206_0204);
	if(nMaxString < strlen(chTmp))		
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);		
	}

	// "진행 지역   레이나드 해변"
	wsprintf(chTmp, STRMSG_C_070206_0205);
	if(nMaxString < strlen(chTmp))		
	{
		strncpy(chMaxTxt, chTmp, MAX_PATH);
		nMaxString = strlen(chTmp);		
	}

	SIZE szTmp;
	szTmp = GetStringSize(chMaxTxt); 
	if(szTmp.cx > sizeMaxWidth.cx)
	{
		sizeMaxWidth.cx = szTmp.cx;
	}

	if(szTmp.cy > sizeMaxWidth.cy)
	{
		sizeMaxWidth.cy = szTmp.cy;
	}	
	
	return sizeMaxWidth;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		글씨크기의 폰트를 구한다.
/// \author		bhsohn
/// \date		2007-02-15 ~ 2007-02-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SIZE CINFMotherShipManager::GetStringSize(char* pTxt)
{
	SIZE sizeTxt;
	memset(&sizeTxt, 0x00, sizeof(SIZE));
	int nSize, nWidth, nHeight;
	DWORD dwFontFlags;

	nSize = 9;
	nWidth = 256;
	nHeight = 32;
	dwFontFlags = D3DFONT_ZENABLE;

	// 글자를 컬링 할것인가D3DFONT_BOLD
	CD3DHanFont* pFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),nSize, dwFontFlags, TRUE, nWidth, nHeight);
	pFont->InitDeviceObjects(g_pD3dDev);
	pFont->RestoreDeviceObjects();
	
	sizeTxt = pFont->GetStringSize(pTxt);		

	pFont->InvalidateDeviceObjects();
	pFont->DeleteDeviceObjects();
	util::del(pFont);	
	

	return sizeTxt;

}
void CINFMotherShipManager::SendMotherShipOption()
{
	// 2008-06-19 by dgwoo 옵션이 변경된것이 없으면 처리 하지 않음.
	if(m_nTempOption == m_nOption)
	{
		return;
	}
	if(m_fTimeOptionChange > 0)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080619_0101,COLOR_ITEM);
		return;
	}

	MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION sMsg;
	sMsg.CharacterUID = g_pShuttleChild->GetMyShuttleInfo().CharacterUniqueNumber;
	sMsg.ClientIndex = g_pShuttleChild->GetMyShuttleInfo().ClientIndex;
	sMsg.MSWarOptionType = m_nTempOption;

	g_pD3dApp->m_pFieldWinSocket->SendMsg(T_FC_INFO_MSWARINFO_DISPLAY_OPTION, (char*)&sMsg, sizeof(sMsg));
} 

///////////////////////////////////////////////////////////////////////////////
/// \fn			MOTHERSHIP_INFO* CINFMotherShipManager::GetMotherShipInfo(int nIdx)
/// \brief		
/// \author		// 2008-04-22 by bhsohn 모선/거점전 진행중인 정보도 표시 하게 변경
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFMotherShipManager::GetMotherShipInfo(int nIdx, 
														BYTE *o_byBelligerence, 
														ATUM_DATE_TIME  *o_pSummonMonsterTime, 
														int *o_nContributionPoint,
														int *o_SummonMonsterUnitkind)
{
	if(nIdx <0 || nIdx>= MOTHERSHIP_MAX_INFLUENCE)
	{
		return ;
	}
	(*o_byBelligerence) = m_struMotherShipInfo[nIdx].byBelligerence;
	(*o_pSummonMonsterTime) = m_struMotherShipInfo[nIdx].SummonMonsterTime;
	(*o_nContributionPoint) = m_struMotherShipInfo[nIdx].nContributionPoint;
	(*o_SummonMonsterUnitkind) = m_struMotherShipInfo[nIdx].SummonMonsterUnitkind;
	
} 
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFMotherShipManager::SetMotherShipSearchTeleportTime(MSG_FC_EVENT_CLICK_TELEPORT_OK *pMsg)
/// \brief		맵인덱스를 검색해 시간 값을 셋팅해준다.
/// \author		dgwoo
/// \date		2008-04-22 ~ 2008-04-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFMotherShipManager::SetMotherShipSearchTeleportTime(MSG_FC_EVENT_CLICK_TELEPORT_OK *pMsg)
{
	vecINFImportantInfoIter it = m_vecImportANI.begin();
	while(it != m_vecImportANI.end())
	{
		if((*it)->m_nMapNum == pMsg->MapIndex)
		{
			(*it)->SetActiveTime(pMsg->StartTime,pMsg->EndTime);
			return TRUE;
		}
		it++;
	}
	it = m_vecImportBCU.begin();
	while(it != m_vecImportBCU.end())
	{
		if((*it)->m_nMapNum == pMsg->MapIndex)
		{
			(*it)->SetActiveTime(pMsg->StartTime,pMsg->EndTime);
			return TRUE;
		}
		it++;
	}
	return FALSE;
}
