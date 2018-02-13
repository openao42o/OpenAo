// INFItemMenuList.h: interface for the CINFItemMenuList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFITEMMENULIST_H__D39D8C6A_32D4_49C0_9B1C_420F95450F0E__INCLUDED_)
#define AFX_INFITEMMENULIST_H__D39D8C6A_32D4_49C0_9B1C_420F95450F0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFDefaultWnd.h"

class CINFDefaultWnd;
class CINFImageBtn;

class CINFItemMenuList : public CINFDefaultWnd
{
public:
	CINFItemMenuList();
	virtual ~CINFItemMenuList();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual void ShowWnd(BOOL bShowWnd, INT nShowItemNum, UID64_t uItemUniNum, POINT* ptPos = NULL, int nWndWidth = 0);

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void UpdateBtnPos(int nWidth, int nHeight);

	virtual int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	CINFImageBtn* m_pItemMixInfoBtn; // 조합정보
	CINFImageBtn* m_pArmorCollectionBtn; // 아머 컬렉션 버튼 // 2013-05-28 by bhsohn 아머 컬렉션 시스템

	INT m_nShowItemNum;
	UID64_t m_uItemUniNum;
};

#endif // !defined(AFX_INFITEMMENULIST_H__D39D8C6A_32D4_49C0_9B1C_420F95450F0E__INCLUDED_)


