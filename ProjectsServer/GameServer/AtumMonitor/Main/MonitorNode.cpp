// MonitorNode.cpp: implementation of the CMonitorServerNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumMonitor.h"
#include "MonitorNode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _STRMSG_S_SCMONITOR_0010 "%Y-%m-%d %H:%M:%S"


CMonitorNode	*CMonitorNode::ms_pDispayNode = NULL;

void CMonitorNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();

	LV_ITEM	lvItem;
	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	lvItem.pszText = "Name";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szName);

	return;
}

void CMonitorServerNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();

	return;
}

CFieldServerNode::~CFieldServerNode()
{
	CChannelInfoNode *pNode;
	while(m_listChannelInfoNodePtr.empty() == false)
	{
		pNode = *m_listChannelInfoNodePtr.begin();
		m_listChannelInfoNodePtr.pop_front();
		util::del(pNode);
	}

	m_pMonitorTickManager->CleanTickManager();
	util::del(m_pMonitorTickManager);
}

void CFieldServerNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();
	pCtrlList.SetColumnWidth(0, 230);
	pCtrlList.SetColumnWidth(1, 260);

	char szBuff[40];
	LV_ITEM	lvItem;

	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	lvItem.pszText = "Loaded Map Count";
	sprintf(szBuff, "%d", m_nLoadedMapCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Port";
	sprintf(szBuff, "%d", m_nPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "IP Address";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szIP);

	lvItem.pszText = "CalcBandwidth";
	sprintf(szBuff, "%d", m_bFlagCalcBandWidth);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "# of Clients";
	sprintf(szBuff, "%d", m_nClients);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Started Time";
	strftime(szBuff, 128, _STRMSG_S_SCMONITOR_0010, localtime(&m_tmStartedTime));
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Name";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szServerName);
}

void CFieldServerNode::InsertChannelInfoNodePtr(CChannelInfoNode *pNode)
{
	if(NULL == pNode){		return;}

	m_listChannelInfoNodePtr.push_back(pNode);
}

CChannelInfoNode *CFieldServerNode::GetChannelInfoNode(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	list<CChannelInfoNode*>::iterator itr(m_listChannelInfoNodePtr.begin());
	while(itr != m_listChannelInfoNodePtr.end())
	{
		if(i_mapChannIdx.MapIndex == (*itr)->m_projectInfo.m_nMapIndex
			&& i_mapChannIdx.ChannelIndex == (*itr)->m_ChannelIndex)
		{
			return (*itr);
		}
		itr++;
	}
	return NULL;
}

void CIMServerNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();
	pCtrlList.DeleteAllItems();
	pCtrlList.SetColumnWidth(0, 230);
	pCtrlList.SetColumnWidth(1, 260);

	char szBuff[40];
	LV_ITEM	lvItem;

	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	vector<MSG_FIELDSERVER_USERCOUNTS>::iterator itr(m_FieldServerCountsVector.begin());
	while(itr != m_FieldServerCountsVector.end())
	{
		lvItem.pszText = "";
		sprintf(szBuff, "Current Field Monster Count(%d)", itr->CurrentFieldMonsterCounts);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "";
		sprintf(szBuff, "Accumulated Field Monster Count(%d)", itr->AccumulatedFieldMonsterCounts);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "";
		sprintf(szBuff, "Max Field User Count(%d)", itr->MaxFieldUserCounts);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "";
		sprintf(szBuff, "Current Field User Count(%d)", itr->CurrentFieldUserCounts);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "";
		sprintf(szBuff, "Accumulated Field User Count(%d)", itr->AccumulatedFieldUserCounts);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "Field Server";
		sprintf(szBuff, "IP(%s) Port(%d)", itr->FieldIP, itr->FieldListenPort);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		itr++;
	}

	lvItem.pszText = "";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, "");

	lvItem.pszText = "Max Total User Count";
	sprintf(szBuff, "%d", m_MaxTotalUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Current Total User Count";
	sprintf(szBuff, "%d", m_CurrentTotalUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Accumulated Total User Count";
	sprintf(szBuff, "%d", m_AccumulatedTotalUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Port";
	sprintf(szBuff, "%d", m_nPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "IP Address";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szIP);

	lvItem.pszText = "CalcBandwidth";
	sprintf(szBuff, "%d", m_bFlagCalcBandWidth);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "# of Clients";
	sprintf(szBuff, "%d", m_nClients);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Started Time";
	strftime(szBuff, 128, _STRMSG_S_SCMONITOR_0010, localtime(&m_tmStartedTime));
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Name";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szServerName);
}

void CPreServerNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();
	pCtrlList.SetColumnWidth(0, 230);
	pCtrlList.SetColumnWidth(1, 260);

	char szBuff[1024];
	LV_ITEM	lvItem;

	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon
	lvItem.pszText = "";
	sprintf(szBuff, "HTTP Notice File Download Path       ==> %s", m_szNoticeFileDownloadHttpPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	sprintf(szBuff, "HTTP Delete File List Download Path         ==> %s", m_szDeleteFileListDownloadHttpPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	sprintf(szBuff, "HTTP Launcher Download Path                 ==> %s", m_szLauncherFileDownloadHttpPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
	
	lvItem.pszText = "";
	sprintf(szBuff, "HTTP Client Version List File Download Path ==> %s", m_szVersionListFileDownloadHttpPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
	
	lvItem.pszText = "";
	sprintf(szBuff, "HTTP Client Download Dir                    ==> %s", m_szClientHttpUpdateDownloadDir);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
	
	lvItem.pszText = "";
	sprintf(szBuff, "HTTP Download Account                       ==> %s:%s", m_szDownloadHttpServerAccountName, m_szDownloadHttpServerPassword);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "HTTP Server setting";
	sprintf(szBuff, "HTTP Download Server                        ==> %s:%d", m_szDownloadHttpServerIP, m_nDownloadHttpServerPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon
	lvItem.pszText = "";
	sprintf(szBuff, "FTP Notice File Upload Path                 ==> %s", m_szNoticeFileUploadPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	sprintf(szBuff, "FTP Delete File List Upload Path            ==> %s", m_szDeleteFileListUploadPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	sprintf(szBuff, "FTP Launcher Upload Path                    ==> %s", m_szLauncherFileUploadPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
	
	lvItem.pszText = "";
	sprintf(szBuff, "FTP Client Version List File Upload Path    ==> %s", m_szVersionListFileUploadPath);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
	
	lvItem.pszText = "";
	sprintf(szBuff, "FTP Client Upload Dir                       ==> %s", m_szClientFTPUpdateUploadDir);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
	
	lvItem.pszText = "";
	sprintf(szBuff, "FTP Upload Account                          ==> %s:%s", m_szFtpAccountName, m_szFtpPassword);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "FTP Server setting";
	sprintf(szBuff, "FTP Upload Server                           ==> %s:%d", m_szFtpIP, m_nFtpPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, "");
	
	lvItem.pszText = "Auto Update Server Type";

	strcpy(szBuff, "HTTP");

	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, "");

	lvItem.pszText = "";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, "");

	lvItem.pszText = "Port";
	sprintf(szBuff, "%d", m_nPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "IP Address";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szIP);

	lvItem.pszText = "CalcBandwidth";
	sprintf(szBuff, "%d", m_bFlagCalcBandWidth);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "# of Clients";
	sprintf(szBuff, "%d", m_nClients);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Started Time";
	strftime(szBuff, 128, _STRMSG_S_SCMONITOR_0010, localtime(&m_tmStartedTime));
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Name";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szServerName);
}

CNPCServerNode::~CNPCServerNode()
{
	CChannelInfoNode *pNode;
	while(m_listChannelInfoNodePtr.empty() == false)
	{
		pNode = *m_listChannelInfoNodePtr.begin();
		m_listChannelInfoNodePtr.pop_front();
		util::del(pNode);
	}
}

void CNPCServerNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();
	pCtrlList.SetColumnWidth(0, 230);
	pCtrlList.SetColumnWidth(1, 260);

	char szBuff[40];
	LV_ITEM	lvItem;

	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	lvItem.pszText = "Loaded Map Count";
	sprintf(szBuff, "%d", m_nLoadedMapCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Port";
	sprintf(szBuff, "%d", m_nPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "IP Address";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szIP);

	lvItem.pszText = "CalcBandwidth";
	sprintf(szBuff, "%d", m_bFlagCalcBandWidth);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "# of Clients";
	sprintf(szBuff, "%d", m_nClients);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Started Time";
	strftime(szBuff, 128, _STRMSG_S_SCMONITOR_0010, localtime(&m_tmStartedTime));
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Name";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_szServerName);
}

void CNPCServerNode::InsertChannelInfoNodePtr(CChannelInfoNode *pNode)
{
	if(NULL == pNode){		return;}

	m_listChannelInfoNodePtr.push_back(pNode);
}

CChannelInfoNode *CNPCServerNode::GetChannelInfoNode(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	list<CChannelInfoNode*>::iterator itr(m_listChannelInfoNodePtr.begin());
	while(itr != m_listChannelInfoNodePtr.end())
	{
		if(i_mapChannIdx.MapIndex == (*itr)->m_projectInfo.m_nMapIndex
			&& i_mapChannIdx.ChannelIndex == (*itr)->m_ChannelIndex)
		{
			return (*itr);
		}
		itr++;
	}
	return NULL;
}

void CChannelInfoNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();

	LV_ITEM	lvItem;
	char szBuff[40];

	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	if(m_bCityWarMap)
	{
		lvItem.pszText = "ShopBriefing";
		sprintf(szBuff, "%s", m_cityWarOccupyInfo.szBriefing);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "DefenseCounts";
		sprintf(szBuff, "%d", m_cityWarOccupyInfo.nDefenseCounts);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "Setting Time for next CityWar";
		sprintf(szBuff, "%s", m_cityWarOccupyInfo.CityWarSettingTime.GetDateTimeString().GetBuffer());
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "Default Time for next CityWar";
		sprintf(szBuff, "%s", m_cityWarOccupyInfo.CityWarDefaultTime.GetDateTimeString().GetBuffer());
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

		lvItem.pszText = "Tex Percent";
		sprintf(szBuff, "%4.1f", m_cityWarOccupyInfo.fTexPercent);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
		

		lvItem.pszText = "Occupation Guild";
		sprintf(szBuff, "%s", m_cityWarOccupyInfo.OccupyGuildName);
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
		
		///////////////////////////////////////////////////////////////////////////////
		// 綴還 2還
		lvItem.pszText = NULL;
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, NULL);
		lvItem.pszText = NULL;
		pCtrlList.InsertItem(&lvItem);
		pCtrlList.SetItemText(lvItem.iItem, 1, NULL);
	}

	lvItem.pszText = "Limit Monster Count";
	sprintf(szBuff, "%d", m_uiLimitMonsterCount);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	///////////////////////////////////////////////////////////////////////////////
	// 綴還 1還
	lvItem.pszText = NULL;
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, NULL);

	lvItem.pszText = "Current Monster Count";
	sprintf(szBuff, "%d", m_uiCurrentMonsterCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Accumulated Monster Count";
	sprintf(szBuff, "%d", m_uiCurrentMonsterCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Max User Count";
	sprintf(szBuff, "%d", m_uiMaxUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Current User Count";
	sprintf(szBuff, "%d", m_uiCurrentUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Accumulated User Count";
	sprintf(szBuff, "%d", m_uiAccumulatedUserCounts);
	pCtrlList.InsertItem(&lvItem);
 	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = NULL;
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, NULL);

	lvItem.pszText = "CityWarpTargetPosition";
	sprintf(szBuff, "(%4d, %4d, %4d)", (int)m_projectInfo.m_CityWarpTargetPosition.x
		, (int)m_projectInfo.m_CityWarpTargetPosition.y, (int)m_projectInfo.m_CityWarpTargetPosition.z);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Monster Impact Distance";
	sprintf(szBuff, "%d", (int)m_projectInfo.m_fFrontPositionDistance);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Max User Count for Map";
	sprintf(szBuff, "%d", m_projectInfo.m_dwMaxUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

// 2005-06-22 by cmkwon
//	lvItem.pszText = "Tutorial Map";
//	sprintf(szBuff, "%d", m_projectInfo.m_bTutorialMap);
//	pCtrlList.InsertItem(&lvItem);
//	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Auto DP Recovery";
	sprintf(szBuff, "%d", m_projectInfo.m_bAutoDPRecovery);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Dead Default Map Name";
	sprintf(szBuff, "%4d", m_projectInfo.m_DeadDefaultMapIndex);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);


	lvItem.pszText = NULL;
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, NULL);

	lvItem.pszText = "NPC Server UDP Port";
	sprintf(szBuff, "%d", m_projectInfo.m_sNPCUDPPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "NPC Server Listening Port";
	sprintf(szBuff, "%d", m_projectInfo.m_sNPCListenPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "NPC Server IP";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_projectInfo.m_strNPCIP);

	lvItem.pszText = "Field Server UDP Port";
	sprintf(szBuff, "%d", m_projectInfo.m_sFieldUDPPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Field Server Listening Port";
	sprintf(szBuff, "%d", m_projectInfo.m_sFieldListenPort);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Field Server IP";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_projectInfo.m_strFieldIP);

	lvItem.pszText = NULL;
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, NULL);
	
	lvItem.pszText = "Map Water Height";
	sprintf(szBuff, "%d", m_projectInfo.m_sWaterAltitude);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Map Altitude limit";
	sprintf(szBuff, "Max(4%d), Min(%4d)", m_projectInfo.m_sMaximumAltitude, m_projectInfo.m_sMinimumAltitude);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Map Size(tiles)";
	sprintf(szBuff, "X(%4d), Y(%4d)", m_projectInfo.m_sXSize, m_projectInfo.m_sYSize);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Channel Index / Total Channel Count";
	sprintf(szBuff, "%d/%d", m_ChannelIndex, m_projectInfo.m_nCreateChannelCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Map Name";
	sprintf(szBuff, "%04d", m_projectInfo.m_nMapIndex);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);
}

CPreServerNode::~CPreServerNode()
{
	CServerGroupInfoNode *pNode;
	while(m_listServerGroupInfoNodePtr.empty() == false)
	{
		pNode = *m_listServerGroupInfoNodePtr.begin();
		m_listServerGroupInfoNodePtr.pop_front();
		util::del(pNode);
	}
}

void CPreServerNode::InsertServerGroupInfoNodePtr(CServerGroupInfoNode *pNode)
{
	if(NULL == pNode){		return;}
	m_listServerGroupInfoNodePtr.push_back(pNode);
}

void CServerGroupInfoNode::DisplayNode(CAtumMonitorView* pView)
{
	ms_pDispayNode = this;
	CListCtrl& pCtrlList = pView->GetListCtrl();

	pCtrlList.DeleteAllItems();

	LV_ITEM	lvItem;
	char szBuff[40];

	lvItem.iItem = pCtrlList.GetItemCount();
	lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvItem.iSubItem = 0;
	lvItem.iImage = 1;
	lvItem.stateMask = LVIS_STATEIMAGEMASK;
	lvItem.state = INDEXTOSTATEIMAGEMASK(1);

	lvItem.pszText = "Limit Server Group User Count";
	sprintf(szBuff, "%d", m_ServerGroupInfo.LimitGroupUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Enable Server Group";
	pCtrlList.InsertItem(&lvItem);
	if(FALSE == m_ServerGroupInfo.bEnableServerGroup)
	{
		pCtrlList.SetItemText(lvItem.iItem, 1, "FALSE");
	}
	else
	{
		pCtrlList.SetItemText(lvItem.iItem, 1, "TRUE");
	}	

	lvItem.pszText = "";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, "");

	lvItem.pszText = "Max Server Group User Count";
	sprintf(szBuff, "%d", m_ServerGroupInfo.MaxGroupUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Current Server Group User Count";
	sprintf(szBuff, "%d", m_ServerGroupInfo.CurrentGroupUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Accumulated Server Group User Count";
	sprintf(szBuff, "%d", m_ServerGroupInfo.AccumulatedGroupUserCounts);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "IM Server Info";
	sprintf(szBuff, "IP(%s), Port(%d)", m_ServerGroupInfo.IMServerID.IP, m_ServerGroupInfo.IMServerID.port);
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, szBuff);

	lvItem.pszText = "Server Group Name";
	pCtrlList.InsertItem(&lvItem);
	pCtrlList.SetItemText(lvItem.iItem, 1, m_ServerGroupInfo.ServerGroupName);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CServerGroupInfoNode::IsActiveServerGroup(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-04 ~ 2006-08-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CServerGroupInfoNode::IsActiveServerGroup(void)
{
	if(0 == strncmp(this->m_ServerGroupInfo.IMServerID.IP, "", SIZE_MAX_IPADDRESS))
	{
		return FALSE;
	}

	return TRUE;
}