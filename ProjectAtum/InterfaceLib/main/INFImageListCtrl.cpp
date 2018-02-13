// INFImageListCtrl.cpp: implementation of the CINFImageListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"

#include "INFImageListCtrl.h"
#include "INFImageEx.h"										 // 2011. 10. 10 by jskim UI시스템 변경

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 격추수로 정렬
struct Mission_Sort_MainIdx: binary_function<stImageListCtrlIcon*, stImageListCtrlIcon*, bool>
{
	bool operator()(stImageListCtrlIcon* pListItem1, stImageListCtrlIcon* pListItem2)
	{
        if(pListItem1->nMainIdx < pListItem2->nMainIdx)
		{
			return TRUE;
		}
		else if(pListItem1->nMainIdx == pListItem2->nMainIdx)
		{
			if(pListItem1->nSubIdx < pListItem2->nSubIdx)
			{
				return TRUE;
			}
		}
		return FALSE;
    };
};


#define	SCROLL_BALL_WIDTH					11
#define	SCROLL_BALL_SCROLL_CAP				100

CINFImageListCtrl::CINFImageListCtrl()
{	
	m_vecMainImage.clear();
	m_fPosX = m_fPosY = 0;
	m_fWidth = m_fHeight = 1;
	m_fSubItemStartX = 0;
	m_fSubItemStartY = 0;
	m_fScrollStartX = m_fScrollStartY = 0;

	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_LIST_SHOW;nCnt++)
	{
		m_pListShowImage[nCnt] = NULL;
		memset(&m_ptIconSize[nCnt],0x00, sizeof(POINT));
	}		
	memset(&m_ptSubItemSize,0x00, sizeof(POINT));
	
	m_pSubItemImage = NULL;
	m_pFontSubItem = NULL;
	m_pINFScrollBar = NULL;
	m_pSelItemImage = NULL;
	m_nScrollStep = 100;

	m_ptSelPoint.x = m_ptSelPoint.y= -1;
}

CINFImageListCtrl::~CINFImageListCtrl()
{
	CINFImageListCtrl::DeleteDeviceObjects();
}


HRESULT CINFImageListCtrl::InitDeviceObjects(int nScrollStep)
{
	m_nScrollStep = nScrollStep;
	// 스크롤바
	{
		char  szScBall[30];	
		if(NULL == m_pINFScrollBar)
		{
			m_pINFScrollBar = new CINFArenaScrollBar;
		}		
		wsprintf(szScBall,"c_scrlb");
		
		m_pINFScrollBar->InitDeviceObjects(m_nScrollStep, szScBall);
	}

	m_pFontSubItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, TRUE,1024,32);
	m_pFontSubItem->InitDeviceObjects(g_pD3dDev);

	return S_OK;
}

HRESULT CINFImageListCtrl::RestoreDeviceObjects()
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(pItem->pInfImage)
		{
			pItem->pInfImage->RestoreDeviceObjects();
		}		
		it++;
	}
	
	int nCnt = 0;	
	{
		for(nCnt = 0;nCnt < MAX_LIST_SHOW;nCnt++)
		{
			m_pListShowImage[nCnt]->RestoreDeviceObjects();
			m_ptIconSize[nCnt] = m_pListShowImage[nCnt]->GetImgSize();
		}
	}	
	m_pSubItemImage->RestoreDeviceObjects();
	m_ptSubItemSize = m_pSubItemImage->GetImgSize();

	m_pSelItemImage->RestoreDeviceObjects();
	
	m_pFontSubItem->RestoreDeviceObjects();
	
	{
		m_pINFScrollBar->RestoreDeviceObjects();		
	}
	return S_OK;
}

