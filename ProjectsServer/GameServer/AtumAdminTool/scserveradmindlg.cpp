// scserveradmindlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "scserveradmindlg.h"
#include "atumprotocol.h"
#include "AtumAdminToolDlg.h"
#include "WavePlayer.h"
#include "MGameMonitor.h"
#include "setlimitusercountdlg.h"		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _STRMSG_S_SCAT_0000 "Group Server"
#define _STRMSG_S_SCAT_0001 "Field Server"
#define _STRMSG_S_SCAT_0002 "No Excute"
#define _STRMSG_S_SCAT_0003 "Normal"
#define _STRMSG_S_SCAT_0004 "Abnormal"


CSCServerAdminTicker::CSCServerAdminTicker(DWORD i_nTickInterval, CSCServerAdminDlg *i_pSCServerAdminDlg)
: CTickManager(i_nTickInterval)
{
	m_pSCServerAdminDlg = i_pSCServerAdminDlg;
}

void CSCServerAdminTicker::DoEveryTickWork(ATUM_DATE_TIME *pDateTime)
{
	m_pSCServerAdminDlg->OnButtonRefresh();
//	m_pSCServerAdminDlg->PrintOutput("%s: refreshing\r\n", pDateTime->GetDateTimeString(STRNBUF(SIZE_MAX_ATUM_DATE_TIME_STRING)));
}

/////////////////////////////////////////////////////////////////////////////
// CSCServerAdminDlg dialog


CSCServerAdminDlg::CSCServerAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCServerAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCServerAdminDlg)
	m_szOutputText = _T("");
	m_ctlStringTotalUserCounts = _T("");
	//}}AFX_DATA_INIT

	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
	m_pServerAdminPreSocket		= NULL;
	m_mapServerAdminIMSocket.clear();
	m_pServerAdminTicker		= NULL;
	m_ctlStringTotalUserCounts	= "0";

	m_pMGameMonitor				= NULL;
	m_bErrorStateFlag			= FALSE;
	m_dwTickLastRecvErrorState	= 0;
}

CSCServerAdminDlg::~CSCServerAdminDlg()
{
	CWavePlayer::StopSound();

	if (m_pServerAdminTicker != NULL)
	{
		m_pServerAdminTicker->CleanThread();
		util::del(m_pServerAdminTicker);
	}

	util::del(m_pMGameMonitor);
}

void CSCServerAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCServerAdminDlg)
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_ctlEditOutput);
	DDX_Control(pDX, IDC_COMBO_CHAT_HISTORY, m_HistoryComboChat);
	DDX_Control(pDX, IDC_COMBO1, m_ComboServerList);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_szOutputText);
	DDX_Text(pDX, IDC_STATIC_TOTAL_USER_COUNT, m_ctlStringTotalUserCounts);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_SERVER_INFO, m_GridServerInfo);
}


BEGIN_MESSAGE_MAP(CSCServerAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CSCServerAdminDlg)
	ON_BN_CLICKED(IDC_BUTTON_SEND_CHAT, OnButtonSendChat)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_COMMAND(ID_CMG_SERVER_GET_ACCOUNT_LIST, OnCmgServerGetAccountList)
	ON_BN_CLICKED(IDC_BUTTON_STOP_SOUND, OnButtonStopSound)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_INIT_TEXT, OnBtnInitText)
	ON_COMMAND(ID_CMG_CHANGE_LOCK_CREATE_CHARACTER_FOR_NEW_ACCOUNT, OnCmgChangeLockCreateCharacterForNewAccount)
	ON_COMMAND(ID_CMG_SET_LIMIT_USER_COUNT, OnCmgSetLimitUserCount)
	ON_COMMAND(ID_CMG_SERVER_RELOAD_INFLUENCE_RATE, OnCmgServerReloadInfluenceRate)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_RCLICK, IDC_GRID_SERVER_INFO, OnGridRClick)
	ON_MESSAGE(WM_PRE_PACKET_NOTIFY, OnSocketNotifyPre)
	ON_MESSAGE(WM_PRE_ASYNC_EVENT, OnAsyncSocketMessagePre)
	ON_MESSAGE(WM_IM_PACKET_NOTIFY, OnSocketNotifyIM)
	ON_MESSAGE(WM_IM_ASYNC_EVENT, OnAsyncSocketMessageIM)
	ON_MESSAGE(WM_FIELD_PACKET_NOTIFY, OnSocketNotifyField)
	ON_MESSAGE(WM_FIELD_ASYNC_EVENT, OnAsyncSocketMessageField)
	ON_MESSAGE(WM_ASYNC_EVENT_MGAME_MONITOR, OnAsyncSocketMessageMGameMonitor)
	ON_MESSAGE(WM_PACKET_NOTIFY_MGAME_MONITOR, OnSocketNotifyMGameMonitor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCServerAdminDlg message handlers

BOOL CSCServerAdminDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
	GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_CHAT_HISTORY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEND_CHAT)->EnableWindow(FALSE);	
#endif
	
	// TODO: Add extra initialization here
	InitGridServerInfo();

	// Make socket instance & connect
	m_pServerAdminPreSocket= new CSCAdminPreWinSocket("CSCServerAdminDlg's PreServer Socket", this, GetSafeHwnd());
	if (!m_pServerAdminPreSocket->Connect(m_pMainDlg->m_pServerInfo4Admin->ServerIP, PRE_SERVER_PORT))
	{
		int err = GetLastError();
		MessageBox(STRERR_S_SCADMINTOOL_0005);
		util::del(m_pServerAdminPreSocket);
		EndDialog(-1);
		return FALSE;
	}

	m_pServerAdminTicker = new CSCServerAdminTicker(30000, this);
	m_pServerAdminTicker->InitThread();

	m_HistoryComboChat.InitHistoryComboBox("", "", FALSE);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0008);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0007);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0006);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0005);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0004);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0003);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0002);
	m_HistoryComboChat.InsertStringAndUpdateList(STRMSG_SCAT_051115_0001);
	m_HistoryComboChat.SetWindowText("");

	m_ComboServerList.AddString("All ServerGroup");
	m_ComboServerList.AddString("--------------------------");
	m_ComboServerList.SetCurSel(0);

	m_pMGameMonitor = new CMGameMonitor(GetSafeHwnd(), WM_ASYNC_EVENT_MGAME_MONITOR, WM_PACKET_NOTIFY_MGAME_MONITOR, PORT_MGAME_MONITOR_LISTENER);
	m_pMGameMonitor->CloseSocket();
	m_pMGameMonitor->InitMGameMonitor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSCServerAdminDlg::InitGridServerInfo()
{
	m_GridServerInfo.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
//	int m_nCols = 5;
	int m_nCols = 7;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

	m_GridServerInfo.SetEditable(FALSE);
	m_GridServerInfo.SetListMode(TRUE);
	m_GridServerInfo.SetSingleRowSelection(TRUE);
	m_GridServerInfo.EnableSelection(TRUE);
	m_GridServerInfo.SetFrameFocusCell(FALSE);
	m_GridServerInfo.SetTrackFocusCell(FALSE);

	m_GridServerInfo.SetRowCount(m_nRows);
	m_GridServerInfo.SetColumnCount(m_nCols);
	m_GridServerInfo.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format(_T("ServerGroup Name"));
	m_GridServerInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Kind of Server"));
	m_GridServerInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Server IP and Port"));
	m_GridServerInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Current User Counts"));
	m_GridServerInfo.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Server State"));
	m_GridServerInfo.SetItem(&Item);

	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("Limite User Count"));
	m_GridServerInfo.SetItem(&Item);

	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("Lock create Character for New Account"));
	m_GridServerInfo.SetItem(&Item);

	// arrange grid
	m_GridServerInfo.AutoSize();
	m_GridServerInfo.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridServerInfo.GetRowCount()-1, m_GridServerInfo.GetColumnCount()-1);
	m_GridServerInfo.ClearCells(tmpCellRange);
}

LONG CSCServerAdminDlg::OnAsyncSocketMessagePre(WPARAM wParam, LPARAM lParam)
{
	m_pServerAdminPreSocket->OnAsyncEvent(lParam);

	return 0;
}

