// INFItemMixWnd.cpp: implementation of the CINFItemMixWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFGameMain.h"
#include "AtumApplication.h"
#include "INFCityBase.h"
#include "GameDataLast.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "StoreData.h"
#include "Chat.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"
#include "FieldWinSocket.h"
#include "INFWindow.h"
#include "ItemInfo.h"
#include "AtumSound.h"
#include "Skill.h"
#include "Interface.h"
#include "INFGroupManager.h"
#include "INFGroupImage.h"
#include "INFToolTip.h"

#include "INFItemMixWnd.h"

#include "INFEditBox.h"
#include "INFAutoEditBox.h"
#include "INFImageBtn.h"
#include "INFComboBoxEX.h"
#include "INFListBox.h"
#include "INFArenaScrollBar.h"
#include "INFIcon.h"
#include "INFInvenExtend.h"

#include "INFItemInfo.h"

#define INF_MIX_WINDOWS_WIDTH			520														// 전체 넒이
#define INF_MIX_WINDOWS_HEIGTH			500														// 전체 높이

// Edit박스
#define ITEM_MIX_EDIT_X						74//54
#define ITEM_MIX_EDIT_Y						65//134
#define ITEM_MIX_EDIT_W						270
#define ITEM_MIX_EDIT_H						20
#define ITEM_MIX_EDIT_ELEMENT_Y				27
#define ITEM_MIX_READ_FONT_LINE_HEIGHT		17

// 검색버튼
#define MIX_BTN_POS_X						348//330
#define MIX_BTN_POS_Y						60//120

//리스트 박스
// #define MIX_COUNT_MAIN_LB_X					(m_ptBkPos.x + 382)
// #define MIX_COUNT_MAIN_LB_Y					(m_ptBkPos.y + 134)
// #define MIX_COUNT_ELEMENT_LB_X				(m_ptBkPos.x + 384)
// #define MIX_COUNT_ELEMENT_LB_Y				(m_ptBkPos.y + 154)
#define MIX_COUNT_MAIN_LB_X					(m_ptBkPos.x + 386)
#define MIX_COUNT_MAIN_LB_Y					(m_ptBkPos.y + 72)
#define MIX_COUNT_ELEMENT_LB_X				(m_ptBkPos.x + 392)//(m_ptBkPos.x + 394)
#define MIX_COUNT_ELEMENT_LB_Y				(m_ptBkPos.y + 92)
#define MIX_COUNT_MAIN_LB_W					(100)
#define MIX_COUNT_MAIN_LB_H					(17)
#define MIX_COUNT_ELEMENT_LB_W				(70)
#define MIX_COUNT_ELEMENT_LB_H				(15)//(13)

// 스크롤바
#define INF_MIX_SCROLL_COUNT				(m_nMaxShowItemLen)
#define INF_MIX_SCROLL_X					(m_ptBkPos.x + 483)
#define INF_MIX_SCROLL_Y					(m_ptBkPos.y + 146)
#define INF_MIX_SCROLL_W					13
#define INF_MIX_SCROLL_H					265		// 2013-01-07 by mspark, 채팅방 셀렉트 문제 해결 - 기존 163에서 180으로 수정
#define INF_MIX_SCROLL_WHELL_L				(m_ptBkPos.x + 35)
#define INF_MIX_SCROLL_WHELL_T				(m_ptBkPos.y + 140)
#define INF_MIX_SCROLL_WHELL_R				(m_ptBkPos.x + 500)
#define INF_MIX_SCROLL_WHELL_B				(m_ptBkPos.y + 450)

#define INF_MIX_SCROLL_BALL_L				(m_ptBkPos.x + 479)		
#define INF_MIX_SCROLL_BALL_T				(m_ptBkPos.y + 143)	
#define	INF_MIX_SCROLL_BALL_R				(m_ptBkPos.x + 490)
#define INF_MIX_SCROLL_BALL_B				(m_ptBkPos.y + 450)	




// 선택 메뉴 커서
#define LIST_BOX_SEARCH_ALLITEM					0
#define LIST_BOX_SEARCH_RESULT					1
#define LIST_BOX_SEARCH_MATERIAL				2

//ShowItem 항목
#define		SHOW_ITEML_POS_X				30
#define		SHOW_ITEML_POS_Y				59

// Search ItemList 시작 지점
#define		SEARCH_ITEMLIST_POS_X				21//40
#define		SEARCH_ITEMLIST_POS_Y				149//180
#define		SEARCH_ITEMLIST_POS_WIDTH			467

// 목적 아이템
#define		SEARCH_ITEMTARGET_POS_X				409//40
#define		SEARCH_ITEMTARGET_POS_Y				7//180

#define		SEARCH_MATERIAL_POS_X				40
#define		SEARCH_MATERIAL_POS_Y				7//180



// Close버튼
#define		OPTION_BTN_CLOSE_X				500
#define		OPTION_BTN_CLOSE_Y				7


#define		PAGE_LIST_WIDTH					30
#define		PAGE_LIST_HEIGHT				30

// Page표시
#define		PAGE_CNT_POSX					125
#define		PAGE_CNT_POSY					433	//437

// 선택 메뉴 커서
#define SEARCH_TYPE_ITEMNUM						0		// ItemNum으로 검색
#define SEARCH_TYPE_NAME						1		// ItemName으로 검색

extern CAtumDatabase			*g_pDatabase;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFItemMixWnd::CINFItemMixWnd(CAtumNode* pParent)
{
	CINFItemMixWnd::InitWndData();
}

void CINFItemMixWnd::InitWndData()
{
	m_pImgBackg = NULL;
	m_pEditItemName = NULL;
	m_bShowWnd = FALSE;
	m_pSearchBtn = NULL;	
	//m_pComboKindEx = NULL;
	
	m_pLBMixKind = NULL;

	m_pScroll = NULL;

	m_pConuntFont = NULL;
	m_pItemListFont = NULL;

	wsprintf(m_strMixKind[0], STRMSG_C_130222_0001);
	wsprintf(m_strMixKind[1], STRMSG_C_130222_0002);
	wsprintf(m_strMixKind[2], STRMSG_C_130222_0003);

	m_ptBkPos.x = m_ptBkPos.y = 0;
	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;

	m_nSearchType = SEARCH_TYPE_ITEMNUM;
	m_nSearchItemNum = 0;

	m_nEditElementIndex = -1;

	m_vecMixingItemList.clear();		// 검색결과
	m_vecMaterialList.clear();			// 재료결과
	m_vecRenderItemList.clear();		// 랜더링 할 벡터

	m_pSearchListBk = NULL;
	m_pSearchListSel = NULL;
	m_pSearchIconBk = NULL;
	
	m_pCloseBtn = NULL;

	m_rcShowItem.left		= SHOW_ITEML_POS_X;
	m_rcShowItem.top		= SHOW_ITEML_POS_Y;
	m_rcShowItem.right		= m_rcShowItem.left + ICON_WIDTH;
	m_rcShowItem.bottom		= m_rcShowItem.top + ICON_HEIGHT;
	{
		int nCntY;
		nCntY = 0;
		for(nCntY=0;nCntY< MAX_INGAME_SEARCH_Y;nCntY++)
		{
			m_rcSearchList[nCntY].left		= SEARCH_ITEMLIST_POS_X;
			m_rcSearchList[nCntY].top		= SEARCH_ITEMLIST_POS_Y + (ICON_HEIGHT*nCntY) + (ICON_CAPY*nCntY);
			m_rcSearchList[nCntY].right		= m_rcSearchList[nCntY].left + SEARCH_ITEMLIST_POS_WIDTH;
			m_rcSearchList[nCntY].bottom	= m_rcSearchList[nCntY].top	+ ICON_HEIGHT+ICON_CAPY;

			m_rcTargetItem[nCntY].left		= m_rcSearchList[nCntY].left	+ SEARCH_ITEMTARGET_POS_X;
			m_rcTargetItem[nCntY].top		= m_rcSearchList[nCntY].top		+ SEARCH_ITEMTARGET_POS_Y;											
			m_rcTargetItem[nCntY].right		= m_rcTargetItem[nCntY].left + ICON_WIDTH;
			m_rcTargetItem[nCntY].bottom	= m_rcTargetItem[nCntY].top	+ ICON_HEIGHT;			
		}	
	}
	
	{
		int nCntX, nCntY;
		nCntX = nCntY = 0;
		for(nCntY=0;nCntY< MAX_INGAME_SEARCH_Y;nCntY++)
		{
			for(nCntX=0;nCntX< MAX_INGAME_SEARCH_X;nCntX++)
			{
				m_rcSearchItem[nCntY][nCntX].left	= m_rcSearchList[nCntY].left + SEARCH_MATERIAL_POS_X 
					+ (ICON_WIDTH*nCntX) + (ICON_CAPX*nCntX);
				m_rcSearchItem[nCntY][nCntX].top	= m_rcSearchList[nCntY].top + SEARCH_MATERIAL_POS_Y;
				m_rcSearchItem[nCntY][nCntX].right	= m_rcSearchItem[nCntY][nCntX].left + ICON_WIDTH;
				m_rcSearchItem[nCntY][nCntX].bottom	= m_rcSearchItem[nCntY][nCntX].top	+ ICON_HEIGHT;
			}
		}	
	}

	InitPagePosList();
	m_nCurrentPage = 1;
	m_nStartPage = 1;
	m_nMaxScrollPage = 1;
	m_nSelItem = -1;

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		m_pPageBtn[nCnt] = NULL;
	}

	m_nShowItemNum = 0;
	m_uItemUniNum = 0;
	m_nDiffScrollHeight = 0;
	
	m_nMaxShowItemLen = MAX_INGAME_SEARCH_Y;	// 최대 라인수
	m_pPageCntBtnPosY = PAGE_CNT_POSY;
	m_WndType = WND_TYPE_MIXITEM;

	memset(m_nBkImageGroup, 0x00, MAX_PATH);
	sprintf(m_nBkImageGroup, "recipe");
}

