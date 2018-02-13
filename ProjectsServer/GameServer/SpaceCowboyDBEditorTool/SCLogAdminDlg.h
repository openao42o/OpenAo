#if !defined(AFX_SCLOGADMINDLG_H__C08F5D92_1858_447B_A654_B987928FA059__INCLUDED_)
#define AFX_SCLOGADMINDLG_H__C08F5D92_1858_447B_A654_B987928FA059__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SCLogAdminDlg.h : header file
//

#include "MyTabCtrl.h"
#include "ODBCStatement.h"

#define SIZE_MAX_LOG_ROW_COUNT 2000

class CSpaceCowboyDBEditorToolDlg;

/////////////////////////////////////////////////////////////////////////////
// CSCLogAdminDlg dialog

class CSCLogAdminDlg : public CDialog
{
// Construction
public:
	CSCLogAdminDlg(CSpaceCowboyDBEditorToolDlg *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSCLogAdminDlg)
	enum { IDD = IDD_DIALOG_LOG_ADMIN_TOOL };
	CMyTabCtrl	m_TabLogTool;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCLogAdminDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//CODBCStatement		m_ODBCStmt;
	CODBCStatement			*m_pODBCStmt;			// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	CSpaceCowboyDBEditorToolDlg	*m_pMainDlg;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

	ez_map<INT, ITEM>		*m_pmapItemInfo;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	ez_map<string, ITEM>	*m_pmapItemInfoName;	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -


protected:

	// Generated message map functions
	//{{AFX_MSG(CSCLogAdminDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCLOGADMINDLG_H__C08F5D92_1858_447B_A654_B987928FA059__INCLUDED_)
