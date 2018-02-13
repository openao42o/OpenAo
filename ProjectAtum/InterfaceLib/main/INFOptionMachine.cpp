//; INFOptionMachine.cpp: implementation of the INFOptionMachine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "INFLuckyMachine.h"
#include "AtumDatabase.h"
#include "AtumApplication.h"
#include "INFIcon.h"
#include "ItemInfo.h"
#include "FieldWinSocket.h"
#include "INFCityBase.h"
#include "StoreData.h"
#include "Chat.h"
#include "INFInvenExtend.h"
#include "ShuttleChild.h"
#include "AtumSound.h"
#include "INFOptionMachine.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"
#include "INFWindow.h" 
#include "INFImageEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define OPTIONMACHINE_EFFECT_FRAME_CHANGE_TIME				0.5f
#define OPTIONMACHINE_EFFECT_IMAGE							2

#define OPTIONMACHINE_MAIN_BG_X								(CITY_BASE_NPC_BOX_START_X + 420)
#define OPTIONMACHINE_MAIN_BG_Y								(CITY_BASE_NPC_BOX_START_Y - 440)

#define	OPTIONMACHINE_EFFECT_BG_X							(CITY_BASE_NPC_BOX_START_X - 7 + 30)
#define OPTIONMACHINE_EFFECT_BG_Y							(OPTIONMACHINE_MAIN_BG_Y - 148 + 100)

#define	OPTIONMACHINE_PRE_FIX_X								(OPTIONMACHINE_MAIN_BG_X + 72 + 30)
#define	OPTIONMACHINE_PRE_FIX_Y								(OPTIONMACHINE_MAIN_BG_Y + 184 + 92)

#define	OPTIONMACHINE_PRE_ARROW_X							(OPTIONMACHINE_PRE_FIX_X + 32 + 30)
#define	OPTIONMACHINE_PRE_ARROW_Y							(OPTIONMACHINE_PRE_FIX_Y - 5 + 100)

#define	OPTIONMACHINE_SUF_FIX_X								(OPTIONMACHINE_MAIN_BG_X + 207 + 30)
#define	OPTIONMACHINE_SUF_FIX_Y								(OPTIONMACHINE_MAIN_BG_Y + 184 + 92) 

#define	OPTIONMACHINE_SUF_ARROW_X							(OPTIONMACHINE_SUF_FIX_X - 35 + 30)
#define	OPTIONMACHINE_SUF_ARROW_Y							(OPTIONMACHINE_SUF_FIX_Y - 5 + 100)

#define	OPTIONMACHINE_TARGET_ITEM_X							(OPTIONMACHINE_MAIN_BG_X + 140 + 30)
#define	OPTIONMACHINE_TARGET_ITEM_Y							(OPTIONMACHINE_MAIN_BG_Y + 184 + 127)

#define OPTIONMACHINE_CENTER_SLOT							3

#define OPTIONMACHINE_CARD_SIZE_W							30
#define OPTIONMACHINE_CARD_SIZE_H							30

#define OPTIONMACHINE_GAMBLE_BUTTON_X						(OPTIONMACHINE_MAIN_BG_X + 198)
#define OPTIONMACHINE_GAMBLE_BUTTON_Y						(OPTIONMACHINE_MAIN_BG_Y + 366)

#define OPTIONMACHINE_TAKE_BUTTON_X							(OPTIONMACHINE_MAIN_BG_X + 141)
#define OPTIONMACHINE_TAKE_BUTTON_Y							(OPTIONMACHINE_MAIN_BG_Y + 366)

#define OPTIONMACHINE_SLOT_X								(OPTIONMACHINE_MAIN_BG_X + 38 + 30)
#define OPTIONMACHINE_SLOT_Y								(OPTIONMACHINE_MAIN_BG_Y + 45 + 90)
#define OPTIONMACHINE_SLOT_GAP_X							136
#define OPTIONMACHINE_SLOT_GAP_Y							25

#define OPTIONMACHINE_SLOT_PRE_CUR_X						(OPTIONMACHINE_MAIN_BG_X + 31 + 36)
#define OPTIONMACHINE_SLOT_PRE_CUR_Y						(OPTIONMACHINE_MAIN_BG_Y + 96 + 89)

#define OPTIONMACHINE_SLOT_SUF_CUR_X						(OPTIONMACHINE_MAIN_BG_X + OPTIONMACHINE_SLOT_GAP_X + 31 + 36)
#define OPTIONMACHINE_SLOT_SUF_CUR_Y						(OPTIONMACHINE_MAIN_BG_Y + 96 + 89)



#define OPTIONMACHINE_CHANGE_PATTERN_TIME					2
#define OPTIONMACHINE_CHANGE_SLOW_TIME						1

#define OPTIONMACHINE_SLOT_READY_QUESTION					"?"

CINFOptionMachine::CINFOptionMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent		= pParent;
	m_pGameData		= NULL;
	
	// 이미지
	m_pImgOptionBG				= NULL;
	
	m_pImgSlotPreQuestion		= NULL;
	m_pImgSlotSufQuestion		= NULL;

	int i;
	for(i=0; i <7; i++)
	{
		m_pImgOptionEffBG[i]	= NULL;
	}
	for(i=0; i< 2; i++)
	{
		m_pImgSlotPreBG[2] = NULL;	
		m_pImgSlotSufBG[2] = NULL;
	}
	m_pImgPrizePrePositionCur	= NULL;
	m_pImgPrizeSufPositionCur	= NULL;

	// 버튼
	m_pBtnStart		= NULL;
	m_pBtnTake		= NULL;

	m_nEffectIdx	= NULL;
	m_fTimeEffectBG = NULL;

	m_IsShopMode	= FALSE;

	m_pOptionFont	= NULL;

	m_nState		= OPTION_STATE_READY;

	m_fTrickTime	= TRICK_SPEED;

	for(i=0; i < OPTION_ITEM_LIST_FIX; i++)
	{
		m_SlotInfo[i] = NULL;
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_IsArrow[i] = NULL;
		m_isArrowCnt[i] = NULL;
		m_pAME_slot[i] = NULL;
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	}
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pImgSlotDark = NULL;

	for(i=0; i < 3; i++)
	{
		m_pImgPreArrow[i] = NULL;
		m_pImgSufArrow[i] = NULL;
	}	

	m_isEffect = FALSE;
	m_bisInit = NULL;
	
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
}

CINFOptionMachine::~CINFOptionMachine()
{
}

