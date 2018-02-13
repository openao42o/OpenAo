// StoreData.h: interface for the CArmorCollectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMORCOLLECTMANAGER_H__0D996EFC_1166_485D_9F11_608DD1565445__INCLUDED_)
#define AFX_ARMORCOLLECTMANAGER_H__0D996EFC_1166_485D_9F11_608DD1565445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StoreData.h"

//////////////////////////////////////////////////////////////////////////////
/// \class		CArmorCollectManager
///
/// \brief		
/// \author		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
/// \version	
/// \date		2013-05-28 ~ 2013-05-28
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CArmorCollectManager  
{	
public:
	CArmorCollectManager();
	virtual ~CArmorCollectManager();

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
public:
	////////////////// 아머 컬렉션 정보 //////////////////
	void AddArmorCollectInfo(ITEM* pItem);	
	struArmorCollectionInfo *GetArmorCollectInfo(INT nUnitCollectType, INT nItemNum);
	void ReleaseArmorCollectInfo();
	CVecArmorCollecInfo* GetVecArmorCollectInfo(INT nUnitCollectType);	
	void SetArmorCollectInfo(MSG_FC_COLLECTION_INFO* pMsg);	// 아머 컬력션 종료 시간
	INT Convert_UNITKIND_To_UnitCollectType(USHORT ushUnitKind);

	struArmorCollectionInfo *GetArmorCollectInfo_SourceIndex(INT nUnitCollectType, INT nItemNum);	// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
	void  RqCollectionShapeChange(ItemUID_t uTargetItemUID,ItemNum_t nShapeNum);

// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머
	void SetMontylyArmorCollectInit();
	void SetMontylyArmorCollectInfo(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pMsg);
	MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* GetMonthlyArmorCollectInfo(INT nUnitCollectType, INT nItemNum);
// end 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머

protected:
	CVecArmorCollecInfo		m_vecArmorCollect[MAX_COLLECT_UNIT];
	CVecMontylyYArmorInfo	m_vecMonthlyArmorCollect[MAX_COLLECT_UNIT];		// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머
	
#endif
};

#endif // !defined(AFX_ARMORCOLLECTMANAGER_H__0D996EFC_1166_485D_9F11_608DD1565445__INCLUDED_)
