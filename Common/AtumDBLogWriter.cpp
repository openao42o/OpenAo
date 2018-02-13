#include "stdafx.h"
#include "atumprotocol.h"
#include "AtumDBLogWriter.h"
#include "AtumDBManager.h"
#include "odbcstatement.h"
#include "GlobalGameServer.h"
#ifdef _ATUM_LOG_SERVER
#include "LogGlobal.h"
#endif

#ifndef DBGOUT
#define DBGOUT (void(0))
#endif

CAtumDBLogWriter::CAtumDBLogWriter()
{
}

CAtumDBLogWriter::~CAtumDBLogWriter()
{
}

const char* GetLogBaseString(FL_USER_LOG_BASE *pLogBase, string& str)
{
	char buffer[512];
	sprintf(buffer, "CharUID(%d) MapIndex(%d) Pos(%d, %d, %d)",
		pLogBase->CharacterUniqueNumber, pLogBase->CurrentMapIndex,
		pLogBase->CurrentPosition.x, pLogBase->CurrentPosition.y, pLogBase->CurrentPosition.z);
	str += buffer;

	return str.c_str();
}

// 2006-09-14 by cmkwon, 사용하지 않는 함수들
// BOOL CAtumDBLogWriter::InsertLog_Connection(
// 						CODBCStatement *i_pODBCStmt,
// 						BYTE			i_nLogType,
// 						BYTE			*i_arrIPAddr,
// 						UID32_t			i_nAccountUniqueNumber,
// 						char			*i_szAccountName,
// 						char			*i_szServerName)
// {
// 	SQLINTEGER cb2, cb4, cb5;
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_connection
// 		@i_LogType					TINYINT,
// 		@i_IPAddress				BINARY(4),
// 		@i_AccountUniqueNumber		INT,
// 		@i_AccountName				VARCHAR(20),
// 		@i_ServerName				VARCHAR(20)
// 	AS
// 		INSERT INTO atum_log_connection
// 		VALUES (@i_LogType, GetDate(), @i_IPAddress,
// 				@i_AccountUniqueNumber, @i_AccountName, @i_ServerName)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	cb2 = 4;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, 4, 0, i_arrIPAddr, 0, &cb2);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_nAccountUniqueNumber, 0, NULL);
// 	cb4 = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccountName, 0, &cb4);
// 	cb5 = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, i_szServerName, 0, &cb5);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_connection(?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// #ifdef _DEBUG
// 		DBGOUT("atum_log_insert_connection ERROR: %d %X.%X.%X.%X %d %s\r\n",
// 			i_nLogType, i_arrIPAddr[0], i_arrIPAddr[1], i_arrIPAddr[2], i_arrIPAddr[3],
// 			i_nAccountUniqueNumber, i_szAccountName);
// #endif
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 
// /*
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
// */
// 	if (i_nLogType == T1_FL_LOG_LOGIN)
// 	{
// 		char strbuf[16];
// 		sprintf(strbuf, "%d.%d.%d.%d", i_arrIPAddr[0], i_arrIPAddr[1], i_arrIPAddr[2], i_arrIPAddr[3]);
// 
// 		SQLINTEGER cb1 = SQL_NTS;
// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, i_szAccountName, 0, &cb1);
// 		SQLINTEGER cb2 = SQL_NTS;
// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 16, 0, strbuf, 0, &cb2);
// 
// 		INT nGameID		= 10061;
// #ifdef _ATUM_LOG_SERVER
// 		nGameID			= g_pLogGlobal->GetMGameServerID();
// #endif
// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nGameID, 0, NULL);
// 		BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.PB_GAMELOGIN(?, ?, ?)}", TRUE);
// 		if (!bRet)
// 		{
// 			DBGOUT(STRCMD_CS_COMMON_DB_0000);
// 			i_pODBCStmt->FreeStatement();
// 			return FALSE;
// 		}
// 
// 		i_pODBCStmt->FreeStatement();
// 	}
// 	else if (i_nLogType == T1_FL_LOG_LOGOUT)
// 	{
// 		SQLINTEGER cb1 = SQL_NTS;
// 		SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, i_szAccountName, 0, &cb1);
// 
// 		BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.PB_GAMELOGOUT(?)}", TRUE);
// 		if (!bRet)
// 		{
// 			DBGOUT(STRCMD_CS_COMMON_DB_0000);
// 			i_pODBCStmt->FreeStatement();
// 			return FALSE;
// 		}
// 
// 		i_pODBCStmt->FreeStatement();
// 	}
// 
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_User_Level_Stat(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_USER_LOG_BASE	*i_pUserLogBase,
// 						USHORT				i_nParam1,
// 						USHORT				i_nParam2,
// 						LONGLONG			i_nParam3)
// {
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_user_level_stat
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_Position_X				SMALLINT,
// 		@i_Position_Y				SMALLINT,
// 		@i_Position_Z				SMALLINT,
// 		@i_Param1					SMALLINT,
// 		@i_Param2					SMALLINT,
// 		@i_Param3					BIGINT
// 	AS
// 		INSERT INTO atum_log_user_level_stat
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_Position_X, @i_Position_Y, @i_Position_Z,
// 				@i_Param1, @i_Param2, @i_Param3)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pUserLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.z, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_nParam1, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_nParam2, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &i_nParam3, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_user_level_stat(?, ?, ?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// //		i_pODBCStmt->ProcessLogMessagesForStmt("atum_log_insert_user_level_stat Failed!\r\n");
// 		char szTemp[1024];
// 		sprintf(szTemp, "%s\r\n", GetLogBaseString(i_pUserLogBase, string()));
// 		g_pGlobal->WriteSystemLog(szTemp);
// 		DBGOUT(szTemp);
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_User_Exp(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_USER_LOG_BASE	*i_pUserLogBase,
// 						FLOAT				i_fParam1,
// 						LONGLONG			i_nParam2)
// {
// //#ifdef _ATUM_LOG_SERVER
// //	char szLog[1024];
// //	sprintf(szLog, "User_Exp : LogType(%2d) CharUID(%8d) MapIndex(%d) Pos(%5d, %5d, %5d) AcquiredExp(%6.2f) CharEXP(%I64d)\r\n"
// //		, i_nLogType, i_pUserLogBase->CharacterUniqueNumber, i_pUserLogBase->CurrentMapIndex
// //		, i_pUserLogBase->CurrentPosition.x, i_pUserLogBase->CurrentPosition.y, i_pUserLogBase->CurrentPosition.z 
// //		, i_fParam1, i_nParam2);
// //	g_pLogGlobal->WriteSystemLog(szLog);
// //	return TRUE;
// //#endif
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_exp
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_Position_X				SMALLINT,
// 		@i_Position_Y				SMALLINT,
// 		@i_Position_Z				SMALLINT,
// 		@i_Param1					FLOAT,
// 		@i_Param2					BIGINT
// 	AS
// 		INSERT INTO atum_log_user_exp
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_Position_X, @i_Position_Y, @i_Position_Z,
// 				@i_Param1, @i_Param2)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pUserLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.z, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &i_fParam1, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &i_nParam2, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_exp(?, ?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// //		i_pODBCStmt->ProcessLogMessagesForStmt("atum_log_insert_exp Failed!\r\n");
// 		char szTemp[1024];
// 		sprintf(szTemp, "LogType(%d) %s, Param1(%5.2f) Param2(%10.2f)\r\n"
// 			, i_nLogType, GetLogBaseString(i_pUserLogBase, string()), i_fParam1, i_nParam2);
// 		g_pGlobal->WriteSystemLog(szTemp);
// 		DBGOUT(szTemp);
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 

// 2008-01-17 by cmkwon, T_A: 삭제 상태의 캐릭터 영구히 삭제하는 기능 추가 - CAtumDBLogWriter::InsertLog_User_Game_Start_End() 를 주석상태를 복원 시킴
BOOL CAtumDBLogWriter::InsertLog_User_Game_Start_End(
						CODBCStatement		*i_pODBCStmt,
						T1<T0::FL_LOG>		i_nLogType,
						FL_USER_LOG_BASE	*i_pUserLogBase,
						char				*i_szCharacterName,
						UID32_t				i_nAccountUniqueNumber,
						char				*i_szAccountName,
						UINT				i_nPlayTime,
						LONGLONG			I_nTotalPlayTime)
{
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
	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pUserLogBase->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentMapIndex, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.x, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.y, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.z, 0, NULL);
	SQLINTEGER cb1 = SQL_NTS;
	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szCharacterName, 0, &cb1);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_nAccountUniqueNumber, 0, NULL);
	SQLINTEGER cb2 = SQL_NTS;
	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccountName, 0, &cb2);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_nPlayTime, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &I_nTotalPlayTime, 0, NULL);

	// 2008-01-17 by cmkwon, T_A: 삭제 상태의 캐릭터 영구히 삭제하는 기능 추가 - 주석 처리된 후 추가된 인자 추가함
	int PCBangPalyTime	= 0;
	int PCBangUID		= 0;
	SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &PCBangPalyTime, 0, NULL);		// 2006-12-18 by cmkwon
	SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &PCBangUID, 0, NULL);			// 2007-01-22 by dhjin, 추가함

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080822_0028), TRUE);
	if (!bRet)
	{
//		i_pODBCStmt->ProcessLogMessagesForStmt("atum_log_insert_user_game_start_end Failed!\r\n");
		char szTemp[1024];
		sprintf(szTemp, "%s\r\n", GetLogBaseString(i_pUserLogBase, string()));
		g_pGlobal->WriteSystemLog(szTemp);
		DBGOUT(szTemp);
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	i_pODBCStmt->FreeStatement();
	return TRUE;
}
// 
// BOOL CAtumDBLogWriter::InsertLog_User_Quest_PK_Dead(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_USER_LOG_BASE	*i_pUserLogBase,
// 						INT					i_nParam1)
// {
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_user_quest_pk_dead
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_Position_X				SMALLINT,
// 		@i_Position_Y				SMALLINT,
// 		@i_Position_Z				SMALLINT,
// 		@i_Param1					INT
// 	AS
// 		INSERT INTO atum_log_user_quest_pk_dead
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_Position_X, @i_Position_Y, @i_Position_Z,
// 				@i_Param1)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pUserLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.z, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nParam1, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_user_quest_pk_dead(?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// //		i_pODBCStmt->ProcessLogMessagesForStmt("atum_log_insert_user_quest_pk_dead Failed!\r\n");
// 		char szTemp[1024];
// 		sprintf(szTemp, "%s\r\n", GetLogBaseString(i_pUserLogBase, string()));
// 		g_pGlobal->WriteSystemLog(szTemp);
// 		DBGOUT(szTemp);
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_User_WARP(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_USER_LOG_BASE	*i_pUserLogBase,
// 						MapIndex_t			i_nTargetMapIndex,
// 						AVECTOR3			&i_TargetPosition)
// {
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_user_warp
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_Position_X				SMALLINT,
// 		@i_Position_Y				SMALLINT,
// 		@i_Position_Z				SMALLINT,
// 		@i_TargetMapIndex			SMALLINT,
// 		@i_TargetPosition_X			SMALLINT,
// 		@i_TargetPosition_Y			SMALLINT,
// 		@i_TargetPosition_Z			SMALLINT
// 	AS
// 		INSERT INTO atum_log_user_warp
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_Position_X, @i_Position_Y, @i_Position_Z,
// 				@i_TargetMapIndex, @i_TargetPosition_X, @i_TargetPosition_Y, @i_TargetPosition_Z)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pUserLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pUserLogBase->CurrentPosition.z, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_nTargetMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_TargetPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_TargetPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_TargetPosition.z, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_user_warp(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// //		i_pODBCStmt->ProcessLogMessagesForStmt("atum_log_insert_user_warp Failed!\r\n");
// 		char szTemp[1024];
// 		sprintf(szTemp, "%s\r\n", GetLogBaseString(i_pUserLogBase, string()));
// 		g_pGlobal->WriteSystemLog(szTemp);
// 		DBGOUT(szTemp);
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_Item_Charac_Use(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_ITEM_LOG_BASE	*i_pItemLogBase,
// 						ITEM_FOR_LOG		*i_pItem4Log,
// 						int					i_nLogParam1/*=-1*/)
// {
// //#ifdef _ATUM_LOG_SERVER
// //	char szLog[1024];
// //	sprintf(szLog, "Item_Charac_Use : LogType(%2d) CharUID(%8d) MapIndex(%d) ItemUID(%8d) ItemNum(%8d) CurrentCounts(%4d)\r\n"
// //		, i_nLogType, i_pItemLogBase->CharacterUniqueNumber, i_pItemLogBase->CurrentMapIndex
// //		, i_pItem4Log->ItemUniqueNumber, i_pItem4Log->ItemNum, i_pItem4Log->CurrentCount);
// //	g_pLogGlobal->WriteSystemLog(szLog);
// //	return TRUE;
// //#endif
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_item_charac_use
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_ItemUniqueNumber			BIGINT,
// 		@i_ItemNum					INT,
// 		@i_CurrentCount				INT,
// 		@i_Param1					INT
// 	AS
// 		INSERT INTO atum_log_item_charac_use
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount, @i_Param1)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pItemLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pItemLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &i_pItem4Log->ItemUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->ItemNum, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->CurrentCount, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nLogParam1, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_item_charac_use(?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_Item_Get_Throw(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_ITEM_LOG_BASE	*i_pItemLogBase,
// 						ITEM_FOR_LOG		*i_pItem4Log,
// 						INT					i_nChangeCount)
// {
// //#ifdef _ATUM_LOG_SERVER
// //	char szLog[1024];
// //	sprintf(szLog, "Item_Get_Thrw : LogType(%2d) CharUID(%8d) MapIndex(%d) ItemUID(%8d) ItemNum(%8d) CurrentCounts(%4d) ChangeCount(%6d)\r\n"
// //		, i_nLogType, i_pItemLogBase->CharacterUniqueNumber, i_pItemLogBase->CurrentMapIndex
// //		, i_pItem4Log->ItemUniqueNumber, i_pItem4Log->ItemNum, i_pItem4Log->CurrentCount
// //		, i_nChangeCount);
// //	g_pLogGlobal->WriteSystemLog(szLog);
// //	return TRUE;
// //#endif
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_item_get_throw
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_ItemUniqueNumber			BIGINT,
// 		@i_ItemNum					INT,
// 		@i_CurrentCount				INT,
// 		@i_ChangeCount				INT
// 	AS
// 		INSERT INTO atum_log_item_get_throw
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
// 				@i_ChangeCount)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pItemLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pItemLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &i_pItem4Log->ItemUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->ItemNum, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->CurrentCount, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nChangeCount, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_item_get_throw(?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_Item_Buy_Sell(
// 						CODBCStatement		*i_pODBCStmt,
// 						BYTE				i_nLogType,
// 						FL_ITEM_LOG_BASE	*i_pItemLogBase,
// 						ITEM_FOR_LOG		*i_pItem4Log,
// 						INT					i_nChangeCount,
// 						INT					i_nRemainedMoney)
// {
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_item_buy_sell
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_ItemUniqueNumber			BIGINT,
// 		@i_ItemNum					INT,
// 		@i_CurrentCount				INT,
// 		@i_ChangeCount				INT,
// 		@i_RemainedMoney			INT
// 	AS
// 		INSERT INTO atum_log_item_buy_sell
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex,
// 				@i_ItemUniqueNumber, @i_ItemNum, @i_CurrentCount,
// 				@i_ChangeCount, @i_RemainedMoney)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pItemLogBase->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pItemLogBase->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &i_pItem4Log->ItemUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->ItemNum, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->CurrentCount, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nChangeCount, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nRemainedMoney, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_item_buy_sell(?, ?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 


// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능
// SCAdminTool에서 사용

BOOL CAtumDBLogWriter::InsertLog_Item_Enchant_Change_By_Admin(
						CODBCStatement		*i_pODBCStmt,
						T1<T0::FL_LOG>		i_nLogType,
						FL_ITEM_LOG_BASE	*i_pItemLogBase,		// 케릭터 유니크 넘버 
						ITEM_FOR_LOG		*i_pItem4Log,			// ItemUniqueNumber;  ItemNum; CurrentCount;
						INT					i_EnchantCardNumber,
						INT					i_ChangeEnchantCount,
						INT					i_ResultEnchantCount,
						char				*i_szCharacterName)		// 운영자 계정
{
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE [dbo].[atum_log_insert_item_Enchant_Change_By_Admin]
		@i_LogType						TINYINT,
		@i_CharacterUniqueNumber		INT,
		@i_ItemUniqueNumber				BIGINT,
		@i_ItemNum						INT,
		@i_EnchantCardNumber						INT,
		@i_ChangeEnchantCount					INT,
		@i_ResultEnchantCount					INT,
		@i_PeerCharacterName			VARCHAR(20)
	AS
		INSERT INTO dbo.atum_log_item_Enchant_Change_By_Admin
		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber,
				@i_ItemUniqueNumber, @i_ItemNum, @i_EnchantCardNumber,
				@i_ChangeEnchantCount,@i_ResultEnchantCount,
				@i_PeerCharacterName)
	GO
	**************************************************************************/
	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pItemLogBase->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &i_pItem4Log->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->ItemNum, 0, NULL);

	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_EnchantCardNumber, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_ChangeEnchantCount, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_ResultEnchantCount, 0, NULL);
	SQLINTEGER pcb = SQL_NTS;
	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szCharacterName, 0, &pcb);

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_121121_0002), TRUE);
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	i_pODBCStmt->FreeStatement();
	return TRUE;
}
// 2012-11-21 by bckim, 운영툴기능추가, 인챈트아이템제거기능. End


