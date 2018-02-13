// INFWorldRankWnd.cpp: implementation of the CINFItemMixFactoryWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumError.h"
#include "AtumDatabase.h"
#include "ItemInfo.h"
#include "INFIcon.h"

#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "Interface.h"
#include "AtumSound.h"
#include "D3DHanFont.h"
#include "INFCityBase.h"
#include "INFListBox.h"
#include "INFItemMixWnd.h"
#include "INFToolTip.h"


#include "INFImageEx.h"									   // 2011. 10. 10 by jskim UI시스템 변경
#include "INFGroupManager.h"
#include "INFGroupImage.h"

#include "INFNumEditBox.H"
#include "INFCityLab.h"
#include "StoreData.h"
#include "INFWindow.h"
#include "INFInvenExtend.h"
#include "Chat.h"


#include "INFItemMixFactoryWnd.h"


/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define		MAX_FACTORY_SEARCH_Y		4
#define		PAGE_CNT_POSY				379//(383)

#define		ANI_NEXT_STEP_TIME			(0.1f)
#define		FULL_TIME_ANI_PLAYINGTIME	(1.5f)		// 애니메이션 최대 플레이 타임

////////////////////// 아이템 조합 모드 //////////////////////
#define		MODE_INVEN_ITEM			0		// 인벤에서 아이템 바로 선택
#define		MODE_SEARCH_SELECT		1		// 검색으로 선택

#define INF_FACTORY_MIX_WINDOWS_WIDTH			520														// 전체 넒이
#define INF_FACTORY_MIX_WINDOWS_HEIGTH			567														// 전체 높이

#define		MAX_PROB				10000	// 확률 [0, 10000)


CINFItemMixFactoryWnd::CINFItemMixFactoryWnd(CAtumNode* pParent)
{
	m_pParent = pParent;
	CINFItemMixFactoryWnd::InitWndData();

	m_nDiffScrollHeight = 54;

	m_nMaxShowItemLen	= MAX_FACTORY_SEARCH_Y;	// 최대 라인수	
	m_pPageCntBtnPosY	= PAGE_CNT_POSY;
	m_WndType			= WND_TYPE_FACTORY;
	sprintf(m_nBkImageGroup, "factory");
}

void CINFItemMixFactoryWnd::InitWndData()
{
	CINFItemMixWnd::InitWndData();

	int nCntX = 0;
	for(nCntX = 0;nCntX < MAX_INGAME_SEARCH_X;nCntX++)
	{
		m_rcFactorySourceItem[nCntX].left		= 37 + (ICON_WIDTH*nCntX) + (ICON_CAPX*nCntX);
		m_rcFactorySourceItem[nCntX].top		= 465;
		m_rcFactorySourceItem[nCntX].right		= m_rcFactorySourceItem[nCntX].left + ICON_WIDTH;
		m_rcFactorySourceItem[nCntX].bottom		= m_rcFactorySourceItem[nCntX].top	+ ICON_HEIGHT;
	}	

	m_rcFactoryTargetItem.left		= 452;
	m_rcFactoryTargetItem.top		= 465;
	m_rcFactoryTargetItem.right		= m_rcFactoryTargetItem.left + ICON_WIDTH;
	m_rcFactoryTargetItem.bottom	= m_rcFactoryTargetItem.top	+ ICON_HEIGHT;

	m_pFactoryAllBtn = NULL;
	m_pFactoryMakeBtn= NULL;
	m_pFactoryCancelBtn= NULL;
	m_pFactoryGetItemBtn = NULL;
	m_pFactoryInitBtn = NULL;
	m_pUpCntBtn= NULL;
	m_pDownCntBtn= NULL;
	m_pFactoryMakeGroup= NULL;

	m_pRollingImage = NULL;

	int nCnt =0;
	for(nCnt =0;nCnt < MAX_MIXSUCCESS;nCnt++)
	{
		m_pSuccessImage[nCnt] = NULL;
	}
	m_pFailImage = NULL;
	m_pLackItemImage = NULL;
	
	m_pNumEditBox = NULL;
	
	for(nCntX = 0;nCntX < MAX_FACTORY_ANI;nCntX++)
	{
		m_rcAniRect[nCntX].left		= 0;
		m_rcAniRect[nCntX].top		= 0;
		m_rcAniRect[nCntX].right	= 1;
		m_rcAniRect[nCntX].bottom	= 1;
	}
	m_nAniPlayIndex = -1;
	m_fAniElapseTime = 0.0f;
	m_fAllAniElapseTime = -1.0f;

	m_nSuccessIndex= -1;
	m_fSuccessElapseTime = 0.0f;
	m_fAllSuccessElapseTime = -1.0f;

	////////////////// 실패 이미지 표시//////////////////	
	m_fAllFailElapseTime = -1.0f;

	memset(&m_struWillResultTargetMixInfo, 0x00, sizeof(ITEM_MIXING_INFO));
	memset(m_struSearchSourceItem, 0x00, sizeof(ITEM_W_COUNT)* COUNT_ITEM_MIXING_SOURCE);
	m_uFinalResultTargetUniqueNum = 0;

	m_nMixMode = MODE_INVEN_ITEM;

	m_nTargetMakingCount = 1; 	
	
}

CINFItemMixFactoryWnd::~CINFItemMixFactoryWnd()
{
	util::del(m_pFactoryAllBtn);
	util::del(m_pFactoryMakeBtn);
	util::del(m_pFactoryCancelBtn);
	util::del(m_pFactoryGetItemBtn);
	util::del(m_pFactoryInitBtn);
	util::del(m_pNumEditBox);
	util::del(m_pUpCntBtn);
	util::del(m_pDownCntBtn);
	util::del(m_pRollingImage);	
	int nCnt =0;
	for(nCnt =0;nCnt < MAX_MIXSUCCESS;nCnt++)
	{
		util::del(m_pSuccessImage[nCnt]);
	}
	util::del(m_pFailImage);
	util::del(m_pLackItemImage);

}

HRESULT CINFItemMixFactoryWnd::InitDeviceObjects()
{
	CINFItemMixWnd::InitDeviceObjects();
	DataHeader*		pDataHeader = NULL;
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "Mallbtn_1");
		wsprintf(szDownBtn, "Mallbtn_2");
		wsprintf(szSelBtn, "Mallbtn_1");
		wsprintf(szDisBtn, "Mallbtn_2");
		if(NULL == m_pFactoryAllBtn)
		{
			m_pFactoryAllBtn = new CINFImageBtn;
		}
		m_pFactoryAllBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "mnQup03");
		wsprintf(szDownBtn, "mnQup01");
		wsprintf(szSelBtn, "mnQup00");
		wsprintf(szDisBtn, "mnQup02");
		if(NULL == m_pUpCntBtn)
		{
			m_pUpCntBtn = new CINFImageBtn;
		}
		m_pUpCntBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "mnQdn03");
		wsprintf(szDownBtn, "mnQdn01");
		wsprintf(szSelBtn, "mnQdn00");
		wsprintf(szDisBtn, "mnQdn02");
		if(NULL == m_pDownCntBtn)
		{
			m_pDownCntBtn = new CINFImageBtn;
		}
		m_pDownCntBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char buf[32];
		wsprintf(buf,"mix_ing");
		pDataHeader = g_pGameMain->FindResource(buf);

		if(pDataHeader && NULL == m_pRollingImage)
		{
			m_pRollingImage = new CINFImageEx;
			m_pRollingImage->InitDeviceObjects( pDataHeader );
		}		
	}
	{
		int nCnt =0;
		for(nCnt =0;nCnt < MAX_MIXSUCCESS;nCnt++)
		{			
			char buf[32];
			wsprintf(buf,"mix_suc%d", nCnt);
			pDataHeader = g_pGameMain->FindResource(buf);
			
			if(pDataHeader && NULL == m_pSuccessImage[nCnt])
			{
				m_pSuccessImage[nCnt] = new CINFImageEx;
				m_pSuccessImage[nCnt]->InitDeviceObjects( pDataHeader );
			}		
		}
	}
	{		
		char buf[32];
		wsprintf(buf,"diskill");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(pDataHeader && NULL == m_pFailImage)
		{
			m_pFailImage = new CINFImageEx;
			m_pFailImage->InitDeviceObjects( pDataHeader );
		}		

	}
	
	{		
		char buf[32];
		wsprintf(buf,"LM_inven");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(pDataHeader && NULL == m_pLackItemImage)
		{
			m_pLackItemImage = new CINFImageEx;
			m_pLackItemImage->InitDeviceObjects( pDataHeader );
		}		
		
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	{
		DataHeader* pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("B_admbtn2");	
		if(pDataHeader)
		{
			m_pFactoryMakeGroup = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		}
	}
#endif

	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
		if(NULL == m_pFactoryMakeBtn)
		{
			m_pFactoryMakeBtn = new CINFImageBtn;
			m_pFactoryMakeBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);			
			m_pFactoryMakeBtn->SetTooltipType("STRTOOLTIP31", "STRTOOLTIP31");
		}		
	}
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
		if(NULL == m_pFactoryCancelBtn)
		{
			m_pFactoryCancelBtn = new CINFImageBtn;
		}
		m_pFactoryCancelBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);				
		m_pFactoryCancelBtn->SetTooltipType("STRTOOLTIP25", "STRTOOLTIP25");
	}

	// 아이템 받기
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
		if(NULL == m_pFactoryGetItemBtn)
		{
			m_pFactoryGetItemBtn = new CINFImageBtn;
		}
		m_pFactoryGetItemBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pFactoryGetItemBtn->SetTooltipType("STRTOOLTIP108", "STRTOOLTIP108");
		
	}
	// 아이템 비우기
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
		if(NULL == m_pFactoryInitBtn)
		{
			m_pFactoryInitBtn = new CINFImageBtn;
		}
		m_pFactoryInitBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
		m_pFactoryInitBtn->SetTooltipType("STRTOOLTIP32", "STRTOOLTIP32");
		
	}
	{
		if(NULL == m_pNumEditBox)
		{
			m_pNumEditBox = new CINFNumEditBox;
		}
		char chBuff[32];
		char chMaxMixCnt[64];
		
		wsprintf(chBuff, "1");
		wsprintf(chMaxMixCnt, "%d", COUNT_MAX_MIXING_COUNT);
		
		POINT ptPos = {307, 470};
		m_pNumEditBox->InitDeviceObjects(9, ptPos, 70, TRUE, 20);				
		m_pNumEditBox->SetMaxStringLen(strlen(chMaxMixCnt));
		m_pNumEditBox->SetString(chBuff, 32);
	}
	
	return S_OK;
}
HRESULT CINFItemMixFactoryWnd::RestoreDeviceObjects()
{
	CINFItemMixWnd::RestoreDeviceObjects();
	if(m_pFactoryAllBtn)
	{				
		m_pFactoryAllBtn->RestoreDeviceObjects();				
	}
	if(m_pUpCntBtn)
	{				
		m_pUpCntBtn->RestoreDeviceObjects();				
	}
	if(m_pDownCntBtn)
	{				
		m_pDownCntBtn->RestoreDeviceObjects();				
	}
	if(m_pFactoryMakeBtn)
	{				
		m_pFactoryMakeBtn->RestoreDeviceObjects();				
	}
	if(m_pRollingImage)
	{
		m_pRollingImage->RestoreDeviceObjects();				

		{
			POINT ptSize = m_pRollingImage->GetImgSize();			
			
			int nOneImageWidth	= ptSize.x/5;
			int nOneImageHeight = ptSize.y/2;

			int nCnt = 0;
			for(nCnt=0;nCnt< MAX_FACTORY_ANI;nCnt++)
			{
				int nAniIdxX = nCnt%5;
				int nAniIdxY = nCnt/5;

				m_rcAniRect[nCnt].left		= nOneImageWidth*nAniIdxX;
				m_rcAniRect[nCnt].top		= nOneImageHeight*nAniIdxY;
				m_rcAniRect[nCnt].right		= m_rcAniRect[nCnt].left + nOneImageWidth;
				m_rcAniRect[nCnt].bottom	= m_rcAniRect[nCnt].top + nOneImageHeight;
			}
		}
	}
	int nCnt =0;
	for(nCnt =0;nCnt < MAX_MIXSUCCESS;nCnt++)
	{
		if(m_pSuccessImage[nCnt])
		{
			m_pSuccessImage[nCnt]->RestoreDeviceObjects();			
		}
	}
	if(m_pFailImage)
	{
		m_pFailImage->RestoreDeviceObjects();
	}
	if(m_pLackItemImage)
	{
		m_pLackItemImage->RestoreDeviceObjects();
	}

	if(m_pFactoryCancelBtn)
	{				
		m_pFactoryCancelBtn->RestoreDeviceObjects();				
	}
	if(m_pFactoryGetItemBtn)
	{
		m_pFactoryGetItemBtn->RestoreDeviceObjects();				
	}
	if(m_pFactoryInitBtn)
	{
		m_pFactoryInitBtn->RestoreDeviceObjects();				
	}

	if(m_pNumEditBox)
	{
		m_pNumEditBox->RestoreDeviceObjects();		
	}	
	
	return S_OK;
}

