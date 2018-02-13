#if !defined(AFX_STATISTICPAGE9DLG_H__BED1C0AD_6A50_4565_8E8E_8F6D0ECF5AFE__INCLUDED_)
#define AFX_STATISTICPAGE9DLG_H__BED1C0AD_6A50_4565_8E8E_8F6D0ECF5AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage9Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage9Dlg dialog

class CStatisticPage9Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage9Dlg)

// Construction
public:
	void ViewConnectionUser();
	void ViewConnectionUserEachDay(void);
	CStatisticPage9Dlg();
	~CStatisticPage9Dlg();
	CGridCtrl m_GridStaConnectUser;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage9Dlg)
	enum { IDD = IDD_STA_SHEET_9 };
	BOOL	m_valueCheckConnectionUser;
	COleDateTime	m_valueDTConnectUserStartDate;
	COleDateTime	m_valueDTConnectUserEndDate;
	COleDateTime	m_valueDTConnectUserStartTime;
	COleDateTime	m_valueDTConnectUserEndTime;
	COleDateTime	m_valueDTConnectUserStartDate2;
	COleDateTime	m_valueDTConnectUserEndDate2;
	COleDateTime	m_valueDTConnectUserStartTime2;
	COleDateTime	m_valueDTConnectUserEndTime2;
	BOOL	m_valueCheckEachDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage9Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage9Dlg)
	afx_msg void OnButtonSt9Select();
	afx_msg void OnButtonSt9Save();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE9DLG_H__BED1C0AD_6A50_4565_8E8E_8F6D0ECF5AFE__INCLUDED_)
