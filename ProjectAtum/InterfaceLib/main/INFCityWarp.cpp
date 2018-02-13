// INFCityWarp.cpp: implementation of the CINFCityWarp class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFCityWarp.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "FieldWinSocket.h"
#include "INFCityBase.h"
#include "INFScrollBar.h"
#include "Interface.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "Cinema.h"
#include "AtumSound.h"
#include "INFGameMain.h"
#include "Chat.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "StoreData.h"
#include "dxutil.h"
#include "QuestData.h"
#include "AtumDatabase.h"				// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ
#include "INFImageEx.h"	// 2011. 11. 17 by jskim CINFImage -> CINFImageEx º¯°æ
#include "INFGroupManager.h"
#include "INFGroupImage.h"
#include "INFToolTip.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	#define CITY_WARP_START_X					CITY_BASE_NPC_BOX_START_X
	#define CITY_WARP_START_Y					(CITY_BASE_NPC_BOX_START_Y-CITY_BASE_WARP_SIZE_Y)

	#define CITY_WARP_BACK_START_X				(CITY_WARP_START_X)
	#define CITY_WARP_BACK_START_Y				(CITY_WARP_START_Y)
	#define CITY_WARP_TITLE_START_X				(CITY_WARP_START_X+9)
	#define CITY_WARP_TITLE_START_Y				(CITY_WARP_START_Y+6)
	#define CITY_WARP_LIST_START_X				(CITY_WARP_START_X+19)
	#define CITY_WARP_LIST_START_Y				(CITY_WARP_START_Y+68)

	#define CITY_WARP_LIST_PRICE_X				( CITY_WARP_LIST_NAME_X )
	#define CITY_WARP_LIST_NAME_X				( CITY_WARP_LIST_START_X + 6 )
	#define CITY_WARP_LIST_PRICE_X				( CITY_WARP_LIST_NAME_X )

	#define CITY_WARP_LIST_SELECTBAR_GAB		5
	#define CITY_WARP_LIST_NAME_SIZE			180
	#define CITY_WARP_LIST_PRICE_SIZE			84

	#define CITY_WARP_CASH_START_X				(CITY_WARP_START_X+55)
	#define CITY_WARP_CASH_START_Y				(CITY_WARP_START_Y+162)
	#define CITY_WARP_LIST_INTERVAL				22

	#define CITY_WARP_BUTTON_MOVE_START_X		(CITY_WARP_START_X+260)
	#define CITY_WARP_BUTTON_MOVE_START_Y		(CITY_WARP_START_Y+243)
	#define CITY_WARP_BUTTON_CANCEL_START_X		(CITY_WARP_START_X+172)
	#define CITY_WARP_BUTTON_CANCEL_START_Y		(CITY_WARP_START_Y+161)
	#define CITY_WARP_BUTTON_SIZE_X				30
	#define CITY_WARP_BUTTON_SIZE_Y				30
	#define CITY_WARP_LINE_SIZE_X				169

	#define SCROLL_START_X						(CITY_WARP_START_X+304)
	#define SCROLL_START_Y						(CITY_WARP_START_Y+68)
	#define SCROLL_LINE_LENGTH					171

	#define CITY_WARP_TEX_X						CITY_WARP_START_X + 243
	#define CITY_WARP_TEX_Y						CITY_WARP_START_Y + 285
#else
#define CITY_WARP_START_X					CITY_BASE_NPC_BOX_START_X
#define CITY_WARP_START_Y					(CITY_BASE_NPC_BOX_START_Y-CITY_BASE_WARP_SIZE_Y)

#define CITY_WARP_BACK_START_X				(CITY_WARP_START_X+12)
#define CITY_WARP_BACK_START_Y				(CITY_WARP_START_Y+27)
#define CITY_WARP_TITLE_START_X				(CITY_WARP_START_X+9)
#define CITY_WARP_TITLE_START_Y				(CITY_WARP_START_Y+6)
#define CITY_WARP_LIST_START_X				(CITY_WARP_START_X+24)
#define CITY_WARP_LIST_START_Y				(CITY_WARP_START_Y+53)
#define CITY_WARP_CASH_START_X				(CITY_WARP_START_X+55)
#define CITY_WARP_CASH_START_Y				(CITY_WARP_START_Y+162)
#define CITY_WARP_LIST_INTERVAL				17

