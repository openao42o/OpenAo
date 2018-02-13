// SetCityWarTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atummonitor.h"
#include "SetCityWarTimeDlg.h"
#include "MonitorNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCityWarTimeDlg dialog


CSetCityWarTimeDlg::CSetCityWarTimeDlg(CChannelInfoNode * i_pChannelInfoNode, CWnd* pParent /*=NULL*/)
	: CDialog(CSetCityWarTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCityWarTimeDlg)
	m_ctlTime_DefaultDate = 0;
	m_ctlTime_SettingDate = 0;
	m_ctlTime_DefaultTime = 0;
	m_ctlTime_SettingTime = 0;
	m_ctlStr_CityWarMapIndex = _T("");
	//}}AFX_DATA_INIT

	m_ATimeDefaultTime		= i_pChannelInfoNode->m_cityWarOccupyInfo.CityWarDefaultTime;
	m_ATimeSettingTime		= i_pChannelInfoNode->m_cityWarOccupyInfo.CityWarSettingTime;

	m_ctlStr_CityWarMapIndex.Format("%d", i_pChannelInfoNode->m_projectInfo.m_nMapIndex);
	
	m_ctlTime_DefaultDate = CTime(m_ATimeDefaultTime.GetTimeInSeconds());
	m_ctlTime_DefaultTime = CTime(m_ATimeDefaultTime.GetTimeInSeconds());

	m_ctlTime_SettingDate = CTime(m_ATimeSettingTime.GetTimeInSeconds());
	m_ctlTime_SettingTime = CTime(m_ATimeSettingTime.GetTimeInSeconds());
}


void CSetCityWarTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCityWarTimeDlg)
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_DEFAULT, m_ctlTime_DefaultDate);
	DDX_DateTimeCtrl(pDX, IDC_DATEPICKER_SETTING, m_ctlTime_SettingDate);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_DEFAULT, m_ctlTime_DefaultTime);
	DDX_DateTimeCtrl(pDX, IDC_TIMEPICKER_SETTING, m_ctlTime_SettingTime);
	DDX_Text(pDX, IDC_EDIT_CITYWAR_MAPINDEX, m_ctlStr_CityWarMapIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCityWarTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CSetCityWarTimeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCityWarTimeDlg message handlers

void CSetCityWarTimeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	m_ATimeDefaultTime.Year		= m_ctlTime_DefaultDate.GetYear();
	m_ATimeDefaultTime.Month	= m_ctlTime_DefaultDate.GetMonth();
	m_ATimeDefaultTime.Day		= m_ctlTime_DefaultDate.GetDay();
	m_ATimeDefaultTime.Hour		= m_ctlTime_DefaultTime.GetHour();
	m_ATimeDefaultTime.Minute	= m_ctlTime_DefaultTime.GetMinute();

	m_ATimeSettingTime.Year		= m_ctlTime_SettingDate.GetYear();
	m_ATimeSettingTime.Month	= m_ctlTime_SettingDate.GetMonth();
	m_ATimeSettingTime.Day		= m_ctlTime_SettingDate.GetDay();
	m_ATimeSettingTime.Hour		= m_ctlTime_SettingTime.GetHour();
	m_ATimeSettingTime.Minute	= m_ctlTime_SettingTime.GetMinute();	
	CDialog::OnOK();
}
