// DeclarationOfWar.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "DeclarationOfWar.h"
#include "ODBCStatement.h"
#include "AtumSJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeclarationOfWar dialog


CDeclarationOfWar::CDeclarationOfWar(CODBCStatement *i_pODBCStat, CWnd* pParent /*=NULL*/)
	: CDialog(CDeclarationOfWar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeclarationOfWar)
	m_Step = 0;
	m_StepDateStart = COleDateTime::GetCurrentTime();
	m_StepTimeStart = COleDateTime::GetCurrentTime();
	m_Select = 0;
	m_Influence = 0;
	m_ForbidTimeEnd = COleDateTime::GetCurrentTime();
	m_ForbidTimeStart = COleDateTime::GetCurrentTime();
	m_MSDateStart = COleDateTime::GetCurrentTime();
	m_MSTimeStart = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_pODBCStmtDeclarationOfWar	= i_pODBCStat;
}


void CDeclarationOfWar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeclarationOfWar)
	DDX_Text(pDX, IDC_EDIT_STEP, m_Step);
	DDV_MinMaxInt(pDX, m_Step, 0, 99);
	DDX_DateTimeCtrl(pDX, IDC_STEP_DATESTART, m_StepDateStart);
	DDX_DateTimeCtrl(pDX, IDC_STEP_TIMESTART, m_StepTimeStart);
	DDX_Text(pDX, IDC_EDIT_SELECT, m_Select);
	DDV_MinMaxInt(pDX, m_Select, 0, 5);
	DDX_Text(pDX, IDC_EDIT_INFLUENCE, m_Influence);
	DDV_MinMaxInt(pDX, m_Influence, 0, 4);
	DDX_Control(pDX, IDC_COMBO_DETAIL_DAYOFWEEK, m_comboDayOfWeek);
	DDX_DateTimeCtrl(pDX, IDC_FORBID_TIMEEND, m_ForbidTimeEnd);
	DDX_DateTimeCtrl(pDX, IDC_FORBID_TIMESTART, m_ForbidTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_MS_DATESTART, m_MSDateStart);
	DDX_DateTimeCtrl(pDX, IDC_MS_TIMESTART, m_MSTimeStart);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_DECLARATION_OF_WAR_LIST, m_GridDeclarationOfWarList);
}


