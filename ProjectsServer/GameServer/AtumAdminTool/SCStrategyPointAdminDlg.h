#if !defined(AFX_SCSTRATEGYPOINTADMINDLG_H__888BD74C_2541_496D_8B0B_76AC5E3B2D30__INCLUDED_)
#define AFX_SCSTRATEGYPOINTADMINDLG_H__888BD74C_2541_496D_8B0B_76AC5E3B2D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCStrategyPointAdminDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SCStrategyPointAdminDlg dialog
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h" //For DB Access
#include "AtumDateTime.h"

class CAtumAdminToolDlg;

class CSCStrategyPointAdminDlg : public CDialog
{
// Construction
public:
	CSCStrategyPointAdminDlg(CWnd* pParent = NULL);   // standard constructor

	CGridCtrl					m_GridStrategyPointInfo;
	CGridCtrl					m_GridStrategyPointRange;

	CODBCStatement				*m_pODBCStmt; //DB 접근 클래스 포인터
	CAtumAdminToolDlg			*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스

	vectSSTRATEGYPOINT_SUMMONTIME_INFO	m_vectSStrategyPointSummonTimeInfo;
	SSTRATEGYPOINT_SUMMON_RANGE			m_SStrategyPointSummonRange;
	vectSSTRATEGYPOINT_NOT_SUMMON_TIME	m_vectSStrategyPointNotSummonTime;

	void	InitGrid();
	void	InitGridStrategyPointSummonTimeInfo();
	void	InitGridStrategyPointSummonRange();
	void	InsertGridStrategyPointSummonTimeInfo();
	void	InsertGridStrategyPointSummonRange();
	void	InsertStrategyPointNotSummonTime();
	void	DBLoadStrategyPointSummonTimeInfo();
	void	DBLoadStrategyPointSummonRangeInfo();
	void    DBUpdateStrategyPointSummonRangeInfo();
	void	DBLoadStrategyPointNotSummonTime();
	void	DBUpdateStrategyPointNotSummonTime();
	void	DBInitStrategyPointSummonRange();

// Dialog Data
	//{{AFX_DATA(SCStrategyPointAdminDlg)
	enum { IDD = IDD_DIALOG_STRATEGYPOINT_ADMIN_TOOL };
//	int		m_MaxSummonCountDaily;
	int		m_StrategyPointSummonCheck;
	int		m_SummonRange;
	COleDateTime	m_eFriday;
	COleDateTime	m_eMonday;
	COleDateTime	m_eSaturday;
	COleDateTime	m_eSunday;
	COleDateTime	m_eThursday;
	COleDateTime	m_eTuesday;
	COleDateTime	m_eWednesday;
	COleDateTime	m_sFriday;
	COleDateTime	m_sMonday;
	COleDateTime	m_sSaturday;
	COleDateTime	m_sSunday;
	COleDateTime	m_sThursday;
	COleDateTime	m_sTuesday;
	COleDateTime	m_sWednesday;
	BOOL	m_bSummonCheckFriday;
	BOOL	m_bSummonCheckMonday;
	BOOL	m_bSummonCheckSaturday;
	BOOL	m_bSummonCheckSunday;
	BOOL	m_bSummonCheckThursday;
	BOOL	m_bSummonCheckTuesday;
	BOOL	m_bSummonCheckWednesday;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SCStrategyPointAdminDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SCStrategyPointAdminDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStrategypointSummonRange();
	afx_msg void OnButtonUpdateStrategypointNotsummontime();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonStrategypointSummonRangeInit();
	afx_msg void OnButtonRefresh();
	afx_msg void OnSummonCheckSunday();
	afx_msg void OnSummonCheckFriday();
	afx_msg void OnSummonCheckMonday();
	afx_msg void OnSummonCheckSaturday();
	afx_msg void OnSummonCheckThursday();
	afx_msg void OnSummonCheckTuesday();
	afx_msg void OnSummonCheckWednesday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCSTRATEGYPOINTADMINDLG_H__888BD74C_2541_496D_8B0B_76AC5E3B2D30__INCLUDED_)
