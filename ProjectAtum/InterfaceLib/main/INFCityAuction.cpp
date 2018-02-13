// INFCityAuction.cpp: implementation of the CINFCityShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityAuction.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "FieldWinSocket.h"
#include "AtumDatabase.h"
#include "INFWindow.h"
#include "D3DHanFont.h"
#include "INFIcon.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFCityBase.h"
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Chat.h"
#include "INFCityStore.h"
#include "Interface.h"
#include "StoreData.h"
#include "INFItemInfo.h"
#include "dxutil.h"
#include "INFImageEx.h"	             // 2011. 10. 10 by jskim UI시스템 변경

#define CITY_SHOP_START_X			CITY_BASE_NPC_BOX_START_X
#define CITY_SHOP_START_Y			(CITY_BASE_NPC_BOX_START_Y - SIZE_NORMAL_WINDOW_Y)

#define SHOP_TAB_START_X			(221+28)
#define SHOP_TAB_START_Y			33
#define SHOP_TAB_NUMBER				4
#define SHOP_ITEM_SLOT_START_X		(227+28)
#define SHOP_ITEM_SLOT_START_Y		51
#define SHOP_ITEM_SLOT_SIZE_X		157
#define SHOP_ITEM_SLOT_SIZE_Y		36
#define SHOP_ICON_START_X			(230+28)
#define SHOP_ICON_START_Y			53
#define SHOP_ICON_SIZE				30
#define SHOP_ITEM_SLOT_INTERVAL		36
#define SHOP_ITEM_NUMBER_X			(308+28)//346
#define SHOP_ITEM_NUMBER_Y			70//54
#define SHOP_NUMBER_BUTTON_X		(336+28)//347
#define SHOP_NUMBER_UP_BUTTON_Y		69//53
#define SHOP_NUMBER_BUTTON_SIZE_X	8
#define SHOP_NUMBER_BUTTON_SIZE_Y	7
#define SHOP_NUMBER_DOWN_BUTTON_Y	79//63
#define SHOP_BUY_BUTTON_START_X		(344+28)
#define SHOP_BUY_BUTTON_START_Y		70
#define SHOP_BUY_BUTTON_SIZE_X		39
#define SHOP_BUY_BUTTON_SIZE_Y		14
#define SHOP_CASH_START_X			(223+28)
#define SHOP_CASH_START_Y			241
#define SHOP_OK_BUTTON_START_X		(333+28)
#define SHOP_OK_BUTTON_START_Y		240
#define SHOP_OK_BUTTON_SIZE_X		35
#define SHOP_OK_BUTTON_SIZE_Y		16
#define SHOP_CANCEL_BUTTON_START_X	(370+28)
#define SHOP_CANCEL_BUTTON_START_Y	240

#define SHOP_SCROLL_START_X			(388+28)
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
//#define SHOP_SCROLL_NUMBER(i)			(m_vecItemInfo[i].size()<=0 ? 1:m_vecItemInfo[i].size())	// i번째 탭의 scroll number
#define AUCTION_SCROLL_NUMBER(i)		(m_vecAuctionItem[i].size()<=0 ? 1:m_vecAuctionItem[i].size())	// i번째 탭의 scroll number
#define SHOP_SCROLL_INTERVAL(i)			(SHOP_SCROLL_LENGTH / AUCTION_SCROLL_NUMBER(i))

#define SHOP_ITEM_NAME_START_X		(260+28)

#define SHOP_SELL_ITEM_START_X	(227+28)
#define SHOP_SELL_ITEM_START_Y	51
#define SHOP_SELL_ITEM_END_X	(383+28)
#define SHOP_SELL_ITEM_END_Y	228

#define SHOP_TAB_B_GEAR				0
#define SHOP_TAB_I_GEAR				1
#define SHOP_TAB_M_GEAR				2
#define SHOP_TAB_A_GEAR				3

#define BOX_SIZE_X					417
#define BOX_SIZE_Y					275
#define END_OF_INVEN_X				200

// 2004-12-10 by ydkim
#define AUCTION_MY_ITEM						0
#define AUCTION_ATTACH_ITEM					1
#define AUCTION_CARD_ITEM					2
#define AUCTION_ETC_ITEM					3

// 2006-03-07 by ispark, 언어에 따라 위치 수정
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define SHOP_ITEM_NAME_START_Y		51//53
#define SHOP_ITEM_PRICE_START_Y		68//70
#else
#define SHOP_ITEM_NAME_START_Y		53
#define SHOP_ITEM_PRICE_START_Y		70
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CINFCityAuction::CINFCityAuction(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	FLOG( "CINFCityAuction(CAtumNode* pParent)" );
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
	m_pFontDate	= NULL;

	m_nCurrentTab = AUCTION_ATTACH_ITEM;
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
}

CINFCityAuction::~CINFCityAuction()
{
	FLOG( "~CINFCityAuction()" );
	int i;
	for(i=0;i<4;i++)
	{
		util::del(m_pOkButton[i]);
		util::del(m_pCancelButton[i]);
		util::del(m_pBuyButton[i]);
		util::del(m_pItemTab[i]);
		util::del_array(m_pnSelectItemNumber[i]);

		// Auction
		m_vecAuctionItem[i].clear();
	}
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
	util::del(m_pFontDate);

}


