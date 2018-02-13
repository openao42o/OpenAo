#if !defined(AFX_EVENTMONSTERMANAGEMENTDLG_H__B07B4B48_C040_44D2_BA8D_F77F8ECFFD69__INCLUDED_)
#define AFX_EVENTMONSTERMANAGEMENTDLG_H__B07B4B48_C040_44D2_BA8D_F77F8ECFFD69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// eventmonstermanagementdlg.h : header file
//
#include "SCGridHelper.h"				// 2008-04-17 by cmkwon
#include "AtumDBHelper.h"				// 2008-04-17 by cmkwon

/////////////////////////////////////////////////////////////////////////////
// CEventMonsterManagementDlg dialog
class CAtumAdminToolDlg;			// 2008-04-17 by cmkwon
class CEventMonsterManagementDlg : public CDialog
{
// Construction
public:
	CEventMonsterManagementDlg(CWnd* pParent = NULL);   // standard constructor
	~CEventMonsterManagementDlg();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	CSCGridCtrl *GetEventMonsterGridPtr(void);
	void InitEventMonsterGrid(void);
	void InsertItemToEventMonsterGrid(SEVENT_MONSTER *i_pEvMonster);
	void LoadEventMonsterFromDB(void);
	BOOL FindEventMonster(SEVENT_MONSTER *o_pEvMon, INT i_evMonUID);
	INT GetSelectedEventMonsterUID(void);
	BOOL DBQueryInsertEventMonster(SEVENT_MONSTER *i_pEvMon);
	BOOL DBQueryUpdateEventMonster(SEVENT_MONSTER *i_pEvMon);
	BOOL DBQueryDeleteEventMonster(INT i_evMonUID);


// Dialog Data
	//{{AFX_DATA(CEventMonsterManagementDlg)
	enum { IDD = IDD_DLG_EVENT_MONSTER_MANAGEMENT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventMonsterManagementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	vector<MEX_MONSTER_INFO>		m_vecMexMonsterInfo;		// 2008-04-17 by cmkwon
protected:

	CSCGridCtrl						m_eventMonserListGrid;		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
	CAtumAdminToolDlg				*m_pMainDlg;				// 2008-04-17 by cmkwon
	CODBCStatement					*m_pODBCStmt;				// 2008-04-17 by cmkwon
	mtvectSEVENT_MONSTER			m_eventMonsterList;			// 2008-04-17 by cmkwon


	// Generated message map functions
	//{{AFX_MSG(CEventMonsterManagementDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnConmenuAddEventMonster();
	afx_msg void OnConmenuModifyEventMonster();
	afx_msg void OnConmenuDeleteEventMonster();
	afx_msg void OnBtnInsert();
	afx_msg void OnBtnReload();
	//}}AFX_MSG
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTMONSTERMANAGEMENTDLG_H__B07B4B48_C040_44D2_BA8D_F77F8ECFFD69__INCLUDED_)
