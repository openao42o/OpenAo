// INFInvenExtend.h: interface for the CINFInvenExtend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINVENEXTEND_H__72C927DE_F4B8_40AF_8E52_F525B1E5541E__INCLUDED_)
#define AFX_INFINVENEXTEND_H__72C927DE_F4B8_40AF_8E52_F525B1E5541E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "atumparam.h"
#include "RangeTime.h"

#include "INFInvenItem.h"								 // 2011. 10. 10 by jskim UI시스템 변경



#define INVEN_X_NUMBER	10
#define INVEN_Y_NUMBER	6
#define INVEN_NUMBER	(INVEN_X_NUMBER*INVEN_Y_NUMBER)	// 총 인벤수
#define WEAR_ITEM_NUMBER 9

#define INVEN_ROTATION_NUM		5	// 로테이션 이미지 
#define INVEN_ROTATION_STATE	2	// 로테이션 이미지 

#define INVEN_SLOT_SIZE			30
#define INVEN_SLOT_INTERVAL		32
#define INVEN_ITEM_SLOT_START_X 309
#define INVEN_ITEM_SLOT_START_Y 30

// 인벤창 순서
#define INVEN_ITEM_WND			0	// 아이템 창
#define INVEN_EQ_WND			1	// 장비창


class CGameData;
class CINFImage;
class CD3DHanFont;
class CItemInfo;

class CINFInvenEquip;
class CINFInvenItem;

class CINFItemMenuList;	// 2013-02-26 by bhsohn 인게임 조합 검색 처리

typedef struct 
{	
	INT			CurrentCount;
	UID64_t		UniqueNumber;					// 아이템 고유번호
} structDelItemInfo;

class CINFInvenExtend  : public CINFBase 
{
public:
	CINFInvenExtend(CAtumNode* pParent);
	virtual ~CINFInvenExtend();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void			Render();
	void			RenderOnCityBase();
	void			RenderSpi(int x, int y);
	virtual void	Tick();

	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven = FALSE);
	int				WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void			SetAllIconInfo();
	void			SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo, POINT *pIconPos=NULL);
	void			SetItemSpi(int nSpi){ m_nItemSpi = nSpi; }
	void			AddItemSpi(int nSpi){ m_nItemSpi += nSpi; }
	void			SetItemSpiUniqueNumber(LONGLONG nUniqueNumber) { m_hyItemSpiUniqueNumber = nUniqueNumber; }
	int				GetItemSpi() { return m_nItemSpi; }
	LONGLONG		GetItemSpiUniqueNumber() { return m_hyItemSpiUniqueNumber; }
	void			DeleteSelectItem(int count);
	void			SendUseItem(ITEM_BASE* pItem);
	void			DeleteEnchantItem(UINT nUniqueNumber);
	// 2009-02-03 by bhsohn 장착 아이템 비교 툴팁
	//void			SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y);
	void			SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y, BOOL bShowMyEq=TRUE);
	void			SetEnemyItemInfo(CItemInfo *pItemInfo, int x, int y, BOOL bShowMyEq=TRUE);	// 2012-06-14 by isshin 아템미리보기

	HRESULT			SetResourceFile(BOOL encode,char* szFileName);
	BOOL			IsTradingItem(ITEM_GENERAL* pItem);
	BOOL			IsAbleSetItem(CItemInfo* pItemInfo, int nWindowPosition);
	BOOL			IsAbleReleaseItem(CItemInfo* pItemInfo, int nWindowPosition);
	BOOL			IsAbleChangeItem(CItemInfo* pSetItemInfo,CItemInfo* pReleaseItemInfo,int nWindowPosition);
	void			SetGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue);
	void			ReleaseGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue);
	
	void			InitInven();

	void			SetScrollEndLine();
	void			ReSetScrollCurrentLine();

	// Item 다중 선택 처리
	void			SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo);	
	int				GetInvenFreeSize();
	
	// 다중 선택 창고 부분 보안
	BOOL			IsInvenHave(int nItemNum);
	
	// 아이템 삭제시, 메모리 버그 수정
	void			InitDeleteItemInfo();
	void			SetDeleteItemInfo(CItemInfo* pSelectItem);		

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	void			ShowInven(POINT *pItem, POINT *pEq, BOOL bClick=FALSE, BOOL bShop=FALSE);
	BOOL			IsShowInven();
	INVEN_DISPLAY_INFO* GetInvenDisplayInfo(int nIdx);
	void			SetWearPosition(int iWearPosition);	
	void			ClearMultiSeletItem();// 다중 선택 추가 보안
	void			SetSingleInvenIconInfo(CItemInfo* pItemInfo);
	INVEN_DISPLAY_INFO* GetWearDisplayInfo(int nIdx);
	UID64_t			GetDeleteItemUID();
	void			SendChangeWearWindowPos(int nWindowPosition);	
	void			ShowEqInven();
	void			ShowAllInven(BOOL bShow);
	void			ShowInvenAndMirror(BOOL i_bInven,BOOL i_bMirror = FALSE );
	
	// 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정
	INVEN_DISPLAY_INFO* GetInvenDisplayInfoToUniqueNum(UID64_t nUniqueNum);
	// end 2013-01-17 by jhjang 아이템 드래그중 아이템 사용시 드래그중인 아이템 정보가 바뀌는 버그 수정

	
	// 장착 아이템	
	void			RenderMirror(POINT *pMirrorPos=NULL);
	BOOL			IsEquipInvenShow();
	D3DXMATRIX		GetInvenMatInven();
	CD3DHanFont*	GetFontSpi();
	CD3DHanFont*	GetFontWeight(int nIdx);

	void			UpdateInvenScrollMax();
	void			SetWndOrder(int nWndIdx);
	POINT			GetEqInvenBkPos();

	void			SetInvenPosInfo(structInvenPosInfo* pstruInvenPosInfo);
	structInvenPosInfo		GetInvenPosInfo();

	// 2010. 04. 01 by ckPark 리소스 변경 시스템시 팩토리나 연구소에서 아이템 회수한 후 변경이 안되는 문제 해결
	void			UpdateIcon( CItemInfo* pItemInfo );
	// end 2010. 04. 01 by ckPark 리소스 변경 시스템시 팩토리나 연구소에서 아이템 회수한 후 변경이 안되는 문제 해결

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착아이템 슬롯 정보 포인터 리턴.
	INVEN_DISPLAY_INFO *rtnPtr_AttachmentItemInfo ( const INT a_nPosKind );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	POINT			GetBkSize();

	BOOL			GetInvenMove();	
