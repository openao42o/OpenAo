// IMIOCPSocket.cpp: implementation of the CIMIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMIOCPSocket.h"
#include "IMIOCP.h"
#include "IMGlobal.h"
#include "AtumIMDBManager.h"
#include "AtumProtocolVoIP.h"
#include "AtumSJ.h"
#include "AtumError.h"
#include "IMParty.h"
//#include "SecurityManager.h" // 2011-06-22 by hskim, 사설 서버 방지

#include "FreeWar.h" // 2015-11-02 PanKJ

CIMIOCP			*CIMIOCPSocket::ms_pIMIOCP	= NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMIOCPSocket::CIMIOCPSocket()
{
	m_nLoginType		= -1;

	m_PeerSocketType	= ST_INVALID_TYPE;
	this->SetIMParty(NULL);

	// 길드 관련
	this->SetGuildUniqueNumber(INVALID_GUILD_UID);

	this->SetCurrentFieldServerSocket(NULL);
	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	m_enableChatFlag8		= (BitFlag8_t)0xFF;
	m_enableChatFlag32 = (BitFlag32_t)0xFFFF;
	m_bSendMessgeTypeDBG	= FALSE;

	memset(&m_character, 0x00, sizeof(CHARACTER));
	memset(&m_MembershipInfo, 0x00, sizeof(MSG_FI_CASH_PREMIUM_CARD_INFO));
	
	m_mtvectorFriendList.reserve(10);
	m_mtvectorRejectList.reserve(10);
	m_nEachOtherFriendCnts	= 0;

	m_VoipType				= CHECK_TYPE_VOIP_NONE; // 2008-06-17 by dhjin, EP3 VOIP -
	m_bChatRoomUseing		= FALSE;				// 2008-06-17 by dhjin, EP3 채팅방 - 채팅방 참여 중인지에 대한 값
	m_nChatRoomNum			= 0;					// 2008-06-18 by dhjin, EP3 채팅방 - 채팅방 번호
	m_bArenaIMServerCheck	= FALSE;				// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크
}

CIMIOCPSocket::~CIMIOCPSocket()
{
}

BOOL CIMIOCPSocket::OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	int				nBytesUsed		= 0;
	MessageType_t	nRecvType		= 0;
	int				nRecvTypeSize	= 0;
	ProcessResult	procRes			= RES_BREAK;
	MessageType_t	prevMsgType		= 0;

	// UDP 처리 루틴
	if(m_bUDPFlag == TRUE)
	{
		if(strncmp(m_szPeerIP, pPeerIP, SIZE_MAX_IPADDRESS) != 0 || m_nPeerPort != nPeerPort)
		{
			// Protocl Error 처리
			// - UDP로 받은 패킷의 IP와 Port가 서버에 설정된 Client의 IP, Port와 같지 않다
			// Error Code : ERR_PROTOCOL_INVALID_PEER
			SendErrorMessage(T_IM_IOCP, ERR_PROTOCOL_INVALID_PEER);
			Close(0x12000);
		}

		return TRUE;
	}

	while(this->IsUsing() && nBytesUsed < nLength)
	{
		nRecvType			= 0;

		nRecvType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

		if(i_pThreadInfo)
		{
			i_pThreadInfo->dwSocketIndex = m_nClientArrayIndex;
			i_pThreadInfo->dwMessageType = nRecvType;
			i_pThreadInfo->nParam1			= 0;			// 2007-02-21 by cmkwon
			i_pThreadInfo->nParam2			= 0;			// 2007-02-21 by cmkwon
		}

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nRecvType, m_szPeerIP, ST_IM_SERVER, GGetexchangeMsgPrintLevel());
#endif

#ifdef _DEBUG
		// 메세지 찍기
		if (m_PeerSocketType == ST_CLIENT_TYPE
			&& m_bSendMessgeTypeDBG && T0_IC_STRING != HIBYTE(nRecvType)
			&& !IS_ALIVE_MSG(nRecvType) && T0_IC_ADMIN != HIBYTE(nRecvType)
		)
		{
			SendString128(STRING_128_USER_NOTICE, "[C->I]%s", GetProtocolTypeString(nRecvType));
		}
#endif

#ifdef S_EP4_DEBUG_HSKIM
// start 2011-10-10 by hskim, EP4 [버그 디버깅]
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[Debug Info] Type(%s) IP(%15s)\r\n", GetProtocolTypeString(nRecvType), this->GetPeerIP());
// end 2011-10-10 by hskim, EP4 [버그 디버깅]
#endif	

		switch(nRecvType)
		{
		////////////////////////////////////////////////////////////////////////
		// IMServer <-- Client, IMServer <-- Monitor, IMServer <-- FieldServer
		case T_IC_CONNECT_ALIVE:
		case T_FI_CONNECT_ALIVE:
		case T_PM_CONNECT_ALIVE:
		case T_IM_CONNECT_ALIVE:
		case T_FM_CONNECT_ALIVE:
		case T_NM_CONNECT_ALIVE:
			OnRecvdAlivePacket();
			procRes = RES_RETURN_TRUE;
			break;
		//////////////////////////////////////////////////////////////////
		// IMServer <-- AdminTool
		case T_IA_ADMIN_CONNECT:
			{
				MSG_IA_ADMIN_CONNECT* msgAdminConnect = (MSG_IA_ADMIN_CONNECT*)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_IA_ADMIN_CONNECT);

				// 인증하기
				INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_CONNECT_OK, T_IA_ADMIN_CONNECT_OK, msgAdminConnectOK, SendBuf);
// 2006-04-15 by cmkwon
//				msgAdminConnectOK->AuthOK = bRet;
				msgAdminConnectOK->AccountType0 = g_pGlobal->AuthAdminToolUser(msgAdminConnect->UID, msgAdminConnect->PWD);
				SendAddData(SendBuf, MSG_SIZE(MSG_IA_ADMIN_CONNECT_OK));

				if (0 == msgAdminConnectOK->AccountType0)
				{
					procRes = RES_RETURN_FALSE;
					Close();
				}
				else
				{
					// 인증 성공
					m_PeerSocketType = ST_ADMIN_TOOL;
					util::strncpy(m_character.AccountName, msgAdminConnect->UID, SIZE_MAX_ACCOUNT_NAME);	// 2007-10-02 by cmkwon, 수정
					m_character.ClientIndex		= this->GetClientArrayIndex();								// 2007-10-02 by cmkwon, 수정

					ms_pIMIOCP->InsertMonitorIOCPSocketPtr(this);
				}
			}
			break;
		case T_IA_ADMIN_GET_CHARACTER:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IA_ADMIN_GET_CHARACTER Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				MSG_IA_ADMIN_GET_CHARACTER* msgAdminGetCharac = (MSG_IA_ADMIN_GET_CHARACTER*)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_IA_ADMIN_GET_CHARACTER);

				CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(msgAdminGetCharac->CharacterName);

				INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_GET_CHARACTER_OK, T_IA_ADMIN_GET_CHARACTER_OK, msgAdminGetCharacOK, SendBuf);
				if (pSocket == NULL)
				{
					// 캐릭터 없음
					msgAdminGetCharacOK->IsOnline = FALSE;
				}
				else
				{
					// 캐릭터 있음
					msgAdminGetCharacOK->IsOnline = TRUE;
				}
				SendAddData(SendBuf, MSG_SIZE(MSG_IA_ADMIN_GET_CHARACTER_OK));
			}
			break;
		case T_IA_ADMIN_CHAT_ALL:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IA_ADMIN_CHAT_ALL Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				MSG_IA_ADMIN_CHAT_ALL* msgChatAll = (MSG_IA_ADMIN_CHAT_ALL*)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_IA_ADMIN_CHAT_ALL) + msgChatAll->MessageLength;

				if(ms_pIMIOCP->m_bArenaServerCheck)
				{// 2008-04-28 by dhjin, 아레나 통합 - 아레나 서버에서는 전체 공지를 보내지 않는다. 메인서버에서 보내기 때문에 2번 찍한다.
					break;
				}

				char *chatString = ((char*)(msgChatAll))+sizeof(MSG_IA_ADMIN_CHAT_ALL);
				if (strncmp(chatString, "/", 1) == 0)
				{
					// 2007-10-02 by cmkwon, COMMAND 로그 남기기

					if (g_pIMGlobal->CheckWriteLog(chatString))		// 2011-06-22 by hskim, 사설 서버 방지
					g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] : %s, IP(%s), command(%s)\r\n", GetCharacterString(&m_character, string()), this->GetPeerIP(), chatString);

					// 명령어임
					BOOL ret = HandleAdminCommands(chatString);
					break;
				}

				// set message type
				SET_MSG_TYPE(((byte*)msgChatAll) - SIZE_FIELD_TYPE_HEADER, T_IC_CHAT_ALL);

				// send msg to all clients
				ms_pIMIOCP->SendMessageToAllClients(((BYTE*)msgChatAll) - SIZE_FIELD_TYPE_HEADER, MSG_SIZE(MSG_IC_CHAT_ALL) + msgChatAll->MessageLength);
			}
			break;
		case T_IA_ADMIN_GET_SERVER_INFO:
			{
				
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IA_ADMIN_GET_SERVER_INFO Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				INIT_MSG_WITH_BUFFER(MSG_IA_ADMIN_GET_SERVER_INFO_OK, T_IA_ADMIN_GET_SERVER_INFO_OK, GetServerInfoOk, GetServerInfoOkBuf);
				GetServerInfoOk->UserCount = ms_pIMIOCP->m_MapChactarterUniqueNumber.size();
				GetServerInfoOk->ServerState = SERVER_STATE_NORMAL;
				SendAddData(GetServerInfoOkBuf, MSG_SIZE(MSG_IA_ADMIN_GET_SERVER_INFO_OK));
			}
			break;
		//////////////////////////////////////////////////////////////////
		// IMServer <-- Monitor
		case T_IM_CONNECT:
			{
				// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 아래와 같이 수정 함
				//if(FALSE == IS_SCADMINTOOL_CONNECTABLE_IP(GetPeerIP()))
				if(FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))	// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
				{
					return FALSE;
				}
				char szSystemLog[256];
				sprintf(szSystemLog, "Monitor Client Connected, IP[%s]\r\n", GetPeerIP());
				DBGOUT(szSystemLog);
				g_pGlobal->WriteSystemLog(szSystemLog);


				m_PeerSocketType = ST_MONITOR_SERVER;
				ms_pIMIOCP->InsertMonitorIOCPSocketPtr(this);

				INIT_MSG_WITH_BUFFER(MSG_IM_CONNECT_OK, T_IM_CONNECT_OK, pSendConnectOK, SendBuf);
				util::strncpy(pSendConnectOK->IPAddress, g_pIMGlobal->GetPublicIPLocal(), SIZE_MAX_IPADDRESS);
				pSendConnectOK->NumOfClients = ms_pIMIOCP->m_MapAccountUniqueNumber.size();
				pSendConnectOK->Port = ms_pIMIOCP->GetListenerPort();
				pSendConnectOK->CalcBandwidth = ms_pIMIOCP->GetFlagCalcTrafficInfo();
				pSendConnectOK->StartedTime = ms_pIMIOCP->m_dwTimeStarted;
				util::strncpy(pSendConnectOK->ServerName, "IM Server", SIZE_MAX_SERVER_NAME);
				SendAddData(SendBuf, MSG_SIZE(MSG_IM_CONNECT_OK));

				INIT_MSG(MSG_IM_GET_TOTAL_USER_COUNTS_ACK, T_IM_GET_TOTAL_USER_COUNTS_ACK, pSeAck, SendBuf);
				pSeAck->AccumulatedTotalUserCounts	= ms_pIMIOCP->m_uiIMAccumulatedTotalUserCounts;
				pSeAck->CurrentTotalUserCounts		= ms_pIMIOCP->m_MapAccountUniqueNumber.size();
				pSeAck->MaxTotalUserCounts			= ms_pIMIOCP->m_uiIMMaxTotalUserCounts;				
				SendAddData(SendBuf, MSG_SIZE(MSG_IM_GET_TOTAL_USER_COUNTS_ACK));
			}
			break;
		case T_IM_GET_NUM_CLIENTS:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IM_GET_NUM_CLIENTS Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				INIT_MSG_WITH_BUFFER(MSG_IM_GET_NUM_CLIENTS_OK, T_IM_GET_NUM_CLIENTS_OK, pSendGetNumClientsOK, SendBuf);
				pSendGetNumClientsOK->NumOfClients = ms_pIMIOCP->m_MapChactarterUniqueNumber.size();
				SendAddData(SendBuf, MSG_SIZE(MSG_IM_GET_NUM_CLIENTS_OK));
			}
			break;
		case T_IM_SHUTDOWN:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IM_SHUTDOWN Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				SendAddMessageType(T_IM_SHUTDOWN_OK);
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - IM서버로 주는 명령
				// PostMessage(g_pGlobal->GetMainWndHandle(), WM_CLOSE, 0, 0);
			}
			break;
		case T_IM_PING:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IM_PING Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				SendAddMessageType(T_IM_PING_OK);
			}
			break;
		case T_IM_PRINTMAP:
			{
			}
			break;
		case T_IM_PRINT_DEBUG_MSG:
			procRes = Process_IM_PRINT_DEBUG_MSG(pPacket, nLength, nBytesUsed);
			break;
		case T_IM_CHANGE_BANDWIDTH_FLAG:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IM_CHANGE_BANDWIDTH_FLAG Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				ms_pIMIOCP->SetFlagCalcTrafficInfo(!ms_pIMIOCP->GetFlagCalcTrafficInfo());

				INIT_MSG_WITH_BUFFER(MSG_IM_CHANGE_BANDWIDTH_FLAG_OK, T_IM_CHANGE_BANDWIDTH_FLAG_OK, pChangeFlag, SendBuf);
				pChangeFlag->bChagedFlag = ms_pIMIOCP->GetFlagCalcTrafficInfo();
				SendAddData(SendBuf, MSG_SIZE(MSG_IM_CHANGE_BANDWIDTH_FLAG_OK));
			}
			break;
		case T_IM_SET_MSG_PRINT_LEVEL:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IM_SET_MSG_PRINT_LEVEL Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				BYTE tmpLevel = *(BYTE*)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_IM_SET_MSG_PRINT_LEVEL);

				// set msg print level
				GSetexchangeMsgPrintLevel(tmpLevel);
			}
			break;
		case T_IM_GET_TOTAL_USER_COUNTS:
			{
				// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				if(FALSE==g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
				{
					g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect T_IM_GET_TOTAL_USER_COUNTS Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
					break;
				}
				// end 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 등록된 IP가 아니면 처리안되도록 한다.
				
				INIT_MSG_WITH_BUFFER(MSG_IM_GET_TOTAL_USER_COUNTS_ACK, T_IM_GET_TOTAL_USER_COUNTS_ACK, pSeAck, SendBuf);
				pSeAck->AccumulatedTotalUserCounts	= ms_pIMIOCP->m_uiIMAccumulatedTotalUserCounts;
				pSeAck->CurrentTotalUserCounts		= ms_pIMIOCP->m_MapAccountUniqueNumber.size();
				pSeAck->MaxTotalUserCounts			= ms_pIMIOCP->m_uiIMMaxTotalUserCounts;
				
				ms_pIMIOCP->SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_IM_GET_TOTAL_USER_COUNTS_ACK));
			}
			break;

		//////////////////////////////////////////////////////////////////
		// IMServer <-- Client, IMServer <-- FieldServer
		case T_IC_CONNECT_LOGIN:
			//로그인
			// Client로부터 AccountName, ServerName, CharacterName, Password(MD5),LoginType(게임중,메신저접속)을 받는다.
			// 접속중인 ID라면 이전 접속자를 끊는다.
			// DB로부터 인증을 요청한다.
			procRes = Process_IC_CONNECT_LOGIN(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_MAP:// 지역 채팅
			procRes = Process_IC_CHAT_MAP(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_REGION:// 지역 채팅
			procRes = Process_IC_CHAT_REGION(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_PARTY:// 파티 채팅
			procRes = Process_IC_CHAT_PARTY(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_GUILD://길드 채팅
			procRes = Process_IC_CHAT_GUILD(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_ALL:// 전체 채팅
			procRes = Process_IC_CHAT_ALL(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_PTOP:
			// 1:1 채팅
			procRes = Process_IC_CHAT_PTOP(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_CHANGE_CHAT_FLAG:	// 채팅 설정
			procRes = Process_IC_CHAT_CHANGE_CHAT_FLAG(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_SELL_ALL:
			procRes = Process_IC_CHAT_SELL_ALL(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_CASH_ALL:
			procRes = Process_IC_CHAT_CASH_ALL(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_INFLUENCE_ALL:		// 2006-04-21 by cmkwon
			procRes = Process_IC_CHAT_INFLUENCE_ALL(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_ARENA:				// 2007-05-02 by dhjin
			procRes = Process_IC_CHAT_ARENA(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_WAR:				// 2008-05-19 by dhjin, EP3 - 채팅 시스템 변경, 전쟁 채팅
			procRes = Process_IC_CHAT_WAR(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_CNC:		// 2014-08-03 Future, adding Cross Nation Chat
			procRes = Process_IC_CHAT_CNC(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_CHATROOM:				// 2008-06-18 by dhjin, EP3 채팅방 - 
			procRes = Process_IC_CHAT_CHATROOM(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;
		case T_IC_CHAT_INFINITY:			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 채팅
			procRes = Process_IC_CHAT_INFINITY(pPacket, nLength, nBytesUsed, i_pThreadInfo);
			break;	
		case T_IC_CHAT_FRIENDLIST_AND_REJECTLIST_LOADING:
			procRes = Process_IC_CHAT_FRIENDLIST_AND_REJECTLIST_LOADING(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_FRIENDLIST_INSERT:
			procRes = Process_IC_CHAT_FRIENDLIST_INSERT(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_FRIENDLIST_DELETE:
			procRes = Process_IC_CHAT_FRIENDLIST_DELETE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_FRIENDLIST_REFRESH:
			procRes = Process_IC_CHAT_FRIENDLIST_REFRESH(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_REJECTLIST_INSERT:
			procRes = Process_IC_CHAT_REJECTLIST_INSERT(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHAT_REJECTLIST_DELETE:
			procRes = Process_IC_CHAT_REJECTLIST_DELETE(pPacket, nLength, nBytesUsed);
			break;

		case T_IC_CHAT_GET_GUILD:// 길드원 정보 요청
		// 길드이름으로 DB에서 길드원 전체를 받아와서
		// 접속자,비접속자,메신저접속자들에게 전송한다.
			{
			}
			break;
		case T_IC_CHAT_CHANGE_GUILD_OK://길드원 정보 바뀜
		// 길드원 정보 바뀐 내용을 받는다.
		// 길드원 전체에게 전송한다.
			{
			}
			break;
		case T_IC_CHAT_POSITION:
		// 좌표 정보를 받는다.
		// (지역 채팅에 사용된다.)
			{
			}
			break;
		case T_IC_PARTY_CREATE:
			procRes = Process_IC_PARTY_CREATE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_GET_MEMBER:
			procRes = Process_IC_PARTY_GET_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_GET_ALL_MEMBER:
			procRes = Process_IC_PARTY_GET_ALL_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_LEAVE:
			procRes = Process_IC_PARTY_LEAVE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_TRANSFER_MASTER:
			procRes = Process_IC_PARTY_TRANSFER_MASTER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_BAN_MEMBER:
			procRes = Process_IC_PARTY_BAN_MEMBER(pPacket, nLength, nBytesUsed);
			break;
// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 사용하지 않는 프로토콜임
//		case T_IC_PARTY_DISMEMBER:
//			procRes = Process_IC_PARTY_DISMEMBER(pPacket, nLength, nBytesUsed);
//			break;
		case T_IC_PARTY_CHANGE_FLIGHT_FORMATION:
			procRes = Process_IC_PARTY_CHANGE_FLIGHT_FORMATION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_GET_FLIGHT_POSITION:
			procRes = Process_IC_PARTY_GET_FLIGHT_POSITION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_CHANGE_FLIGHT_POSITION:
			procRes = Process_IC_PARTY_CHANGE_FLIGHT_POSITION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_CANCEL_FLIGHT_POSITION:
			procRes = Process_IC_PARTY_CANCEL_FLIGHT_POSITION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_ALL_FLIGHT_POSITION:
			procRes = Process_IC_PARTY_ALL_FLIGHT_POSITION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M:		// 2008-02-28 by dhjin, 아레나 통합 - 
			procRes = Process_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_PARTY_LEAVE_FROM_M_TO_A:					// 2008-02-28 by dhjin, 아레나 통합 -
			procRes = Process_IC_PARTY_LEAVE_FROM_M_TO_A(pPacket, nLength, nBytesUsed);
			break;			
		case T_IC_PARTY_LEAVE_FROM_A_TO_M:					// 2008-02-28 by dhjin, 아레나 통합 -
			procRes = Process_IC_PARTY_LEAVE_FROM_A_TO_M(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_PARTY_LIST_INFO:					// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보 리스트 요청
			procRes = Process_IC_PARTY_LIST_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_JOIN_FREE:					// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 자유 참여
			procRes = Process_IC_PARTY_JOIN_FREE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_PARTY_CHANGE_INFO:				// 2008-06-04 by dhjin, EP3 편대 수정 - 편대 정보 수정
			procRes = Process_IC_PARTY_CHANGE_INFO(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_PARTY_RECOMMENDATION_MEMBER:				// 2008-06-04 by dhjin, EP3 편대 수정 - 추천 케릭터 요청
			procRes = Process_IC_PARTY_RECOMMENDATION_MEMBER(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_PARTY_GET_AUTO_PARTY_INFO:				// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대
			procRes = Process_IC_PARTY_GET_AUTO_PARTY_INFO(pPacket, nLength, nBytesUsed);
			break;	
			
		///////////////////////////////////////////////////////////////////////
		// IC_GUILD
		case T_IC_GUILD_CREATE:
			procRes = Process_IC_GUILD_CREATE(pPacket, nLength, nBytesUsed);
			break;
// check: T_IC_GUILD_GET_GUILD_INFO로 통합, 필요하면 살림, 20040520, kelovon
//		case T_IC_GUILD_GET_MEMBER:
//			procRes = Process_IC_GUILD_GET_MEMBER(pPacket, nLength, nBytesUsed);
//			break;
		case T_IC_GUILD_GET_GUILD_INFO:
			procRes = Process_IC_GUILD_GET_GUILD_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_REQUEST_INVITE:
			procRes = Process_IC_GUILD_REQUEST_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_ACCEPT_INVITE:
			procRes = Process_IC_GUILD_ACCEPT_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_REJECT_INVITE:
			procRes = Process_IC_GUILD_REJECT_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_LEAVE:
			procRes = Process_IC_GUILD_LEAVE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_BAN_MEMBER:
			procRes = Process_IC_GUILD_BAN_MEMBER(pPacket, nLength, nBytesUsed);
			break;
// 2007-11-12 by dhjin, Process_IC_GUILD_DISMEMBER  이름은 나두고 T_IC_GUILD_DISMEMBER만 변경
//		case T_IC_GUILD_DISMEMBER:
		case T_FI_GUILD_DISMEMBER:
			procRes = Process_IC_GUILD_DISMEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_CANCEL_DISMEMBER:
			procRes = Process_IC_GUILD_CANCEL_DISMEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_GET_DISMEMBER_DATE:
			procRes = Process_IC_GUILD_GET_DISMEMBER_DATE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_CHANGE_GUILD_NAME:
			procRes = Process_IC_GUILD_CHANGE_GUILD_NAME(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_GET_GUILD_MARK:
			procRes = Process_IC_GUILD_GET_GUILD_MARK(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_SET_GUILD_MARK:
			procRes = Process_IC_GUILD_SET_GUILD_MARK(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_SET_RANK:
			procRes = Process_IC_GUILD_SET_RANK(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_GET_OTHER_GUILD_INFO:
			procRes = Process_IC_GUILD_GET_OTHER_GUILD_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_SURRENDER_GUILD_WAR:
			procRes = Process_IC_GUILD_SURRENDER_GUILD_WAR(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_NEW_COMMANDER:		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
			procRes = Process_IC_GUILD_NEW_COMMANDER(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_GUILD_NOTICE_WRITE:		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
			procRes = Process_IC_GUILD_NOTICE_WRITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_GET_APPLICANT:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리
			procRes = Process_IC_GUILD_GET_APPLICANT(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_GUILD_GET_INTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
			procRes = Process_IC_GUILD_GET_INTRODUCTION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_GET_SELF_INTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 
			procRes = Process_IC_GUILD_GET_SELF_INTRODUCTION(pPacket, nLength, nBytesUsed);
			break;	
		case T_IC_GUILD_SEARCH_INTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
			procRes = Process_IC_GUILD_SEARCH_INTRODUCTION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_UPDATE_INTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 작성 
			procRes = Process_IC_GUILD_UPDATE_INTRODUCTION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_DELETE_INTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 지우기
			procRes = Process_IC_GUILD_DELETE_INTRODUCTION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_UPDATE_SELFINTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
			procRes = Process_IC_GUILD_UPDATE_SELFINTRODUCTION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_DELETE_SELFINTRODUCTION:		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기  
			procRes = Process_IC_GUILD_DELETE_SELFINTRODUCTION(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_APPLICANT_INVITE:				// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대
			procRes = Process_IC_GUILD_APPLICANT_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_GUILD_APPLICANT_REJECT_INVITE:		// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 거부 
			procRes = Process_IC_GUILD_APPLICANT_REJECT_INVITE(pPacket, nLength, nBytesUsed);
			break;
		///////////////////////////////////////////////////////////////////////
		// Admin
		case T_IC_ADMIN_CALL_CHARACTER:
			procRes = Process_IC_ADMIN_CALL_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_ADMIN_CALL_GUILD:
			procRes = Process_IC_ADMIN_CALL_GUILD(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_ADMIN_GET_SERVER_STAT:
			procRes = Process_IC_ADMIN_GET_SERVER_STAT(pPacket, nLength, nBytesUsed);
			break;
		///////////////////////////////////////////////////////////////////////
		// Countdown
		case T_IC_COUNTDOWN_DONE:
			procRes = Process_IC_COUNTDOWN_DONE(pPacket, nLength, nBytesUsed);
			break;
			
		//case T_IC_VOIP_SET:			// C -> I, 2008-06-17 by dhjin, EP3 VOIP - 설정 
		//	procRes = Process_IC_VOIP_SET(pPacket, nLength, nBytesUsed);
		//	break;

		// T0_IC_CHATROOM
		case T_IC_CHATROOM_CREATE:	// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 생성
			procRes = Process_IC_CHATROOM_CREATE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_LIST_INFO:		// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 목록 가져오기		
			procRes = Process_IC_CHATROOM_LIST_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_REQUEST_INVITE:		// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 요청 	
			procRes = Process_IC_CHATROOM_REQUEST_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_JOIN:				// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 참여	
			procRes = Process_IC_CHATROOM_JOIN(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_ACCEPT_INVITE:		// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 수락	
			procRes = Process_IC_CHATROOM_ACCEPT_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_REJECT_INVITE:		// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 초대 거절	
			procRes = Process_IC_CHATROOM_REJECT_INVITE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_LEAVE:				// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 나가기	
			procRes = Process_IC_CHATROOM_LEAVE(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_BAN:					// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 추방	
			procRes = Process_IC_CHATROOM_BAN(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_CHANGE_NAME:			// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 이름 변경	
			procRes = Process_IC_CHATROOM_CHANGE_NAME(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_CHANGE_MASTER:		// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 방장 변경	
			procRes = Process_IC_CHATROOM_CHANGE_MASTER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_CHANGE_LOCK_PW:		// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 비밀번호 변경	
			procRes = Process_IC_CHATROOM_CHANGE_LOCK_PW(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_CHANGE_MAX_MEMBER:	// C -> I, 2008-06-16 by dhjin, EP3 채팅방 - 채팅방 인원수 변경	
			procRes = Process_IC_CHATROOM_CHANGE_MAX_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_CHATROOM_OTHER_MEMBER_INFO:			// I -> C, 2008-06-25 by dhjin, EP3 채팅방 - 채팅방 다른 맴버 정보 전송
			procRes = Process_IC_CHATROOM_OTHER_MEMBER_INFO(pPacket, nLength, nBytesUsed);
			break;
		
		// 2016-07-01 panoskj let's remove VOIP !!! HAYO
		// VOIP
		/*case T_IC_VOIP_GET_ACCOUNTUNIQUENUMBER:
			procRes = Process_IC_VOIP_GET_ACCOUNTUNIQUENUMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_1to1_DIAL:
			procRes = Process_IC_VOIP_1to1_DIAL(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_1to1_DIAL_OK:
			procRes = Process_IC_VOIP_1to1_DIAL_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_1to1_END:
			procRes = Process_IC_VOIP_1to1_END(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_NtoN_CREATEROOM_REQUEST:
			procRes = Process_IC_VOIP_NtoN_CREATEROOM_REQUEST(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_NtoN_INVITE_ALL:
			procRes = Process_IC_VOIP_NtoN_INVITE_ALL(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_NtoN_INVITE_USER:
			procRes = Process_IC_VOIP_NtoN_INVITE_USER(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_NtoN_GOOUT_ALL:
			procRes = Process_IC_VOIP_NtoN_GOOUT_ALL(pPacket, nLength, nBytesUsed);
			break;
		case T_IC_VOIP_ERROR:
			procRes = Process_IC_VOIP_ERROR(pPacket, nLength, nBytesUsed);
			break;*/
		///////////////////////////////////////////////////////////////////////
		// Field Server
		case T_FI_CONNECT:
			procRes = Process_FI_CONNECT(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CONNECT_NOTIFY_FIELDSERVER_IP:
			procRes = Process_FI_CONNECT_NOTIFY_FIELDSERVER_IP(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CONNECT_NOTIFY_GAMEEND:
			procRes = Process_FI_CONNECT_NOTIFY_GAMEEND(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CONNECT_NOTIFY_GAMESTART:
			procRes = Process_FI_CONNECT_NOTIFY_GAMESTART(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CONNECT_NOTIFY_DEAD_GAMESTART:
			procRes = Process_FI_CONNECT_NOTIFY_DEAD_GAMESTART(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_EVENT_NOTIFY_WARP:
			procRes = Process_FI_EVENT_NOTIFY_WARP(pPacket, nLength, nBytesUsed);
			break;
// 2004-10-29 by cmkwon, FieldServer는 하나로 픽스한다
//		case T_FI_EVENT_GET_WARP_INFO:
//			procRes = Process_FI_EVENT_GET_WARP_INFO(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_EVENT_CHAT_BLOCK:		// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 
			procRes = Process_FI_EVENT_CHAT_BLOCK(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_CONNECT_NOTIFY_DEAD:
			procRes = Process_FI_CONNECT_NOTIFY_DEAD(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_ACCEPT_INVITE_OK:
			procRes = Process_FI_PARTY_ACCEPT_INVITE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_ARENA_TEAM_NUM:		// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크할 팀번호 설정
			procRes = Process_FI_PARTY_ARENA_TEAM_NUM(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_UPDATE_ITEM_POS:
			procRes = Process_FI_PARTY_UPDATE_ITEM_POS(pPacket, nLength, nBytesUsed);
			break;
//		case T_FI_PARTY_GET_PARTY_INFO:
//			procRes = Process_FI_PARTY_GET_PARTY_INFO(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_PARTY_AUTO_CREATE:	// 2009-09-09 ~ 2010-01-26 by dhjin, 인피니티 - 자동 편대 
			procRes = Process_FI_PARTY_AUTO_CREATE(pPacket, nLength, nBytesUsed);
			break;	
		case T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK:	// 2010-03-18 by cmkwon, 몬스터변신 구현 - 
			procRes = Process_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK(pPacket, nLength, nBytesUsed);
			break;	

		case T_FI_CHARACTER_DELETE_CHARACTER:
			procRes = Process_FI_CHARACTER_DELETE_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHARACTER_CHANGE_LEVEL:
			procRes = Process_FI_CHARACTER_CHANGE_LEVEL(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHARACTER_UPDATE_MAP_CHANNEL:
			procRes = Process_FI_CHARACTER_UPDATE_MAP_CHANNEL(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHARACTER_CHANGE_INFLUENCE_TYPE:
			procRes = Process_FI_CHARACTER_CHANGE_INFLUENCE_TYPE(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_UPDATE_SUBLEADER:			// 2007-02-14 by dhjin
			procRes = Process_FI_UPDATE_SUBLEADER(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_GUILD_NOTIFY_START_WAR:
			procRes = Process_FI_GUILD_NOTIFY_START_WAR(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_DELETE_GUILD:
			procRes = Process_FI_GUILD_DELETE_GUILD(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_ADD_GUILD_FAME:
			procRes = Process_FI_GUILD_ADD_GUILD_FAME(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_OUTPOST:	 // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 전진기지 관련
			procRes = Process_FI_GUILD_OUTPOST(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_CASH_USING_GUILD:
			procRes = Process_FI_CASH_USING_GUILD(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CASH_USING_CHANGE_CHARACTERNAME:
			procRes = Process_FI_CASH_USING_CHANGE_CHARACTERNAME(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CASH_PREMIUM_CARD_INFO:
			procRes = Process_FI_CASH_PREMIUM_CARD_INFO(pPacket, nLength, nBytesUsed);
			break;
// 2004-12-09 by cmkwon, 여단전의 종료는 IMServer에서만 발생한다. 그러므로 알림은 IMServer -> Field로 발생한다
//		case T_FI_GUILD_NOTIFY_END_WAR:
//			procRes = Process_FI_GUILD_NOTIFY_END_WAR(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_ADMIN_GET_CHARACTER_INFO_OK:
			procRes = Process_FI_ADMIN_GET_CHARACTER_INFO_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GET_FIELD_USER_COUNTS_ACK:
			procRes = Process_FI_GET_FIELD_USER_COUNTS_ACK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_INFO_DECLARATION_MSWAR_SET_OK:	// 2009-01-12 by dhjin, 선전 포고 - 선전포고 시간 및 포기 설정 F->I
			procRes = Process_FI_INFO_DECLARATION_MSWAR_SET_OK(pPacket, nLength, nBytesUsed);
			break;	
		case T_FI_CHAT_CNC:
			procRes = Process_FI_CHAT_CNC(pPacket, nLength, nBytesUsed);
			break;

		case T_ERROR:
			{
				MSG_ERROR	*pMsgRecvErr;
				pMsgRecvErr = (MSG_ERROR*)(pPacket + nBytesUsed);
				nBytesUsed += (sizeof(MSG_ERROR) + pMsgRecvErr->StringLength);

				//  처리
				Err_t error = pMsgRecvErr->ErrorCode;

				g_pGlobal->WriteSystemLogEX(TRUE, "  T_ERROR %s(%#04X) received from %s[%s]\r\n",
					GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode, 
					GGetENServerTypeString(m_PeerSocketType), m_szPeerIP);
			}
			break;
		default:
			{
				// Protocol Error 처리(Close 처리함)
				// - Client로 부터 받은 Field Type이 유효하지 않다
				// Error Code : ERR_PROTOCOL_INVALID_PROTOCOL_TYPE
				SendErrorMessage(T_IM_IOCP, ERR_PROTOCOL_INVALID_PROTOCOL_TYPE, 0, 0, NULL, TRUE);

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CIMIOCPSocket::OnRecvdPacketIMServer invalid protocol type, RecvType[%s(%#04X)]\r\n", GetProtocolTypeString(nRecvType), nRecvType);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
		} // end - switch

		// 에러 관리용 이전 메세지 저장
		prevMsgType = nRecvType;

		// MSG 결과 처리
		if (procRes == RES_BREAK)
		{
			// 경미한 에러들. 연결을 끊지 않는다.
			// do nothing, 그냥 남아있는 다음 packet을 처리한다
		}
		else if (procRes == RES_PACKET_ERROR)
		{
			// 패킷이 손상된 경우. 남은 packet을 처리하지 않고 바로 리턴한다. 서버간 연결에만 사용.
			return TRUE;
		}
		else if (procRes == RES_RETURN_TRUE)
		{
			// 정상
			// do nothing, 그냥 남아있는 다음 packet을 처리한다
		}
		else if (procRes == RES_RETURN_FALSE)
		{
			return FALSE;
		}

//#ifdef _DEBUG
//		if (HIBYTE(nRecvType) == T0_IC_PARTY || nRecvType == T_FI_PARTY_ACCEPT_INVITE_OK)
//		{
//			char *dpm;
//			( (CIMIOCPSocket*)(ms_pIMIOCP->GetIOCPSocket(CLIENT_INDEX_START_NUM)) )->PRINT_DEBUG_MESSAGE(dpm);
//		}
//#endif
	}	// end - while

	return TRUE;
}

void CIMIOCPSocket::OnConnect(void)
{
	memset(&m_character, 0x00, sizeof(CHARACTER));

	char szSystemLog[256];
	sprintf(szSystemLog, "AtumIMServer Index[%3d] Client[%15s] Connected\r\n", this->GetClientArrayIndex(), m_szPeerIP);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	CIOCPSocket::OnConnect();	
	this->SetIMParty(NULL);

	// 길드 관련
	this->SetGuildUniqueNumber(INVALID_GUILD_UID);

	this->SetCurrentFieldServerSocket(NULL);
	m_PeerSocketType			= ST_INVALID_TYPE;
	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	m_enableChatFlag8			= (BitFlag8_t)0xFF;
	m_enableChatFlag32 = (BitFlag32_t)0xFFFF;
	m_bGameEndRoutineFlag		= TRUE;
	m_bFirstTimeLoadFriendList	= TRUE;		// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 
}

void CIMIOCPSocket::OnClose(int reason)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "AtumIMServer Index[%3d] SocketType[%d] MaxWriteBufCounts[%4d] ClientIP[%15s] Closed ==> reason %d[%#08X]\r\n"
		, this->GetClientArrayIndex(), m_PeerSocketType, m_nMaxWriteBufCounts, m_szPeerIP, reason, reason);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	if (m_PeerSocketType == ST_CLIENT_TYPE)
	{
		GameEndRoutineIM();
	}

	if (m_PeerSocketType == ST_FIELD_SERVER)
	{
		// 2009-03-19 by cmkwon, 시스템 로그 추가 - 
		g_pGlobal->WriteSystemLogEX(TRUE, "  [Notify] Socket for FieldServer was closed !! \r\n");

		ms_pIMIOCP->SetFieldServerSocket(NULL);
		ms_pIMIOCP->m_FieldServerInfo.Reset();
		ms_pIMIOCP->m_mCharacterName.clearLock();
		ms_pIMIOCP->m_MapChactarterUniqueNumber.clearLock();
		ms_pIMIOCP->m_mAccountName.clearLock();
		ms_pIMIOCP->m_MapAccountUniqueNumber.clearLock();
	}

	if(m_PeerSocketType == ST_MONITOR_SERVER)
	{
		ms_pIMIOCP->DeleteMonitorIOCPSocketPtr(this);
	}

	if(m_PeerSocketType == ST_ADMIN_TOOL)
	{
		ms_pIMIOCP->DeleteMonitorIOCPSocketPtr(this);
	}

	CIOCPSocket::OnClose(30);
}

CIMParty *CIMIOCPSocket::GetIMParty(void)
{
	CIMParty *ptParty = this->m_pIMParty;
	if(NULL == ptParty || FALSE == ptParty->IsValidIMParty())
	{
		this->SetIMParty(NULL);
	}

	return ptParty;
}

void CIMIOCPSocket::SetIMParty(CIMParty *i_pParty)
{
	this->m_pIMParty = i_pParty;
}

BOOL CIMIOCPSocket::ResAuthChatLogin(CHARACTER *pCharacter, int nLoginType, int nErr)
{
	BYTE	buffer[SIZE_MAX_PACKET];
	int		nUsedBytes	= 0;

	if(IsUsing() == FALSE){	return FALSE;}

	if(0 != nErr)
	{	// Error
		return FALSE;
	}

	m_nLoginType = nLoginType;
	memcpy(&m_character, pCharacter, sizeof(CHARACTER));	
	m_character.ClientIndex = this->GetClientArrayIndex();

	//////////////////////////////////////////////////////////////////////////
	// 2006-12-08 by dhjin, 지도자, 부지도자 설정
	ms_pIMIOCP->SetInflAllLeader(&m_character);

	memset(buffer, 0x00, SIZE_MAX_PACKET);
	*(MessageType_t*)buffer = T_IC_CONNECT_LOGIN_OK;
	nUsedBytes += SIZE_FIELD_TYPE_HEADER;
	SendAddData(buffer, nUsedBytes);

	/* check: 확인 필요
	if(m_nLoginType == CHAT_LOGIN_TYPE_MESSENGER)
	{
		memcpy(buffer+nUsedBytes, &m_character, sizeof(MSG_IC_CONNECT_LOGIN_OK));
		nUsedBytes += sizeof(MSG_IC_CONNECT_LOGIN_OK);

		SendAddData(buffer, nUsedBytes);
	}
	*/

	// CharacterName이 키값으로 Map에 세팅
	char tmCharacterName[SIZE_MAX_CHARACTER_NAME];
	ms_pIMIOCP->m_mCharacterName.insertLock(GGetLowerCase(tmCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME), this);
	ms_pIMIOCP->m_MapChactarterUniqueNumber.insertLock(m_character.CharacterUniqueNumber, this);
	ms_pIMIOCP->m_mAccountName.insertLock(m_character.AccountName, this);
	ms_pIMIOCP->m_MapAccountUniqueNumber.insertLock(m_character.AccountUniqueNumber, this);

	ms_pIMIOCP->m_uiIMAccumulatedTotalUserCounts++;
	if(ms_pIMIOCP->m_uiIMMaxTotalUserCounts < ms_pIMIOCP->m_MapChactarterUniqueNumber.size())
	{
		ms_pIMIOCP->m_uiIMMaxTotalUserCounts = ms_pIMIOCP->m_MapChactarterUniqueNumber.size();
	}

	return TRUE;
}

FRIENDINFO *CIMIOCPSocket::FindFriendInfoByCharacterName(char *i_szCharacterName)
{
	FRIENDINFO *pRet = NULL;
	if(0 == strcmp(i_szCharacterName, "")
		|| 0 == m_mtvectorFriendList.size())
	{
		return pRet;
	}

	m_mtvectorFriendList.lock();
	mtvectorFRIENDINFO::iterator	itr(m_mtvectorFriendList.begin());
	for(; itr != m_mtvectorFriendList.end(); itr++)
	{
		if(0 == strnicmp(itr->szCharacterName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME))
		{
			pRet = &*itr;
			break;
		}
	}
	m_mtvectorFriendList.unlock();
	
	return pRet;
}

mtvectorFRIENDINFO::iterator CIMIOCPSocket::FindFriendInfoByCharacterNameItr(char *i_szCharacterName)
{
	mtvectorFRIENDINFO::iterator pRet = m_mtvectorFriendList.end();
	if (0 == strcmp(i_szCharacterName, "")
		|| 0 == m_mtvectorFriendList.size())
	{
		return m_mtvectorFriendList.end();
	}

	m_mtvectorFriendList.lock();
	mtvectorFRIENDINFO::iterator	itr(m_mtvectorFriendList.begin());
	for (; itr != m_mtvectorFriendList.end(); itr++)
	{
		if (0 == strnicmp(itr->szCharacterName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME))
		{
			pRet = itr;
		}
	}
	m_mtvectorFriendList.unlock();

	return pRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::IsExistFriendList(char *i_szCharacterName)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::IsExistFriendList(char *i_szCharacterName)
{
	if(NULL == this->FindFriendInfoByCharacterName(i_szCharacterName))
	{
		return FALSE;
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::IncreaseOnlineEachOtherFriendCnts(char *i_szOnlineCharacterName)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-18 ~ 2006-07-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::IncreaseOnlineEachOtherFriendCnts(char *i_szOnlineCharacterName)
{
	if(0 == strcmp(i_szOnlineCharacterName, "")
		|| 0 == m_mtvectorFriendList.size())
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_mtvectorFriendList);
	FRIENDINFO *pTmp = this->FindFriendInfoByCharacterName(i_szOnlineCharacterName);
	if(NULL == pTmp)
	{
		return FALSE;
	}

	m_nEachOtherFriendCnts++;		// 2006-07-19 by cmkwon
	m_nEachOtherFriendCnts = min(m_nEachOtherFriendCnts, SIZE_MAX_FRIENDLIST_COUNT);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::DecreaseOnlineEachOtherFriendCnts(char *i_szOfflineCharacterName)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::DecreaseOnlineEachOtherFriendCnts(char *i_szOfflineCharacterName)
{
	if(0 == strcmp(i_szOfflineCharacterName, "")
		|| 0 == m_mtvectorFriendList.size())
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_mtvectorFriendList);
	FRIENDINFO *pTmp = this->FindFriendInfoByCharacterName(i_szOfflineCharacterName);
	if(NULL == pTmp)
	{
		return FALSE;
	}

	m_nEachOtherFriendCnts--;		// 2006-07-19 by cmkwon
	m_nEachOtherFriendCnts = max(m_nEachOtherFriendCnts, 0);
	return TRUE;
}

BOOL CIMIOCPSocket::InsertToFriendList(FRIENDINFO *i_pFriendInfo, BOOL i_bEachOtherFriend/*=FALSE*/)
{
	if(0 == strncmp(i_pFriendInfo->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_mtvectorFriendList);

	if(i_bEachOtherFriend
		&& i_pFriendInfo->byIsOnline)
	{// 2006-07-19 by cmkwon
		m_nEachOtherFriendCnts++;
		m_nEachOtherFriendCnts = min(m_nEachOtherFriendCnts, SIZE_MAX_FRIENDLIST_COUNT);
	}

	FRIENDINFO *pTmp = this->FindFriendInfoByCharacterName(i_pFriendInfo->szCharacterName);
	if(pTmp)
	{
		pTmp->byIsOnline = i_pFriendInfo->byIsOnline;
		return FALSE;
	}

	m_mtvectorFriendList.push_back(*i_pFriendInfo);
	
	return TRUE;
}

BOOL CIMIOCPSocket::DeleteFromFriendList(char *i_szCharacterName, BOOL i_bEachOtherFriend/*=FALSE*/)
{
	if(0 == strncmp(i_szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		return FALSE;
	}

	mt_auto_lock mtA(&m_mtvectorFriendList);

	mtvectorFRIENDINFO::iterator pTmpItr = this->FindFriendInfoByCharacterNameItr(i_szCharacterName);
	FRIENDINFO *pTmp = &*pTmpItr;
	if(NULL == pTmp)
	{
		return FALSE;
	}

	if(i_bEachOtherFriend
		&& pTmp->byIsOnline)
	{// 2006-07-19 by cmkwon
		m_nEachOtherFriendCnts--;
		m_nEachOtherFriendCnts = max(m_nEachOtherFriendCnts, 0);
	}

	m_mtvectorFriendList.erase(pTmpItr);
	return TRUE;
}

FRIENDINFO *CIMIOCPSocket::IsCharacterNameFromRejectList(char *i_szCharacterName)
{
	FRIENDINFO *pRet = NULL;
	if(0 == stricmp(i_szCharacterName, "")
		|| 0 == m_mtvectorRejectList.size())
	{
		return pRet;
	}

	m_mtvectorRejectList.lock();
	mtvectorFRIENDINFO::iterator	itr(m_mtvectorRejectList.begin());
	for(; itr != m_mtvectorRejectList.end(); itr++)
	{
		if(0 == strnicmp(itr->szCharacterName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME))
		{
			pRet = &*itr;
			break;
		}
	}
	m_mtvectorRejectList.unlock();
	
	return pRet;
}

mtvectorFRIENDINFO::iterator CIMIOCPSocket::IsCharacterNameFromRejectListItr(char *i_szCharacterName)
{
	mtvectorFRIENDINFO::iterator pRet = m_mtvectorRejectList.end();
	if (0 == stricmp(i_szCharacterName, "")
		|| 0 == m_mtvectorRejectList.size())
	{
		return m_mtvectorRejectList.end();
	}

	m_mtvectorRejectList.lock();
	mtvectorFRIENDINFO::iterator	itr(m_mtvectorRejectList.begin());
	for (; itr != m_mtvectorRejectList.end(); itr++)
	{
		if (0 == strnicmp(itr->szCharacterName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME))
		{
			pRet = itr;
		}
	}
	m_mtvectorRejectList.unlock();

	return pRet;
}

BOOL CIMIOCPSocket::InsertToRejectList(FRIENDINFO *i_pFriendInfo)
{
	if(0 == strncmp(i_pFriendInfo->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		return FALSE;
	}

	m_mtvectorRejectList.lock();
	FRIENDINFO *pTmp = this->IsCharacterNameFromRejectList(i_pFriendInfo->szCharacterName);
	if(pTmp)
	{
		m_mtvectorRejectList.unlock();
		return FALSE;
	}

	m_mtvectorRejectList.push_back(*i_pFriendInfo);
	m_mtvectorRejectList.unlock();
	
	return TRUE;
}

BOOL CIMIOCPSocket::DeleteFromRejectList(char *i_szCharacterName)
{
	if(0 == strncmp(i_szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		return FALSE;
	}

	m_mtvectorRejectList.lock();
	mtvectorFRIENDINFO::iterator pTmpItr = this->IsCharacterNameFromRejectListItr(i_szCharacterName);
	FRIENDINFO *pTmp = &*pTmpItr;
	if(NULL == pTmp)
	{
		m_mtvectorRejectList.unlock();
		return FALSE;
	}
	m_mtvectorRejectList.erase(pTmpItr);
	m_mtvectorRejectList.unlock();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::SetEachOtherFriendCnts(int i_nFriendCnts)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::SetEachOtherFriendCnts(int i_nFriendCnts)
{
	mt_auto_lock mtA(&m_mtvectorFriendList);
	m_nEachOtherFriendCnts		= i_nFriendCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::ChangedCharacterInfluenceType(BYTE i_byInfluenceTy)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::ChangedCharacterInfluenceType(BYTE i_byInfluenceTy)
{
	m_character.InfluenceType	= i_byInfluenceTy;
}

// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - ResLoadingFriendsAndRejects() 함수에 인자추가, BOOL i_bFirstTimeLoadFriendList
BOOL CIMIOCPSocket::ResLoadingFriendsAndRejects(mtvectorFRIENDINFO *i_pvectorFriendList, mtvectorFRIENDINFO *i_pvectorRejectList, BOOL i_bFirstTimeLoadFriendList)
{
	///////////////////////////////////////////////////////////////////////////////
	// 1. 친구리스트 처리
	if(false == i_pvectorFriendList->empty())
	{
		CIMIOCPSocket *arrIISocketPtr[SIZE_MAX_FRIENDLIST_COUNT];
		util::zero(arrIISocketPtr, sizeof(arrIISocketPtr[0])*SIZE_MAX_FRIENDLIST_COUNT);

// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 아래와 같이 수정 함.
//		INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_FRIENDLIST_LOADING_OK, T_IC_CHAT_FRIENDLIST_LOADING_OK, pSendFriendList, tmSendBuf);
//		pSendFriendList->nFriendListCounts = 0;
//
//		m_mtvectorFriendList.lock();
//		m_mtvectorFriendList.clear();	
//		m_mtvectorFriendList.insert(m_mtvectorFriendList.begin()
//			, i_pvectorFriendList->begin(), i_pvectorFriendList->end());
// 		this->MakeMsg_MSG_IC_CHAT_FRIENDLIST_LOADING_OK(pSendFriendList
// 			, (FRIENDINFO *)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK)), (SIZE_MAX_PACKET-MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK))/sizeof(FRIENDINFO), TRUE, arrIISocketPtr);
// 		m_mtvectorFriendList.unlock();
// 
// 		this->SendAddData(tmSendBuf
// 			, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK) + sizeof(FRIENDINFO)*pSendFriendList->nFriendListCounts);
		mt_auto_lock mtF(&m_mtvectorFriendList);
		m_mtvectorFriendList.clear();	
		m_nEachOtherFriendCnts	= 0;		// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 꼭 필요하지는 않지만 빠진것 같아서 추가 함.
		m_mtvectorFriendList.insert(m_mtvectorFriendList.begin(), i_pvectorFriendList->begin(), i_pvectorFriendList->end());
		this->SendFriendListLoadingOK(arrIISocketPtr);		// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 아래의 함수로 변경함
 		mtF.auto_unlock_cancel();


		///////////////////////////////////////////////////////////////////////////
		// 둘다 친구로 등록되어 있다면 접속정보를 전송한다
		char szTemp[128];
		int nEachOtherFriendCnts = 0;
		sprintf(szTemp, STRMSG_S_I2NOTIFY_0004, m_character.CharacterName);
		for(int i=0; i < SIZE_MAX_FRIENDLIST_COUNT; i++)
		{
			if(NULL == arrIISocketPtr[i] 
				|| FALSE == arrIISocketPtr[i]->IsValidCharacter())
			{
				continue;
			}

			if (FALSE == COMPARE_RACE(this->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR)
				&& COMPARE_RACE(arrIISocketPtr[i]->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
			{// 2006-11-14 by cmkwon, 일반 유저는 관리자,운영자를 친구로 등록해도 Online/Offline 확인 불가, 추가 경험치 불가
				continue;
			}

			//////////////////////////////////////////////////////////////////////////
			// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 아래와 같이 처음 로딩시와 아레나 이동시 로딩시 처리를 다르게 한다.
			if(i_bFirstTimeLoadFriendList)
			{
				if(FALSE == arrIISocketPtr[i]->IncreaseOnlineEachOtherFriendCnts(m_character.CharacterName))
				{
					continue;
				}

				arrIISocketPtr[i]->SendString128(STRING_128_USER_NOTICE, szTemp);
				nEachOtherFriendCnts++;
				
				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
				arrIISocketPtr[i]->SendOnlineEachOtherFriendCntsToFieldServer();
			}
			else
			{
				//////////////////////////////////////////////////////////////////////////
				// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 처음 로딩이 아니면 본인의 카운트만 증가 시켜야 한다.
				if(FALSE == arrIISocketPtr[i]->IsExistFriendList(m_character.CharacterName))
				{
					continue;
				}

				nEachOtherFriendCnts++;
			}
		}
		this->SetEachOtherFriendCnts(nEachOtherFriendCnts);		// 2006-07-19 by cmkwon

		///////////////////////////////////////////////////////////////////////////////
		// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
		this->SendOnlineEachOtherFriendCntsToFieldServer();
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2. 거부리스트 처리	
	if(false == i_pvectorRejectList->empty())
	{
// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 아래와 같이 수정 함
//		INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_REJECTLIST_LOADING_OK, T_IC_CHAT_REJECTLIST_LOADING_OK, pSendRejectList, tmSendBuf);
//		pSendRejectList->nRejectListCounts = 0;
//
//		m_mtvectorRejectList.lock();
//		m_mtvectorRejectList.clear();
//		m_mtvectorRejectList.insert(m_mtvectorRejectList.begin()
//			, i_pvectorRejectList->begin(), i_pvectorRejectList->end());
// 		this->MakeMsg_MSG_IC_CHAT_REJECTLIST_LOADING_OK(pSendRejectList
// 			, (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK)), (SIZE_MAX_PACKET-MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK))/sizeof(FRIENDINFO));
// 		m_mtvectorRejectList.unlock();
// 
// 		this->SendAddData(tmSendBuf
// 			, MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK) + sizeof(FRIENDINFO)*pSendRejectList->nRejectListCounts);
		mt_auto_lock mtR(&m_mtvectorRejectList);
		m_mtvectorRejectList.clear();
		m_mtvectorRejectList.insert(m_mtvectorRejectList.begin(), i_pvectorRejectList->begin(), i_pvectorRejectList->end());
		this->SendRejectListLoadingOK();		// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 
 		mtR.auto_unlock_cancel();
	}
	
	return TRUE;
}

// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 아래의 함수로 변경함
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CIMIOCPSocket::MakeMsg_MSG_IC_CHAT_FRIENDLIST_LOADING_OK(MSG_IC_CHAT_FRIENDLIST_ALL *io_pFiendListAll, FRIENDINFO *io_pFriendInfo, int i_nMaxCount, BOOL i_bAllFlag/*=FALSE*/)
// /// \brief		
// /// \author		cmkwon
// /// \date		2004-10-14 ~ 2004-10-14
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CIMIOCPSocket::MakeMsg_MSG_IC_CHAT_FRIENDLIST_LOADING_OK(MSG_IC_CHAT_FRIENDLIST_LOADING_OK *io_pFriendListAll
// 															  , FRIENDINFO *io_pFriendInfo, int i_nMaxCount															  
// 															  , BOOL i_bAllFlag/*=FALSE*/
// 															  , CIMIOCPSocket **o_ArrOnlineIISocketPtr/*=NULL*/)
// {
// 
// // 2007-06-01 by cmkwon, autolock 으로 수정함
// //	m_mtvectorFriendList.lock();	
// 	mt_auto_lock mtA(&m_mtvectorFriendList);
// 
// 	if(i_bAllFlag)
// 	{// 모든 유저 정보 전송
// 
// 		int nOnlineCounts = 0;
// 		mtvectorFRIENDINFO::iterator itr(m_mtvectorFriendList.begin());
// 		for(; itr != m_mtvectorFriendList.end(); itr++)
// 		{
// 			if(i_nMaxCount <= io_pFriendListAll->nFriendListCounts)
// 			{// 2007-01-24 by cmkwon, 최대 개수 버그 수정
// 
// 				return TRUE;
// 			}
// 
// 			if(strnicmp(itr->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
// 			{
// 				CIMIOCPSocket *pIISocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(itr->szCharacterName);
// 				if(NULL == pIISocket
// 					|| FALSE == pIISocket->IsValidCharacter())
// 				{
// 					itr->byIsOnline			= FALSE;
// 				}
// 				else
// 				{
// 					if(FALSE == COMPARE_RACE(this->GetCharacter()->Race, RACE_OPERATION|RACE_GAMEMASTER)
// 						&& COMPARE_RACE(pIISocket->GetCharacter()->Race, RACE_OPERATION|RACE_GAMEMASTER))
// 					{// 2006-11-14 by cmkwon, 일반 유저는 관리자,운영자를 친구로 등록해도 Online/Offline 확인 불가, 추가 경험치 불가
// 						itr->byIsOnline		= FALSE;
// 					}
// 					else
// 					{
// 						itr->byIsOnline		= TRUE;
// 						if(o_ArrOnlineIISocketPtr 
// 							&& nOnlineCounts < SIZE_MAX_FRIENDLIST_COUNT)
// 						{
// 							o_ArrOnlineIISocketPtr[nOnlineCounts] = pIISocket;
// 							nOnlineCounts++;
// 						}
// 					}
// 				}
// 				
// 				util::strncpy(io_pFriendInfo->szCharacterName, itr->szCharacterName, SIZE_MAX_CHARACTER_NAME);
// 				io_pFriendInfo->byIsOnline = itr->byIsOnline;
// 
// 				io_pFriendListAll->nFriendListCounts++;
// 				io_pFriendInfo++;
// 			}
// 		}
// 	}
// 	else
// 	{// 변경된 유저 정보만 전송
// 
// 		mtvectorFRIENDINFO::iterator itr(m_mtvectorFriendList.begin());
// 		for(; itr != m_mtvectorFriendList.end(); itr++)
// 		{
// 			if(i_nMaxCount <= io_pFriendListAll->nFriendListCounts)
// 			{// 2007-01-24 by cmkwon, 최대 개수 버그 수정
// 
// 				return TRUE;
// 			}
// 
// 			if(strnicmp(itr->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
// 			{
// 				BYTE tmIsOnline = itr->byIsOnline;
// 				itr->byIsOnline = (ms_pIMIOCP->GetIMIOCPSocketByCharacterName(itr->szCharacterName))?TRUE:FALSE;
// 				
// 				if(tmIsOnline != itr->byIsOnline)
// 				{
// 					util::strncpy(io_pFriendInfo->szCharacterName, itr->szCharacterName, SIZE_MAX_CHARACTER_NAME);
// 					io_pFriendInfo->byIsOnline = itr->byIsOnline;
// 
// 					io_pFriendListAll->nFriendListCounts++;
// 					io_pFriendInfo++;
// 				}			
// 			}
// 		}
// 	}	
// // 2007-06-01 by cmkwon, autolock 으로 수정함
// //	m_mtvectorFriendList.unlock();
// 	return TRUE;
// }
// 
// 
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CIMIOCPSocket::MakeMsg_MSG_IC_CHAT_REJECTLIST_LOADING_OK(MSG_IC_CHAT_REJECTLIST_ALL *io_pRejectListAll, FRIENDINFO *io_pFriendInfo, int i_nMaxCount)
// /// \brief		
// /// \author		cmkwon
// /// \date		2004-10-14 ~ 2004-10-14
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CIMIOCPSocket::MakeMsg_MSG_IC_CHAT_REJECTLIST_LOADING_OK(MSG_IC_CHAT_REJECTLIST_LOADING_OK *io_pRejectListAll, FRIENDINFO *io_pFriendInfo, int i_nMaxCount)
// {
// 	m_mtvectorRejectList.lock();
// 	mtvectorFRIENDINFO::iterator itr(m_mtvectorRejectList.begin());
// 	for(; itr != m_mtvectorRejectList.end(); itr++)
// 	{
// 		if(i_nMaxCount <= io_pRejectListAll->nRejectListCounts)
// 		{// 2007-01-24 by cmkwon, 최대 개수 버그 수정
// 			m_mtvectorRejectList.unlock();				// 2008-07-17 by cmkwon, 데드락 버그 수정
// 			return TRUE;
// 		}
// 		
// 		if(strncmp(itr->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
// 		{
// 			util::strncpy(io_pFriendInfo->szCharacterName, itr->szCharacterName, SIZE_MAX_CHARACTER_NAME);
// 			io_pFriendInfo->RegDate	= itr->RegDate;			// 2008-04-11 by dhjin, EP3 거부목록 -
// 			io_pRejectListAll->nRejectListCounts++;
// 			io_pFriendInfo++;
// 		}
// 	}
// 	m_mtvectorRejectList.unlock();
// 	return TRUE;
// }


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::SendFriendListLoadingOK(CIMIOCPSocket **o_ArrOnlineIISocketPtr/*=NULL*/)
/// \brief		// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - MakeMsg_MSG_IC_CHAT_FRIENDLIST_LOADING_OK() 함수를 변경 함
/// \author		cmkwon
/// \date		2008-10-24 ~ 2008-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::SendFriendListLoadingOK(CIMIOCPSocket **o_ArrOnlineIISocketPtr/*=NULL*/)
{
	INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_FRIENDLIST_LOADING_OK, T_IC_CHAT_FRIENDLIST_LOADING_OK, pSendFriendList, tmSendBuf);
	pSendFriendList->nFriendListCounts	= 0;
	FRIENDINFO *pFriendInfo				= (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK));

	mt_auto_lock mtA(&m_mtvectorFriendList);

	int nOnlineCounts = 0;
	mtvectorFRIENDINFO::iterator itr(m_mtvectorFriendList.begin());
	for(; itr != m_mtvectorFriendList.end(); itr++)
	{
		if(0 == strnicmp(itr->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
		{
			continue;
		}

		if( SIZE_MAX_PACKET <= MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK) + (pSendFriendList->nFriendListCounts+1)*sizeof(FRIENDINFO) )
		{// 2008-10-24 by cmkwon, 패킷 최대 사이즈 오버 체크
			this->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK) + pSendFriendList->nFriendListCounts*sizeof(FRIENDINFO));
			pSendFriendList->nFriendListCounts	= 0;
			pFriendInfo							= (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK));
		}

		CIMIOCPSocket *pIISocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(itr->szCharacterName);
		if(NULL == pIISocket
			|| FALSE == pIISocket->IsValidCharacter())
		{
			itr->byIsOnline			= FALSE;
		}
		else
		{
			if (FALSE == COMPARE_RACE(this->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR)
				&& COMPARE_RACE(pIISocket->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
			{// 2006-11-14 by cmkwon, 일반 유저는 관리자,운영자를 친구로 등록해도 Online/Offline 확인 불가, 추가 경험치 불가
				itr->byIsOnline		= FALSE;
			}
			else
			{
				itr->byIsOnline		= TRUE;
				if(o_ArrOnlineIISocketPtr 
					&& nOnlineCounts < SIZE_MAX_FRIENDLIST_COUNT)
				{
					o_ArrOnlineIISocketPtr[nOnlineCounts] = pIISocket;
					nOnlineCounts++;
				}
			}
		}
		
		util::strncpy(pFriendInfo->szCharacterName, itr->szCharacterName, SIZE_MAX_CHARACTER_NAME);
		pFriendInfo->byIsOnline = itr->byIsOnline;

		pSendFriendList->nFriendListCounts++;
		pFriendInfo++;
	}

	if(0 < pSendFriendList->nFriendListCounts)
	{
		this->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_LOADING_OK) + pSendFriendList->nFriendListCounts*sizeof(FRIENDINFO));
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::SendFriendListRefreshOK(void)
/// \brief		// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 
/// \author		cmkwon
/// \date		2008-10-24 ~ 2008-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::SendFriendListRefreshOK(void)
{
	INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK, T_IC_CHAT_FRIENDLIST_REFRESH_OK, pSendFriendList, tmSendBuf);
	pSendFriendList->nFriendListCounts	= 0;
	FRIENDINFO *pFriendInfo				= (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK));
	
	mt_auto_lock mtA(&m_mtvectorFriendList);
			
	mtvectorFRIENDINFO::iterator itr(m_mtvectorFriendList.begin());
	for(; itr != m_mtvectorFriendList.end(); itr++)
	{
		if(0 == strnicmp(itr->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
		{
			continue;
		}

		if( SIZE_MAX_PACKET <= MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK) + (pSendFriendList->nFriendListCounts+1)*sizeof(FRIENDINFO) )
		{// 2008-10-24 by cmkwon, 패킷 최대 사이즈 오버 체크
			this->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK) + pSendFriendList->nFriendListCounts*sizeof(FRIENDINFO));
			pSendFriendList->nFriendListCounts	= 0;
			pFriendInfo							= (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK));
		}

		BYTE tmIsOnline = itr->byIsOnline;
		itr->byIsOnline = (ms_pIMIOCP->GetIMIOCPSocketByCharacterName(itr->szCharacterName))?TRUE:FALSE;
		
		if(tmIsOnline != itr->byIsOnline)
		{
			util::strncpy(pFriendInfo->szCharacterName, itr->szCharacterName, SIZE_MAX_CHARACTER_NAME);
			pFriendInfo->byIsOnline = itr->byIsOnline;
			
			pSendFriendList->nFriendListCounts++;
			pFriendInfo++;
		}
	}

	if(0 < pSendFriendList->nFriendListCounts)
	{
		this->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK) + pSendFriendList->nFriendListCounts*sizeof(FRIENDINFO));
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::SendRejectListLoadingOK(void)
/// \brief		// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - MakeMsg_MSG_IC_CHAT_REJECTLIST_LOADING_OK() 함수를 변경함
/// \author		cmkwon
/// \date		2008-10-24 ~ 2008-10-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::SendRejectListLoadingOK(void)
{
	INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_REJECTLIST_LOADING_OK, T_IC_CHAT_REJECTLIST_LOADING_OK, pSendRejectList, tmSendBuf);
	pSendRejectList->nRejectListCounts	= 0;
	FRIENDINFO *pFriendInfo				= (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK));

	mt_auto_lock mtA(&m_mtvectorRejectList);
	mtvectorFRIENDINFO::iterator itr(m_mtvectorRejectList.begin());
	for(; itr != m_mtvectorRejectList.end(); itr++)
	{
		if(0 == strncmp(itr->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))
		{
			continue;
		}

		if( SIZE_MAX_PACKET <= MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK) + (pSendRejectList->nRejectListCounts+1)*sizeof(FRIENDINFO) )
		{// 2008-10-24 by cmkwon, 패킷 최대 사이즈 오버 체크
			this->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK) + pSendRejectList->nRejectListCounts*sizeof(FRIENDINFO));
			pSendRejectList->nRejectListCounts	= 0;
			pFriendInfo							= (FRIENDINFO*)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK));
		}

		util::strncpy(pFriendInfo->szCharacterName, itr->szCharacterName, SIZE_MAX_CHARACTER_NAME);
		pFriendInfo->RegDate	= itr->RegDate;			// 2008-04-11 by dhjin, EP3 거부목록 -
		pSendRejectList->nRejectListCounts++;
		pFriendInfo++;
	}

	if(0 < pSendRejectList->nRejectListCounts)
	{
		this->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_REJECTLIST_LOADING_OK) + pSendRejectList->nRejectListCounts*sizeof(FRIENDINFO));
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::IsValidCharacter(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-18 ~ 2006-07-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::IsValidCharacter(void)
{
	if(FALSE == IsUsing()
		|| ST_CLIENT_TYPE != m_PeerSocketType
		|| FALSE == IS_VALID_UNIQUE_NUMBER(m_character.CharacterUniqueNumber))
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::SetChatCharacter(CHARACTER *pCharacter)
/// \brief		
/// \author		cmkwon
/// \date		2004-10-14 ~ 2004-10-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::SetChatCharacter(CHARACTER *pCharacter)
{
	memcpy(&m_character, pCharacter, sizeof(CHARACTER));
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed)
//	: IM Server에 접속, 인증을 거친다
ProcessResult CIMIOCPSocket::Process_IC_CONNECT_LOGIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	CIMIOCPSocket			*pIOCPSocket = NULL;
	MSG_IC_CONNECT_LOGIN	*pRecvLogin = NULL;

	nRecvTypeSize = sizeof(MSG_IC_CONNECT_LOGIN);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{	// 받아야할 메세지 타입의 사이즈보다 남은 데이타의 사이즈가 작다면 에러
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_IC_CONNECT_LOGIN, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12002);
		return RES_RETURN_FALSE;
	}
	pRecvLogin = new MSG_IC_CONNECT_LOGIN;
	memcpy(pRecvLogin, pPacket+nBytesUsed, nRecvTypeSize);
	nBytesUsed += nRecvTypeSize;


	m_PeerSocketType = ST_CLIENT_TYPE;
	if(ms_pIMIOCP->IsConnectedUser(pRecvLogin->AccountName, &pIOCPSocket))
	{
		SendErrorMessage(T_IC_CONNECT_LOGIN, ERR_PROTOCOL_DUPLICATE_LOGIN, 0, 0, pRecvLogin->AccountName);
		Close(0x12003);
		util::del(pRecvLogin);
		return RES_RETURN_FALSE;
	}

	util::strncpy(m_character.AccountName, pRecvLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(m_character.CharacterName, pRecvLogin->CharacterName, SIZE_MAX_CHARACTER_NAME);
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_AuthChatLogin, this, pRecvLogin);	// DB로 인증 절차를 넘긴다.
	pRecvLogin = NULL;

	char szSystemLog[256];
	sprintf(szSystemLog, "  IM Server Logined Account[%10s], Character[%10s]\r\n", m_character.AccountName, m_character.CharacterName);
	g_pGlobal->WriteSystemLog(szSystemLog);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CHAT_ALL(const char* pPacket, int nLength, int &nBytesUsed)
//	:
// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int				nRecvTypeSize;
	MSG_IC_CHAT_ALL	*pRecvChatAll;
	char			*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_ALL) + ((MSG_IC_CHAT_ALL*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12005);
		return RES_RETURN_FALSE;
	}
	pRecvChatAll = (MSG_IC_CHAT_ALL*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_ALL));
	nBytesUsed += nRecvTypeSize;

	if (!COMPARE_RACE(m_character.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		SendErrorMessage(T_IC_CHAT_ALL, ERR_CHAT_PERMISSION_DENIED);
		return RES_BREAK;
	}

	if (strnicmp(m_character.CharacterName, pRecvChatAll->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_ALL, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12006);
		return RES_RETURN_FALSE;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_ALL, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_All|%s|%d|%d|%s\r\n",
			pRecvChatAll->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatAll + sizeof(MSG_IC_CHAT_ALL));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	ms_pIMIOCP->SendChatMsgToAllClients(CHAT_TYPE_UNCONDITIONAL, ((BYTE*)pRecvChatAll) - SIZE_FIELD_TYPE_HEADER, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CHAT_MAP(const char* pPacket, int nLength, int &nBytesUsed)
//	:
// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_MAP(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int				nRecvTypeSize;
	MSG_IC_CHAT_MAP	*pRecvChatMap;
	char			*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_MAP) + ((MSG_IC_CHAT_MAP*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_MAP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12007);
		return RES_RETURN_FALSE;
	}
	pRecvChatMap = (MSG_IC_CHAT_MAP*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_MAP));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatMap->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_MAP, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12008);
		return RES_RETURN_FALSE;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
		|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_MAP, 맵
	{
		SendErrorMessage(T_IC_CHAT_MAP, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_MAP, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_Map|%s|%d|%d|%s\r\n",
			pRecvChatMap->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatMap + sizeof(MSG_IC_CHAT_MAP));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_MAP, T_FI_CHAT_MAP, msgChatMap, SendBuf);
	msgChatMap->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
	msgChatMap->MessageLength = pRecvChatMap->MessageLength;
	util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FI_CHAT_MAP), chatString, SIZE_MAX_CHAT_MESSAGE);

	this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CHAT_MAP) + msgChatMap->MessageLength);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CHAT_REGION(const char* pPacket, int nLength, int &nBytesUsed)
//	:
// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_REGION(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int					nRecvTypeSize;
	MSG_IC_CHAT_REGION	*pRecvChatRegion;
	char				*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_REGION) + ((MSG_IC_CHAT_REGION*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_REGION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1200A);
		return RES_RETURN_FALSE;
	}
	pRecvChatRegion = (MSG_IC_CHAT_REGION*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_REGION));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatRegion->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_REGION, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x1200B);
		return RES_RETURN_FALSE;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_REGION, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 채팅 로그 남기기
	char szChatting[1024];
	sprintf(szChatting, "Chat_Region|%s|%d|%d|%s\r\n",
			pRecvChatRegion->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatRegion + sizeof(MSG_IC_CHAT_REGION));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가

	if(g_pIMGlobal->CheckWriteLog(chatString))		// 2011-06-22 by hskim, 사설 서버 방지
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	if (strncmp(chatString, "/", 1) == 0)
	{
		// 2007-10-02 by cmkwon, COMMAND 로그 남기기
		if (g_pIMGlobal->CheckWriteLog(chatString))		// 2011-06-22 by hskim, 사설 서버 방지
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[Notify] : %s, IP(%s), command(%s)\r\n", GetCharacterString(&m_character, string()), this->GetPeerIP(), chatString);

		// 명령어임
		BOOL ret = HandleAdminCommands(chatString);
		if (ret)
		{
			return RES_RETURN_TRUE;
		}
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	if(0 != strncmp(chatString, "/", 1))
	{// 2008-12-30 by cmkwon, 명령어는 채금 체크하지 않는다.
		int nRemainMinute = 0;
		if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
			|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_REGION, 주변
		{
			SendErrorMessage(T_IC_CHAT_REGION, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
			return RES_BREAK;
		}
	}


	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_REGION, T_FI_CHAT_REGION, msgChatRegion, SendBuf);
	msgChatRegion->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
	msgChatRegion->MessageLength = pRecvChatRegion->MessageLength;
	util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FI_CHAT_REGION), chatString, SIZE_MAX_CHAT_MESSAGE);

	if (FALSE == this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CHAT_REGION) + msgChatRegion->MessageLength))
	{
		// current field server socket이 NULL이면, 접속을 끊는다.
		char	szSystemLog[1024];
		sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
			GetCharacterString(&m_character, string()), GetProtocolTypeString(T_IC_CHAT_REGION));
		DBGOUT(szSystemLog);
		g_pGlobal->WriteSystemLog(szSystemLog);

		return RES_RETURN_FALSE;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CHAT_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
//	:
// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_GUILD(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int					nRecvTypeSize;
	MSG_IC_CHAT_GUILD	*pRecvChatGuild;
	char				*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_GUILD) + ((MSG_IC_CHAT_GUILD*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_GUILD, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1200C);
		return RES_RETURN_FALSE;
	}
	pRecvChatGuild = (MSG_IC_CHAT_GUILD*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_GUILD));
	nBytesUsed += nRecvTypeSize;

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_CHAT_GUILD, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	if (strnicmp(m_character.CharacterName, pRecvChatGuild->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_GUILD, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x1200D);
		return RES_RETURN_FALSE;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_GUILD, 여단
	{
		SendErrorMessage(T_IC_CHAT_GUILD, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_GUILD, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];

	// 2013-05-21 by bckim, 캐나다 채팅로그 정보 추가
// 	sprintf(szChatting, "Chat_GUILD|%s|%d|%d|%s\r\n",
// 		pRecvChatGuild->FromCharacterName, m_character.MapChannelIndex.MapIndex,
// 		m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatGuild + sizeof(MSG_IC_CHAT_GUILD));

	sprintf(szChatting, "Chat_GUILD|%s|%d|%d|%d|%s\r\n",
		pRecvChatGuild->FromCharacterName, m_character.GuildUniqueNumber,m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatGuild + sizeof(MSG_IC_CHAT_GUILD));
// End. 2013-05-21 by bckim, 캐나다 채팅로그 정보 추가

	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);


// 2007-01-31 by cmkwon, 아래와 같이 수정함
//	ms_pIMIOCP->m_MapGuild.lock();
//	{
//		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//		if(ptmGuild)
//		{
//			ptmGuild->SendChatMsgToGuildMembers(((BYTE*)pRecvChatGuild) - SIZE_FIELD_TYPE_HEADER
//				, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize, CHAT_TYPE_GUILD);
//		}
//	}
//	ms_pIMIOCP->m_MapGuild.unlock();
	
	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	vectCIMIOCPSocketPtr vectIISockList;
	vectIISockList.reserve(SIZE_MAX_GUILD_CAPACITY);			// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	
	mt_auto_lock mtAG(&ms_pIMIOCP->m_MapGuild);
	CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
	if(NULL == ptmGuild)
	{
		return RES_BREAK;
	}
	ptmGuild->GetGuildMemberIISocket(&vectIISockList);
	mtAG.auto_unlock_cancel();
	
	if(vectIISockList.empty())
	{
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	ms_pIMIOCP->SendChatMsg2MultiUser(&vectIISockList, ((BYTE*)pRecvChatGuild) - SIZE_FIELD_TYPE_HEADER
				, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize, CHAT_TYPE_GUILD);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CHAT_PARTY(const char* pPacket, int nLength, int &nBytesUsed)
//	:
// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_PARTY(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int					nRecvTypeSize;
	MSG_IC_CHAT_PARTY	*pRecvChatParty;
	char				*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_PARTY) + ((MSG_IC_CHAT_PARTY*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_PARTY, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12010);
		return RES_RETURN_FALSE;
	}
	pRecvChatParty = (MSG_IC_CHAT_PARTY*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_PARTY));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatParty->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_PARTY, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12011);
		return RES_RETURN_FALSE;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_PARTY, 편대
	{
		SendErrorMessage(T_IC_CHAT_PARTY, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_PARTY, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	CIMParty *ptmParty = this->GetIMParty();
	if (NULL == ptmParty)
	{
		SendErrorMessage(T_IC_CHAT_PARTY, ERR_PROTOCOL_NOT_HAVE_PARTY);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	ptmParty->LockIMParty();
	{
		if(i_pThreadInfo)
		{// 2007-02-21 by cmkwon
			i_pThreadInfo->nParam1	= 10;
		}
		char szChatting[1024];

// 2013-05-21 by bckim, 캐나다 채팅로그 정보 추가
// 		sprintf(szChatting, "Chat_Party|%s|%d|%d|%s\r\n",
// 			pRecvChatParty->FromCharacterName, m_character.MapChannelIndex.MapIndex,
// 			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatParty + sizeof(MSG_IC_CHAT_PARTY));

		sprintf(szChatting, "Chat_Party|%s|%I64d|%d|%d|%s\r\n",
			pRecvChatParty->FromCharacterName,ptmParty->m_PartyID,m_character.MapChannelIndex.MapIndex,
				m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatParty + sizeof(MSG_IC_CHAT_PARTY));
// End. 2013-05-21 by bckim, 캐나다 채팅로그 정보 추가

		g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

		if(i_pThreadInfo)
		{// 2007-02-21 by cmkwon
			i_pThreadInfo->nParam1++;
		}

		ptmParty->SendChatMsgToMembers(((BYTE*)pRecvChatParty) - SIZE_FIELD_TYPE_HEADER
			, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize, CHAT_TYPE_PARTY);

		if(i_pThreadInfo)
		{// 2007-02-21 by cmkwon
			i_pThreadInfo->nParam1++;
		}
	}
	ptmParty->UnlockIMParty();

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1	= 20;
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// ProcessResult Process_IC_CHAT_PTOP(const char* pPacket, int nLength, int &nBytesUsed)
//	:
// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_PTOP(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int					nRecvTypeSize;
	MSG_IC_CHAT_PTOP	*pRecvChatPTOP;
	char				*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_PTOP) + ((MSG_IC_CHAT_PTOP*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_PTOP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12012);
		return RES_RETURN_FALSE;
	}
	pRecvChatPTOP = (MSG_IC_CHAT_PTOP*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_PTOP));
	nBytesUsed += nRecvTypeSize;


	if (strnicmp(m_character.CharacterName, pRecvChatPTOP->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_PTOP, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12013);
		return RES_RETURN_FALSE;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_PTOP, 귓말
	{
		SendErrorMessage(T_IC_CHAT_PTOP, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	if(1 >= pRecvChatPTOP->MessageLength)
	{// 2006-09-27 by cmkwon, 채팅 내용이 없다
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 상대의 귓속말 거부 여부 확인
	CIMIOCPSocket *pToSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pRecvChatPTOP->ToCharacterName);
	if (NULL == pToSocket
		|| FALSE == pToSocket->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHAT_PTOP, ERR_COMMON_NO_SUCH_CHARACTER, 0, 0, pRecvChatPTOP->ToCharacterName);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 보낸캐릭터의 세력을 설정 해준다
	pRecvChatPTOP->FromInflTy	= m_character.InfluenceType;

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-07 by cmkwon, 세력체크
	if(FALSE == GCheckRaceAndInfluenceType(CHECK_TYPE_CHAT_PTOP, m_character.Race, m_character.InfluenceType,
		pToSocket->GetCharacter()->Race, pToSocket->GetCharacter()->InfluenceType))
	{
		if(FALSE == COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER)
			|| FALSE == COMPARE_RACE(pToSocket->GetCharacter()->Race, RACE_INFLUENCE_LEADER))
		{// 2006-04-20 by cmkwon, 둘다 세력지도자이면 1:1 채팅 가능
			SendErrorMessage(T_IC_CHAT_PTOP, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED);
			return RES_BREAK;
		}
	}

	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	if (FALSE == COMPARE_CHATTYPE_BIT(pToSocket->m_enableChatFlag8, CHAT_TYPE_PTOP))
	if (FALSE == COMPARE_CHATTYPE_BIT(pToSocket->m_enableChatFlag32, CHAT_TYPE_PTOP))
	{
		if (COMPARE_RACE(pToSocket->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2006-11-14 by cmkwon, 관리자,운영자는 알수 없도록 아래의 에러메시지를 전송한다.
			SendErrorMessage(T_IC_CHAT_PTOP, ERR_COMMON_NO_SUCH_CHARACTER, 0, 0, pRecvChatPTOP->ToCharacterName);
		}
		else
		{
			SendErrorMessage(T_IC_CHAT_PTOP, ERR_CHAT_CHAT_NOT_TRANSFERED);
		}
		return RES_BREAK;
	}
	
	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	if(pToSocket->IsCharacterNameFromRejectList(this->m_character.CharacterName))
	{// 상대방 거부리스트에 등록되어 있음

		SendErrorMessage(T_IC_CHAT_PTOP, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);	
		return RES_BREAK;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_PTOP, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_PtoP|%s, %s|%d|%d|%s\r\n",
			pRecvChatPTOP->FromCharacterName, pRecvChatPTOP->ToCharacterName,
			m_character.MapChannelIndex.MapIndex, m_character.MapChannelIndex.ChannelIndex,
			(char*)pRecvChatPTOP + sizeof(MSG_IC_CHAT_PTOP));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	USHORT usRace = m_character.Race | pToSocket->m_character.Race;	// 2007-11-13 by cmkwon, 1:1 채팅은 둘중에 한명이 관리자 혹은 운영자이면 채팅로그 저장

	g_pIMGlobal->WriteChattingLog(szChatting, usRace);


	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	pToSocket->SendAddData(((BYTE*)pRecvChatPTOP) - SIZE_FIELD_TYPE_HEADER, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	SendAddData(((BYTE*)pRecvChatPTOP) - SIZE_FIELD_TYPE_HEADER, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_CHANGE_CHAT_FLAG(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize;
	MSG_IC_CHAT_CHANGE_CHAT_FLAG	*pRecvChatOnOff;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_CHANGE_CHAT_FLAG);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_CHANGE_CHAT_FLAG, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12012);
		return RES_RETURN_FALSE;
	}
	pRecvChatOnOff = (MSG_IC_CHAT_CHANGE_CHAT_FLAG*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	SET_CHATTYPE_BIT(pRecvChatOnOff->bitChatType, CHAT_TYPE_UNCONDITIONAL);		// 
// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	m_enableChatFlag8			= pRecvChatOnOff->bitChatType;					//
	m_enableChatFlag32 = pRecvChatOnOff->bitChatType;					//

	this->SendToFieldServerChatBitFlag();
	return RES_RETURN_TRUE;
}

// 2007-02-21 by cmkwon, 인자 추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_SELL_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int						nRecvTypeSize;
	MSG_IC_CHAT_SELL_ALL	*pRecvChatAll;
	char					*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_SELL_ALL) + ((MSG_IC_CHAT_SELL_ALL*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_SELL_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12005);
		return RES_RETURN_FALSE;
	}
	pRecvChatAll = (MSG_IC_CHAT_SELL_ALL*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_SELL_ALL));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatAll->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_SELL_ALL, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12006);
		return RES_RETURN_FALSE;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
		|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_SELL_ALL, 매매
	{
		SendErrorMessage(T_IC_CHAT_SELL_ALL, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	if(FALSE == COMPARE_INFLUENCE(m_character.InfluenceType, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{// 2006-06-13 by cmkwon, 세력 선택 유저만이 거래 채팅 가능
		SendErrorMessage(T_IC_CHAT_SELL_ALL, ERR_REQ_CHOOSE_INFLUENCE_TYPE);
		return RES_BREAK;
	}

	// 2008-05-19 by dhjin, EP3 - 채팅 시스템 변경, 전쟁 채팅 - 거래 채팅은 세력으로만 체크 한다.
//	if(FALSE == GCheckLimitLevel(CHECK_TYPE_CHAT_SELL_ALL, m_character.Level))
//	{
//		SendErrorMessage(T_IC_CHAT_SELL_ALL, ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED);
//		return RES_BREAK;
//	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_SELL_ALL, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_Sell_All|%s|%d|%d|%s\r\n",
			pRecvChatAll->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatAll + sizeof(MSG_IC_CHAT_SELL_ALL));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-07 by cmkwon, 거래는 동일한 세력에게만 전송한다
	ms_pIMIOCP->SendChatMsgToAllClients(CHAT_TYPE_SELL_ALL, ((BYTE*)pRecvChatAll) - SIZE_FIELD_TYPE_HEADER
		, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize, m_character.InfluenceType);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	return RES_RETURN_TRUE;
}

// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
ProcessResult CIMIOCPSocket::Process_IC_CHAT_CASH_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int						nRecvTypeSize;
	MSG_IC_CHAT_CASH_ALL	*pRecvChatMap;
	char					*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_CASH_ALL) + ((MSG_IC_CHAT_CASH_ALL*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_CASH_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12007);
		return RES_RETURN_FALSE;
	}
	pRecvChatMap = (MSG_IC_CHAT_CASH_ALL*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_CASH_ALL));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatMap->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_MAP, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12008);
		return RES_RETURN_FALSE;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_MAP, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_Cash_All|%s|%d|%d|%s\r\n",
			pRecvChatMap->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatMap + sizeof(MSG_IC_CHAT_MAP));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_CASH_ALL, T_FI_CHAT_CASH_ALL, msgChatMap, SendBuf);
	msgChatMap->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
	msgChatMap->MessageLength			= pRecvChatMap->MessageLength;
	util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FI_CHAT_CASH_ALL), chatString, SIZE_MAX_CHAT_MESSAGE);
	this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CHAT_CASH_ALL) + msgChatMap->MessageLength);
	
	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHAT_INFLUENCE_ALL(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2007-02-21 by cmkwon, 인자추가(SThreadInfo *i_pThreadInfo)
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHAT_INFLUENCE_ALL(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int						nRecvTypeSize;
	char					*chatString;
	MSG_IC_CHAT_INFLUENCE_ALL	*pRInflAll;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_INFLUENCE_ALL) + ((MSG_IC_CHAT_INFLUENCE_ALL*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12007);
		return RES_RETURN_FALSE;
	}
	pRInflAll = (MSG_IC_CHAT_INFLUENCE_ALL*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_INFLUENCE_ALL));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRInflAll->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		return RES_BREAK;
	}

	if(FALSE == COMPARE_INFLUENCE(this->m_character.InfluenceType, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{// 2007-10-06 by cmkwon, 기본 세력 체크
		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_REQ_CHOOSE_INFLUENCE_TYPE);
		return RES_BREAK;
	}

	if(FALSE == COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER|RACE_INFLUENCE_SUBLEADER_MASK))
	{// 2007-10-06 by cmkwon, 전진 기지 소유 여단장 세력 채팅

		if(FALSE == IS_VALID_UNIQUE_NUMBER(this->GetCharacter()->GuildUniqueNumber))
		{// 2007-10-06 by cmkwon, 여단 가입 유무 체크
			SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		if(FALSE == ms_pIMIOCP->IsGuildCommanderW(this->GetCharacter()->GuildUniqueNumber, this->GetCharacter()->CharacterUniqueNumber))
		{// 2007-10-06 by cmkwon, 여단장을 체크
			SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}
	}

//	if(FALSE == COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER))
//	{
//		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_CHAT_PERMISSION_DENIED);
//		return RES_BREAK;
//	}

// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능 - 지도자/부지도자가 아니면 채팅을 FieldServer로 전송하여 FieldServer에서 전진기지 소유 여단장을 체크한다.
//	// 2006-12-08 by dhjin, 지도자, 부지도자는 같은 세력 전체 공지채팅가능
//	// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
//	//if(FALSE == COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER|RACE_INFLUENCE_SUBLEADER))
//	if(FALSE == COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER|RACE_INFLUENCE_SUBLEADER_MASK))
//	{
//		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_CHAT_PERMISSION_DENIED);
//		return RES_BREAK;
//	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-07 by cmkwon, 세력채팅시 채팅 블럭 체크 추가
	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))
	{
		SendErrorMessage(T_IC_CHAT_INFLUENCE_ALL, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_Influence_All|%s|%d|%d|%s\r\n",
			pRInflAll->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRInflAll + sizeof(MSG_IC_CHAT_MAP));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	// 2007-10-06 by cmkwon, 전진 기지 소유한 여단장 세력 채팅 가능 - 지도자/부지도자가 아니면 채팅을 FieldServer로 전송하여 FieldServer에서 전진기지 소유 여단장을 체크한다.
	if(FALSE == COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER|RACE_INFLUENCE_SUBLEADER_MASK))
	{
		INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_OUTPOST_GUILD, T_FI_CHAT_OUTPOST_GUILD, pSOutpost, SendBuf);
		pSOutpost->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
		pSOutpost->MessageLength			= min(SIZE_MAX_CHAT_MESSAGE, pRInflAll->MessageLength);
		util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FI_CHAT_OUTPOST_GUILD), chatString, SIZE_MAX_CHAT_MESSAGE);
		this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CHAT_OUTPOST_GUILD)+pSOutpost->MessageLength);
	}
	else
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_INFLUENCE_ALL, T_IC_CHAT_INFLUENCE_ALL, pSInflAll, SendBuf);
		util::strncpy(pSInflAll->FromCharacterName, GetLeader8SubLeaderString(m_character.InfluenceType, m_character.Race), SIZE_MAX_CHARACTER_NAME);
	// 2007-10-06 by cmkwon, 부지도자 2명의 호칭을 다르게 설정 -
	//// 2006-12-13 by cmkwon, 아래와 같이 전송 캐릭터명을 지도자/부지도자로 설정해서 전송한다.
	////	util::strncpy(pSInflAll->FromCharacterName, pRInflAll->FromCharacterName, SIZE_MAX_CHARACTER_NAME);
	//	if(COMPARE_RACE(m_character.Race, RACE_INFLUENCE_LEADER))
	//	{
	//		util::strncpy(pSInflAll->FromCharacterName, STRCMD_CS_COMMON_INFLUENCE_LEADER, SIZE_MAX_CHARACTER_NAME);
	//	}
	//	else
	//	{
	//		util::strncpy(pSInflAll->FromCharacterName, STRCMD_CS_COMMON_INFLUENCE_SUBLEADER, SIZE_MAX_CHARACTER_NAME);
	//	}
		pSInflAll->MessageLength	= min(SIZE_MAX_CHAT_MESSAGE, pRInflAll->MessageLength);
		util::strncpy((char*)SendBuf + MSG_SIZE(MSG_IC_CHAT_INFLUENCE_ALL), chatString, SIZE_MAX_CHAT_MESSAGE);
		ms_pIMIOCP->SendChatMsgToAllClients(CHAT_TYPE_INFLUENCE, SendBuf, MSG_SIZE(MSG_IC_CHAT_INFLUENCE_ALL)+pSInflAll->MessageLength, m_character.InfluenceType);
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
/// \brief		아레나 채팅을 받아 FieldServer로 보낸다
/// \author		dhjin
/// \date		2007-05-02 ~ 2007-05-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int					nRecvTypeSize;
	MSG_IC_CHAT_REGION	*pRecvChatRegion;
	char				*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_ARENA) + ((MSG_IC_CHAT_ARENA*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_ARENA, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1200A);
		return RES_RETURN_FALSE;
	}
	pRecvChatRegion = (MSG_IC_CHAT_ARENA*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_ARENA));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatRegion->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_ARENA, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x1200B);
		return RES_RETURN_FALSE;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_ARENA, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 채팅 로그 남기기
	char szChatting[1024];
	sprintf(szChatting, "Chat_Arena|%s|%d|%d|%s\r\n",
			pRecvChatRegion->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatRegion + sizeof(MSG_IC_CHAT_ARENA));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
		|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_ARENA, 아레나
	{
		SendErrorMessage(T_IC_CHAT_ARENA, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_ARENA, T_FI_CHAT_ARENA, msgChatRegion, SendBuf);
	msgChatRegion->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
	msgChatRegion->MessageLength = pRecvChatRegion->MessageLength;
	util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FI_CHAT_ARENA), chatString, SIZE_MAX_CHAT_MESSAGE);

	if (FALSE == this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CHAT_ARENA) + msgChatRegion->MessageLength))
	{
		// current field server socket이 NULL이면, 접속을 끊는다.
		char	szSystemLog[1024];
		sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
			GetCharacterString(&m_character, string()), GetProtocolTypeString(T_IC_CHAT_ARENA));
		DBGOUT(szSystemLog);
		g_pGlobal->WriteSystemLog(szSystemLog);

		return RES_RETURN_FALSE;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHAT_WAR(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
/// \brief		EP3 - 채팅 시스템 변경, 전쟁 채팅
/// \author		dhjin
/// \date		2008-05-19 ~ 2008-05-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHAT_WAR(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int						nRecvTypeSize;
	MSG_IC_CHAT_WAR			*pRecvChatWar;
	char					*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_WAR) + ((MSG_IC_CHAT_WAR*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_CHAT_WAR, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12005);
		return RES_RETURN_FALSE;
	}
	pRecvChatWar = (MSG_IC_CHAT_WAR*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_WAR));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChatWar->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_WAR, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12006);
		return RES_RETURN_FALSE;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
		|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_WAR, 전쟁
	{
		SendErrorMessage(T_IC_CHAT_WAR, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	if(FALSE == COMPARE_INFLUENCE(m_character.InfluenceType, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{// 2006-06-13 by cmkwon, 세력 선택 유저만이 거래 채팅 가능
		SendErrorMessage(T_IC_CHAT_WAR, ERR_REQ_CHOOSE_INFLUENCE_TYPE);
		return RES_BREAK;
	}

	if(FALSE == GCheckLimitLevel(CHECK_TYPE_CHAT_WAR, m_character.Level))
	{
		SendErrorMessage(T_IC_CHAT_WAR, ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED);
		return RES_BREAK;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_WAR, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	char szChatting[1024];
	sprintf(szChatting, "Chat_War|%s|%d|%d|%s\r\n",
			pRecvChatWar->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChatWar + sizeof(MSG_IC_CHAT_SELL_ALL));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-07 by cmkwon, 거래는 동일한 세력에게만 전송한다
	ms_pIMIOCP->SendChatMsgToAllClients(CHAT_TYPE_WAR, ((BYTE*)pRecvChatWar) - SIZE_FIELD_TYPE_HEADER
		, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize, m_character.InfluenceType);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHAT_CNC(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
/// \brief		Adding Cross Nation Chat
/// \author		future
/// \date		2014-08-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHAT_CNC(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
#ifdef _CROSS_NATION_CHAT

	int						nRecvTypeSize;
	MSG_IC_CHAT_CNC			*pRecvChatCnc;
	char					*chatString;

	// Size of the RecievedPacket = struct size + MessageLength
	nRecvTypeSize = sizeof(MSG_IC_CHAT_CNC) + ((MSG_IC_CHAT_CNC*)(pPacket + nBytesUsed))->MessageLength;

	// Only the Size of the Recieved Packet should equal the currently available length for reading
	if (nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_CHAT_CNC, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12005);
		return RES_RETURN_FALSE;
	}

	// Get MSG_IC_CHAT_CNC object pointer from packet
	pRecvChatCnc = (MSG_IC_CHAT_CNC*)(pPacket + nBytesUsed);

	// Read Chat String from MSG_IC_CHAT_CNC object
	chatString = (char*)(pPacket + nBytesUsed + sizeof(MSG_IC_CHAT_CNC)); // after the read bytes AND the MSG_IC_CHAT_CNC object

#if defined(_DEBUG) || defined(F_DEBUG)
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: CNC Chat Arrived\r\n");
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: ****Struct Data****\r\n");
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: From Character Name -> %s\r\n", &pRecvChatCnc->FromCharacterName);
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: Influence of Staff Prefix (should be 0 from the client) -> %s\r\n", &pRecvChatCnc->InfluenceOrStaffPrefix);
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: Message Length: %d\r\n", pRecvChatCnc->MessageLength);
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: Calced Chat String: %s\r\n", chatString);
#endif

	// Setting UsedBytes + the packet length after its fully read
	nBytesUsed += nRecvTypeSize;

	// Character Mismatch (between connected char and sender char in the chat object)
	if (strnicmp(m_character.CharacterName, pRecvChatCnc->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_CNC, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
//		Close(0x12006);
		return RES_RETURN_FALSE;
	}

	// no idea again...
	if (i_pThreadInfo)
	{
		i_pThreadInfo->nParam1++;
	}

	// Chat blocked?
	int nRemainMinute = 0;
	if (ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
		|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))
	{
		SendErrorMessage(T_IC_CHAT_CNC, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	// No idea again...
	if (i_pThreadInfo)
	{
		i_pThreadInfo->nParam1++;
	}

	// 2015-07-20 Future, CnC money cost

	if (ms_pIMIOCP->m_bEnableCNC)
	{
#ifdef _CROSS_NATION_CHAT_SPI_COST
		if (!COMPARE_RACE(m_character.Race, RACE_ACCOUNT_TYPE_MASK))
		{
			// Money calculations on Field Server
			SET_MSG_TYPE(((BYTE*)pRecvChatCnc) - SIZE_FIELD_TYPE_HEADER, T_FI_CHAT_CNC);
			this->SendMsgToCurrentFieldServerSocket(((BYTE*)pRecvChatCnc) - SIZE_FIELD_TYPE_HEADER, MSG_SIZE(MSG_FI_CHAT_CNC) + pRecvChatCnc->MessageLength);
		}
		else
		{
			// Staff accounts dont need to pay for chatting in CnC
			this->SendCNC(pRecvChatCnc);
		}
#else
		// Direct Chat sending
		this->SendCNC(pRecvChatCnc);
#endif // _CROSS_NATION_CHAT_SPI_COST
	}
	else
	{
		SendString128(STRING_128_USER_NOTICE, STRMSG_CNC_DISABLED);
	}

	// No idea (3rd time) ...
	if (i_pThreadInfo)
	{
		i_pThreadInfo->nParam1++;
	}
#endif
	return RES_RETURN_TRUE;
}

// 2015-07-20 Future, CnC costs money
ProcessResult CIMIOCPSocket::Process_FI_CHAT_CNC(const char* pPacket, int nLength, int& nBytesUsed)
{
	MSG_FI_CHAT_CNC*	pRecvMsg = (MSG_FI_CHAT_CNC*)(pPacket + nBytesUsed);
	int					nRecvSize = sizeof(MSG_FI_CHAT_CNC) + pRecvMsg->MessageLength;

	if (nLength - nBytesUsed < nRecvSize)
	{
		SendErrorMessage(T_IC_CHAT_CNC, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12005);
		return RES_RETURN_FALSE;
	}
	nBytesUsed += nRecvSize;

	// Send the chat to all clients
	SendCNC(pRecvMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHAT_CHATROOM(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
/// \brief		EP3 채팅방 - 
/// \author		dhjin
/// \date		2008-06-18 ~ 2008-06-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHAT_CHATROOM(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
{
	int						nRecvTypeSize;
	MSG_IC_CHAT_CHATROOM	*pRecvChat;
	char					*chatString;

	nRecvTypeSize = sizeof(MSG_IC_CHAT_CHATROOM) + ((MSG_IC_CHAT_CHATROOM*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_CHATROOM, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12010);
		return RES_RETURN_FALSE;
	}
	pRecvChat = (MSG_IC_CHAT_CHATROOM*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_CHATROOM));
	nBytesUsed += nRecvTypeSize;

	if (strnicmp(m_character.CharacterName, pRecvChat->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_CHATROOM, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x12011);
		return RES_RETURN_FALSE;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_CHATROOM, 채팅방
	{
		SendErrorMessage(T_IC_CHAT_CHATROOM, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_CHATROOM, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	if(this->m_bChatRoomUseing)
	{
		mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));
		mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
		for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
		{
			if(this->m_nChatRoomNum == (*itr)->GetChatRoomNum())
			{
				char szChatting[1024];
				sprintf(szChatting, "Chat_ChatRoom|%s|%d|%d|%s\r\n",
						pRecvChat->FromCharacterName, m_character.MapChannelIndex.MapIndex,
						m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChat + sizeof(MSG_IC_CHAT_CHATROOM));
				// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
				g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

				(*itr)->SendChatMessagetoChatRoomMember(((BYTE*)pRecvChat) - SIZE_FIELD_TYPE_HEADER
					, SIZE_FIELD_TYPE_HEADER + nRecvTypeSize, CHAT_TYPE_CHATROOM);
				break;
			}
		}
	}
	else
	{
		SendErrorMessage(T_IC_CHAT_CHATROOM, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo)
/// \brief		인피니티 - 인피 채팅
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed, SThreadInfo *i_pThreadInfo) {
	int						nRecvTypeSize;
	MSG_IC_CHAT_INFINITY	*pRecvChat;
	char					*chatString;
	
	nRecvTypeSize = sizeof(MSG_IC_CHAT_INFINITY) + ((MSG_IC_CHAT_INFINITY*)(pPacket+nBytesUsed))->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_INFINITY, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12010);
		return RES_RETURN_FALSE;
	}
	pRecvChat = (MSG_IC_CHAT_INFINITY*)(pPacket+nBytesUsed);
	chatString = (char*)(pPacket+nBytesUsed+sizeof(MSG_IC_CHAT_INFINITY));
	nBytesUsed += nRecvTypeSize;
	
	if (strnicmp(m_character.CharacterName, pRecvChat->FromCharacterName, SIZE_MAX_CHARACTER_NAME) != 0)
	{
		SendErrorMessage(T_IC_CHAT_INFINITY, ERR_CHAT_CHARACTER_NAME_NOT_MATCHED);
		Close(0x1200B);
		return RES_RETURN_FALSE;
	}

	// 채팅 필터링
	if (!FilterChattingString(chatString))
	{
		SendErrorMessage(T_IC_CHAT_INFINITY, ERR_CHAT_NOT_ALLOWED_STRING);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 채팅 로그 남기기
	char szChatting[1024];
	sprintf(szChatting, "Chat_INFINITY|%s|%d|%d|%s\r\n",
			pRecvChat->FromCharacterName, m_character.MapChannelIndex.MapIndex,
			m_character.MapChannelIndex.ChannelIndex, (char*)pRecvChat + sizeof(MSG_IC_CHAT_INFINITY));
	// 2007-11-13 by cmkwon, 관리자, 운영자 채팅 로그 저장하기 - CIMGlobal::WriteChattingLog() 함수 인자 추가
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	int nRemainMinute = 0;
	if(ms_pIMIOCP->m_chatBlockManager.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName)
		|| ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinute, m_character.CharacterName))	// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - T_IC_CHAT_ARENA, 아레나
	{
		SendErrorMessage(T_IC_CHAT_INFINITY, ERR_CHAT_CHAT_BLOCKED, nRemainMinute);
		return RES_BREAK;
	}

	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_INFINITY, T_FI_CHAT_INFINITY, msgChatRegion, SendBuf);
	msgChatRegion->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
	msgChatRegion->MessageLength = pRecvChat->MessageLength;
	util::strncpy((char*)SendBuf + MSG_SIZE(MSG_FI_CHAT_INFINITY), chatString, SIZE_MAX_CHAT_MESSAGE);

	if (FALSE == this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CHAT_INFINITY) + msgChatRegion->MessageLength))
	{
		// current field server socket이 NULL이면, 접속을 끊는다.
		char	szSystemLog[1024];
		sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
			GetCharacterString(&m_character, string()), GetProtocolTypeString(T_IC_CHAT_INFINITY));
		DBGOUT(szSystemLog);
		g_pGlobal->WriteSystemLog(szSystemLog);
		
		return RES_RETURN_FALSE;
	}
	
	if(i_pThreadInfo)
	{// 2007-02-21 by cmkwon
		i_pThreadInfo->nParam1++;
	}
	
	return RES_RETURN_TRUE;	

}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_FRIENDLIST_AND_REJECTLIST_LOADING(const char* pPacket, int nLength, int &nBytesUsed)
{// No Body
	
	///////////////////////////////////////////////////////////////////////////
	// 친구, 거부 목록 로딩
	DB_FRIEND_INFO	*pDBFriendInfo = new DB_FRIEND_INFO;
	memset(pDBFriendInfo, 0x00, sizeof(DB_FRIEND_INFO));
	util::strncpy(pDBFriendInfo->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 아래와 같이 처음 로딩 인자를 전달 함.
//	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendLoadFriends, this, pDBFriendInfo);
	
	// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 인자를 전달하고 FALSE를 설정한다.
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendLoadFriends, this, pDBFriendInfo, NULL, this->m_bFirstTimeLoadFriendList);
	this->m_bFirstTimeLoadFriendList		= FALSE;

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_FRIENDLIST_INSERT(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_IC_CHAT_FRIENDLIST_INSERT	*pNewMsg = NULL;

	int			nRecvTypeSize = sizeof(MSG_IC_CHAT_FRIENDLIST_INSERT);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12012);
		return RES_RETURN_FALSE;
	}
	pNewMsg = (MSG_IC_CHAT_FRIENDLIST_INSERT*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(0 == strncmp(pNewMsg->szCharacterName, "", SIZE_MAX_CHARACTER_NAME)
		|| 0 == strnicmp(pNewMsg->szCharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME))
	{// 캐릭터명이 잘못되어 있음

		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME);
		return RES_BREAK;
	}

	// 2009-02-26 by cmkwon, 친구리스트 공백 관련 버그 수정 - 친구캐릭터명에 공백이 있는지 체크
	if(NULL != strstr(pNewMsg->szCharacterName, " "))
	{
		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME);
		return RES_BREAK;
	}

	if(COMPARE_INFLUENCE(m_character.InfluenceType, INFLUENCE_TYPE_NORMAL))
	{// 2006-10-14 by cmkwon, 일반 세력은 친구 등록 불가
		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_REQ_CHOOSE_INFLUENCE_TYPE);
		return RES_BREAK;
	}

	CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pNewMsg->szCharacterName);
	if(pSocket
		&& pSocket->IsValidCharacter())
	{
		if (FALSE == COMPARE_RACE(this->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR)
			&& COMPARE_RACE(pSocket->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2006-11-14 by cmkwon, 일반 유저는 관리자,운영자를 친구로 등록불가

			SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME);
			return RES_BREAK;
		}
		
		if(m_character.InfluenceType != pSocket->GetCharacter()->InfluenceType)
		{// 2006-10-17 by cmkwon, 서로 다른 세력은 친구 추가 불가

			SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED);
			return RES_BREAK;
		}

		if(pSocket->IsCharacterNameFromRejectList(this->m_character.CharacterName))
		{// 상대방 거부리스트에 등록되어 있음
	
			SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_REGISTERED_PEER_REJECTLIST, 0, 0, pNewMsg->szCharacterName);	
			return RES_BREAK;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2009-01-13 by cmkwon, 친구 등록시 상대방에게 알림 시스템 적용 - 상대방에게 친구 등록됨을 알린다. 클라이언트에서 등록된 상대방이 등록 여부 체크 
		INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY, T_IC_CHAT_FRIENDLIST_INSERT_NOTIFY, pNotify, SendBuf);
		util::strncpy(pNotify->szDoerCharacName, this->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
		pSocket->SendAddData(SendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY));		
	}
// 2004-11-02 by cmkwon, 친구 등록은 Offline시에도 등록할수 있게 수정
//	CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pNewMsg->szCharacterName);
//	if(NULL == pSocket)
//	{
//		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_COMMON_NO_SUCH_CHARACTER, 0, 0, pNewMsg->szCharacterName);
//		return RES_BREAK;
//	}
//
//	if(pSocket->IsCharacterNameFromRejectList(this->m_character.CharacterName))
//	{// 상대방 거부리스트에 등록되어 있음
//
//		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);	
//		return RES_BREAK;
//	}

	if(this->FindFriendInfoByCharacterName(pNewMsg->szCharacterName))
	{// 이미 친구리스트에 존재함

		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_ALREADY_REGISTERED, 0, 0, pNewMsg->szCharacterName);		
		return RES_BREAK;
	}

	if(SIZE_MAX_FRIENDLIST_COUNT <= m_mtvectorFriendList.size())
	{// 최대 인원

		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_ALREADY_MAX_LIST, m_mtvectorFriendList.size());
		return RES_BREAK;
	}
	
	///////////////////////////////////////////////////////////////////////////
	// 친구에 추가 쿼리
	DB_FRIEND_INFO	*pDBFriendInfo = new DB_FRIEND_INFO;
	util::strncpy(pDBFriendInfo->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pDBFriendInfo->FriendName, pNewMsg->szCharacterName, SIZE_MAX_CHARACTER_NAME);
	pDBFriendInfo->FriendType = FRIEND_TYPE_FRIEND;
	pDBFriendInfo->RegDate.SetCurrentDateTime();		// 2008-04-11 by dhjin, EP3 거부목록 -
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendInsertFriend, this, pDBFriendInfo);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_FRIENDLIST_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_IC_CHAT_FRIENDLIST_DELETE	*pNewMsg = NULL;

	int			nRecvTypeSize = sizeof(MSG_IC_CHAT_FRIENDLIST_DELETE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_FRIENDLIST_DELETE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12012);
		return RES_RETURN_FALSE;
	}
	pNewMsg = (MSG_IC_CHAT_FRIENDLIST_DELETE*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(strncmp(pNewMsg->szCharacterName, "", SIZE_MAX_CHARACTER_NAME) == 0)
	{// 캐릭터명 이 잘못되어 있음

		SendErrorMessage(T_IC_CHAT_FRIENDLIST_DELETE, ERR_FRIEND_INVALID_CHARACTER_NAME);
		return RES_BREAK;
	}
	
	if(FALSE == this->FindFriendInfoByCharacterName(pNewMsg->szCharacterName))
	{// 친구리스트에 존재하지 않음

		SendErrorMessage(T_IC_CHAT_FRIENDLIST_DELETE, ERR_FRIEND_NO_SUCH_CHARACTER_IN_LIST, 0, 0, pNewMsg->szCharacterName);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////
	// 친구리스트에서 삭제 쿼리
	DB_FRIEND_INFO	*pDBFriendInfo = new DB_FRIEND_INFO;
	util::strncpy(pDBFriendInfo->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pDBFriendInfo->FriendName, pNewMsg->szCharacterName, SIZE_MAX_CHARACTER_NAME);
	pDBFriendInfo->FriendType = FRIEND_TYPE_FRIEND;
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendDeleteFriend, this, pDBFriendInfo);
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_FRIENDLIST_REFRESH(const char* pPacket, int nLength, int &nBytesUsed)
{// No Body

	if(0 == m_mtvectorFriendList.size())
	{
		return RES_BREAK;
	}

// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 아래와 같이 수정 함.
// 	INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK, T_IC_CHAT_FRIENDLIST_REFRESH_OK, pSendMsg, tmSendBuf);
// 	pSendMsg->nFriendListCounts = 0;
// 
// 	this->m_mtvectorFriendList.lock();
// 	this->MakeMsg_MSG_IC_CHAT_FRIENDLIST_LOADING_OK(pSendMsg
// 		, (FRIENDINFO *)(tmSendBuf + MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK)), (SIZE_MAX_PACKET-MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK))/sizeof(FRIENDINFO));
// 	this->m_mtvectorFriendList.unlock();
// 
// 	if(pSendMsg->nFriendListCounts > 0)
// 	{
// 		this->SendAddData(tmSendBuf
// 			, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK) + sizeof(FRIENDINFO)*pSendMsg->nFriendListCounts);
// 	}	
	this->SendFriendListRefreshOK();		// 2008-10-24 by cmkwon, 친구리스트 전송 버그 수정 - 
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_REJECTLIST_INSERT(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_IC_CHAT_REJECTLIST_INSERT	*pNewMsg = NULL;

	int			nRecvTypeSize = sizeof(MSG_IC_CHAT_REJECTLIST_INSERT);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_REJECTLIST_INSERT, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12012);
		return RES_RETURN_FALSE;
	}
	pNewMsg = (MSG_IC_CHAT_REJECTLIST_INSERT*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(0 == strncmp(pNewMsg->szCharacterName, "", SIZE_MAX_CHARACTER_NAME))	
	{// 캐릭터명이 잘못되어 있음
		SendErrorMessage(T_IC_CHAT_REJECTLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME);
		return RES_BREAK;
	}

	if(0 == strnicmp(pNewMsg->szCharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME))
	{// 캐릭터명이 잘못되어 있음

		SendErrorMessage(T_IC_CHAT_REJECTLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME, 0, 0, pNewMsg->szCharacterName);
		return RES_BREAK;
	}

	// 2009-02-26 by cmkwon, 친구리스트 공백 관련 버그 수정 - 거부캐릭터명에 공백이 있는지 체크
	if(NULL != strstr(pNewMsg->szCharacterName, " "))
	{
		SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME);
		return RES_BREAK;
	}

	if(this->IsCharacterNameFromRejectList(pNewMsg->szCharacterName))
	{// 이미 거부리스트에 존재함

		SendErrorMessage(T_IC_CHAT_REJECTLIST_INSERT, ERR_FRIEND_ALREADY_REGISTERED, 0, 0, pNewMsg->szCharacterName);		
		return RES_BREAK;
	}

	if(SIZE_MAX_REJECTLIST_COUNT <= m_mtvectorRejectList.size())
	{// 최대 인원

		SendErrorMessage(T_IC_CHAT_REJECTLIST_INSERT, ERR_FRIEND_ALREADY_MAX_LIST, m_mtvectorRejectList.size());
		return RES_BREAK;
	}
// 2004-11-02 by cmkwon, 거부 리스트도 On/Offline 상태와 상관없이 등록 가능
//	CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pNewMsg->szCharacterName);
//	if(NULL == pSocket)
//	{
//		SendErrorMessage(T_IC_CHAT_REJECTLIST_INSERT, ERR_COMMON_NO_SUCH_CHARACTER, 0, 0, pNewMsg->szCharacterName);
//		return RES_BREAK;
//	}
//
//	if(pSocket->FindFriendInfoByCharacterName(this->m_character.CharacterName))
//	{// 상대방 친구리스트에 등록되어 있으므로 삭제해야한다.
//
//		///////////////////////////////////////////////////////////////////////////
//		// 상대방 친구 목록에서 자신을 삭제하는 쿼리
//		DB_FRIEND_INFO	*pDBFriendInfo = new DB_FRIEND_INFO;
//		strncpy(pDBFriendInfo->CharacterName, pSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
//		strncpy(pDBFriendInfo->FriendName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
//		pDBFriendInfo->FriendType = FRIEND_TYPE_FRIEND;
//		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendDeleteFriend, pSocket, pDBFriendInfo);
//	}

	///////////////////////////////////////////////////////////////////////////
	// 상대방 친구 목록에서 자신을 삭제하는 쿼리
	DB_FRIEND_INFO	*pDBFriendInfo = new DB_FRIEND_INFO;
	util::strncpy(pDBFriendInfo->CharacterName, pNewMsg->szCharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pDBFriendInfo->FriendName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pDBFriendInfo->FriendType = FRIEND_TYPE_FRIEND;
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendDeleteFriend, this, pDBFriendInfo);
	
	///////////////////////////////////////////////////////////////////////////
	// 거부리스트 추가 쿼리
	DB_FRIEND_INFO	*pDBFriendInfo1 = new DB_FRIEND_INFO;
	util::strncpy(pDBFriendInfo1->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pDBFriendInfo1->FriendName, pNewMsg->szCharacterName, SIZE_MAX_CHARACTER_NAME);
	pDBFriendInfo1->FriendType = FRIEND_TYPE_REJECT;
	pDBFriendInfo1->RegDate.SetCurrentDateTime();		// 2008-04-11 by dhjin, EP3 거부목록 -
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendInsertFriend, this, pDBFriendInfo1);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_CHAT_REJECTLIST_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
{
	MSG_IC_CHAT_REJECTLIST_DELETE	*pNewMsg = NULL;

	int			nRecvTypeSize = sizeof(MSG_IC_CHAT_REJECTLIST_DELETE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IC_CHAT_REJECTLIST_DELETE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12012);
		return RES_RETURN_FALSE;
	}
	pNewMsg = (MSG_IC_CHAT_REJECTLIST_DELETE*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(strncmp(pNewMsg->szCharacterName, "", SIZE_MAX_CHARACTER_NAME) == 0)
	{// 캐릭터명 이 잘못되어 있음

		SendErrorMessage(T_IC_CHAT_REJECTLIST_DELETE, ERR_FRIEND_INVALID_CHARACTER_NAME);
		return RES_BREAK;
	}
	
	if(FALSE == this->IsCharacterNameFromRejectList(pNewMsg->szCharacterName))
	{// 거부리스트에 존재하지 않음

		SendErrorMessage(T_IC_CHAT_REJECTLIST_DELETE, ERR_FRIEND_NO_SUCH_CHARACTER_IN_LIST, 0, 0, pNewMsg->szCharacterName);
		return RES_BREAK;
	}
	
	///////////////////////////////////////////////////////////////////////////
	// 친구리스트에서 삭제 쿼리
	DB_FRIEND_INFO	*pDBFriendInfo = new DB_FRIEND_INFO;
	util::strncpy(pDBFriendInfo->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	util::strncpy(pDBFriendInfo->FriendName, pNewMsg->szCharacterName, SIZE_MAX_CHARACTER_NAME);
	pDBFriendInfo->FriendType = FRIEND_TYPE_REJECT;
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_FriendDeleteFriend, this, pDBFriendInfo);
	return RES_RETURN_TRUE;
}


BOOL CIMIOCPSocket::SendToFieldServerChatBitFlag(void)
{
	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_CHANGE_CHAT_FLAG, T_FI_CHAT_CHANGE_CHAT_FLAG, pOnOff, pOnOffBuf);
	pOnOff->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	pOnOff->ChatFlag				= m_enableChatFlag8;
	pOnOff->ChatFlag = m_enableChatFlag32;

	this->SendMsgToCurrentFieldServerSocket(pOnOffBuf, MSG_SIZE(MSG_FI_CHAT_CHANGE_CHAT_FLAG));
	return TRUE;
}

void CIMIOCPSocket::SendString128(BYTE string128_type, char* pFormat, ...)
{
	if (string128_type == STRING_128_DEBUG_L1
		|| string128_type == STRING_128_DEBUG_L2)
	{
		return;
	}

	char szStrTemp[1024];
	int nHeaderSize = 0;
	util::zero(szStrTemp, 1024);
	if (string128_type == STRING_128_DEBUG_L1)
	{
		strcpy(szStrTemp, "[D1]");
		nHeaderSize = 4;
	}
	else if (string128_type == STRING_128_DEBUG_L2)
	{
		strcpy(szStrTemp, "[D2]");
		nHeaderSize = 4;
	}
	else if (string128_type == STRING_128_DEBUG_L3)
	{
		strcpy(szStrTemp, "[D3]");
		nHeaderSize = 4;
	}
	else if (string128_type == STRING_128_ADMIN_CMD)
	{
		strcpy(szStrTemp, "[A]");
		nHeaderSize = 3;
	}

	va_list args;
	va_start(args, pFormat);
	vsprintf(szStrTemp+nHeaderSize, pFormat, args);
    va_end(args);

	int nStrLen = strlen(szStrTemp);
	if(128 > nStrLen)
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_STRING_128, T_IC_STRING_128, MsgStr, SendBuf);
		MsgStr->PrintType = STR128_PRINT_TYPE_CHAT;
		util::strncpy(MsgStr->String, szStrTemp, 128);
		SendAddData(SendBuf, MSG_SIZE(MSG_IC_STRING_128));
	}
	else if(256 > nStrLen)
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_STRING_256, T_IC_STRING_256, MsgStr, SendBuf);
		MsgStr->PrintType = STR128_PRINT_TYPE_CHAT;
		util::strncpy(MsgStr->String, szStrTemp, 256);
		SendAddData(SendBuf, MSG_SIZE(MSG_IC_STRING_256));
	}
	else if(512 > nStrLen)
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_STRING_512, T_IC_STRING_512, MsgStr, SendBuf);
		MsgStr->PrintType = STR128_PRINT_TYPE_CHAT;
		util::strncpy(MsgStr->String, szStrTemp, 512);
		SendAddData(SendBuf, MSG_SIZE(MSG_IC_STRING_512));
	}
	else
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_STRING_512, T_IC_STRING_512, MsgStr, SendBuf);
		MsgStr->PrintType = STR128_PRINT_TYPE_CHAT;
		util::strncpy(MsgStr->String, szStrTemp, 512);
		SendAddData(SendBuf, MSG_SIZE(MSG_IC_STRING_512));

		char szErrStr[1024];
 		wsprintf(szErrStr, "  Error: String Too Long For STRING_XXX(Length:%d, %s)\r\n", nStrLen, szStrTemp);
		g_pIMGlobal->WriteSystemLog(szErrStr);
		DbgOut(szErrStr);
	}

	if (string128_type == STRING_128_ADMIN_CMD)
	{
		DBGOUT2(DO2_SYSLOG, "	SendString128 %s: %s\r\n",
			GetCharacterString(&m_character, string()),
			szStrTemp);
	}
}

BOOL CIMIOCPSocket::SendChatMessage(BitFlag16_t i_CHAT_TYPE, BYTE *i_pChatData, int i_nSize, BYTE i_byInfluenceMask/*=INFLUENCE_TYPE_ALL_MASK*/)
{
	if(FALSE == IsUsing()
		|| FALSE == COMPARE_CHATTYPE_BIT(m_enableChatFlag32, i_CHAT_TYPE)
		|| FALSE == COMPARE_INFLUENCE(m_character.InfluenceType, i_byInfluenceMask))
	{
		return FALSE;
	}

	return SendAddData(i_pChatData, i_nSize);
}



ProcessResult CIMIOCPSocket::Process_FI_CONNECT(const char* pPacket, int nLength, int &nBytesUsed)
{

	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CONNECT Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int					nRecvTypeSize = 0;
	MSG_FI_CONNECT		*pMsgConn;

	nRecvTypeSize = sizeof(MSG_FI_CONNECT)
		+ ((MSG_FI_CONNECT*)(pPacket+nBytesUsed))->NumOfMapIndex * sizeof(MapIndex_t);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_CONNECT, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12016);
		return RES_RETURN_FALSE;
	}
	pMsgConn = (MSG_FI_CONNECT*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(ms_pIMIOCP->m_pFieldServerSocket)
	{
		// 이미 FieldServer가 접속해 있음
		SendErrorMessage(T_FI_CONNECT, ERR_PROTOCOL_FIELD_SERVER_ALREADY_REGISTERD);
		return RES_BREAK;
	}

	m_PeerSocketType	= ST_FIELD_SERVER;
	m_FieldServerID		= pMsgConn->FieldServerID;

	// set field server ID
	FieldServerInfo *pFieldServerInfo = &ms_pIMIOCP->m_FieldServerInfo;
	pFieldServerInfo->Reset();
	pFieldServerInfo->serverID						= pMsgConn->FieldServerID;
	pFieldServerInfo->IsActive						= TRUE;
	pFieldServerInfo->ServerType					= ST_FIELD_SERVER;
	pFieldServerInfo->pSocket						= this;
	pFieldServerInfo->AccumulatedFieldUserCounts	= 0;
	pFieldServerInfo->CurrentFieldUserCounts		= 0;
	pFieldServerInfo->MaxFieldUserCounts			= 0;
	pFieldServerInfo->AccumulatedFieldMonsterCounts	= 0;
	pFieldServerInfo->CurrentFieldMonsterCounts		= 0;	
	for (int i = 0; i < pMsgConn->NumOfMapIndex; i++)
	{
		MapIndex_t pTmpMapIndex = *(MapIndex_t*)(((char*)pMsgConn) + sizeof(MSG_FI_CONNECT) + sizeof(MapIndex_t)*i);
		pFieldServerInfo->MapIndexVector.push_back(pTmpMapIndex);
#ifdef _DEBUG
		DBGOUT("FieldServer(%s) -> %04d\n", pFieldServerInfo->serverID.GetString(string()), pTmpMapIndex);
#endif
	}
	ms_pIMIOCP->SetFieldServerSocket(this);

	if(pMsgConn->ArenaServerCheck)
	{// 2008-02-28 by dhjin, 아레나 통합 - Arena 서버이면 TRUE
		ms_pIMIOCP->m_bArenaServerCheck	= TRUE;
	}

	this->SendAddMessageType(T_FI_CONNECT_OK);

	// 2009-03-26 by cmkwon, 시스템 로그 추가 - 
	//DBGOUT("Connection from Field Server(%s) successfully established...\n", pFieldServerInfo->serverID.GetString(string()));
	g_pGlobal->WriteSystemLogEX(TRUE, "  [Notify] Connected from FieldServer(%s) !! IsArenaServerFlag(%d) \r\n", pFieldServerInfo->serverID.GetString().GetBuffer(), pMsgConn->ArenaServerCheck);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CONNECT_NOTIFY_DEAD(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CONNECT_NOTIFY_DEAD Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}

	int							nRecvTypeSize = 0;
	MSG_FI_CONNECT_NOTIFY_DEAD	*pMsgNotifyDead;

	nRecvTypeSize = sizeof(MSG_FI_CONNECT_NOTIFY_DEAD);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_CONNECT_NOTIFY_DEAD, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12017);
		return RES_RETURN_FALSE;
	}
	pMsgNotifyDead = (MSG_FI_CONNECT_NOTIFY_DEAD*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket *pIMSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgNotifyDead->CharacterUniqueNumber);
	if (pIMSocket
		&& pIMSocket->IsUsing())
	{
		pIMSocket->CharacterDeadRoutine(pMsgNotifyDead->AttackerGuildUID);
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CONNECT_NOTIFY_GAMEEND(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()) || this != ms_pIMIOCP->m_pFieldServerSocket)
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CONNECT_NOTIFY_GAMEEND Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int								nRecvTypeSize = 0;
	MSG_FI_CONNECT_NOTIFY_GAMEEND	*pMsgNotifyGameEnd;

	nRecvTypeSize = sizeof(MSG_FI_CONNECT_NOTIFY_GAMEEND);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMEEND, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	pMsgNotifyGameEnd = (MSG_FI_CONNECT_NOTIFY_GAMEEND*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket *pIMSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgNotifyGameEnd->CharacterUniqueNumber);
	if (pIMSocket
		&& pIMSocket->IsUsing()
		&& pIMSocket->m_character.CharacterUniqueNumber == pMsgNotifyGameEnd->CharacterUniqueNumber)
	{
		if (COMPARE_RACE(pIMSocket->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2006-05-08 by cmkwon
			ms_pIMIOCP->DeleteFromAdminCharacter(pIMSocket->GetCharacter()->CharacterUniqueNumber);
		}

		char szTemp[1024];
		sprintf(szTemp, "%s's GameEnd Notified...\r\n", GetCharacterString(&pIMSocket->m_character, string()));
		g_pIMGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		pIMSocket->GameEndRoutineIM();
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CONNECT_NOTIFY_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CONNECT_NOTIFY_GAMESTART Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int								nRecvTypeSize = 0;
	MSG_FI_CONNECT_NOTIFY_GAMESTART	*pMsgNotifyGameStart;

	nRecvTypeSize = sizeof(MSG_FI_CONNECT_NOTIFY_GAMESTART);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgNotifyGameStart = (MSG_FI_CONNECT_NOTIFY_GAMESTART*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	// 사용자 확인
	CIMIOCPSocket *pCharacterSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgNotifyGameStart->CharacterUniqueNumber);
	if (pCharacterSocket == NULL 
		|| FALSE == pCharacterSocket->IsUsing())
	{
		SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_COMMON_NO_SUCH_CHARACTER, pMsgNotifyGameStart->CharacterUniqueNumber);
		return RES_BREAK;
	}

	pCharacterSocket->GameStartRoutineIM(pMsgNotifyGameStart, this);

// 2006-05-09 by cmkwon, GameStartRoutineIM()함수에서 처리
// 	if(COMPARE_RACE(pCharacterSocket->m_character.Race, RACE_OPERATION|RACE_GAMEMASTER))
// 	{// 2006-05-08 by cmkwon, 관리자/운영자는 귓말끔 상태로 시작됨
// 		CLEAR_CHATTYPE_BIT(pCharacterSocket->m_enableChatFlag8, CHAT_TYPE_PTOP);
// 
// 		///////////////////////////////////////////////////////////////////////////////
// 		// 2006-05-08 by cmkwon
// 		ms_pIMIOCP->AddToAdminCharacter(pCharacterSocket->GetCharacter()->CharacterUniqueNumber);
// 		
// 		///////////////////////////////////////////////////////////////////////////////
// 		// 2006-05-08 by cmkwon
// 		int nCallGMCnt = ms_pIMIOCP->m_mtvectSCallGMList.size();
// 		if(0 < nCallGMCnt)
// 		{
// 			INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_CALLGM_INFO_OK, T_IC_ADMIN_CALLGM_INFO_OK, pCallGMInfo, SendBuf);
// 			pCallGMInfo->nCallGMTotalCount	= ms_pIMIOCP->m_mtvectSCallGMList.size();
// 			pCharacterSocket->SendAddData(SendBuf, MSG_SIZE(MSG_IC_ADMIN_CALLGM_INFO_OK));
// 		}
// 	}
// 
// 	pCharacterSocket->m_bGameEndRoutineFlag			= TRUE;
// 
// 	///////////////////////////////////////////////////////////////////////////
// 	// MAP_CHANNEL_INDEX 할당	
// 	pCharacterSocket->m_character.MapChannelIndex	= pMsgNotifyGameStart->MapChannelIndex;
// 
// 	///////////////////////////////////////////////////////////////////////////
// 	// 길드 정보 로딩	
// 	if(INVALID_GUILD_UID != pCharacterSocket->m_character.GuildUniqueNumber)
// 	{
// 		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildLoadGuild, this, NULL, NULL, pMsgNotifyGameStart->CharacterUniqueNumber);
// 	}	
// 
// 	///////////////////////////////////////////////////////////////////////////
// 	// 이전 파티 검사
// 	///////////////////////////////////////////////////////////////////////////
// 	if (pCharacterSocket->m_character.LastPartyID != 0)
// 	{
// #ifdef _DEBUG
// 		pCharacterSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0005);
// #endif
// 		// 파티 검색
// 		CIMParty *pParty = ms_pIMIOCP->FindParty(pCharacterSocket->m_character.LastPartyID);
// 		if (pParty == NULL)
// 		{
// 			// 파티가 이미 해산된 경우
// 			pCharacterSocket->SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_PROTOCOL_NO_SUCH_PARTY, pCharacterSocket->m_character.LastPartyID);
// 
// 			pCharacterSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0006);
// 
// 			// Update DB
// 			QPARAM_SAVELASTPARTYID *pQSavePartyID = new QPARAM_SAVELASTPARTYID;
// 			pQSavePartyID->CharacterUniqueNumber = pCharacterSocket->m_character.CharacterUniqueNumber;
// 			pQSavePartyID->LastPartyID = 0;
// 			ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_SaveLastPartyID, this, pQSavePartyID);
// 
// 			// Update CHARACTER
// 			pCharacterSocket->m_character.LastPartyID = 0;
// 			return RES_BREAK;
// 		}
// 		
// 		pParty->LockIMParty();
// 		{
// 			IMPartyMember *pMember = pParty->GetMemberPtrByCharacterUniqueNumber(pMsgNotifyGameStart->CharacterUniqueNumber);
// 			if (pMember == NULL)
// 			{
// 				// 파티원이 없는 경우
// 				pParty->UnlockIMParty();
// 				SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pMsgNotifyGameStart->CharacterUniqueNumber);
// 				return RES_BREAK;
// 			}
// 
// 			// 사용자에게 파티 정보 할당
// 			pCharacterSocket->SetIMParty(pParty);
// 
// 			// validate party member
// 			pMember->SetOnlineMember(pCharacterSocket);
// 
// 			// 자기 자신에게 보냄
// 			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_PUT_LAST_PARTY_INFO, T_IC_PARTY_PUT_LAST_PARTY_INFO, pLastPartyInfo, pLastPartyInfoBuf);
// 			pLastPartyInfo->PartyID = pParty->m_PartyID;
// 			pCharacterSocket->SendAddData(pLastPartyInfoBuf, MSG_SIZE(MSG_IC_PARTY_PUT_LAST_PARTY_INFO));
// 
// 			pCharacterSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0007);
// 		}
// 		pParty->UnlockIMParty();
// 	}

	// 2008-03-03 by dhjin, 아레나 통합 - 아레나 서버 일때만 유저에게 FM <-> IM서버 접속 성공을 보낸다
	if(ms_pIMIOCP->m_bArenaServerCheck)
	{
		pCharacterSocket->SendAddMessageType(T_IC_CONNECT_FM_TO_IM_OK);
	}

	return RES_RETURN_TRUE;
}


ProcessResult CIMIOCPSocket::Process_FI_CONNECT_NOTIFY_DEAD_GAMESTART(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CONNECT_NOTIFY_DEAD_GAMESTART Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int										nRecvTypeSize = 0;
	MSG_FI_CONNECT_NOTIFY_DEAD_GAMESTART	*pDeadGameStart;

	nRecvTypeSize = sizeof(MSG_FI_CONNECT_NOTIFY_DEAD_GAMESTART);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_CONNECT_NOTIFY_DEAD_GAMESTART, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pDeadGameStart = (MSG_FI_CONNECT_NOTIFY_DEAD_GAMESTART*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	// 사용자 확인
	CIMIOCPSocket *pCharacterSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pDeadGameStart->CharacterUniqueNumber);
	if (pCharacterSocket == NULL || FALSE == pCharacterSocket->IsUsing())
	{
		SendErrorMessage(T_FI_CONNECT_NOTIFY_DEAD_GAMESTART, ERR_COMMON_NO_SUCH_CHARACTER, pDeadGameStart->CharacterUniqueNumber);
		return RES_BREAK;
	}

	CLEAR_BODYCON_BIT(pCharacterSocket->m_character.BodyCondition, BODYCON_DEAD_MASK);
	return RES_RETURN_TRUE;
}


ProcessResult CIMIOCPSocket::Process_FI_EVENT_NOTIFY_WARP(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_EVENT_NOTIFY_WARP Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int							nRecvTypeSize = 0;
	MSG_FI_EVENT_NOTIFY_WARP	*pMsgNotifyWarp;

	nRecvTypeSize = sizeof(MSG_FI_EVENT_NOTIFY_WARP);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_EVENT_NOTIFY_WARP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	pMsgNotifyWarp = (MSG_FI_EVENT_NOTIFY_WARP*)(pPacket+nBytesUsed);
	nRecvTypeSize += pMsgNotifyWarp->nNumOfTimer*sizeof(TIMER_EVENT_4_EXCHANGE);
	nBytesUsed += nRecvTypeSize;
	
	CIMIOCPSocket *pCharacterSocket
		= ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgNotifyWarp->CharacterUniqueNumber);
	if (pCharacterSocket == NULL || FALSE == pCharacterSocket->IsUsing())
	{
		DBGOUT("[Error]ERR_COMMON_NO_SUCH_CHARACTER: %d\n", pMsgNotifyWarp->CharacterUniqueNumber);
		SendErrorMessage(T_FI_EVENT_NOTIFY_WARP, ERR_COMMON_NO_SUCH_CHARACTER, pMsgNotifyWarp->CharacterUniqueNumber);
		return RES_BREAK;
	}

	if (NULL == ms_pIMIOCP->m_pFieldServerSocket 
		|| FALSE == ms_pIMIOCP->m_pFieldServerSocket->IsUsing()
		|| FALSE == ms_pIMIOCP->m_FieldServerInfo.IsExistMapIndex(pMsgNotifyWarp->MapChannelIndex.MapIndex))
	{
		DBGOUT("[Error]ERR_PROTOCOL_NO_SUCH_MAP_SERVED: %d\n", pMsgNotifyWarp->MapChannelIndex.MapIndex);
		SendErrorMessage(T_FI_EVENT_NOTIFY_WARP, ERR_PROTOCOL_NO_SUCH_MAP_SERVED, pMsgNotifyWarp->CharacterUniqueNumber, pMsgNotifyWarp->MapChannelIndex.MapIndex);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 여단전 체크
	if (IS_VALID_UNIQUE_NUMBER(pCharacterSocket->m_uidGuildUniqueNumber))
	{
		ms_pIMIOCP->GuildWarCheck(pCharacterSocket->m_uidGuildUniqueNumber, pCharacterSocket->m_character.MapChannelIndex);
	}

	pCharacterSocket->m_character.MapChannelIndex = pMsgNotifyWarp->MapChannelIndex;
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_EVENT_CHAT_BLOCK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 
/// \author		cmkwon
/// \date		2008-12-30 ~ 2008-12-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_EVENT_CHAT_BLOCK(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_EVENT_CHAT_BLOCK Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_EVENT_CHAT_BLOCK,
		MSG_FI_EVENT_CHAT_BLOCK, pRMsg);
	
	ms_pIMIOCP->m_chatBlockManagerByLeader.InsertChatBlockList(pRMsg->szBlockedCharacterName, pRMsg->nBlockedMinutes);
	return RES_RETURN_TRUE;
}

// 2004-10-29 by cmkwon, FieldServer는 하나로 픽스한다
//ProcessResult CIMIOCPSocket::Process_FI_EVENT_GET_WARP_INFO(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_FI_EVENT_GET_WARP_INFO	*pMsgGetWarpInfo = NULL;
//
//	nRecvTypeSize = sizeof(MSG_FI_EVENT_GET_WARP_INFO);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_FI_EVENT_GET_WARP_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pMsgGetWarpInfo = (MSG_FI_EVENT_GET_WARP_INFO*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CIMIOCPSocket *pSock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgGetWarpInfo->CharacterUniqueNumber);
//	if (pSock == NULL)
//	{
//		SendErrorMessage(T_FI_EVENT_GET_WARP_INFO, ERR_COMMON_NO_SUCH_CHARACTER, pMsgGetWarpInfo->CharacterUniqueNumber);
//		return RES_BREAK;
//	}
//
//	// 에러가 나면 바로 보내기 위해 미리 만들어둠
//	INIT_MSG_WITH_BUFFER(MSG_FI_EVENT_GET_WARP_INFO_OK, T_FI_EVENT_GET_WARP_INFO_OK, pGetOK, pGetOKBuf);
//	pGetOK->AccountUniqueNumber = pSock->m_character.AccountUniqueNumber;
//	pGetOK->CharacterUniqueNumber = pSock->m_character.CharacterUniqueNumber;
//	pGetOK->PartyID = 0;
//	pGetOK->MasterCharacterUniqueNumber = INVALID_UNIQUE_NUMBER;
//	pGetOK->nPartyMembers = 0;
//	pGetOK->nTimerEvents = 0;
//
//	int offset = MSG_SIZE(MSG_FI_EVENT_GET_WARP_INFO_OK);
//
//	// 소속 파티가 있으면 할당함
//	if (pSock->m_pIMParty != NULL)
//	{
//		pSock->m_pIMParty->m_ListPartyMember.lock();
//		pGetOK->PartyID						= pSock->m_pIMParty->m_PartyID;
//		pGetOK->MasterCharacterUniqueNumber	= pSock->m_pIMParty->GetMasterCharacterUniqueNumber();
//		pGetOK->nPartyMembers				= pSock->m_pIMParty->GetMemberCount(FALSE);
//
//		mt_list<IMPartyMember>::iterator itr = pSock->m_pIMParty->m_ListPartyMember.begin();
//		while(itr != pSock->m_pIMParty->m_ListPartyMember.end())
//		{
//			IMPartyMember &tmpPartyMember = *itr;
//			FI_PARTY_MEMBER_INFO *pPartyMember = (FI_PARTY_MEMBER_INFO*)(pGetOKBuf + offset);
//			pPartyMember->CharacterUniqueNumber = tmpPartyMember.PM_CharacterUniqueNumber;
//
//			offset += sizeof(FI_PARTY_MEMBER_INFO);
//			itr++;
//		}
//		pSock->m_pIMParty->m_ListPartyMember.unlock();
//#ifdef _DEBUG
//		// offset 확인
//		assert(offset
//			== (MSG_SIZE(MSG_FI_EVENT_GET_WARP_INFO_OK) + sizeof(FI_PARTY_MEMBER_INFO)*pSock->m_pIMParty->GetMemberCount(FALSE))
//		);
//#endif
//	}
//
//
//#ifdef _DEBUG
//	// offset 확인
//	assert(offset == (MSG_SIZE(MSG_FI_EVENT_GET_WARP_INFO_OK) +
//						sizeof(FI_PARTY_MEMBER_INFO)*pGetOK->nPartyMembers +
//						sizeof(TIMER_EVENT_4_EXCHANGE)*pGetOK->nTimerEvents)
//	);
//#endif
//
//	// 전송
//	SendAddData(pGetOKBuf, offset);
//
//	return RES_RETURN_TRUE;
//}

ProcessResult CIMIOCPSocket::Process_FI_CONNECT_NOTIFY_FIELDSERVER_IP(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CONNECT_NOTIFY_FIELDSERVER_IP Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int								nRecvTypeSize = 0;
	MSG_FI_CONNECT_NOTIFY_FIELDSERVER_IP	*pMsgNotifyIP;

	nRecvTypeSize = sizeof(MSG_FI_CONNECT_NOTIFY_FIELDSERVER_IP);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_CONNECT_NOTIFY_FIELDSERVER_IP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12018);
		return RES_RETURN_FALSE;
	}
	pMsgNotifyIP = (MSG_FI_CONNECT_NOTIFY_FIELDSERVER_IP*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket *pCharacterSocket
		= ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgNotifyIP->CharacterUniqueNumber);
	if (pCharacterSocket == NULL)
	{
		DBGOUT("[Error]ERR_COMMON_NO_SUCH_CHARACTER: %d\n", pMsgNotifyIP->CharacterUniqueNumber);
		SendErrorMessage(T_FI_CONNECT_NOTIFY_FIELDSERVER_IP, ERR_COMMON_NO_SUCH_CHARACTER, pMsgNotifyIP->CharacterUniqueNumber);
		return RES_BREAK;
	}

	if(NULL == ms_pIMIOCP->m_pFieldServerSocket
		|| FALSE == ms_pIMIOCP->m_pFieldServerSocket->IsUsing()
		|| ms_pIMIOCP->m_FieldServerInfo.serverID != pMsgNotifyIP->FieldServerID)
	{
		DBGOUT("[Error]ERR_PROTOCOL_NO_SUCH_FIELD_SERVER: %s\n", pMsgNotifyIP->FieldServerID.GetString().GetBuffer());
		SendErrorMessage(T_FI_CONNECT_NOTIFY_FIELDSERVER_IP, ERR_PROTOCOL_NO_SUCH_FIELD_SERVER, pMsgNotifyIP->FieldServerID.port, 0, pMsgNotifyIP->FieldServerID.IP);
		return RES_BREAK;
	}

	if ( pCharacterSocket->m_pCurrentFieldServerSocket != this)
	{
		pCharacterSocket->SetCurrentFieldServerSocket(this);
	}

	DBGOUT("%s's FieldServer IP Notified: %s\n", pCharacterSocket->m_character.AccountName, pMsgNotifyIP->FieldServerID.GetString().GetBuffer());
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_CREATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int					nRecvTypeSize;
	MSG_IC_PARTY_CREATE	*pMsgPartyCreate;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_CREATE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_IC_PARTY_CREATE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	pMsgPartyCreate = (MSG_IC_PARTY_CREATE*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (this->GetIMParty() != nullptr
#ifdef FREEWAR_ // 2015-11-04 DevX does not let client in free war map create a party
		|| isFreeForAllMap(this->m_character.MapChannelIndex.MapIndex)
#endif
		)
	{
		// error
		SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0008);
		SendErrorMessage(T_IC_PARTY_CREATE, ERR_PROTOCOL_PARTY_MEMBER_ALREADY_EXISTS);
		return RES_BREAK;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	if(pMsgPartyCreate->PartyInfo.PartyInfluence != this->m_character.InfluenceType)
	{// 2008-06-02 by dhjin, 세력이 오류
		SendErrorMessage(T_IC_PARTY_CREATE, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED, pMsgPartyCreate->PartyInfo.PartyInfluence, this->m_character.InfluenceType);	// 2008-12-04 by cmkwon, 정보 추가
		return RES_BREAK;		
	}

	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	// 2009-12-29 by cmkwon, 캐릭터 최대 레벨 상향(110으로) - 아래와 같이 수정
	//if(pMsgPartyCreate->PartyInfo.MinLevel > pMsgPartyCreate->PartyInfo.MaxLevel
	//	|| pMsgPartyCreate->PartyInfo.MinLevel <= 0
	//	|| pMsgPartyCreate->PartyInfo.MaxLevel > 100
	//	)
	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-29 by cmkwon, 캐릭터 최대 레벨 상향(110으로) - 
	if(pMsgPartyCreate->PartyInfo.MinLevel > pMsgPartyCreate->PartyInfo.MaxLevel
		|| FALSE == util::in_range(1, pMsgPartyCreate->PartyInfo.MinLevel, CHARACTER_MAX_LEVEL)
		|| FALSE == util::in_range(1, pMsgPartyCreate->PartyInfo.MaxLevel, CHARACTER_MAX_LEVEL)
		)
	{
		SendErrorMessage(T_IC_PARTY_CREATE, ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED, pMsgPartyCreate->PartyInfo.MinLevel, pMsgPartyCreate->PartyInfo.MaxLevel);
		return RES_BREAK;
	}

	CIMParty *ptmParty = new CIMParty(ms_pIMIOCP, m_character.CharacterUniqueNumber, timeGetTime(), ms_pIMIOCP->m_UID32GenPartyID.GetNext(), &(pMsgPartyCreate->PartyInfo) );	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	IMPartyMember tmMember(m_character.CharacterName, m_character.CharacterUniqueNumber, this, ptmParty);
	ptmParty->PushBackMember(tmMember);

	this->SetIMParty(ptmParty);

	// 2008-02-28 by dhjin,	아레나 통합 - LastPartyID에 값을 저장한다.
	this->m_character.LastPartyID	= ptmParty->m_PartyID;

	// Make msg
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_CREATE_OK, T_FI_PARTY_CREATE_OK, pMsgPartyCreateOK, pMsgPartyCreateOKBuf);
	pMsgPartyCreateOK->CharacterUniqueNumber = pMsgPartyCreate->CharacterUniqueNumber;
	pMsgPartyCreateOK->PartyID = ptmParty->m_PartyID;
	pMsgPartyCreateOK->FieldPartyInfo.nTotalPartyMember = 1;					// 초기엔 1명
	pMsgPartyCreateOK->FieldPartyInfo.lowestMemberLevel = m_character.Level;	// 초기엔 자신의 level
	pMsgPartyCreateOK->ExpDistributeType	= pMsgPartyCreate->PartyInfo.ExpDistributeType;			// 2008-06-03 by dhjin, EP3 편대 수정
	pMsgPartyCreateOK->ItemDistributeType	= pMsgPartyCreate->PartyInfo.ItemDistributeType;		// 2008-06-03 by dhjin, EP3 편대 수정


	// 파티들의 list에 추가
	ms_pIMIOCP->InsertParty(ptmParty);

	if (FALSE == this->SendMsgToCurrentFieldServerSocket(pMsgPartyCreateOKBuf, MSG_SIZE(MSG_FI_PARTY_CREATE_OK)))
	{
		// 에러이므로 파티들의 list에서 삭제
		ms_pIMIOCP->DeleteParty(ptmParty->m_PartyID);

		// current field server socket이 NULL이면, 접속을 끊는다.
		char	szSystemLog[1024];
		sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
			GetCharacterString(&m_character, string()), GetProtocolTypeString(T_IC_PARTY_CREATE));
		DBGOUT(szSystemLog);
		g_pGlobal->WriteSystemLog(szSystemLog);

		return RES_RETURN_FALSE;
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_GET_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize;
	MSG_IC_PARTY_GET_MEMBER	*pGetMember;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_GET_MEMBER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_IC_PARTY_GET_MEMBER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	pGetMember = (MSG_IC_PARTY_GET_MEMBER*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMParty *ptmParty = this->GetIMParty();
	if (NULL == ptmParty)
	{
		SendErrorMessage(T_IC_PARTY_GET_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();
	// init msg
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_PUT_MEMBER, T_IC_PARTY_PUT_MEMBER, pPutMember, pPutMemberBuf);
	{
		IMPartyMember *pMember = ptmParty->GetMemberPtrByCharacterUniqueNumber(pGetMember->CharacterUniqueNumber);
		if (NULL == pMember)
		{
			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_GET_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER);
			return RES_BREAK;
		}

		if (pMember->IsOnlineMember())
		{
			
			if (FALSE == pMember->PM_pIMIOCPSocket->IsUsing()
				|| pMember->PM_CharacterUniqueNumber != pMember->PM_pIMIOCPSocket->m_character.CharacterUniqueNumber)
			{
				ptmParty->UnlockIMParty();
				SendErrorMessage(T_IC_PARTY_GET_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER);
				g_pGlobal->WriteSystemLogEX(TRUE, STRMSG_S_I2NOTIFY_0009);
				return RES_BREAK;
			}

			pPutMember->IMPartyMemberInfo = pMember->PM_pIMIOCPSocket->m_character;
			pPutMember->IMPartyMemberInfo.VoipType = pMember->PM_pIMIOCPSocket->m_VoipType;		// 2008-07-16 by dhjin, EP3 - Voip 정보
		}
		else
		{
			pPutMember->IMPartyMemberInfo = *pMember;
			pPutMember->IMPartyMemberInfo.VoipType = CHECK_TYPE_VOIP_NONE;		// 2008-07-16 by dhjin, EP3 - Voip 정보	
		}
	}
	ptmParty->UnlockIMParty();


	// 데이터 전송
	SendAddData(pPutMemberBuf, MSG_SIZE(MSG_IC_PARTY_PUT_MEMBER));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_GET_ALL_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_IC_PARTY_GET_ALL_MEMBER	*pGetAllMember;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_GET_ALL_MEMBER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_IC_PARTY_GET_ALL_MEMBER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	pGetAllMember = (MSG_IC_PARTY_GET_ALL_MEMBER*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMParty *ptmParty = this->GetIMParty();
	if (NULL == ptmParty)
	{
		SendErrorMessage(T_IC_PARTY_GET_ALL_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	// init msg
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_PUT_ALL_MEMBER, T_IC_PARTY_PUT_ALL_MEMBER, pPutAllMember, pPutAllMemberBuf);
	
	ptmParty->LockIMParty();	
	{
		pPutAllMember->PartyID				= ptmParty->m_PartyID;
		pPutAllMember->MasterUniqueNumber	= ptmParty->GetMasterCharacterUniqueNumber();

		// 데이터 할당
		ptmParty->MakeMsg_IC_PARTY_PUT_ALL_MEMBER(&pPutAllMember->nNumOfPartyMembers, (IM_PARTY_MEMBER_INFO*)(pPutAllMemberBuf+MSG_SIZE(MSG_IC_PARTY_PUT_ALL_MEMBER)), m_character.CharacterUniqueNumber);

// 2004-10-29 by cmkwon, 위의 함수에서 처리
// 데이터 할당
//		pPutAllMember->nNumOfPartyMembers = m_pIMParty->GetMemberCount(FALSE) - 1;	// 자기 자신은 제외함
//		IM_PARTY_MEMBER_INFO *ArrIMPartyInfo = (IM_PARTY_MEMBER_INFO*)(pPutAllMemberBuf+MSG_SIZE(MSG_IC_PARTY_PUT_ALL_MEMBER));
//		list<IMPartyMember>::iterator itr = m_pIMParty->m_ListPartyMember.begin();
//		int count = 0;
//		while(m_pIMParty->m_ListPartyMember.end() != itr)
//		{
//			IMPartyMember &tmpPartyMember = *itr;
//
//			// 자신의 정보는 보내지 않음
//			if (tmpPartyMember.PM_CharacterUniqueNumber != m_character.CharacterUniqueNumber)
//			{
//				if (tmpPartyMember.IsOnlineMember())
//				{
//					CIMIOCPSocket *pSocket = tmpPartyMember.PM_pIMIOCPSocket;
//					if (NULL == pSocket 
//						|| FALSE == pSocket->IsUsing()
//						|| tmpPartyMember.PM_CharacterUniqueNumber != pSocket->m_character.CharacterUniqueNumber)
//					{
//						m_pIMParty->UnlockIMParty();	// unlock
//
//						SendErrorMessage(T_IC_PARTY_GET_ALL_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER);
//						g_pGlobal->WriteSystemLogEX(TRUE, "  Fatal Error: 심각한 파티 에러! at T_IC_PARTY_GET_MEMBER");
//						return RES_BREAK;
//					}
//
//					ArrIMPartyInfo[count] = pSocket->m_character;
//				}
//				else
//				{
//					ArrIMPartyInfo[count] = tmpPartyMember;
//				}
//
//				count++;
//			}
//
//			itr++;
//		}
	}// LockIMParty_end
	ptmParty->UnlockIMParty();	// unlock

	// 데이터 전송
	SendAddData(pPutAllMemberBuf,
		MSG_SIZE(MSG_IC_PARTY_PUT_ALL_MEMBER) +
		pPutAllMember->nNumOfPartyMembers*sizeof(IM_PARTY_MEMBER_INFO));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_PARTY_ACCEPT_INVITE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_PARTY_ACCEPT_INVITE_OK Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int								nRecvTypeSize = 0;
	MSG_FI_PARTY_ACCEPT_INVITE_OK	*pMsgAcceptOK;

	nRecvTypeSize = sizeof(MSG_FI_PARTY_ACCEPT_INVITE_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgAcceptOK = (MSG_FI_PARTY_ACCEPT_INVITE_OK*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket *pInviteeSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgAcceptOK->CharacterUniqueNumber);
	if(NULL == pInviteeSocket || FALSE == pInviteeSocket->IsUsing())
	{
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-03 by cmkwon, 편대 2중 초대 버그 수정(E014153) - 
	if(pInviteeSocket->GetIMParty())
	{
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK, ERR_PROTOCOL_PARTY_MEMBER_ALREADY_EXISTS);	
		return RES_BREAK;
	}
	
	if(this->IsCharacterNameFromRejectList(pInviteeSocket->m_character.CharacterName)
		|| pInviteeSocket->IsCharacterNameFromRejectList(this->m_character.CharacterName))
	{// 2008-07-03 by dhjin, EP3 거부목록 -
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);	
		return RES_BREAK;
	}
	
	CIMParty *ptmParty = ms_pIMIOCP->FindParty(pMsgAcceptOK->PartyID);
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgAcceptOK->PartyID, pMsgAcceptOK->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	ptmParty->LockIMParty();

	if ( ptmParty->GetMemberCount(FALSE) >= SIZE_MAX_PARTY_MEMBER )
	{	// 여기서 한번더 제한 인원 체크 하도록 수정.
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK , ERR_PROTOCOL_ALREADY_MAX_PARTY_MEMBER , pMsgAcceptOK->PartyID , pMsgAcceptOK->CharacterUniqueNumber );

		pInviteeSocket->SendErrorMessage ( T_FI_PARTY_ACCEPT_INVITE_OK , ERR_PROTOCOL_ALREADY_MAX_PARTY_MEMBER );

		ptmParty->UnlockIMParty();

		return RES_BREAK;
	}
	else
	{
		// party 할당
		pInviteeSocket->SetIMParty(ptmParty);

		// party원 삽입
		IMPartyMember tmMember(pInviteeSocket->m_character.CharacterName, pInviteeSocket->m_character.CharacterUniqueNumber, pInviteeSocket, ptmParty);
		ptmParty->PushBackMember(tmMember);

		// 2008-02-28 by dhjin,	아레나 통합 - LastPartyID에 값을 저장한다.
		pInviteeSocket->m_character.LastPartyID	= pMsgAcceptOK->PartyID;

		// send to SERVER
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_ACCEPT_INVITE_OK, T_FI_PARTY_ACCEPT_INVITE_OK, pInviteOKFI, pInviteOKFIBuf);
		pInviteOKFI->PartyID				= pMsgAcceptOK->PartyID;
		pInviteOKFI->CharacterUniqueNumber	= pMsgAcceptOK->CharacterUniqueNumber;
		pInviteOKFI->FieldPartyInfo.nTotalPartyMember = ptmParty->GetMemberCount(FALSE);		// 주의: m_ListPartyMember에 삽입 이후여야 함!
		pInviteOKFI->FieldPartyInfo.lowestMemberLevel = ptmParty->GetLowestMemberLevel();		// 주의: m_ListPartyMember에 삽입 이후여야 함!
		pInviteeSocket->SendMsgToCurrentFieldServerSocket(pInviteOKFIBuf, MSG_SIZE(MSG_FI_PARTY_ACCEPT_INVITE_OK));

		// client들에게 전송하기
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_ACCEPT_INVITE_OK, T_IC_PARTY_ACCEPT_INVITE_OK, pInviteOKIC, pInviteOKICBuf);
		pInviteOKIC->PartyID			= pMsgAcceptOK->PartyID;
		pInviteOKIC->IMPartyMemberInfo	= pInviteeSocket->m_character;
		ptmParty->SendMsgToMembers(pInviteOKICBuf, MSG_SIZE(MSG_IC_PARTY_ACCEPT_INVITE_OK));

		// 2008-06-10 by dhjin, EP3 편대 수정 - 편대 정보 전송
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_INFO, T_IC_PARTY_INFO, pSendMsg, pSendBuf);
		pSendMsg->PartyInfo = ptmParty->m_SPartyInfo;
		pInviteeSocket->SendAddData(pSendBuf, MSG_SIZE(MSG_IC_PARTY_INFO));
	}
	ptmParty->UnlockIMParty();

//#ifdef _DEBUG
//	char *pdm = NULL;
//	pInviteeSocket->PRINT_DEBUG_MESSAGE(pdm);
//#endif
	return RES_RETURN_TRUE;
}

//////////////////////////////////////////////////////////////////////
// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크할 팀번호 설정
ProcessResult CIMIOCPSocket::Process_FI_PARTY_ARENA_TEAM_NUM(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_PARTY_ARENA_TEAM_NUM Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int								nRecvTypeSize = 0;
	MSG_FI_PARTY_ARENA_TEAM_NUM		*pMsgArenaTeamInfo;
	
	nRecvTypeSize = sizeof(MSG_FI_PARTY_ARENA_TEAM_NUM);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_PARTY_ARENA_TEAM_NUM, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	pMsgArenaTeamInfo = (MSG_FI_PARTY_ARENA_TEAM_NUM*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket *pInviteeSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgArenaTeamInfo->CharacterUniqueNumber);
	// 2012-09-24 by jhseol, 소스정리 - 소켓 유효성 검사
	if( NULL == pInviteeSocket )
	{
		SendErrorMessage(T_FI_PARTY_ARENA_TEAM_NUM, ERR_PROTOCOL_INVALID_CLIENTINDEX);
		return RES_BREAK;
	}
	// end 2012-09-24 by jhseol, 소스정리 - 소켓 유효성 검사

	pInviteeSocket->m_nTeamNum = pMsgArenaTeamInfo->TeamNum;
	return RES_RETURN_TRUE;
}
// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 같은 팀인지 체크할 팀번호 설정

ProcessResult CIMIOCPSocket::Process_FI_PARTY_UPDATE_ITEM_POS(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_PARTY_UPDATE_ITEM_POS Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int								nRecvTypeSize = 0;
	MSG_FI_PARTY_UPDATE_ITEM_POS	*pFIUpdateItemPos;

	nRecvTypeSize = sizeof(MSG_FI_PARTY_UPDATE_ITEM_POS);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		SendErrorMessage(T_FI_PARTY_UPDATE_ITEM_POS, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pFIUpdateItemPos = (MSG_FI_PARTY_UPDATE_ITEM_POS*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	// 사용자 확인
	CIMIOCPSocket *pCharacterSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pFIUpdateItemPos->CharacterUniqueNumber);
	if (pCharacterSocket == NULL || FALSE == pCharacterSocket->IsUsing())
	{
		SendErrorMessage(T_FI_PARTY_UPDATE_ITEM_POS, ERR_COMMON_NO_SUCH_CHARACTER, pFIUpdateItemPos->CharacterUniqueNumber);
		return RES_BREAK;
	}

	// 파티 확인
	CIMParty *pParty = pCharacterSocket->GetIMParty();
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_UPDATE_ITEM_POS, ERR_PROTOCOL_NO_SUCH_PARTY, pFIUpdateItemPos->CharacterUniqueNumber);
		return RES_BREAK;
	}
	pParty->LockIMParty();
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_UPDATE_ITEM_POS, T_IC_PARTY_UPDATE_ITEM_POS, pICUpdateItemPos, pICUpdateItemPosBuf);
		pICUpdateItemPos->CharacterUniqueNumber = pFIUpdateItemPos->CharacterUniqueNumber;
		pICUpdateItemPos->ItemPosition			= pFIUpdateItemPos->ItemPosition;
		pICUpdateItemPos->ItemNum				= pFIUpdateItemPos->ItemNum;
		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
		//pICUpdateItemPos->ColorCode				= pFIUpdateItemPos->ColorCode;		// 2005-12-08 by cmkwon, 아머 색상 튜닝 정보
		pICUpdateItemPos->nShapeItemNum			= pFIUpdateItemPos->nShapeItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
		pICUpdateItemPos->nEffectItemNum		= pFIUpdateItemPos->nEffectItemNum;		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
		pParty->SendMsgToMembers(pICUpdateItemPosBuf, MSG_SIZE(MSG_IC_PARTY_UPDATE_ITEM_POS));
	}
	pParty->UnlockIMParty();

	return RES_RETURN_TRUE;
}

///*
//ProcessResult CIMIOCPSocket::Process_FI_PARTY_GET_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_FI_PARTY_GET_PARTY_INFO	*pMsgGetPartyInfo = NULL;
//
//	nRecvTypeSize = sizeof(MSG_FI_PARTY_GET_PARTY_INFO);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_FI_PARTY_GET_PARTY_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pMsgGetPartyInfo = (MSG_FI_PARTY_GET_PARTY_INFO*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CIMIOCPSocket *pSock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgGetPartyInfo->CharacterUniqueNumber);
//	if (pSock == NULL)
//	{
//		SendErrorMessage(T_FI_PARTY_GET_PARTY_INFO, ERR_COMMON_NO_SUCH_CHARACTER, pMsgGetPartyInfo->CharacterUniqueNumber);
//		return RES_BREAK;
//	}
//
//	// 에러가 나면 바로 보내기 위해 미리 만들어둠
//	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_GET_PARTY_INFO_OK, T_FI_PARTY_GET_PARTY_INFO_OK, pGetOK, pGetOKBuf);
//	pGetOK->AccountUniqueNumber = pSock->m_character.AccountUniqueNumber;
//	pGetOK->CharacterUniqueNumber = pSock->m_character.CharacterUniqueNumber;
//	pGetOK->MasterCharacterUniqueNumber = INVALID_UNIQUE_NUMBER;
//	pGetOK->NumOfPartyMembers = 0;
//
//	if (pSock->m_pIMParty == NULL)
//	{
//		// 소속 파티가 없으므로 그냥 보냄
//		SendAddData(pGetOKBuf, MSG_SIZE(MSG_FI_PARTY_GET_PARTY_INFO_OK));
//		return RES_BREAK;
//	}
//
//	// 파티가 있으므로 채워서 보냄
//	pGetOK->MasterCharacterUniqueNumber = pSock->m_pIMParty->GetMasterCharacterUniqueNumber();
//	pGetOK->NumOfPartyMembers = pSock->m_pIMParty->GetMemberCount(FALSE);
//
//	int offset = MSG_SIZE(MSG_FI_PARTY_GET_PARTY_INFO_OK);
//	pSock->m_pIMParty->m_ListPartyMember.lock();
//	mt_list<IMPartyMember>::iterator itr = pSock->m_pIMParty->m_ListPartyMember.begin();
//	while(itr != pSock->m_pIMParty->m_ListPartyMember.end())
//	{
//		PARTY_MEMBER_INFO *pPartyMember = (PARTY_MEMBER_INFO*)(pGetOKBuf + offset);
//		pPartyMember->CharacterUniqueNumber = itr->CharacterUniqueNumber;
//		strncpy(pPartyMember->CurrentMapName, ms_pIMIOCP->GetCharacterByArrayIndex(itr->ClientIndex)->MapName, SIZE_MAX_MAP_NAME);
//
//		offset += sizeof(PARTY_MEMBER_INFO);
//		itr++;
//	}
//	pSock->m_pIMParty->m_ListPartyMember.unlock();
//#ifdef _DEBUG
//	assert(offset
//		== (MSG_SIZE(MSG_FI_PARTY_GET_PARTY_INFO_OK) + sizeof(PARTY_MEMBER_INFO)*pSock->m_pIMParty->GetMemberCount(FALSE))
//	);
//#endif
//	SendAddData(pGetOKBuf, offset);
//
//	return RES_RETURN_TRUE;
//}
//*/

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_PARTY_AUTO_CREATE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 자동 편대 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_PARTY_AUTO_CREATE(const char* pPacket, int nLength, int &nBytesUsed) {

	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_PARTY_AUTO_CREATE Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int							nRecvTypeSize;
	MSG_FI_PARTY_AUTO_CREATE	* pRecvAutoCreateOK = NULL;
	nRecvTypeSize = sizeof(MSG_FI_PARTY_AUTO_CREATE);
	if(nLength - nBytesUsed < nRecvTypeSize) {
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	pRecvAutoCreateOK = (MSG_FI_PARTY_AUTO_CREATE*)(pPacket+nBytesUsed);	
	nBytesUsed += nRecvTypeSize;

	nRecvTypeSize = pRecvAutoCreateOK->PartyMemberListCount*sizeof(UID32_t);
	if(nLength-nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	UID32_t *pPartyMemberUID = (UID32_t*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket * pMasterIMSoc = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRecvAutoCreateOK->MasterUID);
	if (NULL == pMasterIMSoc) {
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE, ERR_COMMON_NO_SUCH_CHARACTER, pRecvAutoCreateOK->MasterUID);
		return RES_BREAK;
	}

	SPARTY_INFO PartyInfo;
	util::zero(&PartyInfo, sizeof(SPARTY_INFO));
	PartyInfo.PartyInfluence = pMasterIMSoc->m_character.InfluenceType;
	PartyInfo.MinLevel	= 1;
	PartyInfo.MaxLevel  = CHARACTER_MAX_LEVEL;

	// 파티 생성
	CIMParty *ptmParty = new CIMParty(ms_pIMIOCP, pMasterIMSoc->m_character.CharacterUniqueNumber, timeGetTime(), ms_pIMIOCP->m_UID32GenPartyID.GetNext(), &PartyInfo);	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	ptmParty->LockIMParty();
	IMPartyMember tmMasterMember(pMasterIMSoc->m_character.CharacterName, pMasterIMSoc->m_character.CharacterUniqueNumber, pMasterIMSoc, ptmParty);
	ptmParty->PushBackMember(tmMasterMember);
	
	pMasterIMSoc->SetIMParty(ptmParty);
	// 파티들의 list에 추가
	ms_pIMIOCP->InsertParty(ptmParty);

	// 2008-02-28 by dhjin,	아레나 통합 - LastPartyID에 값을 저장한다.
	pMasterIMSoc->m_character.LastPartyID	= ptmParty->m_PartyID;
	
	// Make msg
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_AUTO_CREATE_OK, T_FI_PARTY_AUTO_CREATE_OK, pMsgPartyCreateOK, pMsgPartyCreateOKBuf);
	pMsgPartyCreateOK->FieldPartyInfo.nTotalPartyMember = pRecvAutoCreateOK->PartyMemberListCount + 1;
	pMsgPartyCreateOK->FieldPartyInfo.lowestMemberLevel = 1;
	pMsgPartyCreateOK->MasterUID						= pRecvAutoCreateOK->MasterUID;
	pMsgPartyCreateOK->PartyID							= ptmParty->m_PartyID;

	// 맴버 추가
	UID32_t * pRecvPartyMemberUID = (UID32_t*)( ((BYTE*)pRecvAutoCreateOK) + sizeof(MSG_FI_PARTY_AUTO_CREATE));
	int PartyMemberListCount = 0;
	UID32_t * PartyMemberUID = (UID32_t*)(pMsgPartyCreateOKBuf+MSG_SIZE(MSG_FI_PARTY_AUTO_CREATE_OK));
	for(int i=0; i < pRecvAutoCreateOK->PartyMemberListCount; i++) {
		CIMIOCPSocket * pMemberIMSoc = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRecvPartyMemberUID[i]);
		if(NULL != pMemberIMSoc) {
			// party 할당
			pMemberIMSoc->SetIMParty(ptmParty);
			// party원 삽입
			IMPartyMember tmMember(pMemberIMSoc->m_character.CharacterName, pMemberIMSoc->m_character.CharacterUniqueNumber, pMemberIMSoc, ptmParty);
			ptmParty->PushBackMember(tmMember);
			// 2008-02-28 by dhjin,	아레나 통합 - LastPartyID에 값을 저장한다.
			pMemberIMSoc->m_character.LastPartyID	= ptmParty->m_PartyID;
			// 메세지 파티원 정보 
			PartyMemberUID[PartyMemberListCount] = pMemberIMSoc->m_character.CharacterUniqueNumber;
			PartyMemberListCount++;
		}
	}

	ptmParty->UnlockIMParty();

	pMsgPartyCreateOK->PartyMemberListCount	= PartyMemberListCount;
	pMasterIMSoc->SendMsgToCurrentFieldServerSocket(pMsgPartyCreateOKBuf, MSG_SIZE(MSG_FI_PARTY_AUTO_CREATE_OK) + PartyMemberListCount * sizeof(UID32_t));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-03-18 by cmkwon, 몬스터변신 구현 - 
/// \author		cmkwon
/// \date		2010-03-18 ~ 2010-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK,
		MSG_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK, pMsg);
	
	// 사용자 확인
	CIMIOCPSocket *pIISoc = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsg->CharacUID);
	if (pIISoc == NULL || FALSE == pIISoc->IsUsing())
	{
		SendErrorMessage(T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK, ERR_COMMON_NO_SUCH_CHARACTER, pMsg->CharacUID);
		return RES_BREAK;
	}
	
	// 파티 확인
	CIMParty *pParty = pIISoc->GetIMParty();
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_UPDATE_ITEM_TRANSFORMER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pMsg->CharacUID);
		return RES_BREAK;
	}
	pParty->LockIMParty();
	{
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK, T_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK, pSMsg, SendBuf);
		*pSMsg		= *pMsg;
		pParty->SendMsgToMembers(SendBuf, MSG_SIZE(MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK), pMsg->CharacUID);
	}
	pParty->UnlockIMParty();
	
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CHARACTER_DELETE_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CHARACTER_DELETE_CHARACTER Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CHARACTER_DELETE_CHARACTER,
									MSG_FI_CHARACTER_DELETE_CHARACTER, msgDelCharacter);

	///////////////////////////////////////////////////////////////////////////////
	// 삭제된 캐릭터 처리

	///////////////////////////////////////////////////////////////////////////////
	// 여단처리
	if(IS_VALID_UNIQUE_NUMBER(msgDelCharacter->GuildUniqueNumber))
	{
		mt_auto_lock aMG(&ms_pIMIOCP->m_MapGuild);
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(msgDelCharacter->GuildUniqueNumber);
		if (ptmGuild
			&& ptmGuild->m_GuildUniqueNumber == msgDelCharacter->GuildUniqueNumber)
		{
			ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildLeaveMember, this, NULL, NULL,
				msgDelCharacter->CharacterUniqueNumber, msgDelCharacter->GuildUniqueNumber);
		}		
	}	

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CHARACTER_CHANGE_LEVEL(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CHARACTER_CHANGE_LEVEL Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CHARACTER_CHANGE_LEVEL,
									MSG_FI_CHARACTER_CHANGE_LEVEL, msgChangeLevel);

	CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(msgChangeLevel->CharacterUniqueNumber);
	if (pSocket == NULL)
	{
		return RES_BREAK;
	}

	pSocket->m_character.Level = msgChangeLevel->Level;

	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	if(IS_VALID_UNIQUE_NUMBER(pSocket->m_character.GuildUniqueNumber))
	{
		mt_auto_lock aMG(&ms_pIMIOCP->m_MapGuild);
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(pSocket->m_character.GuildUniqueNumber);
		if (ptmGuild)
		{
			ptmGuild->SetMemberLevel(pSocket->m_character.CharacterUniqueNumber, msgChangeLevel->Level);
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_MEMBER_LEVEL_UP, T_IC_GUILD_MEMBER_LEVEL_UP, pSMsg, pSendBuf);
			pSMsg->CharacterUID		= pSocket->m_character.CharacterUniqueNumber;
			pSMsg->Level			= msgChangeLevel->Level;
			ptmGuild->SendMsgToGuildMembers(pSendBuf, MSG_SIZE(MSG_IC_GUILD_MEMBER_LEVEL_UP));
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CHARACTER_UPDATE_MAP_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CHARACTER_UPDATE_MAP_CHANNEL Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CHARACTER_UPDATE_MAP_CHANNEL,
									MSG_FI_CHARACTER_UPDATE_MAP_CHANNEL, msgUpdateMap);

	CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(msgUpdateMap->CharacterUniqueNumber);
	if (pSocket == NULL || FALSE == pSocket->IsValidCharacter())
	{
		return RES_BREAK;
	}

	CIMParty *ptmParty = pSocket->GetIMParty();

	// 파티원이 있다면 워프 정보 전송
	if (ptmParty)
	{
		// 2015-11-03 DevX will remove clients from party if moved to a free war map
#ifdef FREEWAR_
		if (isFreeForAllMap(msgUpdateMap->MapChannel.MapIndex))
		{
			auto clientsocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(msgUpdateMap->CharacterUniqueNumber);

			auto clientparty = clientsocket != nullptr ? clientsocket->GetIMParty() : nullptr;

			if (clientparty != nullptr)
			{
				clientparty->LockIMParty();
				BOOL r = clientsocket->ProcessPartyLeaveNoLock(clientparty);
				clientparty->UnlockIMParty();
				if (r == FALSE) clientsocket->ProcessPartyDismember(clientparty->m_PartyID);
				clientsocket->SetIMParty(nullptr);

				return RES_RETURN_TRUE;
			}
		}
#endif
		ptmParty->LockIMParty();
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME, T_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME, pUpdatePartyMap, pUpdatePartyMapBuf);
		pUpdatePartyMap->CharacterUniqueNumber = msgUpdateMap->CharacterUniqueNumber;
		pUpdatePartyMap->MapChannelIndex = msgUpdateMap->MapChannel;
		ptmParty->SendMsgToMembers(pUpdatePartyMapBuf, MSG_SIZE(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME), msgUpdateMap->CharacterUniqueNumber);
		ptmParty->UnlockIMParty();
	}

	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_CHARACTER_CHANGE_INFLUENCE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_CHARACTER_CHANGE_INFLUENCE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CHARACTER_CHANGE_INFLUENCE_TYPE Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CHARACTER_CHANGE_INFLUENCE_TYPE,
									MSG_FI_CHARACTER_CHANGE_INFLUENCE_TYPE, pRMsg);

	CIMIOCPSocket *pSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRMsg->CharacterUID);
	if (pSocket == NULL
		|| FALSE == pSocket->IsUsing())
	{
		return RES_BREAK;
	}
	
	pSocket->ChangedCharacterInfluenceType(pRMsg->InfluenceType0);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_UPDATE_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed)

/// \brief		
/// \author		dhjin
/// \date		2007-02-14 ~ 2007-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_UPDATE_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_UPDATE_SUBLEADER Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_UPDATE_SUBLEADER,
									MSG_FI_UPDATE_SUBLEADER, pRMsg);

	if(COMPARE_INFLUENCE(pRMsg->InfluenceType, INFLUENCE_TYPE_VCN))
	{
		if(1 == pRMsg->SubLeaderNum)
		{
			ms_pIMIOCP->m_VCNSub1LeaderCharacterUID = pRMsg->CharacterUID;
		}
		else if(2 == pRMsg->SubLeaderNum)
		{
			ms_pIMIOCP->m_VCNSub2LeaderCharacterUID = pRMsg->CharacterUID;
		}
		else 
		{
			return RES_BREAK;
		}
	}
	else if(COMPARE_INFLUENCE(pRMsg->InfluenceType, INFLUENCE_TYPE_ANI))
	{
		if(1 == pRMsg->SubLeaderNum)
		{
			ms_pIMIOCP->m_ANISub1LeaderCharacterUID = pRMsg->CharacterUID;
		}
		else if(2 == pRMsg->SubLeaderNum)
		{
			ms_pIMIOCP->m_ANISub2LeaderCharacterUID = pRMsg->CharacterUID;
		}
		else 
		{
			return RES_BREAK;
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_GUILD_NOTIFY_START_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_GUILD_NOTIFY_START_WAR Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_NOTIFY_START_WAR,
									MSG_FI_GUILD_NOTIFY_START_WAR, msgStartGuildWar);

	int nGuildWarTimerUniqueNumber = 0;
	ms_pIMIOCP->m_MapGuild.lock();
	{
		///////////////////////////////////////////////////////////////////////////////
		// 할당하고 1을 증가 시킨다
		nGuildWarTimerUniqueNumber = CGuild::ms_dwGuildWarUniqueNumber++;

		CGuild *pDefenderGuild = ms_pIMIOCP->m_MapGuild.findLock(msgStartGuildWar->DefenderGuildUID);
		if (pDefenderGuild == NULL) 
		{
			ms_pIMIOCP->m_MapGuild.unlock();			
			return RES_BREAK;
		}
		pDefenderGuild->StartWar(msgStartGuildWar->ChallengerGuildUID, msgStartGuildWar->WarMapChannel, nGuildWarTimerUniqueNumber);

		CGuild *pChallengerGuild = ms_pIMIOCP->m_MapGuild.findLock(msgStartGuildWar->ChallengerGuildUID);
		if (pChallengerGuild == NULL) 
		{
			ms_pIMIOCP->m_MapGuild.unlock();			
			return RES_BREAK;
		}
		pChallengerGuild->StartWar(msgStartGuildWar->DefenderGuildUID, msgStartGuildWar->WarMapChannel, nGuildWarTimerUniqueNumber);
		
		INIT_MSG_WITH_BUFFER(MSG_IC_COUNTDOWN_START, T_IC_COUNTDOWN_START, pSMsg, pSendBuf);
		pSMsg->byCountdownType	= COUNTDOWN_TYPE_GUILD_WAR;
		pSMsg->nWaitingSecond	= SECOND_COUNTDOWN_TIME_GAP;
		
		// to defender guild
		pDefenderGuild->SendMsgToGuildMembers(pSendBuf, MSG_SIZE(MSG_IC_COUNTDOWN_START), INVALID_UNIQUE_NUMBER, pSMsg);

		// to challenger guild
		pChallengerGuild->SendMsgToGuildMembers(pSendBuf, MSG_SIZE(MSG_IC_COUNTDOWN_START), INVALID_UNIQUE_NUMBER, pSMsg);
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	///////////////////////////////////////////////////////////////////////////////
	// 여단전 타이머 설정(20분)
	// 2006-09-01 by cmkwon, 진행시간 수정함(20분 --> 60분)
	ms_pIMIOCP->m_pIMTickManager->InsertTickEvent(IM_TICK_EVENT_START_GUILD_WAR, 60*60
		, msgStartGuildWar->DefenderGuildUID, nGuildWarTimerUniqueNumber);


// 2004-10-20 by cmkwon
//	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_WAR_READY, T_IC_GUILD_WAR_READY, pMsgReady, pMsgReadyBuf);
//
//	// to defender guild
//	pMsgReady->PeerGuildUID = msgStartGuildWar->ChallengerGuildUID;
//	pMsgReady->WaitingTime = 5;
//	pDefenderGuild->SendMsgToGuildMembers(pMsgReadyBuf, MSG_SIZE(MSG_IC_GUILD_WAR_READY));
//
//	// to challenger guild
//	pMsgReady->PeerGuildUID = msgStartGuildWar->DefenderGuildUID;
//	pMsgReady->WaitingTime = 5;
//	pChallengerGuild->SendMsgToGuildMembers(pMsgReadyBuf, MSG_SIZE(MSG_IC_GUILD_WAR_READY));
//
//	ms_pIMIOCP->m_pIMTickManager->InsertTickEvent(IM_TICK_EVENT_START_GUILD_WAR, 5, msgStartGuildWar->DefenderGuildUID, msgStartGuildWar->ChallengerGuildUID);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_GUILD_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_GUILD_DELETE_GUILD Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_DELETE_GUILD,
									MSG_FI_GUILD_DELETE_GUILD, msgDeleteGuild);

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteGuild, NULL, NULL, NULL, msgDeleteGuild->DeleteGuildUID);
	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_GUILD_ADD_GUILD_FAME(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-27 ~ 2005-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_GUILD_ADD_GUILD_FAME(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_GUILD_ADD_GUILD_FAME Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_ADD_GUILD_FAME,
									MSG_FI_GUILD_ADD_GUILD_FAME, pRMsg);

	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(pRMsg->guildUID);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		return RES_BREAK;
	}
	pGuild->AddGuildFame(pRMsg->addValues);

	// DB에 Update
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildAddGuildFame, this, NULL
		, (void*)pGuild->m_GuildUniqueNumber, pRMsg->addValues, pRMsg->addValues);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_GUILD_OUTPOST(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 전진기지 관련
/// \author		dhjin
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_GUILD_OUTPOST(const char* pPacket, int nLength, int &nBytesUsed)
{

	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_GUILD_OUTPOST Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_OUTPOST,
									MSG_FI_GUILD_OUTPOST, pRMsg);

	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(pRMsg->GuildUID);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		return RES_BREAK;
	}	

	pGuild->m_OutPostCityMapIndex = pRMsg->MapIndex;

	return RES_RETURN_TRUE;
}


// 2004-12-09 by cmkwon, 여단전의 종료는 IMServer에서만 발생한다. 그러므로 알림은 IMServer -> Field로 발생한다
//ProcessResult CIMIOCPSocket::Process_FI_GUILD_NOTIFY_END_WAR(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_NOTIFY_END_WAR,
//									MSG_FI_GUILD_NOTIFY_END_WAR, msgEndGuildWar);
//
//	char szChatBuf[SIZE_MAX_CHAT_MESSAGE];
//
//	ms_pIMIOCP->m_MapGuild.lock();
//	{
//		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_END_WAR, T_IC_GUILD_END_WAR, pMsgEndWar, pMsgEndWarBuf);
//
//		CGuild *pWinnerGuild = ms_pIMIOCP->m_MapGuild.findLock(msgEndGuildWar->WinnerGuildUID);
//		if (pWinnerGuild != NULL)
//		{
//			pWinnerGuild->m_GuildState = GUILD_STATE_NORMAL;
//		}
//
//		CGuild *pLoserGuild = ms_pIMIOCP->m_MapGuild.findLock(msgEndGuildWar->LoserGuildUID);
//		if (pLoserGuild != NULL)
//		{
//			pLoserGuild->m_GuildState = GUILD_STATE_NORMAL;
//		}
//
//		if (pLoserGuild == NULL && pWinnerGuild == NULL)
//		{
//			ms_pIMIOCP->m_MapGuild.unlock();
//			// do nothing
//			return RES_BREAK;
//		}
//		else if (pLoserGuild == NULL && pWinnerGuild != NULL)
//		{
//			sprintf(szChatBuf, "%s 여단이 여단전에서 승리하였습니다!", pWinnerGuild->m_GuildName);
//			ms_pIMIOCP->SendChatAll("운영자", szChatBuf);
//			
//			strncpy(pMsgEndWar->PeerGuildName, "나가버린 ", SIZE_MAX_GUILD_NAME);
//			pMsgEndWar->WarEndType = BATTLE_END_WIN;
//			pWinnerGuild->SendMsgToGuildMembers(pMsgEndWarBuf, MSG_SIZE(MSG_IC_GUILD_END_WAR));
//			
//			ms_pIMIOCP->m_MapGuild.unlock();
//			return RES_BREAK;
//		}
//		else if (pLoserGuild != NULL && pWinnerGuild == NULL)
//		{
//			sprintf(szChatBuf, "%s 여단이 여단전에서 승리하였습니다!", pLoserGuild->m_GuildName);
//			ms_pIMIOCP->SendChatAll("운영자", szChatBuf);
//
//			strncpy(pMsgEndWar->PeerGuildName, "나가버린 ", SIZE_MAX_GUILD_NAME);
//			pMsgEndWar->WarEndType = BATTLE_END_WIN;
//			pLoserGuild->SendMsgToGuildMembers(pMsgEndWarBuf, MSG_SIZE(MSG_IC_GUILD_END_WAR));
//
//			ms_pIMIOCP->m_MapGuild.unlock();
//			return RES_BREAK;
//		}
//
//		// to winner guild
//		strncpy(pMsgEndWar->PeerGuildName, pLoserGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
//		if (msgEndGuildWar->WarEndType == BATTLE_END_WIN || msgEndGuildWar->WarEndType == BATTLE_END_DEFEAT)
//		{
//			pMsgEndWar->WarEndType = BATTLE_END_WIN;
//		}
//		else
//		{
//			pMsgEndWar->WarEndType = msgEndGuildWar->WarEndType;
//		}
//		pWinnerGuild->SendMsgToGuildMembers(pMsgEndWarBuf, MSG_SIZE(MSG_IC_GUILD_END_WAR));
//
//		// to loser guild
//		strncpy(pMsgEndWar->PeerGuildName, pWinnerGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
//		if (msgEndGuildWar->WarEndType == BATTLE_END_WIN || msgEndGuildWar->WarEndType == BATTLE_END_DEFEAT)
//		{
//			pMsgEndWar->WarEndType = BATTLE_END_DEFEAT;
//		}
//		else
//		{
//			pMsgEndWar->WarEndType = msgEndGuildWar->WarEndType;
//		}
//		pLoserGuild->SendMsgToGuildMembers(pMsgEndWarBuf, MSG_SIZE(MSG_IC_GUILD_END_WAR));
//
//		// 여단전 결과 전체 공지
//		if (msgEndGuildWar->WarEndType == BATTLE_END_WIN || msgEndGuildWar->WarEndType == BATTLE_END_DEFEAT)
//		{
//			sprintf(szChatBuf, "%s 여단이 %s 여단과의 여단전에서 승리하였습니다!",
//						pWinnerGuild->m_GuildName,
//						pLoserGuild->m_GuildName);
//		}
//		else if (msgEndGuildWar->WarEndType == BATTLE_END_TIE)
//		{
//			sprintf(szChatBuf, "%s 여단과 %s 여단이 무승부로 여단전을 마쳤습니다!",
//						pWinnerGuild->m_GuildName,
//						pLoserGuild->m_GuildName);
//		}
//		else if (msgEndGuildWar->WarEndType == BATTLE_END_END)
//		{
//			sprintf(szChatBuf, "%s 여단과 %s 여단이 여단전을 마쳤습니다!",
//						pWinnerGuild->m_GuildName,
//						pLoserGuild->m_GuildName);
//		}
//	}// lock_end
//	ms_pIMIOCP->m_MapGuild.unlock();
//	
//	ms_pIMIOCP->SendChatAll("운영자", szChatBuf);
//
//	return RES_RETURN_TRUE;
//}


ProcessResult CIMIOCPSocket::Process_FI_CASH_USING_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CASH_USING_GUILD Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CASH_USING_GUILD,
									MSG_FI_CASH_USING_GUILD, pRUsingGuildCard);

	mt_auto_lock mtA(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(pRUsingGuildCard->guildUID);
	if (pGuild == NULL) 
	{			
		return RES_BREAK;
	}

	// 2008-08-12 by dhjin, 버그로 수정 
	pGuild->m_nGuildMemberCapacity += pRUsingGuildCard->nIncreaseMemberCapacity;
	pGuild->m_nGuildMemberCapacity = min(CAtumSJ::GetMaxGuildSize(pGuild->m_nGuildMemberCapacity, IsExistPremiumCard()), SIZE_MAX_GUILD_CAPACITY);	// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템

	///////////////////////////////////////////////////////////////////////////////
	// DB에 Update
	// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildUpdateMemberCapacity, this, NULL, (UID32_t*)pRUsingGuildCard->CashPrice, pGuild->m_GuildUniqueNumber, pGuild->m_nGuildMemberCapacity);

	///////////////////////////////////////////////////////////////////////////////
	// 길드원에게 모두 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY, T_IC_GUILD_CHANGE_MEMBER_CAPACITY, pSCapacity, SendBuf);
	pSCapacity->guildUID			= pGuild->m_GuildUniqueNumber;
	pSCapacity->nMemberCapacity		= pGuild->m_nGuildMemberCapacity;
	pSCapacity->IncreaseCapacity	= pRUsingGuildCard->nIncreaseMemberCapacity;		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY));
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_CASH_USING_CHANGE_CHARACTERNAME(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CASH_USING_CHANGE_CHARACTERNAME Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CASH_USING_CHANGE_CHARACTERNAME,
									MSG_FI_CASH_USING_CHANGE_CHARACTERNAME, pRChangeCharName);

	CIMIOCPSocket *pIISock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRChangeCharName->charUID);
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// Guild 에서 수정
	if (IS_VALID_UNIQUE_NUMBER(pIISock->m_uidGuildUniqueNumber))
	{
		mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(pIISock->m_uidGuildUniqueNumber);
		if(ptmGuild)
		{
			CGuildMember *ptmGMember;
			if(ptmGuild->GetGuildMember(pIISock->m_character.CharacterUniqueNumber, ptmGMember))
			{
				util::strncpy(ptmGMember->m_CharacterName, pRChangeCharName->szChangedCharacterName, SIZE_MAX_CHARACTER_NAME);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// IMIOCP가 관리하는 데이타 처리
	mt_auto_lock mtA(&ms_pIMIOCP->m_mCharacterName);
	char tmCharacterName[SIZE_MAX_CHARACTER_NAME];
	ms_pIMIOCP->m_mCharacterName.deleteNoLock(GGetLowerCase(tmCharacterName, pIISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME));

	util::strncpy(pIISock->m_character.CharacterName, pRChangeCharName->szChangedCharacterName, SIZE_MAX_CHARACTER_NAME);
	ms_pIMIOCP->m_mCharacterName.insertNoLock(GGetLowerCase(tmCharacterName, pIISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME), pIISock);
	mtA.auto_unlock_cancel();

	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_CASH_PREMIUM_CARD_INFO(const char* pPacket, int nLength, int &nBytesUsed)	
/// \brief		
/// \author		dhjin
/// \date		2006-09-14 ~ 2006-09-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_CASH_PREMIUM_CARD_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_CASH_PREMIUM_CARD_INFO Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
   	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CASH_PREMIUM_CARD_INFO,
								MSG_FI_CASH_PREMIUM_CARD_INFO, pRCashPremiumInfo);

	CIMIOCPSocket *pIISock = ms_pIMIOCP->GetIMIOCPSocketByAccountUID(pRCashPremiumInfo->AccountUID);
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return RES_BREAK;
	}
	
	pIISock->m_MembershipInfo.nCardItemNum1			= pRCashPremiumInfo->nCardItemNum1;

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-20 by dhjin, EP3 여단 수정 사항 - 만료 시간 필요
	pIISock->m_MembershipInfo.atumTimeExpireTime1	= pRCashPremiumInfo->atumTimeExpireTime1;	
	
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(pIISock->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		return RES_BREAK;
	}	

	if(pIISock->m_character.CharacterUniqueNumber == pGuild->m_CommanderUniqueNumber)
	{
		if(FALSE == pRCashPremiumInfo->nCardItemNum1)
		{
			pGuild->SetGuildMemberShip(FALSE);
		}
		else
		{
			pGuild->SetGuildMemberShip(TRUE, &pRCashPremiumInfo->atumTimeExpireTime1);
		}
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_MEMBERSHIP, T_IC_GUILD_CHANGE_MEMBERSHIP, msg, SendBuf);
		msg->MemberShip					= pGuild->m_bMemberShip;
		msg->MemberShipExpireTime		= pGuild->m_MemberShipExpireTime;
		pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_MEMBERSHIP));				
	}

	return RES_RETURN_TRUE;
}


/***********************************************************************************************************
**
**	파티 탈퇴 처리.
**
**	Create Info :	??.??.??.
**
**	Update Info :	아레아 이동에 대한 예외처리 인자 추가.	'a_bMoveToArena'	2010.09.03. by hsLee.
**
************************************************************************************************************/
BOOL CIMIOCPSocket::ProcessPartyLeaveNoLock( CIMParty *i_pIMParty , const bool a_bMoveToArena /*= false*/ )
{	
	UID32_t tmOldUID = i_pIMParty->GetMasterCharacterUniqueNumber();
	i_pIMParty->LeaveMember(m_character.CharacterUniqueNumber);	
	if (0 == i_pIMParty->GetMemberCount())
	{// 유저가 없거나 Online유저가 없음
		
// 		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 		g_pIMGlobal->WriteSystemLogEX(TRUE, "  [TEMP] 090804  CIMIOCPSocket::ProcessPartyLeaveNoLock# %s CurIsOn(%d) FormationTy(%d) \r\n"
// 			, GetCharacterString(GetCharacter(), string()), i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill, i_pIMParty->m_SPartyInfo.FormationType);
		///////////////////////////////////////////////////////////////////////////////
		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 편대 탈퇴시, 
		if(i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill)
		{
			i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill		= FALSE;
			this->SendFieldServPartyChangeFormationSkill(i_pIMParty->GetMasterCharacterUniqueNumber(), FALSE);
		}

		// 파티 해산정보를 전송
		if(this->IsUsing())
		{
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_DISMEMBER_OK, T_IC_PARTY_DISMEMBER_OK, pICDismemberOK, pICDismemberOKBuf);
			pICDismemberOK->PartyID = i_pIMParty->m_PartyID;
			SendAddData(pICDismemberOKBuf, MSG_SIZE(MSG_IC_PARTY_DISMEMBER_OK));
		}

		// Field Server에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DISMEMBER_OK, T_FI_PARTY_DISMEMBER_OK, pFIDismemberOK, pFIDismemberOKBuf);
		pFIDismemberOK->PartyID = i_pIMParty->m_PartyID;
		ms_pIMIOCP->SendMessageToFieldServer(pFIDismemberOKBuf, MSG_SIZE(MSG_FI_PARTY_DISMEMBER_OK));

		return FALSE;
	}
	
	if (tmOldUID != i_pIMParty->GetMasterCharacterUniqueNumber())
	{
		SendMsgPartyTransferMasterInfoNoLock(i_pIMParty, tmOldUID, i_pIMParty->GetMasterCharacterUniqueNumber());
	}
	
// 	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 	g_pIMGlobal->WriteSystemLogEX(TRUE, "  [TEMP] 090804  CIMIOCPSocket::ProcessPartyLeaveNoLock# %s CurIsOn(%d) FormationTy(%d) \r\n"
// 		, GetCharacterString(GetCharacter(), string()), i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill, i_pIMParty->m_SPartyInfo.FormationType);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 편대 탈퇴시, 
	if(i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill)
	{
		i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill		= FALSE;
		this->SendFieldServPartyChangeFormationSkill(i_pIMParty->GetMasterCharacterUniqueNumber(), FALSE);
	}

	// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 강제 종료 유저 파티에서 탈퇴처리!, 밑과 같이 수정
	// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 자기 자신 전송만 IsUsing 체크 하도록 변경, 다른 유저들에겐 전송해서 파티 나간 사실을 알려주자
//	if(this->IsUsing())
//	{
//		// Client에게 보내기
//		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_LEAVE_OK, T_IC_PARTY_LEAVE_OK, pICLeaveOK, pICLeaveOKBuf);
//		pICLeaveOK->PartyID					= i_pIMParty->m_PartyID;
//		pICLeaveOK->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
//
//		if(this->IsUsing()) { 
//			// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - 자기 자신 전송만 IsUsing 체크 하도록 변경, 다른 유저들에겐 전송해서 파티 나간 사실을 알려주자
//			///////////////////////////////////////////////////////////////////////////////
//			// 자기 자신에게 전송한다
//			this->SendAddData(pICLeaveOKBuf, MSG_SIZE(MSG_IC_PARTY_LEAVE_OK));
//		}
//
//		///////////////////////////////////////////////////////////////////////////////
//		// 남은 파티원들에게 전송
//		i_pIMParty->SendMsgToMembers(pICLeaveOKBuf, MSG_SIZE(MSG_IC_PARTY_LEAVE_OK));
//	}
	// Client에게 보내기
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_LEAVE_OK, T_IC_PARTY_LEAVE_OK, pICLeaveOK, pICLeaveOKBuf);
	pICLeaveOK->PartyID					= i_pIMParty->m_PartyID;
	pICLeaveOK->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
	pICLeaveOK->bMoveToArena			= a_bMoveToArena;
	///////////////////////////////////////////////////////////////////////////////
	// 남은 파티원들에게 전송
	i_pIMParty->SendMsgToMembers(pICLeaveOKBuf, MSG_SIZE(MSG_IC_PARTY_LEAVE_OK));

	if(this->IsUsing())	{
		// 자기 자신에게 전송한다
		this->SendAddData(pICLeaveOKBuf, MSG_SIZE(MSG_IC_PARTY_LEAVE_OK));
		// Field Server에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_LEAVE_OK, T_FI_PARTY_LEAVE_OK, pFILeaveOK, pFILeaveOKBuf);
		pFILeaveOK->PartyID								= i_pIMParty->m_PartyID;
		pFILeaveOK->CharacterUniqueNumber				= m_character.CharacterUniqueNumber;
		pFILeaveOK->FieldPartyInfo.nTotalPartyMember	= i_pIMParty->GetMemberCount(FALSE);
		pFILeaveOK->FieldPartyInfo.lowestMemberLevel	= i_pIMParty->GetLowestMemberLevel();
		this->SendMsgToCurrentFieldServerSocket(pFILeaveOKBuf, MSG_SIZE(MSG_FI_PARTY_LEAVE_OK));
	}
	else {
		// Field Server에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DISCONNECT_LEAVE_OK, T_FI_PARTY_DISCONNECT_LEAVE_OK, pFILeaveOK, pFILeaveOKBuf);
		pFILeaveOK->PartyID								= i_pIMParty->m_PartyID;
		pFILeaveOK->CharacterUniqueNumber				= m_character.CharacterUniqueNumber;
		pFILeaveOK->FieldPartyInfo.nTotalPartyMember	= i_pIMParty->GetMemberCount(FALSE);
		pFILeaveOK->FieldPartyInfo.lowestMemberLevel	= i_pIMParty->GetLowestMemberLevel();
		this->SendMsgToCurrentFieldServerSocket(pFILeaveOKBuf, MSG_SIZE(MSG_FI_PARTY_DISCONNECT_LEAVE_OK));
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::ProcessPartyLeaveArenaEndNoLock(CIMParty *i_pIMParty)
/// \brief		아레나 서버에서 경기 종료후 파티 관련 처리
/// \author		dhjin
/// \date		2008-02-28 ~ 2008-02-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::ProcessPartyLeaveArenaEndNoLock(CIMParty *i_pIMParty)
{
	UID32_t tmOldUID = i_pIMParty->GetMasterCharacterUniqueNumber();
	i_pIMParty->LeaveMember(m_character.CharacterUniqueNumber);	

	if (0 == i_pIMParty->GetMemberCount())
	{// 유저가 없거나 Online유저가 없음

		// Field Server에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DISMEMBER_OK, T_FI_PARTY_DISMEMBER_OK, pFIDismemberOK, pFIDismemberOKBuf);
		pFIDismemberOK->PartyID = i_pIMParty->m_PartyID;
		ms_pIMIOCP->SendMessageToFieldServer(pFIDismemberOKBuf, MSG_SIZE(MSG_FI_PARTY_DISMEMBER_OK));

		return FALSE;
	}
	
	if (tmOldUID != i_pIMParty->GetMasterCharacterUniqueNumber())
	{
		SendMsgPartyTransferMasterInfoNoLock(i_pIMParty, tmOldUID, i_pIMParty->GetMasterCharacterUniqueNumber());
	}

	if(this->IsUsing())
	{
		// Client에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_LEAVE_OK, T_IC_PARTY_LEAVE_OK, pICLeaveOK, pICLeaveOKBuf);
		pICLeaveOK->PartyID					= i_pIMParty->m_PartyID;
		pICLeaveOK->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;

		///////////////////////////////////////////////////////////////////////////////
		// 남은 파티원들에게 전송
		i_pIMParty->SendMsgToMembers(pICLeaveOKBuf, MSG_SIZE(MSG_IC_PARTY_LEAVE_OK));
	}

	// Field Server에게 보내기
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_LEAVE_OK, T_FI_PARTY_LEAVE_OK, pFILeaveOK, pFILeaveOKBuf);
	pFILeaveOK->PartyID								= i_pIMParty->m_PartyID;
	pFILeaveOK->CharacterUniqueNumber				= m_character.CharacterUniqueNumber;
	pFILeaveOK->FieldPartyInfo.nTotalPartyMember	= i_pIMParty->GetMemberCount(FALSE);
	pFILeaveOK->FieldPartyInfo.lowestMemberLevel	= i_pIMParty->GetLowestMemberLevel();
	this->SendMsgToCurrentFieldServerSocket(pFILeaveOKBuf, MSG_SIZE(MSG_FI_PARTY_LEAVE_OK));

	return TRUE;
}

BOOL CIMIOCPSocket::ProcessPartyGameEndNoLock(CIMParty *i_pIMParty)
{
	UID32_t tmOldUID = i_pIMParty->GetMasterCharacterUniqueNumber();
	i_pIMParty->SetOfflineMemberW(m_character.CharacterUniqueNumber);	
	if (0 == i_pIMParty->GetMemberCount())
	{// 유저가 없거나 Online유저가 없음
		
		// 파티 해산정보를 전송
		if(this->IsUsing())
		{
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_DISMEMBER_OK, T_IC_PARTY_DISMEMBER_OK, pICDismemberOK, pICDismemberOKBuf);
			pICDismemberOK->PartyID = i_pIMParty->m_PartyID;
			SendAddData(pICDismemberOKBuf, MSG_SIZE(MSG_IC_PARTY_DISMEMBER_OK));
		}

		// Field Server에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DISMEMBER_OK, T_FI_PARTY_DISMEMBER_OK, pFIDismemberOK, pFIDismemberOKBuf);
		pFIDismemberOK->PartyID = i_pIMParty->m_PartyID;
		ms_pIMIOCP->SendMessageToFieldServer(pFIDismemberOKBuf, MSG_SIZE(MSG_FI_PARTY_DISMEMBER_OK));

		return FALSE;
	}
	
	if (tmOldUID != i_pIMParty->GetMasterCharacterUniqueNumber())
	{
		SendMsgPartyTransferMasterInfoNoLock(i_pIMParty, tmOldUID, i_pIMParty->GetMasterCharacterUniqueNumber());
	}

// 	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 	g_pIMGlobal->WriteSystemLogEX(TRUE, "  [TEMP] 090804  CIMIOCPSocket::ProcessPartyGameEndNoLock# %s CurIsOn(%d) FormationTy(%d) \r\n"
// 		, GetCharacterString(GetCharacter(), string()), i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill, i_pIMParty->m_SPartyInfo.FormationType);

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 편대 탈퇴시, 
	if(i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill)
	{
		i_pIMParty->m_SPartyInfo.IsAppliedFormationSkill		= FALSE;
		this->SendFieldServPartyChangeFormationSkill(i_pIMParty->GetMasterCharacterUniqueNumber(), FALSE);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::ProcessPartyFromMainServerToArenaServerNoLock(CIMParty *i_pIMParty)
/// \brief		아레나 통합 - 본 서버에서 아레나 서버로 이동 시 파티 처리 사항
/// \author		dhjin
/// \date		2008-02-28 ~ 2008-02-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::ProcessPartyFromMainServerToArenaServerNoLock(CIMParty *i_pIMParty)
{

	// 본섭 -> 아레나섭 이동 시 본섭 파티에서는 탈퇴처리로 수정.	2010.09.03. by hsLee.
	return ProcessPartyLeaveNoLock ( i_pIMParty , true );

	/*
	UID32_t tmOldUID = i_pIMParty->GetMasterCharacterUniqueNumber();
	i_pIMParty->SetOfflineMemberW( m_character.CharacterUniqueNumber );
	if (0 == i_pIMParty->GetMemberCount())
	{// 유저가 없거나 Online유저가 없음
		
		// Field Server에게 보내기
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DISMEMBER_OK, T_FI_PARTY_DISMEMBER_OK, pFIDismemberOK, pFIDismemberOKBuf);
		pFIDismemberOK->PartyID = i_pIMParty->m_PartyID;
		ms_pIMIOCP->SendMessageToFieldServer(pFIDismemberOKBuf, MSG_SIZE(MSG_FI_PARTY_DISMEMBER_OK));

		return FALSE;
	}
	
	if (tmOldUID != i_pIMParty->GetMasterCharacterUniqueNumber())
	{
		SendMsgPartyTransferMasterInfoNoLock(i_pIMParty, tmOldUID, i_pIMParty->GetMasterCharacterUniqueNumber());
	}

	return TRUE;
	*/
}

BOOL CIMIOCPSocket::SendMsgPartyTransferMasterInfoNoLock(CIMParty *i_pIMParty, UINT OldMaster, UINT NewMaster)
{
	// send to Field Servers
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_TRANSFER_MASTER_OK, T_FI_PARTY_TRANSFER_MASTER_OK, pFITransMasterOK, pFITransMasterOKBuf);
	pFITransMasterOK->PartyID							= i_pIMParty->m_PartyID;
	pFITransMasterOK->OldMasterCharacterUniqueNumber	= OldMaster;
	pFITransMasterOK->NewMasterCharacterUniqueNumber	= NewMaster;
	ms_pIMIOCP->SendMessageToFieldServer(pFITransMasterOKBuf, MSG_SIZE(MSG_FI_PARTY_TRANSFER_MASTER_OK));

	// send to Party Members
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_TRANSFER_MASTER_OK, T_IC_PARTY_TRANSFER_MASTER_OK, pICTransMasterOK, pICTransMasterOKBuf);
	pICTransMasterOK->PartyID							= i_pIMParty->m_PartyID;
	pICTransMasterOK->OldMasterCharacterUniqueNumber	= OldMaster;
	pICTransMasterOK->NewMasterCharacterUniqueNumber	= NewMaster;
	i_pIMParty->SendMsgToMembers(pICTransMasterOKBuf, MSG_SIZE(MSG_IC_PARTY_TRANSFER_MASTER_OK));
	return TRUE;
}

void CIMIOCPSocket::ProcessPartyDismember(PartyID_t i_partyID)
{
	ms_pIMIOCP->m_MapParties.lock();
	{
		CIMParty *pTmpParty = ms_pIMIOCP->m_MapParties.findLock(i_partyID);
		if(NULL == pTmpParty)
		{
			ms_pIMIOCP->m_MapParties.unlock();
			return;
		}
		pTmpParty->LockIMParty();
		{
			// send to Clients
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_DISMEMBER_OK, T_IC_PARTY_DISMEMBER_OK, pICDisMemberOK, pICDisMemberOKBuf);
			pICDisMemberOK->PartyID = i_partyID;
			pTmpParty->SendMsgToMembers(pICDisMemberOKBuf, MSG_SIZE(MSG_IC_PARTY_DISMEMBER_OK), INVALID_UNIQUE_NUMBER, TRUE);

			// send to Field Server
			INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_DISMEMBER_OK, T_FI_PARTY_DISMEMBER_OK, pFIDismemberOK, pFIDismemberOKBuf);
			pFIDismemberOK->PartyID = i_partyID;
			ms_pIMIOCP->SendMessageToFieldServer(pFIDismemberOKBuf, MSG_SIZE(MSG_FI_PARTY_DISMEMBER_OK));
		}
		pTmpParty->UnlockIMParty();
	}	
	// 파티 제거
	ms_pIMIOCP->DeleteParty(i_partyID);
	ms_pIMIOCP->m_MapParties.unlock();	
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_LEAVE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_IC_PARTY_LEAVE		*pMsgLeave = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_LEAVE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_LEAVE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12022);
		return RES_RETURN_FALSE;
	}
	pMsgLeave = (MSG_IC_PARTY_LEAVE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		// error: 가입한 파티가 없음
		SendErrorMessage(T_IC_PARTY_LEAVE, ERR_PROTOCOL_NOT_HAVE_PARTY);
		return RES_BREAK;
	}
	ptmParty->LockIMParty();
	if (ptmParty->m_PartyID != pMsgLeave->PartyID)
	{
		SendErrorMessage(T_IC_PARTY_LEAVE, ERR_PROTOCOL_PARTY_ID_NOT_MATCHED, pMsgLeave->PartyID, ptmParty->m_PartyID);
		ptmParty->UnlockIMParty();
		return RES_BREAK;
	}

	BOOL bReted = ProcessPartyLeaveNoLock(ptmParty);
	ptmParty->UnlockIMParty();

	if(FALSE == bReted)
	{// 파티맵에서 제거해야한다

		ProcessPartyDismember(pMsgLeave->PartyID);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 캐릭터의 파티 정보 삭제
	this->SetIMParty(NULL);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_TRANSFER_MASTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int								nRecvTypeSize = 0;
	MSG_IC_PARTY_TRANSFER_MASTER	*pMsgTranMaster= NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_TRANSFER_MASTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1201D);
		return RES_RETURN_FALSE;
	}
	pMsgTranMaster = (MSG_IC_PARTY_TRANSFER_MASTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (m_character.CharacterUniqueNumber != pMsgTranMaster->OldMasterCharacterUniqueNumber)
	{// 자신의 CharacterUniqueNumber와 다르다

		SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED);
		return RES_BREAK;
	}

	if(pMsgTranMaster->NewMasterCharacterUniqueNumber == m_character.CharacterUniqueNumber)
	{// 
		
		SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_COMMON_NOT_ALLOWED_TO_MYSELF);
		return RES_BREAK;
	}
	
	CIMIOCPSocket *pIISoc = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgTranMaster->NewMasterCharacterUniqueNumber);
	if(NULL == pIISoc
		|| FALSE == pIISoc->IsValidCharacter())
	{// 2007-10-24 by cmkwon, 새로운 파티장 유효 한지 체크 -
		SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, 0, 30);
		return RES_BREAK;
	}
	if(m_character.MapChannelIndex != pIISoc->m_character.MapChannelIndex)
	{// 2007-10-24 by cmkwon, 새로운 파티장 유효 한지 체크 - 같은 맵에 있는지 체크
		SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_MAP_CHANNEL_NOT_MATCHED, 0, 40);
		return RES_BREAK;
	}


	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL || ptmParty->m_PartyID != pMsgTranMaster->PartyID)
	{// error

		SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}
	ptmParty->LockIMParty();
	{
		if (ptmParty->GetMasterCharacterUniqueNumber() != pMsgTranMaster->OldMasterCharacterUniqueNumber)
		{// error: 자신이 파티장이 아니므로 권한이 없음

			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_PARTY_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if(FALSE == ptmParty->TransferMaster(pMsgTranMaster->NewMasterCharacterUniqueNumber))
		{// error

			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_TRANSFER_MASTER, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER);
			return RES_BREAK;
		}
		
		SendMsgPartyTransferMasterInfoNoLock(ptmParty, pMsgTranMaster->OldMasterCharacterUniqueNumber, pMsgTranMaster->NewMasterCharacterUniqueNumber);
	}
	ptmParty->UnlockIMParty();

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_BAN_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_IC_PARTY_BAN_MEMBER	*pMsgBan = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_BAN_MEMBER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12020);
		return RES_RETURN_FALSE;
	}
	pMsgBan = (MSG_IC_PARTY_BAN_MEMBER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (pMsgBan->CharacterUniqueNumber == m_character.CharacterUniqueNumber)
	{
		SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_COMMON_NOT_ALLOWED_TO_MYSELF);
		return RES_BREAK;
	}

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		// error
		SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_PROTOCOL_NOT_HAVE_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();
	{
		if (ptmParty->m_PartyID != pMsgBan->PartyID)
		{
			// error
			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_PROTOCOL_PARTY_ID_NOT_MATCHED);
			return RES_BREAK;
		}

		if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
		{
			// error: 자신이 파티장이 아니므로 권한이 없음
			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_PROTOCOL_PARTY_PERMISSION_DENIED);
			return RES_BREAK;
		}

		// 찾아보기
		IMPartyMember *pMember = ptmParty->GetMemberPtrByCharacterUniqueNumber(pMsgBan->CharacterUniqueNumber);
		if (NULL == pMember)
		{
			// error
			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER);
			return RES_BREAK;
		}

		// 게임을 하지 않는 파티원인 경우 - 그냥 파티서 제거함
		if (FALSE == pMember->IsOnlineMember() || FALSE == pMember->PM_pIMIOCPSocket->IsUsing())
		{
			// 추방할 아그 지우기
			if (FALSE == ptmParty->LeaveMember(pMsgBan->CharacterUniqueNumber))
			{
				ptmParty->UnlockIMParty();
				SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER, pMsgBan->CharacterUniqueNumber, 1);
				return RES_BREAK;
			}

// 			// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 			g_pIMGlobal->WriteSystemLogEX(TRUE, "  [TEMP] 090804  CIMIOCPSocket::Process_IC_PARTY_BAN_MEMBER# 1  %s CurIsOn(%d) FormationTy(%d) \r\n"
// 				, GetCharacterString(GetCharacter(), string()), ptmParty->m_SPartyInfo.IsAppliedFormationSkill, ptmParty->m_SPartyInfo.FormationType);

			///////////////////////////////////////////////////////////////////////////////
			// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 편대원 추방시, 오프라인 편대원 처리
			if(ptmParty->m_SPartyInfo.IsAppliedFormationSkill)
			{
				ptmParty->m_SPartyInfo.IsAppliedFormationSkill	= FALSE;
				this->SendFieldServPartyChangeFormationSkill(m_character.CharacterUniqueNumber, FALSE);
			}

			// send to SERVER
			INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_BAN_MEMBER_OK, T_FI_PARTY_BAN_MEMBER_OK, pFIBanMemberOK, pFIBanMemberOKBuf);
			pFIBanMemberOK->PartyID					= pMsgBan->PartyID;
			pFIBanMemberOK->CharacterUniqueNumber	= pMsgBan->CharacterUniqueNumber;
			pFIBanMemberOK->FieldPartyInfo.nTotalPartyMember = ptmParty->GetMemberCount(FALSE);	// 주의: m_ListPartyMember에서 삭제 이후여야 함!
			pFIBanMemberOK->FieldPartyInfo.lowestMemberLevel = ptmParty->GetLowestMemberLevel();	// 주의: m_ListPartyMember에서 삭제 이후여야 함!
			ms_pIMIOCP->SendMessageToFieldServer(pFIBanMemberOKBuf, MSG_SIZE(MSG_FI_PARTY_BAN_MEMBER_OK));

			// send to CLIENTS
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_BAN_MEMBER_OK, T_IC_PARTY_BAN_MEMBER_OK, pICBanMemberOK, pICBanMemberOKBuf);
			pICBanMemberOK->PartyID					= pMsgBan->PartyID;
			pICBanMemberOK->CharacterUniqueNumber	= pMsgBan->CharacterUniqueNumber;
			ptmParty->SendMsgToMembers(pICBanMemberOKBuf, MSG_SIZE(MSG_IC_PARTY_BAN_MEMBER_OK));
		}
		else
		{
			// 추방 당할 아그 찾기
			CIMIOCPSocket *pSocket = pMember->PM_pIMIOCPSocket;	// 추방 당할 파티원
			if (pSocket == NULL || !pSocket->IsUsing() || pSocket->m_character.CharacterUniqueNumber != pMember->PM_CharacterUniqueNumber)
			{
				ptmParty->UnlockIMParty();
				SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER, pMsgBan->CharacterUniqueNumber, 2);
				return RES_BREAK;
			}

			// 추방할 아그 지우기
			if (FALSE == ptmParty->LeaveMember(pMsgBan->CharacterUniqueNumber))
			{
				ptmParty->UnlockIMParty();
				SendErrorMessage(T_IC_PARTY_BAN_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER, pMsgBan->CharacterUniqueNumber, 1);
				return RES_BREAK;
			}

// 			// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 			g_pIMGlobal->WriteSystemLogEX(TRUE, "  [TEMP] 090804  CIMIOCPSocket::Process_IC_PARTY_BAN_MEMBER# 2  %s CurIsOn(%d) FormationTy(%d) \r\n"
// 				, GetCharacterString(GetCharacter(), string()), ptmParty->m_SPartyInfo.IsAppliedFormationSkill, ptmParty->m_SPartyInfo.FormationType);

			///////////////////////////////////////////////////////////////////////////////
			// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 편대원 추방시, 온라인 편대원 처리
			if(ptmParty->m_SPartyInfo.IsAppliedFormationSkill)
			{
				ptmParty->m_SPartyInfo.IsAppliedFormationSkill	= FALSE;
				this->SendFieldServPartyChangeFormationSkill(m_character.CharacterUniqueNumber, FALSE);
			}

			// send to SERVER
			INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_BAN_MEMBER_OK, T_FI_PARTY_BAN_MEMBER_OK, pFIBanMemberOK, pFIBanMemberOKBuf);
			pFIBanMemberOK->PartyID					= pMsgBan->PartyID;
			pFIBanMemberOK->CharacterUniqueNumber	= pMsgBan->CharacterUniqueNumber;
			pFIBanMemberOK->FieldPartyInfo.nTotalPartyMember = ptmParty->GetMemberCount(FALSE);		// 주의: m_ListPartyMember에서 삭제 이후여야 함!
			pFIBanMemberOK->FieldPartyInfo.lowestMemberLevel = ptmParty->GetLowestMemberLevel();		// 주의: m_ListPartyMember에서 삭제 이후여야 함!
			pSocket->SendMsgToCurrentFieldServerSocket(pFIBanMemberOKBuf, MSG_SIZE(MSG_FI_PARTY_BAN_MEMBER_OK));

			// send to CLIENTS
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_BAN_MEMBER_OK, T_IC_PARTY_BAN_MEMBER_OK, pICBanMemberOK, pICBanMemberOKBuf);
			pICBanMemberOK->PartyID					= pMsgBan->PartyID;
			pICBanMemberOK->CharacterUniqueNumber	= pMsgBan->CharacterUniqueNumber;
			ptmParty->SendMsgToMembers(pICBanMemberOKBuf, MSG_SIZE(MSG_IC_PARTY_BAN_MEMBER_OK));

			// 위에서 추방 당한 자 지웠으므로 따로 보냄
			pSocket->SendAddData(pICBanMemberOKBuf, MSG_SIZE(MSG_IC_PARTY_BAN_MEMBER_OK));

			// 추방 당한 아그에서 파티 정보 제거
			pSocket->SetIMParty(NULL);
		}
	}// LockIMParty_end
	ptmParty->UnlockIMParty();

	return RES_RETURN_TRUE;
}

// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 사용하지 않는 프로토콜임
// ProcessResult CIMIOCPSocket::Process_IC_PARTY_DISMEMBER(const char* pPacket, int nLength, int &nBytesUsed)
// {
// 	int						nRecvTypeSize = 0;
// 	MSG_IC_PARTY_DISMEMBER	*pMsgDismember = NULL;
// 
// 	nRecvTypeSize = sizeof(MSG_IC_PARTY_DISMEMBER);
// 	if(nLength - nBytesUsed < nRecvTypeSize)
// 	{
// 		SendErrorMessage(T_IC_PARTY_DISMEMBER, ERR_PROTOCOL_INVALID_FIELD_DATA);
// 		Close(0x12021);
// 		return RES_RETURN_FALSE;
// 	}
// 	pMsgDismember = (MSG_IC_PARTY_DISMEMBER*)(pPacket + nBytesUsed);
// 	nBytesUsed += nRecvTypeSize;
// 
// 	CIMParty *ptmParty = this->GetIMParty();
// 	if (ptmParty == NULL)
// 	{
// 		// error
// 		SendErrorMessage(T_IC_PARTY_DISMEMBER, ERR_PROTOCOL_NOT_HAVE_PARTY);
// 		return RES_BREAK;
// 	}
// 
// 	ptmParty->LockIMParty();
// 	{
// 		if (ptmParty->m_PartyID != pMsgDismember->PartyID)
// 		{// error
// 
// 			ptmParty->UnlockIMParty();
// 			SendErrorMessage(T_IC_PARTY_DISMEMBER, ERR_PROTOCOL_PARTY_ID_NOT_MATCHED);
// 			return RES_BREAK;
// 		}
// 
// 		if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
// 		{// error: 자신이 파티장이 아니므로 권한이 없음
// 			
// 			ptmParty->UnlockIMParty();
// 			SendErrorMessage(T_IC_PARTY_DISMEMBER, ERR_PROTOCOL_PARTY_PERMISSION_DENIED);
// 			return RES_BREAK;
// 		}
// 	}
// 	PartyID_t partyID = ptmParty->m_PartyID;
// 	ptmParty->UnlockIMParty();
// 
// 	// 파티 해산
// 	ProcessPartyDismember(partyID);
// 
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 캐릭터의 파티 정보 삭제
// 	this->SetIMParty(NULL);
// 	return RES_RETURN_TRUE;
// }

ProcessResult CIMIOCPSocket::Process_IC_PARTY_CHANGE_FLIGHT_FORMATION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int										nRecvTypeSize = 0;
	MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION	*pMsgChangeForm = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_FORMATION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12021);
		return RES_RETURN_FALSE;
	}
	pMsgChangeForm = (MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMParty *ptmParty = this->GetIMParty();

	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_FORMATION, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();
	{
		if (ptmParty->m_PartyID != pMsgChangeForm->PartyID)
		{
			SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_FORMATION, ERR_PROTOCOL_PARTY_ID_NOT_MATCHED, LODWORD(ptmParty->m_PartyID), HIDWORD(pMsgChangeForm->PartyID));
			ptmParty->UnlockIMParty();
			return RES_BREAK;
		}

		if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
		{			
			SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_FORMATION, ERR_PROTOCOL_PARTY_PERMISSION_DENIED,
				ptmParty->GetMasterCharacterUniqueNumber(), m_character.CharacterUniqueNumber);
			ptmParty->UnlockIMParty();
			return RES_BREAK;
		}

// 		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 		g_pIMGlobal->WriteSystemLogEX(TRUE, "  [TEMP] 090804  CIMIOCPSocket::Process_IC_PARTY_CHANGE_FLIGHT_FORMATION#  %s CurIsOn(%d) FormationTy(%d<-%d) \r\n"
// 			, GetCharacterString(GetCharacter(), string()), ptmParty->m_SPartyInfo.IsAppliedFormationSkill, pMsgChangeForm->Formation, ptmParty->m_SPartyInfo.FormationType);

		///////////////////////////////////////////////////////////////////////////////
		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 편대 대형 변경시, 현재 대형을 저장한다.
		ptmParty->m_SPartyInfo.FormationType	= pMsgChangeForm->Formation;
		if(ptmParty->m_SPartyInfo.IsAppliedFormationSkill)
		{
			ptmParty->m_SPartyInfo.IsAppliedFormationSkill	= FALSE;
			this->SendFieldServPartyChangeFormationSkill(m_character.CharacterUniqueNumber, FALSE);
		}

		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK, T_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK, pChangeFormOK, pChangeFormOKBuf);
		pChangeFormOK->PartyID = pMsgChangeForm->PartyID;
		pChangeFormOK->Formation = pMsgChangeForm->Formation;
		ptmParty->SendMsgToMembers(pChangeFormOKBuf, MSG_SIZE(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK));
	}
	ptmParty->UnlockIMParty();


// 2004-08-09 by cmkwon
//	// send to SERVER
//	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK, T_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK, pFIFormationOK, pFIFormationOKBuf);
//	pFIFormationOK->PartyID		= pMsgChangeForm->PartyID;
//	pFIFormationOK->Formation	= pMsgChangeForm->Formation;
//	m_pIMParty->SendMsgToFieldServers(pFIFormationOKBuf, MSG_SIZE(MSG_FI_PARTY_CHANGE_FLIGHT_FORMATION_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_GET_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize = 0;
	MSG_IC_PARTY_GET_FLIGHT_POSITION	*pMsgGetPosition = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_GET_FLIGHT_POSITION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_GET_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12021);
		return RES_RETURN_FALSE;
	}
	pMsgGetPosition = (MSG_IC_PARTY_GET_FLIGHT_POSITION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (m_character.CharacterUniqueNumber != pMsgGetPosition->CharacterUniqueNumber)
	{
		SendErrorMessage(T_IC_PARTY_GET_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER, m_character.CharacterUniqueNumber, pMsgGetPosition->CharacterUniqueNumber);
		return RES_BREAK;
	}

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_GET_FLIGHT_POSITION, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();
	// 파티장에게 보내기
	CIMIOCPSocket *pPartyMasterSock = ptmParty->GetMasterIMIOCPSocket();
	if (NULL == pPartyMasterSock 
		|| FALSE == pPartyMasterSock->IsUsing())
	{		
		SendErrorMessage(T_IC_PARTY_GET_FLIGHT_POSITION, ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED,
						pPartyMasterSock->m_character.CharacterUniqueNumber, ptmParty->GetMasterCharacterUniqueNumber());
		ptmParty->UnlockIMParty();
		return RES_BREAK;
	}
	ptmParty->UnlockIMParty();

	// 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_GET_FLIGHT_POSITION, T_IC_PARTY_GET_FLIGHT_POSITION, pSend, pSendBuf);
	pSend->CharacterUniqueNumber = pMsgGetPosition->CharacterUniqueNumber;
	pPartyMasterSock->SendAddData(pSendBuf, MSG_SIZE(MSG_IC_PARTY_GET_FLIGHT_POSITION));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_CHANGE_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize = 0;
	MSG_IC_PARTY_CHANGE_FLIGHT_POSITION	*pMsgChangePos = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12021);
		return RES_RETURN_FALSE;
	}
	pMsgChangePos = (MSG_IC_PARTY_CHANGE_FLIGHT_POSITION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_POSITION, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();
	{
		if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
		{
			SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_POSITION, ERR_PROTOCOL_PARTY_PERMISSION_DENIED, ptmParty->GetMasterCharacterUniqueNumber(), m_character.CharacterUniqueNumber);
			ptmParty->UnlockIMParty();
			return RES_BREAK;
		}

		// 파티원 찾기
		IMPartyMember *pMember = ptmParty->GetMemberPtrByCharacterUniqueNumber(pMsgChangePos->CharacterUniqueNumber);		
		if (NULL == pMember
			|| FALSE == pMember->IsOnlineMember()
			|| FALSE == pMember->PM_pIMIOCPSocket->IsUsing()
			|| pMember->PM_pIMIOCPSocket->m_character.CharacterUniqueNumber != pMember->PM_CharacterUniqueNumber)
		{
			ptmParty->UnlockIMParty();
			SendErrorMessage(T_IC_PARTY_CHANGE_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER,
							pMsgChangePos->CharacterUniqueNumber, pMember->PM_CharacterUniqueNumber);
			return RES_BREAK;
		}

		// 전송
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION, T_IC_PARTY_CHANGE_FLIGHT_POSITION, pSend, tmSendBuf);
		pSend->CharacterUniqueNumber = pMsgChangePos->CharacterUniqueNumber;
		pSend->FlightPosition = pMsgChangePos->FlightPosition;
		ptmParty->SendMsgToMembers(tmSendBuf, MSG_SIZE(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION));
	}
	ptmParty->UnlockIMParty();

// 2004-08-09 by cmkwon
//	INIT_MSG(MSG_FI_PARTY_CHANGE_FLIGHT_POSITION, T_FI_PARTY_CHANGE_FLIGHT_POSITION, pFISend, tmSendBuf);
//	pFISend->PartyID				= m_pIMParty->m_PartyID;
//	pFISend->CharacterUniqueNumber	= pMsgChangePos->CharacterUniqueNumber;
//	pFISend->FlightPosition			= pMsgChangePos->FlightPosition;
//	m_pIMParty->SendMsgToFieldServers(tmSendBuf, MSG_SIZE(MSG_FI_PARTY_CHANGE_FLIGHT_POSITION));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_CANCEL_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize = 0;
	MSG_IC_PARTY_CANCEL_FLIGHT_POSITION	*pMsgCancelPos = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_CANCEL_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12021);
		return RES_RETURN_FALSE;
	}
	pMsgCancelPos = (MSG_IC_PARTY_CANCEL_FLIGHT_POSITION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (pMsgCancelPos->CharacterUniqueNumber != m_character.CharacterUniqueNumber)
	{
		SendErrorMessage(T_IC_PARTY_CANCEL_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_CHARACTER_UNIQUENUMBER,
						pMsgCancelPos->CharacterUniqueNumber, m_character.CharacterUniqueNumber);
		return RES_BREAK;
	}

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_CANCEL_FLIGHT_POSITION, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();

	// 파티장에게 보내기
	CIMIOCPSocket *pPartyMasterSock = ptmParty->GetMasterIMIOCPSocket();
	
	// 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
	CIMIOCPSocket *pPartyUserSock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgCancelPos->CharacterUniqueNumber);
	// end 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정

	if (pPartyMasterSock == NULL
		|| FALSE == pPartyMasterSock->IsUsing())
	{
		SendErrorMessage(T_IC_PARTY_CANCEL_FLIGHT_POSITION, ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED,
						ptmParty->GetMasterCharacterUniqueNumber());
		ptmParty->UnlockIMParty();
		return RES_BREAK;
	}
	ptmParty->UnlockIMParty();

	// 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
    // 마스터 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION, T_IC_PARTY_CANCEL_FLIGHT_POSITION, pCancel, tmSendBuf);
	pCancel->Formation_On_Off		= FORMATION_SKILL_NULL;
	pCancel->CharacterUniqueNumber = pMsgCancelPos->CharacterUniqueNumber;
	pPartyMasterSock->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION));
	
	// 2012-02-23 by hskim, 편대 탈퇴 처리 중 서버 죽는 문제 수정
	if( NULL == pPartyUserSock || FALSE == pPartyUserSock->IsUsing() )
	{
		return RES_BREAK;
	}
	// 2012-02-23 by hskim, 편대 탈퇴 처리 중 서버 죽는 문제 수정

    // 해당 유저에게도 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION, T_IC_PARTY_CANCEL_FLIGHT_POSITION, pPartyUserCancel, tmSendBuf1);
	pPartyUserCancel->Formation_On_Off		= FORMATION_SKILL_OFF;
	pPartyUserCancel->CharacterUniqueNumber = pMsgCancelPos->CharacterUniqueNumber;
	pPartyUserSock->SendAddData(tmSendBuf1, MSG_SIZE(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION));
	// END 2011-02-22 by shcho&hsSon, 편대버프 해제 안되는 버그 수정
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_PARTY_ALL_FLIGHT_POSITION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize = 0;
	MSG_IC_PARTY_ALL_FLIGHT_POSITION	*pRecvMsg = NULL;

	nRecvTypeSize = sizeof(MSG_IC_PARTY_ALL_FLIGHT_POSITION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_PARTY_ALL_FLIGHT_POSITION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12021);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_IC_PARTY_ALL_FLIGHT_POSITION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 대형 정렬 정보 전송시 오프라인유저체크
	BOOL bIsAllPartyMemberOnline = TRUE;
	for(int i=0; i < SIZE_MAX_PARTY_MEMBER-1; i++)
	{
		if(pRecvMsg->AllPartyMemberCharacterUniqueNumber[i])
		{
			CIMIOCPSocket *pIISoc = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRecvMsg->AllPartyMemberCharacterUniqueNumber[i]);
			if(NULL == pIISoc
				|| FALSE == pIISoc->IsValidCharacter())
			{
				bIsAllPartyMemberOnline		= FALSE;
			}
		}
	}

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_ALL_FLIGHT_POSITION, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

// 	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 임시용 로그
// 	g_pIMGlobal->WriteSystemLogEX(TRUE, "[TEMP] 090804  Process_IC_PARTY_ALL_FLIGHT_POSITION# %s IsOn(%d) FormationTy(%d) AllOnline(%d) (%d %d %d %d %d)\r\n"
// 		, GetCharacterString(GetCharacter(), string()), ptmParty->m_SPartyInfo.IsAppliedFormationSkill, ptmParty->m_SPartyInfo.FormationType, bIsAllPartyMemberOnline
// 		, pRecvMsg->AllPartyMemberCharacterUniqueNumber[0], pRecvMsg->AllPartyMemberCharacterUniqueNumber[1], pRecvMsg->AllPartyMemberCharacterUniqueNumber[2]
// 		, pRecvMsg->AllPartyMemberCharacterUniqueNumber[3], pRecvMsg->AllPartyMemberCharacterUniqueNumber[4]);

	ptmParty->LockIMParty();
	{
		if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
		{
			SendErrorMessage(T_IC_PARTY_ALL_FLIGHT_POSITION, ERR_PROTOCOL_PARTY_PERMISSION_DENIED
				, m_character.CharacterUniqueNumber, ptmParty->GetMasterCharacterUniqueNumber());
			ptmParty->UnlockIMParty();
			return RES_BREAK;
		}

		// 파티장 체크
		CIMIOCPSocket *pPartyMasterSock = ptmParty->GetMasterIMIOCPSocket();
		if (pPartyMasterSock == NULL
			|| !pPartyMasterSock->IsUsing())
		{
			SendErrorMessage(T_IC_PARTY_ALL_FLIGHT_POSITION, ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED,
							ptmParty->GetMasterCharacterUniqueNumber());
			ptmParty->UnlockIMParty();
			return RES_BREAK;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
		if(FALSE == bIsAllPartyMemberOnline
			|| 0 == pRecvMsg->AllPartyMemberCharacterUniqueNumber[SIZE_MAX_PARTY_MEMBER-1-1]
			|| FLIGHT_FORM_NONE == ptmParty->m_SPartyInfo.FormationType)
		{
			// 편대 대형 스킬을 끄는 처리를 해야함.
			if(ptmParty->m_SPartyInfo.IsAppliedFormationSkill)
			{
				ptmParty->m_SPartyInfo.IsAppliedFormationSkill	= FALSE;
				this->SendFieldServPartyChangeFormationSkill(m_character.CharacterUniqueNumber, FALSE);
			}
		}
		else
		{
			// 편대 대형 스킬을 키는 처리를 해야함.
			if(FALSE == ptmParty->m_SPartyInfo.IsAppliedFormationSkill)
			{
				ptmParty->m_SPartyInfo.IsAppliedFormationSkill	= TRUE;
				this->SendFieldServPartyChangeFormationSkill(m_character.CharacterUniqueNumber, TRUE, ptmParty->m_SPartyInfo.FormationType);
			}
		}
		
		// 파티장으로 부터 전송받은 편대 Position 정보를 편대장을 제외한 편대원 모두에게 전송한다
		INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_ALL_FLIGHT_POSITION, T_IC_PARTY_ALL_FLIGHT_POSITION, pSend, tmSendBuf);
		*pSend = *pRecvMsg;
		ptmParty->SendMsgToMembers(tmSendBuf, MSG_SIZE(MSG_IC_PARTY_ALL_FLIGHT_POSITION), m_character.CharacterUniqueNumber);
	}
	ptmParty->UnlockIMParty();

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 통합 - 아레나 종료 후 메인 서버에 왔을 때 기존 파티 검사
/// \author		dhjin
/// \date		2008-02-28 ~ 2008-02-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (this->m_character.LastPartyID != 0)
	{
#ifdef _DEBUG
		this->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0005);
#endif
		// 파티 검색
		CIMParty *pParty = ms_pIMIOCP->FindParty(this->m_character.LastPartyID);
		if (pParty == NULL)
		{
			// 파티가 이미 해산된 경우
			this->SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_PROTOCOL_NO_SUCH_PARTY, this->m_character.LastPartyID);

			this->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0006);

			// Update DB
			QPARAM_SAVELASTPARTYID *pQSavePartyID = new QPARAM_SAVELASTPARTYID;
			pQSavePartyID->CharacterUniqueNumber = this->m_character.CharacterUniqueNumber;
			pQSavePartyID->LastPartyID = 0;
			ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_SaveLastPartyID, this, pQSavePartyID);

			// Update CHARACTER
			this->m_character.LastPartyID = 0;
			return RES_RETURN_TRUE;
		}
		
		pParty->LockIMParty();
		{
			IMPartyMember *pMember = pParty->GetMemberPtrByCharacterUniqueNumber(this->m_character.CharacterUniqueNumber);
			if (pMember == NULL)
			{
				// 파티원이 없는 경우
				pParty->UnlockIMParty();
				return RES_RETURN_TRUE;
			}

			// 사용자에게 파티 정보 할당
			this->SetIMParty(pParty);

			// validate party member
			pMember->SetOnlineMember(this);

			// 자기 자신에게 보냄
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_PUT_LAST_PARTY_INFO, T_IC_PARTY_PUT_LAST_PARTY_INFO, pLastPartyInfo, pLastPartyInfoBuf);
			pLastPartyInfo->PartyID = pParty->m_PartyID;
			this->SendAddData(pLastPartyInfoBuf, MSG_SIZE(MSG_IC_PARTY_PUT_LAST_PARTY_INFO));

			this->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0007);
		}
		pParty->UnlockIMParty();
	}	

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_LEAVE_FROM_M_TO_A(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 통합 - 아레나 시작을 위해 메인서버에서 파티 관련 처리	
/// \author		dhjin
/// \date		2008-02-28 ~ 2008-02-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_LEAVE_FROM_M_TO_A(const char* pPacket, int nLength, int &nBytesUsed)
{
	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty)
	{
		ptmParty->LockIMParty();
		PartyID_t partyID = ptmParty->m_PartyID;
		BOOL bReted = ProcessPartyFromMainServerToArenaServerNoLock(ptmParty);
		ptmParty->UnlockIMParty();

		if(FALSE == bReted)
		{// 파티맵에서 제거해야한다
			
			ProcessPartyDismember(partyID);			
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 캐릭터의 파티 정보 삭제
		this->SetIMParty(NULL);

		//본섭 파티는 탈퇴 처리로 수정되어 DB저장하지 않음. 2010.09.03. by hsLee.
		/*
		QPARAM_SAVELASTPARTYID *pQSavePartyID	= new QPARAM_SAVELASTPARTYID;
		pQSavePartyID->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
		pQSavePartyID->LastPartyID				= partyID;
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_SaveLastPartyID, this, pQSavePartyID);
		*/
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_LEAVE_FROM_A_TO_M(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		아레나 서버에서 메인서버로 복귀할때 아레나 서버에서 파티 탈퇴 처리 클라이언트에게 전송하지 않는다.
/// \author		dhjin
/// \date		2008-02-28 ~ 2008-02-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_LEAVE_FROM_A_TO_M(const char* pPacket, int nLength, int &nBytesUsed)
{
	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		return RES_BREAK;
	}

	// 2008-10-28 by dhjin, 파티관련 lock변경
	ptmParty->LockIMParty();
	PartyID_t partyID = ptmParty->m_PartyID;
	BOOL bReted = ProcessPartyLeaveArenaEndNoLock(ptmParty);
	ptmParty->UnlockIMParty();

	if(FALSE == bReted)
	{// 파티맵에서 제거해야한다
		// 2008-10-28 by dhjin, 파티관련 lock변경 - 아래와 같이 변경
		// ProcessPartyDismember(ptmParty->m_PartyID);
		ProcessPartyDismember(partyID);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 캐릭터의 파티 정보 삭제
	this->SetIMParty(NULL);	

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_LIST_INFO(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 편대 수정 - 편대 정보 리스트 요청	
/// \author		dhjin
/// \date		2008-06-02 ~ 2008-06-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_LIST_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_PARTY_LIST_INFO,
									MSG_IC_PARTY_LIST_INFO, pMsg);
	m_bArenaIMServerCheck = g_pIMGlobal->m_bArenaIMServerCheck;	// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크
	this->SendPartyListInfo(pMsg->StartNum);
	
	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_JOIN_FREE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 편대 수정 - 편대 자유 참여
/// \author		dhjin
/// \date		2008-06-03 ~ 2008-06-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_JOIN_FREE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_PARTY_JOIN_FREE,
									MSG_IC_PARTY_JOIN_FREE, pMsg);
	
	CIMParty *pParty = ms_pIMIOCP->FindParty(pMsg->PartyNum);
	if (pParty == NULL
#ifdef FREEWAR_ // 2015-11-04 DevX does not let client in free war map join a party
		|| isFreeForAllMap(this->m_character.MapChannelIndex.MapIndex)
#endif
		)
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_NO_SUCH_PARTY, pMsg->PartyNum);
		return RES_BREAK;
	}
	
	pParty->LockIMParty();
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-06-03 by dhjin, 파티 입장 전 체크 사항들
	if(NULL != this->m_pIMParty)
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_PARTY_MEMBER_ALREADY_EXISTS, pMsg->PartyNum);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}

	// 2008-11-06 by cmkwon, 편대 자유 참여 관련 버그 수정(E0016695) - 
	if(PARTY_JOIN_TYPE_FREE != pParty->m_SPartyInfo.PartyJoinType)
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_NO_SUCH_PARTY, pMsg->PartyNum);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}

	if(pParty->m_SPartyInfo.PartyLock)
	{
		if(strcmp(pMsg->PartyPW, pParty->m_SPartyInfo.PartyPW))
		{
			SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_REQ_PW_NOT_MATCHED, pMsg->PartyNum);
			pParty->UnlockIMParty();
			return RES_BREAK;
		}
	}
	if(SIZE_MAX_PARTY_MEMBER <= pParty->GetMemberCount(FALSE) )
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_ALREADY_MAX_PARTY_MEMBER, pMsg->PartyNum);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}
	if(this->m_character.InfluenceType != pParty->m_SPartyInfo.PartyInfluence)
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED, pMsg->PartyNum);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}
	if(this->m_character.Level < pParty->m_SPartyInfo.MinLevel)
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_REQ_MINLEVEL_NOT_MATCHED, pMsg->PartyNum);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}
	if(this->m_character.Level > pParty->m_SPartyInfo.MaxLevel)
	{
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_REQ_MAXLEVEL_NOT_MATCHED, pMsg->PartyNum);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}

	CIMIOCPSocket *pMasterSocket = pParty->GetMasterIMIOCPSocket();
	if(NULL == pMasterSocket || FALSE == pMasterSocket->IsValidCharacter())
	{// 2008-12-16 by dhjin, EP3 거부목록 - 거부목록과 관련하여 추가 보안 코드
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED);
		pParty->UnlockIMParty();
		return RES_BREAK;
	}
	if(this->IsCharacterNameFromRejectList(pMasterSocket->m_character.CharacterName)
		|| pMasterSocket->IsCharacterNameFromRejectList(this->m_character.CharacterName))
	{// 2008-07-03 by dhjin, EP3 거부목록 -
		SendErrorMessage(T_IC_PARTY_JOIN_FREE, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);	
		pParty->UnlockIMParty();
		return RES_BREAK;
	}

	// party 할당
	this->SetIMParty(pParty);

	// party원 삽입
	IMPartyMember tmMember(this->m_character.CharacterName, this->m_character.CharacterUniqueNumber, this, pParty);
	pParty->PushBackMember(tmMember);

	// 2008-02-28 by dhjin,	아레나 통합 - LastPartyID에 값을 저장한다.
	this->m_character.LastPartyID	= pMsg->PartyNum;

	// send to SERVER
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_ACCEPT_INVITE_OK, T_FI_PARTY_ACCEPT_INVITE_OK, pInviteOKFI, pInviteOKFIBuf);
	pInviteOKFI->PartyID				= pMsg->PartyNum;
	pInviteOKFI->CharacterUniqueNumber	= this->m_character.CharacterUniqueNumber;
	pInviteOKFI->FieldPartyInfo.nTotalPartyMember = pParty->GetMemberCount(FALSE);		// 주의: m_ListPartyMember에 삽입 이후여야 함!
	pInviteOKFI->FieldPartyInfo.lowestMemberLevel = pParty->GetLowestMemberLevel();		// 주의: m_ListPartyMember에 삽입 이후여야 함!
	this->SendMsgToCurrentFieldServerSocket(pInviteOKFIBuf, MSG_SIZE(MSG_FI_PARTY_ACCEPT_INVITE_OK));

	// client들에게 전송하기
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_ACCEPT_INVITE_OK, T_IC_PARTY_ACCEPT_INVITE_OK, pInviteOKIC, pInviteOKICBuf);
	pInviteOKIC->PartyID			= pMsg->PartyNum;
	pInviteOKIC->IMPartyMemberInfo	= this->m_character;
	pParty->SendMsgToMembers(pInviteOKICBuf, MSG_SIZE(MSG_IC_PARTY_ACCEPT_INVITE_OK));

	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_INFO, T_IC_PARTY_INFO, pSendMsg, pSendBuf);
	pSendMsg->PartyInfo = pParty->m_SPartyInfo;
	this->SendAddData(pSendBuf, MSG_SIZE(MSG_IC_PARTY_INFO));
	pParty->UnlockIMParty();	

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_CHANGE_INFO(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 편대 수정 - 편대 자유 참여
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_CHANGE_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_PARTY_CHANGE_INFO,
									MSG_IC_PARTY_CHANGE_INFO, pMsg);
	
	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_INFO, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	// 2008-06-02 by dhjin, EP3 편대 수정 - 편대 정보
	// 2009-12-29 by cmkwon, 캐릭터 최대 레벨 상향(110으로) - 아래와 같이 수정
	//if(pMsg->PartyInfo.MinLevel > pMsg->PartyInfo.MaxLevel
	//	|| pMsg->PartyInfo.MinLevel <= 0
	//	|| pMsg->PartyInfo.MaxLevel > 100
	//	)
	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-29 by cmkwon, 캐릭터 최대 레벨 상향(110으로) - 
	if(pMsg->PartyInfo.MinLevel > pMsg->PartyInfo.MaxLevel
		|| FALSE == util::in_range(1, pMsg->PartyInfo.MinLevel, CHARACTER_MAX_LEVEL)
		|| FALSE == util::in_range(1, pMsg->PartyInfo.MaxLevel, CHARACTER_MAX_LEVEL)
		)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_INFO, ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED, pMsg->PartyInfo.MinLevel, pMsg->PartyInfo.MaxLevel);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();

	if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
	{
		SendErrorMessage(T_IC_PARTY_CHANGE_INFO, ERR_PROTOCOL_PARTY_PERMISSION_DENIED
			, m_character.CharacterUniqueNumber, ptmParty->GetMasterCharacterUniqueNumber());
		ptmParty->UnlockIMParty();
		return RES_BREAK;
	}

	if(ptmParty->m_SPartyInfo.ExpDistributeType != pMsg->PartyInfo.ExpDistributeType)
	{
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE, T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE, pSendMsg, pSendBuf);
		pSendMsg->PartyID			= ptmParty->m_PartyID;
		pSendMsg->ExpDistributeType	= pMsg->PartyInfo.ExpDistributeType;
		this->SendMsgToCurrentFieldServerSocket(pSendBuf, MSG_SIZE(MSG_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE));
	}
	if(ptmParty->m_SPartyInfo.ItemDistributeType != pMsg->PartyInfo.ItemDistributeType)
	{
		INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE, T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE, pSendMsg, pSendBuf);
		pSendMsg->PartyID				= ptmParty->m_PartyID;
		pSendMsg->ItemDistributeType	= pMsg->PartyInfo.ItemDistributeType;
		this->SendMsgToCurrentFieldServerSocket(pSendBuf, MSG_SIZE(MSG_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE));
	}

	ptmParty->SetPartyInfo(&(pMsg->PartyInfo));
	this->SendAddMessageType(T_IC_PARTY_CHANGE_INFO_OK);
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_INFO, T_IC_PARTY_INFO, pSendMsg, pSendBuf);
	pSendMsg->PartyInfo = pMsg->PartyInfo;
	ptmParty->SendMsgToMembers(pSendBuf, MSG_SIZE(MSG_IC_PARTY_INFO));
	ptmParty->UnlockIMParty();
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_RECOMMENDATION_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 편대 수정 - 추천 케릭터 요청
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_RECOMMENDATION_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL)
	{
		SendErrorMessage(T_IC_PARTY_RECOMMENDATION_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}

	ptmParty->LockIMParty();

	if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
	{
		SendErrorMessage(T_IC_PARTY_RECOMMENDATION_MEMBER, ERR_PROTOCOL_PARTY_PERMISSION_DENIED
			, m_character.CharacterUniqueNumber, ptmParty->GetMasterCharacterUniqueNumber());
		ptmParty->UnlockIMParty();
		return RES_BREAK;
	}
	ptmParty->m_bArenaIMServerCheck	= g_pIMGlobal->m_bArenaIMServerCheck;	// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 서버인지 체크
	ptmParty->SendRecommendationMember(this);

	ptmParty->UnlockIMParty();	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_PARTY_GET_AUTO_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		인피니티 - 자동 편대 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_PARTY_GET_AUTO_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_PARTY_GET_AUTO_PARTY_INFO,
		MSG_IC_PARTY_GET_AUTO_PARTY_INFO, pMsg);

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty == NULL
		|| ptmParty->m_PartyID != pMsg->PartyID) {
		SendErrorMessage(T_IC_PARTY_GET_AUTO_PARTY_INFO, ERR_PROTOCOL_NO_SUCH_PARTY);
		return RES_BREAK;
	}
	
	ptmParty->LockIMParty();

	// 파티 및 파티원 정보 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK, T_IC_PARTY_GET_AUTO_PARTY_INFO_OK, pSendMsg, pSendBuf);
	pSendMsg->PartyInfo = ptmParty->m_SPartyInfo;
	pSendMsg->PartyID				= ptmParty->m_PartyID;
	pSendMsg->MasterUniqueNumber	= ptmParty->GetMasterCharacterUniqueNumber();
	ptmParty->MakeMsg_IC_PARTY_PUT_ALL_MEMBER(&pSendMsg->nNumOfPartyMembers, (IM_PARTY_MEMBER_INFO*)(pSendBuf+MSG_SIZE(MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK)), m_character.CharacterUniqueNumber);
	ptmParty->UnlockIMParty();	// unlock
	
	// 데이터 전송
	SendAddData(pSendBuf, MSG_SIZE(MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK) + pSendMsg->nNumOfPartyMembers*sizeof(IM_PARTY_MEMBER_INFO));

	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Guild
ProcessResult CIMIOCPSocket::Process_IC_GUILD_CREATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_IC_GUILD_CREATE		*pMsgCreate = NULL;

	nRecvTypeSize = sizeof(MSG_IC_GUILD_CREATE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12023);
		return RES_RETURN_FALSE;
	}
	pMsgCreate = (MSG_IC_GUILD_CREATE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if(COMPARE_INFLUENCE(m_character.InfluenceType, INFLUENCE_TYPE_NORMAL))
	{// 2005-12-06 by cmkwon, 일반세력은 여단 생성 불가, 세력선택 후 여단 생성 가능
		SendErrorMessage(T_IC_GUILD_CREATE, ERR_REQ_CHOOSE_INFLUENCE_TYPE);
		return RES_BREAK;
	}

	// 여단장의 레벨 확인
	if (FALSE == GCheckLimitLevel(CHECK_TYPE_GUILD_CREATE, m_character.Level))
	{
		SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_REQ_LEVEL_NOT_MATCHED);
		return RES_BREAK;
	}

	if(FALSE == CAtumSJ::IsValidGuildName(pMsgCreate->GuildName))
	{
		SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_INVLID_GUILDNAME, 1, 0, pMsgCreate->GuildName);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-21 by cmkwon, 유효한 여단명 체크시에 욕설, 금지어도 체크 추가 - 
	if (FALSE == g_pIMGlobal->CheckProhibitedName(pMsgCreate->GuildName))
	{
		SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_INVLID_GUILDNAME, 2, 0, pMsgCreate->GuildName);
		return RES_BREAK;
	}


	// 2008-06-04 by dhjin, EP3 - 여단 수정 - 파티 필요없이 생성가능 밑과 같이 수정
	CAtumIMDBManager::MSG_DB_GUILD_CREATE *pDBCreate = new CAtumIMDBManager::MSG_DB_GUILD_CREATE;
	util::zero(pDBCreate, sizeof(CAtumIMDBManager::MSG_DB_GUILD_CREATE));
	CIMParty *ptmParty = this->GetIMParty();
	if (NULL == ptmParty)
	{
		pDBCreate->arrGuildRank[0]			= GUILD_RANK_COMMANDER;	
		pDBCreate->arrMemberCharacterUID[0]	= this->m_character.CharacterUniqueNumber;
		pDBCreate->nMemberCounts			= 1;
		util::strncpy(pDBCreate->szGuildName, pMsgCreate->GuildName, SIZE_MAX_GUILD_NAME);
		pDBCreate->uidCommanderCharacter	= this->m_character.CharacterUniqueNumber;
	}
	else
	{
		ptmParty->LockIMParty();
		{
			if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
			{
				// error: 파티장 불일치, 파티장만이 길드 생성 가능
				ptmParty->UnlockIMParty();
				SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_PARTY_MASTER_NOT_MATCHED);
				util::del(pDBCreate);
				return RES_BREAK;
			}

			// 초기 길드 생성 사 파티원은 최소한 1명 이상이어야 함
			if (!COMPARE_RACE(m_character.Race, RACE_OPERATION | RACE_GAMEMASTER)
				&& ptmParty->GetMemberCount() < 1)
			{
				ptmParty->UnlockIMParty();
				SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_NOT_ENOUGH_PARTY_MEMBER);
				util::del(pDBCreate);
				return RES_BREAK;
			}

			int nRetedErrNum = ptmParty->MakeMsg_DB_GUILD_CREATE(&pDBCreate->nMemberCounts, pDBCreate->arrMemberCharacterUID);
			switch(nRetedErrNum)
			{
			case ERR_NO_ERROR:
				{
					util::strncpy(pDBCreate->szGuildName, pMsgCreate->GuildName, SIZE_MAX_GUILD_NAME);
					pDBCreate->uidCommanderCharacter = ptmParty->GetMasterCharacterUniqueNumber();
				}
				break;
			case ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER:
				{
					ptmParty->UnlockIMParty();
					SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, 0, 0, STRMSG_S_I2NOTIFY_0010);
					util::del(pDBCreate);
					return RES_BREAK;
				}			
			case ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS:			
				{
					ptmParty->UnlockIMParty();
					SendErrorMessage(T_IC_GUILD_CREATE, ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS, 0, 0, STRMSG_S_I2NOTIFY_0011);
					util::del(pDBCreate);
					return RES_BREAK;
				}
				break;
			default:
				{
					ptmParty->UnlockIMParty();
					SendErrorMessage(T_IC_GUILD_CREATE, ERR_COMMON_UNKNOWN_ERROR);
					util::del(pDBCreate);
					return RES_BREAK;
				}
			}
		}
		ptmParty->UnlockIMParty();
	}

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildCreate, this, pDBCreate);	// DB로 인증 절차를 넘긴다.
	return RES_RETURN_TRUE;
}

// check: T_IC_GUILD_GET_GUILD_INFO로 통합, 필요하면 살림, 20040520, kelovon
//ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
//{
//
//	assert(0);
//
//	return RES_RETURN_TRUE;
//}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_GUILD_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_GET_GUILD_INFO,
									MSG_IC_GUILD_GET_GUILD_INFO, pMsgGetGuildInfo);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(this->m_uidGuildUniqueNumber)
		|| this->m_uidGuildUniqueNumber != pMsgGetGuildInfo->GuildUniqueNumber)
	{
		SendErrorMessage(T_IC_GUILD_GET_GUILD_INFO, ERR_PROTOCOL_NO_SUCH_GUILD
			, this->m_uidGuildUniqueNumber, pMsgGetGuildInfo->GuildUniqueNumber);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			SendErrorMessage(T_IC_GUILD_GET_GUILD_INFO, ERR_PROTOCOL_NO_SUCH_GUILD, pMsgGetGuildInfo->GuildUniqueNumber, this->m_uidGuildUniqueNumber);
			ms_pIMIOCP->m_MapGuild.unlock();
			return RES_BREAK;
		}
		
		ptmGuild->Send_MSG_IC_GUILD_GET_GUILD_INFO_OK(this);		
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_REQUEST_INVITE,
									MSG_IC_GUILD_REQUEST_INVITE, pMsgInvite);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(this->m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_NO_SUCH_GUILD);
		return RES_BREAK;
	}

	CIMIOCPSocket *pInviteeSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pMsgInvite->InviteeCharacterName);
	if (NULL == pInviteeSocket
		|| FALSE == pInviteeSocket->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_COMMON_NO_SUCH_CHARACTER);
		return RES_BREAK;
	}
	
	CIMIOCPSocket *pCharacterSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(m_character.CharacterUniqueNumber);
	if (pCharacterSocket == NULL || FALSE == pCharacterSocket->IsUsing())
	{
		SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_COMMON_NO_SUCH_CHARACTER, m_character.CharacterUniqueNumber);
		return RES_BREAK;
	}

	if (IS_VALID_UNIQUE_NUMBER(pInviteeSocket->m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_CANNOT_INVITE_USER);
		SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0012);
		return RES_BREAK;
	}

	if(FALSE == GCheckRaceAndInfluenceType(CHECK_TYPE_GUILD_JOIN, m_character.Race, m_character.InfluenceType,
		pInviteeSocket->GetCharacter()->Race, pInviteeSocket->GetCharacter()->InfluenceType))
	{// 2005-12-07 by cmkwon, 일반세력은 여단 초대 불가, 다른 세력간은 초대 불가, 세력선택 후 여단 초대 가능
		SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_CANNOT_INVITE_USER);
		return RES_BREAK;
	}

	if (strnicmp(pMsgInvite->InviteeCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME) == 0)
	{
		SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_CANNOT_INVITE_USER);
		SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0013);
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_REQUEST_INVITE_QUESTION, T_IC_GUILD_REQUEST_INVITE_QUESTION, msgInviteQuestion, SendBuf);

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_COMMON_NO_SUCH_CHARACTER);
			return RES_BREAK;
		}
		
		// 2008-05-28 by dhjin, EP3 여단 수정 사항 -
		CGuildMember *pMember = NULL;
		if(!ptmGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
			return RES_BREAK;
		}

		if (FALSE == ptmGuild->IsGuildCommander(m_character.CharacterUniqueNumber)
			&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState != GUILD_STATE_NORMAL)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0014);
			return RES_BREAK;
		}

		// 2006-09-19 by dhjin, 여단 인원 캐쉬 아이템 삭제
//		if (ptmGuild->m_nGuildMemberCapacity <= ptmGuild->m_ListGuildMember.size()
//			|| CAtumSJ::GetMaxGuildSize(pCharacterSocket->IsExistPremiumCard()) <= ptmGuild->m_ListGuildMember.size())
		if (CAtumSJ::GetMaxGuildSize(ptmGuild->m_nGuildMemberCapacity, pCharacterSocket->IsExistPremiumCard()) <= ptmGuild->m_ListGuildMember.size())		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0015);
			return RES_BREAK;
		}

		// msg setting
		util::strncpy(msgInviteQuestion->GuildName, ptmGuild->m_GuildName, SIZE_MAX_GUILD_NAME);
		msgInviteQuestion->GuildUniqueNumber = ptmGuild->m_GuildUniqueNumber;
	}
	ms_pIMIOCP->m_MapGuild.unlock();
	
	if(this->IsUsing() && pInviteeSocket->IsUsing())
	{
		util::strncpy(msgInviteQuestion->GuildCommanderCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);

		pInviteeSocket->SendAddData(SendBuf, MSG_SIZE(MSG_IC_GUILD_REQUEST_INVITE_QUESTION));
	}
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_IC_GUILD_ACCEPT_INVITE	*pMsgAccept = NULL;

	nRecvTypeSize = sizeof(MSG_IC_GUILD_ACCEPT_INVITE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_GUILD_ACCEPT_INVITE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x12026);
		return RES_RETURN_FALSE;
	}
	pMsgAccept = new MSG_IC_GUILD_ACCEPT_INVITE;
	*pMsgAccept = *(MSG_IC_GUILD_ACCEPT_INVITE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(pMsgAccept->GuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_ACCEPT_INVITE, ERR_PROTOCOL_NOT_HAVE_GUILD);
			util::del(pMsgAccept);
			return RES_BREAK;
		}
		
		CGuildMember *pMember = NULL;

		
		// Start 2012-01-05 by shcho, 게임 4G bugFix 길드가 있으므로 성공하면 안되는 처리 - 이미 여단에 가입된 유저 동시에 가입 버튼 눌럿을 경우 추가
		if(NULL != m_character.GuildUniqueNumber)	// 길드가 있는 사람이다. 
		{
			
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_PROTOCOL_CANNOT_INVITE_USER);
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0016);
			util::del(pMsgAccept);
			return RES_BREAK;
		}
		// End 2012-01-05 by shcho, 게임 4G bugFix 길드가 있으므로 성공하면 안되는 처리 - 이미 여단에 가입된 유저 동시에 가입 버튼 눌럿을 경우 추가

		BOOL ret = ptmGuild->GetGuildMember(m_character.CharacterUniqueNumber, pMember);
		if (ret)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_ACCEPT_INVITE, ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS);
			util::del(pMsgAccept);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState != GUILD_STATE_NORMAL)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0016);
			util::del(pMsgAccept);
			return RES_BREAK;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-31 by cmkwon, 여단원 최대 카운트 체크
		// 2008-07-18 by dhjin, EP3 - 여단 수정 사항 - 밑과 같이 수정 
//		CGuildMember *ptmGMember = NULL;
//		ptmGuild->GetGuildMember(ptmGuild->m_CommanderUniqueNumber, ptmGMember);
//		if(NULL == ptmGMember
//			|| NULL == ptmGMember->m_pIMIOCPSocket
//			|| FALSE == ptmGMember->m_pIMIOCPSocket->IsValidCharacter())
//		{
//			ms_pIMIOCP->m_MapGuild.unlock();
//			SendErrorMessage(T_IC_GUILD_REQUEST_INVITE, ERR_COMMON_NO_SUCH_CHARACTER);
//			util::del(pMsgAccept);
//			return RES_BREAK;
//		}
//		if (CAtumSJ::GetMaxGuildSize(ptmGuild->m_nGuildMemberCapacity, ptmGMember->m_pIMIOCPSocket->IsExistPremiumCard()) <= ptmGuild->m_ListGuildMember.size())		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템	
//		{
//			ms_pIMIOCP->m_MapGuild.unlock();
//			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0015);
//			util::del(pMsgAccept);
//			return RES_BREAK;
//		}
		CGuildMember *ptmGMember = NULL;
		ptmGuild->GetGuildMember(ptmGuild->m_CommanderUniqueNumber, ptmGMember);
		if(NULL == ptmGMember)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_ACCEPT_INVITE, ERR_COMMON_NO_SUCH_CHARACTER);
			util::del(pMsgAccept);
			return RES_BREAK;
		}
		if (CAtumSJ::GetMaxGuildSize(ptmGuild->m_nGuildMemberCapacity, ptmGuild->m_bMemberShip) <= ptmGuild->m_ListGuildMember.size())		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템	
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0015);
			util::del(pMsgAccept);
			return RES_BREAK;
		}
	}
	ms_pIMIOCP->m_MapGuild.unlock();	

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildAddMember, this, pMsgAccept);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_REJECT_INVITE,
									MSG_IC_GUILD_REJECT_INVITE, pMsgReject);

	UID32_t uidGuildMaster = INVALID_GUILD_UID;
	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(pMsgReject->GuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_REJECT_INVITE, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		uidGuildMaster = ptmGuild->m_CommanderUniqueNumber;
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	CIMIOCPSocket *pMasterSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(uidGuildMaster);
	if (NULL == pMasterSocket || FALSE == pMasterSocket->IsUsing())
	{
		SendErrorMessage(T_IC_GUILD_REJECT_INVITE, ERR_COMMON_NO_SUCH_CHARACTER);
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_REJECT_INVITE_OK, T_IC_GUILD_REJECT_INVITE_OK, msgRejectOK, SendBuf);
	util::strncpy(msgRejectOK->CharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);

	pMasterSocket->SendAddData(SendBuf, MSG_SIZE(MSG_IC_GUILD_REJECT_INVITE_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_LEAVE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_LEAVE,
									MSG_IC_GUILD_LEAVE, pMsgLeave);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_LEAVE, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_LEAVE, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		if (m_character.CharacterUniqueNumber == ptmGuild->m_CommanderUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_LEAVE, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0017);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState == GUILD_STATE_IN_GUILD_WAR)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0018);
			return RES_BREAK;
		}

		if (pMsgLeave->GuildUniqueNumber != ptmGuild->m_GuildUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_LEAVE, ERR_PROTOCOL_GUILD_NOT_MATCHED);
			return RES_BREAK;
		}

		CGuildMember *pRetMember = NULL;
		BOOL bRet = ptmGuild->GetGuildMember(m_character.CharacterUniqueNumber, pRetMember);
		if (!bRet)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_LEAVE, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
			return RES_BREAK;
		}
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildLeaveMember, this, NULL, NULL,
						m_character.CharacterUniqueNumber, this->m_uidGuildUniqueNumber);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_BAN_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_BAN_MEMBER,
									MSG_IC_GUILD_BAN_MEMBER, pMsgBan);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_BAN_MEMBER, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_BAN_MEMBER, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		CGuildMember *pMySelfMember = NULL;
		if(!ptmGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMySelfMember))
		{// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 -
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_BAN_MEMBER, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
			return RES_BREAK;
		}

		if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber
			&& GUILD_RANK_SUBCOMMANDER != pMySelfMember->m_Rank			// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 -
			)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_BAN_MEMBER, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState == GUILD_STATE_IN_GUILD_WAR)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0019);
			return RES_BREAK;
		}

		if (pMsgBan->MemberUniqueNumber == ptmGuild->m_CommanderUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_BAN_MEMBER, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0020);
			return RES_BREAK;
		}

		CGuildMember *pRetMember = NULL;
		BOOL bRet = ptmGuild->GetGuildMember(pMsgBan->MemberUniqueNumber, pRetMember);
		if (!bRet)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_BAN_MEMBER, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
			return RES_BREAK;
		}
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildBanMember, this, NULL, NULL, pMsgBan->MemberUniqueNumber, m_uidGuildUniqueNumber);
	return RES_RETURN_TRUE;
}

// 2007-11-12 by dhjin, Process_IC_GUILD_DISMEMBER 함수명은 변경없이 T_FI_GUILD_DISMEMBER로 변경
ProcessResult CIMIOCPSocket::Process_IC_GUILD_DISMEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_IC_GUILD_DISMEMBER Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_GUILD_DISMEMBER,
									MSG_FI_GUILD_DISMEMBER, pMsgDismember);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(pMsgDismember->GuildUniqueNumber))
	{
		SendErrorMessage(T_FC_GUILD_DISMEMBER, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(pMsgDismember->GuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_FC_GUILD_DISMEMBER, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState == GUILD_STATE_IN_GUILD_WAR)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0021);
			return RES_BREAK;
		}
// 2007-11-12 by dhjin,	Field서버에서 체크
//		if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
//		{
//			ms_pIMIOCP->m_MapGuild.unlock();
//			SendErrorMessage(T_IC_GUILD_DISMEMBER, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
//			return RES_BREAK;
//		}
//
//		if (pMsgDismember->GuildUniqueNumber != ptmGuild->m_GuildUniqueNumber)
//		{
//			ms_pIMIOCP->m_MapGuild.unlock();
//			SendErrorMessage(T_IC_GUILD_DISMEMBER, ERR_PROTOCOL_NO_SUCH_GUILD);
//			return RES_BREAK;
//		}

	}
	ms_pIMIOCP->m_MapGuild.unlock();	

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDismember, this, NULL, NULL, pMsgDismember->GuildUniqueNumber);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_CANCEL_DISMEMBER(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_CANCEL_DISMEMBER,
									MSG_IC_GUILD_CANCEL_DISMEMBER, pMsgCancelDismember);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_CANCEL_DISMEMBER, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_CANCEL_DISMEMBER, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_CANCEL_DISMEMBER, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if (pMsgCancelDismember->GuildUniqueNumber != ptmGuild->m_GuildUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_CANCEL_DISMEMBER, ERR_PROTOCOL_NO_SUCH_GUILD);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState != GUILD_STATE_DISMEMBER_READY)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_CANCEL_DISMEMBER, ERR_PROTOCOL_NO_SUCH_GUILD);
			return RES_BREAK;
		}
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildCancelDismember, this, NULL, NULL, pMsgCancelDismember->GuildUniqueNumber);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_DISMEMBER_DATE(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_GET_DISMEMBER_DATE,
									MSG_IC_GUILD_GET_DISMEMBER_DATE, pMsgGetDate);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_GET_DISMEMBER_DATE, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_DISMEMBER_DATE_OK, T_IC_GUILD_GET_DISMEMBER_DATE_OK, msgGetDateOK, msgGetDateOKBuf);
	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_GET_DISMEMBER_DATE, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		if (pMsgGetDate->GuildUniqueNumber != ptmGuild->m_GuildUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_GET_DISMEMBER_DATE, ERR_PROTOCOL_NO_SUCH_GUILD);
			return RES_BREAK;
		}

		msgGetDateOK->DismemberDateTime = ptmGuild->m_DismemberDate;
	}
	ms_pIMIOCP->m_MapGuild.unlock();	

	SendAddData(msgGetDateOKBuf, MSG_SIZE(MSG_IC_GUILD_GET_DISMEMBER_DATE_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_CHANGE_GUILD_NAME(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_CHANGE_GUILD_NAME,
									MSG_IC_GUILD_CHANGE_GUILD_NAME, pMsgChangeGuildName);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_CHANGE_GUILD_NAME, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_CHANGE_GUILD_NAME, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}
	
		if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_CHANGE_GUILD_NAME, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if (strnicmp(ptmGuild->m_GuildName, pMsgChangeGuildName->NewGuildName, SIZE_MAX_GUILD_NAME) == 0)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0022);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState != GUILD_STATE_NORMAL)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0023);
			return RES_BREAK;
		}
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	QPARAM_CHANGE_GUILD_NAME *pQParamChangeGuildName = new QPARAM_CHANGE_GUILD_NAME;
	pQParamChangeGuildName->GuildUniqueNumber = m_uidGuildUniqueNumber;
	util::strncpy(pQParamChangeGuildName->NewGuildName, pMsgChangeGuildName->NewGuildName, SIZE_MAX_GUILD_NAME);

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildChangeGuildName, this, pQParamChangeGuildName, NULL, 0, m_uidGuildUniqueNumber);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_GUILD_MARK(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_GET_GUILD_MARK,
									MSG_IC_GUILD_GET_GUILD_MARK, pMsgGetGuildMark);

	if(FALSE == IS_VALID_UNIQUE_NUMBER(pMsgGetGuildMark->GuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_GET_GUILD_MARK, ERR_PROTOCOL_NOT_HAVE_GUILD);		
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_GUILD_MARK_OK, T_IC_GUILD_GET_GUILD_MARK_OK, msgGetMarkOK, msgGetMarkOKBuf);
	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(pMsgGetGuildMark->GuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
// 2005-12-02 by cmkwon, 필요 없음
//			SendErrorMessage(T_IC_GUILD_GET_GUILD_MARK, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}
		if (ptmGuild->m_GuildMark.nGuildMarkSize <= 0
			|| ptmGuild->m_GuildMark.nGuildMarkSize > 1300 // 2007-08-02 by cmkwon, 버퍼 사이즈는 SIZE_MAX_GUILD_MARK_IMAGE이지만 소켓 최대사이즈(SIZE_MAX_PACKET)보다 작아야 하므로 임의로 1300Bytes로 체크함
			|| ptmGuild->m_GuildMark.nGuildMarkVersion == 0)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			return RES_BREAK;
		}
		
		if(GUILD_MARK_STATE_NORMAL != ptmGuild->m_GuildMark.byGuildMarkState)
		{// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 여단마크상태 체크
			ms_pIMIOCP->m_MapGuild.unlock();
			return RES_BREAK;
		}

		msgGetMarkOK->GuildUniqueNumber	= ptmGuild->m_GuildUniqueNumber;
		msgGetMarkOK->GuildMarkVersion	= ptmGuild->m_GuildMark.nGuildMarkVersion;
		msgGetMarkOK->SizeOfGuildMark	= ptmGuild->m_GuildMark.nGuildMarkSize;
		memcpy(msgGetMarkOKBuf+MSG_SIZE(MSG_IC_GUILD_GET_GUILD_MARK_OK), ptmGuild->m_GuildMark.GuildMarkImage, ptmGuild->m_GuildMark.nGuildMarkSize);
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	SendAddData(msgGetMarkOKBuf, MSG_SIZE(MSG_IC_GUILD_GET_GUILD_MARK_OK)+msgGetMarkOK->SizeOfGuildMark);
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_SET_GUILD_MARK(const char *pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_IC_GUILD_SET_GUILD_MARK	*msgSetGuildMark = NULL;

	nRecvTypeSize = sizeof(MSG_IC_GUILD_SET_GUILD_MARK)
					+ ((MSG_IC_GUILD_SET_GUILD_MARK*)(pPacket + nBytesUsed))->SizeOfGuildMark;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_GUILD_SET_GUILD_MARK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_RETURN_FALSE;
	}
	msgSetGuildMark = (MSG_IC_GUILD_SET_GUILD_MARK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize + msgSetGuildMark->SizeOfGuildMark;

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_SET_GUILD_MARK, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_GUILD_MARK, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState != GUILD_STATE_NORMAL)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0024);
			return RES_BREAK;
		}

		if(FALSE == util::in_range(0, msgSetGuildMark->SizeOfGuildMark, 1300))
		{// 2007-08-02 by cmkwon, 버퍼 사이즈는 SIZE_MAX_GUILD_MARK_IMAGE이지만 소켓 최대사이즈(SIZE_MAX_PACKET)보다 작아야 하므로 임의로 1300Bytes로 체크함
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_GUILD_MARK, ERR_PROTOCOL_INVLID_GuildMarkSize);
			return RES_BREAK;
		}

		if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_GUILD_MARK, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 여기에서 할당하지 않고 QP_GuildSetGuildMark()에서 할당하는 것으로 변경한다
		//memcpy(ptmGuild->m_GuildMark.GuildMarkImage, ((char*)msgSetGuildMark)+sizeof(MSG_IC_GUILD_SET_GUILD_MARK), msgSetGuildMark->SizeOfGuildMark);
	}
	ms_pIMIOCP->m_MapGuild.unlock();
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 여기에서 할당하지 않고 QP_GuildSetGuildMark()에서 할당하는 것으로 변경한다
	GUILD_MARK *pGuildMark = new GUILD_MARK;
	pGuildMark->nGuildMarkSize		= msgSetGuildMark->SizeOfGuildMark;
	memcpy(pGuildMark->GuildMarkImage, ((char*)msgSetGuildMark)+sizeof(MSG_IC_GUILD_SET_GUILD_MARK), msgSetGuildMark->SizeOfGuildMark);
	pGuildMark->byGuildMarkState	= GUILD_MARK_STATE_NORMAL;
	if(0 >= msgSetGuildMark->SizeOfGuildMark)
	{
		pGuildMark->byGuildMarkState	= GUILD_MARK_STATE_NONE;
	}
	
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSetGuildMark, this, pGuildMark, NULL, m_uidGuildUniqueNumber);
// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 기본값 허용
//	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSetGuildMark, this, NULL, NULL,
//						m_uidGuildUniqueNumber, msgSetGuildMark->SizeOfGuildMark);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_SET_RANK(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_SET_RANK,
									MSG_IC_GUILD_SET_RANK, pMsgSetRank);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		SendErrorMessage(T_IC_GUILD_SET_RANK, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}
	
	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_RANK, ERR_PROTOCOL_NOT_HAVE_GUILD);
			return RES_BREAK;
		}

		CGuildMember *pMySelfMember = NULL;
		if(!ptmGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMySelfMember))
		{// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 -
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_RANK, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
			return RES_BREAK;
		}

		if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber
			&& GUILD_RANK_SUBCOMMANDER != pMySelfMember->m_Rank			// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 -
			)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_RANK, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if(GUILD_RANK_SUBCOMMANDER == pMySelfMember->m_Rank
			&& (GUILD_RANK_COMMANDER == pMsgSetRank->GuildRank
				|| 	GUILD_RANK_SUBCOMMANDER == pMsgSetRank->GuildRank)
			)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_RANK, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
			return RES_BREAK;
		}

		if (ptmGuild->m_GuildState != GUILD_STATE_NORMAL)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0025);
			return RES_BREAK;
		}

		CGuildMember *pMember = NULL;
		BOOL ret = ptmGuild->GetGuildMember(pMsgSetRank->MemberUniqueNumber, pMember);
		if (!ret)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendErrorMessage(T_IC_GUILD_SET_RANK, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
			return RES_BREAK;
		}

		if (!ptmGuild->CheckRank(pMsgSetRank->MemberUniqueNumber, pMsgSetRank->GuildRank)
			&& GUILD_RANK_SUBCOMMANDER != pMsgSetRank->GuildRank		// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 -
			)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0026);
			//////////////////////////////////////////////////////////////////////////
			// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 - 클라이언트 요청으로 정보 보내준다.
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_IC_GUILD_SET_RANK_OK, msgRank, msgRankBuf);
			msgRank->MemberUniqueNumber	= pMsgSetRank->MemberUniqueNumber;
			msgRank->GuildRank			= pMsgSetRank->GuildRank;
			this->SendAddData(msgRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));
			return RES_BREAK;
		}

		if (pMsgSetRank->GuildRank == GUILD_RANK_COMMANDER)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0027);
			return RES_BREAK;
		}

		if (pMember->m_Rank == GUILD_RANK_COMMANDER)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
			SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0028);
			return RES_BREAK;
		}

		if(GUILD_RANK_SUBCOMMANDER == pMsgSetRank->GuildRank)
		{// 2008-06-17 by dhjin, EP3 - 여단 수정 사항 -
			CGuildMember *pSubCommander = NULL;
			if(ptmGuild->GetGuildMemberByRank(GUILD_RANK_SUBCOMMANDER, pSubCommander))
			{
				pSubCommander->m_Rank	= GUILD_RANK_PRIVATE_NULL;
				ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSetRank, this, NULL
					, (void*)GUILD_RANK_PRIVATE_NULL, pSubCommander->m_CharacterUniqueNumber, m_uidGuildUniqueNumber);
			}
		}
		
		pMember->m_Rank		= pMsgSetRank->GuildRank;
	}
	ms_pIMIOCP->m_MapGuild.unlock();

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSetRank, this, NULL
		, (void*)pMsgSetRank->GuildRank, pMsgSetRank->MemberUniqueNumber, m_uidGuildUniqueNumber);

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_OTHER_GUILD_INFO(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_GET_OTHER_GUILD_INFO,
									MSG_IC_GUILD_GET_OTHER_GUILD_INFO, msgGetOtherGuildInfo);

	ms_pIMIOCP->m_MapGuild.lock();
	{
		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(msgGetOtherGuildInfo->GuildUniqueNumber);
		if(NULL == ptmGuild)
		{
			ms_pIMIOCP->m_MapGuild.unlock();
// 2005-12-02 by cmkwon, 필요없음
//			SendErrorMessage(T_IC_GUILD_GET_OTHER_GUILD_INFO, ERR_PROTOCOL_NO_SUCH_GUILD, msgGetOtherGuildInfo->GuildUniqueNumber);
			return RES_BREAK;
		}

		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK, T_IC_GUILD_GET_OTHER_GUILD_INFO_OK,
							msgGetOK, msgGetOKBuf);
		msgGetOK->OtherGuildMemberInfo = *ptmGuild;
		SendAddData(msgGetOKBuf, MSG_SIZE(MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK));
	}
	ms_pIMIOCP->m_MapGuild.unlock();	

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_GUILD_SURRENDER_GUILD_WAR(const char *pPacket, int nLength, int &nBytesUsed)
{	
	ms_pIMIOCP->GuildWarEnd(BATTLE_END_SURRENDER, this->m_uidGuildUniqueNumber);
	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_NEW_COMMANDER(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단장 위임
/// \author		dhjin
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_NEW_COMMANDER(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_NEW_COMMANDER,
									MSG_IC_GUILD_NEW_COMMANDER, pRMsg);

	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_NEW_COMMANDER, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SendErrorMessage(T_IC_GUILD_NEW_COMMANDER, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}	

	if(this->m_character.CharacterUniqueNumber != pGuild->m_CommanderUniqueNumber)
	{
		SendErrorMessage(T_IC_GUILD_NEW_COMMANDER, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}
	
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildUpdateCommander, this, NULL, (void*)pRMsg->NewCommanderUID);

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_NOTICE_WRITE(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단 공지
/// \author		dhjin
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_NOTICE_WRITE(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_NOTICE_WRITE,
									MSG_IC_GUILD_NOTICE_WRITE, pRMsg);
	
	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_NOTICE_WRITE, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SendErrorMessage(T_IC_GUILD_NOTICE_WRITE, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}
	
	CGuildMember *pMember = NULL;
	if(!pGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
	{
		SendErrorMessage(T_IC_GUILD_NOTICE_WRITE, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
		return RES_BREAK;
	}

	if(this->m_character.CharacterUniqueNumber != pGuild->m_CommanderUniqueNumber
		&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
	{
		SendErrorMessage(T_IC_GUILD_NOTICE_WRITE, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}	

	QPARAM_GUILD_NOTICE *pQParam = new QPARAM_GUILD_NOTICE;
	util::strncpy(pQParam->Notice, pRMsg->Notice, SIZE_MAX_NOTICE);

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildNotice, this, pQParam);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_APPLICANT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 지원자 관리
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_APPLICANT(const char* pPacket, int nLength, int &nBytesUsed)
{
	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_GET_APPLICANT, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SendErrorMessage(T_IC_GUILD_GET_APPLICANT, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	CGuildMember *pMember = NULL;
	if(!pGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
	{
		SendErrorMessage(T_IC_GUILD_GET_APPLICANT, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
		return RES_BREAK;
	}

	if(this->m_character.CharacterUniqueNumber != pGuild->m_CommanderUniqueNumber
		&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
	{
		SendErrorMessage(T_IC_GUILD_GET_APPLICANT, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}	
	
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildGetApplicant, this, NULL);
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_INTRODUCTION(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_INTRODUCTION(const char* pPacket, int nLength, int &nBytesUsed)
{
	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_GET_INTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SendErrorMessage(T_IC_GUILD_GET_INTRODUCTION, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}
	
	CGuildMember *pMember = NULL;
	if(!pGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
	{
		SendErrorMessage(T_IC_GUILD_GET_INTRODUCTION, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
		return RES_BREAK;
	}

	if(this->m_character.CharacterUniqueNumber != pGuild->m_CommanderUniqueNumber
		&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
	{
		SendErrorMessage(T_IC_GUILD_GET_INTRODUCTION, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}	
	
	pGuild->CheckOldIntroductionContent();

	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_INTRODUCTION_OK, T_IC_GUILD_GET_INTRODUCTION_OK, pMsg, SendBuf);
	util::strncpy(pMsg->GuildIntroduction, pGuild->GetGuildIntroduction(), SIZE_MAX_NOTICE);
	this->SendAddData(SendBuf, MSG_SIZE(MSG_IC_GUILD_GET_INTRODUCTION_OK));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_SELF_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단 지원자 소개서 
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_GET_SELF_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_GET_SELF_INTRODUCTION,
									MSG_IC_GUILD_GET_SELF_INTRODUCTION, pRMsg);

	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_GET_SELF_INTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GetSelfIntroduction, this, NULL, (void*)pRMsg->CharacterUID);

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult Process_IC_GUILD_SEARCH_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개 검색
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_SEARCH_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
{
	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_SEARCH_INTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildSearchIntroduction, this, NULL);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_UPDATE_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개 작성 
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_UPDATE_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_UPDATE_INTRODUCTION,
									MSG_IC_GUILD_UPDATE_INTRODUCTION, pRMsg);

	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_UPDATE_INTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SendErrorMessage(T_IC_GUILD_UPDATE_INTRODUCTION, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}
	
	CGuildMember *pMember = NULL;
	if(!pGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
	{
		SendErrorMessage(T_IC_GUILD_UPDATE_INTRODUCTION, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
		return RES_BREAK;
	}

	if(this->m_character.CharacterUniqueNumber != pGuild->m_CommanderUniqueNumber
		&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
	{
		SendErrorMessage(T_IC_GUILD_UPDATE_INTRODUCTION, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}	

	SGUILD_INTRODUCTION	NewGuldIntroduction;
	util::zero(&NewGuldIntroduction, sizeof(SGUILD_INTRODUCTION));
	util::strncpy(NewGuldIntroduction.IntroductionContent, pRMsg->GuildIntroduction, SIZE_MAX_NOTICE);
	NewGuldIntroduction.RegDate.SetCurrentDateTime();
	
	pGuild->SetGuildIntroduction(&NewGuldIntroduction);
	
	QPARAM_GUILD_UPDATE_INTRODUCTION *pQParam = new QPARAM_GUILD_UPDATE_INTRODUCTION;
	util::strncpy(pQParam->GuildIntroduction, pRMsg->GuildIntroduction, SIZE_MAX_NOTICE);
	pQParam->WriteDate	= NewGuldIntroduction.RegDate;
	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildUpdateIntroduction, this, pQParam);

	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_DELETE_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개 지우기
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_DELETE_INTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
{
	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_DELETE_INTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
		
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);
	CGuild *pGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SendErrorMessage(T_IC_GUILD_DELETE_INTRODUCTION, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}
	
	CGuildMember *pMember = NULL;
	if(!pGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
	{
		SendErrorMessage(T_IC_GUILD_DELETE_INTRODUCTION, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
		return RES_BREAK;
	}

	if(this->m_character.CharacterUniqueNumber != pGuild->m_CommanderUniqueNumber
		&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
	{
		SendErrorMessage(T_IC_GUILD_DELETE_INTRODUCTION, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}	

	pGuild->InitGuildIntroduction();

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteIntroduction, NULL, NULL, (void*)pGuild->m_GuildUniqueNumber);

	this->SendAddMessageType(T_IC_GUILD_DELETE_INTRODUCTION_OK);
	return RES_RETURN_TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_UPDATE_SELFINTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 자기 소개 작성 
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_UPDATE_SELFINTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_UPDATE_SELFINTRODUCTION,
									MSG_IC_GUILD_UPDATE_SELFINTRODUCTION, pRMsg);

	if(NULL == this
		|| !this->IsValidCharacter()
		|| IS_VALID_UNIQUE_NUMBER(this->m_character.GuildUniqueNumber)
		|| COMPARE_INFLUENCE(m_character.InfluenceType, INFLUENCE_TYPE_NORMAL)
		)
	{
		SendErrorMessage(T_IC_GUILD_UPDATE_SELFINTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	QPARAM_GUILD_UPDATE_SELFINTRODUCTION *pQParam = new QPARAM_GUILD_UPDATE_SELFINTRODUCTION;
	pQParam->TargetGuildUID		= pRMsg->TargetGuildUID;
	util::strncpy(pQParam->SelfIntroduction, pRMsg->SelfIntroduction, SIZE_MAX_NOTICE);

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildUpdateSelfIntroduction, this, pQParam);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_DELETE_SELFINTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 자기 소개 지우기  
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_DELETE_SELFINTRODUCTION(const char *pPacket, int nLength, int &nBytesUsed)
{
	if(NULL == this
		|| !this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_GUILD_DELETE_SELFINTRODUCTION, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteSelfIntroduction, this, NULL);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_APPLICANT_INVITE(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대
/// \author		dhjin
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_APPLICANT_INVITE(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_APPLICANT_INVITE,
									MSG_IC_GUILD_APPLICANT_INVITE, pRMsg);
	
	mt_auto_lock mtG(&ms_pIMIOCP->m_MapGuild);

	CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(this->m_character.GuildUniqueNumber);
	if(NULL == ptmGuild)
	{
		SendErrorMessage(T_IC_GUILD_APPLICANT_INVITE, ERR_PROTOCOL_NOT_HAVE_GUILD);
		return RES_BREAK;
	}

	CGuildMember *pMember = NULL;
	if(!ptmGuild->GetGuildMember(this->m_character.CharacterUniqueNumber, pMember))
	{
		SendErrorMessage(T_IC_GUILD_APPLICANT_INVITE, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
		return RES_BREAK;
	}

	if(this->m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber
		&& GUILD_RANK_SUBCOMMANDER != pMember->m_Rank)
	{
		SendErrorMessage(T_IC_GUILD_APPLICANT_INVITE, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
		return RES_BREAK;
	}

	if (ptmGuild->m_GuildState != GUILD_STATE_NORMAL)
	{
		SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0016);
		return RES_BREAK;
	}

	CGuildMember *pInviteMember = NULL;
	if(ptmGuild->GetGuildMember(pRMsg->CharacterUID, pInviteMember))
	{
		SendErrorMessage(T_IC_GUILD_APPLICANT_INVITE, ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS);
		return RES_BREAK;
	}
	
	if (CAtumSJ::GetMaxGuildSize(ptmGuild->m_nGuildMemberCapacity, this->IsExistPremiumCard()) <= ptmGuild->m_ListGuildMember.size())		// 2008-05-28 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	{
		SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0015);
		return RES_BREAK;
	}

	CIMIOCPSocket * pIISock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRMsg->CharacterUID);
	if(NULL != pIISock)
	{
		MSG_IC_GUILD_ACCEPT_INVITE	*pMsgAccept = new MSG_IC_GUILD_ACCEPT_INVITE;
		pMsgAccept->GuildUniqueNumber		= ptmGuild->m_GuildUniqueNumber;
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildAddMember, pIISock, pMsgAccept);
	}
	else
	{
		MSG_IC_GUILD_APPLICANT_INVITE	* pMsgAccept = new MSG_IC_GUILD_APPLICANT_INVITE;
		*pMsgAccept	= *pRMsg;
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildAddOffMember, this, pMsgAccept);		
	}
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_GUILD_APPLICANT_REJECT_INVITE(const char *pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 - 여단 수정 사항 - 지원자 관리에서 여단원 초대 거부 
/// \author		dhjin
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_GUILD_APPLICANT_REJECT_INVITE(const char *pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_GUILD_APPLICANT_REJECT_INVITE,
									MSG_IC_GUILD_APPLICANT_REJECT_INVITE, pRMsg);

	CIMIOCPSocket * pIISock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pRMsg->CharacterUID);
	if(NULL != pIISock)
	{
		pIISock->SendString128(STRING_128_USER_ERR, STRMSG_080613_0001, pRMsg->GuildName);
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteSelfIntroduction, pIISock, NULL);
	}
	else
	{
		char NotifyMsgString[SIZE_MAX_NOTIFY_MSG_STRING];
		sprintf(NotifyMsgString, STRMSG_080613_0001, pRMsg->GuildName);
		SNOTIFY_MSG *pQParam = new SNOTIFY_MSG;
		util::zero(pQParam, sizeof(SNOTIFY_MSG));
		pQParam->CharacterUID	= pRMsg->CharacterUID;
		pQParam->NotifyMsgType	= NOTIFY_MSG_TYPE_TEXT;
		util::strncpy(pQParam->NotifyMsgString, NotifyMsgString,SIZE_MAX_NOTIFY_MSG_STRING);
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_InsertNotifyMsg, NULL, pQParam);
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildDeleteSelfIntroductionOffUser, NULL, NULL, (void*)pRMsg->CharacterUID);		
	}
	
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_ADMIN_GET_CHARACTER_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_ADMIN_GET_CHARACTER_INFO_OK Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int									nRecvTypeSize = 0;
	MSG_FI_ADMIN_GET_CHARACTER_INFO_OK	*pMsgGetOK = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_GET_CHARACTER_INFO_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_GET_CHARACTER_INFO_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1201C);
		return RES_RETURN_FALSE;
	}
	pMsgGetOK = (MSG_FI_ADMIN_GET_CHARACTER_INFO_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CIMIOCPSocket *pAdminSock = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsgGetOK->AdminCharacterUniqueNumber);
	if (NULL == pAdminSock || FALSE == pAdminSock->IsUsing())
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	if (pMsgGetOK->Type == ADMIN_GET_CHARAC_TYPE_SEND)
	{
		pAdminSock->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0050,
									pMsgGetOK->CharacterName,
									pMsgGetOK->AccountName,
									pMsgGetOK->MapChannelIndex.MapIndex,
									pMsgGetOK->MapChannelIndex.ChannelIndex,
									pMsgGetOK->Level);
	}
	else if (pMsgGetOK->Type == ADMIN_GET_CHARAC_TYPE_MOVETO)
	{
		INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_MOVETO_CHARACTER, T_FI_ADMIN_MOVETO_CHARACTER, pMsgAdminMoveTo, pMsgAdminMoveToBuf);
		pMsgAdminMoveTo->AdminCharacterUniqueNumber = pMsgGetOK->AdminCharacterUniqueNumber;
		pMsgAdminMoveTo->MapChannelIndex = pMsgGetOK->MapChannelIndex;
		pMsgAdminMoveTo->Position = pMsgGetOK->Position;

		if (FALSE == pAdminSock->SendMsgToCurrentFieldServerSocket(pMsgAdminMoveToBuf, MSG_SIZE(MSG_FI_ADMIN_MOVETO_CHARACTER)))
		{
			// current field server socket이 NULL이면 무시.
			char	szSystemLog[1024];
			sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL. at %s\r\n",
				GetProtocolTypeString(T_FI_ADMIN_GET_CHARACTER_INFO_OK));
			DBGOUT(szSystemLog);
			g_pGlobal->WriteSystemLog(szSystemLog);

			return RES_BREAK;
		}
	}
	else
	{
		// do nothing
	}


	return RES_RETURN_TRUE;
}

BOOL CIMIOCPSocket::SendGuildInfoToFieldServer(UID32_t i_nGuildUniqueNumber, const char *i_szGuildName)
{
	if (FALSE == this->IsUsing())
	{
		return FALSE;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CHARACTER_UPDATE_GUILD_INFO, T_FI_CHARACTER_UPDATE_GUILD_INFO, msgUpdateGuild, msgUpdateGuildBuf);
	msgUpdateGuild->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
	msgUpdateGuild->GuildUniqueNumber		= i_nGuildUniqueNumber;
	if (NULL == i_szGuildName)
	{
		util::zero(msgUpdateGuild->GuildName, SIZE_MAX_GUILD_NAME);
	}
	else
	{
		util::strncpy(msgUpdateGuild->GuildName, i_szGuildName, SIZE_MAX_GUILD_NAME);
	}

	this->SendMsgToCurrentFieldServerSocket(msgUpdateGuildBuf, MSG_SIZE(MSG_FI_CHARACTER_UPDATE_GUILD_INFO));
	return TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_ADMIN_CALL_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_IC_ADMIN_CALL_CHARACTER	*pMsgCallCharac = NULL;

	nRecvTypeSize = sizeof(MSG_IC_ADMIN_CALL_CHARACTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_ADMIN_CALL_CHARACTER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1201C);
		return RES_RETURN_FALSE;
	}
	pMsgCallCharac = (MSG_IC_ADMIN_CALL_CHARACTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (!COMPARE_RACE(m_character.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		// 운영자용, 일반 사용자는 무시
		return RES_BREAK;
	}

	// 자기 자신이면 무시
	if ( strnicmp(m_character.CharacterName, pMsgCallCharac->CharacterName, SIZE_MAX_CHARACTER_NAME) == 0 )
	{
		SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0051);
		return RES_BREAK;
	}

	CIMIOCPSocket *pCharacSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pMsgCallCharac->CharacterName);
	if (pCharacSocket == NULL)
	{
		SendString128(STRING_128_ADMIN_CMD, "%s NOT PLAYING", pMsgCallCharac->CharacterName);
	}
	else
	{
		INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_CALL_CHARACTER, T_FI_ADMIN_CALL_CHARACTER, pMsgFICall, pMsgFICallBuf);
		pMsgFICall->CharacterUniqueNumber = pCharacSocket->m_character.CharacterUniqueNumber;
		pMsgFICall->MapChannelIndex = pMsgCallCharac->MapChannelIndex;
		pMsgFICall->Position = pMsgCallCharac->Position;

		if (FALSE == pCharacSocket->SendMsgToCurrentFieldServerSocket(pMsgFICallBuf, MSG_SIZE(MSG_FI_ADMIN_CALL_CHARACTER)))
		{
			// current field server socket이 NULL이면 무시.
			char	szSystemLog[1024];
			sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
				GetCharacterString(&pCharacSocket->m_character, string()), GetProtocolTypeString(T_IC_ADMIN_CALL_CHARACTER));
			DBGOUT(szSystemLog);
			g_pGlobal->WriteSystemLog(szSystemLog);

			return RES_BREAK;
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_ADMIN_CALL_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_IC_ADMIN_CALL_GUILD	*pMsgCallGuild = NULL;

	nRecvTypeSize = sizeof(MSG_IC_ADMIN_CALL_GUILD);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_ADMIN_CALL_GUILD, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1201C);
		return RES_RETURN_FALSE;
	}
	pMsgCallGuild = (MSG_IC_ADMIN_CALL_GUILD*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	// format: "/여단오기 [여단명]" - 해당 캐릭터를 불러옴
	if (!COMPARE_RACE(m_character.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		// 운영자용, 일반 사용자는 무시
		return RES_BREAK;
	}

	ms_pIMIOCP->WarpGulid(pMsgCallGuild->GuildName, pMsgCallGuild->MapChannelIndex, A2DX(pMsgCallGuild->Position));
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IC_ADMIN_GET_SERVER_STAT(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (!COMPARE_RACE(m_character.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_GET_SERVER_STAT_OK, T_IC_ADMIN_GET_SERVER_STAT_OK, pStatOK, pStatOKBuf);
	pStatOK->TotalUserCount = ms_pIMIOCP->m_MapChactarterUniqueNumber.size();
	mt_map<UID32_t, CIMIOCPSocket*>::iterator it;
	int iBCUCount = 0;
	int iANICount = 0;
	for (it = ms_pIMIOCP->m_MapChactarterUniqueNumber.begin(); it != ms_pIMIOCP->m_MapChactarterUniqueNumber.end(); it++)
	{
		if (COMPARE_INFLUENCE(it->second->m_character.InfluenceType, INFLUENCE_TYPE_VCN))
		{
			iBCUCount++;
		}
		else if (COMPARE_INFLUENCE(it->second->m_character.InfluenceType, INFLUENCE_TYPE_ANI))
		{
			iANICount++;
		}
	}

	pStatOK->ANIUSerCount = iANICount;
	pStatOK->BCUUserCount = iBCUCount;

	SendAddData(pStatOKBuf, MSG_SIZE(MSG_IC_ADMIN_GET_SERVER_STAT_OK));

	return RES_RETURN_TRUE;
}


ProcessResult CIMIOCPSocket::Process_IC_COUNTDOWN_DONE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int			nRecvTypeSize = 0;
	MSG_IC_COUNTDOWN_DONE	*pRecvMsg = NULL;

	nRecvTypeSize = sizeof(MSG_IC_COUNTDOWN_DONE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_IC_COUNTDOWN_DONE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1201C);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_IC_COUNTDOWN_DONE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	///////////////////////////////////////////////////////////////////////////////
	// 시간이 경과 되었는지 체크(향후 추가해야함)
	// ERR_COUNTDOWN_NOT_ENOUGH_TIME
	if (FALSE == m_countdownChecker.IsEndedCountDown(pRecvMsg->byCountdownType, timeGetTime()))
	{
		SendErrorMessage(T_IC_COUNTDOWN_DONE, ERR_COUNTDOWN_NOT_ENOUGH_TIME, pRecvMsg->byCountdownType);
		
		INIT_MSG_WITH_BUFFER(MSG_IC_COUNTDOWN_START, T_IC_COUNTDOWN_START, pSMsg, pSendBuf);
		pSMsg->byCountdownType	= pRecvMsg->byCountdownType;
		pSMsg->nWaitingSecond	= SECOND_COUNTDOWN_TIME_GAP/2;
		this->SendAddData(pSendBuf, MSG_SIZE(MSG_IC_COUNTDOWN_START));
		return RES_BREAK;
	}

	switch(pRecvMsg->byCountdownType)
	{
	case COUNTDOWN_TYPE_GUILD_WAR:
		{
			if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
			{
				SendErrorMessage(T_IC_COUNTDOWN_DONE, ERR_PROTOCOL_NOT_HAVE_GUILD);
				return RES_BREAK;
			}

			///////////////////////////////////////////////////////////////////////////////
			// 클라이언트로 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_START_WAR, T_IC_GUILD_START_WAR, pMsgStart, pMsgStartBuf);
			ms_pIMIOCP->m_MapGuild.lock();
			{
				CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
				if(NULL == ptmGuild)
				{
					ms_pIMIOCP->m_MapGuild.unlock();
					SendErrorMessage(T_IC_COUNTDOWN_DONE, ERR_PROTOCOL_NOT_HAVE_GUILD);
					return RES_BREAK;
				}

				pMsgStart->PeerGuildUID = ptmGuild->m_uidWarPeerGuildUID;
			}
			ms_pIMIOCP->m_MapGuild.unlock();			

			this->SendAddData(pMsgStartBuf, MSG_SIZE(MSG_IC_GUILD_START_WAR));
		}
		break;
	default:
		{
			SendErrorMessage(T_IC_COUNTDOWN_DONE, ERR_COUNTDOWN_INVALID_COUNTDOWN_TYPE, pRecvMsg->byCountdownType);
			return RES_BREAK;
		}
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_VOIP_SET(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 VOIP - 설정
/// \author		dhjin
/// \date		2008-06-17 ~ 2008-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_SET(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_VOIP_SET,
//									MSG_IC_VOIP_SET, pRMsg);
//
//	if(!this->IsValidCharacter())
//	{
//		SendErrorMessage(T_IC_VOIP_SET, ERR_INVALID_CHARACTER);
//		return RES_BREAK;
//	}
//
//	this->SendVoipType(pRMsg->VoipType);
//
//	m_VoipType	= pRMsg->VoipType;
//
//	return RES_RETURN_TRUE;
//}

//////////////////////////////////////////////////////////////////////////
// 2008-06-16 by dhjin, EP3 채팅방 -
///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CREATE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 생성
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CREATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_CREATE,
									MSG_IC_CHATROOM_CREATE, pRMsg);
	
	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_CREATE, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	Err_t errCode =  ms_pIMIOCP->m_ChatRoomManager.CreateChatRoom(this, pRMsg);
	if(ERR_NO_ERROR != errCode)
	{
		SendErrorMessage(T_IC_CHATROOM_CREATE, errCode);
		return RES_BREAK;
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_LIST_INFO(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 목록 가져오기		
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_LIST_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_LIST_INFO,
									MSG_IC_CHATROOM_LIST_INFO, pRMsg);
	
	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_LIST_INFO, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}
	
	this->SendChatRoomListInfo(pRMsg->StartNum);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 초대 요청 	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_REQUEST_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_REQUEST_INVITE,
									MSG_IC_CHATROOM_REQUEST_INVITE, pMsgInvite);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	CIMIOCPSocket *pInviteeSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pMsgInvite->InviteeCharacterName);
	if (NULL == pInviteeSocket
		|| FALSE == pInviteeSocket->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_COMMON_NO_SUCH_CHARACTER);
		return RES_BREAK;
	}
	
	if(this->m_character.InfluenceType != pInviteeSocket->m_character.InfluenceType)
	{
		SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_PROTOCOL_CANNOT_INVITE_USER);
		return RES_BREAK;
	}

	if (strnicmp(pMsgInvite->InviteeCharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME) == 0)
	{
		SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_PROTOCOL_CANNOT_INVITE_USER);
		SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0013);
		return RES_BREAK;
	}

	if(pInviteeSocket->m_bChatRoomUseing)
	{
		SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_CHATROOM_ALREADY_USING);
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION, T_IC_CHATROOM_REQUEST_INVITE_QUESTION, msgInviteQuestion, SendBuf);

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsgInvite->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber != (*itr)->GetChatRoomMasterCharacterUID())
			{
				SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_CHATROOM_PERMISSION_DENIED);
				return RES_BREAK;
			}
			
			if((*itr)->GetChatRoomMemberCount() >= (*itr)->GetChatRoomMaxMemberCount())
			{
				SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_CHATROOM_FULL_MEMBER);
				return RES_BREAK;
			}

			if(this->IsUsing() && pInviteeSocket->IsUsing())
			{
				msgInviteQuestion->ChatRoomNum	= pMsgInvite->ChatRoomNum;
				util::strncpy(msgInviteQuestion->ChatRoomMasterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
				pInviteeSocket->SendAddData(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION));
				return RES_RETURN_TRUE;
			}		
		}
	}

	SendErrorMessage(T_IC_CHATROOM_REQUEST_INVITE, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_JOIN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 참여	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_JOIN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_JOIN,
									MSG_IC_CHATROOM_JOIN, pMsgJoin);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_JOIN, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(this->m_bChatRoomUseing)
	{
		SendErrorMessage(T_IC_CHATROOM_JOIN, ERR_CHATROOM_ALREADY_USING);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_JOIN, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsgJoin->ChatRoomNum == (*itr)->GetChatRoomNum())
		{	
			if((*itr)->GetChatRoomLock())
			{
				if(strcmp(pMsgJoin->ChatRoomPW ,(*itr)->GetChatRoomPW()))
				{
					SendErrorMessage(T_IC_CHATROOM_JOIN, ERR_NOT_MATCH_PW);
					return RES_BREAK;
				}
			}

			if((*itr)->GetChatRoomMemberCount() >= (*itr)->GetChatRoomMaxMemberCount())
			{
				SendErrorMessage(T_IC_CHATROOM_JOIN, ERR_CHATROOM_FULL_MEMBER);
				return RES_BREAK;
			}
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_ACCEPT_INVITE_OK, T_IC_CHATROOM_ACCEPT_INVITE_OK, msg, SendBuf);
			util::strncpy(msg->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			msg->CharacterUID	= this->m_character.CharacterUniqueNumber;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_ACCEPT_INVITE_OK));				
			(*itr)->InsertNewMember(this);
			this->m_bChatRoomUseing = TRUE;
			this->m_nChatRoomNum	= pMsgJoin->ChatRoomNum;
			
			(*itr)->SendChatRoomMemberInfo(this);

			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_JOIN, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 초대 수락	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_ACCEPT_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_ACCEPT_INVITE,
									MSG_IC_CHATROOM_ACCEPT_INVITE, pMsgInvite);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_ACCEPT_INVITE, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(this->m_bChatRoomUseing)
	{
		SendErrorMessage(T_IC_CHATROOM_ACCEPT_INVITE, ERR_CHATROOM_ALREADY_USING);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_ACCEPT_INVITE, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsgInvite->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if((*itr)->GetChatRoomMemberCount() >= (*itr)->GetChatRoomMaxMemberCount())
			{
				SendErrorMessage(T_IC_CHATROOM_ACCEPT_INVITE, ERR_CHATROOM_FULL_MEMBER);
				return RES_BREAK;
			}
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_ACCEPT_INVITE_OK, T_IC_CHATROOM_ACCEPT_INVITE_OK, msg, SendBuf);
			util::strncpy(msg->CharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			msg->CharacterUID	= this->m_character.CharacterUniqueNumber;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_ACCEPT_INVITE_OK));				
			(*itr)->InsertNewMember(this);
			this->m_bChatRoomUseing = TRUE;
			this->m_nChatRoomNum	= pMsgInvite->ChatRoomNum;
			
			(*itr)->SendChatRoomMemberInfo(this);
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_ACCEPT_INVITE, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 초대 거절	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_REJECT_INVITE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_REJECT_INVITE,
									MSG_IC_CHATROOM_REJECT_INVITE, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_REJECT_INVITE, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			CIMIOCPSocket * pMasterIISoc = (*itr)->GetMasterIISoc();
			if(NULL != pMasterIISoc)
			{
				pMasterIISoc->SendAddMessageType(T_IC_CHATROOM_REJECT_INVITE_OK);
			}
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_ACCEPT_INVITE, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_LEAVE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 나가기	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_LEAVE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_LEAVE,
									MSG_IC_CHATROOM_LEAVE, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_LEAVE, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(!this->m_bChatRoomUseing
		|| this->m_nChatRoomNum != pMsg->ChatRoomNum)
	{
		SendErrorMessage(T_IC_CHATROOM_LEAVE, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_LEAVE, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}
	
	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber == (*itr)->GetChatRoomMasterCharacterUID()
				&& 1 < (*itr)->GetChatRoomMemberCount())
			{
				INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CHANGE_MASTER_OK, T_IC_CHATROOM_CHANGE_MASTER_OK, msg, SendBuf);
				util::strncpy(msg->CharacterName, (*itr)->ChangeChatRoomMaster(), SIZE_MAX_CHARACTER_NAME);
				if(NULL == msg->CharacterName)
				{
					SendErrorMessage(T_IC_CHATROOM_LEAVE, ERR_CHATROOM_NO_SUCH_NEW_MASTER);
					return RES_BREAK;
				}
				msg->NewCharacterUID = (*itr)->GetChatRoomMasterCharacterUID();
				(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CHANGE_MASTER_OK));
			}
			
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_LEAVE_OK, T_IC_CHATROOM_LEAVE_OK, msg, SendBuf);
			util::strncpy(msg->LeaveCharacterName, pMsg->LeaveCharacterName, SIZE_MAX_CHARACTER_NAME);
			msg->CharacterUID = this->m_character.CharacterUniqueNumber;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_LEAVE_OK));

			if((*itr)->LeaveMemberAndCheckEmpty(this->m_character.CharacterUniqueNumber))
			{
				ms_pIMIOCP->m_ChatRoomManager.SetReserveChatNum((*itr)->GetChatRoomNum());
				util::del(*itr);
				itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.erase(itr);
			}
					
			this->m_bChatRoomUseing = FALSE;
			this->m_nChatRoomNum	= 0;		// 2008-10-22 by cmkwon, 채팅방 버그 수정 - 

			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_LEAVE, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_BAN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 추방	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_BAN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_BAN,
									MSG_IC_CHATROOM_BAN, pMsg);

	if(!this->IsValidCharacter()
		|| this->m_character.CharacterUniqueNumber == pMsg->BanCharacterUID)
	{
		SendErrorMessage(T_IC_CHATROOM_BAN, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(!this->m_bChatRoomUseing
		|| this->m_nChatRoomNum != pMsg->ChatRoomNum)
	{
		SendErrorMessage(T_IC_CHATROOM_BAN, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_BAN, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber != (*itr)->GetChatRoomMasterCharacterUID())
			{
				SendErrorMessage(T_IC_CHATROOM_BAN, ERR_CHATROOM_PERMISSION_DENIED);
				return RES_BREAK;
			}

			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_BAN_OK, T_IC_CHATROOM_BAN_OK, msg, SendBuf);
			util::strncpy(msg->BanCharacterName, pMsg->BanCharacterName, SIZE_MAX_CHARACTER_NAME);
			msg->BanCharacterUID = pMsg->BanCharacterUID;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_BAN_OK));				
			if((*itr)->LeaveMemberAndCheckEmpty(pMsg->BanCharacterUID))
			{
				ms_pIMIOCP->m_ChatRoomManager.SetReserveChatNum((*itr)->GetChatRoomNum());
				util::del(*itr);
				itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.erase(itr);
			}

			CIMIOCPSocket * pBanFISoc = ms_pIMIOCP->GetIMIOCPSocketByCharacterUID(pMsg->BanCharacterUID);
			if(NULL != pBanFISoc)
			{
				pBanFISoc->m_bChatRoomUseing	= FALSE;
				pBanFISoc->m_nChatRoomNum		= 0;		// 2008-10-22 by cmkwon, 채팅방 버그 수정 - 
			}
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_BAN, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_NAME(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 이름 변경	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_NAME(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_CHANGE_NAME,
									MSG_IC_CHATROOM_CHANGE_NAME, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_NAME, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(!this->m_bChatRoomUseing
		|| this->m_nChatRoomNum != pMsg->ChatRoomNum)
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_NAME, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_NAME, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber != (*itr)->GetChatRoomMasterCharacterUID())
			{
				SendErrorMessage(T_IC_CHATROOM_CHANGE_NAME, ERR_CHATROOM_PERMISSION_DENIED);
				return RES_BREAK;
			}
			
			(*itr)->UpdateChatRoomName(pMsg->ChatRoomName);
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CHANGE_NAME_OK, T_IC_CHATROOM_CHANGE_NAME_OK, msg, SendBuf);
			util::strncpy(msg->ChatRoomName, pMsg->ChatRoomName, SIZE_MAX_CHATROOM_NAME);
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CHANGE_NAME_OK));				
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_CHANGE_NAME, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_MASTER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 방장 변경	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_MASTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_CHANGE_MASTER,
									MSG_IC_CHATROOM_CHANGE_MASTER, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_MASTER, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(!this->m_bChatRoomUseing
		|| this->m_nChatRoomNum != pMsg->ChatRoomNum)
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_MASTER, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_MASTER, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}
 
	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber != (*itr)->GetChatRoomMasterCharacterUID())
			{
				SendErrorMessage(T_IC_CHATROOM_CHANGE_MASTER, ERR_CHATROOM_PERMISSION_DENIED);
				return RES_BREAK;
			}
			
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CHANGE_MASTER_OK, T_IC_CHATROOM_CHANGE_MASTER_OK, msg, SendBuf);
			util::strncpy(msg->CharacterName, (*itr)->ChangeChatRoomMaster(pMsg->NewCharacterUID), SIZE_MAX_CHARACTER_NAME);
			if(NULL == msg->CharacterName)
			{
				SendErrorMessage(T_IC_CHATROOM_CHANGE_MASTER, ERR_CHATROOM_NO_SUCH_NEW_MASTER);
				return RES_BREAK;
			}
			msg->NewCharacterUID = pMsg->NewCharacterUID;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CHANGE_MASTER_OK));				
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_CHANGE_MASTER, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_LOCK_PW(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 비밀번호 변경	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_LOCK_PW(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_CHANGE_LOCK_PW,
									MSG_IC_CHATROOM_CHANGE_LOCK_PW, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_LOCK_PW, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(!this->m_bChatRoomUseing
		|| this->m_nChatRoomNum != pMsg->ChatRoomNum)
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_LOCK_PW, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_LOCK_PW, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber != (*itr)->GetChatRoomMasterCharacterUID())
			{
				SendErrorMessage(T_IC_CHATROOM_CHANGE_LOCK_PW, ERR_CHATROOM_PERMISSION_DENIED);
				return RES_BREAK;
			}
			
			(*itr)->UpdateChatRoomLock(pMsg->ChatRoomLock, pMsg->ChatRoomPW);
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK, T_IC_CHATROOM_CHANGE_LOCK_PW_OK, msg, SendBuf);
			msg->ChatRoomLock		= pMsg->ChatRoomLock;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK));				
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_CHANGE_LOCK_PW, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_MAX_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 인원수 변경	
/// \author		dhjin
/// \date		2008-06-16 ~ 2008-06-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_CHANGE_MAX_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_CHANGE_MAX_MEMBER,
									MSG_IC_CHATROOM_CHANGE_MAX_MEMBER, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_MAX_MEMBER, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	if(!this->m_bChatRoomUseing
		|| this->m_nChatRoomNum != pMsg->ChatRoomNum)
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_MAX_MEMBER, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_CHANGE_MAX_MEMBER, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			if(this->m_character.CharacterUniqueNumber != (*itr)->GetChatRoomMasterCharacterUID())
			{
				SendErrorMessage(T_IC_CHATROOM_CHANGE_MAX_MEMBER, ERR_CHATROOM_PERMISSION_DENIED);
				return RES_BREAK;
			}
			
			(*itr)->UpdateChatRoomMaxMemberCount(pMsg->ChatRoomMaxMember);
			INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK, T_IC_CHATROOM_CHANGE_MAX_MEMBER_OK, msg, SendBuf);
			msg->ChatRoomMaxMember		= pMsg->ChatRoomMaxMember;
			(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK));				
			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_CHANGE_MAX_MEMBER, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_OTHER_MEMBER_INFO(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 채팅방 - 채팅방 맴버 정보 전송
/// \author		dhjin
/// \date		2008-06-25 ~ 2008-06-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_IC_CHATROOM_OTHER_MEMBER_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_IC_CHATROOM_OTHER_MEMBER_INFO,
									MSG_IC_CHATROOM_MEMBER_INFO, pMsg);

	if(!this->IsValidCharacter())
	{
		SendErrorMessage(T_IC_CHATROOM_OTHER_MEMBER_INFO, ERR_INVALID_CHARACTER);
		return RES_BREAK;
	}

	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		SendErrorMessage(T_IC_CHATROOM_OTHER_MEMBER_INFO, ERR_CHATROOM_NO_SUCH);
		return RES_BREAK;
	}

	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(pMsg->ChatRoomNum == (*itr)->GetChatRoomNum())
		{
			(*itr)->SendOtherChatRoomMemberInfo(this);			
			return RES_RETURN_TRUE;
		}
	}

	SendErrorMessage(T_IC_CHATROOM_MEMBER_INFO, ERR_CHATROOM_NO_SUCH);
	return RES_BREAK;
}

// 2016-07-01 panoskj let's remove VOIP !!! HAYO
// VoIP 관련
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_GET_ACCOUNTUNIQUENUMBER(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER	*pRecvMsg = NULL;
//
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_GET_ACCOUNTUNIQUENUMBER, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CIMIOCPSocket *pCalledSock = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(pRecvMsg->szCalledCharacterName);
//	if (NULL == pCalledSock)
//	{
//		INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_ERROR, T_IC_VOIP_ERROR, pSendErr, tmSendBuf);
//		pSendErr->nErrorNum					= VOIPERR_1to1_NO_SUCH_CHARACTER;
//		pSendErr->nSend2AccountUniqueNumber	= this->m_character.AccountUniqueNumber;
//		SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_ERROR));
//		return RES_BREAK;
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK, T_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK, pSendMsg, tmSendBuf);
//	pSendMsg->nCalledAccountUniqueNumber = pCalledSock->m_character.AccountUniqueNumber;
//	SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_GET_ACCOUNTUNIQUENUMBER_OK));
//	
//	return RES_RETURN_TRUE;	
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_1to1_DIAL(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_1to1_DIAL		*pRecvMsg = NULL;
//
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_1to1_DIAL);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_1to1_DIAL, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_1to1_DIAL*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CIMIOCPSocket *pCalledSock = ms_pIMIOCP->GetIMIOCPSocketByAccountUID(pRecvMsg->nCalledAccountUniqueNumber);
//	if (NULL == pCalledSock)
//	{
//		INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_ERROR, T_IC_VOIP_ERROR, pSendErr, tmSendBuf);
//		pSendErr->nErrorNum					= VOIPERR_1to1_NO_SUCH_ACCOUNTUNIQUENUMBER;
//		pSendErr->nSend2AccountUniqueNumber	= this->m_character.AccountUniqueNumber;
//		SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_ERROR));
//
//		return RES_BREAK;
//	}
//
//// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
////	if (FALSE == COMPARE_CHATTYPE_BIT(pCalledSock->m_enableChatFlag8, CHAT_TYPE_PTOP))
//	if (FALSE == COMPARE_CHATTYPE_BIT(pCalledSock->m_enableChatFlag32, CHAT_TYPE_PTOP))
//	{
//		INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_ERROR, T_IC_VOIP_ERROR, pSendErr, tmSendBuf);
//		pSendErr->nErrorNum					= VOIPERR_1to1_CANNOT_CALL;
//		pSendErr->nSend2AccountUniqueNumber	= this->m_character.AccountUniqueNumber;
//		SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_ERROR));		
//		return RES_BREAK;
//	}
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2005-12-07 by cmkwon, 세력체크
//	if(FALSE == GCheckRaceAndInfluenceType(CHECK_TYPE_VOIP_1to1, m_character.Race, m_character.InfluenceType,
//		pCalledSock->GetCharacter()->Race, pCalledSock->GetCharacter()->InfluenceType))
//	{
//		SendErrorMessage(T_IC_VOIP_1to1_DIAL, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED);	
//		return RES_BREAK;
//	}
//
//	if(pCalledSock->IsCharacterNameFromRejectList(this->m_character.CharacterName))
//	{// 상대방 거부리스트에 등록되어 있음
//
//		SendErrorMessage(T_IC_VOIP_1to1_DIAL, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);	
//		return RES_BREAK;
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_1to1_DIAL, T_IC_VOIP_1to1_DIAL, pSendMsg, tmSendBuf);
//	*pSendMsg = *pRecvMsg;
//	pCalledSock->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_1to1_DIAL));
//
//	return RES_RETURN_TRUE;
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_1to1_DIAL_OK(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_1to1_DIAL_OK	*pRecvMsg = NULL;
//
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_1to1_DIAL_OK);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_1to1_DIAL_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_1to1_DIAL_OK*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CIMIOCPSocket *pCallerSock = ms_pIMIOCP->GetIMIOCPSocketByAccountUID(pRecvMsg->nCallerAccountUniqueNumber);
//	if (NULL == pCallerSock)
//	{
//		INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_ERROR, T_IC_VOIP_ERROR, pSendErr, tmSendBuf);
//		pSendErr->nErrorNum					= VOIPERR_1to1_NO_SUCH_ACCOUNTUNIQUENUMBER;
//		pSendErr->nSend2AccountUniqueNumber	= this->m_character.AccountUniqueNumber;
//		SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_ERROR));
//		return RES_BREAK;
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_1to1_DIAL_OK, T_IC_VOIP_1to1_DIAL_OK, pSendMsg, tmSendBuf);
//	*pSendMsg = *pRecvMsg;
//	pCallerSock->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_1to1_DIAL_OK));
//	
//	return RES_RETURN_TRUE;
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_1to1_END(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_1to1_END			*pRecvMsg = NULL;
//	
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_1to1_END);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_ERROR, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_1to1_END*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//	
//	CIMIOCPSocket *pSend2Sock = ms_pIMIOCP->GetIMIOCPSocketByAccountUID(pRecvMsg->nSend2AccountUniqueNumber);
//	if (NULL == pSend2Sock)
//	{
//		// Error를 Client로 전송할 필요 없음
//		return RES_BREAK;
//	}
//	
//	INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_1to1_END, T_IC_VOIP_1to1_END, pSendMsg, tmSendBuf);
//	*pSendMsg = *pRecvMsg;
//	pSend2Sock->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_1to1_END));
//	return RES_RETURN_TRUE;
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_NtoN_CREATEROOM_REQUEST(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST		*pRecvMsg = NULL;
//	
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_NtoN_CREATEROOM_REQUEST, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 1. 길드통화
//	// 2. 편대통화
//	if(FALSE == pRecvMsg->byIsPartyCall)
//	{
//		if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
//		{
//			SendErrorMessage(T_IC_VOIP_NtoN_CREATEROOM_REQUEST, ERR_PROTOCOL_NOT_HAVE_GUILD, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//
//		ms_pIMIOCP->m_MapGuild.lock();
//		{
//			CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//			if(NULL == ptmGuild)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_VOIP_NtoN_CREATEROOM_REQUEST, ERR_PROTOCOL_NOT_HAVE_GUILD);
//				return RES_BREAK;
//			}
//			
//			if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();				
//				SendErrorMessage(T_IC_VOIP_NtoN_CREATEROOM_REQUEST, ERR_PROTOCOL_GUILD_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//		}
//		ms_pIMIOCP->m_MapGuild.unlock();
//	}
//	else
//	{
//		CIMParty *ptmParty = this->GetIMParty();
//		if (ptmParty == NULL)
//		{
//			// error
//			SendErrorMessage(T_IC_VOIP_NtoN_CREATEROOM_REQUEST, ERR_PROTOCOL_NOT_HAVE_PARTY, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//
//		ptmParty->LockIMParty();
//		if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
//		{
//			// error: 자신이 파티장이 아니므로 권한이 없음
//			ptmParty->UnlockIMParty();
//			SendErrorMessage(T_IC_VOIP_NtoN_CREATEROOM_REQUEST, ERR_PROTOCOL_PARTY_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//		ptmParty->UnlockIMParty();
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK, T_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK, pSendMsg, pBuffer);
//	pSendMsg->byIsPartyCall = pRecvMsg->byIsPartyCall;
//	this->SendAddData(pBuffer, MSG_SIZE(MSG_IC_VOIP_NtoN_CREATEROOM_REQUEST_OK));
//	return RES_RETURN_TRUE;
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_NtoN_INVITE_ALL(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_NtoN_INVITE_ALL		*pRecvMsg = NULL;
//	
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_NtoN_INVITE_ALL);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_NtoN_INVITE_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_NtoN_INVITE_ALL*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 1. 길드통화
//	// 2. 편대통화
//	if(FALSE == pRecvMsg->byIsPartyCall)
//	{
//		if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
//		{
//			SendErrorMessage(T_IC_VOIP_NtoN_INVITE_ALL, ERR_PROTOCOL_NOT_HAVE_GUILD, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//
//		ms_pIMIOCP->m_MapGuild.lock();
//		{
//			CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//			if(NULL == ptmGuild)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_VOIP_NtoN_INVITE_ALL, ERR_PROTOCOL_NOT_HAVE_GUILD);
//				return RES_BREAK;
//			}
//			
//			if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();				
//				SendErrorMessage(T_IC_VOIP_NtoN_INVITE_ALL, ERR_PROTOCOL_GUILD_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//			
//			INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_NtoN_INVITE_YOU, T_IC_VOIP_NtoN_INVITE_YOU, pSMsg, pBuffer);
//			pSMsg->byIsPartyCall = pRecvMsg->byIsPartyCall;
//			ptmGuild->SendMsgToGuildMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_YOU), this->m_character.CharacterUniqueNumber);
//		}
//		ms_pIMIOCP->m_MapGuild.unlock();
//	}
//	else
//	{
//		CIMParty *ptmParty = this->GetIMParty();
//		if (ptmParty == NULL)
//		{
//			// error
//			SendErrorMessage(T_IC_VOIP_NtoN_INVITE_ALL, ERR_PROTOCOL_NOT_HAVE_PARTY, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//		
//		ptmParty->LockIMParty();
//		{
//			if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
//			{
//				// error: 자신이 파티장이 아니므로 권한이 없음
//				ptmParty->UnlockIMParty();
//				SendErrorMessage(T_IC_VOIP_NtoN_INVITE_ALL, ERR_PROTOCOL_PARTY_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//			
//			INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_NtoN_INVITE_YOU, T_IC_VOIP_NtoN_INVITE_YOU, pSMsg, pBuffer);
//			pSMsg->byIsPartyCall = pRecvMsg->byIsPartyCall;
//			ptmParty->SendMsgToMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_YOU), this->m_character.CharacterUniqueNumber);
//		}
//		ptmParty->UnlockIMParty();
//	}
//
//	return RES_RETURN_TRUE;
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_NtoN_INVITE_USER(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_NtoN_INVITE_USER		*pRecvMsg = NULL;
//	
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_NtoN_INVITE_USER);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_NtoN_INVITE_USER, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_NtoN_INVITE_USER*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//
//	CIMIOCPSocket *pPeerSocket = NULL;
//	///////////////////////////////////////////////////////////////////////////////
//	// 1. 길드통화
//	// 2. 편대통화
//	if(FALSE == pRecvMsg->byIsPartyCall)
//	{
//		if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
//		{
//			SendErrorMessage(T_IC_VOIP_NtoN_INVITE_USER, ERR_PROTOCOL_NOT_HAVE_GUILD, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//
//		ms_pIMIOCP->m_MapGuild.lock();
//		{
//			CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//			if(NULL == ptmGuild)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_VOIP_NtoN_INVITE_USER, ERR_PROTOCOL_NOT_HAVE_GUILD);
//				return RES_BREAK;
//			}
//			
//			if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_VOIP_NtoN_INVITE_USER, ERR_PROTOCOL_GUILD_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//			
//			CGuildMember *pMember = NULL;
//			BOOL bReted = ptmGuild->GetGuildMember(pRecvMsg->nCharacterUniqueNumber, pMember);
//			if (!bReted)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_GUILD_LEAVE, ERR_PROTOCOL_NO_SUCH_GUILD_MEMBER);
//				return RES_BREAK;
//			}
//
//			if(FALSE == pMember->m_bIsOnline
//				|| NULL == pMember->m_pIMIOCPSocket)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				return RES_BREAK;
//			}
//			pPeerSocket = pMember->m_pIMIOCPSocket;
//		}
//		ms_pIMIOCP->m_MapGuild.unlock();
//
//	}
//	else
//	{
//		CIMParty *ptmParty = this->GetIMParty();
//		if (ptmParty == NULL)
//		{
//			// error
//			SendErrorMessage(T_IC_VOIP_NtoN_INVITE_USER, ERR_PROTOCOL_NOT_HAVE_PARTY, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//		
//		ptmParty->LockIMParty();
//		{
//			if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
//			{
//				// error: 자신이 파티장이 아니므로 권한이 없음
//				ptmParty->UnlockIMParty();
//				SendErrorMessage(T_IC_VOIP_NtoN_INVITE_USER, ERR_PROTOCOL_PARTY_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//			
//			IMPartyMember *pIMMember = ptmParty->GetMemberPtrByCharacterUniqueNumber(pRecvMsg->nCharacterUniqueNumber);
//			if(NULL == pIMMember)
//			{
//				// error: 상대방이 Party에 존재하지 않음, 에러 전송하지 않음
//				ptmParty->UnlockIMParty();
//				return RES_BREAK;
//			}
//			pPeerSocket = pIMMember->PM_pIMIOCPSocket;
//		}
//		ptmParty->UnlockIMParty();
//
//		if(NULL == pPeerSocket || FALSE == pPeerSocket->IsUsing())
//		{
//			return RES_BREAK;
//		}
//	}
//
//	if(pPeerSocket->IsUsing())
//	{
//		INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_NtoN_INVITE_YOU, T_IC_VOIP_NtoN_INVITE_YOU, pSMsg, pSBuffer);
//		pSMsg->byIsPartyCall = pRecvMsg->byIsPartyCall;
//		pPeerSocket->SendAddData(pSBuffer, MSG_SIZE(MSG_IC_VOIP_NtoN_INVITE_YOU));
//	}
//	return RES_RETURN_TRUE;
//}
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_NtoN_GOOUT_ALL(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_NtoN_GOOUT_ALL		*pRecvMsg = NULL;
//	
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_NtoN_GOOUT_ALL);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_NtoN_GOOUT_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_NtoN_GOOUT_ALL*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//	
//	///////////////////////////////////////////////////////////////////////////////
//	// 1. 길드통화
//	// 2. 편대통화
//	if(FALSE == pRecvMsg->byIsPartyCall)
//	{
//		if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
//		{
//			SendErrorMessage(T_IC_VOIP_NtoN_GOOUT_ALL, ERR_PROTOCOL_NOT_HAVE_GUILD, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//
//		ms_pIMIOCP->m_MapGuild.lock();
//		{
//			CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//			if(NULL == ptmGuild)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_VOIP_NtoN_GOOUT_ALL, ERR_PROTOCOL_NOT_HAVE_GUILD);
//				return RES_BREAK;
//			}
//			
//			if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();				
//				SendErrorMessage(T_IC_VOIP_NtoN_GOOUT_ALL, ERR_PROTOCOL_GUILD_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//			
//			INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_NtoN_GOOUT_ALL, T_IC_VOIP_NtoN_GOOUT_ALL, pSMsg, pBuffer);
//			pSMsg->byIsPartyCall = pRecvMsg->byIsPartyCall;
//			ptmGuild->SendMsgToGuildMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_NtoN_GOOUT_ALL), this->m_character.CharacterUniqueNumber);
//		}
//		ms_pIMIOCP->m_MapGuild.unlock();		
//	}
//	else
//	{
//		CIMParty *ptmParty = this->GetIMParty();
//		if (ptmParty == NULL)
//		{
//			// error
//			SendErrorMessage(T_IC_VOIP_NtoN_GOOUT_ALL, ERR_PROTOCOL_NOT_HAVE_PARTY, pRecvMsg->byIsPartyCall);
//			return RES_BREAK;
//		}
//		
//		ptmParty->LockIMParty();
//		{
//			if (ptmParty->GetMasterCharacterUniqueNumber() != m_character.CharacterUniqueNumber)
//			{
//				// error: 자신이 파티장이 아니므로 권한이 없음
//				ptmParty->UnlockIMParty();
//				SendErrorMessage(T_IC_VOIP_NtoN_GOOUT_ALL, ERR_PROTOCOL_PARTY_PERMISSION_DENIED, pRecvMsg->byIsPartyCall);
//				return RES_BREAK;
//			}
//			
//			INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_NtoN_GOOUT_ALL, T_IC_VOIP_NtoN_GOOUT_ALL, pSMsg, pBuffer);
//			pSMsg->byIsPartyCall = pRecvMsg->byIsPartyCall;
//			ptmParty->SendMsgToMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_NtoN_GOOUT_ALL), this->m_character.CharacterUniqueNumber);
//		}
//		ptmParty->UnlockIMParty();
//	}
//
//	return RES_RETURN_TRUE;
//}
//
//
//ProcessResult CIMIOCPSocket::Process_IC_VOIP_ERROR(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int							nRecvTypeSize = 0;
//	MSG_IC_VOIP_ERROR			*pRecvMsg = NULL;
//	
//	nRecvTypeSize = sizeof(MSG_IC_VOIP_ERROR);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_IC_VOIP_ERROR, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		Close(0x1201C);
//		return RES_RETURN_FALSE;
//	}
//	pRecvMsg = (MSG_IC_VOIP_ERROR*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//	
//	CIMIOCPSocket *pSend2Sock = ms_pIMIOCP->GetIMIOCPSocketByAccountUID(pRecvMsg->nSend2AccountUniqueNumber);
//	if (NULL == pSend2Sock)
//	{
//		// Error를 Client로 전송할 필요 없음
//		return RES_BREAK;
//	}
//	
//	INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_ERROR, T_IC_VOIP_ERROR, pSendMsg, tmSendBuf);
//	*pSendMsg = *pRecvMsg;
//	pSend2Sock->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_VOIP_ERROR));
//	return RES_RETURN_TRUE;
//}


ProcessResult CIMIOCPSocket::Process_IM_PRINT_DEBUG_MSG(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_IM_PRINT_DEBUG_MSG Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int						nRecvTypeSize	= 0;
	MSG_FM_PRINT_DEBUG_MSG	*pRecvPrintDbg = NULL;

	nRecvTypeSize = sizeof(MSG_FM_PRINT_DEBUG_MSG);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_IM_PRINT_DEBUG_MSG, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x14056);
		return RES_RETURN_FALSE;
	}
	pRecvPrintDbg = (MSG_FM_PRINT_DEBUG_MSG*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	// 필드 서버들에게 전송
	MessageType_t msgType = T_FI_ADMIN_PRINT_DEBUG_MSG;
	ms_pIMIOCP->SendMessageToFieldServer((BYTE*)&msgType, SIZE_FIELD_TYPE_HEADER);
// 2004-10-21 by cmkwon, 위의 함수로 대체
//	ms_pIMIOCP->m_MapFieldServer.lock();
//	mt_map<SERVER_ID, FieldServerInfo*>::iterator itrServer = ms_pIMIOCP->m_MapFieldServer.begin();
//	while(itrServer != ms_pIMIOCP->m_MapFieldServer.end())
//	{
//		FieldServerInfo *pServerInfo = itrServer->second;
//		MessageType_t pMsgType = T_FI_ADMIN_PRINT_DEBUG_MSG;
//		pServerInfo->pSocket->SendAddData((BYTE*)&pMsgType, SIZE_FIELD_TYPE_HEADER);
//		itrServer++;
//	}
//	ms_pIMIOCP->m_MapFieldServer.unlock();

	DBGOUT("=== START - DEBUG MESSAGE REQUESTED BY MONITOR SERVER ========\n");
	DBGOUT("  Int Param1   : %d \n", pRecvPrintDbg->IntParam1);
	DBGOUT("  Int Param2   : %d \n", pRecvPrintDbg->IntParam2);
	DBGOUT("  Float Param1 : %d \n", (int)pRecvPrintDbg->FloatParam1);
	DBGOUT("  Float Param2 : %d \n", (int)pRecvPrintDbg->FloatParam2);
	DBGOUT("  String Param : \"%s\" \n", pRecvPrintDbg->StringParam);

	//char msg[200000];
	char *msg = NULL;
// 2008-08-29 by cmkwon, 전송 필요 없음
//	PRINT_DEBUG_MESSAGE(msg);


	DBGOUT("=== END --- DEBUG MESSAGE REQUESTED BY MONITOR SERVER ========\n");

	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_IM_GET_TOTAL_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_IM_GET_TOTAL_USER_COUNTS Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	INIT_MSG_WITH_BUFFER(MSG_IM_GET_TOTAL_USER_COUNTS_ACK, T_IM_GET_TOTAL_USER_COUNTS_ACK, pSeAck, SendBuf);
	pSeAck->AccumulatedTotalUserCounts	= ms_pIMIOCP->m_uiIMAccumulatedTotalUserCounts;
	pSeAck->CurrentTotalUserCounts		= ms_pIMIOCP->m_MapAccountUniqueNumber.size();
	pSeAck->MaxTotalUserCounts			= ms_pIMIOCP->m_uiIMMaxTotalUserCounts;
	
	ms_pIMIOCP->SendMessageToMonitor(SendBuf, MSG_SIZE(MSG_IM_GET_TOTAL_USER_COUNTS_ACK));
	return RES_RETURN_TRUE;
}

ProcessResult CIMIOCPSocket::Process_FI_GET_FIELD_USER_COUNTS_ACK(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_GET_FIELD_USER_COUNTS_ACK Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	int									nRecvTypeSize = 0;
	MSG_FI_GET_FIELD_USER_COUNTS_ACK	*pMsgGetCountsAck = NULL;

	nRecvTypeSize = sizeof(MSG_FI_GET_FIELD_USER_COUNTS_ACK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_GET_FIELD_USER_COUNTS_ACK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1201D);
		return RES_RETURN_FALSE;
	}
	pMsgGetCountsAck = (MSG_FI_GET_FIELD_USER_COUNTS_ACK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

//	FieldServerInfo *pFSInfo = ms_pIMIOCP->m_MapFieldServer.findLock(pMsgGetCountsAck->FieldServerID);
//	if (pFSInfo == NULL)
//	{
//		DBGOUT("[Error]ERR_PROTOCOL_NO_SUCH_FIELD_SERVER: %s\n", pMsgGetCountsAck->FieldServerID.GetString(string()));
//		SendErrorMessage(T_FI_GET_FIELD_USER_COUNTS_ACK, ERR_PROTOCOL_NO_SUCH_FIELD_SERVER, pMsgGetCountsAck->FieldServerID.port, 0, pMsgGetCountsAck->FieldServerID.IP);
//		return RES_BREAK;
//	}
//	pFSInfo->AccumulatedFieldUserCounts		= pMsgGetCountsAck->AccumulatedFieldUserCounts;
//	pFSInfo->CurrentFieldUserCounts			= pMsgGetCountsAck->CurrentFieldUserCounts;
//	pFSInfo->MaxFieldUserCounts				= pMsgGetCountsAck->MaxFieldUserCounts;
//	pFSInfo->AccumulatedFieldMonsterCounts	= pMsgGetCountsAck->AccumulatedFieldMonsterCounts;
//	pFSInfo->CurrentFieldMonsterCounts		= pMsgGetCountsAck->CurrentFieldMonsterCounts;
//
//	BYTE	SendBuf[SIZE_MAX_PACKET];
//	MSG_FIELDSERVER_USERCOUNTS *pFUserCounts = NULL;
//
//	INIT_MSG(MSG_IM_GET_TOTAL_USER_COUNTS_ACK, T_IM_GET_TOTAL_USER_COUNTS_ACK, pSendTotalCounts, SendBuf);
//	memset(pSendTotalCounts, 0x00, sizeof(MSG_IM_GET_TOTAL_USER_COUNTS_ACK));
//	pFUserCounts = (MSG_FIELDSERVER_USERCOUNTS*)(SendBuf + MSG_SIZE(MSG_IM_GET_TOTAL_USER_COUNTS_ACK));
//
//	ms_pIMIOCP->m_MapFieldServer.lock();
//	mt_map<SERVER_ID, FieldServerInfo*>::iterator  itr(ms_pIMIOCP->m_MapFieldServer.begin());
//	while(itr != ms_pIMIOCP->m_MapFieldServer.end())
//	{
//		strncpy(pFUserCounts->FieldIP, itr->second->serverID.IP, SIZE_MAX_IPADDRESS);
//		pFUserCounts->FieldListenPort = itr->second->serverID.port;
//		pFUserCounts->AccumulatedFieldUserCounts = itr->second->AccumulatedFieldUserCounts;
//		pFUserCounts->CurrentFieldUserCounts = itr->second->CurrentFieldUserCounts;
//		pFUserCounts->MaxFieldUserCounts = itr->second->MaxFieldUserCounts;
//		pFUserCounts->AccumulatedFieldMonsterCounts = itr->second->AccumulatedFieldMonsterCounts;
//		pFUserCounts->CurrentFieldMonsterCounts = itr->second->CurrentFieldMonsterCounts;
//
//		pSendTotalCounts->AccumulatedTotalUserCounts += pFUserCounts->AccumulatedFieldUserCounts;
//		pSendTotalCounts->CurrentTotalUserCounts += pFUserCounts->CurrentFieldUserCounts;
//		pSendTotalCounts->MaxTotalUserCounts += pFUserCounts->MaxFieldUserCounts;
//		pSendTotalCounts->AccumulatedTotalMonsterCounts += pFUserCounts->AccumulatedFieldMonsterCounts;
//		pSendTotalCounts->CurrentTotalMonsterCounts += pFUserCounts->CurrentFieldMonsterCounts;
//
//		pSendTotalCounts->FieldServerCounts++;
//		pFUserCounts++;
//		itr++;
//	}
//	ms_pIMIOCP->m_MapFieldServer.unlock();
//
//	ms_pIMIOCP->SendIMToMonitor(SendBuf, MSG_SIZE(MSG_IM_GET_TOTAL_USER_COUNTS_ACK) + sizeof(MSG_FIELDSERVER_USERCOUNTS) * pSendTotalCounts->FieldServerCounts);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMIOCPSocket::Process_FI_INFO_DECLARATION_MSWAR_SET_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		선전 포고 - 선전포고 시간 및 포기 설정 F->I
/// \author		dhjin
/// \date		2009-01-20 ~ 2009-01-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMIOCPSocket::Process_FI_INFO_DECLARATION_MSWAR_SET_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	if (FALSE == g_pIMGlobal->CheckAllowedToolIP(this->GetPeerIP()))
	{
		g_pIMGlobal->WriteSystemLogEX(TRUE, "HACKUSER!! Connect Process_FI_INFO_DECLARATION_MSWAR_SET_OK Command Using: HackingIP(%15s)\r\n", this->GetPeerIP());
		return RES_RETURN_FALSE;
	}
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_INFO_DECLARATION_MSWAR_SET_OK,
									MSG_FI_INFO_DECLARATION_MSWAR_SET_OK, pMsg);
	
	INIT_MSG_WITH_BUFFER(MSG_IC_INFO_DECLARATION_MSWAR_SET_OK, T_IC_INFO_DECLARATION_MSWAR_SET_OK, pSendMsg, SendBuf);
	pSendMsg->Influence			= pMsg->Influence;
	pSendMsg->MSWarStartTime	= pMsg->MSWarStartTime;
	pSendMsg->GiveUp			= pMsg->GiveUp;
	pSendMsg->SelectCount		= pMsg->SelectCount;
	this->ms_pIMIOCP->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_IC_INFO_DECLARATION_MSWAR_SET_OK));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::GameStartRoutineIM(MSG_FI_CONNECT_NOTIFY_GAMESTART *i_FINotifyGameStart, CIMIOCPSocket *i_pF2IISock)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-09 ~ 2006-05-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::GameStartRoutineIM(MSG_FI_CONNECT_NOTIFY_GAMESTART *i_FINotifyGameStart, CIMIOCPSocket *i_pF2IISock)
{
	if(this->GetCharacter()->CharacterUniqueNumber != i_FINotifyGameStart->CharacterUniqueNumber)
	{// 2006-05-09 by cmkwon
		return;
	}

	if (COMPARE_RACE(this->m_character.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{// 2006-05-08 by cmkwon,

		///////////////////////////////////////////////////////////////////////////////
		// 2006-11-14 by cmkwon, 기본적으로 /귓말 을 비활성화 한다
// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//		CLEAR_CHATTYPE_BIT(m_enableChatFlag8, CHAT_TYPE_PTOP);
		CLEAR_CHATTYPE_BIT(m_enableChatFlag32, CHAT_TYPE_PTOP);

		///////////////////////////////////////////////////////////////////////////////
		// 2006-05-08 by cmkwon
		ms_pIMIOCP->AddToAdminCharacter(this->GetCharacter()->CharacterUniqueNumber);

		///////////////////////////////////////////////////////////////////////////////
		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
		if(ms_pIMIOCP->IsOnCallGMSystem())
		{
			this->SendString128(STRING_128_ADMIN_CMD, STRMSG_071120_0004
				, ms_pIMIOCP->m_atCallGMStartTime.GetDateTimeString().GetBuffer()
				, ms_pIMIOCP->m_atCallGMEndTime.GetDateTimeString().GetBuffer());
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2006-05-08 by cmkwon
		int nCallGMCnt = ms_pIMIOCP->m_mtvectSCallGMList.size();
		if(0 < nCallGMCnt)
		{
			INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_CALLGM_INFO_OK, T_IC_ADMIN_CALLGM_INFO_OK, pCallGMInfo, SendBuf);
			pCallGMInfo->nCallGMTotalCount	= nCallGMCnt;
			this->SendAddData(SendBuf, MSG_SIZE(MSG_IC_ADMIN_CALLGM_INFO_OK));
		}
	}

	this->m_bGameEndRoutineFlag			= TRUE;
	this->m_bFirstTimeLoadFriendList	= TRUE;		// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 

	///////////////////////////////////////////////////////////////////////////
	// MAP_CHANNEL_INDEX 할당	
	this->m_character.MapChannelIndex	= i_FINotifyGameStart->MapChannelIndex;

	///////////////////////////////////////////////////////////////////////////
	// 길드 정보 로딩	
	if(IS_VALID_UNIQUE_NUMBER(this->m_character.GuildUniqueNumber))
	{
		ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildLoadGuild, this, NULL, NULL, i_FINotifyGameStart->CharacterUniqueNumber);
// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 여단 정보 여단원 접속시마다 로딩하도록 위와 같이 수정
//		///////////////////////////////////////////////////////////////////////////////
//		// 2007-02-06 by cmkwon, 기존에 하나의 DB에 여러개의 서버가 접속하는 시스템은 세력전으로 불가능하므로 아래와 같이 여단정보는 한번만 로딩하도록 수정한다
//		mt_auto_lock mtGA(&ms_pIMIOCP->m_MapGuild);
//
//		CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_character.GuildUniqueNumber);
//		if(NULL == ptmGuild)
//		{// 2007-02-06 by cmkwon, 로딩된 여단정보가 없다
//			ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_GuildLoadGuild, this, NULL, NULL, i_FINotifyGameStart->CharacterUniqueNumber);
//		}
//		else
//		{// 2007-02-06 by cmkwon, 여단정보가 이미 로딩 되어있다
//			CGuildMember *pGMember = NULL;
//			ptmGuild->GetGuildMember(m_character.CharacterUniqueNumber, pGMember);
//			if(pGMember)
//			{
//				ptmGuild->SetOnlineGuildMember(this);
//
//				this->SetGuildUniqueNumber(m_character.GuildUniqueNumber);
//
//				INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_RANK, T_FI_GUILD_RANK_OK, pGRank, SendBuf);
//				pGRank->charUID		= m_character.CharacterUniqueNumber;
//				pGRank->GuildRank	= pGMember->m_Rank;
//				this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));
//
//				INIT_MSG(MSG_IC_GUILD_LOADING_GUILD_DONE, T_IC_GUILD_LOADING_GUILD_DONE, pGLoading, SendBuf);
//				pGLoading->GuildUniqueNumber = m_character.GuildUniqueNumber;
//				this->SendAddData(SendBuf, MSG_SIZE(MSG_IC_GUILD_LOADING_GUILD_DONE));
//			}
//		}
	}	

	///////////////////////////////////////////////////////////////////////////
	// 이전 파티 검사
	///////////////////////////////////////////////////////////////////////////
	if (this->m_character.LastPartyID != 0)
	{
#ifdef _DEBUG
		this->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0005);
#endif
		// 파티 검색
		CIMParty *pParty = ms_pIMIOCP->FindParty(this->m_character.LastPartyID);
		if (pParty == NULL)
		{
			// 파티가 이미 해산된 경우
			this->SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_PROTOCOL_NO_SUCH_PARTY, this->m_character.LastPartyID);

			this->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0006);

			// Update DB
			QPARAM_SAVELASTPARTYID *pQSavePartyID = new QPARAM_SAVELASTPARTYID;
			pQSavePartyID->CharacterUniqueNumber = this->m_character.CharacterUniqueNumber;
			pQSavePartyID->LastPartyID = 0;
			ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_SaveLastPartyID, this, pQSavePartyID);

			// Update CHARACTER
			this->m_character.LastPartyID = 0;
			return;
		}
		
		pParty->LockIMParty();
		{
			IMPartyMember *pMember = pParty->GetMemberPtrByCharacterUniqueNumber(i_FINotifyGameStart->CharacterUniqueNumber);
			if (pMember == NULL)
			{
				// 파티원이 없는 경우
				pParty->UnlockIMParty();
				i_pF2IISock->SendErrorMessage(T_FI_CONNECT_NOTIFY_GAMESTART, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, i_FINotifyGameStart->CharacterUniqueNumber);
				return;
			}

			// 사용자에게 파티 정보 할당
			this->SetIMParty(pParty);

			// validate party member
			pMember->SetOnlineMember(this);

			// 자기 자신에게 보냄
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_PUT_LAST_PARTY_INFO, T_IC_PARTY_PUT_LAST_PARTY_INFO, pLastPartyInfo, pLastPartyInfoBuf);
			pLastPartyInfo->PartyID = pParty->m_PartyID;
			this->SendAddData(pLastPartyInfoBuf, MSG_SIZE(MSG_IC_PARTY_PUT_LAST_PARTY_INFO));

			this->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0007);

			// 2008-06-10 by dhjin, EP3 편대 수정 - 편대 정보 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_INFO, T_IC_PARTY_INFO, pSendMsg, pSendBuf);
			pSendMsg->PartyInfo = pParty->m_SPartyInfo;
			this->SendAddData(pSendBuf, MSG_SIZE(MSG_IC_PARTY_INFO));
		}
		pParty->UnlockIMParty();
	}
}

///////////////////////////////////////////////////////////////////////////////
// return 할 때 unlock 수행하기! 20031103, kelovon
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::GameEndRoutineIM(void)
{
	this->m_mtCritSecForGameEndRoutine.Enter();
	{
		if (FALSE == m_bGameEndRoutineFlag
			|| 0 == m_character.ClientIndex
			|| 0 == m_character.CharacterUniqueNumber
			|| 0 == m_character.AccountUniqueNumber
			|| ST_INVALID_TYPE == m_PeerSocketType)
		{
			DBGOUT("  ClientIndex(%d)'s CIMIOCPSocket::GameEndRoutineIM()... ALREADY PROCESSED..\r\n", this->GetClientArrayIndex());

			this->m_mtCritSecForGameEndRoutine.Leave();
			return;
		}
		m_bGameEndRoutineFlag = FALSE;
	}
	this->m_mtCritSecForGameEndRoutine.Leave();
		
#ifdef _DEBUG
	DBGOUT("  %s's CIMIOCPSocket::GameEndRoutineIM()...\r\n", GetCharacterString(&m_character, string()));
#endif

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-18 by dhjin, EP3 채팅방 -
	if(this->m_bChatRoomUseing)
	{
		mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));
		mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
		for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
		{
			if(this->m_nChatRoomNum == (*itr)->GetChatRoomNum())
			{
				if(this->m_character.CharacterUniqueNumber == (*itr)->GetChatRoomMasterCharacterUID()
					&& 1 < (*itr)->GetChatRoomMemberCount())
				{
					INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_CHANGE_MASTER_OK, T_IC_CHATROOM_CHANGE_MASTER_OK, msg, SendBuf);
					util::strncpy(msg->CharacterName, (*itr)->ChangeChatRoomMaster(), SIZE_MAX_CHARACTER_NAME);
					if(NULL == msg->CharacterName)
					{
						break;
					}
					msg->NewCharacterUID = (*itr)->GetChatRoomMasterCharacterUID();
					(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_CHANGE_MASTER_OK));
				}

				INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_LEAVE_OK, T_IC_CHATROOM_LEAVE_OK, msg, SendBuf);
				util::strncpy(msg->LeaveCharacterName, this->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
				msg->CharacterUID = this->m_character.CharacterUniqueNumber;
				(*itr)->SendMessagetoChatRoomMember(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_LEAVE_OK));

				if((*itr)->LeaveMemberAndCheckEmpty(this->m_character.CharacterUniqueNumber))
				{
					ms_pIMIOCP->m_ChatRoomManager.SetReserveChatNum((*itr)->GetChatRoomNum());
					util::del(*itr);
					itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.erase(itr);
				}
				break;
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2008-10-22 by cmkwon, 채팅방 버그 수정 - 
		this->m_bChatRoomUseing		= FALSE;
		this->m_nChatRoomNum		= 0;
	}

	CIMParty *ptmParty = this->GetIMParty();
	if (ptmParty)
	{
		ptmParty->LockIMParty();
		PartyID_t partyID = ptmParty->m_PartyID;

		//////////////////////////////////////////////////////////////////////////
		// 2008-02-28 by dhjin, 아레나 통합 - 아레나 서버에서 종료는 바로 삭제로 처리해야한다.~!!!
		if(ms_pIMIOCP->m_bArenaServerCheck)
		{
			BOOL bReted = ProcessPartyLeaveNoLock(ptmParty);
			ptmParty->UnlockIMParty();

			if(FALSE == bReted)
			{// 파티맵에서 제거해야한다

				ProcessPartyDismember(partyID);
			}
		}
		else
		{
			BOOL bReted = ProcessPartyGameEndNoLock(ptmParty);
			ptmParty->UnlockIMParty();

			if(FALSE == bReted)
			{// 파티맵에서 제거해야한다
				
				ProcessPartyDismember(partyID);			
			}
			else
			{
				// Update DB
				QPARAM_SAVELASTPARTYID *pQSavePartyID	= new QPARAM_SAVELASTPARTYID;
				pQSavePartyID->CharacterUniqueNumber	= m_character.CharacterUniqueNumber;
				pQSavePartyID->LastPartyID				= partyID;
				ms_pIMIOCP->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_SaveLastPartyID, this, pQSavePartyID);
			}
		}
		///////////////////////////////////////////////////////////////////////////////
		// 캐릭터의 파티 정보 삭제
		this->SetIMParty(NULL);
	
	}
	
	if (IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
	{
		ms_pIMIOCP->m_MapGuild.lock();
		{
			///////////////////////////////////////////////////////////////////////////////
			// 여단전 체크(오프라인 설정 전에 체크해야함)
			ms_pIMIOCP->GuildWarCheck(m_uidGuildUniqueNumber, m_character.MapChannelIndex);

			auto ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
			if (ptmGuild)
			{
				// 오프라인으로 설정
				ptmGuild->SetOfflineGuildMember(m_character.CharacterUniqueNumber);
				
				//ptmGuild->SetMemberVoipType(m_character.CharacterUniqueNumber, CHECK_TYPE_VOIP_NONE);				// 2008-07-16 by dhjin, EP3 - Voip 정보
				//INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_SET_OK, T_IC_VOIP_SET_OK, pSMsg, pBuffer);
				//pSMsg->CharacterUID	= m_character.CharacterUniqueNumber;
				//pSMsg->VoipType		= CHECK_TYPE_VOIP_NONE;
				//ptmGuild->SendMsgToGuildMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_SET_OK));

				if (ptmGuild->GetOnlineMemberCount(m_character.CharacterUniqueNumber) == 0)
				{
					ms_pIMIOCP->m_MapGuild.deleteLock(ptmGuild->m_GuildUniqueNumber);
					util::del(ptmGuild);
				}
				
			}
		}
		ms_pIMIOCP->m_MapGuild.unlock();
		this->SetGuildUniqueNumber(INVALID_GUILD_UID);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-19 by cmkwon, 친구 리스트 정리
	if(false == m_mtvectorFriendList.empty())
	{
		mt_auto_lock mtA(&m_mtvectorFriendList);
		mtvectorFRIENDINFO::iterator itr(m_mtvectorFriendList.begin());
		for(; itr != m_mtvectorFriendList.end(); itr++)
		{
			if(itr->byIsOnline)
			{
				CIMIOCPSocket *pIISockFriend = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(itr->szCharacterName);
				if(pIISockFriend
					&& pIISockFriend->IsValidCharacter()
					&& pIISockFriend->DecreaseOnlineEachOtherFriendCnts(m_character.CharacterName))
				{
					///////////////////////////////////////////////////////////////////////////////
					// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
					pIISockFriend->SendOnlineEachOtherFriendCntsToFieldServer();
				}
			}
		}
		m_mtvectorFriendList.clear();
		m_nEachOtherFriendCnts	= 0;		// 2006-07-19 by cmkwon
	}
	m_mtvectorRejectList.lock();
	m_mtvectorRejectList.clear();
	m_mtvectorRejectList.unlock();

	// 각종 map에서 삭제
	if (COMPARE_RACE(m_character.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		ms_pIMIOCP->DeleteFromAdminCharacter(m_character.CharacterUniqueNumber);
	}
	ms_pIMIOCP->m_mAccountName.deleteLock(m_character.AccountName);
	ms_pIMIOCP->m_MapAccountUniqueNumber.deleteLock(m_character.AccountUniqueNumber);
	char tmCharacterName[SIZE_MAX_CHARACTER_NAME];
	ms_pIMIOCP->m_mCharacterName.deleteLock(GGetLowerCase(tmCharacterName, m_character.CharacterName, SIZE_MAX_CHARACTER_NAME));
	ms_pIMIOCP->m_MapChactarterUniqueNumber.deleteLock(m_character.CharacterUniqueNumber);

	memset(&m_character, 0x00, sizeof(CHARACTER));

	this->SetIMParty(NULL);
	this->SetGuildUniqueNumber(INVALID_GUILD_UID);
	this->SetCurrentFieldServerSocket(NULL);
	// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//	m_enableChatFlag8		= (BitFlag8_t)0xFF;
	m_enableChatFlag32 = (BitFlag32_t)0xFFFF;
	m_PeerSocketType		= ST_INVALID_TYPE;
	m_bSendMessgeTypeDBG	= FALSE;
	m_bGameEndRoutineFlag	= FALSE;

	m_VoipType				= CHECK_TYPE_VOIP_NONE;		// 2008-10-21 by cmkwon, 버그 수정
	return;
}


void CIMIOCPSocket::CharacterDeadRoutine(UID32_t i_uidAttackerGuildUID)
{
	if (IS_VALID_UNIQUE_NUMBER(this->m_uidGuildUniqueNumber)
		&& IS_VALID_UNIQUE_NUMBER(i_uidAttackerGuildUID))
	{
		ms_pIMIOCP->GuildWarCheck(m_uidGuildUniqueNumber, m_character.MapChannelIndex, i_uidAttackerGuildUID);
	}

	///////////////////////////////////////////////////////////////////////////////
	//
	SET_BODYCON_BIT(m_character.BodyCondition, BODYCON_DEAD_MASK);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::SendMsgToCurrentFieldServerSocket(BYTE *i_pData, int i_nLength)
/// \brief		
/// \author		cmkwon
/// \date		2004-10-21 ~ 2004-10-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::SendMsgToCurrentFieldServerSocket(BYTE *i_pData, int i_nLength)
{
	CIMIOCPSocket *ptmIISocket = this->m_pCurrentFieldServerSocket;
	if(NULL == ptmIISocket || FALSE == ptmIISocket->IsUsing())
	{
		return FALSE;
	}

	return ptmIISocket->SendAddData(i_pData, i_nLength);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::SendOnlineEachOtherFriendCntsToFieldServer(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::SendOnlineEachOtherFriendCntsToFieldServer(void)
{	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
	INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK, T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK, pSFriendCnts, SendBuf);
	pSFriendCnts->CharacterUniqueNumber	= this->GetCharacter()->CharacterUniqueNumber;
	pSFriendCnts->OnlineFriendCnts		= this->m_nEachOtherFriendCnts;
	ms_pIMIOCP->SendMessageToFieldServer(SendBuf, MSG_SIZE(MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK));

	//////////////////////////////////////////////////////////////////////////
	// 2008-07-11 by dhjin, EP3 친구목록 -
	INIT_MSG_WITH_BUFFER(MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT, T_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT, pSICFriendCnts, ICSendBuf);
	pSICFriendCnts->OnlineFriendCnts		= this->m_nEachOtherFriendCnts;
	this->SendAddData(ICSendBuf, MSG_SIZE(MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT));
}

void CIMIOCPSocket::PRINT_DEBUG_MESSAGE(char*& msg)
{
	char szTime[128];
	time_t ltime;
	struct tm *today = NULL;
	time(&ltime);
	today = localtime(&ltime);
	strftime(szTime, 128, "[%m-%d %H:%M:%S]", today );

	PRINT_DEBUG_MESSAGE_LOCK.lock();
	///////////////////////////////////////////////////////////////////////////
	// START - DEBUG MESSAGE
	///////////////////////////////////////////////////////////////////////////

	// 자기 자신의 정보
	SendString128(STRING_128_ADMIN_CMD, "IM - AUN(%d) CUN(%d) CI(%d) MAP(%s)",
		m_character.AccountUniqueNumber, m_character.CharacterUniqueNumber,
		m_character.ClientIndex, GET_MAP_STRING(m_character.MapChannelIndex));

// check: DBGOUT에 문제 발생함, 확인 필요함, 20041101, kelovon
//	DBGOUT2(DO2_ALL, "\n[Party Info] - %s\n", szTime);
//
//	ms_pIMIOCP->m_MapParties.lock();
//	mt_map<PartyID_t, CIMParty*>::iterator itr = ms_pIMIOCP->m_MapParties.begin();
//	while(itr != ms_pIMIOCP->m_MapParties.end())
//	{
//		CIMParty *pParty = itr->second;
//		DBGOUT2(DO2_ALL, "PARTY Master: %10s(%d, %d) - key: %d, MemberCounts: %d/%d\n"
//			, ms_pIMIOCP->GetCharacterByArrayIndex(pParty->GetMasterClientIndex())->CharacterName
//			, pParty->GetMasterCharacterUniqueNumber()
//			, pParty->GetMasterClientIndex()
//			, HIDWORD(pParty->m_PartyID)
//			, pParty->GetMemberCount(TRUE)
//			, pParty->GetMemberCount(FALSE)
//		);	// check: DBGOUT에 문제 발생함, 확인 필요함, 20041101, kelovon
//
//		pParty->m_ListPartyMember.lock();
//		mt_list<IMPartyMember>::iterator itr2 = pParty->m_ListPartyMember.begin();
//		while (itr2 != pParty->m_ListPartyMember.end())
//		{			
//			if (FALSE == itr2->IsValidMember())
//			{
//				DBGOUT2(DO2_ALL, "      Member: %10s(%6d, (N/A))\n"
//					, itr2->PM_CharacterName
//					, itr2->PM_CharacterUniqueNumber
//				);
//			}
//			else
//			{
//				DBGOUT2(DO2_ALL, "      Member: %10s(%6d, %4d)\n"
//					, itr2->PM_CharacterName
//					, itr2->PM_CharacterUniqueNumber
//					, itr2->PM_pIMIOCPSocket->GetClientArrayIndex()
//				);
//			}
//
//			itr2++;
//		}
//		pParty->m_ListPartyMember.unlock();
//
//		itr++;
//	}
//	ms_pIMIOCP->m_MapParties.unlock();

	///////////////////////////////////////////////////////////////////////////
	// 주의: 서비스 돌릴 때는 계정 찍지 말자, 20031103, kelovon
	///////////////////////////////////////////////////////////////////////////
	PRINT_DEBUG_MESSAGE_LOCK.unlock();
	return;
	///////////////////////////////////////////////////////////////////////////
	// 주의: 서비스 돌릴 때는 계정 찍지 말자, 20031103, kelovon
	///////////////////////////////////////////////////////////////////////////

	DBGOUT2(DO2_ALL, "[Character Info] - %s\n", szTime);
	ms_pIMIOCP->m_mCharacterName.lock();
	mt_map<string, CIMIOCPSocket* >::iterator itr3 = ms_pIMIOCP->m_mCharacterName.begin();
	while(itr3 != ms_pIMIOCP->m_mCharacterName.end())
	{
		UINT master = 0;
		CIMIOCPSocket *pSocke = itr3->second;
		if (pSocke 
			&& pSocke->IsUsing() 
			&& pSocke->GetIMParty() != NULL)
		{
			master = pSocke->m_pIMParty->GetMasterCharacterUniqueNumber();
		}

		DBGOUT2(DO2_ALL, "%s at %s: PartyMaster(%d)\n", itr3->first.c_str(), GET_MAP_STRING(pSocke->m_character.MapChannelIndex), master);
		itr3++;
	}
	ms_pIMIOCP->m_mCharacterName.unlock();

	DBGOUT2(DO2_ALL, "\n");
	///////////////////////////////////////////////////////////////////////////
	// END - DEBUG MESSAGE
	///////////////////////////////////////////////////////////////////////////
	PRINT_DEBUG_MESSAGE_LOCK.unlock();
}

BOOL CIMIOCPSocket::FilterChattingString(char *chatString)
{
	// check: 구현해야 함

	return TRUE;
}

BOOL CIMIOCPSocket::CheckCommandExecutability(USHORT i_nAllowedRace, BOOL i_bAllowAdminTool /* = FALSE */)
{
	BOOL bRet = COMPARE_RACE(m_character.Race, i_nAllowedRace)
				|| (i_bAllowAdminTool && m_PeerSocketType == ST_ADMIN_TOOL);

	return bRet;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIMIOCPSocket::HandleAdminCommands(char *cmd)
/// \brief		
/// \author		cmkwon
/// \date		2005-06-07 ~ 2005-06-07
/// \warning	
///
/// \param		
/// \return		TRUE	==> 처리가 완료된 명령어로 더이상의 처리는 없다.
///				FALSE	==> 처리가 완료 되지 않은 명령어로 Field Server로 전송되어서 추가로 처리됨
///////////////////////////////////////////////////////////////////////////////
BOOL CIMIOCPSocket::HandleAdminCommands(char *cmd)
{
	char *token;
	char seps[] = " ";
	char buffer[SIZE_MAX_CHAT_MESSAGE];

	util::strncpy(buffer, cmd, SIZE_MAX_CHAT_MESSAGE);
	token = strtok(buffer, seps);
	// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 추가 명령문 삭제
	if( CheckAdminCommand(token) )
		return FALSE;



	if ( stricmp(token, STRCMD_CS_COMMAND_DEBUGPRINTDBG) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_DEBUGPRINTDBG_1))
	{
		if ( !CheckCommandExecutability(RACE_OPERATION) )
		{// 2006-04-13 by cmkwon, 관리자만 사용 가능
			return FALSE;
		}

// 2008-02-14 by cmkwon, FieldServer 로 전송 되도록 FALSE 를 리턴한다.
// 		MessageType_t msgType = T_FI_ADMIN_PRINT_DEBUG_MSG;
// 		ms_pIMIOCP->SendMessageToFieldServer((BYTE*)&msgType, SIZE_FIELD_TYPE_HEADER);
// 
// #ifdef _DEBUG
// 		char *dpm = NULL;
// 		PRINT_DEBUG_MESSAGE(dpm);
// #endif
// 		return TRUE;
		return FALSE;	// 2008-02-14 by cmkwon, FieldServer 로 전송 되도록 FALSE 를 리턴한다.
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_DEBUGSETPARAMI) == 0)
	{
		if ( !CheckCommandExecutability(RACE_OPERATION, TRUE) )
		{
			// 운영자용, 일반 사용자는 무시
			return FALSE;
		}

		return TRUE;
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_WHO) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_WHO_1))
	{
		if (!CheckCommandExecutability(RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR, TRUE))
		{// 2006-04-13 by cmkwon, 관리자/운영자/SCMonitor/SCAdminTool만 사용 가능
			return FALSE;
		}

		// format: "/누구있지 [# of users]" - 현재 서버에 있는 사용자(맵 무관)를 모두 출력

		int nCount = 10;

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			nCount = atoi(token);
		}

		ms_pIMIOCP->m_MapChactarterUniqueNumber.lock();
		if(CheckCommandExecutability(RACE_OPERATION))
		{// 2007-11-08 by cmkwon, GM 명령어 정리 - 관리자만 전체 유저수를 전송한다.
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0029,
				ms_pIMIOCP->m_MapChactarterUniqueNumber.size(), nCount);
		}
		mt_map<UID32_t, CIMIOCPSocket*>::iterator itr3 = ms_pIMIOCP->m_MapChactarterUniqueNumber.begin();
		while(itr3 != ms_pIMIOCP->m_MapChactarterUniqueNumber.end())
		{
//			UINT master = 0;
//			if (itr3->second->m_pIMParty != NULL)
//			{
//				master = itr3->second->m_pIMParty->GetMasterCharacterUniqueNumber();
//			}

			CHARACTER *pCharacter = &itr3->second->m_character;
			SendString128(STRING_128_ADMIN_CMD, "%s(%s) at %s",
				pCharacter->CharacterName, pCharacter->AccountName,
				GET_MAP_STRING(pCharacter->MapChannelIndex));
			itr3++;
			if (--nCount <= 0) { break; }
		}
		ms_pIMIOCP->m_MapChactarterUniqueNumber.unlock();

		return TRUE;
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_REGISTERADMIN) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_REGISTERADMIN_1))
	{
		if ( !CheckCommandExecutability(RACE_OPERATION) )
		{
			// 운영자용, 일반 사용자는 무시
			return FALSE;
		}

		// format: "/운영등록" - 운영자에게 일정 이벤트 발생 시 오는 메세지를 받도록 서버에 등록함
		BOOL ret = ms_pIMIOCP->AddToAdminCharacter(m_character.CharacterUniqueNumber);

		if (ret)
		{
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0030);
		}
		else
		{
			ms_pIMIOCP->DeleteFromAdminCharacter(m_character.CharacterUniqueNumber);
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0031);
		}

		return TRUE;
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_DEBUGPRINTMSGI_0) == 0 
		|| stricmp(token, STRCMD_CS_COMMAND_DEBUGPRINTMSGI_1) == 0 )
	{
		if ( !CheckCommandExecutability(RACE_OPERATION) )
		{// 2006-04-13 by cmkwon, 관리자만 사용 가능
			return TRUE;
		}

		// format: "/messagei" - 클라이언트와 IM 서버가 주고 받는 프로토콜을 모두 출력함
		if (m_bSendMessgeTypeDBG)
		{
			m_bSendMessgeTypeDBG = FALSE;
		}
		else
		{
			m_bSendMessgeTypeDBG = TRUE;
		}

		return TRUE;
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_USERSINFOTOTAL) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_USERSINFOTOTAL_1))
	{
		if (!CheckCommandExecutability(RACE_OPERATION|RACE_MONITOR, TRUE))
		{// 2007-11-23 by cmkwon, GM 명령어 정리 - '/서버' 운영자 불가// 2006-04-13 by cmkwon, 관리자/운영자/SCMonitor/SCAdminTool만 사용 가능
			return FALSE;
		}

		// format: "/서버" - 서버의 정보를 출력함
		SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0032, ms_pIMIOCP->m_szLocalIPAddress);
		SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0033, g_pIMGlobal->GetServerGroupName(), ms_pIMIOCP->m_MapChactarterUniqueNumber.size());
		return FALSE;	// 필드서버에도 전송해야 하므로 FALSE를 return한다.
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_SERVERDOWN) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_SERVERDOWN_1))
	{
		if (!CheckCommandExecutability(RACE_OPERATION))
		{// 2006-04-13 by cmkwon, 관리자만 사용 가능
			return FALSE;
		}

		// format: "/서버다운 [인증숫자]" - 서버를 종료시킴
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_SERVERDOWN_HELP);
			return TRUE;
		}

		USHORT nTmpNumber = atoi(token);
		if (ms_pIMIOCP->m_nServerDownVerifyNumber != nTmpNumber)
		{
			ms_pIMIOCP->m_nServerDownVerifyNumber = RANDI(100, 999);
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0034,
				ms_pIMIOCP->m_szLocalIPAddress,
				ms_pIMIOCP->m_nServerDownVerifyNumber);
		}
		else
		{
			// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 필드서버 명령
			// PostMessage(g_pGlobal->GetMainWndHandle(), WM_CLOSE, 0, 0);
		}
		return TRUE;
	}
	else if ( strcmp(token, STRCMD_CS_COMMAND_LIST) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_LIST_1))
	{
		if (!CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_DEMO | RACE_MONITOR))
		{
			// 운영자용, 일반 사용자는 무시
			return FALSE;
		}

		// check: 요구하면 살리자, kelovon
//		SendString128(STRING_128_ADMIN_CMD, "서버 그룹 \'%s\'의 전체 접속자: %d명", g_szServerGroupName, ms_pIMIOCP->m_MapChactarterUniqueNumber.size());

		return FALSE;		// 필드서버에도 전송해야 하므로 FALSE를 return한다.
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_WHOAREYOU) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_WHOAREYOU_1))
	{
		// format: "/누구 [character name]"

		// 일반 사용자용
		token = strtok(NULL, seps);
		if (FALSE == CheckCommandExecutability(RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR, TRUE))
		{
			if(NULL == token)
			{
				SendString128(STRING_128_USER_NOTICE, STRCMD_CS_COMMAND_WHOAREYOU_HELP);
				return TRUE;
			}

			CIMIOCPSocket *pCharacSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(token);

			if (NULL == pCharacSocket
				|| FALSE == pCharacSocket->IsValidCharacter())
			{
				SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0035, token);
				return TRUE;
			}

			if (COMPARE_RACE(pCharacSocket->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
			{
				SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0035, token);
// 2006-11-14 by cmkwon, 위와 같이 구분 할 수 없도록 수정함
//				SendString128(STRING_128_USER_NOTICE, "Permission denied !!");
				return TRUE;
			}

			if (FALSE == COMPARE_INFLUENCE(m_character.InfluenceType, pCharacSocket->GetCharacter()->InfluenceType))
			{// 2006-11-13 by dhjin, 다른 세력간에는 사용 금지
				SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0035, token);
				return TRUE;
			}


			if(FALSE == IS_VALID_UNIQUE_NUMBER(pCharacSocket->m_uidGuildUniqueNumber))
			{
				SendString128(STRING_128_USER_NOTICE, "\'%s\', \'%s\'"
					, token
					, CAtumSJ::GetMapName(pCharacSocket->m_character.MapChannelIndex.MapIndex));
			}
			else
			{
				SendString128(STRING_128_USER_NOTICE, "\'%s\'(%s), \'%s\'"
					, token
					, pCharacSocket->m_character.GuildName
					, CAtumSJ::GetMapName(pCharacSocket->m_character.MapChannelIndex.MapIndex));
			}

			return TRUE;
		}

		// 운영자용
		if (token != NULL)
		{
			// 사용자가 명시된 경우
			char *CharacName = token;

			CIMIOCPSocket *pCharacSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(CharacName);
			if (NULL == pCharacSocket
				|| FALSE == pCharacSocket->IsValidCharacter())
			{
				SendString128(STRING_128_ADMIN_CMD, "%s NOT PLAYING", CharacName);
				return TRUE;
			}
			
			//SendString128(STRING_128_ADMIN_CMD, "%s PLAYING", CharacName, pCharacSocket->m_character.MapName);
			INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_GET_CHARACTER_INFO, T_FI_ADMIN_GET_CHARACTER_INFO, pMsgAdminGetCharInfo, pMsgAdminGetCharInfoBuf);
			pMsgAdminGetCharInfo->Type = ADMIN_GET_CHARAC_TYPE_SEND;
			pMsgAdminGetCharInfo->AdminCharacterUniqueNumber = m_character.CharacterUniqueNumber;
			pMsgAdminGetCharInfo->CharacterUniqueNumber = pCharacSocket->m_character.CharacterUniqueNumber;

			if (FALSE == pCharacSocket->SendMsgToCurrentFieldServerSocket(pMsgAdminGetCharInfoBuf, MSG_SIZE(MSG_FI_ADMIN_GET_CHARACTER_INFO)))
			{
				// current field server socket이 NULL이면 무시.
				char	szSystemLog[1024];
				sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
					GetCharacterString(&pCharacSocket->m_character, string()), STRCMD_CS_COMMAND_WHOAREYOU);
				DBGOUT(szSystemLog);
				g_pGlobal->WriteSystemLog(szSystemLog);

				return TRUE;	// 필드 서버로 추가 전송 필요 없음
			}
		}
		else
		{
			// 사용자가 명시되지 않은 경우
			int nCount = 10;
			ms_pIMIOCP->m_MapChactarterUniqueNumber.lock();
			//SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0036,
				//ms_pIMIOCP->m_MapChactarterUniqueNumber.size(), nCount);
			mt_map<UID32_t, CIMIOCPSocket*>::iterator itr3 = ms_pIMIOCP->m_MapChactarterUniqueNumber.begin();
			while(itr3 != ms_pIMIOCP->m_MapChactarterUniqueNumber.end())
			{
				CHARACTER *pCharacter = &itr3->second->m_character;
				SendString128(STRING_128_ADMIN_CMD, "%s(%s) at %s",
					pCharacter->CharacterName, pCharacter->AccountName,
					GET_MAP_STRING(pCharacter->MapChannelIndex));
				itr3++;
				if (--nCount <= 0) { break; }
			}
			ms_pIMIOCP->m_MapChactarterUniqueNumber.unlock();
		}

		return TRUE;
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_USERSEND) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_USERSEND_1))
	{
		if ( !CheckCommandExecutability(RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR) )
		{
			// 운영자용, 일반 사용자는 무시
			return FALSE;
		}

		// format: "/보내기 [character name] [map name]" - 캐릭터를 해당 맵으로 보냄
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_USERSEND_HELP);
			return TRUE;
		}
		char *CharacName = token;

		CIMIOCPSocket *pCharacSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(CharacName);

		if (NULL == pCharacSocket)
		{
			SendString128(STRING_128_ADMIN_CMD, "%s NOT PLAYING", CharacName);
			return TRUE;
		}
		
		INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_REGION, T_FI_CHAT_REGION, msgChatRegion, msgChatRegionBuf);
		msgChatRegion->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
		msgChatRegion->MessageLength = strlen(cmd) + 1;		// + 마지막 null 포함
		util::strncpy((char*)msgChatRegionBuf + MSG_SIZE(MSG_FI_CHAT_REGION), cmd, SIZE_MAX_CHAT_MESSAGE);

		if (FALSE == pCharacSocket->SendMsgToCurrentFieldServerSocket(msgChatRegionBuf, MSG_SIZE(MSG_FI_CHAT_REGION) + msgChatRegion->MessageLength))
		{
			// current field server socket이 NULL이면 무시.
			char	szSystemLog[1024];
			sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
				GetCharacterString(&pCharacSocket->m_character, string()), STRCMD_CS_COMMAND_USERSEND);
			DBGOUT(szSystemLog);
			g_pGlobal->WriteSystemLog(szSystemLog);
			return TRUE;	// 필드 서버로 추가 전송 필요 없음
		}
		return TRUE;
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_GOUSER) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_GOUSER_1))
	{
		if ( !CheckCommandExecutability(RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR) )
		{// 2008-09-09 by cmkwon, 모니터 권한 계정에 사용 권한 추가 - STRCMD_CS_COMMAND_GOUSER, // 2006-04-13 by cmkwon, 관리자/운영자만 사용 가능
			return FALSE;
		}

		// format: "/가기 [character name]" - 해당 캐릭터가 있는 곳으로 이동함
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_GOUSER_HELP);
			return TRUE;			
		}
		char *CharacName = token;

		CIMIOCPSocket *pCharacSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(CharacName);
		if (NULL == pCharacSocket)
		{
			SendString128(STRING_128_ADMIN_CMD, "%s NOT PLAYING", CharacName);
			return TRUE;		// 필드 서버로 추가 전송 필요 없음
		}

		INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_GET_CHARACTER_INFO, T_FI_ADMIN_GET_CHARACTER_INFO, pMsgAdminGetCharInfo, pMsgAdminGetCharInfoBuf);
		pMsgAdminGetCharInfo->Type = ADMIN_GET_CHARAC_TYPE_MOVETO;
		pMsgAdminGetCharInfo->AdminCharacterUniqueNumber = m_character.CharacterUniqueNumber;
		pMsgAdminGetCharInfo->CharacterUniqueNumber = pCharacSocket->m_character.CharacterUniqueNumber;

		if (FALSE == pCharacSocket->SendMsgToCurrentFieldServerSocket(pMsgAdminGetCharInfoBuf, MSG_SIZE(MSG_FI_ADMIN_GET_CHARACTER_INFO)))
		{
			// current field server socket이 NULL이면...
			char	szSystemLog[1024];
			sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
				GetCharacterString(&pCharacSocket->m_character, string()), STRCMD_CS_COMMAND_GOUSER);
			DBGOUT(szSystemLog);
			g_pGlobal->WriteSystemLog(szSystemLog);
			return TRUE;		// 필드 서버로 추가 전송 필요 없음
		}
		return TRUE;
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_COMEON) == 0)
	{
		// format: "/오기 [캐릭터명]" - 해당 캐릭터를 불러옴
		// MSG_IC_ADMIN_CALL_CHARACTER 로 처리
		return FALSE;
	}
// 2006-07-25 by cmkwon, 클라이언트에서 T_IC_ADMIN_CALL_GUILD 프로토콜로 보내줘야 한다.
// 	else if (stricmp(token, STRCMD_CS_COMMAND_GUILDCOMEON) == 0
// 		|| 0 == stricmp(token, STRCMD_CS_COMMAND_GUILDCOMEON_1))
// 	{
// 		if ( !CheckCommandExecutability(RACE_OPERATION|RACE_GAMEMASTER) )
// 		{
// 			// 운영자용, 일반 사용자는 무시
// 			return FALSE;
// 		}
// 
// 		// format: "/여단오기 [여단명]" - 해당 여단을 불러옴
// 		token = strtok(NULL, seps);
// 		if (token == NULL)
// 		{
// 			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_GUILDSEND_HELP);
// 			return TRUE;
// 		}
// 
// 		return FALSE;
// 	}
	else if (stricmp(token, STRCMD_CS_COMMAND_GUILDSEND) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_GUILDSEND_1))
	{
		if (!CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{
			// 운영자용, 일반 사용자는 무시
			return FALSE;
		}

		// format: "/여단보내기 [여단명] [map name]" - 캐릭터를 해당 맵으로 보냄
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_GUILDSEND_HELP);
			return TRUE;
		}

		char *szGuildName = token;
		char *szMapName = strtok(NULL, seps);
		if (szMapName == NULL)
		{// 해당 맵이 없음
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_IMSERVER_050607_0001);
			return TRUE;
		}

		ms_pIMIOCP->m_MapGuild.lock();
		{			
			CGuild *pGuild = ms_pIMIOCP->GetGuildByName(szGuildName);

			if (pGuild != NULL)
			{
				char szCmdBuffer[SIZE_MAX_CHAT_MESSAGE];
				listGuildMember::iterator itr = pGuild->m_ListGuildMember.begin();
				while (itr != pGuild->m_ListGuildMember.end())
				{
					CGuildMember *pGuildMember = &(*itr);

					CIMIOCPSocket *pCharacSocket = pGuildMember->m_pIMIOCPSocket;
					if (pCharacSocket == NULL || !pCharacSocket->IsUsing())
					{
						itr++;
						continue;
					}

					sprintf(szCmdBuffer, "/%s %s %s", STRCMD_CS_COMMAND_USERSEND, pCharacSocket->m_character.CharacterName, szMapName);

					INIT_MSG_WITH_BUFFER(MSG_FI_CHAT_REGION, T_FI_CHAT_REGION, msgChatRegion, msgChatRegionBuf);
					msgChatRegion->CharacterUniqueNumber = m_character.CharacterUniqueNumber;
					msgChatRegion->MessageLength = strlen(szCmdBuffer) + 1;		// + 마지막 null 포함
					util::strncpy((char*)msgChatRegionBuf + MSG_SIZE(MSG_FI_CHAT_REGION), szCmdBuffer, SIZE_MAX_CHAT_MESSAGE);

					if (FALSE == pCharacSocket->SendMsgToCurrentFieldServerSocket(msgChatRegionBuf, MSG_SIZE(MSG_FI_CHAT_REGION) + msgChatRegion->MessageLength))
					{
						// current field server socket이 NULL이면 무시.
						char	szSystemLog[1024];
						sprintf(szSystemLog, "  Fatal Error: CurrentFieldServerSocket is NULL, %s at %s\r\n",
							GetCharacterString(&pCharacSocket->m_character, string()), STRCMD_CS_COMMAND_GUILDSEND);
						DBGOUT(szSystemLog);
						g_pGlobal->WriteSystemLog(szSystemLog);

						itr++;
						continue;
					}

					itr++;
				}// while_end
			}
			else
			{
				SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0038);
			}
		}// lock_end
		ms_pIMIOCP->m_MapGuild.unlock();			
		return TRUE;
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_CHATPTOPFLAG) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_CHATPTOPFLAG_1))
	{
		// format: "/귓말" - 귓말을 받지 않음, toggle됨
		// 모두 허용함

		// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//		if (FALSE == COMPARE_CHATTYPE_BIT(m_enableChatFlag8, CHAT_TYPE_PTOP))
		if (FALSE == COMPARE_CHATTYPE_BIT(m_enableChatFlag32, CHAT_TYPE_PTOP))
		{
			// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//			SET_CHATTYPE_BIT(m_enableChatFlag8, CHAT_TYPE_PTOP);
			SET_CHATTYPE_BIT(m_enableChatFlag32, CHAT_TYPE_PTOP);
			this->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0039);
		}
		else
		{
			// 2008-05-15 by dhjin, EP3 - 채팅 시스템 변경
//			CLEAR_CHATTYPE_BIT(m_enableChatFlag8, CHAT_TYPE_PTOP);
			CLEAR_CHATTYPE_BIT(m_enableChatFlag32, CHAT_TYPE_PTOP);
			this->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0040);
		}
		this->SendToFieldServerChatBitFlag();
		return TRUE;
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_GUILDINFO) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_GUILDINFO_1))
	{
		// format: "/길드" - 길드 정보 출력
		if (!CheckCommandExecutability(RACE_OPERATION))
		{// 2006-04-13 by cmkwon, 관리자만 사용 가능
			return FALSE;
		}

		if (FALSE == IS_VALID_UNIQUE_NUMBER(m_uidGuildUniqueNumber))
		{
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0041);
		}
		else
		{
			ms_pIMIOCP->m_MapGuild.lock();
			{
				CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
				if(ptmGuild)
				{
					ptmGuild->PrintGuildInfo(this);
				}
				else
				{
					SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0042, m_uidGuildUniqueNumber);
				}
			}
			ms_pIMIOCP->m_MapGuild.unlock();
		}

		return TRUE;
	}
	else if ( stricmp(token, STRCMD_CS_COMMAND_WEATHERSET) == 0
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_1))
	{
		// format: "/날씨 [기본|맑음|비|눈|흐림|안개] [전체|맵이름] [on|off]" - 날씨 조정

		if (!CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER|RACE_MONITOR, TRUE))
		{// 2006-04-13 by cmkwon, 관리자/SCAdminTool만 사용 가능
			return FALSE;
		}

		INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_CHANGE_WEATHER, T_FI_ADMIN_CHANGE_WEATHER, pAdminChangerWeather, pAdminChangerWeatherBuf);
		memset(pAdminChangerWeather, sizeof(MSG_FI_ADMIN_CHANGE_WEATHER), 0x00);

		CIMIOCPSocket *pFieldServerSocket = NULL;
		BOOL bAllMaps = FALSE;					// 전체 맵 설정이면 TRUE

		// 날씨 종류
		token = strtok(NULL, seps);
		if (token == NULL)
		{
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_WEATHERSET_HELP);
			return TRUE;
		}
		
		if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P1NORMAL)==0)		// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
		{
			pAdminChangerWeather->MapWeather = WEATHER_DEFAULT;
		}
		else if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P1FINE)==0)	// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
		{
			pAdminChangerWeather->MapWeather = WEATHER_SUNNY;
		}
		else if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P1RAIN)==0)	// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
		{
			pAdminChangerWeather->MapWeather = WEATHER_RAINY;
		}
		else if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P1SNOW)==0)	// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
		{
			pAdminChangerWeather->MapWeather = WEATHER_SNOWY;
		}
		else if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P1CLOUDY)==0)	// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
		{
			pAdminChangerWeather->MapWeather = WEATHER_CLOUDY;
		}
		else if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P1FOG)==0)		// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
		{
			pAdminChangerWeather->MapWeather = WEATHER_FOGGY;
		}
		else
		{
			SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0043, token);
			return TRUE;		// 필드 서버로 추가 전송 필요 없음
		}

		// map 이름
		token = strtok(NULL, seps);
		if (token != NULL)
		{
			if(NULL == ms_pIMIOCP->m_pFieldServerSocket || FALSE == ms_pIMIOCP->m_pFieldServerSocket->IsUsing())
			{
				SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0044);
				return TRUE;		// 필드 서버로 추가 전송 필요 없음
			}

			if (stricmp(STRCMD_CS_COMMAND_WEATHERSET_P2ALL, token) == 0)	// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
			{
				bAllMaps = TRUE;
			}
			else
			{
				if( FALSE == ms_pIMIOCP->m_FieldServerInfo.IsExistMapIndex(atoi(token)) )
				{
					SendString128(STRING_128_ADMIN_CMD, STRMSG_S_I2NOTIFY_0045, token);
					return TRUE;		// 필드 서버로 추가 전송 필요 없음
				}

				pFieldServerSocket = ms_pIMIOCP->m_pFieldServerSocket;
			}
			pAdminChangerWeather->MapIndex = atoi(token);
		}
		else
		{
			pFieldServerSocket = m_pCurrentFieldServerSocket;
			pAdminChangerWeather->MapIndex = m_character.MapChannelIndex.MapIndex;
		}

		// on|off
		token = strtok(NULL, seps);
		if (token != NULL)
		{
			if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P3ON)==0)			// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
			{
				pAdminChangerWeather->OnOff = TRUE;
			}
			else if (stricmp(token, STRCMD_CS_COMMAND_WEATHERSET_P3OFF)==0)		// 2006-12-11 by cmkwon, 함수변경(strcmp->stricmp)
			{
				pAdminChangerWeather->OnOff = FALSE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			pAdminChangerWeather->OnOff = TRUE;
		}

		if (bAllMaps)
		{
			// 전체 맵의 날씨를 변경
			pAdminChangerWeather->MapIndex		= MAP_INDEX_ALL;			// 2006-12-11 by cmkwon
			ms_pIMIOCP->SendMessageToFieldServer(pAdminChangerWeatherBuf, MSG_SIZE(MSG_FI_ADMIN_CHANGE_WEATHER));
		}
		else
		{
			// 특정 맵의 날씨를 변경
#ifdef _DEBUG
			assert(pFieldServerSocket != NULL);
#endif
			if (pFieldServerSocket == NULL)
			{
				g_pGlobal->WriteSystemLogEX(TRUE, "FATAL ERROR: NULL pFieldServerSocket(%s)\r\n", cmd);
				return TRUE;		// 필드 서버로 추가 전송 필요 없음
			}

			pFieldServerSocket->SendAddData(pAdminChangerWeatherBuf, MSG_SIZE(MSG_FI_ADMIN_CHANGE_WEATHER));
		}

		return TRUE;		// 필드 서버로 추가 전송 필요 없음
	}
	else if (0 == stricmp(token, STRCMD_CS_COMMAND_CHATFORBID)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_CHATFORBID_1))
	{
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR, TRUE))
		{// 2006-04-13 by cmkwon, 관리자/운영자/SCAdminTool만 사용 가능
			return FALSE;
		}

		// format: "/채금 [character name] [시간(분)]" - 채팅금지 설정하기
		token = strtok(NULL, seps);
		if (NULL == token)
		{// CharacterName 이 없음
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_CHATFORBID_HELP);
			return TRUE;		// 필드 서버로 추가 전송 필요 없음
		}

		char szCharName[SIZE_MAX_CHARACTER_NAME];
		int  nMinutes = 1;
		util::strncpy(szCharName, token, SIZE_MAX_CHARACTER_NAME);

		token = strtok(NULL, seps);
		if(token)
		{
			nMinutes = max(nMinutes, atoi(token));
		}
		
		CIMIOCPSocket *pIISock = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(szCharName);
		if(pIISock
			&& pIISock->IsValidCharacter())
		{
			pIISock->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0046, nMinutes);
		}
		SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0047, szCharName, nMinutes);
		ms_pIMIOCP->m_chatBlockManager.InsertChatBlockList(szCharName, nMinutes);
		return TRUE;
	}
	else if (0 == stricmp(token, STRCMD_CS_COMMAND_CHATFORBIDRELEASE)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_1))
	{
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR, TRUE))
		{// 2006-04-13 by cmkwon, 관리자/운영자/SCAdminTool만 사용 가능
			return FALSE;
		}

		// format: "/채금해제 [character name]" - 채팅금지 해제하기
		token = strtok(NULL, seps);
		if (NULL == token)
		{// CharacterName 이 없음
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_HELP);
			return TRUE;
		}

		CIMIOCPSocket *pIISock = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(token);
		if(pIISock)
		{
			pIISock->SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0048);
		}

		SendString128(STRING_128_USER_NOTICE, STRMSG_S_I2NOTIFY_0049, token);
		ms_pIMIOCP->m_chatBlockManager.DeleteChatBlockList(token);
		return TRUE;
	}
	else if (stricmp(token, STRCMD_CS_COMMAND_COMMANDLIST_0) == 0
		|| stricmp(token, STRCMD_CS_COMMAND_COMMANDLIST_1) == 0
		|| stricmp(token, STRCMD_CS_COMMAND_COMMANDLIST_2) == 0)
	{
		// format: "/?" - 명령어 리스트 출력
		
		// 2007-11-08 by cmkwon, GM 명령어 정리 - 일반유저 명령어 리스트 전송
		int i;
		for (i = 0; i < ms_pIMIOCP->m_vectstringCommandListforUser.size(); i++)
		{
			SendString128(STRING_128_USER_NOTICE, "%s", ms_pIMIOCP->m_vectstringCommandListforUser[i].c_str());
		}
		if (FALSE == CheckCommandExecutability(RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR))
		{// 2007-11-08 by cmkwon, 일반 유저는 여기에서 리턴
			return TRUE;
		}

		// 2007-11-08 by cmkwon, GM 명령어 정리 - 운영자 명령어 리스트 전송
		SendString128(STRING_128_USER_NOTICE, "[%s] command list.", GetRaceString(RACE_GAMEMASTER));
		for (i = 0; i < ms_pIMIOCP->m_vectstringGameMasterCommandList.size(); i++)
		{
			SendString128(STRING_128_USER_NOTICE, "%s", ms_pIMIOCP->m_vectstringGameMasterCommandList[i].c_str());
		}
		if(FALSE == CheckCommandExecutability(RACE_OPERATION))
		{// 2007-11-08 by cmkwon, 운영자는 여기에서 리턴
			return TRUE;
		}

		// 2007-11-08 by cmkwon, GM 명령어 정리 - 관리자 명령어 리스트 전송
		SendString128(STRING_128_USER_NOTICE, "[%s] command list.", GetRaceString(RACE_OPERATION));
		for (i = 0; i < ms_pIMIOCP->m_vectstringCommandList.size(); i++)
		{
			SendString128(STRING_128_USER_NOTICE, "%s", ms_pIMIOCP->m_vectstringCommandList[i].c_str());
		}
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_CALLGM_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_CALLGM_1)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_CALLGM_2))
	{
		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 시스템 켜진 상태 체크
		if(FALSE == ms_pIMIOCP->IsOnCallGMSystem())
		{
			this->SendString128(STRING_128_USER_NOTICE, STRMSG_071120_0001);
			return TRUE;
		}

		// "format: /도우미 [상당내용] - GM과 상담을 신청한다."
		token = strtok(NULL, "");
		if(NULL == token)
		{
			this->SendString128(STRING_128_USER_NOTICE, STRCMD_CS_COMMAND_CALLGM_HELP);
			return TRUE;
		}

		// 2007-11-19 by cmkwon, 진정시스템 업데이트 - CIMIOCP::CallGMInsertRequest() 함수에 세력 인자 추가
		int nReqCnt = ms_pIMIOCP->CallGMInsertRequest(m_character.CharacterUniqueNumber, m_character.InfluenceType, m_character.CharacterName, token);
		
		if(0 >= nReqCnt)
		{
			this->SendString128(STRING_128_USER_NOTICE, "CallGM overflow !!");
			return TRUE;
		}

		// 2006-05-26 by cmkwon, temporay in english
		this->SendString128(STRING_128_USER_NOTICE, STRMSG_060526_0000);

		INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_CALLGM_INFO_OK, T_IC_ADMIN_CALLGM_INFO_OK, pCallGMInfo, SendBuf);
		pCallGMInfo->nCallGMTotalCount	= nReqCnt;
		SendAddData(SendBuf, MSG_SIZE(MSG_IC_ADMIN_CALLGM_INFO_OK));		
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_VIEWCALLGM_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_VIEWCALLGM_1)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_VIEWCALLGM_2))
	{// 2006-05-08 by cmkwon
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2006-05-08 by cmkwon, 관리자/운영자만 사용 가능
			return TRUE;
		}

		// "format: /도우미보기 [|개수(1~10)] - GM 상담 요청 리스트를 지정한 개수 만큼 살펴본다"
		int nReqCount = SIZE_MAX_CALLGM_BRING_COUNT;
		token = strtok(NULL, seps);
		if(token)
		{
			nReqCount = atoi(token);
			nReqCount = min(nReqCount, SIZE_MAX_CALLGM_BRING_COUNT);
			nReqCount = max(nReqCount, 1);
		}

		INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_CALLGM_VIEW_OK, T_IC_ADMIN_CALLGM_VIEW_OK, pViewOK, SendBuf);
		if(ms_pIMIOCP->CallGMMake_MSG_IC_ADMIN_CALLGM_VIEW_OK(pViewOK, nReqCount))
		{
			SendAddData(SendBuf, MSG_SIZE(MSG_IC_ADMIN_CALLGM_VIEW_OK) + sizeof(SCALLGM)*pViewOK->nCallGMCount);
		}
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_BRINGCALLGM_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_BRINGCALLGM_1)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_BRINGCALLGM_2))
	{// 2006-05-08 by cmkwon
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2006-05-08 by cmkwon, 관리자/운영자만 사용 가능
			return TRUE;
		}

		// "format: /도우미가져오기 [|개수(1~10)] - GM 상담 요청 리스트를 지정한 개수 만큼 가져온다(서버에서 삭제됨)"
		int nReqCount = SIZE_MAX_CALLGM_BRING_COUNT;
		token = strtok(NULL, seps);
		if(token)
		{
			nReqCount = atoi(token);
			nReqCount = min(nReqCount, SIZE_MAX_CALLGM_BRING_COUNT);
			nReqCount = max(nReqCount, 1);
		}

		INIT_MSG_WITH_BUFFER(MSG_IC_ADMIN_CALLGM_BRING_OK, T_IC_ADMIN_CALLGM_BRING_OK, pBringOK, SendBuf);
		if(ms_pIMIOCP->CallGMMake_MSG_IC_ADMIN_CALLGM_BRING_OK(pBringOK, nReqCount))
		{
			SendAddData(SendBuf, MSG_SIZE(MSG_IC_ADMIN_CALLGM_BRING_OK) + sizeof(SCALLGM)*pBringOK->nCallGMCount);
		}
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_1)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_2))
	{// 2007-08-27 by cmkwon, 서버다운준비 명령어 추가(SCAdminTool에서 SCMonitor의 PrepareShutdown을 진행 할 수 있게)
		if (FALSE == CheckCommandExecutability(RACE_OPERATION, TRUE) )
		{// 2008-08-18 by dhjin, 관리자만 사용가능 // 관리자/운영자/SCAdminTool만 사용 가능
			return TRUE;
		}
		
		// "format: /[서버다운준비|PrepareShutdown|PrepareShutdown] [Start|Release] - 서버 종료 준비, 모든 유저 연결 강제 종료됨"
		token = strtok(NULL, seps);
		if (NULL == token)
		{// Parma1 없음
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_PrepareShutdown_HELP);
			return TRUE;
		}

		INIT_MSG_WITH_BUFFER(MSG_FI_CONNECT_PREPARE_SHUTDOWN, T_FI_CONNECT_PREPARE_SHUTDOWN, pPreShutdown, SendBuf);
		if(0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_P1Start))
		{
			// 2011-11-18 by shcho, 서버다운 프리페어서버다운 제거 처리 - 커맨드 처리 제거
			// pPreShutdown->bPrepareShutdown		= TRUE;		// TURE면 서버다운 준비 시작
		}
		else if(0 == stricmp(token, STRCMD_CS_COMMAND_PrepareShutdown_P1Release))
		{
			pPreShutdown->bPrepareShutdown		= FALSE;	// FALSE이면 서버다운 준비 해제
		}
		else
		{
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_PrepareShutdown_HELP);
			return TRUE;
		}		
		
		// FieldServer로 전송한다.
		ms_pIMIOCP->SendMessageToFieldServer(SendBuf, MSG_SIZE(MSG_FI_CONNECT_PREPARE_SHUTDOWN));
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_STARTCALLGM_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_STARTCALLGM_1)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_STARTCALLGM_2))
	{// 2007-11-19 by cmkwon, 진정시스템 업데이트 - '/도우미시작' 명령어 처리
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2007-11-19 by cmkwon, 관리자/운영자만 사용 가능
			return TRUE;
		}

		// "format: /도우미시작 [|시간(단위:분)] - 진정 시스템 시작"
		int nTermMinute = 20160;		// 2007-11-19 by cmkwon, 20160분 = 336시간 = 14일
		token = strtok(NULL, seps);
		if(token)
		{
			if(0 >= atoi(token))
			{
				SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_STARTCALLGM_HELP);
				return TRUE;				
			}

			nTermMinute = atoi(token);
		}

		ATUM_DATE_TIME atSTime { true };
		ATUM_DATE_TIME atETime;

		atETime = atSTime;
		atETime.AddDateTime(0, 0, 0, 0, nTermMinute);
		atETime.Second = 0;

		ms_pIMIOCP->SetCallGMSystemTime(&atSTime, &atETime);
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_ENDCALLGM_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_ENDCALLGM_1)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_ENDCALLGM_2))
	{// 2007-11-19 by cmkwon, 진정시스템 업데이트 - '/도우미종료' 명령어 처리
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2007-11-19 by cmkwon, 관리자/운영자만 사용 가능
			return TRUE;
		}

		// "format: /도우미종료 - 진정 시스템 종료"
		ATUM_DATE_TIME atSTime;
		ATUM_DATE_TIME atETime;

		atSTime = ms_pIMIOCP->m_atCallGMStartTime;
		atETime.SetCurrentDateTime(TRUE);
		ms_pIMIOCP->SetCallGMSystemTime(&atSTime, &atETime);
		return TRUE;
	}
	else if(0 == stricmp(token, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_0)
		|| 0 == stricmp(token, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_1))
	{// 2008-12-30 by cmkwon, 지도자 채팅 제한 카드 구현 - 지도자채금해제 명령어 처리
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{// 2008-12-30 by cmkwon, 관리자/운영자만 사용 가능
			return TRUE;
		}

		// "format: /지도자채금해제 [CharacterName] - 지도자 채금 설정이 해제된다."
		token = strtok(NULL, seps);
		if (NULL == token)
		{// CharacterName 이 없음
			SendString128(STRING_128_ADMIN_CMD, STRCMD_CS_COMMAND_CHATFORBIDRELEASE_LEADER_HELP);
			return TRUE;
		}

		int nRemainMinutes = 0;
		if(FALSE == ms_pIMIOCP->m_chatBlockManagerByLeader.IsExistChatBlockList(&nRemainMinutes, token))
		{
			SendString128(STRING_128_ADMIN_CMD, "The CharacterName isn't in blocked list.");
			return TRUE;
		}

		// 2008-12-31 by cmkwon, 채금 리스트에서 삭제
		ms_pIMIOCP->m_chatBlockManagerByLeader.DeleteChatBlockList(token);

		// 2008-12-31 by cmkwon, 접속 중인 유저면 메시지 전송
		CIMIOCPSocket *pIISock = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(token);
		if(pIISock && pIISock->IsValidCharacter())
		{
			pIISock->SendString128(STRING_128_USER_NOTICE, STRMSG_081230_0003);
		}
		
		return TRUE;
	}
	else if (0 == stricmp(token, STRCMD_CS_COMMAND_TOGGLE_CNC))
	{
		if (FALSE == CheckCommandExecutability(RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
		{
			return TRUE;
		}
		ms_pIMIOCP->m_bEnableCNC = !ms_pIMIOCP->m_bEnableCNC;

		if (ms_pIMIOCP->m_bEnableCNC)
		{
			SendString128(STRING_128_ADMIN_CMD, "Cross Nation Chat is now \\gENABLED\\g.");
		}
		else
		{
			SendString128(STRING_128_ADMIN_CMD, "Cross Nation Chat is now \\rDISABLED\\r.");
		}
		return TRUE;
	}

	return FALSE;
}

// error 메세지를 전송
void CIMIOCPSocket::SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1, int errParam2, char* errMsg, BOOL bCloseConnection)
{
	MSG_ERROR	*pMsgError;
	BYTE		SendBuf[SIZE_MAX_PACKET];

	char szSystemLog[256];
	sprintf(szSystemLog, "  SendErrorMsg to %s %s ==> %s(%#04x) AT %s Param1(%d) Param2(%d) Msg(%s)\r\n",
		GetCharacterString(&m_character, string()), m_szPeerIP,
		GetErrorString(err), err, GetProtocolTypeString(msgType), errParam1, errParam2, errMsg);
	g_pIMGlobal->WriteSystemLog(szSystemLog);
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
	if (errMsg == NULL)
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

BOOL CIMIOCPSocket::IsExistPremiumCard()
{
	if(FALSE != m_MembershipInfo.nCardItemNum1)
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::SendFieldCreateGuildBySubLeader()
/// \brief		부지도자가 여단 생성시 필드서버로 정보 전송
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::SendFieldCreateGuildBySubLeader()
{
	if (FALSE == this->IsUsing())
	{
		return;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_CREATE_GUILD_BY_SUBLEADER, T_FI_CREATE_GUILD_BY_SUBLEADER, SendMsg, SendBuf);
	SendMsg->Influence			= this->m_character.InfluenceType;
	SendMsg->CharacterUID		= this->m_character.CharacterUniqueNumber;
	if(ms_pIMIOCP->CheckInfluenceSubLeader1(this->m_character.InfluenceType, this->m_character.CharacterUniqueNumber))
	{
		SendMsg->SubLeaderRank	= 1;
	}
	else if(ms_pIMIOCP->CheckInfluenceSubLeader2(this->m_character.InfluenceType, this->m_character.CharacterUniqueNumber))
	{
		SendMsg->SubLeaderRank	= 2;
	}
	else
	{
		return;
	}

	this->SendMsgToCurrentFieldServerSocket(SendBuf, MSG_SIZE(MSG_FI_CREATE_GUILD_BY_SUBLEADER));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::SendPartyListInfo(PartyID_t i_nStartNum)
/// \brief		EP3 편대 수정 - 편대 정보 리스트
/// \author		dhjin
/// \date		2008-06-02 ~ 2008-06-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::SendPartyListInfo(PartyID_t i_nStartNum)
{
	INIT_MSG_WITH_BUFFER(MSG_IC_PARTY_LIST_INFO_OK, T_IC_PARTY_LIST_INFO_OK, pSMsg, SendBuf);
	pSMsg->PartyInfoListCount	= this->MakeMSG_IC_PARTY_LIST_INFO_OK((SPARTY_LIST_INFO*)(SendBuf+MSG_SIZE(MSG_IC_PARTY_LIST_INFO_OK)), i_nStartNum, &pSMsg->PartyInfoTotalCount);
	if(0 >= pSMsg->PartyInfoListCount)
	{
		this->SendErrorMessage(T_IC_PARTY_LIST_INFO, ERR_PROTOCOL_NO_SUCH_PARTY);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-02 by dhjin, 편대 목록 전송
	this->SendAddData(SendBuf, MSG_SIZE(MSG_IC_PARTY_LIST_INFO_OK)+pSMsg->PartyInfoListCount*sizeof(SPARTY_LIST_INFO));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	 CIMIOCPSocket::MakeMSG_IC_PARTY_LIST_INFO_OK(SPARTY_LIST_INFO * o_pPartyListInfo, PartyID_t i_nStartNum, INT * o_nTotalCount)
/// \brief		EP3 편대 수정 - 편대 정보 리스트	
/// \author		dhjin
/// \date		2008-06-02 ~ 2008-06-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	 CIMIOCPSocket::MakeMSG_IC_PARTY_LIST_INFO_OK(SPARTY_LIST_INFO * o_pPartyListInfo, PartyID_t i_nStartNum, INT * o_nTotalCount)
{
	INT	nPartyListCount	= 0;
	INT	Count = 0;
	
	ms_pIMIOCP->m_MapParties.lock();

	if(ms_pIMIOCP->m_MapParties.empty())
	{
		ms_pIMIOCP->m_MapParties.unlock();
		return nPartyListCount;
	}
	
	mt_map<PartyID_t, CIMParty*>::iterator itr = ms_pIMIOCP->m_MapParties.begin();
	for(; itr != ms_pIMIOCP->m_MapParties.end(); itr++)
	{
		if(this->m_character.InfluenceType != itr->second->m_SPartyInfo.PartyInfluence
			|| PARTY_JOIN_TYPE_INVITE_MASTER == itr->second->m_SPartyInfo.PartyJoinType)
		{
			continue;
		}

		(*o_nTotalCount)++;
	}

	itr = ms_pIMIOCP->m_MapParties.begin();
	for(; itr != ms_pIMIOCP->m_MapParties.end(); itr++)
	{
		if(i_nStartNum >= itr->first
			|| Count >= 13		// 2008-07-18 by cmkwon, 임시로 버그 수정
			|| this->m_character.InfluenceType != itr->second->m_SPartyInfo.PartyInfluence
			|| PARTY_JOIN_TYPE_INVITE_MASTER == itr->second->m_SPartyInfo.PartyJoinType)
		{
			continue;
		}

		//////////////////////////////////////////////////////////////////////
		// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 IM 서버는 팀번호가 같은 파티들만 리스트업 한다.
		if(m_bArenaIMServerCheck)
		{
			CIMIOCPSocket *MasterIOCP = itr->second->GetMasterIMIOCPSocket();
			if( NULL == MasterIOCP )
			{
				continue;
			}
			if(m_nTeamNum != MasterIOCP->m_nTeamNum)
			{
				continue;
			}
		}
		// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 아레나 IM 서버는 팀번호가 같은 파티들만 리스트업 한다.

		o_pPartyListInfo[nPartyListCount].PartyNum				=   itr->first;
		o_pPartyListInfo[nPartyListCount].ExpDistributeType		=	itr->second->m_SPartyInfo.ExpDistributeType;
		o_pPartyListInfo[nPartyListCount].ItemDistributeType	=	itr->second->m_SPartyInfo.ItemDistributeType;
		o_pPartyListInfo[nPartyListCount].MinLevel				=	itr->second->m_SPartyInfo.MinLevel;	
		o_pPartyListInfo[nPartyListCount].MaxLevel				=	itr->second->m_SPartyInfo.MaxLevel;
		o_pPartyListInfo[nPartyListCount].PartyLock				=	itr->second->m_SPartyInfo.PartyLock;
		util::strncpy(o_pPartyListInfo[nPartyListCount].PartyName, itr->second->m_SPartyInfo.PartyName, SIZE_MAX_PARTY_NAME);
		util::strncpy(o_pPartyListInfo[nPartyListCount].PartyPW,	itr->second->m_SPartyInfo.PartyPW, SIZE_MAX_TEAM_PW);
		util::strncpy(o_pPartyListInfo[nPartyListCount].PartyMasterName,	itr->second->GetMasterCharacterName(), SIZE_MAX_CHARACTER_NAME);
		o_pPartyListInfo[nPartyListCount].Membercount			=	itr->second->GetMemberCount(FALSE);
		o_pPartyListInfo[nPartyListCount].VoipCheck				=	itr->second->CheckVoipUse();

		nPartyListCount++;
		Count++;
	}
	ms_pIMIOCP->m_MapParties.unlock();	

	return nPartyListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::SendVoipType(EN_CHECK_TYPE i_NewVoipType)
/// \brief		EP3 VOIP -
/// \author		dhjin
/// \date		2008-06-17 ~ 2008-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CIMIOCPSocket::SendVoipType(EN_CHECK_TYPE i_NewVoipType)
//{
//	switch(i_NewVoipType)
//	{
//	case CHECK_TYPE_VOIP_PARTY:
//		{
//			CIMParty *ptmParty = this->GetIMParty();
//			if (ptmParty == NULL)
//			{
//				SendErrorMessage(T_IC_VOIP_SET, ERR_PROTOCOL_NO_SUCH_PARTY);
//				return;
//			}
//			ptmParty->LockIMParty();
//			INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_SET_OK, T_IC_VOIP_SET_OK, pSendMsg, pSendBuf);
//			pSendMsg->CharacterUID	= this->m_character.CharacterUniqueNumber;
//			pSendMsg->VoipType		= CHECK_TYPE_VOIP_PARTY;
//			ptmParty->SendMsgToMembers(pSendBuf, MSG_SIZE(MSG_IC_VOIP_SET_OK));
//			ptmParty->UnlockIMParty();			
//		}
//		break;
//	case CHECK_TYPE_VOIP_GUILD:
//		{
//			ms_pIMIOCP->m_MapGuild.lock();
//
//			CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//			if(NULL == ptmGuild)
//			{
//				ms_pIMIOCP->m_MapGuild.unlock();
//				SendErrorMessage(T_IC_VOIP_SET, ERR_PROTOCOL_NOT_HAVE_GUILD);
//				return;
//			}
//			
////			if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
////			{
////				ms_pIMIOCP->m_MapGuild.unlock();				
////				SendErrorMessage(T_IC_VOIP_SET, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
////				return;
////			}
//
//			ptmGuild->SetMemberVoipType(m_character.CharacterUniqueNumber, CHECK_TYPE_VOIP_GUILD);				// 2008-07-16 by dhjin, EP3 - Voip 정보
//			
//			INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_SET_OK, T_IC_VOIP_SET_OK, pSMsg, pBuffer);
//			pSMsg->CharacterUID	= this->m_character.CharacterUniqueNumber;
//			pSMsg->VoipType		= CHECK_TYPE_VOIP_GUILD;
//			ptmGuild->SendMsgToGuildMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_SET_OK));
//
//			ms_pIMIOCP->m_MapGuild.unlock();
//		}
//		break;
//	default:
//		{
//		}
//	}
//	
//	if(CHECK_TYPE_VOIP_NONE == i_NewVoipType)
//	{
//		switch(this->m_VoipType)
//		{
//		case CHECK_TYPE_VOIP_PARTY:
//			{
//				CIMParty *ptmParty = this->GetIMParty();
//				if (ptmParty == NULL)
//				{
//					SendErrorMessage(T_IC_VOIP_SET, ERR_PROTOCOL_NO_SUCH_PARTY);
//					return;
//				}
//				ptmParty->LockIMParty();
//				INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_SET_OK, T_IC_VOIP_SET_OK, pSendMsg, pSendBuf);
//				pSendMsg->CharacterUID	= this->m_character.CharacterUniqueNumber;
//				pSendMsg->VoipType		= CHECK_TYPE_VOIP_NONE;
//				ptmParty->SendMsgToMembers(pSendBuf, MSG_SIZE(MSG_IC_VOIP_SET_OK));
//				ptmParty->UnlockIMParty();			
//			}
//			break;
//		case CHECK_TYPE_VOIP_GUILD:
//			{
//				ms_pIMIOCP->m_MapGuild.lock();
//
//				CGuild *ptmGuild = ms_pIMIOCP->m_MapGuild.findLock(m_uidGuildUniqueNumber);
//				if(NULL == ptmGuild)
//				{
//					ms_pIMIOCP->m_MapGuild.unlock();
//					SendErrorMessage(T_IC_VOIP_SET, ERR_PROTOCOL_NOT_HAVE_GUILD);
//					return;
//				}
//				
////				if (m_character.CharacterUniqueNumber != ptmGuild->m_CommanderUniqueNumber)
////				{
////					ms_pIMIOCP->m_MapGuild.unlock();				
////					SendErrorMessage(T_IC_VOIP_SET, ERR_PROTOCOL_GUILD_PERMISSION_DENIED);
////					return;
////				}
//				
//				INIT_MSG_WITH_BUFFER(MSG_IC_VOIP_SET_OK, T_IC_VOIP_SET_OK, pSMsg, pBuffer);
//				pSMsg->CharacterUID	= this->m_character.CharacterUniqueNumber;
//				pSMsg->VoipType		= CHECK_TYPE_VOIP_NONE;
//				ptmGuild->SendMsgToGuildMembers(pBuffer, MSG_SIZE(MSG_IC_VOIP_SET_OK));
//
//				ms_pIMIOCP->m_MapGuild.unlock();
//			}
//			break;
//		default:
//			{
//			}
//		}		
//	}
//
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIMIOCPSocket::SendChatRoomListInfo(INT i_nStartNum)
/// \brief		EP3 채팅방 -
/// \author		dhjin
/// \date		2008-06-17 ~ 2008-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::SendChatRoomListInfo(INT i_nStartNum)
{
	INIT_MSG_WITH_BUFFER(MSG_IC_CHATROOM_LIST_INFO_OK, T_IC_CHATROOM_LIST_INFO_OK, pSMsg, SendBuf);
	pSMsg->ChatRoomInfoListCount	= this->MakeMSG_IC_CHATROOM_LIST_INFO_OK((SCHATROOM_LIST_INFO*)(SendBuf+MSG_SIZE(MSG_IC_CHATROOM_LIST_INFO_OK)), i_nStartNum, &pSMsg->ChatRoomInfoTotalCount);
	if(0 >= pSMsg->ChatRoomInfoListCount)
	{
		this->SendErrorMessage(T_IC_CHATROOM_LIST_INFO, ERR_CHATROOM_NO_SUCH);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-02 by dhjin, 편대 목록 전송
	this->SendAddData(SendBuf, MSG_SIZE(MSG_IC_CHATROOM_LIST_INFO_OK)+pSMsg->ChatRoomInfoListCount*sizeof(SCHATROOM_LIST_INFO));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT	CIMIOCPSocket::MakeMSG_IC_CHATROOM_LIST_INFO_OK(SCHATROOM_LIST_INFO * o_pChatRoomListInfo, INT i_nStartNum, INT * o_nTotalCount)
/// \brief		EP3 채팅방 -
/// \author		dhjin
/// \date		2008-06-17 ~ 2008-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT	CIMIOCPSocket::MakeMSG_IC_CHATROOM_LIST_INFO_OK(SCHATROOM_LIST_INFO * o_pChatRoomListInfo, INT i_nStartNum, INT * o_nTotalCount)
{
	INT	nChatRoomListCount	= 0;
	INT	Count = 0;
	
	mt_auto_lock mta(&(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom));

	if(ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.empty())
	{
		return nChatRoomListCount;
	}
	
	// 같은 세력 전체 채팅방 수 
	mtListChatRoom::iterator itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(this->m_character.InfluenceType != (*itr)->GetChatRoomInfluenceType())
		{
			continue;
		}

		(*o_nTotalCount)++;
	}

	// 같은 세력 시작 채팅방 보내기 위해 처음 보낼 방 생성 시간으로 찾기
	ATUM_DATE_TIME CheckTime;
	itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	if(i_nStartNum)
	{
		CheckTime = *((*itr)->GetChatRoomCreateTime());
	}
	else
	{
		for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
		{
			if(i_nStartNum == (*itr)->GetChatRoomNum())
			{
				CheckTime = *((*itr)->GetChatRoomCreateTime());
				break;
			}
		}
	}


	// 시간으로 비교하여 채팅방 목록 보내기
	itr = ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.begin();
	for(; itr != ms_pIMIOCP->m_ChatRoomManager.m_mtlistChatRoom.end(); itr++)
	{
		if(0 < CheckTime.GetTimeDiffTimeInSeconds(*((*itr)->GetChatRoomCreateTime()))
			|| Count >= ARENA_MAX_REQUEST_TEAM
			|| this->m_character.InfluenceType != (*itr)->GetChatRoomInfluenceType()
			)
		{
			continue;
		}

		o_pChatRoomListInfo[nChatRoomListCount].ChatRoomNum				=   (*itr)->GetChatRoomNum();
		o_pChatRoomListInfo[nChatRoomListCount].ChatRoomLock				=	(*itr)->GetChatRoomLock();
		o_pChatRoomListInfo[nChatRoomListCount].Membercount				=	(*itr)->GetChatRoomMemberCount();	
		o_pChatRoomListInfo[nChatRoomListCount].MaxMembercount				=	(*itr)->GetChatRoomMaxMemberCount();
		util::strncpy(o_pChatRoomListInfo[nChatRoomListCount].ChatRoomName, (*itr)->GetChatRoomName(), SIZE_MAX_CHATROOM_NAME);
		util::strncpy(o_pChatRoomListInfo[nChatRoomListCount].ChatRoomMasterName,	(*itr)->GetChatRoomMasterCharacterName(), SIZE_MAX_CHARACTER_NAME);

		nChatRoomListCount++;
		Count++;
	}

	return nChatRoomListCount;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
/// \author		cmkwon
/// \date		2009-08-12 ~ 2009-08-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMIOCPSocket::SendFieldServPartyChangeFormationSkill(UID32_t i_PartyMasterCharcUID, BOOL i_bOnFlag, BYTE i_byFormationTy/*=FLIGHT_FORM_NONE*/)
{	
	INIT_MSG_WITH_BUFFER(MSG_FI_PARTY_CHANGE_FORMATION_SKILL, T_FI_PARTY_CHANGE_FORMATION_SKILL, pSendFS, SendBuf);
	pSendFS->PartyMasterCharcUID	= i_PartyMasterCharcUID;
	pSendFS->Is_On_or_Off			= i_bOnFlag;
	pSendFS->FlightFormation		= i_byFormationTy;
	ms_pIMIOCP->SendMessageToFieldServer(SendBuf, MSG_SIZE(MSG_FI_PARTY_CHANGE_FORMATION_SKILL));
}

void CIMIOCPSocket::SendCNC(MSG_IC_CHAT_CNC* msgCnC)
{
#ifdef _CROSS_NATION_CHAT
	// Log Chatting
	char szChatting[1024];
	sprintf(szChatting, "Chat_Cross_Nation|%s|%d|%d|%s\r\n",
		msgCnC->FromCharacterName, m_character.MapChannelIndex.MapIndex,
		m_character.MapChannelIndex.ChannelIndex, ((char*)msgCnC) + sizeof(MSG_IC_CHAT_CNC));
	g_pIMGlobal->WriteChattingLog(szChatting, m_character.Race);

	////////////////////////////////////////////////////////////////
	// 2014-08-04 Future, start building response to Client

	// 2014-08-04 Future, get the small (max 12 chars) Race or Influence String of the current character and put it to the Cnc Chat
	CIMIOCPSocket* senderSocket = this;
	if (this == ms_pIMIOCP->m_pFieldServerSocket)
	{
		// Sender is the Field Server not a client! Aquire client socket
		senderSocket = ms_pIMIOCP->GetIMIOCPSocketByCharacterName(msgCnC->FromCharacterName);
		if (!senderSocket)
		{
			g_pIMGlobal->WriteSystemLogEX(TRUE, "ERROR: Cannot find Cross Nation Chat sender. Sender Name (%s)", msgCnC->FromCharacterName);
			return;
		}
	}
	util::strncpy(msgCnC->InfluenceOrStaffPrefix, GetSmallRaceOrInfluenceString(senderSocket->m_character.Race, senderSocket->m_character.InfluenceType), SIZE_MAX_INFLUENCE_OR_STAFFPREFIX);

#if defined(_DEBUG) || defined(F_DEBUG)
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: CNC Chat About to be sent\r\n");
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: ****Struct Data****\r\n");
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: From Character Name -> %s\r\n", msgCnC->FromCharacterName);
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: Influence of Staff Prefix -> %s\t\t(Users Race: %d)\t(Users Masked Race: %d)\t(Users Influence: %d)\r\n"
		, msgCnC->InfluenceOrStaffPrefix
		, m_character.Race
		, m_character.Race&RACE_ACCOUNT_TYPE_MASK
		, m_character.InfluenceType);
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: Message Length: %d\r\n", msgCnC->MessageLength);
	g_pIMGlobal->WriteSystemLogEX(TRUE, "DEBUG: Chat String: %s\r\n", ((char*)msgCnC) + sizeof(MSG_IC_CHAT_CNC));
#endif

	// Make sure the packet has the correct header
	SET_MSG_TYPE(((BYTE*)msgCnC) - SIZE_FIELD_TYPE_HEADER, T_IC_CHAT_CNC)

	// Send Chat to all clients
	ms_pIMIOCP->SendChatMsgToAllClients(CHAT_TYPE_CNC
		, ((BYTE*)msgCnC) - SIZE_FIELD_TYPE_HEADER
		, MSG_SIZE(MSG_IC_CHAT_CNC) + msgCnC->MessageLength
		, INFLUENCE_TYPE_ALL_MASK);		// 2014-08-04 Future, adding Influence Type All mask, for the chat message to be delivered to ALL clients (nation independent)
#endif // _CROSS_NATION_CHAT
}
