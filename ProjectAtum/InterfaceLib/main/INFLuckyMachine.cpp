// INFLuckyMachine.cpp: implementation of the CINFLuckyMachine class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "INFLuckyMachine.h"
#include "AtumDatabase.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "INFIcon.h"
#include "ItemInfo.h"
#include "FieldWinSocket.h"
#include "INFCityBase.h"
#include "StoreData.h"
#include "Chat.h"
#include "INFInvenExtend.h"
#include "ShuttleChild.h"
#include "AtumSound.h"
#include "INFImageEx.h"										   // 2011. 10. 10 by jskim UI시스템 변경
	
#define LUCKYMACHINE_BLANK_ICON_NUMBER			7020630

#define LUCKYMACHINE_EFFECT_FRAME_CHANGE_TIME				0.5f

// 2010. 06. 03 by jskim 웨폰머신, 해피머신 수정
#define LUCKYMACHINE_MAIN_BG_X					(CITY_BASE_NPC_BOX_START_X + 420)
#define LUCKYMACHINE_MAIN_BG_Y					(CITY_BASE_NPC_BOX_START_Y - 440)

#define	LUCKYMACHINE_EFFECT_CH_BG_X				(CITY_BASE_NPC_BOX_START_X)
#define LUCKYMACHINE_EFFECT_CH_BG_Y				(LUCKYMACHINE_MAIN_BG_Y - 192)

#define	LUCKYMACHINE_EFFECT_BG_X				(CITY_BASE_NPC_BOX_START_X -7)
#define LUCKYMACHINE_EFFECT_BG_Y				(LUCKYMACHINE_MAIN_BG_Y - 148)

#define LUCKYMACHINE_SLOT_X						(LUCKYMACHINE_MAIN_BG_X + 43 + 30)
#define LUCKYMACHINE_SLOT_Y						(LUCKYMACHINE_MAIN_BG_Y + 34 + 120)

#define LUCKYMACHINE_SLOT_GAP_X					32
#define LUCKYMACHINE_SLOT_GAP_Y					52

#define	LUCKYMACHINE_COIN_X						(LUCKYMACHINE_MAIN_BG_X + 140 + 30)
#define	LUCKYMACHINE_COIN_Y						(LUCKYMACHINE_MAIN_BG_Y + 191 + 120)

#define ICON_SIZE_W								30
#define ICON_SIZE_H								30

#define LUCKYMACHINE_GAMBLE_BUTTON_X			(LUCKYMACHINE_MAIN_BG_X + 141)
#define LUCKYMACHINE_GAMBLE_BUTTON_Y			(LUCKYMACHINE_MAIN_BG_Y + 240 + 126)

#define LUCKYMACHINE_TAKE_BUTTON_X				(LUCKYMACHINE_MAIN_BG_X + 198)
#define LUCKYMACHINE_TAKE_BUTTON_Y				(LUCKYMACHINE_MAIN_BG_Y + 240 + 126)

// 2009-03-04 by bhsohn 럭키 머신 수정안
// 뒤로 가기
#define LUCKYMACHINE_PREV_X			(CITY_BASE_NPC_BOX_START_X+613)
#define LUCKYMACHINE_PREV_Y			(CITY_BASE_NPC_BOX_START_Y+79)

///////////// 럭키 머신 선택
#define LUCKYMACHINE_SELECT_TYPE_BG_X					(CITY_BASE_NPC_BOX_START_X)
#define LUCKYMACHINE_SELECT_TYPE_BG_Y					(CITY_BASE_NPC_BOX_START_Y - 451)

#define LUCKYMACHINE_SELECT_TYPE_NORMAL_X			(LUCKYMACHINE_SELECT_TYPE_BG_X + 93)
#define LUCKYMACHINE_SELECT_TYPE_NORMAL_Y			(LUCKYMACHINE_SELECT_TYPE_BG_Y + 155)

#define LUCKYMACHINE_SELECT_TYPE_MINI_X				(LUCKYMACHINE_SELECT_TYPE_BG_X + 349)
#define LUCKYMACHINE_SELECT_TYPE_MINI_Y				(LUCKYMACHINE_SELECT_TYPE_BG_Y + 155)

///////////// 럭키 머신 상점 선택

#define		MAX_ONEPAGE_NPCLIST_COUNT							5		// 한페이지 최대 NPC수

#define LUCKYMACHINE_SELECT_SHOP_BG_X					(CITY_BASE_NPC_BOX_START_X)
#define LUCKYMACHINE_SELECT_SHOP_BG_Y					(CITY_BASE_NPC_BOX_START_Y - 276)

#define LUCKYMACHINE_SELECT_SHOP_PREV_X					(LUCKYMACHINE_SELECT_SHOP_BG_X + 44)
#define LUCKYMACHINE_SELECT_SHOP_PREV_Y					(LUCKYMACHINE_SELECT_SHOP_BG_Y + 120)

#define LUCKYMACHINE_SELECT_SHOP_NEXT_X					(LUCKYMACHINE_SELECT_SHOP_BG_X + 768)
#define LUCKYMACHINE_SELECT_SHOP_NEXT_Y					(LUCKYMACHINE_SELECT_SHOP_BG_Y + 120)

#define LUCKYMACHINE_SELECT_SHOP_NPC_X					(LUCKYMACHINE_SELECT_SHOP_BG_X + 148)
#define LUCKYMACHINE_SELECT_SHOP_NPC_Y					(LUCKYMACHINE_SELECT_SHOP_BG_Y + 87)
#define LUCKYMACHINE_SELECT_SHOP_NPC_WIDTH				(117)


// 상점 정보 없는 아이콘 인텍스
#define NOT_OPEN_LUCKY_MACHINE						(999)

// 상점 Index
// #define LUCKY_MACHINE_BUILD_INDEX_BCU							(9348)		// 바이제니유 럭키(2001)
// #define LUCKY_MACHINE_BUILD_INDEX_ANI							(9349)		// 알링턴 럭키(2002)
// #define LUCKY_MACHINE_BUILD_INDEX_NOR							(9350)		// 일반 세력 럭키(2003)


#define MAX_LUCKY_MACHINE_TIME							(9.0f)		// 럭키머신 최대 시간
#define OVERTIME_LUCKY_MACHINE_STEP						(LUCKY_ITEM_LIST_NUM+LUCKY_ITEM_POS+1)		// 럭키머신 시간 초과시 간경

// 성공 스텝들
#define FIRST_SLOT_MACHINE_SUCCESS_STEP					(22)		// 첫번째 슬롯
#define SECOND_SLOT_MACHINE_SUCCESS_STEP				(22)		
#define THIRD_SLOT_MACHINE_SUCCESS_STEP					(22)

#define		MAX_YOU_LUCKY_TIME							5.0f
#define		MAX_YOU_DELAY_LUCKY_TIME					4.0f

#define	PRIZE_LUCKY_ITEM_X								200
#define	PRIZE_LUCKY_ITEM_Y								200


// end 2009-03-04 by bhsohn 럭키 머신 수정안

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFLuckyMachine::CINFLuckyMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent = pParent;	// CGameMain*
	m_pGameData = NULL;
	//m_pBuildingInfo = pBuilding;

	m_vecTakeItem.clear();
	m_pBtnGamble	= NULL;
	m_pBtnTake		= NULL;

	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	m_pBtnGamble1	= NULL;
	m_pBtnTake1		= NULL;
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경

//	m_pImgArrow		= NULL;

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		m_pImgEffectBG[nCnt]	= NULL;
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{
		m_pImgYouLuckyBG[nCnt]	= NULL;
	}
	

	
	m_fTimeEffectBG		= 0.0f;
	m_nEffectBG			= 0;

	m_fLuckyTimeEffectBG = 0.0f;
	m_nLuckyEffectBG	= 0;

	for(nCnt = 0; nCnt < 3; nCnt++)
	{
		m_pImgMainBG[nCnt]	= NULL;
	}
	
	m_pImgQuestionMark	= NULL;

	m_nState			= LUCKY_STATE_READY;	
	///////////////////////////////////////
	InitRulType();	// 룰럿 돌아 가는 규칙을 정한다.	
	///////////////////////////////////////

	// 2009-03-04 by bhsohn 럭키 머신 수정안
	m_nLuckyMechineStep = LUCKY_MAINSTEP_SELECT_SHOP;	
	m_nLuckySelectStartIdx = 0;	

	m_pImgSelctShopBK = NULL;
	m_pSelectShopPrevBtn = NULL;
	m_pSelectShopNextBtn = NULL;

	m_pPrevStepBtn = NULL;
	
	m_vecstruShopList.clear();				// NPC리스트	
	memset(&m_struSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));			// 선택한 럭키 상점

	// 2009-05-13 by bhsohn 럭키 상점 버그 수정
	m_uLoadBuildingIndex=0;
	// end 2009-05-13 by bhsohn 럭키 상점 버그 수정

	m_nLoadNpcListInfluenceType = -1;
	int i =0;
	for(i = 0; i < LUCKY_ITEM_LIST_NUM_LINE; i++)
	{		
		m_fTimeHold[i]		= 0.0f;
		m_fTimeOneStepSlot[i]		= 0.0f;
		m_fTimeAllTimeStepSlot[i]		= 0.0f;
		m_nSuccessSlot[i]				= 0 ;		
		m_nSlotStartPosIdx[i]			= 0;
		m_nSlotType[i]			= 0;
		m_bSlotDone[i]		= FALSE;
		m_nSlotStep[i]		= 0;
		m_bCompleteStep[i]	=FALSE;
	}
	m_pCoinItemInfo = NULL;
	m_bInfluenceOneShopMode = FALSE;
	m_uBuildingIndex = 0;
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	m_bYouLucky = FALSE;	
	m_fYouLuckyViewTime= 0.0f;
	m_dwYouLuckyAlpha= 0;
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안


	// 2009. 05. 07 by ckPark 럭키머신 버튼 수정
	m_dwLastFrameTime	= 0;
	m_nCurFrame			= 0;
	// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정

	// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	m_pItemCoinItemTmp = NULL;	
	// end 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리

	// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	m_pImgDisableItem = NULL;
	//end 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
}

CINFLuckyMachine::~CINFLuckyMachine()
{	
	ClearSearchItem();	
	util::del(m_pCoinItemInfo);
	// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	util::del(m_pItemCoinItemTmp);	
	// end 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
}

