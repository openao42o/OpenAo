// MonsterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MonsterDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterDlg dialog


CMonsterDlg::CMonsterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); //메인 다이얼로그 포인트
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMonsterDetailDlg = NULL;
}


void CMonsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_CUSTOM_CURRENT, m_GridCurrent);
		DDX_Control(pDX, IDC_CUSTOM_UPDATE, m_GridUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterDlg)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SERVERUPDATE, OnButtonServerupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterDlg message handlers

BOOL CMonsterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-08 by cmkwon
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());

	// TODO: Add extra initialization here
	// DB 접근 초기화
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		EndDialog(-1);
//		return FALSE;
//	}

	//GridCtrl 초기화
	InitGridCurrent();
	InitGridUpdate();

	//현재 DB에 있는 값 불러온다.
	GetCurrentList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CMonsterDlg::InitGridCurrent()
{
	m_GridCurrent.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 36;

	m_GridCurrent.SetEditable(FALSE);
	m_GridCurrent.SetListMode(TRUE);
	m_GridCurrent.SetSingleRowSelection(TRUE);
	m_GridCurrent.EnableSelection(TRUE);
	m_GridCurrent.SetFrameFocusCell(FALSE);
	m_GridCurrent.SetTrackFocusCell(FALSE);
	m_GridCurrent.SetHeaderSort();

	m_GridCurrent.SetRowCount(m_nRows);
	m_GridCurrent.SetColumnCount(m_nCols);
	m_GridCurrent.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("UniqueNumber");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterName");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Level");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Experience");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HP");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Race");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Speed");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem01");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem02");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem03");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem04");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem05");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SizeForServer");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SizeForClient");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterForm");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackPattern");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MovePattern");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Belligerence");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Defense");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DefenseProbabillity");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AlphaBlending");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackObject");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackRange");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HPRecoveryTime");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HPRecoveryValue");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ScaleValue");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TextureIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuickTurnAngle");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuickTurnSpeed");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuickTurnTerm");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MPOption");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MPOptionParam1");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MPOptionParam2");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TurnAngle");
	m_GridCurrent.SetItem(&Item);

	// arrange grid
	m_GridCurrent.AutoSize();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCurrent.GetRowCount()-1, m_GridCurrent.GetColumnCount()-1);
	m_GridCurrent.ClearCells(tmpCellRange);
}

void CMonsterDlg::InitGridUpdate()
{
	m_GridUpdate.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 37;

	m_GridUpdate.SetEditable(FALSE);
	m_GridUpdate.SetListMode(TRUE);
	m_GridUpdate.SetSingleRowSelection(TRUE);
	m_GridUpdate.EnableSelection(TRUE);
	m_GridUpdate.SetFrameFocusCell(FALSE);
	m_GridUpdate.SetTrackFocusCell(FALSE);
	m_GridUpdate.SetHeaderSort();

	m_GridUpdate.SetRowCount(m_nRows);
	m_GridUpdate.SetColumnCount(m_nCols);
	m_GridUpdate.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("STATUS");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("UniqueNumber");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterName");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Level");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Experience");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HP");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Race");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Speed");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem01");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem02");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem03");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem04");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterItem05");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SizeForServer");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SizeForClient");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterForm");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackPattern");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MovePattern");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Belligerence");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Defense");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DefenseProbabillity");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AlphaBlending");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackObject");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackRange");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HPRecoveryTime");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HPRecoveryValue");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ScaleValue");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TextureIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuickTurnAngle");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuickTurnSpeed");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuickTurnTerm");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MPOption");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MPOptionParam1");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MPOptionParam2");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TurnAngle");
	m_GridUpdate.SetItem(&Item);

	// arrange grid
	m_GridUpdate.AutoSize();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridUpdate.GetRowCount()-1, m_GridUpdate.GetColumnCount()-1);
	m_GridUpdate.ClearCells(tmpCellRange);
}

