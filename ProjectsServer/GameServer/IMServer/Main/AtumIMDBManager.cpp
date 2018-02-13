#include "stdafx.h"
#include "AtumIMDBManager.h"
#include "IMIOCPSocket.h"
#include "IMIOCP.h"
#include "md5_lib_src.h"
#include "AtumError.h"
#include "IMParty.h"
#include "IMGlobal.h"
#include "ANoticeManager.h"			// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 

CAtumIMDBManager::CAtumIMDBManager(CIMIOCP *pIMIOCP)
: CAtumDBManager(COUNT_IM_SERVER_DBWORKER)
{
	m_pIMIOCP5 = pIMIOCP;

	m_dwCountDBWorker	= 5;
}

BOOL CAtumIMDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog 보완
{
	switch(dbquery.enumQueryType)
	{
	case QT_AuthChatLogin:			QP_AuthChatLogin(dbquery, hstmt);			break;
	case QT_SaveLastPartyID:		QP_SaveLastPartyID(dbquery, hstmt);			break;
	case QT_GuildCreate:			QP_GuildCreate(dbquery, hstmt);				break;
	case QT_GuildAddMember:			QP_GuildAddMember(dbquery, hstmt);			break;
	case QT_GuildAddOffMember:		QP_GuildAddOffMember(dbquery, hstmt);		break;		// 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 오프유저 여단 가입
	case QT_GuildLeaveMember:		QP_GuildLeaveMember(dbquery, hstmt);		break;
	case QT_GuildBanMember:			QP_GuildBanMember(dbquery, hstmt);			break;
	case QT_GuildUpdateMemberCapacity:	QP_GuildUpdateMemberCapacity(dbquery, hstmt);	break;
	case QT_GuildLoadGuild:			QP_GuildLoadGuild(dbquery, hstmt);			break;
	case QT_GuildDismember:			QP_GuildDismember(dbquery, hstmt);			break;
	case QT_GuildCancelDismember:	QP_GuildCancelDismember(dbquery, hstmt);	break;
	case QT_GuildChangeGuildName:	QP_GuildChangeGuildName(dbquery, hstmt);	break;
	case QT_GuildSetGuildMark:		QP_GuildSetGuildMark(dbquery, hstmt);		break;
// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 사용하지 안는 함수임
//	case QT_GuildGetGuildMark:		QP_GuildGetGuildMark(dbquery, hstmt);		break;
	case QT_GuildSetRank:			QP_GuildSetRank(dbquery, hstmt);			break;
	case QT_GuildDeleteGuild:		QP_GuildDeleteGuild(dbquery, hstmt);		break;
	case QT_GuildSaveGuildWarPoint:	QP_GuildSaveGuildWarPoint(dbquery, hstmt);	break;
	case QT_GuildDeleteGuildUIDOfCharacter:	QP_GuildDeleteGuildUIDOfCharacter(dbquery, hstmt);	break;
	case QT_GuildAddGuildFame:		QP_GuildAddGuildFame(dbquery, hstmt);		break;
	case QT_GuildUpdateCommander:	QP_GuildUpdateCommander(dbquery, hstmt);	break;		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
	case QT_GuildNotice:			QP_GuildNotice(dbquery, hstmt);				break;		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
	case QT_GuildGetApplicant:		QP_GuildGetApplicant(dbquery, hstmt);		break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	case QT_GuildGetIntroduction:	QP_GuildGetIntroduction(dbquery, hstmt);	break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	case QT_GuildDeleteIntroduction:	QP_GuildDeleteIntroduction(dbquery, hstmt);	break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
	case QT_GetSelfIntroduction:		QP_GetSelfIntroduction(dbquery, hstmt);		break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서
	case QT_GuildSearchIntroduction:	QP_GuildSearchIntroduction(dbquery, hstmt);		break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
	case QT_GuildUpdateIntroduction:		QP_GuildUpdateIntroduction(dbquery, hstmt);			break;		// 2008-05-27 by dhjin,	EP3 - 여단 수정 사항 - 여단 소개 작성
	case QT_GuildUpdateSelfIntroduction:	QP_GuildUpdateSelfIntroduction(dbquery, hstmt);		break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
	case QT_GuildDeleteSelfIntroduction:	QP_GuildDeleteSelfIntroduction(dbquery, hstmt);		break;		// 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 
	case QT_GuildDeleteSelfIntroductionOffUser:	QP_GuildDeleteSelfIntroductionOffUser(dbquery, hstmt);		break;		// 2008-06-13 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 
	case QT_InsertNotifyMsg:		QP_InsertNotifyMsg(dbquery, hstmt);		break;		// 2008-06-13 by dhjin, EP3 - 여단 수정 사항 - 통지 메세지 보내기 

	case QT_FriendInsertFriend:		QP_FriendInsertFriend(dbquery, hstmt);		break;
	case QT_FriendDeleteFriend:		QP_FriendDeleteFriend(dbquery, hstmt);		break;
	case QT_FriendLoadFriends:		QP_FriendLoadFriends(dbquery, hstmt);		break;

	case QT_ReloadAdminAutoNotice:	QP_ReloadAdminAutoNotice(dbquery, hstmt);	break;		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 

	default:
		{
			// error: no such DB query type
			g_pGlobal->WriteSystemLogEX(TRUE, STRMSG_080904_0001, dbquery.enumQueryType, GetDBQueryTypeString(dbquery.enumQueryType));
			return FALSE;
		}
		break;
	}

	return TRUE;
}


// QT_AuthChatLogin
void CAtumIMDBManager::QP_AuthChatLogin(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_IC_CONNECT_LOGIN	*pRMsg = (MSG_IC_CONNECT_LOGIN*)q.pQueryParam;

	RETCODE ret;
	SDWORD pcbNTS1 = SQL_NTS, pcbNTS2 = SQL_NTS;
	SDWORD q1cb1 = SQL_NTS;

	/**********************************************************************
	-- !!!!
	-- Name:
	-- Desc:
	-- ====
	CREATE PROCEDURE dbo.atum_AuthChatLogin
	@acc			varchar(20),	-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 64를 수정
	@pass			varchar(35)
	AS
		SELECT AccountName
		FROM atum2_db_account.dbo.td_account
		WITH (NOLOCK)
		WHERE AccountName = @acc AND IsBlocked = 0 --AND Password LIKE @pass 
		-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 
		-- WHERE AccountName LIKE @acc AND IsBlocked = 0 --AND Password LIKE @pass 
	GO
	**********************************************************************/
// 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 아래와 같이 수정 함.
//	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
//	_strlwr(pRMsg->AccountName);
//	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER, 0,
//					GetSqlPattern(pRMsg->AccountName, spbuf), 0, &pcbNTS1);
	_strlwr(pRMsg->AccountName);
	SQLBindParameter(hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &pcbNTS1);

	SQLBindParameter(hstmt, 2,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0,
					pRMsg->Password, 0, &pcbNTS2);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0207, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "QP_AuthChatLogin Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	char accountName[SIZE_MAX_ACCOUNT_NAME];
	int nCharacterCount = 0;

	SQLBindCol(hstmt, 1, SQL_C_CHAR, accountName, SIZE_MAX_ACCOUNT_NAME, &q1cb1);

	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 인코딩되지 않은 경우를 대비
	if (ret == SQL_NO_DATA)
	{
		char szPasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];
		char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];
		BYTE byPass[SIZE_MAX_PASSWORD_MD5];

//		char pTmpSql[1024];
		_strlwr(pRMsg->AccountName);

		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//sprintf(pTmpSql, "SELECT Password \
		//					FROM td_Account WITH (NOLOCK)\
		//					WHERE AccountName LIKE '%s' AND IsBlocked = 0",
		//					GetSqlPattern(pRMsg->AccountName, spbuf));
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		sprintf(pTmpSql, QUERY_080702_0249,	GetSqlPattern(pRMsg->AccountName, spbuf));
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, NULL);
		ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0249), SQL_NTS);

		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO ) {
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "QP_AuthChatLogin Failed!\n", TRUE, q.pIOCPSocket);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}

		SQLBindCol(hstmt, 1, SQL_C_CHAR, szPasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &q1cb1);

		ret = SQLFetch(hstmt);
		SQLFreeStmt(hstmt, SQL_CLOSE);

		MD5 md5_instance;
		md5_instance.MD5Encode(szPasswordFromDB, byPass);
		MD5::MD5Binary2String(byPass, szEncodedString);

		if (strcmp(szEncodedString, pRMsg->Password) == 0)
		{
			ret = SQL_SUCCESS;
		}
		else
		{
			ret = SQL_NO_DATA;
		}
	}

	if (ret != SQL_NO_DATA)
	{ // 인증 성공

		CHARACTER retCharacter;
		CHARACTER_DB_EX retCharacterDBEX;
		memset(&retCharacter, 0x00, sizeof(CHARACTER));
		SQLINTEGER pcb1, pcb2;

		/**********************************************************************
		--!!!!
		-- Name:
		-- Desc:
		--		// 2007-07-31 by cmkwon, 인자 수정( <== @acc varchar(64), @charname varchar(64) ) - 쿼리를 like 사용을 하지 않는것으로 수정
		--====
		CREATE PROCEDURE dbo.atum_GetChatCharacterInfoByName
			@acc		varchar(20),
			@charname	varchar(20)
		AS
			-- // 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 캐릭터 정보를 가져올때 td_Guild 테이블을 참조하여 업데이트한 이후에 가져온다
			IF NOT EXISTS(SELECT c.UniqueNumber FROM td_character c, td_guild g, td_guildMember gm WITH(NOLOCK) WHERE c.charactername = @charname AND c.GuildUniqueNumber = g.GuildUniqueNumber AND gm.GuildUniqueNumber = c.GuildUniqueNumber AND gm.CharacterUniqueNumber = c.UniqueNumber)
			BEGIN
				UPDATE td_character
				SET GuildName = NULL, GuildUniqueNumber = 0
				FROM td_character c	WITH (NOLOCK)
				WHERE c.charactername = @charname
			END
			ELSE
			BEGIN
				UPDATE td_character
				SET GuildName = g.GuildName
				FROM td_character c, td_Guild g	WITH (NOLOCK)
				WHERE c.charactername = @charname AND c.GuildUniqueNumber = g.GuildUniqueNumber
			END

			SELECT *
			FROM td_Character
			WITH (NOLOCK)
			WHERE charactername = @charname AND accountname = @acc
		GO
		**********************************************************************/
// 2007-07-31 by cmkwon, 쿼리를 like 사용을 하지 않는것으로 수정을 위해 아래와 같이 수정
//		pcb1 = SQL_NTS;
//		_strlwr(pRMsg->AccountName);
//		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
//						SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern(pRMsg->AccountName, spbuf), 0, &pcb1);
//		pcb2 = SQL_NTS;
//		char spbuf2[SIZE_MAX_SQL_PATTERN_BUFFER];
//		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
//						SIZE_MAX_SQL_PATTERN_BUFFER, 0, GetSqlPattern(pRMsg->CharacterName, spbuf2), 0, &pcb2);
		pcb1 = pcb2 = SQL_NTS;
		_strlwr(pRMsg->AccountName);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0,		&pcb1);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pRMsg->CharacterName, 0,	&pcb2);

		ret = SQLExecDirect(hstmt, PROCEDURE_080822_0208, SQL_NTS);

		// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 비교 추가( && ret!=SQL_NO_DATA)
		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetChatCharacterInfoByName Failed!\n", TRUE, q.pIOCPSocket);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}

		SQLINTEGER	cb[CB_COUNT_CHARACTER];
		fill_n(cb, CB_COUNT_CHARACTER, SQL_NTS);

		// bind columns
		CAtumDBManager::BindColCharacter(hstmt, retCharacter, retCharacterDBEX, cb);

		// initialize to 0x00
		memset(&retCharacter, 0x00, sizeof(CHARACTER));
	
		///////////////////////////////////////////////////////////////////////////////
		// 2008-07-30 by cmkwon, MySQL 포팅 관련(Multi Result 처리 수정) - 
		SQLSMALLINT    nColCnts          = 0;
		while(TRUE)
		{
			SQLNumResultCols(hstmt, &nColCnts);
			if(nColCnts <= 0)
			{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
				ret = SQLMoreResults(hstmt);
				if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
				{
					break;
				}
				continue;
			}
			ret = SQLFetch(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
				if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
				{
					break;
				}
				continue;
			}

			retCharacter				= retCharacterDBEX;
			_strlwr(retCharacter.AccountName);
			retCharacter.TotalGearStat	= retCharacter.GearStat;

			// 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
#ifdef S_ITEM_VIEW_UNCONDITIONAL_ON
			retCharacter.SecretInfoOption = retCharacter.SecretInfoOption & USER_INFO_OPTION_ITEMINFO_DELETE_ALL_MASK;
#endif
			// end 2012-10-17 by jhseol, 아템 미리보기 무조건 ON 시키기
		}
		ret = SQLFreeStmt(hstmt, SQL_CLOSE);
	
// 2008-07-30 by cmkwon, MySQL 포팅 관련(Multi Result 처리 수정) - 아래와 같이 수정함.
// 		// 2007-07-30 by cmkwon, SCAdminTool에서 여단명 변경 기능 추가 - 아래와 같이 수정함, 에러 체크
// 		//if(ret == SQL_NO_DATA)
// 		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		if(FALSE == IS_VALID_UNIQUE_NUMBER(retCharacter.CharacterUniqueNumber))		// 2008-07-30 by cmkwon, MySQL 포팅 관련(Multi Result 처리 수정) - 
		{
			///////////////////////////////////////////////////////////////////////////
			// 캐릭터가 없음, Error 처리
			g_pIMGlobal->WriteSystemLogEX(TRUE, "[ERROR] No Such Character: \"%s\" \"%s\"\r\n", pRMsg->AccountName, pRMsg->CharacterName);

			((CIMIOCPSocket*)q.pIOCPSocket)->SendErrorMessage(T_IC_CONNECT_LOGIN, ERR_NO_SEARCH_CHARACTER);		// 2008-07-30 by cmkwon, MySQL 포팅 관련(Multi Result 처리 수정) - 

			util::del(pRMsg);
			return;
		}

		((CIMIOCPSocket*)q.pIOCPSocket)->ResAuthChatLogin(&retCharacter, pRMsg->LoginType, 0);		
	}
	else
	{
		// error 처리
		// 인증 실패 (아이디가 없거나, 패스워트가 틀림)
		q.pIOCPSocket->SendErrorMessage(T_IM_DB, ERR_COMMON_LOGIN_FAILED, 0, 0, NULL, TRUE);	// client에서 연결 끊기
		q.pIOCPSocket->Close(0x1202B, TRUE);
	}

	util::del(pRMsg);
}

// QT_SaveLastPartyID
void CAtumIMDBManager::QP_SaveLastPartyID(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_SAVELASTPARTYID *pQParam
		= (QPARAM_SAVELASTPARTYID*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_SaveLastPartyID
		@CharacterUniqueNumber	INT,
		@PartyID				BIGINT
	AS
	UPDATE td_character
		SET LastPartyID = @PartyID
		WHERE  UniqueNumber = @CharacterUniqueNumber;
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &pQParam->CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &pQParam->LastPartyID, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0209, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA) {
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SaveLastPartyID Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQParam);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pQParam);
}

// QT_GuildCreate
void CAtumIMDBManager::QP_GuildCreate(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_DB_GUILD_CREATE	*pDBGuildCreate = (MSG_DB_GUILD_CREATE*)q.pQueryParam;
	CIMIOCPSocket		*pSocket = (CIMIOCPSocket*)q.pIOCPSocket;
	UID32_t				GuildUniqueNumber;
	UINT				InitialGuildCapacity = SIZE_MAX_INITIAL_GUILD_CAPACITY;

	if(NULL == pSocket || FALSE == pSocket->IsUsing())
	{
		util::del(pDBGuildCreate);
		return;
	}

	RETCODE ret;
	SDWORD pcb1;

	///////////////////////////////////////////////////////////////////////////
	// 길드 이름 검색
	char TmpGuildName[SIZE_MAX_GUILD_NAME];
//	char sqlString[1024];
//	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(sqlString, "SELECT GuildName FROM td_Guild WITH (NOLOCK) WHERE GuildName LIKE \'%s\'", GetSqlPattern(pDBGuildCreate->szGuildName, spbuf));
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(sqlString, QUERY_080702_0250, GetSqlPattern(pDBGuildCreate->szGuildName, spbuf));
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pDBGuildCreate->szGuildName, 0, NULL);
	SQLBindCol(hstmt,  1, SQL_C_CHAR, TmpGuildName, SIZE_MAX_GUILD_NAME, NULL);
	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0250), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "Get GuildName @ QP_GuildCreate() Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBGuildCreate);
		return;
	}

	ret = SQLFetch(hstmt);
	if(ret != SQL_NO_DATA)
	{
		// 이미 존재하는 길드 이름임
		pSocket->SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0000, pDBGuildCreate->szGuildName);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBGuildCreate);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(sqlString, "SELECT GuildName FROM td_Guild WITH (NOLOCK) WHERE GuildCommanderUniqueNumber = %d", pDBGuildCreate->uidCommanderCharacter);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(sqlString, QUERY_080702_0251, pDBGuildCreate->uidCommanderCharacter);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pDBGuildCreate->uidCommanderCharacter, 0, NULL);
	SQLBindCol(hstmt,  1, SQL_C_CHAR, TmpGuildName, SIZE_MAX_GUILD_NAME, NULL);
	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0251), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "Get GuildName @ QP_GuildCreate() Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBGuildCreate);
		return;
	}
	ret = SQLFetch(hstmt);
	if(ret != SQL_NO_DATA)
	{
		// 이미 존재하는 길드 이름임
		pSocket->SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0001, pSocket->GetCharacter()->CharacterName);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBGuildCreate);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////
	// 길드 생성

	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name:
	-- Desc:
	--====	2005-12-27 by cmkwon, td_Guild에 필드 두개 추가한것 적용
	--		// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 필드 추가(GuildMarkState)
	--			#define GUILD_MARK_STATE_NONE					((BYTE)0)	// 마크 없는 상태
	--			#define GUILD_MARK_STATE_WAITING_PERMISSION		((BYTE)1)	// 마크 허용을 기다리는 상태, 게임상에서는 사용 불가
	--			#define GUILD_MARK_STATE_NORMAL					((BYTE)2)	// 마크 사용 중인 상태
	--DROP PROCEDURE atum_CreateGuild
	--GO
	CREATE PROCEDURE dbo.atum_CreateGuild
		@name			VARCHAR(30),
		@commander		INT,
		@capacity		INT
	AS
		-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항, // 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 필드 추가(GuildMarkState)
		INSERT INTO td_Guild VALUES (@name, @commander, @capacity, 0, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, NULL, 0, 0, 0)

		-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
		--RETURN SCOPE_IDENTITY();
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END

		SELECT SCOPE_IDENTITY();	-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
	GO
	**************************************************************************/

// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 아래와 같이 수정
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
// 	pcb1 = SQL_NTS;
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pDBGuildCreate->szGuildName, 0, &pcb1);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pDBGuildCreate->uidCommanderCharacter, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &InitialGuildCapacity, 0, NULL);
// 
// 	// 2008-07-08 by cmkwon, MySQL 지원으로 수정(프로시저에서 리턴처리 불가로 수정) - 
// 	//ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_CreateGuild(?, ?, ?)}", SQL_NTS);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_CreateGuild(?, ?, ?, ?)}", SQL_NTS);
// 
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
// 	{
// 		UCHAR szSqlState[MAXDBMSGBUFLEN];
// 		ret = GetDBError(hstmt, szSqlState);
// 
// 		if (strcmp((char*)szSqlState, "23000") == 0 )
// 		{
// 			pSocket->SendErrorMessage(T_IM_DB, ERR_PROTOCOL_GUILD_NAME_ALREADY_EXISTS);
// 		}
// 		else
// 		{
// 			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_CreateGuild Failed!\n", TRUE, q.pIOCPSocket);
// 		}
// 
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pDBGuildCreate);
// 
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 
// 	// Close Statement
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 
	pcb1 = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pDBGuildCreate->szGuildName, 0, &pcb1);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pDBGuildCreate->uidCommanderCharacter, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &InitialGuildCapacity, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0210, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		UCHAR szSqlState[MAXDBMSGBUFLEN];
		ret = GetDBError(hstmt, szSqlState);

		if (strcmp((char*)szSqlState, "23000") == 0 )
		{
			pSocket->SendErrorMessage(T_IM_DB, ERR_PROTOCOL_GUILD_NAME_ALREADY_EXISTS);
		}
		else
		{
			pSocket->SendErrorMessage(T_IM_DB, ERR_DB_EXECUTION_FAILED);
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_CreateGuild Failed!\n", TRUE, q.pIOCPSocket);
		}

		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBGuildCreate);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &(GuildUniqueNumber), 0, NULL);	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	if(FALSE == IS_VALID_UNIQUE_NUMBER(GuildUniqueNumber))
	{// 2008-07-24 by cmkwon, 
		if(pSocket->IsValidCharacter())
		{
			pSocket->SendErrorMessage(T_IM_DB, ERR_DB_EXECUTION_FAILED);
		}
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[ERROR] atum_CreateGuild@ error !! %s GuildName(%s)\r\n"
			, GetCharacterString(pSocket->GetCharacter(), string()), pDBGuildCreate->szGuildName);
		util::del(pDBGuildCreate);
		return;
	}

	if(m_pIMIOCP5->CheckInfluenceSubLeader(pSocket->m_character.InfluenceType, pSocket->m_character.CharacterUniqueNumber))
	{// 2007-10-06 by dhjin, 여단장이 부지도자인지 체크한다. 맞다면 FieldServer에 InfluenceWar 클래스에 부지도자 여단을 추가한다.
		pSocket->SendFieldCreateGuildBySubLeader();
	}

	// IOCP에 추가
	CGuild *pNewGuild = new CGuild;
	pNewGuild->m_CommanderUniqueNumber	= pSocket->m_character.CharacterUniqueNumber;
	util::strncpy(pNewGuild->m_GuildName, pDBGuildCreate->szGuildName, SIZE_MAX_GUILD_NAME);
	pNewGuild->m_GuildUniqueNumber		= GuildUniqueNumber;
	pNewGuild->m_nGuildMemberCapacity	= InitialGuildCapacity;
	pNewGuild->m_GuildState				= GUILD_STATE_NORMAL;
	pNewGuild->m_DismemberDate.Reset();
	memset(&pNewGuild->m_GuildMark, 0, sizeof(GUILD_MARK));
	pNewGuild->m_GuildMark.byGuildMarkState		= GUILD_MARK_STATE_NONE;	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 초기값 설정
	pNewGuild->m_nMonthlyFameRank		= 0;			// 2008-06-11 by dhjin, EP3 - 여단 수정 사항
	pNewGuild->m_nTotalFameRank			= 0;			// 2008-06-11 by dhjin, EP3 - 여단 수정 사항

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Get_Guild_Master_CashStoreInfo
	-- DESC				: 여단장 캐쉬 정보 얻어오기
	-- // 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Get_Guild_Master_CashStoreInfo
		@GuildMasterUID			INT	
	AS
		SELECT A.ExpireTime
			FROM td_Character AS C WITH (NOLOCK) INNER JOIN td_AccountCashStore AS A
				ON C.AccountUniqueNumber = A.AccountUniquenumber
			WHERE C.UniqueNumber = @GuildMasterUID	
	GO
	**************************************************************************/
	SQLINTEGER arrCB[1] = {SQL_NTS};
	SQL_TIMESTAMP_STRUCT	GuildMemberShipExpireTime; memset(&GuildMemberShipExpireTime, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pNewGuild->m_CommanderUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0211, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_Get_Guild_Master_CashStoreInfo Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLBindCol(hstmt,  1, SQL_C_TIMESTAMP, &GuildMemberShipExpireTime, 0,		&arrCB[0]);
	
	if(SQL_NO_DATA != SQLFetch(hstmt))
	{
		ATUM_DATE_TIME tmGuildMemberShipExpireTime;
		tmGuildMemberShipExpireTime = GuildMemberShipExpireTime;
		pNewGuild->SetGuildMemberShip(TRUE, &tmGuildMemberShipExpireTime);
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	
	///////////////////////////////////////////////////////////////////////////
	// 길드원 삽입
	int i;
	for(i=0; i < pDBGuildCreate->nMemberCounts; i++)	
	{
		/*[Stored Query Definition]************************************************
		CREATE PROCEDURE atum_InsertGuildMember
			@guildUniqueNum		INT,	-- UINT
			@characUniqueNum	INT,	-- UINT
			@guildRank			TINYINT
		AS
			-- 길드원 삽입
			INSERT INTO td_GuildMember VALUES(@guildUniqueNum, @characUniqueNum, @guildRank, GetDate())
			
			DECLARE @GuildName VARCHAR(30)
			SET @GuildName = (SELECT GuildName FROM td_Guild WHERE GuildUniqueNumber = @guildUniqueNum)

			-- 캐릭터의 길드 정보 업데이트
			UPDATE td_Character
			SET GuildName = @guildName, GuildUniqueNumber = @guildUniqueNum
			WHERE UniqueNumber = @characUniqueNum

		GO
		**************************************************************************/
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pDBGuildCreate->arrMemberCharacterUID[i], 0, NULL);
		pDBGuildCreate->arrGuildRank[i]
			= ((pDBGuildCreate->arrMemberCharacterUID[i] == pDBGuildCreate->uidCommanderCharacter)?GUILD_RANK_COMMANDER:GUILD_RANK_PRIVATE_NULL);
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pDBGuildCreate->arrGuildRank[i], 0, NULL);

		ret = SQLExecDirect(hstmt, PROCEDURE_080822_0212, SQL_NTS);

		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
		{
			UCHAR szSqlState[MAXDBMSGBUFLEN];
			ret = GetDBError(hstmt, szSqlState);

			if (strcmp((char*)szSqlState, "23000") == 0 )
			{
				pSocket->SendErrorMessage(T_IM_DB, ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS);
			}
			else
			{
				ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_InsertGuildMember Failed!\n", TRUE, q.pIOCPSocket);
			}

			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pDBGuildCreate);
			return;
		}

		SQLFreeStmt(hstmt, SQL_CLOSE);
	}

	if(FALSE == pSocket->IsUsing())
	{
		// 2007-08-02 by cmkwon, 필요 없음
		//SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBGuildCreate);
		return;
	}

	m_pIMIOCP5->m_MapGuild.lock();
	{
		// 각 멤버들에게 길드 포인터 할당
		for(i = 0; i < pDBGuildCreate->nMemberCounts; i++)
		{
			INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_RANK, T_FI_GUILD_RANK_OK, msgGuildRank, msgGuildRankBuf);
			CIMIOCPSocket *pMemberSocket = m_pIMIOCP5->GetIMIOCPSocketByCharacterUID(pDBGuildCreate->arrMemberCharacterUID[i]);
			if(pMemberSocket && pMemberSocket->IsUsing())
			{
				pMemberSocket->SetGuildUniqueNumber(GuildUniqueNumber);
				
				// Field Server에 알림
				pMemberSocket->SendGuildInfoToFieldServer(pNewGuild->m_GuildUniqueNumber, pNewGuild->m_GuildName);
				
				// 길드 구조체에 멤버 삽입
				CGuildMember tmpGuildMember(pMemberSocket->m_character.CharacterName,
					pMemberSocket->m_character.CharacterUniqueNumber,
					pMemberSocket,
					pNewGuild->m_GuildUniqueNumber, pDBGuildCreate->arrGuildRank[i], TRUE,
					pMemberSocket->m_character.UnitKind, pMemberSocket->m_character.Level, pMemberSocket->m_VoipType);	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
				pNewGuild->AddGuildMember(tmpGuildMember);

				//////////////////////////////////////////////////////////////////////////
				// 2006-09-29 by dhjin, 랭크 정보 길드원에게 보내기 
				msgGuildRank->charUID	= pMemberSocket->m_character.CharacterUniqueNumber;
				msgGuildRank->GuildRank	= pDBGuildCreate->arrGuildRank[i];
				pMemberSocket->SendMsgToCurrentFieldServerSocket(msgGuildRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));

			}
		}	

		///////////////////////////////////////////////////////////////////////////////
		// 길드맵리스트에 삽입
		m_pIMIOCP5->m_MapGuild.insertLock(GuildUniqueNumber, pNewGuild);

		// Client에 CREATE_OK 전송
		INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CREATE_OK, T_IC_GUILD_CREATE_OK, msgCreateOK, msgCreateOKBuf);
		util::strncpy(msgCreateOK->GuildName, pDBGuildCreate->szGuildName, SIZE_MAX_GUILD_NAME);
		msgCreateOK->GuildUniqueNumber = GuildUniqueNumber;
		msgCreateOK->GuildCommanderUniqueNumber = pSocket->m_character.CharacterUniqueNumber;

		pNewGuild->SendMsgToGuildMembers(msgCreateOKBuf, MSG_SIZE(MSG_IC_GUILD_CREATE_OK));
		
		// 2006-09-28 by dhjin, I-->F 길드 생성 시 여단장 정보 보내기 맴버쉽서비스 로그아웃 없이 사용
//		m_pIMIOCP5->SendMessageToFieldServer(msgCreateOKBuf, MSG_SIZE(MSG_IC_GUILD_CREATE_OK));
	}
	m_pIMIOCP5->m_MapGuild.unlock();
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteSelfIntroduction
	-- DESC				: 자기 소개서 삭제
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteSelfIntroduction
		@i_CharacterUID					INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE CharacterUID = @i_CharacterUID
	GO
	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSocket->m_character.CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0213, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteSelfIntroduction Failed (1) !\r\n", TRUE, q.pIOCPSocket);
		util::del(pDBGuildCreate);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 2007-08-02 by cmkwon, 필요 없음
	//SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pDBGuildCreate);
}

// QT_GuildAddMember
void CAtumIMDBManager::QP_GuildAddMember(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_IC_GUILD_ACCEPT_INVITE	*pMsgAccept = (MSG_IC_GUILD_ACCEPT_INVITE*)q.pQueryParam;
	CIMIOCPSocket				*pSocket = (CIMIOCPSocket*)q.pIOCPSocket;
	BYTE						tmpGuildRank = GUILD_RANK_PRIVATE_NULL;

	if(NULL == pSocket || FALSE == pSocket->IsValidCharacter())
	{
		util::del(pMsgAccept);
		return;
	}

	RETCODE ret;
	SDWORD pcb1;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_InsertGuildMember
		@guildUniqueNum		INT,	-- UINT
		@characUniqueNum	INT,	-- UINT
		@guildRank			TINYINT
	AS
		-- 길드원 삽입
		INSERT INTO td_GuildMember VALUES(@guildUniqueNum, @characUniqueNum, @guildRank, GetDate())
		
		DECLARE @GuildName VARCHAR(30)
		SET @GuildName = (SELECT GuildName FROM td_Guild WHERE GuildUniqueNumber = @guildUniqueNum)

		-- 캐릭터의 길드 정보 업데이트
		UPDATE td_Character
		SET GuildName = @guildName, GuildUniqueNumber = @guildUniqueNum
		WHERE UniqueNumber = @characUniqueNum

	GO
	**************************************************************************/
	pcb1 = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsgAccept->GuildUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSocket->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmpGuildRank, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0212, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		UCHAR szSqlState[MAXDBMSGBUFLEN];
		ret = GetDBError(hstmt, szSqlState);

		if (strcmp((char*)szSqlState, "23000") == 0 )
		{
			pSocket->SendErrorMessage(T_IM_DB, ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS);
		}
		else
		{
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteGuildMember Failed!\n", TRUE, q.pIOCPSocket);
		}

		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsgAccept);
		return;
	}
	
	if(pSocket->IsValidCharacter())
	{
		m_pIMIOCP5->m_MapGuild.lock();
		{
			CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(pMsgAccept->GuildUniqueNumber);
			if(ptmGuild)
			{
				// 각 멤버들에게 길드 포인터 할당
				pSocket->SetGuildUniqueNumber(pMsgAccept->GuildUniqueNumber);
				
				// Field Server에 알림
				pSocket->SendGuildInfoToFieldServer(ptmGuild->m_GuildUniqueNumber, ptmGuild->m_GuildName);
				
				// 길드 구조체에 멤버 삽입
				CGuildMember tmpGuildMember(pSocket->m_character.CharacterName,
					pSocket->m_character.CharacterUniqueNumber,
					pSocket,
					ptmGuild->m_GuildUniqueNumber, tmpGuildRank, TRUE,
					pSocket->m_character.UnitKind, pSocket->m_character.Level, pSocket->m_VoipType);		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
				ptmGuild->AddGuildMember(tmpGuildMember);

				///////////////////////////////////////////////////////////////////////////////				
				// 2007-07-31 by cmkwon, 여단 가입 멤버에게 여단정보 전송 - 클라이언트 요청은 주석처리함
				ptmGuild->Send_MSG_IC_GUILD_GET_GUILD_INFO_OK(pSocket);
				
				const CHARACTER *pTmpCharacter = pSocket->GetCharacter();
				
				// Client에 ACCEPT_INVITE_OK 생성
				INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_ACCEPT_INVITE_OK, T_IC_GUILD_ACCEPT_INVITE_OK, msgAccepOK, msgAccepOKBuf);
				msgAccepOK->MemberInfo = tmpGuildMember;
				ptmGuild->SendMsgToGuildMembers(msgAccepOKBuf, MSG_SIZE(MSG_IC_GUILD_ACCEPT_INVITE_OK));
			}
		}
		m_pIMIOCP5->m_MapGuild.unlock();		
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteSelfIntroduction
	-- DESC				: 자기 소개서 삭제
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteSelfIntroduction
		@i_CharacterUID					INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE CharacterUID = @i_CharacterUID
	GO
	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSocket->m_character.CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0213, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteSelfIntroduction Failed (2) !\r\n", TRUE, q.pIOCPSocket);
		util::del(pMsgAccept);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsgAccept);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildAddOffMember(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 오프유저 여단 가입
/// \author		dhjin
/// \date		2008-06-12 ~ 2008-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildAddOffMember(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildAddOffMember
	MSG_IC_GUILD_APPLICANT_INVITE	*pMsg = (MSG_IC_GUILD_APPLICANT_INVITE*)q.pQueryParam;
	CIMIOCPSocket					*pSocket = (CIMIOCPSocket*)q.pIOCPSocket;
	BYTE						tmpGuildRank = GUILD_RANK_PRIVATE_NULL;

	if(NULL == pSocket || FALSE == pSocket->IsValidCharacter())
	{
		return;
	}

	RETCODE ret;
	SDWORD pcb1;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_InsertGuildMember
		@guildUniqueNum		INT,	-- UINT
		@characUniqueNum	INT,	-- UINT
		@guildRank			TINYINT
	AS
		-- 길드원 삽입
		INSERT INTO td_GuildMember VALUES(@guildUniqueNum, @characUniqueNum, @guildRank, GetDate())
		
		DECLARE @GuildName VARCHAR(30)
		SET @GuildName = (SELECT GuildName FROM td_Guild WHERE GuildUniqueNumber = @guildUniqueNum)

		-- 캐릭터의 길드 정보 업데이트
		UPDATE td_Character
		SET GuildName = @guildName, GuildUniqueNumber = @guildUniqueNum
		WHERE UniqueNumber = @characUniqueNum

	GO
	**************************************************************************/
	pcb1 = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSocket->m_character.GuildUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUID, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmpGuildRank, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0212, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		UCHAR szSqlState[MAXDBMSGBUFLEN];
		ret = GetDBError(hstmt, szSqlState);

		if (strcmp((char*)szSqlState, "23000") == 0 )
		{
			pSocket->SendErrorMessage(T_IM_DB, ERR_PROTOCOL_GUILD_MEMBER_ALREADY_EXISTS);
		}
		else
		{
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteGuildMember Failed!\n", TRUE, q.pIOCPSocket);
		}

		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pMsg);
		return;
	}
	
	if(pSocket->IsValidCharacter())
	{
		m_pIMIOCP5->m_MapGuild.lock();
		{
			CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(pSocket->m_character.GuildUniqueNumber);
			if(ptmGuild)
			{
//				// 각 멤버들에게 길드 포인터 할당
//				pSocket->SetGuildUniqueNumber(pSocket->m_character.GuildUniqueNumber);
//				
//				// Field Server에 알림
//				pSocket->SendGuildInfoToFieldServer(ptmGuild->m_GuildUniqueNumber, ptmGuild->m_GuildName);
				
				// 길드 구조체에 멤버 삽입
				CGuildMember tmpGuildMember(pMsg->CharacterName,
					pMsg->CharacterUID,
					NULL,
					ptmGuild->m_GuildUniqueNumber, tmpGuildRank, FALSE,
					pMsg->UnitKind, pMsg->Level, CHECK_TYPE_VOIP_NONE);		// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
				ptmGuild->AddGuildMember(tmpGuildMember);

				///////////////////////////////////////////////////////////////////////////////				
				// 2007-07-31 by cmkwon, 여단 가입 멤버에게 여단정보 전송 - 클라이언트 요청은 주석처리함
//				ptmGuild->Send_MSG_IC_GUILD_GET_GUILD_INFO_OK(pSocket);
				
				// Client에 ACCEPT_INVITE_OK 생성
				INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_ACCEPT_INVITE_OK, T_IC_GUILD_ACCEPT_INVITE_OK, msgAccepOK, msgAccepOKBuf);
				msgAccepOK->MemberInfo = tmpGuildMember;
				ptmGuild->SendMsgToGuildMembers(msgAccepOKBuf, MSG_SIZE(MSG_IC_GUILD_ACCEPT_INVITE_OK));
			}
		}
		m_pIMIOCP5->m_MapGuild.unlock();		
	}
	
	SQLFreeStmt(hstmt, SQL_CLOSE);

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteSelfIntroduction
	-- DESC				: 자기 소개서 삭제
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteSelfIntroduction
		@i_CharacterUID					INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE CharacterUID = @i_CharacterUID
	GO
	**********************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pMsg->CharacterUID, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0213, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteSelfIntroduction Failed (3) !\r\n", TRUE, NULL);
		util::del(pMsg);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	
	
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pMsg);
}

