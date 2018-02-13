// INFCityBazaar.h: interface for the CINFCityBazaar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYBAZAAR_H__500337C6_49C9_4801_AF8B_CC7F7D26F2EB__INCLUDED_)
#define AFX_INFCITYBAZAAR_H__500337C6_49C9_4801_AF8B_CC7F7D26F2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFComboBoxEX.h"
#include "ChatTab_t.h"

#define BAZAARSHOP_ITEMCOUNT		16
#define ITEMINFO_ITEM_FULL_NAME		140	// 140 = desc title len(20) + prefix len + item name len + suffix len

/*--------------------------------------------------------------------------*/
//버튼의 상태.
#define	BTN_STATE_OVER					0
#define	BTN_STATE_CLICK					1
#define	BTN_STATE_NORMAL				3
#define	BTN_STATE_DEACTIVE				2
/*--------------------------------------------------------------------------*/

enum {
		BAZAAR_VISIT_BUY_COUNT = 1,				//방문자 수량
		BAZAAR_VISIT_BUY_PRICE,					//방문자 가격
		BAZAAR_BTN,								//등록 취소
		BAZAAR_BTN_VISIT_OUT,					//나가는 버튼.
		BAZAAR_ICON,							//아이템 아이콘
		BAZAAR_VISIT_SEL_COUNT,					//

		BAZAAR_OPEN_1_BUY,
		BAZAAR_OPEN_3_BUY,
		BAZAAR_OPEN_2_BUY,
		BAZAAR_OPEN_4_BUY,
		
		BAZAAR_OPEN_2_SEL,
		BAZAAR_OPEN_3_SEL,

		BAZAAR_SHOP_WINDOW,						//상점창 윈도우즈 사이즈.
		BAZAAR_SHOP_TAB,						//탭 사이즈
		BAZAAR_SHOP_SCROLL,						//오픈 스크롤위치.
		BAZAAR_SHOP_VISIT_SCROLL,				//방문자 스크롤위치
		BAZAAR_SHOP_TAB_WHEEL,					//탭안의 사이즈
		BAZAAR_SHOP_OPEN_START,					//시작 
		BAZAAR_SHOP_OPEN_STOP,					//중단.
		BAZAAR_SHOP_ITEM,						//한개의 아이템
		BAZAAR_OPEN_NAME,						//방제
		BAZAAR_SHOP_ITEM_AREA,					//아이템들의 영역.
		BAZAAR_SHOP_MIN,						//최소화버튼 영역.
		BAZAAR_SHOP_ACT							//활성화 버튼 영역.
		

};

class CINFImage;
class CINFImageEx;
class CD3DHanFont;
class CItemInfo;
class CINFGroupImage;

class CINFCityBazaar : public CINFBase  
{
public:
	CINFCityBazaar();
	virtual ~CINFCityBazaar();

	virtual HRESULT InitDeviceObjects()				{return S_OK;}
	virtual HRESULT RestoreDeviceObjects()			{return S_OK;}
	virtual HRESULT DeleteDeviceObjects()			{return S_OK;}
	virtual HRESULT InvalidateDeviceObjects()		{return S_OK;}

	virtual void	Render()														{}
	virtual void	Render_Disable()												{}
	virtual void	Tick()															{}
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void	Recv_OpenShopOK()												{}
	virtual	void	Send_SellBuyItem(BOOL bRequest = FALSE)							{}
	virtual void	Recv_RegSellItemOK(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK* pMsg)		{}
	virtual void	Recv_DelSellItemOK(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK* pMsg)		{}
	virtual void	Recv_SetSellItem(SBAZAAR_SELL_ITEM * pSellItem)					{}
	virtual void	Recv_SetSellEnchantItem(UID64_t itemUID, int EnchantItemNum)	{}
	virtual void	Recv_SellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg)		{}
	virtual void	Recv_BuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg)		{}

	virtual void	Recv_RegBuyItemOK(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK* pMsg)			{}
	virtual void	Recv_DelBuyItemOK(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK* pMsg)		{}
	virtual void	Recv_SetBuyItem(SBAZAAR_BUY_ITEM * pBuyItem)					{}

	virtual void	Recv_CustomerInfoOK(MSG_FC_BAZAAR_CUSTOMER_INFO_OK* pMsg)		{}
	virtual void	InitOpenData()													{}
	virtual void	InitVisitData()													{}

	virtual int		GetShopItemCount(UID64_t itemUID);
	virtual CItemInfo *	GetShopItemInfo(UID64_t itemUID);

	// 2007-11-01 by bhsohn 상점 이용중에 대한 처리
	virtual void	CloseVisitShop();

	BOOL			GetbBazaarShopStart()			{return m_bBazaarShopStart;}
	BOOL			GetbBazaarShopEnd()				{return m_bBazaarShopEnd;}
	void			SetbBazaarShopEnd(BOOL bEnd)	{m_bBazaarShopEnd = bEnd;}
	BYTE			GetbBazaarShopType()			{return m_byBazaarType;}
	BYTE			GetbBazaarShopTypeState()		{return m_byBazaarTypeState;}
	ClientIndex_t	GetBazaarClientIndex()			{return m_ClientIndex;}
	BOOL			GetBazaarDisable()				{return !m_bBazaarMinAct;}

	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);
	BOOL			GetPointInType(POINT &Pt,int Type,int Num = 0);
	int				GetScrollLine();
	void			SetScrollEndLine();
	void			InitScrollLine();

	void			SetItemInfo(CItemInfo* pSelectItem, int x, int y);
	void			UnderBarDrawText(CD3DHanFont *pHanFont,FLOAT sx,FLOAT sy,DWORD dwColor,TCHAR* strText,DWORD dwFlags);

