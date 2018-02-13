#if !defined(AFX_MONSTERATTACKDETAILDLG_H__89B9913E_8558_483E_9EFB_49297E5647B7__INCLUDED_)
#define AFX_MONSTERATTACKDETAILDLG_H__89B9913E_8558_483E_9EFB_49297E5647B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterAttackDetailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttackDetailDlg dialog
class CMonsterAttackDlg;

class CMonsterAttackDetailDlg : public CDialog
{
// Construction
public:
	void ReflectionToGrid(int type); //부모 다이얼로그의 Update GridCtrl에 UPDATE, INSERT 된 값을 넣는다.
	bool ValidateCheck(); //데이터의 유효성을 검사
	CMonsterAttackDetailDlg(CWnd* pParent = NULL);   // standard constructor
	CMonsterAttackDetailDlg::~CMonsterAttackDetailDlg();
	

// Dialog Data
	//{{AFX_DATA(CMonsterAttackDetailDlg)
	enum { IDD = IDD_DIALOG_MONSTERATTACKDETAIL };
	CComboBox	m_ctrlComboKind;
	CComboBox	m_ctrlComboOrbitType;
	CComboBox	m_ctrlComboRace;
	CButton	m_ctrlButtonSave;
	CButton	m_ctrlButtonMake;
	CButton	m_ctrlButtonCancel;
	CString	m_valueEditAbilityMin;
	CString	m_valueEditAbilityMax;
	CString	m_valueEditReqMinLevel;
	CString	m_valueEditHitRate;
	CString	m_valueEditSpeedPenalty;
	CString	m_valueEditRange;
	CString	m_valueEditCharging;
	CString	m_valueEditRangeAngle;
	CString	m_valueEditShotNum;
	CString	m_valueEditMultiNum;
	CString	m_valueEditAttackTime;
	CString	m_valueEditReqSP;
	CString	m_valueEditItemName;
	CString	m_valueEditSummonMonster;
	CString	m_valueEditItemNum;
	CString	m_valueEditCameraPattern;
	CString	m_valueEditSourceIndex;
	CString	m_valueEditDestParameter1;
	CString	m_valueEditDestParameter2;
	CString	m_valueEditDestParameter3;
	CString	m_valueEditDestParameter4;
	CString	m_valueEditParameterValue1;
	CString	m_valueEditParameterValue2;
	CString	m_valueEditParameterValue3;
	CString	m_valueEditParameterValue4;
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //메인 다이얼로그 클래스
	CMonsterAttackDlg					*m_pMonsterAttackDlg; //부모 다이얼로그
	int m_nSelectRow; //부모 다이얼로그의 Current GridCtrl에 선택된 Row


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterAttackDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterAttackDetailDlg)
	afx_msg void OnButtonMake();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERATTACKDETAILDLG_H__89B9913E_8558_483E_9EFB_49297E5647B7__INCLUDED_)