#define CITY_WARP_BUTTON_MOVE_START_X		(CITY_WARP_START_X+132)
#define CITY_WARP_BUTTON_MOVE_START_Y		(CITY_WARP_START_Y+161)
#define CITY_WARP_BUTTON_CANCEL_START_X		(CITY_WARP_START_X+172)
#define CITY_WARP_BUTTON_CANCEL_START_Y		(CITY_WARP_START_Y+161)
#define CITY_WARP_BUTTON_SIZE_X				38
#define CITY_WARP_BUTTON_SIZE_Y				17
#define CITY_WARP_LINE_SIZE_X				169

#define SCROLL_START_X						(CITY_WARP_START_X+202)
#define SCROLL_START_Y						(CITY_WARP_START_Y+52)
#define SCROLL_LINE_LENGTH					103

#define CITY_WARP_TEX_X						CITY_WARP_START_X + 163
#define CITY_WARP_TEX_Y						CITY_WARP_START_Y + 5
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCityWarp::CINFCityWarp(CAtumNode* pParent)
{
	m_pParent = pParent;

	m_pImgBack = NULL;
	m_pImgTitle = NULL;
	for(int i=0;i<CITY_WARP_BUTTON_NUMBER;i++)
	{
		m_pButtonMove[i] = NULL;
		m_pButtonCancel[i] = NULL;
	}
	m_pImgHightLight = NULL;
	m_bRestored = FALSE;
	memset(m_pFontWarpList, 0x00, sizeof(DWORD)*CITY_WARP_LIST_NUMBER);
	memset(m_pFontWarpPrice, 0x00, sizeof(DWORD)*CITY_WARP_LIST_NUMBER);
	m_pScroll = NULL;
	Reset();
	m_pInfluenceTex = NULL;
}

CINFCityWarp::~CINFCityWarp()
{
	util::del(m_pImgBack);
	util::del(m_pImgTitle);
	int i;
	for(i=0;i<CITY_WARP_BUTTON_NUMBER;i++)
	{
		util::del(m_pButtonMove[i]);
		util::del(m_pButtonCancel[i]);
	}
	util::del(m_pImgHightLight);

	for(i=0;i<CITY_WARP_LIST_NUMBER;i++)
	{
		util::del(m_pFontWarpList[i]);
		util::del(m_pFontWarpPrice[i]);
	}

	CVectorWarpTargetInfoIterator it = m_vecWarpTargetInfo.begin();
	while(it != m_vecWarpTargetInfo.end())
	{
		util::del(*it);
		it++;
	}
	m_vecWarpTargetInfo.clear();
	util::del(m_pScroll);
	util::del(m_pInfluenceTex);
}

void CINFCityWarp::Reset()
{
//	m_nCurrentSelectWarpIndex = -1;
	m_nButtonState[0] = BUTTON_STATE_NORMAL;
	m_nButtonState[1] = BUTTON_STATE_NORMAL;
	memset(m_szWarpList, 0x00, CITY_WARP_LIST_NUMBER*CITY_WARP_LIST_STRING_LENGTH);

//	m_nCurrentWarpListScroll = 0;
//	m_bWarpListScrollLock = FALSE;	
	m_nWarpListLineNumber = 0;	

	if(m_vecWarpTargetInfo.size()>0)
	{
		CVectorWarpTargetInfoIterator it = m_vecWarpTargetInfo.begin();
		while(it != m_vecWarpTargetInfo.end())
		{
			util::del(*it);
			it++;
		}
		m_vecWarpTargetInfo.clear();
	}
	m_nMapIndex = 0;	
	m_nTargetIndex = 0;	

}