HRESULT CINFOptionMachine::InitDeviceObjects()
{
	DataHeader	* pDataHeader;
	
	// 이미지
	int i;
	for(i=0; i< 4; i++)
	{
		char temp[256];
		m_pImgOptionEffBG[i] = new CINFImageEx; 
		wsprintf(temp,"OME_BG_%d",i+1);		
		pDataHeader = FindResource(temp);
		m_pImgOptionEffBG[i]->InitDeviceObjects(pDataHeader);
	}
	for(i=0; i< 2; i++)
	{
		char temp[256];
		m_pImgOptionEffBG[i+4] = new CINFImageEx; 
		wsprintf(temp,"OME_BG_%d",i+3);		
		pDataHeader = FindResource(temp);
		m_pImgOptionEffBG[i+4]->InitDeviceObjects(pDataHeader);
		// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_pAME_slot[i] = new CINFImageEx;
		wsprintf(temp,"AME_slot%d",i);
		pDataHeader = FindResource(temp);
		m_pAME_slot[i]->InitDeviceObjects(pDataHeader);
		//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조

	}

	char temp1[256];
	m_pImgOptionEffBG[6] = new CINFImageEx; 
	wsprintf(temp1,"OME_BG_%d",5);
	pDataHeader = FindResource(temp1);
		m_pImgOptionEffBG[6]->InitDeviceObjects(pDataHeader);

	m_pImgOptionBG			= new CINFImageEx;
	//pDataHeader = FindResource("OM_BG");
	pDataHeader = FindResource("OME_BG_0");
	m_pImgOptionBG->InitDeviceObjects(pDataHeader);
	m_pImgOptionBG->SetUVAnimation(4,2,0.07f);
	m_pImgOptionBG->SetLoop(TRUE);

	m_pImgSlotPreQuestion	= new CINFImageEx;
	pDataHeader = FindResource("SlotPre_Q");
	m_pImgSlotPreQuestion->InitDeviceObjects(pDataHeader);
		
	m_pImgSlotSufQuestion	= new CINFImageEx;
	pDataHeader = FindResource("SlotSuf_Q");
	m_pImgSlotSufQuestion->InitDeviceObjects(pDataHeader);

	for(i=1; i< 3; i++)
	{
		m_pImgSlotPreBG[i-1]	= new CINFImageEx;
		char temp[256];
		wsprintf(temp,"SlotPre_%d",i);
		pDataHeader = FindResource(temp);
		m_pImgSlotPreBG[i-1]->InitDeviceObjects(pDataHeader);

		m_pImgSlotSufBG[i-1]	= new CINFImageEx;
		wsprintf(temp,"SlotSuf_%d",i);
		pDataHeader = FindResource(temp);
		m_pImgSlotSufBG[i-1]->InitDeviceObjects(pDataHeader);
	}
	m_pImgPrizePrePositionCur	= new CINFImageEx;
	pDataHeader = FindResource("PrizePCur");
	m_pImgPrizePrePositionCur->InitDeviceObjects(pDataHeader);
	
	m_pImgPrizeSufPositionCur	= new CINFImageEx;
	pDataHeader = FindResource("PrizeSCur");
	m_pImgPrizeSufPositionCur->InitDeviceObjects(pDataHeader);

	// 버튼 
	m_pBtnStart = new CINFImageBtn;
	m_pBtnStart->InitDeviceObjects("GameStart","STRTOOLTIP110");

	m_pBtnTake	= new CINFImageBtn;
	m_pBtnTake->InitDeviceObjects("PrizeItem","STRTOOLTIP108");

	m_pOptionFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,1024,32);
	m_pOptionFont->InitDeviceObjects(g_pD3dDev);
	
	m_IsShopMode = TRUE;
	m_nState = FALSE;

	for(i=0; i < OPTION_SELECT_ITEM; i++)
	{
		m_pSelectItemInfo[i] = NULL;
	}
	

	for(i=0; i < OPTION_ITEM_LIST_FIX; i++)
	{
		if(!m_SlotInfo[i])
			m_SlotInfo[i] = new ST_SLOT_INFO;
	}

	ST_RENDER_ITEM* temp = new ST_RENDER_ITEM;
	temp->nItemName = OPTIONMACHINE_SLOT_READY_QUESTION;
	temp->nItemPosition = 0;
	for(i=0; i< OPTION_ITEM_LIST_NUM; i++)
	{
		m_RenderPreOption.push_back(temp);
		m_RenderSufOption.push_back(temp);
	}
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조	
	m_pImgSlotDark = new CINFImageEx;
	pDataHeader = FindResource("Slot_dark");
	m_pImgSlotDark->InitDeviceObjects(pDataHeader);

	for(i=1; i< 4; i++)
	{
		char temp[256];
		m_pImgPreArrow[i-1]	= new CINFImageEx;
		wsprintf(temp,"OME_Pre%d",i);
		pDataHeader = FindResource(temp);
		m_pImgPreArrow[i-1]->InitDeviceObjects(pDataHeader);
		
		m_pImgSufArrow[i-1]	= new CINFImageEx;
		wsprintf(temp,"OME_Suf%d",i);
		pDataHeader = FindResource(temp);
		m_pImgSufArrow[i-1]->InitDeviceObjects(pDataHeader);	
	}
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	return S_OK;
}

HRESULT CINFOptionMachine::RestoreDeviceObjects()
{
	int i;
	for(i=0; i< 7; i++)
	{
		m_pImgOptionEffBG[i]->RestoreDeviceObjects();
	}

	m_pImgOptionBG->RestoreDeviceObjects();
	for(i = 0; i<2; i++)
	{	
		m_pImgSlotPreBG[i]->RestoreDeviceObjects();
		m_pImgSlotSufBG[i]->RestoreDeviceObjects();
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_pAME_slot[i]->RestoreDeviceObjects();
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	}

	m_pImgSlotPreQuestion->RestoreDeviceObjects();
	m_pImgSlotSufQuestion->RestoreDeviceObjects();
	
	m_pImgPrizePrePositionCur->RestoreDeviceObjects();
	m_pImgPrizeSufPositionCur->RestoreDeviceObjects();

	m_pBtnStart->RestoreDeviceObjects();
	m_pBtnStart->SetBtnPosition(OPTIONMACHINE_GAMBLE_BUTTON_X, OPTIONMACHINE_GAMBLE_BUTTON_Y);
	m_pBtnTake->RestoreDeviceObjects();
	m_pBtnTake->SetBtnPosition(OPTIONMACHINE_TAKE_BUTTON_X, OPTIONMACHINE_TAKE_BUTTON_Y);

	if(m_pOptionFont)
	{
		m_pOptionFont->RestoreDeviceObjects();
	}
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pImgSlotDark->RestoreDeviceObjects();

	for(i=1; i< 4; i++)
	{
		m_pImgPreArrow[i-1]->RestoreDeviceObjects();
		m_pImgSufArrow[i-1]->RestoreDeviceObjects();		
	}
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	return S_OK;
}

HRESULT CINFOptionMachine::DeleteDeviceObjects()
{
	int i;
	for(i=0; i< 7; i++)
	{
		m_pImgOptionEffBG[i]->DeleteDeviceObjects();
		util::del(m_pImgOptionEffBG[i]);

	}

	m_pImgOptionBG->DeleteDeviceObjects();
	util::del(m_pImgOptionBG);

	for(i = 0; i<2; i++)
	{	
		m_pImgSlotPreBG[i]->DeleteDeviceObjects();
		util::del(m_pImgSlotPreBG[i]);
		m_pImgSlotSufBG[i]->DeleteDeviceObjects();
		util::del(m_pImgSlotSufBG[i]);
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_pAME_slot[i]->DeleteDeviceObjects();
		util::del(m_pAME_slot[i]);
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	}

	m_pImgSlotPreQuestion->DeleteDeviceObjects();
	util::del(m_pImgSlotPreQuestion);

	m_pImgSlotSufQuestion->DeleteDeviceObjects();
	util::del(m_pImgSlotSufQuestion);
	
	m_pImgPrizePrePositionCur->DeleteDeviceObjects();
	util::del(m_pImgPrizePrePositionCur);

	m_pImgPrizeSufPositionCur->DeleteDeviceObjects();
	util::del(m_pImgPrizeSufPositionCur);

// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pImgSlotDark->DeleteDeviceObjects();
	util::del(m_pImgSlotDark);

	for(i=1; i< 4; i++)
	{
		m_pImgPreArrow[i-1]->DeleteDeviceObjects();
		m_pImgSufArrow[i-1]->DeleteDeviceObjects();
		util::del(m_pImgPreArrow[i-1]);
		util::del(m_pImgSufArrow[i-1]);
	}
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	if(m_pOptionFont)
	{
		util::del(m_pOptionFont);
	}

	for(i=0; i<2; i++)
		util::del(m_SlotInfo[i]);



	m_RenderPreOption.clear();
	m_RenderSufOption.clear();

	


	return S_OK;
}

HRESULT CINFOptionMachine::InvalidateDeviceObjects()
{
	int i;
	for(i=0; i< 7; i++)
	{
		m_pImgOptionEffBG[i]->InvalidateDeviceObjects();
	}

	m_pImgOptionBG->InvalidateDeviceObjects();

	for( i = 0; i<2; i++)
	{	
		m_pImgSlotPreBG[i]->InvalidateDeviceObjects();
		m_pImgSlotSufBG[i]->InvalidateDeviceObjects();
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_pAME_slot[i]->InvalidateDeviceObjects();
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	}

	m_pImgSlotPreQuestion->InvalidateDeviceObjects();
	m_pImgSlotSufQuestion->InvalidateDeviceObjects();
	
	m_pImgPrizePrePositionCur->InvalidateDeviceObjects();
	m_pImgPrizeSufPositionCur->InvalidateDeviceObjects();
	
	m_pBtnStart->InvalidateDeviceObjects();
	m_pBtnTake->InvalidateDeviceObjects();
		
	if(m_pOptionFont)
	{
		m_pOptionFont->InvalidateDeviceObjects();
	}
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	m_pImgSlotDark->InvalidateDeviceObjects();

	for(i=1; i< 4; i++)
	{
		m_pImgPreArrow[i-1]->InvalidateDeviceObjects();
		m_pImgSufArrow[i-1]->InvalidateDeviceObjects();		
	}
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	return S_OK;
}

