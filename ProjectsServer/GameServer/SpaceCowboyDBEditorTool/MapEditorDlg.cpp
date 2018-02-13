// MapEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MapEditorDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapEditorDlg dialog


CMapEditorDlg::CMapEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_pMapDetailDlg = NULL;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); //메인 다이얼로그 포인트
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
}

CMapEditorDlg::~CMapEditorDlg()
{
// 2006-01-23 by cmkwon
// 	util::del(m_pODBCStmt);
	util::del(m_pMapDetailDlg);
	
}


void CMapEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapEditorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CUSTOM_CURRENT, m_GridCurrent);
	DDX_Control(pDX, IDC_CUSTOM_UPDATE, m_GridUpdate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CMapEditorDlg)
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
// CMapEditorDlg message handlers

void CMapEditorDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

BOOL CMapEditorDlg::OnInitDialog() 
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

void CMapEditorDlg::InitGridCurrent()
{
	m_GridCurrent.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 40;

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
	Item.strText.Format("MapIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RenderMapIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MapName");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MapInfluenceType");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityMapIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CitiWarQuestIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CitiWarQuestNPCIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CitiWarQuestMonsterUID");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxUserCount");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxGroundHeight");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinGroundHeight");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("WaterHeight");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("UserVisibleDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterVisibleDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuestIndexForWarp");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("FrontPositionDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AutoRecoveryFlag");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DefaultMapIndex");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityWarpTargetPositionX");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityWarpTargetPositionY");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityWarpTargetPositionZ");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ClientMaxAltitudeHeight");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DayFogColor");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightFogColor");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DayFogStartDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DayFogEndDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightFogStartDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightFogEndDistance");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DaySunDirectionX");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DaySunDirectionY");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DaySunDirectionZ");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightSunDirectionX");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightSunDirectionY");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightSunDirectionZ");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("WaterType");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("PKZoneFlag");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TileRenderingFlag");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SkyRenderingFlag");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SunRenderingFlag");
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("FogRenderingFlag");
	m_GridCurrent.SetItem(&Item);

	// arrange grid
	m_GridCurrent.AutoSize();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridCurrent.GetRowCount()-1, m_GridCurrent.GetColumnCount()-1);
	m_GridCurrent.ClearCells(tmpCellRange);
}

void CMapEditorDlg::InitGridUpdate()
{

	m_GridUpdate.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 41;

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
	Item.strText.Format("MapIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("RenderMapIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MapName");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MapInfluenceType");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityMapIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CitiWarQuestIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CitiWarQuestNPCIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CitiWarQuestMonsterUID");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxUserCount");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MaxGroundHeight");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MinGroundHeight");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("WaterHeight");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("UserVisibleDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MonsterVisibleDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("QuestIndexForWarp");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("FrontPositionDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("AutoRecoveryFlag");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DefaultMapIndex");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityWarpTargetPositionX");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityWarpTargetPositionY");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("CityWarpTargetPositionZ");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ClientMaxAltitudeHeight");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DayFogColor");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightFogColor");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DayFogStartDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DayFogEndDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightFogStartDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightFogEndDistance");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DaySunDirectionX");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DaySunDirectionY");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("DaySunDirectionZ");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightSunDirectionX");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightSunDirectionY");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NightSunDirectionZ");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("WaterType");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("PKZoneFlag");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("TileRenderingFlag");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SkyRenderingFlag");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("SunRenderingFlag");
	m_GridUpdate.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("FogRenderingFlag");
	m_GridUpdate.SetItem(&Item);

	// arrange grid
	m_GridUpdate.AutoSize();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridUpdate.GetRowCount()-1, m_GridUpdate.GetColumnCount()-1);
	m_GridUpdate.ClearCells(tmpCellRange);
}

