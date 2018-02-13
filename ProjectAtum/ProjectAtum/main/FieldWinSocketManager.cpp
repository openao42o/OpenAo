// FieldWinSocketManager.cpp: implementation of the CFieldWinSocketManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FieldWinSocket.h"
#include "AtumApplication.h"

#include "FieldWinSocketManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldWinSocketManager::CFieldWinSocketManager(HWND hwnd)
{		
	m_hMainWnd = hwnd;

	//CFieldWinSocket(HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY);
	m_pMainFieldWinSocket = new CFieldWinSocket(hwnd, WM_ASYNC_EVENT, WM_PACKET_NOTIFY);	
	m_pArenaFieldWinSocket = NULL;
	g_pFieldWinSocket = this;

	// 요청에 의한 스켓 끊기냐?
	m_bRequestArenaSocketClose = FALSE;

	// 2009. 06. 17 by ckPark Frost System
#if defined( DEFINE_USE_GAMEGUARD_FROST )
	m_pMainFieldWinSocket->SetFrostEnable( TRUE );
#endif
	// end 2009. 06. 17 by ckPark Frost System
}

CFieldWinSocketManager::~CFieldWinSocketManager()
{
	g_pFieldWinSocket = NULL;
	util::del(m_pMainFieldWinSocket);
	util::del(m_pArenaFieldWinSocket);	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		실제 섭에 연결
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::Connect(LPCSTR strPeerIP, int nPort)
{
	return m_pMainFieldWinSocket->Connect(strPeerIP, nPort);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인서버및 아레나 서버에 접속끊음
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::CloseSocket()
{
	DBGOUT("BOOL CFieldWinSocketManager::CloseSocket() \n");
	if(IsFCConnectedArenaServer())
	{
		// 아레나 소켓 Close
		CloseSocketArena();
	}
	return m_pMainFieldWinSocket->CloseSocket();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 상태를 판단하여 패킷을 어디에 날릴지 정한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::Write(LPCSTR pPacket, int nLength)
{
	if(IsArenaLoadSuccess())
	{
		if(IsFCConnectedArenaServer())
		{
			return m_pArenaFieldWinSocket->Write(pPacket, nLength);		
		}
	}
	else
	{
		return m_pMainFieldWinSocket->Write(pPacket, nLength);	
	}
	return 0;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 상태를 판단하여 패킷을 어디에 날릴지 정한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::Write(BYTE *pPacket, int nLength)
{
	if(IsArenaLoadSuccess())
	{
		if(IsFCConnectedArenaServer())
		{
			return m_pArenaFieldWinSocket->Write(pPacket, nLength);		
		}		
	}	
	else
	{
		return m_pMainFieldWinSocket->Write(pPacket, nLength);
	}
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		패킷별로 어디에 날릴지 결정한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::SendMsg( int nType, char *pPacket, int nSize )
{	
	BOOL bArenaSend = FALSE;
	BOOL bBothSend = FALSE;		// 양쪽 다 보내는 메시지
	switch(nType)
	{	
	case T_FC_CONNECT_ARENASERVER_LOGIN:		// 아레나 필드 서버 로그인
	case T_FC_ARENA_REQUEST_TEAM:				// 아레나 팀요청
	case T_FC_ARENA_CREATE_TEAM:				// 아레나 팀 생성
	case T_FC_ARENA_ENTER_TEAM:					// 팀 조인
	case T_FC_ARENA_REAVE_TEAM:					// 팀 탈퇴	
	case T_FC_ARENA_ENTER_ROOM_WARP:			// 아레나 맵으로 이동	
	case T_FC_ARENA_REQUEST_CREATE_TEAMINFO:	// 아레나 팀
	case T_FC_ARENA_QUICK_ENTER_TEAM:			// 빠른 신청
	case T_FC_ARENA_TEAM_READY_FINISH:			// 아레나 팀 레디
	case T_FC_ARENA_TEAM_READY_FINISH_CANCEL:	// 아레나 팀 레디 취소
	case T_FC_ARENA_CHARACTER_GAMESTART:		// 클라이언트가 아레나 게임 스타트 		
	case T_FC_ARENA_FINISH_WARP:				// 아레나 종료 마을로 이동
	case T_FC_CONNECT_ARENASERVER_TO_IMSERVER:				// 아레나 서버 IM서버와의 접속이 완료되었다.
		{
			bArenaSend = TRUE;			
		}
		break;		
	case T_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER: // 아레나 종료를 알려줌		
	// 2010. 04. 22 by ckPark 해킹툴 관련 패킷은 아레나가 아닌 메인 필드 서버로 보내도록 변경
	case T_FC_MOVE_HACKSHIELD_CRCAckMsg:
	case T_FC_MOVE_HACKSHIELD_HACKING_CLIENT:
	// end 2010. 04. 22 by ckPark 해킹툴 관련 패킷은 아레나가 아닌 메인 필드 서버로 보내도록 변경
		{
			bArenaSend = FALSE;
		}
		break;		
	case T_FC_CHARACTER_GAMEEND:// 게임 종료
	case T_FC_CHARACTER_GET_CHARACTER:	// 캐릭터 정보 요청
		{
		}
		break;
	case T_FC_CONNECT_ALIVE:// 게임이 현재 살아있다.
	case T_FC_ARENA_PLAY_CHARACTER_TYPE:	// 2012-06-04 by mspark, 아레나 기어 성향 UI 작업
		{
			bArenaSend = TRUE;			
			bBothSend = TRUE;
		}
		break;

	// 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템
		case T_FC_INFINITY_MODE_LIST:
		case T_FC_INFINITY_READY_LIST:
		case T_FC_INFINITY_CREATE:
		case T_FC_INFINITY_MEMBER_INFO_LIST:
		case T_FC_INFINITY_LEAVE:
		case T_FC_INFINITY_BAN:
		case T_FC_INFINITY_JOIN:
		case T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK:
		case T_FC_INFINITY_READY:
		case T_FC_INFINITY_READY_CANCEL:
		case T_FC_INFINITY_START:
		case T_FC_INFINITY_MAP_LOADED:
		case T_FC_INFINITY_FIN_OK:
		case T_FC_INFINITY_JOIN_CANCEL:
		case T_FC_INFINITY_RESTART_BY_DISCONNECT:
		case T_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT:

		// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		case T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL :

		// 2010. 07. 27 by hsLee 인피니티 2차 거점 방어 시네마 연출 스킵 처리.
		case T_FC_INFINITY_SKIP_ENDING_CINEMA :

		{
			bArenaSend = TRUE;
		}
		break;
	// end 2009. 11. 02 by ckPark 인피니티 필드 인스턴스 던젼 시스템

	default:
		{
			if(IsArenaLoadSuccess())
			{
				bArenaSend = TRUE;
			}
		}
		break;
	}
	if(bArenaSend)
	{
		BOOL bRtn = FALSE;
		if(IsFCConnectedArenaServer())
		{			
			bRtn = m_pArenaFieldWinSocket->SendMsg( nType, pPacket, nSize );					
		}		
		if(FALSE == bBothSend)
		{
			// 양쪽 다 보내는 메시지가 아니면 리턴
			return bRtn;
		}		
		return m_pMainFieldWinSocket->SendMsg( nType, pPacket, nSize );
	}
	else
	{
		return m_pMainFieldWinSocket->SendMsg( nType, pPacket, nSize );		
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 상태를 판단하여 패킷을 어디에 날릴지 정한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::WriteMessageType(MessageType_t msgType)
{
	if(IsArenaLoadSuccess())
	{
		if(IsFCConnectedArenaServer())
		{
			return m_pArenaFieldWinSocket->WriteMessageType(msgType);		
		}
	}
	else
	{
		return m_pMainFieldWinSocket->WriteMessageType(msgType);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인 서버 연결여부 판다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsConnected()
{
	return m_pMainFieldWinSocket->IsConnected();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인 서버 와의 연결을 위한 함수
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CFieldWinSocketManager::OnAsyncEvent(LONG lParam)
{	
	return m_pMainFieldWinSocket->OnAsyncEvent(lParam);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버와의 연결을 위한 함수
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CFieldWinSocketManager::OnArenaAsyncEvent(LONG lParam)
{	
	if(NULL == m_pArenaFieldWinSocket)
	{
		return 0;
	}
	return m_pArenaFieldWinSocket->OnAsyncEvent(lParam);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버와연결 시도
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::ConnectArena(LPCSTR strPeerIP, int nPort, BOOL* bAlReadyConnect)
{	
	if(NULL == m_pArenaFieldWinSocket)
	{
		m_pArenaFieldWinSocket = new  CFieldWinSocket(m_hMainWnd, WM_ASYNC_ARENA_EVENT, WM_PACKET_ARENA_NOTIFY);
	}
	(*bAlReadyConnect) = FALSE;
	if(m_pArenaFieldWinSocket->IsConnected())
	{
		(*bAlReadyConnect) = TRUE;
		return TRUE;
	}
	m_bRequestArenaSocketClose = FALSE;
	BOOL bConnect = m_pArenaFieldWinSocket->Connect(strPeerIP, nPort);

	return bConnect;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버와 접속끊기
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::CloseSocketArena()
{
	DBGOUT("BOOL CFieldWinSocketManager::CloseSocketArena() \n");

	m_bRequestArenaSocketClose = TRUE;
	BOOL bRtn = TRUE;
	if(IsFCConnectedArenaServer())
	{
		bRtn = m_pArenaFieldWinSocket->CloseSocket();
	}	
	return bRtn;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		소켓 타입을 얻어와 어느 소켓인지 알려주는 함수
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldWinSocket* CFieldWinSocketManager::GetFieldWinSocket(UINT nSocketNotifyType)
{
	if(WM_PACKET_ARENA_NOTIFY == nSocketNotifyType)
	{
		return GetArenaFieldWinSocket();
	}
	return m_pMainFieldWinSocket;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 소켓을 리턴한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldWinSocket* CFieldWinSocketManager::GetArenaFieldWinSocket()
{
	return m_pArenaFieldWinSocket;	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 소켓이 연결되어있는지 여부 판단
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsFCConnectedArenaServer()
{
	if(NULL == m_pArenaFieldWinSocket)
	{
		return FALSE;
	}
	return m_pArenaFieldWinSocket->IsConnected();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 상태이냐 판단
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsArenaLoadSuccess()
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
/// \brief		아레나 상태이냐 판단
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsRequestArenaSocketClose()
{
	return m_bRequestArenaSocketClose;
}