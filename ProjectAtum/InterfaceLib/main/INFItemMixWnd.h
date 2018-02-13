// INFItemMixWnd.h: interface for the CINFItemMixWnd class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-02-20 by bhsohn 인게임 조합창 처리
/// \date		2013-02-20 ~ 
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFITEMMIXWND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_)
#define AFX_INFITEMMIXWND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFBase;
class CINFGroupImage;
class CINFEditBox;
class CINFAutoEditBox;
class CINFComboBoxEX;
class CINFListBox;
class CINFArenaScrollBar;

#define MIX_SEARCH_KIND_COUNT				3

// 최대 라인수
#define	MAX_INGAME_SEARCH_X					(COUNT_ITEM_MIXING_SOURCE)
#define	MAX_INGAME_SEARCH_Y					5

// 최대 10페이까지 보여주자
#define	MAX_SEARCH_PAGE						10

// 최대 페이지 관련 버튼 
#define PAGE_BTN_FIRST						0
#define PAGE_BTN_PREV						1
#define PAGE_BTN_NEXT						2
#define PAGE_BTN_LAST						3
#define MAX_PAGE_BTN						4

// 선택 메뉴 커서
#define		WND_TYPE_MIXITEM						0		// 조합창
#define		WND_TYPE_FACTORY						1		// Factory창

// Icone크기값
#define		ICON_WIDTH							28
#define		ICON_HEIGHT							28
#define		ICON_CAPX							20
#define		ICON_CAPY							28

#define MAX_SCROLL_CNT						30						// 최대 페이지

class CINFItemMixWnd  : public CINFBase
{	
protected:
	CINFItemMixWnd() {}
	
public:	
	CINFItemMixWnd(CAtumNode* pParent);
	virtual ~CINFItemMixWnd();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	InitWndData();
	
	virtual void	Tick();
	void			Render();	

	DataHeader* FindResource(char* szRcName);

	void			RenderCommonCtrl();	
	void			RenderSearchItem(int nPage, int nScroll);		
	void			RenderPageCnt();	
	void			RenderCtrl();		// 	

	BOOL IsShowWnd();
	virtual void ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *tMixWndPos=NULL);
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	virtual int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void UpdateBtnPos();
	virtual int OnCilckSelectItem(POINT pt);		// 검색항목 클릭
	virtual INT GetCntInven_ItemNum_To_AllItemCnt( INT nItemNum );

	BOOL IsMouseCaps(POINT ptPos);
	

	// 검색버튼 처리
	void OnClickSearchItem();

	int OnClickResultNameSearch(int nItemCnt);		// 결과로 검색[이름으로 검색]
	int OnClickMaterialNameSearch(int nItemCnt);	// [재료]로 검색

	void UpdateSearchEditList();		// [자동완성] 리스트를 갱신해준다.

	// 검색 타입
	int	 GetSearchType() {return m_nSearchType;}

	// 검색 타입
	void SetSearchItemNum(INT nSearchItemNum) {m_nSearchItemNum = nSearchItemNum;}
	INT GetSearchItemNum() {return m_nSearchItemNum;}

	// 아이템 툴팁 표시
	void SetTooltipItemInfo(INT	nItemNum, int x, int y, int nLinkItem);

	void FindTooltipItemInfo_POS(POINT pt, BOOL* pTarget, int *nFindPosX, int *nFindPosY);
	INT GetTooltipItemInfo_ItemNum(int nPage, int nScroll, BOOL bTarget, int nFindPosX, int nFindPosY, int* pLinkItem);

	INT GetTooltip_Target_ItemInfo_ItemNum(int nPage, int nScroll, int nFindPosX, int nFindPosY);

	void InitItemListScroll();
	void UpdateItemListScroll(int nCurrentPage, int nStartPage);

	void InitPagePosList();
	
	int OnCilckPageCnt(POINT pt);
	

	void OnClickPageBtn(int nPageMode);

	POINT GetBkSize();	
	void SetShowItemNumShowWnd(BOOL bShow, int nShowItemNum, UID64_t uItemUniNum);

protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					       
	CINFGroupImage*		m_pImgBackg;
#endif
	CINFAutoEditBox*		m_pEditItemName;		// Edit박스
	CINFImageBtn*		m_pSearchBtn;			// 버튼
	//CINFComboBoxEX*		m_pComboKindEx;
	CINFListBox*		m_pLBMixKind;
	CINFArenaScrollBar*		m_pScroll;
	

	CD3DHanFont*		m_pItemListFont; // 페이지 폰트

	CD3DHanFont*		m_pConuntFont; // 페이지 폰트

	CINFImageBtn*		m_pCloseBtn;

	char		m_strMixKind[MIX_SEARCH_KIND_COUNT][MAX_PATH];		// 검색 종류

	BOOL		m_bShowWnd;
	
	POINT					m_ptBkPos;	// 윈도우 시작위치
	
	// 윈도우 이동 관련 처리
	BOOL					m_bMove;
	POINT					m_ptCommOpMouse;

	int						m_nSearchType;
	INT						m_nSearchItemNum;

	char					m_szPrice[64];
	vector<CItemInfo*>		m_vecSource;
	vector<CItemInfo*>		m_vecTarget;
	BOOL					m_bShowTarget;

	int						m_nEditElementIndex;

	vector<structResultMixingInfo>		m_vecMaterialList;						// 검색결과
	vector<structResultMixingInfo>		m_vecMixingItemList;		// 재료 결과
	

	vector<structResultMixingInfo>		m_vecRenderItemList;			// 랜더링 할 벡터

	// 아이템 숫자로 검색
	RECT					m_rcShowItem;


	// 재료들 리스트
	RECT					m_rcSearchItem[MAX_INGAME_SEARCH_Y][MAX_INGAME_SEARCH_X];

	// 결과물 리스트
	RECT					m_rcTargetItem[MAX_INGAME_SEARCH_Y];

	INT						m_nMaxShowItemLen;// 최대 라인수 // 아이템 최대 라인수
	int						m_pPageCntBtnPosY;
	int						m_nDiffScrollHeight;

	//////////////////// 페이지 관련 처리 ////////////////////
	// 페이지 위치
	POINT					m_ptPagePosList[MAX_SEARCH_PAGE][MAX_SEARCH_PAGE];

	int						m_nStartPage;		// 현재 그림의 시작 페이지	
	int						m_nCurrentPage;		// 현재 보고 있는 페이지
	int						m_nMaxScrollPage;	// 최대 페이지
	
	CINFImageBtn*			m_pPageBtn[MAX_PAGE_BTN];		// 	

	//////////////////// 선택항목관련 처리 ////////////////////
	RECT				m_rcSearchList[MAX_INGAME_SEARCH_Y];
	CINFImageEx*		m_pSearchIconBk;		
	

	CINFImageEx*		m_pSearchListBk;		// ItemList배경
	CINFImageEx*		m_pSearchListSel;		// 선택한 ItemList배경

	int					m_nSelItem;

	INT					m_nShowItemNum;				
	UID64_t				m_uItemUniNum;
	INT					m_WndType;

	char				m_nBkImageGroup[MAX_PATH];
};
#endif // !defined(AFX_INFITEMMIXWND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_)
