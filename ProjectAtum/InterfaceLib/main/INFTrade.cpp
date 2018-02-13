// INFTrade.cpp: implementation of the CINFTrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFTrade.h"
#include "AtumApplication.h"
#include "INFWindow.h"
#include "StoreData.h"
#include "EnemyData.h"
#include "INFItemInfo.h"
#include "INFIcon.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFGameMain.h"
#include "AtumDatabase.h"
#include "SceneData.h"
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "Interface.h"
#include "dxutil.h"
#include "ItemInfo.h"
#include "Chat.h"
#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2007-12-05 by dgwoo 변경.
//#define INVEN_SPI_START_X		62
//#define INVEN_SPI_START_Y		232
//#define INVEN_SPI_WIDTH			117
//#define INVEN_SPI_HEIGHT		18

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경

#define INVEN_SPI_START_X		327
#define INVEN_SPI_START_Y		223
#define INVEN_SPI_WIDTH			90
#define INVEN_SPI_HEIGHT		18

#define TRADE_OTHER_SPI_START_X		252
#define TRADE_MY_SPI_START_X		252
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define TRADE_OTHER_SPI_START_Y		118
#define TRADE_MY_SPI_START_Y		238
#else
#define TRADE_OTHER_SPI_START_Y		118//116
#define TRADE_MY_SPI_START_Y		238//238
#endif


#define TRADE_MYSTORE_START_X		226
#define TRADE_MYSTORE_START_Y		171
//#define TRADE_MYSTORE_START_Y		(169+52)// 2008-08-22 by bhsohn EP3 인벤토리 처리
#define TRADE_STORE_WIDTH			187
#define TRADE_STORE_HEIGHT			62
#define TRADE_OTHERSTORE_START_Y	50
#define TRADE_SLOT_SIZE				30
#define TRADE_X_NUMBER				6
#define TRADE_Y_NUMBER				2
#define TRADE_SLOT_INTERVAL_X		32	// 2015-06-27 Future, increased to fit the slots
#define TRADE_SLOT_INTERVAL_Y		32

#define RENEW_TRADE_WIDTH			231
#define RENEW_TRADE_INVEN_NUM		6


// 2008-08-22 by bhsohn EP3 인벤토리 처리
// 거래 승인
#define TRADE_TRADE_OK_X		363
#define TRADE_TRADE_OK_Y		236
#define	TRADE_TRADE_OK_W		28
#define	TRADE_TRADE_OK_H		28
// 거래 취소
#define TRADE_TRADE_CANCEL_X		400
#define TRADE_TRADE_CANCEL_Y		236
#define	TRADE_TRADE_CANCEL_W		28
#define	TRADE_TRADE_CANCEL_H		28

// 인벤 위치
#define	TRADE_BK_POS_Y				(DEAULT_WINDOW_POS_Y-115)
// end 2008-08-22 by bhsohn EP3 인벤토리 처리

#else
#define INVEN_SPI_START_X		327
#define INVEN_SPI_START_Y		223
#define INVEN_SPI_WIDTH			90
#define INVEN_SPI_HEIGHT		18

#define TRADE_OTHER_SPI_START_X		212
#define TRADE_MY_SPI_START_X		212
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
#define TRADE_OTHER_SPI_START_Y		114
#define TRADE_MY_SPI_START_Y		236
#else
#define TRADE_OTHER_SPI_START_Y		114//116
#define TRADE_MY_SPI_START_Y		236//238
#endif


#define TRADE_MYSTORE_START_X		211
#define TRADE_MYSTORE_START_Y		169
//#define TRADE_MYSTORE_START_Y		(169+52)// 2008-08-22 by bhsohn EP3 인벤토리 처리
#define TRADE_STORE_WIDTH			187
#define TRADE_STORE_HEIGHT			62
#define TRADE_OTHERSTORE_START_Y	47
#define TRADE_SLOT_SIZE				30
#define TRADE_X_NUMBER				6
#define TRADE_Y_NUMBER				2
#define TRADE_SLOT_INTERVAL_X		31
#define TRADE_SLOT_INTERVAL_Y		32

#define RENEW_TRADE_WIDTH			231
#define RENEW_TRADE_INVEN_NUM		6


// 2008-08-22 by bhsohn EP3 인벤토리 처리
// 거래 승인
#define TRADE_TRADE_OK_X		324
#define TRADE_TRADE_OK_Y		236
#define	TRADE_TRADE_OK_W		35
#define	TRADE_TRADE_OK_H		16
// 거래 취소
#define TRADE_TRADE_CANCEL_X		361
#define TRADE_TRADE_CANCEL_Y		236
#define	TRADE_TRADE_CANCEL_W		35
#define	TRADE_TRADE_CANCEL_H		16

// 인벤 위치
#define	TRADE_BK_POS_Y				(DEAULT_WINDOW_POS_Y-115)
// end 2008-08-22 by bhsohn EP3 인벤토리 처리
#endif