void CINFCityWarp::AddWarpTargetInfoList(WARP_TARGET_MAP_INFO_4_EXCHANGE* pInfo)
{
	// 2006-09-29 by ispark, ³»°¡ °¥ ¼ö ÀÖ´Â ¿öÇÁÀÎÁö Ã¼Å©
	int nQuestIndex = GetQuestIndexForWarp(pInfo->MapIndex);
	if(nQuestIndex != 0 && !g_pQuestData->IsQuestCompleted(nQuestIndex))
	{
		// ¿Ï·á ¾È‰?Äù½ºÆ®
		return;
	}
	// 2007-09-18 by dgwoo, ´Ù¸¥ ¼¼·ÂÀÇ ¸ÊÀº º¸¿©ÁöÁö ¾Ê´Â´Ù.
	if(!IsNotInfluenceSameMap(g_pShuttleChild->m_myShuttleInfo.InfluenceType, pInfo->MapIndex))
	{
		return;
	}

	// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ
	BURNING_MAP_INFO* pBurning = g_pDatabase->GetPtr_BurningMapInfo(pInfo->MapIndex);
	if(pBurning)
	{
		
		if(!IS_SAME_UNITKIND(g_pShuttleChild->m_myShuttleInfo.UnitKind,pBurning->ReqUnitKind))
		{
			return;
		}
	}
	// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ

	WARP_TARGET_MAP_INFO_4_EXCHANGE* pNewInfo = new WARP_TARGET_MAP_INFO_4_EXCHANGE;
	memcpy(pNewInfo, pInfo, sizeof(WARP_TARGET_MAP_INFO_4_EXCHANGE));
	m_vecWarpTargetInfo.push_back(pNewInfo);
//	m_nWarpListLineNumber = m_vecWarpTargetInfo.size();
}

void CINFCityWarp::RecvWarpListDone()
{
	m_nWarpListLineNumber = m_vecWarpTargetInfo.size();
	m_pScroll->SetNumberOfData( m_nWarpListLineNumber );
//	int i =0;
//	CVectorWarpTargetInfoIterator it = m_vecWarpTargetInfo.begin();
//	while(it != m_vecWarpTargetInfo.end())
//	{
//		wsprintf(m_szWarpList[i], "%d    %s",i, (*it)->TargetName);
//		it++;
//		i++;
//	}
}

HRESULT CINFCityWarp::InitDeviceObjects()
{
	DataHeader* pDataHeader;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("warp");
	m_pImgBack = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pImgBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
#else
	pDataHeader = m_pGameData->Find("wpbk");
	m_pImgBack = new CINFImageEx;
	m_pImgBack->InitDeviceObjects( pDataHeader );
#endif
	pDataHeader = m_pGameData->Find("wptitle");
	m_pImgTitle = new CINFImageEx;
	m_pImgTitle->InitDeviceObjects( pDataHeader );
	int i;
	for(i=0;i<CITY_WARP_BUTTON_NUMBER;i++)
	{
		char buf[32];
		wsprintf( buf, "wpmove0%d",i);
		pDataHeader = m_pGameData->Find(buf);
		m_pButtonMove[i] = new CINFImageEx;
		m_pButtonMove[i]->InitDeviceObjects( pDataHeader );
		wsprintf( buf, "shmcan0%d",i);
		pDataHeader = m_pGameData->Find(buf);
		m_pButtonCancel[i] = new CINFImageEx;
		m_pButtonCancel[i]->InitDeviceObjects( pDataHeader );
	}
	pDataHeader = m_pGameData->Find("wphlgt");
	m_pImgHightLight = new CINFImageEx;
	m_pImgHightLight->InitDeviceObjects( pDataHeader );

	for(i=0;i<CITY_WARP_LIST_NUMBER;i++)
	{
		m_pFontWarpList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontWarpList[i]->InitDeviceObjects(g_pD3dDev) ;
		m_pFontWarpPrice[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,256,32);
		m_pFontWarpPrice[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	m_pScroll = new CINFScrollBar(this,
								SCROLL_START_X, 
								SCROLL_START_Y, 
								SCROLL_LINE_LENGTH,
								CITY_WARP_LIST_NUMBER);
	m_pScroll->SetGameData( m_pGameData );
	m_pScroll->InitDeviceObjects();

	m_pInfluenceTex = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  TRUE,256,32);
	m_pInfluenceTex->InitDeviceObjects(g_pD3dDev);

	m_pInfluenceTex->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CINFCityWarp::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		m_pImgBack->RestoreDeviceObjects();
		m_pImgTitle->RestoreDeviceObjects();
		int i;
		for(i=0;i<CITY_WARP_BUTTON_NUMBER;i++)
		{
			m_pButtonMove[i]->RestoreDeviceObjects();
			m_pButtonCancel[i]->RestoreDeviceObjects();
		}
		m_pImgHightLight->RestoreDeviceObjects();
		for(i=0;i<CITY_WARP_LIST_NUMBER;i++)
		{
			m_pFontWarpList[i]->RestoreDeviceObjects() ;
			m_pFontWarpPrice[i]->RestoreDeviceObjects();
		}
		m_pScroll->RestoreDeviceObjects();
		m_pScroll->SetWheelRect(CITY_WARP_LIST_START_X, 
			CITY_WARP_LIST_START_Y,
			CITY_WARP_LIST_START_X+CITY_WARP_LINE_SIZE_X,
			CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*CITY_WARP_LIST_NUMBER);

		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFCityWarp::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		m_pImgBack->InvalidateDeviceObjects();
		m_pImgTitle->InvalidateDeviceObjects();
		int i;
		for(i=0;i<CITY_WARP_BUTTON_NUMBER;i++)
		{
			m_pButtonMove[i]->InvalidateDeviceObjects();
			m_pButtonCancel[i]->InvalidateDeviceObjects();
		}
		m_pImgHightLight->InvalidateDeviceObjects();
		for(i=0;i<CITY_WARP_LIST_NUMBER;i++)
		{
			m_pFontWarpList[i]->InvalidateDeviceObjects() ;
			m_pFontWarpPrice[i]->InvalidateDeviceObjects();
		}
		m_pScroll->InvalidateDeviceObjects();

		m_bRestored = FALSE;
	}
	m_pInfluenceTex->InvalidateDeviceObjects();
	return S_OK;
}