BEGIN_MESSAGE_MAP(CDeclarationOfWar, CDialog)
	//{{AFX_MSG_MAP(CDeclarationOfWar)
	ON_BN_CLICKED(IDC_BUTTON_STEP_START_UPDATE, OnButtonStepStartUpdate)
	ON_BN_CLICKED(IDC_BUTTON_STEP_SELECT, OnButtonStepSelect)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_FORBID_UPDATE, OnButtonForbidUpdate)
	ON_BN_CLICKED(IDC_BUTTON_MS_START_UPDATE, OnButtonMsStartUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeclarationOfWar message handlers
BOOL CDeclarationOfWar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->GetDeclarationOfWarByDB();		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeclarationOfWar::GetDeclarationOfWarByDB()
{
	// TODO: Add your control notification handler code here
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_DeclarationOfWarInfo
	-- DESC				: 선전 포고
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_DeclarationOfWarInfo
	AS
		SELECT Influence, MSWarStep, NCP, MSNum, MSAppearanceMap, MSWarStepStartTime, MSWarStepEndTime, MSWarStartTime,
				MSWarEndTime, SelectCount, GiveUp, MSWarEndState
		FROM dbo.td_DeclarationOfWar WITH(NOLOCK)
	GO
	************************************************************************/
	RETCODE	ret = SQLExecDirect(m_pODBCStmtDeclarationOfWar->m_hstmt, PROCEDURE_090114_0284, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtDeclarationOfWar->FreeStatement();
		return;
	}
	
	SQLINTEGER	arrCB[13] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							, SQL_NTS,SQL_NTS,SQL_NTS};

	SDECLARATION_OF_WAR_FOR_ADMINTOOL	tmDeclarationOfWar;
	util::zero(&tmDeclarationOfWar, sizeof(SDECLARATION_OF_WAR_FOR_ADMINTOOL));
	m_vectDECLARATION_OF_WAR.clear();
	SQL_TIMESTAMP_STRUCT	MSWarStepStartTime;
	SQL_TIMESTAMP_STRUCT	MSWarStepEndTime;
	SQL_TIMESTAMP_STRUCT	MSWarStartTime;
	SQL_TIMESTAMP_STRUCT	MSWarEndTime;
	util::zero(&MSWarStepStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&MSWarStepEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&MSWarStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&MSWarEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 1, SQL_C_TINYINT,	&tmDeclarationOfWar.Influence, 0, &arrCB[1]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 2, SQL_C_TINYINT,	&tmDeclarationOfWar.MSWarStep, 0, &arrCB[2]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 3, SQL_C_ULONG,	&tmDeclarationOfWar.NCP, 0, &arrCB[3]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 4, SQL_C_ULONG,	&tmDeclarationOfWar.MSNum, 0, &arrCB[4]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 5, SQL_C_ULONG,	&tmDeclarationOfWar.MSAppearanceMap, 0, &arrCB[5]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 6, SQL_C_TIMESTAMP,&MSWarStepStartTime, 0, &arrCB[6]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 7, SQL_C_TIMESTAMP,&MSWarStepEndTime, 0, &arrCB[7]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 8, SQL_C_TIMESTAMP,&MSWarStartTime, 0, &arrCB[8]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 9, SQL_C_TIMESTAMP,&MSWarEndTime, 0, &arrCB[9]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 10, SQL_C_TINYINT,	&tmDeclarationOfWar.SelectCount, 0, &arrCB[10]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 11, SQL_C_BIT,		&tmDeclarationOfWar.GiveUp, 0, &arrCB[11]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 12, SQL_C_TINYINT,	&tmDeclarationOfWar.MSWarEndState, 0, &arrCB[12]);
	INT UID = 1;
	while ( (ret = SQLFetch(m_pODBCStmtDeclarationOfWar->m_hstmt)) != SQL_NO_DATA)
	{
		tmDeclarationOfWar.MSWarStepStartTime	= MSWarStepStartTime;
		tmDeclarationOfWar.MSWarStepEndTime		= MSWarStepEndTime;				
		tmDeclarationOfWar.MSWarStartTime		= MSWarStartTime; 
		tmDeclarationOfWar.MSWarEndTime			= MSWarEndTime;
		tmDeclarationOfWar.UID					= UID;
		m_vectDECLARATION_OF_WAR.push_back(tmDeclarationOfWar);
		util::zero(&MSWarStepStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&MSWarStepEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&MSWarStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&MSWarEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&tmDeclarationOfWar, sizeof(SDECLARATION_OF_WAR));
		UID++;
	}
	m_pODBCStmtDeclarationOfWar->FreeStatement();	

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_DeclarationOfWarForbidTimeInfo
	-- DESC				: 선전 포고 금지시간 가져오기
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Load_DeclarationOfWarForbidTimeInfo
	AS
		SELECT DayOfWeek, ForbidStartTime, ForbidEndTime
		FROM atum2_db_account.dbo.td_DeclarationOfWarForbidTime
	GO
	************************************************************************/
	ret = SQLExecDirect(m_pODBCStmtDeclarationOfWar->m_hstmt, PROCEDURE_090114_0285, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtDeclarationOfWar->FreeStatement();
		return;
	}
	
	SQLINTEGER	arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	util::zero(&m_SDeclarationOfWarForbidTime, sizeof(SDECLARATION_OF_WAR_FORBID_TIME));
	SQL_TIMESTAMP_STRUCT	ForbidStartTime;
	SQL_TIMESTAMP_STRUCT	ForbidEndTime;

	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 1, SQL_C_ULONG,		&m_SDeclarationOfWarForbidTime.DayOfWeek, 0, &arrCB2[1]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 2, SQL_C_TIMESTAMP,	&ForbidStartTime, 0, &arrCB2[2]);
	SQLBindCol(m_pODBCStmtDeclarationOfWar->m_hstmt, 3, SQL_C_TIMESTAMP,	&ForbidEndTime, 0, &arrCB2[3]);
	
	while ( (ret = SQLFetch(m_pODBCStmtDeclarationOfWar->m_hstmt)) != SQL_NO_DATA)
	{
		m_SDeclarationOfWarForbidTime.ForbidStartTime	= ForbidStartTime;
		m_SDeclarationOfWarForbidTime.ForbidEndTime		= ForbidEndTime;				
	}

	m_pODBCStmtDeclarationOfWar->FreeStatement();
	
	this->InsertGridDeclarationOfWarList();
	m_comboDayOfWeek.SetCurSel(m_SDeclarationOfWarForbidTime.DayOfWeek);
	m_ForbidTimeStart.SetTime(m_SDeclarationOfWarForbidTime.ForbidStartTime.Hour, m_SDeclarationOfWarForbidTime.ForbidStartTime.Minute, m_SDeclarationOfWarForbidTime.ForbidStartTime.Second);	
	m_ForbidTimeEnd.SetTime(m_SDeclarationOfWarForbidTime.ForbidEndTime.Hour, m_SDeclarationOfWarForbidTime.ForbidEndTime.Minute, m_SDeclarationOfWarForbidTime.ForbidEndTime.Second);
	
	UpdateData(FALSE);
}

