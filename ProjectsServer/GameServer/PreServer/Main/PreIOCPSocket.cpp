#include "stdafx.h"
#include "PreIOCPSocket.h"
#include "AtumError.h"
#include "AccountBlockManager.h"
// 2012-06-07 by hskim, Administrator account security - leaving the log when you change account privileges
#include "AtumSJ.h"
#include <iostream>

constexpr char ARENA_SERVER_GROUP_NAME[] = "ARENA";

CPreIOCP* CPreIOCPSocket::ms_pPreIOCP = nullptr;

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_CONNECT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	util::strncpy(socket->m_szAdminAccountName, msg->UID, SIZE_MAX_ACCOUNT_NAME);

	auto acctype = g_pGlobal->AuthAdminToolUser(msg->UID, msg->PWD);

	if (!acctype)
	{
		server::log(TRUE, "[Notify] SCAdminTool access denied!! AccountName(%s) IP(%s) AccountType(0)\r\n", msg->UID, socket->GetPeerIP());
		return RES_ACCESS_DENIED;
	}

	server::log(TRUE, "[Notify] SCAdminTool connected!! AccountName(%s) IP(%s) AccountType(%d)\r\n", msg->UID, socket->GetPeerIP(), acctype);

	MessageData<T_PA_ADMIN_CONNECT_OK> packet;

	packet->AccountType0 = acctype;

	socket->SendAddData(packet);

	socket->m_PeerSocketType = ST_ADMIN_TOOL;

	ms_pPreIOCP->InsertMonitorIOCPSocketPtr(socket);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_GET_ACCOUNT_INFO>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	MessageData<T_PA_ADMIN_GET_ACCOUNT_INFO_OK> packet { };

	packet->IsBlocked = ms_pPreIOCP->m_pAccountBlockManager->Check(&packet->BlockedAccountInfo, msg->AccountName, nullptr, socket);

	CAccountInfo tmAccountInfo;
	if (!ms_pPreIOCP->GetAccountInfo(&tmAccountInfo, msg->AccountName))
	{
		packet->IsOnline = FALSE;
		util::strncpy(packet->AccountInfo.AccountName, msg->AccountName, SIZE_MAX_ACCOUNT_NAME);
	}
	else
	{
		packet->IsOnline = TRUE;
		packet->AccountInfo = tmAccountInfo;
	}

	socket->SendAddData(packet);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_BLOCK_ACCOUNT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	server::log(TRUE, "[Notify] Account Block: AdminIP(%15s) AdminAccountName(%20s), BlockedUserAccName(%20s) Period(%s ~ %s)\r\n"
		, socket->GetPeerIP(), socket->m_szAdminAccountName, msg->szBlockedAccountName
		, msg->atimeStartTime.GetDateTimeString().GetBuffer()
		, msg->atimeEndTime.GetDateTimeString().GetBuffer());

	ms_pPreIOCP->BlockAccount(msg, socket, TRUE);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_UNBLOCK_ACCOUNT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	server::log(TRUE, "[Notify] Account Block Cancellation: AdminIP(%15s) AdminAccountName(%20s), UserAccName(%20s)\r\n",
		socket->GetPeerIP(), socket->m_szAdminAccountName, msg->szBlockedAccountName);

	ms_pPreIOCP->UnblockAccount(msg, socket);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_GET_SERVER_INFO>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	ms_pPreIOCP->SendServerInfoForAdmin(socket);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_LOG_ACCOUNT_ACCOUNTTYPE_CHANGE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	char szAccountName[SIZE_MAX_ACCOUNT_NAME + 1] = { 0, };
	util::strncpy(szAccountName, msg->AccountName, SIZE_MAX_ACCOUNT_NAME);

	server::log(TRUE, "[Notify] Change Account AccountType : AdminIP(%s) AdminAccountName(%s), UserAccName(%s:%d) AccountType(%s => %s)\r\n",
		socket->GetPeerIP(), socket->m_szAdminAccountName, szAccountName, msg->AccountUniqueNumber,
		CAtumSJ::GetRaceString(msg->OldAccountType), CAtumSJ::GetRaceString(msg->NewAccountType));

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_LOG_CHARACTER_RACEACCOUNTTYPE_CHANGE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	char szAccountName[SIZE_MAX_ACCOUNT_NAME + 1] = { };
	char szCharacterName[SIZE_MAX_CHARACTER_NAME + 1] = { };

	util::strncpy(szAccountName, msg->AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(szCharacterName, msg->CharacterName, SIZE_MAX_CHARACTER_NAME);

	server::log(TRUE, "[Notify] Change Character RaceAccountType : AdminIP(%s) AdminAccountName(%s), UserAccName(%s:%d) UserCharName(%s:%d) AccountType(%s => %s)\r\n",
		socket->GetPeerIP(), socket->m_szAdminAccountName, szAccountName, msg->AccountUniqueNumber, szCharacterName, msg->CharacterUniqueNumber,
		CAtumSJ::GetRaceString(msg->OldAccountType), CAtumSJ::GetRaceString(msg->NewAccountType));

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_STRATRGYPOINT_INFO_CHANGE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	MessageData<T_FP_ADMIN_STRATRGYPOINT_INFO_CHANGE> packet { };

	util::strncpy(packet->DBName, msg->DBName, sizeof(packet->DBName));

	ms_pPreIOCP->SendMessageToAllFieldServer(packet);

	server::log(TRUE, "  [Notify] S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL #Recvd & FieldServer Send Packet - DBName(%s)\r\n", msg->DBName);

	ms_pPreIOCP->SendServerInfoForAdmin(socket); // HandlerT1<T_PA_ADMIN_GET_SERVER_INFO>

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_GET_ACCOUNT_LIST>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	MessageDataArray<T_PA_ADMIN_GET_ACCOUNT_LIST_OK> packet { };

	packet->NumOfAccounts = 0;
	
	auto pServerGroup = ms_pPreIOCP->GetServerGroup(msg->ServerName);

	pServerGroup->m_setLoginedAccount.lock();

	for (auto& acc : pServerGroup->m_setLoginedAccount)
	{
		auto accinfo = packet.append();
			
		if (!accinfo) break;

		util::strncpy(accinfo->AccountName, acc.c_str(), SIZE_MAX_ACCOUNT_NAME);
		util::strncpy(accinfo->CharacterName, "", SIZE_MAX_CHARACTER_NAME);
	}

	pServerGroup->m_setLoginedAccount.unlock();

	socket->SendAddData(packet);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_RELOAD_HAPPYEV>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	server::log(TRUE, "[Notify] Protocol Type(%s:0x%X) !!, AccountName(%s) IP(%s)\r\n",
		GetProtocolTypeString(T_PA_ADMIN_RELOAD_HAPPYEV), T_PA_ADMIN_RELOAD_HAPPYEV, socket->m_szAdminAccountName, socket->GetPeerIP());

	ms_pPreIOCP->SendMessageToAllFieldServer(T_FP_EVENT_RELOAD_HAPPYEV);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_RELOAD_ITEMEV>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	server::log(TRUE, "[Notify] Protocol Type(%s:0x%X) !!, AccountName(%s) IP(%s)\r\n",
		GetProtocolTypeString(T_PA_ADMIN_RELOAD_ITEMEV), T_PA_ADMIN_RELOAD_ITEMEV, socket->m_szAdminAccountName, socket->GetPeerIP());

	ms_pPreIOCP->SendMessageToAllFieldServer(T_FP_EVENT_RELOAD_ITEMEV);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_UPDATE_STRATEGYPOINT_NOTSUMMONTIME>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	ms_pPreIOCP->SendMessageToAllFieldServer(T_FP_EVENT_UPDATE_STRATEGYPOINT_NOTSUMMONTIM);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_PETITION_SET_PERIOD>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;
	
	MessageData<T_IP_ADMIN_PETITION_SET_PERIOD> packet;

	packet = msg;

	ms_pPreIOCP->SendMsgToAllIMServer(packet);

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_SET_DBSERVER_GROUP>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	{
		auto pServG = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);

		if (!pServG || !pServG->m_FieldServerInfo.pSocket || !pServG->m_FieldServerInfo.pSocket->IsUsing() || !pServG->m_FieldServerInfo.IsActive)
		{
			server::log(TRUE, "[Notify] CPreIOCPSocket::HandlerT1<T_PA_ADMIN_SET_DBSERVER_GROUP>::handler# invalid FieldServer !!, %s, %d %d \r\n",
				msg->ServerGroupName, msg->LimitUserCount, msg->LockCreateCharacterForNewAccount);

			return RES_BREAK;
		}

		pServG->m_LimitGroupUserCounts = msg->LimitUserCount;
		pServG->m_bLockCreateCharacterForNewAccount = msg->LockCreateCharacterForNewAccount;

		MessageData<T_FP_CONNECT_UPDATE_DBSERVER_GROUP> packet;
		
		util::strncpy(packet->DBServerGroup.ServerGroupName, pServG->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
		packet->DBServerGroup.LimitUserCount = pServG->m_LimitGroupUserCounts;
		packet->DBServerGroup.LockCreateCharacterForNewAccount = pServG->m_bLockCreateCharacterForNewAccount;
		pServG->SendMessageToFieldServer(packet);
	}

	{
		MessageData<T_PA_ADMIN_SET_DBSERVER_GROUP_OK> packet;
		packet = msg;
		socket->SendAddData(packet);
	}

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	ms_pPreIOCP->SendMsgToAllIMServer(T_IP_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM);
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_RELOAD_WORLDRANKING>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	ms_pPreIOCP->SendMessageToAllFieldServer(T_FP_ADMIN_RELOAD_WORLDRANKING);
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PA_ADMIN_RELOAD_INFLUENCERATE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;
		
	auto pServG = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);
	
	if (!pServG || !pServG->m_FieldServerInfo.pSocket || !pServG->m_FieldServerInfo.pSocket->IsUsing() || !pServG->m_FieldServerInfo.IsActive)
	{
		server::log(TRUE, "[Notify] CPreIOCPSocket::HandlerT1_PA_ADMIN_RELOAD_INFLUENCERATE# invalid FieldServer !!, %s \r\n", msg->ServerGroupName);
		return RES_BREAK;
	}

	pServG->SendMessageToFieldServer(T_FP_ADMIN_RELOAD_INFLUENCERATE);
	return RES_RETURN_TRUE;
}


