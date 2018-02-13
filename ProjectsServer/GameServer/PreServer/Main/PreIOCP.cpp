// PreIOCP.cpp: implementation of the CPreIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "HttpManager.h"
#include "PreIOCP.h"
#include "PreIOCPSocket.h"
#include "PreTickManager.h"
#include "Wininet.h"
#include "PreGlobal.h"
#include "AtumError.h"
#include "AccountBlockManager.h"
#include <timeapi.h>

// If i_nBirthday is less than 10000, this is the year of birth
// Otherwise, it is the date of birth in the decimal format: yyymmdd
// - yyyy is YEAR - 1900
// - mm is MONTH of YEAR
// - dd is DAY of MONTH
void CAccountInfo::SetBirthday(int i_nBirthday)
{
	if (i_nBirthday <= 0) return;

	int nMonth = 0;
	int nDay = 0;
	int nYear = i_nBirthday;

	if (i_nBirthday >= 10000)
	{
		nYear = max(1900, i_nBirthday / 10000);
		nMonth = max(1, (i_nBirthday / 100) % 100);
		nDay = max(1, i_nBirthday % 100);
	}

	atBirthday.SetDateTime(nYear, nMonth, nDay, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreIOCP::CPreIOCP(int nPort, char *szLocalIP) :
	CIOCP { CLIENT_INDEX_START_NUM, SIZE_MAX_PRESERVER_SESSION, nPort, szLocalIP, ST_PRE_SERVER, sizeof(CPreIOCPSocket) },
	m_pAtumDBManager { nullptr },
	m_pPreTickManager { nullptr },
	m_bPauseService { false },
	m_pAccountBlockManager { nullptr }
{
	auto arr = new CPreIOCPSocket[m_dwArrayClientSize];
	
	for (auto i = 0; i < m_dwArrayClientSize; i++) arr[i].InitIOCPSocket(i);

	m_pArrayIOCPSocket = arr;

	m_AllServerGroupVector.clear();
	m_OpeningMovieStandard.Reset();		// 2011-12-21 by hskim, EP4 [동영상 1회 재생]

	CPreIOCPSocket::ms_pPreIOCP = this;
}

CPreIOCP::~CPreIOCP()
{
	ListenerClose();

	CPreIOCP::IOCPClean();

	if (m_pArrayIOCPSocket)
	{
		delete[] reinterpret_cast<CPreIOCPSocket*>(m_pArrayIOCPSocket);
		m_pArrayIOCPSocket = nullptr;
	}

	m_AccountInfoMap.lock();
	for (auto& info : m_AccountInfoMap) util::del(info.second);
	m_AccountInfoMap.clear();
	m_AccountInfoMap.unlock();

	m_mtmapVersionOld2New.clearLock();

	m_AllServerGroupVector.clear();

	util::del(m_pAccountBlockManager);
	util::del(m_pPreTickManager);
	util::del(m_pAtumDBManager);
}

BOOL CPreIOCP::IOCPInit()
{

	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 10000 \r\n");

	if (g_pPreGlobal->GetOnlyChoiceServer())
	{
		server::log(TRUE, "====> [OnlyChoiceServer]\r\n");
		return CIOCP::IOCPInit();
	}

	DBGOUT("\r\nGetting Update Versions from Local File...\r\n");
	// get version history
	if (!CreateVersionInfoFromLocalFile()) return false;
	WriteVersionInfoToLocalFile();

	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 11000 updated version files !\r\n");

	if (!m_mtmapVersionOld2New.empty()) m_LatestClientVersion = m_mtmapVersionOld2New.crbegin()->second;

#ifdef _DEBUG
	if (g_pPreGlobal->m_bIgnoreClientVersionUpdate)
	{
		DBGOUT("!!! IGNORING CLIENT VERSION UPDATE !!!\r\n\r\n");
	}
#endif

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 12000 arranged version list !\r\n");


	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon
	CODBCStatement odbcStmt;

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//	BOOL bRet = odbcStmt.Init((LPCSTR)g_pGlobal->GetODBCDSN(), (LPCSTR)g_pGlobal->GetODBCUID(), (LPCSTR)g_pGlobal->GetODBCPASSWORD());
	BOOL bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CPreIOCP::IOCPInit_) !! %s,%d %s : Wrong DB Password \r\n"		// 2012-09-24 by jhseol, 소스정리 - 주석 정리 ID / PW 미출력
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName());	// 2012-09-24 by jhseol, 소스정리 - 주석 정리 ID / PW 미출력

		MessageBox(NULL, "DB connect CPreIOCP::IOCPInit()", "ERROR - SpaceCowboy PreServer", NULL);
		MessageBox(NULL, (char*)g_pGlobal->GetODBCPASSWORD(), "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 13000 connected to db server !\r\n");

	// 2007-01-29 by cmkwon, DB Table 초기화 관련 처리
	bRet = this->InitDBTable(&odbcStmt);
	if (!bRet)
	{
		MessageBox(NULL, "InitDBTable()", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 14000 called CPreIOCP::InitDBTable# \r\n");


	// Init DBManager
	m_pAtumDBManager = new CAtumPreDBManager(this);
	if (m_pAtumDBManager->Init() == FALSE)
	{
		MessageBox(NULL, "m_pAtumDBManager initialize fail", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 15000 CAtumPreDBManager::Init# !\r\n");

	// Init TickManager
	m_pPreTickManager = new CPreTickManager(this);
	if (m_pPreTickManager->InitThread() == FALSE)
	{
		MessageBox(NULL, "m_pPreTickManager initialize fail", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 16000 CPreTickManager::InitThread# !\r\n");

	m_pAccountBlockManager = new CAccountBlockManager(m_pAtumDBManager);

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 17000 CAccountBlockManager::CAccountBlockManager# !\r\n");

	// 2016-01-09 Future, improved Account block loading
	m_pAccountBlockManager->ResetAccountBlockManager();
	m_pAccountBlockManager->LoadBlockedAccountListFromDB();
	m_pAccountBlockManager->LoadBlockedMACListFromDB();

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 18000 CPreIOCP::LoadBlockedAccounts# !\r\n");

	// server group 정보 로딩

	m_AllServerGroupVector = move(g_pPreGlobal->m_AllServerGroupVectorForLoading);
	// DevX thinks next line might be unnecessary
	g_pPreGlobal->m_AllServerGroupVectorForLoading.clear();

	//vector<CServerGroup*>::iterator itr = g_pPreGlobal->m_AllServerGroupVectorForLoading.begin();
	//while (g_pPreGlobal->m_AllServerGroupVectorForLoading.end() != itr)
	//{
	//	m_AllServerGroupVector.push_back(*itr);
	//	itr++;
	//}
	//// 다 로딩한 후에는 바로 삭제함
	//g_pPreGlobal->m_AllServerGroupVectorForLoading.clear();

	// 2009-10-13 by cmkwon, PreServer 시스템 로그 추가 - 
	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 19000 arranged ServerGroupList !\r\n");


	// start 2011-12-21 by hskim, EP4 [동영상 1회 재생]
#ifdef S_EP4_HSKIM		// ON/OFF 기능 구현
	bRet = this->LoadSystemEvent(&odbcStmt);
	if (!bRet)
	{
		MessageBox(NULL, "LoadSystemEvent()", "ERROR - SpaceCowboy PreServer", NULL);
		return FALSE;
	}
#endif
	// end 2011-12-21 by hskim, EP4 [동영상 1회 재생]

	odbcStmt.Clean();

	server::log(TRUE, "  [Notify] CPreIOCP::IOCPInit# 20000 end !\r\n");

	return CIOCP::IOCPInit();
}

void CPreIOCP::IOCPClean()
{
	CIOCP::IOCPClean();

	if (m_pAtumDBManager)
	{
		m_pAtumDBManager->Clean();
		util::del(m_pAtumDBManager);
	}

	if (m_pPreTickManager != NULL)
	{
		m_pPreTickManager->CleanThread();
		util::del(m_pPreTickManager);
	}
}

SThreadInfo *CPreIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);

	// 2007-07-06 by cmkwon, OnlyChoiceServer 플래그 구현 추가 - NULL 체크 추가
	if (NULL == pTInfo
		&& m_pAtumDBManager)
	{
		pTInfo = m_pAtumDBManager->CheckDBThread(i_dwThreadIdToExclude);
	}
	if (pTInfo
		&& !m_MonitorIOCPSocketPtrVector.empty())
	{
		BYTE SendBuf[256];
		INIT_MSG(MSG_PM_SERVER_THREAD_STATUS, T_PM_SERVER_THREAD_STATUS, pSend, SendBuf);
		pSend->dwThreadId = pTInfo->dwThreadId;
		util::strncpy(pSend->szThreadComment, pTInfo->GetThreadCheckComment(pTInfo->enThreadCheckType), SIZE_MAX_THREAD_COMMENT);
		pSend->dwTimeGap = timeGetTime() - pTInfo->dwLastUseStartTick;
		SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_PM_SERVER_THREAD_STATUS));

		INIT_MSG(MSG_PA_ADMIN_CHANGE_SERVER_STATE, T_PA_ADMIN_CHANGE_SERVER_STATE, msgChangeSS, SendBuf);
		msgChangeSS->ServerID = SERVER_ID(m_szLocalIPAddress, m_nListenerPort);
		msgChangeSS->ServerType = ST_PRE_SERVER;
		msgChangeSS->ServerState = SERVER_STATE_ABNORMAL;
		SendMessageToAdminTool(SendBuf, MSG_SIZE(MSG_PA_ADMIN_CHANGE_SERVER_STATE));		// 2006-05-18 by cmkwon, 버그 수정
	}
	return pTInfo;
}

#define COMMAND_LINE_BUFF 512

BOOL CPreIOCP::CreateVersionInfoFromLocalFile()
{
	// reset version list
	// 2004-10-19 by cmkwon
	//m_mapVersionOld2New.clear();
	m_mtmapVersionOld2New.lock();
	m_mtmapVersionOld2New.clear();

	string versionFileName = CONFIG_ROOT + string("versions.cfg");

	ifstream	fin;
	char		buff[COMMAND_LINE_BUFF];
	const char	*token;
	char		seps[] = " \t";

	fin.open(versionFileName.c_str());

	if (!fin.is_open())
	{
		return FALSE;
	}

	while (TRUE)
	{
		memset(buff, 0x00, COMMAND_LINE_BUFF);
		BOOL bFlagFileEnd = fin.getline(buff, COMMAND_LINE_BUFF).eof();
		if (bFlagFileEnd == TRUE)
		{
			break;
		}

		token = strtok(buff, seps);

		if (token == NULL						// ignore blank lines
			|| strncmp(token, "#", 1) == 0		// ignore comments
			|| strncmp(token, "$", 1) == 0)		// ignore user defined variables
		{
			continue;
		}

		if (stricmp(token, "ClientVersion") == 0)
		{
			VersionInfo tmpOldVersion;
			VersionInfo tmpNewVersion;

			char *token1 = strtok(NULL, seps);
			if (token1 == NULL)
			{
				char *msg = "Loading Version File Failed. ClientVersion\n";
				DBGOUT(msg);
				return FALSE;
			}

			char *token2 = strtok(NULL, seps);
			if (token2 == NULL)
			{
				char *msg = "Loading Version File Failed. ClientVersion\n";
				DBGOUT(msg);
				return FALSE;
			}

			tmpOldVersion.SetVersion(token1);
			tmpNewVersion.SetVersion(token2);

			// 2004-10-19 by cmkwon
			//m_mapVersionOld2New.insertEZ(tmpOldVersion, tmpNewVersion);
			m_mtmapVersionOld2New.insertLock(tmpOldVersion, tmpNewVersion);
		}
		else if (stricmp(token, "LauncherVersion") == 0)
		{
			token = strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Version File Failed. LauncherVersion\n";
				DBGOUT(msg);
				return FALSE;
			}
			m_LatestLauncherVersion.SetVersion(token);
		}
		else if (stricmp(token, "DeleteFileListVersion") == 0)
		{
			token = strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Version File Failed. DeleteFileListVersion\n";
				DBGOUT(msg);
				return FALSE;
			}
			m_LatestDeleteFileListVersion.SetVersion(token);
		}
		else if (stricmp(token, "NoticeVersion") == 0)
		{
			token = strtok(NULL, seps);
			if (token == NULL)
			{
				char *msg = "Loading Version File Failed. NoticeVersion\n";
				DBGOUT(msg);
				return FALSE;
			}
			m_LatestNoticeVersion.SetVersion(token);
		}
	}

	fin.close();

#ifdef _DEBUG
	mtmapVersionInfo::iterator itr(m_mtmapVersionOld2New.begin());
	for (; itr != m_mtmapVersionOld2New.end(); itr++)
	{
		VersionInfo tmpOldVersion = itr->first;
		VersionInfo tmpNewVersion = itr->second;
		DBGOUT("  Client Version: %10s -> %10s\n", tmpOldVersion.GetVersionString(), tmpNewVersion.GetVersionString());
	}
	DBGOUT("  Latest Client Version: %s\n", m_mtmapVersionOld2New.rbegin()->second.GetVersionString());
	DBGOUT("  Client Reinstall Version: %s\n", g_pPreGlobal->GetClientReinstallVersion().GetVersionString());
	DBGOUT("\n  Launcher Version: %s\n", m_LatestLauncherVersion.GetVersionString());
	DBGOUT("  Delete File List Version: %s\n", m_LatestDeleteFileListVersion.GetVersionString());
	DBGOUT("  Notice Version: %s\n\n", m_LatestNoticeVersion.GetVersionString());
	g_pPreGlobal->PrintAllowedList();
#endif

	m_mtmapVersionOld2New.unlock();

	return TRUE;
}

