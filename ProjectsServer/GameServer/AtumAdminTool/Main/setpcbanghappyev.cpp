// setpcbanghappyev.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "setpcbanghappyev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetPCBangHappyEv dialog


CSetPCBangHappyEv::CSetPCBangHappyEv(SHAPPY_HOUR_EVENT *i_pHappyEv, CWnd* pParent /*=NULL*/)
	: CDialog(CSetPCBangHappyEv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetPCBangHappyEv)
	m_OleDatePCBangStart = COleDateTime::GetCurrentTime();
	m_OleTimePCBangStart = COleDateTime::GetCurrentTime();
	m_OleDatePCBangEnd = COleDateTime::GetCurrentTime();
	m_OleTimePCBangEnd = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT

	m_pcBangEv					= *i_pHappyEv;
	m_pcBangEv.DayOfWeek		= DAY_OF_WEEK_PCBANG_HAPPYHOUREVENT;	// 2006-08-28 by cmkwon, PCBangHappyHourEvent
	m_pcBangEv.ServerGroupID	= 0;		// 2006-08-28 by cmkwon, 모든 ServerGroup
	if(0 == m_pcBangEv.EventUniqueNumber)
	{
		m_pcBangEv.fEXPRate2			= 0.0f;
		m_pcBangEv.fSPIRate2			= 0.0f;
		m_pcBangEv.fEXPRepairRate2		= 0.0f;
		m_pcBangEv.fDropItemRate2		= 0.0f;
		m_pcBangEv.fDropRareItemRate2	= 0.0f;
		m_pcBangEv.fWarPointRate2		= 0.0f;	// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	}
	else
	{
		m_OleDatePCBangStart.SetDate(m_pcBangEv.atimeStartTime2.Year, m_pcBangEv.atimeStartTime2.Month, m_pcBangEv.atimeStartTime2.Day);
		m_OleTimePCBangStart.SetTime(m_pcBangEv.atimeStartTime2.Hour, m_pcBangEv.atimeStartTime2.Minute, 0);

		m_OleDatePCBangEnd.SetDate(m_pcBangEv.atimeEndTime2.Year, m_pcBangEv.atimeEndTime2.Month, m_pcBangEv.atimeEndTime2.Day);
		m_OleTimePCBangEnd.SetTime(m_pcBangEv.atimeEndTime2.Hour, m_pcBangEv.atimeEndTime2.Minute, 0);
	}
}


void CSetPCBangHappyEv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetPCBangHappyEv)
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_PCBANG_START, m_OleDatePCBangStart);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_PCBANG_START, m_OleTimePCBangStart);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_PCBANG_END, m_OleDatePCBangEnd);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_PCBANG_END, m_OleTimePCBangEnd);
	DDX_Text(pDX, IDC_EDIT_DROPITEMRATE, m_pcBangEv.fDropItemRate2);
	DDX_Text(pDX, IDC_EDIT_DROPRAREITEMRATE, m_pcBangEv.fDropRareItemRate2);
	DDX_Text(pDX, IDC_EDIT_EXPRATE, m_pcBangEv.fEXPRate2);
	DDX_Text(pDX, IDC_EDIT_EXPREPAIRRATE, m_pcBangEv.fEXPRepairRate2);
	DDX_Text(pDX, IDC_EDIT_SPIRATE, m_pcBangEv.fSPIRate2);
	DDX_Text(pDX, IDC_EDIT_WPRATE, m_pcBangEv.fWarPointRate2);				// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetPCBangHappyEv, CDialog)
	//{{AFX_MSG_MAP(CSetPCBangHappyEv)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_VALUES, OnBtnDefaultValues)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetPCBangHappyEv message handlers

BOOL CSetPCBangHappyEv::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetPCBangHappyEv::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	m_pcBangEv.atimeStartTime2.SetDateTime(m_OleDatePCBangStart.GetYear(), m_OleDatePCBangStart.GetMonth(), m_OleDatePCBangStart.GetDay(), m_OleTimePCBangStart.GetHour(), m_OleTimePCBangStart.GetMinute(), 0);
	m_pcBangEv.atimeEndTime2.SetDateTime(m_OleDatePCBangEnd.GetYear(), m_OleDatePCBangEnd.GetMonth(), m_OleDatePCBangEnd.GetDay(), m_OleTimePCBangEnd.GetHour(), m_OleTimePCBangEnd.GetMinute(), 0);

	if(0.0f > m_pcBangEv.fEXPRate2
		|| 0.0f > m_pcBangEv.fSPIRate2
		|| 0.0f > m_pcBangEv.fEXPRepairRate2
		|| 0.0f > m_pcBangEv.fDropItemRate2
		|| 0.0f > m_pcBangEv.fDropRareItemRate2
		|| 0.0f > m_pcBangEv.fWarPointRate2)		// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	{
		AfxMessageBox("Any Plus Rate value is invalid !!(more then 0.0f)");
		return;
	}
	if(0.0f >= m_pcBangEv.fEXPRate2
		&& 0.0f >= m_pcBangEv.fSPIRate2
		&& 0.0f >= m_pcBangEv.fEXPRepairRate2
		&& 0.0f >= m_pcBangEv.fDropItemRate2
		&& 0.0f >= m_pcBangEv.fDropRareItemRate2
		&& 0.0f >= m_pcBangEv.fWarPointRate2)		// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	{
		AfxMessageBox("All Plus rate is default values !!");
		return;
	}
		
	CDialog::OnOK();
}

void CSetPCBangHappyEv::OnBtnDefaultValues() 
{
	// TODO: Add your control notification handler code here
	
	m_pcBangEv.fEXPRate2			= 0.0f;
	m_pcBangEv.fSPIRate2			= 0.0f;
	m_pcBangEv.fEXPRepairRate2		= 0.0f;
	m_pcBangEv.fDropItemRate2		= 0.0f;
	m_pcBangEv.fDropRareItemRate2	= 0.0f;
	m_pcBangEv.fWarPointRate2		= 0.0f;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	UpdateData(FALSE);
}
