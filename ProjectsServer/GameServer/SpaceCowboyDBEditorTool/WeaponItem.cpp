// WeaponItem.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "WeaponItem.h"
#include "SpaceCowboyDBEditorToolDlg.h"
#include "AtumProtocol.h"
#include "AtumError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeaponItem dialog


CWeaponItem::CWeaponItem(CWnd* pParent /*=NULL*/)
	: CDialog(CWeaponItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWeaponItem)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	m_pODBCStmt				= new CODBCStatement;
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;		// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리

	m_pWeaponItemDetailDlg = NULL;
}

CWeaponItem::~CWeaponItem()
{
	util::del(m_pWeaponItemDetailDlg);
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
//	util::del(m_pODBCStmt);
}


void CWeaponItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeaponItem)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDC_IE_CUSTOM_GRID_CURRENT, m_GridCurrent);
		DDX_Control(pDX, IDC_IE_CUSTOM_GRID_UPDATE, m_GridUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeaponItem, CDialog)
	//{{AFX_MSG_MAP(CWeaponItem)
	ON_BN_CLICKED(IDC_IE_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_IE_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_IE_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_IE_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_IE_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_IE_BUTTON_VIEW, OnButtonView)
	ON_BN_CLICKED(IDC_IE_BUTTON_SERVERUPDATE, OnButtonServerupdate)
	ON_BN_CLICKED(IDC_IE_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_IE_BUTTON_REFRESHUPDATE, OnButtonRefreshupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeaponItem message handlers

BOOL CWeaponItem::OnInitDialog() 
{
	CDialog::OnInitDialog();

	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-08 by cmkwon
	GetDlgItem(IDC_IE_BUTTON_INSERT)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_IE_BUTTON_UPDATE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	GetDlgItem(IDC_IE_BUTTON_DELETE)->EnableWindow(m_pMainDlg->GetEnbaleEdit());
	
	// TODO: Add extra initialization here
// 2006-01-23 by cmkwon
// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 하나의 연결로 처리
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
	GetCurrentItemList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWeaponItem::InitGridCurrent()
{
	m_GridCurrent.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	int m_nCols = 41;
#else
	int m_nCols = 42;
#endif // SHOP_PRICES_PER_BUILDING_NPC

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
	Item.strText.Format("Kind");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemName");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMin");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMax");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RangeAngle");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqRace");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqAttackPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDefensePart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqFuelPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqSoulPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqShieldPart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDodgePart");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqUnitKind");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMinLevel");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMaxLevel");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqItemKind");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Weight");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HitRate");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Defense");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Position");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbrasionRate");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Charging");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinTradeQuality");
	m_GridCurrent.SetItem(&Item);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Item.col = m_nCols++;
	Item.strText.Format("SellingPrice");
	m_GridCurrent.SetItem(&Item);
#else
	Item.col = m_nCols++;
	Item.strText.Format("Price");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CashPrice");
	m_GridCurrent.SetItem(&Item);
#endif

	Item.col = m_nCols++;
	Item.strText.Format("ReAttackTime");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("OverHitTime");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LinkItem");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MultiTarget");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ExplosionRange");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReactionRange");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Shortnum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Multinum");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackTime");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemAttribute");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Description");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Caliber");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("OrbitType");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CameraPattern");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridCurrent.SetItem(&Item);

	// arrange grid
	m_GridCurrent.AutoSize();
//	m_GridCurrent.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCurrent.GetRowCount()-1, m_GridCurrent.GetColumnCount()-1);
	m_GridCurrent.ClearCells(tmpCellRange);
}

