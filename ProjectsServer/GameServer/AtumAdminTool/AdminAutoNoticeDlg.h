#if !defined(AFX_ADMINAUTONOTICEDLG_H__5555F084_F197_4FEC_94D7_28CC84821726__INCLUDED_)
#define AFX_ADMINAUTONOTICEDLG_H__5555F084_F197_4FEC_94D7_28CC84821726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminAutoNoticeDlg.h : header file
//

#include "ReportCtrl.h"
#include "ODBCStatement.h"

/////////////////////////////////////////////////////////////////////////////
// CAdminAutoNoticeDlg dialog
class CAtumAdminToolDlg;
class CAdminAutoNoticeDlg : public CDialog
{
// Construction
public:
	CAdminAutoNoticeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdminAutoNoticeDlg)
	enum { IDD = IDD_DIALOG_ADMIN_AUTO_NOTICE };
	CReportCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminAutoNoticeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	BOOL DBQueryLoadAdminNoticeInfo(SANoticeInfo *o_pNoticeInfo);
	BOOL DBQueryUpdateAdminNoticeInfo(SANoticeInfo *i_pNoticeInfo);
	BOOL DBQueryLoadNoticeStringList(vectSANoticeString *o_pvectNoticeStrList);
	BOOL DBQueryUpdateNoticeStringList(vectSANoticeString *o_pvectNoticeStrList);

	BOOL _UpdateData(BOOL i_bSaveAndValidate=TRUE);

protected:

	CODBCStatement					m_odbcStmt1;
	CAtumAdminToolDlg *				m_pMainDlg;

	SANoticeInfo					m_ANoticeInfo1;
	vectSANoticeString				m_vectANoticeStrList;

	// Generated message map functions
	//{{AFX_MSG(CAdminAutoNoticeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnNoticeAdd();
	afx_msg void OnNoticeDelete();
	afx_msg void OnNoticeUp();
	afx_msg void OnNoticeDown();
	afx_msg void OnBtnGameServerReload();
	afx_msg void OnBtnUpdateDb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void InitListControl();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINAUTONOTICEDLG_H__5555F084_F197_4FEC_94D7_28CC84821726__INCLUDED_)