void CINFOptionMachine::Render()
{	

//	m_pImgOptionEffBG[m_nEffectIdx]->Move(OPTIONMACHINE_EFFECT_BG_X, OPTIONMACHINE_EFFECT_BG_Y);
//	m_pImgOptionEffBG[m_nEffectIdx]->Render();

	m_pImgOptionBG->Move(OPTIONMACHINE_MAIN_BG_X, OPTIONMACHINE_MAIN_BG_Y);
	m_pImgOptionBG->Render();

	m_pBtnStart->Render();
	m_pBtnTake->Render();

	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	RenderSlot();
	
	for(int i=0; i < OPTION_SELECT_ITEM; i++)
	{
		RenderCard(m_pSelectItemInfo[i], i);
	}
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	if(m_IsArrow[OPTION_ITEM_PRE_FIX])
	{
		m_pImgPreArrow[m_isArrowCnt[OPTION_ITEM_PRE_FIX]]->Move(OPTIONMACHINE_PRE_ARROW_X,OPTIONMACHINE_PRE_ARROW_Y);
//		m_pImgPreArrow[m_isArrowCnt[OPTION_ITEM_PRE_FIX]]->Render();
	}	
	if(m_IsArrow[OPTION_ITEM_SUF_FIX])
	{
		m_pImgSufArrow[m_isArrowCnt[OPTION_ITEM_SUF_FIX]]->Move(OPTIONMACHINE_SUF_ARROW_X,OPTIONMACHINE_SUF_ARROW_Y);
//		m_pImgSufArrow[m_isArrowCnt[OPTION_ITEM_SUF_FIX]]->Render();
	}
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
}

void CINFOptionMachine::RenderCard(CItemInfo* nSelectItem, int nPosition)
{
	if(!nSelectItem)
	{
		return;
	}
	if(nSelectItem != NULL && 
		nSelectItem->ItemInfo->SourceIndex != NULL)
	{
		// 아머
		CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
		CItemInfo* pItem = nSelectItem;
		char buf[20];
		wsprintf(buf, "%08d",pItem->ItemInfo->SourceIndex);
		switch(nPosition)
		{
			case OPTION_ITEM_PRE_FIX :
				{
					pIcon->SetIcon( buf, OPTIONMACHINE_PRE_FIX_X,OPTIONMACHINE_PRE_FIX_Y, 1.0f );
				}
				break;
			case OPTION_ITEM_SUF_FIX:
				{
					pIcon->SetIcon( buf, OPTIONMACHINE_SUF_FIX_X,OPTIONMACHINE_SUF_FIX_Y, 1.0f );
				}
				break;
			case OPTION_TARGET_ITEM	:
				{
					pIcon->SetIcon( buf, OPTIONMACHINE_TARGET_ITEM_X,OPTIONMACHINE_TARGET_ITEM_Y, 1.0f );
				}
				break;
			default:
				return;
		}
		pIcon->Render();
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조);
		if(IsAllRuletDone())
		{
			m_pAME_slot[m_isEffect]->Move(OPTIONMACHINE_TARGET_ITEM_X-10, OPTIONMACHINE_TARGET_ITEM_Y- 10);
			m_pAME_slot[m_isEffect]->Render();
		}
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	}
}

void CINFOptionMachine::RenderSlot()
{
	
	for(int i=0; i < OPTION_ITEM_LIST_NUM; i++)
	{
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		BOOL temp = FALSE;
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		if(m_RenderPreOption[i])
		{
			// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			if(m_SlotInfo[OPTION_ITEM_PRE_FIX]->bCompleteStep && i == 2)
			{
				temp = FALSE;
			}
			else if(m_SlotInfo[OPTION_ITEM_PRE_FIX]->bCompleteStep && i != 2)
			{
				temp = TRUE;
			}
			//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			OptionSlotDraw(OPTIONMACHINE_SLOT_X,
				OPTIONMACHINE_SLOT_Y+(OPTIONMACHINE_SLOT_GAP_Y*i),
				m_RenderPreOption[i], OPTION_ITEM_PRE_FIX, temp);
		}
		// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		temp = FALSE;
		//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		if(m_RenderSufOption[i])
		{
		// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			if(m_SlotInfo[OPTION_ITEM_SUF_FIX]->bCompleteStep && i == 2)
			{
				temp = FALSE;
			}
			else if(m_SlotInfo[OPTION_ITEM_SUF_FIX]->bCompleteStep && i != 2)
			{
				temp = TRUE;
			}
		//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			OptionSlotDraw(OPTIONMACHINE_SLOT_X + OPTIONMACHINE_SLOT_GAP_X,
				OPTIONMACHINE_SLOT_Y+(OPTIONMACHINE_SLOT_GAP_Y*i),
				m_RenderSufOption[i], OPTION_ITEM_SUF_FIX, temp);
		}
	}		
	m_pImgPrizePrePositionCur->Move(OPTIONMACHINE_SLOT_PRE_CUR_X, OPTIONMACHINE_SLOT_PRE_CUR_Y);
	m_pImgPrizePrePositionCur->Render();
	
	m_pImgPrizeSufPositionCur->Move(OPTIONMACHINE_SLOT_SUF_CUR_X, OPTIONMACHINE_SLOT_SUF_CUR_Y);
	m_pImgPrizeSufPositionCur->Render();
}

void CINFOptionMachine::Tick()
{
	// 이팩트 보여주기
	FLOAT fElapsedTime = g_pD3dApp->GetElapsedTime();
	if(m_fTimeEffectBG > OPTIONMACHINE_EFFECT_FRAME_CHANGE_TIME)
	{
		m_nEffectIdx += 1;	
		m_fTimeEffectBG = 0.0f;
	
// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조		
		if(m_nState == OPTION_STATE_RULLET)
		{
			if(m_IsArrow[OPTION_ITEM_PRE_FIX])
				m_isArrowCnt[OPTION_ITEM_PRE_FIX]++;

			if(m_isArrowCnt[OPTION_ITEM_PRE_FIX] > 2)
			{
				m_isArrowCnt[OPTION_ITEM_PRE_FIX] = 0;
			}
			
			if(m_IsArrow[OPTION_ITEM_SUF_FIX])
				m_isArrowCnt[OPTION_ITEM_SUF_FIX]++;

			if(m_isArrowCnt[OPTION_ITEM_SUF_FIX] > 2)
			{
				m_isArrowCnt[OPTION_ITEM_SUF_FIX] = 0;
			}			
		}
		m_isEffect = !m_isEffect;
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	}
	else
	{
		m_fTimeEffectBG += fElapsedTime;
	}
	if(m_nEffectIdx > 6)
		m_nEffectIdx = 0;

// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	for(int i=0; i< 2; i++)
	{
		if(m_SlotInfo[i]->bCompleteStep == TRUE)
		{
			m_isArrowCnt[i] = 2;
		}
	}
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	
	m_pImgOptionBG->Tick();
	if(m_nState == OPTION_STATE_READY)
	{
		UpDataInfo();
	// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
		{
			m_IsArrow[OPTION_ITEM_PRE_FIX] = TRUE;
			m_isArrowCnt[OPTION_ITEM_PRE_FIX] = 2;
		}
		if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
		{
			m_IsArrow[OPTION_ITEM_SUF_FIX] = TRUE;
			m_isArrowCnt[OPTION_ITEM_SUF_FIX] = 2;
		}
	}
	//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
	
	if(m_nState == OPTION_STATE_RULLET)
	{
		TickSlot(fElapsedTime);
		if(IsAllRuletDone())
 		{
 			ChangeState(OPTION_STATE_PRIZE);
 		}
	}
}

float CINFOptionMachine::GetStepSpeed(int Step)
{
	float tempSpeed;
	if(Step > 0)
	{
		tempSpeed =(float)(( MAX_SLOT_SPEED_STEP - ( Step - 1 ) )  * STEP_SPEED);
		return tempSpeed;
	}
	else
	{
		return 0;
	}
}

