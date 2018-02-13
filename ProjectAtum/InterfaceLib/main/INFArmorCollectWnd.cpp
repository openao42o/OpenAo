// INFItemMixWnd.cpp: implementation of the CINFArmorCollectWnd class.
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

#include "INFArmorCollectWnd.h"

#include "INFEditBox.h"
#include "INFAutoEditBox.h"
#include "INFImageBtn.h"
#include "INFComboBoxEX.h"
#include "INFListBox.h"
#include "INFArenaScrollBar.h"
#include "INFIcon.h"
#include "INFInvenExtend.h"

#include "INFItemInfo.h"

#define INF_ARMOR_COLLECT_WINDOWS_WIDTH			762														// 전체 넒이
#define INF_ARMOR_COLLECT_WINDOWS_HEIGHT			645														// 전체 높이

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

// 스크롤바
#define INF_MIX_SCROLL_COUNT				(m_nMaxShowItemLen)
#define INF_MIX_SCROLL_X					(m_ptBkPos.x + 724)
#define INF_MIX_SCROLL_Y					(m_ptBkPos.y + 94)
#define INF_MIX_SCROLL_W					13
#define INF_MIX_SCROLL_H					300
#define INF_MIX_SCROLL_WHELL_L				(m_ptBkPos.x + 35)
#define INF_MIX_SCROLL_WHELL_T				(m_ptBkPos.y + 84)
#define INF_MIX_SCROLL_WHELL_R				(m_ptBkPos.x + 760)
#define INF_MIX_SCROLL_WHELL_B				(m_ptBkPos.y + 450)

#define INF_MIX_SCROLL_BALL_L				(m_ptBkPos.x + 628)		
#define INF_MIX_SCROLL_BALL_T				(m_ptBkPos.y + 74)	
#define	INF_MIX_SCROLL_BALL_R				(m_ptBkPos.x + 828)
#define INF_MIX_SCROLL_BALL_B				(m_ptBkPos.y + 412)	




// 선택 메뉴 커서
#define LIST_BOX_SEARCH_ALLITEM					0
#define LIST_BOX_SEARCH_RESULT					1
#define LIST_BOX_SEARCH_MATERIAL				2

//ShowItem 항목
#define		INIT_ARMOR_POS_X				96
#define		INIT_ARMOR_POS_Y				503

//ShowItem 항목
#define		SHOW_ITEML_POS_X				337
#define		SHOW_ITEML_POS_Y				503

//APPLAT 항목
#define		APPLAY_ARMOR_POS_X				578
#define		APPLAY_ARMOR_POS_Y				503

// Search ItemList 시작 지점
#define		SEARCH_ITEMLIST_POS_X				76
#define		SEARCH_ITEMLIST_POS_Y				96


// Close버튼
#define		OPTION_BTN_CLOSE_X				741
#define		OPTION_BTN_CLOSE_Y				7

// 아머 해제									   
// 2013-07-11 by bhsohn 아머 컬렉션 수정안
// #define		OPTION_BTN_INIT_X				227
// #define		OPTION_BTN_INIT_Y				527
#define		OPTION_BTN_INIT_X				54
#define		OPTION_BTN_INIT_Y				490

// 아머 적용
// 2013-07-11 by bhsohn 아머 컬렉션 수정안
// #define		OPTION_BTN_ARMOR_X				469
// #define		OPTION_BTN_ARMOR_Y				527
#define		OPTION_BTN_ARMOR_X				468
#define		OPTION_BTN_ARMOR_Y				490




#define		PAGE_LIST_WIDTH					30
#define		PAGE_LIST_HEIGHT				30

// Page표시
#define		PAGE_CNT_POSX					125
#define		PAGE_CNT_POSY					433	//437

// 선택 메뉴 커서
#define SEARCH_TYPE_ITEMNUM						0		// ItemNum으로 검색
#define SEARCH_TYPE_NAME						1		// ItemName으로 검색

#define	TEXT_CAP						4

#define			APPLAY_BLING_TIME			1.0f


extern CAtumDatabase			*g_pDatabase;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFArmorCollectWnd::CINFArmorCollectWnd(CAtumNode* pParent)
{
	CINFArmorCollectWnd::InitWndData();
	
}

void CINFArmorCollectWnd::InitWndData()
{
	m_pImgBackg = NULL;	
	m_bShowWnd = FALSE;	
	m_pScroll = NULL;

	m_pConuntFont = NULL;
	m_pItemListFont = NULL;

	wsprintf(m_strMixKind[0], STRMSG_C_130222_0001);
	wsprintf(m_strMixKind[1], STRMSG_C_130222_0002);
	wsprintf(m_strMixKind[2], STRMSG_C_130222_0003);

	m_ptBkPos.x = m_ptBkPos.y = 0;
	m_bMove = FALSE;	
	m_ptCommOpMouse.x = m_ptCommOpMouse.y = 0;
	
	m_nSearchItemNum = 0;

	m_nEditElementIndex = -1;

	m_vecMixingItemList.clear();		// 검색결과		

	m_pSearchListBk = NULL;
	m_pSearchListSel = NULL;
	m_pApplyItem = NULL;
	m_pUseSel = NULL;
	m_pSearchIconBk = NULL;
	
	m_pCloseBtn = NULL;

	m_pArmorApplyBtn = NULL;
	m_pArmorInitBtn = NULL;

	{
		m_rcShowItem.left		= SHOW_ITEML_POS_X;
		m_rcShowItem.top		= SHOW_ITEML_POS_Y;
		m_rcShowItem.right		= m_rcShowItem.left + BIGICON_WIDTH;
		m_rcShowItem.bottom		= m_rcShowItem.top + BIGICON_HEIGHT;	
	}

	{
		m_rcInitItem.left		= INIT_ARMOR_POS_X;
		m_rcInitItem.top		= INIT_ARMOR_POS_Y;
		m_rcInitItem.right		= m_rcInitItem.left + BIGICON_WIDTH;
		m_rcInitItem.bottom		= m_rcInitItem.top + BIGICON_HEIGHT;	
	}

	
	{
		m_rcApplayItem.left		= APPLAY_ARMOR_POS_X;
		m_rcApplayItem.top		= APPLAY_ARMOR_POS_Y;
		m_rcApplayItem.right		= m_rcApplayItem.left + BIGICON_WIDTH;
		m_rcApplayItem.bottom		= m_rcApplayItem.top + BIGICON_HEIGHT;	
	}
	
	{
		int nCntX, nCntY;
		nCntX = nCntY = 0;
		for(nCntY=0;nCntY< MAX_ARMOR_Y;nCntY++)
		{
			for(nCntX=0;nCntX< MAX_ARMOR_X;nCntX++)
			{
				m_rcArmorItem[nCntY][nCntX].left	= SEARCH_ITEMLIST_POS_X + (BIGICON_WIDTH*nCntX) + (ICON_CAPX*nCntX);
				m_rcArmorItem[nCntY][nCntX].top		= SEARCH_ITEMLIST_POS_Y + (BIGICON_HEIGHT*nCntY) + (ICON_CAPY*nCntY);
				m_rcArmorItem[nCntY][nCntX].right	= m_rcArmorItem[nCntY][nCntX].left + BIGICON_WIDTH;
				m_rcArmorItem[nCntY][nCntX].bottom	= m_rcArmorItem[nCntY][nCntX].top	+ BIGICON_HEIGHT;
			}
		}	
	}
	
	m_nShowItemNum = 0;
	m_uItemUniNum = 0;
	m_nDiffScrollHeight = 0;
	
	m_nMaxShowItemLen = MAX_ARMOR_Y;	// 최대 라인수
	m_pPageCntBtnPosY = PAGE_CNT_POSY;	

	memset(m_nBkImageGroup, 0x00, MAX_PATH);
	sprintf(m_nBkImageGroup, "collection");

	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		m_mapBigIconInfo[nUnitCollectType].clear();
		m_pGearTabImg[nUnitCollectType] = NULL;		
	}

	m_pLackItemImage = NULL;
	m_nSelUnitCollectType = COLLECT_UNIT_BGEAR;
	
	m_ptSelectPos.x = m_ptSelectPos.y = -1;

	{
		m_rcGearTab[COLLECT_UNIT_BGEAR].left = 114;
		m_rcGearTab[COLLECT_UNIT_BGEAR].top = 38;
		
		m_rcGearTab[COLLECT_UNIT_BGEAR].right = m_rcGearTab[COLLECT_UNIT_BGEAR].left + 96;
		m_rcGearTab[COLLECT_UNIT_BGEAR].bottom = m_rcGearTab[COLLECT_UNIT_BGEAR].top + 25;
	}
	{
		m_rcGearTab[COLLECT_UNIT_IGEAR].left = m_rcGearTab[COLLECT_UNIT_BGEAR].right+50;
		m_rcGearTab[COLLECT_UNIT_IGEAR].top = 38;
		
		m_rcGearTab[COLLECT_UNIT_IGEAR].right = m_rcGearTab[COLLECT_UNIT_IGEAR].left + 96;
		m_rcGearTab[COLLECT_UNIT_IGEAR].bottom = m_rcGearTab[COLLECT_UNIT_IGEAR].top + 25;		
	}
	{
		m_rcGearTab[COLLECT_UNIT_MGEAR].left = m_rcGearTab[COLLECT_UNIT_IGEAR].right+50;
		m_rcGearTab[COLLECT_UNIT_MGEAR].top = 38;
		
		m_rcGearTab[COLLECT_UNIT_MGEAR].right = m_rcGearTab[COLLECT_UNIT_MGEAR].left + 96;
		m_rcGearTab[COLLECT_UNIT_MGEAR].bottom = m_rcGearTab[COLLECT_UNIT_MGEAR].top + 25;
		
	}
	{
		m_rcGearTab[COLLECT_UNIT_AGEAR].left = m_rcGearTab[COLLECT_UNIT_MGEAR].right+50;
		m_rcGearTab[COLLECT_UNIT_AGEAR].top = 38;
		
		m_rcGearTab[COLLECT_UNIT_AGEAR].right = m_rcGearTab[COLLECT_UNIT_AGEAR].left + 96;
		m_rcGearTab[COLLECT_UNIT_AGEAR].bottom = m_rcGearTab[COLLECT_UNIT_AGEAR].top + 25;
		
	}	
}


