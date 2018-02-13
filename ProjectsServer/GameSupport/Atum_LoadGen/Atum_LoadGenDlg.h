// Atum_LoadGenDlg.h : header file
//

#if !defined(AFX_ATUM_LOADGENDLG_H__A2AB30A7_5507_4525_8F18_0F96C955A793__INCLUDED_)
#define AFX_ATUM_LOADGENDLG_H__A2AB30A7_5507_4525_8F18_0F96C955A793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "FieldWinSocket.h"

const UINT TIMERID_CONNECT					= 101;
const UINT TIMERID_SEND_MOVE_MSG			= 102;
const UINT TIMERID_SEND_PRI_ATTACK_MSG		= 103;		// 2007-01-23 by cmkwon
const UINT TIMERID_SEND_SEC_ATTACK_MSG		= 104;		// 2007-01-23 by cmkwon
const UINT TIMERID_SEND_CONNECT_ALIVE		= 105;		// 2008-03-12 by cmkwon

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenDlg dialog

class CAtum_LoadGenDlg : public CDialog
{
// Construction
public:
	CAtum_LoadGenDlg(CWnd* pParent = NULL);	// standard constructor

	BOOL						m_bStartFlag;
	UINT						m_uiConnectTimerID;
	UINT						m_uiSendMoveMSGTimerID;
	vector<CFieldWinSocket*>	m_vectFieldWinSocketPtr;

	UINT						m_uiTryConnectCount;

	UINT						m_uiSendAliveTimerID;		// 2008-03-12 by cmkwon
	
	CFieldWinSocket *FindFieldWinSoketPtr(SOCKET hSock);

	BOOL StartSendPriAttackTimer(void);
	BOOL StartSendSecAttackTimer(void);
	void ReleaseSendPriAttackTimer(void);
	void ReleaseSendSecAttackTimer(void);
// Dialog Data
	//{{AFX_DATA(CAtum_LoadGenDlg)
	enum { IDD = IDD_ATUM_LOADGEN_DIALOG };
	CString	m_ctl_strCharacterName;
	UINT	m_ctl_uiCharacterNameStartIndex;
	UINT	m_ctl_uiCharacterNameEndIndex;
	UINT	m_ctl_uiClientCount;
	UINT	m_ctl_uiConnectedCount;
	int		m_ctl_nX_End;
	int		m_ctl_nX_Start;
	int		m_ctl_nY;
	int		m_ctl_nZ_End;
	int		m_ctl_nZ_Start;	
	UINT	m_ctl_uiReconnectTime;
	CString	m_ctl_strFieldServerIP;
	UINT	m_ctl_uiFieldServerPort;
	UINT	m_ctl_uiConnectFailed;
	UINT	m_ctl_uiSpeed;
	UINT	m_ctl_uiSendMoveCount;
	BOOL	m_ctl_bSendAttackFlag;
	BOOL	m_ctl_bSendAttackSec;
	int		m_ctl_nPriReattakTime;
	int		m_ctl_nSecReattakTime;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtum_LoadGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAtum_LoadGenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnBtnSuspend();
	afx_msg void OnBtnAnalyzePacket();
	//}}AFX_MSG
	afx_msg LONG OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUM_LOADGENDLG_H__A2AB30A7_5507_4525_8F18_0F96C955A793__INCLUDED_)
