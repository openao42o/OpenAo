#if !defined(AFX_SCOUTPOSTDLG_H__AE656F51_A957_481F_8A11_6363A0573E1F__INCLUDED_)
#define AFX_SCOUTPOSTDLG_H__AE656F51_A957_481F_8A11_6363A0573E1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCOutPostDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSCOutPostDlg dialog
#include "GridCtrl/GridCtrl.h" //For GridCtrl
#include "ODBCStatement.h" //For DB Access
#include "AtumDateTime.h"

class CAtumAdminToolDlg;

class CSCOutPostDlg : public CDialog
{
// Construction
public:
	CSCOutPostDlg(CWnd* pParent = NULL);   // standard constructor

	CGridCtrl					m_GridOutPostInfo;
	CODBCStatement				*m_pODBCStmt; //DB 접근 클래스 포인터
	CAtumAdminToolDlg			*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스

	vectSOutPostInfo			m_SOutPostInfo;

	void InitGrid();
	void DBLoadOutPostInfo();
	void InsertOutPostInfo();

// Dialog Data
	//{{AFX_DATA(CSCOutPostDlg)
	enum { IDD = IDD_DLG_OUTPOST };
	int		m_nCityMap;
	int		m_nGuildUID;
	BYTE	m_byInfluence;
	BOOL	m_bNextWarTimeSet;
	int		m_nOutPostMap;
	COleDateTime	m_GetDate;
	COleDateTime	m_GetTime;
	COleDateTime	m_NextWarDate;
	COleDateTime	m_NextWarTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCOutPostDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSCOutPostDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOUTPOSTDLG_H__AE656F51_A957_481F_8A11_6363A0573E1F__INCLUDED_)
