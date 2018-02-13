// SCBitFlagDlg.h : header file
//

#if !defined(AFX_SCBitFlagDLG_H__22C92D2C_2A6C_4C63_ADFF_76F0324FDA03__INCLUDED_)
#define AFX_SCBitFlagDLG_H__22C92D2C_2A6C_4C63_ADFF_76F0324FDA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CSCBitFlagDlg dialog
class CMPOptionDlg;
class CSCBitFlagDlg : public CDialog
{
// Construction
public:
	CSCBitFlagDlg(CWnd* pParent = NULL);	// standard constructor




public:
	CMPOptionDlg			*m_pMPOptionDlg;



// Dialog Data
	//{{AFX_DATA(CSCBitFlagDlg)
	enum { IDD = IDD_SCBitFlag_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCBitFlagDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSCBitFlagDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnMpoption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCBitFlagDLG_H__22C92D2C_2A6C_4C63_ADFF_76F0324FDA03__INCLUDED_)
