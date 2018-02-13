// INFChannelList.cpp: implementation of the CINFChannelList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFChannelList.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "INFWindow.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "FieldWinSocket.h"
#include "INFScrollBar.h"
#include "ObjectChild.h"
#include "ClientParty.h"
#include "dxutil.h"
#include "AtumDatabase.h"
#include "INFImageEx.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"
#include "INFToolTip.h"
#include "Interface.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	#define CHANNEL_LIST_SIZE_X				328
	#define CHANNEL_LIST_SIZE_Y				297
	#define CHANNEL_LIST_START_X			((g_pD3dApp->GetBackBufferDesc().Width - CHANNEL_LIST_SIZE_X)/2)
	#define CHANNEL_LIST_START_Y			((g_pD3dApp->GetBackBufferDesc().Height - CHANNEL_LIST_SIZE_Y)/2)
	#define CHANNEL_LIST_TITLE_START_X		(CHANNEL_LIST_START_X+9)
	#define CHANNEL_LIST_TITLE_START_Y		(CHANNEL_LIST_START_Y+6)
	#define CHANNEL_LIST_BACK_START_X		(CHANNEL_LIST_START_X)
	#define CHANNEL_LIST_BACK_START_Y		(CHANNEL_LIST_START_Y)
	#define CHANNEL_LIST_MAPNAME_START_X	(CHANNEL_LIST_BACK_START_X+37)
	#define CHANNEL_LIST_MAPNAME_START_Y	(CHANNEL_LIST_BACK_START_X+60)
	#define CHANNEL_LIST_LIST_START_X		(CHANNEL_LIST_BACK_START_X+21)
	#define CHANNEL_LIST_LIST_START_Y		(CHANNEL_LIST_BACK_START_Y+62)
	#define CHANNEL_LIST_LINE_INTERVAL		20
	#define CHANNEL_LIST_LINE_GAB			2
	#define CHANNEL_LIST_LIST_SIZE_X		270
	#define CHANNEL_LIST_OK_BUTTON_START_X	(CHANNEL_LIST_BACK_START_X+257)
	#define CHANNEL_LIST_OK_BUTTON_START_Y	(CHANNEL_LIST_BACK_START_Y+240)
	#define CHANNEL_LIST_OK_BUTTON_SIZE_X	30
	#define CHANNEL_LIST_OK_BUTTON_SIZE_Y	30

	#define SCROLL_START_X					(CHANNEL_LIST_BACK_START_X+303)
	#define SCROLL_START_Y					(CHANNEL_LIST_BACK_START_Y+67)
	#define SCROLL_END_Y					(CHANNEL_LIST_BACK_START_Y+154)
	#define SCROLL_LINE_LENGTH				(172)
	
	#define CHANNEL_LIST_MAPNAME_SIZE		(178)
	#define CHANNEL_LIST_CHANNEL_SIZE		(84)
#else
#define CHANNEL_LIST_SIZE_X				233
#define CHANNEL_LIST_SIZE_Y				201
#define CHANNEL_LIST_START_X			((g_pD3dApp->GetBackBufferDesc().Width - CHANNEL_LIST_SIZE_X)/2)
#define CHANNEL_LIST_START_Y			((g_pD3dApp->GetBackBufferDesc().Height - CHANNEL_LIST_SIZE_Y)/2)
#define CHANNEL_LIST_TITLE_START_X		(CHANNEL_LIST_START_X+9)
#define CHANNEL_LIST_TITLE_START_Y		(CHANNEL_LIST_START_Y+6)
#define CHANNEL_LIST_BACK_START_X		(CHANNEL_LIST_START_X+12)
#define CHANNEL_LIST_BACK_START_Y		(CHANNEL_LIST_START_Y+27)
#define CHANNEL_LIST_MAPNAME_START_X	(CHANNEL_LIST_START_X+31)
#define CHANNEL_LIST_MAPNAME_START_Y	(CHANNEL_LIST_START_Y+53)
#define CHANNEL_LIST_LIST_START_X		(CHANNEL_LIST_START_X+31)
#define CHANNEL_LIST_LIST_START_Y		(CHANNEL_LIST_START_Y+70)
#define CHANNEL_LIST_LINE_INTERVAL		17
#define CHANNEL_LIST_LIST_SIZE_X		162
#define CHANNEL_LIST_OK_BUTTON_START_X	(CHANNEL_LIST_START_X+172)
#define CHANNEL_LIST_OK_BUTTON_START_Y	(CHANNEL_LIST_START_Y+161)
#define CHANNEL_LIST_OK_BUTTON_SIZE_X	38
#define CHANNEL_LIST_OK_BUTTON_SIZE_Y	17

