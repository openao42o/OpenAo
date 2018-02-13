// SCAdminWinSocket.cpp: implementation of the CSCAdminWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SCAdminWinSocket.h"
#include "AtumProtocol.h"
#include "SCUserAdminDlg.h"
#include "scserveradmindlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCAdminWinSocket::CSCAdminWinSocket(const char* i_szSocketName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock /* = WM_ASYNC_EVENT */, UINT wmNotify /* = WM_PACKET_NOTIFY */)
:CWinSocket(ST_ALL, hWnd, wmSock, wmNotify, TRUE), m_pMainDlg(i_pMainDlg)
{
	m_szSocketName = i_szSocketName;
}

CSCAdminWinSocket::~CSCAdminWinSocket()
{
}

BOOL CSCAdminWinSocket::OnConnect(int nErrorCode)
{
	if (!CWinSocket::OnConnect(nErrorCode))
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////

	return	TRUE;
}

BOOL CSCAdminWinSocket::OnCloseSocket(int nErrorCode)
{

	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CSCAdminWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int				nBytesUsed	= 0;
	MessageType_t	msgType		= NULL;
	MessageType_t	prevMsgType = NULL;
	int				nMsgSize	= 0;
	char*			pRMsg		= NULL;

	while(nBytesUsed < nLength)
	{
		msgType = *(MessageType_t*)(pPacket + nBytesUsed);

		switch(msgType)
		{
		///////////////////////////////////////////////////////////////////////
		// Pre Server
		case T_PA_ADMIN_CONNECT_OK:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_CONNECT_OK);
			break;
		case T_PA_ADMIN_CHANGE_SERVER_STATE:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_CHANGE_SERVER_STATE);
			break;
		case T_PA_ADMIN_GET_ACCOUNT_INFO_OK:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_INFO_OK);
			break;
		case T_PA_ADMIN_DISCONNECT_USER_OK:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_DISCONNECT_USER_OK);
			break;
		case T_PA_ADMIN_BLOCK_ACCOUNT_OK:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_BLOCK_ACCOUNT_OK);
			break;
		case T_PA_ADMIN_UNBLOCK_ACCOUNT_OK:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK);
			break;
		case T_PA_ADMIN_GET_SERVER_INFO_OK:
// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
//			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK)
//				+ sizeof(MEX_SERVER_INFO_FOR_ADMIN) * ((MSG_PA_ADMIN_GET_SERVER_INFO_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfServers;
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_GET_SERVER_INFO_OK)
				+ sizeof(MEX_SERVER_GROUP_INFO_FOR_ADMIN) * ((MSG_PA_ADMIN_GET_SERVER_INFO_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfServerGroups;
			break;
		case T_PA_ADMIN_GET_ACCOUNT_LIST_OK:
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK)
				+ sizeof(MEX_SIMPLE_ACCOUNT_INFO_FOR_ADMIN) * ((MSG_PA_ADMIN_GET_ACCOUNT_LIST_OK*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->NumOfAccounts;
			break;
		case T_PA_ADMIN_SET_DBSERVER_GROUP_OK:		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
			nMsgSize = MSG_SIZE(MSG_PA_ADMIN_SET_DBSERVER_GROUP_OK);
			break;

		///////////////////////////////////////////////////////////////////////
		// IM Server
		case T_IA_ADMIN_CONNECT_OK:
			nMsgSize = MSG_SIZE(MSG_IA_ADMIN_CONNECT_OK);
			break;
		case T_IA_ADMIN_CHANGE_SERVER_STATE:
			nMsgSize = MSG_SIZE(MSG_IA_ADMIN_CHANGE_SERVER_STATE);
			break;
		case T_IA_ADMIN_GET_CHARACTER_OK:
			nMsgSize = MSG_SIZE(MSG_IA_ADMIN_GET_CHARACTER_OK);
			break;
		case T_IA_ADMIN_GET_SERVER_INFO_OK:
			nMsgSize = MSG_SIZE(MSG_IA_ADMIN_GET_SERVER_INFO_OK);
			break;
		case T_IC_STRING_128:
			nMsgSize = MSG_SIZE(MSG_IC_STRING_128);
			break;
		///////////////////////////////////////////////////////////////////////
		// Field Server
		case T_FA_ADMIN_CONNECT_OK:
			nMsgSize = MSG_SIZE(MSG_FA_ADMIN_CONNECT_OK);
			break;
		case T_FA_ADMIN_CHANGE_SERVER_STATE:
			nMsgSize = MSG_SIZE(MSG_FA_ADMIN_CHANGE_SERVER_STATE);
			break;
		///////////////////////////////////////////////////////////////////////
		// ERROR
		case T_ERROR:
			nMsgSize = MSG_SIZE(MSG_ERROR) + ((MSG_ERROR*)(pPacket+nBytesUsed+SIZE_FIELD_TYPE_HEADER))->StringLength;
			break;
		default:
			{
				// check: unknown message type --> error!, 일단 message box 실행 & close
				char msg[256];
				sprintf(msg, "[WinSocket Error]Unhandled Message Type: %s(%#04X)\n", GetProtocolTypeString(msgType), msgType);
				m_pMainDlg->MessageBox(msg);
				CloseSocket(0);
				m_pMainDlg->EndDialog(-1);
				return FALSE;
			}
		} // end - switch

		pRMsg = new char[nMsgSize];
		memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
		EnterCriticalSection(&m_criticalRecvMessageQueue);
		m_queueRecvMessage.push(pRMsg);
		LeaveCriticalSection(&m_criticalRecvMessageQueue);
		pRMsg = NULL;

		// 처리
		PostNotify(WS_RECEIVED, 0, (char*)this);
		nBytesUsed += nMsgSize;

		prevMsgType = msgType;

		util::del_array(pRMsg);
	}

	return TRUE;
}

BOOL CSCAdminWinSocket::OnWS_Closed(void)
{
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here
	CString szMessage;
	szMessage.Format(STRMSG_S_SCADMINTOOL_0049, m_szSocketName, m_szPeerIP);
	MessageBox(NULL, szMessage, NULL, MB_OK);
	m_pMainDlg->EndDialog(-1);
	//
	//////////////////////////////////////////////////////////////////////
	return TRUE;
}

CSCAdminPreWinSocket::CSCAdminPreWinSocket(const char* i_szSocketName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock /* = WM_PRE_ASYNC_EVENT */, UINT wmNotify /* = WM_PRE_PACKET_NOTIFY */)
: CSCAdminWinSocket(i_szSocketName, i_pMainDlg, hWnd, wmSock, wmNotify)
{
}

CSCAdminIMWinSocket::CSCAdminIMWinSocket(const char* i_szSocketName, char *i_szServerGroupName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock /* = WM_IM_ASYNC_EVENT */, UINT wmNotify /* = WM_IM_PACKET_NOTIFY */)
: CSCAdminWinSocket(i_szSocketName, i_pMainDlg, hWnd, wmSock, wmNotify)
{
	util::strncpy(m_szServerGroupName1, i_szServerGroupName, SIZE_MAX_SERVER_NAME);
	m_UserCount = 0;
}

CSCAdminFieldWinSocket::CSCAdminFieldWinSocket(const char* i_szSocketName, char *i_szServerGroupName, CDialog *i_pMainDlg, HWND hWnd, UINT wmSock /* = WM_FIELD_ASYNC_EVENT */, UINT wmNotify /* = WM_FIELD_PACKET_NOTIFY */)
: CSCAdminWinSocket(i_szSocketName, i_pMainDlg, hWnd, wmSock, wmNotify)
{
	util::strncpy(m_szServerGroupName1, i_szServerGroupName, SIZE_MAX_SERVER_NAME);	
}