HRESULT CINFItemMixFactoryWnd::DeleteDeviceObjects()
{
	CINFItemMixWnd::DeleteDeviceObjects();
	if(m_pFactoryAllBtn)
	{				
		m_pFactoryAllBtn->DeleteDeviceObjects();				
		util::del(m_pFactoryAllBtn);
	}
	if(m_pUpCntBtn)
	{				
		m_pUpCntBtn->DeleteDeviceObjects();				
		util::del(m_pUpCntBtn);
	}
	if(m_pDownCntBtn)
	{				
		m_pDownCntBtn->DeleteDeviceObjects();				
		util::del(m_pDownCntBtn);
	}
	if(m_pFactoryMakeBtn)
	{				
		m_pFactoryMakeBtn->DeleteDeviceObjects();				
		util::del(m_pFactoryMakeBtn);
	}
	if(m_pRollingImage)
	{
		m_pRollingImage->DeleteDeviceObjects();				
		util::del(m_pRollingImage);
	}
	int nCnt =0;
	for(nCnt =0;nCnt < MAX_MIXSUCCESS;nCnt++)
	{
		if(m_pSuccessImage[nCnt])
		{
			m_pSuccessImage[nCnt]->DeleteDeviceObjects();				
			util::del(m_pSuccessImage[nCnt]);
		}
	}
	
	if(m_pFailImage)
	{
		m_pFailImage->DeleteDeviceObjects();				
		util::del(m_pFailImage);
	}
	if(m_pLackItemImage)
	{
		m_pLackItemImage->DeleteDeviceObjects();				
		util::del(m_pLackItemImage);
	}

	if(m_pFactoryCancelBtn)
	{				
		m_pFactoryCancelBtn->DeleteDeviceObjects();				
		util::del(m_pFactoryCancelBtn);
	}
	if(m_pFactoryGetItemBtn)
	{
		m_pFactoryGetItemBtn->DeleteDeviceObjects();				
		util::del(m_pFactoryGetItemBtn);
	}
	if(m_pFactoryInitBtn)
	{
		m_pFactoryInitBtn->DeleteDeviceObjects();				
		util::del(m_pFactoryInitBtn);
	}
	if(m_pNumEditBox)
	{				
		m_pNumEditBox->DeleteDeviceObjects();				
		util::del(m_pNumEditBox);
	}
	
	return S_OK;
}

HRESULT CINFItemMixFactoryWnd::InvalidateDeviceObjects()
{
	CINFItemMixWnd::InvalidateDeviceObjects();
	if(m_pFactoryAllBtn)
	{				
		m_pFactoryAllBtn->InvalidateDeviceObjects();				
	}
	if(m_pUpCntBtn)
	{				
		m_pUpCntBtn->InvalidateDeviceObjects();				
	}
	if(m_pDownCntBtn)
	{				
		m_pDownCntBtn->InvalidateDeviceObjects();				
	}
	if(m_pFactoryMakeBtn)
	{				
		m_pFactoryMakeBtn->InvalidateDeviceObjects();				
	}
	if(m_pRollingImage)
	{
		m_pRollingImage->InvalidateDeviceObjects();				
	}
	int nCnt =0;
	for(nCnt =0;nCnt < MAX_MIXSUCCESS;nCnt++)
	{
		if(m_pSuccessImage[nCnt])
		{
			m_pSuccessImage[nCnt]->InvalidateDeviceObjects();							
		}
	}	
	if(m_pFailImage)
	{
		m_pFailImage->InvalidateDeviceObjects();						
	}
	if(m_pLackItemImage)
	{
		m_pLackItemImage->InvalidateDeviceObjects();						
	}

	if(m_pFactoryCancelBtn)
	{				
		m_pFactoryCancelBtn->InvalidateDeviceObjects();				
	}
	if(m_pFactoryGetItemBtn)
	{
		m_pFactoryGetItemBtn->InvalidateDeviceObjects();				
	}
	if(m_pFactoryInitBtn)
	{
		m_pFactoryInitBtn->InvalidateDeviceObjects();				
	}
	if(m_pNumEditBox)
	{
		m_pNumEditBox->InvalidateDeviceObjects();				
	}
	
	return S_OK;
}

void CINFItemMixFactoryWnd::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}
	CINFItemMixWnd::Tick();
	if(m_pNumEditBox)
	{
		m_pNumEditBox->Tick();
	}	
}

void CINFItemMixFactoryWnd::TickMakingAni()
{
	if(m_fAllAniElapseTime < 0.0f)
	{
		return;
	}
	{
		m_fAniElapseTime += g_pD3dApp->GetElapsedTime();
		if(m_fAniElapseTime > ANI_NEXT_STEP_TIME)
		{
			m_fAniElapseTime = 0.0f;
			m_nAniPlayIndex++;
			if(m_nAniPlayIndex >= (MAX_FACTORY_ANI-1))
			{
				m_nAniPlayIndex = 1;
			}
		}
	}
	
	{
		m_fAllAniElapseTime -= g_pD3dApp->GetElapsedTime();
		if(m_fAllAniElapseTime < 0.0f)
		{
			m_fAllAniElapseTime = -1.0f;

			// 제조 시작
			SendMakeItem();			
		}
		
	}
}

void CINFItemMixFactoryWnd::TickMakeSuccess()
{
	if(m_fAllSuccessElapseTime < 0.0f)
	{
		return;
	}
	{
		m_fSuccessElapseTime += g_pD3dApp->GetElapsedTime();
		if(m_fSuccessElapseTime > ANI_NEXT_STEP_TIME)
		{
			m_fSuccessElapseTime = 0.0f;
			m_nSuccessIndex++;
			if(m_nSuccessIndex >= (MAX_MIXSUCCESS))
			{
				m_nSuccessIndex = 0;
			}
		}
	}
	
	{
		m_fAllSuccessElapseTime -= g_pD3dApp->GetElapsedTime();
		if(m_fAllSuccessElapseTime < 0.0f)
		{
			m_fAllSuccessElapseTime = -1.0f;			
			InitFactorySuccessValue(FALSE);
		}		
	}
}

