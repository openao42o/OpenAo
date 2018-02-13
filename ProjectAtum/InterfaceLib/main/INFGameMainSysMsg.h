// INFGameMainSysMsg.h: interface for the CINFGameMainSysMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINSYSMSG_H__73A78D16_6209_40B1_8821_EB7AFCE81D6C__INCLUDED_)
#define AFX_INFGAMEMAINSYSMSG_H__73A78D16_6209_40B1_8821_EB7AFCE81D6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CINFImageEx;
#define		MAX_STRING_LINE		10			// 최대 라인
#define		MAX_BOX_IMAGE	9

class CINFGameMainSysMsg  : public CINFBase  
{
public:
	CINFGameMainSysMsg(CAtumNode* pParent);
	virtual ~CINFGameMainSysMsg();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	void Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL IsWndRect(POINT ptPos);
	BOOL IsShowWnd();
	BOOL IsMouseCaps(POINT ptPos);
	void ShowWnd(BOOL bShowWnd, POINT *ptPos=NULL, int nWndWidth=0);

	void InitString();
	void SetString(char* i_vecDesc);
	POINT GetWndPos();

	void UpdateString(int nIdx, char* pStr);


private:
	int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void RenderCenterWindow(int x, int y, int cx, int cy);

private:
	// 2008. 11. 21 by ckPark 서버 시간 보여주기, 박스 수정
	CINFImageEx*	m_pBoxImage;	// m_pBoxImage[MAX_BOX_IMAGE]

	char		m_szStringDesc[MAX_STRING_LINE][MAX_PATH];
	SIZE		m_nStringSize[MAX_STRING_LINE]; // 2012-12-10 by jhjang 시간 표시 수정

	BOOL		m_bShowWnd;
	int			m_nMaxLine;
	
	POINT					m_ptBkPos;	
	BOOL					m_bMove;
	POINT					m_ptCommOpMouse;

	int						m_nWidth;
	int						m_nHeight;

	CD3DHanFont		*		m_pFontTxt;
};

#endif // !defined(AFX_INFGAMEMAINSYSMSG_H__73A78D16_6209_40B1_8821_EB7AFCE81D6C__INCLUDED_)
