// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "AtumMonitor.h"

#include "MainFrm.h"
#include "AtumMonitorDoc.h"
#include "AtumMonitorView.h"
#include "LeftView.h"
#include "FieldServerDlg.h"
#include "MonitorWinSocket.h"
#include "MonitorNode.h"
#include "PringDBGDlg.h"
#include "DlgNewServerGroup.h"
#include "AtumProtocol.h"
#include "PreUserCountDlg.h"
#include "InfoGenDlg.h"
#include "SQLReaderDlg.h"
#include "AdminWindowDlg.h"
#include <Mmsystem.h>
#include "MyFileDialog.h"
#include "ClientFTPUpdateManagerDlg.h"
#include "AtumError.h"
#include "zipfile/zipfileDlg.h"
#include "SetCityWarTimeDlg.h"
#include "HttpManager.h"					// 2007-01-08 by cmkwon
#include "AtumParam.h"
//#include "ImportExcelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _STRMSG_S_SCMONITOR_0000 "%04d(%2d -  Active) %3d/%3d"
#define _STRMSG_S_SCMONITOR_0001 "%04d(%2d -Inactive) %3d/%3d"
#define _STRMSG_S_SCMONITOR_0002 "%04d(%2d -  Active)"
#define _STRMSG_S_SCMONITOR_0003 "%04d(%2d -Inactive)"





//CGlobalGameServer *		g_pGlobal = NULL;

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_TCM_CONNECT_FIELD, OnTcmConnectField)
	ON_COMMAND(ID_TCM_DISCONNECT_FIELD, OnTcmDisconnectField)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_COMMAND(ID_TCM_NUM_CLIENTS_FIELD, OnTcmNumClientsField)
	ON_COMMAND(ID_TCM_PING_FIELD, OnTcmPingField)
	ON_COMMAND(ID_TCM_SHUTDOWN_FIELD, OnTcmShutdownField)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_TCM_PRINTMAP_FIELD, OnTcmPrintmapField)
	ON_COMMAND(ID_TCM_MAPBLOCKINFO_FIELD, OnTcmMapblockinfoField)
	ON_COMMAND(ID_TCM_CONNECT_NPC, OnTcmConnectNpc)
	ON_COMMAND(ID_TCM_PING_NPC, OnTcmPingNpc)
	ON_COMMAND(ID_TCM_DISCONNECT_NPC, OnTcmDisconnectNpc)
	ON_COMMAND(ID_TCM_MAPBLOCKINFO_NPC, OnTcmMapblockinfoNpc)
	ON_COMMAND(ID_TCM_CONNECT_PRE, OnTcmConnectPre)
	ON_COMMAND(ID_TCM_DISCONNECT_PRE, OnTcmDisconnectPre)
	ON_COMMAND(ID_TCM_NUM_CLIENTS_PRE, OnTcmNumClientsPre)
	ON_COMMAND(ID_TCM_PING_PRE, OnTcmPingPre)
	ON_COMMAND(ID_TCM_RELOAD_VERSION_INFO_PRE, OnTcmReloadVersionInfoPre)
	ON_COMMAND(ID_TCM_PRINT_DBG, OnTcmPrintDbg)
	ON_COMMAND(ID_TCM_CHANGE_BANDWIDTHFLAG_PRE, OnTcmChangeBandwidthflagPre)
	ON_COMMAND(ID_TCM_CHANGE_BANDWIDTHFLAG_FIELD, OnTcmChangeBandwidthflagField)
	ON_COMMAND(ID_TCM_CHANGE_BANDWIDTHFLAG_NPC, OnTcmChangeBandwidthflagNpc)
	ON_COMMAND(ID_TCM_CONNECT_IM, OnTcmConnectIm)
	ON_COMMAND(ID_TCM_DISCONNECT_IM, OnTcmDisconnectIm)
	ON_COMMAND(ID_TCM_NUM_CLIENTS_IM, OnTcmNumClientsIm)
	ON_COMMAND(ID_TCM_PING_IM, OnTcmPingIm)
	ON_COMMAND(ID_TCM_PRINTMAP_NPC, OnTcmPrintmapNpc)
	ON_COMMAND(ID_TCM_SHUTDOWN_IM, OnTcmShutdownIm)
	ON_COMMAND(ID_TCM_SHUTDOWN_NPC, OnTcmShutdownNpc)
	ON_COMMAND(ID_TCM_CHANGE_BANDWIDTHFLAG_IM, OnTcmChangeBandwidthflagIm)
	ON_COMMAND(ID_TCM_PRINT_DBG_IM, OnTcmPrintDbgIm)
	ON_COMMAND(ID_TCM_SET_MSG_PRINT_LEVEL_FIELD, OnTcmSetMsgPrintLevelField)
	ON_COMMAND(ID_TCM_SET_MSG_PRINT_LEVEL_IM, OnTcmSetMsgPrintLevelIm)
	ON_COMMAND(ID_TCM_SET_MSG_PRINT_LEVEL_NPC, OnTcmSetMsgPrintLevelNpc)
	ON_COMMAND(ID_TCM_SET_MSG_PRINT_LEVEL_PRE, OnTcmSetMsgPrintLevelPre)
	ON_COMMAND(ID_TCM_GET_CHARACTER_INFO_FIELD, OnTcmGetCharacterInfoField)
	ON_COMMAND(ID_TCM_PAUSE_PRE, OnTcmPausePre)
	ON_COMMAND(ID_TCM_START_PRE, OnTcmStartPre)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_TCM_RENAME_ROOT, OnTcmRenameRoot)
	ON_COMMAND(ID_TCM_DELETE_ROOT, OnTcmDeleteRoot)
	ON_COMMAND(ID_TCM_GET_USER_COUNTS_IM, OnTcmGetUserCountsIm)
	ON_COMMAND(ID_TCM_GET_USER_COUNTS_FIELD, OnTcmGetUserCountsField)
	ON_COMMAND(ID_TCM_GET_SERVER_GROUP_INFO_PRE, OnTcmGetServerGroupInfoPre)
	ON_COMMAND(ID_TCM_RELOAD_ITEM_INFO_FIELD, OnTcmReloadItemInfoField)
	ON_COMMAND(ID_TCM_RESET_NPC_CONNECTION_FIELD, OnTcmResetNpcConnectionField)
	ON_COMMAND(ID_TCM_PRE_SET_LIMIT_USER_COUNT, OnTcmPreSetLimitUserCount)
	ON_COMMAND(ID_TCM_GET_USER_COUNTS_NPC, OnTcmGetUserCountsNpc)
	ON_COMMAND(ID_TCM_NPC_SET_LIMIT_MONSTER_COUNT, OnTcmNpcSetLimitMonsterCount)
	ON_COMMAND(ID_INFO_GEN, OnInfoGen)
	ON_COMMAND(ID_SQL_READER, OnSqlReader)
	ON_COMMAND(ID_TOOLS_STOP_NOTICE_PLAYSOUND, OnToolsStopNoticePlaysound)
	ON_COMMAND(ID_TOOLS_SET_NOTICE_FILE_PATH, OnToolsSetNoticeFilePath)
	ON_COMMAND(ID_TCM_RELOAD_BLOCKED_ACCOUNT_PRE, OnTcmReloadBlockedAccountPre)
	ON_COMMAND(ID_CLIENT_FTP_UPDATE_MANAGER, OnClientFtpUpdateManager)
	ON_COMMAND(ID_TCM_RELOAD_QUEST_INFO_FIELD, OnTcmReloadQuestInfoField)
	ON_COMMAND(ID_TCM_FIELD_ENABLE_CHANNEL, OnTcmFieldEnableChannel)
	ON_COMMAND(ID_TCM_FIELD_DISABLE_CHANNEL, OnTcmFieldDisableChannel)
	ON_COMMAND(ID_ZIP_FILE_GEN, OnZipFileGen)
	ON_COMMAND(ID_TCM_GET_USER_MONSTER_COUNT, OnTcmGetUserMonsterCount)
	ON_COMMAND(ID_TCM_SET_MGAME_EVENT, OnTcmSetMgameEvent)
	ON_COMMAND(ID_TCM_PRE_CHANGE_ENABLE_SERVER_GROUP, OnTcmPreChangeEnableServerGroup)
	ON_COMMAND(ID_TCM_FIELD_CITYWAR_CHANGE_TIME, OnTcmFieldCitywarChangeTime)
	ON_COMMAND(ID_TCM_PREPARE_SHUTDOWN, OnTcmPrepareShutdown)
	ON_COMMAND(ID_TCM3_PREPARE_SHUTDOWN, OnTcm3PrepareShutdown)
	//ON_COMMAND(ID_IMPORT_EXCELFILE, OnImportExcelfile)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
	ON_MESSAGE(WM_PACKET_NOTIFY, OnSocketNotify)
	ON_MESSAGE(WM_ASYNC_EVENT, OnAsyncSocketMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

	m_uiAliveTimer = 0;
	m_bNoticePlaySound = FALSE;
}

CLeftView::~CLeftView()
{	
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |
				TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CAtumMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


/////////////////////////////////////////////////////////////////////////////
// CLeftView printing

BOOL CLeftView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLeftView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLeftView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	InitLeftView();
}
///////////////////////////////////////////////////////////////////////////////
// Init Test
///////////////////////////////////////////////////////////////////////////////
/*#include "CommandHistory.h"

void TestOnInit()
{
}
*/

int CLeftView::InitLeftView()
{	
	m_font.CreateFont(10, 0, 0, 0, FW_THIN, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, (LPCTSTR)"Fixedsys");
	this->SetFont(&m_font);

	m_uiAliveTimer = SetTimer(101, 30000, NULL);
	CTreeCtrl &tctl = GetTreeCtrl();
	HTREEITEM	hRootItem;

	for(int i=0; i<g_ServerGroupNameVector.size(); i++)
	{
		// level 0, root
		CMonitorNode *pNode = new CMonitorNode("Root Node");
		hRootItem = tctl.InsertItem(g_ServerGroupNameVector[i].c_str(), 0, 0);
		tctl.SetItemData(hRootItem, (DWORD)pNode);

		// Pre Server
		// level 1
		pNode = new CMonitorNode("PreServer");
		m_hPreItem = GetTreeCtrl().InsertItem("PreServer", 1, 1, hRootItem);
		tctl.SetItemData(m_hPreItem, (DWORD)pNode);
		m_hItemVector.push_back(m_hPreItem);

		// IM Server
		// level 1
		pNode = new CMonitorNode("IMServer");
		m_hIMItem = GetTreeCtrl().InsertItem("IMServer", 1, 1, hRootItem);
		tctl.SetItemData(m_hIMItem, (DWORD)pNode);
		m_hItemVector.push_back(m_hIMItem);

		// Field Server
		// level 1
		pNode = new CMonitorNode("FieldServer");
		m_hFieldItem = GetTreeCtrl().InsertItem("FieldServer", 1, 1, hRootItem);
		tctl.SetItemData(m_hFieldItem, (DWORD)pNode);
		m_hItemVector.push_back(m_hFieldItem);

		// NPC Server
		// level 1
		pNode = new CMonitorNode("NPCServer");
		m_hNPCItem = GetTreeCtrl().InsertItem("NPCServer", 1, 1, hRootItem);
		tctl.SetItemData(m_hNPCItem, (DWORD)pNode);
		m_hItemVector.push_back(m_hNPCItem);

		// Log Server
		// level 1
		pNode = new CMonitorNode("LogServer");
		m_hLogItem = GetTreeCtrl().InsertItem("LogServer", 1, 1, hRootItem);
		tctl.SetItemData(m_hLogItem, (DWORD)pNode);
		m_hItemVector.push_back(m_hLogItem);

		if(i == 0)
		{
			m_hRootItem = hRootItem;
			GetTreeCtrl().Expand(hRootItem, TVE_EXPAND);
		}
	}

//	TestOnInit();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CAtumMonitorDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAtumMonitorDoc)));
	return (CAtumMonitorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	m_ImgList.Create(IDB_TREE_ICON, 16,1, RGB(0,128,128));
	GetTreeCtrl().SetImageList(&m_ImgList, TVSIL_NORMAL);

	return 0;
}

void CLeftView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CMenu muTemp, *pContextMenu;

	UINT uFlags;
	HTREEITEM htItem = GetTreeCtrl().HitTest(point, &uFlags);
	HTREEITEM parentItem = GetTreeCtrl().GetParentItem(htItem);
	HTREEITEM grandParentItem = GetTreeCtrl().GetParentItem(parentItem);

	m_hSelectedItem = GetTreeCtrl().HitTest(point, &uFlags);
	m_hSelectedParentItem =  GetTreeCtrl().GetParentItem(htItem);

	if ((htItem != NULL) && (uFlags & TVHT_ONITEM))
	{
		if(NULL == parentItem)
		{
			m_hRootTempItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_ROOT);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if(strcmp(GetTreeCtrl().GetItemText(htItem), "FieldServer") == 0 )
		{
			m_hFieldItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_FIELD);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if(parentItem &&
			strcmp(GetTreeCtrl().GetItemText(parentItem), "FieldServer") == 0 )
		{
			m_hFieldItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_FIELD);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(1);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if(grandParentItem
			&& strcmp(GetTreeCtrl().GetItemText(grandParentItem), "FieldServer") == 0 )
		{
			m_hFieldItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_FIELD);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(2);

			// 컨텍스트 메뉴 조정
			CChannelInfoNode *pChannelInfoNode
				= (CChannelInfoNode*)GetTreeCtrl().GetItemData(htItem);
			if (pChannelInfoNode->m_bIsEnabled)
			{
				// Grayed out menu item
				pContextMenu->EnableMenuItem(ID_TCM_FIELD_ENABLE_CHANNEL, MF_BYCOMMAND|MF_GRAYED);
				pContextMenu->EnableMenuItem(ID_TCM_FIELD_DISABLE_CHANNEL, MF_BYCOMMAND);
			}
			else
			{
				pContextMenu->EnableMenuItem(ID_TCM_FIELD_ENABLE_CHANNEL, MF_BYCOMMAND);
				pContextMenu->EnableMenuItem(ID_TCM_FIELD_DISABLE_CHANNEL, MF_BYCOMMAND|MF_GRAYED);
			}

			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

		}
		else if (strcmp(GetTreeCtrl().GetItemText(htItem), "IMServer") == 0 )
		{
			m_hIMItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_IM);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if(parentItem
			&& strcmp(GetTreeCtrl().GetItemText(parentItem), "IMServer") == 0 )
		{
			m_hIMItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_IM);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(1);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if(grandParentItem
			&& strcmp(GetTreeCtrl().GetItemText(grandParentItem), "IMServer") == 0 )
		{
			m_hIMItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_IM);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(2);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (strcmp(GetTreeCtrl().GetItemText(htItem), "NPCServer") == 0 )
		{
			m_hNPCItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_NPC);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (parentItem
			&& strcmp(GetTreeCtrl().GetItemText(parentItem), "NPCServer") == 0 )
		{
			m_hNPCItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_NPC);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(1);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (grandParentItem
			&& strcmp(GetTreeCtrl().GetItemText(grandParentItem), "NPCServer") == 0 )
		{
			m_hNPCItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_NPC);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(2);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (strcmp(GetTreeCtrl().GetItemText(htItem), "PreServer") == 0 )
		{
			m_hPreItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_PRE);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (parentItem
			&& strcmp(GetTreeCtrl().GetItemText(parentItem), "PreServer") == 0 )
		{
			m_hPreItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_PRE);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(1);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
		else if (grandParentItem
			&& strcmp(GetTreeCtrl().GetItemText(grandParentItem), "PreServer") == 0 )
		{
			m_hPreItem = htItem;
			GetTreeCtrl().SelectItem(htItem);
			muTemp.LoadMenu(IDR_TREE_CONTEXT_MENU_PRE);
			CTreeView::OnRButtonDown(nFlags, point);
			ClientToScreen(&point);
			pContextMenu = muTemp.GetSubMenu(2);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
	}
}

