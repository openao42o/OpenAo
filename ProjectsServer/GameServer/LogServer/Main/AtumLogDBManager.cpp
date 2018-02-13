// AtumLogDBManager.cpp: implementation of the CAtumLogDBManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
// 2008-11-21 by cmkwon, LogServer를 MFC에서 Win32 프로젝트로 변경 - 
//#include "LogServer.h"
#include "AtumLogDBManager.h"
#include "LogGlobal.h"
#include "AtumDBLogWriter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtumLogDBManager::CAtumLogDBManager(CLogIOCP *i_pLogIOCP)
:CAtumDBManager(COUNT_LOG_SERVER_DBWORKER)
{
	m_pLogIOCP2		= i_pLogIOCP;
}

CAtumLogDBManager::~CAtumLogDBManager()
{

}




///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLogDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-19 ~ 2006-05-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog 보완
{
	switch(dbquery.enumQueryType)
	{
	case QT_FL_LOG_LOGIN:					QP_FL_LOG_LOGIN(dbquery, hstmt);							break;
	case QT_FL_LOG_LOGOUT:					QP_FL_LOG_LOGOUT(dbquery, hstmt);							break;
	case QT_FL_LOG_LEVEL:					QP_FL_LOG_LEVEL(dbquery, hstmt);							break;
	case QT_FL_LOG_EXP:						QP_FL_LOG_EXP(dbquery, hstmt);								break;
	case QT_FL_LOG_STAT:					QP_FL_LOG_STAT(dbquery, hstmt);								break;
	case QT_FL_LOG_GAME_START:				QP_FL_LOG_GAME_START(dbquery, hstmt);						break;
	case QT_FL_LOG_GAME_END:				QP_FL_LOG_GAME_END(dbquery, hstmt);							break;
	case QT_FL_LOG_QUEST_COMPLETION:		QP_FL_LOG_QUEST_COMPLETION(dbquery, hstmt);					break;
	case QT_FL_LOG_PKWIN:					QP_FL_LOG_PKWIN(dbquery, hstmt);							break;
	case QT_FL_LOG_PKLOSS:					QP_FL_LOG_PKLOSS(dbquery, hstmt);							break;
	case QT_FL_LOG_DEAD:					QP_FL_LOG_DEAD(dbquery, hstmt);								break;
	case QT_FL_LOG_WARP:					QP_FL_LOG_WARP(dbquery, hstmt);								break;
	case QT_FL_LOG_SPEED_HACK:				QP_FL_LOG_SPEED_HACK(dbquery, hstmt);						break;
	case QT_FL_LOG_CREATE_CHARACTER:		QP_FL_LOG_CREATE_CHARACTER(dbquery, hstmt);					break;
	case QT_FL_LOG_DELETE_CHARACTER:		QP_FL_LOG_DELETE_CHARACTER(dbquery, hstmt);					break;
	case QT_FL_LOG_MEMORY_HACK:				QP_FL_LOG_MEMORY_HACK(dbquery, hstmt);						break;
	case QT_FL_LOG_PvEWIN:					QP_FL_LOG_PvEWIN(dbquery, hstmt);							break;
	case QT_FL_LOG_PvELOSS:					QP_FL_LOG_PvELOSS(dbquery, hstmt);							break;
	case QT_FL_LOG_ITEM_CHARACTER_ITEM:		QP_FL_LOG_ITEM_CHARACTER_ITEM(dbquery, hstmt);				break;
	case QT_FL_LOG_ITEM_GET_ITEM:			QP_FL_LOG_ITEM_GET_ITEM(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND:	QP_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND(dbquery, hstmt);		break;
	case QT_FL_LOG_ITEM_INSERT_TO_STORE:	QP_FL_LOG_ITEM_INSERT_TO_STORE(dbquery, hstmt);				break;
	case QT_FL_LOG_ITEM_DRAW_FROM_STORE:	QP_FL_LOG_ITEM_DRAW_FROM_STORE(dbquery, hstmt);				break;
	case QT_FL_LOG_ITEM_TRADE_SEND:			QP_FL_LOG_ITEM_TRADE_SEND(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_TRADE_RECV:			QP_FL_LOG_ITEM_TRADE_RECV(dbquery, hstmt);					break;
	case QT_FL_LOG_GUILD_STORE:				QP_FL_LOG_GUILD_STORE(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_THROWAWAY_ITEM:		QP_FL_LOG_ITEM_THROWAWAY_ITEM(dbquery, hstmt);				break;
	case QT_FL_LOG_ITEM_BUY_ITEM:			QP_FL_LOG_ITEM_BUY_ITEM(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_SELL_ITEM:			QP_FL_LOG_ITEM_SELL_ITEM(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_USE_ITEM:			QP_FL_LOG_ITEM_USE_ITEM(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_USE_ENERGY:			QP_FL_LOG_ITEM_USE_ENERGY(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_USE_ENCHANT:		QP_FL_LOG_ITEM_USE_ENCHANT(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_STORE_ITEM:			QP_FL_LOG_ITEM_STORE_ITEM(dbquery, hstmt);					break;
	case QT_FL_LOG_ITEM_USE_MIX:			QP_FL_LOG_ITEM_USE_MIX(dbquery, hstmt);						break;
	case QT_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN:	QP_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN(dbquery, hstmt);	break;

	case QT_FL_LOG_SERVER_INFO_MAP:			QP_FL_LOG_SERVER_INFO_MAP(dbquery, hstmt);					break;
	case QT_FL_LOG_SERVER_INFO_TOTAL:		QP_FL_LOG_SERVER_INFO_TOTAL(dbquery, hstmt);				break;
	case QT_FL_LOG_MONSTER_BOSS:			QP_FL_LOG_MONSTER_BOSS(dbquery, hstmt);						break;
	case QT_FL_LOG_START_FIELD_SERVER:		QP_FL_LOG_START_FIELD_SERVER(dbquery, hstmt);				break;
	case QT_FL_LOG_MS_WAR:					QP_FL_LOG_MS_WAR(dbquery, hstmt);						break;	// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	case QT_FL_LOG_SP_WAR:					QP_FL_LOG_SP_WAR(dbquery, hstmt);						break;	// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	case QT_FL_LOG_STATISTICS_MONEY:		QP_FL_LOG_STATISTICS_MONEY(dbquery, hstmt);				break;	// 2012-01-16 by hskim, 통계 - 화패
	case QT_FL_LOG_ITEM_TRY_ENCHANT_INFO:	QP_FL_LOG_ITEM_TRY_ENCHANT_INFO(dbquery, hstmt);		break;	// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

	default:
		{
			g_pGlobal->WriteSystemLogEX(TRUE, STRMSG_080904_0001, dbquery.enumQueryType, GetDBQueryTypeString(dbquery.enumQueryType));
			return FALSE;
		}
	}

	return TRUE;
}



void CAtumLogDBManager::QP_FL_LOG_LOGIN(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_LOGIN *pRMsg = (MSG_FL_LOG_LOGIN*)q.pQueryParam;

	SQLINTEGER cb2, cb4, cb5, cb6, cb7;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_connection
	-- Desc: inserts connection log
	--====
	--		2006-09-14 by cmkwon, Add Parameter - @i_ServerGroupID INT
	--		// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - atum_Update_ConnectingServerGroupID_Of_Account() 프로시저로 처리
	--
	CREATE PROCEDURE dbo.atum_log_insert_connection
		@i_LogType					TINYINT,
		@i_IPAddress				BINARY(4),
		@i_AccountUniqueNumber		INT,
		@i_AccountName				VARCHAR(20),
		@i_ServerName				VARCHAR(20),
		@i_ServerGroupID			INT,				-- 2006-09-14 by cmkwon, 추가함
		@i_PubilsherName			VARCHAR(20)			-- 2010-11 by dhjin, 아라리오 채널링 로그인.
	AS
		INSERT INTO atum_log_connection
		VALUES (@i_LogType, GetDate(), @i_IPAddress,
				@i_AccountUniqueNumber, @i_AccountName, @i_ServerName, @i_PubilsherName)

		-- // 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - atum_Update_ConnectingServerGroupID_Of_Account() 프로시저로 처리
		---- 2006-09-14 by cmkwon
		---- 로그인시(0 == @i_LogType)	==> ConnectingServerGroupID에 @i_ServerGroupID를 설정한다
		---- 로그아웃시(0 <> @i_LogType)	==> ConnectingServerGroupID에 0를 설정한다
		--IF (0 = @i_LogType)
		--	BEGIN
		--		UPDATE td_account
		--			SET ConnectingServerGroupID = @i_ServerGroupID
		--			WHERE AccountUniqueNumber = @i_AccountUniqueNumber
		--	END
		--ELSE
		--	BEGIN
		--		UPDATE td_account
		--			SET ConnectingServerGroupID = 0
		--			WHERE AccountUniqueNumber = @i_AccountUniqueNumber
		--	END
	GO
	**************************************************************************/
 	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	cb2 = 4;
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, 4, 0, pRMsg->IPAddress, 0, &cb2);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	cb4 = SQL_NTS;
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &cb4);
	cb5 = SQL_NTS;
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRMsg->ServerGroupName, 0, &cb5);

	CHAR szPublisherName[SIZE_MAX_GAME_PUBLISHER_NAME] = "Nemerian";
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GAME_PUBLISHER_NAME, 0, szPublisherName, 0, &cb6);

	// 2015-09-23 Future, added logging of MAC Address
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAC_ADDRESS, 0, pRMsg->MACAddress, 0, &cb7);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0247, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_connection1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szSysLog[1024];
		// 2015-09-23 Future, fixed Access Violation
		sprintf_s(szSysLog, sizeof(szSysLog), "atum_log_insert_connection ERROR: %lld %u.%u.%u.%u %d %s %s\r\n",
			q.nGeneralParam1, (UINT)pRMsg->IPAddress[0], (UINT)pRMsg->IPAddress[1], (UINT)pRMsg->IPAddress[2], (UINT)pRMsg->IPAddress[3],
			pRMsg->AccountUniqueNumber, pRMsg->AccountName, pRMsg->MACAddress);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
// 2006-09-14 by cmkwon, 현재 사용하지 않는 부분임
// 	/*[Stored Query Definition]************************************************
// 	* 로그인할때  프로시져 PB_GAMELOGIN 호출
// 	파라미터  
// 	@strClientID VARCHAR(20),  	-- User ID  <<  MGame ID
// 	@strIPAddr VARCHAR(16),   	-- User IP Address
// 	@nGameID INT    		-- GameSvr ID = 10061
// 
// 		 PB_GAMELOGIN @strClientID, @strIPAddr , @nGameID 
//  
// 	* 로그아웃할때 프로시져  PB_GAMELOGOUT 호출
// 	파라미터
// 	@strClientID VARCHAR(20)  	-- User ID  <<  MGame ID
//  
// 		PB_GAMELOGOUT @strClientID
// 	**************************************************************************/
// 	char strbuf[SIZE_MAX_IPADDRESS];
// 	sprintf(strbuf, "%d.%d.%d.%d", pRMsg->IPAddress[0], pRMsg->IPAddress[1], pRMsg->IPAddress[2], pRMsg->IPAddress[3]);
// 
// 	SQLINTEGER cb1 = SQL_NTS;
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, pRMsg->ServerGroupName, 0, &cb1);
// 	cb2 = SQL_NTS;
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 16, 0, strbuf, 0, &cb2);
// 	INT nGameID		= 10061;
// 	nGameID			= g_pLogGlobal->GetMGameServerID();
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nGameID, 0, NULL);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.PB_GAMELOGIN(?, ?, ?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
// 	{
// 		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@PB_GAMELOGIN1 Failed!\r\n", TRUE, q.pIOCPSocket);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pRMsg);
// 		return;
// 	}
//	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_LOGOUT(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_LOGOUT *pRMsg = (MSG_FL_LOG_LOGOUT*)q.pQueryParam;

	SQLINTEGER cb2, cb4, cb5;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_connection
	-- Desc: inserts connection log
	--====
	--		2006-09-14 by cmkwon, Add Parameter - @i_ServerGroupID INT
	--		// 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - atum_Update_ConnectingServerGroupID_Of_Account() 프로시저로 처리
	--
	CREATE PROCEDURE dbo.atum_log_insert_connection
		@i_LogType					TINYINT,
		@i_IPAddress				BINARY(4),
		@i_AccountUniqueNumber		INT,
		@i_AccountName				VARCHAR(20),
		@i_ServerName				VARCHAR(20),
		@i_ServerGroupID			INT				-- 2006-09-14 by cmkwon, 추가함
	AS
		INSERT INTO atum_log_connection
		VALUES (@i_LogType, GetDate(), @i_IPAddress,
				@i_AccountUniqueNumber, @i_AccountName, @i_ServerName)

		-- // 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기 - atum_Update_ConnectingServerGroupID_Of_Account() 프로시저로 처리
		---- 2006-09-14 by cmkwon
		---- 로그인시(0 == @i_LogType)	==> ConnectingServerGroupID에 @i_ServerGroupID를 설정한다
		---- 로그아웃시(0 <> @i_LogType)	==> ConnectingServerGroupID에 0를 설정한다
		--IF (0 = @i_LogType)
		--	BEGIN
		--		UPDATE td_account
		--			SET ConnectingServerGroupID = @i_ServerGroupID
		--			WHERE AccountUniqueNumber = @i_AccountUniqueNumber
		--	END
		--ELSE
		--	BEGIN
		--		UPDATE td_account
		--			SET ConnectingServerGroupID = 0
		--			WHERE AccountUniqueNumber = @i_AccountUniqueNumber
		--	END
	GO
	**************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	cb2 = 4;
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, 4, 0, pRMsg->IPAddress, 0, &cb2);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	cb4 = SQL_NTS;
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &cb4);
	cb5 = SQL_NTS;
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRMsg->ServerGroupName, 0, &cb5);

	CHAR szPublisherName[SIZE_MAX_GAME_PUBLISHER_NAME] = "Nemerian";
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GAME_PUBLISHER_NAME, 0, szPublisherName, 0, NULL);

	// 2015-09-23 Future, added logging of MAC Address
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAC_ADDRESS, 0, pRMsg->MACAddress, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0247, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_connection2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szSysLog[1024];
		// 2015-09-23 Future, fixed Access Violation
		sprintf_s(szSysLog, "atum_log_insert_connection ERROR: %lld %u.%u.%u.%u %d %s %s\r\n",
			q.nGeneralParam1, pRMsg->IPAddress[0], pRMsg->IPAddress[1], pRMsg->IPAddress[2], pRMsg->IPAddress[3],
			pRMsg->AccountUniqueNumber, pRMsg->AccountName, pRMsg->MACAddress);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