void CINFItemMixFactoryWnd::TickMakeFail()
{
	if(m_fAllFailElapseTime < 0.0f)
	{
		return;
	}	
	
	{
		m_fAllFailElapseTime -= g_pD3dApp->GetElapsedTime();
		if(m_fAllFailElapseTime < 0.0f)
		{
			m_fAllFailElapseTime = -1.0f;						
		}		
	}
}
void CINFItemMixFactoryWnd::Render()
{
	if(!IsShowWnd())
	{
		return;
	}
	TickMakingAni();
	TickMakeSuccess();
	TickMakeFail();

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		

	CINFItemMixWnd::Render();
	RenderSourceItem();
	RenderFactoryTargetItem();
	RenderRollingImage();
}

void CINFItemMixFactoryWnd::RenderSourceItem()
{	
	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;		
	int nCntX = 0;	
	vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();
	vector<CItemInfo*>  *pVecTarget = ((CINFCityLab*)m_pParent)->GetvecTarget();	
	vector<CItemInfo*>::iterator itSource = pVecSource->begin();
	vector<CItemInfo*>::iterator itTarget = pVecTarget->begin();
//	vector<ITEM_GENERAL>::iterator itTarget = m_vecTargetItem.begin();
	

//	if( 0 != m_struSearchSourceItem[COUNT_ITEM_MIXING_SOURCE-1].ItemNum)
	if(MODE_SEARCH_SELECT == m_nMixMode)
	{			
		int nCnt = 0;
		for(nCnt=(COUNT_ITEM_MIXING_SOURCE-1); nCnt >= 0 ;nCnt--)
		{
			if(0 == m_struSearchSourceItem[nCnt].ItemNum)
			{
				continue;
			}

			nCntX = min(nCntX, (COUNT_ITEM_MIXING_SOURCE-1));

			int nIconStartX = nStartX + m_rcFactorySourceItem[nCntX].left;
			int nIconStartY = nStartY + m_rcFactorySourceItem[nCntX].top;
			
			{
				m_pSearchIconBk->Move(nIconStartX, nIconStartY);
				m_pSearchIconBk->Render();
			}
			CINFIcon*	pRenderIcon = NULL;

			DWORD dwFontColor = GUI_FONT_COLOR_W;
			if(m_pItemListFont)
			{
				char chItemCnt[64] = {0,};
				INT nMixCounts = GetMakeNumCnt();
//				int nInvenItemCnt	= g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt(m_struSearchSourceItem[nCnt].ItemNum);
				int nInvenItemCnt	= GetCntInven_ItemNum_To_AllItemCnt(m_struSearchSourceItem[nCnt].ItemNum);
				int nSourceItemCnt	= m_struSearchSourceItem[nCnt].Count * nMixCounts;
				wsprintf(chItemCnt, "%d/%d", nInvenItemCnt, nSourceItemCnt);				
				SIZE szFontSize = m_pItemListFont->GetStringSize(chItemCnt);
				POINT ptBkSize = m_pSearchIconBk->GetImgSize();
				int nStrPosX = nIconStartX + ptBkSize.x/2 - szFontSize.cx/2;
				int nStrPosY = nIconStartY + ptBkSize.y;

				if((-1 == m_nAniPlayIndex ) && nInvenItemCnt < nSourceItemCnt) // 조합중일떄는 Red로 표시 안함
				{
					dwFontColor = GUI_FONT_COLOR_R;					
				}			
				
				pRenderIcon = RenderMixSourceItemInfo(m_struSearchSourceItem[nCnt].ItemNum, 0, nIconStartX, nIconStartY, m_pConuntFont);							

				// 글씨를 써주자
				m_pItemListFont->DrawText(nStrPosX,nStrPosY,dwFontColor, chItemCnt);
			}
			if(GUI_FONT_COLOR_R == dwFontColor && m_pLackItemImage && pRenderIcon)
			{
				// 없는 아이템
				m_pLackItemImage->Move(nIconStartX+1, nIconStartY+1);
				m_pLackItemImage->SetScale( pRenderIcon->GetIconSize().x, pRenderIcon->GetIconSize().y );
				m_pLackItemImage->Render();
			}
			nCntX++;
		}
	}
	else
	{
		while(itSource != pVecSource->end())	
		{	
			if(nCntX >= MAX_INGAME_SEARCH_X)
			{
				break;
			}
			// 배경 
			int nIconStartX = nStartX + m_rcFactorySourceItem[nCntX].left;
			int nIconStartY = nStartY + m_rcFactorySourceItem[nCntX].top;
			
			{
				m_pSearchIconBk->Move(nIconStartX, nIconStartY);
				m_pSearchIconBk->Render();
			}
			CItemInfo* pItem = *itSource;		
			RenderItemInfo(pItem, nIconStartX, nIconStartY, m_pItemListFont, -1, TRUE);
			
			itSource++;
			nCntX ++;
		}	
	}
	
	for(;nCntX < MAX_INGAME_SEARCH_X; nCntX++)
	{
		m_pSearchIconBk->Move(nStartX + m_rcFactorySourceItem[nCntX].left, nStartY + m_rcFactorySourceItem[nCntX].top);
		m_pSearchIconBk->Render();
	}

	// 목적 아이템	
	{
		int nIconStartX = nStartX + m_rcFactoryTargetItem.left;
		int nIconStartY = nStartY + m_rcFactoryTargetItem.top;
		
		m_pSearchIconBk->Move(nIconStartX, nIconStartY);
		m_pSearchIconBk->Render();
		
		if(itTarget != pVecTarget->end())
		{			
			CItemInfo* pTagetItem = *itTarget;
			INT nItemCnt = pTagetItem->CurrentCount;
			
			CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
			CItemInfo*	pTargetItem	= pCityLab->FindItemFromTarget( m_uFinalResultTargetUniqueNum);

			if(pTargetItem)
			{
				nItemCnt = GetTargetVecItemCount(pTargetItem->ItemNum);
				RenderItemInfo(pTargetItem, nIconStartX, nIconStartY, m_pItemListFont, nItemCnt, TRUE);

			}
			else if(m_struWillResultTargetMixInfo.TargetItemNum == pTagetItem->ItemNum)
			{
				nItemCnt = GetTargetVecItemCount(pTagetItem->ItemNum);
				RenderMixSourceItemInfo(pTagetItem->ItemNum, nItemCnt, nIconStartX, nIconStartY, m_pItemListFont, TRUE);
			}
			else
			{
				RenderMixSourceItemInfo(m_struWillResultTargetMixInfo.TargetItemNum, 0, nIconStartX, nIconStartY, m_pItemListFont, TRUE);
			}
		}
		else
		{			
			// 아이템 미리 보기 상태임
			CINFIcon*	pRenderIcon = RenderMixSourceItemInfo(m_struWillResultTargetMixInfo.TargetItemNum, 0, nIconStartX, nIconStartY, m_pItemListFont, TRUE);
			if(m_pLackItemImage && pRenderIcon)
			{
				// 없는 아이템
				m_pLackItemImage->Move(nIconStartX+1, nIconStartY+1);
				m_pLackItemImage->SetScale( pRenderIcon->GetIconSize().x, pRenderIcon->GetIconSize().y);
				m_pLackItemImage->Render();
			}
		}		
		if((m_struWillResultTargetMixInfo.TargetItemNum > 0) && (MAX_PROB != m_struWillResultTargetMixInfo.MixingProbability))
		{
			int nTmpStartX = nStartX + 25;
			int nTmpStartY = nStartY + 520;
			m_pConuntFont->DrawText(nTmpStartX,nTmpStartY,			RGB(207,214,20), 
				STRMSG_C_130222_0006);
			
		}
	}	

}

void CINFItemMixFactoryWnd::RenderItemInfo(CItemInfo* pItem, int nIconStartX, int nIconStartY, CD3DHanFont*	 pFont, INT nCurrentCount/*=-1*/, BOOL bCenter/*=FALSE*/)
{	
	if(!pItem)
	{
		return;
	}
	
	CINFIcon* pIcon = g_pGameMain->m_pIcon;
	char buf[32] = {0,};	
	
	if( !pItem->ShapeItemNum )
		wsprintf(buf, "%08d", pItem->ItemInfo->SourceIndex);
	else
	{
		ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItem->ShapeItemNum );
		if( pShapeItem )
			wsprintf(buf, "%08d", pShapeItem->SourceIndex );
		else
			wsprintf(buf, "%08d", pItem->ItemInfo->SourceIndex);
	}			
	
	pIcon->SetIcon( buf, nIconStartX+1, nIconStartY+1, 1.0f );
	pIcon->Render();

	memset(buf, 0x00, 32);

	if(nCurrentCount > 0)
	{
		wsprintf( buf, "%d", nCurrentCount);		
	}	
	else if(IS_COUNTABLE_ITEM(pItem->Kind))
	{
		wsprintf( buf, "%d", pItem->CurrentCount);		
	}
	if(strlen(buf) > 0)
	{
		SIZE size = pFont->GetStringSize(buf);
		
		int nTmpStartX = nIconStartX + 30 - (size.cx);
		int nTmpStartY = nIconStartY + 20;
		if(bCenter)
		{
			nTmpStartX = nIconStartX + 16 - (size.cx/2);
			nTmpStartY = nIconStartY + 30;
		}
		
		pFont->DrawText(nTmpStartX,nTmpStartY,
			GUI_FONT_COLOR_W, 
			buf);
	}
}
CINFIcon* CINFItemMixFactoryWnd::RenderMixSourceItemInfo(INT nItemNum, INT nCurrentCount, 
														 int nIconStartX, int nIconStartY, CD3DHanFont* pFont, BOOL bCenter/*=FALSE*/)
{		
	if(0 == nItemNum)
	{
		return NULL;
	}
	CINFIcon* pIcon = g_pGameMain->m_pIcon;
	char buf[32] = {0,};	

	ITEM* pItem = g_pDatabase->GetServerItemInfo( nItemNum);
	if(!pItem)
	{
		return NULL;
	}	

	wsprintf(buf, "%08d", pItem->SourceIndex );	
	
	pIcon->SetIcon( buf, nIconStartX+1, nIconStartY+1, 1.0f );
	pIcon->Render();	

	if((nCurrentCount > 0))
	{
		wsprintf( buf, "%d", nCurrentCount);
		SIZE size = pFont->GetStringSize(buf);
		
		int nTmpStartX = nIconStartX + 30 - (size.cx);
		int nTmpStartY = nIconStartY + 20;
		if(bCenter)
		{
			nTmpStartX = nIconStartX + 16 - (size.cx/2);
			nTmpStartY = nIconStartY + 30;
		}
		
		pFont->DrawText(nTmpStartX,nTmpStartY,
			GUI_FONT_COLOR_W, 
			buf);
	}

	return pIcon;
}

