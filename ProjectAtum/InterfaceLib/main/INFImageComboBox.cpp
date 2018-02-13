// INFImageComboBox.cpp: implementation of the CINFImageComboBox class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "D3dHanFont.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"

#include "INFImageComboBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageComboBox::CINFImageComboBox()
{
	m_vecComboInfo.clear();
	m_pFontTxt = NULL;

	m_ptPos.x = m_ptPos.y = 0;
	m_nWidth = 1;
	m_nHeight= 1;

	m_bShowItem = FALSE;
	m_bShowWnd= TRUE;
	m_nCurSel = -1;
	m_nCurSelItem = -1;

	m_pImgCombo = NULL;
	m_pImgSelect = NULL;
}

CINFImageComboBox::~CINFImageComboBox()
{
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{		
		util::del((*it));		
		it++;
	}
	m_vecComboInfo.clear();
	
	util::del(m_pImgCombo);
	util::del(m_pImgSelect);
}

HRESULT CINFImageComboBox::InitDeviceObjects(POINT ptPos, int nWidth, int nHeight)
{
	m_ptPos = ptPos;
	m_nWidth = nWidth;
	m_nHeight= nHeight;

	if(NULL == m_pFontTxt)
	{
		m_pFontTxt = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontTxt->InitDeviceObjects(g_pD3dDev);
	}
	
	DataHeader* pDataHeader = m_pGameData->Find("combobak");
	if(NULL == m_pImgCombo)
	{
		m_pImgCombo = new CINFImageEx;
		m_pImgCombo->InitDeviceObjects( pDataHeader );
	}

	pDataHeader = m_pGameData->Find("combosel");
	if(NULL == m_pImgSelect)
	{
		m_pImgSelect = new CINFImageEx;
		m_pImgSelect->InitDeviceObjects( pDataHeader );
	}

	return S_OK;
}

HRESULT CINFImageComboBox::RestoreDeviceObjects()
{
	if(m_pFontTxt)
	{
		m_pFontTxt->RestoreDeviceObjects();
	}

	if(m_pImgCombo)
	{
		m_pImgCombo->RestoreDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->RestoreDeviceObjects();
	}
	
	return S_OK;
}
	
HRESULT CINFImageComboBox::DeleteDeviceObjects()
{
	if(m_pFontTxt)
	{
		m_pFontTxt->DeleteDeviceObjects();
		util::del(m_pFontTxt);
	}
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{		
		util::del((*it));		
		it++;
	}
	m_vecComboInfo.clear();

	if(m_pImgCombo)
	{
		m_pImgCombo->DeleteDeviceObjects();
		util::del(m_pImgCombo);
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->DeleteDeviceObjects();
		util::del(m_pImgSelect);
	}


	return S_OK;
}

HRESULT CINFImageComboBox::InvalidateDeviceObjects()
{
	if(m_pFontTxt)
	{
		m_pFontTxt->InvalidateDeviceObjects();
	}
	if(m_pImgCombo)
	{
		m_pImgCombo->InvalidateDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->InvalidateDeviceObjects();
	}
	return S_OK;
}
	
void CINFImageComboBox::Render()
{	
	if(FALSE == IsShowWindow())
	{
		return ;
	}
	int nLine, nPosX, nPosY;
	nLine = nPosX = nPosY= 0;			
	if(m_nCurSel >=0 && m_nCurSel < m_vecComboInfo.size())
	{
		structComboInfo* pItem = m_vecComboInfo[m_nCurSel];
		if(pItem && pItem->pInfImage)
		{
			nPosX = m_ptPos.x;
			nPosY = m_ptPos.y + (nLine*m_nHeight);			

			pItem->pInfImage->Move(nPosX, nPosY);
			pItem->pInfImage->Render();
		}
		nLine++;
	}
	if(m_bShowItem)
	{
		int nCursel = 0;
		vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
		while(it !=  m_vecComboInfo.end()) 
		{				
			structComboInfo* pItem = (*it);
			if((NULL == pItem)
				||(!pItem->bShowItem))
			{
				nCursel++;
				it++;
				continue;
			}
			
			nPosX = m_ptPos.x;
			nPosY = m_ptPos.y + (nLine*m_nHeight);

			// 배경
			if(nCursel == m_nCurSelItem)
			{
				m_pImgSelect->Move(nPosX, nPosY);
				m_pImgSelect->SetScale(m_nWidth, m_nHeight);
				m_pImgSelect->Render();				
			}
			else
			{
				m_pImgCombo->Move(nPosX, nPosY);
				m_pImgCombo->SetScale(m_nWidth, m_nHeight);
				m_pImgCombo->Render();				
			}

			if(pItem->pInfImage)
			{
				pItem->pInfImage->Move(nPosX, nPosY);
				pItem->pInfImage->Render();
			}

			nPosX += 20;
			m_pFontTxt->DrawText(nPosX, nPosY,GUI_FONT_COLOR, pItem->chComboTxt, 0 );
		
			nCursel++;
			nLine++;
			it++;
		}		
	}
	else
	{

	}
}

