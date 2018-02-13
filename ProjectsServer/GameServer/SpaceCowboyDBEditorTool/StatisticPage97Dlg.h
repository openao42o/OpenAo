#if !defined(AFX_STATISTICPAGE97DLG_H__FA9102D6_4F1C_41DE_8FC5_9E8308111778__INCLUDED_)
#define AFX_STATISTICPAGE97DLG_H__FA9102D6_4F1C_41DE_8FC5_9E8308111778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage97Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage97Dlg dialog

class CStatisticPage97Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage97Dlg)

// Construction
public:
	CStatisticPage97Dlg();
	~CStatisticPage97Dlg();

	void ViewBossMonsterDead(BOOL i_bOnlyInfluenceMonster=FALSE);

	CGridCtrl						m_gridView97;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg		*m_pMainDlg;


// Dialog Data
	//{{AFX_DATA(CStatisticPage97Dlg)
	enum { IDD = IDD_STA_SHEET_97 };
	COleDateTime	m_valueStartDate;
	COleDateTime	m_valueEndDate;
	COleDateTime	m_valueStartTime;
	COleDateTime	m_valueEndTime;
	BOOL	m_ctl_bOnlyInflMonster;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage97Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage97Dlg)
	afx_msg void OnBtnSearch();
	afx_msg void OnBtnSaveAsFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE97DLG_H__FA9102D6_4F1C_41DE_8FC5_9E8308111778__INCLUDED_)