HRESULT CINFCityWarp::DeleteDeviceObjects()
{
	m_pImgBack->DeleteDeviceObjects();
	util::del(m_pImgBack);
	m_pImgTitle->DeleteDeviceObjects();
	util::del(m_pImgTitle);
	int i;
	for(i=0;i<CITY_WARP_BUTTON_NUMBER;i++)
	{
		m_pButtonMove[i]->DeleteDeviceObjects();
		m_pButtonCancel[i]->DeleteDeviceObjects();
		util::del(m_pButtonMove[i]);
		util::del(m_pButtonCancel[i]);
	}
	m_pImgHightLight->DeleteDeviceObjects();
	util::del(m_pImgHightLight);
	for(i=0;i<CITY_WARP_LIST_NUMBER;i++)
	{
		m_pFontWarpList[i]->DeleteDeviceObjects();
		util::del(m_pFontWarpList[i]);
		m_pFontWarpPrice[i]->DeleteDeviceObjects();
		util::del(m_pFontWarpPrice[i]);
	}
	m_pScroll->DeleteDeviceObjects();
	util::del(m_pScroll);

	m_pInfluenceTex->DeleteDeviceObjects();
	util::del(m_pInfluenceTex);
	return S_OK;
}

void CINFCityWarp::Render()
{
	m_pImgBack->Move(CITY_WARP_BACK_START_X,CITY_WARP_BACK_START_Y);
	m_pImgBack->Render();

#ifndef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgTitle->Move(CITY_WARP_TITLE_START_X,CITY_WARP_TITLE_START_Y);
	m_pImgTitle->Render();

	if(m_nButtonState[CITY_WARP_BUTTON_MOVE] != BUTTON_STATE_NORMAL)
	{
		m_pButtonMove[m_nButtonState[CITY_WARP_BUTTON_MOVE]]->Move(CITY_WARP_BUTTON_MOVE_START_X, CITY_WARP_BUTTON_MOVE_START_Y);
		m_pButtonMove[m_nButtonState[CITY_WARP_BUTTON_MOVE]]->Render();
	}

	if(m_nButtonState[CITY_WARP_BUTTON_CANCEL] != BUTTON_STATE_NORMAL)
	{
		m_pButtonCancel[m_nButtonState[CITY_WARP_BUTTON_CANCEL]]->Move(CITY_WARP_BUTTON_CANCEL_START_X, CITY_WARP_BUTTON_CANCEL_START_Y);
		m_pButtonCancel[m_nButtonState[CITY_WARP_BUTTON_CANCEL]]->Render();
	}
#else
 	m_pButtonMove[m_nButtonState[CITY_WARP_BUTTON_MOVE]]->Move(CITY_WARP_BUTTON_MOVE_START_X, CITY_WARP_BUTTON_MOVE_START_Y);
 	m_pButtonMove[m_nButtonState[CITY_WARP_BUTTON_MOVE]]->Render();
#endif
//	if(m_nCurrentSelectWarpIndex != -1)
//	{
//		m_pImgHightLight->Move(CITY_WARP_LIST_START_X+1,
//			CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*m_nCurrentSelectWarpIndex+1);
//		m_pImgHightLight->Render();
//	}
	if(m_pScroll->GetCurrentSelectWindowIndex() >= 0 &&
		m_pScroll->GetCurrentSelectWindowIndex() < CITY_WARP_LIST_NUMBER)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
		m_pImgHightLight->Move(CITY_WARP_LIST_START_X,
			CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*m_pScroll->GetCurrentSelectWindowIndex()+1);
#else
		m_pImgHightLight->Move( CITY_WARP_LIST_START_X + 2,
							    CITY_WARP_LIST_START_Y - CITY_WARP_LIST_SELECTBAR_GAB + CITY_WARP_LIST_INTERVAL * m_pScroll->GetCurrentSelectWindowIndex());
#endif
		m_pImgHightLight->Render();
	}
