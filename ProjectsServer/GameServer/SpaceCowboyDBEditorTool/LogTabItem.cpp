// LogTabItem.cpp : implementation file
//

#include "stdafx.h"
#include "LogTabItem.h"
#include "SCLogAdminDlg.h"
#include "ODBCStatement.h"
#include "AtumProtocol.h"
#include "SelectItemDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _STRMSG_S_SCAT_0100 "CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define _STRMSG_S_SCAT_0101 "CAST(l.ChangeCount AS VARCHAR(10)) + 'counts Pick up, ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define _STRMSG_S_SCAT_0102 "'Gave ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'to ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define _STRMSG_S_SCAT_0103 "'Received ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'from ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define _STRMSG_S_SCAT_0104 "'Throw away ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts, ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define _STRMSG_S_SCAT_0105 "'Buy ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts, Remained SPI: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
#define _STRMSG_S_SCAT_0106 "'Sell ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts, Remained SPI: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
#define _STRMSG_S_SCAT_0107 "'Send ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'to ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"
#define _STRMSG_S_SCAT_0108 "'Received ' + CAST(l.ChangeCount AS VARCHAR(10)) + 'counts ' + 'from ''' + l.PeerCharacterName + ''', ' + CAST(l.CurrentCount AS VARCHAR(10)) + 'counts'"




/////////////////////////////////////////////////////////////////////////////
// CLogTabItem dialog


CLogTabItem::CLogTabItem(CDialog *i_pMainDlg, CWnd* pParent /*=NULL*/)
	: CDialog(CLogTabItem::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogTabItem)
	m_szCharacterName = _T("");
	m_bCheckCharacterName = FALSE;
	m_bCheckDate = TRUE;
	m_dateStart = (CTime::GetCurrentTime() - CTimeSpan(1, 0 , 0, 0));
	m_dateEnd = CTime::GetCurrentTime();
	m_nMaxRowCount = 100;
	m_bCheckAddItemByAdmin = FALSE;
	m_bCheckAddItemByCommand = FALSE;
	m_bCheckBuyItem = FALSE;
	m_bCheckCharacterItem = FALSE;
	m_bCheckDelItemByAdmin = FALSE;
	m_bCheckDrawFromStore = FALSE;
	m_bCheckGetItem = FALSE;
	m_bCheckInsertToStore = FALSE;
	m_bCheckMaxRowCount = TRUE;
	m_bCheckSellItem = FALSE;
	m_bCheckThrowAwayItem = FALSE;
	m_bCheckTradeRecv = FALSE;
	m_bCheckTradeSend = FALSE;
	m_bCheckUseEnergy = FALSE;
	m_bCheckUseEnchant = FALSE;
	m_bCheckUseItem = FALSE;
	m_bCheckAccountName = FALSE;
	m_szAccountName = _T("");
	m_szAccountName2 = _T("");
	m_szCharacterName2 = _T("");
	m_bCheckAccountName2 = FALSE;
	m_bCheckCharacterName2 = FALSE;
	m_szSelectedItemName = _T("");
	m_nItemUID = 0;
	m_bCheckItemUID = FALSE;
	m_bCheckItemName = FALSE;
	m_timeEnd = 0;
	m_timeStart = 0;
	m_bCheckStoreStartItem = FALSE;
	m_bCheckBazaarBuy = FALSE;
	m_bCheckBazaarSell = FALSE;
	m_ctlbCheckGiftRecv = FALSE;
	m_ctlbCheckGiftSend = FALSE;
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSCLogAdminDlg*)i_pMainDlg;
	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//m_pODBCStmt = &m_pMainDlg->m_ODBCStmt;
	m_pODBCStmt = m_pMainDlg->m_pODBCStmt;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

	m_pmapItemInfo		= m_pMainDlg->m_pmapItemInfo;		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	m_pmapItemInfoName	= m_pMainDlg->m_pmapItemInfoName;	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
}

void CLogTabItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTabItem)
	DDX_Control(pDX, IDC_EDIT_TOTAL_LOG_ROW_COUNT, m_EditTotalLogRowCount);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME, m_szCharacterName);
	DDX_Check(pDX, IDC_CHECK_CHARACTER_1, m_bCheckCharacterName);
	DDX_Check(pDX, IDC_CHECK_DATE, m_bCheckDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_DATE, m_dateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_DATE, m_dateEnd);
	DDX_Text(pDX, IDC_EDIT_MAX_ROW_COUNT, m_nMaxRowCount);
	DDX_Check(pDX, IDC_CHECK_ADD_ITEM_BY_ADMIN, m_bCheckAddItemByAdmin);
	DDX_Check(pDX, IDC_CHECK_ADD_ITEM_BY_COMMAND, m_bCheckAddItemByCommand);
	DDX_Check(pDX, IDC_CHECK_BUY_ITEM, m_bCheckBuyItem);
	DDX_Check(pDX, IDC_CHECK_CHARACTER_ITEM, m_bCheckCharacterItem);
	DDX_Check(pDX, IDC_CHECK_DEL_ITEM_BY_ADMIN, m_bCheckDelItemByAdmin);
	DDX_Check(pDX, IDC_CHECK_DRAW_FROM_STORE, m_bCheckDrawFromStore);
	DDX_Check(pDX, IDC_CHECK_GET_ITEM, m_bCheckGetItem);
	DDX_Check(pDX, IDC_CHECK_INSERT_TO_STORE, m_bCheckInsertToStore);
	DDX_Check(pDX, IDC_CHECK_MAX_ROW_COUNT, m_bCheckMaxRowCount);
	DDX_Check(pDX, IDC_CHECK_SELL_ITEM, m_bCheckSellItem);
	DDX_Check(pDX, IDC_CHECK_THROWAWAY_ITEM, m_bCheckThrowAwayItem);
	DDX_Check(pDX, IDC_CHECK_TRADE_RECV, m_bCheckTradeRecv);
	DDX_Check(pDX, IDC_CHECK_TRADE_SEND, m_bCheckTradeSend);
	DDX_Check(pDX, IDC_CHECK_USE_ENERGY, m_bCheckUseEnergy);
	DDX_Check(pDX, IDC_CHECK_USE_ENCHANT, m_bCheckUseEnchant);
	DDX_Check(pDX, IDC_CHECK_USE_ITEM, m_bCheckUseItem);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT_NAME, m_bCheckAccountName);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_NAME, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_NAME2, m_szAccountName2);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME2, m_szCharacterName2);
	DDX_Check(pDX, IDC_CHECK_ACCOUNT_NAME2, m_bCheckAccountName2);
	DDX_Check(pDX, IDC_CHECK_CHARACTER2, m_bCheckCharacterName2);
	DDX_Text(pDX, IDC_EDIT_SELECTED_ITEM, m_szSelectedItemName);
	DDX_Text(pDX, IDC_EDIT_ITEM_UID, m_nItemUID);
	DDX_Check(pDX, IDC_CHECK_ITEM_UID, m_bCheckItemUID);
	DDX_Check(pDX, IDC_CHECK_ITEM_NUM, m_bCheckItemName);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END_TIME, m_timeEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START_TIME, m_timeStart);
	DDX_Check(pDX, IDC_CHECK_STORE_START_ITEM, m_bCheckStoreStartItem);
	DDX_Check(pDX, IDC_CHECK_Bazaar_Buy, m_bCheckBazaarBuy);
	DDX_Check(pDX, IDC_CHECK_Bazaar_Sell, m_bCheckBazaarSell);
	DDX_Check(pDX, IDC_CHECK_GIFT_RECV, m_ctlbCheckGiftRecv);
	DDX_Check(pDX, IDC_CHECK_GIFT_SEND, m_ctlbCheckGiftSend);
	DDX_Check(pDX, IDC_CHECK_REARRANGEITEM_DELETE, m_ctlbCheckRearrangeItemDeleted);
	DDX_Check(pDX, IDC_CHECK_REARRANGEITEM_ITEM, m_ctlbCheckRearrangeItemMain);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_ITEM_LOG, m_GridItemLog);
}


