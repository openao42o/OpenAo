// INFCityInfinityShop.h: interface for the CINFCityInfinityShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYINFINITYSHOP_H__5D106B53_DF7A_43D3_AAE1_52006A959933__INCLUDED_)
#define AFX_INFCITYINFINITYSHOP_H__5D106B53_DF7A_43D3_AAE1_52006A959933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFDefaultWnd.h"

#define	MAX_ORB_KIND	5

// 인피니티 교환 아이템 가격
struct ItemPrice
{
	ItemNum_t				ItemNum;		// 필요 아이템
	InfinityShopItemCnt_t	ItemCount;		// 필요 아이템 갯수

	ItemPrice( ItemNum_t nItemNum = 0, InfinityShopItemCnt_t nItemCount = 0 ) : ItemNum( nItemNum ), ItemCount( nItemCount )
	{

	}
};

// 인피니티 교환 아이템 구매 정보
struct BuyInfo
{
	InfinityShopUID_t	ItemUID;		// 구매 아이템 UID
	ItemNum_t			ItemNum;		// 구매 아이템 번호
	int					ItemCount;		// 갯수
	std::vector<ItemPrice>	PriceInfo;	// 가격 정보

	BuyInfo() : ItemUID( 0 ), ItemNum( 0 ), ItemCount( 0 )
	{

	}

	void	Clear( void )
	{
		ItemUID = ItemNum = ItemCount = 0;
		PriceInfo.clear();
	}
};

class CINFArenaScrollBar;

class CINFCityInfinityShop  : public CINFDefaultWnd
{
private:
	CINFImageBtn*	m_pOKButton;
	CINFImageBtn*	m_pCancelButton;

	CINFImageEx*		m_pTab[SHOP_ITEM_TAB_NUMBER];

	CINFImageEx*		m_pSlot;
	CINFImageEx*		m_pSelectSlot;
	CINFImageEx*		m_pCostBGLarge;
	CINFImageEx*		m_pCostBGSmall;

	CINFImageBtn*	m_pScrollUpBtn;
	CINFImageBtn*	m_pScrollDownBtn;
	CINFArenaScrollBar*	m_pScroll;

	CD3DHanFont*	m_pFont;
	CD3DHanFont*	m_pFontSmall;

	int				m_nCurrentTab;
	int				m_nSelectSlot[SHOP_ITEM_TAB_NUMBER];	// 선택한 슬롯
	int				m_nScrollStep[SHOP_ITEM_TAB_NUMBER];	// 스크롤

	std::vector<INFINITY_SHOP_INFO>	m_vecItem[SHOP_ITEM_TAB_NUMBER];	// 아이템 목록
	BuyInfo			m_BuyInfo;								// 구매 정보

	BOOL			m_bItemInputDone;						// 아이템 목록을 서버에서 다 받아왔는가?

public:
	CINFCityInfinityShop( CAtumNode* pParent );
	virtual ~CINFCityInfinityShop();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	UpdateBtnPos( void );
	
	virtual	void	Render();

	int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void	InitData( void );
	void	ClearItemInfo( void );
	void	ClearBuyInfo( void );

	void	GetOrbInfo( const INFINITY_SHOP_INFO* pItem, const int nIndex, ItemNum_t* pItemNum, InfinityShopItemCnt_t* pItemCount );

	void	AddItemInfo( INFINITY_SHOP_INFO* pItem );
	void	AddBuyInfo( INFINITY_SHOP_INFO* pItem, int nCount );

	BOOL	GetItemInputDone( void )
	{
		return m_bItemInputDone;
	}
	
	void	ItemInputDone( void )
	{
		m_bItemInputDone = TRUE;
	}

	BuyInfo	GetBuyInfo( void )
	{
		return m_BuyInfo;
	}

	BOOL	FindInfinityShopInfo_From_CurrentTab( ItemNum_t nItemNum, INFINITY_SHOP_INFO* pInfo );
};

#endif // !defined(AFX_INFCITYINFINITYSHOP_H__5D106B53_DF7A_43D3_AAE1_52006A959933__INCLUDED_)
