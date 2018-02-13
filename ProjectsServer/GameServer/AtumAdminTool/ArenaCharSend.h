#if !defined(AFX_ARENACHARSEND_H__9E210A91_A2C8_4BFB_B9F2_99F80803B380__INCLUDED_)
#define AFX_ARENACHARSEND_H__9E210A91_A2C8_4BFB_B9F2_99F80803B380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArenaCharSend.h : header file
//

#include "ODBCStatement.h"
#include "AtumAdminToolDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CArenaCharSend dialog
struct EnchantInfo
{
	int EnchantCount;
	int EnchantItemNum;
};

struct ItemInfo
{
	INT64 StoreUID;
	int ItemStorage;
	int	Wear;
	int	CurrentCount;
	int	ItemWindowIndex;
	int	ItemNum;
	int	PrefixCodeNum;
	int	SuffixCodeNum;
	float CurrentEndurance;
	int EnchantTypeCount;
	EnchantInfo SendEnchant[5];
};

struct CharacterInfo
{
	int CUID;
	char CharacterName[20];
	int Race;
	int UnitKind;
	int AttackPart;
	int DefensePart;
	int FuelPart;
	int SoulPart;
	int ShieldPart;
	int DodgePart;
	int Level;
	float Experience;
	INT64 BodyCondition;
	int Propensity;
	int Status;
	int HP;
	int DP;
	int SP;
	int EP;
	int BonusStat;
	int BonusStatPoint;
	int ItemCount;
	ItemInfo SendItem[30];
	int SkillCount;
	int SkillItemNum[30];
};

struct AccountInfo
{
	char AccountName[20];
	char Password[35];
	CharacterInfo SendCharacter[3];
};

class CArenaCharSend : public CDialog
{
// Construction
public:
	CArenaCharSend(CAtumAdminToolDlg *i_pMainDlg, CWnd* pParent = NULL);   // standard constructor
	~CArenaCharSend();
	CODBCStatement			*m_pODBCStmt;
	CAtumAdminToolDlg		*m_pMainDlg;

// Dialog Data
	//{{AFX_DATA(CArenaCharSend)
	enum { IDD = IDD_DIALOG_ARENA_CHAR_SEND };
	CString	m_editBAccName;
	CString	m_editAAccName;
	CString	m_editIAccName;
	CString	m_editMAccName;
	CString	m_editAAttName;
	CString	m_editBAttName;
	CString	m_editIAttName;
	CString	m_editMAttName;
	CString	m_editADefName;
	CString	m_editBDefName;
	CString	m_editIDefName;
	CString	m_editMDefName;
	CString	m_editADodName;
	CString	m_editBDodName;
	CString	m_editIDodName;
	CString	m_editMDodName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArenaCharSend)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CArenaCharSend::GetAccountInfo(AccountInfo *o_AccountInfo);
	void CArenaCharSend::GetCharacterInfo(CharacterInfo *o_CharacterInfo);
	void CArenaCharSend::GetEnchantInfo(ItemInfo *o_EnchantInfo);

	// Generated message map functions
	//{{AFX_MSG(CArenaCharSend)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCreate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARENACHARSEND_H__9E210A91_A2C8_4BFB_B9F2_99F80803B380__INCLUDED_)

