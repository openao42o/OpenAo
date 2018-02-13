#if !defined(AFX_CHARACTERINFODLG_H__F0C34717_47E1_4CB5_B9E6_506A6EAAE2FF__INCLUDED_)
#define AFX_CHARACTERINFODLG_H__F0C34717_47E1_4CB5_B9E6_506A6EAAE2FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// characterinfodlg.h : header file
//

#include "atumparam.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfoDlg dialog
class CODBCStatement;
class CCharacterInfoDlg : public CDialog
{
	friend class CSCUserAdminDlg;
// Construction
public:
	CCharacterInfoDlg(BOOL i_bEnableEdit, CODBCStatement *i_pODBCState, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharacterInfoDlg)
	enum { IDD = IDD_DIALOG_CHARACTER_INFO };
	CString	m_CharacterName;
	int		m_DP;
	int		m_CurDP;
	int		m_HP;
	int		m_CurHP;
	int		m_SP;
	int		m_CurSP;
	int		m_EP;
	int		m_CurEP;
	int		m_ExpPercentage;
	int		m_Level;
	int		m_MapIndex;
	int		m_nChannelIndex;
	int		m_PartAttack;
	int		m_PartDefense;
	int		m_PartFuel;
	int		m_PartSoul;
	int		m_PartShield;
	int		m_PartDodge;
	int		m_BonusStat;
	int		m_BonusStatPoint;
	int		m_PosX;
	int		m_PosZ;
	int		m_Propensity;
	int		m_RacingPoint;
	double	m_ctl_doExp;
	BOOL	m_ctl_bInflLeader;
	BOOL	m_ctl_bInflSubLeader;
	BOOL	m_ctl_bInflSub2Leader;
	long	m_PCBangTotalPlayTime;
	int		m_ArenaCWP;
	int		m_ArenaDisConnect;
	int		m_ArenaLose;
	int		m_ArenaWin;
	int		m_ArenaWP;
	//}}AFX_DATA

	USHORT	m_Race;
	USHORT	m_RaceAccType;
	BYTE	m_byInfluenceType;

	BOOL	m_bEnableEdit;				// 2006-04-15 by cmkwon

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CODBCStatement			*m_pODBCStmt3;				// 2006-05-03 by cmkwon
	UID32_t					m_CharacterUID;				// 2006-05-03 by cmkwon
	USHORT					m_usUnitKind;				// 2007-01-11 by cmkwon, 
	UID32_t					m_AccountUID;				// 2007-02-13 by dhjin
	CString					m_csAccountName;			// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	BOOL					m_bReloadCharacterInfo;		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

// Implementation
protected:
	void Info2Character(CHARACTER *o_pCharac);
	void Character2Info(CHARACTER *i_pCharac, BOOL i_bInflLeader, BOOL i_bInflSub1Leader, BOOL i_bInflSub2Leader);
	BOOL StoreInflLeader(BYTE i_byInflTy, UID32_t i_LeaderCharUID);
	BOOL StoreInflSubLeader(BYTE i_byInflTy, UID32_t i_SubLeaderCharUID);
	BOOL StoreInflSub2Leader(BYTE i_byInflTy, UID32_t i_Sub2LeaderCharUID);

	BOOL DBQ_CheckEnableChangeInfluence(void);				// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	BOOL DBQ_InitializeInfluenceAccount(UID32_t i_AccUID);	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

	// Generated message map functions
	//{{AFX_MSG(CCharacterInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnCheckSetInflLeader();
	afx_msg void OnCHECKSubLeader();
	afx_msg void OnCHECKInflSub2Leader();
	afx_msg void OnBtnInitInfluence();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERINFODLG_H__F0C34717_47E1_4CB5_B9E6_506A6EAAE2FF__INCLUDED_)