#define SCROLL_START_X					(CHANNEL_LIST_START_X+202)
#define SCROLL_START_Y					(CHANNEL_LIST_START_Y+69)
#define SCROLL_END_Y					(CHANNEL_LIST_START_Y+154)
#define SCROLL_LINE_LENGTH				(SCROLL_END_Y - SCROLL_START_Y)
#endif

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI) // 2008-04-30 by bhsohn 태국 버전 추가
#define CHANNEL_LIST_TEXT_Y				0
#else
#define CHANNEL_LIST_TEXT_Y				1
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFChannelList::CINFChannelList(CAtumNode* pParent)
{
	g_pChannelList = this;
	m_pParent = pParent;
	m_bRestored = FALSE;
	m_pImgBack = NULL;
	m_pImgTitle = NULL;
	m_pImgScrollBar = NULL;
	memset(m_pFontChannelList, 0x00, sizeof(DWORD)*CHANNEL_LIST_NUMBER);
	memset(m_pImgButtonOk, 0x00, sizeof(DWORD)*CHANNEL_BUTTON_NUBER);
	m_pFontMapName = NULL;
	m_pImgHightLight = NULL;
	m_pScroll = NULL;

	Reset();
}

CINFChannelList::~CINFChannelList()
{
	g_pChannelList = NULL;
	util::del(m_pImgBack);
	util::del(m_pImgTitle);
	util::del(m_pImgScrollBar);
	util::del(m_pImgHightLight);
	int i;
	for(i=0;i<CHANNEL_LIST_NUMBER;i++)
	{
		util::del(m_pFontChannelList[i]);
	}
	for(i=0;i<CHANNEL_BUTTON_NUBER;i++)
	{
		util::del(m_pImgButtonOk[i]);
	}
	util::del(m_pScroll);
}

void CINFChannelList::Reset()
{
//	m_nCurrentSelectIndex = 0;
//	m_nCurrentScrollNumber = 0;
	memset(m_szChannelList, 0x00, CHANNEL_LIST_NUMBER*CHANNEL_LIST_STRING_LENGTH);
	memset(m_szMapName, 0x00, SIZE_MAX_MAP_NAME);
	m_nButtonState = BUTTON_STATE_NORMAL;
	m_nChannelListNumber = 0;

	CVectorChannelIterator it = m_vecChannelList.begin();
	while(it != m_vecChannelList.end())
	{
		util::del(*it);
		it++;
	}
	m_vecChannelList.clear();
	memset(&m_msgEventSelectChannel, 0x00, sizeof(MSG_FC_EVENT_SELECT_CHANNEL));
}

void CINFChannelList::RecvStartChannelList(int nMapIndex, int nTargetIndex)
{
	Reset();
	m_msgEventSelectChannel.MapChannelIndex.MapIndex = nMapIndex;
	m_msgEventSelectChannel.WarpTargetIndex = nTargetIndex;
}

void CINFChannelList::RecvChannelList(MEX_CHANNEL_INFO* pChannel)
{
	MEX_CHANNEL_INFO* pNew = new MEX_CHANNEL_INFO;
	memcpy(pNew, pChannel, sizeof(MEX_CHANNEL_INFO));
/*	if(m_vecChannelList.size() <CHANNEL_LIST_NUMBER)
	{
		if(pChannel->Crowdedness<30)
		{
			wsprintf(m_szChannelList[m_vecChannelList.size()],"%3d%20s",pNew->ChannelIndex,"원할");
		}
		else if(pChannel->Crowdedness<60)
		{
			wsprintf(m_szChannelList[m_vecChannelList.size()],"%3d%20s",pNew->ChannelIndex,"보통");
		}
		else
		{
			wsprintf(m_szChannelList[m_vecChannelList.size()],"%3d%20s",pNew->ChannelIndex,"혼잡");
		}
	}*/
	m_vecChannelList.push_back(pNew);
}

void CINFChannelList::RecvDoneChannelList()
{
	m_nChannelListNumber = m_vecChannelList.size();
	m_pScroll->SetNumberOfData( m_nChannelListNumber );
}

