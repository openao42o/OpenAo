// LeaderElection.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "LeaderElection.h"
#include "ODBCStatement.h"
#include "AtumSJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeaderElection dialog


CLeaderElection::CLeaderElection(CODBCStatement *i_pODBCStat, CWnd* pParent /*=NULL*/)
	: CDialog(CLeaderElection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeaderElection)
	m_Election_State = _T("");
	m_TotalVoteOfNumber = 0;
	//}}AFX_DATA_INIT

	m_pODBCStmtLeaderElection	= i_pODBCStat;
	m_CheckAUTOUpdate			= 0;
}


void CLeaderElection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeaderElection)
	DDX_DateTimeCtrl(pDX, IDC_APPLICATION_DATESTART, m_ApplicationDateStart);
	DDX_DateTimeCtrl(pDX, IDC_APPLICATION_DATEEND, m_ApplicationDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_APPLICATION_TIMESTART, m_ApplicationTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_APPLICATION_TIMEEND, m_ApplicationTimeEnd);
	DDX_DateTimeCtrl(pDX, IDC_ELECTION_DATE, m_ElectionDate);
	DDX_DateTimeCtrl(pDX, IDC_ELECTION_TIME, m_ElectionTime);
	DDX_DateTimeCtrl(pDX, IDC_VOTE_DATEEND, m_VoteDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_VOTE_DATESTART, m_VoteDateStart);
	DDX_DateTimeCtrl(pDX, IDC_VOTE_TIMEEND, m_VoteTimeEnd);
	DDX_DateTimeCtrl(pDX, IDC_VOTE_TIMESTART, m_VoteTimeStart);
	DDX_Text(pDX, IDC_EDIT_ELECTION_STATE, m_Election_State);
	DDX_Text(pDX, IDC_EDIT_VOTE_NUMBER, m_TotalVoteOfNumber);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_VOTE_RESULT_LIST, m_GridVoteResultList);
}


