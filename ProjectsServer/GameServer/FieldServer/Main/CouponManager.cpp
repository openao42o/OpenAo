// CouponManager.cpp: implementation of the CCouponManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CouponManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCouponManager::CCouponManager()
{

}

CCouponManager::~CCouponManager()
{

}

BOOL CCouponManager::InitCouponManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD)
{
	mt_auto_lock mtA(&m_mtlock);
	m_pFieldIOCP12				= i_pFieldIOCP;	
	BOOL bRet = m_ODBCStmt3.Init(i_szDSN, i_szUID, i_szPASSWORD);
	if(FALSE == bRet)
	{
		return FALSE;
	}

	return TRUE;
}
void CCouponManager::CleanCouponManager(void)
{
	m_ODBCStmt3.Clean();
}
BOOL CCouponManager::ResetCouponManager(void)
{
	return TRUE;
}


BOOL CCouponManager::GetCouponByNumber(SCOUPON *o_pCoupon, char *i_szCouponNumber)
{	
	return QP_LoadCouponByNumber(o_pCoupon, i_szCouponNumber);
}

BOOL CCouponManager::QP_LoadCouponByNumber(SCOUPON *o_pCoupon, char *i_szCouponNumber)
{
	if(NULL == i_szCouponNumber){										return FALSE;}
	if(0 == strncmp(i_szCouponNumber, "", SIZE_MAX_COUPON_NUMBER)){		return FALSE;}

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_Coupon_LoadCouponByNumber
		@i_CouponNumber			VARCHAR(30)			-- 쿠폰 넘버	
	AS
		SELECT * 
			FROM td_CouponList 
			WHERE CouponNumber = @i_CouponNumber
	GO
	**************************************************************************/
	SQLINTEGER					arrCB[7] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
											,SQL_NTS,SQL_NTS};
	SQLBindParameter(m_ODBCStmt3.m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_COUPON_NUMBER, 0, i_szCouponNumber, 0, &arrCB[1]);
	BOOL bRet = m_ODBCStmt3.ExecuteQuery((char*)(PROCEDURE_080822_0194));
	if(FALSE == bRet)
	{
		m_ODBCStmt3.FreeStatement();		
		return FALSE;
	}

	arrCB[1] = SQL_NTS;
	SQL_TIMESTAMP_STRUCT		arrSqlTime[2];		util::zero(arrSqlTime, sizeof(arrSqlTime[0])*2);
	SQLBindCol(m_ODBCStmt3.GetSTMTHandle(), 1, SQL_C_ULONG, &o_pCoupon->nCouponUniqueNumber, 0,						&arrCB[1]);
	SQLBindCol(m_ODBCStmt3.GetSTMTHandle(), 2, SQL_C_CHAR, o_pCoupon->szCouponNumber, SIZE_MAX_COUPON_NUMBER,		&arrCB[2]);
	SQLBindCol(m_ODBCStmt3.GetSTMTHandle(), 3, SQL_C_TIMESTAMP, &arrSqlTime[0], 0,									&arrCB[3]);	
	SQLBindCol(m_ODBCStmt3.GetSTMTHandle(), 4, SQL_C_ULONG, &o_pCoupon->bUsedFlag, 0,								&arrCB[4]);
	SQLBindCol(m_ODBCStmt3.GetSTMTHandle(), 5, SQL_C_CHAR, o_pCoupon->szUsedAccountName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[5]);
	SQLBindCol(m_ODBCStmt3.GetSTMTHandle(), 6, SQL_C_TIMESTAMP, &arrSqlTime[1], 0,									&arrCB[6]);
	if (SQL_NO_DATA == SQLFetch(m_ODBCStmt3.GetSTMTHandle()))
	{
		m_ODBCStmt3.FreeStatement();
		return FALSE;
	}

	o_pCoupon->atimeExpireTime	= arrSqlTime[0];
	o_pCoupon->atimeUsedTime	= arrSqlTime[1];		
	m_ODBCStmt3.FreeStatement();
	return TRUE;
}

BOOL CCouponManager::UseCoupon(INT i_CouponUID, char *i_szAccountName)
{
	return QP_UseCoupon(i_CouponUID, i_szAccountName);
}
BOOL CCouponManager::QP_UseCoupon(INT i_CouponUID, char *i_szAccountName)
{	
	if(0 == i_CouponUID
		|| 0 == strncmp(i_szAccountName, "", SIZE_MAX_ACCOUNT_NAME))
	{
		return FALSE;
	}

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE atum_Coupon_UseCoupon
		@i_UniqueNumber			INT,				-- 쿠폰 유니크 넘버
		@i_UsedAccountName		VARCHAR(20),		-- 쿠폰 사용 AccountName
		@i_UsedTime				VARCHAR(30)			-- 쿠폰 사용 시간(거의 현재 시간과 동일)
	AS
		UPDATE td_CouponList
			SET UsedAccountName = @i_UsedAccountName, UsedTime = @i_UsedTime
			WHERE UniqueNumber = @i_UniqueNumber
	GO
	**************************************************************************/

	SQLINTEGER		arrCB[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	ATUM_DATE_TIME atimeCur { true };
	char szCurTimeBuf[SIZE_MAX_SQL_DATETIME_STRING];
	atimeCur.GetSQLDateTimeString(szCurTimeBuf, SIZE_MAX_SQL_DATETIME_STRING);	
	SQLBindParameter(m_ODBCStmt3.m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_CouponUID, 0,								&arrCB[1]);
	SQLBindParameter(m_ODBCStmt3.m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccountName, 0,		&arrCB[2]);
	SQLBindParameter(m_ODBCStmt3.m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SQL_DATETIME_STRING, 0, szCurTimeBuf, 0,	&arrCB[3]);
	BOOL bRet = m_ODBCStmt3.ExecuteQuery((char*)(PROCEDURE_080822_0195));
	if(FALSE == bRet)
	{
		m_ODBCStmt3.FreeStatement();
		return FALSE;
	}
	m_ODBCStmt3.FreeStatement();
	return TRUE;
}