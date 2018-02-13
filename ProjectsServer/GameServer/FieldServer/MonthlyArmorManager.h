// MonthlyArmorManager.h: interface for the CMonthlyArmorManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MonthlyArmorManager_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_)
#define AFX_MonthlyArmorManager_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef	mt_vector<MONTHLY_ARMOR_EVNET_INFO>	mtvectMONTHLY_ARMOR_EVNET_INFO;

class CFieldIOCPSocket;
class CMonthlyArmorManager  
{
public:
	CMonthlyArmorManager(CFieldIOCP *i_pFieldIOCP);
	virtual ~CMonthlyArmorManager();
	BOOL InitMonthlyArmorEventManager(vectMONTHLY_ARMOR_EVNET_INFO *i_pMonthlyArmorEventList);
	mtvectMONTHLY_ARMOR_EVNET_INFO * GetVectMonthlyArmorEventListPtr(void);

	void CheckMonthlyArmorEventItem(ITEM_GENERAL* io_pItemG, CFieldIOCPSocket* i_pSock = NULL);
	void SendMonthlyArmorEventListToAllClients();
	void CheckMonthlyArmorEventCollection(CFieldIOCPSocket* i_pSock, ItemNum_t i_nShapeItemNum);	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 하나의 컬렉션에 대하여 이달의 어머 이벤트 정보 보내기
	void SendMonthlyArmorEventAllCollectionList(CFieldIOCPSocket* i_pSock);							// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 모든 컬렉션에 대하여 이달의 어머 이벤트 정보 보내기
protected:
	mtvectMONTHLY_ARMOR_EVNET_INFO	m_mtMonthlyArmorEventList;

	CFieldIOCP *ms_pFieldIOCP;
};

#endif // !defined(AFX_MonthlyArmorManager_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_)