BOOL CPreIOCP::WriteVersionInfoToLocalFile()
{
	string versionFileName = CONFIG_ROOT + string("versions.cfg");

	// 지우고 새로 저장
	DeleteFile(versionFileName.c_str());

	// 파일 생성
	HANDLE hFile;

	hFile = CreateFile(versionFileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		// int nErr = GetLastError();

		SetLastError(0);

		DBGOUT("ERROR: Cannot VersionFile\r\n");

		return FALSE;
	}

	// 파일 쓰기
	DWORD nWritten;
	char lineBuf[512];

	// ClientVersion list
	m_mtmapVersionOld2New.lock();
	mtmapVersionInfo::iterator itr = m_mtmapVersionOld2New.begin();
	for (; m_mtmapVersionOld2New.end() != itr; itr++)
	{
		sprintf(lineBuf, "ClientVersion\t\t\t%s\t%s\r\n", ((VersionInfo&)itr->first).GetVersionString(), itr->second.GetVersionString());
		WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);
	}
	m_mtmapVersionOld2New.unlock();

	// blank lines
	sprintf(lineBuf, "\r\n");
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// LauncherVersion
	sprintf(lineBuf, "LauncherVersion\t\t\t%s\r\n", m_LatestLauncherVersion.GetVersionString());
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// DeleteFileListVersion
	sprintf(lineBuf, "DeleteFileListVersion\t%s\r\n", m_LatestDeleteFileListVersion.GetVersionString());
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// NoticeVersion
	sprintf(lineBuf, "NoticeVersion\t\t\t%s\r\n", m_LatestNoticeVersion.GetVersionString());
	WriteFile(hFile, lineBuf, strlen(lineBuf), &nWritten, NULL);

	// 파일 닫기
	CloseHandle(hFile);

	return TRUE;
}