void CINFCityAuction::InitData()//UINT nShopUniqueNumber, UINT nNPCTalkIndex)
{
//	memcpy( &m_buildingInfo, &pMsg->BuildingNPCInfo, sizeof(BUILDINGNPC));
//	if(IS_BT(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
//	{
//		m_nCurrentTab = AUCTION_ATTACH_ITEM;
//	}
//	else if(IS_OT(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
//	{
//		m_nCurrentTab = AUCTION_MY_ITEM;
//	}
//	else if(IS_ST(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
//	{
//		m_nCurrentTab = AUCTION_CARD_ITEM;
//	}
//	else if(IS_DT(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind))
//	{
//		m_nCurrentTab = AUCTION_ETC_ITEM;
//	}
	for(int i=0;i<4;i++)
	{
		util::del_array(m_pnSelectItemNumber[i]);
		m_nCurrentScrollNumber[i] = 0;
		m_nCurrentSelectSlot[i] = -1;
		// Auction
		m_vecAuctionItem[i].clear();
	}
}
HRESULT CINFCityAuction::InitDeviceObjects()
{
	FLOG( "CINFCityAuction::InitDeviceObjects()" );
	DataHeader	* pDataHeader;

	int i;
	for(i=0;i<4;i++)
	{
		char buf[16];

		m_pOkButton[i] = new CINFImageEx;
		wsprintf(buf, "sauok%d",i);
		pDataHeader = FindResource(buf);
		m_pOkButton[i]->InitDeviceObjects( pDataHeader ) ;

		m_pCancelButton[i] = new CINFImageEx;
		wsprintf(buf, "shmcan0%d",i);
		pDataHeader = FindResource(buf);
		m_pCancelButton[i]->InitDeviceObjects( pDataHeader ) ;

		wsprintf(buf, "sbuy%d",i);//sbuy3은 없음.
		pDataHeader = FindResource(buf);
		if(pDataHeader)
		{
			m_pBuyButton[i] = new CINFImageEx;
			m_pBuyButton[i]->InitDeviceObjects( pDataHeader ) ;
		}

		m_pItemTab[i] = new CINFImageEx;
		wsprintf(buf, "staau%d",i);
		pDataHeader = FindResource(buf);
		m_pItemTab[i]->InitDeviceObjects( pDataHeader ) ;

	}

	m_pCash = new CINFImageEx;
	pDataHeader = FindResource("scash");
	m_pCash->InitDeviceObjects( pDataHeader ) ;

	m_pSlot = new CINFImageEx;
	pDataHeader = FindResource("auslot");
	m_pSlot->InitDeviceObjects( pDataHeader ) ;

	m_pScrollUpDown = new CINFImageEx;
	pDataHeader = FindResource("c_scrlbt");
	m_pScrollUpDown->InitDeviceObjects( pDataHeader ) ;

	m_pScrollBar = new CINFImageEx;
	pDataHeader = FindResource("c_scrlb");
	m_pScrollBar->InitDeviceObjects( pDataHeader ) ;

	m_pSelectSlot = new CINFImageEx;
	pDataHeader = FindResource("slosel");
	m_pSelectSlot->InitDeviceObjects( pDataHeader ) ;

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
	m_pFontDate = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,128,32);
	m_pFontDate->InitDeviceObjects(g_pD3dDev);

	return S_OK ;
}

HRESULT CINFCityAuction::RestoreDeviceObjects()
{
	FLOG( "CINFCityAuction::RestoreDeviceObjects()" );
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
	m_pFontDate->RestoreDeviceObjects();
	m_bRestored = TRUE;

	return S_OK ;
}

HRESULT CINFCityAuction::DeleteDeviceObjects()
{
	FLOG( "CINFCityAuction::DeleteDeviceObjects()" );
	int i;
	for(i=0;i<4;i++)
	{
		m_pOkButton[i]->DeleteDeviceObjects();
		m_pCancelButton[i]->DeleteDeviceObjects();
		if(m_pBuyButton[i])
			m_pBuyButton[i]->DeleteDeviceObjects();
		m_pItemTab[i]->DeleteDeviceObjects();
		util::del(m_pOkButton[i]);
		util::del(m_pCancelButton[i]);
		util::del(m_pBuyButton[i]);
		util::del(m_pItemTab[i]);
		
//		vector<SHOP_ITEM*>::iterator it = m_vecItemInfo[i].begin();
//		while(it != m_vecItemInfo[i].end())
//		{
//			SHOP_ITEM* pItem = *it;
//			delete pItem;
//			pItem = NULL;
//			it++;
//		}
//		m_vecItemInfo[i].clear();

		// Auction
		m_vecAuctionItem[i].clear();
	}
	m_pCash->DeleteDeviceObjects();
	m_pSlot->DeleteDeviceObjects();

	util::del(m_pItemTab[3]);
	util::del(m_pCash);
	util::del(m_pSlot);

	m_pScrollUpDown->DeleteDeviceObjects();
	util::del(m_pScrollUpDown);
	m_pScrollBar->DeleteDeviceObjects();
	util::del(m_pScrollBar);
	m_pSelectSlot->DeleteDeviceObjects();
	util::del(m_pSelectSlot);
	for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
	{
		m_pFontItemName[i]->DeleteDeviceObjects();
		util::del(m_pFontItemName[i]);
		m_pFontItemPrice[i]->DeleteDeviceObjects();
		util::del(m_pFontItemPrice[i]);
		m_pFontItemNumber[i]->DeleteDeviceObjects();
		util::del(m_pFontItemNumber[i]);
	}

	m_pFontTotalPrice->DeleteDeviceObjects();
	util::del(m_pFontTotalPrice);
	m_pFontDate->DeleteDeviceObjects();
	util::del(m_pFontDate);
//	memset(&m_buildingInfo, 0x00, sizeof(BUILDINGNPC));
	m_bInvalidated = FALSE;
	return S_OK ;
}