void CINFOptionMachine::OptionSlotDraw(int x, int y, ST_RENDER_ITEM* RenderItem, int nPosition, BOOL slotDark)
{
	SIZE size = m_pOptionFont->GetStringSize(RenderItem->nItemName);
	if(nPosition == OPTION_ITEM_PRE_FIX)
	{
		if(!strcmp(RenderItem->nItemName, OPTIONMACHINE_SLOT_READY_QUESTION))
		{
			m_pImgSlotPreQuestion->Move(x,y);
			m_pImgSlotPreQuestion->Render();		
		}
		else
		{
			if(RenderItem->nImageType == 1 || m_nState != OPTION_STATE_RULLET)
			{
				m_pImgSlotPreBG[1]->Move(x,y);
				m_pImgSlotPreBG[1]->Render();
			}
			else
			{
				m_pImgSlotPreBG[0]->Move(x,y);
				m_pImgSlotPreBG[0]->Render();
			}
			m_pOptionFont->DrawText( (x + (m_pImgSlotPreBG[RenderItem->nImageType]->GetImgSize().x - size.cx) / 2) - 5,
								 	 (y + (m_pImgSlotPreBG[RenderItem->nImageType]->GetImgSize().y - size.cy) / 2) - 2,
									 D3DCOLOR_ARGB(0,255,255,255), RenderItem->nItemName);
			// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조			
			if(slotDark)
			{
				m_pImgSlotDark->Move(x,y);
				m_pImgSlotDark->Render();
			}
			//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		}
	}
	
	if(nPosition == OPTION_ITEM_SUF_FIX)
	{
		if(!strcmp(RenderItem->nItemName, OPTIONMACHINE_SLOT_READY_QUESTION))
		{
			m_pImgSlotSufQuestion->Move(x,y);
			m_pImgSlotSufQuestion->Render();	
		}
		else
		{
			if(RenderItem->nImageType == 1 || m_nState != OPTION_STATE_RULLET)
			{
				m_pImgSlotSufBG[1]->Move(x,y);
				m_pImgSlotSufBG[1]->Render();
			}
			else
			{
				m_pImgSlotSufBG[0]->Move(x,y);
				m_pImgSlotSufBG[0]->Render();
			}
			m_pOptionFont->DrawText( (x + (m_pImgSlotSufBG[RenderItem->nImageType]->GetImgSize().x - size.cx) / 2) - 5,
								 	 (y + (m_pImgSlotSufBG[RenderItem->nImageType]->GetImgSize().y - size.cy) / 2) - 2,
									 D3DCOLOR_ARGB(0,255,255,255), RenderItem->nItemName);
			// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			if(slotDark)
			{
				m_pImgSlotDark->Move(x,y);
				m_pImgSlotDark->Render();
			}
			//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		}
	}
}

int CINFOptionMachine::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			if(m_pBtnTake->OnMouseMove(pt))
			{
				return INF_MSGPROC_BREAK;
			}			
			if(m_pBtnStart->OnMouseMove(pt))
			{
				return INF_MSGPROC_BREAK;
			}

			if(m_nState == OPTION_STATE_READY || m_nState == OPTION_STATE_PRIZE)
			{
				if(pt.x > OPTIONMACHINE_PRE_FIX_X &&
					pt.x < OPTIONMACHINE_PRE_FIX_X + OPTIONMACHINE_CARD_SIZE_W &&
					pt.y > OPTIONMACHINE_PRE_FIX_Y &&
					pt.y < OPTIONMACHINE_PRE_FIX_Y + OPTIONMACHINE_CARD_SIZE_H)
				{
					// 접두
					if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX])
					{
						g_pGameMain->SetItemInfo1(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX],
							m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->ItemNum, pt.x, pt.y, 0, FALSE); 
					}
					else
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_100421_0412);
					}
				}
				else if(pt.x > OPTIONMACHINE_SUF_FIX_X &&
					pt.x < OPTIONMACHINE_SUF_FIX_X + OPTIONMACHINE_CARD_SIZE_W &&
					pt.y > OPTIONMACHINE_SUF_FIX_Y &&
					pt.y < OPTIONMACHINE_SUF_FIX_Y + OPTIONMACHINE_CARD_SIZE_H)
				{
					// 접미
					if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX])
					{
						g_pGameMain->SetItemInfo1(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX],
							m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->ItemNum, pt.x, pt.y, 0, FALSE); 
					}
					else
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_100421_0413);
					}
				}
				else if(pt.x > OPTIONMACHINE_TARGET_ITEM_X &&
					pt.x < OPTIONMACHINE_TARGET_ITEM_X + OPTIONMACHINE_CARD_SIZE_W &&
					pt.y > OPTIONMACHINE_TARGET_ITEM_Y &&
					pt.y < OPTIONMACHINE_TARGET_ITEM_Y + OPTIONMACHINE_CARD_SIZE_H)
				{
					// 아머, 웨폰
					if(m_pSelectItemInfo[OPTION_TARGET_ITEM])
					{
						g_pGameMain->SetItemInfo1(m_pSelectItemInfo[OPTION_TARGET_ITEM],
						m_pSelectItemInfo[OPTION_TARGET_ITEM]->ItemNum, pt.x, pt.y, 0, FALSE); 
					}
					else
					{
						g_pGameMain->SetToolTip(pt.x,pt.y,STRMSG_C_100421_0414);
					}
				}
				else
				{
					g_pGameMain->SetItemInfo(0,0,0,0);
				}

			}
			return INF_MSGPROC_BREAK;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(m_pBtnTake->OnLButtonDown(pt))
			{
				return INF_MSGPROC_BREAK;
			}			
			if(m_pBtnStart->OnLButtonDown(pt))
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

			if(m_pBtnTake->OnLButtonUp(pt))
			{ 
				PrizeItemGiveMe();
				return INF_MSGPROC_BREAK;
			}
			
			if(m_pBtnStart->OnLButtonUp(pt))
			{
				if(!m_SlotInfo)
				{
					return INF_MSGPROC_BREAK;
				}
				if(m_nState == OPTION_STATE_READY)
				{
					GambleButtonOK();
				}
				return INF_MSGPROC_BREAK;
			}
			//아이템 올려 놓는 지점
			// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 아이템 올려 놓는 구조 변경
// 			if(pt.x > OPTIONMACHINE_PRE_FIX_X &&
// 				pt.x < OPTIONMACHINE_PRE_FIX_X + OPTIONMACHINE_CARD_SIZE_W &&
// 				pt.y > OPTIONMACHINE_PRE_FIX_Y &&
// 				pt.y < OPTIONMACHINE_PRE_FIX_Y + OPTIONMACHINE_CARD_SIZE_H &&
// 				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
// 				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
// 			{
// 				// 접두
// 				UsingItemLoad(OPTION_ITEM_PRE_FIX);
// 			}
// 			if(pt.x > OPTIONMACHINE_SUF_FIX_X &&
// 				pt.x < OPTIONMACHINE_SUF_FIX_X + OPTIONMACHINE_CARD_SIZE_W &&
// 				pt.y > OPTIONMACHINE_SUF_FIX_Y &&
// 				pt.y < OPTIONMACHINE_SUF_FIX_Y + OPTIONMACHINE_CARD_SIZE_H &&
// 				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
// 				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
// 			{
// 				// 접미
// 				UsingItemLoad(OPTION_ITEM_SUF_FIX);
// 			}
// 			if(pt.x > OPTIONMACHINE_TARGET_ITEM_X &&
// 				pt.x < OPTIONMACHINE_TARGET_ITEM_X + OPTIONMACHINE_CARD_SIZE_W &&
// 				pt.y > OPTIONMACHINE_TARGET_ITEM_Y &&
// 				pt.y < OPTIONMACHINE_TARGET_ITEM_Y + OPTIONMACHINE_CARD_SIZE_H &&
// 				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
// 				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
// 			{
// 				// 아머, 웨폰
// 				UsingItemLoad(OPTION_TARGET_ITEM);
// 			}
			if(pt.x > OPTIONMACHINE_MAIN_BG_X &&
				pt.x < OPTIONMACHINE_MAIN_BG_X + m_pImgOptionBG->GetImgSize().x &&
				pt.y > OPTIONMACHINE_MAIN_BG_Y &&
				pt.y < OPTIONMACHINE_MAIN_BG_Y + m_pImgOptionBG->GetImgSize().y &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS)
			{
				UsingItemLoad();
			}
			//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 아이템 올려 놓는 구조 변경
		}	
		break;
		
	}
	return INF_MSGPROC_NORMAL;
}