// 해당 버전이 있는지 확인하고, 업데이트할 버전을 리턴함
BOOL CPreIOCP::GetNextUpdateVersion(VersionInfo *o_pNextVersion, VersionInfo *i_pVersion)
{
	m_mtmapVersionOld2New.lock();
	mtmapVersionInfo::iterator itr = m_mtmapVersionOld2New.find(*i_pVersion);
	if (itr == m_mtmapVersionOld2New.end())
	{
		m_mtmapVersionOld2New.unlock();
		return FALSE;
	}

	*o_pNextVersion = itr->second;
	m_mtmapVersionOld2New.unlock();

	return TRUE;
}

BOOL CPreIOCP::GetLatestUpdateVersion(VersionInfo *o_pLatestVersion)
{
	m_mtmapVersionOld2New.lock();
	if (m_mtmapVersionOld2New.empty())
	{
		m_mtmapVersionOld2New.unlock();
		return FALSE;
	}

	*o_pLatestVersion = m_mtmapVersionOld2New.rbegin()->second;
	m_mtmapVersionOld2New.unlock();

	return TRUE;
}

BOOL CPreIOCP::IsLogined(char* accountName)
{
	CAccountInfo * pAccInfo = GetAccountInfoPtr(accountName);

	if (pAccInfo == NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CPreIOCP::GetAccountInfo(CAccountInfo *o_pAccountInfo, const char* accountName)
{
	// GetAccountInfoPtr does lock/unlock internally,
	// but we probably need to keep a lock until the data has been copied

	m_AccountInfoMap.lock();

	auto ptmAccInfo = GetAccountInfoPtr(accountName);

	if (ptmAccInfo) *o_pAccountInfo = *ptmAccInfo;

	m_AccountInfoMap.unlock();

	return ptmAccInfo != nullptr;
}

CAccountInfo *CPreIOCP::GetAccountInfoPtr(const char *i_szAccountName)
{
	char name[SIZE_MAX_ACCOUNT_NAME];

	util::strncpy(name, i_szAccountName, SIZE_MAX_ACCOUNT_NAME);

	_strlwr(name);

	return m_AccountInfoMap.findLock(name);
}

BOOL CPreIOCP::InsertAccountInfo(char* accountName, CAccountInfo* accountInfo)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-18 by cmkwon, 소문자로 변환
	char name[SIZE_MAX_ACCOUNT_NAME];
	util::strncpy(name, accountName, SIZE_MAX_ACCOUNT_NAME);
	_strlwr(name);

	auto bRet = m_AccountInfoMap.insertLock(name, accountInfo);

	if (bRet)
	{
		auto pServerGroup = GetServerGroup(accountInfo->CurrentServerGroup);

		if (pServerGroup) pServerGroup->m_setLoginedAccount.insertLock(name);
	}

	return bRet;
}

BOOL CPreIOCP::DeleteAccountInfo(const char* accountName) // todo : more methods may be accept const args
{
	///////////////////////////////////////////////////////////////////////////////
	// 2006-10-18 by cmkwon, 소문자로 변환
	char szLowercaseAccName[SIZE_MAX_ACCOUNT_NAME];
	util::zero(szLowercaseAccName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(szLowercaseAccName, accountName, SIZE_MAX_ACCOUNT_NAME);
	_strlwr(szLowercaseAccName);

	m_AccountInfoMap.lock();
	CAccountInfo *pAccountInfo = GetAccountInfoPtr(szLowercaseAccName);
	if (pAccountInfo == NULL)
	{
		m_AccountInfoMap.unlock();
		return FALSE;
	}

	char szTmpServerGroupName[SIZE_MAX_SERVER_NAME];
	util::strncpy(szTmpServerGroupName, pAccountInfo->CurrentServerGroup, SIZE_MAX_SERVER_NAME);

	// 2012-07-11 by hskim, 선택적 셧다운
	//if( NULL != g_pPreGlobal->m_pSelectiveShutdown )
	//{
	//	if( TRUE == pAccountInfo->bSelectiveShutdownTarget )
	//	{
	//		CShutdownTarget TargeInfo(accountName, pAccountInfo->SelectiveShutdownInfo);
	//		g_pPreGlobal->m_pSelectiveShutdown->DelTarget(TargeInfo);
	//	}
	//}
	// end 2012-07-11 by hskim, 선택적 셧다운

	int nDeleted = m_AccountInfoMap.erase(szLowercaseAccName);
	util::del(pAccountInfo);

	if (nDeleted != 0)
	{
		CServerGroup *pServerGroup = GetServerGroup(szTmpServerGroupName);
		if (pServerGroup != NULL)
		{
			pServerGroup->m_setLoginedAccount.deleteLock(szLowercaseAccName);
		}
	}
	m_AccountInfoMap.unlock();

	return (nDeleted != 0);
}

void CPreIOCP::DeleteAccountInfoByServerGroupName(const char *i_szServerGroup)
{
	m_AccountInfoMap.lock();
	{
		mtmapAccountInfoPtr::iterator itrAccInfo = m_AccountInfoMap.begin();
		while (itrAccInfo != m_AccountInfoMap.end())
		{
			CAccountInfo *pAccInfo = itrAccInfo->second;
			if (0 == strnicmp(pAccInfo->CurrentServerGroup, i_szServerGroup, SIZE_MAX_SERVER_NAME))
			{
				m_AccountInfoMap.erase(itrAccInfo++);
				util::del(pAccInfo);
				continue;
			}

			itrAccInfo++;
		}
	}
	m_AccountInfoMap.unlock();
}

void CPreIOCP::DBG_PrintAccountMap()
{
	if (m_AccountInfoMap.size() >= 10)
	{
		return;
	}

	DBGOUT("\nAccount Info...\n");
	m_AccountInfoMap.lock();
	mtmapAccountInfoPtr::iterator itr = m_AccountInfoMap.begin();

	for (; itr != m_AccountInfoMap.end(); itr++)
	{
		DBGOUT("  ID(%s), State(%d), PrivateIP(%s)\n", (itr->first).c_str(), (itr->second)->ClientState, (itr->second)->PrivateClientIP);
	}
	m_AccountInfoMap.unlock();
	DBGOUT("\n");
}

// start 2011-12-21 by hskim, EP4 [동영상 1회 재생]
BOOL CPreIOCP::LoadSystemEvent(CODBCStatement *i_pODBCStmt)
{
	if (FALSE == CAtumDBHelper::DBLoadSystemEventInfo(i_pODBCStmt, &m_OpeningMovieStandard, FALSE))
	{
		return FALSE;
	}

	return TRUE;
}
// end 2011-12-21 by hskim, EP4 [동영상 1회 재생]

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPreIOCP::InitDBTable(CODBCStatement *i_pODBCStmt)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-29 ~ 2007-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CPreIOCP::InitDBTable(CODBCStatement *i_pODBCStmt)
{
	//	char szQuery[1024];

		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-29 by cmkwon, 접속 중인 ServerGroupID 필드를 모두 초기화 한다.
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//	sprintf(szQuery, "UPDATE td_Account SET ConnectingServerGroupID = 0");

	// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	//	sprintf(szQuery, QUERY_080702_0087);
	//	BOOL bRet = i_pODBCStmt->ExecuteQuery(szQuery);
	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0087));
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();		// error
		return FALSE;
	}
	i_pODBCStmt->FreeStatement();

	return TRUE;
}

BOOL CPreIOCP::BlockAccount(const SBLOCKED_ACCOUNT_INFO *i_pBlockedAccountInfo, CPreIOCPSocket *i_pSocketToNotify, BOOL i_bDisconnectUser/*=TRUE*/)
{
	if (i_bDisconnectUser)
	{
		CAccountInfo tmAccountInfo;
		BOOL bRet = GetAccountInfo(&tmAccountInfo, i_pBlockedAccountInfo->szBlockedAccountName);
		if (bRet)
		{// 연결된 상태

			DisconnectAccount(&tmAccountInfo, ERR_PROTOCOL_ACCOUNT_BLOCKED, i_pSocketToNotify);
		}
	}

	m_pAccountBlockManager->AddBlockedAccountList(i_pBlockedAccountInfo);

	SBLOCKED_ACCOUNT_INFO *pDBQuery = new SBLOCKED_ACCOUNT_INFO;
	*pDBQuery = *i_pBlockedAccountInfo;
	m_pAtumDBManager->MakeAndEnqueueQuery(QT_BlockUser, i_pSocketToNotify, pDBQuery);

	return TRUE;
}

// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - UnblockAccount() 함수 인자 수정
//BOOL CPreIOCP::UnblockAccount(const char *i_szAccountName, CPreIOCPSocket *i_pSocketToNotify)
BOOL CPreIOCP::UnblockAccount(const MSG_PA_ADMIN_UNBLOCK_ACCOUNT *i_pUnblockAccount, CPreIOCPSocket *i_pSocketToNotify)
{
	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 아래와 같이 수정
	// 	m_pAccountBlockManager->DeleteBlockedAccountList(i_szAccountName);
	// 
	// 	char *szAccountName = new char[SIZE_MAX_ACCOUNT_NAME];
	// 	util::strncpy(szAccountName, i_szAccountName, SIZE_MAX_ACCOUNT_NAME);
	// 	m_pAtumDBManager->MakeAndEnqueueQuery(QT_UnblockUser, i_pSocketToNotify, szAccountName);

		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 블럭 상태의 계정 체크 추가
	if (FALSE == m_pAccountBlockManager->DeleteBlockedAccountList(i_pUnblockAccount->szBlockedAccountName))
	{
		return FALSE;
	}

	auto pDBQuery = new SBLOCKED_ACCOUNT_INFO { *i_pUnblockAccount };

	pDBQuery->atimeEndTime.SetCurrentDateTime();

	m_pAtumDBManager->MakeAndEnqueueQuery(QT_UnblockUser, i_pSocketToNotify, pDBQuery);

	return TRUE;
}

BOOL CPreIOCP::DisconnectAccount(CAccountInfo *i_pAccountInfo, Err_t i_nError, CPreIOCPSocket *i_pSocketToNotify)
{
	if (i_pAccountInfo == NULL)
	{
		return FALSE;
	}

	CServerGroup *pServerGroup = GetServerGroup(i_pAccountInfo->CurrentServerGroup);
	if (pServerGroup == NULL)
	{
		DBGOUT("  Error: No Such Server Group!");
		return FALSE;
	}

	CPreIOCPSocket *pFieldServerSocket = (CPreIOCPSocket*)pServerGroup->m_FieldServerInfo.pSocket;
	if (NULL == pFieldServerSocket
		|| FALSE == pFieldServerSocket->IsUsing())
	{
		DBGOUT("  Error: Not Available Field Server Socket!");
		return FALSE;
	}
	pFieldServerSocket->SendErrorMessage(T_PA_ADMIN_DISCONNECT_USER, i_nError, i_pAccountInfo->CurrentClientIndex, 0, i_pAccountInfo->AccountName);

	// Account 정보 지우기
	DeleteAccountInfo(i_pAccountInfo->AccountName);

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CPreIOCP::On_MSG_FP_CONNECT_AUTH_USER(CAccountInfo *o_AccountInfo, MSG_FP_CONNECT_AUTH_USER *i_pMsg)
/// \brief		
/// \author		cmkwon
/// \date		2004-10-19 ~ 2004-10-19
/// \warning	
///
/// \param		
/// \return		nErrorNum
///////////////////////////////////////////////////////////////////////////////
int CPreIOCP::On_MSG_FP_CONNECT_AUTH_USER(CAccountInfo *o_AccountInfo, const MSG_FP_CONNECT_AUTH_USER *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	auto pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);

	DBGOUT("%s AUTH_USER: %s %s %d %s\n", GetTimeString(string()), i_pMsg->AccountName
		, i_pMsg->FieldServerID.IP, i_pMsg->AuthType
		, (pAccountInfo != NULL) ? STRMSG_S_P2PRENOTIFY_0001 : STRMSG_S_P2PRENOTIFY_0002);

	if (NULL == pAccountInfo)	// 로그인 되어있는지 확인
		
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;

	else if (pAccountInfo->ClientState != CP_LOGGEDIN)	// state가 LOGINED인지 확인
		
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;

	else if (pAccountInfo->CurrentFieldServerID != i_pMsg->FieldServerID)
		
		nRetErrorNum = ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED;

	else if (strncmp(i_pMsg->PrivateIP, pAccountInfo->PrivateClientIP, SIZE_MAX_IPADDRESS) != 0)
		
		nRetErrorNum = ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED;

	else
	{// 인증 성공

		// client index 저장
		pAccountInfo->CurrentClientIndex = i_pMsg->ClientIndex;
		*o_AccountInfo = *pAccountInfo;
	}
	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_NOTIFY_WARP(MSG_FP_EVENT_NOTIFY_WARP *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);

	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGGEDIN)	// state가 LOGINED인지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
	}
	else
	{
		//strncpy(pAccountInfo->CurrentFieldServerIP, pMsgRecvWarp->FieldServerIP, SIZE_MAX_IPADDRESS);
		pAccountInfo->CurrentFieldServerID = i_pMsg->FieldServerID;

		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_ENTER_SELECT_SCREEN(CP_State *o_pState, const MSG_FP_EVENT_ENTER_SELECT_SCREEN* i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGGEDIN && pAccountInfo->ClientState != CP_GAMESTARTED)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
		*o_pState = pAccountInfo->ClientState;
	}
	else
	{// 

		util::strncpy(pAccountInfo->CurrentCharacterName, "", SIZE_MAX_CHARACTER_NAME);
		pAccountInfo->CurrentCharacterUniqueNumber = 0;
		pAccountInfo->CurrentMapChannel.MapIndex = 0;
		pAccountInfo->CurrentMapChannel.ChannelIndex = 0;
		pAccountInfo->ClientState = CP_FIELD_LOGGEDIN;

		*o_pState = pAccountInfo->ClientState;
		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_GAME_STARTED(CP_State *o_pState, const MSG_FP_EVENT_GAME_STARTED *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_FIELD_LOGGEDIN)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
		*o_pState = pAccountInfo->ClientState;
	}
	else
	{// 

		util::strncpy(pAccountInfo->CurrentCharacterName, i_pMsg->CharacterName, SIZE_MAX_CHARACTER_NAME);
		pAccountInfo->CurrentCharacterUniqueNumber = i_pMsg->CharacterUniqueNumber;
		pAccountInfo->CurrentMapChannel = i_pMsg->MapChannelIndex;
		pAccountInfo->ClientState = CP_GAMESTARTED;

		*o_pState = pAccountInfo->ClientState;
		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_EVENT_MAP_CHANGED(CP_State *o_pState, const MSG_FP_EVENT_MAP_CHANGED *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_GAMESTARTED)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;

		*o_pState = pAccountInfo->ClientState;
	}
	else
	{// 

		// 맵 업데이트
		pAccountInfo->CurrentMapChannel = i_pMsg->MapChannelIndex;

		*o_pState = pAccountInfo->ClientState;
		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}

int CPreIOCP::On_MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE(const MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE *i_pMsg)
{
	this->m_AccountInfoMap.lock();
	int nRetErrorNum = 0;
	CAccountInfo *pAccountInfo = this->GetAccountInfoPtr(i_pMsg->AccountName);
	DBGOUT("FIELDSERVER_CHANGE: %s %s %s\n", i_pMsg->AccountName
		, i_pMsg->FieldServerID.GetString().GetBuffer()
		, (pAccountInfo != NULL) ? STRMSG_S_P2PRENOTIFY_0001 : STRMSG_S_P2PRENOTIFY_0002);


	if (pAccountInfo == NULL)	// 로그인 되어있는지 확인
	{
		nRetErrorNum = ERR_PROTOCOL_NOT_LOGINED;
	}
	else if (pAccountInfo->ClientState != CP_LOGGEDIN)
	{
		nRetErrorNum = ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE;
	}
	else
	{// 

		//strncpy(pAccountInfo->CurrentFieldServerIP, pMsgRecvNotifyFieldServer->FieldServerIP, SIZE_MAX_IPADDRESS);
		pAccountInfo->CurrentFieldServerID = i_pMsg->FieldServerID;

		nRetErrorNum = 0;
	}

	this->m_AccountInfoMap.unlock();

	return nRetErrorNum;
}



