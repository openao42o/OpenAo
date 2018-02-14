//Copyright [2002] MasangSoft
#ifndef COMMON_ATUMDBMANAGER_H_
#define COMMON_ATUMDBMANAGER_H_

#include <thread>
#include <future>

#include <sql.h>
#include "mt_stl.h"
#include "ThreadCheck.h"
#include "windows.h"
#include "AtumDBHelper.h"
#include "IOCPSocket.h"
#include "QueryTypes.h"

using namespace std;

#define MAXDBMSGBUFLEN                        255
#define SIZE_MAX_SQL_PATTERN_BUFFER            64

#define COUNT_MAX_DBWORKER_THREADS            100

#define COUNT_PRE_SERVER_DBWORKER            10
#define COUNT_LOG_SERVER_DBWORKER            15        // 2006-05-21 by cmkwon
#define COUNT_IM_SERVER_DBWORKER            15        // 2006-05-21 by cmkwon
#define COUNT_FIELD_SERVER_DBWORKER            25        // 2006-05-21 by cmkwon

DWORD WINAPI DBWorkerThread(LPVOID lpParam);
DWORD WINAPI DBDynamicWorkerThread(LPVOID lpParam);        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 

class CAtumDBManager;
class CIOCPSocket;
class CIOCP;


struct DB_QUERY
{
    CIOCPSocket*    pIOCPSocket;
    UID32_t            nCharacterUID;
    EnumQueryType    enumQueryType;
    void            *pQueryParam;
    void            *pGeneralParam;        // General-Purpose void* Parameter
    INT64            nGeneralParam1;        // General-Purpose INT   Parameter
    INT64            nGeneralParam2;        // General-Purpose INT   Parameter
    DWORD            dwProcessedTick;    // 쿼리가 처리되는 시간

    DB_QUERY()
    {
        memset(this, 0x00, sizeof(*this));
    }
};

typedef mt_vector<DB_QUERY>                mtvectorDB_QUERY;
typedef mt_vector<HANDLE>                mtvectHANDLE;        // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 

const char* GetDBQueryTypeString(EnumQueryType qType);

enum EN_DBCONN_TYPE
{    EN_DBCONN_AUTO_COMMIT        = 0,    // 2006-05-17 by cmkwon
    EN_DBCONN_MANUAL_COMMIT        = 1,    // 2006-05-17 by cmkwon
    EN_DBCONN_GLOG_COMMIT                // 2013-06-20 by jhseol,bckim GLog 보완
};                                

struct DBWorkerData
{
    DBWorkerData()
    {        
        henv            = SQL_NULL_HENV;
        hdbc            = SQL_NULL_HDBC;
        hstmt            = SQL_NULL_HSTMT;

        henv_ExtAuth    = SQL_NULL_HENV;        // 2006-05-17 by cmkwon
        hdbc_ExtAuth    = SQL_NULL_HDBC;        // 2006-05-17 by cmkwon
        hstmt_ExtAuth    = SQL_NULL_HSTMT;        // 2006-05-17 by cmkwon

        henv_mc            = SQL_NULL_HENV;
        hdbc_mc            = SQL_NULL_HDBC;
        hstmt_mc        = SQL_NULL_HSTMT;

        henv_GLog        = SQL_NULL_HENV;        // 2013-06-20 by jhseol,bckim GLog 보완
        hdbc_GLog        = SQL_NULL_HDBC;        // 2013-06-20 by jhseol,bckim GLog 보완
        hstmt_GLog        = SQL_NULL_HSTMT;        // 2013-06-20 by jhseol,bckim GLog 보완
    }

    SQLHSTMT GetSQLHSTMT(EN_DBCONN_TYPE i_dbConnTy) const
    {
        switch (i_dbConnTy)
        {
        case EN_DBCONN_AUTO_COMMIT:
            return hstmt;

        case EN_DBCONN_MANUAL_COMMIT:
            return hstmt_mc;

        case EN_DBCONN_GLOG_COMMIT:
            return hstmt_GLog;
        
        default: return nullptr;
        }
    }

    SQLHENV            henv;
    SQLHDBC            hdbc;
    SQLHSTMT        hstmt;

    SQLHENV            henv_ExtAuth;        // 2006-05-17 by cmkwon
    SQLHDBC            hdbc_ExtAuth;        // 2006-05-17 by cmkwon
    SQLHSTMT        hstmt_ExtAuth;        // 2006-05-17 by cmkwon

    SQLHENV            henv_mc;        // for manual commit
    SQLHDBC            hdbc_mc;        // for manual commit
    SQLHSTMT        hstmt_mc;        // for manual commit

    SQLHENV            henv_GLog;        // 2013-06-20 by jhseol,bckim GLog 보완
    SQLHDBC            hdbc_GLog;        // 2013-06-20 by jhseol,bckim GLog 보완
    SQLHSTMT        hstmt_GLog;        // 2013-06-20 by jhseol,bckim GLog 보완
};

class CODBCStatement;
class CAtumDBManager : public CAtumDBHelper
{

// public member functions

public:
    explicit CAtumDBManager(DWORD i_dwWorkerCounts);
    virtual ~CAtumDBManager();

