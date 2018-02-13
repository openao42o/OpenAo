// IMIOCP.cpp: implementation of the CIMIOCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMIOCP.h"
#include "IMIOCPSocket.h"
#include "AtumIMDBManager.h"
#include "IMGlobal.h"
#include "AtumSJ.h"
#include "IMParty.h"
#include "ANoticeManager.h"			// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
//#include "SecurityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
struct Comparestring
{
    bool operator() (const string& v1, const string& v2)
    {
        return v1 < v2;			// 오름차순 정렬
    }
};

CIMIOCP::CIMIOCP(int nPort, char *szLocalIP) :
	CIOCP(CLIENT_INDEX_START_NUM, SIZE_MAX_IMSERVER_SESSION, nPort, szLocalIP, ST_IM_SERVER, sizeof(CIMIOCPSocket))
{
	CIMIOCPSocket::ms_pIMIOCP		= this;
	CGuild::ms_pIMIOCP3				= this;
	CIMParty::ms_pIMIOCP2			= this;

	m_mtvectorDeletedParties.reserve(50);

	auto pIMIOCPSocket = new CIMIOCPSocket[m_dwArrayClientSize];

	for(int i = 0; i < m_dwArrayClientSize; i++) pIMIOCPSocket[i].InitIOCPSocket(i);

	m_pArrayIOCPSocket = pIMIOCPSocket;

	m_nServerDownVerifyNumber = 0xFFFF;

	m_uiIMAccumulatedTotalUserCounts = 0;
	m_uiIMMaxTotalUserCounts = 0;

	m_VCNLeaderCharacterUID				= NULL;
	m_ANILeaderCharacterUID				= NULL;
	m_VCNSub1LeaderCharacterUID			= NULL;
	m_ANISub1LeaderCharacterUID			= NULL;
	m_VCNSub2LeaderCharacterUID			= NULL;
	m_ANISub2LeaderCharacterUID			= NULL;

	this->SetFieldServerSocket(nullptr);

	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	m_bOnCallGMSystem					= FALSE;
	m_atCallGMStartTime.SetCurrentDateTime(TRUE);
	m_atCallGMEndTime					= m_atCallGMStartTime;

	m_pANoticeManager					= new CANoticeManager(this);		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - CIMIOCP::CIMIOCP() 생성

	m_bEnableCNC = TRUE;

	for (int i = 0; ArrCommandListforUser[i] != nullptr; ++i)
	{
		// panosk wonders why they disable cmds HERE
		// let's remove this feature

		//switch(g_pIMGlobal->GetLanguageType())
		//{
		//case LANGUAGE_TYPE_DEFAULT:
		//	{
		//		if(0 == strcmp(STRCMD_CS_COMMAND_CALLGM_HELP, ArrCommandListforUser[i])
		//			|| 0 == strcmp(STRCMD_CS_COMMAND_PLAYTIME_HELP, ArrCommandListforUser[i]))
		//		{
		//			continue;
		//		}
		//	}
		//	break;
		//case LANGUAGE_TYPE_ENGLISH:
		//	{
		//		if(0 == strcmp(STRCMD_CS_COMMAND_PLAYTIME_HELP, ArrCommandListforUser[i]))
		//		{
		//			continue;
		//		}
		//	}
		//	break;
		//}

		m_vectstringCommandListforUser.emplace_back(ArrCommandListforUser[i]);
	}

	sort(m_vectstringCommandListforUser.begin(), m_vectstringCommandListforUser.end(), Comparestring());
	
	for (int i = 0; ArrGameMasterCommandList[i] != nullptr; ++i)
		
		m_vectstringGameMasterCommandList.emplace_back(ArrGameMasterCommandList[i]);

	sort(m_vectstringGameMasterCommandList.begin(), m_vectstringGameMasterCommandList.end(), Comparestring());

	for (int i = 0; ArrCommandList[i] != nullptr; ++i) m_vectstringCommandList.emplace_back(ArrCommandList[i]);

	sort(m_vectstringCommandList.begin(), m_vectstringCommandList.end(), Comparestring());

	m_bArenaServerCheck	= false;

	random::init();

	//m_mtRandManager.seed();
}

CIMIOCP::~CIMIOCP()
{
	ListenerClose();
	CIMIOCP::IOCPClean();

	//memset(m_ArrayClient, 0x00, sizeof(CIOCPSocket*) * COUNT_MAX_SOCKET_SESSION);
	if (m_pArrayIOCPSocket != nullptr)
	{
		delete[] reinterpret_cast<CIMIOCPSocket*>(m_pArrayIOCPSocket);
		m_pArrayIOCPSocket = nullptr;
	}

	util::del(m_pANoticeManager);		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - CIMIOCP::~CIMIOCP() 소멸

	util::del(m_pAtumIMDBManager);
	util::del(m_pIMTickManager);
	//util::del(m_pSecurityManager);	// 2011-06-22 by hskim, 사설 서버 방지
}


