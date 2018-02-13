// INFImageListBox.cpp: implementation of the CINFImageListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "D3DHanFont.h"

#include "INFImageListBox.h"

#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn 태국 버전 추가
	#define STRING_CULL ::StringCullingUserData_ToBlank
#else
	#define STRING_CULL ::StringCullingUserDataEx	
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	SCROLL_BALL_WIDTH					11
#define	SCROLL_BALL_SCROLL_CAP				100

CINFImageListBox::CINFImageListBox()
{
	m_vecFontTxt.clear();
	m_vecTitleText.clear();
	m_vecEditText.clear();

	m_pINFScrollBar = NULL;
}

CINFImageListBox::~CINFImageListBox()
{
	CINFImageListBox::DeleteDeviceObjects();
}


HRESULT CINFImageListBox::InitDeviceObjects(int nOnePageLineCnt, DWORD *i_pFlag, int *i_nFontSize)
{
	int nCnt = 0;	
	for(nCnt = 0;nCnt < nOnePageLineCnt;nCnt++)
	{		
		structEditBoxFont* pEditBoxFont = new structEditBoxFont;
		pEditBoxFont->nLine = nCnt;
		pEditBoxFont->fPosX = 0;
		pEditBoxFont->fPosY = 0;
		pEditBoxFont->fPosY = 0;
		memset(pEditBoxFont->szEditText,0x00, 1024);
		pEditBoxFont->pEditFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), i_nFontSize[nCnt], i_pFlag[nCnt], TRUE,1024,32);
		pEditBoxFont->pEditFont->InitDeviceObjects(g_pD3dDev);

		m_vecFontTxt.push_back(pEditBoxFont);		
	}
	{
		char  szScBall[30];	
		if(NULL == m_pINFScrollBar)
		{
			m_pINFScrollBar = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pINFScrollBar->InitDeviceObjects(nOnePageLineCnt, szScBall);
	}
	
	return S_OK ;
}

HRESULT CINFImageListBox::RestoreDeviceObjects()
{
	vector<structEditBoxFont*>::iterator itFont = m_vecFontTxt.begin();
	while(itFont != m_vecFontTxt.end())
	{
		structEditBoxFont*	pEditBoxFont = (*itFont);
		if(pEditBoxFont)
		{
			pEditBoxFont->pEditFont->RestoreDeviceObjects();
		}
		itFont++;
	}
	m_pINFScrollBar->RestoreDeviceObjects();		

	return S_OK ;
}

HRESULT CINFImageListBox::DeleteDeviceObjects()
{
	{
		vector<structEditBoxFont*>::iterator itFont = m_vecFontTxt.begin();
		while(itFont != m_vecFontTxt.end())
		{
			structEditBoxFont*	pEditBoxFont = (*itFont);
			if(pEditBoxFont)
			{
				pEditBoxFont->pEditFont->DeleteDeviceObjects();
				util::del(pEditBoxFont->pEditFont);
			}
			util::del(pEditBoxFont);
			itFont++;
		}
		m_vecFontTxt.clear();
	}	
	if(m_pINFScrollBar)
	{
		m_pINFScrollBar->DeleteDeviceObjects();	
		util::del(m_pINFScrollBar);
	}
	return S_OK ;
}

HRESULT CINFImageListBox::InvalidateDeviceObjects()
{
	vector<structEditBoxFont*>::iterator itFont = m_vecFontTxt.begin();
	while(itFont != m_vecFontTxt.end())
	{
		structEditBoxFont*	pEditBoxFont = (*itFont);
		if(pEditBoxFont)
		{
			pEditBoxFont->pEditFont->InvalidateDeviceObjects();
		}
		itFont++;
	}
	m_pINFScrollBar->InvalidateDeviceObjects();
	
	return S_OK ;
}