    BOOL Init();
    void Clean();
    bool IsReadyAtumDBManager() const;
    BOOL Connect2DBServer(SQLHENV *i_phenv, SQLHDBC    *i_phdbc, SQLHSTMT *i_phstmt, EN_DBCONN_TYPE i_dbConnTy = EN_DBCONN_AUTO_COMMIT);
    BOOL Connect2DBServer(DBWorkerData *io_pDBTlsData, EN_DBCONN_TYPE i_dbConnTy);
    void DisconnectDBServer(SQLHENV *i_phenv, SQLHDBC    *i_phdbc, SQLHSTMT *i_phstmt);
    void DisconnectDBServer(DBWorkerData *io_pDBTlsData, EN_DBCONN_TYPE i_dbConnTy);
    DWORD AtumDBWorker(int i_nAtumDBThreadIndex);


    // General-function (if it was general it would be a template at first place)
    // 2016-06-30 panoskj made this generic
    template<typename a_t = INT64, typename b_t = INT64>
    void MakeAndEnqueueQuery(EnumQueryType type, CIOCPSocket* pIOCPSocket, void *pMsg, void* i_pGeneralParam = nullptr, a_t i_nGeneralParam1 = 0, b_t i_nGeneralParam2 = 0)    // Asynchronous Excution
    {
        // make query, enqueue
        DB_QUERY dbquery;

        dbquery.pIOCPSocket = pIOCPSocket;
        dbquery.nCharacterUID = 0;
        dbquery.enumQueryType = type;
        dbquery.pQueryParam = pMsg;
        dbquery.pGeneralParam = i_pGeneralParam;
        dbquery.nGeneralParam1 = static_cast<INT64>(i_nGeneralParam1);
        dbquery.nGeneralParam2 = static_cast<INT64>(i_nGeneralParam2);

        auto nDBThreadIndex = pIOCPSocket ? pIOCPSocket->GetClientArrayIndex() % m_dwCountDBWorker : RANDI(m_dwCountDBWorker);

        m_arrmtvectorInputDBQuery[nDBThreadIndex].pushBackLock(dbquery);
    }
    
    BOOL ExecuteQuery(EnumQueryType type, CIOCPSocket* pIOCPSocket, void *pMsg, void* i_pGeneralParam = NULL, INT64 i_nGeneralParam1 = 0, INT64 i_nGeneralParam2 = 0);        // Synchronous Excution

    // 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
    mtvectorDB_QUERY    m_mtvectInputQueryWithNewThread;
    mt_vector<future<DWORD>> m_mtvectQueryFutureList;
    BOOL CheckAndCloseHandleDynamicDBThread();
    BOOL InsertDBQueryToDynamicQueryList(DB_QUERY *i_pDBQuery);
    BOOL GetDBQueryFromDynamicQueryList(DB_QUERY *o_pDBQuery);
    BOOL MakeQueryWithNewThread(EnumQueryType type, CIOCPSocket* pIOCPSocket, void *pMsg, void* i_pGeneralParam = NULL, INT64 i_nGeneralParam1 = 0, INT64 i_nGeneralParam2 = 0);
    DWORD AtumDBDynamicWorker();
    
    virtual BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) = 0;
    

    // TLS
    //BOOL TLSDataInit(LPVOID &lpvData);
    //BOOL TLSDataClean(LPVOID &lpvData);


    static DBWorkerData* GetDBWorkerTLSDATA() { return &workerdata; }

private:

    thread_local static DBWorkerData workerdata;

    
// private member functions

private:
    virtual BOOL ProcessServerQuery(DB_QUERY q, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog) = 0;        // 2013-06-20 by jhseol,bckim GLog 보완

public:
    char* GetSqlPattern(const char* str, char* buf);    // where의 string 비교 문에서 %, [, ], _를 제거하기 위해
                                                        // 검색에서만 사용하면 됨
                                                        // buffer size는 max 64임!

    RETCODE GetDBError(SQLHSTMT hstmt, UCHAR *sqlState);

    void ProcessLogMessages(SQLSMALLINT plm_handle_type,
        SQLHANDLE &plm_handle, const char *logstring, BOOL ConnInd, CIOCPSocket *pIOCPSocket = NULL);

    SThreadInfo *CheckDBThread(DWORD i_dwThreadIdToExclude) {    return m_DBThreadCheck.CheckThreadInfo(i_dwThreadIdToExclude);}


// public member variables

public:
    thread                m_threadDBWorker[COUNT_MAX_DBWORKER_THREADS];
    //HANDLE            m_hDBWorkerThread[COUNT_MAX_DBWORKER_THREADS];
    CThreadCheck        m_DBThreadCheck;


// private member variables

protected:
    DWORD                m_dwCountDBWorker;
    BOOL                m_bShutDownFlag;
    mtvectorDB_QUERY    m_arrmtvectorInputDBQuery[COUNT_MAX_DBWORKER_THREADS];
    mtvectorDB_QUERY    m_arrmtvectorProcessDBQuery[COUNT_MAX_DBWORKER_THREADS];

    CODBCStatement        *m_ArrOdbcStmt;        // Synchronous Execution용
};

#endif // COMMON_ATUMDBMANAGER_H_