CINFArmorCollectWnd::~CINFArmorCollectWnd()
{
	util::del(m_pImgBackg);	
	util::del(m_pScroll);

	util::del(m_pSearchListBk);
	util::del(m_pApplyItem);
	util::del(m_pSearchListSel);
	util::del(m_pUseSel);
	util::del(m_pSearchIconBk);
	
	util::del(m_pConuntFont);
	util::del(m_pItemListFont);

	util::del(m_pCloseBtn);	
	util::del(m_pArmorApplyBtn);
	util::del(m_pArmorInitBtn);
	util::del(m_pLackItemImage);
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		util::del(m_pGearTabImg[nUnitCollectType]);
	}
}

HRESULT CINFArmorCollectWnd::InitDeviceObjects()
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


	if(NULL == m_pScroll)
	{
		m_pScroll = new CINFArenaScrollBar;
		m_pScroll->InitDeviceObjects(INF_MIX_SCROLL_COUNT,"c_scrlb");
		m_pScroll->InitDeviceObjects_Border("scr01", "scr02", "scr03"); 
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
		// 적용된 아머
		char buf[16];
		memset(buf, 0x00, 16);		
		wsprintf(buf,"coll_use");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pApplyItem))
		{
			m_pApplyItem = new CINFImageEx;
			m_pApplyItem->InitDeviceObjects(pDataHeader);
		}
		
	}
	
	{
		char buf[16];
		memset(buf, 0x00, 16);		
		wsprintf(buf,"aremapsel");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pSearchListSel))
		{
			m_pSearchListSel = new CINFImageEx;
			m_pSearchListSel->InitDeviceObjects(pDataHeader);
		}
		
	}

	{
		char buf[16];
		memset(buf, 0x00, 16);		
		wsprintf(buf,"coll_use");
		
		pDataHeader = FindResource(buf);		
		if(pDataHeader && (NULL == m_pUseSel))
		{
			m_pUseSel = new CINFImageEx;
			m_pUseSel->InitDeviceObjects(pDataHeader);
		}
		
	}

	if(NULL == m_pConuntFont)
	{
		m_pConuntFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,512,32);
		m_pConuntFont->InitDeviceObjects(g_pD3dDev) ;
	}	
	if(NULL == m_pItemListFont)
	{
//		m_pItemListFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),8, D3DFONT_ZENABLE, TRUE,512,32);
		m_pItemListFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,512,32);		
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

	// 적용
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		
		wsprintf(szUpBtn, "coll_chan3");
		wsprintf(szDownBtn, "coll_chan1");
		wsprintf(szSelBtn, "coll_chan0");
		wsprintf(szDisBtn, "coll_chan2");
		if(NULL == m_pArmorApplyBtn)
		{
			m_pArmorApplyBtn = new CINFImageBtn;			
			m_pArmorApplyBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP17");
		}
		
	}

	// 해제
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];

		wsprintf(szUpBtn, "coll_can3");
		wsprintf(szDownBtn, "coll_can1");
		wsprintf(szSelBtn, "coll_can0");
		wsprintf(szDisBtn, "coll_can2");

		if(NULL == m_pArmorInitBtn)
		{
			m_pArmorInitBtn = new CINFImageBtn;			
			m_pArmorInitBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn,"STRTOOLTIP14");
		}
		
	}
	
	{		
		char buf[32];
		wsprintf(buf,"dark75");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(pDataHeader && NULL == m_pLackItemImage)
		{
			m_pLackItemImage = new CINFImageEx;
			m_pLackItemImage->InitDeviceObjects( pDataHeader );
		}				
	}
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{		
		char buf[32];
		wsprintf(buf,"coll_tab%d", nUnitCollectType);
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(pDataHeader && NULL == m_pGearTabImg[nUnitCollectType])
		{
			m_pGearTabImg[nUnitCollectType] = new CINFImageEx;
			m_pGearTabImg[nUnitCollectType]->InitDeviceObjects( pDataHeader );
		}				
	}

	return S_OK ;
}
HRESULT CINFArmorCollectWnd::RestoreDeviceObjects()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		

	if(m_pImgBackg)
	{
		m_pImgBackg->RestoreDeviceObjects( );		
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
	if(m_pApplyItem)
	{
		m_pApplyItem->RestoreDeviceObjects();
	}
	if(m_pSearchListSel)
	{
		m_pSearchListSel->RestoreDeviceObjects();
	}
	if(m_pUseSel)
	{
		m_pUseSel->RestoreDeviceObjects();
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

	if(m_pArmorApplyBtn)
	{
		m_pArmorApplyBtn->RestoreDeviceObjects();				
		// 2013-06-25 by bhsohn 아머 컬렉션 수정안 처리
		m_pArmorApplyBtn->SetLoopPlay(TRUE);
		m_pArmorApplyBtn->SetBtnBlingBling(TRUE, APPLAY_BLING_TIME); 
		// END 2013-06-25 by bhsohn 아머 컬렉션 수정안 처리
	}	
	
	if(m_pArmorInitBtn)
	{
		m_pArmorInitBtn->RestoreDeviceObjects();		
		// 2013-06-25 by bhsohn 아머 컬렉션 수정안 처리
		m_pArmorInitBtn->SetLoopPlay(TRUE);
		m_pArmorInitBtn->SetBtnBlingBling(TRUE, APPLAY_BLING_TIME); 
		// END 2013-06-25 by bhsohn 아머 컬렉션 수정안 처리
	}	
	if(m_pLackItemImage)
	{
		m_pLackItemImage->RestoreDeviceObjects();
	}

	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		if(m_pGearTabImg[nUnitCollectType])
		{
			m_pGearTabImg[nUnitCollectType]->RestoreDeviceObjects();
		}

	}


	ResotreAllArmorIcon();		// 아머 아이콘 전체 갱신

	// RestoreDeviceObjects해주고 버튼, UI위치 수정 해주자
	UpdateBtnPos();
	return S_OK ;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		전체 아머 BigIcon로딩 반드시 Restore에 있어야 한다. FindBigIcon에서 Restore를 해준다.
/// \author		
/// \date		2013-05-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFArmorCollectWnd::ResotreAllArmorIcon()
{
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);
		CVecArmorCollecInfoIterator itArmor = pvecArmorCollect->begin();
		while(itArmor != pvecArmorCollect->end())
		{
			struArmorCollectionInfo* pArmorInfo = (*itArmor);
			ITEM* pItem = g_pDatabase->GetServerItemInfo( pArmorInfo->struCollectItemInfo.ShapeNum );
			if(!pItem)
			{
				itArmor++;
				continue;
			}		
			
			//CINFImageEx* pBigIconImg = g_pGameMain->FindBigIcon(pArmorInfo->struCollectItemInfo.ShapeNum); // 이곳에서 Init, Resotre이미지 관련 처리 다한다.
			CINFImageEx* pBigIconImg = g_pGameMain->FindBigIcon(pItem->SourceIndex); // 이곳에서 Init, Resotre이미지 관련 처리 다한다.
			// 2013-06-18 by ssjung bigicon item의 소스 인덱스로 수정 
			if(!pBigIconImg)
			{
				itArmor++;
				continue;
			}
			map<INT, CINFImageEx*>::iterator it_map = m_mapBigIconInfo[nUnitCollectType].find(pArmorInfo->struCollectItemInfo.ShapeNum);
			if(it_map == m_mapBigIconInfo[nUnitCollectType].end())
			{
				m_mapBigIconInfo[nUnitCollectType].insert(pair<INT, CINFImageEx*>(pArmorInfo->struCollectItemInfo.ShapeNum, pBigIconImg));
			}
			
			itArmor++;		
		}
	}
	
