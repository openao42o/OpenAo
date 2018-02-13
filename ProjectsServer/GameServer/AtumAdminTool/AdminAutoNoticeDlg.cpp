// AdminAutoNoticeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "AdminAutoNoticeDlg.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminAutoNoticeDlg dialog


CAdminAutoNoticeDlg::CAdminAutoNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdminAutoNoticeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdminAutoNoticeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
}


void CAdminAutoNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminAutoNoticeDlg)
	DDX_Control(pDX, IDC_ADMIN_AUTO_NOTICE_LIST, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminAutoNoticeDlg, CDialog)
	//{{AFX_MSG_MAP(CAdminAutoNoticeDlg)
	ON_BN_CLICKED(IDC_NOTICE_ADD, OnNoticeAdd)
	ON_BN_CLICKED(IDC_NOTICE_DELETE, OnNoticeDelete)
	ON_BN_CLICKED(IDC_NOTICE_UP, OnNoticeUp)
	ON_BN_CLICKED(IDC_NOTICE_DOWN, OnNoticeDown)
	ON_BN_CLICKED(IDC_BTN_GAMESERVER_RELOAD, OnBtnGameServerReload)
	ON_BN_CLICKED(IDC_BTN_UPDATE_DB, OnBtnUpdateDb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminAutoNoticeDlg message handlers

BOOL CAdminAutoNoticeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Connect DB
	if ( FALSE == m_odbcStmt1.Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
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

	SANoticeInfo		tmNoticeInfo;
	vectSANoticeString	vectNoticeStrList;
	util::zero(&tmNoticeInfo, sizeof(tmNoticeInfo));

	if(FALSE == this->DBQueryLoadAdminNoticeInfo(&tmNoticeInfo)
		|| FALSE == this->DBQueryLoadNoticeStringList(&vectNoticeStrList))
	{
		MessageBox("Can not load AdminAutoNotice from DB !!");
		EndDialog(-1);
		return FALSE;
	}
	m_ANoticeInfo1			= tmNoticeInfo;

	// 2009-01-19 by cmkwon, 정렬
	sort(vectNoticeStrList.begin(), vectNoticeStrList.end(), sort_SANoticeString_by_StrIdx());
	m_vectANoticeStrList	= vectNoticeStrList;

	// 2009-01-16 by cmkwon, 리스트 컨트롤 초기화
	InitListControl();

	_UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdminAutoNoticeDlg::InitListControl()
{
	m_wndList.SetGridLines(TRUE);
	m_wndList.SetEditable(TRUE);

	m_wndList.SetColumnEditEnable(0, FALSE);

	// the heading text is in the format of "text,width,format;text,width,format;..."
	m_wndList.SetColumnHeader("Idx, 50;Notice String, 720");
}

void CAdminAutoNoticeDlg::OnNoticeAdd() 
{
	// TODO: Add your control notification handler code here

	int nItemCount = m_wndList.GetItemCount();
	if(nItemCount >= SIZE_MAX_COUNT_ADMIN_NOTICE_STRING)
	{
		AfxMessageBox("Can't insert Notice String !!");
		return;
	}

	m_wndList.InsertItem(nItemCount, "");
	
	char szBuff[MAX_PATH];
	sprintf(szBuff, "%d", nItemCount + 1);
	m_wndList.SetItemText(nItemCount, 0, szBuff);

	sprintf(szBuff, "%s", "Input ...");
	m_wndList.SetItemText(nItemCount, 1, szBuff);
}

void CAdminAutoNoticeDlg::OnNoticeDelete() 
{
	// TODO: Add your control notification handler code here

	int nSelectItem = m_wndList.GetSelectedItem();
	if(nSelectItem == -1)
		return;

	m_wndList.DeleteItem(nSelectItem);

	char szBuff[MAX_PATH];

	for(int i=nSelectItem; i<m_wndList.GetItemCount(); ++i)
	{
		sprintf(szBuff, "%d", i + 1);
		m_wndList.SetItemText(i, 0, szBuff);
	}

	m_wndList.SetItemStates(nSelectItem, RC_ITEM_SELECTED);
	m_wndList.SetFocus();
}

void CAdminAutoNoticeDlg::OnNoticeUp() 
{
	// TODO: Add your control notification handler code here
	int nSelItem = m_wndList.GetSelectedItem();
	if( nSelItem > 0)
	{
		m_wndList.MoveUp(nSelItem);
		
		char szBuff1[MAX_PATH], szBuff2[MAX_PATH];
		
		m_wndList.GetItemText(nSelItem-1, 0, szBuff1, MAX_PATH);
		m_wndList.GetItemText(nSelItem, 0, szBuff2, MAX_PATH);

		m_wndList.SetItemText(nSelItem-1, 0, szBuff2);
		m_wndList.SetItemText(nSelItem, 0, szBuff1);

		m_wndList.SetItemStates(nSelItem-1, RC_ITEM_SELECTED);
		m_wndList.SetFocus();
	}
}

void CAdminAutoNoticeDlg::OnNoticeDown() 
{
	// TODO: Add your control notification handler code here
	int nSelItem = m_wndList.GetSelectedItem();
	if( nSelItem < m_wndList.GetItemCount() - 1)
	{
		m_wndList.MoveDown(nSelItem);

		char szBuff1[MAX_PATH], szBuff2[MAX_PATH];
		
		m_wndList.GetItemText(nSelItem+1, 0, szBuff1, MAX_PATH);
		m_wndList.GetItemText(nSelItem, 0, szBuff2, MAX_PATH);
		
		m_wndList.SetItemText(nSelItem+1, 0, szBuff2);
		m_wndList.SetItemText(nSelItem, 0, szBuff1);

		m_wndList.SetItemStates(nSelItem+1, RC_ITEM_SELECTED);
		m_wndList.SetFocus();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-19 ~ 2009-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAdminAutoNoticeDlg::OnBtnGameServerReload() 
{
	// TODO: Add your control notification handler code here
	if(NULL == m_pMainDlg->m_pAdminPreSocket
		|| FALSE == m_pMainDlg->m_pAdminPreSocket->IsConnected())
	{
		AfxMessageBox("disconnected from PreServer !!");
		return;
	}

	// 2009-01-19 by cmkwon, PreServer 로 전송
	m_pMainDlg->m_pAdminPreSocket->WriteMessageType(T_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM);

	AfxMessageBox("Success send message that GameServer reload AadminAutoNotice Information.");
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-01-16 ~ 2009-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAdminAutoNoticeDlg::DBQueryLoadAdminNoticeInfo(SANoticeInfo *o_pNoticeInfo)
{
	SQLHSTMT hstmt = m_odbcStmt1.GetSTMTHandle();
	BOOL bRet = m_odbcStmt1.ExecuteQuery(PROCEDURE_090115_0280);
	if (!bRet)
	{
		m_odbcStmt1.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryLoadAdminNoticeInfo error !!");
		return FALSE;
	}
	SQLINTEGER arrCB[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindCol(hstmt,  1, SQL_C_LONG, &o_pNoticeInfo->UsingFlag, 0,								&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_LONG, &o_pNoticeInfo->LoopSec, 0,								&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_LONG, &o_pNoticeInfo->IntervalSec, 0,							&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_C_CHAR, o_pNoticeInfo->EditorAccountName, SIZE_MAX_ACCOUNT_NAME,	&arrCB[4]);
	
	SQLFetch(hstmt);
	m_odbcStmt1.FreeStatement();		// cleanup
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-01-16 ~ 2009-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAdminAutoNoticeDlg::DBQueryUpdateAdminNoticeInfo(SANoticeInfo *i_pNoticeInfo)
{
	SQLHSTMT hstmt = m_odbcStmt1.GetSTMTHandle();

	SQLINTEGER arrCB[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pNoticeInfo->UsingFlag, 0,	&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pNoticeInfo->LoopSec, 0,		&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pNoticeInfo->IntervalSec, 0,	&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pNoticeInfo->EditorAccountName, 0, &arrCB[4]);
	BOOL bRet = m_odbcStmt1.ExecuteQuery(PROCEDURE_090115_0282);
	m_odbcStmt1.FreeStatement();		// cleanup		
	if (!bRet)
	{
		AfxMessageBox("DBQueryUpdateAdminNoticeInfo error !!");
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-01-16 ~ 2009-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAdminAutoNoticeDlg::DBQueryLoadNoticeStringList(vectSANoticeString *o_pvectNoticeStrList)
{
	SQLHSTMT hstmt = m_odbcStmt1.GetSTMTHandle();
	BOOL bRet = m_odbcStmt1.ExecuteQuery(PROCEDURE_090115_0281);
	if (!bRet)
	{
		m_odbcStmt1.FreeStatement();		// cleanup		
		AfxMessageBox("DBQueryLoadNoticeStringList error !!");
		return FALSE;
	}	
	SQLINTEGER arrCB[3]={SQL_NTS,SQL_NTS,SQL_NTS};
	SANoticeString tmNoticeStr;
	SQLBindCol(hstmt,  1, SQL_C_LONG, &tmNoticeStr.NoticeStringIndex, 0,								&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, &tmNoticeStr.NoticeString, SIZE_MAX_ADMIN_NOTICE_STRING,			&arrCB[2]);
	
	do
	{
		util::zero(&tmNoticeStr, sizeof(tmNoticeStr));
		SQLRETURN ret = SQLFetch(hstmt);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		o_pvectNoticeStrList->push_back(tmNoticeStr);
	}while(TRUE);
	m_odbcStmt1.FreeStatement();		// cleanup

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-01-16 ~ 2009-01-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAdminAutoNoticeDlg::DBQueryUpdateNoticeStringList(vectSANoticeString *o_pvectNoticeStrList)
{
	SQLHSTMT hstmt = m_odbcStmt1.GetSTMTHandle();

	BOOL bDeleteOldList = TRUE;
	vectSANoticeString::iterator itr(o_pvectNoticeStrList->begin());
	for (; itr != o_pvectNoticeStrList->end(); itr++)
	{
		SANoticeString *pNoticeStr = &*itr;

		SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bDeleteOldList, 0,						&arrCB[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pNoticeStr->NoticeStringIndex, 0,		&arrCB[2]);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ADMIN_NOTICE_STRING, 0, &pNoticeStr->NoticeString, 0, &arrCB[3]);
		BOOL bRet = m_odbcStmt1.ExecuteQuery(PROCEDURE_090115_0283);
		if (!bRet)
		{
			m_odbcStmt1.FreeStatement();		// cleanup		
			AfxMessageBox("DBQueryUpdateAdminNoticeInfo error !!");
			return FALSE;
		}
		m_odbcStmt1.FreeStatement();		// cleanup		

		bDeleteOldList = FALSE;
	}
	return TRUE;
}

void CAdminAutoNoticeDlg::OnBtnUpdateDb() 
{
	// TODO: Add your control notification handler code here
	_UpdateData();

	if(NULL == m_pMainDlg->m_pAdminPreSocket
		|| FALSE == m_pMainDlg->m_pAdminPreSocket->IsConnected())
	{
		AfxMessageBox("disconnected from PreServer !!");
		return;
	}

	this->DBQueryUpdateAdminNoticeInfo(&m_ANoticeInfo1);
	this->DBQueryUpdateNoticeStringList(&m_vectANoticeStrList);

	AfxMessageBox("Success update DB.");
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAdminAutoNoticeDlg::_UpdateData(BOOL i_bSaveAndValidate/*=TRUE*/)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-19 ~ 2009-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAdminAutoNoticeDlg::_UpdateData(BOOL i_bSaveAndValidate/*=TRUE*/)
{
	CString csTemp;
	if(FALSE == i_bSaveAndValidate)
	{
		if(FALSE == m_ANoticeInfo1.UsingFlag)
		{
			GetDlgItem(IDC_RADIO_NO)->SendMessage(BM_SETCHECK, 1, 0);
		}
		else
		{
			GetDlgItem(IDC_RADIO_YES)->SendMessage(BM_SETCHECK, 1, 0);
			
		}
		
		csTemp.Format("%d", m_ANoticeInfo1.LoopSec/60);
		GetDlgItem(IDC_EDIT_LoopTime)->SetWindowText(csTemp);
		
		csTemp.Format("%d", m_ANoticeInfo1.IntervalSec);
		GetDlgItem(IDC_EDIT_IntervalTime)->SetWindowText(csTemp);

		int nCnt = m_wndList.GetItemCount() + 1;
		vectSANoticeString::iterator itr(m_vectANoticeStrList.begin());
		for(;itr != m_vectANoticeStrList.end(); itr++, nCnt++)
		{
			CString csStrIdx;
			csStrIdx.Format("%d", nCnt);
			m_wndList.InsertItemEx(nCnt, csStrIdx, itr->NoticeString);
		}
	}
	else
	{
		m_ANoticeInfo1.UsingFlag		= FALSE;
		if(BST_CHECKED != GetDlgItem(IDC_RADIO_NO)->SendMessage(BM_GETCHECK, 0, 0))
		{
			m_ANoticeInfo1.UsingFlag	= TRUE;
		}

		GetDlgItem(IDC_EDIT_LoopTime)->GetWindowText(csTemp);
		m_ANoticeInfo1.LoopSec = max(MIN_LOOP_SECONDS_OF_ADMIN_NOTICE, atoi(csTemp) * 60);
		m_ANoticeInfo1.LoopSec = min(MAX_LOOP_SECONDS_OF_ADMIN_NOTICE, atoi(csTemp) * 60);
		
		GetDlgItem(IDC_EDIT_IntervalTime)->GetWindowText(csTemp);
		m_ANoticeInfo1.IntervalSec = max(MIN_INTERVAL_SECONDS_OF_ADMIN_NOTICE, atoi(csTemp));
		m_ANoticeInfo1.IntervalSec = min(MAX_INTERVAL_SECONDS_OF_ADMIN_NOTICE, atoi(csTemp));


		m_vectANoticeStrList.clear();
		int nItemCnts = m_wndList.GetItemCount();
		for(int i=0; i < nItemCnts; i++)
		{
	 		char szTab1[MAX_PATH], szTab2[MAX_PATH];
 			m_wndList.GetItemText(i, 0, szTab1, MAX_PATH);
 			m_wndList.GetItemText(i, 1, szTab2, MAX_PATH);

			SANoticeString tmNStr;
			tmNStr.NoticeStringIndex	= atoi(szTab1);
			util::strncpy(tmNStr.NoticeString, szTab2, SIZE_MAX_ADMIN_NOTICE_STRING);
			m_vectANoticeStrList.push_back(tmNStr);
		}
	}
	return UpdateData(i_bSaveAndValidate);
}