CIMIOCPSocket* CIMIOCP::GetIMIOCPSocket(int idx)
{
	return (CIMIOCPSocket*)GetIOCPSocket(idx);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CIMIOCPSocket* CIMIOCP::GetIMIOCPSocketByCharacterUID(UID32_t i_characterUID)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CIMIOCPSocket* CIMIOCP::GetIMIOCPSocketByCharacterUID(UID32_t i_characterUID)
{
	CIMIOCPSocket *pIISock = m_MapChactarterUniqueNumber.findLock(i_characterUID);
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing()
		|| i_characterUID != pIISock->GetCharacter()->CharacterUniqueNumber)
	{
		return NULL;
	}
	return pIISock;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CIMIOCPSocket* CIMIOCP::GetIMIOCPSocketByCharacterName(char *i_szCharacterName)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-16 ~ 2005-12-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CIMIOCPSocket* CIMIOCP::GetIMIOCPSocketByCharacterName(char *i_szCharacterName)
{
	char tmCharacterName[SIZE_MAX_CHARACTER_NAME];	
	CIMIOCPSocket *pIISock = m_mCharacterName.findLock(GGetLowerCase(tmCharacterName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME));
	if(NULL == pIISock
		|| FALSE == pIISock->IsValidCharacter())
	{
		return NULL;
	}
	return pIISock;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CIMIOCPSocket* CIMIOCP::GetIMIOCPSocketByAccountUID(UID32_t i_accountUID)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-16 ~ 2005-12-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CIMIOCPSocket* CIMIOCP::GetIMIOCPSocketByAccountUID(UID32_t i_accountUID)
{
	CIMIOCPSocket *pIISock = m_MapAccountUniqueNumber.findLock(i_accountUID);
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing()
		|| i_accountUID != pIISock->GetCharacter()->AccountUniqueNumber)
	{
		return NULL;
	}
	return pIISock;
}

CHARACTER* CIMIOCP::GetCharacterByArrayIndex(int idx)
{
	CIMIOCPSocket *pIMIOCPSocket = GetIMIOCPSocket(idx);
	if(pIMIOCPSocket == NULL)
	{
		return NULL;
	}

	return (CHARACTER*)pIMIOCPSocket->GetCharacter();
}

BOOL CIMIOCP::IOCPInit(void)
{
	CGuildMember::ms_pIMIOCP1	= this;

	BOOL	bRet = FALSE;

/*	// 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
	char szServerName[SIZE_MAX_STRING_128] = { 0, };
	sprintf(szServerName, "IMServer_%s", g_pIMGlobal->GetServerGroupName());
	if( FALSE == g_pGlobal->CreateDuplicateRun(szServerName) )
	{
		MessageBox(NULL, "ERROR : \nApplication is running already...", szServerName, MB_OK);
		return FALSE;
	}
	// end 2013-06-12 by hskim, 하나의 OS 에서 서버 동시 실행
*/
	CODBCStatement odbcStmt;
// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//	bRet = odbcStmt.Init((LPCSTR)g_pGlobal->GetODBCDSN(), (LPCSTR)g_pGlobal->GetODBCUID(), (LPCSTR)g_pGlobal->GetODBCPASSWORD());
	bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (!bRet)
	{
		// 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CIMIOCP::IOCPInit_1) !! %s,%d %s %s %s\r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());

		MessageBox(NULL, "DB connect fail@CFieldIOCP::IOCPInit()", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// Load AtumSJ Data
	if (!CAtumSJ::LoadData(&odbcStmt))
	{
		MessageBox(NULL, "CAtumSJ::LoadData() fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-20 by cmkwon
	if(FALSE == CAtumDBHelper::LoadInfluenceLeader(&m_VCNLeaderCharacterUID, &m_ANILeaderCharacterUID, &odbcStmt))
	{
		MessageBox(NULL, "CAtumDBHelper::LoadInfluenceLeader_ fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	// 2006-12-08 by dhjin, 부지도자1 로딩
	if(FALSE == CAtumDBHelper::LoadInfluenceSub1Leader(&m_VCNSub1LeaderCharacterUID, &m_ANISub1LeaderCharacterUID, &odbcStmt))
	{
		MessageBox(NULL, "CAtumDBHelper::LoadInfluenceSub1Leader fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}
	// 2006-12-08 by dhjin, 부지도자2 로딩
	if(FALSE == CAtumDBHelper::LoadInfluenceSub2Leader(&m_VCNSub2LeaderCharacterUID, &m_ANISub2LeaderCharacterUID, &odbcStmt))
	{
		MessageBox(NULL, "CAtumDBHelper::LoadInfluenceSub2Leader fail", "ERROR - SpaceCowboy FieldServer", NULL);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-16 by dhjin, EP3 채팅방 -
	m_ChatRoomManager.InitChatRoomManager(this);
	
	// Initialize Log Manager
#ifdef ARENA
	g_pIMGlobal->GetPtrChattingLogManager()->InitLogManger(TRUE, "ArenaIMChatting", (char*)(CONFIG_ROOT + "../log/ChattingLog/").c_str());
#else
	g_pIMGlobal->GetPtrChattingLogManager()->InitLogManger(TRUE, "IMChatting", (char*)(CONFIG_ROOT + "../log/ChattingLog/").c_str());
#endif

	// Initialize DB Manager
	m_pAtumIMDBManager = new CAtumIMDBManager(this);
	if (m_pAtumIMDBManager->Init() == FALSE) { return FALSE; }

	// Initialize Tick Manager
	m_pIMTickManager = new CIMTickManager(this);
	if (m_pIMTickManager->InitThread() == FALSE) { return FALSE; }

	// Set Exchange Message Level
	GSetexchangeMsgPrintLevel(PRINTLEVEL_NO_MSG);


	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - CIMIOCP::IOCPInit() 로딩 요청
	m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_ReloadAdminAutoNotice, NULL, NULL);


	// clean
	odbcStmt.Clean();

	return CIOCP::IOCPInit();
}

void CIMIOCP::IOCPClean(void)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "IM Server End\r\n");
	g_pGlobal->WriteSystemLog(szSystemLog);

	g_pIMGlobal->GetPtrChattingLogManager()->Clean();
	
	CIOCP::IOCPClean();

	m_pAtumIMDBManager->Clean();

	m_pIMTickManager->CleanThread();


	/////////////////////////////////////////////////////////////////////////////////////
	// 생성된 Party 메모리 삭제
	m_MapParties.lock();
	mt_map<PartyID_t, CIMParty*>::iterator it(m_MapParties.begin());
	for(; it != m_MapParties.end(); it++)
	{
		util::del(it->second);
	}
	m_MapParties.clear();
	m_MapParties.unlock();

	/////////////////////////////////////////////////////////////////////////////////////
	// Deleted 된 Party 메모리 삭제
	m_mtvectorDeletedParties.lock();
	for (auto x : m_mtvectorDeletedParties) delete x;
	m_mtvectorDeletedParties.clear();
	m_mtvectorDeletedParties.unlock();
}

SThreadInfo *CIMIOCP::CheckIOCPThread(DWORD i_dwThreadIdToExclude)
{
	SThreadInfo* pTInfo = CIOCP::CheckIOCPThread(i_dwThreadIdToExclude);
	if(!pTInfo)
	{
		pTInfo = m_pAtumIMDBManager->CheckDBThread(i_dwThreadIdToExclude);
	}
	if(pTInfo
		&& !m_MonitorIOCPSocketPtrVector.empty())
	{
		INIT_MSG_WITH_BUFFER(MSG_IM_SERVER_THREAD_STATUS, T_IM_SERVER_THREAD_STATUS, pSend, SendBuf);
		pSend->dwThreadId = pTInfo->dwThreadId;
		util::strncpy(pSend->szThreadComment, pTInfo->GetThreadCheckComment(pTInfo->enThreadCheckType), SIZE_MAX_THREAD_COMMENT);
		pSend->dwTimeGap = timeGetTime() - pTInfo->dwLastUseStartTick;
		SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_IM_SERVER_THREAD_STATUS));

		INIT_MSG(MSG_IA_ADMIN_CHANGE_SERVER_STATE, T_IA_ADMIN_CHANGE_SERVER_STATE, msgChangeSS, SendBuf);
		msgChangeSS->ServerID = SERVER_ID(g_pIMGlobal->GetPublicIPLocal(), m_nListenerPort);
		msgChangeSS->ServerType = ST_IM_SERVER;
		msgChangeSS->ServerState = SERVER_STATE_ABNORMAL;
		SendMessageToAdminTool(SendBuf, MSG_SIZE(MSG_IA_ADMIN_CHANGE_SERVER_STATE));
	}
	return pTInfo;
}

BOOL CIMIOCP::IsConnectedUser(char* AccountName, CIMIOCPSocket **ppSock)
{
	*ppSock = m_mAccountName.findLock(AccountName);
	if(*ppSock == NULL
		|| FALSE == (*ppSock)->IsUsing()
		|| 0 != strnicmp(AccountName, (*ppSock)->GetCharacter()->AccountName, SIZE_MAX_ACCOUNT_NAME))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CIMIOCP::SendMessageToFieldServer(BYTE *buffer, int size)
{
	// 2015-07-21 Future, Optimised Masang Bullshit Code
	if (!m_pFieldServerSocket || !m_pFieldServerSocket->IsUsing())
	{
		m_pFieldServerSocket = NULL;
		return FALSE;
	}
	return m_pFieldServerSocket->SendAddData(buffer, size);

//	// 필드 서버들에게 전송
//	m_MapFieldServer.lock();
//	mt_map<SERVER_ID, FieldServerInfo*>::iterator itrServer = m_MapFieldServer.begin();
//	while(itrServer != m_MapFieldServer.end())
//	{
//		FieldServerInfo *pServerInfo = itrServer->second;
//		pServerInfo->pSocket->SendAddData(buffer, size);
//		itrServer++;
//	}
//	m_MapFieldServer.unlock();
}


void CIMIOCP::SendMessageToAllAdmin(BYTE *buffer, int size)
{
	// lock
	mt_auto_lock mtA(&m_setAdminCharacterUniqueNumbers);

	mtsetUID32::iterator itr = m_setAdminCharacterUniqueNumbers.begin();
	while (itr != m_setAdminCharacterUniqueNumbers.end())
	{
		CIMIOCPSocket *pSocket = this->GetIMIOCPSocketByCharacterUID(*itr);
		if (NULL == pSocket
			|| FALSE == pSocket->IsUsing())
		{
			m_setAdminCharacterUniqueNumbers.erase(itr++);
			continue;
		}

		pSocket->SendAddData(buffer, size);		
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::SendString128ToAllAdmin(BYTE i_byString128Ty, char *i_szString)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - CIMIOCP::SendString128ToAllAdmin() 추가
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::SendString128ToAllAdmin(BYTE i_byString128Ty, char *i_szString)
{
	// lock
	mt_auto_lock mtA(&m_setAdminCharacterUniqueNumbers);

	mtsetUID32::iterator itr = m_setAdminCharacterUniqueNumbers.begin();
	while (itr != m_setAdminCharacterUniqueNumbers.end())
	{
		CIMIOCPSocket *pSocket = this->GetIMIOCPSocketByCharacterUID(*itr);
		if (NULL == pSocket
			|| FALSE == pSocket->IsUsing())
		{
			m_setAdminCharacterUniqueNumbers.erase(itr++);
			continue;
		}

		pSocket->SendString128(i_byString128Ty, i_szString);		
		itr++;
	}
}

BOOL CIMIOCP::AddToAdminCharacter(UID32_t AdminCharacterUniqueNumber)
{
	return m_setAdminCharacterUniqueNumbers.insertLock(AdminCharacterUniqueNumber);
}

BOOL CIMIOCP::DeleteFromAdminCharacter(UID32_t AdminCharacterUniqueNumber)
{
	return m_setAdminCharacterUniqueNumbers.deleteLock(AdminCharacterUniqueNumber);
}


BOOL CIMIOCP::OnGuildLoading(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock)
{
	mt_auto_lock mtA(&m_MapGuild);
	if(FALSE == i_pIISock->IsUsing())
	{
		return FALSE;
	}

	CGuild *pGuild = m_MapGuild.findNoLock(i_pGuild->m_GuildUniqueNumber);
	if (NULL == pGuild)
	{// 
		CGuildMember *ptmGMember;
		if(FALSE == i_pGuild->GetGuildMember(i_pIISock->m_character.CharacterUniqueNumber, ptmGMember))
		{// 여단에 지금 접속한 캐릭이 속해 있지 않음

			g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] Guild loading Error, GuildUID(%8d) CharacterUID(%8d)\r\n"
				, i_pGuild->m_GuildUniqueNumber, i_pIISock->m_character.CharacterUniqueNumber);			

			m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteGuildUIDOfCharacter, i_pIISock, NULL, NULL, i_pIISock->m_character.CharacterUniqueNumber);
			i_pIISock->SetGuildUniqueNumber(INVALID_GUILD_UID);
			i_pIISock->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);
			return FALSE;
		}
		i_pGuild->SetOnlineGuildMember(i_pIISock, FALSE);

		pGuild = new CGuild(i_pGuild);
		m_MapGuild.insertNoLock(pGuild->m_GuildUniqueNumber, pGuild);
		return TRUE;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 로딩된 여단에 존재 하지 않는 여단원을 추가한다.
	listGuildMember::iterator itr(i_pGuild->m_ListGuildMember.begin());
	for(; itr != i_pGuild->m_ListGuildMember.end(); itr++)
	{
		CGuildMember *ptmGMember;
		if(FALSE == pGuild->GetGuildMember(itr->m_CharacterUniqueNumber, ptmGMember))
		{// 로딩된 여단에 캐릭이 속해 있지 않음 - 추가해줘야함

			// 로딩된 여단에 추가한다
			pGuild->AddGuildMember(*itr);

			// 여단원에게 추가된 정보 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_ACCEPT_INVITE_OK, T_IC_GUILD_ACCEPT_INVITE_OK, msgAccepOK, msgAccepOKBuf);
			msgAccepOK->MemberInfo = *itr;
			pGuild->SendMsgToGuildMembers(msgAccepOKBuf, MSG_SIZE(MSG_IC_GUILD_ACCEPT_INVITE_OK));
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 로딩된 여단에서 여단에서 탈퇴된 여단원을 제거한다
	itr = pGuild->m_ListGuildMember.begin();
	while (itr != pGuild->m_ListGuildMember.end())
	{
		CGuildMember *ptmGMember;
		if(FALSE == i_pGuild->GetGuildMember(itr->m_CharacterUniqueNumber, ptmGMember))
		{// DB에서 로딩한 여단정보에 캐릭이 없음 - 탈퇴 처리 해야함

			if(itr->m_pIMIOCPSocket
				&& itr->m_pIMIOCPSocket->IsUsing())
			{
				itr->m_pIMIOCPSocket->SetGuildUniqueNumber(INVALID_GUILD_UID);
				itr->m_pIMIOCPSocket->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);
			}
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_LEAVE_OK, T_IC_GUILD_LEAVE_OK, msgLeaveOK, SendBuf);
			msgLeaveOK->MemberUniqueNumber = itr->m_CharacterUniqueNumber;			
			pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_LEAVE_OK));

			itr = pGuild->m_ListGuildMember.erase(itr);
			continue;
		}
		itr++;
	}
	
	CGuildMember *ptmGMember;
	if(FALSE == pGuild->GetGuildMember(i_pIISock->m_character.CharacterUniqueNumber, ptmGMember))
	{// 여단에 지금 접속한 캐릭이 속해 있지 않음
		
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] Guild loading Error, GuildUID(%8d) CharacterUID(%8d)\r\n"
			, pGuild->m_GuildUniqueNumber, i_pIISock->m_character.CharacterUniqueNumber);			
		
		m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteGuildUIDOfCharacter, i_pIISock, NULL, NULL, i_pIISock->m_character.CharacterUniqueNumber);
		i_pIISock->SetGuildUniqueNumber(INVALID_GUILD_UID);
		i_pIISock->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);
		return FALSE;
	}
	pGuild->SetOnlineGuildMember(i_pIISock, TRUE);

	if(0 != strncmp(pGuild->m_GuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME))
	{// 2007-08-01 by cmkwon, 여단명이 변경되었는지 체크

		///////////////////////////////////////////////////////////////////////////////
		// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 이미 로딩된 여단이 있을 경우 새로 로딩된 여단명으로 설정
		util::strncpy(pGuild->m_GuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);

		///////////////////////////////////////////////////////////////////////////////
		// 2007-08-01 by cmkwon, 변경된 여단명 모든 여단원에 전송
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK, T_IC_GUILD_CHANGE_GUILD_NAME_OK, msgChangeName, msgChangeNameBuf);
		util::strncpy(msgChangeName->NewGuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
		pGuild->SendMsgToGuildMembers(msgChangeNameBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK));
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 새로 로딩된 여단마크로 설정
	pGuild->m_GuildMark		= i_pGuild->m_GuildMark;

	return TRUE;
}

