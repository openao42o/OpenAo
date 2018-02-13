// INFComboBox.cpp: implementation of the CINFComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFComboBox.h"
#include "AtumApplication.h"
#include "D3DHanFont.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "dxutil.h"
#include "INFImageEx.h"	// 2011. 11. 17 by jskim CINFImage -> CINFImageEx 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFComboBox::CINFComboBox(CAtumNode* pParent, 
						   int nStartX, 
						   int nStartY, 
						   int nSizeX, 
						   int nSizeY, 
						   int nDefaultSelect)
{
	m_pImgCombo = NULL;
	m_pImgSelect = NULL;
	m_nStartX = nStartX;
	m_nStartY = nStartY;
	m_nSizeX = nSizeX;	
	m_nSizeY = nSizeY;
	m_nCurrentSelectIndex = nDefaultSelect;
	Init();
	m_bShowList = FALSE;
}

CINFComboBox::~CINFComboBox()
{
	util::del(m_pImgCombo);
	util::del(m_pImgSelect);
	vector<COMBO_BOX*>::iterator it =	m_vecComboBox.begin();
	while(it != m_vecComboBox.end())
	{
		(*it)->pFont->InvalidateDeviceObjects();
		(*it)->pFont->DeleteDeviceObjects();
		util::del((*it)->pFont);
		util::del(*it);
		it++;
	}
}