HRESULT CINFImageListCtrl::DeleteDeviceObjects()
{
	DeleteaAllItem();	
	int nCnt = 0;
	{
		for(nCnt = 0;nCnt < MAX_LIST_SHOW;nCnt++)
		{
			if(m_pListShowImage[nCnt])
			{
				m_pListShowImage[nCnt]->DeleteDeviceObjects();
				util::del(m_pListShowImage[nCnt]);
			}			
		}
	}
	if(m_pSubItemImage)
	{
		m_pSubItemImage->DeleteDeviceObjects();
		util::del(m_pSubItemImage);
	}
	
	if(m_pSelItemImage)
	{
		m_pSelItemImage->DeleteDeviceObjects();
		util::del(m_pSelItemImage);
	}
	

	if(m_pFontSubItem)
	{
		m_pFontSubItem->DeleteDeviceObjects();
		util::del(m_pFontSubItem);
	}

	if(m_pINFScrollBar)
	{
		m_pINFScrollBar->DeleteDeviceObjects();	
		util::del(m_pINFScrollBar);
	}
	
	return S_OK;
}

HRESULT CINFImageListCtrl::InvalidateDeviceObjects()
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(pItem->pInfImage)
		{
			pItem->pInfImage->InvalidateDeviceObjects();
		}
		it++;
	}
	int nCnt = 0;
	{
		for(nCnt = 0;nCnt < MAX_LIST_SHOW;nCnt++)
		{
			m_pListShowImage[nCnt]->InvalidateDeviceObjects();
		}
	}
	m_pSubItemImage->InvalidateDeviceObjects();
	m_pSelItemImage->InvalidateDeviceObjects();
	m_pFontSubItem->InvalidateDeviceObjects();
	m_pINFScrollBar->InvalidateDeviceObjects();
	return S_OK;
}

