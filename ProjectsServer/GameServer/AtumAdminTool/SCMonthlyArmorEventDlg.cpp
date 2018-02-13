// SCMonthlyArmorEventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "SCMonthlyArmorEventDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SCMonthlyArmorEventDlg dialog


SCMonthlyArmorEventDlg::SCMonthlyArmorEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SCMonthlyArmorEventDlg::IDD, pParent)
{
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();

	//{{AFX_DATA_INIT(SCMonthlyArmorEventDlg)
	m_timeEndTime.SetTime(23,59,59);
	m_timeEndDate  = COleDateTime::GetCurrentTime();
	m_timeStartDate = COleDateTime::GetCurrentTime();
	m_timeStartTime.SetTime(0,0,0);
	//}}AFX_DATA_INIT
}

SCMonthlyArmorEventDlg::~SCMonthlyArmorEventDlg()
{
	//
}


void SCMonthlyArmorEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SCMonthlyArmorEventDlg)
	DDX_Control(pDX, IDC_EDIT_INPUT_ARMOR, m_EditSearchArmor);
	DDX_Control(pDX, IDC_EDIT_EVENT_INDEX, m_EditEventIndex);
	DDX_Control(pDX, IDC_COMBO_INDEX_TYPE, m_comboIndexType);
	DDX_Control(pDX, IDC_COMBO_OPTION_LIST, m_comboOptionList);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_END_TIME, m_timeEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_END_DATE, m_timeEndDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_START_DATE, m_timeStartDate);
	DDX_Control(pDX, IDC_CUSTOM_MONTHLY_ARMOR_EVENT_LIST, m_GridList1);	
	DDX_Control(pDX, IDC_CUSTOM_MONTHLY_SHAPE_ITEM_LIST, m_GridList2);	
	DDX_DateTimeCtrl(pDX, IDC_DATETIME_START_TIME, m_timeStartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SCMonthlyArmorEventDlg, CDialog)
	//{{AFX_MSG_MAP(SCMonthlyArmorEventDlg)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MONTHLY_SEARCH, OnButtonMonthlySearch)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_RELOAD_MONTHLY_EVENT, OnBtnReloadMonthlyEvent)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MONTHLY_ARMOR_EVENT_LIST, OnGrid1Click)
	ON_NOTIFY(NM_CLICK, IDC_CUSTOM_MONTHLY_SHAPE_ITEM_LIST, OnGrid2Click)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SCMonthlyArmorEventDlg message handlers

void SCMonthlyArmorEventDlg::OnButtonDown() // Insert !!
{	
	// TODO: Add your control notification handler code here
	char str[10];
	m_EditEventIndex.GetWindowText(str,10);
	int tmpEventIndex = atoi(str);

	int tmpRow = 0;
	tmpRow = m_comboOptionList.GetCurSel() + 1;


	vectMONTHLY_OPTION_INFO::iterator itr(m_vectSortedMonthlyOptionList.begin());
	for(; itr != m_vectSortedMonthlyOptionList.end(); itr++)
	{
		MONTHLY_OPTION_INFO *pOptionInfo = &*itr;
		if( tmpRow == pOptionInfo->TempRows)
		{
			m_SelectedOption = *pOptionInfo;
		}
	}

	if( 0 == m_SelectedOption.TempRows )
	{
		MessageBox(" Option is not selected !",NULL,MB_OK);
		return;
	}
	if( 0 == m_SelectedArmor.TempRows )
	{
		MessageBox("  The armor is not selected !",NULL,MB_OK);
		return;
	}

	UpdateData(TRUE);

	MONTHLY_ARMOR_EVNET_INFO NewEvent;
	NewEvent.nEventUID = tmpEventIndex;
	//NewEvent.atStartDate.SetDateTime(m_timeStartDate.GetYear(),m_timeStartDate.GetMonth(),m_timeStartDate.GetDay(),m_timeStartTime.GetHour(),m_timeStartTime.GetMinute(),m_timeStartTime.GetSecond());
	//NewEvent.atEndDate.SetDateTime(m_timeEndDate.GetYear(),m_timeEndDate.GetMonth(),m_timeEndDate.GetDay(),m_timeEndTime.GetHour(),m_timeEndTime.GetMinute(),m_timeEndTime.GetSecond());
	
	// 시간단위만 설정하게 하고 시작시간은 시:00:00, 종료시간은 시:59:59 로 항시 설정
	NewEvent.atStartDate.SetDateTime(m_timeStartDate.GetYear(),m_timeStartDate.GetMonth(),m_timeStartDate.GetDay(),m_timeStartTime.GetHour(),0,0);
	NewEvent.atEndDate.SetDateTime(m_timeEndDate.GetYear(),m_timeEndDate.GetMonth(),m_timeEndDate.GetDay(),m_timeEndTime.GetHour(),59,59);

	NewEvent.nArmorItemNum = m_SelectedArmor.ItemNum;
	NewEvent.nArmorSourceIndex = m_SelectedArmor.LinkItem;
	NewEvent.nOptionItemNum = m_SelectedOption.ItemNum;

	if( IsExsistSameKit(&NewEvent) ) 
	{
		MessageBox("The armor is already registered for the event. !",NULL,MB_OK);
	}
	else
	{
	this->InsertDBMonthlyEvent(&NewEvent);
	}
	this->Refresh();
}

