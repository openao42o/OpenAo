// MonsterAttackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MonsterAttackDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterAttackDlg dialog


CMonsterAttackDlg::CMonsterAttackDlg(MONSTER_ITEMLIST_TYPE i_enItemListTy, int i_arrItemNumList[ARRAY_SIZE_MONSTER_ITEM]/*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterAttackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterAttackDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); //메인 다이얼로그 포인터 얻기
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMonsterAttackDetailDlg = NULL; 

	m_enMonsterItemListType		= i_enItemListTy;		
	util::zero(m_arrMonsterItemNumList, sizeof(m_arrMonsterItemNumList[0])*ARRAY_SIZE_MONSTER_ITEM);
	if(MONSTER_ITEMLIST_TYPE_ANYMONSTER == m_enMonsterItemListType
		&& i_arrItemNumList)
	{
		memcpy(m_arrMonsterItemNumList, i_arrItemNumList, sizeof(m_arrMonsterItemNumList[0])*ARRAY_SIZE_MONSTER_ITEM);
	}
}

CMonsterAttackDlg::~CMonsterAttackDlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
	util::del(m_pMonsterAttackDetailDlg);
}

void CMonsterAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterAttackDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_CUSTOM_CURRENT, m_GridCurrent);
		DDX_Control(pDX, IDC_CUSTOM_UPDATE, m_GridUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterAttackDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterAttackDlg)
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
// CMonsterAttackDlg message handlers

BOOL CMonsterAttackDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-08 by cmkwon
	GetDlgItem(IDC_BUTTON_INSERT)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	
	// TODO: Add extra initialization here
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

void CMonsterAttackDlg::InitGridCurrent()
{
	m_GridCurrent.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 28;

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
	Item.strText.Format("ItemNum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Itemname");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMin");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMax");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqRace");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMinLevel");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Hitrate");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SpeedPenalty");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Charging");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RangeAngle");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Shotnum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Multinum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Attacktime");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqSP");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Summonmonster");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Orbittype");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Kind");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CameraPattern");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridCurrent.SetItem(&Item);

// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
// 	Item.col = m_nCols++;
// 	Item.strText.Format("DestParameter1");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("ParameterValue1");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("DestParameter2");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("ParameterValue2");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("DestParameter3");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("ParameterValue3");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("DestParameter4");
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("ParameterValue4");
// 	m_GridCurrent.SetItem(&Item);
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		Item.col = m_nCols++;
		Item.strText.Format("DestParameter%d", i+1);
		m_GridCurrent.SetItem(&Item);
		
		Item.col = m_nCols++;
		Item.strText.Format("ParameterValue%d", i+1);
		m_GridCurrent.SetItem(&Item);		
	}

	// arrange grid
	m_GridCurrent.AutoSize();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCurrent.GetRowCount()-1, m_GridCurrent.GetColumnCount()-1);
	m_GridCurrent.ClearCells(tmpCellRange);

}

void CMonsterAttackDlg::InitGridUpdate()
{
	m_GridUpdate.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 29;

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
	Item.strText.Format("ItemNum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Itemname");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMin");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMax");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqRace");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMinLevel");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Hitrate");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SpeedPenalty");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Charging");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RangeAngle");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Shotnum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Multinum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Attacktime");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqSP");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Summonmonster");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Orbittype");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Kind");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CameraPattern");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DestParameter1");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ParameterValue1");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DestParameter2");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ParameterValue2");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DestParameter3");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ParameterValue3");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DestParameter4");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ParameterValue4");
	m_GridUpdate.SetItem(&Item);


	// arrange grid
	m_GridUpdate.AutoSize();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridUpdate.GetRowCount()-1, m_GridUpdate.GetColumnCount()-1);
	m_GridUpdate.ClearCells(tmpCellRange);
}

void CMonsterAttackDlg::GetCurrentList(void)
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	GV_ITEM Item;
// 	Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 	Item.row = 1;
// 	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
// 	
// 	//DB에서 값 SELECT
// 	CString szSQLQuery;
// 	if(MONSTER_ITEMLIST_TYPE_ALL_MONSTER == m_enMonsterItemListType)
// 	{
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, CameraPattern, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%'");
// 		szSQLQuery.Format(QUERY_080702_0100);
// 	}
// 	else
// 	{
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, CameraPattern, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%d, %d, %d, %d, %d, %d)",
// 		//	m_arrMonsterItemNumList[0],m_arrMonsterItemNumList[1],m_arrMonsterItemNumList[2]
// 		//	,m_arrMonsterItemNumList[3],m_arrMonsterItemNumList[4], m_arrMonsterItemNumList[5]);
// 		szSQLQuery.Format(QUERY_080702_0101,
// 			m_arrMonsterItemNumList[0],m_arrMonsterItemNumList[1],m_arrMonsterItemNumList[2]
// 			,m_arrMonsterItemNumList[3],m_arrMonsterItemNumList[4], m_arrMonsterItemNumList[5]);
// 	}
// 
// 	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, cb10, cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20, cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28;
// 
// 	_TI_ITEM		st_Item;
// 	
// 	memset(&st_Item, 0, sizeof(st_Item));
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &st_Item.ItemNum, 0, &cb1);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, &st_Item.ItemName, 40, &cb2);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &st_Item.AbilityMin, 0, &cb3);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_FLOAT, &st_Item.AbilityMax, 0, &cb4);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &st_Item.ReqRace, 0, &cb5);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_UTINYINT, &st_Item.ReqMinLevel, 0, &cb6);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &st_Item.HitRate, 0, &cb7);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_UTINYINT, &st_Item.SpeedPenalty, 0, &cb8);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_SSHORT, &st_Item.Range, 0, &cb9);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_SSHORT, &st_Item.Charging, 0, &cb10);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_FLOAT, &st_Item.RangeAngle, 0, &cb11);
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_UTINYINT, &st_Item.ShotNum, 0, &cb12);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_UTINYINT, &st_Item.MultiNum, 0, &cb13);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_SSHORT, &st_Item.AttackTime, 0, &cb14);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_UTINYINT, &st_Item.ReqSP, 0, &cb15);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_ULONG, &st_Item.SummonMonster, 0, &cb16);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 17, SQL_C_UTINYINT, &st_Item.OrbitType, 0, &cb17);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 18, SQL_C_FLOAT, &st_Item.CameraPattern, 0, &cb18);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 19, SQL_C_UTINYINT, &st_Item.Kind, 0, &cb19);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 20, SQL_C_ULONG, &st_Item.SourceIndex, 0, &cb20);
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 21, SQL_C_UTINYINT, &st_Item.DestParameter1, 0, &cb21);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 22, SQL_C_FLOAT, &st_Item.ParameterValue1, 0, &cb22);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 23, SQL_C_UTINYINT, &st_Item.DestParameter2, 0, &cb23);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 24, SQL_C_FLOAT, &st_Item.ParameterValue2, 0, &cb24);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 25, SQL_C_UTINYINT, &st_Item.DestParameter3, 0, &cb25);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 26, SQL_C_FLOAT, &st_Item.ParameterValue3, 0, &cb26);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 27, SQL_C_UTINYINT, &st_Item.DestParameter4, 0, &cb27);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 28, SQL_C_FLOAT, &st_Item.ParameterValue4, 0, &cb28);
// 	
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return;
// 	}
// 
// 	int nCount = 0;
// 	m_GridCurrent.SetRowCount(nCount+1);
// 	// DB에 값이 없을때까지 loop를 돈다
// 	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
// 	{
// 		m_GridCurrent.SetRowCount(nCount+2);
// 
// 		//select된 값을 GridDetail GridCtrl에 넣어준다.
// 		InsertItemToGridCurrent(&st_Item, nCount+1);
// 
// 		nCount++;
// 		memset(&st_Item, 0, sizeof(st_Item));
// 	}
// 	
// 	// clean up
// 	m_pODBCStmt->FreeStatement();
// 
// 	m_GridCurrent.AutoSize();
}

void CMonsterAttackDlg::InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow)
{
	// 칼럼 만들기
	int m_nCols = 0;

	// 컬럼에 값 넣기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ItemNum);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_Item->ItemName);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%f", i_pTi_Item->AbilityMin);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%f", i_pTi_Item->AbilityMax);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqRace);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqMinLevel);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->HitRate);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SpeedPenalty);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Range);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Charging);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%f", i_pTi_Item->RangeAngle);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ShotNum);
	m_GridCurrent.SetItem(&Item);
	

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->MultiNum);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->AttackTime);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqSP);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SummonMonster);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->OrbitType);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Kind);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pTi_Item->CameraPattern);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SourceIndex);
	m_GridCurrent.SetItem(&Item);

// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%d", i_pTi_Item->DestParameter1);
// 	m_GridCurrent.SetItem(&Item);
// 	
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%.1f", i_pTi_Item->ParameterValue1);
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%d", i_pTi_Item->DestParameter2);
// 	m_GridCurrent.SetItem(&Item);
// 	
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%.1f", i_pTi_Item->ParameterValue2);
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%d", i_pTi_Item->DestParameter3);
// 	m_GridCurrent.SetItem(&Item);
// 	
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%.1f", i_pTi_Item->ParameterValue3);
// 	m_GridCurrent.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%d", i_pTi_Item->DestParameter4);
// 	m_GridCurrent.SetItem(&Item);
// 	
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%.1f", i_pTi_Item->ParameterValue4);
// 	m_GridCurrent.SetItem(&Item);
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		Item.col = m_nCols++;
		Item.strText.Format("%d", i_pTi_Item->ArrDestParameter[i]);
		m_GridCurrent.SetItem(&Item);
		
		Item.col = m_nCols++;
		Item.strText.Format("%.1f", i_pTi_Item->ArrParameterValue[i]);
		m_GridCurrent.SetItem(&Item);
	}
}

void CMonsterAttackDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CMonsterAttackDlg::OnButtonView() 
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

	if (m_pMonsterAttackDetailDlg != NULL)
	{
		util::del(m_pMonsterAttackDetailDlg);
	}

	if (m_pMonsterAttackDetailDlg == NULL)
	{
		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}

	SetDetailDialog(tmpCellID.row); //자식 다이얼로그 컨트롤에 값 셋팅
	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterAttackDlg::SetDetailDialog(int row)
{
	//선택된 셀을 바탕으로 값을 설정한다.
	m_pMonsterAttackDetailDlg->m_valueEditItemNum = m_GridCurrent.GetItemText(row,0);
	m_pMonsterAttackDetailDlg->m_valueEditItemName = m_GridCurrent.GetItemText(row,1);
	m_pMonsterAttackDetailDlg->m_valueEditAbilityMin = m_GridCurrent.GetItemText(row,2);
	m_pMonsterAttackDetailDlg->m_valueEditAbilityMax = m_GridCurrent.GetItemText(row,3);
	m_pMonsterAttackDetailDlg->m_ctrlComboRace.SetWindowText(CAtumSJ::GetRaceString(atoi(m_GridCurrent.GetItemText(row,4))));
	m_pMonsterAttackDetailDlg->m_valueEditReqMinLevel = m_GridCurrent.GetItemText(row,5);
	m_pMonsterAttackDetailDlg->m_valueEditHitRate = m_GridCurrent.GetItemText(row,6);
	m_pMonsterAttackDetailDlg->m_valueEditSpeedPenalty = m_GridCurrent.GetItemText(row,7);
	m_pMonsterAttackDetailDlg->m_valueEditRange = m_GridCurrent.GetItemText(row,8);
	m_pMonsterAttackDetailDlg->m_valueEditCharging = m_GridCurrent.GetItemText(row,9);
	m_pMonsterAttackDetailDlg->m_valueEditRangeAngle = m_GridCurrent.GetItemText(row,10);
	m_pMonsterAttackDetailDlg->m_valueEditShotNum = m_GridCurrent.GetItemText(row,11);
	m_pMonsterAttackDetailDlg->m_valueEditMultiNum = m_GridCurrent.GetItemText(row,12);
	m_pMonsterAttackDetailDlg->m_valueEditAttackTime = m_GridCurrent.GetItemText(row,13);
	m_pMonsterAttackDetailDlg->m_valueEditReqSP = m_GridCurrent.GetItemText(row,14);
	m_pMonsterAttackDetailDlg->m_valueEditSummonMonster = m_GridCurrent.GetItemText(row,15);

	int tempValue;
	tempValue = atoi(m_GridCurrent.GetItemText(row,16));
	if(tempValue == ORBIT_SINGLE_CROSS_FIRE)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_CROSS_FIRE");
	else if(tempValue == ORBIT_SINGLE_FIRE_100)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_100");
	else if(tempValue == ORBIT_SINGLE_FIRE_150)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_150");
	else if(tempValue == ORBIT_SINGLE_FIRE_200)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_200");
	else if(tempValue == ORBIT_SINGLE_FIRE_250)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_250");
	else if(tempValue == ORBIT_SINGLE_FIRE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_300");
	else if(tempValue == ORBIT_SINGLE_FIRE_320)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_320");
	else if(tempValue == ORBIT_SINGLE_FIRE_350)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_SINGLE_FIRE_350");
	else if(tempValue == ORBIT_TWIN_CROSS_FIRE)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_CROSS_FIRE");
	else if(tempValue == ORBIT_TWIN_FIRE_100)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_100");
	else if(tempValue == ORBIT_TWIN_FIRE_150)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_150");
	else if(tempValue == ORBIT_TWIN_FIRE_200)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_200");
	else if(tempValue == ORBIT_TWIN_FIRE_250)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_250");
	else if(tempValue == ORBIT_TWIN_FIRE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_300");
	else if(tempValue == ORBIT_TWIN_FIRE_320)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_320");
	else if(tempValue == ORBIT_TWIN_FIRE_350)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_TWIN_FIRE_350");
	else if(tempValue == ORBIT_CROSS_FIRE)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_CROSS_FIRE");
	else if(tempValue == ORBIT_STRAIGHT_ROCKET_250)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STRAIGHT_ROCKET_250");
	else if(tempValue == ORBIT_FALL_ROCKET_250)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FALL_ROCKET_250");
	else if(tempValue == ORBIT_LEFT_ROCKET_200)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_LEFT_ROCKET_200");
	else if(tempValue == ORBIT_RIGHT_ROCKET_200)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RIGHT_ROCKET_200");
	else if(tempValue == ORBIT_THREE_ROCKET)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_THREE_ROCKET");
	else if(tempValue == ORBIT_CHILD_ROCKET)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_CHILD_ROCKET");
	else if(tempValue == ORBIT_STRAIGHT_MISSILE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STRAIGHT_MISSILE_300");
	else if(tempValue == ORBIT_FALL_MISSILE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FALL_MISSILE_300");
	else if(tempValue == ORBIT_LEFT_MISSILE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_LEFT_MISSILE_300");
	else if(tempValue == ORBIT_RIGHT_MISSILE_270)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RIGHT_MISSILE_270");
	else if(tempValue == ORBIT_UPDOWN_ROCKET_270)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_UPDOWN_ROCKET_270");
	else if(tempValue == ORBIT_UPDOWN_MISSILE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_UPDOWN_MISSILE_300");
	else if(tempValue == ORBIT_FLAME_THROWER)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FLAME_THROWER");
	else if(tempValue == ORBIT_BODYSLAM)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_BODYSLAM");
	else if(tempValue == ORBIT_MINE_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_MINE_300");
	else if(tempValue == ORBIT_MINE_400)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_MINE_400");
	else if(tempValue == ORBIT_RANGE_ROCKET_300)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RANGE_ROCKET_300");
	else if(tempValue == ORBIT_RANGE_ROCKET_400)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_RANGE_ROCKET_400");
	else if(tempValue == ORBIT_FIRE)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_FIRE");
	else if(tempValue == ORBIT_OBJBEAM)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_OBJBEAM");
	else if(tempValue == ORBIT_STRAIGHTBOOM)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STRAIGHTBOOM");
	else if(tempValue == ORBIT_CATCH)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_CATCH");
	else if(tempValue == ORBIT_STICK)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_STICK");
	else if(tempValue == ORBIT_ATTACK_ALL)
		m_pMonsterAttackDetailDlg->m_ctrlComboOrbitType.SetWindowText("ORBIT_ATTACK_ALL");	

	tempValue = atoi(m_GridCurrent.GetItemText(row,17));
	if(tempValue == ITEMKIND_FOR_MON_PRIMARY)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_PRIMARY);
	else if(tempValue == ITEMKIND_FOR_MON_GUN)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_GUN);
	else if(tempValue == ITEMKIND_FOR_MON_BEAM)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_BEAM);
	else if(tempValue == ITEMKIND_FOR_MON_ALLATTACK)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_ALLATTACK);
	else if(tempValue == ITEMKIND_FOR_MON_SECONDARY)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_SECONDARY);
	else if(tempValue == ITEMKIND_FOR_MON_ROCKET)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_ROCKET);
	else if(tempValue == ITEMKIND_FOR_MON_MISSILE)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_MISSILE);
	else if(tempValue == ITEMKIND_FOR_MON_BUNDLE)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_BUNDLE);
	else if(tempValue == ITEMKIND_FOR_MON_MINE)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_MINE);
	else if(tempValue == ITEMKIND_FOR_MON_SHIELD)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_SHIELD);
	else if(tempValue == ITEMKIND_FOR_MON_DUMMY)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_DUMMY);
	else if(tempValue == ITEMKIND_FOR_MON_FIXER)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_FIXER);
	else if(tempValue == ITEMKIND_FOR_MON_DECOY)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_DECOY);
	else if(tempValue == ITEMKIND_FOR_MON_FIRE)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_FIRE);
	else if(tempValue == ITEMKIND_FOR_MON_OBJBEAM)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_OBJBEAM);
	else if(tempValue == ITEMKIND_FOR_MON_STRAIGHTBOOM)
		m_pMonsterAttackDetailDlg->m_ctrlComboKind.SetWindowText(STRCMD_CS_ITEMKIND_FOR_MON_STRAIGHTBOOM);

	m_pMonsterAttackDetailDlg->m_valueEditCameraPattern = m_GridCurrent.GetItemText(row,18);
	m_pMonsterAttackDetailDlg->m_valueEditSourceIndex = m_GridCurrent.GetItemText(row,19);

	m_pMonsterAttackDetailDlg->m_valueEditDestParameter1 = m_GridCurrent.GetItemText(row,20);
	m_pMonsterAttackDetailDlg->m_valueEditParameterValue1 = m_GridCurrent.GetItemText(row,21);
	m_pMonsterAttackDetailDlg->m_valueEditDestParameter2 = m_GridCurrent.GetItemText(row,22);
	m_pMonsterAttackDetailDlg->m_valueEditParameterValue2 = m_GridCurrent.GetItemText(row,23);
	m_pMonsterAttackDetailDlg->m_valueEditDestParameter3 = m_GridCurrent.GetItemText(row,24);
	m_pMonsterAttackDetailDlg->m_valueEditParameterValue3 = m_GridCurrent.GetItemText(row,25);
	m_pMonsterAttackDetailDlg->m_valueEditDestParameter4 = m_GridCurrent.GetItemText(row,26);
	m_pMonsterAttackDetailDlg->m_valueEditParameterValue4 = m_GridCurrent.GetItemText(row,27);

	m_pMonsterAttackDetailDlg->UpdateData(FALSE);
}

void CMonsterAttackDlg::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here
	if (m_pMonsterAttackDetailDlg != NULL)
	{
		util::del(m_pMonsterAttackDetailDlg);
	}

	if (m_pMonsterAttackDetailDlg == NULL)
	{
		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
		m_pMonsterAttackDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE); //버튼 비활성화
	}

	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);
}

void CMonsterAttackDlg::OnButtonUpdate() 
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

	if (m_pMonsterAttackDetailDlg != NULL)
	{
		util::del(m_pMonsterAttackDetailDlg);
	}

	if (m_pMonsterAttackDetailDlg == NULL)
	{
		m_pMonsterAttackDetailDlg = new CMonsterAttackDetailDlg(this);
		m_pMonsterAttackDetailDlg->Create(IDD_DIALOG_MONSTERATTACKDETAIL, this);
		m_pMonsterAttackDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE); //버튼 비활성화
	}
	SetDetailDialog(tmpCellID.row);
	m_pMonsterAttackDetailDlg->ShowWindow(SW_SHOW);

	m_pMonsterAttackDetailDlg->m_nSelectRow = tmpCellID.row; //선택된 row를 저장한다.
	m_GridCurrent.Refresh();
}

void CMonsterAttackDlg::OnButtonDelete() 
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

void CMonsterAttackDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	m_GridCurrent.UpdateWindow();
}


void CMonsterAttackDlg::OnButtonCancel() 
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

void CMonsterAttackDlg::ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row)
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

void CMonsterAttackDlg::OnButtonServerupdate() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//DB에 값을 집어 넣는다.
// 	CString szQuery;
// 	BOOL bRet= 0;
// 	int row = m_GridUpdate.GetRowCount();
// 	for(int i = 0; i< row; i++)
// 	{
// 		if(m_GridUpdate.GetItemText(1, 0) == "DELETE")
// 		{
// 			//DELETE ROW
// 			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 			//szQuery.Format("delete ti_item where itemnum = %s", m_GridUpdate.GetItemText(1, 1));
// 			szQuery.Format(QUERY_080702_0102, m_GridUpdate.GetItemText(1, 1));
// 
// 			TRACE(szQuery.Left(500));
// 			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
// 			if (!bRet)
// 			{
// 				// cleanup
// 				AfxMessageBox("Fail During Delete 1 Row Data");
// 				m_pODBCStmt->FreeStatement();
// 				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
// 				return;
// 			}	
// 			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
// 
// 			m_GridUpdate.DeleteRow(1);
// 			m_GridUpdate.Refresh();
// 		}
// 		else if(m_GridUpdate.GetItemText(1, 0) == "UPDATE")
// 		{
// 			//Update ROW
// 			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 			//szQuery.Format("Update ti_item set Itemname='%s', AbilityMin=%s, AbilityMax=%s, ReqRace=%s, ReqMinLevel=%s, Hitrate=%s, SpeedPenalty=%s, Range=%s, Charging=%s, RangeAngle=%s, Shotnum=%s, Multinum=%s, Attacktime=%s, ReqSP=%s, Summonmonster=%s, Orbittype=%s, Kind=%s, CameraPattern=%s, SourceIndex = %s, DestParameter1 = %s, ParameterValue1 = %s, DestParameter2 = %s, ParameterValue2 = %s, DestParameter3 = %s, ParameterValue3 = %s, DestParameter4 = %s, ParameterValue4 = %s where itemnum = %s"
// 			szQuery.Format(QUERY_080702_0103
// 				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
// 				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
// 				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
// 				, m_GridUpdate.GetItemText(1, 8), m_GridUpdate.GetItemText(1, 9)
// 				, m_GridUpdate.GetItemText(1, 10), m_GridUpdate.GetItemText(1, 11)
// 				, m_GridUpdate.GetItemText(1, 12), m_GridUpdate.GetItemText(1, 13)
// 				, m_GridUpdate.GetItemText(1, 14), m_GridUpdate.GetItemText(1, 15)
// 				, m_GridUpdate.GetItemText(1, 16), m_GridUpdate.GetItemText(1, 17)
// 				, m_GridUpdate.GetItemText(1, 18), m_GridUpdate.GetItemText(1, 19)
// 				, m_GridUpdate.GetItemText(1, 20), m_GridUpdate.GetItemText(1, 21)
// 				, m_GridUpdate.GetItemText(1, 22), m_GridUpdate.GetItemText(1, 23)
// 				, m_GridUpdate.GetItemText(1, 24), m_GridUpdate.GetItemText(1, 25)
// 				, m_GridUpdate.GetItemText(1, 26), m_GridUpdate.GetItemText(1, 27)
// 				, m_GridUpdate.GetItemText(1, 28)
// 				, m_GridUpdate.GetItemText(1, 1)
// 				);
// 			TRACE(szQuery.Left(500));
// 			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
// 			
// 
// 			if (!bRet)
// 			{
// 				// cleanup
// 				AfxMessageBox("Fail During Update 1 Row Data");
// 				m_pODBCStmt->FreeStatement();
// 				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
// 				return;
// 			}	
// 			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
// 
// 			m_GridUpdate.DeleteRow(1);
// 			m_GridUpdate.Refresh();
// 		}
// 		else if(m_GridUpdate.GetItemText(1, 0) == "INSERT")
// 		{
// 			//Insert ROW
// 			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 			//szQuery.Format("Insert into ti_item(ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, CameraPattern, SourceIndex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4) values ( %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"
// 			szQuery.Format(QUERY_080702_0104
// 				, m_GridUpdate.GetItemText(1, 1)
// 				, m_GridUpdate.GetItemText(1, 2), m_GridUpdate.GetItemText(1, 3)
// 				, m_GridUpdate.GetItemText(1, 4), m_GridUpdate.GetItemText(1, 5)
// 				, m_GridUpdate.GetItemText(1, 6), m_GridUpdate.GetItemText(1, 7)
// 				, m_GridUpdate.GetItemText(1, 8), m_GridUpdate.GetItemText(1, 9)
// 				, m_GridUpdate.GetItemText(1, 10), m_GridUpdate.GetItemText(1, 11)
// 				, m_GridUpdate.GetItemText(1, 12), m_GridUpdate.GetItemText(1, 13)
// 				, m_GridUpdate.GetItemText(1, 14), m_GridUpdate.GetItemText(1, 15)
// 				, m_GridUpdate.GetItemText(1, 16), m_GridUpdate.GetItemText(1, 17)
// 				, m_GridUpdate.GetItemText(1, 18), m_GridUpdate.GetItemText(1, 19)
// 				, m_GridUpdate.GetItemText(1, 20), m_GridUpdate.GetItemText(1, 21)
// 				, m_GridUpdate.GetItemText(1, 22), m_GridUpdate.GetItemText(1, 23)
// 				, m_GridUpdate.GetItemText(1, 24), m_GridUpdate.GetItemText(1, 25)
// 				, m_GridUpdate.GetItemText(1, 26), m_GridUpdate.GetItemText(1, 27)
// 				, m_GridUpdate.GetItemText(1, 28)
// 				);
// 			TRACE(szQuery.Left(500));
// 
// 			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
// 			
// 			if (!bRet)
// 			{
// 				// cleanup
// 				AfxMessageBox("Fail During Update 1 Row Data");
// 				m_pODBCStmt->FreeStatement();
// 				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
// 				return;
// 			}	
// 			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
// 
// 			m_GridUpdate.DeleteRow(1);
// 			m_GridUpdate.Refresh();
// 		}
// 		
// 	}
// 	// cleanup
// 	m_pODBCStmt->FreeStatement();
// 	GetCurrentList(); //새로 값을 SELECT해서 Current Grid Ctrl에 넣는다.
}