// QT_GuildLeaveMember
void CAtumIMDBManager::QP_GuildLeaveMember(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t	CharacterUniqueNumberDel = (UID32_t)q.nGeneralParam1;
	UID32_t	GuildUniqueNumber = (UID32_t)q.nGeneralParam2;

	RETCODE ret;
	SDWORD pcb1;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_DeleteGuildMember
		@characUniqueNum	INT		-- UINT
	AS
		-- 길드원에서 제거
		DELETE td_guildMember WHERE characterUniqueNumber = @characUniqueNum

		-- 캐릭터의 길드 정보 제거
		UPDATE td_character
			SET GuildName = NULL, GuildUniqueNumber = 0
			WHERE uniquenumber = @characUniqueNum
	GO
	**************************************************************************/
	pcb1 = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (UID32_t*)&CharacterUniqueNumberDel, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0218, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteGuildMember@QP_GuildLeaveMember() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);
		if(ptmGuild)
		{
			// LEAVE_OK 생성 및 전송			
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_LEAVE_OK, T_IC_GUILD_LEAVE_OK, msgLeaveOK, SendBuf);
			msgLeaveOK->MemberUniqueNumber = CharacterUniqueNumberDel;
			
			ptmGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_LEAVE_OK));			
			ptmGuild->DeleteGuildMember(CharacterUniqueNumberDel);
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();	

	CIMIOCPSocket *pCharacterSocket = m_pIMIOCP5->GetIMIOCPSocketByCharacterUID(CharacterUniqueNumberDel);
	if (pCharacterSocket
		&& pCharacterSocket->IsUsing())
	{		
		pCharacterSocket->SetGuildUniqueNumber(INVALID_GUILD_UID);				// 길드 포인터 삭제		
		pCharacterSocket->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);	// Field Server에 알림
	}
}

// QT_GuildBanMember
void CAtumIMDBManager::QP_GuildBanMember(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t			CharacterUniqueNumberDel = (UID32_t)q.nGeneralParam1;
	UID32_t			tmUIDGuild = (UID32_t)q.nGeneralParam2;
	CIMIOCPSocket	*pBanSocket = m_pIMIOCP5->GetIMIOCPSocketByCharacterUID(CharacterUniqueNumberDel);
	CIMIOCPSocket	*pCommanderSocket = (CIMIOCPSocket*)q.pIOCPSocket;

	if(NULL == pCommanderSocket || FALSE == pCommanderSocket->IsUsing())
	{
		return;
	}

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_DeleteGuildMember
		@characUniqueNum	INT		-- UINT
	AS
		-- 길드원에서 제거
		DELETE td_guildMember WHERE characterUniqueNumber = @characUniqueNum

		-- 캐릭터의 길드 정보 제거
		UPDATE td_character
			SET GuildName = NULL, GuildUniqueNumber = 0
			WHERE uniquenumber = @characUniqueNum
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, (UID32_t*)&CharacterUniqueNumberDel, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0218, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteGuildMember@QP_GuildBanMember() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(tmUIDGuild);
		if(ptmGuild)
		{
			// MSG_IC_GUILD_BAN_MEMBER_OK 생성 및 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_BAN_MEMBER_OK, T_IC_GUILD_BAN_MEMBER_OK, msgLeaveOK, SendBuf);
			msgLeaveOK->MemberUniqueNumber = CharacterUniqueNumberDel;

			ptmGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_BAN_MEMBER_OK));

			// 길드 구조체에서 멤버 삭제
			ptmGuild->DeleteGuildMember(CharacterUniqueNumberDel);
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();

	// 길드 포인터 삭제
	if (pBanSocket && pBanSocket->IsUsing())
	{
		pBanSocket->SetGuildUniqueNumber(INVALID_GUILD_UID);		
		pBanSocket->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);	// Field Server에 알림
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_GuildDismember
void CAtumIMDBManager::QP_GuildDismember(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t GuildUniqueNumber = (UID32_t)q.nGeneralParam1;

	if(NULL == q.pIOCPSocket || FALSE == q.pIOCPSocket->IsUsing())
	{
		return;
	}

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_SetDismemberDate
		@guildUniqueNum		INT		-- UINT
	AS
		DECLARE @dismemberDate DATETIME
		SET @dismemberDate = GetDate()
		SET @dismemberDate = dateadd(day, 7, @dismemberDate)

		UPDATE td_Guild
		SET GuildDismemberDate = @dismemberDate , GuildState = 1
		WHERE GuildUniqueNumber = @guildUniqueNum

		-- // 2008-06-13 by dhjin, EP3 - 여단 수정 사항
		DELETE FROM dbo.td_SelfIntroduction WHERE GuildUID = @guildUniqueNum
		DELETE FROM dbo.td_GuildIntroduction WHERE GuildUID = @guildUniqueNum
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0220, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SetDismemberDate @ QP_GuildDismember() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	// 날짜 가지고 오기
	SQL_TIMESTAMP_STRUCT GuildDismemberDate; memset(&GuildDismemberDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
//	char sqlString[256];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(sqlString, "SELECT GuildDismemberDate FROM td_Guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d", GuildUniqueNumber);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(sqlString, QUERY_080702_0252, GuildUniqueNumber);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
	SQLBindCol(hstmt,  1, SQL_C_TIMESTAMP, &GuildDismemberDate, 0, NULL);
	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0252), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "Get DismemberDate @ QP_GuildDismember() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	ret = SQLFetch(hstmt);
	if(ret == SQL_NO_DATA)
	{
		// fatal error
		SQLFreeStmt(hstmt, SQL_CLOSE);
// 2005-04-25 by cmkwon
//		ASSERT_NEVER_GET_HERE();
		return;
	}

	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);
		if(ptmGuild)
		{
			ptmGuild->m_GuildState = GUILD_STATE_DISMEMBER_READY;
			ptmGuild->m_DismemberDate = GuildDismemberDate;
			
			// 2007-11-09 by dhjin, 여단 삭제 요청을 필드 서버로 전송
			INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_REG_DELETE_GUILD, T_FI_GUILD_REG_DELETE_GUILD, pSeMsg, FIBuff);
			pSeMsg->DeleteGuildUID		= GuildUniqueNumber;
			m_pIMIOCP5->SendMessageToFieldServer(FIBuff, MSG_SIZE(MSG_FI_GUILD_REG_DELETE_GUILD));

			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_GUILD_STATE, T_IC_GUILD_CHANGE_GUILD_STATE, msgChangeState, msgChangeStateBuf);
			msgChangeState->GuildState = GUILD_STATE_DISMEMBER_READY;
			ptmGuild->SendMsgToGuildMembers(msgChangeStateBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_GUILD_STATE));
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;
}

// QT_GuildUpdateMemberCapacity
void CAtumIMDBManager::QP_GuildUpdateMemberCapacity(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t	CashPrice				= (UID32_t)q.pGeneralParam;
	UID32_t GuildUniqueNumber		= (UID32_t)q.nGeneralParam1;
	int		nGuildMemberCapacity	= (int)q.nGeneralParam2;
	/*[Stored Query Definition]************************************************
	--!!!!
	-- Name: atum_Guild_UpdateMemberCapacity
	-- Desc: 길드 제한 인원 수정
	--====
	--DROP PROCEDURE atum_Guild_UpdateMemberCapacity
	--GO
	CREATE PROCEDURE atum_Guild_UpdateMemberCapacity
		@guildUniqueNum				INT,		-- UINT
		@MemberCapacity				INT,		--
		@GuildMemberCardCashPrice	INT
	AS
		UPDATE td_Guild
			SET GuildMemberCapacity = @MemberCapacity, GuildMemberCardCashPrice = GuildMemberCardCashPrice + @GuildMemberCardCashPrice
			WHERE GuildUniqueNumber = @guildUniqueNum
	GO
	**************************************************************************/
	RETCODE		ret;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nGuildMemberCapacity, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CashPrice, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0221, SQL_NTS);
	if (ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_Guild_UpdateMemberCapacity() Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_GuildCancelDismember
void CAtumIMDBManager::QP_GuildCancelDismember(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t GuildUniqueNumber = (UID32_t)q.nGeneralParam1;
	
	if(NULL == q.pIOCPSocket || FALSE == q.pIOCPSocket->IsUsing())
	{
		return;
	}

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_CancelDismember
		@guildUniqueNum		INT		-- UINT
	AS
		UPDATE td_Guild
		SET GuildDismemberDate = NULL, GuildState = 0
		WHERE GuildUniqueNumber = @guildUniqueNum
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0222, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_CancelDismember @ QP_GuildCancelDismember() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);
		if(ptmGuild)
		{
			ptmGuild->m_GuildState = GUILD_STATE_NORMAL;
			ptmGuild->m_DismemberDate.Reset();

			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CANCEL_DISMEMBER_OK, T_IC_GUILD_CANCEL_DISMEMBER_OK, msgCancel, msgCancelBuf);
			msgCancel->GuildState = GUILD_STATE_NORMAL;
			ptmGuild->SendMsgToGuildMembers(msgCancelBuf, MSG_SIZE(MSG_IC_GUILD_CANCEL_DISMEMBER_OK));
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;
}

// QT_GuildChangeGuildName
void CAtumIMDBManager::QP_GuildChangeGuildName(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_CHANGE_GUILD_NAME *pChangeGuildName = (QPARAM_CHANGE_GUILD_NAME*)q.pQueryParam;
	CIMIOCPSocket	*pSocket = (CIMIOCPSocket*)q.pIOCPSocket;
	UID32_t			uidGuild = (UID32_t)q.nGeneralParam2;
	
	if(NULL == pSocket || FALSE == pSocket->IsUsing())
	{
		util::del(pChangeGuildName);
		return;
	}

	RETCODE ret;
	///////////////////////////////////////////////////////////////////////////
	// 길드 이름 검색
	char TmpGuildName[SIZE_MAX_GUILD_NAME];
//	char sqlString[256];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(sqlString, "SELECT GuildName FROM td_Guild WITH (NOLOCK) WHERE GuildName LIKE \'%s\'", pChangeGuildName->NewGuildName);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(sqlString, QUERY_080702_0253, pChangeGuildName->NewGuildName);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pChangeGuildName->NewGuildName, 0, NULL);	
	SQLBindCol(hstmt,  1, SQL_C_CHAR, TmpGuildName, SIZE_MAX_GUILD_NAME, NULL);
	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0253), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "Get GuildName @ QP_GuildChangeGuildName() Failed!\n", TRUE, q.pIOCPSocket);

		util::del(pChangeGuildName);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	ret = SQLFetch(hstmt);
	if(ret != SQL_NO_DATA)
	{
		// 이미 존재하는 길드 이름임
		pSocket->SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0002, pChangeGuildName->NewGuildName);

		util::del(pChangeGuildName);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////
	// 길드 이름 변경

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_ChangeGuildName
		@guildUniqueNumber		INT,	-- UINT
		@guildName				VARCHAR(30)
	AS
		UPDATE td_guild
		SET GuildName = @guildName
		WHERE GuildUniqueNumber = @guildUniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pChangeGuildName->GuildUniqueNumber, 0, NULL);
	SQLINTEGER pcb1 = SQL_NTS;
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, pChangeGuildName->NewGuildName, 0, &pcb1);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0223, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_ChangeGuildName @ QP_GuildChangeGuildName() Failed!\n", TRUE, q.pIOCPSocket);

		util::del(pChangeGuildName);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(uidGuild);	
		if(ptmGuild)
		{
			// GuildName 설정함
			util::strncpy(ptmGuild->m_GuildName, pChangeGuildName->NewGuildName, SIZE_MAX_GUILD_NAME);

			// MSG 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK, T_IC_GUILD_CHANGE_GUILD_NAME_OK, msgChangeName, msgChangeNameBuf);
			util::strncpy(msgChangeName->NewGuildName, pChangeGuildName->NewGuildName, SIZE_MAX_GUILD_NAME);
			ptmGuild->SendMsgToGuildMembers(msgChangeNameBuf, MSG_SIZE(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK));
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();

	util::del(pChangeGuildName);
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

// QT_GuildSetGuildMark
void CAtumIMDBManager::QP_GuildSetGuildMark(DB_QUERY q, SQLHSTMT hstmt)
{
// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 아래와 같이 수정한다
//	UID32_t	GuildUniqueNumber = (UID32_t)q.nGeneralParam1;
//	INT		GuildMarkImageSize = (INT)q.nGeneralParam2;
//	INT		GuildMarkVersion = 0;
//	
//	if(NULL == q.pIOCPSocket || FALSE == q.pIOCPSocket->IsUsing())
//	{
//		return;
//	}
//	
//
//	RETCODE ret;
//
//	char	newGuildMarkImage[SIZE_MAX_GUILD_MARK_IMAGE];
//	m_pIMIOCP5->m_MapGuild.lock();
//	{
//		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);	
//		if(NULL == ptmGuild)
//		{
//			m_pIMIOCP5->m_MapGuild.unlock();
//			return;
//		}
//
//		memcpy(newGuildMarkImage, ptmGuild->m_GuildMark.GuildMarkImage, SIZE_MAX_GUILD_MARK_IMAGE);
//	}
//	m_pIMIOCP5->m_MapGuild.unlock();
//
//	
//	/*[Stored Query Definition]************************************************
//	CREATE PROCEDURE atum_SetGuildMark
//		@guildUniqueNumber		INT,	-- UINT
//		@guildMarkSize			INT,
//		@guildMarkImage			BINARY(1600)
//	AS
//		DECLARE @GuildMarkVersion INT
//		SET @GuildMarkVersion = (SELECT GuildMarkVersion FROM td_Guild WHERE GuildUniqueNumber = @guildUniqueNumber) + 1
//
//		UPDATE td_guild
//		SET GuildMarkVersion = @GuildMarkVersion, GuildMarkSize = @guildMarkSize, GuildMarkImage = @guildMarkImage
//		WHERE GuildUniqueNumber = @guildUniqueNumber
//		
//		RETURN @GuildMarkVersion
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &GuildMarkVersion, 0, NULL);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &GuildMarkImageSize, 0, NULL);
//	SQLINTEGER pcb = SIZE_MAX_GUILD_MARK_IMAGE;
//	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, SIZE_MAX_GUILD_MARK_IMAGE, 0, newGuildMarkImage, 0, &pcb);
//
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call dbo.atum_SetGuildMark(?, ?, ?)}", SQL_NTS);
//
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SetGuildMark @ QP_GuildSetGuildMark() Failed!\n", TRUE, q.pIOCPSocket);
//
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	// Clear any result sets generated.
//	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	m_pIMIOCP5->m_MapGuild.lock();
//	{
//		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);	
//		if(ptmGuild)
//		{
//			// guild mark의 정보 할당
//			ptmGuild->m_GuildMark.nGuildMarkVersion = GuildMarkVersion;
//			ptmGuild->m_GuildMark.nGuildMarkSize = GuildMarkImageSize;
//
//			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_GUILD_MARK_OK, T_IC_GUILD_SET_GUILD_MARK_OK, msgSetMarkOK, msgSetMarkOKBuf);
//			msgSetMarkOK->GuildMarkVersion = GuildMarkVersion;
//			msgSetMarkOK->SizeOfGuildMark = GuildMarkImageSize;
//			memcpy(msgSetMarkOKBuf+MSG_SIZE(MSG_IC_GUILD_SET_GUILD_MARK_OK), ptmGuild->m_GuildMark.GuildMarkImage, GuildMarkImageSize);
//			ptmGuild->SendMsgToGuildMembers(msgSetMarkOKBuf, MSG_SIZE(MSG_IC_GUILD_SET_GUILD_MARK_OK)+GuildMarkImageSize);
//		}
//	}
//	m_pIMIOCP5->m_MapGuild.unlock();

	UID32_t		GuildUniqueNumber	= (UID32_t)q.nGeneralParam1;
	GUILD_MARK	*pGuildMark			= (GUILD_MARK*)q.pQueryParam;
	CIMIOCPSocket *pIISock			= (CIMIOCPSocket*)q.pIOCPSocket;
	
	if(NULL == pIISock || FALSE == pIISock->IsValidCharacter())
	{
		util::del(pGuildMark);
		return;
	}

	RETCODE ret;
	/*[Stored Query Definition]************************************************
	-- Name: atum_SetGuildMark
	-- Desc: 길드 마크 할당
	--====
	--			// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 인자추가(@i_guildMarkState		TINYINT)
	--DROP PROCEDURE atum_SetGuildMark
	--GO
	CREATE PROCEDURE dbo.atum_SetGuildMark
		@guildUniqueNumber		INT,	-- UINT
		@guildMarkSize			INT,
		@guildMarkImage			BINARY(1600),
		@i_guildMarkState		TINYINT			-- // 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 인자추가(@i_guildMarkState		TINYINT)
	AS
		DECLARE @GuildMarkVersion INT
		SET @GuildMarkVersion = (SELECT GuildMarkVersion FROM td_Guild WITH (NOLOCK) WHERE GuildUniqueNumber = @guildUniqueNumber) + 1

		-- // 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - Update에 추가함
		UPDATE td_Guild
		SET GuildMarkVersion = @GuildMarkVersion, GuildMarkSize = @guildMarkSize, GuildMarkImage = @guildMarkImage, GuildMarkState= @i_guildMarkState
		WHERE GuildUniqueNumber = @guildUniqueNumber
		
		-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
		--RETURN @GuildMarkVersion
		SELECT @GuildMarkVersion;
	GO
	**************************************************************************/

// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pGuildMark->nGuildMarkVersion, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pGuildMark->nGuildMarkSize, 0, NULL);
// 	SQLINTEGER pcb = SIZE_MAX_GUILD_MARK_IMAGE;
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, SIZE_MAX_GUILD_MARK_IMAGE, 0, pGuildMark->GuildMarkImage, 0, &pcb);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pGuildMark->byGuildMarkState, 0, NULL);
// 
// 	// 2008-07-08 by cmkwon, MySQL 지원으로 수정(프로시저에서 리턴처리 불가로 수정) - 
// 	//ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call atum_SetGuildMark(?, ?, ?, ?)}", SQL_NTS);
// 	ret = SQLExecDirect(hstmt, (UCHAR*)"{call atum_SetGuildMark(?, ?, ?, ?, ?)}", SQL_NTS);
// 	if(ret == SQL_NO_DATA)
// 	{// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - GuildUID 오류
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pGuildMark);
// 		return;
// 	}
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SetGuildMark @ QP_GuildSetGuildMark() Failed!\n", TRUE, q.pIOCPSocket);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pGuildMark);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pGuildMark->nGuildMarkSize, 0, NULL);
	SQLINTEGER pcb = SIZE_MAX_GUILD_MARK_IMAGE;
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, SIZE_MAX_GUILD_MARK_IMAGE, 0, pGuildMark->GuildMarkImage, 0, &pcb);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pGuildMark->byGuildMarkState, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0224, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SetGuildMark @ QP_GuildSetGuildMark() Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pGuildMark);
		return;
	}
	if(ret == SQL_NO_DATA)
	{// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - GuildUID 오류
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pGuildMark);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &(pGuildMark->nGuildMarkVersion), 0, NULL);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);	
		if(ptmGuild)
		{
			// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 여단 마크 할당한다
			ptmGuild->m_GuildMark	= *pGuildMark;

			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_GUILD_MARK_OK, T_IC_GUILD_SET_GUILD_MARK_OK, msgSetMarkOK, msgSetMarkOKBuf);
			msgSetMarkOK->GuildMarkVersion		= pGuildMark->nGuildMarkVersion;
			if(GUILD_MARK_STATE_NORMAL == pGuildMark->byGuildMarkState)
			{
				msgSetMarkOK->SizeOfGuildMark	= pGuildMark->nGuildMarkSize;
				memcpy(msgSetMarkOKBuf+MSG_SIZE(MSG_IC_GUILD_SET_GUILD_MARK_OK), pGuildMark->GuildMarkImage, pGuildMark->nGuildMarkSize);
			}
			else
			{
				msgSetMarkOK->SizeOfGuildMark	= 0;
			}
			ptmGuild->SendMsgToGuildMembers(msgSetMarkOKBuf, MSG_SIZE(MSG_IC_GUILD_SET_GUILD_MARK_OK)+msgSetMarkOK->SizeOfGuildMark);
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();

	if(GUILD_MARK_STATE_WAITING_PERMISSION == pGuildMark->byGuildMarkState)
	{// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 마크심사 대기 상태 체크
		pIISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070802_0001);
		pIISock->SendString128(STRING_128_USER_NOTICE, STRMSG_070802_0002);
	}

	util::del(pGuildMark);
}

// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 사용하지 안는 함수임
//// QT_GuildGetGuildMark
//void CAtumIMDBManager::QP_GuildGetGuildMark(DB_QUERY q, SQLHSTMT hstmt)
//{
//	UID32_t GuildUniqueNumber = (UID32_t)q.nGeneralParam1;
//
//	if(NULL == q.pIOCPSocket || FALSE == q.pIOCPSocket->IsUsing())
//	{
//		return;
//	}
//
//	RETCODE ret;
//	SQLINTEGER pcb;
//	INT nImageSize = 0;
//	INT nImageVersion = 0;
//	char Image[SIZE_MAX_GUILD_MARK_IMAGE];
//
//	/*[Stored Query Definition]************************************************
//	CREATE PROCEDURE atum_GetGuildMark
//		@guildUniqueNumber		INT	-- UINT
//	AS
//		SELECT GuildMarkVersion, GuildMarkSize, GuildMarkImage
//		FROM td_Guild
//		WHERE GuildUniqueNumber = @guildUniqueNumber
//	GO
//	**************************************************************************/
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
//
//	ret = SQLExecDirect(hstmt, (UCHAR*)"{call dbo.atum_GetGuildMark(?)}", SQL_NTS);
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetGuildMark @ QP_GuildGetGuildMark() Failed!\n", TRUE, q.pIOCPSocket);
//
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return;
//	}
//
//	SQLBindCol(hstmt, 1, SQL_C_ULONG, &nImageVersion, 0, NULL);
//	SQLBindCol(hstmt, 2, SQL_C_ULONG, &nImageSize, 0, NULL);
//	SQLBindCol(hstmt, 3, SQL_C_BINARY, Image, SIZE_MAX_GUILD_MARK_IMAGE, &pcb);
//
//	// Fetch Data
//	ret = SQLFetch(hstmt);
//
//	if(ret == SQL_NO_DATA)
//	{
//		// 길드가 없음, 그냥 리턴
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//
//		return;
//	}
//
//	m_pIMIOCP5->m_MapGuild.lock();
//	{
//		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(GuildUniqueNumber);	
//		if(ptmGuild)
//		{
//			ptmGuild->m_GuildMark.nGuildMarkVersion = nImageVersion;
//			ptmGuild->m_GuildMark.nGuildMarkSize = nImageSize;
//			memcpy(ptmGuild->m_GuildMark.GuildMarkImage, Image, nImageSize);
//
//			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_GUILD_MARK_OK, T_IC_GUILD_GET_GUILD_MARK_OK, msgGetMarkOK, msgGetMarkOKBuf);
//			msgGetMarkOK->GuildUniqueNumber = ptmGuild->m_GuildUniqueNumber;
//			msgGetMarkOK->GuildMarkVersion = nImageVersion;
//			msgGetMarkOK->SizeOfGuildMark = nImageSize;
//			memcpy(msgGetMarkOKBuf+MSG_SIZE(MSG_IC_GUILD_GET_GUILD_MARK_OK), ptmGuild->m_GuildMark.GuildMarkImage, nImageSize);
//			ptmGuild->SendMsgToGuildMembers(msgGetMarkOKBuf, MSG_SIZE(MSG_IC_GUILD_GET_GUILD_MARK_OK)+nImageSize);
//		}
//	}
//	m_pIMIOCP5->m_MapGuild.unlock();
//
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//	return;
//}

// QT_GuildSetRank
void CAtumIMDBManager::QP_GuildSetRank(DB_QUERY q, SQLHSTMT hstmt)
{
	BYTE	NewGuildRank = (BYTE)q.pGeneralParam;
	UID32_t	MemberUniqueNumber = (UID32_t)q.nGeneralParam1;
	UID32_t	uidGuild = (UID32_t)q.nGeneralParam2;
	
	if(NULL == q.pIOCPSocket || FALSE == q.pIOCPSocket->IsUsing())
	{
		return;
	}

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_ChangeGuildRank
		@memberUniqueNumber		INT,	-- UINT
		@rank					TINYINT
	AS
		UPDATE td_guildMember
		SET GuildRank = @rank
		WHERE CharacterUniqueNumber = @memberUniqueNumber
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &MemberUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &NewGuildRank, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0225, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_ChangeGuildRank @ QP_GuildSetRank() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.findLock(uidGuild);
		if (ptmGuild)
		{
			CGuildMember *pMember = NULL;
			ptmGuild->GetGuildMember(MemberUniqueNumber, pMember);
			if (pMember != NULL)
			{
				pMember->m_Rank = NewGuildRank;
			}
			
			// 2006-09-25 by dhjin, FieldServer로 길드 랭크 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_FI_GUILD_RANK_OK, msgRank,msgRankBuf);
			msgRank->MemberUniqueNumber	= MemberUniqueNumber;
			msgRank->GuildRank			= NewGuildRank;
			m_pIMIOCP5->SendMessageToFieldServer(msgRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));
	
			// MSG 전송
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_SET_RANK_OK, T_IC_GUILD_SET_RANK_OK, msgSetRank, msgSetRankBuf);
			msgSetRank->MemberUniqueNumber = MemberUniqueNumber;
			msgSetRank->GuildRank = NewGuildRank;
			ptmGuild->SendMsgToGuildMembers(msgSetRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();
		
	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;
}

// QT_GuildDeleteGuild
void CAtumIMDBManager::QP_GuildDeleteGuild(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t GuildUniqueNumber = (UID32_t)q.nGeneralParam1;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_DismemberGuild
		@guildUniqueNum		INT	-- UINT
	AS
		-- 길드 삭제
		DELETE FROM td_Guild WHERE GuildUniqueNumber = @guildUniqueNum

		-- 길드원 삭제
		DELETE FROM td_GuildMember WHERE GuildUniqueNumber = @guildUniqueNum
		
		-- 2007-11-02 by jin, 지도자 후보이면 삭제
		DELETE FROM td_LeaderCandidate WHERE GuildUID = @guildUniqueNum

		-- 캐릭터 정보 삭제
		UPDATE td_Character
		SET guildname = NULL, guilduniquenumber = 0
		WHERE guilduniquenumber = @guildUniqueNum

		-- // 2008-06-13 by dhjin, EP3 - 여단 수정 사항
		DELETE FROM dbo.td_SelfIntroduction WHERE GuildUID = @guildUniqueNum
		DELETE FROM dbo.td_GuildIntroduction WHERE GuildUID = @guildUniqueNum
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0226, SQL_NTS);

	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DismemberGuild @ QP_GuildDeleteGuild() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// m_MapGuild에서 지우기
	m_pIMIOCP5->m_MapGuild.lock();
	{
		CGuild *ptmGuild = m_pIMIOCP5->m_MapGuild.popLock(GuildUniqueNumber);	
		if (ptmGuild != NULL)
		{
			// MSG 준비
			INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_DISMEMBER_OK, T_IC_GUILD_DISMEMBER_OK, msgDismember, msgDismemberBuf);
			util::strncpy(msgDismember->GuildName, ptmGuild->m_GuildName, SIZE_MAX_GUILD_NAME);

			BOOL CheckDelete0 = TRUE;
			listGuildMember::iterator itr = ptmGuild->m_ListGuildMember.begin();
			while(itr != ptmGuild->m_ListGuildMember.end())
			{
				CGuildMember &tmpMember = *itr;
				CIMIOCPSocket *pSocket = tmpMember.m_pIMIOCPSocket;
				if (pSocket
					&& pSocket->IsValidCharacter())
				{					
					pSocket->SetGuildUniqueNumber(INVALID_GUILD_UID);				// 캐릭터 데이터 초기화					
					pSocket->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);	// Field Server에 알림

					// MSG 전송
					pSocket->SendAddData(msgDismemberBuf, MSG_SIZE(MSG_IC_GUILD_DISMEMBER_OK));

					//////////////////////////////////////////////////////////////////////////
					// 2006-09-29 by dhjin, 길드 삭제 시 한번 만 PROCEDURE atum_AllDelete_Guild_Store 실행
					INIT_MSG_WITH_BUFFER(MSG_FI_CHARACTER_UPDATE_GUILD_INFO, T_FI_GUILD_DELETE_INFO_OK, msgGuilddelete, msgGuilddeleteBuf);
					msgGuilddelete->CharacterUniqueNumber	= pSocket->m_character.CharacterUniqueNumber;
					msgGuilddelete->GuildUniqueNumber		= GuildUniqueNumber;
					msgGuilddelete->GuildDelete				= TRUE;
					m_pIMIOCP5->SendMessageToFieldServer(msgGuilddeleteBuf, MSG_SIZE(MSG_FI_CHARACTER_UPDATE_GUILD_INFO));
				}
				itr++;
			}

			util::del(ptmGuild);
		}
	}
	m_pIMIOCP5->m_MapGuild.unlock();

	return;
}

