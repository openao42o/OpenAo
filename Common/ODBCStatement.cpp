#include "stdafx.h"
#include "ODBCStatement.h"
#include "DBGOUT_c.h"
#define UNICODE
#include "odbcss.h"
#undef UNICODE

///////////////////////////////////////////////////////////////////////////////
// class CODBCStatement
///////////////////////////////////////////////////////////////////////////////
BOOL CODBCStatement::Init(const char *i_szDSN, const char *i_szUID, const char *i_szPassword)
{
	return Init((UCHAR*)i_szDSN, (UCHAR*)i_szUID, (UCHAR*)i_szPassword);
}

BOOL CODBCStatement::Init(UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPassword)
{
	RETCODE		ret;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_henv);
	ret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);

	ret = SQLConnect(m_hdbc, i_szDSN, SQL_NTS, i_szUID, SQL_NTS, i_szPassword, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_DBC, m_hdbc,"CODBCStatement::Init() Failed!\n", FALSE);
		Clean();
		return FALSE;
	}

	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(m_hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(m_hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);

	return TRUE;
}

BOOL CODBCStatement::Init(const char *i_szIPAddress, int i_nPort, const char *i_szDatabase, const char *i_szUID, const char *i_szPassword, SQLHWND hwnd)
{
	RETCODE		ret;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_henv);
	ret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);

	char szConnectionString[512];
	char szOutConnectionString[512]; SQLSMALLINT pcb;
#if defined(DB_SERVER_MYSQL)
	sprintf(szConnectionString, "DRIVER={MySQL ODBC 5.1 Driver};SERVER=%s;ADDRESS=%s,%d;\
								NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s",
								i_szIPAddress, i_szIPAddress, i_nPort, i_szUID, i_szPassword, i_szDatabase);
#else
	sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s;ADDRESS=%s,%d;\
								NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s",
								i_szIPAddress, i_szIPAddress, i_nPort, i_szUID, i_szPassword, i_szDatabase);	
#endif

	ret = SQLDriverConnect(m_hdbc, hwnd, (SQLCHAR*)szConnectionString, strlen(szConnectionString), (SQLCHAR*)szOutConnectionString, 512, &pcb, SQL_DRIVER_NOPROMPT);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_DBC, m_hdbc,"CODBCStatement::Init() Failed!\n", FALSE);
		Clean();
		return FALSE;
	}

	
	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(m_hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(m_hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);

	return TRUE;
}

BOOL CODBCStatement::Init(const char *i_szIPAddress, int i_nPort, const char *i_szDatabase, SQLHWND hwnd)
{
	RETCODE		ret;

	ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &m_henv);
	ret = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	ret = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);

	char szConnectionString[512];
	char szOutConnectionString[512]; SQLSMALLINT pcb;
#if defined(DB_SERVER_MYSQL)
	sprintf(szConnectionString, "DRIVER={MySQL ODBC 5.1 Driver};SERVER=%s,%d;ADDRESS=%s,%d;\
								NETWORK=DBMSSOCN;DATABASE=%s;Trusted_Connection=no",
								i_szIPAddress, i_nPort, i_szIPAddress, i_nPort, i_szDatabase);
#else
	sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s,%d;ADDRESS=%s,%d;\
								NETWORK=DBMSSOCN;DATABASE=%s;Trusted_Connection=no",
								i_szIPAddress, i_nPort, i_szIPAddress, i_nPort, i_szDatabase);
#endif

	ret = SQLDriverConnect(m_hdbc, hwnd, (SQLCHAR*)szConnectionString, strlen(szConnectionString), (SQLCHAR*)szOutConnectionString, 512, &pcb, SQL_DRIVER_PROMPT);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		ProcessLogMessages(SQL_HANDLE_DBC, m_hdbc,"CODBCStatement::Init() Failed!\n", FALSE);
		Clean();
		return FALSE;
	}

	
	// prints DB Info
	char DatabaseName[40];
	char DSN[40];
	SQLSMALLINT StrLen;
	SQLGetInfo(m_hdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
	SQLGetInfo(m_hdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

	ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);

	return TRUE;
}

BOOL CODBCStatement::Clean()
{
	this->Lock();
	if (m_hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HSTMT;
	}

	if (m_hdbc != SQL_NULL_HDBC)
	{
		SQLDisconnect(m_hdbc);
#ifndef _DEBUG
		SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
#endif
		m_hdbc = SQL_NULL_HDBC;
	}

	if (m_henv != SQL_NULL_HENV)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
		m_henv = SQL_NULL_HENV;
	}

	this->Unlock();

	return TRUE;
}

BOOL CODBCStatement::IsReadyToUse()
{
	return (m_henv != SQL_NULL_HENV && m_hdbc != SQL_NULL_HDBC && m_hstmt != SQL_NULL_HSTMT);
}