HRESULT CINFCityAuction::InvalidateDeviceObjects()
{
	FLOG( "CINFCityAuction::InvalidateDeviceObjects()" );
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
	m_pFontDate->InvalidateDeviceObjects();
	m_bRestored = FALSE;
	m_bInvalidated = TRUE;

	return S_OK ;
}


void CINFCityAuction::Tick()
{
	FLOG( "CINFCityAuction::Tick()" );


}

void CINFCityAuction::Render()
{
	FLOG( "CINFCityAuction::Render()" );
	// 기본틀 렌더링
	m_pItemTab[m_nCurrentTab]->Move(CITY_SHOP_START_X+SHOP_TAB_START_X, CITY_SHOP_START_Y+SHOP_TAB_START_Y);
	m_pItemTab[m_nCurrentTab]->Render();

	int nSelectNumber = 0;
	
	// Auction
	vector<AuctionItem_t>::iterator it = m_vecAuctionItem[m_nCurrentTab].begin();
	int i;
	for(i=0;i<m_nCurrentScrollNumber[m_nCurrentTab];i++)
	{
		if(it != m_vecAuctionItem[m_nCurrentTab].end())
		{
			it++;
			nSelectNumber++;
		}
		else
		{
			break;
		}
	}
	
//	if(m_pnSelectItemNumber[AUCTION_ATTACH_ITEM])
	{
		for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
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
			// Auction
			if(it != m_vecAuctionItem[m_nCurrentTab].end())
			{
				AuctionItem_t* pItemAuction = &(*it);
				char buf[64];
//				wsprintf( buf, "%08d", pItemAuction->AuctionItemGeneral.ItemNum);
//				wsprintf( buf, "%08d", pItemAuction->pItemInfo->ItemNum);			// 2005-08-23 by ispark


				// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

				//wsprintf( buf, "%08d", pItemAuction->pItemInfo->ItemInfo->SourceIndex);
				if( pItemAuction->pItemInfo->ShapeItemNum )
					wsprintf( buf, "%08d", pItemAuction->pItemInfo->ItemInfo->SourceIndex);
				else
				{
					ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemAuction->pItemInfo->ShapeItemNum );
					if( pShapeItem )
						wsprintf( buf, "%08d", pShapeItem->SourceIndex);
					else
						wsprintf( buf, "%08d", pItemAuction->pItemInfo->ItemInfo->SourceIndex);
				}


				// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


				((CINFGameMain*)m_pParent)->m_pIcon->SetIcon( buf,CITY_SHOP_START_X+SHOP_ICON_START_X,CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+1, 1.0f );
				((CINFGameMain*)m_pParent)->m_pIcon->Render();
				
				char temp1[64];
				char temp2[64];
				wsprintf( temp1, "%d", pItemAuction->AuctionPrice );
				MakeCurrencySeparator( temp2, temp1, 3, ',' );
				//m_pFontItemName[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X, CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, pItemAuction->AuctionItemGeneral.ItemInfo->ItemName, 0L);
				m_pFontItemName[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X, CITY_SHOP_START_Y+SHOP_ITEM_NAME_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, pItemAuction->pItemInfo->ItemInfo->ItemName, 0L);
				// 가격 표시
				wsprintf(buf, "%7s", temp2);
				m_pFontItemPrice[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NAME_START_X,CITY_SHOP_START_Y+SHOP_ITEM_PRICE_START_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
				// 남은 날자 표시
				//m_pFontDate

				it++;				
				//wsprintf(buf, "%3d", m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]);
				//m_pFontItemNumber[i]->DrawText( CITY_SHOP_START_X+SHOP_ITEM_NUMBER_X, CITY_SHOP_START_Y+SHOP_ITEM_NUMBER_Y+SHOP_ITEM_SLOT_INTERVAL*i, GUI_FONT_COLOR, buf, 0L);
			}

		}
//		m_pCash->Move(CITY_SHOP_START_X+SHOP_CASH_START_X, CITY_SHOP_START_Y+SHOP_CASH_START_Y);
//		m_pCash->Render();
//		char temp1[64];
//		char temp2[64];
//		wsprintf( temp1, "%d", GetTotalPrice() );
//		MakeCurrencySeparator( temp2, temp1, 3, ',' );
//		m_pFontTotalPrice->DrawText( CITY_SHOP_START_X+SHOP_CASH_START_X, CITY_SHOP_START_Y+SHOP_CASH_START_Y, GUI_FONT_COLOR, temp2, 0L);
	}

	m_pOkButton[m_nOkButtonState]->Move(CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y);
	m_pOkButton[m_nOkButtonState]->Render();
	m_pCancelButton[m_nCancelButtonState]->Move(CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X, CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y);
	m_pCancelButton[m_nCancelButtonState]->Render();

	//////////////////
	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y);
	m_pScrollUpDown->Render();
	m_pScrollUpDown->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X, CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y);
	m_pScrollUpDown->Render();

	m_pScrollBar->Move( CITY_SHOP_START_X+SHOP_SCROLL_START_X,
		CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab));
	m_pScrollBar->Render();
	//////////////////

}


