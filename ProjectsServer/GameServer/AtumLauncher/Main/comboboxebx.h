#if !defined(AFX_COMBOBOXEBX_H__69F5FDE1_D321_4376_B975_8B596B92037E__INCLUDED_)
#define AFX_COMBOBOXEBX_H__69F5FDE1_D321_4376_B975_8B596B92037E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// comboboxebx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEBX window

#define DEF_COL 17000000

class CComboBoxEBX : public CComboBox
{
protected:
	struct ComboItem
	{
		COLORREF	cBg,	cFg;
		int			nHal;
		int			nIndex;
		CString		strText;
		int			nCrowdedness;
	};

	ComboItem*	Item(int ix);
// Construction
public:
	CComboBoxEBX();

	int Ix(int ix);
	void ResetContent();
	BOOL RemoveAt(int ix);
	void DrawText(CDC *pDC, CString &str, CPoint pt, int TextSize, DWORD TextColor);
	void UpdateNetworkState(int i_nNetworkState){};		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 콤보박스에는 필요 없음

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboBoxEBX)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	int InsertItem(int nIndex, LPCTSTR lpszItem, int i_nCrowdedness, COLORREF bg, COLORREF fg, int hal);
	int GetCrowdedness(int nIndex);
	void GetTextString(int nIndex, CString& rString);
	virtual ~CComboBoxEBX();

	// Generated message map functions
protected:
	CComboBox*	m_pComboList;
	HBITMAP		m_hBitMap;

	int			m_nWidth;
	int			m_nHeight;
	//{{AFX_MSG(CComboBoxEBX)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBOXEBX_H__69F5FDE1_D321_4376_B975_8B596B92037E__INCLUDED_)
