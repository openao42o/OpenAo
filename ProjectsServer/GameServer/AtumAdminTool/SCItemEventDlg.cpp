// SCItemEventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "SCItemEventDlg.h"
#include "AtumAdminToolDlg.h"
#include "SetItemEv.h"
#include "atumsj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCItemEventDlg dialog


CSCItemEventDlg::CSCItemEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCItemEventDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCItemEventDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pODBCStmt = new CODBCStatement;

}
CSCItemEventDlg::~CSCItemEventDlg()
{
	util::del(m_pODBCStmt);
}

void CSCItemEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCItemEventDlg)
	DDX_Control(pDX, IDC_GRID_ITEM_EVENT_LIST, m_GridList);	
	DDX_Control(pDX, IDC_COMBO_SERVER_GROUP, m_comboServerGroupID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSCItemEventDlg, CDialog)
	//{{AFX_MSG_MAP(CSCItemEventDlg)
	ON_COMMAND(IDR_CONMENU_SELECTED_INSERT, OnConmenuSelectedInsert)
	ON_COMMAND(IDR_CONMENU_NOTSELECTED_INSERT, OnConmenuNotselectedInsert)
	ON_COMMAND(IDR_CONMENU_SELECTED_UPDATE, OnConmenuSelectedUpdate)
	ON_COMMAND(IDR_CONMENU_SELECTED_DELETE, OnConmenuSelectedDelete)
	ON_BN_CLICKED(IDC_ITEM_EVENT_ADMIN_TOOL_BUTTON_OK, OnItemEventAdminToolButtonOk)
	ON_BN_CLICKED(IDC_RELOAD_ITEM_EVENT_BUTTON, OnReloadItemEventButton)
	ON_BN_CLICKED(IDC_BUTTON_LUCKYMACHINE_INIT, OnLuckyMachineInitButton)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_ITEM_EVENT_LIST, OnGridRClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCItemEventDlg message handlers

BOOL CSCItemEventDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comboServerGroupID.AddString(STR_ALL_SERVER_GROUP_STRING);
	for(int i=0; i < SIZE_MAX_SERVER_GROUP_COUNT; i++)
	{
		char szTemp[512]={0x00,};
		m_comboServerGroupID.AddString(itoa(SERVERGROUP_START_ID+i, szTemp, 10));
	}
//	m_comboServerGroupID.SetCurSel(SERVERGROUP_START_ID);
	
	// Connect DB
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}
	
	CAtumDBHelper::LoadItemInfo(m_pODBCStmt, &m_mapItemInfo, &m_mapItemInfoName);
	CAtumDBHelper::LoadRareItemInfo(m_pODBCStmt, &m_mapRareItemInfo,  &m_mapRareItemInfoName);

	//현재 DB에 있는 값 불러온다.
	GetItemEventInfo();

	// 2012-02-01 by jhseol, 베트남 GM 권한 제한
#ifdef S_MANAGER_ADMIN_HSSON
	GetDlgItem(IDC_RELOAD_ITEM_EVENT_BUTTON)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));
	GetDlgItem(IDC_BUTTON_LUCKYMACHINE_INIT)->EnableWindow(COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION));
