#include "stdafx.h"
#include "GuildWar.h"
#include "FieldIOCP.h"
#include "FieldTickManager.h"

///////////////////////////////////////////////////////////////////////////////
// SGuildForGuildWar
///////////////////////////////////////////////////////////////////////////////
BOOL SGuildForGuildWar::IsGuildMember(UID32_t i_uidCharacterUID)
{
	int nMemberCounts = vectorUID32CharacterUID.size();
	for(int i=0; i < nMemberCounts; i++)
	{
		if(i_uidCharacterUID == vectorUID32CharacterUID[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// CGuildWar
///////////////////////////////////////////////////////////////////////////////
CGuildWar::CGuildWar(CGuildWarMananger *i_pGuildWarManager)
{
	m_pGuildWarManager = i_pGuildWarManager;
}

MAP_CHANNEL_INDEX CGuildWar::GetMapChannelIndex(void)
{
	return m_MapChannelIndex;
}

SGuildForGuildWar *CGuildWar::GetPeerGuildForGuildWar(UID32_t i_GuildUID)
{
	if(m_ChallengerGuild.uidGuildUID == i_GuildUID)
	{
		return &m_DefenderGuild;
	}
	else if(m_DefenderGuild.uidGuildUID == i_GuildUID)
	{
		return &m_ChallengerGuild;
	}

	return NULL;
}


SGuildForGuildWar *CGuildWar::GetDefenderGuildPtr(void)
{
	return &m_DefenderGuild;
}

SGuildForGuildWar *CGuildWar::GetChallengerGuildPtr(void)
{
	return &m_ChallengerGuild;
}


void CGuildWar::PrintGuildWar(void)
{
	DBGOUT("============================\r\n");
	DBGOUT("=== Defender Guild \r\n");
	// 수성 길드 처리
	vectorUID32::iterator itr = m_DefenderGuild.vectorUID32CharacterUID.begin();
	for(; itr != m_DefenderGuild.vectorUID32CharacterUID.end(); itr++)
	{
		CFieldIOCPSocket *pSocket = m_pGuildWarManager->m_pFieldIOCP5->m_mapCharacterUniqueNumber.findLock(*itr);
		if (pSocket != NULL && pSocket->IsUsing())
		{
			DBGOUT("G1: %s %s at %s\r\n",
				pSocket->m_character.CharacterName,
				pSocket->m_character.GuildName,
				GetMapString(pSocket->m_character.MapChannelIndex, string()));
		}
	}
	
	DBGOUT("  vs\r\n");
	DBGOUT("=== Challenger Guild \r\n");
	// 도전 길드 처리
	itr = m_ChallengerGuild.vectorUID32CharacterUID.begin();
	for(; itr != m_ChallengerGuild.vectorUID32CharacterUID.end(); itr++)
	{
		CFieldIOCPSocket *pSocket = m_pGuildWarManager->m_pFieldIOCP5->m_mapCharacterUniqueNumber.findLock(*itr);
		if (pSocket != NULL && pSocket->IsUsing())
		{
			DBGOUT("G2: %s %s at %s\r\n",
				pSocket->m_character.CharacterName,
				pSocket->m_character.GuildName,
				GetMapString(pSocket->m_character.MapChannelIndex, string()));
		}		
	}
	DBGOUT("============================\r\n");
}


///////////////////////////////////////////////////////////////////////////////
// CGuildWarMananger
///////////////////////////////////////////////////////////////////////////////
CGuildWarMananger::CGuildWarMananger()
{
	m_pFieldIOCP5 = NULL;
}

CGuildWarMananger::~CGuildWarMananger()
{
	CleanGuildWarMananger();
}

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//BOOL CGuildWarMananger::InitGuildWarMananger(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD)
BOOL CGuildWarMananger::InitGuildWarMananger(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd)
{
	BOOL bRet = TRUE;

	m_pFieldIOCP5 = i_pFieldIOCP;

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//bRet = m_ODBCStmt1.Init(i_szDSN, i_szUID, i_szPASSWORD);
	bRet = m_ODBCStmt1.Init(i_szServIP, i_nServPort, i_szDatabaseName, i_szUID, i_szPassword, i_hWnd);

	return bRet;
}

void CGuildWarMananger::CleanGuildWarMananger()
{
	m_ODBCStmt1.Clean();
}

BOOL CGuildWarMananger::CheckGuildCommander(UID32_t i_CharacterUniqueNumber, UID32_t *o_pGuildUID/*=NULL*/)
{
	BOOL	bRet;
//	char	szQuery[SIZE_MAX_SQL_QUERY_STRING];
	UID32_t	retGuildUID;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szQuery,
// 			"SELECT g.GuildUniqueNumber\
// 			FROM td_Guild g WITH (NOLOCK), td_Character c WITH (NOLOCK)\
// 			WHERE c.UniqueNumber = %d AND g.GuildUniqueNumber = c.GuildUniqueNumber\
// 			AND g.GuildCommanderUniqueNumber = c.UniqueNumber", i_CharacterUniqueNumber);

//	DBGOUT(szQuery);

	m_ODBCStmt1.Lock();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery, QUERY_080702_0077, i_CharacterUniqueNumber);
//	if (!m_ODBCStmt1.ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_CharacterUniqueNumber), 0,			&arrCB2[1]);
	bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0077));
	if(FALSE == bRet)
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return FALSE;
	}

	SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_ULONG, &retGuildUID, 0, NULL);
	if (SQLFetch(m_ODBCStmt1.GetSTMTHandle()) == SQL_NO_DATA)
	{
		bRet = FALSE;
	}
	else
	{
		if(o_pGuildUID)
		{
			*o_pGuildUID = retGuildUID;
		}
		bRet = TRUE;
	}
	m_ODBCStmt1.FreeStatementAndUnlock();

	return bRet;
}

