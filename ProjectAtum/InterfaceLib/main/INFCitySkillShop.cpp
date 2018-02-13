// INFCitySkillShop.cpp: implementation of the CINFCitySkillShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCitySkillShop.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "FieldWinSocket.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFCityBase.h"
#include "INFIcon.h"
#include "INFWindow.h"
#include "Skill.h"
#include "Chat.h"
#include "dxutil.h"

#include "INFImageEx.h"								  // 2011. 10. 10 by jskim UI시스템 변경
#include "INFToolTip.h"								// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현	
#include "Interface.h"
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#define CITY_SHOP_START_X			CITY_BASE_NPC_BOX_START_X
#define CITY_SHOP_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)

// 2008-08-22 by bhsohn EP3 인벤토리 처리
//#define SHOP_TAB_START_X			(221+28)
//#define SHOP_TAB_START_Y			33
//#define SHOP_ITEM_SLOT_START_X		(227+28)
//#define SHOP_ITEM_SLOT_START_Y		51
//#define SHOP_ICON_START_X			(230+28)
//#define SHOP_ICON_START_Y			53
#define SHOP_TAB_BK_START_X			(512)
#define SHOP_TAB_BK_START_Y			(41)
#define SHOP_TAB_START_X			(525)
#define SHOP_TAB_START_Y			(31)
#define SHOP_ITEM_SLOT_START_X		(227+28+260)
#define SHOP_ITEM_SLOT_START_Y		63
#define SHOP_ICON_START_X			(230+28+260 + 5 )
#define SHOP_ICON_START_Y			68
// end 2008-08-22 by bhsohn EP3 인벤토리 처리
#define SHOP_TAB_NUMBER				4
#define SHOP_ITEM_SLOT_SIZE_X		157
#define SHOP_ITEM_SLOT_SIZE_Y		36
#define SHOP_ICON_SIZE				30
#define SHOP_ITEM_SLOT_INTERVAL		36
#define SHOP_ITEM_NUMBER_X			(308+28+276)//346
#define SHOP_ITEM_NUMBER_Y			70//54
#define SHOP_NUMBER_BUTTON_X		(336+28+276)//347
#define SHOP_NUMBER_UP_BUTTON_Y		58//69
#define SHOP_NUMBER_BUTTON_SIZE_X	8
#define SHOP_NUMBER_BUTTON_SIZE_Y	7
#define SHOP_NUMBER_DOWN_BUTTON_Y	71//79
#define SHOP_BUY_BUTTON_START_X		(344+28+276)
#define SHOP_BUY_BUTTON_START_Y		70
#define SHOP_BUY_BUTTON_SIZE_X		30
#define SHOP_BUY_BUTTON_SIZE_Y		30
#define SHOP_CASH_START_Y			260
#define SHOP_OK_BUTTON_START_X		(333+110+270)
#define SHOP_OK_BUTTON_START_Y		250
#define SHOP_OK_BUTTON_SIZE_X		30
#define SHOP_OK_BUTTON_SIZE_Y		30
#define SHOP_CANCEL_BUTTON_START_X	(333+110+276+25)
#define SHOP_CANCEL_BUTTON_START_Y	250

#define SHOP_SCROLL_START_X			(388+28+274+86)		// 2012-02-27 by mspark, 스킬상점 스크롤 위치 라인위로 맞추기 - 맨 끝 85를 86으로 변경
#define SHOP_SCROLL_UP_START_Y		51
#define SHOP_SCROLL_SIZE_X			11
#define SHOP_SCROLL_SIZE_Y			12
#define SHOP_SCROLL_BAR_SIZE_Y		25
#define SHOP_SCROLL_DOWN_START_Y	217

#define SHOP_SCROLL_BAR_START_Y		67
#define SHOP_SCROLL_BAR_END_Y		215

#define SHOP_TAB_BUTTON_SIZE_X		276 / 4
#define SHOP_TAB_BUTTON_SIZE_Y		30

#define SHOP_SCROLL_LENGTH				(185-SHOP_SCROLL_BAR_SIZE_Y)	// 151 : scroll bar line length
//#define SHOP_SCROLL_NUMBER(i)			(m_mapItemInfo[i].size()<=0 ? 1:m_mapItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_NUMBER(i)			(m_vecItemInfo[i].size()<=0 ? 1:m_vecItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_INTERVAL(i)			(SHOP_SCROLL_LENGTH / SHOP_SCROLL_NUMBER(i))

#define SHOP_ITEM_NAME_START_X		(260+28+276)

#define SHOP_SELL_ITEM_START_X	(227+28+276)
#define SHOP_SELL_ITEM_START_Y	51
#define SHOP_SELL_ITEM_END_X	(383+28+276)
#define SHOP_SELL_ITEM_END_Y	228

#define SHOP_TAB_ATTACK				0		// SHOP_TAB_B_GEAR	
#define SHOP_TAB_DEFENSE			1		// SHOP_TAB_I_GEAR	
#define SHOP_TAB_CONTROL			2		// SHOP_TAB_M_GEAR	
#define SHOP_TAB_ATTRIBUTE			3		// SHOP_TAB_A_GEAR	

// 2008-08-22 by bhsohn EP3 인벤토리 처리
//#define BOX_SIZE_X					417
#define BOX_SIZE_X					817
#define BOX_SIZE_Y					326
#define END_OF_INVEN_X				200

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define SHOP_CASH_START_X			(223+29+275+175)
#define SHOP_ITEM_NAME_START_Y		75//53
#define SHOP_ITEM_PRICE_START_Y		75//70
#define FONT_TOTAL_PRICE_X			0
#define FONT_TOTAL_PRICE_Y			1
#define SHOP_TEX_X					CITY_SHOP_START_X + 375 + 346
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 301
#else
#define SHOP_CASH_START_X			(223+28+275+175)
#define SHOP_ITEM_NAME_START_Y		75//53
#define SHOP_ITEM_PRICE_START_Y		75//70
#define FONT_TOTAL_PRICE_X			0
#define FONT_TOTAL_PRICE_Y			1//3
#define SHOP_TEX_X					CITY_SHOP_START_X + 375 + 346
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 301
#endif 
#else
#define CITY_SHOP_START_X			CITY_BASE_NPC_BOX_START_X
#define CITY_SHOP_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)

