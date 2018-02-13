// seteventmonsterdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "seteventmonsterdlg.h"
#include "eventmonstermanagementdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetEventMonsterDlg dialog


CSetEventMonsterDlg::CSetEventMonsterDlg(SEVENT_MONSTER *i_pEvMon, CWnd* pParent /*=NULL*/)
	: CDialog(CSetEventMonsterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetEventMonsterDlg)
	m_ctlnMapIndex = 0;
	m_ctlnReqMinLevel = 0;
	m_ctlnReqMaxLevel = 0;
	m_ctlbExceptObjMon = TRUE;
	m_ctlbExceptInflMon = TRUE;
	m_ctlbExceptNotAttackMon = TRUE;
	m_ctlnMonsterNum = 0;
	m_ctlnMonsterCount = 0;
	m_ctlnDelayTime = 0;
	m_ctlnProbability = 0;
	//}}AFX_DATA_INIT
	m_odtStartDate	= COleDateTime::GetCurrentTime();
	m_odtStartTime	= COleDateTime::GetCurrentTime();
	m_odtEndDate	= COleDateTime::GetCurrentTime() + COleDateTimeSpan(1,0,0,0);
	m_odtEndTime	= COleDateTime::GetCurrentTime();

	m_pEventMonser		= i_pEvMon;
}


void CSetEventMonsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetEventMonsterDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER_GROUP, m_comboServerGroupID);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_odtStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_odtStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_odtEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_odtEndTime);
	DDX_Text(pDX, IDC_EDIT_MAPINDEX, m_ctlnMapIndex);
	DDX_Text(pDX, IDC_EDIT_REQ_MIN_LEVEL, m_ctlnReqMinLevel);
	DDX_Text(pDX, IDC_EDIT_REQ_MAX_LEVEL, m_ctlnReqMaxLevel);
	DDX_Check(pDX, IDC_CHECK_EXCEPT_OBJ_MON, m_ctlbExceptObjMon);
	DDX_Check(pDX, IDC_CHECK_EXCEPT_INFL_MON, m_ctlbExceptInflMon);
	DDX_Check(pDX, IDC_CHECK_EXCEPT_NOT_ATTACK_MON, m_ctlbExceptNotAttackMon);
	DDX_Text(pDX, IDC_EDIT_MONSTERNUM, m_ctlnMonsterNum);
	DDX_Text(pDX, IDC_EDIT_MONSTER_COUNT, m_ctlnMonsterCount);
	DDX_Text(pDX, IDC_EDIT_DELAY_TIME, m_ctlnDelayTime);
	DDX_Text(pDX, IDC_EDIT_PROBABILITY, m_ctlnProbability);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetEventMonsterDlg, CDialog)
	//{{AFX_MSG_MAP(CSetEventMonsterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetEventMonsterDlg message handlers

