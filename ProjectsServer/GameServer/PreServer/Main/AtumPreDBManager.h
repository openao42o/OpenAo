#ifndef __ATUM_PRE_DB_MANAGER_H__
#define __ATUM_PRE_DB_MANAGER_H__

#include "AtumDBManager.h"


struct MSG_DB_TOTAL_USER
{
	char		szServerGroup[SIZE_MAX_SERVER_NAME];
	int			nUserCounts;
};

struct QPARAM_GLOG_INSERT_ACCOUNT
{
	char		szAccountName[SIZE_MAX_ACCOUNT_NAME];
};

struct QPARAM_UPDATE_ACCOUNT_LASTGAMEENDDATE
{
	UID32_t		AccountUniqueNumber;
};

struct QPARAM_BLOCK_UNBLOCK_MAC_ADDRESS
{
	char	MACAddress[SIZE_MAX_MAC_ADDRESS];
};

class CPreIOCP;

class CAtumPreDBManager : public CAtumDBManager
{
public:
	explicit CAtumPreDBManager(CPreIOCP *pPreIOCP) : CAtumDBManager { COUNT_PRE_SERVER_DBWORKER }, m_pPreIOCP { pPreIOCP } { }

	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog) override;		// 2013-06-20 by jhseol,bckim GLog º¸¿Ï

	// For PreServer
	void QP_PRE_Login(DB_QUERY q, SQLHSTMT &hstmt);
	void QP_BlockUser(DB_QUERY q, SQLHSTMT hstmt);
	void QP_UnblockUser(DB_QUERY q, SQLHSTMT hstmt);
	void QP_LogTotalUser(DB_QUERY q, SQLHSTMT hstmt);
	void QP_UpdateAccountLastGameEndDate(DB_QUERY q, SQLHSTMT hstmt);

	// 2016-01-09 Future, improved Account block loading
	void QP_LoadBlockedAccounts(DB_QUERY dbquery, SQLHSTMT hstmt);

	// 2016-01-09 Future, MAC Blocking
	void QP_LoadBlockedMACs(DB_QUERY dbquery, SQLHSTMT & hstmt);
	void QP_BlockMAC(DB_QUERY dbquery, SQLHSTMT hstmt);
	void QP_UnblockMAC(DB_QUERY dbquery, SQLHSTMT hstmt);


	BOOL IsExistAccount(char *pAccountName, SQLHSTMT &hstmt);

	BOOL LogInsertBlockUnblock(SQLHSTMT hstmt, SBLOCKED_ACCOUNT_INFO *i_pBlockAccInfo, T1<T0::FL_LOG> i_byLogType);

	BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC) override;

	CPreIOCP	*m_pPreIOCP;
};

#endif