CGuild* CIMIOCP::GetGuildByName(const char *i_szGuildName)
{
	CGuild *pRetGuild = NULL;

	m_MapGuild.lock();

	mt_map<UID32_t, CGuild*>::iterator itr = m_MapGuild.begin();
	while(m_MapGuild.end() != itr)
	{
		CGuild *pGuild = itr->second;

		if (strnicmp(pGuild->m_GuildName, i_szGuildName, SIZE_MAX_GUILD_NAME) == 0)
		{
			pRetGuild = pGuild;
		}

		itr++;
	}

	m_MapGuild.unlock();

	return pRetGuild;
}

void CIMIOCP::DismemberAllPendingGuild(ATUM_DATE_TIME *pCurrentDateTime)
{
	vector<UINT> tmpVectorGuildToBeDeleted;

	///////////////////////////////////////////////////////////////////////////
	// 지워져야 할 길드 찾기

	// lock
	m_MapGuild.lock();

	mt_map<UID32_t, CGuild*>::iterator itr = m_MapGuild.begin();
	while(m_MapGuild.end() != itr)
	{
		CGuild *pGuild = itr->second;

		if (pGuild->m_GuildState == GUILD_STATE_DISMEMBER_READY
			&& *pCurrentDateTime >= pGuild->m_DismemberDate)
		{
			tmpVectorGuildToBeDeleted.push_back(pGuild->m_GuildUniqueNumber);
		}

		itr++;
	}

	// unlock
	m_MapGuild.unlock();

	if (tmpVectorGuildToBeDeleted.size() == 0)
	{
		return;
	}

	ATUM_DATE_TIME tmpTime;
	tmpTime.SetCurrentDateTime();

	///////////////////////////////////////////////////////////////////////////
	// 모두 지우기
	for (int i = 0; i < tmpVectorGuildToBeDeleted.size(); i++)
	{
		m_pAtumIMDBManager->ExecuteQuery(QT_GuildDeleteGuild, NULL, NULL, NULL, (UINT)tmpVectorGuildToBeDeleted[i]);
	}
}

// 2004-10-20 by cmkwon, 원래 사용하지 않던 함수임
//void CIMIOCP::ReadyGuildWar(UID32_t DefenderGuildUID, UID32_t ChallengerGuildUID, INT ReadyTimeInSeconds)
//{
//	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_WAR_READY, T_IC_GUILD_WAR_READY, pMsgReady, pMsgReadyBuf);
//	pMsgReady->WaitingTime = ReadyTimeInSeconds;
//
//	CGuild *pDefenderGuild = m_MapGuild.findLock(DefenderGuildUID);
//	if (pDefenderGuild == NULL)
//	{
//		// 몰수패 처리
//		ASSERT_NOT_IMPLEMENTED_YET();
//		return;
//	}
//	pMsgReady->PeerGuildUID = ChallengerGuildUID;
//	pDefenderGuild->SendMsgToGuildMembers(pMsgReadyBuf, MSG_SIZE(MSG_IC_GUILD_WAR_READY));
//
//	CGuild *pChallengerGuild = m_MapGuild.findLock(ChallengerGuildUID);
//	if (pChallengerGuild == NULL)
//	{
//		// 몰수패 처리
//		ASSERT_NOT_IMPLEMENTED_YET();
//		return;
//	}
//	pMsgReady->PeerGuildUID = DefenderGuildUID;
//	pChallengerGuild->SendMsgToGuildMembers(pMsgReadyBuf, MSG_SIZE(MSG_IC_GUILD_WAR_READY));
//}

// 2004-10-20 by cmkwon, 카운트 방식 수정
//void CIMIOCP::StartGuildWar(UID32_t DefenderGuildUID, UID32_t ChallengerGuildUID)
//{
//	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_START_WAR, T_IC_GUILD_START_WAR, pMsgStart, pMsgStartBuf);
//
//	CGuild *pDefenderGuild = m_MapGuild.findLock(DefenderGuildUID);
//	if (pDefenderGuild == NULL)
//	{
//		// 몰수패 처리
//		ASSERT_NOT_IMPLEMENTED_YET();
//		return;
//	}
//	pMsgStart->PeerGuildUID = ChallengerGuildUID;
//	pDefenderGuild->SendMsgToGuildMembers(pMsgStartBuf, MSG_SIZE(MSG_IC_GUILD_START_WAR));
//
//	CGuild *pChallengerGuild = m_MapGuild.findLock(ChallengerGuildUID);
//	if (pChallengerGuild == NULL)
//	{
//		// 몰수패 처리
//		ASSERT_NOT_IMPLEMENTED_YET();
//		return;
//	}
//	pMsgStart->PeerGuildUID = DefenderGuildUID;
//	pChallengerGuild->SendMsgToGuildMembers(pMsgStartBuf, MSG_SIZE(MSG_IC_GUILD_START_WAR));
//}

void CIMIOCP::SendMsgToGuildMembersByGuildUID(UID32_t i_uidGuild
											  , BYTE *buffer
											  , int size
											  , UID32_t CharacterUniqueNumberToExclude/*=INVALID_UNIQUE_NUMBER*/)
{
	this->m_MapGuild.lock();
	{
		CGuild *pGuild = this->m_MapGuild.findLock(i_uidGuild);
		if(pGuild)
		{
			pGuild->SendMsgToGuildMembers(buffer, size, CharacterUniqueNumberToExclude);
		}
	}
	this->m_MapGuild.unlock();
}

