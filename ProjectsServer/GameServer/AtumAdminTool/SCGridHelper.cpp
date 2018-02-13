#include "StdAfx.h"
#include "SCGridHelper.h"

///////////////////////////////////////////////////////////////////////////////
// GRID_BAD_USER
///////////////////////////////////////////////////////////////////////////////
const char *g_arrBadUserColumnNames[] = {
	"AccountName",
	"Type",
	"Previous Cause",
	"Start Date",
	"End Date",
	"Handler",
	"Handling Cause",
	NULL
};

///////////////////////////////////////////////////////////////////////////////
// GRID_CONNECTION_LOG
///////////////////////////////////////////////////////////////////////////////
const char *g_arrConnectionLogColumnNames[] = {
	"AccountName",
	"Date",
	"Log Type",
	"IPAddress",
	"ServerGroup Name",
	"MAC Address",	// 2015-09-23 Future, added logging of MAC Address
	NULL
};

///////////////////////////////////////////////////////////////////////////////
// GRID_USER_LOG
///////////////////////////////////////////////////////////////////////////////
const char *g_arrUserLogColumnNames[] = {
	"AccountName",
	"CharacterName",
	"Map and Position",
	"Date",
	"Log Type",
	"Description",
	NULL
};

///////////////////////////////////////////////////////////////////////////////
// GRID_ITEM_LOG
///////////////////////////////////////////////////////////////////////////////
const char *g_arrItemLogColumnNames[] = {
	"AccountName",
	"CharacterName",
	"Date",
	"Log Type",
	"Item Name",
	"Item UID",
	"Description",
	NULL
};


// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
///////////////////////////////////////////////////////////////////////////////
// GRID_ITEM_LOG
///////////////////////////////////////////////////////////////////////////////
const char *g_arrStaisicsMoneyColumnNames[] = {
	"Count",
	"Time",
	"SPI Gap",
	"SPI Current",
	"WP Gap",
	"WP Current",
	"S-Orb Gap", //"SystemOrb Gap",
	"S-Orb Current", //"SystemOrb Current",
	"Q-Orb Gap", //"QuasarOrb Gap",
	"Q-Orb Current",//"QuasarOrb Current",
	"C-Orb Gap", //"ClusterOrb Gap",
	"C-Orb Current",//"ClusterOrb Current",
	"G-Orb Gap", //"GalaxyOrb Gap",
	"G-Orb Current",//"GalaxyOrb Current",
	"U-Orb Gap", //"UniverseOrb Gap",
	"U-Orb Current",//"UniverseOrb Current",
	NULL
};
// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가


///////////////////////////////////////////////////////////////////////////////
// CSCGridCtrl
///////////////////////////////////////////////////////////////////////////////
void CSCGridCtrl::ArrangeGrid()
{
	// arrange grid
	AutoSize();
	ExpandColumnsToFit();
}

void CSCGridCtrl::InitGrid(int i_nColumns, const char *i_arrColName[])
{
	SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = i_nColumns;

	SetEditable(TRUE);
	SetListMode(TRUE);
	SetSingleRowSelection(TRUE);
	EnableSelection(TRUE);
	SetFrameFocusCell(FALSE);
	SetTrackFocusCell(FALSE);
	SetHeaderSort(TRUE);

	SetRowCount(m_nRows);
	SetColumnCount(m_nCols);
	SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	for (int i = 0; i < i_nColumns; i++)
	{
		Item.col = i;
		Item.strText.Format(_T(i_arrColName[i]));
		SetItem(&Item);
	}

	ArrangeGrid();

	// clean all cells
	CCellRange tmpCellRange(1, 0, GetRowCount()-1, GetColumnCount()-1);
	ClearCells(tmpCellRange);
}