void CWeaponItem::InitGridUpdate()
{
	m_GridUpdate.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	int m_nCols = 42;
#else
	int m_nCols = 43;
#endif // SHOP_PRICES_PER_BUILDING_NPC

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
	Item.strText.Format("Kind");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemName");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMin");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbilityMax");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RangeAngle");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqRace");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqAttackPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDefensePart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqFuelPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqSoulPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqShieldPart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqDodgePart");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqUnitKind");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMinLevel");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqMaxLevel");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqItemKind");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Weight");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("HitRate");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Defense");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Range");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Position");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AbrasionRate");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Charging");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinTradeQuality");
	m_GridUpdate.SetItem(&Item);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Item.col = m_nCols++;
	Item.strText.Format("SellingPrice");
	m_GridUpdate.SetItem(&Item);
#else
	Item.col = m_nCols++;
	Item.strText.Format("Price");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CashPrice");
	m_GridUpdate.SetItem(&Item);
#endif // SHOP_PRICES_PER_BUILDING_NPC

	Item.col = m_nCols++;
	Item.strText.Format("ReAttackTime");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("OverHitTime");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("LinkItem");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MultiTarget");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ExplosionRange");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReactionRange");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Shortnum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Multinum");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AttackTime");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemAttribute");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Description");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Caliber");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("OrbitType");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CameraPattern");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SourceIndex");
	m_GridUpdate.SetItem(&Item);

	// arrange grid
	m_GridUpdate.AutoSize();
//	m_GridUpdate.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridUpdate.GetRowCount()-1, m_GridUpdate.GetColumnCount()-1);
	m_GridUpdate.ClearCells(tmpCellRange);
}

void CWeaponItem::GetCurrentItemList()
{
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	CString szSQLQuery;
	
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("select itemnum, kind, itemname, abilitymin, abilitymax, rangeangle, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, reqitemkind, weight, hitrate, defense, range, position, abrasionrate, charging, mintradequantity, price, cashprice, reattacktime, [time], linkitem, multitarget, explosionrange, reactionrange, shotnum, multinum, attacktime, itemattribute, description, caliber, orbittype, camerapattern, sourceindex from ti_item WITH (NOLOCK) where kind >= %d and kind <= %d"
// 		, ITEMKIND_AUTOMATIC, ITEMKIND_MINE);
	szSQLQuery.Format(QUERY_080702_0230, ITEMKIND_AUTOMATIC, ITEMKIND_MINE);

	SQLINTEGER cb[43];
	fill_n(cb, 43, SQL_NTS);

	_TI_ITEM		st_WeaponItem;
	memset(&st_WeaponItem, 0, sizeof(st_WeaponItem));

	int i = 0;
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.ItemNum, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.Kind, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_CHAR, &st_WeaponItem.ItemName, 20, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_WeaponItem.AbilityMin, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_WeaponItem.AbilityMax, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_WeaponItem.RangeAngle, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqRace, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqAttackPart, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqDefensePart, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqFuelPart, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqSoulPart, 0, &cb[i]);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqShieldPart, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReqDodgePart, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.ReqUnitKind, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.ReqMinLevel, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.ReqMaxLevel, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.ReqItemKind, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.Weight, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_FLOAT, &st_WeaponItem.HitRate, 1, &cb[i]);		// 2010-07-19 by dhjin, 확률 수식 변경
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.Defense, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.Range, 0, &cb[i]);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.Position, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.AbrasionRate, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.Charging, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.MinTradeQuantity, 0, &cb[i]);
#ifdef SHOP_PRICES_PER_BUILDING_NPC
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.SellingPrice, 0, &cb[i]);
#else
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.Price, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.CashPrice, 0, &cb[i]);
#endif // SHOP_PRICES_PER_BUILDING_NPC
	
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.ReAttacktime, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.Time, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.LinkItem, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.MultiTarget, 1, &cb[i]);

	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ExplosionRange, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.ReactionRange, 1, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.ShotNum, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.MultiNum, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_SSHORT, &st_WeaponItem.AttackTime, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.ItemAttribute, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_CHAR, &st_WeaponItem.Description, 200, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.Caliber, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_UTINYINT, &st_WeaponItem.OrbitType, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.CameraPattern, 0, &cb[i]);
	SQLBindCol(m_pODBCStmt->m_hstmt, ++i, SQL_C_ULONG, &st_WeaponItem.SourceIndex, 0, &cb[i]);

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
		InsertItemToGridCurrent(&st_WeaponItem, nCount+1);

		nCount++;
		memset(&st_WeaponItem, 0, sizeof(st_WeaponItem));
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridCurrent.AutoSize();

}

