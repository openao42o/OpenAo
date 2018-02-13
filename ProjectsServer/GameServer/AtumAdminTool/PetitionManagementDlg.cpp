// PetitionManagementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "PetitionManagementDlg.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPetitionManagementDlg dialog


CPetitionManagementDlg::CPetitionManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPetitionManagementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPetitionManagementDlg)
	m_ctlodtStartDate = COleDateTime::GetCurrentTime();
	m_ctlodtStartTime = COleDateTime::GetCurrentTime();
	m_ctlodtEndDate = COleDateTime::GetCurrentTime();
	m_ctlodtEndTime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT

	m_ctlodtEndDate	+= COleDateTimeSpan(1, 0 , 0, 0);	// 2007-11-20 by cmkwon, 1ÀÏ ±âº»
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
}


void CPetitionManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPetitionManagementDlg)
	DDX_DateTimeCtrl(pDX, IDC_DTPICKER_START_DATE, m_ctlodtStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DTPICKER_START_TIME, m_ctlodtStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DTPICKER_END_DATE, m_ctlodtEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DTPICKER_END_TIME, m_ctlodtEndTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPetitionManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CPetitionManagementDlg)
	ON_BN_CLICKED(IDC_BTN_IMMEDIAT_ON, OnBtnImmediatOn)
	ON_BN_CLICKED(IDC_BTN_IMMEDIAT_OFF, OnBtnImmediatOff)
	ON_BN_CLICKED(IDC_BTN_PERIOD_SETTING, OnBtnPeriodSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPetitionManagementDlg message handlers

void CPetitionManagementDlg::OnBtnImmediatOn() 
{
	// TODO: Add your control notification handler code here
	
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_PETITION_SET_PERIOD, T_PA_ADMIN_PETITION_SET_PERIOD, pSMsg, SendBuf);
	pSMsg->byIsImmediatOn		= TRUE;
	if(FALSE == m_pMainDlg->SendMsgToPreServer(SendBuf, MSG_SIZE(MSG_PA_ADMIN_PETITION_SET_PERIOD)))
	{
		MessageBox("disconnected from PreServer !!");		
		return;
	}
}

void CPetitionManagementDlg::OnBtnImmediatOff() 
{
	// TODO: Add your control notification handler code here
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_PETITION_SET_PERIOD, T_PA_ADMIN_PETITION_SET_PERIOD, pSMsg, SendBuf);
	pSMsg->byIsImmediatOff		= TRUE;
	if(FALSE == m_pMainDlg->SendMsgToPreServer(SendBuf, MSG_SIZE(MSG_PA_ADMIN_PETITION_SET_PERIOD)))
	{
		MessageBox("disconnected from PreServer !!");		
		return;
	}
}

void CPetitionManagementDlg::OnBtnPeriodSetting() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_PETITION_SET_PERIOD, T_PA_ADMIN_PETITION_SET_PERIOD, pSMsg, SendBuf);	
	pSMsg->atStart.SetDateTime(m_ctlodtStartDate.GetYear(), m_ctlodtStartDate.GetMonth(), m_ctlodtStartDate.GetDay()
		, m_ctlodtStartTime.GetHour(), m_ctlodtStartTime.GetMinute(), 0);
	pSMsg->atEnd.SetDateTime(m_ctlodtEndDate.GetYear(), m_ctlodtEndDate.GetMonth(), m_ctlodtEndDate.GetDay()
		, m_ctlodtEndTime.GetHour(), m_ctlodtEndTime.GetMinute(), 0);

	if(FALSE == m_pMainDlg->SendMsgToPreServer(SendBuf, MSG_SIZE(MSG_PA_ADMIN_PETITION_SET_PERIOD)))
	{
		MessageBox("disconnected from PreServer !!");		
		return;
	}
}
