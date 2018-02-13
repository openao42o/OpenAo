// guildmarkrecognitiondlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "guildmarkrecognitiondlg.h"
#include "SCGridHelper.h"
#include "SCGuildAdminDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuildMarkRecognitionDlg dialog

// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 인자추가(, BOOL i_bEnableEdit)
CGuildMarkRecognitionDlg::CGuildMarkRecognitionDlg(CODBCStatement *i_pODBCStat, BOOL i_bEnableEdit, CWnd* pParent /*=NULL*/)
	: CDialog(CGuildMarkRecognitionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGuildMarkRecognitionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pODBCStmt4 = i_pODBCStat;

	m_bEnableEdit2		= i_bEnableEdit;	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 
}


void CGuildMarkRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuildMarkRecognitionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_GUILD_MARK_LIST, m_GridGuildMarkList);
}


BEGIN_MESSAGE_MAP(CGuildMarkRecognitionDlg, CDialog)
	//{{AFX_MSG_MAP(CGuildMarkRecognitionDlg)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_GRID_GUILD_MARK_LIST, OnGridGuildMarkListClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuildMarkRecognitionDlg message handlers
BOOL CGuildMarkRecognitionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitGridGuildMarkList();

	this->GetGuildMarkList();
	this->ViewGridGuildMarkList();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildMarkRecognitionDlg::OnGridGuildMarkListClick(NMHDR *pNotifyStruct, LRESULT* pResult)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildMarkRecognitionDlg::OnGridGuildMarkListClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	
	if (GRID_COL_INDEX_GUILD_MARK_STATE != pItem->iColumn)
	{
		return;
	}

	if(FALSE == m_bEnableEdit2)
	{// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 여단마크 상태 수정시 권한 체크
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}

	if(0 == pItem->iRow)
	{
		int nImageIndex = 0;
		CGridCellBase *pGCell = m_GridGuildMarkList.GetCell(0, pItem->iColumn);
		if(0 == pGCell->GetImage())
		{
			nImageIndex	= 1;
		}

		for(int i=0; i < m_GridGuildMarkList.GetRowCount(); i++)
		{
			pGCell = m_GridGuildMarkList.GetCell(i, pItem->iColumn);
			pGCell->SetImage(nImageIndex);
			m_GridGuildMarkList.Invalidate();
		}
		return;
	}

	CGridCellBase *pGCell = m_GridGuildMarkList.GetCell(pItem->iRow, pItem->iColumn);
	if(0 == pGCell->GetImage())
	{
		pGCell->SetImage(1);
	}
	else
	{
		pGCell->SetImage(0);
	}

	m_GridGuildMarkList.Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildMarkRecognitionDlg::InitGridGuildMarkList(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-02 ~ 2007-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildMarkRecognitionDlg::InitGridGuildMarkList(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-03 by cmkwon
	m_ilGuildMarkList2.DeleteImageList();
	m_ilGuildMarkList2.Create(24, 12, ILC_COLOR32, 1, 1);
	

	CBitmap bmCheckImage;
	bmCheckImage.LoadBitmap(IDB_BITMAP_CHECK_IMAGE);
	m_ilGuildMarkList2.Add(&bmCheckImage, RGB(0,0,0));
	m_GridGuildMarkList.SetImageList(&m_ilGuildMarkList2);
	
	m_GridGuildMarkList.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 5;

	m_GridGuildMarkList.SetEditable(FALSE);
	m_GridGuildMarkList.SetListMode(TRUE);
	m_GridGuildMarkList.SetSingleRowSelection(TRUE);
	m_GridGuildMarkList.EnableSelection(TRUE);
	m_GridGuildMarkList.SetFrameFocusCell(FALSE);
	m_GridGuildMarkList.SetTrackFocusCell(FALSE);

	m_GridGuildMarkList.SetRowCount(m_nRows);
	m_GridGuildMarkList.SetColumnCount(m_nCols);
	m_GridGuildMarkList.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format("NUM");
	m_GridGuildMarkList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("GuildUID");
	m_GridGuildMarkList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("GuildName");
	m_GridGuildMarkList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("MarkImage");
	m_GridGuildMarkList.SetItem(&Item);

	Item.col = m_nCols++;
	Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_IMAGE;
	Item.strText.Format("ALL");
	Item.iImage		= 0;
	Item.crBkClr	= RGB(200, 200, 200);
	m_GridGuildMarkList.SetItem(&Item);

	// arrange grid
	m_GridGuildMarkList.AutoSize();
	m_GridGuildMarkList.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridGuildMarkList.GetRowCount()-1, m_GridGuildMarkList.GetColumnCount()-1);
	m_GridGuildMarkList.ClearCells(tmpCellRange);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildMarkRecognitionDlg::ViewGridGuildMarkList(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-02 ~ 2007-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildMarkRecognitionDlg::ViewGridGuildMarkList(void)
{
	GV_ITEM Item;

	int nSize = m_vectGuildMarkList.size();
	for(int i=0; i < nSize; i++)
	{
		SGUILD_MARK_M1 *pGuild = &(m_vectGuildMarkList[i]);
		Item.mask		= GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat	= DT_LEFT|DT_VCENTER|DT_SINGLELINE;

		int nNewRowIdx = m_GridGuildMarkList.GetRowCount();
		m_GridGuildMarkList.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridGuildMarkList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%d", pGuild->GuildUID1);
		m_GridGuildMarkList.SetItem(&Item);

		Item.col++;
		Item.strText.Format("%s", pGuild->GuildName1);
		m_GridGuildMarkList.SetItem(&Item);

		COLORREF crRef = RGB(0,0,0);
		CBitmap bitmap;
		if(CSCGuildAdminDlg::LoadBMPFileFromMemory(&bitmap, pGuild->GuildMarkImage1, SIZE_MAX_GUILD_MARK_IMAGE))
		{
			Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
			Item.col++;
			Item.iImage		= m_ilGuildMarkList2.GetImageCount();
			Item.crBkClr	= RGB(200, 200, 200);
			m_ilGuildMarkList2.Add(&bitmap, crRef);
		}
		else
		{
			Item.strText.Empty();
			Item.col++;
		}
		m_GridGuildMarkList.SetItem(&Item);

		Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
		Item.col++;
		Item.iImage		= 0;
		Item.crBkClr	= RGB(200, 200, 200);
		m_GridGuildMarkList.SetItem(&Item);		
	}
	m_GridGuildMarkList.UpdateData();
	m_GridGuildMarkList.AutoSize();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildMarkRecognitionDlg::AddLineToGridGuildMarkListMembers(const char *i_szKey, CBitmap *i_pbmGuildMark)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-02 ~ 2007-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildMarkRecognitionDlg::AddLineToGridGuildMarkListMembers(const char *i_szKey, CBitmap *i_pbmGuildMark)
{
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SGUILD_MARK_M1 *CGuildMarkRecognitionDlg::FindGuildMarkByGuildUID(UID32_t i_guildUID)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-03 ~ 2007-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SGUILD_MARK_M1 *CGuildMarkRecognitionDlg::FindGuildMarkByGuildUID(UID32_t i_guildUID)
{
	for(int i=0; i < m_vectGuildMarkList.size(); i++)
	{
		if(i_guildUID == m_vectGuildMarkList[i].GuildUID1)
		{
			return &m_vectGuildMarkList[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildMarkRecognitionDlg::GetGuildMarkList(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-02 ~ 2007-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildMarkRecognitionDlg::GetGuildMarkList(void)
{
	CString szSQLQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT GuildUniqueNumber, GuildName, GuildMarkImage, GuildMarkState \
// 				FROM td_Guild WITH (NOLOCK) WHERE GuildMarkState = %d", GUILD_MARK_STATE_WAITING_PERMISSION);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0054, GUILD_MARK_STATE_WAITING_PERMISSION);
//	BOOL bRet = m_pODBCStmt4->ExecuteQuery((LPCSTR)szSQLQuery);
	BYTE byGuildMarkState = GUILD_MARK_STATE_WAITING_PERMISSION;
	SQLHSTMT hstmt = m_pODBCStmt4->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(byGuildMarkState), 0,				&arrCB2[1]);
	BOOL bRet = m_pODBCStmt4->ExecuteQuery((char*)(PROCEDURE_080827_0054));
	if (!bRet)
	{
		m_pODBCStmt4->FreeStatement();		// clean up
		return FALSE;
	}

	SQLINTEGER	cb[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SGUILD_MARK_M1		tmGUILD;			util::zero(&tmGUILD, sizeof(tmGUILD));

	SQLBindCol(m_pODBCStmt4->m_hstmt, 1, SQL_C_ULONG, &tmGUILD.GuildUID1, 0,						&cb[1]);
	SQLBindCol(m_pODBCStmt4->m_hstmt, 2, SQL_C_CHAR, tmGUILD.GuildName1, SIZE_MAX_GUILD_NAME,		&cb[2]);
	SQLBindCol(m_pODBCStmt4->m_hstmt, 3, SQL_C_BINARY, &tmGUILD.GuildMarkImage1, SIZE_MAX_GUILD_MARK_IMAGE,	&cb[3]);
	SQLBindCol(m_pODBCStmt4->m_hstmt, 4, SQL_C_TINYINT, &tmGUILD.GuildMarkState1, 0,				&cb[4]);

	while(TRUE)
	{
		util::zero(&tmGUILD, sizeof(tmGUILD));
		SQLRETURN nSRet = SQLFetch(m_pODBCStmt4->m_hstmt);
		if(nSRet != SQL_SUCCESS && nSRet != SQL_SUCCESS_WITH_INFO)
		{
			break;
		}
		m_vectGuildMarkList.push_back(tmGUILD);
	}	
	m_pODBCStmt4->FreeStatement();		// clean up
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CGuildMarkRecognitionDlg::UpdateGuildMarkState(vectSGUILD_MARK_M1 *i_pvectGuildLit)
/// \brief		
/// \author		cmkwon
/// \date		2007-08-02 ~ 2007-08-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CGuildMarkRecognitionDlg::UpdateGuildMarkState(vectSGUILD_MARK_M1 *i_pvectGuildLit)
{
	int nUpdateCnts = 0;

	for(int i=0; i < i_pvectGuildLit->size(); i++)
	{
		SGUILD_MARK_M1 *pGuildMark = &((*i_pvectGuildLit)[i]);
		
		CString szSQLQuery;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szSQLQuery.Format("UPDATE td_guild SET GuildMarkState = %d WHERE GuildUniqueNumber = %u", pGuildMark->GuildMarkState1, pGuildMark->GuildUID1);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		szSQLQuery.Format(QUERY_080702_0055, pGuildMark->GuildMarkState1, pGuildMark->GuildUID1);
//		BOOL bRet = m_pODBCStmt4->ExecuteQuery((LPCSTR)szSQLQuery);
		SQLHSTMT hstmt = m_pODBCStmt4->GetSTMTHandle();
		SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(pGuildMark->GuildUID1), 0,				&arrCB2[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(pGuildMark->GuildMarkState1), 0,		&arrCB2[2]);
		BOOL bRet = m_pODBCStmt4->ExecuteQuery((char*)(PROCEDURE_080827_0055));
		if (!bRet)
		{
			m_pODBCStmt4->FreeStatement();		// clean up
			return nUpdateCnts;
		}
		SQLRETURN nSRet = SQLMoreResults(m_pODBCStmt4->m_hstmt);
		m_pODBCStmt4->FreeStatement();		// clean up

		nUpdateCnts++;
	}
	return nUpdateCnts;
}

void CGuildMarkRecognitionDlg::OnOK() 
{
	// TODO: Add extra validation here

	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 
	if(FALSE == m_bEnableEdit2)
	{
		CDialog::OnOK();
		return;
	}

	CGridCellBase *pGuildUIDColCell = NULL;
	CGridCellBase *pMarkStateColCell = NULL;
	for(int i=1; i < m_GridGuildMarkList.GetRowCount(); i++)
	{
		pGuildUIDColCell	= m_GridGuildMarkList.GetCell(i, GRID_COL_INDEX_GUILD_UID);
		SGUILD_MARK_M1 *pGuildMark = this->FindGuildMarkByGuildUID(atoi(pGuildUIDColCell->GetText()));
		if(NULL == pGuildMark)
		{
			continue;
		}

		pMarkStateColCell	= m_GridGuildMarkList.GetCell(i, GRID_COL_INDEX_GUILD_MARK_STATE);
		if(0 != pMarkStateColCell->GetImage())
		{
			pGuildMark->GuildMarkState1	= GUILD_MARK_STATE_NORMAL;
		}		
	}
	
	vectSGUILD_MARK_M1::iterator	itr(m_vectGuildMarkList.begin());
	while(itr != m_vectGuildMarkList.end())
	{
		if(GUILD_MARK_STATE_NORMAL != itr->GuildMarkState1)
		{
			itr = m_vectGuildMarkList.erase(itr);
			continue;
		}
		itr++;
	}

	if(0 < m_vectGuildMarkList.size())
	{
		CString csMsg;
		csMsg.Format(STRMSG_070802_0003, m_vectGuildMarkList.size());
		if(IDCANCEL == AfxMessageBox(csMsg, MB_OKCANCEL))
		{
			return;
		}

		this->UpdateGuildMarkState(&m_vectGuildMarkList);
	}
	
	CDialog::OnOK();
}