BEGIN_MESSAGE_MAP(CLogTabItem, CDialog)
	//{{AFX_MSG_MAP(CLogTabItem)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_NAME, OnCheckAccountName)
	ON_BN_CLICKED(IDC_CHECK_CHARACTER_1, OnCheckCharacter)
	ON_BN_CLICKED(IDC_CHECK_DATE, OnCheckDate)
	ON_BN_CLICKED(IDC_CHECK_MAX_ROW_COUNT, OnCheckMaxRowCount)
	ON_BN_CLICKED(IDC_CHECK_ACCOUNT_NAME2, OnCheckAccountName2)
	ON_BN_CLICKED(IDC_CHECK_CHARACTER2, OnCheckCharacter2)
	ON_BN_CLICKED(IDC_CHECK_TRADE_RECV, OnCheckTradeRecv)
	ON_BN_CLICKED(IDC_CHECK_TRADE_SEND, OnCheckTradeSend)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_DISELECT_ALL, OnButtonDiselectAll)
	ON_BN_CLICKED(IDC_CHECK_ITEM_NUM, OnCheckItemNum)
	ON_BN_CLICKED(IDC_CHECK_ITEM_UID, OnCheckItemUid)
	ON_BN_CLICKED(IDC_BTN_SAVE_RESULT, OnBtnSaveResult)
	ON_BN_CLICKED(IDC_CHECK_Bazaar_Sell, OnCHECKBazaarSell)
	ON_BN_CLICKED(IDC_CHECK_Bazaar_Buy, OnCHECKBazaarBuy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTabItem message handlers

BOOL CLogTabItem::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		m_pMainDlg->EndDialog(-1);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLogTabItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_GridItemLog.InitGrid(NUM_COLUMNS_ITEM_LOG_GRID, g_arrItemLogColumnNames);

	ResetVariables();
	ResetControls();

// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
//	// 데이타 로딩
//	CAtumDBHelper::LoadItemInfo(m_pODBCStmt, &m_mapItemInfo, &(m_pMainDlg->m_pMainDlg->m_Localization), &m_mapItemInfoName);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogTabItem::ResetVariables()
{
	m_szCharacterName = _T("");
	m_szAccountName = _T("");
	m_szAccountName2 = _T("");
	m_szCharacterName2 = _T("");
	m_bCheckCharacterName = FALSE;
	m_bCheckAccountName = FALSE;
	m_bCheckAccountName2 = FALSE;
	m_bCheckCharacterName2 = FALSE;

	m_bCheckDate = TRUE;
	m_dateStart = (CTime::GetCurrentTime() - CTimeSpan(1, 0, 0, 0));
	m_dateEnd = CTime::GetCurrentTime();
	m_timeStart = CTime(1977, 1, 1, 0, 0, 0);
	m_timeEnd = CTime(1977, 1, 1, 23, 59, 59);
	m_bCheckMaxRowCount = TRUE;
	m_nMaxRowCount = 100;

	// check log type
	m_bCheckAddItemByAdmin = TRUE;
	m_bCheckAddItemByCommand = TRUE;
	m_bCheckBuyItem = TRUE;
	m_bCheckCharacterItem = FALSE;
	m_bCheckDelItemByAdmin = TRUE;
	m_bCheckDrawFromStore = TRUE;
	m_bCheckGetItem = FALSE;
	m_bCheckInsertToStore = TRUE;
	m_bCheckStoreStartItem = FALSE;
	m_bCheckSellItem = TRUE;
	m_bCheckThrowAwayItem = TRUE;
	m_bCheckTradeRecv = TRUE;
	m_bCheckTradeSend = TRUE;
	m_bCheckUseEnergy = FALSE;
	m_bCheckUseEnchant = TRUE;
	m_bCheckUseItem = TRUE;
	m_bCheckBazaarSell = TRUE; //2006-8-02, dhjin
	m_bCheckBazaarBuy  = TRUE;
	m_EditTotalLogRowCount.SetWindowText("");

	UpdateData(FALSE);
}

void CLogTabItem::ResetControls()
{
	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);
	GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bCheckCharacterName);
	GetDlgItem(IDC_EDIT_ACCOUNT_NAME2)->EnableWindow(m_bCheckAccountName2);
	GetDlgItem(IDC_EDIT_CHARACTER_NAME2)->EnableWindow(m_bCheckCharacterName2);
	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);
	GetDlgItem(IDC_EDIT_SELECTED_ITEM)->EnableWindow(m_bCheckItemName);
	GetDlgItem(IDC_EDIT_ITEM_UID)->EnableWindow(m_bCheckItemUID);

	OnCheckCharacter();
}

