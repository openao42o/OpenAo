// BillCrux_BillingLib.h: interface for the BillCrux_BillingLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BILLCRUX_BILLINGLIB_H__ED5B4D47_ECAD_4E49_BD9C_442FAFEE104A__INCLUDED_)
#define AFX_BILLCRUX_BILLINGLIB_H__ED5B4D47_ECAD_4E49_BD9C_442FAFEE104A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID_MAX_STR_LEN			32
#define ITEMNAME_MAX_STR_LEN	50

#define ARG_GAME_SERVICE_ID		5	// 고정값 게임 서비스 아이디

#define ARG_RETURN_SUCCESS				100
#define ERR_ARG_RETURN_INVALED_USER		-100
#define ERR_ARG_RETURN_NOT_NORMAL_USER	-200
#define ERR_ARG_RETURN_NOT_ENOUGH_CASH	-300

class BillCrux_BillingLib  
{
public:
	BillCrux_BillingLib(); 
	virtual ~BillCrux_BillingLib();
	
protected:


};

#endif // !defined(AFX_BILLCRUX_BILLINGLIB_H__ED5B4D47_ECAD_4E49_BD9C_442FAFEE104A__INCLUDED_)
