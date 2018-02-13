// NPCIOCPSocket.cpp: implementation of the CNPCIOCPSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCIOCPSocket.h"
#include "NPCIOCP.h"
#include "NPCMapProject.h"
#include "NPCGlobal.h"
#include "NPCMapChannel.h"
#include "AtumError.h"

#include <process.h>

CNPCIOCP				*CNPCIOCPSocket::ms_pNPCIOCP = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCIOCPSocket::CNPCIOCPSocket() : m_threadMapWorker { nullptr }
{
	m_pNPCMapProject			= NULL;
	//m_hThreadMapWorker			= NULL;
	m_bThreadEndFlagMapWorker	= FALSE;
}

CNPCIOCPSocket::~CNPCIOCPSocket()
{
	MapWorkerEnd();
}

BOOL CNPCIOCPSocket::OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	int						nBytesUsed		= 0;
	MessageType_t			nRecvType		= 0;
	int						nRecvTypeSize	= 0;
	ProcessResult			procRes			= RES_BREAK;
	CNPCIOCPSocket			*pNPCIOCPSocket	= NULL;
	int						tmpSeq;
	MessageType_t			prevMsgType		= 0;

	// UDP 처리 루틴
	if(m_bUDPFlag == TRUE)
	{
// 2006-05-19 by cmkwon
// 		if(strncmp(m_szPeerIP, pPeerIP, SIZE_MAX_IPADDRESS) != 0 || m_nPeerPort != nPeerPort)
// 		{
// 			// Protocl Error 처리
// 			// - UDP로 받은 패킷의 IP와 Port가 서버에 설정된 Client의 IP, Port와 같지 않다
// 			// Error Code : ERR_PROTOCOL_INVALID_PEER
// 
// 			char szSystemLog[256];
// 			sprintf(szSystemLog, "  ERR_PROTOCOL_INVALID_PEER SocketIndex[%3d] RecvIP[%15s] Port[%4d]\r\n",
// 				this->GetClientArrayIndex(), pPeerIP, nPeerPort);
// 			g_pNPCGlobal->WriteSystemLog(szSystemLog);
// 			DBGOUT(szSystemLog);
// 
// 			return FALSE;
// 		}

		while(this->IsUsing() && nBytesUsed < nLength)
		{
			nRecvType			= 0;

			nRecvType = *(MessageType_t*)(pPacket+nBytesUsed);
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;

			if(i_pThreadInfo)
			{
				i_pThreadInfo->dwSocketIndex = m_nClientArrayIndex;
				i_pThreadInfo->dwMessageType = nRecvType;
			}


//			if (nRecvType != T_FN_MOVE_OK)
//			{
//				DbgOut("    [NPCServer]index[%d] RecvType[%s(%#04X)]\n", this->GetClientArrayIndex(), GetProtocolTypeString(nRecvType), nRecvType);
//			}

			switch(nRecvType)
			{
				////////////////////////////////////////////////////////////////////////
				// From Field Server
			case T_FN_MAPPROJECT_START_OK:
				procRes = Process_FN_MAPPROJECT_START_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_CREATE_OK:
				procRes = Process_FN_MONSTER_CREATE_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CLIENT_GAMESTART_OK:
				procRes = Process_FN_CLIENT_GAMESTART_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CLIENT_GAMEEND_OK:
				procRes = Process_FN_CLIENT_GAMEEND_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MOVE_OK:
				procRes = Process_FN_MOVE_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_GET_CHARACTER_INFO_OK:
				procRes = Process_FN_GET_CHARACTER_INFO_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_BATTLE_SET_ATTACK_CHARACTER:
				procRes = Process_FN_BATTLE_SET_ATTACK_CHARACTER(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_BATTLE_DROP_FIXER:
				procRes = Process_FN_BATTLE_DROP_FIXER(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP:
				procRes = Process_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_CHANGE_HP:
				procRes = Process_FN_CHARACTER_CHANGE_MONSTER_HP(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CHARACTER_CHANGE_BODYCONDITION:
				procRes = Process_FN_CHARACTER_CHANGE_BODYCONDITION(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CHARACTER_CHANGE_STEALTHSTATE:
				procRes = Process_FN_CHARACTER_CHANGE_STEALTHSTATE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK:
				procRes = Process_FN_CHARACTER_CHANGE_CHARACTER_MODE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CHARACTER_CHANGE_INFLUENCE_TYPE:
				procRes = Process_FN_CHARACTER_CHANGE_INFLUENCE_TYPE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CHARACTER_CHANGE_INVISIBLE:
				procRes = Process_FN_CHARACTER_CHANGE_INVISIBLE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_DELETE:
				procRes = Process_FN_MONSTER_DELETE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_ADMIN_SUMMON_MONSTER:
				procRes = Process_FN_ADMIN_SUMMON_MONSTER(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_SKILL_USE_SKILL_OK:
				procRes = Process_FN_SKILL_USE_SKILL_OK(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_CHANGE_BODYCONDITION:
				procRes = Process_FN_MONSTER_CHANGE_BODYCONDITION(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_SKILL_END_SKILL:
				procRes = Process_FN_MONSTER_SKILL_END_SKILL(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_STRATEGYPOINT_INIT:
				procRes = Process_FN_MONSTER_STRATEGYPOINT_INIT(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_STRATEGYPOINT_SUMMON:
				procRes = Process_FN_MONSTER_STRATEGYPOINT_SUMMON(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_OUTPOST_INIT:
				procRes = Process_FN_MONSTER_OUTPOST_INIT(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_OUTPOST_RESET_SUMMON:
				procRes = Process_FN_MONSTER_OUTPOST_SUMMON(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE:
				procRes = Process_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_MONSTER_TELEPORT_SUMMON:
				procRes = Process_FN_MONSTER_TELEPORT_SUMMON(pPacket, nLength, nBytesUsed);
				break;

			case T_FN_CONNECT_SET_CHANNEL_STATE:
				procRes = Process_FN_CONNECT_SET_CHANNEL_STATE(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CONNECT_INCREASE_CHANNEL:
				procRes = Process_FN_CONNECT_INCREASE_CHANNEL(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CITYWAR_START_WAR:
				procRes = Process_FN_CITYWAR_START_WAR(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CITYWAR_END_WAR:
				procRes = Process_FN_CITYWAR_END_WAR(pPacket, nLength, nBytesUsed);
				break;
			case T_FN_CITYWAR_CHANGE_OCCUPY_INFO:
				procRes = Process_FN_CITYWAR_CHANGE_OCCUPY_INFO(pPacket, nLength, nBytesUsed);
				break;
			case T_ERROR:
				{
					MSG_ERROR *pRecvMsg;
					pRecvMsg = (MSG_ERROR*)(pPacket + nBytesUsed);
					nBytesUsed += sizeof(MSG_ERROR) + pRecvMsg->StringLength;
					g_pNPCGlobal->WriteSystemLogEX(TRUE, "  T_ERROR %s(%#04X) received from %s[%s]\r\n", GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode, "ST_FIELD_SERVER", m_szPeerIP);
					procRes = RES_RETURN_TRUE;
				}
				break;
			default:
				{
					// Protocol Error 처리(Close 처리함)
					// - Client로 부터 받은 Field Type이 유효하지 않다
					// Error Code : ERR_PROTOCOL_INVALID_PROTOCOL_TYPE
					SendErrorMessage(T_NPC_IOCP, ERR_PROTOCOL_INVALID_PROTOCOL_TYPE, 0, 0, NULL, TRUE);

					char	szSystemLog[256];
					sprintf(szSystemLog, "[Error] CNPCIOCPSocket::OnRecvdPacketNPCServer1 invalid protocol type, RecvType[%s(%#04x)], PrevType[%s(%#04x)]\r\n",
							GetProtocolTypeString(nRecvType), nRecvType,
							GetProtocolTypeString(prevMsgType), prevMsgType);
					g_pNPCGlobal->WriteSystemLog(szSystemLog);
					DBGOUT(szSystemLog);
					return FALSE;
				}
			}

#ifdef _DEBUG
			// 이전 타입 저장
			prevMsgType = nRecvType;
#endif

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
		} // while end

		return TRUE;
	} // end UDP 처리 루틴

	// TCP 처리 루틴
	if(m_bPeerSequenceNumberInitFlag == FALSE)
	{
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
		m_bPeerSequenceNumberInitFlag = TRUE;
	}
	else
	{
		//DBGOUT("%s[%d]: Sequence Number ==> server %d, client %d\n", m_character.CharacterName			, m_character.ClientIndex, nSeq, m_byPeerSequenceNumber);
		if(m_byPeerSequenceNumber != nSeq)
		{
			// Protocl Error 처리
			// - 받은 패킷의 Sequence Number가 유효하지 않음
			// Error Code : ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER
			SendErrorMessage(T_NPC_IOCP, ERR_PROTOCOL_INVALID_SEQUENCE_NUMBER);
			Close(0x15002);
			return FALSE;
		}
		tmpSeq = (nSeq + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byPeerSequenceNumber = ++tmpSeq;
	} // end TCP 처리 루틴


	while(this->IsUsing() && nBytesUsed < nLength)
	{
		nRecvType			= 0;
		pNPCIOCPSocket		= NULL;

		nRecvType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

		switch(nRecvType)
		{
		////////////////////////////////////////////////////////////////////////
		// FieldServer <-- Client, FieldServer <-- Monitor, FieldServer <-- NPCServer
		case T_PM_CONNECT_ALIVE:
		case T_IM_CONNECT_ALIVE:
		case T_FM_CONNECT_ALIVE:
		case T_NM_CONNECT_ALIVE:
			procRes = RES_RETURN_TRUE;
			break;

		////////////////////////////////////////////////////////////////////////
		// From Monitor Client
		case T_NM_CONNECT:
			{
				// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 아래와 같이 수정 함
				//if(FALSE == IS_SCADMINTOOL_CONNECTABLE_IP(GetPeerIP()))
				if(FALSE == g_pNPCGlobal->CheckAllowedToolIP(this->GetPeerIP()))	// 2008-06-05 by cmkwon, AdminTool, Monitor 접근 가능 IP를 server config file 에 설정하기 - 
				{
					return FALSE;
				}
				char szSystemLog[256];
				sprintf(szSystemLog, "Monitor Client Connected, IP[%s]\r\n", GetPeerIP());
				DBGOUT(szSystemLog);
				g_pNPCGlobal->WriteSystemLog(szSystemLog);

				m_PeerSocketType = ST_MONITOR_SERVER;
				ms_pNPCIOCP->InsertMonitorIOCPSocketPtr(this);

				INIT_MSG_WITH_BUFFER(MSG_NM_CONNECT_OK, T_NM_CONNECT_OK, pSendNMConnectOK, SendBuf);
				util::strncpy(pSendNMConnectOK->IPAddress, g_pNPCGlobal->GetPublicIPLocal(), SIZE_MAX_IPADDRESS);
				pSendNMConnectOK->NumOfClients = 1;
				pSendNMConnectOK->Port = ms_pNPCIOCP->m_nListenerPort;
				pSendNMConnectOK->CalcBandwidth = ms_pNPCIOCP->GetFlagCalcTrafficInfo();
				pSendNMConnectOK->LoadedMapCounts = ms_pNPCIOCP->GetMapWorkspace()->GetMapProjectCounts();
				pSendNMConnectOK->StartedTime = ms_pNPCIOCP->m_dwTimeStarted;
				util::strncpy(pSendNMConnectOK->ServerName, "NPC Server", SIZE_MAX_SERVER_NAME);
				SendAddData(SendBuf, MSG_SIZE(MSG_NM_CONNECT_OK));

				//////////////////////////////////////////////////////////
				// Loaded Map Info
				for(int i = 0; i < ms_pNPCIOCP->GetMapWorkspace()->GetMapProjectCounts(); i++)
				{
					CMapProject *pTmpMapProject
						= ms_pNPCIOCP->GetMapWorkspace()->GetMapProjectByIndex(i);

					INIT_MSG(MSG_NM_SEND_CHANNEL_INFO, T_NM_SEND_CHANNEL_INFO, pSendChannelInfo, SendBuf);

					int nChannCounts = pTmpMapProject->GetNumberOfMapChannels();
					for (int j = 0; j < nChannCounts; j++)
					{
						CNPCMapChannel *pNPCMapChannel = (CNPCMapChannel*)pTmpMapProject->GetMapChannelByIndex(j);
						if (pNPCMapChannel == NULL)
						{
							continue;
						}

						pSendChannelInfo->projectInfo = *((PROJECTINFO*)pTmpMapProject);
						pSendChannelInfo->ChannelIndex = pNPCMapChannel->m_MapChannelIndex.ChannelIndex;
						pSendChannelInfo->IsEnabled = pNPCMapChannel->IsEnabled();
						pSendChannelInfo->LimitMonsterCount = pTmpMapProject->m_nMaxMonsterCount;
						SendAddData(SendBuf, MSG_SIZE(MSG_NM_SEND_CHANNEL_INFO));
					}
				}
			}
			break;
		case T_NM_GET_NUM_MONSTERS:
			{
				INIT_MSG_WITH_BUFFER(MSG_NM_GET_NUM_MONSTERS_OK, T_NM_GET_NUM_MONSTERS_OK, pSendNMGetNumClientsOK, SendBuf);
				pSendNMGetNumClientsOK->NumOfMonsters = 100;
				SendAddData(SendBuf, MSG_SIZE(MSG_NM_GET_NUM_MONSTERS_OK));
			}
			break;
		case T_NM_SHUTDOWN:
			{
				SendAddMessageType(T_NM_SHUTDOWN_OK);
				PostMessage(g_pNPCGlobal->GetMainWndHandle(), WM_CLOSE, 0, 0);
			}
			break;
		case T_NM_PING:
			{
				SendAddMessageType(T_NM_PING_OK);
			}
			break;
		case T_NM_SAVE_MAPBLOCK_INFO:
			procRes = Process_NM_SAVE_MAPBLOCK_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_NM_PRINTMAP:
			procRes = Process_NM_PRINTMAP(pPacket, nLength, nBytesUsed);
			break;
		case T_NM_CHANGE_BANDWIDTH_FLAG:
			{
				ms_pNPCIOCP->SetFlagCalcTrafficInfo(!ms_pNPCIOCP->GetFlagCalcTrafficInfo());

				INIT_MSG_WITH_BUFFER(MSG_NM_CHANGE_BANDWIDTH_FLAG_OK, T_NM_CHANGE_BANDWIDTH_FLAG_OK, pChangeFlag, SendBuf);
				pChangeFlag->bChagedFlag = ms_pNPCIOCP->GetFlagCalcTrafficInfo();
				SendAddData(SendBuf, MSG_SIZE(MSG_NM_CHANGE_BANDWIDTH_FLAG_OK));
			}
			break;
		case T_NM_SET_MSG_PRINT_LEVEL:
			{
				BYTE tmpLevel = *(BYTE*)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_NM_SET_MSG_PRINT_LEVEL);

				// set msg print level
				GSetexchangeMsgPrintLevel(tmpLevel);
			}
			break;
		case T_NM_GET_MAP_USER_COUNTS:
			procRes = Process_NM_GET_MAP_USER_COUNTS(pPacket, nLength, nBytesUsed);
			break;
		case T_NM_SET_LIMIT_MONSTER_COUNT:
			procRes = Process_NM_SET_LIMIT_MONSTER_COUNT(pPacket, nLength, nBytesUsed);
			break;
		default:
			{
				// Protocol Error 처리(Close 처리함)
				// - Client로 부터 받은 Field Type이 유효하지 않다
				// Error Code : ERR_PROTOCOL_INVALID_PROTOCOL_TYPE
				SendErrorMessage(T_NPC_IOCP, ERR_PROTOCOL_INVALID_PROTOCOL_TYPE, 0, 0, NULL, TRUE);

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CNPCIOCPSocket::OnRecvdPacketNPCServer invalid protocol type, RecvType[%s(%#04X)]\r\n", GetProtocolTypeString(nRecvType), nRecvType);
				g_pNPCGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return FALSE;
			}
		}	// end switch

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

	}	// end while

	return TRUE;
}

void CNPCIOCPSocket::OnConnect(void)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pNPCGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	CIOCPSocket::OnConnect();
}

void CNPCIOCPSocket::OnClose(int reason)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket  Closed SocketIndex[%3d] PeerIP[%15s] Port[%4d] MaxWriteBufCounts[%4d] ==> reason %d[%#08X]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort, m_nMaxWriteBufCounts, reason, reason);
	g_pNPCGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	if(m_PeerSocketType == ST_MONITOR_SERVER)
	{
		ms_pNPCIOCP->DeleteMonitorIOCPSocketPtr(this);
	}

	CIOCPSocket::OnClose(30);
}

BOOL CNPCIOCPSocket::OnError(int errCode)
{
	switch(errCode)
	{
	case ERROR_PORT_UNREACHABLE:
		{	// UDP 전송일때 상대방의 Port가 열려있지 않을때 발생하는 에러
			// Monster 정보를 모두 초기화, Read를 다시 한번 호출

			MessageBox(NULL, "UDP Error", "Error", MB_OK);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CNPCIOCPSocket::InitializeNPCMap(CMapProject *pmapProject, ez_map<int, MONSTER_INFO> *mapMonsterParam)
{
	if(m_pNPCMapProject){ return TRUE;}

	m_pNPCMapProject = (CNPCMapProject*)pmapProject;
	m_pNPCMapProject->SetIOCPSocketMapSession(this);
	m_pNPCMapProject->m_pMapMonsterParameter = mapMonsterParam;

	int nSendType = T_FN_MAPPROJECT_START;
	SendAddData((BYTE*)&nSendType, SIZE_FIELD_TYPE_HEADER);
	return TRUE;
	//return CreateThreadMapWorker();
}

DWORD WINAPI MapWorkerThread(LPVOID lpParam)
{
	if(lpParam == NULL){ return 20;}
	return ((CNPCIOCPSocket*)lpParam)->MapWorker();
}

BOOL CNPCIOCPSocket::CreateThreadMapWorker(void)
{	
	if (!m_threadMapWorker)
	{
		try
		{
			m_threadMapWorker = new thread { MapWorkerThread, LPVOID(this) };
		}
		catch (const system_error&)
		{
			return false;
		}
	}

	return true;
}

DWORD CNPCIOCPSocket::MapWorker()
{
	vector<ClientIndex_t>	vecClientIndex;			// Monster 이동시 사용할 인덱스 vector
	vector<D3DXVECTOR3>		vecD3DXVECTOR;			// Monster 생성시 Position Vecotr로 사용할 vector

	if(m_pNPCMapProject->m_pMapMonsterParameter->empty())	// 몬스터 정보가 없으면 리턴
	{
		return 22;
	}

	// todo : remove this srand call
	srand(timeGetTime());

	random::init();

	vecClientIndex.reserve(100);
	vecD3DXVECTOR.reserve(100);
	
	{
		unique_lock<mutex> lock { m_mutexMapWorker };

		m_terminateCondition.wait_for(lock, 2s + chrono::seconds { GetClientArrayIndex() + 1 });
	}


	while (!m_bThreadEndFlagMapWorker)
	{
		// 2008-11-11 by cmkwon, NPCServer CPU 점유율 문제 보완 - 
		unsigned long long dwStart	= timeGetTime();
		BYTE SendBuf[SIZE_MAX_PACKET];
		m_pNPCMapProject->WorkerNPCAllChannel(SendBuf, &vecD3DXVECTOR, &vecClientIndex);
		unsigned long long dwEnd	= timeGetTime();
		
		long long dwTickGap	= dwEnd - dwStart;
		if (dwTickGap < 0) dwTickGap += 1ll << 32;

		DWORD dwSleepTick = max(20, 100 - (int)dwTickGap);
		Sleep(dwSleepTick);
	}
	return 21;
}

void CNPCIOCPSocket::MapWorkerEnd()
{
	if (m_threadMapWorker)
	{
		server::log(" CNPCIOCPSocket::MapWorkerEnd_1 ArrayIndex(%d)\r\n", m_nClientArrayIndex);

		{
			lock_guard<mutex> lock { m_mutexMapWorker };

			m_bThreadEndFlagMapWorker = true;
			m_terminateCondition.notify_all();
		}

		if (m_threadMapWorker->joinable()) m_threadMapWorker->join();

		delete m_threadMapWorker;

		m_threadMapWorker = nullptr;
	}
}


ProcessResult CNPCIOCPSocket::Process_FN_MAPPROJECT_START_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_MAPPROJECT_START_OK	*pRecvStartOK = NULL;
	nRecvTypeSize = sizeof(MSG_FN_MAPPROJECT_START_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MAPPROJECT_START_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15005);
		return RES_RETURN_FALSE;
	}
	pRecvStartOK = (MSG_FN_MAPPROJECT_START_OK*)(pPacket+nBytesUsed);	
	nBytesUsed += nRecvTypeSize;

	nRecvTypeSize = pRecvStartOK->CreateChannelCounts*sizeof(MSG_FN_CONNECT_SET_CHANNEL_STATE);
	if(nLength-nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MAPPROJECT_START_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15005);
		return RES_RETURN_FALSE;
	}
	MSG_FN_CONNECT_SET_CHANNEL_STATE *pChannelState = (MSG_FN_CONNECT_SET_CHANNEL_STATE*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	g_pNPCGlobal->WriteSystemLogEX(TRUE, "	CNPCIOCPSocket::OnRecvdPacketNPCServer# NPC MapWorker Start, Map[%04d], ChannelCount(%d) \r\n", m_pNPCMapProject->m_nMapIndex, pRecvStartOK->CreateChannelCounts);

	if(m_pNPCMapProject->m_bCreateNPCThread)
	{
		///////////////////////////////////////////////////////////////////////////
		// Map Project의 Channel 을 초기화 한다.
		m_pNPCMapProject->InitializeMapChannels(pRecvStartOK->CreateChannelCounts);

		for(int i=0; i < pRecvStartOK->CreateChannelCounts; i++)
		{
			CNPCMapChannel *pChannel = m_pNPCMapProject->GetNPCMapChannelByIndex(pChannelState[i].MapChannelIndex.ChannelIndex);
			if(pChannel)
			{
				pChannel->SetChannelState(pChannelState[i].EnableChannel);
				pChannel->m_bCityWarStarted	= pRecvStartOK->bCityWarStarted;
				pChannel->SetCityWarOccupyGuildUID(pRecvStartOK->CityWarOccupyGuildUID);
			}
		}
		
		CreateThreadMapWorker();					// NPC Server 스레드를 생성한다.
	}	

	///////////////////////////////////////////////////////////////////////////
	// Field Server로 MapProjectStartOKAck 메세지를 전송한.
	MessageType_t	tmSendType = T_FN_MAPPROJECT_START_OK_ACK;
	SendAddData((BYTE*)&tmSendType, SIZE_FIELD_TYPE_HEADER);
	return RES_RETURN_TRUE;
}


ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;

	MSG_FN_MONSTER_CREATE_OK	*pRecvFMCreateOK = NULL;
	nRecvTypeSize = sizeof(MSG_FN_MONSTER_CREATE_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MONSTER_CREATE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15005);
		return RES_RETURN_FALSE;
	}

	pRecvFMCreateOK = (MSG_FN_MONSTER_CREATE_OK*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnMonsterCreateOKW(pRecvFMCreateOK);

	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CLIENT_GAMESTART_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CLIENT_GAMESTART_OK	*pRecvClientGameStart = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CLIENT_GAMESTART_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CLIENT_GAMESTART_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15006);
		return RES_RETURN_FALSE;
	}
	pRecvClientGameStart = (MSG_FN_CLIENT_GAMESTART_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnClientGameStartOKW(pRecvClientGameStart);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CLIENT_GAMEEND_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CLIENT_GAMEEND_OK	*pRecvClientGameEndOK = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CLIENT_GAMEEND_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CLIENT_GAMEEND_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15007);
		return RES_RETURN_FALSE;
	}
	pRecvClientGameEndOK = (MSG_FN_CLIENT_GAMEEND_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnClientGameEndOKW(pRecvClientGameEndOK);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_MOVE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	BYTE						SendBuf[SIZE_MAX_PACKET];
	MSG_FN_MOVE_OK				*pRecvFCMoveOK = NULL;
	MSG_FN_GET_CHARACTER_INFO	*pSendCharacterInfo = NULL;

	nRecvTypeSize = sizeof(MSG_FN_MOVE_OK);
	memset(SendBuf, 0x00, sizeof(SendBuf));
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MOVE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15008);
		return RES_RETURN_FALSE;
	}
	pRecvFCMoveOK = (MSG_FN_MOVE_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnMoveOKW(pRecvFCMoveOK);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_GET_CHARACTER_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_GET_CHARACTER_INFO_OK	*pRecvCharacterInfoOK = NULL;

	nRecvTypeSize = sizeof(MSG_FN_GET_CHARACTER_INFO_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_GET_CHARACTER_INFO_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1500A);
		return RES_RETURN_FALSE;
	}
	pRecvCharacterInfoOK = (MSG_FN_GET_CHARACTER_INFO_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnGetCharacterInfoOKW(pRecvCharacterInfoOK);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_BATTLE_DROP_FIXER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_BATTLE_DROP_FIXER	*pDropFixer = NULL;

	nRecvTypeSize = sizeof(MSG_FN_BATTLE_DROP_FIXER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_BATTLE_DROP_FIXER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1500D);
		return RES_RETURN_FALSE;
	}
	pDropFixer = (MSG_FN_BATTLE_DROP_FIXER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnBattleDropFixerW(pDropFixer);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_BATTLE_SET_ATTACK_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize;
	MSG_FN_BATTLE_SET_ATTACK_CHARACTER	*pSetCharac = NULL;

	nRecvTypeSize = sizeof(MSG_FN_BATTLE_SET_ATTACK_CHARACTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_BATTLE_SET_ATTACK_CHARACTER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x1500D);
		return RES_RETURN_FALSE;
	}
	pSetCharac = (MSG_FN_BATTLE_SET_ATTACK_CHARACTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnBattleSetAttackCharacterW(pSetCharac);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP	*pChange = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15010);
		return RES_RETURN_FALSE;
	}
	pChange = (MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnCharacterChangeCurrentHPDPSPEPW(pChange);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_MONSTER_HP(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_MONSTER_CHANGE_HP	*pChange = NULL;

	nRecvTypeSize = sizeof(MSG_FN_MONSTER_CHANGE_HP);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MONSTER_CHANGE_HP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15010);
		return RES_RETURN_FALSE;
	}
	pChange = (MSG_FN_MONSTER_CHANGE_HP*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnMonsterChangeHPW(pChange);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CHARACTER_CHANGE_BODYCONDITION	*pBody = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CHARACTER_CHANGE_BODYCONDITION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CHARACTER_CHANGE_BODYCONDITION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15011);
		return RES_RETURN_FALSE;
	}
	pBody = (MSG_FN_CHARACTER_CHANGE_BODYCONDITION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnCharacterChangeBodyconditionW(pBody);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_STEALTHSTATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CHARACTER_CHANGE_STEALTHSTATE	*pBody = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CHARACTER_CHANGE_STEALTHSTATE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CHARACTER_CHANGE_STEALTHSTATE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15011);
		return RES_RETURN_FALSE;
	}
	pBody = (MSG_FN_CHARACTER_CHANGE_STEALTHSTATE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnCharacterChangeStealthStateW(pBody);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_CHARACTER_MODE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK	*pBody = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CHARACTER_CHANGE_STEALTHSTATE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15011);
		return RES_RETURN_FALSE;
	}
	pBody = (MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnCharacterChangeCharacterModeW(pBody);
	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_INFLUENCE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-03 ~ 2005-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_INFLUENCE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE	*pBody = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CHARACTER_CHANGE_INFLUENCE_TYPE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15011);
		return RES_RETURN_FALSE;
	}
	pBody = (MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnCharacterChangeInfluenceTypeW(pBody);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_INVISIBLE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2006-11-27 ~ 2006-11-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_CHARACTER_CHANGE_INVISIBLE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize;
	MSG_FN_CHARACTER_CHANGE_INVISIBLE	*pBody = NULL;

	nRecvTypeSize = sizeof(MSG_FN_CHARACTER_CHANGE_INVISIBLE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_CHARACTER_CHANGE_INVISIBLE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15011);
		return RES_RETURN_FALSE;
	}
	pBody = (MSG_FN_CHARACTER_CHANGE_INVISIBLE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnCharacterChangeInvisibleW(pBody);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_DELETE(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize	= 0;
	MSG_FN_MONSTER_DELETE	*pRecvMonsterDele = NULL;

	nRecvTypeSize = sizeof(MSG_FN_MONSTER_DELETE);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MONSTER_DELETE, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15012);
		return RES_RETURN_FALSE;
	}
	pRecvMonsterDele = (MSG_FN_MONSTER_DELETE*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnMonsterDeleteW(pRecvMonsterDele);
	return RES_RETURN_TRUE;
}


ProcessResult CNPCIOCPSocket::Process_FN_ADMIN_SUMMON_MONSTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FN_ADMIN_SUMMON_MONSTER	*pRecvSummon = NULL;

	nRecvTypeSize = sizeof(MSG_FN_ADMIN_SUMMON_MONSTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_ADMIN_SUMMON_MONSTER, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15013);
		return RES_RETURN_FALSE;
	}
	pRecvSummon = (MSG_FN_ADMIN_SUMMON_MONSTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnAdminSummonMonsterW(pRecvSummon);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_SKILL_USE_SKILL_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FN_SKILL_USE_SKILL_OK	*pRecvSkillOK = NULL;

	nRecvTypeSize = sizeof(MSG_FN_SKILL_USE_SKILL_OK);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_SKILL_USE_SKILL_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15013);
		return RES_RETURN_FALSE;
	}
	pRecvSkillOK = (MSG_FN_SKILL_USE_SKILL_OK*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnSkillUseSkillOKW(pRecvSkillOK);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_CHANGE_BODYCONDITION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FN_MONSTER_CHANGE_BODYCONDITION	*pRecvMsg = NULL;

	nRecvTypeSize = sizeof(MSG_FN_MONSTER_CHANGE_BODYCONDITION);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MONSTER_CHANGE_BODYCONDITION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15013);
		return RES_RETURN_FALSE;
	}
	pRecvMsg = (MSG_FN_MONSTER_CHANGE_BODYCONDITION*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnMonsterChangeBodyconditionW(pRecvMsg);
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_SKILL_END_SKILL(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FN_MONSTER_SKILL_END_SKILL	*pReEndSkill = NULL;

	nRecvTypeSize = sizeof(MSG_FN_MONSTER_SKILL_END_SKILL);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FN_MONSTER_SKILL_END_SKILL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x15013);
		return RES_RETURN_FALSE;
	}
	pReEndSkill = (MSG_FN_MONSTER_SKILL_END_SKILL*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	m_pNPCMapProject->NPCOnMonsterSkillEndSkillW(pReEndSkill);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_STRATEGYPOINT_INIT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-20 ~ 2006-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_STRATEGYPOINT_INIT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_MONSTER_STRATEGYPOINT_INIT,
									MSG_FN_MONSTER_STRATEGYPOINT_INIT, pRMsg);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-21 by cmkwon, 이 메시지를 해당 맵세력의 0번 채널에 모두 추가한다.
	pRMsg->ChannelIndex			= 0;		// 2006-11-21 by cmkwon, 확인 차원으로 0번 채널을 설정한다
	ms_pNPCIOCP->m_pNPCMapWorkspace->AddMessageMSG_FN_MONSTER_STRATEGYPOINT_INIT(pRMsg);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_STRATEGYPOINT_SUMMON(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-03-02 ~ 2007-03-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_STRATEGYPOINT_SUMMON(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_MONSTER_STRATEGYPOINT_SUMMON,
									MSG_FN_MONSTER_STRATEGYPOINT_SUMMON, pRMsg);

	ms_pNPCIOCP->m_pNPCMapWorkspace->AddMessageMSG_FN_MONSTER_STRATEGYPOINT_SUMMON(pRMsg->MapIndex);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_OUTPOST_INIT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_OUTPOST_INIT(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_MONSTER_OUTPOST_INIT,
									MSG_FN_MONSTER_OUTPOST_INIT, pRMsg);

	CNPCMapChannel * pNMChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}

	pNMChann->NPCOnMonsterOutPostInit(pRMsg);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_OUTPOST_SUMMON(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-08-24 ~ 2007-08-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_OUTPOST_SUMMON(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_MONSTER_OUTPOST_RESET_SUMMON,
									MSG_FN_MONSTER_OUTPOST_RESET_SUMMON, pRMsg);

	CNPCMapChannel * pNMChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}

// 2007-09-19 by cmkwon, Bell로 소환 처리 - 아래와 같이 수정함
//	// 2007-08-24 by dhjin, 전진기지 중앙처리장치 소환
//	pNMChann->NPCOnSummonObjectMonsterBYBelligerence(pRMsg->bell1);
	MSG_MONSTER_SUMMON_BY_BELL msg;
	msg.MonsterBell		= pRMsg->bell1;
	pNMChann->NPCOnMonsterSummonByBell(&msg);

	return RES_RETURN_TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-08-29 ~ 2007-08-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE,
									MSG_FN_MONSTER_CREATE_IN_MAPCHANNEL_BYVALUE, pRMsg);

	CNPCMapChannel * pNMChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}

	// 2007-08-29 by dhjin, 몬스터 생성 가능
	pNMChann->SetNotCreateMonsterValue(FALSE);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_TELEPORT_SUMMON(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-09-05 ~ 2007-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CNPCIOCPSocket::Process_FN_MONSTER_TELEPORT_SUMMON(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_MONSTER_TELEPORT_SUMMON,
									MSG_FN_MONSTER_TELEPORT_SUMMON, pRMsg);

	CNPCMapChannel * pNMChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}

// 2007-09-19 by cmkwon, Bell로 소환 처리 - 아래와 같이 수정함
//	// 2007-09-05 by dhjin, 텔레포트 소환 중앙처리장치 소환
//	pNMChann->NPCOnSummonObjectMonsterBYBelligerence(pRMsg->bell1);
	MSG_MONSTER_SUMMON_BY_BELL msg;
	msg.MonsterBell		= pRMsg->bell1;
	pNMChann->NPCOnMonsterSummonByBell(&msg);

	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_NM_PRINTMAP(const char* pPacket, int nLength, int &nBytesUsed)
{
	int					nRecvTypeSize = 0;
	MSG_NM_PRINTMAP		*pReMsg = NULL;

	nRecvTypeSize = sizeof(MSG_NM_PRINTMAP);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_NM_PRINTMAP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x14056);
		return RES_RETURN_FALSE;
	}
	pReMsg = (MSG_NM_PRINTMAP*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CNPCMapChannel * pNMChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pReMsg->MapChannelIndex);
	if(NULL == pNMChann){			return RES_BREAK;}
#ifdef _DEBUG
	pNMChann->SaveUnitCountsPerBlock();
#endif
	
	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_NM_SAVE_MAPBLOCK_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_NM_SAVE_MAPBLOCK_INFO	*pRecvMapBlockInfo = NULL;

	nRecvTypeSize = sizeof(MSG_NM_SAVE_MAPBLOCK_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_NM_SAVE_MAPBLOCK_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x14056);
		return RES_RETURN_FALSE;
	}
	pRecvMapBlockInfo = (MSG_NM_SAVE_MAPBLOCK_INFO*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CNPCMapChannel * pNMChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRecvMapBlockInfo->MapChannelIndex);
	if(NULL == pNMChann){			return RES_BREAK;}

	pNMChann->SaveUnitCountsPerBlock();
	return RES_RETURN_TRUE;
}