BOOL SCMonthlyArmorEventDlg::IsExsistSameKit(MONTHLY_ARMOR_EVNET_INFO * i_pNewEvent)
{
	vectMONTHLY_ARMOR_EVENT_LIST::iterator itr(m_vectCurMonthlyEventInfo.begin());
	for(; itr != m_vectCurMonthlyEventInfo.end(); itr++)
	{
		MONTHLY_ARMOR_EVENT *pEventInfo = &*itr;
		if( pEventInfo->stMonthlyEventInfo.nArmorItemNum == i_pNewEvent->nArmorItemNum  
			&& pEventInfo->stMonthlyEventInfo.nEventUID != i_pNewEvent->nEventUID)
		{
			return TRUE;
		}
	}
	return FALSE;
}
void SCMonthlyArmorEventDlg::Refresh()
{
	m_timeStartDate = COleDateTime::GetCurrentTime();
	m_timeStartTime.SetTime(0,0,0);
	m_timeEndDate = COleDateTime::GetCurrentTime();
	m_timeEndTime.SetTime(23,59,59);

	UpdateData(FALSE);

	// 이벤트 인덱스 초기화 
	m_EditEventIndex.SetWindowText(NULL);		
	m_comboOptionList.ResetContent();

	vectMONTHLY_OPTION_INFO::iterator itr(m_vectEditMonthlyOptionList.begin());
	for(; itr != m_vectEditMonthlyOptionList.end(); itr++)
	{
		m_comboOptionList.AddString(itr->ItemName);
	}
	
	m_comboIndexType.SetCurSel(0);
	util::zero(&m_SelectedOption,sizeof(MONTHLY_OPTION_INFO));	

	m_EditSearchArmor.SetWindowText("");

	m_vectCurMonthlyEventInfo.clear();
	this->DBQueryGetMonthlyEventInfo(&m_vectCurMonthlyEventInfo);
	_InitGrid_EventInfo(&m_GridList1);
	ViewGridEventInfo(&m_GridList1);

	m_vectCurMonthlyArmorInfo.clear();
	m_vectCurMonthlyArmorInfo.assign(m_vectCurMonthlyArmorInfoBase.begin(), m_vectCurMonthlyArmorInfoBase.end());

	_InitGrid_ArmorInfo(&m_GridList2);
	ViewGridArmorInfo(&m_GridList2);
	
	(CComboBox*)GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	(CComboBox*)GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	(CComboBox*)GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);	
	
	util::zero(&m_SelectedArmor, sizeof(MONTHLY_ARMOR_INFO));
	util::zero(&m_SelectedEvent, sizeof(MONTHLY_ARMOR_EVENT));

	// 2013-06-18 by bckim, 이달의 아머 운영툴 보완작업
	(CComboBox*)GetDlgItem(IDC_COMBO_OPTION_LIST)->EnableWindow(FALSE);

	m_GridList1.EnableWindow(TRUE);
}

BOOL SCMonthlyArmorEventDlg::InsertDBMonthlyEvent(MONTHLY_ARMOR_EVNET_INFO* tmpInsertEvent )
{

	char szEventStartDate[SIZE_MAX_SQL_DATETIME_STRING];
	char szEventEndDate[SIZE_MAX_SQL_DATETIME_STRING];

	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	SQLINTEGER arrCB2[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(tmpInsertEvent->nEventUID),				0,&arrCB2[1]);
	tmpInsertEvent->atStartDate.GetSQLDateTimeString(szEventStartDate,SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szEventStartDate, 0,&arrCB2[2]);
	tmpInsertEvent->atEndDate.GetSQLDateTimeString(szEventEndDate,SIZE_MAX_SQL_DATETIME_STRING);
 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szEventEndDate,	0,&arrCB2[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(tmpInsertEvent->nArmorItemNum),			0,&arrCB2[4]);
 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(tmpInsertEvent->nArmorSourceIndex),		0,&arrCB2[5]);
 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(tmpInsertEvent->nOptionItemNum),			0,&arrCB2[6]);

	BOOL bRet = m_odbcStmt2.ExecuteQuery((char*)(PROCEDURE_130503_0001));
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup
		return FALSE;
	}

	int nMonthlyEventUID = 0;
	SQLINTEGER arrCB[2]		= {SQL_NTS,SQL_NTS};
	SQLBindCol(m_odbcStmt2.m_hstmt, 1, SQL_C_ULONG, &nMonthlyEventUID, 0,&arrCB[1]);

	bRet = SQLFetch(m_odbcStmt2.m_hstmt);