BOOL CODBCStatement::ExecuteQuery(const char *pSQLQueryString, BOOL i_bAutoClean/*=FALSE*/)
{
	SQLRETURN ret;
	ret = SQLExecDirect(m_hstmt, (UCHAR*)pSQLQueryString, SQL_NTS);

	if(SQL_NO_DATA == ret
		|| SQL_SUCCESS == ret
		|| SQL_SUCCESS_WITH_INFO == ret)
	{
		return TRUE;
	}
	
	string err = "Query Exec FAILED: ";
	err += pSQLQueryString;
	err += "\r\n";
	ProcessLogMessages(SQL_HANDLE_STMT, m_hstmt, err.c_str(), TRUE);

	if(i_bAutoClean)
	{// 2006-05-18 by cmkwon, 

		Clean();
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CODBCStatement::ExecuteQuery(const UCHAR *pSQLQueryString, BOOL i_bAutoClean/*=FALSE*/)
/// \brief		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CODBCStatement::ExecuteQuery(const UCHAR *pSQLQueryString, BOOL i_bAutoClean/*=FALSE*/)
{
	return this->ExecuteQuery((char*)(pSQLQueryString), i_bAutoClean);
}

void CODBCStatement::FreeStatement()
{
	if(m_hstmt)
	{
		SQLFreeStmt(m_hstmt, SQL_CLOSE);
	}
}

void CODBCStatement::ClearAllResultSets()
{
	// Clear any result sets generated.
	while (SQLMoreResults(m_hstmt) != SQL_NO_DATA)
		;
}

void CODBCStatement::ProcessLogMessagesForStmt(const char *i_szLogString)
{
	if (m_hstmt == SQL_NULL_HSTMT)
	{
		return;
	}

	ProcessLogMessages(SQL_HANDLE_STMT, m_hstmt, i_szLogString, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CODBCStatement::ProcessLogMessagesForStmt(const UCHAR *i_szLogString/*=NULL*/)
/// \brief		
/// \author		cmkwon
/// \date		2008-12-01 ~ 2008-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CODBCStatement::ProcessLogMessagesForStmt(const UCHAR *i_szLogString/*=NULL*/)
{
	this->ProcessLogMessagesForStmt((char*)(i_szLogString));
}

void CODBCStatement::ProcessLogMessages(SQLSMALLINT plm_handle_type,
												SQLHANDLE &plm_handle,
												const char *logstring, BOOL ConnInd)
{
	RETCODE		plm_retcode						= SQL_SUCCESS;
	UCHAR		plm_szSqlState[MAXDBMSGBUFLEN]	= "",
	plm_szErrorMsg[MAXDBMSGBUFLEN]				= "";
	SDWORD		plm_pfNativeError				= 0L;
	SWORD		plm_pcbErrorMsg					= 0;
	SQLSMALLINT	plm_cRecNmbr					= 1;
	SDWORD		plm_SS_MsgState = 0, plm_SS_Severity = 0;
	SQLINTEGER	plm_Rownumber					= 0;
	USHORT		plm_SS_Line;
	SQLSMALLINT	plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR		plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];

	DbgOut(logstring);

	while (plm_handle != SQL_NULL_HANDLE && plm_retcode != SQL_NO_DATA_FOUND)
	{
		plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle,
				plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
				plm_szErrorMsg, MAXDBMSGBUFLEN - 1, &plm_pcbErrorMsg);
		if(SQL_INVALID_HANDLE == plm_retcode)
		{// 2006-05-18 by cmkwon
			break;
		}
		if (plm_retcode != SQL_NO_DATA_FOUND)
		{
			if (ConnInd)
			{
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
												SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
												SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
												SQL_DIAG_SS_LINE, &plm_SS_Line,
												SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
												SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
												SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
												SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
												SQL_IS_INTEGER, NULL);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
												SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
												sizeof(plm_SS_Procname), &plm_cbSS_Procname);
				plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr,
												SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
												sizeof(plm_SS_Srvname), &plm_cbSS_Srvname);
			}

			DbgOut("	szSqlState    = %s\n",plm_szSqlState);
			DbgOut("	pfNativeError = %d\n",plm_pfNativeError);
			DbgOut("	szErrorMsg    = %s\n",plm_szErrorMsg);
			DbgOut("	pcbErrorMsg   = %d\n\n",plm_pcbErrorMsg);

			if (ConnInd)
			{
				DbgOut("	ODBCRowNumber = %d\n", plm_Rownumber);
				DbgOut("	SSrvrLine     = %d\n", plm_Rownumber);
				DbgOut("	SSrvrMsgState = %d\n",plm_SS_MsgState);
				DbgOut("	SSrvrSeverity = %d\n",plm_SS_Severity);
				DbgOut("	SSrvrProcname = %s\n",plm_SS_Procname);
				DbgOut("	SSrvrSrvname  = %s\n\n",plm_SS_Srvname);
			}
		}
		plm_cRecNmbr++; //Increment to next diagnostic record.
	} // End while.
}