BOOL CINFOptionMachine::IsAllRuletDone()
{
	if(m_SlotInfo[OPTION_ITEM_PRE_FIX]->bCompleteStep == TRUE && m_SlotInfo[OPTION_ITEM_SUF_FIX]->bCompleteStep == TRUE)
	{
		return TRUE;
	}	
	return FALSE;
}

void CINFOptionMachine::ChangeState(int i_nState)
{
	switch(i_nState)
	{
	case OPTION_STATE_READY:
		{
			ResetInfo();
			m_nState = OPTION_STATE_READY;
		}
		break;
	case OPTION_STATE_RULLET:
		{
			for(int i=0; i< OPTION_SELECT_ITEM - 1; i++)
			{
				if(m_pSelectItemInfo[i])
				{
					m_pSelectItemInfo[i]->ItemWindowIndex = POS_INVALID_POSITION;
					util::del(m_pSelectItemInfo[i]);
				}
				else
					continue;
			}
			m_nState = OPTION_STATE_RULLET;			
		}
		break;
	case OPTION_STATE_PRIZE:
		{
			m_nState = OPTION_STATE_PRIZE;

// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			for(int i=0; i< 2; i++)
			{
				m_isArrowCnt[i] = 2;
			}			
			if(m_bisInit == 1)
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_100421_0415, _MESSAGE);		//"레어 옵션이 초기화 되었습니다."
			if(m_bisInit == 2)
				g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_100421_0416, _MESSAGE);		//"레어 옵션이 생성 되었습니다."
//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
			DeleteTargetItem(m_pSelectItemInfo[OPTION_TARGET_ITEM]->UniqueNumber);
			m_pSelectItemInfo[OPTION_TARGET_ITEM] = m_pPrizeItem;
			g_pD3dApp->m_pSound->StopD3DSound(SOUND_LUCKY_SLOT);
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_081202_0102,COLOR_ERROR);//"아이템이 결정 되었습니다. ""아이템 받기""버튼을 눌러서 아이템을 가지고 가세요"
		}
		break;
	}	
}
void CINFOptionMachine::UpDataInfo()
{
	if((m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM]) ||
		(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM]))
	{
 		if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] && !m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE))
 		{
			if(!m_vecItemInfo[OPTION_ITEM_PRE_FIX].empty())
			{
				m_RenderPreOption.clear();
				for(int i=0; i< OPTION_ITEM_LIST_NUM; i++)
	 			{
					ST_RENDER_ITEM* temp = new ST_RENDER_ITEM;
					temp->nItemName = m_vecItemInfo[OPTION_ITEM_PRE_FIX][i]->nItemName;
 					temp->nItemPosition = m_vecItemInfo[OPTION_ITEM_PRE_FIX][i]->nItemPosition;
					temp->nImageType = m_vecItemInfo[OPTION_ITEM_PRE_FIX][i]->nItemPosition%2;
					m_RenderPreOption.push_back(temp);
 				}
			}
 		}
		if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] && !m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
		{
			if(!m_vecItemInfo[OPTION_ITEM_SUF_FIX].empty())
			{
				m_RenderSufOption.clear();
				for(int i=0; i< OPTION_ITEM_LIST_NUM; i++)
				{
					ST_RENDER_ITEM* temp = new ST_RENDER_ITEM;
					temp->nItemName = m_vecItemInfo[OPTION_ITEM_SUF_FIX][i]->nItemName;
					temp->nItemPosition = m_vecItemInfo[OPTION_ITEM_SUF_FIX][i]->nItemPosition;
					temp->nImageType = m_vecItemInfo[OPTION_ITEM_SUF_FIX][i]->nItemPosition%2;
					m_RenderSufOption.push_back(temp);
				}
			}
 		}
	}
}

void CINFOptionMachine::TickSlot(float i_fElapsedTime)
{
	if(m_nState != OPTION_STATE_RULLET)
	{
		return;
	}

	for(int i=0;  i< OPTION_ITEM_LIST_FIX; i++)
	{
		if(!m_SlotInfo[i]->nUseSlot || m_SlotInfo[i]->bCompleteStep)
		{
			continue;
		}

		if(m_SlotInfo[i]->fSlotTime >= GetStepSpeed(m_SlotInfo[i]->nSpeed) && TrickPattern(m_SlotInfo[i]))
		{
			SlotTranslate(m_SlotInfo[i]);
		}

		if(m_SlotInfo[i]->fAllSlotTime > OPTIONMACHINE_CHANGE_PATTERN_TIME &&
		   m_SlotInfo[i]->bChosePattern == FALSE)
		{
			ChangePattern(m_SlotInfo[i]);
		}
		if(m_SlotInfo[i]->fAllSlotTime > OPTIONMACHINE_CHANGE_PATTERN_TIME &&
			m_SlotInfo[i]->fAllSlotTime < OPTIONMACHINE_CHANGE_PATTERN_TIME + OPTIONMACHINE_CHANGE_SLOW_TIME ||
			m_SlotInfo[i]->fAllSlotTime > OPTIONMACHINE_CHANGE_PATTERN_TIME * 2)
			ChangeSpeed(m_SlotInfo[i]);
		

		if(m_SlotInfo[i]->nSpeed < OPTION_SPEED_SLOW)
			m_SlotInfo[i]->nSpeed = OPTION_SPEED_SLOW;

		if(m_SlotInfo[i]->nSpeed > MAX_SLOT_SPEED_STEP)
			m_SlotInfo[i]->nSpeed = MAX_SLOT_SPEED_STEP;

		m_SlotInfo[i]->fAllSlotTime += i_fElapsedTime;
		m_SlotInfo[i]->fSlotTime += i_fElapsedTime;
	}
}

void CINFOptionMachine::SlotTranslate(ST_SLOT_INFO* i_nInfo)
{
	if(!i_nInfo || i_nInfo->nItemInfo.empty() || i_nInfo->bCompleteStep || !i_nInfo->nUseSlot)
	{
		return;
	}

	int temp = 0;
	i_nInfo->fSlotTime = 0;
	i_nInfo->nItemPosition += i_nInfo->nSpeedCourse;
	if(i_nInfo->nItemPosition > i_nInfo->nItemInfo.size())
	{
		i_nInfo->nItemPosition = 1;
	}
	else if(i_nInfo->nItemPosition < 1)
	{
		i_nInfo->nItemPosition = i_nInfo->nItemInfo.size();
	}
	if(i_nInfo->nSlotNum == OPTION_ITEM_PRE_FIX)
	{
		m_RenderPreOption.clear();
	}
	else if(i_nInfo->nSlotNum == OPTION_ITEM_SUF_FIX)
	{
		m_RenderSufOption.clear();
	}
	int tempPosition = i_nInfo->nItemPosition;
	if(i_nInfo->nItemPosition - OPTION_ITEM_LIST_CENTER < 1)
	{
		tempPosition = i_nInfo->nItemInfo.size() - OPTION_ITEM_LIST_CENTER + tempPosition;
	}
	else
	{
		tempPosition = i_nInfo->nItemPosition - OPTION_ITEM_LIST_CENTER;
	}
	
	for(int i=0; i < OPTION_ITEM_LIST_NUM; i++)
	{
		ST_RENDER_ITEM* tempItem = new ST_RENDER_ITEM;
		
		vector<ST_ITEM_INFO*>::iterator it = i_nInfo->nItemInfo.begin();
		while(it != i_nInfo->nItemInfo.end())
		{
			temp = (*it)->nItemPosition;
			if(tempPosition == temp)
			{
				tempItem->nItemPosition = tempPosition;
				tempItem->nItemName = (*it)->nItemName;
				tempItem->nImageType = tempItem->nItemPosition%2;
				if(i_nInfo->nSlotNum == OPTION_ITEM_PRE_FIX)
				{
					m_RenderPreOption.push_back(tempItem);
					break;
				}
				else if(i_nInfo->nSlotNum == OPTION_ITEM_SUF_FIX)
				{
					m_RenderSufOption.push_back(tempItem);
					break;	
				}
			}
			it++;			
		}
		tempPosition++;
		if(tempPosition > i_nInfo->nItemInfo.size())
		{
			tempPosition = 1;
		}
		else if(tempPosition < 1)
		{
			tempPosition = i_nInfo->nItemInfo.size();
		}
	}
}

