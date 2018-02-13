// INFSecuEditBox.h: interface for the CINFSecuEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSECUEDITBOX_H__073AFF3C_8BB0_458B_A55F_640A0688D0DF__INCLUDED_)
#define AFX_INFSECUEDITBOX_H__073AFF3C_8BB0_458B_A55F_640A0688D0DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFEditBox.h"

#include "INFBase.h"

class CINFSecuEditBox  : public CINFEditBox
{
public:
	CINFSecuEditBox();
	virtual ~CINFSecuEditBox();	
	
	virtual void Render();
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual void EnableEdit(BOOL bEnableEdit);	
	virtual BOOL OnLButtonDown(POINT ptPos);	

	void SetMaxStringLen(int nMaxStringLen);

private:
	BOOL OnKeyDown(WPARAM wParam, LPARAM lParam);
	
	BOOL CheckAlphabet(LPARAM lParam, char* pSmallKey, char* pBigKey);
	BOOL CheckEtc(LPARAM lParam, char* pSmallKey, char* pBigKey);
	BOOL CheckNumPad(LPARAM lParam, char* pSmallKey, char* pBigKey);
	

private:
	// ÃÖ´ë ±Û¾¾ ¼ö 
	int				m_nMaxStringLen ;
	
};

#endif // !defined(AFX_INFSECUEDITBOX_H__073AFF3C_8BB0_458B_A55F_640A0688D0DF__INCLUDED_)
