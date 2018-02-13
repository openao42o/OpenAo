// NPCIOCP.cpp: implementation of the CNPCIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCIOCP.h"
#include "NPCIOCPSocket.h"
#include "MapWorkspace.h"
#include "MonsterDBAccess.h"
#include "NPCMapProject.h"
#include "NPCGlobal.h"
#include "AtumSJ.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCIOCP::CNPCIOCP(int nPort, char *szLocalIP) :
	CIOCP { CLIENT_INDEX_START_NUM, SIZE_MAX_NPCSERVER_SESSION, nPort, szLocalIP, ST_NPC_SERVER, sizeof(CNPCIOCPSocket) },
	m_bSummonVCNJacoMonster { false },
	m_bSummonANIJacoMonster { false },
	m_vectorClientInfo { SIZE_MAX_FIELDSERVER_SESSION }
{
	CNPCIOCPSocket::ms_pNPCIOCP		= this;

	auto pNPCIOCPSocket = new CNPCIOCPSocket[m_dwArrayClientSize];

	for (auto i = 0; i < m_dwArrayClientSize; i++) pNPCIOCPSocket[i].InitIOCPSocket(i);

	m_pArrayIOCPSocket = pNPCIOCPSocket;

	m_mapMonsterParameter.clear();
	m_mapItemInfo.clear();
	m_pNPCMapWorkspace = new CNPCMapWorkspace(this, SIZE_BLOCK_X_FOR_NPCSERVER, SIZE_BLOCK_Z_FOR_NPCSERVER);

	// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - CNPCMapChannel 에 있던것을 이동 함.
	for (auto i = 0; i < SIZE_MAX_FIELDSERVER_SESSION; i++)
	{		
		m_vectorClientInfo[i].ClientIndex	= i;
		m_vectorClientInfo[i].ResetClientInfo();
	}

}

CNPCIOCP::~CNPCIOCP()
{
	ListenerClose();
	CNPCIOCP::IOCPClean();

	delete[] static_cast<CNPCIOCPSocket*>(m_pArrayIOCPSocket);
	m_pArrayIOCPSocket = nullptr;

	delete m_pNPCMapWorkspace;
	m_pNPCMapWorkspace = nullptr;

	// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	delete m_pWayPointManager;
	m_pWayPointManager = nullptr;
}