void CDeclarationOfWar::InitGridDeclarationOfWarList()
{
	m_GridDeclarationOfWarList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 13;
	int m_nFixRows = 1;
	
	m_GridDeclarationOfWarList.SetEditable(FALSE);
	m_GridDeclarationOfWarList.SetListMode(TRUE);
	m_GridDeclarationOfWarList.SetSingleRowSelection(TRUE);
	m_GridDeclarationOfWarList.EnableSelection(TRUE);
	m_GridDeclarationOfWarList.SetHeaderSort(FALSE);
	m_GridDeclarationOfWarList.SetFrameFocusCell(FALSE);
	m_GridDeclarationOfWarList.SetTrackFocusCell(FALSE);

	m_GridDeclarationOfWarList.SetRowCount(m_nRows);
	m_GridDeclarationOfWarList.SetColumnCount(m_nCols);
	m_GridDeclarationOfWarList.SetFixedRowCount(m_nFixRows);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	Item.col = m_nCols++;
	Item.strText.Format(_T("UID"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Influence"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("WarStep"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("NCP"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("M.S Num"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("M.S Map"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("StepStartDate"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("StepEndDate"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("StartDate"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("EndDate"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Select"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("GiveUp"));
	m_GridDeclarationOfWarList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Result"));
	m_GridDeclarationOfWarList.SetItem(&Item);
	
	m_GridDeclarationOfWarList.AutoSize();
	m_GridDeclarationOfWarList.ExpandColumnsToFit();

	CCellRange tmpCellRange(1, 0, m_GridDeclarationOfWarList.GetRowCount()-1, m_GridDeclarationOfWarList.GetColumnCount()-1);
	m_GridDeclarationOfWarList.ClearCells(tmpCellRange);
}

void CDeclarationOfWar::InsertGridDeclarationOfWarList()
{
	this->InitGridDeclarationOfWarList();

	vectSDECLARATION_OF_WAR_FOR_ADMINTOOL::iterator itr = m_vectDECLARATION_OF_WAR.begin();
	for(; itr != m_vectDECLARATION_OF_WAR.end(); itr++)
	{
		int m_nCols = 0;

		m_GridDeclarationOfWarList.SetRowCount(m_GridDeclarationOfWarList.GetRowCount()+1);

		GV_ITEM Item;
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.row = m_GridDeclarationOfWarList.GetRowCount()-1;
		Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->UID);
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(itr->Influence));
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->MSWarStep);
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->NCP);
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->MSNum);
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->MSAppearanceMap);
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->MSWarStepStartTime.GetDateTimeString().GetBuffer());
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->MSWarStepEndTime.GetDateTimeString().GetBuffer());
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->MSWarStartTime.GetDateTimeString().GetBuffer());
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%s", itr->MSWarEndTime.GetDateTimeString().GetBuffer());
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->SelectCount);
		m_GridDeclarationOfWarList.SetItem(&Item);

		Item.col = m_nCols++;
		Item.strText.Format("%d", itr->GiveUp);
		m_GridDeclarationOfWarList.SetItem(&Item);

		if(MSWAR_END_WIN == itr->MSWarEndState)
		{
			Item.col = m_nCols++;
			Item.strText.Format("WIN");
			m_GridDeclarationOfWarList.SetItem(&Item);
		}
		else if(MSWAR_END_LOSS == itr->MSWarEndState)
		{
			Item.col = m_nCols++;
			Item.strText.Format("LOSS");
			m_GridDeclarationOfWarList.SetItem(&Item);
		}

	}
	
	UpdateData(FALSE);
}