void CLeftView::GetOnlyPathWithoutFileName(CString &i_szFilePath)
{
	if (i_szFilePath.ReverseFind('/') != -1)
	{
		i_szFilePath = i_szFilePath.Left(i_szFilePath.ReverseFind('/'));
	}
}

LONG CLeftView::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	CMonitorWinSocket *pMonitorWinSocket = (CMonitorWinSocket*)lParam;
	if(NULL == pMonitorWinSocket){	return 0L;}

	switch(LOWORD(wParam))
	{
	case CMonitorWinSocket::WS_ERROR:
		{
			if(pMonitorWinSocket->GetTreeItem())
			{
				CMonitorNode *pNode = (CMonitorNode*)this->GetTreeCtrl().GetItemData(pMonitorWinSocket->GetTreeItem());
				util::del(pNode);
				GetTreeCtrl().DeleteItem(pMonitorWinSocket->GetTreeItem());
				pMonitorWinSocket->SetTreeItem(NULL);
			}
			DeleteListMonitorSocket(pMonitorWinSocket);
		}
		break;
	case CMonitorWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				MessageType_t msgType = T_NA;
				if (strcmp(GetTreeCtrl().GetItemText(m_hSelectedItem), "FieldServer") == 0)
				{
					msgType = T_FM_CONNECT;
				}
				else if (strcmp(GetTreeCtrl().GetItemText(m_hSelectedItem), "IMServer") == 0)
				{
					msgType = T_IM_CONNECT;
				}
				else if (strcmp(GetTreeCtrl().GetItemText(m_hSelectedItem), "NPCServer") == 0)
				{
					msgType = T_NM_CONNECT;
				}
				else if (strcmp(GetTreeCtrl().GetItemText(m_hSelectedItem), "PreServer") == 0)
				{
					msgType = T_PM_CONNECT;
				}
				pMonitorWinSocket->WriteMessageType(msgType);
			}
			else
			{
				MessageBox("Cannot Connect to Server!");
				///////////////////////////////////////////////////////////////////////////
				// Delete Node, Socket, hItem
				if(pMonitorWinSocket->GetTreeItem())
				{
					CMonitorNode *pNode = (CMonitorNode*)this->GetTreeCtrl().GetItemData(pMonitorWinSocket->GetTreeItem());
					util::del(pNode);
					GetTreeCtrl().DeleteItem(pMonitorWinSocket->GetTreeItem());
					pMonitorWinSocket->SetTreeItem(NULL);
				}
				DeleteListMonitorSocket(pMonitorWinSocket);
			}
		}
		break;
	case CMonitorWinSocket::WS_CLOSED:
		{
			if(pMonitorWinSocket->GetTreeItem())
			{
				CMonitorNode *pNode = (CMonitorNode*)this->GetTreeCtrl().GetItemData(pMonitorWinSocket->GetTreeItem());
				util::del(pNode);
				GetTreeCtrl().DeleteItem(pMonitorWinSocket->GetTreeItem());
				pMonitorWinSocket->SetTreeItem(NULL);
			}
			DeleteListMonitorSocket(pMonitorWinSocket);

			if(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath.IsEmpty())
			{
				m_bNoticePlaySound = sndPlaySound("NoFilePath", SND_ASYNC|SND_LOOP);
				SetLastError(0);
			}
			else
			{
				m_bNoticePlaySound = sndPlaySound(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath, SND_ASYNC|SND_LOOP);
				SetLastError(0);
			}
			MessageBox("Socket Closed by Server!");
			sndPlaySound(NULL, 0);
		}
		break;
	case CMonitorWinSocket::WS_RECEIVED:
		{
			CMonitorServerNode *pMonitorServerNode = NULL;
			char * pPacket = NULL;
			int len;
			MessageType_t nType;
			pMonitorWinSocket->Read(&pPacket, len);
			if(pPacket)
			{
				nType = *(MessageType_t*)pPacket;
				switch(nType)
				{
				///////////////////////////////////////////////////////////////////////
				// Field Server
				case T_FM_CONNECT_OK:
					{
						MSG_FM_CONNECT_OK *pMsg;
						pMsg = (MSG_FM_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						((CFieldServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem))->SetData(pMsg->ServerName, pMsg->StartedTime, pMsg->IPAddress, pMsg->Port, pMsg->NumOfClients, pMsg->LoadedMapCounts, pMsg->CalcBandwidth);

						char name[1024];
						sprintf(name, "%s(%15s)", pMsg->ServerName, pMsg->IPAddress);
						tctl.SetItemText(pMonitorWinSocket->m_hTreeItem, name);

						tctl.Expand(m_hFieldItem, TVE_EXPAND);
						tctl.SelectItem(pMonitorWinSocket->m_hTreeItem);
					}
					break;
				case T_FM_GET_NUM_CLIENTS_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MSG_FM_GET_NUM_CLIENTS_OK *pMsg;
						pMsg = (MSG_FM_GET_NUM_CLIENTS_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CFieldServerNode *pNode = (CFieldServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem);
						pNode->m_nClients = pMsg->NumOfClients;

						if(pNode == CMonitorNode::ms_pDispayNode)
						{
							pNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
						}
					}
					break;
				case T_FM_GET_CHARACTER_INFO_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MSG_FM_GET_CHARACTER_INFO_OK *pMsg;
						pMsg = (MSG_FM_GET_CHARACTER_INFO_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);
					}
					break;
				case T_FM_PING_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CFieldServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Is Alive.";
						MessageBox(str);
					}
					break;
				case T_FM_SHUTDOWN_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CFieldServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Shutdown.";
						MessageBox(str);

						///////////////////////////////////////////////////////////////////////////
						// Delete item, node, socket
						HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
						CMonitorWinSocket *pMonitorWinSocket
							= ((CFieldServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;
						if (pMonitorWinSocket != NULL)
							pMonitorWinSocket->CloseSocket();

						CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
						util::del(pNode);
						util::del(pMonitorWinSocket);
						GetTreeCtrl().DeleteItem(hItem);
					}
					break;
				case T_FM_SEND_CHANNEL_INFO:
					{
						char szTemp[256];
						MSG_FM_SEND_CHANNEL_INFO *pMsg;
						pMsg = (MSG_FM_SEND_CHANNEL_INFO*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CChannelInfoNode *pNode = NULL;

						pNode	= new CChannelInfoNode(pMonitorWinSocket);
						pNode->m_ChannelIndex	= pMsg->ChannelIndex;
						pNode->m_bIsEnabled		= pMsg->IsEnabled;
						pNode->m_projectInfo	= pMsg->projectInfo;
						if(pMsg->bCityWarMap)
						{
							pNode->m_bCityWarMap		= TRUE;
							pNode->m_cityWarOccupyInfo	= pMsg->cityWarOccupyInfo;
						}						

						///////////////////////////////////////////////////////
						// make node
						CTreeCtrl &tctl = GetTreeCtrl();
						((CFieldServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem()))->InsertChannelInfoNodePtr(pNode);
						if (pMsg->IsEnabled)
						{
							sprintf(szTemp, "%04d(%2d )", pNode->m_projectInfo.m_nMapIndex, pNode->m_ChannelIndex);
						}
						else
						{
							sprintf(szTemp, "%04d(%2d )", pNode->m_projectInfo.m_nMapIndex, pNode->m_ChannelIndex);
						}
						HTREEITEM hItem = tctl.InsertItem(szTemp, 3, 3, pMonitorWinSocket->GetTreeItem(), TVI_SORT);
						pNode->m_hItem = hItem;

						///////////////////////////////////////////////////////
						// associate hItem & socket
						tctl.SetItemData(hItem, (DWORD)pNode);
						GetTreeCtrl().Expand(pMonitorWinSocket->GetTreeItem(), TVE_EXPAND);
					}
					break;
				case T_FM_CHANGE_BANDWIDTH_FLAG_OK:
					{
						MSG_FM_CHANGE_BANDWIDTH_FLAG_OK *pChangeFlagOK
							= (MSG_FM_CHANGE_BANDWIDTH_FLAG_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						pMonitorServerNode = (CMonitorServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						pMonitorServerNode->m_bFlagCalcBandWidth = pChangeFlagOK->bChagedFlag;
						pMonitorServerNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;
				case T_FM_GET_MAP_USER_COUNTS_ACK:
					{
						MSG_FM_GET_MAP_USER_COUNTS_ACK *pMapUserCountsAck
							= (MSG_FM_GET_MAP_USER_COUNTS_ACK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						///////////////////////////////////////////////////////
						// make node
						CTreeCtrl &tctl = GetTreeCtrl();
						CChannelInfoNode *pPINode = ((CFieldServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem()))->GetChannelInfoNode(pMapUserCountsAck->MapChannIndex);
						if(pPINode)
						{
							pPINode->m_uiAccumulatedUserCounts		= pMapUserCountsAck->AccumulatedUserCounts;
							pPINode->m_uiCurrentUserCounts			= pMapUserCountsAck->CurrentUserCounts;
							pPINode->m_uiMaxUserCounts				= pMapUserCountsAck->MaxUserCounts;
							pPINode->m_uiAccumulatedMonsterCounts	= pMapUserCountsAck->AccumulatedMonsterCounts;
							pPINode->m_uiCurrentMonsterCounts		= pMapUserCountsAck->CurrentMonsterCounts;
							pPINode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());

							if (pPINode != NULL)
							{
								// 이름 변경
								CString szChannelNodeName;
								if (pPINode->m_bIsEnabled)
								{
									szChannelNodeName.Format(_STRMSG_S_SCMONITOR_0000,
										pPINode->m_projectInfo.m_nMapIndex, pPINode->m_ChannelIndex,
										pPINode->m_uiCurrentUserCounts, pPINode->m_uiCurrentMonsterCounts);
								}
								else
								{
									szChannelNodeName.Format(_STRMSG_S_SCMONITOR_0001,
										pPINode->m_projectInfo.m_nMapIndex, pPINode->m_ChannelIndex,
										pPINode->m_uiCurrentUserCounts, pPINode->m_uiCurrentMonsterCounts);
								}
								tctl.SetItemText(pPINode->m_hItem, szChannelNodeName);
							}
						}
					}
					break;
				case T_FM_RELOAD_ITEM_INFO_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "Item Info Reload DONE!";
						MessageBox(str);
					}
					break;
				case T_FM_RELOAD_SERVER_INFO_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "Server's data reloaded successfully.";
						MessageBox(str);
					}
					break;
				case T_FM_SET_SERVICE_STATE_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "Service state reflected successfully.";
						MessageBox(str);
					}
					break;
				case T_FM_RESET_NPC_SERVER_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "NPC Server RESET DONE!";
						MessageBox(str);
					}
					break;
				case T_FM_SERVER_THREAD_STATUS:
					{
						MSG_FM_SERVER_THREAD_STATUS *pRecvFM
							= (MSG_FM_SERVER_THREAD_STATUS*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						if(!m_bNoticePlaySound)
						{
							if(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath.IsEmpty())
							{
								m_bNoticePlaySound = sndPlaySound("NoFilePath", SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
							else
							{
								m_bNoticePlaySound = sndPlaySound(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath, SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
						}
					}
					break;
				case T_FM_SET_CHANNEL_STATE_OK:
					{
						MSG_FM_SET_CHANNEL_STATE_OK *pSetOK
							= (MSG_FM_SET_CHANNEL_STATE_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CChannelInfoNode *pPINode
							= ((CFieldServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem()))->GetChannelInfoNode(pSetOK->MapChannelIndex);
						if (pPINode != NULL)
						{
							// 채널 상태 변경
							pPINode->m_bIsEnabled = pSetOK->IsEnabled;

							// 이름 변경
							CString szChannelNodeName;
							if (pSetOK->IsEnabled)
							{
								szChannelNodeName.Format(_STRMSG_S_SCMONITOR_0000
									, pPINode->m_projectInfo.m_nMapIndex, pPINode->m_ChannelIndex
									, pPINode->m_uiCurrentUserCounts, pPINode->m_uiCurrentMonsterCounts);
							}
							else
							{
								szChannelNodeName.Format(_STRMSG_S_SCMONITOR_0001
									, pPINode->m_projectInfo.m_nMapIndex, pPINode->m_ChannelIndex
									, pPINode->m_uiCurrentUserCounts, pPINode->m_uiCurrentMonsterCounts);
							}
							tctl.SetItemText(pPINode->m_hItem, szChannelNodeName);
						}
					}
					break;
				case T_FM_CITYWAR_CHANGED_OCCUPY_INFO:
					{
						MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO *pChangeOccInfo
							= (MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO*)(pPacket+SIZE_FIELD_TYPE_HEADER);
						
						CTreeCtrl &tctl = GetTreeCtrl();
						CFieldServerNode *pFSNode = (CFieldServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						if(pFSNode)
						{
							CChannelInfoNode *pPINode = pFSNode->GetChannelInfoNode(pChangeOccInfo->MapChannelIndex);
							if(pPINode)
							{
								pPINode->m_cityWarOccupyInfo = pChangeOccInfo->cityWarOccupyInfo;							
							}

							if(pPINode == CMonitorNode::ms_pDispayNode)
							{
								pPINode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
							}
						}
					}
					break;
				///////////////////////////////////////////////////////////////////////
				// NPC Server
				case T_NM_CONNECT_OK:
					{
						MSG_NM_CONNECT_OK *pMsg;
						pMsg = (MSG_NM_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						((CNPCServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem))->SetData(pMsg->ServerName, pMsg->StartedTime, pMsg->IPAddress, pMsg->Port, pMsg->NumOfClients, pMsg->LoadedMapCounts, pMsg->CalcBandwidth);
						char name[1024];
						sprintf(name, "%s(%15s)", pMsg->ServerName, pMsg->IPAddress);
						tctl.SetItemText(pMonitorWinSocket->m_hTreeItem, name);

						tctl.Expand(m_hNPCItem, TVE_EXPAND);
						tctl.SelectItem(pMonitorWinSocket->m_hTreeItem);
					}
					break;
				case T_NM_GET_NUM_MONSTERS_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MSG_NM_GET_NUM_MONSTERS_OK *pMsg;
						pMsg = (MSG_NM_GET_NUM_MONSTERS_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CNPCServerNode *pNode = (CNPCServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem);
						pNode->m_nClients = pMsg->NumOfMonsters;
						pNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;
				case T_NM_PING_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CNPCServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Is Alive.";
						MessageBox(str);
					}
					break;
				case T_NM_SHUTDOWN_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CNPCServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Shutdown.";
						MessageBox(str);

						///////////////////////////////////////////////////////////////////////////
						// Delete item, node, socket
						HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
						CMonitorWinSocket *pMonitorWinSocket
							= ((CNPCServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;
						if (pMonitorWinSocket != NULL)
							pMonitorWinSocket->CloseSocket();

						CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
						util::del(pNode);
						util::del(pMonitorWinSocket);
						GetTreeCtrl().DeleteItem(hItem);

					}
					break;
				case T_NM_SEND_CHANNEL_INFO:
					{
						char szTemp[256];
						MSG_NM_SEND_CHANNEL_INFO *pMsg;
						pMsg = (MSG_NM_SEND_CHANNEL_INFO*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CChannelInfoNode *pNode = NULL;

						pNode = new CChannelInfoNode(pMonitorWinSocket);
						pNode->m_ChannelIndex			= pMsg->ChannelIndex;
						pNode->m_bIsEnabled				= pMsg->IsEnabled;
						pNode->m_projectInfo			= pMsg->projectInfo;
						pNode->m_uiLimitMonsterCount	= pMsg->LimitMonsterCount;

						///////////////////////////////////////////////////////
						// make node
						CTreeCtrl &tctl = GetTreeCtrl();
						((CNPCServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem()))->InsertChannelInfoNodePtr(pNode);
						if (pMsg->IsEnabled)
						{
							sprintf(szTemp, _STRMSG_S_SCMONITOR_0002, pNode->m_projectInfo.m_nMapIndex, pNode->m_ChannelIndex);
						}
						else
						{
							sprintf(szTemp, _STRMSG_S_SCMONITOR_0003, pNode->m_projectInfo.m_nMapIndex, pNode->m_ChannelIndex);
						}
						HTREEITEM hItem = tctl.InsertItem(szTemp, 3, 3, pMonitorWinSocket->GetTreeItem(), TVI_SORT);
						pNode->m_hItem = hItem;

						///////////////////////////////////////////////////////
						// associate hItem & socket
						tctl.SetItemData(hItem, (DWORD)pNode);
						GetTreeCtrl().Expand(pMonitorWinSocket->GetTreeItem(), TVE_EXPAND);
					}
					break;
				case T_NM_CHANGE_BANDWIDTH_FLAG_OK:
					{
						MSG_NM_CHANGE_BANDWIDTH_FLAG_OK *pChangeFlagOK
							= (MSG_NM_CHANGE_BANDWIDTH_FLAG_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						pMonitorServerNode = (CMonitorServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						pMonitorServerNode->m_bFlagCalcBandWidth = pChangeFlagOK->bChagedFlag;
						pMonitorServerNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;
				case T_NM_GET_MAP_USER_COUNTS_ACK:
					{
						MSG_NM_GET_MAP_USER_COUNTS_ACK *pMapUserCountsAck
							= (MSG_NM_GET_MAP_USER_COUNTS_ACK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						///////////////////////////////////////////////////////
						// make node
						CTreeCtrl &tctl = GetTreeCtrl();
						CChannelInfoNode *pPINode = ((CNPCServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem()))->GetChannelInfoNode(pMapUserCountsAck->MapChannIndex);
						if(pPINode)
						{
							pPINode->m_uiAccumulatedUserCounts = pMapUserCountsAck->AccumulatedUserCounts;
							pPINode->m_uiCurrentUserCounts = pMapUserCountsAck->CurrentUserCounts;
							pPINode->m_uiMaxUserCounts = pMapUserCountsAck->MaxUserCounts;
							pPINode->m_uiAccumulatedMonsterCounts = pMapUserCountsAck->AccumulatedMonsterCounts;
							pPINode->m_uiCurrentMonsterCounts = pMapUserCountsAck->CurrentMonsterCounts;
							pPINode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
						}
					}
					break;
					
				case T_NM_SET_LIMIT_MONSTER_COUNT_ACK:
					{
						MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK *pAck
							= (MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						///////////////////////////////////////////////////////
						// make node
						CTreeCtrl &tctl = GetTreeCtrl();
						CChannelInfoNode *pPINode = ((CNPCServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem()))->GetChannelInfoNode(pAck->MapChannelIndex);
						if(pPINode)
						{
							pPINode->m_uiLimitMonsterCount = pAck->SetLimitMonsterCount;
							pPINode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
						}
					}
					break;
				case T_NM_SERVER_THREAD_STATUS:
					{
						MSG_NM_SERVER_THREAD_STATUS *pRecvNM
							= (MSG_NM_SERVER_THREAD_STATUS*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						if(!m_bNoticePlaySound)
						{
							if(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath.IsEmpty())
							{
								m_bNoticePlaySound = sndPlaySound("NoFilePath", SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
							else
							{
								m_bNoticePlaySound = sndPlaySound(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath, SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
						}
					}
					break;

				///////////////////////////////////////////////////////////////////////
				// PRE Server
				case T_PM_CONNECT_OK:
					{
						MSG_PM_CONNECT_OK *pMsg;
						pMsg = (MSG_PM_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						((CPreServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem))->SetData(pMsg->ServerName
							, pMsg->StartedTime, pMsg->IPAddress, pMsg->Port, pMsg->NumOfClients, pMsg->CalcBandwidth);
						CPreServerNode *pPreNode = ((CPreServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem));
// 2007-01-08 by cmkwon, T_PM_AUTO_UPDATE_FTP_SERVER_SETTING로 나눠서 전송으로 변경
//						util::strncpy(pPreNode->m_szFtpIP, pMsg->FtpIP, SIZE_MAX_FTP_URL);
//						pPreNode->m_nFtpPort = pMsg->FtpPort;
//						util::strncpy(pPreNode->m_szFtpAccountName, pMsg->FtpAccountName, SIZE_MAX_ACCOUNT_NAME);
//						util::strncpy(pPreNode->m_szFtpPassword, pMsg->FtpPassword, SIZE_MAX_PASSWORD);
//// 2005-12-23 by cmkwon, 아래와 같이 수정함
////#ifdef _MGAME_TEST_SERVER
////						pPreNode->m_szClientFTPUpdateDir	= "//TestServer/spacecowboy";
////						pPreNode->m_szLauncherFilePath		= "//TestServer/launcher";
////						pPreNode->m_szDeleteFileListPath	= "//TestServer/deletefilelist";
////						pPreNode->m_szNoticeFilePath		= "//TestServer/notice";
////#else 
////						pPreNode->m_szClientFTPUpdateDir	= "//spacecowboy";
////						pPreNode->m_szLauncherFilePath		= "//launcher";
////						pPreNode->m_szDeleteFileListPath	= "//deletefilelist";
////						pPreNode->m_szNoticeFilePath		= "//notice";
////#endif
//						pPreNode->m_szClientFTPUpdateUploadDir	= pMsg->ClientFTPUpdateUploadDir;		// 2005-12-23 by cmkwon
//						pPreNode->m_szLauncherFileUploadPath	= pMsg->LauncherFileUploadPath;
//						pPreNode->m_szDeleteFileListUploadPath	= pMsg->DeleteFileListUploadPath;
//						pPreNode->m_szNoticeFileUploadPath		= pMsg->NoticeFileUploadPath;
//						GetOnlyPathWithoutFileName(pPreNode->m_szLauncherFileUploadPath);
//						GetOnlyPathWithoutFileName(pPreNode->m_szDeleteFileListUploadPath);
//						GetOnlyPathWithoutFileName(pPreNode->m_szNoticeFileUploadPath);

						char name[1024];
						sprintf(name, "%s(%15s)", pPreNode->m_szServerName, pPreNode->m_szIP);
						tctl.SetItemText(pMonitorWinSocket->m_hTreeItem, name);

						tctl.Expand(m_hPreItem, TVE_EXPAND);
						tctl.SelectItem(pMonitorWinSocket->m_hTreeItem);
					}
					break;
				case T_PM_GET_NUM_CLIENTS_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MSG_PM_GET_NUM_CLIENTS_OK *pMsg;
						pMsg = (MSG_PM_GET_NUM_CLIENTS_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CPreServerNode *pNode = (CPreServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem);
						pNode->m_nClients = pMsg->NumOfClients;
						pNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());

						char str[256];
						sprintf(str, "%s\'s # of clients: %d\n"
							, ((CPreServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName
							, pMsg->NumOfClients
						);
						MessageBox(str);
					}
					break;
				case T_PM_RELOAD_VERSION_INFO_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "Version infomation list reloaded successfully!";
						MessageBox(str);
					}
					break;
				case T_PM_RELOAD_BLOCKED_ACCOUNTS_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "Complete blocked Account list loading.";
						MessageBox(str);
					}
					break;
				case T_PM_PING_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CPreServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Is Alive.";
						MessageBox(str);
					}
					break;
				case T_PM_PAUSE_SERVICE_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MessageBox("Pre server service discontinued.");
					}
					break;
				case T_PM_START_SERVICE_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MessageBox("Pre server service began.");
					}
					break;
				case T_PM_SHUTDOWN_OK:
					{
						/*
						pMonitorWinSocket->KillTimeOut();
						CString str = "\"";
						str += ((CPreServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Shutdown.";
						MessageBox(str);

						CString str = "\"";
						str += ((CPreServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Shutdown.";
						MessageBox(str);

						///////////////////////////////////////////////////////////////////////////
						// Delete item, node, socket
						HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
						CMonitorWinSocket *pMonitorWinSocket
							= ((CNPCServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;
						if (pMonitorWinSocket != NULL)
							pMonitorWinSocket->CloseSocket();

						CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
						util::del(pNode);
						util::del(pMonitorWinSocket);
						GetTreeCtrl().DeleteItem(hItem);
						//*/
					}
					break;
				case T_PM_CHANGE_BANDWIDTH_FLAG_OK:
					{
						MSG_PM_CHANGE_BANDWIDTH_FLAG_OK *pChangeFlagOK
							= (MSG_PM_CHANGE_BANDWIDTH_FLAG_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						pMonitorServerNode = (CMonitorServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						pMonitorServerNode->m_bFlagCalcBandWidth = pChangeFlagOK->bChagedFlag;
						pMonitorServerNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;
				case T_PM_GET_SERVER_GROUP_INFO_ACK:
					{
						MSG_PM_GET_SERVER_GROUP_INFO_ACK *pGroupInfo
							= (MSG_PM_GET_SERVER_GROUP_INFO_ACK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CPreServerNode *pPreServerNode = (CPreServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						list<CServerGroupInfoNode*>::iterator itr(pPreServerNode->m_listServerGroupInfoNodePtr.begin());
						while(itr != pPreServerNode->m_listServerGroupInfoNodePtr.end())
						{
							if(strnicmp(pGroupInfo->ServerGroupName, (*itr)->m_ServerGroupInfo.ServerGroupName, SIZE_MAX_SERVER_NAME) == 0)
							{
								(*itr)->m_ServerGroupInfo.AccumulatedGroupUserCounts = pGroupInfo->AccumulatedGroupUserCounts;
								(*itr)->m_ServerGroupInfo.CurrentGroupUserCounts = pGroupInfo->CurrentGroupUserCounts;
								(*itr)->m_ServerGroupInfo.MaxGroupUserCounts = pGroupInfo->MaxGroupUserCounts;
								(*itr)->m_ServerGroupInfo.LimitGroupUserCounts = pGroupInfo->LimitGroupUserCounts;

								if(*itr == CMonitorNode::ms_pDispayNode)
								{
									(*itr)->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
								}								

								char szTemp[256];
								sprintf(szTemp, "%12s(%4d)", (*itr)->m_ServerGroupInfo.ServerGroupName, pGroupInfo->CurrentGroupUserCounts);
								tctl.SetItemText((*itr)->m_hItem, szTemp);
								break;
							}
							itr++;
						}

						if(itr == pPreServerNode->m_listServerGroupInfoNodePtr.end())
						{
							CServerGroupInfoNode *pNode	= new CServerGroupInfoNode(pMonitorWinSocket);
							pNode->m_ServerGroupInfo = *pGroupInfo;

							///////////////////////////////////////////////////////
							// make node
							char szTemp[256];
							pPreServerNode->InsertServerGroupInfoNodePtr(pNode);
							sprintf(szTemp, "%12s(%4d)", pNode->m_ServerGroupInfo.ServerGroupName, pGroupInfo->CurrentGroupUserCounts);
							pNode->m_hItem = tctl.InsertItem(szTemp, 3, 3, pMonitorWinSocket->GetTreeItem());

							///////////////////////////////////////////////////////
							// associate hItem & socket
							tctl.SetItemData(pNode->m_hItem, (DWORD)pNode);
							GetTreeCtrl().Expand(pMonitorWinSocket->GetTreeItem(), TVE_EXPAND);
						}
					}
					break;
				case T_PM_SERVER_THREAD_STATUS:
					{
						MSG_PM_SERVER_THREAD_STATUS *pRecvPM
							= (MSG_PM_SERVER_THREAD_STATUS*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						if(!m_bNoticePlaySound)
						{
							if(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath.IsEmpty())
							{
								m_bNoticePlaySound = sndPlaySound("NoFilePath", SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
							else
							{
								m_bNoticePlaySound = sndPlaySound(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath, SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
						}
					}
					break;
				case T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING:		// 2007-01-06 by cmkwon
					{
						MSG_PM_AUTO_UPDATE_HTTP_SERVER_SETTING *pRMsg
							= (MSG_PM_AUTO_UPDATE_HTTP_SERVER_SETTING*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CPreServerNode *pPreNode = ((CPreServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem));

						///////////////////////////////////////////////////////////////////////////////
						// 2007-01-06 by cmkwon
						util::strncpy(pPreNode->m_szDownloadHttpServerIP, pRMsg->szDownloadHttpServerIP, SIZE_MAX_FTP_URL);
						pPreNode->m_nDownloadHttpServerPort		= pRMsg->nDownloadHttpServerPort;
						util::strncpy(pPreNode->m_szDownloadHttpServerAccountName, pRMsg->szDownloadHttpServerAccountName, SIZE_MAX_ACCOUNT_NAME);
						util::strncpy(pPreNode->m_szDownloadHttpServerPassword, pRMsg->szDownloadHttpServerPassword, SIZE_MAX_PASSWORD);
						pPreNode->m_szClientHttpUpdateDownloadDir		= pRMsg->szClientHttpUpdateDownloadDir;
						pPreNode->m_szLauncherFileDownloadHttpPath		= pRMsg->szLauncherFileDownloadHttpPath;
						pPreNode->m_szDeleteFileListDownloadHttpPath	= pRMsg->szDeleteFileListDownloadHttpPath;
						pPreNode->m_szNoticeFileDownloadHttpPath		= pRMsg->szNoticeFileDownloadHttpPath;
						util::strncpy(pPreNode->m_szVersionListFileDownloadHttpPath, pRMsg->szVersionListFileDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);	// 2007-01-08 by cmkwon

						GetOnlyPathWithoutFileName(pPreNode->m_szLauncherFileDownloadHttpPath);
						GetOnlyPathWithoutFileName(pPreNode->m_szDeleteFileListDownloadHttpPath);
						GetOnlyPathWithoutFileName(pPreNode->m_szNoticeFileDownloadHttpPath);
						pPreNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;

				//////////////////////////////////////////////////////////
				// Monitor <-- IMServer
				case T_IM_CONNECT_OK:
					{
						MSG_IM_CONNECT_OK *pMsg;
						pMsg = (MSG_IM_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						((CIMServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem))->SetData(pMsg->ServerName, pMsg->StartedTime, pMsg->IPAddress, pMsg->Port, pMsg->NumOfClients, pMsg->CalcBandwidth);
						char name[1024];
						sprintf(name, "%s(%15s)", pMsg->ServerName, pMsg->IPAddress);
						tctl.SetItemText(pMonitorWinSocket->m_hTreeItem, name);

						tctl.Expand(m_hIMItem, TVE_EXPAND);
						tctl.SelectItem(pMonitorWinSocket->m_hTreeItem);
					}
					break;
				case T_IM_GET_NUM_CLIENTS_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						MSG_IM_GET_NUM_CLIENTS_OK *pMsg;
						pMsg = (MSG_IM_GET_NUM_CLIENTS_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						CIMServerNode *pNode = (CIMServerNode*)tctl.GetItemData(pMonitorWinSocket->m_hTreeItem);
						pNode->m_nClients = pMsg->NumOfClients;
						pNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());

						char str[256];
						sprintf(str, "%s\'s # of clients: %d\n"
							, ((CIMServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName
							, pMsg->NumOfClients
						);
						MessageBox(str);
					}
					break;
				case T_IM_PING_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CIMServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Is Alive.";
						MessageBox(str);
					}
					break;
				case T_IM_SHUTDOWN_OK:
					{
						pMonitorWinSocket->KillTimeOut();

						CString str = "\"";
						str += ((CIMServerNode*)GetTreeCtrl().GetItemData(pMonitorWinSocket->m_hTreeItem))->m_szServerName;
						str += "\" Shutdown.";
						MessageBox(str);

						///////////////////////////////////////////////////////////////////////////
						// Delete item, node, socket
						HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
						CMonitorWinSocket *pMonitorWinSocket
							= ((CIMServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;
						if (pMonitorWinSocket != NULL)
							pMonitorWinSocket->CloseSocket();

						CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
						util::del(pNode);
						util::del(pMonitorWinSocket);
						GetTreeCtrl().DeleteItem(hItem);

					}
					break;
				case T_IM_CHANGE_BANDWIDTH_FLAG_OK:
					{
						MSG_IM_CHANGE_BANDWIDTH_FLAG_OK *pChangeFlagOK
							= (MSG_IM_CHANGE_BANDWIDTH_FLAG_OK*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						CTreeCtrl &tctl = GetTreeCtrl();
						pMonitorServerNode = (CMonitorServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						pMonitorServerNode->m_bFlagCalcBandWidth = pChangeFlagOK->bChagedFlag;
						pMonitorServerNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;
				case T_IM_GET_TOTAL_USER_COUNTS_ACK:
					{
						MSG_IM_GET_TOTAL_USER_COUNTS_ACK	*pRecvGetCountsAck
							= (MSG_IM_GET_TOTAL_USER_COUNTS_ACK*)(pPacket+ SIZE_FIELD_TYPE_HEADER);
						
						CTreeCtrl &tctl = GetTreeCtrl();
						CIMServerNode *pIMNode = (CIMServerNode*)tctl.GetItemData(pMonitorWinSocket->GetTreeItem());
						pIMNode->m_AccumulatedTotalUserCounts	= pRecvGetCountsAck->AccumulatedTotalUserCounts;
						pIMNode->m_CurrentTotalUserCounts		= pRecvGetCountsAck->CurrentTotalUserCounts;
						pIMNode->m_MaxTotalUserCounts			= pRecvGetCountsAck->MaxTotalUserCounts;

						pIMNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
					}
					break;
				case T_IM_SERVER_THREAD_STATUS:
					{
						MSG_IM_SERVER_THREAD_STATUS *pRecvIM
							= (MSG_IM_SERVER_THREAD_STATUS*)(pPacket + SIZE_FIELD_TYPE_HEADER);
						if(!m_bNoticePlaySound)
						{
							if(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath.IsEmpty())
							{
								m_bNoticePlaySound = sndPlaySound("NoFilePath", SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
							else
							{
								m_bNoticePlaySound = sndPlaySound(((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath, SND_ASYNC|SND_LOOP);
								SetLastError(0);
							}
						}
					}
					break;

				//////////////////////////////////////////////////////////
				//
				case T_ERROR:
					{
						MSG_ERROR *pRecvMsg;
						pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						char buf[128];
						Err_t error = pRecvMsg->ErrorCode;

						switch (error)
						{
						case ERR_PROTOCOL_ALL_FIELD_SERVER_NOT_ALIVE:
							{
								MessageBox("Field server is not run!!");
								//OnCancel();
							}
							break;
						default:
							{
								sprintf(buf, "Error: %s(%#04x)", GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
								MessageBox(buf);
							}
						}
					}
					break;
				default:
					{
						// check: unknown message type --> error!, 일단 message box 실행 & close
						char msg[256];
						sprintf(msg, "[Error]Unable to process Message Type: %s(%#04x) in CLeftView::OnSocketNotify()\n", GetProtocolTypeString(nType), nType);
						MessageBox(msg);
					}
					break;
				}
				util::del(pPacket);
			}
		}
		break;
	}

	return 0;
}

// WPARAM: Socket descriptor

LONG CLeftView::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	CMonitorWinSocket *pMonitorWinSocket = FindMonitorSocket(wParam);
	if(pMonitorWinSocket)
	{	// not user-called timer

		pMonitorWinSocket->OnAsyncEvent(lParam);
	}
	else
	{
		// error
		MessageBox("No Such Connection");
	}
	return 0;
}

void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//MessageBox("SelChanged!");

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);

	pNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());

	*pResult = 0;
}

void CLeftView::OnTcmNumClientsField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_FM_GET_NUM_CLIENTS;
	pFieldServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);

	pFieldServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmConnectField()
{
	// TODO: Add your command handler code here

	///////////////////////////////////////////////////////////////////////////
	// input server ip, port...
	CFieldServerDlg	dlg(g_szFieldServerIPReg, g_szServerIPHistoryReg, FIELD_SERVER_PORT);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	g_szFieldServerIPReg = dlg.m_ipaddr;
	g_nServerPortReg = dlg.m_port;
	g_szServerIPHistoryReg = dlg.m_szIPHistory;
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();

	///////////////////////////////////////////////////////////////////////////
	// Connect to server, send connect
	// if succeeded, insert node to tree and expand
	////////////////////////////////////////
	// Initializing Socket
	CMonitorWinSocket *pMonitorWinSocket = new CMonitorWinSocket(GetSafeHwnd(), this, WM_ASYNC_EVENT, WM_PACKET_NOTIFY);
	if (!pMonitorWinSocket->Connect(dlg.m_ipaddr, dlg.m_port))
	{
		MessageBox("Connect Error!");
		util::del(pMonitorWinSocket);
		return;
	}
	InsertListMonitorSocket(pMonitorWinSocket);

	CFieldServerNode *pNode
		= new CFieldServerNode(pMonitorWinSocket);

	///////////////////////////////////////////////////////
	// make node
	CTreeCtrl &tctl = GetTreeCtrl();
	HTREEITEM hItem = tctl.InsertItem("Connecting Field Server", 2, 2, m_hFieldItem);

	///////////////////////////////////////////////////////
	// associate hItem & socket
	tctl.SetItemData(hItem, (DWORD)pNode);
	pMonitorWinSocket->SetTreeItem(hItem);
}

void CLeftView::OnTcmDisconnectField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorWinSocket *pMonitorWinSocket
		= ((CFieldServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;

	if(pMonitorWinSocket)
	{
		pMonitorWinSocket->CloseSocket();
		DeleteListMonitorSocket(pMonitorWinSocket);
		pMonitorWinSocket = NULL;
	}

	///////////////////////////////////////////////////////////////////////////
	// Delete item
	CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
	util::del(pNode);
	GetTreeCtrl().DeleteItem(hItem);
}

void CLeftView::OnTcmPingField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_FM_PING;
	pFieldServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);

	pFieldServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmShutdownField()
{
	// TODO: Add your command handler code here
	if (IDYES != AfxMessageBox("Do you end Field server really?", MB_YESNO))
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_FM_SHUTDOWN;
	pFieldServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);

	pFieldServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 101)
	{
		listMonitorWinSocketPtr::iterator itr(m_listMonitorWinSocketPtr.begin());
		while(itr != m_listMonitorWinSocketPtr.end())
		{
			if((*itr)->IsConnected())
			{
				char	SendBuf[256];
				*(MessageType_t *)SendBuf = T_PM_CONNECT_ALIVE;
				(*itr)->Write(SendBuf, SIZE_FIELD_TYPE_HEADER);
			}
			itr++;
		}
	}
	else
	{
		CMonitorWinSocket *pMonitorWinSocket = FindMonitorSocket(nIDEvent);
		if (pMonitorWinSocket) // not user-called timer
		{
			if(!pMonitorWinSocket->GetTimeOutFlag())
			{
				return;
			}

			pMonitorWinSocket->KillTimeOut();
			MessageBox("Request Time Out!");
		}
	}
	CTreeView::OnTimer(nIDEvent);
}

void CLeftView::OnDestroy()
{
	CMonitorNode *pNode;

	for(int i=0; i < m_hItemVector.size(); i++)
	{
		pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(m_hItemVector[i]);
		util::del(pNode);
	}

	for(; m_hRootItem != NULL; m_hRootItem = GetTreeCtrl().GetNextItem(m_hRootItem, TVGN_NEXT))
	{
		pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(m_hRootItem);
		util::del(pNode);
	}

	sndPlaySound(NULL, 0);
	if(m_listMonitorWinSocketPtr.empty() == false)
	{
		CMonitorWinSocket	*pSocket = NULL;
		CMonitorNode		*pNode = NULL;
		while(m_listMonitorWinSocketPtr.empty() == false)
		{
			pSocket = *m_listMonitorWinSocketPtr.begin();
			pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(pSocket->GetTreeItem());
			util::del(pNode);

			pSocket->CloseSocket();
			delete pSocket;
			m_listMonitorWinSocketPtr.pop_front();
		}
	}
	
	CTreeView::OnDestroy();

	// TODO: Add your message handler code here
}

void CLeftView::OnTcmPrintmapField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_FM_PRINTMAP, T_FM_PRINTMAP, pSeMap, buff);
	pSeMap->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
	pSeMap->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;

	pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_FM_PRINTMAP));
}

void CLeftView::OnTcmMapblockinfoField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_FM_SAVE_MAPBLOCK_INFO, T_FM_SAVE_MAPBLOCK_INFO, pMapBlockInfo, buff);
	pMapBlockInfo->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
	pMapBlockInfo->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;

	pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_FM_SAVE_MAPBLOCK_INFO));
}

void CLeftView::OnTcmConnectNpc()
{
	// TODO: Add your command handler code here

	///////////////////////////////////////////////////////////////////////////
	// input server ip, port...
	CFieldServerDlg	dlg(g_szNPCServerIPReg, g_szServerIPHistoryReg, NPC_SERVER_PORT);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	g_szNPCServerIPReg = dlg.m_ipaddr;
	g_nServerPortReg = dlg.m_port;
	g_szServerIPHistoryReg = dlg.m_szIPHistory;
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();

	///////////////////////////////////////////////////////////////////////////
	// Connect to server, send connect
	// if succeeded, insert node to tree and expand
	////////////////////////////////////////
	// Initializing Socket
	CMonitorWinSocket *pMonitorWinSocket = new CMonitorWinSocket(GetSafeHwnd(), this, WM_ASYNC_EVENT, WM_PACKET_NOTIFY);
	if (!pMonitorWinSocket->Connect(dlg.m_ipaddr, dlg.m_port))
	{
		MessageBox("Connect Error!");
		util::del(pMonitorWinSocket);
		return;
	}
	InsertListMonitorSocket(pMonitorWinSocket);

	CNPCServerNode *pNode
		= new CNPCServerNode(pMonitorWinSocket);

	///////////////////////////////////////////////////////
	// make node
	CTreeCtrl &tctl = GetTreeCtrl();
	HTREEITEM hItem = tctl.InsertItem("Connecting NPC Server", 2, 2, m_hNPCItem);

	///////////////////////////////////////////////////////
	// associate hItem & socket
	tctl.SetItemData(hItem, (DWORD)pNode);
	pMonitorWinSocket->SetTreeItem(hItem);
}

void CLeftView::OnTcmPingNpc()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CNPCServerNode *pNode = (CNPCServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_NM_PING;
	pNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);

	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmDisconnectNpc()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorWinSocket *pMonitorWinSocket
		= ((CNPCServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;

	if(pMonitorWinSocket)
	{
		pMonitorWinSocket->CloseSocket();
		DeleteListMonitorSocket(pMonitorWinSocket);
		pMonitorWinSocket = NULL;
	}

	///////////////////////////////////////////////////////////////////////////
	// Delete item
	CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
	util::del(pNode);
	GetTreeCtrl().DeleteItem(hItem);
}

void CLeftView::OnTcmMapblockinfoNpc()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_NM_SAVE_MAPBLOCK_INFO, T_NM_SAVE_MAPBLOCK_INFO, pMapBlockInfo, buff);
	pMapBlockInfo->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
	pMapBlockInfo->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;

	pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_NM_SAVE_MAPBLOCK_INFO));
}

void CLeftView::OnTcmConnectPre()
{
	// TODO: Add your command handler code here

	///////////////////////////////////////////////////////////////////////////
	// input server ip, port...
	CFieldServerDlg	dlg(g_szPreServerIPReg, g_szServerIPHistoryReg, PRE_SERVER_PORT);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	g_szPreServerIPReg = dlg.m_ipaddr;
	g_nServerPortReg = dlg.m_port;
	g_szServerIPHistoryReg = dlg.m_szIPHistory;
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();

	///////////////////////////////////////////////////////////////////////////
	// Connect to server, send connect
	// if succeeded, insert node to tree and expand
	////////////////////////////////////////
	// Initializing Socket
	CMonitorWinSocket *pMonitorWinSocket = new CMonitorWinSocket(GetSafeHwnd(), this, WM_ASYNC_EVENT, WM_PACKET_NOTIFY);
	if (!pMonitorWinSocket->Connect(dlg.m_ipaddr, dlg.m_port))
	{
		MessageBox("Connect Error!");
		util::del(pMonitorWinSocket);
		return;
	}
	InsertListMonitorSocket(pMonitorWinSocket);

	CPreServerNode *pNode
		= new CPreServerNode(pMonitorWinSocket);

	///////////////////////////////////////////////////////
	// make node
	CTreeCtrl &tctl = GetTreeCtrl();
	HTREEITEM hItem = tctl.InsertItem("Connecting Pre Server", 2, 2, m_hPreItem);

	///////////////////////////////////////////////////////
	// associate hItem & socket
	tctl.SetItemData(hItem, (DWORD)pNode);
	pMonitorWinSocket->SetTreeItem(hItem);
}

void CLeftView::OnTcmDisconnectPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorWinSocket *pMonitorWinSocket
		= ((CPreServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;


	if(pMonitorWinSocket)
	{
		pMonitorWinSocket->CloseSocket();
		DeleteListMonitorSocket(pMonitorWinSocket);
		pMonitorWinSocket = NULL;
	}

	///////////////////////////////////////////////////////////////////////////
	// Delete item
	CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
	util::del(pNode);
	GetTreeCtrl().DeleteItem(hItem);
}

void CLeftView::OnTcmNumClientsPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pPreServerNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	pPreServerNode->m_pMonitorWinSocket->WriteMessageType(T_PM_GET_NUM_CLIENTS);

	pPreServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmPingPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	pNode->m_pMonitorWinSocket->WriteMessageType(T_PM_PING);

	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

BOOL DoesVersionExist(vector<VersionInfo> *i_pVersionInfoList, char *pVersionString)
{
	VersionInfo tmpVersion(pVersionString);

	vector<VersionInfo>::iterator itr = i_pVersionInfoList->begin();

	while (itr != i_pVersionInfoList->end())
	{
		if ( (*itr) == tmpVersion )
		{
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

static bool version_lt_data2(VersionInfo& d1, VersionInfo& d2)
{
	if (d1.GetVersion()[0] != d2.GetVersion()[0])
	{
		return d1.GetVersion()[0] < d2.GetVersion()[0];
	}
	if (d1.GetVersion()[1] != d2.GetVersion()[1])
	{
		return d1.GetVersion()[1] < d2.GetVersion()[1];
	}
	if (d1.GetVersion()[2] != d2.GetVersion()[2])
	{
		return d1.GetVersion()[2] < d2.GetVersion()[2];
	}
	if (d1.GetVersion()[3] != d2.GetVersion()[3])
	{
		return d1.GetVersion()[3] < d2.GetVersion()[3];
	}

	return FALSE;
}

BOOL CLeftView::GetVersionInfoFromFTP(CPreServerNode *i_pPreNode, ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
{
	HINTERNET hInternet;
	HINTERNET hFtpConnect;
	HINTERNET hDir;
	WIN32_FIND_DATA pDirInfo;
	char pDirString[SIZE_MAX_FTP_FILE_PATH];

	hInternet = InternetOpen("Atum Pre Server", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		// check: error
		MessageBox("InternetOpen ERROR!");
		return FALSE;
	}

	hFtpConnect = InternetConnect(hInternet, i_pPreNode->m_szFtpIP, i_pPreNode->m_nFtpPort,
		i_pPreNode->m_szFtpAccountName, i_pPreNode->m_szFtpPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hFtpConnect == NULL)
	{
		// check: error
		MessageBox("InternetConnect ERROR!");
		return FALSE;
	}

	char currDir[128];
	DWORD currDirLen = 128;
	if (FtpGetCurrentDirectory(hFtpConnect, currDir, &currDirLen))
	{
	}

	// change directory
	if (FALSE == FtpSetCurrentDirectory(hFtpConnect, (LPCSTR)i_pPreNode->m_szClientFTPUpdateUploadDir))
	{
		// check: error
		DWORD err = GetLastError();
		DWORD len = 128;
		char errString[128];
		InternetGetLastResponseInfo(&err, errString, &len);
		MessageBox(errString, "Change Directory ERROR!");
		
		return FALSE;
	}

	// Find files and update UpdateFileListVector
	sprintf(pDirString, "%s", "*");
// 2006-04-07 by cmkwon, 위와 같이 수정함. 
//	sprintf(pDirString, "%s/%s", (LPCSTR)i_pPreNode->m_szClientFTPUpdateUploadDir, "*");
	if (!(hDir = FtpFindFirstFile(hFtpConnect, pDirString, &pDirInfo, 0, 0) ) )
	{
		// check: error
		MessageBox("ERROR!");
		return FALSE;
	}
	else
	{
		if (strcmp(pDirInfo.cFileName, ".") != 0 && strcmp(pDirInfo.cFileName, "..") != 0 )
		{
			if (pDirInfo.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
			{
				// zip file 이름 해석하여 버전 넣기
				const int SIZE_BUFF = 512;
				char		buff[SIZE_BUFF];
				char		seps[] = "_zip";
				util::strncpy(buff, pDirInfo.cFileName, SIZE_BUFF);

				// from version
				char *token1 = strtok(buff, seps);
				// to version
				char *token2 = strtok(NULL, seps);

				VersionInfo OldVersion(token1);
				VersionInfo NewVersion(token2);

				VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
				if (pVersion == NULL)
				{
					o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
				}
				else
				{
					if (*pVersion < NewVersion)
					{
						*pVersion = NewVersion;
					}
				}
			}
			else
			{
				// check: error! directory가 존재하면 아니 됨!
				MessageBox("ERROR!");
				return FALSE;
			}
		}

		while(TRUE)
		{
			if (!InternetFindNextFile (hDir, &pDirInfo))
			{
				int		nErr = GetLastError();
				SetLastError(0);

				if (ERROR_NO_MORE_FILES == nErr)
				{
					DBGOUT("No more files here\n");
					break;
				}
				else
				{
					// check: error
					MessageBox("ERROR!");
					return FALSE;
				}
			}
			else
			{
				if (strcmp(pDirInfo.cFileName, ".") == 0 || strcmp(pDirInfo.cFileName, "..") == 0 )
				{
					continue;
				}

				if (pDirInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					// check: error! directory가 존재하면 아니 됨!
					MessageBox("ERROR!");
					return FALSE;
				}

				// zip file 이름 해석하여 버전 넣기
				const int SIZE_BUFF = 512;
				char		buff[SIZE_BUFF];
				char		seps[] = "_zip";
				util::strncpy(buff, pDirInfo.cFileName, SIZE_BUFF);

				// from version
				char *token1 = strtok(buff, seps);
				// to version
				char *token2 = strtok(NULL, seps);

				VersionInfo OldVersion(token1);
				VersionInfo NewVersion(token2);

				if(FALSE == OldVersion.IsValidVersionInfo()
					|| FALSE == NewVersion.IsValidVersionInfo()
					|| OldVersion >= NewVersion)
				{// 2006-12-28 by cmkwon, 유효한 업데이트 파일이 아니다

					char szErr[1024];
					sprintf(szErr, "Invalid update version file(%s/%s) !!", i_pPreNode->m_szClientFTPUpdateUploadDir, pDirInfo.cFileName);
					MessageBox(szErr);
					continue;
				}

				VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
				if (pVersion == NULL)
				{
					o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
				}
				else
				{
					if (*pVersion < NewVersion)
					{
						*pVersion = NewVersion;
					}
				}
			}
		}
	}

	if (o_pMapVersionOld2New->empty())
	{
		MessageBox("No Version File in FTP Server");
		return FALSE;
	}

	char *buffer = new char[DOWNLOAD_BUFFER_SIZE];
	DWORD amount_read = 0;
	int	total_read = 0;
	BOOL ret = FALSE;

	///////////////////////////////////////////////////////////////////////////
	// get launcher version
	sprintf(pDirString, "%s/lversion.ver", (LPCSTR)i_pPreNode->m_szLauncherFileUploadPath);
	HINTERNET hLauncherVersionFile
		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);

	amount_read = 0;
	total_read = 0;

	// To ensure all data is retrieved, an application must continue
	// to call the InternetReadFile function until the function returns TRUE
	// and the lpdwNumberOfBytesRead parameter equals zero.
	ret = FALSE;
	while(TRUE)
	{
		ret = InternetReadFile (hLauncherVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
		if (!ret)
		{
			DWORD err;
			char errMsg[1024];
			DWORD errMsgLen = 1024;
			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
			MessageBox(errMsg);
			return FALSE;
		}

		if(0 == amount_read)
		{
			break;
		}
		total_read += amount_read;
	}

	memcpy(buffer + total_read, "\0", 1);

	o_pLauncherVersion->SetVersion(buffer);

	// close file handle
	InternetCloseHandle(hLauncherVersionFile);

	///////////////////////////////////////////////////////////////////////////
	// get delete file list version
	sprintf(pDirString, "%s/dversion.ver", (LPCSTR)i_pPreNode->m_szDeleteFileListUploadPath);
	HINTERNET hDelFileListVersionFile
		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);

	amount_read = 0;
	total_read = 0;

	// To ensure all data is retrieved, an application must continue
	// to call the InternetReadFile function until the function returns TRUE
	// and the lpdwNumberOfBytesRead parameter equals zero.
	ret = FALSE;
	while(ret)
	{
		ret = InternetReadFile (hDelFileListVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
		if(FALSE == ret)
		{// 2006-06-30 by cmkwon
			DWORD err;
			char errMsg[1024];
			DWORD errMsgLen = 1024;
			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
			MessageBox(errMsg);
			return FALSE;
		}
		if(0 == amount_read)
		{// 2006-06-30 by cmkwon
			break;
		}
		total_read += amount_read;
	}

	memcpy(buffer + total_read, "\0", 1);
	if(0 < total_read)
	{
		o_pDeleteFileListVersion->SetVersion(buffer);
	}

	// close file handle
	InternetCloseHandle(hDelFileListVersionFile);


// 2008-06-20 by cmkwon, 현재는 notice 를 사용하지 않으므로 주석처리, 업데이트 필요 없음
// 	///////////////////////////////////////////////////////////////////////////
// 	// get notice.txt version
// 	sprintf(pDirString, "%s/nversion.ver", (LPCSTR)i_pPreNode->m_szNoticeFileUploadPath);
// 	HINTERNET hNoticeVersionFile
// 		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);
// 
// 	amount_read = 0;
// 	total_read = 0;
// 
// 	// To ensure all data is retrieved, an application must continue
// 	// to call the InternetReadFile function until the function returns TRUE
// 	// and the lpdwNumberOfBytesRead parameter equals zero.
// 	ret = FALSE;
// 	while(TRUE)
// 	{
// 		ret = InternetReadFile (hNoticeVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
// 		if(FALSE == ret)
// 		{// 2006-06-30 by cmkwon
// 			DWORD err;
// 			char errMsg[1024];
// 			DWORD errMsgLen = 1024;
// 			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
// 			MessageBox(errMsg);
// 			return FALSE;
// 		}
// 		if(0 == amount_read)
// 		{// 2006-06-30 by cmkwon
// 			break;
// 		}
// 		total_read += amount_read;
// 	}
// 
// 	memcpy(buffer + total_read, "\0", 1);
// 
// 	o_pNoticeVersion->SetVersion(buffer);
// 
// 	// close file handle
// 	InternetCloseHandle(hNoticeVersionFile);

	// delete buffer
	delete buffer;

	// close handles
	InternetCloseHandle(hDir);
	InternetCloseHandle(hFtpConnect);
	InternetCloseHandle(hInternet);

#ifdef _DEBUG
	for (ez_map<VersionInfo, VersionInfo>::iterator itr = o_pMapVersionOld2New->begin(); itr != o_pMapVersionOld2New->end(); itr++)
	{
		VersionInfo tmpOldVersion = itr->first;
		VersionInfo tmpNewVersion = itr->second;
		DBGOUT("  Client Version: %s -> %s\n", tmpOldVersion.GetVersionString(), tmpNewVersion.GetVersionString());
	}
	DBGOUT("  Latest Client Version: %s\n", o_pMapVersionOld2New->rbegin()->second.GetVersionString());
	DBGOUT("\n  Launcher Version: %s\n", o_pLauncherVersion->GetVersionString());
	DBGOUT("  Delete File List Version: %s\n", o_pDeleteFileListVersion->GetVersionString());
	DBGOUT("  Notice Version: %s\n\n", o_pNoticeVersion->GetVersionString());
#endif

	return TRUE;
}


const TCHAR g_szHeaders[] =
	_T("Accept: text/*\r\nUser-Agent: MFC_Tear_Sample\r\n");

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CLeftView::GetVersionInfoByHTTP(CPreServerNode *i_pPreNode, ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
/// \brief		
/// \author		cmkwon
/// \date		2006-12-27 ~ 2006-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CLeftView::GetVersionInfoByHTTP(CPreServerNode *i_pPreNode, ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
{
	char			szRemoteFileName[SIZE_MAX_FTP_FILE_PATH];
	char			szTempFileName[SIZE_MAX_FTP_FILE_PATH];
	CHttpManager	httpMan;
	Err_t			errCode;
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Delete file list version
	util::strncpy(szTempFileName, "tmdversion.ver", SIZE_MAX_FTP_FILE_PATH);
	sprintf(szRemoteFileName, "%s/dversion.ver", i_pPreNode->m_szDeleteFileListDownloadHttpPath);
	errCode = httpMan.DownloadFileByHttp(i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort
		, szRemoteFileName, szTempFileName);
	if(ERR_NO_ERROR != errCode)
	{
		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
		CSystemLogManager errLog;
		if(errLog.InitLogManger(TRUE, "SCMonitor", "./"))
		{
			char szResult[2048];
			util::zero(szResult, 2048);
			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
				, i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort, szRemoteFileName);
 			errLog.WriteSystemLog(szResult);
		}
		return FALSE;
	}

	const int	SIZE_BUFF = 1024;
	char		buffer[SIZE_BUFF];
	{
		ifstream	finD;

		finD.open(szTempFileName);
		if (false == finD.is_open())
		{
			int nErr = GetLastError();
			SetLastError(0);
			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
			return FALSE;
		}
		util::zero(buffer, SIZE_BUFF);
		finD.getline(buffer, SIZE_BUFF);
		if(7 > strlen(buffer))
		{
			buffer[SIZE_BUFF-1] = '\0';
			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
			return FALSE;
		}
		o_pDeleteFileListVersion->SetVersion(buffer);
		finD.close();
	}

// 2008-06-20 by cmkwon, 현재는 notice 를 사용하지 않으므로 주석처리, 업데이트 필요 없음
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2007-01-08 by cmkwon, Notice File Version
// 	util::strncpy(szTempFileName, "tmnversion.ver", SIZE_MAX_FTP_FILE_PATH);
// 	sprintf(szRemoteFileName, "%s/nversion.ver", i_pPreNode->m_szNoticeFileDownloadHttpPath);
// 	errCode = httpMan.DownloadFileByHttp(i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort
// 		, szRemoteFileName, szTempFileName);
// 	if(ERR_NO_ERROR != errCode)
// 	{
// 		return FALSE;
// 	}
// 	{
// 		ifstream finN;
// 		finN.open(szTempFileName);
// 		if (false == finN.is_open())
// 		{
// 			int nErr = GetLastError();
// 			SetLastError(0);
// 			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
// 			return FALSE;
// 		}
// 		util::zero(buffer, SIZE_BUFF);
// 		finN.getline(buffer, SIZE_BUFF);
// 		if(7 > strlen(buffer))
// 		{
// 			buffer[SIZE_BUFF-1] = '\0';
// 			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
// 			return FALSE;
// 		}
// 		o_pNoticeVersion->SetVersion(buffer);
// 		finN.close();
// 	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Launcher File Version
	util::strncpy(szTempFileName, "tmlversion.ver", SIZE_MAX_FTP_FILE_PATH);
	sprintf(szRemoteFileName, "%s/lversion.ver", i_pPreNode->m_szLauncherFileDownloadHttpPath);
	errCode = httpMan.DownloadFileByHttp(i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort
		, szRemoteFileName, szTempFileName);
	if(ERR_NO_ERROR != errCode)
	{
		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
		CSystemLogManager errLog;
		if(errLog.InitLogManger(TRUE, "SCMonitor", "./"))
		{
			char szResult[2048];
			util::zero(szResult, 2048);
			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
				, i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort, szRemoteFileName);
			errLog.WriteSystemLog(szResult);
		}
		return FALSE;
	}

	{
		ifstream finl;
		finl.open(szTempFileName);
		if (false == finl.is_open())
		{
			int nErr = GetLastError();
			SetLastError(0);
			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
			return FALSE;
		}
		util::zero(buffer, SIZE_BUFF);
		finl.getline(buffer, SIZE_BUFF);
		if(7 > strlen(buffer))
		{
			buffer[SIZE_BUFF-1] = '\0';
			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
			return FALSE;
		}
		o_pLauncherVersion->SetVersion(buffer);
		finl.close();
	}


	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon, Version list file
	util::strncpy(szTempFileName, "tmversionlist.ver", SIZE_MAX_FTP_FILE_PATH);
	errCode = httpMan.DownloadFileByHttp(i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort
		, i_pPreNode->m_szVersionListFileDownloadHttpPath, szTempFileName);
	if(ERR_NO_ERROR != errCode)
	{
		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
		CSystemLogManager errLog;
		if(errLog.InitLogManger(TRUE, "SCMonitor", "./"))
		{
			char szResult[2048];
			util::zero(szResult, 2048);
			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
				, i_pPreNode->m_szDownloadHttpServerIP, i_pPreNode->m_nDownloadHttpServerPort, szRemoteFileName);
			errLog.WriteSystemLog(szResult);
		}
		return FALSE;
	}

	{
		ifstream finV;
		finV.open(szTempFileName);
		if (false == finV.is_open())
		{
			int nErr = GetLastError();
			SetLastError(0);
			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
			return FALSE;
		}

		bool bFileEndFlag = false;
		do
		{
			util::zero(buffer, SIZE_BUFF);
			finV.getline(buffer, SIZE_BUFF);
			bFileEndFlag = finV.eof();
		
			char		buffLine[SIZE_BUFF];
			char		seps[] = "_zip";
			util::strncpy(buffLine, buffer, SIZE_BUFF);
			if(0 == strnicmp(buffLine, "#", 1))
			{// 2007-01-09 by cmkwon, 주석임
				continue;
			}

			// from version
			char *token1 = strtok(buffLine, seps);
			// to version
			char *token2 = strtok(NULL, seps);

			if(token1 && token2)
			{
				VersionInfo OldVersion(token1);
				VersionInfo NewVersion(token2);
				if(OldVersion.IsValidVersionInfo()
					&& NewVersion.IsValidVersionInfo()
					&& OldVersion < NewVersion)
				{
					VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
					if (pVersion == NULL)
					{
						o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
					}
					else
					{
						if (*pVersion < NewVersion)
						{
							*pVersion = NewVersion;
						}
					}
				}
			}

		} while(false == bFileEndFlag);
		finV.close();
	}

	return TRUE;
}

void CLeftView::OnTcmReloadVersionInfoPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	ez_map<VersionInfo, VersionInfo> mapVersionOld2New;
	VersionInfo				LatestLauncherVersion;			// launcher 버젼
	VersionInfo				LatestDeleteFileListVersion;	// delete file list 버젼
	VersionInfo				LatestNoticeVersion;			// notice.txt 버젼
	

	if (!GetVersionInfoByHTTP(pNode, &mapVersionOld2New, &LatestLauncherVersion, &LatestDeleteFileListVersion, &LatestNoticeVersion))
	{
		MessageBox("Get version list error by Http!");
		return;
	}


	BYTE		SendBuf[SIZE_MAX_PACKET];

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-09 by cmkwon
	INIT_MSG(MSG_PM_RELOAD_VERSION_INFO_HEADER, T_PM_RELOAD_VERSION_INFO_HEADER, pSHeader, SendBuf);
	LatestLauncherVersion.GetVersion(pSHeader->LauncherVersion);
	LatestDeleteFileListVersion.GetVersion(pSHeader->DeleteFileListVersion);
	LatestNoticeVersion.GetVersion(pSHeader->NoticeVersion);
	pNode->m_pMonitorWinSocket->Write(SendBuf, MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO_HEADER));

	// 2007-01-09 by cmkwon
	INIT_MSG(MSG_PM_RELOAD_VERSION_INFO_LIST, T_PM_RELOAD_VERSION_INFO_LIST, pSList, SendBuf);
	int nNumOfVersions = 0;
	ez_map<VersionInfo, VersionInfo>::iterator itrVersion = mapVersionOld2New.begin();
	while (itrVersion != mapVersionOld2New.end())
	{
		if(MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO_LIST) + (nNumOfVersions+1)*8*sizeof(USHORT) > SIZE_MAX_PACKET)
		{
			pSList->NumOfClientVersions	= nNumOfVersions;
			pNode->m_pMonitorWinSocket->Write( SendBuf, MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO_LIST) + nNumOfVersions*8*sizeof(USHORT) );
			nNumOfVersions				= 0;
		}
		USHORT *pOldClientVersion = (USHORT*)(SendBuf + MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO_LIST) + nNumOfVersions*8*sizeof(USHORT));
		USHORT *pNewClientVersion = (USHORT*)(SendBuf + MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO_LIST) + nNumOfVersions*8*sizeof(USHORT) + 4*sizeof(USHORT));

		VersionInfo tmpOldVersion(itrVersion->first); tmpOldVersion.GetVersion(pOldClientVersion);
		VersionInfo tmpNewVersion(itrVersion->second); tmpNewVersion.GetVersion(pNewClientVersion);
		nNumOfVersions++;

		itrVersion++;
	}
	if(0 < nNumOfVersions)
	{
		pSList->NumOfClientVersions	= nNumOfVersions;
		pNode->m_pMonitorWinSocket->Write( SendBuf, MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO_LIST) + nNumOfVersions*8*sizeof(USHORT) );
		nNumOfVersions				= 0;
	}

	// 2007-01-09 by cmkwon
	pNode->m_pMonitorWinSocket->WriteMessageType(T_PM_RELOAD_VERSION_INFO_DONE);
	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);

// 2007-01-09 by cmkwon, 전송을 3개의 프로토콜로 나누어서 전송하도록 위와 같이 수정
//	char szTemp[1024];
//	int nSendSize = MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO) + mapVersionOld2New.size()*8*sizeof(USHORT);
//	if(nSendSize > SIZE_MAX_PACKET)
//	{
//		sprintf(szTemp, "Update Version List overflow error !!(Version List Counts[%3d], Total Bytes[%4dBytes])\r\n\r\n    Must arrange version list!!"
//			, mapVersionOld2New.size(), nSendSize);
//		MessageBox(szTemp, "ERROR");
//		return;
//	}
//
//	sprintf(szTemp, "Update Version List Infomation(MaxBytes: 1492 Bytes)\r\n\r\n    Version List Counts[%3d], Total Bytes[%4dBytes]"
//		, mapVersionOld2New.size(), nSendSize);
//	MessageBox(szTemp);
//	
//	INIT_MSG_WITH_BUFFER(MSG_PM_RELOAD_VERSION_INFO, T_PM_RELOAD_VERSION_INFO, msgReloadVersion, SendBuf);
//	LatestLauncherVersion.GetVersion(msgReloadVersion->LauncherVersion);
//	LatestDeleteFileListVersion.GetVersion(msgReloadVersion->DeleteFileListVersion);
//	LatestNoticeVersion.GetVersion(msgReloadVersion->NoticeVersion);
//	msgReloadVersion->NumOfClientVersions = mapVersionOld2New.size();
//	int nCount = 0;
//	ez_map<VersionInfo, VersionInfo>::iterator itrVersion = mapVersionOld2New.begin();
//	while (itrVersion != mapVersionOld2New.end())
//	{
//		USHORT *pOldClientVersion = (USHORT*)(SendBuf + MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO) + nCount*8*sizeof(USHORT));
//		USHORT *pNewClientVersion = (USHORT*)(SendBuf + MSG_SIZE(MSG_PM_RELOAD_VERSION_INFO) + nCount*8*sizeof(USHORT) + 4*sizeof(USHORT));
//
//		VersionInfo tmpOldVersion(itrVersion->first); tmpOldVersion.GetVersion(pOldClientVersion);
//		VersionInfo tmpNewVersion(itrVersion->second); tmpNewVersion.GetVersion(pNewClientVersion);
//
//		nCount++;
//		itrVersion++;
//	}
//	pNode->m_pMonitorWinSocket->Write(SendBuf, nSendSize);
//	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmPrintDbg()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocke = pFieldServerNode->m_pMonitorWinSocket;


	char buff[SIZE_MAX_PACKET];
	*(MessageType_t*)buff = (MessageType_t)T_FM_PRINT_DEBUG_MSG;
	MSG_FM_PRINT_DEBUG_MSG *pMsgPrintDbg = (MSG_FM_PRINT_DEBUG_MSG*)(buff + SIZE_FIELD_TYPE_HEADER);
	pMsgPrintDbg->IntParam1 = dlg.m_int1;
	pMsgPrintDbg->IntParam2 = dlg.m_int2;
	pMsgPrintDbg->FloatParam1 = dlg.m_float1;
	pMsgPrintDbg->FloatParam2 = dlg.m_float2;
	util::strncpy(pMsgPrintDbg->StringParam, (LPCTSTR)dlg.m_string, 100);

	pSocke->Write(buff, MSG_SIZE(MSG_FM_PRINT_DEBUG_MSG));
}


BOOL CLeftView::InsertListMonitorSocket(CMonitorWinSocket *pSocket)
{
	if(NULL == pSocket)
	{
		return FALSE;
	}

	m_listMonitorWinSocketPtr.push_back(pSocket);
	return TRUE;
}
BOOL CLeftView::DeleteListMonitorSocket(CMonitorWinSocket *pSocket)
{
	if(NULL == pSocket)
	{
		return FALSE;
	}

	listMonitorWinSocketPtr::iterator itr(m_listMonitorWinSocketPtr.begin());
	while(itr != m_listMonitorWinSocketPtr.end())
	{
		if(*itr == pSocket)
		{
			m_listMonitorWinSocketPtr.remove(pSocket);
			delete pSocket;
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

CMonitorWinSocket * CLeftView::FindMonitorSocket(SOCKET hSocket)
{
	listMonitorWinSocketPtr::iterator itr(m_listMonitorWinSocketPtr.begin());
	while(itr != m_listMonitorWinSocketPtr.end())
	{
		if((*itr)->GetSocket() == hSocket)
		{
			return *itr;
		}
		itr++;
	}

	return NULL;
}

void CLeftView::OnTcmChangeBandwidthflagPre()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorServerNode *pServerNode = (CMonitorServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_PM_CHANGE_BANDWIDTH_FLAG;
	pServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);
}

void CLeftView::OnTcmChangeBandwidthflagField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorServerNode *pServerNode = (CMonitorServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_FM_CHANGE_BANDWIDTH_FLAG;
	pServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);
}

void CLeftView::OnTcmChangeBandwidthflagNpc()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorServerNode *pServerNode = (CMonitorServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_NM_CHANGE_BANDWIDTH_FLAG;
	pServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);
}

void CLeftView::OnTcmChangeBandwidthflagIm()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorServerNode *pServerNode = (CMonitorServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(USHORT*)buff = (USHORT)T_IM_CHANGE_BANDWIDTH_FLAG;
	pServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);
}

// 2006-08-04 by cmkwon, 기능 삭제함
// void CLeftView::OnTcmShutdownPre()
// {
// 	// TODO: Add your command handler code here
// 	if (IDYES != AfxMessageBox("Do you end Pre server really?", MB_YESNO))
// 	{
// 		return;
// 	}
// 
// 	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
// 	CPreServerNode *pServerNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);
// 
// 	char buff[SIZE_MAX_PACKET];
// 	*(USHORT*)buff = (USHORT)T_PM_SHUTDOWN;
// 	pServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);
// 	pServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
// }

void CLeftView::OnTcmConnectIm()
{
	// TODO: Add your command handler code here
	///////////////////////////////////////////////////////////////////////////
	// input server ip, port...
	CFieldServerDlg	dlg(g_szIMServerIPReg, g_szServerIPHistoryReg, IM_SERVER_PORT);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	g_szIMServerIPReg = dlg.m_ipaddr;
	g_nServerPortReg = dlg.m_port;
	g_szServerIPHistoryReg = dlg.m_szIPHistory;
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();

	///////////////////////////////////////////////////////////////////////////
	// Connect to server, send connect
	// if succeeded, insert node to tree and expand
	////////////////////////////////////////
	// Initializing Socket
	CMonitorWinSocket *pMonitorWinSocket = new CMonitorWinSocket(GetSafeHwnd(), this, WM_ASYNC_EVENT, WM_PACKET_NOTIFY);
	if (!pMonitorWinSocket->Connect(dlg.m_ipaddr, dlg.m_port))
	{
		MessageBox("Connect Error!");
		util::del(pMonitorWinSocket);
		return;
	}
	InsertListMonitorSocket(pMonitorWinSocket);

	CIMServerNode *pNode
		= new CIMServerNode(pMonitorWinSocket);

	///////////////////////////////////////////////////////
	// make node
	CTreeCtrl &tctl = GetTreeCtrl();
	HTREEITEM hItem = tctl.InsertItem("Connecting IM Server", 2, 2, m_hIMItem);

	///////////////////////////////////////////////////////
	// associate hItem & socket
	tctl.SetItemData(hItem, (DWORD)pNode);
	pMonitorWinSocket->SetTreeItem(hItem);
}

void CLeftView::OnTcmDisconnectIm()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorWinSocket *pMonitorWinSocket
		= ((CFieldServerNode*)GetTreeCtrl().GetItemData(hItem))->m_pMonitorWinSocket;

	if(pMonitorWinSocket)
	{
		pMonitorWinSocket->CloseSocket();
		DeleteListMonitorSocket(pMonitorWinSocket);
		pMonitorWinSocket = NULL;
	}

	///////////////////////////////////////////////////////////////////////////
	// Delete item
	CMonitorNode *pNode = (CMonitorNode*)GetTreeCtrl().GetItemData(hItem);
	util::del(pNode);
	GetTreeCtrl().DeleteItem(hItem);
}

void CLeftView::OnTcmNumClientsIm()
{
	// TODO: Add your command handler code here

}

void CLeftView::OnTcmPingIm()
{
	// TODO: Add your command handler code here

}

void CLeftView::OnTcmPrintmapNpc()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_NM_PRINTMAP, T_NM_PRINTMAP, pSeMap, buff);
	pSeMap->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
	pSeMap->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;

	pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_NM_PRINTMAP));
}

void CLeftView::OnTcmShutdownIm()
{
	// TODO: Add your command handler code here
	if (IDYES != AfxMessageBox("Do you end IM server really?", MB_YESNO))
	{
		return;
	}
}

void CLeftView::OnTcmShutdownNpc()
{
	// TODO: Add your command handler code here
	if (IDYES != AfxMessageBox("Do you end NPC server really?", MB_YESNO))
	{
		return;
	}
}

void CLeftView::OnTcmPrintDbgIm()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CIMServerNode *pIMServerNode = (CIMServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocke = pIMServerNode->m_pMonitorWinSocket;

	char buff[SIZE_MAX_PACKET];
	*(MessageType_t*)buff = (MessageType_t)T_IM_PRINT_DEBUG_MSG;
	MSG_IM_PRINT_DEBUG_MSG *pMsgPrintDbg = (MSG_IM_PRINT_DEBUG_MSG*)(buff + SIZE_FIELD_TYPE_HEADER);
	pMsgPrintDbg->IntParam1 = dlg.m_int1;
	pMsgPrintDbg->IntParam2 = dlg.m_int2;
	pMsgPrintDbg->FloatParam1 = dlg.m_float1;
	pMsgPrintDbg->FloatParam2 = dlg.m_float2;
	util::strncpy(pMsgPrintDbg->StringParam, (LPCTSTR)dlg.m_string, 100);

	pSocke->Write(buff, MSG_SIZE(MSG_IM_PRINT_DEBUG_MSG));
}

void CLeftView::OnTcmSetMsgPrintLevelField()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocket = pServerNode->m_pMonitorWinSocket;

	INIT_MSG_WITH_BUFFER(MSG_FM_SET_MSG_PRINT_LEVEL, T_FM_SET_MSG_PRINT_LEVEL, pMsgSet, pMsgSetBuf);
	pMsgSet->Level = dlg.m_int1;

	pSocket->Write((char*)pMsgSetBuf, MSG_SIZE(MSG_FM_SET_MSG_PRINT_LEVEL));
}

void CLeftView::OnTcmSetMsgPrintLevelIm()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CIMServerNode *pServerNode = (CIMServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocket = pServerNode->m_pMonitorWinSocket;

	INIT_MSG_WITH_BUFFER(MSG_IM_SET_MSG_PRINT_LEVEL, T_IM_SET_MSG_PRINT_LEVEL, pMsgSet, pMsgSetBuf);
	pMsgSet->Level = dlg.m_int1;

	pSocket->Write((char*)pMsgSetBuf, MSG_SIZE(MSG_IM_SET_MSG_PRINT_LEVEL));
}

void CLeftView::OnTcmSetMsgPrintLevelNpc()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CNPCServerNode *pServerNode = (CNPCServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocket = pServerNode->m_pMonitorWinSocket;

	INIT_MSG_WITH_BUFFER(MSG_NM_SET_MSG_PRINT_LEVEL, T_NM_SET_MSG_PRINT_LEVEL, pMsgSet, pMsgSetBuf);
	pMsgSet->Level = dlg.m_int1;

	pSocket->Write((char*)pMsgSetBuf, MSG_SIZE(MSG_NM_SET_MSG_PRINT_LEVEL));
}

void CLeftView::OnTcmSetMsgPrintLevelPre()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pServerNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocket = pServerNode->m_pMonitorWinSocket;

	INIT_MSG_WITH_BUFFER(MSG_PM_SET_MSG_PRINT_LEVEL, T_PM_SET_MSG_PRINT_LEVEL, pMsgSet, pMsgSetBuf);
	pMsgSet->Level = dlg.m_int1;

	pSocket->Write((char*)pMsgSetBuf, MSG_SIZE(MSG_PM_SET_MSG_PRINT_LEVEL));
}

void CLeftView::OnTcmGetCharacterInfoField()
{
	// TODO: Add your command handler code here
	CPringDBGDlg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);
	CMonitorWinSocket *pSocket = pServerNode->m_pMonitorWinSocket;

	INIT_MSG_WITH_BUFFER(MSG_FM_GET_CHARACTER_INFO, T_FM_GET_CHARACTER_INFO, pMsgGet, pMsgGetBuf);
	memset(pMsgGet, 0x00, sizeof(MSG_FM_GET_CHARACTER_INFO));

	util::strncpy(pMsgGet->CharacterName, dlg.m_string, SIZE_MAX_CHARACTER_NAME);
	if (dlg.m_int1 == 0) { pMsgGet->CharacterUniqueNumber = INVALID_UNIQUE_NUMBER; }
	else { pMsgGet->CharacterUniqueNumber = dlg.m_int1; }
	if (dlg.m_int2 == 0) { pMsgGet->ClientIndex = INVALID_CLIENT_INDEX; }
	else { pMsgGet->ClientIndex = dlg.m_int2; }

	pSocket->Write((char*)pMsgGetBuf, MSG_SIZE(MSG_FM_GET_CHARACTER_INFO));
}

void CLeftView::OnTcmPausePre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(MessageType_t*)buff = T_PM_PAUSE_SERVICE;
	pNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);

	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmStartPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[SIZE_MAX_PACKET];
	*(MessageType_t*)buff = T_PM_START_SERVICE;
	pNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);

	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnFileNew()
{
	// TODO: Add your command handler code here
	CDlgNewServerGroup dlg(TRUE);
	BOOL bQuit;

	do
	{
		bQuit = TRUE;
		if(dlg.DoModal() == IDOK
			&& FALSE == dlg.m_csNewServerGroupName.IsEmpty())
		{
			for(int i=0; i<g_ServerGroupNameVector.size(); i++)
			{
				if(strcmp(dlg.m_csNewServerGroupName, g_ServerGroupNameVector[i].c_str()) == 0)
				{
					AfxMessageBox("A Server Group Name already exists with the name you specified.\nPlease use a different Server Group Name.");
					bQuit = FALSE;
					break;
				}
			}
			if(bQuit)
			{
				g_ServerGroupNameVector.push_back((LPSTR)(LPCSTR)dlg.m_csNewServerGroupName);
				CTreeCtrl &tctl = GetTreeCtrl();
				HTREEITEM hRootItem;
				// level 0, root
				CMonitorNode *pNode = new CMonitorNode("Root Node");
				hRootItem = tctl.InsertItem((LPSTR)(LPCSTR)dlg.m_csNewServerGroupName, 0, 0);
				tctl.SetItemData(hRootItem, (DWORD)pNode);

				// Pre Server
				// level 1
				pNode = new CMonitorNode("PreServer");
				m_hPreItem = GetTreeCtrl().InsertItem("PreServer", 1, 1, hRootItem);
				tctl.SetItemData(m_hPreItem, (DWORD)pNode);
				m_hItemVector.push_back(m_hPreItem);

				// IM Server
				// level 1
				pNode = new CMonitorNode("IMServer");
				m_hIMItem = GetTreeCtrl().InsertItem("IMServer", 1, 1, hRootItem);
				tctl.SetItemData(m_hIMItem, (DWORD)pNode);
				m_hItemVector.push_back(m_hIMItem);

				// Field Server
				// level 1
				pNode = new CMonitorNode("FieldServer");
				m_hFieldItem = GetTreeCtrl().InsertItem("FieldServer", 1, 1, hRootItem);
				tctl.SetItemData(m_hFieldItem, (DWORD)pNode);
				m_hItemVector.push_back(m_hFieldItem);

				// NPC Server
				// level 1
				pNode = new CMonitorNode("NPCServer");
				m_hNPCItem = GetTreeCtrl().InsertItem("NPCServer", 1, 1, hRootItem);
				tctl.SetItemData(m_hNPCItem, (DWORD)pNode);
				m_hItemVector.push_back(m_hNPCItem);

				// Log Server
				// level 1
				pNode = new CMonitorNode("LogServer");
				m_hLogItem = GetTreeCtrl().InsertItem("LogServer", 1, 1, hRootItem);
				tctl.SetItemData(m_hLogItem, (DWORD)pNode);
				m_hItemVector.push_back(m_hLogItem);

				((CAtumMonitorApp*)AfxGetApp())->WriteProfile();
				GetTreeCtrl().Expand(hRootItem, TVE_EXPAND);
			}
		}
	} while(!bQuit);
}

void CLeftView::OnTcmRenameRoot()
{
	// TODO: Add your command handler code here

	CString csName = GetTreeCtrl().GetItemText(m_hRootTempItem);
	CDlgNewServerGroup dlg(FALSE, csName);
	BOOL	bQuit;

	do
	{
		bQuit = TRUE;
		if(dlg.DoModal() == IDOK
			&& FALSE == dlg.m_csNewServerGroupName.IsEmpty())
		{
			if(csName.Compare(dlg.m_csNewServerGroupName) == 0)
			{
				return;
			}
			int i;
			for(i=0; i<g_ServerGroupNameVector.size(); i++)
			{
				if(strcmp(dlg.m_csNewServerGroupName, g_ServerGroupNameVector[i].c_str()) == 0)
				{
					AfxMessageBox("A Server Group Name already exists with the name you specified.\nPlease use a different Server Group Name.");
					bQuit = FALSE;
					break;
				}
			}

			if(bQuit)
			{
				for(i=0; i<g_ServerGroupNameVector.size(); i++)
				{
					if(strcmp(csName, g_ServerGroupNameVector[i].c_str()) == 0)
					{
						GetTreeCtrl().SetItemText(m_hRootTempItem, dlg.m_csNewServerGroupName);
						g_ServerGroupNameVector[i] = dlg.m_csNewServerGroupName;
						((CAtumMonitorApp*)AfxGetApp())->WriteProfile();
						break;
					}
				}
			}
		}
	} while(!bQuit);
}


void CLeftView::OnTcmDeleteRoot()
{
	// TODO: Add your command handler code here
	CTreeCtrl &tctl = GetTreeCtrl();
	if(tctl.GetNextItem(m_hRootItem, TVGN_NEXT) == NULL)
	{
		return;
	}

	CMonitorNode *pNode = NULL;
	CString csName = tctl.GetItemText(m_hRootTempItem);
	if(IDNO == AfxMessageBox("Delete the selected Server Group?", MB_YESNO))
	{
		return;
	}
	vector<string>::iterator	itrV(g_ServerGroupNameVector.begin());
	while(itrV != g_ServerGroupNameVector.end())
	{
		if(strcmp(itrV->c_str(), csName) == 0)
		{
			g_ServerGroupNameVector.erase(itrV);
			break;
		}
		itrV++;
	}

	HTREEITEM hChild = tctl.GetChildItem(m_hRootTempItem);
	for(; hChild != NULL; hChild = tctl.GetNextItem(hChild, TVGN_NEXT))
	{
		vector<HTREEITEM>::iterator itr(m_hItemVector.begin());
		while(itr != m_hItemVector.end())
		{
			if(hChild == *itr)
			{
				m_hItemVector.erase(itr);
				break;
			}
			itr++;
		}
		pNode = (CMonitorNode*)tctl.GetItemData(hChild);
		util::del(pNode);
	}

	if(m_hRootItem == m_hRootTempItem)
	{
		m_hRootItem = tctl.GetNextItem(m_hRootItem, TVGN_NEXT);
	}
	pNode = (CMonitorNode*)tctl.GetItemData(m_hRootTempItem);
	util::del(pNode);
	GetTreeCtrl().DeleteItem(m_hRootTempItem);
	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();
}

void CLeftView::OnTcmGetUserCountsIm()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CMonitorServerNode *pServerNode = (CMonitorServerNode*)GetTreeCtrl().GetItemData(hItem);

	char buff[256];
	*(MessageType_t*)buff = T_IM_GET_TOTAL_USER_COUNTS;
	pServerNode->m_pMonitorWinSocket->Write(buff, SIZE_FIELD_TYPE_HEADER);
}

void CLeftView::OnTcmGetUserCountsField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_FM_GET_MAP_USER_COUNTS, T_FM_GET_MAP_USER_COUNTS, pSendGetMapUser, buff);
	pSendGetMapUser->MapChannIndex.MapIndex		= pProjectNode->m_projectInfo.m_nMapIndex;
	pSendGetMapUser->MapChannIndex.ChannelIndex	= pProjectNode->m_ChannelIndex;
	pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_FM_GET_MAP_USER_COUNTS));
}

