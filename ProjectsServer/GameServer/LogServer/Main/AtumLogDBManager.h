// AtumLogDBManager.h: interface for the CAtumLogDBManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMLOGDBMANAGER_H__27E199C3_F294_4E0C_A634_0E7656E7339A__INCLUDED_)
#define AFX_ATUMLOGDBMANAGER_H__27E199C3_F294_4E0C_A634_0E7656E7339A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumDBManager.h"

// start 2011-12-12 by hskim, GLog 2차
struct QPARAM_GLOG_USER_INFO_COUNT_INC
{
	char		szAccountName[SIZE_MAX_ACCOUNT_NAME];
};

struct QPARAM_GLOG_CONNECT_USER_ACCOUNT_NAME
{
	char		szAccountName[SIZE_MAX_ACCOUNT_NAME];
};
// end 2011-12-12 by hskim, GLog 2차

class CLogIOCP;
class CAtumLogDBManager : public CAtumDBManager  
{
public:
	CAtumLogDBManager(CLogIOCP *i_pLogIOCP);
	virtual ~CAtumLogDBManager();


	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog 보완

	void QP_FL_LOG_LOGIN(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_LOGOUT(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_LEVEL(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_EXP(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_STAT(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_GAME_START(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_GAME_END(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_QUEST_COMPLETION(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_PKWIN(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_PKLOSS(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_DEAD(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_WARP(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_SPEED_HACK(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_CREATE_CHARACTER(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_DELETE_CHARACTER(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_MEMORY_HACK(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_PvEWIN(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_PvELOSS(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_CHARACTER_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_GET_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_ADD_ITEM_BY_COMMAND(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_INSERT_TO_STORE(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_DRAW_FROM_STORE(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_TRADE_SEND(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_TRADE_RECV(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_GUILD_STORE(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_THROWAWAY_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_BUY_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_SELL_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_USE_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_USE_ENERGY(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_USE_ENCHANT(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_STORE_ITEM(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_USE_MIX(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_ITEM_USE_CHANGE_CHARACTER_NAME_ORIGIN(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_SERVER_INFO_MAP(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_SERVER_INFO_TOTAL(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_MONSTER_BOSS(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_START_FIELD_SERVER(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FL_LOG_MS_WAR(DB_QUERY q, SQLHSTMT hstmt);	// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 모선전 로그 추가
	void QP_FL_LOG_SP_WAR(DB_QUERY q, SQLHSTMT hstmt);	// 2008-04-01 by dhjin, 모선전, 거점전 정보창 기획안 - 거점전 로그 추가
	void QP_FL_LOG_STATISTICS_MONEY(DB_QUERY q, SQLHSTMT hstmt);	// 2012-01-16 by hskim, 통계 - 화패
	void QP_FL_LOG_ITEM_TRY_ENCHANT_INFO(DB_QUERY q, SQLHSTMT hstmt);		// 2013-07-01 by bckim, 10인챈트 미만 하이퍼카드처리, 로그추가, 운영툴 보완

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, 쿼리별로 스레드를 만들어 처리하는 시스템 구축 - 
	virtual BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

protected:
	CLogIOCP		*m_pLogIOCP2;
};

#endif // !defined(AFX_ATUMLOGDBMANAGER_H__27E199C3_F294_4E0C_A634_0E7656E7339A__INCLUDED_)
