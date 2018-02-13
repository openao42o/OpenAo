// INFItemMenuList.cpp: implementation of the CINFItemMenuList class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "AtumSound.h"

#include "INFWorldRankWnd.h"
#include "AtumDatabase.h"
#include "INFItemMenuList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFItemMenuList::CINFItemMenuList()
{
	m_pItemMixInfoBtn = NULL;		// 조합정보
	m_pArmorCollectionBtn = NULL;	// 아머 컬렉션 버튼 // 2013-05-28 by bhsohn 아머 컬렉션 시스템

	m_nShowItemNum = 0;
	m_uItemUniNum = 0;

}

CINFItemMenuList::~CINFItemMenuList()
{
	util::del(m_pItemMixInfoBtn);		// 조합정보
	util::del(m_pArmorCollectionBtn);	// 아머 컬렉션 버튼 // 2013-05-28 by bhsohn 아머 컬렉션 시스템

}

HRESULT CINFItemMenuList::InitDeviceObjects()
{
	char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];
	{
		wsprintf(szUpBtn, "reci_view3");
		wsprintf(szDownBtn, "reci_view1");
		wsprintf(szSelBtn, "reci_view0");
		wsprintf(szDisBtn, "reci_view2");
		
		if(NULL == m_pItemMixInfoBtn)
		{
			m_pItemMixInfoBtn = new CINFImageBtn;
			m_pItemMixInfoBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		}			
	}
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	{
		wsprintf(szUpBtn, "coll_view3");
		wsprintf(szDownBtn, "coll_view1");
		wsprintf(szSelBtn, "coll_view0");
		wsprintf(szDisBtn, "coll_view2");
		
		if(NULL == m_pArmorCollectionBtn)
		{
			m_pArmorCollectionBtn = new CINFImageBtn;
			m_pArmorCollectionBtn->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);
		}			
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템
	return S_OK ;
}
HRESULT CINFItemMenuList::RestoreDeviceObjects()
{
	int nWidth = 0;
	int nHeight = 0;
	if(m_pItemMixInfoBtn)
	{
		m_pItemMixInfoBtn->RestoreDeviceObjects();	
		nWidth += m_pItemMixInfoBtn->GetImgSize().x;
		nHeight += m_pItemMixInfoBtn->GetImgSize().y;
	}
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pArmorCollectionBtn)
	{
		m_pArmorCollectionBtn->RestoreDeviceObjects();
		nHeight += m_pArmorCollectionBtn->GetImgSize().y;
	}	
	SetSize(nWidth, nHeight);
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	return S_OK ;
}

HRESULT CINFItemMenuList::DeleteDeviceObjects()
{
	if(m_pItemMixInfoBtn)
	{
		m_pItemMixInfoBtn->DeleteDeviceObjects();
		util::del(m_pItemMixInfoBtn);
	}
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pArmorCollectionBtn)
	{
		m_pArmorCollectionBtn->DeleteDeviceObjects();
		util::del(m_pArmorCollectionBtn);
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	return S_OK ;
}

HRESULT CINFItemMenuList::InvalidateDeviceObjects()
{
	if(m_pItemMixInfoBtn)
	{
		m_pItemMixInfoBtn->InvalidateDeviceObjects();		
	}
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pArmorCollectionBtn)
	{
		m_pArmorCollectionBtn->InvalidateDeviceObjects();		
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	return S_OK ;
}

void CINFItemMenuList::Render()
{
	if(!IsShowWnd())
	{
		return;
	}

	if(m_pItemMixInfoBtn)
	{
		m_pItemMixInfoBtn->Render();		
	}
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pArmorCollectionBtn)
	{
		m_pArmorCollectionBtn->Render();		
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템
	CINFDefaultWnd::Render();
}
void CINFItemMenuList::Tick()
{
	if(!IsShowWnd())
	{
		return;
	}

	CINFDefaultWnd::Tick();

}

void CINFItemMenuList::ShowWnd(BOOL bShowWnd, INT nShowItemNum, UID64_t uItemUniNum, POINT *ptPos/*=NULL*/, int nWndWidth/*=0*/)
{
	m_nShowItemNum = nShowItemNum;
	m_uItemUniNum = uItemUniNum;

	CINFDefaultWnd::ShowWnd(bShowWnd, ptPos, nWndWidth);	

	if(bShowWnd)
	{
		UpdateBtnPos(0, 0);		// 버튼 위치 갱신		
	}
}