void CINFItemMixFactoryWnd::RenderFactoryTargetItem()
{
	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;
	
	if(m_pFactoryAllBtn)
	{				
		m_pFactoryAllBtn->Render();				
	}
	if(m_pUpCntBtn)
	{				
		m_pUpCntBtn->Render();				
	}
	if(m_pDownCntBtn)
	{				
		m_pDownCntBtn->Render();				
	}
	if(m_pFactoryMakeBtn)
	{				
		m_pFactoryMakeBtn->Render();				
	}		
	if(m_pFactoryCancelBtn)
	{				
		m_pFactoryCancelBtn->Render();				
	}
	if(m_pFactoryGetItemBtn)
	{
		m_pFactoryGetItemBtn->Render();				
	}
	if(m_pFactoryInitBtn)
	{
		m_pFactoryInitBtn->Render();				
	}
	if(m_pConuntFont)
	{
		CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);

		// 가격 표시
		char strtemp2[64], strtemp1[64];					
		strtemp2[0] = strtemp1[0] = NULL;

		wsprintf(strtemp1, "0");
		char* pPrice = pCityLab->GetMixPrice();		// 조합 가격 결정		
		if(NULL != pPrice[0])
		{
			strncpy(strtemp1, pPrice, 64);
		}

		DWORD dwFontColor = GUI_FONT_COLOR_W;

		if(g_pGameMain
			&& g_pGameMain->m_pInven
			&& (atoi(strtemp1) > g_pGameMain->m_pInven->GetItemSpi()))
		{
			// 인벤의 돈이 더 크다.
			dwFontColor = GUI_FONT_COLOR_R;
		}

		MakeCurrencySeparator( strtemp2, strtemp1, 3, ',' );
		
		SIZE szSize = m_pConuntFont->GetStringSize(strtemp2);					
		
		// 가운데 정렬
		int nTmpStartX = nStartX + 330	-szSize.cx;
		int nTmpStartY = nStartY + 470;
		
		m_pConuntFont->DrawText(nTmpStartX,nTmpStartY,
												dwFontColor, 
												strtemp2);
		
	}

	if(m_pNumEditBox)
	{
		m_pNumEditBox->Render();
	}
	
}

void CINFItemMixFactoryWnd::RenderRollingImage()
{
	if(!m_pRollingImage)
	{
		return;
	}
//	POINT ptRollingImgSize = m_pRollingImage->GetImgSize();
	RECT rcAniRect = {0,0,1,1};
	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;

	//////////////////// 조합중 이미지 표시 ////////////////////
	if(m_nAniPlayIndex >=0 && m_nAniPlayIndex < MAX_FACTORY_ANI)
	{		
		vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();
		int nCntX = 0;
		for(nCntX=0;nCntX< MAX_INGAME_SEARCH_X;nCntX++)
		{
			if(nCntX >= pVecSource->size())
			{
				break;
			}

			rcAniRect = m_rcAniRect[m_nAniPlayIndex];			

			m_pRollingImage->Move(nStartX + m_rcFactorySourceItem[nCntX].left-9, 
								nStartY + m_rcFactorySourceItem[nCntX].top-9);
			m_pRollingImage->SetRect(rcAniRect.left, 
				rcAniRect.top, 
				rcAniRect.right, 
				rcAniRect.bottom);
			m_pRollingImage->Render();
		}		
	}

	//////////////////// 조합중 이미지 표시 ////////////////////
	if(m_nSuccessIndex >=0 && m_nSuccessIndex < MAX_MIXSUCCESS)
	{			
		m_pSuccessImage[m_nSuccessIndex]->Move(nStartX + m_rcFactoryTargetItem.left-9, 
												nStartY + m_rcFactoryTargetItem.top-9);		
		m_pSuccessImage[m_nSuccessIndex]->Render();
	}

	//////////////////// 실패 이미지 표시 ////////////////////
	if(m_fAllFailElapseTime > 0.0f&& (0 != m_uFinalResultTargetUniqueNum))
	{			
		m_pFailImage->Move(nStartX + m_rcFactoryTargetItem.left, 
												nStartY + m_rcFactoryTargetItem.top);		
		m_pFailImage->Render();
	}

}

void CINFItemMixFactoryWnd::ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *tMixWndPos/*=NULL*/)
{
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	CINFItemMixWnd::ShowWnd(bShow, nShowItemNum, uItemUniNum, tMixWndPos);
	if(bShow)
	{
		ClearListMakingItem();

		// 모든지 초기화
		InitFactoryAniValue(FALSE);
		InitFactorySuccessValue(FALSE);
		InitFactoryFail(FALSE);
		UpdateMakeBtn();
		
	}
	else
	{		
		ClearListMakingItem();		
		pCityLab->InitData();		
	}
	m_pNumEditBox->EnableEdit(FALSE);
}

int CINFItemMixFactoryWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
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
			if(WM_KEYDOWN == uMsg)
			{
				if(m_pNumEditBox->WndProc(uMsg, wParam, lParam))
				{
					CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
					pCityLab->UpdateMixPrice();// 조합가격 표시

					// 갱신
					return INF_MSGPROC_BREAK;
				}

			}
			if(INF_MSGPROC_BREAK == CINFItemMixWnd::WndProc(uMsg, wParam, lParam))
			{
				return INF_MSGPROC_BREAK;
			}

		}
		break;
	}			
	
	return INF_MSGPROC_NORMAL;
}

