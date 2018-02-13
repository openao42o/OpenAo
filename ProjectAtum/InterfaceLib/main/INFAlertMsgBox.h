// INFAlertMsgBox.h: interface for the CINFAlertMsgBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFALERTMSGBOX_H__491C7375_BC7F_4BD0_9331_F6D799A78F1B__INCLUDED_)
#define AFX_INFALERTMSGBOX_H__491C7375_BC7F_4BD0_9331_F6D799A78F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

typedef struct 
{
	int			nStartX;
	int			nStartY;
	char		strTxt[1024];	
} stMsgBoxString;

class CD3DHanFont;
class CINFImageBtn;
class CINFAlertMsgBox  : public CINFBase
{
public:
	CINFAlertMsgBox();
	virtual ~CINFAlertMsgBox();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void ShowAlertWnd(BOOL bShow, int nStartX, int nStartY);
	void AddString(int nStatX, int nStartY, char* pTxt);

	void ResetContent();
	
	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam, BOOL *o_bOk);

	BOOL IsShowPopup();
private:
	
private:
	CD3DHanFont	*			m_pInfWarning;
	CINFImageEx*			m_pImgWarning;						// 버튼 이미지

	int						m_nPosX;
	int						m_nPosY;
	
	vector<stMsgBoxString*>			m_vecString;			// 설명
	BOOL					m_bShowPopup;

	// 진형 선택
	CINFImageBtn*			m_pOkBtn;	
	CINFImageBtn*			m_pCancelBtn;	

};

#endif // !defined(AFX_INFALERTMSGBOX_H__491C7375_BC7F_4BD0_9331_F6D799A78F1B__INCLUDED_)
