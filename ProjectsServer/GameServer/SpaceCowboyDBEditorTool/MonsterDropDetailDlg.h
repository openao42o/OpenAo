#if !defined(AFX_MONSTERDROPDETAILDLG_H__DF257038_6D7C_467D_A544_72D0FFC1971E__INCLUDED_)
#define AFX_MONSTERDROPDETAILDLG_H__DF257038_6D7C_467D_A544_72D0FFC1971E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterDropDetailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterDropDetailDlg dialog
class CMonsterDropDlg;

class CMonsterDropDetailDlg : public CDialog
{
// Construction
public:
	void ReflectionToGrid(int type); //부모의 Update Grid Ctrl에 값을 1 row 넣어준다.
	bool ValidateCheck(); // 제약사항 체크
	CMonsterDropDetailDlg(CWnd* pParent = NULL);   // standard constructor
	CMonsterDropDetailDlg::~CMonsterDropDetailDlg();

// Dialog Data
	//{{AFX_DATA(CMonsterDropDetailDlg)
	enum { IDD = IDD_DIALOG_MONSTERDROPDETAIL };
	CButton	m_ctrlButtonSave;
	CButton	m_ctrlButtonMake;
	CButton	m_ctrlButtonCancel;
	CString	m_valueEditMonsterUniqueNumber;
	CString	m_valueEditItemNumber;
	CString	m_valueEditMinCount;
	CString	m_valueEditMaxCount;
	CString	m_valueEditProbability;
	CString	m_valueEditPrefixProbability;
	CString	m_valueEditSuffixProbability;
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스
	CMonsterDropDlg					*m_pMonsterDropDlg; // 부모 다이얼로그 클래스 포인터
	int m_nSelectRow;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterDropDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterDropDetailDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonMake();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERDROPDETAILDLG_H__DF257038_6D7C_467D_A544_72D0FFC1971E__INCLUDED_)