BOOL CNPCIOCP::IOCPInit(void)
{

/*	// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
	char szServerName[SIZE_MAX_STRING_128] = { 0, };
	sprintf(szServerName, "NPCServer_%s", g_pNPCGlobal->GetServerGroupName());
	if( FALSE == g_pGlobal->CreateDuplicateRun(szServerName) )
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", szServerName, MB_OK);
		return FALSE;
	}
	// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//bRet = odbcStmt.Init((LPCSTR)g_pNPCGlobal->GetODBCDSN(), (LPCSTR)g_pNPCGlobal->GetODBCUID(), (LPCSTR)g_pNPCGlobal->GetODBCPASSWORD());
	
	CODBCStatement odbcStmt;
	
	bool bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CNPCIOCP::IOCPInit_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "DB connect fail@CNPCIOCP::IOCPInit()", "ERROR - SpaceCowboy NPCServer", NULL);
		return FALSE;
	}

	// Load AtumSJ Data
	if (!AtumSJ::LoadData(&odbcStmt))
	{
		MessageBox(NULL, "CAtumSJ::LoadData() fail", "ERROR - SpaceCowboy NPCServer", NULL);
		return FALSE;
	}

	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
	vectMapIndexList		vectArenaMapIndexList;
	bRet = CAtumDBHelper::LoadArenaMapList(&vectArenaMapIndexList, &odbcStmt);
	if (!bRet)
	{
		g_pNPCGlobal->WriteSystemLogEX(TRUE, "[Error] Load CAtumDBHelper::LoadArenaMapList_ !!");
		MessageBox(NULL, "[Error] Load CAtumDBHelper::LoadArenaMapList_ !!", "ERROR - SpaceCowboy NPCServer", NULL);
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	vectorInfinityModeInfo		tmVectInfinityModeInfo;
	bRet = CAtumDBHelper::DBLoadInfinityModeInfo(&odbcStmt, &tmVectInfinityModeInfo, &vectArenaMapIndexList);
	if (!bRet) {
		MessageBox(NULL, "[Error] Load CAtumDBHelper::DBLoadInfinityModeInfo fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}	

	g_pNPCGlobal->WriteSystemLogEX(TRUE, "[Notify] CNPCIOCP::IOCPInit# 60000! InfinityModeInfoListCnt(%d) ArenaMapIndexListCnt(%d) \r\n", tmVectInfinityModeInfo.size(), vectArenaMapIndexList.size());

// 2008-09-10 by cmkwon, 통합아레나 맵설정(map.cfg) 관련 수정 - 
//	bRet = m_pNPCMapWorkspace->LoadAllProject((CONFIG_ROOT+string("./map.cfg")).c_str());
#ifdef ARENA
	bRet = m_pNPCMapWorkspace->LoadAllProject((CONFIG_ROOT+string("./ArenaMap.cfg")).c_str(), g_pNPCGlobal->GetIsArenaServer(), &vectArenaMapIndexList);
#else
	bRet = m_pNPCMapWorkspace->LoadAllProject((CONFIG_ROOT+string("./map.cfg")).c_str(), g_pNPCGlobal->GetIsArenaServer(), &vectArenaMapIndexList);
#endif	
	if (!bRet || m_pNPCMapWorkspace->m_vectorPtrMapProject.empty())
	{
		MessageBox(NULL, "Failed to load map projects", "ERROR - NPC Server", NULL);

		return FALSE;
	}

	g_pNPCGlobal->WriteSystemLogEX(TRUE, "[Notify] CNPCIOCP::IOCPInit# 70000! MapProjectListCnt(%d) \r\n", m_pNPCMapWorkspace->m_vectorPtrMapProject.empty());

	// Load ItemInfo...
	LoadItemInfo();		// +1

	// Load MonsterInfo...(항상 LoadItemInfo 뒤에 처리되어야함)
	CMonsterDBAccess MonsterDBAccess;
	MonsterDBAccess.GetAllMonsters(m_mapMonsterParameter, &m_mapItemInfo, MONSTER_LOAD_TYPE_SIZE_FOR_SERVER);

	////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - HPAction 로딩
	bRet = CAtumDBHelper::LoadHPActionByDB(&odbcStmt, &m_mapHPAction);
	if (!bRet)
	{
		g_pNPCGlobal->WriteSystemLogEX(TRUE, "[Error] Load CAtumDBHelper::LoadHPActionByDB_ !!");
		MessageBox(NULL, "[Error] Load CAtumDBHelper::LoadHPActionByDB_ !!", "ERROR - SpaceCowboy NPCServer", NULL);
		return FALSE;
	}		

	m_nListenerPort = m_pNPCMapWorkspace->GetMapProjectByIndex(0)->m_sNPCListenPort;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - DB에서 로딩(CNPCIOCP::IOCPInit)
	mtvectSEVENT_MONSTER	eventMonsterList;
	if(FALSE == CAtumDBHelper::LoadEventMonster(&eventMonsterList, &odbcStmt))
	{
		server::log(true, "[Error] CNPCIOCP::IOCPInit_ LoadEventMonster error !!");
		return FALSE;
	}
	this->SetEventMonsterList(&eventMonsterList);

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	m_pWayPointManager = new CWayPointManager;

	if( m_pWayPointManager == NULL || FALSE == m_pWayPointManager->Create() )
	{
		server::log(true, "[Error] Create CNPCIOCP::IOCPInit_ CWayPointManager!!");

		return FALSE;
	}

	if ( FALSE == m_pWayPointManager->LoadDBWayPointData(&odbcStmt) )
	{
		server::log(true, "[Error] Load CAtumDBHelper::LoadDBWayPointData_ !!");

		return FALSE;
	}
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	odbcStmt.Clean();		// clean
	return CIOCP::IOCPInit();
}

void CNPCIOCP::IOCPClean()
{
	char szSystemLog[512];
	sprintf(szSystemLog, "CNPCIOCP::IOCPClean_1\r\n");
	DBGOUT(szSystemLog);
	g_pNPCGlobal->WriteSystemLog(szSystemLog);

	for (DWORD i = 0; i < m_dwArrayClientSize; i++)
		
		reinterpret_cast<CNPCIOCPSocket*>(m_pArrayIOCPSocket)[i].MapWorkerEnd();

	///////////////////////////////////////////////////////////////////////////////
	// 부모 클래스의 IOCPClean()을 처리한다
	CIOCP::IOCPClean();

	if (!m_mapItemInfo.empty())
	{
		for (auto& info : m_mapItemInfo) delete info.second;

		m_mapItemInfo.clear();
	}

	// 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	if(m_pWayPointManager) m_pWayPointManager->Clean();
}

SThreadInfo *CNPCIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);
	if(pTInfo
		&& !m_MonitorIOCPSocketPtrVector.empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_NM_SERVER_THREAD_STATUS, T_NM_SERVER_THREAD_STATUS, pSend, SendBuf);
		pSend->dwThreadId = pTInfo->dwThreadId;
		util::strncpy(pSend->szThreadComment, pTInfo->GetThreadCheckComment(pTInfo->enThreadCheckType), SIZE_MAX_THREAD_COMMENT);
		pSend->dwTimeGap = timeGetTime() - pTInfo->dwLastUseStartTick;
		SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_NM_SERVER_THREAD_STATUS));
	}
	return pTInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-08 by cmkwon, 서버에서 맵로딩시 AlivePacket 전송 추가 - 
/// \author		cmkwon
/// \date		2010-04-08 ~ 2010-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCIOCP::SendAlivePacket2OtherServers(void)
{
	MessageType_t nMsgTy = T_FN_CONNECT_ALIVE;
	g_pNPCGlobal->WriteN2FSocket((BYTE*)&nMsgTy, sizeof(nMsgTy));
}

BOOL CNPCIOCP::OpenUDPPortForOtherServer(void)
{
	//*
	CIOCPSocket *pIOCPSocket = NULL;
	CMapProject *pMapProject = NULL;

	for(int i=0; i < SIZE_MAX_UDP_CHANNEL_SESSION; i++)		// 2008-08-29 by cmkwon, FieldServ<->NPCServ 가 UDP 통신 할 채널수 - // 2006-12-07 by cmkwon, 변경(SIZE_MAX_MAP_PER_A_FIELD_SERVER->SIZE_MAX_NPCSERVER_SESSION)
	{
		pMapProject = m_pNPCMapWorkspace->GetMapProjectByIndex(i);
		if(NULL == pMapProject)
		{
			break;
		}

		pIOCPSocket = FindEmptyIOCPSocket(i);
		if(pIOCPSocket == NULL)
		{
			char	szError[1024];
			sprintf(szError, "[Error] CFieldIOCP::OpenUDPPortForOtherServer error, i[%d]\r\n", i);
			g_pNPCGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}

		if(pIOCPSocket->OpenUDPPort(pMapProject->m_sNPCUDPPort) == FALSE){ return FALSE;}
		pIOCPSocket->SetPeerAddress(pMapProject->m_strFieldIP, pMapProject->m_sFieldUDPPort);

		char	szSystemLog[256];
		sprintf(szSystemLog, "UDP Opened for Field Server(IP[%s], Port[%d]) SocketIndex[%d]\r\n",
			pMapProject->m_strFieldIP, pMapProject->m_sFieldUDPPort, i);
		g_pNPCGlobal->WriteSystemLog(szSystemLog);
		DBGOUT(szSystemLog);

		HANDLE hret = CreateIoCompletionPort((HANDLE)pIOCPSocket->m_hSocket, m_hCompletionPort, (DWORD)pIOCPSocket, 0);
		if(NULL == hret )
		{
			pIOCPSocket->Close(0x15000);

			int nErr = GetLastError();
			SetLastError(0);
			char	szError[1024];
			sprintf(szError, "[Error] CNPCIOCP::OpenUDPPortForOtherServer CreateIoCompletionPort() LastError[%d] hCompletionPort[%p] pIOCPSocket[%p]\r\n"
				, nErr, m_hCompletionPort, pIOCPSocket);
			g_pNPCGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}
		pIOCPSocket->Read();
		pIOCPSocket->SetPeerUDPReady(TRUE);
		((CNPCIOCPSocket*)pIOCPSocket)->InitializeNPCMap(pMapProject, &m_mapMonsterParameter);
		Sleep(200);
	}

	//*/

	// 2009-03-26 by cmkwon, 시스템 로그 추가 - 
	g_pGlobal->WriteSystemLogEX(TRUE, "  [Notify] Open all UDP Socket for FieldServer!! Count(%3d)\r\n", m_pNPCMapWorkspace->GetMapProjectCounts());

	return TRUE;
}