void CLogTabItem::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_EditTotalLogRowCount.SetWindowText("");

	// 그리드 정리하기
	m_GridItemLog.InitGrid(NUM_COLUMNS_ITEM_LOG_GRID, g_arrItemLogColumnNames);

	BOOL bRet = TRUE;
	if (bRet && m_bCheckCharacterItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_CHARACTER_ITEM, "atum_log_item_charac_use",
								_STRMSG_S_SCAT_0100);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_CHARACTER_ITEM, "atum_backup_log_item_charac_use",
									_STRMSG_S_SCAT_0100);
		}
	}

	if (bRet && m_bCheckStoreStartItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_STORE_ITEM, "atum_log_item_charac_use",
								_STRMSG_S_SCAT_0100);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_STORE_ITEM, "atum_backup_log_item_charac_use",
									_STRMSG_S_SCAT_0100);
		}
	}

	if (bRet && m_bCheckGetItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_GET_ITEM, "atum_log_item_get_throw",
								_STRMSG_S_SCAT_0101);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_GET_ITEM, "atum_backup_log_item_get_throw",
									_STRMSG_S_SCAT_0101);
		}
	}

	if (bRet && m_bCheckTradeSend)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_TRADE_SEND, "atum_log_item_trade"
			, _STRMSG_S_SCAT_0102);

		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_TRADE_SEND, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0102);
		}
	}

	if (bRet && m_bCheckTradeRecv)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_TRADE_RECV, "atum_log_item_trade",
								_STRMSG_S_SCAT_0103);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_TRADE_RECV, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0103);
		}
	}

	if (bRet && m_bCheckThrowAwayItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_THROWAWAY_ITEM, "atum_log_item_get_throw",
								_STRMSG_S_SCAT_0104);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_THROWAWAY_ITEM, "atum_backup_log_item_get_throw",
									_STRMSG_S_SCAT_0104);
		}
	}

	if (bRet && m_bCheckBuyItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_BUY_ITEM, "atum_log_item_buy_sell",
								_STRMSG_S_SCAT_0105);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_BUY_ITEM, "atum_backup_log_item_buy_sell",
									_STRMSG_S_SCAT_0105);
		}
	}

	if (bRet && m_bCheckSellItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_SELL_ITEM, "atum_log_item_buy_sell",
								_STRMSG_S_SCAT_0106);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_SELL_ITEM, "atum_backup_log_item_buy_sell",
									_STRMSG_S_SCAT_0106);
		}
	}

	if (bRet && m_bCheckUseItem)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_USE_ITEM, "atum_log_item_charac_use",
								_STRMSG_S_SCAT_0100);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_USE_ITEM, "atum_backup_log_item_charac_use",
									_STRMSG_S_SCAT_0100);
		}
	}

	if (bRet && m_bCheckUseEnergy)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_USE_ENERGY, "atum_log_item_charac_use",
								_STRMSG_S_SCAT_0100);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_USE_ENERGY, "atum_backup_log_item_charac_use",
									_STRMSG_S_SCAT_0100);
		}
	}

	if (bRet && m_bCheckUseEnchant)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_USE_ENCHANT, "atum_log_item_charac_use",
								STRMSG_S_SCADMINTOOL_050512_0000);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_USE_ENCHANT, "atum_backup_log_item_charac_use",
									STRMSG_S_SCADMINTOOL_050512_0000);
		}
	}

	if (bRet && m_bCheckAddItemByAdmin)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN, "atum_log_item_trade",
								STRMSG_S_SCADMINTOOL_0012);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_ADD_ITEM_BY_ADMIN, "atum_backup_log_item_trade",
									STRMSG_S_SCADMINTOOL_0012);
		}
	}

	if (bRet && m_bCheckDelItemByAdmin)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, "atum_log_item_trade",
								STRMSG_S_SCADMINTOOL_0013);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, "atum_backup_log_item_trade",
									STRMSG_S_SCADMINTOOL_0013);
		}
	}

	if (bRet && m_bCheckAddItemByCommand)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, "atum_log_item_get_throw",
								STRMSG_S_SCADMINTOOL_0014);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND, "atum_backup_log_item_get_throw",
									STRMSG_S_SCADMINTOOL_0014);
		}
	}

	if (bRet && m_bCheckInsertToStore)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_INSERT_TO_STORE, "atum_log_item_get_throw",
								STRMSG_S_SCADMINTOOL_0015);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_INSERT_TO_STORE, "atum_backup_log_item_get_throw",
									STRMSG_S_SCADMINTOOL_0015);
		}
	}

	if (bRet && m_bCheckDrawFromStore)
	{
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_DRAW_FROM_STORE, "atum_log_item_get_throw",
								STRMSG_S_SCADMINTOOL_0016);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_DRAW_FROM_STORE, "atum_backup_log_item_get_throw",
									STRMSG_S_SCADMINTOOL_0016);
		}
	}

	if (bRet && m_bCheckBazaarSell)
	{//2006-8-02, dhjin
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_BAZAAR_SEND, "atum_log_item_trade"
			, _STRMSG_S_SCAT_0107);

		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_BAZAAR_SEND, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0107);
		}
	}

	if (bRet && m_bCheckBazaarBuy)
	{//2006-8-02, dhjin
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_BAZAAR_RECV, "atum_log_item_trade",
								_STRMSG_S_SCAT_0108);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_BAZAAR_RECV, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0108);
		}
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	if (bRet && m_ctlbCheckGiftSend)
	{// 2007-11-15 by cmkwon
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_GIFT_SEND, "atum_log_item_trade",
								_STRMSG_S_SCAT_0107);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_GIFT_SEND, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0107);
		}
	}
	if (bRet && m_ctlbCheckGiftRecv)
	{// 2007-11-15 by cmkwon
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_GIFT_RECV, "atum_log_item_trade",
								_STRMSG_S_SCAT_0108);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_GIFT_RECV, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0108);
		}
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 	
	if (bRet && m_ctlbCheckRearrangeItemDeleted)
	{// 2007-11-29 by cmkwon
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM, "atum_log_item_trade",
								_STRMSG_S_SCAT_0107);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_REARRANGE_DELETE_ITEM, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0107);
		}
	}
	if (bRet && m_ctlbCheckRearrangeItemMain)
	{// 2007-11-29 by cmkwon
		bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_REARRANGE_ITEM, "atum_log_item_trade",
								_STRMSG_S_SCAT_0108);
		if(bRet)
		{
			bRet = GetLogAndInsertToGrid(T1_FL_LOG_ITEM_REARRANGE_ITEM, "atum_backup_log_item_trade"
				, _STRMSG_S_SCAT_0108);
		}
	}


	m_GridItemLog.SortTextItems(COL_ITEM_LOG_DATE_TIME, FALSE);

	// 리턴된 로그 수 할당
	char szRowCountBuf[10];
	m_EditTotalLogRowCount.SetWindowText(itoa(m_GridItemLog.GetRowCount() - 1, szRowCountBuf, 10));
}

