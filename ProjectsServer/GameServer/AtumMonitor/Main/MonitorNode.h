// MonitorNode.h: interface for the CMonitorServerNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORNODE_H__59C90551_5406_4A85_BA1D_98C6053CF888__INCLUDED_)
#define AFX_MONITORNODE_H__59C90551_5406_4A85_BA1D_98C6053CF888__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	SIZE_MAX_NODE_NAME 20

#include "AtumMonitorView.h"
#include "MonitorWinSocket.h"
#include "MonitorTickManager.h"
#include "AtumProtocol.h"

enum EnumProtocolType
{
	TCP_ENCODED = 0,
	TCP_NOT_ENCODED,
	UDP_ENCODED,
	UDP_NOT_ENCODED
};

enum MonitorNodeType
{
	NODE_TYPE_NONE,
	NODE_TYPE_FIELD_SERVER
};

class CMonitorNode
{
public:
	CMonitorNode(char* n)
	{
		util::strncpy(m_szName, n, SIZE_MAX_NODE_NAME);
		m_NodeType = NODE_TYPE_NONE;
	}
	virtual ~CMonitorNode() {}

	virtual void DisplayNode(CAtumMonitorView* pView);

public:
	char		m_szName[SIZE_MAX_NODE_NAME];
	MonitorNodeType	m_NodeType;

	static CMonitorNode	*ms_pDispayNode;
};

class CMonitorServerNode : public CMonitorNode
{
protected:
	CMonitorServerNode(CMonitorWinSocket *pWinSocket)
		: CMonitorNode("Unknown")
	{
		m_szServerName[0] = NULL;
		time(&m_tmStartedTime);
		m_szIP[0] = NULL;
		m_nPort = 0;
		m_pMonitorWinSocket = pWinSocket;
	}

	CMonitorServerNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port)
		: CMonitorNode("Unknown")
	{
		util::strncpy(m_szServerName, name, SIZE_MAX_SERVER_NAME);
		util::strncpy(m_szIP, ip, SIZE_MAX_IPADDRESS);
		m_nPort = port;
		m_pMonitorWinSocket = pWinSocket;
	}
public:
	virtual ~CMonitorServerNode() {}

	virtual void DisplayNode(CAtumMonitorView* pView);

public:
	char				m_szServerName[SIZE_MAX_SERVER_NAME];
	time_t				m_tmStartedTime;
	char				m_szIP[SIZE_MAX_IPADDRESS];
	int					m_nPort;
	BOOL				m_bFlagCalcBandWidth;
	CMonitorWinSocket	*m_pMonitorWinSocket;
};

class CChannelInfoNode;
class CFieldServerNode : public CMonitorServerNode
{
public:
	CFieldServerNode(CMonitorWinSocket *pWinSocket)
		:CMonitorServerNode(pWinSocket)
	{
		m_NodeType = NODE_TYPE_FIELD_SERVER;
		m_pMonitorTickManager = new CMonitorTickManager;
		m_pMonitorTickManager->InitTickManager(this);
		m_pMonitorTickManager->SetTickInterval(1000);		// 04-07-09, cmkwon 종료시 Sleep 시간이 길면 문제 발생
	}

	CFieldServerNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port, int client)
		: CMonitorServerNode(pWinSocket, name, ip, port), m_nClients(client)
	{
		m_NodeType = NODE_TYPE_FIELD_SERVER;
		m_pMonitorTickManager = new CMonitorTickManager;
		m_pMonitorTickManager->InitTickManager(this);
		m_pMonitorTickManager->SetTickInterval(1000);		// 04-07-09, cmkwon 종료시 Sleep 시간이 길면 문제 발생
	}

	virtual ~CFieldServerNode();

	void DisplayNode(CAtumMonitorView* pView);
	void SetData(char *name, time_t startedTime, char *ip, int port, int client, int mapCount, BOOL flagBandwidth)
	{
		util::strncpy(m_szServerName, name, SIZE_MAX_SERVER_NAME);
		m_tmStartedTime = startedTime;
		util::strncpy(m_szIP, ip, SIZE_MAX_IPADDRESS);
		m_nPort = port;
		m_nClients = client;
		m_bFlagCalcBandWidth = flagBandwidth;
		m_nLoadedMapCounts = mapCount;
	}

	void InsertChannelInfoNodePtr(CChannelInfoNode *pNode);
	CChannelInfoNode *GetChannelInfoNode(MAP_CHANNEL_INDEX i_mapChannIdx);

