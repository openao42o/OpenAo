// SpaceCowboyDBEditorToolDlg.h : header file
//

#if !defined(AFX_SPACECOWBOYDBEDITORTOOLDLG_H__0A15E309_8B4D_4792_884A_E1D1F89C8FCE__INCLUDED_)
#define AFX_SPACECOWBOYDBEDITORTOOLDLG_H__0A15E309_8B4D_4792_884A_E1D1F89C8FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolDlg dialog
class CSCLogAdminDlg;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
class CSpaceCowboyDBEditorToolDlg : public CDialog
{
// Construction
public:
	BOOL CheckPasswordAndRace(BOOL *o_pbEnableEdit, CODBCStatement *i_pODBCStatement, char *i_szAccName, char *i_szPassword);
	CSpaceCowboyDBEditorToolDlg(CWnd* pParent = NULL);	// standard constructor
	~CSpaceCowboyDBEditorToolDlg();
	void EnableToolControls(BOOL i_bEnable);

	CMonsterDlg*					m_pMonsterDlg;
	CMonsterDropDlg*				m_pMonsterDropDlg;
	CMonsterAttackDlg*				m_pMonsterAttackDlg;
	CQuestEditorDlg*				m_pQuestEditorDlg;
	CMapEditorDlg*					m_pMapEditorDlg;
	CStatisticSheetDlg*				m_pStatisticDlg;
	CWeaponItem*					m_pWeaponItemDlg;
	CEngineItemDlg*					m_pEngineItemDlg;
	GAME_SERVER_INFO_FOR_ADMIN		*m_pServerInfo4Admin;
	CString							m_szServerName;
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CWriteLog						*m_pWriteLog;

	CSCLogAdminDlg					*m_pLogAdminDlg;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

	ez_map<INT, ITEM>				m_mapItemInfo;				// 2006-01-19 by cmkwon
	ez_map<string, ITEM>			m_mapItemInfoName;			// 2006-01-19 by cmkwon
	ez_map<INT, RARE_ITEM_INFO>		m_mapRareItemInfo;			// 2006-01-19 by cmkwon
	ez_map<string, RARE_ITEM_INFO>	m_mapRareItemInfoName;		// 2006-01-19 by cmkwon
	vector<MEX_MONSTER_INFO>		m_vecMexMonsterInfo;		// 2006-02-03 by cmkwon

	void ResetItemInfoANDRareInfo(void);
	MEX_MONSTER_INFO *GetMEXMonsterInfo(int i_monsterUnitKind);


	BOOL							m_bEnableEdit;				// 2006-05-08 by cmkwon
	BOOL GetEnbaleEdit(void){		return m_bEnableEdit;};

// Dialog Data
	//{{AFX_DATA(CSpaceCowboyDBEditorToolDlg)
	enum { IDD = IDD_SPACECOWBOYDBEDITORTOOL_DIALOG };
	CComboBox	m_ctrlComboServer;
	CString	m_valueEditID;
	CString	m_valueEditPW;
	CString	m_ctl_strLanguageString;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpaceCowboyDBEditorToolDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpaceCowboyDBEditorToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonMonster();
	afx_msg void OnButtonMonsterattack();
	afx_msg void OnButtonMonsterdrop();
	afx_msg void OnButtonQuesteditor();
	afx_msg void OnButtonWeapon();
	afx_msg void OnButtonEngine();
	afx_msg void OnButtonMapeditor();
	afx_msg void OnButtonStatistic();
	afx_msg void OnDestroy();
	afx_msg void OnBtnLogManagement();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPACECOWBOYDBEDITORTOOLDLG_H__0A15E309_8B4D_4792_884A_E1D1F89C8FCE__INCLUDED_)