// QT_GuildLoadGuild
void CAtumIMDBManager::QP_GuildLoadGuild(DB_QUERY q, SQLHSTMT hstmt)
{
	CIMIOCPSocket *pSocket = m_pIMIOCP5->GetIMIOCPSocketByCharacterUID((UID32_t)q.nGeneralParam1);
	if (pSocket == NULL || FALSE == pSocket->IsValidCharacter())
	{
		return;
	}

	UID32_t	GuildUniqueNumber = pSocket->m_character.GuildUniqueNumber;
	if(FALSE == IS_VALID_UNIQUE_NUMBER(GuildUniqueNumber))
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 1. 길드 로딩
	CGuild tmGuild;
	SQLINTEGER arrCB[19] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS								// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
							};
	SQL_TIMESTAMP_STRUCT	GuildDismemberDate; memset(&GuildDismemberDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_GetGuildInfoByUniqueNumber
		@guildUniqueNum		INT	-- UINT
	AS
		SELECT g.GuildUniqueNumber, g.GuildName, g.GuildCommanderUniqueNumber, g.GuildMemberCapacity, g.GuildState, g.GuildDismemberDate,
				g.GuildMarkVersion, g.GuildMarkSize, g.GuildMarkImage, g.WarWinPoint, g.WarLossPoint, g.TotalFame, g.MonthlyFame,
				g.GuildMarkState, g.Notice, o.OutPostCityMapIndex, g.GuildTotalFameRank, g.GuildMonthlyFameRank
		FROM td_Guild AS g WITH (NOLOCK) Left outer join td_outpostinfo AS o WITH (NOLOCK)
			ON g.GuildUniqueNumber = o.outpostGuilduid
			WHERE GuildUniqueNumber = @guildUniqueNum
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
	RETCODE	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0227, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetGuildInfoByUniqueNumber Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLBindCol(hstmt,  1, SQL_C_ULONG, &tmGuild.m_GuildUniqueNumber, 0,									&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, tmGuild.m_GuildName, SIZE_MAX_GUILD_NAME,							&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_ULONG, &tmGuild.m_CommanderUniqueNumber, 0,								&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_C_LONG, &tmGuild.m_nGuildMemberCapacity, 0,								&arrCB[4]);
	SQLBindCol(hstmt,  5, SQL_C_UTINYINT, &tmGuild.m_GuildState, 0,										&arrCB[5]);
	SQLBindCol(hstmt,  6, SQL_C_TIMESTAMP, &GuildDismemberDate, 0,										&arrCB[6]);
	SQLBindCol(hstmt,  7, SQL_C_LONG, &tmGuild.m_GuildMark.nGuildMarkVersion, 0,						&arrCB[7]);
	SQLBindCol(hstmt,  8, SQL_C_LONG, &tmGuild.m_GuildMark.nGuildMarkSize, 0,							&arrCB[8]);
	SQLBindCol(hstmt,  9, SQL_C_BINARY, tmGuild.m_GuildMark.GuildMarkImage, SIZE_MAX_GUILD_MARK_IMAGE,	&arrCB[9]);
	SQLBindCol(hstmt, 10, SQL_C_LONG, &tmGuild.m_nWarWinPoint, 0,										&arrCB[10]);
	SQLBindCol(hstmt, 11, SQL_C_LONG, &tmGuild.m_nWarLossPoint, 0,										&arrCB[11]);	
	SQLBindCol(hstmt, 12, SQL_C_LONG, &tmGuild.m_nTotalFame, 0,											&arrCB[12]);	
	SQLBindCol(hstmt, 13, SQL_C_LONG, &tmGuild.m_nMonthlyFame, 0,										&arrCB[13]);	
	SQLBindCol(hstmt, 14, SQL_C_TINYINT, &tmGuild.m_GuildMark.byGuildMarkState, 0,						&arrCB[14]);	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 추가된 필드 가져오기
	SQLBindCol(hstmt, 15, SQL_C_CHAR, tmGuild.m_Notice, SIZE_MAX_NOTICE,								&arrCB[15]);	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	SQLBindCol(hstmt, 16, SQL_C_ULONG, &tmGuild.m_OutPostCityMapIndex, 0,								&arrCB[16]);	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	SQLBindCol(hstmt, 17, SQL_C_ULONG, &tmGuild.m_nTotalFameRank, 0,									&arrCB[17]);	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항
	SQLBindCol(hstmt, 18, SQL_C_ULONG, &tmGuild.m_nMonthlyFameRank, 0,									&arrCB[18]);	// 2008-06-05 by dhjin, EP3 - 여단 수정 사항	

	if(SQL_NO_DATA == SQLFetch(hstmt))
	{// 길드 정보가 없다, 그냥 리턴
		SQLFreeStmt(hstmt, SQL_CLOSE);
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[ERROR] QP_GuildLoadGuild error 1, CharacterUID(%d), CharacterGuildUID(%d)\r\n"
			, pSocket->m_character.CharacterUniqueNumber, pSocket->m_character.GuildUniqueNumber);
		pSocket->SendGuildInfoToFieldServer(INVALID_GUILD_UID, NULL);
		pSocket->SendString128(STRING_128_USER_ERR, STRMSG_S_I2NOTIFY_0003);
		return;
	}
	tmGuild.m_DismemberDate = GuildDismemberDate;
	SQLFreeStmt(hstmt, SQL_CLOSE);

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Get_Guild_Master_CashStoreInfo
	-- DESC				: 여단장 캐쉬 정보 얻어오기
	-- // 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Get_Guild_Master_CashStoreInfo
		@GuildMasterUID			INT	
	AS
		SELECT A.ExpireTime
			FROM td_Character AS C WITH (NOLOCK) INNER JOIN td_AccountCashStore AS A
				ON C.AccountUniqueNumber = A.AccountUniquenumber
			WHERE C.UniqueNumber = @GuildMasterUID	
	GO
	**************************************************************************/
	arrCB[1] = SQL_NTS;
	SQL_TIMESTAMP_STRUCT	GuildMemberShipExpireTime; memset(&GuildMemberShipExpireTime, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmGuild.m_CommanderUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0211, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_Get_Guild_Master_CashStoreInfo Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLBindCol(hstmt,  1, SQL_C_TIMESTAMP, &GuildMemberShipExpireTime, 0,		&arrCB[1]);
	
	if(SQL_NO_DATA != SQLFetch(hstmt))
	{
		ATUM_DATE_TIME tmGuildMemberShipExpireTime;
		tmGuildMemberShipExpireTime = GuildMemberShipExpireTime;
		tmGuild.SetGuildMemberShip(TRUE, &tmGuildMemberShipExpireTime);
	}	
	SQLFreeStmt(hstmt, SQL_CLOSE);	
	// end_길드 로딩
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// 3. 길드 해제 시간 체크
	ATUM_DATE_TIME tmAtumTime;
	tmAtumTime = GuildDismemberDate;
	if (tmGuild.m_GuildState == GUILD_STATE_DISMEMBER_READY
		&& tmAtumTime < ATUM_DATE_TIME::GetCurrentDateTime())
	{			
		MakeAndEnqueueQuery(QT_GuildDeleteGuild, NULL, NULL, NULL, GuildUniqueNumber, tmGuild.m_CommanderUniqueNumber);
		return;
	}
	// end_길드 해제 시간 체크
	///////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-30 by dhjin, EP3 - 여단 수정 사항 - 여단이 로딩되어 있는지 체크
	if(FALSE == m_pIMIOCP5->CheckGuildLoaded(&tmGuild, pSocket))
	{// 2008-05-30 by dhjin, 여단이 로딩 되어 있다면 길드원 로딩은 하지 않는다.
		///////////////////////////////////////////////////////////////////////////////
		// 4. 길드원 로딩
		/*[Stored Query Definition]************************************************
		CREATE PROCEDURE atum_GetAllGuildMembers
			@guildUniqueNum		INT	-- UINT
		AS
			-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
			SELECT c.CharacterName, c.UniqueNumber, gm.GuildRank, c.UnitKind, c.Level
			FROM td_Character c, td_GuildMember gm
			WHERE c.UniqueNumber = gm.CharacterUniqueNumber AND gm.GuildUniqueNumber = @guildUniqueNum
		GO
		**************************************************************************/
		MEX_GUILD_MEMBER_INFO tmpMemberInfo;
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
		ret = SQLExecDirect(hstmt, PROCEDURE_080822_0229, SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
		{
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetAllGuildMembers Failed!\n", TRUE, q.pIOCPSocket);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			return;
		}
		arrCB[1] = arrCB[2] = arrCB[3] = arrCB[4] = arrCB[5] = SQL_NTS;			// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
		SQLBindCol(hstmt,  1, SQL_C_CHAR, tmpMemberInfo.MemberName, SIZE_MAX_CHARACTER_NAME,	&arrCB[1]);
		SQLBindCol(hstmt,  2, SQL_C_ULONG, &tmpMemberInfo.MemberUniqueNumber, 0,				&arrCB[2]);
		SQLBindCol(hstmt,  3, SQL_C_UTINYINT, &tmpMemberInfo.GuildRank, 0,						&arrCB[3]);
		SQLBindCol(hstmt,  4, SQL_C_ULONG, &tmpMemberInfo.UnitKind, 0,							&arrCB[4]);
		SQLBindCol(hstmt,  5, SQL_C_UTINYINT, &tmpMemberInfo.MemberLevel, 0,					&arrCB[5]);

		// 2008-05-30 by dhjin, EP3 - 여단 수정 사항 - 여단이 로딩되어 있는지 체크, FieldServer로 전송 길드 랭크 수정
//		INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_RANK, T_FI_GUILD_RANK_OK, msgGuildRank, msgGuildRankBuf);	// 2006-09-25 by dhjin, FieldServer로 전송 길드 랭크 준비

		while ( (ret = SQLFetch(hstmt)) != SQL_NO_DATA)
		{
			CGuildMember tmpNewMember(tmpMemberInfo.MemberName, tmpMemberInfo.MemberUniqueNumber, NULL,
									GuildUniqueNumber, tmpMemberInfo.GuildRank, FALSE, tmpMemberInfo.UnitKind, tmpMemberInfo.MemberLevel, CHECK_TYPE_VOIP_NONE);	// 2008-05-20 by dhjin, EP3 - 여단 수정 사항
			tmGuild.AddGuildMember(tmpNewMember);

			// 2008-05-30 by dhjin, EP3 - 여단 수정 사항 - 여단이 로딩되어 있는지 체크, FieldServer로 전송 길드 랭크 수정
//			if (tmpMemberInfo.MemberUniqueNumber == pSocket->m_character.CharacterUniqueNumber)
//			{// 2006-09-25 by dhjin, FieldServer로 전송 하기 위한 길드 랭크 정보 셋팅
//				msgGuildRank->charUID		= tmpMemberInfo.MemberUniqueNumber;
//				msgGuildRank->GuildRank		= tmpMemberInfo.GuildRank;
//			}
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		// end_길드원 로딩
		///////////////////////////////////////////////////////////////////////////////

		if(FALSE == pSocket->IsValidCharacter())
		{
			return;
		}
		
		if(FALSE == m_pIMIOCP5->OnGuildLoading(&tmGuild, pSocket))
		{
			return;
		}		
	}

	pSocket->SetGuildUniqueNumber(GuildUniqueNumber);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-05-30 by dhjin, EP3 - 여단 수정 사항 - 여단이 로딩되어 있는지 체크, FieldServer로 전송 길드 랭크 수정
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetGuildRank
	-- DESC				: 여단 랭크 가져오기
	-- // 2008-05-30 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE atum_GetGuildRank
		@i_GuildUID			INT,	-- UINT
		@i_CharacterUID		INT
	AS
		SELECT GuildRank
		FROM td_GuildMember
		WHERE CharacterUniqueNumber = @i_CharacterUID AND GuildUniqueNumber = @i_GuildUID
	GO
	**************************************************************************/	
	INIT_MSG_WITH_BUFFER(MSG_FI_GUILD_RANK, T_FI_GUILD_RANK_OK, msgGuildRank, msgGuildRankBuf);	// 2006-09-25 by dhjin, FieldServer로 전송 길드 랭크 준비
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSocket->m_character.CharacterUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0230, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetGuildRank Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	arrCB[1] = SQL_NTS;	
	SQLBindCol(hstmt,  1, SQL_C_UTINYINT, &msgGuildRank->GuildRank, 0,						&arrCB[1]);
	// fetching
	ret = SQLFetch(hstmt);
	if(SQL_NO_DATA != ret) 	{}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	msgGuildRank->charUID		= pSocket->m_character.CharacterUniqueNumber;
	pSocket->SendMsgToCurrentFieldServerSocket(msgGuildRankBuf, MSG_SIZE(MSG_IC_GUILD_SET_RANK_OK));			// 2006-09-25 by dhjin, FieldServer로 정보 전송

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteGuildIntroduction_By_PassedRangeDay
	-- DESC				: 지난 여단 소개 삭제
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteGuildIntroduction_By_PassedRangeDay
		@i_PassedRangeDay	INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE GuildUID IN 
					(SELECT GuildUID FROM dbo.td_GuildIntroduction WITH (NOLOCK) WHERE (DATEDIFF(dd, SendDate, GETDATE()) > @i_PassedRangeDay )
		DELETE FROM dbo.td_GuildIntroduction WHERE (DATEDIFF(dd, SendDate, GETDATE()) > @i_PassedRangeDay
	GO
	**************************************************************************/
	INT PassedRangeDay = GUILDINTRODUCTION_PASSED_RANGE_DAY;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &PassedRangeDay, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0231, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt,"atum_DeleteGuildIntroduction_By_PassedRangeDay Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetGuildIntroduction
	-- DESC				: 여단 소개 가져오기
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetGuildIntroduction
		@i_GuildUID					INT
	AS
		SELECT Content, WriteDate FROM dbo.td_GuildIntroduction WITH (NOLOCK) WHERE GuildUID = @i_GuildUID
	GO
	**************************************************************************/
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSocket->m_character.GuildUniqueNumber, 0,			&arrCB[1]);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0232, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetGuildIntroduction Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLINTEGER cb1, cb2;
	SGUILD_INTRODUCTION	GuildIntroduction;
	util::zero(&GuildIntroduction, sizeof(SGUILD_INTRODUCTION));
	SQL_TIMESTAMP_STRUCT	RegDate; memset(&RegDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));

	SQLBindCol(hstmt,  1, SQL_C_CHAR, GuildIntroduction.IntroductionContent, SIZE_MAX_NOTICE, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_TIMESTAMP, &RegDate, 0, &cb2);

	// fetching
	ret = SQLFetch(hstmt);
	if(SQL_NO_DATA != ret)
	{
		GuildIntroduction.RegDate	= RegDate;
	}

	mt_auto_lock mtG(&m_pIMIOCP5->m_MapGuild);
	CGuild *pGuild = m_pIMIOCP5->m_MapGuild.findLock(pSocket->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	pGuild->SetGuildIntroduction(&GuildIntroduction);

	// 길드 로딩 완료 전송
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_LOADING_GUILD_DONE, T_IC_GUILD_LOADING_GUILD_DONE, msgLoadingDone, msgLoadingDoneBuf);
	msgLoadingDone->GuildUniqueNumber = GuildUniqueNumber;
	pSocket->SendAddData(msgLoadingDoneBuf, MSG_SIZE(MSG_IC_GUILD_LOADING_GUILD_DONE));

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;	

}

void CAtumIMDBManager::QP_GuildSaveGuildWarPoint(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t		uidGuildUID		= (UID32_t)q.pGeneralParam;
	INT			nWarWinPoint	= q.nGeneralParam1;
	INT			nWarLossPoint	= q.nGeneralParam2;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_SaveGuildWarPoint
		@i_GuildUniqueNumber	INT,	-- UINT
		@i_WarWinPoint			INT,
		@i_WarLossPoint			INT
	AS
		UPDATE td_Guild
		SET WarWinPoint = @i_WarWinPoint, WarLossPoint = @i_WarLossPoint
		WHERE GuildUniqueNumber = @i_GuildUniqueNumber
	GO
	**************************************************************************/

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &uidGuildUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nWarWinPoint, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nWarLossPoint, 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0233, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SaveGuildWarPoint @ QP_GuildSaveGuildWarPoint() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}


void CAtumIMDBManager::QP_GuildDeleteGuildUIDOfCharacter(DB_QUERY q, SQLHSTMT hstmt)
{
	UID32_t uidCharacterUID = q.nGeneralParam1;
	if(0 == uidCharacterUID)
	{
		return;
	}
	
//	char	SQLString[1024];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(SQLString, "UPDATE td_Character SET GuildName = \'\', GuildUniqueNumber = 0 WHERE uniqueNumber = %d", uidCharacterUID);
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	sprintf(SQLString, QUERY_080702_0254, uidCharacterUID);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &uidCharacterUID, 0, NULL);

	RETCODE	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0254), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		// 실패
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "QP_GuildDeleteGuildUIDOfCharacter() Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildAddGuildFame(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-27 ~ 2005-12-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildAddGuildFame(DB_QUERY q, SQLHSTMT hstmt)
{
	CIMIOCPSocket *pIISock = (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}

	UID32_t		uidGuildUID				= (UID32_t)q.pGeneralParam;
	INT			nTotalFameAddition		= (INT)q.nGeneralParam1;
	INT			nMonthlyFameAddition	= (INT)q.nGeneralParam1;
		
	/*[Stored Query Definition]************************************************
	-- Name: atum_GuildAddGuildFame
	-- Desc: 2005-12-28 by cmkwon, 여단 명성 더하기
	--====
	CREATE PROCEDURE atum_GuildAddGuildFame
		@i_guildUID				INT,	-- UINT
		@i_totalFameAddition	INT,	-- 추가량
		@i_monthlyFameAddition	INT
	AS
		UPDATE td_Guild
			SET TotalFame= TotalFame+@i_totalFameAddition, MonthlyFame= MonthlyFame+@i_monthlyFameAddition
			WHERE GuildUniqueNumber = @i_guildUID
		
		SELECT TotalFame, MonthlyFame
			FROM td_Guild
			WHERE GuildUniqueNumber = @i_guildUID		
	GO
	**************************************************************************/

	SQLINTEGER arrCB[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &uidGuildUID, 0,			&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTotalFameAddition, 0,		&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nMonthlyFameAddition, 0,	&arrCB[3]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0234, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GuildAddGuildFame Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	INT	nRetTotalFame	= 0;
	INT	nRetMonthlyFame	= 0;
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindCol(hstmt,  1, SQL_C_LONG, &nRetTotalFame, 0,		&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_LONG, &nRetMonthlyFame, 0,		&arrCB[2]);
	if(SQL_NO_DATA == SQLFetch(hstmt))
	{// 리턴 정보가 없다
		SQLFreeStmt(hstmt, SQL_CLOSE);
		g_pIMGlobal->WriteSystemLogEX(TRUE, "[ERROR] QP_GuildAddGuildFame error 1, CharacterUID(%d), CharacterGuildUID(%d)\r\n"
			, pIISock->m_character.CharacterUniqueNumber, pIISock->m_character.GuildUniqueNumber);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	m_pIMIOCP5->GuildSetGuildFame(uidGuildUID, nRetTotalFame, nRetMonthlyFame);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildUpdateCommander(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단장 위임
/// \author		dhjin
/// \date		2008-05-20 ~ 2008-05-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildUpdateCommander(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildUpdateCommander
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateGuildCommander
	-- DESC				: 여단장 위임
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--						-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - Result Set ==> ErrorCode
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateGuildCommander
		@GuildUniqueNum INT,
		@Commander		INT,
		@NewCommander	INT,
		@LevelCheck     INT
	AS
		DECLARE @CheckBit	 TINYINT
		SET @CheckBit = (SELECT count(*) FROM dbo.td_CityLeaderInfo WITH (NOLOCK) WHERE CharacterUID = @Commander)
		IF(0 <> @CheckBit)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
			--RETURN 1
			SELECT 1;
			RETURN;
		END
		SET @CheckBit = (SELECT count(*) FROM dbo.td_LeaderCandidate WITH (NOLOCK) WHERE CharacterUID = @Commander)
		IF(0 <> @CheckBit)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
			--RETURN 2
			SELECT 2;
			RETURN;
		END
		SET @CheckBit = (SELECT count(*) FROM dbo.td_character WITH (NOLOCK) WHERE UniqueNumber = @NewCommander and Level >= @LevelCheck )
		IF(0 = @CheckBit)
		BEGIN
			SELECT 3;	-- // 2008-10-13 by dhjin, 여단장 위임 가능 레벨 체크 추가.
			RETURN;
		END

		UPDATE dbo.td_Guild SET GuildCommanderUniqueNumber = @NewCommander WHERE GuildUniqueNumber = @GuildUniqueNum
		UPDATE dbo.td_GuildMember SET GuildRank = 0	WHERE CharacterUniqueNumber = @Commander
		UPDATE dbo.td_GuildMember SET GuildRank = 1	WHERE CharacterUniqueNumber = @NewCommander

		-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
		SELECT 0;
	GO
	**************************************************************************/
	CIMIOCPSocket *pIISock = (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}

	UID32_t	NewCommander	= (UID32_t)q.pGeneralParam;
	
	BYTE ErrCheck = 0;	
	SQLINTEGER arrCB[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 아래와 같이 수정 함
//	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &ErrCheck, 0, &arrCB[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.GuildUniqueNumber, 0,			&arrCB[2]);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.CharacterUniqueNumber, 0,		&arrCB[3]);
//	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &NewCommander, 0,	&arrCB[4]);
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call dbo.atum_UpdateGuildCommander(?,?,?) }", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
// 	{
// 		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_UpdateGuildCommander Failed!\n", TRUE, q.pIOCPSocket);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
	int CheckGuildCommanderMinLevel = GuildCommanderMinLevel;			// 2008-10-13 by dhjin, 여단장 위임 가능 레벨 체크 추가.
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.GuildUniqueNumber, 0,			&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.CharacterUniqueNumber, 0,		&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &NewCommander, 0,									&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CheckGuildCommanderMinLevel, 0,					&arrCB[4]);		// 2008-10-13 by dhjin, 여단장 위임 가능 레벨 체크 추가.
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0279, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_UpdateGuildCommander Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &(ErrCheck), 0, NULL);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}

	if(FALSE == ErrCheck)
	{
		m_pIMIOCP5->UpdateGuildCommander(pIISock->m_character.GuildUniqueNumber, pIISock->m_character.CharacterUniqueNumber, NewCommander);
	}
	else
	{
		if(1 == ErrCheck)
		{
			pIISock->SendErrorMessage(T_IC_GUILD_NEW_COMMANDER, ERR_CANNOT_USE_NEW_COMMANDER_BY_CITYLEADER);
		}
		else if(2 == ErrCheck)
		{
			pIISock->SendErrorMessage(T_IC_GUILD_NEW_COMMANDER, ERR_CANNOT_USE_NEW_COMMANDER_BY_POLL);
		}
		else if(3 == ErrCheck)
		{// 2008-10-13 by dhjin, 여단장 위임 가능 레벨 체크 추가.
			pIISock->SendErrorMessage(T_IC_GUILD_NEW_COMMANDER, ERR_PROTOCOL_REQ_MINLEVEL_NOT_MATCHED);
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	
		
	SQLFreeStmt(hstmt, SQL_CLOSE);

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Get_Guild_Master_CashStoreInfo
	-- DESC				: 여단장 캐쉬 정보 얻어오기
	-- // 2008-06-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_Get_Guild_Master_CashStoreInfo
		@GuildMasterUID			INT	
	AS
		SELECT A.ExpireTime
			FROM td_Character AS C WITH (NOLOCK) INNER JOIN td_AccountCashStore AS A
				ON C.AccountUniqueNumber = A.AccountUniquenumber
			WHERE C.UniqueNumber = @GuildMasterUID	
	GO
	**************************************************************************/
	arrCB[1] = SQL_NTS;
	SQL_TIMESTAMP_STRUCT	GuildMemberShipExpireTime; memset(&GuildMemberShipExpireTime, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &NewCommander, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0211, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_Get_Guild_Master_CashStoreInfo Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLBindCol(hstmt,  1, SQL_C_TIMESTAMP, &GuildMemberShipExpireTime, 0,		&arrCB[1]);
	
	if(SQL_NO_DATA != SQLFetch(hstmt))
	{
		ATUM_DATE_TIME tmGuildMemberShipExpireTime;
		tmGuildMemberShipExpireTime = GuildMemberShipExpireTime;
		m_pIMIOCP5->SetGuildMemberShipW(pIISock->m_character.GuildUniqueNumber, TRUE, &tmGuildMemberShipExpireTime);
	}	
	else
	{
		m_pIMIOCP5->SetGuildMemberShipW(pIISock->m_character.GuildUniqueNumber, FALSE);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);	
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildNotice(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 공지
/// \author		dhjin
/// \date		2008-05-21 ~ 2008-05-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildNotice(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildNotice
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateGuildNotice
	-- DESC				: 여단 공지
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateGuildNotice
		@GuildUniqueNum INT,
		@Notice			VARCHAR(512)
	AS
		UPDATE dbo.td_Guild SET Notice = @Notice WHERE GuildUniqueNumber = @GuildUniqueNum
	GO
	**************************************************************************/
	QPARAM_GUILD_NOTICE *pGuildNotice = (QPARAM_GUILD_NOTICE*)q.pQueryParam;
	CIMIOCPSocket *pIISock = (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		util::del(pGuildNotice);
		return;
	}

	SQLINTEGER arrCB[3]={SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.GuildUniqueNumber, 0,			&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTICE, 0, pGuildNotice->Notice, 0,				&arrCB[2]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0236, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_UpdateGuildNotice Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pGuildNotice);
		return;
	}
		
	mt_auto_lock mtG(&m_pIMIOCP5->m_MapGuild);
	CGuild *pGuild = m_pIMIOCP5->m_MapGuild.findLock(pIISock->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pGuildNotice);
		return;
	}
	util::strncpy(pGuild->m_Notice, pGuildNotice->Notice, SIZE_MAX_NOTICE);

	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_NOTICE_WRITE_OK, T_IC_GUILD_NOTICE_WRITE_OK, pMsg, SendBuf);
	util::strncpy(pMsg->Notice, pGuild->m_Notice, SIZE_MAX_NOTICE);
	pGuild->SendMsgToGuildMembers(SendBuf, MSG_SIZE(MSG_IC_GUILD_NOTICE_WRITE_OK));

	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pGuildNotice);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildGetApplicant(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildGetApplicant(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildGetApplicant
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetGuildApplicant
	-- DESC				: 여단 지원자 가져오기
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetGuildApplicant
		@i_GuildUID					INT
	AS
		SELECT S.CharacterUID, C.CharacterName, C.UnitKind, C.Level
		FROM dbo.td_SelfIntroduction AS S WITH (NOLOCK) INNER JOIN dbo.td_character AS C WITH (NOLOCK)
			ON C.UniqueNumber = S.CharacterUID
			WHERE S.GuildUID = @i_GuildUID
	GO
	**************************************************************************/
	CIMIOCPSocket *pIISock = (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}

	SQLINTEGER arrCB[2]={SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.GuildUniqueNumber, 0,			&arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0237, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetGuildApplicant Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLINTEGER cb1, cb2, cb3, cb4;
	
	SGUILD_APPLICANT_INFO	tmpGuildApplicantInfo;
	util::zero(&tmpGuildApplicantInfo, sizeof(SGUILD_APPLICANT_INFO));
	vectSGUILD_APPLICANT_INFO	vectGuildApplicantInfo;
	vectGuildApplicantInfo.clear();

	SQLBindCol(hstmt,  1, SQL_C_ULONG, &tmpGuildApplicantInfo.CharacterUID, 0, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, tmpGuildApplicantInfo.CharacterName, SIZE_MAX_CHARACTER_NAME, &cb2);
	SQLBindCol(hstmt,  3, SQL_C_ULONG, &tmpGuildApplicantInfo.UnitKind, 0, &cb3);
	SQLBindCol(hstmt,  4, SQL_C_UTINYINT, &tmpGuildApplicantInfo.Level, 0, &cb4);

	// fetching
	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		vectGuildApplicantInfo.push_back(tmpGuildApplicantInfo);
		util::zero(&tmpGuildApplicantInfo, sizeof(SGUILD_APPLICANT_INFO));
	}
	
	// 전송
	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_IC_GUILD_GET_APPLICANT_OK_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;
	
	vectSGUILD_APPLICANT_INFO::iterator itr = vectGuildApplicantInfo.begin();
	for (; itr != vectGuildApplicantInfo.end(); itr++)
	{
		if(nSendBytes + MSG_SIZE(MSG_IC_GUILD_GET_APPLICANT_OK) > SIZE_MAX_PACKET)
		{
			pIISock->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_IC_GUILD_GET_APPLICANT_OK;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_IC_GUILD_GET_APPLICANT_OK *pRMsg	= (MSG_IC_GUILD_GET_APPLICANT_OK*)(SendBuf + nSendBytes);
		pRMsg->CharacterUID		= itr->CharacterUID;
		util::strncpy(pRMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
		pRMsg->UnitKind			= itr->UnitKind;
		pRMsg->Level			= itr->Level;
		nSendBytes				+= sizeof(MSG_IC_GUILD_GET_APPLICANT_OK);
	}

	if(nSendBytes > 0)
	{
		pIISock->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	pIISock->SendAddMessageType(T_IC_GUILD_GET_APPLICANT_OK_DONE);	

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildGetIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildGetIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildGetIntroduction
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetGuildIntroduction
	-- DESC				: 여단 소개 가져오기
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetGuildIntroduction
		@i_GuildUID					INT
	AS
		SELECT Content, WriteDate FROM dbo.td_GuildIntroduction WITH (NOLOCK) WHERE GuildUID = @i_GuildUID
	GO
	**************************************************************************/
	CIMIOCPSocket *pIISock = (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}

	SQLINTEGER arrCB[2]={SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.GuildUniqueNumber, 0,			&arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0232, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetGuildIntroduction Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}	

	SQLINTEGER cb1, cb2;
	SGUILD_INTRODUCTION	GuildIntroduction;
	util::zero(&GuildIntroduction, sizeof(SGUILD_INTRODUCTION));
	SQL_TIMESTAMP_STRUCT	RegDate; memset(&RegDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));

	SQLBindCol(hstmt,  1, SQL_C_CHAR, GuildIntroduction.IntroductionContent, SIZE_MAX_NOTICE, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_TIMESTAMP, &RegDate, 0, &cb2);

	// fetching
	ret = SQLFetch(hstmt);
	if(SQL_NO_DATA != ret)
	{
		GuildIntroduction.RegDate	= RegDate;
	}

	mt_auto_lock mtG(&m_pIMIOCP5->m_MapGuild);
	CGuild *pGuild = m_pIMIOCP5->m_MapGuild.findLock(pIISock->m_character.GuildUniqueNumber);
	if (NULL == pGuild) 
	{// 여단 정보가 없다
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	pGuild->SetGuildIntroduction(&GuildIntroduction);

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildDeleteIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildDeleteIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildDeleteIntroduction
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteGuildIntroduction
	-- DESC				: 여단 소개 삭제
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteGuildIntroduction
		@i_GuildUID		INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE GuildUID = @i_GuildUID
		DELETE FROM dbo.td_GuildIntroduction WHERE GuildUID = @i_GuildUID
	GO
	**************************************************************************/
	UID32_t	GuildUID				= (UID32_t)q.pGeneralParam;
	SQLINTEGER arrCB[2]={SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &GuildUID, 0,			&arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0239, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret!=SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteGuildIntroduction Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GetSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 지원자 소개서
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GetSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GetSelfIntroduction
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_GetSelfIntroduction
	-- DESC				: 자기 소개서 가져오기
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_GetSelfIntroduction
		@i_CharacterUID					INT
	AS
		SELECT G.GuildName, S.Content FROM dbo.td_Guild AS G WITH (NOLOCK) INNER JOIN dbo.td_SelfIntroduction AS S WITH (NOLOCK)
			ON G.GuildUniqueNumber = S.GuildUID
			WHERE CharacterUID = @i_CharacterUID
	GO
	**************************************************************************/
	UID32_t	CharacterUID	= (UID32_t)q.pGeneralParam;
	CIMIOCPSocket *pIISock	= (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}
	SQLINTEGER arrCB[2]={SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CharacterUID, 0,			&arrCB[1]);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0240, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_GetSelfIntroduction Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER cb1, cb2;
	INIT_MSG_WITH_BUFFER(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK, T_IC_GUILD_GET_SELF_INTRODUCTION_OK, msg, SendBuf);
	SQLBindCol(hstmt,  1, SQL_C_CHAR, msg->GuildName, SIZE_MAX_GUILD_NAME, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, msg->SelfIntroduction, SIZE_MAX_NOTICE, &cb2);

	// fetching
	ret = SQLFetch(hstmt);
	pIISock->SendAddData(SendBuf, MSG_SIZE(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK));

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildSearchIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개 검색
/// \author		dhjin
/// \date		2008-05-27 ~ 2008-05-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildSearchIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildSearchIntroduction
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteGuildIntroduction_By_PassedRangeDay
	-- DESC				: 지난 여단 소개 삭제
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteGuildIntroduction_By_PassedRangeDay
		@i_PassedRangeDay	INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE GuildUID IN 
					(SELECT GuildUID FROM dbo.td_GuildIntroduction WITH (NOLOCK) WHERE (DATEDIFF(dd, SendDate, GETDATE()) > @i_PassedRangeDay )
		DELETE FROM dbo.td_GuildIntroduction WHERE (DATEDIFF(dd, SendDate, GETDATE()) > @i_PassedRangeDay
	GO
	**************************************************************************/
	INT PassedRangeDay = GUILDINTRODUCTION_PASSED_RANGE_DAY;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &PassedRangeDay, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0231, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt,"atum_DeleteGuildIntroduction_By_PassedRangeDay Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_SearchGuildIntroduction
	-- DESC				: 여단 소개 전체 가져오기
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_SearchGuildIntroduction
		@i_InfluenceType		TINYINT
	AS
		SELECT G.GuildUniqueNumber, G.GuildName, GI.WriteDate, GI.Content, C.CharacterName
		FROM dbo.td_Guild AS G WITH (NOLOCK) INNER JOIN dbo.td_GuildIntroduction AS GI WITH (NOLOCK)
				ON G.GuildUniqueNumber = GI.GuildUID 
			INNER JOIN dbo.td_character AS C WITH (NOLOCK)
				ON C.UniqueNumber = G.GuildCommanderUniqueNumber
		WHERE C.InfluenceType = @i_InfluenceType
	GO
	**************************************************************************/
	CIMIOCPSocket *pIISock	= (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pIISock->m_character.InfluenceType, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0242, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_SearchGuildIntroduction Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	
	SQLINTEGER cb1, cb2, cb3, cb4, cb5;
	SGUILD_SEARCH_INTRODUCTION	AllGuildIntroduction;
	util::zero(&AllGuildIntroduction, sizeof(SGUILD_SEARCH_INTRODUCTION));
	vectSGUILD_SEARCH_INTRODUCTION	vectAllGuildIntroduction;
	vectAllGuildIntroduction.clear();
	SQL_TIMESTAMP_STRUCT	WriteDate; memset(&WriteDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));
	SQLBindCol(hstmt,  1, SQL_C_ULONG, &AllGuildIntroduction.GuildUID, 0, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, AllGuildIntroduction.GuildName, SIZE_MAX_GUILD_NAME, &cb2);
	SQLBindCol(hstmt,  3, SQL_C_TIMESTAMP, &WriteDate, 0, &cb3);
	SQLBindCol(hstmt,  4, SQL_C_CHAR, AllGuildIntroduction.GuildIntroduction, SIZE_MAX_NOTICE, &cb4);
	SQLBindCol(hstmt,  5, SQL_C_CHAR, AllGuildIntroduction.CharacterName, SIZE_MAX_CHARACTER_NAME, &cb5);

	// fetching
	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		AllGuildIntroduction.WriteDate	= WriteDate;
		vectAllGuildIntroduction.push_back(AllGuildIntroduction);
		util::zero(&AllGuildIntroduction, sizeof(SGUILD_SEARCH_INTRODUCTION));
	}
	
	// 전송
	BYTE SendBuf[SIZE_MAX_PACKET];
	int			nSendBytes		= 0;
	*(MessageType_t*)(SendBuf)	= T_IC_GUILD_SEARCH_INTRODUCTION_OK_HEADER;
	nSendBytes					= SIZE_FIELD_TYPE_HEADER;

	vectSGUILD_SEARCH_INTRODUCTION::iterator itr = vectAllGuildIntroduction.begin();
	for (; itr != vectAllGuildIntroduction.end(); itr++)
	{
		if(nSendBytes + MSG_SIZE(MSG_IC_GUILD_SEARCH_INTRODUCTION_OK) > SIZE_MAX_PACKET)
		{
			pIISock->SendAddData(SendBuf, nSendBytes);
			nSendBytes					= 0;
		}

		*((MessageType_t*)(SendBuf + nSendBytes))	= T_IC_GUILD_SEARCH_INTRODUCTION_OK;
		nSendBytes									+= SIZE_FIELD_TYPE_HEADER;
		MSG_IC_GUILD_SEARCH_INTRODUCTION_OK *pRMsg	= (MSG_IC_GUILD_SEARCH_INTRODUCTION_OK*)(SendBuf + nSendBytes);
		util::strncpy(pRMsg->GuildName, itr->GuildName, SIZE_MAX_GUILD_NAME);
		util::strncpy(pRMsg->GuildIntroduction, itr->GuildIntroduction, SIZE_MAX_NOTICE);
		util::strncpy(pRMsg->CharacterName, itr->CharacterName, SIZE_MAX_CHARACTER_NAME);
		pRMsg->GuildUID			= itr->GuildUID;
		pRMsg->WriteDate		= itr->WriteDate;
		nSendBytes				+= sizeof(MSG_IC_GUILD_SEARCH_INTRODUCTION_OK);
	}

	if(nSendBytes > 0)
	{
		pIISock->SendAddData(SendBuf, nSendBytes);
		nSendBytes						= 0;
	}

	pIISock->SendAddMessageType(T_IC_GUILD_SEARCH_INTRODUCTION_OK_DONE);	

	SQLFreeStmt(hstmt, SQL_CLOSE);	

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildUpdateIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 여단 소개 작성
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildUpdateIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildUpdateIntroduction
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateGuildIntroduction
	-- DESC				: 여단 소개 업데이트
	-- // 2008-05-20 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateGuildIntroduction
		@i_GuildUID		INT,
		@i_WriteDate	VARCHAR(30),
		@i_Content		VARCHAR(512)
	AS
		DECLARE @CheckBit	 TINYINT
		SET @CheckBit = (SELECT count(*) FROM dbo.td_GuildIntroduction WITH (NOLOCK) WHERE GuildUID = @i_GuildUID)
		IF(0 = @CheckBit)
		BEGIN
			INSERT INTO dbo.td_GuildIntroduction (GuildUID, Content, WriteDate)
				VALUES (@i_GuildUID, @i_Content, @i_WriteDate)	
			RETURN
		END

		UPDATE dbo.td_GuildIntroduction SET Content = @i_Content, WriteDate = @i_WriteDate WHERE GuildUID = @i_GuildUID
	GO
	**************************************************************************/
	QPARAM_GUILD_UPDATE_INTRODUCTION *pGuildIntroduction = (QPARAM_GUILD_UPDATE_INTRODUCTION*)q.pQueryParam;
	CIMIOCPSocket *pIISock	= (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		util::del(pGuildIntroduction);
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.GuildUniqueNumber, 0, NULL);
	char RegDate[SIZE_MAX_SQL_DATETIME_STRING];
	pGuildIntroduction->WriteDate.GetSQLDateTimeString(RegDate, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, RegDate, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTICE, 0, pGuildIntroduction->GuildIntroduction, 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0243, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_UpdateGuildIntroduction Failed!\n", TRUE, q.pIOCPSocket);

		util::del(pGuildIntroduction);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	pIISock->SendAddMessageType(T_IC_GUILD_UPDATE_INTRODUCTION_OK);

	util::del(pGuildIntroduction);
	SQLFreeStmt(hstmt, SQL_CLOSE);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildUpdateSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 자기 소개 작성 
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildUpdateSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_UpdateSelfIntroduction
	-- DESC				: 자기 소개서 업데이트
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--						-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - Result Set ==> ErrorFlag
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateSelfIntroduction
		@i_TargetGuildUID				INT,
		@i_CharacterUID					INT,
		@i_Content						VARCHAR(512)
	AS
		DECLARE @CheckBit	 TINYINT
		SET @CheckBit = (SELECT count(*) FROM dbo.td_SelfIntroduction WITH (NOLOCK) WHERE CharacterUID = @i_CharacterUID)
		IF(0 = @CheckBit)
		BEGIN
			INSERT INTO dbo.td_SelfIntroduction (GuildUID, CharacterUID, Content)
				VALUES (@i_TargetGuildUID, @i_CharacterUID, @i_Content)	
			-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
			--RETURN 1;	
			SELECT 1;
			RETURN;
		END
		ELSE IF(2 <= @CheckBit)
		BEGIN
			-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
			--RETURN 0;
			SELECT 0;
			RETURN;
		END

		UPDATE dbo.td_SelfIntroduction SET Content = @i_Content WHERE GuildUID = @i_TargetGuildUID AND CharacterUID = @i_CharacterUID
		-- // 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
		--RETURN 1;
		SELECT 1;
	GO
	**************************************************************************/
	QPARAM_GUILD_UPDATE_SELFINTRODUCTION *pSelfIntroduction = (QPARAM_GUILD_UPDATE_SELFINTRODUCTION*)q.pQueryParam;
	CIMIOCPSocket *pIISock	= (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		util::del(pSelfIntroduction);
		return;
	}
// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
//	BYTE ErrCheck = 0;
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &ErrCheck, 0, NULL);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSelfIntroduction->TargetGuildUID, 0, NULL);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.CharacterUniqueNumber, 0, NULL);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTICE, 0, pSelfIntroduction->SelfIntroduction, 0, NULL);
// 	
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call dbo.atum_UpdateSelfIntroduction(?, ?, ?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
// 	{
// 		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_UpdateSelfIntroduction Failed!\n", TRUE, q.pIOCPSocket);
// 
// 		util::del(pSelfIntroduction);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		return;
// 	}
// 
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );

	///////////////////////////////////////////////////////////////////////////////
	// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 
	BYTE ErrCheck = FALSE;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pSelfIntroduction->TargetGuildUID, 0, NULL);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.CharacterUniqueNumber, 0, NULL);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTICE, 0, pSelfIntroduction->SelfIntroduction, 0, NULL);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0280, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_UpdateSelfIntroduction Failed!\n", TRUE, q.pIOCPSocket);

		util::del(pSelfIntroduction);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UTINYINT, &(ErrCheck), 0, NULL);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	
	if(FALSE == ErrCheck)
	{
		pIISock->SendErrorMessage(T_IC_GUILD_UPDATE_SELFINTRODUCTION, ERR_ALREADY_REG);
	}
	else
	{
		pIISock->SendAddMessageType(T_IC_GUILD_UPDATE_SELFINTRODUCTION_OK);
	}
	
	util::del(pSelfIntroduction);
	SQLFreeStmt(hstmt, SQL_CLOSE);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildDeleteSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 자기 소개 지우기 
/// \author		dhjin
/// \date		2008-05-28 ~ 2008-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildDeleteSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildDeleteSelfIntroduction
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteSelfIntroduction
	-- DESC				: 자기 소개서 삭제
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteSelfIntroduction
		@i_CharacterUID					INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE CharacterUID = @i_CharacterUID
	GO
	**********************************************************************/
	CIMIOCPSocket *pIISock	= (CIMIOCPSocket*)q.pIOCPSocket; 
	if(NULL == pIISock
		|| FALSE == pIISock->IsUsing())
	{
		return;
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pIISock->m_character.CharacterUniqueNumber, 0, NULL);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0213, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteSelfIntroduction Failed (4) !\r\n", TRUE, q.pIOCPSocket);
		pIISock->SendErrorMessage(T_IC_GUILD_DELETE_SELFINTRODUCTION, ERR_DB_NO_SUCH_DATA);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	pIISock->SendAddMessageType(T_IC_GUILD_DELETE_SELFINTRODUCTION_OK);

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_GuildDeleteSelfIntroductionOffUser(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		
/// \author		dhjin
/// \date		2008-06-13 ~ 2008-06-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_GuildDeleteSelfIntroductionOffUser(DB_QUERY q, SQLHSTMT hstmt)
{// QT_GuildDeleteSelfIntroductionOffUser
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_DeleteSelfIntroduction
	-- DESC				: 자기 소개서 삭제
	-- // 2008-05-26 by dhjin, EP3 - 여단 수정 사항
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_DeleteSelfIntroduction
		@i_CharacterUID					INT
	AS
		DELETE FROM dbo.td_SelfIntroduction WHERE CharacterUID = @i_CharacterUID
	GO
	**********************************************************************/
	UID32_t	CharacterUID				= (UID32_t)q.pGeneralParam;
	
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &CharacterUID, 0, NULL);	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0213, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_DeleteSelfIntroduction Failed (5) !\r\n", TRUE, NULL);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_InsertNotifyMsg(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		EP3 - 여단 수정 사항 - 통지 메세지 보내기
/// \author		dhjin
/// \date		2008-06-13 ~ 2008-06-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_InsertNotifyMsg(DB_QUERY q, SQLHSTMT hstmt)
{// QT_InsertNotifyMsg
	SNOTIFY_MSG						*pQPMsg = (SNOTIFY_MSG*)q.pQueryParam;

	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_InsertNotifyMsg
	-- DESC				: // 2007-11-28 by cmkwon, 통지시스템 구현 - dbo.atum_InsertNotifyMsg 추가
	--					  // 2007-12-20 by cmkwon, 통지시스템 버그 수정 - 추가된 NotifyMsgUID 를 리턴한다.
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_InsertNotifyMsg
		@i_CharacterUID				INT,							-- 통지 받을 CharacterUID
		@i_NotifyMsgType			TINYINT,						-- 통지 메시지 타입(NOTIFY_MSG_TYPE_XXX)
		@i_NotifyMsgString			VARCHAR(512),					-- 통지 메시지 스트링(512)
		@i_SenderCharacterUID		INT,							-- 통지를 보낸 CharacterUID
		@i_SenderCharacterName		VARCHAR(20)						-- 통지를 보낸 CharacterName
	AS
		INSERT INTO dbo.td_NotifyMsg(CharacterUID, NotifyMsgType, NotifyMsgString, SenderCharacterUID, SenderCharacterName)
		VALUES(@i_CharacterUID, @i_NotifyMsgType, @i_NotifyMsgString, @i_SenderCharacterUID, @i_SenderCharacterName)

		-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
		--RETURN SCOPE_IDENTITY();		-- // 2007-12-20 by cmkwon, 통지시스템 버그 수정 - 추가된 NotifyMsgUID 를 리턴한다.
		IF (@@ERROR <> 0)
		BEGIN
			SELECT 0;
			RETURN;
		END

		SELECT SCOPE_IDENTITY();
	GO
	************************************************************************/

	// 2007-12-20 by cmkwon, 통지시스템 버그 수정 - 추가된 NotifyMsgUID 를 리턴 받아서 클라이언트로 전송한다.
	SQLINTEGER	arrCB[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS};
// 2008-08-22 by cmkwon, MySQL 관련 소스 통합 - 빠진 부분 처리
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &pQPMsg->NotifyMsgUID, 0,									&arrCB[1]);
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,									&arrCB[2]);
// 	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->NotifyMsgType, 0,								&arrCB[3]);
// 	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTIFY_MSG_STRING, 0, pQPMsg->NotifyMsgString, 0,			&arrCB[4]);
// 	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->SenderCharacterUID, 0,								&arrCB[5]);
// 	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->SenderCharacterName, 0,		&arrCB[6]);
// 	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)"{? = call dbo.atum_InsertNotifyMsg(?,?,?,?,?)}", SQL_NTS);
// 	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
// 	{
// 		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt,"atum_InsertNotifyMsg_ Failed!\r\n", TRUE, NULL);
// 		SQLFreeStmt(hstmt, SQL_CLOSE);
// 		util::del(pQPMsg);
// 		return;
// 	}
// 	// Clear any result sets generated.
// 	while ( ( ret = SQLMoreResults(hstmt) ) != SQL_NO_DATA );		// 2007-12-20 by cmkwon, 통지시스템 버그 수정 - 
// 	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->CharacterUID, 0,									&arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_TINYINT, SQL_TINYINT, 0, 0, &pQPMsg->NotifyMsgType, 0,								&arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_NOTIFY_MSG_STRING, 0, pQPMsg->NotifyMsgString, 0,			&arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pQPMsg->SenderCharacterUID, 0,								&arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pQPMsg->SenderCharacterName, 0,		&arrCB[5]);
	
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0169, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt,"atum_InsertNotifyMsg_ Failed!\r\n", TRUE, NULL);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pQPMsg);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_UBIGINT, &(pQPMsg->NotifyMsgUID), 0, NULL);
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(hstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(hstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	util::del(pQPMsg);
}


// QT_FriendInsertFriend
void CAtumIMDBManager::QP_FriendInsertFriend(DB_QUERY q, SQLHSTMT hstmt)
{
	CIMIOCPSocket	*pIMSocket = (CIMIOCPSocket*)q.pIOCPSocket;
	DB_FRIEND_INFO	*pDBFriendInfo = (DB_FRIEND_INFO*)q.pQueryParam;


	if(NULL == pIMSocket || FALSE == pIMSocket->IsValidCharacter())
	{
		util::del(pDBFriendInfo);
		return;
	}
	
	SQLRETURN ret;		
	SQLINTEGER arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};		// 2006-11-14 by cmkwon
//	char spbuf[2][SIZE_MAX_SQL_PATTERN_BUFFER];

	///////////////////////////////////////////////////////////////////////////
	// CharacterName이 DB에 존재하는지 검색
	char szCharacterName[SIZE_MAX_CHARACTER_NAME];
	USHORT usRace = 0;									// 2006-11-14 by cmkwon
	BYTE byInflTy = INFLUENCE_TYPE_UNKNOWN;				// 2006-10-17 by cmkwon
//	char sqlString[1024];
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//sprintf(sqlString, "SELECT CharacterName, Race, InfluenceType FROM td_character WITH (NOLOCK) WHERE CharacterName = \'%s\'", pDBFriendInfo->FriendName);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	sprintf(sqlString, QUERY_080702_0255, pDBFriendInfo->FriendName);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->FriendName, 0, NULL);
	ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0255), SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "Get CharacterName @ QP_FriendInsertFriend() Failed!\n", TRUE, q.pIOCPSocket);

		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBFriendInfo);
		return;
	}
	SQLBindCol(hstmt, 1, SQL_C_CHAR, szCharacterName, SIZE_MAX_CHARACTER_NAME,	&arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_USHORT, &usRace, 0,								&arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_UTINYINT, &byInflTy, 0,							&arrCB[3]);

	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);
	if(ret == SQL_NO_DATA)
	{// 존재하지 않는 CharacterName이다
		
		pIMSocket->SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME, 0, 0, pDBFriendInfo->FriendName);			
		util::del(pDBFriendInfo);
		return;
	}
	// CharacterName이 DB에 존재하는지 검색_end
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// 2006-11-14 by cmkwon
	if (FALSE == COMPARE_RACE(pIMSocket->GetCharacter()->Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR)
		&& COMPARE_RACE(usRace, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{// 2006-11-14 by cmkwon, 일반 유저는 관리자,운영자를 친구로 등록해도 Online/Offline 확인 불가, 추가 경험치 불가
		pIMSocket->SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_INVALID_CHARACTER_NAME);			
		util::del(pDBFriendInfo);
		return;
	}

	// 2006-10-17 by cmkwon, 두 캐릭터의 세력이 같은지 체크
	/*if(pIMSocket->GetCharacter()->InfluenceType != byInflTy)
	{// 2006-10-17 by cmkwon, 서로 다른 세력은 친구 추가 불가
		pIMSocket->SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED, 0, 0, pDBFriendInfo->FriendName);			
		util::del(pDBFriendInfo);
		return;
	}*/

	///////////////////////////////////////////////////////////////////////////////
	// 추가할 캐릭터명을 친구/거부에 등록되어 있는지 체크
	//	- 등록되어 있다면 그냥 리턴(두번 추가 요청이 된경우)
	switch(pDBFriendInfo->FriendType)
	{
	case FRIEND_TYPE_FRIEND:
		{
			if (pIMSocket->GetCharacter()->InfluenceType != byInflTy)
			{// 2006-10-17 by cmkwon, 서로 다른 세력은 친구 추가 불가
				pIMSocket->SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_PROTOCOL_REQ_INFLUENCE_NOT_MATCHED, 0, 0, pDBFriendInfo->FriendName);
				util::del(pDBFriendInfo);
				return;
			}
			if(pIMSocket->FindFriendInfoByCharacterName(pDBFriendInfo->FriendName))
			{
				util::del(pDBFriendInfo);
				return;
			}
		}
		break;
	case FRIEND_TYPE_REJECT:
		{
			if(pIMSocket->IsCharacterNameFromRejectList(pDBFriendInfo->FriendName))
			{
				util::del(pDBFriendInfo);
				return;
			}
		}
		break;
	default:
		{
			util::del(pDBFriendInfo);
			return;
		}
	}


	///////////////////////////////////////////////////////////////////////////////
	// 친구 추가는 Offline에서도 가능하므로 Offline인 사용자는 상대방 거부목록을 DB에서 검색 해야한다
	if(FRIEND_TYPE_FRIEND == pDBFriendInfo->FriendType
		&& NULL == m_pIMIOCP5->GetIMIOCPSocketByCharacterName(pDBFriendInfo->FriendName))
	{
		///////////////////////////////////////////////////////////////////////////
		// 상대방 거부리스트에 등록되어있는지의 여부 검색
		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
		//sprintf(sqlString, "SELECT CharacterName FROM td_FriendList WITH (NOLOCK) WHERE CharacterName LIKE \'%s\' AND FriendName LIKE \'%s\' AND Type = %d"
		//	, GetSqlPattern(pDBFriendInfo->FriendName, spbuf[0]), GetSqlPattern(pDBFriendInfo->CharacterName, spbuf[1]), FRIEND_TYPE_REJECT);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//		sprintf(sqlString, QUERY_080702_0256, GetSqlPattern(pDBFriendInfo->FriendName, spbuf[0]), GetSqlPattern(pDBFriendInfo->CharacterName, spbuf[1]), FRIEND_TYPE_REJECT);
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->FriendName, 0, NULL);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->CharacterName, 0, NULL);
		BYTE tmFriendType = FRIEND_TYPE_REJECT;
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &tmFriendType, 0, NULL);

		arrCB[1] = arrCB[2] = arrCB[3] = SQL_NTS;		// 2006-11-14 by cmkwon
		ret = SQLExecDirect(hstmt, (UCHAR*)(PROCEDURE_080827_0256), SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "Get CharacterName1 @ QP_FriendInsertFriend() Failed!\n", TRUE, q.pIOCPSocket);
			
			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pDBFriendInfo);
			return;
		}
		SQLBindCol(hstmt,  1, SQL_C_CHAR, szCharacterName, SIZE_MAX_CHARACTER_NAME, &arrCB[1]);

		ret = SQLFetch(hstmt);
		if(ret != SQL_NO_DATA)
		{// 상대방 거부리스트에 등록되어있음
			
			pIMSocket->SendErrorMessage(T_IC_CHAT_FRIENDLIST_INSERT, ERR_FRIEND_REGISTERED_PEER_REJECTLIST);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			util::del(pDBFriendInfo);
			return;
		}
		SQLFreeStmt(hstmt, SQL_CLOSE);			
		// 상대방 거부리스트에 등록되어있는지의 여부 검색_end
		///////////////////////////////////////////////////////////////////////////
	}

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_FriendInsertFriend
		@i_CharacterName				CHAR(20),
		@i_FriendName					CHAR(20),
		@i_Type							TINYINT,
		@i_RegDate						VARCHAR(30)
	AS
		INSERT INTO td_FriendList
		VALUES (@i_CharacterName, @i_FriendName, @i_Type, @i_RegDate)
	GO
	**************************************************************************/
	arrCB[1] = arrCB[2] = arrCB[3] = arrCB[4] = SQL_NTS;	// 2008-04-11 by dhjin, EP3 거부목록 -	// 2006-11-14 by cmkwon
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->CharacterName, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->FriendName, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pDBFriendInfo->FriendType, 0, &arrCB[3]);
	// 2008-04-11 by dhjin, EP3 거부목록 -
	char RegDate[SIZE_MAX_SQL_DATETIME_STRING];
	pDBFriendInfo->RegDate.GetSQLDateTimeString(RegDate, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, RegDate, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0244, SQL_NTS);	// 2008-04-11 by dhjin, EP3 거부목록 -
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_FriendInsertFriend Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBFriendInfo);		
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);	// cleanup
	
	if(pIMSocket->IsValidCharacter())
	{
		FRIENDINFO	tmFriend;
		util::zero(&tmFriend, sizeof(FRIENDINFO));
		util::strncpy(tmFriend.szCharacterName, pDBFriendInfo->FriendName, SIZE_MAX_CHARACTER_NAME);
		tmFriend.RegDate	= pDBFriendInfo->RegDate;	// 2008-04-11 by dhjin, EP3 거부목록 -

		BYTE	tmSendBuf[SIZE_MAX_PACKET];
		if(FRIEND_TYPE_FRIEND == pDBFriendInfo->FriendType)
		{
			tmFriend.byIsOnline = FALSE;
			BOOL bEachOtherFriend = FALSE;
			CIMIOCPSocket *pIISockFriend = m_pIMIOCP5->GetIMIOCPSocketByCharacterName(tmFriend.szCharacterName);
			if(pIISockFriend
				&& pIISockFriend->IsValidCharacter())
			{
				tmFriend.byIsOnline = TRUE;
				bEachOtherFriend	= pIISockFriend->IncreaseOnlineEachOtherFriendCnts(pDBFriendInfo->CharacterName);		
				if(bEachOtherFriend)
				{
					///////////////////////////////////////////////////////////////////////////////
					// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
					pIISockFriend->SendOnlineEachOtherFriendCntsToFieldServer();
				}
			}
			pIMSocket->InsertToFriendList(&tmFriend, bEachOtherFriend);
			
			/////////////////////////////////////////////////////////////////////////////// 
			// 클라이언트에 T_IC_CHAT_FRIENTLIST_INSERT_OK 전송			
			INIT_MSG(MSG_IC_CHAT_FRIENDLIST_INSERT_OK, T_IC_CHAT_FRIENDLIST_INSERT_OK, pSendMsg, tmSendBuf);
			util::strncpy(pSendMsg->szCharacterName, tmFriend.szCharacterName, SIZE_MAX_CHARACTER_NAME);
			pSendMsg->byIsOnline = tmFriend.byIsOnline;
			pIMSocket->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_INSERT_OK));

			if(bEachOtherFriend)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
				pIMSocket->SendOnlineEachOtherFriendCntsToFieldServer();
			}
		}
		else if(FRIEND_TYPE_REJECT == pDBFriendInfo->FriendType)
		{
			tmFriend.byIsOnline = FALSE;
			pIMSocket->InsertToRejectList(&tmFriend);
 
			/////////////////////////////////////////////////////////////////////////////// 
			// 클라이언트에 T_IC_CHAT_REJECTLIST_INSERT_OK 전송			
			INIT_MSG(MSG_IC_CHAT_REJECTLIST_INSERT_OK, T_IC_CHAT_REJECTLIST_INSERT_OK, pSendMsg, tmSendBuf);
			util::strncpy(pSendMsg->szCharacterName, tmFriend.szCharacterName, SIZE_MAX_CHARACTER_NAME);
			pSendMsg->RegDate		= tmFriend.RegDate;		// 2008-04-11 by dhjin, EP3 거부목록 -
			pIMSocket->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_REJECTLIST_INSERT_OK));
		}
	}
	
	util::del(pDBFriendInfo);
}

