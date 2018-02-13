// INFComboBoxEX.cpp: implementation of the CINFComboBoxEX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFComboBoxEX.h"
#include "INFImage.h"
#include "D3dHanFont.h"
#include "dxutil.h"
#include "Chat.h"
#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI시스템 변경

#define COMBO_WINDOW_TOP_GAP		4			//최상위에서 여유 공간.
#define COMBO_WINDOW_LEFT_GAP		3			//최상위에서 여유 공간.


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFComboBoxEX::CINFComboBoxEX()
{
	m_nActiveItem		= 0;
	m_nScrollIndex		= 0;
	memset(&m_rtCombo,0,sizeof(RenderRect));
	memset(&m_sizeItem,0,sizeof(SIZE));

	m_nSelectDataIndex	= -1;
}

CINFComboBoxEX::~CINFComboBoxEX()
{


}
	
CINFComboBoxEX::CINFComboBoxEX(CAtumNode* pParent, CGameData* pGameData)
{
	m_pParent			= pParent;	// CGameMain*
	m_pGameData			= pGameData;
	m_nActiveItem		= 0;
	m_nScrollIndex		= 0;
	memset(&m_rtCombo,0,sizeof(RenderRect));
	memset(&m_sizeItem,0,sizeof(SIZE));

	m_nSelectDataIndex	= -1;
}
	