void CIMIOCP::GuildWarCheck(UID32_t i_uidGuildUID
							, MAP_CHANNEL_INDEX i_mapChannIdxCurrent
							, UID32_t i_uidAttackerGuildUID/*=INVALID_GUILD_UID*/)
{
	char szSystemLog[512];
//	sprintf(szSystemLog, "	CIMIOCP::GuildWarCheck1 GuildUID(%6d) Map(%s) AttackerGuildUID(%6d)\r\n"
//		, i_uidGuildUID, GET_MAP_STRING(i_mapChannIdxCurrent), i_uidAttackerGuildUID);
//	DBGOUT(szSystemLog);

	this->m_MapGuild.lock();
	{
		CGuild *ptmGuild = this->m_MapGuild.findLock(i_uidGuildUID);
		if(ptmGuild 
			&& GUILD_STATE_IN_GUILD_WAR == ptmGuild->m_GuildState
			&& ptmGuild->m_WarMapChannelIndex == i_mapChannIdxCurrent)
		{
			if(i_uidAttackerGuildUID
				&& i_uidAttackerGuildUID != ptmGuild->m_uidWarPeerGuildUID)
			{
				this->m_MapGuild.unlock();
				return;
			}

			CGuild *ptmPeerGuild = this->m_MapGuild.findLock(ptmGuild->m_uidWarPeerGuildUID);
			if(ptmPeerGuild
				&& GUILD_STATE_IN_GUILD_WAR == ptmPeerGuild->m_GuildState)
			{				
				if(i_uidAttackerGuildUID)
				{
					ptmPeerGuild->m_WarKillCounts++;
				}
				
				sprintf(szSystemLog, "	CIMIOCP::GuildWarCheck2 %s(killCounts:%3d) vs %s(killCounts:%3d)\r\n"
					, ptmGuild->m_GuildName, ptmGuild->m_WarKillCounts
					, ptmPeerGuild->m_GuildName, ptmPeerGuild->m_WarKillCounts);
				DBGOUT(szSystemLog);

				int nOnlineMemberCount = ptmGuild->GetLiveMemberCountByMapChannelIndex(ptmGuild->m_WarMapChannelIndex);
				if(nOnlineMemberCount <= 1)
				{// 길드전 패배

					this->GuildWarEnd(BATTLE_END_WIN, ptmPeerGuild->m_GuildUniqueNumber);
				}
			}
		}
	}
	this->m_MapGuild.unlock();
}