CServerGroup* CPreIOCP::GetServerGroup(const char * szServerGruopName)
{
	for (auto& group : m_AllServerGroupVector)
		
		if (strnicmp(group.m_ServerGroupName, szServerGruopName, SIZE_MAX_SERVER_NAME) == 0)
			
			return &group;

	return nullptr;
}

void CPreIOCP::ResetServerGroup(const char *i_szServerGroupName, BOOL i_bExcludeIMServer/*=FALSE*/)
{
	CServerGroup *pSGroup = GetServerGroup(i_szServerGroupName);
	if (pSGroup)
	{
		pSGroup->ResetServerGroup(i_bExcludeIMServer);
	}

	this->DeleteAccountInfoByServerGroupName(i_szServerGroupName);
}


void CPreIOCP::SendServerInfoForAdmin(CPreIOCPSocket *i_pSocket)
{
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 아래와 같이 수정 함
	// 	MEX_SERVER_INFO_FOR_ADMIN *pMexServerInfo = NULL;
	// 	int offset = 0;
	// 
	// 	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_SERVER_INFO_OK, T_PA_ADMIN_GET_SERVER_INFO_OK, msgServerInfoOK, msgServerInfoOKBuf);
	// 	msgServerInfoOK->NumOfServers = 0;
	// 
	// 	offset = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK);
	// 	vector<CServerGroup*>::iterator itr = m_AllServerGroupVector.begin();
	// 	for (; itr != m_AllServerGroupVector.end(); itr++)
	// 	{
	// 		CServerGroup *pServerGroup = *itr;
	// 		if(pServerGroup->m_bEnableServerGroup)
	// 		{
	// 			pMexServerInfo = (MEX_SERVER_INFO_FOR_ADMIN*)(msgServerInfoOKBuf+offset);
	// 			util::zero(pMexServerInfo, sizeof(MEX_SERVER_INFO_FOR_ADMIN));
	// 
	// 			util::strncpy(pMexServerInfo->ServerName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
	// 			pMexServerInfo->ServerType		= ST_IM_SERVER;
	// 			pMexServerInfo->ServerID		= pServerGroup->m_IMServerInfo.serverID;
	// 			pMexServerInfo->UserCount		= pServerGroup->GetUserCount();
	// 			pMexServerInfo->ServerState		= (pServerGroup->m_IMServerInfo.IsActive?SERVER_STATE_NORMAL:SERVER_STATE_NOT_ACTIVE);
	// 			offset += sizeof(MEX_SERVER_INFO_FOR_ADMIN);
	// 			msgServerInfoOK->NumOfServers++;
	// 
	// 			FieldServerInfo *pFieldServerInfo = &pServerGroup->m_FieldServerInfo;
	// 			pMexServerInfo = (MEX_SERVER_INFO_FOR_ADMIN*)(msgServerInfoOKBuf+offset);
	// 			util::zero(pMexServerInfo, sizeof(MEX_SERVER_INFO_FOR_ADMIN));
	// 
	// 			util::strncpy(pMexServerInfo->ServerName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
	// 			pMexServerInfo->ServerType		= ST_FIELD_SERVER;
	// 			pMexServerInfo->ServerID		= pFieldServerInfo->serverID;
	// 			pMexServerInfo->UserCount		= 0;	// 필드 서버는 알 수 없음
	// 			pMexServerInfo->ServerState		= (pFieldServerInfo->IsActive?SERVER_STATE_NORMAL:SERVER_STATE_NOT_ACTIVE);
	// 			offset += sizeof(MEX_SERVER_INFO_FOR_ADMIN);
	// 			msgServerInfoOK->NumOfServers++;
	// 		}		
	// 	}
	// 
	// 	i_pSocket->SendAddData(msgServerInfoOKBuf, offset);

		///////////////////////////////////////////////////////////////////////////////	
		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) -
	int offset = 0;

	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_GET_SERVER_INFO_OK, T_PA_ADMIN_GET_SERVER_INFO_OK, msgServerInfoOK, msgServerInfoOKBuf);
	msgServerInfoOK->NumOfServerGroups = 0;

	offset = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK);
	for (auto& group : m_AllServerGroupVector)
	{
		//CServerGroup *pServerGroup = *itr;
		if (FALSE == group.m_bEnableServerGroup)
		{
			continue;
		}
		if (offset + sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN) > SIZE_MAX_PACKET)
		{
			i_pSocket->SendAddData(msgServerInfoOKBuf, offset);
			msgServerInfoOK->NumOfServerGroups = 0;
			offset = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK);
		}

		auto pMexServGInfo = (MEX_SERVER_GROUP_INFO_FOR_ADMIN*)(msgServerInfoOKBuf + offset);
		util::zero(pMexServGInfo, sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN));

		util::strncpy(pMexServGInfo->ServerName, group.m_ServerGroupName, SIZE_MAX_SERVER_NAME);
		pMexServGInfo->IMServerID = group.m_IMServerInfo.serverID;
		pMexServGInfo->UserCount = group.GetUserCount();
		pMexServGInfo->IMServerState = (group.m_IMServerInfo.IsActive ? SERVER_STATE_NORMAL : SERVER_STATE_NOT_ACTIVE);

		pMexServGInfo->FieldServerID = group.m_FieldServerInfo.serverID;
		pMexServGInfo->FieldServerState = (group.m_FieldServerInfo.IsActive ? SERVER_STATE_NORMAL : SERVER_STATE_NOT_ACTIVE);
		pMexServGInfo->LimitUserCount = group.m_LimitGroupUserCounts;
		pMexServGInfo->LockCreateCharacterForNewAccount = group.m_bLockCreateCharacterForNewAccount;
		offset += sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN);
		msgServerInfoOK->NumOfServerGroups++;
	}

	if (0 < msgServerInfoOK->NumOfServerGroups)
	{
		i_pSocket->SendAddData(msgServerInfoOKBuf, offset);
	}
}


BOOL CPreIOCP::SendErrorToFieldServer(char *i_szServerGroupName
	, SERVER_ID i_FieldServerid
	, MessageType_t msgType
	, Err_t err
	, int errParam1/*=0*/
	, int errParam2/*=0*/
	, char* errMsg/*=NULL*/
	, BOOL bCloseConnection/*=FALSE*/)
{
	CServerGroup *pServerGroup = this->GetServerGroup(i_szServerGroupName);
	if (pServerGroup == NULL)
	{// 서버 그룹이 없다
		DBGOUT("  Error: No Such Server Group(%s)!", i_szServerGroupName);
		return FALSE;
	}

	CPreIOCPSocket *pPISocket = (CPreIOCPSocket*)pServerGroup->m_FieldServerInfo.pSocket;
	if (pPISocket == NULL
		|| FALSE == pPISocket->IsUsing())
	{
		DBGOUT("  Error: Not Available Field Server Socket!");
		return FALSE;
	}

	pPISocket->SendErrorMessage(msgType, err
		, errParam1, errParam2, errMsg, bCloseConnection);
	return TRUE;
}


void CPreIOCP::SendMessageToAllFieldServer(BYTE *i_pBuff, int i_nSize)
{
	for (auto& group : m_AllServerGroupVector) group.SendMessageToFieldServer(i_pBuff, i_nSize);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPreIOCP::SendMsgToAllIMServer(BYTE *i_pBuff, int i_nSize)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CPreIOCP::SendMsgToAllIMServer(BYTE *i_pBuff, int i_nSize)
{
	for (auto& group : m_AllServerGroupVector) group.SendMsgToIMServer(i_pBuff, i_nSize);
}


void CPreIOCP::OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime)
{
	if (0 == pDateTime->Minute % 10)
	{// 10분에 한번씩 처리

		this->CheckOnlyLauncherLoginedList(pDateTime);
	}

	if (0 == pDateTime->Minute % 5)
	{
		this->StoredLogTotalUser();
	}
}