CINFTrade::CINFTrade(CAtumNode* pParent)
{
	m_pTralog = NULL;
	m_pTrame = NULL;
	m_pTrayou = NULL;
	m_bTrading = FALSE;

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	m_pTrameComple = NULL;
	m_pTrayouComple = NULL;	
	m_bTraMeComple = m_bTraYouComple = FALSE;
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	m_nTradeButtonState[0] = BUTTON_STATE_NORMAL;
	m_nTradeButtonState[1] = BUTTON_STATE_NORMAL;
	int i;
	for(i=0;i<3;i++)
	{
		m_pTradeOkButton[i] = NULL;
		m_pTradeCancelButton[i] = NULL;
	}
	m_pParent = pParent;
	memset(m_strTradeCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	m_pFontCharacterName[0] = NULL;
	m_pFontCharacterName[1] = NULL;
	for(i=0;i<2;i++)
	{
		m_pFontItemNum[i] = NULL;
	}
	m_nTradeCharacterUniqueNumber = 0;
	m_pTempItem = NULL;
	m_nMyStoreScroll = 0;
	m_nOtherStoreScroll = 0;
	m_nMyStoreSpi = 0;
	m_nOtherStoreSpi = 0;
	m_pFontStoreSpi[0] = NULL;
	m_pFontStoreSpi[1] = NULL;
	m_bRestored = FALSE;
	m_bInvalidated = FALSE;
}	

CINFTrade::~CINFTrade()
{
	int i;
	for(i=0;i<3;i++)
	{
		util::del(m_pTradeOkButton[i]);
		util::del(m_pTradeCancelButton[i]);
	}
	util::del(m_pTralog );
	util::del(m_pTrame );
	util::del(m_pTrayou );
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	util::del(m_pTrameComple);
	util::del(m_pTrayouComple);
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리


	util::del(m_pFontCharacterName[0] );
	util::del(m_pFontCharacterName[1] );
	for(i=0;i<2;i++)
	{
		util::del(m_pFontItemNum[i]);
	}

	util::del(m_pFontStoreSpi[0] );
	util::del(m_pFontStoreSpi[1] );

	vector<CItemInfo*>::iterator it = m_vecMyStore.begin();
	while(it != m_vecMyStore.end())
	{
		CItemInfo* pItem = *it;
		it++;
		util::del(pItem); 
	}
	it = m_vecOtherStore.begin();
	while(it != m_vecOtherStore.end())
	{
		CItemInfo* pItem = *it;
		it++;
		util::del(pItem);
	}
//	map<LONGLONG, ITEM_ENCHANT*>::iterator it2 = m_mapOtherEnchant.begin();
//	while(it2 != m_mapOtherEnchant.end())
//	{
//		ITEM_ENCHANT* p = it2->second;
//		it2++;
//		util::del(p);
//	}

}

HRESULT CINFTrade::InitDeviceObjects()
{
	m_pFontCharacterName[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontCharacterName[0]->InitDeviceObjects(g_pD3dDev);
	m_pFontCharacterName[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontCharacterName[1]->InitDeviceObjects(g_pD3dDev);
	m_pFontStoreSpi[0] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontStoreSpi[0]->InitDeviceObjects(g_pD3dDev);
	m_pFontStoreSpi[1] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
	m_pFontStoreSpi[1]->InitDeviceObjects(g_pD3dDev);

	for(int i=0;i<2;i++)
	{
		m_pFontItemNum[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE,  TRUE,256,32);
		m_pFontItemNum[i]->InitDeviceObjects(g_pD3dDev);
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	DataHeader	* pDataHeader;
// 2011. 10. 10 by jskim UI시스템 변경	
	m_pTradeOkButton[0] = new CINFImageEx;
	pDataHeader = FindResource("oks00");
	m_pTradeOkButton[0]->InitDeviceObjects(pDataHeader) ;

	m_pTradeOkButton[1] = new CINFImageEx;
	pDataHeader = FindResource("oks01");
	m_pTradeOkButton[1]->InitDeviceObjects(pDataHeader) ;

	m_pTradeOkButton[2] = new CINFImageEx;
	pDataHeader = FindResource("oks02");
	m_pTradeOkButton[2]->InitDeviceObjects(pDataHeader) ;

	m_pTradeOkButton[3] = new CINFImageEx;
	pDataHeader = FindResource("oks03");
	m_pTradeOkButton[3]->InitDeviceObjects(pDataHeader) ;

// 2005-09-20 by ispark
	m_pTradeCancelButton[0] = new CINFImageEx;
	pDataHeader = FindResource("cans00");
	m_pTradeCancelButton[0]->InitDeviceObjects(pDataHeader) ;

	m_pTradeCancelButton[1] = new CINFImageEx;
	pDataHeader = FindResource("cans01");
	m_pTradeCancelButton[1]->InitDeviceObjects(pDataHeader) ;

	m_pTradeCancelButton[2] = new CINFImageEx;
	pDataHeader = FindResource("cans02");
	m_pTradeCancelButton[2]->InitDeviceObjects(pDataHeader) ;

	m_pTradeCancelButton[3] = new CINFImageEx;
	pDataHeader = FindResource("cans03");
	m_pTradeCancelButton[3]->InitDeviceObjects(pDataHeader) ;

	m_pTralog = new CINFImageEx;
	pDataHeader = FindResource("tra_log");
	m_pTralog->InitDeviceObjects(pDataHeader) ;
	
	m_pTrame = new CINFImageEx;
	pDataHeader = FindResource("tra_you");
	m_pTrame->InitDeviceObjects(pDataHeader) ;
	
	m_pTrayou = new CINFImageEx;
	pDataHeader = FindResource("tra_you");
	m_pTrayou->InitDeviceObjects(pDataHeader) ;	
	
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리	
	m_pTrameComple = new CINFImageEx;
	pDataHeader = FindResource("tra_you1");
	m_pTrameComple->InitDeviceObjects(pDataHeader) ;
	
	m_pTrayouComple = new CINFImageEx;
	pDataHeader = FindResource("tra_you1");
	m_pTrayouComple->InitDeviceObjects(pDataHeader ) ;	
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

#else
	DataHeader	* pDataHeader;
// 2011. 10. 10 by jskim UI시스템 변경	
		m_pTradeOkButton[0] = new CINFImageEx;
	pDataHeader = FindResource("sok0");
	m_pTradeOkButton[0]->InitDeviceObjects(pDataHeader) ;

		m_pTradeOkButton[1] = new CINFImageEx;
	pDataHeader = FindResource("sok1");
	m_pTradeOkButton[1]->InitDeviceObjects(pDataHeader) ;

		m_pTradeOkButton[2] = new CINFImageEx;
	pDataHeader = FindResource("sok2");
	m_pTradeOkButton[2]->InitDeviceObjects(pDataHeader) ;

// 2005-09-20 by ispark
		m_pTradeCancelButton[0] = new CINFImageEx;
	pDataHeader = FindResource("glcan00");
	m_pTradeCancelButton[0]->InitDeviceObjects(pDataHeader) ;

		m_pTradeCancelButton[1] = new CINFImageEx;
	pDataHeader = FindResource("glcan01");
	m_pTradeCancelButton[1]->InitDeviceObjects(pDataHeader) ;

		m_pTradeCancelButton[2] = new CINFImageEx;
	pDataHeader = FindResource("glcan02");
	m_pTradeCancelButton[2]->InitDeviceObjects(pDataHeader) ;

		m_pTralog = new CINFImageEx;
	pDataHeader = FindResource("tra_log");
	m_pTralog->InitDeviceObjects(pDataHeader) ;

		m_pTrame = new CINFImageEx;
	pDataHeader = FindResource("tra_me");
	m_pTrame->InitDeviceObjects(pDataHeader) ;

		m_pTrayou = new CINFImageEx;
	pDataHeader = FindResource("tra_you");
	m_pTrayou->InitDeviceObjects(pDataHeader) ;	

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리	
	m_pTrameComple = new CINFImageEx;
	pDataHeader = FindResource("tra_me1");
	m_pTrameComple->InitDeviceObjects(pDataHeader) ;

	m_pTrayouComple = new CINFImageEx;
	pDataHeader = FindResource("tra_you1");
	m_pTrayouComple->InitDeviceObjects(pDataHeader ) ;	
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
#endif
	return S_OK ;
}

HRESULT CINFTrade::RestoreDeviceObjects()
{
	int i;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<4;i++)
#else
	for(i=0;i<3;i++)
#endif
	{
		m_pTradeOkButton[i]->RestoreDeviceObjects();
		m_pTradeCancelButton[i]->RestoreDeviceObjects();
	}
	if(m_pTralog )
		m_pTralog->RestoreDeviceObjects();
	if(m_pTrame )
		m_pTrame->RestoreDeviceObjects();
	if(m_pTrayou )
		m_pTrayou->RestoreDeviceObjects();

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리	
	if(m_pTrameComple)
	{
		m_pTrameComple->RestoreDeviceObjects();
	}
	if(m_pTrayouComple)
	{
		m_pTrayouComple->RestoreDeviceObjects();
	}	
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	m_pFontCharacterName[0]->RestoreDeviceObjects();
	m_pFontCharacterName[1]->RestoreDeviceObjects();
	m_pFontStoreSpi[0]->RestoreDeviceObjects();
	m_pFontStoreSpi[1]->RestoreDeviceObjects();
	
	for(i=0;i<2;i++)
	{
		m_pFontItemNum[i]->RestoreDeviceObjects();
	}

	m_bRestored = TRUE;
	return S_OK ;
}

HRESULT CINFTrade::DeleteDeviceObjects()
{
	int i;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<4;i++)
#else
	for(=0;i<3;i++)
#endif
	{
		m_pTradeOkButton[i]->DeleteDeviceObjects();
		util::del(m_pTradeOkButton[i] );
		m_pTradeCancelButton[i]->DeleteDeviceObjects();
		util::del(m_pTradeCancelButton[i] );
	}
	if(m_pTralog )
		m_pTralog->DeleteDeviceObjects();
		util::del(m_pTralog );
	if(m_pTrame )
		m_pTrame->DeleteDeviceObjects();
		util::del(m_pTrame );
	if(m_pTrayou )
		m_pTrayou->DeleteDeviceObjects();
		util::del(m_pTrayou );

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리	
	if(m_pTrameComple)
	{
		m_pTrameComple->DeleteDeviceObjects();
		util::del(m_pTrameComple );
	}
	if(m_pTrayouComple)
	{		
		m_pTrayouComple->DeleteDeviceObjects();
		util::del(m_pTrayouComple);
	}	
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	m_pFontCharacterName[0]->DeleteDeviceObjects();
	util::del(m_pFontCharacterName[0] );
	m_pFontCharacterName[1]->DeleteDeviceObjects();
	util::del(m_pFontCharacterName[1] );
	m_pFontStoreSpi[0]->DeleteDeviceObjects();
	util::del(m_pFontStoreSpi[0] );
	m_pFontStoreSpi[1]->DeleteDeviceObjects();
	util::del(m_pFontStoreSpi[1] );

	for(i=0;i<2;i++)
	{
		m_pFontItemNum[i]->DeleteDeviceObjects();
		util::del(m_pFontItemNum[i]);
	}
	
	m_bInvalidated = FALSE;
	
	return S_OK ;
}


HRESULT CINFTrade::InvalidateDeviceObjects()
{
	int i;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	for(i=0;i<4;i++)
#else
	for(i=0;i<3;i++)
#endif
	{
		m_pTradeOkButton[i]->InvalidateDeviceObjects();
		m_pTradeCancelButton[i]->InvalidateDeviceObjects();
	}
	if(m_pTralog )
		m_pTralog->InvalidateDeviceObjects();
	if(m_pTrame )
		m_pTrame->InvalidateDeviceObjects();
	if(m_pTrayou )
		m_pTrayou->InvalidateDeviceObjects();
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리	
	if(m_pTrameComple)
	{
		m_pTrameComple->InvalidateDeviceObjects();
	}
	if(m_pTrayouComple)
	{		
		m_pTrayouComple->InvalidateDeviceObjects();
	}	
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	m_pFontCharacterName[0]->InvalidateDeviceObjects();
	m_pFontCharacterName[1]->InvalidateDeviceObjects();
	m_pFontStoreSpi[0]->InvalidateDeviceObjects();
	m_pFontStoreSpi[1]->InvalidateDeviceObjects();
	
	for(i=0;i<2;i++)
	{
		m_pFontItemNum[i]->InvalidateDeviceObjects();
	}

	m_bRestored = FALSE;

	m_bInvalidated = TRUE;

	return S_OK ;
}


void CINFTrade::Tick()
{


}


void CINFTrade::Render()
{
	//int nWindowPosY = g_pGameMain->m_nLeftWindowY;
	int nWindowPosY = TRADE_BK_POS_Y;

//	if(m_pTralog )
//	{
//		m_pTralog->Move(8, nWindowPosY + 5);
//		m_pTralog->Render() ;
//	}
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
//	if(m_pTrayou )
//	{
//		m_pTrayou->Move(202+RENEW_TRADE_WIDTH, nWindowPosY + 23);
//		m_pTrayou->Render() ;
//	}	
//	if(m_pTrame )
//	{
//		m_pTrame->Move(202+RENEW_TRADE_WIDTH, nWindowPosY + 144);
//		m_pTrame->Render() ;
//	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_bTraYouComple)
	{
		if(m_pTrayouComple )
		{
			m_pTrayouComple->Move(214+RENEW_TRADE_WIDTH, nWindowPosY + 26);
			m_pTrayouComple->Render() ;
		}
	}
	
	if(m_bTraMeComple)
	{
		if(m_pTrameComple )
		{
			m_pTrameComple->Move(214+RENEW_TRADE_WIDTH, nWindowPosY + 147);
			m_pTrameComple->Render() ;
		}
	}
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
	
	// 버튼 : OK
	if(m_nTradeButtonState[0] != BUTTON_STATE_NORMAL)
	{
		m_pTradeOkButton[m_nTradeButtonState[0]]->Move(TRADE_TRADE_OK_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_TRADE_OK_Y);
		m_pTradeOkButton[m_nTradeButtonState[0]]->Render();
	}
	else
	{
		m_pTradeOkButton[BUTTON_STATE_NORMAL]->Move(TRADE_TRADE_OK_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_TRADE_OK_Y);
		m_pTradeOkButton[BUTTON_STATE_NORMAL]->Render();
	}
	// 버튼 : Cancel
	if(m_nTradeButtonState[1] != BUTTON_STATE_NORMAL)
	{
		m_pTradeCancelButton[m_nTradeButtonState[1]]->Move(TRADE_TRADE_CANCEL_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_TRADE_CANCEL_Y);
		m_pTradeCancelButton[m_nTradeButtonState[1]]->Render();
	}
	else
	{
		m_pTradeCancelButton[BUTTON_STATE_NORMAL]->Move(TRADE_TRADE_CANCEL_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_TRADE_CANCEL_Y);
		m_pTradeCancelButton[BUTTON_STATE_NORMAL]->Render();
	}
#else
	if(m_bTraYouComple)
	{
		if(m_pTrayouComple )
		{
			m_pTrayouComple->Move(202+RENEW_TRADE_WIDTH, nWindowPosY + 23);
			m_pTrayouComple->Render() ;
		}
	}
	else
	{
		if(m_pTrayou )
		{
			m_pTrayou->Move(202+RENEW_TRADE_WIDTH, nWindowPosY + 23);
			m_pTrayou->Render() ;
		}
	}
	
	if(m_bTraMeComple)
	{
		if(m_pTrameComple )
		{
			m_pTrameComple->Move(202+RENEW_TRADE_WIDTH, nWindowPosY + 144);
			m_pTrameComple->Render() ;
		}
	}
	else
	{
		if(m_pTrame )
		{
			m_pTrame->Move(202+RENEW_TRADE_WIDTH, nWindowPosY + 144);
			m_pTrame->Render() ;
		}
	}
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	// 버튼 : OK
	if(m_nTradeButtonState[0] != BUTTON_STATE_NORMAL)
	{
		m_pTradeOkButton[m_nTradeButtonState[0]]->Move(TRADE_TRADE_OK_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_TRADE_OK_Y);
		m_pTradeOkButton[m_nTradeButtonState[0]]->Render();
	}
	// 버튼 : Cancel
	if(m_nTradeButtonState[1] != BUTTON_STATE_NORMAL)
	{
		m_pTradeCancelButton[m_nTradeButtonState[1]]->Move(TRADE_TRADE_CANCEL_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_TRADE_CANCEL_Y);
		m_pTradeCancelButton[m_nTradeButtonState[1]]->Render();
	}
#endif
	

	// 아이템 정보
	CINFIcon* pIcon = ((CINFGameMain*)m_pParent)->m_pIcon;
	vector<CItemInfo*>::iterator it = m_vecOtherStore.begin();
	int count = RENEW_TRADE_INVEN_NUM*m_nOtherStoreScroll, j;
	for(j=0;j<count;j++)
	{
		if(it != m_vecOtherStore.end())
			it++;
	}
	int i=0;
	char buf[20];
	char bufCnt[8];
	memset(bufCnt, 0x00, 8);
	while(it != m_vecOtherStore.end())
	{
		CItemInfo* pItem = *it;


		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		
		//wsprintf(buf, "%08d",pItem->ItemInfo->SourceIndex);
		if( !pItem->ShapeItemNum )
			wsprintf( buf, "%08d",pItem->ItemInfo->SourceIndex );
		else
		{
			ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItem->ShapeItemNum );
			if( pShapeItem )
				wsprintf( buf, "%08d", pShapeItem->SourceIndex );
			else
				wsprintf( buf, "%08d", pItem->ItemInfo->SourceIndex );
		}

		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현


		pIcon->SetIcon( buf
			, TRADE_MYSTORE_START_X + i % RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_X + 1 + RENEW_TRADE_WIDTH
			, nWindowPosY + TRADE_OTHERSTORE_START_Y + i / RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_Y + 1
			, 1.0f );
		pIcon->Render();

		if(IS_COUNTABLE_ITEM(pItem->Kind))
		{
			memset(bufCnt, 0x00, 8);
			wsprintf(bufCnt, "%4.d",pItem->CurrentCount);
			m_pFontItemNum[0]->DrawText(TRADE_MYSTORE_START_X+i%RENEW_TRADE_INVEN_NUM*TRADE_SLOT_INTERVAL_X+1+RENEW_TRADE_WIDTH,
				nWindowPosY+TRADE_OTHERSTORE_START_Y+i/RENEW_TRADE_INVEN_NUM*TRADE_SLOT_INTERVAL_Y+1, GUI_FONT_COLOR, bufCnt, 0L);
		}


		// 2010. 02. 11 by ckPark 발동류 장착아이템
		if( pItem->ItemInfo->InvokingDestParamID
			|| pItem->ItemInfo->InvokingDestParamIDByUse )
		{
			char buf[128];

			if( GetString_CoolTime( pItem, buf ) )
			{
				int nFontPosX = TRADE_MYSTORE_START_X + i % RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_X + RENEW_TRADE_WIDTH;
				int nFontPosY = nWindowPosY + TRADE_OTHERSTORE_START_Y + i / RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_Y + 8;

				m_pFontItemNum[0]->DrawText( nFontPosX, nFontPosY, GUI_FONT_COLOR, buf, 0L );
			}
		}
		// end 2010. 02. 11 by ckPark 발동류 장착아이템


		it++;
		i++;
	}	
	count = RENEW_TRADE_INVEN_NUM*m_nMyStoreScroll;
	for(j=0;j<count;j++)
	{
		if(it != m_vecMyStore.end())
			it++;
	}
	i=0;
	it = m_vecMyStore.begin();
	while(it != m_vecMyStore.end())
	{
		CItemInfo* pItemInfo = *it;


		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		//wsprintf(buf, "%08d",pItemInfo->ItemInfo->SourceIndex);
		if( !pItemInfo->ShapeItemNum )
			wsprintf( buf, "%08d", pItemInfo->ItemInfo->SourceIndex );
		else
		{
			ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItemInfo->ShapeItemNum );
			if( pShapeItem )
				wsprintf( buf, "%08d", pShapeItem->SourceIndex );
			else
				wsprintf( buf, "%08d", pItemInfo->ItemInfo->SourceIndex );
		}

		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현



		pIcon->SetIcon( buf
			, TRADE_MYSTORE_START_X + i % RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_X + 1 + RENEW_TRADE_WIDTH
			, nWindowPosY + TRADE_MYSTORE_START_Y + i / RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_Y + 1
			, 1.0f );
		pIcon->Render();

		if(IS_COUNTABLE_ITEM(pItemInfo->Kind))
		{
			memset(bufCnt, 0x00, 8);
			wsprintf(bufCnt, "%4.d",pItemInfo->CurrentCount);
			m_pFontItemNum[1]->DrawText(TRADE_MYSTORE_START_X+i%RENEW_TRADE_INVEN_NUM*TRADE_SLOT_INTERVAL_X+1+RENEW_TRADE_WIDTH,
				nWindowPosY+TRADE_MYSTORE_START_Y+i/RENEW_TRADE_INVEN_NUM*TRADE_SLOT_INTERVAL_Y+1, GUI_FONT_COLOR, bufCnt, 0L);
		}


		// 2010. 02. 11 by ckPark 발동류 장착아이템
		if( pItemInfo->ItemInfo->InvokingDestParamID
			|| pItemInfo->ItemInfo->InvokingDestParamIDByUse )
		{
			char buf[128];

			if( GetString_CoolTime( pItemInfo, buf ) )
			{
				int nFontPosX = TRADE_MYSTORE_START_X + i % RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_X + RENEW_TRADE_WIDTH;
				int nFontPosY = nWindowPosY + TRADE_MYSTORE_START_Y + i / RENEW_TRADE_INVEN_NUM * TRADE_SLOT_INTERVAL_Y + 8;

				m_pFontItemNum[0]->DrawText( nFontPosX, nFontPosY, GUI_FONT_COLOR, buf, 0L );
			}
		}
		// end 2010. 02. 11 by ckPark 발동류 장착아이템


		it++;
		i++;
	}

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	if(m_strTradeCharacterName[0])
		m_pFontCharacterName[0]->DrawText(219+RENEW_TRADE_WIDTH, nWindowPosY + 27, GUI_FONT_COLOR, m_strTradeCharacterName, 0L);
	m_pFontCharacterName[1]->DrawText(219+RENEW_TRADE_WIDTH, nWindowPosY + 148, GUI_FONT_COLOR, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, 0L);
#else
	if(m_strTradeCharacterName[0])
		m_pFontCharacterName[0]->DrawText(209+RENEW_TRADE_WIDTH, nWindowPosY + 27, GUI_FONT_COLOR, m_strTradeCharacterName, 0L);
	m_pFontCharacterName[1]->DrawText(209+RENEW_TRADE_WIDTH, nWindowPosY + 148, GUI_FONT_COLOR, g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterName, 0L);
#endif
	char temp1[64];
	char temp2[64];
	wsprintf( temp1, "%d", m_nOtherStoreSpi );
	MakeCurrencySeparator( temp2, temp1, 3, ',' );
//	wsprintf(buf, "%10d",m_nOtherStoreSpi);
	m_pFontStoreSpi[0]->DrawText(TRADE_OTHER_SPI_START_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_OTHER_SPI_START_Y, GUI_FONT_COLOR, temp2, 0L);
//	wsprintf(buf, "%10d",m_nMyStoreSpi);
	wsprintf( temp1, "%d", m_nMyStoreSpi );
	memset(temp2, 0x00, sizeof(temp2));
	MakeCurrencySeparator( temp2, temp1, 3, ',' );
	m_pFontStoreSpi[1]->DrawText(TRADE_MY_SPI_START_X+RENEW_TRADE_WIDTH, nWindowPosY + TRADE_MY_SPI_START_Y, GUI_FONT_COLOR, temp2, 0L);
}