#endif
	// end 2012-02-01 by jhseol, 베트남 GM 권한 제한

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSCItemEventDlg::GetItemEventInfo()
{
	this->InitGrid();

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 1;
	Item.nFormat = GRID_CELL_FORMAT;
	
	CString szSQLQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	szSQLQuery.Format("SELECT * FROM ti_ItemEvent WITH (NOLOCK) order by StartTime");

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0029);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	/*-------------------------------------------------------------------------------
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0029
	-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	--						-- // 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 필드 추가 한 것 검색 결과에 추가 및 결과 필드명 몇시
	--------------------------------------------------------------------------------
	call dbo.atum_PROCEDURE_080827_0029
	-------------------------------------------------------------------------------*/

	SQLINTEGER arrCB[25] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
		,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
		,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	SITEM_EVENT_INFO tmItemEv;	
	util::zero(&tmItemEv, sizeof(tmItemEv));

	SQL_TIMESTAMP_STRUCT tmpStartTime;
	SQL_TIMESTAMP_STRUCT tmpEndTime;
	SQL_TIMESTAMP_STRUCT tmpLastGameEndDate;	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	util::zero(&tmpStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&tmpEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&tmpLastGameEndDate, sizeof(SQL_TIMESTAMP_STRUCT));

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmItemEv.ItemEventUID, 0,					&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &tmItemEv.ItemEventType, 0,				&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmItemEv.OnlyPCBang, 0,					&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &tmItemEv.byInfluenceType, 0,			&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG, &tmItemEv.ItemNum, 0,						&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG, &tmItemEv.Count, 0,						&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_TIMESTAMP, &tmpStartTime, 0,						&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_TIMESTAMP, &tmpEndTime, 0,						&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_BIT, &tmItemEv.MemberShip, 0,						&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_TINYINT, &tmItemEv.ReqLevel, 0,					&arrCB[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_BIT, &tmItemEv.NewMember, 0,						&arrCB[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_ULONG, &tmItemEv.UnitKind, 0,					&arrCB[12]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_ULONG, &tmItemEv.PrefixCodeNum, 0,				&arrCB[13]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_ULONG, &tmItemEv.SuffixCodeNum, 0,				&arrCB[14]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_ULONG, &tmItemEv.UseLastGameEndDate, 0,			&arrCB[15]);	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_TIMESTAMP, &tmpLastGameEndDate, 0,				&arrCB[16]);	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	SQLBindCol(m_pODBCStmt->m_hstmt, 17, SQL_C_LONG, &tmItemEv.CheckWithCharacterUID, 0,		&arrCB[17]);	// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
	SQLBindCol(m_pODBCStmt->m_hstmt, 18, SQL_C_ULONG, &tmItemEv.iLevelMin, 0,					&arrCB[18]);	// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - 
	SQLBindCol(m_pODBCStmt->m_hstmt, 19, SQL_C_ULONG, &tmItemEv.iLevelMax, 0,					&arrCB[19]);	// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
	SQLBindCol(m_pODBCStmt->m_hstmt, 20, SQL_C_ULONG, &tmItemEv.LoginCheckNumber, 0,			&arrCB[20]);	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 -

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	SQLBindCol(m_pODBCStmt->m_hstmt, 21, SQL_C_ULONG, &tmItemEv.UseFixedPeriod, 0,			&arrCB[21]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 22, SQL_C_ULONG, &tmItemEv.FixedPeriod_DAY, 0,			&arrCB[22]);
#endif

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0029));
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return;
	}

	m_mtvectITEM_EVENT_INFO.clear();			// 2006-08-25 by cmkwon, 
	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		tmItemEv.StartTime		= tmpStartTime;
		tmItemEv.EndTime		= tmpEndTime;
		tmItemEv.atLastGameEndDate	= tmpLastGameEndDate;	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
		m_mtvectITEM_EVENT_INFO.push_back(tmItemEv);

		util::zero(&tmItemEv, sizeof(tmItemEv));
		util::zero(&tmpStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&tmpEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
		util::zero(&tmpLastGameEndDate, sizeof(SQL_TIMESTAMP_STRUCT));
	}
	m_pODBCStmt->FreeStatement();	// clean up

	// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	vectItemEventInfo::iterator itr = m_mtvectITEM_EVENT_INFO.begin();
	for ( ; itr != m_mtvectITEM_EVENT_INFO.end() ; itr++ )
	{
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &itr->ItemEventUID, 0, NULL);
		BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_130329_0005));
		if (!bRet)
		{
			m_pODBCStmt->FreeStatement();	// clean up
			continue;
		}
		SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_BIT,	&itr->SubEventType, 0,				&arrCB[1]);
		for (int i = 0 ; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT ; i++ )
		{
			SQLBindCol(m_pODBCStmt->m_hstmt, 2+(2*i), SQL_C_USHORT, &itr->ArrDestParameter[i], 0,			&arrCB[2+(2*i)]);
			SQLBindCol(m_pODBCStmt->m_hstmt, 3+(2*i), SQL_C_ULONG, &itr->ArrParameterValue[i], 0,			&arrCB[3+(2*i)]);
		}
		SQLFetch(m_pODBCStmt->m_hstmt);
		m_pODBCStmt->FreeStatement();	// clean up
	}
