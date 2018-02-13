// INFSecuSetPassWnd.h: interface for the CINFSecuSetPassWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSECUSETPASSWND_H__3C05C8DA_7B32_4131_BEA8_574C03350CB4__INCLUDED_)
#define AFX_INFSECUSETPASSWND_H__3C05C8DA_7B32_4131_BEA8_574C03350CB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFBase.h"
#include "INFSecuEditBox.h"

class CINFImage;
class CD3DHanFont;
class CINFSecuMain;
class CINFGroupImage;


class CINFSecuSetPassWnd  : public CINFBase
{
public:
	CINFSecuSetPassWnd(CINFSecuMain* pMain);
	virtual ~CINFSecuSetPassWnd();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnCloseInfWnd();
	
	void ShowSetPassWnd();

private:
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void UpdateBtnPos(int nBackPosX, int nBackPosY);

	void OnClickOk();
	void OnClickCancel();

	void RefreshEditCtl(int nEditMode, CINFSecuEditBox* pIEdit);

private:
	CINFSecuMain*		m_pINFSecuMain;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*			m_pSetPassBk;
#else
	CINFImageEx*			m_pSetPassBk;
#endif

	int				m_nBackPosX;
	int				m_nBackPosY;

	int				m_nCx;
	int				m_nCy;

	BOOL			m_bMouseLock;
	POINT			m_ptMouseLockCap;

	CINFImageBtn*	m_pSetOk;
	CINFImageBtn*	m_pSetCancel;

	CINFSecuEditBox*	m_pINFSecuEditBox;
	CINFSecuEditBox*	m_pINFSecuComfirmEditBox;

	int				m_nSelEdit;
};

#endif // !defined(AFX_INFSECUSETPASSWND_H__3C05C8DA_7B32_4131_BEA8_574C03350CB4__INCLUDED_)
