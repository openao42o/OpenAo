#if !defined(AFX_SCSERVERADMINDLG_H__13433B21_9303_4EE4_9A45_8C8A3E16EFEF__INCLUDED_)
#define AFX_SCSERVERADMINDLG_H__13433B21_9303_4EE4_9A45_8C8A3E16EFEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// scserveradmindlg.h : header file
//

#include "GridCtrl/GridCtrl.h"
#include "main/SCAdminWinSocket.h"
#include "atumprotocol.h"
#include "TickManager.h"
#include "historycombobox.h"

class CSCServerAdminDlg;

class CSCServerAdminTicker : public CTickManager
{
public:
	CSCServerAdminTicker(DWORD i_nTickInterval, CSCServerAdminDlg *i_pSCServerAdminDlg);

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) {}
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// 매번 수행해야 할 일
	void DoDailyWork(ATUM_DATE_TIME *pDateTime) {}
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime) {}
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime) {}
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime) {}
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 2005-12-27 by cmkwon, 한달에 한 번 씩 해야 할 일

public:
	CSCServerAdminDlg *m_pSCServerAdminDlg;
};

class CAtumAdminToolDlg;
class CSCAdminPreWinSocket;
class CSCAdminIMWinSocket;
class CSCAdminFieldWinSocket;
class CMGameMonitor;

struct SERVER_INFO_ADMIN_TOOL
{
	SERVER_INFO_ADMIN_TOOL()
	{
		ServerName = "";
		ServerType = ST_INVALID_TYPE;
		ServerID.Reset();
		UserCount = 0;
		ServerState = SERVER_STATE_NOT_ACTIVE;
		LimitUserCount						= 0;		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
		LockCreateCharacterForNewAccount	= FALSE;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
		RowCountInGrid = -1;
	}

	CString			ServerName;
	ENServerType	ServerType;
	SERVER_ID		ServerID;
	INT				UserCount;
	BYTE			ServerState;		// SERVER_STATE_XXX
	INT				LimitUserCount;						// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	BOOL			LockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	INT				RowCountInGrid;
};

/////////////////////////////////////////////////////////////////////////////
// CSCServerAdminDlg dialog

class CSCServerAdminDlg : public CDialog
{
	friend class CSCServerAdminTicker;

// Construction
public:
	CSCServerAdminDlg(CWnd* pParent = NULL);   // standard constructor
	~CSCServerAdminDlg();

	void OnTimerForSendAlivePacket(void);
	void SendAlive2AllIMServer(void);
	void SendAlive2AllFieldServer(void);

// Dialog Data
	//{{AFX_DATA(CSCServerAdminDlg)
	enum { IDD = IDD_DIALOG_SERVER_ADMIN_TOOL };
	CEdit	m_ctlEditOutput;
	CHistoryComboBox	m_HistoryComboChat;
	CComboBox	m_ComboServerList;
	CString	m_szOutputText;
	CString	m_ctlStringTotalUserCounts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCServerAdminDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitGridServerInfo();

// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
//	void InsertServerInfoToGrid(MEX_SERVER_INFO_FOR_ADMIN *i_pServerInfo, int i_nRow);
	void InsertServerInfoToGrid(MEX_SERVER_GROUP_INFO_FOR_ADMIN *i_pServGInfo, int i_nIMServerRowIdx);
	void UpdateServerGroupInfoInGrid(SERVER_INFO_ADMIN_TOOL *i_pServInfoAdmin, int i_nIMServerRowIdx);

	SERVER_INFO_ADMIN_TOOL GetServerGroupInfo(const char *i_szServerGroupName);
	SERVER_INFO_ADMIN_TOOL GetServerInfoByServerID(SERVER_ID i_ServerID);

	BOOL SendChatToServerGroup(const char *i_szServerGroupName, const char *i_szChatMsg);

	CSCAdminFieldWinSocket* GetFieldWinSocket(SERVER_ID i_ServerID);

	void SetGridRowColor(INT rowCount, COLORREF clr);

	void ProcessPreServerProtocols(CSCAdminPreWinSocket *i_pPreWinSocket, MessageType_t i_nMsgType, char *i_pMsg);
	void ProcessIMServerProtocols(CSCAdminIMWinSocket *i_pIMWinSocket, MessageType_t i_nMsgType, char *i_pMsg);
	void ProcessFieldServerProtocols(CSCAdminFieldWinSocket *i_pFieldWinSocket, MessageType_t i_nMsgType, char *i_pMsg);

	void PrintOutput(char* pFormat, ...);
	void ResetOutput();

	// 서버 상태 체크 관련
	void CheckServerErrorState(BOOL i_bRecvErrorState=FALSE);
	BOOL CheckAllSessionState(void);

	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	int GetTotalLoginedUserCount(void);
	int GetTotalGameStartedUserCount(void);
	CString GetServerGroupNameFromSelectedGridRow(void);

public:
	CGridCtrl								m_GridServerInfo;

	CSCAdminPreWinSocket					*m_pServerAdminPreSocket;
	ez_map<string, CSCAdminIMWinSocket*>	m_mapServerAdminIMSocket;
	vector<CSCAdminFieldWinSocket*>			m_vectorServerAdminFieldSocket;

	CAtumAdminToolDlg						*m_pMainDlg;
	CSCServerAdminTicker					*m_pServerAdminTicker;


	// 서버 상태 체크 관련
	CMGameMonitor							*m_pMGameMonitor;
	BOOL									m_bErrorStateFlag;
	DWORD									m_dwTickLastRecvErrorState;

protected:

	// Generated message map functions
	//{{AFX_MSG(CSCServerAdminDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSendChat();
	afx_msg void OnButtonRefresh();
	afx_msg void OnCmgServerGetAccountList();
	afx_msg void OnButtonStopSound();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnBtnInitText();
	afx_msg void OnCmgChangeLockCreateCharacterForNewAccount();
	afx_msg void OnCmgSetLimitUserCount();
	afx_msg void OnCmgServerReloadInfluenceRate();
	//}}AFX_MSG
    afx_msg void OnGridRClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg LONG OnSocketNotifyPre(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSocketNotifyIM(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSocketNotifyField(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnSocketNotifyMGameMonitor(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessagePre(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessageIM(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessageField(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAsyncSocketMessageMGameMonitor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCSERVERADMINDLG_H__13433B21_9303_4EE4_9A45_8C8A3E16EFEF__INCLUDED_)