void CMapEditorDlg::GetCurrentList()
{
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	//DB에서 SELECT
	CString szSQLQuery;	
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select MapIndex, RenderMapIndex, MapName, MapInfluenceType, CityMapIndex, CityWarQuestIndex, CityWarQuestNPCIndex, CityWarQuestMonsterUID, MaxUserCount, MaxGroundHeight, MinGroundHeight, WaterHeight, UserVisibleDistance, MonsterVisibleDistance, QuestIndexForWarp, FrontPositionDistance, AutoRecoveryFlag, DefaultMapIndex, CityWarpTargetPositionX, CityWarpTargetPositionY, CityWarpTargetPositionZ, ClientMaxAltitudeHeight, DayFogColor, NightFogColor, DayFogStartDistance, DayFogEndDistance, NightFogStartDistance, NightFogEndDistance, DaySunDirectionX, DaySunDirectionY, DaySunDirectionZ, NightSunDirectionX, NightSunDirectionY, NightSunDirectionZ, WaterType, PKZoneFlag, TileRenderingFlag, SkyRenderingFlag, SunRenderingFlag, FogRenderingFlag from ti_mapinfo");
	szSQLQuery.Format(QUERY_080702_0096);

	SQLINTEGER		arrCB[41] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								,SQL_NTS};
	_TI_MAPINFO		st_MapInfo;
	
	memset(&st_MapInfo, 0, sizeof(st_MapInfo));

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &st_MapInfo.MapIndex, 4,					&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &st_MapInfo.RenderMapIndex, 4,				&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, &st_MapInfo.MapName, 30,					&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_SHORT, &st_MapInfo.MapInfluenceType, 2,			&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &st_MapInfo.CitiMapIndex, 4,				&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &st_MapInfo.CitiWarQuestIndex, 4,			&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &st_MapInfo.CitiWarQuestNPCIndex, 4,		&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_ULONG, &st_MapInfo.CitiWarQuestMonsterUID, 4, 	&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_SSHORT, &st_MapInfo.MaxUserCount, 2,				&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_SSHORT, &st_MapInfo.MaxGroundHeight, 0,			&arrCB[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_SSHORT, &st_MapInfo.MinGroundHeight, 0,			&arrCB[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_SSHORT, &st_MapInfo.WaterHeight, 0,				&arrCB[12]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_SSHORT, &st_MapInfo.UserVisibleDistance, 0,		&arrCB[13]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_SSHORT, &st_MapInfo.MonsterVisibleDistance, 0,	&arrCB[14]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_ULONG, &st_MapInfo.QuestIndexForWarp, 0,			&arrCB[15]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_SSHORT, &st_MapInfo.FrontPositionDistance, 0,	&arrCB[16]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 17, SQL_C_UTINYINT, &st_MapInfo.AutoRecoveryFlag, 0,		&arrCB[17]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 18, SQL_C_ULONG, &st_MapInfo.DefaultMapIndex, 0,			&arrCB[18]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 19, SQL_C_SSHORT, &st_MapInfo.CitiWarpTargetPositionX, 0,	&arrCB[19]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 20, SQL_C_SSHORT, &st_MapInfo.CitiWarpTargetPositionY, 0,	&arrCB[20]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 21, SQL_C_SSHORT, &st_MapInfo.CitiWarpTargetPositionZ, 0,	&arrCB[21]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 22, SQL_C_SSHORT, &st_MapInfo.ClientMaxAltitudeHeight, 0,	&arrCB[22]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 23, SQL_C_ULONG, &st_MapInfo.DayFogColor, 0,				&arrCB[23]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 24, SQL_C_ULONG, &st_MapInfo.NightFogColor, 0,				&arrCB[24]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 25, SQL_C_SSHORT, &st_MapInfo.DayFogStartDistance, 0,		&arrCB[25]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 26, SQL_C_SSHORT, &st_MapInfo.DayFogEndDistance, 0,		&arrCB[26]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 27, SQL_C_SSHORT, &st_MapInfo.NightFogStartDistance, 0,	&arrCB[27]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 28, SQL_C_SSHORT, &st_MapInfo.NightFogEndDistance, 0,		&arrCB[28]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 29, SQL_C_SSHORT, &st_MapInfo.DaySunDirectionX, 0,			&arrCB[29]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 30, SQL_C_SSHORT, &st_MapInfo.DaySunDirectionY, 0,			&arrCB[30]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 31, SQL_C_SSHORT, &st_MapInfo.DaySunDirectionZ, 0,			&arrCB[31]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 32, SQL_C_SSHORT, &st_MapInfo.NightSunDirectionZ, 0,		&arrCB[32]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 33, SQL_C_SSHORT, &st_MapInfo.NightSunDirectionY, 0,		&arrCB[33]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 34, SQL_C_SSHORT, &st_MapInfo.NightSunDirectionZ, 0,		&arrCB[34]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 35, SQL_C_UTINYINT, &st_MapInfo.WaterType, 0,				&arrCB[35]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 36, SQL_C_UTINYINT, &st_MapInfo.PKZoneFlag, 0,				&arrCB[36]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 37, SQL_C_UTINYINT, &st_MapInfo.TileRenderingFlag, 0,		&arrCB[37]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 38, SQL_C_UTINYINT, &st_MapInfo.SkyRenderingFlag, 0,		&arrCB[38]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 39, SQL_C_UTINYINT, &st_MapInfo.SunRenderingFlag, 0,		&arrCB[39]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 40, SQL_C_UTINYINT, &st_MapInfo.FogRenderingFlag, 0,		&arrCB[40]);

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
	memset(&st_MapInfo, 0, sizeof(st_MapInfo));
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		m_GridCurrent.SetRowCount(nCount+2);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		InsertItemToGridCurrent(&st_MapInfo, nCount+1);

		nCount++;
		memset(&st_MapInfo, 0, sizeof(st_MapInfo));
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	m_GridCurrent.AutoSize();
}