// 	{
// 		int nMaxScroll = pvecArmorCollect->size()/MAX_ARMOR_X;
// 		nMaxScroll = max(1, nMaxScroll);
// 		if(m_pScroll)
// 		{
// 			m_pScroll->SetMaxItem(nMaxScroll);
// 		}
// 
// 	}
	

}

HRESULT CINFArmorCollectWnd::InvalidateDeviceObjects()
{
	if(m_pImgBackg)
	{
		m_pImgBackg->InvalidateDeviceObjects();		
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
	if(m_pApplyItem)
	{
		m_pApplyItem->InvalidateDeviceObjects();
	}
	if(m_pSearchListSel)
	{
		m_pSearchListSel->InvalidateDeviceObjects();
	}
	if(m_pUseSel)
	{
		m_pUseSel->InvalidateDeviceObjects();
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
	if(m_pArmorApplyBtn)
	{
		m_pArmorApplyBtn->InvalidateDeviceObjects();		
	}
	if(m_pArmorInitBtn)
	{
		m_pArmorInitBtn->InvalidateDeviceObjects();		
	}
	if(m_pLackItemImage)
	{
		m_pLackItemImage->InvalidateDeviceObjects();						
	}
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		if(m_pGearTabImg[nUnitCollectType])
		{
			m_pGearTabImg[nUnitCollectType]->InvalidateDeviceObjects();
		}
	}

	InvalidateAllArmorIcon();		// 아머 아이콘 전체 갱신
	return S_OK ;
}

// 아머 아이콘 전체 갱신
void CINFArmorCollectWnd::InvalidateAllArmorIcon()
{
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		map<INT, CINFImageEx*>::iterator itArmor = m_mapBigIconInfo[nUnitCollectType].begin();		// BigIcon	
		while(itArmor != m_mapBigIconInfo[nUnitCollectType].end())
		{
			CINFImageEx* pBigArmor = (itArmor->second);		
			
			if(pBigArmor)
			{
				pBigArmor->InvalidateDeviceObjects();
			}
			
			itArmor++;		
		}
	}
	
}
HRESULT CINFArmorCollectWnd::DeleteDeviceObjects()
{
	if(m_pImgBackg)
	{
		m_pImgBackg->DeleteDeviceObjects();
		util::del(m_pImgBackg);
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
	if(m_pApplyItem)
	{
		m_pApplyItem->DeleteDeviceObjects();
		util::del(m_pApplyItem);
	}
	if(m_pSearchListSel)
	{		
		m_pSearchListSel->DeleteDeviceObjects();
		util::del(m_pSearchListSel);
	}
	if(m_pUseSel)
	{
		m_pUseSel->DeleteDeviceObjects();
		util::del(m_pUseSel);
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
	if(m_pArmorApplyBtn)
	{
		m_pArmorApplyBtn->DeleteDeviceObjects();
		util::del(m_pArmorApplyBtn);
	}	
	if(m_pArmorInitBtn)
	{
		m_pArmorInitBtn->DeleteDeviceObjects();
		util::del(m_pArmorInitBtn);
	}	
	if(m_pLackItemImage)
	{
		m_pLackItemImage->DeleteDeviceObjects();				
		util::del(m_pLackItemImage);
	}
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		if(m_pGearTabImg[nUnitCollectType])
		{
			m_pGearTabImg[nUnitCollectType]->DeleteDeviceObjects();				
			util::del(m_pGearTabImg[nUnitCollectType]);
		}
	}


	DeleteAllArmorIcon();
	return S_OK ;
}
void CINFArmorCollectWnd::DeleteAllArmorIcon()
{
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		map<INT, CINFImageEx*>::iterator itArmor = m_mapBigIconInfo[nUnitCollectType].begin();		// BigIcon	
		while(itArmor != m_mapBigIconInfo[nUnitCollectType].end())
		{
			CINFImageEx* pBigArmor = (itArmor->second);				
			if(pBigArmor)
			{
				pBigArmor->DeleteDeviceObjects();
				util::del(pBigArmor);
			}		
			itArmor++;		
		}
		m_mapBigIconInfo[nUnitCollectType].clear();	
	}
	
}

CINFImageEx* CINFArmorCollectWnd::GetArmorIcon(INT nUnitCollectType, INT nItemNum)
{
	if(nUnitCollectType >= MAX_COLLECT_UNIT)
	{
		return NULL;
	}
	map<INT, CINFImageEx*>::iterator it_map = m_mapBigIconInfo[nUnitCollectType].find(nItemNum);
	if(it_map != m_mapBigIconInfo[nUnitCollectType].end())
	{
		return it_map->second;
	}

	// 정보가 없으면 여기서 insert처리
	CINFImageEx* pBigIconImg = g_pGameMain->FindBigIcon(nItemNum); // 이곳에서 Init, Resotre이미지 관련 처리 다한다.
	m_mapBigIconInfo[nUnitCollectType].insert(pair<INT, CINFImageEx*>(nItemNum, pBigIconImg)); // NULL이라도 insert하자
	return pBigIconImg;
}

void CINFArmorCollectWnd::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}	
}

void CINFArmorCollectWnd::Render()
{
	if(!IsShowWnd())
	{
		return;
	}

	RenderCommonCtrl();		// 컨트롤들 디스 플레이	
	RenderArmorList();		// 아머 정보 랜더링
	RenderGearTab();

}

void CINFArmorCollectWnd::RenderCommonCtrl()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		
	{
		m_pImgBackg->Move( nWindowPosX, nWindowPosY);
		m_pImgBackg->Render();
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

// 2013-07-11 by bhsohn 아머 컬렉션 수정안
	RenderInitArmor();		// 초기화 아머
	RenderCurrentArmor();	// 현재 외형
	RenderApplyArmor();		// 변영후 아머

	if(m_pArmorApplyBtn)
	{
		m_pArmorApplyBtn->Render();
	}	
	if(m_pArmorInitBtn)
	{
		m_pArmorInitBtn->Render();
	}	
	
}