BOOL CSetEventMonsterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboServerGroupID.AddString(STR_ALL_SERVER_GROUP_STRING);
	for(int i=0; i < SIZE_MAX_SERVER_GROUP_COUNT; i++)
	{
		char szTemp[512]={0x00,};
		m_comboServerGroupID.AddString(itoa(SERVERGROUP_START_ID+i, szTemp, 10));
	}

	if(IS_VALID_UNIQUE_NUMBER(m_pEventMonser->EventMonsterUID))
	{
		m_odtStartDate.SetDate(m_pEventMonser->StartDateTime.Year, m_pEventMonser->StartDateTime.Month, m_pEventMonser->StartDateTime.Day);
		m_odtStartTime.SetTime(m_pEventMonser->StartDateTime.Hour, m_pEventMonser->StartDateTime.Minute, m_pEventMonser->StartDateTime.Second);
		m_odtEndDate.SetDate(m_pEventMonser->EndDateTime.Year, m_pEventMonser->EndDateTime.Month, m_pEventMonser->EndDateTime.Day);
		m_odtEndTime.SetTime(m_pEventMonser->EndDateTime.Hour, m_pEventMonser->EndDateTime.Minute, m_pEventMonser->EndDateTime.Second);
		m_ctlnMapIndex			= m_pEventMonser->SummonerMapIndex;
		m_ctlnReqMinLevel		= m_pEventMonser->SummonerReqMinLevel;
		m_ctlnReqMaxLevel		= m_pEventMonser->SummonerReqMaxLevel;
		m_ctlbExceptObjMon		= COMPARE_BIT_FLAG(m_pEventMonser->SummonerExceptMonster, EXCEPTMON_OBJECT_MONSTER_MASK) ? TRUE : FALSE;
		m_ctlbExceptInflMon		= COMPARE_BIT_FLAG(m_pEventMonser->SummonerExceptMonster, EXCEPTMON_INFLUENCE_TYPE_MASK) ? TRUE : FALSE;
		m_ctlbExceptNotAttackMon= COMPARE_BIT_FLAG(m_pEventMonser->SummonerExceptMonster, EXCEPTMON_BELL_NOTATTACK_MASK) ? TRUE : FALSE;
		m_ctlnMonsterNum		= m_pEventMonser->SummonMonsterNum;
		m_ctlnMonsterCount		= m_pEventMonser->SummonMonsterCount;
		m_ctlnDelayTime			= m_pEventMonser->SummonDelayTime;
		m_ctlnProbability		= m_pEventMonser->SummonProbability;		
	}

	int nServerGroupIdx		= m_pEventMonser->ServerGroupID;
	if(0 != nServerGroupIdx)
	{
		nServerGroupIdx		-= SERVERGROUP_START_ID - 1;
	}	
	m_comboServerGroupID.SetCurSel(nServerGroupIdx);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetEventMonsterDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	ATUM_DATE_TIME atStart, atEnd;

	atStart.SetDateTime(m_odtStartDate.GetYear(), m_odtStartDate.GetMonth(), m_odtStartDate.GetDay(), m_odtStartTime.GetHour(), m_odtStartTime.GetMinute(), 0);
	atEnd.SetDateTime(m_odtEndDate.GetYear(), m_odtEndDate.GetMonth(), m_odtEndDate.GetDay(), m_odtEndTime.GetHour(), m_odtEndTime.GetMinute(), 0);
	if(atStart >= atEnd)
	{
		AfxMessageBox("Invalid period !!");
		return;
	}

	if(0 != m_ctlnReqMinLevel
		&& 0 != m_ctlnReqMaxLevel
		&& m_ctlnReqMinLevel > m_ctlnReqMaxLevel)
	{
		AfxMessageBox("Invalid range level !!");
		return;
	}

	CEventMonsterManagementDlg *pEvMonManagementDlg = (CEventMonsterManagementDlg*)(this->GetParent());
	if( 0 == m_ctlnMonsterNum
		|| NULL == CAtumDBHelper::FindMEX_MONSTER_INFO(&(pEvMonManagementDlg->m_vecMexMonsterInfo), m_ctlnMonsterNum) )
	{
		char szTemp[1024];
		sprintf(szTemp, "Invalid MonsterNum(%d) !!", m_ctlnMonsterNum);
		AfxMessageBox(szTemp);
		return;
	}


	if(FALSE == util::in_range(1, m_ctlnMonsterCount, 100))
	{
		AfxMessageBox("Invalid MonsterCount !!");
		return;
	}

	if(FALSE == util::in_range(1, m_ctlnDelayTime, MAX_DELAY_TIME_AT_EVENT_MONSTER))
	{
		char szTemp[1024];
		sprintf(szTemp, "Invalid DelayTime !!(1 ~ %d)", MAX_DELAY_TIME_AT_EVENT_MONSTER);
		AfxMessageBox(szTemp);
		return;
	}

	if (!util::in_range(0, m_ctlnProbability, 10000))
	{
		AfxMessageBox("Invalid Probability !!(0 ~ 10000)");
		return;
	}

	ExceptMon_t	tmExMon = 0;
	tmExMon	= (m_ctlbExceptObjMon) ? (tmExMon|EXCEPTMON_OBJECT_MONSTER_MASK) : tmExMon;
	tmExMon	= (m_ctlbExceptInflMon) ? (tmExMon|EXCEPTMON_INFLUENCE_TYPE_MASK) : tmExMon;
	tmExMon	= (m_ctlbExceptNotAttackMon) ? (tmExMon|EXCEPTMON_BELL_NOTATTACK_MASK) : tmExMon;

	m_pEventMonser->ServerGroupID			= m_comboServerGroupID.GetCurSel();
	if(0 != m_pEventMonser->ServerGroupID)
	{// 2008-04-28 by cmkwon, 버그 수정 
		m_pEventMonser->ServerGroupID		= SERVERGROUP_START_ID + m_pEventMonser->ServerGroupID - 1;
	}
	m_pEventMonser->StartDateTime			= atStart;
	m_pEventMonser->EndDateTime				= atEnd;
	m_pEventMonser->SummonerMapIndex		= m_ctlnMapIndex;
	m_pEventMonser->SummonerReqMinLevel		= m_ctlnReqMinLevel;
	m_pEventMonser->SummonerReqMaxLevel		= m_ctlnReqMaxLevel;
	m_pEventMonser->SummonerExceptMonster	= tmExMon;
	m_pEventMonser->SummonMonsterNum		= m_ctlnMonsterNum;
	m_pEventMonser->SummonMonsterCount		= m_ctlnMonsterCount;
	m_pEventMonser->SummonDelayTime			= m_ctlnDelayTime;
	m_pEventMonser->SummonProbability		= min(m_ctlnProbability, 10000);

	CDialog::OnOK();
}