void CMapEditorDlg::InsertItemToGridCurrent(_TI_MAPINFO *i_pTi_MapInfo, int i_nRow)
{
	// 칼럼 만들기
	int m_nCols = 0;

	// 컬럼에 값 넣기
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = ((DWORD)DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->MapIndex);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->RenderMapIndex);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pTi_MapInfo->MapName);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->MapInfluenceType);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiMapIndex);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiWarQuestIndex);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiWarQuestNPCIndex);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiWarQuestMonsterUID);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->MaxUserCount);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->MaxGroundHeight);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->MinGroundHeight);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->WaterHeight);
	m_GridCurrent.SetItem(&Item);
	/////////////////////////////////////////////////10 items

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->UserVisibleDistance);
	m_GridCurrent.SetItem(&Item);
	

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->MonsterVisibleDistance);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->QuestIndexForWarp);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->FrontPositionDistance);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->AutoRecoveryFlag);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DefaultMapIndex);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiWarpTargetPositionX);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiWarpTargetPositionY);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->CitiWarpTargetPositionZ);
	m_GridCurrent.SetItem(&Item);

	/////////////////////////////////////////////////20items
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->ClientMaxAltitudeHeight);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DayFogColor);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->NightFogColor);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DayFogStartDistance);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DayFogEndDistance);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->NightFogStartDistance);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->NightFogEndDistance);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DaySunDirectionX);
	m_GridCurrent.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DaySunDirectionY);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->DaySunDirectionZ);
	m_GridCurrent.SetItem(&Item);

	/////////////////////////////////////////////////30items
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->NightSunDirectionX);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->NightSunDirectionY);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->NightSunDirectionZ);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->WaterType);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->PKZoneFlag);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->TileRenderingFlag);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->SkyRenderingFlag);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->SunRenderingFlag);
	m_GridCurrent.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pTi_MapInfo->FogRenderingFlag);
	m_GridCurrent.SetItem(&Item);
}

void CMapEditorDlg::OnButtonView() 
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

	if (m_pMapDetailDlg != NULL)
	{
		util::del(m_pMapDetailDlg);
	}

	if (m_pMapDetailDlg == NULL)
	{
		m_pMapDetailDlg = new CMapEditorDetailDlg(this);
		m_pMapDetailDlg->Create(IDD_DIALOG_MAPDETAIL, this);
		m_pMapDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pMapDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
		m_pMapDetailDlg->m_ctrlEditMapIndex.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pMapDetailDlg->ShowWindow(SW_SHOW);
}

void CMapEditorDlg::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here
	if (m_pMapDetailDlg != NULL)
	{
		util::del(m_pMapDetailDlg);
	}

	if (m_pMapDetailDlg == NULL)
	{
		m_pMapDetailDlg = new CMapEditorDetailDlg(this);
		m_pMapDetailDlg->Create(IDD_DIALOG_MAPDETAIL, this);
		m_pMapDetailDlg->m_ctrlButtonSave.EnableWindow(FALSE);
		m_pMapDetailDlg->m_ctrlEditMapIndex.EnableWindow(TRUE);
	}

	m_pMapDetailDlg->ShowWindow(SW_SHOW);
}

void CMapEditorDlg::OnButtonUpdate() 
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

	if (m_pMapDetailDlg != NULL)
	{
		util::del(m_pMapDetailDlg);
	}

	if (m_pMapDetailDlg == NULL)
	{
		m_pMapDetailDlg = new CMapEditorDetailDlg(this);
		m_pMapDetailDlg->Create(IDD_DIALOG_MAPDETAIL, this);
		m_pMapDetailDlg->m_ctrlButtonMake.EnableWindow(FALSE);
		m_pMapDetailDlg->m_ctrlEditMapIndex.EnableWindow(FALSE);
	}
	SetDetailDialog(tmpCellID.row);
	m_pMapDetailDlg->ShowWindow(SW_SHOW);

	m_pMapDetailDlg->m_nSelectRow = tmpCellID.row;

	m_GridCurrent.Refresh();
}

void CMapEditorDlg::OnButtonDelete() 
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

void CMapEditorDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	m_GridCurrent.UpdateWindow();
}

