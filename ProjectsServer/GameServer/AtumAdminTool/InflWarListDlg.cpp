// InflWarListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "InflWarListDlg.h"
#include "ODBCStatement.h"
#include "AtumSJ.h"
#include "LeaderElection.h"
#include "DeclarationOfWar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInflWarListDlg dialog


CInflWarListDlg::CInflWarListDlg(BOOL i_bEnableEdit, CODBCStatement *i_pODBCStat, CWnd* pParent /*=NULL*/)
	: CDialog(CInflWarListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInflWarListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pODBCStmt2		= i_pODBCStat;
	m_bEnableEdit		= i_bEnableEdit;		// 2006-05-23 by cmkwon
}


void CInflWarListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInflWarListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_INFLWAR_LIST, m_GridInflWarList);
}


BEGIN_MESSAGE_MAP(CInflWarListDlg, CDialog)
	//{{AFX_MSG_MAP(CInflWarListDlg)
	ON_BN_CLICKED(IDC_BTN_RELOAD_INFLWARDATA_LIST, OnBtnReloadInflwardataList)
	ON_BN_CLICKED(IDC_BTN_INIT_INFL_DATA, OnBtnInitInflData)
	ON_BN_CLICKED(IDC_BTN_INIT_MONTHLY_GUILDFAME, OnBtnInitMonthlyGuildfame)
	ON_BN_CLICKED(IDC_BTN_LEADER_ELECTION, OnBtnLeaderElection)
	ON_BN_CLICKED(IDC_BTN_DECLARATION_OF_WAR, OnBtnDeclarationOfWar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInflWarListDlg message handlers


BOOL CInflWarListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	this->OnBtnReloadInflwardataList();

	GetDlgItem(IDC_BTN_INIT_INFL_DATA)->EnableWindow(m_bEnableEdit);
	GetDlgItem(IDC_BTN_INIT_MONTHLY_GUILDFAME)->EnableWindow(m_bEnableEdit);		// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 수정 권한이 없으면 버튼 비활성화
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}








