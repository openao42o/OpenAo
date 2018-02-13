#if !defined(AFX_STATISTICPAGE1DLG_H__A68B9515_3348_486C_8840_892C41FB6367__INCLUDED_)
#define AFX_STATISTICPAGE1DLG_H__A68B9515_3348_486C_8840_892C41FB6367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage1Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage1Dlg dialog

class CStatisticPage1Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage1Dlg)

// Construction
public:
	void ViewSameAccount();
	void ViewNewAccountAge();
	void ViewUnRegAccount();
	void ViewDailyAccount();
	CStatisticPage1Dlg();
	~CStatisticPage1Dlg();
	CGridCtrl m_GridStaNewAccount;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage1Dlg)
	enum { IDD = IDD_STA_SHEET_1 };
	BOOL	m_valueCheckDailyAccount;
	COleDateTime	m_valueDTStartDate;
	COleDateTime	m_valueDTEndDate;
	BOOL	m_valueCheckUnregAccount;
	BOOL	m_valueCheckNewAccountAge;
	COleDateTime	m_valueDTNewAccountAge;
	COleDateTime	m_valueDTSameAccountStartDate;
	COleDateTime	m_valueDTSameAccountEndDate;
	BOOL	m_valueCheckSameAccount;
	COleDateTime	m_valueDTStartTime;
	COleDateTime	m_valueDTEndTime;
	COleDateTime	m_valueDTNewAccountAgeTime;
	COleDateTime	m_valueDTSameAccountStartTime;
	COleDateTime	m_valueDTSameAccountEndTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage1Dlg)
	afx_msg void OnButtonSt1Select();
	afx_msg void OnButtonSt1Save();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE1DLG_H__A68B9515_3348_486C_8840_892C41FB6367__INCLUDED_)
