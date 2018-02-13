#if !defined(AFX_DLGSETINFLUENCERATE_H__7FB8222A_41F2_44C6_95AD_53B162AFD352__INCLUDED_)
#define AFX_DLGSETINFLUENCERATE_H__7FB8222A_41F2_44C6_95AD_53B162AFD352__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgsetinfluencerate.h : header file
//
#include "InfluenceRateGridCtrl.h"				// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
#include "ODBCStatement.h"				// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

/////////////////////////////////////////////////////////////////////////////
// CDlgSetInfluenceRate dialog

///////////////////////////////////////////////////////////////////////////////
/// \class		
///
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \version	
/// \date		2009-09-21 ~ 2009-09-21
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CDlgSetInfluenceRate : public CDialog
{
// Construction
public:
	CDlgSetInfluenceRate(BOOL i_bEnableEdit, CODBCStatement *i_pODBCState, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetInfluenceRate)
	enum { IDD = IDD_DLG_SET_INFLUENCE_RATE };
	BOOL	m_ctlbSetForAll;
	BOOL	m_ctlbSetForLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetInfluenceRate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CInfluenceRateGridCtrl	m_GridSetForLevel;
	CInfluenceRateGridCtrl	m_GridSetForAll;

	BOOL					m_bEnableEdit;
	CODBCStatement			*m_pODBCStmt5;
	mtvectSINFL_USER_COUNT	m_vectInflUserList;


	void InitGrid(CInfluenceRateGridCtrl *i_pGridCtrl);
	void AddItemToGrid(CInfluenceRateGridCtrl *i_pGridCtrl, SINFL_USER_COUNT *i_pUserCnt);
	void ArrangeSetInfluenceRate(void);
	BOOL UpdateDataInfluenceRate(void);
	BOOL UpdateDatatRowFromGrid(CInfluenceRateGridCtrl *i_pGridCtrl, int i_nRow, int i_nCol);
	SINFL_USER_COUNT * GetInflUserCount(mtvectSINFL_USER_COUNT *i_pUserCountList, BYTE i_byStartLv, BYTE i_byEndLv);
	
	BOOL DB_LoadInfluenceRateList(mtvectSINFL_USER_COUNT *o_pvectInflUserList);
	BOOL DB_UpdateInfluenceRateList(BOOL i_bSetForAll, mtvectSINFL_USER_COUNT *i_pvectInflUserList);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetInfluenceRate)
	afx_msg void OnCHECKSetForLevel();
	afx_msg void OnCHECKSetForAll();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnGridClickSetForLevel(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClickSetForAll(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETINFLUENCERATE_H__7FB8222A_41F2_44C6_95AD_53B162AFD352__INCLUDED_)
