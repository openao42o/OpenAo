// AdminWindowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "AdminWindowDlg.h"
#include "md5_lib_src.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminWindowDlg dialog


CAdminWindowDlg::CAdminWindowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminWindowDlg)
	m_szAdminCommand = _T("");
	m_szDbgOutBin = _T("");
	m_szCmdOut = _T("");
	//}}AFX_DATA_INIT

	m_pCmdHist = new CCommandHistory(20, FALSE, FALSE, "./command_history.txt");
}

CAdminWindowDlg::~CAdminWindowDlg()
{
	m_odbcStmt.Clean();
	delete m_pCmdHist;
}


void CAdminWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminWindowDlg)
	DDX_Control(pDX, IDC_EDIT_CMD_OUT, m_ctrlCmdOut);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_szAdminCommand);
	DDX_Text(pDX, IDC_EDIT_DBGOUT_BIN, m_szDbgOutBin);
//	DDX_Text(pDX, IDC_EDIT_CMD_OUT, m_szCmdOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminWindowDlg, CDialog)
	//{{AFX_MSG_MAP(CAdminWindowDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_EXEC_DBOUT, OnButtonExecDbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminWindowDlg message handlers

void CAdminWindowDlg::OnOk()
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if (m_szAdminCommand == "")
	{
		return;
	}

	m_pCmdHist->AddCommand((LPCSTR)m_szAdminCommand);

//	m_szCmdOut = "";
	printf2("\r\n===== Command: %s =========================================================\r\n\r\n", m_szAdminCommand);

	BOOL nRet = HandleAdminCommand((LPCSTR)m_szAdminCommand);
	if (nRet)
	{
		printf2("  ==> Command succeeded.\r\n");
	}
	else
	{
		printf2("  ==> Command failed.\r\n");
	}

//	m_szCmdOut += "\r\n";
	//GetDlgItem(IDC_EDIT_CMD_OUT)->SetWindowText(m_szCmdOut);

	m_szAdminCommand = "";
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();

	m_pCmdHist->ResetPosition();

	return;
}

BOOL CAdminWindowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_szAdminCommand = "";
	UpdateData(FALSE);

//	printf2("\r\n== Admin Command window opened ==================================\r\n");

	((CAtumMonitorApp*)AfxGetApp())->m_szDbgOutBin = m_szDbgOutBin;
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();

//	UINT ret = WinExec(m_szDbgOutBin, SW_SHOWNORMAL);

	m_ServerDBConnDlg.SetAttr(TRUE, FALSE, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdminWindowDlg::OnButtonExecDbout()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	((CAtumMonitorApp*)AfxGetApp())->m_szDbgOutBin = m_szDbgOutBin;
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();

	UINT ret = WinExec(m_szDbgOutBin, SW_SHOWNORMAL);
}

void CAdminWindowDlg::OnCancel()
{
	// TODO: Add extra cleanup here
//	printf2("== Admin Command window closed ==================================\r\n");

	CDialog::OnCancel();
}