void CLeftView::OnTcmGetServerGroupInfoPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CServerGroupInfoNode *pServerGroupNode = (CServerGroupInfoNode*)GetTreeCtrl().GetItemData(hItem);

	if(strncmp(pServerGroupNode->m_ServerGroupInfo.IMServerID.IP, "", SIZE_MAX_IPADDRESS) == 0)
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_PM_GET_SERVER_GROUP_INFO, T_PM_GET_SERVER_GROUP_INFO, pSendGroupInfo, buff);
	util::strncpy(pSendGroupInfo->ServerGroupName, pServerGroupNode->m_ServerGroupInfo.ServerGroupName, SIZE_MAX_SERVER_NAME);
	pServerGroupNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_PM_GET_SERVER_GROUP_INFO));
}

void CLeftView::OnTcmReloadItemInfoField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	MessageType_t type = T_FM_RELOAD_ITEM_INFO;
	pFieldServerNode->m_pMonitorWinSocket->Write((char*)(&type), SIZE_FIELD_TYPE_HEADER);

	pFieldServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmResetNpcConnectionField()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	MessageType_t type = T_FM_RESET_NPC_SERVER;
	pFieldServerNode->m_pMonitorWinSocket->Write((char*)(&type), SIZE_FIELD_TYPE_HEADER);

	pFieldServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmPreSetLimitUserCount()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CServerGroupInfoNode *pServerGroupNode = (CServerGroupInfoNode*)GetTreeCtrl().GetItemData(hItem);

	if(strncmp(pServerGroupNode->m_ServerGroupInfo.IMServerID.IP, "", SIZE_MAX_IPADDRESS) == 0)
	{
		return;
	}

	CPreUserCountDlg	dlg(DIALOG_TYPE_LIMIT_USER, pServerGroupNode->m_ServerGroupInfo.LimitGroupUserCounts);
	if(dlg.DoModal() == IDOK)
	{
		INIT_MSG_WITH_BUFFER(MSG_PM_SET_LIMIT_GROUP_USER_COUNTS, T_PM_SET_LIMIT_GROUP_USER_COUNTS, pSetUserCount, buff);
		util::strncpy(pSetUserCount->ServerGroupName, pServerGroupNode->m_ServerGroupInfo.ServerGroupName, SIZE_MAX_SERVER_NAME);
		pSetUserCount->LimitGroupUserCounts = dlg.m_ctl_uiLimitGroupUserCounts;
		pServerGroupNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_PM_SET_LIMIT_GROUP_USER_COUNTS));
	}
}

