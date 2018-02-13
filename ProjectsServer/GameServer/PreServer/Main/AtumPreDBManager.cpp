#include "stdafx.h"
#include "AtumPreDBManager.h"
#include "PreIOCPSocket.h"
#include "PreIOCP.h"
#include "md5_lib_src.h"
#include "AtumError.h"
#include "PreGlobal.h"
#include "AccountBlockManager.h"
#include <iostream>


BOOL CAtumPreDBManager::ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog)		// 2013-06-20 by jhseol,bckim GLog 보완
{
	switch (dbquery.enumQueryType)
	{
	case QT_PRE_Login:			QP_PRE_Login(dbquery, hstmt);	break;
	case QT_BlockUser:			QP_BlockUser(dbquery, hstmt);					break;
	case QT_UnblockUser:		QP_UnblockUser(dbquery, hstmt);					break;
	case QT_LogTotalUser:		QP_LogTotalUser(dbquery, hstmt);				break;
	case QT_UpdateAccountLastGameEndDate:
								QP_UpdateAccountLastGameEndDate(dbquery, hstmt);break;		// 2012-01-13 by hskim, EP4 [동영상 1회 재생]
	case QT_LoadBlockedAccounts:QP_LoadBlockedAccounts(dbquery, hstmt);			break;		// 2016-01-09 Future, improved Account block loading
	case QT_LoadBlockedMACs:	QP_LoadBlockedMACs(dbquery, hstmt);				break;
	case QT_BlockMAC:			QP_BlockMAC(dbquery, hstmt);					break;
	case QT_UnblockMAC:			QP_UnblockMAC(dbquery, hstmt);					break;

	default:
		// error: no such DB query type
		DBGOUT("Error! No such DB query type! %d\n", dbquery.enumQueryType);
		return FALSE;
	}

	return TRUE;
}

// QT_PRE_Login
void CAtumPreDBManager::QP_PRE_Login(DB_QUERY q, SQLHSTMT &hstmt)
{
	auto pRMsg = static_cast<MSG_PC_CONNECT_LOGIN*>(q.pQueryParam);

	auto socket = static_cast<CPreIOCPSocket*>(q.pIOCPSocket);

	CAccountInfo accInfo;


	_strlwr_s(pRMsg->AccountName);			// 2006-06-02 by cmkwon, 소문자로 변환

	/* DevX considers the following code is gibberish
	if(g_pPreGlobal->IsExternalAuthentication() || g_pPreGlobal->GetIsExtAuthUseSOAP())
	{// 2006-04-27 by cmkwon, 외부 DB 인증 사용시 처리 추가
		INT		nExtAccountIDNum = 0;

		// 2008-10-08 by cmkwon, 대만 2단계 계정 시스템 지원 구현(email->uid) -
		char szOriginalAccountName[SIZE_MAX_ORIGINAL_ACCOUNT_NAME];
		util::strncpy(szOriginalAccountName, pRMsg->AccountName, SIZE_MAX_ORIGINAL_ACCOUNT_NAME);

		// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 -
		if(FALSE == this->ExternalAuthentication(&accInfo, pRMsg, hstmt, hstmt_extAuth, q.pIOCPSocket->GetPeerIP4()))
		{
			((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_COMMON_LOGIN_FAILED);
		}
		else
		{

			// 2012-10-07 by hskim, 휴식 경험치
			//[Stored Query Definition]************************************************
			//CREATE PROCEDURE [dbo].[atum_PreServerUpdateLastLoginDate]
			//	@i_accUID			INT
			//WITH EXECUTE AS 'proexe'	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) -
			//AS
			//BEGIN
			//	DECLARE @NowDate				DATETIME; SET @NowDate = GETDATE();
			//	DECLARE @DiffMin				INT
			//	DECLARE @RestExperienceCount	INT

			//	DECLARE @MinTime				INT; SET @MinTime = 300			-- 기본 값 설정	(최소 적용 분	: [300, 5시간])
			//	DECLARE @MaxCount				INT; SET @MaxCount = 3600		-- 기본 값 설정	(최대 적용 숫자	: [3600, 2분당 1마리, 총 5일, 3600마리)

			//	UPDATE dbo.td_account SET LastLoginDate = @NowDate WHERE AccountUniqueNumber = @i_accUID

			//	-- 현재 테이블에 값이 있는지 확인후에 없으면 종료
			//	SELECT @RestExperienceCount = RestExperienceCount FROM dbo.td_AdditionalExperience WITH (NOLOCK) WHERE AccountUniqueNumber = @i_accUID

			//	IF( @RestExperienceCount IS NULL )
			//	BEGIN
			//		EXEC [dbo].[atum_Set_AddExp_RestExperienceCount] @i_AccountUniqueNumber = @i_accUID, @i_RestExperienceCount = 0;

			//		RETURN
			//	END

			//	SELECT @DiffMin = DATEDIFF(minute, LastGameEndDate, @NowDate) FROM dbo.td_account WITH (NOLOCK) WHERE [AccountUniqueNumber] = @i_accUID;

			//	IF( @DiffMin IS NULL )
			//	BEGIN
			//		RETURN
			//	END

			//	IF( @DiffMin >= @MinTime )
			//	BEGIN
			//		SET @RestExperienceCount = @RestExperienceCount + (@DiffMin / 2)

			//		IF( @RestExperienceCount > @MaxCount )
			//		BEGIN
			//			SET @RestExperienceCount = @MaxCount
			//		END

			//		EXEC [dbo].[atum_Set_AddExp_RestExperienceCount] @i_AccountUniqueNumber = @i_accUID, @i_RestExperienceCount = @RestExperienceCount;
			//	END
			//END
		//	************************************************************************** /

			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &accInfo.AccountUniqueNumber, 0, NULL);
			SQLExecDirect(hstmt, PROCEDURE_080822_0266, SQL_NTS);
			SQLMoreResults(hstmt);
			SQLFreeStmt(hstmt, SQL_CLOSE);
			// end 2012-10-07 by hskim, 휴식 경험치

			((CPreIOCPSocket*)q.pIOCPSocket)->ResPreLogin(pRMsg, &accInfo, ERR_NO_ERROR);
		}
		util::del(pRMsg);
		return;
	}
	*/

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-11 by cmkwon, 러시아 Innova_Rus 인증 시스템 수정 - Usp_Do_Login() 에서 인증 성공시에만 내부인증 체크를 진행하도록 수정
	//							Innova에서 Usp_Do_Login() 내에서 atum2_db_account.dbo.td_account 테이블에 AccountName, Password를 추가 하고, Password 변경시에도 Innova에서 적용해주기로 함.
	RETCODE ret;
	SQLINTEGER arrCB[10] = { SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS };	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 날짜는 이전에 로그인한 날짜로딩을 위해 하나 추가
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pRMsg->AccountName, 0, &arrCB[0]);