#endif
	// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경

	CString szItemEvType, szPCBangType, szInfluenceType, szMemberShipType, szNewMemberType, szUnitkind;
	
	for(int i=0; i < m_mtvectITEM_EVENT_INFO.size(); i++)
	{
		int nNewRowIdx = m_GridList.GetRowCount();
		m_GridList.SetRowCount(nNewRowIdx+1);

		if (ITEM_EVENT_TYPE_ONLYONE == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{
			szItemEvType.Format("ONCE");
		}
		
		if (ITEM_EVENT_TYPE_ONEDAY == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{
			szItemEvType.Format("DAILY");
		}

		if (ITEM_EVENT_TYPE_INFLCHANGE == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{
			szItemEvType.Format("InflChange");
		}
		if (ITEM_EVENT_TYPE_LEVELUP == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{
			szItemEvType.Format("LevelUp");
		}
		if (ITEM_EVENT_TYPE_COUPON_ONLYONE == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{// 2008-01-11 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
			szItemEvType.Format("Coupon Once");
		}
		if (ITEM_EVENT_TYPE_COUPON_ONEDAY == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{// 2008-01-11 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
			szItemEvType.Format("Coupon Daily");
		}
		if(ITEM_EVENT_TYPE_LOGIN == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
			szItemEvType.Format("Login Event");
		}
		if(ITEM_EVENT_TYPE_FIRST_PURCHASE == m_mtvectITEM_EVENT_INFO[i].ItemEventType)
		{// 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
			szItemEvType.Format("First Purchase");
		}

		if (TRUE == m_mtvectITEM_EVENT_INFO[i].OnlyPCBang)
		{
			szPCBangType.Format("TRUE");
		}
		
		else 
		{
			szPCBangType.Format("FALSE");
		}

		if (INFLUENCE_TYPE_NORMAL == m_mtvectITEM_EVENT_INFO[i].byInfluenceType)
		{
			szInfluenceType.Format("NORMAL");
		}

		if (INFLUENCE_TYPE_VCN == m_mtvectITEM_EVENT_INFO[i].byInfluenceType)
		{
			szInfluenceType.Format("VCN");
		}

		if (INFLUENCE_TYPE_ANI == m_mtvectITEM_EVENT_INFO[i].byInfluenceType)
		{
			szInfluenceType.Format("ANI");
		}

		if (INFLUENCE_TYPE_ALL_MASK == m_mtvectITEM_EVENT_INFO[i].byInfluenceType)
		{
			szInfluenceType.Format("ALL");
		}

		if (TRUE == m_mtvectITEM_EVENT_INFO[i].MemberShip)
		{
			szMemberShipType.Format("TRUE");
		}
		else
		{
			szMemberShipType.Format("FALSE");
		}

		if (TRUE == m_mtvectITEM_EVENT_INFO[i].NewMember)
		{
			szNewMemberType.Format("TRUE");
		}
		else
		{
			szNewMemberType.Format("FALSE");
		}
		
		// 2007-07-25 by dhjin, 이벤트 대상 기어
		if(UNITKIND_ALL_MASK == m_mtvectITEM_EVENT_INFO[i].UnitKind)
		{
			szUnitkind.Format("All Gear");
		}
		else
		{
			szUnitkind.Format("");
			if(UNITKIND_BGEAR_MASK & m_mtvectITEM_EVENT_INFO[i].UnitKind)
			{
				szUnitkind = szUnitkind +"B ";
			}
			if(UNITKIND_MGEAR_MASK & m_mtvectITEM_EVENT_INFO[i].UnitKind)
			{
				szUnitkind = szUnitkind +"M ";
			}
			if(UNITKIND_AGEAR_MASK & m_mtvectITEM_EVENT_INFO[i].UnitKind)
			{
				szUnitkind = szUnitkind +"A ";
			}
			if(UNITKIND_IGEAR_MASK & m_mtvectITEM_EVENT_INFO[i].UnitKind)
			{
				szUnitkind = szUnitkind +"I ";
			}
			szUnitkind = szUnitkind + "Gear";
		}

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].ItemEventUID);
		m_GridList.SetItem(&Item);
					
		Item.col++;
		Item.strText.Format("%s", szItemEvType);
		m_GridList.SetItem(&Item);

		// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
		Item.col++;
		Item.strText.Format("%s (%d)", m_mtvectITEM_EVENT_INFO[i].GetDestParameterShotStr(m_mtvectITEM_EVENT_INFO[i].ArrDestParameter[0]), m_mtvectITEM_EVENT_INFO[i].ArrParameterValue[0]);
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%s (%d)", m_mtvectITEM_EVENT_INFO[i].GetDestParameterShotStr(m_mtvectITEM_EVENT_INFO[i].ArrDestParameter[1]), m_mtvectITEM_EVENT_INFO[i].ArrParameterValue[1]);
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%s (%d)", m_mtvectITEM_EVENT_INFO[i].GetDestParameterShotStr(m_mtvectITEM_EVENT_INFO[i].ArrDestParameter[2]), m_mtvectITEM_EVENT_INFO[i].ArrParameterValue[2]);
		m_GridList.SetItem(&Item);
#endif
		// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
		
		Item.col++;
		Item.strText.Format("%s", szPCBangType);
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%s", szMemberShipType);
		m_GridList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%s", szNewMemberType);
		m_GridList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(m_mtvectITEM_EVENT_INFO[i].byInfluenceType));
		m_GridList.SetItem(&Item);

		Item.col++;
		if(m_mapItemInfo.findEZ_ptr(m_mtvectITEM_EVENT_INFO[i].ItemNum))
		{
			Item.strText.Format("[%d]  %s", m_mtvectITEM_EVENT_INFO[i].ItemNum, m_mapItemInfo.findEZ_ptr(m_mtvectITEM_EVENT_INFO[i].ItemNum)->ItemName);
		}
		else
		{
			Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].ItemNum);
		}
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].Count);
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%s", m_mtvectITEM_EVENT_INFO[i].StartTime.GetDateTimeString().GetBuffer());
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%s", m_mtvectITEM_EVENT_INFO[i].EndTime.GetDateTimeString().GetBuffer());
		m_GridList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].ReqLevel);
		m_GridList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%s", szUnitkind);
		m_GridList.SetItem(&Item);	

		Item.col++;
		if(m_mapRareItemInfo.findEZ_ptr(m_mtvectITEM_EVENT_INFO[i].PrefixCodeNum))
		{
			Item.strText.Format("[%d]  %s", m_mtvectITEM_EVENT_INFO[i].PrefixCodeNum, m_mapRareItemInfo.findEZ_ptr(m_mtvectITEM_EVENT_INFO[i].PrefixCodeNum)->Name);
		}
		else
		{
			Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].PrefixCodeNum);
		}
		m_GridList.SetItem(&Item);

		Item.col++;
		if(m_mapRareItemInfo.findEZ_ptr(m_mtvectITEM_EVENT_INFO[i].SuffixCodeNum))
		{
			Item.strText.Format("[%d]  %s", m_mtvectITEM_EVENT_INFO[i].SuffixCodeNum, m_mapRareItemInfo.findEZ_ptr(m_mtvectITEM_EVENT_INFO[i].SuffixCodeNum)->Name);
		}
		else
		{
			Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].SuffixCodeNum);
		}
		m_GridList.SetItem(&Item);

		// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
		Item.col++;
		Item.strText.Format("%s", m_mtvectITEM_EVENT_INFO[i].UseLastGameEndDate==FALSE ? "FALSE" : "TRUE");
		m_GridList.SetItem(&Item);
		
		Item.col++;
		Item.strText.Format("%s", m_mtvectITEM_EVENT_INFO[i].atLastGameEndDate.GetDateTimeString().GetBuffer());
		m_GridList.SetItem(&Item);	
 
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
		Item.col++;
		Item.strText.Format("%s", m_mtvectITEM_EVENT_INFO[i].UseFixedPeriod==FALSE ? "FALSE" : "TRUE");
		m_GridList.SetItem(&Item);	
		
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].FixedPeriod_DAY);
		m_GridList.SetItem(&Item);	

		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].CheckWithCharacterUID);
		m_GridList.SetItem(&Item);
		
		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].iLevelMin);
		m_GridList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].iLevelMax);
		m_GridList.SetItem(&Item);
		//end 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -

		// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].LoginCheckNumber);
		m_GridList.SetItem(&Item);
