#pragma once

#include "INFBase.h"
#include "INFImageBtn.h"

#define CASH_SHOP_BUTTON_STATE_ALL	3		// 버튼 상태
#define CASH_SHOP_ITEM_FONT			15		// 아이템샵 아이템 설명(이름, 개수, 캐쉬)
#define CASH_SHOP_ITEM_INFO			13		// 아이템샵 아이템 설명(용도)

#define INVEN_ROTATION_NUM		5	// 로테이션 이미지 
#define INVEN_ROTATION_STATE	2	// 로테이션 이미지 


#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define MAX_CASH_SHOP_TAB					7		// 캐쉬샵 전체 탭수+1 (마지막 1개는 더미탭이다.) // 2009-01-28 by bhsohn 캐쉬샵 처리
#else
#define MAX_CASH_SHOP_TAB					11		// 캐쉬샵 전체 탭수+1 (마지막 1개는 더미탭이다.) // 2009-01-28 by bhsohn 캐쉬샵 처리
#endif

#define		CASH_SHOP_VIEW_TAB					MAX_CASH_SHOP_TAB	// 탭의 최대수

#define		MAX_NEW_CASHSHOP_IMAGE				2

class CINFImage;
class CINFImageEx;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CINFGroupImage;
class CINFImageBtn;

struct CashItemIcon
{
	int						nItemNum;
	DataHeader*				pDataHeader;
	CINFImageEx	*			vecImgCashIcon;
};