void CLeftView::OnTcmPreChangeEnableServerGroup() 
{
	// TODO: Add your command handler code here

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CServerGroupInfoNode *pServerGroupNode = (CServerGroupInfoNode*)GetTreeCtrl().GetItemData(hItem);
	pServerGroupNode->m_ServerGroupInfo.bEnableServerGroup = !(pServerGroupNode->m_ServerGroupInfo.bEnableServerGroup);
	pServerGroupNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());

	INIT_MSG_WITH_BUFFER(MSG_PM_CHANGE_ENABLE_SERVER_GROUP, T_PM_CHANGE_ENABLE_SERVER_GROUP, pSMsg, SendBuf);
	util::strncpy(pSMsg->ServerGroupName, pServerGroupNode->m_ServerGroupInfo.ServerGroupName, SIZE_MAX_SERVER_NAME);
	pSMsg->bEnableServerGroup = pServerGroupNode->m_ServerGroupInfo.bEnableServerGroup;
	pServerGroupNode->m_pMonitorWinSocket->Write(SendBuf, MSG_SIZE(MSG_PM_CHANGE_ENABLE_SERVER_GROUP));
}

void CLeftView::OnTcmGetUserCountsNpc()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_NM_GET_MAP_USER_COUNTS, T_NM_GET_MAP_USER_COUNTS, pSendGetMapUser, buff);
	pSendGetMapUser->MapChannIndex.MapIndex		= pProjectNode->m_projectInfo.m_nMapIndex;
	pSendGetMapUser->MapChannIndex.ChannelIndex	= pProjectNode->m_ChannelIndex;
	pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_NM_GET_MAP_USER_COUNTS));
}

