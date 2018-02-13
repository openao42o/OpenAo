#if !defined(AFX_PARTNERDLG_H__B96E5678_B7E4_42EE_8FDA_B77CC44E2014__INCLUDED_)
#define AFX_PARTNERDLG_H__B96E5678_B7E4_42EE_8FDA_B77CC44E2014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PartnerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPartnerDlg dialog
#include "SCUserAdminDlg.h"

#define PARTNER_TYPE_DEFAULT			1000
#define PARTNER_TYPE_GROWTH_SOCKET_1	1001
#define PARTNER_MAX_LEVEL				25
#define PARTNER_SOCKET_ITEM_KIT			7032500
#define PARTNER_SOCKET_ITEM_BUFF		7032510
#define PARTNER_SOCKET_NON_USE			0
#define PARTNER_SOCKET_EMPTY			0
#define ITEM_HIDDEN						98
#define ITEM_NOTWEAR					0

struct PARTNER_INFO
{
	UINT64 ItemUID;
	int PartnerIndex;
	char name[40];
	int level;
	int Experience;
	int ExpRatio;
	int EnableSocketCount;
	UINT64  SocketItem[SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT];
	UINT64 HPKitItemNum; 
	float HPKitTriggerValue;
	UINT64 ShieldKitItemNum;
	float ShieldKitTriggerValue;
	UINT64 SPKitItemNum;
	float SPKitTriggerValue;
	UINT64 AutoSkilItemNum[SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT];
};

struct PARTNER_LIVEL_INFO
{
	int nPartherIndex;
	int nPartherLevel;
	int nExperience;
	int nUseWeaponIndex;
	int nSlotCountSkill;
	int nSlotCountSocket;
	int nKitLevelHP;
	int nKitLevelShield;
	int nKitLevelSP;
};


class CAtumAdminToolDlg;

class CPartnerDlg : public CDialog
{
	friend class CSCUserAdminDlg;
// Construction
public:
	CPartnerDlg(CAtumAdminToolDlg *i_pMainDlg, CSCUserAdminDlg *i_pUserAdminDlg, UID64_t i_StoreUniqueNumber, CWnd* pParent = NULL);   // standard constructor
	~CPartnerDlg();
	CAtumAdminToolDlg *m_pMainDlg;
	CSCUserAdminDlg *m_pUserAdminDlg;
	CODBCStatement	*m_pODBCStmt;
	int		m_nPartnerExpLevel[PARTNER_MAX_LEVEL+1];
	vector<UINT64>	vectSocketUniqueNumber;
	
// Dialog Data
	//{{AFX_DATA(CPartnerDlg)
	enum { IDD = IDD_DIALOG_PARTNER };
	CComboBox	m_CComboPartnerProssessSocketList;
	CSpinButtonCtrl	m_PartnerLvCurrentSpin;
	CString	m_szAccountName;
	CString	m_szCharacterName;
	CString	m_szPartnerName;
	int		m_nPartnerType;
	int		m_nPartnerLvMax;
	int		m_nPartnerLvCurrent;
	int		m_nPartnerExpMax;
	int		m_nPartnerExpCurrent;
	int		m_nPartnerExpPercentage;
	CSliderCtrl	m_PartnerExpSlider;
	UID64_t m_nStoreUniqueNumber;
	PARTNER_INFO m_tPartnerInfo;
	PARTNER_INFO m_tUpdatePartnerInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartnerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPartnerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnSockDel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateEditPartnerLvCurrent();
	afx_msg void OnUpdateEditPartnerExpCurrent();
	afx_msg void OnUpdateEditPartnerExpPercentage();
	afx_msg void OnUpdateEditPartnerName();
	afx_msg void OnBtnPartnerSave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTNERDLG_H__B96E5678_B7E4_42EE_8FDA_B77CC44E2014__INCLUDED_)
