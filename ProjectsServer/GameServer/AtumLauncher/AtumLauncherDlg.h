// AtumLauncherDlg.h : header file
//

#if !defined(AFX_ATUMLAUNCHERDLG_H__58AA0312_8865_48DA_956A_3F204A6C82FA__INCLUDED_)
#define AFX_ATUMLAUNCHERDLG_H__58AA0312_8865_48DA_956A_3F204A6C82FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UpdateWinSocket.h"
#include "FtpDownload.h"
#include "resource.h"
#include "FTP/ProgressCtrlX.h"
#include "FTP/KbcButton.h"
#include "ListBoxEBX.h"
#include "comboboxebx.h"
#include "IExplore.h"



// 2008-12-17 by ckPark 러시아 런쳐
#include "MyEdit.h"
#include "MyCheck.h"
#include "MyComboBox.h"
#include "afxwin.h"


#define SERVER_DECA			0x00
#define SERVER_BATTALUS		0x01
#define SERVER_SHARRINE		0x02
#define SERVER_PHILON		0x03

#define GAME_MODE_WINDOW		0
#define GAME_MODE_FULLSCREEN	1


#define TIMERID_CONNECT_PRESERVER		1
#define TIMERID_ENABLE_CONTROL			2
#define TIMERID_SEND_ALIVE_PACKET		3
#define TIMERID_NETWORK_STATE_CHECK		4

typedef enum
{	
	UPDATE_STATE_INIT			= 0,
	UPDATE_STATE_CONNECTING		= 1,
	UPDATE_STATE_DOWNLOADING	= 2,
	UPDATE_STATE_DOWNLOADED		= 3
} FTP_UPDATE_STATE;

// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - 
struct SSERVER_GROUP_FOR_LAUNCHER
{
	int		nIndex;
	char	szServerGroupName[SIZE_MAX_SERVER_NAME];
};
typedef vector<SSERVER_GROUP_FOR_LAUNCHER>		vectSSERVER_GROUP_FOR_LAUNCHER;

typedef vector<DEVMODE>			vectDEVMODE;		// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - 

/////////////////////////////////////////////////////////////////////////////
// CAtumLauncherDlg dialog
class CFTPManager;
class CHttpManager;
class CVTCGuardManager;			// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경
class CScreenKeyboardDlg;		// 2007-09-10 by cmkwon, 베트남 화면키보드 구현 -
class CAtumLauncherDlg : public CDialog
{
// Construction
public:
	CAtumLauncherDlg(CWnd* pParent = NULL);	// standard constructor

	char *GetPublicLocalIP(char *o_szLocalIP);
	char				m_szLocalIP[SIZE_MAX_IPADDRESS];		// 2006-05-07 by cmkwon


	// 2004-07-05, cmkwon
	CBitmapButton		m_bitmapBtnCancel;
	CBitmapButton		m_bitmapBtnMin;
	CBitmapButton		m_bmpBtnHomepage;		// 2007-09-27 by cmkwon, Homepage가기 버튼 추가(베트남 VTC-Intecom 요청) - 멤버 변수 추가


	CBitmap				m_BackGround;
	CKbcButton			m_KbcGO;
	CKbcButton			m_kbcBtnJoin;

	//CFTPManager			*m_pUpdateFTPManager;
	FTP_UPDATE_STATE	m_FTPUpdateState;
	MSG_PC_CONNECT_UPDATE_INFO	m_msg_PC_CONNECT_UPDATE_INFO;

	CHttpManager		*m_pHttpManager;
	//CVTCGuardManager	*m_pVTCGuard;			// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

	void SetFTPUpdateState(FTP_UPDATE_STATE i_state){	m_FTPUpdateState = i_state;};


	CFont				m_fontServerGroupListBox;
	CFont				m_fontNoticeBox;

	CFont				m_fontDownloadFileNum;
	CFont				m_fontFileInfo;

// Dialog Data
	//{{AFX_DATA(CAtumLauncherDlg)
	enum { IDD = IDD_ATUMLAUNCHER_DIALOG };
	CButton	m_ctlBtnRememberID;
	CComboBoxEBX	m_comboServerList;
	
	CEdit		m_ctrlEditAccount;