void CSCGridCtrl::InsertGridBadUser(BAD_USER_INFO *i_pBadUserInfo)
{
	SetRowCount(GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = COL_BAD_USER_ACCOUNT_NAME;
	Item.strText = i_pBadUserInfo->AccountName;
	SetItem(&Item);

	Item.col = COL_BAD_USER_BLOCK_TYPE;
	if (i_pBadUserInfo->BlockType == BLOCK_TYPE_ACCOUNT_BLOCK)
		Item.strText = BLOCK_TYPE_STRING_ACCOUNT_BLOCK;
	else if (i_pBadUserInfo->BlockType == BLOCK_TYPE_CHATTING_BLOCK)
		Item.strText = BLOCK_TYPE_STRING_CHATTING_BLOCK;
	else
		Item.strText = "";
	SetItem(&Item);

	Item.col = COL_BAD_USER_PREV_BLOCK;
	Item.strText = i_pBadUserInfo->PrevBlock;
	SetItem(&Item);

	Item.col = COL_BAD_USER_START_TIME;
	Item.strText = i_pBadUserInfo->StartTime.GetDateTimeString().GetBuffer();
	SetItem(&Item);

	Item.col = COL_BAD_USER_END_TIME;
	Item.strText = i_pBadUserInfo->EndTime.GetDateTimeString().GetBuffer();
	SetItem(&Item);

	Item.col = COL_BAD_USER_ADMIN_NAME;
	Item.strText = i_pBadUserInfo->AdminName;
	SetItem(&Item);

	Item.col = COL_BAD_USER_REASON;
	Item.strText = i_pBadUserInfo->Reason;
	SetItem(&Item);

//	ArrangeGrid();
}

BAD_USER_INFO CSCGridCtrl::GetGridBadUser(INT i_nRow)
{
	BAD_USER_INFO ret;

	if (i_nRow < 1 || i_nRow >= GetRowCount())
	{
		return ret;
	}

	ret.AccountName = GetItemText(i_nRow, COL_BAD_USER_ACCOUNT_NAME);
	if (GetItemText(i_nRow, COL_BAD_USER_BLOCK_TYPE) == BLOCK_TYPE_STRING_ACCOUNT_BLOCK)
		ret.BlockType = BLOCK_TYPE_ACCOUNT_BLOCK;
	else if (GetItemText(i_nRow, COL_BAD_USER_BLOCK_TYPE) == BLOCK_TYPE_STRING_CHATTING_BLOCK)
		ret.BlockType = BLOCK_TYPE_CHATTING_BLOCK;
	else
		ret.BlockType = BLOCK_TYPE_NONE;
	ret.PrevBlock = GetItemText(i_nRow, COL_BAD_USER_PREV_BLOCK);
	ret.StartTime.SetDateTime(GetItemText(i_nRow, COL_BAD_USER_START_TIME));
	ret.EndTime.SetDateTime(GetItemText(i_nRow, COL_BAD_USER_END_TIME));
	ret.AdminName = GetItemText(i_nRow, COL_BAD_USER_ADMIN_NAME);
	ret.Reason = GetItemText(i_nRow, COL_BAD_USER_REASON);

	return ret;
}

void CSCGridCtrl::InsertGridConnectionLog(CONNECTION_LOG_INFO *i_pConnLogInfo)
{
	SetRowCount(GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = COL_CONN_LOG_ACCOUNT_NAME;
	Item.strText = i_pConnLogInfo->AccountName;
	SetItem(&Item);

	Item.col = COL_CONN_LOG_DATE_TIME;
	Item.strText = i_pConnLogInfo->Time.GetSQLDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING));
	SetItem(&Item);

	Item.col = COL_CONN_LOG_TYPE;
	Item.strText = i_pConnLogInfo->LogType;
	SetItem(&Item);

	Item.col = COL_CONN_LOG_IPADDRESS;
	Item.strText = i_pConnLogInfo->IPAddress;
	SetItem(&Item);

	Item.col = COL_CONN_LOG_SERVER_NAME;
	Item.strText = i_pConnLogInfo->ServerName;
	SetItem(&Item);

	Item.col = COL_CONN_LOG_MAC_ADDRESS;
	Item.strText = i_pConnLogInfo->MACAddress;
	SetItem(&Item);

//	ArrangeGrid();
}

CONNECTION_LOG_INFO CSCGridCtrl::GetGridConnectionLog(INT i_nRow)
{
	CONNECTION_LOG_INFO ret;

	return ret;
}