#else
		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].CheckWithCharacterUID);
		m_GridList.SetItem(&Item);	
		
		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].iLevelMin);
		m_GridList.SetItem(&Item);	
		
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].iLevelMax);
		m_GridList.SetItem(&Item);
		//end 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 -
		
		// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
		Item.col++;
		Item.strText.Format("%d", m_mtvectITEM_EVENT_INFO[i].LoginCheckNumber);
		m_GridList.SetItem(&Item);
#endif
	}	

	m_GridList.UpdateData();
	m_GridList.AutoSize();

}

void CSCItemEventDlg::InitGrid()
{
	m_GridList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 20;		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - // 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 필드 추가 // 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현

	m_GridList.SetEditable(FALSE);
	m_GridList.SetListMode(TRUE);
	m_GridList.SetSingleRowSelection(TRUE);
	m_GridList.EnableSelection(TRUE);
	m_GridList.SetFrameFocusCell(FALSE);
	m_GridList.SetTrackFocusCell(FALSE);

	m_GridList.SetRowCount(m_nRows);
	m_GridList.SetColumnCount(m_nCols);
	m_GridList.SetFixedRowCount(1);
// 2008-02-01 by cmkwon, ItemEventUID 도 보이게 수정한다.
//	m_GridList.SetColumnWidth(0,0);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	Item.col = m_nCols++;
	Item.strText.Format("ItemEventUID");
	m_GridList.SetItem(&Item);

	// 2006-08-25 by cmkwon
	Item.col = m_nCols++;
	Item.strText.Format("ItemEventType");
	m_GridList.SetItem(&Item);

	// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	Item.col = m_nCols++;
	Item.strText.Format("SubParam1");
	m_GridList.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("SubParam2");
	m_GridList.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("SubParam3");
	m_GridList.SetItem(&Item);
#endif
	// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경

	Item.col = m_nCols++;
	Item.strText.Format("OnlyPCBang");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MemberShip");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("NewMember");
	m_GridList.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("InfluenceType");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ItemNumInfo");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Count");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("StartTime");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("EndTime");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("ReqLevel");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("UnitKind");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Prefix");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Suffix");
	m_GridList.SetItem(&Item);

	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	Item.col = m_nCols++;
	Item.strText.Format("UseLastGameEndDate");
	m_GridList.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format("LastGameEndDate");
	m_GridList.SetItem(&Item);

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	Item.col = m_nCols++;
	Item.strText.Format("UseFixedPeriod");
	m_GridList.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("FixedPeriod_DAY");
	m_GridList.SetItem(&Item);
