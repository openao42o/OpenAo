// sethappyev.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "sethappyev.h"
#include "AtumSJ.h"				// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - stdafx.h 로 위치 이동
//#define STR_ALL_SERVER_GROUP_STRING			"All ServerGroup"	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -	

/////////////////////////////////////////////////////////////////////////////
// CSetHappyEv dialog


CSetHappyEv::CSetHappyEv(BOOL i_bInsertHappyEv, CWnd* pParent /*=NULL*/)
	: CDialog(CSetHappyEv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetHappyEv)
	m_timeEvEnd = COleDateTime::GetCurrentTime();
	m_timeEvStart = COleDateTime::GetCurrentTime();
	m_ctlfDropItemRate = 0.0f;
	m_ctlfDropRareItemRate = 0.0f;
	m_ctlfExpRate = 0.0f;
	m_ctlfExpRepairRate = 0.0f;
	m_ctlfSPIRate = 0.0f;
	m_ctlfWPRate = 0.0f;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	m_MinLevel = 1;
	m_MaxLevel = CHARACTER_LEVEL_110;	// 2010-03-11 by cmkwon, 지원레벨 110 상항 처리 빠진것 수정 - 
	//}}AFX_DATA_INIT

	m_bInsertHappyEv		= i_bInsertHappyEv;
	if(i_bInsertHappyEv)
	{
		m_happyEv.EventUniqueNumber		= 0;
		m_happyEv.ServerGroupID			= 0;
		m_happyEv.DayOfWeek				= 0;
		m_happyEv.atimeStartTime2.SetCurrentDateTime(TRUE);
		m_happyEv.atimeEndTime2.SetCurrentDateTime(TRUE);
		m_happyEv.InfluenceType3		= INFLUENCE_TYPE_ALL_MASK;	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
	}
}


void CSetHappyEv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetHappyEv)
	DDX_Control(pDX, IDC_COMBO_INFLUENCE_TYPE_LIST, m_comboInfluenceTypeList);
	DDX_Control(pDX, IDC_COMBO_SERVER_GROUP, m_comboServerGroupID);
	DDX_Control(pDX, IDC_COMBO_DETAIL_DAYOFWEEK, m_comboDayOfWeek);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DETAIL_ENDTIME, m_timeEvEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DETAIL_STARTTIME, m_timeEvStart);
	DDX_Text(pDX, IDC_EDIT_DROPITEMRATE, m_ctlfDropItemRate);
	DDX_Text(pDX, IDC_EDIT_DROPRAREITEMRATE, m_ctlfDropRareItemRate);
	DDX_Text(pDX, IDC_EDIT_EXPRATE, m_ctlfExpRate);
	DDX_Text(pDX, IDC_EDIT_EXPREPAIRRATE, m_ctlfExpRepairRate);
	DDX_Text(pDX, IDC_EDIT_SPIRATE, m_ctlfSPIRate);
	DDX_Text(pDX, IDC_EDIT_WPRATE, m_ctlfWPRate);			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	DDX_Text(pDX, IDC_EDIT_MINLEVEL, m_MinLevel);
	DDX_Text(pDX, IDC_EDIT_MAXLEVEL, m_MaxLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetHappyEv, CDialog)
	//{{AFX_MSG_MAP(CSetHappyEv)
	ON_BN_CLICKED(IDC_BTN_DEFAULT_VALUES, OnBtnDefaultValues)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetHappyEv message handlers