template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_ALIVE>::handler(CPreIOCPSocket* socket, const char*, int, int &)
{
	socket->OnRecvdAlivePacket();
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg)
	{
		socket->SendErrorMessage(T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK, ERR_PROTOCOL_INVALID_FIELD_DATA);

		socket->Close(0x11002, TRUE);

		return RES_RETURN_FALSE;
	}

	if (socket->m_CPClientState != CP_CONNECTED)
	{
		socket->SendErrorMessage(T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE);

		return RES_BREAK;
	}

	// todo : this is the reason why ignore updates does not work

#ifdef _DEBUG
	if (g_pPreGlobal->m_bIgnoreClientVersionUpdate)
	{
		socket->SendAddMessageType(T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK_OK);
		socket->SetClientState(CP_SINGLE_FILE_VERSIONCHECKED, NULL);
		socket->m_PeerSocketType = ST_CLIENT_TYPE;
		return RES_RETURN_TRUE;
	}
#endif

	auto bDownloadDeleteFileList = ms_pPreIOCP->m_LatestDeleteFileListVersion != msg->DeleteFileListVersion;
	auto bDownloadNotice = ms_pPreIOCP->m_LatestNoticeVersion != msg->NoticeVersion;

	if (bDownloadDeleteFileList || bDownloadNotice)
	{
		MessageData<T_PC_CONNECT_SINGLE_FILE_UPDATE_INFO> packet;

		ms_pPreIOCP->m_LatestDeleteFileListVersion.GetVersion(packet->NewDeleteFileListVersion);
		ms_pPreIOCP->m_LatestNoticeVersion.GetVersion(packet->NewNoticeVersion);

		util::strncpy(packet->FtpIP, g_pPreGlobal->GetDownloadServerIP(), SIZE_MAX_FTP_URL);
		packet->FtpPort = g_pPreGlobal->GetDownloadServerPort();

		util::strncpy(packet->DeleteFileListDownloadPath, g_pPreGlobal->GetDeleteFileListDownloadPath(), SIZE_MAX_FTP_FILE_PATH);
		util::strncpy(packet->NoticeFileDownloadPath, g_pPreGlobal->GetNoticeFileDownloadPath(), SIZE_MAX_FTP_FILE_PATH);

		socket->SendAddData(packet);
	}
	else socket->SendAddMessageType(T_PC_CONNECT_SINGLE_FILE_VERSION_CHECK_OK);

	socket->m_CPClientState = CP_SINGLE_FILE_VERSIONCHECKED;
	socket->m_PeerSocketType = ST_CLIENT_TYPE;

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_VERSION>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto pMsgRecvVersion = GetMessageData(pPacket, nLength, nBytesUsed);

	if(!pMsgRecvVersion)
	{
		socket->SendErrorMessage(T_PC_CONNECT_VERSION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		socket->Close(0x11002, TRUE);
		return RES_RETURN_FALSE;
	}

#ifdef _DEBUG
	if (g_pPreGlobal->m_bIgnoreClientVersionUpdate)
	{
		SendAddMessageType(T_PC_CONNECT_VERSION_OK);
		SetClientState(CP_VERSIONCHECKED, NULL);
		return RES_RETURN_TRUE;
	}
#endif

	VersionInfo CurrentClientVersion { pMsgRecvVersion->ClientVersion };

	if (socket->m_CPClientState != CP_SINGLE_FILE_VERSIONCHECKED)
	{
		socket->SendErrorMessage(T_PC_CONNECT_VERSION, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE);

		return RES_BREAK;
	}

	if (g_pPreGlobal->GetClientReinstallVersion() >= CurrentClientVersion || ms_pPreIOCP->m_LatestClientVersion < CurrentClientVersion)
	{
		MessageData<T_PC_CONNECT_REINSTALL_CLIENT> packet;

		// send REINSTALL_CLIENT
		ms_pPreIOCP->m_LatestClientVersion.GetVersion(packet->LatestVersion);

		socket->SendAddData(packet);

		return RES_BREAK;
	}

	if (ms_pPreIOCP->m_LatestClientVersion == CurrentClientVersion)
	{
		// send VERSION_OK
		socket->SendAddMessageType(T_PC_CONNECT_VERSION_OK);

		socket->m_CPClientState = CP_VERSIONCHECKED;
	}
	else // (ms_pPreIOCP->m_LatestClientVersion > CurrentClientVersion)
	{

		// set updateinfo
		VersionInfo tmNewVersion;
		util::zero(&tmNewVersion, sizeof(VersionInfo));

		if (!ms_pPreIOCP->GetNextUpdateVersion(&tmNewVersion, &CurrentClientVersion))
		{
			socket->SendErrorMessage(T_PC_CONNECT_VERSION, ERR_COMMON_INVALID_CLIENT_VERSION, 0, 0, CurrentClientVersion.GetVersionString(), TRUE);
			return RES_BREAK;
		}

		MessageData<T_PC_CONNECT_UPDATE_INFO> packet;

		// send UPDATE_INFO
		CurrentClientVersion.GetVersion(packet->OldVersion);
		tmNewVersion.GetVersion(packet->UpdateVersion);
		util::strncpy(packet->FtpIP, g_pPreGlobal->GetDownloadServerIP(), SIZE_MAX_FTP_URL);
		packet->FtpPort = g_pPreGlobal->GetDownloadServerPort();
		util::strncpy(packet->FtpUpdateDownloadDir, g_pPreGlobal->GetClientUpdateDownloadDir(), SIZE_MAX_FTP_FILE_PATH);

		socket->SendAddData(packet);
	}

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_LOGIN>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg)
	{
		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_FIELD_DATA);
		socket->Close(0x11005, TRUE);
		return RES_RETURN_FALSE;
	}

	if (socket->m_CPClientState != CP_VERSIONCHECKED)
	{
		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE, socket->m_CPClientState);
		return RES_BREAK;
	}

	if (msg->AccountName[0] == '\0')
	{
		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_EMPTY_ACCOUNTNAME);
		return RES_BREAK;
	}

	auto pSGroup = ms_pPreIOCP->GetServerGroup(msg->FieldServerGroupName);

	if (pSGroup == nullptr)
	{
		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_SERVER_GROUP_NAME);
		return RES_BREAK;
	}

	if (pSGroup->GetUserCount() >= pSGroup->m_LimitGroupUserCounts && !socket->IsWhitelisted())
	{
		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_LIMIT_GROUP_USER_COUNT);
		return RES_BREAK;
	}

	auto* pRecvMsg = new MSG_PC_CONNECT_LOGIN { *msg };

	_strlwr(pRecvMsg->AccountName);
	util::strncpy(pRecvMsg->ClientIP, socket->GetPeerIP(), SIZE_MAX_IPADDRESS);

	SBLOCKED_ACCOUNT_INFO tmBlockedInfo;

	if (ms_pPreIOCP->m_pAccountBlockManager->Check(&tmBlockedInfo, pRecvMsg->AccountName, pRecvMsg->MACAddress))
	{
		if (tmBlockedInfo.IsMACBlocked) socket->SendAddMessageType(T_PC_CONNECT_MAC_BLOCKED);
		else
		{
			MessageData<T_PC_CONNECT_LOGIN_BLOCKED> packet;

			// Account Block
			util::strncpy(packet->szAccountName, tmBlockedInfo.szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);		// 2007-01-10 by cmkwon
			packet->nBlockedType = tmBlockedInfo.enBlockedType;
			packet->atimeStart = tmBlockedInfo.atimeStartTime;
			packet->atimeEnd = tmBlockedInfo.atimeEndTime;
			util::strncpy(packet->szBlockedReasonForUser, tmBlockedInfo.szBlockedReasonForUser, SIZE_MAX_BLOCKED_ACCOUNT_REASON);		// 2007-01-10 by cmkwon
			
			socket->SendAddData(packet);
		}

		delete pRecvMsg;
		return RES_BREAK;
	}

	// DevX tolerates direct login only
	if (pRecvMsg->LoginType != LOGIN_TYPE_DIRECT)
	{
		delete pRecvMsg;

		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_FIELD_DATA);

		socket->Close(0x11005, TRUE);

		return RES_RETURN_FALSE;
	}

	ms_pPreIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_PRE_Login, socket, pRecvMsg);

	util::strncpy(socket->m_szAdminAccountName, pRecvMsg->AccountName, SIZE_MAX_ACCOUNT_NAME);

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_GET_SERVER_GROUP_LIST>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	MessageDataArray<T_PC_CONNECT_GET_SERVER_GROUP_LIST_OK> packet { };

	packet->NumOfServerGroup = 0;

	for (auto& group : ms_pPreIOCP->m_AllServerGroupVector)
	{

		if (group.m_bEnableServerGroup && !group.m_bForbidViewServer)	// 2007-12-22 by dhjin, 아레나 통합 - 런쳐 리스트에서 보이면 안되는 서버 체크, TRUE = 보이지 않는다.	
		{
			auto info = packet.append();

			util::strncpy(info->ServerGroupName, group.m_ServerGroupName, SIZE_MAX_SERVER_NAME);

			if (!group.m_IMServerInfo.IsActive || !group.m_FieldServerInfo.IsActive) info->Crowdedness = 0;
			else
			{
				int nCrowdedness = (group.GetUserCount() * 100) / group.m_LimitGroupUserCounts;
				info->Crowdedness = max(1, nCrowdedness);
			}
		}
	}

	socket->SendAddData(packet);

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	//// 2007-10-19 by cmkwon, AllowedIP 시스템 변경 - 허가 IP 체크 함수
	//if (!socket->IsWhitelisted())
	//{
	//	socket->SendErrorMessage(T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST, ERR_NOT_ALLOWED_IP, 0, 0, socket->GetPeerIP());
	//	socket->Close();
	//	return RES_BREAK;
	//}
	MessageData<T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK> packet;

	if (!g_pPreGlobal->Make_MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST(packet.data()))
	{
		return RES_BREAK;
	}
	socket->SendAddData(packet);
	
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	//// 2007-10-19 by cmkwon, AllowedIP 시스템 변경 - 허가 IP 체크 함수
	//if (!socket->IsWhitelisted())
	//{
	//	socket->SendErrorMessage(T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST, ERR_NOT_ALLOWED_IP, 0, 0, socket->GetPeerIP());
	//	socket->Close();
	//	return RES_BREAK;		
	//}

	MessageData<T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK> packet { };
	
	if (!g_pPreGlobal->Make_MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK(packet.data())) return RES_BREAK;

	socket->SendAddData(packet);
	
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_CONNECT_NETWORK_CHECK>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	MessageData<T_PC_CONNECT_NETWORK_CHECK_OK> packet { };
	packet->nCheckCount	= msg->nCheckCount;
	socket->SendAddData(packet);

	return RES_RETURN_TRUE;
}


