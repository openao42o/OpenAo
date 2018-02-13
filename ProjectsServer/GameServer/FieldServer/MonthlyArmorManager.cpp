// MonthlyArmorManager.cpp: implementation of the CMonthlyArmorManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"
#include "MonthlyArmorManager.h"
#include "FieldGlobal.h"			// 2008-01-17 by cmkwon, S_F: 시스템 로그 추가 - 헤더파일 추가

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonthlyArmorManager::CMonthlyArmorManager(CFieldIOCP *i_pFieldIOCP)
{
	ms_pFieldIOCP = i_pFieldIOCP;
	m_mtMonthlyArmorEventList.reserve(10);
}

CMonthlyArmorManager::~CMonthlyArmorManager()
{
	m_mtMonthlyArmorEventList.clear();
}

mtvectMONTHLY_ARMOR_EVNET_INFO * CMonthlyArmorManager::GetVectMonthlyArmorEventListPtr()
{
	return &m_mtMonthlyArmorEventList;
}

BOOL CMonthlyArmorManager::InitMonthlyArmorEventManager(vectMONTHLY_ARMOR_EVNET_INFO *i_pMonthlyArmorEventList)
{
	// 2008-01-17 by cmkwon, S_F: 시스템 로그 추가
	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] MonthlyArmorEvent reloaded !!, MonthlyArmorEvent Count(%d)\r\n", i_pMonthlyArmorEventList->size());

	mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());				// 2006-08-24 by cmkwon

	m_mtMonthlyArmorEventList.clear();			// 2006-09-12 by cmkwon, 무조건 초기화 처리해야함

	if(i_pMonthlyArmorEventList->empty())
	{
		return FALSE;
	}
	
	m_mtMonthlyArmorEventList.assign(i_pMonthlyArmorEventList->begin(), i_pMonthlyArmorEventList->end());
	SendMonthlyArmorEventListToAllClients();
	return TRUE;
}

void CMonthlyArmorManager::CheckMonthlyArmorEventItem(ITEM_GENERAL* io_pItemG, CFieldIOCPSocket* i_pSock /*= NULL*/)
{
	if ( NULL == io_pItemG )
	{
		return;
	}

	if ( ITEMKIND_DEFENSE != io_pItemG->Kind && FALSE == io_pItemG->ItemInfo->IsExistDesParam(DES_SHAPE_ITEM) )
	{
		return;
	}

	if ( (ITEMKIND_DEFENSE == io_pItemG->Kind && io_pItemG->FixedTermShape.nStatLevel > SHAPE_STAT_INIT_LEVEL ) ||		// 인첸트 외변킷이 적용된 아머 인지 체크
		(TRUE == io_pItemG->ItemInfo->IsExistDesParam(DES_COLLECTION_ARMOR_INDEX) && io_pItemG->ItemInfo->SkillLevel > 0) )		// 인첸트 외변킷 인지 체크
	{
		mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());
		
		ATUM_DATE_TIME atCurrentDate { true };
		ItemNum_t tmBeforeOptionItemNum		= io_pItemG->nMonthlyOptionItemNum;
		ATUM_DATE_TIME tmBeforeEventDate	= io_pItemG->atMonthlyEventEndDate;
		
		io_pItemG->nMonthlyOptionItemNum	= 0;
		io_pItemG->atMonthlyEventEndDate.Reset();
		
		mtvectMONTHLY_ARMOR_EVNET_INFO::iterator itrEvent = m_mtMonthlyArmorEventList.begin();
		for ( ; itrEvent != m_mtMonthlyArmorEventList.end() ; itrEvent++ )
		{
			if ( atCurrentDate >= itrEvent->atStartDate && atCurrentDate < itrEvent->atEndDate )
			{
				if ( (ITEMKIND_DEFENSE == io_pItemG->Kind && io_pItemG->ShapeItemNum == itrEvent->nArmorSourceIndex) ||		// 아머 이면서 외형정보가 같은지 체크
					(io_pItemG->ItemInfo->IsExistDesParam(DES_SHAPE_ITEM) && io_pItemG->ItemInfo->LinkItem == itrEvent->nArmorSourceIndex) )	// 외변킷 아이템 이며 변경할 외형이 같은지 체크
				{
					io_pItemG->nMonthlyOptionItemNum = itrEvent->nOptionItemNum;
					io_pItemG->atMonthlyEventEndDate = itrEvent->atEndDate;
					break;	// 더 있을지도 모르지만 그냥 무시하고 여기서 stop, 가장 첫번째 이벤트만 적용
				}
			}
		}
		mtA.auto_unlock_cancel();
		
		if ( NULL != i_pSock )
		{
			if ( tmBeforeOptionItemNum != io_pItemG->nMonthlyOptionItemNum && tmBeforeEventDate != io_pItemG->atMonthlyEventEndDate )
			{
				if ( WEAR_ATTACHED == io_pItemG->Wear )
				{
					ITEM *pBeforeItem = ms_pFieldIOCP->GetItemInfo(tmBeforeOptionItemNum);
					ITEM *pAfterItem = ms_pFieldIOCP->GetItemInfo(io_pItemG->nMonthlyOptionItemNum);
					if ( NULL != pBeforeItem )
					{
						i_pSock->ReleaseItemDesParam(pBeforeItem);
					}
					if ( NULL != pAfterItem )
					{
						i_pSock->ApplyItemDesParam(pAfterItem);
					}
				}
				INIT_MSG_OF_SIZE(MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO, T_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO, pUpdateArmorEventInfo, pUpdateArmorEventInfoBuf);
				pUpdateArmorEventInfo->ItemUID			= io_pItemG->UniqueNumber;
				pUpdateArmorEventInfo->nOptionItemNum	= io_pItemG->nMonthlyOptionItemNum;
				pUpdateArmorEventInfo->atEndDate		= io_pItemG->atMonthlyEventEndDate;
				i_pSock->SendAddData(pUpdateArmorEventInfoBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO));
			}
		}
	}
}