void CMonsterDlg::GetCurrentList()
{
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	//DB에서 SELECT
	CString szSQLQuery;	
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select UniqueNumber, MonsterName, Level, Experience, HP, Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, MonsterItem04, MonsterItem05, SizeForServer, SizeForClient, MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, Range, TurnAngle  from ti_monster");
	szSQLQuery.Format(QUERY_080702_0111);

	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, cb10, cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20, cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28, cb29, cb30, cb31, cb32, cb33, cb34, cb35, cb36;

	_TI_MONSTER		st_Monster;
	
	memset(&st_Monster, 0, sizeof(st_Monster));

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &st_Monster.UniqueNumber, 0, &cb1);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, &st_Monster.MonsterName, 20, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_UTINYINT, &st_Monster.Level, 1, &cb3);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_ULONG, &st_Monster.Experience, 0, &cb4);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &st_Monster.HP, 0, &cb5);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_SSHORT, &st_Monster.Race, 0, &cb6);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &st_Monster.Speed, 1, &cb7);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_ULONG, &st_Monster.MonsterItem01, 0, &cb8);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_ULONG, &st_Monster.MonsterItem02, 0, &cb9);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_ULONG, &st_Monster.MonsterItem03, 0, &cb10);

	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_ULONG, &st_Monster.MonsterItem04, 0, &cb11);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_ULONG, &st_Monster.MonsterItem05, 0, &cb12);
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_ULONG, &st_Monster.SizeForServer, 0, &cb13);
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_ULONG, &st_Monster.SizeForClient, 0, &cb14);
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_SSHORT, &st_Monster.MonsterForm, 0, &cb15);
	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_UTINYINT, &st_Monster.AttackPattern, 1, &cb16);
	SQLBindCol(m_pODBCStmt->m_hstmt, 17, SQL_C_SSHORT, &st_Monster.MovePattern, 0, &cb17);
	SQLBindCol(m_pODBCStmt->m_hstmt, 18, SQL_C_UTINYINT, &st_Monster.Belligerence, 1, &cb18);
	SQLBindCol(m_pODBCStmt->m_hstmt, 19, SQL_C_FLOAT, &st_Monster.Defense, 1, &cb19);				// 2010-07-19 by dhjin, 확률 수식 변경

	SQLBindCol(m_pODBCStmt->m_hstmt, 20, SQL_C_FLOAT, &st_Monster.DefenseProbability, 1, &cb20);	// 2010-07-19 by dhjin, 확률 수식 변경
	SQLBindCol(m_pODBCStmt->m_hstmt, 21, SQL_C_UTINYINT, &st_Monster.AlphaBlending, 1, &cb21);
	SQLBindCol(m_pODBCStmt->m_hstmt, 22, SQL_C_UTINYINT, &st_Monster.AttackObject, 1, &cb22);
	SQLBindCol(m_pODBCStmt->m_hstmt, 23, SQL_C_SSHORT, &st_Monster.AttackRange, 0, &cb23);
	SQLBindCol(m_pODBCStmt->m_hstmt, 24, SQL_C_SSHORT, &st_Monster.HPRecoveryTime, 0, &cb24);
	SQLBindCol(m_pODBCStmt->m_hstmt, 25, SQL_C_SSHORT, &st_Monster.HPRecoveryValue, 0, &cb25);
	SQLBindCol(m_pODBCStmt->m_hstmt, 26, SQL_C_SSHORT, &st_Monster.ScaleValue, 0, &cb26);
	SQLBindCol(m_pODBCStmt->m_hstmt, 27, SQL_C_UTINYINT, &st_Monster.TextureIndex, 1, &cb27);
	SQLBindCol(m_pODBCStmt->m_hstmt, 28, SQL_C_ULONG, &st_Monster.SourceIndex, 0, &cb28);
	SQLBindCol(m_pODBCStmt->m_hstmt, 29, SQL_C_UTINYINT, &st_Monster.QuickTurnAngle, 1, &cb29);

	SQLBindCol(m_pODBCStmt->m_hstmt, 30, SQL_C_UTINYINT, &st_Monster.QuickTurnSpeed, 1, &cb30);
	SQLBindCol(m_pODBCStmt->m_hstmt, 31, SQL_C_SSHORT, &st_Monster.QuickTurnTerm, 0, &cb31);
	SQLBindCol(m_pODBCStmt->m_hstmt, 32, SQL_C_UTINYINT, &st_Monster.MPOption, 1, &cb32);
	SQLBindCol(m_pODBCStmt->m_hstmt, 33, SQL_C_UTINYINT, &st_Monster.MPOptionParam1, 1, &cb33);
	SQLBindCol(m_pODBCStmt->m_hstmt, 34, SQL_C_UTINYINT, &st_Monster.MPOptionParam2, 1, &cb34);
	SQLBindCol(m_pODBCStmt->m_hstmt, 35, SQL_C_SSHORT, &st_Monster.Range, 0, &cb35);
	SQLBindCol(m_pODBCStmt->m_hstmt, 36, SQL_C_FLOAT, &st_Monster.TurnAngle, 0, &cb36);

	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	int nCount = 0;
	m_GridCurrent.SetRowCount(nCount+1);
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridCurrent.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		InsertItemToGridCurrent(&st_Monster, nCount+1);

		nCount++;
		memset(&st_Monster, 0, sizeof(st_Monster));
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridCurrent.AutoSize();
}

