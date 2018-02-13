#if !defined(AFX_STATISTICPAGE7DLG_H__3F1EE65C_7622_4E95_864F_29836BBC6A5E__INCLUDED_)
#define AFX_STATISTICPAGE7DLG_H__3F1EE65C_7622_4E95_864F_29836BBC6A5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage7Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage7Dlg dialog

class CStatisticPage7Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage7Dlg)

// Construction
public:
	void ViewCharacterPKCnt();
	void ViewPKLoseLog();
	void ViewPKWinLog();
	void ViewDailyPK();
	CStatisticPage7Dlg();
	~CStatisticPage7Dlg();
	CGridCtrl m_GridStaWar;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage7Dlg)
	enum { IDD = IDD_STA_SHEET_7 };
	BOOL	m_valueCheckDailyPK;
	COleDateTime	m_valueDTDailyPKStartDate;
	COleDateTime	m_valueDTDailyPKEndDate;
	BOOL	m_valueCheckPKWinLog;
	COleDateTime	m_valueDTPKWinLogStartDate;
	COleDateTime	m_valueDTPKWinLogEndDate;
	BOOL	m_valueCheckPKLoseLog;
	COleDateTime	m_valueDTPKLoseLogStartDate;
	COleDateTime	m_valueDTPKLoseLogEndDate;
	BOOL	m_valueCheckCharacterPKCnt;
	COleDateTime	m_valueRadioCharacterWarCntStartDate;
	COleDateTime	m_valueRadioCharacterWarCntEndDate;
	COleDateTime	m_valueDTDailyPKStartTime;
	COleDateTime	m_valueDTDailyPKEndTime;
	COleDateTime	m_valueDTPKWinLogStartTime;
	COleDateTime	m_valueDTPKWinLogEndTime;
	COleDateTime	m_valueDTPKLoseLogStartTime;
	CTime	m_valueDTPKLoseLogEndTime;
	COleDateTime	m_valueRadioCharacterWarCntStartTime;
	COleDateTime	m_valueRadioCharacterWarCntEndTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage7Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage7Dlg)
	afx_msg void OnButtonSt7Select();
	afx_msg void OnButtonSt7Save();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE7DLG_H__3F1EE65C_7622_4E95_864F_29836BBC6A5E__INCLUDED_)