LONG CSCServerAdminDlg::OnAsyncSocketMessageIM(WPARAM wParam, LPARAM lParam)
{
	CSCAdminIMWinSocket *pSocket = NULL;

	ez_map<string, CSCAdminIMWinSocket*>::iterator itr = m_mapServerAdminIMSocket.begin();
	while (m_mapServerAdminIMSocket.end() != itr)
	{
		pSocket = itr->second;

		if (pSocket->GetSocketHandle() == wParam)
		{
			break;
		}
		itr++;
	}

	if (pSocket != NULL)
	{
		pSocket->OnAsyncEvent(lParam);
	}

	return 0;
}

LONG CSCServerAdminDlg::OnAsyncSocketMessageField(WPARAM wParam, LPARAM lParam)
{
	CSCAdminFieldWinSocket *pSocket = NULL;

	vector<CSCAdminFieldWinSocket*>::iterator itr = m_vectorServerAdminFieldSocket.begin();
	while (m_vectorServerAdminFieldSocket.end() != itr)
	{
		pSocket = *itr;

		if (pSocket->GetSocketHandle() == wParam)
		{
			break;
		}
		itr++;
	}

	if (pSocket != NULL)
	{
		pSocket->OnAsyncEvent(lParam);
	}

	return 0;
}

LONG CSCServerAdminDlg::OnSocketNotifyPre(WPARAM wParam, LPARAM lParam)
{
	CSCAdminPreWinSocket *pSCAdminPreWinSocket = (CSCAdminPreWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// 연결 성공
//				MessageBox("Pre Server에 연결하였습니다.");
				INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_CONNECT, T_PA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				util::strncpy(msgConnect->UID, m_pMainDlg->m_UID, SIZE_MAX_ACCOUNT_NAME);
				util::strncpy(msgConnect->PWD, m_pMainDlg->m_PWD, SIZE_MAX_PASSWORD);
				pSCAdminPreWinSocket->Write(msgConnectBuf, MSG_SIZE(MSG_PA_ADMIN_CONNECT));
			}
			else
			{
				// 연결 실패
				MessageBox(STRERR_S_SCADMINTOOL_0006);
				OnClose();
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			pSCAdminPreWinSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				ProcessPreServerProtocols(pSCAdminPreWinSocket, msgType, pPacket+SIZE_FIELD_TYPE_HEADER);
			}

			util::del(pPacket);
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			pSCAdminPreWinSocket->OnWS_Closed();
		}
		break;
	}// end_switch

	return 0;
}

LONG CSCServerAdminDlg::OnSocketNotifyIM(WPARAM wParam, LPARAM lParam)
{
	CSCAdminIMWinSocket *pSCAdminIMWinSocket = (CSCAdminIMWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// 연결 성공
				//MessageBox("서버에 연결하였습니다.");
				INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_CONNECT, T_IA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				util::strncpy(msgConnect->UID, m_pMainDlg->m_UID, SIZE_MAX_ACCOUNT_NAME);
				util::strncpy(msgConnect->PWD, m_pMainDlg->m_PWD, SIZE_MAX_PASSWORD);
				pSCAdminIMWinSocket->Write(msgConnectBuf, MSG_SIZE(MSG_IA_ADMIN_CONNECT));
			}
			else
			{
				// 연결 실패
				//MessageBox(STRERR_S_SCADMINTOOL_0006);
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			pSCAdminIMWinSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				ProcessIMServerProtocols(pSCAdminIMWinSocket, msgType, pPacket+SIZE_FIELD_TYPE_HEADER);
			}

			util::del(pPacket);
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			pSCAdminIMWinSocket->OnWS_Closed();
		}
		break;
	}	// end of switch

	return 0;
}

LONG CSCServerAdminDlg::OnSocketNotifyField(WPARAM wParam, LPARAM lParam)
{
	CSCAdminFieldWinSocket *pSCAdminFieldWinSocket = (CSCAdminFieldWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// 연결 성공
				//MessageBox("서버에 연결하였습니다.");
				INIT_MSG_WITH_BUFFER(MSG_FA_ADMIN_CONNECT, T_FA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				util::strncpy(msgConnect->UID, m_pMainDlg->m_UID, SIZE_MAX_ACCOUNT_NAME);
				util::strncpy(msgConnect->PWD, m_pMainDlg->m_PWD, SIZE_MAX_PASSWORD);
				pSCAdminFieldWinSocket->Write(msgConnectBuf, MSG_SIZE(MSG_FA_ADMIN_CONNECT));
			}
			else
			{
				// 연결 실패
				//MessageBox(STRERR_S_SCADMINTOOL_0006);
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			pSCAdminFieldWinSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				ProcessFieldServerProtocols(pSCAdminFieldWinSocket, msgType, pPacket+SIZE_FIELD_TYPE_HEADER);
			}

			util::del(pPacket);
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			pSCAdminFieldWinSocket->OnWS_Closed();
		}
		break;
	}	// end of switch

	return 0;
}

// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
//void CSCServerAdminDlg::InsertServerInfoToGrid(MEX_SERVER_INFO_FOR_ADMIN *i_pServerInfo, int i_nRow)
// {
// 	// 칼럼 만들기
// 	int m_nCols = 0;
// 
// 	GV_ITEM Item;
// 	Item.mask = GVIF_TEXT|GVIF_FORMAT;
// 	Item.row = i_nRow;
// 	Item.nFormat = GRID_CELL_FORMAT;
// 	if (i_pServerInfo->ServerType == ST_IM_SERVER)
// 	{
// 		Item.crBkClr = RGB(0xFF, 0xFF, 0xE0);
// 		Item.mask |= (GVIF_BKCLR);
// 	}
// 	else
// 	{
// 		Item.crBkClr = RGB(0xFF, 0xFF, 0xFF);
// 		Item.mask |= (GVIF_BKCLR);
// 	}
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%s", i_pServerInfo->ServerName);
// 	if (i_pServerInfo->ServerType == ST_IM_SERVER)
// 	{
// 		m_GridServerInfo.SetItem(&Item);
// 	}
// 
// 	Item.col = m_nCols++;
// 	if (i_pServerInfo->ServerType == ST_IM_SERVER)
// 	{
// 		Item.strText.Format(_STRMSG_S_SCAT_0000);
// 	}
// 	else if (i_pServerInfo->ServerType == ST_FIELD_SERVER)
// 	{
// 		Item.strText.Format(_STRMSG_S_SCAT_0001);
// 	}
// 	else
// 	{
// 		Item.strText.Format("Unknown Server");
// 	}
// 	m_GridServerInfo.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	Item.strText.Format("%s", i_pServerInfo->ServerID.GetString(string()));
// 	m_GridServerInfo.SetItem(&Item);
// 
// 	Item.col = m_nCols++;
// 	if (i_pServerInfo->ServerType == ST_IM_SERVER)
// 	{
// 		CSCAdminIMWinSocket *pIMSocket
// 			= m_mapServerAdminIMSocket.findEZ(i_pServerInfo->ServerName);
// 		if (pIMSocket != NULL)
// 		{
// 			Item.strText.Format("%d/%d", pIMSocket->m_UserCount, i_pServerInfo->UserCount);
// 		}
// 		else
// 		{
// 			Item.strText.Format("0/%d", i_pServerInfo->UserCount);
// 		}
// 		m_GridServerInfo.SetItem(&Item);
// 	}
// 
// 	Item.col = m_nCols++;
// 	if (i_pServerInfo->ServerState == SERVER_STATE_NOT_ACTIVE) {	Item.strText = _STRMSG_S_SCAT_0002; }
// 	else if (i_pServerInfo->ServerState == SERVER_STATE_NORMAL) {	Item.strText = _STRMSG_S_SCAT_0003; }
// 	else if (i_pServerInfo->ServerState == SERVER_STATE_ABNORMAL) {	Item.strText = _STRMSG_S_SCAT_0004; }
// 	m_GridServerInfo.SetItem(&Item);
// }
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCServerAdminDlg::InsertServerInfoToGrid(MEX_SERVER_GROUP_INFO_FOR_ADMIN *i_pServGInfo, int i_nIMServerRowIdx)
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 위의 함수를 아래와 같이 변경함
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCServerAdminDlg::InsertServerInfoToGrid(MEX_SERVER_GROUP_INFO_FOR_ADMIN *i_pServGInfo, int i_nIMServerRowIdx)
{
	CGridCtrl	*pGridCtrl = &m_GridServerInfo;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-30 by cmkwon, IMServer 정보 추가
	int m_nCols		= 0;

	GV_ITEM Item;
	Item.mask		= GVIF_TEXT | GVIF_FORMAT | GVIF_BKCLR;
	Item.row		= i_nIMServerRowIdx;
	Item.nFormat	= GRID_CELL_FORMAT;
	Item.crBkClr	= RGB(0xFF, 0xFF, 0xE0);

	Item.col		= m_nCols++;
	Item.strText.Format("%s", i_pServGInfo->ServerName);
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	Item.strText.Format(_STRMSG_S_SCAT_0000);
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	Item.strText.Format("%s", i_pServGInfo->IMServerID.GetString().GetBuffer());
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	CSCAdminIMWinSocket *pIMSocket = m_mapServerAdminIMSocket.findEZ(i_pServGInfo->ServerName);
	if (pIMSocket != NULL)
	{
		Item.strText.Format("%d/%d", pIMSocket->m_UserCount, i_pServGInfo->UserCount);
	}
	else
	{
		Item.strText.Format("0/%d", i_pServGInfo->UserCount);
	}
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	switch(i_pServGInfo->IMServerState)
	{
	case SERVER_STATE_NOT_ACTIVE:	Item.strText = _STRMSG_S_SCAT_0002;		break;
	case SERVER_STATE_NORMAL:		Item.strText = _STRMSG_S_SCAT_0003;		break;
	case SERVER_STATE_ABNORMAL:		Item.strText = _STRMSG_S_SCAT_0004;		break;
	default:						Item.strText = "UnknownState";
	}
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	Item.strText.Format("%d", i_pServGInfo->LimitUserCount);
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	Item.strText	= "FALSE";
	if(i_pServGInfo->LockCreateCharacterForNewAccount)
	{
		Item.strText	= "TRUE";
	}
	pGridCtrl->SetItem(&Item);


	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-30 by cmkwon, FieldServer
	m_nCols			= 0;

	Item.row		= i_nIMServerRowIdx + 1;
	Item.mask		= GVIF_TEXT | GVIF_FORMAT;
	Item.crBkClr	= RGB(0xFF, 0xFF, 0xFF);

	Item.col		= m_nCols++;
	
	Item.col		= m_nCols++;
	Item.strText.Format(_STRMSG_S_SCAT_0001);
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;
	Item.strText.Format("%s", i_pServGInfo->FieldServerID.GetString().GetBuffer());
	pGridCtrl->SetItem(&Item);

	Item.col		= m_nCols++;

	Item.col		= m_nCols++;
	switch(i_pServGInfo->FieldServerState)
	{
	case SERVER_STATE_NOT_ACTIVE:	Item.strText = _STRMSG_S_SCAT_0002;		break;
	case SERVER_STATE_NORMAL:		Item.strText = _STRMSG_S_SCAT_0003;		break;
	case SERVER_STATE_ABNORMAL:		Item.strText = _STRMSG_S_SCAT_0004;		break;
	default:						Item.strText = "UnknownState";
	}
	pGridCtrl->SetItem(&Item);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCServerAdminDlg::UpdateServerGroupInfoInGrid(SERVER_INFO_ADMIN_TOOL *i_pServInfoAdmin, int i_nIMServerRowIdx)
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - CSCServerAdminDlg::UpdateServerGroupInfoInGrid() 추가
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCServerAdminDlg::UpdateServerGroupInfoInGrid(SERVER_INFO_ADMIN_TOOL *i_pServInfoAdmin, int i_nIMServerRowIdx)
{
	CGridCtrl	*pGridCtrl = &m_GridServerInfo;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-30 by cmkwon, ServerGroup 접보 Update
	int m_nCols		= 5;
	
	GV_ITEM Item;
	Item.mask		= GVIF_TEXT | GVIF_FORMAT | GVIF_BKCLR;
	Item.row		= i_nIMServerRowIdx;
	Item.nFormat	= GRID_CELL_FORMAT;
	Item.crBkClr	= RGB(0xFF, 0xFF, 0xE0);

	Item.col		= m_nCols++;
	Item.strText.Format("%d", i_pServInfoAdmin->LimitUserCount);
	pGridCtrl->SetItem(&Item);
	
	Item.col		= m_nCols++;
	Item.strText	= "FALSE";
	if(i_pServInfoAdmin->LockCreateCharacterForNewAccount)
	{
		Item.strText	= "TRUE";
	}
	pGridCtrl->SetItem(&Item);
}


void CSCServerAdminDlg::OnButtonSendChat() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CString szServer;
	m_ComboServerList.GetLBText(m_ComboServerList.GetCurSel(), szServer);

	CString szChatMessage = m_HistoryComboChat.GetSelectedString();
	szChatMessage.TrimRight(); szChatMessage.TrimLeft();

	if (szChatMessage == "")
	{
		MessageBox(STRERR_S_SCADMINTOOL_0042);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 사용 불가 명령어 체크
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		char szCmd[1024];
		char seps[] = " ";
		util::strncpy(szCmd, szChatMessage, 1024);
		char *token = strtok(szCmd, seps);

		if(0 == stricmp(token, STRCMD_CS_COMMAND_USERSINFOTOTAL)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_USERSINFOTOTAL_1)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_WHO)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_WHO_1)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_WHOAREYOU)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_WHOAREYOU_1)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_SERVERDOWN)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_SERVERDOWN_1)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_0)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_1)
			|| 0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_2)
			)
		{
			AfxMessageBox("You are not have permission !!");
			m_HistoryComboChat.SetWindowText("");
			return;
		}
	}

	if (szServer == "All ServerGroup")
	{
		for (int i = 0; i < m_GridServerInfo.GetRowCount(); i++)
		{
			if (m_GridServerInfo.GetItemText(i, 4) == _STRMSG_S_SCAT_0003 && m_GridServerInfo.GetItemText(i, 1) == _STRMSG_S_SCAT_0000)
			{
				if (SendChatToServerGroup(m_GridServerInfo.GetItemText(i, 0), szChatMessage))
					
					PrintOutput(STRMSG_S_SCADMINTOOL_0044, GET_CURRENT_DATE_TIME_STRING(), m_GridServerInfo.GetItemText(i, 0), szChatMessage);
			}
		}
	}
	else
	{
		if (SendChatToServerGroup(szServer, szChatMessage))
		{
			PrintOutput(STRMSG_S_SCADMINTOOL_0044, GET_CURRENT_DATE_TIME_STRING(), (LPCSTR)szServer, szChatMessage);			
		}
		else
		{
			MessageBox(STRERR_S_SCADMINTOOL_0043);
		}
	}

	m_HistoryComboChat.InsertStringAndUpdateList(szChatMessage);
	m_HistoryComboChat.SetWindowText("");
}

BOOL CSCServerAdminDlg::SendChatToServerGroup(const char *i_szServerGroupName, const char *i_szChatMsg)
{
	int nLen = strlen(i_szChatMsg);
	if(512 < nLen)
	{
		MessageBox("Chat message length overflow !!");
		return FALSE;
	}
	SERVER_INFO_ADMIN_TOOL servInfo = GetServerGroupInfo(i_szServerGroupName);
	CSCAdminIMWinSocket *pIMSocket = m_mapServerAdminIMSocket.findEZ(i_szServerGroupName);

	if (servInfo.ServerState == SERVER_STATE_NORMAL
		&& (pIMSocket != NULL && pIMSocket->IsConnected()))
	{
		char szTemp[1024];		util::zero(szTemp, 1024);
		strcpy(szTemp, i_szChatMsg);

		INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_CHAT_ALL, T_IA_ADMIN_CHAT_ALL, msgChatAll, msgChatAllBuf);
		util::strncpy(msgChatAll->FromCharacterName, STRMSG_S_SCAT_COLNAME_0094, SIZE_MAX_CHAT_MESSAGE);
		int nStartIdx = 0;
		while(TRUE)
		{
			int nDelimiterIdx = GDelimiterIndex(&szTemp[nStartIdx], SIZE_MAX_CHAT_MESSAGE-1);
			if(0 == nDelimiterIdx)
			{
				break;
			}

			msgChatAll->MessageLength = min(nDelimiterIdx, SIZE_MAX_CHAT_MESSAGE-1) + 1;
			memcpy((char*)msgChatAllBuf + MSG_SIZE(MSG_IA_ADMIN_CHAT_ALL), &szTemp[nStartIdx], msgChatAll->MessageLength-1);
			*(((char*)msgChatAllBuf) + MSG_SIZE(MSG_IA_ADMIN_CHAT_ALL) + msgChatAll->MessageLength-1) = '\0';
			pIMSocket->Write(msgChatAllBuf, MSG_SIZE(MSG_IA_ADMIN_CHAT_ALL) + msgChatAll->MessageLength);

			nStartIdx += nDelimiterIdx;
			if(nLen <= nStartIdx)
			{
				break;
			}
		}
		
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

void CSCServerAdminDlg::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	
	///////////////////////////////////////////////////////////////////////////////
	// 서버 에러 상태를 체크한다
	this->CheckServerErrorState();
	
	///////////////////////////////////////////////////////////////////////////////
	// PreServer로 UserCount 정보를 요청한다
	m_pServerAdminPreSocket->WriteMessageType(T_PA_ADMIN_GET_SERVER_INFO);

	///////////////////////////////////////////////////////////////////////////////
	// IMServer로 UserCount 정보를 요청한다
	ez_map<string, CSCAdminIMWinSocket*>::iterator itr = m_mapServerAdminIMSocket.begin();
	while (m_mapServerAdminIMSocket.end() != itr)
	{
		CSCAdminIMWinSocket *pSocket = itr->second;
		pSocket->WriteMessageType(T_IA_ADMIN_GET_SERVER_INFO);
		itr++;
	}
}