// 2008-08-22 by bhsohn EP3 인벤토리 처리
//#define SHOP_TAB_START_X			(221+28)
//#define SHOP_TAB_START_Y			33
//#define SHOP_ITEM_SLOT_START_X		(227+28)
//#define SHOP_ITEM_SLOT_START_Y		51
//#define SHOP_ICON_START_X			(230+28)
//#define SHOP_ICON_START_Y			53
#define SHOP_TAB_BK_START_X			(10)
#define SHOP_TAB_BK_START_Y			(24)
#define SHOP_TAB_START_X			(525)
#define SHOP_TAB_START_Y			(31)
#define SHOP_ITEM_SLOT_START_X		(227+28+276)
#define SHOP_ITEM_SLOT_START_Y		51
#define SHOP_ICON_START_X			(230+28+276)
#define SHOP_ICON_START_Y			53
// end 2008-08-22 by bhsohn EP3 인벤토리 처리
#define SHOP_TAB_NUMBER				4
#define SHOP_ITEM_SLOT_SIZE_X		157
#define SHOP_ITEM_SLOT_SIZE_Y		36
#define SHOP_ICON_SIZE				30
#define SHOP_ITEM_SLOT_INTERVAL		36
#define SHOP_ITEM_NUMBER_X			(308+28+276)//346
#define SHOP_ITEM_NUMBER_Y			70//54
#define SHOP_NUMBER_BUTTON_X		(336+28+276)//347
#define SHOP_NUMBER_UP_BUTTON_Y		58//69
#define SHOP_NUMBER_BUTTON_SIZE_X	8
#define SHOP_NUMBER_BUTTON_SIZE_Y	7
#define SHOP_NUMBER_DOWN_BUTTON_Y	71//79
#define SHOP_BUY_BUTTON_START_X		(344+28+276)
#define SHOP_BUY_BUTTON_START_Y		70
#define SHOP_BUY_BUTTON_SIZE_X		39
#define SHOP_BUY_BUTTON_SIZE_Y		14
#define SHOP_CASH_START_Y			238
#define SHOP_OK_BUTTON_START_X		(333+28+276)
#define SHOP_OK_BUTTON_START_Y		240
#define SHOP_OK_BUTTON_SIZE_X		35
#define SHOP_OK_BUTTON_SIZE_Y		16
#define SHOP_CANCEL_BUTTON_START_X	(370+28+276)
#define SHOP_CANCEL_BUTTON_START_Y	240

#define SHOP_SCROLL_START_X			(388+28+274)
#define SHOP_SCROLL_UP_START_Y		51
#define SHOP_SCROLL_SIZE_X			11
#define SHOP_SCROLL_SIZE_Y			12
#define SHOP_SCROLL_BAR_SIZE_Y		30
#define SHOP_SCROLL_DOWN_START_Y	217

#define SHOP_SCROLL_BAR_START_Y		65
#define SHOP_SCROLL_BAR_END_Y		215

#define SHOP_TAB_BUTTON_SIZE_X		47
#define SHOP_TAB_BUTTON_SIZE_Y		14

#define SHOP_SCROLL_LENGTH				(151-SHOP_SCROLL_BAR_SIZE_Y)	// 151 : scroll bar line length
//#define SHOP_SCROLL_NUMBER(i)			(m_mapItemInfo[i].size()<=0 ? 1:m_mapItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_NUMBER(i)			(m_vecItemInfo[i].size()<=0 ? 1:m_vecItemInfo[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_INTERVAL(i)			(SHOP_SCROLL_LENGTH / SHOP_SCROLL_NUMBER(i))

#define SHOP_ITEM_NAME_START_X		(260+28+276)

#define SHOP_SELL_ITEM_START_X	(227+28+276)
#define SHOP_SELL_ITEM_START_Y	51
#define SHOP_SELL_ITEM_END_X	(383+28+276)
#define SHOP_SELL_ITEM_END_Y	228

#define SHOP_TAB_ATTACK				0		// SHOP_TAB_B_GEAR	
#define SHOP_TAB_DEFENSE			1		// SHOP_TAB_I_GEAR	
#define SHOP_TAB_CONTROL			2		// SHOP_TAB_M_GEAR	
#define SHOP_TAB_ATTRIBUTE			3		// SHOP_TAB_A_GEAR	

// 2008-08-22 by bhsohn EP3 인벤토리 처리
//#define BOX_SIZE_X					417
#define BOX_SIZE_X					720
#define BOX_SIZE_Y					275
#define END_OF_INVEN_X				200

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define SHOP_CASH_START_X			(223+29+275)
#define SHOP_ITEM_NAME_START_Y		51//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#define FONT_TOTAL_PRICE_X			1
#define FONT_TOTAL_PRICE_Y			1
#define SHOP_TEX_X					CITY_SHOP_START_X + 375
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 3
#else
#define SHOP_CASH_START_X			(223+28+275)
#define SHOP_ITEM_NAME_START_Y		51//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#define FONT_TOTAL_PRICE_X			0
#define FONT_TOTAL_PRICE_Y			1//3
#define SHOP_TEX_X					CITY_SHOP_START_X + 375
#define SHOP_TEX_Y					CITY_SHOP_START_Y + 3
#endif
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCitySkillShop::CINFCitySkillShop(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	m_pParent = pParent;	// CGameMain*
	m_pGameData = NULL;

	int i;
	for(i=0;i<4;i++)
	{
		m_pOkButton[i] = NULL;
		m_pCancelButton[i] = NULL;
		m_pBuyButton[i] = NULL;// [3]은 NULL
		m_pItemTab[i] = NULL;
		m_pnSelectItemNumber[i] = NULL;
		m_nCurrentScrollNumber[i] = 0;
		m_nCurrentSelectSlot[i] = -1;
	}
	m_pCash = NULL;
	m_pSlot = NULL;
	m_pScrollUpDown = NULL;
	m_pScrollBar = NULL;
	m_pSelectSlot = NULL;
	m_pFontTotalPrice = NULL;

	m_nCurrentTab = 0;
	m_nOkButtonState = BUTTON_STATE_NORMAL;
	m_nCancelButtonState = BUTTON_STATE_NORMAL;

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_nBuyButtonState[i] = BUTTON_STATE_NORMAL;
		m_pFontItemName[i] = NULL;
		m_pFontItemPrice[i] = NULL;
		m_pFontItemNumber[i] = NULL;
	}
	m_bScrollLock = FALSE;
	m_ptMouse.x = 0;
	m_ptMouse.y = 0;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
	m_bInputItemDone = FALSE;
	m_pBuildingInfo = pBuilding;
//	m_bCashErr = FALSE;
	m_pInfluenceTex = NULL;
}

CINFCitySkillShop::~CINFCitySkillShop()
{
	int i;
	for(i=0;i<4;i++)
	{
		util::del(m_pOkButton[i]);
		util::del(m_pCancelButton[i]);
		util::del(m_pBuyButton[i]);
		util::del(m_pItemTab[i]);
		util::del_array(m_pnSelectItemNumber[i]);

		m_vecItemInfo[i].clear();

	}
	vector<SHOP_ITEM*>::iterator it = m_vecAllSkillData.begin();
	while(it != m_vecAllSkillData.end())
	{
		SHOP_ITEM* pItem = *it;
		delete pItem;
		pItem = NULL;
		it++;
	}
	m_vecAllSkillData.clear();
	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		util::del(m_pFontItemName[i]);
		util::del(m_pFontItemPrice[i]);
		util::del(m_pFontItemNumber[i]);
	}
	util::del(m_pCash);
	util::del(m_pSlot);
	util::del(m_pScrollUpDown);
	util::del(m_pScrollBar);
	util::del(m_pSelectSlot);
	util::del(m_pFontTotalPrice);

	util::del(m_pInfluenceTex);
}


void CINFCitySkillShop::InitData()//UINT nShopUniqueNumber, UINT nNPCTalkIndex)
{
	m_nCurrentTab = SHOP_TAB_ATTACK;
	for(int i=0;i<4;i++)
	{
		util::del_array(m_pnSelectItemNumber[i]);
		m_nCurrentScrollNumber[i] = 0;
		m_nCurrentSelectSlot[i] = -1;
		m_vecItemInfo[i].clear();
	}
	vector<SHOP_ITEM*>::iterator it = m_vecAllSkillData.begin();
	while(it != m_vecAllSkillData.end())
	{
		SHOP_ITEM* pItem = *it;
		delete pItem;
		pItem = NULL;
		it++;
	}
	m_vecAllSkillData.clear();
}