void CNPCIOCP::LoadItemInfo()
{
	SQLHENV		henv = SQL_NULL_HENV;
	SQLHDBC		hdbc = SQL_NULL_HDBC;
	SQLHSTMT	hstmt = SQL_NULL_HSTMT;

	RETCODE		ret;
	SQLINTEGER pcbNTS = SQL_NTS;
	SQLINTEGER cb[CB_COUNT_ITEM];
	ITEM		item;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);

	ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//	ret = SQLConnect(hdbc, g_pGlobal->GetODBCDSN(), SQL_NTS, g_pGlobal->GetODBCUID(), SQL_NTS, g_pGlobal->GetODBCPASSWORD(), SQL_NTS);

	char szConnectionString[512];
	char szOutConnectionString[512]; SQLSMALLINT pcb;
#if defined(DB_SERVER_MYSQL)
	sprintf(szConnectionString, "DRIVER={MySQL ODBC 5.1 Driver};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
		, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetDBServerDatabaseName());
#else
	sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
		, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetDBServerDatabaseName());
#endif
	ret = SQLDriverConnect(hdbc, g_pGlobal->GetMainWndHandle(), (SQLCHAR*)szConnectionString, strlen(szConnectionString), (SQLCHAR*)szOutConnectionString, 512, &pcb, SQL_DRIVER_NOPROMPT);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CNPCIOCP::LoadItemInfo_) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		CAtumDBManager::ProcessLogMessagesStatic(SQL_HANDLE_DBC, hdbc,"@CFieldIOCP::LoadItemInfo Failed!\r\n", FALSE);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}
	
	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);


	ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//ret = SQLExecDirect(hstmt, (UCHAR*)"SELECT * FROM ti_ITEM WITH (NOLOCK)", SQL_NTS);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0257, SQL_NTS);

	// bind columns
	CAtumDBHelper::BindColITEM(hstmt, item, cb);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		// error
		return;
	}

	// initialize to 0x00
	memset(&item, 0x00, sizeof(ITEM));
	while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		ITEM *pItem = new ITEM;
		memcpy(pItem, &item, sizeof(ITEM));

		m_mapItemInfo.insert(pair<INT, ITEM*>(pItem->ItemNum, pItem));

		// initialize to 0x00
		memset(&item, 0x00, sizeof(ITEM));
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// cleanup odbc resources
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	SQLDisconnect(hdbc);
#ifndef _DEBUG
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
#endif
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