// 	while ( (bRet = SQLFetch(m_odbcStmt2.m_hstmt)) != SQL_NO_DATA)
// 	{
// 
// 	};
// 
// 	if(nSRet != SQL_SUCCESS && nSRet != SQL_SUCCESS_WITH_INFO)
// 	{
// 		break;
// 	}

	m_odbcStmt2.FreeStatement();	// cleanup
	
	if(0 == nMonthlyEventUID)
	{
		return FALSE;
	}
	
	return TRUE;
}

void SCMonthlyArmorEventDlg::OnButtonUp() // Update !!
{
	// TODO: Add your control notification handler code here
	ATUM_DATE_TIME tmpStartDateTime = m_SelectedEvent.stMonthlyEventInfo.atStartDate;
	ATUM_DATE_TIME tmpEndDateTime = m_SelectedEvent.stMonthlyEventInfo.atEndDate;

	m_timeStartDate.SetDate(tmpStartDateTime.Year,tmpStartDateTime.Month,tmpStartDateTime.Day);
	m_timeStartTime.SetTime( tmpStartDateTime.Hour,tmpStartDateTime.Minute,tmpStartDateTime.Second);
	m_timeEndDate.SetDate( tmpEndDateTime.Year,tmpEndDateTime.Month,tmpEndDateTime.Day);
	m_timeEndTime.SetTime( tmpEndDateTime.Hour,tmpEndDateTime.Minute,tmpEndDateTime.Second);
	
	UpdateData(FALSE);

	m_GridList1.EnableWindow(FALSE);

	char str[10];
	itoa(m_SelectedEvent.stMonthlyEventInfo.nEventUID,str,10);
	m_EditEventIndex.SetWindowText(str);

	int tmpOptionRow = GetOptionRowByItemNum(m_SelectedEvent.stMonthlyEventInfo.nOptionItemNum);
	m_comboOptionList.SetCurSel( tmpOptionRow -1 );
	
	// 아머 정보 리스트 
	int tmpArmorRow = GetArmorRowByItemNum(m_SelectedEvent.stMonthlyEventInfo.nArmorItemNum);
	CCellID selected(tmpArmorRow,0);
	m_GridList2.EnsureVisible(selected);
	m_GridList2.SelectRows(selected);
	
	(CComboBox*)GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	(CComboBox*)GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	(CComboBox*)GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(TRUE);
	
	UpdateData();
}

INT	SCMonthlyArmorEventDlg::GetArmorRowByItemNum(INT ArmorItemNum)
{
	vectMONTHLY_ARMOR_INFO::iterator itr(m_vectCurMonthlyArmorInfo.begin());
	for(; itr != m_vectCurMonthlyArmorInfo.end(); itr++)
	{	
		if( itr->ItemNum == ArmorItemNum )		
		{
			m_SelectedArmor = *itr;
			return m_SelectedArmor.TempRows;			
		}
	}
	return 0;
}

INT	SCMonthlyArmorEventDlg::GetOptionRowByItemNum(INT OptionItemNum)
{
	vectMONTHLY_OPTION_INFO::iterator itr(m_vectEditMonthlyOptionList.begin());
	for(; itr != m_vectEditMonthlyOptionList.end(); itr++)
	{	
		if( itr->ItemNum == OptionItemNum )		
		{
			return itr->TempRows;
		}
	}
	return 0;
}

void SCMonthlyArmorEventDlg::OnButtonDelete()  // Delete !!
{
	// TODO: Add your control notification handler code here
	char szTemp[1024];
	sprintf(szTemp, " [ EventNum : %d ] : Are you sure you want to delete it ? ", m_SelectedEvent.stMonthlyEventInfo.nEventUID);

	if( IDOK == MessageBox(szTemp,NULL,MB_OKCANCEL) )
	{
		this->DeleteDBMonthlyEvent(m_SelectedEvent.stMonthlyEventInfo.nEventUID);
	}
	this->Refresh();
}

