#if !defined(AFX_MPOPTIONDLG_H__2C154E94_B30B_4FDB_A3C4_CBB7E714D382__INCLUDED_)
#define AFX_MPOPTIONDLG_H__2C154E94_B30B_4FDB_A3C4_CBB7E714D382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPOptionDlg.h : header file
//

#include "SCBitFlagHeader.h"

typedef struct
{
	char	szMPOptionComment[2][SIZE_MAX_BIT_FALG_COMMENTS];
} MPOptionComment;

/////////////////////////////////////////////////////////////////////////////
// CMPOptionDlg dialog

class CMPOptionDlg : public CDialog
{
// Construction
public:
	CMPOptionDlg(CWnd* pParent = NULL);   // standard constructor

public:
	MPOptionComment			m_ArrMPOptionComment[MPOPTION_BIT_COUNTS];


// Dialog Data
	//{{AFX_DATA(CMPOptionDlg)
	enum { IDD = IDD_DLG_MPOPTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMPOptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBtnClicked(void);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPOPTIONDLG_H__2C154E94_B30B_4FDB_A3C4_CBB7E714D382__INCLUDED_)