void CPreIOCP::CheckOnlyLauncherLoginedList(ATUM_DATE_TIME *pDateTime)
{
	m_AccountInfoMap.lock();
	{
		mtmapAccountInfoPtr::iterator itr(m_AccountInfoMap.begin());
		while (itr != m_AccountInfoMap.end())
		{
			CAccountInfo *pAccInfo = itr->second;
			if (CP_LOGGEDIN == pAccInfo->ClientState
				&& 10 < pDateTime->GetTimeDiffTimeInMinutes(pAccInfo->LauncherLoginTime))
			{// LauncherLogin후 10분이 지났다

				CServerGroup *pServerGroup = GetServerGroup(pAccInfo->CurrentServerGroup);
				if (pServerGroup != NULL)
				{
					///////////////////////////////////////////////////////////////////////////////
					// 2006-10-18 by cmkwon, 소문자로 변환
					char szLowercaseAccName[SIZE_MAX_ACCOUNT_NAME];
					util::zero(szLowercaseAccName, SIZE_MAX_CHARACTER_NAME);
					util::strncpy(szLowercaseAccName, pAccInfo->AccountName, SIZE_MAX_ACCOUNT_NAME);
					_strlwr(szLowercaseAccName);

					pServerGroup->m_setLoginedAccount.deleteLock(szLowercaseAccName);
				}
				m_AccountInfoMap.erase(itr++);
				util::del(pAccInfo);
				continue;
			}
			itr++;
		}
	}
	m_AccountInfoMap.unlock();
}