void CMonsterDlg::InsertItemToGridCurrent(_TI_MONSTER *i_pTi_Monster, int i_nRow)
{

	// 칼럼 만들기
	int m_nCols = 0;

	// 컬럼에 값 넣기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->UniqueNumber);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_Monster->MonsterName);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Level);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Experience);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->HP);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Race);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Speed);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MonsterItem01);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MonsterItem02);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MonsterItem03);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////10 items

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MonsterItem04);
	m_GridCurrent.SetItem(&Item);
	

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MonsterItem05);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->SizeForServer);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->SizeForClient);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MonsterForm);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->AttackPattern);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MovePattern);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Belligerence);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Defense);
	m_GridCurrent.SetItem(&Item);

	/////////////////////////////////////////////////20items
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->DefenseProbability);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->AlphaBlending);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->AttackObject);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->AttackRange);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->HPRecoveryTime);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->HPRecoveryValue);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->ScaleValue);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->TextureIndex);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->SourceIndex);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->QuickTurnAngle);
	m_GridCurrent.SetItem(&Item);

	/////////////////////////////////////////////////30items
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->QuickTurnSpeed);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->QuickTurnTerm);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MPOption);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MPOptionParam1);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->MPOptionParam2);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Monster->Range);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%f", i_pTi_Monster->TurnAngle);
	m_GridCurrent.SetItem(&Item);
}