void CSCGridCtrl::InsertGridUserLog(USER_LOG_INFO *i_pUserLogInfo)
{
	SetRowCount(GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = COL_USER_LOG_ACCOUNT_NAME;
	Item.strText = i_pUserLogInfo->AccountName;
	SetItem(&Item);

	Item.col = COL_USER_LOG_CHARACTER_NAME;
	Item.strText = i_pUserLogInfo->CharacterName;
	SetItem(&Item);

	Item.col = COL_USER_LOG_POSITION;
	Item.strText = i_pUserLogInfo->Position;
	SetItem(&Item);

	Item.col = COL_USER_LOG_DATE_TIME;	
	Item.strText = i_pUserLogInfo->Time.GetDateTimeString().GetBuffer();
	SetItem(&Item);

	Item.col = COL_USER_LOG_TYPE;
	Item.strText = i_pUserLogInfo->LogType;
	SetItem(&Item);

	Item.col = COL_USER_LOG_CONTENTS;
	Item.strText = i_pUserLogInfo->Contents;
	SetItem(&Item);

//	ArrangeGrid();
}

USER_LOG_INFO CSCGridCtrl::GetGridUserLog(INT i_nRow)
{
	USER_LOG_INFO ret;

	return ret;
}

void CSCGridCtrl::InsertGridItemLog(ITEM_LOG_INFO *i_pItemLogInfo)
{
	SetRowCount(GetRowCount()+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = COL_ITEM_LOG_ACCOUNT_NAME;
	Item.strText = i_pItemLogInfo->AccountName;
	SetItem(&Item);

	Item.col = COL_ITEM_LOG_CHARACTER_NAME;
	Item.strText = i_pItemLogInfo->CharacterName;
	SetItem(&Item);

	Item.col = COL_ITEM_LOG_DATE_TIME;	
	Item.strText = i_pItemLogInfo->Time.GetDateTimeString().GetBuffer();
	SetItem(&Item);

	Item.col = COL_ITEM_LOG_TYPE;
	Item.strText = i_pItemLogInfo->LogType;
	SetItem(&Item);

	Item.col = COL_ITEM_LOG_ITEM;
	Item.strText = i_pItemLogInfo->ItemString;
	SetItem(&Item);

	Item.col = COL_ITEM_LOG_ITEM_UID;
	Item.strText.Format("%d", i_pItemLogInfo->ItemUID);
	SetItem(&Item);

	Item.col = COL_ITEM_LOG_CONTENTS;
	Item.strText = i_pItemLogInfo->Contents;
	SetItem(&Item);

//	ArrangeGrid();
}

ITEM_LOG_INFO CSCGridCtrl::GetGridItemLog(INT i_nRow)
{
	ITEM_LOG_INFO ret;

	return ret;
}


// start 2012-02-27 by jhseol, 화폐통계툴 기능 추가
void CSCGridCtrl::InsertGridItemStaisticsMoney(SSTAISTICSMONEY_INFO *i_pItemLogInfo)
{
	char szTemp[50]={0x00,};
	SetRowCount(GetRowCount()+1);
	
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_INT64;
	Item.row = GetRowCount()-1;
	Item.nFormat = GRID_CELL_FORMAT;
	
	Item.col = COL_ITEM_STAISTICSMONEY_COUNT;
	Item.strText = itoa(i_pItemLogInfo->Count, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_TIME;
	Item.strText = i_pItemLogInfo->Time.GetDateTimeString().GetBuffer();
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_SPI_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->SPIGap, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_SPI_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->SPICurrent, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_WP_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->WPGap, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_WP_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->WPCurrent, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_SORB_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->SOrbGap, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_SORB_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->SOrbCurrent, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_QORB_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->QOrbGap, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_QORB_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->QOrbCurrent, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_CORB_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->COrbGap, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_CORB_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->COrbCurrent, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_GORB_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->GOrbGap, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_GORB_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->GOrbCurrent, szTemp, 10);
	SetItem(&Item);
	
	Item.col = COL_ITEM_STAISTICSMONEY_UORB_GAP;	
	Item.strText = _i64toa(i_pItemLogInfo->UOrbGap, szTemp, 10);
	SetItem(&Item);

	Item.col = COL_ITEM_STAISTICSMONEY_UORB_CURRENT;	
	Item.strText = _i64toa(i_pItemLogInfo->UOrbCurrent, szTemp, 10);
	SetItem(&Item);
}

SSTAISTICSMONEY_INFO CSCGridCtrl::GetGridStaisticsMoney(INT i_nRow)
{
	SSTAISTICSMONEY_INFO ret;

	return ret;
}
// end 2012-02-27 by jhseol, 화폐통계툴 기능 추가