void CLeftView::OnTcmNpcSetLimitMonsterCount()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	CPreUserCountDlg	dlg(DIALOG_TYPE_LIMIT_MONSTER, pProjectNode->m_uiLimitMonsterCount);
	if(dlg.DoModal() == IDOK)
	{
		INIT_MSG_WITH_BUFFER(MSG_NM_SET_LIMIT_MONSTER_COUNT, T_NM_SET_LIMIT_MONSTER_COUNT, pSetMonsterCount, buff);
		pSetMonsterCount->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
		pSetMonsterCount->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;

		pProjectNode->m_uiLimitMonsterCount = dlg.m_ctl_uiLimitGroupUserCounts;
		pSetMonsterCount->SetLimitMonsterCount = dlg.m_ctl_uiLimitGroupUserCounts;
		pProjectNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_NM_SET_LIMIT_MONSTER_COUNT));
	}
}

void CLeftView::OnInfoGen()
{
	// TODO: Add your command handler code here
	CInfoGenDlg dlg;
	dlg.m_szInfoGenOutPath = ((CAtumMonitorApp*)AfxGetApp())->m_szInfoGenOutPath;
	dlg.DoModal();

}

void CLeftView::OnSqlReader()
{
	// TODO: Add your command handler code here
	CSQLReaderDlg dlg;
	dlg.m_szSQLFilePath = "D:\\0000_Projects(Masang_Kor)_Dev\\Database\\000_Main\\atum_1_create_table_account.sql";
	dlg.DoModal();
}