void CMonsterDlg::OnButtonView() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridCurrent.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	if (m_pMonsterDetailDlg != NULL)
	{
		util::del(m_pMonsterDetailDlg);
	}

	if (m_pMonsterDetailDlg == NULL)
	{
		m_pMonsterDetailDlg = new CMonsterDetailDlg(this);
		m_pMonsterDetailDlg->Create(IDD_DIALOG_MONSTERDETAIL, this);
		m_pMonsterDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pMonsterDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
		m_pMonsterDetailDlg->m_ctrlEditUniqueNumber.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pMonsterDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDlg::SetDetailDialog(int row)
{
	//선택된 셀을 바탕으로 값을 설정한다.
	m_pMonsterDetailDlg->m_valueEditUniqueNumber = m_GridCurrent.GetItemText(row,0);
	m_pMonsterDetailDlg->m_valueEditMonsterName = m_GridCurrent.GetItemText(row,1);
	m_pMonsterDetailDlg->m_valueEditLevel = m_GridCurrent.GetItemText(row,2);
	m_pMonsterDetailDlg->m_valueEditExperience = m_GridCurrent.GetItemText(row,3);
	m_pMonsterDetailDlg->m_valueEditHP = m_GridCurrent.GetItemText(row,4);
	m_pMonsterDetailDlg->m_ctrlComboRace.SetWindowText(CAtumSJ::GetRaceString(atoi(m_GridCurrent.GetItemText(row,5))));
	m_pMonsterDetailDlg->m_valueEditSpeed = m_GridCurrent.GetItemText(row,6);
	m_pMonsterDetailDlg->m_ctrlComboMonsterItem01.SetWindowText(m_GridCurrent.GetItemText(row,7)); 
	m_pMonsterDetailDlg->m_ctrlComboMonsterItem02.SetWindowText(m_GridCurrent.GetItemText(row,8)); 
	m_pMonsterDetailDlg->m_ctrlComboMonsterItem03.SetWindowText(m_GridCurrent.GetItemText(row,9)); 
	m_pMonsterDetailDlg->m_ctrlComboMonsterItem04.SetWindowText(m_GridCurrent.GetItemText(row,10)); 
	m_pMonsterDetailDlg->m_ctrlComboMonsterItem05.SetWindowText(m_GridCurrent.GetItemText(row,11));

	m_pMonsterDetailDlg->m_valueEditSizeForServer = m_GridCurrent.GetItemText(row,12);
	m_pMonsterDetailDlg->m_valueEditSizeForClient = m_GridCurrent.GetItemText(row,13);

	int tempValue;
	tempValue = atoi(m_GridCurrent.GetItemText(row,14));
	if(tempValue == FORM_FLYING_RIGHT)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_FLYING_RIGHT");
	else if(tempValue == FORM_FLYING_COPTER)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_FLYING_COPTER");
	else if(tempValue == FORM_GROUND_MOVE)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_GROUND_MOVE");
	else if(tempValue == FORM_FLYINGandGROUND_RIGHT)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_FLYINGandGROUND_RIGHT");
	else if(tempValue == FORM_FLYINGandGROUND_COPTER)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_FLYINGandGROUND_COPTER");
	else if(tempValue == FORM_SWIMMINGFLYING_RIGHT)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_SWIMMINGFLYING_RIGHT");
	else if(tempValue == FORM_SWIMMINGFLYING_COPTER)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_SWIMMINGFLYING_COPTER");
	else if(tempValue == FORM_OBJECT_STOP)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_OBJECT_STOP");
	else if(tempValue == FORM_OBJECT_PLANE_ROTATE)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_OBJECT_PLANE_ROTATE");
	else if(tempValue == FORM_OBJECT_CANNON)
		m_pMonsterDetailDlg->m_ctrlComboMonsterForm.SetWindowText("FORM_OBJECT_CANNON");

	tempValue = atoi(m_GridCurrent.GetItemText(row,15));
	if(tempValue == ATTPAT_NORMAL)
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_NORMAL");
	else if(tempValue == ATTPAT_SELFDESTRUCTION)
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_SELFDESTRUCTION");
	else if(tempValue == ATTPAT_RETREAT)
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_RETREAT");
	else if(tempValue == ATTPAT_HITANDRUN)
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_HITANDRUN");
	else if(tempValue == ATTPAT_ONEATATTCK)
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_ONEATATTCK");
	else if(tempValue == ATTPAT_PK)
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_PK");
	else
		m_pMonsterDetailDlg->m_ctrlComboAttackPattern.SetWindowText("ATTPAT_NORMAL");

	tempValue = atoi(m_GridCurrent.GetItemText(row,16));
	if(tempValue == MOVPAT_STOP)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_STOP");
	else if(tempValue == MOVPAT_STRAIGHT)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_STRAIGHT");
	else if(tempValue == MOVPAT_RIGHTANDLEFT)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_RIGHTANDLEFT");
	else if(tempValue == MOVPAT_8RIGHTANDLEFT)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_8RIGHTANDLEFT");
	else if(tempValue == MOVPAT_PATROL)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_PATROL");
	else if(tempValue == MOVPAT_PARTY)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_PARTY");
	else if(tempValue == MOVPAT_UPANDDOWN)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_UPANDDOWN");
	else if(tempValue == MOVPAT_OBLIQUE)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_OBLIQUE");
	else if(tempValue == MOVPAT_OBLIQUEANDCURVE)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_OBLIQUEANDCURVE");
	else if(tempValue == MOVPAT_TELEPORT)
		m_pMonsterDetailDlg->m_ctrlComboMovePattern.SetWindowText("MOVPAT_TELEPORT");

	tempValue = atoi(m_GridCurrent.GetItemText(row,17));
	if(tempValue == BELL_ATATTACK)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_ATATTACK");
	else if(tempValue == BELL_DEFENSE)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_DEFENSE");
	else if(tempValue == BELL_RETREAT)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_RETREAT");
	else if(tempValue == BELL_TAGETATATTACK)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_TAGETATATTACK");
	else if(tempValue == BELL_NOTATTACK)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_NOTATTACK");
	else if(tempValue == BELL_INFLUENCE_VCN)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_INFLUENCE_VCN");
	else if(tempValue == BELL_INFLUENCE_ANI)
		m_pMonsterDetailDlg->m_ctrlComboBelligerence.SetWindowText("BELL_INFLUENCE_ANI");

	m_pMonsterDetailDlg->m_valueEditDefense = m_GridCurrent.GetItemText(row,18);
	m_pMonsterDetailDlg->m_valueEditDefenseProbability = m_GridCurrent.GetItemText(row,19);
	m_pMonsterDetailDlg->m_valueEditAlphaBlending = m_GridCurrent.GetItemText(row,20);

//	m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText(m_GridCurrent.GetItemText(row,21));
	tempValue = atoi(m_GridCurrent.GetItemText(row,21));
	if(tempValue == ATTACKOBJ_CLOSERANGE)
		m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText("ATTACKOBJ_CLOSERANGE");
	else if(tempValue == ATTACKOBJ_FIRSTATTACK)
		m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText("ATTACKOBJ_FIRSTATTACK");
	else if(tempValue == ATTACKOBJ_LOWHP)
		m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText("ATTACKOBJ_LOWHP");
	else if(tempValue == ATTACKOBJ_HIGHHP)
		m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText("ATTACKOBJ_HIGHHP");
	else if(tempValue == ATTACKOBJ_SAMERACE)
		m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText("ATTACKOBJ_SAMERACE");
	else if(tempValue == ATTACKOBJ_RANDOM)
		m_pMonsterDetailDlg->m_ctrlComboAttackObject.SetWindowText("ATTACKOBJ_RANDOM");

	tempValue = atoi(m_GridCurrent.GetItemText(row,16));

	m_pMonsterDetailDlg->m_valueEditAttackRange = m_GridCurrent.GetItemText(row,22);
	m_pMonsterDetailDlg->m_valueEditHPRecoveryTime = m_GridCurrent.GetItemText(row,23);
	m_pMonsterDetailDlg->m_valueEditHPRecoveryValue = m_GridCurrent.GetItemText(row,24);
	m_pMonsterDetailDlg->m_valueEditScaleValue = m_GridCurrent.GetItemText(row,25);
	m_pMonsterDetailDlg->m_valueEditTextureIndex = m_GridCurrent.GetItemText(row,26);
	m_pMonsterDetailDlg->m_valueEditSourceIndex = m_GridCurrent.GetItemText(row,27);
	m_pMonsterDetailDlg->m_valueEditQuickTurnAngle = m_GridCurrent.GetItemText(row,28);
	m_pMonsterDetailDlg->m_valueEditQuickTurnSpeed = m_GridCurrent.GetItemText(row,29);
	m_pMonsterDetailDlg->m_valueEditQuickTurnTerm = m_GridCurrent.GetItemText(row,30);

	unsigned char tempMPOption = atoi((char*)(LPCTSTR)m_GridCurrent.GetItemText(row,31));
	if(COMPARE_MPOPTION_BIT(tempMPOption, MPOPTION_BIT_DEAD_FALL))
		m_pMonsterDetailDlg->m_valueCheckDeadFall = TRUE;
	if(COMPARE_MPOPTION_BIT(tempMPOption, MPOPTION_BIT_MOVE_PARTY))
		m_pMonsterDetailDlg->m_valueCheckMoveParty = TRUE;
	if(COMPARE_MPOPTION_BIT(tempMPOption, MPOPTION_BIT_PATTERN_MONSTER))
		m_pMonsterDetailDlg->m_valueCheckPatternMonster = TRUE;
	if(COMPARE_MPOPTION_BIT(tempMPOption, MPOPTION_BIT_BOSS_MONSTER))
		m_pMonsterDetailDlg->m_valueCheckBossMonster = TRUE;
	if(COMPARE_MPOPTION_BIT(tempMPOption, MPOPTION_BIT_AUTO_DESTROY))
		m_pMonsterDetailDlg->m_valueCheckAutoDestroy = TRUE;

	m_pMonsterDetailDlg->m_valueEditMPOptionParam1 = m_GridCurrent.GetItemText(row,32);
	m_pMonsterDetailDlg->m_valueEditMPOptionParam2 = m_GridCurrent.GetItemText(row,33);
	m_pMonsterDetailDlg->m_valueEditRange = m_GridCurrent.GetItemText(row,34);
	m_pMonsterDetailDlg->m_valueEditTurnAngle = m_GridCurrent.GetItemText(row,35);

	m_pMonsterDetailDlg->UpdateData(FALSE);
}

void CMonsterDlg::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here
	if (m_pMonsterDetailDlg != NULL)
	{
		util::del(m_pMonsterDetailDlg);
	}

	if (m_pMonsterDetailDlg == NULL)
	{
		m_pMonsterDetailDlg = new CMonsterDetailDlg(this);
		m_pMonsterDetailDlg->Create(IDD_DIALOG_MONSTERDETAIL, this);
		m_pMonsterDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pMonsterDetailDlg->m_ctrlEditUniqueNumber.EnableWindow(TRUE);
	}

	m_pMonsterDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterDlg::OnButtonUpdate() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridCurrent.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	if (m_pMonsterDetailDlg != NULL)
	{
		util::del(m_pMonsterDetailDlg);
	}

	if (m_pMonsterDetailDlg == NULL)
	{
		m_pMonsterDetailDlg = new CMonsterDetailDlg(this);
		m_pMonsterDetailDlg->Create(IDD_DIALOG_MONSTERDETAIL, this);
		m_pMonsterDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
		m_pMonsterDetailDlg->m_ctrlEditUniqueNumber.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pMonsterDetailDlg->ShowWindow(SW_SHOW);

	m_pMonsterDetailDlg->m_nSelectRow = tmpCellID.row;

//	m_GridCurrent.DeleteRow(tmpCellID.row);
	m_GridCurrent.Refresh();
}

void CMonsterDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridCurrent.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	ChangeGridDate(&m_GridCurrent, &m_GridUpdate, 1, tmpCellID.row);

	m_GridCurrent.DeleteRow(tmpCellID.row);
	m_GridCurrent.Refresh();
}

void CMonsterDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	m_GridCurrent.UpdateWindow();
}

void CMonsterDlg::ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row)
{
	// 칼럼 만들기
	newGrid->SetRowCount(newGrid->GetRowCount()+1);

	if(type == 1) //Delete 버튼을 눌렀을때
	{
		newGrid->SetItemText(newGrid->GetRowCount()-1, 0, "DELETE");
		for(int m_nCols = 0; m_nCols<41;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols+1, oriGrid->GetItemText(row,m_nCols));
	}
	else if(type == 2) //Delete 버튼을 눌렀을때
	{
		for(int m_nCols = 0; m_nCols<41;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols, oriGrid->GetItemText(row,m_nCols+1));
	}

	newGrid->Refresh();
}

void CMonsterDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	//현재 선택된 row를 알아온다.
	CCellID tmpCellID = m_GridUpdate.GetFocusCell();

	if(tmpCellID.col == -1 || tmpCellID.row == -1)
	{
		// GridCtrl에서 row가 선택되지 않았다.
		AfxMessageBox("Please Select 1 Row");
		return;
	}

	ChangeGridDate(&m_GridUpdate, &m_GridCurrent, 2, tmpCellID.row);

	m_GridUpdate.DeleteRow(tmpCellID.row);
	m_GridUpdate.Refresh();
}