BOOL CAdminWindowDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP && GetFocus()->GetDlgCtrlID() == IDC_EDIT_COMMAND)
	{
		char *cmd = NULL;
		if ((cmd = m_pCmdHist->GetNext()) == NULL)
		{
			GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_COMMAND))->SetSel(0, -1);
		}
		else
		{
			GetDlgItem(IDC_EDIT_COMMAND)->SetWindowText(cmd);

			GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_COMMAND))->SetSel(0, -1);
		}

		return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN && GetFocus()->GetDlgCtrlID() == IDC_EDIT_COMMAND)
	{
		char *cmd = NULL;
		if ((cmd = m_pCmdHist->GetPrev()) == NULL)
		{
			GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_COMMAND))->SetSel(0, -1);
		}
		else
		{
			GetDlgItem(IDC_EDIT_COMMAND)->SetWindowText(cmd);

			GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_COMMAND))->SetSel(0, -1);
		}
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAdminWindowDlg::HandleAdminCommand(const char* cmd)
{
	if (cmd == NULL)
		return FALSE;

	char *token;
	char seps[] = " \t";
	char buffer[SIZE_MAX_CHAT_MESSAGE];

	util::strncpy(buffer, cmd, SIZE_MAX_CHAT_MESSAGE);
	token = strtok(buffer, seps);

	if (token == NULL)
	{
		return FALSE;
	}

	if (!m_odbcStmt.IsReadyToUse())
	{
		if (IDOK != m_ServerDBConnDlg.DoModal())
			return FALSE;

		GAME_SERVER_INFO_FOR_ADMIN *pInfo = m_ServerDBConnDlg.m_pServerInfo4Admin;
		if (!m_odbcStmt.Init(pInfo->DBIP, pInfo->DBPort, pInfo->DBName, pInfo->DBUID, pInfo->DBPWD, GetSafeHwnd()))
		{
			MessageBox("Can not connect to DB Server!!");
			return FALSE;
		}
	}

	// format: "/암호설정 [AccountName] [Password]"
	if ( stricmp(token, STRCMD_CS_COMMAND_PASSWORDSET) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_PASSWORDSET_1))
	{
		string szAccountName;
		string szPassword;

		token = strtok(NULL, seps);
		if (token == NULL)
		{
			return FALSE;
		}
		szAccountName = token;

		token = strtok(NULL, seps);
		if (token == NULL)
		{
			return FALSE;
		}
		szPassword = token;

		BOOL bRet = ChangeUserPassword(szAccountName.c_str(), szPassword.c_str());

		PrintAllBackupAccounts();

		return bRet;
	}
	// format: "/암호복구 [AccountName]"
	else if ( stricmp(token, STRCMD_CS_COMMAND_PASSWORDROLLBACK) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_PASSWORDROLLBACK_1))
	{
		string szAccountName;
		string szPassword;

		token = strtok(NULL, seps);
		if (token == NULL)
		{
			return FALSE;
		}
		szAccountName = token;

		BOOL bRet = RecoverUserPassword(szAccountName.c_str());

		PrintAllBackupAccounts();

		return bRet;
	}
	// format: "/암호목록"
	else if ( stricmp(token, STRCMD_CS_COMMAND_PASSWORDLIST) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_PASSWORDLIST_1))
	{
		PrintAllBackupAccounts();

		return TRUE;
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_PASSWORDENCRYPT) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_PASSWORDENCRYPT_1))
	{
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			return FALSE;
		}

		MD5 MD5_instance;
		unsigned char md5_string[16];
		char szEncodedString[33];

		MD5_instance.MD5Encode(token, md5_string);
		MD5_instance.MD5Binary2String(md5_string, szEncodedString);
		printf2("%s -> %s\r\n", token, szEncodedString);

		return TRUE;

	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_ACCOUNTBLOCK) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_ACCOUNTBLOCK_1))
	{
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			return FALSE;
		}

		return BlockAccount(token);
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE_1))
	{
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			return FALSE;
		}

		return UnblockAccount(token);
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST_1))
	{
		return ListupBlockedAccount();
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0
		|| stricmp(token, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0
		|| stricmp(token, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0
	)
	{
		printf2("Command List\r\n");
		printf2("  %s\r\n", STRCMD_CS_COMMAND_PASSWORDSET_HELP);
		printf2("  %s\r\n", STRCMD_CS_COMMAND_PASSWORDROLLBACK_HELP);
		printf2("  %s\r\n", STRCMD_CS_COMMAND_PASSWORDLIST_HELP);
		printf2("  %s\r\n", STRCMD_CS_COMMAND_PASSWORDENCRYPT_HELP);
		printf2("  %s\r\n", STRCMD_CS_COMMAND_ACCOUNTBLOCKT_HELP);
		printf2("  %s\r\n", STRCMD_CS_COMMAND_ACCOUNTBLOCKRELEASE_HELP);
		printf2("  %s\r\n", STRCMD_CS_COMMAND_ACCOUNTBLOCKLIST_HELP);		
		return TRUE;
	}

	return FALSE;
}

void CAdminWindowDlg::printf2(char* pFormat, ...)
{
	char buff2[1024];

	va_list args;
	va_start(args, pFormat);
	vsprintf(buff2, pFormat, args);
	va_end(args);

	m_szCmdOut += buff2;

	m_ctrlCmdOut.SetWindowText(m_szCmdOut);
	m_ctrlCmdOut.SetSel(m_szCmdOut.GetLength(), m_szCmdOut.GetLength());

	DBGOUT(buff2);
}

BOOL CAdminWindowDlg::PrintAllBackupAccounts()
{
//	char		SQLString[1024];
	RETCODE	ret;
	SQLINTEGER pcbNTS = SQL_NTS;
	SQLINTEGER cb1, cb2, cb3; //, cb4, cb5, cb6, cb7, cb8, cb9, cb10,
//			cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20,
//			cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28, cb29, cb30,
//			cb31, cb32, cb33, cb34, cb35, cb36, cb37, cb38, cb39, cb40,
//			cb41, cb42, cb43, cb44, cb45, cb46, cb47, cb48, cb49, cb50, cb51;

	char AccountName[SIZE_MAX_ACCOUNT_NAME];
	char OrgPassword[SIZE_MAX_PASSWORD_MD5_STRING];
	char TempPassword[SIZE_MAX_PASSWORD_MD5_STRING];

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(SQLString, "SELECT a.AccountName, a.Password as TempPassword, ak.Password as OrgPassword\
	//					FROM td_account a, td_accountBackup ak\
	//					WHERE a.accountname = ak.accountname"
	//					);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(SQLString, QUERY_080702_0236);

	ret = SQLExecDirect(m_odbcStmt.GetSTMTHandle(), (UCHAR*)(PROCEDURE_080827_0236), SQL_NTS);

	cb1 = SQL_NTS;
	SQLBindCol(m_odbcStmt.GetSTMTHandle(), 1, SQL_C_CHAR, AccountName, SIZE_MAX_ACCOUNT_NAME, &cb1);
	cb2 = SQL_NTS;
	SQLBindCol(m_odbcStmt.GetSTMTHandle(), 2, SQL_C_CHAR, TempPassword, SIZE_MAX_PASSWORD_MD5_STRING, &cb2);
	cb3 = SQL_NTS;
	SQLBindCol(m_odbcStmt.GetSTMTHandle(), 3, SQL_C_CHAR, OrgPassword, SIZE_MAX_PASSWORD_MD5_STRING, &cb3);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		// 실패
		m_odbcStmt.ProcessLogMessagesForStmt("@CAdminWindowDlg::PrintAllBackupAccounts() Failed!\r\n");

		// close up
		m_odbcStmt.FreeStatement();

		return FALSE;
	}

	printf2("----- Backed up account list ---------------------------------------\r\n");
	while ( (ret = SQLFetch(m_odbcStmt.GetSTMTHandle())) != SQL_NO_DATA)
	{
		printf2("  AccountName: \'%s\', Original Password: \'%s\', Temporary Password: \'%s\'\r\n"
			, AccountName, OrgPassword, TempPassword);
	}
	printf2("--------------------------------------------------------------\r\n\r\n");

	// close cursor
	m_odbcStmt.FreeStatement();

	return TRUE;
}

BOOL CAdminWindowDlg::ChangeUserPassword(CString szAccount, CString szPassword)
{
//	char		SQLString[1024];
	BOOL		bRet = FALSE;

	SQLHSTMT	hstmt = m_odbcStmt.GetSTMTHandle();

//	RETCODE	ret;
	SQLINTEGER pcbNTS = SQL_NTS;
//	SQLINTEGER cb1, cb2, cb4, cb5, cb6, cb7, cb8, cb9, cb10,
//			cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20,
//			cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28, cb29, cb30,
//			cb31, cb32, cb33, cb34, cb35, cb36, cb37, cb38, cb39, cb40,
//			cb41, cb42, cb43, cb44, cb45, cb46, cb47, cb48, cb49, cb50, cb51;

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(SQLString, "INSERT INTO AccountBackup\
// 							SELECT AccountName, Password\
// 							FROM td_Account\
// 							WHERE AccountName like \'%s\';"
// 						, szAccount);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(SQLString, QUERY_080702_0237, szAccount);
//	ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
	char tmAccount[SIZE_MAX_ACCOUNT_NAME];
	util::zero(tmAccount, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(tmAccount, (LPSTR)(LPCSTR)szAccount, SIZE_MAX_ACCOUNT_NAME);
	SQLBindParameter(m_odbcStmt.m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
	bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0237));

	if (FALSE == bRet)
	{
		// 실패
		m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::ChangeUserPassword() 1 Failed!\r\n", TRUE);

		// close cursor
		m_odbcStmt.FreeStatement();

		return FALSE;
	}
	else
	{
		// close cursor
		m_odbcStmt.FreeStatement();

		// 성공
		bRet = TRUE;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//sprintf(SQLString, "UPDATE td_Account SET Password = \'%s\' WHERE AccountName like \'%s\';"
		//					, szPassword, szAccount);
		// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		sprintf(SQLString, QUERY_080702_0238, szPassword, szAccount);

//		ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
		char tmPassword[SIZE_MAX_PASSWORD_MD5_STRING];
		util::zero(tmPassword, SIZE_MAX_PASSWORD_MD5_STRING);
		strcpy(tmPassword, (LPSTR)(LPCSTR)szPassword);
		char tmAccount[SIZE_MAX_ACCOUNT_NAME];
		util::zero(tmAccount, SIZE_MAX_ACCOUNT_NAME);
		strcpy(tmAccount, (LPSTR)(LPCSTR)szAccount);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, tmPassword, 0, NULL);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
		bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0238));
		if (FALSE == bRet)
		{
			// 실패
			m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::ChangeUserPassword() 2 Failed!\r\n", TRUE);

			// close cursor
			m_odbcStmt.FreeStatement();

			return FALSE;
		}
		else
		{
			// 성공
			bRet = TRUE;
		}
	}

	// close cursor
	m_odbcStmt.FreeStatement();

	return bRet;
}