void CMonthlyArmorManager::SendMonthlyArmorEventListToAllClients()
{
	CFieldIOCPSocket *pSock = NULL;

	for (int i = (CLIENT_INDEX_START_NUM); i < ms_pFieldIOCP->GetArrayClientSize(); i++)
	{
		pSock = (CFieldIOCPSocket*)ms_pFieldIOCP->GetIOCPSocket(i);
		if ( NULL != pSock )
		{
			if ( TRUE == pSock->IsValidCharacter(FALSE) )
			{
				mt_auto_lock igLock(&(pSock->m_ItemManager.m_mapItemGeneral));
				mtmapUID2ItemGeneral::iterator itrItem = pSock->m_ItemManager.m_mapItemGeneral.begin();
				for(; pSock->m_ItemManager.m_mapItemGeneral.end() != itrItem; itrItem++)
				{
					ITEM_GENERAL *pItemG = itrItem->second;
					CheckMonthlyArmorEventItem(pItemG, pSock);
				}
				SendMonthlyArmorEventAllCollectionList(pSock);	// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 이달의 어머 이벤트 정보 보내기
			}
		}
	}
}

// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 이달의 어머 이벤트 정보 보내기
void CMonthlyArmorManager::CheckMonthlyArmorEventCollection(CFieldIOCPSocket* i_pSock, ItemNum_t i_nShapeItemNum)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - 하나의 컬렉션에 대하여 이달의 어머 이벤트 정보 보내기
	if ( NULL == i_pSock )
	{
		// 에러
		return;
	}
	mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());

	COLLECTION_INFO *tmCollectionPtr = i_pSock->m_ArmorCollection.GetCollectionInfoPtr(i_nShapeItemNum);
	if ( NULL != tmCollectionPtr )
	{
		ATUM_DATE_TIME atCurrentDate { true };
		mtvectMONTHLY_ARMOR_EVNET_INFO::iterator itrEvent = m_mtMonthlyArmorEventList.begin();
		for ( ; itrEvent != m_mtMonthlyArmorEventList.end() ; itrEvent++ )
		{
			if ( atCurrentDate >= itrEvent->atStartDate && atCurrentDate < itrEvent->atEndDate && tmCollectionPtr->ShapeNum == itrEvent->nArmorSourceIndex )
			{
				INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, pSend, SendBuf);
				pSend->CollectionShapeNum	= tmCollectionPtr->ShapeNum;
				pSend->nOptionItemNum		= itrEvent->nOptionItemNum;
				pSend->atEndDate			= itrEvent->atEndDate;
				i_pSock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO));
			}
		}
	}

	mtA.auto_unlock_cancel();
#endif
}

void CMonthlyArmorManager::SendMonthlyArmorEventAllCollectionList(CFieldIOCPSocket* i_pSock)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - 모든 컬렉션에 대하여 이달의 어머 이벤트 정보 보내기
	if ( NULL == i_pSock )
	{
		// 에러
		return;
	}
	mt_auto_lock mtA(this->GetVectMonthlyArmorEventListPtr());
	
	i_pSock->SendAddMessageType(T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INIT);
	
	ATUM_DATE_TIME atCurrentDate { true };
	mtvectMONTHLY_ARMOR_EVNET_INFO::iterator itrEvent = m_mtMonthlyArmorEventList.begin();
	for ( ; itrEvent != m_mtMonthlyArmorEventList.end() ; itrEvent++ )
	{
		if ( atCurrentDate >= itrEvent->atStartDate && atCurrentDate < itrEvent->atEndDate )
		{
			COLLECTION_INFO *tmCollectionPtr = i_pSock->m_ArmorCollection.GetCollectionInfoPtr(itrEvent->nArmorSourceIndex);
			if ( NULL != tmCollectionPtr )
			{
				INIT_MSG_WITH_BUFFER(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, T_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO, pSend, SendBuf);
				pSend->CollectionShapeNum	= tmCollectionPtr->ShapeNum;
				pSend->nOptionItemNum		= itrEvent->nOptionItemNum;
				pSend->atEndDate			= itrEvent->atEndDate;
				i_pSock->SendAddData(SendBuf, MSG_SIZE(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO));
			}
		}
	}
	
	mtA.auto_unlock_cancel();
#endif
}
// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 이달의 어머 이벤트 정보 보내기