HRESULT CINFCitySkillShop::InitDeviceObjects()
{
	DataHeader	* pDataHeader;

	int i;
	for(i=0;i<4;i++)
	{
		char buf[16];

		m_pOkButton[i] = new CINFImageEx;
		wsprintf(buf, "sauok%d",i);
		pDataHeader = FindResource(buf);
		m_pOkButton[i]->InitDeviceObjects(pDataHeader ) ;

		m_pCancelButton[i] = new CINFImageEx;
		wsprintf(buf, "shmcan0%d",i);
		pDataHeader = FindResource(buf);
		m_pCancelButton[i]->InitDeviceObjects(pDataHeader ) ;

		wsprintf(buf, "sbuy%d",i);//sbuy3은 없음.
		pDataHeader = FindResource(buf);
		if(pDataHeader)
		{
			m_pBuyButton[i] = new CINFImageEx;
			m_pBuyButton[i]->InitDeviceObjects(pDataHeader ) ;
		}

		m_pItemTab[i] = new CINFImageEx;
		// 2008-08-22 by bhsohn EP3 인벤토리 처리
		//wsprintf(buf, "sktab%d",i);
		wsprintf(buf, "sksk%d",i);
		// end 2008-08-22 by bhsohn EP3 인벤토리 처리

		pDataHeader = FindResource(buf);
		m_pItemTab[i]->InitDeviceObjects(pDataHeader ) ;

	}

	m_pCash = new CINFImageEx;
	pDataHeader = FindResource("scash");
	m_pCash->InitDeviceObjects(pDataHeader ) ;

	m_pSlot = new CINFImageEx;
	pDataHeader = FindResource("akslot");
	m_pSlot->InitDeviceObjects(pDataHeader ) ;

	m_pScrollUpDown = new CINFImageEx;
	pDataHeader = FindResource("c_scrlbt");
	m_pScrollUpDown->InitDeviceObjects(pDataHeader ) ;

	m_pScrollBar = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pScrollBar->InitDeviceObjects(pDataHeader ) ;

	m_pSelectSlot = new CINFImageEx;
	pDataHeader = FindResource("ssslot");
	m_pSelectSlot->InitDeviceObjects(pDataHeader ) ;

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,256,32);
		m_pFontItemName[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontItemPrice[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
		m_pFontItemPrice[i]->InitDeviceObjects(g_pD3dDev);
		m_pFontItemNumber[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
		m_pFontItemNumber[i]->InitDeviceObjects(g_pD3dDev);
	}
	m_pFontTotalPrice = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontTotalPrice->InitDeviceObjects(g_pD3dDev);

	m_pInfluenceTex = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pInfluenceTex->InitDeviceObjects(g_pD3dDev);

	return S_OK ;
}

HRESULT CINFCitySkillShop::RestoreDeviceObjects()
{
	int i;
	for(i=0;i<4;i++)
	{
		m_pOkButton[i]->RestoreDeviceObjects();
		m_pCancelButton[i]->RestoreDeviceObjects();
		if(m_pBuyButton[i])
			m_pBuyButton[i]->RestoreDeviceObjects();
		m_pItemTab[i]->RestoreDeviceObjects();
	}
	m_pCash->RestoreDeviceObjects();
	m_pSlot->RestoreDeviceObjects();
	m_pScrollUpDown->RestoreDeviceObjects();
	m_pScrollBar->RestoreDeviceObjects();
	m_pSelectSlot->RestoreDeviceObjects();
	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i]->RestoreDeviceObjects();
		m_pFontItemPrice[i]->RestoreDeviceObjects();
		m_pFontItemNumber[i]->RestoreDeviceObjects();
	}
	m_pFontTotalPrice->RestoreDeviceObjects();

	m_bRestored = TRUE;

	m_pInfluenceTex->RestoreDeviceObjects();
	return S_OK ;
}

HRESULT CINFCitySkillShop::DeleteDeviceObjects()
{
	int i;
	for(i=0;i<4;i++)
	{
		// 2012-11-08 by jhjang 안전 코드 추가.
		if(m_pOkButton[i])
		{
			m_pOkButton[i]->DeleteDeviceObjects();
			util::del(m_pOkButton[i]);
		}
		if(m_pCancelButton[i])
		{
			m_pCancelButton[i]->DeleteDeviceObjects();
			util::del(m_pCancelButton[i]);
		}
		if(m_pBuyButton[i])
		{
			m_pBuyButton[i]->DeleteDeviceObjects();
			util::del(m_pBuyButton[i]);
		}
		if(m_pItemTab[i])
		{
			m_pItemTab[i]->DeleteDeviceObjects();
			util::del(m_pItemTab[i]);
		}
		// end 2012-11-08 by jhjang 안전 코드 추가.
		m_vecItemInfo[i].clear();
	}
	vector<SHOP_ITEM*>::iterator it = m_vecAllSkillData.begin();
	while(it != m_vecAllSkillData.end())
	{
		SHOP_ITEM* pItem = *it;
		delete pItem;
		pItem = NULL;
		it++;
	}
	m_vecAllSkillData.clear();

	// 2012-11-08 by jhjang 안전 코드 추가.
	if(m_pCash)
	{
		m_pCash->DeleteDeviceObjects();
		util::del(m_pCash);
	}
	if(m_pSlot)
	{
		m_pSlot->DeleteDeviceObjects();
		util::del(m_pSlot);
	}
//	util::del(m_pItemTab[3]);

	if(m_pScrollUpDown)
	{
		m_pScrollUpDown->DeleteDeviceObjects();
		util::del(m_pScrollUpDown);
	}
	if(m_pScrollBar)
	{
		m_pScrollBar->DeleteDeviceObjects();
		util::del(m_pScrollBar);
	}
	if(m_pSelectSlot)
	{
		m_pSelectSlot->DeleteDeviceObjects();
		util::del(m_pSelectSlot);
	}
	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		if(m_pFontItemName[i])
		{
			m_pFontItemName[i]->DeleteDeviceObjects();
			util::del(m_pFontItemName[i]);
		}
		if(m_pFontItemPrice[i])
		{
			m_pFontItemPrice[i]->DeleteDeviceObjects();
			util::del(m_pFontItemPrice[i]);
		}
		if(m_pFontItemNumber[i])
		{
			m_pFontItemNumber[i]->DeleteDeviceObjects();
			util::del(m_pFontItemNumber[i]);
		}
	}

	if(m_pFontTotalPrice)
	{
		m_pFontTotalPrice->DeleteDeviceObjects();
		util::del(m_pFontTotalPrice);
	}

	m_bInvalidated = FALSE;

	if(m_pInfluenceTex)
	{
		m_pInfluenceTex->DeleteDeviceObjects();
		util::del(m_pInfluenceTex);
	}
	// end 2012-11-08 by jhjang 안전 코드 추가.
	return S_OK ;
}