int CINFTrade::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
	
			CheckMouseReverse(&pt);
			//int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			int nWindowPosY = TRADE_BK_POS_Y;
			int i = (pt.x-(TRADE_MYSTORE_START_X+RENEW_TRADE_WIDTH))/TRADE_SLOT_INTERVAL_X;
			if( pt.x > TRADE_MYSTORE_START_X+RENEW_TRADE_WIDTH &&
				i >= 0 && i < TRADE_X_NUMBER &&
				pt.x < (TRADE_MYSTORE_START_X+RENEW_TRADE_WIDTH)+(TRADE_SLOT_INTERVAL_X*i)+TRADE_SLOT_SIZE)
			{
				// my store
				int j = (pt.y - nWindowPosY - TRADE_MYSTORE_START_Y)/TRADE_SLOT_INTERVAL_Y;
				if( pt.y > nWindowPosY + TRADE_MYSTORE_START_Y &&
					j >= 0 && j < TRADE_Y_NUMBER &&
					pt.y < nWindowPosY + TRADE_MYSTORE_START_Y + TRADE_SLOT_INTERVAL_Y*j + TRADE_SLOT_SIZE)
				{
					int count = j*TRADE_X_NUMBER+i;
					vector<CItemInfo*>::iterator it = m_vecMyStore.begin();
					for(int k=0;k<count;k++)
					{
						if(it != m_vecMyStore.end())
						{
							it++;
						}
					}
					if(it != m_vecMyStore.end())
					{
						CItemInfo* pItemInfo = *it;
						// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
						//((CINFGameMain*)m_pParent)->m_pItemInfo->SetItemInfoUser(pItemInfo, pt.x, pt.y);
						((CINFGameMain*)m_pParent)->SetItemInfoUser(pItemInfo, pt.x, pt.y);
						// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
						return INF_MSGPROC_BREAK;
					}
				}
				else
				{
					// other store
					j = (pt.y - nWindowPosY - TRADE_OTHERSTORE_START_Y)/TRADE_SLOT_INTERVAL_Y;
					if( pt.y > nWindowPosY + TRADE_OTHERSTORE_START_Y &&
						j >= 0 && j < TRADE_Y_NUMBER &&
						pt.y < nWindowPosY + TRADE_OTHERSTORE_START_Y + TRADE_SLOT_INTERVAL_Y*j + TRADE_SLOT_SIZE)
					{
						int count = j*TRADE_X_NUMBER+i;
						vector<CItemInfo*>::iterator it = m_vecOtherStore.begin();
						for(int k=0;k<count;k++)
						{
							if(it != m_vecOtherStore.end())
							{
								it++;
							}
						}
						if(it != m_vecOtherStore.end())
						{
							CItemInfo* pItemInfo = *it;
							// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							//((CINFGameMain*)m_pParent)->m_pItemInfo->SetItemInfoUser(pItemInfo, pt.x, pt.y);
							((CINFGameMain*)m_pParent)->SetItemInfoUser(pItemInfo, pt.x, pt.y);
							// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
							return INF_MSGPROC_BREAK;
						}
					}
				}
				// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
				//((CINFGameMain*)m_pParent)->m_pItemInfo->SetItemInfoUser( NULL, 0, 0 );
				((CINFGameMain*)m_pParent)->SetItemInfoUser( NULL, 0, 0 );
				// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
			}

			if(pt.y > nWindowPosY + TRADE_TRADE_OK_Y && pt.y < nWindowPosY + (TRADE_TRADE_OK_Y+TRADE_TRADE_OK_H))
			{
				if(pt.x > TRADE_TRADE_OK_X+RENEW_TRADE_WIDTH && pt.x < TRADE_TRADE_OK_X+TRADE_TRADE_OK_W+RENEW_TRADE_WIDTH)
				{
					if(m_nTradeButtonState[0] != BUTTON_STATE_DOWN)
						m_nTradeButtonState[0] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nTradeButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if(pt.x > TRADE_TRADE_CANCEL_X+RENEW_TRADE_WIDTH && pt.x < TRADE_TRADE_CANCEL_X + TRADE_TRADE_CANCEL_W+ RENEW_TRADE_WIDTH)
				{
					if(m_nTradeButtonState[1] != BUTTON_STATE_DOWN)
						m_nTradeButtonState[1] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nTradeButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}
			else
			{
				for(int i=0;i<2;i++)
					m_nTradeButtonState[i] = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			CheckMouseReverse(&pt);
			//int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			int nWindowPosY = TRADE_BK_POS_Y;

			// 우선 순위 지정
			if( pt.x > 1 && pt.x <445+RENEW_TRADE_WIDTH && pt.y > nWindowPosY && pt.y <nWindowPosY +275)
				g_pInterface->SetWindowOrder(WNDLeftRightWindow);
			
			if(pt.y > nWindowPosY + TRADE_TRADE_OK_Y && pt.y < nWindowPosY + TRADE_TRADE_OK_Y + TRADE_TRADE_OK_H)
			{
				if(pt.x > TRADE_TRADE_OK_X+RENEW_TRADE_WIDTH && pt.x < TRADE_TRADE_OK_X + TRADE_TRADE_OK_W+RENEW_TRADE_WIDTH)
				{
					m_nTradeButtonState[0] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nTradeButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if(pt.x > TRADE_TRADE_CANCEL_X+RENEW_TRADE_WIDTH && pt.x < TRADE_TRADE_CANCEL_X + TRADE_TRADE_CANCEL_W+RENEW_TRADE_WIDTH)
				{
					m_nTradeButtonState[1] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nTradeButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}
			if(pt.x>INVEN_SPI_START_X&& pt.x<INVEN_SPI_START_X+INVEN_SPI_WIDTH&&
				pt.y>nWindowPosY+INVEN_SPI_START_Y && pt.y<nWindowPosY+INVEN_SPI_START_Y+INVEN_SPI_HEIGHT )
			{
				if(!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_TRADE_ITEM_NUMBER))
				{
					if(((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi()>0)
					{						
						((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
							STRMSG_C_TRADE_0005, _Q_TRADE_ITEM_NUMBER, 0, ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi());//"얼마를 올리시겠습니까?"
						g_pInterface->SetWindowOrder(WNDInfWindow);
						return INF_MSGPROC_BREAK;
					}
				}
			}
			// 2005-12-26 by ispark
			// 창안에 마우스 클릭시 무효
			if(pt.y>nWindowPosY &&
				pt.y<nWindowPosY+SIZE_NORMAL_WINDOW_Y &&
				pt.x>425 &&
				pt.x<425+228)
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
			//int nWindowPosY = g_pGameMain->m_nLeftWindowY;
			int nWindowPosY = TRADE_BK_POS_Y;
			if(pt.y > nWindowPosY + TRADE_TRADE_OK_Y && pt.y < nWindowPosY + TRADE_TRADE_OK_Y + TRADE_TRADE_OK_H)
			{
				if(pt.x > TRADE_TRADE_OK_X+RENEW_TRADE_WIDTH && pt.x < TRADE_TRADE_OK_X + TRADE_TRADE_OK_W+RENEW_TRADE_WIDTH)
				{
					if(m_nTradeButtonState[0] == BUTTON_STATE_DOWN)
					{
						OnTradeButtonClick(0);
					}
					m_nTradeButtonState[0] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nTradeButtonState[0] = BUTTON_STATE_NORMAL;
				}
				if(pt.x > TRADE_TRADE_CANCEL_X+RENEW_TRADE_WIDTH && pt.x < TRADE_TRADE_CANCEL_X + TRADE_TRADE_CANCEL_W+RENEW_TRADE_WIDTH)
				{
					if(m_nTradeButtonState[1] == BUTTON_STATE_DOWN)
					{
						OnTradeButtonClick(1);
					}
					m_nTradeButtonState[1] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nTradeButtonState[1] = BUTTON_STATE_NORMAL;
				}
			}
			if( m_vecMyStore.size() < MAX_TRADE_ITEM_NUMBER &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem &&
				((CINFGameMain*)m_pParent)->m_stSelectItem.bySelectType == ITEM_INVEN_POS && 
				pt.x > TRADE_MYSTORE_START_X+RENEW_TRADE_WIDTH && pt.x < TRADE_MYSTORE_START_X+TRADE_STORE_WIDTH+RENEW_TRADE_WIDTH &&
				pt.y > nWindowPosY + TRADE_MYSTORE_START_Y && pt.y < nWindowPosY + TRADE_MYSTORE_START_Y+TRADE_STORE_HEIGHT)
			{
				ITEM_GENERAL* pItem = (CItemInfo*)((CINFGameMain*)m_pParent)->m_stSelectItem.pSelectItem->pItem;
				if(IS_COUNTABLE_ITEM(pItem->Kind) && pItem->CurrentCount > 1)
				{
					if(!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_TRADE_ITEM_NUMBER))
					{
						m_pTempItem = pItem;
						((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox(
							STRMSG_C_TRADE_0006, _Q_TRADE_ITEM_NUMBER, 0, m_pTempItem->CurrentCount);//"몇개를 올리시겠습니까?"
					}
				}
				else
				{
					// 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
					if(pItem->FixedTermShape.nStatShapeItemNum && pItem->FixedTermShape.nStatLevel)
					{
						g_pStoreData->RqCollectionShapeChange(pItem->UniqueNumber,0);
					}
#endif
					// end 2013-06-13 by ssjung 외형 변경 컬렉션 예외처리 추가(외형 적용이 안되야 할 부분에 외형 적용이 안될 수 있도록)

					MSG_FC_TRADE_TRANS_ITEM sMsg;
					memset(&sMsg,0x00,sizeof(sMsg));
					char buffer[SIZE_MAX_PACKET];
					int nType = T_FC_TRADE_TRANS_ITEM;
					sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
					sMsg.ItemUniqueNumber = pItem->UniqueNumber;
					sMsg.ItemNum = pItem->ItemNum;
					sMsg.Amount = pItem->CurrentCount;
					memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
					memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
					g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

					// 2008-08-22 by bhsohn EP3 인벤토리 처리
					if(g_pGameMain && g_pGameMain->m_pInven)
					{
						g_pGameMain->SetToolTip(NULL, 0, 0);
						g_pGameMain->m_pInven->SetItemInfo(NULL, 0, 0);
						g_pGameMain->m_pInven->SetMultiSelectItem(NULL);	// 다중 처리 제거
						g_pGameMain->m_pInven->SetSelectItem(NULL);
					}
					return INF_MSGPROC_BREAK;
					// end 2008-08-22 by bhsohn EP3 인벤토리 처리
				}

			}
			((CINFGameMain*)m_pParent)->SetItemInfo(0, 0, 0, 0);
		}
		break;
		// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_SHIFT:
				{
					g_pGameMain->ShowMyEqItemTooltip();
				}
				break;
			}
		}
		break;
		// end 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	}
	return INF_MSGPROC_NORMAL;
}


void CINFTrade::OnTradeButtonClick(int nButton)
{
	switch(nButton)
	{
	case 0://ok
		{
			((CINFGameMain*)m_pParent)->m_pInfWindow->DeleteMsgBox(_Q_TRADE_CANCEL);
			if(!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_TRADE_OK))
			{
				((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0007, _Q_TRADE_OK);//"다시 한번 확인하십시요. 거래를 완료하시겠습니까?"
			}
		}
		break;
	case 1://cancel
		{
			((CINFGameMain*)m_pParent)->m_pInfWindow->DeleteMsgBox(_Q_TRADE_OK);
			if(!((CINFGameMain*)m_pParent)->m_pInfWindow->IsExistMsgBox(_Q_TRADE_CANCEL))
			{
				((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox( STRMSG_C_TRADE_0008, _Q_TRADE_CANCEL);//"거래를 취소하시겠습니까?"
			}
		}
		break;
	}
}

void CINFTrade::SendTradeRequest(char* strCharacterName)
{
	// 2008-07-14 by bhsohn EP3 관련 처리
	if(g_pGameMain->IsRejectUser(strCharacterName))
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080714_0207,COLOR_ERROR);//"\\y거부 목록에 등록된 대상입니다."
		return;
	}
	// end 2008-07-14 by bhsohn EP3 관련 처리

	MSG_FC_TRADE_REQUEST_TRADE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	strncpy(sMsg.TradeTarget, strCharacterName, SIZE_MAX_CHARACTER_NAME);
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_TRADE_REQUEST_TRADE;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CINFTrade::RequestTrade(char* strCharacterName,UINT nTradeCharacterUniqueNumber)
{
	char buf[256];
	if(!m_bTrading)
	{
		strncpy(m_strTradeCharacterName, strCharacterName, SIZE_MAX_CHARACTER_NAME);
		m_nTradeCharacterUniqueNumber = nTradeCharacterUniqueNumber;
		// 2008-11-21 by bhsohn 거래 완료 시스템 처리
		m_bTraMeComple = m_bTraYouComple = FALSE;
		// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

		wsprintf(buf, STRMSG_C_TRADE_0009,strCharacterName);//"%s 와 거래를 하시겠습니까?"
		// 2008-07-18 by dgwoo 거래 요청메시지 시간제한메시지 창으로 변경.
		//((CINFGameMain*)m_pParent)->m_pInfWindow->AddMsgBox( buf, _Q_TRADE_REQUEST,0,0,0,0,strCharacterName);
		((CINFGameMain*)m_pParent)->m_pInfWindow->AddTimeMsgBox( buf, _Q_TRADE_REQUEST,30,STRMSG_C_060724_0000);
	}
	else
	{
		SendTradeReject();
	}
}

void CINFTrade::SendTradeAccept()
{
	// 풀화면거래시 풀화면 해제
	if(g_pInterface->m_bShowInterface == FALSE)
	{
		g_pInterface->m_bShowInterface = TRUE;
	}
	m_bTrading = TRUE;// 교환 시작
	MSG_FC_TRADE_ACCEPT_TRADE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_TRADE_ACCEPT_TRADE;
	sMsg.TradeSourceCharacterUniqueNumber = m_nTradeCharacterUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
}

void CINFTrade::SendTradeReject()
{
	m_bTrading = FALSE;
	MSG_FC_TRADE_REJECT_TRADE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_TRADE_REJECT_TRADE;
	sMsg.TradeSourceCharacterUniqueNumber = m_nTradeCharacterUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	m_strTradeCharacterName[0] = NULL;
	m_nTradeCharacterUniqueNumber = 0;
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	m_bTraMeComple = m_bTraYouComple = FALSE;
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
}

void CINFTrade::RecvTradeAccept(char* strCharacterName, UINT nTradeCharacterUniqueNumber)
{
	m_bTrading = TRUE;
	strcpy(m_strTradeCharacterName, strCharacterName);
	m_nTradeCharacterUniqueNumber = nTradeCharacterUniqueNumber;

	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	m_bTraMeComple = m_bTraYouComple = FALSE;
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리
}

void CINFTrade::SendTradeCancel()
{
	MSG_FC_TRADE_CANCEL_TRADE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_TRADE_CANCEL_TRADE;
	sMsg.CharacterUniqueNumber = m_nTradeCharacterUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	g_pGameMain->m_pInven->ShowInven(NULL, NULL);

}

void CINFTrade::SendTradeOk()
{
	MSG_FC_TRADE_OK_TRADE sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_TRADE_OK_TRADE;
	sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));

	// 2008-10-20 by bhsohn 거래시 창버그 수정
	//g_pGameMain->m_pInven->ShowInven(NULL, NULL);
}


void CINFTrade::SendTradeTrans(int count)
{
	// 선택된 아이템이 있는 경우
	if(m_pTempItem)
	{
		if(m_pTempItem->CurrentCount>=count)
		{
			MSG_FC_TRADE_TRANS_ITEM sMsg;
			memset(&sMsg,0x00,sizeof(sMsg));
			char buffer[SIZE_MAX_PACKET];
			int nType = T_FC_TRADE_TRANS_ITEM;
			sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
			sMsg.ItemUniqueNumber = m_pTempItem->UniqueNumber;
			sMsg.ItemNum = m_pTempItem->ItemNum;
			sMsg.Amount = count;
			memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
			g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
		}
	}
	// 선택된 아이템이 없으면 돈을 올린다.
	else if(((CINFGameMain*)m_pParent)->m_pInven->GetItemSpi()>=count)
	{
		MSG_FC_TRADE_TRANS_ITEM sMsg;
		memset(&sMsg,0x00,sizeof(sMsg));
		char buffer[SIZE_MAX_PACKET];
		int nType = T_FC_TRADE_TRANS_ITEM;
		sMsg.CharacterUniqueNumber = g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber;
		sMsg.ItemUniqueNumber = ((CINFGameMain*)m_pParent)->m_pInven->GetItemSpiUniqueNumber();
		sMsg.ItemNum = MONEY_ITEM_NUMBER;
		sMsg.Amount = count;
		memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
		g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg));
	}
	m_pTempItem = NULL;
}

void CINFTrade::EndTrade()
{
	if(((CINFGameMain*)m_pParent)->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
		((CINFGameMain*)m_pParent)->LeftWindowShow( TRUE, LEFT_WINDOW_WEAPON);
	m_bTrading = FALSE;
	memset(m_strTradeCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	m_nTradeCharacterUniqueNumber = 0;
	
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	m_bTraMeComple = m_bTraYouComple = FALSE;
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	vector<CItemInfo*>::iterator it = m_vecMyStore.begin();
	while(it != m_vecMyStore.end())
	{
		CItemInfo* pItem = *it;
		it++;
		util::del(pItem);
	}
	m_vecMyStore.clear();
	it = m_vecOtherStore.begin();
	while(it != m_vecOtherStore.end())
	{
		CItemInfo* pItem = *it;
		it++;
		util::del(pItem);
	}
	m_vecOtherStore.clear();
	m_nMyStoreSpi = 0;
	m_nOtherStoreSpi = 0;
	((CINFGameMain*)m_pParent)->SetItemInfo(0, 0, 0, 0);

	// 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그
	// 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
	//if(g_INFCnt==0)
	if(g_pD3dApp->GetINFCnt() == 0)
	{
		g_pGameMain->m_bChangeMousePoint = FALSE;
	}
	// end 2012-12-17 by jhjang 비행중 마우스 커서가 제대로 동작하지 않는 버그 수정
	// end 2009. 08. 19 by jsKim 랜딩 중 메뉴 생성할 경우 커서가 변하지 않는 버그

}

void CINFTrade::CancelTrade()
{
	// 2003.20.14 거래
	vector<CItemInfo*>::iterator it = m_vecMyStore.begin();
	while(it != m_vecMyStore.end())
	{
		if(IS_COUNTABLE_ITEM((*it)->GetItemInfo()->Kind))
		{
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByItemNum((*it)->GetItemInfo()->ItemNum);
			if( pItemInfo )
			{
				pItemInfo->CurrentCount += (*it)->CurrentCount;
			}
			else
			{
//				ITEM_GENERAL* pItem = (ITEM_GENERAL*)(*it);
//				g_pStoreData->PutItem((char *)pItem, TRUE);//update
				g_pStoreData->AddItemDataToUniqueNumberAndWindowPosition( *it );
			}
		}
		else
		{
			g_pStoreData->AddItemDataToUniqueNumberAndWindowPosition( *it );
		}
		it++;
	}
	
	g_pGameMain->m_pInven->SetItemSpi(g_pGameMain->m_pInven->GetItemSpi() + m_nMyStoreSpi);
	g_pShuttleChild->ResortingItem();
	g_pShuttleChild->SetShuttleData(5);
	g_pGameMain->m_pInven->SetAllIconInfo();
	if(((CINFGameMain*)m_pParent)->m_nLeftWindowInfo == LEFT_WINDOW_TRANS)
		((CINFGameMain*)m_pParent)->LeftWindowShow( TRUE, LEFT_WINDOW_WEAPON);
	m_bTrading = FALSE;
	memset(m_strTradeCharacterName, 0x00, SIZE_MAX_CHARACTER_NAME);
	m_nTradeCharacterUniqueNumber = 0;
	
	// 2008-11-21 by bhsohn 거래 완료 시스템 처리
	m_bTraMeComple = m_bTraYouComple = FALSE;
	// end 2008-11-21 by bhsohn 거래 완료 시스템 처리

	m_vecMyStore.clear();
	it = m_vecOtherStore.begin();
	while(it != m_vecOtherStore.end())
	{
		CItemInfo* pItem = *it;
		it++;
		util::del(pItem);
	}
	m_vecOtherStore.clear();
	m_nMyStoreSpi = 0;
	m_nOtherStoreSpi = 0;
	((CINFGameMain*)m_pParent)->SetItemInfo(0, 0, 0, 0);
}

void CINFTrade::AddItem(CItemInfo* pItem, int me)
{
	if(me == 1) // 자신의 아이템
	{
		if( pItem->Kind == ITEMKIND_ENERGY && pItem->ItemNum == MONEY_ITEM_NUMBER )
		{
			// 2002.10.14, 아이템창(돈)
			int nSpi = g_pGameMain->m_pInven->GetItemSpi();
			nSpi -= pItem->CurrentCount;
			nSpi += m_nMyStoreSpi;
			m_nMyStoreSpi = pItem->CurrentCount;
			g_pGameMain->m_pInven->SetItemSpi(nSpi);
			util::del(pItem);
		}
		else if(IS_COUNTABLE_ITEM(pItem->Kind))
		{
			CItemInfo* pOrgItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( pItem->UniqueNumber );
			vector<CItemInfo*>::iterator itMyStore = m_vecMyStore.begin();
			while(itMyStore != m_vecMyStore.end())
			{
				if((*itMyStore)->UniqueNumber == pItem->UniqueNumber)
				{
					pOrgItemInfo->CurrentCount = pOrgItemInfo->CurrentCount+(*itMyStore)->CurrentCount - pItem->CurrentCount;
					(*itMyStore)->CurrentCount = pItem->CurrentCount;
					// 2007-03-26 by bhsohn 카운터블 아이템 거래시 버그 수정
					if(0 == pOrgItemInfo->CurrentCount)
					{
						g_pStoreData->UpdateItemCount( pItem->UniqueNumber, pOrgItemInfo->CurrentCount);
					}
					// end 2007-03-26 by bhsohn 카운터블 아이템 거래시 버그 수정
					util::del(pItem);
					return;
				}
				itMyStore++;
			}
			g_pStoreData->UpdateItemCount( pItem->UniqueNumber, pOrgItemInfo->CurrentCount - pItem->CurrentCount );
			// 새로 추가
			m_vecMyStore.push_back( pItem );
		}
		else
		{
			g_pStoreData->DeleteItem( pItem->UniqueNumber );
			m_vecMyStore.push_back( pItem );
		}
	}
	else
	{
		if(pItem->Kind == ITEMKIND_ENERGY && pItem->ItemNum == MONEY_ITEM_NUMBER)
		{
			// 2002.10.14, 아이템창(돈)
			m_nOtherStoreSpi = pItem->CurrentCount;
		}
		else
		{
			vector<CItemInfo*>::iterator itOtherStore = m_vecOtherStore.begin();
			while(itOtherStore != m_vecOtherStore.end())
			{
				if((*itOtherStore)->UniqueNumber == pItem->UniqueNumber)
				{
					(*itOtherStore)->CurrentCount = pItem->CurrentCount;
					util::del(pItem);
					return;
				}
				itOtherStore++;
			}
			m_vecOtherStore.push_back(pItem);
		}
	}
}

BOOL CINFTrade::TradeQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y)//유닛을 정확히 찍은 경우 return TRUE
{
	D3DXVECTOR2 vPos1 = D3DXVECTOR2(x,y);
	float fLength = 100.0f;
	CEnemyData * pTrade = NULL;
	CEnemyData * pEnemy = NULL;
	map<INT,CEnemyData *>::iterator itEnemy = g_pD3dApp->m_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pD3dApp->m_pScene->m_mapEnemyList.end())
	{
		if( itEnemy->second->m_nObjScreenW > 0)
		{
			D3DXVECTOR2 vPos2 = D3DXVECTOR2(itEnemy->second->m_nObjScreenX,itEnemy->second->m_nObjScreenY);
			float fLengthTemp = D3DXVec2Length(&(vPos1-vPos2));
			if(fLengthTemp < 50.0f && fLengthTemp < fLength)
			{
				fLength = fLengthTemp;
				pTrade = itEnemy->second;
			}
		}
		itEnemy++;
	}
	if(pTrade)
	{
		strcpy(pMsgBox->m_strInputMessage,pTrade->m_infoCharacter.CharacterInfo.CharacterName);
		strcat(pMsgBox->m_strInputMessage, "_");
		return TRUE;
	}

	return FALSE;
}


void CINFTrade::AddEnchantItem( LONGLONG uniqueNumber, INT nEnchantItemNum )
{
	vector<CItemInfo*>::iterator it = m_vecOtherStore.begin();
	while(it != m_vecOtherStore.end())
	{
		if((*it)->UniqueNumber == uniqueNumber)
		{
			(*it)->AddEnchantItem( nEnchantItemNum );
			break;
		}
		it++;
	}
#ifdef _DEBUG
	if(it == m_vecOtherStore.end())
	{
		DBGOUT("Trade::AddEnchantItem , Enchant Info Item is nULL(%d)\n",uniqueNumber);
	}
#endif // _DEBUG_endif

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFTrade::FindTradeMyItem(UID64_t ItemUniqueNumber)
/// \brief		거래 아이템 찾기
/// \author		ispark
/// \date		2006-05-15 ~ 2006-05-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemInfo* CINFTrade::FindTradeMyItem(UID64_t ItemUniqueNumber)
{
	vector<CItemInfo*>::iterator it = m_vecMyStore.begin();
	while(it != m_vecMyStore.end())
	{
		if((*it)->UniqueNumber == ItemUniqueNumber)
		{
			return *it;
		}

		it++;
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		거래 상에 죽었을시, 키 안먹는 문제처리
/// \author		// 2007-10-16 by bhsohn 거래 상에 죽었을시, 키 안먹는 문제처리
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTrade::SetTradeInfo(BOOL bTrade)
{
	m_bTrading = bTrade;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-11-21 by bhsohn 거래 완료 시스템 처리
/// \date		2008-11-21 ~ 2008-11-21
/// \warning	
///
/// \param		// 거래 승인 버튼을 실행한 캐릭터
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFTrade::SetTradeOkTradeNotify(UID32_t CharacterUniqueNumber)
{	
	if (CharacterUniqueNumber == g_pShuttleChild->GetMyShuttleInfo().CharacterUniqueNumber)
	{
		// 나 자신이다.	
		m_bTraMeComple = TRUE;
	}
	else if (CharacterUniqueNumber == m_nTradeCharacterUniqueNumber)
	{
		m_bTraYouComple = TRUE;
	}
}