BOOL SCMonthlyArmorEventDlg::DeleteDBMonthlyEvent(INT EventUID)
{
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &EventUID,				0,&arrCB2[1]);
	
	BOOL bRet = m_odbcStmt2.ExecuteQuery((char*)(PROCEDURE_130503_0003));
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup
		return FALSE;
	}
	m_odbcStmt2.FreeStatement();	// cleanup
	return TRUE;
}


void SCMonthlyArmorEventDlg::OnButtonMonthlySearch() // Search!!
{
	// TODO: Add your control notification handler code here

	CString csInItemKindName;	
	m_comboIndexType.GetWindowText(csInItemKindName);

	if( 0 == strcmp(csInItemKindName, COMBO_LIST1) )	// name Search
	{
		CString csInputItemName;
		m_EditSearchArmor.GetWindowText(csInputItemName);

		m_vectCurMonthlyArmorInfo.clear();
		vectMONTHLY_ARMOR_INFO::iterator itr(m_vectCurMonthlyArmorInfoBase.begin());
		for(; itr != m_vectCurMonthlyArmorInfoBase.end(); itr++ )
		{
			MONTHLY_ARMOR_INFO tmpArmorInfo = *itr;
			if( NULL != strstr( tmpArmorInfo.ItemName, csInputItemName) )
			{	
				m_vectCurMonthlyArmorInfo.push_back(tmpArmorInfo);
			}
		}

		_InitGrid_ArmorInfo(&m_GridList2);
		ViewGridArmorInfo(&m_GridList2);

		return;
	}	
	else if( 0 == strcmp(csInItemKindName, COMBO_LIST2) ) // item index Search 
	{
		CString csInputItemIdx;
		m_EditSearchArmor.GetWindowText(csInputItemIdx);
		
		m_vectCurMonthlyArmorInfo.clear();
		vectMONTHLY_ARMOR_INFO::iterator itr(m_vectCurMonthlyArmorInfoBase.begin());
		for(; itr != m_vectCurMonthlyArmorInfoBase.end(); itr++ )
		{
			MONTHLY_ARMOR_INFO tmpArmorInfo = *itr;
			char csTmpArmorIndex[10];
			itoa( tmpArmorInfo.ItemNum,csTmpArmorIndex,10);
			if( NULL != strstr( csTmpArmorIndex, csInputItemIdx) )
			{	
				m_vectCurMonthlyArmorInfo.push_back(tmpArmorInfo);
			}
		}

		_InitGrid_ArmorInfo(&m_GridList2);
		ViewGridArmorInfo(&m_GridList2);
		return;
	}	
}


BOOL SCMonthlyArmorEventDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if ( FALSE == m_odbcStmt2.Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd()) )
	{
		char szTemp[1024];
		sprintf(szTemp, "Can not connect DBServer<%s(%s:%d)> !!"
			, m_pMainDlg->m_pServerInfo4Admin->DBName, m_pMainDlg->m_pServerInfo4Admin->DBIP
			, m_pMainDlg->m_pServerInfo4Admin->DBPort);
		MessageBox(szTemp);
		EndDialog(-1);
		return FALSE;
	}

	m_vectCurMonthlyEventInfo.clear();
	m_vectCurMonthlyArmorInfo.clear();
	m_vectEditMonthlyOptionList.clear();

	this->DBQueryGetMonthlyEventInfo(&m_vectCurMonthlyEventInfo);
	this->DBQueryGetMonthlyArmorInfo(&m_vectCurMonthlyArmorInfoBase);
	this->DBQueryGetMonthlyOptionInfo(&m_vectEditMonthlyOptionList);

	// 2013-06-18 by bckim, 이달의 아머 운영툴 보완작업
	Refresh_Option();

	m_comboIndexType.AddString(COMBO_LIST1);	// name : 0 
	m_comboIndexType.AddString(COMBO_LIST2);	// item index : 1
	m_comboIndexType.SetCurSel(0);				// name

	_InitGrid_EventInfo(&m_GridList1);
	ViewGridEventInfo(&m_GridList1);

	_InitGrid_ArmorInfo(&m_GridList2);
	ViewGridArmorInfo(&m_GridList2);
	
	
	(CComboBox*)GetDlgItem(IDC_BUTTON_UP)->EnableWindow(FALSE);
	(CComboBox*)GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
	
	// 2013-06-18 by bckim, 이달의 아머 운영툴 보완작업
	(CComboBox*)GetDlgItem(IDC_COMBO_OPTION_LIST)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SCMonthlyArmorEventDlg::Refresh_Option()
{

	if( 0 == m_SelectedArmor.ItemNum )
	{
		return;
	}

	m_comboOptionList.ResetContent();
	int tempRow = 0 ;

	vectMONTHLY_OPTION_INFO::iterator itr(m_vectEditMonthlyOptionList.begin());
	for(; itr != m_vectEditMonthlyOptionList.end(); itr++)
	{
		if( IS_SAME_UNITKIND(m_SelectedArmor.ReqUnitKind,itr->ReqUnitKind) )
		{
			char szTemp[1024];
			sprintf(szTemp, "%s : [%d]",itr->ItemName,itr->ItemNum);
			m_comboOptionList.AddString(szTemp);
			
			tempRow++;
			itr->TempRows = tempRow;
			m_vectSortedMonthlyOptionList.push_back(*itr);
		}
	}
}