ProcessResult CNPCIOCPSocket::Process_NM_GET_MAP_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_NM_GET_MAP_USER_COUNTS	*pRecvMapUserCounts = NULL;

	nRecvTypeSize = sizeof(MSG_NM_GET_MAP_USER_COUNTS);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_NM_GET_MAP_USER_COUNTS, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x14056);
		return RES_RETURN_FALSE;
	}
	pRecvMapUserCounts = (MSG_NM_GET_MAP_USER_COUNTS*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CNPCMapChannel *pNMapChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRecvMapUserCounts->MapChannIndex);
	if(NULL == pNMapChann){				return RES_BREAK;}

	INIT_MSG_WITH_BUFFER(MSG_NM_GET_MAP_USER_COUNTS_ACK, T_NM_GET_MAP_USER_COUNTS_ACK, pSendUserCountsAck, SendB);
	pSendUserCountsAck->MapChannIndex = pRecvMapUserCounts->MapChannIndex;
	pSendUserCountsAck->AccumulatedUserCounts = pNMapChann->m_uiAccumulatedUserCountsInChannel;
	pSendUserCountsAck->CurrentUserCounts = pNMapChann->GetNumClients();
	pNMapChann->UpdateMaxUserCounts(pSendUserCountsAck->CurrentUserCounts);
	pSendUserCountsAck->MaxUserCounts = pNMapChann->m_uiMaxUserCountsInChannel;
	pSendUserCountsAck->AccumulatedMonsterCounts = pNMapChann->m_uiAccumulatedMonsterCountsInChannel;
	pSendUserCountsAck->CurrentMonsterCounts = pNMapChann->GetNumMonsters();
	SendAddData(SendB, MSG_SIZE(MSG_NM_GET_MAP_USER_COUNTS_ACK));

	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_NM_SET_LIMIT_MONSTER_COUNT(const char* pPacket, int nLength, int &nBytesUsed)
{
	int								nRecvTypeSize = 0;
	MSG_NM_SET_LIMIT_MONSTER_COUNT	*pRecvLimitCount = NULL;

	nRecvTypeSize = sizeof(MSG_NM_SET_LIMIT_MONSTER_COUNT);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error 처리
		// - Client로 부터 받은 Data Size가 Field Type에 따른 Data Size보다 작다
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_NM_GET_MAP_USER_COUNTS, ERR_PROTOCOL_INVALID_FIELD_DATA);
		Close(0x14056);
		return RES_RETURN_FALSE;
	}
	pRecvLimitCount = (MSG_NM_SET_LIMIT_MONSTER_COUNT*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CNPCMapChannel *pMapChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pRecvLimitCount->MapChannelIndex);
	if(NULL == pMapChann){			return RES_BREAK;}

	if(pRecvLimitCount->SetLimitMonsterCount < pMapChann->m_nSizemtvectorMonsterPtr)
	{
		pMapChann->m_uiLimitMonsterCountsInChannel = pRecvLimitCount->SetLimitMonsterCount;
	}
	
	INIT_MSG_WITH_BUFFER(MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK, T_NM_SET_LIMIT_MONSTER_COUNT_ACK, pSeAck, SendBuf);
	pSeAck->MapChannelIndex = pRecvLimitCount->MapChannelIndex;
	pSeAck->SetLimitMonsterCount = pMapChann->m_uiLimitMonsterCountsInChannel;
	SendAddData(SendBuf, MSG_SIZE(MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK));

	return RES_RETURN_TRUE;
}