void CINFImageComboBox::AddItem(int nIdx, char *pComboTxt, CINFImage* pInfImage, BOOL bShowItem)
{
	structComboInfo* pComboInfo = new structComboInfo;
	pComboInfo->nIdx = nIdx;	// 인덱스 
	pComboInfo->pInfImage = pInfImage;
	memset(pComboInfo->chComboTxt, 0x00, 256);
	strncpy(pComboInfo->chComboTxt, pComboTxt, strlen(pComboTxt)+1);
	pComboInfo->bShowItem = bShowItem;

	m_vecComboInfo.push_back(pComboInfo);
}

void CINFImageComboBox::ResetItem()
{
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{		
		util::del((*it));		
		it++;
	}
	m_vecComboInfo.clear();

}

void CINFImageComboBox::UpdateItemInfo(int nIdx, BOOL bShowItem)
{
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{		
		if((*it)->nIdx == nIdx)
		{
			(*it)->bShowItem = bShowItem;			
		}		
		it++;
	}

}


int CINFImageComboBox::OnLButtonDown(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return LBTN_COMBO_NONE;
	}
	
	BOOL bXPos = ((m_ptPos.x <= ptPos.x) &&(ptPos.x <= (m_ptPos.x+m_nWidth)));
	if(!bXPos)
	{
		// X좌표 영역안에 없다.
		//m_bShowItem = FALSE;
		return 	LBTN_COMBO_NONE;	
	}
	if((m_ptPos.y <= ptPos.y)&&( ptPos.y <= (m_ptPos.y + m_nHeight) ))
	{	
		if(!m_bShowItem)
		{
			//m_bShowItem = TRUE;
			return LBTN_COMBO_SHOWITEM;
		}
		//m_bShowItem = FALSE;
		return LBTN_COMBO_HIDEITEM;			
	}
	if(!m_bShowItem )
	{
		//m_bShowItem = FALSE;
		return LBTN_COMBO_NONE;
	}
	m_bShowItem = FALSE;	

	int nLine = 1;
	int nCursel = 0;
	int nPosX, nPosY;
	nPosX = nPosY= 0;
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{				
		structComboInfo* pItem = (*it);
		if((NULL == pItem)
			|| (!pItem->bShowItem))
		{
			nCursel++;
			it++;
			continue;
		}		
		nPosY = m_ptPos.y + (nLine*m_nHeight);
		if((nPosY <= ptPos.y)&&( ptPos.y <= (nPosY + m_nHeight) ))
		{
			m_nCurSel = nCursel;			
			return LBTN_COMBO_CHANGEITEM;
		}
		
		nCursel++;
		nLine++;
		it++;
	}
	
	return LBTN_COMBO_NONE;
}

BOOL CINFImageComboBox::OnMouseMove(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return FALSE;
	}
	if(!m_bShowItem)
	{
		return FALSE;
	}
	BOOL bXPos = ((m_ptPos.x <= ptPos.x) &&(ptPos.x <= (m_ptPos.x+m_nWidth)));
	if(!bXPos)
	{
		// X좌표 영역안에 없다.
		return FALSE;	
	}
	int nLine = 1;
	int nCursel = 0;
	int nPosX, nPosY;
	nPosX = nPosY= 0;
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{
		structComboInfo* pItem = (*it);
		if((NULL == pItem)
			||(!pItem->bShowItem))
		{
			nCursel++;
			it++;
			continue;
		}		
		nPosY = m_ptPos.y + (nLine*m_nHeight);
		if((nPosY <= ptPos.y)&&( ptPos.y <= (nPosY + m_nHeight) ))
		{
			m_nCurSelItem = nCursel;			
			return TRUE;
		}
		nCursel++;
		nLine++;
		it++;
	}

	return FALSE;
}
// 버튼 위에 마우스가 있냐?

BOOL CINFImageComboBox::OnLButtonUp(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return FALSE;
	}		
	
	return FALSE;
}

BOOL CINFImageComboBox::IsShowWindow()
{
	return m_bShowWnd;
}

void CINFImageComboBox::ShowWindow(BOOL bShowWnd, POINT *pPos)
{
	m_bShowWnd = bShowWnd;	
	if(bShowWnd && pPos)
	{
		m_ptPos = (*pPos);
	}
}

void CINFImageComboBox::ShowItem(BOOL bShowItem)
{
	m_bShowItem = bShowItem;
}

BOOL CINFImageComboBox::IsShowItem()
{
	return m_bShowItem;
}
int CINFImageComboBox::GetCurSel()
{
	return m_nCurSel;
}

void CINFImageComboBox::SetCurSel(int nCurSel)
{
	m_nCurSel = nCurSel;
}
void CINFImageComboBox::SetPos(POINT ptPos)
{
	m_ptPos = ptPos;
}