HRESULT CINFChannelList::InitDeviceObjects()
{
	DataHeader* pDataHeader;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("channel");
	m_pImgBack = g_pGameMain->m_GruopImagemanager->GetGroupImage(pDataHeader);
	m_pImgBack->InitDeviceObjects( g_pD3dApp->m_pImageList );
#else
	pDataHeader = m_pGameData->Find("chbk");
	m_pImgBack = new CINFImageEx;
	m_pImgBack->InitDeviceObjects( pDataHeader );
#endif
	pDataHeader = m_pGameData->Find("chtitle");
	m_pImgTitle = new CINFImageEx;
	m_pImgTitle->InitDeviceObjects( pDataHeader );
	pDataHeader = m_pGameData->Find("c_scrlb");
	m_pImgScrollBar = new CINFImageEx;
	m_pImgScrollBar->InitDeviceObjects( pDataHeader );
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	pDataHeader = m_pGameData->Find("wphlgt");
#else
	pDataHeader = m_pGameData->Find("chhlgt");
#endif	
	m_pImgHightLight = new CINFImageEx;
	m_pImgHightLight->InitDeviceObjects( pDataHeader );
	int i;
	for(i=0;i<CHANNEL_BUTTON_NUBER;i++)
	{
		char buf[64];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		wsprintf(buf, "wpmove0%d",i);
#else
		wsprintf(buf, "shlaok0%d",i);
#endif
		pDataHeader = m_pGameData->Find(buf);
		m_pImgButtonOk[i] = new CINFImageEx;
		m_pImgButtonOk[i]->InitDeviceObjects( pDataHeader );
	}
	for(i=0;i<CHANNEL_LIST_NUMBER;i++)
	{
		m_pFontChannelList[i] = new CD3DHanFont( _T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 256, 32);
		m_pFontChannelList[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	m_pFontMapName = new CD3DHanFont( _T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 256, 32);
	m_pFontMapName->InitDeviceObjects(g_pD3dDev) ;

	m_pScroll = new CINFScrollBar(this,
								SCROLL_START_X, 
								SCROLL_START_Y, 
								SCROLL_LINE_LENGTH,
								CHANNEL_LIST_NUMBER);
	m_pScroll->SetGameData( m_pGameData );
	m_pScroll->InitDeviceObjects();
	m_pScroll->SetWheelRect(CHANNEL_LIST_LIST_START_X, 
		CHANNEL_LIST_LIST_START_Y,
		CHANNEL_LIST_LIST_START_X+CHANNEL_LIST_LIST_SIZE_X,
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		CHANNEL_LIST_LIST_START_Y+( CHANNEL_LIST_LINE_INTERVAL + CHANNEL_LIST_LINE_GAB ) * CHANNEL_LIST_NUMBER);
#else
		CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*CHANNEL_LIST_NUMBER);
#endif
	return S_OK;
}

HRESULT CINFChannelList::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		m_pImgBack->RestoreDeviceObjects();
		m_pImgTitle->RestoreDeviceObjects();
		m_pImgScrollBar->RestoreDeviceObjects();
		m_pImgHightLight->RestoreDeviceObjects();
		int i;
		for(i=0;i<CHANNEL_BUTTON_NUBER;i++)
		{
			m_pImgButtonOk[i]->RestoreDeviceObjects();
		}
		for(i=0;i<CHANNEL_LIST_NUMBER;i++)
		{
			m_pFontChannelList[i]->RestoreDeviceObjects() ;
		}
		m_pFontMapName->RestoreDeviceObjects();
		m_pScroll->RestoreDeviceObjects();

		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFChannelList::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		m_pImgBack->InvalidateDeviceObjects();
		m_pImgTitle->InvalidateDeviceObjects();
		m_pImgScrollBar->InvalidateDeviceObjects();
		m_pImgHightLight->InvalidateDeviceObjects();
		int i;
		for(i=0;i<CHANNEL_BUTTON_NUBER;i++)
		{
			m_pImgButtonOk[i]->InvalidateDeviceObjects();
		}
		for(i=0;i<CHANNEL_LIST_NUMBER;i++)
		{
			m_pFontChannelList[i]->InvalidateDeviceObjects() ;
		}
		m_pFontMapName->InvalidateDeviceObjects();
		m_pScroll->InvalidateDeviceObjects();

		m_bRestored = FALSE;
	}
	return S_OK;
}

