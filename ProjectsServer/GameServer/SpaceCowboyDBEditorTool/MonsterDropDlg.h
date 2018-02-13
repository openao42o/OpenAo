#if !defined(AFX_MONSTERDROPDLG_H__6EA0C318_5AEF_43D2_8B9F_65C05D83DCBE__INCLUDED_)
#define AFX_MONSTERDROPDLG_H__6EA0C318_5AEF_43D2_8B9F_65C05D83DCBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterDropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterDropDlg dialog

class CMonsterDropDlg : public CDialog
{
// Construction
public:
	void ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row);
	void SetDetailDialog(int row);
	void InsertItemToGridCurrent(_TI_MONSTERITEM *i_pTi_MonsterItem, int i_nRow);
	void GetCurrentList(int type, int uniquenumber=0);
	void InitGridUpdate();
	void InitGridCurrent();
	CMonsterDropDlg(CWnd* pParent = NULL);   // standard constructor
	CMonsterDropDlg::~CMonsterDropDlg();

// Dialog Data
	//{{AFX_DATA(CMonsterDropDlg)
	enum { IDD = IDD_DIALOG_MONSTERDROP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스
	CMonsterDropDetailDlg				*m_pMonsterDropDetailDlg;
	CGridCtrl		m_GridCurrent;
	CGridCtrl		m_GridUpdate;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterDropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterDropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEnd();
	afx_msg void OnButtonView();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonServerupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERDROPDLG_H__6EA0C318_5AEF_43D2_8B9F_65C05D83DCBE__INCLUDED_)