int CINFItemMixFactoryWnd::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}	
	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	if(INF_MSGPROC_BREAK == CINFItemMixWnd::OnMouseWheel(wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}
	
	return INF_MSGPROC_NORMAL;	
	
}
int CINFItemMixFactoryWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
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
		if(TRUE == m_pFactoryAllBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pUpCntBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pDownCntBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pFactoryMakeBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pFactoryCancelBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pFactoryGetItemBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pFactoryInitBtn->OnLButtonDown(pt))
		{			
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}
	}
	BOOL bOldEditMode = m_pNumEditBox->IsEditMode();
	// 조합중일떄는 클릭을 못한다.
	if((-1 == m_nAniPlayIndex ) && (TRUE == m_pNumEditBox->OnLButtonDown(pt) ))
	{			
// 		if(!bOldEditMode)
// 		{
// 			UpdateMixPrice();// 조합가격 표시
// 		}
		m_pNumEditBox->EnableEdit(TRUE);
		// 버튼위에 마우스가 있다.
		return  INF_MSGPROC_BREAK;				
	}
	m_pNumEditBox->EnableEdit(FALSE);
	
	if(INF_MSGPROC_BREAK == CINFItemMixWnd::OnLButtonDown(wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}
	
	return INF_MSGPROC_NORMAL;		
}
int CINFItemMixFactoryWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if(FALSE == IsShowWnd() )
	{
		return INF_MSGPROC_NORMAL;
	}	

	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
	CheckMouseReverse(&pt);	

	m_pFactoryAllBtn->OnMouseMove(pt);
	m_pUpCntBtn->OnMouseMove(pt);
	m_pDownCntBtn->OnMouseMove(pt);
	m_pFactoryMakeBtn->OnMouseMove(pt);
	m_pFactoryCancelBtn->OnMouseMove(pt);
	m_pFactoryGetItemBtn->OnMouseMove(pt);
	m_pFactoryInitBtn->OnMouseMove(pt);

	int nStartX = m_ptBkPos.x;
	int nStartY = m_ptBkPos.y;

	{
		int nCntX = 0;
		int nFindIdx = -1;
		for(nCntX=0;nCntX< MAX_INGAME_SEARCH_X;nCntX++)
		{		
			if((((nStartY+m_rcFactorySourceItem[nCntX].top) <= pt.y) && (pt.y <= (nStartY+m_rcFactorySourceItem[nCntX].bottom)))
				&& (((nStartX + m_rcFactorySourceItem[nCntX].left) <= pt.x) && (pt.x <= (nStartX + m_rcFactorySourceItem[nCntX].right))))
			{
				nFindIdx = nCntX;
				break;
			}			
		}
		vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();
		if( (0 != m_struSearchSourceItem[COUNT_ITEM_MIXING_SOURCE-1].ItemNum)
			&& (0 <= nFindIdx && nFindIdx < COUNT_ITEM_MIXING_SOURCE))
		{
			int nTmpFindIdx = COUNT_ITEM_MIXING_SOURCE-(nFindIdx+1);
			nTmpFindIdx = max(0, nTmpFindIdx);

			INT nItemNum = m_struSearchSourceItem[nTmpFindIdx].ItemNum;
			SetTooltipItemInfo(nItemNum, pt.x, pt.y, 0);
			return INF_MSGPROC_BREAK;
		}
		else if(0 <= nFindIdx && nFindIdx < pVecSource->size())
		{
			INT nItemNum = (*pVecSource)[nFindIdx]->ItemNum;
			SetTooltipItemInfo(nItemNum, pt.x, pt.y, 0);
			return INF_MSGPROC_BREAK;
		}
	}

	{		
		if((((nStartY+m_rcFactoryTargetItem.top) <= pt.y) && (pt.y <= (nStartY+m_rcFactoryTargetItem.bottom)))
			&& (((nStartX + m_rcFactoryTargetItem.left) <= pt.x) && (pt.x <= (nStartX + m_rcFactoryTargetItem.right))))
		{
			INT nItemNum = 0;
			switch(m_nMixMode)
			{
			case MODE_INVEN_ITEM:
				{
					CItemInfo*	pTargetItem	= pCityLab->FindItemFromTarget( m_uFinalResultTargetUniqueNum);
					if(pTargetItem)
					{
						SetTooltipItemInfo(pTargetItem->ItemNum, pt.x, pt.y, pTargetItem->ShapeItemNum);
						return INF_MSGPROC_BREAK;
					}
					
					// 2013-06-17 by bhsohn 아이템 조합 미리 보기 툴팁 나오는 현상 처리
					nItemNum = m_struWillResultTargetMixInfo.TargetItemNum;				
					if(nItemNum > 0)
					{
						SetTooltipItemInfo(nItemNum, pt.x, pt.y, 0);
						return INF_MSGPROC_BREAK;
					}
					// END 2013-06-17 by bhsohn 아이템 조합 미리 보기 툴팁 나오는 현상 처리
				}
				break;
			case MODE_SEARCH_SELECT:
				{
					nItemNum = m_struWillResultTargetMixInfo.TargetItemNum;				
					if(nItemNum > 0)
					{
						SetTooltipItemInfo(nItemNum, pt.x, pt.y, 0);
						return INF_MSGPROC_BREAK;
					}
				}
				break;
			}
		}			
	}

	if(INF_MSGPROC_BREAK == CINFItemMixWnd::OnMouseMove(wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}

	if(pt.x > m_ptBkPos.x &&
		pt.x < m_ptBkPos.x + INF_FACTORY_MIX_WINDOWS_WIDTH &&
		pt.y > m_ptBkPos.y &&
		pt.y < m_ptBkPos.y + INF_FACTORY_MIX_WINDOWS_HEIGTH)
	{
		return INF_MSGPROC_BREAK;
	}
	
	
	return INF_MSGPROC_NORMAL;	
	
}

int CINFItemMixFactoryWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
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
		if(TRUE == m_pFactoryAllBtn->OnLButtonUp(pt))
		{				
			OnClickAllMake();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pUpCntBtn->OnLButtonUp(pt))
		{				
			OnClickCntBtn(TRUE);
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pDownCntBtn->OnLButtonUp(pt))
		{				
			OnClickCntBtn(FALSE);
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pFactoryMakeBtn->OnLButtonUp(pt))
		{				
			OnClickFactoryMake();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pFactoryCancelBtn->OnLButtonUp(pt))
		{				
			OnClickCancelMake();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pFactoryGetItemBtn->OnLButtonUp(pt))
		{				
			OnClickInitMake();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pFactoryInitBtn->OnLButtonUp(pt))
		{				
			OnClickInitMake();
			// 버튼 클릭 
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		int nStartX = m_ptBkPos.x;
		int nStartY = m_ptBkPos.y;		

		vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();

		if( pt.x > nStartX+m_rcFactorySourceItem[0].left &&
			pt.x < nStartX+m_rcFactorySourceItem[MAX_INGAME_SEARCH_X-1].right &&
			pt.y > nStartY+m_rcFactorySourceItem[0].top &&
			pt.y < nStartY+m_rcFactorySourceItem[MAX_INGAME_SEARCH_X-1].bottom &&
			g_pGameMain->m_stSelectItem.pSelectItem &&
			g_pGameMain->m_stSelectItem.bySelectType == ITEM_INVEN_POS &&
			pVecSource->size() < COUNT_ITEM_MIXING_SOURCE )
		{
			if(g_pGameMain->m_stSelectItem.pSelectItem->pItem)
			{
				UID64_t uItemUniNum = g_pGameMain->m_stSelectItem.pSelectItem->pItem->UniqueNumber;
				CItemInfo* pItemInfo = g_pStoreData->FindItemInInventoryByUniqueNumber(uItemUniNum);
				if(pItemInfo)
				{
					((CINFCityLab*)m_pParent)->UpLoadItem(pItemInfo); // 아이템 드래그 
				}
				return INF_MSGPROC_BREAK;
			}
		}
	}

	if(INF_MSGPROC_BREAK == CINFItemMixWnd::OnLButtonUp(wParam, lParam))
	{
		return INF_MSGPROC_BREAK;
	}

	return INF_MSGPROC_NORMAL;		
}


void CINFItemMixFactoryWnd::UpdateBtnPos()
{
	CINFItemMixWnd::UpdateBtnPos();

	int nWindowPosX = m_ptBkPos.x;
	int nWindowPosY = m_ptBkPos.y;		

	m_pFactoryAllBtn->SetBtnPosition(nWindowPosX+391, nWindowPosY+465);
	
	m_pUpCntBtn->SetBtnPosition(nWindowPosX+379, nWindowPosY+468);
	m_pDownCntBtn->SetBtnPosition(nWindowPosX+379, nWindowPosY+483);

// 	m_pFactoryMakeBtn->SetBtnPosition(nWindowPosX+430, nWindowPosY+516);
// 	m_pFactoryCancelBtn->SetBtnPosition(nWindowPosX+478, nWindowPosY+516);	
	m_pFactoryMakeBtn->SetBtnPosition( nWindowPosX + 430 + m_pFactoryMakeGroup->GetFindControlTargetofMinPos("oks00").x, nWindowPosY+516);
	m_pFactoryCancelBtn->SetBtnPosition( nWindowPosX + 430 + m_pFactoryMakeGroup->GetFindControlTargetofMinPos("cans00").x , nWindowPosY+516);
	m_pFactoryGetItemBtn->SetBtnPosition( nWindowPosX + 430 + m_pFactoryMakeGroup->GetFindControlTargetofMinPos("cans00").x , nWindowPosY+516);
	m_pFactoryInitBtn->SetBtnPosition( nWindowPosX + 430 + m_pFactoryMakeGroup->GetFindControlTargetofMinPos("cans00").x , nWindowPosY+516);

	m_pFactoryMakeBtn->SetBtnPosition(nWindowPosX+430, nWindowPosY+520);
	m_pFactoryGetItemBtn->SetBtnPosition(nWindowPosX+478, nWindowPosY+520);	
	m_pFactoryCancelBtn->SetBtnPosition(nWindowPosX+478, nWindowPosY+520);	
	m_pFactoryInitBtn->SetBtnPosition(nWindowPosX+478, nWindowPosY+520);
	

	m_pNumEditBox->SetPos(nWindowPosX+302, nWindowPosY+470);
	m_pCloseBtn->ShowWindow(FALSE);

}

void CINFItemMixFactoryWnd::OnClickFactoryMake()
{
	switch(m_nMixMode)
	{
	case MODE_INVEN_ITEM:
		{
			vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();
			if(pVecSource->size() <= 0)
			{
				// 아무것도 없다.
				return;
			}
		}
		break;
	case MODE_SEARCH_SELECT:
		{
			if(0 == m_struWillResultTargetMixInfo.TargetItemNum)
			{
				return;
			}
		}
		break;
	default:
		{
			return;
		}
		break;
	}
	if(IsPossbileSearchMixItem())
	{		
		// 제조 아이템 정보 관련 전체 갱신
		ReadyMakeItemInfo(); 
		
		StartListMakingItem();		// List에 있는것을 보낸다.		
	}
	
}
// 전체 제조 버튼 클릭
void CINFItemMixFactoryWnd::OnClickAllMake()
{
	if(MODE_SEARCH_SELECT != m_nMixMode)	
	{
		// 아이템 검색모드일떄만 처리
		return;
	}
	int nCnt = 0;	
	int nCanMakeItemCnt	= COUNT_MAX_MIXING_COUNT+1;
	for(nCnt = 0; nCnt< COUNT_ITEM_MIXING_SOURCE; nCnt++)
	{
		if(0 == m_struSearchSourceItem[nCnt].ItemNum)
		{				
			continue;
		}
// 		CItemInfo* pInvenItem = g_pStoreData->FindItemInInventoryByItemNum(m_struSearchSourceItem[nCnt].ItemNum);
// 		if(!pInvenItem)
// 		{
// 			// 아이템이 없다.
// 			nCanMakeItemCnt = 0;
// 			break;
// 		}		
// 		int nInvenItemCnt	= pInvenItem->CurrentCount;
// 		int nSourceItemCnt	= max(1, m_struSearchSourceItem[nCnt].Count);
// 		int nTmpMakeItemCnt	= pInvenItem->CurrentCount/nSourceItemCnt; // 실제 만들수 있는 아이템 갯수
//		int nInvenItemCnt	= g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt( m_struSearchSourceItem[nCnt].ItemNum);
		int nInvenItemCnt	= GetCntInven_ItemNum_To_AllItemCnt( m_struSearchSourceItem[nCnt].ItemNum);
		int nSourceItemCnt	= max(1, m_struSearchSourceItem[nCnt].Count);
		int nTmpMakeItemCnt	= nInvenItemCnt/nSourceItemCnt; // 실제 만들수 있는 아이템 갯수

		if(nTmpMakeItemCnt < nCanMakeItemCnt)
		{
			nCanMakeItemCnt = nTmpMakeItemCnt;
		}
	}
	if(nCanMakeItemCnt <= COUNT_MAX_MIXING_COUNT && nCanMakeItemCnt>0)
	{
		char chTmp[32]={0,};
		sprintf(chTmp, "%d", nCanMakeItemCnt);
		
		m_pNumEditBox->SetString(chTmp, 32);		
	}
	
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	pCityLab->UpdateMixPrice();// 조합가격 표시
	
}