void CINFItemMixWnd::InitPagePosList()
{
	int nCntX, nCntY;
	nCntX = nCntY = 0;
	for(nCntY=0;nCntY< MAX_SEARCH_PAGE;nCntY++)
	{
		for(nCntX=0;nCntX< MAX_SEARCH_PAGE;nCntX++)
		{
			m_ptPagePosList[nCntY][nCntX].x = -1;
			m_ptPagePosList[nCntY][nCntX].y = -1;
		}
	}
	
	
	POINT	ptPagePosList[MAX_SEARCH_PAGE][MAX_SEARCH_PAGE] = 
	{
		// 1자리
		{					
			{PAGE_LIST_WIDTH/2 + 4*PAGE_LIST_WIDTH, 0},								
				
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},
		// 2 자리
		{					
			{4*PAGE_LIST_WIDTH, 0},				
			{5*PAGE_LIST_WIDTH, 0},				
			
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},
		// 3자리
		{					
			{PAGE_LIST_WIDTH/2 + 3*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 4*PAGE_LIST_WIDTH, 0},				
			{PAGE_LIST_WIDTH/2 + 5*PAGE_LIST_WIDTH, 0},				
			
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},	
		//4자리
		{					
			{3*PAGE_LIST_WIDTH, 0},
			{4*PAGE_LIST_WIDTH, 0},				
			{5*PAGE_LIST_WIDTH, 0},
			{6*PAGE_LIST_WIDTH, 0},				
			
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},	
		// 5자리
		{					
			{PAGE_LIST_WIDTH/2 + 2*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 3*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 4*PAGE_LIST_WIDTH, 0},				
			{PAGE_LIST_WIDTH/2 + 5*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 6*PAGE_LIST_WIDTH, 0},				
			
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},		
		// 6자리
		{								
			{2*PAGE_LIST_WIDTH, 0},
			{3*PAGE_LIST_WIDTH, 0},
			{4*PAGE_LIST_WIDTH, 0},
			
			{5*PAGE_LIST_WIDTH, 0},
			{6*PAGE_LIST_WIDTH, 0},
			{7*PAGE_LIST_WIDTH, 0},				
			
			{-1, -1},
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},
		// 7자리
		{	
			{PAGE_LIST_WIDTH/2 + PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 2*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 3*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 4*PAGE_LIST_WIDTH, 0},
			
			{PAGE_LIST_WIDTH/2 + 5*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 6*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 7*PAGE_LIST_WIDTH, 0},
			
			{-1, -1},
			{-1, -1},
			{-1, -1},
		},		
		// 8 자리
		{			
			{PAGE_LIST_WIDTH, 0},
			{2*PAGE_LIST_WIDTH, 0},
			{3*PAGE_LIST_WIDTH, 0},
			{4*PAGE_LIST_WIDTH, 0},
			
			{5*PAGE_LIST_WIDTH, 0},
			{6*PAGE_LIST_WIDTH, 0},
			{7*PAGE_LIST_WIDTH, 0},
			{8*PAGE_LIST_WIDTH, 0},
			
			{-1, -1},
			{-1, -1},
		},		
		// 9자리
		{
			{PAGE_LIST_WIDTH/2,	0},
			{PAGE_LIST_WIDTH/2 + PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 2*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 3*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 4*PAGE_LIST_WIDTH, 0},
			
			{PAGE_LIST_WIDTH/2 + 5*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 6*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 7*PAGE_LIST_WIDTH, 0},
			{PAGE_LIST_WIDTH/2 + 8*PAGE_LIST_WIDTH, 0},
			{-1, -1},
		},
		// 10자리 표시
		{
			{0,	0},
			{PAGE_LIST_WIDTH, 0},
			{2*PAGE_LIST_WIDTH, 0},
			{3*PAGE_LIST_WIDTH, 0},
			{4*PAGE_LIST_WIDTH, 0},
			
			{5*PAGE_LIST_WIDTH, 0},
			{6*PAGE_LIST_WIDTH, 0},
			{7*PAGE_LIST_WIDTH, 0},
			{8*PAGE_LIST_WIDTH, 0},
			{9*PAGE_LIST_WIDTH, 0},
			
		},		
		
	};
	memcpy(m_ptPagePosList, ptPagePosList, sizeof(POINT)*MAX_SEARCH_PAGE*MAX_SEARCH_PAGE);
}

CINFItemMixWnd::~CINFItemMixWnd()
{
	util::del(m_pImgBackg);
	util::del(m_pEditItemName);
	util::del(m_pSearchBtn);	 
	//util::del(m_pComboKindEx);
	util::del(m_pScroll);

	util::del(m_pSearchListBk);
	util::del(m_pSearchListSel);
	util::del(m_pSearchIconBk);
	
	util::del(m_pConuntFont);
	util::del(m_pItemListFont);

	util::del(m_pCloseBtn);

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		util::del(m_pPageBtn[nCnt]);
	}


}

HRESULT CINFItemMixWnd::InitDeviceObjects()
{
	DataHeader *pDataHeader = NULL;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        
	if(NULL == m_pImgBackg)
	{
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource(m_nBkImageGroup);
		m_pImgBackg = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pImgBackg->InitDeviceObjects( g_pD3dApp->m_pImageList );		
	}
#endif

	{
		if(NULL == m_pEditItemName)
		{
			m_pEditItemName = new CINFAutoEditBox;
		}
		POINT ptPos = {ITEM_MIX_EDIT_X, ITEM_MIX_EDIT_Y};
		m_pEditItemName->InitDeviceObjects(9, ptPos, ITEM_MIX_EDIT_W, TRUE, ITEM_MIX_READ_FONT_LINE_HEIGHT);				
		m_pEditItemName->SetStringMaxBuff(SIZE_MAX_ITEM_NAME);
		// by mspark, 인게임 조합
		m_pEditItemName->SetElementArea(ITEM_MIX_EDIT_X, ITEM_MIX_EDIT_ELEMENT_Y+ITEM_MIX_EDIT_ELEMENT_Y, ITEM_MIX_EDIT_W, ITEM_MIX_EDIT_H);
		m_pEditItemName->SetBGPos(ITEM_MIX_EDIT_X, ITEM_MIX_EDIT_ELEMENT_Y+ITEM_MIX_EDIT_ELEMENT_Y, ITEM_MIX_EDIT_W, ITEM_MIX_EDIT_H);
		// by mspark, 인게임 조합
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "reci_btn3");
		wsprintf(szDownBtn, "reci_btn1");
		wsprintf(szSelBtn, "reci_btn0");
		wsprintf(szDisBtn, "reci_btn2");
		if(NULL == m_pSearchBtn)
		{
			m_pSearchBtn = new CINFImageBtn;
		}
		m_pSearchBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

// 	if(NULL == m_pComboKindEx)
// 	{
// 		m_pComboKindEx = new CINFComboBoxEX(this, m_pGameData);
// 		m_pComboKindEx->CreateImage("Name","SelName","c_scrlb");
// 		m_pComboKindEx->InitDeviceObjects();
// 	}

	if(m_pLBMixKind == NULL)
	{
//		m_pLBMixKind = new CINFListBox("cbopa","s_combo");		
//		m_pLBMixKind = new CINFListBox("cbarenaba","cbamode");
		m_pLBMixKind = new CINFListBox("cbarena","cbarenab");		
		m_pLBMixKind->InitDeviceObjects();
	}

	int i;
	char szTemp[MAX_PATH];
	
	for(i = 0; i < MIX_SEARCH_KIND_COUNT; i++)
	{
		memset(szTemp,0x00,MAX_PATH);
		sprintf(szTemp,"%s",m_strMixKind[i]);
		m_pLBMixKind->AddElement(szTemp);
	}
	m_pLBMixKind->SetSelectItem(0);

	if(NULL == m_pScroll)
	{
		m_pScroll = new CINFArenaScrollBar;
		m_pScroll->InitDeviceObjects(INF_MIX_SCROLL_COUNT,"c_scrlb");
		m_pScroll->SetMaxItem(1);
	}

	{
		char buf[16];
		memset(buf, 0x00, 16);		
		wsprintf(buf,"itemsl");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pSearchIconBk))
		{
			m_pSearchIconBk = new CINFImageEx;
			m_pSearchIconBk->InitDeviceObjects(pDataHeader);
		}
		
	}	
	{
		char buf[16];
		memset(buf, 0x00, 16);		
		wsprintf(buf,"reci_slot");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pSearchListBk))
		{
			m_pSearchListBk = new CINFImageEx;
			m_pSearchListBk->InitDeviceObjects(pDataHeader);
		}
		
	}
	
	{
		char buf[16];
		memset(buf, 0x00, 16);		
		wsprintf(buf,"reci_sel");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pSearchListSel))
		{
			m_pSearchListSel = new CINFImageEx;
			m_pSearchListSel->InitDeviceObjects(pDataHeader);
		}
		
	}

	if(NULL == m_pConuntFont)
	{
		m_pConuntFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pConuntFont->InitDeviceObjects(g_pD3dDev) ;
	}	
	if(NULL == m_pItemListFont)
	{
		m_pItemListFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE, TRUE,1024,32);
		m_pItemListFont->InitDeviceObjects(g_pD3dDev) ;
	}

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
		wsprintf(szUpBtn, "close");
		wsprintf(szDownBtn, "close");
		wsprintf(szSelBtn, "close");
		wsprintf(szDisBtn, "close");															  
#else
		wsprintf(szUpBtn, "xclose");
		wsprintf(szDownBtn, "xclose");
		wsprintf(szSelBtn, "xclose");
		wsprintf(szDisBtn, "xclose");