void CINFImageListCtrl::Render()
{
	RenderListItem();
	m_pINFScrollBar->Render();
}
void CINFImageListCtrl::RenderListItem()
{		
	float fPosX, fPosY;
	fPosX = m_fPosX;
	fPosY = m_fPosY;	

	int nItemNonEpTitle = (int)(m_ptIconSize[LIST_SHOW_PLUS].x);

	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	int nCnt=0;
	int nShowItemCnt = 0;	
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		CINFImage* pImage = pItem->pInfImage;			

		if(FALSE == pItem->bShow)
		{
			it++;
			continue;
		}		
		if(nCnt < m_pINFScrollBar->GetScrollStep())
		{
			nCnt++;
			it++;
			continue;
		}	

		// 그리는 최대수 
		if(nShowItemCnt > m_nScrollStep)
		{
			break;
		}	
		nShowItemCnt++;	

		fPosX = m_fPosX;
		fPosY = pItem->fPosY;	

		// 헤더 정보 
		if( 0 == pItem->nSubIdx)
		{
			if(!pItem->bEpTitle)
			{				
				fPosX += nItemNonEpTitle;
			}

			if(pItem->bExtend)
			{
				m_pListShowImage[LIST_SHOW_MINUS]->Move(fPosX, fPosY);
				m_pListShowImage[LIST_SHOW_MINUS]->Render();				
			}
			else
			{
				m_pListShowImage[LIST_SHOW_PLUS]->Move(fPosX, fPosY);
				m_pListShowImage[LIST_SHOW_PLUS]->Render();				
			}
			if(pImage)
			{
				pImage->Move(pItem->fPosX, pItem->fPosY);
				pImage->Render();
			}			
		}
		else
		{			
			m_pSubItemImage->Move(m_fPosX + m_fSubItemStartX, pItem->fPosY + m_fSubItemStartY);
			m_pSubItemImage->Render();			

			BOOL bSelect = FALSE;
			if((m_ptSelPoint.x == pItem->nMainIdx )&&(m_ptSelPoint.y == pItem->nSubIdx))
			{
				bSelect = TRUE;
			}
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
			char chBuf[512], chResultBuf[512];
			chBuf[0] = NULL;
			memset(chResultBuf,0x00,512);

			if(bSelect)
			{					
				wsprintf(chBuf, "\\e%s", pItem->szItemTxt);
			}
			else
			{
				strncpy(chBuf, pItem->szItemTxt, strlen(pItem->szItemTxt)+1);
			}
			if( strlen(chBuf) > 25 )
			{
				char chTemp[512];
				memset(chTemp,0x00,512);
 				strncpy( chResultBuf, chBuf, 25 );
 				chResultBuf[ GetStringBuffPos(chResultBuf, GetStringBuffLen( chResultBuf ) - 1) ]='\0';
 				sprintf(chBuf,"%s..", chResultBuf);
			}  
#else
			char chBuf[512];
			chBuf[0] = NULL;

			if(bSelect)
			{					
				wsprintf(chBuf, "\\e%s", pItem->szItemTxt);
			}
			else
			{
				strncpy(chBuf, pItem->szItemTxt, strlen(pItem->szItemTxt)+1);
			}
#endif
			
			// 헤더가 아닌정보 
			m_pFontSubItem->DrawText(pItem->fPosX, pItem->fPosY, 
										pItem->dwColor,
										chBuf, 0L);				
			// 선택한 아이템이냐?
			if(bSelect)
			{
				m_pSelItemImage->Move(m_fPosX + m_fSubItemStartX, pItem->fPosY );
				m_pSelItemImage->Render();
			}
		}
		it++;
	}
}
void CINFImageListCtrl::InitDeviceEtc(char* pPlus, char* pMinus, char* pSmallIcon, char* pSelIcon)
{
	DataHeader	* pDataHeader = NULL;
	// 
	{
		if(NULL == m_pListShowImage[LIST_SHOW_PLUS])
		{
			m_pListShowImage[LIST_SHOW_PLUS] = new CINFImageEx;
		}		
		pDataHeader = g_pGameMain->FindResource(pPlus);
		m_pListShowImage[LIST_SHOW_PLUS]->InitDeviceObjects(pDataHeader);
	}	
	{
		if(NULL == m_pListShowImage[LIST_SHOW_MINUS])
		{
			m_pListShowImage[LIST_SHOW_MINUS] = new CINFImageEx;
		}		
		pDataHeader = g_pGameMain->FindResource(pMinus);
		m_pListShowImage[LIST_SHOW_MINUS]->InitDeviceObjects(pDataHeader);				
	}
	{
		if(NULL == m_pSubItemImage)
		{
			m_pSubItemImage = new CINFImageEx;
		}		
		pDataHeader = g_pGameMain->FindResource(pSmallIcon);
		m_pSubItemImage->InitDeviceObjects(pDataHeader);				
	}
	// 선책 상자
	{
		if(NULL == m_pSelItemImage)
		{
			m_pSelItemImage = new CINFImageEx;
		}		
		pDataHeader = g_pGameMain->FindResource(pSelIcon);
		m_pSelItemImage->InitDeviceObjects(pDataHeader);				
	}


}
void CINFImageListCtrl::InserItem(int nEpId, int nMainIdx, char* pItemName, BOOL bEpTitle)
{
	stImageListCtrlIcon* pImageListCtrlIcon = new stImageListCtrlIcon;
	pImageListCtrlIcon->nMainIdx	= nMainIdx;
	pImageListCtrlIcon->nSubIdx		= 0;
	pImageListCtrlIcon->fPosX		= 0;
	pImageListCtrlIcon->fPosY		= 0;
	pImageListCtrlIcon->fWidth		= 1;
	pImageListCtrlIcon->fHeight		= 1;
	if(bEpTitle)
	{
		pImageListCtrlIcon->bShow		= TRUE;
	}
	else
	{
		pImageListCtrlIcon->bShow		= FALSE;
	}
	pImageListCtrlIcon->bExtend		= FALSE;	
	pImageListCtrlIcon->dwColor		= 0;	
	memset(pImageListCtrlIcon->szItemTxt, 0x00, 256);
	pImageListCtrlIcon->nEpId		= nEpId;
	pImageListCtrlIcon->bEpTitle	= bEpTitle;
	pImageListCtrlIcon->pInfImage = new CINFImageEx;	

	DataHeader	* pDataHeader = NULL;
	pDataHeader = g_pGameMain->FindResource(pItemName);
	pImageListCtrlIcon->pInfImage->InitDeviceObjects(pDataHeader);		

	int nSize = m_vecMainImage.size();

	m_vecMainImage.push_back(pImageListCtrlIcon);	
}