void CWeaponItem::InsertItemToGridCurrent(_TI_ITEM *i_pTi_Item, int i_nRow)
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
	Item.strText.Format("%d", i_pTi_Item->Kind);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_Item->ItemName);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pTi_Item->AbilityMin);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pTi_Item->AbilityMax);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%.1f", i_pTi_Item->RangeAngle);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqRace);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqAttackPart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqDefensePart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqFuelPart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqSoulPart);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////10 items

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqShieldPart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqDodgePart);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqUnitKind);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqMinLevel);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqMaxLevel);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReqItemKind);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Weight);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->HitRate);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Defense);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Range);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////20items

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Position);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->AbrasionRate);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Charging);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->MinTradeQuantity);
	m_GridCurrent.SetItem(&Item);

#ifdef SHOP_PRICES_PER_BUILDING_NPC
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SellingPrice);
	m_GridCurrent.SetItem(&Item);
#else
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Price);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->CashPrice);
	m_GridCurrent.SetItem(&Item);
#endif // SHOP_PRICES_PER_BUILDING_NPC
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReAttacktime);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Time);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->LinkItem);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->MultiTarget);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////30items

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ExplosionRange);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->ReactionRange);
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
	Item.strText.Format("%d", i_pTi_Item->ItemAttribute);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_Item->Description);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->Caliber);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->OrbitType);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->CameraPattern);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_Item->SourceIndex);
	m_GridCurrent.SetItem(&Item);


}	

void CWeaponItem::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here

	if (m_pWeaponItemDetailDlg != NULL)
	{
		util::del(m_pWeaponItemDetailDlg);
	}

	if (m_pWeaponItemDetailDlg == NULL)
	{
		m_pWeaponItemDetailDlg = new CWeaponItemDetail(this);
		m_pWeaponItemDetailDlg->Create(IDD_DIALOG_WEAPON_DETAIL, this);
		m_pWeaponItemDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
	}

	m_pWeaponItemDetailDlg->ShowWindow(SW_SHOW);
}

void CWeaponItem::OnButtonUpdate() 
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

	if (m_pWeaponItemDetailDlg != NULL)
	{
		util::del(m_pWeaponItemDetailDlg);
	}

	if (m_pWeaponItemDetailDlg == NULL)
	{
		m_pWeaponItemDetailDlg = new CWeaponItemDetail(this);
		m_pWeaponItemDetailDlg->Create(IDD_DIALOG_WEAPON_DETAIL, this);
		m_pWeaponItemDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pWeaponItemDetailDlg->ShowWindow(SW_SHOW);

	m_pWeaponItemDetailDlg->m_nSelectRow = tmpCellID.row;
//	m_GridCurrent.DeleteRow(tmpCellID.row);
	m_GridCurrent.Refresh();
}

void CWeaponItem::OnButtonDelete() 
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

void CWeaponItem::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	m_GridCurrent.UpdateWindow();
}


void CWeaponItem::ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row)
{
	// 칼럼 만들기
	newGrid->SetRowCount(newGrid->GetRowCount()+1);

	if(type == 1) //Delete 버튼을 눌렀을때
	{
		newGrid->SetItemText(newGrid->GetRowCount()-1, 0, "DELETE");
		for(int m_nCols = 0; m_nCols<43;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols+1, oriGrid->GetItemText(row,m_nCols));
	}
	else
	{
		for(int m_nCols = 0; m_nCols<44;m_nCols++)
			newGrid->SetItemText(newGrid->GetRowCount()-1, m_nCols, oriGrid->GetItemText(row,m_nCols+1));

	}

	newGrid->Refresh();
}


void CWeaponItem::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
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

