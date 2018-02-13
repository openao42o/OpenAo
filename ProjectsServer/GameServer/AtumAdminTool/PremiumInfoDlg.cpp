// PremiumInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "PremiumInfoDlg.h"
#include "ODBCStatement.h"
#include "SCUserAdminDlg.h"		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 -

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPremiumInfoDlg dialog


CPremiumInfoDlg::CPremiumInfoDlg(BOOL i_bEnableEdit, CString i_strAccName, UID32_t i_u32AccUID, CODBCStatement *i_pODBC
								 , CWnd* pParent /*=NULL*/)
	: CDialog(CPremiumInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPremiumInfoDlg)
	m_ctl_strAccountName	= i_strAccName;
	m_ctimeCreatedDate = CTime::GetCurrentTime();
	m_ctimeCreatedTime = CTime::GetCurrentTime();
	m_ctimeExpireDate = (CTime::GetCurrentTime() + CTimeSpan(30, 0 , 0, 0));
	m_ctimeExpireTime = (CTime::GetCurrentTime() + CTimeSpan(30, 0 , 0, 0));
	m_ctimeUpdatedDate = CTime::GetCurrentTime();
	m_ctimeUpdatedTime = CTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_uidAccountUID			= i_u32AccUID;
	m_pODBCStmt				= i_pODBC;
	util::zero(&m_premiumInfo, sizeof(SPREMIUM_INFO));

	m_bEnableEdit			= i_bEnableEdit;			// 2006-04-15 by cmkwon
}


void CPremiumInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPremiumInfoDlg)
	DDX_Control(pDX, IDC_COMBO_PREMIUM_ITEM, m_ctlComboBoxPremiumItem);
	DDX_Control(pDX, IDC_BTN_DELETE, m_ctlBtnDelete);
	DDX_Control(pDX, IDC_BTN_UPDATE, m_ctlBtnUpdate);
	DDX_Control(pDX, IDC_BTN_INSERT, m_ctlBtnInsert);
	DDX_Text(pDX, IDC_EDIT_ACCOUNTNAME, m_ctl_strAccountName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_CREATED_DATE, m_ctimeCreatedDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_CREATED_TIME, m_ctimeCreatedTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EXPIRE_DATE, m_ctimeExpireDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EXPIRE_TIME, m_ctimeExpireTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_UPDATED_DATE, m_ctimeUpdatedDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_UPDATED_TIME, m_ctimeUpdatedTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPremiumInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CPremiumInfoDlg)
	ON_BN_CLICKED(IDC_BTN_RELOAD, OnBtnReload)
	ON_BN_CLICKED(IDC_BTN_INSERT, OnBtnInsert)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPremiumInfoDlg message handlers