// QT_FriendDeleteFriend
void CAtumIMDBManager::QP_FriendDeleteFriend(DB_QUERY q, SQLHSTMT hstmt)
{
	DB_FRIEND_INFO	*pDBFriendInfo = (DB_FRIEND_INFO*)q.pQueryParam;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_FriendDeleteFriend
		@i_CharacterName				VARCHAR(20),	-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 64를 수정
		@i_FriendName					VARCHAR(20),	-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 64를 수정
		@i_Type							TINYINT
	AS
		DELETE FROM td_FriendList
		WHERE CharacterName = @i_CharacterName AND FriendName = @i_FriendName AND Type = @i_Type
		-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 
		-- WHERE CharacterName like @i_CharacterName AND FriendName like @i_FriendName AND Type = @i_Type
	GO
	**************************************************************************/

// 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 아래와 같이 수정 함.
// 	char spbuf[2][SIZE_MAX_SQL_PATTERN_BUFFER];
// 	SQLINTEGER cb1, cb2;
// 	cb1 = SQL_NTS;
// 	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER
// 		, 0, GetSqlPattern(pDBFriendInfo->CharacterName, spbuf[0]), 0, &cb1);
// 	cb2 = SQL_NTS;
// 	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER
// 		, 0, GetSqlPattern(pDBFriendInfo->FriendName, spbuf[1]), 0, &cb2);
	SQLINTEGER cb1, cb2;
	cb1 = cb2 = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->CharacterName, 0, &cb1);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->FriendName, 0, &cb2);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &pDBFriendInfo->FriendType, 0, NULL);

	/////////////////////////////////////////////////////////////////////////////// 
	// 1. DB에서 삭제한다.
	SQLRETURN ret = SQLExecDirect(hstmt, PROCEDURE_080822_0245, SQL_NTS);
	if(ret == SQL_NO_DATA)
	{
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBFriendInfo);
		return;
	}
	else if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_FriendDeleteFriend Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBFriendInfo);
		return;
	}
	// cleanup
	SQLFreeStmt(hstmt, SQL_CLOSE);

	CIMIOCPSocket *pIMSocket = m_pIMIOCP5->GetIMIOCPSocketByCharacterName(pDBFriendInfo->CharacterName);
	if(pIMSocket
		&& pIMSocket->IsValidCharacter())
	{
		BYTE tmSendBuf[SIZE_MAX_PACKET];
		/////////////////////////////////////////////////////////////////////////////// 
		// 2. 리스트에서 삭제하고 삭제성공 메세지를 전송한다
		if(FRIEND_TYPE_FRIEND == pDBFriendInfo->FriendType)
		{
			BOOL bEachOtherFriend = FALSE;
			CIMIOCPSocket *pIISockFriend = m_pIMIOCP5->GetIMIOCPSocketByCharacterName(pDBFriendInfo->FriendName);
			if(pIISockFriend
				&& pIISockFriend->IsValidCharacter())
			{// 2006-07-19 by cmkwon, 서로 등록 여부 체크
				bEachOtherFriend = pIISockFriend->DecreaseOnlineEachOtherFriendCnts(pDBFriendInfo->CharacterName);
				if(bEachOtherFriend)
				{
					///////////////////////////////////////////////////////////////////////////////
					// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
					pIISockFriend->SendOnlineEachOtherFriendCntsToFieldServer();
				}
			}
			pIMSocket->DeleteFromFriendList(pDBFriendInfo->FriendName, bEachOtherFriend);

			INIT_MSG(MSG_IC_CHAT_FRIENDLIST_DELETE_OK, T_IC_CHAT_FRIENDLIST_DELETE_OK, pSendMsg, tmSendBuf);
			util::strncpy(pSendMsg->szCharacterName, pDBFriendInfo->FriendName, SIZE_MAX_CHARACTER_NAME);
			pIMSocket->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_FRIENDLIST_DELETE_OK));

			if(bEachOtherFriend)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, 서로 등록한 온라인 친구 카운트 FieldServer로 전송
				pIMSocket->SendOnlineEachOtherFriendCntsToFieldServer();
			}
		}
		else if(FRIEND_TYPE_REJECT == pDBFriendInfo->FriendType)
		{
			pIMSocket->DeleteFromRejectList(pDBFriendInfo->FriendName);

			INIT_MSG(MSG_IC_CHAT_REJECTLIST_DELETE_OK, T_IC_CHAT_REJECTLIST_DELETE_OK, pSendMsg, tmSendBuf);
			util::strncpy(pSendMsg->szCharacterName, pDBFriendInfo->FriendName, SIZE_MAX_CHARACTER_NAME);

			pIMSocket->SendAddData(tmSendBuf, MSG_SIZE(MSG_IC_CHAT_REJECTLIST_DELETE_OK));
		}
	}

	util::del(pDBFriendInfo);
}