#endif
		if(NULL == m_pCloseBtn)
		{
			m_pCloseBtn = new CINFImageBtn;
		}
		m_pCloseBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		char szUpBtn[MAX_PAGE_BTN][30], szDownBtn[MAX_PAGE_BTN][30], szSelBtn[MAX_PAGE_BTN][30], szDisBtn[MAX_PAGE_BTN][30];

		wsprintf(szUpBtn[PAGE_BTN_FIRST], "rec_pagef3");
		wsprintf(szDownBtn[PAGE_BTN_FIRST], "rec_pagef1");
		wsprintf(szSelBtn[PAGE_BTN_FIRST], "rec_pagef0");
		wsprintf(szDisBtn[PAGE_BTN_FIRST], "rec_pagef2");

		wsprintf(szUpBtn[PAGE_BTN_PREV], "call3");
		wsprintf(szDownBtn[PAGE_BTN_PREV], "call1");
		wsprintf(szSelBtn[PAGE_BTN_PREV], "call0");
		wsprintf(szDisBtn[PAGE_BTN_PREV], "call2");

		wsprintf(szUpBtn[PAGE_BTN_NEXT], "calr3");
		wsprintf(szDownBtn[PAGE_BTN_NEXT], "calr1");
		wsprintf(szSelBtn[PAGE_BTN_NEXT], "calr0");
		wsprintf(szDisBtn[PAGE_BTN_NEXT], "calr2");

		wsprintf(szUpBtn[PAGE_BTN_LAST], "rec_pagee3");
		wsprintf(szDownBtn[PAGE_BTN_LAST], "rec_pagee1");
		wsprintf(szSelBtn[PAGE_BTN_LAST], "rec_pagee0");
		wsprintf(szDisBtn[PAGE_BTN_LAST], "rec_pagee2");

		if(NULL == m_pPageBtn[nCnt])
		{
			m_pPageBtn[nCnt] = new CINFImageBtn;
		}
		m_pPageBtn[nCnt]->InitDeviceObjects(szUpBtn[nCnt], szDownBtn[nCnt], szSelBtn[nCnt], szDisBtn[nCnt]);		
	}

	return S_OK ;
}
HRESULT CINFItemMixWnd::RestoreDeviceObjects()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		

	if(m_pImgBackg)
	{
		m_pImgBackg->RestoreDeviceObjects( );		
	}
	if(m_pEditItemName)
	{
		m_pEditItemName->RestoreDeviceObjects();		
	}

	if(m_pSearchBtn)
	{				
		m_pSearchBtn->RestoreDeviceObjects();				
	}
	if(m_pLBMixKind)
	{
		m_pLBMixKind->RestoreDeviceObjects();
	}

	if(m_pScroll)
	{
		m_pScroll->RestoreDeviceObjects();
	}	
	
	if(m_pSearchIconBk )
	{
		m_pSearchIconBk->RestoreDeviceObjects();
	}
	if(m_pSearchListBk)
	{
		m_pSearchListBk->RestoreDeviceObjects();
	}
	if(m_pSearchListSel)
	{
		m_pSearchListSel->RestoreDeviceObjects();
	}

	if(m_pConuntFont)
	{
		m_pConuntFont->RestoreDeviceObjects();
	}
	
	if(m_pItemListFont)
	{
		m_pItemListFont->RestoreDeviceObjects();
	}

	if(m_pCloseBtn)
	{
		m_pCloseBtn->RestoreDeviceObjects();		
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		if(m_pPageBtn[nCnt])
		{
			m_pPageBtn[nCnt]->RestoreDeviceObjects();		
		}
	}

	// RestoreDeviceObjects해주고 버튼, UI위치 수정 해주자
	UpdateBtnPos();
	return S_OK ;
}
HRESULT CINFItemMixWnd::InvalidateDeviceObjects()
{
	if(m_pImgBackg)
	{
		m_pImgBackg->InvalidateDeviceObjects();		
	}
	if(m_pEditItemName)
	{
		m_pEditItemName->InvalidateDeviceObjects();
	}

	if(m_pSearchBtn)
	{
		m_pSearchBtn->InvalidateDeviceObjects();		
	}

	if(m_pLBMixKind)
	{
		m_pLBMixKind->InvalidateDeviceObjects();
	}

	if(m_pScroll)
	{
		m_pScroll->InvalidateDeviceObjects();
	}
	if(m_pSearchIconBk )
	{
		m_pSearchIconBk->InvalidateDeviceObjects();
	}
	if(m_pSearchListBk)
	{
		m_pSearchListBk->InvalidateDeviceObjects();
	}
	if(m_pSearchListSel)
	{
		m_pSearchListSel->InvalidateDeviceObjects();
	}
	if(m_pConuntFont)
	{
		m_pConuntFont->InvalidateDeviceObjects();
	}
	if(m_pItemListFont)
	{
		m_pItemListFont->InvalidateDeviceObjects();
	}
	if(m_pCloseBtn)
	{
		m_pCloseBtn->InvalidateDeviceObjects();		
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		if(m_pPageBtn[nCnt])
		{
			m_pPageBtn[nCnt]->InvalidateDeviceObjects();		
		}
		
	}
	return S_OK ;
}
HRESULT CINFItemMixWnd::DeleteDeviceObjects()
{
	if(m_pImgBackg)
	{
		m_pImgBackg->DeleteDeviceObjects();
		util::del(m_pImgBackg);
	}
	if(m_pEditItemName)
	{
		m_pEditItemName->DeleteDeviceObjects();
		util::del(m_pEditItemName);		
	}

	if(m_pSearchBtn)
	{
		m_pSearchBtn->DeleteDeviceObjects();	
		util::del(m_pSearchBtn);
	}

	if(m_pLBMixKind)
	{
		m_pLBMixKind->DeleteDeviceObjects();
		util::del(m_pLBMixKind);
	}

	if(m_pScroll)
	{
		m_pScroll->DeleteDeviceObjects();
		util::del(m_pScroll);
	}
	if(m_pSearchIconBk )
	{		
		m_pSearchIconBk->DeleteDeviceObjects();
		util::del(m_pSearchIconBk);
	}
	if(m_pSearchListBk)
	{		
		m_pSearchListBk->DeleteDeviceObjects();
		util::del(m_pSearchListBk);
	}
	if(m_pSearchListSel)
	{		
		m_pSearchListSel->DeleteDeviceObjects();
		util::del(m_pSearchListSel);
	}

	if(m_pConuntFont)
	{
		m_pConuntFont->DeleteDeviceObjects();
		util::del(m_pConuntFont);
	}
	if(m_pItemListFont)
	{
		m_pItemListFont->DeleteDeviceObjects();
		util::del(m_pItemListFont);
	}
	
	if(m_pCloseBtn)
	{
		m_pCloseBtn->DeleteDeviceObjects();
		util::del(m_pCloseBtn);
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		if(m_pPageBtn[nCnt])
		{			
			m_pPageBtn[nCnt]->DeleteDeviceObjects();
			util::del(m_pPageBtn[nCnt]);
		}
		
	}
	return S_OK ;
}

void CINFItemMixWnd::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}

	if(m_pSearchBtn)
	{
		m_pSearchBtn->Tick();
	}
	m_pEditItemName->Tick();
}

void CINFItemMixWnd::Render()
{
	if(!IsShowWnd())
	{
		return;
	}

	RenderCommonCtrl();		// 컨트롤들 디스 플레이

	RenderSearchItem(m_nCurrentPage, m_pScroll->GetScrollStep());		// 검색결과 디스플레이
	RenderPageCnt();

	RenderCtrl();		// 	
}

void CINFItemMixWnd::RenderCtrl()
{
	{
		m_pEditItemName->Render();
	}
	if(m_pLBMixKind)
	{	
		m_pLBMixKind->SetMainArea(MIX_COUNT_MAIN_LB_X,MIX_COUNT_MAIN_LB_Y,MIX_COUNT_MAIN_LB_W,MIX_COUNT_MAIN_LB_H);
		m_pLBMixKind->SetElementArea(MIX_COUNT_ELEMENT_LB_X,MIX_COUNT_ELEMENT_LB_Y,MIX_COUNT_ELEMENT_LB_W,MIX_COUNT_ELEMENT_LB_H);
		
		m_pLBMixKind->Render();
	}	

	switch(GetSearchType())
	{
	case SEARCH_TYPE_ITEMNUM:
		{
			if(m_nShowItemNum > 0)
			{
				CINFIcon* pIconInfo = g_pGameMain->m_pIcon;
				ITEM* pShowItem = g_pDatabase->GetItemInfoLoadItemData(m_nShowItemNum);		
				if(pShowItem)
				{
					int nIconStartX = m_ptBkPos.x + m_rcShowItem.left;
					int nIconStartY = m_ptBkPos.y + m_rcShowItem.top;

					char chIconName[32] = {0,};	
					if(m_pSearchIconBk )
					{
						m_pSearchIconBk->Move(nIconStartX, nIconStartY);
						m_pSearchIconBk->Render();
					}
					{
						sprintf( chIconName, "%08d", SKILL_BASE_SOURCEINDEX( pShowItem->SourceIndex ) );	
						
						pIconInfo->SetIcon(chIconName, 
							nIconStartX + 1,
							nIconStartY + 1,
							1.0f);				
						pIconInfo->Render();					
					}
					
				}
			}

		}
		break;	
	}
}

void CINFItemMixWnd::RenderCommonCtrl()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		
	{
		m_pImgBackg->Move( nWindowPosX, nWindowPosY);
		m_pImgBackg->Render();
	}
	
	if(m_pSearchBtn)
	{
		m_pSearchBtn->Render();
	}		
	
	if(m_pScroll)
	{
		{
			RECT rtWhell;
			rtWhell.left		= INF_MIX_SCROLL_WHELL_L;
			rtWhell.top			= INF_MIX_SCROLL_WHELL_T;
			
			rtWhell.right		= INF_MIX_SCROLL_WHELL_R;
			rtWhell.bottom		= INF_MIX_SCROLL_WHELL_B-m_nDiffScrollHeight;		
			m_pScroll->SetMouseWhellRect(rtWhell);
		}
		
		m_pScroll->SetPosition(INF_MIX_SCROLL_X,INF_MIX_SCROLL_Y,INF_MIX_SCROLL_W,INF_MIX_SCROLL_H-m_nDiffScrollHeight);		
		
		RECT rtMousePos;
		
		rtMousePos.left		= INF_MIX_SCROLL_BALL_L;
		rtMousePos.top		= INF_MIX_SCROLL_BALL_T;		
		rtMousePos.right	= INF_MIX_SCROLL_BALL_R;
		rtMousePos.bottom	= INF_MIX_SCROLL_BALL_B-m_nDiffScrollHeight;
		
		m_pScroll->SetMouseBallRect(rtMousePos);
		
		m_pScroll->Render();
	}
	

	if(m_pCloseBtn)
	{
		m_pCloseBtn->Render();
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		if(m_pPageBtn[nCnt])
		{			
			m_pPageBtn[nCnt]->Render();			
		}
		
	}
}

