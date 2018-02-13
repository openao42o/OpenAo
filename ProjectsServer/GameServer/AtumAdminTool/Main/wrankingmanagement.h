#if !defined(AFX_WRANKINGMANAGEMENT_H__B95C3AB7_C6D4_4A18_860D_7BCD1383D93C__INCLUDED_)
#define AFX_WRANKINGMANAGEMENT_H__B95C3AB7_C6D4_4A18_860D_7BCD1383D93C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// wrankingmanagement.h : header file
//
#include "ODBCStatement.h"

/////////////////////////////////////////////////////////////////////////////
// CWRankingManagement dialog
class CAtumAdminToolDlg;
class CWRankingManagement : public CDialog
{
// Construction
public:
	CWRankingManagement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWRankingManagement)
	enum { IDD = IDD_DLG_WRK_MANAGEMENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	CAtumAdminToolDlg *			m_pMainDlg;
	CODBCStatement				m_odbcStmtLocal;
	CODBCStatement				m_odbcStmtWRServer;
	BOOL DBQ_LoadSelfServiceInfo(SWRK_SERVICE_INFO *o_pServiceInfo);
	BOOL DBQ_UpdateSelfServiceInfo(SWRK_SERVICE_INFO *i_pServiceInfo);
	BOOL DBQ_DeleteSelfServiceInfo(SWRK_SERVICE_INFO *i_pServiceInfo);

	BOOL DBQ_LoadServerGroupInfo(SWRK_SERVER_GROUP *o_pServerGroupInfo);
	BOOL DBQ_UpdateServerGroupInfo(SWRK_SERVER_GROUP *i_pServerGroupInfo);

	// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 
	BOOL DBQ_DeleteServerGroupInfo(SWRK_SERVER_GROUP *i_pServerGroupInfo, INT i_nServiceUID);

	BOOL DBQ_UpdateAllServiceList(void);

	BOOL DBQ_ArrangeAllLocalRanking(void);

	BOOL DBQ_UploadLocalRankingToWorldRankingServer(void);
	BOOL DBQ_DownloadWorldRankingFromWorldRankingServer(void);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWRankingManagement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWRankingManagement)
	afx_msg void OnBtnServiceSetting();
	afx_msg void OnBtnUpdateRanking();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRANKINGMANAGEMENT_H__B95C3AB7_C6D4_4A18_860D_7BCD1383D93C__INCLUDED_)
