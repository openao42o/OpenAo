// INFImageComboBoxEx.cpp: implementation of the CINFImageComboBoxEx class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "D3dHanFont.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

#include "INFImageComboBoxEx.h"

#define MAX_ONE_PAGE		10

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageComboBoxEx::CINFImageComboBoxEx()
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

	m_pImgComboBtn = NULL;
	m_pImgSelect = NULL;
	m_pImgSelectBk= NULL;

	m_pScroll				= NULL;
}

CINFImageComboBoxEx::~CINFImageComboBoxEx()
{
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{		
		util::del((*it));		
		it++;
	}
	m_vecComboInfo.clear();
	
	util::del(m_pImgComboBtn);
	util::del(m_pImgSelect);
	util::del(m_pImgSelectBk);

	util::del(m_pScroll);
}

HRESULT CINFImageComboBoxEx::InitDeviceObjects(POINT ptPos, int nWidth, int nHeight, 
												char* i_pBkImg, 
												char* i_pSelBkImg, 
												char* i_pSelImg, 
												char* i_pScroll)
{
	m_ptPos = ptPos;
	m_nWidth = nWidth;
	m_nHeight= nHeight;

	if(NULL == m_pFontTxt)
	{
		m_pFontTxt = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE,128,32);
		m_pFontTxt->InitDeviceObjects(g_pD3dDev);
	}
	
	//DataHeader* pDataHeader = m_pGameData->Find("combobak");
	DataHeader* pDataHeader = NULL;
	
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "%s3", i_pBkImg);
		wsprintf(szDownBtn, "%s1", i_pBkImg);
		wsprintf(szSelBtn, "%s0", i_pBkImg);
		wsprintf(szDisBtn, "%s2", i_pBkImg);
		if(NULL == m_pImgComboBtn)
		{
			m_pImgComboBtn = new CINFImageBtn;
		}
		m_pImgComboBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}

	//pDataHeader = m_pGameData->Find("combosel");
	pDataHeader = m_pGameData->Find(i_pSelImg);
	if(NULL == m_pImgSelect)
	{
		// 배경선택이미지 "jcsel"
		m_pImgSelect = new CINFImageEx;
		m_pImgSelect->InitDeviceObjects( pDataHeader );
	}

	pDataHeader = m_pGameData->Find(i_pSelBkImg);
	if(NULL == m_pImgSelectBk)
	{
		// 배경선택이미지 배경 "jcbk"
		m_pImgSelectBk = new CINFImageEx;
		m_pImgSelectBk->InitDeviceObjects( pDataHeader );
	}

	{
		char  szScBall[30];	
		if(NULL == m_pScroll)
		{
			m_pScroll = new CINFArenaScrollBar;
			//wsprintf(szScBall,"c_scrlb");
			wsprintf(szScBall,i_pScroll);
			m_pScroll->InitDeviceObjects(MAX_ONE_PAGE, szScBall);
		}		
	}

	return S_OK;
}

HRESULT CINFImageComboBoxEx::RestoreDeviceObjects()
{
	if(m_pFontTxt)
	{
		m_pFontTxt->RestoreDeviceObjects();
	}

	if(m_pImgComboBtn)
	{
		m_pImgComboBtn->RestoreDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->RestoreDeviceObjects();
	}
	if(m_pImgSelectBk)
	{
		m_pImgSelectBk->RestoreDeviceObjects();
	}
	if(m_pScroll)
	{
		// 스크롤
		m_pScroll->RestoreDeviceObjects();		
	}

	// 위치 재갱신
	SetPos(m_ptPos);
	return S_OK;
}
	
HRESULT CINFImageComboBoxEx::DeleteDeviceObjects()
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

	if(m_pImgComboBtn)
	{
		m_pImgComboBtn->DeleteDeviceObjects();
		util::del(m_pImgComboBtn);
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->DeleteDeviceObjects();
		util::del(m_pImgSelect);
	}
	if(m_pImgSelectBk)
	{
		m_pImgSelectBk->DeleteDeviceObjects();
		util::del(m_pImgSelectBk);
	}

	if(m_pScroll)
	{
		m_pScroll->DeleteDeviceObjects();
		util::del(m_pScroll);
	}


	return S_OK;
}

HRESULT CINFImageComboBoxEx::InvalidateDeviceObjects()
{
	if(m_pFontTxt)
	{
		m_pFontTxt->InvalidateDeviceObjects();
	}
	if(m_pImgComboBtn)
	{
		m_pImgComboBtn->InvalidateDeviceObjects();
	}
	if(m_pImgSelect)
	{
		m_pImgSelect->InvalidateDeviceObjects();
	}
	if(m_pImgSelectBk)
	{
		m_pImgSelectBk->InvalidateDeviceObjects();
	}
	if(m_pScroll)
	{
		m_pScroll->InvalidateDeviceObjects();
	}
	return S_OK;
}
	
