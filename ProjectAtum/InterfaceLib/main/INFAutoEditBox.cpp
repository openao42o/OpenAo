// INFAutoEditBox.cpp: implementation of the CINFAutoEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFGameMainChat.h"
#include "INFArenaScrollBar.h"

#include "INFEditBox.h"
#include "INFImageEx.h"	

#include "INFAutoEditBox.h"

// 스크롤바
#define INF_MIX_SCROLL_COUNT				MAX_SEARCH_ITEM
#define INF_MIX_SCROLL_X					(m_nBGPosX + 258)
#define INF_MIX_SCROLL_Y					(m_nBGPosY + 0)
#define INF_MIX_SCROLL_W					13
#define INF_MIX_SCROLL_H					140		
#define INF_MIX_SCROLL_WHELL_L				(m_nBGPosX + 35)
#define INF_MIX_SCROLL_WHELL_T				(m_nBGPosY + 0)
#define INF_MIX_SCROLL_WHELL_R				(m_nBGPosX + 600)
#define INF_MIX_SCROLL_WHELL_B				(m_nBGPosY + 140)

#define INF_MIX_SCROLL_BALL_L				(m_nBGPosX + 158)		
#define INF_MIX_SCROLL_BALL_T				(m_nBGPosY + 0)	
#define	INF_MIX_SCROLL_BALL_R				(m_nBGPosX + 600)
#define INF_MIX_SCROLL_BALL_B				(m_nBGPosY + 140)	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFAutoEditBox::CINFAutoEditBox()
{
	m_pImgElementBG = NULL;
	m_pFontItem = NULL;

	// by mspark, 인게임 조합
	m_pImgElementBG = NULL;
	m_pFontItem = NULL;
	
	m_nSelect = -1;
	m_nRenderSelect = -1;
	
	m_bItemShow = FALSE;
	m_nItemSize = 0;
	
	memset(&m_rtElementArea,0x00,sizeof(RECT));
// 	int nCnt=0;
// 	for(nCnt=0; nCnt<MAX_SEARCH_ITEM; nCnt++)
// 	{
// 		memset(m_szListItem[nCnt],0x00, 512);
// 	}
	memset(m_szBlankBuff, 0x00 ,512);
	
	m_nBGPosX = m_nBGPosY = 0;
	m_nBGPosWidth = m_nBGPosHeight = 0;
	
	m_pImgHeader = NULL;			
	m_pImgSelItem = NULL;			
	m_pImgBottom = NULL;

	m_pScroll = NULL;
	m_vecComboItem.clear();

	m_strListString = "";

}

CINFAutoEditBox::~CINFAutoEditBox()
{
	util::del(m_pImgHeader);
	util::del(m_pImgSelItem);
	util::del(m_pImgBottom);
	util::del(m_pScroll);

}

HRESULT CINFAutoEditBox::InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, 
											BOOL bCullText/*=FALSE*/, int nMouseHeight/*=-1*/)
{
	HRESULT hr =  CINFEditBox::InitDeviceObjects(nFontHeight, ptEditPos, nStrWidth, bUseCap, nCap, 
											bCullText, nMouseHeight);
	// by mspark, 인게임 조합
	char buf[64];
	DataHeader	* pDataHeader = NULL;
	{
		wsprintf(buf, "reci_serL2");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(NULL == m_pImgElementBG && pDataHeader)
		{
			m_pImgElementBG = new CINFImageEx;			
			m_pImgElementBG->InitDeviceObjects(pDataHeader);
		}
	}
	if(NULL == m_pFontItem)
	{
		m_pFontItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, FALSE,1024,32);
		m_pFontItem->InitDeviceObjects(g_pD3dDev);
	}

	{
		wsprintf(buf, "reci_serL1");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(NULL == m_pImgHeader && pDataHeader)
		{
			m_pImgHeader = new CINFImageEx;			
			m_pImgHeader->InitDeviceObjects(pDataHeader);
		}
	}
	{
		wsprintf(buf, "reci_serS");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(NULL == m_pImgSelItem && pDataHeader)
		{
			m_pImgSelItem = new CINFImageEx;			
			m_pImgSelItem->InitDeviceObjects(pDataHeader);
		}
	}
	{
		wsprintf(buf, "reci_serL3");
		pDataHeader = g_pGameMain->FindResource(buf);
		
		if(NULL == m_pImgBottom && pDataHeader)
		{
			m_pImgBottom = new CINFImageEx;			
			m_pImgBottom->InitDeviceObjects(pDataHeader);
		}
	}
	if(NULL == m_pScroll)
	{
		m_pScroll = new CINFArenaScrollBar;
		m_pScroll->InitDeviceObjects(INF_MIX_SCROLL_COUNT,"c_scrlb");
		m_pScroll->InitDeviceObjects_Border("scr01", "scr02", "scr03"); // 2013-04-08 by bhsohn 인게임 조합창 추가 구현
		m_pScroll->SetMaxItem(1);
	}
	
	// by mspark, 인게임 조합
	return hr;
}