// 제조 취소 클릭
void CINFItemMixFactoryWnd::OnClickCancelMake()
{
	InitFactoryAniValue(FALSE);
	if(MODE_SEARCH_SELECT == m_nMixMode)	
	{
		OnClickInitMake();
	}
	UpdateMakeBtn();
	
}

// 초기좌 버튼
void CINFItemMixFactoryWnd::OnClickInitMake()
{	
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	pCityLab->OnButtonClicked(1);

	ClearListMakingItem();
	UpdateMakeBtn();
	
}


void CINFItemMixFactoryWnd::OnClickCntBtn(BOOL bUpBtn)
{
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	if(!m_pNumEditBox)
	{
		return;
	}
	char chBuff[256] = {0,};
	m_pNumEditBox->GetString(chBuff, 256);
	INT nMixCounts = atoi(chBuff);
	if(nMixCounts <= 0)
	{
		return;
	}
	else if(nMixCounts > COUNT_MAX_MIXING_COUNT)
	{
		nMixCounts = COUNT_MAX_MIXING_COUNT;
	}
	
	if(bUpBtn)
	{
		nMixCounts++;
		nMixCounts = min(nMixCounts, COUNT_MAX_MIXING_COUNT);
	}
	else
	{
		nMixCounts--;
		nMixCounts = max(nMixCounts, 1);
	}	
	sprintf(chBuff, "%d", nMixCounts);
	
	m_pNumEditBox->SetString(chBuff, 32);
	pCityLab->UpdateMixPrice();// 조합가격 표시
}



INT CINFItemMixFactoryWnd::GetMakeNumCnt()
{
	char chBuff[32] = {0,};	
	m_pNumEditBox->GetString(chBuff, 32);
	INT nMixCounts = atoi(chBuff);
	return nMixCounts;
}

// 아이템 갯수를 얻어오자
INT CINFItemMixFactoryWnd::GetSendMakeNumCnt()
{
	return m_nTargetMakingCount;
	
}

void CINFItemMixFactoryWnd::SetMakeNumCnt(INT nCnt)
{
	char chBuff[32] ={0,};
	wsprintf(chBuff, "%d", nCnt);	
	m_pNumEditBox->SetString(chBuff, 32);

	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	pCityLab->UpdateMixPrice();// 조합가격 표시
	
}

void CINFItemMixFactoryWnd::InitFactoryAniValue(BOOL bPlay)
{
	m_nAniPlayIndex = -1;
	m_fAniElapseTime = 0.0f;
	m_fAllAniElapseTime = -1.0f;
	if(bPlay)
	{
		// 제조 애니메이션 시작	
		m_nAniPlayIndex = 0;
		m_fAniElapseTime = 0.0f;
		m_fAllAniElapseTime = FULL_TIME_ANI_PLAYINGTIME;	
	}		
}

void CINFItemMixFactoryWnd::UpdateMakeBtn()
{
	// 툴팁을 숨기자
	if(g_pInterface->m_pToolTip)
	{
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	}

	vector<CItemInfo*>  *pVecTarget = ((CINFCityLab*)m_pParent)->GetvecTarget();
	vector<CItemInfo*>::iterator itTarget = pVecTarget->begin();
	if(m_nAniPlayIndex < 0)
	{
		// 조합중이 아니다.
		if(!m_listMakingItem.empty() )
		{
			// 현재 다중 조합중이면 취소 버튼만 활성화
			// 조합
			m_pFactoryMakeBtn->ShowWindow(TRUE);			
			m_pFactoryMakeBtn->EnableBtn(FALSE);			
			// 취소
			m_pFactoryCancelBtn->ShowWindow(TRUE);						
			// 아이템 받기
			m_pFactoryGetItemBtn->ShowWindow(FALSE); 
			// 비우기
			m_pFactoryInitBtn->ShowWindow(FALSE); 
			
		}
		else if(pVecTarget->size() > 0 )
		{			
			// 결과물이 있으면 강제로 초기화 버튼만 활성화
			// 조합
			m_pFactoryMakeBtn->ShowWindow(FALSE);
			m_pFactoryMakeBtn->EnableBtn(FALSE);			
			// 취소
			m_pFactoryCancelBtn->ShowWindow(FALSE);
			// 아이템 받기
			m_pFactoryGetItemBtn->ShowWindow(TRUE);			
			// 비우기
			m_pFactoryInitBtn->ShowWindow(FALSE); 
		}
		else
		{			
			// 결과물이 없다.
			m_pFactoryMakeBtn->ShowWindow(TRUE);
			m_pFactoryMakeBtn->EnableBtn(TRUE);	
			
			m_pFactoryCancelBtn->ShowWindow(FALSE);
			m_pFactoryGetItemBtn->ShowWindow(FALSE);

			// 비우기
			m_pFactoryInitBtn->ShowWindow(TRUE); 
		}
	}
	else if(!m_listMakingItem.empty())
	{
		// 현재 다중 조합중이면 취소 버튼만 활성화
		m_pFactoryMakeBtn->ShowWindow(TRUE);
		m_pFactoryMakeBtn->EnableBtn(FALSE);			
		m_pFactoryCancelBtn->ShowWindow(TRUE);
		
		m_pFactoryGetItemBtn->ShowWindow(FALSE);		
		// 비우기
		m_pFactoryInitBtn->ShowWindow(FALSE); 
	}
	else
	{			
		// 일반상태
		m_pFactoryMakeBtn->ShowWindow(TRUE);
		m_pFactoryMakeBtn->EnableBtn(FALSE);				
		m_pFactoryCancelBtn->ShowWindow(TRUE);

		m_pFactoryGetItemBtn->ShowWindow(FALSE);		
		// 비우기
		m_pFactoryInitBtn->ShowWindow(TRUE); 
		
	}

}

void CINFItemMixFactoryWnd::InitFactorySuccessValue(BOOL bPlay)
{
	m_nSuccessIndex = -1;
	m_fSuccessElapseTime = 0.0f;
	m_fAllSuccessElapseTime = -1.0f;
	if(bPlay)
	{
		// 제조 애니메이션 시작	
		m_nSuccessIndex = 0;
		m_fSuccessElapseTime = 0.0f;
		m_fAllSuccessElapseTime = FULL_TIME_ANI_PLAYINGTIME;	
	}
}

void CINFItemMixFactoryWnd::InitFactoryFail(BOOL bPlay)
{	
	m_fAllFailElapseTime = -1.0f;
	if(bPlay)
	{
		// 실패 
		m_fAllFailElapseTime = FULL_TIME_ANI_PLAYINGTIME;	
	}
}

void CINFItemMixFactoryWnd::SetTargetMixInfo(ITEM_MIXING_INFO* pTargetMixInfo)
{
	if(NULL == pTargetMixInfo)
	{
		memset(&m_struWillResultTargetMixInfo, 0x00, sizeof(ITEM_MIXING_INFO));		
		return;
	}
	memcpy(&m_struWillResultTargetMixInfo, pTargetMixInfo, sizeof(ITEM_MIXING_INFO));	
}

void CINFItemMixFactoryWnd::InitData(BOOL bItemsResult)
{
	m_uFinalResultTargetUniqueNum = 0;
	ClearListMakingItem();
	
	// 모든지 초기화
	InitFactoryAniValue(FALSE);
	InitFactorySuccessValue(FALSE);
	InitFactoryFail(FALSE);
	
	if(!bItemsResult)
	{
		// 취소등으로 인한 초기화
		InitSearchItemSourceItem();
		SetTargetMixInfo(NULL);
	}
	else
	{
		// 조합 실패로 인한 초기화
	}
	UpdateMakeBtn();
}

void CINFItemMixFactoryWnd::ItemMixItemsResult(MSG_FC_ITEM_MIX_ITEMS_RESULT* pMsg)
{
	// 	if( pMsg->Err != ERR_NO_ERROR )
	// 	{
	// 		CINFCityLab * pCityLab = (CINFCityLab*)m_pInterface->m_pCityBase->FindBuildingShop(BUILDINGKIND_FACTORY);
	// 		if(pCityLab)
	// 		{
	// 			pCityLab->InitData();
	// 		}
// 		g_pGameMain->m_pInfWindow->AddMsgBox( STRERR_C_MIX_0002, _MESSAGE );
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);

	switch(pMsg->Err)
	{
	case ERR_NO_ERROR:
		{
			// 아이템 제조 성공
			InitFactorySuccessValue(TRUE); 
			InitFactoryFail(FALSE);

			UpdateMixTarget();
			// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
//			pCityLab->InitData(TRUE); // 초기화
		}
		break;
	default:
		{
			// 아이템 제조 실패
			// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
//			pCityLab->InitData(TRUE); // 초기화

			InitFactorySuccessValue(FALSE); // 아이템 제조 실패
			InitFactoryFail(TRUE);

		}
		break;
	}	

	if(m_listMakingItem.empty())	
	{
		// 더보낼것이 없다.
		InitFactoryAniValue(FALSE); // 애니메이션 초기화
	}
	else
	{
		StartListMakingItem(); // List에 있는것을 보낸다.		
	}
}