HRESULT CINFChannelList::DeleteDeviceObjects()
{
	m_pImgBack->DeleteDeviceObjects();
	util::del(m_pImgBack);
	m_pImgTitle->InvalidateDeviceObjects();
	util::del(m_pImgTitle);
	m_pImgScrollBar->InvalidateDeviceObjects();
	util::del(m_pImgScrollBar);
	m_pImgHightLight->InvalidateDeviceObjects();
	util::del(m_pImgHightLight);
	int i;
	for(i=0;i<CHANNEL_BUTTON_NUBER;i++)
	{
		m_pImgButtonOk[i]->DeleteDeviceObjects();
		util::del(m_pImgButtonOk[i]);
	}
	for(i=0;i<CHANNEL_LIST_NUMBER;i++)
	{
		m_pFontChannelList[i]->DeleteDeviceObjects() ;
		util::del(m_pFontChannelList[i]);
	}
	m_pFontMapName->InvalidateDeviceObjects();
	util::del(m_pFontMapName);
	m_pScroll->DeleteDeviceObjects();
	util::del(m_pScroll);
	return S_OK;
}


void CINFChannelList::Render()
{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
	g_pGameMain->m_pInfWindow->RenderCenterWindow( CHANNEL_LIST_START_X, 
												CHANNEL_LIST_START_Y, 
												CHANNEL_LIST_SIZE_X, 
												CHANNEL_LIST_SIZE_Y, FALSE);
#endif
	m_pImgBack->Move(CHANNEL_LIST_BACK_START_X, CHANNEL_LIST_BACK_START_Y);
	m_pImgBack->Render();
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgTitle->Move(CHANNEL_LIST_TITLE_START_X, CHANNEL_LIST_TITLE_START_Y);
	m_pImgTitle->Render();
#endif
//	if(m_nCurrentSelectIndex != -1 )
//	{
//		m_pImgHightLight->Move(CHANNEL_LIST_LIST_START_X+1, 
//			CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*m_nCurrentSelectIndex+1);
//		m_pImgHightLight->Render();
//	}
	if(m_pScroll->GetCurrentSelectWindowIndex() >= 0 &&
		m_pScroll->GetCurrentSelectWindowIndex() < CHANNEL_LIST_NUMBER)
	{
#ifndef C_EPSODE4_UI_CHANGE_JSKIM
		m_pImgHightLight->Move(CHANNEL_LIST_LIST_START_X+1, 
			CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*m_pScroll->GetCurrentSelectWindowIndex()+1);
#else
		m_pImgHightLight->Move(CHANNEL_LIST_LIST_START_X, 
			CHANNEL_LIST_LIST_START_Y + ( CHANNEL_LIST_LINE_INTERVAL + CHANNEL_LIST_LINE_GAB )*m_pScroll->GetCurrentSelectWindowIndex()+1);
#endif
		m_pImgHightLight->Render();
	}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM	
	m_pImgButtonOk[m_nButtonState]->Move(CHANNEL_LIST_OK_BUTTON_START_X,CHANNEL_LIST_OK_BUTTON_START_Y);
	m_pImgButtonOk[m_nButtonState]->Render();
#else
	if(m_nButtonState != BUTTON_STATE_NORMAL)
	{
		m_pImgButtonOk[m_nButtonState]->Move(CHANNEL_LIST_OK_BUTTON_START_X,CHANNEL_LIST_OK_BUTTON_START_Y);
		m_pImgButtonOk[m_nButtonState]->Render();
	}
#endif

	if(m_szMapName[0])
	{
		m_pFontMapName->DrawText(CHANNEL_LIST_MAPNAME_START_X,
			CHANNEL_LIST_MAPNAME_START_Y,
			GUI_FONT_COLOR,
			m_szMapName,0L);
	}
//	for(int i=0;i<m_nChannelListNumber;i++)
//	{
//		m_pFontChannelList[i]->DrawText(CHANNEL_LIST_LIST_START_X+1,
//			CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*i+1,
//			m_nCurrentSelectIndex == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
//			m_szChannelList[i],0L);
//	}
	for(int i=0;i<CHANNEL_LIST_NUMBER;i++)
	{
		int index = m_pScroll->GetCurrentScrollIndex()+i;
		if(index >= m_vecChannelList.size())
		{
			break;
		}
		MEX_CHANNEL_INFO* pChannel = m_vecChannelList[index];
		char stChannelIndex[32] = {0,};

		// 2006-11-08 by ispark, 중립 지역에서는 일반 숫자로 표시
		//MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(g_pShuttleChild->m_myShuttleInfo.MapChannelIndex.MapIndex);
		// 2007-02-06 by dgwoo 맵인덱스를 워프시 통과할 맵으로 변경.
		MAP_INFO* pMapInfo = g_pDatabase->GetMapInfo(m_msgEventSelectChannel.MapChannelIndex.MapIndex);
		if(MAP_INFLUENCE_NEUTRALITY == pMapInfo->MapInfluenceType)
		{
			// 중립지역
// 2007-02-06 by dgwoo 중립지역은 모두 전쟁지대이다.
//			if(0 >= index)
//				wsprintf(stChannelIndex, "%d", pChannel->ChannelIndex);
//			else
//				wsprintf(stChannelIndex, "%d", pChannel->ChannelIndex);
			wsprintf(stChannelIndex, "%d(%s)", pChannel->ChannelIndex,STRMSG_C_061106_0100);
		}
		else
		{
			// 2010. 11. 15. by hsLee. 버닝타임 맵일 경우 무조건 안전 지대로 표시.
			if ( NULL == g_pDatabase->GetPtr_BurningMapInfo( pMapInfo->MapIndex ) )
			{
				// 2006-11-06 by dgwoo 전쟁지대와 안전지대를 표시.
				if(0 >= index)
					wsprintf(stChannelIndex, "%d(%s)", pChannel->ChannelIndex,STRMSG_C_061106_0100);
				else
					wsprintf(stChannelIndex, "%d(%s)", pChannel->ChannelIndex,STRMSG_C_061106_0101);
			}
			else
			{
				wsprintf(stChannelIndex, "%d(%s)", pChannel->ChannelIndex,STRMSG_C_061106_0101);
			}
		}

		if(pChannel->Crowdedness<30)
		{
			wsprintf(m_szChannelList[i],"%s",STRMSG_C_SERVER_0001);//"원활"
		}
		else if(pChannel->Crowdedness<60)
		{
			wsprintf(m_szChannelList[i],"%s",STRMSG_C_SERVER_0002);//"보통"
		}
		else
		{
			wsprintf(m_szChannelList[i],"%s",STRMSG_C_SERVER_0003);//"혼잡"
		}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
		SIZE Size = m_pFontChannelList[i]->GetStringSize( stChannelIndex );
		m_pFontChannelList[i]->DrawText(CHANNEL_LIST_LIST_START_X + 4 + ( ( CHANNEL_LIST_MAPNAME_SIZE - Size.cx ) / 2 ) ,
										CHANNEL_LIST_LIST_START_Y + 5 + ( CHANNEL_LIST_LINE_INTERVAL + CHANNEL_LIST_LINE_GAB ) * i + CHANNEL_LIST_TEXT_Y,
										m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
										stChannelIndex,0L);
		Size = m_pFontChannelList[i]->GetStringSize( m_szChannelList[i] );
		m_pFontChannelList[i]->DrawText( CHANNEL_LIST_LIST_START_X + 4 + CHANNEL_LIST_MAPNAME_SIZE + 4 + ( ( CHANNEL_LIST_CHANNEL_SIZE - Size.cx ) / 2 ),
										CHANNEL_LIST_LIST_START_Y + 5 + (CHANNEL_LIST_LINE_INTERVAL + 2) * i + CHANNEL_LIST_TEXT_Y,
										m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
										m_szChannelList[i],0L);
#else
		m_pFontChannelList[i]->DrawText(CHANNEL_LIST_LIST_START_X+1,
			CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*i+CHANNEL_LIST_TEXT_Y,
			m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
			stChannelIndex,0L);
		m_pFontChannelList[i]->DrawText(CHANNEL_LIST_LIST_START_X+111,
			CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*i+CHANNEL_LIST_TEXT_Y,
			m_pScroll->GetCurrentSelectWindowIndex() == i ? GUI_SELECT_FONT_COLOR : GUI_FONT_COLOR,
			m_szChannelList[i],0L);
#endif
		
	}
	m_pScroll->Render();
}