void CDeclarationOfWar::OnButtonStepStartUpdate() 
{
	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL))
	{
		return;
	}
	UpdateData(TRUE);
	if(0 == m_Step)
	{
		MessageBox("Check Update Select");
		return;
	}

	if(1 != m_Step)
	{
		vectSDECLARATION_OF_WAR_FOR_ADMINTOOL::iterator itr = m_vectDECLARATION_OF_WAR.begin();
		for(; itr != m_vectDECLARATION_OF_WAR.end(); itr++)
		{
			if(MSWAR_NEXT_LEADER_SETP == m_Step)
			{
				if(MSWAR_FINAL_SETP == itr->MSWarStep)
				{
					ATUM_DATE_TIME CheckTime;
					CheckTime.SetDateTime(m_StepDateStart.GetYear(), m_StepDateStart.GetMonth(), m_StepDateStart.GetDay(),
											m_StepTimeStart.GetHour(), m_StepTimeStart.GetMinute(), m_StepTimeStart.GetSecond());
					if(CheckTime < itr->MSWarStepEndTime)
					{
						MessageBox("Invaild MSStep Time");
						return;
					}
					break;
				}
			}
			else
			{
				if(m_Step-1 == itr->MSWarStep)
				{
					ATUM_DATE_TIME CheckTime;
					CheckTime.SetDateTime(m_StepDateStart.GetYear(), m_StepDateStart.GetMonth(), m_StepDateStart.GetDay(),
											m_StepTimeStart.GetHour(), m_StepTimeStart.GetMinute(), m_StepTimeStart.GetSecond());
					if(CheckTime < itr->MSWarStepEndTime)
					{
						MessageBox("Invaild MSStep Time");
						return;
					}
					break;
				}
			}
		}
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateStepDeclarationOfWarByAdminTool
	-- DESC				: 선전 포고 단계 설정.
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateStepDeclarationOfWarByAdminTool
		@i_MSWarStep			TINYINT,
		@i_MSWarStartTime		VARCHAR(30)
	AS
		IF (1 = @i_MSWarStep)
		BEGIN
			UPDATE dbo.td_DeclarationOfWar SET MSWarStepStartTime = @i_MSWarStartTime, MSWarStepEndTime = DateAdd(day, 7, @i_MSWarStartTime)
												, MSWarStartTime = DateAdd(day, 3, MSWarStepStartTime)
			WHERE MSWarStep = @i_MSWarStep
			SET @i_MSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE MSWarStep = @i_MSWarStep)
			SET @i_MSWarStep = @i_MSWarStep + 1
		END
		IF (2 = @i_MSWarStep)
		BEGIN
			UPDATE dbo.td_DeclarationOfWar SET MSWarStepStartTime = @i_MSWarStartTime, MSWarStepEndTime = DateAdd(day, 7, @i_MSWarStartTime)
												, MSWarStartTime = DateAdd(day, 3, MSWarStepStartTime)
			WHERE MSWarStep = @i_MSWarStep
			SET @i_MSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE MSWarStep = @i_MSWarStep)
			SET @i_MSWarStep = @i_MSWarStep + 1
		END
		IF (3 = @i_MSWarStep)
		BEGIN
			UPDATE dbo.td_DeclarationOfWar SET MSWarStepStartTime = @i_MSWarStartTime, MSWarStepEndTime = DateAdd(day, 7, @i_MSWarStartTime)
												, MSWarStartTime = DateAdd(day, 3, MSWarStepStartTime)
			WHERE MSWarStep = @i_MSWarStep
			SET @i_MSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE MSWarStep = @i_MSWarStep)
			SET @i_MSWarStep = @i_MSWarStep + 1
		END
		IF (4 = @i_MSWarStep)
		BEGIN
			UPDATE dbo.td_DeclarationOfWar SET MSWarStepStartTime = @i_MSWarStartTime, MSWarStepEndTime = DateAdd(day, 7, @i_MSWarStartTime)
												, MSWarStartTime = DateAdd(day, 3, MSWarStepStartTime)
			WHERE MSWarStep = @i_MSWarStep
			SET @i_MSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE MSWarStep = @i_MSWarStep)
			SET @i_MSWarStep = @i_MSWarStep + 1
		END
		IF (5 = @i_MSWarStep)
		BEGIN
			UPDATE dbo.td_DeclarationOfWar SET MSWarStepStartTime = @i_MSWarStartTime, MSWarStepEndTime = DateAdd(day, 7, @i_MSWarStartTime)
												, MSWarStartTime = DateAdd(day, 3, MSWarStepStartTime)
			WHERE MSWarStep = @i_MSWarStep
			SET @i_MSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE MSWarStep = @i_MSWarStep)
			SET @i_MSWarStep = @i_MSWarStep + 1
		END
		IF (99 = @i_MSWarStep)
		BEGIN
			UPDATE dbo.td_DeclarationOfWar SET MSWarStepStartTime = @i_MSWarStartTime, MSWarStepEndTime = DateAdd(day, 7, @i_MSWarStartTime)
												, MSWarStartTime = DateAdd(day, 3, MSWarStepStartTime)
			WHERE MSWarStep = @i_MSWarStep
			SET @i_MSWarStartTime = (SELECT MSWarStepEndTime FROM dbo.td_DeclarationOfWar WHERE MSWarStep = @i_MSWarStep)
			SET @i_MSWarStep = @i_MSWarStep + 1
		END
	GO
	************************************************************************/
	SQLINTEGER	arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_Step, 0,	&arrCB[1]);
	ATUM_DATE_TIME	tmSetTime;
	tmSetTime.SetDateTime(m_StepDateStart.GetYear(), m_StepDateStart.GetMonth(), m_StepDateStart.GetDay()
							, m_StepTimeStart.GetHour(), m_StepTimeStart.GetMinute(), m_StepTimeStart.GetSecond());
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	tmSetTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	&arrCB[2]);
	RETCODE	ret = SQLExecDirect(m_pODBCStmtDeclarationOfWar->m_hstmt, PROCEDURE_090114_0289, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtDeclarationOfWar->FreeStatement();
		return;
	}
	m_pODBCStmtDeclarationOfWar->FreeStatement();
	
	this->GetDeclarationOfWarByDB();
}

