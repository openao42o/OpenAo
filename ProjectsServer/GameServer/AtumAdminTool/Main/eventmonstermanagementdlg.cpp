// eventmonstermanagementdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "eventmonstermanagementdlg.h"
#include "AtumAdminToolDlg.h"				// 2008-04-17 by cmkwon
#include "seteventmonsterdlg.h"				// 2008-04-17 by cmkwon

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventMonsterManagementDlg dialog


CEventMonsterManagementDlg::CEventMonsterManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventMonsterManagementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventMonsterManagementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pODBCStmt		= new CODBCStatement;
	

	m_pMainDlg		= (CAtumAdminToolDlg*)pParent;
}

CEventMonsterManagementDlg::~CEventMonsterManagementDlg()
{
	util::del(m_pODBCStmt);	
}


void CEventMonsterManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventMonsterManagementDlg)
	DDX_Control(pDX, IDC_GRID_EVENT_MONSTER_LIST, m_eventMonserListGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEventMonsterManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CEventMonsterManagementDlg)
	ON_COMMAND(ID_CONMENU_ADD_EVENT_MONSTER, OnConmenuAddEventMonster)
	ON_COMMAND(ID_CONMENU_MODIFY_EVENT_MONSTER, OnConmenuModifyEventMonster)
	ON_COMMAND(ID_CONMENU_DELETE_EVENT_MONSTER, OnConmenuDeleteEventMonster)
	ON_BN_CLICKED(IDC_BTN_INSERT, OnBtnInsert)
	ON_BN_CLICKED(IDC_BTN_RELOAD, OnBtnReload)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_EVENT_MONSTER_LIST, OnGridRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventMonsterManagementDlg message handlers