HRESULT CINFCitySkillShop::InvalidateDeviceObjects()
{
	int i;
	for(i=0;i<4;i++)
	{
		m_pOkButton[i]->InvalidateDeviceObjects();
		m_pCancelButton[i]->InvalidateDeviceObjects();
		if(m_pBuyButton[i])
			m_pBuyButton[i]->InvalidateDeviceObjects();
		m_pItemTab[i]->InvalidateDeviceObjects();
	}
	m_pCash->InvalidateDeviceObjects();
	m_pSlot->InvalidateDeviceObjects();
	m_pScrollUpDown->InvalidateDeviceObjects();
	m_pScrollBar->InvalidateDeviceObjects();
	m_pSelectSlot->InvalidateDeviceObjects();

	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i]->InvalidateDeviceObjects();
		m_pFontItemPrice[i]->InvalidateDeviceObjects();
		m_pFontItemNumber[i]->InvalidateDeviceObjects();
	}
	m_pFontTotalPrice->InvalidateDeviceObjects();
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	m_pInfluenceTex->InvalidateDeviceObjects();
	return S_OK ;
}

void CINFCitySkillShop::Tick()
{
}

void CINFCitySkillShop::Render()
{
	FLOG( "CINFCityShop::Render()" );
	// 2008-08-22 by bhsohn EP3 인벤토리 처리
//	m_pItemTab[m_nCurrentTab]->Move(CITY_SHOP_START_X+SHOP_TAB_START_X, CITY_SHOP_START_Y+SHOP_TAB_START_Y);
//	m_pItemTab[m_nCurrentTab]->Render();
	m_pItemTab[m_nCurrentTab]->Move(CITY_SHOP_START_X+SHOP_TAB_BK_START_X, CITY_SHOP_START_Y+SHOP_TAB_BK_START_Y);
	m_pItemTab[m_nCurrentTab]->Render();	

	int nSelectNumber = 0;
	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
	int i;
	for(i=0;i<m_nCurrentScrollNumber[m_nCurrentTab];i++)
	{
		if(it != m_vecItemInfo[m_nCurrentTab].end())
		{
			it++;
			nSelectNumber++;
		}
		else
		{
			break;
		}
	}
	if(m_pnSelectItemNumber[m_nCurrentTab])
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		for(i=0;i<SHOP_ITEM_SLOT_NUMBER-3 ;i++)
#else						  
		for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
#endif
		{
			if(m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && m_nCurrentSelectSlot[m_nCurrentTab] == nSelectNumber+i)
			{
				m_pSelectSlot->Move(CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X, CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_INTERVAL*i);
				m_pSelectSlot->Render();
			}
			else
			{
				m_pSlot->Move(CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X, CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_INTERVAL*i);
				m_pSlot->Render();
			}
//			if(m_nBuyButtonState[i] != BUTTON_STATE_NORMAL)
//			{
//				m_pBuyButton[m_nBuyButtonState[i]]->Move(CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i);
//				m_pBuyButton[m_nBuyButtonState[i]]->Render();
//			}
			if(it != m_vecItemInfo[m_nCurrentTab].end())
			{
				SHOP_ITEM* pItem = *it;
				char buf[64];
				// 2005-08-23 by ispark
				ITEM* pOriItem = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
//				wsprintf( buf, "%08d", SKILL_BASE_NUM(pItem->ItemNum));
				// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//				wsprintf( buf, "%08d", SKILL_BASE_NUM(pOriItem->SourceIndex));
				wsprintf( buf, "%08d", SKILL_BASE_SOURCEINDEX(pOriItem->SourceIndex));
				((CINFGameMain*)m_pParent)->m_pIcon->SetIcon( buf,CITY_SHOP_START_X+SHOP_ICON_START_X,CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+1, 1.0f );
				((CINFGameMain*)m_pParent)->m_pIcon->Render();

				char temp1[64];
				char temp2[64];
//				wsprintf( temp1, "%d", pItem->Price );
				wsprintf( temp1, "%d", CAtumSJ::GetCityWarTex(pItem->Price, m_fTexRate)+pItem->Price );
				MakeCurrencySeparator( temp2, temp1, 3, ',' );
				m_pFontItemName[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X, CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, pItem->ItemName, 0L);
				wsprintf(buf, "%7s", temp2);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
				SIZE sSize = m_pFontItemPrice[i]->GetStringSize( buf );
				m_pFontItemPrice[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X + 200 - sSize.cx, CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
#else
				m_pFontItemPrice[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X,CITY_SHOP_START_Y+SHOP_ITEM_PRICE_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
#endif
				it++;

				
				// 아이템 스킬레벨 렌더링
				vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
				for( int j=0;j<i+m_nCurrentScrollNumber[m_nCurrentTab];j++)
				{
					if(it != m_vecItemInfo[m_nCurrentTab].end())
					{
						it++;
					}
					else
					{
						break;
					}
				}
				if(it != m_vecItemInfo[m_nCurrentTab].end())
				{
					wsprintf(buf, STRMSG_C_SHOP_0012, SKILL_LEVEL((*it)->ItemNum));//"레벨:%d"
					
					ITEM* pITEM = g_pDatabase->GetServerItemInfo( (*it)->ItemNum );
					if(pITEM)
					{
						if(g_pShuttleChild->m_myShuttleInfo.Level >= pITEM->ReqMinLevel)
						{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
							m_pFontItemNumber[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NUMBER_X+40,
								(CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i)-1, GUI_FONT_COLOR_Y, buf, 0L);
#else
							m_pFontItemNumber[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NUMBER_X+30,
								(CITY_SHOP_START_Y+SHOP_ITEM_NUMBER_Y+SHOP_ITEM_SLOT_INTERVAL*i)-1, GUI_FONT_COLOR_Y, buf, 0L);
#endif
						}
						else
						{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
							m_pFontItemNumber[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NUMBER_X+40,
								(CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i)-1, RGB(255,0,0), buf, 0L);
#else							
							m_pFontItemNumber[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NUMBER_X+30,
								(CITY_SHOP_START_Y+SHOP_ITEM_NUMBER_Y+SHOP_ITEM_SLOT_INTERVAL*i)-1, RGB(255,0,0), buf, 0L);
#endif
						}
					}					
				}
				
			}
		}
//		m_pCash->Move(CITY_SHOP_START_X+SHOP_CASH_START_X, CITY_SHOP_START_Y+SHOP_CASH_START_Y);
//		m_pCash->Render();
		char temp1[64];
		char temp2[64];
		wsprintf( temp1, "%d", GetTotalPrice() );
		MakeCurrencySeparator( temp2, temp1, 3, ',' );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		SIZE sSize = m_pFontTotalPrice->GetStringSize( temp2 );
		m_pFontTotalPrice->DrawText( CITY_SHOP_START_X+SHOP_CASH_START_X + FONT_TOTAL_PRICE_X - sSize.cx, CITY_SHOP_START_Y+SHOP_CASH_START_Y + FONT_TOTAL_PRICE_Y, GUI_FONT_COLOR, temp2, 0L);
#else
		m_pFontTotalPrice->DrawText( CITY_SHOP_START_X+SHOP_CASH_START_X + FONT_TOTAL_PRICE_X, CITY_SHOP_START_Y+SHOP_CASH_START_Y + FONT_TOTAL_PRICE_Y, GUI_FONT_COLOR, temp2, 0L);
#endif
	}

	m_pOkButton[m_nOkButtonState]->Move(CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y);
	m_pOkButton[m_nOkButtonState]->Render();
	m_pCancelButton[m_nCancelButtonState]->Move(CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y);
	m_pCancelButton[m_nCancelButtonState]->Render();

	//////////////////

	// scroll rendering
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y);
	m_pScrollUpDown->Render();
	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y);
	m_pScrollUpDown->Render();