#endif

	// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
	Item.col = m_nCols++;
	Item.strText.Format("CheckWithCharacterUID");
	m_GridList.SetItem(&Item);
	
	Item.col = m_nCols++;
	Item.strText.Format("iLevelMin");
	m_GridList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("iLevelMax");
	m_GridList.SetItem(&Item);

	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 컬럼 추가
	Item.col = m_nCols++;
	Item.strText.Format("LoginCheckNumber");
	m_GridList.SetItem(&Item);

	// arrange grid
	m_GridList.AutoSize();
	m_GridList.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridList.GetRowCount()-1, m_GridList.GetColumnCount()-1);
	m_GridList.ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCItemEventDlg::CheckEnbaleInsertCouponEvent(int i_nItemEvTy, int i_nExcludeItemEventUID/*=0*/)
/// \brief		// 2008-01-11 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - CSCItemEventDlg::CheckEnbaleInsertCouponEvent() 추가
/// \author		cmkwon
/// \date		2008-01-11 ~ 2008-01-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCItemEventDlg::CheckEnbaleInsertCouponEvent(int i_nItemEvTy, int i_nExcludeItemEventUID/*=0*/)
{
	if(ITEM_EVENT_TYPE_COUPON_ONLYONE != i_nItemEvTy && ITEM_EVENT_TYPE_COUPON_ONEDAY != i_nItemEvTy)
	{// 2008-01-11 by cmkwon, 쿠폰 이벤트가 아니면 무저건 TRUE 를 리턴
		return TRUE;
	}

	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("SELECT ItemEventUID FROM dbo.ti_ItemEvent WHERE ItemEventUID <> %d AND (ItemEventType = %d OR ItemEventType = %d)"
// 		, i_nExcludeItemEventUID, ITEM_EVENT_TYPE_COUPON_ONLYONE, ITEM_EVENT_TYPE_COUPON_ONEDAY);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 	
//	szQuery.Format(QUERY_080702_0030, i_nExcludeItemEventUID, ITEM_EVENT_TYPE_COUPON_ONLYONE, ITEM_EVENT_TYPE_COUPON_ONEDAY);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szQuery);
	int nItemEvTy1 = ITEM_EVENT_TYPE_COUPON_ONLYONE;
	int nItemEvTy2 = ITEM_EVENT_TYPE_COUPON_ONEDAY;
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_nExcludeItemEventUID), 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(nItemEvTy1), 0,					&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(nItemEvTy2), 0,					&arrCB2[3]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0030));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup
		return FALSE;
	}

	int nCouponItemEventUID = 0;
	SQLINTEGER arrCB[2]		= {SQL_NTS,SQL_NTS};
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &nCouponItemEventUID, 0,					&arrCB[1]);
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA){};
	m_pODBCStmt->FreeStatement();	// cleanup

	if(0 != nCouponItemEventUID)
	{
		return FALSE;
	}

	return TRUE;
}

void CSCItemEventDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	UpdateData();

    NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	if (pGridView->iRow == 0)
	{// 2006-08-25 by cmkwon, 목차행 선택
		return;
	}

	// 2012-02-01 by jhseol, 베트남 GM 권한 제한
#ifdef S_MANAGER_ADMIN_HSSON
	if(FALSE == COMPARE_RACE(m_pMainDlg->m_usManagerAccountType, RACE_OPERATION))
	{
		AfxMessageBox("You are not have permission !!");
		return;
	}
#endif
	// end 2012-02-01 by jhseol, 베트남 GM 권한 제한
	
	m_GridList.SetFocusCell(-1,-1);
	m_GridList.SetSelectedRange(pGridView->iRow, 0, pGridView->iRow, m_GridList.GetColumnCount()-1, TRUE, TRUE);
	m_GridList.SetFocusCell(pGridView->iRow, pGridView->iColumn);

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_HAPPYEV);
	CPoint point;
	GetCursorPos(&point);

	if(pGridView->iRow > 0)
	{// 2006-08-25 by cmkwon, 유효한 행을 선택
		pContextMenu = muTemp.GetSubMenu(0);
	}
	else
	{// 2006-08-25 by cmkwon, 유효하지 않은 행을 선택
		pContextMenu = muTemp.GetSubMenu(1);
	}
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);	
}