protected:
	BYTE			m_byBazaarType;				//1 : Sell ,  2 : Buy;
	BYTE			m_byBazaarTypeState;		//1 : Open,	  2 : Visit;

	/*--------------------------------------------------------------------------*/
	// 2006-07-25 by dgwoo
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*	m_pBackWindow;					//바탕 배경.
#else
	CINFImageEx	*	m_pBackWindow;					//바탕 배경.
#endif
	CINFImageEx	*	m_pPutItemBtn[4];				//아이템 등록 버튼 이미지 
	CINFImageEx	*	m_pDelItemBtn[4];				//아이템 취소 버튼 이미지
	CINFImageEx	*	m_pStartBtn[4];					//상점 시작 버튼 이미지
	CINFImageEx	*	m_pStopBtn[4];					//상점 중단 버튼 이미지
	CINFImageEx	*	m_pMinBtn[4];					//최소화 버튼.
	CINFImageEx	*	m_pActBtn[4];					//활성화 버튼.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx	*	m_pMinBar1;						//최소화시 바(판매 상점)
#endif
	CINFImageEx	*	m_pMinBar;						//최소화시 바.

	CINFImageEx	*	m_pScrollButton;				// 스크롤 버튼

	int				m_nStartBtnState;				//시작 버튼 상태
	int				m_nStopBtnState;				//중단 버튼 상태
	int				m_nMinBtnState;					//바자창의 최소화 & 활성화 버튼의 상태.
	int				m_nOutBtnState;					//나가기 버튼 상태.
	int				m_nPutDelState[5];				//등록 삭제 버튼의 상태.


	/*--------------------------------------------------------------------------*/
	CItemInfo	*	m_pCurrentItemInfo;				// 현재 보이는 아이템 정보

	/*--------------------------------------------------------------------------*/
	int				m_nScrollBtn;					// 스크롤 누른 위치
	int				m_nScrollPosition;				// 스크롤 위치
	int				m_nScrollLine;					// 스크롤 라인
	int				m_nSelectIndex;					// 선택 라인

	BOOL			m_bLButtonDown;
	BOOL			m_bScrollLock;

	int				m_nScrollMaxCount;
	int				m_nScrollMaxLine;
	
	/*--------------------------------------------------------------------------*/
	// 상점 상태
	BOOL			m_bBazaarShopStart;				// 상점 시작
	BOOL			m_bBazaarShopEnd;				// 상점 종료
	BOOL			m_bBazaarMinAct;				//상점이 비활성화 : 0,활성화 : 1

	/*--------------------------------------------------------------------------*/
	// 기타
	ClientIndex_t	m_ClientIndex;				// 상점 오픈한 클라이언트 인덱스
};



//////////////////////////////////////////////////////////////////////////
// 개인 상점 개설자
enum {TEXT_NON = 0, TEXT_NAME, TEXT_COUNT, TEXT_PRICE};
enum {COMBO_NORMAL = 0,COMBO_UNDER,COMBO_CLICK};
enum {OPEN_START = 1, OPEN_END, OPEN_REGISTER, OPEN_CANCEL};

typedef struct OPENITEM
{
	CItemInfo *		pItemBase;
	int				nItemCount;
	int				nItemPrice;
	int				nItemTotal;

	int				nItemIndex;

	BOOL			bRegister;				//0: 취소 , 1: 등록

	// 2007-11-14 by bhsohn 바자 상점에 무기를 올렸다 취소시, 무기 총알 갯수 잘못나오는 버그 수정
	INT				nBulletCount;			// 무기: 남은 발수, 에너지: 남은 개수

	OPENITEM()
	{
		pItemBase		= NULL;
		nItemCount		= 1;
		nItemPrice		= 0;
		nItemTotal		= 0;
		nItemIndex		= 0;
		bRegister		= FALSE;

		// 2007-11-14 by bhsohn 바자 상점에 무기를 올렸다 취소시, 무기 총알 갯수 잘못나오는 버그 수정
		nBulletCount	= 0;
	}

	~OPENITEM()
	{
		pItemBase		= NULL;
	}
	
} OPEN_BAZAAR_ITEM;

