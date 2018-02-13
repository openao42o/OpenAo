// PreIMWinSocket.cpp: implementation of the CPreIMWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PreIMWinSocket.h"
#include "IOCP.h"
#include "IMIOCP.h"
#include "IMGlobal.h"
#include "AtumError.h"
#include "AtumIMDBManager.h"		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreIMWinSocket::CPreIMWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify)
:CWinSocket(ST_PRE_SERVER, hWnd, wmSock, wmNotify, FALSE)
{

}

CPreIMWinSocket::~CPreIMWinSocket()
{

}

void CPreIMWinSocket::SetIMIOCP(CIMIOCP *pIMIOCP)
{
	m_pIMIOCP3 = pIMIOCP;
}


BOOL CPreIMWinSocket::OnCloseSocket(int nErrorCode)
{
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CPreIMWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}

BOOL CPreIMWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int				nBytesUsed	= 0;
	int				nMsgSize	= 0;
	MessageType_t	nType;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket + nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nType, m_szPeerIP, ST_PRE_SERVER, GGetexchangeMsgPrintLevel());
#endif

		switch(nType)
		{
		case T_IP_CONNECT_IM_CONNECT_OK:	// no body
			{
				DBGOUT("IMServer successfully registered to PreServer...\n");
			}
			break;
		case T_IP_GET_SERVER_GROUP_INFO:
			{
				INIT_MSG_WITH_BUFFER(MSG_IP_GET_SERVER_GROUP_INFO_ACK, T_IP_GET_SERVER_GROUP_INFO_ACK, pSAck, SendBuf);
				util::strncpy(pSAck->ServerGroupName, g_pIMGlobal->GetServerGroupName(), SIZE_MAX_SERVER_NAME);
				pSAck->IMCurrentUserCounts = m_pIMIOCP3->m_MapChactarterUniqueNumber.size();
				pSAck->IMMaxUserCounts = m_pIMIOCP3->m_uiIMMaxTotalUserCounts;
				Write(SendBuf, MSG_SIZE(MSG_IP_GET_SERVER_GROUP_INFO_ACK));
			}
			break;
		case T_IP_ADMIN_PETITION_SET_PERIOD:
			{// 2007-11-19 by cmkwon, 진정시스템 업데이트 - 
				MSG_IP_ADMIN_PETITION_SET_PERIOD *pRMsg = (MSG_IP_ADMIN_PETITION_SET_PERIOD*)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_IP_ADMIN_PETITION_SET_PERIOD);

				if(pRMsg->byIsImmediatOn)
				{
					pRMsg->atStart.SetCurrentDateTime();
					pRMsg->atEnd	= pRMsg->atStart;
					pRMsg->atEnd.AddDateTime(0, 0, 14);	// 2007-11-20 by cmkwon, 14일
				}
				else if(pRMsg->byIsImmediatOff)
				{
					pRMsg->atStart	= m_pIMIOCP3->m_atCallGMStartTime;
					pRMsg->atEnd.SetCurrentDateTime(TRUE);
				}
				m_pIMIOCP3->SetCallGMSystemTime(&pRMsg->atStart, &pRMsg->atEnd);
			}
			break;

		case T_IP_ADMIN_RELOAD_ADMIN_NOTICE_SYSTEM:		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - AdminTool에 의해 DB에서 다시 로드, 즉시 시작
			{
				m_pIMIOCP3->m_pAtumIMDBManager->MakeAndEnqueueQuery(QT_ReloadAdminAutoNotice, NULL, NULL, NULL, TRUE);
			}
			break;

		// 2012-07-11 by hskim, 선택적 셧다운
		case T_IP_SELECTIVE_SHUTDOWN_NOTICE:
			{
				MSG_IP_SELECTIVE_SHUTDOWN_NOTICE *pRMsg = (MSG_IP_SELECTIVE_SHUTDOWN_NOTICE *)(pPacket + nBytesUsed);
				nBytesUsed += sizeof(MSG_IP_SELECTIVE_SHUTDOWN_NOTICE);

				CIMIOCPSocket *pSocket = m_pIMIOCP3->GetIMIOCPSocketByAccountUID(pRMsg->AccountUID);
				
				if( NULL != pSocket)
				{
					pSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_120712_0001, pRMsg->nRemainingTime);
				}
			}
			break;
		// end 2012-07-11 by hskim, 선택적 셧다운

		case T_ERROR:
			{
				MSG_ERROR	*pMsgRecvErr;
				pMsgRecvErr = (MSG_ERROR*)(pPacket + nBytesUsed);
				nBytesUsed += (sizeof(MSG_ERROR) + pMsgRecvErr->StringLength);
				//  처리
				g_pGlobal->WriteSystemLogEX(TRUE, "  T_ERROR %s(%#04X) received from %s[%s]\r\n", GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode, "ST_PRE_SERVER", m_szPeerIP);
			}
			break;
		default:
			{	// check: unknown message type --> error!
				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CPreIMWinSocket::OnRecvdPacket Unknown Message Type, %d (0x%08X)\r\n"
					, nType, nType);
				g_pIMGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return TRUE;
			}
		}
	}
	return TRUE;
}