SERVER_INFO_ADMIN_TOOL CSCServerAdminDlg::GetServerGroupInfo(const char *i_szServerGroupName)
{
	SERVER_INFO_ADMIN_TOOL ret;

	for (int i = 0; i < m_GridServerInfo.GetRowCount(); i++)
	{
		if (m_GridServerInfo.GetItemText(i, 0) == i_szServerGroupName)
		{
			ret.ServerName = m_GridServerInfo.GetItemText(i, 0);
			if (m_GridServerInfo.GetItemText(i, 1) == _STRMSG_S_SCAT_0000) { ret.ServerType = ST_IM_SERVER; }
			else if (m_GridServerInfo.GetItemText(i, 1) == _STRMSG_S_SCAT_0001) { ret.ServerType = ST_FIELD_SERVER; }
			else { m_GridServerInfo.GetItemText(i, 1); ret.ServerType = ST_INVALID_TYPE; }
			ret.ServerID.SetValue(m_GridServerInfo.GetItemText(i, 2));
			ret.UserCount = atoi((LPCSTR)m_GridServerInfo.GetItemText(i, 3));
			
			CString szServerState = m_GridServerInfo.GetItemText(i, 4);
			if (szServerState == _STRMSG_S_SCAT_0002) { ret.ServerState = SERVER_STATE_NOT_ACTIVE; }
			else if (szServerState == _STRMSG_S_SCAT_0003) { ret.ServerState = SERVER_STATE_NORMAL; }
			else if (szServerState == _STRMSG_S_SCAT_0004) { ret.ServerState = SERVER_STATE_ABNORMAL; }

			ret.LimitUserCount			= atoi(m_GridServerInfo.GetItemText(i, 5));		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
			CString strLockCreateChar	= m_GridServerInfo.GetItemText(i, 6);
			ret.LockCreateCharacterForNewAccount		= FALSE;					// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
			if(0 != strLockCreateChar.CompareNoCase("FALSE"))
			{
				ret.LockCreateCharacterForNewAccount	= TRUE;
			}
			ret.RowCountInGrid = i;

			return ret;
		}
	}

	return ret;
}

SERVER_INFO_ADMIN_TOOL CSCServerAdminDlg::GetServerInfoByServerID(SERVER_ID i_ServerID)
{
	SERVER_INFO_ADMIN_TOOL ret;

	for (int i = 1; i < m_GridServerInfo.GetRowCount(); i++)
	{
		if (SERVER_ID(m_GridServerInfo.GetItemText(i, 2)) == i_ServerID)
		{
			ret.ServerName = m_GridServerInfo.GetItemText(i, 0);
			if (m_GridServerInfo.GetItemText(i, 1) == _STRMSG_S_SCAT_0000) { ret.ServerType = ST_IM_SERVER; }
			else if (m_GridServerInfo.GetItemText(i, 1) == _STRMSG_S_SCAT_0001) { ret.ServerType = ST_FIELD_SERVER; }
			else { m_GridServerInfo.GetItemText(i, 1); ret.ServerType = ST_INVALID_TYPE; }
			ret.ServerID.SetValue(m_GridServerInfo.GetItemText(i, 2));
			ret.UserCount = atoi((LPCSTR)m_GridServerInfo.GetItemText(i, 3));
			CString szServerState = m_GridServerInfo.GetItemText(i, 4);
			if (szServerState == _STRMSG_S_SCAT_0002) { ret.ServerState = SERVER_STATE_NOT_ACTIVE; }
			else if (szServerState == _STRMSG_S_SCAT_0003) { ret.ServerState = SERVER_STATE_NORMAL; }
			else if (szServerState == _STRMSG_S_SCAT_0004) { ret.ServerState = SERVER_STATE_ABNORMAL; }
			ret.RowCountInGrid = i;

			return ret;
		}
	}

	return ret;
}

CSCAdminFieldWinSocket* CSCServerAdminDlg::GetFieldWinSocket(SERVER_ID i_ServerID)
{
	CSCAdminFieldWinSocket *pSocket = NULL;

	vector<CSCAdminFieldWinSocket*>::iterator itr = m_vectorServerAdminFieldSocket.begin();
	while (m_vectorServerAdminFieldSocket.end() != itr)
	{
		pSocket = *itr;

		if (pSocket->m_ServerID == i_ServerID)
		{
			return pSocket;
		}

		itr++;
	}

	return NULL;
}

void CSCServerAdminDlg::OnCmgServerGetAccountList() 
{
#ifdef _ATUM_ADMIN_RELEASE
	return;
#endif// _ATUM_ADMIN_RELEASE_end_ifdef

	// TODO: Add your command handler code here
	CCellRange cellRange = m_GridServerInfo.GetSelectedCellRange();
	CString szServerGroupName = m_GridServerInfo.GetItemText(cellRange.GetMinRow(), 0);
//	CSCAdminIMWinSocket *pSocket = m_mapServerAdminIMSocket.findEZ((LPCSTR)szServerGroupName);
//	if (pSocket == NULL)
//	{
//		return;
//	}

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_ACCOUNT_LIST, T_PA_ADMIN_GET_ACCOUNT_LIST, msgGetList, msgGetListBuf);
	util::strncpy(msgGetList->ServerName, szServerGroupName, SIZE_MAX_SERVER_NAME);
	m_pServerAdminPreSocket->Write(msgGetListBuf, MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_LIST));
}