void CSCItemEventDlg::OnConmenuSelectedInsert()
{
	CSetItemEv	dlg(TRUE,this);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-11 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 추가 가능한지 체크
	if(FALSE == this->CheckEnbaleInsertCouponEvent(dlg.m_ItemEv.ItemEventType))
	{
		MessageBox("ItemEvent insert error !!, You can't make more coupon event.");
		return;
	}

	/*-------------------------------------------------------------------------------
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0266
	-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	--						-- // 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 인자 추가 및 필드 추가 처리
	--------------------------------------------------------------------------------
	CALL dbo.atum_PROCEDURE_080827_0266
	-------------------------------------------------------------------------------*/
//	CString szQuery;
// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
// 	// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
// 	szQuery.Format("INSERT into ti_ItemEvent values( %d, %d, %d, %d, %d,'%s', '%s', %d, %d, %d, %d, %d, %d)"
// 		, dlg.m_ItemEv.ItemEventType, dlg.m_ItemEv.OnlyPCBang, dlg.m_ItemEv.byInfluenceType
// 		, dlg.m_ItemEv.ItemNum, dlg.m_ItemEv.Count
// 		, dlg.m_ItemEv.StartTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_ItemEv.EndTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
// 		, dlg.m_ItemEv.MemberShip, dlg.m_ItemEv.ReqLevel, dlg.m_ItemEv.NewMember, dlg.m_ItemEv.UnitKind
// 		, dlg.m_ItemEv.PrefixCodeNum, dlg.m_ItemEv.SuffixCodeNum);
	// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("INSERT into ti_ItemEvent values( %d, %d, %d, %d, %d,'%s', '%s', %d, %d, %d, %d, %d, %d, %d, '%s')"
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0266
//		, dlg.m_ItemEv.ItemEventType, dlg.m_ItemEv.OnlyPCBang, dlg.m_ItemEv.byInfluenceType
//		, dlg.m_ItemEv.ItemNum, dlg.m_ItemEv.Count
//		, dlg.m_ItemEv.StartTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_ItemEv.EndTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//		, dlg.m_ItemEv.MemberShip, dlg.m_ItemEv.ReqLevel, dlg.m_ItemEv.NewMember, dlg.m_ItemEv.UnitKind
//		, dlg.m_ItemEv.PrefixCodeNum, dlg.m_ItemEv.SuffixCodeNum, dlg.m_ItemEv.UseLastGameEndDate
//		, dlg.m_ItemEv.atLastGameEndDate.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)));
//		bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0265));
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.ItemEventType, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.OnlyPCBang, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &dlg.m_ItemEv.byInfluenceType, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.ItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.Count, 0, NULL);
	char szATStart[SIZE_MAX_SQL_DATETIME_STRING];
	dlg.m_ItemEv.StartTime.GetSQLDateTimeString(szATStart, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATStart, 0, NULL);
	char szATEnd[SIZE_MAX_SQL_DATETIME_STRING];
	dlg.m_ItemEv.EndTime.GetSQLDateTimeString(szATEnd, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATEnd, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &dlg.m_ItemEv.MemberShip, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &dlg.m_ItemEv.ReqLevel, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &dlg.m_ItemEv.NewMember, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.UnitKind, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.PrefixCodeNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.SuffixCodeNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.UseLastGameEndDate, 0, NULL);
	char szLastGameEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	dlg.m_ItemEv.atLastGameEndDate.GetSQLDateTimeString(szLastGameEndDate, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szLastGameEndDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.CheckWithCharacterUID, 0, NULL);		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.iLevelMin, 0, NULL);		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.iLevelMax, 0, NULL);		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.LoginCheckNumber, 0, NULL);		// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 -

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	SQLBindParameter(m_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.UseFixedPeriod, 0, NULL);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.FixedPeriod_DAY, 0, NULL);	
#endif

	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0266));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		MessageBox("ItemEvent insert error !!");
		return;
	}

	// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	SQLINTEGER arrCB[2] = {SQL_NTS,SQL_NTS};
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &dlg.m_ItemEv.ItemEventUID, 0,	&arrCB[1]);
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		if ( FALSE == dlg.m_ItemEv.SubEventType )
		{
			break;
		}
		SQLNumResultCols(m_pODBCStmt->m_hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			bRet = SQLMoreResults(m_pODBCStmt->m_hstmt);
			if(bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		bRet = SQLFetch(m_pODBCStmt->m_hstmt);
		if(bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO)
		{
			bRet = SQLMoreResults(m_pODBCStmt->m_hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		if ( FALSE == InsetSubTypeInfo(dlg.m_ItemEv) )
		{
			MessageBox("ItemEvent insert error _ SubTypeInfo !!");
			m_pODBCStmt->FreeStatement();	// cleanup
			return;
		}
	}
#endif
	// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
	
	m_pODBCStmt->FreeStatement();	// cleanup

	this->GetItemEventInfo();
}

void CSCItemEventDlg::OnConmenuNotselectedInsert()
{
	OnConmenuSelectedInsert();
}

void CSCItemEventDlg::OnConmenuSelectedDelete()
{
	CCellID cellID = m_GridList.GetFocusCell();
	if(0 >= cellID.row)
	{
		return;
	}

	int nItemEvUID				= atoi(m_GridList.GetItemText(cellID.row, 0));
	SITEM_EVENT_INFO	*pItemEv = this->FindItemEvent(nItemEvUID);

	if(NULL == pItemEv)
	{
		return;
	}

	CString szQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("DELETE ti_ItemEvent WHERE ItemEventUID = %d", pItemEv->ItemEventUID);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0267, pItemEv->ItemEventUID);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemEv->ItemEventUID, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0267));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		MessageBox("ItemEvent delete error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();		// cleanup

	this->GetItemEventInfo();
}

