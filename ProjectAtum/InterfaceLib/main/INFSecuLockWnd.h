// INFSecuLockWnd.h: interface for the CINFSecuLockWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSECULOCKWND_H__533680C1_BFD3_40F8_B64F_B699B0EECF1B__INCLUDED_)
#define AFX_INFSECULOCKWND_H__533680C1_BFD3_40F8_B64F_B699B0EECF1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CD3DHanFont;
class CINFSecuMain;
class CINFImageEx;											// 2011. 10. 10 by jskim UI시스템 변경
class CINFGroupImage;

class CINFSecuLockWnd   : public CINFBase
{
public:
	CINFSecuLockWnd(CINFSecuMain* pMain);
	virtual ~CINFSecuLockWnd();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitBtn();

	void UpdateLockBtn(BOOL bLock);

	void SetFirstMode(BOOL bFirstLoad);
private:
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void UpdateBtnPos(int nBackPosX, int nBackPosY);

	void OnClickSetupPass();
	void OnClickSetupChangePass();	
	

private:
	// 배경 이미지
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*		m_pLockBk;
#else
	CINFImageEx*		m_pLockBk;
#endif
	

	int				m_nBackPosX;
	int				m_nBackPosY;

	int				m_nCx;
	int				m_nCy;

	BOOL			m_bMouseLock;

	POINT			m_ptMouseLockCap;

	CINFImageBtn*			m_pLockBtn;
	CINFImageBtn*			m_pUnLockBtn;
	CINFImageBtn*			m_pLockSetupBtn;
	CINFImageBtn*			m_pLockChangeBtn;

	CINFSecuMain*			m_pINFSecuMain;	

	BOOL			m_bFirstSetup;
};

#endif // !defined(AFX_INFSECULOCKWND_H__533680C1_BFD3_40F8_B64F_B699B0EECF1B__INCLUDED_)