//	for(int i=0;i<CITY_WARP_LIST_NUMBER;i++)
//	{
//		if(m_szWarpList[i])
//		{
//			m_pFontWarpList[i]->DrawText(CITY_WARP_LIST_START_X+1, 
//				CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*i+1,
//				m_pScroll->GetCurrentSelectWindowIndex == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
//				m_szWarpList[i],0L);
//		}
//		else 
//		{
//			break;
//		}
//	}
	for(int i=0;i<CITY_WARP_LIST_NUMBER;i++)
	{
		int index = m_pScroll->GetCurrentScrollIndex()+i;
		if(index >= m_vecWarpTargetInfo.size())
		{
			break;
		}
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
		char chWarpPrice[30] = {0, };
		int len = 0;
//		wsprintf(m_szWarpList[i], "%16s%10d", m_vecWarpTargetInfo[index]->TargetName, m_vecWarpTargetInfo[index]->Fee);
		wsprintf(m_szWarpList[i], "%s", m_vecWarpTargetInfo[index]->TargetName);
		int nWarpPrice = (CAtumSJ::GetCityWarTex(m_vecWarpTargetInfo[index]->Fee, m_fTexRate) + m_vecWarpTargetInfo[index]->Fee);
		// 2006-04-21 by ispark, ÁöµµÀÚ´Â °øÂ¥
		if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_INFLUENCE_LEADER))
		{
			nWarpPrice = 0;
		// 2007-10-10 by dgwoo ¸â¹ö½± À¯Àú´Â ¿öÇÁºñ Àý°¨
		}else if(0 != g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1)
		{
			nWarpPrice = nWarpPrice - (nWarpPrice / (100/MEMBERSHIP_DISCOUNT_WARPFEE));
		}
		wsprintf(chWarpPrice, "%d", nWarpPrice);
		len = strlen(chWarpPrice);
//#else
//		int nWarpPrice = (CAtumSJ::GetCityWarTex(m_vecWarpTargetInfo[index]->Fee, m_fTexRate) + m_vecWarpTargetInfo[index]->Fee);
//		// 2006-04-21 by ispark, ÁöµµÀÚ´Â °øÂ¥
//		if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_INFLUENCE_LEADER))
//		{
//			nWarpPrice = 0;
//		}
//		wsprintf(m_szWarpList[i], "%16s%10d", m_vecWarpTargetInfo[index]->TargetName, nWarpPrice);
//#endif
		if(m_szWarpList[i])
		{
//#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			SIZE Size = m_pFontWarpPrice[i]->GetStringSize( m_szWarpList[i] );
			m_pFontWarpList[i]->DrawText(CITY_WARP_LIST_NAME_X + ( CITY_WARP_LIST_NAME_SIZE / 2 ) - ( Size.cx / 2 ),
										 CITY_WARP_LIST_START_Y + CITY_WARP_LIST_INTERVAL * i,
										 m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
										 m_szWarpList[i],0L);
			Size = m_pFontWarpPrice[i]->GetStringSize( chWarpPrice );
			m_pFontWarpPrice[i]->DrawText( CITY_WARP_LIST_PRICE_X + CITY_WARP_LIST_NAME_SIZE + ( CITY_WARP_LIST_PRICE_SIZE / 2 ) - ( Size.cx / 2 ),
  										   CITY_WARP_LIST_START_Y + CITY_WARP_LIST_INTERVAL * i,
										   m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
									 	   chWarpPrice,0L);
#else
			m_pFontWarpList[i]->DrawText(CITY_WARP_LIST_START_X+1, 
				CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*i-1,
				m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
				m_szWarpList[i],0L);
			m_pFontWarpPrice[i]->DrawText(CITY_WARP_LIST_START_X+165 - len*7, 
				CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*i-1,
				m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
				chWarpPrice,0L);
#endif
//#else
//			m_pFontWarpList[i]->DrawText(CITY_WARP_LIST_START_X+1, 
//				CITY_WARP_LIST_START_Y+CITY_WARP_LIST_INTERVAL*i+1,
//				m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
//				m_szWarpList[i],0L);
//#endif
		}
		else 
		{
			break;
		}
	}
	m_pScroll->Render();

	// 2006-02-08 by ispark, ¼¼·Â ¼¼±Ý
	char chTexbuf[30] = {0,};
	sprintf(chTexbuf, STRMSG_C_060208_0000, m_fTexRate);
	m_pInfluenceTex->DrawText(CITY_WARP_TEX_X, CITY_WARP_TEX_Y, GUI_FONT_COLOR, chTexbuf, 0L);
}

