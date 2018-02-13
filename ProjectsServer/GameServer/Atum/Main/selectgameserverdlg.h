#if !defined(AFX_SELECTGAMESERVERDLG_H__07E2069B_25F7_453D_A6EC_D43F308A8E49__INCLUDED_)
#define AFX_SELECTGAMESERVERDLG_H__07E2069B_25F7_453D_A6EC_D43F308A8E49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// selectgameserverdlg.h : header file
//
#include "AtumProtocol.h"
#include <FTP\KbcButton.h>
#include "ListBoxEBX.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectGameServerDlg dialog
class Host;
class CSelectGameServerDlg : public CDialog
{
// Construction
public:
	CSelectGameServerDlg(vectSGAME_SERVER_GROUP *i_pGameServerGroupList, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectGameServerDlg)
	enum { IDD = IDD_DLG_SELECT_GAME_SERVER };
	CListBoxEBX	m_ctlLBLoginServer2;
	CListBoxEBX	m_ctlLBLoginServer1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectGameServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectGameServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDblclkListLoginserver1();
	afx_msg void OnDblclkListLoginserver2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetfocusListLoginserver1();
	afx_msg void OnSetfocusListLoginserver2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	vectSGAME_SERVER_GROUP *m_pGameServerGroupList;			// 2007-05-15 by cmkwon

	CBitmap					m_BackGround;			// 2007-07-04 by cmkwon, EXE1 인터페이스 수정
// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 기존 버튼 제거
//	CKbcButton				m_kbcGameServer[10];	// 2007-07-04 by cmkwon, EXE1 인터페이스 수정

	Host					*m_pHost1;				// 2007-09-03 by cmkwon, 로그인(PreServer)서버 선택창에 웹 추가 - 상위
	Host					*m_pHost2;				// 2007-09-03 by cmkwon, 로그인(PreServer)서버 선택창에 웹 추가 - 우측하단

	CBrush					m_listBrush;


public:
	SGAME_SERVER_GROUP		m_selectedGameServerGroup;		// 2007-05-15 by cmkwon
	void MoveWindow2Center(void);		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정, 백그라운드 크기로 화면 중앙으로 이동 시키기
	SGAME_SERVER_GROUP *FindLoginServerByLoginServerName(char *i_pLoginServerName);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTGAMESERVERDLG_H__07E2069B_25F7_453D_A6EC_D43F308A8E49__INCLUDED_)
