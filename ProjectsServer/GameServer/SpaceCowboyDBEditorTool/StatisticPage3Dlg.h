#if !defined(AFX_STATISTICPAGE3DLG_H__D991F0DE_B199_4923_AF89_3B2B48D98EFE__INCLUDED_)
#define AFX_STATISTICPAGE3DLG_H__D991F0DE_B199_4923_AF89_3B2B48D98EFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage3Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage3Dlg dialog

class CStatisticPage3Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage3Dlg)

// Construction
public:
	void ViewByDayHourConnection();
	void ViewByDayConnection();
	void ViewByHourConnection();
	void ViewDailyConnection();
	CStatisticPage3Dlg();
	~CStatisticPage3Dlg();
	CGridCtrl m_GridStaConnection;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage3Dlg)	
	enum { IDD = IDD_STA_SHEET_3 };
	BOOL	m_valueCheckDailyConnection;
	COleDateTime	m_valueDTDailyConnectionStartDate;
	COleDateTime	m_valueDTDailyConnectionEndDate;
	BOOL	m_valueCheckByHourConnection;
	COleDateTime	m_valueDTByHourConnectionStartDate;
	COleDateTime	m_valueDTByHourConnectionEndDate;
	BOOL	m_valueCheckByDayConnection;
	COleDateTime	m_valueDTByDayConnectionStartDate;
	COleDateTime	m_valueDTByDayConnectionEndDate;
	BOOL	m_valueCheckByDayHourConnection;
	COleDateTime	m_valueDTByDayHourConnectionStartDate;
	COleDateTime	m_valueDTByDayHourConnectionEndDate;
	BOOL	m_valueCheckDailyConnectionSort;
	COleDateTime	m_valueDTByDayConnectionStartTime;
	COleDateTime	m_valueDTByDayConnectionEndTime;
	COleDateTime	m_valueDTDailyConnectionStartTime;
	COleDateTime	m_valueDTDailyConnectionEndTime;
	COleDateTime	m_valueDTByHourConnectionStartTime;
	COleDateTime	m_valueDTByHourConnectionEndTime;
	COleDateTime	m_valueDTByDayHourConnectionStartTime;
	COleDateTime	m_valueDTByDayHourConnectionEndTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage3Dlg)
	afx_msg void OnButtonSt3Select();
	afx_msg void OnCheckSt3DailyConnection();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnButtonSt3Save();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE3DLG_H__D991F0DE_B199_4923_AF89_3B2B48D98EFE__INCLUDED_)