void CDeclarationOfWar::OnButtonStepSelect() 
{
	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL))
	{
		return;
	}
	UpdateData(TRUE);
	if(0 == m_Step)
	{
		MessageBox("Check Update Select");
		return;
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateSelectCountDeclarationOfWarByAdminTool
	-- DESC				: 선전 포고 전쟁 수정 설정 수 업데이트
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateSelectCountDeclarationOfWarByAdminTool
		@i_Influence			TINYINT, 
		@i_MSWarStep			TINYINT, 
		@i_SelectCount			TINYINT    
	AS
		UPDATE dbo.td_DeclarationOfWar SET SelectCount = @i_SelectCount
		WHERE Influence = @i_Influence AND MSWarStep = @i_MSWarStep
	GO
	************************************************************************/
	SQLINTEGER	arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_Influence, 0,	&arrCB[1]);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_Step, 0,	&arrCB[2]);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_Select, 0,	&arrCB[3]);
	RETCODE	ret = SQLExecDirect(m_pODBCStmtDeclarationOfWar->m_hstmt, PROCEDURE_090114_0290, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtDeclarationOfWar->FreeStatement();
		return;
	}
	m_pODBCStmtDeclarationOfWar->FreeStatement();	

	this->GetDeclarationOfWarByDB();
}

