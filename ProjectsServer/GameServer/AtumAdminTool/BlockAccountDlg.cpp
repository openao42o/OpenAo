// BlockAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "BlockAccountDlg.h"
#include "AtumParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockAccountDlg dialog


CBlockAccountDlg::CBlockAccountDlg(BOOL i_bBlocked, CString i_strAccName, SBLOCKED_ACCOUNT_INFO *i_pBlockedInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CBlockAccountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockAccountDlg)
	m_bBlocked			= i_bBlocked;
	m_szUserName		= i_strAccName;
	m_szBlockReason		= _T("");
	m_szBlockReasonForOnlyAdmin		= _T("");
	m_ctl_StartDate		= COleDateTime::GetCurrentTime();
	m_ctl_StartTime		= COleDateTime::GetCurrentTime();
	m_ctl_EndDate		= (COleDateTime::GetCurrentTime() + COleDateTimeSpan(1, 0 , 0, 0));
	m_ctl_EndTime		= COleDateTime::GetCurrentTime();
	m_ctl_strAdminName	= _T("scm_admin");	
	//}}AFX_DATA_INIT

	if(i_bBlocked)
	{
		m_nBlockType		= i_pBlockedInfo->enBlockedType;
		m_ctl_StartDate		= COleDateTime((long)i_pBlockedInfo->atimeStartTime.GetTimeInSeconds());
		m_ctl_StartTime		= COleDateTime((long)i_pBlockedInfo->atimeStartTime.GetTimeInSeconds());
		m_ctl_EndDate		= COleDateTime((long)i_pBlockedInfo->atimeEndTime.GetTimeInSeconds());
		m_ctl_EndTime		= COleDateTime((long)i_pBlockedInfo->atimeEndTime.GetTimeInSeconds());
		m_ctl_strAdminName	= i_pBlockedInfo->szBlockAdminAccountName;
		m_szBlockReason		= i_pBlockedInfo->szBlockedReasonForUser;
		m_szBlockReasonForOnlyAdmin		= i_pBlockedInfo->szBlockedReasonForOnlyAdmin;
	}
}


void CBlockAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockAccountDlg)
	DDX_Control(pDX, IDC_COMBO_BLOCK_TYPE, m_ComboBlockType);
	DDX_Text(pDX, IDC_EDIT_REASON, m_szBlockReason);
	DDX_Text(pDX, IDC_EDIT_REASON_FOR_ONLY_ADMIN, m_szBlockReasonForOnlyAdmin);
	DDX_Text(pDX, IDC_EDIT_USER_INPUT, m_szUserName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTDATE, m_ctl_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_STARTTIME, m_ctl_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDDATE, m_ctl_EndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_ENDTIME, m_ctl_EndTime);
	DDX_Text(pDX, IDC_EDIT_ADMIN_NAME, m_ctl_strAdminName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockAccountDlg, CDialog)
	//{{AFX_MSG_MAP(CBlockAccountDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockAccountDlg message handlers

BOOL CBlockAccountDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_NORMAL));
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_MONEY_RELATED));
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_ITEM_RELATED));
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_SPEEDHACK_RELATED));
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_CHAT_RELATED));
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_CHAT_GAMEBUG));
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK));		// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
	m_ComboBlockType.AddString(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK));		// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
	m_ComboBlockType.SetCurSel(0);

	if(m_bBlocked)
	{
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBlockAccountDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_szUserName.IsEmpty())
	{
		MessageBox(STRERR_S_SCADMINTOOL_0009);
		GetDlgItem(IDC_EDIT_USER_INPUT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_USER_INPUT))->SetSel(0, -1);
		return;
	}

	CString strBlockType;
	m_ComboBlockType.GetWindowText(strBlockType);
	if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_NORMAL)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_NORMAL;
	}
	else if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_MONEY_RELATED)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_MONEY_RELATED;
	}
	else if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_ITEM_RELATED)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_ITEM_RELATED;
	}
	else if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_SPEEDHACK_RELATED)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_SPEEDHACK_RELATED;
	}
	else if( strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_CHAT_RELATED)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_CHAT_RELATED;
	}
	else if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_CHAT_GAMEBUG)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_CHAT_GAMEBUG;
	}
	// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
	else if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_MEMORYHACK_AUTOBLOCK;
	}
	// end 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
	// 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
	else if(0 == strBlockType.Compare(GetStringBLOCKED_TYPE(T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK)) )
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_SPEEDHACK_AUTOBLOCK;
	}
	// end 2013-01-29 by hskim, 스피드핵 자동 블럭 기능 구현
	else 
	{
		m_nBlockType	= T_BLOCKEDACCOUNT_NORMAL;
	}
	
	
	m_atimeStart.SetDateTime(m_ctl_StartDate.GetYear(), m_ctl_StartDate.GetMonth(), m_ctl_StartDate.GetDay()
		, m_ctl_StartTime.GetHour(), m_ctl_StartTime.GetMinute(), m_ctl_StartTime.GetSecond());
	m_atimeEnd.SetDateTime(m_ctl_EndDate.GetYear(), m_ctl_EndDate.GetMonth(), m_ctl_EndDate.GetDay()
		, m_ctl_EndTime.GetHour(), m_ctl_EndTime.GetMinute(), m_ctl_EndTime.GetSecond());
	if(m_atimeStart >= m_atimeEnd)
	{
		MessageBox("Is period that is not valid !!");
		return;
	}	
	ATUM_DATE_TIME atCur(true);
	if(atCur >= m_atimeEnd)
	{
		MessageBox("Is time that end time is not valid !!");
		return;
	}

// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 
// 	m_ctl_strAdminName.TrimRight();
// 	if(m_ctl_strAdminName.IsEmpty())
// 	{
// 		MessageBox("Input the name of Administrator");
// 		GetDlgItem(IDC_EDIT_ADMIN_NAME)->SetFocus();
// 		((CEdit*)GetDlgItem(IDC_EDIT_ADMIN_NAME))->SetSel(0, -1);
// 		return;
// 	}

	m_szBlockReason.TrimRight();
	if (m_szBlockReason.IsEmpty())
	{
		MessageBox("Input reason for user !!");
		GetDlgItem(IDC_EDIT_REASON)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_REASON))->SetSel(0, -1);
		return;
	}

	// 2007-01-10 by cmkwon, 추가함
	m_szBlockReasonForOnlyAdmin.TrimRight();
	if (m_szBlockReasonForOnlyAdmin.IsEmpty())
	{
		MessageBox("Input reason for only Admin !!");
		GetDlgItem(IDC_EDIT_REASON_FOR_ONLY_ADMIN)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_REASON_FOR_ONLY_ADMIN))->SetSel(0, -1);
		return;
	}

	CDialog::OnOK();
}

void CBlockAccountDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