	CListBoxEBX		m_ctrlServerList;			// 2006-05-02 by ispark
	CProgressCtrlX	m_progressCtrl;
	int		m_nServer;
	CString	m_szAccountName;
	CString	m_szPassword;
	CString	m_staticNumFileCtrl;	
	int		m_nWindowDegree;
	BOOL	m_ctlbWindowMode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumLauncherDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CUpdateWinSocket	*m_pUpdateWinsocket;
//	CFieldWinSocket		*m_pFieldWinsocket;
	SOCKET				m_PreSD;
	SOCKET				m_FieldSD;
	VersionInfo			m_CurrentVersion;
	VersionInfo			m_CurrentDelFileListVersion;
	VersionInfo			m_CurrentNoticeVersion;
//	CFtpDownload		m_FtpDownload;
	BOOL				m_bCancelFlag;
	BOOL				m_bProcessingVersionUpdate;
	char				m_PrivateIP[SIZE_MAX_IPADDRESS];
	BOOL				m_bShowPreServerIPDlg;
	BOOL				m_bUpdateClientFile;

	// config data in registry
	CString				m_szPreServerIPReg;
	CString				m_szExecutableBinaryReg;
	CString				m_szExecutePathReg;
	CString				m_szPreServerIPHistoryReg;
	CString				m_szExecuteBinHistoryReg;
	CString				m_szExecutePathHistoryReg;
	CString				m_szAccountNameReg;
	CString				m_szPasswordReg;
	//int					m_nServerGroupReg;
	CString				m_strServerGroupName;
	int					m_nWindowModeReg;	// GAME_MODE_FULLSCREEN(1) or GAME_MODE_WINDOW(0)
// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 아래의 스트링 변수로 수정 함
//	int					m_nWindowDegreeReg;
	CString				m_csWindowsResolutionReg;		// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 - 해상도 저장 시스템 수정


	BOOL				m_bGuardAgreementReg;		// 2009-08-31 by cmkwon, Gameforge4D 게임가드 동의창 띄우기 - 

	// 2015-12-16 Future, Vsync setting
	BOOL				m_bVSync;

	//ysw
	CString				m_NoticeEdit;
	int					m_Cur_Rect;
	DWORD				m_Cur_Percent;
	BOOL				m_SelectFlag;
	BOOL				m_bControlEnabled;		// control의 enable 상태를 저장
	CBrush				m_StaticBrushBlack;
	CBrush				m_StaticBrushGray;
	CBrush				m_ListBrushGray;
	CBrush				m_listBrush;

	CBrush				m_progressBk;
	CBrush				m_progressBar;

	Host				*m_pHost;				// 2006-05-02 by ispark, 웹페이지
	int					m_nOldSel;				// 7/13/2006 by dgwoo list Old Select item
	CListBoxEBX				*m_ServerList;			// 7/13/2006 by dgwoo combo & list box handler

	int					m_nBirthYear;			// 2007-06-05 by cmkwon, 출생년도

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-06-18 by cmkwon, 네트워크 상태 체크 
	int					m_nMaxNetworkCheckCount;		// 2007-06-18 by cmkwon, 테스트 횟수
	int					m_nCurNetworkCheckCount;		// 2007-06-18 by cmkwon, 현재 테스트 진행 횟수
	DWORD				m_dwNetworkCheckSendTick;		// 2007-06-18 by cmkwon, 테스트 패킷을 전송한 Tick
	DWORD				m_dwSumPacketTickGap;			// 2007-06-18 by cmkwon, 각 패킷 전송 후 받은 시간의 합
	BOOL NTStartNetworkCheck(void);
	int NTOnReceivedNetworkCheckOK(int i_nCheckCount);
	int NTGetPingAverageTime(void);
	BOOL NTCheckTimeOver(DWORD *o_pdwPingTime);


//	BOOL SetCurrentClientVersion();
	char* GetFileName(char *szFullPathName, char *szFileName);
	BOOL Tokenizer(CList <CString, CString&>& lsString, CString strMsg, TCHAR devide);
	int CreateDirectory(CString strParent, CList<CString, CString&> &lsDir);
	BOOL ConnectPreServer();


	void ExecGame(char *cmdLine, char *i_szAppPath=NULL, char *i_szCmdParam=NULL);


	[[deprecated]] static bool ProcessDeleteFileList(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo) { return true; }
	[[deprecated]] static bool ProcessNoticeFile(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo) { return true; }

	BOOL SendLogin(BYTE i_nLoginType);	// LOGIN_TYPE_XXX