HRESULT CINFAutoEditBox::RestoreDeviceObjects()
{
	HRESULT hr = CINFEditBox::RestoreDeviceObjects();
	// by mspark, 인게임 조합
	if(m_pImgElementBG)
	{
		m_pImgElementBG->RestoreDeviceObjects();		
	}
	if(m_pFontItem)
	{
		m_pFontItem->RestoreDeviceObjects();
	}
	// by mspark, 인게임 조합	
	if(m_pImgHeader)
	{
		m_pImgHeader->RestoreDeviceObjects();
	}
	if(m_pImgSelItem)
	{
		m_pImgSelItem->RestoreDeviceObjects();
	}
	if(m_pImgBottom)
	{
		m_pImgBottom->RestoreDeviceObjects();
	}
	if(m_pScroll)
	{
		m_pScroll->RestoreDeviceObjects();
	}	
	return hr;
}

HRESULT CINFAutoEditBox::DeleteDeviceObjects()
{
	HRESULT hr = CINFEditBox::DeleteDeviceObjects();
	// by mspark, 인게임 조합
	if(m_pImgElementBG)
	{
		m_pImgElementBG->DeleteDeviceObjects();		
		util::del(m_pImgElementBG);		
	}
	if(m_pFontItem)
	{
		m_pFontItem->DeleteDeviceObjects();
		util::del(m_pFontItem);
	}
	// by mspark, 인게임 조합
	if(m_pImgHeader)
	{
		m_pImgHeader->DeleteDeviceObjects();
		util::del(m_pImgHeader);
	}
	if(m_pImgSelItem)
	{
		m_pImgSelItem->DeleteDeviceObjects();
		util::del(m_pImgSelItem);
	}
	if(m_pImgBottom)
	{
		m_pImgBottom->DeleteDeviceObjects();
		util::del(m_pImgBottom);
	}
	if(m_pScroll)
	{
		m_pScroll->DeleteDeviceObjects();
		util::del(m_pScroll);
	}
	return hr;
}

HRESULT CINFAutoEditBox::InvalidateDeviceObjects()
{
	HRESULT hr = CINFEditBox::InvalidateDeviceObjects();
	// by mspark, 인게임 조합
	if(m_pImgElementBG)
	{
		m_pImgElementBG->InvalidateDeviceObjects();
	}
	if(m_pFontItem)
	{
		m_pFontItem->InvalidateDeviceObjects();
	}
	// by mspark, 인게임 조합
	if(m_pImgHeader)
	{
		m_pImgHeader->InvalidateDeviceObjects();		
	}
	if(m_pImgSelItem)
	{
		m_pImgSelItem->InvalidateDeviceObjects();		
	}
	if(m_pImgBottom)
	{
		m_pImgBottom->InvalidateDeviceObjects();		
	}
	if(m_pScroll)
	{
		m_pScroll->InvalidateDeviceObjects();
	}
	return hr;
}

void CINFAutoEditBox::Render()
{	
	CINFEditBox::Render();

	if(!m_pImgElementBG)
	{
		return;
	}
	
	if(!m_bItemShow)
	{
		return;
	}
	if(0 == m_vecComboItem.size())
	{
		return;
	}

	int nWndHeight = 0;
		
	int nStartItemY = m_nBGPosY - 4;
	if(m_pImgHeader)
	{
		m_pImgHeader->Move(m_nBGPosX - 6, m_nBGPosY - 4);
		m_pImgHeader->Render();
		nWndHeight += 2;
	}
	vector<string>::iterator it = m_vecComboItem.begin();
	int nPos=0;

	while(it != m_vecComboItem.end())
	{		
		if(nPos >= m_pScroll->GetScrollStep())
		{
			break;
		}
		nPos++;
		it++;
	}

	int nLen = 0;
	while(it != m_vecComboItem.end())
	{
		if(nLen >= MAX_SEARCH_ITEM)
		{
			break;
		}
		std::string strTmp = (*it);
		// 배경부터 쭉그린다.		
		{
			m_pImgElementBG->Move(m_nBGPosX - 6, m_nBGPosY - 2 + (m_nBGPosHeight * nLen));
			m_pImgElementBG->Render();
		}
		if(m_nRenderSelect == nLen)
		{
			m_pImgSelItem->Move(m_nBGPosX - 6, m_nBGPosY - 2 + (m_nBGPosHeight * nLen));
			m_pImgSelItem->Render();
		}
		
		m_pFontItem->DrawText(m_nBGPosX, 
			m_rtElementArea.top - 2 + (m_rtElementArea.bottom * nLen),
			GUI_FONT_COLOR_Y,
			(char*)strTmp.c_str());

//		nWndHeight += m_pImgElementBG->GetImgSize().y;
		nWndHeight += m_nBGPosHeight;

		it++;
		nLen++;
		nPos++;
	}
	
	if(m_pImgBottom)
	{				
		m_pImgBottom->Move(m_nBGPosX - 6, m_nBGPosY +(m_nBGPosHeight * nLen) - 2);
		m_pImgBottom->Render();

		nWndHeight+=2;
	}	

	RenderScroll(nWndHeight-25); // 25픽셀은 막대 크기
}

