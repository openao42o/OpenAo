// StoreData.cpp: implementation of the CArmorCollectManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumDatabase.h"

#include "ArmorCollectManager.h"

extern CAtumDatabase * g_pDatabase;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct ARMOR_COLLECT_SORT : binary_function<struArmorCollectionInfo*, struArmorCollectionInfo*, bool>
{
	bool operator()(struArmorCollectionInfo* Param1, struArmorCollectionInfo* Param2)
	{
		return Param1->nIndex < Param2->nIndex; 	// 오름 차순 정렬
	}
};

CArmorCollectManager::CArmorCollectManager()
{	
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	ReleaseArmorCollectInfo();
	SetMontylyArmorCollectInit();			// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 
#endif
}

CArmorCollectManager::~CArmorCollectManager()
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	ReleaseArmorCollectInfo();
	SetMontylyArmorCollectInit();			// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 
#endif
}
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-28 by bhsohn 아머 컬렉션 시스템
/// \date		2013-05-28 ~ 2013-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArmorCollectManager::AddArmorCollectInfo(ITEM* pItem)
{
	if(!pItem)
	{
		return;
	}
	
	if(!pItem->IsExistDesParam(DES_COLLECTION_ARMOR_INDEX))
	{
		return;
	}

	int nUnitCollectType = Convert_UNITKIND_To_UnitCollectType(pItem->ReqUnitKind);

	if(nUnitCollectType >= MAX_COLLECT_UNIT)
	{
		return;
	}
	INT nItemNum = pItem->ItemNum;
	INT nShapeNum = pItem->LinkItem;
	struArmorCollectionInfo *pInfo = GetArmorCollectInfo(nUnitCollectType, nItemNum); // 외형킷 아이템 번호로 관리하자
	
	if(pInfo != nullptr)
	{
		// 없을때만 insert
		return;
	}
	INT nIndex = pItem->GetParameterValue(DES_COLLECTION_ARMOR_INDEX);
	

	pInfo = new struArmorCollectionInfo;
	
	pInfo->nIndex			= nIndex;

	memset(&pInfo->struCollectItemInfo, 0x00, sizeof(pInfo->struCollectItemInfo));		

	pInfo->bGetServerInfo  =  false;		// 서버에서 정보가 왔는지 여부
	
	pInfo->struCollectItemInfo.CollectionType   = 1;		// 컬렉션 타입 (1 : 아머)
	pInfo->struCollectItemInfo.AccountUID		= 0;			// 계정 UID
	pInfo->struCollectItemInfo.CharacterUID		= 0;		// 캐릭터 UID
	pInfo->struCollectItemInfo.ShapeNum			= nShapeNum;			// 외형 번호(소스인덱스)
	pInfo->struCollectItemInfo.ShapeItemNum		= nItemNum;		// 외형킷 아이템 번호
	pInfo->struCollectItemInfo.EnchantLevel		= 0;		// 강화 단계
	pInfo->struCollectItemInfo.RemainSeconds	= 0;		// 남은 시간(초) : 최대 136년
//	pInfo->struCollectItemInfo.EndTime			= atimeEndTime;			// 현제 시간에서 남은 시간을 더하여 산출된 외형의 절대시간
	pInfo->struCollectItemInfo.ActivedCount		= 0;		// 외형이 적용된 아이템의 수 (0:적용된 아머가 없음=사용중이 아님, 1이상 사용중)
	
	m_vecArmorCollect[nUnitCollectType].push_back(pInfo);

	sort(m_vecArmorCollect[nUnitCollectType].begin(), m_vecArmorCollect[nUnitCollectType].end(), ARMOR_COLLECT_SORT());

}

struArmorCollectionInfo *CArmorCollectManager::GetArmorCollectInfo(INT nUnitCollectType, INT nItemNum)
{
	if(nUnitCollectType >= MAX_COLLECT_UNIT)
	{
		return nullptr;
	}
	CVecArmorCollecInfoIterator it = m_vecArmorCollect[nUnitCollectType].begin();
	while(it != m_vecArmorCollect[nUnitCollectType].end())
	{
		struArmorCollectionInfo *pInfo = (*it);
		if(pInfo && (nItemNum == pInfo->struCollectItemInfo.ShapeItemNum)) // 외형킷 아이템 번호
		{
			return pInfo;
		}
		it++;
	}
	return nullptr;
}
// 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
struArmorCollectionInfo *CArmorCollectManager::GetArmorCollectInfo_SourceIndex(INT nUnitCollectType, INT nItemNum)
{
	if(nUnitCollectType >= MAX_COLLECT_UNIT)
	{
		return nullptr;
	}
	CVecArmorCollecInfoIterator it = m_vecArmorCollect[nUnitCollectType].begin();
	while(it != m_vecArmorCollect[nUnitCollectType].end())
	{
		struArmorCollectionInfo *pInfo = (*it);
		if(pInfo && (nItemNum == pInfo->struCollectItemInfo.ShapeNum)) // 소스 인데스 
		{
			return pInfo;
		}
		it++;
	}
	return nullptr;
}
// end 2013-06-10 by ssjung 아머 컬렉션 툴팁 표시
void CArmorCollectManager::ReleaseArmorCollectInfo()
{
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR;nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		CVecArmorCollecInfoIterator it = m_vecArmorCollect[nUnitCollectType].begin();
		while(it != m_vecArmorCollect[nUnitCollectType].end())
		{
			struArmorCollectionInfo* pInfo = *it;
			util::del(pInfo);
			it++;
		}
		m_vecArmorCollect[nUnitCollectType].clear();
	}
}

