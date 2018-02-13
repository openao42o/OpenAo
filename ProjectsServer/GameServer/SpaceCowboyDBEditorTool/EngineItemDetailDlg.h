#if !defined(AFX_ENGINEITEMDETAILDLG_H__B0EDBB63_A3A6_4DE0_BCEC_01DEAB86D3BB__INCLUDED_)
#define AFX_ENGINEITEMDETAILDLG_H__B0EDBB63_A3A6_4DE0_BCEC_01DEAB86D3BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EngineItemDetailDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEngineItemDetailDlg dialog
class CEngineItemDlg;

class CEngineItemDetailDlg : public CDialog
{
// Construction
public:
	CEngineItemDetailDlg(CWnd* pParent = NULL);   // standard constructor
	CEngineItemDetailDlg::~CEngineItemDetailDlg();

	BOOL ValidateCheck();
	void ReflectionToGrid(int type);
// Dialog Data
	//{{AFX_DATA(CEngineItemDetailDlg)
	enum { IDD = IDD_DIALOG_ENGINE_DETAIL };
	CComboBox	m_ctrlComboReqRace2;
	CComboBox	m_ctrlComboReqRace;
	CComboBox	m_ctrlComboPosition;
	CComboBox	m_ctrlComboKind;
	CButton	m_ctrlButtonCancel;
	CButton	m_ctrlButtonMake;
	CButton	m_ctrlButtonSave;
	CString	m_valueEditItemNum;
	CString	m_valueEditPrice;
	CString	m_valueEditCashPrice;
	CString	m_valueEditItemName;
	CString	m_valueEditReqMinLevel;
	CString	m_valueEditReqMaxLevel;
	CString	m_valueEditAbilityMin;
	CString	m_valueEditAbilityMax;
	CString	m_valueEditScarcity;
	CString	m_valueEditWeight;
	CString	m_valueEditRange;
	CString	m_valueEditTime;
	CString	m_valueEditMinTradeQuantity;
	CString	m_valueEditRangeAngle;
	CString	m_valueEditBoosterAngle;
	CString	m_valueEditReqAttack;
	CString	m_valueEditReqFuel;
	CString	m_valueEditReqShield;
	CString	m_valueEditReqDefense;
	CString	m_valueEditReqSoul;
	CString	m_valueEditReqDodge;
	CString	m_valueEditDescription;
	BOOL	m_valueCheckBGEAR;
	BOOL	m_valueCheckAGEAR;
	BOOL	m_valueCheckMGEAR;
	BOOL	m_valueCheckIGEAR;
	BOOL	m_valueCheckAutoPickUp;
	BOOL	m_valueCheckNoTransfer;
	BOOL	m_valueCheckQuestItem;
	BOOL	m_valueCheckTimeLimite;
	BOOL	m_valueCheckCashItem;
	CString	m_valueEditSourceIndex;
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 
	CEngineItemDlg					*m_pEngineItem;

	int m_nSelectRow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEngineItemDetailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEngineItemDetailDlg)
	afx_msg void OnButtonMake();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENGINEITEMDETAILDLG_H__B0EDBB63_A3A6_4DE0_BCEC_01DEAB86D3BB__INCLUDED_)