void CINFChannelList::Tick()
{

}

int CINFChannelList::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			if( pt.y > CHANNEL_LIST_OK_BUTTON_START_Y && 
				pt.y < CHANNEL_LIST_OK_BUTTON_START_Y + CHANNEL_LIST_OK_BUTTON_SIZE_Y &&
				pt.x > CHANNEL_LIST_OK_BUTTON_START_X && 
				pt.x < CHANNEL_LIST_OK_BUTTON_START_X+CHANNEL_LIST_OK_BUTTON_SIZE_X)
			{
				if(m_nButtonState != BUTTON_STATE_DOWN)
					// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					g_pInterface->m_pToolTip->SetNameToolTip(pt,"STRTOOLTIP52");
					// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
					m_nButtonState = BUTTON_STATE_UP;
			}
			else 
			{
// 				// 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
 				g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
// 				// end 2011. 1. 12 by jskim UI 이미지 버튼 툴팁 구현
				m_nButtonState = BUTTON_STATE_NORMAL;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			if( pt.y > CHANNEL_LIST_OK_BUTTON_START_Y && 
				pt.y < CHANNEL_LIST_OK_BUTTON_START_Y + CHANNEL_LIST_OK_BUTTON_SIZE_Y &&
				pt.x > CHANNEL_LIST_OK_BUTTON_START_X && 
				pt.x < CHANNEL_LIST_OK_BUTTON_START_X+CHANNEL_LIST_OK_BUTTON_SIZE_X)
			{
				m_nButtonState = BUTTON_STATE_DOWN;
			}
			else 
			{
				m_nButtonState = BUTTON_STATE_NORMAL;
			}
//			if( pt.x > CHANNEL_LIST_LIST_START_X && 
//				pt.x < CHANNEL_LIST_LIST_START_X + CHANNEL_LIST_LIST_SIZE_X &&
//				pt.y > CHANNEL_LIST_LIST_START_Y && 
//				pt.y < CHANNEL_LIST_LIST_START_Y + CHANNEL_LIST_LINE_INTERVAL*CHANNEL_LIST_NUMBER)
//			{
//				int i = (pt.y-CHANNEL_LIST_LIST_START_Y)/CHANNEL_LIST_LINE_INTERVAL;
//				if(i>=0 && i<CHANNEL_LIST_NUMBER)
//				{
//					if(m_vecChannelList.size() >= i + m_nCurrentScrollNumber)
//					{
//						m_nCurrentSelectIndex = i;
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
			if( pt.y > CHANNEL_LIST_OK_BUTTON_START_Y && 
				pt.y < CHANNEL_LIST_OK_BUTTON_START_Y + CHANNEL_LIST_OK_BUTTON_SIZE_Y &&
				pt.x > CHANNEL_LIST_OK_BUTTON_START_X && 
				pt.x < CHANNEL_LIST_OK_BUTTON_START_X+CHANNEL_LIST_OK_BUTTON_SIZE_X)
			{
				if(m_nButtonState == BUTTON_STATE_DOWN)
				{
					OnOkButtonClicked();
				}
				m_nButtonState = BUTTON_STATE_UP;
			}
			else 
			{
				m_nButtonState = BUTTON_STATE_NORMAL;
			}
		}
		break;
 	case WM_LBUTTONDBLCLK:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
			if( pt.x > CHANNEL_LIST_LIST_START_X &&
				pt.x < CHANNEL_LIST_LIST_START_X + CHANNEL_LIST_LIST_SIZE_X &&
				pt.y > CHANNEL_LIST_LIST_START_Y + CHANNEL_LIST_LINE_GAB &&
				pt.y < CHANNEL_LIST_LIST_START_Y + ( CHANNEL_LIST_LINE_INTERVAL + CHANNEL_LIST_LINE_GAB ) * CHANNEL_LIST_NUMBER )
