// INFPopupMenu.cpp: implementation of the CINFPopupMenu class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "INFImageBtn.h"

#include "INFPopupMenu.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFPopupMenu::CINFPopupMenu()
{
	m_vecMenuBtn.clear();
	m_bShow = FALSE;
	m_ptMenuPos.x = m_ptMenuPos.y = 0;

}

CINFPopupMenu::~CINFPopupMenu()
{
	CINFPopupMenu::DeleteDeviceObjects();
}

HRESULT CINFPopupMenu::InitDeviceObjects()
{
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(NULL == pMenu)
		{
			itMenu++;
			continue;
		}
		if(NULL == pMenu->pImageBtn)
		{
			pMenu->pImageBtn= new CINFImageBtn;
			pMenu->pImageBtn->InitDeviceObjects(pMenu->pBtup, 
												pMenu->pBtDown, 
												pMenu->pSel, 
												pMenu->pDisable);
		}
		itMenu++;
	}
	return S_OK;
}
HRESULT CINFPopupMenu::RestoreDeviceObjects()
{
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(NULL == pMenu)
		{
			itMenu++;
			continue;
		}
		if(pMenu->pImageBtn)
		{
			pMenu->pImageBtn->RestoreDeviceObjects();			
		}		
		itMenu++;
	}
	return S_OK;
}
HRESULT CINFPopupMenu::DeleteDeviceObjects()
{
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(NULL == pMenu)
		{
			itMenu++;
			continue;
		}
		if(pMenu->pImageBtn)
		{
			pMenu->pImageBtn->DeleteDeviceObjects();
			util::del(pMenu->pImageBtn);
		}		
		util::del(*itMenu);
		itMenu++;
	}	
	m_vecMenuBtn.clear();


	return S_OK;
}
HRESULT CINFPopupMenu::InvalidateDeviceObjects()
{
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(NULL == pMenu)
		{
			itMenu++;
			continue;
		}
		if(pMenu->pImageBtn)
		{
			pMenu->pImageBtn->InvalidateDeviceObjects();			
		}		
		itMenu++;
	}
	return S_OK;
}

void CINFPopupMenu::Render()
{
	if(!IsShowWindow())
	{
		return;
	}
	int nPosX, nPosY;
	nPosX = m_ptMenuPos.x;
	nPosY = m_ptMenuPos.y;

	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(pMenu && pMenu->pImageBtn && pMenu->pImageBtn->IsShowWindow())
		{
			pMenu->pImageBtn->SetBtnPosition(nPosX, nPosY);
			pMenu->pImageBtn->Render();
			nPosY += pMenu->pImageBtn->GetImgSize().y;
		}		
		itMenu++;
	}

}

void CINFPopupMenu::AddMenu(int nMenuId, char* pBtup, char* pBtDown, char* pSel, char* pDisable)
{
	structMenuInfo* pMenuInfo = new structMenuInfo;

	pMenuInfo->nMenuId = nMenuId;
	
	pMenuInfo->bShowMenu = TRUE;

	pMenuInfo->pImageBtn = NULL;

	memset(pMenuInfo->pBtup, 0x00, 64);
	strncpy(pMenuInfo->pBtup, pBtup, strlen(pBtup)+1);

	memset(pMenuInfo->pBtDown, 0x00, 64);
	strncpy(pMenuInfo->pBtDown, pBtDown, strlen(pBtDown)+1);

	memset(pMenuInfo->pSel, 0x00, 64);
	strncpy(pMenuInfo->pSel, pSel, strlen(pSel)+1);

	memset(pMenuInfo->pDisable, 0x00, 64);
	strncpy(pMenuInfo->pDisable, pDisable, strlen(pDisable)+1);	

	m_vecMenuBtn.push_back(pMenuInfo);
}

void CINFPopupMenu::ShowWindow(BOOL bShow, POINT *pPos)
{	
	m_bShow = bShow;
	if(bShow && pPos)
	{
		m_ptMenuPos = (*pPos);
	}
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(pMenu && pMenu->pImageBtn)
		{			
			if(!bShow)
			{
				pMenu->pImageBtn->ShowWindow(FALSE);
			}
			else if(pMenu->bShowMenu && bShow)
			{
				pMenu->pImageBtn->ShowWindow(TRUE);
			}
			else
			{
				pMenu->pImageBtn->ShowWindow(FALSE);
			}
		}		
		itMenu++;
	}
}

BOOL CINFPopupMenu::IsShowWindow()
{
	return m_bShow ;
}

void CINFPopupMenu::OnMouseMove(POINT pt)
{
	if(!IsShowWindow())
	{
		return;
	}
	
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(pMenu && pMenu->pImageBtn)
		{			
			pMenu->pImageBtn->OnMouseMove(pt);
		}		
		itMenu++;
	}
}

BOOL CINFPopupMenu::OnLButtonDown(POINT pt)
{
	if(!IsShowWindow())
	{
		return FALSE;
	}

	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(pMenu && pMenu->pImageBtn)
		{			
			if(pMenu->pImageBtn->OnLButtonDown(pt))
			{
				return TRUE;
			}
		}		
		itMenu++;
	}
	return FALSE;
}

int CINFPopupMenu::OnLButtonUp(POINT pt)
{
	if(!IsShowWindow())
	{
		return -1;
	}

	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(pMenu && pMenu->pImageBtn)
		{			
			if(pMenu->pImageBtn->OnLButtonUp(pt))
			{
				return pMenu->nMenuId;
			}
		}		
		itMenu++;
	}
	return -1;
}

void CINFPopupMenu::SetMenuShow(int nMenuId, BOOL bShowMenu)
{
	vector<structMenuInfo*>::iterator itMenu = m_vecMenuBtn.begin();
	while(itMenu != m_vecMenuBtn.end())
	{
		structMenuInfo* pMenu = (*itMenu);
		if(pMenu && pMenu->nMenuId == nMenuId)
		{
			(*itMenu)->bShowMenu = bShowMenu;
		}
		itMenu++;
	}

}