BOOL CGuildWarMananger::CheckGuildCommanderAndGetMemberCapacity(UID32_t i_CharacterUniqueNumber, int *o_pGuildMemberCapacity)
{
	BOOL	bRet;
//	char	szQuery[SIZE_MAX_SQL_QUERY_STRING];
	int		nCapacity = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szQuery,
// 			"SELECT g.GuildMemberCapacity\
// 			FROM td_Guild g, td_Character c WITH (NOLOCK)\
// 			WHERE c.UniqueNumber = %d AND g.GuildUniqueNumber = c.GuildUniqueNumber\
// 			AND g.GuildCommanderUniqueNumber = c.UniqueNumber", i_CharacterUniqueNumber);

	m_ODBCStmt1.Lock();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery, QUERY_080702_0078, i_CharacterUniqueNumber);
//	if (FALSE == m_ODBCStmt1.ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_CharacterUniqueNumber), 0,			&arrCB2[1]);
	bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0078));
	if(FALSE == bRet)
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return FALSE;
	}

	SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_ULONG, &nCapacity, 0, NULL);
	if( SQL_NO_DATA == (SQLFetch(m_ODBCStmt1.GetSTMTHandle())) )
	{
		bRet						= FALSE;
	}
	else
	{
		*o_pGuildMemberCapacity		= nCapacity;
		bRet						= TRUE;
	}
	m_ODBCStmt1.FreeStatementAndUnlock();
	return bRet;
}

