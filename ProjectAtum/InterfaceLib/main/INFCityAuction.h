// CINFCityAuction.h: interface for the CINFCityShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYAUCTION_H__9B6EB8A3_634C_4255_A627_69015270E88C__INCLUDED_)
#define AFX_INFCITYAUCTION_H__9B6EB8A3_634C_4255_A627_69015270E88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

typedef struct
{
	int				AuctionKind;									// 아이템 종류
	char			SellerCharacterName[SIZE_MAX_CHARACTER_NAME];	// 아이템 소유주 주인
//	ITEM_GENERAL	AuctionItemGeneral;								// 경매 아이템
	int 			AuctionPrice;									// 경매 가격
	ATUM_DATE_TIME	AuctionStartDate;								// 경매 시작 시간
	CItemInfo*		pItemInfo;
} AuctionItem_t;

class CINFImage;
class CINFImageEx;								// 2011. 10. 10 by jskim UI시스템 변경
class CD3DHanFont;
class CINFCityAuction : public CINFBase  
{
public:
	CINFCityAuction(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityAuction();

	BOOL IsInputItemDone() { return m_bInputItemDone; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitData();
	
	void InputItemDone();//m_pnSelectItemNumber 배열 초기화
	void BuyItem(int nItemNum, int nAmount);
	void InputItem(MSG_FC_AUCTION_INSERT_ITEM* pMsg);
	void AuctionItemInfoView(int nTab);

	void OnOkButtonClicked();
	void OnCancelButtonClicked();
	void OnBuyButtonClicked(int nSlotNumber);
	void AddEnchantItem(BYTE nKind, UID64_t nUniqueNumber, INT nEnchantItemNum);

//	void GetAuctionItemList(int nAuctionItemKind);

public:
	BOOL			m_bInputItemDone;// InputItemDone이 온 경우 TRUE, 이 값이 TRUE이면 아이템 리스트를 다시 받지 않는다.
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;

	CINFImageEx	*	m_pOkButton[4];
	CINFImageEx	*	m_pCancelButton[4];
	CINFImageEx	*	m_pBuyButton[4];
	CINFImageEx	*	m_pCash;
	CINFImageEx	*	m_pSlot;
	CINFImageEx	*	m_pItemTab[4];
	CINFImageEx	*	m_pScrollUpDown;
	CINFImageEx	*	m_pScrollBar;
	CINFImageEx	*	m_pSelectSlot;

	int				m_nCurrentTab;
	int				m_nOkButtonState;
	int				m_nCancelButtonState;
	int				m_nBuyButtonState[SHOP_ITEM_SLOT_NUMBER];

	//vector<SHOP_ITEM*> m_vecItemInfo[SHOP_ITEM_TAB_NUMBER];
	// 정보를 보여주기 위한 구조체
	vector<AuctionItem_t> m_vecAuctionItem[SHOP_ITEM_TAB_NUMBER];

	int				m_nCurrentScrollNumber[SHOP_ITEM_TAB_NUMBER];
	BOOL			m_bScrollLock;
	POINT			m_ptMouse;
	CD3DHanFont*	m_pFontItemName[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontItemPrice[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontItemNumber[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontTotalPrice;
	CD3DHanFont*	m_pFontDate;

	int*			m_pnSelectItemNumber[SHOP_ITEM_TAB_NUMBER];
	int				m_nCurrentSelectSlot[SHOP_ITEM_TAB_NUMBER];
	BUILDINGNPC*	m_pBuildingInfo;
};

#endif // !defined(AFX_INFCITYSHOP_H__9B6EB8A3_634C_4255_A627_69015270E88C__INCLUDED_)