class CINFCityCashShop: public CINFBase
{
protected:
	CINFCityCashShop() = default;

public:
	CINFCityCashShop(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityCashShop();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2007-08-08 by dgwoo 캐쉬탭에 미러 추가.
	int ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int ProcessRotationUnit(POINT pt, UINT uMsg);
	void SetByPushButton(BOOL bFlag);
	void OnChangeWearItem();
	void InputCashColorShopItem();
	void SendMsgGift(MSG_FC_SHOP_CHECK_GIVE_TARGET_OK* pMsg);

	// 2009-01-28 by bhsohn 캐쉬샵 처리
	//SHOP_ITEM* FindCashShopData(int nTabIndex, SHOP_ITEM* pCashShop);
	SHOP_ITEM* FindCashShopData(int nTabIndex, UINT ItemNum);
	// end 2009-01-28 by bhsohn 캐쉬샵 처리
private:
	// 2007-08-21 by bhsohn 캐쉬 아이템 alt+tab버그 수정
	CINFImageEx* FindCashIcon(int nItemNum, DataHeader*	pDataHeader);

	// 2009-01-28 by bhsohn 캐쉬샵 처리
	void UpdateGiftButton(int i_nCurrentSelectTab, int i_nItemSelect);
	void RefreshGetSkinItemIndex();
	int GetSkinItemIndex();
	int GetNonEmptyNextIndex(int i_nStartIdx);	
	void ReLoadSkinItemVector();
	int GetTabToItemIdx(int i_nIdx);
	void RenderNewShopInfo(int nPosX, int nPosY, SHOP_ITEM* i_pShopInfo);
	// end 2009-01-28 by bhsohn 캐쉬샵 처리

protected:
	// 2011. 11. 17 by jskim EP4 UI 변경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgBack;
	CINFImageBtn*			m_pCloseBtn;
#else
	CINFImageEx	*			m_pImgBack;
#endif
	
	vector< CINFImageEx* > m_vecCINFImageInfo;


	// end 2011. 11. 17 by jskim EP4 UI 변경
	CINFImageEx	*			m_pImgSelectItem;
	CINFImageEx	*			m_pImgTab[CASH_SHOP_VIEW_TAB];
	CINFImageEx	*			m_pImgChargeButton[CASH_SHOP_BUTTON_STATE_ALL];
	CINFImageBtn	*		m_pImgGiftButton;
	CINFImageEx	*			m_pImgBuyButton[CASH_SHOP_BUTTON_STATE_ALL];
	CINFImageEx	*			m_pImgMoreItem[4];

	vector<CashItemIcon>	m_vecImgCashIcon[MAX_CASH_SHOP_TAB];
	CD3DHanFont	*			m_pFontItem[CASH_SHOP_ITEM_FONT];
	CD3DHanFont	*			m_pFontItemInfo[CASH_SHOP_ITEM_INFO];
	BUILDINGNPC	*			m_pBuildingInfo;
	BOOL					m_bInputItemDone;				// InputItemDone이 온 경우 TRUE, 이 값이 TRUE이면 아이템 리스트를 다시 받지 않는다.
	CINFScrollBar*			m_pScroll;
	POINT					m_pointBeforeMousePoints;			// 마우스 이전좌표
	ITEM		*			m_pItem;
	
	int						m_nCurrentMouseStateCharge;		// 캐쉬 충전 버튼 상태 
	BOOL					m_bCurrentMouseStateCharge;	// 캐쉬 충전 버튼 상태 
	int						m_nCurrentMouseStateBuy;		// 사기 버튼 상태
	BOOL					m_bCurrentMouseStateBuy;		// 사기 버튼 상태
	int						m_nMyCashPoint;					// 내 캐쉬 정보(전체 : 캐쉬 + 상품권)
	int						m_nGiftCardPoint;				// 내 캐쉬중 상품권
	int						m_nGiftButtonState;				// 선물버튼 상태.
	
	int						m_nShopOldPosX;
	int						m_nShopOldPosY;
	float					m_fTimeMoreIcon;
	BOOL					m_bMoreItemRenderFlag;

	int						m_nShopTapEndPosX[CASH_SHOP_VIEW_TAB];

	// 2007-08-08 by dgwoo 미러 추가.
	CINFImageEx*			m_pDirection[INVEN_ROTATION_STATE][INVEN_ROTATION_NUM];
	CINFImageEx*			m_pRotationBase;

	BOOL					m_bByPush[4];
	int						m_nRotationState;
	int						m_nButtonState;
	
	BOOL					m_bTurnLeft;
	BOOL					m_bTurnRight;
	BOOL					m_bTurnUp;
	BOOL					m_bTurnDown;

	// 2009-01-28 by bhsohn 캐쉬샵 처리
	int						m_nTabToItemIdx[MAX_CASH_SHOP_TAB];
	int						m_nSkinItemIdx;
	CINFImageEx	*			m_pImgNewTab[MAX_NEW_CASHSHOP_IMAGE];
	int						m_nNewPlayIndex;
	float					m_fNewPlayTime;
	// end 2009-01-28 by bhsohn 캐쉬샵 처리
	// 2010. 01. 27 by ckPark 캐쉬 아이템 한정 판매 시스템
	CINFImageEx*				m_pSoldOut_Item;
	CINFImageEx*				m_pSoldOut_ToolTip;
	CINFImageEx*				m_pLimitedEdtion[ 2 ];

	BOOL					m_bBling;
	DWORD					m_nBlingStartTime;
	// end 2010. 01. 27 by ckPark 캐쉬 아이템 한정 판매 시스템


public:
	BOOL		IsInputItemDone() { return m_bInputItemDone; }
	void		InitData();
	void		InputItemDone();
	void		SendBuyItem(ITEM *pItem);
	void		SendChargeCash();
	int			InputItem(char* pMsg);
	void		SetCashMoney(int nMoney){m_nMyCashPoint = nMoney; }
	void		SetGiftCardCashPoint(int nGiftCardPoint) { m_nGiftCardPoint = nGiftCardPoint; }
	void		RenderDescIcon();

	void		ClearItemList();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	void		RenderDescIcon( POINT pPos, ITEM* pItem, int nWidht, int nHeight );
#endif
	void		CashItemSort();

	// 2010. 01. 27 by ckPark 캐쉬 아이템 한정 판매 시스템
	void		UpdateCashItemRemainCount( ItemNum_t nItemNum, INT nRemainCount );
	// end 2010. 01. 27 by ckPark 캐쉬 아이템 한정 판매 시스템


	int			m_nItemSelect;								// 아이템 선택
	int			m_nShopMovePosX;							// 창이동 포지션
	int			m_nShopMovePosY;							// 창이동 포지션
	int			m_nCurrentSelectTab;
	
	BOOL		m_bItemBuySend;
	BOOL		m_bMoveShop;
	
	UID32_t		m_nGiveTargetCharacterUID;					// 2007-11-15 by dgwoo 선물을 줄 캐릭터의 UID

	vector<SHOP_ITEM*> m_vecItemInfo[MAX_CASH_SHOP_TAB];	
};