BOOL CAdminWindowDlg::RecoverUserPassword(CString szAccount)
{
//	char		SQLString[1024];
	BOOL		bRet = FALSE;

	SQLHSTMT	hstmt = m_odbcStmt.GetSTMTHandle();

//	RETCODE	ret;
	SQLINTEGER pcbNTS = SQL_NTS;
//	SQLINTEGER cb1, cb2, cb4, cb5, cb6, cb7, cb8, cb9, cb10,
//			cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20,
//			cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28, cb29, cb30,
//			cb31, cb32, cb33, cb34, cb35, cb36, cb37, cb38, cb39, cb40,
//			cb41, cb42, cb43, cb44, cb45, cb46, cb47, cb48, cb49, cb50, cb51;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(SQLString, "UPDATE td_Account SET Password = (SELECT Password FROM td_AccountBackup WHERE AccountName like \'%s\')\
	//					WHERE AccountName LIKE \'%s\';"
	//					, szAccount, szAccount);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(SQLString, QUERY_080702_0239, szAccount, szAccount);

//	ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
	char tmAccount[SIZE_MAX_ACCOUNT_NAME];
	util::zero(tmAccount, SIZE_MAX_ACCOUNT_NAME);
	strcpy(tmAccount, (LPSTR)(LPCSTR)szAccount);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
	bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0239));
	if (FALSE == bRet)
	{
		// 실패
		m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::RecoverUserPassword() 1 Failed!\r\n", TRUE);

		// close cursor
		m_odbcStmt.FreeStatement();

		return FALSE;
	}
	else
	{
		// close cursor
		m_odbcStmt.FreeStatement();

		// 성공
		bRet = TRUE;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//sprintf(SQLString, "DELETE FROM td_AccountBackup WHERE AccountName like \'%s\';"
		//					, szAccount);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		sprintf(SQLString, QUERY_080702_0240, szAccount);

//		ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
		char tmAccount[SIZE_MAX_ACCOUNT_NAME];
		util::zero(tmAccount, SIZE_MAX_ACCOUNT_NAME);
		strcpy(tmAccount, (LPSTR)(LPCSTR)szAccount);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
		bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0240));
		if (FALSE == bRet)
		{
			// 실패
			m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::RecoverUserPassword() 2 Failed!\r\n", TRUE);

			// close cursor
			m_odbcStmt.FreeStatement();

			return FALSE;
		}
		else
		{
			// 성공
			bRet = TRUE;
		}
	}

	// close cursor
	m_odbcStmt.FreeStatement();

	return bRet;
}