BOOL CLogTabItem::GetLogAndInsertToGrid(T1<T0::FL_LOG> T1_LogType, LPCSTR szTableName, LPCSTR szContentString)
{
	// max row count
	CString szQuery;
	if (m_bCheckMaxRowCount)
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery.Format("select top %d ", min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery.Format(QUERY_080702_0094_1_A, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	}
	else
	{
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szQuery = "select ";
		szQuery = QUERY_080702_0094_1_B;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	}

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery += " c.accountname, c.charactername, l.Time, i.LogStringKO, ii.itemnum, ii.itemname, l.ItemUniqueNumber, ";
// 	szQuery += szContentString;
// 	szQuery += " from atum_log_type_info i, ";
// 	szQuery += szTableName;
// 	szQuery += " l, ti_item ii, td_character c WITH (NOLOCK) where i.logtype = l.logtype and l.itemnum = ii.itemnum and c.uniquenumber = l.characteruniquenumber ";
	
	szQuery += QUERY_080702_0094_2;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szContentString;
	szQuery += QUERY_080702_0094_3;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szTableName;
	szQuery += QUERY_080702_0094_4;		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

	// log type
	CString szLogTypeCond;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szLogTypeCond.Format(" and l.logtype = %d ", T1_LogType);
	szLogTypeCond.Format(QUERY_080702_0094_5, T1_LogType);			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	szQuery += szLogTypeCond;
	
	// account
	if (m_bCheckAccountName)
	{
		CString szAccountCond;
		if (!m_bCheckAccountName2)
		{
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szAccountCond.Format(" and c.accountname = \'%s\' ", m_szAccountName);
			szAccountCond.Format(QUERY_080702_0094_6_A, m_szAccountName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		}
		else
		{
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szAccountCond.Format(" and (c.accountname = \'%s\' or c.accountname = \'%s\') ", m_szAccountName, m_szAccountName2);
			szAccountCond.Format(QUERY_080702_0094_6_B, m_szAccountName, m_szAccountName2);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		}
		szQuery += szAccountCond;
	}

	// character
	if (m_bCheckCharacterName)
	{
		CString szCharacterCond;
		if (m_bCheckCharacterName2 &&
			(T1_LogType == T1_FL_LOG_ITEM_TRADE_RECV || T1_LogType == T1_FL_LOG_ITEM_TRADE_SEND)
		)
		{
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szCharacterCond.Format(" and ((c.charactername = \'%s\' and l.PeerCharacterName = \'%s\')\
			//							or (c.charactername = \'%s\' and l.PeerCharacterName = \'%s\')) ",
			//							m_szCharacterName, m_szCharacterName2,
			//							m_szCharacterName2, m_szCharacterName);
			szCharacterCond.Format(QUERY_080702_0094_7_A,		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
				m_szCharacterName, m_szCharacterName2, m_szCharacterName2, m_szCharacterName);
		}
		else
		{
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szCharacterCond.Format(" and c.charactername = \'%s\' ", m_szCharacterName);
			szCharacterCond.Format(QUERY_080702_0094_7_B, m_szCharacterName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		}
		szQuery += szCharacterCond;
	}

	// date
	if (m_bCheckDate)
	{
		CString szDateCond;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szDateCond.Format(" and l.time > \'%s\' and l.time < \'%s\' ",
		//				(LPCSTR)(m_dateStart.Format("%Y/%m/%d") + m_timeStart.Format(" %H:%M:%S.0")),
		//				(LPCSTR)(m_dateEnd.Format("%Y/%m/%d") + m_timeEnd.Format(" %H:%M:%S.0")));
		szDateCond.Format(QUERY_080702_0094_8,		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
						(LPCSTR)(m_dateStart.Format("%Y/%m/%d") + m_timeStart.Format(" %H:%M:%S.0")),
						(LPCSTR)(m_dateEnd.Format("%Y/%m/%d") + m_timeEnd.Format(" %H:%M:%S.0")));
		szQuery += szDateCond;
	}

	// itemnum
	if (m_bCheckItemName)
	{
		CString szItemNum;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szItemNum.Format(" and l.ItemNum = %d ", m_nSelectedItemNum);
		szItemNum.Format(QUERY_080702_0094_9, m_nSelectedItemNum);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += szItemNum;
	}

	// ItemUID
	if (m_bCheckItemUID)
	{
		CString szItemUID;
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//szItemUID.Format(" and l.ItemUniqueNumber = %d ", m_nItemUID);
		szItemUID.Format(QUERY_080702_0094_10, m_nItemUID);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		szQuery += szItemUID;
	}

	// ordering
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szQuery += " order by l.time desc ";
	szQuery += QUERY_080702_0094_11;			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

#if defined(DB_SERVER_MYSQL)
	// 2008-07-08 by cmkwon, MySQL 지원 구현 - 
	if(m_bCheckMaxRowCount)
	{
		char szTm[1024];
		sprintf(szTm, QUERY_080702_0094_12_A, min(m_nMaxRowCount, SIZE_MAX_LOG_ROW_COUNT));
		szQuery += szTm;
	}
	else
	{
		szQuery += QUERY_080702_0094_12_B;
	}
#endif

	ITEM_LOG_INFO ItemLog;

	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8;
	char AccountName[SIZE_MAX_ACCOUNT_NAME];
	char CharacterName[SIZE_MAX_CHARACTER_NAME];
	SQL_TIMESTAMP_STRUCT sqlTime;
	char LogTypeString[30];
	int ItemNum;
	char ItemName[SIZE_MAX_ITEM_NAME];
	int ItemUID;
	char Contents[200];

	DBGOUT(szQuery);
	
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	if (!m_pODBCStmt->ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_TABLE_NAME, 0, (LPSTR)(LPCSTR)szTableName, 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SZQUERY, 0, (LPSTR)(LPCSTR)szQuery, 0,			&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0010));
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		return FALSE;
	}

	cb1 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, AccountName, SIZE_MAX_ACCOUNT_NAME, &cb1);
	cb2 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, CharacterName, SIZE_MAX_CHARACTER_NAME, &cb2);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_TIMESTAMP, &sqlTime, 0, &cb3);
	cb4 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_CHAR, LogTypeString, 30, &cb4);
	cb5 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_LONG, &ItemNum, 0, &cb5);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_CHAR, ItemName, SIZE_MAX_ITEM_NAME, &cb6);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_LONG, &ItemUID, 0, &cb7);
	cb7 = SQL_NTS;
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_CHAR, Contents, 200, &cb8);

	while (SQLFetch(m_pODBCStmt->m_hstmt) != SQL_NO_DATA)
	{
		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
		//ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(ItemNum);
		ITEM *pItemInfo = m_pmapItemInfo->findEZ_ptr(ItemNum);
		if(pItemInfo)
		{// 2006-03-27 by cmkwon, 아이템 이름 현지화 처리
			util::strncpy(ItemName, pItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
		}
		ItemLog.AccountName = AccountName;
		ItemLog.CharacterName = CharacterName;
		ItemLog.Time = sqlTime;
		ItemLog.LogType = LogTypeString;
		ItemLog.ItemString = ItemName;
		ItemLog.ItemUID = ItemUID;
		ItemLog.Contents = Contents;

		m_GridItemLog.InsertGridItemLog(&ItemLog);

		int nRowCount = m_GridItemLog.GetRowCount() - 1;
		if (nRowCount >= max(100, (m_bCheckMaxRowCount?m_nMaxRowCount:SIZE_MAX_LOG_ROW_COUNT)))
		{
			MessageBox(STRERR_S_SCADMINTOOL_0012);
			m_GridItemLog.ArrangeGrid();
			m_pODBCStmt->FreeStatement();
			return FALSE;
		}
	}

	m_GridItemLog.ArrangeGrid();
	m_pODBCStmt->FreeStatement();
	return TRUE;
}

