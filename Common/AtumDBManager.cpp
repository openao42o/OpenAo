//Copyright [2002] MasangSoft
#include "stdafx.h"
#include "AtumDBManager.h"
#include "IOCP.h"
#include "odbcstatement.h"
#define UNICODE
#include "odbcss.h"
#undef UNICODE
#include <timeapi.h>
#define    NUM_OF_SYNC_ODBC_STATEMENT 10    // Sync Exec.용 statement 개수


thread_local DBWorkerData CAtumDBManager::workerdata { };

struct ATUM_DB_THREAD_BEGIN_DATA
{
    CAtumDBManager *pAtumDBManager;
    int nAtumDBThreadIndex;
};

DWORD WINAPI DBWorkerThread(LPVOID lpParam)
{
    if (lpParam == nullptr) return 0x100;

    auto pTmpAtumDBThreadData = reinterpret_cast<ATUM_DB_THREAD_BEGIN_DATA*>(lpParam);

    auto nRet = pTmpAtumDBThreadData->pAtumDBManager->AtumDBWorker(pTmpAtumDBThreadData->nAtumDBThreadIndex);

    delete pTmpAtumDBThreadData;

    pTmpAtumDBThreadData = nullptr;

    return nRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            DWORD WINAPI DBDynamicWorkerThread(LPVOID lpParam)
/// \brief        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author        cmkwon
/// \date        2008-12-01 ~ 2008-12-01
///////////////////////////////////////////////////////////////////////////////
DWORD WINAPI DBDynamicWorkerThread(LPVOID lpParam)
{
    auto data = reinterpret_cast<ATUM_DB_THREAD_BEGIN_DATA*>(lpParam);

    auto nRet = data->pAtumDBManager->AtumDBDynamicWorker();

    delete data;
    
    return nRet;
}

// constructor
CAtumDBManager::CAtumDBManager(DWORD i_dwWorkerCounts) : m_threadDBWorker { }
{
    m_dwCountDBWorker = min(i_dwWorkerCounts, COUNT_MAX_DBWORKER_THREADS);
    m_bShutDownFlag        = FALSE;
    m_ArrOdbcStmt        = NULL;    

    for (int i=0; i < m_dwCountDBWorker; i++)
    {
        m_arrmtvectorInputDBQuery[i].reserve(100);
        m_arrmtvectorProcessDBQuery[i].reserve(100);
    }

    m_mtvectInputQueryWithNewThread.reserve(100);
}


CAtumDBManager::~CAtumDBManager()
{
    // Close event handle
}

void CAtumDBManager::Clean()
{
    // Set ShutDownFlag
    m_bShutDownFlag = true;

    // Wait for threads to quit
    for (auto& t : m_threadDBWorker) if (t.joinable()) t.join();

    // For Synchronous Execution
    if (m_ArrOdbcStmt)
    
        for (auto i = 0; i < NUM_OF_SYNC_ODBC_STATEMENT; ++i)
            
            m_ArrOdbcStmt[i].Clean();

    // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - CAtumDBManager::Clean# 에서 종료 체크
    while (true)
    {
        if (CheckAndCloseHandleDynamicDBThread()) break;
        
        this_thread::sleep_for(1s);
    }
    
    delete[] m_ArrOdbcStmt;

    m_ArrOdbcStmt = nullptr;
}

BOOL CAtumDBManager::Init()
{
    m_DBThreadCheck.SetCheckThreadCounts(m_dwCountDBWorker);
    
    // start threads

    try
    {
        for (auto i = 0; i < m_dwCountDBWorker; i++)
        {
            m_threadDBWorker[i] = thread { DBWorkerThread, LPVOID(new ATUM_DB_THREAD_BEGIN_DATA { this, i }) };

            this_thread::yield();
        }
    }
    catch (const system_error&)
    {
        server::log(" [Error] CAtumDBManager::IOCPInit() Worker thread create, WorkerCount[%d]\r\n", m_dwCountDBWorker);

        return false;
    }

    if (!IsReadyAtumDBManager())
    {
        server::log(" [Error] CAtumDBManager::Init IsReadyAtumDBManager() Error\r\n");

        return false;
    }

    // For Synchronous Execution
    m_ArrOdbcStmt = new CODBCStatement[NUM_OF_SYNC_ODBC_STATEMENT];

    for (auto i = 0; i < NUM_OF_SYNC_ODBC_STATEMENT; i++)
    {
        auto bRet = m_ArrOdbcStmt[i].Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(),
            g_pGlobal->GetDBServerDatabaseName(), g_pGlobal->GetODBCUID(),
            g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());

        if (!bRet)
        {
            server::log(" [DB Error] CAtumDBManager::Init fail to connect DBServer !! %s, %d %s %s %s, Idx(%d)\r\n",
                g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(),
                g_pGlobal->GetODBCUID(), g_pGlobal->GetODBCPASSWORD(), i);
        }
    }

    return true;
}