void CINFAutoEditBox::RenderScroll(int nWndHeight)
{
	if(!m_bItemShow)
	{
		return;
	}
	else if(INF_MIX_SCROLL_COUNT >= m_vecComboItem.size())
	{
		return;
	}

	if(!m_pScroll)
	{
		return;
	}
	
	{
		RECT rtWhell;
		rtWhell.left		= INF_MIX_SCROLL_WHELL_L;
		rtWhell.top			= INF_MIX_SCROLL_WHELL_T;
		
		rtWhell.right		= INF_MIX_SCROLL_WHELL_R;
		rtWhell.bottom		= rtWhell.top+nWndHeight;		
		m_pScroll->SetMouseWhellRect(rtWhell);
	}
	
	m_pScroll->SetPosition(INF_MIX_SCROLL_X,INF_MIX_SCROLL_Y,INF_MIX_SCROLL_W,nWndHeight);
	
	RECT rtMousePos;
	
	rtMousePos.left		= INF_MIX_SCROLL_BALL_L;
	rtMousePos.top		= INF_MIX_SCROLL_BALL_T;		
	rtMousePos.right	= INF_MIX_SCROLL_BALL_R;
	rtMousePos.bottom	= rtMousePos.top+nWndHeight;		
	m_pScroll->SetMouseBallRect(rtMousePos);
	
	m_pScroll->Render();

}

void CINFAutoEditBox::Render(int nScroll, int nOnePageLine)
{
	CINFEditBox::Render(nScroll, nOnePageLine);
}

void CINFAutoEditBox::EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus)
{
	CINFEditBox::EnableEdit(bEnableEdit, bOnlyFocus);
	
	m_bItemShow = FALSE;	
}

// 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
BOOL CINFAutoEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep /*= NULL*/, BOOL bAddString/*=FALSE*/, BOOL bUseBackSpace/*=TRUE*/)
{
	BOOL bRtn  = CINFEditBox::WndProc(uMsg, wParam, lParam, pLienStep, bAddString, bUseBackSpace); // 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리

	if(m_vecComboItem.size() > INF_MIX_SCROLL_COUNT )
	{
		if(m_pScroll && m_pScroll->WndProc(uMsg, wParam, lParam) == INF_MSGPROC_BREAK)
		{
			return TRUE;
		}		
	}
	
	
	return bRtn;
}

// 버튼 클릭 
BOOL CINFAutoEditBox::OnLButtonDown(POINT ptPos)
{
	// by mspark, 인게임 조합
	if(m_bItemShow)
	{
		if(ptPos.x > m_rtElementArea.left
			&& ptPos.x < m_rtElementArea.left + m_rtElementArea.right
			&& ptPos.y > m_rtElementArea.top
			&& ptPos.y < m_rtElementArea.top + (m_rtElementArea.bottom * m_nItemSize))
		{
			BOOL bClick = m_pScroll->IsMouseBallPos(ptPos);
			if(bClick)
			{
				m_pScroll->SetMouseMoveMode(TRUE);
				return -1;
			}						

			float fSelect = (ptPos.y - m_rtElementArea.top) / m_rtElementArea.bottom;
			if(fSelect < 0)
				return -1;
			m_nSelect = (UINT)fSelect;
			if(m_pScroll)
			{
				m_nSelect += m_pScroll->GetScrollStep();
			}			
			GetItem(m_nSelect);
			m_bItemShow = FALSE;
			return 1;
		}

	}
	// by mspark, 인게임 조합

	return CINFEditBox::OnLButtonDown(ptPos);
}

BOOL CINFAutoEditBox::OnLButtonUp(POINT ptPos)
{
	BOOL bClick = m_pScroll->GetMouseMoveMode();
	if(bClick)
	{
		m_pScroll->SetMouseMoveMode(FALSE);
		return TRUE;
	}			
	return FALSE;
}


