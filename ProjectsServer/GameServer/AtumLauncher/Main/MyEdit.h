#if !defined(AFX_MYEDIT_H__A6EDB6D3_E5F9_416B_A512_A62AF6146819__INCLUDED_)
#define AFX_MYEDIT_H__A6EDB6D3_E5F9_416B_A512_A62AF6146819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyEdit window

// ********** .h
class CMyEdit : public CEdit
{
protected:
	CBitmap		m_Bmp;				// 에디트 박스에 쓸 비트맵


	// Generated message map functions
protected:
	//{{AFX_MSG(CMyEdit)
	afx_msg void OnPaint();
	afx_msg void OnChange();
	afx_msg void OnSetfocus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Construction
public:
	CMyEdit();
	virtual ~CMyEdit();

	void	SetBitMap(const int nBitMapID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__A6EDB6D3_E5F9_416B_A512_A62AF6146819__INCLUDED_)