void CLeftView::OnToolsStopNoticePlaysound()
{
	// TODO: Add your command handler code here
	sndPlaySound(NULL, SND_ASYNC|SND_LOOP);
	m_bNoticePlaySound = FALSE;
}

void CLeftView::OnToolsSetNoticeFilePath()
{
	// TODO: Add your command handler code here
	CMyFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST,
		"Wave Files(*.wav)|*.wav||",
		this);

	if(dlg.DoModal() == IDOK )
	{
		((CAtumMonitorApp*)AfxGetApp())->m_cstrNoticeFilePath = dlg.GetPathName();
		((CAtumMonitorApp*)AfxGetApp())->WriteProfile();
	}
}

void CLeftView::OnTcmReloadBlockedAccountPre()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	pNode->m_pMonitorWinSocket->WriteMessageType(T_PM_RELOAD_BLOCKED_ACCOUNTS);

	pNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);

}

void CLeftView::OnClientFtpUpdateManager() 
{
	// TODO: Add your command handler code here
	CClientFTPUpdateManagerDlg dlg;
	dlg.ReadProfile();
	dlg.DoModal();
}

void CLeftView::OnTcmReloadQuestInfoField() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode = (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_FM_RELOAD_SERVER_INFO, T_FM_RELOAD_SERVER_INFO, msgReloadServerInfo, msgReloadServerInfoBuf);
	msgReloadServerInfo->InfoType = SERVER_INFO_TYPE_QUEST;
	pFieldServerNode->m_pMonitorWinSocket->Write(msgReloadServerInfoBuf, MSG_SIZE(MSG_FM_RELOAD_SERVER_INFO));

	pFieldServerNode->m_pMonitorWinSocket->SetTimeOut(TIMEOUT);
}