BOOL CINFAutoEditBox::OnMouseMove(POINT ptPos)
{	
	if(m_bItemShow
		&& ptPos.x > m_rtElementArea.left
		&& ptPos.x < m_rtElementArea.left + m_rtElementArea.right
		&& ptPos.y > m_rtElementArea.top
		&& ptPos.y < m_rtElementArea.top + (m_rtElementArea.bottom * m_nItemSize))
	{
		if(m_pScroll->GetMouseMoveMode())
		{
			if(FALSE == m_pScroll->IsMouseScrollPos(ptPos))
			{
				m_pScroll->SetMouseMoveMode(FALSE);
			}
			else
			{
				m_pScroll->SetScrollPos(ptPos);
				return FALSE;
			}
		}			

		float fSelect = (ptPos.y - m_rtElementArea.top) / m_rtElementArea.bottom;
		if(fSelect < 0)
		{
			m_nRenderSelect = -1;
			return FALSE;
		}
		m_nRenderSelect = (UINT)fSelect;
		
		return TRUE;
	}
	
	
	return FALSE;
}

BOOL CINFAutoEditBox::OnMouseWheel(POINT ptPos, WPARAM wParam, LPARAM lParam)
{
	if(!m_bItemShow)
	{
		return FALSE;
	}
	{
		BOOL bClick = m_pScroll->IsMouseWhellPos(ptPos);
		if(bClick)		
		{			
			m_pScroll->OnMouseWheel(wParam, lParam);	
			return TRUE;
		}
	}
	return FALSE;
}

void CINFAutoEditBox::Tick()
{
	CINFEditBox::Tick();
}

// by mspark, 인게임 조합
INT	CINFAutoEditBox::AddElement(char* c)
{	
// 	strncpy(m_szListItem[m_nItemSize], c, strlen(c)+1);
// 	
// 	m_nItemSize++;
// 	if(m_nItemSize >= MAX_SEARCH_ITEM)
// 	{
// 		m_nItemSize = MAX_SEARCH_ITEM-1;
// 	}
	std::string strItem(c);
	m_vecComboItem.push_back(strItem);

	return m_nItemSize;
}

void CINFAutoEditBox::ItemAddDone()
{
	m_nItemSize = m_vecComboItem.size();
	m_pScroll->SetMaxItem(m_nItemSize, FALSE);
}

void CINFAutoEditBox::ItemClear()
{
	m_nItemSize = 0;
// 	int  nCnt=0;
// 	for(nCnt=0; nCnt<MAX_SEARCH_ITEM;nCnt++)
// 	{
// 		memset(m_szListItem[nCnt],0x00, 512);
// 	}
	m_vecComboItem.clear();
}

void CINFAutoEditBox::SetElementArea(int cx,int cy,int cw,int ch)
{
	m_rtElementArea.left = cx;
	m_rtElementArea.top = cy;
	m_rtElementArea.bottom = ch;
	m_rtElementArea.right = cw;
	
	m_nBGPosX = m_rtElementArea.left;
	m_nBGPosY = m_rtElementArea.top;
	m_nBGPosWidth = m_rtElementArea.right;
	m_nBGPosHeight = m_rtElementArea.bottom;
}

char *CINFAutoEditBox::GetItem(UINT num)
{
	if(0 == m_nItemSize)
	{
		return m_szBlankBuff;
	}
// 	if(num >= m_nItemSize)
// 	{
// 		return m_szBlankBuff;
// 	}
// 	return m_szListItem[num];

	if(num >= m_vecComboItem.size() )
	{
		return m_szBlankBuff;
	}
	return (char*)m_vecComboItem[num].c_str();
}

void CINFAutoEditBox::SetSelect(INT nSelect)
{
	m_nSelect = nSelect;
}

INT	CINFAutoEditBox::GetSelect()
{
	return m_nSelect;
}

void CINFAutoEditBox::SetItemShow(bool bItemShow)
{
	m_bItemShow = bItemShow;
}

void CINFAutoEditBox::SetBGPos(int nBGPosX, int nBGPosY, int nWidth ,int nHeight)
{
	m_nBGPosX = nBGPosX;
	m_nBGPosY = nBGPosY;
	m_nBGPosWidth = nWidth ;
	m_nBGPosHeight = nHeight;	
}
// by mspark, 인게임 조합

void CINFAutoEditBox::SetAutoEditBoxString(char* pEditString)
{
	std::string	 strListString(pEditString);
	m_strListString = strListString;
}

char* CINFAutoEditBox::GetAutoEditBoxString()
{
	if(!m_strListString.size())
	{
		return m_szBlankBuff;
	}
	return (char*)m_strListString.c_str();
}