void CDeclarationOfWar::OnButtonSelect() 
{
	// TODO: Add your control notification handler code here

	CCellID cellID = m_GridDeclarationOfWarList.GetFocusCell();
	if(0 >= cellID.row)
	{
		return;
	}

	int SelectUID				= atoi(m_GridDeclarationOfWarList.GetItemText(cellID.row, 0));
	vectSDECLARATION_OF_WAR_FOR_ADMINTOOL::iterator itr = m_vectDECLARATION_OF_WAR.begin();
	for(; itr != m_vectDECLARATION_OF_WAR.end(); itr++)
	{
		if(SelectUID == itr->UID)
		{
			m_Step			= itr->MSWarStep;
			m_Influence		= itr->Influence;
			m_Select		= itr->SelectCount;
			m_StepDateStart.SetDate(itr->MSWarStepStartTime.Year, itr->MSWarStepStartTime.Month, itr->MSWarStepStartTime.Day);
			m_StepTimeStart.SetTime(itr->MSWarStepStartTime.Hour, itr->MSWarStepStartTime.Minute, itr->MSWarStepStartTime.Second);
			m_MSDateStart.SetDate(itr->MSWarStartTime.Year, itr->MSWarStartTime.Month, itr->MSWarStartTime.Day);
			m_MSTimeStart.SetTime(itr->MSWarStartTime.Hour, itr->MSWarStartTime.Minute, itr->MSWarStartTime.Second);
		}
	}

	UpdateData(FALSE);	
}

void CDeclarationOfWar::OnButtonForbidUpdate() 
{
	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL))
	{
		return;
	}
	
	UpdateData(TRUE);
	m_SDeclarationOfWarForbidTime.DayOfWeek		= m_comboDayOfWeek.GetCurSel();	
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateForbidTimeDeclarationOfWarByAdminTool
	-- DESC				: 선전 포고 전쟁 수정 설정 수 업데이트
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateForbidTimeDeclarationOfWarByAdminTool
		@i_DayOfWeek				INT,
		@i_ForbidStartTime			VARCHAR(30),
		@i_ForbidEndTime			VARCHAR(30)
	AS
		UPDATE atum2_db_account.dbo.td_DeclarationOfWarForbidTime SET DayOfWeek = @i_DayOfWeek, ForbidStartTime = @i_ForbidStartTime
																	, ForbidEndTime = @i_ForbidEndTime
	GO
	************************************************************************/
	SQLINTEGER	arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_SDeclarationOfWarForbidTime.DayOfWeek, 0,	&arrCB[1]);
	ATUM_DATE_TIME	tmStartTime;
	tmStartTime.SetCurrentDateTime();
	tmStartTime.Hour = m_ForbidTimeStart.GetHour();
	tmStartTime.Minute = m_ForbidTimeStart.GetMinute();
	tmStartTime.Second = m_ForbidTimeStart.GetSecond();
	char tmpSTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	tmStartTime.GetSQLDateTimeString(tmpSTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpSTimeString, 0,	&arrCB[2]);
	ATUM_DATE_TIME	tmEndTime;
	tmEndTime.SetCurrentDateTime();
	tmEndTime.Hour = m_ForbidTimeEnd.GetHour();
	tmEndTime.Minute = m_ForbidTimeEnd.GetMinute();
	tmEndTime.Second = m_ForbidTimeEnd.GetSecond();
	char tmpETimeString[SIZE_MAX_SQL_DATETIME_STRING];
	tmEndTime.GetSQLDateTimeString(tmpETimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpETimeString, 0,	&arrCB[3]);
	RETCODE	ret = SQLExecDirect(m_pODBCStmtDeclarationOfWar->m_hstmt, PROCEDURE_090114_0291, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtDeclarationOfWar->FreeStatement();
		return;
	}
	m_pODBCStmtDeclarationOfWar->FreeStatement();

	this->GetDeclarationOfWarByDB();
}

