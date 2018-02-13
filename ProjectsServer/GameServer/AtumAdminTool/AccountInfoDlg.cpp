// AccountInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "AccountInfoDlg.h"
#include "AtumParam.h"
#include "md5_lib_src.h"
#include "SCUserAdminDlg.h"		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountInfoDlg dialog


CAccountInfoDlg::CAccountInfoDlg(BOOL i_bEnableEdit, CWnd* pParent /*=NULL*/)
	: CDialog(CAccountInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountInfoDlg)
	m_szAccountName = _T("");
	m_szPassword = _T("");
	m_szSex = _T("");
	m_szRegisterdDate = _T("");
	m_szLastLoginDate = _T("");
	m_bAccountBlocked = FALSE;
	m_bChattingBlocked = FALSE;
	m_szBirthYear = _T("");
	m_ctlcsSecPassword = _T("");
	//}}AFX_DATA_INIT

	m_nAcountType		= 0;
	m_bEnableEdit		= i_bEnableEdit;	// 2006-04-15 by cmkwon

	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	m_nOldAcountType		= 0;
	m_bChangeAccountType	= FALSE;
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
}


void CAccountInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountInfoDlg)
	DDX_Control(pDX, IDC_COMBO_RACE_ACC_TYPE2, m_ComboAccountType);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_szPassword);
	DDX_Text(pDX, IDC_EDIT_SEX, m_szSex);
	DDX_Text(pDX, IDC_EDIT_REG_DATE, m_szRegisterdDate);
	DDX_Text(pDX, IDC_EDIT_LAST_LOGIN_DATE, m_szLastLoginDate);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT_BLOCKED, m_bAccountBlocked);
	DDX_Check(pDX, IDC_CHECK_CHATTING_BLOCKED, m_bChattingBlocked);
	DDX_Text(pDX, IDC_EDIT_BIRTH_YEAR, m_szBirthYear);
	DDX_Text(pDX, IDC_EDIT_SECPASSWORD, m_ctlcsSecPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CAccountInfoDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_MD5_PWD, OnButtonMd5Pwd)
	ON_BN_CLICKED(IDC_BUTTON_MD5_SECPWD, OnButtonMd5Secpwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountInfoDlg message handlers

#define RACE_ACC_TYPE_COMBO_NO_TYPE			0 // (일반)
#define RACE_ACC_TYPE_COMBO_OPERATION		1 // (관리자)
#define RACE_ACC_TYPE_COMBO_GAMEMASTER		2 // (게임마스터)
#define RACE_ACC_TYPE_COMBO_MONITOR			3 // (모니터)
#define RACE_ACC_TYPE_COMBO_GUEST			4 // (게스트)
#define RACE_ACC_TYPE_COMBO_DEMO			5 // (데모)


BOOL CAccountInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-02 by cmkwon, AdminTool에서 계정 권한 설정을 번역 한것을 사용함.
	m_ComboAccountType.AddString(STRCMD_CS_COMMON_RACE_NORMAL);
	m_ComboAccountType.AddString(STRCMD_CS_COMMON_RACE_OPERATION);
	m_ComboAccountType.AddString(STRCMD_CS_COMMON_RACE_GAMEMASTER);
	m_ComboAccountType.AddString(STRCMD_CS_COMMON_RACE_MONITOR);
	m_ComboAccountType.AddString(STRCMD_CS_COMMON_RACE_GUEST);
	m_ComboAccountType.AddString(STRCMD_CS_COMMON_RACE_DEMO);


	// 2005-02-13 by cmkwon, m_szAccountName SQL Query 형태로 변경되어져 있음
	// 2007-09-13 by cmkwon, 베트남 2차패스워드 시스템 구현 - SCAdminTool 수정
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("select AccountName, Password, AccountType,\
// 					Sex, BirthYear, RegisteredDate, LastLoginDate,\
// 					IsBlocked, ChattingBlocked, SecondaryPassword\
// 					from td_Account WITH (NOLOCK) where accountname = \'%s\'", m_szAccountName);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0001, m_szAccountName);
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, (LPSTR)(LPCSTR)m_szAccountName, 0, &arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0001));
	if (!bRet)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0000);
		m_pODBCStmt->FreeStatement();
		OnCancel();
		return TRUE;
	}

	// 2007-09-13 by cmkwon, 베트남 2차패스워드 시스템 구현 - SCAdminTool 수정
	SQLINTEGER arrCB[11] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
							SQL_NTS};

	char szAccountName[SIZE_MAX_ACCOUNT_NAME];
	char szPassword[SIZE_MAX_PASSWORD_MD5_STRING]; memset(szPassword, 0, SIZE_MAX_PASSWORD_MD5_STRING);
	BYTE nSex;
	INT BirthYear;
	SQL_TIMESTAMP_STRUCT RegDate, LastLoginDate;
	memset(&RegDate, 0, sizeof(SQL_TIMESTAMP_STRUCT));
	memset(&LastLoginDate, 0, sizeof(SQL_TIMESTAMP_STRUCT));
	BYTE nIsBlocked, nChattingBlocked;
	ATUM_DATE_TIME tmpDateTime;
	char szSecPassword[SIZE_MAX_PASSWORD_MD5_STRING]; util::zero(szSecPassword, SIZE_MAX_PASSWORD_MD5_STRING);	// 2007-09-13 by cmkwon, 베트남 2차패스워드 시스템 구현 - SCAdminTool 수정

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccountName, SIZE_MAX_ACCOUNT_NAME,			&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szPassword, SIZE_MAX_PASSWORD_MD5_STRING,		&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_USHORT, &m_nAcountType, 0,							&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &nSex, 0,									&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &BirthYear, 0,									&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_TIMESTAMP, &RegDate, 0,								&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_TIMESTAMP, &LastLoginDate, 0,							&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_TINYINT, &nIsBlocked, 0,								&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_TINYINT, &nChattingBlocked, 0,						&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_CHAR, szSecPassword, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[10]);	// 2007-09-13 by cmkwon, 베트남 2차패스워드 시스템 구현 - SCAdminTool 수정, 가져오기 필드 추가

	if (SQL_NO_DATA == SQLFetch(m_pODBCStmt->GetSTMTHandle()))
	{
		MessageBox(STRERR_S_SCADMINTOOL_0000);
		m_pODBCStmt->FreeStatement();
		OnCancel();
		return TRUE;
	}

	// free statement
	m_pODBCStmt->FreeStatement();

	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	m_nOldAcountType = m_nAcountType & RACE_ACCOUNT_TYPE_MASK;
	m_bChangeAccountType = FALSE;
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

	m_szPassword = szPassword;
	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-13 by cmkwon, 권한 및 종족
	if (m_nAcountType == 0) m_ComboAccountType.SetCurSel(RACE_ACC_TYPE_COMBO_NO_TYPE);
	else if (COMPARE_RACE(m_nAcountType, RACE_OPERATION)) m_ComboAccountType.SetCurSel(RACE_ACC_TYPE_COMBO_OPERATION);
	else if (COMPARE_RACE(m_nAcountType, RACE_GAMEMASTER)) m_ComboAccountType.SetCurSel(RACE_ACC_TYPE_COMBO_GAMEMASTER);
	else if (COMPARE_RACE(m_nAcountType, RACE_MONITOR)) m_ComboAccountType.SetCurSel(RACE_ACC_TYPE_COMBO_MONITOR);
	else if (COMPARE_RACE(m_nAcountType, RACE_GUEST)) m_ComboAccountType.SetCurSel(RACE_ACC_TYPE_COMBO_GUEST);
	else if (COMPARE_RACE(m_nAcountType, RACE_DEMO)) m_ComboAccountType.SetCurSel(RACE_ACC_TYPE_COMBO_DEMO);

	m_szSex = (nSex==1?"Man":"Woman");
	tmpDateTime.SetCurrentDateTime();
	m_szBirthYear.Format("Year %d, %d Year old", BirthYear, tmpDateTime.Year-BirthYear+1);
	tmpDateTime = RegDate;
	m_szRegisterdDate = tmpDateTime.GetDateTimeString().GetBuffer();
	tmpDateTime = LastLoginDate;
	m_szLastLoginDate = tmpDateTime.GetDateTimeString().GetBuffer();
	m_bAccountBlocked = nIsBlocked;
	m_bChattingBlocked = nChattingBlocked;
	m_ctlcsSecPassword	= szSecPassword;		// 2007-09-13 by cmkwon, 베트남 2차패스워드 시스템 구현 - SCAdminTool 수정

	UpdateData(FALSE);

	if(FALSE == m_bEnableEdit)
	{// 2006-04-15 by cmkwon, 수정 권한 처리
		GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_BUTTON_MD5_PWD)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_COMBO_RACE_ACC_TYPE2)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_SECPASSWORD)->EnableWindow(m_bEnableEdit);			// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 수정 권한이 없을때 비활성화
		GetDlgItem(IDC_BUTTON_MD5_SECPWD)->EnableWindow(m_bEnableEdit);			// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 수정 권한이 없을때 비활성화
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccountInfoDlg::OnOk() 
{
	if(FALSE == m_bEnableEdit)
	{// 2006-04-15 by cmkwon, 수정 없음
		CDialog::OnCancel();
		return;
	}

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 수정 도중 브럭정보가 변경 될 수 있다, 그래서 한번더 체크
	CSCUserAdminDlg *pSCUserAdminDlg = (CSCUserAdminDlg *)this->GetParent();
	if(FALSE == pSCUserAdminDlg->IsEnabledEdit())
	{
		AfxMessageBox("Now, you can't update AccountInfo !! Retry");
		CDialog::OnCancel();
		return;
	}

	// update data
	UpdateData();

	// Account Type 설정
	if (m_ComboAccountType.GetCurSel() == RACE_ACC_TYPE_COMBO_NO_TYPE) m_nAcountType = 0;
	else if (m_ComboAccountType.GetCurSel() == RACE_ACC_TYPE_COMBO_OPERATION) m_nAcountType = RACE_OPERATION;
	else if (m_ComboAccountType.GetCurSel() == RACE_ACC_TYPE_COMBO_GAMEMASTER) m_nAcountType = RACE_GAMEMASTER;
	else if (m_ComboAccountType.GetCurSel() == RACE_ACC_TYPE_COMBO_MONITOR) m_nAcountType = RACE_MONITOR;
	else if (m_ComboAccountType.GetCurSel() == RACE_ACC_TYPE_COMBO_GUEST) m_nAcountType = RACE_GUEST;
	else if (m_ComboAccountType.GetCurSel() == RACE_ACC_TYPE_COMBO_DEMO) m_nAcountType = RACE_DEMO;

	CString szQuery;
	// 2007-09-13 by cmkwon, 베트남 2차패스워드 시스템 구현 - SCAdminTool 수정, Update를 한다
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("update td_account set AccountType = %d, Password = \'%s\', SecondaryPassword = \'%s\' where accountname = \'%s\'",

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0002, m_nAcountType, m_szPassword, m_ctlcsSecPassword, m_szAccountName);
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, (LPSTR)(LPCSTR)m_szAccountName, 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &m_nAcountType, 0,											&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, (LPSTR)(LPCSTR)m_szPassword, 0,		&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, (LPSTR)(LPCSTR)m_ctlcsSecPassword, 0,	&arrCB2[4]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0002));
	if (!bRet)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0001);
	}
	// 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김
	else
	{
		if( m_nOldAcountType != (m_nAcountType & RACE_ACCOUNT_TYPE_MASK) )
		{
		m_bChangeAccountType = TRUE;		
		}
	}
	// end 2012-06-07 by hskim, 관리자 계정 보안 강화 - 계정 권한 변경시 로그 남김

	CDialog::OnOK();
}

void CAccountInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CAccountInfoDlg::OnButtonMd5Pwd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_szPassword.IsEmpty())
	{
		AfxMessageBox("Input Password !!");
		return;
	}

	MD5 MD5_instance;
	unsigned char md5_string[16];
	char szEncodedString[33];

	MD5_instance.MD5Encode((char*)(LPCSTR)m_szPassword, md5_string);
	MD5_instance.MD5Binary2String(md5_string, szEncodedString);

	// MD5 암호 설정
	m_szPassword = szEncodedString;

	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAccountInfoDlg::OnButtonMd5Secpwd() 
/// \brief		
/// \author		cmkwon
/// \date		2007-09-13 ~ 2007-09-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAccountInfoDlg::OnButtonMd5Secpwd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_ctlcsSecPassword.IsEmpty())
	{
		AfxMessageBox("Input SecondaryPassword !!");
		return;
	}

	MD5 MD5_instance;
	unsigned char md5_string[16];
	char szEncodedString[33];

	MD5_instance.MD5Encode((char*)(LPCSTR)m_ctlcsSecPassword, md5_string);
	MD5_instance.MD5Binary2String(md5_string, szEncodedString);

	// MD5 암호 설정
	m_ctlcsSecPassword = szEncodedString;

	UpdateData(FALSE);
	
}
