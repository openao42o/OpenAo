// FieldGlobal.cpp: implementation of the CFieldGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldGlobal.h"
#include "LogWinSocket.h"
#include "PreWinSocket.h"
#include "IMWinSocket.h"
#include "ArenaFieldWinSocket.h"		// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - 
#include "FieldIOCP.h"
#include "Config.h"
#include "PCBangIPManager.h"
#include "AtumLogSender.h"				// 2012-10-08 by khkim, GLog
#include "DGAEventManager.h"
#ifdef S_WEB_CASHSHOP_SERVER_MODULE_HSKIM
#include "FieldWebIOCP.h"				// 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡
#endif
CFieldGlobal *			g_pFieldGlobal = NULL;
#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
int						g_diffVictoryWarByroo = 0;
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldGlobal::CFieldGlobal()
{
	if(g_pFieldGlobal)
	{
		return;
	}
	g_pFieldGlobal				= this;

	m_dgaMode = 0;
	m_dgaItemMinCount = 0;
	m_dgaItemMaxCount = 0;
	m_rndDgaItemMinCount = 0;
	m_rndDgaItemMaxCount = 0;
	m_rndDgaDailyMaxCount = 0;
	m_rndDgaItems.clear();
	m_dgaItems.clear();
	m_rndDgaMaps.clear();

	m_dwLastTickLogSystem		= 0;
	memset(m_szFieldServerGroupName, 0x00, SIZE_MAX_SERVER_NAME);
	memset(m_szIPLogServer, 0x00, SIZE_MAX_IPADDRESS);
	memset(m_szIPNPCServer, 0x00, SIZE_MAX_IPADDRESS);		// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
	memset(m_szIPIMServer, 0x00, SIZE_MAX_IPADDRESS);
	memset(m_szIPVoIP1to1Server, 0x00, SIZE_MAX_IPADDRESS);
	m_nPortLogServer			= 0;
	m_nPortIMServer				= 0;
	m_nPortVoIP1to1Server		= 0;

	m_vectorF2LWSocketPtr.reserve(SIZE_FIELD2LOG_WINSOCKET);
	m_nSendIndexF2LWSocket		= 0;
	m_pField2PreWinSocket		= NULL;
	m_pField2IMWinSocket		= NULL;
	m_pField2ArenaFieldWinSocket = NULL;					// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - 

	m_bEventActivated			= FALSE;

	m_plRequestInCRTLib			= NULL;		// 2006-05-30 by cmkwon
	m_plRequestInCRTLib			= (long*)0x10261538;		// 2006-05-30 by cmkwon

	m_bCashShopServiceFlag		= FALSE;	// 2007-01-10 by cmkwon
	m_bUseSecondaryPasswordSystemFlag	= FALSE;	// 2007-09-12 by cmkwon, º£Æ®³² 2Â÷ÆÐ½º¿öµå ±¸Çö - ÃÊ±âÈ­

	util::zero(m_szMainORTestServerName, SIZE_MAX_SERVER_NAME);		// 2007-04-09 by cmkwon
	util::zero(m_szGamePublisherName, SIZE_MAX_GAME_PUBLISHER_NAME);	// 2007-04-09 by cmkwon
	m_bIsJamboreeServer			= FALSE;	// 2007-04-09 by cmkwon

	strcpy(m_szMainORTestServerName, "Main");
	strcpy(m_szGamePublisherName, "aresonline");

	m_pPCBangIPManager			= NULL;
	util::zero(&m_sArenaServerInfo, sizeof(SARENA_SERVER_INFO));		// 2008-01-17 by dhjin, ¾Æ·¹³ª ÅëÇÕ -  


	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 	
	util::zero(m_szBillingDBServerIP, SIZE_MAX_ODBC_CONN_STRING);
	m_nBillingDBServerPort		= 0;
	util::zero(m_szBillingDBServerDatabaseName, SIZE_MAX_ODBC_CONN_STRING);
	util::zero(m_szBillingDBServerUserID, SIZE_MAX_ODBC_CONN_STRING);
	util::zero(m_szBillingDBServerPassword, SIZE_MAX_ODBC_CONN_STRING);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-06-03 by cmkwon, ¼¼·Â ¼±ÅÃ½Ã Á¦ÇÑ ½Ã½ºÅÛ »ç¿ë ¿©ºÎ ÇÃ·¡±× Ãß°¡ - ÃÊ±âÈ­
	m_bUseInflSelectionRestrictSystem	= FALSE;


	// 2015-11-25 Future, Multiple IP Restriction System
#ifdef S_IP_UNIQUE_CONNECTION
	m_bMultipleIPRestriction = FALSE;
#endif // S_IP_UNIQUE_CONNECTION

}

CFieldGlobal::~CFieldGlobal()
{
	EndServerSocket();
	this->DestroyField2PreWinSocket();
	this->DestroyField2IMWinSocket();
	this->DestroyAllF2LWSocket();

	for (auto item : m_dgaItems)
	{
		util::del(item);
	}

	for (auto item : m_rndDgaItems)
	{
		util::del(item);
	}

	m_rndDgaItems.clear();
	m_dgaItems.clear();
	m_rndDgaMaps.clear();

	g_pGlobal = NULL;
	util::del(m_pPCBangIPManager);


}


//////////////////////////////////////////////////////////////////////
// Property
//////////////////////////////////////////////////////////////////////
void CFieldGlobal::SetLastTickLogSystem(DWORD i_dwTick)
{
	m_dwLastTickLogSystem  = i_dwTick;
}

DWORD CFieldGlobal::GetLastTickLogSystem(void)
{
	return m_dwLastTickLogSystem;
}

void CFieldGlobal::SetFieldServerGroupName(char *i_szFieldServerGroupName)
{
	util::strncpy(m_szFieldServerGroupName, i_szFieldServerGroupName, SIZE_MAX_SERVER_NAME);
}

char *CFieldGlobal::GetFieldServerGroupName(void)
{
	return m_szFieldServerGroupName;
}

void CFieldGlobal::SetIPLogServer(char *i_szIP)
{
	memcpy(m_szIPLogServer, i_szIP, SIZE_MAX_IPADDRESS);
}

char *CFieldGlobal::GetIPLogServer(void)
{
	return m_szIPLogServer;
}

// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
void CFieldGlobal::SetIPNPCServer(char *i_szIP)
{
	memcpy(m_szIPNPCServer, i_szIP, SIZE_MAX_IPADDRESS);
}

char *CFieldGlobal::GetIPNPCServer(void)
{
	return m_szIPNPCServer;
}
// end 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î

void CFieldGlobal::SetIPIMServer(char *i_szIP)
{
	memcpy(m_szIPIMServer, i_szIP, SIZE_MAX_IPADDRESS);
}

char *CFieldGlobal::GetIPIMServer(void)
{
	return m_szIPIMServer;
}

char *CFieldGlobal::GetIPVoIP1to1Server(void)
{
	return m_szIPVoIP1to1Server;
}

char *CFieldGlobal::GetIPVoIPNtoNServer(void)
{
	return m_szIPVoIPNtoNServer;
}


void CFieldGlobal::SetPortLogServer(int i_nPort)
{
	m_nPortLogServer = i_nPort;
}

int CFieldGlobal::GetPortLogServer(void)
{
	return m_nPortLogServer;
}

void CFieldGlobal::SetPortIMServer(int i_nPort)
{
	m_nPortIMServer = i_nPort;
}

int CFieldGlobal::GetPortIMServer(void)
{
	return m_nPortIMServer;
}

int CFieldGlobal::GetPortVoIP1to1Server(void)
{
	return m_nPortVoIP1to1Server;
}

int CFieldGlobal::GetPortVoIPNtoNServer(void)
{
	return m_nPortVoIPNtoNServer;
}

CPreWinSocket * CFieldGlobal::GetField2PreWinSocket(void)
{
	return m_pField2PreWinSocket;
}

CIMWinSocket * CFieldGlobal::GetField2IMWinSocket(void)
{
	return m_pField2IMWinSocket;
}