void SCMonthlyArmorEventDlg::_InitGrid_ArmorInfo(CGridCtrl *i_pGridCtrl)
{
	i_pGridCtrl->SetBkColor(0xFFFFFF);
	
	int nRows = 1;
	int nCols = 3;
	
	i_pGridCtrl->SetEditable(FALSE);		 
	i_pGridCtrl->SetListMode(TRUE);
	i_pGridCtrl->SetSingleRowSelection(TRUE);
	i_pGridCtrl->EnableSelection(TRUE);
	i_pGridCtrl->SetFrameFocusCell(TRUE);
	i_pGridCtrl->SetTrackFocusCell(TRUE);
	
	i_pGridCtrl->SetRowCount(nRows);
	i_pGridCtrl->SetColumnCount(nCols);
	i_pGridCtrl->SetFixedRowCount(1);

	// 칼럼 만들기
	nCols			= 0;
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= 0;
	Item.nFormat	= GRID_CELL_FORMAT;

	Item.col = GRID2_COL_ARMOR_INFO_SKILLLEVEL;
	Item.strText.Format("Skill Level");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID2_COL_ARMOR_INFO_ITEM_INDEX;
	Item.strText.Format("Item Index");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID2_COL_ARMOR_INFO_ARMOR_NAME;
	Item.strText.Format("Armor Name");
	i_pGridCtrl->SetItem(&Item);
	
	// arrange grid
	//i_pGridCtrl->AutoSize();
	i_pGridCtrl->ExpandColumnsToFit();
	
	// clean all cells
	CCellRange tmpCellRange(1, 0, i_pGridCtrl->GetRowCount()-1, i_pGridCtrl->GetColumnCount()-1);
	i_pGridCtrl->ClearCells(tmpCellRange);
}

void SCMonthlyArmorEventDlg::_InitGrid_EventInfo(CGridCtrl *i_pGridCtrl)
{
	i_pGridCtrl->SetBkColor(0xFFFFFF);
	
	int nRows = 1;
	int nCols = 5;
	
	i_pGridCtrl->SetEditable(FALSE);		 
	i_pGridCtrl->SetListMode(TRUE);
	i_pGridCtrl->SetSingleRowSelection(TRUE);
	i_pGridCtrl->EnableSelection(TRUE);
	i_pGridCtrl->SetFrameFocusCell(TRUE);
	i_pGridCtrl->SetTrackFocusCell(TRUE);
	
	i_pGridCtrl->SetRowCount(nRows);
	i_pGridCtrl->SetColumnCount(nCols);
	i_pGridCtrl->SetFixedRowCount(1);
	
	// 칼럼 만들기
	nCols			= 0;
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= 0;
	Item.nFormat	= GRID_CELL_FORMAT;
	
	Item.col = GRID1_COL_EVENT_INFO_EVENT_NUM;
	Item.strText.Format("Event Num");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID1_COL_EVENT_INFO_EVENT_START_DATE;
	Item.strText.Format("Start Date/Time");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID1_COL_EVENT_INFO_EVENT_END_DATE;
	Item.strText.Format("End Date/Time");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID1_COL_EVENT_INFO_ARMOR_NAME;
	Item.strText.Format("Armor Name");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col = GRID1_COL_EVENT_INFO_OPTION;
	Item.strText.Format("Option");
	i_pGridCtrl->SetItem(&Item);
	
	// arrange grid
	//i_pGridCtrl->AutoSize();
	i_pGridCtrl->ExpandColumnsToFit();
	
	// clean all cells
	CCellRange tmpCellRange(1, 0, i_pGridCtrl->GetRowCount()-1, i_pGridCtrl->GetColumnCount()-1);
	i_pGridCtrl->ClearCells(tmpCellRange);
}