void CDeclarationOfWar::OnButtonMsStartUpdate() 
{
	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL))
	{
		return;
	}
	UpdateData(TRUE);
	if(0 == m_Step)
	{
		MessageBox("Check Update Select");
		return;
	}
	
	
	vectSDECLARATION_OF_WAR_FOR_ADMINTOOL::iterator itr = m_vectDECLARATION_OF_WAR.begin();
	for(; itr != m_vectDECLARATION_OF_WAR.end(); itr++)
	{
		if(m_Step == itr->MSWarStep
			&& m_Influence == itr->Influence)
		{
			ATUM_DATE_TIME CheckTime;
			CheckTime.SetDateTime(m_MSDateStart.GetYear(), m_MSDateStart.GetMonth(), m_MSDateStart.GetDay(),
									m_MSTimeStart.GetHour(), m_MSTimeStart.GetMinute(), m_MSTimeStart.GetSecond());
			if(CheckTime <= itr->MSWarStepStartTime
				|| CheckTime >= itr->MSWarStepEndTime)
			{
				MessageBox("Invaild MSStart Time");
				return;
			}
		}
	}

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateMSWarStartDeclarationOfWarByAdminTool
	-- DESC				: 선전 포고 전쟁 모선전 시작 시간 업데이트
	-- 2009-01-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateMSWarStartDeclarationOfWarByAdminTool
		@i_Influence			TINYINT, 
		@i_MSWarStep			TINYINT, 
		@i_MSWarStartTime		VARCHAR(30)   
	AS
		UPDATE dbo.td_DeclarationOfWar SET MSWarStartTime = @i_MSWarStartTime
		WHERE Influence = @i_Influence AND MSWarStep = @i_MSWarStep AND MSWarStepStartTime <= @i_MSWarStartTime AND MSWarStepEndTime >= @i_MSWarStartTime
	GO
	************************************************************************/
	SQLINTEGER	arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_Influence, 0,	&arrCB[1]);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &m_Step, 0,	&arrCB[2]);
	ATUM_DATE_TIME	tmSetTime;
	tmSetTime.SetDateTime(m_MSDateStart.GetYear(), m_MSDateStart.GetMonth(), m_MSDateStart.GetDay()
							, m_MSTimeStart.GetHour(), m_MSTimeStart.GetMinute(), m_MSTimeStart.GetSecond());
	char tmpTimeString[SIZE_MAX_SQL_DATETIME_STRING];
	tmSetTime.GetSQLDateTimeString(tmpTimeString, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(m_pODBCStmtDeclarationOfWar->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpTimeString, 0,	&arrCB[3]);
	RETCODE	ret = SQLExecDirect(m_pODBCStmtDeclarationOfWar->m_hstmt, PROCEDURE_090114_0293, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		m_pODBCStmtDeclarationOfWar->FreeStatement();
		return;
	}
	m_pODBCStmtDeclarationOfWar->FreeStatement();		

	this->GetDeclarationOfWarByDB();
}