CArenaFieldWinSocket * CFieldGlobal::GetField2ArenaFieldWinSocket(void)
{
	return m_pField2ArenaFieldWinSocket;					// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldGlobal::ChecklRequestInCRTLib(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-30 ~ 2006-05-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldGlobal::ChecklRequestInCRTLib(void)
{
#ifdef _DEBUG
	if(NULL != m_plRequestInCRTLib
		&& 0 > (*m_plRequestInCRTLib))
	{// 2006-05-30 by cmkwon, CRTLib¿¡ debug ¸ðµåÀÏ¶§ 
		*m_plRequestInCRTLib	= 1;		// 2006-05-30 by cmkwon
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CFieldGlobal::GetMainORTestServerName(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-04-09 ~ 2007-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CFieldGlobal::GetMainORTestServerName(void)
{
	return m_szMainORTestServerName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CFieldGlobal::GetGamePublisherName(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-04-09 ~ 2007-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CFieldGlobal::GetGamePublisherName(void)
{
	return m_szGamePublisherName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2007-04-09 ~ 2007-04-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::GetIsJamboreeServer(void)
{
	return m_bIsJamboreeServer;
}


///////////////////////////////////////////////////////////////////////////
// Method
///////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::CreateAllF2WSocket(HWND i_hWnd)
{
	if( false == m_vectorF2LWSocketPtr.empty() )
	{
		return FALSE;
	}

	if( NULL == m_hMainWndHandle )
	{
		m_hMainWndHandle = i_hWnd;
	}


	for( int i = 0; i < SIZE_FIELD2LOG_WINSOCKET; i++ )
	{
		CLogWinSocket *pF2LWSocket = new CLogWinSocket( m_hMainWndHandle , WM_LOG_ASYNC_EVENT , WM_LOG_PACKET_NOTIFY );
		m_vectorF2LWSocketPtr.push_back( pF2LWSocket );
		if ( m_vectorF2LWSocketPtr.size() >= SIZE_FIELD2LOG_WINSOCKET )
		{
			break;
		}
	}
	 
	return TRUE;
}

BOOL CFieldGlobal::CreateField2PreWinSocket(HWND i_hWnd)
{
	if(m_pField2PreWinSocket){		return FALSE;}
	if(NULL == m_hMainWndHandle)
	{
		m_hMainWndHandle = i_hWnd;
	}

	m_pField2PreWinSocket = new CPreWinSocket(m_hMainWndHandle, WM_PRE_ASYNC_EVENT, WM_PRE_PACKET_NOTIFY);
	return TRUE;
}

BOOL CFieldGlobal::CreateField2IMWinSocket(HWND i_hWnd)
{
	if(m_pField2IMWinSocket){		return FALSE;}
	if(NULL == m_hMainWndHandle)
	{
		m_hMainWndHandle = i_hWnd;
	}

	m_pField2IMWinSocket = new CIMWinSocket(m_hMainWndHandle, WM_IM_ASYNC_EVENT, WM_IM_PACKET_NOTIFY);
	return TRUE;
}

BOOL CFieldGlobal::CreateField2ArenaFieldWinSocket(HWND i_hWnd)
{// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - 
	if(m_pField2ArenaFieldWinSocket){		return FALSE;}
	if(NULL == m_hMainWndHandle)
	{
		m_hMainWndHandle = i_hWnd;
	}

	m_pField2ArenaFieldWinSocket = new CArenaFieldWinSocket(m_hMainWndHandle, WM_FIELD_ASYNC_EVENT, WM_FIELD_PACKET_NOTIFY);
	return TRUE;
}

void CFieldGlobal::DestroyAllF2LWSocket(void)
{
	// 2009-04-20 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡ - 
	this->WriteSystemLogEX(TRUE, "  [Notify] CFieldGlobal::DestroyAllF2LWSocket# \r\n");

	int nSize = m_vectorF2LWSocketPtr.size();
	for(int i=0; i < nSize; i++)
	{
		if(m_vectorF2LWSocketPtr[i]
			&& INVALID_SOCKET != m_vectorF2LWSocketPtr[i]->GetSocketHandle())
		{
			m_vectorF2LWSocketPtr[i]->CloseSocket();
		}
	}
	for (auto x : m_vectorF2LWSocketPtr) delete x;
	m_vectorF2LWSocketPtr.clear();
}

void CFieldGlobal::DestroyField2PreWinSocket(void)
{
	// 2009-03-19 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡ - FieldServer 
	this->WriteSystemLogEX(TRUE, "  [Notify] CFieldGlobal::DestroyField2PreWinSocket# 0x%X IsConnected(%d) \r\n", m_pField2ArenaFieldWinSocket, (m_pField2ArenaFieldWinSocket)?m_pField2ArenaFieldWinSocket->IsConnected():FALSE);

	if(m_pField2PreWinSocket && m_pField2PreWinSocket->IsConnected())
	{
		m_pField2PreWinSocket->CloseSocket();
	}
	util::del(m_pField2PreWinSocket);
}

void CFieldGlobal::DestroyField2IMWinSocket(void)
{
	// 2009-03-19 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡ - FieldServer 
	this->WriteSystemLogEX(TRUE, "  [Notify] CFieldGlobal::DestroyField2IMWinSocket# 0x%X IsConnected(%d) \r\n", m_pField2IMWinSocket, (m_pField2IMWinSocket)?m_pField2IMWinSocket->IsConnected():FALSE);

	if(m_pField2IMWinSocket && m_pField2IMWinSocket->IsConnected())
	{
		m_pField2IMWinSocket->CloseSocket();
	}
	util::del(m_pField2IMWinSocket);
}

void CFieldGlobal::DestroyField2ArenaFieldWinSocket(void)
{// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - 
	// 2009-03-19 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡ - FieldServer 
	this->WriteSystemLogEX(TRUE, "  [Notify] CFieldGlobal::DestroyField2ArenaFieldWinSocket# 0x%X IsConnected(%d) \r\n", m_pField2ArenaFieldWinSocket, (m_pField2ArenaFieldWinSocket)?m_pField2ArenaFieldWinSocket->IsConnected():FALSE);

	if(m_pField2ArenaFieldWinSocket && m_pField2ArenaFieldWinSocket->IsConnected())
	{
		m_pField2ArenaFieldWinSocket->CloseSocket();
	}
	util::del(m_pField2ArenaFieldWinSocket);
}

///////////////////////////////////////////////////////////////////////////
// virtual Function
///////////////////////////////////////////////////////////////////////////
bool CFieldGlobal::InitServerSocket()
{
	if(FALSE == IsArenaServer())
	{// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ¾Æ·¹³ª ¼­¹ö°¡ ¾Æ´Ï¸é »ý¼º
		if(NULL == m_pField2PreWinSocket || m_pField2PreWinSocket->IsConnected() == FALSE
			|| NULL == m_pField2IMWinSocket || m_pField2IMWinSocket->IsConnected() == FALSE
			|| NULL == m_pField2ArenaFieldWinSocket)
		{
			return FALSE;
		}
	}
	else
	{
		if(NULL == m_pField2PreWinSocket || m_pField2PreWinSocket->IsConnected() == FALSE
			|| NULL == m_pField2IMWinSocket || m_pField2IMWinSocket->IsConnected() == FALSE)
		{
			return FALSE;
		}
	}
		
	// check log server°¡ TRUEÀÌ¸é log server ¾øÀÌ´Â ¼­¹ö ½ÇÇà ¾È µÊ
	if (m_bCheckLogServer
		&& FALSE == this->IsConnectedAllF2LWSocket())
	{
		return FALSE;
	}

	MessageType_t msgType = T_FL_LOG_START_FIELD_SERVER;
	this->SendLogFieldServer2LogServer((BYTE*)&msgType, sizeof(msgType));

	// start 2012-10-08 by khkim, GLog 
	// ServerOn GLog ÆÐÅ¶Send ºÎºÐ
	MSG_FL_LOG_SERVER LogServerOnParameter;
	util::zero(&LogServerOnParameter, sizeof(LogServerOnParameter));		// 2013-05-30 by jhseol, GLog ½Ã½ºÅÛ º¸¿Ï - ÃÊ±âÈ­
	LogServerOnParameter.LogType		= GLOG_SL_LT_SERVERON;
	CAtumLogSender::SendLogServer(&LogServerOnParameter);
	// end 2012-10-08 by khkim, GLog

	if (m_pGIOCP) return false;

	// 2009-03-19 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡ - 
	server::log(" [Notify] CFieldGlobal::InitServerSocket# Starting... \r\n");

	m_dwLastTickLogSystem = GetTickCount() - (TIMERGAP_LOGSYSTEM - 60000);			// FieldServer°¡ ½ÃÀÛÇÏ°í 1ºÐÈÄ¿¡ ÇÑ¹ø Á¢¼ÓÁ¤º¸¸¦ ·Î±× ¼­¹ö¿¡ ³²±ä´Ù. ÀÌÈÄ ºÎÅÍ´Â 5ºÐ°£¿¡ ÇÑ¹ø¾¿ ·Î±×¸¦ ³²±ä´Ù

	//////////////////////////////////////////////////////////////////////////
	// 2007-08-07 by dhjin, CPCBangIPManager »ý¼º
	m_pPCBangIPManager = new CPCBangIPManager;	

	auto iocp = new CFieldIOCP(m_nPortListening, m_szIPLocal);
	m_pGIOCP = iocp;
	iocp->m_pPreWinSocket = m_pField2PreWinSocket;
	iocp->m_pIMWinSocket = m_pField2IMWinSocket;
	if(FALSE == IsArenaServer())
	{// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ¾Æ·¹³ª ¼­¹ö°¡ ¾Æ´Ï¸é »ý¼º
		iocp->m_pArenaFieldWinSocket = m_pField2ArenaFieldWinSocket;
	}
	if(m_pGIOCP->IOCPInit() == FALSE)
	{
		char	szSystemLog[256];
		sprintf(szSystemLog, "[Error] FieldServer IOCPInit Error\r\n");
		this->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);
		MessageBox(NULL, szSystemLog, "ERROR", MB_OK);
		return FALSE;
	}
	//-- CMKWON 2003-06-12 NPC Server·Î ºÎÅÍ T_FN_NPCSERVER_START¸¦ ¹Þ°í Ã³¸®ÇÏµµ·Ï ¼öÁ¤
	//m_pGIOCP->OpenUDPPortForOtherServer();

	m_pField2PreWinSocket->SetFieldIOCP(iocp);
	m_pField2IMWinSocket->SetFieldIOCP(iocp);
	if (m_pField2ArenaFieldWinSocket)
	
		m_pField2ArenaFieldWinSocket->SetFieldIOCP(iocp);	// 2009-04-17 by cmkwon, ÅëÇÕArena °ü·Ã ¹ö±× ¼öÁ¤ - ¿©±â¿¡¼­ ¼³Á¤ÇØ¾ß ÇÔ.
	


	///////////////////////////////////////////////////////////////////////////
	// PRE server¿¡ Connect ¹× Map Á¤º¸ Àü¼Û - by kelovon
	INIT_MSG_WITH_BUFFER(MSG_FP_CONNECT_FIELD_CONNECT, T_FP_CONNECT_FIELD_CONNECT, msgConnect, SendBuf);
	util::strncpy(msgConnect->FieldServerGroupName, GetFieldServerGroupName());
	msgConnect->FieldServerID.SetValue(g_pFieldGlobal->GetPublicIPLocal(), m_pGIOCP->GetListenerPort());
	msgConnect->NumOfMapIndex = iocp->GetMapWorkspace()->m_vectorPtrMapProject.size();	// ÀÌÈÄ¿¡ ¼­ºñ½ºµÇ´Â map ¼ö¸¸ ÇÒ´ç
	msgConnect->ArenaFieldServerCheck = m_sArenaServerInfo.ArenaFieldServerCheck;		// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - TRUE => ¾Æ·¹³ª ÇÊµå ¼­¹ö
	msgConnect->DBServerGroup			= this->m_DBServerGroup;		// 2008-04-29 by cmkwon, ¼­¹ö±º Á¤º¸ DB¿¡ Ãß°¡(½Å±Ô °èÁ¤ Ä³¸¯ÅÍ »ý¼º Á¦ÇÑ ½Ã½ºÅÛÃß°¡) - CFieldIOCP::IOCPInit() ¿¡¼­ ¼³Á¤µÊ

	int offset = 0;
	int NumOfMapServiced = 0;
	int i;
	for (i = 0; i < msgConnect->NumOfMapIndex; i++)
	{
		SERVER_ID tmpID;
		tmpID.SetValue(iocp->GetMapWorkspace()->m_vectorPtrMapProject[i]->m_strFieldIP
					, iocp->GetMapWorkspace()->m_vectorPtrMapProject[i]->m_sFieldListenPort);
		if (tmpID == iocp->m_FieldServerID)
		{
			*(USHORT*)((char*)SendBuf + MSG_SIZE(MSG_FP_CONNECT_FIELD_CONNECT) + offset)
				= iocp->GetMapWorkspace()->m_vectorPtrMapProject[i]->m_nMapIndex;
			offset += sizeof(USHORT);
			NumOfMapServiced++;
		}
	}
	msgConnect->NumOfMapIndex = NumOfMapServiced;
	m_pField2PreWinSocket->Write((char*)SendBuf, MSG_SIZE(MSG_FP_CONNECT_FIELD_CONNECT) + offset);

	server::log(" [Notify] CFieldGlobal::InitServerSocket_ send to PreServer !!, Service map counts(%3d), Send Bytes(%d)\r\n", NumOfMapServiced, MSG_SIZE(MSG_FP_CONNECT_FIELD_CONNECT) + offset);

	///////////////////////////////////////////////////////////////////////////
	// IM server¿¡ Connect ¹× Map Á¤º¸ Àü¼Û - by kelovon
	offset = 0;

	*(MessageType_t*)SendBuf = T_FI_CONNECT;
	offset += SIZE_FIELD_TYPE_HEADER;

	MSG_FI_CONNECT *pMsgConn = (MSG_FI_CONNECT*)(SendBuf + offset);
	pMsgConn->NumOfMapIndex = iocp->GetMapWorkspace()->m_vectorPtrMapProject.size();	// ÀÌÈÄ¿¡ ¼­ºñ½ºµÇ´Â map ¼ö¸¸ ÇÒ´ç
	pMsgConn->FieldServerID = iocp->m_FieldServerID;
	pMsgConn->ArenaServerCheck = IsArenaServer();

	offset += sizeof(MSG_FI_CONNECT);

	NumOfMapServiced = 0;
	for (i = 0; i < pMsgConn->NumOfMapIndex; i++)
	{
		SERVER_ID tmpID;
		tmpID.SetValue(iocp->GetMapWorkspace()->m_vectorPtrMapProject[i]->m_strFieldIP
					, iocp->GetMapWorkspace()->m_vectorPtrMapProject[i]->m_sFieldListenPort);
		if (tmpID == iocp->m_FieldServerID)
		{
			*(USHORT*)((char*)SendBuf + offset)
				= iocp->GetMapWorkspace()->m_vectorPtrMapProject[i]->m_nMapIndex;
			offset += sizeof(USHORT);
			NumOfMapServiced++;
		}
	}
	pMsgConn->NumOfMapIndex = NumOfMapServiced;

	m_pField2IMWinSocket->Write((char*)SendBuf, offset);

	server::log(" [Notify] CFieldGlobal::InitServerSocket_ send to IMServer !!, Service map counts(%3d), Send Bytes(%d)\r\n", NumOfMapServiced, offset);
	server::log(" [Notify] CFieldGlobal::InitServerSocket# Started \r\n");

	return true;
}

bool CFieldGlobal::EndServerSocket()
{
	// 2009-03-19 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡ - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤
	//this->WriteSystemLogEX(TRUE, "[Notify] CFieldGlobal::EndServerSocket\r\n");	// 2007-12-17 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡
	this->WriteSystemLogEX(TRUE, "  [Notify] CFieldGlobal::EndServerSocket# \r\n");

	CGlobalGameServer::EndServerSocket();

	return TRUE;
}

void CFieldGlobal::VMemAlloc()
{
	VMemPool::vmPoolAddObject(sizeof(ActionInfo), SIZE_MAX_FIELDSERVER_SESSION * 5);
	// 2010-04-14 by cmkwon, ¼­¹ö ¸Þ¸ð¸® ºÎÁ· ¹®Á¦ ¼öÁ¤ - 
	//	VMemPool::vmPoolAddObject(sizeof(DROPMINE), SIZE_MAX_FIELDSERVER_SESSION * 12);
	VMemPool::vmPoolAddObject(sizeof(COverlapped), SIZE_MAX_FIELDSERVER_SESSION * 10);
	VMemPool::vmPoolAddObject(sizeof(ITEM_GENERAL), SIZE_MAX_FIELDSERVER_SESSION * 20);
	VMemPool::vmPoolAddObject(sizeof(ITEM_SKILL), SIZE_MAX_FIELDSERVER_SESSION * 5);
	VMemPool::vmPoolAddObject(sizeof(EVENTINFO), SIZE_MAX_FIELDSERVER_SESSION);
	VMemPool::vmPoolAddObject(sizeof(CFieldCharacterQuest), SIZE_MAX_FIELDSERVER_SESSION * 10);
	VMemPool::vmPoolAddObject(sizeof(FIELD_DUMMY), SIZE_MAX_FIELDSERVER_SESSION * 2);
	VMemPool::vmPoolAddObject(sizeof(DROPITEM), SIZE_MAX_FIELDSERVER_SESSION * 2);
	VMemPool::vmPoolAddObject(sizeof(TradeItem), SIZE_MAX_FIELDSERVER_SESSION * 1);
	VMemPool::vmPoolAddObject(sizeof(CSendPacket), 1000);
	VMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);

	// 2010-06-15 by shcho&hslee Æê½Ã½ºÅÛ - Æê ¼ÒÀ¯ Á¤º¸ ¸Þ¸ð¸® Ç® µî·Ï.
	VMemPool::vmPoolAddObject(sizeof(tPET_CURRENTINFO), SIZE_MAX_FIELDSERVER_SESSION * 10);
}

BOOL CFieldGlobal::LoadDGAConfiguration(void)
{
	const int   SIZE_BUFF = 512;
	char		buff[SIZE_BUFF];
	char		buffBackup[SIZE_BUFF];		// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
	const char	*token;
	char		seps[] = " \t";
	ifstream	fin;
	CConfig		config;
	char		szLog[1024];

	if (!config.LoadUserDefinedVariables(GLOBAL_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables_1() error, FileName[%s]\r\n", GLOBAL_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	if (!config.LoadUserDefinedVariables(DGA_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables_1() error, FileName[%s]\r\n", DGA_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	fin.open(DGA_CONFIG_FILE_PATH);

	if (!fin.is_open())
	{	// cannot open file...
		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n", nErr, FIELD_SERVER_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	while (TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if (bFlagFileEnd && strcmp(buff, "") == 0)
		{
			break;
		}

		util::strncpy(buffBackup, buff, SIZE_BUFF);	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 

		token = strtok(buff, seps);

		if (token == NULL || strncmp(token, "#", 1) == 0 || strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables
			continue;
		}

		if (stricmp(token, "DGAMode") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_dgaMode = 0;
			}
			else
			{
				int tmpTok = atoi(token);
				if (tmpTok >= 0 && tmpTok <= 3)
				{
					m_dgaMode = tmpTok;
				}
				else
				{
					m_dgaMode = 0;
				}
			}
			sprintf(szLog, "[DEBUG] DGA Mode: %d\r\n", m_dgaMode);
			this->WriteSystemLog(szLog);
		}
		else if (stricmp(token, "DGATime") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_dgaTime.SetDateTime(0, 0, 0, 20, 0, 0);
				sprintf(szLog, "[DEBUG] DGA Time: Time(hour:%d,minute:%d)\r\n", 20, 0);
				this->WriteSystemLog(szLog);
			}
			else
			{
				char* tmpTok;

				tmpTok = strtok((char*)token, ":");

				int hour = 20;
				int minute = 0;
				if (tmpTok)
				{
					hour = atoi(tmpTok);
				}

				tmpTok = strtok(NULL, ":");

				if (tmpTok)
				{
					minute = atoi(tmpTok);
				}

				if (hour < 0 || hour > 24)
				{
					hour = 20;
				}
				if (minute < 0 || minute > 59)
				{
					minute = 0;
				}

				m_dgaTime.SetDateTime(0, 0, 0, hour, minute, 0);
				sprintf(szLog, "[DEBUG] DGA Time: Time(hour:%d,minute:%d)\r\n", hour, minute);
				this->WriteSystemLog(szLog);
			}

		}
		else if (stricmp(token, "DGAItemMinCount") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_dgaItemMinCount = 1;
			}
			else
			{
				m_dgaItemMinCount = atoi(token);
			}
			sprintf(szLog, "[DEBUG] DGA Item Min Count: %d\r\n", m_dgaItemMinCount);
			this->WriteSystemLog(szLog);
		}
		else if (stricmp(token, "DGAItemMaxCount") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_dgaItemMaxCount = 5;
			}
			else
			{
				m_dgaItemMaxCount = atoi(token);
			}
			sprintf(szLog, "[DEBUG] DGA Item Max Count: %d\r\n", m_dgaItemMaxCount);
			this->WriteSystemLog(szLog);
		}
		else if (stricmp(token, "DGARndItemMinCount") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_rndDgaItemMinCount = 1;
			}
			else
			{
				m_rndDgaItemMinCount = atoi(token);
			}
			sprintf(szLog, "[DEBUG] DGA Random Item Min Count: %d\r\n", m_rndDgaItemMinCount);
			this->WriteSystemLog(szLog);
		}
		else if (stricmp(token, "DGARndItemMaxCount") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_rndDgaItemMaxCount = 5;
			}
			else
			{
				m_rndDgaItemMaxCount = atoi(token);
			}
			sprintf(szLog, "[DEBUG] DGA Random Item Max Count: %d\r\n", m_rndDgaItemMaxCount);
			this->WriteSystemLog(szLog);
		}
		else if (stricmp(token, "DGARndDailyMaxCount") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				m_rndDgaDailyMaxCount = 5;
			}
			else
			{
				m_rndDgaDailyMaxCount = atoi(token);
			}
			sprintf(szLog, "[DEBUG] DGA Random Daily Max Count: %d\r\n", m_rndDgaDailyMaxCount);
			this->WriteSystemLog(szLog);
		}
		else if (stricmp(token, "DGAItem") == 0)
		{
			token = config.strtok(NULL, seps);

			if (!token || strcmp(token, "") == 0)
			{
				continue;
			}
			else
			{
				char* tmpTok;

				tmpTok = strtok((char*)token, ":");

				if (!tmpTok)
				{
					continue;
				}

				int itemNum = atoi(tmpTok);
				int count = 1;

				tmpTok = strtok(NULL, ":");

				if (tmpTok)
				{
					count = atoi(tmpTok);

					if (count < 1){
						count = 1;
					}
				}

				dgaItem* item = new dgaItem;

				item->itemNum = itemNum;
				item->count = count;

				m_dgaItems.push_back(item);

				sprintf(szLog, "[DEBUG] DGA Item: (itemNum:%d,count:%d)\r\n", itemNum, count);
				this->WriteSystemLog(szLog);
			}
		}
		else if (stricmp(token, "DGARndItem") == 0)
		{
			token = config.strtok(NULL, seps);
			if (!token || strcmp(token, "") == 0)
			{
				continue;
			}
			else
			{
				char* tmpTok;

				tmpTok = strtok((char*)token, ":");

				if (!tmpTok)
				{
					continue;
				}

				int itemNum = atoi(tmpTok);
				int count = 1;

				tmpTok = strtok(NULL, ":");

				if (tmpTok)
				{
					count = atoi(tmpTok);

					if (count < 1){
						count = 1;
					}
				}

				dgaItem* item = new dgaItem;

				item->itemNum = itemNum;
				item->count = count;

				m_rndDgaItems.push_back(item);

				sprintf(szLog, "[DEBUG] DGA Random Item: (itemNum:%d,count:%d)\r\n", itemNum, count);
				this->WriteSystemLog(szLog);
			}

		}
		else if (stricmp(token, "DGARndMaps") == 0)
		{
			token = config.strtok(NULL, seps);
			if (strcmp(token, "") == 0)
			{
				continue;
			}
			else
			{
				m_rndDgaMaps.push_back(atoi(token));
			}
			sprintf(szLog, "[DEBUG] DGA Random Maps: (Map:%d)\r\n", atoi(token));
			this->WriteSystemLog(szLog);
		}
	}
	fin.close();

	return TRUE;
}


