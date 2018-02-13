// IMSocketManager.cpp: implementation of the CIMSocketManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "IMSocket.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "Chat.h"

#include "IMSocketManager.h"

// 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가
#define MIN_CHATCAP_SECOND	5		// 채팅간 최소 간격
#define MIN_CHATCAP_COUNT	5		// 최대 채팅 허용 수
#define FORBID_CHAT_TIME	60		// 채팅 금지 시간(1분=60초)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMSocketManager::CIMSocketManager(HWND hwnd)
{	
	m_pMainIMSocket = new CIMSocket(hwnd, WM_IM_ASYNC, WM_IM_NOTIFY);
	g_pIMSocket = this;

	m_pArenaIMSocket = NULL;
	
	m_hwnd = hwnd;

	// 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가
	m_timeLastSendChat = GetServerDateTime();
	m_timeLastSendChat.Year = 0;
	m_timeLastSendChat.Month = 0;
	m_timeLastSendChat.Day = 0;
	m_timeLastSendChat.Hour = 0;
	m_timeLastSendChat.Minute = 0;
	m_timeLastSendChat.Second = 0;
	
	m_timeForbidSendChat = GetServerDateTime();	// 금지되는 시간	
	m_timeForbidSendChat.Year = 0;
	m_timeForbidSendChat.Month = 0;
	m_timeForbidSendChat.Day = 0;
	m_timeForbidSendChat.Hour = 0;
	m_timeForbidSendChat.Minute = 0;
	m_timeForbidSendChat.Second = 0;

	m_nMinChatCount = 0;

	// 2009. 06. 17 by ckPark Frost System
#if defined( DEFINE_USE_GAMEGUARD_FROST )
	m_pMainIMSocket->SetFrostEnable( TRUE );
#endif
	// end 2009. 06. 17 by ckPark Frost System
}

