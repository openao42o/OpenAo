#include "stdafx.h"
#include "MapOwnership.h"
#include "FieldIOCP.h"

///////////////////////////////////////////////////////////////////////////////
// MAP_OWNERSHIP
///////////////////////////////////////////////////////////////////////////////
MAP_OWNERSHIP::MAP_OWNERSHIP()
{
	Reset();
}

void MAP_OWNERSHIP::Reset()
{
	util::zero(this, sizeof(MAP_OWNERSHIP));
}

///////////////////////////////////////////////////////////////////////////////
// CMapOwnershipManager
///////////////////////////////////////////////////////////////////////////////
CMapOwnershipManager::CMapOwnershipManager()
{
	m_pFieldIOCP3 = NULL;
}

CMapOwnershipManager::~CMapOwnershipManager()
{
	CleanMapOwnershipManager();
}

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//BOOL CMapOwnershipManager::InitMapOwnershipManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD)
BOOL CMapOwnershipManager::InitMapOwnershipManager(CFieldIOCP *i_pFieldIOCP, char *i_szServIP, int i_nServPort, char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd)
{
	BOOL bRet = TRUE;

	m_pFieldIOCP3 = i_pFieldIOCP;

	// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
	//bRet = m_ODBCStmt5.Init(i_szDSN, i_szUID, i_szPASSWORD);
	bRet = m_ODBCStmt5.Init(i_szServIP, i_nServPort, i_szDatabaseName, i_szUID, i_szPassword, i_hWnd);

	return bRet;
}

void CMapOwnershipManager::CleanMapOwnershipManager()
{
	m_ODBCStmt5.Clean();
}

//BOOL CMapOwnershipManager::LoadMapOwnership()
//{
//	m_ODBCStmt5.Lock();
//
//	MAP_OWNERSHIP	retMapOwnership;
//	BOOL			bRet;
//
//	const char *szQuery = "SELECT * FROM td_MapOwnership";
//
//	bRet = m_ODBCStmt5.ExecuteQuery(szQuery);
//	if (!bRet)
//	{
//		m_ODBCStmt5.FreeStatementAndUnlock();
//		return FALSE;
//	}
//
//	SQLBindCol(m_ODBCStmt5.m_hstmt, 1, SQL_C_USHORT, &retMapOwnership.MapIndex, 0, NULL);
//	SQLBindCol(m_ODBCStmt5.m_hstmt, 2, SQL_C_ULONG, &retMapOwnership.OwnerGuildUniqueNumber, 0, NULL);
//	SQLBindCol(m_ODBCStmt5.m_hstmt, 3, SQL_C_ULONG, &retMapOwnership.ChallengerGuildUniqueNumber, 0, NULL);
//
//	util::zero(&retMapOwnership, sizeof(MAP_OWNERSHIP));
//	while (SQLFetch(m_ODBCStmt5.m_hstmt) != SQL_NO_DATA)
//	{
//		m_mapMapOwership.insertLock(retMapOwnership.MapIndex, retMapOwnership);
//		util::zero(&retMapOwnership, sizeof(MAP_OWNERSHIP));
//	}
//
//	m_ODBCStmt5.FreeStatementAndUnlock();
//
//	return TRUE;
//}

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 사용하지 않는 부분임
//BOOL CMapOwnershipManager::GetMapOwnership(MapIndex_t i_nMapIndex, MAP_OWNERSHIP *o_pMapOwnership)
//{
// 	m_ODBCStmt5.Lock();
// 
// 	BOOL bRet;
// 	char szQuery[SIZE_MAX_SQL_QUERY_STRING];
// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// // 	sprintf(szQuery, "SELECT o.MapIndex, g.GuildUniqueNumber, g.GuildName\
// // 						FROM td_MapOwnership o, td_Guild g WITH (NOLOCK)\
// // 						WHERE o.MapIndex = %d AND g.GuildUniqueNumber = o.OwnerGuildUniqueNumber", i_nMapIndex);
// 	sprintf(szQuery, QUERY_080702_0085, i_nMapIndex);
// 
// 	bRet = m_ODBCStmt5.ExecuteQuery(szQuery);
// 	if (!bRet)
// 	{
// 		m_ODBCStmt5.FreeStatementAndUnlock();
// 		return FALSE;
// 	}
// 
// 	SQLBindCol(m_ODBCStmt5.m_hstmt, 1, SQL_C_USHORT, &o_pMapOwnership->MapIndex, 0, NULL);
// 	SQLBindCol(m_ODBCStmt5.m_hstmt, 2, SQL_C_ULONG, &o_pMapOwnership->OwnerGuildUniqueNumber, 0, NULL);
// 	SQLINTEGER pcb = SQL_NTS;
// 	SQLBindCol(m_ODBCStmt5.m_hstmt, 3, SQL_C_CHAR, o_pMapOwnership->OwnerGuildName, SIZE_MAX_GUILD_NAME, &pcb);
// 
// 	util::zero(o_pMapOwnership, sizeof(MAP_OWNERSHIP));
// 	if (SQLFetch(m_ODBCStmt5.m_hstmt) != SQL_NO_DATA)
// 	{
// 		bRet = TRUE;
// 	}
// 	else
// 	{
// 		bRet = FALSE;
// 	}
// 
// 	m_ODBCStmt5.FreeStatementAndUnlock();
// 
// 	return bRet;
//}
//
// BOOL CMapOwnershipManager::GetGuildWarChallengers(UID32_t i_nGuildUniqueNumber, vector<string> *o_pChallengers)
// {
// 	o_pChallengers->clear();
// 
// 	m_ODBCStmt5.Lock();
// 
// 	BOOL bRet;
// 	char retGuildName[SIZE_MAX_GUILD_NAME];
// 	char szQuery[SIZE_MAX_SQL_QUERY_STRING];
// // 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// // 	sprintf(szQuery, "SELECT g.GuildName\
// // 						FROM td_GuildWar gw, td_Guild g WITH (NOLOCK)\
// // 						WHERE g.GuildUniqueNumber = gw.DefenderGuildUniqueNumber AND g.GuildUniqueNumber = %d", i_nGuildUniqueNumber);
// 	sprintf(szQuery, QUERY_080702_0086, i_nGuildUniqueNumber);
// 
// 	bRet = m_ODBCStmt5.ExecuteQuery(szQuery);
// 	if (!bRet)
// 	{
// 		m_ODBCStmt5.FreeStatementAndUnlock();
// 		return FALSE;
// 	}
// 
// 	SQLINTEGER pcb = SQL_NTS;
// 	SQLBindCol(m_ODBCStmt5.m_hstmt, 1, SQL_C_CHAR, retGuildName, SIZE_MAX_GUILD_NAME, &pcb);
// 
// 	while (SQLFetch(m_ODBCStmt5.m_hstmt) != SQL_NO_DATA)
// 	{
// 		o_pChallengers->push_back(retGuildName);
// 	}
// 
// 	m_ODBCStmt5.FreeStatementAndUnlock();
// 
// 	return TRUE;
// }

// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 사용하지 않는 함수, 주석 처리 함.
// /*
//  * 리턴 값이 0 이하이면 신청 실패
//  * 1 이상이면 신청 성공 및 신청 순위
//  *
//  */
// INT CMapOwnershipManager::InsertRequestForGuildWar(MapIndex_t i_nMapIndex, char *i_szDefenderGuildName, char *i_szChallengerGuildName)
// {
// 	m_ODBCStmt5.Lock();
// 
// 	SQLINTEGER	cb1, cb2, cb3, cb4;
// 	INT			nRetOrder = 0;		// 여단전 신청 순위
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_InsertRequestForGuildWar
// 		@i_MapIndex				SMALLINT,
// 		@i_DefenderGuildName	VARCHAR(30),
// 		@i_ChallengerGuildName	VARCHAR(30)
// 	AS
// 		DECLARE @DefenderGuildUNUM INT
// 		DECLARE @ChallengerGuildUNUM INT
// 
// 		SET @DefenderGuildUNUM = (SELECT TOP 1 GuildUniqueNumber FROM td_Guild WHERE GuildName like @i_DefenderGuildName)
// 		SET @ChallengerGuildUNUM = (SELECT TOP 1 GuildUniqueNumber FROM td_Guild WHERE GuildName like @i_ChallengerGuildName)
// 
// 		IF (@DefenderGuildUNUM IS NULL OR @ChallengerGuildUNUM IS NULL)
// 			RETURN 0
// 
// 		INSERT INTO GuildWar(MapIndex, DefenderGuildUniqueNumber, ChallengerGuildUniqueNumber)
// 		VALUES (@i_MapIndex, @DefenderGuildUNUM, @ChallengerGuildUNUM)
// 		
// 		IF (@@ERROR <> 0)
// 			RETURN 0
// 
// 		RETURN (SELECT count(*) FROM td_GuildWar
// 				WHERE MapIndex = @i_MapIndex AND DefenderGuildUniqueNumber = @DefenderGuildUNUM)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(m_ODBCStmt5.GetSTMTHandle(), 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRetOrder, 0, &cb1);
// 	SQLBindParameter(m_ODBCStmt5.GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_nMapIndex, 0, &cb2);
// 	cb2 = SQL_NTS;
// 	SQLBindParameter(m_ODBCStmt5.GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, i_szDefenderGuildName, 0, &cb3);
// 	cb3 = SQL_NTS;
// 	SQLBindParameter(m_ODBCStmt5.GetSTMTHandle(), 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, i_szChallengerGuildName, 0, &cb4);
// 
// 	// 2008-07-08 by cmkwon, MySQL 지원으로 수정(프로시저에서 리턴처리 불가로 수정) - 
// 	//if (!m_ODBCStmt5.ExecuteQuery("{? = call atum_InsertRequestForGuildWar(?, ?, ?)}"))
// 	if (!m_ODBCStmt5.ExecuteQuery("{call atum_InsertRequestForGuildWar(?, ?, ?, ?)}"))
// 	{
// 		m_ODBCStmt5.FreeStatementAndUnlock();
// 
// 		return 0;
// 	}
// 
// 	// return 값 할당
// 	m_ODBCStmt5.ClearAllResultSets();
// 
// 	m_ODBCStmt5.FreeStatementAndUnlock();
// 
// 	return nRetOrder;
// }

BOOL CMapOwnershipManager::CancelRequestForGuildWar(char *i_szDefenderGuildName, char *i_szChallengerGuildName)
{
	return TRUE;
}
