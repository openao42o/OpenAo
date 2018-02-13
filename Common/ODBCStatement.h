#pragma once

#include <sql.h>
#include <sqlext.h>
#include "mt_stl.h"

#ifndef MAXDBMSGBUFLEN
#define MAXDBMSGBUFLEN	255
#endif

#define SIZE_MAX_SQL_QUERY_STRING	1024

class CODBCStatement
{
public:
	CODBCStatement() : m_henv(SQL_NULL_HENV), m_hdbc(SQL_NULL_HDBC), m_hstmt(SQL_NULL_HSTMT) { }
	virtual ~CODBCStatement() { Clean(); }

	BOOL Init(const char *i_szDSN, const char *i_szUID, const char *i_szPassword);
	BOOL Init(UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPassword);
	BOOL Init(const char *i_szIPAddress, int i_nPort, const char *i_szDatabase, const char *i_szUID, const char *i_szPassword, SQLHWND hwnd);
	BOOL Init(const char *i_szIPAddress, int i_nPort, const char *i_szDatabase, SQLHWND hwnd);
	BOOL Clean();

	BOOL IsReadyToUse();

	BOOL ExecuteQuery(const char *pSQLQueryString, BOOL i_bAutoClean = FALSE);
	BOOL ExecuteQuery(const UCHAR *pSQLQueryString, BOOL i_bAutoClean = FALSE);		// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	void FreeStatement();
	void ClearAllResultSets();

	void ProcessLogMessagesForStmt(const char *i_szLogString = "");
	void ProcessLogMessagesForStmt(const UCHAR *i_szLogString = NULL);				// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	void ProcessLogMessages(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, const char *logstring, BOOL ConnInd);

	void Lock() { m_lock.lock(); }
	void Unlock() { m_lock.unlock(); }

	void FreeStatementAndUnlock() { FreeStatement(); m_lock.unlock(); }

	SQLHENV GetENVHandle() const { return m_henv; }
	SQLHDBC GetDBCHandle() const { return m_hdbc; }
	SQLHSTMT GetSTMTHandle() const { return m_hstmt; }
	mt_lock *GetLockPtr() { return &m_lock; };		// 2006-04-27 by cmkwon

public:
	SQLHENV		m_henv;
	SQLHDBC		m_hdbc;
	SQLHSTMT	m_hstmt;

	mt_lock		m_lock;
};
