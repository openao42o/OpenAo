// ArenaManager.cpp: implementation of the CArenaManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ArenaManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArenaManager::CArenaManager()
{	
	m_nArenaState = ARENA_STATE_NONE;

	m_pInfoArenaServer = new MSG_FC_CONNECT_ARENASERVER_INFO;
	memset(m_pInfoArenaServer, 0x00, sizeof(MSG_FC_CONNECT_ARENASERVER_INFO));

	m_pAFSCharacter = new CHARACTER;
	memset(m_pAFSCharacter, 0x00, sizeof(CHARACTER));
	
	m_pMFSCharacter = new CHARACTER;
	memset(m_pMFSCharacter, 0x00, sizeof(CHARACTER));

	m_bGmModeConnect = FALSE;

	m_vecArenaServerInfo.clear();
	m_bForceCloseArenaSocket = FALSE;

	m_nInfPoint = 0;

}

CArenaManager::~CArenaManager()
{
	m_vecArenaServerInfo.clear();

	util::del(m_pInfoArenaServer);
	util::del(m_pAFSCharacter);
	util::del(m_pMFSCharacter);
	

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2007-11-20 by bhsohn 맵로딩 하는 방식 변경
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetArenaState(int nState)
{
	DBGOUT("CArenaManager::SetArenaState [%d] \n", nState);
	m_nArenaState = nState;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		현재 아레나 정보를 가져온다. 
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CArenaManager::GetArenaState()
{
	return m_nArenaState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 접속 정보를 얻어온다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MSG_FC_CONNECT_ARENASERVER_INFO* CArenaManager::GetArenaServerInfo()
{
	return m_pInfoArenaServer;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 접속 정보를 업데이트 한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetArenaServerInfo(MSG_FC_CONNECT_ARENASERVER_INFO* pMsg)
{
	memcpy(m_pInfoArenaServer, pMsg, sizeof(MSG_FC_CONNECT_ARENASERVER_INFO));	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버 정보를 업데이트 한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetArenaCharacterInfo(CHARACTER	 *pAFSCharacter)
{
	memcpy(m_pAFSCharacter, pAFSCharacter, sizeof(CHARACTER));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		아레나 서버 정보를 얻어온다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CHARACTER*	CArenaManager::GetArenaCharacterInfo()
{
	return m_pAFSCharacter;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인서버 정보를 업데이트 한다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetMFSMyShuttleInfo(CHARACTER *pMyShuttleInfo)
{
	memcpy(m_pMFSCharacter, pMyShuttleInfo, sizeof(CHARACTER));	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메인서버 정보를 가져온다.
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CHARACTER* CArenaManager::GetMFSMyShuttleInfo()
{
	return m_pMFSCharacter;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumApplication::ConevertArenaRenderUserName()
/// \brief		실제로 아레나맵에서 랜더링할 맵 이름 결정
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::ConevertArenaRenderUserName(int nArenaState, char* pSrcDst)
{
	if((ARENA_STATE_ARENA_GAMING != nArenaState)
		&&(ARENA_STATE_ARENA_LOAD_GAME_INFO != nArenaState))
	{
		return;
	}
	char chFindChar[8];
	char chTmpBuff[128];
	//memset(chFindChar, 0x00, 8);
	//sprintf(chFindChar, "\\");	
	//memset(chTmpBuff, 0x00, 128);
	char* pFindChChatting = strstr(pSrcDst, chFindChar);

	if(pFindChChatting)
	{
//		BOOL bFindServerName = FALSE;
//		char chArenaServerId[SIZE_MAX_SERVER_NAME];		
//		util::strncpy(chArenaServerId, pFindChChatting, SIZE_MAX_SERVER_NAME);
//		bFindServerName = GetArenaId_To_ArenaServerName(chArenaServerId, chArenaServerId);
		
		int nPos = pFindChChatting - pSrcDst;
		strncpy(chTmpBuff, pSrcDst, nPos);
		util::strncpy(pSrcDst, chTmpBuff, nPos+1);
		
//		if(bFindServerName)
//		{
//			// 서버 목록에 이름이 있다.
//			strcat(pSrcDst, "_");
//			strcat(pSrcDst, chArenaServerId);
//		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetGmModeConnect(BOOL bGmModeConnect)
/// \brief		GM이 아레나 서버에 접속시도냐?
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetGmModeConnect(BOOL bGmModeConnect)
{
	m_bGmModeConnect = bGmModeConnect;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::IsGmModeConnect()
/// \brief		GM이 아레나 서버에 접속시도냐?
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::IsGmModeConnect()
{
	return m_bGmModeConnect ;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg)
/// \brief		아레나 서버 목록을 알려준다.
/// \author		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg)
{
	m_vecArenaServerInfo.clear();
	int nCnt = 0;
	for(nCnt = 0;nCnt < SIZE_MAX_SERVER_GROUP_COUNT; nCnt++)
	{
		m_vecArenaServerInfo.push_back(pMsg->ServerGroupInfo[nCnt]);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void GetArenaId_To_ArenaServerName(char* pArenaId, char* pDstServerName)
/// \brief		아레나 서버 목록에서 서버 이름을 얻어온다.
/// \author		// 2008-02-25 by bhsohn 아레나 유저 정보 서버도 표시하게 변경
/// \date		2008-02-25 ~ 2008-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::GetArenaId_To_ArenaServerName(char* pArenaId, char* pDstServerName)
{
	vector<SSERVER_GROUP_FOR_CLIENT>::iterator it = m_vecArenaServerInfo.begin();
	while(it != m_vecArenaServerInfo.end())
	{
		SSERVER_GROUP_FOR_CLIENT sMsg = (*it);
		if(0 == stricmp(sMsg.MFS_ServerIDName, pArenaId))
		{
			util::strncpy(pDstServerName, sMsg.MFS_Name, SIZE_MAX_SERVER_NAME);
			return TRUE;
		}
		it++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaManager::SetGmModeConnect(BOOL bGmModeConnect)
/// \brief		GM이 아레나 서버에 접속시도냐?
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetForceCloseArenaSocket(BOOL bForceCloseArenaSocket)
{	
	m_bForceCloseArenaSocket = bForceCloseArenaSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CArenaManager::IsGmModeConnect()
/// \brief		GM이 아레나 서버에 접속시도냐?
/// \author		// 2007-11-22 by bhsohn 아레나 통합서버
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CArenaManager::IsForceCloseArenaSocket()
{
	return m_bForceCloseArenaSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-15 by bhsohn 아레나 워포인트 찍는 시스템 변경
/// \date		2008-04-15 ~ 2008-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaManager::SetInflPoint(int nInflPoint)
{
	m_nInfPoint = nInflPoint;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-15 by bhsohn 아레나 워포인트 찍는 시스템 변경
/// \date		2008-04-15 ~ 2008-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int  CArenaManager::GetInflPoint()
{
	return m_nInfPoint;
}