template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		socket->SendErrorMessage(T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		socket->Close(0x11003, TRUE);
		return RES_RETURN_FALSE;
	}

	VersionInfo				CurrentClientVersion;

	// type conversion for temporary use
	CurrentClientVersion.SetVersion(msg->Version[0], msg->Version[1]
		, msg->Version[2], msg->Version[3]);

	if ( ms_pPreIOCP->m_LatestLauncherVersion == CurrentClientVersion)
	{
		// send VERSION_OK
		socket->SendAddMessageType(T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION_OK);
	}
	else
	{// Launcher 버젼이 다르면 무조건 Update한다
		// send UPDATE_INFO
		MessageData<T_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO> packet { };

		ms_pPreIOCP->m_LatestLauncherVersion.GetVersion(packet->UpdateVersion);
		util::strncpy(packet->FtpIP, g_pPreGlobal->GetRandomDownloadHttpIP(), SIZE_MAX_FTP_URL);
		packet->FtpPort = g_pPreGlobal->GetDownloadServerPort();
		util::strncpy(packet->LauncherFileDownloadPath, g_pPreGlobal->m_szLauncherFileDownloadPath, SIZE_MAX_FTP_FILE_PATH);

		socket->SendAddData(packet);
	}
	socket->m_PeerSocketType = ST_CLIENT_TYPE;
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if(!msg)
	{
		socket->SendErrorMessage(T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_VERSION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		socket->Close(0x11003, TRUE);
		return RES_RETURN_FALSE;
	}

	VersionInfo				CurrentClientVersion;

	///////////////////////////////////////////////////////////////////////////////
	// 
	socket->m_PeerSocketType = ST_CLIENT_TYPE;		// 2007-01-08 by cmkwon

	///////////////////////////////////////////////////////////////////////////////	
	// type conversion for temporary use
	CurrentClientVersion.SetVersion(msg->Version);

	if ( ms_pPreIOCP->m_LatestLauncherVersion == CurrentClientVersion)
	{
		// send VERSION_OK
		socket->SendAddMessageType(T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION_OK);
		return RES_RETURN_TRUE;
	}

	MessageData<T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO> packet;

	ms_pPreIOCP->m_LatestLauncherVersion.GetVersion(packet->UpdateVersion);
	util::strncpy(packet->UpdateServerIP, g_pPreGlobal->GetDownloadServerIP(), SIZE_MAX_FTP_URL);
	packet->UpdateServerPort		= g_pPreGlobal->GetDownloadServerPort();
	util::strncpy(packet->LauncherFileDownloadPath, g_pPreGlobal->GetLauncherFileDownloadPath(), SIZE_MAX_FTP_FILE_PATH);

	socket->SendAddData(packet);

	return RES_RETURN_TRUE;
}


template<> ProcessResult CPreIOCPSocket::HandlerT1<T_IP_CONNECT_ALIVE>::handler(CPreIOCPSocket* socket, const char*, int, int &)
{
	socket->OnRecvdAlivePacket();
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_IP_CONNECT_IM_CONNECT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{	
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	auto pServerGroup = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);
	
	// Check whether the server group is valid

	if (!pServerGroup)
	{
		socket->SendErrorMessage(T_IP_CONNECT_IM_CONNECT, ERR_PROTOCOL_NO_SUCH_SERVER_GROUP, 0, 0, msg->ServerGroupName);
		socket->Close(0x11008, TRUE);
		return RES_RETURN_FALSE;
	}

	// Check whether an IM server is already connected

	if (pServerGroup->m_IMServerInfo.IsActive && pServerGroup->m_IMServerInfo.pSocket && pServerGroup->m_IMServerInfo.pSocket->IsUsing())
	{
		socket->SendErrorMessage(T_IP_CONNECT_IM_CONNECT, ERR_PROTOCOL_IMSERVER_ALREADY_CONNECTED);
		socket->Close(0x1100A, TRUE);
		return RES_RETURN_FALSE;
	}

	// Everything is OK... save the information

	util::strncpy(socket->m_szConnectedServerGroupName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
	socket->m_PeerSocketType = ST_IM_SERVER;
	
	pServerGroup->m_IMServerInfo.serverID	= msg->IMServerID;
	pServerGroup->m_IMServerInfo.IsActive	= true;
	pServerGroup->m_IMServerInfo.pSocket	= socket;
	pServerGroup->m_IMServerInfo.ServerType	= ST_IM_SERVER;
	
	server::log(true, "ServerGroupName(%s) IMServer(%s), PeerIPPort[%s:%d] registeration done...\r\n",
		pServerGroup->m_ServerGroupName, pServerGroup->m_IMServerInfo.serverID.IP, socket->GetPeerIP(), socket->GetPeerPort());

	// Respond to IM server

	socket->SendAddMessageType(T_IP_CONNECT_IM_CONNECT_OK);

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_IP_GET_SERVER_GROUP_INFO_ACK>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	auto pServerGroup = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);
	
	if (!pServerGroup)
	{
		socket->SendErrorMessage(T_IP_GET_SERVER_GROUP_INFO_ACK, ERR_PROTOCOL_NO_SUCH_SERVER_GROUP);
		return RES_BREAK;
	}

	pServerGroup->m_MaxGroupUserCounts			= msg->IMMaxUserCounts;

	BYTE	SendBuf[SIZE_MAX_PACKET];
	INIT_MSG(MSG_PM_GET_SERVER_GROUP_INFO_ACK, T_PM_GET_SERVER_GROUP_INFO_ACK, pSendGroupInfo, SendBuf);
	util::strncpy(pSendGroupInfo->ServerGroupName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
	pSendGroupInfo->IMServerID					= pServerGroup->m_IMServerInfo.serverID;
	pSendGroupInfo->AccumulatedGroupUserCounts	= pServerGroup->m_AccumulatedGroupUserCounts;
	pSendGroupInfo->CurrentGroupUserCounts		= msg->IMCurrentUserCounts;
	pSendGroupInfo->MaxGroupUserCounts			= msg->IMMaxUserCounts;
	pSendGroupInfo->bEnableServerGroup			= pServerGroup->m_bEnableServerGroup;
	pSendGroupInfo->LimitGroupUserCounts		= pServerGroup->m_LimitGroupUserCounts;
	ms_pPreIOCP->SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_PM_GET_SERVER_GROUP_INFO_ACK));
	return RES_RETURN_TRUE;
}