void CINFImageListCtrl::InserSubItem(int nEpId, int nMainIdx, int nSubIdx, char *szItemTitle, DWORD dwColor)
{
	stImageListCtrlIcon* pImageListCtrlIcon = new stImageListCtrlIcon;
	pImageListCtrlIcon->nMainIdx	= nMainIdx;
	pImageListCtrlIcon->nSubIdx		= nSubIdx;
	pImageListCtrlIcon->fPosX		= 0;
	pImageListCtrlIcon->fPosY		= 0;
	pImageListCtrlIcon->fWidth		= 1;
	pImageListCtrlIcon->fHeight		= 1;
	pImageListCtrlIcon->bShow		= FALSE;
	pImageListCtrlIcon->bExtend		= FALSE;	
	pImageListCtrlIcon->dwColor		= dwColor;	
	pImageListCtrlIcon->nEpId		= nEpId;

	memset(pImageListCtrlIcon->szItemTxt, 0x00, 256);
	strncpy(pImageListCtrlIcon->szItemTxt, szItemTitle, strlen(szItemTitle)+1);

	int nSize = m_vecMainImage.size();

	pImageListCtrlIcon->pInfImage	= NULL;	

	m_vecMainImage.push_back(pImageListCtrlIcon);
	
}
void CINFImageListCtrl::SortListCtrlItem()
{
	sort(m_vecMainImage.begin(), m_vecMainImage.end(), Mission_Sort_MainIdx());		
}

int CINFImageListCtrl::GetMaxShowItem()
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	int nCnt=0;
	int nShowItem = 0;
	
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(TRUE == pItem->bShow)
		{
			nShowItem++;
		}
		it++;
	}
	return nShowItem;
}

void CINFImageListCtrl::UpdateItemPos()
{
	float fPosX, fPosY;
	fPosX = m_fPosX;
	fPosY = m_fPosY;
	
	int nItemWidth[MAX_LIST_SHOW], nItemHeight[MAX_LIST_SHOW];
	int nItemNonEpTitle = (int)(m_ptIconSize[LIST_SHOW_PLUS].x);

	nItemWidth[LIST_SHOW_PLUS]		= (int)(m_ptIconSize[LIST_SHOW_PLUS].x*1.5);
	nItemWidth[LIST_SHOW_MINUS]		= (int)(m_ptIconSize[LIST_SHOW_MINUS].x*1.5);
	
	nItemHeight[LIST_SHOW_PLUS]		= (int)(m_ptIconSize[LIST_SHOW_PLUS].y*1.5);
	nItemHeight[LIST_SHOW_MINUS]	= (int)(m_ptIconSize[LIST_SHOW_MINUS].y*1.5);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	int nSubItemIconWidth	=			(int)(m_ptSubItemSize.x*1.1);	
#else 
	int nSubItemIconWidth	=			(int)(m_ptSubItemSize.x*1.5);	
#endif

	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	int nCnt=0;
	
	while(it != m_vecMainImage.end())
	{		
		stImageListCtrlIcon* pItem = (*it);
		CINFImage* pImage = pItem->pInfImage;
		if(FALSE == pItem->bShow)
		{
			pItem->fPosX		= 0;
			pItem->fPosY		= -100;
			pItem->fWidth		= 1;
			pItem->fHeight		= 1;
			it++;
			continue;
		}			
		
		if(nCnt < m_pINFScrollBar->GetScrollStep())
		{
			nCnt++;
			it++;
			continue;

		}		

		pItem->fPosX = fPosX;

		// 헤더 정보 
		if(0 == pItem->nSubIdx)
		{
			pItem->fPosX += nItemWidth[LIST_SHOW_PLUS];				
			if(!pItem->bEpTitle)
			{
				pItem->fPosX += nItemNonEpTitle;
			}
		}
		else
		{
			pItem->fPosX += m_fSubItemStartX + nSubItemIconWidth;
		}
		pItem->fPosY = fPosY;
		fPosY += nItemHeight[LIST_SHOW_PLUS];
		if(pImage)
		{
			pItem->fWidth	= pImage->GetImgSize().x;
			pItem->fHeight	= pImage->GetImgSize().y;
		}
		else
		{
			pItem->fWidth	= m_fWidth;
			pItem->fHeight	= nItemHeight[LIST_SHOW_MINUS];
		}				
		it++;
	}			
}