bool CAtumDBManager::IsReadyAtumDBManager() const
{
    auto dwStartTick = timeGetTime();
    
    while (true)
    {
        auto dwCurTick = timeGetTime();

        if (m_DBThreadCheck.GetSettingCompletionFlag()) return true;
        
        if (dwCurTick - dwStartTick > 30000) return false;
        
        Sleep(100);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumDBManager::Connect2DBServer(SQLHENV *i_phenv, SQLHDBC    *i_phdbc, SQLHSTMT *i_phstmt, EN_DB_CONNECTION_TYPE i_connTy/*=EN_DB_CONNECTION_TYPE_AUTO_COMMIT*/)
/// \brief        
/// \author        cmkwon
/// \date        2006-05-17 ~ 2006-05-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBManager::Connect2DBServer(SQLHENV *i_phenv, SQLHDBC    *i_phdbc, SQLHSTMT *i_phstmt, EN_DBCONN_TYPE i_connTy/*=EN_DBCONN_AUTO_COMMIT*/)
{
    RETCODE        ret;
    
    ret = SQLAllocHandle(SQL_HANDLE_ENV, NULL, i_phenv);
    ret = SQLSetEnvAttr(*i_phenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
    ret = SQLAllocHandle(SQL_HANDLE_DBC, *i_phenv, i_phdbc);


    char szConnectionString[512];
    char szOutConnectionString[512]; SQLSMALLINT pcb;

#if defined(DB_SERVER_MYSQL)

    sprintf(szConnectionString, "DRIVER={MySQL ODBC 5.1 Driver};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
        , g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetDBServerDatabaseName());
    
#else
    if ( EN_DBCONN_GLOG_COMMIT == i_connTy )
    {
        sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=GLog"
            , g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD());
    }
    // end 2013-06-20 by jhseol,bckim GLog 보완
    else
    {
        sprintf(szConnectionString, "DRIVER={SQL Server};SERVER=%s;ADDRESS=%s,%d;NETWORK=DBMSSOCN;UID=%s;PWD=%s;DATABASE=%s"
            , g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetDBServerDatabaseName());
    }
#endif
    ret = SQLDriverConnect(*i_phdbc, g_pGlobal->GetMainWndHandle(), (SQLCHAR*)szConnectionString, strlen(szConnectionString), (SQLCHAR*)szOutConnectionString, 512, &pcb, SQL_DRIVER_NOPROMPT);


    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
    {

        // 2008-09-05 by cmkwon, DBServer 연결 실패 시스템 로그 남기기 - 
        g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(CAtumDBManager::Connect2DBServer_) !! %s,%d %s %s %s\r\n"
            , g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (CHAR*)g_pGlobal->GetODBCUID(), (CHAR*)g_pGlobal->GetODBCPASSWORD());


        if (SQL_NULL_HDBC != *i_phdbc) {SQLDisconnect(*i_phdbc);}
    #ifndef _DEBUG
        if (SQL_NULL_HDBC != *i_phdbc) {SQLFreeHandle(SQL_HANDLE_DBC, *i_phdbc);}
    #endif
        if (SQL_NULL_HENV != *i_phenv) {SQLFreeHandle(SQL_HANDLE_ENV, *i_phenv);}

        *i_phdbc        = SQL_NULL_HDBC;
        *i_phenv        = SQL_NULL_HENV;
        return FALSE;
    }

    if (EN_DBCONN_MANUAL_COMMIT == i_connTy)
    {
        SQLSetConnectAttr(*i_phdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);    // set mannualcommit
    }

    // prints DB Info
    char DatabaseName[40];
    char DSN[40];
    SQLSMALLINT StrLen;
    SQLGetInfo(*i_phdbc, SQL_DATABASE_NAME, (SQLPOINTER)DatabaseName, sizeof(DatabaseName), &StrLen);
    SQLGetInfo(*i_phdbc, SQL_DATA_SOURCE_NAME, (SQLPOINTER)DSN, sizeof(DSN), &StrLen);

    ret = SQLAllocHandle(SQL_HANDLE_STMT, *i_phdbc, i_phstmt);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumDBManager::Connect2DBServer(DBWorkerData *io_pDBTlsData, EN_DBCONN_TYPE i_dbConnTy)
/// \brief        
/// \author        cmkwon
/// \date        2006-05-17 ~ 2006-05-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBManager::Connect2DBServer(DBWorkerData *io_pDBTlsData, EN_DBCONN_TYPE i_dbConnTy)
{
    switch (i_dbConnTy)
    {
    case EN_DBCONN_AUTO_COMMIT:
        return Connect2DBServer(&io_pDBTlsData->henv, &io_pDBTlsData->hdbc, &io_pDBTlsData->hstmt, i_dbConnTy);
    case EN_DBCONN_MANUAL_COMMIT:
        return Connect2DBServer(&io_pDBTlsData->henv_mc, &io_pDBTlsData->hdbc_mc, &io_pDBTlsData->hstmt_mc, i_dbConnTy);
    case EN_DBCONN_GLOG_COMMIT:        // 2013-06-20 by jhseol,bckim GLog 보완
        return Connect2DBServer(&io_pDBTlsData->henv_GLog, &io_pDBTlsData->hdbc_GLog, &io_pDBTlsData->hstmt_GLog, i_dbConnTy);
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            void CAtumDBManager::DisconnectDBServer(SQLHENV *i_phenv, SQLHDBC    *i_phdbc, SQLHSTMT *i_phstmt)
/// \brief        
/// \author        cmkwon
/// \date        2006-05-17 ~ 2006-05-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CAtumDBManager::DisconnectDBServer(SQLHENV *i_phenv, SQLHDBC    *i_phdbc, SQLHSTMT *i_phstmt)
{
    // cleanup odbc resources
    if (SQL_NULL_HSTMT != *i_phstmt) {    SQLFreeHandle(SQL_HANDLE_STMT, *i_phstmt);}
    if (SQL_NULL_HDBC != *i_phdbc) {        SQLDisconnect (*i_phdbc);}
#ifndef _DEBUG
    if (SQL_NULL_HDBC != *i_phdbc) {        SQLFreeHandle (SQL_HANDLE_DBC, *i_phdbc);}
#endif
    if (SQL_NULL_HENV != *i_phenv) {        SQLFreeHandle (SQL_HANDLE_ENV, *i_phenv);}

    *i_phstmt        = SQL_NULL_HSTMT;
    *i_phdbc        = SQL_NULL_HDBC;
    *i_phenv        = SQL_NULL_HENV;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn            void CAtumDBManager::DisconnectDBServer(DBWorkerData *io_pDBTlsData, EN_DBCONN_TYPE i_dbConnTy)
/// \brief        
/// \author        cmkwon
/// \date        2006-05-17 ~ 2006-05-17
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
void CAtumDBManager::DisconnectDBServer(DBWorkerData *io_pDBTlsData, EN_DBCONN_TYPE i_dbConnTy)
{
    switch (i_dbConnTy)
    {
    case EN_DBCONN_AUTO_COMMIT:
        DisconnectDBServer(&io_pDBTlsData->henv, &io_pDBTlsData->hdbc, &io_pDBTlsData->hstmt);
        break;
    case EN_DBCONN_MANUAL_COMMIT:
        DisconnectDBServer(&io_pDBTlsData->henv_mc, &io_pDBTlsData->hdbc_mc, &io_pDBTlsData->hstmt_mc);
        break;
    case EN_DBCONN_GLOG_COMMIT:            // 2013-06-20 by jhseol,bckim GLog 보완
        DisconnectDBServer(&io_pDBTlsData->henv_GLog, &io_pDBTlsData->hdbc_GLog, &io_pDBTlsData->hstmt_GLog);
        break;
    }
}

DWORD CAtumDBManager::AtumDBWorker(int i_nAtumDBThreadIndex)
{
    g_pGlobal->WriteSystemLogEX(true, "CAtumDBManager::AtumDBWorker_ DWWorkerThread, \t ThreadID(%6d, 0x%X)\r\n", GetCurrentThreadId(), GetCurrentThreadId());

    auto tlsData = GetDBWorkerTLSDATA();

    ////////////////////////////////////
    // Init DB
    ////////////////////////////////////

    if (!Connect2DBServer(tlsData, EN_DBCONN_AUTO_COMMIT))
    {
        char    szError[1024];
        sprintf(szError, "[Error] CAtumDBManager::Worker Connect2DBServer_1 Error\r\n");
        g_pGlobal->WriteSystemLog(szError);
        DbgOut(szError);
        return -1;
    }
    if (!Connect2DBServer(tlsData, EN_DBCONN_MANUAL_COMMIT))
    {
        char    szError[1024];
        sprintf(szError, "[Error] CAtumDBManager::Worker Connect2DBServer_2 Error\r\n");
        g_pGlobal->WriteSystemLog(szError);
        DbgOut(szError);
        return -1;
    }


    SThreadInfo    stInfo;
    memset(&stInfo, 0x00, sizeof(SThreadInfo));
    stInfo.dwThreadId            = GetCurrentThreadId();
    stInfo.enThreadCheckType    = THREAD_CHECK_TYPE_DB_WORKER;
    m_DBThreadCheck.AddThreadInfo(stInfo);
    
    SThreadInfo* pstInfo = nullptr;

    while (true)
    {
        pstInfo = m_DBThreadCheck.GetThreadInfo(stInfo.dwThreadId);
        
        if (pstInfo) break;
        
        this_thread::sleep_for(100ms);
    }

    // todo : maybe remove this srand call
    ::srand(timeGetTime());            // 2007-04-03 by dhjin
    // todo : deprecate
    random::init();
    

    // Start working
    while (true)
    {
        m_arrmtvectorInputDBQuery[i_nAtumDBThreadIndex].lock();
        if (m_arrmtvectorInputDBQuery[i_nAtumDBThreadIndex].empty())
        {
            m_arrmtvectorInputDBQuery[i_nAtumDBThreadIndex].unlock();
            
            if (m_bShutDownFlag) break;
            
            this_thread::sleep_for(100ms);
            
            continue;
        }        
        m_arrmtvectorInputDBQuery[i_nAtumDBThreadIndex].swap(m_arrmtvectorProcessDBQuery[i_nAtumDBThreadIndex]);
        m_arrmtvectorInputDBQuery[i_nAtumDBThreadIndex].unlock();

        pstInfo->dwLastUseStartTick    = timeGetTime();
        pstInfo->bThreadUseFlag        = true;

        auto nSize = m_arrmtvectorProcessDBQuery[i_nAtumDBThreadIndex].size();
        pstInfo->nQueryArraySize = nSize;
        for (auto i = 0; i < nSize; i++)
        {
            auto dwCurTick = timeGetTime();
            auto& dbquery        = m_arrmtvectorProcessDBQuery[i_nAtumDBThreadIndex][i];
            pstInfo->dwSocketIndex    = i_nAtumDBThreadIndex;
            pstInfo->dwMessageType    = dbquery.enumQueryType;
            pstInfo->dwCharacterUID    = dbquery.nCharacterUID;            
            auto bRet = ProcessServerQuery(dbquery, tlsData->hstmt, tlsData->hstmt_mc, tlsData->hstmt_ExtAuth, tlsData->hstmt_GLog);    // 2013-06-20 by jhseol,bckim GLog 보완    // Process each query
            dbquery.dwProcessedTick = timeGetTime() - dwCurTick;
        }
        auto dwEndTick = timeGetTime();

        if (nSize > 100)
        {
            g_pGlobal->WriteSystemLogEX(false, "DB Index : %2d, Size(%4d) StartTick(%8d) TotalProcessedTime(%8d)\r\n"
                , i_nAtumDBThreadIndex, nSize, pstInfo->dwLastUseStartTick, dwEndTick - pstInfo->dwLastUseStartTick);
        }

        if (pstInfo->bPrintOut)
        {
            g_pGlobal->WriteSystemLogEX(false, "DB Index : %2d, TotalProcessedTime(%8d)==================\r\n",
                i_nAtumDBThreadIndex, dwEndTick - pstInfo->dwLastUseStartTick);

            for (auto i = 0; i < nSize; i++)
            {
                auto& dbquery = m_arrmtvectorProcessDBQuery[i_nAtumDBThreadIndex][i];

                g_pGlobal->WriteSystemLogEX(false, "    ArrayIndex(%4d) : %30s ProcessedTime(%4d) CharacterUID(%6d)\r\n",
                    i, GetDBQueryTypeString(dbquery.enumQueryType), dbquery.dwProcessedTick, dbquery.nCharacterUID);
            }

            g_pGlobal->WriteSystemLog("=======================================================\r\n");
            
            pstInfo->bPrintOut = false;
        }

        m_arrmtvectorProcessDBQuery[i_nAtumDBThreadIndex].clear();
        pstInfo->bThreadUseFlag = false;
    } // end while

// 2006-05-17 by cmkwon, 아래 함수로 처리
//     // cleanup odbc resources
//     SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//     SQLDisconnect(hdbc);
// #ifndef _DEBUG
//     SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
// #endif
//     SQLFreeHandle(SQL_HANDLE_ENV, henv);
// 
//     // for manual commit
//     SQLFreeHandle(SQL_HANDLE_STMT, hstmt_mc);
//     SQLDisconnect(hdbc_mc);
// #ifndef _DEBUG
//     SQLFreeHandle(SQL_HANDLE_DBC, hdbc_mc);
// #endif
//     SQLFreeHandle(SQL_HANDLE_ENV, henv_mc);

    DisconnectDBServer(tlsData, EN_DBCONN_AUTO_COMMIT);
    DisconnectDBServer(tlsData, EN_DBCONN_MANUAL_COMMIT);
    DisconnectDBServer(tlsData, EN_DBCONN_GLOG_COMMIT);    // 2013-06-20 by jhseol,bckim GLog 보완
    //if (!TLSDataClean(lpvData))
    //{
    //    // init error
    //    return -1;
    //}

    return 5;
}

BOOL CAtumDBManager::ExecuteQuery(EnumQueryType type, CIOCPSocket* pIOCPSocket, void *pMsg, void* i_pGeneralParam /* = NULL */, INT64 i_nGeneralParam1 /* = 0 */, INT64 i_nGeneralParam2 /* = 0 */)
{
    BOOL        bRet = FALSE;
    DB_QUERY    dbquery;

    dbquery.pIOCPSocket = pIOCPSocket;
    dbquery.enumQueryType = type;
    dbquery.pQueryParam = (void*)pMsg;
    dbquery.pGeneralParam = i_pGeneralParam;
    dbquery.nGeneralParam1 = i_nGeneralParam1;
    dbquery.nGeneralParam2 = i_nGeneralParam2;

    auto idx = pIOCPSocket ? pIOCPSocket->GetClientArrayIndex() % NUM_OF_SYNC_ODBC_STATEMENT : RANDI(NUM_OF_SYNC_ODBC_STATEMENT);

    auto pOdbcStmt = &m_ArrOdbcStmt[idx];

    pOdbcStmt->Lock();
    SQLHSTMT    hstmt_ms = SQL_NULL_HSTMT;
    SQLHSTMT    hstmt_extAuth = SQL_NULL_HSTMT;
    SQLHSTMT    hstmt_GLog = SQL_NULL_HSTMT;        // 2013-06-20 by jhseol,bckim GLog 보완
    bRet = ProcessServerQuery(dbquery, pOdbcStmt->m_hstmt, hstmt_ms, hstmt_extAuth, hstmt_GLog);    // 2013-06-20 by jhseol,bckim GLog 보완
    pOdbcStmt->Unlock();
    return bRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumDBManager::CheckAndCloseHandleDynamicDBThread(void)
/// \brief        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author        cmkwon
/// \date        2008-12-01 ~ 2008-12-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBManager::CheckAndCloseHandleDynamicDBThread()
{
    mt_auto_lock mtA(&m_mtvectQueryFutureList);

    if (m_mtvectQueryFutureList.empty()) return true;

    
    for (auto itr = m_mtvectQueryFutureList.begin(); itr != m_mtvectQueryFutureList.end(); )
    {
        auto status = itr->wait_for(10ms);

        if (status == future_status::ready)
        {
            itr->get();

            itr = m_mtvectQueryFutureList.erase(itr);
        }

        else ++itr;
    }

    return m_mtvectQueryFutureList.empty();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumDBManager::InsertDBQueryToDynamicQueryList(DB_QUERY *i_pDBQuery)
/// \brief        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author        cmkwon
/// \date        2008-12-01 ~ 2008-12-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBManager::InsertDBQueryToDynamicQueryList(DB_QUERY *i_pDBQuery)
{
    mt_auto_lock mtA(&m_mtvectInputQueryWithNewThread);
    m_mtvectInputQueryWithNewThread.push_back(*i_pDBQuery);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumDBManager::GetDBQueryFromDynamicQueryList(DB_QUERY *o_pDBQuery)
/// \brief        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author        cmkwon
/// \date        2008-12-01 ~ 2008-12-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBManager::GetDBQueryFromDynamicQueryList(DB_QUERY *o_pDBQuery)
{
    mt_auto_lock mtA(&m_mtvectInputQueryWithNewThread);
    if (m_mtvectInputQueryWithNewThread.empty())
    {
        return FALSE;
    }

    mtvectorDB_QUERY::iterator itr(m_mtvectInputQueryWithNewThread.begin());
    *o_pDBQuery        = *itr;

    m_mtvectInputQueryWithNewThread.erase(itr);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            BOOL CAtumDBManager::MakeQueryWithNewThread(EnumQueryType type, CIOCPSocket* pIOCPSocket, void *pMsg, void* i_pGeneralParam/*=NULL*/, INT64 i_nGeneralParam1/*=0*/, INT64 i_nGeneralParam2/*=0*/)
/// \brief        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author        cmkwon
/// \date        2008-12-01 ~ 2008-12-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumDBManager::MakeQueryWithNewThread(EnumQueryType type, CIOCPSocket* pIOCPSocket, void *pMsg, void* i_pGeneralParam/*=NULL*/, INT64 i_nGeneralParam1/*=0*/, INT64 i_nGeneralParam2/*=0*/)
{
    // make query, enqueue
    DB_QUERY dbquery;
    
    dbquery.pIOCPSocket        = pIOCPSocket;
    dbquery.nCharacterUID    = 0;
    dbquery.enumQueryType    = type;
    dbquery.pQueryParam        = (void*)pMsg;
    dbquery.pGeneralParam    = i_pGeneralParam;
    dbquery.nGeneralParam1    = i_nGeneralParam1;
    dbquery.nGeneralParam2    = i_nGeneralParam2;

    InsertDBQueryToDynamicQueryList(&dbquery);

    mt_auto_lock mtQTH(&m_mtvectQueryFutureList);

    CheckAndCloseHandleDynamicDBThread();
    
    try
    {    
        m_mtvectQueryFutureList.lock();
        m_mtvectQueryFutureList.push_back(async(launch::async, DBDynamicWorkerThread, new ATUM_DB_THREAD_BEGIN_DATA { this, 0 }));
        m_mtvectQueryFutureList.unlock();
    }
    catch (const system_error& error)
    {
        g_pGlobal->WriteSystemLogEX(true, "[Error] CAtumDBManager::MakeQueryWithNewThread DBDynamicWorkerThread create error!!\r\n");
        
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn            DWORD CAtumDBManager::AtumDBDynamicWorker(void)
/// \brief        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
/// \author        cmkwon
/// \date        2008-12-01 ~ 2008-12-01
/// \warning    
///
/// \param        
/// \return        
///////////////////////////////////////////////////////////////////////////////
DWORD CAtumDBManager::AtumDBDynamicWorker()
{

    DB_QUERY dbQuery;
    if (FALSE == this->GetDBQueryFromDynamicQueryList(&dbQuery))
    {
        return 60;
    }

    CODBCStatement odbcStmt;
    BOOL bRet = odbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (LPCSTR)g_pGlobal->GetODBCUID(), (LPCSTR)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
    if (FALSE == bRet)
    {
        g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBManager::AtumDBDynamicWorker# connect db fail !! GetLastError(%d)\r\n", GetLastError());
        SetLastError(0);

        // 2008-12-19 by cmkwon, 연결시 에러가 발생한 경우는 쿼리를 벡터에 다시 추가한다.
        this->InsertDBQueryToDynamicQueryList(&dbQuery);
        return 61;
    }

    while (TRUE)
    {
        bRet = this->ProcessDynamicServerQuery(&dbQuery, &odbcStmt);
        if (FALSE == bRet)
        {// 2008-12-01 by cmkwon, 쿼리처리 실패함, 해당 쿼리를 벡터에 다시 추가한다.

            g_pGlobal->WriteSystemLogEX(TRUE, "[ERROR] CAtumDBManager::AtumDBDynamicWorker# call ProcessDynamicServerQuery() error !!, CurThreadID(%d) %d(%s)\r\n"
                , ::GetCurrentThreadId(), dbQuery.enumQueryType, GetDBQueryTypeString(dbQuery.enumQueryType));
            // 2008-12-19 by cmkwon, 에러가 발생하면 쿼리를 다시 추가 할 필요는 없다.
            //this->InsertDBQueryToDynamicQueryList(&dbQuery);
            break;
        }

        if (FALSE == this->GetDBQueryFromDynamicQueryList(&dbQuery))
        {
            break;
        }
    }

    return 62;
}

void CAtumDBManager::ProcessLogMessages(SQLSMALLINT plm_handle_type,
                                        SQLHANDLE &plm_handle,
                                        const char *logstring, BOOL ConnInd, CIOCPSocket *pIOCPSocket)
{
    g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] DBWorkerThreadID(%5d) hstmt(0x%X): %s\r\n", GetCurrentThreadId(), plm_handle, (char*)logstring);

    if (plm_handle == SQL_NULL_HSTMT)
    {
        auto pTlsData = GetDBWorkerTLSDATA();
        BOOL bRet = Connect2DBServer(pTlsData, EN_DBCONN_AUTO_COMMIT);
        if (bRet)
        {
            g_pGlobal->WriteSystemLogEX(TRUE, "[DB Notify] ThreadID(%5d) Connect2DBServer_3 success !!, hstmt(0x%X) dbConnType(%d)\r\n", GetCurrentThreadId(), pTlsData->hstmt, EN_DBCONN_AUTO_COMMIT);
        }
        else
        {
            g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] ThreadID(%5d) Connect2DBServer_3 error !!, dbConnType(%d)\r\n", GetCurrentThreadId(), EN_DBCONN_AUTO_COMMIT);
        }
        bRet = Connect2DBServer(pTlsData, EN_DBCONN_MANUAL_COMMIT);
        if (bRet)
        {
            g_pGlobal->WriteSystemLogEX(TRUE, "[DB Notify] ThreadID(%5d) Connect2DBServer_3 success !!, hstmt(0x%X) dbConnType(%d)\r\n", GetCurrentThreadId(), pTlsData->hstmt_mc, EN_DBCONN_MANUAL_COMMIT);
        }
        else
        {
            g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] ThreadID(%5d) Connect2DBServer_3 error !!, dbConnType(%d)\r\n", GetCurrentThreadId(), EN_DBCONN_MANUAL_COMMIT);
        }

        return;
    }

    RETCODE        plm_retcode                        = SQL_SUCCESS;
    UCHAR        plm_szSqlState[1024]            = "";
    UCHAR        plm_szErrorMsg[1024]            = "";
    SDWORD        plm_pfNativeError                = 0L;
    SWORD        plm_pcbErrorMsg                    = 0;
    SQLSMALLINT    plm_cRecNmbr                    = 1;
    SDWORD        plm_SS_MsgState = 0, plm_SS_Severity = 0;
    SQLINTEGER    plm_Rownumber                    = 0;
    USHORT        plm_SS_Line;
    SQLSMALLINT    plm_cbSS_Procname, plm_cbSS_Srvname;
    SQLCHAR        plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];
    while (plm_retcode != SQL_NO_DATA_FOUND)
    {
        plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle, plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
                plm_szErrorMsg, 1024 - 1, &plm_pcbErrorMsg);

        // Note that if the application has not yet made a
        // successful connection, the SQLGetDiagField
        // information has not yet been cached by ODBC
        // Driver Manager and these calls to SQLGetDiagField
        // will fail.
        if (SQL_INVALID_HANDLE == plm_retcode)
        {// 2006-05-18 by cmkwon
            break;
        }
        if (plm_retcode != SQL_NO_DATA_FOUND)
        {
            if (ConnInd)
            {
                plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr, SQL_DIAG_ROW_NUMBER, &plm_Rownumber, SQL_IS_INTEGER, NULL);
                plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr, SQL_DIAG_SS_LINE, &plm_SS_Line, SQL_IS_INTEGER, NULL);
                plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr, SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState, SQL_IS_INTEGER, NULL);
                plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr, SQL_DIAG_SS_SEVERITY, &plm_SS_Severity, SQL_IS_INTEGER, NULL);
                plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr, SQL_DIAG_SS_PROCNAME, &plm_SS_Procname, sizeof(plm_SS_Procname), &plm_cbSS_Procname);
                plm_retcode = SQLGetDiagField(plm_handle_type, plm_handle, plm_cRecNmbr, SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname, sizeof(plm_SS_Srvname), &plm_cbSS_Srvname);
            }

            g_pGlobal->WriteSystemLogEX(TRUE, "    szSqlState    = %s\r\n",plm_szSqlState);
            g_pGlobal->WriteSystemLogEX(TRUE, "    pfNativeError = %d\r\n",plm_pfNativeError);
            g_pGlobal->WriteSystemLogEX(TRUE, "    szErrorMsg    = %s\r\n",plm_szErrorMsg);
            g_pGlobal->WriteSystemLogEX(TRUE, "    pcbErrorMsg   = %d\r\n\r\n",plm_pcbErrorMsg);

            if ( strcmp((char*)plm_szSqlState, "2400") == 0    )    // 잘못된 커서 상태 오류
            {
                auto pTlsData = GetDBWorkerTLSDATA();
                
                // 2006-05-17 by cmkwon, AutoCommit
                SQLFreeHandle(SQL_HANDLE_STMT, pTlsData->hstmt);
                SQLAllocHandle(SQL_HANDLE_STMT, pTlsData->hdbc, &pTlsData->hstmt);

                // 2006-05-17 by cmkwon, ManualCommit
                SQLFreeHandle(SQL_HANDLE_STMT, pTlsData->hstmt_mc);
                SQLAllocHandle(SQL_HANDLE_STMT, pTlsData->hdbc_mc, &pTlsData->hstmt_mc);
            }
            else if ( strcmp((char*)plm_szSqlState, "08S01") == 0    // Communication link failure
                || strcmp((char*)plm_szSqlState, "01000") == 0    // 일반 네트p 오류
            )
            {
                auto pTlsData = GetDBWorkerTLSDATA();

                // start 2011-07-15 by hskim, 인증 서버 구현 (DB 서버 다운시 죽는 문제 처리)
                if ( NULL == pTlsData )
                {
                    g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] ThreadID(%5d) DBWorker TLSData is NULL!!\r\n", GetCurrentThreadId());
                    
                    // 2011-07-15 by hskim
                    // DB 연결 관련되어 문제가 발행했다면 다시 연결(복구)하는것보다 빠른 운영팀의 확인과 후속 조치 후 재시작이 필요하다고 판단합니다.
                    // 제가 담당하는 이상 이런 정책으로 처리할 예정입니다.
                    continue;
                    //exit(1);
                }
                // end 2011-07-15 by hskim, 인증 서버 구현 (DB 서버 다운시 죽는 문제 처리)                

                DisconnectDBServer(pTlsData, EN_DBCONN_AUTO_COMMIT);
                DisconnectDBServer(pTlsData, EN_DBCONN_MANUAL_COMMIT);
                DisconnectDBServer(pTlsData, EN_DBCONN_GLOG_COMMIT);    // 2013-06-20 by jhseol,bckim GLog 보완

                BOOL bRet = Connect2DBServer(pTlsData, EN_DBCONN_AUTO_COMMIT);
                if (bRet)
                {
                    g_pGlobal->WriteSystemLogEX(TRUE, "[DB Notify] ThreadID(%5d) Connect2DBServer_4 success !!, hstmt(0x%X) dbConnType(%d)\r\n", GetCurrentThreadId(), pTlsData->hstmt, EN_DBCONN_AUTO_COMMIT);
                }
                else
                {
                    g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] ThreadID(%5d) Connect2DBServer_4 error !!, dbConnType(%d)\r\n", GetCurrentThreadId(), EN_DBCONN_AUTO_COMMIT);
                }
                bRet = Connect2DBServer(pTlsData, EN_DBCONN_MANUAL_COMMIT);
                if (bRet)
                {
                    g_pGlobal->WriteSystemLogEX(TRUE, "[DB Notify] ThreadID(%5d) Connect2DBServer_4 success !!, hstmt(0x%X) dbConnType(%d)\r\n", GetCurrentThreadId(), pTlsData->hstmt_mc, EN_DBCONN_MANUAL_COMMIT);
                }
                else
                {
                    g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] ThreadID(%5d) Connect2DBServer_4 error !!, dbConnType(%d)\r\n", GetCurrentThreadId(), EN_DBCONN_MANUAL_COMMIT);
                }

                return;
            }

            if (ConnInd)
            {
                g_pGlobal->WriteSystemLogEX(TRUE, "    ODBCRowNumber = %d\r\n", plm_Rownumber);
                g_pGlobal->WriteSystemLogEX(TRUE, "    SSrvrLine     = %d\r\n", plm_Rownumber);
                g_pGlobal->WriteSystemLogEX(TRUE, "    SSrvrMsgState = %d\r\n",plm_SS_MsgState);
                g_pGlobal->WriteSystemLogEX(TRUE, "    SSrvrSeverity = %d\r\n",plm_SS_Severity);
                g_pGlobal->WriteSystemLogEX(TRUE, "    SSrvrProcname = %s\r\n",plm_SS_Procname);
                g_pGlobal->WriteSystemLogEX(TRUE, "    SSrvrSrvname  = %s\r\n\r\n",plm_SS_Srvname);
            }
        }// End_if (plm_retcode != SQL_NO_DATA_FOUND)
        
        plm_cRecNmbr++;        // Increment to next diagnostic record.

    }// End_while (plm_retcode != SQL_NO_DATA_FOUND)
}

