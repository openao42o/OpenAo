// dlgsetinfluencerate.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "dlgsetinfluencerate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetInfluenceRate dialog


CDlgSetInfluenceRate::CDlgSetInfluenceRate(BOOL i_bEnableEdit, CODBCStatement *i_pODBCState, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetInfluenceRate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetInfluenceRate)
	m_ctlbSetForAll = FALSE;
	m_ctlbSetForLevel = FALSE;
	//}}AFX_DATA_INIT

	m_bEnableEdit			= i_bEnableEdit;
	m_pODBCStmt5			= i_pODBCState;
}


void CDlgSetInfluenceRate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetInfluenceRate)
	DDX_Check(pDX, IDC_CHECK_SetForAll, m_ctlbSetForAll);
	DDX_Check(pDX, IDC_CHECK_SetForLevel, m_ctlbSetForLevel);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_SET_FOR_LEVEL, m_GridSetForLevel);
	DDX_GridControl(pDX, IDC_GRID_SET_FOR_ALL, m_GridSetForAll);
}


BEGIN_MESSAGE_MAP(CDlgSetInfluenceRate, CDialog)
	//{{AFX_MSG_MAP(CDlgSetInfluenceRate)
	ON_BN_CLICKED(IDC_CHECK_SetForLevel, OnCHECKSetForLevel)
	ON_BN_CLICKED(IDC_CHECK_SetForAll, OnCHECKSetForAll)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_GRID_SET_FOR_LEVEL, OnGridClickSetForLevel)
	ON_NOTIFY(NM_CLICK, IDC_GRID_SET_FOR_ALL, OnGridClickSetForAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetInfluenceRate message handlers

void CDlgSetInfluenceRate::OnCHECKSetForLevel() 
{
	// TODO: Add your control notification handler code here	
	UpdateData();
	
	m_ctlbSetForAll		= !m_ctlbSetForLevel;
	UpdateData(FALSE);

	m_GridSetForAll.EnableWindow(m_ctlbSetForAll);
	m_GridSetForLevel.EnableWindow(m_ctlbSetForLevel);
	m_GridSetForAll.SetSelectedRange(-1,-1,-1,-1);
	m_GridSetForLevel.SetSelectedRange(-1,-1,-1,-1);
}

void CDlgSetInfluenceRate::OnCHECKSetForAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_ctlbSetForLevel	= !m_ctlbSetForAll;
	UpdateData(FALSE);
	
	m_GridSetForAll.EnableWindow(m_ctlbSetForAll);
	m_GridSetForLevel.EnableWindow(m_ctlbSetForLevel);
	m_GridSetForAll.SetSelectedRange(-1,-1,-1,-1);
	m_GridSetForLevel.SetSelectedRange(-1,-1,-1,-1);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-21 ~ 2009-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDlgSetInfluenceRate::InitGrid(CInfluenceRateGridCtrl *i_pGridCtrl)
{
	i_pGridCtrl->SetBkColor(0xFFFFFF);
	
	int nRows = 1;
	int nCols = 6;
	
	i_pGridCtrl->SetEditable(TRUE);		// 2009-09-21 by cmkwon, 수정 가능
	i_pGridCtrl->SetSingleRowSelection(TRUE);
	i_pGridCtrl->SetSingleColSelection(TRUE);
	i_pGridCtrl->SetFrameFocusCell(FALSE);
	i_pGridCtrl->SetTrackFocusCell(FALSE);
	
	i_pGridCtrl->SetRowCount(nRows);
	i_pGridCtrl->SetColumnCount(nCols);
	i_pGridCtrl->SetFixedRowCount(1);
	
	// 칼럼 만들기
	nCols			= 0;
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= 0;
	Item.nState		= GVIS_READONLY;
	Item.nFormat	= GRID_CELL_FORMAT;
	
	Item.col		= nCols++;
	Item.strText.Format("Level");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= nCols++;
	Item.strText.Format("Proportion(%%)");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= nCols++;
	Item.strText.Format("Update Date");
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= nCols++;
	Item.strText.Format("Normal Users");
	i_pGridCtrl->SetItem(&Item);

	Item.col		= nCols++;
	Item.strText.Format("BCU Users");
	i_pGridCtrl->SetItem(&Item);

	Item.col		= nCols++;
	Item.strText.Format("ANI Users");
	i_pGridCtrl->SetItem(&Item);

	// arrange grid
	i_pGridCtrl->ArrangeGrid();
	
	// clean all cells
	CCellRange tmpCellRange(1, 0, i_pGridCtrl->GetRowCount()-1, i_pGridCtrl->GetColumnCount()-1);
	i_pGridCtrl->ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDlgSetInfluenceRate::AddItemToGrid(CInfluenceRateGridCtrl *i_pGridCtrl, SINFL_USER_COUNT *i_pUserCnt)
{
	i_pGridCtrl->SetRowCount(i_pGridCtrl->GetRowCount()+1);
	
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT|GVIF_STATE|GVIF_PARAM;
	Item.row		= i_pGridCtrl->GetRowCount()-1;
	Item.nState		= GVIS_READONLY;
	Item.nFormat	= GRID_CELL_FORMAT;
	Item.lParam		= (long)i_pUserCnt;		// 데이터 저장
	
	int nColIndex	= 0;
	Item.col		= nColIndex++;
	Item.strText.Format("%d ~ %d", i_pUserCnt->StartLevel, i_pUserCnt->EndLevel);
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= nColIndex++;
	Item.nState		= 0;				// 이 필드만 수정 가능
	Item.strText.Format("%d", i_pUserCnt->AllowablePercent);
	Item.lParam		= 
	i_pGridCtrl->SetItem(&Item);
	
	Item.col		= nColIndex++;
	Item.nState		= GVIS_READONLY;
	Item.strText	= i_pUserCnt->UpdatedTime.GetDateTimeString().GetBuffer();
	i_pGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.nState		= GVIS_READONLY;
	Item.strText.Format("%d", i_pUserCnt->arrUserCount[0]);
	i_pGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.nState		= GVIS_READONLY;
	Item.strText.Format("%d", i_pUserCnt->arrUserCount[1]);
	i_pGridCtrl->SetItem(&Item);

	Item.col		= nColIndex++;
	Item.nState		= GVIS_READONLY;
	Item.strText.Format("%d", i_pUserCnt->arrUserCount[2]);
	i_pGridCtrl->SetItem(&Item);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDlgSetInfluenceRate::ArrangeSetInfluenceRate(void)
{
	BOOL bSetForAll = FALSE;
	int TotalNormals	= 0;
	int TotalBCUs		= 0;
	int TotalANIs		= 0;

	mtvectSINFL_USER_COUNT::iterator itr(m_vectInflUserList.begin());
	for(; itr != m_vectInflUserList.end(); itr++)
	{
		SINFL_USER_COUNT *pUserCnt = &*itr;
		if(pUserCnt->IsSetForAll())
		{
			bSetForAll	= TRUE;
		}
		else
		{
			TotalNormals	+= pUserCnt->arrUserCount[0];
			TotalBCUs		+= pUserCnt->arrUserCount[1];
			TotalANIs		+= pUserCnt->arrUserCount[2];
		}
	}

	m_ctlbSetForAll		= FALSE;
	m_ctlbSetForLevel	= FALSE;
	if(bSetForAll)
	{
		m_ctlbSetForAll = TRUE;
	}
	else
	{
		m_ctlbSetForLevel = TRUE;

		// 2009-09-22 by cmkwon, 전체세력 설정을 위한 임시 리스트를 위해 기본으로 추가한다
		SINFL_USER_COUNT tmUserCnt;
		util::zero(&tmUserCnt, sizeof(tmUserCnt));
		tmUserCnt.StartLevel		= 0;
		tmUserCnt.EndLevel			= 0;
		tmUserCnt.AllowablePercent	= 0;
		tmUserCnt.arrUserCount[0]	= TotalNormals;
		tmUserCnt.arrUserCount[1]	= TotalBCUs;
		tmUserCnt.arrUserCount[2]	= TotalANIs;
		m_vectInflUserList.push_back(tmUserCnt);
	}
	m_GridSetForAll.EnableWindow(m_ctlbSetForAll);
	m_GridSetForLevel.EnableWindow(m_ctlbSetForLevel);
	UpdateData(FALSE);

	itr = m_vectInflUserList.begin();
	for(; itr != m_vectInflUserList.end(); itr++)
	{
		SINFL_USER_COUNT *pUserCnt = &*itr;
		if(pUserCnt->IsSetForAll())
		{
			AddItemToGrid(&m_GridSetForAll, pUserCnt);
		}
		else
		{
			AddItemToGrid(&m_GridSetForLevel, pUserCnt);
		}
	}
	m_GridSetForAll.ArrangeGrid();
	m_GridSetForLevel.ArrangeGrid();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDlgSetInfluenceRate::UpdateDataInfluenceRate(void)
{
	if(m_ctlbSetForAll)
	{
		if(FALSE == this->UpdateDatatRowFromGrid(&m_GridSetForAll, 1, 1))
		{
			AfxMessageBox("CDlgSetInfluenceRate::UpdateDataInfluenceRate error !! 100000");
			return FALSE;
		}
	}
	else
	{
		int nRow = m_GridSetForLevel.GetRowCount();
		for(int i=1; i < nRow; i++)
		{
			if(FALSE == this->UpdateDatatRowFromGrid(&m_GridSetForLevel, i, 1))
			{
				CString csErrTxt;
				csErrTxt.Format("CDlgSetInfluenceRate::UpdateDataInfluenceRate error !! 110000, Row(%d)", i);
				AfxMessageBox(csErrTxt);
				return FALSE;
			}
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDlgSetInfluenceRate::UpdateDatatRowFromGrid(CInfluenceRateGridCtrl *i_pGridCtrl, int i_nRow, int i_nCol)
{
	if(0 >= i_nRow)
	{
		return FALSE;
	}

	SINFL_USER_COUNT *pUserCnt = (SINFL_USER_COUNT*)(i_pGridCtrl->GetItemData(i_nRow, i_nCol));
	if(NULL == pUserCnt)
	{
		return FALSE;
	}

	pUserCnt->AllowablePercent	= 0;

	CString csText = i_pGridCtrl->GetItemText(i_nRow, i_nCol);
 	if(FALSE == csText.IsEmpty())
 	{
 		pUserCnt->AllowablePercent = atoi(csText);
 	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SINFL_USER_COUNT * CDlgSetInfluenceRate::GetInflUserCount(mtvectSINFL_USER_COUNT *i_pUserCountList, BYTE i_byStartLv, BYTE i_byEndLv)
{
	mtvectSINFL_USER_COUNT::iterator itr(i_pUserCountList->begin());
	for(; itr != i_pUserCountList->end(); itr++)
	{
		SINFL_USER_COUNT *pUserCnt = &*itr;
		if(i_byStartLv == pUserCnt->StartLevel && i_byEndLv == pUserCnt->EndLevel)
		{
			return pUserCnt;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-21 ~ 2009-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDlgSetInfluenceRate::DB_LoadInfluenceRateList(mtvectSINFL_USER_COUNT *o_pvectInflUserList)
{
	o_pvectInflUserList->clear();

	SQLHSTMT hstmt = m_pODBCStmt5->GetSTMTHandle();

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Rearrange_InfluenceRate
	-- DESC				: -- // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	--						
	--------------------------------------------------------------------------------
	CALL dbo.atum_Rearrange_InfluenceRate

	************************************************************************/
	SQLINTEGER	arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS};
	int nPeriodDay = (-1)*PERIOD_OF_DAY_FOR_VALID_INFLUENCE;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nPeriodDay, 0,	&arrCB[1]);
	
	BOOL bRet = m_pODBCStmt5->ExecuteQuery(PROCEDURE_090918_0400);
	if(FALSE == bRet)
	{
		m_pODBCStmt5->FreeStatement();
		return FALSE;
	}
	m_pODBCStmt5->FreeStatement();
	
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Load_InfluenceRate
	-- DESC				: -- // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	--						
	--------------------------------------------------------------------------------
	CALL dbo.atum_Load_InfluenceRate

	************************************************************************/
	SINFL_USER_COUNT		tmUserCnt;
	SQL_TIMESTAMP_STRUCT	UpdatedTime;
	
	arrCB[1] = arrCB[2] = arrCB[3] = arrCB[4] = arrCB[5] = arrCB[6] = arrCB[7] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &tmUserCnt.StartLevel, 0,			&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_UTINYINT, &tmUserCnt.EndLevel, 0,			&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_LONG, &tmUserCnt.AllowablePercent, 0,		&arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &UpdatedTime, 0,					&arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_LONG, &tmUserCnt.arrUserCount[0], 0,			&arrCB[5]);
	SQLBindCol(hstmt, 6, SQL_C_LONG, &tmUserCnt.arrUserCount[1], 0,			&arrCB[6]);
	SQLBindCol(hstmt, 7, SQL_C_LONG, &tmUserCnt.arrUserCount[2], 0,			&arrCB[7]);

	bRet = m_pODBCStmt5->ExecuteQuery(PROCEDURE_090918_0401);
	if(FALSE == bRet)
	{
		m_pODBCStmt5->FreeStatement();
		return FALSE;
	}

	util::zero(&tmUserCnt, sizeof(tmUserCnt));
	util::zero(&UpdatedTime, sizeof(UpdatedTime));
	while(TRUE)
	{
		SQLRETURN ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			break;
		}
		tmUserCnt.UpdatedTime	= UpdatedTime;

		o_pvectInflUserList->push_back(tmUserCnt);

		util::zero(&tmUserCnt, sizeof(tmUserCnt));
		util::zero(&UpdatedTime, sizeof(UpdatedTime));
	}
	m_pODBCStmt5->FreeStatement();

	if(o_pvectInflUserList->empty())
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-21 ~ 2009-09-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CDlgSetInfluenceRate::DB_UpdateInfluenceRateList(BOOL i_bSetForAll, mtvectSINFL_USER_COUNT *i_pvectInflUserList)
{
	if(i_pvectInflUserList->empty())
	{
		return FALSE;
	}

	SQLHSTMT hstmt = m_pODBCStmt5->GetSTMTHandle();
	if(i_bSetForAll)
	{
		/************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_Update_AllowablePercent
		-- DESC				: -- // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
		--						
		--------------------------------------------------------------------------------
		CALL dbo.atum_Update_AllowablePercent
		************************************************************************/

		SINFL_USER_COUNT *pUserCnt = this->GetInflUserCount(i_pvectInflUserList, 0, 0);		
		if(NULL == pUserCnt)
		{
			AfxMessageBox("Don't have userList !! 200000");
			return FALSE;
		}

		SQLINTEGER	arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pUserCnt->StartLevel, 0,	&arrCB[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pUserCnt->EndLevel, 0,		&arrCB[2]);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pUserCnt->AllowablePercent, 0,	&arrCB[3]);
		BOOL bRet = m_pODBCStmt5->ExecuteQuery(PROCEDURE_090918_0402);
		if(FALSE == bRet)
		{
			m_pODBCStmt5->FreeStatement();
			return FALSE;
		}
		m_pODBCStmt5->FreeStatement();
	}
	else
	{
		/************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_Delete_InfluenceRate
		-- DESC				: -- // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
		--						
		--------------------------------------------------------------------------------
		CALL dbo.atum_Delete_InfluenceRate
		************************************************************************/
		SQLINTEGER	arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		BYTE byStartLv	= 0;
		BYTE byEndLv	= 0;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &byStartLv, 0,	&arrCB[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &byEndLv, 0,		&arrCB[2]);
		BOOL bRet = m_pODBCStmt5->ExecuteQuery(PROCEDURE_090918_0403);
		if(FALSE == bRet)
		{
			m_pODBCStmt5->FreeStatement();
			return FALSE;
		}
		m_pODBCStmt5->FreeStatement();

		mtvectSINFL_USER_COUNT::iterator itr(i_pvectInflUserList->begin());
		for(; itr != i_pvectInflUserList->end(); itr++)
		{
			SINFL_USER_COUNT *pUserCnt		= &*itr;
			if(pUserCnt->IsSetForAll())
			{
				continue;
			}

			/************************************************************************
			--------------------------------------------------------------------------------
			-- PROCEDURE NAME	: dbo.atum_Update_AllowablePercent
			-- DESC				: -- // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
			--						
			--------------------------------------------------------------------------------
			CALL dbo.atum_Update_AllowablePercent
			************************************************************************/
			arrCB[1] = arrCB[2] = arrCB[3] = SQL_NTS;
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pUserCnt->StartLevel, 0,	&arrCB[1]);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pUserCnt->EndLevel, 0,		&arrCB[2]);
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pUserCnt->AllowablePercent, 0,	&arrCB[3]);
			BOOL bRet = m_pODBCStmt5->ExecuteQuery(PROCEDURE_090918_0402);
			if(FALSE == bRet)
			{
				m_pODBCStmt5->FreeStatement();
				return FALSE;
			}
			m_pODBCStmt5->FreeStatement();
		}
	}

	return TRUE;
}

BOOL CDlgSetInfluenceRate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	DB_LoadInfluenceRateList(&m_vectInflUserList);
	this->InitGrid(&m_GridSetForAll);
	this->InitGrid(&m_GridSetForLevel);
	this->ArrangeSetInfluenceRate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDlgSetInfluenceRate::OnGridClickSetForLevel(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;	
	if (0 >= pItem->iRow || 1 != pItem->iColumn)
	{
		m_GridSetForLevel.SetSelectedRange(-1,-1,-1,-1);
		return;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
/// \author		cmkwon
/// \date		2009-09-22 ~ 2009-09-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CDlgSetInfluenceRate::OnGridClickSetForAll(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;	
	if (0 >= pItem->iRow || 1 != pItem->iColumn)
	{
		m_GridSetForAll.SetSelectedRange(-1,-1,-1,-1);
		return;
	}
}

void CDlgSetInfluenceRate::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	
	if(FALSE == m_bEnableEdit
		|| IDOK != AfxMessageBox("Do you want to save changed things?", MB_OKCANCEL))
	{
		CDialog::OnOK();
		return;
	}

	if(FALSE == this->UpdateDataInfluenceRate())
	{
		AfxMessageBox("this->UpdateDataInfluenceRate error !! ");
		CDialog::OnOK();
		return;		
	}
	
	if(FALSE == this->DB_UpdateInfluenceRateList(m_ctlbSetForAll, &m_vectInflUserList))
	{
		AfxMessageBox("this->DB_UpdateInfluenceRateList error !! ");
		CDialog::OnOK();
		return;
	}

	CDialog::OnOK();
}

