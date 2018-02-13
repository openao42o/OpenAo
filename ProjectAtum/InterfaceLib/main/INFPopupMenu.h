// INFPopupMenu.h: interface for the CINFPopupMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFPOPUPMENU_H__20551D25_C230_425F_AECC_5F44F5160415__INCLUDED_)
#define AFX_INFPOPUPMENU_H__20551D25_C230_425F_AECC_5F44F5160415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImageBtn;

struct structMenuInfo
{
	int						nMenuId;
	BOOL					bShowMenu;	// 보일지 여부
	
	CINFImageBtn*			pImageBtn;
	char					pBtup[64];
	char					pBtDown[64];
	char					pSel[64];
	char					pDisable[64];		
};


class CINFPopupMenu  : public CINFBase
{
public:
	CINFPopupMenu();
	virtual ~CINFPopupMenu();

	HRESULT InitDeviceObjects();		

	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void Render();
	
	void ShowWindow(BOOL bShow, POINT *pPos);
	BOOL IsShowWindow();

	void AddMenu(int nMenuId, char* pBtup, char* pBtDown, char* pSel, char* pDisable);// 메뉴 추가
	void OnMouseMove(POINT pt);
	BOOL OnLButtonDown(POINT pt);
	int  OnLButtonUp(POINT pt);
	void SetMenuShow(int nMenuId, BOOL bShow);

private:
	vector<structMenuInfo*>		m_vecMenuBtn;

	BOOL	m_bShow;

	POINT			m_ptMenuPos;


};

#endif // !defined(AFX_INFPOPUPMENU_H__20551D25_C230_425F_AECC_5F44F5160415__INCLUDED_)