void CMonsterDlg::OnButtonServerupdate() 
{
	// TODO: Add your control notification handler code here
	//DB에 값을 집어 넣는다.
	CString szQuery;
	BOOL bRet= 0;
	int row = m_GridUpdate.GetRowCount();
	for(int i = 0; i< row; i++)
	{
		if(m_GridUpdate.GetItemText(1, 0) == "DELETE")
		{
			//DELETE ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("delete ti_monster where uniquenumber = %s", m_GridUpdate.GetItemText(1, 1));
			szQuery.Format(QUERY_080702_0112, m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Delete 1 Row Data");
				m_pODBCStmt->FreeStatement();
				//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "UPDATE")
		{

			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Update ti_monster set MonsterName= '%s', Level= %s, Experience= %s, HP= %s, Race= %s, Speed= %s, MonsterItem01= %s, MonsterItem02= %s, MonsterItem03= %s, MonsterItem04= %s, MonsterItem05= %s, SizeForServer= %s, SizeForClient= %s, MonsterForm= %s, AttackPattern= %s, MovePattern= %s, Belligerence= %s, Defense= %s, DefenceProbability= %s, AlphaBlending= %s, AttackObject= %s, AttackRange= %s, HPRecoveryTime= %s, HPRecoveryValue= %s, ScaleValue= %s, TextureIndex= %s, SourceIndex= %s, QuickTurnAngle= %s, QuickSpeed= %s, QuickTurnTerm= %s, MPOption= %s, MPOptionParam1= %s, MPOptionParam2= %s, Range= %s, TurnAngle= %s where uniquenumber = %s"
			szQuery.Format(QUERY_080702_0113
				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
				, m_GridUpdate.GetItemText(1, 8), m_GridUpdate.GetItemText(1, 9)
				, m_GridUpdate.GetItemText(1, 10), m_GridUpdate.GetItemText(1, 11)
				, m_GridUpdate.GetItemText(1, 12), m_GridUpdate.GetItemText(1, 13)
				, m_GridUpdate.GetItemText(1, 14), m_GridUpdate.GetItemText(1, 15)
				, m_GridUpdate.GetItemText(1, 16), m_GridUpdate.GetItemText(1, 17)
				, m_GridUpdate.GetItemText(1, 18), m_GridUpdate.GetItemText(1, 19)
				, m_GridUpdate.GetItemText(1, 20), m_GridUpdate.GetItemText(1, 21)
				, m_GridUpdate.GetItemText(1, 22), m_GridUpdate.GetItemText(1, 23)
				, m_GridUpdate.GetItemText(1, 24), m_GridUpdate.GetItemText(1, 25)
				, m_GridUpdate.GetItemText(1, 26), m_GridUpdate.GetItemText(1, 27)
				, m_GridUpdate.GetItemText(1, 28), m_GridUpdate.GetItemText(1, 29)
				, m_GridUpdate.GetItemText(1, 30), m_GridUpdate.GetItemText(1, 31)
				, m_GridUpdate.GetItemText(1, 32), m_GridUpdate.GetItemText(1, 33)
				, m_GridUpdate.GetItemText(1, 34), m_GridUpdate.GetItemText(1, 35)
				, m_GridUpdate.GetItemText(1, 36)
				, m_GridUpdate.GetItemText(1, 1)
				);
			TRACE(m_GridUpdate.GetItemText(1, 32));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
				//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "INSERT")
		{
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Insert into ti_monster(UniqueNumber, MonsterName, Level, Experience, HP, Race, Speed, MonsterItem01, MonsterItem02, MonsterItem03, MonsterItem04, MonsterItem05, SizeForServer, SizeForClient, MonsterForm, AttackPattern, MovePattern, Belligerence, Defense, DefenceProbability, AlphaBlending, AttackObject, AttackRange, HPRecoveryTime, HPRecoveryValue, ScaleValue, TextureIndex, SourceIndex, QuickTurnAngle, QuickSpeed, QuickTurnTerm, MPOption, MPOptionParam1, MPOptionParam2, Range, TurnAngle) values ( %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"				
			szQuery.Format(QUERY_080702_0114				
				, m_GridUpdate.GetItemText(1, 1)
				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
				, m_GridUpdate.GetItemText(1, 8), m_GridUpdate.GetItemText(1, 9)
				, m_GridUpdate.GetItemText(1, 10), m_GridUpdate.GetItemText(1, 11)
				, m_GridUpdate.GetItemText(1, 12), m_GridUpdate.GetItemText(1, 13)
				, m_GridUpdate.GetItemText(1, 14), m_GridUpdate.GetItemText(1, 15)
				, m_GridUpdate.GetItemText(1, 16), m_GridUpdate.GetItemText(1, 17)
				, m_GridUpdate.GetItemText(1, 18), m_GridUpdate.GetItemText(1, 19)
				, m_GridUpdate.GetItemText(1, 20), m_GridUpdate.GetItemText(1, 21)
				, m_GridUpdate.GetItemText(1, 22), m_GridUpdate.GetItemText(1, 23)
				, m_GridUpdate.GetItemText(1, 24), m_GridUpdate.GetItemText(1, 25)
				, m_GridUpdate.GetItemText(1, 26), m_GridUpdate.GetItemText(1, 27)
				, m_GridUpdate.GetItemText(1, 28), m_GridUpdate.GetItemText(1, 29)
				, m_GridUpdate.GetItemText(1, 30), m_GridUpdate.GetItemText(1, 31)
				, m_GridUpdate.GetItemText(1, 32), m_GridUpdate.GetItemText(1, 33)
				, m_GridUpdate.GetItemText(1, 34), m_GridUpdate.GetItemText(1, 35)
				, m_GridUpdate.GetItemText(1, 36), m_GridUpdate.GetItemText(1, 37)
				);
			TRACE(m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
				//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		
	}
	// cleanup
	m_pODBCStmt->FreeStatement();
	GetCurrentList();
}

CMonsterDlg::~CMonsterDlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
	util::del(m_pMonsterDetailDlg);
	
}