void CLeftView::OnTcmFieldEnableChannel() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_FM_SET_CHANNEL_STATE, T_FM_SET_CHANNEL_STATE, pSetState, pSetStateBuf);
	pSetState->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
	pSetState->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;
	pSetState->EnableChannel = TRUE;

	pProjectNode->m_pMonitorWinSocket->Write(pSetStateBuf, MSG_SIZE(MSG_FM_SET_CHANNEL_STATE));
}

void CLeftView::OnTcmFieldDisableChannel() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_FM_SET_CHANNEL_STATE, T_FM_SET_CHANNEL_STATE, pSetState, pSetStateBuf);
	pSetState->MapChannelIndex.MapIndex = pProjectNode->m_projectInfo.m_nMapIndex;
	pSetState->MapChannelIndex.ChannelIndex = pProjectNode->m_ChannelIndex;
	pSetState->EnableChannel = FALSE;

	pProjectNode->m_pMonitorWinSocket->Write(pSetStateBuf, MSG_SIZE(MSG_FM_SET_CHANNEL_STATE));
}

void CLeftView::OnZipFileGen() 
{
	// TODO: Add your command handler code here
	CZipfileDlg dlg;
	//dlg.m_szInfoGenOutPath = ((CAtumMonitorApp*)AfxGetApp())->m_szInfoGenOutPath;
	dlg.DoModal();
}