///////////////////////////////////////////////////////////////////////////////
// 2009-02-26 by cmkwon, 친구리스트 공백 관련 버그 수정 - 
struct find_if_FRIENDINFO_BY_FriendName
{
	find_if_FRIENDINFO_BY_FriendName(char *i_szFName)
	{
		util::strncpy(m_szFriendName, i_szFName, SIZE_MAX_CHARACTER_NAME);
	};
	bool operator()(const FRIENDINFO fInfo)
	{
		if(0 != strncmp(m_szFriendName, fInfo.szCharacterName, SIZE_MAX_SQL_DATETIME_STRING))
		{
			return false;
		}
		
		return true;
	}
	char m_szFriendName[SIZE_MAX_CHARACTER_NAME];
};

// QT_FriendLoadFriends
void CAtumIMDBManager::QP_FriendLoadFriends(DB_QUERY q, SQLHSTMT hstmt)
{
	BOOL bFirstTimeLoadFriendList	= q.nGeneralParam1;			// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 
	DB_FRIEND_INFO	*pDBFriendInfo = (DB_FRIEND_INFO*)q.pQueryParam;
	CIMIOCPSocket *pSocket = m_pIMIOCP5->GetIMIOCPSocketByCharacterName(pDBFriendInfo->CharacterName);
	if (pSocket == NULL)
	{
		util::del(pDBFriendInfo);
		return;
	}
	SQLRETURN		ret;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_FriendLoadFriends
		@i_CharacterName				VARCHAR(20)		-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 64를 수정
	AS
		SELECT *
		FROM td_FriendList WITH (NOLOCK)
		WHERE CharacterName = @i_CharacterName
		-- // 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 
		-- WHERE CharacterName like @i_CharacterName
	GO
	**************************************************************************/

// 2009-01-29 by cmkwon, 문자열 쿼리 검색시 필요없는 LIKE 사용을 제거 - 아래와 같이 수정
//	char spbuf[SIZE_MAX_SQL_PATTERN_BUFFER];
//	SQLINTEGER pcb = SQL_NTS;
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_PATTERN_BUFFER
//		, 0, GetSqlPattern(pDBFriendInfo->CharacterName, spbuf), 0, &pcb);
	SQLINTEGER pcb = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, pDBFriendInfo->CharacterName, 0, &pcb);

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0246, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_FriendLoadFriends Failed!\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pDBFriendInfo);
		return;
	}

	// bind columns
	SQLINTEGER cb1, cb2, cb3, cb4;		// 2008-04-11 by dhjin, EP3 거부목록 -
	DB_FRIEND_INFO	tmpFriendInfo;
	SQL_TIMESTAMP_STRUCT	RegDate; memset(&RegDate, 0x00, sizeof(SQL_TIMESTAMP_STRUCT));	// 2008-04-11 by dhjin, EP3 거부목록 -

	// check: 친구 구조체로 대체하기, 20041013, kelovon
	//////////////////////////////////////////////////////
	SQLBindCol(hstmt,  1, SQL_C_CHAR, tmpFriendInfo.CharacterName, SIZE_MAX_CHARACTER_NAME, &cb1);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, tmpFriendInfo.FriendName, SIZE_MAX_CHARACTER_NAME, &cb2);
	SQLBindCol(hstmt,  3, SQL_C_UTINYINT, &tmpFriendInfo.FriendType, 0, &cb3);
	SQLBindCol(hstmt,  4, SQL_C_TIMESTAMP, &RegDate, 0, &cb4);		// 2008-04-11 by dhjin, EP3 거부목록 -

	char seps[] = " ";
	char *token = NULL;
	FRIENDINFO				FriendInfo;
	mtvectorFRIENDINFO		tmpFriendList, tmpRejectList;
	tmpFriendList.reserve(10);
	tmpRejectList.reserve(10);
	// fetching
	util::zero(&tmpFriendInfo, sizeof(DB_FRIEND_INFO));
	while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
	{
		// check: 리스트에 삽입, 20041013, kelovon
		
		// 2009-02-26 by cmkwon, 친구리스트 공백 관련 버그 수정 - 원본 캐릭터명을 임시 저장한다.
		mtvectorFRIENDINFO::iterator itrFCheck;
		char szTmFName[SIZE_MAX_CHARACTER_NAME];
		util::strncpy(szTmFName, tmpFriendInfo.FriendName, SIZE_MAX_CHARACTER_NAME);

		strtok(tmpFriendInfo.FriendName, seps);
		util::strncpy(FriendInfo.szCharacterName, tmpFriendInfo.FriendName, SIZE_MAX_CHARACTER_NAME);
		FriendInfo.byIsOnline = FALSE;
		FriendInfo.RegDate = RegDate;		// 2008-04-11 by dhjin, EP3 거부목록 -

		switch(tmpFriendInfo.FriendType)
		{
		case FRIEND_TYPE_FRIEND:

			// 2009-02-26 by cmkwon, 친구리스트 공백 관련 버그 수정 - 자신 동록이거나, 이미 등록되어 있는지 체크해서 이미 등록된 친구는 원본으로 제거를 요청한다.
			itrFCheck = find_if(tmpFriendList.begin(), tmpFriendList.end(), find_if_FRIENDINFO_BY_FriendName(FriendInfo.szCharacterName));
			if(itrFCheck != tmpFriendList.end()				
				|| 0 == strnicmp(pDBFriendInfo->CharacterName, FriendInfo.szCharacterName, SIZE_MAX_CHARACTER_NAME))
			{
				///////////////////////////////////////////////////////////////////////////
				// 친구리스트에서 삭제 쿼리
				DB_FRIEND_INFO	*pDBFInfo = new DB_FRIEND_INFO;
				util::strncpy(pDBFInfo->CharacterName, pDBFriendInfo->CharacterName, SIZE_MAX_CHARACTER_NAME);
				util::strncpy(pDBFInfo->FriendName, szTmFName, SIZE_MAX_CHARACTER_NAME);
				pDBFInfo->FriendType = FRIEND_TYPE_FRIEND;
				this->MakeAndEnqueueQuery(QT_FriendDeleteFriend, pSocket, pDBFInfo);
				continue;
			}

			tmpFriendList.push_back(FriendInfo);
			break;
		case FRIEND_TYPE_REJECT:

			// 2009-02-26 by cmkwon, 친구리스트 공백 관련 버그 수정 - 자신 동록이거나, 이미 등록되어 있는지 체크해서 이미 등록된 친구는 원본으로 제거를 요청한다.
			itrFCheck = find_if(tmpRejectList.begin(), tmpRejectList.end(), find_if_FRIENDINFO_BY_FriendName(FriendInfo.szCharacterName));
			if(itrFCheck != tmpRejectList.end()
				|| 0 == strnicmp(pDBFriendInfo->CharacterName, FriendInfo.szCharacterName, SIZE_MAX_CHARACTER_NAME))
			{
				///////////////////////////////////////////////////////////////////////////
				// 친구리스트에서 삭제 쿼리
				DB_FRIEND_INFO	*pDBFInfo = new DB_FRIEND_INFO;
				util::strncpy(pDBFInfo->CharacterName, pDBFriendInfo->CharacterName, SIZE_MAX_CHARACTER_NAME);
				util::strncpy(pDBFInfo->FriendName, szTmFName, SIZE_MAX_CHARACTER_NAME);
				pDBFInfo->FriendType = FRIEND_TYPE_REJECT;
				this->MakeAndEnqueueQuery(QT_FriendDeleteFriend, pSocket, pDBFInfo);
				continue;
			}

			tmpRejectList.push_back(FriendInfo);
			break;
		default:
			{
				DBGOUT("[ERROR] CAtumIMDBManager::QP_FriendLoadFriends Invalid FriendType (%d)\n", tmpFriendInfo.FriendType);
			}
		}
		util::zero(&tmpFriendInfo, sizeof(DB_FRIEND_INFO));
	}

	// cleanup
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if(pSocket->IsUsing())
	{
		pSocket->ResLoadingFriendsAndRejects(&tmpFriendList, &tmpRejectList, bFirstTimeLoadFriendList);		// 2009-03-05 by cmkwon, 통합 아레나 이용시 친구 리스트 보너스 경험치 관련 버그 수정 - 
	}

	util::del(pDBFriendInfo);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumIMDBManager::QP_ReloadAdminAutoNotice(DB_QUERY q, SQLHSTMT hstmt)
