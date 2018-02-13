// INFScrollBar.cpp: implementation of the CINFScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFScrollBar.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"											 // 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFScrollBar::CINFScrollBar(CAtumNode *pParent, 
							 int nStartX, 
							 int nStartY, 
							 int nLineLength, 
							 int nNumberOfWindowList,
							 int nScrollBarSizeX,
							 int nScrollBarSizeY,
							 char *strIconImg,
							 int nScrollStyle)
{
	m_nStartX = nStartX;
	m_nStartY = nStartY;
	m_nScrollLineLength = nLineLength;
	m_nNumberOfWindowList = nNumberOfWindowList;
	m_pScrollBar = NULL;
	memset( m_pScrollLine, 0x00, sizeof(DWORD)*3);
	memset( &m_rcWheel, 0x00, sizeof(RECT));
	m_bLockMouse = FALSE;
	m_nNumberOfData = 0;
	m_nCurrentScrollIndex = 0;
	m_ptOldMouse.x = 0;
	m_ptOldMouse.y = 0;
	m_nCurrentSelectDataIndex = -1;
	m_nScrollSizeY = nScrollBarSizeY;
	m_nScrollSizeX = nScrollBarSizeX;
	m_nScrollStyle = nScrollStyle;
	memset(m_strScrollImgName, 0x00, 64);
	if(strIconImg != NULL)
		strncpy(m_strScrollImgName, strIconImg, strlen(strIconImg));

	SetCurrentScrollBar(nScrollStyle);
}

CINFScrollBar::~CINFScrollBar()
{
	util::del(m_pScrollBar);
	for(int i=0;i<3;i++)
	{
		util::del(m_pScrollLine[i]);
	}
}

void CINFScrollBar::Reset()
{
	m_bLockMouse = FALSE;
	m_nCurrentSelectDataIndex = -1;
	m_nCurrentScrollIndex = 0;
	m_ptOldMouse.x = 0;
	m_ptOldMouse.y = 0;
}

void CINFScrollBar::SetWheelRect(int nStartX, int nStartY, int nEndX, int nEndY)
{	
	m_rcWheel.left = nStartX; 
	m_rcWheel.top = nStartY; 
	m_rcWheel.right = nEndX; 
	m_rcWheel.bottom = nEndY; 
} 


HRESULT CINFScrollBar::InitDeviceObjects()
{
	DataHeader* pDataHeader = NULL;

	if(strlen(m_strScrollImgName)>0)
		pDataHeader = FindResource(m_strScrollImgName);
	else
		pDataHeader = FindResource("c_scrlb");

	if(pDataHeader)
	{
		m_pScrollBar = new CINFImageEx;
		m_pScrollBar->InitDeviceObjects(pDataHeader ) ;
	}
	
	for(int i=0;i<3;i++)
	{
		char buf[32];
		wsprintf(buf, "scrline%d",i);
		pDataHeader = FindResource(buf);
		m_pScrollLine[i] = new CINFImageEx;
		m_pScrollLine[i]->InitDeviceObjects(pDataHeader) ;	
	}
	return S_OK;
}

HRESULT CINFScrollBar::RestoreDeviceObjects()
{
	m_pScrollBar->RestoreDeviceObjects() ;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
	for(int i=0;i<3;i++)
	{
		m_pScrollLine[i]->RestoreDeviceObjects() ;	
	}
#endif
	return S_OK;
}

HRESULT CINFScrollBar::InvalidateDeviceObjects()
{
	m_pScrollBar->InvalidateDeviceObjects() ;	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
	for(int i=0;i<3;i++)
	{
		m_pScrollLine[i]->InvalidateDeviceObjects() ;	
	}
#endif
	return S_OK;
}

HRESULT CINFScrollBar::DeleteDeviceObjects()
{
	m_pScrollBar->DeleteDeviceObjects() ;	
	util::del(m_pScrollBar);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
	for(int i=0;i<3;i++)
	{
		m_pScrollLine[i]->DeleteDeviceObjects() ;	
		util::del(m_pScrollLine[i]);
	}
#endif
	return S_OK;
}

void CINFScrollBar::Tick()
{

}