void CINFArmorCollectWnd::RenderInitArmor()
{
	if(m_uItemUniNum <= 0)
	{
		return;
	}
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_uItemUniNum);
	
	int nUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);			
	
	if(!pItemInfo || !pItemInfo->ItemInfo) // 인벤에 있는 아이템만
	{
		return;
	}	
	INT nIconItemNum = pItemInfo->ItemInfo->SourceIndex;
		
	CINFImageEx* pArmorIcon = GetArmorIcon(nUnitCollectType, nIconItemNum);
	if(!pArmorIcon)
	{
		return;
	}
	
	int nStartX = m_ptBkPos.x + m_rcInitItem.left;
	int nStartY = m_ptBkPos.y + m_rcInitItem.top;
	
	pArmorIcon->Move(nStartX, nStartY);
	pArmorIcon->Render();	
	if(m_pItemListFont)
	{
		int nTmpX, nTmpY;
		nTmpX = nTmpY = 0;		
		char chTxt[64] = {0,};
		
		// 아이템 이름
		{
			sprintf(chTxt, "%s", pItemInfo->ItemInfo->ItemName);					

			g_pGameMain->TextReduce(m_pItemListFont, 150, chTxt);

			SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
			
			nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
			nTmpY = nStartY + BIGICON_HEIGHT + 2 + TEXT_CAP;
			
			m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
		}								
	}	
}

void CINFArmorCollectWnd::RenderCurrentArmor()
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	if(m_uItemUniNum <= 0)
	{
		return;
	}
	CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_uItemUniNum);
	
	int nUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);			
	
	if(!pItemInfo || !pItemInfo->ItemInfo) // 인벤에 있는 아이템만
	{
		return;
	}
	
	ITEM* pShapeItem = NULL;
	INT nIconItemNum = pItemInfo->ItemInfo->SourceIndex;
	if(pItemInfo->ShapeItemNum > 0)
	{
		pShapeItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
		if(pShapeItem)
		{
			nIconItemNum = pShapeItem->SourceIndex;
		}
	}			
	CINFImageEx* pArmorIcon = GetArmorIcon(nUnitCollectType, nIconItemNum);
	if(!pArmorIcon)
	{
		return;
	}
	
	int nStartX = m_ptBkPos.x + m_rcShowItem.left;
	int nStartY = m_ptBkPos.y + m_rcShowItem.top;
	
	pArmorIcon->Move(nStartX, nStartY);
	pArmorIcon->Render();	
	if(m_pItemListFont)
	{
		int nTmpX, nTmpY;
		nTmpX = nTmpY = 0;		
		char chTxt[64] = {0,};
		
		// 아이템 이름
		{
			if(pShapeItem)
			{
				sprintf(chTxt, "%s", pShapeItem->ItemName);					
			}
			else
			{
				sprintf(chTxt, "%s", pItemInfo->ItemInfo->ItemName);					
			}

			g_pGameMain->TextReduce(m_pItemListFont, 120, chTxt);

			SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
			
			nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
			nTmpY = nStartY + BIGICON_HEIGHT+ 2 + TEXT_CAP;
			
			m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
		}	
		// 남은 시간
		if(pShapeItem) // 아머 컬력센 사용중일떄
		{	
			int nUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);			
			struArmorCollectionInfo *pArmorInfo = g_pStoreData->GetArmorCollectInfo_SourceIndex(nUnitCollectType, pShapeItem->SourceIndex);
			if(pArmorInfo)
			{
				int nSecondDiff = 0;
				if(0 == pArmorInfo->struCollectItemInfo.ActivedCount)
				{
					nSecondDiff = pArmorInfo->struCollectItemInfo.RemainSeconds;
				}
				else
				{
					ATUM_DATE_TIME	currenttime = GetServerDateTime();			
					ATUM_DATE_TIME	atimeEndTime = pArmorInfo->struCollectItemInfo.EndTime;
					nSecondDiff = atimeEndTime.GetTimeInSeconds() - currenttime.GetTimeInSeconds();
				}
				INT nSelItemNum = GetSelectArmorShapeNum(m_ptSelectPos);
				
				
				if(MAX_ARMOR_COLLECT_ENCHANT <= pArmorInfo->struCollectItemInfo.EnchantLevel)
				{
					sprintf(chTxt, STRMSG_C_130529_0005);	// "(무제한)"
					
				}
				else
				{
					sprintf(chTxt, STRMSG_C_130529_0004);	// "기간만료"
					
					if(nSecondDiff > 0)
					{
						int nDay, nHour, nMin;
						nDay = nHour = nMin = 0;
						ConvertSecond_To_DayHourMin(nSecondDiff, &nDay, &nHour, &nMin);						
						sprintf(chTxt, STRMSG_C_130529_0003, nDay, nHour, nMin);	// "기간만료"
					}	
				}
				
				{					
					SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
					
					nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
					nTmpY = nStartY + BIGICON_HEIGHT + szSize.cy+ 4 + TEXT_CAP;
					
					m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
				}
				
			}
			
		}
	}	
#endif
}

void CINFArmorCollectWnd::RenderApplyArmor()
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	INT nSelItemNum = GetSelectArmorShapeNum(m_ptSelectPos);
	if(!nSelItemNum)
	{
		return;
	}	
	ITEM* pItemInfo = g_pDatabase->GetServerItemInfo( nSelItemNum);		
	int nUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);			

	
	if(!pItemInfo) // 인벤에 있는 아이템만
	{
		return;
	}		

	INT nSelKitItemNum = GetSelectArmorItemNum(m_ptSelectPos); // 외형 킷 ItemNum	
	struArmorCollectionInfo *pArmorInfo = g_pStoreData->GetArmorCollectInfo(nUnitCollectType, nSelKitItemNum);

	if(!pArmorInfo)
	{
		return;
	}
	
	CINFImageEx* pArmorIcon = GetArmorIcon(nUnitCollectType, nSelItemNum);
	if(!pArmorIcon)
	{
		return;
	}
	
	int nStartX = m_ptBkPos.x + m_rcApplayItem.left;
	int nStartY = m_ptBkPos.y + m_rcApplayItem.top;
	
	pArmorIcon->Move(nStartX, nStartY);
	pArmorIcon->Render();	
	if(m_pItemListFont)
	{
		int nTmpX, nTmpY;
		nTmpX = nTmpY = 0;		
		char chTxt[64] = {0,};
		
		// 아이템 이름
		{
			sprintf(chTxt, "%s", pItemInfo->ItemName);					

			g_pGameMain->TextReduce(m_pItemListFont, 120, chTxt);

			SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
			
			nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
			nTmpY = nStartY + BIGICON_HEIGHT+ 2 + TEXT_CAP;
			
			m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
		}								
		// 남은 시간
		{	
			int nSecondDiff = 0;
			if(0 == pArmorInfo->struCollectItemInfo.ActivedCount)
			{
				nSecondDiff = pArmorInfo->struCollectItemInfo.RemainSeconds;
			}
			else
			{
				ATUM_DATE_TIME	currenttime = GetServerDateTime();			
				ATUM_DATE_TIME	atimeEndTime = pArmorInfo->struCollectItemInfo.EndTime;
				nSecondDiff = atimeEndTime.GetTimeInSeconds() - currenttime.GetTimeInSeconds();
			}
			INT nSelItemNum = GetSelectArmorShapeNum(m_ptSelectPos);

			
			if(MAX_ARMOR_COLLECT_ENCHANT <= pArmorInfo->struCollectItemInfo.EnchantLevel)
			{
				sprintf(chTxt, STRMSG_C_130529_0005);	// "(무제한)"
				
			}
			else
			{
				sprintf(chTxt, STRMSG_C_130529_0004);	// "기간만료"
				
				if(nSecondDiff > 0)
				{
					int nDay, nHour, nMin;
					nDay = nHour = nMin = 0;
					ConvertSecond_To_DayHourMin(nSecondDiff, &nDay, &nHour, &nMin);						
					sprintf(chTxt, STRMSG_C_130529_0003, nDay, nHour, nMin);	// "기간만료"
				}	
			}
			
			{					
				SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
				
				nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
				nTmpY = nStartY + BIGICON_HEIGHT + szSize.cy+ 4 + TEXT_CAP;
				
				m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
			}
		}
	}	