void CINFLuckyMachine::InitRulType()
{
	int nRulType = 0;
	int nSlot = 0;
	int nSlotStep = 0;

	for(nSlot = 0;nSlot < LUCKY_ITEM_LIST_NUM_LINE;nSlot++)
	{
		for(nRulType = 0;nRulType < MAX_SLOT_RULLET_TYPE;nRulType++)
		{
			for(nSlotStep = 0;nSlotStep < MAX_SLOT_RULLET_STEP;nSlotStep++)
			{			
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= -1.0f;				
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
				m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= FALSE;
			}
		}
	}

	
	for(nSlot = 0;nSlot < LUCKY_ITEM_LIST_NUM_LINE;nSlot++)
	{
		nRulType = 0;				
		
		{			
			// 기본 타입(1)
			nSlotStep = 0;

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 1.3f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.2f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.02f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;			

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.5f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.05f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= TRUE;
			nSlotStep++;						

			nRulType++;
		}		

		{
			// 기본 타입(2)
			nSlotStep = 0;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 1.7f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.3f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.03f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			

			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.6f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.06f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= TRUE;
			nSlotStep++;
			
			nRulType++;
		}
		
		{
			// 기본 타입(3)
			nSlotStep = 0;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 2.0f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.0f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.4f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.04f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			nSlotStep++;
			
			
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fSlotTime		= 0.7f;				
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].fHoldTime		= 0.07f;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].nSlotStep		= 1;
			m_stRulTime[nSlot][nRulType].stRulTime[nSlotStep].bCompleteStep	= TRUE;
			
			nRulType++;
		}		
	}
}
void CINFLuckyMachine::ResetData()
{
	int i;
	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
// 	if(m_pCoinItemInfo)
// 	{
// 		//m_pCoinItemInfo->CurrentCount = 1;				// 주화 아이템은 한개만 올릴수있다.
// 		//g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), FALSE);
// 		g_pStoreData->UpdateItemCount(m_pCoinItemInfo->UniqueNumber, m_pCoinItemInfo->CurrentCount + 1);		
// 		util::del(m_pCoinItemInfo);
// 	}
	ClearConinItemInfo(); // 코인정보 초기화
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	for(i = 0; i < LUCKY_ITEM_LIST_NUM_LINE; i++)
	{		
		m_fTimeHold[i]		= 0.0f;
		m_fTimeOneStepSlot[i]		= 0.0f;
		m_fTimeAllTimeStepSlot[i]		= 0.0f;
		m_nSuccessSlot[i]				= 0;		
		m_nSlotStartPosIdx[i]			= 0;		
		m_bSlotDone[i]		= FALSE;
		m_nSlotStep[i]		= 0;
		m_bCompleteStep[i]	=FALSE;
	}	
	ClearSearchItem();
	m_nState = LUCKY_STATE_READY;
	Init();
	
}
HRESULT CINFLuckyMachine::InitDeviceObjects()
{
	DataHeader	* pDataHeader;

	m_pImgMainBG[2] = new CINFImageEx;
	//pDataHeader = FindResource("AM_BG");
	pDataHeader = FindResource("AME_BG_0");
	m_pImgMainBG[2]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[2]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[2]->SetLoop(TRUE);

	m_pImgMainBG[1] = new CINFImageEx;
	//pDataHeader = FindResource("WM_BG");
	pDataHeader = FindResource("WME_BG_0");
	m_pImgMainBG[1]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[1]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[1]->SetLoop(TRUE);


	m_pImgMainBG[0] = new CINFImageEx;
	//pDataHeader = FindResource("HM_BG");
	pDataHeader = FindResource("HME_BG_0");
	m_pImgMainBG[0]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[0]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[0]->SetLoop(TRUE);

	m_pImgMainBG[3] = new CINFImageEx;
	//pDataHeader = FindResource("HM_BG");
	pDataHeader = FindResource("MME_BG_0");
	m_pImgMainBG[3]->InitDeviceObjects(pDataHeader);
	m_pImgMainBG[3]->SetUVAnimation(4,2,0.07f);
	m_pImgMainBG[3]->SetLoop(TRUE);




	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{	
		char chTmp[32];
		wsprintf(chTmp, "LMEBG%d", nCnt);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			m_pImgEffectBG[nCnt] = new CINFImageEx;		
			m_pImgEffectBG[nCnt]->InitDeviceObjects(pDataHeader);
		}	
	}	

	int i;
	for(i=0; i< 4; i++)
	{
		char temp[256];
		m_pImgEffBG[2][i] = new CINFImageEx; 
		wsprintf(temp,"AME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[2][i]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[1][i] = new CINFImageEx; 
		wsprintf(temp,"WME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[1][i]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[0][i] = new CINFImageEx; 
		wsprintf(temp,"HME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[0][i]->InitDeviceObjects(pDataHeader);
	}
	
	for(i=0; i< 2; i++)
	{
		char temp[256];
		m_pImgEffBG[2][i+4] = new CINFImageEx; 
		wsprintf(temp,"AME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[2][i+4]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[1][i+4] = new CINFImageEx; 
		wsprintf(temp,"WME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[1][i+4]->InitDeviceObjects(pDataHeader);

		m_pImgEffBG[0][i+4] = new CINFImageEx; 
		wsprintf(temp,"HME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgEffBG[0][i+4]->InitDeviceObjects(pDataHeader);
	}
	
	char temp1[256];
	m_pImgEffBG[2][6] = new CINFImageEx; 
	wsprintf(temp1,"AME_BG_%d",5);
	pDataHeader = FindResource(temp1);
	m_pImgEffBG[2][6]->InitDeviceObjects(pDataHeader);

	m_pImgEffBG[1][6] = new CINFImageEx; 
	wsprintf(temp1,"WME_BG_%d",5);
	pDataHeader = FindResource(temp1);
	m_pImgEffBG[1][6]->InitDeviceObjects(pDataHeader);

	m_pImgEffBG[0][6] = new CINFImageEx; 
	wsprintf(temp1,"HME_BG_%d",5);
	pDataHeader = FindResource(temp1);
	m_pImgEffBG[0][6]->InitDeviceObjects(pDataHeader);

	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{
		char chTmp[32];
		wsprintf(chTmp, "LMLBG%d", nCnt);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			m_pImgYouLuckyBG[nCnt] = new CINFImageEx;		
			m_pImgYouLuckyBG[nCnt]->InitDeviceObjects(pDataHeader);
		}		
	}
	

//	m_pImgArrow = new CINFImage;
//	pDataHeader = FindResource("arrow");
//	m_pImgArrow->InitDeviceObjects(pDataHeader->m_pData,pDataHeader->m_DataSize);
	
	m_pImgQuestionMark = new CINFImageEx;
	pDataHeader = FindResource("question");
	m_pImgQuestionMark->InitDeviceObjects(pDataHeader);
	

	if(m_pBtnGamble == NULL)
	{
		m_pBtnGamble = new CINFImageBtn;
	}
	//m_pBtnGamble->InitDeviceObjects("AM_Start");
	m_pBtnGamble->InitDeviceObjects("GameStart");
	
	if(m_pBtnTake == NULL)
	{
		m_pBtnTake = new CINFImageBtn;
	}
	//m_pBtnTake->InitDeviceObjects("AM_Prize");
	m_pBtnTake->InitDeviceObjects("PrizeItem");
	
	// 2009-03-04 by bhsohn 럭키 머신 수정안	
	
	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	if(m_pBtnGamble1 == NULL)
	{
		m_pBtnGamble1 = new CINFImageBtn;
	}
	m_pBtnGamble1->InitDeviceObjects("GameStart");
	if(m_pBtnTake1 == NULL)
	{
		m_pBtnTake1 = new CINFImageBtn;
	}
	m_pBtnTake1->InitDeviceObjects("PrizeItem");
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	// 럭키머신 캐릭터
	{
		if(NULL == m_pImgSelctShopBK)
		{
			m_pImgSelctShopBK = new CINFImageEx;
			pDataHeader = FindResource("lbk1");
			m_pImgSelctShopBK->InitDeviceObjects(pDataHeader);
		}
		if(NULL == m_pSelectShopPrevBtn)
		{
			m_pSelectShopPrevBtn = new CINFImageBtn;			
			m_pSelectShopPrevBtn->InitDeviceObjects("lspre");
		}
		if(NULL == m_pSelectShopNextBtn)
		{
			m_pSelectShopNextBtn = new CINFImageBtn;			
			m_pSelectShopNextBtn->InitDeviceObjects("lsnex");
		}		 
	}
	if(NULL == m_pPrevStepBtn)
	{
		m_pPrevStepBtn = new CINFImageBtn;			
		m_pPrevStepBtn->InitDeviceObjects("lpre");

	}
	InitDeviceObjectsNPCListBtn();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안
	// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	if(NULL == m_pImgDisableItem)
	{
		m_pImgDisableItem = new CINFImageEx;
		pDataHeader = FindResource("LM_inven");
		m_pImgDisableItem->InitDeviceObjects(pDataHeader);
	}
	//end 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	if(NULL == m_pAME_slot[0])
	{
		m_pAME_slot[0] = new CINFImageEx;
		pDataHeader = FindResource("AME_slot0");
		m_pAME_slot[0]->InitDeviceObjects(pDataHeader );
	}
	if(NULL == m_pAME_slot[1])
	{
		m_pAME_slot[1] = new CINFImageEx;
		pDataHeader = FindResource("AME_slot1");
		m_pAME_slot[1]->InitDeviceObjects(pDataHeader);
	}

	for(nCnt=0; nCnt < 3; nCnt++)
	{
		m_nAME_EffectNum[nCnt] = NULL;
	}	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	return S_OK;
}
HRESULT CINFLuckyMachine::RestoreDeviceObjects()
{
//	m_pImgArrow->RestoreDeviceObjects();
	for(int i=0; i< 7; i++)
	{
		for(int j=0; j < 3; j++)
		{
			m_pImgEffBG[j][i]->RestoreDeviceObjects();
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		if(m_pImgEffectBG[nCnt])
		{
			m_pImgEffectBG[nCnt]->RestoreDeviceObjects();		
		}
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{	
		if(m_pImgYouLuckyBG[nCnt])
		{			
			m_pImgYouLuckyBG[nCnt]->RestoreDeviceObjects();
		}		
	}
	for(nCnt = 0;nCnt < LUCKY_SELECT_MAX;nCnt++)
	{	
		m_pImgMainBG[nCnt]->RestoreDeviceObjects();
	}
	
	m_pImgQuestionMark->RestoreDeviceObjects();

	m_pBtnGamble->RestoreDeviceObjects();
	m_pBtnTake->RestoreDeviceObjects();
	m_pBtnGamble->SetBtnPosition(LUCKYMACHINE_GAMBLE_BUTTON_X,LUCKYMACHINE_GAMBLE_BUTTON_Y);
	m_pBtnTake->SetBtnPosition(LUCKYMACHINE_TAKE_BUTTON_X,LUCKYMACHINE_TAKE_BUTTON_Y);

	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	m_pBtnGamble1->RestoreDeviceObjects();
	m_pBtnTake1->RestoreDeviceObjects();
	m_pBtnGamble1->SetBtnPosition(LUCKYMACHINE_GAMBLE_BUTTON_X,LUCKYMACHINE_GAMBLE_BUTTON_Y);
	m_pBtnTake1->SetBtnPosition(LUCKYMACHINE_TAKE_BUTTON_X,LUCKYMACHINE_TAKE_BUTTON_Y);
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경

	// 2009-03-04 by bhsohn 럭키 머신 수정안	
	{
		m_pImgSelctShopBK->RestoreDeviceObjects();
		m_pSelectShopPrevBtn->RestoreDeviceObjects();		
		m_pSelectShopPrevBtn->SetBtnPosition(LUCKYMACHINE_SELECT_SHOP_PREV_X, LUCKYMACHINE_SELECT_SHOP_PREV_Y);
		
		m_pSelectShopNextBtn->RestoreDeviceObjects();		
		m_pSelectShopNextBtn->SetBtnPosition(LUCKYMACHINE_SELECT_SHOP_NEXT_X,LUCKYMACHINE_SELECT_SHOP_NEXT_Y);
		
	}
	{
		m_pPrevStepBtn->RestoreDeviceObjects();
		m_pPrevStepBtn->SetBtnPosition(LUCKYMACHINE_PREV_X, LUCKYMACHINE_PREV_Y);
	}
	RestoreDeviceObjectsNPCListBtn();


	RefreshNPCListBtnStat();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안
	// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	m_pImgDisableItem->RestoreDeviceObjects();
	//end 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기

	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pAME_slot[0]->RestoreDeviceObjects();
	m_pAME_slot[1]->RestoreDeviceObjects();
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	return S_OK;
}

HRESULT CINFLuckyMachine::DeleteDeviceObjects()
{
//	m_pImgArrow->DeleteDeviceObjects();

	for(int i=0; i< 7; i++)
	{
		for(int j=0; j < 3; j++)
		{
			m_pImgEffBG[j][i]->DeleteDeviceObjects();
			util::del(m_pImgEffBG[j][i]);
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		if(m_pImgEffectBG[nCnt])
		{			
			m_pImgEffectBG[nCnt]->DeleteDeviceObjects();
			util::del(m_pImgEffectBG[nCnt]);			
		}
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{	
		if(m_pImgYouLuckyBG[nCnt])
		{			
			m_pImgYouLuckyBG[nCnt]->DeleteDeviceObjects();
			util::del(m_pImgYouLuckyBG[nCnt]);			
		}		
	}
	
	for(nCnt = 0;nCnt < LUCKY_SELECT_MAX;nCnt++)
	{
		m_pImgMainBG[nCnt]->DeleteDeviceObjects();
		util::del(m_pImgMainBG[nCnt]);
	}	

	m_pImgQuestionMark->DeleteDeviceObjects();
	util::del(m_pImgQuestionMark);

	m_pBtnGamble->DeleteDeviceObjects();
	util::del(m_pBtnGamble);

	m_pBtnTake->DeleteDeviceObjects();
	util::del(m_pBtnTake);

	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	m_pBtnGamble1->DeleteDeviceObjects();
	util::del(m_pBtnGamble1);
	
	m_pBtnTake1->DeleteDeviceObjects();
	util::del(m_pBtnTake1);
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	// 2009-03-04 by bhsohn 럭키 머신 수정안	
	{
		m_pImgSelctShopBK->DeleteDeviceObjects();
		util::del(m_pImgSelctShopBK);

		m_pSelectShopPrevBtn->DeleteDeviceObjects();		
		util::del(m_pSelectShopPrevBtn);

		m_pSelectShopNextBtn->DeleteDeviceObjects();		
		util::del(m_pSelectShopNextBtn);	
	}

	{
		m_pPrevStepBtn->DeleteDeviceObjects();
		util::del(m_pPrevStepBtn);
	}
	DeleteDeviceObjectsNPCListBtn();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	m_pImgDisableItem->DeleteDeviceObjects();
	util::del(m_pImgDisableItem);
	//end 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기


	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pAME_slot[0]->DeleteDeviceObjects();
	util::del(m_pAME_slot[0]);
	m_pAME_slot[1]->DeleteDeviceObjects();
	util::del(m_pAME_slot[1]);
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조

	return S_OK;
}

HRESULT CINFLuckyMachine::InvalidateDeviceObjects()
{
//	m_pImgArrow->InvalidateDeviceObjects();

	for(int i=0; i< 7; i++)
	{
		for(int j=0; j < 3; j++)
		{
			m_pImgEffBG[j][i]->InvalidateDeviceObjects();
		}
	}

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LUCKY_TITLE_IMAGE;nCnt++)
	{
		if(m_pImgEffectBG[nCnt])
		{			
			m_pImgEffectBG[nCnt]->InvalidateDeviceObjects();
		}
	}
	for(nCnt = 0;nCnt < MAX_LUCKY_YOULUCKY;nCnt++)
	{	
		if(m_pImgYouLuckyBG[nCnt])
		{			
			m_pImgYouLuckyBG[nCnt]->InvalidateDeviceObjects();			
		}		
	}

	for(nCnt = 0;nCnt < LUCKY_SELECT_MAX;nCnt++)
	{	
		m_pImgMainBG[nCnt]->InvalidateDeviceObjects();
	}
	m_pImgQuestionMark->InvalidateDeviceObjects();

	m_pBtnGamble->InvalidateDeviceObjects();
	m_pBtnTake->InvalidateDeviceObjects();

	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	m_pBtnGamble1->InvalidateDeviceObjects();
	m_pBtnTake1->InvalidateDeviceObjects();
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	
	// 2009-03-04 by bhsohn 럭키 머신 수정안	
	{
		m_pImgSelctShopBK->InvalidateDeviceObjects();			

		m_pSelectShopPrevBtn->InvalidateDeviceObjects();		
		m_pSelectShopNextBtn->InvalidateDeviceObjects();
	}
	{
		m_pPrevStepBtn->InvalidateDeviceObjects();
	}
	InvalidateDeviceObjectsNPCListBtn();
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
	m_pImgDisableItem->InvalidateDeviceObjects();
	//end 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기

	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pAME_slot[0]->DeleteDeviceObjects();
	m_pAME_slot[1]->DeleteDeviceObjects();
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	return S_OK;
}

void CINFLuckyMachine::Render()
{
	switch(m_nLuckyMechineStep)
	{ 	
	case LUCKY_MAINSTEP_SELECT_SHOP:		// 럭키 머신 상점 선택
		{
			RenderLuckyStepStepSelectShop();
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:		// 럭키 머신 게임중
		{
			RenderLuckyStepStepSelectGaming();
		}
		break;
	}
	
	// 뒤로 가기 
	m_pPrevStepBtn->Render();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderLuckyStepStepSelectShop()
{
	m_pImgSelctShopBK->Move(LUCKYMACHINE_SELECT_SHOP_BG_X, LUCKYMACHINE_SELECT_SHOP_BG_Y);
	m_pImgSelctShopBK->Render();

	// 럭키 머신 타입 노말
	m_pSelectShopPrevBtn->Render();		
	
	// 럭키 머신 타입 미니
	m_pSelectShopNextBtn->Render();			

	// NPC버튼	
	{
		vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
		while(itNpcBtn != m_vecstruShopList.end())
		{
			structNPCList* pstruNPCList = (*itNpcBtn);
			
			if(pstruNPCList->pInfImageBtn)
			{
				pstruNPCList->pInfImageBtn->Render();
			}		
			itNpcBtn++;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderLuckyStepStepSelectGaming()
{	
	// 배경 이미지 
	int nImageIdx=0, nImageType=0;
	nImageType = m_struSelLuckyMachine.MachineKind - 1;
		nImageIdx = m_nEffectBG;
//	m_pImgEffBG[nImageType][nImageIdx]->Move(LUCKYMACHINE_EFFECT_BG_X,LUCKYMACHINE_EFFECT_BG_Y);
//	m_pImgEffBG[nImageType][nImageIdx]->Render();

	m_pImgMainBG[nImageType]->Move(LUCKYMACHINE_MAIN_BG_X,LUCKYMACHINE_MAIN_BG_Y);
	m_pImgMainBG[nImageType]->Render();

	int i = 0,j = 0;
	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	switch(m_nState)
	{
	case LUCKY_STATE_READY:
		{
			// 2009-03-04 by bhsohn 럭키 머신 수정안
			for(j = 0 ; j < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) ; j++)
			{
				int nSlotPosIdx = j;
				if(LUCKY_SELECT_TYPE_MINI == CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind))//m_struSelLuckyMachine.MachineKind)
				{
					nSlotPosIdx = LUCKY_SELECT_TYPE_MINI;
				}
				for(i = 0 ; i < LUCKY_ITEM_LIST_NUM ; i++)
				{
					m_pImgQuestionMark->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*i),
						LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx));
					m_pImgQuestionMark->Render();
				}
			}
		}
		break;
	case LUCKY_STATE_PRIZE:
	case LUCKY_STATE_RULLET:
		{					
			int nSlotNum=0;
			for(nSlotNum=0;nSlotNum < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind); nSlotNum++)
			{
				RenderLuckyMachineSlot(nSlotNum);
			}
		}
		break;
	}

	// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
// 	m_pBtnGamble->Render();
// 	m_pBtnTake->Render();
	if(m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
	{
		m_pBtnGamble1->Render();
		m_pBtnTake1->Render();
	}
	else
	{
		m_pBtnGamble->Render();
		m_pBtnTake->Render();
	}
	//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
	
	if(m_pCoinItemInfo != NULL && 
		m_pCoinItemInfo->ItemInfo->SourceIndex != NULL)
	{// 주화.
		CItemInfo* pItem = m_pCoinItemInfo;
		char buf[20];
		wsprintf(buf, "%08d",pItem->ItemInfo->SourceIndex);
		pIcon->SetIcon( buf, LUCKYMACHINE_COIN_X,LUCKYMACHINE_COIN_Y, 1.0f );
		pIcon->Render();
	}

	// 2010. 05. 12 by jskim 기존 머신 축하 이펙트 보이는 부분을 인벤 쪽에서 럭키머신 쪽으로 변경
	RenderYouLucky();
	//end 2010. 05. 12 by jskim 기존 머신 축하 이펙트 보이는 부분을 인벤 쪽에서 럭키머신 쪽으로 변경
}


void CINFLuckyMachine::Tick()
{
	switch(m_nLuckyMechineStep)
	{ 	
	case LUCKY_MAINSTEP_SELECT_SHOP:		// 럭키 머신 상점 선택
		{
			TickLuckyStepStepSelectShop();
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:		// 럭키 머신 게임중
		{
			TickLuckyStepStepSelectGaming();
		}
		break;
	}

}
void CINFLuckyMachine::TickSlot(int i_nSlot,float i_fElapsedTime)
{
	int nRulType = m_nSlotType[i_nSlot];
	
	if(m_fTimeAllTimeStepSlot[i_nSlot] != -1)
	{
		////////////////////// 전체 스텝 시간  //////////////////////
		m_fTimeAllTimeStepSlot[i_nSlot] += i_fElapsedTime;
	}

	if(!m_bCompleteStep[i_nSlot])
	{
		////////////////////// 한개 스텝 유지할 시간 //////////////////////
		m_fTimeOneStepSlot[i_nSlot] += i_fElapsedTime;
		int nSlotStep = m_nSlotStep[i_nSlot];		
		
		if(m_fTimeOneStepSlot[i_nSlot] > m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fSlotTime)
		{						
			// 한단계 더 위로 올라가자
			m_bCompleteStep[i_nSlot] = SlotSpeedStepUp(i_nSlot);						
			UpdateCurrentSlotPos(i_nSlot);	// 이번 스텝에 대한 시작과의 갱신
		}
	}

	{
		////////////////////// 한칸 유지할 시간 //////////////////////		
		int nSlotStep = m_nSlotStep[i_nSlot];		
		if(m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fHoldTime <= m_fTimeHold[i_nSlot])
		{
			// 정해진 스텝 대로 이동 씩 이동.
			m_fTimeHold[i_nSlot] = 0.0f;
			m_nSlotStartPosIdx[i_nSlot] += m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].nSlotStep;
		}
		else
		{// 시간 계산.
			m_fTimeHold[i_nSlot] += i_fElapsedTime;
		}
		// 카운트값 에러 안나게 함
		if(m_nSlotStartPosIdx[i_nSlot] <0)
		{
			if(m_vecIconNum[i_nSlot].empty())
			{
				m_nSlotStartPosIdx[i_nSlot] = 0;
			}
			else
			{
				m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size());
			}
		}
		else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}		
		
	}
	
	if(m_bCompleteStep[i_nSlot] && !m_bSlotDone[i_nSlot])
	{
		// 마지막 바퀴를 돌면서 그 슬롯이 완료 됐는지 판단
		// 그 슬롯의 정보가 완료 됬�?
		m_bSlotDone[i_nSlot] = IsSlotDone(i_nSlot);	
	}	

	if(m_fTimeAllTimeStepSlot[i_nSlot] > MAX_LUCKY_MACHINE_TIME)
	{
		// 총 시간 초과 하면 강제로 셋팅
		m_fTimeAllTimeStepSlot[i_nSlot] = -1.0f;
		
		int nSlotStep = m_nSlotStep[i_nSlot];		

		int nCap = abs(m_nSuccessSlot[i_nSlot] - m_nSlotStartPosIdx[i_nSlot]);
		if(nCap > OVERTIME_LUCKY_MACHINE_STEP)
		{
			if(m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].nSlotStep > 0)
			{
				m_nSlotStartPosIdx[i_nSlot] = m_nSuccessSlot[i_nSlot] - OVERTIME_LUCKY_MACHINE_STEP;
			}
			else
			{
				m_nSlotStartPosIdx[i_nSlot] = m_nSuccessSlot[i_nSlot] + OVERTIME_LUCKY_MACHINE_STEP;
			}
		}
		
		// 카운트값 에러 안나게 함
		if(m_nSlotStartPosIdx[i_nSlot] <0)
		{
			if(m_vecIconNum[i_nSlot].empty())
			{
				m_nSlotStartPosIdx[i_nSlot] = 0;
			}
			else
			{
				m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size());
			}
		}
		else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}

		
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsSlotDone(int i_nSlot)
{
	if(m_vecIconNum[i_nSlot].empty())
	{
		// 아이템이 없으면 완료 된꺼당~
		return TRUE;
	}
	

	int nTakeGetItemNum = m_vecTakeItem[i_nSlot].Itemnum;
	int nCheckItemPos = m_nSlotStartPosIdx[i_nSlot];

// 	 // 완료 위치 체크 

	int nCompleteItemIdx = LUCKY_ITEM_POS;
	if(nCompleteItemIdx > (int)m_vecIconNum[i_nSlot].size())
	{
		nCompleteItemIdx = (int)m_vecIconNum[i_nSlot].size();
	}

	nCheckItemPos += nCompleteItemIdx;
	if(nCheckItemPos >= (int)m_vecIconNum[i_nSlot].size())
	{
		nCheckItemPos -= (int)m_vecIconNum[i_nSlot].size();
	}	
	if(nCheckItemPos < 0)
	{
		nCheckItemPos = 0;
	}
	

	ST_ICON_ITEM_NUMBER stIconNum = m_vecIconNum[i_nSlot][nCheckItemPos];
	if(stIconNum.nItemNum == nTakeGetItemNum)
	{
		// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_nAME_EffectNum[i_nSlot] = 0.001f;
		//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		return TRUE;
	}
	
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TickLuckyStepStepSelectShop()
{
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TickLuckyStepStepSelectGaming()
{
	FLOAT fElapsedTime = g_pD3dApp->GetElapsedTime();
	int i;
	

	// 2012-02-02 by jhahn 럭키 머신 UV 애니메이션 처리
	for (int nCount  = 0 ; nCount < LUCKY_SELECT_MAX; nCount++)
	{
		m_pImgMainBG[nCount]->Tick();
	}
	//end 2012-02-02 by jhahn 럭키 머신 UV 애니메이션 처리
	
	

	if(m_fLuckyTimeEffectBG > LUCKYMACHINE_EFFECT_FRAME_CHANGE_TIME)
	{
		m_nLuckyEffectBG = !m_nLuckyEffectBG;
		m_fLuckyTimeEffectBG = 0;
	}
	else
	{
		m_fLuckyTimeEffectBG += fElapsedTime;
	}

		if(m_fTimeEffectBG > LUCKYMACHINE_EFFECT_FRAME_CHANGE_TIME)
		{
		m_nEffectBG += 1;	
			m_fTimeEffectBG = 0.0f;
		}
		else
		{
			m_fTimeEffectBG += fElapsedTime;
		}
	if(m_nEffectBG > 6)
		m_nEffectBG = 0;

	switch(m_nState)
	{
	case LUCKY_STATE_READY:
		{
		}
		break;
	case LUCKY_STATE_RULLET:
		{			
			//for(i = 0; i < LUCKY_ITEM_LIST_NUM_LINE ;i++)
			for(i = 0 ; i < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind); i++)
			{
				if(m_bSlotDone[i])
				{					
					continue;
				}
				TickSlot(i,fElapsedTime);				
			}
			
			if(IsAllRuletDone())
			{
				// 모든 슬롯이 완료시 들어옴
				ChangeState(LUCKY_STATE_PRIZE);				
			}
			
		}
		break;
	case LUCKY_STATE_PRIZE:
		{
			
		}
		break;
	}
	for(i=0; i< LUCKY_ITEM_LIST_NUM_LINE; i++)
	{
		if(m_nAME_EffectNum[i] > 0)
			m_nAME_EffectNum[i] += g_pD3dApp->GetElapsedTime();

		if(m_nAME_EffectNum[i] > 1.0f)
		{
			m_nAME_EffectNum[i]=0.001f;
		}
	}
	
}
BOOL CINFLuckyMachine::SlotSpeedStepUp(int i_nSlot)
{	
	int nRulType = m_nSlotType[i_nSlot];
	
	int nOldSlotStep = 	m_nSlotStep[i_nSlot];
	int nSlotStep = m_nSlotStep[i_nSlot]+1;
	if(nSlotStep >= MAX_SLOT_RULLET_STEP)
	{
		m_nSlotStep[i_nSlot] = (MAX_SLOT_RULLET_STEP-1);
		return TRUE;
	}	
	
	BOOL bCompleteStep = m_stRulTime[i_nSlot][nRulType].stRulTime[nOldSlotStep].bCompleteStep;

	m_nSlotStep[i_nSlot] = nSlotStep;
	m_fTimeOneStepSlot[i_nSlot] = 0.0f;		

	//if(-1 == m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fSlotTime)
	if(bCompleteStep || (-1 == m_stRulTime[i_nSlot][nRulType].stRulTime[nSlotStep].fSlotTime))
	{
		m_nSlotStep[i_nSlot] = nOldSlotStep;
		// 스텝완료
		return TRUE;
	}	
	
	return FALSE;

}


int CINFLuckyMachine::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(INF_MSGPROC_BREAK == WndProcCommon(uMsg, wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}

	switch(m_nLuckyMechineStep)
	{ 	
	case LUCKY_MAINSTEP_SELECT_SHOP:		// 럭키 머신 상점 선택
		{
			return WndProcLuckyStepStepSelectShop(uMsg, wParam, lParam);
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:		// 럭키 머신 게임중
		{
			return WndProcLuckyStepStepSelectGaming(uMsg, wParam, lParam);
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFLuckyMachine::WndProcCommon(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pPrevStepBtn->OnMouseMove(pt);			
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pPrevStepBtn->OnLButtonDown(pt))
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
			if(m_pPrevStepBtn->OnLButtonUp(pt))
			{				
				OnClickPrevStepBtn();
				// 버튼 클릭 			
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				
				return INF_MSGPROC_BREAK;
			}			
			
		}
		break;
		
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFLuckyMachine::WndProcLuckyStepStepSelectShop(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_pSelectShopPrevBtn->OnMouseMove(pt);
			m_pSelectShopNextBtn->OnMouseMove(pt);			
			
			BOOL bSelectBtn = FALSE;
			char chTmp[MAX_PATH];
			ZERO_MEMORY(chTmp);

			
			{
				vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
				while(itNpcBtn != m_vecstruShopList.end())
				{
					structNPCList* pstruNPCList = (*itNpcBtn);
					
					if(pstruNPCList->pInfImageBtn && (pstruNPCList->pInfImageBtn->OnMouseMove(pt)))
					{
						bSelectBtn = TRUE;					
						strncpy(chTmp, pstruNPCList->struLuckyMachineOmi.szDescription, SIZE_MAX_LUCKYMACHINE_DESCRIPTION);
					}		
					itNpcBtn++;
				}
			}
			
			g_pGameMain->SetToolTip(pt.x,pt.y,chTmp);
			if(bSelectBtn)
			{				
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pSelectShopPrevBtn->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}
			if(m_pSelectShopNextBtn->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}
			
			{
				vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
				while(itNpcBtn != m_vecstruShopList.end())
				{
					structNPCList* pstruNPCList = (*itNpcBtn);
					
					if(pstruNPCList->pInfImageBtn && pstruNPCList->pInfImageBtn->OnLButtonDown(pt))
					{
						return INF_MSGPROC_BREAK;
					}		
					itNpcBtn++;
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
			if(m_pSelectShopPrevBtn->OnLButtonUp(pt))
			{						
				OnClickNPCListPos(-1);
				// 버튼 클릭 			
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
				
				return INF_MSGPROC_BREAK;
			}
			if(m_pSelectShopNextBtn->OnLButtonUp(pt))
			{						
				OnClickNPCListPos(1);
				// 버튼 클릭 			
				g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);						
				return INF_MSGPROC_BREAK;
			}		
			
			{
				vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
				while(itNpcBtn != m_vecstruShopList.end())
				{
					structNPCList* pstruNPCList = (*itNpcBtn);					
					
					if(pstruNPCList->pInfImageBtn && (pstruNPCList->pInfImageBtn->OnLButtonUp(pt)))
					{
						if(NOT_OPEN_LUCKY_MACHINE != pstruNPCList->struLuckyMachineOmi.SourceIndex)
						{
							OnClickSelectNPCShop(&pstruNPCList->struLuckyMachineOmi);
							
							g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
						}
						return INF_MSGPROC_BREAK;
					}		
					itNpcBtn++;
				}
			}
			
		}
		break;
		
	}
	return INF_MSGPROC_NORMAL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFLuckyMachine::WndProcLuckyStepStepSelectGaming(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_MOUSEMOVE:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

					// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
					if(m_pBtnGamble->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE) 
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_081201_0100);
						return INF_MSGPROC_BREAK;
					}										
					else if(m_pBtnGamble1->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_081201_0100);
						return INF_MSGPROC_BREAK;
					}
					if(m_pBtnTake->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
					{
					}
					else if(m_pBtnTake1->OnMouseMove(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
					{
					}
					//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				
				if(pt.x > LUCKYMACHINE_COIN_X &&
					pt.x < LUCKYMACHINE_COIN_X + ICON_SIZE_W &&
					pt.y > LUCKYMACHINE_COIN_Y &&
					pt.y < LUCKYMACHINE_COIN_Y + ICON_SIZE_H)
				{
					g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_081201_0102);
					return INF_MSGPROC_BREAK;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);
				if(m_pBtnGamble->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				else if(m_pBtnGamble1->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				if(m_pBtnTake->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				else if(m_pBtnTake1->OnLButtonDown(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(m_pBtnGamble->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					GambleButtonOK();
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				else if(m_pBtnGamble1->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					GambleButtonOK();
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				if(m_pBtnTake->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind != LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					PrizeItemGiveMe();
					return INF_MSGPROC_BREAK;
				}
				// 2010. 06. 14 by jskim 해피머신 버튼 UI 변경
				else if(m_pBtnTake1->OnLButtonUp(pt) && m_struSelLuckyMachine.MachineKind == LUCKY_MACHINE_KIND_HAPPY_MACHINE)
				{
					PrizeItemGiveMe();
					return INF_MSGPROC_BREAK;
				}
				//end 2010. 06. 14 by jskim 해피머신 버튼 UI 변경

				if(pt.x > LUCKYMACHINE_COIN_X &&
					pt.x < LUCKYMACHINE_COIN_X + ICON_SIZE_W &&
					pt.y > LUCKYMACHINE_COIN_Y &&
					pt.y < LUCKYMACHINE_COIN_Y + ICON_SIZE_H &&
					((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
					((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
				{
					if(m_nState == LUCKY_STATE_RULLET)
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081201_0101,COLOR_ERROR);
						break;
					}
					
					if(m_bPrizeItem)
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102, COLOR_ERROR);
						break;
					}
					// 2009-04-14 by bhsohn 인벤 3개 미만일시, 럭키머신 시 버그 수정
					if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > g_pGameMain->m_pInven->GetInvenFreeSize())
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0101,COLOR_ERROR);
						break;
					}
					// end 2009-04-14 by bhsohn 인벤 3개 미만일시, 럭키머신 시 버그 수정
					if(m_pCoinItemInfo == NULL)
					{
						if(g_pGameMain->m_stSelectItem.pSelectItem &&
							g_pGameMain->m_stSelectItem.pSelectItem->pItem)
						{
							//CItemInfo* pItemInfo = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;
							CItemInfo* pItemInfo = (CItemInfo*)g_pGameMain->m_stSelectItem.pSelectItem->pItem;
							// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
							if(pItemInfo && pItemInfo->ItemInfo
								//&& COMPARE_BIT_FLAG(pItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_LUCKY_ITEM)
								&& (IsPossibleUpLoadCoin(pItemInfo->ItemInfo)))	
							{
								// LuckyMachine에서 쓰이는 아이템이면 								
// 								util::del(m_pCoinItemInfo);
// 								m_pCoinItemInfo = new CItemInfo((ITEM_GENERAL*)pItemInfo);
// 								m_pCoinItemInfo->CurrentCount = 1;								 
// 								
// 								g_pStoreData->UpdateItemCount(pItemInfo->UniqueNumber, pItemInfo->CurrentCount - 1);
// 								g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_COIN, D3DXVECTOR3(0,0,0), FALSE);	

								// 동전을 올리자
								UpLoadCoin((ITEM_GENERAL*)pItemInfo);								
							}
							else
							{
								g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0102,COLOR_ERROR);//"해당 상점에서 사용 할 수 없는 아이템 입니다."
							}
						}
						else
						{
							DBGOUT("m_stSelectItem Error\n");
						}					
					}
				}
				//end 2010. 05. 04 by jskim 아머 머신 UI 변경
			}
			break;

	}
	return INF_MSGPROC_NORMAL;
}

INT CINFLuckyMachine::FindSearchItem(INT i_nSlotNum, INT i_nMysteryNum)
{
	i_nSlotNum = i_nSlotNum - 1;
	// 2009-03-04 by bhsohn 럭키 머신 수정안
	if(i_nSlotNum < 0 || i_nSlotNum >= LUCKY_ITEM_LIST_NUM_LINE)
	{
		return 0;
	}
	// end 2009-03-04 by bhsohn 럭키 머신 수정안
	m_vecIconNum[i_nSlotNum].clear();

	// 입력받은 아이템 번호로 뽑을수 있는 아이템을 vector에 등록해준다.
	vector<MYSTERY_ITEM_DROP*> vecMystery;
	g_pDatabase->FindMysteryInfo(i_nMysteryNum,&vecMystery);

	DBGOUT("Lucky Machine %d Slot List\n",i_nSlotNum);
	

	vector<MYSTERY_ITEM_DROP*>::iterator it = vecMystery.begin();
	ST_ICON_ITEM_NUMBER stNum;
	while(it != vecMystery.end())
	{
		ITEM * pItem = g_pDatabase->GetServerItemInfo((*it)->DropItemNum);
		// 2009-05-08 by bhsohn 럭키 머신 아이템 정보 문제시, 클라이언트 강제 종료
		if(NULL == pItem)
		{
			char szTemp[256] = {0, };
				sprintf ( szTemp , "%s\n[%s]" , STRMSG_C_060608_0000 , "LuckMachine:Can't Found ItemInfo." );

			g_pD3dApp->NetworkErrorMsgBox ( szTemp );
			//g_pD3dApp->NetworkErrorMsgBox(STRMSG_C_060608_0000);	// "서버와 접속이 종료되었습니다. 게임이 종료됩니다."
			break;
		}
		// end 2009-05-08 by bhsohn 럭키 머신 아이템 정보 문제시, 클라이언트 강제 종료
		
		stNum.nIconNum = pItem->SourceIndex;
		stNum.nItemNum = pItem->ItemNum;
		
		m_vecIconNum[i_nSlotNum].push_back(stNum);
		DBGOUT("Lucky Machine Item Num : %d \n",stNum.nItemNum);
		it++;
	}
	stNum.nIconNum = LUCKYMACHINE_BLANK_ICON_NUMBER;
	stNum.nItemNum = LUCKYMACHINE_BLANK_ICON_NUMBER;
	m_vecIconNum[i_nSlotNum].push_back(stNum);
	random_shuffle(m_vecIconNum[i_nSlotNum].begin(),m_vecIconNum[i_nSlotNum].end());
	//
	if(m_vecIconNum[i_nSlotNum].size() < LUCKY_ITEM_LIST_NUM)
	{// Error -> 링크 아이템은 적어도 7개 이상이어야함.
		return -1;
	}
	return 0;
}
void CINFLuckyMachine::ClearSearchItem()
{
	m_vecIconNum[0].clear();
	m_vecIconNum[1].clear();
	m_vecIconNum[2].clear();
	m_vecTakeItem.clear();
	
}
void CINFLuckyMachine::Init()
{
	g_pShuttleChild->ResortingItem();// 여기
	if(g_pGameMain && g_pGameMain->m_pInven)
	{
		g_pGameMain->m_pInven->SetScrollEndLine();				// 2006-06-21 by ispark
		g_pGameMain->m_pInven->SetAllIconInfo();
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFLuckyMachine::TakeItem(LUCKY_ITEM_SIMPLEINFO i_sItemInfo)
/// \brief		내가 받을 아이템.
/// \author		dgwoo
/// \date		2008-11-04 ~ 2008-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TakeItem(LUCKY_ITEM_SIMPLEINFO i_sItemInfo)
{
	if(i_sItemInfo.Itemnum == 0)
		i_sItemInfo.Itemnum = LUCKYMACHINE_BLANK_ICON_NUMBER;
	
	m_vecTakeItem.push_back(i_sItemInfo);
	DBGOUT("***** Lucky Machine Take Item Num : %d \n",i_sItemInfo.Itemnum);
}


void CINFLuckyMachine::SetPrizeItem(ITEM_GENERAL i_Item)
{
	m_vecPrizeItem.push_back(i_Item);
	DBGOUT("** Lucky Machine SetPrizeItem Num : %d \n",i_Item.ItemNum);
	m_bPrizeItem = TRUE;
}


void CINFLuckyMachine::SetPrizeItem(MSG_FC_TRADE_UPDATE_ITEM_COUNT *pMsg)
{
	MSG_FC_TRADE_UPDATE_ITEM_COUNT sMsg;
	sMsg.ItemUniqueNumber		= pMsg->ItemUniqueNumber;
	sMsg.ItemUpdateType			= pMsg->ItemUpdateType;
	sMsg.NewCount				= pMsg->NewCount;
	m_vecPrizeItemCount.push_back(sMsg);
	DBGOUT("** Lucky Machine SetPrizeItem(c) Num : %d \n",pMsg->ItemUniqueNumber);
	m_bPrizeItem = TRUE;
}


/**********************************************************************
**
**	받을 아이템 목록에서 아이템 삭제.
**
**	Create Info :	2010. 08. 02. by hsLee.
**
***********************************************************************/
void CINFLuckyMachine :: SetDeletePrizeItem ( const UID64_t a_ItemUniqueNumber )
{

	// Insert 목록 체크.
	vector<ITEM_GENERAL>::iterator itr_InsertItem = m_vecPrizeItem.begin();

	for( ; itr_InsertItem != m_vecPrizeItem.end(); itr_InsertItem++ )
	{
		if ( a_ItemUniqueNumber == itr_InsertItem->UniqueNumber )
		{
			m_vecPrizeItem.erase ( itr_InsertItem ); 
			break;
		}
	}

	// Update 목록 체크.
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>::iterator itr_UpdateItem = m_vecPrizeItemCount.begin();
	
	for ( ; itr_UpdateItem != m_vecPrizeItemCount.end(); itr_UpdateItem++ )
	{
		if ( a_ItemUniqueNumber == itr_UpdateItem->ItemUniqueNumber )
		{
			m_vecPrizeItemCount.erase ( itr_UpdateItem ); 
			break;
		}
	}
}


void CINFLuckyMachine::PrizeItemGiveMe()
{
	if(!m_bPrizeItem)
	{
		return;
	}
	else if(!IsAllRuletDone())
	{
		// "럭키머신이 완전히 멈춘 후에 아이템을 받을 수 있습니다."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0100,COLOR_ERROR);
		return;
	}

	//if(	m_bSlotDone[0] && m_bSlotDone[1] && m_bSlotDone[2])
	vector<ITEM_GENERAL>::iterator it = m_vecPrizeItem.begin();
	while(it != m_vecPrizeItem.end())
	{
		ITEM_GENERAL itemGeneral = (*it);
		g_pStoreData->PutItem((char*)&itemGeneral, TRUE);
		char buf[256];
		
		if(IS_COUNTABLE_ITEM(itemGeneral.ItemInfo->Kind))
		{
			wsprintf(buf,STRMSG_C_081124_0103,itemGeneral.ItemInfo->ItemName,itemGeneral.CurrentCount);
		}
		else
		{
			wsprintf(buf,STRMSG_C_081124_0103,itemGeneral.ItemInfo->ItemName,1);
		}

		g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		it++;
	}
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>::iterator itc =m_vecPrizeItemCount.begin();
	while(itc != m_vecPrizeItemCount.end())
	{
		//g_pStoreData->PutItem((char*)((ITEM_GENERAL*)it), TRUE);
		
		char buf[256];
		ZERO_MEMORY(buf);
		
		if(itc->ItemUniqueNumber == g_pGameMain->m_pInven->GetItemSpiUniqueNumber())
		{
			// SPI아이템을 먹었다.				
			wsprintf( buf, STRMSG_C_ITEM_0002, itc->NewCount - g_pGameMain->m_pInven->GetItemSpi());//"%d 스피가 추가 되었습니다."
			g_pGameMain->m_pInven->SetItemSpi(itc->NewCount);
		}
		else
		{
			CItemInfo* pItem = g_pStoreData->FindItemInInventoryByUniqueNumber( itc->ItemUniqueNumber );
			if(pItem)
			{
				if(IS_COUNTABLE_ITEM(pItem->ItemInfo->Kind))
				{
					wsprintf(buf,STRMSG_C_081124_0103,pItem->ItemInfo->ItemName,itc->NewCount - pItem->CurrentCount);
				}
				else
				{
					wsprintf(buf,STRMSG_C_081124_0103,pItem->ItemInfo->ItemName,1);
				}
				g_pStoreData->UpdateItemCount( itc->ItemUniqueNumber, itc->NewCount );				
			}
		}
		if(strlen(buf) > 0)
		{
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
		itc++;		
	}

	m_vecPrizeItem.clear();
	m_vecPrizeItemCount.clear();
	m_bPrizeItem = FALSE;

	for(int i=0; i<3; i++)
	{
		m_nAME_EffectNum[i] = 0.0f;
	}	
	Init();
	ChangeState(LUCKY_STATE_READY);
}

void CINFLuckyMachine::GambleButtonOK()
{
	if(m_bPrizeItem)			// 받을것이 있다면 겜블은 되지 않는다.
		return;
	ClearSearchItem();
	if(LUCKY_STATE_RULLET == m_nState ||
		LUCKY_STATE_PRIZE == m_nState)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0101,COLOR_ERROR);
		return;
	}
	else if(NULL == m_pCoinItemInfo)
	{
		//"주화가 없습니다. 주화를 넣고 다시 시도해 주세요."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0100,COLOR_ERROR);
		return;
	}
	else if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > g_pGameMain->m_pInven->GetInvenFreeSize())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0101,COLOR_ERROR);
		return;
	}

	int nSlot = 0;	
	for(nSlot = 0 ; nSlot < CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind); nSlot++)
	{	
		m_fTimeAllTimeStepSlot[nSlot] = 0.0f;
		m_nSuccessSlot[nSlot]	= 0;		// 성공 위치
		m_nSlotType[nSlot] = nSlot;		
	}
	if(LUCKY_SELECT_TYPE_MINI == m_struSelLuckyMachine.MachineKind)
	{
		// 미니는 가장 느린 패턴으로
		m_nSlotType[0] = 2;		
	}


	if(m_struSelLuckyMachine.MachineNum != 0)
	{
		MSG_FC_ITEM_USE_LUCKY_ITEM sMsg;

		// 2009-03-04 by bhsohn 럭키 머신 수정안
		memset(&sMsg, 0x00, sizeof(MSG_FC_ITEM_USE_LUCKY_ITEM));
		sMsg.MachineNum = m_struSelLuckyMachine.MachineNum;
		// end 2009-03-04 by bhsohn 럭키 머신 수정안

		sMsg.ItemUID = m_pCoinItemInfo->UniqueNumber;

		ClearConinItemInfo();

		g_pFieldWinSocket->SendMsg(T_FC_ITEM_USE_LUCKY_ITEM,(char*)&sMsg,sizeof(sMsg));
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_SLOT, D3DXVECTOR3(0,0,0), FALSE);		

		// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
// 		CItemInfo* pCoinSelItem = g_pStoreData->FindItemInInventoryByUniqueNumber(sMsg.ItemUID);
// 		if(pCoinSelItem && pCoinSelItem->ItemInfo)
// 		{
// 			// 코인이 이미 있으면 다시 넣자 
// 			UpLoadCoin((ITEM_GENERAL*)pCoinSelItem);
// 		}
		// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안

	}	

}
void CINFLuckyMachine::ChangeState(int i_nState)
{
	switch(i_nState)
	{
	case LUCKY_STATE_READY:
		ResetData();
		break;
	case LUCKY_STATE_RULLET:
		{
			vector<LUCKY_ITEM_SIMPLEINFO>::iterator it = m_vecTakeItem.begin();
			while(it != m_vecTakeItem.end())
			{
				LUCKY_ITEM_SIMPLEINFO stLucky = (*it);
				if(-1 == FindSearchItem(stLucky.SlotNum,stLucky.MysteryItemDropNum))
				{
					//m_bSlotDone[stLucky.SlotNum-1] = TRUE;
					//DBGOUT("Lucky Machine Error(count deficiency) : %d Slot %d MysteryItem\n",stLucky.SlotNum,stLucky.MysteryItemDropNum);
				}
				it++;
			}
			// 벡터에 애드하고 그 값을 통해 완료 품을 찾자.
			UpdateSuccessSlot();			

		}
		break;
	case LUCKY_STATE_PRIZE:
		{
			// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
			vector<LUCKY_ITEM_SIMPLEINFO>::iterator it = m_vecTakeItem.begin();
			while(it != m_vecTakeItem.end())
			{
				LUCKY_ITEM_SIMPLEINFO stLucky = (*it);
				if(IsShowLucySuccessEffect(stLucky.MysteryItemDropNum, stLucky.Itemnum))
				{
					ShowLuckySuccessEffect();					
					break;
				}
				it++;
			}
			// end 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
			g_pD3dApp->m_pSound->StopD3DSound(SOUND_LUCKY_SLOT);
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102,COLOR_ERROR);					
			
		}
		break;
	}
	m_nState = i_nState;
}

void CINFLuckyMachine::OnCloseInfWnd()
{
	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	if(m_pCoinItemInfo)
	{	
// 		m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;
// 		if(g_pStoreData)
// 		{
// 			g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), TRUE);
// 		}
// 		util::del(m_pCoinItemInfo);
// 		m_pCoinItemInfo = NULL;
		m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;

 		CItemInfo* pCoinSelItem = g_pStoreData->FindItemInInventoryByUniqueNumber(m_pCoinItemInfo->UniqueNumber);
 		if(pCoinSelItem && pCoinSelItem->ItemInfo)
 		{
 			// 이미 있으면 아이템 카운트 수를 더하자			
 			g_pStoreData->UpdateItemCount(m_pCoinItemInfo->UniqueNumber, pCoinSelItem->CurrentCount + m_pCoinItemInfo->CurrentCount);
 		}
		// 2009-04-14 by bhsohn 인벤 3개 미만일시, 럭키머신 시 버그 수정
		else if(NULL == pCoinSelItem)
		{
			g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), TRUE);			
		}
		// end 2009-04-14 by bhsohn 인벤 3개 미만일시, 럭키머신 시 버그 수정
		ClearConinItemInfo();
	}
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	PrizeItemGiveMe();
	// 2010. 05. 27 by jskim 신규 럭키 머신 구현 - 버그 수정
	m_fYouLuckyViewTime = 0.0f;
	m_dwYouLuckyAlpha = 0;		
	m_bYouLucky = FALSE;
	//end 2010. 05. 27 by jskim 신규 럭키 머신 구현 - 버그 수정
}
BOOL CINFLuckyMachine::IsCloseLucky()
{
	if(LUCKY_STATE_RULLET == m_nState ||
		LUCKY_STATE_PRIZE == m_nState)
	{
		return FALSE;
	}
	return TRUE;
}
UID64_t CINFLuckyMachine::GetSelUID()
{
	if(m_pCoinItemInfo)
		return m_pCoinItemInfo->UniqueNumber;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		내 위치에 이펙트 보이게 한다.
/// \author		// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsShowLucySuccessEffect(INT	i_MysteryItemDropNum, int i_nMysteryNum)
{
	if(LUCKYMACHINE_BLANK_ICON_NUMBER == i_nMysteryNum)
	{
		return FALSE;		
	}
	MYSTERY_ITEM_DROP*	pMisteyItemDrop = g_pDatabase->GetMysteryInfo(i_MysteryItemDropNum, i_nMysteryNum);
	if(NULL == pMisteyItemDrop)
	{
		return FALSE;
	}

	if(pMisteyItemDrop->Period > 0)
	{
		return TRUE;
	}
	
	return FALSE;	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		내 위치에 이펙트 보이게 한다.
/// \author		// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
/// \date		2008-12-29 ~ 2008-12-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ShowLuckySuccessEffect()
{
	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	m_bYouLucky = TRUE;	
	m_fYouLuckyViewTime= 0.0f;
	m_dwYouLuckyAlpha= 0;
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안

	D3DXVECTOR3	vPos = g_pD3dApp->GetMyShuttleCharacterPos();	
	D3DXVECTOR3	vVel, vUp;	
	
	vUp = D3DXVECTOR3(0,1,0);
	
	vVel = g_pD3dApp->GetViewDirVel();
	vVel.y = 0;
	D3DXVec3Normalize(&vVel,&vVel);

	g_pD3dApp->CreateMyAppEffectData(RC_EFF_LUCKY_SUCCESS, vPos, vVel, vUp); // 내 위치에 이펙트 생성

	// 이펙트를 다른 사람도 볼수있게한다
	{
		D3DXVECTOR3	vSVel, vSUp;	
		
		vSUp = D3DXVECTOR3(0,1,0);		
		vSVel = D3DXVECTOR3(0,0,0); // 방향벡터가 크기가 0이면 받는 사람 방향벡터로 한다.		

		MSG_FC_CHARACTER_SHOW_MAP_EFFECT sMsg;
		memset(&sMsg, 0x00, sizeof(MSG_FC_CHARACTER_SHOW_MAP_EFFECT));
		
		sMsg.avec3Position		= vPos;
		sMsg.avec3Target		= vSVel*1000.0f;
		sMsg.avec3Up			= vSUp*1000.0f;
		sMsg.nLifetime			= 10;
		sMsg.EffectIndex		= RC_EFF_LUCKY_SUCCESS;
		
		g_pD3dApp->SendCharcterShowMapEffect(&sMsg);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		UINT		BuildingIndex;							// 건물(가게 등) 고유 번호
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::EnterMachineShop(UINT i_uBuildingIndex)
{	
	m_nLuckySelectStartIdx = 0;
	memset(&m_struSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));			// 선택한 럭키 상점 초기화
	m_nLuckyMechineStep = -1;

	m_uBuildingIndex = i_uBuildingIndex;

	m_bInfluenceOneShopMode = FALSE;
	
	// 코인 정보 초기화
	ClearConinItemInfo();
	
	LUCKY_MACHINE_OMI stSelLuckyMachine;	
	if(GetInfluenceOneShop(&stSelLuckyMachine))
	{	
		// 이미지가 한개있다.
		CreateNPCListBtn();
		m_bInfluenceOneShopMode = TRUE;
		// NPCShop버튼 자동 클릭
		OnClickSelectNPCShop(&stSelLuckyMachine);		
	}
// 	else
// 	{
// 		SetLuckyMechineStep(LUCKY_MAINSTEP_SELECT_SHOP);	
// 	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::SetLuckyMechineStep(int nLuckyMechineStep)
{	
	int nOldLuckyMechineStep = m_nLuckyMechineStep;

	m_nLuckyMechineStep = nLuckyMechineStep;

	if(nOldLuckyMechineStep == nLuckyMechineStep)
	{
		// 전하고 같으면 업데이트 할 필요없다.
		return;		
	}
	RefreshLuckyMachineCtrl();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RefreshNPCListBtnStat()
{
// 	AddNPCListBtn(1000);
// 	AddNPCListBtn(1001);
// 	AddNPCListBtn(1002);
// 	AddNPCListBtn(1003);

// 	
// 	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
// 	int nCnt = 0;
// 	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
// 	{
// 		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
// 		if(NULL == pLuckyMachine)
// 		{
// 			break;
// 		}
// 		AddNPCListBtn(pLuckyMachine->SourceIndex);
// 	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::CreateNPCListBtn()
{
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	
	// 세력 타입으로
	// 2009-05-13 by bhsohn 럭키 상점 버그 수정
	if((myShuttleInfo.InfluenceType != m_nLoadNpcListInfluenceType)
		||(m_uLoadBuildingIndex != m_uBuildingIndex))
	{
		// 세력이 틀리거나 상점 인덱스가 틀리면 버튼 초기화
		DeleteDeviceObjectsNPCListBtn();		
	}
	m_nLoadNpcListInfluenceType = myShuttleInfo.InfluenceType;	
	// 2009-05-13 by bhsohn 럭키 상점 버그 수정
	m_uLoadBuildingIndex = m_uBuildingIndex;
	// end 2009-05-13 by bhsohn 럭키 상점 버그 수정

	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
	int nCnt = 0;
	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
	{
		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
		if(NULL == pLuckyMachine)
		{
			break;
		}
		
		if(IsHasNPCBtn(pLuckyMachine) // 이미 있는 소스면 다음으로 넘기자
			|| (!IsMyBelignece(pLuckyMachine))) // 같은 세력이냐>?
		{			
			continue;
		}		

		structNPCList* pstruNPCList = new structNPCList;

		memcpy(&pstruNPCList->struLuckyMachineOmi, pLuckyMachine, sizeof(LUCKY_MACHINE_OMI));

		int nSourceIndex = pstruNPCList->struLuckyMachineOmi.SourceIndex;

		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "%d_3", nSourceIndex);
		wsprintf(szDownBtn, "%d_1", nSourceIndex);
		wsprintf(szSelBtn, "%d_0", nSourceIndex);
		wsprintf(szDisBtn, "%d_2", nSourceIndex);

		// 2009. 05. 07 by ckPark 럭키머신 버튼 수정
		//pstruNPCList->pInfImageBtn = new CINFImageBtn;
		pstruNPCList->pInfImageBtn = new CINFAUTOSPRITEBTN( LUCKY_MACHINE_BTN_FRAME_COUNT, LUCKY_MACHINE_BTN_FRAME );
		// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정
		pstruNPCList->pInfImageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		pstruNPCList->pInfImageBtn->RestoreDeviceObjects();
		pstruNPCList->pInfImageBtn->ShowWindow(FALSE);

		char chTmp[16];		
		DataHeader	* pDataHeader = NULL;
		
		wsprintf(chTmp, "%d_5", nSourceIndex);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			pstruNPCList->pInfShopImage[0] = new CINFImageEx;
			pstruNPCList->pInfShopImage[0]->InitDeviceObjects(pDataHeader );
			pstruNPCList->pInfShopImage[0]->RestoreDeviceObjects();
		}

		wsprintf(chTmp, "%d_6", nSourceIndex);
		pDataHeader = FindResource(chTmp);
		if(pDataHeader)
		{
			pstruNPCList->pInfShopImage[1] = new CINFImageEx;
			pstruNPCList->pInfShopImage[1]->InitDeviceObjects(pDataHeader );
			pstruNPCList->pInfShopImage[1]->RestoreDeviceObjects();
		}

		pstruNPCList->bReStore = TRUE;		
		
		m_vecstruShopList.push_back(pstruNPCList);		
	}

	{
		if(((int)m_vecstruShopList.size()) < MAX_ONEPAGE_NPCLIST_COUNT)
		{
			// 한페이지 수 보다 작으면 채워줘야한다.
			int nDummyLen = MAX_ONEPAGE_NPCLIST_COUNT - ((int)m_vecstruShopList.size());
			int nCnt = 0;
			for(nCnt = 0;nCnt < nDummyLen;nCnt++)
			{
				structNPCList* pstruNPCList = new structNPCList;
				
				// 초기화
				memset(&pstruNPCList->struLuckyMachineOmi, 0x00, sizeof(LUCKY_MACHINE_OMI));			
				
				pstruNPCList->struLuckyMachineOmi.SourceIndex = NOT_OPEN_LUCKY_MACHINE;			// 상점없음
				int nSourceIndex = pstruNPCList->struLuckyMachineOmi.SourceIndex;
				
				char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
				wsprintf(szUpBtn, "%d", nSourceIndex);
				wsprintf(szDownBtn, "%d", nSourceIndex);
				wsprintf(szSelBtn, "%d", nSourceIndex);
				wsprintf(szDisBtn, "%d", nSourceIndex);
				
				// 2009. 05. 07 by ckPark 럭키머신 버튼 수정
				//pstruNPCList->pInfImageBtn = new CINFImageBtn;
				pstruNPCList->pInfImageBtn = new CINFAUTOSPRITEBTN( LUCKY_MACHINE_BTN_FRAME_COUNT, LUCKY_MACHINE_BTN_FRAME );
				// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정
				pstruNPCList->pInfImageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
				pstruNPCList->pInfImageBtn->RestoreDeviceObjects();
				pstruNPCList->pInfImageBtn->ShowWindow(FALSE);
				
				pstruNPCList->pInfShopImage[0] = NULL;
				pstruNPCList->pInfShopImage[1] = NULL;
				
				pstruNPCList->bReStore = TRUE;
				
				m_vecstruShopList.push_back(pstruNPCList);				
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsMyBelignece(LUCKY_MACHINE_OMI* pLuckyMechineOmi)
{
	if(m_uBuildingIndex == pLuckyMechineOmi->BuildingInx)
	{
		return TRUE;
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsHasNPCBtn(LUCKY_MACHINE_OMI* pLuckyMechineOmi)
{	
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		if((pLuckyMechineOmi->BuildingInx == pstruNPCList->struLuckyMachineOmi.BuildingInx)
			&&(pLuckyMechineOmi->MachineNum == pstruNPCList->struLuckyMachineOmi.MachineNum))
		{
			return TRUE;
		}
		itNpcBtn++;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::InitDeviceObjectsNPCListBtn()
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(NULL == pstruNPCList->pInfImageBtn)
		{
			char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
			wsprintf(szUpBtn, "%d_3", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			wsprintf(szDownBtn, "%d_1", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			wsprintf(szSelBtn, "%d_0", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			wsprintf(szDisBtn, "%d_2", pstruNPCList->struLuckyMachineOmi.SourceIndex);
			
			// 2009. 05. 07 by ckPark 럭키머신 버튼 수정
			//pstruNPCList->pInfImageBtn = new CINFImageBtn;
			pstruNPCList->pInfImageBtn = new CINFAUTOSPRITEBTN( LUCKY_MACHINE_BTN_FRAME_COUNT, LUCKY_MACHINE_BTN_FRAME );
			// end 2009. 05. 07 by ckPark 럭키머신 버튼 수정
			pstruNPCList->pInfImageBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
			
			pstruNPCList->bReStore = FALSE;
		}
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
		{
			if(NULL == pstruNPCList->pInfShopImage[nCnt])
			{
				char chTmp[16];							
				DataHeader	* pDataHeader = NULL;					
				wsprintf(chTmp, "%d_%d", pstruNPCList->struLuckyMachineOmi.SourceIndex, (5+nCnt));
				
				pDataHeader = FindResource(chTmp);
				if(pDataHeader)
				{
					pstruNPCList->pInfShopImage[nCnt] = new CINFImageEx;
					pstruNPCList->pInfShopImage[nCnt]->InitDeviceObjects(pDataHeader);
				}					
			}
		}
		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RestoreDeviceObjectsNPCListBtn()
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if( !pstruNPCList->bReStore)
		{
			if(pstruNPCList->pInfImageBtn)
			{
				pstruNPCList->pInfImageBtn->RestoreDeviceObjects();					
			}
			int nCnt = 0;
			for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
			{
				if(pstruNPCList->pInfShopImage[nCnt])
				{
					pstruNPCList->pInfShopImage[nCnt]->RestoreDeviceObjects();								
				}
			}
			pstruNPCList->bReStore = TRUE;
		}
		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::InvalidateDeviceObjectsNPCListBtn()
{	
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->bReStore)
		{
			if(pstruNPCList->pInfImageBtn )
			{
				pstruNPCList->pInfImageBtn->InvalidateDeviceObjects();		
			}
			int nCnt = 0;
			for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
			{
				if(pstruNPCList->pInfShopImage[nCnt])
				{
					pstruNPCList->pInfShopImage[nCnt]->InvalidateDeviceObjects();								
				}
			}
			
			pstruNPCList->bReStore = FALSE;
		}
		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::DeleteDeviceObjectsNPCListBtn()
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->bReStore = FALSE;
			pstruNPCList->pInfImageBtn->DeleteDeviceObjects();					
			util::del(pstruNPCList->pInfImageBtn);			
		}
		int nCnt = 0;
		for(nCnt = 0;nCnt < MAX_SHOP_BG;nCnt++)
		{
			if(pstruNPCList->pInfShopImage[nCnt])
			{
				pstruNPCList->pInfShopImage[nCnt]->DeleteDeviceObjects();					
				util::del(pstruNPCList->pInfShopImage[nCnt]);							
			}
		}
		util::del((*itNpcBtn));		
		itNpcBtn++;
	}	
	
	m_vecstruShopList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ShowNPCListBtn(BOOL i_bShow)
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->pInfImageBtn->ShowWindow(i_bShow);
		}		
		itNpcBtn++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// NPC버튼 위치 지정
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::RefreshNPCListBtnPos()
{
	BOOL bRtnValue = TRUE;	// 한개라도 X가 있냐?	
	
	// 2009. 11. 09 by ckPark 럭키머신 월별버튼 버그 수정
	//m_nLuckySelectStartIdx = 0;
	// end 2009. 11. 09 by ckPark 럭키머신 월별버튼 버그 수정

	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	int nCnt = 0;
	for(nCnt = 0;nCnt < m_nLuckySelectStartIdx;nCnt++)
	{
		if(itNpcBtn == m_vecstruShopList.end())
		{
			break;
		}
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->pInfImageBtn->ShowWindow(FALSE);
		}		

		itNpcBtn++;

	}
	// 페이지 수
	for(nCnt = 0;nCnt < MAX_ONEPAGE_NPCLIST_COUNT;nCnt++)
	{
		if(itNpcBtn == m_vecstruShopList.end())
		{
			break;
		}
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			if(NOT_OPEN_LUCKY_MACHINE == pstruNPCList->struLuckyMachineOmi.SourceIndex)
			{
				// 상점없음
				bRtnValue = FALSE;
			}
			int nPosX = LUCKYMACHINE_SELECT_SHOP_NPC_X+(nCnt*LUCKYMACHINE_SELECT_SHOP_NPC_WIDTH);
			int nPosY = LUCKYMACHINE_SELECT_SHOP_NPC_Y;
			pstruNPCList->pInfImageBtn->SetBtnPosition(nPosX, nPosY);
			pstruNPCList->pInfImageBtn->ShowWindow(TRUE);
		}		
		itNpcBtn++;
	}
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);
		
		if(pstruNPCList->pInfImageBtn)
		{
			pstruNPCList->pInfImageBtn->ShowWindow(FALSE);
		}		
		itNpcBtn++;
	}
	return bRtnValue;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPC표시
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::OnClickNPCListPos(int i_nStep)
{	
	int nTmpLuckySelectStartIdx = m_nLuckySelectStartIdx;
	nTmpLuckySelectStartIdx += i_nStep;

	int nShopListLen = ((int)m_vecstruShopList.size()) - MAX_ONEPAGE_NPCLIST_COUNT;

	if(nTmpLuckySelectStartIdx > nShopListLen)
	{
		nTmpLuckySelectStartIdx = nShopListLen;
	}

	if(nTmpLuckySelectStartIdx < 0)
	{
		nTmpLuckySelectStartIdx = 0;
	}
	
	m_nLuckySelectStartIdx = nTmpLuckySelectStartIdx;

	// 2009. 11. 09 by ckPark 럭키머신 월별버튼 버그 수정
	DeleteDeviceObjectsNPCListBtn();
	// end 2009. 11. 09 by ckPark 럭키머신 월별버튼 버그 수정

	RefreshLuckyMachineCtrl();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPC표시
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RefreshLuckyMachineCtrl()
{
	BOOL bPrevBtn, bTypeNorLuckyBtn, bTypeMiniLuckyBtn, bShopPrevBtn, bShopNextBtn, bNPCListBtn;	
	bPrevBtn = bTypeNorLuckyBtn = bTypeMiniLuckyBtn = bShopPrevBtn = bShopNextBtn = bNPCListBtn= FALSE;
	
	switch(m_nLuckyMechineStep)
	{	
	case LUCKY_MAINSTEP_SELECT_SHOP:
		{
			bShopPrevBtn = bShopNextBtn = TRUE;
			bPrevBtn = FALSE;
			bNPCListBtn = TRUE;
			CreateNPCListBtn();	// NPC버튼 생성
		}
		break;
	case LUCKY_MAINSTEP_SELECT_GAMING:
		{
			bPrevBtn = TRUE;
			LUCKY_MACHINE_OMI stSelLuckyMachine;	
			if(GetInfluenceOneShop(&stSelLuckyMachine))
			{
				bPrevBtn = FALSE;
			}
		}
		break;
	}
	{
		m_pPrevStepBtn->ShowWindow(bPrevBtn);
	}	
	
	// 상점 종류 
	{
		m_pSelectShopPrevBtn->ShowWindow(bShopPrevBtn);
		m_pSelectShopNextBtn->ShowWindow(bShopNextBtn);

		if(bShopPrevBtn && bShopNextBtn)
		{
			m_pSelectShopPrevBtn->EnableBtn(TRUE);
			m_pSelectShopNextBtn->EnableBtn(TRUE);
		}
		
	}
	if(bNPCListBtn)
	{
		if(!RefreshNPCListBtnPos())	// NPC버튼 위치 지정
		{
			// 한개라도 X가 있냐?			
			m_pSelectShopPrevBtn->EnableBtn(FALSE);
			m_pSelectShopNextBtn->EnableBtn(FALSE);

		}
	}
	else
	{
		ShowNPCListBtn(bNPCListBtn);
	}	
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPC표시
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::OnClickSelectNPCShop(LUCKY_MACHINE_OMI	*pSelLuckyMachine)
{		
	memcpy(&m_struSelLuckyMachine, pSelLuckyMachine, sizeof(LUCKY_MACHINE_OMI));

	if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > LUCKY_ITEM_LIST_NUM_LINE)
	{
		// 최대 슬롯수를 넘을수 없다.
		m_struSelLuckyMachine.MachineKind = LUCKY_ITEM_LIST_NUM_LINE;
	}

	g_pGameMain->ShowShopInven(BUILDINGKIND_LUCKY, TRUE);
	SetLuckyMechineStep(LUCKY_MAINSTEP_SELECT_GAMING);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::OnClickPrevStepBtn()
{
	switch(m_nLuckyMechineStep)
	{ 		
	case LUCKY_MAINSTEP_SELECT_GAMING:		// 럭키 머신 게임중
		{
			if(m_bPrizeItem)			// 받을것이 있다면 겜블은 되지 않는다.
			{
				return;
			}
			
			if(!IsCloseLucky())		
			{
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0101,COLOR_ERROR);
				return;
			}
			// 2009-05-11 by bhsohn 럭키 머신 뒤로 가기 키 버그 수정
			if(m_pCoinItemInfo)
			{					
				m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;
				
				CItemInfo* pCoinSelItem = g_pStoreData->FindItemInInventoryByUniqueNumber(m_pCoinItemInfo->UniqueNumber);
				if(pCoinSelItem && pCoinSelItem->ItemInfo)
				{
					// 이미 있으면 아이템 카운트 수를 더하자			
					g_pStoreData->UpdateItemCount(m_pCoinItemInfo->UniqueNumber, pCoinSelItem->CurrentCount + m_pCoinItemInfo->CurrentCount);
				}				
				else if(NULL == pCoinSelItem)
				{
					g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pCoinItemInfo), TRUE);			
				}				
				ClearConinItemInfo();
			}
			// end 2009-05-11 by bhsohn 럭키 머신 뒤로 가기 키 버그 수정
			// 인벤을 숨기자
			g_pGameMain->m_pInven->ShowInven(NULL, NULL, FALSE, TRUE);

			// 선택한 럭키 상점 초기화
			memset(&m_struSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));

			SetLuckyMechineStep(LUCKY_MAINSTEP_SELECT_SHOP);

			// 초기화
			ResetLuckyMachineInfo();
		}
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderLuckyMachineSlot(int nSlotNum)
{
	if(m_vecIconNum[nSlotNum].empty() || m_vecTakeItem.size() <= nSlotNum)
	{
		return;
	}	

	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;

	vector<ST_ICON_ITEM_NUMBER>::iterator it = m_vecIconNum[nSlotNum].begin();
	int nCnt = 0;
	for(nCnt = 0 ; nCnt < m_nSlotStartPosIdx[nSlotNum]; nCnt++)
	{
		if(it == m_vecIconNum[nSlotNum].end())
		{
			it = m_vecIconNum[nSlotNum].begin();
		}
		it++;
	}

	int nSlotPosX = 0;
	int nSlotPosY = 0;
	int nItemPos = 0;
	
	int nMaxItemSlotCnt = min(LUCKY_ITEM_LIST_NUM, m_vecIconNum[nSlotNum].size());

	int nSlotPosIdx = nSlotNum;
	if(LUCKY_SELECT_TYPE_MINI == CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind))
	{
		nSlotPosIdx = LUCKY_SELECT_TYPE_MINI;
	}

	for(nItemPos = 0 ; nItemPos < nMaxItemSlotCnt; nItemPos++)
	{
		if(it == m_vecIconNum[nSlotNum].end())
		{
			it = m_vecIconNum[nSlotNum].begin();
		}
		
		ST_ICON_ITEM_NUMBER stItemNum = (*it);				
		
		{
			char buf[20];
			
			wsprintf(buf, "%08d",stItemNum.nIconNum);
			pIcon->SetIcon( buf, LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos),
								LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx), 1.0f );
			pIcon->Render();
			// 2010. 05. 12 by jskim 신규 럭키 머신 추가방안 - 럭키머신에서 받는 아이템만 보여주기
			if(nItemPos != 3)
			{
				m_pImgDisableItem->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos), 
										LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx));

				m_pImgDisableItem->SetScale(pIcon->GetIconSize().x, pIcon->GetIconSize().y);
				m_pImgDisableItem->Render();	
			}
			else
			{
				if(m_nAME_EffectNum[nSlotNum] != 0.0f && m_nAME_EffectNum[nSlotNum] <= 0.5f)
				{
					m_pAME_slot[0]->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos) - 10,
						LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx) - 10);
					m_pAME_slot[0]->Render();
				}
				else if(m_nAME_EffectNum[nSlotNum] > 0.5f)
				{
					m_pAME_slot[1]->Move(LUCKYMACHINE_SLOT_X+(LUCKYMACHINE_SLOT_GAP_X*nItemPos)  - 10,
						LUCKYMACHINE_SLOT_Y+(LUCKYMACHINE_SLOT_GAP_Y*nSlotPosIdx) - 10);
					m_pAME_slot[1]->Render();
				}
			}
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsAllRuletDone()
{
	//int nRuletLen = m_struSelLuckyMachine.MachineKind;	// 최대 룰렛 수
	int nRuletLen = CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind);
	int nCnt = 0;
	for(nCnt = 0;nCnt < nRuletLen;nCnt++)
	{
		if(!m_bSlotDone[nCnt])
		{
			return FALSE;
		}
	}		
	return TRUE;	

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ResetLuckyMachineInfo()
{
	m_vecPrizeItem.clear();
	m_vecPrizeItemCount.clear();
	m_bPrizeItem = FALSE;
	Init();
	ChangeState(LUCKY_STATE_READY);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		올릴수 있는 코인이냐?
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsPossibleUpLoadCoin(ITEM* i_pItem)
{	
	if(!COMPARE_BIT_FLAG(i_pItem->ItemAttribute, ITEM_ATTR_LUCKY_ITEM))
	{
		// 럭키 상점 형이 아니다.
		//"\\y상점에서는 이용 할 수 없습니다."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080603_0100, COLOR_ERROR);
		return FALSE;
	}
	if(m_nState == LUCKY_STATE_RULLET)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081201_0101,COLOR_ERROR);
		return FALSE;
	}
	
	if(m_bPrizeItem)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102, COLOR_ERROR);
		return FALSE;
	}

	// 2009-04-14 by bhsohn 인벤 3개 미만일시, 럭키머신 시 버그 수정
	if(CAtumSJ::GetLuckyMachineSlotCount(m_struSelLuckyMachine.MachineKind) > g_pGameMain->m_pInven->GetInvenFreeSize())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081124_0101,COLOR_ERROR);
		return FALSE;
	}
	// end 2009-04-14 by bhsohn 인벤 3개 미만일시, 럭키머신 시 버그 수정

	int nMachineNum = m_struSelLuckyMachine.MachineNum;
	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
	int nCnt = 0;
	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
	{
		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
		if(NULL == pLuckyMachine)
		{
			break;
		}
		if((nMachineNum == pLuckyMachine->MachineNum)
			&& (i_pItem->ItemNum == pLuckyMachine->CoinItemNum ))
		{
			return TRUE;
		}

	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		올릴수 있는 코인이냐?
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::UpLoadCoin(ITEM_GENERAL* i_pItem)
{
	if(NULL == i_pItem)
	{
		return;
	}
	// LuckyMachine에서 쓰이는 아이템이면 								
	ClearConinItemInfo(); // 코인정보 초기화
	m_pCoinItemInfo = new CItemInfo(i_pItem);	
	
	// 전에는 한개만 올라갔다.
	m_pCoinItemInfo->CurrentCount = 1;								 	
	g_pStoreData->UpdateItemCount(i_pItem->UniqueNumber, i_pItem->CurrentCount - m_pCoinItemInfo->CurrentCount);

	// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	util::del(m_pItemCoinItemTmp);
	m_pItemCoinItemTmp = new ITEM_GENERAL;
	memcpy(m_pItemCoinItemTmp, i_pItem, sizeof(ITEM_GENERAL));
	m_pItemCoinItemTmp->CurrentCount = 1;
	// end 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	
	// 인벤에 있는건 다 지우자 
	//g_pStoreData->UpdateItemCount(i_pItem->UniqueNumber, 0);	
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_COIN, D3DXVECTOR3(0,0,0), FALSE);			

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤이 풀이라는 에러는 아이템 지급이 실패므로, 동전을 원래대로 돌리자
/// \author		// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
/// \date		2009-05-18 ~ 2009-05-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ErrorLuckyMechineInvenFull()
{
	if(NULL == m_pItemCoinItemTmp)
	{
		return;
	}
	g_pStoreData->PutItem((char*)((ITEM_GENERAL*)m_pItemCoinItemTmp), TRUE);	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		올릴수 있는 코인이냐?
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::UpdateSuccessSlot()
{
	int nSlotPos = 0;	
	int nItemPos = 0;
	for(nSlotPos = 0;nSlotPos < LUCKY_ITEM_LIST_NUM_LINE;nSlotPos++)
	{
		int nTakeGetItemNum = m_vecTakeItem[nSlotPos].Itemnum;
		m_nSuccessSlot[nSlotPos]	= 0;		// 성공 위치 없음
		nItemPos = 0;

		vector<ST_ICON_ITEM_NUMBER>::iterator itSearchItem = m_vecIconNum[nSlotPos].begin();
		while(itSearchItem != m_vecIconNum[nSlotPos].end())
		{
			ST_ICON_ITEM_NUMBER stIconNum = (*itSearchItem);
			if(stIconNum.nItemNum == nTakeGetItemNum)
			{				
				m_nSuccessSlot[nSlotPos] = nItemPos;
				break;
			}
			nItemPos++;
			itSearchItem++;
		}
	}
	// 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
	// 성공 위치를 갱신시, 기존 동전 정보 초기화
	util::del(m_pItemCoinItemTmp);	
	// end 2009-05-18 by bhsohn 럭키상점에서 인벤풀일떄의 아이템 처리
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		올릴수 있는 코인이냐?
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImage*	CINFLuckyMachine::GetShopImageBk(int nEffectBG)
{
	vector<structNPCList*>::iterator itNpcBtn = m_vecstruShopList.begin();
	while(itNpcBtn != m_vecstruShopList.end())
	{
		structNPCList* pstruNPCList = (*itNpcBtn);		
		
		if((m_struSelLuckyMachine.SourceIndex == pstruNPCList->struLuckyMachineOmi.SourceIndex )
			&& pstruNPCList->pInfShopImage[nEffectBG])
		{
			return 	pstruNPCList->pInfShopImage[nEffectBG];
		}		
		itNpcBtn++;
	}
	return NULL;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		강제로 위치 갱신
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::UpdateCurrentSlotPos(int i_nSlot)
{
	int nSlotTmpPos = (FIRST_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);
	switch(i_nSlot)
	{
	case 0:	
		{
			// 0번 슬롯
			nSlotTmpPos = (FIRST_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);			
		}
		break;
	case 1:
		{
			// 1번슬롯
			nSlotTmpPos = (SECOND_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);			
		}
		break;
	case 2:
		{
			// 2번슬롯
			nSlotTmpPos = (THIRD_SLOT_MACHINE_SUCCESS_STEP) + RANDI(0, LUCKY_ITEM_POS - 1);			
		}
		break;
	}
	switch(m_nSlotStep[i_nSlot])
	{
	case 1:
		{
			// 첫번째 스텝에 성공 위치를 지정한다.
			
			int nSlotItemMax = ((int)m_vecIconNum[i_nSlot].size()) - RANDI(0, LUCKY_ITEM_POS - 1);
			int nSlotPos = min(nSlotTmpPos, nSlotItemMax);
			m_nSlotStartPosIdx[i_nSlot] = m_nSuccessSlot[i_nSlot] - nSlotPos;
		}
		break;
	}
	//
	// 값을 정해줌
	if(m_nSlotStartPosIdx[i_nSlot] <0)
	{
		if(m_vecIconNum[i_nSlot].empty())
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}
		else
		{
			m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size()) + m_nSlotStartPosIdx[i_nSlot];
		}
	}
	else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
	{
		m_nSlotStartPosIdx[i_nSlot] = m_nSlotStartPosIdx[i_nSlot] - ((int)m_vecIconNum[i_nSlot].size());
	}	
	
	// 카운트값 에러 안나게 함
	if(m_nSlotStartPosIdx[i_nSlot] <0)
	{
		if(m_vecIconNum[i_nSlot].empty())
		{
			m_nSlotStartPosIdx[i_nSlot] = 0;
		}
		else
		{
			m_nSlotStartPosIdx[i_nSlot] = ((int)m_vecIconNum[i_nSlot].size());
		}
	}
	else if(((int)m_vecIconNum[i_nSlot].size()) < m_nSlotStartPosIdx[i_nSlot])
	{
		m_nSlotStartPosIdx[i_nSlot] = 0;
	}		

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		세력에 상점이 한개 있는지에 대한 처리
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::GetInfluenceOneShop(LUCKY_MACHINE_OMI* o_pSelLuckyMachine)
{
	int nLuckyMachineLen = g_pDatabase->GetLuckyMachineLen();
	int nMyBeligenceCnt = 0;
	
	LUCKY_MACHINE_OMI	stTmpSelLuckyMachine;
	memset(&stTmpSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));
	memset(o_pSelLuckyMachine, 0x00, sizeof(LUCKY_MACHINE_OMI));

	int nCnt = 0;	
	for(nCnt = 0;nCnt<nLuckyMachineLen;nCnt++)
	{
		LUCKY_MACHINE_OMI*  pLuckyMachine = g_pDatabase->GetLuckyMachineInfo(nCnt);
		if(NULL == pLuckyMachine)
		{
			continue;
		}
// 		if((pLuckyMachine->BuildingInx == stTmpSelLuckyMachine.BuildingInx)
// 			&&(pLuckyMachine->MachineNum == stTmpSelLuckyMachine.MachineNum))
// 		{
// 			continue;
// 		}
// 		else if(!IsMyBelignece(pLuckyMachine)) // 같은 세력이냐>?
// 		{			
// 			continue;
// 		}		
		if(IsMyBelignece(pLuckyMachine) && pLuckyMachine->BuildingInx == m_uBuildingIndex)
		{
			memcpy(o_pSelLuckyMachine, pLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
		return TRUE;
	}
//		memcpy(&o_pSelLuckyMachine, pLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
//		nMyBeligenceCnt++;
	}
// 	if(1 == nMyBeligenceCnt)
// 	{
// 		// 상점이 한개이다.
// 		memcpy(o_pSelLuckyMachine, &stTmpSelLuckyMachine, sizeof(LUCKY_MACHINE_OMI));
// 		return TRUE;
// 	}
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		//NPC표시
/// \author		// 2009-03-04 by bhsohn 럭키 머신 수정안
/// \date		2009-03-04 ~ 2009-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFLuckyMachine::IsInfluenceOneShopMode()
{
	return m_bInfluenceOneShopMode;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemInfo* CINFLuckyMachine::GetConinItemInfo()
{
	return m_pCoinItemInfo;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::ClearConinItemInfo()
{
	if(m_pCoinItemInfo)
	{
		m_pCoinItemInfo->ItemWindowIndex = POS_INVALID_POSITION;
		util::del(m_pCoinItemInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::TickYouLucky()
{	
	if(!m_bYouLucky)
	{
		return;
	}
	// 1시간이 넘으면 그떄서야 살려준다. 	
	m_fYouLuckyViewTime += g_pD3dApp->GetCheckElapsedTime();	
	
	// 8초동안 살아있고 지운다.
	if(m_fYouLuckyViewTime > MAX_YOU_LUCKY_TIME)
	{		
		m_fYouLuckyViewTime = 0.0f;
		m_dwYouLuckyAlpha = 0;		
		m_bYouLucky = FALSE;
		return;
	}	
	// 맵이름 사라지기..
	//if(m_fYouLuckyViewTime > MAX_YOU_DELAY_LUCKY_TIME)
	{
		m_dwYouLuckyAlpha = g_pGameMain->GetCurrentColor(m_fYouLuckyViewTime, 
			ALPHA_DELAY_WAVE, 
			MAX_YOU_LUCKY_TIME,
			MAX_YOU_DELAY_LUCKY_TIME);	
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFLuckyMachine::RenderYouLucky()
{		
	TickYouLucky();
	if(!m_bYouLucky)
	{
		return;
	}

	int nYouLucky = m_nLuckyEffectBG;
	if(nYouLucky >= MAX_LUCKY_YOULUCKY)
	{
		nYouLucky = 0;
	}
	DWORD dwYouLuckyAlpha = m_dwYouLuckyAlpha;
	if(m_fYouLuckyViewTime < MAX_YOU_DELAY_LUCKY_TIME)
	{
		dwYouLuckyAlpha = 0xFFFFFFFF;
	}
	else
	{
		nYouLucky = 0;
	}
	if(m_pImgYouLuckyBG[nYouLucky])
	{
		m_pImgYouLuckyBG[nYouLucky]->SetColor(dwYouLuckyAlpha);
		m_pImgYouLuckyBG[nYouLucky]->Move(LUCKYMACHINE_EFFECT_BG_X,0);
		m_pImgYouLuckyBG[nYouLucky]->Render();	
	}	
}