#endif

	m_pScrollBar->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X,
		CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab));
	m_pScrollBar->Render();

	//////////////////
	// 2006-02-08 by ispark, 세력 세금
	char chTexbuf[30] = {0,};
	sprintf(chTexbuf, STRMSG_C_060208_0000, m_fTexRate);
	m_pInfluenceTex->DrawText(SHOP_TEX_X, SHOP_TEX_Y, GUI_FONT_COLOR, chTexbuf, 0L);
}


int CINFCitySkillShop::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			int nStartX = CITY_SHOP_START_X;
			int nStartY = CITY_SHOP_START_Y;
			if( pt.x > nStartX &&
				pt.x < nStartX+BOX_SIZE_X &&
				pt.y > nStartY &&
				pt.y < nStartY+BOX_SIZE_Y )
			{
				if( pt.x> CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X && 
					pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
					pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
// 					if(m_nOkButtonState != BUTTON_STATE_DOWN)
// 					{
// 						m_nOkButtonState = BUTTON_STATE_UP;
// 					}
// 					}
// 					else
// 					{
// 						m_nOkButtonState = BUTTON_STATE_NORMAL;
// 					}
// 					if( pt.x> CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
// 						pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
// 						pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y &&
// 						pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
// 					{
// 						if(m_nCancelButtonState != BUTTON_STATE_DOWN)
// 						{
// 							m_nCancelButtonState = BUTTON_STATE_UP;
// 						}
// 					}
// 					else
// 					{
// 						m_nCancelButtonState = BUTTON_STATE_NORMAL;
// 					}
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				{
					if(m_nOkButtonState != BUTTON_STATE_DOWN)
					{
						m_nOkButtonState = BUTTON_STATE_UP;
						g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP71");
						//break;
					}
				}
				else if( pt.x> CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
					pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
					pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y &&
					pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
				{
					if(m_nCancelButtonState != BUTTON_STATE_DOWN)
					{
						m_nCancelButtonState = BUTTON_STATE_UP;
						// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
						g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP41");
						// end end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
						//break;
					}
				}
				else
				{
					m_nOkButtonState = BUTTON_STATE_NORMAL;
					m_nCancelButtonState = BUTTON_STATE_NORMAL;
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					//break;
				}
				for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
				{
//					if( pt.x>CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X && 
//						pt.x<CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X+SHOP_BUY_BUTTON_SIZE_X &&
//						pt.y>CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
//						pt.y<CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_BUY_BUTTON_SIZE_Y+SHOP_ITEM_SLOT_INTERVAL*i)
//					{
//						if(m_nBuyButtonState[i] != BUTTON_STATE_DOWN)
//						{
//							m_nBuyButtonState[i] = BUTTON_STATE_UP;
//						}
//					}
//					else
//					{
//						m_nBuyButtonState[i] = BUTTON_STATE_NORMAL;
//					}
					if( pt.x > CITY_SHOP_START_X+SHOP_ICON_START_X && 
						pt.x < CITY_SHOP_START_X+SHOP_ICON_START_X+SHOP_ICON_SIZE &&
						pt.y > CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y < CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_ICON_SIZE)
					{
						vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
						for( int j=0;j<i+m_nCurrentScrollNumber[m_nCurrentTab];j++)
						{
							if(it != m_vecItemInfo[m_nCurrentTab].end())
							{
								it++;
							}
							else
							{
								break;
							}
						}
						if(it != m_vecItemInfo[m_nCurrentTab].end())
						{
							((CINFGameMain*)m_pParent)->SetItemInfo( 0, (*it)->ItemNum, pt.x, pt.y);
						}
						break;
					}
					else if( pt.x > CITY_SHOP_START_X+SHOP_ICON_START_X+SHOP_ICON_SIZE ||
						(pt.x > CITY_SHOP_START_X+END_OF_INVEN_X && pt.x < CITY_SHOP_START_X+SHOP_ICON_START_X) ||
						pt.y < CITY_SHOP_START_Y+SHOP_ICON_START_Y-5 ||
						pt.y > CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*SHOP_ITEM_SLOT_NUMBER)
					{
						((CINFGameMain*)m_pParent)->SetItemInfo( 0, NULL, 0, 0);
					}

				}
			}
			else
			{
				((CINFGameMain*)m_pParent)->SetItemInfo( 0, NULL, 0, 0);
			}
			if(m_bScrollLock)
			{
				int interval = SHOP_SCROLL_INTERVAL(m_nCurrentTab);
				if(interval > 0)
				{
					int nMove = (pt.y - m_ptMouse.y)/interval;
					if(nMove != 0)
					{
						m_nCurrentScrollNumber[m_nCurrentTab] += nMove;
						if(m_nCurrentScrollNumber[m_nCurrentTab] < 0 )
							m_nCurrentScrollNumber[m_nCurrentTab] = 0;
						if(m_nCurrentScrollNumber[m_nCurrentTab] > SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab))
							m_nCurrentScrollNumber[m_nCurrentTab] = SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab);
						m_ptMouse = pt;
					}
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
			if( pt.x>CITY_SHOP_START_X+ SHOP_OK_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				m_nOkButtonState = BUTTON_STATE_DOWN;
			}
			if( pt.x>CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				m_nCancelButtonState = BUTTON_STATE_DOWN;
			}
//			for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
//			{
//				if( pt.x>CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X && 
//					pt.x<CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X+SHOP_BUY_BUTTON_SIZE_X &&
//					pt.y>CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
//					pt.y<CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_BUY_BUTTON_SIZE_Y+SHOP_ITEM_SLOT_INTERVAL*i)
//				{
//					m_nBuyButtonState[i] = BUTTON_STATE_DOWN;
//				}
//			}
			int i;
			for(i=0;i<SHOP_TAB_NUMBER;i++)
			{
				int nStartX = CITY_SHOP_START_X+SHOP_TAB_START_X+SHOP_TAB_BUTTON_SIZE_X*i;
				int nStartY = CITY_SHOP_START_Y+SHOP_TAB_START_Y;
				if( pt.x> nStartX && 
					pt.x< (nStartX+SHOP_TAB_BUTTON_SIZE_X) &&
					pt.y>nStartY && 
					pt.y<(nStartY+SHOP_TAB_BUTTON_SIZE_Y))
				{
					for(int Stabj=0;Stabj<SHOP_ITEM_TAB_NUMBER;Stabj++)
					{
						for(int Sta=0;Sta<m_vecItemInfo[Stabj].size();Sta++)
						{
							m_pnSelectItemNumber[Stabj][Sta] = 0;
						}
					}
					
					m_nCurrentTab = i;
				}
			}
			if( pt.x > CITY_SHOP_START_X+SHOP_SCROLL_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_SCROLL_START_X+SHOP_SCROLL_SIZE_X)
			{
				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y+SHOP_SCROLL_SIZE_Y)
				{
					m_nCurrentScrollNumber[m_nCurrentTab]--;
					if(m_nCurrentScrollNumber[m_nCurrentTab]<0)
						m_nCurrentScrollNumber[m_nCurrentTab] = 0;
				}
				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y+SHOP_SCROLL_SIZE_Y)
				{
					m_nCurrentScrollNumber[m_nCurrentTab]++;
					if(m_nCurrentScrollNumber[m_nCurrentTab]>SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab))
						m_nCurrentScrollNumber[m_nCurrentTab] = SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab);
				}
				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab) &&
					pt.y < CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab)+SHOP_SCROLL_BAR_SIZE_Y)
				{
					m_bScrollLock = TRUE;
					m_ptMouse = pt;
				}
			}
			if( pt.x > CITY_SHOP_START_X+SHOP_NUMBER_BUTTON_X+34 && 
				pt.x < CITY_SHOP_START_X+SHOP_NUMBER_BUTTON_X+SHOP_NUMBER_BUTTON_SIZE_X+34)
			{
				for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
				{
					if( pt.y>CITY_SHOP_START_Y+SHOP_NUMBER_UP_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y<CITY_SHOP_START_Y+SHOP_NUMBER_UP_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
					{
						if(m_vecItemInfo[m_nCurrentTab].size() > i+m_nCurrentScrollNumber[m_nCurrentTab] )
						{
							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 1;
							if( GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
							{
								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
							}
//							int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]];
//							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] += 
//								m_vecItemInfo[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
//							if( GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//							{//더한 후 price가 원래돈보다 많으면 취소해야 한다.
//								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = temp;
//							}
						}
					}
					if( pt.y>CITY_SHOP_START_Y+SHOP_NUMBER_DOWN_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y<CITY_SHOP_START_Y+SHOP_NUMBER_DOWN_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
					{
						if(m_vecItemInfo[m_nCurrentTab].size() > i+m_nCurrentScrollNumber[m_nCurrentTab] )
						{
							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
//							int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]];
//							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] -= 
//								m_vecItemInfo[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
//							if(m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]<0)
//								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
						}
					}
				}
			}
			if( pt.x > CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X + SHOP_ITEM_SLOT_SIZE_X)
			{
				for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
				{
					if( pt.y > CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_SIZE_Y*i &&
						pt.y < CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_SIZE_Y*(i+1))
					{
						m_nCurrentSelectSlot[m_nCurrentTab] = m_nCurrentScrollNumber[m_nCurrentTab]+i;

						for(int Stabi=0;Stabi<SHOP_ITEM_TAB_NUMBER;Stabi++)
						{
							for(int Stab=0;Stab<m_vecItemInfo[Stabi].size();Stab++)
							{
								m_pnSelectItemNumber[Stabi][Stab] = 0;
							}
						}
						if((m_pnSelectItemNumber[m_nCurrentTab])
							&& (m_nCurrentSelectSlot[m_nCurrentTab] < m_vecItemInfo[m_nCurrentTab].size())) // 2012-11-08 by jhjang 스킬샵 빈칸 선택시 익셉션 오류가 나던것 수정
						{
							m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 1;
						}
//						if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//						{
//							m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
//							m_bCashErr = TRUE;
//						}
					}
				}
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if( pt.x > CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X &&
				pt.x < CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X+SHOP_ITEM_SLOT_SIZE_X &&
				pt.y > CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y &&
				pt.y < CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_SIZE_Y*SHOP_ITEM_SLOT_NUMBER)
			{
				if((int)wParam<0)
				{
					m_nCurrentScrollNumber[m_nCurrentTab]++;
					if(m_nCurrentScrollNumber[m_nCurrentTab]>SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab))
						m_nCurrentScrollNumber[m_nCurrentTab] = SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab);
					return INF_MSGPROC_BREAK;
				}
				else
				{
					m_nCurrentScrollNumber[m_nCurrentTab]--;
					if(m_nCurrentScrollNumber[m_nCurrentTab]<0)
						m_nCurrentScrollNumber[m_nCurrentTab] = 0;
					return INF_MSGPROC_BREAK;
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
			if( pt.x>CITY_SHOP_START_X+ SHOP_OK_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y &&
				pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				if(m_nOkButtonState == BUTTON_STATE_DOWN && m_nCurrentSelectSlot[m_nCurrentTab] >= 0)
				{
					OnOkButtonClicked();
				}
				else if(m_nCurrentSelectSlot[m_nCurrentTab] < 0)
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060105_0100, COLOR_ERROR);// "구입을 할 스킬을 선택하세요."
				}
				m_nOkButtonState = BUTTON_STATE_UP;
			}
			if( pt.x>CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				if(m_nCancelButtonState == BUTTON_STATE_DOWN)
				{
					OnCancelButtonClicked();
				}
				m_nCancelButtonState = BUTTON_STATE_UP;
			}
//			for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
//			{
//				if( pt.x>CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X && 
//					pt.x<CITY_SHOP_START_X+SHOP_BUY_BUTTON_START_X+SHOP_BUY_BUTTON_SIZE_X &&
//					pt.y>CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i && 
//					pt.y<CITY_SHOP_START_Y+SHOP_BUY_BUTTON_START_Y+SHOP_BUY_BUTTON_SIZE_Y+SHOP_ITEM_SLOT_INTERVAL*i)
//				{
//					if(m_nBuyButtonState[i] == BUTTON_STATE_DOWN)
//					{
//						OnBuyButtonClicked(i);
//					}
//					m_nBuyButtonState[i] = BUTTON_STATE_UP;
//				}
//			}

/*			if( g_pGameMain->m_pInven->m_pSelectItem &&
				pt.x > CITY_SHOP_START_X+SHOP_SELL_ITEM_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_SELL_ITEM_END_X &&
				pt.y > CITY_SHOP_START_Y+SHOP_SELL_ITEM_START_Y &&
				pt.y<CITY_SHOP_START_Y+SHOP_SELL_ITEM_END_Y)
			{
				if(g_pGameMain->m_pInven->m_pSelectItem->Wear == WEAR_NOT_ATTACHED )
				{
					if( IS_COUNTABLE_ITEM(g_pGameMain->m_pInven->m_pSelectItem->Kind))
					{
						char buf[128];
						ITEM *pITEM = g_pGameMain->m_pInven->m_pSelectItem->GetItemInfo();
						if(pITEM)
						{
							char temp1[64];
							char temp2[64];
							wsprintf( temp1, "%d", CAtumSJ::GetPriceAtShop(pITEM) );
							MakeCurrencySeparator( temp2, temp1, 3, ',' );
							wsprintf( buf, "%s 몇개 파시겠습니까?[가격:%d개당 %s(spi)]", pITEM->ItemName, pITEM->MinTradeQuantity, temp2 );
							g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ENERGY,
								(DWORD)g_pGameMain->m_pInven->m_pSelectItem);
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( "서버로부터 정보 받는중...다시 시도하세요.", COLOR_SYSTEM );
						}
					}
					else
					{
						char buf[128];
						ITEM *pITEM = g_pGameMain->m_pInven->m_pSelectItem->GetItemInfo();
						if(pITEM)
						{
							char temp1[64];
							char temp2[64];
							wsprintf( temp1, "%d", CAtumSJ::GetPriceAtShop(pITEM) );
							MakeCurrencySeparator( temp2, temp1, 3, ',' );
							wsprintf( buf, "%s 를 파시겠습니까?[가격:%s(spi)]", pITEM->ItemName, temp2);
							g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_SHOP_SELL_ITEM,
								(DWORD)g_pGameMain->m_pInven->m_pSelectItem);
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( "서버로부터 정보 받는중...다시 시도하세요.", COLOR_SYSTEM );
						}
					}
				}
				else
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( "장착된 아이템은 팔 수 없습니다.", _MESSAGE );
				}

			}
*/			m_bScrollLock = FALSE;
			((CINFGameMain*)m_pParent)->SetItemInfo( 0, 0, 0, 0 );
		}
		break;
		// 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
	case WM_CHAR:
		{
 			char code = wParam;
// 			if(m_pnSelectItemNumber[m_nCurrentTab])
// 			{
// 				if(m_nCurrentSelectSlot[m_nCurrentTab]>=0)
// 				{
// 					if(code == '0')
// 					{
// 						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
// 					}
// 					else if(code == '1')
// 					{
// 						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 1;
// 						if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
// 							m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
// 					}
// 				}
// 			}
			if( code == VK_ESCAPE )
			{
				OnCancelButtonClicked();
			}
		}
		break;
	case WM_KEYDOWN:
		{
			DWORD dwCode = lParam;
			if(m_pnSelectItemNumber[m_nCurrentTab])
			{
				if((m_nCurrentSelectSlot[m_nCurrentTab]>=0)
					&& (m_nCurrentSelectSlot[m_nCurrentTab] >= m_vecItemInfo[m_nCurrentTab].size()))// 2012-11-08 by jhjang 스킬샵 빈칸 선택시 익셉션 오류가 나던것 수정
				{
					int nNumber = g_pGameMain->GetKeyDownToNumber(dwCode);
					if(nNumber == 0)
					{
						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
					}
					else if(nNumber == 1)
					{
						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 1;
						if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
							m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
					}
				}
			}
		}
		break;
		// end 2009-01-30 by bhsohn 상점 숫자 입력 방식 변경
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCitySkillShop::OnOkButtonClicked()
{
	

	for(int tab=0;tab<4;tab++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[tab].begin();
		int i=0;
		while(it != m_vecItemInfo[tab].end())
		{
			if(m_pnSelectItemNumber[tab][i]>0)
			{
				// 2005-11-18 by ispark
				// 레벨이 맞지 않으면 보내지 않는다.
				ITEM* pITEM = g_pDatabase->GetServerItemInfo( (*it)->ItemNum );

				// 2006-08-07 by dgwoo 일반 세력일때 판매 및 구매 스킬을 구입할수 없다.
				if(IsInfluenceCharacter(g_pShuttleChild->m_myShuttleInfo.InfluenceType,INFLUENCE_TYPE_NORMAL, 0))
				{
					if(IS_BAZAAR_SKILL(pITEM))
					{
						g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_060807_0100, COLOR_ERROR);		//"세력을 선택하지 않아 스킬(판매,구매)을 구입할 수 없습니다."
						return ;
					}
				}

				if(g_pShuttleChild->m_myShuttleInfo.Level < pITEM->ReqMinLevel)
				{
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_051118_0001, COLOR_ERROR);//레벨이 낮아 구입을 할 수 없습니다."
					return;	
				}

				g_pD3dApp->m_bRequestEnable = FALSE;

				SHOP_ITEM* pItem = *it;
				MSG_FC_SHOP_BUY_ITEM sMsg;
				memset(&sMsg,0x00,sizeof(sMsg));
				sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
				sMsg.ItemNum = pItem->ItemNum;
				sMsg.Amount = m_pnSelectItemNumber[tab][i];
				g_pFieldWinSocket->SendMsg( T_FC_SHOP_BUY_ITEM, (char*)&sMsg, sizeof(sMsg) );
				m_pnSelectItemNumber[tab][i] = 0;
				m_nCurrentSelectSlot[m_nCurrentTab] = -1;
				return;
			}
			it++;
			i++;
		}
	}	
	// 2006-01-05 by ispark, 스피 비교 수정
	if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
	{
		m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
		m_nCurrentSelectSlot[m_nCurrentTab] = -1;
		g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_SHOP_0013,COLOR_ERROR );//"스킬을 구입하기 위한 스피가 부족합니다"
