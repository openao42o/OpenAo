#if !defined(AFX_WEAPONITEMDETAIL_H__A49812BF_E645_4586_9D8D_E070DB96A8BE__INCLUDED_)
#define AFX_WEAPONITEMDETAIL_H__A49812BF_E645_4586_9D8D_E070DB96A8BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WeaponItemDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeaponItemDetail dialog

#include "AtumParam.h"
#include "stdafx.h"
#include "resource.h"

class CLinkDetailDlg;
class CSpaceCowboyDBEditorToolDlg;
class CWeaponItem;
class CWeaponItemDetail : public CDialog
{
// Construction
public:
	void SetDetailLinkDetail(_TI_ITEM *i_pTi_Item);
	BOOL ValidateCheck();
	void ReflectionToGrid(int type);
	CWeaponItemDetail(CWnd* pParent = NULL);   // standard constructor
	CWeaponItemDetail::~CWeaponItemDetail();

// Dialog Data
	//{{AFX_DATA(CWeaponItemDetail)
	enum { IDD = IDD_DIALOG_WEAPON_DETAIL };
	CButton	m_ctrlCheckCashItem;
	CComboBox	m_ctrlComboItemAttribute;
	CComboBox	m_ctrlComboRace2;
	CButton	m_ctrlButtonSave;
	CButton	m_ctrlButtonCancel;
	CButton	m_ctrlButtonMake;
	CComboBox	m_ctrlComboLink;
	CComboBox	m_ctrlComboPosition;
	CComboBox	m_ctrlComboItemKind;
	CComboBox	m_ctrlComboRace;
	CComboBox	m_ctrlComboKind;
	CString	m_valueEditItemNum;
	CString	m_valueEditAttackMax;
	CString	m_valueEditAttackMin;
	CString	m_valueEditReqAttack;
	CString	m_valueEditReqDefense;
	CString	m_valueEditReqFuel;
	CString	m_valueEditReqSoul;
	CString	m_valueEditReqShield;
	CString	m_valueEditReqDodge;
	BOOL	m_valueCheckBGEAR;
	BOOL	m_valueCheckIGEAR;
	BOOL	m_valueCheckMGEAR;
	BOOL	m_valueCheckAGEAR;
	CString	m_valueEditMinLevel;
	CString	m_valueEditMaxLevel;
	CString	m_valueEditWeight;
	CString	m_valueEditHitRate;
	CString	m_valueEditDefense;
	CString	m_valueEditRange;
	CString	m_valueEditAbrasionRate;
	CString	m_valueEditCharging;
	CString	m_valueEditMinTradeQuantity;
	CString	m_valueEditPrice;
	CString	m_valueEditCashPrice;
	CString	m_valueEditReattackTime;
	CString	m_valueEditOverHitTime;
	CString	m_valueEditMultiTarget;
	CString	m_valueEditExplosionRange;
	CString	m_valueEditReactionRange;
	CString	m_valueEditShotNum;
	CString	m_valueEditMultiNum;
	CString	m_valueEditAttackTime;
	CString	m_valueEditItemName;
	CString	m_valueEditDescription;
	BOOL	m_valueCheckCashItem;
	CString	m_valueEditCaliber;
	CString	m_valueEditOrbitType;
	CString	m_valueEditCameraPattern;
	BOOL	m_valueCheckAutoPickUp;
	BOOL	m_valueCheckQuestItem;
	BOOL	m_valueCheckTimeLimite;
	BOOL	m_valueCheckNoTransfer;
	CString	m_valueEditRangeAngle;
	CString	m_valueEditSourceIndex;
	//}}AFX_DATA
	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; 
	CWeaponItem					*m_pWeaponItem;
	CLinkDetailDlg				*m_pLinkDetailDlg;

	int m_nSelectRow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeaponItemDetail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWeaponItemDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonMake();
	afx_msg void OnEditchangeComboKind();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLinkdetail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEAPONITEMDETAIL_H__A49812BF_E645_4586_9D8D_E070DB96A8BE__INCLUDED_)