#define LINE_INDEX_0_SIZE_X			3
#define LINE_INDEX_0_SIZE_Y			3
#define LINE_INDEX_2_SIZE_X			3
#define LINE_INDEX_2_SIZE_Y			3
#define SCROLL_BAR_SIZE_X			m_nScrollSizeX
#define SCROLL_BAR_SIZE_Y			m_nScrollSizeY // 30
#define SCROLL_LENGTH				(m_nScrollLineLength - SCROLL_BAR_SIZE_Y)		// 실제 스크롤되는 양
#define INTERVAL_PIXEL_PER_LINE		(m_nNumberOfData == 0 ? 0 : ((float)SCROLL_LENGTH/(float)m_nNumberOfData))	// 리스트박스 한라인에 의해 스크롤되는 픽셀량
//#define CURRENT_SCOLL_BAR_START_X	(m_nStartX-4)	//	4 = (11-3)/2, 스크롤바 가로 길이 11, 스크롤라인 가로길이 3
//#define CURRENT_SCOLL_BAR_START_Y	(m_nStartY+m_nCurrentScrollIndex*INTERVAL_PIXEL_PER_LINE)

//void CINFScrollBar::Render()
//{
//	if(m_nScrollStyle == INFSCROLL_TYPE_VERTICAL)
//	{
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
//#else
//		m_pScrollLine[0]->Move(m_nStartX, m_nStartY);
//		m_pScrollLine[0]->Render();
//#endif
//		m_pScrollLine[1]->Move(m_nStartX, m_nStartY+LINE_INDEX_0_SIZE_Y);
//		m_pScrollLine[1]->SetScale(1.0f, m_nScrollLineLength-(LINE_INDEX_0_SIZE_X+LINE_INDEX_2_SIZE_X) );
//		m_pScrollLine[1]->Render();
//		m_pScrollLine[2]->Move(m_nStartX, m_nStartY+m_nScrollLineLength-LINE_INDEX_2_SIZE_X);
//		m_pScrollLine[2]->Render();
//	}
	
//	SetCurrentScrollBar(m_nScrollStyle);
//	m_pScrollBar->Move(m_nCurrentScrollBarX,m_nCurrentScrollBarY);	
//	m_pScrollBar->Render();
//}

// 2012-02-27 by mspark, 스킬상점 스크롤 위치 라인위로 맞추기
void CINFScrollBar::Render(int nScrollX /*=0*/)
{
	if(m_nScrollStyle == INFSCROLL_TYPE_VERTICAL)
	{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
#else
		m_pScrollLine[0]->Move(m_nStartX, m_nStartY);
		m_pScrollLine[0]->Render();		
#endif
		m_pScrollLine[1]->Move(m_nStartX, m_nStartY+LINE_INDEX_0_SIZE_Y);
		m_pScrollLine[1]->SetScale(1.0f, m_nScrollLineLength-(LINE_INDEX_0_SIZE_X+LINE_INDEX_2_SIZE_X) );
		m_pScrollLine[1]->Render();
		m_pScrollLine[2]->Move(m_nStartX, m_nStartY+m_nScrollLineLength-LINE_INDEX_2_SIZE_X);
		m_pScrollLine[2]->Render();
	}
	
	SetCurrentScrollBar(m_nScrollStyle);
	//m_pScrollBar->Move(m_nCurrentScrollBarX,m_nCurrentScrollBarY);
	m_pScrollBar->Move(m_nCurrentScrollBarX+nScrollX,m_nCurrentScrollBarY);	
	m_pScrollBar->Render();
}
// end 2012-02-27 by mspark, 스킬상점 스크롤 위치 라인 맞추기

void CINFScrollBar::SetIncreaseCurrent(int nNum) 
{
	m_nCurrentScrollIndex = m_nCurrentScrollIndex+nNum; 
	if(m_nNumberOfData<m_nCurrentScrollIndex+nNum)
	{
		m_nCurrentScrollIndex = m_nNumberOfData;
	}
}