INT SCMonthlyArmorEventDlg::_AddEventInfo(CGridCtrl *i_pGridCtrl, MONTHLY_ARMOR_EVENT *i_pEvenrInfo)
{
	GV_ITEM Item;
	
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	
	int nNewRowIdx = i_pGridCtrl->GetRowCount();
	i_pGridCtrl->SetRowCount(nNewRowIdx+1);
	
	//select된 값을 GridDetail GridCtrl에 넣어준다.
	Item.row		= nNewRowIdx;
	Item.col		= 0;
	Item.nFormat	= DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%d", i_pEvenrInfo->stMonthlyEventInfo.nEventUID);
	Item.strText = Item.strText.Right(9);
	i_pGridCtrl->SetColumnWidth(Item.col,70);	
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID1_COL_EVENT_INFO_EVENT_START_DATE;
	Item.strText.Format("%s", i_pEvenrInfo->stMonthlyEventInfo.atStartDate.GetDateTimeString().GetBuffer());
	Item.strText = Item.strText.Right(20);
	i_pGridCtrl->SetColumnWidth(Item.col,130);	
	i_pGridCtrl->SetItem(&Item);

	Item.col		= GRID1_COL_EVENT_INFO_EVENT_END_DATE;
	Item.strText.Format("%s", i_pEvenrInfo->stMonthlyEventInfo.atEndDate.GetDateTimeString().GetBuffer());
	Item.strText = Item.strText.Right(20);
	i_pGridCtrl->SetColumnWidth(Item.col,130);	
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID1_COL_EVENT_INFO_ARMOR_NAME;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%s : [%d]", i_pEvenrInfo->ArmorName,i_pEvenrInfo->stMonthlyEventInfo.nArmorItemNum);
	i_pGridCtrl->SetColumnWidth(Item.col,350);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID1_COL_EVENT_INFO_OPTION;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%s : [%d]", i_pEvenrInfo->ArmorOptionName,i_pEvenrInfo->stMonthlyEventInfo.nOptionItemNum);
	i_pGridCtrl->SetColumnWidth(Item.col,300);
	i_pGridCtrl->SetItem(&Item);
	
	return nNewRowIdx;
}

INT SCMonthlyArmorEventDlg::_AddArmorInfo(CGridCtrl *i_pGridCtrl, MONTHLY_ARMOR_INFO *i_pArmorInfo)
{
	GV_ITEM Item;
	
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;
	
	int nNewRowIdx = i_pGridCtrl->GetRowCount();
	i_pGridCtrl->SetRowCount(nNewRowIdx+1);
	
	//select된 값을 GridDetail GridCtrl에 넣어준다.
	Item.row		= nNewRowIdx;
	Item.col		= 0;
	Item.nFormat	= DT_CENTER|DT_VCENTER|DT_SINGLELINE;
	Item.strText.Format("%d", i_pArmorInfo->SkillLevel);
	i_pGridCtrl->SetColumnWidth(Item.col,70);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID2_COL_ARMOR_INFO_ITEM_INDEX;
	Item.strText.Format("%d", i_pArmorInfo->ItemNum);
	i_pGridCtrl->SetColumnWidth(Item.col,100);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= GRID2_COL_ARMOR_INFO_ARMOR_NAME;
	Item.strText.Format("%s ", i_pArmorInfo->ItemName);
	i_pGridCtrl->SetColumnWidth(Item.col,320);
	i_pGridCtrl->SetItem(&Item);

	return nNewRowIdx;
}


void SCMonthlyArmorEventDlg::ViewGridEventInfo(CGridCtrl *i_pGridCtrl)
{
	vectMONTHLY_ARMOR_EVENT_LIST::iterator itr(m_vectCurMonthlyEventInfo.begin());
	for(; itr != m_vectCurMonthlyEventInfo.end(); itr++)
	{
		MONTHLY_ARMOR_EVENT *pEventInfo = &*itr;
		itr->TempRows = _AddEventInfo(i_pGridCtrl, pEventInfo);
	}
	i_pGridCtrl->UpdateData();
//	i_pGridCtrl->AutoSize();
}

void SCMonthlyArmorEventDlg::ViewGridArmorInfo(CGridCtrl *i_pGridCtrl)
{
	vectMONTHLY_ARMOR_INFO::iterator itr(m_vectCurMonthlyArmorInfo.begin());
	for(; itr != m_vectCurMonthlyArmorInfo.end(); itr++)
	{
		MONTHLY_ARMOR_INFO *pArmorInfo = &*itr;
		itr->TempRows = _AddArmorInfo(i_pGridCtrl, pArmorInfo);
	}
	i_pGridCtrl->UpdateData();
//	i_pGridCtrl->AutoSize();
}