void CSCServerAdminDlg::OnGridRClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	UpdateData();

    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (pItem->iRow == 0)
	{
		return;
	}

	m_GridServerInfo.SetFocusCell(-1,-1);
	m_GridServerInfo.SetSelectedRange(pItem->iRow, 0, pItem->iRow, m_GridServerInfo.GetColumnCount()-1, TRUE, TRUE);

	if (m_GridServerInfo.GetItemText(pItem->iRow, 1) != _STRMSG_S_SCAT_0000)
	{
		return;
	}

	// show context menu
	CMenu muTemp, *pContextMenu;
	muTemp.LoadMenu(IDR_CONTEXT_MENU_GRID_SERVER);
	CPoint point;
	GetCursorPos(&point);
	pContextMenu = muTemp.GetSubMenu(0);
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CSCServerAdminDlg::ProcessPreServerProtocols(CSCAdminPreWinSocket *i_pPreWinSocket, MessageType_t i_nMsgType, char *i_pMsg)
{
	switch(i_nMsgType)
	{
	case T_PA_ADMIN_CONNECT_OK:
		{
			MSG_PA_ADMIN_CONNECT_OK *msgConnectOK
				= (MSG_PA_ADMIN_CONNECT_OK*)i_pMsg;

// 2006-04-15 by cmkwon
//			if (!msgConnectOK->AuthOK)
			if (0 == msgConnectOK->AccountType0)
			{
				MessageBox("Fail PreServer3 certification!!");
				OnCancel();
			}

			i_pPreWinSocket->WriteMessageType(T_PA_ADMIN_GET_SERVER_INFO);
		}
		break;
	case T_PA_ADMIN_CHANGE_SERVER_STATE:
		{
			MSG_PA_ADMIN_CHANGE_SERVER_STATE *msgChangeState
				= (MSG_PA_ADMIN_CHANGE_SERVER_STATE*)i_pMsg;

			SERVER_INFO_ADMIN_TOOL ServInfo = GetServerInfoByServerID(msgChangeState->ServerID);
			if (ServInfo.ServerType != ST_INVALID_TYPE)
			{
				// check: 처리하기, sound 등....
			
				SetGridRowColor(ServInfo.RowCountInGrid, RGB(200, 0, 0));
				m_GridServerInfo.AutoSize();
				m_GridServerInfo.ExpandColumnsToFit();

				PrintOutput(STRERR_S_SCADMINTOOL_0044, GET_CURRENT_DATE_TIME_STRING(), msgChangeState->ServerState);

				this->CheckServerErrorState(TRUE);
			}
			else
			{
// 2006-05-21 by cmkwon, 아래와 같이 수정함				
// 				CString szMsg = "Invalid MSG_PA_ADMIN_CHANGE_SERVER_STATE\r\n";
// 				szMsg += msgChangeState->ServerID.GetString(string());
// 				MessageBox(szMsg);
				PrintOutput("%s: server state(%d), ServerInfo(%s)\r\n", GET_CURRENT_DATE_TIME_STRING(), msgChangeState->ServerState
					, msgChangeState->ServerID.GetString().GetBuffer());
			}
		}
		break;
	case T_PA_ADMIN_GET_SERVER_INFO_OK:
		{
// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 아래와 같이 수정함.
//			MSG_PA_ADMIN_GET_SERVER_INFO_OK *msgGetServerInfoOK
//				= (MSG_PA_ADMIN_GET_SERVER_INFO_OK*)i_pMsg;
//
// 			int nLoginedTotal = 0;
// 			int nGameStartedTotal = 0;
// 			MEX_SERVER_INFO_FOR_ADMIN *pServerInfo = (MEX_SERVER_INFO_FOR_ADMIN *)(i_pMsg+sizeof(MSG_PA_ADMIN_GET_SERVER_INFO_OK));
// 			for (int i = 0; i < msgGetServerInfoOK->NumOfServers; i++)
// 			{
// 				pServerInfo = (MEX_SERVER_INFO_FOR_ADMIN*)(i_pMsg+sizeof(MSG_PA_ADMIN_GET_SERVER_INFO_OK)+i*sizeof(MEX_SERVER_INFO_FOR_ADMIN));
// 				m_GridServerInfo.SetRowCount(i+2);
// #ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
// 				pServerInfo->UserCount = min(pServerInfo->UserCount, 10);
// #endif
// 				// 2007-07-05 by cmkwon, 관리자만 동접 확인 가능하게 수정 - 운영자는 10명 까지만 표시
// 				if(FALSE == m_pMainDlg->IsManagerAdministrator())
// 				{
// 					pServerInfo->UserCount = min(pServerInfo->UserCount, 10);
// 				}
// 				InsertServerInfoToGrid(pServerInfo, i+1);
// 
// 				if (pServerInfo->ServerType == ST_IM_SERVER)
// 				{// IM Server
// 					
// 					if(CB_ERR == m_ComboServerList.FindString(0, pServerInfo->ServerName))
// 					{
// 						m_ComboServerList.AddString(pServerInfo->ServerName);
// 					}
// 					
// 					if (pServerInfo->ServerState == SERVER_STATE_NORMAL)
// 					{
// 						nLoginedTotal += pServerInfo->UserCount;
// 						CSCAdminIMWinSocket *pSocket = m_mapServerAdminIMSocket.findEZ((LPCSTR)pServerInfo->ServerName);
// 						if(pSocket == NULL)
// 						{
// 							// connect and insert
// 							pSocket = new CSCAdminIMWinSocket("CSCServerAdminDlg's IMServer Socket", pServerInfo->ServerName, this, GetSafeHwnd());
// 							if (!pSocket->Connect(pServerInfo->ServerID.IP, pServerInfo->ServerID.port))
// 							{
// 								int err = GetLastError();
// 								MessageBox(STRERR_S_SCADMINTOOL_0045);
// 								util::del(pSocket);
// 							}
// 							m_mapServerAdminIMSocket.insertEZ(pServerInfo->ServerName, pSocket);
// 						}
// 						else
// 						{
// #ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
// 							pSocket->m_UserCount = min(pSocket->m_UserCount, 10);
// #endif
// 							// 2007-07-05 by cmkwon, 관리자만 동접 확인 가능하게 수정 - 운영자는 10명 까지만 표시
// 							if(FALSE == m_pMainDlg->IsManagerAdministrator())
// 							{
// 								pSocket->m_UserCount = min(pSocket->m_UserCount, 10);
// 							}
// 							nGameStartedTotal+= pSocket->m_UserCount;
// 						}
// 					}
// 				}			
// 
// 				if (pServerInfo->ServerType == ST_FIELD_SERVER
// 					&& pServerInfo->ServerState == SERVER_STATE_NORMAL)
// 				{// Field Server
// 
// 					CSCAdminFieldWinSocket *pFieldSocket = GetFieldWinSocket(pServerInfo->ServerID);
// 					if(pFieldSocket == NULL)
// 					{
// 						// connect and insert
// 						pFieldSocket = new CSCAdminFieldWinSocket("CSCServerAdminDlg's FieldServer Socket", pServerInfo->ServerName, this, GetSafeHwnd());
// 						pFieldSocket->m_ServerID = pServerInfo->ServerID;
// 						if (!pFieldSocket->Connect(pServerInfo->ServerID.IP, pServerInfo->ServerID.port))
// 						{
// 							int err = GetLastError();
// 							MessageBox(STRERR_S_SCADMINTOOL_0046);
// 							util::del(pFieldSocket);
// 						}
// 						m_vectorServerAdminFieldSocket.push_back(pFieldSocket);
// 					}
// 				}
// 			}
// #ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
// 			nGameStartedTotal = min(nGameStartedTotal, 10);
// 			nLoginedTotal = min(nLoginedTotal, 10);
// #endif
// 			// 2007-07-05 by cmkwon, 관리자만 동접 확인 가능하게 수정 - 운영자는 10명 까지만 표시
// 			if(FALSE == m_pMainDlg->IsManagerAdministrator())
// 			{
// 				nGameStartedTotal = min(nGameStartedTotal, 10);
// 				nLoginedTotal = min(nLoginedTotal, 10);
// 			}
// 			m_ctlStringTotalUserCounts.Format("%5d/%5d", nGameStartedTotal, nLoginedTotal);
// 			GetDlgItem(IDC_STATIC_TOTAL_USER_COUNT)->SetWindowText(m_ctlStringTotalUserCounts);
// //			UpdateData(FALSE);
// 
// 			// arrange grid
//			m_GridServerInfo.AutoSize();
// 			m_GridServerInfo.ExpandColumnsToFit();
			///////////////////////////////////////////////////////////////////////////////
			// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
			MSG_PA_ADMIN_GET_SERVER_INFO_OK *msgGetServerInfoOK = (MSG_PA_ADMIN_GET_SERVER_INFO_OK*)i_pMsg;
			for (int i = 0; i < msgGetServerInfoOK->NumOfServerGroups; i++)
			{
				MEX_SERVER_GROUP_INFO_FOR_ADMIN *pServGInfo = (MEX_SERVER_GROUP_INFO_FOR_ADMIN*)(i_pMsg + sizeof(MSG_PA_ADMIN_GET_SERVER_INFO_OK) + i*sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN));
				// 2007-07-05 by cmkwon, 관리자만 동접 확인 가능하게 수정 - 운영자는 10명 까지만 표시
				if(FALSE == m_pMainDlg->IsManagerAdministrator())
				{
					pServGInfo->UserCount = min(pServGInfo->UserCount, 10);
				}
				
				SERVER_INFO_ADMIN_TOOL	tmServInfo = this->GetServerGroupInfo(pServGInfo->ServerName);
				int						nCurRowCnt = m_GridServerInfo.GetRowCount();
				if(FALSE == util::in_range(0, tmServInfo.RowCountInGrid, nCurRowCnt-1))
				{// 2008-04-30 by cmkwon, ServerGroup 정보가 없다. 행을 추가한다.

					m_GridServerInfo.SetRowCount(nCurRowCnt + 2);
					InsertServerInfoToGrid(pServGInfo, nCurRowCnt);
				}
				else
				{// 2008-04-30 by cmkwon, ServerGroup 정보가 이미 존재하고 있다, 업데이트로 처리

					InsertServerInfoToGrid(pServGInfo, tmServInfo.RowCountInGrid);
				}
		
				if(CB_ERR == m_ComboServerList.FindString(0, pServGInfo->ServerName))
				{
					m_ComboServerList.AddString(pServGInfo->ServerName);
				}
				
				if (pServGInfo->IMServerState == SERVER_STATE_NORMAL)
				{
					CSCAdminIMWinSocket *pSocket = m_mapServerAdminIMSocket.findEZ((LPCSTR)pServGInfo->ServerName);
					if(pSocket == NULL)
					{
						// connect and insert
						pSocket = new CSCAdminIMWinSocket("CSCServerAdminDlg's IMServer Socket", pServGInfo->ServerName, this, GetSafeHwnd());
						if (!pSocket->Connect(pServGInfo->IMServerID.IP, pServGInfo->IMServerID.port))
						{
							int err = GetLastError();
							MessageBox(STRERR_S_SCADMINTOOL_0045);
							util::del(pSocket);
						}
						m_mapServerAdminIMSocket.insertEZ(pServGInfo->ServerName, pSocket);
					}
					else
					{
						// 2007-07-05 by cmkwon, 관리자만 동접 확인 가능하게 수정 - 운영자는 10명 까지만 표시
						if(FALSE == m_pMainDlg->IsManagerAdministrator())
						{
							pSocket->m_UserCount = min(pSocket->m_UserCount, 10);
						}
					}
				}

				if (pServGInfo->FieldServerState == SERVER_STATE_NORMAL)
				{// Field Server

					CSCAdminFieldWinSocket *pFieldSocket = GetFieldWinSocket(pServGInfo->FieldServerID);
					if(pFieldSocket == NULL)
					{
						// connect and insert
						pFieldSocket = new CSCAdminFieldWinSocket("CSCServerAdminDlg's FieldServer Socket", pServGInfo->ServerName, this, GetSafeHwnd());
						pFieldSocket->m_ServerID = pServGInfo->FieldServerID;
						if (!pFieldSocket->Connect(pServGInfo->FieldServerID.IP, pServGInfo->FieldServerID.port))
						{
							int err = GetLastError();
							MessageBox(STRERR_S_SCADMINTOOL_0046);
							util::del(pFieldSocket);
						}
						m_vectorServerAdminFieldSocket.push_back(pFieldSocket);
					}
				}
			}

			int nLoginedTotal		= this->GetTotalLoginedUserCount();
			int nGameStartedTotal	= this->GetTotalGameStartedUserCount();
			m_ctlStringTotalUserCounts.Format("%5d/%5d", nGameStartedTotal, nLoginedTotal);
			GetDlgItem(IDC_STATIC_TOTAL_USER_COUNT)->SetWindowText(m_ctlStringTotalUserCounts);

			// arrange grid
			m_GridServerInfo.AutoSize();
			m_GridServerInfo.ExpandColumnsToFit();

		}
		break;
	case T_PA_ADMIN_GET_ACCOUNT_LIST_OK:
		{
			MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK *msgGetAccountListOK
				= (MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK*)i_pMsg;

			PrintOutput(STRMSG_S_SCADMINTOOL_0045, msgGetAccountListOK->NumOfAccounts);
			for (int i = 0; i < msgGetAccountListOK->NumOfAccounts; i++)
			{
				MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN* pAccountInfo
					= (MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN*)(((char*)msgGetAccountListOK) + sizeof(MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK)
						+ i*sizeof(MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN));
				PrintOutput("%s\r\n", pAccountInfo->AccountName);
			}
		}
		break;
	case T_PA_ADMIN_SET_DBSERVER_GROUP_OK:		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
		{
			MSG_PA_ADMIN_SET_DBSERVER_GROUP_OK *pRMsg = (MSG_PA_ADMIN_SET_DBSERVER_GROUP_OK*)i_pMsg;

			SERVER_INFO_ADMIN_TOOL tmServInfoAdmin = this->GetServerGroupInfo(pRMsg->ServerGroupName);
			if(0 <= tmServInfoAdmin.RowCountInGrid)
			{// 2008-04-30 by cmkwon, ServerGroupName 으로 정보를 가져온다.

				tmServInfoAdmin.LimitUserCount						= pRMsg->LimitUserCount;
				tmServInfoAdmin.LockCreateCharacterForNewAccount	= pRMsg->LockCreateCharacterForNewAccount;
				
				// 2008-04-30 by cmkwon, Grid Update
				this->UpdateServerGroupInfoInGrid(&tmServInfoAdmin, tmServInfoAdmin.RowCountInGrid);
			}
		}
		break;
	default:
		{
		}
		break;
	}	// end - switch
}