BOOL CPremiumInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctlComboBoxPremiumItem.AddString(STR_NORMAL_PREMIUM_CARD);
	m_ctlComboBoxPremiumItem.AddString(STR_SUPER_PREMIUM_CARD);
	m_ctlComboBoxPremiumItem.AddString(STR_UNKNOWN_PREMIUM_CARD);
	
	if(FALSE == LoadPremiumInfoBYAccountUID(&m_premiumInfo, m_uidAccountUID))
	{
		OnCancel();
		return FALSE;
	}

	if(FALSE == m_bEnableEdit)
	{
		GetDlgItem(IDC_COMBO_PREMIUM_ITEM)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_DATETIMEPICKER_EXPIRE_DATE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_DATETIMEPICKER_EXPIRE_TIME)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_BTN_INSERT)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(m_bEnableEdit);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPremiumInfoDlg::LoadPremiumInfoBYAccountUID(SPREMIUM_INFO *o_pPremiumInfo, UID32_t i_accUID)
{
	if(0 == i_accUID){						return FALSE;}

	m_ctlComboBoxPremiumItem.SetWindowText("");

	CString szQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("select UniqueNumber, ItemNum, CreatedTime,\
	//				UpdatedTime, ExpireTime\
	//				from td_AccountCashStore WITH (NOLOCK) where AccountUniqueNumber = %d", i_accUID);	

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0014, i_accUID);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_accUID, 0,		&arrCB2[1]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0014));
	if (FALSE == bRet)
	{
		MessageBox("Error to load premium information !!");
		m_pODBCStmt->FreeStatement();
		return FALSE;
	}

	util::zero(o_pPremiumInfo, sizeof(SPREMIUM_INFO));
	SQLINTEGER arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQL_TIMESTAMP_STRUCT		arrSqlTime[3];
	util::zero(arrSqlTime, sizeof(arrSqlTime[0])*3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_SBIGINT, &o_pPremiumInfo->u64PremiumUID, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_LONG, &o_pPremiumInfo->nItemNum, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &arrSqlTime[0], 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TIMESTAMP, &arrSqlTime[1], 0, &arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TIMESTAMP, &arrSqlTime[2], 0, &arrCB[5]);
	SQLFetch(m_pODBCStmt->GetSTMTHandle());
	m_pODBCStmt->FreeStatement();				// free statement

	if(o_pPremiumInfo->u64PremiumUID)
	{
		o_pPremiumInfo->atimeCreated	= arrSqlTime[0];
		o_pPremiumInfo->atimeUpdated	= arrSqlTime[1];
		o_pPremiumInfo->atimeExpire		= arrSqlTime[2];
	}

	int nComboIdx = m_ctlComboBoxPremiumItem.FindString(0, GetStringPremiumItemNameByItemNum(o_pPremiumInfo->nItemNum));
	m_ctlComboBoxPremiumItem.SetCurSel(nComboIdx);

	if(0 != o_pPremiumInfo->u64PremiumUID)
	{
		m_ctimeCreatedDate	= CTime(o_pPremiumInfo->atimeCreated.GetTimeInSeconds());
		m_ctimeCreatedTime	= CTime(o_pPremiumInfo->atimeCreated.GetTimeInSeconds());
		m_ctimeUpdatedDate	= CTime(o_pPremiumInfo->atimeUpdated.GetTimeInSeconds());
		m_ctimeUpdatedTime	= CTime(o_pPremiumInfo->atimeUpdated.GetTimeInSeconds());
		m_ctimeExpireDate	= CTime(o_pPremiumInfo->atimeExpire.GetTimeInSeconds());
		m_ctimeExpireTime	= CTime(o_pPremiumInfo->atimeExpire.GetTimeInSeconds());
	}
	if(m_bEnableEdit)
	{// 2006-04-17 by cmkwon
		if(0 == o_pPremiumInfo->u64PremiumUID)
		{
			m_ctlBtnInsert.EnableWindow(TRUE);
			m_ctlBtnUpdate.EnableWindow(FALSE);
			m_ctlBtnDelete.EnableWindow(FALSE);
		}
		else
		{
			m_ctlBtnInsert.EnableWindow(FALSE);
			m_ctlBtnUpdate.EnableWindow(TRUE);
			m_ctlBtnDelete.EnableWindow(TRUE);
		}
	}
	UpdateData(FALSE);
	return TRUE;
}

void CPremiumInfoDlg::OnBtnReload() 
{
	// TODO: Add your control notification handler code here
	LoadPremiumInfoBYAccountUID(&m_premiumInfo, m_uidAccountUID);
}

char *CPremiumInfoDlg::GetStringPremiumItemNameByItemNum(INT i_nItemNum)
{
// 2006-07-05 by cmkwon, MonthlyPay 시스템으로 수정
// 	switch(i_nItemNum)
// 	{
// 	case ITEM_NUM_CASH_NORMAL_PREMIUM_CARD:		return STR_NORMAL_PREMIUM_CARD;
// 	case ITEM_NUM_CASH_SUPER_PREMIUM_CARD:		return STR_SUPER_PREMIUM_CARD;
// 	}
	return STR_NORMAL_PREMIUM_CARD;
}

INT CPremiumInfoDlg::GetItemNumByPremiumItemName(char *i_szPremiumItem)
{
// 2006-07-05 by cmkwon, MonthlyPay 시스템으로 수정
// 	if(0 == strcmp(i_szPremiumItem, STR_NORMAL_PREMIUM_CARD))
// 	{
// 		return ITEM_NUM_CASH_NORMAL_PREMIUM_CARD;
// 	}
// 
// 	if(0 == strcmp(i_szPremiumItem, STR_SUPER_PREMIUM_CARD))
// 	{
// 		return ITEM_NUM_CASH_SUPER_PREMIUM_CARD;
// 	}

	return ITEM_NUM_CASH_NORMAL_PREMIUM_CARD;
}