void CINFImageComboBoxEx::Render()
{	
	if(FALSE == IsShowWindow())
	{
		return ;
	}
	int nLine, nPosX, nPosY;
	nLine = nPosX = nPosY= 0;	
	
	
	{
		// 배경 이미지		
		m_pImgComboBtn->Render();				
	}
	if(m_nCurSel >=0 && m_nCurSel < m_vecComboInfo.size())
	{
		structComboInfo* pItem = m_vecComboInfo[m_nCurSel];
		if(pItem)
		{
			nPosX = m_ptPos.x;
			nPosY = m_ptPos.y + (nLine*m_nHeight);			

			if(pItem->pInfImage)
			{				
				pItem->pInfImage->Move(nPosX, nPosY);
				pItem->pInfImage->Render();
			}
			if(strlen(pItem->chComboTxt) > 0)
			{
				m_pFontTxt->DrawText(nPosX, nPosY,GUI_FONT_COLOR, pItem->chComboTxt, 0 );
			}
		}
		nLine++;
	}
	if(m_bShowItem)
	{
		// 배경 이미지
		{
			nPosX = m_ptPos.x;
			nPosY = m_ptPos.y + (nLine*m_nHeight);
			m_pImgSelectBk->Move(nPosX, nPosY);
			m_pImgSelectBk->Render();

		}
		int nCursel = 0;
		int nRenderItemCnt = 0;

		// 스크롤
		m_pScroll->Render();
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
			else if(nCursel < m_pScroll->GetScrollStep())
			{
				nCursel++;
				it++;
				continue;
			}
			if(nRenderItemCnt >= MAX_ONE_PAGE)
			{
				//최대 10개까지 표시
				break;
			}
			nPosX = m_ptPos.x;
			nPosY = m_ptPos.y + (nLine*m_nHeight);

			// 배경
			if(nCursel == m_nCurSelItem)
			{
				m_pImgSelect->Move(nPosX, nPosY);				
				m_pImgSelect->Render();				
			}
//			else
//			{
//				m_pImgCombo->Move(nPosX, nPosY);
//				m_pImgCombo->SetScale(m_nWidth, m_nHeight);
//				m_pImgCombo->Render();				
//			}

			if(pItem->pInfImage)
			{
				pItem->pInfImage->Move(nPosX, nPosY);
				pItem->pInfImage->Render();
			}

			nPosX += 20;
			m_pFontTxt->DrawText(nPosX, nPosY,GUI_FONT_COLOR, pItem->chComboTxt, 0 );

			// 스크롤
			//m_pScroll->Render();
		
			nCursel++;
			nRenderItemCnt++;	// 그리고 있는 아이템수
			nLine++;
			it++;
		}		
	}
	else
	{

	}
}

void CINFImageComboBoxEx::AddItem(int nIdx, char *pComboTxt, CINFImage* pInfImage, BOOL bShowItem)
{
	structComboInfo* pComboInfo = new structComboInfo;
	pComboInfo->nIdx = nIdx;	// 인덱스 
	pComboInfo->pInfImage = pInfImage;
	memset(pComboInfo->chComboTxt, 0x00, 256);
	strncpy(pComboInfo->chComboTxt, pComboTxt, strlen(pComboTxt)+1);
	pComboInfo->bShowItem = bShowItem;

	m_vecComboInfo.push_back(pComboInfo);
}

void CINFImageComboBoxEx::ResetItem()
{
	vector<structComboInfo*>::iterator it =  m_vecComboInfo.begin();
	while(it !=  m_vecComboInfo.end()) 
	{		
		util::del((*it));		
		it++;
	}
	m_vecComboInfo.clear();

}

void CINFImageComboBoxEx::UpdateItemInfo(int nIdx, BOOL bShowItem)
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


int CINFImageComboBoxEx::OnLButtonDown(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return LBTN_COMBOEX_NONE;
	}

	if(m_bShowItem)
	{
		BOOL bClick = m_pScroll->IsMouseBallPos(ptPos);
		if(bClick)
		{
			m_pScroll->SetMouseMoveMode(TRUE);
			return LBTN_COMBOEX_SELSCROLL;
		}
	}
	
	
	BOOL bXPos = ((m_ptPos.x <= ptPos.x) &&(ptPos.x <= (m_ptPos.x+m_nWidth)));
	if(!bXPos)
	{
		// X좌표 영역안에 없다.
		//m_bShowItem = FALSE;
		return 	LBTN_COMBOEX_NONE;	
	}
	
	if((m_ptPos.y <= ptPos.y)&&( ptPos.y <= (m_ptPos.y + m_nHeight) ))
	{	
		if(!m_bShowItem)
		{
			//m_bShowItem = TRUE;
			return LBTN_COMBOEX_SHOWITEM;
		}
		//m_bShowItem = FALSE;
		return LBTN_COMBOEX_HIDEITEM;			
	}
	if(!m_bShowItem )
	{
		//m_bShowItem = FALSE;
		return LBTN_COMBOEX_NONE;
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
		else if(nCursel < m_pScroll->GetScrollStep())
		{
			nCursel++;
			it++;
			continue;
		}
		nPosY = m_ptPos.y + (nLine*m_nHeight);
		if((nPosY <= ptPos.y)&&( ptPos.y <= (nPosY + m_nHeight) ))
		{
			m_nCurSel = nCursel;			
			return LBTN_COMBOEX_CHANGEITEM;
		}
		
		nCursel++;
		nLine++;
		it++;
	}
	
	return LBTN_COMBOEX_NONE;
}