SITEM_EVENT_INFO * CSCItemEventDlg::FindItemEvent(int i_nItemEvUID)
{
	for(int i=0; i < m_mtvectITEM_EVENT_INFO.size(); i++)
	{
		if(i_nItemEvUID == m_mtvectITEM_EVENT_INFO[i].ItemEventUID)
		{
			return &m_mtvectITEM_EVENT_INFO[i];
		}
	}
	
	return NULL;
}

void CSCItemEventDlg::OnConmenuSelectedUpdate()
{
	CCellID cellID = m_GridList.GetFocusCell();
	if(0 >= cellID.row)
	{// 2006-08-25 by cmkwon, 선택 오류
		return;
	}

	int nItemEvUID				= atoi(m_GridList.GetItemText(cellID.row, 0));
	SITEM_EVENT_INFO	*pItemEv = this->FindItemEvent(nItemEvUID);
	if(NULL == pItemEv)
	{
		return;
	}

	CSetItemEv	dlg(FALSE, this);
	dlg.m_ItemEv = *pItemEv;
	dlg.m_nItemPrefixCodeNum = pItemEv->PrefixCodeNum;
	dlg.m_nItemSuffixCodeNum = pItemEv->SuffixCodeNum;
	dlg.m_nItemKind			 = m_mapItemInfo.findEZ_ptr(pItemEv->ItemNum)->Kind;

	if (IDOK != dlg.DoModal())
	{
		return;
	}
	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-11 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 추가 가능한지 체크
	if(FALSE == this->CheckEnbaleInsertCouponEvent(dlg.m_ItemEv.ItemEventType, pItemEv->ItemEventUID))
	{
		MessageBox("ItemEvent update error !!, You can't make more coupon event.");
		return;
	}

	/*-------------------------------------------------------------------------------
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0268
	-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	--						-- // 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 인자 추가 및 필드 추가 처리
	--------------------------------------------------------------------------------
	CALL dbo.atum_PROCEDURE_080827_0268
	-------------------------------------------------------------------------------*/

	//CString szQuery;
	// 2008-01-24 by cmkwon, S_A: 날짜 관련 로컬리 제이션 처리 버그 수정 - GetDateTimeString() 를 GetSQLDateTimeString() 로 변경 함
	// 2008-02-01 by cmkwon, ItemEvent 에 LastGameEndDate 체크 루틴 추가 - 
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery.Format("UPDATE ti_ItemEvent SET ItemEventType = %d, OnlyPCBang = %d, InfluenceType = %d, ItemNum = %d, Count = %d, StartTime = '%s', EndTime = '%s', MemberShip = %d, ReqLevel = %d, NewMember = %d, UnitKind = %d, PrefixCodeNum = %d, SuffixCodeNum = %d, UseLastGameEndDate=%d, LastGameEndDate= '%s' WHERE ItemEventUID = %d"
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0268
//		, dlg.m_ItemEv.ItemEventType, dlg.m_ItemEv.OnlyPCBang, dlg.m_ItemEv.byInfluenceType
//		, dlg.m_ItemEv.ItemNum, dlg.m_ItemEv.Count
//		, dlg.m_ItemEv.StartTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), dlg.m_ItemEv.EndTime.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING))
//		, dlg.m_ItemEv.MemberShip, dlg.m_ItemEv.ReqLevel, dlg.m_ItemEv.NewMember, dlg.m_ItemEv.UnitKind
//		, dlg.m_ItemEv.PrefixCodeNum, dlg.m_ItemEv.SuffixCodeNum, dlg.m_ItemEv.UseLastGameEndDate
//		, dlg.m_ItemEv.atLastGameEndDate.GetSQLDateTimeString(STRNBUF(SIZE_MAX_SQL_DATETIME_STRING)), pItemEv->ItemEventUID);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.ItemEventType, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.OnlyPCBang, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &dlg.m_ItemEv.byInfluenceType, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.ItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.Count, 0, NULL);
	char szATStart[SIZE_MAX_SQL_DATETIME_STRING];
	dlg.m_ItemEv.StartTime.GetSQLDateTimeString(szATStart, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATStart, 0, NULL);
	char szATEnd[SIZE_MAX_SQL_DATETIME_STRING];
	dlg.m_ItemEv.EndTime.GetSQLDateTimeString(szATEnd, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szATEnd, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &dlg.m_ItemEv.MemberShip, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &dlg.m_ItemEv.ReqLevel, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &dlg.m_ItemEv.NewMember, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.UnitKind, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.PrefixCodeNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.SuffixCodeNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.UseLastGameEndDate, 0, NULL);
	char LastGameEndDate[SIZE_MAX_SQL_DATETIME_STRING];
	dlg.m_ItemEv.atLastGameEndDate.GetSQLDateTimeString(LastGameEndDate, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, LastGameEndDate, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pItemEv->ItemEventUID, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.CheckWithCharacterUID, 0, NULL);		// 2009-11-19 by cmkwon, 아이템 이벤트에 Account/Character 체크 기능 추가 - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.iLevelMin, 0, NULL);		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - 
	SQLBindParameter(m_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.iLevelMax, 0, NULL);		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - 
    SQLBindParameter(m_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.LoginCheckNumber, 0, NULL);		// 2010-06-16 by jskim, 레벨별 이벤트 체크 기능 - 

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
	SQLBindParameter(m_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.UseFixedPeriod, 0, NULL);	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &dlg.m_ItemEv.FixedPeriod_DAY, 0, NULL);	
#endif 

	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0268));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// cleanup

		AfxMessageBox("ItemEvent update error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup

	// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	dlg.m_ItemEv.ItemEventUID = pItemEv->ItemEventUID;
	if ( FALSE == InsetSubTypeInfo(dlg.m_ItemEv) )
	{
		AfxMessageBox("ItemEvent update error _ SubTypeInfo !!");
		return;
	}
#endif
	// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
	this->GetItemEventInfo();
}