void CPremiumInfoDlg::OnBtnInsert() 
{
	// TODO: Add your control notification handler code here

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 다시 할당 해준다
	m_bEnableEdit = ((CSCUserAdminDlg*)this->GetParent())->IsEnabledEdit();

	if(FALSE == m_bEnableEdit)
	{// 2006-04-15 by cmkwon, 수정 권한 체크
		MessageBox("Permission denied !!");
		return;
	}

	UpdateData();
	
	CString strPremiumItem;
	m_ctlComboBoxPremiumItem.GetWindowText(strPremiumItem);
	if(strPremiumItem.IsEmpty())
	{
		MessageBox("Select Premium Item !!");
		return;
	}
	SPREMIUM_INFO tmPreInfo;
	util::zero(&tmPreInfo, sizeof(SPREMIUM_INFO));
	tmPreInfo.nItemNum = GetItemNumByPremiumItemName((LPSTR)(LPCSTR)strPremiumItem);
	if(0 == tmPreInfo.nItemNum)
	{
		MessageBox("Select valid Premium Item !!");
		return;
	}

	tmPreInfo.atimeCreated.SetCurrentDateTime();
	tmPreInfo.atimeUpdated.SetCurrentDateTime();
	tmPreInfo.atimeExpire.SetDateTime(m_ctimeExpireDate.GetYear(), m_ctimeExpireDate.GetMonth(), m_ctimeExpireDate.GetDay()
		, m_ctimeExpireTime.GetHour(), m_ctimeExpireTime.GetMinute(), m_ctimeExpireTime.GetSecond());
	if(tmPreInfo.atimeExpire <= tmPreInfo.atimeCreated)
	{
		MessageBox("It is mistake during Expire time.");
		return;
	}

	if(FALSE == util::in_range(MIN_SUPPORT_YEAR, tmPreInfo.atimeExpire.Year, MAX_SUPPORT_YEAR))
	{// 2008-06-27 by cmkwon, 날짜시간 관련 버그 수정(1970 ~ 2037) - td_AccountCashStore.ExpireTime 체크
		char szTemp[1024];
		sprintf(szTemp, "Invalid year !!(Support Year:%d~%d)", MIN_SUPPORT_YEAR, MAX_SUPPORT_YEAR);
		AfxMessageBox(szTemp);
		return;
	}

	char tmTimeBuf[3][SIZE_MAX_SQL_DATETIME_STRING];
	tmPreInfo.atimeCreated.GetSQLDateTimeString(tmTimeBuf[0], SIZE_MAX_SQL_DATETIME_STRING);
	tmPreInfo.atimeUpdated.GetSQLDateTimeString(tmTimeBuf[1], SIZE_MAX_SQL_DATETIME_STRING);
	tmPreInfo.atimeExpire.GetSQLDateTimeString(tmTimeBuf[2], SIZE_MAX_SQL_DATETIME_STRING);
	CString szQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("INSERT INTO td_accountCashStore\
	//				VALUES (%u, %d, \'%s\', \'%s\', \'%s\')"
	//				, m_uidAccountUID, tmPreInfo.nItemNum
	//				, tmTimeBuf[0], tmTimeBuf[1], tmTimeBuf[2]);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0015		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//						, m_uidAccountUID, tmPreInfo.nItemNum
//						, tmTimeBuf[0], tmTimeBuf[1], tmTimeBuf[2]);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[6] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_uidAccountUID, 0,						&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmPreInfo.nItemNum, 0,						&arrCB2[2]);	
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmTimeBuf[0], 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmTimeBuf[1], 0,	&arrCB2[4]);	
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmTimeBuf[2], 0,	&arrCB2[5]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0015));
	if (FALSE == bRet)
	{
		MessageBox("Error to Insert premium Item !!");
		m_pODBCStmt->FreeStatement();
		return;
	}
	SQLFetch(m_pODBCStmt->GetSTMTHandle());
	m_pODBCStmt->FreeStatement();				// free statement

	OnBtnReload();
}

void CPremiumInfoDlg::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 다시 할당 해준다
	m_bEnableEdit = ((CSCUserAdminDlg*)this->GetParent())->IsEnabledEdit();

	if(FALSE == m_bEnableEdit)
	{// 2006-04-15 by cmkwon, 수정 권한 체크
		MessageBox("Permission denied !!");
		return;
	}
	
	if(0 == m_premiumInfo.u64PremiumUID)
	{
		return;
	}

