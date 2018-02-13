// INFDefaultWnd.h: interface for the CINFDefaultWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFDEFAULTWND_H__6C1691EC_1F96_4751_89DE_F98A596E895A__INCLUDED_)
#define AFX_INFDEFAULTWND_H__6C1691EC_1F96_4751_89DE_F98A596E895A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFDefaultWnd  : public CINFBase
{
public:
	CINFDefaultWnd();
	virtual ~CINFDefaultWnd();

	virtual HRESULT InitDeviceObjects(char* i_pBkFile);
	HRESULT RestoreDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	void	Render() override;
	void	Tick() override;
	
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	virtual BOOL IsWndRect(POINT ptPos);
	virtual BOOL IsShowWnd();
	virtual BOOL IsMouseCaps(POINT ptPos);
	virtual void ShowWnd(BOOL bShowWnd, POINT *ptPos=nullptr, int nWndWidth=0);

	virtual void SetSize(int i_nWidth,int i_nHeight);
	virtual POINT GetBkPos();

	virtual void UpdateBtnPos(int nWidth, int nHeight);
private:
	virtual int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CINFImageEx*			m_pDefaultBkImage;

	BOOL					m_bShowWnd;
	POINT					m_ptBkPos;	
	BOOL					m_bMove;
	POINT					m_ptCommOpMouse;
	
	int						m_nWidth;
	int						m_nHeight;

	CINFImageBtn*			m_pCloseBtn;

};

#endif // !defined(AFX_INFDEFAULTWND_H__6C1691EC_1F96_4751_89DE_F98A596E895A__INCLUDED_)