BOOL CGuildWarMananger::MakeGuildForGuildWar(SGuildForGuildWar *o_pGuildForGuildWar, UID32_t i_uidGuildUID)
{
	m_ODBCStmt1.Lock();
	{
//		char	szQuery[SIZE_MAX_SQL_QUERY_STRING];

		o_pGuildForGuildWar->uidGuildUID = i_uidGuildUID;
		o_pGuildForGuildWar->vectorUID32CharacterUID.reserve(50);

		///////////////////////////////////////////////////////////////////////////////
		// 길드 확인
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//		sprintf(szQuery, "SELECT * FROM td_Guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d", i_uidGuildUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		sprintf(szQuery, QUERY_080702_0079, i_uidGuildUID);
//		if (!m_ODBCStmt1.ExecuteQuery(szQuery))
		SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
		SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_uidGuildUID), 0,			&arrCB2[0]);
		BOOL bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0079));
		if(FALSE == bRet)
		{
			m_ODBCStmt1.FreeStatementAndUnlock();
			return FALSE;
		}
		if (SQLFetch(m_ODBCStmt1.GetSTMTHandle()) == SQL_NO_DATA)
		{
			m_ODBCStmt1.FreeStatementAndUnlock();
			return FALSE;
		}
		m_ODBCStmt1.FreeStatement();
		// 길드 확인_end
		///////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////
		// 모든 여단원 CharacterUID 삽입
		UID32_t MemberUniqueNumber = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//		sprintf(szQuery, "SELECT CharacterUniqueNumber FROM td_GuildMember WITH (NOLOCK) WHERE GuildUniqueNumber = %d", i_uidGuildUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		sprintf(szQuery, QUERY_080702_0080, i_uidGuildUID);
//		if (!m_ODBCStmt1.ExecuteQuery(szQuery))
		hstmt = m_ODBCStmt1.GetSTMTHandle();
		arrCB2[1] = SQL_NTS;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_uidGuildUID), 0,			&arrCB2[1]);
		bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0080));
		if(FALSE == bRet)
		{
			m_ODBCStmt1.FreeStatementAndUnlock();
			return FALSE;
		}

		SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_ULONG, &MemberUniqueNumber, 0, NULL);
		while (SQLFetch(m_ODBCStmt1.GetSTMTHandle()) != SQL_NO_DATA)
		{
			o_pGuildForGuildWar->vectorUID32CharacterUID.push_back(MemberUniqueNumber);
		}
		vectorUID32(o_pGuildForGuildWar->vectorUID32CharacterUID).swap(o_pGuildForGuildWar->vectorUID32CharacterUID);
		// 모든 여단원 CharacterUID 삽입_end
		///////////////////////////////////////////////////////////////////////////////
	}
	m_ODBCStmt1.FreeStatementAndUnlock();

	return TRUE;
}

CGuildWar* CGuildWarMananger::FindGuildWarPtr(UID32_t i_nGuildUID)
{
	return m_mtmapGuildWar.findLock(i_nGuildUID);
}

CGuildWar* CGuildWarMananger::PopGuildWarPtr(UID32_t i_nGuildUID)
{
	return m_mtmapGuildWar.popLock(i_nGuildUID);
}

BOOL CGuildWarMananger::StartGuildWar(UID32_t i_uidDefenderGuildUID, UID32_t i_uidChallengerGuildUID, MAP_CHANNEL_INDEX *i_pMapChannelIdx)
{
	m_mtmapGuildWar.lock();	
	{
		///////////////////////////////////////////////////////////////////////////////
		// 길드 체크
		CGuildWar *pGuildWar = m_mtmapGuildWar.findNoLock(i_uidChallengerGuildUID);
		if(pGuildWar)
		{
			m_mtmapGuildWar.unlock();
			return FALSE;
		}		
		pGuildWar = m_mtmapGuildWar.findNoLock(i_uidDefenderGuildUID);
		if(pGuildWar)
		{
			m_mtmapGuildWar.unlock();
			return FALSE;
		}
		// 길드 체크_end
		///////////////////////////////////////////////////////////////////////////////

		pGuildWar = new CGuildWar(this);
		pGuildWar->m_MapChannelIndex = *i_pMapChannelIdx;
		if(FALSE == MakeGuildForGuildWar(pGuildWar->GetDefenderGuildPtr(), i_uidDefenderGuildUID))
		{
			m_mtmapGuildWar.unlock();
			return FALSE;
		}
		if(FALSE == MakeGuildForGuildWar(pGuildWar->GetChallengerGuildPtr(), i_uidChallengerGuildUID))
		{
			m_mtmapGuildWar.unlock();
			return FALSE;
		}		

		///////////////////////////////////////////////////////////////////////////////
		// 두개 길드의 UID를 맵에 추가한다
		m_mtmapGuildWar.insertNoLock(i_uidChallengerGuildUID, pGuildWar);
		m_mtmapGuildWar.insertNoLock(i_uidDefenderGuildUID, pGuildWar);
	}
	m_mtmapGuildWar.unlock();

	INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_NOTIFY_START_WAR, T_FI_GUILD_NOTIFY_START_WAR, pMsgStartWar, pMsgStartWarBuf);
	pMsgStartWar->DefenderGuildUID		= i_uidDefenderGuildUID;
	pMsgStartWar->ChallengerGuildUID	= i_uidChallengerGuildUID;
	pMsgStartWar->WarMapChannel			= *i_pMapChannelIdx;
	m_pFieldIOCP5->m_pIMWinSocket->Write(pMsgStartWarBuf, MSG_SIZE(MSG_FI_GUILD_NOTIFY_START_WAR));
	return TRUE;
}