void CINFCityWarp::Tick()
{

}

int CINFCityWarp::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(m_pScroll)
	{
		if(m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return INF_MSGPROC_BREAK;
		}
	}
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.y > CITY_WARP_BUTTON_MOVE_START_Y && 
				pt.y < CITY_WARP_BUTTON_MOVE_START_Y + CITY_WARP_BUTTON_SIZE_Y)
			{
				if( pt.x > CITY_WARP_BUTTON_MOVE_START_X && 
					pt.x < CITY_WARP_BUTTON_MOVE_START_X+CITY_WARP_BUTTON_SIZE_X)
				{
					if(m_nButtonState[CITY_WARP_BUTTON_MOVE] != BUTTON_STATE_DOWN)
					{
						
						m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_UP;
						// 2011. 1. 12 by jskim UI ÀÌ¹ÌÁö ¹öÆ° ÅøÆÁ ±¸Çö	
						g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP52");
						// end 2011. 1. 12 by jskim UI ÀÌ¹ÌÁö ¹öÆ° ÅøÆÁ ±¸Çö	
					}
				}
				else 
				{
					m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_NORMAL;
					// 2011. 1. 12 by jskim UI ÀÌ¹ÌÁö ¹öÆ° ÅøÆÁ ±¸Çö	
					g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
					// end 2011. 1. 12 by jskim UI ÀÌ¹ÌÁö ¹öÆ° ÅøÆÁ ±¸Çö	
				}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
				if( pt.x > CITY_WARP_BUTTON_CANCEL_START_X && 
					pt.x < CITY_WARP_BUTTON_CANCEL_START_X+CITY_WARP_BUTTON_SIZE_X)
				{
					if(m_nButtonState[CITY_WARP_BUTTON_CANCEL] != BUTTON_STATE_DOWN)
						m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
				}
#endif
			}
			else
			{
				m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_NORMAL;
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
				m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
#endif
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.y > CITY_WARP_BUTTON_MOVE_START_Y && 
				pt.y < CITY_WARP_BUTTON_MOVE_START_Y + CITY_WARP_BUTTON_SIZE_Y)
			{
				if( pt.x > CITY_WARP_BUTTON_MOVE_START_X && 
					pt.x < CITY_WARP_BUTTON_MOVE_START_X+CITY_WARP_BUTTON_SIZE_X)
				{
					m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_NORMAL;
				}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
				if( pt.x > CITY_WARP_BUTTON_CANCEL_START_X && 
					pt.x < CITY_WARP_BUTTON_CANCEL_START_X+CITY_WARP_BUTTON_SIZE_X)
				{
					m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_DOWN;
				}
				else 
				{
					m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
				}
#endif
			}
