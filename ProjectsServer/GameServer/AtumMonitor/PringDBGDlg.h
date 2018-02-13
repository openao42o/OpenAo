#if !defined(AFX_PRINGDBGDLG_H__50957D82_B8E8_4AD3_906D_0C81F65A295D__INCLUDED_)
#define AFX_PRINGDBGDLG_H__50957D82_B8E8_4AD3_906D_0C81F65A295D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PringDBGDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPringDBGDlg dialog

class CPringDBGDlg : public CDialog
{
// Construction
public:
	CPringDBGDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPringDBGDlg)
	enum { IDD = IDD_DIALOG_DBG_PARAM };
	float	m_float1;
	float	m_float2;
	int		m_int1;
	int		m_int2;
	CString	m_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPringDBGDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPringDBGDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINGDBGDLG_H__50957D82_B8E8_4AD3_906D_0C81F65A295D__INCLUDED_)
