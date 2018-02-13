#if !defined(AFX_MONSTERDLG_H__9E9217CE_5E19_4D04_BFFC_A0185C69017C__INCLUDED_)
#define AFX_MONSTERDLG_H__9E9217CE_5E19_4D04_BFFC_A0185C69017C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonsterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMonsterDlg dialog
#include "resource.h"
#include "stdafx.h"

class CSpaceCowboyDBEditorToolDlg;

class CMonsterDlg : public CDialog
{
// Construction
public:
	void ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row); //Grid의 선택된 row 데이터를 다른 Grid로 이전
	void SetDetailDialog(int row); // 선택된 Row를 자식 다이얼로그의 컨트롤에 셋팅
	void GetCurrentList(); // DB에서 값을 가져와서 Current Grid Ctrl에 셋팅
	void InitGridUpdate(); // Update Grid Ctrl의 초기화
	void InitGridCurrent(); // Current Grid Ctrl의 초기화
	CMonsterDlg(CWnd* pParent = NULL);   // standard constructor
	CMonsterDlg::~CMonsterDlg();
	void InsertItemToGridCurrent(_TI_MONSTER *i_pTi_Monster, int i_nRow); //DB에서 select한 1줄의 데이터를 Current Grid Ctrl에 넣는 함수

// Dialog Data
	//{{AFX_DATA(CMonsterDlg)
	enum { IDD = IDD_DIALOG_MONSTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CODBCStatement					*m_pODBCStmt; //DB 접근 클래스 포인터
	CSpaceCowboyDBEditorToolDlg				*m_pMainDlg; //AtumAdminToll 다이얼로그 클래스
	CMonsterDetailDlg				*m_pMonsterDetailDlg; // 자식 다이얼로그 클래스 포인터
	CGridCtrl		m_GridCurrent; // Current Grid Ctrl
	CGridCtrl		m_GridUpdate; // Update Grid Ctrl
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonsterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMonsterDlg)
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

#endif // !defined(AFX_MONSTERDLG_H__9E9217CE_5E19_4D04_BFFC_A0185C69017C__INCLUDED_)