void CINFOptionMachine::ChangeSpeed(ST_SLOT_INFO* i_nInfo)
{
	if(RestPosition(i_nInfo, GetItemPosition(i_nInfo, m_pPrizeItem)) > 20)
	{
		if(i_nInfo->nSpeed < OPTION_SPEED_NORMAL)
			i_nInfo->nSpeed = OPTION_SPEED_NORMAL;
		return;
	}
	else if(RestPosition(i_nInfo, GetItemPosition(i_nInfo, m_pPrizeItem)) == 0)
	{
		i_nInfo->bCompleteStep = TRUE;
	}
	else 
	{
		if(i_nInfo->nSpeed > 1)
		{
			i_nInfo->nSpeed -= abs(i_nInfo->nSpeedCourse);	
		}		
	}
}

int CINFOptionMachine::RestPosition(ST_SLOT_INFO* i_nInfo, int Position)
{
	int size = i_nInfo->nItemInfo.size();
	int slotPosition = i_nInfo->nItemPosition;
	if(slotPosition > i_nInfo->nItemInfo.size())
	{
		slotPosition -= i_nInfo->nItemInfo.size();
	}
	if(i_nInfo->nSpeedCourse == 1)
	{
		if(Position - slotPosition > 0)
		{
			return Position - slotPosition;
		}
		else if(Position - slotPosition < 0)
		{
			return slotPosition + (size - Position);
		}
		else
		{
			return 0;
		}
	}
	else if(i_nInfo->nSpeedCourse == -1)
	{
		if(slotPosition - Position > 0)
		{
			return slotPosition - Position;
		}
		else if(slotPosition - Position < 0)
		{
			return Position + (size - slotPosition);
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

void CINFOptionMachine::ChangePattern(ST_SLOT_INFO* i_nInfo)
{
	i_nInfo->bChosePattern = TRUE;
	
	i_nInfo->nSpeed = RANDI(1, 5);

	i_nInfo->nSpeedCourse = RANDBOOL() ? 1 : -1;
	
	i_nInfo->nTrick = RANDBOOL() ? 1 : 0;
}

BOOL CINFOptionMachine::TrickPattern(ST_SLOT_INFO* i_nInfo)
{
 	if(i_nInfo->nTrick <= 0)
 	{
		return TRUE;
	}
	else
	{
		if(RestPosition(i_nInfo, GetItemPosition(i_nInfo, m_pPrizeItem)) == 1)
		{
			if(m_fTrickTime > 0.0f)
			{
				m_fTrickTime -= g_pD3dApp->GetElapsedTime();
				return FALSE;
			}
			else if( m_fTrickTime <= 0.0f)
			{
				i_nInfo->nTrick--;
				m_fTrickTime = TRICK_SPEED;
				return FALSE;
			}		
			else if(i_nInfo->nTrick < 1)
			{
				return TRUE;
			}			
		}
		return TRUE;
	}
}

int CINFOptionMachine::GetItemPosition(ST_SLOT_INFO* i_nInfo,  ITEM_GENERAL* i_Item)
{
	vector<ST_ITEM_INFO*>::iterator it = i_nInfo->nItemInfo.begin();
	while(it != i_nInfo->nItemInfo.end())
	{
		if(i_nInfo->nSlotNum == OPTION_ITEM_PRE_FIX && (*it)->nItemCode == i_Item->PrefixCodeNum)
		{
			return (*it)->nItemPosition;
		}
		else if(i_nInfo->nSlotNum == OPTION_ITEM_SUF_FIX && (*it)->nItemCode == i_Item->SuffixCodeNum)
		{
			return (*it)->nItemPosition;
		}		
		it++;
	}
	return 0;
}
void CINFOptionMachine::SetSlotinfo(vectRARE_ITEM_INFOPtrList mRareitem, int position)
{
	if(mRareitem.empty())
		return;

	vectRARE_ITEM_INFOPtrList::iterator it = mRareitem.begin();
	while(it != mRareitem.end())
	{
		if(position == OPTION_ITEM_PRE_FIX)
		{
			if(!IS_RARE_PREFIX((*it)->CodeNum))
			{
				it++;
				continue;
			}
		}
		else if(position == OPTION_ITEM_SUF_FIX)
		{
			if(!IS_RARE_SUFFIX((*it)->CodeNum))
			{
				it++;
				continue;
			}
		}
		ST_ITEM_INFO* temp = new ST_ITEM_INFO;
		temp->nItemCode = (*it)->CodeNum;
		temp->nItemName = (*it)->Name;
		m_vecItemInfo[position].push_back(temp); 
		it++;
	}
	random_shuffle(m_vecItemInfo[position].begin(), m_vecItemInfo[position].end());
}

void CINFOptionMachine::ResetInfo()
{
	m_nState = OPTION_STATE_READY;
	m_RenderPreOption.clear();
	m_RenderSufOption.clear();
	
	ST_RENDER_ITEM* temp = new ST_RENDER_ITEM;
	temp->nItemName = OPTIONMACHINE_SLOT_READY_QUESTION;
	temp->nItemPosition = 0;
	int i;
	for(i=0; i< OPTION_ITEM_LIST_NUM; i++)
	{
		m_RenderPreOption.push_back(temp);
		m_RenderSufOption.push_back(temp);
	}

	for(i=0; i< OPTION_SELECT_ITEM; i++)
	{
		if(m_pSelectItemInfo[i])
		{
			m_pSelectItemInfo[i] = NULL;
		}
	}
	for(i=0; i< OPTION_ITEM_LIST_FIX; i++)
	{
		if(m_SlotInfo[i])
			util::del(m_SlotInfo[i]);
		m_SlotInfo[i] = new ST_SLOT_INFO;
		m_vecItemInfo[i].clear();
		// 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_IsArrow[i] = NULL;
		//end 2010. 05. 13 by jskim 신규 럭키 머신 추가방안 - 받는 아이템 이펙트 강조
		m_bisInit = NULL;		
	}
	if(g_pGameMain->m_pInfWindow->IsExistMsgBox(_MESSAGE))
	{
		g_pGameMain->m_pInfWindow->DeleteMsgBox(_MESSAGE);
	}
}

void CINFOptionMachine::ResetInfo(vector<ST_RENDER_ITEM*> mRenderItem)
{
	ST_RENDER_ITEM* temp = new ST_RENDER_ITEM;
	temp->nItemName = OPTIONMACHINE_SLOT_READY_QUESTION;
	temp->nItemPosition = 0;
	
	mRenderItem.clear();
	for(int i=0; i< OPTION_ITEM_LIST_NUM; i++)
	{
		mRenderItem.push_back(temp);
	}
}

void CINFOptionMachine::PrizeItemGiveMe()
{
	ITEM_GENERAL itemGeneral;

	if(m_nState == OPTION_STATE_RULLET)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0409,COLOR_ERROR);//"\\y옵션 머신이 완전히 멈춘 후에 아이템을 받을 수 있습니다.\\y"
		return;
	}

	if(m_nState == OPTION_STATE_READY)
	{
		for(int i=0; i< OPTION_SELECT_ITEM; i++)
		{
			if(m_pSelectItemInfo[i])
			{
				ITEM_GENERAL itemGeneral;
				itemGeneral = (ITEM_GENERAL)(*m_pSelectItemInfo[i]);
				itemGeneral.ItemWindowIndex = POS_INVALID_POSITION;
				g_pStoreData->PutItem((char*)&itemGeneral, TRUE);
				g_pStoreData->FindItemInInventoryByUniqueNumber(m_pSelectItemInfo[i]->UniqueNumber)->CopyItemInfo(m_pSelectItemInfo[i]);
			}
		}
		if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] || m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] || m_pSelectItemInfo[OPTION_TARGET_ITEM])
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0410,COLOR_ERROR);//"\\y올려 놓은 아이템을 돌려 받았습니다.\\y"
		else
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0407,COLOR_ERROR);//""\\y받을 아이템이 존재 하지 않습니다.\\y""
	}
	if(m_nState == OPTION_STATE_PRIZE)
	{
		m_nState = OPTION_STATE_READY;
		if(m_pPrizeItem)
		{
			itemGeneral = (ITEM_GENERAL)(*m_pPrizeItem);
			itemGeneral.ItemWindowIndex = POS_INVALID_POSITION;
			g_pStoreData->PutItem((char*)&itemGeneral, TRUE);
			// 2010-10-14 by jskim 인첸된 무기, 아머를 옵션머신 돌린 후 받았을 경울 인첸이 사라지는 버그 수정
			g_pStoreData->FindItemInInventoryByUniqueNumber(m_pPrizeItem->UniqueNumber)->CopyItemInfo(m_pPrizeItem);
			// end 2010-10-14 by jskim 인첸된 무기, 아머를 옵션머신 돌린 후 받았을 경울 인첸이 사라지는 버그 수정

			char buf[256];
			wsprintf(buf,STRMSG_C_081124_0103,itemGeneral.ItemInfo->ItemName,1); //"%s아이템  %d개 획득"	 
			g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
		}
		else
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0407,COLOR_ERROR);
		}
	}
	g_pShuttleChild->ResortingItem();// 여기
	if(g_pGameMain && g_pGameMain->m_pInven)
	{
		g_pGameMain->m_pInven->SetScrollEndLine();
		g_pGameMain->m_pInven->SetAllIconInfo();
	}
	ResetInfo();
}