//		m_bCashErr = FALSE;
		return;
	}
	m_nCurrentSelectSlot[m_nCurrentTab] = -1;
}

void CINFCitySkillShop::OnCancelButtonClicked()
{
	for(int tab=0;tab<4;tab++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[tab].begin();
		int i=0;
		while(it != m_vecItemInfo[tab].end())
		{
			m_pnSelectItemNumber[tab][i] = 0;
			it++;
			i++;
		}
		m_nCurrentSelectSlot[tab] = -1;
	}
}

//void CINFCitySkillShop::OnBuyButtonClicked(int nSlotNumber)
//{
//	vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[m_nCurrentTab].begin();
//	for(int i=0;i<m_nCurrentScrollNumber[m_nCurrentTab]+nSlotNumber;i++)
//	{
//		if(it != m_vecItemInfo[m_nCurrentTab].end())
//		{
//			it++;
//		}
//		else
//		{
//			break;
//		}
//	}
//	if(it != m_vecItemInfo[m_nCurrentTab].end())
//	{
//		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]] == 0)
//		{
//			m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]] = 1;
//			if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//			{
//				m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
//				g_pD3dApp->m_pChat->CreateChatChild( "스킬을 구입하기 위한 스피가 부족합니다",COLOR_ERROR );
//			}
//		}
//		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]]>0)
//		{
//			BuyItem((*it)->ItemNum, m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]]);
//		}
//		m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
//	}
//
//}