BOOL CINFImageComboBoxEx::OnMouseMove(POINT ptPos)
{
	if (!IsShowWindow()) return FALSE;
	
	int  nComboBtnState = BTN_STATUS_UP;
	
	if((m_ptPos.x <= ptPos.x) &&(ptPos.x <= (m_ptPos.x+m_nWidth)))
	{
		if((m_ptPos.y <= ptPos.y)&&( ptPos.y <= (m_ptPos.y + m_nHeight) ))
		{
			nComboBtnState = BTN_STATUS_SEL;
		}
	}
	m_pImgComboBtn->SetBtnState(nComboBtnState);	

	if(!m_bShowItem)
	{
		return FALSE;
	}
	
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
				return TRUE;
			}
		}
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
		else if(nCursel < m_pScroll->GetScrollStep())
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

BOOL CINFImageComboBoxEx::OnLButtonUp(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return FALSE;
	}		
	{
		BOOL bClick = m_pScroll->GetMouseMoveMode();
		if(bClick)
		{
			m_pScroll->SetMouseMoveMode(FALSE);
			return FALSE;
		}				
	}
	return FALSE;
}

BOOL CINFImageComboBoxEx::IsShowWindow()
{
	return m_bShowWnd;
}

void CINFImageComboBoxEx::ShowWindow(BOOL bShowWnd, POINT *pPos)
{
	m_bShowWnd = bShowWnd;	
	if(bShowWnd && pPos)
	{
		m_ptPos = (*pPos);
	}	
}

void CINFImageComboBoxEx::ShowItem(BOOL bShowItem)
{
	m_bShowItem = bShowItem;
	if(bShowItem && m_pScroll)
	{
		m_pScroll->SetMaxItem(m_vecComboInfo.size());
	}
}

BOOL CINFImageComboBoxEx::IsShowItem()
{
	return m_bShowItem;
}
int CINFImageComboBoxEx::GetCurSel()
{
	return m_nCurSel;
}

void CINFImageComboBoxEx::SetCurSel(int nCurSel)
{
	m_nCurSel = nCurSel;
}
void CINFImageComboBoxEx::SetPos(POINT ptPos)
{
	m_ptPos = ptPos;	

	{
		POINT ptSize = m_pImgComboBtn->GetImgSize();
		int nPosX, nPosY;		
		nPosX = m_ptPos.x + m_nWidth - ptSize.x;
		nPosY = m_ptPos.y;		
		m_pImgComboBtn->SetBtnPosition(nPosX, nPosY);
	}

	{		
		// 스크롤	
		RECT rcMouseWhell, rcMousePos;
		POINT ptScrollPos = ptPos;		
		
		ptScrollPos.x += (118);
		ptScrollPos.y += (19);
		int nItemSize = (int)m_vecComboInfo.size();
		if(nItemSize > MAX_ONE_PAGE)
		{
			nItemSize = MAX_ONE_PAGE;
		}
		int nHeight = nItemSize*m_nHeight;

		nHeight -= 38;
		
		m_pScroll->SetPosition(ptScrollPos.x ,ptScrollPos.y,11,nHeight);
		rcMouseWhell.left		= ptScrollPos.x - m_nWidth;
		rcMouseWhell.top		= ptScrollPos.y - 30;
		rcMouseWhell.right		= ptScrollPos.x + 30;
		rcMouseWhell.bottom		= ptScrollPos.y + 252;
		m_pScroll->SetMouseWhellRect(rcMouseWhell);
		rcMousePos.left			= ptScrollPos.x - 11;
		rcMousePos.top			= ptScrollPos.y ;
		rcMousePos.right		= rcMousePos.left + 32;
		rcMousePos.bottom		= rcMousePos.top + 282;
		m_pScroll->SetMouseBallRect(rcMousePos);
	}
}

BOOL CINFImageComboBoxEx::OnMouseWheel(POINT ptPos, WPARAM wParam, LPARAM lParam)
{
	if(!m_bShowItem)
	{
		return FALSE;
	}

	BOOL bClick = m_pScroll->IsMouseWhellPos(ptPos);
	if(bClick)		
	{		
		m_pScroll->OnMouseWheel(wParam, lParam);		
		return TRUE;
	}
	return FALSE;
}