template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_CONNECT_ALIVE>::handler(CPreIOCPSocket* socket, const char*, int, int &)
{
	socket->OnRecvdAlivePacket();
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_CONNECT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	server::log(true, "[Notify] SCMonitor connected !!, IP(%s)\r\n", socket->GetPeerIP());

	socket->m_PeerSocketType = ST_MONITOR_SERVER;
	ms_pPreIOCP->InsertMonitorIOCPSocketPtr(socket);


	MessageData<T_PM_CONNECT_OK> packet;

	util::strncpy(packet->IPAddress, g_pPreGlobal->GetPublicIPLocal(), SIZE_MAX_IPADDRESS);
	packet->NumOfClients = ms_pPreIOCP->m_AccountInfoMap.size();
	packet->Port = ms_pPreIOCP->m_nListenerPort;
	packet->CalcBandwidth = ms_pPreIOCP->GetFlagCalcTrafficInfo();
	packet->StartedTime = ms_pPreIOCP->m_dwTimeStarted;
	util::strncpy(packet->ServerName, "Pre Server", SIZE_MAX_SERVER_NAME);

	socket->SendAddData(packet);

	MessageData<T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING> pSHttpMsg;

	util::strncpy(pSHttpMsg->szDownloadHttpServerIP, g_pPreGlobal->GetRandomDownloadHttpIP(), SIZE_MAX_FTP_URL);
	pSHttpMsg->nDownloadHttpServerPort = g_pPreGlobal->m_nDownloadHttpServerPort;
	util::strncpy(pSHttpMsg->szClientHttpUpdateDownloadDir, g_pPreGlobal->m_szClientHttpUpdateDownloadDir, SIZE_MAX_FTP_FILE_PATH);
	util::strncpy(pSHttpMsg->szLauncherFileDownloadHttpPath, g_pPreGlobal->m_szLauncherFileDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);
	util::strncpy(pSHttpMsg->szDeleteFileListDownloadHttpPath, g_pPreGlobal->m_szDeleteFileListDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);
	util::strncpy(pSHttpMsg->szNoticeFileDownloadHttpPath, g_pPreGlobal->m_szNoticeFileDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);
	util::strncpy(pSHttpMsg->szNoticeFileDownloadHttpPath, g_pPreGlobal->m_szNoticeFileDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);
	util::strncpy(pSHttpMsg->szVersionListFileDownloadHttpPath, g_pPreGlobal->m_szVersionListFileDownloadHttpPath, SIZE_MAX_FTP_FILE_PATH);
	
	socket->SendAddData(pSHttpMsg);


	for (const auto& i : ms_pPreIOCP->m_AllServerGroupVector)
	{
		MessageData<T_PM_GET_SERVER_GROUP_INFO_ACK> pSendGroupInfo;

		util::strncpy(pSendGroupInfo->ServerGroupName, i.m_ServerGroupName, SIZE_MAX_SERVER_NAME);
		pSendGroupInfo->IMServerID = i.m_IMServerInfo.serverID;
		pSendGroupInfo->AccumulatedGroupUserCounts = i.m_AccumulatedGroupUserCounts;
		pSendGroupInfo->CurrentGroupUserCounts = i.GetUserCount();
		pSendGroupInfo->MaxGroupUserCounts = i.m_MaxGroupUserCounts;
		pSendGroupInfo->bEnableServerGroup = i.m_bEnableServerGroup;
		pSendGroupInfo->LimitGroupUserCounts = i.m_LimitGroupUserCounts;

		socket->SendAddData(pSendGroupInfo);
	}

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_PREPARE_SHUTDOWN>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	return RES_RETURN_TRUE; // 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 

	// DevX might use them
	//DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_PM_PREPARE_SHUTDOWN
	//	, MSG_PM_PREPARE_SHUTDOWN, pRMsg);

	//CServerGroup *pServerGroup = ms_pPreIOCP->GetServerGroup(pRMsg->ServerGroupName);
	//if (pServerGroup == NULL)
	//{
	//	SendErrorMessage(T_PM_PREPARE_SHUTDOWN, ERR_PROTOCOL_NO_SUCH_SERVER_GROUP);
	//	return RES_BREAK;
	//}

	//MessageType_t msgTy = T_FP_CONNECT_PREPARE_SHUTDOWN;
	//pServerGroup->SendMessageToFieldServer((BYTE*)&msgTy, SIZE_FIELD_TYPE_HEADER);
	//return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_RELOAD_VERSION_INFO_HEADER>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	mt_auto_lock mtTV(&(ms_pPreIOCP->m_tmmtmapVersionOld2New));

	ms_pPreIOCP->m_tmmtmapVersionOld2New.clear();
	ms_pPreIOCP->m_tmLatestLauncherVersion.SetVersion(msg->LauncherVersion);
	ms_pPreIOCP->m_tmLatestDeleteFileListVersion.SetVersion(msg->DeleteFileListVersion);
	ms_pPreIOCP->m_tmLatestNoticeVersion.SetVersion(msg->NoticeVersion);
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_RELOAD_VERSION_INFO_LIST>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	// todo : improve this kind of handlers

	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	if(nLength-nBytesUsed < msg->NumOfClientVersions * (sizeof(USHORT)*8))
	{
		socket->SendErrorMessage(T_PM_RELOAD_VERSION_INFO_LIST, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	nBytesUsed		+= msg->NumOfClientVersions * (sizeof(USHORT)*8);


	mt_auto_lock mtTV(&(ms_pPreIOCP->m_tmmtmapVersionOld2New));

	for (int i = 0; i < msg->NumOfClientVersions; i++)
	{
		USHORT *pOldVersionArray = (USHORT*)(((char*)msg) + sizeof(MSG_PM_RELOAD_VERSION_INFO_LIST) + i*8*sizeof(USHORT));
		USHORT *pNewVersionArray = (USHORT*)(((char*)msg) + sizeof(MSG_PM_RELOAD_VERSION_INFO_LIST) + i*8*sizeof(USHORT) + 4*sizeof(USHORT));

		VersionInfo OldVersion(pOldVersionArray);
		VersionInfo NewVersion(pNewVersionArray);

		ms_pPreIOCP->m_tmmtmapVersionOld2New.insertLock(OldVersion, NewVersion);
	}

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_RELOAD_VERSION_INFO_DONE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	ms_pPreIOCP->m_bPauseService = TRUE;
	
	mt_auto_lock mtV(&ms_pPreIOCP->m_mtmapVersionOld2New);
	mt_auto_lock mtTV(&ms_pPreIOCP->m_tmmtmapVersionOld2New);

	ms_pPreIOCP->m_mtmapVersionOld2New.clear();
	ms_pPreIOCP->m_LatestLauncherVersion		= ms_pPreIOCP->m_tmLatestLauncherVersion;
	ms_pPreIOCP->m_LatestDeleteFileListVersion	= ms_pPreIOCP->m_tmLatestDeleteFileListVersion;
	ms_pPreIOCP->m_LatestNoticeVersion			= ms_pPreIOCP->m_tmLatestNoticeVersion;

	ms_pPreIOCP->m_mtmapVersionOld2New = move(ms_pPreIOCP->m_tmmtmapVersionOld2New);

	//mtmapVersionInfo::iterator itr = ms_pPreIOCP->m_tmmtmapVersionOld2New.begin();
	//for(; itr != ms_pPreIOCP->m_tmmtmapVersionOld2New.end(); itr++)
	//{
	//	ms_pPreIOCP->m_mtmapVersionOld2New.insertLock(itr->first, itr->second);
	//}

	if (!ms_pPreIOCP->m_mtmapVersionOld2New.empty())
		
		ms_pPreIOCP->m_LatestClientVersion = ms_pPreIOCP->m_mtmapVersionOld2New.rbegin()->second;

	
	mtTV.auto_unlock_cancel();
	mtV.auto_unlock_cancel();
	
	ms_pPreIOCP->WriteVersionInfoToLocalFile();

	ms_pPreIOCP->m_bPauseService = false;

	socket->SendAddMessageType(T_PM_RELOAD_VERSION_INFO_OK);

	ms_pPreIOCP->SendMessageToAllFieldServer(T_FP_MONITOR_RELOAD_VERSION_INFO_OK);

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_GET_NUM_CLIENTS>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	BYTE SendBuf[SIZE_MAX_PACKET];

	MSG_PM_GET_NUM_CLIENTS_OK	*pSendPMGetNumClientsOK = NULL;

	*(MessageType_t*)SendBuf = T_PM_GET_NUM_CLIENTS_OK;
	pSendPMGetNumClientsOK = (MSG_PM_GET_NUM_CLIENTS_OK*)(SendBuf + SIZE_FIELD_TYPE_HEADER);
	pSendPMGetNumClientsOK->NumOfClients = ms_pPreIOCP->m_AccountInfoMap.size();

	socket->SendAddData(SendBuf, MSG_SIZE(MSG_PM_GET_NUM_CLIENTS_OK));

	//ms_pPreIOCP->DBG_PrintAccountMap();
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_SHUTDOWN>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	BYTE SendBuf[SIZE_MAX_PACKET];

	*(MessageType_t*)SendBuf = T_PM_SHUTDOWN_OK;
	socket->SendAddData(SendBuf, SIZE_FIELD_TYPE_HEADER);
	// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 -  프리서버 명령
	// PostMessage(g_pGlobal->GetMainWndHandle(), WM_CLOSE, 0, 0);
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_PING>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	BYTE SendBuf[SIZE_MAX_PACKET];

	*(MessageType_t*)SendBuf = T_PM_PING_OK;
	socket->SendAddData(SendBuf, SIZE_FIELD_TYPE_HEADER);
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_RELOAD_VERSION_INFO>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	ms_pPreIOCP->m_bPauseService = TRUE;

	// delete version list
	ms_pPreIOCP->m_mtmapVersionOld2New.lock();
	ms_pPreIOCP->m_mtmapVersionOld2New.clear();

	// set version history
	ms_pPreIOCP->m_LatestLauncherVersion.SetVersion(msg->LauncherVersion);
	ms_pPreIOCP->m_LatestDeleteFileListVersion.SetVersion(msg->DeleteFileListVersion);
	ms_pPreIOCP->m_LatestNoticeVersion.SetVersion(msg->NoticeVersion);
	for (int i = 0; i < msg->NumOfClientVersions; i++)
	{
		USHORT *pOldVersionArray = (USHORT*)(((char*)msg) + sizeof(MSG_PM_RELOAD_VERSION_INFO) + i*8*sizeof(USHORT));
		USHORT *pNewVersionArray = (USHORT*)(((char*)msg) + sizeof(MSG_PM_RELOAD_VERSION_INFO) + i*8*sizeof(USHORT) + 4*sizeof(USHORT));

		VersionInfo OldVersion(pOldVersionArray);
		VersionInfo NewVersion(pNewVersionArray);

		ms_pPreIOCP->m_mtmapVersionOld2New.insertLock(OldVersion, NewVersion);
	}

	// set current client version
	if (!ms_pPreIOCP->m_mtmapVersionOld2New.empty())
	{
		ms_pPreIOCP->m_LatestClientVersion = ms_pPreIOCP->m_mtmapVersionOld2New.rbegin()->second;
	}

	// write versions
	ms_pPreIOCP->WriteVersionInfoToLocalFile();
#ifdef _DEBUG
	mtmapVersionInfo::iterator itr = ms_pPreIOCP->m_mtmapVersionOld2New.begin();
	for (; itr != ms_pPreIOCP->m_mtmapVersionOld2New.end(); itr++)
	{
		VersionInfo tmpOldVersion = itr->first;
		VersionInfo tmpNewVersion = itr->second;
		DBGOUT("  Client Version: %10s -> %10s\n", tmpOldVersion.GetVersionString(), tmpNewVersion.GetVersionString());
	}
	DBGOUT("  Latest Client Version: %s\n", ms_pPreIOCP->m_mtmapVersionOld2New.rbegin()->second.GetVersionString());
	DBGOUT("  Client Reinstall Version: %s\n", g_pPreGlobal->GetClientReinstallVersion().GetVersionString());
	DBGOUT("\n  Launcher Version: %s\n", ms_pPreIOCP->m_LatestLauncherVersion.GetVersionString());
	DBGOUT("  Delete File List Version: %s\n", ms_pPreIOCP->m_LatestDeleteFileListVersion.GetVersionString());
	DBGOUT("  Notice Version: %s\n\n", ms_pPreIOCP->m_LatestNoticeVersion.GetVersionString());
#endif
	ms_pPreIOCP->m_mtmapVersionOld2New.unlock();
	ms_pPreIOCP->m_bPauseService = FALSE;

	socket->SendAddMessageType(T_PM_RELOAD_VERSION_INFO_OK);
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_RELOAD_BLOCKED_ACCOUNTS>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{	
	// 2016-01-09 Future, improved Account block loading
	ms_pPreIOCP->m_pAccountBlockManager->LoadBlockedAccountListFromDB();

	socket->SendAddMessageType(T_PM_RELOAD_BLOCKED_ACCOUNTS_OK);
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_CHANGE_BANDWIDTH_FLAG>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	BYTE SendBuf[SIZE_MAX_PACKET];
	
	ms_pPreIOCP->SetFlagCalcTrafficInfo(!ms_pPreIOCP->GetFlagCalcTrafficInfo());

	*(MessageType_t*)SendBuf = T_PM_CHANGE_BANDWIDTH_FLAG_OK;
	MSG_PM_CHANGE_BANDWIDTH_FLAG_OK		*pChangeFlag
		 = (MSG_PM_CHANGE_BANDWIDTH_FLAG_OK*)(SendBuf + SIZE_FIELD_TYPE_HEADER);
	pChangeFlag->bChagedFlag = ms_pPreIOCP->GetFlagCalcTrafficInfo();

	socket->SendAddData(SendBuf, MSG_SIZE(MSG_PM_CHANGE_BANDWIDTH_FLAG_OK));
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_SET_MSG_PRINT_LEVEL>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{	
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	GSetexchangeMsgPrintLevel(msg->Level);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_PAUSE_SERVICE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{	
	server::log(true, "[Notice] Service paused, T_PM_PAUSE_SERVICE COMMAND! PeerIP:[%s]\r\n", socket->GetPeerIP());

	ms_pPreIOCP->m_bPauseService = true;

	socket->SendAddMessageType(T_PM_PAUSE_SERVICE_OK);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_START_SERVICE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{	
	ms_pPreIOCP->m_bPauseService = false;

	socket->SendAddMessageType(T_PM_START_SERVICE_OK);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_GET_SERVER_GROUP_INFO>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;
	
	auto pSGroup = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);
	
	if (pSGroup && pSGroup->m_IMServerInfo.IsActive && pSGroup->m_IMServerInfo.pSocket)
		
		pSGroup->m_IMServerInfo.pSocket->SendAddMessageType(T_IP_GET_SERVER_GROUP_INFO);

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_SET_LIMIT_GROUP_USER_COUNTS>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	auto pSGroup = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);
	
	if (pSGroup)
	{
		pSGroup->m_LimitGroupUserCounts = msg->LimitGroupUserCounts;

		MessageData<T_PM_GET_SERVER_GROUP_INFO_ACK> pSendGroupInfo;

		util::strncpy(pSendGroupInfo->ServerGroupName, pSGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
		pSendGroupInfo->IMServerID					= pSGroup->m_IMServerInfo.serverID;
		pSendGroupInfo->AccumulatedGroupUserCounts	= pSGroup->m_AccumulatedGroupUserCounts;
		pSendGroupInfo->CurrentGroupUserCounts		= pSGroup->GetUserCount();
		pSendGroupInfo->MaxGroupUserCounts			= pSGroup->m_MaxGroupUserCounts;
		pSendGroupInfo->bEnableServerGroup			= pSGroup->m_bEnableServerGroup;
		pSendGroupInfo->LimitGroupUserCounts		= pSGroup->m_LimitGroupUserCounts;
		
		ms_pPreIOCP->SendMessageToMonitor(pSendGroupInfo);
	}

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_PM_CHANGE_ENABLE_SERVER_GROUP>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{	
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	auto pSGroup = ms_pPreIOCP->GetServerGroup(msg->ServerGroupName);
	
	if (pSGroup) pSGroup->m_bEnableServerGroup = msg->bEnableServerGroup;
	
	return RES_BREAK;
}


template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CONNECT_ALIVE>::handler(CPreIOCPSocket* socket, const char*, int, int &)
{
	socket->OnRecvdAlivePacket();
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CONNECT_AUTH_USER>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		socket->SendErrorMessage(T_FP_CONNECT_AUTH_USER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}

	CAccountInfo tmAccountInfo;
	int nRetedErrorNum = ms_pPreIOCP->On_MSG_FP_CONNECT_AUTH_USER(&tmAccountInfo, msg);

	switch(nRetedErrorNum)
	{
	case 0:// 인증 성공
		{
			auto pSGroup = ms_pPreIOCP->GetServerGroup(tmAccountInfo.CurrentServerGroup);

			if (pSGroup && msg->AuthType == PRESERVER_AUTH_TYPE_LOGIN)
					
				pSGroup->m_AccumulatedGroupUserCounts++;

			//////////////////////////////
			// Send AUTH_USER_OK
			MessageData<T_FP_CONNECT_AUTH_USER_OK> packet;

			util::strncpy(packet->AccountName, tmAccountInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
			packet->AccountUniqueNumber	= tmAccountInfo.AccountUniqueNumber;
			packet->ClientIndex			= msg->ClientIndex;
			packet->AuthType				= msg->AuthType;
			packet->AccountRegisteredDate	= tmAccountInfo.AccountRegisteredDate;		// 2006-06-02 by cmkwon
			packet->GameContinueTimeInSecondOfToday	= tmAccountInfo.GameContinueTimeInSecondOfToday;		// 2006-11-15 by cmkwon
			packet->LastGameEndDate					= tmAccountInfo.LastGameEndDate;						// 2006-11-15 by cmkwon
			packet->Birthday							= tmAccountInfo.atBirthday;								// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 프로토콜 수정
			packet->UserType							= tmAccountInfo.AccountType;								// 2013-01-18 by khkim, GLog 유저 권한 
			util::strncpy(packet->PasswordFromDB, tmAccountInfo.PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING);	// 2006-06-02 by cmkwon
			util::strncpy(packet->SecondaryPassword, tmAccountInfo.SecondaryPassword, SIZE_MAX_PASSWORD_MD5_STRING);	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - Field Server로 전달

			socket->SendAddData(packet);

			return RES_RETURN_TRUE;
		}
	case ERR_PROTOCOL_NOT_LOGINED:
		{
			// Protocol Error 처리
			// Error Code : ERR_PROTOCOL_NOT_LOGINED
			socket->SendErrorMessage(T_FP_CONNECT_AUTH_USER, ERR_PROTOCOL_NOT_LOGINED, msg->ClientIndex);
		}
		break;
	case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE:
		{
			// Protocol Error 처리
			// Error Code : ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE
			socket->SendErrorMessage(T_FP_CONNECT_AUTH_USER, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE, msg->ClientIndex);
		}
		break;
	case ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED:
		{
			// Protocol Error 처리
			// Error Code : ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED
			DBGOUT("ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED: %s != %s\n", tmAccountInfo.CurrentFieldServerID.GetString().GetBuffer(), msg->FieldServerID.GetString().GetBuffer());
			socket->SendErrorMessage(T_FP_CONNECT_AUTH_USER, ERR_PROTOCOL_FIELD_SERVER_ID_NOT_MATCHED, msg->ClientIndex, 0, tmAccountInfo.AccountName);
		}
		break;
	case ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED:
		{
			// Protocol Error 처리
			// Error Code : ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED
#ifdef _DEBUG
			DBGOUT("MSG_FP_CONNECT_AUTH_USER(%s) != CAccountInfo(%s)\n", pMsgRecvAuthUser->PrivateIP, tmAccountInfo.PrivateClientIP);
#endif
			socket->SendErrorMessage(T_FP_CONNECT_AUTH_USER, ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED, msg->ClientIndex, 0, tmAccountInfo.AccountName);
		}
		break;
	default:
		{
			socket->SendErrorMessage(T_FP_CONNECT_AUTH_USER, ERR_COMMON_UNKNOWN_ERROR, 0, 0, 0);
		}
	}

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CONNECT_FIELD_CONNECT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	/*
	1. 필드 서버를 active 상태로 전환
	2. 프리 서버가 기지고 있는 ip와 필드 서버 이름, 서버군 이름을 비교 확인

	- 이 프로토콜은 필드 서버를 프리 서버에 등록시키기 위함
	*/

	// todo : improve this mess in T_FP_CONNECT_FIELD_CONNECT handler

	int		nRecvTypeSize	= 0;
	MSG_FP_CONNECT_FIELD_CONNECT *pMsgRecvFieldConnect;

	nRecvTypeSize = sizeof(MSG_FP_CONNECT_FIELD_CONNECT)
			+ ((MSG_FP_CONNECT_FIELD_CONNECT*)(pPacket+nBytesUsed))->NumOfMapIndex * sizeof(MapIndex_t);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		socket->SendErrorMessage(T_FP_CONNECT_FIELD_CONNECT, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgRecvFieldConnect = (MSG_FP_CONNECT_FIELD_CONNECT*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CServerGroup *pServerGroup = ms_pPreIOCP->GetServerGroup(pMsgRecvFieldConnect->FieldServerGroupName);
	if (pServerGroup == NULL)
	{
		socket->SendErrorMessage(T_FP_CONNECT_FIELD_CONNECT, ERR_PROTOCOL_NO_SUCH_SERVER_GROUP);
		socket->Close(0x1100C, TRUE);
		return RES_RETURN_FALSE;
	}
	
	if (pServerGroup->m_FieldServerInfo.IsActive)
	{
		socket->SendErrorMessage(T_FP_CONNECT_FIELD_CONNECT, ERR_PROTOCOL_FIELD_SERVER_ALREADY_REGISTERD);
		socket->Close(0x1100D, TRUE);
		return RES_RETURN_FALSE;
	}

	util::strncpy(socket->m_szConnectedServerGroupName, pServerGroup->m_ServerGroupName, SIZE_MAX_SERVER_NAME);
	socket->m_PeerSocketType = ST_FIELD_SERVER;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
	pServerGroup->m_LimitGroupUserCounts				= pMsgRecvFieldConnect->DBServerGroup.LimitUserCount;
	pServerGroup->m_bLockCreateCharacterForNewAccount	= pMsgRecvFieldConnect->DBServerGroup.LockCreateCharacterForNewAccount;

	// 2007-12-26 by dhjin, 아레나 통합 - TRUE => 아레나 필드 서버
	if(TRUE == pMsgRecvFieldConnect->ArenaFieldServerCheck)
	{// 2007-12-26 by dhjin, 아레나 통합 - 아레나 서버는 런쳐 목록에서 제외한다.
		pServerGroup->m_bForbidViewServer = TRUE;
	}
	FieldServerInfo *pFieldServerInfo = &pServerGroup->m_FieldServerInfo;
	pFieldServerInfo->serverID		= pMsgRecvFieldConnect->FieldServerID;
	pFieldServerInfo->pSocket		= socket;
	pFieldServerInfo->ServerType	= ST_FIELD_SERVER;
	for (int i = 0; i < pMsgRecvFieldConnect->NumOfMapIndex; i++)
	{
		MapIndex_t tmpMapIndex
			= *(MapIndex_t*)(((char*)pMsgRecvFieldConnect) + sizeof(MSG_FP_CONNECT_FIELD_CONNECT) + sizeof(MapIndex_t)*i);


		pFieldServerInfo->MapIndexVector.push_back(tmpMapIndex);
#ifdef _DEBUG
		DBGOUT("FieldServer(%s) -> %04d\n", pFieldServerInfo->serverID.GetString(string()), tmpMapIndex);
#endif
	}
	pFieldServerInfo->IsActive = true;				// Map을 모두 추가하고 TRUE를 설정한다

	server::log(true, "ServerGroup(%s) FieldServer(%s), PeerIPPort[%s:%d] registeration done...\r\n",
		pServerGroup->m_ServerGroupName, pFieldServerInfo->serverID.GetString().GetBuffer(), socket->GetPeerIP(), socket->GetPeerPort());

	// send T_FP_CONNECT_FIELD_CONNECT_OK
	MessageData<T_FP_CONNECT_FIELD_CONNECT_OK> packet;
	socket->SendAddData(packet);
	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CONNECT_NOTIFY_CLOSE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		socket->SendErrorMessage(T_FP_CONNECT_NOTIFY_CLOSE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}

#ifdef _DEBUG
	DBGOUT("%s NOTIFY_CLOSE: %s\n", GetTimeString(string()), msg->AccountName);
#endif

	// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
	//ms_pPreIOCP->DeleteAccountInfo(pMsgRecvClose->AccountName);
	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
	ms_pPreIOCP->DeleteAccountInfo(msg->AccountName);

//#ifdef _DEBUG
//	ms_pPreIOCP->DBG_PrintAccountMap();
//#endif

	return RES_RETURN_TRUE;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_EVENT_ENTER_SELECT_SCREEN>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	CP_State cpState;
	int nRetedErrorNum = ms_pPreIOCP->On_MSG_FP_EVENT_ENTER_SELECT_SCREEN(&cpState, msg);

	switch(nRetedErrorNum)
	{
	case 0: return RES_RETURN_TRUE;
	case ERR_PROTOCOL_NOT_LOGINED:
		socket->SendErrorMessage(T_FP_EVENT_ENTER_SELECT_SCREEN, ERR_PROTOCOL_NOT_LOGINED, 0, 0, msg->AccountName);
		break;
	case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE:
		socket->SendErrorMessage(T_FP_EVENT_ENTER_SELECT_SCREEN, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE, cpState,  0, msg->AccountName);
		break;
	default: socket->SendErrorMessage(T_FP_EVENT_ENTER_SELECT_SCREEN, ERR_COMMON_UNKNOWN_ERROR);
	}

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_EVENT_GAME_STARTED>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	CP_State cpState;
	int nRetedErrorNum = ms_pPreIOCP->On_MSG_FP_EVENT_GAME_STARTED(&cpState, msg);
	switch(nRetedErrorNum)
	{
	case 0: return RES_RETURN_TRUE;

	case ERR_PROTOCOL_NOT_LOGINED:
		socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_PROTOCOL_NOT_LOGINED, 0, 0, msg->AccountName);
		break;
	case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE:
		socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE, cpState,  0, msg->AccountName);
		break;
	default: socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_COMMON_UNKNOWN_ERROR);
	}

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_EVENT_MAP_CHANGED>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed) 
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;
 
	CP_State cpState; 
	int nRetedErrorNum = ms_pPreIOCP->On_MSG_FP_EVENT_MAP_CHANGED(&cpState, msg); 
	switch(nRetedErrorNum) 
	{ 
	case 0: return RES_RETURN_TRUE;
	case ERR_PROTOCOL_NOT_LOGINED: 
		socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_PROTOCOL_NOT_LOGINED, 0, 0, msg->AccountName);
		break; 
	case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE: 
		socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE, cpState,  0, msg->AccountName);
		break; 
	default: socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_COMMON_UNKNOWN_ERROR);
	} 
 
	return RES_BREAK; 
} 
 
template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CASH_CHANGE_CHARACTERNAME>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	mt_auto_lock mtA(&ms_pPreIOCP->m_AccountInfoMap);
	CAccountInfo *pAccountInfo = ms_pPreIOCP->GetAccountInfoPtr(msg->szAccName);
	if (pAccountInfo == NULL)
	{		
		return RES_BREAK;
	}

	util::strncpy(pAccountInfo->CurrentCharacterName, msg->szChangedCharName, SIZE_MAX_CHARACTER_NAME);
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_ADMIN_BLOCKACCOUNT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 
	server::log(TRUE, "[Notify] Account Block: AdminAccountName(%20s), BlockedUserAccName(%20s) Period(%s ~ %s)\r\n"
		, msg->blockAccInfo.szBlockAdminAccountName, msg->blockAccInfo.szBlockedAccountName
		, msg->blockAccInfo.atimeStartTime.GetDateTimeString().GetBuffer()
		, msg->blockAccInfo.atimeEndTime.GetDateTimeString().GetBuffer());

	ms_pPreIOCP->BlockAccount(&msg->blockAccInfo, socket, TRUE);
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_ADMIN_UNBLOCKACCOUNT>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-20 by cmkwon, 계정 블럭정보 시스템 로그에 추가
	server::log(TRUE, "[Notify] Account Unblock: AdminAccountName(%20s), BlockedUserAccName(%20s)\r\n"
		, msg->blockAccInfo.szBlockAdminAccountName, msg->blockAccInfo.szBlockedAccountName);

	//msg->blockAccInfo.atimeStartTime.SetCurrentDateTime();
	//msg->blockAccInfo.atimeEndTime = msg->blockAccInfo.atimeStartTime;
	if(FALSE == CPreIOCPSocket::ms_pPreIOCP->UnblockAccount(&msg->blockAccInfo, socket))
	{
		// 2008-02-01 by cmkwon, 블록 리스트에 없다.
		MessageData<T_FP_ADMIN_UNBLOCKACCOUNT_OK> packet { };
		packet->ErrCode		= ERR_NOT_ACCOUNT_BLOCKED;
		util::strncpy(packet->UnblockedAccName, msg->blockAccInfo.szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
		socket->SendAddData(packet);
	}
	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		socket->SendErrorMessage(T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}

	int nRetedErrorNum = ms_pPreIOCP->On_MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE(msg);
	switch(nRetedErrorNum)
	{
	case 0:
		{
			//////////////////////////////
			// Send MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK
			MessageData<T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK> packet { };
			packet->ClientIndex = msg->ClientIndex;

			socket->SendAddData(packet);
			return RES_RETURN_TRUE;
		}
	case ERR_PROTOCOL_NOT_LOGINED:
		socket->SendErrorMessage(T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE, ERR_PROTOCOL_NOT_LOGINED, msg->ClientIndex);
		break;
	case ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE:
		socket->SendErrorMessage(T_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE, ERR_PROTOCOL_INVALID_PRESERVER_CLIENT_STATE, msg->ClientIndex);
		break;
	default: socket->SendErrorMessage(T_FP_EVENT_GAME_STARTED, ERR_COMMON_UNKNOWN_ERROR);
	}

	return RES_BREAK;
}

template<> ProcessResult CPreIOCPSocket::HandlerT1<T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT_OK>::handler(CPreIOCPSocket* socket, const char* pPacket, int nLength, int &nBytesUsed)
{
	auto msg = GetMessageData(pPacket, nLength, nBytesUsed);

	if (!msg) return RES_PACKET_ERROR;

	CAccountInfo tmPreAccountInfo;	
	BOOL bReted = ms_pPreIOCP->GetAccountInfo(&tmPreAccountInfo, msg->AccountName);
	if(FALSE == bReted)
	{// 2008-04-30 by cmkwon, 계정 정보가 없다.
		return RES_BREAK;
	}

	if(FALSE == tmPreAccountInfo.CurrentFieldServerID.CompareValue(msg->PCConnectLoginOK.FieldServerIP, msg->PCConnectLoginOK.FieldServerPort))
	{// 2008-04-30 by cmkwon, FieldServer 정보가 다르다
		return RES_BREAK;
	}

	CPreIOCPSocket *pPISoc = ms_pPreIOCP->GetPreIOCPSocket(tmPreAccountInfo.CurrentPreServerClientIndex);
	if( NULL == pPISoc
		|| FALSE == pPISoc->IsUsing()
		|| 0 != strnicmp(pPISoc->m_szAdminAccountName, msg->AccountName, SIZE_MAX_ACCOUNT_NAME) )
	{
		return RES_BREAK;
	}

	if(ERR_NO_ERROR != msg->ErrorCode)
	{
		pPISoc->SendErrorMessage(T_PC_CONNECT_LOGIN, msg->ErrorCode, 0, 0, msg->AccountName);
		return RES_BREAK;
	}

	pPISoc->SendAddData(MessageData<T_PC_CONNECT_LOGIN_OK> { msg->PCConnectLoginOK });
	
#ifdef S_GLOG_HSSON
	// 2010-06-01 by shcho, GLogDB 관련 -
	QPARAM_GLOG_INSERT_ACCOUNT * pQMsg = new QPARAM_GLOG_INSERT_ACCOUNT;
	util::strncpy(pQMsg->szAccountName, pRecvMsg->AccountName, SIZE_MAX_ACCOUNT_NAME);
	ms_pPreIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertGlogAccount, this, (void*)pQMsg);
#endif

	// start 2011-12-12 by hskim, GLog 2차
#ifdef S_GLOG_2ND_HSKIM
	MessageData<T_FP_GLOG_CONNECT_USER_LOGIN> packet { };
	util::strncpy(pSendGLogMsg->AccountName, pRecvMsg->AccountName, SIZE_MAX_ACCOUNT_NAME);
	SendAddData(pSendGLogBuf, MSG_SIZE(MSG_FP_GLOG_CONNECT_USER_LOGIN));
#endif
	// end 2011-12-12 by hskim, GLog 2차

	return RES_RETURN_TRUE;
}


template<T0 t0> ProcessResult CPreIOCPSocket::HandlerT0(CPreIOCPSocket* socket, MessageType_t msgtype, const char* data, int length, int& offset)
{
	switch (t0)
	{
	case T0::PC_DEFAULT_UPDATE:
	case T0::PC_CONNECT:

		if (!ms_pPreIOCP->m_bPauseService || socket->IsWhitelisted()) break;
		socket->SendErrorMessage(T_ERROR, ERR_COMMON_SERVICE_TEMPORARILY_PAUSED, 0, 0, socket->GetPeerIP());
		return RES_PACKET_ERROR;

	case T0::FP_CONNECT:

		if (socket->m_PeerSocketType == ST_FIELD_SERVER) break;
		if (socket->IsWhitelisted()) break;
		return RES_ACCESS_DENIED;

	case T0::IP_CONNECT:

		if (socket->m_PeerSocketType == ST_IM_SERVER) break;
		if (socket->IsWhitelisted()) break;
		return RES_ACCESS_DENIED;

	case T0::PM_CONNECT:

		if (socket->m_PeerSocketType == ST_MONITOR_SERVER) break;
		if (socket->IsWhitelisted()) break;
		return RES_ACCESS_DENIED;

	case T0::FP_EVENT:
	case T0::FP_CASH:

		if (socket->m_PeerSocketType == ST_FIELD_SERVER) break;
		return RES_ACCESS_DENIED;

	case T0::PM_MONITOR:

		if (socket->m_PeerSocketType == ST_MONITOR_SERVER) break;
		return RES_ACCESS_DENIED;

	case T0::PA_ADMIN:

		if (socket->m_PeerSocketType == ST_ADMIN_TOOL) break;
		if (socket->IsWhitelisted()) break;
		return RES_ACCESS_DENIED;

	default: return RES_PACKET_NA;
	}

	return socket->CallHandlerT1<CPreIOCPSocket, t0>(msgtype, data, length, offset);
}

void CPreIOCPSocket::OnConnect()
{
	server::log(TRUE, "Socket Connected: Index[%3d] PeerIP[%15s] Port[%4d]\r\n", GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);

	m_CPClientState = CP_CONNECTED;
	m_PeerSocketType = ST_INVALID_TYPE;

	// 2007-06-20 by cmkwon, Added to your account information block the system log
	util::zero(m_szAdminAccountName, SIZE_MAX_ACCOUNT_NAME);

	CIOCPSocket::OnConnect();
}

void CPreIOCPSocket::OnClose(int reason)
{
	server::log(TRUE, "Socket Closed: Index[%3d] Type[%d] PeerIP[%15s] Port[%4d] MaxWriteBufCounts[%4d] ==> reason [%#08X]\r\n",
		GetClientArrayIndex(), m_PeerSocketType, GetPeerIP(), GetPeerPort(), m_nMaxWriteBufCounts, reason);
	
	if (m_PeerSocketType == ST_FIELD_SERVER)
	{
		server::log(TRUE, "DISCONNECTED FieldServer(%15s:%5d), ServerGroupName(%12s)\r\n",
			GetPeerIP(), GetPeerPort(), m_szConnectedServerGroupName);

		ms_pPreIOCP->ResetServerGroup(m_szConnectedServerGroupName, TRUE);
	}

	if (m_PeerSocketType == ST_IM_SERVER)
	{
		server::log(TRUE, "DISCONNECTED IMServer(%15s:%5d), ServerGroupName(%12s)\r\n",
			GetPeerIP(), GetPeerPort(), m_szConnectedServerGroupName);
		
		ms_pPreIOCP->ResetServerGroup(m_szConnectedServerGroupName);
	}

	if (m_PeerSocketType == ST_MONITOR_SERVER)
	{
		ms_pPreIOCP->DeleteMonitorIOCPSocketPtr(this);
	}

	if (m_PeerSocketType == ST_ADMIN_TOOL)
	{
		ms_pPreIOCP->DeleteMonitorIOCPSocketPtr(this);
	}

	m_CPClientState = CP_NOTCONNECTED;
	m_PeerSocketType = ST_INVALID_TYPE;

	util::zero(m_szConnectedServerGroupName, SIZE_MAX_SERVER_NAME);

	CIOCPSocket::OnClose(30);
}

// returns whether to keep the connection alive after procesing this packet
BOOL CPreIOCPSocket::OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	int nBytesUsed = 0;

	if (m_bPeerSequenceNumberInitFlag)
	{
		if (nSeq != m_byPeerSequenceNumber)
		{
			SendErrorMessage(T_PRE_IOCP, ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER);
			Close(0x11000, TRUE);
			return FALSE;
		}

		int tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;

		if (tmpSeq > SEQNO_VAR_C) tmpSeq = tmpSeq % SEQNO_VAR_C;

		m_byPeerSequenceNumber = ++tmpSeq;
	}
	else
	{
		int tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;

		if (tmpSeq > SEQNO_VAR_C) tmpSeq = tmpSeq % SEQNO_VAR_C;

		m_byPeerSequenceNumber = ++tmpSeq;
		m_bPeerSequenceNumberInitFlag = true;
	}


	while (IsUsing() && nBytesUsed < nLength)
	{
		auto nRecvType = *(MessageType_t*)(pPacket + nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

		if (i_pThreadInfo)
		{
			i_pThreadInfo->dwSocketIndex = m_nClientArrayIndex;
			i_pThreadInfo->dwMessageType = nRecvType;
		}

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nRecvType, m_szPeerIP, m_PeerSocketType, GGetexchangeMsgPrintLevel());
#endif

#ifdef SERVERCONSOLE
		//cout << "[" << m_nClientArrayIndex << "] Type(" << nRecvType << ":" << GetProtocolTypeString(nRecvType) << ") IP(" << this->GetPeerIP() << ")" << endl;
#endif	

		auto procRes = CallHandlerT0<CPreIOCPSocket>(nRecvType, pPacket, nLength, nBytesUsed);

		if (procRes == RES_PACKET_NA)
		{
			SendErrorMessage(T_PRE_IOCP, ERR_PROTOCOL_INVALID_PROTOCOL_TYPE, 0, 0, nullptr, TRUE);

			server::log(TRUE, "[Error] CPreIOCPSocket::OnRecvdPacketPreServer invalid protocol type, CurRecvType[%s(%#04X)]", GetProtocolTypeString(nRecvType), nRecvType);

			return FALSE;
		}

		if (procRes == RES_ACCESS_DENIED)
		{
			server::log(TRUE, "[HACKUSER] CPreIOCPSocket::OnRecvdPacketPreServer, CurRecvType[%s(%#04X) HackingIP(%15s)\r\n", GetProtocolTypeString(nRecvType), nRecvType, GetPeerIP());

			return FALSE;
		}

		if (procRes == RES_PACKET_ERROR) return TRUE;

		if (procRes == RES_RETURN_FALSE) return FALSE;

	}

	return TRUE;
}