void CSCServerAdminDlg::ProcessIMServerProtocols(CSCAdminIMWinSocket *i_pIMWinSocket, MessageType_t i_nMsgType, char *i_pMsg)
{
	switch(i_nMsgType)
	{
	case T_IA_ADMIN_CONNECT_OK:
		{
			MSG_IA_ADMIN_CONNECT_OK *msgConnectOK
				= (MSG_IA_ADMIN_CONNECT_OK*)i_pMsg;

// 2006-04-15 by cmkwon
//			if (!msgConnectOK->AuthOK)
			if (0 == msgConnectOK->AccountType0)
			{
				MessageBox("Fail IMServer certification!!");
				OnCancel();
			}
		}
		break;
	case T_IA_ADMIN_CHANGE_SERVER_STATE:
		{
			MSG_IA_ADMIN_CHANGE_SERVER_STATE *msgChangeState
				= (MSG_IA_ADMIN_CHANGE_SERVER_STATE*)i_pMsg;

			SERVER_INFO_ADMIN_TOOL ServInfo = GetServerInfoByServerID(msgChangeState->ServerID);
			if (ServInfo.ServerType != ST_INVALID_TYPE)
			{
				// check: 처리하기, sound 등....

				SetGridRowColor(ServInfo.RowCountInGrid, RGB(200, 0, 0));
				m_GridServerInfo.AutoSize();
				m_GridServerInfo.ExpandColumnsToFit();

				PrintOutput(STRERR_S_SCADMINTOOL_0047
					, GET_CURRENT_DATE_TIME_STRING()
					, i_pIMWinSocket->m_szServerGroupName1
					, msgChangeState->ServerState);

				this->CheckServerErrorState(TRUE);
			}
			else
			{
// 2006-05-29 by cmkwon
// 				CString szMsg = "Invalid MSG_IA_ADMIN_CHANGE_SERVER_STATE\r\n";
// 				szMsg += msgChangeState->ServerID.GetString(string());
// 				MessageBox(szMsg);
				PrintOutput("%s: server state(%d), ServerInfo(%s)\r\n", GET_CURRENT_DATE_TIME_STRING(),
					msgChangeState->ServerState, msgChangeState->ServerID.GetString().GetBuffer());
			}
		}
		break;
	case T_IC_STRING_128:
		{
			MSG_IC_STRING_128 *msgString128
				= (MSG_IC_STRING_128*)i_pMsg;

			PrintOutput(STRMSG_S_SCADMINTOOL_0046
				, GET_CURRENT_DATE_TIME_STRING()
				, i_pIMWinSocket->m_szServerGroupName1
				, msgString128->String);
		}
		break;
	case T_IA_ADMIN_GET_SERVER_INFO_OK:
		{
			MSG_IA_ADMIN_GET_SERVER_INFO_OK *pMsgServerInfoOK
				= (MSG_IA_ADMIN_GET_SERVER_INFO_OK*)i_pMsg;

#ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
			pMsgServerInfoOK->UserCount = min(pMsgServerInfoOK->UserCount, 10);
#endif
			// 2007-07-05 by cmkwon, 관리자만 동접 확인 가능하게 수정 - 운영자는 10명 까지만 표시
			if(FALSE == m_pMainDlg->IsManagerAdministrator())
			{
				pMsgServerInfoOK->UserCount = min(pMsgServerInfoOK->UserCount, 10);
			}
			i_pIMWinSocket->m_UserCount = pMsgServerInfoOK->UserCount;

			PrintOutput(STRMSG_S_SCADMINTOOL_0047
				, GET_CURRENT_DATE_TIME_STRING()
				, i_pIMWinSocket->m_szServerGroupName1
				, i_pIMWinSocket->m_UserCount);
		}
		break;
	default:
		{
		}
		break;
	}
}