// 2006-09-21 by cmkwon, SCAdminTool에서 사용하고 있음
BOOL CAtumDBLogWriter::InsertLog_Item_Trade(
						CODBCStatement		*i_pODBCStmt,
						T1<T0::FL_LOG>		i_nLogType,
						FL_ITEM_LOG_BASE	*i_pItemLogBase,
						ITEM_FOR_LOG		*i_pItem4Log,
						INT					i_nChangeCount,
						char				*i_szCharacterName,
						UID32_t				i_nCharacterUniqueNumber)
{
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
	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pItemLogBase->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pItemLogBase->CurrentMapIndex, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &i_pItem4Log->ItemUniqueNumber, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->ItemNum, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pItem4Log->CurrentCount, 0, NULL);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nChangeCount, 0, NULL);
	SQLINTEGER pcb = SQL_NTS;
	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szCharacterName, 0, &pcb);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nCharacterUniqueNumber, 0, NULL);

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080822_0029), TRUE);
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	i_pODBCStmt->FreeStatement();
	return TRUE;
}

// 
// BOOL CAtumDBLogWriter::InsertLog_Server_Map(
// 					CODBCStatement		*i_pODBCStmt,
// 					BYTE				i_nLogType,
// 					MapIndex_t			i_nMapIndex,
// 					ChannelIndex_t		i_nChannelIndex,
// 					INT					i_nClientCount,
// 					INT					i_nMonsterCount)
// {
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_server_map
// 		@i_LogType						TINYINT,
// 		@i_MapIndex						SMALLINT,
// 		@i_ChannelIndex					SMALLINT,
// 		@i_ClientCount					INT,
// 		@i_MonsterCount					INT
// 	AS
// 		INSERT INTO atum_log_server_map
// 		VALUES (@i_LogType, GetDate(), @i_MapIndex, @i_ChannelIndex,
// 				@i_ClientCount, @i_MonsterCount)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_nMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_nChannelIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nClientCount, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nMonsterCount, 0, NULL);
// 	INT nGameID		= 10061;
// #ifdef _ATUM_LOG_SERVER
// 		nGameID			= g_pLogGlobal->GetMGameServerID();
// #endif
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nGameID, 0, NULL);
// 
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_server_map(?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_Speed_Hack(
// 								 CODBCStatement			*i_pODBCStmt,
// 								 MSG_FL_LOG_SPEED_HACK	*i_pMsgSpeedHack)
// {
// 	BYTE logType = T1_FL_LOG_SPEED_HACK;
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_user_speed_hack
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_Position_X				SMALLINT,
// 		@i_Position_Y				SMALLINT,
// 		@i_Position_Z				SMALLINT,
// 		@i_CheckType				TINYINT,
//  		@i_ReceivedMoveCounts		INT,
//  		@i_EngineItemNum			INT,	
// 		@i_ServerPastTime			INT,
// 		@i_ClientPastTime			INT,
// 		@i_PastDistance				INT,
//  		@i_CurrentSpeed				INT
// 	AS
// 		INSERT INTO atum_log_user_speed_hack
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex, @i_Position_X,
// 				@i_Position_Y, @i_Position_Z, @i_CheckType, @i_ReceivedMoveCounts, @i_EngineItemNum,
// 				@i_ServerPastTime, @i_ClientPastTime, @i_PastDistance, @i_CurrentSpeed)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pMsgSpeedHack->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pMsgSpeedHack->CurrentPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pMsgSpeedHack->CurrentPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pMsgSpeedHack->CurrentPosition.z, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_pMsgSpeedHack->CheckType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->ReceivedMoveCounts, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->EngineItemNum, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->ServerPastTime, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->ClientPastTime, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->PastDistance, 0, NULL);	
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgSpeedHack->CurrentSpeed, 0, NULL);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_user_speed_hack(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 
// 	return TRUE;
// }
// 
// 
// ///////////////////////////////////////////////////////////////////////////////
// /// \fn			BOOL CAtumDBLogWriter::InsertLog_Memory_Hack(CODBCStatement *i_pODBCStmt, MSG_FL_LOG_MEMORY_HACK *i_pMsgMemoryHack)
// /// \brief		
// /// \author		cmkwon
// /// \date		2005-12-20 ~ 2005-12-20
// /// \warning	
// ///
// /// \param		
// /// \return		
// ///////////////////////////////////////////////////////////////////////////////
// BOOL CAtumDBLogWriter::InsertLog_Memory_Hack(CODBCStatement *i_pODBCStmt, MSG_FL_LOG_MEMORY_HACK *i_pMsgMemoryHack)
// {
// 	BYTE logType = T1_FL_LOG_MEMORY_HACK;
// 
// 	/*[Stored Query Definition]************************************************
// 	--!!!!
// 	-- Name: atum_log_insert_user_memory_hack
// 	-- Desc: 2005-12-20 by cmkwon
// 	--			inserts log
// 	--====
// 
// 	CREATE PROCEDURE atum_log_insert_user_memory_hack
// 		@i_LogType					TINYINT,
// 		@i_CharacterUniqueNumber	INT,
// 		@i_MapIndex					SMALLINT,
// 		@i_Position_X				SMALLINT,
// 		@i_Position_Y				SMALLINT,
// 		@i_Position_Z				SMALLINT,
// 		@i_ItemKind					INT,
//  		@i_ItemNum					INT,
//  		@i_ItemUniqueNumber			BIGINT,
//  		@i_MemoryHackCheckType		INT,	
// 		@i_ValidValue				FLOAT,
// 		@i_CurrentValue				FLOAT,
// 		@i_Param1					INT
// 	AS
// 		INSERT INTO atum_log_user_memory_hack
// 		VALUES (@i_LogType, GetDate(), @i_CharacterUniqueNumber, @i_MapIndex, @i_Position_X,
// 				@i_Position_Y, @i_Position_Z, @i_ItemKind, @i_ItemNum, @i_ItemUniqueNumber,
// 				@i_MemoryHackCheckType, @i_ValidValue, @i_CurrentValue, @i_Param1)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_UID32, SQL_INTEGER, 0, 0, &i_pMsgMemoryHack->CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pMsgMemoryHack->CurrentMapIndex, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pMsgMemoryHack->CurrentPosition.x, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pMsgMemoryHack->CurrentPosition.y, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_SHORT, SQL_SMALLINT, 0, 0, &i_pMsgMemoryHack->CurrentPosition.z, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgMemoryHack->nItemKind, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgMemoryHack->nItemNum0, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &i_pMsgMemoryHack->uid64ItemUID, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgMemoryHack->nMemHackCheckType, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &i_pMsgMemoryHack->fValidValue, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &i_pMsgMemoryHack->fCurrentValue, 0, NULL);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pMsgMemoryHack->nParam1, 0, NULL);
// 	
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_user_memory_hack(?,?,?,?,?, ?,?,?,?,?, ?,?,?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 
// 	return TRUE;
// }
// 
// BOOL CAtumDBLogWriter::InsertLog_Monster_Boss(
// 								   CODBCStatement			*i_pODBCStmt,
// 								   MSG_FL_LOG_MONSTER_BOSS	*i_pMsgMonsterBoss,
// 								   const char				*i_szDropItemListString)
// {
// 	BYTE logType = T1_FL_LOG_MONSTER_BOSS;
// 	SQLINTEGER cb[19]; util::zero(cb, sizeof(SQLINTEGER)*19);
// 	char tmpCreatedTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
// 	char tmpDeadTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
// 
// 	/*[Stored Query Definition]************************************************
// 	CREATE PROCEDURE atum_log_insert_monster_boss
// 		@i_LogType					TINYINT,
// 		@i_MapIndex					SMALLINT,
// 		@i_ChannelIndex				SMALLINT,
//  		@i_MonsterUnitKind			INT,		-- 생성 몬스터 정보
//  		@i_CreatedTime				VARCHAR(30),
//  		@i_DeadTime					VARCHAR(30),
//  		@i_AttackUser1				VARCHAR(20),
//  		@i_AttackUser2				VARCHAR(20),
//  		@i_AttackUser3				VARCHAR(20),
//  		@i_AttackUser4				VARCHAR(20),
//  		@i_AttackUser5				VARCHAR(20),
//  		@i_AttackUser6				VARCHAR(20),
//  		@i_AttackUser7				VARCHAR(20),
//  		@i_AttackUser8				VARCHAR(20),
//  		@i_AttackUser9				VARCHAR(20),
//  		@i_AttackUser10				VARCHAR(20),
//  		@i_DropItemList				VARCHAR(200)
// 	AS
// 		INSERT INTO atum_log_monster_boss
// 		VALUES (@i_LogType, GetDate(), @i_MapIndex, @i_ChannelIndex,
// 				@i_MonsterUnitKind, @i_CreatedTime, @i_DeadTime, @i_AttackUser1, @i_AttackUser2,
// 				@i_AttackUser3, @i_AttackUser4, @i_AttackUser5, @i_AttackUser6, @i_AttackUser7,
// 				@i_AttackUser8, @i_AttackUser9, @i_AttackUser10, @i_DropItemList)
// 	GO
// 	**************************************************************************/
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &logType, 0, &cb[1]);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pMsgMonsterBoss->MapChannel.MapIndex, 0, &cb[2]);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_USHORT, SQL_SMALLINT, 0, 0, &i_pMsgMonsterBoss->MapChannel.ChannelIndex, 0, &cb[3]);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_pMsgMonsterBoss->MonsterUnitKind, 0, &cb[4]);
// 	cb[5] = SQL_NTS;
// 	i_pMsgMonsterBoss->CreatedTime.GetSQLDateTimeString(tmpCreatedTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpCreatedTimeBuf, 0, &cb[5]);
// 	cb[6] = SQL_NTS;
// 	i_pMsgMonsterBoss->DeadTime.GetSQLDateTimeString(tmpDeadTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpDeadTimeBuf, 0, &cb[6]);
// 	cb[7] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[0], 0, &cb[7]);
// 	cb[8] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[1], 0, &cb[8]);
// 	cb[9] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[2], 0, &cb[9]);
// 	cb[10] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[3], 0, &cb[10]);
// 	cb[11] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[4], 0, &cb[11]);
// 	cb[12] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[5], 0, &cb[12]);
// 	cb[13] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[6], 0, &cb[13]);
// 	cb[14] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[7], 0, &cb[14]);
// 	cb[15] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[8], 0, &cb[15]);
// 	cb[16] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pMsgMonsterBoss->AttackUser[9], 0, &cb[16]);
// 	cb[17] = SQL_NTS;
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 200, 0, (char*)i_szDropItemListString, 0, &cb[17]);
// 	
// 	cb[18] = SQL_NTS;
// 	INT nGameID		= 10061;
// #ifdef _ATUM_LOG_SERVER
// 		nGameID			= g_pLogGlobal->GetMGameServerID();
// #endif
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nGameID, 0, &cb[18]);
// 
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.atum_log_insert_monster_boss(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 
// 	return TRUE;
// }
// 
// 
// BOOL CAtumDBLogWriter::MGame_PB_Init(CODBCStatement			*i_pODBCStmt)
// {
// 	/*[Stored Query Definition]************************************************
// 	-- 접속자 체크 프로시져
// 	CREATE PROCEDURE PB_INIT
// 	@nGameID int		-- Game ID
// 
// 	AS
// 
// 	exec [pubaccount].[dbo].PB_INIT @nGameID
// 	GO
// 	**************************************************************************/
// 
// 	INT nGameID		= 10061;
// #ifdef _ATUM_LOG_SERVER
// 		nGameID			= g_pLogGlobal->GetMGameServerID();
// #endif	
// 	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nGameID, 0, NULL);
// 	BOOL bRet = i_pODBCStmt->ExecuteQuery("{call dbo.PB_INIT(?)}", TRUE);
// 	if (!bRet)
// 	{
// 		i_pODBCStmt->FreeStatement();
// 		return FALSE;
// 	}
// 
// 	i_pODBCStmt->FreeStatement();
// 
// 	return TRUE;
//}

BOOL CAtumDBLogWriter::InserLog_Live_Deleted_Character(
							CODBCStatement			*i_pODBCStmt,
							T1<T0::FL_LOG>			i_nLogType,
							SDELETED_CHARACTER_INFO	*i_pDeletedCharacterInfo)
{
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.Insert_atum_log_Live_Deleted_Character
		@i_LogType						TINYINT,
		@i_AccountUID					INT,
		@i_AccountName					VARCHAR(20),
		@i_CharacterUID					INT,
		@i_CharacterName				VARCHAR(20)
	AS
		INSERT INTO dbo.atum_log_Live_Deleted_Character
		VALUES (@i_LogType, GetDate(), @i_AccountUID, @i_AccountName, @i_CharacterUID, @i_CharacterName)
	GO
	**************************************************************************/
	SQLINTEGER arrCB[6] = {SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS};
	SQLBindParameter(i_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_nLogType, 0, &arrCB[1]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pDeletedCharacterInfo->AccountUID, 0, &arrCB[2]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pDeletedCharacterInfo->AccountName, 0, &arrCB[3]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pDeletedCharacterInfo->CharacterUID, 0, &arrCB[4]);
	SQLBindParameter(i_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_pDeletedCharacterInfo->CharacterName, 0, &arrCB[5]);

	BOOL bRet = i_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080822_0030), TRUE);
	if (!bRet)
	{
		i_pODBCStmt->FreeStatement();
		return FALSE;
	}

	i_pODBCStmt->FreeStatement();
	return TRUE;
}