#endif
}

// 아머 정보 랜더링
void CINFArmorCollectWnd::RenderArmorList()
{
	//m_rcArmorItem
	int nUnitCollectType = m_nSelUnitCollectType;
	CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);
	if(!pvecArmorCollect)
	{
		return;
	}
	CVecArmorCollecInfoIterator itArmor = pvecArmorCollect->begin();
	
	int nStartPos = m_pScroll->GetScrollStep() * MAX_ARMOR_X;
	
	int nPos = 0;
	while(itArmor != pvecArmorCollect->end())
	{
		if(nPos >= nStartPos)
		{
			break;
		}
		nPos++;
		itArmor++;
	}	
	
	int nPosX = 0;
	int nPosY = 0;	

	while(itArmor != pvecArmorCollect->end())
	{
		if(nPosX >= MAX_ARMOR_X)
		{
			nPosX = 0;
			nPosY++;
		}
		if(nPosY >= MAX_ARMOR_Y)
		{
			break;
		}
		struArmorCollectionInfo* pArmorInfo = (*itArmor);
		CINFImageEx* pArmorIcon = GetArmorIcon(nUnitCollectType, pArmorInfo->struCollectItemInfo.ShapeNum);
		if(!pArmorIcon)
		{
			itArmor++;
			continue;
		}
		RECT rcArmorItem = m_rcArmorItem[nPosY][nPosX];		
		int nStartX = m_ptBkPos.x + rcArmorItem.left;
		int nStartY = m_ptBkPos.y + rcArmorItem.top;

		if(!pArmorInfo->bGetServerInfo)
		{
			RenderNotHaveArmor(nStartX, nStartY, pArmorIcon, pArmorInfo);
		}
		else
		{
			RenderHaveArmor(nStartX, nStartY, pArmorIcon, pArmorInfo);
		}
		CItemInfo* pApplyItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_uItemUniNum);
		if(pApplyItemInfo)
		{
			// 적용된 아머 항목
			if(pArmorInfo->struCollectItemInfo.ShapeNum == pApplyItemInfo->ShapeItemNum )
			{
				int nTmpX = nStartX-6;
				int nTmpY = nStartY-6;
				
				m_pApplyItem->Move(nTmpX, nTmpY);
				m_pApplyItem->Render();
			}
		}

		// 선택한 항목
		{	
			int nSelItemX = m_ptSelectPos.x;
			int nSelItemY = m_ptSelectPos.y  - m_pScroll->GetScrollStep();
			if(nPosY == nSelItemY && (nSelItemX == nPosX))
			{
				int nTmpX = nStartX-1;
				int nTmpY = nStartY-1;
				
				m_pSearchListSel->Move(nTmpX, nTmpY);
				m_pSearchListSel->Render();
			}
		}					

		nPosX++;
		itArmor++;	
	}		
}

void CINFArmorCollectWnd::RenderNotHaveArmor(int nStartX, int nStartY, CINFImageEx* pArmorIcon, struArmorCollectionInfo* pArmorInfo)
{	
	char chTxt[256] ={0,}; 
	
	pArmorIcon->Move(nStartX, nStartY);
	pArmorIcon->Render();	

	{
		// 없는 아이템
		m_pLackItemImage->Move(nStartX, nStartY);
		m_pLackItemImage->SetScale( pArmorIcon->GetImgSize().x, pArmorIcon->GetImgSize().y);
		m_pLackItemImage->Render();
	}

	if(m_pItemListFont)
	{
		int nTmpX, nTmpY;
		nTmpX = nTmpY = 0;				
		
		// 아이템 이름
		{	
			ITEM* pItem = g_pDatabase->GetServerItemInfo(pArmorInfo->struCollectItemInfo.ShapeNum);
			if(pItem)
			{
				sprintf(chTxt, "%s", pItem->ItemName);					

				g_pGameMain->TextReduce(m_pItemListFont, 120, chTxt);

				SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
				
				nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
				nTmpY = nStartY + BIGICON_HEIGHT + 2 + TEXT_CAP;
				
				m_pItemListFont->DrawText(nTmpX, nTmpY, RGB(75,75,75), chTxt);
			}				
		}
	}
		
}

void CINFArmorCollectWnd::RenderHaveArmor(int nStartX, int nStartY, CINFImageEx* pArmorIcon, struArmorCollectionInfo* pArmorInfo)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	char chTxt[256] ={0,}; 

	pArmorIcon->Move(nStartX, nStartY);
	pArmorIcon->Render();	
	if(m_pItemListFont)
	{
		int nTmpX, nTmpY;
		nTmpX = nTmpY = 0;
		// 인첸횟수
		{
			nTmpX = nStartX;
			nTmpY = nStartY;
			
			if(MAX_ARMOR_COLLECT_ENCHANT <= pArmorInfo->struCollectItemInfo.EnchantLevel)
			{
				sprintf(chTxt, STRMSG_C_130529_0002); // "완료"					
			}
			else
			{
				sprintf(chTxt, STRMSG_C_130529_0001, pArmorInfo->struCollectItemInfo.EnchantLevel); // "%d회"					
			}
			m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
		}
		
		// 아이템 이름
		{	
			ITEM* pItem = g_pDatabase->GetServerItemInfo(pArmorInfo->struCollectItemInfo.ShapeNum);
			if(pItem)
			{
				sprintf(chTxt, "%s", pItem->ItemName);					
				
				g_pGameMain->TextReduce(m_pItemListFont, 120, chTxt);

				SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
				
				nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
				nTmpY = nStartY + BIGICON_HEIGHT+ 2 + TEXT_CAP;
				
				m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
			}				
		}
		
		// 남은 시간
		{	
			int nSecondDiff = 0;
			if(0 == pArmorInfo->struCollectItemInfo.ActivedCount)
			{
				nSecondDiff = pArmorInfo->struCollectItemInfo.RemainSeconds;
			}
			else
			{
				ATUM_DATE_TIME	currenttime = GetServerDateTime();			
				ATUM_DATE_TIME	atimeEndTime = pArmorInfo->struCollectItemInfo.EndTime;
				nSecondDiff = atimeEndTime.GetTimeInSeconds() - currenttime.GetTimeInSeconds();
			}
			
			if(MAX_ARMOR_COLLECT_ENCHANT <= pArmorInfo->struCollectItemInfo.EnchantLevel)
			{
				sprintf(chTxt, STRMSG_C_130529_0005);	// "(무제한)"
				
			}
			else
			{
				sprintf(chTxt, STRMSG_C_130529_0004);	// "기간만료"
				
				if(nSecondDiff > 0)
				{
					int nDay, nHour, nMin;
					nDay = nHour = nMin = 0;
					ConvertSecond_To_DayHourMin(nSecondDiff, &nDay, &nHour, &nMin);						
					sprintf(chTxt, STRMSG_C_130529_0003, nDay, nHour, nMin);	// "기간만료"
				}	
			}
			
			{					
				SIZE szSize = m_pItemListFont->GetStringSize(chTxt);
				
				nTmpX = nStartX + BIGICON_WIDTH/2 - szSize.cx/2;
				nTmpY = nStartY + BIGICON_HEIGHT + szSize.cy+ 4 + TEXT_CAP;
				
				m_pItemListFont->DrawText(nTmpX, nTmpY, GUI_FONT_COLOR_W, chTxt);
			}
		}					
	}	
#endif
}

void CINFArmorCollectWnd::RenderGearTab()
{
	int nStartX = m_ptBkPos.x ;
	int nStartY = m_ptBkPos.y ;
	int nUnitCollectType = m_nSelUnitCollectType;

	if(nUnitCollectType < MAX_COLLECT_UNIT)
	{
		if(m_pGearTabImg[nUnitCollectType])
		{
			m_pGearTabImg[nUnitCollectType]->Move(nStartX, 
													nStartY+38);
			m_pGearTabImg[nUnitCollectType]->Render();
		}
	}
}