void CIMIOCP::GuildWarEnd(USHORT i_usWarEndType, UID32_t i_uidGuildUID, DWORD i_dwTimerUniqueNumber/*=0*/)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2004-12-09 by cmkwon, FieldServer로 알려줘야함
	INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_NOTIFY_END_WAR, T_FI_GUILD_NOTIFY_END_WAR, pSeMsg, FIBuff);
	this->m_MapGuild.lock();
	{
		CGuild *pGuild = this->m_MapGuild.findLock(i_uidGuildUID);
		if(NULL == pGuild
			|| GUILD_STATE_IN_GUILD_WAR != pGuild->m_GuildState
			|| (BATTLE_END_TIME_LIMITE == i_usWarEndType && i_dwTimerUniqueNumber != pGuild->m_WarTimerUniqueNumber))
		{
			this->m_MapGuild.unlock();
			return;
		}
		
		CGuild *pPeerGuild = this->m_MapGuild.findNoLock(pGuild->m_uidWarPeerGuildUID);
		if(NULL == pPeerGuild
			|| GUILD_STATE_IN_GUILD_WAR != pPeerGuild->m_GuildState
			|| (BATTLE_END_TIME_LIMITE == i_usWarEndType && i_dwTimerUniqueNumber != pPeerGuild->m_WarTimerUniqueNumber))
		{
			this->m_MapGuild.unlock();
			return;
		}
		
//		char szSystemLog[512];
//		sprintf(szSystemLog, "	CIMIOCP::GuildWarEnd1 WarEndType(%d) GuildUID(%6d), %s(killCounts:%3d) vs %s(killCounts:%3d)\r\n"
//			, i_usWarEndType, i_uidGuildUID
//			, pGuild->m_GuildName, pGuild->m_WarKillCounts
//			, pPeerGuild->m_GuildName, pPeerGuild->m_WarKillCounts);
//		DBGOUT(szSystemLog);

		pGuild->m_GuildState		= GUILD_STATE_NORMAL;
		pPeerGuild->m_GuildState	= GUILD_STATE_NORMAL;
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR, T_IC_GUILD_END_WAR, pMsgEndWar, EndWarBuff);
		switch(i_usWarEndType)
		{
		case BATTLE_END_WIN:
			{				
				pGuild->WarResultWin();
				pPeerGuild->WarResultLose();

				util::strncpy(pMsgEndWar->PeerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				pMsgEndWar->WarEndType = BATTLE_END_WIN;
				pGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));

				util::strncpy(pMsgEndWar->PeerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				pMsgEndWar->WarEndType = BATTLE_END_DEFEAT;
				pPeerGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));

				///////////////////////////////////////////////////////////////////////////////
				// 2006-08-09 by cmkwon, 관리자/운영자에게도 정보를 전송한다.
				INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY, T_IC_GUILD_END_WAR_ADMIN_NOTIFY, pSEndWarNotify, SendBufNotify);
				pSEndWarNotify->WarEndType			= BATTLE_END_WIN;
				util::strncpy(pSEndWarNotify->WinerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				util::strncpy(pSEndWarNotify->LoserGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				this->SendMessageToAllAdmin(SendBufNotify, MSG_SIZE(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY));

				pSeMsg->WinnerGuildUID	= pGuild->m_GuildUniqueNumber;
				pSeMsg->LoserGuildUID	= pPeerGuild->m_GuildUniqueNumber;
				pSeMsg->WarEndType		= BATTLE_END_WIN;
				pSeMsg->WarMapChannel	= pGuild->m_WarMapChannelIndex;
			}
			break;
		case BATTLE_END_TIME_LIMITE:
			{
				if(pGuild->m_WarKillCounts == pPeerGuild->m_WarKillCounts)
				{// 무승부

					util::strncpy(pMsgEndWar->PeerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					pMsgEndWar->WarEndType = BATTLE_END_TIE;
					pGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));
					
					util::strncpy(pMsgEndWar->PeerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					pMsgEndWar->WarEndType = BATTLE_END_TIE;
					pPeerGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));
					
					///////////////////////////////////////////////////////////////////////////////
					// 2006-08-09 by cmkwon, 관리자/운영자에게도 정보를 전송한다.
					INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY, T_IC_GUILD_END_WAR_ADMIN_NOTIFY, pSEndWarNotify, SendBufNotify);
					pSEndWarNotify->WarEndType			= BATTLE_END_TIE;
					util::strncpy(pSEndWarNotify->WinerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					util::strncpy(pSEndWarNotify->LoserGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					this->SendMessageToAllAdmin(SendBufNotify, MSG_SIZE(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY));

					pSeMsg->WinnerGuildUID	= pGuild->m_GuildUniqueNumber;
					pSeMsg->LoserGuildUID	= pPeerGuild->m_GuildUniqueNumber;
					pSeMsg->WarEndType		= BATTLE_END_TIE;
					pSeMsg->WarMapChannel	= pGuild->m_WarMapChannelIndex;
				}
				else if(pGuild->m_WarKillCounts > pPeerGuild->m_WarKillCounts)
				{// pGuild가 승리

					pGuild->WarResultWin();
					pPeerGuild->WarResultLose();

					util::strncpy(pMsgEndWar->PeerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					pMsgEndWar->WarEndType = BATTLE_END_WIN;
					pGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));
					
					util::strncpy(pMsgEndWar->PeerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					pMsgEndWar->WarEndType = BATTLE_END_DEFEAT;
					pPeerGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));
					
					///////////////////////////////////////////////////////////////////////////////
					// 2006-08-09 by cmkwon, 관리자/운영자에게도 정보를 전송한다.
					INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY, T_IC_GUILD_END_WAR_ADMIN_NOTIFY, pSEndWarNotify, SendBufNotify);
					pSEndWarNotify->WarEndType			= BATTLE_END_WIN;
					util::strncpy(pSEndWarNotify->WinerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					util::strncpy(pSEndWarNotify->LoserGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					this->SendMessageToAllAdmin(SendBufNotify, MSG_SIZE(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY));

					pSeMsg->WinnerGuildUID	= pGuild->m_GuildUniqueNumber;
					pSeMsg->LoserGuildUID	= pPeerGuild->m_GuildUniqueNumber;
					pSeMsg->WarEndType		= BATTLE_END_WIN;
					pSeMsg->WarMapChannel	= pGuild->m_WarMapChannelIndex;
				}
				else
				{// pPeerGuild가 승리
					
					pGuild->WarResultLose();
					pPeerGuild->WarResultWin();

					util::strncpy(pMsgEndWar->PeerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					pMsgEndWar->WarEndType = BATTLE_END_DEFEAT;
					pGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));
					
					util::strncpy(pMsgEndWar->PeerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					pMsgEndWar->WarEndType = BATTLE_END_WIN;
					pPeerGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));
					
					///////////////////////////////////////////////////////////////////////////////
					// 2006-08-09 by cmkwon, 관리자/운영자에게도 정보를 전송한다.
					INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY, T_IC_GUILD_END_WAR_ADMIN_NOTIFY, pSEndWarNotify, SendBufNotify);
					pSEndWarNotify->WarEndType			= BATTLE_END_WIN;
					util::strncpy(pSEndWarNotify->WinerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					util::strncpy(pSEndWarNotify->LoserGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
					this->SendMessageToAllAdmin(SendBufNotify, MSG_SIZE(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY));

					pSeMsg->WinnerGuildUID	= pPeerGuild->m_GuildUniqueNumber;
					pSeMsg->LoserGuildUID	= pGuild->m_GuildUniqueNumber;
					pSeMsg->WarEndType		= BATTLE_END_WIN;
					pSeMsg->WarMapChannel	= pGuild->m_WarMapChannelIndex;
				}
			}
			break;
		case BATTLE_END_SURRENDER:
			{
				pGuild->WarResultLose();
				pPeerGuild->WarResultWin();

				util::strncpy(pMsgEndWar->PeerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				pMsgEndWar->WarEndType = BATTLE_END_DEFEAT;
				pGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));

				util::strncpy(pMsgEndWar->PeerGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				pMsgEndWar->WarEndType = BATTLE_END_WIN;
				pPeerGuild->SendMsgToGuildMembers(EndWarBuff, MSG_SIZE(MSG_IC_GUILD_END_WAR));

				///////////////////////////////////////////////////////////////////////////////
				// 2006-08-09 by cmkwon, 관리자/운영자에게도 정보를 전송한다.
				INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY, T_IC_GUILD_END_WAR_ADMIN_NOTIFY, pSEndWarNotify, SendBufNotify);
				pSEndWarNotify->WarEndType			= BATTLE_END_WIN;
				util::strncpy(pSEndWarNotify->WinerGuildName, pPeerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				util::strncpy(pSEndWarNotify->LoserGuildName, pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
				this->SendMessageToAllAdmin(SendBufNotify, MSG_SIZE(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY));

				pSeMsg->WinnerGuildUID	= pPeerGuild->m_GuildUniqueNumber;
				pSeMsg->LoserGuildUID	= pGuild->m_GuildUniqueNumber;
				pSeMsg->WarEndType		= BATTLE_END_WIN;
				pSeMsg->WarMapChannel	= pGuild->m_WarMapChannelIndex;
			}
			break;
		default:
			{
				this->m_MapGuild.unlock();

				char szSystemLog[512];
				sprintf(szSystemLog, "[ERROR] CIMIOCP::EndGuildWar WarEndType Error, WarEndType(%d) %12s vs %12s\r\n"
					, i_usWarEndType, pGuild->m_GuildName, pPeerGuild->m_GuildName);
				DBGOUT(szSystemLog);
				g_pGlobal->WriteSystemLog(szSystemLog);
				return;
			}
		}// switch_end						
	}// m_MapGuild.lock_end
	this->m_MapGuild.unlock();

	///////////////////////////////////////////////////////////////////////////////
	// 2004-12-09 by cmkwon, FieldServer로 알려줘야함
	this->SendMessageToFieldServer(FIBuff, MSG_SIZE(MSG_FI_GUILD_NOTIFY_END_WAR));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::GuildSetGuildFame(UID32_t i_guildUID, int i_nTotalFame, int i_nMonthlyFame)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-27 ~ 2005-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::GuildSetGuildFame(UID32_t i_guildUID, int i_nTotalFame, int i_nMonthlyFame)
{
	mt_auto_lock mtG(&m_MapGuild);

	CGuild *pGuild = this->m_MapGuild.findLock(i_guildUID);
	if(NULL == pGuild)
	{
		return;
	}

	// 2008-07-18 by dhjin, EP3
	if(pGuild->m_nTotalFame < i_nTotalFame)
	{
		pGuild->m_nTotalFame	= i_nTotalFame;
	}
	if(pGuild->m_nMonthlyFame < i_nMonthlyFame)
	{
		pGuild->m_nMonthlyFame	= i_nMonthlyFame;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::WarpGulid(char *i_szGuildName, MAP_CHANNEL_INDEX i_wapMapChann, D3DXVECTOR3 i_vWarpPosition)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::WarpGulid(char *i_szGuildName, MAP_CHANNEL_INDEX i_wapMapChann, D3DXVECTOR3 i_vWarpPosition)
{
	int nWarpedCnts = 0;
	mt_auto_lock mtAG(&m_MapGuild);

	CGuild *pGuild = this->GetGuildByName((const char*)i_szGuildName);
	if (NULL == pGuild)
	{
		return nWarpedCnts;
	}

// 2007-01-31 by cmkwon, 아래와 같이 수정함	
//	CIMIOCPSocket *arrMemberIISock[SIZE_MAX_INITIAL_GUILD_CAPACITY+COUNT_IN_MEMBERSHIP_ADDED_GUILD_CAPACITY];
//	util::zero(arrMemberIISock, sizeof(arrMemberIISock[0]));
//
//	int nOnlineMemberCnts = pGuild->GetGuildMemberIISocket(arrMemberIISock);
//	mtA.auto_unlock_cancel();
//
//	if(0 >= nOnlineMemberCnts)
//	{
//		return nWarpedCnts;
//	}
//
//	for(int i=0; i < nOnlineMemberCnts; i++)
//	{
//		if(arrMemberIISock[i]
//			&& arrMemberIISock[i]->IsValidCharacter())
//		{
//			INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_CALL_CHARACTER, T_FI_ADMIN_CALL_CHARACTER, pMsgFICall, pMsgFICallBuf);
//			pMsgFICall->CharacterUniqueNumber	= arrMemberIISock[i]->m_character.CharacterUniqueNumber;
//			pMsgFICall->MapChannelIndex			= i_wapMapChann;
//			pMsgFICall->Position				= i_vWarpPosition;
//
//			if (FALSE == arrMemberIISock[i]->SendMsgToCurrentFieldServerSocket(pMsgFICallBuf, MSG_SIZE(MSG_FI_ADMIN_CALL_CHARACTER)))
//			{
//				// current field server socket이 NULL이면 무시.
//				char	szSystemLog[1024];
//				sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
//					GetCharacterString(&arrMemberIISock[i]->m_character, string()), GetProtocolTypeString(T_IC_ADMIN_CALL_CHARACTER));
//				DBGOUT(szSystemLog);
//				g_pGlobal->WriteSystemLog(szSystemLog);
//				break;
//			}
//
//			nWarpedCnts++;
//		}
//	}
	
	vectCIMIOCPSocketPtr vectIISockList;
	vectIISockList.reserve(SIZE_MAX_GUILD_CAPACITY);			// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템

	int nOnlineMemberCnts = pGuild->GetGuildMemberIISocket(&vectIISockList);
	mtAG.auto_unlock_cancel();

	if(0 >= nOnlineMemberCnts)
	{
		return nWarpedCnts;
	}

	for(int i=0; i < nOnlineMemberCnts; i++)
	{
		CIMIOCPSocket *pIISock = vectIISockList[i];
		if(pIISock
			&& pIISock->IsValidCharacter())
		{
			INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_CALL_CHARACTER, T_FI_ADMIN_CALL_CHARACTER, pMsgFICall, pMsgFICallBuf);
			pMsgFICall->CharacterUniqueNumber	= pIISock->m_character.CharacterUniqueNumber;
			pMsgFICall->MapChannelIndex			= i_wapMapChann;
			pMsgFICall->Position				= i_vWarpPosition;

			if (FALSE == pIISock->SendMsgToCurrentFieldServerSocket(pMsgFICallBuf, MSG_SIZE(MSG_FI_ADMIN_CALL_CHARACTER)))
			{
				// current field server socket이 NULL이면 무시.
				char	szSystemLog[1024];
				sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
					GetCharacterString(&pIISock->m_character, string()), GetProtocolTypeString(T_IC_ADMIN_CALL_CHARACTER));
				DBGOUT(szSystemLog);
				g_pGlobal->WriteSystemLog(szSystemLog);
				break;
			}

			nWarpedCnts++;
		}
	}
	
	return nWarpedCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::IsGuildCommanderW(UID32_t i_guildUID, UID32_t i_charUID)
/// \brief		// 2007-10-06 by cmkwon, 여단장 체크
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::IsGuildCommanderW(UID32_t i_guildUID, UID32_t i_charUID)
{
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_guildUID)
		|| FALSE == IS_VALID_UNIQUE_NUMBER(i_charUID))
	{
		return FALSE;
	}
	
	mt_auto_lock mtA(&m_MapGuild);

	CGuild *ptmGuild = this->m_MapGuild.findLock(i_guildUID);
	if(NULL == ptmGuild)
	{
		return FALSE;
	}

	return ptmGuild->IsGuildCommander(i_charUID);
}