void CINFItemMixWnd::RenderSearchItem(int nPage, int nScroll)
{
	CINFIcon* pIconInfo = g_pGameMain->m_pIcon;
	if(!pIconInfo)
	{
		return;
	}	
	
	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;

	// 배경 Render
	{
		int nLine = 0;
		for(nLine =0;nLine < m_nMaxShowItemLen; nLine++)
		{
			m_pSearchListBk->Move(nStartX + m_rcSearchList[nLine].left, 
				nStartY + m_rcSearchList[nLine].top);
			m_pSearchListBk->Render();			
			int nSelItem = m_nSelItem -m_pScroll->GetScrollStep();
			if(nLine == nSelItem)
			{
				m_pSearchListSel->Move(nStartX + m_rcSearchList[nSelItem].left, 
					nStartY + m_rcSearchList[nSelItem].top);
				m_pSearchListSel->Render();
			}
		}		
	}
	
	{
		//////////// 검색 아이템 리스트 ////////////		
		int nCnt = 0;
		char chIconName[32] = {0,};	
		
		
		int nMaterialListCnt = 0;		
		int nMaterialCnt = 0;	
		int nPosX = 0;
		int nLine = 0;
		int nMaxLine = m_nMaxShowItemLen;

//		int nStartPos = (nPage*nMaxLine) + nScroll;
		int nStartPos = nScroll;
		if(nPage <= m_nMaxScrollPage )
		{
			nStartPos = (max(nPage-1, 0)*MAX_SCROLL_CNT)+nScroll;
		}		

		int nPos =0;
		
		vector<structResultMixingInfo>::iterator it = m_vecRenderItemList.begin();
		while(it != m_vecRenderItemList.end())
		{			
			if(nPos >= nStartPos)
			{
				break;
			}
			nPos++;
			it++;
		}	
		
		while(it != m_vecRenderItemList.end())
		{	
			if(nMaxLine <= nLine)
			{
				break;
			}

			nPosX = 0;
			ITEM_MIXING_INFO struMixingInfo = (*it).struMixingInfo;

			ITEM* pTargetItem = g_pDatabase->GetItemInfoLoadItemData(struMixingInfo.TargetItemNum);		
			if(!pTargetItem)
			{
				it++;
				continue;
			}			

			// 재료 
			for(nCnt=(COUNT_ITEM_MIXING_SOURCE-1); nCnt >= 0 ;nCnt--)
			{				
				ITEM* pSourceItem = g_pDatabase->GetItemInfoLoadItemData(struMixingInfo.SourceItem[nCnt].ItemNum);		
				if(!pSourceItem)
				{
//					nPosX++;
					continue;
				}
				if(m_pSearchIconBk )
				{
					// 배경
					m_pSearchIconBk->Move(nStartX + m_rcSearchItem[nLine][nPosX].left, nStartY + m_rcSearchItem[nLine][nPosX].top);
					m_pSearchIconBk->Render();
					
					if(m_pItemListFont)
					{
						
						char chTxt[64];
						wsprintf(chTxt, "%d/%d", 
//										g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt(struMixingInfo.SourceItem[nCnt].ItemNum), 
										GetCntInven_ItemNum_To_AllItemCnt(struMixingInfo.SourceItem[nCnt].ItemNum), 
										struMixingInfo.SourceItem[nCnt].Count);
						SIZE szSize = m_pItemListFont->GetStringSize(chTxt);

						// 가운데 정렬
						int nTmpStartX = nStartX + m_rcSearchItem[nLine][nPosX].left	+ (m_pSearchIconBk->GetImgSize().x/2 -szSize.cx/2);
						int nTmpStartY = nStartY + m_rcSearchItem[nLine][nPosX].bottom	+ 1;
						
						m_pItemListFont->DrawText(nTmpStartX,nTmpStartY,
													GUI_FONT_COLOR_W, 
													chTxt);
						
					}
				}

				{
					// Icon표시
					// 2013-03-13 by bhsohn 스킬 번호 구조 수정
//					sprintf( chIconName, "%08d", SKILL_BASE_NUM( pSourceItem->SourceIndex ) );	
					sprintf( chIconName, "%08d", SKILL_BASE_SOURCEINDEX( pSourceItem->SourceIndex ) );	
					pIconInfo->SetIcon(chIconName, 
						nStartX + m_rcSearchItem[nLine][nPosX].left + 1,
						nStartY + m_rcSearchItem[nLine][nPosX].top + 1, 
						1.0f);				
					pIconInfo->Render();					
				}

				if(m_pConuntFont)
				{
					// 가격 표시
#ifndef _ENHANCED_MIXING_DISPLAY
					char strtemp2[64], strtemp1[64];					
					strtemp2[0] = strtemp1[0] = NULL;

					wsprintf(strtemp1, "%d", struMixingInfo.MixingCost);
					MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );

					SIZE szSize = m_pConuntFont->GetStringSize(strtemp2);					
					
					// 가운데 정렬
					int nTmpStartX = nStartX + 380	-szSize.cx;
					int nTmpStartY = nStartY + m_rcTargetItem[nLine].top + 8;
					
					m_pConuntFont->DrawText(nTmpStartX,nTmpStartY,
												GUI_FONT_COLOR_W, 
												strtemp2);
#else
					char strtemp2[64], strtemp1[64], strDraw[512];					
					strtemp2[0] = strtemp1[0] = strDraw[0] = NULL;

					if (struMixingInfo.MixingCost == 0)
					{
						wsprintf(strtemp2, "Free");
					}
					else
					{
						wsprintf(strtemp1, "%d", struMixingInfo.MixingCost);
						MakeCurrencySeparator(strtemp2, strtemp1, 3, ',');
					}
					if (struMixingInfo.MixingProbability == 10000 || struMixingInfo.MixingProbability == 10000)
					{
						wsprintf(strDraw, "%s (\\g%d%%\\g)", strtemp2, struMixingInfo.MixingProbability / 100);
					}
					else
					{
						wsprintf(strDraw, "%s (%d%%)", strtemp2, (int)(struMixingInfo.MixingProbability / 100));
					}

					SIZE szSize = m_pConuntFont->GetStringSize(strDraw);

					// 가운데 정렬
					int nTmpStartX = nStartX + 420 - szSize.cx;
					int nTmpStartY = nStartY + m_rcTargetItem[nLine].top + 8;

					m_pConuntFont->DrawText(nTmpStartX, nTmpStartY,
						GUI_FONT_COLOR_W,
						strDraw);
#endif
					
				}
				nPosX++;
			}
			
			// 결과물 아이템 표시
			{
				if(m_pSearchIconBk )
				{
					m_pSearchIconBk->Move(nStartX + m_rcTargetItem[nLine].left, nStartY + m_rcTargetItem[nLine].top);
					m_pSearchIconBk->Render();
				}

				{
					int nSourceIndex = pTargetItem->SourceIndex;
					if((*it).LinkItem > 0)
					{
						ITEM* pTmpItem = g_pDatabase->GetItemInfoLoadItemData((*it).LinkItem);		
						if(pTmpItem)
						{
							nSourceIndex = pTmpItem->SourceIndex;
						}						
					}
//					sprintf( chIconName, "%08d", SKILL_BASE_SOURCEINDEX( pTargetItem->SourceIndex ) );	
					sprintf( chIconName, "%08d", SKILL_BASE_SOURCEINDEX( nSourceIndex) );	
					
					pIconInfo->SetIcon(chIconName, 
						nStartX + m_rcTargetItem[nLine].left + 1,
						nStartY + m_rcTargetItem[nLine].top + 1, 
						1.0f);				
					pIconInfo->Render();					
				}
				if(m_pItemListFont)
				{					
					char chTxt[64];
					wsprintf(chTxt, "1");
						
					SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
					
					// 가운데 정렬
					int nTmpStartX = nStartX + m_rcTargetItem[nLine].left	+ (m_pSearchIconBk->GetImgSize().x/2 -szSize.cx/2);
					int nTmpStartY = nStartY + m_rcTargetItem[nLine].bottom	+ 1;
					
					m_pItemListFont->DrawText(nTmpStartX,nTmpStartY,
						GUI_FONT_COLOR_W, 
						chTxt);
					
				}
			}
			nLine++;
			it++;			
		}	
		
	}

	
	if(m_pConuntFont)
	{
		int nStartX = m_ptBkPos.x;
		int nStartY = m_ptBkPos.y;
		
		char chTxt[256];
		if(0 == m_vecRenderItemList.size())
		{
			wsprintf(chTxt, STRMSG_C_130222_0005); // "(검색결과:%d)"
			SIZE szSize = m_pConuntFont->GetStringSize(chTxt);
			// 오른쪽 정렬
			int nTmpStartX = nStartX + 13 + 240/*Width*/ - (szSize.cx/2);
			int nTmpStartY = nStartY + 170;
			
			m_pConuntFont->DrawText(nTmpStartX,nTmpStartY,
				GUI_FONT_COLOR_W, 
				chTxt);
			
		}
		
		{
			wsprintf(chTxt, STRMSG_C_130222_0004, m_vecRenderItemList.size()); // "(검색결과:%d)"
			SIZE szSize = m_pConuntFont->GetStringSize(chTxt);
			
			// 오른쪽 정렬
			int nTmpStartX = nStartX + 500	- (szSize.cx);
			int nTmpStartY = nStartY + 109;
			
			m_pConuntFont->DrawText(nTmpStartX,nTmpStartY,
				GUI_FONT_COLOR_W, 
				chTxt);
		}
		
	}
}