int	CINFComboBoxEX::AddItem(void *pItem)
{
	//Add Item
	return true;
}
int	CINFComboBoxEX::DelItem(void *pItem)
{
	//Delete Item

	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \class		bool SetScrollBarInfo(int x,int y, int lenght,int width,int height)
/// \brief		스크롤바의 정보를 입력.
///
/// \author		dgwoo
/// \version	
/// \date		2006-07-31 ~ 2006-07-31
/// \warning	
///////////////////////////////////////////////////////////////////////////////
bool CINFComboBoxEX::SetScrollBarInfo(int x,int y, int lenght,int width,int height)
{
	m_ScrollBarInfo.xOri = x;
	m_ScrollBarInfo.yOri = y;
	m_ScrollBarInfo.x = m_rtCombo.x + m_ScrollBarInfo.xOri-(m_ScrollBarInfo.barwidth/2);
	m_ScrollBarInfo.y = m_rtCombo.y + m_ScrollBarInfo.yOri;
	m_ScrollBarInfo.lenght = lenght-height;
	m_ScrollBarInfo.barheight = height;
	m_ScrollBarInfo.barwidth = width;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \class		void SetComboInfo(int ItemCount,int SlotNum,int MaxLine)
/// \brief		콤보 박스 의 정보를 입력.
///
/// \author		dgwoo
/// \version	
/// \date		2006-07-31 ~ 2006-07-31
/// \warning	
///////////////////////////////////////////////////////////////////////////////
void CINFComboBoxEX::SetComboInfo(int ItemCount,int SlotNum,int SelectWidth,int SelectHeight)
{
	m_ComboInfo.ItemCount = ItemCount;
	m_ComboInfo.SlotNum = SlotNum;
	m_ComboInfo.MaxLine = ItemCount - SlotNum;
	m_ComboInfo.SelectHeight = SelectHeight;
	m_ComboInfo.SelectWidth = SelectWidth;
	return ;
}

void CINFComboBoxEX::SetComboPt(int x,int y)
{
	m_rtCombo.x = x;
	m_rtCombo.y = y;
	m_ScrollBarInfo.x = x + m_ScrollBarInfo.xOri-(m_ScrollBarInfo.barwidth/2);
	m_ScrollBarInfo.y = y + m_ScrollBarInfo.yOri;
	/*--------------------------------------------------------------------------*/
	//콤보 박스에서 기존의 m_nScrollLine이 가지고 있으면서 Render에서 item index보다 작으면
	//그리지 않고 리턴하는 부분에서 오류 발생 스크롤 인덱스 값을 -1로 초기화 해준다.
	m_nScrollLine = 0;
	m_nScrollPosition = 0;

}
int CINFComboBoxEX::GetItemIndexByPos(int nPosY)
{
	int nItemCount = m_ComboInfo.ItemCount;
	if(nItemCount >= m_nScrollLine)
	{
		int nPtLine = (nPosY - m_rtCombo.y) / m_ComboInfo.SelectHeight;

		if(nPtLine + m_nScrollLine < nItemCount)
		{
			return nPtLine + m_nScrollLine;
		}
	}

	return -1;
}

int	CINFComboBoxEX::ComboRender()
{
	int i = 0;
	int nItemCount = m_vecData.size();
	//배경을 먼저 찍는다.
	m_pGroundImg->Move(m_rtCombo.x,m_rtCombo.y);
	m_pGroundImg->Render();

	if(m_ComboInfo.ItemCount < m_nScrollLine)
		return -1;
	
	vector<ComboData>::iterator ite = m_vecData.begin();
	while(ite != m_vecData.end())
	{
		int nLineCount = i - m_nScrollLine;
		if(nLineCount >= 0)
		{
			//if(nLineCount >= SHOP_ITEM_SLOT_NUMBER)
			if(nLineCount >= m_ComboInfo.SlotNum)
			{
				break;
			}

			char buf[125] = {0,};

			// 선택
			if(m_nSelectIndex == i)
			{
				m_pSelectImg->Move(m_rtCombo.x+COMBO_WINDOW_LEFT_GAP, m_rtCombo.y +COMBO_WINDOW_TOP_GAP+ (nLineCount * m_ComboInfo.SelectHeight));
				m_pSelectImg->Render();

			}
		
			wsprintf( buf, "%s", ite->DataName);
			m_pFontItemName->DrawText(m_rtCombo.x + COMBO_WINDOW_LEFT_GAP, m_rtCombo.y + COMBO_WINDOW_TOP_GAP + (nLineCount * m_ComboInfo.SelectHeight),GUI_FONT_COLOR_W,buf,0L);
			
		}

		ite++;
		i++;
	}
	/*--------------------------------------------------------------------------*/
	
	m_pScrollImg->Move(m_ScrollBarInfo.x,m_ScrollBarInfo.y + m_nScrollPosition);
	m_pScrollImg->Render();

	return true;
}
bool CINFComboBoxEX::CreateImage(const char * Ground,const char * Select,const char * Scroll)
{
	wsprintf(m_strGround,"%s",Ground);
	wsprintf(m_strSelect,"%s",Select);
	wsprintf(m_strScroll,"%s",Scroll);


	return true;
}

HRESULT	CINFComboBoxEX::InitDeviceObjects()
{
	DataHeader	* pDataHeader;

	m_pGroundImg = new CINFImageEx;
	pDataHeader = FindResource(m_strGround);
	m_pGroundImg->InitDeviceObjects(pDataHeader );

	m_pScrollImg = new CINFImageEx;
	pDataHeader = FindResource(m_strScroll);
	m_pScrollImg->InitDeviceObjects(pDataHeader );

	m_pSelectImg = new CINFImageEx;
	pDataHeader = FindResource(m_strSelect);
	m_pSelectImg->InitDeviceObjects(pDataHeader );

	m_pFontItemName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE, TRUE,202,32);
	m_pFontItemName->InitDeviceObjects(g_pD3dDev);
	//const ITEMKIND_STRING g_arrItemKindString[]
	
	return S_OK;
}
HRESULT	CINFComboBoxEX::InvalidateDeviceObject()
{
	m_pGroundImg->InvalidateDeviceObjects();
	m_pSelectImg->InvalidateDeviceObjects();
	m_pScrollImg->InvalidateDeviceObjects();
	m_pFontItemName->InvalidateDeviceObjects();
	return S_OK;
}
HRESULT	CINFComboBoxEX::RestoreDeviceObject()
{
	m_pGroundImg->RestoreDeviceObjects();
	m_pSelectImg->RestoreDeviceObjects();
	m_pScrollImg->RestoreDeviceObjects();
	m_pFontItemName->RestoreDeviceObjects();
	POINT pt = m_pSelectImg->GetImgSize();
	m_sizeItem.cx = pt.x;
	m_sizeItem.cy = pt.y;

	pt= m_pGroundImg->GetImgSize();
	m_rtCombo.w = pt.x;
	m_rtCombo.h = pt.y;

//	pt = m_pScrollImg->GetImgSize();
//	m_rtScroll.w = pt.x;
//	m_rtScroll.h = pt.y;
	
	return S_OK;
}
HRESULT	CINFComboBoxEX::DeleteDeviceObject()
{
	m_pGroundImg->DeleteDeviceObjects();
	m_pScrollImg->DeleteDeviceObjects();
	m_pSelectImg->DeleteDeviceObjects();
	m_pFontItemName->DeleteDeviceObjects();
	util::del(m_pGroundImg);
	util::del(m_pScrollImg);
	util::del(m_pSelectImg);
	util::del(m_pFontItemName);
	m_vecData.clear();

	return S_OK;
}

int CINFComboBoxEX::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int i;
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			m_bScrollLock = FALSE;
			for(i = 0; i < m_ComboInfo.SlotNum; i++)
			{
				// 선택
				if(//!g_pGameMain->m_stSelectItem.pSelectItem && 
					pt.x >= m_rtCombo.x &&
					pt.x <= m_rtCombo.x + m_ComboInfo.SelectWidth &&
					pt.y >= m_rtCombo.y + (i * m_ComboInfo.SelectHeight)&&
					pt.y <= m_rtCombo.y + ((i + 1) * m_ComboInfo.SelectHeight))
				{
					int nItemIndex = GetItemIndexByPos(pt.y);
					if(nItemIndex == m_nSelectIndex)
					{
						// 선택
						m_nSelectDataIndex = nItemIndex;
						break;
					}
					else
					{
						m_nSelectIndex = -1;
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
			
			//scroll control
			if( pt.x > m_ScrollBarInfo.x &&
				pt.x < m_ScrollBarInfo.x + m_ScrollBarInfo.barwidth &&
				pt.y > m_ScrollBarInfo.y + m_nScrollPosition &&
				pt.y < m_ScrollBarInfo.y + m_nScrollPosition + m_ScrollBarInfo.barheight)
			{
				m_nScrollBtn = pt.y - (m_ScrollBarInfo.y + m_nScrollPosition);
				m_bScrollLock = TRUE;
				return INF_MSGPROC_BREAK;
			}
		
						
			// 스크롤 모드로 변경. 
			if(!(pt.x >= m_rtCombo.x &&
				pt.x <= m_rtCombo.x + m_rtCombo.w &&
				pt.y >= m_rtCombo.y &&
				pt.y <= m_rtCombo.y + m_rtCombo.h))
			{
				
				return INF_MSGPROC_NORMAL;
			}			
			
			for(i = 0; i < m_ComboInfo.SlotNum; i++)
			{
				// 선택
				if(//!g_pGameMain->m_stSelectItem.pSelectItem && 
					pt.x >= m_rtCombo.x &&
					pt.x <= m_rtCombo.x + m_ComboInfo.SelectWidth &&
					pt.y >= m_rtCombo.y + (i * m_ComboInfo.SelectHeight)&&
					pt.y <= m_rtCombo.y + ((i + 1) * m_ComboInfo.SelectHeight))
				{
					int nItemIndex = GetItemIndexByPos(pt.y);
					if(nItemIndex != -1)
					{
						m_nSelectIndex = nItemIndex;
						break;
					}
					else
					{
						m_nSelectIndex = -1;
					}
				}
				else
				{
					m_nSelectIndex = -1;
				}
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			// 스크롤
			if(m_bScrollLock &&
				pt.x >= m_ScrollBarInfo.x - 5 && 
				pt.x <= m_ScrollBarInfo.x + m_ScrollBarInfo.barwidth + 5)
			{
				int nScrollBtnY = pt.y - m_nScrollBtn;
				if(nScrollBtnY >= m_ScrollBarInfo.y &&
					nScrollBtnY <= m_ScrollBarInfo.y + m_ScrollBarInfo.lenght)
				{
					m_nScrollPosition = nScrollBtnY - m_ScrollBarInfo.y;
					m_nScrollLine = GetScrollLineNumber(m_nScrollPosition);
				}
				else if(nScrollBtnY < m_ScrollBarInfo.y)
				{
					InitScrollLine();
				}
				else if(nScrollBtnY > m_ScrollBarInfo.y + m_ScrollBarInfo.lenght)
				{
					SetScrollEndLine();
				}
			}

			for(i = 0; i < m_ComboInfo.SlotNum; i++)
			{
				// 선택
				if(//!g_pGameMain->m_stSelectItem.pSelectItem && 
					pt.x >= m_rtCombo.x &&
					pt.x <= m_rtCombo.x + m_ComboInfo.SelectWidth &&
					pt.y >= m_rtCombo.y + (i * m_ComboInfo.SelectHeight)&&
					pt.y <= m_rtCombo.y + ((i + 1) * m_ComboInfo.SelectHeight))
				{
					int nItemIndex = GetItemIndexByPos(pt.y);
					if(nItemIndex != -1)
					{
						m_nSelectIndex = nItemIndex;
						break;
					}
					else
					{
						m_nSelectIndex = -1;
					}
				}
				else
				{
					m_nSelectIndex = -1;
				}
			}
			
			return INF_MSGPROC_BREAK;

		}
		break;
	case WM_MOUSEWHEEL:
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_pD3dApp->GetHwnd(), &pt);
			CheckMouseReverse(&pt);

			if(pt.x >= m_rtCombo.x &&
				pt.x <= m_rtCombo.x + m_rtCombo.w &&
				pt.y >= m_rtCombo.y &&
				pt.y <= m_rtCombo.y + m_rtCombo.h)
			{
				if((int)wParam<0)
				{
					m_nScrollLine++;
					if(m_nScrollLine > m_ComboInfo.MaxLine)
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
						InitScrollLine();
					}
					else
					{
						m_nScrollPosition = GetScrollLineInterval(m_nScrollLine);
					}
				}

				return INF_MSGPROC_BREAK;
			}
			
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

int	CINFComboBoxEX::GetScrollLine()
{
//	int ntemp = m_nScrollMaxCount - m_nScrollMaxLine;
//	int ntemp = m_ComboInfo.ItemCount - m_ComboInfo.MaxLine;
	int ntemp = m_ComboInfo.MaxLine;
	if(ntemp < 0)
	{
		ntemp = 0;
	}
	return ntemp;
}

int	CINFComboBoxEX::GetScrollLineNumber(float nLength)
{
	int ntemp = GetScrollLine();
	float fLineNum = (nLength * ntemp) / m_ScrollBarInfo.lenght;

	return (int)fLineNum;
}

float CINFComboBoxEX::GetScrollLineInterval(int nScrollLineNum)
{
	int ntemp = GetScrollLine();
	float fLineNum = 0.0f;
	if(ntemp)
		fLineNum = (m_ScrollBarInfo.lenght * nScrollLineNum) / ntemp;

	return fLineNum;
}

void CINFComboBoxEX::SetScrollEndLine()
{
	m_nScrollPosition = m_ScrollBarInfo.lenght;
	m_nScrollLine = GetScrollLine();
}

void CINFComboBoxEX::InitScrollLine()
{
	m_nScrollPosition = 0;
	m_nScrollLine = 0;
}
