// CouponManager.h: interface for the CCouponManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COUPONMANAGER_H__DFACFBB5_6EBA_4824_8D76_FC831AE5EA90__INCLUDED_)
#define AFX_COUPONMANAGER_H__DFACFBB5_6EBA_4824_8D76_FC831AE5EA90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ODBCStatement.h"
#include "AtumProtocol.h"


class CFieldIOCP;
class CCouponManager  
{
public:
	CCouponManager();
	~CCouponManager();

	BOOL InitCouponManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD);
	void CleanCouponManager(void);
	BOOL ResetCouponManager(void);

	BOOL GetCouponByNumber(SCOUPON *o_pCoupon, char *i_szCouponNumber);
	BOOL UseCoupon(INT i_CouponUID, char *i_szAccountName);
protected:
	BOOL QP_LoadCouponByNumber(SCOUPON *o_pCoupon, char *i_szCouponNumber);
	BOOL QP_UseCoupon(INT i_CouponUID, char *i_szAccountName);

protected:
	CFieldIOCP			*m_pFieldIOCP12;
	CODBCStatement		m_ODBCStmt3;

public:
	mt_lock				m_mtlock;
};

#endif // !defined(AFX_COUPONMANAGER_H__DFACFBB5_6EBA_4824_8D76_FC831AE5EA90__INCLUDED_)