void CINFComboBox::Init()
{
	m_nMoveSelectIndex = -1;
	// 2006-09-14 by dgwoo 선택창이 사라지지 않는 버그 수정. 
	m_bShowList = FALSE;
	m_bSelectChanged = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFComboBox::AddString(char* szString, CINFImage* pImage = NULL, DWORD dwData)
/// \brief		스트링을 추가한다.
/// \author		dhkwon
/// \date		2004-05-28 ~ 2004-05-28
/// \warning	Image는 밖에서 로딩하여 ComboBox클래스에서는 초기화 하지 않는다.
///	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFComboBox::AddString(char* szString, CINFImage* pImage, DWORD dwData)
{
	COMBO_BOX* pComboBox = new COMBO_BOX;
	memset(pComboBox, 0x00, sizeof(COMBO_BOX));
	int nWidth = 0;
	if(m_nSizeX > 256)
	{
		nWidth = 512;
	}
	else if(m_nSizeX > 128)
	{
		nWidth = 256;
	}
	else
	{
		nWidth = 128;
	}
	pComboBox->pFont = new CD3DHanFont( _T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,nWidth,32);
//	pComboBox->pFont->InitDeviceObjects( g_pD3dDev );
//	pComboBox->pFont->RestoreDeviceObjects();
	strcpy( pComboBox->szString, szString );
	pComboBox->pImage = pImage;
	pComboBox->dwData = dwData;
	m_vecComboBox.push_back(pComboBox);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DWORD CINFComboBox::GetCurSelData()
/// \brief		현재 선택된 인덱스의 데이타를 가져온다.
/// \author		dhkwon
/// \date		2004-05-28 ~ 2004-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CINFComboBox::GetCurSelData()
{
	if(m_nCurrentSelectIndex >= 0 && m_nCurrentSelectIndex < m_vecComboBox.size())
	{
		return m_vecComboBox[m_nCurrentSelectIndex]->dwData;
	}
	return 0;
}

HRESULT CINFComboBox::InitDeviceObjects()
{
	DataHeader* pDataHeader = m_pGameData->Find("combobak");
	m_pImgCombo = new CINFImageEx;
	m_pImgCombo->InitDeviceObjects( pDataHeader );

	pDataHeader = m_pGameData->Find("combosel");
	m_pImgSelect = new CINFImageEx;
	m_pImgSelect->InitDeviceObjects( pDataHeader );
	vector<COMBO_BOX*>::iterator it = m_vecComboBox.begin();
	while(it != m_vecComboBox.end())
	{
		(*it)->pFont->InitDeviceObjects( g_pD3dDev );
		it++;
	}
	return S_OK;
}

HRESULT CINFComboBox::RestoreDeviceObjects()
{
	m_pImgCombo->RestoreDeviceObjects();
	m_pImgSelect->RestoreDeviceObjects();
	vector<COMBO_BOX*>::iterator it = m_vecComboBox.begin();
	while(it != m_vecComboBox.end())
	{
		(*it)->pFont->RestoreDeviceObjects();
		it++;
	}
	return S_OK;
}

HRESULT CINFComboBox::InvalidateDeviceObjects()
{
	m_pImgCombo->InvalidateDeviceObjects();
	m_pImgSelect->InvalidateDeviceObjects();
	vector<COMBO_BOX*>::iterator it = m_vecComboBox.begin();
	while(it != m_vecComboBox.end())
	{
		(*it)->pFont->InvalidateDeviceObjects();
		it++;
	}
	return S_OK;
}

HRESULT CINFComboBox::DeleteDeviceObjects()
{
	m_pImgCombo->DeleteDeviceObjects();
	m_pImgSelect->DeleteDeviceObjects();
	util::del(m_pImgCombo);
	util::del(m_pImgSelect);
	vector<COMBO_BOX*>::iterator it = m_vecComboBox.begin();
	while(it != m_vecComboBox.end())
	{
		(*it)->pFont->DeleteDeviceObjects();
		util::del((*it)->pFont);
		util::del(*it);
		it++;
	}
	m_vecComboBox.clear();
	return S_OK;
}

void CINFComboBox::Tick()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFComboBox::Render(BOOL bShowString)
/// \brief		
/// \author		dhkwon
/// \date		2004-05-29 ~ 2004-05-29
/// \warning	
///
/// \param		bShowString : 이미지가 있는 경우 스트링을 안보여줄 수 있다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
#define IMAGE_SIZE		12
void CINFComboBox::Render(BOOL bShowString)
{
	if( m_nCurrentSelectIndex >= 0 && 
		m_vecComboBox.size() > m_nCurrentSelectIndex)
	{
		int nStartX = m_nStartX;
		if(m_vecComboBox[m_nCurrentSelectIndex]->pImage)
		{
			m_vecComboBox[m_nCurrentSelectIndex]->pImage->Move(nStartX, m_nStartY);
			m_vecComboBox[m_nCurrentSelectIndex]->pImage->Render();
			nStartX += IMAGE_SIZE;
		}
		if(bShowString)
		{
			m_vecComboBox[m_nCurrentSelectIndex]->pFont->DrawText(nStartX, m_nStartY, GUI_FONT_COLOR,
				m_vecComboBox[m_nCurrentSelectIndex]->szString, 0L);
		}
	}
	if(m_bShowList)
	{
		RenderComboList();
	}
}
void CINFComboBox::RenderComboList()
{
	int nStartX, nStartY;
	int i=0;
	vector<COMBO_BOX*>::iterator it = m_vecComboBox.begin();
	while(it != m_vecComboBox.end())
	{
		nStartX = m_nStartX;
		nStartY = m_nStartY + (i+1)* m_nSizeY;
		if(i == m_nMoveSelectIndex)
		{
			m_pImgSelect->Move(nStartX, nStartY);
			m_pImgSelect->SetScale( m_nSizeX, m_nSizeY);
			m_pImgSelect->Render();
			if((*it)->pImage)
			{
				(*it)->pImage->Move(nStartX, nStartY);
				(*it)->pImage->Render();
				nStartX += IMAGE_SIZE;
			}
			(*it)->pFont->DrawText(nStartX, nStartY, GUI_SELECT_FONT_COLOR,
				(*it)->szString, 0L);
		}
		else
		{
			m_pImgCombo->Move(nStartX, nStartY);
			m_pImgCombo->SetScale( m_nSizeX, m_nSizeY);
			m_pImgCombo->Render();
			if((*it)->pImage)
			{
				(*it)->pImage->Move(nStartX, nStartY);
				(*it)->pImage->Render();
				nStartX += IMAGE_SIZE;
			}
			(*it)->pFont->DrawText(nStartX, nStartY, GUI_FONT_COLOR,
				(*it)->szString, 0L);
		}
		it++;
		i++;
	}
}

int CINFComboBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

//			if(m_bShowList == TRUE)
			{
				if( pt.x > m_nStartX && pt.x < m_nStartX + m_nSizeX-50 )
				{
					int i = (pt.y - m_nStartY) / m_nSizeY;
					if( i > 0 && 
						i <= m_vecComboBox.size() &&
						m_nCurrentSelectIndex != i-1)
					{
						if(m_bShowList == TRUE)
						{
							m_bSelectChanged = TRUE;
							m_nCurrentSelectIndex = i-1;
							m_nMoveSelectIndex = -1;
							m_bShowList = FALSE;
							return INF_MSGPROC_BREAK;
						}
					}

					else if( pt.x > m_nStartX && 
					pt.x < m_nStartX + m_nSizeX &&
					pt.y > m_nStartY &&
					pt.y < m_nStartY + m_nSizeY )
					{
						m_bShowList = !m_bShowList;
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
			if(m_bShowList)
			{
				if( pt.x > m_nStartX && pt.x < m_nStartX + m_nSizeX-50 )
				{
					int i = (pt.y - m_nStartY) / m_nSizeY;
					if( i > 0 && 
						i <= m_vecComboBox.size() &&
						m_nCurrentSelectIndex != i-1)
					{
//						m_bSelectChanged = TRUE;
//						m_nCurrentSelectIndex = i-1;
//						m_nMoveSelectIndex = -1;
////						m_bShowList = FALSE;
						return INF_MSGPROC_BREAK;
					}
				}
				Init();
				return INF_MSGPROC_BREAK;
			}
			else
			{
				if( pt.x > m_nStartX && 
					pt.x < m_nStartX + m_nSizeX &&
					pt.y > m_nStartY &&
					pt.y < m_nStartY + m_nSizeY )
				{
					Init();
//					m_bShowList = TRUE;
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
			if(m_bShowList)
			{
				if( pt.x > m_nStartX && pt.x < m_nStartX + m_nSizeX-50 )
				{
					int i = (pt.y - m_nStartY) / m_nSizeY;
					if( i > 0 && i <= m_vecComboBox.size() )
					{
						m_nMoveSelectIndex = i-1;
						return INF_MSGPROC_BREAK;
					}
				}
			}
		}
		break;
	}
	return INF_MSGPROC_NORMAL;
}