BOOL SCMonthlyArmorEventDlg::DBQueryGetMonthlyEventInfo(vectMONTHLY_ARMOR_EVENT_LIST *o_pVectEventList)
{
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();
	BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_130503_0002_1);	
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryGetMonthlyEventInfo error !!");
		return FALSE;
	}	

	SQLINTEGER arrCB[9]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	MONTHLY_ARMOR_EVENT tmEventInfo;
	
	SQL_TIMESTAMP_STRUCT tmpStartTime;
	SQL_TIMESTAMP_STRUCT tmpEndTime;
	util::zero(&tmpStartTime, sizeof(SQL_TIMESTAMP_STRUCT));
	util::zero(&tmpEndTime, sizeof(SQL_TIMESTAMP_STRUCT));
	
	SQLBindCol(hstmt,  1, SQL_C_LONG,		&tmEventInfo.stMonthlyEventInfo.nEventUID,			0,&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_TIMESTAMP,	&tmpStartTime,										0,&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_TIMESTAMP,	&tmpEndTime,										0,&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_C_LONG,		&tmEventInfo.stMonthlyEventInfo.nArmorItemNum,		0,&arrCB[4]);
	SQLBindCol(hstmt,  5, SQL_C_LONG,		&tmEventInfo.stMonthlyEventInfo.nArmorSourceIndex,	0,&arrCB[5]);
	SQLBindCol(hstmt,  6, SQL_C_LONG,		&tmEventInfo.stMonthlyEventInfo.nOptionItemNum,		0,&arrCB[6]);

	SQLBindCol(hstmt,  7, SQL_CHAR,			&tmEventInfo.ArmorName,				SIZE_MAX_ITEM_NAME,&arrCB[7]);
	SQLBindCol(hstmt,  8, SQL_CHAR,			&tmEventInfo.ArmorOptionName,		SIZE_MAX_ITEM_NAME,&arrCB[8]);

	int nTempRows =0;
	do
	{
		util::zero(&tmEventInfo, sizeof(tmEventInfo));
		
		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_ERROR == ret)
		{
			m_odbcStmt2.ProcessLogMessagesForStmt(PROCEDURE_130503_0001);	
		}
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		tmEventInfo.stMonthlyEventInfo.atStartDate	= tmpStartTime;
		tmEventInfo.stMonthlyEventInfo.atEndDate	= tmpEndTime;
		nTempRows++;
		tmEventInfo.TempRows  = nTempRows;
		
		o_pVectEventList->push_back(tmEventInfo);
	}while(TRUE);
	
	m_odbcStmt2.FreeStatement();		// cleanup
	
	return TRUE;
}

BOOL SCMonthlyArmorEventDlg::DBQueryGetMonthlyArmorInfo(vectMONTHLY_ARMOR_INFO *o_pVectArmorList)
{
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();

	SQLINTEGER cb[3]={SQL_NTS,SQL_NTS,SQL_NTS};
	
	cb[1] = SQL_NTS;
	cb[2] = SQL_NTS;
	int nDesparamNum = DES_SHAPE_ITEM;
	BYTE nItemKind    = ITEMKIND_CARD;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nDesparamNum, 0,&cb[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &nItemKind, 0,&cb[2]);
	
	BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_130503_0004);	
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryGetMonthlyArmorInfo error !!");
		return FALSE;
	}	
	
	SQLINTEGER arrCB[6]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	MONTHLY_ARMOR_INFO tmpArmorInfo;
	
	SQLBindCol(hstmt,  1, SQL_C_TINYINT,	&tmpArmorInfo.SkillLevel,					0,&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_LONG,		&tmpArmorInfo.ItemNum,						0,&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_LONG,		&tmpArmorInfo.LinkItem,						0,&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_CHAR,			&tmpArmorInfo.ItemName,	SIZE_MAX_ITEM_NAME	 ,&arrCB[4]);
		
	SQLBindCol(hstmt,  5, SQL_C_LONG,		&tmpArmorInfo.ReqUnitKind,					0,&arrCB[5]);
		
	int nTempRows =0;
	do
	{
		util::zero(&tmpArmorInfo, sizeof(tmpArmorInfo));
		
		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_ERROR == ret)
		{
			m_odbcStmt2.ProcessLogMessagesForStmt(PROCEDURE_130503_0004);
		}
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		nTempRows++;
		tmpArmorInfo.TempRows = nTempRows;
		o_pVectArmorList->push_back(tmpArmorInfo);

	}while(TRUE);
	
	m_vectCurMonthlyArmorInfo.assign(o_pVectArmorList->begin(), o_pVectArmorList->end());
	
	m_odbcStmt2.FreeStatement();		// cleanup
	
	return TRUE;
}