	int AtumMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);


	static BOOL ProcessDeleteFileListByHttp(MSG_PC_CONNECT_SINGLE_FILE_UPDATE_INFO *pUpdateInfo);


	void MoveWindow2Center();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-09-10 by cmkwon, 베트남 화면키보드 구현 - 
	CScreenKeyboardDlg		*m_pScreenKeyboardDlg;
	CWnd					*m_pInputEditFromScreenKeyboard;
	BOOL					m_bHideScreenKeyboardByScreenKeyboardWindow;	// 2007-09-18 by cmkwon, 화상키보드 수정 - 
	void PushCharFromScreenKeyboard(char i_cPushChar);
	void DeleteCharFromScreenKeyboard(void);
	void HideScreenKeyboardByScreenKeyboardWindow(void);		// 2007-09-18 by cmkwon, 화상키보드 수정 -

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-27 by cmkwon, 윈도우즈 모드 기능 추가 -
	BOOL FindWindowResolutionByWindowDegree(int *o_pnCX, int *o_pnCY, int *o_pnDegree, char *i_szWindowDegreeName);
	int InsertWindowDegreeList(CComboBox *i_pComboBox, BOOL i_bWindowsMode);
	int FindWindowDegreeComboBoxIndex(CComboBox *i_pComboBox, char *i_szWindowDegreeName);

	// 2008-01-03 by cmkwon, 지원하는 해상도 리스트만 보여주기 - 
	vectDEVMODE				m_vectSupportedResolutionList;
	int InitSupportedWindowResolutionList(void);
	BOOL IsSupportedResolution(int i_nWidth, int i_nHeight);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-02-14 by cmkwon, 런처에서 서버그룹 명이 깨져도 게임 실행에 문제가 없도록 수정 - 
	vectSSERVER_GROUP_FOR_LAUNCHER	m_vectServerGroupList;
	BOOL FindServerGroupName(CString *o_pcsServerGroupName, int i_nFindIndex);

	CString					m_strEventURL;		// 2008-10-16 by cmkwon, Gameforge4D(Eng,Deu) Launcher Registry event 구현 - 런처 이벤트 URL

	CString					m_csSessionKey;		// 2008-12-18 by cmkwon, 일본 Arario 런처 수정 - CAtumLauncherDlg에 변수 추가
public:
	// display download progress & status
	void SetProgressRange(int up);
	void SetProgressPos(int pos);
	void SetFileNumText(char* str);
	void SetProgressGroupText(const char* str);

	void DisableControls();
	void EnableControls();

	void SetPrivateIP();

	BOOL DownloadUpdateFile(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo);
	void ExtractUpdateFile(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo);
	BOOL DownloadUpdateFileByHttp(MSG_PC_CONNECT_UPDATE_INFO *pMsgUpdateInfo);
	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor/* = 0*/, COLORREF cTolerance/* = 0x101010*/,int nx,int ny);
	BOOL BitmapRgn( LPCTSTR resource, COLORREF TansColor ,int nx,int ny);
	BOOL BitmapRgn( UINT resource, COLORREF TansColor ,int nx,int ny);

	//ysw : 추가
	BOOL	ReadNoticeFile();
	int		DrawProgressBar();
	void	Set_Cur_Percent(DWORD CurSize);

	// UpdateData overloading
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE)
	{
		BOOL bRet = FALSE;

		m_szAccountName.MakeLower();
		m_szAccountNameReg.MakeLower();

		bRet = CDialog::UpdateData(bSaveAndValidate);

		m_szAccountName.MakeLower();
		m_szAccountNameReg.MakeLower();

		return bRet;
	}

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAtumLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnJoin();
	afx_msg void OnSecret();
	afx_msg void OnEnd();
	afx_msg void OnDeca();
	afx_msg void OnBattal();
	afx_msg void OnSharin();
	afx_msg void OnPhilon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeList();
	afx_msg void OnCan();
	afx_msg void OnMin();
	afx_msg void OnSetfocusEditAccount();
	afx_msg void OnSetfocusEditPassword();
	afx_msg void OnBtnViewScreenKeyboard();
	afx_msg void OnBtnHomepage();
	afx_msg void OnCheckWindowsMode();
	//}}AFX_MSG
	afx_msg LONG OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnDownLoadGamefilesDone(WPARAM wParam, LPARAM lParam);

	afx_msg LONG OnUpdateFileDownloadError(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadInit(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnUpdateFileDownloadOK(WPARAM wParam, LPARAM lParam);	
	afx_msg LONG OnUpdateVTCGuardOK(WPARAM wParam, LPARAM lParam);		// 2012-02-09 by hskim, 베트남 X-Trap -> VTC 가드로 변경

	// ysw
	afx_msg UINT OnNcHitTest( CPoint point );  // <- 여기에 추가

	// 2015-12-16 Future, Vsync setting
	CButton m_ctlChckVsync;
	afx_msg void OnBnClickedCheckVSync();

   	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMLAUNCHERDLG_H__58AA0312_8865_48DA_956A_3F204A6C82FA__INCLUDED_)