class CINFCityBazaarOpen : public CINFCityBazaar
{
public:
	CINFCityBazaarOpen(CAtumNode* pParent, CGameData* m_pGameData, int nBazaarType);
	virtual ~CINFCityBazaarOpen();

	virtual void	InitOpenData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	virtual void	Render();
	virtual void	Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void	Recv_RegSellItemOK(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK* pMsg);
	virtual void	Recv_DelSellItemOK(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK* pMsg);
	virtual void	Recv_RegBuyItemOK(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK* pMsg);
	virtual void	Recv_DelBuyItemOK(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK* pMsg);
	virtual void	Recv_OpenShopOK();
	virtual void	Recv_SellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg);
	virtual void	Recv_BuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg);
	virtual void	Recv_CustomerInfoOK(MSG_FC_BAZAAR_CUSTOMER_INFO_OK* pMsg);

	virtual int		GetShopItemCount(UID64_t itemUID);
	virtual CItemInfo *	GetShopItemInfo(UID64_t itemUID);

private:
	void			Render_Common();
	void			Render_Sell();
	void			Render_Buy();
	void			Render_Log();
	void			Render_Disable();

	int				WndProc_Sell(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Buy(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Common(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Disable(UINT uMsg,WPARAM wParam, LPARAM lParam);
	

	void			Send_OpenShop();
	void			Send_CloseShop();
	void			Send_DelItem(int nSelectItem);
	void			Send_RegItem(int nSelectItem);

	void			OnSellItem(CItemInfo* pSelectItem);
	void			OnSellDelItem(UID64_t itemUID);
	void			OnBuyItem(int ItemNum, int ItemKind);
	void			OnReadyBuyItem();
	void			OnButtonClicked(int nButton);

	void			SetDataCombo(int nComboActive);
	void			SetSelectItem(CItemInfo* pSelectItem, POINT pt);

	int				GetItemIndexByPos(int nPosY);
	bool			PointInRect(POINT &pt,RenderRect &rt);

	OPEN_BAZAAR_ITEM * FindItem(CItemInfo* pItemInfo);
	OPEN_BAZAAR_ITEM * FindItem(UID64_t itemUID);
	vector<OPEN_BAZAAR_ITEM>::iterator FindItemItr(UID64_t itemUID);
	OPEN_BAZAAR_ITEM * FindItem(int nItemNum, BYTE nItemType = 1, BOOL bRegister = FALSE);
	vector<OPEN_BAZAAR_ITEM>::iterator FindItemItr(int nItemNum, BYTE nItemType = 1, BOOL bRegister = FALSE);

	void			CheckBtnState();
	void			CheckSellItemCount();
	BOOL			CheckBuyCountItem(int nKind, int nItemNum);
	void			CheckLogMsg(char* msgLog, DWORD dwColor);
	BOOL			CheckOnSellItem(CItemInfo* pSelectItem);
	BOOL			CheckOnBuyItem(int nItemNum, int nItemKind);

	void			CalTotalPrice();

protected:
	CINFImageEx *		m_pShopName;				//상점의 이름 이미지.
	CINFImageEx *		m_pBackTab[2];				//탭 배경
	CINFImageEx *		m_pBackList;				//리스트 배경
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx *		m_pBackListBox;				//리스트 배경
#endif
	CINFImageEx *		m_pBackItem;				//활성화된 아이템의 배경.
	CINFImageEx *		m_pSelectBar;				// 선택 바
	CINFImageEx *		m_pBackTotal;				//총액 배경.
	CINFImageEx *		m_pArrowIcon[2];			//comboBox화살표 아이콘.
	CINFImageEx *		m_pBackCombo;				//콤보박스.
	CINFImageEx *		m_pBackComboScroll;			//콤보박스 스크롤.
	CINFImageEx *		m_pBackComboSel;			//콤보박스에서 선택아이템.

	CD3DHanFont *	m_pFontItemName;			//아이템 이름
	CD3DHanFont *	m_pFontItemPrice;			//아이템 가격
	CD3DHanFont	*	m_pFontItemCount;			//아이템 수량
	CD3DHanFont *	m_pFontItemSum;				//아이템 총 가격 
	CD3DHanFont *	m_pFontName;				//방제
	CD3DHanFont *	m_pFontTotal;				//총액
	CD3DHanFont *	m_pFontLogMsg;				//로그
	CD3DHanFont	*	m_pFontShopType;			//판매상점, 구매상점.

	/*--------------------------------------------------------------------------*/
	BYTE			m_byTabIndex;	
	int				m_nTotal;					//총액
	char			m_strInputMessage[SIZE_MAX_BAZAAR_NAME];		//이름
	int				m_nComboState[5][2];						//ComboBox Arrow Icon State
	int				m_nComboActive;								//콤보박스 활성화.0:비활성화.1:Kind 2:Name

	int				m_nLogMaxCount;				
	
	/*--------------------------------------------------------------------------*/
	// 키 입력
	BYTE			m_byFocusText;
	BOOL			m_bSelectItem;

	/*--------------------------------------------------------------------------*/
	// 기타
	INVEN_DISPLAY_INFO m_DisplayInfo;

private:
	vector<OPEN_BAZAAR_ITEM> m_vecOpenBazaarItem;
	CINFComboBoxEX		*	m_pComboKindEx;						//종류 콤보
	CINFComboBoxEX		*	m_pComboNameEx;						//종류 콤보

	ChatTab_t		m_LogChat;					// 로그

};


//////////////////////////////////////////////////////////////////////////
// 개인 상점 방문자
typedef struct VISITITEM
{
	char			cItemName[ITEMINFO_ITEM_FULL_NAME];
	CItemInfo*		pItemInfo;

	int				nAmount0;
	int				nEachPrice0;

	int				nCurrentCount;

	int				nItemIndex;

	BOOL			bExistence;

	VISITITEM()
	{
		memset(cItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);
		pItemInfo		= NULL;

		nAmount0		= 0;
		nEachPrice0		= 0;

		nCurrentCount	= 0;
		nItemIndex		= 0;

		bExistence		= FALSE;
	}

	~VISITITEM()
	{
		memset(cItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);
		pItemInfo		= NULL;
	}

} VISIT_BAZAAR_ITEM;

class CINFCityBazaarVisit : public CINFCityBazaar
{
public:
	CINFCityBazaarVisit(CAtumNode* pParent, CGameData* m_pGameData, int nBazaarType, ClientIndex_t clientIndex);
	virtual ~CINFCityBazaarVisit();

	virtual void	InitVisitData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	virtual void	Render();
	virtual void	Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual void	Recv_SetSellItem(SBAZAAR_SELL_ITEM *pSellItem);
	virtual void	Recv_SetSellEnchantItem(UID64_t itemUID, int EnchantItemNum);
	virtual void	Recv_SellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg);
	virtual void	Recv_SetBuyItem(SBAZAAR_BUY_ITEM * pBuyItem);
	virtual void	Recv_BuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg);

	virtual	void	Send_SellBuyItem(BOOL bRequest = FALSE);
	
	// 2007-11-01 by bhsohn 상점 이용중에 대한 처리
	virtual	void	CloseVisitShop();


private:
	void			Render_Common();
	void			Render_Sell();
	void			Render_Buy();

	int				WndProc_Sell(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Buy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int				GetItemIndexByPos(int nPosY);
	void			CheckItemCount();

	// 2007-11-01 by bhsohn 상점 이용중에 대한 처리
	//void			CloseVisitShop();

	void			SetItemFullName(VISIT_BAZAAR_ITEM* pVisitItem);

	VISIT_BAZAAR_ITEM * FindItem(UID64_t itemUID);
	vector<VISIT_BAZAAR_ITEM>::iterator FindItemItr(UID64_t itemUID);
	VISIT_BAZAAR_ITEM * FindItem(int nItemIndex);
	vector<VISIT_BAZAAR_ITEM>::iterator FindItemItr(int nItemIndex);

protected:
	CINFImageEx *		m_pSum;						//합계 배경.
	CINFImageEx *		m_pBackList;				//리스트 배경.
	CINFImageEx *		m_pBackTab;					//탭 배경.
	CINFImageEx *		m_pBackItem;				//아이템 배경.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI시스템 변경
	CINFImageEx *		m_pBackListBox;				//리스트 배경
#endif
	CINFImageEx *		m_pSelectBar;				// 선택 바
	CINFImageEx *		m_pOKBtn[4];				//구입, 판매 버튼
	
	CD3DHanFont *	m_pFontItemName;			//아이템 이름
	CD3DHanFont *	m_pFontItemPrice;			//아아템 가격
	CD3DHanFont *	m_pFontItemCount;			//아이템 수량 
	CD3DHanFont *	m_pFontBuySellItemCount;	//아이템 판매, 구매 수량 
	CD3DHanFont *	m_pFontSumPrice;								//전체 가격

	int				m_nSum;											//합계.
	int				m_nOKState[5];									//구입 판매 버튼 상태.
	int				m_byFocusText;				//텍스트 활성화.

	vector<VISIT_BAZAAR_ITEM> m_vecVisitBazaarItem;


};

#endif // !defined(AFX_INFCITYBAZAAR_H__500337C6_49C9_4801_AF8B_CC7F7D26F2EB__INCLUDED_)