BOOL SCMonthlyArmorEventDlg::DBQueryGetMonthlyOptionInfo(vectMONTHLY_OPTION_INFO *o_pVectOptionList)
{
	SQLHSTMT hstmt = m_odbcStmt2.GetSTMTHandle();

	SQLINTEGER cb[2]={SQL_NTS,SQL_NTS};
	
	cb[1] = SQL_NTS;

    int nDesparamNum = DES_OPTION_ITEM_DEFAULT_DESPARAM;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nDesparamNum, 0,&cb[1]);
	
	BOOL bRet = m_odbcStmt2.ExecuteQuery(PROCEDURE_130503_0005);	
	if (!bRet)
	{
		m_odbcStmt2.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryGetMonthlyOptionInfo error !!");
		return FALSE;
	}	
	
	SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	MONTHLY_OPTION_INFO tmpOptionInfo;
	
	SQLBindCol(hstmt,  1, SQL_C_LONG,		&tmpOptionInfo.ItemNum,						0,&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_CHAR,			&tmpOptionInfo.ItemName,	SIZE_MAX_ITEM_NAME ,&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_LONG,		&tmpOptionInfo.ReqUnitKind,					0,&arrCB[3]);
	
	int nTempRows = 0;
	do
	{
		util::zero(&tmpOptionInfo, sizeof(tmpOptionInfo));
		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_ERROR == ret)
		{
			m_odbcStmt2.ProcessLogMessagesForStmt(PROCEDURE_130503_0004);	
		}
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		nTempRows++;
		tmpOptionInfo.TempRows = nTempRows;
		o_pVectOptionList->push_back(tmpOptionInfo);
		
	}while(TRUE);
	
	m_odbcStmt2.FreeStatement();		// cleanup
	
	return TRUE;
}

void SCMonthlyArmorEventDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	//OnInitDialog();
	this->Refresh();	
}

void SCMonthlyArmorEventDlg::OnBtnReloadMonthlyEvent() 
{
	// TODO: Add your control notification handler code here

	MessageBox(" Has been applied !!");

	if(FALSE == m_pMainDlg->SendMsgTypeToPreServer(T_PA_ADMIN_RELOAD_ITEMEV))
	{
		MessageBox("disconnected from PreServer !!");
		return;
	}	
}

// 이벤트 리스트 선택
void SCMonthlyArmorEventDlg::OnGrid1Click(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	UpdateData();
    NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	if (pGridView->iRow == 0) { return; }

	vectMONTHLY_ARMOR_EVENT_LIST::iterator itr(m_vectCurMonthlyEventInfo.begin());
	for(; itr != m_vectCurMonthlyEventInfo.end(); itr++)
	{
		MONTHLY_ARMOR_EVENT *pArmorEvent = &*itr;
		if(pGridView->iRow == pArmorEvent->TempRows)
		{
			m_SelectedEvent = *pArmorEvent;
			(CComboBox*)GetDlgItem(IDC_BUTTON_UP)->EnableWindow(TRUE);
			(CComboBox*)GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
			(CComboBox*)GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(FALSE);
			return;
		}
	}
}

// 아머 리스트 선택
void SCMonthlyArmorEventDlg::OnGrid2Click(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	UpdateData();
    NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	if (pGridView->iRow == 0) { return; }

	vectMONTHLY_ARMOR_INFO::iterator itr(m_vectCurMonthlyArmorInfo.begin());
	for(; itr != m_vectCurMonthlyArmorInfo.end(); itr++)
	{
		MONTHLY_ARMOR_INFO *pArmorInfo = &*itr;
		if(pGridView->iRow == pArmorInfo->TempRows)
		{
			m_SelectedArmor = *pArmorInfo;
			
			// 2013-06-18 by bckim, 이달의 아머 운영툴 보완작업
			(CComboBox*)GetDlgItem(IDC_COMBO_OPTION_LIST)->EnableWindow(TRUE);
			Refresh_Option();
			return;
		}
	}
}

BOOL SCMonthlyArmorEventDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnButtonMonthlySearch();
		return TRUE;
    }
    else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		CDialog::OnCancel();
        return TRUE;
    }
    return CDialog::PreTranslateMessage(pMsg);
}