BOOL CPreIOCPSocket::ResPreLogin(MSG_PC_CONNECT_LOGIN *pRecvMsgLogin, CAccountInfo *i_pAccInfo, Err_t nErr)
{
	if (!IsUsing()) return FALSE;

	if (ERR_NO_ERROR != nErr || !IS_VALID_UNIQUE_NUMBER(i_pAccInfo->AccountUniqueNumber))
	{
		SendErrorMessage(T_PC_CONNECT_LOGIN, nErr, 0, 0, pRecvMsgLogin->AccountName);
		return FALSE;
	}
	
	auto serverGroup = ms_pPreIOCP->GetServerGroup(pRecvMsgLogin->FieldServerGroupName);
	if (!serverGroup)
	{
		SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_NO_SUCH_SERVER_GROUP);
		return FALSE;
	}
	
	if (!serverGroup->m_IMServerInfo.IsActive)
	{
		SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_IM_SERVER_NOT_ALIVE, serverGroup->m_IMServerInfo.IsActive, 0, pRecvMsgLogin->FieldServerGroupName);
		DBGOUT("Account(%s), PrivateIP(%s)\r\n", pRecvMsgLogin->AccountName, pRecvMsgLogin->PrivateIP);
		return FALSE;
	}
	
	ServerInfo *serverInfo = &serverGroup->m_FieldServerInfo;
	if(FALSE == serverInfo->IsActive)
	{
		SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_ALL_FIELD_SERVER_NOT_ALIVE);
		return FALSE;
	}
	
	if (FALSE == g_pPreGlobal->CheckAllowedList(serverGroup, pRecvMsgLogin->AccountName, pRecvMsgLogin->PrivateIP, i_pAccInfo->AccountType))
	{
		DBGOUT("  NOT ALLOWED IP or Account: Account(%s) Private(%s) Public(%s)\r\n"
			, pRecvMsgLogin->AccountName, pRecvMsgLogin->PrivateIP, this->GetPeerIP());
		SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PERMISSION_DENIED);					// 2006-09-27 by cmkwon, error code 변경
		return FALSE;
	}

	CAccountInfo tmPreAccountInfo;
	ms_pPreIOCP->m_AccountInfoMap.lock();
	BOOL bReted = ms_pPreIOCP->GetAccountInfo(&tmPreAccountInfo, pRecvMsgLogin->AccountName);
	if (bReted)
	{// 계정 정보가 있음

		if (strncmp(tmPreAccountInfo.PrivateClientIP, pRecvMsgLogin->PrivateIP, SIZE_MAX_IPADDRESS) != 0
			|| tmPreAccountInfo.ClientState >= CP_FIELD_LOGGEDIN)
		{// 이중 로그인, 양 쪽의 연결을 다 끊는다. AccountInfoMap에서 제거한다.

			///////////////////////////////////////////////////////////////////////////////
			// Account 정보 지우기
			ms_pPreIOCP->DeleteAccountInfo(pRecvMsgLogin->AccountName);
			ms_pPreIOCP->m_AccountInfoMap.unlock();

			///////////////////////////////////////////////////////////////////////////////
			// 기존의 user 연결 끊기: 필드 서버에 에러 메세지를 보낸다
			ms_pPreIOCP->SendErrorToFieldServer(tmPreAccountInfo.CurrentServerGroup, tmPreAccountInfo.CurrentFieldServerID
				, T_PC_CONNECT_LOGIN, ERR_PROTOCOL_DUPLICATE_LOGIN
				, tmPreAccountInfo.CurrentClientIndex, 0, pRecvMsgLogin->AccountName);

			///////////////////////////////////////////////////////////////////////////////
			// 새로운(방금 접속을 시도하는) user 연결 끊기
			SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_DUPLICATE_LOGIN, 0, 0, pRecvMsgLogin->AccountName);
			Sleep(100);
			Close(0x11015, TRUE);			
			return FALSE;
		}

		// Laucher만  로그인후 다시 Launcher로 로그인 시도를 한상태
		// Launcher 실행후 클라이언트가 실행접속 되지않고 종료된 상태임
		ms_pPreIOCP->DeleteAccountInfo(tmPreAccountInfo.AccountName);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 추가할 계정 정보를 설정한다.
	auto pCurrentAccountInfo = new CAccountInfo { *i_pAccInfo }; // copy construction

	util::strncpy(pCurrentAccountInfo->AccountName, pRecvMsgLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(pCurrentAccountInfo->PrivateClientIP, pRecvMsgLogin->PrivateIP, SIZE_MAX_IPADDRESS);
	util::strncpy(pCurrentAccountInfo->PublicClientIP, GetPeerIP(), SIZE_MAX_IPADDRESS);
	util::strncpy(pCurrentAccountInfo->CurrentCharacterName, "", SIZE_MAX_CHARACTER_NAME);	// check: 확인!
	pCurrentAccountInfo->CurrentCharacterUniqueNumber	= 0;							// check: 확인!
	util::strncpy(pCurrentAccountInfo->CurrentServerGroup, pRecvMsgLogin->FieldServerGroupName, SIZE_MAX_SERVER_NAME);	
	pCurrentAccountInfo->CurrentFieldServerID			= serverInfo->serverID;
	pCurrentAccountInfo->ClientState = CP_LOGGEDIN; //SetClientState(CP_LOGGEDIN, pCurrentAccountInfo);
	pCurrentAccountInfo->LauncherLoginTime.SetCurrentDateTime();
	pCurrentAccountInfo->CurrentPreServerClientIndex	= this->GetClientArrayIndex();		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
		
	pCurrentAccountInfo->bSelectiveShutdownTarget = FALSE;
	
	memcpy(pCurrentAccountInfo->SelectiveShutdownInfo, pRecvMsgLogin->SelectiveShutdownInfo, sizeof(pRecvMsgLogin->SelectiveShutdownInfo));
	// end 2012-07-11 by hskim, 선택적 셧다운

	/////////////////////////////////////////////////////
	// 메시지를 미리 만든다, MSG_PC_CONNECT_LOGIN_OK to client
	MessageData<T_PC_CONNECT_LOGIN_OK> packet { };
	util::strncpy(packet->AccountName, pRecvMsgLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);	// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) - 2단계 계정을 전송한다.
	util::strncpy(packet->FieldServerIP, pCurrentAccountInfo->CurrentFieldServerID.IP, SIZE_MAX_IPADDRESS);
	packet->FieldServerPort = (USHORT)pCurrentAccountInfo->CurrentFieldServerID.port;
	util::strncpy(packet->IMServerIP, serverGroup->m_IMServerInfo.serverID.IP, SIZE_MAX_IPADDRESS);
	packet->IMServerPort = (USHORT)serverGroup->m_IMServerInfo.serverID.port;

	// start 2011-12-21 by hskim, EP4 [동영상 1회 재생]
#ifdef S_EP4_HSKIM		// ON/OFF 기능 구현
	if (ms_pPreIOCP->m_OpeningMovieStandard.IsValid() && pCurrentAccountInfo->LastGameEndDate.IsValid() )
	{
		if (pCurrentAccountInfo->LastGameEndDate != pCurrentAccountInfo->AccountRegisteredDate)

			packet->OpeningMoviePlay = pCurrentAccountInfo->LastGameEndDate < ms_pPreIOCP->m_OpeningMovieStandard;

		else packet->OpeningMoviePlay = true;
	}
	else packet->OpeningMoviePlay = true;

	if (packet->OpeningMoviePlay)
	{
		auto pQMsg = new QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE;
		pQMsg->AccountUniqueNumber = i_pAccInfo->AccountUniqueNumber;
		ms_pPreIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateAccountLastGameEndDate, this, (void*)pQMsg);
	}