void CWeaponItem::OnButtonView() 
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

	if (m_pWeaponItemDetailDlg != NULL)
	{
		util::del(m_pWeaponItemDetailDlg);
	}

	if (m_pWeaponItemDetailDlg == NULL)
	{
		m_pWeaponItemDetailDlg = new CWeaponItemDetail(this);
		m_pWeaponItemDetailDlg->Create(IDD_DIALOG_WEAPON_DETAIL, this);
		m_pWeaponItemDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pWeaponItemDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pWeaponItemDetailDlg->ShowWindow(SW_SHOW);
}

void CWeaponItem::SetDetailDialog(int row)
{
	//선택된 셀을 바탕으로 값을 설정한다.

	m_pWeaponItemDetailDlg->m_valueEditItemNum = m_GridCurrent.GetItemText(row,0);
	m_pWeaponItemDetailDlg->m_ctrlComboKind.SetWindowText(CAtumSJ::GetItemKindName(atoi(m_GridCurrent.GetItemText(row,1))));
	m_pWeaponItemDetailDlg->m_valueEditItemName = m_GridCurrent.GetItemText(row,2);
	m_pWeaponItemDetailDlg->m_valueEditAttackMin = m_GridCurrent.GetItemText(row,3);
	m_pWeaponItemDetailDlg->m_valueEditAttackMax = m_GridCurrent.GetItemText(row,4);
	m_pWeaponItemDetailDlg->m_valueEditRangeAngle = m_GridCurrent.GetItemText(row,5);
	m_pWeaponItemDetailDlg->m_ctrlComboRace.SetWindowText(CAtumSJ::GetRaceString(atoi(m_GridCurrent.GetItemText(row,6))));
	m_pWeaponItemDetailDlg->m_valueEditReqAttack = m_GridCurrent.GetItemText(row,7);
	m_pWeaponItemDetailDlg->m_valueEditReqDefense = m_GridCurrent.GetItemText(row,8);
	m_pWeaponItemDetailDlg->m_valueEditReqFuel = m_GridCurrent.GetItemText(row,9);
	m_pWeaponItemDetailDlg->m_valueEditReqSoul = m_GridCurrent.GetItemText(row,10);
	m_pWeaponItemDetailDlg->m_valueEditReqShield = m_GridCurrent.GetItemText(row,11);
	m_pWeaponItemDetailDlg->m_valueEditReqDodge = m_GridCurrent.GetItemText(row,12);
	int tempBit = atoi((char*)(LPCTSTR)m_GridCurrent.GetItemText(row,13)); //unitkind
	if(IS_BGEAR((USHORT)tempBit) == TRUE)
		m_pWeaponItemDetailDlg->m_valueCheckBGEAR = TRUE;
	if(IS_MGEAR((USHORT)tempBit) == TRUE)
		m_pWeaponItemDetailDlg->m_valueCheckMGEAR = TRUE;
	if(IS_AGEAR((USHORT)tempBit) == TRUE)
		m_pWeaponItemDetailDlg->m_valueCheckAGEAR = TRUE;
	if(IS_IGEAR((USHORT)tempBit) == TRUE)
		m_pWeaponItemDetailDlg->m_valueCheckIGEAR = TRUE;
	m_pWeaponItemDetailDlg->m_valueEditMinLevel = m_GridCurrent.GetItemText(row,14);
	m_pWeaponItemDetailDlg->m_valueEditMaxLevel = m_GridCurrent.GetItemText(row,15);
	if(atoi(m_GridCurrent.GetItemText(row,16)) >= 43 && atoi(m_GridCurrent.GetItemText(row,16)) <= 49)
		m_pWeaponItemDetailDlg->m_ctrlComboItemKind.SetWindowText(CAtumSJ::GetItemKindName(atoi(m_GridCurrent.GetItemText(row,15))));
	m_pWeaponItemDetailDlg->m_valueEditWeight = m_GridCurrent.GetItemText(row,17);
	m_pWeaponItemDetailDlg->m_valueEditHitRate = m_GridCurrent.GetItemText(row,18);
	m_pWeaponItemDetailDlg->m_valueEditDefense = m_GridCurrent.GetItemText(row,19);
	m_pWeaponItemDetailDlg->m_valueEditRange = m_GridCurrent.GetItemText(row,20);
	m_pWeaponItemDetailDlg->m_ctrlComboPosition.SetWindowText(CAtumSJ::GetPositionString(atoi(m_GridCurrent.GetItemText(row,21)))); //position
	m_pWeaponItemDetailDlg->m_valueEditAbrasionRate = m_GridCurrent.GetItemText(row,22);
	m_pWeaponItemDetailDlg->m_valueEditCharging = m_GridCurrent.GetItemText(row,23);
	m_pWeaponItemDetailDlg->m_valueEditMinTradeQuantity = m_GridCurrent.GetItemText(row,24);
	m_pWeaponItemDetailDlg->m_valueEditPrice = m_GridCurrent.GetItemText(row,25);
	m_pWeaponItemDetailDlg->m_valueEditCashPrice = m_GridCurrent.GetItemText(row,26);
	m_pWeaponItemDetailDlg->m_valueEditReattackTime = m_GridCurrent.GetItemText(row,27);
	m_pWeaponItemDetailDlg->m_valueEditOverHitTime = m_GridCurrent.GetItemText(row,28);
	m_pWeaponItemDetailDlg->m_ctrlComboLink.SetWindowText(m_GridCurrent.GetItemText(row,29)); //Link
	m_pWeaponItemDetailDlg->m_valueEditMultiTarget = m_GridCurrent.GetItemText(row,30);
	m_pWeaponItemDetailDlg->m_valueEditExplosionRange = m_GridCurrent.GetItemText(row,31);
	m_pWeaponItemDetailDlg->m_valueEditReactionRange = m_GridCurrent.GetItemText(row,32);
	m_pWeaponItemDetailDlg->m_valueEditShotNum = m_GridCurrent.GetItemText(row,33);
	m_pWeaponItemDetailDlg->m_valueEditMultiNum = m_GridCurrent.GetItemText(row,34);
	m_pWeaponItemDetailDlg->m_valueEditAttackTime = m_GridCurrent.GetItemText(row,35);
	//CAtumSJ에 함수 추가된 뒤 수정 필요함.
	//m_pWeaponItemDetailDlg->m_ctrlComboPosition.SetWindowText(CAtumSJ::Get..m_GridCurrent.GetItemText(row,35)); //itemattribute

	unsigned char tempValue = atoi((char*)(LPCTSTR)m_GridCurrent.GetItemText(row,36));
	if(tempValue & 0x01)
		m_pWeaponItemDetailDlg->m_valueCheckAutoPickUp = TRUE;
	if(tempValue >> 1 & 0x01)
		m_pWeaponItemDetailDlg->m_valueCheckNoTransfer = TRUE;
	if(tempValue >> 2 & 0x01)
		m_pWeaponItemDetailDlg->m_valueCheckQuestItem = TRUE;
	if(tempValue >> 3 & 0x01)
		m_pWeaponItemDetailDlg->m_valueCheckTimeLimite = TRUE;
	if(tempValue >> 16 & 0x01)
		m_pWeaponItemDetailDlg->m_valueCheckCashItem = TRUE;

	m_pWeaponItemDetailDlg->m_valueEditDescription = m_GridCurrent.GetItemText(row,37);
	m_pWeaponItemDetailDlg->m_valueEditCaliber = m_GridCurrent.GetItemText(row,38);
	m_pWeaponItemDetailDlg->m_valueEditOrbitType = m_GridCurrent.GetItemText(row,39);
	m_pWeaponItemDetailDlg->m_valueEditSourceIndex = m_GridCurrent.GetItemText(row,40);
	m_pWeaponItemDetailDlg->UpdateData(FALSE);
}

