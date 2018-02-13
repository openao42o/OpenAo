// FieldODBC.cpp: implementation of the CFieldODBC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldODBC.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldODBC::CFieldODBC()
{

}

CFieldODBC::~CFieldODBC()
{

}

// 2008-07-11 by cmkwon, MySQL 지원을 위해 수정(IP,Port 로 DB 서버 연결) - 아래와 같이 수정 함
//BOOL CFieldODBC::InitFieldODBC(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD)
//{
//	return CODBCStatement::Init(i_szDSN, i_szUID, i_szPASSWORD);
//}
BOOL CFieldODBC::InitFieldODBC(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd)
{
	return CODBCStatement::Init(i_szServIP, i_nServPort, i_szDatabaseName, i_szUID, i_szPassword, i_hWnd);
}

void CFieldODBC::CleanFieldODBC(void)
{
	CODBCStatement::Clean();
}

Err_t CFieldODBC::ChangeCharacterName(UID32_t i_characterUID, char *i_szOriginName, char *i_szChangeName)
{
	mt_auto_lock mtA(&this->m_lock);

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_Cash_ChangeCharacterName
		@i_CharacterUID				INT,					-- 변경 할 캐릭터 UID
		@i_OriginCharacterName		VARCHAR(20),			-- 원래 캐릭터명
		@i_ChangeCharacterName		VARCHAR(20)				-- 변경하려는 캐릭터명
	AS
		DECLARE @searchUniqueNumber INT
		SET @searchUniqueNumber = (SELECT UniqueNumber	FROM td_Character WHERE CharacterName = @i_ChangeCharacterName)

		IF (@searchUniqueNumber IS NOT NULL)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
			--return 0
			SELECT 0;
			RETURN;
		END
		
		-- 친구리스트에서 모두(자신,상대방) 삭제
		DELETE FROM td_FriendList
			WHERE CharacterName = @i_OriginCharacterName OR FriendName = @i_OriginCharacterName
		
		UPDATE td_Character
			SET CharacterName = @i_ChangeCharacterName
			WHERE UniqueNumber = @i_CharacterUID
		
		-- // 2008-08-20 by cmkwon, MySQL 관련 소스 통합 - 
		-- return 1
		SELECT 1;
	GO
	**************************************************************************/
	SQLINTEGER arrCB[5]	= {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	BOOL bRetSuccess	= FALSE;

// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
// 	SQLBindParameter(this->GetSTMTHandle(), 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bRetSuccess, 0,	&arrCB[1]);
// 	SQLBindParameter(this->GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_characterUID, 0,	&arrCB[2]);
// 	SQLBindParameter(this->GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szOriginName, 0, &arrCB[3]);
// 	SQLBindParameter(this->GetSTMTHandle(), 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szChangeName, 0, &arrCB[4]);
// 	// 2008-07-08 by cmkwon, MySQL 지원으로 수정(프로시저에서 리턴처리 불가로 수정) - 
// 	//if(FALSE == this->ExecuteQuery("{? = call atum_Cash_ChangeCharacterName(?, ?, ?)}"))
// 	if(FALSE == this->ExecuteQuery("{call atum_Cash_ChangeCharacterName(?, ?, ?, ?)}"))
// 	{
// 		this->FreeStatement();
// 		return ERR_DB_EXECUTION_FAILED;
// 	}
// 
// 	this->ClearAllResultSets();		// return 값 할당
// 	this->FreeStatement();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLBindParameter(this->GetSTMTHandle(), 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_characterUID, 0,	&arrCB[1]);
	SQLBindParameter(this->GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szOriginName, 0, &arrCB[2]);
	SQLBindParameter(this->GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szChangeName, 0, &arrCB[3]);

	BOOL bRet = this->ExecuteQuery((char*)(PROCEDURE_080822_0206));
	if(FALSE == bRet)
	{
		this->FreeStatement();
		return ERR_DB_EXECUTION_FAILED;
	}
	SQLBindCol(this->GetSTMTHandle(), 1, SQL_C_LONG, &(bRetSuccess), 0, NULL);

	RETCODE ret = 0;
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL 포팅 관련(OUTPUT 제거) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(this->GetSTMTHandle(), &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(this->GetSTMTHandle());
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(this->GetSTMTHandle());
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(this->GetSTMTHandle());                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	this->FreeStatement();

	if(FALSE == bRetSuccess)
	{
		return ERR_COMMON_CHARACTER_ALREADY_EXISTS;
	}

	return ERR_NO_ERROR;
}

// start 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 횟수를 리턴한다.
INT CFieldODBC::CheckLoginEventDBData(UID32_t Account_UID, INT i_eventItemnumber/*=1*/)
{ 
	SQLRETURN	bRet	= 0;
	INT	nReturn = 0;

	// 리턴 값 체크
	bRet = SQLBindParameter(this->GetSTMTHandle(), 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nReturn, 0,	NULL);	// 리턴 값
	bRet = SQLBindParameter(this->GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_eventItemnumber, 0,	NULL);	// 현재는 이벤트 넘버는 고정 처리하고 있다.
	bRet = SQLBindParameter(this->GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &Account_UID, 0,			NULL);
	
	bRet = SQLExecDirect(this->GetSTMTHandle(), PROCEDURE_LOGINITEMEVENT_110825_0002, SQL_NTS);	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[SQL Error] dbo.atum_CheckLoginItemEvent Failed!!\r\n");
		SQLFreeStmt(this->GetSTMTHandle(), SQL_CLOSE);
		return -1; // 실패
	}
	SQLFreeStmt(this->GetSTMTHandle(), SQL_CLOSE);
	return nReturn;
};
// End 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현 - 횟수를 리턴한다.