#else
	pSendMsg->OpeningMoviePlay = FALSE;
#endif
	// end 2011-12-21 by hskim, EP4 [동영상 1회 재생]

	if (!ms_pPreIOCP->InsertAccountInfo(pCurrentAccountInfo->AccountName, pCurrentAccountInfo))
	{
		
		server::log(TRUE, STRMSG_S_P2PRENOTIFY_0003, pRecvMsgLogin->AccountName, pRecvMsgLogin->PrivateIP);

		SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_PROTOCOL_DUPLICATE_LOGIN, 0, 0, pCurrentAccountInfo->AccountName);
		Close(0x11016, TRUE);
		
		if(ms_pPreIOCP->GetAccountInfo(&tmPreAccountInfo, pCurrentAccountInfo->AccountName))
		{
			ms_pPreIOCP->SendErrorToFieldServer(tmPreAccountInfo.CurrentServerGroup, tmPreAccountInfo.CurrentFieldServerID
				, T_PC_CONNECT_LOGIN, ERR_PROTOCOL_DUPLICATE_LOGIN, tmPreAccountInfo.CurrentClientIndex, 0, tmPreAccountInfo.AccountName);
		}
		
		ms_pPreIOCP->DeleteAccountInfo(pRecvMsgLogin->AccountName);
		ms_pPreIOCP->m_AccountInfoMap.unlock();

		util::del(pCurrentAccountInfo);
		return FALSE;
	}

	ms_pPreIOCP->m_AccountInfoMap.unlock();

	if (!serverGroup->m_bLockCreateCharacterForNewAccount || COMPARE_BIT_FLAG(i_pAccInfo->AccountType, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))	// 2008-05-19 by cmkwon, 관리자/운영자는 서버군 접속 가능여부 체크 하지 않고 성공을 바로 전송
	{
		SendAddData(packet);

#ifdef S_GLOG_HSSON
			// 2010-06-01 by shcho, GLogDB 관련 -
			QPARAM_GLOG_INSERT_ACCOUNT * pQMsg = new QPARAM_GLOG_INSERT_ACCOUNT;
			util::strncpy(pQMsg->szAccountName, pCurrentAccountInfo->AccountName, SIZE_MAX_ACCOUNT_NAME);
			ms_pPreIOCP->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertGlogAccount, this, (void*)pQMsg);