CVecArmorCollecInfo* CArmorCollectManager::GetVecArmorCollectInfo(INT nUnitCollectType)
{
	if(nUnitCollectType >= MAX_COLLECT_UNIT)
	{
		return nullptr;
	}
	return &m_vecArmorCollect[nUnitCollectType];
}

void CArmorCollectManager::SetArmorCollectInfo(MSG_FC_COLLECTION_INFO* pMsg)
{	
//	int nUnitCollectType = COLLECT_UNIT_BGEAR;	
	INT nItemNum =	pMsg->ShapeItemNum;	
	ITEM* pItem = g_pDatabase->GetItemInfoLoadItemData(nItemNum);
	if(!pItem)
	{
		return;
	}	
	
	int nUnitCollectType = Convert_UNITKIND_To_UnitCollectType(pItem->ReqUnitKind);
	if(MAX_COLLECT_UNIT == nUnitCollectType)
	{
		return;
	}
	
	struArmorCollectionInfo *pInfo = GetArmorCollectInfo(nUnitCollectType, nItemNum);	
	if(NULL == pInfo)
	{		
		return;
	}	
	pInfo->bGetServerInfo  =  true;		
	memcpy(&pInfo->struCollectItemInfo, pMsg, sizeof(MSG_FC_COLLECTION_INFO));

	sort(m_vecArmorCollect[nUnitCollectType].begin(), m_vecArmorCollect[nUnitCollectType].end(), ARMOR_COLLECT_SORT()); // 정렬

}

INT CArmorCollectManager::Convert_UNITKIND_To_UnitCollectType(USHORT ushUnitKind)
{
	if(IS_BT(ushUnitKind)) // B-GEAR
	{
		return COLLECT_UNIT_BGEAR;
	}
	if(IS_OT(ushUnitKind))	// M-GEAR
	{
		return COLLECT_UNIT_MGEAR;
	}
	if(IS_DT(ushUnitKind))	// A-GEAR
	{
		return COLLECT_UNIT_AGEAR;
	}
	if(IS_ST(ushUnitKind))	// I-GEAR
	{
		return COLLECT_UNIT_IGEAR;
	}
	return MAX_COLLECT_UNIT;
}
void  CArmorCollectManager::RqCollectionShapeChange(ItemUID_t uTargetItemUID,// 외형변경을 적용받을 아이템UID
														ItemNum_t nShapeNum)			// 외형 번호(소스인덱스))
{
	MSG_FC_COLLECTION_SHAPE_CHANGE msg;
	memset(&msg, 0x00 ,sizeof(msg));
	
	{
		msg.CollectionType	= COLLECTION_TYPE_ARMOR;									// 컬렉션 타입 (1 : 아머)
		msg.TargetItemUID	= uTargetItemUID;						// 외형변경을 적용받을 아이템UID
		msg.ShapeNum		= nShapeNum;							// 외형 번호(소스인덱스)			
	}
	
	g_pFieldWinSocket->SendMsg( T_FC_COLLECTION_SHAPE_CHANGE, (char*)(&msg), sizeof(MSG_FC_COLLECTION_SHAPE_CHANGE) );	
}

// 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 
void CArmorCollectManager::SetMontylyArmorCollectInit()
{
	int nUnitCollectType = 0;
	for(nUnitCollectType = COLLECT_UNIT_BGEAR; nUnitCollectType < MAX_COLLECT_UNIT; nUnitCollectType++)
	{
		CVecMontylyYArmorInfoIterator it = m_vecMonthlyArmorCollect[nUnitCollectType].begin();
		while(it != m_vecMonthlyArmorCollect[nUnitCollectType].end())
		{
			MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pInfo = *it;
			util::del(pInfo);
			it++;
		}
		m_vecMonthlyArmorCollect[nUnitCollectType].clear();
	}
}
void CArmorCollectManager::SetMontylyArmorCollectInfo(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pMsg)
{
	INT nItemNum =	pMsg->CollectionShapeNum;	
	ITEM* pItem = g_pDatabase->GetItemInfoLoadItemData(nItemNum);
	MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pInfo = nullptr;
	if(!pItem)
	{
		return;
	}	
	
	int nUnitCollectType = Convert_UNITKIND_To_UnitCollectType(pItem->ReqUnitKind);
	if(MAX_COLLECT_UNIT == nUnitCollectType)
	{
		return;
	}
	
	pInfo = GetMonthlyArmorCollectInfo(nUnitCollectType, nItemNum);	
	if(NULL == pInfo)
	{	
		pInfo = new MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO;
		memset(pInfo,0x00, sizeof(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO));		
		m_vecMonthlyArmorCollect[nUnitCollectType].push_back(pInfo);
	}
	memcpy(pInfo, pMsg, sizeof(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO));
	
}
MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* CArmorCollectManager:: GetMonthlyArmorCollectInfo(INT nUnitCollectType, INT nItemNum)
{
	if(nUnitCollectType >= MAX_COLLECT_UNIT)
	{
		return nullptr;
	}
	CVecMontylyYArmorInfoIterator it = m_vecMonthlyArmorCollect[nUnitCollectType].begin();
	while(it != m_vecMonthlyArmorCollect[nUnitCollectType].end())
	{
		MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO *pInfo = (*it);
		if(pInfo && (nItemNum == pInfo->CollectionShapeNum))
		{
			return pInfo;
		}
		it++;
	}
	return nullptr;
}
// end 2013-06-14 by ssjung 아머 컬렉션 시스템 이달의 아머 
#endif // #ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