BOOL CEventMonsterManagementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 2008-04-17 by cmkwon, Connect DB
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		char szTemp[1024];
		sprintf(szTemp, STRERR_S_SCADMINTOOL_0021
			, m_pMainDlg->m_pServerInfo4Admin->DBName, m_pMainDlg->m_pServerInfo4Admin->DBIP
			, m_pMainDlg->m_pServerInfo4Admin->DBPort);
		AfxMessageBox(szTemp);
		return TRUE;
	}
	CAtumDBHelper::Load_MEX_MONSTER_INFO(m_pODBCStmt, &m_vecMexMonsterInfo);
	
	this->LoadEventMonsterFromDB();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEventMonsterManagementDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	UpdateData();

	CSCGridCtrl *pEvMonGridCtrl = this->GetEventMonsterGridPtr();
	if(NULL == pEvMonGridCtrl)
	{
		return;
	}

    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	if (pItem->iRow == 0)
	{
		return;
	}

	pEvMonGridCtrl->SetFocusCell(-1,-1);
	pEvMonGridCtrl->SetSelectedRange(pItem->iRow, 0, pItem->iRow, pEvMonGridCtrl->GetColumnCount()-1, TRUE, TRUE);
	pEvMonGridCtrl->SetFocusCell(pItem->iRow, pItem->iColumn);

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_EVENT_MONSTER_MENAGEMENT);
	CPoint point;
	GetCursorPos(&point);
	pContextMenu = muTemp.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CSCGridCtrl *CEventMonsterManagementDlg::GetEventMonsterGridPtr(void)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CEventMonsterManagementDlg::GetEventMonsterGridPtr
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CSCGridCtrl *CEventMonsterManagementDlg::GetEventMonsterGridPtr(void)
{
	return &m_eventMonserListGrid;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEventMonsterManagementDlg::InitEventMonsterGrid(void)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CEventMonsterManagementDlg::InitEventMonsterGrid() 추가
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEventMonsterManagementDlg::InitEventMonsterGrid(void)
{
	CSCGridCtrl *pEvMonGridCtrl = this->GetEventMonsterGridPtr();
	if(NULL == pEvMonGridCtrl)
	{
		return;
	}

	int	nColCount = 12;
	const char *arrColName[] = {"EventMonsterUID"
								, "ServerGroupID"
								, "StartDate"
								, "EndDate"
								, "MapIndex"
								, "ReqMinLevel"
								, "ReqMaxLevel"
								, "ExceptMonster"
								, "MonsterNum"
								, "MonsterCount"
								, "DelayTime(Sec)"
								, "Probability(Prob10K)"};
	pEvMonGridCtrl->InitGrid(nColCount, arrColName);
	pEvMonGridCtrl->SetEditable(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEventMonsterManagementDlg::InsertItemToEventMonsterGrid(SEVENT_MONSTER *i_pEvMonster)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CEventMonsterManagementDlg::InsertItemToEventMonsterGrid() 추가
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEventMonsterManagementDlg::InsertItemToEventMonsterGrid(SEVENT_MONSTER *i_pEvMonster)
{
	CSCGridCtrl *pEvMonGridCtrl = this->GetEventMonsterGridPtr();
	if(NULL == pEvMonGridCtrl)
	{
		return;
	}
	pEvMonGridCtrl->SetRowCount(pEvMonGridCtrl->GetRowCount()+1);

	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= pEvMonGridCtrl->GetRowCount()-1;
	Item.nFormat	= GRID_CELL_FORMAT;

	int nColIndex	= 0;
	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->EventMonsterUID);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->ServerGroupID);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText	= i_pEvMonster->StartDateTime.GetDateTimeString().GetBuffer();
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText	= i_pEvMonster->EndDateTime.GetDateTimeString().GetBuffer();
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonerMapIndex);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonerReqMinLevel);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonerReqMaxLevel);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonerExceptMonster);
	pEvMonGridCtrl->SetItem(&Item);

	MEX_MONSTER_INFO *pMexMonInfo = CAtumDBHelper::FindMEX_MONSTER_INFO(&m_vecMexMonsterInfo, i_pEvMonster->SummonMonsterNum);
	if(NULL == pMexMonInfo)
	{
		Item.col		= nColIndex++;
		Item.strText.Format("%d(Unknown Monster)", i_pEvMonster->SummonMonsterNum);
		pEvMonGridCtrl->SetItem(&Item);
	}
	else
	{
		Item.col		= nColIndex++;
		Item.strText.Format("%d(%s)", i_pEvMonster->SummonMonsterNum, pMexMonInfo->MonsterName);
		pEvMonGridCtrl->SetItem(&Item);
	}

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonMonsterCount);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonDelayTime);
	pEvMonGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.strText.Format("%d", i_pEvMonster->SummonProbability);
	pEvMonGridCtrl->SetItem(&Item);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEventMonsterManagementDlg::LoadEventMonsterFromDB(void)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CEventMonsterManagementDlg::LoadEventMonsterFromDB
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEventMonsterManagementDlg::LoadEventMonsterFromDB(void)
{
	this->InitEventMonsterGrid();	// 2008-04-17 by cmkwon, Grid 초기화

	m_eventMonsterList.clear();		// 2008-04-17 by cmkwon, List 초기화

	if(FALSE == CAtumDBHelper::LoadEventMonster(&m_eventMonsterList, m_pODBCStmt))
	{
		AfxMessageBox("[Error] LoadEventMonsterFromDB_ LoadEventMonster error !!");
		return;
	}

	mtvectSEVENT_MONSTER::iterator itr(m_eventMonsterList.begin());
	for(; itr != m_eventMonsterList.end(); itr++)
	{
		SEVENT_MONSTER *pEvMon = &*itr;

		this->InsertItemToEventMonsterGrid(pEvMon);
	}

	CSCGridCtrl *pEvMonGridCtrl = this->GetEventMonsterGridPtr();
	if(NULL == pEvMonGridCtrl)
	{
		return;
	}
	pEvMonGridCtrl->AutoSize();
	pEvMonGridCtrl->ExpandColumnsToFit();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEventMonsterManagementDlg::FindEventMonster(SEVENT_MONSTER *o_pEvMon, INT i_evMonUID)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CEventMonsterManagementDlg::FindEventMonster
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEventMonsterManagementDlg::FindEventMonster(SEVENT_MONSTER *o_pEvMon, INT i_evMonUID)
{
	mtvectSEVENT_MONSTER::iterator itr(m_eventMonsterList.begin());
	for(; itr != m_eventMonsterList.end(); itr++)
	{
		SEVENT_MONSTER *pEvMon = &*itr;
		if(i_evMonUID == pEvMon->EventMonsterUID)
		{
			*o_pEvMon	= *pEvMon;
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT CEventMonsterManagementDlg::GetSelectedEventMonsterUID(void)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CEventMonsterManagementDlg::GetSelectedEventMonsterUID
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CEventMonsterManagementDlg::GetSelectedEventMonsterUID(void)
{
	CSCGridCtrl *pEvMonGridCtrl = this->GetEventMonsterGridPtr();
	if(NULL == pEvMonGridCtrl)
	{
		return 0;
	}
	
	CCellID cellID = pEvMonGridCtrl->GetFocusCell();
	if(0 >= cellID.row)
	{
		return 0;
	}

	int nEventMonsterUID	= atoi(pEvMonGridCtrl->GetItemText(cellID.row, 0));
	return nEventMonsterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEventMonsterManagementDlg::DBQueryInsertEventMonster(SEVENT_MONSTER *i_pEvMon)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEventMonsterManagementDlg::DBQueryInsertEventMonster(SEVENT_MONSTER *i_pEvMon)
{
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("INSERT INTO ti_EventMonster VALUES(%d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d)"
// 		, i_pEvMon->ServerGroupID, i_pEvMon->StartDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), i_pEvMon->EndDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, i_pEvMon->SummonerMapIndex, i_pEvMon->SummonerReqMinLevel, i_pEvMon->SummonerReqMaxLevel, i_pEvMon->SummonerExceptMonster
// 		, i_pEvMon->SummonMonsterNum, i_pEvMon->SummonMonsterCount, i_pEvMon->SummonDelayTime, i_pEvMon->SummonProbability);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0051
//		, i_pEvMon->ServerGroupID, i_pEvMon->StartDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), i_pEvMon->EndDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//		, i_pEvMon->SummonerMapIndex, i_pEvMon->SummonerReqMinLevel, i_pEvMon->SummonerReqMaxLevel, i_pEvMon->SummonerExceptMonster
//		, i_pEvMon->SummonMonsterNum, i_pEvMon->SummonMonsterCount, i_pEvMon->SummonDelayTime, i_pEvMon->SummonProbability);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	char tmSDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	char tmEDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	i_pEvMon->StartDateTime.GetSQLDateTimeString(tmSDateTime, SIZE_MAX_SQL_DATETIME_STRING);
	i_pEvMon->EndDateTime.GetSQLDateTimeString(tmEDateTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[12] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
								SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->ServerGroupID), 0,				&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSDateTime, 0,	&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEDateTime, 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(i_pEvMon->SummonerMapIndex), 0,			&arrCB2[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pEvMon->SummonerReqMinLevel), 0,		&arrCB2[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pEvMon->SummonerReqMaxLevel), 0,		&arrCB2[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonerExceptMonster), 0,		&arrCB2[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonMonsterNum), 0,			&arrCB2[8]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonMonsterCount), 0,			&arrCB2[9]);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonDelayTime), 0,			&arrCB2[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonProbability), 0,			&arrCB2[11]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0051));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		AfxMessageBox("EventMonster insert error !!");
		return FALSE;
	}
	m_pODBCStmt->FreeStatement();		// cleanup

	this->LoadEventMonsterFromDB();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEventMonsterManagementDlg::DBQueryUpdateEventMonster(SEVENT_MONSTER *i_pEvMon)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEventMonsterManagementDlg::DBQueryUpdateEventMonster(SEVENT_MONSTER *i_pEvMon)
{
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("UPDATE ti_EventMonster SET ServerGroupID= %d, StartDateTime= '%s', EndDateTime= '%s', SummonerMapIndex= %d, SummonerReqMinLevel= %d, SummonerReqMaxLevel= %d, SummonerExceptMonster= %d, SummonMonsterNum= %d, SummonMonsterCount= %d, SummonDelayTime= %d, SummonProbability= %d WHERE EventMonsterUID = %d"
// 		, i_pEvMon->ServerGroupID, i_pEvMon->StartDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), i_pEvMon->EndDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, i_pEvMon->SummonerMapIndex, i_pEvMon->SummonerReqMinLevel, i_pEvMon->SummonerReqMaxLevel, i_pEvMon->SummonerExceptMonster
// 		, i_pEvMon->SummonMonsterNum, i_pEvMon->SummonMonsterCount, i_pEvMon->SummonDelayTime, i_pEvMon->SummonProbability, i_pEvMon->EventMonsterUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0052
//		, i_pEvMon->ServerGroupID, i_pEvMon->StartDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), i_pEvMon->EndDateTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//		, i_pEvMon->SummonerMapIndex, i_pEvMon->SummonerReqMinLevel, i_pEvMon->SummonerReqMaxLevel, i_pEvMon->SummonerExceptMonster
//		, i_pEvMon->SummonMonsterNum, i_pEvMon->SummonMonsterCount, i_pEvMon->SummonDelayTime, i_pEvMon->SummonProbability, i_pEvMon->EventMonsterUID);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	char tmSDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	char tmEDateTime[SIZE_MAX_SQL_DATETIME_STRING];
	i_pEvMon->StartDateTime.GetSQLDateTimeString(tmSDateTime, SIZE_MAX_SQL_DATETIME_STRING);
	i_pEvMon->EndDateTime.GetSQLDateTimeString(tmEDateTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[13] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,
								SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->ServerGroupID), 0,				&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSDateTime, 0,	&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmEDateTime, 0,	&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &(i_pEvMon->SummonerMapIndex), 0,			&arrCB2[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pEvMon->SummonerReqMinLevel), 0,		&arrCB2[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pEvMon->SummonerReqMaxLevel), 0,		&arrCB2[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonerExceptMonster), 0,		&arrCB2[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonMonsterNum), 0,			&arrCB2[8]);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonMonsterCount), 0,			&arrCB2[9]);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonDelayTime), 0,			&arrCB2[10]);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->SummonProbability), 0,			&arrCB2[11]);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_pEvMon->EventMonsterUID), 0,			&arrCB2[12]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0052));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		AfxMessageBox("EventMonster update error !!");
		return FALSE;
	}
	m_pODBCStmt->FreeStatement();		// cleanup

	this->LoadEventMonsterFromDB();
	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEventMonsterManagementDlg::DBQueryDeleteEventMonster(INT i_evMonUID)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - 
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEventMonsterManagementDlg::DBQueryDeleteEventMonster(INT i_evMonUID)
{
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szQuery.Format("DELETE ti_EventMonster WHERE EventMonsterUID = %d", i_evMonUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0053, i_evMonUID);
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &(i_evMonUID), 0,			&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0053));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		AfxMessageBox("EventMonster delete error !!");
		return FALSE;
	}
	m_pODBCStmt->FreeStatement();		// cleanup

	this->LoadEventMonsterFromDB();
	return TRUE;
}