void CINFImageListCtrl::SetListCtrlPos(float fPosX, float fPosY, 
										float fSubItemStartX, float fSubItemStartY, 
										float fScrollStartX, float fScrollStartY, 
										float fWidth,float fHeight)
{
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	m_fSubItemStartX = fSubItemStartX;
	m_fSubItemStartY = fSubItemStartY;

	m_fScrollStartX = fScrollStartX;
	m_fScrollStartY = fScrollStartY;

	UpdateScrollPos();

	// 리스트 컨트롤 위치 지정
	UpdateItemPos();
}

void CINFImageListCtrl::UpdateScrollPos()
{	
	float fPosX, fPosY;
	fPosX = m_fPosX;
	fPosY = m_fPosY;			
	
	POINT ptScroll;
	RECT rcMouseWhell, rcMousePos;
	
	// 휠 위치지정 
	rcMouseWhell.left		= fPosX ;
	rcMouseWhell.top		= fPosY;
	rcMouseWhell.right		= rcMouseWhell.left + m_fWidth;
	rcMouseWhell.bottom		= rcMouseWhell.top + m_fHeight;
	
	
	// 마우스 위치 지정
	rcMousePos.left			= fPosX + m_fScrollStartX;
	rcMousePos.top			= fPosY + m_fScrollStartY;
	rcMousePos.right		= rcMousePos.left;
	rcMousePos.bottom		= rcMousePos.top;
		
	// Resotre를 해야지만 이미지 크기를 알수 있다. 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	m_pINFScrollBar->SetPosition(rcMousePos.left, rcMousePos.top, SCROLL_BALL_WIDTH, m_fHeight - 23 );
#else		  
	m_pINFScrollBar->SetPosition(rcMousePos.left, rcMousePos.top, SCROLL_BALL_WIDTH, m_fHeight);
#endif
	m_pINFScrollBar->SetMouseWhellRect(rcMouseWhell);			

	ptScroll = m_pINFScrollBar->GetImgBkSize();
	
	rcMousePos.bottom		= rcMousePos.top + ptScroll.y + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.top			= SCROLL_BALL_SCROLL_CAP;
	rcMousePos.right		= rcMousePos.left + SCROLL_BALL_SCROLL_CAP;
	rcMousePos.left			-= SCROLL_BALL_SCROLL_CAP;					
	
	m_pINFScrollBar->SetMouseBallRect(rcMousePos);
}

BOOL CINFImageListCtrl::OnLButtonDown(POINT pt, int* o_nMainIdx, int* o_nSubIdx)
{		
	(*o_nMainIdx) = (*o_nSubIdx) = -1;

	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	int nCnt=0;
	while(it != m_vecMainImage.end())
	{
		// 2009-04-07 by bhsohn 미션 트리 창 버그 수정
		stImageListCtrlIcon* pItem = (*it);
		if(FALSE == pItem->bShow)
		{
			it++;
			continue;
		}		
		// end 2009-04-07 by bhsohn 미션 트리 창 버그 수정

		if(nCnt < m_pINFScrollBar->GetScrollStep())
		{
			nCnt++;
			it++;
			continue;
		}	

		//stImageListCtrlIcon* pItem = (*it);
		if((m_fPosX <= pt.x)
			&&(pt.x <= (m_fPosX+pItem->fWidth)))
		{		
			if((pItem->fPosY <= pt.y)
				&&(pt.y <= (pItem->fPosY+pItem->fHeight)))
			{
				(*o_nMainIdx) = pItem->nMainIdx;
				(*o_nSubIdx) = pItem->nSubIdx;

				if(0 == pItem->nSubIdx)
				{
					pItem->bExtend ^= TRUE;
					PopupItem(pItem->bExtend, pItem->nMainIdx, pItem->bEpTitle, pItem->nEpId);
				}
				else
				{
					// 선택한 아이템 저장
					SetSelPoint(pItem->nMainIdx, pItem->nSubIdx);
				}
				
				return TRUE;
			}
		}
		it++;
	}
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

BOOL CINFImageListCtrl::OnLButtonUp(POINT pt)
{
	BOOL bClick = m_pINFScrollBar->GetMouseMoveMode();
	if(bClick)		
	{
		m_pINFScrollBar->SetMouseMoveMode(FALSE);			
	}		
	return bClick;
}
BOOL CINFImageListCtrl::OnMouseMove(POINT pt)
{
	if(m_pINFScrollBar->GetMouseMoveMode())
	{
		if(FALSE == m_pINFScrollBar->IsMouseScrollPos(pt))
		{
			m_pINFScrollBar->SetMouseMoveMode(FALSE);
			return FALSE;			
		}
		m_pINFScrollBar->SetScrollPos(pt);
		UpdateItemPos();
		return TRUE;			
	}
	return FALSE;	

}
BOOL CINFImageListCtrl::OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam)
{
	BOOL bClick = m_pINFScrollBar->IsMouseWhellPos(pt);
	if(bClick)		
	{			
		m_pINFScrollBar->OnMouseWheel(wParam, lParam);	
		UpdateItemPos();		
	}
	return bClick;
}