#ifndef ARESBETA
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0265, SQL_NTS);
#else
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0265_BETA, SQL_NTS);
#endif

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_PreServerGetAccountInfo Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		socket->SendErrorMessage(T_PC_CONNECT_LOGIN, ERR_DB_EXECUTION_FAILED);
		util::del(pRMsg);
		return;
	}
	if (ret == SQL_NO_DATA)
	{// 인증 실패 (아이디가 없음)

		SQLFreeStmt(hstmt, SQL_CLOSE);
		socket->ResPreLogin(pRMsg, &accInfo, ERR_DB_NO_SUCH_ACCOUNT);
		util::del(pRMsg);
		return;
	}

	char szMD5String[SIZE_MAX_PASSWORD_MD5_STRING];			util::zero(szMD5String, SIZE_MAX_PASSWORD_MD5_STRING);
	MD5::MD5Binary2String(pRMsg->Password, szMD5String);
	SQL_TIMESTAMP_STRUCT		tmSqlTime;		util::zero(&tmSqlTime, sizeof(tmSqlTime));
	SQL_TIMESTAMP_STRUCT		tmSqlTime2;		util::zero(&tmSqlTime2, sizeof(tmSqlTime2));
	SQL_TIMESTAMP_STRUCT		tmSqlTime3;		util::zero(&tmSqlTime3, sizeof(tmSqlTime3));		// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 날짜는 이전에 로그인한 날짜	

	int nBirthYear = 0;		// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 출생년도(중국은 출생년월일)를 가져옴
	SQLBindCol(hstmt, 1, SQL_C_ULONG, &accInfo.AccountUniqueNumber, 0, &arrCB[1]);
	SQLBindCol(hstmt, 2, SQL_C_USHORT, &accInfo.AccountType, 0, &arrCB[2]);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, accInfo.PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &arrCB[3]);
	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &tmSqlTime, 0, &arrCB[4]);
	SQLBindCol(hstmt, 5, SQL_C_LONG, &accInfo.GameContinueTimeInSecondOfToday, 0, &arrCB[5]);		// 2006-11-15 by cmkwon
	SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &tmSqlTime2, 0, &arrCB[6]);		// 2006-11-15 by cmkwon
	SQLBindCol(hstmt, 7, SQL_C_LONG, &nBirthYear, 0, &arrCB[7]);		// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기)
	SQLBindCol(hstmt, 8, SQL_C_CHAR, accInfo.SecondaryPassword, SIZE_MAX_PASSWORD_MD5_STRING, &arrCB[8]);	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - SecondaryPassword 가져오기 추가
	SQLBindCol(hstmt, 9, SQL_C_TIMESTAMP, &tmSqlTime3, 0, &arrCB[9]);	// 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 날짜는 이전에 로그인한 날짜확인을 위해	

	while (SQL_NO_DATA != (ret = SQLFetch(hstmt)));
	SQLFreeStmt(hstmt, SQL_CLOSE);
	accInfo.AccountRegisteredDate = tmSqlTime;
	accInfo.LastGameEndDate = tmSqlTime2;			// 2006-11-15 by cmkwon
	accInfo.SetBirthday(nBirthYear);						// 2007-06-28 by cmkwon, 중국 방심취관련(출생년월일 FielServer로 가져오기) - 

	if (!IS_VALID_UNIQUE_NUMBER(accInfo.AccountUniqueNumber))
	{// 2006-06-02 by cmkwon, AccountUID 체크 - 인증 실패 (아이디가 없음)
		socket->ResPreLogin(pRMsg, &accInfo, ERR_DB_NO_SUCH_ACCOUNT);
		util::del(pRMsg);
		return;
	}

#ifdef ARESBETA
	if (accInfo.AccountType != 0)
#endif
		if (stricmp(szMD5String, accInfo.PasswordFromDB) != 0)
		{// 2006-06-02 by cmkwon, 비번 체크
			char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];
			BYTE byPass[SIZE_MAX_PASSWORD_MD5];
			MD5 md5_instance;
			char szTmPassword[1024];		util::zero(szTmPassword, 1024);
			wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, accInfo.PasswordFromDB);
			md5_instance.MD5Encode(szTmPassword, byPass);
			MD5::MD5Binary2String(byPass, szEncodedString);
			if (stricmp(szEncodedString, szMD5String) != 0)
			{
				socket->ResPreLogin(pRMsg, &accInfo, ERR_COMMON_LOGIN_FAILED);
				util::del(pRMsg);
				return;
			}
		}

	// start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 날짜는 이전에 로그인한 날짜	
	// 비교할 타임 값 쿼리 내부에서 값을 비교 처리한다.	
	ATUM_DATE_TIME	CheckDateTime;
	CheckDateTime.SetCurrentDateTime();

	if (CheckDateTime.Year == tmSqlTime3.year) //년도도 같아야하고.
	{
		if (CheckDateTime.Month == tmSqlTime3.month) //달도 같아야 한다.
		{
			if (CheckDateTime.Day != tmSqlTime3.day) //날짜가 오늘 같으면 루틴을 처리하지 않아야한다.
			{
				SQLRETURN retEv = 0;
				SQLINTEGER tmpNum = 1;
				SQLINTEGER tmpnum1 = 0;
				SQLINTEGER AccountUniqueNumber = accInfo.AccountUniqueNumber;
				SQLINTEGER	arrcb1[3] = { SQL_NTS,SQL_NTS,SQL_NTS };

				retEv = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmpNum, 0, &arrcb1[0]); // 현재는 디폴트로 사용함
				retEv = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &AccountUniqueNumber, 0, &arrcb1[1]);
				retEv = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &tmpnum1, 0, &arrcb1[2]); // 처음 삽입은 무조껀 0으로 한다.

				ret = SQLExecDirect(hstmt, PROCEDURE_LOGINITEMEVENT_110825_0001, SQL_NTS);
				if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
				{
					ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_InsertLoginItemEvent Failed!\n", TRUE);
					SQLFreeStmt(hstmt, SQL_CLOSE);
					// return; 리턴은 하지 말아야 한다. 인증에 실패하게 된다.
				}
			}
		}
	}

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &accInfo.AccountUniqueNumber, 0, NULL);
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0266, SQL_NTS);
	ret = SQLMoreResults(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	socket->ResPreLogin(pRMsg, &accInfo, ERR_NO_ERROR);

	util::del(pRMsg);
}



