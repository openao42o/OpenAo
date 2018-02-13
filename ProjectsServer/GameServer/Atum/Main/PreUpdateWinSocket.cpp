// UpdateSocket.cpp: implementation of the CPreUpdateWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PreUpdateWinSocket.h"
#include "AtumProtocol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreUpdateWinSocket::CPreUpdateWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify) : CWinSocket { ST_PRE_SERVER, hWnd, wmSock, wmNotify, TRUE }
{
}

CPreUpdateWinSocket::~CPreUpdateWinSocket()
{
}

BOOL CPreUpdateWinSocket::OnCloseSocket(int nErrorCode)
{
	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CPreUpdateWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);

	return	TRUE;
}

BOOL CPreUpdateWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int		nBytesUsed	= 0;
	int		nType		= 0;
	int		nMsgSize	= 0;
	char*	pRMsg		= NULL;

	while(nBytesUsed < nLength)
	{
		memcpy(&nType, pPacket + nBytesUsed, SIZE_FIELD_TYPE_HEADER);
		switch(nType)
		{
		case T_PC_DEFAULT_UPDATE_LAUNCHER_VERSION_OK:	// No Body
			{
				nMsgSize = SIZE_FIELD_TYPE_HEADER;
				pRMsg = new char[nMsgSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  처리
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nMsgSize;
			}
			break;
		case T_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO:
			{
				nMsgSize = MSG_SIZE(MSG_PC_DEFAULT_UPDATE_LAUNCHER_UPDATE_INFO);
				pRMsg = new char[nMsgSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  처리
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nMsgSize;
			}
			break;
		case T_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO:		// 2007-01-18 by cmkwon
			{
				nMsgSize = MSG_SIZE(MSG_PC_DEFAULT_NEW_UPDATE_LAUNCHER_UPDATE_INFO);
				pRMsg = new char[nMsgSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  처리
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nMsgSize;
			}
			break;
// 2007-09-06 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 아래의 추가된 프로토콜로 처리됨
//		case T_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK:		// 2007-05-15 by cmkwon
//			{
//				nMsgSize = MSG_SIZE(MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK);
//				pRMsg = new char[nMsgSize];
//				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
//				EnterCriticalSection(&m_criticalRecvMessageQueue);
//				m_queueRecvMessage.push(pRMsg);
//				LeaveCriticalSection(&m_criticalRecvMessageQueue);
//				pRMsg = NULL;
//				//  처리
//				PostNotify(WS_RECEIVED, 0, (char*)this);
//				nBytesUsed += nMsgSize;
//			}
//			break;
		case T_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK:		// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 추가된 프로토콜 처리
			{
				nMsgSize = MSG_SIZE(MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK);
				pRMsg = new char[nMsgSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  처리
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nMsgSize;
			}
			break;
		
		case T_ERROR:
			{
				nMsgSize = MSG_SIZE(MSG_ERROR) + ((MSG_ERROR*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->StringLength;
				pRMsg = new char[nMsgSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				pRMsg = NULL;
				//  처리
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nMsgSize;
			}
			break;
		default:
			{
				// check: unknown message type --> error!, 일단 message box 실행 & close
				char msg[256];
#if true // def _ATUM_DEVELOP
				sprintf(msg, "[Error] Unhandled Message Type: %s(%#04X)\n", GetProtocolTypeString(nType), nType);
#else
				sprintf(msg, "[Error] Unhandled Message Type!\n");
#endif
				MessageBox(NULL, msg, NULL, MB_OK);
				exit(0);
				//break;
			}
		} // end - switch

		util::del(pRMsg);
	}
	return TRUE;
}