#else
			if( pt.x > CHANNEL_LIST_LIST_START_X &&
				pt.x < CHANNEL_LIST_LIST_START_X+CHANNEL_LIST_LIST_SIZE_X &&
				pt.y > CHANNEL_LIST_LIST_START_Y &&
				pt.y < CHANNEL_LIST_LIST_START_Y+CHANNEL_LIST_LINE_INTERVAL*CHANNEL_LIST_NUMBER)
#endif
			{
				OnOkButtonClicked();
			}

		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFChannelList::OnOkButtonClicked()
{
//	int index = m_nCurrentScrollNumber + m_nCurrentSelectIndex;
//	if(m_nCurrentSelectIndex >= 0 && 
//		index >= 0 && 
//		index < m_vecChannelList.size())
//	{
//		m_msgEventSelectChannel.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
//		m_msgEventSelectChannel.MapChannelIndex.ChannelIndex = m_vecChannelList[index]->ChannelIndex;
//		g_pD3dApp->m_pFieldWinSocket->SendMsg(T_FC_EVENT_SELECT_CHANNEL, (char*)&m_msgEventSelectChannel, sizeof(m_msgEventSelectChannel));
//
//		DBGOUT("필드서버 상황 : 채널 선택 전송[Map:%d][Channel:%d]\n", 
//			m_msgEventSelectChannel.MapChannelIndex.MapIndex,
//			m_msgEventSelectChannel.MapChannelIndex.ChannelIndex );
//	}
	int nCurrentSelectRealIndex = m_pScroll->GetCurrentSelectDataIndex();
	if( nCurrentSelectRealIndex >= 0 && 
		nCurrentSelectRealIndex < m_vecChannelList.size())
	{
		m_msgEventSelectChannel.ClientIndex = g_pShuttleChild->m_myShuttleInfo.ClientIndex;
		m_msgEventSelectChannel.MapChannelIndex.ChannelIndex = m_vecChannelList[nCurrentSelectRealIndex]->ChannelIndex;
		// 내가 파티 마스터라면 MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY 를 보낸다.
//		if(g_pShuttleChild->m_infoParty.bPartyType == _PARTYMASTER)
		if(g_pShuttleChild->m_pClientParty->GetPartyInfo().bPartyType == _PARTYMASTER)
		{
			g_pShuttleChild->m_pClientParty->FSendEventSelectChannelWithParty(m_msgEventSelectChannel);
//			INIT_MSG_WITH_BUFFER(MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY, T_FC_EVENT_SELECT_CHANNEL_WITH_PARTY, sMsg, buffer);
//			
//			sMsg->ClientIndex		= m_msgEventSelectChannel.ClientIndex;
//			sMsg->WarpTargetIndex	= m_msgEventSelectChannel.WarpTargetIndex;
//			sMsg->MapChannelIndex	= m_msgEventSelectChannel.MapChannelIndex;
//			
//			int cont = 0;
//			for(int i=0;i<MAX_PARTY_NUMBER - 1;i++)
//			{
//				if(g_pShuttleChild->m_nCharactUniqueNumber[i])
//				{
//					memcpy(buffer+MSG_SIZE(MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY)+i*sizeof(UINT), &g_pShuttleChild->m_nCharactUniqueNumber[i], sizeof(UINT));
//					cont++;
//				}
//			}
//			sMsg->nPartyMembers = cont;
//			
//			g_pD3dApp->m_pFieldWinSocket->Write(buffer, MSG_SIZE(MSG_FC_EVENT_SELECT_CHANNEL_WITH_PARTY) + cont*sizeof(UINT));
//			
//			if(g_pShuttleChild->m_dwState != _NORMAL && g_pShuttleChild->m_infoParty.bPartyType == _PARTYMASTER)
//			{
//				for(int i=0;i<MAX_PARTY_NUMBER-1;i++)
//				{
//					g_pShuttleChild->m_nCharactUniqueNumber[i] = 0;
//				}
//			}
//			DBGOUT("필드서버 상황 : 채널 선택 후 파티 워프시 파티원 정보를 서버로 전송[Map:%d][Channel:%d]\n", 
//			m_msgEventSelectChannel.MapChannelIndex.MapIndex,
//			m_msgEventSelectChannel.MapChannelIndex.ChannelIndex );		
		}
		else
		{			
			g_pD3dApp->m_pFieldWinSocket->SendMsg(T_FC_EVENT_SELECT_CHANNEL, (char*)&m_msgEventSelectChannel, sizeof(m_msgEventSelectChannel));

			DBGOUT("Field Server State : Send --> T_FC_EVENT_SELECT_CHANNEL [Map:%d][Channel:%d]\n", 
			m_msgEventSelectChannel.MapChannelIndex.MapIndex,
			m_msgEventSelectChannel.MapChannelIndex.ChannelIndex );
		}		
	}
}