// QT_BlockUser
void CAtumPreDBManager::QP_BlockUser(DB_QUERY q, SQLHSTMT hstmt)
{
	SBLOCKED_ACCOUNT_INFO *pBlockedAccInfo = (SBLOCKED_ACCOUNT_INFO*)q.pQueryParam;
	/*[Stored Query Definition]************************************************
	-- 2005-06-02 by cmkwon
	-- 계정 블럭 리스트에 추가 혹은 정보 Update
	--			2007-01-10 by cmkwon, td_BlockedAccounts 테이블에 BlockedReasonForOnlyAdmin 필드 추가함
	CREATE PROCEDURE atum_Insert_BlockedAccounts
		@i_AccountName			VARCHAR(20),
		@i_BlockedType			INT,
		@i_StartDate			VARCHAR(30),
		@i_EndDate				VARCHAR(30),
		@i_AdminAccountName		VARCHAR(20),
		@i_BlockedReason		VARCHAR(200),			-- 유저가 보는 블럭 사유
		@i_BlockedReasonForOnlyAdmin	VARCHAR(200)	-- 2007-01-10 by cmkwon, 추가함 - 관리자가 보는 블럭사유
	AS
		DECLARE @blockedTy INT
		SET @blockedTy = (SELECT BlockedType FROM td_BlockedAccounts WHERE AccountName = @i_AccountName)

		IF (@blockedTy IS NULL)
			BEGIN
				INSERT INTO td_BlockedAccounts
					VALUES (@i_AccountName, @i_BlockedType, @i_StartDate, @i_EndDate, @i_AdminAccountName, @i_BlockedReason, @i_BlockedReasonForOnlyAdmin)
			END
		ELSE
			BEGIN
				UPDATE td_BlockedAccounts
					SET BlockedType = @i_BlockedType, StartDate = @i_StartDate
							, EndDate = @i_EndDate, AdminAccountName = @i_AdminAccountName, BlockedReason = @i_BlockedReason, BlockedReasonForOnlyAdmin = @i_BlockedReasonForOnlyAdmin
						WHERE AccountName = @i_AccountName
			END
	GO
	**************************************************************************/

	SQLINTEGER arrCB[8] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pBlockedAccInfo->szBlockedAccountName, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pBlockedAccInfo->enBlockedType, 0, &arrCB[2]);
	char tmpStartTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	pBlockedAccInfo->atimeStartTime.GetSQLDateTimeString(tmpStartTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpStartTimeBuf, 0, &arrCB[3]);
	char tmpEndTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	pBlockedAccInfo->atimeEndTime.GetSQLDateTimeString(tmpEndTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpEndTimeBuf, 0, &arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pBlockedAccInfo->szBlockAdminAccountName, 0, &arrCB[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, pBlockedAccInfo->szBlockedReasonForUser, 0, &arrCB[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, pBlockedAccInfo->szBlockedReasonForOnlyAdmin, 0, &arrCB[7]);		// 2007-01-10 by cmkwon

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0267, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@QP_BlockUser() Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pBlockedAccInfo);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 로그를 추가한다.
	if (!LogInsertBlockUnblock(hstmt, pBlockedAccInfo, T1_FL_LOG_BLOCK_ACCOUNT))
	{
		server::log(TRUE, "[Error] Insert log error!!, AccountName(%s) BlockType(%d) Period(%s~%s) AdminAccName(%s) ReasonForUser(%s) ReasonForAdmin(%s)\r\n"
			, pBlockedAccInfo->szBlockedAccountName, pBlockedAccInfo->enBlockedType, pBlockedAccInfo->atimeStartTime.GetDateTimeString().GetBuffer()
			, pBlockedAccInfo->atimeEndTime.GetDateTimeString().GetBuffer(), pBlockedAccInfo->szBlockAdminAccountName
			, pBlockedAccInfo->szBlockedReasonForUser, pBlockedAccInfo->szBlockedReasonForOnlyAdmin);
	}

	// 2008-04-04 by cmkwon, 요 메시지는 무조건 전송
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_BLOCK_ACCOUNT_OK, T_PA_ADMIN_BLOCK_ACCOUNT_OK, msgBlockOK, msgBlockOKBuf);
	*msgBlockOK = *pBlockedAccInfo;
	m_pPreIOCP->SendMessageToAdminTool(msgBlockOKBuf, MSG_SIZE(MSG_PA_ADMIN_BLOCK_ACCOUNT_OK));

	// Notify
	if (q.pIOCPSocket
		&& q.pIOCPSocket->IsUsing())
	{
		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 아래와 같이 SocketType 별로 처리
		if (ST_FIELD_SERVER == q.pIOCPSocket->m_PeerSocketType)
		{
			// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - FieldServer 로 결과 전송
			INIT_MSG_WITH_BUFFER(MSG_FP_ADMIN_BLOCKACCOUNT_OK, T_FP_ADMIN_BLOCKACCOUNT_OK, pSMsgOK, SendBuf);
			pSMsgOK->ErrCode = ERR_NO_ERROR;
			util::strncpy(pSMsgOK->BlockedAccName, pBlockedAccInfo->szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
			pSMsgOK->AdminFieldServerClientIndex = pBlockedAccInfo->AdminFieldServerClientIndex;
			util::strncpy(pSMsgOK->AdminAccName, pBlockedAccInfo->szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME);
			pSMsgOK->atimeEndTime = pBlockedAccInfo->atimeEndTime;
			q.pIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FP_ADMIN_BLOCKACCOUNT_OK));
		}
	}
	util::del(pBlockedAccInfo);
}

// QT_UnblockUser
void CAtumPreDBManager::QP_UnblockUser(DB_QUERY q, SQLHSTMT hstmt)
{
	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 아래와 같이 수정
	// 	char *szAccountName = (char*)q.pQueryParam;
	auto pBlockedAccInfo = reinterpret_cast<SBLOCKED_ACCOUNT_INFO*>(q.pQueryParam);

	/*[Stored Query Definition]************************************************
	-- 계정 블럭 리스트에서 제거
	CREATE PROCEDURE atum_Delete_BlockedAccounts
		@i_AccountName			VARCHAR(20)
	AS
		DELETE FROM td_BlockedAccounts
			WHERE AccountName = @i_AccountName
	GO
	**************************************************************************/

	SQLINTEGER cb = SQL_NTS;
	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 아래와 같이 수정
	//SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, szAccountName, 0, &cb);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pBlockedAccInfo->szBlockedAccountName, 0, &cb);

	// 2008-01-29 by cmkwon, T_A: 계정 블럭 관련 쿼리 수정 - 사용하지 않는 코드임
	//char szQuery[512];
	//sprintf(szQuery, "DELETE FROM td_BlockedAccounts WHERE AccountName LIKE \'%s\'", szAccountName);
	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0268, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@QP_UnblockUser() Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pBlockedAccInfo);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	///////////////////////////////////////////////////////////////////////////////	
	// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 로그를 추가한다.
	if (FALSE == this->LogInsertBlockUnblock(hstmt, pBlockedAccInfo, T1_FL_LOG_UNBLOCK_ACCOUNT))
	{
		server::log(TRUE, "[Error] Insert log error!!, AccountName(%s) BlockType(%d) Period(%s~%s) AdminAccName(%s) ReasonForUser(%s) ReasonForAdmin(%s)\r\n"
			, pBlockedAccInfo->szBlockedAccountName, pBlockedAccInfo->enBlockedType, pBlockedAccInfo->atimeStartTime.GetDateTimeString().GetBuffer()
			, pBlockedAccInfo->atimeEndTime.GetDateTimeString().GetBuffer(), pBlockedAccInfo->szBlockAdminAccountName
			, pBlockedAccInfo->szBlockedReasonForUser, pBlockedAccInfo->szBlockedReasonForOnlyAdmin);
	}

	// 2008-04-04 by cmkwon, 이 메시지는 무조건 전송 해야 한다.
	INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK, T_PA_ADMIN_UNBLOCK_ACCOUNT_OK, msgUnblockOK, msgUnblockOKBuf);
	util::strncpy(msgUnblockOK->AccountName, pBlockedAccInfo->szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
	m_pPreIOCP->SendMessageToAdminTool(msgUnblockOKBuf, MSG_SIZE(MSG_PA_ADMIN_UNBLOCK_ACCOUNT_OK));

	// Notify
	if (q.pIOCPSocket && q.pIOCPSocket->IsUsing())
	{
		// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - 아래와 같이 SocketType 별로 처리
		if (q.pIOCPSocket->m_PeerSocketType == ST_FIELD_SERVER)
		{
			// 2008-01-31 by cmkwon, 계정 블럭/해제 명령어로 가능한 시스템 구현 - FieldServer 로 결과 전송
			INIT_MSG_WITH_BUFFER(MSG_FP_ADMIN_UNBLOCKACCOUNT_OK, T_FP_ADMIN_UNBLOCKACCOUNT_OK, pSMsgOK, SendBuf);
			pSMsgOK->ErrCode = ERR_NO_ERROR;
			util::strncpy(pSMsgOK->UnblockedAccName, pBlockedAccInfo->szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME);
			pSMsgOK->AdminFieldServerClientIndex = pBlockedAccInfo->AdminFieldServerClientIndex;
			util::strncpy(pSMsgOK->AdminAccName, pBlockedAccInfo->szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME);
			q.pIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FP_ADMIN_UNBLOCKACCOUNT_OK));
		}
	}
	util::del(pBlockedAccInfo);
}