void CINFImageListBox::Render()
{		
	// 타이틀 랜더링 
	RenderTitle();	
	// 미션 세부 설명 
	RenderNormalString();
			
	m_pINFScrollBar->Render();
}
void CINFImageListBox::RenderTitle()
{
	int nCnt = 0;
	// 미션 제목	
	int nMissionNameLineCount = m_vecTitleText.size();
	{		
		structEditBoxFont*	pTitleFont = GetListFont(0);	
		for(nCnt = 0; nCnt < nMissionNameLineCount; nCnt++)
		{
			if(NULL == pTitleFont)
			{
				break;
			}
			CD3DHanFont* pSelEditFont = pTitleFont->pEditFont;	
			char* pTxt = (char*)m_vecTitleText[nCnt].c_str();
			if(NULL == pTxt)
			{
				continue;
			}
			pSelEditFont->DrawText( pTitleFont->fPosX, 
									pTitleFont->fPosY,
									pTitleFont->dwColor, 
									pTxt);						
		}
	}
}


void CINFImageListBox::RenderNormalString()
{
	int nCnt = 0;
	int nLineCount = 0;		
	vector<string>::iterator it = m_vecEditText.begin();
	for(nCnt = 0; nCnt < m_pINFScrollBar->GetScrollStep();nCnt++)
	{
		if(it == m_vecEditText.end())
		{
			break;
		}
		it++;
	}
	while(it != m_vecEditText.end())
	{
		structEditBoxFont*	pTitleFont = GetListFont(nLineCount+1);	
		if(NULL == pTitleFont)
		{
			it++;
			continue;
		}
		CD3DHanFont* pSelEditFont = pTitleFont->pEditFont;	
		char* pTxt = (char*)(*it).c_str();
		if(NULL == pTxt)
		{				
			it++;
			continue;
		}
		pSelEditFont->DrawText( pTitleFont->fPosX, 
			pTitleFont->fPosY,
			pTitleFont->dwColor, 
			pTxt);			
		nLineCount++;
		it++;
	}		
}

void CINFImageListBox::SetStringCull(int i_nLine, char* pTxt, vector<string> *vecStorage, int i_nMaxLen)
{		
	structEditBoxFont*	pSelEditBoxFont = GetListFont(i_nLine);	
	if(NULL == pSelEditBoxFont)
	{
		return;
	}	
	
	STRING_CULL(pTxt, i_nMaxLen, vecStorage, pSelEditBoxFont->pEditFont, TRUE);
}

void CINFImageListBox::SetPosition(int i_nLine, float fPosX, float fPosY, DWORD dwColor)
{
	structEditBoxFont*	pSelEditBoxFont = GetListFont(i_nLine);	
	if(NULL == pSelEditBoxFont)
	{
		return;
	}	
	pSelEditBoxFont->fPosX		= fPosX;
	pSelEditBoxFont->fPosY		= fPosY;
	pSelEditBoxFont->dwColor	= dwColor;	
	
}
void CINFImageListBox::SetScrollPos(float fPosX, float fPosY, float fWidth, float fHeight, 
									float fScrollPosX, float fScrollPosY, float fScrollWidth, float fScrollHeight)
{
	POINT ptScroll;
	RECT rcMouseWhell, rcMousePos;
	
	// 휠 위치지정 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	rcMouseWhell.left		= fPosX ;
	rcMouseWhell.top		= fPosY;
	rcMouseWhell.right		= rcMouseWhell.left + fWidth;
	rcMouseWhell.bottom		= rcMouseWhell.top + fHeight;
	
	ptScroll = m_pINFScrollBar->GetImgBkSize();
	
	// 마우스 위치 지정
	rcMousePos.left			= fPosX + fScrollPosX;
	rcMousePos.top			= fPosY + fScrollPosY;
	rcMousePos.right		= rcMousePos.left + fScrollWidth;
	rcMousePos.bottom		= rcMousePos.top + fScrollHeight;

	m_pINFScrollBar->SetPosition( rcMousePos.left, rcMousePos.top, fScrollWidth, fScrollHeight - 23 );
	m_pINFScrollBar->SetMouseWhellRect(rcMouseWhell);			
	
// 	rcMousePos.bottom		= rcMousePos.top + ptScroll.y + SCROLL_BALL_SCROLL_CAP;
// 	rcMousePos.top			-= SCROLL_BALL_SCROLL_CAP;
// 	rcMousePos.right		= rcMousePos.left + SCROLL_BALL_SCROLL_CAP;
// 	rcMousePos.left			-= SCROLL_BALL_SCROLL_CAP;					
	
	m_pINFScrollBar->SetMouseBallRect(rcMousePos);	
#else 
	rcMouseWhell.left		= fPosX ;
	rcMouseWhell.top		= fPosY;
	rcMouseWhell.right		= rcMouseWhell.left + fScrollWidth;
	rcMouseWhell.bottom		= rcMouseWhell.top + fScrollWidth;
	
	
	// 마우스 위치 지정
	rcMousePos.left			= fPosX + fScrollPosX;
	rcMousePos.top			= fPosY + fScrollPosY;
	rcMousePos.right		= rcMousePos.left;
	rcMousePos.bottom		= rcMousePos.top;

	m_pINFScrollBar->SetPosition(rcMousePos.left, rcMousePos.top, SCROLL_BALL_WIDTH, fHeight);
	m_pINFScrollBar->SetMouseWhellRect(rcMouseWhell);			
	
	ptScroll = m_pINFScrollBar->GetImgBkSize();
	
	rcMousePos.bottom		= rcMousePos.top + ptScroll.y + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.top			-= SCROLL_BALL_SCROLL_CAP;
	rcMousePos.right		= rcMousePos.left + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.left			-= SCROLL_BALL_SCROLL_CAP;					
	
	m_pINFScrollBar->SetMouseBallRect(rcMousePos);
#endif
}