void CMapEditorDlg::OnButtonCancel() 
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

void CMapEditorDlg::OnButtonServerupdate() 
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
			//szQuery.Format("delete ti_mapinfo where mapindex = %s", m_GridUpdate.GetItemText(1, 1));
			szQuery.Format(QUERY_080702_0097, m_GridUpdate.GetItemText(1, 1));

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Delete 1 Row Data");
				m_pODBCStmt->FreeStatement();
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "UPDATE")
		{

			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Update ti_mapinfo set RenderMapIndex = %s, MapName = '%s', MapInfluenceType = %s, CityMapIndex = %s, CityWarQuestIndex = %s, CityWarQuestNPCIndex = %s, CityWarQuestMonsterUID = %s, MaxUserCount = %s, MaxGroundHeight = %s, MinGroundHeight = %s, WaterHeight = %s, UserVisibleDistance = %s, MonsterVisibleDistance = %s, QuestIndexForWarp = %s, FrontPositionDistance = %s, AutoRecoveryFlag = %s, DefaultMapIndex = %s, CityWarpTargetPositionX = %s, CityWarpTargetPositionY = %s, CityWarpTargetPositionZ = %s, ClientMaxAltitudeHeight = %s, DayFogColor = %s, NightFogColor = %s, DayFogStartDistance = %s, DayFogEndDistance = %s, NightFogStartDistance = %s, NightFogEndDistance = %s, DaySunDirectionX = %s, DaySunDirectionY = %s, DaySunDirectionZ = %s, NightSunDirectionX = %s, NightSunDirectionY = %s, NightSunDirectionZ = %s, WaterType = %s, PKZoneFlag = %s, TileRenderingFlag = %s, SkyRenderingFlag = %s, SunRenderingFlag = %s, FogRenderingFlag = %s where MapIndex = %s"
			szQuery.Format(QUERY_080702_0098
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
				, m_GridUpdate.GetItemText(1, 40)
				, m_GridUpdate.GetItemText(1, 1)
				);

			bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
			TRACE(szQuery.Left(500));
			TRACE(szQuery.Right(500));

			if (!bRet)
			{
				// cleanup
				AfxMessageBox("Fail During Update 1 Row Data");
				m_pODBCStmt->FreeStatement();
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		else if(m_GridUpdate.GetItemText(1, 0) == "INSERT")
		{
			//Update ROW
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szQuery.Format("Insert into ti_mapinfo(MapIndex, RenderMapIndex, MapName, MapInfluenceType, CityMapIndex, CityWarQuestIndex, CityWarQuestNPCIndex, CityWarQuestMonsterUID, MaxUserCount, MaxGroundHeight, MinGroundHeight, WaterHeight, UserVisibleDistance, MonsterVisibleDistance, QuestIndexForWarp, FrontPositionDistance, AutoRecoveryFlag, DefaultMapIndex, CityWarpTargetPositionX, CityWarpTargetPositionY, CityWarpTargetPositionZ, ClientMaxAltitudeHeight, DayFogColor, NightFogColor, DayFogStartDistance, DayFogEndDistance, NightFogStartDistance, NightFogEndDistance, DaySunDirectionX, DaySunDirectionY, DaySunDirectionZ, NightSunDirectionX, NightSunDirectionY, NightSunDirectionZ, WaterType, PKZoneFlag, TileRenderingFlag, SkyRenderingFlag, SunRenderingFlag, FogRenderingFlag) values ( %s, %s, '%s', %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)"				
			szQuery.Format(QUERY_080702_0099				
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
				, m_GridUpdate.GetItemText(1, 40)
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
				m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);
				return;
			}	
			m_pMainDlg->m_pWriteLog->WriteLogFile((char*)(LPCTSTR)szQuery, szQuery.GetLength(), m_pMainDlg);

			m_GridUpdate.DeleteRow(1);
			m_GridUpdate.Refresh();
		}
		
	}
	// cleanup
	m_pODBCStmt->FreeStatement();
	GetCurrentList();
}

void CMapEditorDlg::ChangeGridDate(CGridCtrl *oriGrid, CGridCtrl *newGrid, int type, int row)
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