UINT CINFCitySkillShop::GetTotalPrice()
{
	FLOG( "CINFCityShop::GetTotalPrice()" );
	int total = 0;
	for(int tab=0;tab<4;tab++)
	{
		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[tab].begin();
		int i=0;
		while(it != m_vecItemInfo[tab].end())
		{
			if(m_pnSelectItemNumber[tab][i]>0)
			{
				SHOP_ITEM* pItem = *it;
				total += pItem->Price;
			}
			it++;
			i++;
		}
	}
	total += CAtumSJ::GetCityWarTex(total, m_fTexRate);
	return total;

}

//void CINFCitySkillShop::BuyItem(int nItemNum, int nAmount)
//{
//	MSG_FC_SHOP_BUY_ITEM sMsg;
//	memset(&sMsg,0x00,sizeof(sMsg));
//	sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
//	sMsg.ItemNum = nItemNum;
//	sMsg.Amount = nAmount;
//	g_pFieldWinSocket->SendMsg( T_FC_SHOP_BUY_ITEM, (char*)&sMsg, sizeof(sMsg) );
//}


int CINFCitySkillShop::InputItem(char* pMsg)
{
	if(pMsg)
	{
		SHOP_ITEM* pShopSkill = new SHOP_ITEM;
		memcpy(pShopSkill, pMsg, sizeof(SHOP_ITEM));

		// 2006-01-11 by ispark, 같은 정보가 있는지 확인
		// 렉에 의해서 중복 입력이 되는 현상이 있다. 그래서 같은 정보가 있으면 리턴
		if(FindSkillShopData(pShopSkill) == NULL)
		{
		m_vecAllSkillData.push_back(pShopSkill);
		}

/*		ITEM *pITEMSkill = g_pDatabase->GetServerItemInfo(((SHOP_ITEM*)pMsg)->ItemNum);
		if(pITEMSkill == NULL)
		{
			DBGOUT("스킬(ItemNum:%d) 정보가 없습니다.\n",((SHOP_ITEM*)pMsg)->ItemNum);
			return sizeof(SHOP_ITEM);
		}
		if(FALSE == IS_SAME_UNITKIND(g_pShuttleChild->m_myShuttleInfo.UnitKind, pITEMSkill->ReqUnitKind))
		{
			return sizeof(SHOP_ITEM);
		}
		int nSkillLevel = g_pShuttleChild->m_pSkill->FindItemSkillLevelByBaseNum(SKILL_BASE_NUM(pITEMSkill->ItemNum));
		if( nSkillLevel == 0 ) // skill이 존재하지 않음
		{
			if(SKILL_LEVEL(pShopSkill->ItemNum) != 1) // 레벨 1의 스킬
			{
				return sizeof(SHOP_ITEM);
			}
		}
		else if( nSkillLevel != SKILL_LEVEL(pShopSkill->ItemNum)+1) // skill 다음 레벨
		{
			return sizeof(SHOP_ITEM);
		}
		switch(pITEMSkill->Kind)
		{
		case ITEMKIND_SKILL_ATTACK:
			{
				m_vecItemInfo[SHOP_TAB_ATTACK].push_back(pShopSkill);
			}
			break;
		case ITEMKIND_SKILL_DEFENSE:
			{
				m_vecItemInfo[SHOP_TAB_DEFENSE].push_back(pShopSkill);
			}
			break;
		case ITEMKIND_SKILL_SUPPORT:
			{
				m_vecItemInfo[SHOP_TAB_CONTROL].push_back(pShopSkill);
			}
			break;
		case ITEMKIND_SKILL_ATTRIBUTE:
			{
				m_vecItemInfo[SHOP_TAB_ATTRIBUTE].push_back(pShopSkill);
			}
			break;
		}
*/	}
	return sizeof(SHOP_ITEM);
}