///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarListDlg::InitGridInflWarList(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-03 ~ 2006-05-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarListDlg::InitGridInflWarList(void)
{
	m_GridInflWarList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 8;			// 2013-05-09 by hskim, 세력 포인트 개선
	int m_nFixRows = 1;
	
	m_GridInflWarList.SetEditable(TRUE);
	m_GridInflWarList.SetListMode(TRUE);
	m_GridInflWarList.SetSingleRowSelection(TRUE);
	m_GridInflWarList.EnableSelection(TRUE);
	m_GridInflWarList.SetHeaderSort(FALSE);
	m_GridInflWarList.SetFrameFocusCell(FALSE);
	m_GridInflWarList.SetTrackFocusCell(FALSE);
//	m_GridInflWarList.EnableDragAndDrop(TRUE);
//	m_GridInflWarList.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

	m_GridInflWarList.SetRowCount(m_nRows);
	m_GridInflWarList.SetColumnCount(m_nCols);
	m_GridInflWarList.SetFixedRowCount(m_nFixRows);
	//m_GridInflWarList.SetFixedColumnCount(m_nFixCols);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format(_T("InfluenceType"));
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Server Group ID"));
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Wartime Stage"));
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Contribution Point"));
	m_GridInflWarList.SetItem(&Item);

	// 2013-05-09 by hskim, 세력 포인트 개선
	Item.col = m_nCols++;
	Item.strText.Format(_T("ConsecutiveVictories"));
	m_GridInflWarList.SetItem(&Item);
	// end 2013-05-09 by hskim, 세력 포인트 개선

	Item.col = m_nCols++;
	Item.strText.Format(_T("Influence Leader Character Info"));
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Influence SubLeader_1 Character Info"));
	m_GridInflWarList.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("Influence SubLeader_2 Character Info"));
	m_GridInflWarList.SetItem(&Item);

	m_GridInflWarList.AutoSize();
	m_GridInflWarList.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridInflWarList.GetRowCount()-1, m_GridInflWarList.GetColumnCount()-1);
	m_GridInflWarList.ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarListDlg::LoadInflWarData(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-03 ~ 2006-05-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarListDlg::LoadInflWarData(void)
{
	// 2006-12-13 by dhjin, 부지도자 정보 추가로 수정됨
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("SELECT wd.InfluenceType, wd.MGameServerID, wd.WartimeStage, wd.ContributionPoint, wd.InflLeaderCharacterUID, c.CharacterName, wd.InflSub1LeaderCharacterUID, c1.CharacterName, wd.InflSub2LeaderCharacterUID, c2.CharacterName \
// 					FROM td_InfluenceWarData wd  LEFT OUTER JOIN td_character c \
// 							ON wd.InflLeaderCharacterUID = c.uniquenumber	\
// 							LEFT OUTER JOIN td_character c1	\
// 							ON wd.InflSub1LeaderCharacterUID = c1.uniquenumber \
// 							LEFT OUTER JOIN td_character c2 \
// 							ON wd.InflSub2LeaderCharacterUID = c2.uniquenumber"); 

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0007);
//	BOOL bRet = m_pODBCStmt2->ExecuteQuery(szQuery);
	BOOL bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0007));
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CInflWarListDlg::LoadInflWarData_)");
		m_pODBCStmt2->FreeStatement();
		return;
	}

	SQLINTEGER arrCB[11] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS};		// 2013-05-09 by hskim, 세력 포인트 개선

	SINFLUENCE_WAR_DATA		tmInflWarData;
	char					szInflLeaderCharacterName[SIZE_MAX_CHARACTER_NAME];
	char					szInflSub1LeaderCharacterName[SIZE_MAX_CHARACTER_NAME];
	char					szInflSub2LeaderCharacterName[SIZE_MAX_CHARACTER_NAME];
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 1, SQL_C_UTINYINT, &tmInflWarData.InfluenceType, 0,					&arrCB[1]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 2, SQL_C_UTINYINT, &tmInflWarData.WartimeStage, 0,					&arrCB[2]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 3, SQL_C_LONG, &tmInflWarData.ContributionPoint, 0,					&arrCB[3]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 4, SQL_C_ULONG, &tmInflWarData.InflLeaderCharacterUID, 0,				&arrCB[4]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 5, SQL_C_CHAR, szInflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[5]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 6, SQL_C_ULONG, &tmInflWarData.InflSub1LeaderCharacterUID, 0,				&arrCB[6]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 7, SQL_C_CHAR, szInflSub1LeaderCharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[7]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 8, SQL_C_ULONG, &tmInflWarData.InflSub2LeaderCharacterUID, 0,				&arrCB[8]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 9, SQL_C_CHAR, szInflSub2LeaderCharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[9]);
	SQLBindCol(m_pODBCStmt2->GetSTMTHandle(), 10, SQL_C_SHORT, &tmInflWarData.ConsecutiveVictories, 0,					&arrCB[10]);		// 2013-05-09 by hskim, 세력 포인트 개선

	util::zero(&tmInflWarData, sizeof(tmInflWarData));
	util::zero(szInflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::zero(szInflSub1LeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::zero(szInflSub2LeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	while (SQL_NO_DATA != SQLFetch(m_pODBCStmt2->GetSTMTHandle()))
	{
		this->InsertInflWarData(&tmInflWarData, szInflLeaderCharacterName, szInflSub1LeaderCharacterName, szInflSub2LeaderCharacterName);

		util::zero(&tmInflWarData, sizeof(tmInflWarData));
		util::zero(szInflLeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
		util::zero(szInflSub1LeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
		util::zero(szInflSub2LeaderCharacterName, SIZE_MAX_CHARACTER_NAME);
	}
	m_pODBCStmt2->FreeStatement();				// free statement
	m_GridInflWarList.Refresh();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarListDlg::DBInitAllInflWarData(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-23 ~ 2006-05-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarListDlg::DBInitAllInflWarData(void)
{
	// 2006-08-02 by cmkwon, OwnerOfConflictArea 초기화 추가 - 세력전 초기화 시에 분쟁지역 설정도 초기화
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("UPDATE td_InfluenceWarData SET WartimeStage = 0, ContributionPoint = 0, InflLeaderCharacterUID = 0, OwnerOfConflictArea = 0, InflSub1LeaderCharacterUID = 0, InflSub2LeaderCharacterUID = 0, MSWarOptionType = 510"); // 2008-04-10 by dhjin, 모선전 정보 표시 기획안 - 기본값 510

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0008);
//	BOOL bRet = m_pODBCStmt2->ExecuteQuery(szQuery);
	BOOL bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0008));
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CInflWarListDlg::DBInitAllInflWarData_)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();				// free statement
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(DBInitAllInflWarData_)");	

	// 2007-02-13 by dhjin, 지도자 아이템 삭제
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_DeleteLeaderItem
	-- DESC      : 2007-02-13 by dhjin, 지도자 전용 아이템 삭제하는 프로시저
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteLeaderItem
	AS
		DELETE FROM dbo.td_Store WHERE Itemnum = 7001320
		DELETE FROM dbo.td_Store WHERE Itemnum = 7001330
		DELETE FROM dbo.td_Store WHERE Itemnum = 7001090
		DELETE FROM dbo.td_Store WHERE Itemnum = 7001110
	GO
	**************************************************************************/
	RETCODE	ret;

	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0006, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(CInflWarListDlg::DeleteLeaderItem_)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(DeleteLeaderItem_)");

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-30 by cmkwon, 회의룸 시스템 구현 - SCAdminTool에서 세력전 초기화 시에 회의룸 접근 리스트도 초기화 한다.
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:dbo.atum_deleteAllWarpableUserList
	-- Desc: // 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 모든 유저 삭제하기
	--====
	CREATE PROCEDURE dbo.atum_deleteAllWarpableUserList
	AS
		DELETE FROM dbo.td_user_list_warpable_to_map
	GO
	**************************************************************************/
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0007, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_deleteAllWarpableUserList_)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();	
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(deleteAllWarpableUserList_)");

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_InitCityLeaderForInflLeader
	-- DESC			 : 세력 초기화 시 td_CityLeaderInfo에 지도자 관련 초기화한다.
	-- 2007-09-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InitCityLeaderForInflLeader
	AS
		UPDATE dbo.td_CityLeaderInfo
		SET	Influence = 0, CharacterUID = 0, ExpediencyFundCumulative = 0, ExpediencyFund = 0, ExpediencyFundRate = 0, Notice = NULL
		WHERE MapIndex = 2001

		UPDATE dbo.td_CityLeaderInfo
		SET	Influence = 0, CharacterUID = 0, ExpediencyFundCumulative = 0, ExpediencyFund = 0, ExpediencyFundRate = 0, Notice = NULL
		WHERE MapIndex = 2002
	GO
	**************************************************************************/	
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0008, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_InitCityLeaderForInflLeader)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();	
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(InitCityLeaderForInflLeader)");

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InitVoterList
	-- DESC				: 투표자 리스트 초기화 프로시저
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InitVoterList
	AS
		DELETE FROM dbo.td_VoterList
	GO
	**************************************************************************/	
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0009, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_InitVoterList)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();	
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(InitVoterList)");

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InitLeaderCandidate
	-- DESC				: 지도자 후보 리스트 초기화 프로시저
	-- 2007-10-24 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InitLeaderCandidate
	AS
		DELETE FROM dbo.td_LeaderCandidate
	GO
	**************************************************************************/	
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0010, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_InitLeaderCandidate)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();	
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(InitLeaderCandidate)");

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_InitMSWar
	-- DESC				: 모선전 리스트 초기화 프로시저
	-- // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_InitMSWar
	AS
		DELETE FROM dbo.atum_log_MSWar
	GO
	**************************************************************************/	
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0011, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_log_InitMSWar)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();	
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(log_InitMSWar)");

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_InitSPWar
	-- DESC				: 거점전 리스트 초기화 프로시저
	-- // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_InitSPWar
	AS
		DELETE FROM dbo.atum_log_SPWar
	GO
	**************************************************************************/	
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_080822_0012, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_log_InitSPWar)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(log_InitSPWar)");

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_ResetDeclarationOfWarByAdminTool
	-- DESC				: 선전 포고 지도자 초기화때 초기화된다.
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_ResetDeclarationOfWarByAdminTool
	AS
		DELETE FROM dbo.td_DeclarationOfWar WHERE MSWarStep = 1 or MSWarStep = 2 or MSWarStep = 3 or MSWarStep = 4 or MSWarStep = 5
		
		UPDATE dbo.td_DeclarationOfWar SET MSWarStep = 1 WHERE MSWarStep = 99

		DECLARE @tmMSWarStartTime datetime
		SET @tmMSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE Influence = 2 AND MSWarStep = 1)

		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (2, 2, 0, 0, 0, @tmMSWarStartTime, DateAdd(day, 7, @tmMSWarStartTime), DateAdd(day, 3, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (2, 3, 0, 0, 0, DateAdd(day, 7, @tmMSWarStartTime), DateAdd(day, 14, @tmMSWarStartTime), DateAdd(day, 10, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (2, 4, 0, 0, 0, DateAdd(day, 14, @tmMSWarStartTime), DateAdd(day, 21, @tmMSWarStartTime), DateAdd(day, 17, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (2, 5, 0, 0, 0, DateAdd(day, 21, @tmMSWarStartTime), DateAdd(day, 28, @tmMSWarStartTime), DateAdd(day, 24, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (2, 99, 0, 0, 0, DateAdd(day, 28, @tmMSWarStartTime), DateAdd(day, 35, @tmMSWarStartTime), DateAdd(day, 31, @tmMSWarStartTime), NULL, 3, 0, 0)

		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (4, 2, 0, 0, 0, @tmMSWarStartTime, DateAdd(day, 7, @tmMSWarStartTime), DateAdd(day, 3, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (4, 3, 0, 0, 0, DateAdd(day, 7, @tmMSWarStartTime), DateAdd(day, 14, @tmMSWarStartTime), DateAdd(day, 10, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (4, 4, 0, 0, 0, DateAdd(day, 14, @tmMSWarStartTime), DateAdd(day, 21, @tmMSWarStartTime), DateAdd(day, 17, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (4, 5, 0, 0, 0, DateAdd(day, 21, @tmMSWarStartTime), DateAdd(day, 28, @tmMSWarStartTime), DateAdd(day, 24, @tmMSWarStartTime), NULL, 3, 0, 0)
		INSERT INTO dbo.td_DeclarationOfWar (Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime
												, MSWarStartTime, MSWarEndTime, SelectCount, GiveUp, MSWarEndState)
			VALUES (4, 99, 0, 0, 0, DateAdd(day, 28, @tmMSWarStartTime), DateAdd(day, 35, @tmMSWarStartTime), DateAdd(day, 31, @tmMSWarStartTime), NULL, 3, 0, 0)
	GO
	**************************************************************************/	
	ret = SQLExecDirect(m_pODBCStmt2->m_hstmt, PROCEDURE_090114_0292, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		MessageBox("DB Execute Error !!(atum_ResetDeclarationOfWarByAdminTool)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();
	// 2008-08-06 by dhjin, 성공메세지 찍기,
	MessageBox("Success !!(ResetDeclarationOfWar)");

	MessageBox("Success !!(Init_InflWarList)");

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarListDlg::DBInitAllMonthlyGuildFame(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-06-20 ~ 2006-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarListDlg::DBInitAllMonthlyGuildFame(void)
{
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("UPDATE td_Guild SET MonthlyFame = 0");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0009);
//	BOOL bRet = m_pODBCStmt2->ExecuteQuery(szQuery);
	BOOL bRet = m_pODBCStmt2->ExecuteQuery((char*)(PROCEDURE_080827_0009));
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CInflWarListDlg::DBInitAllMonthlyGuildFame_)");
		m_pODBCStmt2->FreeStatement();
		return;
	}
	m_pODBCStmt2->FreeStatement();				// free statement
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CInflWarListDlg::InsertInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, char *i_szLeaderCharacterName, char *i_szSub1LeaderCharacterName, char *i_szSub2LeaderCharacterName)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-03 ~ 2006-05-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CInflWarListDlg::InsertInflWarData(SINFLUENCE_WAR_DATA *i_pInflWarData, char *i_szLeaderCharacterName, char *i_szSub1LeaderCharacterName, char *i_szSub2LeaderCharacterName)
{
	int m_nCols = 0;

	m_GridInflWarList.SetRowCount(m_GridInflWarList.GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = m_GridInflWarList.GetRowCount()-1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	Item.col = m_nCols++;
	Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(i_pInflWarData->InfluenceType));
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", ""); // 2017-01-02 panoskj
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pInflWarData->WartimeStage);
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pInflWarData->ContributionPoint);
	m_GridInflWarList.SetItem(&Item);
	
	// 2013-05-09 by hskim, 세력 포인트 개선
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pInflWarData->ConsecutiveVictories);
	m_GridInflWarList.SetItem(&Item);
	// 2013-05-09 by hskim, 세력 포인트 개선

	// 2006-12-13 by dhjin, 부지도자 보여주기
	Item.col = m_nCols++;
	Item.strText.Format("%s(%d)", i_szLeaderCharacterName, i_pInflWarData->InflLeaderCharacterUID);
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s(%d)", i_szSub1LeaderCharacterName, i_pInflWarData->InflSub1LeaderCharacterUID);
	m_GridInflWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s(%d)", i_szSub2LeaderCharacterName, i_pInflWarData->InflSub2LeaderCharacterUID);
	m_GridInflWarList.SetItem(&Item);
}

void CInflWarListDlg::OnBtnReloadInflwardataList() 
{
	// TODO: Add your control notification handler code here

	this->InitGridInflWarList();
	this->LoadInflWarData();
}

void CInflWarListDlg::OnBtnInitInflData() 
{
	// TODO: Add your control notification handler code here

	CString str;
	str.Format("IMServer and FieldServer must be shutdown if you want to initialize InfluenceWar Data.\n\n  IMServer and FieldServer is shutdowned?");
	int nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		return;
	}

	this->DBInitAllInflWarData();
	this->OnBtnReloadInflwardataList();
}

void CInflWarListDlg::OnBtnInitMonthlyGuildfame() 
{
	// TODO: Add your control notification handler code here
	CString str;
	str.Format("IMServer and FieldServer must be shutdown if you want to initialize all Monthly Guild Fame.\n\n  IMServer and FieldServer is shutdowned?");
	int nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		return;
	}

	this->DBInitAllMonthlyGuildFame();
}

void CInflWarListDlg::OnBtnLeaderElection() 
{
	// TODO: Add your control notification handler code here
	CLeaderElection dlg(m_pODBCStmt2);
	dlg.DoModal();

}

void CInflWarListDlg::OnBtnDeclarationOfWar() 
{
	// TODO: Add your control notification handler code here
	CDeclarationOfWar dlg(m_pODBCStmt2);
	dlg.DoModal();	
}