void CEventMonsterManagementDlg::OnConmenuAddEventMonster() 
{
	// TODO: Add your command handler code here
	
	SEVENT_MONSTER tmEvMon;
	util::zero(&tmEvMon, sizeof(SEVENT_MONSTER));
	CSetEventMonsterDlg dlg(&tmEvMon, this);
	if(IDOK != dlg.DoModal())
	{
		return;
	}

	this->DBQueryInsertEventMonster(&tmEvMon);
}

void CEventMonsterManagementDlg::OnConmenuModifyEventMonster() 
{
	// TODO: Add your command handler code here
	INT nEventMonsterUID = this->GetSelectedEventMonsterUID();
	if(0 == nEventMonsterUID)
	{
		AfxMessageBox("Please, select valid item !!");
		return;
	}

	SEVENT_MONSTER tmEvMon;
	util::zero(&tmEvMon, sizeof(SEVENT_MONSTER));
	if(FALSE == this->FindEventMonster(&tmEvMon, nEventMonsterUID))
	{
		AfxMessageBox("Please, reload EventMonsters !!");
		return;
	}
	
	CSetEventMonsterDlg dlg(&tmEvMon, this);
	if(IDOK != dlg.DoModal())
	{
		return;
	}

	this->DBQueryUpdateEventMonster(&tmEvMon);
}

void CEventMonsterManagementDlg::OnConmenuDeleteEventMonster() 
{
	// TODO: Add your command handler code here
	INT nEventMonsterUID = this->GetSelectedEventMonsterUID();
	if(0 == nEventMonsterUID)
	{
		AfxMessageBox("Please, select valid item !!");
		return;
	}
	SEVENT_MONSTER tmEvMon;
	util::zero(&tmEvMon, sizeof(SEVENT_MONSTER));
	if(FALSE == this->FindEventMonster(&tmEvMon, nEventMonsterUID))
	{
		AfxMessageBox("Please, reload EventMonsters !!");
		return;
	}
	
	this->DBQueryDeleteEventMonster(nEventMonsterUID);
}

void CEventMonsterManagementDlg::OnBtnInsert() 
{
	// TODO: Add your control notification handler code here
	this->OnConmenuAddEventMonster();
}

void CEventMonsterManagementDlg::OnBtnReload() 
{
	// TODO: Add your control notification handler code here
	this->LoadEventMonsterFromDB();
}