void CPreIOCP::StoredLogTotalUser()
{
	MSG_DB_TOTAL_USER *pTotalUser = new MSG_DB_TOTAL_USER;
	util::strncpy(pTotalUser->szServerGroup, "AllServer", SIZE_MAX_SERVER_NAME);
	pTotalUser->nUserCounts = 0;

	for (auto& group : m_AllServerGroupVector)
	{
		if (group.m_bEnableServerGroup)
		{
			pTotalUser->nUserCounts += group.GetUserCount();

			MSG_DB_TOTAL_USER *pSGUser = new MSG_DB_TOTAL_USER;
			util::strncpy(pSGUser->szServerGroup, group.m_ServerGroupName, SIZE_MAX_SERVER_NAME);
			pSGUser->nUserCounts = group.GetUserCount();

			m_pAtumDBManager->MakeAndEnqueueQuery(QT_LogTotalUser, NULL, (void*)pSGUser);
		}
	}

	m_pAtumDBManager->MakeAndEnqueueQuery(QT_LogTotalUser, NULL, (void*)pTotalUser);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPreIOCP::ExtAuthODBCInit()
/// \brief		
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//BOOL CPreIOCP::ExtAuthODBCInit()
//{
//	if(FALSE == g_pPreGlobal->IsExternalAuthentication())
//	{
//		return FALSE;
//	}
//
//	mt_auto_lock mtA(m_odbcExtAuth.GetLockPtr());
//	if(m_odbcExtAuth.IsReadyToUse())
//	{
//		return TRUE;
//	}
//
//// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 
////	return m_odbcExtAuth.Init(g_pPreGlobal->GetExtAuthODBCDSN(), g_pPreGlobal->GetExtAuthODBCUID(), g_pPreGlobal->GetExtAuthODBCPASSWORD());
//	return m_odbcExtAuth.Init(g_pPreGlobal->GetExtAuthDBServerIP(), g_pPreGlobal->GetExtAuthDBServerPort(), g_pPreGlobal->GetExtAuthDBServerDatabaseName(), (char*)g_pPreGlobal->GetExtAuthODBCUID(), (char*)g_pPreGlobal->GetExtAuthODBCPASSWORD(), g_pPreGlobal->GetMainWndHandle());
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CPreIOCP::ExtAuthODBCReset()
/// \brief		
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CPreIOCP::ExtAuthODBCReset()
//{
//	m_odbcExtAuth.Clean();
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CPreIOCP::ExtAuthODBCAuthentication(char *i_szAccName, char *i_szPwd)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-27 ~ 2006-04-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//BOOL CPreIOCP::ExtAuthODBCAuthentication(char *i_szAccName, char *i_szPwd)
//{
//	if(FALSE == g_pPreGlobal->IsExternalAuthentication())
//	{
//		return FALSE;
//	}
//
//	mt_auto_lock mtA(m_odbcExtAuth.GetLockPtr());
//	if(FALSE == m_odbcExtAuth.IsReadyToUse())
//	{
//		if(FALSE == this->ExtAuthODBCInit())
//		{
//			// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
//			g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CPreIOCP::ExtAuthODBCAuthentication_) !! %s,%d %s %s %s\r\n"
//				, g_pGlobal->GetExtAuthDBServerIP(), g_pGlobal->GetExtAuthDBServerPort(), g_pGlobal->GetExtAuthDBServerDatabaseName(), (char*)g_pGlobal->GetExtAuthODBCUID(), (char*)g_pGlobal->GetExtAuthODBCPASSWORD());
//			return FALSE;
//		}
//	}
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name: atum_ExternalAuthentication
//	-- Desc: 2006-04-27 by cmkwon, 추가 함
//	--		   외부 DB 인증 프로시저 - 이것은 없어도 되지만 내부에서 테스트 하기 위해 필요한 모듈임
//	--		   실제로 이 모듈은 상대 회사에서 구현해 주어야 한다.
//	--====
//	CREATE PROCEDURE atum_ExternalAuthentication
//		@i_AccountName				VARCHAR(20),	
//		@i_Password					VARCHAR(35)
//	AS
//		DECLARE @AccountUID INT
//		
//		SET @AccountUID = (SELECT AccountUniqueNumber 
//								FROM td_Account
//								WHERE AccountName = @i_AccountName AND Password = @i_Password)
//		
//		IF (@AccountUID IS NULL)
//			BEGIN
//					RETURN 0
//			END
//
//		RETURN 1
//	GO	
//	**************************************************************************/
//	SQLHSTMT	hstmt = m_odbcExtAuth.GetSTMTHandle();
//	BOOL		bAuth = FALSE;
//
//	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bAuth, 0,								NULL);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
//	BOOL ret = SQLExecDirect(hstmt, (UCHAR*)"{ ? = call atum_ExternalAuthentication(?, ?)}", SQL_NTS);
//
//	if (ret != SQL_SUCCESS
//		&& ret != SQL_SUCCESS_WITH_INFO)
//	{
//		if(ret != SQL_NO_DATA)
//		{// 쿼리 실패
//
//			m_odbcExtAuth.ProcessLogMessagesForStmt("@CPreIOCP::ExtAuthODBCAuthentication_ Failed!\r\n");
//			this->ExtAuthODBCReset();
//		}
//		m_odbcExtAuth.FreeStatement();	// close cursor
//		return FALSE;
//	}
//	m_odbcExtAuth.FreeStatement();	// close cursor
//
//	if(FALSE == bAuth)
//	{
//		return FALSE;
//	}
//	
//	return TRUE;
//}

// PreServer 시작 시 자동 버젼 업데이트 갱신
//BOOL CPreIOCP::PreGetVersionInfoFromFTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
//{
//	HINTERNET hInternet;
//	HINTERNET hFtpConnect;
//	HINTERNET hDir;
//	WIN32_FIND_DATA pDirInfo;
//	char pDirString[SIZE_MAX_FTP_FILE_PATH];
//
//	hInternet = InternetOpen("Atum Pre Server", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
//	if (hInternet == NULL)
//	{
//		// check: error
//		server::log(TRUE, "[ERROR] InternetOpen ERROR!!!");
//		return FALSE;
//	}
//
//	hFtpConnect = InternetConnect(hInternet, g_pPreGlobal->GetUploadFTPIP(), g_pPreGlobal->GetUploadFTPPort(),
//		g_pPreGlobal->GetUploadFTPAccount(), g_pPreGlobal->GetUploadFTPPassword(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
//	if (hFtpConnect == NULL)
//	{
//		// check: error
//		server::log(TRUE, "[ERROR] InternetConnect ERROR!!!");
//		return FALSE;
//	}
//
//	char currDir[128];
//	DWORD currDirLen = 128;
//	if (FtpGetCurrentDirectory(hFtpConnect, currDir, &currDirLen))
//	{
//	}
//
//	// change directory
//	if (FALSE == FtpSetCurrentDirectory(hFtpConnect, (LPCSTR)g_pPreGlobal->m_szClientFTPUpdateUploadDir))
//	{
//		// check: error
//		DWORD err = GetLastError();
//		DWORD len = 128;
//		char errString[128];
//		InternetGetLastResponseInfo(&err, errString, &len);
//		server::log(TRUE, "[ERROR] %s Change Directory ERROR!", errString);
//		return FALSE;
//	}
//
//	// Find files and update UpdateFileListVector
//	sprintf(pDirString, "%s", "*");
//	if (!(hDir = FtpFindFirstFile(hFtpConnect, pDirString, &pDirInfo, 0, 0)))
//	{
//		// check: error
//		server::log(TRUE, "[ERROR] FTPFile Not Find ");
//		return FALSE;
//	}
//	else
//	{
//		if (strcmp(pDirInfo.cFileName, ".") != 0 && strcmp(pDirInfo.cFileName, "..") != 0)
//		{
//			if (pDirInfo.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
//			{
//				// zip file 이름 해석하여 버전 넣기
//				const int SIZE_BUFF = 512;
//				char		buff[SIZE_BUFF];
//				char		seps[] = "_zip";
//				util::strncpy(buff, pDirInfo.cFileName, SIZE_BUFF);
//
//				// from version
//				char *token1 = strtok(buff, seps);
//				// to version
//				char *token2 = strtok(NULL, seps);
//
//				VersionInfo OldVersion(token1);
//				VersionInfo NewVersion(token2);
//
//				VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
//				if (pVersion == NULL)
//				{
//					o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
//				}
//				else
//				{
//					if (*pVersion < NewVersion)
//					{
//						*pVersion = NewVersion;
//					}
//				}
//			}
//			else
//			{
//				// check: error! directory가 존재하면 아니 됨!
//				server::log(TRUE, "[ERROR] directory Error ");
//				return FALSE;
//			}
//		}
//
//		while (TRUE)
//		{
//			if (!InternetFindNextFile(hDir, &pDirInfo))
//			{
//				int		nErr = GetLastError();
//				SetLastError(0);
//
//				if (ERROR_NO_MORE_FILES == nErr)
//				{
//					DBGOUT("No more files here\n");
//					break;
//				}
//				else
//				{
//					// check: error
//					server::log(TRUE, "[ERROR] FindNextFile ");
//					return FALSE;
//				}
//			}
//			else
//			{
//				if (strcmp(pDirInfo.cFileName, ".") == 0 || strcmp(pDirInfo.cFileName, "..") == 0)
//				{
//					continue;
//				}
//
//				if (pDirInfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
//				{
//					// check: error! directory가 존재하면 아니 됨!
//					server::log(TRUE, "[ERROR] FILE_ATTRIBUTE_DIRECTORY ");
//					return FALSE;
//				}
//
//				// zip file 이름 해석하여 버전 넣기
//				const int SIZE_BUFF = 512;
//				char		buff[SIZE_BUFF];
//				char		seps[] = "_zip";
//				util::strncpy(buff, pDirInfo.cFileName, SIZE_BUFF);
//
//				// from version
//				char *token1 = strtok(buff, seps);
//				// to version
//				char *token2 = strtok(NULL, seps);
//
//				VersionInfo OldVersion(token1);
//				VersionInfo NewVersion(token2);
//
//				if (FALSE == OldVersion.IsValidVersionInfo()
//					|| FALSE == NewVersion.IsValidVersionInfo()
//					|| OldVersion >= NewVersion)
//				{// 2006-12-28 by cmkwon, 유효한 업데이트 파일이 아니다
//
//					char szErr[1024];
//					sprintf(szErr, "Invalid update version file(%s/%s) !!", g_pPreGlobal->m_szClientFTPUpdateUploadDir, pDirInfo.cFileName);
//					server::log(TRUE, "[ERROR] %s ", szErr);
//					continue;
//				}
//
//				VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
//				if (pVersion == NULL)
//				{
//					o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
//				}
//				else
//				{
//					if (*pVersion < NewVersion)
//					{
//						*pVersion = NewVersion;
//					}
//				}
//			}
//		}
//	}
//
//	if (o_pMapVersionOld2New->empty())
//	{
//		server::log(TRUE, "[ERROR] No Version File in FTP Server");
//		return FALSE;
//	}
//
//	char *buffer = new char[DOWNLOAD_BUFFER_SIZE];
//	DWORD amount_read = 0;
//	int	total_read = 0;
//	BOOL ret = FALSE;
//
//	///////////////////////////////////////////////////////////////////////////
//	// get launcher version
//	sprintf(pDirString, "%s/lversion.ver", (LPCSTR)g_pPreGlobal->m_szLauncherFileUploadPath);
//	HINTERNET hLauncherVersionFile
//		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);
//
//	amount_read = 0;
//	total_read = 0;
//
//	ret = FALSE;
//	while (TRUE)
//	{
//		ret = InternetReadFile(hLauncherVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
//		if (!ret)
//		{
//			DWORD err;
//			char errMsg[1024];
//			DWORD errMsgLen = 1024;
//			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
//			server::log(TRUE, "[ERROR] %s ", errMsg);
//			return FALSE;
//		}
//
//		if (0 == amount_read)
//		{
//			break;
//		}
//		total_read += amount_read;
//	}
//
//	memcpy(buffer + total_read, "\0", 1);
//
//	o_pLauncherVersion->SetVersion(buffer);
//
//	// close file handle
//	InternetCloseHandle(hLauncherVersionFile);
//
//	sprintf(pDirString, "%s/dversion.ver", (LPCSTR)g_pPreGlobal->m_szDeleteFileListUploadPath);
//	HINTERNET hDelFileListVersionFile
//		= FtpOpenFile(hFtpConnect, pDirString, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);
//
//	amount_read = 0;
//	total_read = 0;
//
//	ret = FALSE;
//	while (ret)
//	{
//		ret = InternetReadFile(hDelFileListVersionFile, buffer, DOWNLOAD_BUFFER_SIZE, &amount_read);
//		if (FALSE == ret)
//		{// 2006-06-30 by cmkwon
//			DWORD err;
//			char errMsg[1024];
//			DWORD errMsgLen = 1024;
//			InternetGetLastResponseInfo(&err, errMsg, &errMsgLen);
//			server::log(TRUE, "[ERROR] %s ", errMsg);
//			return FALSE;
//		}
//		if (0 == amount_read)
//		{// 2006-06-30 by cmkwon
//			break;
//		}
//		total_read += amount_read;
//	}
//
//	memcpy(buffer + total_read, "\0", 1);
//	if (0 < total_read)
//	{
//		o_pDeleteFileListVersion->SetVersion(buffer);
//	}
//
//	// close file handle
//	InternetCloseHandle(hDelFileListVersionFile);
//
//	// delete buffer
//	delete buffer;
//
//	// close handles
//	InternetCloseHandle(hDir);
//	InternetCloseHandle(hFtpConnect);
//	InternetCloseHandle(hInternet);
//
//#ifdef _DEBUG
//	for (ez_map<VersionInfo, VersionInfo>::iterator itr = o_pMapVersionOld2New->begin(); itr != o_pMapVersionOld2New->end(); itr++)
//	{
//		VersionInfo tmpOldVersion = itr->first;
//		VersionInfo tmpNewVersion = itr->second;
//		DBGOUT("  Client Version: %s -> %s\n", tmpOldVersion.GetVersionString(), tmpNewVersion.GetVersionString());
//	}
//	DBGOUT("  Latest Client Version: %s\n", o_pMapVersionOld2New->rbegin()->second.GetVersionString());
//	DBGOUT("\n  Launcher Version: %s\n", o_pLauncherVersion->GetVersionString());
//	DBGOUT("  Delete File List Version: %s\n", o_pDeleteFileListVersion->GetVersionString());
//	DBGOUT("  Notice Version: %s\n\n", o_pNoticeVersion->GetVersionString());
//#endif
//
//	return TRUE;
//}
//
//// PreServer 시작 시 자동 버젼 업데이트 갱신
//BOOL CPreIOCP::PreGetVersionInfoByHTTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion)
//{
//	char			szRemoteFileName[SIZE_MAX_FTP_FILE_PATH];
//	char			szTempFileName[SIZE_MAX_FTP_FILE_PATH];
//	CHttpManager	httpMan;
//	Err_t			errCode;
//
//	//	GetOnlyPathWithoutFileName(g_pPreGlobal->m_szLauncherFileDownloadHttpPath);
//	//	GetOnlyPathWithoutFileName(g_pPreGlobal->m_szDeleteFileListDownloadHttpPath);
//	//	GetOnlyPathWithoutFileName(g_pPreGlobal->m_szNoticeFileDownloadHttpPath);
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 2007-01-08 by cmkwon, Delete file list version
//	util::strncpy(szTempFileName, "tmdversion.ver", SIZE_MAX_FTP_FILE_PATH);
//	sprintf(szRemoteFileName, "%s/dversion.ver", (LPSTR)(LPCSTR)GetOnlyPathWithoutFileName((CString)g_pPreGlobal->m_szDeleteFileListDownloadHttpPath));
//	errCode = httpMan.DownloadFileByHttp(m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort
//		, szRemoteFileName, szTempFileName);
//	if (ERR_NO_ERROR != errCode)
//	{
//		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
//		CSystemLogManager errLog;
//		if (errLog.InitLogManger(TRUE, "SCMonitor", "./"))
//		{
//			char szResult[2048];
//			util::zero(szResult, 2048);
//			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
//				, m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort, szRemoteFileName);
//			errLog.WriteSystemLog(szResult);
//		}
//		return FALSE;
//	}
//
//	const int	SIZE_BUFF = 1024;
//	char		buffer[SIZE_BUFF];
//	{
//		ifstream	finD;
//
//		finD.open(szTempFileName);
//		if (false == finD.is_open())
//		{
//			int nErr = GetLastError();
//			SetLastError(0);
//			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
//			return FALSE;
//		}
//		util::zero(buffer, SIZE_BUFF);
//		finD.getline(buffer, SIZE_BUFF);
//		if (7 > strlen(buffer))
//		{
//			buffer[SIZE_BUFF - 1] = '\0';
//			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
//			return FALSE;
//		}
//		o_pDeleteFileListVersion->SetVersion(buffer);
//		finD.close();
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-01-08 by cmkwon, Launcher File Version
//	util::strncpy(szTempFileName, "tmlversion.ver", SIZE_MAX_FTP_FILE_PATH);
//	sprintf(szRemoteFileName, "%s/lversion.ver", (LPSTR)(LPCSTR)GetOnlyPathWithoutFileName((CString)g_pPreGlobal->m_szLauncherFileDownloadHttpPath));
//	errCode = httpMan.DownloadFileByHttp(m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort
//		, szRemoteFileName, szTempFileName);
//	if (ERR_NO_ERROR != errCode)
//	{
//		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
//		CSystemLogManager errLog;
//		if (errLog.InitLogManger(TRUE, "SCMonitor", "./"))
//		{
//			char szResult[2048];
//			util::zero(szResult, 2048);
//			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
//				, m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort, szRemoteFileName);
//			errLog.WriteSystemLog(szResult);
//		}
//		return FALSE;
//	}
//
//	{
//		ifstream finl;
//		finl.open(szTempFileName);
//		if (false == finl.is_open())
//		{
//			int nErr = GetLastError();
//			SetLastError(0);
//			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
//			return FALSE;
//		}
//		util::zero(buffer, SIZE_BUFF);
//		finl.getline(buffer, SIZE_BUFF);
//		if (7 > strlen(buffer))
//		{
//			buffer[SIZE_BUFF - 1] = '\0';
//			DbgOut("file(%s) error :%s\r\n", szTempFileName, buffer);
//			return FALSE;
//		}
//		o_pLauncherVersion->SetVersion(buffer);
//		finl.close();
//	}
//
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2007-01-08 by cmkwon, Version list file
//	util::strncpy(szTempFileName, "tmversionlist.ver", SIZE_MAX_FTP_FILE_PATH);
//	errCode = httpMan.DownloadFileByHttp(m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort
//		, g_pPreGlobal->m_szVersionListFileDownloadHttpPath, szTempFileName);
//	if (ERR_NO_ERROR != errCode)
//	{
//		// 2008-06-20 by cmkwon, SCMonitor Reload version info error 파일로 저장
//		CSystemLogManager errLog;
//		if (errLog.InitLogManger(TRUE, "SCMonitor", "./"))
//		{
//			char szResult[2048];
//			util::zero(szResult, 2048);
//			sprintf(szResult, "[Error]Reload version info error !! HTTPServer(%s:%d), ErrorFileName(%s)\r\n"
//				, m_szDownloadHttpServerIP, g_pPreGlobal->m_nDownloadHttpServerPort, szRemoteFileName);
//			errLog.WriteSystemLog(szResult);
//		}
//		return FALSE;
//	}
//
//	{
//		ifstream finV;
//		finV.open(szTempFileName);
//		if (false == finV.is_open())
//		{
//			int nErr = GetLastError();
//			SetLastError(0);
//			DbgOut("Unable to create file(%s): error code(%d)\r\n", szTempFileName, nErr);
//			return FALSE;
//		}
//
//		bool bFileEndFlag = false;
//		do
//		{
//			util::zero(buffer, SIZE_BUFF);
//			finV.getline(buffer, SIZE_BUFF);
//			bFileEndFlag = finV.eof();
//
//			char		buffLine[SIZE_BUFF];
//			char		seps[] = "_zip";
//			util::strncpy(buffLine, buffer, SIZE_BUFF);
//			if (0 == strnicmp(buffLine, "#", 1))
//			{// 2007-01-09 by cmkwon, 주석임
//				continue;
//			}
//
//			// from version
//			char *token1 = strtok(buffLine, seps);
//			// to version
//			char *token2 = strtok(NULL, seps);
//
//			if (token1 && token2)
//			{
//				VersionInfo OldVersion(token1);
//				VersionInfo NewVersion(token2);
//				if (OldVersion.IsValidVersionInfo()
//					&& NewVersion.IsValidVersionInfo()
//					&& OldVersion < NewVersion)
//				{
//					VersionInfo *pVersion = o_pMapVersionOld2New->findEZ_ptr(OldVersion);
//					if (pVersion == NULL)
//					{
//						o_pMapVersionOld2New->insertEZ(OldVersion, NewVersion);
//					}
//					else
//					{
//						if (*pVersion < NewVersion)
//						{
//							*pVersion = NewVersion;
//						}
//					}
//				}
//			}
//
//		} while (false == bFileEndFlag);
//		finV.close();
//	}
//
//	return TRUE;
//}

CString CPreIOCP::GetOnlyPathWithoutFileName(CString & i_szFilePath)
{
	if (i_szFilePath.ReverseFind('/') != -1)
	{
		return i_szFilePath.Left(i_szFilePath.ReverseFind('/'));
	}

	return "";
}