BEGIN_MESSAGE_MAP(CLeaderElection, CDialog)
	//{{AFX_MSG_MAP(CLeaderElection)
	ON_BN_CLICKED(IDC_BTN_AUTO_EDIT, OnBtnAutoEdit)
	ON_BN_CLICKED(IDC_BTN_GM_EDIT, OnBtnGmEdit)
	ON_BN_CLICKED(IDC_BTN_RELOAD, OnBtnReload)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_RESULT, OnBtnResult)
	ON_BN_CLICKED(IDC_BTN_SAVE_RESULT, OnBtnSaveResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeaderElection message handlers

BOOL CLeaderElection::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-10-31 by dhjin, DB에서 선거 기간 얻어오기
	this->GetPollDateByDB();
	this->AllDisableWindowDate();
	this->InitGridVoteResultList();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLeaderElection::GetPollDateByDB()
/// \brief		선거 기간 정보 가져오기
/// \author		dhjin
/// \date		2007-10-31 ~ 2007-10-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLeaderElection::GetPollDateByDB()
{
	/************************************************************************
	CREATE PROCEDURE dbo.atum_LoadPollDate
	AS
		SELECT ApplicationStartDate, ApplicationEndDate, VoteStartDate, VoteEndDate, Election
		FROM atum2_db_account_test.dbo.td_PollDate
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(m_pODBCStmtLeaderElection->m_hstmt, PROCEDURE_080822_0013, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtLeaderElection->FreeStatement();
		return;
	}
	
	SPOLLDATE_DB		tmPOLLDATE_DB;
	util::zero(&tmPOLLDATE_DB, sizeof(SPOLLDATE_DB));
	
	SQL_TIMESTAMP_STRUCT	tmApplicationStartDate;
	SQL_TIMESTAMP_STRUCT	tmApplicationEndDate;
	SQL_TIMESTAMP_STRUCT	tmVoteStartDate;
	SQL_TIMESTAMP_STRUCT	tmVoteEndDate;
	SQL_TIMESTAMP_STRUCT	tmElection;

	SQLINTEGER	arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,    SQL_NTS};
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 1, SQL_C_TIMESTAMP, &tmApplicationStartDate, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 2, SQL_C_TIMESTAMP, &tmApplicationEndDate, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 3, SQL_C_TIMESTAMP, &tmVoteStartDate, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 4, SQL_C_TIMESTAMP, &tmVoteEndDate, 0, &arrCB[4]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 5, SQL_C_TIMESTAMP, &tmElection, 0, &arrCB[5]);
	
	if ( (ret = SQLFetch(m_pODBCStmtLeaderElection->m_hstmt)) != SQL_NO_DATA)
	{
		tmPOLLDATE_DB.ApplicationStartDate			= tmApplicationStartDate;
		tmPOLLDATE_DB.ApplicationEndDate			= tmApplicationEndDate;
		tmPOLLDATE_DB.VoteStartDate					= tmVoteStartDate;
		tmPOLLDATE_DB.VoteEndDate					= tmVoteEndDate;
		tmPOLLDATE_DB.Election						= tmElection;
	}
	m_pODBCStmtLeaderElection->FreeStatement();

	m_ApplicationDateStart.SetDate(tmPOLLDATE_DB.ApplicationStartDate.Year, tmPOLLDATE_DB.ApplicationStartDate.Month, tmPOLLDATE_DB.ApplicationStartDate.Day);
	m_ApplicationDateEnd.SetDate(tmPOLLDATE_DB.ApplicationEndDate.Year, tmPOLLDATE_DB.ApplicationEndDate.Month, tmPOLLDATE_DB.ApplicationEndDate.Day);
	m_ApplicationTimeStart.SetTime(tmPOLLDATE_DB.ApplicationStartDate.Hour, tmPOLLDATE_DB.ApplicationStartDate.Minute, tmPOLLDATE_DB.ApplicationStartDate.Second);
	m_ApplicationTimeEnd.SetTime(tmPOLLDATE_DB.ApplicationEndDate.Hour, tmPOLLDATE_DB.ApplicationEndDate.Minute, tmPOLLDATE_DB.ApplicationEndDate.Second);
	m_VoteDateStart.SetDate(tmPOLLDATE_DB.VoteStartDate.Year, tmPOLLDATE_DB.VoteStartDate.Month, tmPOLLDATE_DB.VoteStartDate.Day);
	m_VoteDateEnd.SetDate(tmPOLLDATE_DB.VoteEndDate.Year, tmPOLLDATE_DB.VoteEndDate.Month, tmPOLLDATE_DB.VoteEndDate.Day);
	m_VoteTimeStart.SetTime(tmPOLLDATE_DB.VoteStartDate.Hour, tmPOLLDATE_DB.VoteStartDate.Minute, tmPOLLDATE_DB.VoteStartDate.Second);
	m_VoteTimeEnd.SetTime(tmPOLLDATE_DB.VoteEndDate.Hour, tmPOLLDATE_DB.VoteEndDate.Minute, tmPOLLDATE_DB.VoteEndDate.Second);
	m_ElectionDate.SetDate(tmPOLLDATE_DB.Election.Year, tmPOLLDATE_DB.Election.Month, tmPOLLDATE_DB.Election.Day);
	m_ElectionTime.SetTime(tmPOLLDATE_DB.Election.Hour, tmPOLLDATE_DB.Election.Minute, tmPOLLDATE_DB.Election.Second);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLeaderElection::AllDisableWindowDate()
/// \brief		선거 기간 정보 전부 비활성화
/// \author		dhjin
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLeaderElection::AllDisableWindowDate()
{
	GetDlgItem(IDC_APPLICATION_DATESTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_APPLICATION_DATEEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_APPLICATION_TIMESTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_APPLICATION_TIMEEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_ELECTION_DATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ELECTION_TIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_VOTE_DATEEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_VOTE_DATESTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_VOTE_TIMEEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_VOTE_TIMESTART)->EnableWindow(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLeaderElection::AllEnableWindowDate()
/// \brief		선거 기간 정보 전부 활성화
/// \author		dhjin
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLeaderElection::AllEnableWindowDate()
{
	GetDlgItem(IDC_APPLICATION_DATESTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLICATION_DATEEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLICATION_TIMESTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLICATION_TIMEEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_ELECTION_DATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_ELECTION_TIME)->EnableWindow(TRUE);
	GetDlgItem(IDC_VOTE_DATEEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_VOTE_DATESTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_VOTE_TIMEEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_VOTE_TIMESTART)->EnableWindow(TRUE);
}

void CLeaderElection::OnBtnAutoEdit() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ELECTION_DATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_ELECTION_TIME)->EnableWindow(TRUE);
	
	m_CheckAUTOUpdate = TRUE;

}

void CLeaderElection::OnBtnGmEdit() 
{
	// TODO: Add your control notification handler code here
	this->AllEnableWindowDate();	

}

void CLeaderElection::OnBtnReload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// 2007-11-01 by dhjin, 입력된 값을 DB에 저장한다.
	/************************************************************************
	CREATE PROCEDURE dbo.atum_UpdatePollDate
		@i_ApplicationStartDate		DATETIME,
		@i_ApplicationEndDate		DATETIME,
		@i_VoteStartDate			DATETIME,
		@i_VoteEndDate				DATETIME,
		@i_Election					DATETIME
	AS
		UPDATE atum2_db_account_test.dbo.td_PollDate
		SET ApplicationStartDate = @i_ApplicationStartDate, ApplicationEndDate = @i_ApplicationEndDate
			, VoteStartDate = @i_VoteStartDate, VoteEndDate = @i_VoteEndDate, Election = @i_Election
	GO
	************************************************************************/
	SPOLLDATE_DB		tmPOLLDATE_DB;
	util::zero(&tmPOLLDATE_DB, sizeof(SPOLLDATE_DB));
	tmPOLLDATE_DB.ApplicationStartDate.SetDateTime(m_ApplicationDateStart.GetYear(), m_ApplicationDateStart.GetMonth(), m_ApplicationDateStart.GetDay()
													, m_ApplicationTimeStart.GetHour(), m_ApplicationTimeStart.GetMinute(), m_ApplicationTimeStart.GetSecond());
	tmPOLLDATE_DB.ApplicationEndDate.SetDateTime(m_ApplicationDateEnd.GetYear(), m_ApplicationDateEnd.GetMonth(), m_ApplicationDateEnd.GetDay()
												, m_ApplicationTimeEnd.GetHour(), m_ApplicationTimeEnd.GetMinute(), m_ApplicationTimeEnd.GetSecond());
	tmPOLLDATE_DB.VoteStartDate.SetDateTime(m_VoteDateStart.GetYear(), m_VoteDateStart.GetMonth(), m_VoteDateStart.GetDay()
											, m_VoteTimeStart.GetHour(), m_VoteTimeStart.GetMinute(), m_VoteTimeStart.GetSecond());
	tmPOLLDATE_DB.VoteEndDate.SetDateTime(m_VoteDateEnd.GetYear(), m_VoteDateEnd.GetMonth(), m_VoteDateEnd.GetDay()
											, m_VoteTimeEnd.GetHour(), m_VoteTimeEnd.GetMinute(), m_VoteTimeEnd.GetSecond());
	tmPOLLDATE_DB.Election.SetDateTime(m_ElectionDate.GetYear(), m_ElectionDate.GetMonth(), m_ElectionDate.GetDay()
										, m_ElectionTime.GetHour(), m_ElectionTime.GetMinute(), m_ElectionTime.GetSecond());
	
	//////////////////////////////////////////////////////////////////////////
	// 2007-11-01 by dhjin, 자동 업데이트
	if(m_CheckAUTOUpdate)
	{
		tmPOLLDATE_DB.ApplicationStartDate	= tmPOLLDATE_DB.Election;
		tmPOLLDATE_DB.ApplicationEndDate	= tmPOLLDATE_DB.Election;
		tmPOLLDATE_DB.VoteStartDate			= tmPOLLDATE_DB.Election;
		tmPOLLDATE_DB.VoteEndDate			= tmPOLLDATE_DB.Election;
		tmPOLLDATE_DB.ApplicationStartDate.AddDateTime(0,0,-6,0,0,0);
		tmPOLLDATE_DB.ApplicationStartDate.SetDateTime(tmPOLLDATE_DB.ApplicationStartDate.Year, tmPOLLDATE_DB.ApplicationStartDate.Month, tmPOLLDATE_DB.ApplicationStartDate.Day
														, 0, 0, 1);
		tmPOLLDATE_DB.ApplicationEndDate.AddDateTime(0,0,-4,0,0,0);
		tmPOLLDATE_DB.ApplicationEndDate.SetDateTime(tmPOLLDATE_DB.ApplicationEndDate.Year, tmPOLLDATE_DB.ApplicationEndDate.Month, tmPOLLDATE_DB.ApplicationEndDate.Day
														, 23, 59, 59);
		tmPOLLDATE_DB.VoteStartDate.AddDateTime(0,0,-3,0,0,0);
		tmPOLLDATE_DB.VoteStartDate.SetDateTime(tmPOLLDATE_DB.VoteStartDate.Year, tmPOLLDATE_DB.VoteStartDate.Month, tmPOLLDATE_DB.VoteStartDate.Day
														, 0, 0, 1);
		tmPOLLDATE_DB.VoteEndDate.AddDateTime(0,0,-1,0,0,0);
		tmPOLLDATE_DB.VoteEndDate.SetDateTime(tmPOLLDATE_DB.VoteEndDate.Year, tmPOLLDATE_DB.VoteEndDate.Month, tmPOLLDATE_DB.VoteEndDate.Day
														, 23, 59, 59);
		m_ApplicationDateStart.SetDate(tmPOLLDATE_DB.ApplicationStartDate.Year, tmPOLLDATE_DB.ApplicationStartDate.Month, tmPOLLDATE_DB.ApplicationStartDate.Day);
		m_ApplicationDateEnd.SetDate(tmPOLLDATE_DB.ApplicationEndDate.Year, tmPOLLDATE_DB.ApplicationEndDate.Month, tmPOLLDATE_DB.ApplicationEndDate.Day);
		m_ApplicationTimeStart.SetTime(tmPOLLDATE_DB.ApplicationStartDate.Hour, tmPOLLDATE_DB.ApplicationStartDate.Minute, tmPOLLDATE_DB.ApplicationStartDate.Second);
		m_ApplicationTimeEnd.SetTime(tmPOLLDATE_DB.ApplicationEndDate.Hour, tmPOLLDATE_DB.ApplicationEndDate.Minute, tmPOLLDATE_DB.ApplicationEndDate.Second);
		m_VoteDateStart.SetDate(tmPOLLDATE_DB.VoteStartDate.Year, tmPOLLDATE_DB.VoteStartDate.Month, tmPOLLDATE_DB.VoteStartDate.Day);
		m_VoteDateEnd.SetDate(tmPOLLDATE_DB.VoteEndDate.Year, tmPOLLDATE_DB.VoteEndDate.Month, tmPOLLDATE_DB.VoteEndDate.Day);
		m_VoteTimeStart.SetTime(tmPOLLDATE_DB.VoteStartDate.Hour, tmPOLLDATE_DB.VoteStartDate.Minute, tmPOLLDATE_DB.VoteStartDate.Second);
		m_VoteTimeEnd.SetTime(tmPOLLDATE_DB.VoteEndDate.Hour, tmPOLLDATE_DB.VoteEndDate.Minute, tmPOLLDATE_DB.VoteEndDate.Second);
		m_ElectionDate.SetDate(tmPOLLDATE_DB.Election.Year, tmPOLLDATE_DB.Election.Month, tmPOLLDATE_DB.Election.Day);
		m_ElectionTime.SetTime(tmPOLLDATE_DB.Election.Hour, tmPOLLDATE_DB.Election.Minute, tmPOLLDATE_DB.Election.Second);
	}

	char szApplicationStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szApplicationEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szVoteStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szVoteEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szElection[SIZE_MAX_SQL_DATETIME_STRING];
	tmPOLLDATE_DB.ApplicationStartDate.GetSQLDateTimeString(szApplicationStartDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.ApplicationEndDate.GetSQLDateTimeString(szApplicationEndDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.VoteStartDate.GetSQLDateTimeString(szVoteStartDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.VoteEndDate.GetSQLDateTimeString(szVoteEndDate, SIZE_MAX_SQL_DATETIME_STRING);
	tmPOLLDATE_DB.Election.GetSQLDateTimeString(szElection, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(m_pODBCStmtLeaderElection->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szApplicationStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmtLeaderElection->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szApplicationEndDate, 0, NULL);
	SQLBindParameter(m_pODBCStmtLeaderElection->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szVoteStartDate, 0, NULL);
	SQLBindParameter(m_pODBCStmtLeaderElection->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szVoteEndDate, 0, NULL);
	SQLBindParameter(m_pODBCStmtLeaderElection->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szElection, 0, NULL);

	RETCODE	ret = SQLExecDirect(m_pODBCStmtLeaderElection->m_hstmt, PROCEDURE_080822_0014, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtLeaderElection->FreeStatement();
		return;
	}
		

	this->AllDisableWindowDate();
	UpdateData(FALSE);
	m_CheckAUTOUpdate = FALSE;
}

void CLeaderElection::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CLeaderElection::OnBtnResult() 
{
	// TODO: Add your control notification handler code here
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_LoadLeaderCandidate
	-- DESC				: 지도자 후보 리스트 가져오는 프로시저
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_LoadLeaderCandidate
	AS
		SELECT LeaderCandidateNum, AccountUID, CharacterUID, CharacterName, GuildUID, GuildName, Influence, DeleteCandidate, PollCount, CampaignPromises
		FROM dbo.td_LeaderCandidate
	GO
	************************************************************************/
	
	RETCODE	ret = SQLExecDirect(m_pODBCStmtLeaderElection->m_hstmt, PROCEDURE_080822_0015, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtLeaderElection->FreeStatement();
		return;
	}
	
	SQLINTEGER	arrCB[11] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS,	SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS
							, SQL_NTS};

	SLEADER_CANDIDATE			tmSLEADER_CANDIDATE;
	util::zero(&tmSLEADER_CANDIDATE, sizeof(SLEADER_CANDIDATE));
	m_TotalVoteCount = 0;
	m_mtvectSLEADER_CANDIDATE.clear();

	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 1, SQL_C_ULONG, &tmSLEADER_CANDIDATE.LeaderCandidateNum, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 2, SQL_C_ULONG, &tmSLEADER_CANDIDATE.AccountUID, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 3, SQL_C_ULONG, &tmSLEADER_CANDIDATE.CharacterUID, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 4, SQL_C_CHAR, tmSLEADER_CANDIDATE.CharacterName, SIZE_MAX_CHARACTER_NAME, &arrCB[4]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 5, SQL_C_ULONG, &tmSLEADER_CANDIDATE.GuildUID, 0, &arrCB[5]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 6, SQL_C_CHAR, tmSLEADER_CANDIDATE.GuildName, SIZE_MAX_GUILD_NAME, &arrCB[6]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 7, SQL_C_TINYINT, &tmSLEADER_CANDIDATE.Influence, 0, &arrCB[7]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 8, SQL_C_TINYINT, &tmSLEADER_CANDIDATE.DeleteCandidate, 0, &arrCB[8]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 9, SQL_C_ULONG, &tmSLEADER_CANDIDATE.PollCount, 0, &arrCB[9]);
	SQLBindCol(m_pODBCStmtLeaderElection->m_hstmt, 10, SQL_C_CHAR, tmSLEADER_CANDIDATE.CampaignPromises, SIZE_MAX_CAMPAIGNPROMISES, &arrCB[10]);

	while ( (ret = SQLFetch(m_pODBCStmtLeaderElection->m_hstmt)) != SQL_NO_DATA)
	{
		m_TotalVoteCount += tmSLEADER_CANDIDATE.PollCount;
		m_mtvectSLEADER_CANDIDATE.push_back(tmSLEADER_CANDIDATE);
		util::zero(&tmSLEADER_CANDIDATE, sizeof(SSTRATEGYPOINT_NOT_SUMMON_TIME));
	}
	m_pODBCStmtLeaderElection->FreeStatement();
	
	this->InsertGridVoteResultList();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLeaderElection::InitGridVoteResultList()
/// \brief		선거 결과창 초기화
/// \author		dhjin
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLeaderElection::InitGridVoteResultList()
{
	m_GridVoteResultList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 4;
	int m_nFixRows = 1;
	
	m_GridVoteResultList.SetEditable(TRUE);
	m_GridVoteResultList.SetListMode(TRUE);
	m_GridVoteResultList.SetSingleRowSelection(TRUE);
	m_GridVoteResultList.EnableSelection(TRUE);
	m_GridVoteResultList.SetHeaderSort(FALSE);
	m_GridVoteResultList.SetFrameFocusCell(FALSE);
	m_GridVoteResultList.SetTrackFocusCell(FALSE);

	m_GridVoteResultList.SetRowCount(m_nRows);
	m_GridVoteResultList.SetColumnCount(m_nCols);
	m_GridVoteResultList.SetFixedRowCount(m_nFixRows);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Name"));
	m_GridVoteResultList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Influence"));
	m_GridVoteResultList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Total Vote"));
	m_GridVoteResultList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Percent"));
	m_GridVoteResultList.SetItem(&Item);

	m_GridVoteResultList.AutoSize();
	m_GridVoteResultList.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridVoteResultList.GetRowCount()-1, m_GridVoteResultList.GetColumnCount()-1);
	m_GridVoteResultList.ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLeaderElection::InsertGridVoteResultList()
/// \brief		선거 결과창 보여주기
/// \author		dhjin
/// \date		2007-11-01 ~ 2007-11-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLeaderElection::InsertGridVoteResultList()
{
	this->InitGridVoteResultList();

	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		int m_nCols = 0;

		m_GridVoteResultList.SetRowCount(m_GridVoteResultList.GetRowCount()+1);

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = m_GridVoteResultList.GetRowCount()-1;
		Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		Item.col = m_nCols++;
		Item.strText.Format("%s(%d)", itr->CharacterName, itr->CharacterUID);
		m_GridVoteResultList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(itr->Influence));
		m_GridVoteResultList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->PollCount);
		m_GridVoteResultList.SetItem(&Item);

		if(0 >= m_TotalVoteCount)
		{
			Item.col = m_nCols++;
			Item.strText.Format("%d(%%)", 0);
			m_GridVoteResultList.SetItem(&Item);
		}
		else
		{
			Item.col = m_nCols++;
			float fDivideTmp = (float)((float)itr->PollCount / (float)m_TotalVoteCount);
			Item.strText.Format("%d(%%)", (int)(fDivideTmp * 100.0f));
			m_GridVoteResultList.SetItem(&Item);
		}

	}
	
	m_TotalVoteOfNumber	= m_TotalVoteCount;

	ATUM_DATE_TIME CurrentTime;
	CurrentTime.SetCurrentDateTime();
	ATUM_DATE_TIME VoteEndTime;
	VoteEndTime.SetDateTime(m_VoteDateEnd.GetYear(), m_VoteDateEnd.GetMonth(), m_VoteDateEnd.GetDay()
							, m_VoteTimeEnd.GetHour(), m_VoteTimeEnd.GetMinute(), m_VoteTimeEnd.GetSecond());
	if(CurrentTime > VoteEndTime)
	{
		m_Election_State.Format("End");
	}
	else
	{
		m_Election_State.Format("Progress");
	}
	
	UpdateData(FALSE);

}

void CLeaderElection::OnBtnSaveResult() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_mtvectSLEADER_CANDIDATE.size())
	{
		MessageBox("No data !!");
		return;
	}

	CSystemLogManager resultLog;
	if(FALSE == resultLog.InitLogManger(TRUE, "LeaderElectionLog", "./resultLog/"))
	{
		return;
	}
	
	INT tmPercent = 0;
	mtvectSLEADER_CANDIDATE::iterator itr = m_mtvectSLEADER_CANDIDATE.begin();
	for(; itr != m_mtvectSLEADER_CANDIDATE.end(); itr++)
	{
		if(0 < m_TotalVoteCount)
		{
			tmPercent = (itr->PollCount / m_TotalVoteCount) * 100;
		}
		
		char szResult[2048];
		util::zero(szResult, 2048);
		sprintf(szResult, "%s(%d);%s;%d;%d(%%)\r\n",
			itr->CharacterName, itr->CharacterUID, CAtumSJ::GetInfluenceTypeString(itr->Influence),
			itr->PollCount, tmPercent);
		resultLog.WriteSystemLog(szResult, FALSE);

		tmPercent = 0;
	}
	
	MessageBox("Save success !!");	
}