void CMapEditorDlg::SetDetailDialog(int row)
{
	m_pMapDetailDlg->m_valueEditMapIndex				= m_GridCurrent.GetItemText(row,0);
	m_pMapDetailDlg->m_valueEditRenderMapIndex			= m_GridCurrent.GetItemText(row,1);
	m_pMapDetailDlg->m_valueEditMapName					= m_GridCurrent.GetItemText(row,2);
	m_pMapDetailDlg->m_valueEditMapInfluenceType		= m_GridCurrent.GetItemText(row,3);
	m_pMapDetailDlg->m_valueEditCityMapIndex			= m_GridCurrent.GetItemText(row,4);
	m_pMapDetailDlg->m_valueEditCityWarQuestIndex		= m_GridCurrent.GetItemText(row,5);
	m_pMapDetailDlg->m_valueEditCityWarQuestNPCIndex	= m_GridCurrent.GetItemText(row,6);
	m_pMapDetailDlg->m_valueEditCityWarQuestMonsterUID	= m_GridCurrent.GetItemText(row,7);
	m_pMapDetailDlg->m_valueEditMaxUserCount			= m_GridCurrent.GetItemText(row,8);
	m_pMapDetailDlg->m_valueEditMaxGroundHeight			= m_GridCurrent.GetItemText(row,9);
	m_pMapDetailDlg->m_valueEditMinGroundHeight			= m_GridCurrent.GetItemText(row,10);
	m_pMapDetailDlg->m_valueEditWaterHeight				= m_GridCurrent.GetItemText(row,11);
	m_pMapDetailDlg->m_valueEditUserVisibleDistance		= m_GridCurrent.GetItemText(row,12);
	m_pMapDetailDlg->m_valueEditMonsterVisibleDistance	= m_GridCurrent.GetItemText(row,13);
	m_pMapDetailDlg->m_valueEditQuestIndexForWarp		= m_GridCurrent.GetItemText(row,14);
	m_pMapDetailDlg->m_valueEditDefaultMapIndex			= m_GridCurrent.GetItemText(row,15);
	m_pMapDetailDlg->m_valueEditFrontPositionDistance	= m_GridCurrent.GetItemText(row,16);
	m_pMapDetailDlg->m_valueEditAutoRecoveryFlag		= m_GridCurrent.GetItemText(row,17);
	m_pMapDetailDlg->m_valueEditCityWarpTargetPositionX = m_GridCurrent.GetItemText(row,18);
	m_pMapDetailDlg->m_valueEditCityWarpTargetPositionY = m_GridCurrent.GetItemText(row,19);
	m_pMapDetailDlg->m_valueEditCityWarpTargetPositionZ = m_GridCurrent.GetItemText(row,20);
	m_pMapDetailDlg->m_valueEditClientMaxAltitudeHeight = m_GridCurrent.GetItemText(row,21);
	m_pMapDetailDlg->m_valueEditDayFogColor				= m_GridCurrent.GetItemText(row,22);
	m_pMapDetailDlg->m_valueEditNightFogColor			= m_GridCurrent.GetItemText(row,23);
	m_pMapDetailDlg->m_valueEditDayFogStartDistance		= m_GridCurrent.GetItemText(row,24);
	m_pMapDetailDlg->m_valueEditDayFogEndDistance		= m_GridCurrent.GetItemText(row,25);
	m_pMapDetailDlg->m_valueEditNightFogStartDistance	= m_GridCurrent.GetItemText(row,26);
	m_pMapDetailDlg->m_valueEditNightFogEndDistance		= m_GridCurrent.GetItemText(row,27);
	m_pMapDetailDlg->m_valueEditDaySunDirectionX		= m_GridCurrent.GetItemText(row,28);
	m_pMapDetailDlg->m_valueEditDaySunDirectionY		= m_GridCurrent.GetItemText(row,29);
	m_pMapDetailDlg->m_valueEditDaySunDirectionZ		= m_GridCurrent.GetItemText(row,30);
	m_pMapDetailDlg->m_valueEditNightSunDirectionX		= m_GridCurrent.GetItemText(row,31);
	m_pMapDetailDlg->m_valueEditNightSunDirectionY		= m_GridCurrent.GetItemText(row,32);
	m_pMapDetailDlg->m_valueEditNightSunDirectionZ		= m_GridCurrent.GetItemText(row,33);
	m_pMapDetailDlg->m_valueEditWaterType				= m_GridCurrent.GetItemText(row,34);
	m_pMapDetailDlg->m_valueEditPKZoneFlag				= m_GridCurrent.GetItemText(row,35);
	m_pMapDetailDlg->m_valueEditTileRenderingFlag		= m_GridCurrent.GetItemText(row,36);
	m_pMapDetailDlg->m_valueEditSkyRenderingFlag		= m_GridCurrent.GetItemText(row,37);
	m_pMapDetailDlg->m_valueEditSunRenderingFlag		= m_GridCurrent.GetItemText(row,38);
	m_pMapDetailDlg->m_valueEditFogRenderingFlag		= m_GridCurrent.GetItemText(row,39);

	m_pMapDetailDlg->UpdateData(FALSE);
}
