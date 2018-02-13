// INFSecuPassWnd.h: interface for the CINFSecuPassWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSECUPASSWND_H__1DCA703B_C80E_4D86_B68B_AF9449BBA0E8__INCLUDED_)
#define AFX_INFSECUPASSWND_H__1DCA703B_C80E_4D86_B68B_AF9449BBA0E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFSecuEditBox.h"

class CINFImage;
class CD3DHanFont;
class CINFSecuMain;
class CINFImageEx;								// 2011. 10. 10 by jskim UI시스템 변경

class CINFSecuPassWnd  : public CINFBase
{
public:
	CINFSecuPassWnd(CINFSecuMain* pMain);
	virtual ~CINFSecuPassWnd();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ShowSetPassWnd(int nUnLockMode);
	void OnCloseInfWnd();

	void ClearEdit();

private:
	void RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose);

	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void UpdateBtnPos(int nBackPosX, int nBackPosY);

	void OnClickOk();
	void OnClickCancel();
	
	void RenderString(int nBackPosX, int nBackPosY);
private:
	CINFImageEx*	m_pBoxImage[9];
	CINFSecuMain*			m_pINFSecuMain;

	int				m_nBackPosX;
	int				m_nBackPosY;

	int				m_nCx;
	int				m_nCy;

	BOOL			m_bMouseLock;
	POINT			m_ptMouseLockCap;

	CINFImageBtn*	m_pSetOk;
	CINFImageBtn*	m_pSetCancel;

	CINFSecuEditBox*	m_pINFPassEditBox;
	CD3DHanFont	*		m_pTxtFont;

	int				m_nUnLockMode;
};

#endif // !defined(AFX_INFSECUPASSWND_H__1DCA703B_C80E_4D86_B68B_AF9449BBA0E8__INCLUDED_)
