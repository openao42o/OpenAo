#if !defined(AFX_STATISTICPAGE2DLG_H__81CEA9AB_8E34_4885_9B59_B61AE3593FB4__INCLUDED_)
#define AFX_STATISTICPAGE2DLG_H__81CEA9AB_8E34_4885_9B59_B61AE3593FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage2Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage2Dlg dialog

class CStatisticPage2Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage2Dlg)

// Construction
public:
	void ViewExpectAccount();
	void ViewUnregAccount();
	void ViewCurrentAccount();
	void ViewTotalAccumAccount();

	void ResetGridStaAccumAccount(void);
	void InsertGridData(char * i_szRowName, int i_nTotalCount, int i_nNormalCount=0, int i_nSpecialCount=0);

	CStatisticPage2Dlg();
	~CStatisticPage2Dlg();

	CGridCtrl m_GridStaAccumAccount;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage2Dlg)
	enum { IDD = IDD_STA_SHEET_2 };
	BOOL	m_valueCheckTotalAccumAccount;
	BOOL	m_valueCheckCurrentAccount;
	BOOL	m_valueCheckUnregAccount;
	BOOL	m_valueCheckExpectAccount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage2Dlg)
	afx_msg void OnButtonSt2Select();
	afx_msg void OnButtonSt2Save();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE2DLG_H__81CEA9AB_8E34_4885_9B59_B61AE3593FB4__INCLUDED_)