#endif

		// start 2011-12-12 by hskim, GLog 2차
#ifdef S_GLOG_2ND_HSKIM
		MessageData<T_FP_GLOG_CONNECT_USER_LOGIN> packet2 { };
		util::strncpy(packet2->AccountName, pCurrentAccountInfo->AccountName, SIZE_MAX_ACCOUNT_NAME);
		serverGroup->SendMessageToFieldServer(packet2);
#endif
		// end 2011-12-12 by hskim, GLog 2차
	}
	else
	{
		MessageData<T_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT> packet2;
		util::strncpy(packet2->AccountName, pRecvMsgLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);
		packet2->PCConnectLoginOK		= *packet.data();
		serverGroup->SendMessageToFieldServer(packet2);
	}
	return TRUE;
}


void CPreIOCPSocket::SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1, int errParam2, const char* errMsg, BOOL bCloseConnection)
{
	MSG_ERROR	*pMsgError;
	BYTE		SendBuf[SIZE_MAX_PACKET];

	char szSystemLog[256];
	sprintf(szSystemLog, "  SendErrorMsg to ClientIndex[%3d] %15s(%4d) ==> %s(%#04x) AT %s Param1(%d) Param2(%d) Msg(%s)\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort,
		GetErrorString(err), err, GetProtocolTypeString(msgType), errParam1, errParam2, errMsg);
	g_pGlobal->WriteSystemLog(szSystemLog);
#ifdef _DEBUG
//	if (err != ERR_PROTOCOL_INVALID_MAP_EVENT_INFO)
	{
		DBGOUT(szSystemLog);
	}
#endif

	*(MessageType_t*)SendBuf = T_ERROR;
	pMsgError = (MSG_ERROR*)(SendBuf+SIZE_FIELD_TYPE_HEADER);
	pMsgError->CloseConnection = bCloseConnection;
	pMsgError->ErrorCode = err;
	pMsgError->MsgType = msgType;
	pMsgError->ErrParam1 = errParam1;
	pMsgError->ErrParam2 = errParam2;
	if (errMsg == NULL || 0 == strncmp(errMsg, "", SIZE_MAX_ERROR_STRING))
	{
		pMsgError->StringLength = 0;
	}
	else
	{
		pMsgError->StringLength = (USHORT)(strlen(errMsg) + 1);
		util::strncpy((char*)SendBuf + MSG_SIZE(MSG_ERROR), errMsg, SIZE_MAX_ERROR_STRING);
	}
	SendAddData(SendBuf, MSG_SIZE(MSG_ERROR) + pMsgError->StringLength);
}
