#if !defined(AFX_MYCOMBOBOX_H__0FEE3345_886B_4491_819E_CEEF1C5FA41D__INCLUDED_)
#define AFX_MYCOMBOBOX_H__0FEE3345_886B_4491_819E_CEEF1C5FA41D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox window

class CMyComboBox : public CComboBox
{
// Construction
public:
	CMyComboBox();

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyComboBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyComboBox)
	afx_msg void OnSelchange();
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CBitmap		m_EditBitMap;		// 에디트 박스에 그려질 비트맵
	COLORREF	m_TextColor;		// 글자 색깔
	COLORREF	m_BackColor;		// 백그라운드 색깔

public:
	void	SetBitMap(const int nEditBitMapID);
	void	SetTextColor(const COLORREF dwTextColor);
	void	SetBackColor(const COLORREF dwBackColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCOMBOBOX_H__0FEE3345_886B_4491_819E_CEEF1C5FA41D__INCLUDED_)