void CLogTabItem::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	m_GridItemLog.InitGrid(NUM_COLUMNS_ITEM_LOG_GRID, g_arrItemLogColumnNames);
	ResetVariables();
	ResetControls();
}

void CLogTabItem::OnCheckAccountName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_ACCOUNT_NAME)->EnableWindow(m_bCheckAccountName);
}

void CLogTabItem::OnCheckCharacter() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bCheckCharacterName);
	GetDlgItem(IDC_CHECK_CHARACTER2)->EnableWindow(m_bCheckCharacterName);
}

void CLogTabItem::OnCheckDate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_DATETIMEPICKER_START_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_DATE)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_START_TIME)->EnableWindow(m_bCheckDate);
	GetDlgItem(IDC_DATETIMEPICKER_END_TIME)->EnableWindow(m_bCheckDate);
}

void CLogTabItem::OnCheckMaxRowCount() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (!m_bCheckMaxRowCount)
	{
		m_nMaxRowCount = SIZE_MAX_LOG_ROW_COUNT;
		UpdateData(FALSE);
	}
	else
	{
		m_nMaxRowCount = 100;
		UpdateData(FALSE);
	}

	GetDlgItem(IDC_EDIT_MAX_ROW_COUNT)->EnableWindow(m_bCheckMaxRowCount);
}