void CINFItemMixFactoryWnd::ErrorMixItem(Err_t ErrorCode)
{
	MSG_FC_ITEM_MIX_ITEMS_RESULT struMsg;
	struMsg.Err = ErrorCode;
	
	ItemMixItemsResult(&struMsg); // 아이템 오류 처리

}
// 아이템 조합 완료값 갱신
void CINFItemMixFactoryWnd::UpdateMixTarget()
{
	// 2013-04-08 by bhsohn 인게임 조합창 추가 구현
// 	vector<CItemInfo*>  *pVecTarget = ((CINFCityLab*)m_pParent)->GetvecTarget();
// 	vector<CItemInfo*>::iterator itTarget = pVecTarget->begin();
// 
// 	m_vecTargetItem.clear(); // 초기화
// 
// 	while(itTarget != pVecTarget->end())
// 	{
// 		CItemInfo* pTargetItemInfo = (*itTarget);
// 		if(!pTargetItemInfo)
// 		{
// 			itTarget++;
// 			continue;				
// 		}
// 		ITEM_GENERAL itemGeneral;
// 		memset(&itemGeneral, 0x00, sizeof(ITEM_GENERAL));
// 
// 		// 아이템 정보를 얻어오자
// 		pTargetItemInfo->GetItemGeneral(&itemGeneral);
// 
// 		m_vecTargetItem.push_back(itemGeneral);
// 
// 		itTarget++;
// 	}
}

// 검색항목 클릭
int CINFItemMixFactoryWnd::OnCilckSelectItem(POINT pt)		
{
	int nRtn = CINFItemMixWnd::OnCilckSelectItem(pt);
	if(INF_MSGPROC_BREAK == nRtn)		
	{
		((CINFCityLab*)m_pParent)->InitData(); // 초기화

		int nPage = m_nCurrentPage;
		int nStartPos = 0;
		if(nPage <= m_nMaxScrollPage )
		{
			nStartPos = (max(nPage-1, 0)*MAX_SCROLL_CNT);
		}

		if(m_nSelItem != -1)
		{
			nStartPos += m_nSelItem;
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
		if(it != m_vecRenderItemList.end())
		{
			ITEM_MIXING_INFO struItemInfo = (*it).struMixingInfo;
			SearchItemToSourceItem(struItemInfo.SourceItem);
			SetTargetMixInfo(&struItemInfo);
			{
				// 아이템 갯수 갱신
				char chTmp[32]={0,};
				sprintf(chTmp, "1");								
				m_pNumEditBox->SetString(chTmp, 32);				
				CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
				pCityLab->UpdateMixPrice();// 조합가격 표시
			}
		}
		
	}
	return nRtn;
}

void CINFItemMixFactoryWnd::SearchItemToSourceItem(ITEM_W_COUNT SourceItem[COUNT_ITEM_MIXING_SOURCE])
{
	m_nMixMode = MODE_SEARCH_SELECT;
//	memcpy(m_struSearchSourceItem, SourceItem, sizeof(ITEM_W_COUNT)* COUNT_ITEM_MIXING_SOURCE);	
	memset(m_struSearchSourceItem, 0x00, sizeof(ITEM_W_COUNT)* COUNT_ITEM_MIXING_SOURCE);

	int nCnt = 0;
	int nMemPos = (COUNT_ITEM_MIXING_SOURCE-1);
	for(nCnt = (COUNT_ITEM_MIXING_SOURCE-1); nCnt >= 0 ;nCnt--)
	{
		if(0 != SourceItem[nCnt].ItemNum)
		{
			m_struSearchSourceItem[nMemPos] = SourceItem[nCnt];
			nMemPos--;
			nMemPos = max(nMemPos, 0);
		}
	}
	UpdateMixPrice();
}

void CINFItemMixFactoryWnd::InitSearchItemSourceItem()
{
	m_nMixMode = MODE_INVEN_ITEM;
	memset(m_struSearchSourceItem, 0x00, sizeof(ITEM_W_COUNT)* COUNT_ITEM_MIXING_SOURCE);
}

// 제조 변수 초기화
void CINFItemMixFactoryWnd::ReadyMakeItemInfo()
{	
	ClearListMakingItem();
	switch(m_nMixMode)
	{
	case MODE_INVEN_ITEM:
		{
			MakeItem_InvenInputMode();
		}
		break;
	case MODE_SEARCH_SELECT:
		{
			MakeItem_SearchSelectMode(); // 아이템 선택모드에서 만들기		
		}
		break;
	}	
}

// 아이템 제조 시작
void CINFItemMixFactoryWnd::SendMakeItem()
{
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);		
	vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();	
	if(pVecSource->size() > 0)
	{
		// 재료가 있다.		
		pCityLab->OnButtonClicked(0);
		InitFactoryAniValue(FALSE); // 초기화
		UpdateMakeBtn();
	}
}
void CINFItemMixFactoryWnd::MakeItem_InvenInputMode()
{
	int nItemCount = 0;
	char chBuff[32] = {0,};
	m_pNumEditBox->GetString(chBuff, 32);
	nItemCount = atoi(chBuff);
	
	m_nTargetMakingCount = nItemCount;

}

void CINFItemMixFactoryWnd::MakeItem_SearchSelectMode()
{	
	int nItemCount = 0;
	char chBuff[32] = {0,};
	m_pNumEditBox->GetString(chBuff, 32);
	nItemCount = atoi(chBuff);

	m_nTargetMakingCount = nItemCount;

	BOOL bMakeCountableItemMode = TRUE; // 재료 만들기 모드
	int nCnt =0;
	for(nCnt = 0; nCnt< COUNT_ITEM_MIXING_SOURCE; nCnt++)
	{
		if(0 == m_struSearchSourceItem[nCnt].ItemNum)
		{				
			continue;
		}
//		CItemInfo* pInvenItem = g_pStoreData->FindItemInInventoryByItemNum(m_struSearchSourceItem[nCnt].ItemNum);
		CItemInfo* pInvenItem = FindItemInInventoryByItemNum_MixPossible(m_struSearchSourceItem[nCnt].ItemNum);
		if(!pInvenItem)
		{
			// 아이템이 없다.
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080318_0200,COLOR_ERROR);// "수량이 맞지 않아 아이템을 제조할 수 없습니다."		
			return;
		}
//		int nInvenItemCnt	= g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt( m_struSearchSourceItem[nCnt].ItemNum);
		int nInvenItemCnt	= GetCntInven_ItemNum_To_AllItemCnt( m_struSearchSourceItem[nCnt].ItemNum);
		int nSourceItemCnt	= m_struSearchSourceItem[nCnt].Count * nItemCount;
		if( nSourceItemCnt > 0 && (nInvenItemCnt >= nSourceItemCnt))
		{
		}
		else
		{
			// 아이템이 모자르는 상황
			return;
		}
		if( !IS_COUNTABLE_ITEM(pInvenItem->Kind) )
		{
			bMakeCountableItemMode = FALSE; // Non카운터블 아이템이 있다.			
		}		
	}
	if(bMakeCountableItemMode)
	{
		MakeItem_SearchSelectMode_CountItem(nItemCount);	
	}
	else
	{
		// NonCoutable Item 만들기 모드
		MakeItem_SearchSelectMode_NonCountItem(nItemCount);	
	}	
}
// 카운터블 아이템 조합 모드
void CINFItemMixFactoryWnd::MakeItem_SearchSelectMode_CountItem(int nItemCount)
{		
	struMakingItemInfo struMakingInfo;
	memset(&struMakingInfo, 0x00, sizeof(struMakingItemInfo));
	memcpy(struMakingInfo.struSourceItem, m_struSearchSourceItem, sizeof(ITEM_W_COUNT)*COUNT_ITEM_MIXING_SOURCE);
	struMakingInfo.nItemCount = nItemCount; // 여러개 생산을 한번에 보내자

	// 패킷보낼때 보낼갯수
	m_nTargetMakingCount = struMakingInfo.nItemCount;	
	
	m_listMakingItem.push_back(struMakingInfo);
}