RETCODE CAtumDBManager::GetDBError(SQLHSTMT hstmt, UCHAR *sqlState)
{
    UCHAR plm_szErrorMsg[MAXDBMSGBUFLEN] = "";
    SDWORD plm_pfNativeError = 0L;
    SWORD plm_pcbErrorMsg = 0;
    RETCODE retcode = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, 1, sqlState, &plm_pfNativeError, plm_szErrorMsg, MAXDBMSGBUFLEN - 1, &plm_pcbErrorMsg);

    return retcode;
}

char* CAtumDBManager::GetSqlPattern(const char* str, char* buf)
{
    ///////////////////////////////////////////////////////////////////////////////    
    // 2009-01-28 by cmkwon, 일본 캐릭터명 관련 버그 수정 - 
    util::zero(buf, SIZE_MAX_SQL_PATTERN_BUFFER);

    int        nOffset        = 0;
    int        nLen        = strlen(str);
    char *    pCurStr        = (char*)(str);
    while (pCurStr && pCurStr < str + nLen)
    {
        switch (*pCurStr)
        {
        case '%':
        case '[':
        case '_':
            {
                 if (nOffset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
                 {
                     strcpy(buf, '\0');
                     return buf;
                 }
                buf[nOffset++] = '[';
                buf[nOffset++] = *pCurStr;
                buf[nOffset++] = ']';
            }
            break;
        default:
            {
                BOOL bIsDBSLeadByte = IsDBCSLeadByte(*pCurStr);

                if (FALSE == bIsDBSLeadByte)
                {
                    if (nOffset+1 >= SIZE_MAX_SQL_PATTERN_BUFFER)
                    {
                        strcpy(buf, '\0');
                        return buf;
                    }
                    buf[nOffset++]        = *pCurStr;

                }
                else
                {
                    if (nOffset+2 >= SIZE_MAX_SQL_PATTERN_BUFFER)
                    {
                        strcpy(buf, '\0');
                        return buf;
                    }
                    buf[nOffset++]        = *pCurStr;
                    buf[nOffset++]        = *(pCurStr+1);
                }
            }
        }
        pCurStr = CharNext(pCurStr);        // 다음 문자로 이동
    }
    return buf;
    

// 2009-01-28 by cmkwon, 일본 캐릭터명 관련 버그 수정 - 위와 같이 수정 함.
//     int len = strlen(str);
//     int offset = 0;
// 
//     for (int i = 0; i < len; i++)
//     {
//         if (str[i] == '%')
//         {
//             if (offset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
//             {
//                 strcpy(buf, '\0');
//                 return buf;
//             }
// 
//             buf[offset++] = '[';
//             buf[offset++] = '%';
//             buf[offset++] = ']';
//         }
//         else if (str[i] == '[')
//         {
//             if (offset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
//             {
//                 strcpy(buf, '\0');
//                 return buf;
//             }
// 
//             buf[offset++] = '[';
//             buf[offset++] = '[';
//             buf[offset++] = ']';
//         }
//         else if (str[i] == '_')
//         {
//             if (offset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
//             {
//                 strcpy(buf, '\0');
//                 return buf;
//             }
// 
//             buf[offset++] = '[';
//             buf[offset++] = '_';
//             buf[offset++] = ']';
//         }
//         else
//         {
//             if (offset+1 >= SIZE_MAX_SQL_PATTERN_BUFFER)
//             {
//                 strcpy(buf, '\0');
//                 return buf;
//             }
// 
//             buf[offset++] = str[i];
//         }
//     }
// 
//     buf[offset] = '\0';
//
//    return buf;
}



const char* GetDBQueryTypeString(EnumQueryType qType)
{
    switch (qType)
    {
    case QT_CreateCharacter:                        return "QT_CreateCharacter";
    case QT_DeleteCharacter:                        return "QT_DeleteCharacter";
    case QT_GetCharacterInfoByName:                    return "QT_GetCharacterInfoByName";
    case QT_GetAccountUniqueNumber:                    return "QT_GetAccountUniqueNumber";
    case QT_ConnectGameStart:                        return "QT_ConnectGameStart";
    case QT_GetAllCharacterInfoByID:                return "QT_GetAllCharacterInfoByID";
    case QT_SaveCharacterCriticalData:                return "QT_SaveCharacterCriticalData";
    case QT_ChangeUnitKind:                            return "QT_ChangeUnitKind";
    case QT_ChangeStat:                                return "QT_ChangeStat";
    case QT_ChangeBonusStatPoint:                    return "QT_ChangeBonusStatPoint";
    case QT_ChangeGuild:                            return "QT_ChangeGuild";
    case QT_ChangeFriendGroup:                        return "QT_ChangeFriendGroup";
    case QT_ChangeExp:                                return "QT_ChangeExp";
    case QT_ChangeLevel:                            return "QT_ChangeLevel";
    case QT_ChangeBodyCondition:                    return "QT_ChangeBodyCondition";
//    case QT_ChangePropensity:
//        return "QT_ChangePropensity";
    case QT_ChangeStatus:                            return "QT_ChangeStatus";
    case QT_ChangePKPoint:                            return "QT_ChangePKPoint";
// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 사용하지 않으므로 제거
//    case QT_ChangeRacingPoint:                        return "QT_ChangeRacingPoint";
    case QT_UpdateTotalPlayTime:                    return "QT_UpdateTotalPlayTime";
    case QT_UpdateLastStartedTime:                    return "QT_UpdateLastStartedTime";
    case QT_ChangeHPDPSPEP:                            return "QT_ChangeHPDPSPEP";
    case QT_ChangeCurrentHPDPSPEP:                    return "QT_ChangeCurrentHPDPSPEP";
    case QT_ChangeMapChannel:                        return "QT_ChangeMapChannel";
    case QT_ChangePetInfo:                            return "QT_ChangePetInfo";
    case QT_ChangeDockingItem:                        return "QT_ChangeDockingItem";
    case QT_ChangePosition:                            return "QT_ChangePosition";

    case QT_AuthChatLogin:                            return "QT_AuthChatLogin";
    case QT_SaveLastPartyID:                        return "QT_SaveLastPartyID";
    case QT_EventWarpConnect:                        return "QT_EventWarpConnect";
    case QT_PRE_Login:                                return "QT_PRE_Login";
    case QT_BlockUser:                                return "QT_BlockUser";
    case QT_UnblockUser:                            return "QT_UnblockUser";
    //case QT_MGameLogin:                                return "QT_MGameLogin";
    case QT_LogTotalUser:                            return "QT_LogTotalUser";            // 2007-03-16 by cmkwon
    //case QT_InsertGlogAccount:                        return "QT_InsertGlogAccount";        // 2010-06-01 by shcho, GLogDB 관련 -
    case QT_UpdateAccountLastGameEndDate:            return "QT_UpdateAccountLastGameEndDate";        // 2012-01-13 by hskim, EP4 [동영상 1회 재생]

    case QT_GetStoreItem:                            return "QT_GetStoreItem";
    case QT_ReloadAllEnchant:                        return "QT_ReloadAllEnchant";
    case QT_DeleteStoreItem:                        return "QT_DeleteStoreItem";
    case QT_InsertStoreItem:                        return "QT_InsertStoreItem";
    case QT_InsertStoreItemSkill:                    return "QT_InsertStoreItemSkill";
    case QT_UpdateStoreItemSkillUseTime:            return "QT_UpdateStoreItemSkillUseTime";    // 2006-11-17 by dhjin
    case QT_UpdateItemCount:                        return "QT_UpdateItemCount";
    case QT_UpdateItemNum:                            return "QT_UpdateItemNum";
    case QT_UpdateEndurance:                        return "QT_UpdateEndurance";
    case QT_StoreUpdateColorCode:                    return "QT_StoreUpdateColorCode";
    case QT_UpdateShapeItemNum:                        return "QT_UpdateShapeItemNum";        // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 

    case QT_UpdateItemUsingTimeStamp:                return "QT_UpdateItemUsingTimeStamp";
    case QT_UpdateItemRareFix:                        return "QT_UpdateItemRareFix";
    case QT_InsertDefaultItems:                        return "QT_InsertDefaultItems";
    case QT_ChangeItemPosition:                        return "QT_ChangeItemPosition";
    case QT_UpdateWindowItemList:                    return "QT_UpdateWindowItemList";
    case QT_UpdateItemPossess:                        return "QT_UpdateItemPossess";
    case QT_UpdateItemStorage:                        return "QT_UpdateItemStorage";
    case QT_LoadOneItem:                            return "QT_LoadOneItem";
    case QT_INSERTLOGINITEMEVENT:                    return "QT_INSERTLOGINITEMEVENT";        // 2011-08-25 by shcho, 횟수별 아이템 지급기능 구현
    case QT_CheckEventItem:                            return "QT_CheckEventItem";
    case QT_InsertEventItem:                        return "QT_InsertEventItem";
    case QT_UpdateEventItemFixedPeriod:                return "QT_UpdateEventItemFixedPeriod";    // 2013-02-28 by bckim, 복귀유져 버프추가
    case QT_CheckCouponEvent:                        return "QT_CheckCouponEvent";            // 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
    case QT_GetGuildStoreItem:                        return "QT_GetGuildStoreItem";    
    case QT_UpdateGuildStoreItem:                    return "QT_UpdateGuildStoreItem";    
    case QT_InsertGuildStoreItem:                    return "QT_InsertGuildStoreItem";    
    case QT_DeleteGuildStoreItem:                    return "QT_DeleteGuildStoreItem";    
    case QT_GetLogGuildStoreItem:                    return "QT_GetLogGuildStoreItem";    
    case QT_AllDeleteGuildStoreItem:                return "QT_AllDeleteGuildStoreItem";    
    case QT_Insert2WarpableUserList:                return "QT_Insert2WarpableUserList";    // 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 입장허가 추가
    case QT_DeleteWarpableUser:                        return "QT_DeleteWarpableUser";            // 2007-08-30 by cmkwon, 회의룸 시스템 구현 - 입장허가 삭제
    //case QT_UPDATE_ConnectingServerGroupID:            return "QT_UPDATE_ConnectingServerGroupID";            // 2007-11-06 by cmkwon, 게임 로그 DB 서버 따로 구축하기
    case QT_GiveStoreItem:                            return "QT_GiveStoreItem";            // 2007-11-13 by cmkwon, 선물하기 기능 추가 - 
    case QT_GetLetter:                                return "QT_GetLetter";            // 2008-04-24 by dhjin, EP3 편지 시스템 - DB에서 편지 가져오기 
    case QT_ReadLetter:                                return "QT_ReadLetter";            // 2008-04-24 by dhjin, EP3 편지 시스템 - 편지 읽기
    case QT_DeleteLetter:                            return "QT_DeleteLetter";        // 2008-04-24 by dhjin, EP3 편지 시스템 - 편지 삭제
    case QT_SendLetter:                                return "QT_SendLetter";            // 2008-05-08 by dhjin, EP3 편지 시스템 - 편지 보내기
    case QT_GetAllLetter:                            return "QT_GetAllLetter";        // 2008-05-09 by dhjin, EP3 편지 시스템 - DB에서 전체 편지 가져오기
    case QT_SendAllLetter:                            return "QT_SendAllLetter";        // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 보내기
    case QT_ReadAllLetter:                            return "QT_ReadAllLetter";        // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 읽기
    case QT_DeleteAllLetter:                        return "QT_DeleteAllLetter";    // 2008-05-09 by dhjin, EP3 편지 시스템 - 전체 편지 삭제
    case QT_DeleteOldAllLetter:                        return "QT_DeleteOldAllLetter";    // 2008-05-09 by dhjin, EP3 편지 시스템 - 오래된 전체 편지 삭제

    case QT_GuildCreate:                            return "QT_GuildCreate";
    case QT_GuildAddMember:                            return "QT_GuildAddMember";
    case QT_GuildAddOffMember:                        return "QT_GuildAddOffMember";        // 2008-06-12 by dhjin, EP3 - 여단 수정 사항 - 오프유저 여단 가입
    case QT_GuildLeaveMember:                        return "QT_GuildLeaveMember";
    case QT_GuildBanMember:                            return "QT_GuildBanMember";
    case QT_GuildUpdateMemberCapacity:                return "QT_GuildUpdateMemberCapacity";
    case QT_GuildLoadGuild:                            return "QT_GuildLoadGuild";
    case QT_GuildDismember:                            return "QT_GuildDismember";
    case QT_GuildCancelDismember:                    return "QT_GuildCancelDismember";
    case QT_GuildChangeGuildName:                    return "QT_GuildChangeGuildName";
    case QT_GuildSetGuildMark:                        return "QT_GuildSetGuildMark";
// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 사용하지 안는 함수임
//    case QT_GuildGetGuildMark:                        return "QT_GuildGetGuildMark";
    case QT_GuildSetRank:                            return "QT_GuildSetRank";
    case QT_GuildDeleteGuild:                        return "QT_GuildDeleteGuild";
    case QT_GuildSaveGuildWarPoint:                    return "QT_GuildSaveGuildWarPoint";
    case QT_GuildDeleteGuildUIDOfCharacter:            return "QT_GuildDeleteGuildUIDOfCharacter";
    case QT_GuildAddGuildFame:                        return "QT_GuildAddGuildFame";
    case QT_GuildUpdateCommander:                    return "QT_GuildUpdateCommander";            // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단장 위임
    case QT_GuildNotice:                            return "QT_GuildNotice";                    // 2008-05-20 by dhjin, EP3 - 여단 수정 사항 - 여단 공지
    case QT_GuildGetApplicant:                        return "QT_GuildGetApplicant";                // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
    case QT_GuildGetIntroduction:                    return "QT_GuildGetIntroduction";                // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
    case QT_GuildDeleteIntroduction:                return "QT_GuildDeleteIntroduction";                // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개
    case QT_GetSelfIntroduction:                    return "QT_GetSelfIntroduction";            // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 지원자 소개서 
    case QT_GuildSearchIntroduction:                return "QT_GuildSearchIntroduction";            // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 여단 소개 검색
    case QT_GuildUpdateIntroduction:                return "QT_GuildUpdateIntroduction";            // 2008-05-27 by dhjin,    EP3 - 여단 수정 사항 - 여단 소개 작성
    case QT_GuildUpdateSelfIntroduction:            return "QT_GuildUpdateSelfIntroduction";        // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 작성 
    case QT_GuildDeleteSelfIntroduction:            return "QT_GuildDeleteSelfIntroduction";        // 2008-05-27 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 
    case QT_GuildDeleteSelfIntroductionOffUser:        return "QT_GuildDeleteSelfIntroductionOffUser";        // 2008-06-13 by dhjin, EP3 - 여단 수정 사항 - 자기 소개 지우기 

    case QT_ExecuteTrade:                            return "QT_ExecuteTrade";
    case QT_TradeMoveItem:                            return "QT_TradeMoveItem";

    case QT_InsertEnchant:                            return "QT_InsertEnchant";
    case QT_DeleteAllEnchant:                        return "QT_DeleteAllEnchant";
    case QT_GetAllQuest:                            return "QT_GetAllQuest";
    case QT_InsertQuest:                            return "QT_InsertQuest";
    case QT_UpdateQuestState:                        return "QT_UpdateQuestState";
    case QT_DeleteQuest:                            return "QT_DeleteQuest";
    case QT_UpdateQuestMonsterCount:                return "QT_UpdateQuestMonsterCount";
    case QT_DeleteQuestMonsterCount:                return "QT_DeleteQuestMonsterCount";

    case QT_AuctionGetItemList:                        return "QT_AuctionGetItemList";
    case QT_AuctionRegisterItem:                    return "QT_AuctionRegisterItem";
    case QT_AuctionCancelRegister:                    return "QT_AuctionCancelRegister";
    case QT_AuctionBuyItem:                            return "QT_AuctionBuyItem";
    case QT_GeneralExec:                            return "QT_GeneralExec";
    case QT_FriendInsertFriend:                        return "QT_FriendInsertFriend";
    case QT_FriendDeleteFriend:                        return "QT_FriendDeleteFriend";
    case QT_FriendLoadFriends:                        return "QT_FriendLoadFriends";
    case QT_CashInsertPremiumCard:                    return "QT_CashInsertPremiumCard";
    case QT_CashUpdatePremiumCard:                    return "QT_CashUpdatePremiumCard";
    case QT_CashDeletePremiumCard:                    return "QT_CashDeletePremiumCard";

    case QT_LoadHappyHourEvent:                        return "QT_LoadHappyHourEvent";
    case QT_LoadItemEvent:                            return "QT_LoadItemEvent";    
// 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - 
//    case QT_UpdatePCBangList:                        return "QT_UpdatePCBangList";    
    case QT_Get_QuickSlot:                            return "QT_Get_QuickSlot";            // 2006-09-04 by dhjin
    case QT_Delete_QuickSlot:                        return "QT_Delete_QuickSlot";        // 2006-09-04 by dhjin
    case QT_Update_QuickSlot:                        return "QT_Update_QuickSlot";        // 2006-09-04 by dhjin

    // 2007-02-28 by dhjin, 전략포인트 생성 주기 수정 관련.
    case QT_LoadStrategyPointSummonInfo:                    return "QT_LoadStrategyPointSummonInfo";
    case QT_UpdateStrategyPointSummonInfoBySummon:            return "QT_UpdateStrategyPointSummonInfoBySummon";
    case QT_UpdateStrategyPointSummonInfoBySummonTime:        return "QT_UpdateStrategyPointSummonInfoBySummonTime";
    case QT_InsertStrategyPointSummonInfoBySummon:            return "QT_InsertStrategyPointSummonInfoBySummon";
    case QT_DeleteStrategyPointSummonInfoBySummon:            return "QT_DeleteStrategyPointSummonInfoBySummon";
    case QT_UpdateStrategyPointSummonInfoByNewSummonRange:    return "QT_UpdateStrategyPointSummonInfoByNewSummonRange";
    case QT_LoadStrategyPointSummonRange:                    return "QT_LoadStrategyPointSummonRange";
    case QT_LoadStrategyPointNotSummonTime:                    return "QT_LoadStrategyPointNotSummonTime";
    case QT_LoadStrategyPointNotSummonTimeByAdminTool:        return "QT_LoadStrategyPointNotSummonTimeByAdminTool";

    case QT_UpdateHappyHourEventDate:                return "QT_UpdateHappyHourEventDate";
    case QT_UpdateInfluenceWarData:                    return "QT_UpdateInfluenceWarData";
    case QT_UpdateOwnerOfConflictArea:                return "QT_UpdateOwnerOfConflictArea";
    case QT_UpdateSubleader:                        return "QT_UpdateSubleader";        // 2007-02-13 by dhjin

    //case QT_JamboreeInit:                            return "QT_JamboreeInit";            // 2007-04-10 by cmkwon
    //case QT_JamboreeEntrants:                        return "QT_JamboreeEntrants";        // 2007-04-10 by cmkwon

    case QT_UpdateWarPoint:                            return "QT_UpdateWarPoint";            // 2007-04-25 by dhjin
    case QT_UpdateArenaResult:                        return "QT_UpdateArenaResult";            // 2007-06-07 by dhjin
    case QT_UpdateArenaDisConnect:                    return "QT_UpdateArenaDisConnect";            // 2007-06-07 by dhjin
    case QT_MF_Updata_CharacterArena:                return "QT_MF_Updata_CharacterArena";    // 2012-04-12 by jhseol, 아레나 추가개발 - 보상 : AS에서 받은 CharacterArena 정보 업데이트


    case QT_LoadTutorialComplete:                    return "QT_LoadTutorialComplete";            // 2007-07-06 by dhjin
    case QT_InsertTutorialComplete:                    return "QT_InsertTutorialComplete";            // 2007-07-06 by dhjin

    case QT_SetNPCPossessionOutPost:                return "QT_SetNPCPossessionOutPost";            // 2007-08-21 by dhjin
    case QT_SetGuildPossessionOutPost:                return "QT_SetGuildPossessionOutPost";            // 2007-08-21 by dhjin
    case QT_SetOutPostNextWarTime:                    return "QT_SetOutPostNextWarTime";            // 2007-08-21 by dhjin
    
    case QT_InitExpediencyFund:                        return "QT_InitExpediencyFund";                // 2007-08-22 by dhjin
    case QT_SetExpediencyFund:                        return "QT_SetExpediencyFund";                // 2007-08-22 by dhjin
    case QT_SetExpediencyFundPayBack:                return "QT_SetExpediencyFundPayBack";            // 2007-08-22 by dhjin
    case QT_SetExpediencyFundRate:                    return "QT_SetExpediencyFundRate";            // 2007-08-22 by dhjin
    case QT_DeleteCityLeaderInfo:                    return "QT_DeleteCityLeaderInfo";            // 2007-08-22 by dhjin
    case QT_RegNotice:                                return "QT_RegNotice";                        // 2007-08-22 by dhjin
    case QT_ModifyNotice:                            return "QT_ModifyNotice";                    // 2007-08-22 by dhjin
    case QT_UpdateSecondaryPassword:                return "QT_UpdateSecondaryPassword";        // 2007-09-12 by cmkwon

    case QT_LoadVoterList:                            return "QT_LoadVoterList";        // 2007-10-29 by dhjin
    case QT_LoadLeaderCandidate:                    return "LoadLeaderCandidate";        // 2007-10-29 by dhjin
    case QT_SelectLeaderCandidateInfoByRealTimeVariable:    return "QT_SelectLeaderCandidateInfoByRealTimeVariable";        // 2007-10-30 by dhjin
    case QT_InsertLeaderCandidate:                    return "QT_InsertLeaderCandidate";        // 2007-10-30 by dhjin
    case QT_UpdateLeaderDeleteCandidate:            return "QT_UpdateLeaderDeleteCandidate";        // 2007-10-30 by dhjin
    case QT_UpdateLeaderPollCount:                    return "QT_UpdateLeaderPollCount";        // 2007-10-31 by dhjin
    case QT_InsertVoterList:                        return "QT_InsertVoterList";        // 2007-10-31 by dhjin
    case QT_CheckGiveTarget:                        return "QT_CheckGiveTarget";        // 2007-11-13 by cmkwon, 선물하기 기능 추가 -
    case QT_UpdatePilotFace:                        return "QT_UpdatePilotFace";        // 2007-11-21 by cmkwon, PilotFace 변경 카드 구현 - 

    case QT_InsertNotifyMsg:                        return "QT_InsertNotifyMsg";        // 2007-11-28 by cmkwon, 통지시스템 구현 -
    case QT_GetNotifyMsg:                            return "QT_GetNotifyMsg";            // 2007-11-28 by cmkwon, 통지시스템 구현 -
    case QT_DeleteNotifyMsg:                        return "QT_DeleteNotifyMsg";        // 2007-11-28 by cmkwon, 통지시스템 구현 -
        
    case QT_GetGuildMark:                            return "QT_GetGuildMark";        // 2007-12-07 by dhjin
    
    //////////////////////////////////////////////////////////////////////////
    // 2007-12-28 by dhjin, 아레나 통합 - 
    case QT_ArenaUpdateCharacterInfo:                return "QT_ArenaUpdateCharacterInfo";
    case QT_ArenaGetCharacter:                        return "QT_ArenaGetCharacter";
    case QT_ArenaCopyDBInfo:                        return "QT_ArenaCopyDBInfo";
    case QT_ArenaStartGetCharacter:                    return "QT_ArenaStartGetCharacter";

    //////////////////////////////////////////////////////////////////////////
    // 2008-04-02 by dhjin,    모선전, 거점전 정보창 기획안 -
    case QT_GetLogMSWarInfo:                        return "QT_GetLogMSWarInfo";
    case QT_GetLogSPWarInfo:                        return "QT_GetLogSPWarInfo";
    case QT_UpdateMSWarOptionType:                    return "QT_UpdateMSWarOptionType";
    case QT_InsertMSWarLog:                            return "QT_InsertMSWarLog";        // 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.
    case QT_InsertSPWarLog:                            return "QT_InsertSPWarLog";        // 2008-08-28 by dhjin, 버그 수정, 게임DB에 남겨야 Admintool로 초기화가 가능하다.


    case QT_UpdateDBServerGroup:                    return "QT_UpdateDBServerGroup";        // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
    case QT_CheckConnectableAccount:                return "QT_CheckConnectableAccount";    // 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 

    case QT_GetUserInfo:                            return "QT_GetUserInfo";                    // 2008-06-23 by dhjin, EP3 유저정보옵션 - 다른 유저 정보 요청

// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - DQT_DailyJob 로 변경 함.
//    //////////////////////////////////////////////////////////////////////////
//    // 2008-08-19 by dhjin, MySQL포팅 문제로 MySQL에서 지원하지 않는 MSSQL에 Job관련 작업을 여기서 처리한다.
//    case QT_DailyJob:                                return "QT_DailyJob";

    // 2008-11-04 by dhjin, 럭키머신
    case QT_UpdateLuckyItemDropCount:                return "QT_UpdateLuckyItemDropCount";
    case QT_UpdateLuckyItemStarttime:                return "QT_UpdateLuckyItemStarttime";
        
    //////////////////////////////////////////////////////////////////////////
    // 2009-01-12 by dhjin, 선전 포고
    case QT_UpdateStartDeclarationOfWar:            return "QT_UpdateStartDeclarationOfWar";
    case QT_UpdateEndDeclarationOfWar:                return "QT_UpdateEndDeclarationOfWar";
    case QT_UpdateMSWarStartTime:                    return "QT_UpdateMSWarStartTime";        
    case QT_UpdateNickName:                            return "QT_UpdateNickName";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case QT_GetSelfRanking:                            return "QT_GetSelfRanking";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

    case QT_ChangeItemWithItemMatching:                return "QT_ChangeItemWithItemMatching";        // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

    case QT_ChangeStartCityMapIndex:                return "QT_ChangeStartCityMapIndex";        // 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
    case QT_ChangeAddedInventoryCount:                return "QT_ChangeAddedInventoryCount";        // 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 

    case QT_LoadInfinityImpute:                        return "QT_LoadInfinityImpute";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 -     인피 귀속 정보 가져오기
    case QT_InsertInfinityImpute:                    return "QT_InsertInfinityImpute";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 -     인피 귀속 정보 추가
    case QT_UpdateInfinityImpute:                    return "QT_UpdateInfinityImpute";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 -     인피 완료 
    case QT_ResetInfinityImpute:                    return "QT_ResetInfinityImpute";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 -     해당 인피 리셋으로 정보 삭제
    case QT_ArenaCopyInfinityDBInfo:                return "QT_ArenaCopyInfinityDBInfo";    // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 아레나DB에 복사하기
    case QT_CharacterSaveDataInfinityFin:            return "QT_CharacterSaveDataInfinityFin";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 케릭터 정보 수정
    case QT_InfinityFinUpdateItem:                    return "QT_InfinityFinUpdateItem";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 정보 수정
    case QT_InfinityFinInsertItem:                    return "QT_InfinityFinInsertItem";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 추가ㅣ
    case QT_InfinityInsertLog:                        return "QT_InfinityInsertLog";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 로그
    case QT_CharacterSaveDataInfinityFinByDisconnect:    return "QT_CharacterSaveDataInfinityFinByDisconnect";        // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 케릭터 정보 수정
    case QT_InfinityFinUpdateItemByDisconnect:            return "QT_InfinityFinUpdateItemByDisconnect";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 정보 수정
    case QT_InfinityFinInsertItemByDisconnect:            return "QT_InfinityFinInsertItemByDisconnect";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 강제 종료 후 MainSvr에 인피 진행하면서 수정된 아이템 추가ㅣ
    case QT_ResetInfinityImputeByServerStart:            return "QT_ResetInfinityImputeByServerStart";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 서버 시작시 인피 리셋
    case QT_InfinityComBackPostWork:                    return "QT_InfinityComBackPostWork";                        // 2012-01-16 by hskim, 통계 - 화패

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
//    case QT_LoadTenderInfo:                            return "QT_LoadTenderInfo";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Tender

    case QT_CashLoadPremiumCard:                    return "QT_CashLoadPremiumCard";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 프리미엄 정보 가져오기
    case QT_LoadInfinityShopInfo:                    return "QT_LoadInfinityShopInfo";                // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
    case QT_InfinityUpdateUserMapInfo:                return "QT_InfinityUpdateUserMapInfo";            // 2010-04-06 by cmkwon, 인피2차 추가 수정 - 
    case QT_LoadBurningMap:                            return "QT_LoadBurningMap";                        // 2010-08-05 by dhjin, 버닝맵 -
    case QT_Log_UserGetTenderItem:                    return "QT_Log_UserGetTenderItem";                // 2010-06-25 by shcho, 인피니티 관련로그 찍기 - 습득 아이템 정보 DB저장
    case QT_NA:                                        return "QT_NA";

    ///////////////////////////////////////////////////////////////////////////////
    // 2011-08-22 by hskim, 파트너 시스템 2차 - 기능 구현

    case QT_PetUpdateInfinityFin:                    return "QT_PetUpdateInfinityFin";
    case QT_PetSetName:                                return "QT_PetSetName";
    case QT_PetSetExpRatio:                            return "QT_PetSetExpRatio";
    case QT_PetSetLevel:                            return "QT_PetSetLevel";
    case QT_PetSetExp:                                return "QT_PetSetExp";
    case QT_PetSetSocket:                            return "QT_PetSetSocket";
    case QT_PetSetKitSlot:                            return "QT_PetSetKitSlot";
    case QT_PetSetAutoSkillSlot:                    return "QT_PetSetAutoSkillSlot";
    case QT_PetChangeSocketOwner:                    return "QT_PetChangeSocketOwner";                // 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
    case QT_StoreGetItemOne:                        return "QT_StoreGetItemOne";                // 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
    case QT_UpgradePet:                                return "QT_UpgradePet";                        // 2015-06-22 Future, upgrading of Pets with sockets

    ///////////////////////////////////////////////////////////////////////////////
    // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
    case DQT_DailyJob:                                return "DQT_DailyJob";
    case DQT_UpdatePCBangList:                        return "DQT_UpdatePCBangList";        // 2008-12-19 by cmkwon, QT_UpdatePCBangList->DQT_UpdatePCBangList 변경 - 
    case DQT_ReloadWRKServiceList:                    return "DQT_ReloadWRKServiceList";    // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case DQT_ReloadWRKLevel:                        return "DQT_ReloadWRKLevel";        // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case DQT_ReloadWRKFame:                            return "DQT_ReloadWRKFame";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case DQT_ReloadWRKPVP:                            return "DQT_ReloadWRKPVP";            // 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
    case DQT_LoadInfluenceRate:                        return "DQT_LoadInfluenceRate";                // 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - 

    case DQT_NA:                                    return "DQT_NA";            // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 -         

    case QT_ArenaCharacterReset:                    return "QT_ArenaCharacterReset";    // 2012-10-21 by jhseol, 아레나 버그수정 - 아레나 종료시 케릭터 정보 리셋.

    //////////////////////////////////////////////////////////////////////////////
    // 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전
    case QT_LoadRenewalStrategyPointSummonInfo:        return "QT_LoadRenewalStrategyPointSummonInfo";
    case QT_UpdateStrategyPointSummonInfo:            return "QT_UpdateStrategyPointSummonInfo";
    // end 2012-11-13 by jhseol, 전쟁 시스템 리뉴얼 - 거점전

    case QT_ArenaCharacterReload:                    return "QT_ArenaCharacterReload";    // 2012-12-18 by jhseol, 아레나 캐릭터 리셋 후 캐릭터 정보 다시 로딩하기.

    case QT_InsertStoreItemFromXMLRPC:                return "QT_InsertStoreItemFromXMLRPC";        // 2013-03-13 by hskim, 웹 캐시 상점
    case QT_DeleteStoreItemFromXMLRPC:                return "QT_DeleteStoreItemFromXMLRPC";        // 2013-03-13 by hskim, 웹 캐시 상점
    case QT_GetCharacterInfoFromXMLRPC:                return "QT_GetCharacterInfoFromXMLRPC";        // 2013-03-13 by hskim, 웹 캐시 상점
    case QT_LoadCashBuyDate:                        return "QT_LoadCashBuyDate";        // 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급
    case QT_InsertCashBuyDate:                        return "QT_InsertCashBuyDate";        // 2013-03-29 by jhseol, 아이템 이벤트 - 매월 첫 결재시 선물지급

    case QT_LoadMonthlyArmorEvent:                    return "QT_LoadMonthlyArmorEvent";    // 2013-04-18 by jhseol,bckim 이달의 아머 - 이달의 아머 이벤트 로드

    case QT_UpdateInfluenceConsecutiveVictorites:    return "QT_UpdateInfluenceConsecutiveVictorites";    // 2013-05-09 by hskim, 세력 포인트 개선
        
    case QT_GetTemporarySystemInfomation:            return "QT_GetTemporarySystemInfomation";    // 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
    case QT_GetStoreExtension:                        return "QT_GetStoreExtension";                // 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
    case QT_InsertStoreExtension:                    return "QT_InsertStoreExtension";            // 2013-05-20 by hskim, [보안 시스템] 비정상적인 방법으로 아이템 추가 방지
    case QT_CollectionArmorListLoad:                return "QT_CollectionArmorListLoad";        // 2013-05-31 by jhseol,bckim 아머 컬렉션 - 아머 컬렉션 로드
    case QT_CollectionArmorUpdate:                    return "QT_CollectionArmorUpdate";            // 2013-05-31 by jhseol,bckim 아머 컬렉션 - 아머 컬렉션 저장

    case QT_GetCharacterMultipleIPStatus:            return "QT_GetCharacterMultipleIPStatus";    // 2015-11-24 Future, Multiple IP Restriction

    case QT_NationTransfer:                            return "QT_NationTransfer";                    // 2015-12-17 Future, Nation Change Card

    // 2016-01-09 Future, Pre Account Block improvements
    case QT_LoadBlockedAccounts:                    return "QT_LoadBlockedAccounts";
    case QT_LoadBlockedMACs:                        return "QT_LoadBlockedMACs";
    case QT_BlockMAC:                                return "QT_BlockMAC";
    case QT_UnblockMAC:                                return "QT_UnblockMAC";
    
    case QT_ChangePropensity:                        return "QT_ChangePropensity";
    case QT_ChangeInfluenceType:                    return "QT_ChangeInfluenceType";
    case QT_ReloadAdminAutoNotice:                    return "QT_ReloadAdminAutoNotice";
    case QT_UpdateItemCoolingTimeStamp:                return "QT_UpdateItemCoolingTimeStamp";
    case QT_GetEnchantCount:                        return "QT_GetEnchantCount";
    case QT_GetAddExpRestExperienceCount:            return "QT_GetAddExpRestExperienceCount";
    case QT_SetAddExpRestExperienceCount:            return "QT_SetAddExpRestExperienceCount";
    case QT_GetFixedTerm:                            return "QT_GetFixedTerm";
    case QT_InsertFixedTerm:                        return "QT_InsertFixedTerm";
    case QT_DeleteFixedTerm:                        return "QT_DeleteFixedTerm";
    case QT_FL_LOG_LOGIN:                            return "QT_FL_LOG_LOGIN";
    case QT_FL_LOG_LOGOUT:                            return "QT_FL_LOG_LOGOUT";
    case QT_FL_LOG_LEVEL:                            return "QT_FL_LOG_LEVEL";
    case QT_FL_LOG_EXP:                                return "QT_FL_LOG_EXP";
    case QT_FL_LOG_STAT:                            return "QT_FL_LOG_STAT";
    case QT_FL_LOG_GAME_START:                        return "QT_FL_LOG_GAME_START";
    case QT_FL_LOG_GAME_END:                        return "QT_FL_LOG_GAME_END";
    case QT_FL_LOG_QUEST_COMPLETION:                return "QT_FL_LOG_QUEST_COMPLETION";
    case QT_FL_LOG_QUEST_COMPENSATION:                return "QT_FL_LOG_QUEST_COMPENSATION";
    case QT_FL_LOG_PKWIN:                            return "QT_FL_LOG_PKWIN";
    case QT_FL_LOG_PKLOSS:                            return "QT_FL_LOG_PKLOSS";
    case QT_FL_LOG_DEAD:                            return "QT_FL_LOG_DEAD";
    case QT_FL_LOG_WARP:                            return "QT_FL_LOG_WARP";
    case QT_FL_LOG_SPEED_HACK:                        return "QT_FL_LOG_SPEED_HACK";
    case QT_FL_LOG_CREATE_CHARACTER:                return "QT_FL_LOG_CREATE_CHARACTER";
    case QT_FL_LOG_DELETE_CHARACTER:                return "QT_FL_LOG_DELETE_CHARACTER";
    case QT_FL_LOG_MEMORY_HACK:                        return "QT_FL_LOG_MEMORY_HACK";
    case QT_FL_LOG_PvEWIN:                            return "QT_FL_LOG_PvEWIN";
    case QT_FL_LOG_PvELOSS:                            return "QT_FL_LOG_PvELOSS";
    case QT_FL_LOG_ITEM_CHARACTER_ITEM:                return "QT_FL_LOG_ITEM_CHARACTER_ITEM";
    case QT_FL_LOG_ITEM_GET_ITEM:                    return "QT_FL_LOG_ITEM_GET_ITEM";
    case QT_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND:        return "QT_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND";
    case QT_FL_LOG_ITEM_INSERT_TO_STORE:            return "QT_FL_LOG_ITEM_INSERT_TO_STORE";
    case QT_FL_LOG_ITEM_DRAW_FROM_STORE:            return "QT_FL_LOG_ITEM_DRAW_FROM_STORE";
    case QT_FL_LOG_ITEM_TRADE_SEND:                    return "QT_FL_LOG_ITEM_TRADE_SEND";
    case QT_FL_LOG_ITEM_TRADE_RECV:                    return "QT_FL_LOG_ITEM_TRADE_RECV";
    case QT_FL_LOG_GUILD_STORE:                        return "QT_FL_LOG_GUILD_STORE";
    case QT_FL_LOG_ITEM_THROWAWAY_ITEM:                return "QT_FL_LOG_ITEM_THROWAWAY_ITEM";
    case QT_FL_LOG_ITEM_BUY_ITEM:                    return "QT_FL_LOG_ITEM_BUY_ITEM";
    case QT_FL_LOG_ITEM_SELL_ITEM:                    return "QT_FL_LOG_ITEM_SELL_ITEM";
    case QT_FL_LOG_ITEM_USE_ITEM:                    return "QT_FL_LOG_ITEM_USE_ITEM";
    case QT_FL_LOG_ITEM_USE_ENERGY:                    return "QT_FL_LOG_ITEM_USE_ENERGY";
    case QT_FL_LOG_ITEM_USE_ENCHANT:                return "QT_FL_LOG_ITEM_USE_ENCHANT";
    case QT_FL_LOG_ITEM_STORE_ITEM:                    return "QT_FL_LOG_ITEM_STORE_ITEM";
    case QT_FL_LOG_ITEM_USE_MIX:                    return "QT_FL_LOG_ITEM_USE_MIX";
    case QT_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN:            return "QT_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN";
    case QT_FL_LOG_SERVER_INFO_MAP:                    return "QT_FL_LOG_SERVER_INFO_MAP";
    case QT_FL_LOG_SERVER_INFO_TOTAL:                return "QT_FL_LOG_SERVER_INFO_TOTAL";
    case QT_FL_LOG_MONSTER_BOSS:                    return "QT_FL_LOG_MONSTER_BOSS";
    case QT_FL_LOG_START_FIELD_SERVER:                return "QT_FL_LOG_START_FIELD_SERVER";
    case QT_FL_LOG_MS_WAR:                            return "QT_FL_LOG_MS_WAR";
    case QT_FL_LOG_SP_WAR:                            return "QT_FL_LOG_SP_WAR";
    case QT_FL_LOG_STATISTICS_MONEY:                return "QT_FL_LOG_STATISTICS_MONEY";
    case QT_FL_GLOG_BUY_CASH_ITEM:                    return "QT_FL_GLOG_BUY_CASH_ITEM";
    case QT_FL_GLOG_CHARACTER_PLAY_TIME:            return "QT_FL_GLOG_CHARACTER_PLAY_TIME";
    case QT_FL_GLOG_CONNECT_TOTAL_USER_COUNT:        return "QT_FL_GLOG_CONNECT_TOTAL_USER_COUNT";
    case QT_FL_GLOG_CONNECT_ZONE_USER_COUNT:        return "QT_FL_GLOG_CONNECT_ZONE_USER_COUNT";
    case QT_FL_GLOG_EVENT_PARTICIPATION_RATE:        return "QT_FL_GLOG_EVENT_PARTICIPATION_RATE";
    case QT_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc:        return "QT_FL_GLOG_TB_USER_LocalUserConnectInfo_DelCountInc";
    case QT_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc:        return "QT_FL_GLOG_TB_USER_LocalUserConnectInfo_NewCountInc";
    case QT_FL_GLOG_TB_CONNECT_USER:                return "QT_FL_GLOG_TB_CONNECT_USER";
    case QT_FL_GLOG_TB_ITEM_BUY:                    return "QT_FL_GLOG_TB_ITEM_BUY";
    case QT_FL_GLOG_TB_CONCURRENT_USER:                return "QT_FL_GLOG_TB_CONCURRENT_USER";
    case QT_FL_GLOG_TB_USER_PLAYTIME:                return "QT_FL_GLOG_TB_USER_PLAYTIME";
    case QT_AUTH_CONFIRM:                            return "QT_AUTH_CONFIRM";
    case QT_AUTH_INSERT_CONFIRM_LOG:                return "QT_AUTH_INSERT_CONFIRM_LOG";
    case QT_ArenaPlayCount:                            return "QT_ArenaPlayCount";
    case QT_FL_LOG_ACCOUNTCONNECT:                    return "QT_FL_LOG_ACCOUNTCONNECT";
    case QT_FL_LOG_CHARDATA:                        return "QT_FL_LOG_CHARDATA";
    case QT_FL_LOG_ITEM:                            return "QT_FL_LOG_ITEM";
    case QT_FL_LOG_ITEMSTATE:                        return "QT_FL_LOG_ITEMSTATE";
    case QT_FL_LOG_SERVER:                            return "QT_FL_LOG_SERVER";
    case QT_GetShapeStatLevel:                        return "QT_GetShapeStatLevel";
    case QT_InsertShapeStatLevel:                    return "QT_InsertShapeStatLevel";
    case QT_FL_LOG_ITEM_TRY_ENCHANT_INFO:            return "QT_FL_LOG_ITEM_TRY_ENCHANT_INFO";
    case QT_AccountInflChange:                        return "QT_AccountInflChange";

    default: return "QT_UNKNOWN";
    }
}