BOOL CFieldGlobal::LoadConfiguration(void)
{
	LoadAbuseAndProhibitedName();
// 2007-01-22 by dhjin, PC¹æ ¸®½ºÆ® ½Ç½Ã°£À¸·Î º¯°æ
//	LoadPCBangIPListFromConfig();// 2006-08-18 by dhjin, pc¹æ ¸®½ºÆ® .cfg¿¡¼­ ÀÐ±â.

	///////////////////////////////////////////////////////////////////////////////
	// 2007-05-07 by cmkwon, Ä³¸¯ÅÍ¸í »ý¼º½Ã »ç¿ë°¡´ÉÇÑ ¹®ÀÚ ¸®½ºÆ®¸¦ ·ÎµùÇÑ´Ù.
	// 2007-05-07 by cmkwon, ¹®ÀÚ¸®½ºÆ®ÆÄÀÏ(AllLetterList.cfg)ÀÌ ¾øÀ¸¸é Ã¼Å© ÇÒ ÇÊ¿ä ¾ø´Ù
	CAtumSJ::LoadAllLetterList((CONFIG_ROOT+string("./AllLetterList.cfg")).c_str());

	const int SIZE_BUFF = 512;
	char		buff[SIZE_BUFF];
	char		buffBackup[SIZE_BUFF];		// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
	const char	*token;
	char		seps[] = " \t";
	ifstream	fin;
	CConfig		config;

	if (!config.LoadUserDefinedVariables(GLOBAL_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables_1() error, FileName[%s]\r\n"
			, GLOBAL_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	if (!config.LoadUserDefinedVariables( FIELD_SERVER_CONFIG_FILE_PATH))
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration LoadUserDefinedVariables_2() error, FileName[%s]\r\n"
			, FIELD_SERVER_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	fin.open(FIELD_SERVER_CONFIG_FILE_PATH);
	if (!fin.is_open())
	{	// cannot open file...

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration open() error, LastError[%d] ConfigFileName[%s]\r\n"
			, nErr, FIELD_SERVER_CONFIG_FILE_PATH);
		this->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	while(TRUE)
	{
		memset(buff, 0x00, SIZE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, SIZE_BUFF).eof();
		if(bFlagFileEnd == TRUE
			&& strcmp(buff, "") == 0)
		{
			break;
		}

		util::strncpy(buffBackup, buff, SIZE_BUFF);	// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 

		token = strtok(buff, seps);

		if (token == NULL
			|| strncmp(token, "#", 1) == 0
			|| strncmp(token, "$", 1) == 0)
		{	// ignore blank lines
			// ignore comments
			// ignore user defined variables

			continue;
		}

		if(stricmp(token, "LocalIPAddress") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") == 0)
			{
				this->SetIPLocal("127.0.0.1");
			}
			else
			{
				this->SetIPLocal((char*)token);
			}
		}
		else if(stricmp(token, "LocalPublicIPAddress") == 0)
		{// 2006-05-23 by cmkwon,
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			this->SetPublicIPLocal((char*)token);
		}
		else if(stricmp(token, "LocalListenPort") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortListening(atoi(token));
			}
		}
		else if(stricmp(token, "ODBCDSN") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCDSN\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCDSN, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if(stricmp(token, "ODBCUID") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCUID\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCUID, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szODBCUID, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCUID, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "ODBCPASSWD") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCPASSWD\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				util::strncpy((char*)m_szODBCPASSWORD, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szODBCPASSWORD, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "LOGDB_ODBCDSN") == 0)
		{// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - LOGDB_ODBCDSN °¡Á®¿À±â
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCDSN\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - m_szLogDBODBCDSN ¼³Á¤ÇÏ±â
				util::strncpy((char*)m_szLogDBODBCDSN, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if(stricmp(token, "LOGDB_ODBCUID") == 0)
		{// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - LOGDB_ODBCUID °¡Á®¿À±â
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCUID\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - m_szLogDBODBCUID ¼³Á¤ÇÏ±â
				util::strncpy((char*)m_szLogDBODBCUID, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szLogDBODBCUID, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szLogDBODBCUID, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "LOGDB_ODBCPASSWD") == 0)
		{// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - LOGDB_ODBCPASSWD °¡Á®¿À±â
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed. ODBCPASSWD\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - m_szLogDBODBCPASSWORD ¼³Á¤ÇÏ±â
				util::strncpy((char*)m_szLogDBODBCPASSWORD, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szLogDBODBCPASSWORD, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szLogDBODBCPASSWORD, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "FieldServerGroupName") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetFieldServerGroupName((char*)token);
			}
		}
		else if(stricmp(token, "PreServer") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetIPPreServer((char*)token);
			}
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortPreServer(atoi(token));
			}
		}
		else if(stricmp(token, "IMServer") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetIPIMServer((char*)token);
			}
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortIMServer(atoi(token));
			}
		}
		else if(stricmp(token, "LogServer") == 0)
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetIPLogServer((char*)token);
			}
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetPortLogServer(atoi(token));
			}
		}
		//else if(stricmp(token, "VoIP1to1Server") == 0)
		//{
		//	token = config.strtok(NULL, seps);
		//	if (token == NULL)
		//	{
		//		char *msg = "Loading Configuration File Failed.\n";
		//		DBGOUT(msg);
		//		MessageBox(NULL, msg, "Error", MB_OK);
		//		return FALSE;
		//	}
		//	util::strncpy(m_szIPVoIP1to1Server, token, SIZE_MAX_IPADDRESS);

		//	token = config.strtok(NULL, seps);
		//	if (token == NULL)
		//	{
		//		char *msg = "Loading Configuration File Failed.\n";
		//		DBGOUT(msg);
		//		MessageBox(NULL, msg, "Error", MB_OK);
		//		return FALSE;
		//	}
		//	m_nPortVoIP1to1Server = atoi(token);
		//}
		//else if(stricmp(token, "VoIPNtoNServer") == 0)
		//{
		//	token = config.strtok(NULL, seps);
		//	if (token == NULL)
		//	{
		//		char *msg = "Loading Configuration File Failed.\n";
		//		DBGOUT(msg);
		//		MessageBox(NULL, msg, "Error", MB_OK);
		//		return FALSE;
		//	}
		//	util::strncpy(m_szIPVoIPNtoNServer, token, SIZE_MAX_IPADDRESS);

		//	token = config.strtok(NULL, seps);
		//	if (token == NULL)
		//	{
		//		char *msg = "Loading Configuration File Failed.\n";
		//		DBGOUT(msg);
		//		MessageBox(NULL, msg, "Error", MB_OK);
		//		return FALSE;
		//	}
		//	m_nPortVoIPNtoNServer = atoi(token);
		//}
		else if (stricmp(token, "CheckLogServer") == 0)
		{
			token = config.strtok(NULL, seps);

			if (token == NULL || (stricmp(token, "TRUE") != 0 && stricmp(token, "FALSE") != 0))
			{
				char	szSystemLog[256];
				sprintf(szSystemLog, "Loading Configuration File Failed: CheckLogServer\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}

			if (stricmp(token, "TRUE") == 0)
			{
				m_bCheckLogServer = TRUE;
			}
			else if (stricmp(token, "FALSE") == 0)
			{
				m_bCheckLogServer = FALSE;
			}
			else
			{
				// unreachable
				return FALSE;
			}
		}
		else if(stricmp(token, "ServerGroupName") == 0)
		{
			token = config.strtok(NULL, seps);
			if(strcmp(token, "") != 0)
			{
				this->SetServerGroupName((char*)token);
			}
		}
// 2008-09-24 by cmkwon, global.cfg Çü½Ä ¼öÁ¤ - ServerGroupInfo ·Î ¼öÁ¤ ÇÔ.
// 		else if(stricmp(token, "ServerGroup") == 0)
// 		{
// 			token = config.strtok(NULL, seps);
// 			if (token == NULL)
// 			{
// 				char	szSystemLog[256];
// 				sprintf(szSystemLog, "[Error] CPreIOCP::LoadAllServers Config Initialization Error2: ServerGroup\r\n");
// 				WriteSystemLog(szSystemLog);
// 				DBGOUT(szSystemLog);
// 				return FALSE;
// 			}
// 
// 			char szGroupName[SIZE_MAX_SERVER_NAME];
// 			util::strncpy(szGroupName, token, SIZE_MAX_SERVER_NAME);
// 
// 			BOOL nServerIDforMGame = 10061;
// 			token = config.strtok(NULL, seps);
// 			if(token)
// 			{
// 				nServerIDforMGame = atoi(token);
// 			}
// 
// 			if (FALSE == InsertServerGroupforMGame(szGroupName, nServerIDforMGame))
// 			{				
// 				DBGOUT("[ERROR] Already ServerGroupName : %20s, ServerID(%5d)\n", szGroupName, nServerIDforMGame);
// 			}
// 		}
		else if(0 == stricmp(token, "IsTestServer"))
		{
			token = config.strtok(NULL, seps);
			if(token == NULL)
			{
				m_bIsTestServer = FALSE;
			}
			else
			{
				if (stricmp(token, "FALSE") == 0)
				{
					m_bIsTestServer = FALSE;
				}
				else
				{
					m_bIsTestServer = TRUE;
				}
			}
		}
		else if(0 == stricmp(token, "MainORTestServerName"))		// 2007-04-09 by cmkwon
		{
			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szMainORTestServerName, token, SIZE_MAX_SERVER_NAME);
			}
		}
		else if(0 == stricmp(token, "GamePublisher"))				// 2007-04-09 by cmkwon
		{
			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szGamePublisherName, token, SIZE_MAX_GAME_PUBLISHER_NAME);
			}
		}
		else if(0 == stricmp(token, "IsJamboreeServer"))				// 2007-04-09 by cmkwon
		{
			token = config.strtok(NULL, seps);
			if(token == NULL)
			{
				m_bIsJamboreeServer = FALSE;
			}
			else
			{
				if (stricmp(token, "FALSE") == 0)
				{
					m_bIsJamboreeServer = FALSE;
				}
				else
				{
					m_bIsJamboreeServer = TRUE;
				}
			}
		}
		else if(0 == stricmp(token, "CashShopServiceFlag"))
		{// 2007-01-10 by cmkwon
			
			token = config.strtok(NULL, seps);
			if(token == NULL)
			{
				m_bCashShopServiceFlag = FALSE;
			}
			else
			{
				if (0 == stricmp(token, "TRUE"))
				{
					m_bCashShopServiceFlag = TRUE;
				}
				else
				{
					m_bCashShopServiceFlag = FALSE;
				}
			}
		}
		else if(0 == stricmp(token, "UseSecondaryPasswrodSystemFlag"))
		{// 2007-09-12 by cmkwon, º£Æ®³² 2Â÷ÆÐ½º¿öµå ±¸Çö - field.cfg Çü½Ä Ãß°¡
			
			token = config.strtok(NULL, seps);
			if(token == NULL)
			{
				m_bUseSecondaryPasswordSystemFlag = FALSE;
			}
			else
			{
				if (0 == stricmp(token, "TRUE"))
				{
					m_bUseSecondaryPasswordSystemFlag = TRUE;
				}
				else
				{
					m_bUseSecondaryPasswordSystemFlag = FALSE;
				}
			}
		}
		// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ¾Æ·¹³ª ¼­¹ö Á¤º¸
		else if(0 == stricmp(token, "ArenaServerFlag"))
		{
			token = config.strtok(NULL, seps);
			if(NULL == token)
			{
				char	szSystemLog[256];
				sprintf(szSystemLog, "Loading Configuration File Failed: ArenaServerLoading\r\n");
				this->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
			
			if(0 == stricmp(token, "TRUE"))
			{
				m_sArenaServerInfo.ArenaFieldServerCheck	= TRUE;
			}
			else if(0 == stricmp(token, "FALSE"))
			{// 2007-12-26 by dhjin, ¾Æ·¹³ª ¼­¹ö Á¤º¸ ¼³Á¤
				m_sArenaServerInfo.ArenaFieldServerCheck	= FALSE;
				// 2007-12-26 by dhjin, ¾Æ·¹³ª ¼­¹ö ÀÌ¸§ ¼³Á¤
				token = config.strtok(NULL, seps);
				if(NULL == token)
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, "Loading Configuration File Failed: ArenaServerName\r\n");
					this->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}				
				util::strncpy(m_sArenaServerInfo.ArenaServerName, token, SIZE_MAX_SERVER_NAME);
				// 2007-12-26 by dhjin, ¾Æ·¹³ª ¼­¹ö ID ¼³Á¤
				token = config.strtok(NULL, seps);
				if(NULL == token)
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, "Loading Configuration File Failed: ArenaServerID\r\n");
					this->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}
				m_sArenaServerInfo.ArenaServerID	= atoi(token);
				// 2007-12-26 by dhjin, ¾Æ·¹³ª Field ¼­¹ö IP ¼³Á¤
				token = config.strtok(NULL, seps);
				if(NULL == token)
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, "Loading Configuration File Failed: ArenaFieldServerIP\r\n");
					this->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}				
				util::strncpy(m_sArenaServerInfo.ArenaFieldServerIP, token, SIZE_MAX_IPADDRESS);
				// 2007-12-26 by dhjin, ¾Æ·¹³ª Field ¼­¹ö Port ¼³Á¤
				token = config.strtok(NULL, seps);
				if(NULL == token)
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, "Loading Configuration File Failed: ArenaFieldServerPort\r\n");
					this->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}
				m_sArenaServerInfo.ArenaFieldServerPort	= atoi(token);
				// 2007-12-26 by dhjin, ¾Æ·¹³ª IM ¼­¹ö IP ¼³Á¤
				token = config.strtok(NULL, seps);
				if(NULL == token)
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, "Loading Configuration File Failed: ArenaIMServerIP\r\n");
					this->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}				
				util::strncpy(m_sArenaServerInfo.ArenaIMServerIP, token, SIZE_MAX_IPADDRESS);
				// 2007-12-26 by dhjin, ¾Æ·¹³ª IM ¼­¹ö Port ¼³Á¤
				token = config.strtok(NULL, seps);
				if(NULL == token)
				{
					char	szSystemLog[256];
					sprintf(szSystemLog, "Loading Configuration File Failed: ArenaIMServerPort\r\n");
					this->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}
				m_sArenaServerInfo.ArenaIMServerPort	= atoi(token);
			}
		}// 2007-12-26 by dhjin, ¾Æ·¹³ª ÅëÇÕ - ¾Æ·¹³ª ¼­¹ö Á¤º¸
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_IP))
		{// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CFieldGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_IP, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			util::strncpy(m_szDBServerIP, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_PORT))
		{// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CFieldGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_PORT, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			m_nDBServerPort = atoi(token);			
		}
		else if(0 == stricmp(token, CONFIG_NAME_DB_SERVER_DATABASE_NAEE))
		{// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CFieldGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_DB_SERVER_DATABASE_NAEE, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			util::strncpy(m_szDBServerDatabaseName, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, CONFIG_NAME_LOGDB_DB_SERVER_IP))
		{// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CFieldGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_LOGDB_DB_SERVER_IP, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			util::strncpy(m_szLogDBDBServerIP, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, CONFIG_NAME_LOGDB_DB_SERVER_PORT))
		{// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CFieldGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_LOGDB_DB_SERVER_PORT, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			m_nLogDBDBServerPort = atoi(token);			
		}
		else if(0 == stricmp(token, CONFIG_NAME_LOGDB_DB_SERVER_DATABASE_NAEE))
		{// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
			
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				char szErr[1024];
				sprintf(szErr, "[ERROR] CFieldGlobal::LoadConfiguration error !! %s, %s\r\n", CONFIG_NAME_LOGDB_DB_SERVER_DATABASE_NAEE, buffBackup);
				MessageBox(NULL, szErr, "Error", MB_OK);
				return FALSE;
			}
			
			util::strncpy(m_szLogDBDBServerDatabaseName, token, SIZE_MAX_ODBC_CONN_STRING);
		}
		else if(0 == stricmp(token, "AllowedToolIP"))
		{// 2008-06-05 by cmkwon, AdminTool, Monitor Á¢±Ù °¡´É IP¸¦ server config file ¿¡ ¼³Á¤ÇÏ±â - 
			
			token = config.strtok(NULL, seps);			
			if (token == NULL)
			{
				continue;
			}
			this->AddAllowedToolIP(token);
		}
		else if(0 == stricmp(token, "BillingDBServerIP"))
		{// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 

			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szBillingDBServerIP, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if(0 == stricmp(token, "BillingDBServerPort"))
		{// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
			token = config.strtok(NULL, seps);
			if(token)
			{
				m_nBillingDBServerPort = atoi(token);
			}
		}
		else if(0 == stricmp(token, "BillingDBServerDatabaseName"))
		{// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szBillingDBServerDatabaseName, token, SIZE_MAX_ODBC_CONN_STRING);
			}
		}
		else if(0 == stricmp(token, "BillingDBServerUserID"))
		{// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szBillingDBServerUserID, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szBillingDBServerUserID, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szBillingDBServerUserID, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(0 == stricmp(token, "BillingDBServerPassword"))
		{// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
			token = config.strtok(NULL, seps);
			if(token)
			{
				util::strncpy(m_szBillingDBServerPassword, token, SIZE_MAX_ODBC_CONN_STRING);
				///////////////////////////////////////////////////////////////////////////////
				// 2008-09-01 by cmkwon, global.cfg ÆÄÀÏ¿¡ DB ID/PWD ¾ÏÈ£È­ - 
				BYTE byEncodedBinary[1024];
				util::zero(byEncodedBinary, 1024);
				/*if(XOR::XORString2Binary(byEncodedBinary, (char*)token))
				{
					util::zero(m_szBillingDBServerPassword, SIZE_MAX_ODBC_CONN_STRING);
					XOR::XOREncode((BYTE*)m_szBillingDBServerPassword, byEncodedBinary, strlen(token)/2, STR_XOR_KEY_STRING_DB_ID_PWD);
				}*/
			}
		}
		else if(stricmp(token, "ServerGroupInfo") == 0)
		{// 2008-09-24 by cmkwon, global.cfg Çü½Ä ¼öÁ¤ - ServerGroup À» ¼öÁ¤ÇÑ °ÍÀÓ
			
			// 2008-09-24 by cmkwon, Syntax						= [ServerGroupName]			[ServerGroupID]		[Enable Flag]
			token = config.strtok(NULL, seps);
			if (NULL == token)
			{
				continue;
			}
			char szTemp[CONFIG_SIZE_BUFF];
			util::strncpy(szTemp, token, CONFIG_SIZE_BUFF);
			vectString tokenList;
			CConfig::GetTokenList(&tokenList, szTemp, seps);
			if(3 > tokenList.size())
			{
				if(0 != tokenList.size())
				{
					g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CFieldGlobal::LoadConfiguration# error !! %s\r\n", buffBackup);
					return FALSE;
				}
				continue;
			}
			
			if(SIZE_MAX_SERVER_NAME <= strlen(tokenList[0].c_str()))
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "[Error] CFieldGlobal::LoadConfiguration# longer than max servergroupname !! %s\r\n", buffBackup);
				return FALSE;
			}
			
			char	szGroupName[SIZE_MAX_SERVER_NAME];
			util::strncpy(szGroupName, tokenList[0].c_str(), SIZE_MAX_SERVER_NAME);
			int		nMGameServerID		= atoi(tokenList[1].c_str());			
			BOOL	bEnableServerGroup	= FALSE;
			if(0 == stricmp(tokenList[2].c_str(), "TRUE")) bEnableServerGroup	= TRUE;

			g_pGlobal->WriteSystemLogEX(TRUE, "	ServerGroup: %20s, Enable(%d)\r\n", szGroupName, bEnableServerGroup);
		}
		// 2009-06-03 by cmkwon, ¼¼·Â ¼±ÅÃ½Ã Á¦ÇÑ ½Ã½ºÅÛ »ç¿ë ¿©ºÎ ÇÃ·¡±× Ãß°¡ - 
		else if(0 == stricmp(token, "UseInflSelectionRestrictSystem"))
		{
			token = config.strtok(NULL, seps);
			
			g_pFieldGlobal->SetUseInflSelectionRestrictSystem(token && stricmp("TRUE", token) == 0);
		}
		// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
		else if(0 == stricmp(token, "NPCServer"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if(strcmp(token, "") != 0)
			{
				this->SetIPNPCServer((char*)token);
			}
		}
		// end 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
		// 2015-09-14 Future, disabled Monster Maps during Influence war
		/*else if (0 == stricmp(token, "InfluenceWarDisabledMonsterMaps"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if (strcmp(token, "") != 0)
			{
				char* tempMapCfgCopy = new char[strlen(token) + 1];
				strcpy_s(tempMapCfgCopy, strlen(token) + 1, token);

				char* mapIdxToken = strtok(tempMapCfgCopy, ",");
				while (mapIdxToken != NULL)
				{
					// Convert map idx to int and add it to the vec
					this->AddInfluenceWarDisabledMonsterMap(atoi(mapIdxToken));

					mapIdxToken = strtok(NULL, ",");
				}

				delete[] tempMapCfgCopy;
			}
		}*/

		// 2015-11-25 Future, Multiple IP Restriction System
#ifdef S_IP_UNIQUE_CONNECTION
		else if (0 == stricmp(token, "MultipleIpRestriction"))
		{
			token = config.strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Configuration File Failed.\n";
				DBGOUT(msg);
				MessageBox(NULL, msg, "Error", MB_OK);
				return FALSE;
			}
			if (strcmp(token, "") != 0)
			{
				if (stricmp(token, "TRUE") == 0)
					m_bMultipleIPRestriction = TRUE;
				else
					m_bMultipleIPRestriction = FALSE;
			}
		}
#endif // S_IP_UNIQUE_CONNECTION

		// 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡
#ifdef S_WEB_CASHSHOP_SERVER_MODULE_HSKIM
		else if(0 == stricmp(token, "IsWebInterface"))
		{
			token = config.strtok(NULL, seps);
			if (token
				&& 0 == stricmp("TRUE", token))
			{
				g_pFieldGlobal->SetWebInterface(TRUE);
			}
			else
			{
				g_pFieldGlobal->SetWebInterface(FALSE);
			}
		}
		else if(0 == stricmp(token, "IsWebCashShop"))
		{
			token = config.strtok(NULL, seps);
			if (token
				&& 0 == stricmp("TRUE", token))
			{
				g_pFieldGlobal->SetWebCashShop(TRUE);
			}
			else
			{
				g_pFieldGlobal->SetWebCashShop(FALSE);
			}
		}
		else if(0 == stricmp(token, "WebInterfacePort"))
		{
			token = config.strtok(NULL, seps);
			if(token)
			{
				g_pFieldGlobal->SetPortWebInterface(atoi(token));
			}
		}
#endif
		// end 2013-03-13 by hskim, À¥ Ä³½Ã »óÁ¡
		else
		{
			server::log(true, "Unrecognized token in configuration: \"%s\"\r\n", token);

			// configuration file error!
			//assert(0);
		}
	}
	fin.close();

	if(strcmp(this->GetIPLocal(), "") == 0
		|| strcmp(this->GetFieldServerGroupName(), "") == 0
		|| strcmp(this->GetIPPreServer(), "") == 0
		|| strcmp(this->GetIPIMServer(), "") == 0
		|| strcmp(this->GetIPLogServer(), "") == 0
		|| strcmp(this->GetIPNPCServer(), "") == 0		// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
		//|| strcmp(this->GetIPVoIP1to1Server(), "") == 0
		//|| strcmp(this->GetIPVoIPNtoNServer(), "") == 0
		|| this->GetPortListening() == 0
		|| this->GetPortLogServer() == 0
		|| this->GetPortIMServer() == 0
		|| this->GetPortPreServer() == 0)
		//|| this->GetPortVoIP1to1Server() == 0
		//|| this->GetPortVoIPNtoNServer() == 0)
	{
		char	szError[1024];
		sprintf(szError, "[Error] LoadConfiguration() Loading Configuration Failed, LocalIP[%s] FieldServerGroupName[%s] PreServer[%s:%d] LogServer[%s:%d] IMServer[%s:%d] NPCServer[%s]\r\n"		// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
			, this->GetIPLocal(), this->GetFieldServerGroupName(),
			this->GetIPPreServer(), this->GetPortPreServer(),
			this->GetIPLogServer(), this->GetPortLogServer(),
			this->GetIPIMServer(), this->GetPortIMServer(),
			this->GetIPNPCServer());		// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
		this->WriteSystemLog(szError);
		DBGOUT(szError);

		MessageBox(NULL, szError, "ERROR", MB_OK);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-03 by cmkwon, °ÔÀÓ ·Î±× DB µû·Î ±¸Ãà ÇÏ±â ¹ö±× ¼öÁ¤ - Ã¼Å© Ãß°¡
// 2008-07-11 by cmkwon, MySQL Áö¿øÀ» À§ÇØ ¼öÁ¤(IP,Port ·Î DB ¼­¹ö ¿¬°á) - 
//	if(0 == strcmp((LPSTR)this->GetLogDBODBCDSN(), "")
	if(0 == strcmp((LPSTR)this->GetLogDBODBCUID(), "")
		|| 0 == strcmp((LPSTR)this->GetLogDBODBCPASSWORD(), ""))
	{
		char	szError[1024];
		sprintf_s(szError, "[Error] CFieldGlobal::LoadConfiguration You must set LogDBServer !!, LOGDB_ODBCUID(%s) LOGDB_ODBCPASSWORD(%s)\r\n",
			GetLogDBODBCUID(), GetLogDBODBCPASSWORD());
		MessageBox(NULL, szError, "ERROR", MB_OK);
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-26 by cmkwon, º£Æ®³² VTC-Intecom_Viet 2Â÷ ÆÐ½º¿öµå ½Ã½ºÅÛ ¹ö±× ¼öÁ¤ - ¾Æ·¹³ª ¼­¹ö±ºÀÏ °æ¿ì 2Â÷ ÆÐ½º¿öµå¸¦ »ç¿ëÇÏÁö ¾Êµµ·Ï ÇÑ´Ù.
	if(IsArenaServer())
	{
		m_bUseSecondaryPasswordSystemFlag	= FALSE;	// 2008-08-26 by cmkwon, º£Æ®³² VTC-Intecom_Viet 2Â÷ ÆÐ½º¿öµå ½Ã½ºÅÛ ¹ö±× ¼öÁ¤ - FALSE·Î ¼³Á¤
	}

#ifdef _AUTO_DGA
	return LoadDGAConfiguration();
#else
	return true;
#endif
}

BOOL CFieldGlobal::SendLogFieldServer2LogServer(BYTE *szLogMsg, int nLength)
{
	if(m_vectorF2LWSocketPtr.empty())
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [ERROR] Nothing F2L socket !! ArraySize(%d) \r\n", m_vectorF2LWSocketPtr.size());
		return FALSE;
	}

	int nCurF2LSocketIndex = m_nSendIndexF2LWSocket;
	for(int i = 0; i < SIZE_FIELD2LOG_WINSOCKET; i++)
	{
		int nIdx = (nCurF2LSocketIndex + i)%m_vectorF2LWSocketPtr.size();
		if(NULL == m_vectorF2LWSocketPtr[nIdx]
			|| FALSE == m_vectorF2LWSocketPtr[nIdx]->IsConnected())
		{
			continue;
		}
		int nSize = m_vectorF2LWSocketPtr[nIdx]->GetSendBufferCounts();
		if(nSize > 500)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] LogError, Index(%3d) CLogWinSocket(0x%X) SendBufferSize(%4d) \r\n", nIdx, m_vectorF2LWSocketPtr[nIdx], nSize);
		}
		if (m_vectorF2LWSocketPtr[nIdx]->Write(szLogMsg, nLength))
		{
			m_nSendIndexF2LWSocket = (++nIdx)%m_vectorF2LWSocketPtr.size();
			return TRUE;
		}
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] F2L send to LogServer fail !!, Index(%3d) CLogWinSocket(0x%X) SendBufferSize(%4d), SockHandle(%d) IsConnected(%d) \r\n"
			, nIdx, m_vectorF2LWSocketPtr[nIdx], nSize, m_vectorF2LWSocketPtr[nIdx]->GetSocketHandle(), m_vectorF2LWSocketPtr[nIdx]->IsConnected());
	}

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] F2L send to LogServer all fail !!, ArraySize(%d) \r\n", m_vectorF2LWSocketPtr.size());	
	return FALSE;
}