void CINFItemMixWnd::RenderPageCnt()
{	
	if(NULL == m_pConuntFont)
	{
		return;
	}
	{
		int nStartX = m_ptBkPos.x + PAGE_CNT_POSX;
		int nStartY = m_ptBkPos.y + m_pPageCntBtnPosY;

		int nPageCnt = min(MAX_SEARCH_PAGE, m_nMaxScrollPage-m_nStartPage+1);
		nPageCnt = max(1, nPageCnt);

		int nCnt = 0;

		char chTxt[64];				
		for(nCnt = 0;nCnt < nPageCnt;nCnt++)
		{
			if(-1 == m_ptPagePosList[nPageCnt-1][nCnt].x)
			{
				break;
			}
			DWORD dwColor = GUI_FONT_COLOR_W;
			int nShowPage = m_nStartPage+nCnt;
			if(nShowPage == m_nCurrentPage)
			{
				dwColor = GUI_FONT_COLOR_Y;
			}
			wsprintf(chTxt, "%d", nShowPage);
			m_pConuntFont->DrawText(nStartX+m_ptPagePosList[nPageCnt-1][nCnt].x,
									nStartY+m_ptPagePosList[nPageCnt-1][nCnt].y,
									dwColor, 
									chTxt);
		}		
	}
}


BOOL CINFItemMixWnd::IsShowWnd()
{
	return m_bShowWnd;
}

void CINFItemMixWnd::ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *ptMixWndPos/*=NULL*/)
{	
	SetShowItemNumShowWnd(bShow, nShowItemNum, uItemUniNum);
	m_bShowWnd = bShow;
	if(bShow)
	{
		m_bMove = FALSE;
		m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;		

// 		m_ptBkPos.x = ( g_pD3dApp->GetBackBufferDesc().Width/2) - (m_pImgBackg->GetImgSize().x / 2);
// 		m_ptBkPos.y = ( g_pD3dApp->GetBackBufferDesc().Height/2) - ( m_pImgBackg->GetImgSize().y/2);		
		if(ptMixWndPos)
		{
			m_ptBkPos = *ptMixWndPos;
		}
		else
		{
			m_ptBkPos.x = ( g_pD3dApp->GetBackBufferDesc().Width/2) - (m_pImgBackg->GetImgSize().x / 2);
			m_ptBkPos.y = ( g_pD3dApp->GetBackBufferDesc().Height/2) - ( m_pImgBackg->GetImgSize().y/2);		
		}
		
		m_pLBMixKind->SetSelectItem(LIST_BOX_SEARCH_ALLITEM);		// 전체 선택

		UpdateBtnPos();		

		g_pInterface->SetWindowOrder(WNDItemMix);		
	}
	else
	{
		SetTooltipItemInfo(0, 0, 0, 0); // 툴팁 초기화
		m_pEditItemName->EnableEdit(FALSE, FALSE);
	}
}
void CINFItemMixWnd::SetShowItemNumShowWnd(BOOL bShow, int nShowItemNum, UID64_t uItemUniNum)
{
	m_nShowItemNum = nShowItemNum;
	m_uItemUniNum = uItemUniNum;
	if(bShow)
	{
		char chItemName[SIZE_MAX_ITEM_NAME] = {0,};		
		if(m_nShowItemNum > 0)
		{
			m_nSearchType = SEARCH_TYPE_ITEMNUM;
			
			ITEM* pItem = g_pDatabase->GetItemInfoLoadItemData(m_nShowItemNum);
			if(pItem)
			{
				strncpy(chItemName, pItem->ItemName, SIZE_MAX_ITEM_NAME);			
			}			
		}
		else
		{
			m_nSearchType = SEARCH_TYPE_NAME;			
		}
		m_pEditItemName->SetString(chItemName, SIZE_MAX_ITEM_NAME);	
		OnClickSearchItem();
	}

}


int CINFItemMixWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}

	if(m_pScroll)
	{
		if(m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}

	switch(uMsg)
	{		
	case WM_MOUSEWHEEL:
		{
			return OnMouseWheel(wParam, lParam);			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			return OnLButtonDown(wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			return OnMouseMove(wParam, lParam);			
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			return OnLButtonUp(wParam, lParam);			
		}
		break;	
	case WM_IME_STARTCOMPOSITION:
		//	case WM_IME_NOTIFY:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
	case WM_KEYDOWN:
		{
			BOOL bBreak = FALSE;
			BOOL bOldEditMode = m_pEditItemName->IsEditMode();
			// 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
			if(m_pEditItemName->WndProc(uMsg, wParam, lParam, NULL, TRUE, FALSE))
			{
				// by mspark, 인게임 조합
				m_pEditItemName->SetItemShow(true);
				// by mspark, 인게임 조합
				bBreak = TRUE;				
			}
				// 2013-02-28 by bhsohn [인게임 조합식] 자동 완성 처리
			
			{
				{
					char chItemName[SIZE_MAX_ITEM_NAME];
					chItemName[0] = NULL;					
					m_pEditItemName->GetString(chItemName, SIZE_MAX_ITEM_NAME);
					DBGOUT("uMsg[%d] wParam[%d] lParam[%d] [%s]\n", uMsg, wParam, lParam, chItemName);
				}
				UpdateSearchEditList();
			}
			if(WM_KEYDOWN==uMsg )
			{
				switch(wParam)
				{
				case VK_ESCAPE:
					{
						if(WND_TYPE_MIXITEM == m_WndType)
						{				
							// 현재 윈도우를 숨긴다.
							ShowWnd(FALSE, 0, 0);
							if(g_pGameMain && g_pGameMain->m_pInven)
							{
								g_pGameMain->m_pInven->ShowInven(NULL, NULL, FALSE);	
							}
							return INF_MSGPROC_BREAK;				
						}
					}
					break;
				case VK_RETURN:
					{
						if(bOldEditMode)
						{
							OnClickSearchItem();
							return INF_MSGPROC_BREAK;
						}

					}
					break;
				}
			}
			if(bBreak)
			{
				return INF_MSGPROC_BREAK;
			}						
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}


int CINFItemMixWnd::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}
	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	{
		if(m_pEditItemName->OnMouseWheel(pt, wParam, lParam))
		{
			return INF_MSGPROC_BREAK;
		}		
	}

	{
		BOOL bClick = m_pScroll->IsMouseWhellPos(pt);
		if(bClick)		
		{			
			m_pScroll->OnMouseWheel(wParam, lParam);	
			return INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;	
	
}
int CINFItemMixWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}
	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);

	{
		if(TRUE == m_pCloseBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		if(m_pPageBtn[nCnt])
		{			
			if(TRUE == m_pPageBtn[nCnt]->OnLButtonDown(pt))
			{
				return  INF_MSGPROC_BREAK;
			}
		}		
	}

	GUI_BUILDINGNPC* pTempBaseShop = g_pInterface->m_pCityBase->GetCurrentBuildingNPC(); // 상점이 없을때만 	
	if(IsMouseCaps(pt) && !pTempBaseShop)
	{
		m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
		m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
		m_bMove = TRUE;

		g_pInterface->SetWindowOrder(WNDItemMix);

		return INF_MSGPROC_BREAK;
	}			

	
	nCnt = 0;
	BOOL bClick = FALSE;	

	{
		if(TRUE == m_pSearchBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}

	{
		if(TRUE == m_pEditItemName->OnLButtonDown(pt))
		{				
			m_nSearchType = SEARCH_TYPE_NAME;
//			m_nSearchType = SEARCH_TYPE_ITEMNUM;

			m_pEditItemName->EnableEdit(TRUE, TRUE);

			// by mspark, 인게임 조합
			if(m_pEditItemName->GetSelect() > -1)
			{
				m_nEditElementIndex = m_pEditItemName->GetSelect();
				char * pEditItemStr = m_pEditItemName->GetItem(m_nEditElementIndex);
				m_pEditItemName->SetString(pEditItemStr, SIZE_MAX_ITEM_NAME);
				m_pEditItemName->SetSelect(-1);
				m_pEditItemName->BackupTxtString(); // 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
			}
			// by mspark, 인게임 조합
			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}

	{
		m_pLBMixKind->LButtonDown(pt);
	}

	if(INF_MSGPROC_BREAK == OnCilckPageCnt(pt))
	{
		return INF_MSGPROC_BREAK;
	}

	if(INF_MSGPROC_BREAK == OnCilckSelectItem(pt))
	{
		return INF_MSGPROC_BREAK;
	}

	// 툴팁관련 처리
	if(pt.x > m_ptBkPos.x +m_rcShowItem.left&&
		pt.x < m_ptBkPos.x + m_rcShowItem.right &&
		pt.y > m_ptBkPos.y +m_rcShowItem.top &&
		pt.y < m_ptBkPos.y + m_rcShowItem.bottom &&
		m_uItemUniNum > 0
		&& (SEARCH_TYPE_ITEMNUM == GetSearchType())) // 2013-06-19 by bhsohn 인게임 조합식 수정
	{
		if(g_pGameMain && g_pGameMain->m_pInven)
		{
			INVEN_DISPLAY_INFO* pDisplayItemInfo = g_pGameMain->m_pInven->GetInvenDisplayInfoToUniqueNum(m_uItemUniNum);
			if(pDisplayItemInfo)
			{
				POINT ptIconPos;
				
				ptIconPos.x = pt.x - (m_ptBkPos.x + m_rcShowItem.left);
				ptIconPos.y = pt.y - (m_ptBkPos.y + m_rcShowItem.top);

				g_pGameMain->SetSelectItem(pDisplayItemInfo, ptIconPos, ITEM_MIXWND_POS);
				return INF_MSGPROC_BREAK;
			}
		}		
	}

	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_MIX_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_MIX_WINDOWS_HEIGTH)
	{
		BOOL bClick = m_pScroll->IsMouseBallPos(pt);
		if(bClick)
		{
			m_pScroll->SetMouseMoveMode(TRUE);
			return INF_MSGPROC_BREAK;
		}
		
		return INF_MSGPROC_BREAK;
	}
	
	return INF_MSGPROC_NORMAL;		
}

int CINFItemMixWnd::OnCilckSelectItem(POINT ptPos)
{
	int nStartX = m_ptBkPos.x ;
	int nStartY = m_ptBkPos.y ;
	
	int nCntY = 0;	
	
	for(nCntY = 0;nCntY < m_nMaxShowItemLen;nCntY++)
	{	
		if(((nStartY+m_rcSearchList[nCntY].top) <= ptPos.y) && (ptPos.y <= (nStartY+m_rcSearchList[nCntY].bottom)))
		{
			if(((nStartX+m_rcSearchList[nCntY].left) <= ptPos.x) && (ptPos.x <= (nStartX+m_rcSearchList[nCntY].right)))
			{
				m_nSelItem = nCntY + m_pScroll->GetScrollStep();				
				return INF_MSGPROC_BREAK;	
			}	
		}
		
	}
	return INF_MSGPROC_NORMAL;	
	
}

int CINFItemMixWnd::OnCilckPageCnt(POINT ptPos)
{
	int nStartX = m_ptBkPos.x + PAGE_CNT_POSX;
	int nStartY = m_ptBkPos.y + m_pPageCntBtnPosY;

	int nPageCnt = min(MAX_SEARCH_PAGE, m_nMaxScrollPage-m_nStartPage+1);
	nPageCnt = max(1, nPageCnt);	
	int nCnt = 0;
		
	for(nCnt = 0;nCnt < nPageCnt;nCnt++)
	{		
		int nTmpPosX = nStartX+m_ptPagePosList[nPageCnt-1][nCnt].x;
		int nTmpPosY = nStartY+m_ptPagePosList[nPageCnt-1][nCnt].y;

		if((nTmpPosY <= ptPos.y) && (ptPos.y <= nTmpPosY+PAGE_LIST_HEIGHT))
		{
			if((nTmpPosX <= ptPos.x) && (ptPos.x <= nTmpPosX+PAGE_LIST_WIDTH))
			{
				int nCurrentPage = m_nStartPage+nCnt;
				UpdateItemListScroll(nCurrentPage, m_nStartPage);

				return INF_MSGPROC_BREAK;	
			}	
		}

	}
	return INF_MSGPROC_NORMAL;	

}

int CINFItemMixWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}
	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	if(m_bMove)
	{
		m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
		m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
		// UI유저 지정 
		UpdateBtnPos();
		return INF_MSGPROC_BREAK;
	}
	
	{
		if(TRUE == m_pSearchBtn->OnMouseMove(pt))
		{
			g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP4");
			return INF_MSGPROC_BREAK;
		}
	}
	
	{
		m_pLBMixKind->MouseMove(pt);
	}

	m_pCloseBtn->OnMouseMove(pt);	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{
		if(m_pPageBtn[nCnt])
		{			
			m_pPageBtn[nCnt]->OnMouseMove(pt);				
		}		
	}

	
	if(m_pEditItemName)
	{
		m_pEditItemName->OnMouseMove(pt);
	}

	if(pt.x > m_ptBkPos.x +m_rcShowItem.left&&
		pt.x < m_ptBkPos.x + m_rcShowItem.right &&
		pt.y > m_ptBkPos.y +m_rcShowItem.top &&
		pt.y < m_ptBkPos.y + m_rcShowItem.bottom &&
		m_nShowItemNum > 0
		&& (SEARCH_TYPE_ITEMNUM == GetSearchType()))	
	{
		SetTooltipItemInfo(m_nShowItemNum, pt.x, pt.y, 0);
		return INF_MSGPROC_BREAK;
	}
	
	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_MIX_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_MIX_WINDOWS_HEIGTH)
	{			
		{
			int nFindPosX, nFindPosY;
			nFindPosX = nFindPosY = -1; 
			BOOL bTargetNum = FALSE;
			int nLinkItem = 0;
			
			// 아이템 툴팁
			FindTooltipItemInfo_POS(pt, &bTargetNum, &nFindPosX, &nFindPosY);
			INT nItemNum = GetTooltipItemInfo_ItemNum(m_nCurrentPage, m_pScroll->GetScrollStep(), bTargetNum, nFindPosX, nFindPosY, 
														&nLinkItem);
			
			{
				// 아이템 툴팁
				SetTooltipItemInfo(nItemNum, pt.x, pt.y, nLinkItem);
			}			
		}
		
		if(m_pScroll->GetMouseMoveMode())
		{
			if(FALSE == m_pScroll->IsMouseScrollPos(pt))
			{
				m_pScroll->SetMouseMoveMode(FALSE);
			}
			else
			{
				m_pScroll->SetScrollPos(pt);
				return INF_MSGPROC_BREAK;
			}
		}			
		}
	
	return INF_MSGPROC_NORMAL;	
	
}

int CINFItemMixWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}
	
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	
	
	if(m_bMove)
	{
		m_bMove = FALSE;
		return INF_MSGPROC_BREAK;
	}

	if(TRUE == m_pEditItemName->OnLButtonUp(pt))
	{
		return  INF_MSGPROC_BREAK;
	}

	{
		if(TRUE == m_pSearchBtn->OnLButtonUp(pt))
		{	
			OnClickSearchItem();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{					
			// 현재 윈도우를 숨긴다.
			ShowWnd(FALSE,0,0);
			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			return  INF_MSGPROC_BREAK;
		}
	}
	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_PAGE_BTN; nCnt++)
	{		
		if(TRUE == m_pPageBtn[nCnt]->OnLButtonUp(pt))
		{			
			OnClickPageBtn(nCnt);
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			return  INF_MSGPROC_BREAK;
		}		
	}	

	if(pt.x > m_ptBkPos.x +m_rcShowItem.left&&
		pt.x < m_ptBkPos.x + m_rcShowItem.right &&
		pt.y > m_ptBkPos.y +m_rcShowItem.top &&
		pt.y < m_ptBkPos.y + m_rcShowItem.bottom)
	{
		if(g_pGameMain->m_stSelectItem.pSelectItem 
			&& g_pGameMain->m_stSelectItem.bySelectType == ITEM_INVEN_POS 
			&& g_pGameMain->m_stSelectItem.pSelectItem->pItem)
		{
			UID64_t uItemUniNum = g_pGameMain->m_stSelectItem.pSelectItem->pItem->UniqueNumber;
			CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(uItemUniNum);
			if(pItemInfo)
			{				
				SetShowItemNumShowWnd(TRUE, pItemInfo->ItemNum, uItemUniNum);
			}
			SetTooltipItemInfo(0, 0, 0, 0); // 툴팁 초기화
			g_pGameMain->SetSelectItem(NULL, pt, ITEM_INVEN_POS);
			return INF_MSGPROC_BREAK;
		}

	}

	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_MIX_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_MIX_WINDOWS_HEIGTH)
	{
		BOOL bClick = m_pScroll->GetMouseMoveMode();
		if(bClick)
		{
			m_pScroll->SetMouseMoveMode(FALSE);
			return INF_MSGPROC_BREAK;
		}				
	}
	if(g_pGameMain->m_stSelectItem.pSelectItem 
		&& g_pGameMain->m_stSelectItem.pSelectItem->pItem
		&& ITEM_MIXWND_POS == g_pGameMain->m_stSelectItem.bySelectType)
	{
		m_nShowItemNum = 0;				
		m_uItemUniNum = 0;

		SetTooltipItemInfo(0, 0, 0, 0); // 툴팁 초기화
		g_pGameMain->SetSelectItem(NULL, pt, ITEM_MIXWND_POS);
		return INF_MSGPROC_BREAK;
	}

	return INF_MSGPROC_NORMAL;		
}

void CINFItemMixWnd::OnClickPageBtn(int nPageMode)
{
	switch(nPageMode)
	{
	case PAGE_BTN_FIRST:
		{			
			//첫페이지로
			UpdateItemListScroll(1,1);
		}
		break;
	case PAGE_BTN_PREV:
		{
			int	nStartPage		= m_nStartPage-MAX_SEARCH_PAGE;		// 현재 그림의 시작 페이지
			int	nCurrentPage	= m_nCurrentPage;	// 현재 보고 있는 페이지
			if(nStartPage <= 0)
			{
				// 10페이지 씩이동하는데 10페이지보다 작다
				return;
			}			
			UpdateItemListScroll(nStartPage,nStartPage);
		}
		break;
	case PAGE_BTN_NEXT:
		{
			int	nStartPage		= m_nStartPage+MAX_SEARCH_PAGE;		// 현재 그림의 시작 페이지
			int	nCurrentPage	= m_nCurrentPage;	// 현재 보고 있는 페이지
			if(nStartPage > m_nMaxScrollPage)
			{
				// 10페이지 씩이동하는데 10페이지보다 작다
				return;
			}			
			UpdateItemListScroll(nStartPage,nStartPage);
		}
		break;
	case PAGE_BTN_LAST:
		{
			int	nCap		= max(0, m_nMaxScrollPage-m_nStartPage);		// 현재 그림의 시작 페이지
			if(nCap <= 0)
			{
				return;
			}
			int	nAddPage	= nCap/MAX_SEARCH_PAGE;	// 현재 보고 있는 페이지
			int	nStartPage	= m_nStartPage+(nAddPage*MAX_SEARCH_PAGE);			
			UpdateItemListScroll(m_nMaxScrollPage,nStartPage);
		}
		break;
	default:
		{
			return;
		}
		break;
	}
}

BOOL CINFItemMixWnd::IsMouseCaps(POINT ptPos)
{
	POINT ptBakPos = m_ptBkPos;
	POINT ptSize = {0,0};
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        
	ptSize = m_pImgBackg->GetImgSize();
#endif
	if((ptPos.x >= ptBakPos.x && (ptPos.x <= ptBakPos.x+ptSize.x))
		&& (ptPos.y >= ptBakPos.y && (ptPos.y <= ptBakPos.y+20)))
	{
		return TRUE;
	}
	return FALSE;	
}


