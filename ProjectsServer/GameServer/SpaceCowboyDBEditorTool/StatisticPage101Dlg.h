#if !defined(AFX_STATISTICPAGE101DLG_H__79EE4B32_DA80_4419_A659_D6241485218D__INCLUDED_)
#define AFX_STATISTICPAGE101DLG_H__79EE4B32_DA80_4419_A659_D6241485218D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticPage101Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatisticPage101Dlg dialog
struct m_PCBangTotal
{
	UID32_t	m_PCBangUID;
	CString	m_PCBangName;
	INT		m_TotalPCBangPlayTime;
	CString m_PCBangTel;
	CString m_PCBangZipCode;
	CString m_PCBangAddr_Sido;
	CString m_PCBangAddr_SiGuGun;
	CString m_PCBangAddr_Dong;
	CString m_PCBangAddr_Detail;
	BYTE	m_PCBangLevel;			// 2007-06-25 by dhjin, PC방 유저 레벨
};


struct m_PCBangTotalByDaily
{
	CString m_PCBangDate;
	UID32_t m_PCBangUID;
	INT		m_PCBangPlayTime;
	INT		m_Count;
};


struct  CDAILYDATE{
int		Year;
int		Month;
int		Day;
};

class CStatisticPage101Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CStatisticPage101Dlg)

// Construction
public:
	BOOL GetPCBangPlayData();			// 2007-01-23 by dhjin, PC방 정보 DB에서 가져와서 m_lPCBangTotal에 저장
	BOOL GetPCBangPlayTimeDailyData();	// 2007-01-23 by dhjin, 날자별 PC방 플레이 시간 m_lPCBangTotalByDaily저장
	void DisPlayPCBangTotalPlayTime();	// 2007-01-23 by dhjin, PC방 정보 화면에 뿌려주기
	void DisPlayPCBangTotalPlayTimeDaily();	 // 2007-01-23 by dhjin, PC방 Daily 화면에 뿌려주기
	CStatisticPage101Dlg();
	~CStatisticPage101Dlg();
	CGridCtrl m_GridStaPCBang;
	CGridCtrl m_GridStaPCBangDaily;
	CODBCStatement					*m_pODBCStmt2; // DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 

	vector<m_PCBangTotal> m_lPCBangTotal;
	list<m_PCBangTotalByDaily> m_lPCBangTotalByDaily;
	
// Dialog Data
	//{{AFX_DATA(CStatisticPage101Dlg)
	enum { IDD = IDD_STA_SHEET_101 };
	COleDateTime	m_ctl_CreatedTimeStart;
	COleDateTime	m_ctl_CreatedDateEnd;
	COleDateTime	m_ctl_CreatedTimeEnd;
	int		m_edit_MaxRankCount;
	COleDateTime	m_ctl_CreatedDateStart;
	BOOL	m_ctl_bIncrease;
	BOOL    m_ctl_bDecrease;
	BYTE	m_edit_PCBangLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStatisticPage101Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStatisticPage101Dlg)
	afx_msg void OnButtonSt101Select();
	afx_msg void OnButtonSt101Save();
	afx_msg void OnIncreaseSort();
	afx_msg void OnDecreaseSort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICPAGE101DLG_H__79EE4B32_DA80_4419_A659_D6241485218D__INCLUDED_)