BOOL CINFOptionMachine::IsPossibleUpLoadItem(ITEM* pItem, int nPosition)
{
	if(!pItem->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE) &&
	   !pItem->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
	{
		for(int i=0; i< OPTION_SELECT_ITEM; i++)
		{
			if(m_pSelectItemInfo[i])
			{
				if(ITEMKIND_DEFENSE == m_pSelectItemInfo[i]->ItemInfo->Kind ||
					IS_WEAPON(m_pSelectItemInfo[i]->ItemInfo->Kind)
					)
				{
					if(!CAtumSJ::CheckReqItemKind(pItem->ReqItemKind, m_pSelectItemInfo[i]->ItemInfo->Kind))
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y올려 놓을 수 없습니다.\\y"
						return FALSE;
					}
				}
				else if(ITEMKIND_DEFENSE == pItem->Kind ||
					IS_WEAPON(pItem->Kind) )
				{
					if(!CAtumSJ::CheckReqItemKind(m_pSelectItemInfo[i]->ItemInfo->ReqItemKind,pItem->Kind))
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y올려 놓을 수 없습니다.\\y"
						return FALSE;
					}
				}
				else
				{
					if(!CAtumSJ::CheckReqItemKind(m_pSelectItemInfo[i]->ItemInfo->ReqItemKind, pItem->ReqItemKind))
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y올려 놓을 수 없습니다.\\y"
						return FALSE;
					}
				}
			}			
		}
	}	
	switch(nPosition)
	{
	case OPTION_ITEM_PRE_FIX:
		{
			// 접두 아이템
			if(pItem && (pItem->IsExistDesParam(DES_RARE_FIX_PREFIX) ||
						 pItem->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE))				
				)
			{
				return TRUE;
			}
		}
		break;
	case OPTION_ITEM_SUF_FIX:
		{
			// 접미 아이템
			if(pItem && (pItem->IsExistDesParam(DES_RARE_FIX_SUFFIX) ||
						 pItem->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
				)
			{
				return TRUE;
			}
		}
		break;
	case OPTION_TARGET_ITEM :
		{
			// 타겟 아이템(아머, 무기)
			if(pItem && ( IS_WEAPON(pItem->Kind) ||
						  ITEMKIND_DEFENSE == pItem->Kind) )
			{
				return TRUE;
			}

		}
		break;
	}
	return FALSE;
}

int CINFOptionMachine::PossibleUpLoadItemNum(ITEM* pItem)
{
	if(!pItem)
	{
		return -1;
	}
	if(pItem->IsExistDesParam(DES_RARE_FIX_PREFIX) ||
		pItem->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE))
		return OPTION_ITEM_PRE_FIX;

	if(pItem->IsExistDesParam(DES_RARE_FIX_SUFFIX) ||
		pItem->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
		return OPTION_ITEM_SUF_FIX;

	if(IS_WEAPON(pItem->Kind) ||
		ITEMKIND_DEFENSE == pItem->Kind )
		return OPTION_TARGET_ITEM;

	return -1;
}
void CINFOptionMachine::UpLoadItem(CItemInfo* i_pItem, int nPosition)
{	
	if(NULL == i_pItem || m_nState != OPTION_STATE_READY)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y올려 놓을 수 없습니다.\\y"
		return;
	}
	if(!m_pSelectItemInfo[nPosition])
	{
		m_pSelectItemInfo[nPosition] = new CItemInfo(i_pItem);
		m_pSelectItemInfo[nPosition]->CopyItemInfo(i_pItem);
	}
	else
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"\\y올려 놓을 수 없습니다.\\y"
		return;
	}

	if(IS_COUNTABLE_ITEM(m_pSelectItemInfo[nPosition]->Kind))
	{
		m_pSelectItemInfo[nPosition]->CurrentCount = 1;
		g_pStoreData->UpdateItemCount(i_pItem->UniqueNumber, i_pItem->CurrentCount - m_pSelectItemInfo[nPosition]->CurrentCount);
	}
	else
	{
		g_pStoreData->DeleteItem( m_pSelectItemInfo[nPosition]->UniqueNumber );
	}	
	if( nPosition == OPTION_TARGET_ITEM || nPosition == OPTION_ITEM_PRE_FIX)
	{
		if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
		{
			CAtumSJ::GetRareItemInfoList(&m_RareItemList[OPTION_ITEM_PRE_FIX], TRUE, &g_pDatabase->GetServerRareItemList(),m_pSelectItemInfo[OPTION_TARGET_ITEM]->ItemInfo, m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->ItemInfo);
			
			if(!m_RareItemList[OPTION_ITEM_PRE_FIX].empty())
				SetSlotinfo(m_RareItemList[OPTION_ITEM_PRE_FIX],OPTION_ITEM_PRE_FIX);
			
			m_RareItemList[OPTION_ITEM_PRE_FIX].clear();
		}
	}
	if( nPosition == OPTION_TARGET_ITEM || nPosition == OPTION_ITEM_SUF_FIX)
	{
		if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
		{
			CAtumSJ::GetRareItemInfoList(&m_RareItemList[OPTION_ITEM_SUF_FIX], FALSE, &g_pDatabase->GetServerRareItemList(),m_pSelectItemInfo[OPTION_TARGET_ITEM]->ItemInfo, m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->ItemInfo);
			
			if(!m_RareItemList[OPTION_ITEM_SUF_FIX].empty())
				SetSlotinfo(m_RareItemList[OPTION_ITEM_SUF_FIX],OPTION_ITEM_SUF_FIX);
	
			m_RareItemList[OPTION_ITEM_SUF_FIX].clear();
		}
	}
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_COIN, D3DXVECTOR3(0,0,0), FALSE);
}

void CINFOptionMachine::ClearItemInfo(int nPosition)
{
		m_pSelectItemInfo[nPosition]->ItemWindowIndex = POS_INVALID_POSITION;
		util::del(m_pSelectItemInfo[nPosition]);
}