public:
	int		m_nClients;
	int		m_nLoadedMapCounts;
	list<CChannelInfoNode*>		m_listChannelInfoNodePtr;

	CMonitorTickManager			*m_pMonitorTickManager;
};

class CIMServerNode : public CMonitorServerNode
{
public:
	CIMServerNode(CMonitorWinSocket *pWinSocket)
		:CMonitorServerNode(pWinSocket)
	{
		m_AccumulatedTotalUserCounts = 0;
		m_CurrentTotalUserCounts = 0;
		m_MaxTotalUserCounts = 0;		
	}

	CIMServerNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port, int client)
		: CMonitorServerNode(pWinSocket, name, ip, port), m_nClients(client)
	{
	}
	virtual ~CIMServerNode() {}

	void DisplayNode(CAtumMonitorView* pView);
	void SetData(char *name, time_t startedTime, char *ip, int port, int client, BOOL flagBandwidth)
	{
		util::strncpy(m_szServerName, name, SIZE_MAX_SERVER_NAME);
		m_tmStartedTime = startedTime;
		util::strncpy(m_szIP, ip, SIZE_MAX_IPADDRESS);
		m_nPort = port;
		m_nClients = client;
		m_bFlagCalcBandWidth = flagBandwidth;
	}

public:
	int		m_nClients;
	UINT	m_AccumulatedTotalUserCounts;
	UINT	m_CurrentTotalUserCounts;
	UINT	m_MaxTotalUserCounts;
	vector<MSG_FIELDSERVER_USERCOUNTS>	m_FieldServerCountsVector;
};

class CServerGroupInfoNode;
class CPreServerNode : public CMonitorServerNode
{
public:
	CPreServerNode(CMonitorWinSocket *pWinSocket)
		:CMonitorServerNode(pWinSocket)
	{
		util::zero(m_szVersionListFileUploadPath, SIZE_MAX_FTP_FILE_PATH);
		util::zero(m_szDownloadHttpServerIP,SIZE_MAX_FTP_URL);
		m_nDownloadHttpServerPort		= 0;
		util::zero(m_szDownloadHttpServerAccountName, SIZE_MAX_ACCOUNT_NAME);
		util::zero(m_szDownloadHttpServerPassword, SIZE_MAX_PASSWORD);
		util::zero(m_szVersionListFileDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);
	}

	CPreServerNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port, int client)
		: CMonitorServerNode(pWinSocket, name, ip, port), m_nClients(client)
	{
	}
	virtual ~CPreServerNode();

	void InsertServerGroupInfoNodePtr(CServerGroupInfoNode *pNode);

	void DisplayNode(CAtumMonitorView* pView);
	void SetData(char *name, time_t startedTime, char *ip, int port, int client, BOOL flagBandwidth)
	{
		util::strncpy(m_szServerName, name, SIZE_MAX_SERVER_NAME);
		m_tmStartedTime = startedTime;
		util::strncpy(m_szIP, ip, SIZE_MAX_IPADDRESS);
		m_nPort = port;
		m_nClients = client;
		m_bFlagCalcBandWidth = flagBandwidth;
	}

public:
	int		m_nClients;
	list<CServerGroupInfoNode*>		m_listServerGroupInfoNodePtr;

	// FTP 정보
	char	m_szFtpIP[SIZE_MAX_FTP_URL];
	USHORT	m_nFtpPort;
	char	m_szFtpAccountName[SIZE_MAX_ACCOUNT_NAME];
	char	m_szFtpPassword[SIZE_MAX_PASSWORD];
	CString	m_szClientFTPUpdateUploadDir;	// UPDATE_DIR// 2005-12-23 by cmkwon, 수정
	CString	m_szLauncherFileUploadPath;	// LAUNCHER_FILE_NAME// 2005-12-23 by cmkwon, 수정
	CString	m_szDeleteFileListUploadPath;	// DELFILELIST_FILE_NAME// 2005-12-23 by cmkwon, 수정
	CString	m_szNoticeFileUploadPath;		// NOTICE_FILE_NAME// 2005-12-23 by cmkwon, 수정
	char					m_szVersionListFileUploadPath[SIZE_MAX_FTP_FILE_PATH];

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-01-06 by cmkwon, Http Auto Update server 구현 추가
	// 2007-01-06 by cmkwon, HTTP Server 정보
	char					m_szDownloadHttpServerIP[SIZE_MAX_FTP_URL];
	USHORT					m_nDownloadHttpServerPort;
	char					m_szDownloadHttpServerAccountName[SIZE_MAX_ACCOUNT_NAME];
	char					m_szDownloadHttpServerPassword[SIZE_MAX_PASSWORD];
	CString					m_szClientHttpUpdateDownloadDir;
	CString					m_szLauncherFileDownloadHttpPath;
	CString					m_szDeleteFileListDownloadHttpPath;
	CString					m_szNoticeFileDownloadHttpPath;
	char					m_szVersionListFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
};