void CSCServerAdminDlg::ProcessFieldServerProtocols(CSCAdminFieldWinSocket *i_pFieldWinSocket, MessageType_t i_nMsgType, char *i_pMsg)
{
	switch(i_nMsgType)
	{
	case T_FA_ADMIN_CONNECT_OK:
		{
			MSG_FA_ADMIN_CONNECT_OK *msgConnectOK
				= (MSG_FA_ADMIN_CONNECT_OK*)i_pMsg;

// 2006-04-15 by cmkwon
//			if (!msgConnectOK->AuthOK)
			if (0 == msgConnectOK->AccountType0)
			{
				MessageBox("Fail FieldServer certification!!");
				OnCancel();
			}
		}
		break;
	case T_FA_ADMIN_CHANGE_SERVER_STATE:
		{
			MSG_FA_ADMIN_CHANGE_SERVER_STATE *msgChangeState
				= (MSG_FA_ADMIN_CHANGE_SERVER_STATE*)i_pMsg;

			SERVER_INFO_ADMIN_TOOL ServInfo = GetServerInfoByServerID(msgChangeState->ServerID);
			if (ServInfo.ServerType != ST_INVALID_TYPE)
			{
				// check: 처리하기, sound 등....

				SetGridRowColor(ServInfo.RowCountInGrid, RGB(200, 0, 0));
				m_GridServerInfo.AutoSize();
				m_GridServerInfo.ExpandColumnsToFit();

				PrintOutput(STRMSG_S_SCADMINTOOL_0048
					, GET_CURRENT_DATE_TIME_STRING()
					, i_pFieldWinSocket->m_szServerGroupName1
					, msgChangeState->ServerState);

				this->CheckServerErrorState(TRUE);
			}
			else
			{
// 2006-05-29 by cmkwon
// 				CString szMsg = "Invalid MSG_FA_ADMIN_CHANGE_SERVER_STATE\r\n";
// 				szMsg += msgChangeState->ServerID.GetString(string());
// 				MessageBox(szMsg);
				PrintOutput("%s: server state(%d), ServerInfo(%s)\r\n", GET_CURRENT_DATE_TIME_STRING(), msgChangeState->ServerState
					, msgChangeState->ServerID.GetString().GetBuffer());
			}
		}
		break;
	default:
		{
		}
		break;
	}
}

void CSCServerAdminDlg::OnButtonStopSound() 
{
	// TODO: Add your control notification handler code here
	CWavePlayer::StopSound();
}

void CSCServerAdminDlg::SetGridRowColor(INT rowCount, COLORREF clr)
{
	for (int i = 0; i < m_GridServerInfo.GetColumnCount(); i++)
	{
		m_GridServerInfo.SetItemBkColour(rowCount, i, clr);
	}
}

void CSCServerAdminDlg::PrintOutput(char* pFormat, ...)
{
	if(m_szOutputText.GetLength() > 20000)
	{
		m_szOutputText.Empty();
	}
	char buff2[1024];

	va_list args;
	va_start(args, pFormat);
	vsprintf(buff2, pFormat, args);
	va_end(args);

	m_szOutputText += buff2;
	UpdateData(FALSE);

	m_ctlEditOutput.LineScroll(m_ctlEditOutput.GetLineCount(), 0);
	//GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText(m_szOutputText);

	//((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT))->SetSel(m_szOutputText.GetLength(), m_szOutputText.GetLength(), TRUE);
}

void CSCServerAdminDlg::ResetOutput()
{
	m_szOutputText = "";
	UpdateData(FALSE);
}

void CSCServerAdminDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if (m_pServerAdminTicker != NULL)
	{
		m_pServerAdminTicker->CleanThread();
		util::del(m_pServerAdminTicker);
	}
	
	CDialog::OnCancel();
}


void CSCServerAdminDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}


void CSCServerAdminDlg::OnTimerForSendAlivePacket(void)
{
	if(m_pServerAdminPreSocket
		&& m_pServerAdminPreSocket->IsConnected())
	{
		m_pServerAdminPreSocket->WriteMessageType(T_PM_CONNECT_ALIVE);
	}

	this->SendAlive2AllIMServer();
	this->SendAlive2AllFieldServer();
}

void CSCServerAdminDlg::SendAlive2AllIMServer(void)
{
	ez_map<string, CSCAdminIMWinSocket*>::iterator itr = m_mapServerAdminIMSocket.begin();
	while (m_mapServerAdminIMSocket.end() != itr)
	{
		CSCAdminIMWinSocket *pSocket = itr->second;
		pSocket->WriteMessageType(T_IC_CONNECT_ALIVE);
		itr++;
	}
}
void CSCServerAdminDlg::SendAlive2AllFieldServer(void)
{
	CSCAdminFieldWinSocket *pSocket = NULL;
	vector<CSCAdminFieldWinSocket*>::iterator itr = m_vectorServerAdminFieldSocket.begin();
	for(; m_vectorServerAdminFieldSocket.end() != itr; itr++)
	{
		pSocket = *itr;
		if (pSocket
			&& pSocket->IsConnected())
		{
			pSocket->WriteMessageType(T_FC_CONNECT_ALIVE);
		}
	}
}


LONG CSCServerAdminDlg::OnAsyncSocketMessageMGameMonitor(WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_pMGameMonitor)
	{
		return FALSE;
	}

	CMGameMonitor *ptmMGame = m_pMGameMonitor->FindConnectedSocketBySocketHandle(wParam);
	if(NULL == ptmMGame)
	{
		return FALSE;
	}
	
	ptmMGame->OnAsyncEvent(lParam);
	return TRUE;
}

LONG CSCServerAdminDlg::OnSocketNotifyMGameMonitor(WPARAM wParam, LPARAM lParam)
{
	if(NULL == m_pMGameMonitor)
	{
		return 0;
	}

	CMGameMonitor *ptmMGame = (CMGameMonitor*)lParam;
	if(NULL == ptmMGame)
	{
		return FALSE;
	}	

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
		}
		break;
	case CWinSocket::WS_ACCEPTED:	// 2007-06-21 by cmkwon, 클라이언트의 소스와 통합
		{
			ptmMGame->OnWS_Accept(wParam, lParam);
		}
		break;
	case CWinSocket::WS_CLOSED:
		{			
			m_pMGameMonitor->ReleaseConnectedSocket(ptmMGame);
		}
		break;
	}// end_switch(LOWORD(wParam))

	return 0;
}

void CSCServerAdminDlg::CheckServerErrorState(BOOL i_bRecvErrorState/*=FALSE*/)
{
	DBGOUT("0310 - void CSCServerAdminDlg::CheckServerErrorState() RecvErrorState(%d)\r\n", i_bRecvErrorState);

	DWORD dwCurTick = timeGetTime();
	if(i_bRecvErrorState)
	{
		m_bErrorStateFlag			= TRUE;
		m_dwTickLastRecvErrorState	= dwCurTick;
		CWavePlayer::PlaySound(IDR_WAVE_ALERT);
		if(m_pMGameMonitor)
		{
			m_pMGameMonitor->ResetMGameMonitor();
		}
		return;
	}

	if(FALSE == CheckAllSessionState())
	{
		CWavePlayer::PlaySound(IDR_WAVE_ALERT);
		m_pMGameMonitor->ResetMGameMonitor();
		return;
	}

	if(FALSE == m_bErrorStateFlag)
	{
		return;
	}
	
	if(dwCurTick - m_dwTickLastRecvErrorState < 60*1000)
	{
		return;
	}

	
	DBGOUT("0310 - Error state release\r\n");

	m_bErrorStateFlag			= FALSE;
	CWavePlayer::StopSound();

	if(m_pMGameMonitor)
	{
		m_pMGameMonitor->InitMGameMonitor();
	}
}