void CLogTabItem::OnCheckAccountName2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_bCheckAccountName)
	{
		GetDlgItem(IDC_EDIT_ACCOUNT_NAME2)->EnableWindow(m_bCheckAccountName2);
	}
	else
	{
		m_bCheckAccountName2 = FALSE;
		UpdateData(FALSE);
	}
}

void CLogTabItem::OnCheckCharacter2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_bCheckCharacterName)
	{
		GetDlgItem(IDC_EDIT_CHARACTER_NAME2)->EnableWindow(m_bCheckCharacterName2);
	}
	else
	{
		m_bCheckCharacterName2 = FALSE;
		UpdateData(FALSE);
	}
}

void CLogTabItem::OnCheckTradeRecv() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	OnCheckCharacter();
}

void CLogTabItem::OnCheckTradeSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	OnCheckCharacter();
}

void CLogTabItem::OnButtonSelectAll() 
{
	UpdateData(TRUE);

	m_bCheckAddItemByAdmin = TRUE;
	m_bCheckAddItemByCommand = TRUE;
	m_bCheckBuyItem = TRUE;
	m_bCheckCharacterItem = TRUE;
	m_bCheckDelItemByAdmin = TRUE;
	m_bCheckDrawFromStore = TRUE;
	m_bCheckGetItem = TRUE;
	m_bCheckInsertToStore = TRUE;
	m_bCheckStoreStartItem = TRUE;
	m_bCheckSellItem = TRUE;
	m_bCheckThrowAwayItem = TRUE;
	m_bCheckTradeRecv = TRUE;
	m_bCheckTradeSend = TRUE;
	m_bCheckUseEnergy = TRUE;
	m_bCheckUseEnchant = TRUE;
	m_bCheckUseItem = TRUE;
	m_bCheckBazaarSell = TRUE;
	m_bCheckBazaarBuy  = TRUE;

	m_ctlbCheckGiftSend		= TRUE;		// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	m_ctlbCheckGiftRecv		= TRUE;		// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	m_ctlbCheckRearrangeItemDeleted		= TRUE;		// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	m_ctlbCheckRearrangeItemMain		= TRUE;		// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	UpdateData(FALSE);

	// 나머지 설정
	OnCheckTradeRecv();
	OnCheckTradeSend();
}