int CINFScrollBar::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			if(m_bLockMouse)
			{
				POINT pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				CheckMouseReverse(&pt);

				if(m_nScrollStyle == INFSCROLL_TYPE_VERTICAL)
				{
					if(INTERVAL_PIXEL_PER_LINE > 0)
					{
						int nMove = (pt.y - m_ptOldMouse.y)/INTERVAL_PIXEL_PER_LINE;
						if(nMove != 0)
						{
							m_nCurrentScrollIndex += nMove;
							if(m_nCurrentScrollIndex < 0 )
								m_nCurrentScrollIndex = 0;
							if(m_nCurrentScrollIndex > m_nNumberOfData)
								m_nCurrentScrollIndex = m_nNumberOfData;
							m_ptOldMouse = pt;
						}
						return INF_MSGPROC_BREAK;
					}
				}
				else
				{
					if(INTERVAL_PIXEL_PER_LINE > 0)
					{
						int nMove = (pt.x - m_ptOldMouse.x)/INTERVAL_PIXEL_PER_LINE;
						if(nMove != 0)
						{
							m_nCurrentScrollIndex += nMove;
							if(m_nCurrentScrollIndex < 0 )
								m_nCurrentScrollIndex = 0;
							if(m_nCurrentScrollIndex > m_nNumberOfData)
								m_nCurrentScrollIndex = m_nNumberOfData;
							m_ptOldMouse = pt;
						}
						return INF_MSGPROC_BREAK;
					}

				}
			}
			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			SetCurrentScrollBar(m_nScrollStyle);
			if( pt.x > m_nCurrentScrollBarX &&
				pt.x < m_nCurrentScrollBarX + SCROLL_BAR_SIZE_X &&
				pt.y > m_nCurrentScrollBarY &&
				pt.y < m_nCurrentScrollBarY + SCROLL_BAR_SIZE_Y)
			{
				m_bLockMouse = TRUE;
				m_ptOldMouse = pt;
				return INF_MSGPROC_BREAK;
			}
			if( pt.x > m_rcWheel.left &&
				pt.x < m_rcWheel.right &&
				pt.y > m_rcWheel.top &&
				pt.y < m_rcWheel.bottom)
			{
				int i = (pt.y-m_rcWheel.top)/((m_rcWheel.bottom-m_rcWheel.top)/m_nNumberOfWindowList);
				if(i>=0 && i<m_nNumberOfWindowList)
				{
					m_nCurrentSelectDataIndex = i+m_nCurrentScrollIndex;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			m_bLockMouse = FALSE;
		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if( pt.x > m_rcWheel.left &&
				pt.x < m_rcWheel.right &&
				pt.y > m_rcWheel.top &&
				pt.y < m_rcWheel.bottom)
			{
				if((int)wParam<0)
				{
					m_nCurrentScrollIndex += 1;
					if(m_nCurrentScrollIndex > m_nNumberOfData)
					{
						m_nCurrentScrollIndex = m_nNumberOfData;
					}
					return INF_MSGPROC_BREAK;
				}
				else
				{
					m_nCurrentScrollIndex -= 1;
					if(m_nCurrentScrollIndex < 0 )
					{
						m_nCurrentScrollIndex = 0;
					}
					return INF_MSGPROC_BREAK;
				}
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

void CINFScrollBar::SetNumberOfData(int nNum) 
{ 
	m_nNumberOfData = nNum-m_nNumberOfWindowList+1;
	if(m_nNumberOfData<0)
	{
		m_nNumberOfData = 0;
	}
}	//	데이타 갯수 변화가 있을 경우

void CINFScrollBar::SetCurrentScrollBar(int nScrollStyle)
{
	if(nScrollStyle == INFSCROLL_TYPE_VERTICAL)
	{
		m_nCurrentScrollBarX = (m_nStartX-4);
		m_nCurrentScrollBarY = (m_nStartY+m_nCurrentScrollIndex*INTERVAL_PIXEL_PER_LINE);
	}
	else
	{
		m_nCurrentScrollBarY = (m_nStartY-4);
		m_nCurrentScrollBarX = (m_nStartX+m_nCurrentScrollIndex*INTERVAL_PIXEL_PER_LINE);
	}
}


//////////////////////////////////////////////////////////////////////////
// 스크롤바 EX
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CINFScrollBarEX::CINFScrollBarEX(CGameData* pGameData, 
								 int * pParentStartX, 
								 int * pParentStartY, 
								 int nStartX, 
								 int nStartY,
								 int nScrollBarW,
								 int nScrollBarH,
								 int nMaxCount, 
								 int nMaxLine, 
								 int nLineLength)
{
	m_pGameData					= pGameData;

	m_nParentX					= pParentStartX;
	m_nParentY					= pParentStartY;
	m_nStartX					= nStartX;
	m_nStartY					= nStartY;
	m_nTopBtnSize				= 0;
	m_nBottomBtnSize			= 0;

	m_ImgScrollBar				= NULL;
	m_ImgScrollLine				= NULL;

	int i = 0;
	for(i = 0; i < 4; i++)
	{
		m_ImgScrollTopBtn[i]	= NULL;
		m_ImgScrollBottomBtn[i] = NULL;
	} 

	m_nScrollMaxCount			= nMaxCount;
	m_nScrollMaxLine			= nMaxLine;
	m_nScrollLineLength			= nLineLength;
	m_nScrollBarWidth			= nScrollBarW;
	m_nScrollBarHeight			= nScrollBarH;
	m_nScrollMoveLength			= m_nScrollLineLength - m_nScrollBarHeight;

	m_nScrollBtn				= 0;
	m_nScrollPosition			= 0;
	m_nScrollLine				= 0;

	m_byScrollTopBtnState		= SCROLL_BTN_NON;
	m_byScrollBottomBtnState	= SCROLL_BTN_NON;
	m_bScrollLock				= FALSE;

}
CINFScrollBarEX::~CINFScrollBarEX()
{
}

HRESULT CINFScrollBarEX::InitDeviceObjects(char * pScrollBar, char * pScrollLine, char * pScrollTop, char * pScrollBottom)
{
	int i = 0;
	char buf[16] = {0,};
	DataHeader	* pDataHeader;

	if(pScrollBar)
	{
		pDataHeader = FindResource(pScrollBar);
		if(pDataHeader)
		{
			m_ImgScrollBar = new CINFImageEx;
			m_ImgScrollBar->InitDeviceObjects(pDataHeader) ;	
		}
	}
	
	if(pScrollLine)
	{
		pDataHeader = FindResource(pScrollLine);
		if(pDataHeader)
		{
			m_ImgScrollLine = new CINFImageEx;
			m_ImgScrollLine->InitDeviceObjects(pDataHeader) ;	
		}
	}

	for(i = 0; i < 4; i++)
	{
		if(pScrollTop)
		{
			wsprintf(buf, "%s%d", pScrollTop, i);
			pDataHeader = FindResource(buf);
			if(pDataHeader)
			{
				m_ImgScrollTopBtn[i] = new CINFImageEx;
				m_ImgScrollTopBtn[i]->InitDeviceObjects(pDataHeader) ;	
			}
		}

		if(pScrollBottom)
		{
			wsprintf(buf, "%s%d", pScrollBottom, i);
			pDataHeader = FindResource(buf);
			if(pDataHeader)
			{
				m_ImgScrollBottomBtn[i] = new CINFImageEx;
				m_ImgScrollBottomBtn[i]->InitDeviceObjects(pDataHeader) ;	
			}
		}
	} 

	return S_OK;
}

HRESULT CINFScrollBarEX::RestoreDeviceObjects()
{
	int i = 0;

	if(m_ImgScrollBar)
	{
		m_ImgScrollBar->RestoreDeviceObjects();
	}
	
	if(m_ImgScrollLine)
	{
		m_ImgScrollLine->RestoreDeviceObjects();
	}

	for(i = 0; i < 4; i++)
	{
		if(m_ImgScrollTopBtn[i])
		{
			m_ImgScrollTopBtn[i]->RestoreDeviceObjects();
		}

		if(m_ImgScrollBottomBtn[i])
		{
			m_ImgScrollBottomBtn[i]->RestoreDeviceObjects();
		}
	}

	return S_OK;
}

HRESULT CINFScrollBarEX::DeleteDeviceObjects()
{
	int i = 0;

	if(m_ImgScrollBar)
	{
		m_ImgScrollBar->DeleteDeviceObjects();
		util::del(m_ImgScrollBar);
	}
	
	if(m_ImgScrollLine)
	{
		m_ImgScrollLine->DeleteDeviceObjects();
		util::del(m_ImgScrollLine);
	}

	for(i = 0; i < 4; i++)
	{
		if(m_ImgScrollTopBtn[i])
		{
			m_ImgScrollTopBtn[i]->DeleteDeviceObjects();
			util::del(m_ImgScrollTopBtn[i]);
		}

		if(m_ImgScrollBottomBtn[i])
		{
			m_ImgScrollBottomBtn[i]->DeleteDeviceObjects();
			util::del(m_ImgScrollBottomBtn[i]);
		}
	}

	return S_OK;
}

HRESULT CINFScrollBarEX::InvalidateDeviceObjects()
{
	int i = 0;

	if(m_ImgScrollBar)
	{
		m_ImgScrollBar->InvalidateDeviceObjects();
	}
	
	if(m_ImgScrollLine)
	{
		m_ImgScrollLine->InvalidateDeviceObjects();
	}

	for(i = 0; i < 4; i++)
	{
		if(m_ImgScrollTopBtn[i])
		{
			m_ImgScrollTopBtn[i]->InvalidateDeviceObjects();
		}

		if(m_ImgScrollBottomBtn[i])
		{
			m_ImgScrollBottomBtn[i]->InvalidateDeviceObjects();
		}
	}

	return S_OK;
}

void CINFScrollBarEX::Render()
{
	if(m_ImgScrollBar)
	{
		m_ImgScrollBar->Move(*m_nParentX + m_nStartX, *m_nParentY + m_nStartY + m_nTopBtnSize + m_nScrollPosition);
		m_ImgScrollBar->Render();
	}
	
	if(m_ImgScrollLine)
	{
		m_ImgScrollLine->Move(*m_nParentX + m_nStartX, *m_nParentY + m_nStartY + m_nTopBtnSize);
		m_ImgScrollLine->Render();
	}

	if(m_ImgScrollTopBtn[m_byScrollTopBtnState])
	{
		m_ImgScrollTopBtn[m_byScrollTopBtnState]->Move(*m_nParentX + m_nStartX, *m_nParentY + m_nStartY);
		m_ImgScrollTopBtn[m_byScrollTopBtnState]->Render();
	}

	if(m_ImgScrollBottomBtn[m_byScrollBottomBtnState])
	{
		m_ImgScrollTopBtn[m_byScrollBottomBtnState]->Move(*m_nParentX + m_nStartX, *m_nParentY + m_nStartY + m_nTopBtnSize + m_nScrollLineLength);
		m_ImgScrollBottomBtn[m_byScrollBottomBtnState]->Render();
	}
}

void CINFScrollBarEX::Tick()
{
}

int CINFScrollBarEX::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_bScrollLock = FALSE;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
		
			if(CheckINFPosition(pt, SCROLLPOS_BAR))
			{
				m_nScrollBtn = pt.y - (*m_nParentY + m_nStartY + m_nScrollPosition);
				m_bScrollLock = TRUE;
				return INF_MSGPROC_BREAK;
			}
			else if(CheckINFPosition(pt, SCROLLPOS_TOP_BTN))
			{
				m_byScrollTopBtnState = SCROLL_BTN_PUSH;
				return INF_MSGPROC_BREAK;
			}
			else if(CheckINFPosition(pt, SCROLLPOS_TOP_BTN))
			{
				m_byScrollBottomBtnState = SCROLL_BTN_PUSH;
				return INF_MSGPROC_BREAK;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			m_byScrollTopBtnState = SCROLL_BTN_NON;
			m_byScrollBottomBtnState = SCROLL_BTN_NON;

			int nScrollBtnY = pt.y - m_nScrollBtn;
			int nStartX = *m_nParentX + m_nStartX;
			int nStartY = *m_nParentY + m_nStartY;

			// 스크롤 바 버튼
			if(m_bScrollLock  == TRUE &&
				pt.x >= nStartX &&
				pt.x <= nStartX + m_nScrollBarWidth)
			{
				if(nScrollBtnY >= nStartY + m_nTopBtnSize &&
					nScrollBtnY <= nStartY + m_nTopBtnSize + m_nScrollMoveLength)
				{
					m_nScrollPosition = nScrollBtnY - nStartY;
					m_nScrollLine = GetScrollLineNumber(m_nScrollPosition);
				}
				else if(nScrollBtnY < nStartY + m_nTopBtnSize)
				{
					SetScrollStartLine();
				}
				else if(nScrollBtnY > nStartY + m_nTopBtnSize + m_nScrollMoveLength)
				{
					SetScrollEndLine();
				}
			}
			// 스크롤 상 버튼
			else if(m_bScrollLock  == FALSE && 
					CheckINFPosition(pt, SCROLLPOS_TOP_BTN))
			{
				m_byScrollTopBtnState = SCROLL_BTN_OVER;
			}
			// 스크롤 하 버튼
			else if(m_bScrollLock  == FALSE && 
					CheckINFPosition(pt, SCROLLPOS_BOTTOM_BTN))
			{
				m_byScrollBottomBtnState = SCROLL_BTN_OVER;
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

BOOL CINFScrollBarEX::CheckINFPosition(POINT pt, BYTE byType)
{
	int nStartX = *m_nParentX + m_nStartX;
	int nStartY = *m_nParentY + m_nStartY;

	// 상 버튼
	if(byType == SCROLLPOS_TOP_BTN &&
		pt.x >= nStartX &&
		pt.x <= nStartX + m_nScrollBarWidth &&
		pt.y >= nStartY &&
		pt.y <= nStartY + m_nTopBtnSize)
	{
		return TRUE;
	}
	// 하 버튼
	else if(byType == SCROLLPOS_BOTTOM_BTN &&
			pt.x >= nStartX &&
			pt.x <= nStartX + m_nScrollBarWidth &&
			pt.y >= nStartY + m_nTopBtnSize + m_nScrollMoveLength &&
			pt.y <= nStartY + m_nTopBtnSize + m_nScrollMoveLength + m_nBottomBtnSize)
	{
		return TRUE;
	}
	// 스크롤 바
	else if(byType == SCROLLPOS_BAR &&
			pt.x >= nStartX &&
			pt.x <= nStartX + m_nScrollBarWidth &&
			pt.y >= nStartY + m_nScrollPosition&&
			pt.y <= nStartY + m_nScrollPosition + m_nScrollBarHeight)
	{
		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 중심 코드
int CINFScrollBarEX::GetScrollLine()
{
	int ntemp = m_nScrollMaxCount - m_nScrollMaxLine;

	if(ntemp < 0)
	{
		ntemp = 0;
	}
	return ntemp;
}

int CINFScrollBarEX::GetScrollLineNumber(float nLength)
{
	int ntemp = GetScrollLine();
	float fLineNum = (nLength * ntemp) / m_nScrollMoveLength;

	return (int)fLineNum;
}

float CINFScrollBarEX::GetScrollLineInterval(int nScrollLineNum)
{
	int ntemp = GetScrollLine();
	float fLineNum = 0.0f;
	if(ntemp)
		fLineNum = (m_nScrollMoveLength * nScrollLineNum) / ntemp;

	return fLineNum;
}

void CINFScrollBarEX::SetScrollStartLine()
{
	m_nScrollPosition = 0;
	m_nScrollLine = 0;
}

void CINFScrollBarEX::SetScrollEndLine()
{
	m_nScrollPosition = 0;
	m_nScrollLine = GetScrollLine();
	if(m_nScrollLine)
		m_nScrollPosition = m_nScrollMoveLength;

}

// 중심 코드
//////////////////////////////////////////////////////////////////////////
void CINFScrollBarEX::SetScrollImgSize(int nTopBtnSize, int nBottomBtnSize)
{
	m_nTopBtnSize				= nTopBtnSize;
	m_nBottomBtnSize			= nBottomBtnSize;

}

void CINFScrollBarEX::SetScrollData(int nMaxCount, int nMaxLine)
{
	m_nScrollMaxCount = nMaxCount;
	m_nScrollMaxLine = nMaxLine;

	m_nScrollPosition = GetScrollLineInterval(m_nScrollLine);
}

void CINFScrollBarEX::SetMouseWheel(int nwParam)
{
	if(nwParam < 0)
	{
		m_nScrollLine++;
		if(m_nScrollLine > GetScrollLine())
		{
			SetScrollEndLine();
		}
		else
		{
			m_nScrollPosition = GetScrollLineInterval(m_nScrollLine);
		}
	}
	else
	{
		m_nScrollLine--;
		if(m_nScrollLine < 0)
		{
			SetScrollStartLine();
		}
		else
		{
			m_nScrollPosition = GetScrollLineInterval(m_nScrollLine);
		}
	}
}