class CNPCServerNode : public CMonitorServerNode
{
public:
	CNPCServerNode(CMonitorWinSocket *pWinSocket)
		:CMonitorServerNode(pWinSocket)
	{
	}

	CNPCServerNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port, int client)
		: CMonitorServerNode(pWinSocket, name, ip, port), m_nClients(client)
	{
	}
	virtual ~CNPCServerNode();

	void InsertChannelInfoNodePtr(CChannelInfoNode *pNode);
	CChannelInfoNode *GetChannelInfoNode(MAP_CHANNEL_INDEX i_mapChannIdx);

	void DisplayNode(CAtumMonitorView* pView);
	void SetData(char *name, time_t startedTime, char *ip, int port, int client, int mapCount, BOOL flagBandwidth)
	{
		util::strncpy(m_szServerName, name, SIZE_MAX_SERVER_NAME);
		m_tmStartedTime = startedTime;
		util::strncpy(m_szIP, ip, SIZE_MAX_IPADDRESS);
		m_nPort = port;
		m_nClients = client;
		m_bFlagCalcBandWidth = flagBandwidth;
		m_nLoadedMapCounts = mapCount;
	}

public:
	int		m_nClients;
	int		m_nLoadedMapCounts;
	list<CChannelInfoNode*>		m_listChannelInfoNodePtr;
};

class CLogServerNode : public CMonitorServerNode
{
public:
	BOOL	m_bIsLogging;
};

class CChannelInfoNode : public CMonitorServerNode
{
public:
	CChannelInfoNode(CMonitorWinSocket *pWinSocket)
		:CMonitorServerNode(pWinSocket)
	{
		m_uiAccumulatedUserCounts		= 0;
		m_uiCurrentUserCounts			= 0;
		m_uiMaxUserCounts				= 0;
		m_uiAccumulatedMonsterCounts	= 0;
		m_uiCurrentMonsterCounts		= 0;
		m_uiLimitMonsterCount			= 0;

		m_bCityWarMap					= FALSE;
		util::zero(&m_cityWarOccupyInfo, sizeof(m_cityWarOccupyInfo));
	}

	CChannelInfoNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port, int client)
		: CMonitorServerNode(pWinSocket, name, ip, port)
	{
	}
	virtual ~CChannelInfoNode() {}

	void DisplayNode(CAtumMonitorView* pView);
public:
	PROJECTINFO			m_projectInfo;
	BOOL				m_bCityWarMap;
	SCITY_OCCUPY_INFO	m_cityWarOccupyInfo;

	ChannelIndex_t		m_ChannelIndex;
	UINT				m_uiAccumulatedUserCounts;
	UINT				m_uiCurrentUserCounts;
	UINT				m_uiMaxUserCounts;
	UINT				m_uiAccumulatedMonsterCounts;
	UINT				m_uiCurrentMonsterCounts;

	UINT				m_uiLimitMonsterCount;

	BOOL				m_bIsEnabled;

	HTREEITEM			m_hItem;		// TreeCtrl의 아이템
};

class CServerGroupInfoNode : public CMonitorServerNode
{
public:
	CServerGroupInfoNode(CMonitorWinSocket *pWinSocket)
		:CMonitorServerNode(pWinSocket)
	{
		m_hItem = NULL;
	}

	CServerGroupInfoNode(CMonitorWinSocket *pWinSocket, char *name, char *ip, int port, int client)
		: CMonitorServerNode(pWinSocket, name, ip, port)
	{
	}
	virtual ~CServerGroupInfoNode() {}

	void DisplayNode(CAtumMonitorView* pView);
	BOOL IsActiveServerGroup(void);
public:
	MSG_PM_GET_SERVER_GROUP_INFO_ACK	m_ServerGroupInfo;
	HTREEITEM							m_hItem;
};


#endif // !defined(AFX_MONITORNODE_H__59C90551_5406_4A85_BA1D_98C6053CF888__INCLUDED_)