BOOL CSetHappyEv::OnInitDialog() 
{
	CDialog::OnInitDialog();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
	m_comboServerGroupID.AddString(STR_ALL_SERVER_GROUP_STRING);
	for(int i=0; i < SIZE_MAX_SERVER_GROUP_COUNT; i++)
	{
		char szTemp[512]={0x00,};
		m_comboServerGroupID.AddString(itoa(SERVERGROUP_START_ID+i, szTemp, 10));
	}

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 세력 콤보 박스 스트링 추가
	m_comboInfluenceTypeList.AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_NORMAL));
	m_comboInfluenceTypeList.AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_VCN));
	m_comboInfluenceTypeList.AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_ANI));
	m_comboInfluenceTypeList.AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_ALL_MASK));

	if(m_bInsertHappyEv)
	{
		OnBtnDefaultValues();
		m_happyEv.fDropItemRate2		= m_ctlfDropItemRate;
		m_happyEv.fDropRareItemRate2	= m_ctlfDropRareItemRate;
		m_happyEv.fEXPRate2				= m_ctlfExpRate;
		m_happyEv.fEXPRepairRate2		= m_ctlfExpRepairRate;
		m_happyEv.fSPIRate2				= m_ctlfSPIRate;
		m_happyEv.fWarPointRate2		= m_ctlfWPRate;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
		m_happyEv.MinLevel				= m_MinLevel;
		m_happyEv.MaxLevel				= m_MaxLevel;
	}

	int nServerGroupIdx		= m_happyEv.ServerGroupID;
	if(0 != nServerGroupIdx)
	{
		nServerGroupIdx		-= SERVERGROUP_START_ID - 1;
	}	
	m_comboServerGroupID.SetCurSel(nServerGroupIdx);
	m_comboDayOfWeek.SetCurSel(m_happyEv.DayOfWeek);
	m_ctlfDropItemRate		= m_happyEv.fDropItemRate2;
	m_ctlfDropRareItemRate	= m_happyEv.fDropRareItemRate2;
	m_ctlfExpRate			= m_happyEv.fEXPRate2;
	m_ctlfExpRepairRate		= m_happyEv.fEXPRepairRate2;
	m_ctlfSPIRate			= m_happyEv.fSPIRate2;
	m_ctlfWPRate			= m_happyEv.fWarPointRate2;			// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	m_MinLevel				= m_happyEv.MinLevel;
	m_MaxLevel				= m_happyEv.MaxLevel;

	m_timeEvStart.SetTime(m_happyEv.atimeStartTime2.Hour, m_happyEv.atimeStartTime2.Minute, 0);
	m_timeEvEnd.SetTime(m_happyEv.atimeEndTime2.Hour, m_happyEv.atimeEndTime2.Minute, 0);

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 세력 정보 설정함
	m_comboInfluenceTypeList.SetCurSel(max(0, GetArrayIndexByInfluenceType(m_happyEv.InfluenceType3)));
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetHappyEv::OnBtnDefaultValues() 
{
	// TODO: Add your control notification handler code here
	
	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 기본값을 0.0f 로 설정함
	m_ctlfDropItemRate		= 0.0f;
	m_ctlfDropRareItemRate	= 0.0f;
	m_ctlfExpRate			= 0.0f;
	m_ctlfExpRepairRate		= 0.0f;
	m_ctlfSPIRate			= 0.0f;
	m_ctlfWPRate			= 0.0f;				// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가

	UpdateData(FALSE);
}

void CSetHappyEv::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_timeEvStart >= m_timeEvEnd)
	{
		AfxMessageBox("Time error !!");
		return;
	}

	
	m_happyEv.ServerGroupID			= m_comboServerGroupID.GetCurSel();
	if(0 != m_happyEv.ServerGroupID)
	{
		m_happyEv.ServerGroupID		+= SERVERGROUP_START_ID - 1;
	}
	m_happyEv.DayOfWeek				= m_comboDayOfWeek.GetCurSel();
	m_happyEv.atimeStartTime2.SetDateTime(m_happyEv.atimeStartTime2.Year, m_happyEv.atimeStartTime2.Month, m_happyEv.atimeStartTime2.Day, m_timeEvStart.GetHour(), m_timeEvStart.GetMinute(), 0);
	m_happyEv.atimeEndTime2.SetDateTime(m_happyEv.atimeStartTime2.Year, m_happyEv.atimeStartTime2.Month, m_happyEv.atimeStartTime2.Day, m_timeEvEnd.GetHour(), m_timeEvEnd.GetMinute(), 0);
	m_happyEv.fDropItemRate2		= m_ctlfDropItemRate;
	m_happyEv.fDropRareItemRate2	= m_ctlfDropRareItemRate;
	m_happyEv.fEXPRate2				= m_ctlfExpRate;
	m_happyEv.fEXPRepairRate2		= m_ctlfExpRepairRate;
	m_happyEv.fSPIRate2				= m_ctlfSPIRate;
	m_happyEv.fWarPointRate2		= m_ctlfWPRate;					// 2007-06-26 by dhjin, 워포인트 이벤트 관련 추가
	m_happyEv.MinLevel				= m_MinLevel;
	m_happyEv.MaxLevel				= m_MaxLevel;

	// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 -
	m_happyEv.InfluenceType3		= GetInfluenceTypeByArrayIndex(m_comboInfluenceTypeList.GetCurSel());
	if(INFLUENCE_TYPE_UNKNOWN == m_happyEv.InfluenceType3)
	{
		AfxMessageBox("Influence Type error !!");
		return;
	}

	CDialog::OnOK();
}
