// INFOpSmallHelper.h: interface for the CINFOpSmallHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPSMALLHELPER_H__4B4B9082_DCC2_4D82_934B_4CEB30F17E7D__INCLUDED_)
#define AFX_INFOPSMALLHELPER_H__4B4B9082_DCC2_4D82_934B_4CEB30F17E7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFImageBtn.h"

#define HELPER_INFO_BALON			10		// 정보창 테두리

class CINFOpUserHelper;

class CINFOpSmallHelper  
{
public:
	CINFOpSmallHelper(CINFOpUserHelper* pParent);
	virtual ~CINFOpSmallHelper();

	HRESULT InitDeviceObjects(int nFontWidth);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 글씨를 쓴다.
	void SetHelperString(int nPosX, int nPosY, int nBubbleX, char* pTxt, int nStrCapWidth, int nShowModeBtn);	

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
private:
	void RenderBkBalon(int x, int y, int cx, int cy, int nBubblePosX);
private:
	// 타이틀
	vector<string>				m_vecText;
	SIZE				m_sizeMaxStrSize;

	// 시작점
	float					m_fStartX;
	float					m_fStartY;
	// 말풍선 크기
	int						m_nBalonWidth;
	int						m_nBalonHeight;

	CD3DHanFont*	m_pFontHelpInfo;

	CINFImageBtn*			m_pSmallStartBtn;
	CINFImageBtn*			m_pNextBtn;
	CINFImageBtn*			m_pGoMainBtn;
	CINFImageBtn*			m_pCloseBtn;
	
	CINFImageEx	*	m_pBalonChat[HELPER_INFO_BALON];	

	CINFOpUserHelper*		m_pParent;

	int		m_nBubblePosX;

	int		m_nHelpTxtPosX;
	int		m_nHelpTxtPosY;

};

#endif // !defined(AFX_INFOPSMALLHELPER_H__4B4B9082_DCC2_4D82_934B_4CEB30F17E7D__INCLUDED_)