//			if( pt.x > CITY_WARP_LIST_START_X && 
//				pt.x < CITY_WARP_LIST_START_X + CITY_WARP_LINE_SIZE_X &&
//				pt.y > CITY_WARP_LIST_START_Y && 
//				pt.y < CITY_WARP_LIST_START_Y + CITY_WARP_LIST_INTERVAL*CITY_WARP_LIST_NUMBER)
//			{
//				int i = (pt.y-CITY_WARP_LIST_START_Y)/CITY_WARP_LIST_INTERVAL;
//				if(i>=0 && i<CITY_WARP_LIST_NUMBER)
//				{
//					if(m_vecWarpTargetInfo.size() >= i + m_nCurrentWarpListScroll)
//					{
//						m_nCurrentSelectWarpIndex = i;
//					}
//				}
//			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if(pt.y > CITY_WARP_BUTTON_MOVE_START_Y && 
				pt.y < CITY_WARP_BUTTON_MOVE_START_Y + CITY_WARP_BUTTON_SIZE_Y)
			{
				if( pt.x > CITY_WARP_BUTTON_MOVE_START_X && 
					pt.x < CITY_WARP_BUTTON_MOVE_START_X+CITY_WARP_BUTTON_SIZE_X)
				{
					if(m_nButtonState[CITY_WARP_BUTTON_MOVE] == BUTTON_STATE_DOWN)
					{
						OnButtonClicked(CITY_WARP_BUTTON_MOVE);
					}
					m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_WARP_BUTTON_MOVE] = BUTTON_STATE_NORMAL;
				}
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
				if( pt.x > CITY_WARP_BUTTON_CANCEL_START_X && 
					pt.x < CITY_WARP_BUTTON_CANCEL_START_X+CITY_WARP_BUTTON_SIZE_X)
				{
					if(m_nButtonState[CITY_WARP_BUTTON_CANCEL] == BUTTON_STATE_DOWN)
					{
						OnButtonClicked(CITY_WARP_BUTTON_CANCEL);
					}
					m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_UP;
				}
				else 
				{
					m_nButtonState[CITY_WARP_BUTTON_CANCEL] = BUTTON_STATE_NORMAL;
				}