BOOL CSCServerAdminDlg::CheckAllSessionState(void)
{
	ez_map<string, CSCAdminIMWinSocket*>::iterator itrIM = m_mapServerAdminIMSocket.begin();
	while (m_mapServerAdminIMSocket.end() != itrIM)
	{
		CSCAdminIMWinSocket *pSocket = itrIM->second;
		if(FALSE == pSocket->IsConnected())
		{
			return FALSE;
		}
		itrIM++;
	}

	CSCAdminFieldWinSocket *pSocket = NULL;
	vector<CSCAdminFieldWinSocket*>::iterator itr = m_vectorServerAdminFieldSocket.begin();
	for(; m_vectorServerAdminFieldSocket.end() != itr; itr++)
	{
		pSocket = *itr;
		if (FALSE == pSocket->IsConnected())
		{
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CSCServerAdminDlg::GetTotalLoginedUserCount(void)
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CSCServerAdminDlg::GetTotalLoginedUserCount(void)
{
	int nLoginedUserCnt = 0;
	for (int i = 1; i < m_GridServerInfo.GetRowCount(); i++)
	{
		if ( 0 == (m_GridServerInfo.GetItemText(i, 1)).CompareNoCase(_STRMSG_S_SCAT_0000) )
		{
			CString strUserCnt = m_GridServerInfo.GetItemText(i, 3);
			int nIdx = strUserCnt.Find('/');
			if(0 <= nIdx)
			{
				CString strLoginUserCnt = strUserCnt.Right(strUserCnt.GetLength() - nIdx - 1);
				nLoginedUserCnt += atoi((LPCSTR)strLoginUserCnt);			
			}
		}
	}
	
	return nLoginedUserCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CSCServerAdminDlg::GetTotalGameStartedUserCount(void)
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CSCServerAdminDlg::GetTotalGameStartedUserCount(void)
{
	int nGameStartedUserCnt = 0;
	for (int i = 1; i < m_GridServerInfo.GetRowCount(); i++)
	{
		if ( 0 == (m_GridServerInfo.GetItemText(i, 1)).CompareNoCase(_STRMSG_S_SCAT_0000) )
		{
			nGameStartedUserCnt += atoi((LPCSTR)m_GridServerInfo.GetItemText(i, 3));			
		}
	}
	
	return nGameStartedUserCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CString CSCServerAdminDlg::GetServerGroupNameFromSelectedGridRow(void)
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - CSCServerAdminDlg::GetServerGroupNameFromSelectedGridRow() 추가
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CString CSCServerAdminDlg::GetServerGroupNameFromSelectedGridRow(void)
{
	CCellRange	cellRange			= m_GridServerInfo.GetSelectedCellRange();
	CString		szServerGroupName	= m_GridServerInfo.GetItemText(cellRange.GetMinRow(), 0);
	
	return szServerGroupName;
}


void CSCServerAdminDlg::OnBtnInitText() 
{
	// TODO: Add your control notification handler code here

	m_szOutputText.Empty();
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCServerAdminDlg::OnCmgChangeLockCreateCharacterForNewAccount() 
{
	// TODO: Add your command handler code here
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox("Permission denied !!");
		return;
	}

	if(NULL == m_pServerAdminPreSocket
		|| FALSE == m_pServerAdminPreSocket->IsConnected())
	{// 2008-04-30 by cmkwon, PreServer 연결 체크
		AfxMessageBox("Disconnected socket to PreServer !!");
		return;
	}

	CString strServGName = this->GetServerGroupNameFromSelectedGridRow();
	if(strServGName.IsEmpty())
	{// 2008-04-30 by cmkwon, 선택한 행에서 ServerGroupName 을 가져온다.
		AfxMessageBox("Select valid ServerGroup !!");
		return;
	}

	SERVER_INFO_ADMIN_TOOL tmServInfoAdmin = this->GetServerGroupInfo(strServGName);
	if(0 > tmServInfoAdmin.RowCountInGrid
		|| SERVER_STATE_NOT_ACTIVE == tmServInfoAdmin.ServerState)
	{// 2008-04-30 by cmkwon, ServerGroupName 으로 정보를 가져온다.
		AfxMessageBox("Select valid ServerGroup !!");
		return;
	}

	// 2008-04-30 by cmkwon, 값을 변경한다.
	tmServInfoAdmin.LockCreateCharacterForNewAccount	= !(tmServInfoAdmin.LockCreateCharacterForNewAccount);

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_SET_DBSERVER_GROUP, T_PA_ADMIN_SET_DBSERVER_GROUP, pSMsg, SendBuf);
	util::strncpy(pSMsg->ServerGroupName, strServGName, SIZE_MAX_SERVER_NAME);
	pSMsg->LimitUserCount					= tmServInfoAdmin.LimitUserCount;
	pSMsg->LockCreateCharacterForNewAccount	= tmServInfoAdmin.LockCreateCharacterForNewAccount;	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 변경

	if(NULL == m_pServerAdminPreSocket
		|| FALSE == m_pServerAdminPreSocket->IsConnected())
	{
		return;
	}

	// 2008-04-30 by cmkwon, PreServer 로 전송
	m_pServerAdminPreSocket->Write(SendBuf, MSG_SIZE(MSG_PA_ADMIN_SET_DBSERVER_GROUP));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
/// \author		cmkwon
/// \date		2008-04-30 ~ 2008-04-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCServerAdminDlg::OnCmgSetLimitUserCount() 
{
	// TODO: Add your command handler code here

	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox("Permission denied !!");
		return;
	}

	if(NULL == m_pServerAdminPreSocket
		|| FALSE == m_pServerAdminPreSocket->IsConnected())
	{
		AfxMessageBox("Disconnected socket to PreServer !!");
		return;
	}

	CString strServGName = this->GetServerGroupNameFromSelectedGridRow();
	if(strServGName.IsEmpty())
	{// 2008-04-30 by cmkwon, 선택한 행에서 ServerGroupName 을 가져온다.
		AfxMessageBox("Select valid ServerGroup !!");
		return;
	}
	
	SERVER_INFO_ADMIN_TOOL tmServInfoAdmin = this->GetServerGroupInfo(strServGName);
	if(0 > tmServInfoAdmin.RowCountInGrid
		|| SERVER_STATE_NOT_ACTIVE == tmServInfoAdmin.ServerState)
	{// 2008-04-30 by cmkwon, ServerGroupName 으로 정보를 가져온다.
		AfxMessageBox("Select valid ServerGroup !!");
		return;
	}

	CSetLimitUserCountDlg dlg(tmServInfoAdmin.LimitUserCount, this);
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	// 2008-04-30 by cmkwon, 설정된 값을 가져온다.
	tmServInfoAdmin.LimitUserCount		= dlg.m_nUserCount;

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_SET_DBSERVER_GROUP, T_PA_ADMIN_SET_DBSERVER_GROUP, pSMsg, SendBuf);	
	util::strncpy(pSMsg->ServerGroupName, strServGName, SIZE_MAX_SERVER_NAME);
	pSMsg->LimitUserCount					= tmServInfoAdmin.LimitUserCount;			// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - LimitUserCount 설정
	pSMsg->LockCreateCharacterForNewAccount	= tmServInfoAdmin.LockCreateCharacterForNewAccount;

	if(NULL == m_pServerAdminPreSocket
		|| FALSE == m_pServerAdminPreSocket->IsConnected())
	{
		return;
	}

	// 2008-04-30 by cmkwon, PreServer 로 전송
	m_pServerAdminPreSocket->Write(SendBuf, MSG_SIZE(MSG_PA_ADMIN_SET_DBSERVER_GROUP));
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
void CSCServerAdminDlg::OnCmgServerReloadInfluenceRate() 
{
	// TODO: Add your command handler code here
	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{
		AfxMessageBox("Permission denied !!");
		return;
	}
	
	if(NULL == m_pServerAdminPreSocket
		|| FALSE == m_pServerAdminPreSocket->IsConnected())
	{// 2008-04-30 by cmkwon, PreServer 연결 체크
		AfxMessageBox("Disconnected socket to PreServer !!");
		return;
	}
	
	CString strServGName = this->GetServerGroupNameFromSelectedGridRow();
	if(strServGName.IsEmpty())
	{// 2008-04-30 by cmkwon, 선택한 행에서 ServerGroupName 을 가져온다.
		AfxMessageBox("Select valid ServerGroup !!");
		return;
	}
	
	SERVER_INFO_ADMIN_TOOL tmServInfoAdmin = this->GetServerGroupInfo(strServGName);
	if(0 > tmServInfoAdmin.RowCountInGrid
		|| SERVER_STATE_NOT_ACTIVE == tmServInfoAdmin.ServerState)
	{// 2008-04-30 by cmkwon, ServerGroupName 으로 정보를 가져온다.
		AfxMessageBox("Select valid ServerGroup !!");
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_RELOAD_INFLUENCERATE, T_PA_ADMIN_RELOAD_INFLUENCERATE, pSMsg, SendBuf);	
	util::strncpy(pSMsg->ServerGroupName, strServGName, SIZE_MAX_SERVER_NAME);
	if(NULL == m_pServerAdminPreSocket
		|| FALSE == m_pServerAdminPreSocket->IsConnected())
	{
		return;
	}
	
	// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 
	m_pServerAdminPreSocket->Write(SendBuf, MSG_SIZE(MSG_PA_ADMIN_RELOAD_INFLUENCERATE));
}