void CINFItemMixWnd::UpdateBtnPos()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		

	if(m_pSearchBtn)
	{
		m_pSearchBtn->SetBtnPosition(nWindowPosX+MIX_BTN_POS_X, nWindowPosY+MIX_BTN_POS_Y);
	}

	if(m_pEditItemName)
	{
		int nPosX = nWindowPosX + ITEM_MIX_EDIT_X;
		int nPosY = nWindowPosY + ITEM_MIX_EDIT_Y;
		m_pEditItemName->SetPos(nPosX, nPosY);
		m_pEditItemName->SetElementArea(nPosX, nPosY+ITEM_MIX_EDIT_ELEMENT_Y, ITEM_MIX_EDIT_W, ITEM_MIX_EDIT_H);
		m_pEditItemName->SetBGPos(nPosX, nPosY+ITEM_MIX_EDIT_ELEMENT_Y, ITEM_MIX_EDIT_W, ITEM_MIX_EDIT_H);
	}

	{		
		int nPosX = nWindowPosX + OPTION_BTN_CLOSE_X;
		int nPosY = nWindowPosY + OPTION_BTN_CLOSE_Y;	
		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
	
	{	
		int nPageCnt = min(MAX_SEARCH_PAGE, m_nMaxScrollPage-m_nStartPage+1);
		nPageCnt = max(1, nPageCnt);	

		int nStartX = nWindowPosX + PAGE_CNT_POSX;
		int nStartY = nWindowPosY + m_pPageCntBtnPosY;
		
		int nTmpPosX = nStartX+m_ptPagePosList[nPageCnt-1][0].x;
		int nTmpPosY = nStartY+m_ptPagePosList[nPageCnt-1][0].y;

		m_pPageBtn[PAGE_BTN_FIRST]->SetBtnPosition(nTmpPosX-32, nTmpPosY+2);				
		m_pPageBtn[PAGE_BTN_PREV]->SetBtnPosition(nTmpPosX-18, nTmpPosY+2);				

		nTmpPosX = nStartX+m_ptPagePosList[nPageCnt-1][nPageCnt-1].x;
		nTmpPosY = nStartY+m_ptPagePosList[nPageCnt-1][nPageCnt-1].y;

// 		m_pPageBtn[PAGE_BTN_NEXT]->SetBtnPosition(nTmpPosX+16, nTmpPosY+2);				
// 		m_pPageBtn[PAGE_BTN_LAST]->SetBtnPosition(nTmpPosX+26, nTmpPosY+2);				
		int nTmpNextPosX = nTmpPosX+16;
		if(m_pConuntFont)
		{
			char chTxt[64] ={0,}; 
			wsprintf(chTxt, "%d", m_nMaxScrollPage);
			SIZE szSize = m_pConuntFont->GetStringSize(chTxt);
			nTmpNextPosX = nTmpPosX + szSize.cx + 8;
		}
		m_pPageBtn[PAGE_BTN_NEXT]->SetBtnPosition(nTmpNextPosX, nTmpPosY+2);				
 		m_pPageBtn[PAGE_BTN_LAST]->SetBtnPosition(nTmpNextPosX+10, nTmpPosY+2);				

	}
}