ProcessResult CNPCIOCPSocket::Process_FN_CONNECT_SET_CHANNEL_STATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_CONNECT_SET_CHANNEL_STATE,
										MSG_FN_CONNECT_SET_CHANNEL_STATE, pMsgSetChannelState);

	// check_cmkwon, 채널 상태(enable/disable) 변경 시 처리 부분 구현, 20040907, kelovon
	// 2004-11-20 by cmkwon, NPC Server는 다른 처리가 필요없음
	//ASSERT_NOT_IMPLEMENTED_YET();

	CNPCMapChannel *pMapChann = ms_pNPCIOCP->GetNPCMapChannelByMapChannelIndex(pMsgSetChannelState->MapChannelIndex);
	if(NULL == pMapChann){			return RES_BREAK;}

	pMapChann->SetChannelState(pMsgSetChannelState->EnableChannel);
	return RES_RETURN_TRUE;
}


ProcessResult CNPCIOCPSocket::Process_FN_CONNECT_INCREASE_CHANNEL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_CONNECT_INCREASE_CHANNEL,
									MSG_FN_CONNECT_INCREASE_CHANNEL, pMsgIncreaseChannel);

	// check_cmkwon, 채널(항상 한 채널 씩 증가함) 증가 시 처리 부분 구현, 20040907, kelovon
	ASSERT_NOT_IMPLEMENTED_YET();

	return RES_RETURN_TRUE;
}


ProcessResult CNPCIOCPSocket::Process_FN_CITYWAR_START_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_CITYWAR_START_WAR,
		MSG_FN_CITYWAR_START_WAR, pRMsg);

	m_pNPCMapProject->NPCOnCityWarStartW(pRMsg);
	return RES_RETURN_TRUE;
}
ProcessResult CNPCIOCPSocket::Process_FN_CITYWAR_END_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_CITYWAR_END_WAR,
		MSG_FN_CITYWAR_END_WAR, pRMsg);

	m_pNPCMapProject->NPCOnCityWarEndW(pRMsg);
	return RES_RETURN_TRUE;
}
ProcessResult CNPCIOCPSocket::Process_FN_CITYWAR_CHANGE_OCCUPY_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_CITYWAR_CHANGE_OCCUPY_INFO,
		MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO, pRMsg);

	m_pNPCMapProject->NPCOnCityWarChangeOccupyInfoW(pRMsg);
	return RES_RETURN_TRUE;
}