structEditBoxFont*	CINFImageListBox::GetListFont(int i_nLine)
{	
	vector<structEditBoxFont*>::iterator itFont = m_vecFontTxt.begin();
	while(itFont != m_vecFontTxt.end())
	{
		structEditBoxFont*	pEditBoxFont = (*itFont);
		if(i_nLine == pEditBoxFont->nLine )
		{			
			return (*itFont);			
		}
		itFont++;
	}
	return NULL;

}

void CINFImageListBox::ResetContent()
{
	vector<structEditBoxFont*>::iterator itFont = m_vecFontTxt.begin();
	while(itFont != m_vecFontTxt.end())
	{		
		wsprintf((*itFont)->szEditText, " ");
		itFont++;
	}
	m_vecTitleText.clear();
	m_vecEditText.clear();
}

void CINFImageListBox::SetTitleText(vector<string>	i_vecTitleText)
{
	m_vecTitleText	= i_vecTitleText;
}


void CINFImageListBox::SetEditText(vector<string>	i_vecEditText)
{
	m_vecEditText	= i_vecEditText;
}

void CINFImageListBox::UpdateScrollPos()
{
	int nMaxItem = m_vecEditText.size();
	m_pINFScrollBar->SetMaxItem(nMaxItem);
}

BOOL CINFImageListBox::OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam)
{
	BOOL bClick = m_pINFScrollBar->IsMouseWhellPos(pt);
	if(bClick)		
	{			
		m_pINFScrollBar->OnMouseWheel(wParam, lParam);			
	}
	return bClick;
}
BOOL CINFImageListBox::OnMouseMove(POINT pt)
{
	if(m_pINFScrollBar->GetMouseMoveMode())
	{
		if(FALSE == m_pINFScrollBar->IsMouseScrollPos(pt))
		{
			m_pINFScrollBar->SetMouseMoveMode(FALSE);
			return FALSE;			
		}
		m_pINFScrollBar->SetScrollPos(pt);		
		return TRUE;			
	}
	return FALSE;	
}

BOOL CINFImageListBox::OnLButtonDown(POINT pt)
{		
	{
		BOOL bClick = m_pINFScrollBar->IsMouseBallPos(pt);
		if(bClick)		
		{
			m_pINFScrollBar->SetMouseMoveMode(TRUE);
			return TRUE;
		}		
	}
	return FALSE;
}

BOOL CINFImageListBox::OnLButtonUp(POINT pt)
{
	BOOL bClick = m_pINFScrollBar->GetMouseMoveMode();
	if(bClick)		
	{
		m_pINFScrollBar->SetMouseMoveMode(FALSE);			
	}		
	return bClick;
}