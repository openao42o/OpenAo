// AtumDlg.h : header file
//

#if !defined(AFX_ATUMDLG_H__9A30D9F2_14D0_4046_A219_C4B9DBC64A41__INCLUDED_)
#define AFX_ATUMDLG_H__9A30D9F2_14D0_4046_A219_C4B9DBC64A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PreUpdateWinSocket.h"
#include "AtumProtocol.h"


// 2008-01-30 by cmkwon, C_Exe1: 런처 업데이트시에 PreServer로 Alive Packet 전송하기 - 
#define TIMERID_SEND_ALIVE_PACKET		3


/////////////////////////////////////////////////////////////////////////////
// CAtumDlg dialog
class CHttpManager;
class CAtumDlg : public CDialog
{
// Construction
public:
	CAtumDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAtumDlg)
	enum { IDD = IDD_ATUM_DIALOG };
	CProgressCtrl	m_progressCtrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CPreUpdateWinSocket	*m_pPreUpdateWinsocket;
	VersionInfo			m_CurrentLauncherVersion;
	VersionInfo			m_UpdatingLauncherVersion;		// 2007-01-06 by cmkwon
	CString				m_szCrocessSuffix;
	CString				m_szMainPreServerIP;
	CBitmap				m_BackGround;


	CHttpManager		*m_pHttpManager;				// 2007-01-05 by cmkwon
	vectSGAME_SERVER_GROUP		m_vectGameServerGroupList;			// 2007-05-15 by cmkwon

	[[deprecated]] BOOL GetLauncherFileFTP(MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo);
	BOOL GetLauncherFileHTTP(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO *pMsgUpdateInfo);
	void ExecLauncher();
	void ExecLauncherCrocess();

	int AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	//add
	int AtumMessageBox_info(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor/* = 0*/, COLORREF cTolerance/* = 0x101010*/, int nx, int ny);
	BOOL BitmapRgn(LPCTSTR resource, COLORREF TansColor, int nx, int ny);
	BOOL BitmapRgn(UINT resource, COLORREF TansColor, int nx, int ny);
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAtumDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);

	afx_msg LONG OnUpdateFileDownloadError(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadOK(WPARAM wParam, LPARAM lParam);	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawProgress2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedPictureboxProgressbar();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMDLG_H__9A30D9F2_14D0_4046_A219_C4B9DBC64A41__INCLUDED_)