int CINFItemMenuList::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!IsShowWnd())
	{
		return INF_MSGPROC_NORMAL;
	}
	switch(uMsg)
	{
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(uMsg, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(uMsg, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(uMsg, wParam, lParam);
		}
		break;	
	}
	
	return INF_MSGPROC_NORMAL;

}
void CINFItemMenuList::UpdateBtnPos(int nWidth, int nHeight)
{
	int nPosY = m_ptBkPos.y;

	BOOL bShowMixInfo, bShowArmorCollect;
	bShowMixInfo = bShowArmorCollect = FALSE;

#ifdef C_INGAME_MIX_ITEM
	bShowMixInfo = TRUE;
#endif

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	bShowArmorCollect = FALSE;
	ITEM* pItem = g_pDatabase->GetItemInfoLoadItemData(m_nShowItemNum);
	if(pItem && (ITEMKIND_DEFENSE == pItem->Kind))
	{		 
		bShowArmorCollect = TRUE;
	}

	// 아레나에서는 안되게
	if(g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING)
	{
		bShowArmorCollect = FALSE;
	}
#endif
	
	if(bShowMixInfo && m_pItemMixInfoBtn)
	{
		m_pItemMixInfoBtn->ShowWindow(TRUE);
		m_pItemMixInfoBtn->SetBtnPosition(m_ptBkPos.x, m_ptBkPos.y);
		nPosY += m_pItemMixInfoBtn->GetImgSize().y;
	}
	else if(m_pItemMixInfoBtn)
	{
		m_pItemMixInfoBtn->ShowWindow(FALSE);
	}
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(bShowArmorCollect && m_pArmorCollectionBtn)
	{
//		nPosY -= 3;
		m_pArmorCollectionBtn->ShowWindow(TRUE);
		m_pArmorCollectionBtn->SetBtnPosition(m_ptBkPos.x, nPosY );
	}
	else if(m_pArmorCollectionBtn)
	{
		m_pArmorCollectionBtn->ShowWindow(FALSE);
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

//	CINFDefaultWnd::UpdateBtnPos(nWidth, nHeight);		
}

int CINFItemMenuList::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	
	{
		if(m_bMove)
		{
			m_bMove = FALSE;
			return INF_MSGPROC_BREAK;
		}		
	}		
	if(m_pItemMixInfoBtn && TRUE == m_pItemMixInfoBtn->OnLButtonUp(pt))
	{	
		g_pGameMain->OnClickItemMixWnd(TRUE, m_nShowItemNum, m_uItemUniNum);
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
		ShowWnd(FALSE, 0, 0);
		
		return  INF_MSGPROC_BREAK;
	}	
	
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	if(m_pArmorCollectionBtn && TRUE == m_pArmorCollectionBtn->OnLButtonUp(pt))
	{			
		g_pGameMain->OnClickArmorCollectionWnd(TRUE, m_nShowItemNum, m_uItemUniNum);
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);
		ShowWnd(FALSE, 0, 0);
		
		return  INF_MSGPROC_BREAK;
	}	
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템
	
	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	return INF_MSGPROC_NORMAL;
}

int CINFItemMenuList::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);	
	
	if(!IsWndRect(pt) && !m_bMove)
	{
		ShowWnd(FALSE,0, 0); // 다른곳 클릭시 숨기게 수정
		return INF_MSGPROC_NORMAL;
	}
	
	
	{
		if(m_pItemMixInfoBtn && TRUE == m_pItemMixInfoBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}	
	
	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	{
		if(m_pArmorCollectionBtn && TRUE == m_pArmorCollectionBtn->OnLButtonDown(pt))
		{
			// 버튼위에 마우스가 있다.
			return  INF_MSGPROC_BREAK;
		}		
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템
	
	return INF_MSGPROC_NORMAL;
	
	
}

int CINFItemMenuList::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);		

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	BOOL bRtn = m_pItemMixInfoBtn->OnMouseMove(pt);	
	if(m_pArmorCollectionBtn )
	{
		bRtn |= m_pArmorCollectionBtn->OnMouseMove(pt);
	}
	
	if(bRtn)	
	{
		g_pGameMain->SetToolTip(0,0,NULL);
		g_pGameMain->SetItemInfoUser( NULL, 0, 0 );
		
		return INF_MSGPROC_BREAK;
	}
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

	if(!IsWndRect(pt) && !m_bMove)
	{
		return INF_MSGPROC_NORMAL;
	}
	
	{
		if(m_bMove)
		{
			m_ptBkPos.x = pt.x - m_ptCommOpMouse.x;
			m_ptBkPos.y = pt.y - m_ptCommOpMouse.y;				
			// UI유저 지정 
			//UpdateBtnPos();
			return INF_MSGPROC_BREAK;
		}
	}
	return INF_MSGPROC_NORMAL;
	
}