ITEM* CNPCIOCP::GetItemInfo(int ItemNum)
{

	ez_map<int, ITEM*>::iterator itr = m_mapItemInfo.find(ItemNum);
	if ( itr == m_mapItemInfo.end() )
	{
		return NULL;
	}
	else
	{
		return itr->second;
	}

	// not reachable
	return NULL;
}


MONSTER_INFO *CNPCIOCP::GetMonsterInfo(int i_nMonsterUnitKind)
{
	map<int, MONSTER_INFO>::iterator it = m_mapMonsterParameter.find(i_nMonsterUnitKind);
	if(it == m_mapMonsterParameter.end())
	{
		return NULL;
	}
	return &it->second;
}

CNPCMapChannel * CNPCIOCP::GetNPCMapChannelByMapChannelIndex(MAP_CHANNEL_INDEX MapChannIndex)
{
	return m_pNPCMapWorkspace->GetNPCMapChannelByMapChannelIndex(MapChannIndex);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCIOCP::SetEventMonsterList(mtvectSEVENT_MONSTER *i_pEvMonList)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CNPCIOCP::SetEventMonsterList() 추가
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCIOCP::SetEventMonsterList(mtvectSEVENT_MONSTER *i_pEvMonList)
{
	mt_auto_lock mtAEvMon(&m_EventMonsterList);
	m_EventMonsterList.clear();

	mtvectSEVENT_MONSTER::iterator itr(i_pEvMonList->begin());
	for(; itr != i_pEvMonList->end(); itr++)
	{
		SEVENT_MONSTER *pEvMon = &*itr;
		if(0 == pEvMon->SummonMonsterNum
			|| NULL == this->GetMonsterInfo(pEvMon->SummonMonsterNum))
		{
			g_pNPCGlobal->WriteSystemLogEX(TRUE, "[Error] CNPCIOCP::SetEventMonsterList_ EventMonster error!!, EventMonsterUID(%d) SummonMonsterNum(%d)\r\n"
				, pEvMon->EventMonsterUID, pEvMon->SummonMonsterNum);
			continue;
		}
		pEvMon->SummonDelayTime		= max(1, pEvMon->SummonDelayTime);
		pEvMon->SummonMonsterCount	= max(1, min(100, pEvMon->SummonMonsterCount));

		m_EventMonsterList.push_back(*pEvMon);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCIOCP::AddCheckSummonEventMonster(SEVENT_MONSTER *i_pEventMonster, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CNPCIOCP::AddCheckSummonEventMonster() 추가
/// \author		cmkwon
/// \date		2008-04-16 ~ 2008-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCIOCP::AddCheckSummonEventMonster(SEVENT_MONSTER *i_pEventMonster, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo)
{
	if(NULL == i_pMonInfo)
	{
		return FALSE;
	}

	if(0 == i_pEventMonster->SummonMonsterNum
		|| i_pMonInfo->MonsterUnitKind == i_pEventMonster->SummonMonsterNum)
	{// 2008-04-16 by cmkwon, Summoner 와 소환 될 MonsterNum 이 같으면 소환 불가 처리
		return FALSE;
	}

	ATUM_DATE_TIME atCur { true };
	if(atCur < i_pEventMonster->StartDateTime
		|| atCur > i_pEventMonster->EndDateTime)
	{// 2008-04-16 by cmkwon, 기간 체크
		return FALSE;
	}

	if(0 != i_pEventMonster->SummonerMapIndex
		&& i_mapChannIdx.MapIndex != i_pEventMonster->SummonerMapIndex)
	{// 2008-04-16 by cmkwon, MapIndex 체크
		return FALSE;
	}

	if(0 != i_pEventMonster->SummonerReqMinLevel
		&& i_pMonInfo->Level < i_pEventMonster->SummonerReqMinLevel)
	{// 2008-04-16 by cmkwon, 최저레벨 체크
		return FALSE;
	}

	if(0 != i_pEventMonster->SummonerReqMaxLevel
		&& i_pMonInfo->Level > i_pEventMonster->SummonerReqMaxLevel)
	{// 2008-04-16 by cmkwon, 최고레벨 체크
		return FALSE;
	}

	if(COMPARE_BIT_FLAG(i_pEventMonster->SummonerExceptMonster, EXCEPTMON_OBJECT_MONSTER_MASK)
		&& IS_OBJECT_MON_FORM(i_pMonInfo->MonsterForm))
	{// 2008-04-16 by cmkwon, 오브젝트 몬스터 제외
		return FALSE;
	}
	if(COMPARE_BIT_FLAG(i_pEventMonster->SummonerExceptMonster, EXCEPTMON_INFLUENCE_TYPE_MASK)
		&& IS_INFLWAR_MONSTER(i_pMonInfo->Belligerence))
	{// 2008-04-16 by cmkwon, 세력몬스터 제외
		return FALSE;
	}
	if(COMPARE_BIT_FLAG(i_pEventMonster->SummonerExceptMonster, EXCEPTMON_BELL_NOTATTACK_MASK)
		&& BELL_NOTATTACK == i_pMonInfo->Belligerence)
	{// 2008-04-16 by cmkwon, 절대로 공격하지 않는 몬스터 제외
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCIOCP::GetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *o_pSummonEvMonList, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo)
/// \brief		// 2008-04-16 by cmkwon, 몬스터 사망 시 몬스터 소환 이벤트 시스템 구현 - CNPCIOCP::GetSummonEventMonsterListAfterDead() 추가
/// \author		cmkwon
/// \date		2008-04-16 ~ 2008-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCIOCP::GetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *o_pSummonEvMonList, MAP_CHANNEL_INDEX i_mapChannIdx, MONSTER_INFO *i_pMonInfo)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-18 by cmkwon, 소환 이벤트 몬스터 관련 문제 수정 - 통합아레나서버, 튜토리얼맵 몬스터 소환 이벤트 제외
	if(g_pNPCGlobal->GetIsArenaServer()
		|| IS_TUTORIAL_MAP_INDEX(i_mapChannIdx.MapIndex))
	{
		return FALSE;
	}
	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-18 by cmkwon, 소환 이벤트 몬스터 관련 문제 수정 - 이벤트맵, 전진기지전 관련 맵은 몬스터 소환 이벤트 제외
	CNPCMapProject *pNMapPro = this->m_pNPCMapWorkspace->GetNPCMapProject(i_mapChannIdx.MapIndex);
	if(NULL == pNMapPro
		|| IS_MAP_INFLUENCE_OUTPOST(pNMapPro->m_nMapInfluenceType)
		|| IS_MAP_INFLUENCE_EVENT_AREA(pNMapPro->m_nMapInfluenceType))
	{
		return FALSE;
	}

	if(m_EventMonsterList.empty())
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_EventMonsterList);
	if(m_EventMonsterList.empty())
	{
		return FALSE;
	}
	
	mtvectSEVENT_MONSTER::iterator itr(m_EventMonsterList.begin());
	for(; itr != m_EventMonsterList.end(); itr++)
	{
		SEVENT_MONSTER *pEvMon = &*itr;
		if(FALSE == this->AddCheckSummonEventMonster(pEvMon, i_mapChannIdx, i_pMonInfo))
		{
			continue;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2008-04-16 by cmkwon, 소환 체크 할 몬스터 리스트에 추가 한다.
		SSUMMON_EVENT_MONSTER summonEvMon;
		util::zero(&summonEvMon, sizeof(SSUMMON_EVENT_MONSTER));
		summonEvMon.IsSummonChecked		= FALSE;		// 명시
		summonEvMon						= *pEvMon;
		o_pSummonEvMonList->push_back(summonEvMon);
	}

	if(o_pSummonEvMonList->empty())
	{// 소환 몬스터가 없다
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CLIENT_INFO* CNPCIOCP::GetClientInfoO(int i_Characteridx)
/// \brief		// 2008-12-02 by cmkwon, NPCServer 클라이언트 관리 구조 수정 - 
/// \author		cmkwon
/// \date		2008-12-02 ~ 2008-12-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CLIENT_INFO* CNPCIOCP::GetClientInfoO(int i_Characteridx)
{
	if(i_Characteridx < 0 || i_Characteridx >= SIZE_MAX_FIELDSERVER_SESSION)
	{
		return NULL;
	}

	return &m_vectorClientInfo[i_Characteridx];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCIOCP::IsExistClient(MAP_CHANNEL_INDEX i_mapChannIdx)
/// \brief		// 2008-12-03 by cmkwon, NPCServer 클라이언트 없는 맵채널 처리 수정 - 
/// \author		cmkwon
/// \date		2008-12-03 ~ 2008-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCIOCP::IsExistClient(MAP_CHANNEL_INDEX i_mapChannIdx)
{
	vectorCLIENT_INFO::iterator itr(m_vectorClientInfo.begin());

	for(; itr != m_vectorClientInfo.end(); itr++)
	{
		if(CS_NULL != itr->ClientState
			&& itr->MapChannelIdx == i_mapChannIdx)
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CNPCIOCP::SetSummonJacoMonster(BYTE i_byBelligerence, BOOL i_bIsSummon)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-18 ~ 2006-04-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CNPCIOCP::SetSummonJacoMonster(BYTE i_byBelligerence, BOOL i_bIsSummon)
{
	if(BELL_INFLUENCE_VCN == i_byBelligerence)
	{
		m_bSummonVCNJacoMonster	= i_bIsSummon;
	}
	else if(BELL_INFLUENCE_ANI == i_byBelligerence)
	{
		m_bSummonANIJacoMonster	= i_bIsSummon;
	}
	char szSysLog[1024];
	wsprintf(szSysLog, "CNPCIOCP::SetSummonJacoMonster_ InfluenceType(%d) IsSummonSpyMonster(%d)\r\n"
		, i_byBelligerence, i_bIsSummon);
	g_pNPCGlobal->WriteSystemLog(szSysLog);
	DBGOUT(szSysLog);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CNPCIOCP::CheckSummonJacoMonster(BYTE i_byBelligerence)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-18 ~ 2006-04-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CNPCIOCP::CheckSummonJacoMonster(BYTE i_byBelligerence)
{
	if(BELL_INFLUENCE_VCN == i_byBelligerence)
	{
		return m_bSummonVCNJacoMonster;
	}
	else if(BELL_INFLUENCE_ANI == i_byBelligerence)
	{
		return m_bSummonANIJacoMonster;
	}
	return FALSE;
}