#endif

protected:
	int				GetScrollLine();
	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);

	int				GetWearItemNum();
	void			InitInvenIconInfo();
	void			InitWearIconInfo();
	
	
	
	void			SetSingleWearIconInfo(CItemInfo* pItemInfo);	

	// 스피커 아이템 추가
	void			DoModalChatAllInfluence(UID64_t		UniqueNumber);

	// 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	void			RenderLuckyMechine();
	// end 2009-04-02 by bhsohn 럭키 머신 추가 기획안
	
public:
	//CGameData *		m_pGameData;
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;
	CINFImageEx	*	m_pItemSl;	
	
	CD3DHanFont *	m_pFontSpi;
	CD3DHanFont *	m_pFontWeight[3];
	CD3DHanFont *	m_pFontItemNum[INVEN_Y_NUMBER];
	int				m_nItemSpi;
	LONGLONG		m_hyItemSpiUniqueNumber;

	// [0][1] ... [5]
	// [6] ...
	// [12] ...
	INVEN_DISPLAY_INFO *m_pInvenDisplayInfo[INVEN_NUMBER];
	//	[2]	[0]	[4]	
	//	[1]	[5]	[3]
	//	[8]	[6]	[7]

	// 2010-06-15 by shcho&hslee 펫시스템 - 장착품 최대 개수 변경.
	//INVEN_DISPLAY_INFO *m_pWearDisplayInfo[WEAR_ITEM_NUMBER];
	INVEN_DISPLAY_INFO *m_pWearDisplayInfo[MAX_EQUIP_POS];
	
	//int					m_nInvenCurrentScroll;
	//int					m_nInvenCurrentScrollPosition;
	
	POINT				m_ptMouse;

	CINFImageEx*			m_pSelectIcon;// 마우스 클릭에 의해 선택된 아이콘 이미지(reference)
	CItemInfo*			m_pSelectItem;// 아이템 포인터(reference)
	
	// 아이템 삭제시, 메모리 버그 수정
	//CItemInfo*			m_pDeleteItem;// 지울 아이템 포인터
	structDelItemInfo		m_struDeleteItem;

//	int					m_nSelectItemWindowPosition;// 클릭된 아이템의 Window에서의 위치 // 2006-06-21 by ispark, m_SelectItem->ItemWindowIndex 이 변수를 쓰기로 함

	BOOL				m_bEnchantState;
	CItemInfo*			m_pEnchantItem;

	int					m_nRenderMoveIconIntervalHeight;
	int					m_nRenderMoveIconIntervalWidth;

	INVEN_DISPLAY_INFO *m_pCurrentItemInfo;// 현재 보여지고 있는 아이템 정보
	
	BOOL				m_bSelectWearItem;
	BOOL				m_bMouseDownState;

	//////////////////////////////////////////////////////////////////////////	
	// 수정 인벤토리 이미지 (기존이미지는 상점및 교환창 변경후 완전 삭제 요망
	// ydkim 2005.11.04
	// 완전 수정후 함수정리 필요	

	// 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정
	enum { SHOP_CONCURRENT_DELAY = 200, };
	DWORD				m_bShopConcurrent;
	// end 2011-07-22 by hsson 샾 중복 접속으로 인챈트 되던 버그 수정

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	// Item 메뉴 리스트
	void OnClickItemMenuListWnd(BOOL bShow, POINT ptPoint, INT nItemNum, UID64_t uItemUniNum);
	int WndProcItemMenuListWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RenderItemMenuListWnd();	
	void SetBkPos(POINT ptBkPos);
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2013-03-18 by bhsohn 팩토리 조합 추가
	POINT GetBkPos();	
	// END 2013-03-18 by bhsohn 팩토리 조합 추가
		
protected:

	// 2008-08-22 by bhsohn EP3 인벤토리 처리
	// 인벤토리
	CINFInvenEquip*		m_pINFInvenEquip;	// 장착창
	CINFInvenItem*		m_pINFInvenItem;	// 아이템 창
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	deque<int>			m_vecWndOrder;

	POINT				m_pEqIvenNormalPos;
	BOOL				m_bEqSetPos;		// 위치를 지정했냐?

	POINT				m_pEqIvenShopPos;
	BOOL				m_bEqShopSetPos;		// 위치를 지정했냐?
 	BOOL				m_bShowEqWnd;		// 일반 장비창
 	BOOL				m_bShowEqShopWnd;	// 상점 장비창
#endif
	
	POINT				m_pItemIvenPos;	
	BOOL				m_bItemSetPos;		// 위치를 지정했냐?

	CINFItemMenuList*			m_pItemMenuList;// 2013-02-26 by bhsohn 인게임 조합 검색 처리

};

#endif // !defined(AFX_INFINVENEXTEND_H__72C927DE_F4B8_40AF_8E52_F525B1E5541E__INCLUDED_)