void CIMIOCP::SendMessageToAllClients(BYTE *buffer, int size)
{
	int nArrSize = this->GetArrayClientSize();
	for(int i = 0; i < nArrSize; i++)
	{
		CIMIOCPSocket *pSocket = GetIMIOCPSocket(i);
		if (pSocket
			&& pSocket->IsUsing()
			&& pSocket->m_PeerSocketType == ST_CLIENT_TYPE)
		{
			pSocket->SendAddData(buffer, size);
		}
	}
}

void CIMIOCP::SendChatMsgToAllClients(BitFlag16_t i_chatType, BYTE *i_pChat, int i_nSize, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	int nArrSize = this->GetArrayClientSize();
	for(int i = 0; i < nArrSize; i++)
	{
		CIMIOCPSocket *pSocket = this->GetIMIOCPSocket(i);
		if(pSocket
			&& pSocket->IsUsing()
			&& pSocket->m_PeerSocketType == ST_CLIENT_TYPE)
		{
			pSocket->SendChatMessage(i_chatType, i_pChat, i_nSize, i_byInfluenceMask);
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::SendMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-31 ~ 2007-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::SendMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen)
{
	int nSendedCnt = 0;
	int nCnt = i_pvectCIISockPtrList->size();
	for(int i=0; i < nCnt; i++)
	{
		CIMIOCPSocket *pIISock = (*i_pvectCIISockPtrList)[i];
		if(NULL == pIISock
			|| FALSE == pIISock->IsValidCharacter())
		{
			continue;
		}

		if(pIISock->SendAddData(i_pbyBuf, i_nLen))
		{
			nSendedCnt++;
		}
	}

	return nSendedCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::SendMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-31 ~ 2007-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::SendMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen)
{
	int nSendedCnt = 0;
	int nCnt = i_pvectClientIdxList->size();
	for(int i=0; i < nCnt; i++)
	{
		CIMIOCPSocket *pIISock = this->GetIMIOCPSocket((*i_pvectClientIdxList)[i]);
		if(NULL == pIISock
			|| FALSE == pIISock->IsValidCharacter())
		{
			continue;
		}

		if(pIISock->SendAddData(i_pbyBuf, i_nLen))
		{
			nSendedCnt++;
		}
	}

	return nSendedCnt;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::SendChatMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-31 ~ 2007-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::SendChatMsg2MultiUser(vectCIMIOCPSocketPtr *i_pvectCIISockPtrList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType)
{
	int nSendedCnt = 0;
	int nCnt = i_pvectCIISockPtrList->size();
	for(int i=0; i < nCnt; i++)
	{
		CIMIOCPSocket *pIISock = (*i_pvectCIISockPtrList)[i];
		if(NULL == pIISock
			|| FALSE == pIISock->IsValidCharacter())
		{
			continue;
		}

		if(pIISock->SendChatMessage(i_chatType, i_pbyBuf, i_nLen))
		{
			nSendedCnt++;
		}
	}

	return nSendedCnt;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::SendChatMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen), BitFlag16_t i_chatType
/// \brief		
/// \author		cmkwon
/// \date		2007-01-31 ~ 2007-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::SendChatMsg2MultiUser(vectClientIndex_t *i_pvectClientIdxList, BYTE *i_pbyBuf, int i_nLen, BitFlag16_t i_chatType)
{
	int nSendedCnt = 0;
	int nCnt = i_pvectClientIdxList->size();
	for(int i=0; i < nCnt; i++)
	{
		CIMIOCPSocket *pIISock = this->GetIMIOCPSocket((*i_pvectClientIdxList)[i]);
		if(NULL == pIISock
			|| FALSE == pIISock->IsValidCharacter())
		{
			continue;
		}

		if(pIISock->SendChatMessage(i_chatType, i_pbyBuf, i_nLen))
		{
			nSendedCnt++;
		}
	}

	return nSendedCnt;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::SendString128ToAllClients(BYTE i_byStr128Ty, char *i_szString, BOOL i_bWriteLog/*=FALSE*/, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::SendString128ToAllClients(BYTE i_byStr128Ty, char *i_szString, BOOL i_bWriteLog/*=FALSE*/, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	int nSentCntForRet = 0;
	int nArrSize = this->GetArrayClientSize();
	for(int i = 0; i < nArrSize; i++)
	{
		CIMIOCPSocket *pIISoc = this->GetIMIOCPSocket(i);
		if(pIISoc
			&& pIISoc->IsValidCharacter())
		{
			pIISoc->SendString128(i_byStr128Ty, i_szString);
			nSentCntForRet++;
		}
	}
	return nSentCntForRet;
}


///////////////////////////////////////////////////////////////////////////
// 편대관련
void CIMIOCP::InsertParty(CIMParty *i_pParty)
{
	m_MapParties.insertLock(i_pParty->m_PartyID, i_pParty);
	
	this->AdjustDeletedParties();								// 지워진 파티 정리
}

CIMParty *CIMIOCP::FindParty(PartyID_t i_partyID)
{
	return m_MapParties.findLock(i_partyID);
}

void CIMIOCP::DeleteParty(PartyID_t i_partyID)
{
	CIMParty *ptmParty = m_MapParties.popLock(i_partyID);
	if(ptmParty)
	{
		ptmParty->m_PartyDeletedTick = timeGetTime();
		m_mtvectorDeletedParties.pushBackLock(ptmParty);
	}

	this->AdjustDeletedParties();			// 지워진 파티 정리
}

void CIMIOCP::AdjustDeletedParties(void)
{
	if(m_mtvectorDeletedParties.empty()){			return;}

	m_mtvectorDeletedParties.lock();
	if(m_mtvectorDeletedParties.empty())
	{		
		m_mtvectorDeletedParties.unlock();
		return;
	}

	CIMParty *pParty;
	DWORD dwCurTick = timeGetTime();
	mt_vector<CIMParty*>::iterator it(m_mtvectorDeletedParties.begin());
	while (it != m_mtvectorDeletedParties.end())
	{
		pParty = *it;
		if(dwCurTick - pParty->m_PartyDeletedTick < 300000)
		{// 2006-07-04 by cmkwon, 지워진지 5분이 지나지 않은 것은 삭제하지 않는다, 뒤의 것들도 체크할 필요 없음

			break;
		}

		util::del(pParty);
		it = m_mtvectorDeletedParties.erase(it);
	}
	m_mtvectorDeletedParties.unlock();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::SetInflAllLeader(BYTE i_byInflTy, CHARACTER *o_m_character)
/// \brief		
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::SetInflAllLeader(CHARACTER *o_m_character)
{
// 2007-10-06 by cmkwon, 부지도자는 여단 가입 상태와 상관이 없음 - 여단 체크 필요 없음
//	// 2007-09-28 by cmkwon, 지도자,부지도자는 여단 가입 상태에만 유효하다
//	if(FALSE == IS_VALID_UNIQUE_NUMBER(o_m_character->GuildUniqueNumber))
//	{
//		return;
//	}

	if(CheckInfluenceLeader(o_m_character->InfluenceType, o_m_character->CharacterUniqueNumber))
	{
		o_m_character->Race	|= RACE_INFLUENCE_LEADER;
	}
	else if(CheckInfluenceSubLeader1(o_m_character->InfluenceType, o_m_character->CharacterUniqueNumber))
	{// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
		o_m_character->Race |= RACE_INFLUENCE_SUBLEADER_1;
	}
	else if(CheckInfluenceSubLeader2(o_m_character->InfluenceType, o_m_character->CharacterUniqueNumber))
	{// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
		o_m_character->Race |= RACE_INFLUENCE_SUBLEADER_2;
	}

// 2007-10-05 by cmkwon, 위와 같이 함수를 추가하여 수정함
//	if(COMPARE_INFLUENCE(o_m_character->InfluenceType, INFLUENCE_TYPE_VCN))
//	{
//		if(o_m_character->CharacterUniqueNumber == m_VCNLeaderCharacterUID)
//		{
//			o_m_character->Race	|= RACE_INFLUENCE_LEADER;
//		}
//		else if(o_m_character->CharacterUniqueNumber == m_VCNSub1LeaderCharacterUID
//				|| o_m_character->CharacterUniqueNumber == m_VCNSub2LeaderCharacterUID)
//		{
//			o_m_character->Race |= RACE_INFLUENCE_SUBLEADER;
//		}
//	}
//	else if(COMPARE_INFLUENCE(o_m_character->InfluenceType, INFLUENCE_TYPE_ANI))
//	{
//		if(o_m_character->CharacterUniqueNumber == m_ANILeaderCharacterUID)
//		{
//			o_m_character->Race	|= RACE_INFLUENCE_LEADER;
//		}
//		else if(o_m_character->CharacterUniqueNumber == m_ANISub1LeaderCharacterUID
//				|| o_m_character->CharacterUniqueNumber == m_ANISub2LeaderCharacterUID)
//		{
//			o_m_character->Race |= RACE_INFLUENCE_SUBLEADER;
//		}
//	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CIMIOCP::CallGMInsertRequest(UID32_t i_characterUID, BYTE i_byInflTy, char *i_szCharName, char *i_szCallDes)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 인자추가(, BYTE i_byInflTy)
/// \author		cmkwon
/// \date		2006-05-08 ~ 2006-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMIOCP::CallGMInsertRequest(UID32_t i_characterUID, BYTE i_byInflTy, char *i_szCharName, char *i_szCallDes)
{
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_characterUID)
		|| 0 >= strlen(i_szCharName)
		|| 0 >= strlen(i_szCallDes))
	{// 2006-05-08 by cmkwon, 파라미터 유효성 체크
		return 0;
	}

	mt_auto_lock mtA(&m_mtvectSCallGMList);
	int nCnt = m_mtvectSCallGMList.size(); 
	for(int i=0; i<nCnt; i++)
	{
		if(i_characterUID == m_mtvectSCallGMList[i].CallerCharacterUID)
		{// 2006-05-08 by cmkwon, 이전에 CallGM 신청 내역이 있다
			util::strncpy(m_mtvectSCallGMList[i].szCallGMDescription, i_szCallDes, SIZE_MAX_CHAT_MESSAGE);
			mtA.auto_unlock_cancel();

			this->CallGMOnRequest(nCnt);
			return nCnt;
		}
	}
	if(nCnt >= SIZE_MAX_CALLGM_LIST)
	{
		return 0;
	}

	SCALLGM	tmCallGM;
	util::zero(&tmCallGM, sizeof(tmCallGM));
	tmCallGM.CallerCharacterUID	= i_characterUID;
	tmCallGM.CallerInflTy		= i_byInflTy;		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 세력 설정
	util::strncpy(tmCallGM.szCallerCharacterName, i_szCharName, SIZE_MAX_CHARACTER_NAME);
	tmCallGM.atimeCallTime.SetCurrentDateTime();
	util::strncpy(tmCallGM.szCallGMDescription, i_szCallDes, SIZE_MAX_CHAT_MESSAGE);
	m_mtvectSCallGMList.push_back(tmCallGM);
	mtA.auto_unlock_cancel();

	this->CallGMOnRequest(nCnt+1);
	return nCnt+1;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldIOCP::CallGMOnRequest(int i_nCallGMRequestCount)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-08 ~ 2006-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::CallGMOnRequest(int i_nCallGMRequestCount)
{
	INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_CALLGM_INFO_OK, T_IC_ADMIN_CALLGM_INFO_OK, pSCallGMInfo, SendBuf);
	pSCallGMInfo->nCallGMTotalCount = i_nCallGMRequestCount;
	this->SendMessageToAllAdmin(SendBuf, MSG_SIZE(MSG_IC_ADMIN_CALLGM_INFO_OK));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CallGMMake_MSG_IC_ADMIN_CALLGM_VIEW_OK(MSG_IC_ADMIN_CALLGM_VIEW_OK *o_pCallGMViewOK, int i_nReqCount)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-08 ~ 2006-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CallGMMake_MSG_IC_ADMIN_CALLGM_VIEW_OK(MSG_IC_ADMIN_CALLGM_VIEW_OK *o_pCallGMViewOK, int i_nReqCount)
{
	if(0 >= i_nReqCount
		|| SIZE_MAX_CALLGM_BRING_COUNT < i_nReqCount)
	{
		return FALSE;
	}
	
	mt_auto_lock mtA(&m_mtvectSCallGMList);
	int nCnt = m_mtvectSCallGMList.size();
	if(0 >= nCnt)
	{
		return FALSE;
	}

	o_pCallGMViewOK->nCallGMCount	= 0;
	SCALLGM		*pCallGM			= (SCALLGM*)((BYTE*)o_pCallGMViewOK + sizeof(MSG_IC_ADMIN_CALLGM_VIEW_OK));
	for(int i=0; i < nCnt; i++)
	{
		*pCallGM = m_mtvectSCallGMList[i];
		
		o_pCallGMViewOK->nCallGMCount++;
		pCallGM++;

		if(i_nReqCount <= o_pCallGMViewOK->nCallGMCount)
		{
			break;
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CallGMMake_MSG_IC_ADMIN_CALLGM_BRING_OK(MSG_IC_ADMIN_CALLGM_BRING_OK *o_pCallGMBringOK, int i_nReqCount)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-08 ~ 2006-05-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CallGMMake_MSG_IC_ADMIN_CALLGM_BRING_OK(MSG_IC_ADMIN_CALLGM_BRING_OK *o_pCallGMBringOK, int i_nReqCount)
{
	if(0 >= i_nReqCount
		|| SIZE_MAX_CALLGM_BRING_COUNT < i_nReqCount)
	{
		return FALSE;
	}
	
	mt_auto_lock mtA(&m_mtvectSCallGMList);
	int nCnt = m_mtvectSCallGMList.size();
	if(0 >= nCnt)
	{
		return FALSE;
	}

	o_pCallGMBringOK->nCallGMCount	= 0;
	SCALLGM		*pCallGM			= (SCALLGM*)((BYTE*)o_pCallGMBringOK + sizeof(MSG_IC_ADMIN_CALLGM_BRING_OK));
	for(int i=0; i < nCnt; i++)
	{
		*pCallGM = m_mtvectSCallGMList[i];
		
		o_pCallGMBringOK->nCallGMCount++;
		pCallGM++;

		if(i_nReqCount <= o_pCallGMBringOK->nCallGMCount)
		{
			break;
		}
	}
	
	m_mtvectSCallGMList.erase(m_mtvectSCallGMList.begin(), m_mtvectSCallGMList.begin() + o_pCallGMBringOK->nCallGMCount);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CheckInfluenceLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CheckInfluenceLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
{
	UID32_t uidLeader = 0;
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		uidLeader = m_VCNLeaderCharacterUID;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		uidLeader = m_ANILeaderCharacterUID;
	}

	return uidLeader == i_uidCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CheckInfluenceSubLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CheckInfluenceSubLeader(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
{
	return CheckInfluenceSubLeader1(i_byInflTy, i_uidCharacterUID) || CheckInfluenceSubLeader2(i_byInflTy, i_uidCharacterUID);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CheckInfluenceSubLeader1(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CheckInfluenceSubLeader1(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
{
	UID32_t uidLeader = 0;
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		uidLeader = m_VCNSub1LeaderCharacterUID;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		uidLeader = m_ANISub1LeaderCharacterUID;
	}

	return uidLeader == i_uidCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CheckInfluenceSubLeader2(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
/// \brief		// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
/// \author		cmkwon
/// \date		2007-10-05 ~ 2007-10-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CheckInfluenceSubLeader2(BYTE i_byInflTy, UID32_t i_uidCharacterUID)
{
	UID32_t uidLeader = 0;
	if(INFLUENCE_TYPE_VCN == i_byInflTy)
	{
		uidLeader = m_VCNSub2LeaderCharacterUID;
	}
	else if(INFLUENCE_TYPE_ANI == i_byInflTy)
	{
		uidLeader = m_ANISub2LeaderCharacterUID;
	}

	return uidLeader == i_uidCharacterUID;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::IsOnCallGMSystem(void)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - CIMIOCP::IsOnCallGMSystem() 추가
/// \author		cmkwon
/// \date		2007-11-19 ~ 2007-11-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
bool CIMIOCP::IsOnCallGMSystem() const
{
	ATUM_DATE_TIME atCur { true };

	if (atCur < m_atCallGMStartTime || atCur >= m_atCallGMEndTime) return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::SetCallGMSystemTime(ATUM_DATE_TIME *i_patStartTime, ATUM_DATE_TIME *i_patEndTime)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - CIMIOCP::SetCallGMSystemTime() 추가
/// \author		cmkwon
/// \date		2007-11-19 ~ 2007-11-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::SetCallGMSystemTime(ATUM_DATE_TIME *i_patStartTime, ATUM_DATE_TIME *i_patEndTime)
{
	BOOL bBeforeFlag = this->IsOnCallGMSystem();

	m_atCallGMStartTime	= *i_patStartTime;
	m_atCallGMEndTime	= *i_patEndTime;

	m_bOnCallGMSystem	= this->IsOnCallGMSystem();

	g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] SetCallGMSystem OnFlag(%d) Time(%s ~ %s)\r\n"
		, m_bOnCallGMSystem, m_atCallGMStartTime.GetDateTimeString().GetBuffer()
		, m_atCallGMEndTime.GetDateTimeString().GetBuffer());

	if(bBeforeFlag == m_bOnCallGMSystem)
	{// 2007-11-20 by cmkwon, On/Off 가 변경되지 않은 상태일때에는 안내메시지 전송 필요 없다
		return;
	}

	if(FALSE == m_bOnCallGMSystem)
	{
		this->SendString128ToAllAdmin(STRING_128_ADMIN_CMD, STRMSG_071120_0003);
	}
	else
	{
		this->SendString128ToAllAdmin(STRING_128_ADMIN_CMD, STRMSG_071120_0002);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::OnDoMinutelyWorkIMIOCP(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - CIMIOCP::OnDoMinutelyWorkIMIOCP() 추가
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::OnDoMinutelyWorkIMIOCP(ATUM_DATE_TIME *pDateTime)
{
	///////////////////////////////////////////////////////////////////////////////	
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
	if(FALSE == m_bOnCallGMSystem)
	{
		m_bOnCallGMSystem = this->IsOnCallGMSystem();
		if(m_bOnCallGMSystem)
		{// 2007-11-20 by cmkwon, 진정 시스템이 켜지는 상황
			this->SendString128ToAllAdmin(STRING_128_ADMIN_CMD, STRMSG_071120_0002);
		}
	}
	else
	{
		m_bOnCallGMSystem = this->IsOnCallGMSystem();
		if(FALSE == m_bOnCallGMSystem)
		{// 2007-11-20 by cmkwon, 진정 시스템이 꺼지는 상황
			this->SendString128ToAllAdmin(STRING_128_ADMIN_CMD, STRMSG_071120_0003);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::OnDoSecondlyWorkIMIOCP(ATUM_DATE_TIME *pDateTime)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::OnDoSecondlyWorkIMIOCP(ATUM_DATE_TIME *pDateTime)
{
	//if(m_pANoticeManager)
	if(m_pANoticeManager && FALSE == m_bArenaServerCheck ) 	// 2013-04-03 by bckim, 운영자 자동공지 2번씩 나오는 버그 수정 
	{
		m_pANoticeManager->OnDoSecondlyWorkCANoticeManager(pDateTime);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::UpdateGuildCommander(UID32_t i_GuildUID, UID32_t i_Commander, UID32_t i_NewCommander)
/// \brief		EP3 - 여단 수정 사항 - 여단장 위임
/// \author		dhjin
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::UpdateGuildCommander(UID32_t i_GuildUID, UID32_t i_Commander, UID32_t i_NewCommander)
{
	mt_auto_lock mtA(&m_MapGuild);
	CGuild *pGuild = m_MapGuild.findNoLock(i_GuildUID);
	if (NULL == pGuild)
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] UpdateGuildCommander Error \r\n");
		return;	
	}
	
	CGuildMember * pCommader;
	CGuildMember * pNewComnmader;
	pGuild->GetGuildMember(i_Commander, pCommader);
	pGuild->GetGuildMember(i_NewCommander, pNewComnmader);

	if(NULL == pCommader 
		|| NULL == pNewComnmader)
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] UpdateGuildCommander Error \r\n");
		return;
	}

	// 2006-09-25 by dhjin, FieldServer로 길드 랭크 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_FI_GUILD_RANK_OK, msgRank,msgRankBuf);
	msgRank->MemberUniqueNumber	= i_Commander;
	msgRank->GuildRank			= GUILD_RANK_PRIVATE_NULL;
	this->SendMessageToFieldServer(msgRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));

	// MSG 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_IC_GUILD_SET_RANK_OK, msgSetRank, msgSetRankBuf);
	msgSetRank->MemberUniqueNumber	= i_Commander;
	msgSetRank->GuildRank			= GUILD_RANK_PRIVATE_NULL;
	pGuild->SendMsgToGuildMembers(msgSetRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));

	
	// 2006-09-25 by dhjin, FieldServer로 길드 랭크 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_FI_GUILD_RANK_OK, msgNewRank, msgNewRankBuf);
	msgNewRank->MemberUniqueNumber	= i_NewCommander;
	msgNewRank->GuildRank			= GUILD_RANK_COMMANDER;
	this->SendMessageToFieldServer(msgNewRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));

	// MSG 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_IC_GUILD_SET_RANK_OK, msgNewSetRank, msgNewSetRankBuf);
	msgNewSetRank->MemberUniqueNumber	= i_NewCommander;
	msgNewSetRank->GuildRank			= GUILD_RANK_COMMANDER;
	pGuild->SendMsgToGuildMembers(msgNewSetRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));

	pCommader->m_Rank		= GUILD_RANK_PRIVATE_NULL;
	pNewComnmader->m_Rank	= GUILD_RANK_COMMANDER;
	pGuild->m_CommanderUniqueNumber = i_NewCommander;

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCP::CheckGuildLoaded(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock)
/// \brief		EP3 - 여단 수정 사항 - 여단이 로딩되어 있는지 체크
/// \author		dhjin
/// \date		2008-05-30 ~ 2008-05-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCP::CheckGuildLoaded(CGuild *i_pGuild, CIMIOCPSocket *i_pIISock)
{
	mt_auto_lock mtA(&m_MapGuild);

	CGuild *pGuild = m_MapGuild.findNoLock(i_pGuild->m_GuildUniqueNumber);
	if (NULL == pGuild)
	{// 
		return FALSE;
	}
	
	CGuildMember *ptmGMember;
	if(FALSE == pGuild->GetGuildMember(i_pIISock->m_character.CharacterUniqueNumber, ptmGMember))
	{// 여단에 지금 접속한 캐릭이 속해 있지 않음
		
		g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] Guild loading Error, GuildUID(%8d) CharacterUID(%8d)\r\n"
			, pGuild->m_GuildUniqueNumber, i_pIISock->m_character.CharacterUniqueNumber);			
		
		m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteGuildUIDOfCharacter, i_pIISock, NULL, NULL, i_pIISock->m_character.CharacterUniqueNumber);
		i_pIISock->SetGuildUniqueNumber(INVALID_GUILD_UID);
		i_pIISock->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);
		return FALSE;
	}
	pGuild->SetOnlineGuildMember(i_pIISock, TRUE);

	if(0 != strncmp(pGuild->m_GuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME))
	{// 2007-08-01 by cmkwon, 여단명이 변경되었는지 체크

		///////////////////////////////////////////////////////////////////////////////
		// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 이미 로딩된 여단이 있을 경우 새로 로딩된 여단명으로 설정
		util::strncpy(pGuild->m_GuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);

		///////////////////////////////////////////////////////////////////////////////
		// 2007-08-01 by cmkwon, 변경된 여단명 모든 여단원에 전송
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK, T_IC_GUILD_CHANGE_GUILD_NAME_OK, msgChangeName, msgChangeNameBuf);
		util::strncpy(msgChangeName->NewGuildName, i_pGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
		pGuild->SendMsgToGuildMembers(msgChangeNameBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK));
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 새로 로딩된 여단마크로 설정
	pGuild->m_GuildMark		= i_pGuild->m_GuildMark;
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	if(pGuild->m_nMonthlyFameRank != i_pGuild->m_nMonthlyFameRank
		|| pGuild->m_nTotalFameRank	!= i_pGuild->m_nTotalFameRank
		)
	{
		pGuild->m_nMonthlyFameRank	= i_pGuild->m_nMonthlyFameRank;
		pGuild->m_nTotalFameRank	= i_pGuild->m_nTotalFameRank;
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_FAME_RANK, T_IC_GUILD_CHANGE_FAME_RANK, msg, SendBuf);
		msg->GuildMonthlyFameRank	= pGuild->m_nMonthlyFameRank;
		msg->GuildTotalFameRank		= pGuild->m_nTotalFameRank;
		pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_FAME_RANK));	
	}

	if(pGuild->m_bMemberShip != i_pGuild->m_bMemberShip
		|| pGuild->m_MemberShipExpireTime != i_pGuild->m_MemberShipExpireTime)
	{
		pGuild->m_bMemberShip			= i_pGuild->m_bMemberShip;
		pGuild->m_MemberShipExpireTime	= i_pGuild->m_MemberShipExpireTime;
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_MEMBERSHIP, T_IC_GUILD_CHANGE_MEMBERSHIP, msg, SendBuf);
		msg->MemberShip					= pGuild->m_bMemberShip;
		msg->MemberShipExpireTime		= pGuild->m_MemberShipExpireTime;
		pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_MEMBERSHIP));
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCP::SetGuildMemberShipW(UID32_t i_GuildUID, BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipDate /* = NULL*/)
/// \brief		EP3 - 여단 수정 사항 - 여단장 맴버쉽 정보 저장
/// \author		dhjin
/// \date		2008-06-20 ~ 2008-06-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCP::SetGuildMemberShipW(UID32_t i_GuildUID, BOOL i_bMemberShip, ATUM_DATE_TIME * i_pMemberShipDate /* = NULL*/)
{
	mt_auto_lock mtA(&m_MapGuild);
	CGuild *pGuild = m_MapGuild.findNoLock(i_GuildUID);
	if (NULL == pGuild)
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] SetGuildMemberShipW Error \r\n");
		return;	
	}	

	pGuild->SetGuildMemberShip(i_bMemberShip, i_pMemberShipDate);

	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_MEMBERSHIP, T_IC_GUILD_CHANGE_MEMBERSHIP, msg, SendBuf);
	msg->MemberShip					= pGuild->m_bMemberShip;
	msg->MemberShipExpireTime		= pGuild->m_MemberShipExpireTime;
	pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_MEMBERSHIP));	
}

//UINT CIMIOCP::GetRandInt32(UINT i_uiMin, UINT i_uiMax);
//{
//	//if(i_uiMin >= i_uiMax)
//	//{// 2007-12-10 by cmkwon, Min 이 Max 보다 크거나 같다면 Min 값을 리턴한다.
//	//	return i_uiMin;
//	//}
//	//
//	//mt_auto_lock mta(&m_lockRandManager);
//
//	//UINT nGap = i_uiMax - i_uiMin;
//	//return i_uiMin + m_mtRandManager.randInt(nGap);	
//}