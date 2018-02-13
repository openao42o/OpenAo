// INFCityShop.h: interface for the CINFCityShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYSHOP_H__9B6EB8A3_634C_4255_A627_69015270E88C__INCLUDED_)
#define AFX_INFCITYSHOP_H__9B6EB8A3_634C_4255_A627_69015270E88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CD3DHanFont;
class CINFImageEx;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFCityShop : public CINFBase
{
public:
	CINFCityShop(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityShop();

	BOOL IsInputItemDone() { return m_bInputItemDone; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitData(BOOL bColorShop=FALSE);
//	void InitData(MSG_FC_EVENT_ENTER_BUILDING* pMsg);
	int InputItem(char* pMsg);
	void InputItemDone();//m_pnSelectItemNumber 배열 초기화
	void SellItem( LONGLONG nUniqueNumber, int nAmount/* 개수**/, BYTE itemKind );
	// 2008-07-16 by bhsohn 논 카운터블 아이템 10개 이상 구매불가
	//void BuyItem(int nItemNum, int nAmount);
	void BuyItem(int nItemNum, int nAmount, int ItemKind);
	// end 2008-07-16 by bhsohn 논 카운터블 아이템 10개 이상 구매불가
	void BuyItemColorShop(int nItemNum);
	UINT GetTotalPrice();

	void OnOkButtonClicked();
	void OnCancelButtonClicked();
	void OnBuyButtonClicked(int nSlotNumber);
	void OnChangeWearItem();	// 상점내에 아이템 장착해보기 

	void UpdateItem(int nItemNum);
	void UpdateFindShopItem(int nTab, ITEM* pITEM);
	void InputColorShopItem();

	SHOP_ITEM* FindCityShopDataByColor(int nColor);
	void		UpdateItemRemainCount( ItemNum_t nItemNum, INT nRemainCount );

	void InitSelectItemNum(); // 2013-06-20 by bhsohn 상점에서 아이템 구매후 숫자 초기화 코드 추가

protected:
	SHOP_ITEM* FindCityShopData(int nTabIndex, SHOP_ITEM* pShopItem);
	int GetScrollLineNumber(float nLength);
	float GetScrollLineInterval(int nScrollLineNum);
	int GetScrollLine();



public:
	BOOL			m_bInputItemDone;// InputItemDone이 온 경우 TRUE, 이 값이 TRUE이면 아이템 리스트를 다시 받지 않는다.
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageBtn*	m_pOkButton;
	CINFImageBtn*	m_pCancelButton;
	//NFImageEx	*	m_pCash;
	CINFImageEx	*	m_pSlot;
	CINFImageEx	*	m_pItemTab[4];
	//CINFImageEx	*	m_pScrollUpDown;
	CINFImageEx	*	m_pScrollBar;
	CINFImageEx	*	m_pSelectSlot;		
	CINFImageEx	*	m_pTriSlot;							// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	CINFImageEx	*	m_pTriSelectSlot;					// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경

#else			
	CINFImageEx	*	m_pTriSelectSlot;					// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경
	CINFImageEx	*	m_pTriSlot;							// 2011-12-08 by jhahn EP4 트리거 상점 UI 변경

	CINFImageEx	*	m_pOkButton[4];
	CINFImageEx	*	m_pCancelButton[4];
	CINFImageEx	*	m_pBuyButton[4];
	CINFImageEx	*	m_pCash;
	CINFImageEx	*	m_pSlot;
	CINFImageEx	*	m_pItemTab[4];
	CINFImageEx	*	m_pScrollUpDown;
	CINFImageEx	*	m_pScrollBar;
	CINFImageEx	*	m_pSelectSlot;
#endif

	int				m_nCurrentTab;
	int				m_nOkButtonState;
	int				m_nCancelButtonState;
	int				m_nBuyButtonState[SHOP_ITEM_SLOT_NUMBER];

	vector<SHOP_ITEM*> m_vecItemInfo[SHOP_ITEM_TAB_NUMBER];

	int				m_nCurrentScrollNumber[SHOP_ITEM_TAB_NUMBER];
	int				m_nMouseScrollPosition[SHOP_ITEM_TAB_NUMBER];
	BOOL			m_bScrollLock;
	POINT			m_ptMouse;
	CD3DHanFont*	m_pFontItemName[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontItemPrice[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontItemNumber[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontTotalPrice;

	int*			m_pnSelectItemNumber[SHOP_ITEM_TAB_NUMBER];// array, 각 상점별로 선택한 아이템 갯수를 저장한다.
	int				m_nCurrentSelectSlot[SHOP_ITEM_TAB_NUMBER];
	BUILDINGNPC*	m_pBuildingInfo;	

	CD3DHanFont*	m_pInfluenceTex;
};

#endif // !defined(AFX_INFCITYSHOP_H__9B6EB8A3_634C_4255_A627_69015270E88C__INCLUDED_)