// 2006-09-14 by cmkwon, 현재 사용하지 않는 부분임
// 	SQLINTEGER cb1 = SQL_NTS;
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, pRMsg->AccountName, 0, &cb1);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.PB_GAMELOGOUT(?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
// 	{
// 		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@PB_GAMELOGOUT1 Failed!\r\n", TRUE, q.pIOCPSocket);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pRMsg);
// 		return;
// 	}
// 	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_LEVEL(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_LEVEL *pRMsg = (MSG_FL_LOG_LEVEL*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_level_stat
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					SMALLINT,
		@i_Param2					SMALLINT,
		@i_Param3					BIGINT
	AS
		INSERT INTO atum_log_user_level_stat
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1, @i_Param2, @i_Param3)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->FromLevel, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->ToLevel, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->TotalPlayTime, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0248, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_level_stat1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_EXP(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_EXP *pRMsg = (MSG_FL_LOG_EXP*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_exp
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					FLOAT,
		@i_Param2					BIGINT
	AS
		INSERT INTO atum_log_user_exp
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1, @i_Param2)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->AcquiredExperience, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pRMsg->CharacterExperiece, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0249, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_exp Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "LogType(%d) %s, Param1(%5.2f) Param2(%10.2f)\r\n"
			, q.nGeneralParam1, GetLogBaseString(pRMsg, string()), pRMsg->AcquiredExperience, pRMsg->CharacterExperiece);
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_STAT(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_STAT *pRMsg = (MSG_FL_LOG_STAT*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_level_stat
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					SMALLINT,
		@i_Param2					SMALLINT,
		@i_Param3					BIGINT
	AS
		INSERT INTO atum_log_user_level_stat
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1, @i_Param2, @i_Param3)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	// 2013-04-04 by jhseol, DB로그값 허용 범위 초과 버그 수정 - SQL_C_USHORT => SQL_C_UTINYINT 자료형 변경
//	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->StatKind, 0, NULL);
//	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->RemainedBonusStat, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &pRMsg->StatKind, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_SMALLINT, 0, 0, &pRMsg->RemainedBonusStat, 0, NULL);
	// end 2013-04-04 by jhseol, DB로그값 허용 범위 초과 버그 수정 - SQL_C_USHORT => SQL_C_UTINYINT 자료형 변경
	INT64 TotalPlayTime = 0;
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &TotalPlayTime, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0248, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_level_stat2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_GAME_START(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_GAME_START *pRMsg = (MSG_FL_LOG_GAME_START*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_user_game_start_end
	-- Desc: inserts log
	--			2006-12-18 by cmkwon, 인자 추가(@i_PCBangPlayTime INT)
	--====
	CREATE PROCEDURE atum_log_insert_user_game_start_end
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_CharacterName			VARCHAR(20),
		@i_AccountUniqueNumber		INT,			-- UINT
		@i_AccountName				VARCHAR(20),
		@i_PlayTime					INT,
		@i_TotalPlayTime			BIGINT,
		@i_PCBangPlayTime			INT,				-- 2006-12-18 by cmkwon, 
		@i_PCBangUID				INT				-- 2007-01-22 by dhjin
	AS
		INSERT INTO atum_log_user_game_start_end
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_CharacterName, @i_AccountUniqueNumber, @i_AccountName, @i_PlayTime, @i_TotalPlayTime, @i_PCBangPlayTime, @i_PCBangUID)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLINTEGER cb1 = SQL_NTS;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0, &cb1);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	SQLINTEGER cb2 = SQL_NTS;
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &cb2);
	int nPlay = 0;
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPlay, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->TotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPlay, 0, NULL);			// 2006-12-18 by cmkwon, 추가함
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->PCBangUID, 0, NULL);	// 2007-01-23 by dhjin, 추가함

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0028, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_game_start_end1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_GAME_END(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_GAME_END *pRMsg = (MSG_FL_LOG_GAME_END*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_user_game_start_end
	-- Desc: inserts log
	--			2006-12-18 by cmkwon, 인자 추가(@i_PCBangPlayTime INT)
	--====
	CREATE PROCEDURE atum_log_insert_user_game_start_end
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_CharacterName			VARCHAR(20),
		@i_AccountUniqueNumber		INT,			-- UINT
		@i_AccountName				VARCHAR(20),
		@i_PlayTime					INT,
		@i_TotalPlayTime			BIGINT,
		@i_PCBangPlayTime			INT,				-- 2006-12-18 by cmkwon, 
		@i_PCBangUID				INT
	AS
		INSERT INTO atum_log_user_game_start_end
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_CharacterName, @i_AccountUniqueNumber, @i_AccountName, @i_PlayTime, @i_TotalPlayTime, @i_PCBangPlayTime, @i_PCBangUID)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLINTEGER cb1 = SQL_NTS;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0, &cb1);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	SQLINTEGER cb2 = SQL_NTS;
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &cb2);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->PlayTime, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->TotalPlayTime, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->PCBangPlayTime, 0, NULL);		// 2006-12-18 by cmkwon
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->PCBangUID, 0, NULL);			// 2007-01-22 by dhjin, 추가함

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0028, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_game_start_end2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_QUEST_COMPLETION(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_QUEST_COMPLETION *pRMsg = (MSG_FL_LOG_QUEST_COMPLETION*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_quest_pk_dead
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->QuestIndex, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0251, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_quest_pk_dead3 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_PKWIN(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_PKWIN *pRMsg = (MSG_FL_LOG_PKWIN*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_quest_pk_dead
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PeerCharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0251, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_quest_pk_dead4 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_PKLOSS(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_PKLOSS *pRMsg = (MSG_FL_LOG_PKLOSS*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_quest_pk_dead
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PeerCharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0251, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_quest_pk_dead2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_DEAD(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_DEAD *pRMsg = (MSG_FL_LOG_DEAD*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_quest_pk_dead
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->DamageKind, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0251, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_quest_pk_dead1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_WARP(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_WARP *pRMsg = (MSG_FL_LOG_WARP*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_warp
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_TargetMapIndex			SMALLINT,
		@i_TargetPosition_X			SMALLINT,
		@i_TargetPosition_Y			SMALLINT,
		@i_TargetPosition_Z			SMALLINT
	AS
		INSERT INTO atum_log_user_warp
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_TargetMapIndex, @i_TargetPosition_X, @i_TargetPosition_Y, @i_TargetPosition_Z)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->TargetMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->TargetPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->TargetPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->TargetPosition.z, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0252, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_warp Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_SPEED_HACK(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SPEED_HACK *pRMsg = (MSG_FL_LOG_SPEED_HACK*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_speed_hack
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_CheckType				TINYINT,
 		@i_ReceivedMoveCounts		INT,
 		@i_EngineItemNum			INT,	
		@i_ServerPastTime			INT,
		@i_ClientPastTime			INT,
		@i_PastDistance				INT,
 		@i_CurrentSpeed				INT
	AS
		INSERT INTO atum_log_user_speed_hack
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex, @i_Position_X,
				@i_Position_Y, @i_Position_Z, @i_CheckType, @i_ReceivedMoveCounts, @i_EngineItemNum,
				@i_ServerPastTime, @i_ClientPastTime, @i_PastDistance, @i_CurrentSpeed)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->CheckType, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ReceivedMoveCounts, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->EngineItemNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ServerPastTime, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ClientPastTime, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PastDistance, 0, NULL);	
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->CurrentSpeed, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0253, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_speed_hack Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_CREATE_CHARACTER(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_CREATE_CHARACTER *pRMsg = (MSG_FL_LOG_CREATE_CHARACTER*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_user_game_start_end
	-- Desc: inserts log
	--			2006-12-18 by cmkwon, 인자 추가(@i_PCBangPlayTime INT)
	--====
	CREATE PROCEDURE [dbo].[atum_log_insert_user_game_start_end]
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_CharacterName			VARCHAR(20),
		@i_AccountUniqueNumber		INT,			-- UINT
		@i_AccountName				VARCHAR(20),
		@i_PlayTime					INT,
		@i_TotalPlayTime			BIGINT,
		@i_PCBangPlayTime			INT,				-- 2006-12-18 by cmkwon, 
		@i_PCBangUID				INT					-- 2007-01-24 by dhjin
	AS
		INSERT INTO atum_log_user_game_start_end
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_CharacterName, @i_AccountUniqueNumber, @i_AccountName, @i_PlayTime, @i_TotalPlayTime, @i_PCBangPlayTime, @i_PCBangUID)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLINTEGER cb1 = SQL_NTS;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0, &cb1);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	SQLINTEGER cb2 = SQL_NTS;
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &cb2);
	// 2013-05-23 by bckim, 베트남 서버다운 에러메세지 관련 수정 - 사용하지 않는 무의미한 값
// 	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->UnitKind, 0, NULL);
// 	INT64 nTPlay = 1;
// 	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &nTPlay, 0, NULL);
// 	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->UnitKind, 0, NULL);		// 2006-12-18 by cmkwon
	INT nPlaytime  = 0;
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPlaytime, 0, NULL);
	INT64 nTotalPlayTime  = 0;
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &nTotalPlayTime, 0, NULL);
	INT nPCBangPlayTime = 0;
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPCBangPlayTime, 0, NULL);	
	INT nPCBangUID  = 0;
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPCBangUID, 0, NULL);	
	// End. 2013-05-23 by bckim, 베트남 서버다운 에러메세지 관련 수정 - 사용하지 않는 무의미한 값

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0028, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_game_start_end3 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_DELETE_CHARACTER(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_DELETE_CHARACTER *pRMsg = (MSG_FL_LOG_DELETE_CHARACTER*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_user_game_start_end
	-- Desc: inserts log
	--			2006-12-18 by cmkwon, 인자 추가(@i_PCBangPlayTime INT)
	--====
	CREATE PROCEDURE [dbo].[atum_log_insert_user_game_start_end]
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_CharacterName			VARCHAR(20),
		@i_AccountUniqueNumber		INT,			-- UINT
		@i_AccountName				VARCHAR(20),
		@i_PlayTime					INT,
		@i_TotalPlayTime			BIGINT,
		@i_PCBangPlayTime			INT,				-- 2006-12-18 by cmkwon, 
		@i_PCBangUID				INT					-- 2007-01-24 by dhjin
	AS
		INSERT INTO atum_log_user_game_start_end
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
			@i_CharacterName, @i_AccountUniqueNumber, @i_AccountName, @i_PlayTime, @i_TotalPlayTime, @i_PCBangPlayTime, @i_PCBangUID)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLINTEGER cb1 = SQL_NTS;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0, &cb1);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);
	SQLINTEGER cb2 = SQL_NTS;
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &cb2);
	// 2013-05-23 by bckim, 베트남 서버다운 에러메세지 관련 수정 - 사용하지 않는 무의미한 값
// 	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->UnitKind, 0, NULL);
// 	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Level, 0, NULL);
// 	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->UnitKind, 0, NULL);		// 2006-12-18 by cmkwon
	INT nPlaytime  = 0;
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPlaytime, 0, NULL);
	INT64 nTotalPlayTime  = 0;
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &nTotalPlayTime, 0, NULL);
	INT nPCBangPlayTime = 0;
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPCBangPlayTime, 0, NULL);	
	INT nPCBangUID  = 0;
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nPCBangUID, 0, NULL);	
	// End. 2013-05-23 by bckim, 베트남 서버다운 에러메세지 관련 수정 - 사용하지 않는 무의미한 값


	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0028, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_game_start_end4 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_MEMORY_HACK(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_MEMORY_HACK *pRMsg = (MSG_FL_LOG_MEMORY_HACK*)q.pQueryParam;
	auto logType = T1_FL_LOG_MEMORY_HACK;

	///////////////////////////////////////////////////////////////////////////////
	// 2009-04-28 by cmkwon, 버그 수정(float 자료형 범위) - float 최소/최대값 범위 오류로 쿼리 오류 발생
	pRMsg->fCurrentValue	= max(pRMsg->fCurrentValue, FLT_MIN);
	pRMsg->fCurrentValue	= min(pRMsg->fCurrentValue, FLT_MAX);
	
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_log_insert_user_memory_hack
	-- Desc: 2005-12-20 by cmkwon
	--			inserts log
	--====

	CREATE PROCEDURE atum_log_insert_user_memory_hack
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_ItemKind					INT,
 		@i_ItemNum					INT,
 		@i_ItemUniqueNumber			BIGINT,
 		@i_MemoryHackCheckType		INT,	
		@i_ValidValue				FLOAT,
		@i_CurrentValue				FLOAT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_memory_hack
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex, @i_Position_X,
				@i_Position_Y, @i_Position_Z, @i_ItemKind, @i_ItemNum, @i_ItemUniqueNumber,
				@i_MemoryHackCheckType, @i_ValidValue, @i_CurrentValue, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->nItemKind, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->nItemNum0, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pRMsg->uid64ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->nMemHackCheckType, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->fValidValue, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pRMsg->fCurrentValue, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->nParam1, 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0254, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_memory_hack Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_PvEWIN(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_PvEWIN *pRMsg = (MSG_FL_LOG_PvEWIN*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_quest_pk_dead
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PeerCharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0251, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_quest_pk_dead5 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}


void CAtumLogDBManager::QP_FL_LOG_PvELOSS(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_PvELOSS *pRMsg = (MSG_FL_LOG_PvELOSS*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_Position_X				SMALLINT,
		@i_Position_Y				SMALLINT,
		@i_Position_Z				SMALLINT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_user_quest_pk_dead
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_Position_X, @i_Position_Y, @i_Position_Z,
				@i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.x, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.y, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentPosition.z, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PeerCharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0251, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_user_quest_pk_dead6 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(pRMsg, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}


void CAtumLogDBManager::QP_FL_LOG_ITEM_CHARACTER_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_CHARACTER_ITEM *pRMsg = (MSG_FL_LOG_ITEM_CHARACTER_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_charac_use
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_item_charac_use
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	int nLogParam1 = -1;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nLogParam1, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0255, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_charac_use1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_GET_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_GET_ITEM *pRMsg = (MSG_FL_LOG_ITEM_GET_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_get_throw
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT
	AS
		INSERT INTO atum_log_item_get_throw
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0256, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_get_throw2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND *pRMsg = (MSG_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_get_throw
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT
	AS
		INSERT INTO atum_log_item_get_throw
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0256, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_get_throw1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_INSERT_TO_STORE(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_INSERT_TO_STORE *pRMsg = (MSG_FL_LOG_ITEM_INSERT_TO_STORE*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_get_throw
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT
	AS
		INSERT INTO atum_log_item_get_throw
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0256, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_get_throw3 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_DRAW_FROM_STORE(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_DRAW_FROM_STORE *pRMsg = (MSG_FL_LOG_ITEM_DRAW_FROM_STORE*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_get_throw
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT
	AS
		INSERT INTO atum_log_item_get_throw
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0256, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_get_throw4 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_TRADE_SEND(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_TRADE_SEND *pRMsg = (MSG_FL_LOG_ITEM_TRADE_SEND*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_trade
		@i_LogType						TINYINT,
		@i_CharacterUniqueNumber		INT,
		@i_MapIndex						SMALLINT,
		@i_ItemUniqueNumber				BIGINT,
		@i_ItemNum						INT,
		@i_CurrentCount					INT,
		@i_ChangeCount					INT,
		@i_PeerCharacterName			VARCHAR(20),
		@i_PeerCharacterUniqueNumber	INT				-- UINT
	AS
		INSERT INTO atum_log_item_trade
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount, @i_PeerCharacterName, @i_PeerCharacterUniqueNumber)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);
	SQLINTEGER pcb = SQL_NTS;
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->PeerCharacterName, 0, &pcb);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PeerCharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0029, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_trade4 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_TRADE_RECV(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_TRADE_RECV *pRMsg = (MSG_FL_LOG_ITEM_TRADE_RECV*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_trade
		@i_LogType						TINYINT,
		@i_CharacterUniqueNumber		INT,
		@i_MapIndex						SMALLINT,
		@i_ItemUniqueNumber				BIGINT,
		@i_ItemNum						INT,
		@i_CurrentCount					INT,
		@i_ChangeCount					INT,
		@i_PeerCharacterName			VARCHAR(20),
		@i_PeerCharacterUniqueNumber	INT				-- UINT
	AS
		INSERT INTO atum_log_item_trade
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount, @i_PeerCharacterName, @i_PeerCharacterUniqueNumber)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);
	SQLINTEGER pcb = SQL_NTS;
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->PeerCharacterName, 0, &pcb);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PeerCharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0029, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_trade1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLogDBManager::QP_FL_LOG_GUILD_STORE(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLogDBManager::QP_FL_LOG_GUILD_STORE(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_GUILD_STORE *pRMsg = (MSG_FL_LOG_GUILD_STORE*)q.pQueryParam;
	SQLINTEGER		arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_Insert_GuildStore
		@i_LogType						TINYINT,
		@i_GuildUID						INT,
		@i_GuildName					VARCHAR(30),
		@i_AccountUID					INT,
		@i_AccountName					VARCHAR(20),
		@i_CharacterUID					INT,
		@i_CharacterName				VARCHAR(20),
		@i_ItemUID						BIGINT,
		@i_ItemNum						INT,
		@i_PrefixCodeNum				INT,
		@i_SuffixCodeNum				INT,
		@i_ItemResultCount				INT,
		@i_ItemChangeCount				INT
		AS
		INSERT INTO atum_log_GuildItem 
			VALUES (@i_LogType, GetDate(), @i_GuildUID, @i_GuildName, @i_AccountUID, @i_AccountName, 
					@i_CharacterUID, @i_CharacterName, @i_ItemUID, @i_ItemNum, 
					@i_PrefixCodeNum, @i_SuffixCodeNum, @i_ItemResultCount, @i_ItemChangeCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->GuildUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pRMsg->GuildName, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUID, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0, &arrCB[3]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ItemUID, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->SuffixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ItemResultCount, 0, NULL);
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ItemChangeCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0258, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "atum_log_Insert_GuildStore Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_THROWAWAY_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_THROWAWAY_ITEM *pRMsg = (MSG_FL_LOG_ITEM_THROWAWAY_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_get_throw
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT
	AS
		INSERT INTO atum_log_item_get_throw
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0256, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_get_throw5 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_BUY_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_BUY_ITEM *pRMsg = (MSG_FL_LOG_ITEM_BUY_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_buy_sell
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT,
		@i_RemainedMoney			INT
	AS
		INSERT INTO atum_log_item_buy_sell
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount, @i_RemainedMoney)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->RemainedMoney, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0259, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_buy_sell1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_SELL_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_SELL_ITEM *pRMsg = (MSG_FL_LOG_ITEM_SELL_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_buy_sell
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_ChangeCount				INT,
		@i_RemainedMoney			INT
	AS
		INSERT INTO atum_log_item_buy_sell
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount, @i_RemainedMoney)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ChangeCount, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->RemainedMoney, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0259, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_buy_sell2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_USE_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_USE_ITEM *pRMsg = (MSG_FL_LOG_ITEM_USE_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_charac_use
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_item_charac_use
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	int nLogParam1 = -1;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nLogParam1, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0255, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_charac_use4 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_USE_ENERGY(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_USE_ENERGY *pRMsg = (MSG_FL_LOG_ITEM_USE_ENERGY*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_charac_use
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_item_charac_use
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	int nLogParam1 = -1;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nLogParam1, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0255, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_charac_use5 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_USE_ENCHANT(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_USE_ENCHANT *pRMsg = (MSG_FL_LOG_ITEM_USE_ENCHANT*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_charac_use
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_item_charac_use
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->EnchantLogType, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0255, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_charac_use6 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_STORE_ITEM(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_STORE_ITEM *pRMsg = (MSG_FL_LOG_ITEM_STORE_ITEM*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_charac_use
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_item_charac_use
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	int nLogParam1 = -1;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nLogParam1, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0255, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_charac_use2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_USE_MIX(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_USE_MIX *pRMsg = (MSG_FL_LOG_ITEM_USE_MIX*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_charac_use
		@i_LogType					TINYINT,
		@i_CharacterUniqueNumber	INT,
		@i_MapIndex					SMALLINT,
		@i_ItemUniqueNumber			BIGINT,
		@i_ItemNum					INT,
		@i_CurrentCount				INT,
		@i_Param1					INT
	AS
		INSERT INTO atum_log_item_charac_use
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	int nLogParam1 = -1;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nLogParam1, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0255, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_charac_use3 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN *pRMsg = (MSG_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_item_trade
		@i_LogType						TINYINT,
		@i_CharacterUniqueNumber		INT,
		@i_MapIndex						SMALLINT,
		@i_ItemUniqueNumber				BIGINT,
		@i_ItemNum						INT,
		@i_CurrentCount					INT,
		@i_ChangeCount					INT,
		@i_PeerCharacterName			VARCHAR(20),
		@i_PeerCharacterUniqueNumber	INT				-- UINT
	AS
		INSERT INTO atum_log_item_trade
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
				@i_ChangeCount, @i_PeerCharacterName, @i_PeerCharacterUniqueNumber)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	int nChangeCount = 0;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nChangeCount, 0, NULL);
	SQLINTEGER pcb = SQL_NTS;
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->szOriginCharName, 0, &pcb);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0029, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_trade3 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	q.nGeneralParam1	= INT64(T1_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_NEW);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->CurrentMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.ItemNum, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->Item4Log.CurrentCount, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nChangeCount, 0, NULL);
	pcb = SQL_NTS;
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->szNewCharName, 0, &pcb);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->CharacterUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0029, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_item_trade3 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_SERVER_INFO_MAP(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SERVER_INFO_MAP *pRMsg = (MSG_FL_LOG_SERVER_INFO_MAP*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_server_map
		@i_LogType						TINYINT,
		@i_MapIndex						SMALLINT,
		@i_ChannelIndex					SMALLINT,
		@i_ClientCount					INT,
		@i_MonsterCount					INT
	AS
		INSERT INTO atum_log_server_map
		VALUES (@i_LogType, GetDate(), @i_MapIndex, @i_ChannelIndex,
				@i_ClientCount, @i_MonsterCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->MapIndex, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->ChannelIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ClientCount, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->MonsterCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0260, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_server_map1 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_SERVER_INFO_TOTAL(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_SERVER_INFO_TOTAL *pRMsg = (MSG_FL_LOG_SERVER_INFO_TOTAL*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_server_map
		@i_LogType						TINYINT,
		@i_MapIndex						SMALLINT,
		@i_ChannelIndex					SMALLINT,
		@i_ClientCount					INT,
		@i_MonsterCount					INT
	AS
		INSERT INTO atum_log_server_map
		VALUES (@i_LogType, GetDate(), @i_MapIndex, @i_ChannelIndex,
				@i_ClientCount, @i_MonsterCount)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &q.nGeneralParam1, 0, NULL);
	int nMapIndex = 0;
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &nMapIndex, 0, NULL);
	int nChannIdx = 0;
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &nChannIdx, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->ClientCount, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->MonsterCount, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0260, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_server_map2 Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_MONSTER_BOSS(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_MONSTER_BOSS *pRMsg = (MSG_FL_LOG_MONSTER_BOSS*)q.pQueryParam;
	char szItems[200];
	util::zero(szItems, 200);
	if(q.pGeneralParam)
	{
		util::strncpy(szItems, (char*)q.pGeneralParam, 200);
		util::del(q.pGeneralParam);
	}

	auto logType = T1_FL_LOG_MONSTER_BOSS;
	SQLINTEGER cb[19]; util::zero(cb, sizeof(SQLINTEGER)*19);
	char tmpCreatedTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	char tmpDeadTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_monster_boss
		@i_LogType					TINYINT,
		@i_MapIndex					SMALLINT,
		@i_ChannelIndex				SMALLINT,
 		@i_MonsterUnitKind			INT,		-- 생성 몬스터 정보
 		@i_CreatedTime				VARCHAR(30),
 		@i_DeadTime					VARCHAR(30),
 		@i_AttackUser1				VARCHAR(20),
 		@i_AttackUser2				VARCHAR(20),
 		@i_AttackUser3				VARCHAR(20),
 		@i_AttackUser4				VARCHAR(20),
 		@i_AttackUser5				VARCHAR(20),
 		@i_AttackUser6				VARCHAR(20),
 		@i_AttackUser7				VARCHAR(20),
 		@i_AttackUser8				VARCHAR(20),
 		@i_AttackUser9				VARCHAR(20),
 		@i_AttackUser10				VARCHAR(20),
 		@i_DropItemList				VARCHAR(200)
	AS
		INSERT INTO atum_log_monster_boss
		VALUES (@i_LogType, GetDate(), @i_MapIndex, @i_ChannelIndex,
				@i_MonsterUnitKind, @i_CreatedTime, @i_DeadTime, @i_AttackUser1, @i_AttackUser2,
				@i_AttackUser3, @i_AttackUser4, @i_AttackUser5, @i_AttackUser6, @i_AttackUser7,
				@i_AttackUser8, @i_AttackUser9, @i_AttackUser10, @i_DropItemList)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, &cb[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->MapChannel.MapIndex, 0, &cb[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &pRMsg->MapChannel.ChannelIndex, 0, &cb[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->MonsterUnitKind, 0, &cb[4]);
	cb[5] = SQL_NTS;
	pRMsg->CreatedTime.GetSQLDateTimeString(tmpCreatedTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCreatedTimeBuf, 0, &cb[5]);
	cb[6] = SQL_NTS;
	pRMsg->DeadTime.GetSQLDateTimeString(tmpDeadTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpDeadTimeBuf, 0, &cb[6]);
	cb[7] = SQL_NTS;
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[0], 0, &cb[7]);
	cb[8] = SQL_NTS;
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[1], 0, &cb[8]);
	cb[9] = SQL_NTS;
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[2], 0, &cb[9]);
	cb[10] = SQL_NTS;
	SQLBindParameter(hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[3], 0, &cb[10]);
	cb[11] = SQL_NTS;
	SQLBindParameter(hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[4], 0, &cb[11]);
	cb[12] = SQL_NTS;
	SQLBindParameter(hstmt, 12, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[5], 0, &cb[12]);
	cb[13] = SQL_NTS;
	SQLBindParameter(hstmt, 13, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[6], 0, &cb[13]);
	cb[14] = SQL_NTS;
	SQLBindParameter(hstmt, 14, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[7], 0, &cb[14]);
	cb[15] = SQL_NTS;
	SQLBindParameter(hstmt, 15, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[8], 0, &cb[15]);
	cb[16] = SQL_NTS;
	SQLBindParameter(hstmt, 16, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->AttackUser[9], 0, &cb[16]);
	cb[17] = SQL_NTS;
	SQLBindParameter(hstmt, 17, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 200, 0, szItems, 0, &cb[17]);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0261, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_monster_boss Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

void CAtumLogDBManager::QP_FL_LOG_START_FIELD_SERVER(DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	-- 접속자 체크 프로시져
	CREATE PROCEDURE [dbo].[PB_INIT]
		@nGameID int		-- GameSvr ID
	AS
		--exec [pubaccount].[dbo].PB_INIT @nGameID

		-- 2007-01-29 by cmkwon, ServerGroupID 초기화
		UPDATE atum2_db_account.dbo.td_account 
		SET ConnectingServerGroupID = 0
		WHERE ConnectingServerGroupID = @nGameID
	GO
	**************************************************************************/

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0262, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA )		// 2013-04-05 by hskim, 쿼리안에 업데이트 대상이 없을 때 에러 메시지 찍는 버그 수정
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@PB_INIT Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLogDBManager::QP_FL_LOG_MS_WAR(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		모선전, 거점전 정보창 기획안 - 모선전 로그 추가
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLogDBManager::QP_FL_LOG_MS_WAR(DB_QUERY q, SQLHSTMT hstmt)
{// QT_FL_LOG_MS_WAR
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_Insert_MSWar
	-- DESC				: 모선전 로그 추가
	-- // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_Insert_MSWar
		@i_LogType					TINYINT,
		@i_AttInfluence				TINYINT,
		@i_MonsterUID				INT,
		@i_ContributionPoint		INT,
		@i_MSWarStartTime			VARCHAR(30),
		@i_WinInfluence				TINYINT
	AS
		INSERT INTO dbo.atum_log_MSWar(LogType, AttInfluence, MonsterUID, ContributionPoint, MSWarStartTime, MSWarEndTime, WinInfluence)
		VALUES(@i_LogType, @i_AttInfluence, @i_MonsterUID, @i_ContributionPoint, @i_MSWarStartTime, GetDate(), @i_WinInfluence)
	GO
	**************************************************************************/
	MSG_FL_LOG_MS_WAR *pRMsg = (MSG_FL_LOG_MS_WAR*)q.pQueryParam;

	auto logType = T1_FL_LOG_MS_WAR;

	char tmMSWarStartTime[SIZE_MAX_SQL_DATETIME_STRING];

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->AttInfluence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->MonsterUID, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->ContributionPoint, 0, NULL);
	pRMsg->MSWarStartTime.GetSQLDateTimeString(tmMSWarStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmMSWarStartTime, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->WinInfluence, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0263, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "dbo.atum_log_Insert_MSWar Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumLogDBManager::QP_FL_LOG_SP_WAR(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		모선전, 거점전 정보창 기획안 - 거점전 로그 추가
/// \author		dhjin
/// \date		2008-04-01 ~ 2008-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumLogDBManager::QP_FL_LOG_SP_WAR(DB_QUERY q, SQLHSTMT hstmt)
{// QT_FL_LOG_SP_WAR
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_log_Insert_SPWar
	-- DESC				: 거점전 로그 추가
	-- // 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_Insert_SPWar
		@i_LogType					TINYINT,
		@i_AttInfluence				TINYINT,
		@i_SPSummonMapIndex			INT,
		@i_SPWarStartTime			VARCHAR(30),
		@i_WinInfluence				TINYINT
	AS
		INSERT INTO dbo.atum_log_SPWar(LogType, AttInfluence, SPSummonMapIndex, SPWarStartTime, SPWarEndTime, WinInfluence)
		VALUES(@i_LogType, @i_AttInfluence, @i_SPSummonMapIndex, @i_SPWarStartTime, GetDate(), @i_WinInfluence)
	GO
	**************************************************************************/
	MSG_FL_LOG_SP_WAR *pRMsg = (MSG_FL_LOG_SP_WAR*)q.pQueryParam;

	auto logType = T1_FL_LOG_SP_WAR;

	char tmSPWarStartTime[SIZE_MAX_SQL_DATETIME_STRING];
	pRMsg->SPWarStartTime.GetSQLDateTimeString(tmSPWarStartTime, SIZE_MAX_SQL_DATETIME_STRING);
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->AttInfluence, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->SPSummonMapIndex, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmSPWarStartTime, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->WinInfluence, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0264, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "dbo.atum_log_Insert_SPWar Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

// start 2012-01-16 by hskim, 통계 - 화패
void CAtumLogDBManager::QP_FL_LOG_STATISTICS_MONEY(DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_log_Insert_StatisticsMoney
	-- DESC				: // 2012-01-16 by hskim, 통계 - 화패
	--------------------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[atum_log_Insert_StatisticsMoney]
		@i_LogType				TINYINT,
		@i_DeltaSPI				BIGINT,
		@i_DeltaWarPoint		BIGINT,
		@i_DeltaSystemOrb		BIGINT,
		@i_DeltaQuasarOrb		BIGINT,
		@i_DeltaClusterOrb		BIGINT,
		@i_DeltaGalaxyOrb		BIGINT,
		@i_DeltaUniverseOrb		BIGINT
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
	BEGIN
		INSERT INTO [atum2_db_1].[dbo].[atum_log_statistics_money] ([LogType], [Time],[DeltaSPI], [DeltaWarPoint], [DeltaSystemOrb], [DeltaQuasarOrb], [DeltaClusterOrb], [DeltaGalaxyOrb], [DeltaUniverseOrb])
		 VALUES(@i_LogType, getdate(), @i_DeltaSPI, @i_DeltaWarPoint, @i_DeltaSystemOrb, @i_DeltaQuasarOrb, @i_DeltaClusterOrb, @i_DeltaGalaxyOrb, @i_DeltaUniverseOrb)
	END
	GO
	**************************************************************************/
	MSG_FL_LOG_STATISTICS_MONEY *pRMsg = (MSG_FL_LOG_STATISTICS_MONEY*)q.pQueryParam;

	auto logType = T1_FL_LOG_STATISTICS_MONEY;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pRMsg->FunctionType, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->SPI, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->WarPoint, 0, NULL);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->SystemOrb, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->QuasarOrb, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->ClusterOrb, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->GalaxyOrb, 0, NULL);
	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pRMsg->UniverseOrb, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_120116_0001, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "dbo.atum_log_Insert_StatisticsMoney Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	util::del(pRMsg);
}
// end 2012-01-16 by hskim, 통계 - 화패


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumLogDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumLogDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	// error: no such DB query type
	server::log(" [ERROR] CAtumLogDBManager::ProcessDynamicServerQuery# no such DB Query Type !! %d(%s)\r\n",
		i_qQuery->enumQueryType, GetDBQueryTypeString(i_qQuery->enumQueryType));

	return false;
}

// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
void CAtumLogDBManager::QP_FL_LOG_ITEM_TRY_ENCHANT_INFO(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO_STRING *pRMsg = (MSG_FL_LOG_ITEM_TRY_ENCHANT_INFO_STRING*)q.pQueryParam;
	auto logType = T1_FL_LOG_ITEM_TRY_ENCHANT_INFO;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER,	 0, 0, &pRMsg->TryEnchantInfo.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT,	 0, 0, &pRMsg->TryEnchantInfo.Item4Log.ItemUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,	 0, 0, &pRMsg->TryEnchantInfo.Item4Log.ItemNum, 0, NULL);
	char* pTempSuccessStr = GetTryEnchantSuccessTypeString(pRMsg->TryEnchantInfo.Success);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 32, 0, pTempSuccessStr, 0, NULL);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->TryEnchantInfo.PrefixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->TryEnchantInfo.SuffixCodeNum, 0, NULL);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->TryEnchantInfo.EnchantCount, 0, NULL);

	SQLINTEGER pcb = SQL_NTS;
 	SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 1024, 0, &pRMsg->szCardList, 0, &pcb);
	
 	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_130702_0001, SQL_NTS);
 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
 	{
 		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "dbo.atum_log_Insert_Try_Enchant_Info Failed!\r\n", TRUE, q.pIOCPSocket);
 		SQLFreeStmt(hstmt, SQL_CLOSE);
 		util::del(pRMsg);
 		return;
 	}
 	SQLFreeStmt(hstmt, SQL_CLOSE);
 
 	util::del(pRMsg);
}
// End. 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완