void CAtumPreDBManager::QP_LogTotalUser(DB_QUERY q, SQLHSTMT hstmt)
{
	MSG_DB_TOTAL_USER	*pRMsg = (MSG_DB_TOTAL_USER*)q.pQueryParam;

	RETCODE ret;
	SQLINTEGER arrCB[4] = { SQL_NTS, SQL_NTS, SQL_NTS, SQL_NTS };		// 2010-11 by dhjin, 아라리오 채널링 로그인.

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_log_insert_total_user
		@i_ServerGroupName			VARCHAR(20),
		@i_MGameServerID			INT,
		@i_UserCount				INT,
		@i_nUserCountsOtherPublisherConncect				INT
	AS
		INSERT INTO atum_log_total_user
		VALUES (GetDate(), @i_ServerGroupName, @i_MGameServerID, @i_UserCount, @i_nUserCountsOtherPublisherConncect)
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, pRMsg->szServerGroup, 0, &arrCB[0]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &pRMsg->nUserCounts, 0, &arrCB[1]);

	int nUserCountsOtherPublisherConncect = 0;
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &nUserCountsOtherPublisherConncect, 0, &arrCB[2]);		// 2010-11 by dhjin, 아라리오 채널링 로그인.

	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0269, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_total_user Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}

// start 2011-12-12 by hskim, GLog 2차
BOOL CAtumPreDBManager::IsExistAccount(char *pAccountName, SQLHSTMT &hstmt)
{
	CAccountInfo AccountInfo;
	SDWORD pcbNTS = SQL_NTS;

	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, pAccountName, 0, NULL);

	RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0258, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "CAtumPreDBManager::IsExistAccount PROCEDURE_080827_0258 Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}

	SQLBindCol(hstmt, 1, SQL_C_ULONG, &AccountInfo.AccountUniqueNumber, 0, NULL);
	SQLBindCol(hstmt, 2, SQL_C_USHORT, &AccountInfo.AccountType, 0, NULL);
	SQLBindCol(hstmt, 3, SQL_C_CHAR, AccountInfo.PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &pcbNTS);

	ret = SQLFetch(hstmt);
	SQLFreeStmt(hstmt, SQL_CLOSE);

	if (FALSE == IS_VALID_UNIQUE_NUMBER(AccountInfo.AccountUniqueNumber))
	{
		return FALSE;
	}

	return TRUE;
}
// end 2011-12-12 by hskim, GLog 2차