//	CString szQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("DELETE FROM td_accountCashStore \
	//					WHERE AccountUniqueNumber = %u", m_uidAccountUID);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0259, m_uidAccountUID);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_uidAccountUID, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0259));
	if (FALSE == bRet)
	{
		MessageBox("Error to Delete premium Item !!");
		m_pODBCStmt->FreeStatement();
		return;
	}
	SQLFetch(m_pODBCStmt->GetSTMTHandle());
	m_pODBCStmt->FreeStatement();				// free statement

	OnBtnReload();
}

void CPremiumInfoDlg::OnBtnUpdate() 
{
	// TODO: Add your control notification handler code here

	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 다시 할당 해준다
	m_bEnableEdit = ((CSCUserAdminDlg*)this->GetParent())->IsEnabledEdit();

	if(FALSE == m_bEnableEdit)
	{// 2006-04-15 by cmkwon, 수정 권한 체크
		MessageBox("Permission denied !!");
		return;
	}

	UpdateData();
	
	CString strPremiumItem;
	m_ctlComboBoxPremiumItem.GetWindowText(strPremiumItem);
	if(strPremiumItem.IsEmpty())
	{
		MessageBox("Select Premium Item !!");
		return;
	}
	SPREMIUM_INFO tmPreInfo;
	util::zero(&tmPreInfo, sizeof(SPREMIUM_INFO));
	tmPreInfo.nItemNum = GetItemNumByPremiumItemName((LPSTR)(LPCSTR)strPremiumItem);
	if(0 == tmPreInfo.nItemNum)
	{
		MessageBox("Select valid Premium Item !!");
		return;
	}

	tmPreInfo.atimeUpdated.SetCurrentDateTime();
	tmPreInfo.atimeExpire.SetDateTime(m_ctimeExpireDate.GetYear(), m_ctimeExpireDate.GetMonth(), m_ctimeExpireDate.GetDay()
		, m_ctimeExpireTime.GetHour(), m_ctimeExpireTime.GetMinute(), m_ctimeExpireTime.GetSecond());
// 2006-09-21 by cmkwon, 체크 필요 없음
// 	if(tmPreInfo.atimeExpire <= tmPreInfo.atimeUpdated)
// 	{
// 		MessageBox("It is mistake during Expire time.");
// 		return;
// 	}

	if(FALSE == util::in_range(MIN_SUPPORT_YEAR, tmPreInfo.atimeExpire.Year, MAX_SUPPORT_YEAR))
	{// 2008-06-27 by cmkwon, 날짜시간 관련 버그 수정(1970 ~ 2037) - td_AccountCashStore.ExpireTime 체크
		char szTemp[1024];
		sprintf(szTemp, "Invalid year !!(Support Year:%d~%d)", MIN_SUPPORT_YEAR, MAX_SUPPORT_YEAR);
		AfxMessageBox(szTemp);
		return;
	}

	char tmTimeBuf[2][SIZE_MAX_SQL_DATETIME_STRING];
	tmPreInfo.atimeUpdated.GetSQLDateTimeString(tmTimeBuf[0], SIZE_MAX_SQL_DATETIME_STRING);
	tmPreInfo.atimeExpire.GetSQLDateTimeString(tmTimeBuf[1], SIZE_MAX_SQL_DATETIME_STRING);
//	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("UPDATE td_accountCashStore \
// 						SET ItemNum = %d, UpdatedTime = \'%s\', ExpireTime = \'%s\' \
// 						WHERE AccountUniqueNumber = %u"
// 						, tmPreInfo.nItemNum, tmTimeBuf[0], tmTimeBuf[1]
// 						, m_uidAccountUID);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 	
//	szQuery.Format(QUERY_080702_0260, tmPreInfo.nItemNum, tmTimeBuf[0], tmTimeBuf[1], m_uidAccountUID);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmPreInfo.nItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmTimeBuf[0], 0, NULL);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmTimeBuf[1], 0, NULL);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_uidAccountUID, 0, NULL);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0260));
	if (FALSE == bRet)
	{
		MessageBox("Error to Update premium Item !!");
		m_pODBCStmt->FreeStatement();
		return;
	}
	m_pODBCStmt->FreeStatement();				// free statement

	OnBtnReload();
}