void CLeftView::OnTcmGetUserMonsterCount() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CFieldServerNode *pFieldServerNode
		= (CFieldServerNode*)GetTreeCtrl().GetItemData(hItem);

	if(pFieldServerNode)
	{
		list<CChannelInfoNode*>::iterator itr = pFieldServerNode->m_listChannelInfoNodePtr.begin();
		while (itr != pFieldServerNode->m_listChannelInfoNodePtr.end())
		{
			CChannelInfoNode *pChannelInfoNode = *itr;

			INIT_MSG_WITH_BUFFER(MSG_FM_GET_MAP_USER_COUNTS, T_FM_GET_MAP_USER_COUNTS, pSendGetMapUser, buff);
			pSendGetMapUser->MapChannIndex.MapIndex		= pChannelInfoNode->m_projectInfo.m_nMapIndex;
			pSendGetMapUser->MapChannIndex.ChannelIndex	= pChannelInfoNode->m_ChannelIndex;
			pChannelInfoNode->m_pMonitorWinSocket->Write(buff, MSG_SIZE(MSG_FM_GET_MAP_USER_COUNTS));

			itr++;
		}

		pFieldServerNode->m_pMonitorWinSocket->WriteMessageType(T_FM_GET_NUM_CLIENTS);
	}
}

void CLeftView::OnTcmSetMgameEvent() 
{
	//// TODO: Add your command handler code here
	//HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	//CPreServerNode *pPServerNode
	//	= (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	//CSetMGameEventTypeDlg dlg(pPServerNode->m_enMGameEventType);
	//if (dlg.DoModal() != IDOK)
	//{
	//	return;
	//}

	//if(pPServerNode->m_enMGameEventType == dlg.m_enEventType)
	//{
	//	return;
	//}
	//pPServerNode->m_enMGameEventType = dlg.m_enEventType;
	//pPServerNode->DisplayNode((CAtumMonitorView*)((CMainFrame*)AfxGetMainWnd())->GetRightPane());
	//	

	//INIT_MSG_WITH_BUFFER(MSG_PM_SET_MGAME_EVENT_TYPE, T_PM_SET_MGAME_EVENT_TYPE, pSend, Buff);
	//pSend->enMGameEvent = pPServerNode->m_enMGameEventType;
	//pPServerNode->m_pMonitorWinSocket->Write(Buff, MSG_SIZE(MSG_PM_SET_MGAME_EVENT_TYPE));
}



void CLeftView::OnTcmFieldCitywarChangeTime() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CChannelInfoNode *pProjectNode = (CChannelInfoNode*)GetTreeCtrl().GetItemData(hItem);

	if(FALSE == pProjectNode->m_bCityWarMap)
	{
		AfxMessageBox("This Map is not CityWar Map!!");
		return;
	}
	
	CSetCityWarTimeDlg dlg(pProjectNode, this);
	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}
	
	if(pProjectNode->m_cityWarOccupyInfo.CityWarDefaultTime == dlg.m_ATimeDefaultTime
		&& pProjectNode->m_cityWarOccupyInfo.CityWarSettingTime == dlg.m_ATimeSettingTime)
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FM_CITYWAR_CHANGE_WAR_TIME, T_FM_CITYWAR_CHANGE_WAR_TIME, pSWarTime, SendBuf);
	pSWarTime->MapChannelIndex.MapIndex		= pProjectNode->m_projectInfo.m_nMapIndex;
	pSWarTime->MapChannelIndex.ChannelIndex	= 0;
	pSWarTime->CityWarATimeDefault			= dlg.m_ATimeDefaultTime;
	pSWarTime->CityWarATimeSetting			= dlg.m_ATimeSettingTime;
	pProjectNode->m_pMonitorWinSocket->Write(SendBuf, MSG_SIZE(MSG_FM_CITYWAR_CHANGE_WAR_TIME));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CLeftView::OnTcmPrepareShutdown()
/// \brief		
/// \author		cmkwon
/// \date		2006-08-04 ~ 2006-08-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CLeftView::OnTcmPrepareShutdown() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CPreServerNode *pPSNode = (CPreServerNode*)GetTreeCtrl().GetItemData(hItem);

	INIT_MSG_WITH_BUFFER(MSG_PM_PREPARE_SHUTDOWN, T_PM_PREPARE_SHUTDOWN, pSShutdown, SendBuf);
	list<CServerGroupInfoNode*>::iterator itr(pPSNode->m_listServerGroupInfoNodePtr.begin());
	for(;itr != pPSNode->m_listServerGroupInfoNodePtr.end(); itr++)
	{
		CServerGroupInfoNode *pSGInfoNode = *itr;
		if(pSGInfoNode->IsActiveServerGroup())
		{
			util::strncpy(pSShutdown->ServerGroupName, pSGInfoNode->m_ServerGroupInfo.ServerGroupName, SIZE_MAX_SERVER_NAME);
			pPSNode->m_pMonitorWinSocket->Write(SendBuf, MSG_SIZE(MSG_PM_PREPARE_SHUTDOWN));
		}
	}
}

void CLeftView::OnTcm3PrepareShutdown() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	CServerGroupInfoNode *pServerGroupNode = (CServerGroupInfoNode*)GetTreeCtrl().GetItemData(hItem);

	if(FALSE == pServerGroupNode->IsActiveServerGroup())
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_PM_PREPARE_SHUTDOWN, T_PM_PREPARE_SHUTDOWN, pSShutdown, SendBuf);
	util::strncpy(pSShutdown->ServerGroupName, pServerGroupNode->m_ServerGroupInfo.ServerGroupName, SIZE_MAX_SERVER_NAME);
	pServerGroupNode->m_pMonitorWinSocket->Write(SendBuf, MSG_SIZE(MSG_PM_PREPARE_SHUTDOWN));
}

/*void CLeftView::OnImportExcelfile() 
{
	// TODO: Add your command handler code here
	CImportExcelDlg dlg;
	
	if(dlg.DoModal() != IDOK){
		return;
	}
}*/
