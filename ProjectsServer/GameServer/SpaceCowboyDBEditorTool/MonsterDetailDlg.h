#if !defined(AFX_MONSTERDETAILDLG_H__B399F22D_0651_4E78_8548_227E33BD7EAB__INCLUDED_)
#define AFX_MONSTERDETAILDLG_H__B399F22D_0651_4E78_8548_227E33BD7EAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterDetailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterDetailDlg dialog
#include "resource.h"

class CMonsterAttackDetailDlg;
class CSpaceCowboyDBEditorToolDlg;
class CMonsterDlg;

class CMonsterDetailDlg : public CDialog
{
// Construction
public:
	bool ValidateCheck(); //제약사항 체크
	void ReflectionToGrid(int type); //부모 다이얼로그의 Update GridCtrl에 값을 추가한다.
	CMonsterDetailDlg(CWnd* pParent = NULL);   // standard constructor
	CMonsterDetailDlg::~CMonsterDetailDlg();
	void SetDetailMonsterItem(_TI_ITEM *i_pTi_Item);

// Dialog Data
	//{{AFX_DATA(CMonsterDetailDlg)
	enum { IDD = IDD_DIALOG_MONSTERDETAIL };
	CEdit	m_ctrlEditUniqueNumber;
	CComboBox	m_ctrlComboAttackObject;
	CComboBox	m_ctrlComboAttackPattern;
	CComboBox	m_ctrlComboBelligerence;
	CComboBox	m_ctrlComboMovePattern;
	CComboBox	m_ctrlComboMonsterForm;
	CComboBox	m_ctrlComboMonsterItem05;
	CComboBox	m_ctrlComboMonsterItem04;
	CComboBox	m_ctrlComboMonsterItem03;
	CComboBox	m_ctrlComboMonsterItem02;
	CComboBox	m_ctrlComboMonsterItem01;
	CComboBox	m_ctrlComboRace;
	CButton	m_ctrlButtonCancel;
	CButton	m_ctrlButtonSave;
	CButton	m_ctrlButtonMake;
	CString	m_valueEditLevel;
	CString	m_valueEditUniqueNumber;
	CString	m_valueEditMonsterName;
	CString	m_valueEditExperience;
	CString	m_valueEditHP;
	CString	m_valueEditSpeed;
	CString	m_valueEditSizeForServer;
	CString	m_valueEditSizeForClient;
	CString	m_valueEditDefense;
	CString	m_valueEditDefenseProbability;
	CString	m_valueEditAlphaBlending;
	CString	m_valueEditAttackRange;
	CString	m_valueEditHPRecoveryTime;
	CString	m_valueEditHPRecoveryValue;
	CString	m_valueEditScaleValue;
	CString	m_valueEditTextureIndex;
	CString	m_valueEditSourceIndex;
	CString	m_valueEditQuickTurnAngle;
	CString	m_valueEditQuickTurnSpeed;
	CString	m_valueEditQuickTurnTerm;
	CString	m_valueEditMPOptionParam1;
	CString	m_valueEditMPOptionParam2;
	CString	m_valueEditTurnAngle;
	CString	m_valueEditRange;
	BOOL	m_valueCheckMoveParty;
	BOOL	m_valueCheckPatternMonster;
	BOOL	m_valueCheckBossMonster;
	BOOL	m_valueCheckDeadFall;
	BOOL	m_valueCheckAutoDestroy;
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스
	CMonsterDlg					*m_pMonsterDlg; //부모 다이얼로그 포인터
	CMonsterAttackDetailDlg		*m_pMonsterAttackDetailDlg;
	int	m_nSelectRow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterDetailDlg)
	afx_msg void OnButtonMake();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonAttackitem();
	afx_msg void OnButtonDropitem();
	afx_msg void OnButtonMonsteritem01();
	afx_msg void OnButtonMonsteritem2();
	afx_msg void OnButtonMonsteritem3();
	afx_msg void OnButtonMonsteritem4();
	afx_msg void OnButtonMonsteritem5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONSTERDETAILDLG_H__B399F22D_0651_4E78_8548_227E33BD7EAB__INCLUDED_)