BOOL CFieldGlobal::SendLogFieldServer2LogServerORGFile(BYTE *szLogMsg, int nLength)
{
	return TRUE;
}


void CFieldGlobal::OnF2LAsyncEvent(SOCKET i_hSocket, LONG i_nEvent)
{
	int nSize = m_vectorF2LWSocketPtr.size();
	for(int i=0; i < nSize; i++)
	{
		if(m_vectorF2LWSocketPtr[i]
			&& m_vectorF2LWSocketPtr[i]->GetSocketHandle() == i_hSocket)
		{
			if(FD_CLOSE == WSAGETSELECTEVENT(i_nEvent))
			{
				// 2009-05-06 by cmkwon, ½Ã½ºÅÛ ·Î±× Ãß°¡(F2L°ü·Ã) - 
				this->WriteSystemLogEX(TRUE, "[Notify] ON FD_Close(%s:%d) SockH(%ld) ErrorCode(%d) \r\n", m_vectorF2LWSocketPtr[i]->m_szPeerIP, m_vectorF2LWSocketPtr[i]->m_nPeerPort, m_vectorF2LWSocketPtr[i]->GetSocketHandle(), WSAGETSELECTERROR(i_nEvent));
			}
			m_vectorF2LWSocketPtr[i]->OnAsyncEvent(i_nEvent);
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-04-20 by cmkwon, F2L °ü·Ã ½Ã½ºÅÛ ¼öÁ¤ - 
/// \author		cmkwon
/// \date		2009-04-20 ~ 2009-04-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::OnF2LClosed(CLogWinSocket *i_pF2LWinSoc)
{
	if(NULL == i_pF2LWinSoc)
	{
		return FALSE;
	}

	int nSize = m_vectorF2LWSocketPtr.size();
	for(int i=0; i < nSize; i++)
	{
		if(m_vectorF2LWSocketPtr[i]
			&& m_vectorF2LWSocketPtr[i] == i_pF2LWinSoc)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] F2L socket closed ! Index(%2d) CLogWinSocket(0x%X) \r\n", i, i_pF2LWinSoc);
			i_pF2LWinSoc->CloseSocket();			
			return TRUE;
		}
	}
	return FALSE;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldGlobal::Send2PreServer(BYTE *i_pData, int i_nDataLen)
/// \brief		// 2007-11-19 by cmkwon, °èÁ¤ ºí·°/ÇØÁ¦ ¸í·É¾î·Î °¡´ÉÇÑ ½Ã½ºÅÛ ±¸Çö - CFieldGlobal::Send2PreServer() Ãß°¡
/// \author		cmkwon
/// \date		2008-01-31 ~ 2008-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::Send2PreServer(BYTE *i_pData, int i_nDataLen)
{
	CPreWinSocket *pF2PreSoc = this->GetField2PreWinSocket();
	if(NULL == pF2PreSoc
		|| FALSE == pF2PreSoc->IsConnected())
	{
		return FALSE;
	}

	return pF2PreSoc->Write(i_pData, i_nDataLen);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldGlobal::LoadResObjCheckListW(BOOL i_bReloadOnlyRexTexDirectory/*=FALSE*/)
/// \brief		// 2008-09-08 by cmkwon, SCMonitor¿¡¼­ ReloadVersionInfo½Ã¿¡ ÀÏºÎ Ã¼Å©¼¶ÆÄÀÏ(.\Res-Tex\*.*)µµ ¸®·ÎµåÇÏ±â - 
/// \author		cmkwon
/// \date		2008-09-08 ~ 2008-09-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::LoadResObjCheckListW(BOOL i_bReloadOnlyRexTexDirectory/*=FALSE*/)
{
	if(NULL == m_pGIOCP)
	{
		return FALSE;
	}

	CFieldIOCP *pFIOCP = (CFieldIOCP*)(m_pGIOCP);

	if(NULL == pFIOCP
		|| FALSE == pFIOCP->GetListeningFlag()
		|| FALSE == pFIOCP->GetServiceStartFlag())
	{
		return FALSE;
	}

	return pFIOCP->LoadResObjCheckList(i_bReloadOnlyRexTexDirectory);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldGlobal::IsArenaServer(void)
/// \brief		// 2009-03-05 by cmkwon, ¼±ÀüÆ÷°í ¾Æ·¹³ª ¼­¹ö Ã³¸® - 
/// \author		cmkwon
/// \date		2009-03-05 ~ 2009-03-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::IsArenaServer(void)
{
	return m_sArenaServerInfo.ArenaFieldServerCheck;
}

BOOL CFieldGlobal::ConnectAllF2LWSocket(char *i_szSvrIP, int i_nSvrPort)
{
	int nSize = m_vectorF2LWSocketPtr.size();
	for(int i=0; i < nSize; i++)
	{
		if(m_vectorF2LWSocketPtr[i]
			&& FALSE == m_vectorF2LWSocketPtr[i]->IsConnected())
		{
			m_vectorF2LWSocketPtr[i]->Connect(i_szSvrIP, i_nSvrPort);
		}
	}

	return TRUE;
}

BOOL CFieldGlobal::ReConnectAllF2LWSocket(char *i_szSvrIP, int i_nSvrPort)
{
	int nSize = m_vectorF2LWSocketPtr.size();
	for(int i=0; i < nSize; i++)
	{
		if(m_vectorF2LWSocketPtr[i]
			&& INVALID_SOCKET == m_vectorF2LWSocketPtr[i]->GetSocketHandle())
		{
			m_vectorF2LWSocketPtr[i]->Connect(i_szSvrIP, i_nSvrPort);
		}
	}

	return TRUE;
}

BOOL CFieldGlobal::IsConnectedAllF2LWSocket(void)
{
	int nSize = m_vectorF2LWSocketPtr.size();
	for(int i=0; i < nSize; i++)
	{
		if(NULL == m_vectorF2LWSocketPtr[i]
			|| FALSE == m_vectorF2LWSocketPtr[i]->IsConnected())
		{
			return FALSE;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldGlobal::GetUseSecondaryPasswordSystemFlag(void)
/// \brief		// 2007-09-12 by cmkwon, º£Æ®³² 2Â÷ÆÐ½º¿öµå ±¸Çö - CFieldGlobal ¿¡ ¸â¹ö ÇÔ¼ö Ãß°¡
/// \author		cmkwon
/// \date		2007-09-12 ~ 2007-09-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::GetUseSecondaryPasswordSystemFlag(void)
{
	return m_bUseSecondaryPasswordSystemFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldGlobal::SetDBServerGroup(SDBSERVER_GROUP *i_pDBServGroup)
/// \brief		// 2008-04-29 by cmkwon, ¼­¹ö±º Á¤º¸ DB¿¡ Ãß°¡(½Å±Ô °èÁ¤ Ä³¸¯ÅÍ »ý¼º Á¦ÇÑ ½Ã½ºÅÛÃß°¡) - CFieldGlobal::SetDBServerGroup() Ãß°¡
/// \author		cmkwon
/// \date		2008-04-29 ~ 2008-04-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldGlobal::SetDBServerGroup(SDBSERVER_GROUP *i_pDBServGroup)
{
	m_DBServerGroup		= *i_pDBServGroup;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CFieldGlobal::GetBillingDBServerIP(void)
/// \brief		// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
/// \author		cmkwon
/// \date		2008-07-30 ~ 2008-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CFieldGlobal::GetBillingDBServerIP(void)
{
	return m_szBillingDBServerIP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CFieldGlobal::GetBillingDBServerPort(void)
/// \brief		// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
/// \author		cmkwon
/// \date		2008-07-30 ~ 2008-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldGlobal::GetBillingDBServerPort(void)
{
	return m_nBillingDBServerPort;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CFieldGlobal::GetBillingDBServerDatabaseName(void)
/// \brief		// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
/// \author		cmkwon
/// \date		2008-07-30 ~ 2008-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CFieldGlobal::GetBillingDBServerDatabaseName(void)
{
	return m_szBillingDBServerDatabaseName;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CFieldGlobal::GetBillingDBServerUserID(void)
/// \brief		// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
/// \author		cmkwon
/// \date		2008-07-30 ~ 2008-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CFieldGlobal::GetBillingDBServerUserID(void)
{
	return m_szBillingDBServerUserID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char *CFieldGlobal::GetBillingDBServerPassword(void)
/// \brief		// 2008-07-30 by cmkwon, Yedang_Kor ºô¸µ DBServer Á÷Á¢ ¿¬°áÇÏµµ·Ï ¼öÁ¤ - 
/// \author		cmkwon
/// \date		2008-07-30 ~ 2008-07-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char *CFieldGlobal::GetBillingDBServerPassword(void)
{
	return m_szBillingDBServerPassword;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldGlobal::SendArenaServerMFSInfo()
/// \brief		¾Æ·¹³ª ÅëÇÕ - ¾Æ·¹³ª ¼­¹ö·Î ÇÊµå ¼­¹ö Á¤º¸ Àü¼Û
/// \author		dhjin
/// \date		2008-01-17
///////////////////////////////////////////////////////////////////////////////
void CFieldGlobal::SendArenaServerMFSInfo()
{
	MessageData<T_FtoA_MFSINFO> pMsgFtoAConn;

	pMsgFtoAConn->MFS_Port	= GetPortListening();

	util::strncpy(pMsgFtoAConn->MFS_Name, GetFieldServerGroupName());

	util::strncpy(pMsgFtoAConn->MFS_IP, GetPublicIPLocal());

	m_pField2ArenaFieldWinSocket->Write(pMsgFtoAConn);

	server::log(" [Notify] CFieldGlobal::InitServerSocket_ send to ArenaServer !!\r\n");
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldGlobal::GetUseInflSelectionRestrictSystem(void)
/// \brief		// 2009-06-03 by cmkwon, ¼¼·Â ¼±ÅÃ½Ã Á¦ÇÑ ½Ã½ºÅÛ »ç¿ë ¿©ºÎ ÇÃ·¡±× Ãß°¡ - 
/// \author		cmkwon
/// \date		2009-06-03
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::GetUseInflSelectionRestrictSystem(void)
{
	return m_bUseInflSelectionRestrictSystem;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldGlobal::SetUseInflSelectionRestrictSystem(BOOL i_bFlag)
/// \brief		// 2009-06-03 by cmkwon, ¼¼·Â ¼±ÅÃ½Ã Á¦ÇÑ ½Ã½ºÅÛ »ç¿ë ¿©ºÎ ÇÃ·¡±× Ãß°¡ - 
/// \author		cmkwon
/// \date		2009-06-03 ~ 2009-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldGlobal::SetUseInflSelectionRestrictSystem(BOOL i_bFlag)
{
	m_bUseInflSelectionRestrictSystem	= i_bFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldGlobal::checkSHUTDOWNMINORS(int i_birthdayYears /* »ýÀÏ */ , ATUM_DATE_TIME currentTime/* ÇöÀç ½Ã°£ */)
/// \brief		// start 2011-11-03 by shcho, yedang ¼Ë´Ù¿îÁ¦ ±¸Çö
/// \author		shcho
/// \date		20111110 ~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldGlobal::checkSHUTDOWNMINORS(int i_birthdayYears /* »ýÀÏ */ , ATUM_DATE_TIME currentTime/* ÇöÀç ½Ã°£ */) 
{ // ³âµµº° Ã¼Å© ÇÔ¼ö
	int nCurrentTime = (currentTime.Year*10000)+(currentTime.Month*100)+(currentTime.Day);
	
	if(10000 > i_birthdayYears)
	{ // ³âµµÀÏ¶§ 
		i_birthdayYears = i_birthdayYears * 10000; // ¸¸ 16¼¼ (17»ì)
	}
	
	// ¾Æ´Ï¸é ¹«Á¶²« ¼öÇàÇÑ´Ù.
	if(160000 <= nCurrentTime-i_birthdayYears)
	{ // ¼ºÀÎ 
		return FALSE;
	}
	else
	{ // 16¼¼ ¹Ì¸¸ 
		return TRUE;
	}
}

// 2013-04-01 by hskim, NPCServer ÆÐÅ¶À¸·Î À§Á¶ °ø°Ý ¹æ¾î
BOOL CFieldGlobal::CheckAllowedNPCServerIP(const char *i_szIP)
{
	if (strncmp(GetIPNPCServer(), i_szIP, strlen(GetIPNPCServer())) == 0)
	
		return true;

	server::log(" [Notify] CheckAllowedNPCServerIP ==> Not Allowed (%s) !!\r\n", i_szIP);

	return false;
}

// 2015-09-14 Future, disabled Monster Maps during Influence war
/*void CFieldGlobal::AddInfluenceWarDisabledMonsterMap(MapIndex_t mapIndex)
{
	m_vecInfluenceWarDisabledMonsterMaps.push_back(mapIndex);
}

// 2015-09-14 Future, disabled Monster Maps during Influence war
const vector<MapIndex_t>& CFieldGlobal::GetInfluenceWarDisabledMonsterMaps()
{
	return m_vecInfluenceWarDisabledMonsterMaps;
}*/

#ifdef S_IP_UNIQUE_CONNECTION
BOOL CFieldGlobal::GetMultipleIPRestrictionFlag()
{
	return m_bMultipleIPRestriction;
}

#endif // S_IP_UNIQUE_CONNECTION