/// \brief		// 2009-01-14 by cmkwon, 운영자 자동 공지 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-01-15 ~ 2009-01-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumIMDBManager::QP_ReloadAdminAutoNotice(DB_QUERY q, SQLHSTMT hstmt)
{
	BOOL bStartInstantly = (BOOL)q.nGeneralParam1;

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_Load_AdminAutoNoticeInfo
	AS	
		SELECT UsingFlag,LoopTime,IntervalTime,EditorAccountName
		FROM atum2_db_account.dbo.td_AdminAutoNoticeInfo WITH(NOLOCK);
	GO
	**************************************************************************/

	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-15 by cmkwon, 자동 공지 초기화
	m_pIMIOCP5->m_pANoticeManager->ResetANoticeManager();

	SANoticeInfo			tmANoticeInfo;
	mtvectSANoticeString	tmVectNoticeStrList;
	util::zero(&tmANoticeInfo, sizeof(tmANoticeInfo));

	// 2009-01-15 by cmkwon, 운영자 자동 공지 정보 가져오기
	SQLRETURN ret = SQLExecDirect(hstmt, PROCEDURE_090115_0280, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_Load_AdminAutoNoticeInfo Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SQLINTEGER arrCB[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindCol(hstmt,  1, SQL_C_LONG, &tmANoticeInfo.UsingFlag, 0,								&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_LONG, &tmANoticeInfo.LoopSec, 0,								&arrCB[2]);
	SQLBindCol(hstmt,  3, SQL_C_LONG, &tmANoticeInfo.IntervalSec, 0,							&arrCB[3]);
	SQLBindCol(hstmt,  4, SQL_C_CHAR, tmANoticeInfo.EditorAccountName, SIZE_MAX_ACCOUNT_NAME,	&arrCB[4]);

	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);		// cleanup


	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_Load_AdminAutoNoticeString
	AS	
		SELECT NoticeStringIndex,NoticeString
		FROM atum2_db_account.dbo.td_AdminAutoNoticeString WITH(NOLOCK);
	GO
	**************************************************************************/
	// 2009-01-15 by cmkwon, 운영자 자동 공지 리스트 가져오기
	ret = SQLExecDirect(hstmt, PROCEDURE_090115_0281, SQL_NTS);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		ProcessLogMessagesIM(SQL_HANDLE_STMT, hstmt, "atum_Load_AdminAutoNoticeString Failed!\r\n", TRUE, q.pIOCPSocket);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	SANoticeString		tmNoticeStr;
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindCol(hstmt,  1, SQL_C_LONG, &tmNoticeStr.NoticeStringIndex, 0,								&arrCB[1]);
	SQLBindCol(hstmt,  2, SQL_C_CHAR, &tmNoticeStr.NoticeString, SIZE_MAX_ADMIN_NOTICE_STRING,			&arrCB[2]);

	do
	{
		util::zero(&tmNoticeStr, sizeof(tmNoticeStr));
		ret = SQLFetch(hstmt);
		if(SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			break;
		}
		tmVectNoticeStrList.push_back(tmNoticeStr);
	}while(TRUE);
	SQLFreeStmt(hstmt, SQL_CLOSE);		// cleanup

	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-15 by cmkwon, NoticeStringIndex로 정렬
	if(1 < tmVectNoticeStrList.size())
	{
		sort(tmVectNoticeStrList.begin(), tmVectNoticeStrList.end(), sort_SANoticeString_by_StrIdx());
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-01-15 by cmkwon, 자동 공지 초기화
	m_pIMIOCP5->m_pANoticeManager->InitANoticeManager(&tmANoticeInfo, &tmVectNoticeStrList, bStartInstantly);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumIMDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumIMDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - IMServer는 사용하는 쿼리가 없음
//	switch(i_qQuery->enumQueryType)
//	{
//	case :
//	}

	// error: no such DB query type
	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumIMDBManager::ProcessDynamicServerQuery# no such DB Query Type !! %d(%s)\r\n", i_qQuery->enumQueryType, GetDBQueryTypeString(i_qQuery->enumQueryType));
	return FALSE;
}


void CAtumIMDBManager::ProcessLogMessagesIM(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, char *logstring, int ConnInd, CIOCPSocket *pIOCPSocket)
{
	if(pIOCPSocket)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "=== %s's ProcessLogMessagesIM =====================\r\n", GetCharacterString(&((CIMIOCPSocket*)pIOCPSocket)->m_character, string()));
	}

	CAtumDBManager::ProcessLogMessages(plm_handle_type, plm_handle, logstring, ConnInd, pIOCPSocket);
}