void CINFImageListCtrl::PopupItem(BOOL bPopup, int nMainIdx, BOOL bEpTitle, int nEpId)
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(bEpTitle)
		{
			// 같은 에피소드 아이디만 표시 한다.
			if(pItem->nEpId == nEpId)
			{
				if(pItem->bEpTitle)
				{
					(*it)->bExtend = bPopup;
					(*it)->bShow = TRUE;
				}
				else
				{
					if(pItem->nSubIdx == 0)
					{
						// 
						(*it)->bExtend = FALSE;
						(*it)->bShow = bPopup;
					}
					else
					{
						(*it)->bShow = FALSE;
					}					
				}
			}
		}
		else
		{
			if(pItem->nMainIdx == nMainIdx)
			{				
				if(pItem->nSubIdx == 0)
				{				
					if(!pItem->bEpTitle)
					{
						(*it)->bExtend = bPopup;
						(*it)->bShow = TRUE;
					}									
				}
				else
				{
					(*it)->bShow = bPopup;
				}		
			}		
		}		
		it++;
	}		
	int nShowItem = GetMaxShowItem();
	m_pINFScrollBar->SetOnlyMaxItem(nShowItem);	
	// 리스트 컨트롤 위치 지정
	UpdateItemPos();	
	
}
// 서브 타이틀을 보여준다.
void CINFImageListCtrl::ShowTitleAllEpId(BOOL bShow, int nEpId)	
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(pItem->nEpId == nEpId 
			&& 0 == pItem->nSubIdx
			&& !pItem->bEpTitle)
		{
			(*it)->bShow = bShow;
		}
		it++;
	}

	int nShowItem = GetMaxShowItem();
	m_pINFScrollBar->SetOnlyMaxItem(nShowItem);	
	// 리스트 컨트롤 위치 지정
	UpdateItemPos();	

}

void CINFImageListCtrl::DeleteaAllItem()
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(pItem->pInfImage)
		{
			pItem->pInfImage->DeleteDeviceObjects();
			util::del(pItem->pInfImage);
		}
		util::del(pItem);		
		it++;
	}
	m_vecMainImage.clear();	
}

void CINFImageListCtrl::RestoreItemDeviceObjects()
{
	vector<stImageListCtrlIcon*>::iterator it = m_vecMainImage.begin();
	while(it != m_vecMainImage.end())
	{
		stImageListCtrlIcon* pItem = (*it);
		if(pItem->pInfImage)
		{
			pItem->pInfImage->RestoreDeviceObjects();
		}		
		it++;
	}
	int nShowItem = GetMaxShowItem();
	m_pINFScrollBar->SetOnlyMaxItem(nShowItem);	
}

void CINFImageListCtrl::ResetContent()
{
	DeleteaAllItem();
}

void CINFImageListCtrl::SetSelPoint(int nMainIdx,int nSubIdx)
{
	m_ptSelPoint.x = nMainIdx;
	m_ptSelPoint.y = nSubIdx;
}