// 검색버튼 처리
void CINFItemMixWnd::OnClickSearchItem()
{
	if(!m_pLBMixKind)
	{
		return;
	}
	m_pEditItemName->EnableEdit(FALSE, FALSE);

	m_pEditItemName->ItemClear();	// Item 클리어 
	m_vecRenderItemList.clear();	// 랜더링 할 벡터

	int nItemCnt = 0;

	switch(m_pLBMixKind->GetSelect())
	{
	case LIST_BOX_SEARCH_ALLITEM:	// 전체
		{
			nItemCnt = OnClickResultNameSearch(nItemCnt);
			nItemCnt= OnClickMaterialNameSearch(nItemCnt);

		}
		break;
	case LIST_BOX_SEARCH_RESULT:	// 결과
		{
			nItemCnt = OnClickResultNameSearch(nItemCnt);
		}
		break;
	case LIST_BOX_SEARCH_MATERIAL:	// 재료
		{			
			nItemCnt = OnClickMaterialNameSearch(nItemCnt);
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	// 정보 초기화
	InitItemListScroll();
	
}

// [결과]로 검색
int CINFItemMixWnd::OnClickResultNameSearch(int nItemCnt)
{
	char chItemName[SIZE_MAX_ITEM_NAME];
	chItemName[0] = NULL;

	m_vecMixingItemList.clear();		// 검색결과

	
	switch(GetSearchType())
	{
	case SEARCH_TYPE_ITEMNUM:
		{	
			if(m_nShowItemNum <= 0)
			{
				return nItemCnt;
			}
// 			m_pEditItemName->GetString(chItemName, SIZE_MAX_ITEM_NAME);				
// 			g_pDatabase->GetResultMixingItemListIndex(atoi(chItemName), &m_vecMixingItemList);
			nItemCnt = g_pDatabase->GetResultMixingItemListIndex(nItemCnt, m_nShowItemNum, &m_vecMixingItemList);			
		}
		break;
	case SEARCH_TYPE_NAME:
		{
			m_pEditItemName->GetString(chItemName, SIZE_MAX_ITEM_NAME);				
			if(0 == strlen(chItemName))
			{
				return nItemCnt;
			}
			nItemCnt = g_pDatabase->GetResultMixingItemList(nItemCnt, chItemName, &m_vecMixingItemList);

		}
		break;		
	default:
		{
			return nItemCnt;
		}
		break;	
	}	
	vector<structResultMixingInfo>::iterator it = m_vecMixingItemList.begin();
	while(it != m_vecMixingItemList.end())
	{
		structResultMixingInfo stTmp = (*it);
		
		// 아이템 재정렬
		structResultMixingInfo stDest;		
		memcpy(&stDest, &stTmp, sizeof(structResultMixingInfo));
		{		
			// 재료 정보 재정렬
			memset(stDest.struMixingInfo.SourceItem, 0x00, sizeof(ITEM_W_COUNT)* COUNT_ITEM_MIXING_SOURCE);
			
			int nCnt = 0;
			int nMemPos = (COUNT_ITEM_MIXING_SOURCE-1);
			for(nCnt = (COUNT_ITEM_MIXING_SOURCE-1); nCnt >= 0 ;nCnt--)
			{
				if(0 != stTmp.struMixingInfo.SourceItem[nCnt].ItemNum)
				{
					stDest.struMixingInfo.SourceItem[nMemPos] = stTmp.struMixingInfo.SourceItem[nCnt];
					nMemPos--;
					nMemPos = max(nMemPos, 0);
				}		
			}	
		}	

		m_vecRenderItemList.push_back(stDest);
		it++;
	}
	return nItemCnt;
}

// [재료]로 검색
int CINFItemMixWnd::OnClickMaterialNameSearch(int nItemCnt)
{
	char chItemName[SIZE_MAX_ITEM_NAME];
	chItemName[0] = NULL;
	
	m_vecMaterialList.clear();

	switch(GetSearchType())
	{
	case SEARCH_TYPE_ITEMNUM:
		{			
			if(m_nShowItemNum<=0)
			{
				return nItemCnt;
			}
// 			m_pEditItemName->GetString(chItemName, SIZE_MAX_ITEM_NAME);				
// 			g_pDatabase->GetMaterialMixingItemListIndex(atoi(chItemName), &m_vecMaterialList);
			nItemCnt = g_pDatabase->GetMaterialMixingItemListIndex(nItemCnt, m_nShowItemNum, &m_vecMaterialList);
		}
		break;
	case SEARCH_TYPE_NAME:
		{
			m_pEditItemName->GetString(chItemName, SIZE_MAX_ITEM_NAME);

			if(0 == strlen(chItemName))
			{
				return nItemCnt;
			}
			nItemCnt = g_pDatabase->GetMaterialMixingItemList(nItemCnt, chItemName, &m_vecMaterialList);	// 전체 재료 리스트를 가져온다.
		}
		break;
	default:
		{
			return nItemCnt;
		}
		break;	
	}	
	{
		//////////// 검색 아이템 리스트 ////////////		
		int nCnt = 0;
		char chIconName[32] = {0,};
		
		int nStartX = m_ptBkPos.x;
		int nStartY = m_ptBkPos.y;
		
		vector<structResultMixingInfo>::iterator it = m_vecMaterialList.begin();
		
		int nMaterialListCnt = 0;
		
		int nMaterialCnt = 0;		
		
		while(it != m_vecMaterialList.end())
		{							
			structResultMixingInfo struRMixingInfo = (*it);
			ITEM_MIXING_INFO struMixingInfo = struRMixingInfo.struMixingInfo;				
			ITEM* pTargetItem = g_pDatabase->GetItemInfoLoadItemData(struMixingInfo.TargetItemNum);
			if(!pTargetItem)
			{					
				it++;
				continue;
			}
			// 아이템 재정렬
			structResultMixingInfo stDest;
			memcpy(&stDest, &struRMixingInfo, sizeof(structResultMixingInfo));
			{				
				// 재료 정보 재정렬
				memset(stDest.struMixingInfo.SourceItem, 0x00, sizeof(ITEM_W_COUNT)* COUNT_ITEM_MIXING_SOURCE);
				
				int nCnt = 0;
				int nMemPos = (COUNT_ITEM_MIXING_SOURCE-1);
				for(nCnt = (COUNT_ITEM_MIXING_SOURCE-1); nCnt >= 0 ;nCnt--)
				{
					if(0 != struRMixingInfo.struMixingInfo.SourceItem[nCnt].ItemNum)
					{
						stDest.struMixingInfo.SourceItem[nMemPos] = struRMixingInfo.struMixingInfo.SourceItem[nCnt];
						nMemPos--;
						nMemPos = max(nMemPos, 0);
					}		
				}	
			}
			m_vecRenderItemList.push_back(stDest);				

			nMaterialCnt++;
			
			it++;			
		}	
	}
	return nItemCnt;
}

// Scroll 갱신
void CINFItemMixWnd::InitItemListScroll()
{
	m_nCurrentPage	= 1;	
	m_nStartPage	= 1;

	m_nMaxScrollPage = 1;
	m_nSelItem = -1;
	int nItemCnt = m_vecRenderItemList.size();	// 랜더링 할 벡터
	int nPageCnt = nItemCnt/MAX_SCROLL_CNT;
	if(0 == nPageCnt)
	{
		m_pScroll->SetMaxItem(max(1, nItemCnt));
		UpdateBtnPos();
		return;
	}
	int nScrollItemCnt = min(MAX_SCROLL_CNT, nItemCnt);
	m_pScroll->SetMaxItem(nScrollItemCnt); // 최대 MAX_SCROLL_CNT 갯수만 넣자
	m_nMaxScrollPage = nPageCnt+1;

	UpdateBtnPos();
}

void CINFItemMixWnd::UpdateItemListScroll(int nCurrentPage, int nStartPage)
{
	m_nCurrentPage	= nCurrentPage;	
	m_nStartPage	= nStartPage;	
	
	int nItemCnt = m_vecRenderItemList.size();	// 랜더링 할 벡터
	int nPageCnt = nItemCnt/MAX_SCROLL_CNT;
	if(0 == nPageCnt)
	{
		m_pScroll->SetMaxItem(max(1, nItemCnt));
		return;
	}
	int nScrollItemCnt = min(MAX_SCROLL_CNT, nItemCnt-(MAX_SCROLL_CNT*(nCurrentPage-1)));
	m_pScroll->SetMaxItem(nScrollItemCnt); // 최대 MAX_SCROLL_CNT 갯수만 넣자	
	
	UpdateBtnPos();
}

// [자동완성] 리스트를 갱신해준다.
void CINFItemMixWnd::UpdateSearchEditList()
{
	// by mspark, 인게임 조합
	m_pEditItemName->ItemClear();
	// by mspark, 인게임 조합

	if(FALSE == m_pEditItemName->IsEditMode())
	{
		return;
	}	

	// 글씨가 완성되면 그 리스트로 검색하자
	char chItemName[SIZE_MAX_ITEM_NAME];
	chItemName[0] = NULL;					
	m_pEditItemName->GetString(chItemName, SIZE_MAX_ITEM_NAME);

	char* pEditItemName = m_pEditItemName->GetAutoEditBoxString();
	if(0 == strcmp(pEditItemName, chItemName))
	{
		return;
	}
	m_pEditItemName->SetAutoEditBoxString(chItemName);

	if(strlen(chItemName) <= 0)
	{
		return;
	}
	
	std::map<INT, std::string> vecItemNameList;
	g_pDatabase->GetSearchItemList(chItemName, &vecItemNameList);

	std::map<INT, std::string>::iterator it = vecItemNameList.begin();
	int nCnt = 0;
	while(it != vecItemNameList.end())
	{
		std::string strItemList = it->second;
		//DBGOUT("[%d] UpdateSearchEditList[%s] \n", nCnt++, strItemList.c_str());
		// by mspark, 인게임 조합
		m_pEditItemName->AddElement((char*)strItemList.c_str());
		// by mspark, 인게임 조합
		it++;
	}
	m_pEditItemName->ItemAddDone(); // 추가 완료
}

// 아이템 툴팁
void CINFItemMixWnd::SetTooltipItemInfo(INT	nItemNum, int x, int y, int nLinkItem)
{	
	CINFItemInfo* pToolTipItemInfo = g_pGameMain->GetINFItemInfo();
	if(!pToolTipItemInfo)
	{
		g_pGameMain->SetItemInfo(0,0,0,0);
		return;
	}
	const ITEM	*pRefITEM = pToolTipItemInfo->GetRefITEM();	
	
	if(pToolTipItemInfo 
		&& (0 != nItemNum)
		&& pRefITEM
		&& (nItemNum == pRefITEM->ItemNum))
	{
		// 아이템이 같으면 툴팁을 갱신해줄 필요가 없다.
		return;
	}

	if(nItemNum != 0)
	{
		// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
//		g_pGameMain->SetItemInfo(0, nItemNum, x, y);
		g_pGameMain->SetItemInfo(0, nItemNum, x, y, 0, TRUE, nLinkItem);
	}
	else
	{
		g_pGameMain->SetItemInfo(0,0,0,0);
	}		
}

// 아이템 툴팁
void CINFItemMixWnd::FindTooltipItemInfo_POS(POINT pt, BOOL* pTarget, int *nFindPosX, int *nFindPosY)
{
	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;

	(*pTarget) = FALSE;
	(*nFindPosX) = (*nFindPosY) = -1;

	int nCntX, nCntY;
	nCntX = nCntY = 0;
	for(nCntY=0;nCntY< m_nMaxShowItemLen;nCntY++)
	{
		for(nCntX=0;nCntX< MAX_INGAME_SEARCH_X;nCntX++)
		{
			if(((nStartY+m_rcSearchItem[nCntY][nCntX].top) <= pt.y) && (pt.y <= (nStartY+m_rcSearchItem[nCntY][nCntX].bottom)))
			{
				if(((nStartX+m_rcSearchItem[nCntY][nCntX].left) <= pt.x) && (pt.x <= (nStartX+m_rcSearchItem[nCntY][nCntX].right)))
				{
					(*pTarget) = FALSE;
					(*nFindPosX) = nCntX;
					(*nFindPosY) = nCntY;					
					return;
				}
			}			
		}
	}	

	for(nCntY=0;nCntY< m_nMaxShowItemLen;nCntY++)
	{
		if(((nStartY+m_rcTargetItem[nCntY].top) <= pt.y) && (pt.y <= (nStartY+m_rcTargetItem[nCntY].bottom)))
		{
			if(((nStartX+m_rcTargetItem[nCntY].left) <= pt.x) && (pt.x <= (nStartX+m_rcTargetItem[nCntY].right)))
			{
				(*pTarget) = TRUE;
				(*nFindPosX) = 0;
				(*nFindPosY) = nCntY;				
				return;
			}
		}			
	}	
}

INT CINFItemMixWnd::GetTooltipItemInfo_ItemNum(int nPage, int nScroll, BOOL bTarget, int nFindPosX, int nFindPosY, int* pLinkItem)
{
	if(nFindPosX < 0 || nFindPosY < 0 )
	{
		return 0;
	}
	else if(nFindPosX >= MAX_INGAME_SEARCH_X  || nFindPosY >= m_nMaxShowItemLen)
	{
		return 0;
	}
	
	int nFindItemNum = 0;
	int nMaxLine = m_nMaxShowItemLen;
	
	vector<structResultMixingInfo>::iterator it = m_vecRenderItemList.begin();
//	int nStartPos = (nPage*nMaxLine) + nScroll;
	int nStartPos = nScroll;
	if(nPage <= m_nMaxScrollPage )
	{
		nStartPos = (max(nPage-1, 0)*MAX_SCROLL_CNT)+nScroll;
	}		
	int nPos =0;
	
	{
		// 스크롤과 페이지에 관한 찾기
		while(it != m_vecRenderItemList.end())
		{			
			if(nPos >= nStartPos)
			{
				break;
			}
			nPos++;
			it++;
		}	
	}

	{	
		// 현재 페이지에 대한 검색		
		int nLine = 0;
		int nCnt = 0;
		nPos =0;
		
		while(it != m_vecRenderItemList.end())
		{
			ITEM_MIXING_INFO stTmp = (*it).struMixingInfo;
			if(nLine == nFindPosY)
			{			
				if(!bTarget && nFindPosX < COUNT_ITEM_MIXING_SOURCE)
				{
					int nIndexX = (COUNT_ITEM_MIXING_SOURCE-1) - nFindPosX;
					nIndexX = max(nIndexX, 0);
					nIndexX = min(nIndexX, (COUNT_ITEM_MIXING_SOURCE-1));

					for(nCnt = 0;nCnt < COUNT_ITEM_MIXING_SOURCE; nCnt++)
					{
						nFindItemNum = stTmp.SourceItem[nIndexX].ItemNum;
						if(0 != nFindItemNum)
						{
							// ItemNum이 0이면 다음을 가져온다.
							break;
						}
						else
						{
							nIndexX--;
						}
					}
				}
				else
				{
					nFindItemNum = stTmp.TargetItemNum;
					if((*it).LinkItem > 0)
					{
						(*pLinkItem) = (*it).LinkItem;
					}					
				}								
				return nFindItemNum;
			}
			nLine++;						
			it++;
		}
	}	
	
	return nFindItemNum;
}


POINT CINFItemMixWnd::GetBkSize()
{
	return m_pImgBackg->GetImgSize();
}

DataHeader* CINFItemMixWnd::FindResource(char* szRcName)
{
	return CINFBase::FindResource(szRcName);
}


INT CINFItemMixWnd::GetCntInven_ItemNum_To_AllItemCnt( INT nItemNum )
{
	CItemInfo* pInvenItem = g_pStoreData->FindItemInInventoryByItemNum( nItemNum);
	if(!pInvenItem)
	{
		return 0;
	}
	INT nItemNumCnt = 0;
	switch(pInvenItem->Kind)
	{
	case ITEMKIND_ACCESSORY_TIMELIMIT :
		{
			vector<CItemInfo*> vecItemList;	
			vecItemList.clear();
			
			// 같은 ItemNum을 갖는 아이템들을 다 가져오자
			g_pStoreData->FindItemInInventoryByItemNumList(nItemNum, &vecItemList);
			if(vecItemList.empty())
			{
				return 0;
			}
			vector<CItemInfo*>::iterator it = vecItemList.begin();
			while(it != vecItemList.end())
			{
				pInvenItem = (*it);
				if(ITEMKIND_ACCESSORY_TIMELIMIT == pInvenItem->Kind && (pInvenItem->GetItemPassTime() > 0))
				{
					// 기간제 악세사리는 사용한적이 있으면 카운트 하지 않는다.
				}
				else
				{
					if( IS_COUNTABLE_ITEM(pInvenItem->Kind) )
					{
						nItemNumCnt += pInvenItem->CurrentCount;				
					}
					else
					{
						nItemNumCnt += 1;							
					}
				}
				it++;
			}			
		}
		break;
	default:
		{
			nItemNumCnt = g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt(nItemNum);
		}
		break;
	}
	return nItemNumCnt;	
}