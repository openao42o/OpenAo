#include "stdafx.h"
#include "MonitorWinSocket.h"
#include "MonitorNode.h"
#include "assert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonitorWinSocket::CMonitorWinSocket(HWND hWnd, CLeftView *cWnd, UINT wmSock, UINT wmNotify)
:CWinSocket(ST_ALL, hWnd, wmSock, wmNotify, TRUE)
{
	m_bTimeOutFlag = FALSE;
	m_LeftView = cWnd;

	// reset long packet variables
	m_bReceivingLongPacket = FALSE;
	m_LongPacketOffset = 0;
	m_hTreeItem = NULL;
}

CMonitorWinSocket::~CMonitorWinSocket()
{

}

BOOL CMonitorWinSocket::OnCloseSocket(int nErrorCode)
{
	KillTimeOut();
	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CMonitorWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}

BOOL CMonitorWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int		nBytesUsed	= 0;
	int		nType		= 0;
	int		nTypeSize	= 0;
	char*	pRMsg		= NULL;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket + nBytesUsed);

		switch(nType)
		{
		///////////////////////////////////////////////////////////////////////
		// Field Server
		case T_FM_CONNECT_OK:
			{
				nTypeSize = MSG_SIZE(MSG_FM_CONNECT_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_GET_NUM_CLIENTS_OK:
			{
				nTypeSize = MSG_SIZE(MSG_FM_GET_NUM_CLIENTS_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_GET_CHARACTER_INFO_OK:
			{
				nTypeSize = MSG_SIZE(MSG_FM_GET_CHARACTER_INFO_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_SHUTDOWN_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_PING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_SEND_CHANNEL_INFO:
			{
				nTypeSize = MSG_SIZE(MSG_FM_SEND_CHANNEL_INFO);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_CHANGE_BANDWIDTH_FLAG_OK:
			{
				nTypeSize = MSG_SIZE(MSG_FM_CHANGE_BANDWIDTH_FLAG_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_GET_MAP_USER_COUNTS_ACK:
			{
				nTypeSize = MSG_SIZE(MSG_FM_GET_MAP_USER_COUNTS_ACK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_RELOAD_ITEM_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_RESET_NPC_SERVER_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_SERVER_THREAD_STATUS:
			{
				nTypeSize = MSG_SIZE(MSG_FM_SERVER_THREAD_STATUS);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_RELOAD_SERVER_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_SET_SERVICE_STATE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_SET_CHANNEL_STATE_OK:
			{
				nTypeSize = MSG_SIZE(MSG_FM_SET_CHANNEL_STATE_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_FM_CITYWAR_CHANGED_OCCUPY_INFO:
			{
				nTypeSize = MSG_SIZE(MSG_FM_CITYWAR_CHANGED_OCCUPY_INFO);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
			
		///////////////////////////////////////////////////////////////////////
		// NPC Server
		case T_NM_CONNECT_OK:
			{
				nTypeSize = MSG_SIZE(MSG_NM_CONNECT_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_GET_NUM_MONSTERS_OK:
			{
				nTypeSize = MSG_SIZE(MSG_NM_GET_NUM_MONSTERS_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_SHUTDOWN_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_PING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_SEND_CHANNEL_INFO:
			{
				nTypeSize = MSG_SIZE(MSG_NM_SEND_CHANNEL_INFO);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_CHANGE_BANDWIDTH_FLAG_OK:
			{
				nTypeSize = MSG_SIZE(MSG_NM_CHANGE_BANDWIDTH_FLAG_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_GET_MAP_USER_COUNTS_ACK:
			{
				nTypeSize = MSG_SIZE(MSG_NM_GET_MAP_USER_COUNTS_ACK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_SET_LIMIT_MONSTER_COUNT_ACK:
			{
				nTypeSize = MSG_SIZE(MSG_NM_SET_LIMIT_MONSTER_COUNT_ACK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_NM_SERVER_THREAD_STATUS:
			{
				nTypeSize = MSG_SIZE(MSG_NM_SERVER_THREAD_STATUS);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;

				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;

		///////////////////////////////////////////////////////////////////////
		// PRE Server
		case T_PM_CONNECT_OK:
			{
				nTypeSize = MSG_SIZE(MSG_PM_CONNECT_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_GET_NUM_CLIENTS_OK:
			{
				nTypeSize = MSG_SIZE(MSG_PM_GET_NUM_CLIENTS_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_SHUTDOWN_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_PING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_RELOAD_BLOCKED_ACCOUNTS_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_PAUSE_SERVICE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_START_SERVICE_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_RELOAD_VERSION_INFO_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_CHANGE_BANDWIDTH_FLAG_OK:
			{
				nTypeSize = MSG_SIZE(MSG_PM_CHANGE_BANDWIDTH_FLAG_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_GET_SERVER_GROUP_INFO_ACK:
			{
				nTypeSize = MSG_SIZE(MSG_PM_GET_SERVER_GROUP_INFO_ACK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_SERVER_THREAD_STATUS:
			{
				nTypeSize = MSG_SIZE(MSG_PM_SERVER_THREAD_STATUS);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;

				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_PM_AUTO_UPDATE_HTTP_SERVER_SETTING:
			{
				nTypeSize = MSG_SIZE(MSG_PM_AUTO_UPDATE_HTTP_SERVER_SETTING);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;

				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;

		//////////////////////////////////////////////////////////////////
		// Monitor <-- IMServer
		case T_IM_CONNECT_OK:
			{
				nTypeSize = MSG_SIZE(MSG_IM_CONNECT_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_IM_GET_NUM_CLIENTS_OK:
			{
				nTypeSize = MSG_SIZE(MSG_IM_GET_NUM_CLIENTS_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_IM_SHUTDOWN_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_IM_PING_OK:
			{
				nTypeSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_IM_CHANGE_BANDWIDTH_FLAG_OK:
			{
				nTypeSize = MSG_SIZE(MSG_IM_CHANGE_BANDWIDTH_FLAG_OK);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		case T_IM_GET_TOTAL_USER_COUNTS_ACK:
			{
				nTypeSize = MSG_SIZE(MSG_IM_GET_TOTAL_USER_COUNTS_ACK);

				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);

				nBytesUsed += nTypeSize;
			}
			break;
		case T_IM_SERVER_THREAD_STATUS:
			{
				nTypeSize = MSG_SIZE(MSG_IM_SERVER_THREAD_STATUS);
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;

				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		//////////////////////////////////////////////////////////////////
		//
		case T_ERROR:
			{
				nTypeSize = MSG_SIZE(MSG_ERROR) + ((MSG_ERROR*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->StringLength;
				pRMsg = new char[nTypeSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nTypeSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  贸府
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nTypeSize;
			}
			break;
		default:
			{
				// check: unknown message type --> error!, 老窜 message box 角青 & close
				char msg[256];
				sprintf(msg, "[Error]Unable to process Message Type: %s(%#04x) in CMonitorWinSocket::OnRecvdPacket()\n", GetProtocolTypeString(nType), nType);
				MessageBox(NULL, msg, NULL, MB_OK);

				KillTimeOut();
				PostNotify(WS_ERROR, 0, (char*)this);

				return FALSE;
			}
		} // end - switch

		if(NULL != pRMsg){ util::del(pRMsg);}
	}
	return TRUE;
}