BOOL CGuildWarMananger::EndGuildWar(UID32_t i_nGuildUID, BOOL i_bSurrender/*=FALSE*/)
{
	this->LockGuildWarManager();
	{
		CGuildWar *pGuildWar = PopGuildWarPtr(i_nGuildUID);
		if (pGuildWar == NULL)
		{
			this->UnlockGuildWarManager();
			return FALSE;
		}

		SGuildForGuildWar *pGuild = pGuildWar->GetPeerGuildForGuildWar(i_nGuildUID);
		if(pGuild)
		{
			PopGuildWarPtr(pGuild->uidGuildUID);
		}

		util::del(pGuildWar);
	}
	this->UnlockGuildWarManager();

	return TRUE;
}

void CGuildWarMananger::PrintGuildWar(UID32_t i_nGuildUID)
{
	this->LockGuildWarManager();
	{
		CGuildWar *pGuildWar = FindGuildWarPtr(i_nGuildUID);
		if (pGuildWar == NULL)
		{
			this->UnlockGuildWarManager();
			return;
		}
		
		pGuildWar->PrintGuildWar();		
	}
	this->UnlockGuildWarManager();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CGuildWarMananger::GetGuildCommanderUID(UID32_t i_nGuildUID)
/// \brief		여단 UID로 여단장 UID 얻어오기
/// \author		dhjin
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CGuildWarMananger::GetGuildCommanderUID(UID32_t i_nGuildUID)
{
//	char	szQuery[SIZE_MAX_SQL_QUERY_STRING];
	int		nCapacity = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szQuery,
// 			"SELECT GuildCommanderUniqueNumber\
// 			FROM td_Guild WITH (NOLOCK)\
// 			WHERE GuildUniqueNumber = %d", i_nGuildUID);

	m_ODBCStmt1.Lock();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery,QUERY_080702_0081, i_nGuildUID);
//	if (FALSE == m_ODBCStmt1.ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_nGuildUID), 0,			&arrCB2[1]);
	BOOL bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0081));
	if(FALSE == bRet)
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return FALSE;
	}

	SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_ULONG, &nCapacity, 0, NULL);
	if( SQL_NO_DATA == (SQLFetch(m_ODBCStmt1.GetSTMTHandle())) )
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return 0;
	}
	else
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return nCapacity;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			UID32_t CGuildWarMananger::GetLeaderGuildUID(UID32_t i_LeaderUniqueNumber)
/// \brief		지도자 UID로 지도자 여단 UID가져오기
/// \author		dhjin
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
UID32_t CGuildWarMananger::GetLeaderGuildUID(UID32_t i_LeaderUniqueNumber)
{
//	char	szQuery[SIZE_MAX_SQL_QUERY_STRING];
	int		nCapacity = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szQuery,
// 			"SELECT GuildUniqueNumber FROM td_character	\
// 				WHERE UniqueNumber = %u", i_LeaderUniqueNumber);

	m_ODBCStmt1.Lock();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery, QUERY_080702_0082, i_LeaderUniqueNumber);