void CSCItemEventDlg::OnItemEventAdminToolButtonOk() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CSCItemEventDlg::OnReloadItemEventButton() 
{
	// TODO: Add your control notification handler code here
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 아래와 같이 함수로 처리함
	//m_pMainDlg->m_pAdminPreSocket->WriteMessageType(T_PA_ADMIN_RELOAD_ITEMEV);	
	if(FALSE == m_pMainDlg->SendMsgTypeToPreServer(T_PA_ADMIN_RELOAD_ITEMEV))
	{
		MessageBox("disconnected from PreServer !!");
		return;
	}
}

void CSCItemEventDlg::OnLuckyMachineInitButton()
{
	CString str;
	str.Format("FieldServer must be shutdown if you want to initialize LuckyMachine.");
	int nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{
		return;
	}
	
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	int ServerGroupID = m_comboServerGroupID.GetCurSel();
	if(-1 == ServerGroupID)
	{
		MessageBox("ServerGroupID Error!!");
		return;
	}

	if(0 == ServerGroupID)
	{
		/************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_Update_LuckyItem_DropCount_AllReset_ByAdminTool
		-- DESC				: 럭키머신 드랍아이템 전부 초기화
		-- 2008-12-01 by dhjin,
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_Update_LuckyItem_DropCount_AllReset_ByAdminTool				
		AS
			UPDATE atum2_db_account.dbo.td_MysteryItemDropCount SET DropCount = 0
		GO
		************************************************************************/
		RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_081201_0278, SQL_NTS);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
		{
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
	
	}
	else
	{
		ServerGroupID = (SERVERGROUP_START_ID - 1) + ServerGroupID;
		/************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_Update_LuckyItem_DropCount_ServerGroupIDReset_ByAdminTool
		-- DESC				: 럭키머신 드랍아이템 ServerID로 초기화
		-- 2008-12-01 by dhjin,
		--------------------------------------------------------------------------------
		CREATE PROCEDURE dbo.atum_Update_LuckyItem_DropCount_ServerGroupIDReset_ByAdminTool	
			@i_ServerGroupID	INT
		AS
			UPDATE atum2_db_account.dbo.td_MysteryItemDropCount SET DropCount = 0
			WHERE ServerGroupID = @i_ServerGroupID
		GO
		************************************************************************/
		SQLINTEGER	arrCB[2] = {SQL_NTS, SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ServerGroupID, 0,	&arrCB[1]);
		RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_081201_0279, SQL_NTS);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
		{
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);	
}

// 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
BOOL CSCItemEventDlg::InsetSubTypeInfo(SITEM_EVENT_INFO i_SITEM_EVENT_INFO)
{
//	return TRUE;
#ifdef S_ITEM_EVENT_SUB_TYPE_JHSEOL
	m_pODBCStmt->FreeStatement();
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_SITEM_EVENT_INFO.ItemEventUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &i_SITEM_EVENT_INFO.SubEventType, 0, NULL);
	for (int i = 0 ; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM_EVENT ; i++ )
	{
		SQLBindParameter(hstmt, 3+(2*i), SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_SITEM_EVENT_INFO.ArrDestParameter[i], 0, NULL);
		SQLBindParameter(hstmt, 4+(2*i), SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_SITEM_EVENT_INFO.ArrParameterValue[i], 0, NULL);
	}
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_130329_0004, SQL_NTS);
	if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret && SQL_NO_DATA != ret)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
#endif
	return TRUE;
}
// end 2013-03-29 by jhseol, 아이템 이벤트 - 서브타입 추가구조로 변경