int CINFCityAuction::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCityAuction::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );
	if( g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_AUCTION_INPUT_ITEM ) ||
		g_pGameMain->m_pInfWindow->IsExistMsgBox( _Q_SHOP_SELL_ENERGY ) )
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
			if( pt.x > CITY_SHOP_START_X &&
				pt.x < CITY_SHOP_START_X+BOX_SIZE_X &&
				pt.y > CITY_SHOP_START_Y &&
				pt.y < CITY_SHOP_START_Y+BOX_SIZE_Y )
			{
				if( pt.x> CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X && 
					pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
					pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
				{
					if(m_nOkButtonState != BUTTON_STATE_DOWN)
					{
						m_nOkButtonState = BUTTON_STATE_UP;
					}
				}
				else
				{
					m_nOkButtonState = BUTTON_STATE_NORMAL;
				}
				if( pt.x> CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X && 
					pt.x<CITY_SHOP_START_X+SHOP_CANCEL_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
					pt.y>CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y &&
					pt.y<CITY_SHOP_START_Y+SHOP_CANCEL_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
				{
					if(m_nCancelButtonState != BUTTON_STATE_DOWN)
					{
						m_nCancelButtonState = BUTTON_STATE_UP;
					}
				}
				else
				{
					m_nCancelButtonState = BUTTON_STATE_NORMAL;
				}

				for(int i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
				{
					if( pt.x > CITY_SHOP_START_X+SHOP_ICON_START_X && 
						pt.x < CITY_SHOP_START_X+SHOP_ICON_START_X+SHOP_ICON_SIZE &&
						pt.y > CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
						pt.y < CITY_SHOP_START_Y+SHOP_ICON_START_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_ICON_SIZE)
					{					
						int nCount = -m_nCurrentScrollNumber[m_nCurrentTab];
						vector<AuctionItem_t>::iterator it = m_vecAuctionItem[m_nCurrentTab].begin();
						while (it != m_vecAuctionItem[m_nCurrentTab].end()) 
						{
							if(nCount == i)
							{
								AuctionItem_t &aucTemp = *it;
								// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
								//g_pGameMain->m_pItemInfo->SetItemInfoUser( aucTemp.pItemInfo, pt.x, pt.y );								
								g_pGameMain->SetItemInfoUser( aucTemp.pItemInfo, pt.x, pt.y );								
								// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							}			
							nCount++;
							it++;
						}						
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
			int i = 0;
			// 구매 하기
			if( pt.x>CITY_SHOP_START_X+ SHOP_OK_BUTTON_START_X && 
				pt.x<CITY_SHOP_START_X+SHOP_OK_BUTTON_START_X+SHOP_OK_BUTTON_SIZE_X &&
				pt.y>CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y && 
				pt.y<CITY_SHOP_START_Y+SHOP_OK_BUTTON_START_Y+SHOP_OK_BUTTON_SIZE_Y)
			{
				m_nOkButtonState = BUTTON_STATE_DOWN;
			}
			// 등록 취소하기
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
			for(i=0;i<SHOP_TAB_NUMBER;i++)
			{
				if( pt.x>CITY_SHOP_START_X+SHOP_TAB_START_X+SHOP_TAB_BUTTON_SIZE_X*i && 
					pt.x<CITY_SHOP_START_X+SHOP_TAB_START_X+SHOP_TAB_BUTTON_SIZE_X*(i+1) &&
					pt.y>CITY_SHOP_START_Y+SHOP_TAB_START_Y && 
					pt.y<CITY_SHOP_START_Y+SHOP_TAB_START_Y+SHOP_TAB_BUTTON_SIZE_Y)
				{
					int nOldCurrent = m_nCurrentTab;
					m_nCurrentTab = i+1;
					if(m_nCurrentTab>SHOP_TAB_NUMBER-1) m_nCurrentTab = 0;					
					if(nOldCurrent != m_nCurrentTab)
					{
						m_vecAuctionItem[m_nCurrentTab].clear();
						AuctionItemInfoView(m_nCurrentTab);
					}
				}
			}
//			if( pt.x > CITY_SHOP_START_X+SHOP_SCROLL_START_X && 
//				pt.x < CITY_SHOP_START_X+SHOP_SCROLL_START_X+SHOP_SCROLL_SIZE_X)
//			{
//				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y && 
//					pt.y<CITY_SHOP_START_Y+SHOP_SCROLL_UP_START_Y+SHOP_SCROLL_SIZE_Y)
//				{
//					m_nCurrentScrollNumber[m_nCurrentTab]--;
//					if(m_nCurrentScrollNumber[m_nCurrentTab]<0)
//						m_nCurrentScrollNumber[m_nCurrentTab] = 0;
//				}
//				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y && 
//					pt.y<CITY_SHOP_START_Y+SHOP_SCROLL_DOWN_START_Y+SHOP_SCROLL_SIZE_Y)
//				{
//					m_nCurrentScrollNumber[m_nCurrentTab]++;
//					if(m_nCurrentScrollNumber[m_nCurrentTab]>SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab))
//						m_nCurrentScrollNumber[m_nCurrentTab] = SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab);
//				}
//				if( pt.y > CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab) &&
//					pt.y < CITY_SHOP_START_Y+SHOP_SCROLL_BAR_START_Y+m_nCurrentScrollNumber[m_nCurrentTab]*SHOP_SCROLL_INTERVAL(m_nCurrentTab)+SHOP_SCROLL_BAR_SIZE_Y)
//				{
//					m_bScrollLock = TRUE;
//					m_ptMouse = pt;
//				}
//			}
//			if( pt.x > CITY_SHOP_START_X+SHOP_NUMBER_BUTTON_X && 
//				pt.x < CITY_SHOP_START_X+SHOP_NUMBER_BUTTON_X+SHOP_NUMBER_BUTTON_SIZE_X)
//			{
//				for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
//				{
//					if( pt.y>CITY_SHOP_START_Y+SHOP_NUMBER_UP_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
//						pt.y<CITY_SHOP_START_Y+SHOP_NUMBER_UP_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
//					{
//						if(m_vecAuctionItem[m_nCurrentTab].size() > i+m_nCurrentScrollNumber[m_nCurrentTab] )
//						{
//							int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]];
//							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] += 1;
//								//m_vecAuctionItem[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
//							if( GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//							{//더한 후 price가 원래돈보다 많으면 취소해야 한다.
//								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = temp;
//							}
//						}
//					}
//					if( pt.y>CITY_SHOP_START_Y+SHOP_NUMBER_DOWN_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i &&
//						pt.y<CITY_SHOP_START_Y+SHOP_NUMBER_DOWN_BUTTON_Y+SHOP_ITEM_SLOT_INTERVAL*i+SHOP_NUMBER_BUTTON_SIZE_Y)
//					{
//						if(m_vecAuctionItem[m_nCurrentTab].size() > i+m_nCurrentScrollNumber[m_nCurrentTab] )
//						{
//							int temp = m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]];
//							m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] -= 
//								m_vecAuctionItem[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]->MinTradeQuantity;
//							if(m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]]<0)
//								m_pnSelectItemNumber[m_nCurrentTab][i+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
//						}
//					}
//				}
//			}
			if( pt.x > CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_ITEM_SLOT_START_X + SHOP_ITEM_SLOT_SIZE_X)
			{
				for(i=0;i<SHOP_ITEM_SLOT_NUMBER;i++)
				{
					if( pt.y > CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_SIZE_Y*i &&
						pt.y < CITY_SHOP_START_Y+SHOP_ITEM_SLOT_START_Y+SHOP_ITEM_SLOT_SIZE_Y*(i+1))
					{
						m_nCurrentSelectSlot[m_nCurrentTab] = m_nCurrentScrollNumber[m_nCurrentTab]+i;
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
					((CINFGameMain*)m_pParent)->SetItemInfo( 0, NULL, 0, 0);
					m_nCurrentScrollNumber[m_nCurrentTab]++;
					if(m_nCurrentScrollNumber[m_nCurrentTab]>SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab))
						m_nCurrentScrollNumber[m_nCurrentTab] = SHOP_SCROLL_LENGTH/SHOP_SCROLL_INTERVAL(m_nCurrentTab);
					return INF_MSGPROC_BREAK;
				}
				else
				{
					((CINFGameMain*)m_pParent)->SetItemInfo( 0, NULL, 0, 0);
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
				if(m_nOkButtonState == BUTTON_STATE_DOWN)
				{
					OnOkButtonClicked();
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

			if( g_pGameMain->m_pInven->m_pSelectItem &&
				pt.x > CITY_SHOP_START_X+SHOP_SELL_ITEM_START_X && 
				pt.x < CITY_SHOP_START_X+SHOP_SELL_ITEM_END_X &&
				pt.y > CITY_SHOP_START_Y+SHOP_SELL_ITEM_START_Y &&
				pt.y<CITY_SHOP_START_Y+SHOP_SELL_ITEM_END_Y)
			{
				if(g_pGameMain->m_pInven->m_pSelectItem->Wear == WEAR_NOT_ATTACHED )
				{
					if( IS_COUNTABLE_ITEM(g_pGameMain->m_pInven->m_pSelectItem->Kind))
					{
						g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_AUCTION_0001, COLOR_SYSTEM );	//"현재 이아이템은 물품대행을 할 수 없습니다."					
					}
					else
					{
						if(m_nCurrentTab == AUCTION_MY_ITEM)
						{
							char buf[128];
							ITEM *pITEM = g_pGameMain->m_pInven->m_pSelectItem->GetItemInfo();
							if(pITEM)
							{
								wsprintf( buf, STRMSG_C_AUCTION_0002, pITEM->ItemName);//"%s를(을) 물품대행 상점에 올리시겠습니까?"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_AUCTION_INPUT_ITEM,
									(DWORD)g_pGameMain->m_pInven->m_pSelectItem->UniqueNumber);
							}
							else
							{
								g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_AUCTION_0003, COLOR_SYSTEM );//"서버로부터 정보 받는중...다시 시도 하세요."
							}
						}
						else
						{
							g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_AUCTION_0004, COLOR_SYSTEM );//"등록에서만 아이템을 등록 할 수 있습니다."
						}
					}
				}
				else
				{
					g_pGameMain->m_pInfWindow->AddMsgBox( STRMSG_C_AUCTION_0005, _MESSAGE );//"장착된 아이템은 팔 수 없습니다."
				}
			}
			m_bScrollLock = FALSE;
			((CINFGameMain*)m_pParent)->SetItemInfo( 0, 0, 0, 0 );
		}
		break;
	case WM_CHAR:
		{
			char code = wParam;
//			if(m_pnSelectItemNumber[m_nCurrentTab] && g_pD3dApp->m_bChatMode == FALSE)
//			{
//				if(code >= '0' && code <= '9' && m_nCurrentSelectSlot[m_nCurrentTab]>=0)
//				{
//					int temp = m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]];
//					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] *= 10;
//					m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] += code - '0';
//					if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = temp;
//				}
//				else if( code == VK_BACK )
//				{
//					if( m_nCurrentSelectSlot[m_nCurrentTab] >= 0 && 
//						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] > 0 )
//					{
//						m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] /= 10;
//					}
//				}
//			}
			if( code == VK_ESCAPE )
			{
//				OnCancelButtonClicked();
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityAuction::OnOkButtonClicked()
{
	FLOG( "CINFCityAuction::OnOkButtonClicked()" );
	if(m_nCurrentTab == AUCTION_MY_ITEM)
	{
		int nCount = 0;
		vector<AuctionItem_t>::iterator itA = m_vecAuctionItem[AUCTION_MY_ITEM].begin();
		while (itA != m_vecAuctionItem[AUCTION_MY_ITEM].end()) 
		{
			if(nCount == m_nCurrentSelectSlot[m_nCurrentTab])
			{
				AuctionItem_t &tempAu = *itA;
				g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_AUCTION_0006, COLOR_SYSTEM );		//"현재의 위치에서는 구매를 할 수 없습니다."
			}
			nCount++;
			itA++;
		}		
	}
	else
	{
		int nCount = 0;
		vector<AuctionItem_t>::iterator it = m_vecAuctionItem[m_nCurrentTab].begin();
		while (it != m_vecAuctionItem[m_nCurrentTab].end()) 
		{
			if(nCount == m_nCurrentSelectSlot[m_nCurrentTab])
			{
				MSG_FC_AUCTION_BUY_ITEM sMsg;
				AuctionItem_t &aucTemp = *it;
				sMsg.ItemUID = aucTemp.pItemInfo->UniqueNumber;
					
				g_pFieldWinSocket->SendMsg( T_FC_AUCTION_BUY_ITEM, (char*)&sMsg, sizeof(sMsg) );
			}			
			nCount++;
			it++;
		}

	}
}

void CINFCityAuction::OnCancelButtonClicked()
{
	if(m_nCurrentTab == AUCTION_MY_ITEM)
	{
		int nCount = 0;
		vector<AuctionItem_t>::iterator it = m_vecAuctionItem[AUCTION_MY_ITEM].begin();
		while (it != m_vecAuctionItem[AUCTION_MY_ITEM].end()) 
		{
			if(nCount == m_nCurrentSelectSlot[m_nCurrentTab])
			{
				AuctionItem_t &aucTemp = *it;
				char buf[512];
				
				wsprintf( buf, STRMSG_C_AUCTION_0007, aucTemp.pItemInfo->ItemInfo->ItemName);//"%s를(을) 등록에서 취소하시겠습니까?"
				g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_AUCTION_DELETE_ITEM,
//								(DWORD)aucTemp.AuctionItemGeneral.UniqueNumber);
								(DWORD)aucTemp.pItemInfo->UniqueNumber);
			}
			nCount++;
			it++;
		}

	}
	else
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_AUCTION_0008, COLOR_SYSTEM );		//"경매 아이템 삭제는 현재 위치에서는 할 수 없습니다."
	}
}

void CINFCityAuction::OnBuyButtonClicked(int nSlotNumber)
{
	FLOG( "CINFCityAuction::OnBuyButtonClicked(int nSlotNumber)" );
//	vector<AuctionItem_t*>::iterator it = m_vecAuctionItem[m_nCurrentTab].begin();
//	for(int i=0;i<m_nCurrentScrollNumber[m_nCurrentTab]+nSlotNumber;i++)
//	{
//		if(it != m_vecAuctionItem[m_nCurrentTab].end())
//		{
//			it++;
//		}
//		else
//		{
//			break;
//		}
//	}
//	if(it != m_vecAuctionItem[m_nCurrentTab].end())
//	{
//		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]] == 0)
//		{
//			m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]] = 1;
//			if(GetTotalPrice() > ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi())
//			{
//				m_pnSelectItemNumber[m_nCurrentTab][m_nCurrentSelectSlot[m_nCurrentTab]] = 0;
//				g_pD3dApp->m_pChat->CreateChatChild( "아이템을 구입하기 위한 스피가 부족합니다",COLOR_ERROR );
//			}
//		}
//		if(m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]]>0)
//		{
//			BuyItem((*it)->ItemNum, m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]]);
//		}
//		m_pnSelectItemNumber[m_nCurrentTab][nSlotNumber+m_nCurrentScrollNumber[m_nCurrentTab]] = 0;
//	}
}

void CINFCityAuction::BuyItem(int nItemNum, int nAmount)
{
	FLOG( "CINFCityAuction::BuyItem(int nItemNum, int nAmount)" );

	// 2005-03-07 by jschoi
	g_pD3dApp->m_bRequestEnable = FALSE;

	MSG_FC_SHOP_BUY_ITEM sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	sMsg.BuildingIndex = m_pBuildingInfo->BuildingIndex;
	sMsg.ItemNum = nItemNum;
	sMsg.Amount = nAmount;
	g_pFieldWinSocket->SendMsg( T_FC_SHOP_BUY_ITEM, (char*)&sMsg, sizeof(sMsg) );
}


void CINFCityAuction::InputItem(MSG_FC_AUCTION_INSERT_ITEM* pMsg)
{
	FLOG( "CINFCityAuction::InputItem(char* pMsg)" );
	if(pMsg)
	{
		ITEM_GENERAL *item = &pMsg->AuctionItemGeneral;
		if(item)
		{
			switch(pMsg->AuctionKind) 
			{
			case AUCTION_KIND_MY_ITEM:
				{// 내가 등록한 물품
					AuctionItem_t auctionItem;
					//auctionItem.AuctionItemGeneral		= pMsg->AuctionItemGeneral;
					auctionItem.pItemInfo				= new CItemInfo(&pMsg->AuctionItemGeneral);
					auctionItem.AuctionKind				= pMsg->AuctionKind;
					auctionItem.AuctionPrice			= pMsg->AuctionPrice;
					auctionItem.AuctionStartDate		= pMsg->AuctionStartDate;
					strncpy(auctionItem.SellerCharacterName, pMsg->SellerCharacterName, sizeof(pMsg->SellerCharacterName));
//					auctionItem.AuctionItemGeneral.ItemInfo = g_pDatabase->GetServerItemInfo(pMsg->AuctionItemGeneral.ItemNum);
					m_vecAuctionItem[AUCTION_MY_ITEM].push_back(auctionItem);
				}
				break;
			case AUCTION_KIND_ATTACH:
				{// 장착류
					AuctionItem_t auctionItem;
					//auctionItem.AuctionItemGeneral		= pMsg->AuctionItemGeneral;
					auctionItem.pItemInfo				= new CItemInfo(&pMsg->AuctionItemGeneral);
					auctionItem.AuctionKind				=  pMsg->AuctionKind;
					auctionItem.AuctionPrice			= pMsg->AuctionPrice;
					auctionItem.AuctionStartDate		= pMsg->AuctionStartDate;
					strncpy(auctionItem.SellerCharacterName, pMsg->SellerCharacterName, sizeof(pMsg->SellerCharacterName));
//					auctionItem.AuctionItemGeneral.ItemInfo = g_pDatabase->GetServerItemInfo(pMsg->AuctionItemGeneral.ItemNum);
					m_vecAuctionItem[AUCTION_ATTACH_ITEM].push_back(auctionItem);
				}
				break;
			case AUCTION_KIND_CARD:
				{// 카드류
					AuctionItem_t auctionItem;
					//auctionItem.AuctionItemGeneral		= pMsg->AuctionItemGeneral;
					auctionItem.pItemInfo				= new CItemInfo(&pMsg->AuctionItemGeneral);
					auctionItem.AuctionKind				= pMsg->AuctionKind;
					auctionItem.AuctionPrice			= pMsg->AuctionPrice;
					auctionItem.AuctionStartDate		= pMsg->AuctionStartDate;
					strncpy(auctionItem.SellerCharacterName, pMsg->SellerCharacterName, sizeof(pMsg->SellerCharacterName));
//					auctionItem.AuctionItemGeneral.ItemInfo = g_pDatabase->GetServerItemInfo(pMsg->AuctionItemGeneral.ItemNum);
					m_vecAuctionItem[AUCTION_CARD_ITEM].push_back(auctionItem);
				}
				break;
			default:
				{// 기타
					AuctionItem_t auctionItem;
					//auctionItem.AuctionItemGeneral	= pMsg->AuctionItemGeneral;
					auctionItem.pItemInfo				= new CItemInfo(&pMsg->AuctionItemGeneral);
					auctionItem.AuctionKind				= pMsg->AuctionKind;
					auctionItem.AuctionPrice			= pMsg->AuctionPrice;
					auctionItem.AuctionStartDate		= pMsg->AuctionStartDate;
					strncpy(auctionItem.SellerCharacterName, pMsg->SellerCharacterName, sizeof(pMsg->SellerCharacterName));
//					auctionItem.AuctionItemGeneral.ItemInfo = g_pDatabase->GetServerItemInfo(pMsg->AuctionItemGeneral.ItemNum);
					m_vecAuctionItem[AUCTION_ETC_ITEM].push_back(auctionItem);
				}
			}

		}
	}
}

void CINFCityAuction::AuctionItemInfoView(int nTab)
{
	switch(nTab) 
	{
	case AUCTION_MY_ITEM:
		{// 내가 등록한 물품
			MSG_FC_AUCTION_GET_ITEM_LIST sMsg;
			sMsg.AuctionKind  = AUCTION_KIND_MY_ITEM;
			sMsg.CharacterUID = g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
			g_pFieldWinSocket->SendMsg( T_FC_AUCTION_GET_ITEM_LIST, (char*)&sMsg, sizeof(sMsg));
		}
		break;
	case AUCTION_ATTACH_ITEM:
		{// 장착류
			MSG_FC_AUCTION_GET_ITEM_LIST sMsg;
			sMsg.AuctionKind  = AUCTION_KIND_ATTACH;
			sMsg.CharacterUID = 0;
			g_pFieldWinSocket->SendMsg( T_FC_AUCTION_GET_ITEM_LIST, (char*)&sMsg, sizeof(sMsg));
		}
		break;
	case AUCTION_CARD_ITEM:
		{// 카드류
			MSG_FC_AUCTION_GET_ITEM_LIST sMsg;
			sMsg.AuctionKind  = AUCTION_KIND_CARD;
			sMsg.CharacterUID = 0;
			g_pFieldWinSocket->SendMsg( T_FC_AUCTION_GET_ITEM_LIST, (char*)&sMsg, sizeof(sMsg));
		}
		break;
	default:
		{// 기타
			MSG_FC_AUCTION_GET_ITEM_LIST sMsg;
			sMsg.AuctionKind  = AUCTION_KIND_ETC;
			sMsg.CharacterUID = 0;
			g_pFieldWinSocket->SendMsg( T_FC_AUCTION_GET_ITEM_LIST, (char*)&sMsg, sizeof(sMsg));
		}
	}
}


void CINFCityAuction::AddEnchantItem(BYTE nKind, UID64_t nUniqueNumber, INT nEnchantItemNum)
{
	int nKindNum = 0;
	switch(nKind)
	{
	case AUCTION_KIND_MY_ITEM:
		{
			nKindNum = AUCTION_MY_ITEM;
		}
		break;
	case AUCTION_KIND_ATTACH:
		{
			nKindNum = AUCTION_ATTACH_ITEM;
		}
		break;
	case AUCTION_KIND_CARD:
		{
			nKindNum = AUCTION_CARD_ITEM;
		}
		break;
	default:
		{
			nKindNum = AUCTION_KIND_ETC;
		}		

	}
		//== AUCTION_KIND_MY_ITEM)

	vector<AuctionItem_t>::iterator it = m_vecAuctionItem[nKindNum].begin();
	while(it != m_vecAuctionItem[nKindNum].end())
	{		
		if(it->pItemInfo->UniqueNumber == nUniqueNumber)
		{
			it->pItemInfo->AddEnchantItem( nEnchantItemNum );
//			ITEM* pEnchantITEM = g_pDatabase->GetServerItemInfo( nEnchantItemNum );
//			if( pEnchantITEM )
//			{
//				if( pEnchantITEM->DestParameter1 != DES_NULL )
//				{
//					it->pItemInfo->PutEnchant(pEnchantITEM->DestParameter1, pEnchantITEM->ParameterValue1);
//				}
//				if( pEnchantITEM->DestParameter2 != DES_NULL )
//				{
//					it->pItemInfo->PutEnchant(pEnchantITEM->DestParameter2, pEnchantITEM->ParameterValue2);
//				}
//				if( pEnchantITEM->DestParameter3 != DES_NULL )
//				{
//					it->pItemInfo->PutEnchant(pEnchantITEM->DestParameter3, pEnchantITEM->ParameterValue3);
//				}
//				if( pEnchantITEM->DestParameter4 != DES_NULL )
//				{
//					it->pItemInfo->PutEnchant(pEnchantITEM->DestParameter4, pEnchantITEM->ParameterValue4);
//				}
//			}
			break;
		}
		it++;
	}
}


void CINFCityAuction::InputItemDone()
{
	FLOG( "CINFCityAuction::InputItemDone()" );
	for(int i=0;i<4;i++)
	{
		util::del_array(m_pnSelectItemNumber[i]);
		if(m_vecAuctionItem[i].size() > 0)
		{
			int num = m_vecAuctionItem[i].size();
			m_pnSelectItemNumber[i] = new int[num];
			memset(m_pnSelectItemNumber[i], 0x00, num*sizeof(UINT));
		}

	}
	m_bInputItemDone = TRUE;
	///////////////////////////////
	// UNLOCK BUILDING BUTTON
	///////////////////////////////
}