//	if (FALSE == m_ODBCStmt1.ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_LeaderUniqueNumber), 0,			&arrCB2[1]);
	BOOL bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0082));
	if(FALSE == bRet)
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return FALSE;
	}

	SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_ULONG, &nCapacity, 0, NULL);
	if( SQL_NO_DATA == (SQLFetch(m_ODBCStmt1.GetSTMTHandle())) )
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return 0;
	}

	m_ODBCStmt1.FreeStatementAndUnlock();
	return nCapacity;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CGuildWarMananger::CheckGuildStateDisMemberReady(UID32_t i_nGuildUID)
/// \brief		여단 해체 대기 중인지 체크
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CGuildWarMananger::CheckGuildStateDisMemberReady(UID32_t i_nGuildUID)
{
//	char	szQuery[SIZE_MAX_SQL_QUERY_STRING];
	int		nResult = 0;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szQuery,
// 			"SELECT GuildState\
// 			FROM td_Guild WITH (NOLOCK)\
// 			WHERE GuildUniqueNumber = %d", i_nGuildUID);

	m_ODBCStmt1.Lock();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery, QUERY_080702_0083, i_nGuildUID);
//	if (FALSE == m_ODBCStmt1.ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_nGuildUID), 0,			&arrCB2[1]);
	BOOL bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0083));
	if(FALSE == bRet)
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return FALSE;
	}

	SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_ULONG, &nResult, 0, NULL);
	if( SQL_NO_DATA == (SQLFetch(m_ODBCStmt1.GetSTMTHandle())) )
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return FALSE;
	}

	m_ODBCStmt1.FreeStatementAndUnlock();

	if(GUILD_STATE_DISMEMBER_READY == nResult)
	{// 2007-11-09 by dhjin, 여단 해체 대기 중이면 
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CGuildWarMananger::GetGuildCommanderName(UID32_t i_nGuildCommanderUID, char * o_pGuildCommanderName)
/// \brief		여단 UID로 여단장 이름 얻어오기
/// \author		dhjin
/// \date		2007-12-03 ~ 2007-12-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CGuildWarMananger::GetGuildCommanderName(UID32_t i_nGuildCommanderUID, char * o_pGuildCommanderName)
{
//	char	szQuery[SIZE_MAX_SQL_QUERY_STRING];

// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	sprintf(szQuery,
// 			"SELECT CharacterName FROM td_character WITH (NOLOCK) \
// 				WHERE UniqueNumber = (SELECT GuildCommanderUniqueNumber	\
// 						FROM td_Guild WITH (NOLOCK)	\
// 						WHERE GuildUniqueNumber = %d)", i_nGuildCommanderUID);

	m_ODBCStmt1.Lock();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(szQuery, QUERY_080702_0084, i_nGuildCommanderUID);
//	if (FALSE == m_ODBCStmt1.ExecuteQuery(szQuery))
	SQLHSTMT hstmt = m_ODBCStmt1.GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_nGuildCommanderUID), 0,			&arrCB2[1]);
	BOOL bRet = m_ODBCStmt1.ExecuteQuery((char*)(PROCEDURE_080827_0084));
	if(FALSE == bRet)
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return;
	}

	SQLBindCol(m_ODBCStmt1.GetSTMTHandle(), 1, SQL_C_CHAR, o_pGuildCommanderName, SIZE_MAX_CHARACTER_NAME, NULL);
	if( SQL_NO_DATA == (SQLFetch(m_ODBCStmt1.GetSTMTHandle())) )
	{
		m_ODBCStmt1.FreeStatementAndUnlock();
		return;
	}

	m_ODBCStmt1.FreeStatementAndUnlock();
}