// 2015-05-28 Future, function never referenced => Commented out
//void CINFCitySkillShop::UpdateItem(int nItemNum)
//{
//	for(int i=0;i<4;i++)
//	{
//		util::del_array(m_pnSelectItemNumber[i]);
//		if(m_vecItemInfo[i].size() > 0)
//		{
//			int num = m_vecItemInfo[i].size();
//			m_pnSelectItemNumber[i] = new int[num];
//			memset(m_pnSelectItemNumber[i], 0x00, num*sizeof(UINT));
//		}
//	}
//	ITEM* pITEM = g_pDatabase->GetServerItemInfo( nItemNum );
//	if( pITEM )
//	{
//		vector<SHOP_ITEM*>::iterator it = m_vecAllSkillData.begin();
//		while(it != m_vecAllSkillData.end())
//		{
//			if( (*it)->ItemNum == pITEM->ItemNum )
//			{
//				(*it)->ItemKind = pITEM->Kind;
//				strcpy( (*it)->ItemName, pITEM->ItemName );
//				(*it)->ItemNum = pITEM->ItemNum;
//				(*it)->MinTradeQuantity = pITEM->MinTradeQuantity;
//				(*it)->Price = pITEM->Price;
//				return;
//			}
//		}
//		SHOP_ITEM* pNew = new SHOP_ITEM;
//		pNew->ItemKind = pITEM->Kind;
//		strcpy( pNew->ItemName, pITEM->ItemName );
//		pNew->ItemNum = pITEM->ItemNum;
//		pNew->MinTradeQuantity = pITEM->MinTradeQuantity;
//		pNew->Price = pITEM->Price;
//		// 2009-01-28 by bhsohn 캐쉬샵 처리
//		pNew->CashShopIndex= 0;
//		// end 2009-01-28 by bhsohn 캐쉬샵 처리
//		InputItem( (char*)pNew );
//		delete pNew;
//	}
//}

void CINFCitySkillShop::InputItemDone()
{
	ReloadSkillItemList();
	for(int i=0;i<4;i++)
	{
		util::del_array(m_pnSelectItemNumber[i]);
		if(m_vecItemInfo[i].size() > 0)
		{
			int num = m_vecItemInfo[i].size();
			m_pnSelectItemNumber[i] = new int[num];
			memset(m_pnSelectItemNumber[i], 0x00, num*sizeof(UINT));
			sort(m_vecItemInfo[i].begin(), m_vecItemInfo[i].end(), CompareLevel());
		}

	}
	m_bInputItemDone = TRUE;
	///////////////////////////////
	// UNLOCK BUILDING BUTTON
	///////////////////////////////
}

void CINFCitySkillShop::ReloadSkillItemList()
{
	m_vecItemInfo[0].clear();
	m_vecItemInfo[1].clear();
	m_vecItemInfo[2].clear();
	m_vecItemInfo[3].clear();
	vector<SHOP_ITEM*>::iterator it = m_vecAllSkillData.begin();
	while(it != m_vecAllSkillData.end())
	{
		SHOP_ITEM* pShopSkill = *it;
		ITEM *pITEMSkill = g_pDatabase->GetServerItemInfo(pShopSkill->ItemNum);
		if(pITEMSkill == NULL)
		{
			DBGOUT("Skill(ItemNum:%d)info is NULL.\n",(pShopSkill->ItemNum));
			it++;
			continue;
		}
		if(FALSE == IS_SAME_UNITKIND(g_pShuttleChild->m_myShuttleInfo.UnitKind, pITEMSkill->ReqUnitKind))
		{
			it++;
			continue;
		}
		int nSkillLevel = g_pShuttleChild->m_pSkill->FindItemSkillLevelByBaseNum(SKILL_BASE_NUM(pITEMSkill->ItemNum));	// 2005-12-06 by ispark
		if( nSkillLevel == 0 ) // skill이 존재하지 않음
		{
			if(SKILL_LEVEL(pShopSkill->ItemNum) != 1) // 레벨 1의 스킬
			{
				it++;
				continue;
			}
		}
		else if( nSkillLevel+1 != SKILL_LEVEL(pShopSkill->ItemNum)) // skill 다음 레벨
		{
			it++;
			continue;
		}
		switch(pITEMSkill->Kind)
		{
		case ITEMKIND_SKILL_ATTACK:
			{
				m_vecItemInfo[SHOP_TAB_ATTACK].push_back(pShopSkill);
			}
			break;
		case ITEMKIND_SKILL_DEFENSE:
			{
				m_vecItemInfo[SHOP_TAB_DEFENSE].push_back(pShopSkill);
			}
			break;
		case ITEMKIND_SKILL_SUPPORT:
			{
				m_vecItemInfo[SHOP_TAB_CONTROL].push_back(pShopSkill);
			}
			break;
		case ITEMKIND_SKILL_ATTRIBUTE:
			{
				m_vecItemInfo[SHOP_TAB_ATTRIBUTE].push_back(pShopSkill);
			}
			break;
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFCitySkillShop::FindSkillShopData(SHOP_ITEM *pShopSkill)
/// \brief		스킬 데이타 아이템 검색
/// \author		ispark
/// \date		2006-01-11 ~ 2006-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SHOP_ITEM* CINFCitySkillShop::FindSkillShopData(SHOP_ITEM *pShopSkill)
{
	vector<SHOP_ITEM*>::iterator it = m_vecAllSkillData.begin();
	while(it != m_vecAllSkillData.end())
	{
		SHOP_ITEM* pitShopSkill = *it;
		if(pShopSkill->ItemNum == pitShopSkill->ItemNum)
		{
			return pShopSkill;
		}

		it++;
	}

	// 없음
	return NULL;
}