void CINFOptionMachine::UsingItemLoad()
{
	if(g_pGameMain->m_stSelectItem.pSelectItem &&
		g_pGameMain->m_stSelectItem.pSelectItem->pItem)
	{
		// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 아이템 올려 놓는 구조 변경
		//CItemInfo* pItemInfo = (CItemInfo*)g_pGameMain->m_stSelectItem.pSelectItem->pItem;
		int nPosition = 0;
		CItemInfo* pItemInfo = (CItemInfo*)g_pGameMain->m_stSelectItem.pSelectItem->pItem;
		if(pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE) ||
			pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX))
		{
			nPosition = OPTION_ITEM_PRE_FIX;
		}
		else if(pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE) ||
				pItemInfo->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX))
		{
			nPosition = OPTION_ITEM_SUF_FIX;
		}
		else if(IS_WEAPON(pItemInfo->Kind) ||
				ITEMKIND_DEFENSE == pItemInfo->Kind)
		{
			nPosition = OPTION_TARGET_ITEM;
		}
		//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 아이템 올려 놓는 구조 변경
		
		if(pItemInfo && pItemInfo->ItemInfo	&& (IsPossibleUpLoadItem(pItemInfo->ItemInfo, nPosition)))	
		{
			UpLoadItem(pItemInfo, nPosition);
		}
		else
		{
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0401,COLOR_ERROR);//"해당 상점에서 사용 할 수 없는 아이템 입니다."
		}
	}
	else
	{
		DBGOUT("m_stSelectItem Error\n");
	}
}

void CINFOptionMachine::GambleButtonOK()
{
	MSG_FC_ITEM_USE_ENCHANT sMsg;
	memset((char*)&sMsg, 0x00, sizeof(sMsg));
	if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] && m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
	{
		sMsg.EnchantItemUniqueNumber	= m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->UniqueNumber;
		sMsg.EnchantItemUniqueNumber2	= m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->UniqueNumber;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->nUseSlot =TRUE;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->nSpeedCourse= 1;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->nSlotNum= 0;

		m_SlotInfo[OPTION_ITEM_SUF_FIX]->nUseSlot =TRUE;
		m_SlotInfo[OPTION_ITEM_SUF_FIX]->nSpeedCourse= 1;
		m_SlotInfo[OPTION_ITEM_SUF_FIX]->nSlotNum = 1;
	}
	else if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
	{
		sMsg.EnchantItemUniqueNumber = m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->UniqueNumber;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->nUseSlot =TRUE;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->nSpeedCourse= 1;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->nSlotNum = OPTION_ITEM_PRE_FIX;
		m_SlotInfo[OPTION_ITEM_SUF_FIX]->bCompleteStep = 1;
	}
	else if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX] && m_pSelectItemInfo[OPTION_TARGET_ITEM])
	{
		sMsg.EnchantItemUniqueNumber = m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->UniqueNumber;
		m_SlotInfo[OPTION_ITEM_SUF_FIX]->nUseSlot = TRUE;
		m_SlotInfo[OPTION_ITEM_SUF_FIX]->nSpeedCourse= 1;
		m_SlotInfo[OPTION_ITEM_SUF_FIX]->nSlotNum = OPTION_ITEM_SUF_FIX;
		m_SlotInfo[OPTION_ITEM_PRE_FIX]->bCompleteStep = 1;
	}
	else
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_100421_0402,COLOR_ERROR);//"\y인챈트/갬블을 할 수 없습니다.\y"
		return;
	}
	if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX])
	{
		if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX_INITIALIZE))
		{
			m_bisInit = 1;	
		}
	}
	if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX])
	{
		if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX_INITIALIZE))
		{
			m_bisInit = 1;
		}
	}
	if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX])
	{
		if(m_pSelectItemInfo[OPTION_ITEM_PRE_FIX]->ItemInfo->IsExistDesParam(DES_RARE_FIX_PREFIX))
		{
			m_bisInit = 2;	
		}
	}
	if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX])
	{
		if(m_pSelectItemInfo[OPTION_ITEM_SUF_FIX]->ItemInfo->IsExistDesParam(DES_RARE_FIX_SUFFIX))
		{
			m_bisInit = 2;
		}
	}
	sMsg.TargetItemUniqueNumber	= m_pSelectItemInfo[OPTION_TARGET_ITEM]->UniqueNumber;
	g_pFieldWinSocket->SendMsg( T_FC_ITEM_USE_ENCHANT, (char*)&sMsg, sizeof(sMsg));
	g_pD3dApp->m_bRequestEnable = FALSE;
	// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 접두, 접미 지울때 사운드 제거
	//g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_SLOT, D3DXVECTOR3(0,0,0), FALSE);
	//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 접두, 접미 지울때 사운드 제거
}  

BOOL CINFOptionMachine::PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg)
{
	if(m_pPrizeItem)
	{
		util::del(m_pPrizeItem);
	}
	// 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 접두, 접미 지울때 사운드 제거
	g_pD3dApp->m_pSound->PlayD3DSound(SOUND_LUCKY_SLOT, D3DXVECTOR3(0,0,0), FALSE);
	//end 2010. 05. 10 by jskim 신규 럭키 머신 추가방안 - 접두, 접미 지울때 사운드 제거

	m_pPrizeItem = new CItemInfo((ITEM_GENERAL*)m_pSelectItemInfo[OPTION_TARGET_ITEM]);
	m_pPrizeItem->CopyItemInfo(m_pSelectItemInfo[OPTION_TARGET_ITEM]);
	if(m_pPrizeItem->UniqueNumber == pMsg->ItemUID)
	{
		m_pPrizeItem->ChangeRareInfo(pMsg->PrefixCodeNum, pMsg->SuffixCodeNum);
		
		if(pMsg->PrefixCodeNum)
		{
			m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.clear();
			vector<ST_ITEM_INFO*>::iterator it= m_vecItemInfo[OPTION_ITEM_PRE_FIX].begin();
			while(it != m_vecItemInfo[OPTION_ITEM_PRE_FIX].end())
			{
				if((*it)->nItemCode == pMsg->PrefixCodeNum)
				{
					m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.push_back((*it));
					break;
				}
				it++;
			}
			while(it != m_vecItemInfo[OPTION_ITEM_PRE_FIX].end())
			{
				if(m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.size() > 99)
				{
					break;
				}
				if((*it)->nItemCode != m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo[0]->nItemCode)
					m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.push_back((*it));
				it++;
			}
			
			random_shuffle(m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.begin(), m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.end());
			for(int i=0; i < m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo.size(); i++)
			{
				m_SlotInfo[OPTION_ITEM_PRE_FIX]->nItemInfo[i]->nItemPosition = i + 1.;
			}
		}
		if(pMsg->SuffixCodeNum)
		{
			m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.clear();
			vector<ST_ITEM_INFO*>::iterator it= m_vecItemInfo[OPTION_ITEM_SUF_FIX].begin();
			while(it != m_vecItemInfo[OPTION_ITEM_SUF_FIX].end())
			{
				if((*it)->nItemCode == pMsg->SuffixCodeNum)
				{
					m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.push_back((*it));
					break;
				}
				it++;
			}
			while(it != m_vecItemInfo[OPTION_ITEM_SUF_FIX].end())
			{
				if(m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.size() > 99)
				{
					break;
				}
				if((*it)->nItemCode != m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo[0]->nItemCode)
					m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.push_back((*it));
				it++;
			}
			
			random_shuffle(m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.begin(), m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.end());
			for(int i=0; i < m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo.size(); i++)
			{
				m_SlotInfo[OPTION_ITEM_SUF_FIX]->nItemInfo[i]->nItemPosition = i + 1.;
			}
		}
		ChangeState(OPTION_STATE_RULLET);
	}
	return FALSE;
}

void CINFOptionMachine::DeleteTargetItem(int nItemUniqueNumber)
{
	
	if(m_pSelectItemInfo[OPTION_TARGET_ITEM]->UniqueNumber == nItemUniqueNumber)
		util::del( m_pSelectItemInfo[OPTION_TARGET_ITEM] );

	g_pD3dApp->m_bRequestEnable = TRUE;			// 2006-06-19 by ispark
	g_pD3dApp->m_fRequestEnableTime = REQUEST_ENABLE_INIT_TIME;
}

BOOL CINFOptionMachine::IsCloseOptionMachine()
{
	if(OPTION_STATE_RULLET == m_nState ||
		OPTION_STATE_PRIZE == m_nState)
	{
		return FALSE;
	}
	return TRUE;
}

void CINFOptionMachine::EnterMachineShop()
{
	g_pGameMain->ShowShopInven(BUILDINGKIND_LUCKY_OPTION_MACHINE, FALSE);
}

void CINFOptionMachine::OnCloseInfWnd()
{
	PrizeItemGiveMe();
}

