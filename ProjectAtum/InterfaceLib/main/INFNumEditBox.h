// INFNumEditBox.h: interface for the CINFNumEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFNUMEDITBOX_H__4EE1EDCB_E75B_4B39_B948_C9871617F003__INCLUDED_)
#define AFX_INFNUMEDITBOX_H__4EE1EDCB_E75B_4B39_B948_C9871617F003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFEditBox.h"

#include "INFBase.h"


class CINFNumEditBox    : public CINFEditBox
{
public:
	CINFNumEditBox();
	virtual ~CINFNumEditBox();

	virtual void Render();
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual void EnableEdit(BOOL bEnableEdit);	
	virtual BOOL OnLButtonDown(POINT ptPos);	

	void SetMaxStringLen(int nMaxStringLen);

private:
	BOOL OnKeyDown(WPARAM wParam, LPARAM lParam);	
	
	BOOL CheckNum(LPARAM lParam, char* pSmallKey, char* pBigKey);

private:
	// ÃÖ´ë ±Û¾¾ ¼ö 
	int				m_nMaxStringLen ;

};

#endif // !defined(AFX_INFNUMEDITBOX_H__4EE1EDCB_E75B_4B39_B948_C9871617F003__INCLUDED_)