void CINFArmorCollectWnd::ConvertSecond_To_DayHourMin(INT nSecond, int *pDay, int *pHour, int *pMin)
{
	INT nRemainTime = nSecond;
	// 1일 = 60초*60분*24시간 
	INT nDay = nRemainTime/86400;
	
	nRemainTime = nRemainTime- (nDay*86400);

	INT nHour = nRemainTime/3600;
	nRemainTime = nRemainTime - (nHour*3600);

	INT nMinute = nRemainTime/60;

	(*pDay)		= nDay;
	(*pHour)	= nHour;
	(*pMin)		= nMinute;
}


BOOL CINFArmorCollectWnd::IsShowWnd()
{
	return m_bShowWnd;
}

void CINFArmorCollectWnd::ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *ptMixWndPos/*=NULL*/)
{	
	SetShowItemNumShowWnd(bShow, nShowItemNum, uItemUniNum);
	m_bShowWnd = bShow;
	if(bShow)
	{
		m_nSelUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);

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

		InitItemListScroll();  // 2013-07-25 by bhsohn 아머 컬렉션 스크롤 동작 안하는 버그 수정
		
		UpdateBtnPos();		

		g_pInterface->SetWindowOrder(WNDArmorCollection);		
	}
	else
	{
		SetTooltipItemInfo(0, 0, 0, 0); // 툴팁 초기화		
	}
}
void CINFArmorCollectWnd::SetShowItemNumShowWnd(BOOL bShow, int nShowItemNum, UID64_t uItemUniNum)
{
	m_nShowItemNum = nShowItemNum;
	m_uItemUniNum = uItemUniNum;
	if(bShow)
	{
		char chItemName[SIZE_MAX_ITEM_NAME] = {0,};		
		if(m_nShowItemNum > 0)
		{			
			ITEM* pItem = g_pDatabase->GetItemInfoLoadItemData(m_nShowItemNum);
			if(pItem)
			{
				strncpy(chItemName, pItem->ItemName, SIZE_MAX_ITEM_NAME);			
			}			
		}					
	}

}


int CINFArmorCollectWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			
			if(WM_KEYDOWN==uMsg )
			{
				switch(wParam)
				{
				case VK_ESCAPE:
					{				
						// 2013-06-17 by ssjung 외형 적용, 해제 버튼 툴팁 사라지 않는 현상 수정 
						if(g_pInterface && g_pInterface->m_pToolTip && g_pInterface->m_pToolTip->m_bToolTipState)
							g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
						//end 2013-06-17 by ssjung 외형 적용, 해제 버튼 툴팁 사라지 않는 현상 수정 

						// 현재 윈도우를 숨긴다.
						ShowWnd(FALSE, 0, 0);
						if(g_pGameMain && g_pGameMain->m_pInven)
						{
							g_pGameMain->m_pInven->ShowInven(NULL, NULL, FALSE);	
						}
						return INF_MSGPROC_BREAK;				
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


int CINFArmorCollectWnd::OnMouseWheel(WPARAM wParam, LPARAM lParam)
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
		BOOL bClick = m_pScroll->IsMouseWhellPos(pt);
		if(bClick)		
		{			
			m_pScroll->OnMouseWheel(wParam, lParam);	
			return INF_MSGPROC_BREAK;
		}
	}

	return INF_MSGPROC_NORMAL;	
	
}
int CINFArmorCollectWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
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
	{
		if(TRUE == m_pArmorApplyBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	{
		if(TRUE == m_pArmorInitBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}

	GUI_BUILDINGNPC* pTempBaseShop = g_pInterface->m_pCityBase->GetCurrentBuildingNPC(); // 상점이 없을때만 	
	if(IsMouseCaps(pt) && !pTempBaseShop)
	{
		m_ptCommOpMouse.x = pt.x - m_ptBkPos.x;
		m_ptCommOpMouse.y = pt.y - m_ptBkPos.y;
		m_bMove = TRUE;

		g_pInterface->SetWindowOrder(WNDArmorCollection);

		return INF_MSGPROC_BREAK;
	}			
	
	BOOL bClick = FALSE;	

	if(INF_MSGPROC_BREAK == OnCilckSelectItem(pt))
	{
		return INF_MSGPROC_BREAK;
	}

	if(INF_MSGPROC_BREAK == OnCilckGearTab(pt))
	{
		return INF_MSGPROC_BREAK;
	}
	

	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_ARMOR_COLLECT_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_ARMOR_COLLECT_WINDOWS_HEIGHT)
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

int CINFArmorCollectWnd::OnCilckSelectItem(POINT ptPos)
{
	int nStartX = m_ptBkPos.x ;
	int nStartY = m_ptBkPos.y ;
	
	int nCntX = 0;	
	int nCntY = 0;	
	
	for(nCntY = 0;nCntY < MAX_ARMOR_Y;nCntY++)
	{	
		for(nCntX = 0;nCntX < MAX_ARMOR_X;nCntX++)
		{
			if(((nStartY+m_rcArmorItem[nCntY][nCntX].top) <= ptPos.y) && (ptPos.y <= (nStartY+m_rcArmorItem[nCntY][nCntX].bottom)))
			{
				if(((nStartX+m_rcArmorItem[nCntY][nCntX].left) <= ptPos.x) && (ptPos.x <= (nStartX+m_rcArmorItem[nCntY][nCntX].right)))
				{	
					POINT ptSelectPos;
					ptSelectPos.x = nCntX;
					ptSelectPos.y = nCntY + m_pScroll->GetScrollStep();

					INT nSelItemNum = GetSelectArmorItemNum(ptSelectPos); // 외형 킷 ItemNum
					int nUnitCollectType = m_nSelUnitCollectType;	
					struArmorCollectionInfo *pColltionInfo = g_pStoreData->GetArmorCollectInfo(nUnitCollectType, nSelItemNum);
					if(pColltionInfo && pColltionInfo->bGetServerInfo) // 서버에서 정보 받은것만 클릭 가능
					{
						m_ptSelectPos = ptSelectPos;						
					}				
					return INF_MSGPROC_BREAK;	
				}	
			}
		}		
	}
//	m_ptSelectPos.x = m_ptSelectPos.y = -1; // 초기화// 2013-06-20 by bhsohn 아머 컬렉션 버그 수정
	return INF_MSGPROC_NORMAL;	
	
}

int CINFArmorCollectWnd::OnCilckGearTab(POINT ptPos)
{
	int nStartX = m_ptBkPos.x ;
	int nStartY = m_ptBkPos.y ;	

	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		if(((nStartY+m_rcGearTab[nUnitCollectType].top) <= ptPos.y) && (ptPos.y <= (nStartY+m_rcGearTab[nUnitCollectType].bottom)))
		{
			if(((nStartX+m_rcGearTab[nUnitCollectType].left) <= ptPos.x) && (ptPos.x <= (nStartX+m_rcGearTab[nUnitCollectType].right)))
			{				
				m_nSelUnitCollectType = nUnitCollectType;
				m_ptSelectPos.x = m_ptSelectPos.y = -1; // 초기화// 2013-06-20 by bhsohn 아머 컬렉션 버그 수정

				InitItemListScroll();  // 2013-07-25 by bhsohn 아머 컬렉션 스크롤 동작 안하는 버그 수정

				return INF_MSGPROC_BREAK;	
			}	
		}
	}
//	m_ptSelectPos.x = m_ptSelectPos.y = -1; // 초기화// 2013-06-20 by bhsohn 아머 컬렉션 버그 수정
	
	return INF_MSGPROC_NORMAL;	
	
}

int CINFArmorCollectWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
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
	
	m_pCloseBtn->OnMouseMove(pt);	

	m_pArmorApplyBtn->OnMouseMove(pt);	
	m_pArmorInitBtn->OnMouseMove(pt);	

	
	if(pt.x > m_ptBkPos.x + m_rcInitItem.left&&
		pt.x < m_ptBkPos.x + m_rcInitItem.right &&
		pt.y > m_ptBkPos.y +m_rcInitItem.top &&
		pt.y < m_ptBkPos.y + m_rcInitItem.bottom )		
	{
		SetTooltipItemInfo(m_nShowItemNum, pt.x, pt.y, 0);
		
		return INF_MSGPROC_BREAK;
	}

	if(pt.x > m_ptBkPos.x +m_rcShowItem.left&&
		pt.x < m_ptBkPos.x + m_rcShowItem.right &&
		pt.y > m_ptBkPos.y +m_rcShowItem.top &&
		pt.y < m_ptBkPos.y + m_rcShowItem.bottom &&
		m_nShowItemNum > 0)
	{
		CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber( m_uItemUniNum);
		if(pItemInfo && pItemInfo->ItemInfo)
		{			
			INT nIconItemNum = m_nShowItemNum;
			if(pItemInfo->ShapeItemNum > 0)
			{
				nIconItemNum = pItemInfo->ShapeItemNum;				
			}		
			SetTooltipItemInfo(nIconItemNum, pt.x, pt.y, 0);
			return INF_MSGPROC_BREAK;
		}
	}
	if(pt.x > m_ptBkPos.x + m_rcApplayItem.left&&
		pt.x < m_ptBkPos.x + m_rcApplayItem.right &&
		pt.y > m_ptBkPos.y +m_rcApplayItem.top &&
		pt.y < m_ptBkPos.y + m_rcApplayItem.bottom )		
	{
		int nUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);			
		INT nSelShapeItemNum = GetSelectArmorShapeNum(m_ptSelectPos);
		INT nSelItemNum = GetSelectArmorItemNum(m_ptSelectPos);
		struArmorCollectionInfo *pArmorInfo = g_pStoreData->GetArmorCollectInfo(nUnitCollectType, nSelItemNum);
		if(pArmorInfo && nSelShapeItemNum > 0)
		{
			SetTooltipItemInfo(nSelShapeItemNum, pt.x, pt.y, 0);
			return INF_MSGPROC_BREAK;
		}		
	}

	
	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_ARMOR_COLLECT_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_ARMOR_COLLECT_WINDOWS_HEIGHT)
	{	
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
		{
			int nFindPosX, nFindPosY;
			nFindPosX = nFindPosY = -1; 			
			int nLinkItem = 0;
			
			// 아이템 툴팁
			FindTooltipItemInfo_POS(pt, &nFindPosX, &nFindPosY);			
			struArmorCollectionInfo* pArmorInfo = GetTooltipItemInfo_ItemNum(m_pScroll->GetScrollStep(), nFindPosX, nFindPosY, 
														&nLinkItem);			

			if(pArmorInfo && pArmorInfo->bGetServerInfo)
			{
				// 아이템 툴팁
				SetTooltipItemInfo(nLinkItem, pt.x, pt.y, 0);
			}			
			else if(pArmorInfo)
			{
				ITEM* pItem = g_pDatabase->GetServerItemInfo( pArmorInfo->struCollectItemInfo.ShapeNum );				
				if(pItem)
				{
					g_pGameMain->SetToolTip( pt.x, pt.y, pItem->ItemName);
					return INF_MSGPROC_BREAK;
				}
			}
			else
			{
				g_pGameMain->SetToolTip(NULL, 0, 0);
				// 아이템 툴팁
				SetTooltipItemInfo(nLinkItem, pt.x, pt.y, 0);				
			}
		}		

		return INF_MSGPROC_BREAK;
		
	}
	
	return INF_MSGPROC_NORMAL;	
	
}

int CINFArmorCollectWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
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

	{
		if(TRUE == m_pCloseBtn->OnLButtonUp(pt))
		{					
			// 현재 윈도우를 숨긴다.
			ShowWnd(FALSE,0,0);
			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			return  INF_MSGPROC_BREAK;
		}
	}

	// 아머 적용 버튼 
	{
		if(TRUE == m_pArmorApplyBtn->OnLButtonUp(pt))
		{	
			OnClickApplyArrmor();
			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			return  INF_MSGPROC_BREAK;
		}
	}
	// 아머 해제 버튼 
	{
		if(TRUE == m_pArmorInitBtn->OnLButtonUp(pt))
		{			
			OnClickInitArmor();
			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_CLOSE_MENU, g_pShuttleChild->m_vPos, FALSE);
			return  INF_MSGPROC_BREAK;
		}
	}
	
	
	
	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_ARMOR_COLLECT_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_ARMOR_COLLECT_WINDOWS_HEIGHT)
	{
		BOOL bClick = m_pScroll->GetMouseMoveMode();
		if(bClick)
		{
			m_pScroll->SetMouseMoveMode(FALSE);
			return INF_MSGPROC_BREAK;
		}				
	}	

	return INF_MSGPROC_NORMAL;		
}

BOOL CINFArmorCollectWnd::IsMouseCaps(POINT ptPos)
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


void CINFArmorCollectWnd::UpdateBtnPos()
{
	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;			

	{		
		int nPosX = nWindowPosX + OPTION_BTN_CLOSE_X;
		int nPosY = nWindowPosY + OPTION_BTN_CLOSE_Y;	
		
		m_pCloseBtn->SetBtnPosition(nPosX, nPosY);
	}
	{		
		int nPosX = nWindowPosX + OPTION_BTN_ARMOR_X;
		int nPosY = nWindowPosY + OPTION_BTN_ARMOR_Y;	
		
		m_pArmorApplyBtn->SetBtnPosition(nPosX, nPosY);
	}
	{		
		int nPosX = nWindowPosX + OPTION_BTN_INIT_X;
		int nPosY = nWindowPosY + OPTION_BTN_INIT_Y;	
		
		m_pArmorInitBtn->SetBtnPosition(nPosX, nPosY);
	}

		
}


// Scroll 갱신
void CINFArmorCollectWnd::InitItemListScroll()
{
	int nUnitCollectType = m_nSelUnitCollectType;
	CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);

	int nItemCnt = pvecArmorCollect->size() ;	// 랜더링 할 벡터
//	int nPageCnt = ((nItemCnt-(MAX_ARMOR_X*MAX_ARMOR_Y))/MAX_ARMOR_X)+1;
	int nPageCnt = nItemCnt/MAX_ARMOR_X;

	m_pScroll->SetMaxItem(max(1, nPageCnt)); // 최대 MAX_SCROLL_CNT 갯수만 넣자	

	UpdateBtnPos();
}

void CINFArmorCollectWnd::UpdateItemListScroll(int nCurrentPage, int nStartPage)
{	
	
	int nUnitCollectType = m_nSelUnitCollectType;
	CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);

	int nItemCnt = pvecArmorCollect->size();	// 랜더링 할 벡터
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


// 아이템 툴팁
void CINFArmorCollectWnd::SetTooltipItemInfo(INT	nItemNum, int x, int y, int nLinkItem)
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
//		g_pGameMain->SetItemInfo(0, nItemNum, x, y);
		g_pGameMain->SetItemInfo(0, nItemNum, x, y, 0, TRUE, nLinkItem,TRUE);
	}
	else
	{
		g_pGameMain->SetItemInfo(0,0,0,0);
	}		
}

// 아이템 툴팁
void CINFArmorCollectWnd::FindTooltipItemInfo_POS(POINT pt, int *nFindPosX, int *nFindPosY)
{
	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;
	
	(*nFindPosX) = (*nFindPosY) = -1;

	int nCntX, nCntY;
	nCntX = nCntY = 0;
	for(nCntY=0;nCntY< MAX_ARMOR_Y;nCntY++)
	{
		for(nCntX=0;nCntX< MAX_ARMOR_X;nCntX++)
		{
			if(((nStartY+m_rcArmorItem[nCntY][nCntX].top) <= pt.y) && (pt.y <= (nStartY+m_rcArmorItem[nCntY][nCntX].bottom)))
			{
				if(((nStartX+m_rcArmorItem[nCntY][nCntX].left) <= pt.x) && (pt.x <= (nStartX+m_rcArmorItem[nCntY][nCntX].right)))
				{					
					(*nFindPosX) = nCntX;
					(*nFindPosY) = nCntY;					
					return;
				}
			}			
		}
	}		
		
}

