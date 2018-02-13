#if !defined(AFX_MYCHECK_H__439F4193_4824_41F0_AFA0_0E6FC4D0799D__INCLUDED_)
#define AFX_MYCHECK_H__439F4193_4824_41F0_AFA0_0E6FC4D0799D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCheck.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCheck window

class CMyCheck : public CButton
{
// Construction
public:
	CMyCheck();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCheck)
	virtual int		GetCheck();
	virtual void	SetCheck(int nCheck);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyCheck();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCheck)
		// NOTE - the ClassWizard will add and remove member functions here.
 	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnPaint();
 	afx_msg LRESULT OnChange(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void	OnEnable(BOOL bEnable);
 	//}}AFX_MSG

 	DECLARE_MESSAGE_MAP()

public:
	enum {CHECKBOX_TOTAL_STATE = 2};				// 체크 박스 상태 종류
	enum {CHECKBOX_STATE_UNCHECKED = 0,				// 체크 안됨
		CHECKBOX_STATE_CHECKED};					// 체크
	
private:
	BOOL	m_bMyCheckState;						// 체크 박스 상태 변수
	
	CBitmap	m_CheckStateBMP[CHECKBOX_TOTAL_STATE];	// 체크/비체크시 비트맵

public:
	void	SetBitMap(const int nUnCheckedBMPID, const int nCheckedBMPID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCHECK_H__439F4193_4824_41F0_AFA0_0E6FC4D0799D__INCLUDED_)