void CWeaponItem::OnButtonServerupdate() 
{
	// TODO: Add your control notification handler code here
	//DB에 값을 집어 넣는다.
	CString szQuery;
	BOOL bRet= 0;
	int initRow = m_GridUpdate.GetRowCount();
	for(int i = 0; i< initRow -1; i++)
	{
		if(m_GridUpdate.GetItemText(1, 0) == "DELETE")
		{
			//DELETE ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("delete ti_item where itemnum = %s", m_GridUpdate.GetItemText(1, 1));
			szQuery.Format(QUERY_080702_0231, m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));
			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Delete 1 Row Data");
				m_pODBCStmt->FreeStatement();
				return;
			}	
		//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "UPDATE")
		{
			if(m_GridUpdate.GetItemText(1, 16) == "")
				m_GridUpdate.SetItemText(1,16,"NULL");
			if(m_GridUpdate.GetItemText(1, 36) == "")
				m_GridUpdate.SetItemText(1,36,"NULL");
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Update ti_item set kind = %s, itemname = '%s', abilitymin = %s, abilitymax = %s, rangeangle = %s, reqrace = %s, reqattackpart = %s, reqdefensepart = %s, ReqFuelPart = %s, reqsoulpart = %s, reqshieldpart = %s, reqdodgepart = %s, requnitkind = %s, reqminlevel = %s, reqmaxlevel = %s, reqitemkind = %s, weight = %s, hitrate = %s, defense = %s, range = %s, position = %s, abrasionrate = %s, charging = %s, mintradequantity = %s, price = %s, cashprice = %s, reattacktime = %s, [time] = %s, linkitem = %s, multitarget = %s, explosionrange = %s, reactionrange = %s, shotnum = %s, multinum = %s, attacktime = %s, itemattribute = %s, [description] = '%s', caliber = %s, orbittype = %s, camerapattern = %s, sourceindex = %s where itemnum = %s "				
			szQuery.Format(QUERY_080702_0232				
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
				, m_GridUpdate.GetItemText(1, 38), m_GridUpdate.GetItemText(1, 39)
				, m_GridUpdate.GetItemText(1, 40), m_GridUpdate.GetItemText(1, 41)
				, m_GridUpdate.GetItemText(1, 42)
				, m_GridUpdate.GetItemText(1, 1)
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
				return;
			}	
			//	szQuery = "[" + m_pMainDlg->m_valueEditID + "] " + "[" + m_pMainDlg->m_pServerInfo4Admin->DBUID + "] " + "[]+szQuery;
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "INSERT")
		{
			if(m_GridUpdate.GetItemText(1, 16) == "")
				m_GridUpdate.SetItemText(1,16,"NULL");
			if(m_GridUpdate.GetItemText(1, 36) == "")
				m_GridUpdate.SetItemText(1,36,"NULL");
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Insert into ti_item(itemnum, kind, itemname, abilitymin, abilitymax, rangeangle, reqrace, reqattackpart, reqdefensepart, ReqFuelPart, reqsoulpart, reqshieldpart, reqdodgepart, requnitkind, reqminlevel, reqmaxlevel, reqitemkind, weight, hitrate, defense, range, position, abrasionrate, charging, mintradequantity, price, cashprice, reattacktime, [time], linkitem, multitarget, explosionrange, reactionrange, shotnum, multinum, attacktime, itemattribute, [description], caliber, orbittype, camerapattern, sourceindex) values ( %s, %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, '%s', %s, %s, %s, %s)"				
			szQuery.Format(QUERY_080702_0233				
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
				, m_GridUpdate.GetItemText(1, 38), m_GridUpdate.GetItemText(1, 39)
				, m_GridUpdate.GetItemText(1, 40), m_GridUpdate.GetItemText(1, 41)
				, m_GridUpdate.GetItemText(1, 42)
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
	GetCurrentItemList();
}

void CWeaponItem::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CWeaponItem::OnButtonRefreshupdate() 
{
	// TODO: Add your control notification handler code here
	m_GridUpdate.Refresh();
}