struArmorCollectionInfo* CINFArmorCollectWnd::GetTooltipItemInfo_ItemNum(int nScroll, int nFindPosX, int nFindPosY, int* pLinkItem)
{
	if(nFindPosX < 0 || nFindPosY < 0 )
	{
		return NULL;
	}
	else if(nFindPosX >= MAX_ARMOR_X  || nFindPosY >= m_nMaxShowItemLen)
	{
		return NULL;
	}
	
	int nFindItemNum = 0;
	int nMaxLine = m_nMaxShowItemLen;
	
	int nUnitCollectType = m_nSelUnitCollectType;
	CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);

	CVecArmorCollecInfoIterator itArmor = pvecArmorCollect->begin();

	int nStartPos = nScroll * MAX_ARMOR_X;

	int nPos =0;
	
	{
		// 스크롤과 페이지에 관한 찾기
		while(itArmor != pvecArmorCollect->end())
		{			
			if(nPos >= nStartPos)
			{
				break;
			}
			nPos++;
			itArmor++;
		}	
	}

	int nPosX = 0;
	int nPosY = 0;
	char chTxt[256] ={0,}; 

	while(itArmor != pvecArmorCollect->end())
	{
		if(nPosX >= MAX_ARMOR_X)
		{
			nPosX = 0;
			nPosY++;
		}
		if(nPosY >= MAX_ARMOR_Y)
		{
			break;
		}
		struArmorCollectionInfo* pArmorInfo = (*itArmor);		
		if((nFindPosX == nPosX) && (nFindPosY == nPosY))
		{
// 			nFindItemNum = pArmorInfo->struCollectItemInfo.ShapeItemNum; 			
// 			return nFindItemNum;
			(*pLinkItem) = pArmorInfo->struCollectItemInfo.ShapeNum;			
			return pArmorInfo;
		}		
		nPosX++;
		itArmor++;	
	}
	
	return NULL;
}


POINT CINFArmorCollectWnd::GetBkSize()
{
	return m_pImgBackg->GetImgSize();
}

DataHeader* CINFArmorCollectWnd::FindResource(char* szRcName)
{
	return CINFBase::FindResource(szRcName);
}
////////////////////// 아머 적용 //////////////////////
void CINFArmorCollectWnd::OnClickApplyArrmor()
{
	int nUnitCollectType = g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pShuttleChild->m_myShuttleInfo.UnitKind);			
	INT nSelItemNum = GetSelectArmorItemNum(m_ptSelectPos);
	struArmorCollectionInfo *pInfo = g_pStoreData->GetArmorCollectInfo(nUnitCollectType, nSelItemNum);
	if(!pInfo)
	{
		return;
	}

	INVEN_DISPLAY_INFO* pDisplayItemInfo = NULL;
	
	if(g_pGameMain && g_pGameMain->m_pInven)
	{
		pDisplayItemInfo = g_pGameMain->m_pInven->GetInvenDisplayInfoToUniqueNum(m_uItemUniNum);
	}

	if(!pDisplayItemInfo)
	{
		return;
	}
	
	int nFindPosX = m_ptSelectPos.x;
	int nFindPosY = m_ptSelectPos.y  - m_pScroll->GetScrollStep();
	
	int nLinkItem = 0;

	struArmorCollectionInfo* pArmorInfo = GetTooltipItemInfo_ItemNum(m_pScroll->GetScrollStep(), nFindPosX, nFindPosY, &nLinkItem);

	if(!pArmorInfo)
	{
		return;
	}
	
	g_pStoreData->RqCollectionShapeChange(m_uItemUniNum, pArmorInfo->struCollectItemInfo.ShapeNum);
}

////////////////////// 아머 초기화 //////////////////////
void CINFArmorCollectWnd::OnClickInitArmor()
{
	if(!m_uItemUniNum)
	{
		return;
	}	

	INVEN_DISPLAY_INFO* pDisplayItemInfo = NULL;

	if (g_pGameMain && g_pGameMain->m_pInven)
	{
		pDisplayItemInfo = g_pGameMain->m_pInven->GetInvenDisplayInfoToUniqueNum(m_uItemUniNum);
	}

	if (!pDisplayItemInfo)
	{
		return;
	}
	
	g_pStoreData->RqCollectionShapeChange(m_uItemUniNum, 0);
}

INT CINFArmorCollectWnd::GetSelectArmorShapeNum(POINT ptSelectPos)
{	
	if(-1 == ptSelectPos.x || -1 == ptSelectPos.y)
	{
		return  0;
	}
	int nUnitCollectType = m_nSelUnitCollectType;	
	CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);
	if(!pvecArmorCollect)
	{
		return 0 ;
	}
	CVecArmorCollecInfoIterator itArmor = pvecArmorCollect->begin();
	
	int nStartPos = m_pScroll->GetScrollStep() * MAX_ARMOR_X;
	
	int nPos = 0;
	while(itArmor != pvecArmorCollect->end())
	{
//		if(nStartPos >= nPos)
		if(nPos >= nStartPos)
		{
			break;
		}
		nPos++;
		itArmor++;
	}	
	
	int nPosX = 0;
	int nPosY = 0;	
	
	while(itArmor != pvecArmorCollect->end())
	{
		struArmorCollectionInfo* pArmorInfo = (*itArmor);

		if(nPosX >= MAX_ARMOR_X)
		{
			nPosX = 0;
			nPosY++;
		}
		if(nPosY >= MAX_ARMOR_Y)
		{
			break;
		}				
		// 선택한 항목
		{	
			int nSelItemX = ptSelectPos.x;
			int nSelItemY = ptSelectPos.y - m_pScroll->GetScrollStep();
			if(nPosY == nSelItemY && (nSelItemX == nPosX))
			{				
				return pArmorInfo->struCollectItemInfo.ShapeNum; // 외형 번호(소스인덱스)
			}
		}					
		
		nPosX++;
		itArmor++;	
	}
	return 0;
}


INT CINFArmorCollectWnd::GetSelectArmorItemNum(POINT ptSelectPos)
{	
	if(-1 == ptSelectPos.x || -1 == ptSelectPos.y)
	{
		return  0;
	}
	int nUnitCollectType = m_nSelUnitCollectType;	
	CVecArmorCollecInfo* pvecArmorCollect = g_pStoreData->GetVecArmorCollectInfo(nUnitCollectType);
	if(!pvecArmorCollect)
	{
		return 0 ;
	}
	CVecArmorCollecInfoIterator itArmor = pvecArmorCollect->begin();
	
	int nStartPos = m_pScroll->GetScrollStep() * MAX_ARMOR_X;
	
	int nPos = 0;
	while(itArmor != pvecArmorCollect->end())
	{
		if(nPos >= nStartPos )
		{
			break;
		}
		nPos++;
		itArmor++;
	}	
	
	int nPosX = 0;
	int nPosY = 0;	
	
	while(itArmor != pvecArmorCollect->end())
	{
		struArmorCollectionInfo* pArmorInfo = (*itArmor);
		
		if(nPosX >= MAX_ARMOR_X)
		{
			nPosX = 0;
			nPosY++;
		}
		if(nPosY >= MAX_ARMOR_Y)
		{
			break;
		}				
		// 선택한 항목
		{	
			int nSelItemX = ptSelectPos.x;
			int nSelItemY = ptSelectPos.y - m_pScroll->GetScrollStep();
			if(nPosY == nSelItemY && (nSelItemX == nPosX))
			{				
				return pArmorInfo->struCollectItemInfo.ShapeItemNum; // 외형킷 아이템 번호
			}
		}					
		
		nPosX++;
		itArmor++;	
	}

	return 0;
}