// start 2012-01-13 by hskim, EP4 [동영상 1회 재생]
void CAtumPreDBManager::QP_UpdateAccountLastGameEndDate(DB_QUERY q, SQLHSTMT hstmt)
{
	QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE *pRMsg = (QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE*)q.pQueryParam;

	RETCODE ret;

	/*[Stored Query Definition]************************************************
	CREATE PROC [dbo].[atum_Update_AccountLastGameEndDate]
		@i_AccountUniqueNumber		INT
	AS
	-- YD 보안 코드
	BEGIN
		UPDATE [dbo].[td_Account] SET [LastGameEndDate] = getdate() WHERE AccountUniqueNumber = @i_AccountUniqueNumber
	END
	GO
	**************************************************************************/
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pRMsg->AccountUniqueNumber, 0, NULL);

	ret = SQLExecDirect(hstmt, PROCEDURE_111222_0002, SQL_NTS);

	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "QP_UpdateAccountLastGameEndDate Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		util::del(pRMsg);
		return;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
	util::del(pRMsg);
}
// end 2012-01-13 by hskim, EP4 [동영상 1회 재생]

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::LogInsertBlockUnblock(SQLHSTMT hstmt, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo, BYTE i_byLogType)
/// \brief		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - CAtumPreDBManager::LogInsertBlockUnblock() 추가
/// \author		cmkwon
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::LogInsertBlockUnblock(SQLHSTMT hstmt, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo, T1<T0::FL_LOG> i_byLogType)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- Name: atum_log_insert_block_unblock
	-- Desc: // 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 계정 블럭/수정/해제 정보 로그 추가하기
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_log_insert_block_unblock
		@i_LogType				TINYINT,
		@i_AccountName			VARCHAR(20),
		@i_BlockedType			INT,
		@i_StartDate			VARCHAR(30),
		@i_EndDate				VARCHAR(30),
		@i_AdminAccountName		VARCHAR(20),
		@i_BlockedReason		VARCHAR(200),			-- 유저가 보는 블럭 사유
		@i_BlockedReasonForOnlyAdmin	VARCHAR(200)	-- 2007-01-10 by cmkwon, 추가함 - 관리자가 보는 블럭사유
	AS
		INSERT INTO dbo.atum_log_blockedAccount
		VALUES (@i_LogType, GetDate(), @i_AccountName, @i_BlockedType, @i_StartDate, @i_EndDate, @i_AdminAccountName, @i_BlockedReason, @i_BlockedReasonForOnlyAdmin)
	GO
	**************************************************************************/

	SQLINTEGER arrCB[9] = { SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS, SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS };
	char tmpStartTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	char tmpEndTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	i_pBlockAccInfo->atimeStartTime.GetSQLDateTimeString(tmpStartTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	i_pBlockAccInfo->atimeEndTime.GetSQLDateTimeString(tmpEndTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byLogType, 0, &arrCB[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pBlockAccInfo->szBlockedAccountName, 0, &arrCB[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pBlockAccInfo->enBlockedType, 0, &arrCB[3]);
	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpStartTimeBuf, 0, &arrCB[4]);
	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, tmpEndTimeBuf, 0, &arrCB[5]);
	SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pBlockAccInfo->szBlockAdminAccountName, 0, &arrCB[6]);
	SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, i_pBlockAccInfo->szBlockedReasonForUser, 0, &arrCB[7]);
	SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_BLOCKED_ACCOUNT_REASON, 0, i_pBlockAccInfo->szBlockedReasonForOnlyAdmin, 0, &arrCB[8]);		// 2007-01-10 by cmkwon

	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0270, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "@atum_log_insert_block_unblock_ Failed!\r\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return FALSE;
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CAtumPreDBManager::ExternalAuthentication(CAccountInfo *o_pAccInfo, MSG_PC_CONNECT_LOGIN *i_pLogin, SQLHSTMT hstmt, SQLHSTMT hstmt_extAuth, BOOL i_bAuthenticateCheck/*=TRUE*/)
///// \brief		
/////				// 2010-04-26 by cmkwon, 러시아 Innva 인증/빌링 시스템 변경 - 
///// \author		cmkwon
///// \date		2006-04-27 ~ 2006-04-27
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumPreDBManager::ExternalAuthentication(CAccountInfo *o_pAccInfo, MSG_PC_CONNECT_LOGIN *i_pLogin, SQLHSTMT &hstmt, SQLHSTMT &hstmt_extAuth, BOOL i_bAuthenticateCheck/*=TRUE*/, UINT i_nClientIP/*=0*/)
//{
//	// 2008-04-25 by cmkwon, 지원 언어/서비스 추가시 꼭 추가 되어야 하는 사항 - [언어-옵션] 추가 언어시에 외부 인증 처리 루틴에 추가
//
//	char szJuminNum[SIZE_MAX_JUMIN_NUMBER]={NULL,};		// 2007-06-29 by cmkwon, td_account 테이블에 주민번호저장하기
//	if(i_bAuthenticateCheck)
//	{
//		char szMD5String[SIZE_MAX_PASSWORD_MD5_STRING];
//		MD5::MD5Binary2String(i_pLogin->Password, szMD5String);
//		BOOL bExtAuth = FALSE;
//		switch(g_pPreGlobal->GetLanguageType())
//		{
//		case LANGUAGE_TYPE_DEFAULT:
//		case LANGUAGE_TYPE_ENGLISH:
//			{
//				bExtAuth = this->ExecuteExtAuth2(i_pLogin->AccountName, szMD5String, i_pLogin->WebLoginAuthKey, hstmt_extAuth);
//			}
//			break;
//		default:
//			{
//				bExtAuth = this->ExecuteExtAuth(i_pLogin->AccountName, szMD5String, hstmt_extAuth);
//			}
//		}
//		
//		if(FALSE == bExtAuth)
//		{// 2006-04-27 by cmkwon, 외부 인증에서 실패함
//
//			/*if(FALSE == IS_MASANG_IP(i_pLogin->PrivateIP))
//			{// 2006-04-28 by cmkwon, 마상소프트 사내 IP가 아니면 인증 실패
//				return FALSE;
//			}*/
//
//			// 2007-10-08 by cmkwon
//			server::log(TRUE, "[Notify] MasangIP authentication !!, AccountName(%s) WebAuthKey(%s) IP(%s)\r\n"
//													, i_pLogin->AccountName, i_pLogin->WebLoginAuthKey, i_pLogin->PrivateIP);
//
////			char szQuery[1024];
//			SDWORD pcbNTS = SQL_NTS;
//
//			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//			//wsprintf(szQuery, "SELECT AccountUniqueNumber, AccountType, Password FROM td_Account WITH (NOLOCK) \
//			//					WHERE AccountName = \'%s\'", i_pLogin->AccountName);
//// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
////			wsprintf(szQuery, QUERY_080702_0258, i_pLogin->AccountName);
//			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pLogin->AccountName, 0, NULL);			
//			RETCODE ret = SQLExecDirect(hstmt, (UCHAR*)PROCEDURE_080827_0258, SQL_NTS);
//			if ( ret!=SQL_SUCCESS
//				&& ret!=SQL_SUCCESS_WITH_INFO )
//			{
//				ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"CAtumPreDBManager::ExternalAuthentication_ Failed!\r\n", TRUE);
//				SQLFreeStmt(hstmt, SQL_CLOSE);
//				return FALSE;
//			}
//			SQLBindCol(hstmt, 1, SQL_C_ULONG, &o_pAccInfo->AccountUniqueNumber, 0, NULL);
//			SQLBindCol(hstmt, 2, SQL_C_USHORT, &o_pAccInfo->AccountType, 0, NULL);
//			SQLBindCol(hstmt, 3, SQL_C_CHAR, o_pAccInfo->PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING, &pcbNTS);
//			ret = SQLFetch(hstmt);
//			SQLFreeStmt(hstmt, SQL_CLOSE);
//			if(0 == strncmp(o_pAccInfo->PasswordFromDB, "", SIZE_MAX_PASSWORD_MD5_STRING))
//			{// 2007-10-08 by cmkwon, DB 패스워드에 설정된 것이 없다 - 인증 불가
//				return FALSE;
//			}
//
//			if(FALSE == IS_VALID_UNIQUE_NUMBER(o_pAccInfo->AccountUniqueNumber))
//			{// 2006-04-28 by cmkwon, 계정이 없다
//				return FALSE;
//			}
//			if(0 != strnicmp(szMD5String, o_pAccInfo->PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING))
//			{// 2006-04-28 by cmkwon, MD5 패스워드 비교
//
//				char szEncodedString[SIZE_MAX_PASSWORD_MD5_STRING];		util::zero(szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);
//				BYTE byPass[SIZE_MAX_PASSWORD_MD5];
//				MD5 md5_instance;
//
//				char szTmPassword[1024];			// 2006-05-22 by cmkwon, 비번에 추가 스트링 적용
//				util::zero(szTmPassword, 1024);
//				wsprintf(szTmPassword, "%s%s", MD5_PASSWORD_ADDITIONAL_STRING, o_pAccInfo->PasswordFromDB);
//				md5_instance.MD5Encode(szTmPassword, byPass);
//				MD5::MD5Binary2String(byPass, szEncodedString);
//
//				if (0 != strnicmp(szEncodedString, szMD5String, SIZE_MAX_PASSWORD_MD5_STRING))
//				{
//					return FALSE;
//				}
//			}
//		}
//	}
//
//	int						nMGameEventType = 0;
//	///////////////////////////////////////////////////////////////////////////////
//	// MGameEvent 처리
//	if(MGAME_EVENT_OPEN_BETA_ATTENDANCE == g_pPreGlobal->m_enMGameEventType
//		&& g_pPreGlobal->IsExistAccountNameForMGameEvent(i_pLogin->AccountName))
//	{
//		nMGameEventType = g_pPreGlobal->m_enMGameEventType;
//	}
//
//	// start 2011-12-12 by hskim, GLog 2차
//#ifdef S_GLOG_2ND_HSKIM		// ON/OFF 기능 구현
//
//	if( FALSE == IsExistAccount(i_pLogin->AccountName, hstmt) )
//	{
//		if( NULL != m_pPreIOCP ) 
//		{
//			CServerGroup *ServerGroup = m_pPreIOCP->GetServerGroup(i_pLogin->FieldServerGroupName);
//
//			if( ServerGroup != NULL )
//			{
//				ServerInfo *ServerInfo = &ServerGroup->m_FieldServerInfo;
//
//				if( TRUE == ServerInfo->IsActive )
//				{
//					INIT_MSG_WITH_BUFFER(MSG_FP_GLOG_CONNECT_USER_NEW, T_FP_GLOG_CONNECT_USER_NEW, pSendGLogMsg, pSendGLogBuf);
//					util::strncpy(pSendGLogMsg->AccountName, i_pLogin->AccountName, SIZE_MAX_ACCOUNT_NAME);
//					ServerGroup->SendMessageToFieldServer(pSendGLogBuf, MSG_SIZE(MSG_FP_GLOG_CONNECT_USER_NEW));
//				}
//			}
//		}
//	}
//#endif
//	// end 2011-12-12 by hskim, GLog 2차
//
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name: atum_PreServerExtAuthLogin
//	-- Desc: 2006-04-27 by cmkwon, 추가함
//	--		   외부 DB 인증 로그인 처리 - 비번 체크는 없음, 계정이 없으면 계정 추가, AccountUniqueNumber를 리턴한다.
//	--		2006-11-15 by cmkwon, Select 검색 필드 추가(td_account테이블에 추가된 필드 - GameContinueTimeInSecondOfToday, LastGameEndDate)
//	--		2007-06-29 by cmkwon, td_account 테이블에 주민번호저장하기 - 주민번호 인자 추가
//	--		// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - atum_PreServerExtAuthLogin 프로시저 수정
//	--====
//	CREATE PROCEDURE dbo.atum_PreServerExtAuthLogin
//		@i_AccountName				VARCHAR(20),
//		@i_Sex						TINYINT,			-- 1: 남, 2: 여
//		@i_BirthYear				INT,				-- 생년
//		@i_MGameEventType			INT,
//		@i_JuminNumber				VARCHAR(20)			-- 2007-06-29 by cmkwon, td_account 테이블에 주민번호저장하기 - 추가된 인자
//	AS
//		DECLARE @AccountUID INT
//		SET @AccountUID = (SELECT AccountUniqueNumber FROM td_Account WITH (NOLOCK) WHERE AccountName = @i_AccountName)
//
//		-- 최초 접속자는 계정 추가
//		IF (@AccountUID IS NULL)
//		BEGIN
//			INSERT INTO td_Account(AccountName, Sex, BirthYear, MGameEventType, JuminNumber)
//				VALUES(@i_AccountName, @i_Sex, @i_BirthYear, @i_MGameEventType, @i_JuminNumber)
//		END
//		ELSE
//		BEGIN
//			-- 2007-03-30 by cmkwon
//			-- SQLMoreResults()의 개수를 맞추기 위해서 추가한 코드임
//			UPDATE td_Account
//				SET Sex = Sex
//				WHERE AccountName = @i_AccountName
//		END
//		
//		-- 마지막 로그인 시간 저장
//		UPDATE td_Account
//			SET LastLoginDate = GetDate()
//			WHERE AccountName = @i_AccountName	
//		
//		-- // 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - SecondaryPassword 가져오기 추가
//		SELECT AccountUniqueNumber, AccountType, Password, RegisteredDate, GameContinueTimeInSecondOfToday, LastGameEndDate, SecondaryPassword
//			FROM td_Account WHERE AccountName = @i_AccountName
//	GO
//	**************************************************************************/
//	SQLINTEGER arrCB[8] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS};
//	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_pLogin->AccountName, 0,	&arrCB[1]);
//	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_pLogin->MGameSEX, 0,						&arrCB[2]);
//	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pLogin->MGameYear, 0,							&arrCB[3]);
//	SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nMGameEventType, 0,								&arrCB[4]);
//	SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_JUMIN_NUMBER, 0, szJuminNum, 0,				&arrCB[5]);	// 2007-06-29 by cmkwon, td_account 테이블에 주민번호저장하기
//
//	RETCODE ret = SQLExecDirect(hstmt, PROCEDURE_080822_0271, SQL_NTS);
//	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//	{
//		ProcessLogMessages(SQL_HANDLE_STMT, hstmt,"@atum_PreServerExtAuthLogin Failed!\n\n", TRUE);
//		SQLFreeStmt(hstmt, SQL_CLOSE);
//		return FALSE;
//	}
//
//	arrCB[0]= arrCB[1]= arrCB[2]= arrCB[3]= arrCB[4]= arrCB[5]= arrCB[6]= arrCB[7]= SQL_NTS;
//	char szPasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];	util::zero(szPasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING);
//	SQL_TIMESTAMP_STRUCT		tmSqlTime;		util::zero(&tmSqlTime, sizeof(tmSqlTime));
//	SQL_TIMESTAMP_STRUCT		tmSqlTime2;		util::zero(&tmSqlTime2, sizeof(tmSqlTime2));
// 	SQLBindCol(hstmt, 1, SQL_C_ULONG, &o_pAccInfo->AccountUniqueNumber, 0,						&arrCB[1]);
// 	SQLBindCol(hstmt, 2, SQL_C_USHORT, &o_pAccInfo->AccountType, 0,								&arrCB[2]); 
//	SQLBindCol(hstmt, 3, SQL_C_CHAR, o_pAccInfo->PasswordFromDB, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[3]);
//	SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &tmSqlTime, 0,										&arrCB[4]);
// 	SQLBindCol(hstmt, 5, SQL_C_LONG, &o_pAccInfo->GameContinueTimeInSecondOfToday, 0,			&arrCB[5]);		// 2006-11-15 by cmkwon
//	SQLBindCol(hstmt, 6, SQL_C_TIMESTAMP, &tmSqlTime2, 0,										&arrCB[6]);		// 2006-11-15 by cmkwon
//	SQLBindCol(hstmt, 7, SQL_C_CHAR, o_pAccInfo->SecondaryPassword, SIZE_MAX_PASSWORD_MD5_STRING,	&arrCB[7]);	// 2007-09-12 by cmkwon, 베트남 2차패스워드 구현 - SecondaryPassword 가져오기 추가
//
//// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) - 아래와 같이 수정
//// 	///////////////////////////////////////////////////////////////////////////////
//// 	// 2007-03-30 by cmkwon
//// 	ret = SQLMoreResults(hstmt);		// 2007-03-30 by cmkwon, 없는 계정 추가 처리
//// 	ret = SQLMoreResults(hstmt);		// 2007-03-30 by cmkwon, 마지막 로그인 시간 저장 처리
////
////	///////////////////////////////////////////////////////////////////////////////
////	// 2007-03-30 by cmkwon, 계정 정보 가져오기
////	ret= SQLFetch(hstmt);
////	SQLFreeStmt(hstmt, SQL_CLOSE);
////	o_pAccInfo->AccountRegisteredDate	= tmSqlTime;
////	o_pAccInfo->LastGameEndDate			= tmSqlTime2;		// 2006-11-15 by cmkwon
//
//	///////////////////////////////////////////////////////////////////////////////
//	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
//	SQLSMALLINT    nColCnts          = 0;
//	while(TRUE)
//	{
//		SQLNumResultCols(hstmt, &nColCnts);
//		if(nColCnts <= 0)
//		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
//			ret = SQLMoreResults(hstmt);
//			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//			{
//				break;
//			}
//			continue;
//		}
//		ret = SQLFetch(hstmt);
//		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//		{
//			ret = SQLMoreResults(hstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
//			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
//			{
//				break;
//			}
//			continue;
//		}
//		o_pAccInfo->AccountRegisteredDate	= tmSqlTime;
//		o_pAccInfo->LastGameEndDate			= tmSqlTime2;		// 2006-11-15 by cmkwon
//	}
//	SQLFreeStmt(hstmt, SQL_CLOSE);
//
//	return IS_VALID_UNIQUE_NUMBER(o_pAccInfo->AccountUniqueNumber);
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CAtumPreDBManager::ExecuteExtAuth(char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
///// \brief		
///// \author		cmkwon
///// \date		2006-05-17 ~ 2006-05-17
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumPreDBManager::ExecuteExtAuth(char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
//{
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name: atum_ExternalAuthentication
//	-- Desc: 2006-04-27 by cmkwon, 추가 함
//	--		   외부 DB 인증 프로시저 - 이것은 없어도 되지만 내부에서 테스트 하기 위해 필요한 모듈임
//	--		   실제로 이 모듈은 상대 회사에서 구현해 주어야 한다.
//	--====
//	CREATE PROCEDURE atum_ExternalAuthentication
//		@i_AccountName				VARCHAR(20),	
//		@i_Password					VARCHAR(35)
//	AS
//		DECLARE @AccountUID INT
//		
//		SET @AccountUID = (SELECT AccountUniqueNumber 
//								FROM td_Account
//								WHERE AccountName = @i_AccountName AND Password = @i_Password)
//		
//		IF (@AccountUID IS NULL)
//			BEGIN
//					RETURN 0
//			END
//
//		RETURN 1
//	GO	
//	**************************************************************************/
//	BOOL		bAuth = FALSE;
//
//	SQLBindParameter(hstmt_extAuth, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bAuth, 0,								NULL);
//	SQLBindParameter(hstmt_extAuth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
//	SQLBindParameter(hstmt_extAuth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
//	BOOL ret = SQLExecDirect(hstmt_extAuth, (UCHAR*)"{ ? = call atum_ExternalAuthentication(?, ?)}", SQL_NTS);
//	if (ret != SQL_SUCCESS
//		&& ret != SQL_SUCCESS_WITH_INFO)
//	{
//		if(ret != SQL_NO_DATA)
//		{// 쿼리 실패
//			ProcessLogMessages(SQL_HANDLE_STMT, hstmt_extAuth, "@atum_ExternalAuthentication Failed!!\r\n", TRUE);
//		}
//		SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
//		return FALSE;
//	}
//
//	ret = SQLMoreResults(hstmt_extAuth);
//	ret = SQLMoreResults(hstmt_extAuth);
//	SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
//	if(FALSE == bAuth)
//	{
//		return FALSE;
//	}
//	
//	return TRUE;
//}
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CAtumPreDBManager::ExecuteExtAuth_GalaNet(INT *o_pnExtAccountIDNum, char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
///// \brief		
///// \author		cmkwon
///// \date		2006-05-22 ~ 2006-05-22
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumPreDBManager::ExecuteExtAuth_GalaNet(INT *o_pnExtAccountIDNum, char *i_szAccName, char *i_szPwd, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
//{
//	*o_pnExtAccountIDNum	= 0;		// 2006-05-22 by cmkwon, 초기화
//
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name: G_AUTHENTICATE
//	-- Desc: 2006-04-27 by cmkwon, 추가 함
//	--		   Galanet용 
//	--		@o_ExtAccountIDNum = -1 ==> 어카운트차단상태
//	--		@o_ExtAccountIDNum =  0 ==> 계정이 존재하지 안거만 비밀번호가 오류 상태임
//	--		@o_ExtAccountIDNum =  그 이외의 값 ==> 인증 성공, 계정의 고유 IDNumber
//	--====
//	CREATE PROCEDURE G_AUTHENTICATE
//		@i_AccountName			VARCHAR(20),	
//		@i_Password				VARCHAR(35),
//		@i_GameName				VARCHAR(10),
//		@o_ExtAccountIDNum		INT				OUTPUT
//	AS		
//	GO	
//	**************************************************************************/
//	char *szExtAuthGameName  = EXT_AUTH_GAME_NAME; 
//	SQLINTEGER arrCB[5] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
//	SQLBindParameter(hstmt_extAuth, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
//	SQLBindParameter(hstmt_extAuth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
//	SQLBindParameter(hstmt_extAuth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 10, 0, szExtAuthGameName, 0,					NULL);
//	SQLBindParameter(hstmt_extAuth, 4, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, o_pnExtAccountIDNum, 0,					NULL);
//	BOOL ret = SQLExecDirect(hstmt_extAuth, PROCEDURE_080822_0273, SQL_NTS);
//	if (ret != SQL_SUCCESS
//		&& ret != SQL_SUCCESS_WITH_INFO)
//	{
//		if(ret != SQL_NO_DATA)
//		{// 쿼리 실패
//			ProcessLogMessages(SQL_HANDLE_STMT, hstmt_extAuth, "@G_AUTHENTICATE Failed!!\r\n", TRUE);
//		}
//		SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
//		return FALSE;
//	}
//
//	ret = SQLMoreResults(hstmt_extAuth);
//	ret = SQLMoreResults(hstmt_extAuth);
//	SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
//	if(0 == *o_pnExtAccountIDNum
//		|| -1 == *o_pnExtAccountIDNum)
//	{
//		return FALSE;
//	}
//	
//	return TRUE;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
///// \fn			BOOL CAtumPreDBManager::ExecuteExtAuth2(char *i_szAccName, char *i_szPwd, char *i_szWebLoginAuthKey, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
///// \brief		
///// \author		cmkwon
///// \date		2007-03-30 ~ 2007-03-30
///// \warning	
/////
///// \param		
///// \return		
/////////////////////////////////////////////////////////////////////////////////
//BOOL CAtumPreDBManager::ExecuteExtAuth2(char *i_szAccName, char *i_szPwd, char *i_szWebLoginAuthKey, SQLHSTMT &hstmt_extAuth, EN_DBCONN_TYPE i_dbConnTy/*=EN_DBCONN_EXT_AUTH*/)
//{
//	/*[Stored Query Definition]************************************************
//	--!!!!
//	-- Name: atum_ExternalAuthentication2
//	-- Desc: 2007-03-30 by cmkwon, 추가 함
//	--		   외부 DB 인증 프로시저 - 이것은 없어도 되지만 내부에서 테스트 하기 위해 필요한 모듈임
//	--		   실제로 이 모듈은 상대 회사에서 구현해 주어야 한다.
//	--====
//	CREATE PROCEDURE atum_ExternalAuthentication2
//		@o_RetCode					INT				OUTPUT,		-- 2007-03-30 by cmkwon
//		@i_AccountName				VARCHAR(20),				-- 계정명
//		@i_Password					VARCHAR(35),				-- 패스워드
//		@i_WebLoginAuthKey			VARCHAR(20)					-- 웹인증키
//	AS
//		SET @o_RetCode		= 0
//
//		DECLARE @AccountUID INT	
//		SET @AccountUID = (SELECT AccountUniqueNumber FROM td_Account WITH(NOLOCK) WHERE AccountName = @i_AccountName AND Password = @i_Password)
//		
//		IF (@AccountUID IS NULL)
//		BEGIN
//			SET @o_RetCode		= 1
//			RETURN
//		END
//
//		SET @o_RetCode		= 0
//		RETURN
//	GO
//	**************************************************************************/
//	int		nRetCode = 1;		// 2008-01-23 by cmkwon, 기본값을 에러 상태로 설정한다.
//	SQLBindParameter(hstmt_extAuth, 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRetCode, 0,								NULL);
//	SQLBindParameter(hstmt_extAuth, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,		NULL);
//	SQLBindParameter(hstmt_extAuth, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPwd, 0,	NULL);
//	SQLBindParameter(hstmt_extAuth, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_WEBLOGIN_AUTHENTICATION_KEY, 0, i_szWebLoginAuthKey, 0,		NULL);
//	BOOL ret = SQLExecDirect(hstmt_extAuth, PROCEDURE_080822_0274, SQL_NTS);
//	if (ret != SQL_SUCCESS
//		&& ret != SQL_SUCCESS_WITH_INFO)
//	{
//		if(ret != SQL_NO_DATA)
//		{// 쿼리 실패
//			ProcessLogMessages(SQL_HANDLE_STMT, hstmt_extAuth, "@atum_ExternalAuthentication2 Failed!!\r\n", TRUE);
//		}
//		SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
//		return FALSE;
//	}
//
//	ret = SQLMoreResults(hstmt_extAuth);
//	ret = SQLMoreResults(hstmt_extAuth);
//	SQLFreeStmt(hstmt_extAuth, SQL_CLOSE);
//	if(0 != nRetCode)
//	{// 2007-03-30 by cmkwon, 리턴값이 0이 아니면 인증 실패임
//		return FALSE;
//	}
//	
//	return TRUE;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumPreDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
/// \brief		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumPreDBManager::ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC)
{
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - PreServer는 사용하는 쿼리가 없음
	//	switch(i_qQuery->enumQueryType)
	//	{
	//	case :
	//	}

	// error: no such DB query type
	g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumPreDBManager::ProcessDynamicServerQuery# no such DB Query Type !! %d(%s)\r\n", i_qQuery->enumQueryType, GetDBQueryTypeString(i_qQuery->enumQueryType));
	return FALSE;
}

void CAtumPreDBManager::QP_LoadBlockedAccounts(DB_QUERY dbquery, SQLHSTMT hstmt)
{
	SQLRETURN ret;

	// Get the account block manager
	CAccountBlockManager* pBlockedAccountManager = (CAccountBlockManager*)dbquery.pQueryParam;

	// Exec query
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0276, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		CAtumDBManager::ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "LoadBlockedAccounts Failed!\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	// Fetch the result
	if (SQL_NO_DATA != ret)
	{
		SQLINTEGER arrCB[8];
		fill_n(arrCB, 8, SQL_NTS);

		SBLOCKED_ACCOUNT_INFO	tmBlockedInfo;
		SQL_TIMESTAMP_STRUCT	arrSqlTime[2];

		SQLBindCol(hstmt, 1, SQL_C_CHAR, tmBlockedInfo.szBlockedAccountName, SIZE_MAX_ACCOUNT_NAME, &arrCB[1]);
		SQLBindCol(hstmt, 2, SQL_C_LONG, &tmBlockedInfo.enBlockedType, 0, &arrCB[2]);
		SQLBindCol(hstmt, 3, SQL_C_TIMESTAMP, &arrSqlTime[0], 0, &arrCB[3]);
		SQLBindCol(hstmt, 4, SQL_C_TIMESTAMP, &arrSqlTime[1], 0, &arrCB[4]);
		SQLBindCol(hstmt, 5, SQL_C_CHAR, tmBlockedInfo.szBlockAdminAccountName, SIZE_MAX_ACCOUNT_NAME, &arrCB[5]);
		SQLBindCol(hstmt, 6, SQL_C_CHAR, tmBlockedInfo.szBlockedReasonForUser, SIZE_MAX_BLOCKED_ACCOUNT_REASON, &arrCB[6]);
		SQLBindCol(hstmt, 7, SQL_C_CHAR, tmBlockedInfo.szBlockedReasonForOnlyAdmin, SIZE_MAX_BLOCKED_ACCOUNT_REASON, &arrCB[7]);	// 2007-01-10 by cmkwon

		util::zero(&tmBlockedInfo, sizeof(SBLOCKED_ACCOUNT_INFO));
		util::zero(arrSqlTime, sizeof(arrSqlTime[0]) * 2);
		while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
		{
			tmBlockedInfo.atimeStartTime = arrSqlTime[0];
			tmBlockedInfo.atimeEndTime = arrSqlTime[1];

			// 2010-02-22 by cmkwon, 계정블럭시 대문자포함 계정명 문제 해결(K0002462) - 
			_strlwr(tmBlockedInfo.szBlockedAccountName);

			pBlockedAccountManager->AddBlockedAccountList(&tmBlockedInfo);

			util::zero(&tmBlockedInfo, sizeof(SBLOCKED_ACCOUNT_INFO));
			util::zero(arrSqlTime, sizeof(arrSqlTime[0]) * 2);
		}
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void CAtumPreDBManager::QP_LoadBlockedMACs(DB_QUERY dbquery, SQLHSTMT & hstmt)
{
	SQLRETURN ret;

	// Get blocked MAC list
	mtsetString* pBlockedMacs = (mtsetString*)dbquery.pQueryParam;

	// Exec query
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0283, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		CAtumDBManager::ProcessLogMessages(SQL_HANDLE_STMT, hstmt, "LoadBlockedMACs Failed!\n", TRUE);
		SQLFreeStmt(hstmt, SQL_CLOSE);
		return;
	}

	// Fetch the result
	if (ret != SQL_NO_DATA)
	{
		char blockedMacAddr[SIZE_MAX_MAC_ADDRESS] { };

		SQLBindCol(hstmt, 1, SQL_C_CHAR, blockedMacAddr, SIZE_MAX_MAC_ADDRESS, NULL);

		while ((ret = SQLFetch(hstmt)) != SQL_NO_DATA)
		{
			pBlockedMacs->insertNoLock(blockedMacAddr);
			util::zero(blockedMacAddr, SIZE_MAX_MAC_ADDRESS);
		}
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void CAtumPreDBManager::QP_BlockMAC(DB_QUERY dbquery, SQLHSTMT hstmt)
{
	SQLRETURN ret;

	// Get the MAC
	QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS* pParam = (QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS*)dbquery.pQueryParam;

	// Bind param
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAC_ADDRESS, 0, pParam->MACAddress, 0, NULL);

	// Exec Query
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0284, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		char errMsg[256];
		sprintf_s(errMsg, "Failed to insert blocked MAC Address! Address(%s)\n", pParam->MACAddress);
		CAtumDBManager::ProcessLogMessages(SQL_HANDLE_STMT, hstmt, errMsg, TRUE);
	}

	util::del(pParam);
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

void CAtumPreDBManager::QP_UnblockMAC(DB_QUERY dbquery, SQLHSTMT hstmt)
{
	SQLRETURN ret;

	// Get the MAC
	QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS* pParam = (QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS*)dbquery.pQueryParam;

	// Bind param
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_MAC_ADDRESS, 0, pParam->MACAddress, 0, NULL);

	// Exec Query
	ret = SQLExecDirect(hstmt, PROCEDURE_080822_0285, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		char errMsg[256];
		sprintf_s(errMsg, "Failed to delete blocked MAC Address! Address(%s)\n", pParam->MACAddress);
		CAtumDBManager::ProcessLogMessages(SQL_HANDLE_STMT, hstmt, errMsg, TRUE);
	}

	util::del(pParam);
	SQLFreeStmt(hstmt, SQL_CLOSE);
}