BOOL CAdminWindowDlg::BlockAccount(CString szAccount)
{
//	char		SQLString[1024];
	BOOL		bRet = FALSE;

	SQLHSTMT	hstmt = m_odbcStmt.GetSTMTHandle();

//	RETCODE	ret;
	SQLINTEGER pcbNTS = SQL_NTS;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(SQLString, "INSERT INTO BlockedAccounts VALUES(\'%s\')", szAccount);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(SQLString, QUERY_080702_0241, szAccount);

//	ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
	char tmAccount[SIZE_MAX_ACCOUNT_NAME];
	util::zero(tmAccount, SIZE_MAX_ACCOUNT_NAME);
	strcpy(tmAccount, (LPSTR)(LPCSTR)szAccount);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
	bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0241));
	if (FALSE == bRet)
	{
		// 실패
		m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::BlockAccount() Failed!\r\n", TRUE);

		// close cursor
		m_odbcStmt.FreeStatement();

		return FALSE;
	}

	// close cursor
	m_odbcStmt.FreeStatement();

	return TRUE;
}

BOOL CAdminWindowDlg::UnblockAccount(CString szAccount)
{
//	char		SQLString[1024];
	BOOL		bRet = FALSE;

	SQLHSTMT	hstmt = m_odbcStmt.GetSTMTHandle();

//	RETCODE	ret;
	SQLINTEGER pcbNTS = SQL_NTS;

	// 2008-01-29 by cmkwon, T_A: 계정 블럭 관련 쿼리 수정
	//sprintf(SQLString, "DELETE FROM td_BlockedAccounts WHERE AccountName like \'%s\'", szAccount);
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(SQLString, "DELETE FROM td_BlockedAccounts WHERE AccountName = \'%s\'", szAccount);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(SQLString, QUERY_080702_0242, szAccount);

//	ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
	char tmAccount[SIZE_MAX_ACCOUNT_NAME];
	util::zero(tmAccount, SIZE_MAX_ACCOUNT_NAME);
	strcpy(tmAccount, (LPSTR)(LPCSTR)szAccount);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, tmAccount, 0, NULL);
	bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0242));
	if (FALSE == bRet)
	{
		// 실패
		m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::BlockAccount() Failed!\r\n", TRUE);

		// close cursor
		m_odbcStmt.FreeStatement();

		return FALSE;
	}

	// close cursor
	m_odbcStmt.FreeStatement();

	return TRUE;
}

