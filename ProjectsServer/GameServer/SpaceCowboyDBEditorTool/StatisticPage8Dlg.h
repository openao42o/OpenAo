#if !defined(AFX_STATISTICPAGE8DLG_H__B7E0652A_2D07_4E76_9366_2ECB4D3D6249__INCLUDED_)
#define AFX_STATISTICPAGE8DLG_H__B7E0652A_2D07_4E76_9366_2ECB4D3D6249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage8Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage8Dlg dialog

class CStatisticPage8Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage8Dlg)

// Construction
public:
	void ViewDailyMission(void);
	void ViewSpecCompleteMission(int i_nQuestIndex);
	void ViewSpecProgressMission(int i_nQuestIndex);

	CStatisticPage8Dlg();
	~CStatisticPage8Dlg();
	CGridCtrl m_GridStaMission;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

// Dialog Data
	//{{AFX_DATA(CStatisticPage8Dlg)
	enum { IDD = IDD_STA_SHEET_8 };
	BOOL	m_valueCheckDailyMission;
	BOOL	m_valueCheckSpecMission;
	COleDateTime	m_valueDTByDayMissionStartDate;
	COleDateTime	m_valueDTByDayMissionEndDate;
	COleDateTime	m_valueDTByDayMissionStartTime;
	COleDateTime	m_valueDTByDayMissionEndTime;
	COleDateTime	m_valueDTBySpecMissionStartDate;
	COleDateTime	m_valueDTBySpecMissionEndDate;
	COleDateTime	m_valueDTBySpecMissionStartTime;
	COleDateTime	m_valueDTBySpecMissionEndTime;
	int		m_ctlnMissionNumber;
	int		m_ctlnResultCount;
	BOOL	m_ctlbProgressMission;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage8Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage8Dlg)
	afx_msg void OnButtonSt8Select();
	afx_msg void OnButtonSt8Save();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE8DLG_H__B7E0652A_2D07_4E76_9366_2ECB4D3D6249__INCLUDED_)