#endif
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFCityWarp::OnButtonClicked(int nButton)
{
	switch(nButton)
	{
	case CITY_WARP_BUTTON_MOVE:
		{
//			if(m_nCurrentSelectWarpIndex != -1 && 
//				m_nCurrentSelectWarpIndex + m_nCurrentWarpListScroll < m_vecWarpTargetInfo.size())
//			{
//				WARP_TARGET_MAP_INFO_4_EXCHANGE *pWarpInfo = m_vecWarpTargetInfo[m_nCurrentSelectWarpIndex + m_nCurrentWarpListScroll];
//				if(pWarpInfo)
//				{
//					DBGOUT("¿öÇÁ ÇÕ½Ã´Ù(Map:%d,Target:%d,Name:%s).!!\n",
//						pWarpInfo->MapIndex, pWarpInfo->TargetIndex, pWarpInfo->TargetName);
//					SendFieldSocketRequestShopWarp(pWarpInfo->MapIndex, pWarpInfo->TargetIndex);
//				}
//			}
			int nCurrentSelectRealIndex = m_pScroll->GetCurrentSelectDataIndex();
			if( nCurrentSelectRealIndex >= 0 && 
				nCurrentSelectRealIndex < m_vecWarpTargetInfo.size())
			{
				WARP_TARGET_MAP_INFO_4_EXCHANGE *pWarpInfo = m_vecWarpTargetInfo[nCurrentSelectRealIndex];
				if(pWarpInfo)
				{
					// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ
					BURNING_MAP_INFO* pBurning = g_pDatabase->GetPtr_BurningMapInfo(pWarpInfo->MapIndex);
					if(pBurning)
					{
						
						if((pBurning->ReqMaxLv < g_pShuttleChild->m_myShuttleInfo.Level)
							|| (pBurning->ReqMinLv > g_pShuttleChild->m_myShuttleInfo.Level))
						{
							CHAR buf[256] = {0,};
							sprintf(buf,STRMSG_C_100810_0100,pBurning->ReqMinLv,pBurning->ReqMaxLv);
							g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_ERROR );
						}
					}
					// 2010-08-10 by dgwoo ¹ö´×¸Ê ½Ã½ºÅÛ

					// 2008-01-31 by dgwoo ¸â¹ö½± À¯ÀúÀÇ °æ¿ì ¿öÇÁºñ ÇÒÀÎ.
					INT nWarpPrice = pWarpInfo->Fee;
					if(0 != g_pD3dApp->GetPrimiumCardInfo()->nCardItemNum1)
					{
						nWarpPrice = nWarpPrice - (nWarpPrice / (100/MEMBERSHIP_DISCOUNT_WARPFEE));
					}
					if( nWarpPrice > g_pGameMain->m_pInven->GetItemSpi() )
					{
						g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_CITY_0006, COLOR_ERROR );//"¿öÇÁ¿¡ ÇÊ¿äÇÑ ½ºÇÇ°¡ ºÎÁ·ÇÕ´Ï´Ù."
						break;
					}
					if( g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR ) == NULL )
					{
						g_pD3dApp->m_pChat->CreateChatChild( STRMSG_C_CITY_0007, COLOR_ERROR );//"¿£ÁøÀÌ ¾øÀ¸¸é Ãâ°Ý ÇÒ ¼ö ¾ø½À´Ï´Ù."
						break;
					}
					
					GUI_BUILDINGNPC* pBuilding = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
					if(pBuilding)
					{
						g_pInterface->m_pCityBase->SendLeaveEnterBuilding( pBuilding->buildingInfo.BuildingIndex, -1 );
					}
//					SendFieldSocketRequestShopWarp(pWarpInfo->MapIndex, pWarpInfo->TargetIndex);
					m_nMapIndex = pWarpInfo->MapIndex;	
					m_nTargetIndex = pWarpInfo->TargetIndex;	

					// 2004-10-25 by jschoi
					// ¾Æ·¡ ¸ðµÎ Áö¿ì°í Ãâ°Ý °¡´ÉÇÑ°¡ ¼­¹ö·Î ¿äÃ»ÇÑ´Ù.
					// ¼­¹ö¿¡¼­ È®ÀÎÀ» ¹ÞÀ¸¸é ¾Æ·¡ Áö¿î ºÎºÐÀ» µ¿ÀÛÇÏµµ·Ï ÇÑ´Ù.
					// ¼­¹ö¿¡¼­ ¿¡·¯°¡ ¶³¾îÁö°Å³ª Ãâ°Ý ºÒ°¡´ÉÇÑ ÀÌÀ¯°¡ ¿À¸é ÀÌÀ¯¸¦ Ãâ·ÂÇÑ´Ù.
						g_pShuttleChild->m_nEventType = EVENT_CITY_OUT_MOVE;
					g_pFieldWinSocket->SendMsg(T_FC_CITY_CHECK_WARP_STATE, NULL, 0);

//					util::del(g_pShuttleChild->m_pCinemaCamera);
//					if (g_pShuttleChild->InitCinemaUnit(PATTERN_UNIT_CITY_OUT) == TRUE)
//					{
//						g_pD3dApp->m_pSound->PlayD3DSound( SOUND_TAKEINGOFF_IN_CITY, g_pShuttleChild->m_vPos );
//						g_pShuttleChild->ChangeSingleBodyCondition(BODYCON_TAKEOFF_MASK);
//						g_pShuttleChild->m_nEventType = EVENT_CITY_OUT_MOVE;
//						g_pShuttleChild->ChangeUnitState( _TAKINGOFF );
//						CAppEffectData * pEffect = new CAppEffectData(RC_EFF_LANDING_TAKEOFF,MAP_TYPE_CITY_UNIT_POS);
//						pEffect->ChangeBodyCondition(BODYCON_LANDED_MASK);
//						g_pD3dApp->m_pEffectList->AddChild(pEffect);
//
//					}
//					else
//					{
//						SendFieldSocketRequestShopWarp();
//					}
				}
			}
		}
		break;
	case CITY_WARP_BUTTON_CANCEL:
		{
		}
		break;
	}

}

void CINFCityWarp::SendFieldSocketRequestShopWarp()//int nMapIndex, int nTargetIndex)
{
	MSG_FC_EVENT_REQUEST_SHOP_WARP sMsg;
	sMsg.MapIndex = m_nMapIndex;
	sMsg.TargetIndex = m_nTargetIndex;
	g_pFieldWinSocket->SendMsg(T_FC_EVENT_REQUEST_SHOP_WARP, (char*)&sMsg, sizeof(sMsg));
	DBGOUT("FieldSocket : Request Warp. T_FC_EVENT_REQUEST_SHOP_WARP\n");
}

WARP_TARGET_MAP_INFO_4_EXCHANGE *CINFCityWarp::GetCurrentWarpInfo()
{
//	return m_vecWarpTargetInfo[m_nCurrentSelectWarpIndex + m_nCurrentWarpListScroll];
	return m_vecWarpTargetInfo[m_pScroll->GetCurrentSelectDataIndex()];
}