BOOL CAdminWindowDlg::ListupBlockedAccount()
{
//	char		SQLString[1024];
	BOOL		bRet = FALSE;
	char		AccountName[SIZE_MAX_ACCOUNT_NAME];

	SQLHSTMT	hstmt = m_odbcStmt.GetSTMTHandle();

	RETCODE	ret;
	SQLINTEGER pcbNTS = SQL_NTS;

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(SQLString, "SELECT * FROM td_BlockedAccounts WITH (NOLOCK)");
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(SQLString, QUERY_080702_0243);

	SQLBindCol(hstmt, 1, SQL_C_CHAR, AccountName, SIZE_MAX_ACCOUNT_NAME, &pcbNTS);

//	ret = SQLExecDirect(hstmt, (UCHAR*)SQLString, SQL_NTS);
	bRet = m_odbcStmt.ExecuteQuery((char*)(PROCEDURE_080827_0243));
	if (FALSE == bRet)
	{
		// 실패
		m_odbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@CAdminWindowDlg::BlockAccount() Failed!\r\n", TRUE);

		// close cursor
		m_odbcStmt.FreeStatement();

		return FALSE;
	}

	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		printf2("  AccountName: \'%s\' \r\n", AccountName);
	}

	// close cursor
	m_odbcStmt.FreeStatement();

	return TRUE;
}