CIMSocketManager::~CIMSocketManager()
{
	util::del(m_pMainIMSocket);
	util::del(m_pArenaIMSocket);
	g_pIMSocket = NULL;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인 IM 서버와 연결
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::Connect(LPCSTR strPeerIP, int nPort)
{
	return m_pMainIMSocket->Connect(strPeerIP, nPort);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인 IM 서버와 연결종료
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::CloseSocket()
{
	if(IsIMArenaServerConnected())
	{
		CloseArenaIMSocket();
	}
	return m_pMainIMSocket->CloseSocket();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		IM서버쪽에 패킷을 쏜다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::Write(LPCSTR pPacket, int nLength, BOOL bArenaSend)
{
	if(IsArenaLoadSuccess() || bArenaSend)
	{
		if(IsIMArenaServerConnected())
		{
			return m_pArenaIMSocket->Write(pPacket, nLength);		
		}		
	}
	else
	{
		return m_pMainIMSocket->Write(pPacket, nLength);
	}	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		IM서버쪽에 패킷을 쏜다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::Write(BYTE *pPacket, int nLength) 
{ 
	if(IsArenaLoadSuccess())
	{
		if(IsIMArenaServerConnected())
		{
			return m_pArenaIMSocket->Write(pPacket, nLength);
		}
	}
	else
	{
		return m_pMainIMSocket->Write(pPacket, nLength);	
	}
	return TRUE;	
}	

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		IM서버쪽에 패킷을 쏜다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::SendMsg( int nType, char *pPacket, int nSize )
{
	BOOL bBothSend = FALSE;
	BOOL bArenaSend = FALSE;
	switch(nType)
	{
	case T_IC_CONNECT_ALIVE:	// 클라이언트가 살아있음을 알려줌
		{
			bArenaSend = TRUE;
			bBothSend = TRUE;
		}
		break;
	case T_IC_PARTY_LEAVE_FROM_M_TO_A:					// Main서버에서 Arena서버로 올떄 날림
	case T_IC_PARTY_REQUEST_PARTYINFO_FROM_A_TO_M:		// Arena서버에서 Main서버로 올떄 날림	
		{
			bArenaSend = FALSE;
		}
		break;
	case T_IC_PARTY_LEAVE_FROM_A_TO_M:
	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	case T_IC_PARTY_GET_AUTO_PARTY_INFO:
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		{
			bArenaSend = TRUE;
		}
		break;
	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 파티관련 패킷도 아레나서버로 전송이 가능하게 수정
	case T_IC_PARTY_RECOMMENDATION_MEMBER:
	case T_IC_PARTY_CHANGE_INFO:
	case T_IC_PARTY_LIST_INFO:
	case T_IC_PARTY_JOIN_FREE:
		{
#ifdef S_ARENA_EX_1ST_JHSEOL
			bArenaSend = TRUE;
			bBothSend = TRUE;
#endif
		}
		break;
	/// end 2012-04-12 by jhseol, 아레나 추가개발 - 자동파티 : 파티관련 패킷도 아레나서버로 전송이 가능하게 수정
	///////////////////////////////////////////////////////////////////////////////////////////////////
	}	

	if(bArenaSend )
	{
		BOOL bRtn = FALSE;
		if(IsIMArenaServerConnected())
		{
			bRtn = m_pArenaIMSocket->SendMsg( nType, pPacket, nSize );
		}
		if(FALSE == bBothSend)
		{
			return bRtn;
		}		
		return m_pMainIMSocket->SendMsg( nType, pPacket, nSize );
	}
	else
	{
		return m_pMainIMSocket->SendMsg( nType, pPacket, nSize );
	}
	return TRUE;
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		IM서버쪽에 패킷을 쏜다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::WriteMessageType(MessageType_t msgType)
{
	if(IsArenaLoadSuccess())
	{
		if(IsIMArenaServerConnected())
		{
			return m_pArenaIMSocket->WriteMessageType(msgType);
		}
	}
	else
	{
		return m_pMainIMSocket->WriteMessageType(msgType);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		IM서버쪽에 채팅 메시지를 뿌린다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMSocketManager::SendChat(int nType, const char* strCharacter, const char* strChat)
{
	BOOL bSendArenaServer = FALSE;
	BOOL bNoSend = FALSE;
	switch(nType)
	{
	case T_IC_CHAT_ALL:		// 공지사항
	case T_IC_CHAT_GUILD:	// 길드채팅	
	case T_IC_CHAT_MAP:		// 맵채팅
	case T_IC_CHAT_SELL_ALL:	// 전체 채팅($)
	case T_IC_CHAT_CASH_ALL:	// 유료 맵 채팅(&)
	case T_IC_CHAT_PTOP:		// 귓속말 채팅 
	case T_IC_CHAT_WAR:
#ifdef _CROSS_NATION_CHAT
	case T_IC_CHAT_CNC:
#endif
	case T_IC_CHAT_CHATROOM:
	case T_IC_CHAT_INFLUENCE_ALL:	// 지도자 채팅
		{

		}
		break;			
	case T_IC_CHAT_PARTY:	// 편대채팅	
		{
			if((IsArenaLoadSuccess())
				&& (IsIMArenaServerConnected()))
			{
				bSendArenaServer = TRUE;
			}
		}
		break;
	case T_IC_CHAT_REGION:	// 지역 채팅	
		{
			BOOL bAdmin = 
				COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR);
			if(IsIMArenaServerConnected())
			{
				if(IsArenaLoadSuccess())
				{
					bSendArenaServer = TRUE;
				}
				else if(bAdmin && IsOnlySendArenaCmd(strChat))
				{
					bSendArenaServer = TRUE;
				}
			}
			else if(bAdmin && IsOnlySendArenaCmd(strChat))
			{			
				bNoSend = TRUE;
				g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080225_0205,COLOR_ERROR);//"먼저 아레나 서버와 연결해주세요"
			}
		}
		break;

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	case T_IC_CHAT_INFINITY:
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
	case T_IC_CHAT_ARENA:	// 아레나 채팅
		{
			if(IsIMArenaServerConnected())
			{
				bSendArenaServer = TRUE;
			}
			else
			{
				bNoSend = TRUE;
			}

		}
		break;
	}
	if(bNoSend)
	{
		return;
	}
	
	// 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가
	if(!IsPossibleSendChat())
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_080711_0200,COLOR_ERROR);//"\\y잠시동안 채팅을 이용하실 수 없습니다."
		return;
	}

	// end 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가


	if(bSendArenaServer)
	{
		m_pArenaIMSocket->SendChat(nType, g_pShuttleChild->GetShuttleInfo()->Color, strCharacter, strChat);
	}
	else
	{
		m_pMainIMSocket->SendChat(nType, g_pShuttleChild->GetShuttleInfo()->Color, strCharacter, strChat);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인IM서버와 연결이 되어있냐?
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::IsConnected()
{
	return m_pMainIMSocket->IsConnected();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인IM서버와 연결에 필요한 이벤트 처리
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CIMSocketManager::OnAsyncEvent(LONG lParam)
{
	return m_pMainIMSocket->OnAsyncEvent(lParam);

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인IM서버와의 패킷을 읽는다. 
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIMSocketManager::Read(LPSTR *pPacket, int &nLength, int nSocketNotifyType)
{
	if(WM_IM_ARENA_NOTIFY == nSocketNotifyType)
	{
		if(m_pArenaIMSocket)
		{
			return m_pArenaIMSocket->Read(pPacket, nLength);
		}
	}	
	else
	{
		return m_pMainIMSocket->Read(pPacket, nLength);
	}
	return 0;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		길드 정보를 읽는다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMSocketManager::OnRecvdGetGuildOK(MSG_IC_CHAT_GET_GUILD_OK* pMsg, int nSocketNotifyType)
{
	if(WM_IM_ARENA_NOTIFY == nSocketNotifyType)
	{
		m_pArenaIMSocket->OnRecvdGetGuildOK(pMsg);
	}
	else
	{
		m_pMainIMSocket->OnRecvdGetGuildOK(pMsg);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		바뀐 길드 정보를 읽는다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMSocketManager::OnRecvdChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg, int nSocketNotifyType)
{
	if(WM_IM_ARENA_NOTIFY == nSocketNotifyType)
	{
		m_pArenaIMSocket->OnRecvdChangeGuild(pMsg);
	}
	else
	{
		m_pMainIMSocket->OnRecvdChangeGuild(pMsg);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		바뀐 파티정보를 갱신한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMSocketManager::OnRecvdChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg, int nSocketNotifyType)
{
	if(WM_IM_ARENA_NOTIFY == nSocketNotifyType)
	{
		m_pArenaIMSocket->OnRecvdChangeParty(pMsg);
	}
	else
	{
		m_pMainIMSocket->OnRecvdChangeParty(pMsg);
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인서버에 케릭터 정보 갱신
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMSocketManager::SetChaterInfo(CHARACTER* pInfo)
{
	m_pMainIMSocket->SetChaterInfo(pInfo);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버에 접속 시도
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::ConnectArena(LPCSTR strPeerIP, int nPort, BOOL* bAlReadyConnect)
{
	if(NULL == m_pArenaIMSocket)
	{			
		m_pArenaIMSocket = new CIMSocket(m_hwnd, WM_IM_ARENA_ASYNC, WM_IM_ARENA_NOTIFY);		
	}
	(*bAlReadyConnect) = FALSE;
	if(m_pArenaIMSocket->IsConnected())
	{
		(*bAlReadyConnect) = TRUE;
		return TRUE;
	}
	BOOL bConnect = m_pArenaIMSocket->Connect(strPeerIP, nPort);
	return bConnect;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버에 접속에 필요한 이벤트 처리
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CIMSocketManager::OnArenaAsyncEvent(LONG lParam)
{
	if(NULL == m_pArenaIMSocket)
	{
		return 0;
	}

	return m_pArenaIMSocket->OnAsyncEvent(lParam);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 IM서버에 접속을 끊는다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::CloseArenaIMSocket()
{
	DBGOUT("CIMSocketManager::CloseArenaIMSocket\n");
	BOOL bRtn = TRUE;
	if(IsIMArenaServerConnected())
	{
		bRtn = m_pArenaIMSocket->CloseSocket();				
	}
	return bRtn;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		적합한 소켓을 얻어온다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CIMSocket* CIMSocketManager::GetIMWinSocket(UINT nSocketNotifyType)
{	
	if(WM_IM_ARENA_NOTIFY == nSocketNotifyType)
	{
		return GetArenaIMSocket();
	}
	return m_pMainIMSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 소켓을 리턴
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CIMSocket* CIMSocketManager::GetArenaIMSocket()
{
	return m_pArenaIMSocket;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 IM서버와 연결되었는지 여부 판단
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::IsIMArenaServerConnected()
{
	if(m_pArenaIMSocket && m_pArenaIMSocket->IsConnected())
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 상태인지 판단
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::IsArenaLoadSuccess()
{
	if((g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_LOAD_GAME_INFO )
		|| (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING ))
	{
		return TRUE;
	}
	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 케릭터 상태 정보 갱신
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIMSocketManager::SetArenaChaterInfo(CHARACTER* pInfo)
{
	m_pArenaIMSocket->SetChaterInfo(pInfo);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 IM서버에만 보내는 커맨드인지 판단
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-03-10 ~ 2008-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::IsOnlySendArenaCmd(const char* pChat)
{
	if (0 == COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		// 관리자가 아니면 보내면 안된다.
		return FALSE;
	}

	char chCmd[256];
	memset(chCmd, 0x00, 256);
	int nCnt = 0;

	for(nCnt = 0; nCnt< strlen(pChat);nCnt++)
	{
		if(pChat[nCnt] == ' ')		// || m_strInputMessage[n] == '\n')
			break;
		chCmd[nCnt] = pChat[nCnt];
	}	

	if ((stricmp( chCmd, STRCMD_CS_COMMAND_ARENAEVENT_0 )==0)
		||(stricmp( chCmd, STRCMD_CS_COMMAND_ARENAEVENT_1 )==0)
		||(stricmp( chCmd, STRCMD_CS_COMMAND_ARENAEVENT_2 )==0)
		||(stricmp( chCmd, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_0 )==0)
		||(stricmp( chCmd, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_1 )==0)
		||(stricmp( chCmd, STRCMD_CS_COMMAND_ARENAEVENTRELEASE_2 )==0))
	{
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-07-11 by bhsohn 과도한 채팅시, 채팅금지 시스템 추가
/// \date		2008-07-11 ~ 2008-07-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIMSocketManager::IsPossibleSendChat()
{
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();		
	// 지도자 및 부지도자 혹은 관리자는 채팅 금지에서 제외된다.
	if(COMPARE_RACE(myShuttleInfo.Race, RACE_INFLUENCE_LEADER)
		||COMPARE_RACE(myShuttleInfo.Race, RACE_INFLUENCE_SUBLEADER_1)
		||COMPARE_RACE(myShuttleInfo.Race, RACE_INFLUENCE_SUBLEADER_2)
		|| COMPARE_RACE(myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		return TRUE;
	}
	
	
	ATUM_DATE_TIME	currenttime = GetServerDateTime();
	if(m_timeForbidSendChat.Year != 0)
	{
		if(currenttime < m_timeForbidSendChat)
		{
			// 금지 시간이다.
			return FALSE;
		}
	}
	
	ATUM_DATE_TIME	oldtime = m_timeLastSendChat;

	if(currenttime < oldtime || (0 == m_timeLastSendChat.Year) )
	{
		m_timeLastSendChat = currenttime;
		return TRUE;
	}
	int nSecondDiff = currenttime.GetTimeInSeconds() - oldtime.GetTimeInSeconds();
	if(nSecondDiff < MIN_CHATCAP_SECOND)
	{
		m_nMinChatCount++;		
		if(m_nMinChatCount >= MIN_CHATCAP_COUNT)
		{
			m_timeForbidSendChat = currenttime;
			m_timeForbidSendChat.AddDateTime(0,0,0,0,0, FORBID_CHAT_TIME);	// 1분동안 채팅 금지
			return FALSE;
		}
	}
	else
	{
		m_timeForbidSendChat.Year = 0;
		m_timeForbidSendChat.Month = 0;
		m_timeForbidSendChat.Day = 0;
		m_timeForbidSendChat.Hour = 0;
		m_timeForbidSendChat.Minute = 0;
		m_timeForbidSendChat.Second = 0;

		m_timeForbidSendChat.Year = 0;
		m_timeLastSendChat = currenttime;
		m_nMinChatCount = 0;
	}
	
	
	return TRUE;

}