void CLogTabItem::OnButtonDiselectAll() 
{
	UpdateData(TRUE);

	m_bCheckAddItemByAdmin = FALSE;
	m_bCheckAddItemByCommand = FALSE;
	m_bCheckBuyItem = FALSE;
	m_bCheckCharacterItem = FALSE;
	m_bCheckDelItemByAdmin = FALSE;
	m_bCheckDrawFromStore = FALSE;
	m_bCheckGetItem = FALSE;
	m_bCheckInsertToStore = FALSE;
	m_bCheckStoreStartItem = FALSE;
	m_bCheckSellItem = FALSE;
	m_bCheckThrowAwayItem = FALSE;
	m_bCheckTradeRecv = FALSE;
	m_bCheckTradeSend = FALSE;
	m_bCheckUseEnergy = FALSE;
	m_bCheckUseEnchant = FALSE;
	m_bCheckUseItem = FALSE;
	m_bCheckBazaarSell = FALSE;
	m_bCheckBazaarBuy  = FALSE;

	m_ctlbCheckGiftSend		= FALSE;		// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	m_ctlbCheckGiftRecv		= FALSE;		// 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
	m_ctlbCheckRearrangeItemDeleted		= FALSE;		// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	m_ctlbCheckRearrangeItemMain		= FALSE;		// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 
	UpdateData(FALSE);

	// 나머지 설정
	OnCheckTradeRecv();
	OnCheckTradeSend();
}

void CLogTabItem::OnButtonSelectItem() 
{
	// TODO: Add your control notification handler code here
	// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
	//CSelectItemDlg dlg(&m_mapItemInfo, &m_mapItemInfoName, FALSE);
	CSelectItemDlg dlg(m_pmapItemInfo, m_pmapItemInfoName, FALSE);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	m_nSelectedItemNum = dlg.m_nSelectedItemNum;
	m_szSelectedItemName = dlg.m_szSelectedItemName;

	UpdateData(FALSE);
}

void CLogTabItem::OnCheckItemNum() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_SELECTED_ITEM)->EnableWindow(m_bCheckItemName);

	if (m_bCheckItemName)
	{
		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
		//CSelectItemDlg dlg(&m_mapItemInfo, &m_mapItemInfoName, FALSE);
		CSelectItemDlg dlg(m_pmapItemInfo, m_pmapItemInfoName, FALSE);
		if (IDOK != dlg.DoModal())
		{
			return;
		}

		m_nSelectedItemNum = dlg.m_nSelectedItemNum;
		m_szSelectedItemName = dlg.m_szSelectedItemName;

		UpdateData(FALSE);
	}
}

void CLogTabItem::OnCheckItemUid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_EDIT_ITEM_UID)->EnableWindow(m_bCheckItemUID);
}

void CLogTabItem::OnBtnSaveResult() 
{
	// TODO: Add your control notification handler code here
	
	int nRowCount = m_GridItemLog.GetRowCount();
	if(1 == nRowCount)
	{
		MessageBox("No data !!");
		return;
	}

	CSystemLogManager resultLog;
	if(FALSE == resultLog.InitLogManger(TRUE, "itemLog", "./resultLog/"))
	{
		return;
	}

	for(int i=0; i < nRowCount; i++)
	{
		char szResult[2048];
		util::zero(szResult, 2048);
		sprintf(szResult, "%s;%s;%s;%s;%s;%s;%s\r\n",
			m_GridItemLog.GetItemText(i, 0), m_GridItemLog.GetItemText(i, 1), m_GridItemLog.GetItemText(i, 2),
			m_GridItemLog.GetItemText(i, 3), m_GridItemLog.GetItemText(i, 4), m_GridItemLog.GetItemText(i, 5),
			m_GridItemLog.GetItemText(i, 6));
		resultLog.WriteSystemLog(szResult, FALSE);
	}
	MessageBox("Save success !!");
}

void CLogTabItem::OnCHECKBazaarSell() 
{//2006-8-02, dhjin
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_CHECK_CHARACTER2)->EnableWindow(m_bCheckBazaarSell || m_bCheckBazaarBuy);
	GetDlgItem(IDC_EDIT_CHARACTER_NAME2)->EnableWindow((m_bCheckBazaarSell || m_bCheckBazaarBuy) && m_bCheckCharacterName2);

	OnCheckCharacter();	
}

void CLogTabItem::OnCHECKBazaarBuy() 
{//2006-8-02, dhjin
	// TODO: Add your control notification handler code here
	UpdateData();

	GetDlgItem(IDC_CHECK_CHARACTER2)->EnableWindow(m_bCheckBazaarSell || m_bCheckBazaarBuy);
	GetDlgItem(IDC_EDIT_CHARACTER_NAME2)->EnableWindow((m_bCheckBazaarSell || m_bCheckBazaarBuy) && m_bCheckCharacterName2);

	OnCheckCharacter();		
}