void CINFItemMixFactoryWnd::MakeItem_SearchSelectMode_NonCountItem(int nItemCount)
{	
	m_struWillResultTargetMixInfo.NumOfSourceItems = nItemCount;

	struMakingItemInfo struMakingInfo;
	memset(&struMakingInfo, 0x00, sizeof(struMakingItemInfo));
	memcpy(struMakingInfo.struSourceItem, m_struSearchSourceItem, sizeof(ITEM_W_COUNT)*COUNT_ITEM_MIXING_SOURCE);
	struMakingInfo.nItemCount = 1; // 한개 씩 여러분 보내자
	// 1개씩 보내자
	m_nTargetMakingCount = 1;
	

	int nItemCountCnt = 0;
	for(nItemCountCnt = 0; nItemCountCnt< nItemCount; nItemCountCnt++)
	{	
		m_listMakingItem.push_back(struMakingInfo);
	}	
}
void CINFItemMixFactoryWnd::InvenToSourceItem(ITEM_W_COUNT struSearchSourceItem[COUNT_ITEM_MIXING_SOURCE], int nItemCount)
{
	int nItemLen =0;	
	vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	
	int nCnt = 0;
	for(nCnt = 0; nCnt< COUNT_ITEM_MIXING_SOURCE; nCnt++)
	{
		if(0 == struSearchSourceItem[nCnt].ItemNum)
		{				
			continue;
		}
//		CItemInfo* pInvenItem = g_pStoreData->FindItemInInventoryByItemNum(struSearchSourceItem[nCnt].ItemNum);
		CItemInfo* pInvenItem = FindItemInInventoryByItemNum_MixPossible(struSearchSourceItem[nCnt].ItemNum);
		if(!pInvenItem)
		{
			// 아이템이 없다.
			return;
		}
//		int nInvenItemCnt	= g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt( struSearchSourceItem[nCnt].ItemNum);
		int nInvenItemCnt	= GetCntInven_ItemNum_To_AllItemCnt( struSearchSourceItem[nCnt].ItemNum);
		int nSourceItemCnt	= struSearchSourceItem[nCnt].Count * nItemCount;
		if( nSourceItemCnt > 0 && (nInvenItemCnt >= nSourceItemCnt))
		{
			pCityLab->InvenToSourceItem(pInvenItem, nSourceItemCnt);					
		}		
		else
		{
			// 아이템이 모자르는 상황
			return;
		}
		nItemLen++;
	}
}
BOOL CINFItemMixFactoryWnd::StartListMakingItem()
{
	if(MODE_SEARCH_SELECT != m_nMixMode)	
	{
		InitFactoryAniValue(TRUE); // 제조 애니메이션 시작			
		UpdateMakeBtn();
		return TRUE;
	}
	if(m_listMakingItem.empty())
	{
		return FALSE;
	}
	struMakingItemInfo struItemInfo = m_listMakingItem.front();
	m_listMakingItem.pop_front();
	
	InvenToSourceItem(struItemInfo.struSourceItem, struItemInfo.nItemCount); 

	InitFactoryAniValue(TRUE); // 제조 애니메이션 시작			
	UpdateMakeBtn();

	return TRUE;

}
void CINFItemMixFactoryWnd::ClearListMakingItem()
{
	m_uFinalResultTargetUniqueNum = 0; // 결과적인 아이템수 
	m_listMakingItem.clear();
	m_nTargetMakingCount = 1;
}

BOOL CINFItemMixFactoryWnd::IsPossbileSearchMixItem()
{	
	switch(m_nMixMode)
	{
	case MODE_INVEN_ITEM:
		{
		}
		break;
	case MODE_SEARCH_SELECT:
		{
			int nItemCount = 0;
			char chBuff[32] = {0,};
			m_pNumEditBox->GetString(chBuff, 32);
			nItemCount = atoi(chBuff);
			
			int nCnt = 0;
			int nItemLen =0;
			for(nCnt = 0; nCnt< COUNT_ITEM_MIXING_SOURCE; nCnt++)
			{
				if(0 == m_struSearchSourceItem[nCnt].ItemNum)
				{				
					continue;
				}
				// 		CItemInfo* pInvenItem = g_pStoreData->FindItemInInventoryByItemNum(m_struSearchSourceItem[nCnt].ItemNum);
				// 		if(!pInvenItem)
				// 		{
				// 			// 아이템이 없다.
				// 			g_pD3dApp->m_pChat->CreateChatChild(STRERR_ERROR_0106,COLOR_ERROR);// "조합을 할 수 없습니다."
				// 			return FALSE;
				// 		}
				// 		int nInvenItemCnt	= pInvenItem->CurrentCount;
				
				//		int nInvenItemCnt	= g_pStoreData->GetCntInven_ItemNum_To_AllItemCnt( m_struSearchSourceItem[nCnt].ItemNum);
				int nInvenItemCnt	= GetCntInven_ItemNum_To_AllItemCnt( m_struSearchSourceItem[nCnt].ItemNum);
				int nSourceItemCnt	= m_struSearchSourceItem[nCnt].Count * nItemCount;
				if( nSourceItemCnt > 0 && (nInvenItemCnt >= nSourceItemCnt))
				{			
				}		
				else
				{
					// 아이템이 모자르는 상황
					g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080318_0200,COLOR_ERROR);// "수량이 맞지 않아 아이템을 제조할 수 없습니다."		
					return FALSE;
				}
				nItemLen++;
			}		
		}
		break;

	}

#ifdef C_WARNING_APPEARANCE_CHANGE
	{
		CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
		
		if ( FALSE == pCityLab->IsIsAppearanceChangeCheck() && pCityLab->IsWarning_AppearanceChange())
		{
			g_pGameMain->m_pInfWindow->AddMsgBox ( STRMSG_C_121030_0001, _Q_APPEARANCE_CHANGE_PREVENTION );	
			return FALSE;
		}		
	}
#endif
	return TRUE;	
}

void CINFItemMixFactoryWnd::UpdateMixPrice()
{
	CINFCityLab* pCityLab = ((CINFCityLab*)m_pParent);
	INT nMixCounts = GetMakeNumCnt();
	
	if(nMixCounts <= 0)
	{
		return;
	}
	else if(nMixCounts > COUNT_MAX_MIXING_COUNT)
	{
		nMixCounts = COUNT_MAX_MIXING_COUNT;
	}	
		
	ITEM_MIXING_INFO	stMixInfo;
	memset(&stMixInfo, 0x00, sizeof(ITEM_MIXING_INFO));

	vector<CItemInfo*>  *pVecSource = ((CINFCityLab*)m_pParent)->GetvecSource();
	vector<CItemInfo*>::iterator it = pVecSource->begin();

	int nCnt =0;
	int nItemCnt =0;

	switch(m_nMixMode)
	{
	case MODE_INVEN_ITEM:		// 인벤에서 아이템 바로 선택
		{			
			while(it != pVecSource->end())
			{
				if(nCnt >= COUNT_ITEM_MIXING_SOURCE)
				{
					break;
				}
				CItemInfo* pItem = (*it);
				// 아이템 번호
				stMixInfo.SourceItem[nCnt].ItemNum = pItem->ItemNum;
				// 갯수
				if(IS_COUNTABLE_ITEM(pItem->Kind))
				{					
					stMixInfo.SourceItem[nCnt].Count = pItem->CurrentCount/nMixCounts;			
				}
				else
				{
					stMixInfo.SourceItem[nCnt].Count = 1;			
				}
				nCnt++;
				nItemCnt++;
				it++;
			}
		}
		break;
	case MODE_SEARCH_SELECT:	// 검색으로 선택
		{
			for(nCnt =0;nCnt < COUNT_ITEM_MIXING_SOURCE;nCnt++)
			{
				if(0 != m_struSearchSourceItem[nCnt].ItemNum)
				{
					nItemCnt++;
				}
				stMixInfo.SourceItem[nCnt] = m_struSearchSourceItem[nCnt];
			}		
		}
		break;
	default:
		{
			return;
		}
		break;
	}		
	
	stMixInfo.NumOfSourceItems = nItemCnt;
	
	ITEM_MIXING_INFO* pMixInfo = g_pDatabase->GetMixerPrice(&stMixInfo);
	if(pMixInfo)
	{		
		int nAllPrice = pMixInfo->MixingCost*nMixCounts;
		if(nAllPrice >= 0)
		{
			pCityLab->SetPrice(nAllPrice);
		}
	}

	SetTargetMixInfo(pMixInfo);
	m_uFinalResultTargetUniqueNum = 0; // 초기화

}

void CINFItemMixFactoryWnd::SetFinalResultTargetUniqueNum(UID64_t	uFinalResultTargetUniqueNum)
{
	m_uFinalResultTargetUniqueNum = uFinalResultTargetUniqueNum;
}

// Item번호로 적합한 아이템을 찾는다.
CItemInfo* CINFItemMixFactoryWnd::FindItemInInventoryByItemNum_MixPossible(INT nItemNum)
{
	CItemInfo* pInvenItem = g_pStoreData->FindItemInInventoryByItemNum( nItemNum);
	if(!pInvenItem)
	{
		return pInvenItem;
	}
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
				return NULL;
			}
			vector<CItemInfo*>::iterator it = vecItemList.begin();
			while(it != vecItemList.end())
			{
				pInvenItem = (*it);
				if(ITEMKIND_ACCESSORY_TIMELIMIT == pInvenItem->Kind && (pInvenItem->GetItemPassTime() > 0))
				{
					// 기간제 악세사리는 사용한적이 있으면 넣으면 안된다.			
				}
				else
				{
					return pInvenItem;
				}
				it++;
			}
			// 시간제 악세사리중 적당한 것을 찾지 못했다.
			g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_130222_0007,COLOR_ERROR);// "기간제 아이템중 사용한적이 있는 아이템이 있습니다."	
			return NULL;
		}
		break;
	}	
	return pInvenItem;

}

INT CINFItemMixFactoryWnd::GetTargetVecItemCount(INT nItemNum)
{
	INT nItemCnt = 0;
	vector<CItemInfo*>  *pVecTarget = ((CINFCityLab*)m_pParent)->GetvecTarget();		
	vector<CItemInfo*>::iterator itTarget = pVecTarget->begin();
	while(itTarget != pVecTarget->end())
	{
		CItemInfo* pItemInfo = *itTarget;
		if(nItemNum == pItemInfo->ItemNum)
		{
			if(IS_COUNTABLE_ITEM(pItemInfo->Kind))
			{
				nItemCnt += pItemInfo->CurrentCount;
			}
			else
			{
				nItemCnt += 1;
			}
		}
		itTarget++;
	}
	return nItemCnt;
	
}