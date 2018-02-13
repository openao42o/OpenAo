#if !defined(AFX_DELETEDCHARACTER_H__40853BF9_880C_4592_9DA7_3ECAC8EE8894__INCLUDED_)
#define AFX_DELETEDCHARACTER_H__40853BF9_880C_4592_9DA7_3ECAC8EE8894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeletedCharacter.h : header file
//
#include "GridCtrl/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDeletedCharacter dialog

class CODBCStatement;
class CDeletedCharacter : public CDialog
{
// Construction
public:
	CDeletedCharacter(char *i_szAccName, CODBCStatement *i_pODBCState, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeletedCharacter)
	enum { IDD = IDD_DLG_DELETED_CHARACTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeletedCharacter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	CGridCtrl				m_GridDeletedCharacterList;
	CODBCStatement			*m_pODBCStmt3;
	char					m_szAccountName1[SIZE_MAX_ACCOUNT_NAME];

	void InitGridDeletedCharacterList(void);
	void InsertDeletedCharacterData(SDELETED_CHARACTER_INFO *i_pDeletedCharacterData);
	void DBLoadDeletedCharacterData(void);
	INT DBLiveDeletedCharacterData(UID32_t TmSelectedAccountUID, UID32_t TmSelectedCharacterUID);

	// 2008-01-17 by cmkwon, T_A: 삭제 상태의 캐릭터 영구히 삭제하는 기능 추가 - DBDeletePermanentlyDeletedCharacter() 추가
	BOOL DBDeletePermanentlyDeletedCharacter(SDELETED_CHARACTER_INFO *i_pDeletedCharacterData);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeletedCharacter)
	virtual BOOL OnInitDialog();
	afx_msg void OnConmenuSelectedUpdate();
	afx_msg void OnConmenuSelectedDeleteCharacter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELETEDCHARACTER_H__40853BF9_880C_4592_9DA7_3ECAC8EE8894__INCLUDED_)
