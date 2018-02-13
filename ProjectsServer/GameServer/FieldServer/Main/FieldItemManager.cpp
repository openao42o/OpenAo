#include "stdafx.h"
#include "FieldItemManager.h"
#include "FieldIOCP.h"
#include "AtumError.h"
#include "AtumLogSender.h"
#include "FieldGlobal.h"
#include "BunchCountableItemCount.h"

#include "FieldParty.h"		// 2013-02-28 by bckim, 복귀유져 버프추가

CFieldItemManager::CFieldItemManager()
{
	m_vectDelayStoredItemInfo.reserve(10);

	m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();

	m_dwTimeLimiteCardItemCheckCounts	= 0;

	m_uid32StoreCharacterUID			= 0;

	this->SetUsingFieldStore(FALSE);		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - CFieldItemManager::CFieldItemManager(), 
	this->SetUsingStore(FALSE);				// 2013-04-12 by jhseol, 필드창고 사용으로 더블인첸트 버그 수정

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버와 아이템 동기화를 위한 처리
	m_mtInfinityInsertItemInfoList.clear();
	m_mtInfinityUpdateItemInfoList.clear();
	m_mtInfinityDeleteItemInfoList.clear();

	m_nInventoryHiddenItemCount = 0;		// 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자
}

void CFieldItemManager::ResetAllItemGenerals()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	
	for (auto& x : m_mapItemGeneral) delete x.second;
	
	m_mapItemGeneral.clear();

	// 2010-06-15 by shcho&hslee 펫시스템 - 소유 펫 정보 초기화.
	mtvectPetCurrentInfo::iterator it_PetCurrentInfo = m_vectPetCurrentInfo.begin();

	for ( ; it_PetCurrentInfo != m_vectPetCurrentInfo.end(); ++it_PetCurrentInfo )
	{
		util::del( *it_PetCurrentInfo );
	}
	m_vectPetCurrentInfo.clear();
	
	// 2008-01-31 by cmkwon, 시간제한 아이템 DB 에 추가 중 체크 - 초기화하기
	m_vectInsertingItemNumList2DB.clear();

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Main서버와 아이템 동기화를 위한 처리
	m_mtInfinityInsertItemInfoList.clearLock();
	m_mtInfinityUpdateItemInfoList.clearLock();
	m_mtInfinityDeleteItemInfoList.clearLock();

	m_nInventoryHiddenItemCount = 0;		// 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자
}

// 2007-01-29 by cmkwon, 인자 추가(BOOL i_bClearEnchat)
void CFieldItemManager::ResetAllItemGeneralsInStore(BOOL i_bClearEnchat/*=TRUE*/)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-29 by cmkwon, 창고 아이템이 소유한 인챈트를 제거한다
	if(i_bClearEnchat
		&& m_pFieldIOCPSocket)
	{
		mt_auto_lock mtE(&m_pFieldIOCPSocket->m_mapEnchant);

		mtmapUID2ItemGeneral::iterator itr(m_mapItemInStore.begin());
		for(; itr != m_mapItemInStore.end(); itr++)
		{
			m_pFieldIOCPSocket->DeleteAllEnchantToItem(itr->first);
		}
	}
	for (auto& x : m_mapItemInStore) delete x.second;
	
	m_mapItemInStore.clear();

	m_uid32StoreCharacterUID	= 0;
}
void CFieldItemManager::ResetAllItemSkills()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for (auto x : m_vectItemSkillPtr) delete x;
	m_vectItemSkillPtr.clear();
}
void CFieldItemManager::ResetUsingGeneralItems(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr(m_vectUsingGeneralItem.begin());
	for(; itr != m_vectUsingGeneralItem.end(); itr++)
	{
		ITEM_GENERAL *pItemG = *itr;
// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldItemManager::ResetUsingGeneralItems#, 아래와 같이 장착된 아이템은 제외
//		if(ITEMKIND_ACCESSORY_TIMELIMIT != pItemG->Kind)
		if(FALSE == IS_ATTACHABLE_ITEM(pItemG->ItemInfo) || WEAR_NOT_ATTACHED == pItemG->Wear)
		{// 2006-10-11 by cmkwon, ITEMKIND_ACCESSORY_TIMELIMIT는 장착 아이템 이므로 여기에서 메모리를 삭제하지 않는다
			util::del(*itr);
		}
	}
	m_vectUsingGeneralItem.clear();
}

// start 2011-09-29 by hskim, 파트너 시스템 2차 - 인피니티에서 돌아올 때 사용
void CFieldItemManager::ResetUsingSocketItems(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr(m_vectUsingGeneralItem.begin());

	while( itr != m_vectUsingGeneralItem.end() )
	{
		ITEM_GENERAL *pItemG = *itr;

		if(ITEMKIND_PET_SOCKET_ITEM == pItemG->Kind)
		{
			DeleteInsertingItemNumList2DB(pItemG->ItemInfo->ItemNum);
			
			util::del(*itr);
			itr = m_vectUsingGeneralItem.erase(itr);

			continue;
		}

		itr++;
	}
}
// end 2011-09-29 by hskim, 파트너 시스템 2차 - 인피니티에서 돌아올 때 사용

void CFieldItemManager::ResetPremiumCardInfo(void)
{
	m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();
}


// 캐릭터의 게임 종료 시 호출
void CFieldItemManager::ResetAllCharacterItems()
{
	ResetUsingGeneralItems();
	ResetAllItemGenerals();
	ResetAllItemGeneralsInStore();
	ResetAllItemSkills();

	this->SetUsingFieldStore(FALSE);	// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - CFieldItemManager::ResetAllCharacterItems, 
	this->SetUsingStore(FALSE);				// 2013-04-12 by jhseol, 필드창고 사용으로 더블인첸트 버그 수정
}

ITEM_GENERAL* CFieldItemManager::GetItemGeneralByUID(UID64_t i_nItemUID, BYTE i_enumStorage)
{
// 2008-07-25 by cmkwon, CFieldItemManager::GetItemGeneralByUID() ItemUID 체크 추가 - 아래와 같이 수정
//	mt_auto_lock mtAuto(&m_mapItemGeneral);
// 	if (i_enumStorage == ITEM_IN_CHARACTER)
// 	{
// 		return m_mapItemGeneral.findNoLock(i_nItemUID);
// 	}
// 	else if (i_enumStorage == ITEM_IN_STORE
// 			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
// 			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
// 	{
// 		return m_mapItemInStore.findNoLock(i_nItemUID);
// 	}
//
//	return NULL;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-25 by cmkwon, CFieldItemManager::GetItemGeneralByUID() ItemUID 체크 추가 - 
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = NULL;
	if (i_enumStorage == ITEM_IN_CHARACTER)
	{
		pItemG = m_mapItemGeneral.findNoLock(i_nItemUID);
	}
	else if (i_enumStorage == ITEM_IN_STORE)
// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::GetItemGeneralByUID#, 
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::GetItemGeneralByUID#, 
		if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
			|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetItemGeneralByUID State error !! %s %s pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItemG, string()), m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
			return NULL;
		}

		pItemG = m_mapItemInStore.findNoLock(i_nItemUID);
	}

	if(NULL == pItemG)
	{// 2008-07-25 by cmkwon, 아이템이 존재 하지 않는다.
		return NULL;
	}

	if(pItemG->UniqueNumber != i_nItemUID)
	{// 2008-07-25 by cmkwon, 찾은 아이템의 ItemUID 를 비교한다.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetItemGeneralByUID ItemUID error !! %s %s %I64d\r\n"
			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(pItemG, string()), i_nItemUID);
		return NULL;
	}

	return pItemG;
}

ITEM_GENERAL* CFieldItemManager::GetFirstItemGeneralByItemNum(INT i_nItemNum, BYTE i_enumItemStorage)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	if (i_enumItemStorage == ITEM_IN_CHARACTER)
	{
		mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
		while (itr != m_mapItemGeneral.end())
		{
			if (itr->second->ItemNum == (int)i_nItemNum)
			{
				return itr->second;
			}
			itr++;
		}
	}
	else if (i_enumItemStorage == ITEM_IN_STORE)
// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::GetFirstItemGeneralByItemNum#, 
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
//			&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::GetFirstItemGeneralByItemNum, 
		if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
			|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetFirstItemGeneralByItemNum# State error !! %s ItemNum(%d) pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), i_nItemNum, m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
			return NULL;
		}
		
		mtmapUID2ItemGeneral::iterator itr = m_mapItemInStore.begin();
		while (itr != m_mapItemInStore.end())
		{
			if (itr->second->ItemNum == (int)i_nItemNum)
			{
				return itr->second;
			}
			itr++;
		}
	}

	return NULL;
}

ITEM_GENERAL* CFieldItemManager::PopItemGeneralByUID(UID64_t i_nItemUID)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);	
	return m_mapItemGeneral.popLock(i_nItemUID);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		같은 ItemNum중에 이미 사용중인 시간형 아이템이 있는지 체크
/// \author		cmkwon
/// \date		2005-03-10 ~ 2005-03-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsExistAlreadyUsingItemByDestParameter1(DestParam_t i_DestParameter)		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
{
	if(DES_NULL == i_DestParameter)
	{// 2009-09-25 by cmkwon, 체크 추가
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
		//if(m_vectUsingGeneralItem[i]->ItemInfo->DestParameter1 == i_DestParameter)
		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 아래와 같이 8개를 모두 체크하게 수정
		//if(m_vectUsingGeneralItem[i]->ItemInfo->ArrDestParameter[0] == i_DestParameter)
		if(m_vectUsingGeneralItem[i]->ItemInfo->IsExistDesParam(i_DestParameter))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::IsExistAlreadyUsingItemByItemNum(int i_nItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsExistAlreadyUsingItemByItemNum(int i_nItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemNum == i_nItemNum)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::DeleteUsingTimeLimitedItemList(int i_nItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-21 ~ 2006-04-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteUsingTimeLimitedItemList(int i_nItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemNum == i_nItemNum)
		{
			///////////////////////////////////////////////////////////////////////////////
			// 1. 아이템 기능 해제
			m_pFieldIOCPSocket->ReleaseCardItem(m_vectUsingGeneralItem[i]->ItemInfo);

			///////////////////////////////////////////////////////////////////////////////
			// 2. 클라이언트로 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_USINGITEM, T_FC_STORE_DELETE_USINGITEM, pSDUsingItem, SendBuf);
			pSDUsingItem->ItemNum			= m_vectUsingGeneralItem[i]->ItemNum;
			pSDUsingItem->ItemDeletionType	= IUT_EXPIRE_CARD_ITEM;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_DELETE_USINGITEM));

			///////////////////////////////////////////////////////////////////////////////
			// 3. DB에서 아이템 삭제			
			QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
			pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParam->ItemUniqueNumber		= m_vectUsingGeneralItem[i]->UniqueNumber;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] DeleteUsingTimeLimitedItemList ItemUniqueNumber = %d\r\n"
//				, m_vectUsingGeneralItem[i]->UniqueNumber);

			util::del(m_vectUsingGeneralItem[i]);

			m_vectUsingGeneralItem.erase(m_vectUsingGeneralItem.begin() + i);

			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItem(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-24 ~ 2006-04-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ApplyParamFactorWithAllUsingTimeLimitedOnlyCardItem(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		ITEM_GENERAL *pUsingItemG = m_vectUsingGeneralItem[i];
// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
// 		if(ITEMKIND_ACCESSORY_TIMELIMIT != pUsingItemG->ItemInfo->Kind)
// 		{// 2006-10-11 by cmkwon, ITEMKIND_ACCESSORY_TIMELIMIT는 장착 아이템이므로 따로 처리된다
// // 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter1, pUsingItemG->ItemInfo->ParameterValue1);
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter2, pUsingItemG->ItemInfo->ParameterValue2);
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter3, pUsingItemG->ItemInfo->ParameterValue3);
// // 			m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->DestParameter4, pUsingItemG->ItemInfo->ParameterValue4);
// 			for(int j=0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
// 			{
// 				m_pFieldIOCPSocket->SetParamFactor(pUsingItemG->ItemInfo->ArrDestParameter[j], pUsingItemG->ItemInfo->ArrParameterValue[j]);
// 			}
// 		}
		///////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템, 장착가능 아이템이 아닌것만 기능 적용
		if(FALSE == IS_ATTACHABLE_ITEM(pUsingItemG->ItemInfo))
		{
			// 2013-07-22 by bckim, 스텔스 카드 버그 수정  
			if ( FALSE == pUsingItemG->ItemInfo->IsExistDesParam(DES_CASH_STEALTH) )//&& pUsingItemG->ItemInfo->Kind == ITEMKIND_CARD)// 2013-08-20 by bckim, 메모리핵 관련 수정 
			{
				m_pFieldIOCPSocket->ApplyItemDesParam(pUsingItemG->ItemInfo);
			}
			//m_pFieldIOCPSocket->ApplyItemDesParam(pUsingItemG->ItemInfo);
			// End. 2013-07-22 by bckim, 스텔스 카드 버그 수정 
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM_GENERAL *CFieldItemManager::FindUsingItemByItemNumNoLock(int i_nItemNum)
/// \brief		// 2007-06-15 by cmkwon, 사용 시간 제한 아이템 시스템 수정
/// \author		cmkwon
/// \date		2007-06-15 ~ 2007-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL *CFieldItemManager::FindUsingItemByItemNumNoLock(int i_nItemNum)
{
	mt_auto_lock igLock(&m_mapItemGeneral);		// 2012-11-21 by hskim, 캐나다 서버 죽는 문제 보안

	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemNum == i_nItemNum)
		{
			return m_vectUsingGeneralItem[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, 몬스터변신관련 해제 카드 구현 - 2010-04-16 by cmkwon, 빠진사항 추가 수정
/// \author		cmkwon
/// \date		2010-04-16 ~ 2010-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////			
ITEM_GENERAL *CFieldItemManager::FindUsingItemByDestParameterNoLock(DestParam_t i_DestParameter)		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
{
	mt_auto_lock igLock(&m_mapItemGeneral);		// 2012-11-21 by hskim, 캐나다 서버 죽는 문제 보안

	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(m_vectUsingGeneralItem[i]->ItemInfo->IsExistDesParam(i_DestParameter))
		{
			return m_vectUsingGeneralItem[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, 몬스터변신관련 해제 카드 구현 - 2010-04-16 by cmkwon, 빠진사항 추가 수정
/// \author		cmkwon
/// \date		2010-04-16 ~ 2010-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteUsingTimeLimitedItemListByDestParameter(DestParam_t i_DestParameter)		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	ITEM_GENERAL *pItemG = this->FindUsingItemByDestParameterNoLock(i_DestParameter);
	if(NULL == pItemG)
	{
		return FALSE;
	}

	return this->DeleteUsingTimeLimitedItemList(pItemG->ItemNum);
}


////////////////////////////////////////////////////////////////////////////
// 2013-02-28 by bckim, 복귀유져 버프추가
//
////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::SetReturnItemDesParamInParty(CFieldIOCPSocket *i_pUseItemFISoc, ITEM* i_pItem)
{

	if( NULL == i_pUseItemFISoc || NULL == i_pItem )
	{	return FALSE;	}

	i_pUseItemFISoc->m_character.bUsingReturnItem = RETURN_USER_USING_ITEM0;		// RETURN_USER_USING_ITEM0 = 1
	i_pUseItemFISoc->SendCharacterOtherInfo( i_pUseItemFISoc->m_character.ClientIndex, TRUE );
	i_pUseItemFISoc->ApplyItemDesParam(i_pItem);
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-01-18 ~ 2010-01-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM *CFieldItemManager::CheckItemParamOverlapOfUsingItem(ITEM *i_pToUseItemInfo, INT i_nExcludeItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		ITEM_GENERAL *pUsingItemG = m_vectUsingGeneralItem[i];
		if(pUsingItemG->ItemNum == i_nExcludeItemNum)
		{// 2010-01-18 by cmkwon, 체크 제외 아이템
			continue;
		}

		if(FALSE == i_pToUseItemInfo->CheckParamOverlap(pUsingItemG->ItemInfo))
		{
			return pUsingItemG->ItemInfo;
		}
	}

	return NULL;
}
  

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::InsertInsertingItemNumList2DB(int i_nItemNum)
/// \brief		// 2008-01-31 by cmkwon, 시간제한 아이템 DB 에 추가 중 체크 - CFieldItemManager::InsertInsertingItemNumList2DB() 추가
/// \author		cmkwon
/// \date		2008-01-31 ~ 2008-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InsertInsertingItemNumList2DB(int i_nItemNum)
{
	vectINT::iterator itr = find_if(m_vectInsertingItemNumList2DB.begin(), m_vectInsertingItemNumList2DB.end(), Sfind_if_INT(i_nItemNum));
	if(itr != m_vectInsertingItemNumList2DB.end())
	{
		return;
	}

	m_vectInsertingItemNumList2DB.push_back(i_nItemNum);
}


/****************************************************************************************************************************************************
**
**	DB 등록 중인 아이템목록 체크.
**
**	Create Info :	2008-01-31 ~ 2008-01-31	cmkwon.
**
**
**	Update Info :	- // 2008-01-31 by cmkwon, 시간제한 아이템 DB 에 추가 중 체크 - CFieldItemManager::IsExistInsertingItemNumList2DB() 추가
**
**					- 아이템 목록에서 효과(Ex:DESPARAM)겹치는 아이템들 추가로 체크.	2010. 09. 17. by hsLee.
**
*****************************************************************************************************************************************************/
BOOL CFieldItemManager::IsExistInsertingItemNumList2DB(int i_nItemNum)
{

	// 2010. 09. 17. by hsLee.
	ITEM *pcUsingItem = ms_pFieldIOCP2->GetItemInfo ( i_nItemNum );
	
	if ( NULL == pcUsingItem )
		return FALSE;
	
	for(int i=0; i < m_vectInsertingItemNumList2DB.size(); i++)
	{
		ITEM *pcItem = ms_pFieldIOCP2->GetItemInfo( m_vectInsertingItemNumList2DB[i] ); 
		
		if ( NULL == pcItem || pcItem->ItemNum == i_nItemNum )
			continue;
		
		if( FALSE == pcItem->CheckParamOverlap( pcUsingItem ) )
		{
			return TRUE;
		}
	}
	// End 2010. 09. 17. by hsLee.
	
	vectINT::iterator itr = find_if(m_vectInsertingItemNumList2DB.begin(), m_vectInsertingItemNumList2DB.end(), Sfind_if_INT(i_nItemNum));
	if(itr == m_vectInsertingItemNumList2DB.end())
	{
		return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::DeleteInsertingItemNumList2DB(int i_nItemNum)
/// \brief		// 2008-01-31 by cmkwon, 시간제한 아이템 DB 에 추가 중 체크 - CFieldItemManager::DeleteInsertingItemNumList2DB() 추가
/// \author		cmkwon
/// \date		2008-01-31 ~ 2008-01-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::DeleteInsertingItemNumList2DB(int i_nItemNum)
{
	vectINT::iterator itr = find_if(m_vectInsertingItemNumList2DB.begin(), m_vectInsertingItemNumList2DB.end(), Sfind_if_INT(i_nItemNum));
	if(itr == m_vectInsertingItemNumList2DB.end())
	{
		return;
	}

	itr = m_vectInsertingItemNumList2DB.erase(itr);
}

ITEM_BASE* CFieldItemManager::MakeNewItem(INT ItemNum,
										BYTE i_enumStorage,
										ITEM *pItemInfo, UID64_t ItemUniqueNumber)
{
	ITEM_BASE *pRetItem = NULL;

	if (pItemInfo == NULL)
	{
		pItemInfo = ms_pFieldIOCP2->GetItemInfo(ItemNum);
		if (pItemInfo == NULL)
		{
			DBGOUT("Error! No such item(%d) at MakeNewItem()\n", ItemNum);
			return NULL;
		}
	}

	if (IS_GENERAL_ITEM(pItemInfo->Kind))
	{
		ITEM_GENERAL *pItem			= new ITEM_GENERAL(pItemInfo);
		pItem->UniqueNumber			= ItemUniqueNumber;
		pItem->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
		pItem->Possess				= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		pItem->ItemStorage			= i_enumStorage;
		pItem->Wear					= WEAR_NOT_ATTACHED;
		pItem->CurrentCount			= (IS_CHARGABLE_ITEM(pItemInfo->Kind)?pItemInfo->Charging:1);
// 2009-08-25 by cmkwon, 사용하지 않는 필드 제거(td_Store.ScarcityNumber) - 
//		pItem->ScarcityNumber		= 0;
		pItem->ItemWindowIndex		= POS_INVALID_POSITION;
		pItem->NumOfEnchants		= 0;
		
		pRetItem					= pItem;
	}
	else if (IS_SKILL_ITEM(pItemInfo->Kind))
	{
		ITEM_SKILL *pItem			= new ITEM_SKILL(pItemInfo);
		pItem->UniqueNumber			= ItemUniqueNumber;
		pItem->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
		pItem->Possess				= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		pItem->ItemWindowIndex		= POS_INVALID_POSITION;

		pRetItem					= pItem;
	}
	else
	{
		// check: error
		DBGOUT("Error! No such item Type(%s, %d) --> type: %d, at MakeNewItem()\n",
			pItemInfo->ItemName, pItemInfo->ItemNum, pItemInfo->Kind
		);
		return NULL;
	}

	return pRetItem;
}

// 2010-06-15 by shcho&hslee 펫시스템 - 
/********************************************************************************************
**
**	펫 데이터 생성.
**
**	Create Info :	2010-06-15 by shcho&hslee
**
*********************************************************************************************/
tPET_CURRENTINFO* CFieldItemManager :: MakeNewPetData ( UID64_t i_nPetUID , const tPET_BASEDATA *a_pPetBaseData , const tPET_LEVEL_DATA *a_pPetLevelData /*= NULL*/ )
{

	tPET_CURRENTINFO *RetPetData = NULL;

	if ( NULL == a_pPetBaseData )
		return NULL;

	tPET_CURRENTINFO *pInputPetData = new tPET_CURRENTINFO(NULL);

	if ( pInputPetData )
	{

// 		pInputPetData->CreatedPetUID = i_nPetUID;
// 
// 		pInputPetData->EnableEditPetName = true;
// 		strcpy ( pInputPetData->szPetName , a_pPetBaseData->szPetName );
// 
// 		pInputPetData->PetIndex = a_pPetBaseData->PetIndex;
// 		pInputPetData->PetLevel = a_pPetBaseData->PetLevel;
// 
// 		pInputPetData->PetExp = a_pPetLevelData != NULL ? a_pPetLevelData->NeedExp : 0;
// 
// 		pInputPetData->Stamina = a_pPetBaseData->Stamina;

// 		ITEM *psItem = NULL;
// 
// 		for ( int i = 0; i < SIZE_MAX_PETSKILLITEM; ++i )
// 		{
// 
// 			psItem = ms_pFieldIOCP2->GetItemInfo( a_pPetBaseData->PetSkillItemIndex[i] );
// 
// 			if ( NULL == psItem )
// 				continue;
// 
// 		}

		RetPetData = pInputPetData;

	}
	

	return RetPetData;

}


BOOL CFieldItemManager::InsertItemGeneralByPointer(ITEM_GENERAL *i_pItemGeneral
												   , BYTE i_nItemUpdateType
												   , BOOL i_bCheckGeneralItemCounts/*=FALSE*/
												   , BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/
												   , BOOL i_bUpdateDB/*=TRUE*/
												   , BOOL i_bSendMSG/*=TRUE*/
												   , UID32_t i_tradeCharacterUID/*=INVALID_UNIQUE_NUMBER*/		// 2006-05-15 by cmkwon
												   , BOOL i_bCheckGamePlayTime/*=TRUE*/							// 2008-03-28 by cmkwon, PlayTime 관련 제한 버그 수정 - 
												   , int  i_nGLogItemtype/*=0*/									// 2012-11-21 by khkim, GLog 2차
												   )
{
// 2008-04-08 by cmkwon, 게임 제한 시스템 수정 - 아래의 코드는 필요 없음, 드랍율로 처리하고 습득율은 항상 100%
// 	// 2008-03-28 by cmkwon, PlayTime 관련 제한 버그 수정 - 여기에서도 필요시 체크 해줘야 함
// 	if( i_bCheckGamePlayTime
// 		&& (IUT_DROP_ITEM == i_nItemUpdateType || IUT_QUEST == i_nItemUpdateType) )
// 	{
// 		float	fApplyRate		= 0.0f;
// 		BOOL	bRestriction	= m_pFieldIOCPSocket->GPGetApplyRateByGamePlayingTime(&fApplyRate);
// 		if(bRestriction)
// 		{
// 			if(0.0f >= fApplyRate)
// 			{
// 				// 2008-03-28 by cmkwon, 아이템이 사라진다.
// 				util::del(i_pItemGeneral);
// 				return TRUE;
// 			}
// 
// 			if(IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
// 			{
// 				i_pItemGeneral->CurrentCount	= (i_pItemGeneral->CurrentCount+1)*fApplyRate;
// 				if(0 >= i_pItemGeneral->CurrentCount)
// 				{					
// 					// 2008-03-28 by cmkwon, 아이템이 사라진다.
// 					util::del(i_pItemGeneral);
// 					return TRUE;
// 				}
// 			}
// 		}
//	}

	// lock m_mapItemGeneral, @InsertItemGeneralByPointer
	mt_auto_lock igLock(&m_mapItemGeneral);

// 2006-11-17 by cmkwon, 시스템 변경으로 필요 없음
//	if(FALSE == m_pFieldIOCPSocket->IsValidGameUser()
//		&& (IUT_DROP_ITEM == i_nItemUpdateType || IUT_QUEST == i_nItemUpdateType))
//	{// 2006-06-14 by cmkwon, 베트남 정액제 체크 - 드랍 아이템 추가
//		return FALSE;
//	}

	if (!IS_GENERAL_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{
		// ITEM_GENERAL이 아니면 삽입할 수 없다.
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind)
		&& i_pItemGeneral->CurrentCount < 0)
	{
		// 개수가 0보다 작으면 error임
		return FALSE;
	}

//#ifdef _DEBUG
//	if (IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
//	{
//		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD,
//				"아이템 \'%s\'이(가) %d개 %s추가되었습니다",
//				i_pItemGeneral->ItemInfo->ItemName, i_pItemGeneral->CurrentCount,
//				((i_enumStorage==ITEM_IN_STORE)?"창고에 ":""));
//	}
//	else
//	{
//		m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, "아이템 \'%s\'이(가) 1개 %s추가되었습니다",
//				i_pItemGeneral->ItemInfo->ItemName,
//				((i_enumStorage==ITEM_IN_STORE)?"창고에 ":""));
//	}
//#endif

	// start 2012-11-21 by khkim, GLog 2차
	if(!COMPARE_RACE(this->m_pCharacter->Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (일반유저만 기록)	
	{
		if( NULL != m_pFieldIOCPSocket )
		{
			switch((FALSE == i_nGLogItemtype)? i_nItemUpdateType : i_nGLogItemtype)
			{
			case IUT_DROP_ITEM:
				{
					SetGLogData(GLOG_IL_LT_DROP,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_QUEST:
				{
					SetGLogData(GLOG_IL_LT_QUEST,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_SHOP:
				{
					SetGLogData(GLOG_IL_LT_SHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case GLOG_WARPOINTSHOP:
				{
					SetGLogData(GLOG_IL_LT_WARPOINTSHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case GLOG_INFINITYOBSHOP:
				{
					SetGLogData(GLOG_IL_LT_INFINITYOBSHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case GLOG_CASHSHOP:
				{
					SetGLogData(GLOG_IL_LT_CASHSHOP_BUY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_LUCKY_ITEM:
				{
					SetGLogData(GLOG_IL_LT_LUCKYMACHINE_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_SELL:
				{
					SetGLogData(GLOG_IL_LT_BAZAARSELL_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_BUY:
				{
					SetGLogData(GLOG_IL_LT_BAZAARBUY_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_TRADE:
				{
					SetGLogData(GLOG_IL_LT_TREADGET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_MIXING:
				{
					SetGLogData(GLOG_IL_LT_MIXING_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_ENCHANT:
				{
					SetGLogData(GLOG_IL_LT_ENCHANT_UP,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_DISSOLUTION_ITEM:
				{
					SetGLogData(GLOG_IL_LT_DISSOLUTION_GETITEM,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_RANDOMBOX:
 				{
					SetGLogData(GLOG_IL_LT_RANDOMBOX,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
 				}
				// 2013-05-22 by jhseol, 소스정리 - 주석삭제
			default:
				{
					break;
				}
			}
		}
	}
	// end 2012-11-21 by khkim, GLog 2차


	///////////////////////////////////////////////////////////////////////////
	// countable item이고 이미 존재하는 경우는 update count
	///////////////////////////////////////////////////////////////////////////
	if (IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind)
		&& (// 2006-09-29 by cmkwon, ITEM_ATTR_TIME_LIMITE만 체크하도록 수정 ITEMKIND_CARD != i_pItemGeneral->Kind
			FALSE == COMPARE_BIT_FLAG(i_pItemGeneral->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)	// 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 
			|| ITEM_NOT_USING == i_pItemGeneral->UsingTimeStamp)
		)
	{
		// Countable Item 인 경우
		ITEM_GENERAL *pItemCountableFound = GetFirstItemGeneralByItemNum(i_pItemGeneral->ItemNum, i_enumStorage);

		if (pItemCountableFound != NULL)
		{
			// 이미 존재하면
			UpdateItemCountByPointer(pItemCountableFound, i_pItemGeneral->CurrentCount, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG);

// 2008-01-30 by cmkwon, 게임로그 두번 저장되는 버그 수정 - 여기는 주석 처리하고 UpdateItemCountByPointer() 함수안에서 처리한다.
// 			///////////////////////////////////////////////////////////////////////////////
// 			// 2007-01-11 by cmkwon, 카운터블 아이템은 여기에서 로그를 남겨야 한다.
// 			switch(i_nItemUpdateType)
// 			{
// 			case IUT_DROP_ITEM:
// 			case IUT_QUEST:			// 2007-10-30 by cmkwon, 퀘스트 아이템 습득 게임 로그에 추가
// 				CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, pItemCountableFound, i_pItemGeneral->CurrentCount);
// 				break;
// 			case IUT_GIVEEVENTITEM:		// 2006-08-28 by dhjin
// 			case IUT_BONUS_ITEM:		// 2007-09-11 by cmkwon, 명령어 이벤트 아이템 지급 게임로그에 추가하기
// 			case IUT_GIVEEVENTITEM_COUPONEVENT:		// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
// 				CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(m_pFieldIOCPSocket, pItemCountableFound, i_pItemGeneral->CurrentCount);
// 				break;
// 			}

			// check: 일단 지우는 걸로 구현했지만 이게 과연 좋은 판단일까? 20031029, kelovon
			util::del(i_pItemGeneral);
			return TRUE;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// 그 이외의 경우에는 삽입 - UniqueNumber를 받아야 하기 때문에 QP_InsertStoreItem() 이후에 처리
	///////////////////////////////////////////////////////////////////////////
	// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
	if(i_bCheckGeneralItemCounts
		&& this->GetInventoryItemCounts() >= this->GetMaxInventoryCount())
	{// 인벤토리가 다 참

		// 2007-01-04 by cmkwon, 시스템로그에 남지 않도록 수정
		m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_INVENTORY_FULL_ITEM, this->GetMaxInventoryCount(), this->GetInventoryItemCounts(), NULL, FALSE, FALSE);
		return FALSE;
	}

	if (i_bUpdateDB)
	{
		m_pFieldIOCPSocket->IncrementInsertingStoreCounts();
		
		if (ITEM_IN_STORE == i_enumStorage)
		{// 창고 사용 중이라면

			if(FALSE == IS_VALID_UNIQUE_NUMBER(m_uid32StoreCharacterUID))
			{// 2006-09-20 by dhjin, 체크
				i_pItemGeneral->Possess		= m_pCharacter->CharacterUniqueNumber;
			}
			else
			{
				i_pItemGeneral->Possess		= m_uid32StoreCharacterUID;				// 2006-09-20 by dhjin, 케릭터간 창고 공유 -> 선택된 케릭터 UID
			}
		}

		m_pFieldDBManager->MakeAndEnqueueQuery(QT_InsertStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, i_pItemGeneral, NULL, i_nItemUpdateType, i_tradeCharacterUID);
	}
	else
	{// 2006-05-11 by cmkwon, DB에 업데이트 하지 않으면 바로 m_mapItemGeneral에 추가한다.
		i_pItemGeneral->Possess		= m_pCharacter->CharacterUniqueNumber;		// 2006-09-20 by dhjin, 케릭터간 창고 공유 -> 선택된 케릭터 UID
		m_mapItemGeneral.insertLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
		// 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
		if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::InsertItemGeneralByPointer - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
				, i_pItemGeneral->Possess, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
		}
#endif
		// end 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그
		this->ItemInsertByInfinityPlaying(i_pItemGeneral);			// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 진행 중 아이템 습득
		m_pFieldIOCPSocket->Check8SendAFSUpdateItem(i_pItemGeneral);			// 2010-03-31 by dhjin, 인피니티 입장 캐쉬 아이템 구현 - 
		if(i_bSendMSG)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_ITEM, T_FC_STORE_INSERT_ITEM, pInsertItem, SendBuf);
			pInsertItem->FromCharacterUniqueNumber	= 0;
			pInsertItem->ItemInsertionType			= i_nItemUpdateType;
			pInsertItem->ItemGeneral				= *i_pItemGeneral;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_INSERT_ITEM));
		}
	}

	return TRUE;
}

// 2010-06-15 by shcho&hslee 펫시스템 - 소유 팻 정보 저장
BOOL CFieldItemManager::InsertItemPetByPointer( tPET_CURRENTINFO *i_pItemPet )
{

	if ( NULL == i_pItemPet ) 
		return FALSE;

	mt_auto_lock mtAuto(&m_vectPetCurrentInfo);
	if(this->GetItemPetByUID(i_pItemPet->CreatedPetUID))
	{
		return FALSE;
	}
	m_vectPetCurrentInfo.push_back(i_pItemPet);
	return TRUE;

}
// 스킬로딩시, 스킬구입시 사용됨
BOOL CFieldItemManager::InsertItemSkillByPointer(UID64_t i_SkillItemUID, ITEM_SKILL *i_pItemSkill)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	if(this->GetItemSkillByUID(i_pItemSkill->UniqueNumber))
	{
		return FALSE;
	}
	m_vectItemSkillPtr.push_back(i_pItemSkill);
	return TRUE;
}

BOOL CFieldItemManager::InsertStoreItemByPointer(UID64_t i_n64ItemUID, ITEM_GENERAL *i_pItemG)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	if(i_pItemG->Possess != m_uid32StoreCharacterUID)
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::InsertStoreItemByPointer#,
	if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
		|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::InsertStoreItemByPointer State error !! %s %s pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemG, string()), m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
		return FALSE;
	}

	m_mapItemInStore.insertNoLock(i_n64ItemUID, i_pItemG);
	return TRUE;
}

// 2007-07-25 by cmkwon, 이벤트 아이템의 접두/접미 레어 설정 - CFieldItemManager::InsertItemBaseByItemNum()함수 인자 추가(, int i_nPrefixCodeNum/*=0*/, int i_nSuffixCodeNum/*=0*/)
// 2010-02-11 by cmkwon, 조합시 외형,이펙트 유지 시스템 구현 - 인자추가(, INT i_nShapeItemNum=0, INT i_nEffectItemNum=0)
BOOL CFieldItemManager::InsertItemBaseByItemNum(INT i_nItemNum, INT i_nItemCount
												, BYTE i_nItemUpdateType
												, BOOL i_bCheckGeneralItemCounts/*=FALSE*/
												, BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/
												, BOOL i_bUpdateDB/*=TRUE*/
												, BOOL i_bSendMSG/*=TRUE*/
												, INT i_nPrefixCodeNum/*=0*/
												, INT i_nSuffixCodeNum/*=0*/
												, INT i_nShapeItemNum/*=0*/
												, INT i_nEffectItemNum/*=0*/
												, int i_nGLogItemtype/*=0*/)			//  2012-11-21 by khkim, GLog 2차
{
	ITEM *pItemInfo = ms_pFieldIOCP2->GetItemInfo(i_nItemNum);
	if (pItemInfo == NULL)
	{
		DBGOUT("FATAL ERROR@CFieldItemManager::InsertItemBaseByItemNum(): T_FIELD_IOCP, ERR_PROTOCOL_NO_SUCH_ITEM, ItemNum, 0\r\n");
		return FALSE;
	}

	if (IS_GENERAL_ITEM(pItemInfo->Kind))
	{
		if (IS_COUNTABLE_ITEM(pItemInfo->Kind))
		{
			ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)MakeNewItem(pItemInfo->ItemNum);
			if (pItemGeneral == NULL) { return FALSE; }
			pItemGeneral->ItemStorage	= i_enumStorage;
			pItemGeneral->CurrentCount	= i_nItemCount;

			// 2008-03-28 by cmkwon, PlayTime 관련 제한 버그 수정 - 아래와 같이 인자 수정함, PlayTime 관련 제한 체크 하지 않아야 함, 요 위에서 이미 처리가 되어 있음
			//BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG);
			BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG, INVALID_UNIQUE_NUMBER, FALSE, i_nGLogItemtype);	//  2012-11-21 by khkim, GLog 2차
			if (!bRet)
			{
				util::del(pItemGeneral);		// 2006-06-14 by cmkwon
				return FALSE;
			}
		}
		else
		{
			for (int i = 0; i < i_nItemCount; i++)
			{
				ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)MakeNewItem(pItemInfo->ItemNum);
				if (pItemGeneral == NULL) { return FALSE; }
				pItemGeneral->ItemStorage	= i_enumStorage;

				//////////////////////////////////////////////////////////////////////////
				// 2007-07-25 by cmkwon, 이벤트 아이템의 접두/접미 레어 설정 - 인자로 들어온 레어 설정하기
				if(0 != i_nPrefixCodeNum)
				{
					if(ms_pFieldIOCP2->m_mapRareItemInfo.findEZ_ptr(i_nPrefixCodeNum))					
					{
						pItemGeneral->PrefixCodeNum = i_nPrefixCodeNum;
					}
				}
				if(0 != i_nSuffixCodeNum)
				{
					if(ms_pFieldIOCP2->m_mapRareItemInfo.findEZ_ptr(i_nSuffixCodeNum))
					{
						pItemGeneral->SuffixCodeNum = i_nSuffixCodeNum;
					}
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2010-02-11 by cmkwon, 조합시 외형,이펙트 유지 시스템 구현 - 
				if(0 != i_nShapeItemNum)
				{
					if(ms_pFieldIOCP2->GetItemInfo(i_nShapeItemNum))
					{
						pItemGeneral->ShapeItemNum	= i_nShapeItemNum;
					}
				}
				if(0 != i_nEffectItemNum)
				{
					if(ms_pFieldIOCP2->GetItemInfo(i_nEffectItemNum))
					{
						pItemGeneral->ColorCode		= i_nEffectItemNum;
					}
				}

				// 2008-03-28 by cmkwon, PlayTime 관련 제한 버그 수정 - 아래와 같이 인자 수정함, PlayTime 관련 제한 체크 하지 않아야 함, 요 위에서 이미 처리가 되어 있음
				//BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG);
				BOOL bRet = InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, i_bCheckGeneralItemCounts, i_enumStorage, i_bUpdateDB, i_bSendMSG, INVALID_UNIQUE_NUMBER, FALSE, i_nGLogItemtype); //  2012-11-21 by khkim, GLog 2차
				if (!bRet)
				{
					util::del(pItemGeneral);		// 2006-06-14 by cmkwon
					return FALSE;
				}
			}
		}
	}
	else if (IS_SKILL_ITEM(pItemInfo->Kind))
	{
		// 기존 스킬이 있고 레벨이 높으면 취소
		// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		ITEM_SKILL *pItemSkillOld = GetFirstSkillBySkillBaseNum(pItemInfo->NextSkill);
		if (pItemSkillOld != NULL && pItemSkillOld->ItemInfo->SkillLevel > pItemInfo->SkillLevel)
		{
			return TRUE;
		}
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		ITEM_SKILL *pItemSkillOld = GetFirstSkillBySkillBaseNum(pItemInfo->ItemNum);
		if (pItemSkillOld != NULL && SKILL_LEVEL(pItemSkillOld->ItemNum) > SKILL_LEVEL(pItemInfo->ItemNum))
		{
			return TRUE;
		}
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정

		// 스킬 추가
		ITEM_SKILL *pItemSkill = (ITEM_SKILL*)MakeNewItem(pItemInfo->ItemNum, ITEM_IN_CHARACTER, pItemInfo);
		if (pItemSkill == NULL || (pItemSkill->ItemInfo->ReqUnitKind&m_pFieldIOCPSocket->m_character.UnitKind) == 0)
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_REQ_UNITKIND_NOT_MATCHED);
			return FALSE;
		}
		m_pFieldDBManager->MakeAndEnqueueQuery(QT_InsertStoreItemSkill, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pItemSkill, NULL, i_nItemUpdateType);
	}

	return TRUE;
}

BOOL CFieldItemManager::InsertItemGeneralByItemNum(INT i_nItemNum, INT i_nItemCount,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG
							, UID32_t i_tradeCharacterUID/*=INVALID_UNIQUE_NUMBER*/		// 2006-05-15 by cmkwon
							)
{
	ITEM_GENERAL *pItemGeneral = (ITEM_GENERAL*)MakeNewItem(i_nItemNum, i_enumStorage);

	if (pItemGeneral == NULL || !IS_GENERAL_ITEM(pItemGeneral->Kind))
	{
		util::del(pItemGeneral);
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
	{
		pItemGeneral->CurrentCount = i_nItemCount;
	}
	else
	{
		ASSERT_NOT_IMPLEMENTED_YET();
		util::del(pItemGeneral);
		return FALSE;
	}

	if(FALSE == InsertItemGeneralByPointer(pItemGeneral, i_nItemUpdateType, FALSE, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_tradeCharacterUID))
	{
		util::del(pItemGeneral);
		return FALSE;
	}

	return TRUE;
}

BOOL CFieldItemManager::UpdateItemCountByPointer(ITEM_GENERAL *i_pItemGeneral,
							INT i_nChangeCount,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int i_nGLogItemtype/*=0*/ )			//  2012-11-21 by khkim, GLog 2차
	{
	if(NULL == i_pItemGeneral) {
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점, NULL 값 체크 추가
		return FALSE;
	}
	///////////////////////////////////////////////////////////////////////////
	// 무기류 및 Countable Item에 대해서만 적용됨, nCount만큼 CurrentCount를 변화시킴
	// nCount < 0이면 아이템 개수를 줄이고, nCount > 0이면 개수를 늘인다
	// 최종 개수 0보다 작으면 아이템을 삭제한다
	///////////////////////////////////////////////////////////////////////////
	if(0 == i_pItemGeneral->UniqueNumber)
	{
		char szSysLog[1024];
		sprintf(szSysLog, "[ERROR] CFieldItemManager::UpdateItemCountByPointer_ ItemUID Error, %s %s ChangeCount(%d) ItemUpdateType(%d)\r\n"
			, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string())
			, i_nChangeCount, i_nItemUpdateType);
		g_pFieldGlobal->WriteSystemLog(szSysLog);
		DbgOut(szSysLog);
	}

	if (IS_CHARGABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{
		// do nothing
	}
	else if (!IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind))
	{
		return FALSE;
	}
	else
	{
		// countable item인 경우
		if (i_pItemGeneral->CurrentCount <= 0 && i_pItemGeneral->ItemNum != MONEY_ITEM_NUMBER)
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_INVALID_ITEM_COUNT, i_pItemGeneral->UniqueNumber, i_pItemGeneral->CurrentCount, i_pItemGeneral->ItemInfo->ItemName);
			m_pFieldIOCPSocket->DBGOUT2(DO2_ALL, "FATAL ERROR: Item Count 이상! %s %s\r\n"
					, GetCharacterString(&m_pFieldIOCPSocket->m_character, string())
					, GetItemGeneralString(i_pItemGeneral, string())
			);
			return FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////
		// 2005-09-13 by cmkwon, 카운터블 아이템 최대 카운트 체크
		INT64 tmCurrentCount = (INT64)i_pItemGeneral->CurrentCount + (INT64)i_nChangeCount;
		if(tmCurrentCount > MAX_NOTIFY_ITEM_COUNTS)
		{
			m_pFieldIOCPSocket->SendErrorMessage(T_FIELD_IOCP, ERR_PROTOCOL_NOTIFY_MAX_ITEM_COUNTS_OVER, 0, 0);
		}
	}

	// start 2012-01-16 by hskim, 통계 - 화패
	if( NULL != ms_pFieldIOCP2->m_pStatisticsManager )
	{
		if( MONEY_ITEM_NUMBER == i_pItemGeneral->ItemNum )
		{
			if( i_nChangeCount < 0 )
			{
				ms_pFieldIOCP2->m_pStatisticsManager->Sub(STATISTICS_SPI, i_nChangeCount);
			}
			else if( i_nChangeCount > 0 )
			{
				ms_pFieldIOCP2->m_pStatisticsManager->Add(STATISTICS_SPI, i_nChangeCount);
			}
		}
	}
	// end 2012-01-16 by hskim, 통계 - 화패

	// Item Count 수정하기
	INT64 n64Counts = i_pItemGeneral->CurrentCount;
	n64Counts = min((INT64)n64Counts + (INT64)i_nChangeCount, MAX_ITEM_COUNTS);
	if (n64Counts <= 0
		&& IS_COUNTABLE_ITEM(i_pItemGeneral->ItemInfo->Kind)
		&& i_pItemGeneral->ItemNum != MONEY_ITEM_NUMBER)
	{
		// 지우기
		if(CFieldItemManager::IsDelayStoreItem(i_pItemGeneral->ItemInfo))
		{
			this->DeleteDelayStoredItemInfoByItemUID(i_pItemGeneral->UniqueNumber);
		}
		DeleteItemFromCharacterByPointer(i_pItemGeneral, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);  //  2012-11-21 by khkim, GLog 2차
	}
	else if(i_pItemGeneral->CurrentCount != n64Counts)
	{
		// 개수 업데이트
		i_pItemGeneral->CurrentCount = max(0, n64Counts);

		// Update DB
		if (i_bUpdateDB)
		{
			if(0 == i_pItemGeneral->CurrentCount		// 2006-05-21 by cmkwon, SPI이 경우
				|| (IUT_USE_ENERGY != i_nItemUpdateType && IUT_DROP_ITEM != i_nItemUpdateType)
				|| FALSE == CFieldItemManager::IsDelayStoreItem(i_pItemGeneral->ItemInfo)	// 2006-05-21 by cmkwon, 카운트 모든변경 지연저장 처리
				)
			{
				////////////////////////////////////////////////////////////////////////////////
				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 밑과 같이 수정, 인피 진행 중 아이템 수정
//				QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
//				pQParam->ItemUniqueNumber		= i_pItemGeneral->UniqueNumber;
//				pQParam->Count					= i_pItemGeneral->CurrentCount;
//				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				if(FALSE == g_pFieldGlobal->IsArenaServer()) {
					QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
					pQParam->ItemUniqueNumber		= i_pItemGeneral->UniqueNumber;
					pQParam->Count					= i_pItemGeneral->CurrentCount;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				}
				else {
					this->ItemUpdateByInfinityPlaying(i_pItemGeneral);
				}
			}
		}

		// start 2012-11-21 by khkim, GLog 2차 
		if(!COMPARE_RACE(this->m_pCharacter->Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (일반유저만 기록)	
		{
			if( NULL != m_pFieldIOCPSocket )
			{
				if( 0 > i_nChangeCount )					
				{ 
					switch((FALSE == i_nGLogItemtype)? i_nItemUpdateType : i_nGLogItemtype)
					{
					case IUT_SHOP:
						{
							SetGLogData(GLOG_IL_LT_SHOP_SELL, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_BAZAAR_SELL:
						{
							SetGLogData( GLOG_IL_LT_BAZAARSELL_GIVE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_BAZAAR_BUY:
						{
							SetGLogData(GLOG_IL_LT_BAZAARBUY_GIVE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_TRADE:
						{
							SetGLogData(GLOG_IL_LT_TREADGIVE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_LUCKY_ITEM:
						{
							SetGLogData(GLOG_IL_LT_LUCKYMACHINE_USE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_GENERAL:
						{
							SetGLogData(GLOG_IL_LT_THROWAWAY, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_USE_ITEM:
						{
							SetGLogData(GLOG_IL_LT_USE_USEITEM, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_ENCHANT:
						{
							SetGLogData(GLOG_IL_LT_ENCHANT_USEITEM, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					case IUT_MIXING:
						{
							SetGLogData(GLOG_IL_LT_MIXING_USE, i_pItemGeneral, m_pFieldIOCPSocket->m_character.Level, i_nChangeCount);
							break;
						}
					default:
						{
							break;
						}
					}
				}
			}
		}
		// end 2012-11-21 by khkim, GLog 2차


		// 2007-10-30 by cmkwon, 퀘스트 아이템 습득 게임 로그에 추가  - 아래와 같이 수정함
		//// 로그 남기기, 20040820, kelovon
		//if (i_nItemUpdateType == IUT_DROP_ITEM)
		//{
		//	CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
		//}
// 2008-01-30 by cmkwon, 게임로그 두번 저장되는 버그 수정 - 아래와 같이 저장하는 로그 i_nItemUpdateType 을 추가함
// 		switch(i_nItemUpdateType)
// 		{
// 		case IUT_DROP_ITEM:
// 		case IUT_QUEST:			// 2007-10-30 by cmkwon, 퀘스트 아이템 습득 게임 로그에 추가
// 			CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
// 			break;
//		}

		// 2008-01-30 by cmkwon, 게임로그 두번 저장되는 버그 수정 - CFieldItemManager::InsertItemGeneralByPointer() 함수에서 저장하던 것은 주석 처리하고 여기에서 처리한다.
		switch(i_nItemUpdateType)
		{
		case IUT_DROP_ITEM:
		case IUT_QUEST:			// 2007-10-30 by cmkwon, 퀘스트 아이템 습득 게임 로그에 추가
		case IUT_MIXING:		// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
		case IUT_DISSOLUTION_ITEM : // 2010-08-31 by shcho&jskim 아이템용해 시스템 -
		case IUT_RANDOMBOX :	// 2013-06-10 by jhseol, GameLog 추가 - 캡슐류 사용 시 획득하는 아이템도 게임로그를 남기도록 추가
			// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
			// 2012-08-30 by jhseol, 풀로그 남기기
#ifdef S_FULL_LOG_JHSEOL
			//CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);	// 2012-08-30 by jhseol, 풀로그 남기기 - 변경 전의 함수 임으로 주석 해제 안함.
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, 풀로그 남기기
			CAtumLogSender::SendLogMessageITEMGetITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount, i_nItemUpdateType);		// 2008-09-26 by cmkwon, 조합시 GameLog 수정 - 
			break;
		case IUT_GIVEEVENTITEM:		// 2006-08-28 by dhjin
		case IUT_BONUS_ITEM:		// 2007-09-11 by cmkwon, 명령어 이벤트 아이템 지급 게임로그에 추가하기
		case IUT_GIVEEVENTITEM_COUPONEVENT:		// 2008-01-10 by cmkwon, 아이템 이벤트 시스템에 신 쿠폰 시스템 추가 - 
			CAtumLogSender::SendLogMessageITEMGetItemByItemEvent(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
			break;
		case IUT_STORE:		// 2008-02-15 by cmkwon, 인벤<->창고 이동 로그 수정 - 개수 변경 로그 전송하기 추가
			{
				// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
				CFieldIOCPSocket tempCharacterSocket;
				tempCharacterSocket.m_character.CharacterUniqueNumber = i_pItemGeneral->Possess;
				tempCharacterSocket.m_character.MapChannelIndex.MapIndex =this->m_pCharacter->MapChannelIndex.MapIndex;
				// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가.End
				
				if(0 < i_nChangeCount)
				{// 2008-02-15 by cmkwon, i_nChangeCount 이 0보다 작다면 창고 로그로 이미 저장되고 있다.
					if(ITEM_IN_CHARACTER == i_enumStorage)
					{
						CAtumLogSender::SendLogMessageITEMCharacterITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
						// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
						if( i_pItemGeneral->Possess != this->m_uid32StoreCharacterUID)
						{
							CAtumLogSender::SendLogMessageITEMGetWithSameAccount(&tempCharacterSocket, i_pItemGeneral, i_nChangeCount, this->m_uid32StoreCharacterUID);
						}
						// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가.End
					}
					else
					{
						CAtumLogSender::SendLogMessageITEMStoreITEM(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
						// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가
						if( i_pItemGeneral->Possess != m_pFieldIOCPSocket->m_character.CharacterUniqueNumber)
						{
							CAtumLogSender::SendLogMessageITEMGetWithSameAccount(&tempCharacterSocket, i_pItemGeneral, i_nChangeCount, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber);											
						}
						// 2012-12-27 by bckim, 동일계정, 각각의 케릭터간의 아이템 이동시 로그추가.End
					}
				}
			}
			break;

			// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그
		case IUT_GUILD_STORE:
			{
				CAtumLogSender::SendLogMessageITEMCharacterToFromGuildstoreResult(m_pFieldIOCPSocket, i_pItemGeneral, i_nChangeCount);
			}
		break;
		// 2012-12-17 by bckim, 길드창고 아이템 이동 관련 추가로그. End
		}

		// MSG 전송
		if (i_bSendMSG)
		{
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEM_COUNT, T_FC_STORE_UPDATE_ITEM_COUNT, msgUpdate, msgUpdateBuf);
			msgUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
			msgUpdate->NewCount			= i_pItemGeneral->CurrentCount;
			msgUpdate->ItemUpdateType	= i_nItemUpdateType;
			m_pFieldIOCPSocket->SendAddData(msgUpdateBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEM_COUNT));
		}
	}

	return TRUE;
}




///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::UpdateItemNumByPointer(ITEM_GENERAL *i_pItemGeneral, ITEM *i_pChangeITEM, BYTE i_nItemUpdateType, BOOL i_bSendMSG/*=TRUE*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-06-14 ~ 2006-06-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateItemNumByPointer(ITEM_GENERAL *i_pItemGeneral, ITEM *i_pChangeITEM, BYTE i_nItemUpdateType, BOOL i_bSendMSG/*=TRUE*/)
{
	if(i_pItemGeneral->ItemInfo->Kind != i_pChangeITEM->Kind)
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-14 by cmkwon, 적용
	i_pItemGeneral->ItemNum			= i_pChangeITEM->ItemNum;
	i_pItemGeneral->ItemInfo		= i_pChangeITEM;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-14 by cmkwon
	QPARAM_UPDATEITEMNUM *pQParam	= new QPARAM_UPDATEITEMNUM;
	pQParam->ItemUniqueNumber		= i_pItemGeneral->UniqueNumber;
	pQParam->ItemNum				= i_pChangeITEM->ItemNum;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemNum, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-06-14 by cmkwon
	if(i_bSendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMNUM, T_FC_STORE_UPDATE_ITEMNUM, pSUItemNum, SendBuf);
		pSUItemNum->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
		pSUItemNum->ItemNum				= i_pItemGeneral->ItemNum;
		pSUItemNum->ItemUpdateType		= i_nItemUpdateType;
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMNUM));
	}

	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////
// 아이템 자체를 삭제, Countable 아이템이더라도 개수에 무관하게 삭제
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteItemFromCharacterByPointer(ITEM_GENERAL *i_pItemGeneral,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int	 i_nGLogItemtype/*=0*/)	// 2012-11-21 by khkim, GLog 2차

{
	if (i_pItemGeneral == NULL)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mapItemGeneral);
	BOOL bRet = FALSE;

	if (i_enumStorage == ITEM_IN_CHARACTER)
	{// 이벤아이템 리스트에서 삭제
		bRet = m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);
	}
	else if (i_enumStorage == ITEM_IN_STORE)
	{// 창고 아이템 리스트에서 삭제
		bRet = m_mapItemInStore.deleteNoLock(i_pItemGeneral->UniqueNumber);
	}
	else
	{
		return FALSE;
	}

	if(FALSE == bRet)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::DeleteItemFromCharacterByPointer() Failed: Type(%d) ItemInfo(%s) CharInfo(%s)\r\n"
			, i_enumStorage, GetItemGeneralString(i_pItemGeneral, string()), GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()));
		return FALSE;
	}
	mtAuto.auto_unlock_cancel();

	this->ItemDeleteByInfinityPlaying(i_pItemGeneral);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 진행 중 아이템 삭제

	// start 2012-11-21 by khkim, GLog 2차 
	if(!COMPARE_RACE(this->m_pCharacter->Race, RACE_OPERATION|RACE_GAMEMASTER|RACE_MONITOR|RACE_GUEST|RACE_DEMO))		// 2013-01-18 by khkim, GLog (일반유저만 기록)	
	{
		if( NULL != m_pFieldIOCPSocket )
		{
			switch((FALSE == i_nGLogItemtype)? i_nItemUpdateType : i_nGLogItemtype)
			{
			case IUT_SHOP:
				{
					SetGLogData(GLOG_IL_LT_SHOP_SELL,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_SELL:
				{
					SetGLogData(GLOG_IL_LT_BAZAARSELL_GIVE,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_BAZAAR_BUY:
				{
					SetGLogData(GLOG_IL_LT_BAZAARBUY_GIVE,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_TRADE:
				{
					SetGLogData(GLOG_IL_LT_TREADGIVE,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			case IUT_LUCKY_ITEM:
				{
					SetGLogData(GLOG_IL_LT_LUCKYMACHINE_GET,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}	
			case IUT_GENERAL:
				{
					SetGLogData(GLOG_IL_LT_THROWAWAY,i_pItemGeneral,m_pFieldIOCPSocket->m_character.Level);
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
	// end 2012-11-21 by khkim, GLog 2차


	// DB에서 삭제
	if (i_bUpdateDB)
	{
		QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
		pQParam->AccountUniqueNumber = m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
		pQParam->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
// 2004-12-31 by cmkwon, 필요없음
//#ifdef _DEBUG
//		// 아이템 사라지는 오류 발견하기 위해 로그 남김
//		m_pFieldIOCPSocket->DBGOUT2(DO2_SYSLOG, "DeleteStoreItem: %s %s PF(%d) SF(%d) IUT: %d\r\n",
//						GetCharacterString(m_pCharacter, string()),
//						GetItemGeneralString(i_pItemGeneral, string()),
//						i_pItemGeneral->PrefixCodeNum, i_pItemGeneral->SuffixCodeNum,
//						i_nItemUpdateType);
//		m_pFieldIOCPSocket->PrintEnchant(i_pItemGeneral->UniqueNumber, TRUE);
//#endif
	}

	// MSG 보내기
	if (i_bSendMSG)
	{
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
		msgDelete->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
		msgDelete->ItemDeletionType = i_nItemUpdateType;
		m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
		switch(i_nItemUpdateType)
		{
		case IUT_TRADE:
		case IUT_BAZAAR_SELL:
		case IUT_BAZAAR_BUY:
			break;
		default:
			{
				m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0137, i_pItemGeneral->ItemInfo->ItemName);
			}
		}
	}

	// 2010-06-15 by shcho&hslee 펫시스템 - 소유 펫 정보 메모리 해제.
	if ( i_pItemGeneral && i_pItemGeneral->ItemInfo->Kind == ITEMKIND_PET_ITEM )
	{
		DeleteItemPetFromCharacterByUID ( i_pItemGeneral->UniqueNumber , i_bSendMSG , i_bDeleteMemory );
	}

	// free memory
	if (i_bDeleteMemory)
	{
		util::del(i_pItemGeneral);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 아이템 자체를 삭제, Countable 아이템이더라도 개수에 무관하게 삭제
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteItemFromCharacterByUID(UID64_t i_nItemUID,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int i_nGLogItemtype/*=0*/)			//  2012-11-21 by khkim, GLog 2차
{
	ITEM_GENERAL *i_pItemGeneral = GetItemGeneralByUID(i_nItemUID, i_enumStorage);
	if (i_pItemGeneral == NULL)
	{
		return FALSE;
	}

	return DeleteItemFromCharacterByPointer(i_pItemGeneral, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);	//  2012-11-21 by khkim, GLog 2차
}

BOOL CFieldItemManager::DeleteItemFromCharacterByItemNum(INT i_nItemNum,
							INT i_nDelCount,
							BYTE i_nItemUpdateType, BYTE i_enumStorage,
							BOOL i_bUpdateDB, BOOL i_bSendMSG, BOOL i_bDeleteMemory,
							int  i_nGLogItemtype/*=0*/ )			//  2012-11-21 by khkim, GLog 2차
{
	ITEM_GENERAL *pItemGeneral
		= GetFirstItemGeneralByItemNum(i_nItemNum, i_enumStorage);

	if (pItemGeneral == NULL)
	{
		return FALSE;
	}

	if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
	{
		// Countable Item인 경우
		UpdateItemCountByPointer(pItemGeneral, -i_nDelCount, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);		//  2012-11-21 by khkim, GLog 2차

		return TRUE;
	}

	// NON-Countable Item인 경우
	INT nRetDelCount = 0;
	DeleteItemFromCharacterByUID(pItemGeneral->UniqueNumber, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);		//  2012-11-21 by khkim, GLog 2차
	nRetDelCount++;

	while (i_nDelCount > nRetDelCount)
	{
		ITEM_GENERAL *pItemToDelete = GetFirstItemGeneralByItemNum(i_nItemNum, i_enumStorage);
		if (pItemToDelete == NULL)
		{
			return TRUE;
		}
		DeleteItemFromCharacterByUID(pItemToDelete->UniqueNumber, i_nItemUpdateType, i_enumStorage, i_bUpdateDB, i_bSendMSG, i_bDeleteMemory, i_nGLogItemtype);	//  2012-11-21 by khkim, GLog 2차
		nRetDelCount++;
	}

	return TRUE;
}


// 2010-06-15 by shcho&hslee 펫시스템 - 소유 펫 정보 메모리 해제.
/**************************************************************************************
**
**	소유 펫 정보 메모리 해제. 
**
**	Create Info : 2010-06-15 by shcho&hslee.
**
***************************************************************************************/
BOOL CFieldItemManager :: DeleteItemPetFromCharacterByUID ( UID64_t i_ItemUID , BOOL i_bSendMSG /*= TRUE*/ , BOOL i_bDeleteMemory /*= TRUE*/ )
{

	tPET_CURRENTINFO *pPetCurInfo = GetItemPetByUID ( i_ItemUID );

	if ( NULL == pPetCurInfo )
		return FALSE;

	if ( i_bSendMSG )
	{	// 클라이언트로 삭제 정보 전송.
		INIT_MSG_WITH_BUFFER(MSG_TRADE_PET_DATA, T_FC_TRADE_DELETE_CURRENT_PET_DATA, Petcurrentmsg, sendPetbuff);

		///////////////////////////////////////////////////////////////////////////////////////
		// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		//
		// 기존 
		//

// 		Petcurrentmsg->CreatedPetUID		= pPetCurInfo->CreatedPetUID;
// 		Petcurrentmsg->EnableEditPetName	= pPetCurInfo->EnableEditPetName;
// 		Petcurrentmsg->EnableLevelUp		= pPetCurInfo->EnableLevelUp;
// 		Petcurrentmsg->PetExp				= pPetCurInfo->PetExp;
// 		Petcurrentmsg->PetIndex				= pPetCurInfo->PetIndex;
// 		Petcurrentmsg->PetLevel				= pPetCurInfo->PetLevel;
// 
// 		memcpy( Petcurrentmsg->szPetName, pPetCurInfo->szPetName, SIZE_MAX_PET_NAME );

		/////////
		// 수정

		Petcurrentmsg->CreatedPetUID		= pPetCurInfo->CreatedPetUID;
		Petcurrentmsg->PetExp				= pPetCurInfo->PetExp;
		Petcurrentmsg->PetIndex				= pPetCurInfo->PetIndex;
		Petcurrentmsg->PetLevel				= pPetCurInfo->PetLevel;

		memcpy(Petcurrentmsg->PetName, pPetCurInfo->PetName, SIZE_MAX_PET_NAME);

		// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
		///////////////////////////////////////////////////////////////////////////////////////

		m_pFieldIOCPSocket->SendAddData( sendPetbuff, MSG_SIZE(MSG_TRADE_PET_DATA) ); // 펫 정보 전송
	}

	if ( i_bDeleteMemory )
	{
		DeleteItemPetByUID ( i_ItemUID );
	}

	return TRUE;

}
// End 2010-06-15 by shcho&hslee 펫시스템 - 소유 펫 정보 메모리 해제.

BOOL CFieldItemManager::DeleteAllItemGeneralFromCharacter(BOOL i_bOnlyNotAttachedItems /* = TRUE */)
{
	BOOL bDeleted = FALSE;

	m_mapItemGeneral.lock();
	while (TRUE)
	{
		bDeleted = FALSE;
		mtmapUID2ItemGeneral::iterator itrItemGeneral = m_mapItemGeneral.begin();
		while (itrItemGeneral != m_mapItemGeneral.end())
		{
			ITEM_GENERAL *pItem = itrItemGeneral->second;
			if (pItem->ItemNum != MONEY_ITEM_NUMBER
				&& IS_GENERAL_ITEM(pItem->Kind)
				&& POS_HIDDEN_ITEM != pItem->ItemWindowIndex	// // 2011-09-30 by hskim, 파트너 시스템 2차 - 숨겨진 아이템은 삭제 금지 (연결된 아이템이 삭제될때 같이 삭제됨)
				&& (i_bOnlyNotAttachedItems?(pItem->Wear == WEAR_NOT_ATTACHED):TRUE)
			)
			{
				bDeleted = DeleteItemFromCharacterByUID(pItem->UniqueNumber, IUT_ADMIN);
				break;
			}
			
			itrItemGeneral++;
		}
		if (!bDeleted) break;
	}
	m_mapItemGeneral.unlock();

	return bDeleted;
}

BOOL CFieldItemManager::DeleteAllItemSkillFromCharacter()
{
	ASSERT_NOT_IMPLEMENTED_YET();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::DeleteAllArenaItem()
/// \brief		아레나 전용 아이템을 삭제한다.
/// \author		dhjin
/// \date		2007-06-04 ~ 2007-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::DeleteAllArenaItem()
{
	m_mapItemGeneral.lock();

	BOOL bDeleted = FALSE;
	while (TRUE)
	{
		bDeleted = FALSE;
		mtmapUID2ItemGeneral::iterator itrItemGeneral = m_mapItemGeneral.begin();
		while (itrItemGeneral != m_mapItemGeneral.end())
		{
			ITEM_GENERAL *pItem = itrItemGeneral->second;
			if (COMPARE_BIT_FLAG(pItem->ItemInfo->ItemAttribute, ITEM_ATTR_ARENA_ITEM))
			{
				bDeleted = DeleteItemFromCharacterByUID(pItem->UniqueNumber, IUT_ARENA_ITEM);
				break;
			}
			
			itrItemGeneral++;
		}
		if (!bDeleted) break;
	}

	m_mapItemGeneral.unlock();
}

BOOL CFieldItemManager::LoadEnchantItem(UID64_t i_nItemUID, BOOL i_bLoadServer, BOOL i_bSendClient)
{
	ASSERT_NOT_IMPLEMENTED_YET();

	return FALSE;
}


 
/**********************************************************************************************************************************************************
**
**	겹쳐질 수 있는 카운터블 아이템들 정리 처리.
**	
**	Create Info : ??.??.??.
**
**	Update Info :	// 2008-02-18 by cmkwon, 카운터블 아이템인 아이템 습득시 인스턴스 여러개되는 버그 수정 - CFieldItemManager::RearrangeCountableItems() 함수 수정
**
**					// 클라이언트에서 럭키 머신 아이템 수량 업데이트 패킷을 무시하는 경우 확인되어 i_byItemUpdateType까지 인자로 넘겨받아 처리하도록 수정. 2010. 08. 02 by hsLee.
**
***********************************************************************************************************************************************************/
void CFieldItemManager::RearrangeCountableItems(BYTE i_enumStorage, int i_nItemNum/*=0*/ , BYTE i_byItemUpdateType /*= IUT_GENERAL*/ )
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	if (i_enumStorage == ITEM_IN_CHARACTER)
	{
		mtmapUID2ItemGeneral::iterator itrItemGeneral = m_mapItemGeneral.begin();
		while (itrItemGeneral != m_mapItemGeneral.end())
		{
			ITEM_GENERAL *pItemGeneral = itrItemGeneral->second;

			// 2008-02-18 by cmkwon, 카운터블 아이템인 아이템 습득시 인스턴스 여러개되는 버그 수정 - i_nItemNum 를 체크한다.			
			if(0 != i_nItemNum && pItemGeneral->ItemNum != i_nItemNum)
			{
				itrItemGeneral++;
				continue;
			}

			if (IS_COUNTABLE_ITEM(pItemGeneral->Kind))
			{
				ITEM_GENERAL *pItemGeneralFound = GetFirstItemGeneralByItemNum(pItemGeneral->ItemNum, ITEM_IN_CHARACTER);
				if (pItemGeneralFound != NULL && pItemGeneralFound->UniqueNumber != pItemGeneral->UniqueNumber)
				{
					// 2007-11-29 by cmkwon, 시스템 로그 수정
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] RearrangeItems %s: %s <- %s\r\n",
														GetCharacterString(&(m_pFieldIOCPSocket->m_character), string()),
														GetItemGeneralString(pItemGeneralFound, string()),
														GetItemGeneralString(pItemGeneral, string()));

					// 중복된 경우임
					//UpdateItemCountByPointer(pItemGeneralFound, pItemGeneral->CurrentCount, IUT_GENERAL, ITEM_IN_CHARACTER);
					UpdateItemCountByPointer(pItemGeneralFound , pItemGeneral->CurrentCount , i_byItemUpdateType , ITEM_IN_CHARACTER );

					if(g_pFieldGlobal->IsArenaServer())
					{
						// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
						this->ItemUpdateByInfinityPlaying(pItemGeneralFound);
						this->ItemDeleteByInfinityPlaying(pItemGeneral);
					}

					///////////////////////////////////////////////////////////////////////////////
					// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 인벤
					int nChangeCnts				= pItemGeneral->CurrentCount;
					pItemGeneral->CurrentCount	= 0;
					CAtumLogSender::SendLogMessageRearrangeDeleteItem(m_pFieldIOCPSocket, pItemGeneral, nChangeCnts);
					CAtumLogSender::SendLogMessageRearrangeItem(m_pFieldIOCPSocket, pItemGeneralFound, nChangeCnts);

					// DB에서 삭제
					QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber = m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
					pQParam->ItemUniqueNumber = pItemGeneral->UniqueNumber;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

					// MSG 보내기
					INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
					msgDelete->ItemUniqueNumber = pItemGeneral->UniqueNumber;
					//msgDelete->ItemDeletionType = IUT_GENERAL;
					msgDelete->ItemDeletionType = i_byItemUpdateType;
					m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
					m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0137, pItemGeneral->ItemInfo->ItemName);

					// map에서 삭제
					m_mapItemGeneral.erase(itrItemGeneral++);

					// memory에서 제거
					util::del(pItemGeneral);

					continue;
				}
			}

			itrItemGeneral++;
		}
	}
	else if (i_enumStorage == ITEM_IN_STORE)
	{
		mtmapUID2ItemGeneral::iterator itrItemInStore = m_mapItemInStore.begin();
		while (itrItemInStore != m_mapItemInStore.end())
		{
			ITEM_GENERAL *pItemInStore = itrItemInStore->second;

			// 2008-02-18 by cmkwon, 카운터블 아이템인 아이템 습득시 인스턴스 여러개되는 버그 수정 - i_nItemNum 를 체크한다.			
			if(0 != i_nItemNum && pItemInStore->ItemNum != i_nItemNum)
			{
				itrItemInStore++;
				continue;
			}

			if (IS_COUNTABLE_ITEM(pItemInStore->Kind))
			{
				ITEM_GENERAL *pItemInStoreFound = GetFirstItemGeneralByItemNum(pItemInStore->ItemNum, ITEM_IN_STORE);
				if (pItemInStoreFound != NULL && pItemInStoreFound->UniqueNumber != pItemInStore->UniqueNumber)
				{
					// 2007-11-29 by cmkwon, 시스템 로그 수정
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] RearrangeItems %s: %s <- %s\r\n",
														GetCharacterString(&(m_pFieldIOCPSocket->m_character), string()),
														GetItemGeneralString(pItemInStoreFound, string()),
														GetItemGeneralString(pItemInStore, string()));

					// 중복된 경우임
					//UpdateItemCountByPointer(pItemInStoreFound, pItemInStore->CurrentCount, IUT_GENERAL, ITEM_IN_STORE);
					UpdateItemCountByPointer(pItemInStoreFound , pItemInStore->CurrentCount , i_byItemUpdateType , ITEM_IN_STORE );

					///////////////////////////////////////////////////////////////////////////////
					// 2007-11-29 by cmkwon, 카운터블아이템 합쳐지는 게임 로그 남기기 - 창고
					int nChangeCnts				= pItemInStore->CurrentCount;
					pItemInStore->CurrentCount	= 0;
					CAtumLogSender::SendLogMessageRearrangeDeleteItem(m_pFieldIOCPSocket, pItemInStore, nChangeCnts, TRUE);
					CAtumLogSender::SendLogMessageRearrangeItem(m_pFieldIOCPSocket, pItemInStoreFound, nChangeCnts, TRUE);

					// DB에서 삭제
					QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
					pQParam->AccountUniqueNumber = m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
					pQParam->ItemUniqueNumber = pItemInStore->UniqueNumber;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

					// MSG 보내기
					INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
					msgDelete->ItemUniqueNumber = pItemInStore->UniqueNumber;
					//msgDelete->ItemDeletionType = IUT_GENERAL;
					msgDelete->ItemDeletionType = i_byItemUpdateType;
					m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
					m_pFieldIOCPSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0137, pItemInStore->ItemInfo->ItemName);

					// map에서 삭제
					m_mapItemInStore.erase(itrItemInStore++);

					// memory에서 제거
					util::del(pItemInStore);
					continue;
				}
			}// end_if (IS_COUNTABLE_ITEM(pItemInStore->Kind))
			
			itrItemInStore++;
		}// end_while (itrItemInStore != m_mapItemInStore.end())
	}
	return;
}

// 2006-01-24 by cmkwon, 아이템에 중량 인챈트 적용
// 2006-10-13 by cmkwon, 리턴값 자료형 변경(float-->int)
// 2007-11-02 by cmkwon, 중량 구하는 시스템 변경 - 리턴값 자료형 변경(<--int)
float CFieldItemManager::GetCurrentCarryingItemWeight()
{
	float retTotalWeight = 0.0f;

	m_mapItemGeneral.lock();

	map<UID64_t, ITEM_GENERAL*>::iterator itr = m_mapItemGeneral.begin();

	while (itr != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemGeneral = itr->second;

// 2005-09-27 by cmkwon, 무게게이즈는 장착된 아이템만 적용하도록 수정함
//		retTotalWeight += CAtumSJ::GetItemWeight(pItemGeneral->ItemInfo, pItemGeneral->CurrentCount);
		if(WEAR_ATTACHED == pItemGeneral->Wear)
		{
			retTotalWeight += CAtumSJ::GetItemWeight(pItemGeneral->ItemInfo, pItemGeneral->CurrentCount, pItemGeneral->DesWeight);
		}
		itr++;
	}

	m_mapItemGeneral.unlock();

	return retTotalWeight;
}

void CFieldItemManager::DeleteItemSkillBySkillBaseNum(INT i_nSkillItemNum, BOOL i_bSendMsg)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	
	vectItemSkillPtr::iterator itr = m_vectItemSkillPtr.begin();
	while (itr != m_vectItemSkillPtr.end())
	{
		ITEM_SKILL *pItemSkill = *itr;
		// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if ( pItemSkill->ItemInfo->NextSkill == i_nSkillItemNum )
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if (SKILL_BASE_NUM(pItemSkill->ItemNum) == SKILL_BASE_NUM(i_nSkillItemNum))
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정
		{			
			if (i_bSendMsg)
			{// send msg

				INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, pDelSkill, pDelSkillBuf);
				pDelSkill->ItemUniqueNumber = pItemSkill->UniqueNumber;
				pDelSkill->ItemDeletionType = IUT_SKILL;
				m_pFieldIOCPSocket->SendAddData(pDelSkillBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
			}

			itr = m_vectItemSkillPtr.erase(itr);			
			util::del(pItemSkill);			// release memory

			continue;							// continue searching
		}
		itr++;
	}
}

ITEM_SKILL *CFieldItemManager::GetFirstSkillBySkillBaseNum(INT i_nSkillItemNum)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	vectItemSkillPtr::iterator itr = m_vectItemSkillPtr.begin();
	while (itr != m_vectItemSkillPtr.end())
	{
		ITEM_SKILL *pItemSkill = *itr;
		// 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정
#ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if ( pItemSkill->ItemInfo->NextSkill == i_nSkillItemNum )
#else	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		if (SKILL_BASE_NUM(pItemSkill->ItemNum) == SKILL_BASE_NUM(i_nSkillItemNum))
#endif	// #ifdef S_SKILL_NUMBERING_RENEWAL_JHSEOL
		// end 2013-03-12 by jhseol, 스킬 넘버링 시스템 수정
		{
			return pItemSkill;
		}

		itr++;
	}
	return NULL;
}

ITEM_SKILL *CFieldItemManager::GetItemSkillByUID(UID64_t i_nSkillUID)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	for(int i=0; i < m_vectItemSkillPtr.size(); i++)
	{
		if(m_vectItemSkillPtr[i]->UniqueNumber == i_nSkillUID)
		{
			return m_vectItemSkillPtr[i];
		}
	}

	return NULL;
}


// 2010-06-15 by shcho&hslee 펫시스템

// ITEM_PET 관련.
void CFieldItemManager :: DeleteItemPetByUID ( UID64_t i_nPetUID )
{
	mt_auto_lock mtauto(&m_vectPetCurrentInfo);

	mtvectPetCurrentInfo::iterator itr = m_vectPetCurrentInfo.begin();

	for ( ; itr != m_vectPetCurrentInfo.end(); ++itr )
	{
		if ( (*itr)->CreatedPetUID == i_nPetUID )
		{
			if((*itr) != NULL)
			{
				/////////////////////////////////////////////////
				// start 2011-09-20 by hskim, 파트너 시스템 2차
				// 종속적인 소켓 아이템 지우기
				for(int a=0; a<SIZE_MAX_PETSOCKET; a++)
				{
					if( 0 != (*itr)->PetSocketItemUID[a] )
					{
						DeleteItemFromCharacterByUID((*itr)->PetSocketItemUID[a], IUT_DEPENDENCY_ITEM);
					}
				}
				// end 2011-09-20 by hskim, 파트너 시스템 2차
				/////////////////////////////////////////////////
				
			util::del ( *itr );
			}
			m_vectPetCurrentInfo.erase( itr );
			break;
		}
	}

	mtauto.auto_unlock_cancel();
}

tPET_CURRENTINFO *CFieldItemManager :: GetItemPetByUID ( UID64_t i_nPetUID )
{
	mt_auto_lock mtauto ( &m_vectPetCurrentInfo );

	mtvectPetCurrentInfo::iterator itr = m_vectPetCurrentInfo.begin();

	//for ( ; itr != m_vectPetCurrentInfo.end(); ++itr )
	while ( itr != m_vectPetCurrentInfo.end() )
	{
		if ( (*itr)->CreatedPetUID == i_nPetUID )
			return (*itr);

		++itr;
	}

	mtauto.auto_unlock_cancel();

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////
// start 2011-09-20 by hskim, 파트너 시스템 2차

BOOL CFieldItemManager::IsBelongPetSocketItemByUID(UID64_t i_nPetID, UID64_t i_nSocketID)
{
	mt_auto_lock mtauto(&m_vectPetCurrentInfo);

	tPET_CURRENTINFO *pCurrentPet = GetItemPetByUID(i_nPetID);
	
	if( NULL != pCurrentPet )
	{
		for(int a=0; a<pCurrentPet->PetEnableSocketCount; a++)
		{
			if( i_nSocketID == pCurrentPet->PetSocketItemUID[a] )
			{
				return TRUE;
			}
		}
	}

	mtauto.auto_unlock_cancel();

	return FALSE;
}

// end 2011-09-20 by hskim, 파트너 시스템 2차
///////////////////////////////////////////////////////////////////////////////////////

// End 2010-06-15 by shcho&hslee 펫시스템

void CFieldItemManager::CheckRequirementAllGeneralItems(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	for(; m_mapItemGeneral.end() != itr; itr++)
	{
		ITEM_GENERAL *pItemG = itr->second;
		if(pItemG->Wear == WEAR_ATTACHED)
		{			
			Err_t nErr = m_pFieldIOCPSocket->CheckGeneralItemRequirements(pItemG);
			if ( nErr != ERR_NO_ERROR
				|| (ITEMKIND_SUPPORT != pItemG->Kind && m_pFieldIOCPSocket->GetCurrentLoadagePercentage() > 1.0f) )
			{
				int nTmpWindowIndex		= pItemG->ItemWindowIndex;	// 기존 위치

				///////////////////////////////////////////////////////////////////////////////
				// 2006-10-23 by cmkwon, 장착 해제 처리함
				m_pFieldIOCPSocket->UpdateWearItemPointer(pItemG->UniqueNumber, pItemG->UniqueNumber, pItemG->ItemWindowIndex, POS_ITEMWINDOW_OFFSET + nTmpWindowIndex);
				m_pFieldIOCPSocket->OnDetachItem(pItemG, pItemG->ItemWindowIndex, FALSE);

				// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 아이템 강제 장착해제시 처리
				m_pFieldIOCPSocket->OnUpdateWearItem(pItemG->UniqueNumber, NULL, pItemG->ItemWindowIndex, POS_ITEMWINDOW_OFFSET + nTmpWindowIndex);

				pItemG->Wear			= WEAR_NOT_ATTACHED;
				pItemG->ItemWindowIndex	= POS_ITEMWINDOW_OFFSET + nTmpWindowIndex;

				///////////////////////////////////////////////////////////////////////////////
				// 클라이언트에 창착 해제 전송
				INIT_MSG_OF_SIZE(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK, T_FC_ITEM_CHANGE_WINDOW_POSITION_OK, pMsgChangeOK, pMsgChangeOKBuf);
				pMsgChangeOK->UniqueNumber			= pItemG->UniqueNumber;
				pMsgChangeOK->ItemWindowIndex		= pItemG->ItemWindowIndex;
				pMsgChangeOK->Wear					= pItemG->Wear;
				pMsgChangeOK->UniqueNumberDest		= 0;
				pMsgChangeOK->ItemWindowIndexDest	= nTmpWindowIndex;
				pMsgChangeOK->WearDest				= 0;
				m_pFieldIOCPSocket->SendAddData(pMsgChangeOKBuf, MSG_SIZE(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK));

				///////////////////////////////////////////////////////////////////////////////
				// DB에 장착 해제 저장
				QPARAM_CHANGE_ITEM_WINDOW_POSITION *pQParamChangeWindowPosition	= new QPARAM_CHANGE_ITEM_WINDOW_POSITION;
				pQParamChangeWindowPosition->AccountUID			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pQParamChangeWindowPosition->CharacterUID		= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				pQParamChangeWindowPosition->ItemUID1			= pItemG->UniqueNumber;
				pQParamChangeWindowPosition->ItemWear1			= pItemG->Wear;
				pQParamChangeWindowPosition->ItemWindowIndex1	= pItemG->ItemWindowIndex;
				pQParamChangeWindowPosition->ItemUID2			= 0;
				pQParamChangeWindowPosition->ItemWindowIndex2	= POS_INVALID_POSITION;
				pQParamChangeWindowPosition->ItemWear2			= WEAR_NOT_ATTACHED;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ChangeItemPosition, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamChangeWindowPosition);
			}
		}
	}
}

INT CFieldItemManager::GetInventoryItemCounts(void)		// 2011-10-21 by hskim, 파트너 시스템 2차
{
	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-09-20 by hskim, 파트너 시스템 2차
	//
	// 기존 

	//return m_mapItemGeneral.size();

	/////////
	// 수정

	return m_mapItemGeneral.size() - m_nInventoryHiddenItemCount;

	// end 2011-09-20 by hskim, 파트너 시스템 2차
	///////////////////////////////////////////////////////////////////////////////////////
}

// start 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자
INT CFieldItemManager::GetInventoryHiddenItemCounts(void)
{
	return m_nInventoryHiddenItemCount;
}

INT CFieldItemManager::CalcInventoryHiddenItemCounts(void)
{
	INT nItemCount = 0;
	mt_auto_lock igLock(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	while(itr != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemG = itr->second;

		if( NULL != pItemG->ItemInfo && pItemG->ItemWindowIndex == POS_HIDDEN_ITEM )
		{
			nItemCount++;
		}

		itr++;
	}

	m_nInventoryHiddenItemCount = nItemCount;

	igLock.auto_unlock_cancel();

	return nItemCount;
}
// end 2011-10-21 by hskim, 파트너 시스템 2차 - 현재 가지고 있는 숨겨진 아이템 (POS_HIDDEN_ITEM) 숫자

// 2013-05-22 by jhseol, 소스정리 - 주석삭제

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::UpdateItemStorage(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage)
/// \brief		
/// \author		cmkwon
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateItemStorage(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage)
{
	if (m_pFieldIOCPSocket->m_pCurrentBuildingNPC == NULL
		|| m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind != BUILDINGKIND_STORE)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mapItemGeneral);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-23 by dhjin, 여단 창고 <--> 캐릭터인벤 이동 처리
	// 캐릭터 인벤처리만 하고 DB Update는 하지 않음
	if(ITEM_IN_GUILD_STORE == i_enFromStorage
		||ITEM_IN_GUILD_STORE == i_enToStorage)
	{
		if (ITEM_IN_CHARACTER == i_enFromStorage
			&& ITEM_IN_GUILD_STORE == i_enToStorage)
		{// 캐릭터인벤->여단창고
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);

			RemoveTimerFixedTermShape(i_pItemGeneral);			// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

			return TRUE;
		}
		else if (ITEM_IN_GUILD_STORE == i_enFromStorage
				&& ITEM_IN_CHARACTER == i_enToStorage)
		{// 여단창고->케릭터인벤
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemGeneral.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			// 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::UpdateItemStorage_1 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그

			RegisterTimerFixedTermShape(i_pItemGeneral);		// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

			return TRUE;
		}
	}

	if (m_pFieldIOCPSocket->m_character.CharacterUniqueNumber == m_uid32StoreCharacterUID)
	{// 2006-09-18 by dhjin, 자기 자신 케릭터 일때
		if (ITEM_IN_CHARACTER == i_enToStorage)
		{// 창고->캐릭터인벤
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemInStore.deleteNoLock(i_pItemGeneral->UniqueNumber);
			m_mapItemGeneral.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			// 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::UpdateItemStorage_2 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그

			RegisterTimerFixedTermShape(i_pItemGeneral);		// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
		}
		else if (ITEM_IN_STORE == i_enToStorage)
		{// 캐릭터인벤->창고
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemInStore.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);

			RemoveTimerFixedTermShape(i_pItemGeneral);			// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
		}
		else
		{
			return FALSE;
		}
	}
	else
	{// 2006-09-18 by dhjin, 다른 케릭터 일때

		// start 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동	
		if( i_pItemGeneral && i_pItemGeneral->ItemInfo->Kind == ITEMKIND_PET_ITEM )
		{
			UpdateItemStorageSubItem(i_pItemGeneral, i_enFromStorage, i_enToStorage);
		}
		// end 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

		if (ITEM_IN_CHARACTER == i_enToStorage)
		{// 창고->캐릭터인벤
			i_pItemGeneral->ItemStorage = i_enToStorage;
			m_mapItemInStore.deleteNoLock(i_pItemGeneral->UniqueNumber);
			i_pItemGeneral->Possess		= m_pCharacter->CharacterUniqueNumber;
			m_mapItemGeneral.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			// 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그
#ifdef S_ITEMUID_ZERO_DEBUG_LOG_JHSEOL
			if (IS_COUNTABLE_ITEM(i_pItemGeneral->Kind))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG] SPI ItemUID Zero : CFieldItemManager::UpdateItemStorage_3 - CUID(%d), ItemNum(%d), Count(%d), ItemUID(%d)\r\n"
					, m_pFieldIOCPSocket->m_character.CharacterUniqueNumber, i_pItemGeneral->ItemNum, i_pItemGeneral->CurrentCount, i_pItemGeneral->UniqueNumber);
			}
#endif
			// end 2013-06-25 by jhseol, 아이템 UID '0' 추적용 디버깅 로그

			RegisterTimerFixedTermShape(i_pItemGeneral);		// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
		}
		else if (ITEM_IN_STORE == i_enToStorage)
		{// 캐릭터인벤->창고
			i_pItemGeneral->ItemStorage = i_enToStorage;
			i_pItemGeneral->Possess		= m_uid32StoreCharacterUID;
			m_mapItemInStore.insertNoLock(i_pItemGeneral->UniqueNumber, i_pItemGeneral);
			m_mapItemGeneral.deleteNoLock(i_pItemGeneral->UniqueNumber);

			RemoveTimerFixedTermShape(i_pItemGeneral);			// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
		}
		else
		{
			return FALSE;
		}	
	}
	
	QPARAM_STORE_UPDATE *qQParamStoreUpdate = new QPARAM_STORE_UPDATE;
	qQParamStoreUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
	qQParamStoreUpdate->CharacterUID		= i_pItemGeneral->Possess;
	qQParamStoreUpdate->ItemStorage			= i_enToStorage;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemStorage, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, qQParamStoreUpdate);
	
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMSTORAGE, T_FC_STORE_UPDATE_ITEMSTORAGE, pMsgUpdate, SendBuf);
	pMsgUpdate->ItemUniqueNumber	= i_pItemGeneral->UniqueNumber;
	pMsgUpdate->FromItemStorage		= i_enFromStorage;
	pMsgUpdate->ToItemStorage		= i_enToStorage;
	m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMSTORAGE));
	
	// 2011-02-07 by shcho, 펫 데이터 정보 재전송 추가
	// 내 펫 정보에서 가지고 있는 펫 데이터 정보를 찾아서 재전송 한다.

	tPET_CURRENTINFO* pSendPetCurrentData;
	pSendPetCurrentData = m_pFieldIOCPSocket->m_ItemManager.GetItemPetByUID(i_pItemGeneral->UniqueNumber);
	// 펫일 경우에 펫정보를 보낸다.
	if(NULL!=pSendPetCurrentData)
	{
		m_pFieldIOCPSocket->SendInsertPetItemInfo(ITEM_IN_CHARACTER,pSendPetCurrentData);
	}
	// end 2011-02-07 by shcho, 펫 데이터 정보 재전송 추가

	return TRUE;
}

// start 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동
BOOL CFieldItemManager::UpdateItemStorageSubItem(ITEM_GENERAL *i_pItemGeneral, BYTE i_enFromStorage, BYTE i_enToStorage)
{
	// 파트너 아이템이 창고 계정간 이동(멤버쉽)될 경우 실행
	// 파트너에 장착된 소켓아이템도 같이 변경 하기 위해 작업

	tPET_CURRENTINFO *pPetCurrentData = NULL;
	pPetCurrentData = m_pFieldIOCPSocket->m_ItemManager.GetItemPetByUID(i_pItemGeneral->UniqueNumber);

	if( NULL != pPetCurrentData )
	{
		mt_auto_lock mtAuto(&m_mapItemGeneral);

		for(int i=0; i<SIZE_MAX_PETSOCKET; i++)
		{
			UID32_t Possess = 0;

			if( 0 != pPetCurrentData->PetSocketItemUID[i] )
			{
				// DB의 파트너 오토 스킬 정보 초기화

				QPARAM_PET_SET_AUTOSKILL_SLOT *pQParamClearAutoSkill = new QPARAM_PET_SET_AUTOSKILL_SLOT;
				pQParamClearAutoSkill->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
				memset(&pQParamClearAutoSkill->PetAutoSkill, 0, sizeof(pQParamClearAutoSkill->PetAutoSkill));
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_PetSetAutoSkillSlot, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamClearAutoSkill);

				// 메모리상의 파트너 오토 스킬 정보 초기화
				
				memset(&pPetCurrentData->PetAutoSkill, 0, sizeof(pPetCurrentData->PetAutoSkill));

				if( ITEM_IN_CHARACTER == i_enToStorage )
				{
					// 창고->캐릭터인벤

					Possess = m_pCharacter->CharacterUniqueNumber;

					QPARAM_STORE_GET_ITEM_ONE *qQParamStoreUpdate = new QPARAM_STORE_GET_ITEM_ONE;
					qQParamStoreUpdate->ItemUniqueNumber	= pPetCurrentData->PetSocketItemUID[i];
					qQParamStoreUpdate->bSendToClient	= TRUE;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_StoreGetItemOne, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, qQParamStoreUpdate);
				}
				else if( ITEM_IN_STORE == i_enToStorage )
				{
					// 캐릭터인벤->창고

					Possess = m_uid32StoreCharacterUID;

					ITEM_GENERAL *pItemGDel = GetItemGeneralByUID(pPetCurrentData->PetSocketItemUID[i]);
					
					if( NULL != pItemGDel )
					{
						m_mapItemGeneral.deleteNoLock(pPetCurrentData->PetSocketItemUID[i]);
						util::del(pItemGDel);
					}

					INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
					msgDelete->ItemUniqueNumber = pPetCurrentData->PetSocketItemUID[i];
					msgDelete->ItemDeletionType = IUT_DEPENDENCY_ITEM;
					m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));
				}

				// 파트너 아이템 소켓 소속 변경 진행

				QPARAM_PET_CHANGE_SOCKET_OWNER *qQParamStoreUpdate = new QPARAM_PET_CHANGE_SOCKET_OWNER;
				qQParamStoreUpdate->ItemUniqueNumber	= pPetCurrentData->PetSocketItemUID[i];
				qQParamStoreUpdate->CharacterUID		= Possess;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_PetChangeSocketOwner, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, qQParamStoreUpdate);
			}
		}
	}

	return TRUE;
}
// end 2012-01-30 by hskim, 파트너 시스템 2차 - 개인 창고 이동

void CFieldItemManager::DBG_PRINT_ITEM()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	int nCnts = 1;
	mtmapUID2ItemGeneral::iterator tmpItr = m_mapItemGeneral.begin();
	while (m_mapItemGeneral.end() != tmpItr)
	{
		ITEM_GENERAL *pItemG = tmpItr->second;

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] %30s Inventory Item: %s ItemCount(%d)\r\n",
				GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()),
				GetItemGeneralString(pItemG, string()), nCnts);

		nCnts++;
		tmpItr++;
	}

	for(int i=0; i < m_vectItemSkillPtr.size(); i++)	
	{
		ITEM_SKILL *pSkill = m_vectItemSkillPtr[i];

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] %30s Skill: %s SkillCount(%d)\r\n",
				GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()),
				GetItemSkillString(pSkill, string()), i+1);
	}


	m_pFieldIOCPSocket->m_mapTradeItem.lock();
	nCnts = 1;
	mtmapUID2TradeItem::iterator tmpItrTrade = m_pFieldIOCPSocket->m_mapTradeItem.begin();
	while (m_pFieldIOCPSocket->m_mapTradeItem.end() != tmpItrTrade)
	{
		ITEM_GENERAL *pTrdItemG = tmpItrTrade->second.pStoreItem;

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "  [Notify] %30s Trade Item: %s ItemCount(%d)\r\n",
				GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()),
				GetItemGeneralString(pTrdItemG, string()), nCnts);

		nCnts++;
		tmpItrTrade++;
	}
	m_pFieldIOCPSocket->m_mapTradeItem.unlock();

	mtAuto.auto_unlock_cancel();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-01 ~ 2009-04-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldItemManager::ChangeItemNum(INT i_nFromItemNum, INT i_nToItemNum)
{
	int nCnt = 0;
	mt_auto_lock igLock(&m_mapItemGeneral);
	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	for(; itr != m_mapItemGeneral.end(); itr++)
	{
		ITEM_GENERAL *pItemG = itr->second;
		if (pItemG->ItemNum == i_nFromItemNum)
		{
			nCnt++;

			///////////////////////////////////////////////////////////////////////////////
			// 변경된 정보를 설정한다.
			pItemG->ItemNum					= i_nToItemNum;
			pItemG->ItemInfo				= ms_pFieldIOCP2->GetItemInfo(i_nToItemNum);

			///////////////////////////////////////////////////////////////////////////////
			// 변경된 ItemNum을 DB에도 저장한다.
			QPARAM_UPDATEITEMNUM *pQParam	= new QPARAM_UPDATEITEMNUM;
			pQParam->ItemUniqueNumber		= pItemG->UniqueNumber;
			pQParam->ItemNum				= i_nToItemNum;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemNum, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);			

			///////////////////////////////////////////////////////////////////////////////
			// 2009-04-06 by cmkwon, 세력초기화 시스템 추가 - 변경된 ItemNum 정보도 같이 전송한다.
			INIT_MSG_WITH_BUFFER(MSG_FC_STORE_UPDATE_ITEMNUM, T_FC_STORE_UPDATE_ITEMNUM, pSUItemNum, SendBuf);
			pSUItemNum->ItemUniqueNumber	= pItemG->UniqueNumber;
			pSUItemNum->ItemNum				= pItemG->ItemNum;
			pSUItemNum->ItemUpdateType		= IUT_GENERAL;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_UPDATE_ITEMNUM));
		}
	}
	
	return nCnt;
}

// 2013-04-12 by jhseol, 필드창고 사용으로 더블인첸트 버그 수정
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		일반 창고 사용 여부도 별도로 만들어 이중 체크해야함.
/// \author		jhseol
/// \date		2013-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::GetUsingStore(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	return m_bUsingStore;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		일반 창고 사용 여부도 별도로 만들어 이중 체크해야함.
/// \author		jhseol
/// \date		2013-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::SetUsingStore(BOOL i_bFlag)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	m_bUsingStore		= i_bFlag;
}
// end 2013-04-12 by jhseol, 필드창고 사용으로 더블인첸트 버그 수정

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::GetUsingFieldStore(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	return m_bUsingFieldStore;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::SetUsingFieldStore(BOOL i_bFlag)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	m_bUsingFieldStore		= i_bFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsUseableFieldStore(void)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	ITEM_GENERAL *pItemG = GetItemGeneralByDestParam(DES_FIELD_STORE);

	if(NULL == pItemG)
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-23 by cmkwon, 필드창고 캐쉬 아이템 구현 - 
/// \author		cmkwon
/// \date		2009-09-25 ~ 2009-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL * CFieldItemManager::GetItemGeneralByDestParam(DestParam_t i_byDestParam, BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/)		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-25 by cmkwon, CFieldItemManager::GetItemGeneralByUID() ItemUID 체크 추가 - 
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = NULL;
	if (i_enumStorage == ITEM_IN_CHARACTER)
	{
		mtmapUID2ItemGeneral::iterator itr(m_mapItemGeneral.begin());
		for(; itr != m_mapItemGeneral.end(); itr++)
		{
			pItemG = itr->second;
			if(pItemG->ItemInfo->IsExistDesParam(i_byDestParam))
			{
				return pItemG;
			}
		}

		return NULL;
	}

	if (i_enumStorage == ITEM_IN_STORE)
// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::GetItemGeneralByDestParam#,
//		&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC != NULL
//		&& m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind == BUILDINGKIND_STORE)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-10-30 by cmkwon, 창고 아이템 가져오기 관련 버그 수정 - CFieldItemManager::GetItemGeneralByDestParam#, 
		if(NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC
			|| BUILDINGKIND_STORE != m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind)
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::GetItemGeneralByDestParam# State error !! %s DestParam(%d) pBuildingNPC(0x%X) BuildingKind(%d) \r\n"
				, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), i_byDestParam, m_pFieldIOCPSocket->m_pCurrentBuildingNPC, (NULL == m_pFieldIOCPSocket->m_pCurrentBuildingNPC)?-1:m_pFieldIOCPSocket->m_pCurrentBuildingNPC->BuildingKind);
			return NULL;
		}

		mtmapUID2ItemGeneral::iterator itr(m_mapItemInStore.begin());
		for(; itr != m_mapItemInStore.end(); itr++)
		{
			pItemG = itr->second;
			if(pItemG->ItemInfo->IsExistDesParam(i_byDestParam))
			{
				return pItemG;
			}
		}
		
		return NULL;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 
/// \author		cmkwon
/// \date		2009-11-02 ~ 2009-11-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CFieldItemManager::GetMaxInventoryCount(BYTE i_enStorage/*=ITEM_IN_CHARACTER*/)
{
	switch(i_enStorage)
	{
	case ITEM_IN_CHARACTER:		return CAtumSJ::GetMaxInventorySize(this->IsExistPremiumCard(), m_pFieldIOCPSocket->m_character.GetAddedPermanentInventoryCount(i_enStorage));
	case ITEM_IN_STORE:
		{
			if(m_uid32StoreCharacterUID == m_pCharacter->CharacterUniqueNumber)
			{
				return CAtumSJ::GetMaxStoreSize(this->IsExistPremiumCard(), m_pFieldIOCPSocket->m_character.GetAddedPermanentInventoryCount(i_enStorage));
			}
			return CAtumSJ::GetMaxStoreSize(this->IsExistPremiumCard(), m_pFieldIOCPSocket->GetAddedPermanentInventoryCountOfOthersCharacter(m_uid32StoreCharacterUID));
		}
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::Add2UsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen)
/// \brief		
///				// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 사용류 아이템 파람 적용 관련 버그 수정
/// \author		cmkwon
/// \date		2006-03-29 ~ 2006-03-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::Add2UsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bResetAllSkillAndEnchant/*=FALSE*/)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(i_pItemGen == m_vectUsingGeneralItem[i])
		{
			return FALSE;
		}
	}

	i_pItemGen->UsingStartTime = ATUM_DATE_TIME { true };			// 2006-03-30 by cmkwon, 아이템 사용 시작시간을 설정한다.
	m_vectUsingGeneralItem.push_back(i_pItemGen);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 사용류 아이템 파람 적용 관련 버그 수정
	if(FALSE == IS_ATTACHABLE_ITEM(i_pItemGen->ItemInfo)
		&& i_bResetAllSkillAndEnchant)
	{
		m_pFieldIOCPSocket->ResetAllSkillAndEnchant();
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM_GENERAL * CFieldItemManager::Stop8StoreFromUsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bDeleteUsingTimeLimitedItem)
/// \brief		사용중인 시간제한 아이템에서 인자로 받은 아이템을 찾아서 사용리스트에서 제거하고 사용된 시간을 계산하여 DB에 저장한다.
/// \author		cmkwon
/// \date		2006-03-29 ~ 2006-03-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL * CFieldItemManager::Stop8StoreFromUsingTimeLimitedItemList(ITEM_GENERAL *i_pItemGen, BOOL i_bDeleteUsingTimeLimitedItem)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if(i_pItemGen == m_vectUsingGeneralItem[i])
		{
			///////////////////////////////////////////////////////////////////////////////
			// 2006-03-29 by cmkwon, 사용된 시간을 계산하고 DB에 저장한다.
			ATUM_DATE_TIME atimeCur { true };
			i_pItemGen->UsingTimeStamp	= i_pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(i_pItemGen->UsingStartTime);
			i_pItemGen->UsingStartTime	= atimeCur;
	
			QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= i_pItemGen->UniqueNumber;
			pQParam->UsingTimeStamp1	= i_pItemGen->UsingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

			if(i_bDeleteUsingTimeLimitedItem)
			{
				///////////////////////////////////////////////////////////////////////////////
				// 2006-03-29 by cmkwon, 사용중 아이템 벡터 리스트에서 제거한다.                                                         
//				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] Stop8StoreFromUsingTimeLimitedItemList ItemUniqueNumber = %d\r\n"
//					, m_vectUsingGeneralItem[i]->UniqueNumber);
				m_vectUsingGeneralItem.erase(m_vectUsingGeneralItem.begin() + i);
			}
			return i_pItemGen;
		}
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList(BOOL i_bAfterArena = FALSE)
/// \brief		모든 사용중 시간제한 아이템에 사용 시작 시간을 설정하고 아이템 정보를 클라이언트로 전송한다., 2008-02-01 by dhjin, 아레나 통합 - 아레나 종료후 게임서버에서 시작시 체크 비트 추가
/// \author		cmkwon
/// \date		2005-XX-XX ~ 2005-XX-XX
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList(BOOL i_bAfterArena /* = FALSE*/)
{
	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		if ( NULL == m_vectUsingGeneralItem[i]->ItemInfo )
		{
			g_pFieldGlobal->WriteSystemLogEX ( FALSE , " [ERROR] CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList# ITEM Info NULL!! ItemNum(%d) Loop i(%d)\r\n" ,
				m_vectUsingGeneralItem[i]->ItemNum , i );
			
			m_vectUsingGeneralItem[i]->ItemInfo = this->ms_pFieldIOCP2->GetItemInfo( m_vectUsingGeneralItem[i]->ItemNum );
			
			if ( NULL == m_vectUsingGeneralItem[i]->ItemInfo )
				continue;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 현재 시간을 설정한다
		m_vectUsingGeneralItem[i]->UsingStartTime = atimeCur;
		
		// 2012-11-21 by hskim, 캐나다 서버 죽는 문제 보안 - 추후 정리되면 해당 코드 삭제 예정
		if( NULL == m_pFieldIOCPSocket )
		{
			g_pFieldGlobal->WriteSystemLogEX ( FALSE , " [DEBUG] CFieldItemManager::ResetTimeStampUsingTimeLimitedItemList# m_pFieldIOCPSocket is NULL!! ItemNum(%d) Loop i(%d) Loop size (%d)\r\n" ,
				m_vectUsingGeneralItem[i]->ItemNum , i, m_vectUsingGeneralItem.size());

			continue;		// 의미없음 -- 어짜피 터질거임
		}
		// end 2012-11-21 by hskim, 캐나다 서버 죽는 문제 보안

		if(i_bAfterArena)
		{// 2008-03-05 by dhjin, 아레나 경기 복귀 시 setparam은 되어 있으므로 스텔스 카드 부분만 따로 처리한다.
			// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
			//if(DES_CASH_STEALTH == m_vectUsingGeneralItem[i]->ItemInfo->DestParameter1)
			if(DES_CASH_STEALTH == m_vectUsingGeneralItem[i]->ItemInfo->ArrDestParameter[0])
			{
				m_pFieldIOCPSocket->m_bStealthState	= TRUE;
			}
		}
		else
		{
			m_pFieldIOCPSocket->ApplyCardItem(m_vectUsingGeneralItem[i]->ItemInfo);
		}
		
		INIT_MSG_WITH_BUFFER(MSG_FC_STORE_INSERT_USINGITEM, T_FC_STORE_INSERT_USINGITEM, pSUsingItem, SendBuf);
		pSUsingItem->ItemNum				= m_vectUsingGeneralItem[i]->ItemNum;
		pSUsingItem->ItemUID				= m_vectUsingGeneralItem[i]->UniqueNumber;
		pSUsingItem->ItemInsertionType		= IUT_LOADING;
		// 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 아래와 같이 수정 함.
		//pSUsingItem->nRemainSecond			= m_vectUsingGeneralItem[i]->ItemInfo->Time/1000 - m_vectUsingGeneralItem[i]->UsingTimeStamp;
		// 2013-01-02 by jhseol, 캐나다 아레나 서버 크래쉬 디버깅 로그 추후 삭제 예정
#if defined(S_CAN_SERVER_SETTING_HSSON) || defined(S_DA_SERVER_SETTING_HSSON) || defined(S_ETRS_SERVER_SETTING_HSSON)
		BOOL IsArenaServerCheck = g_pFieldGlobal->IsArenaServer();
		UID32_t	tempCUID = 0;
		if( IsArenaServerCheck )
		{
			if( NULL == m_pFieldIOCPSocket )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E1] CUID(%8d) :: m_pFieldIOCPSocket Pointer is NULL \r\n", tempCUID);
				if( NULL != this )
				{
					if( NULL != this->m_pCharacter )
					{
						tempCUID = this->m_pCharacter->CharacterUniqueNumber;
						g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E2] CUID(%8d) :: CUID is this Pointer Find \r\n", tempCUID);
					}
				}
				if( 0 == tempCUID )
				{
					tempCUID = GetTickCount();
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E3] CUID(%8d) :: CUID is GetTickCount \r\n", tempCUID);
				}
			}
			else
			{
				tempCUID = m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
			}
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_01] CUID(%8d) :: VectorSize(%d), ForloopIndex(%d) \r\n", tempCUID, m_vectUsingGeneralItem.size(), i);
			if( NULL == this )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E4] CUID(%8d) :: This Pointer is NULL \r\n", tempCUID);
			}
			if( NULL == m_vectUsingGeneralItem[i]->ItemInfo )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_E5] CUID(%8d) :: ForloopIndex ItemInfo Pointer is NULL \r\n", tempCUID);
			}
			if( i < m_vectUsingGeneralItem.size() )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_02] CUID(%8d) :: ItemNum(%d), ForloopIndex Item UsingTimeStamp(%d) \r\n"
					, tempCUID, m_vectUsingGeneralItem[i]->ItemNum, m_vectUsingGeneralItem[i]->UsingTimeStamp);
			}
		}
		pSUsingItem->nRemainSecond			= CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(m_vectUsingGeneralItem[i]->ItemInfo) - m_vectUsingGeneralItem[i]->UsingTimeStamp;
		if( IsArenaServerCheck )
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[DEBUG_SEOL_03] CUID(%8d) :: Pass to Result ItemNum(%d), pSUsingItem->nRemainSecond(%d) \r\n", tempCUID, pSUsingItem->nRemainSecond);
		}
#else
		pSUsingItem->nRemainSecond			= CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(m_vectUsingGeneralItem[i]->ItemInfo) - m_vectUsingGeneralItem[i]->UsingTimeStamp;
#endif	// #ifdef S_CAN_SERVER_SETTING_HSSON || S_DA_SERVER_SETTING_HSSON || S_ETRS_SERVER_SETTING_HSSON
		// end 2013-01-02 by jhseol, 캐나다 아레나 서버 크래쉬 디버깅 로그 추후 삭제 예정
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] T_FC_STORE_INSERT_USINGITEM nRemainSecond = %d, Min = %d, S = %d\r\n"
//					, pSUsingItem->nRemainSecond, m_vectUsingGeneralItem[i]->UsingStartTime.Minute, m_vectUsingGeneralItem[i]->UsingStartTime.Second);		
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_INSERT_USINGITEM));
	}

	///////////////////////////////////////////////////////////////////////////////
	// 유효 시간 체크
	this->CheckExpireUsingTimeLimitedItemList();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::CheckExpireUsingTimeLimitedItemList(UID64_t i_itemUID/*=INVALID_UID64*/)
/// \brief		모든 사용중 시간제한 아이템의 남은시간을 체크하고 사용시간이 경과된 아이템을 삭제 처리하고 클라이언트로 정보를 전송한다.
///				// 2006-10-11 by cmkwon, 인자 추가(UID64_t i_itemUID/*=INVALID_UID64*/)
///				// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, 필요 없는 주석 제거 및 수정
/// \author		cmkwon
/// \date		2005-XX-XX ~ 2005-XX-XX
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::CheckExpireUsingTimeLimitedItemList(UID64_t i_itemUID/*=INVALID_UID64*/)
{
	// 2008-03-05 by dhjin, 아레나 통합 - 아레나 경기 중에는 삭제하지 않는다.
	if(CS_ARENASERVER_PLAYING == m_pFieldIOCPSocket->GetClientState())		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - CS_ARENA_PLAYING => CS_ARENASERVER_PLAYING 변경
	{
		return;
	}

	// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 사용류 아이템 만료시 파람 처리
	BOOL bCallResetAllSkillAndEnchant = FALSE;

	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	while (itr != m_vectUsingGeneralItem.end())
	{
		ITEM_GENERAL *pItemGen = *itr;
		if(IS_VALID_UID64(i_itemUID)
			&& pItemGen->UniqueNumber != i_itemUID)
		{// 2006-10-11 by cmkwon, 하나의 아이템만 체크한다
			itr++;
			continue;
		}

		int nCurUsingSec	= pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);
		int temp1 = pItemGen->UsingTimeStamp;
		int temp2 = atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);

		// 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
		//if(CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemGen->ItemInfo) <= nCurUsingSec)
		if(CAtumSJ::GetTimeSecondByItemKind8ItemAttribute(pItemGen->ItemInfo) <= nCurUsingSec && pItemGen->ItemInfo->Time != NULL)
		// end 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현
		
		{
			// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, 장착으로 체크 			
			if(WEAR_ATTACHED == pItemGen->Wear)
			{
				MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
				util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
				tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
				tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
				tmItemChangePos.ToItemUniqueNumber		= 0;
				tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
				m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);	// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldIOCPSocket::ProcessItemChangeWindowPosition# 수정

				// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
				// 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#
				CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);
				this->DeleteItemFromCharacterByPointer(pItemGen, IUT_EXPIRE_CARD_ITEM);
			}
			else
			{
				// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
				// 2009-11-17 by cmkwon, 시작제한 관련 아이템 삭제시 로그 정보 추가 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#
				CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);

				///////////////////////////////////////////////////////////////////////////////
				// 1. 아이템 기능 해제
				m_pFieldIOCPSocket->ReleaseCardItem(pItemGen->ItemInfo);

				///////////////////////////////////////////////////////////////////////////////
				// 2. 클라이언트로 전송
				INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_USINGITEM, T_FC_STORE_DELETE_USINGITEM, pSDUsingItem, SendBuf);
				pSDUsingItem->ItemNum			= pItemGen->ItemNum;
				pSDUsingItem->ItemDeletionType	= IUT_EXPIRE_CARD_ITEM;
				m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_STORE_DELETE_USINGITEM));

				///////////////////////////////////////////////////////////////////////////////
				// 3. DB에서 아이템 삭제			
				QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
				pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				
				this->ItemDeleteByInfinityPlaying(pItemGen);		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 진행 중 아이템 삭제

				///////////////////////////////////////////////////////////////////////////////
				// 4. ITEM_GENERAL의 메모리 해제
				util::del(pItemGen);

				// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 사용류 아이템 만료시 파람 처리
				if(FALSE == bCallResetAllSkillAndEnchant)
				{
					bCallResetAllSkillAndEnchant = TRUE;
				}
			}
			itr = m_vectUsingGeneralItem.erase(itr);

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, 복귀유져 버프추가
			if( NULL != m_pFieldIOCPSocket )		//	2013-07-07 by bckim, 서버 데드락 보완 
			{
			CFieldParty *pFParty = m_pFieldIOCPSocket->m_pFieldParty;
				if( NULL != pFParty )				//	2013-07-07 by bckim, 서버 데드락 보완 
			{
				pFParty->SetReturnUserBuffSkill(m_pFieldIOCPSocket); // 아이템 기간만료시 이미 아이템 해제되고 DB 삭제 되었음
				}			
			}			
#endif
			
			if(IS_VALID_UID64(i_itemUID))
			{// 2006-10-11 by cmkwon, 아이템 하나만 체크
				return;
			}
			
			continue;
		}
		itr++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-17 by cmkwon, 인피1차 발동류 아이템 관련 버그 수정 - 사용류 아이템 만료시 파람 처리
	if(bCallResetAllSkillAndEnchant)
	{
		m_pFieldIOCPSocket->ResetAllSkillAndEnchant();
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon, 아이템 생성 후 자동 삭제 아이템 만료 체크
	mtmapUID2ItemGeneral::iterator itrG(m_mapItemGeneral.begin());
	while(itrG != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemGen = itrG->second;		
		if(IS_VALID_UID64(i_itemUID)
			&& pItemGen->UniqueNumber != i_itemUID)
		{// 2006-10-11 by cmkwon, 하나의 아이템만 체크한다

			itrG++;
			continue;
		}

		if(COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			ATUM_DATE_TIME expireATime;
			expireATime			= pItemGen->CreatedTime;
			// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, 
			//expireATime.AddDateTime(0, 0, 0, pItemGen->ItemInfo->Endurance, 0, 0);
			expireATime.AddDateTime(0, 0, 0, 0, CAtumSJ::GetTimeMinuteByItemKind8ItemAttribute(pItemGen->ItemInfo, TIME_TERM_DELETE_ITEM));	// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#,

			if(atimeCur > expireATime)
			{// 2006-09-29 by cmkwon, 기간 만료, 삭제해야함

				// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldItemManager::CheckExpireUsingTimeLimitedItemList#, 아래와 같이 처리
				if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo))
				{// 2009-10-22 by cmkwon, 장착 가능 아이템류
					if(WEAR_ATTACHED == pItemGen->Wear)
					{
						///////////////////////////////////////////////////////////////////////////////
						// 2007-09-07 by cmkwon, 기간 만료된 장착된 아이템 장착 해제 처리 - 마크 아이템을 기간 제한 아이템에 추가함
						MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
						util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
						tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
						tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
						tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
						tmItemChangePos.ToItemUniqueNumber		= 0;
						tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
						m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);	// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CFieldIOCPSocket::ProcessItemChangeWindowPosition# 수정
					}
				}

				// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 아래와 같이 CAtumLogSender::SendLogMessageITEMDelete() 로 대체 함
				// 2012-08-30 by jhseol, 풀로그 남기기
#ifdef S_FULL_LOG_JHSEOL
				CAtumLogSender::SendLogMessageITEMUseITEM(m_pFieldIOCPSocket, pItemGen);						// 사용 아이템 로그 추가
#endif	//#ifdef S_FULL_LOG_JHSEOL
				// end 2012-08-30 by jhseol, 풀로그 남기기
				// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 기간 만료로 삭제함
				CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME);

				// 1. m_mapItemGeneral에서 제거
				itrG = m_mapItemGeneral.erase(itrG);

				// 2. DB에서 아이템 삭제
				QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
				pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

				// 3. 클라이언트로 전송
				INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
				msgDelete->ItemUniqueNumber		= pItemGen->UniqueNumber;
				msgDelete->ItemDeletionType		= IUT_EXPIRE_ITEM;
				m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));

				util::del(pItemGen);

				if(IS_VALID_UID64(i_itemUID))
				{// 2006-10-11 by cmkwon, 아이템 하나만 체크
					return;
				}
				
				continue;
			}
		}
		itrG++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::StoreTimeStampUsingTimeLimitedIteList(void)
/// \brief		사용중인 모든 사용시간제한 아이템을 사용된 시간을 계산하여 DB에 저장한다
/// \author		cmkwon
/// \date		2005-XX-XX ~ 2005-XX-XX
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::StoreTimeStampUsingTimeLimitedIteList(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
	{
		ITEM_GENERAL *pItemGen		= m_vectUsingGeneralItem[i];
		pItemGen->UsingTimeStamp	= pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);
		pItemGen->UsingStartTime	= atimeCur;
		
		////////////////////////////////////////////////////////////////////////////////
		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 밑과 같이 수정, 인피 진행 중 아이템 수정
// 		QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
// 		pQParam->ItemUniqueNumber	= pItemGen->UniqueNumber;
// 		pQParam->UsingTimeStamp1	= pItemGen->UsingTimeStamp;
// 		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		if(FALSE == g_pFieldGlobal->IsArenaServer()) {
			QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemGen->UniqueNumber;
			pQParam->UsingTimeStamp1	= pItemGen->UsingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}
		else {
//			if(pItemGen->ItemInfo->Kind == ITEMKIND_CARD && COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE|ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED)) {
			if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo)) {
				this->ItemUpdateByInfinityPlaying(pItemGen);
			}
			else {

				// 인피니티 전용 사용 아이템 관련 삭제 처리. 2010. 08. 09. by hsLee. - 기존 처리에서 사용한 아이템만 저장 안하도록 수정.
				if ( ITEM_NOT_USING != pItemGen->UsingTimeStamp
					&& COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_ONLY_USE_INFINITY ) ) 
				{
					// 2009-09-09 ~ 2010-01-28 by dhjin, 인피니티 - 인피에서만 사용 가능한 아이템 본섭에 돌아올때 삭제
					continue;
				}

				// 사용시 바로 삭제되는 시간제 아이템
				CIOCPSocket	*	MFSSock;
				MFSSock = this->ms_pFieldIOCP2->GetMFSSockForArenaServer();
				if(NULL == MFSSock) {// 2007-12-28 by dhjin, 필드 서버에 대한 정보가 없으면 오류
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR][Infinity][%I64d] MFS NULL 'StoreTimeStampUsingTimeLimitedIteList()', CharacterName = %s \r\n", m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, m_pFieldIOCPSocket->m_character.CharacterName);
					break;
				}
				INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, T_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, pUpdateItemSendMsg, UpdateItemSendBuf);
				pUpdateItemSendMsg->AccountUID			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
				pUpdateItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
				pUpdateItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
				pUpdateItemSendMsg->CurrentCount		= pItemGen->CurrentCount;
				pUpdateItemSendMsg->ItemWindowIndex		= pItemGen->ItemWindowIndex;
				pUpdateItemSendMsg->MainSvrItemUID		= pItemGen->MainSvrItemUID;
				pUpdateItemSendMsg->Wear				= pItemGen->Wear;
				pUpdateItemSendMsg->UsingTimeStamp		= pItemGen->UsingTimeStamp;
				pUpdateItemSendMsg->CreatedTime			= pItemGen->CreatedTime;
				pUpdateItemSendMsg->ItemNum				= pItemGen->ItemNum;
				pUpdateItemSendMsg->CoolingTimeStamp	= pItemGen->CoolingTimeStamp;		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
				MFSSock->SendAddData(UpdateItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM));
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ITEM][Infinity][%I64d] MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM, CharacterName = %s, ItemNum = %d, CurrentCnt = %d, Wear = %d, UsingTimeStamp = %d \r\n"
					, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, m_pFieldIOCPSocket->m_character.CharacterName, pItemGen->ItemInfo->ItemNum
					, pItemGen->CurrentCount, pItemGen->Wear, pItemGen->UsingTimeStamp);
			} // if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo)) {} else {
		} // if(FALSE == g_pFieldGlobal->IsArenaServer()) {} else {
	} // for(int i=0; i < m_vectUsingGeneralItem.size(); i++)
}

SDelayStoreItemInfo *CFieldItemManager::FindDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID)
{
	for(int i=0; i < m_vectDelayStoredItemInfo.size(); i++)
	{
		if(i_n64ItemUID == m_vectDelayStoredItemInfo[i].n64ItemUID)
		{
			return &m_vectDelayStoredItemInfo[i];
		}
	}
	
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		DB에 저장된 카운트를 저장해 둔다
/// \author		cmkwon
/// \date		2006-06-01 ~ 2006-06-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::SetDelayStoredItemCounts(ITEM_GENERAL *i_pItemG)
{
	if(FALSE == CFieldItemManager::IsDelayStoreItem(i_pItemG->ItemInfo))
	{
		return FALSE;
	}

	SDelayStoreItemInfo *pSItemInfo = FindDelayStoredItemInfoByItemUID(i_pItemG->UniqueNumber);
	if(pSItemInfo)
	{
		pSItemInfo->nStoredItemCounts		= i_pItemG->CurrentCount;
		return TRUE;
	}
	
	m_vectDelayStoredItemInfo.push_back(SDelayStoreItemInfo(i_pItemG->UniqueNumber, i_pItemG->CurrentCount));
	return FALSE;
}

BOOL CFieldItemManager::DeleteDelayStoredItemInfoByItemUID(UID64_t i_n64ItemUID)
{
	vectDelayStoreItemInfo::iterator itr(m_vectDelayStoredItemInfo.begin());

	for(; itr != m_vectDelayStoredItemInfo.end(); itr++)
	{
		if(i_n64ItemUID == itr->n64ItemUID)
		{
			m_vectDelayStoredItemInfo.erase(itr);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CFieldItemManager::StoreCountableItem(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	
	mtmapUID2ItemGeneral::iterator itr(m_mapItemGeneral.begin());
	for(; itr != m_mapItemGeneral.end(); itr++)
	{
		ITEM_GENERAL *pItemGen = itr->second;		
		if(IsDelayStoreItem(pItemGen->ItemInfo))
		{
			BOOL bStore2DB = FALSE;
			SDelayStoreItemInfo *pStoredInfo = FindDelayStoredItemInfoByItemUID(pItemGen->UniqueNumber);
			
			if(pStoredInfo
				&& pStoredInfo->nStoredItemCounts != pItemGen->CurrentCount)
			{
				pStoredInfo->nStoredItemCounts	= pItemGen->CurrentCount;
				bStore2DB	= TRUE;
			}
			else if(NULL == pStoredInfo)
			{
				SetDelayStoredItemCounts(pItemGen);
				pStoredInfo	= FindDelayStoredItemInfoByItemUID(pItemGen->UniqueNumber);
				pStoredInfo->nStoredItemCounts	= pItemGen->CurrentCount;
				bStore2DB	= TRUE;
			}
			
			if(bStore2DB)
			{
				////////////////////////////////////////////////////////////////////////////////
				// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 밑과 같이 수정, 인피 진행 중 아이템 수정
// 				QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
// 				pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
// 				pQParam->Count					= pItemGen->CurrentCount;
// 				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				if(FALSE == g_pFieldGlobal->IsArenaServer()) {
					QPARAM_UPDATEITEMCOUNT *pQParam	= new QPARAM_UPDATEITEMCOUNT;
					pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
					pQParam->Count					= pItemGen->CurrentCount;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCount, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
				}
				else {
					this->ItemUpdateByInfinityPlaying(pItemGen);
				}
			}
		}
	}

	return TRUE;
}

BOOL CFieldItemManager::IsDelayStoreItem(ITEM *i_pItem)
{
	if(IS_COUNTABLE_ITEM(i_pItem->Kind))
	{
		return FALSE;
	}
	
	if(MONEY_ITEM_NUMBER == i_pItem->ItemNum)
	{// 2006-05-21 by cmkwon, 돈도 지연 업데이트 아이템으로 처리한다.
		return TRUE;
	}

	if(ITEMKIND_ENERGY != i_pItem->Kind)
	{
		return FALSE;
	}

	// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
	//switch(i_pItem->DestParameter1)
	switch(i_pItem->ArrDestParameter[0])
	{
	case DES_GRADUAL_HP_UP:
	case DES_GRADUAL_DP_UP:
	case DES_GRADUAL_EP_UP:
	case DES_IN_TIME_HP_UP:
	case DES_IN_TIME_DP_UP:
	case DES_IN_TIME_EP_UP:
	case DES_IMMEDIATE_HP_UP:
	case DES_IMMEDIATE_DP_UP:
	case DES_IMMEDIATE_EP_UP:
		{
		}
		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::IsExistPremiumCard(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-30 ~ 2006-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::IsExistPremiumCard(void)
{
	if(CASH_PREMIUM_CARD_STATE_NORMAL != m_cashPremiumCardInfo.enCardState
		|| 0 == m_cashPremiumCardInfo.n64UniqueNumber10
		|| m_pFieldIOCPSocket->m_character.AccountUniqueNumber != m_cashPremiumCardInfo.nAccountUID10)
	{
		return FALSE;
	}

	return TRUE;
}

void CFieldItemManager::CashInitPremiumCardInfo(SCASH_PREMEIUM_CARD_INFO *i_pCardInfo)
{
	mt_auto_lock at(&m_mapItemGeneral);
	if(i_pCardInfo->nAccountUID10 != m_pFieldIOCPSocket->m_character.AccountUniqueNumber)
	{
		return;
	}

	m_cashPremiumCardInfo				= *i_pCardInfo;
	m_cashPremiumCardInfo.enCardState	= CASH_PREMIUM_CARD_STATE_NORMAL;
	m_cashPremiumCardInfo.ResetAllPlusRateByPremiumCard();		// 2006-10-11 by cmkwon
}

Err_t CFieldItemManager::CashInsertPremiumCard(ITEM *i_pItem, BOOL i_bOnlyCheck/*=FALSE*/)
{
	if(NULL == i_pItem){			return ERR_PROTOCOL_NO_SUCH_ITEM_INFO;}		// 2006-04-25 by cmkwon
// 2008-10-20 by cmkwon, 멥버쉽 아이템 체크 ItemAttribute 로 처리 - 아래와 같이 수정 
//	if(CASH_ITEMKIND_PREMIUM_CARD != i_pItem->SummonMonster)
	if(FALSE == COMPARE_BIT_FLAG(i_pItem->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD))
	{
		return ERR_CASH_PREMIUM_CARD_INVALID_ITEMNUM;
	}

	mt_auto_lock at(&m_mapItemGeneral);
	if(FALSE == m_pFieldIOCPSocket->IsUsing())
	{
		return ERR_COMMON_SOCKET_CLOSED;
	}
	else if(CASH_PREMIUM_CARD_STATE_NOT_EXIST != m_cashPremiumCardInfo.enCardState
		&& CASH_PREMIUM_CARD_STATE_NORMAL != m_cashPremiumCardInfo.enCardState)
	{
		return ERR_CASH_PREMIUM_CARD_ALREADY_USING;
	}

	if(CASH_PREMIUM_CARD_STATE_NOT_EXIST == m_cashPremiumCardInfo.enCardState)
	{// 사용중인 프리미엄카드가 없다

// 2006-07-05 by cmkwon, 프리미엄 카드 시스템을 MonthlyPay로 변경
// 		if(ITEM_NUM_CASH_UPGRADE_PREMIUM_CARD == i_pItem->ItemNum)
// 		{
// 			return ERR_CASH_PREMIUM_CARD_NOT_MATCHED;
// 		}

		if(FALSE == i_bOnlyCheck)
		{
			m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_INSERTING;		
			m_cashPremiumCardInfo.n64UniqueNumber10		= 0;
			m_cashPremiumCardInfo.nAccountUID10			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			m_cashPremiumCardInfo.nCardItemNum			= i_pItem->ItemNum;
			m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
			m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
			m_cashPremiumCardInfo.atumTimeCreatedTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;			
			m_cashPremiumCardInfo.atumTimeExpireTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
			m_cashPremiumCardInfo.atumTimeExpireTime.AddDateTime(0, 0, i_pItem->Time, 0, 0, 0);

			QPARAM_CASH_INSERT_PREMIUM_CARD *pQParam = new QPARAM_CASH_INSERT_PREMIUM_CARD;
			util::zero(pQParam, sizeof(QPARAM_CASH_INSERT_PREMIUM_CARD));

			pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
			pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
			pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
			pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashInsertPremiumCard, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}		
	}
	else
	{// 이미 카드가 존재한다 - 

		ATUM_DATE_TIME atimeMaxDate(true);

		// 2007-10-01 by cmkwon, 멤버쉬 구입 가능 최대 일수 - 각 서버별 최대 일수를 가져온다
		atimeMaxDate.AddDateTime(0, 0, CFieldIOCPSocket::GetMaxBuyableDayOfMembership());
		ATUM_DATE_TIME atimeExpireDate = m_cashPremiumCardInfo.atumTimeExpireTime;
		atimeExpireDate.AddDateTime(0, 0, i_pItem->Time, 0, 0, 0);

		if(atimeMaxDate < atimeExpireDate)
		{// 연장 가능 기간을 오바한다

			return ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE;
		}

		if(FALSE == i_bOnlyCheck)
		{
			m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_UPDATING;			
			m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
			m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
			m_cashPremiumCardInfo.atumTimeExpireTime.AddDateTime(0, 0, i_pItem->Time);

			QPARAM_CASH_UPDATE_PREMIUM_CARD *pQParam = new QPARAM_CASH_UPDATE_PREMIUM_CARD;
			util::zero(pQParam, sizeof(QPARAM_CASH_UPDATE_PREMIUM_CARD));

			pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
			pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
			pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
			pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashUpdatePremiumCard, m_pFieldIOCPSocket
				, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		}

// 2006-07-05 by cmkwon, 프리미엄 카드 시스템을 MonthlyPay로 변경하면서 위와 같이 수정되었습니다.
// 		if(m_cashPremiumCardInfo.nCardItemNum == i_pItem->ItemNum)
// 		{// 같은 종류의 카드를 연장한다
// 			
// 			ATUM_DATE_TIME tmAtumTime(true);
// 			tmAtumTime.AddDateTime(0, 0, i_pItem->Time*TERM_ENABLE_CASH_PREMIUM_CARD_BUY_MONTH, 0, 0, 0);
// 
// 			if(tmAtumTime <= m_cashPremiumCardInfo.atumTimeExpireTime)
// 			{// 연장 가능 기간을 오바한다
// 				
// 				return ERR_CASH_PREMIUM_CARD_NOT_YET_BUY_STATE;
// 			}
// 
// 			if(FALSE == i_bOnlyCheck)
// 			{
// 				m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_UPDATING;			
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
// 				m_cashPremiumCardInfo.atumTimeExpireTime.AddDateTime(0, 0, i_pItem->Time, 0, 0, 0);
// 
// 				QPARAM_CASH_UPDATE_PREMIUM_CARD *pQParam = new QPARAM_CASH_UPDATE_PREMIUM_CARD;
// 				util::zero(pQParam, sizeof(QPARAM_CASH_UPDATE_PREMIUM_CARD));
// 
// 				pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
// 				pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
// 				pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
// 				pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
// 				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashUpdatePremiumCard, m_pFieldIOCPSocket
// 					, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
// 			}
// 		}
// 		else
// 		{// 업그레이드 카드 구입
// 
// 			if(ITEM_NUM_CASH_NORMAL_PREMIUM_CARD != m_cashPremiumCardInfo.nCardItemNum
// 				|| ITEM_NUM_CASH_UPGRADE_PREMIUM_CARD != i_pItem->ItemNum)
// 			{
// 				return ERR_CASH_PREMIUM_CARD_NOT_MATCHED;
// 			}
// 
// 			if(FALSE == i_bOnlyCheck)
// 			{
// 				m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_UPDATING;
// 				m_cashPremiumCardInfo.nCardItemNum			= ITEM_NUM_CASH_SUPER_PREMIUM_CARD;
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.SetCurrentDateTime();
// 				m_cashPremiumCardInfo.atumTimeUpdatedTime.Second = 0;
// 
// 				QPARAM_CASH_UPDATE_PREMIUM_CARD *pQParam = new QPARAM_CASH_UPDATE_PREMIUM_CARD;
// 				util::zero(pQParam, sizeof(QPARAM_CASH_UPDATE_PREMIUM_CARD));
// 
// 				pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
// 				pQParam->nItemNum				= m_cashPremiumCardInfo.nCardItemNum;
// 				pQParam->atumTimeCurrentTime	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
// 				pQParam->atumTimeExpireTime		= m_cashPremiumCardInfo.atumTimeExpireTime;
// 				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashUpdatePremiumCard, m_pFieldIOCPSocket
// 					, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
// 			}
// 		}
	}

	if(FALSE == i_bOnlyCheck)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 보너스 아이템 지급
		this->CashInsertBonusItemPremiumCard(i_pItem);

		///////////////////////////////////////////////////////////////////////////////
		// 프리미엄 아이템 정보 전송
		INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, pSCardInfo, SendBuf);
		this->Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(pSCardInfo);
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO));	
		INIT_MSG(MSG_FI_CASH_PREMIUM_CARD_INFO, T_FI_CASH_PREMIUM_CARD_INFO, pIMSCardInfo, SendBuf);// 2006-09-14 by dhjin, IM서버에 맴버쉽 기간 끝난것 알리기
		this->Make_MSG_FI_CASH_PREMIUM_CARD_INFO(pIMSCardInfo);
		m_pFieldIOCPSocket->ms_pFieldIOCP->Send2IMServer(SendBuf, MSG_SIZE(MSG_FI_CASH_PREMIUM_CARD_INFO));
	}
	return ERR_NO_ERROR;
}


BOOL CFieldItemManager::CashOnEventPremiumCardFromDB(int i_queryType, UID32_t i_AccUID
													 , UID64_t i_n64CardUID, Err_t i_errNum/*=0*/)
{
	if(m_pFieldIOCPSocket->GetCharacter()->AccountUniqueNumber != i_AccUID)
	{
		return FALSE;
	}

	mt_auto_lock at(&m_mapItemGeneral);
	
	if(ERR_NO_ERROR == i_errNum)
	{// 에러 없음

		switch(i_queryType)
		{
		case QT_CashInsertPremiumCard:
		case QT_CashUpdatePremiumCard:
			{
				m_cashPremiumCardInfo.enCardState		= CASH_PREMIUM_CARD_STATE_NORMAL;
				m_cashPremiumCardInfo.n64UniqueNumber10	= i_n64CardUID;
#ifdef NEMERIAN_UPGRADED_MEMBERSHIP
				m_pFieldIOCPSocket->ChangeHP(m_pFieldIOCPSocket->GetCharacterTotalHP());
				m_pFieldIOCPSocket->ChangeDP(m_pFieldIOCPSocket->GetCharacterTotalDP());
				m_pFieldIOCPSocket->ChangeEP(m_pFieldIOCPSocket->GetCharacterTotalEP());
#endif
			}
			break;
		case QT_CashDeletePremiumCard:
			{
				m_cashPremiumCardInfo.ResetCASH_PREMEIUM_CARD_INFO();
#ifdef NEMERIAN_UPGRADED_MEMBERSHIP
				m_pFieldIOCPSocket->ChangeHP(m_pFieldIOCPSocket->GetCharacterTotalHP());
				m_pFieldIOCPSocket->ChangeDP(m_pFieldIOCPSocket->GetCharacterTotalDP());
				m_pFieldIOCPSocket->ChangeEP(m_pFieldIOCPSocket->GetCharacterTotalEP());
#endif
			}
			break;		
		}	
		
		m_cashPremiumCardInfo.ResetAllPlusRateByPremiumCard();			// 2006-10-11 by cmkwon
		return TRUE;
	}
	
	return TRUE;
}


BOOL CFieldItemManager::CashCheckPremiumCardExpire(BOOL i_bUnconditionalDeleteFlag/*=FALSE*/)
{
	mt_auto_lock at(&m_mapItemGeneral);
	if(CASH_PREMIUM_CARD_STATE_NORMAL != m_cashPremiumCardInfo.enCardState)
	{// 유료화 프리미엄 카드 없거나 다른 처리중

		return FALSE;
	}
	
	if(FALSE == i_bUnconditionalDeleteFlag)
	{
		ATUM_DATE_TIME	tmAtumCurTime(true);
		if(tmAtumCurTime < m_cashPremiumCardInfo.atumTimeExpireTime)
		{// 사용 기간이 남은 상태임

			return FALSE;
		}
	}
	
	m_cashPremiumCardInfo.enCardState			= CASH_PREMIUM_CARD_STATE_DELETING;
	m_cashPremiumCardInfo.nCardItemNum			= 0;
	
	///////////////////////////////////////////////////////////////////////////////
	// DB에서 삭제
	QPARAM_CASH_DELETE_PREMIUM_CARD *pQParam = new QPARAM_CASH_DELETE_PREMIUM_CARD;
	util::zero(pQParam, sizeof(QPARAM_CASH_DELETE_PREMIUM_CARD));
	pQParam->nAccountUID			= m_cashPremiumCardInfo.nAccountUID10;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_CashDeletePremiumCard, m_pFieldIOCPSocket
		, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

	///////////////////////////////////////////////////////////////////////////////
	// 클라이언트로 정보 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, T_FC_CHARACTER_CASH_PREMIUM_CARD_INFO, pSCardInfo, SendBuf);
	this->Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(pSCardInfo);
	m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO));
	
	INIT_MSG(MSG_FI_CASH_PREMIUM_CARD_INFO, T_FI_CASH_PREMIUM_CARD_INFO, pIMSCardInfo, SendBuf);// 2006-09-14 by dhjin, IM서버에 맴버쉽 기간 끝난것 알리기
	this->Make_MSG_FI_CASH_PREMIUM_CARD_INFO(pIMSCardInfo);
	m_pFieldIOCPSocket->ms_pFieldIOCP->Send2IMServer(SendBuf, MSG_SIZE(MSG_FI_CASH_PREMIUM_CARD_INFO));
	return TRUE;
}

void CFieldItemManager::CashInsertBonusItemPremiumCard(ITEM *i_pItemInfo)
{
	mt_auto_lock at(&m_mapItemGeneral);
	
	for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		if(0.0f < i_pItemInfo->ArrParameterValue[i]
			&& 0 < i_pItemInfo->ArrDestParameter[i])
		{
			ITEM *pLinkItemInfo = ms_pFieldIOCP2->GetItemInfo(i_pItemInfo->ArrParameterValue[i]);
			if (pLinkItemInfo)
			{
				this->InsertItemBaseByItemNum(pLinkItemInfo->ItemNum, i_pItemInfo->ArrDestParameter[i], IUT_SHOP);
			}
		}	
	}
}


void CFieldItemManager::Make_MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO *o_pCardInfo)
{
	o_pCardInfo->nCardItemNum1			= m_cashPremiumCardInfo.nCardItemNum;	
	o_pCardInfo->atumTimeUpdatedTime1	= m_cashPremiumCardInfo.atumTimeUpdatedTime;
	o_pCardInfo->atumTimeExpireTime1	= m_cashPremiumCardInfo.atumTimeExpireTime;
	o_pCardInfo->fExpRate1				= m_cashPremiumCardInfo.fExpRate;
// 2005-03-23 by cmkwon, 프로토콜로는 아직 구현되지 않은 것(필요시 추가)
//	o_pCardInfo->fSPIRate1				= m_cashPremiumCardInfo.fSPIRate;
	o_pCardInfo->fDropRate1				= m_cashPremiumCardInfo.fDropRate;
	o_pCardInfo->fDropRareRate1			= m_cashPremiumCardInfo.fDropRareRate;
	o_pCardInfo->fExpRepairRate1		= m_cashPremiumCardInfo.fExpRepairRate;
}

void CFieldItemManager::Make_MSG_FI_CASH_PREMIUM_CARD_INFO(MSG_FI_CASH_PREMIUM_CARD_INFO *o_pCardInfo)
{
	o_pCardInfo->AccountUID				= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
	o_pCardInfo->nCardItemNum1			= m_cashPremiumCardInfo.nCardItemNum;	
	o_pCardInfo->atumTimeExpireTime1	= m_cashPremiumCardInfo.atumTimeExpireTime;	// 2008-06-20 by dhjin, EP3 여단 수정 사항 - 만료 시간 필요
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::AllDetachTimeLimitItem()
/// \brief		아레나 통합 - 장착중인 시간제한 아이템을 벗는다.
/// \author		dhjin
/// \date		2008-03-03 ~ 2008-03-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::AllDetachTimeLimitItem()
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	while (itr != m_vectUsingGeneralItem.end())
	{
		ITEM_GENERAL *pUsingItemG = *itr;

		if(ITEMKIND_ACCESSORY_TIMELIMIT == pUsingItemG->ItemInfo->Kind
			&& WEAR_ATTACHED == pUsingItemG->Wear)
		{
			int nTmpWindowIndex		= pUsingItemG->ItemWindowIndex;	// 기존 위치

			// 2008-03-03 by dhjin, 장착 해제 처리함
			m_pFieldIOCPSocket->UpdateWearItemPointer(pUsingItemG->UniqueNumber, pUsingItemG->UniqueNumber, pUsingItemG->ItemWindowIndex, POS_ITEMWINDOW_OFFSET + nTmpWindowIndex);
			
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 아래와 같이 수정
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter1, pUsingItemG->ItemInfo->ParameterValue1);
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter2, pUsingItemG->ItemInfo->ParameterValue2);
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter3, pUsingItemG->ItemInfo->ParameterValue3);
// 			m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->DestParameter4, pUsingItemG->ItemInfo->ParameterValue4);
			for(int i=0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
			{
				m_pFieldIOCPSocket->UnsetParamFactor(pUsingItemG->ItemInfo->ArrDestParameter[i], pUsingItemG->ItemInfo->ArrParameterValue[i]);
			}

			ATUM_DATE_TIME atimeCur { true };
			pUsingItemG->UsingTimeStamp	= pUsingItemG->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pUsingItemG->UsingStartTime);
			pUsingItemG->UsingStartTime	= atimeCur;
			
			QPARAM_UPDATE_ITEM_USINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_USINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pUsingItemG->UniqueNumber;
			pQParam->UsingTimeStamp1	= pUsingItemG->UsingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemUsingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 현재는 시간제한악세사리 아이템만 로그에 남긴다
			CAtumLogSender::SendLogMessageITEMDetachItem(m_pFieldIOCPSocket, pUsingItemG);

			pUsingItemG->Wear				= WEAR_NOT_ATTACHED;
			pUsingItemG->ItemWindowIndex	= POS_ITEMWINDOW_OFFSET + nTmpWindowIndex;
	
			QPARAM_CHANGE_ITEM_WINDOW_POSITION *pQParamChangeWindowPosition	= new QPARAM_CHANGE_ITEM_WINDOW_POSITION;
			pQParamChangeWindowPosition->AccountUID			= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParamChangeWindowPosition->CharacterUID		= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
			pQParamChangeWindowPosition->ItemUID1			= pUsingItemG->UniqueNumber;
			pQParamChangeWindowPosition->ItemWear1			= pUsingItemG->Wear;
			pQParamChangeWindowPosition->ItemWindowIndex1	= pUsingItemG->ItemWindowIndex;
			pQParamChangeWindowPosition->ItemUID2			= 0;
			pQParamChangeWindowPosition->ItemWindowIndex2	= POS_INVALID_POSITION;
			pQParamChangeWindowPosition->ItemWear2			= WEAR_NOT_ATTACHED;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_ChangeItemPosition, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamChangeWindowPosition);

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ARENACHECK] AllDetachTimeLimitItem ItemUniqueNumber = %d\r\n"
//				, pUsingItemG->UniqueNumber);			
			itr = m_vectUsingGeneralItem.erase(itr);
			continue;
		}
		
		itr++;		
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ItemInsertByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
/// \brief		인피니티 - 인피 진행 중 아이템 습득
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ItemInsertByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
{
	if(FALSE == g_pFieldGlobal->IsArenaServer()
		|| NULL == i_pItemGeneral) {
		return;
	}

	mt_auto_lock mtA(&m_mtInfinityInsertItemInfoList);

	INFINITY_INSERT_ITEM_INFO InsertItemInfo;
	util::zero(&InsertItemInfo, sizeof(INFINITY_INSERT_ITEM_INFO));
	InsertItemInfo.ItemUID				= i_pItemGeneral->UniqueNumber;
	InsertItemInfo.ItemNum				= i_pItemGeneral->ItemNum;
	InsertItemInfo.CurrentCount			= i_pItemGeneral->CurrentCount;
	InsertItemInfo.ItemWindowIndex		= i_pItemGeneral->ItemWindowIndex;
	InsertItemInfo.PrefixCodeNum		= i_pItemGeneral->PrefixCodeNum;
	InsertItemInfo.SuffixCodeNum		= i_pItemGeneral->SuffixCodeNum;
	InsertItemInfo.Wear					= i_pItemGeneral->Wear;
	InsertItemInfo.UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
	InsertItemInfo.CreatedTime			= i_pItemGeneral->CreatedTime;
	InsertItemInfo.CoolingTimeStamp		= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

	m_mtInfinityInsertItemInfoList.push_back(InsertItemInfo);

//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemInsertByInfinityPlaying# Inserted Item!, %s %s \r\n"
//		, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ItemUpdateByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
/// \brief		인피니티 - 인피 진행 중 아이템 수정
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ItemUpdateByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral) {
	if(FALSE == g_pFieldGlobal->IsArenaServer()
		|| NULL == i_pItemGeneral)
	{
		return;
	}

	{
		// 인피 진행 중 습득한 물품에 있는지 체크하고 있다면 습득한 물품 정보 수정, 인피껀 인피가 알아서 처리
		mt_auto_lock mta(&m_mtInfinityInsertItemInfoList);
		INFINITY_INSERT_ITEM_INFO *pInsertItemInfo = this->InfinityFindItemInfoFromInsertItemListNoLock(i_pItemGeneral->UniqueNumber);
		if(pInsertItemInfo)
		{
			pInsertItemInfo->CurrentCount		= i_pItemGeneral->CurrentCount;
			pInsertItemInfo->ItemWindowIndex	= i_pItemGeneral->ItemWindowIndex;
			pInsertItemInfo->PrefixCodeNum		= i_pItemGeneral->PrefixCodeNum;
			pInsertItemInfo->SuffixCodeNum		= i_pItemGeneral->SuffixCodeNum;
			pInsertItemInfo->Wear				= i_pItemGeneral->Wear;
			pInsertItemInfo->UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
			pInsertItemInfo->CoolingTimeStamp	= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
			
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemUpdateByInfinityPlaying# Update Item from InsertList, %s %s, Wear = %d, UsingTimeStamp(%d) CoolingTimeStamp(%d) MFS(%I64d) \r\n"
//				, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), i_pItemGeneral->Wear, i_pItemGeneral->UsingTimeStamp, i_pItemGeneral->CoolingTimeStamp, i_pItemGeneral->MainSvrItemUID);
			return;
		}
	}

	{
		mt_auto_lock mtu(&m_mtInfinityUpdateItemInfoList);
		INFINITY_UPDATE_ITEM_INFO *pUpdateItemInfo = this->InfinityFindItemInfoFromUpdateItemListNoLock(i_pItemGeneral->UniqueNumber);
		if(pUpdateItemInfo)
		{
			pUpdateItemInfo->CurrentCount		= i_pItemGeneral->CurrentCount;
			pUpdateItemInfo->ItemWindowIndex	= i_pItemGeneral->ItemWindowIndex;
			pUpdateItemInfo->Wear				= i_pItemGeneral->Wear;
			pUpdateItemInfo->UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
			pUpdateItemInfo->CoolingTimeStamp	= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템

//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemUpdateByInfinityPlaying# Update Item from UpdateList, %s %s, Wear = %d, UsingTimeStamp(%d) CoolingTimeStamp(%d) MFS(%I64d) \r\n"
//				, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), i_pItemGeneral->Wear, i_pItemGeneral->UsingTimeStamp, i_pItemGeneral->CoolingTimeStamp, i_pItemGeneral->MainSvrItemUID);
			return;
		}

		INFINITY_UPDATE_ITEM_INFO UpdateItemInfo;
		util::zero(&UpdateItemInfo, sizeof(INFINITY_UPDATE_ITEM_INFO));
		UpdateItemInfo.ItemUID				= i_pItemGeneral->UniqueNumber;
		UpdateItemInfo.CurrentCount			= i_pItemGeneral->CurrentCount;
		UpdateItemInfo.ItemWindowIndex		= i_pItemGeneral->ItemWindowIndex;
		UpdateItemInfo.Wear					= i_pItemGeneral->Wear;
		UpdateItemInfo.MainSvrItemUID		= i_pItemGeneral->MainSvrItemUID;
		UpdateItemInfo.UsingTimeStamp		= i_pItemGeneral->UsingTimeStamp;
		UpdateItemInfo.CoolingTimeStamp		= i_pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
		m_mtInfinityUpdateItemInfoList.push_back(UpdateItemInfo);

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemUpdateByInfinityPlaying# Insert Item to UpdateList, %s %s, Wear = %d, UsingTimeStamp(%d) CoolingTimeStamp(%d) MFS(%I64d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), i_pItemGeneral->Wear, i_pItemGeneral->UsingTimeStamp, i_pItemGeneral->CoolingTimeStamp, i_pItemGeneral->MainSvrItemUID);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ItemDeleteByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral)
/// \brief		인피니티 - 인피 진행 중 아이템 삭제
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ItemDeleteByInfinityPlaying(ITEM_GENERAL *i_pItemGeneral) {
	if(FALSE == g_pFieldGlobal->IsArenaServer()
		|| NULL == i_pItemGeneral) {
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
	if(this->InfinityDeleteItemInfoFromInsertItemList(i_pItemGeneral->UniqueNumber))
	{
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# deleted from InsertList !, %s %s \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
		return;
	}

	// 2010-07-16 by jskim 인피니티 사용한 아이템 원래대로 복원되는 버그 수정 
	/*
	if(this->InfinityDeleteItemInfoFromUpdateItemList(i_pItemGeneral->UniqueNumber))
	{		
//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# deleted from UpdateList !, %s %s \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
		return;
	}
	*/
	this->InfinityDeleteItemInfoFromUpdateItemList(i_pItemGeneral->UniqueNumber);

	if(0 == i_pItemGeneral->MainSvrItemUID)
	{
		// 2009-09-09 ~ 2010-01-27 by dhjin, 인피니티 - MainServerUID = 0 이면 본섭 아이템이 아니다. 이게 왜 위 항목에서 걸러지지 않는지 체크해야함!!!임시방편코드
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# %s %s MainSvrUID = 0 \r\n"
			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()));
		return;
	}

	BOOL bReted = this->InfinityInsertItemInfoFromDeleteItemList(i_pItemGeneral->MainSvrItemUID);
//	g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::ItemDeleteByInfinityPlaying# inserted ItemList to delete item On MFS !, %s %s bReted(%d) \r\n"
//		, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pFieldIOCPSocket->GetCharacter(), string()), GetItemGeneralString(i_pItemGeneral, string()), bReted);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::UpdateMainSvrUsingTimeLimitedItemByInfinity(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM *i_pItemInfo)
/// \brief		인피니티 - 인피에서 수정된 시간제 아이템 업뎃!
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateMainSvrUsingTimeLimitedItemByInfinity(MSG_FtoA_INFINITY_USING_TIME_LIMIT_ITEM *i_pItemInfo)	{
	if(NULL == i_pItemInfo) {
		return FALSE;
	}

	mt_auto_lock igLock(&m_mapItemGeneral);
	for(int i=0; i < m_vectUsingGeneralItem.size(); i++) {
		if(i_pItemInfo->MainSvrItemUID == m_vectUsingGeneralItem[i]->UniqueNumber) {
			// 존재하면 업뎃!
			m_vectUsingGeneralItem[i]->UsingTimeStamp	= i_pItemInfo->UsingTimeStamp;
			m_vectUsingGeneralItem[i]->Wear				= i_pItemInfo->Wear;
			return TRUE;
		}
	}

	return FALSE;
} 

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::DeleteUsingTimeLimitedItemByInfinity(UID64_t i_MainSvrItemUID)
/// \brief		인피니티 - 인피에서 삭제된 시간제 아이템 처리
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::DeleteUsingTimeLimitedItemByInfinity(UID64_t i_MainSvrItemUID) {

	mt_auto_lock igLock(&m_mapItemGeneral);

	ATUM_DATE_TIME atimeCur { true };
	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	while (itr != m_vectUsingGeneralItem.end())
	{
		ITEM_GENERAL *pItemGen = *itr;
		if(IS_VALID_UID64(i_MainSvrItemUID)
			&& pItemGen->UniqueNumber != i_MainSvrItemUID)
		{// 2006-10-11 by cmkwon, 하나의 아이템만 체크한다
			itr++;
			continue;
		}

		int nCurUsingSec	= pItemGen->UsingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);
		int temp1 = pItemGen->UsingTimeStamp;
		int temp2 = atimeCur.GetTimeDiffTimeInSeconds(pItemGen->UsingStartTime);

		// if(pItemGen->ItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		if(WEAR_ATTACHED == pItemGen->Wear)
		{
			MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
			util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
			tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
			tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
			tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
			tmItemChangePos.ToItemUniqueNumber		= 0;
			tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
			m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);

			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
			CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);

			this->DeleteItemFromCharacterByPointer(pItemGen, IUT_EXPIRE_CARD_ITEM);
		}
		else
		{
			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 
			CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME, nCurUsingSec);
			///////////////////////////////////////////////////////////////////////////////
			// 1. 아이템 기능 해제
			m_pFieldIOCPSocket->ReleaseCardItem(pItemGen->ItemInfo);

			///////////////////////////////////////////////////////////////////////////////
			// 3. DB에서 아이템 삭제			
			QPARAM_DELETESTOREITEM *pQParam	= new QPARAM_DELETESTOREITEM;
			pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
			
			///////////////////////////////////////////////////////////////////////////////
			// 4. ITEM_GENERAL의 메모리 해제
			util::del(pItemGen);
		}

		itr = m_vectUsingGeneralItem.erase(itr);
	
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, 밑과 같이 수정
//		if(IS_VALID_UID64(i_MainSvrItemUID))
//		{// 2006-10-11 by cmkwon, 아이템 하나만 체크
//			return TRUE;
//		}
// 2010-01-21 by cmkwon, 버그 삭제 되어야 함.
//		itr++;
		return TRUE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-09-29 by cmkwon, 아이템 생성 후 자동 삭제 아이템 만료 체크
	mtmapUID2ItemGeneral::iterator itrG(m_mapItemGeneral.begin());
	while(itrG != m_mapItemGeneral.end())
	{
		ITEM_GENERAL *pItemGen = itrG->second;		
		if(IS_VALID_UID64(i_MainSvrItemUID)
			&& pItemGen->UniqueNumber != i_MainSvrItemUID)
		{// 2006-10-11 by cmkwon, 하나의 아이템만 체크한다

			itrG++;
			continue;
		}

		if(COMPARE_BIT_FLAG(pItemGen->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			if(IS_ATTACHABLE_ITEM(pItemGen->ItemInfo))
			{// 2009-10-22 by cmkwon, 장착 가능 아이템류
				if(WEAR_ATTACHED == pItemGen->Wear)	{
				///////////////////////////////////////////////////////////////////////////////
				// 2007-09-07 by cmkwon, 기간 만료된 장착된 아이템 장착 해제 처리 - 마크 아이템을 기간 제한 아이템에 추가함
				MSG_FC_ITEM_CHANGE_WINDOW_POSITION tmItemChangePos;
				util::zero(&tmItemChangePos, sizeof(MSG_FC_ITEM_CHANGE_WINDOW_POSITION));
				tmItemChangePos.CharacterUniqueNumber	= m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
				tmItemChangePos.FromItemUniqueNumber	= pItemGen->UniqueNumber;
				tmItemChangePos.FromItemWindowIndex		= pItemGen->ItemWindowIndex;
				tmItemChangePos.ToItemUniqueNumber		= 0;
				tmItemChangePos.ToItemWindowIndex		= POS_ITEMWINDOW_OFFSET;
				m_pFieldIOCPSocket->ProcessItemChangeWindowPosition(&tmItemChangePos, FALSE, FALSE);
				}
			}

			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 아래와 같이 CAtumLogSender::SendLogMessageITEMDelete() 로 대체 함
			// 2012-08-30 by jhseol, 풀로그 남기기
#ifdef S_FULL_LOG_JHSEOL
			CAtumLogSender::SendLogMessageITEMUseITEM(m_pFieldIOCPSocket, pItemGen);						// 사용 아이템 로그 추가
#endif	//#ifdef S_FULL_LOG_JHSEOL
			// end 2012-08-30 by jhseol, 풀로그 남기기
			// 2008-01-23 by cmkwon, S_F, S_L: 장착/장착해제 게임 로그에 추가 - 기간 만료로 삭제함
			CAtumLogSender::SendLogMessageITEMDelete(m_pFieldIOCPSocket, pItemGen, IDT_EXPIRE_TIME);

			// 1. m_mapItemGeneral에서 제거
			itrG = m_mapItemGeneral.erase(itrG);

			// 2. DB에서 아이템 삭제
			QPARAM_DELETESTOREITEM *pQParam = new QPARAM_DELETESTOREITEM;
			pQParam->AccountUniqueNumber	= m_pFieldIOCPSocket->m_character.AccountUniqueNumber;
			pQParam->ItemUniqueNumber		= pItemGen->UniqueNumber;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteStoreItem, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		
			util::del(pItemGen);

			// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크, 밑과 같이 수정
//			if(IS_VALID_UID64(i_MainSvrItemUID))
//			{// 2006-10-11 by cmkwon, 아이템 하나만 체크
//				return TRUE;
//			}
			return TRUE;

		}
		itrG++;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::LogInfinityUsingItem()
/// \brief		인피니티 - 인피 게임로그 추가
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::LogInfinityUsingItem() {
	mt_auto_lock igLock(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr = m_vectUsingGeneralItem.begin();
	for(; itr != m_vectUsingGeneralItem.end(); itr++) {
		CAtumLogSender::SendLogMessageInfinityFinItem(m_pFieldIOCPSocket, *itr);
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
/// \author		cmkwon
/// \date		2010-01-26 ~ 2010-01-26
/// \warning	
///
/// \param		
/// \return		FALSE	==> 구매 불가( 매진 or ERROR)
///				TRUE	==> 구매 가능
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::CheckBuyLimitedEditionShopItem(INT *o_pRemainCount, INT i_nItemNum)
{
	CODBCStatement mainDBOdbcStmt;
	BOOL bRet = mainDBOdbcStmt.Init(g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD(), g_pGlobal->GetMainWndHandle());
	if (FALSE == bRet)
	{		
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to connect DBServer(%s:%d %s %s %s) !! CFieldItemManager::CheckBuyLimitedEditionShopItem# %s %d \r\n"
			, g_pGlobal->GetDBServerIP(), g_pGlobal->GetDBServerPort(), g_pGlobal->GetDBServerDatabaseName(), (char*)g_pGlobal->GetODBCUID(), (char*)g_pGlobal->GetODBCPASSWORD()
			, GetCharacterString(m_pCharacter, string()), i_nItemNum);
		return FALSE;
	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_check_Buy_LimitedEdtion_shopItem
	-- DESC				: // 2010-01-26 by cmkwon, 캐쉬 아이템 한정판매 시스템 구현 - 
	--					
	-- Result set		: [ErrorCode] [RemainCountForLimitedEdition]
	--------------------------------------------------------------------------------
	CALL dbo.atum_check_Buy_LimitedEdtion_shopItem
	**************************************************************************/
	SQLHSTMT mDBstmt = mainDBOdbcStmt.GetSTMTHandle();
	SQLINTEGER arrCB[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(mDBstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,	&i_nItemNum, 0, &arrCB[1]);
	
	RETCODE ret = mainDBOdbcStmt.ExecuteQuery(PROCEDURE_100127_0545);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to ExecuteQuery !! PROCEDURE_100127_0545 %s %d \r\n", GetCharacterString(m_pCharacter, string()), i_nItemNum);
		mainDBOdbcStmt.ProcessLogMessages(SQL_HANDLE_STMT, mDBstmt,"@PROCEDURE_100127_0545 1 Failed!\r\n", TRUE);
		mainDBOdbcStmt.FreeStatement();
		return FALSE;
	}

	int nErrCode = 1;					// 2010-01-27 by cmkwon, 기본값 Error
	arrCB[1] = arrCB[2] = SQL_NTS;
	SQLBindCol(mDBstmt, 1, SQL_C_LONG, &nErrCode,	0,			&arrCB[1]);
	SQLBindCol(mDBstmt, 2, SQL_C_LONG, o_pRemainCount,	0,		&arrCB[2]);
	
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(mDBstmt, &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 이면 적용된 RowCount 이다 
			ret = SQLMoreResults(mDBstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(mDBstmt);
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(mDBstmt);                 // 2008-07-25 by cmkwon, 다음 Results 가 있는지를 한번더 체크한다.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	mainDBOdbcStmt.FreeStatement();	
	if(ERR_NO_ERROR != nErrCode)
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[DB Error] fail to Fetch !! PROCEDURE_100127_0545 %s %d ErrCode(%d) %d \r\n"
			, GetCharacterString(m_pCharacter, string()), i_nItemNum, nErrCode, *o_pRemainCount);
		return FALSE;
	}

	// 2010-02-02 by cmkwon, 변경된 아이템 정보 저장
	ms_pFieldIOCP2->LimitedEUpdateItemCount(i_nItemNum, *o_pRemainCount);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::AddInvokingItemList(ItemUID_t i_ItemUID)
/// \brief		인피니티 - 발동류장착아이템, 재발동 진행 중 추가
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::AddInvokingItemList(ItemUID_t i_ItemUID)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = this->GetItemGeneralByUID(i_ItemUID);
	if(NULL == pItemG || 0 == pItemG->ItemInfo->ReAttacktime)
	{
		return FALSE;
	}

	if(NULL != this->InvokingFindItemGNoLock(i_ItemUID))
	{// 2010-03-04 by cmkwon, 이미 리스트에 존재

		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldItemManager::AddInvokingItemList# %s %s %d %s\r\n"
			, GetCharacterString(m_pCharacter, string()), GetItemString(pItemG, string()), pItemG->CoolingTimeStamp, pItemG->CoolingStartTime.GetDateTimeString().GetBuffer());
		return FALSE;
	}
	
	pItemG->CoolingStartTime	= ATUM_DATE_TIME { true };	// 2006-03-30 by cmkwon, 아이템 사용 시작시간만을 설정, CoolingTimeStamp는 초기화 하면 안됨
	m_InvokingItemList.push_back(pItemG);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldItemManager::CheckCoolingTimeInvokingItem(ItemUID_t i_ItemUID)
/// \brief		인피니티 - 발동류장착아이템, 쿨타임이 돌고 있는지 체크
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::CheckCoolingTimeInvokingItem(ItemUID_t i_ItemUID, BOOL i_bSendElapsedSec/*=FALSE*/)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	ITEM_GENERAL *pItemG = this->InvokingFindItemGNoLock(i_ItemUID);
	if(NULL == pItemG)
	{
		return FALSE;
	}

	ATUM_DATE_TIME atimeCur { true };
	int nElapsedSec	= pItemG->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemG->CoolingStartTime);	
	if(nElapsedSec < pItemG->ItemInfo->ReAttacktime/1000)
	{
		if(i_bSendElapsedSec)
		{
			// 유저에게 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
			pSMsg->ItemUID			= pItemG->UniqueNumber;
			pSMsg->CoolingTimeStamp	= nElapsedSec;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
		}
		return TRUE;
	}
	this->InvokingDeleteItemGNoLock(i_ItemUID);		// 쿨타임리스트에서 제거

	pItemG->CoolingTimeStamp	= 0;				// 

	//쿨 타임 완료!!
	QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
	pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
	pQParam->CoolingTimeStamp	= pItemG->CoolingTimeStamp;
	ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
	
	// 유저에게 전송
	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
	pMsg->ItemUID	= i_ItemUID;
	m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::StopInvokingItemList(ITEM_GENERAL *i_pItemGen)
/// \brief		인피니티 - 발동류장착아이템, 쿨타임이 정지
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::StopInvokingItemList(ITEM_GENERAL *i_pItemGen) {
	if(NULL == i_pItemGen) {
		return;
	}

	mt_auto_lock igLock(&m_mapItemGeneral);
	
	if(NULL == this->InvokingFindItemGNoLock(i_pItemGen->UniqueNumber))
	{
		return;
	}

	this->InvokingDeleteItemGNoLock(i_pItemGen->UniqueNumber);		// 쿨타임리스트에서 제거
	
	ATUM_DATE_TIME atimeCur { true };
	int nElapsedSec	= i_pItemGen->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(i_pItemGen->CoolingStartTime);	
	if(nElapsedSec < i_pItemGen->ItemInfo->ReAttacktime/1000)
	{		
		i_pItemGen->CoolingTimeStamp	= nElapsedSec;
		i_pItemGen->CoolingStartTime	= atimeCur;

		// 유저에게 전송
		INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
		pSMsg->ItemUID			= i_pItemGen->UniqueNumber;
		pSMsg->CoolingTimeStamp	= nElapsedSec;
		m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
			
		QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
		pQParam->ItemUniqueNumber	= i_pItemGen->UniqueNumber;
		pQParam->CoolingTimeStamp	= nElapsedSec;
		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);		
	}
	else
	{
		i_pItemGen->CoolingTimeStamp = 0;

		//쿨 타임 완료!!
		QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
		pQParam->ItemUniqueNumber	= i_pItemGen->UniqueNumber;
		pQParam->CoolingTimeStamp	= i_pItemGen->CoolingTimeStamp;
		ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
		
		// 유저에게 전송
		INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
		pMsg->ItemUID	= i_pItemGen->UniqueNumber;
		m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
	}	
	ITEM_GENERAL tmItemG = *i_pItemGen;
	igLock.auto_unlock_cancel();

	this->ItemUpdateByInfinityPlaying(&tmItemG);// 인피니티 동기화
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::SaveInvokingItemList()
/// \brief		인피니티 - 발동류장착아이템, 쿨타임 저장
/// \author		dhjin
/// \date		2009-09-09 ~ 2010-02-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::SaveInvokingItemList(BOOL i_bSaveCoolingItem/*=TRUE*/)
 {
	mt_auto_lock igLock(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	while(itr != m_InvokingItemList.end())
	{
		ITEM_GENERAL *pItemG = *itr;
		
		ATUM_DATE_TIME atimeCur { true };
		int nElapsedSec	= pItemG->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemG->CoolingStartTime);			
		if(nElapsedSec < pItemG->ItemInfo->ReAttacktime/1000)
		{
			if(i_bSaveCoolingItem)
			{
				QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
				pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
				pQParam->CoolingTimeStamp	= nElapsedSec;
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

				// 유저에게 전송
				INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
				pSMsg->ItemUID			= pItemG->UniqueNumber;
				pSMsg->CoolingTimeStamp	= nElapsedSec;
				m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
			}
		}
		else
		{
			pItemG->CoolingTimeStamp	= 0;
			
			QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
			pQParam->CoolingTimeStamp	= pItemG->CoolingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

			// 유저에게 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
			pMsg->ItemUID	= pItemG->UniqueNumber;
			m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
			
			// 리스트에서 삭제
			itr = m_InvokingItemList.erase(itr);
			continue;
		}
		itr++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL *CFieldItemManager::InvokingFindItemGNoLock(ItemUID_t i_ItemUID)
{
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	for(; itr != m_InvokingItemList.end(); itr++)
	{
		ITEM_GENERAL *pItemG = *itr;
		if(i_ItemUID == pItemG->UniqueNumber)
		{
			return pItemG;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InvokingDeleteItemGNoLock(ItemUID_t i_ItemUID)
{
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	for(; itr != m_InvokingItemList.end(); itr++)
	{
		ITEM_GENERAL *pItemG = *itr;
		if(i_ItemUID == pItemG->UniqueNumber)
		{
			m_InvokingItemList.erase(itr);
			return TRUE;
		}
	}
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
/// \author		cmkwon
/// \date		2010-03-04 ~ 2010-03-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InvokingCheckExpire(void)
{
	mt_auto_lock igLock(&m_mapItemGeneral);
	vectITEM_GENERALPtr::iterator itr = m_InvokingItemList.begin();
	while(itr != m_InvokingItemList.end())
	{
		ITEM_GENERAL *pItemG = *itr;
		
		ATUM_DATE_TIME atimeCur { true };
		int nElapsedSec	= pItemG->CoolingTimeStamp + atimeCur.GetTimeDiffTimeInSeconds(pItemG->CoolingStartTime);			
		if(nElapsedSec < pItemG->ItemInfo->ReAttacktime/1000)
		{
			QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
			pQParam->CoolingTimeStamp	= nElapsedSec;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
			
			// 유저에게 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK, T_FC_ITEM_GET_COOLINGTIME_INFO_OK, pSMsg, SendBuf);
			pSMsg->ItemUID			= pItemG->UniqueNumber;
			pSMsg->CoolingTimeStamp	= nElapsedSec;
			m_pFieldIOCPSocket->SendAddData(SendBuf, MSG_SIZE(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK));
		}
		else
		{
			pItemG->CoolingTimeStamp	= 0;
			
			QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP *pQParam = new QPARAM_UPDATE_ITEM_COOLINGTIMESTAMP;
			pQParam->ItemUniqueNumber	= pItemG->UniqueNumber;
			pQParam->CoolingTimeStamp	= pItemG->CoolingTimeStamp;
			ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateItemCoolingTimeStamp, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);
			
			// 유저에게 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK, T_FC_ITEM_END_COOLINGTIME_ITEM_OK, pMsg, pBuf);
			pMsg->ItemUID	= pItemG->UniqueNumber;
			m_pFieldIOCPSocket->SendAddData(pBuf, MSG_SIZE(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK));
			
			// 리스트에서 삭제
			itr = m_InvokingItemList.erase(itr);
			continue;
		}
		
		itr++;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인피니티 입장 캐쉬 아이템 구현 -
/// \author		cmkwon
/// \date		2010-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::UpdateItemCountByPointer(DestParam_t i_byDestParam, BYTE i_enumStorage/*=ITEM_IN_CHARACTER*/)		// 2011-08-01 by hskim, 파트너 시스템 2차 - 자료형 변경 (DestParameter - 255 -> 32767 지원)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	
	ITEM_GENERAL *pItemG = this->GetItemGeneralByDestParam(i_byDestParam, i_enumStorage);
	
	if(NULL == pItemG
		|| FALSE == IS_COUNTABLE_ITEM(pItemG->Kind))
	{// 2010-03-23 by cmkwon, 현재는 카운터블 아이템만 처리
		return FALSE;
	}
	
	this->UpdateItemCountByPointer(pItemG, -1, IUT_USE_ITEM);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_INSERT_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromInsertItemListNoLock(INT i_ItemNum)
{
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemNum == pInfiItemInfo->ItemNum)
		{
			return pInfiItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_INSERT_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromInsertItemListNoLock(ItemUID_t i_ItemUID)
{
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			return pInfiItemInfo;
		}
	}
	
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityDeleteItemInfoFromInsertItemList(ItemUID_t i_ItemUID)
{
	mt_auto_lock mtA(&m_mtInfinityInsertItemInfoList);
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			m_mtInfinityInsertItemInfoList.erase(itr);
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InfinitySendInsertItemList(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityInsertItemInfoList);
	mtvectorInfinityInsertItemInfo::iterator itr(m_mtInfinityInsertItemInfoList.begin());
	for(; itr != m_mtInfinityInsertItemInfoList.end(); itr++)
	{
		INFINITY_INSERT_ITEM_INFO *pInfiItemInfo = &*itr;

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_INSERT_ITEM, T_FtoA_INFINITY_INSERT_ITEM, pInsertItemSendMsg, InsertItemSendBuf);
		pInsertItemSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
		pInsertItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
		pInsertItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
		pInsertItemSendMsg->CreatedTime			= pInfiItemInfo->CreatedTime;
		pInsertItemSendMsg->CurrentCount		= pInfiItemInfo->CurrentCount;
		pInsertItemSendMsg->ItemNum				= pInfiItemInfo->ItemNum;
		pInsertItemSendMsg->ItemWindowIndex		= pInfiItemInfo->ItemWindowIndex;
		pInsertItemSendMsg->PrefixCodeNum		= pInfiItemInfo->PrefixCodeNum;
		pInsertItemSendMsg->SuffixCodeNum		= pInfiItemInfo->SuffixCodeNum;
		pInsertItemSendMsg->UsingTimeStamp		= pInfiItemInfo->UsingTimeStamp;
		pInsertItemSendMsg->Wear				= pInfiItemInfo->Wear;
		pInsertItemSendMsg->ShapeItemNum		= pInfiItemInfo->ShapeItemNum;
		pInsertItemSendMsg->CoolingTimeStamp	= pInfiItemInfo->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
		i_pFISoc4MFS->SendAddData(InsertItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_INSERT_ITEM));

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::InfinitySendInsertItemList# !, %s, ItemNum(%d) CurrentCount(%8d) Wear(%d) UsingTimeStamp(%d) CoolingTimeStamp(%d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pInfiItemInfo->ItemNum, pInfiItemInfo->CurrentCount, pInfiItemInfo->Wear, pInfiItemInfo->UsingTimeStamp, pInfiItemInfo->CoolingTimeStamp);
	}

	// 2010-07-15 by jskim 인피니티 종료시 아이템 복사 되는 버그 수정
	m_mtInfinityInsertItemInfoList.clear();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_UPDATE_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromUpdateItemListNoLock(ItemUID_t i_ItemUID)
{
	mtvectorInfinityUpdateItemInfo::iterator itr(m_mtInfinityUpdateItemInfoList.begin());
	for(; itr != m_mtInfinityUpdateItemInfoList.end(); itr++)
	{
		INFINITY_UPDATE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			return pInfiItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityDeleteItemInfoFromUpdateItemList(ItemUID_t i_ItemUID)
{
	mt_auto_lock mtA(&m_mtInfinityUpdateItemInfoList);
	mtvectorInfinityUpdateItemInfo::iterator itr(m_mtInfinityUpdateItemInfoList.begin());
	for(; itr != m_mtInfinityUpdateItemInfoList.end(); itr++)
	{
		INFINITY_UPDATE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID == pInfiItemInfo->ItemUID)
		{
			m_mtInfinityUpdateItemInfoList.erase(itr);
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InfinitySendUpdateItemList(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityUpdateItemInfoList);
	mtvectorInfinityUpdateItemInfo::iterator itr(m_mtInfinityUpdateItemInfoList.begin());
	for(; itr != m_mtInfinityUpdateItemInfoList.end(); itr++)
	{
		INFINITY_UPDATE_ITEM_INFO *pInfiItemInfo = &*itr;

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_UPDATE_ITEM, T_FtoA_INFINITY_UPDATE_ITEM, pUpdateItemSendMsg, UpdateItemSendBuf);
		pUpdateItemSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
		pUpdateItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
		pUpdateItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
		pUpdateItemSendMsg->CurrentCount		= pInfiItemInfo->CurrentCount;
		pUpdateItemSendMsg->ItemWindowIndex		= pInfiItemInfo->ItemWindowIndex;
		pUpdateItemSendMsg->MainSvrItemUID		= pInfiItemInfo->MainSvrItemUID;
		pUpdateItemSendMsg->Wear				= pInfiItemInfo->Wear;
		pUpdateItemSendMsg->UsingTimeStamp		= pInfiItemInfo->UsingTimeStamp;
		pUpdateItemSendMsg->CoolingTimeStamp	= pInfiItemInfo->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
		i_pFISoc4MFS->SendAddData(UpdateItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_UPDATE_ITEM));

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::InfinitySendUpdateItemList# !, %s, ItemUID4MFS(%I64d) CurrentCount(%8d) Wear(%d) UsingTimeStamp(%d) CoolingTimeStamp(%d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pInfiItemInfo->MainSvrItemUID, pInfiItemInfo->CurrentCount, pInfiItemInfo->Wear, pInfiItemInfo->UsingTimeStamp, pInfiItemInfo->CoolingTimeStamp);
	}

	// 2010-07-15 by jskim 인피니티 종료시 아이템 복사 되는 버그 수정
	m_mtInfinityUpdateItemInfoList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INFINITY_DELETE_ITEM_INFO *CFieldItemManager::InfinityFindItemInfoFromDeleteItemListNoLock(ItemUID_t i_ItemUID4MFS)
{
	mtvectorInfinityDeleteItemInfo::iterator itr(m_mtInfinityDeleteItemInfoList.begin());
	for(; itr != m_mtInfinityDeleteItemInfoList.end(); itr++)
	{
		INFINITY_DELETE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID4MFS == pInfiItemInfo->MainSvrItemUID)
		{
			return pInfiItemInfo;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityDeleteItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityDeleteItemInfoList);
	mtvectorInfinityDeleteItemInfo::iterator itr(m_mtInfinityDeleteItemInfoList.begin());
	for(; itr != m_mtInfinityDeleteItemInfoList.end(); itr++)
	{
		INFINITY_DELETE_ITEM_INFO *pInfiItemInfo = &*itr;
		if(i_ItemUID4MFS == pInfiItemInfo->MainSvrItemUID)
		{
			m_mtInfinityDeleteItemInfoList.erase(itr);
			return TRUE;
		}
	}
	
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinityInsertItemInfoFromDeleteItemList(ItemUID_t i_ItemUID4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityDeleteItemInfoList);
	INFINITY_DELETE_ITEM_INFO *pDelItemInfo = this->InfinityFindItemInfoFromDeleteItemListNoLock(i_ItemUID4MFS);
	if(pDelItemInfo)
	{
		return FALSE;
	}

	INFINITY_DELETE_ITEM_INFO tmItemInfo;	util::zero(&tmItemInfo, sizeof(tmItemInfo));
	tmItemInfo.MainSvrItemUID	= i_ItemUID4MFS;
	m_mtInfinityDeleteItemInfoList.push_back(tmItemInfo);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-02 by cmkwon, 인피2차 추가 수정 - 
/// \author		cmkwon
/// \date		2010-04-02 ~ 2010-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::InfinitySendDeleteItemList(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtA(&m_mtInfinityDeleteItemInfoList);
	mtvectorInfinityDeleteItemInfo::iterator itr(m_mtInfinityDeleteItemInfoList.begin());
	for(; itr != m_mtInfinityDeleteItemInfoList.end(); itr++)
	{
		INFINITY_DELETE_ITEM_INFO *pInfiItemInfo = &*itr;

		INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_DELETE_ITEM, T_FtoA_INFINITY_DELETE_ITEM, pDeleteItemSendMsg, DeleteItemSendBuf);
		pDeleteItemSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
		pDeleteItemSendMsg->MFSCharacterUID		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
		pDeleteItemSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
		pDeleteItemSendMsg->MainSvrItemUID		= pInfiItemInfo->MainSvrItemUID;
		i_pFISoc4MFS->SendAddData(DeleteItemSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_DELETE_ITEM));

//		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] [Infinity][%I64d] CFieldItemManager::InfinitySendDeleteItemList# !, %s DeleteItemMainSvrUID(%I64d) \r\n"
//			, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pInfiItemInfo->MainSvrItemUID);
	}

	// 2010-07-15 by jskim 인피니티 종료시 아이템 복사 되는 버그 수정
	m_mtInfinityDeleteItemInfoList.clear();
}

// start 2011-09-30 by hskim, 파트너 시스템 2차
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// start 2011-09-30 by hskim, 파트너 시스템 2차 - 파트너 Current 정보 전달 (현재 가지고 있는 td_Pet 정보 전달)
/// \author		hskim
/// \date		2010-09-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldItemManager::InfinitySendPetItemInfo(CFieldIOCPSocket *i_pFISoc4MFS)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();

	for( ; itr != m_mapItemGeneral.end(); itr++)
	{
		if( ITEMKIND_PET_ITEM == itr->second->Kind )
		{
			// 펫 정보 읽어와서 전달
			ITEM_GENERAL *pItemG = itr->second;
			ITEM *pItemInfo = itr->second->ItemInfo;
			tPET_CURRENTINFO *pPetCurrentData = NULL;

			if( NULL == pItemG ) continue;
			if( NULL == pItemInfo) continue;
			if( 0 == pItemG->MainSvrItemUID )
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Error] [Infinity][%I64d] CFieldItemManager::InfinitySendPetItemInfo MainSvrItemUID is Zero!!, %s, ItemNum(%d) CurrentCount(%8d) Wear(%d) UsingTimeStamp(%d) CoolingTimeStamp(%d) \r\n"
					, m_pFieldIOCPSocket->m_InfinityPlayingInfo.InfinityCreateUID, GetCharacterString(m_pCharacter, string()), pItemG->ItemNum, pItemG->CurrentCount, pItemG->Wear, pItemG->UsingTimeStamp, pItemG->CoolingTimeStamp);

				continue;
			}

			pPetCurrentData = GetItemPetByUID(pItemG->UniqueNumber);

			if( NULL != pPetCurrentData )
			{
				INIT_MSG_WITH_BUFFER(MSG_FtoA_INFINITY_UPDATE_ITEM_PET, T_FtoA_INFINITY_UPDATE_ITEM_PET, pPetSendMsg, PetSendBuf);
				pPetSendMsg->AccountUID			= m_pCharacter->AccountUniqueNumber;
				pPetSendMsg->MFSCharacterUID	= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSCharacterUID;
				pPetSendMsg->MFSClientIndex		= m_pFieldIOCPSocket->m_AfsNeedMfsInfo.MFSClientIdx;
				pPetSendMsg->MainSvrItemUID		= pItemG->MainSvrItemUID;		// 인피니티나 아레나에서는 파트너 생성이 안됨, 이 UID가 0인 경우는 논리적인 에러

				memcpy(&pPetSendMsg->PetCurrentData, pPetCurrentData, sizeof(pPetSendMsg->PetCurrentData));

				i_pFISoc4MFS->SendAddData(PetSendBuf, MSG_SIZE(MSG_FtoA_INFINITY_UPDATE_ITEM_PET));
			}
		}
	}

	return TRUE;
}
// end 2011-09-30 by hskim, 파트너 시스템 2차

// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
BOOL CFieldItemManager::RegisterTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral)
{
	if( NULL == i_pItemGeneral )
	{
		return FALSE;
	}

	if( IS_ENABLE_CHANGE_ShapeItemNum(i_pItemGeneral->Kind) )
	{
		if( 0 != i_pItemGeneral->ShapeItemNum && TRUE == i_pItemGeneral->FixedTermShape.bActive )
		{
			INT nCalcMin = 0;
			ATUM_DATE_TIME CurrentDate { true };

			nCalcMin = i_pItemGeneral->FixedTermShape.EndDate.GetTimeDiffTimeInMinutes(CurrentDate);

			if( nCalcMin > 0 && nCalcMin < MANAGE_MAX_TIME_FIXED_TERM_MINUTELY )
			{
				i_pItemGeneral->FixedTermShape.TimerUID = m_pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, nCalcMin * TIMER_DO_MINUTELY_WORK, 0, 0);
			}
		}
	}

	return TRUE;
}

BOOL CFieldItemManager::RemoveTimerFixedTermShape(ITEM_GENERAL *i_pItemGeneral)
{
	if( NULL == i_pItemGeneral )
	{
		return FALSE;
	}	

	if( IS_ENABLE_CHANGE_ShapeItemNum(i_pItemGeneral->Kind) )
	{
		if( 0 != i_pItemGeneral->ShapeItemNum && TRUE == i_pItemGeneral->FixedTermShape.bActive )
		{
			m_pFieldIOCPSocket->m_TimerManager.StopTimerField(i_pItemGeneral->FixedTermShape.TimerUID);
			i_pItemGeneral->FixedTermShape.TimerUID = 0;
		}
	}
	
	return TRUE;
}

BOOL CFieldItemManager::DeleteFixedTermShapeByTimerUID(UID32_t TimerUID)
{
	mt_auto_lock mtAuto(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();

	for( ; itr != m_mapItemGeneral.end(); itr++)
	{
		if( IS_ENABLE_CHANGE_ShapeItemNum(itr->second->Kind) )		// 외형 변경 가능한 아이템들만 확인
		{
			ITEM_GENERAL *pItemG = itr->second;
			ITEM *pItemInfo = itr->second->ItemInfo;

			if( pItemG->FixedTermShape.TimerUID == TimerUID )
			{
				INT nCalcMin = 0;
				ATUM_DATE_TIME CurrentDate { true };

				nCalcMin = pItemG->FixedTermShape.EndDate.GetTimeDiffTimeInMinutes(CurrentDate);

				if( nCalcMin > 0 )
				{
					if( nCalcMin < MANAGE_MAX_TIME_FIXED_TERM_MINUTELY )
					{
						pItemG->FixedTermShape.TimerUID = m_pFieldIOCPSocket->m_TimerManager.StartTimerField(TE_TYPE_FIXED_TERM_SHAPE, nCalcMin * TIMER_DO_MINUTELY_WORK, 0, 0);
					}

					return FALSE;
				}
				
				INT nDelShapeItemNum = pItemG->ShapeItemNum;
				FIXED_TERM_INFO DelFixedTermShape;
				memcpy(&DelFixedTermShape, &pItemG->FixedTermShape, sizeof(FIXED_TERM_INFO));

				// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 기간 종료
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - 기간 종료
				if ( ITEMKIND_DEFENSE == pItemG->Kind )	// 아머류만
				{
					COLLECTION_INFO *tmCollectiobPtr = m_pFieldIOCPSocket->m_ArmorCollection.ChangeActivedCount(pItemG->ShapeItemNum, FALSE);
					if ( NULL != tmCollectiobPtr && 0 == tmCollectiobPtr->ActivedCount && 0 >= tmCollectiobPtr->RemainSeconds )
					{
						ITEM *tmShapeItem = ms_pFieldIOCP2->GetItemInfo(pItemG->ShapeItemNum);
						if ( NULL != tmShapeItem )
						{
							m_pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_130531_0008, tmShapeItem->ItemName);
						}
					}
					if ( WEAR_ATTACHED == pItemG->Wear )
					{
						ITEM *pShapeKitItem = ms_pFieldIOCP2->GetItemInfo(pItemG->FixedTermShape.nStatShapeItemNum);
						if ( NULL != pShapeKitItem )
						{
							if ( TRUE == pShapeKitItem->IsExistDesParam(DES_ALL_PART) )
							{
								m_pFieldIOCPSocket->CheckAllState(pShapeKitItem->GetParameterValue(DES_ALL_PART)*min(pItemG->FixedTermShape.nStatLevel, SHAPE_STAT_MAX_LEVEL), FALSE);
							}
						}
					}
					////////////////////////////////////////////
					// 장착중인 엔진을 먼저 찾는다,
					ITEM_GENERAL *pBeforeEngineItem = m_pFieldIOCPSocket->GetAttachItemGeneralByPosition(POS_REAR);
					m_pFieldIOCPSocket->m_ItemManager.CheckRequirementAllGeneralItems();
					ITEM_GENERAL *pAfterEngineItem = m_pFieldIOCPSocket->GetAttachItemGeneralByPosition(POS_REAR);
					if ( NULL != pBeforeEngineItem && NULL == pAfterEngineItem )		// 장착 엔진이 없음.
					{
						m_pFieldIOCPSocket->SendString128(STRING_128_USER_POPUP, STRMSG_130531_0004);
// 						if ( FALSE == IS_CITY_MAP_INDEX(m_pFieldIOCPSocket->m_character.MapChannelIndex.MapIndex) )		// 도시맵이 아니면 강제 워프
// 						{
// 							m_pFieldIOCPSocket->SendString128(STRING_128_USER_NOTICE, STRMSG_130531_0005);
// 							m_pFieldIOCPSocket->WarpToCityMap();
// 						}
					}
					pItemG->FixedTermShape.nStatLevel			= SHAPE_STAT_INIT_LEVEL;
					pItemG->FixedTermShape.nStatShapeItemNum	= 0;
					
					// DB 저장 부분 
					QPARAM_SHAPE_STATLEVEL	*pQParamSS = new QPARAM_SHAPE_STATLEVEL;
					pQParamSS->ItemKind				= pItemG->ItemInfo->Kind;			// 아이템 kind
					pQParamSS->ItemUID				= pItemG->UniqueNumber;
					pQParamSS->nStatLevel			= pItemG->FixedTermShape.nStatLevel;
					pQParamSS->nStatShapeItemNum	= pItemG->FixedTermShape.nStatShapeItemNum;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_InsertShapeStatLevel, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamSS);
				}
#endif
				// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 기간 종료
				pItemG->ShapeItemNum = 0;

				QPARAM_UPDATE_SHAPEITEMNUM *pQParam = new QPARAM_UPDATE_SHAPEITEMNUM;
				pQParam->ItemUID		= pItemG->UniqueNumber;
				pQParam->nShapeItemNum	= pItemG->ShapeItemNum;
				// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
				pQParam->FixedTermShape.bActive = FALSE;
				pQParam->FixedTermShape.StartDate.Reset();
				pQParam->FixedTermShape.EndDate.Reset();
				pQParam->FixedTermShape.TimerUID = 0;
				// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)
				
				// 2013-05-31 by jhseol,bckim 아머 컬렉션 - 기간 종료
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - 기간 종료
				pQParam->FixedTermShape.nStatLevel			= pItemG->FixedTermShape.nStatLevel;
				pQParam->FixedTermShape.nStatShapeItemNum	= pItemG->FixedTermShape.nStatShapeItemNum;
#endif
				// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - 기간 종료
	
				ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_UpdateShapeItemNum, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParam);

				if( FALSE == g_pFieldGlobal->IsArenaServer() )
				{
					QPARAM_FIXED_TERM_SHAPE	*pQParamFT = new QPARAM_FIXED_TERM_SHAPE;
					pQParamFT->ItemUID = pItemG->UniqueNumber;
					pQParamFT->TermType = FIXED_TERM_SHAPE;
					pQParamFT->AppliedItemNum = pItemG->ShapeItemNum;
					pQParamFT->StartDate = pItemG->FixedTermShape.StartDate;
					pQParamFT->EndDate = pItemG->FixedTermShape.EndDate;
					ms_pFieldIOCP2->m_pAtumDBManager->MakeAndEnqueueQuery(QT_DeleteFixedTerm, m_pFieldIOCPSocket, m_pFieldIOCPSocket->m_character.AccountUniqueNumber, pQParamFT);

					CAtumLogSender::SendLogMessageFixedTermShapeEnd(m_pFieldIOCPSocket, pItemG, nDelShapeItemNum, DelFixedTermShape);
				}

				// 2013-04-18 by jhseol,bckim 이달의 아머 - 외형 기간 종료에 따른 이벤트 확인
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// 외형 기간 종료에 따른 이벤트 확인
				if ( NULL != ms_pFieldIOCP2->m_pMonthlyArmorManager )
				{
					ms_pFieldIOCP2->m_pMonthlyArmorManager->CheckMonthlyArmorEventItem(pItemG, m_pFieldIOCPSocket);
				}
#endif
				// end 2013-04-18 by jhseol,bckim 이달의 아머 - 외형 기간 종료에 따른 이벤트 확인

				return TRUE;
			}
		}
	}

	return FALSE;
}
// end 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		인벤토리 내에 특정 아이템을 찾는 함수
/// \author		jhjang
/// \date		2012-10-13 ~ 2012-10-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM_GENERAL *CFieldItemManager::FindGeneralItemByItemNum(INT i_nItemNum)
{
	mt_auto_lock igLock(&m_mapItemGeneral);

	mtmapUID2ItemGeneral::iterator itr = m_mapItemGeneral.begin();
	for(; itr != m_mapItemGeneral.end(); itr++)
	{
		ITEM_GENERAL *pItemG = itr->second;
		if(i_nItemNum == pItemG->ItemNum)
		{
			return pItemG;
		}
	}
	return NULL;
}
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

// start 2012-11-21 by khkim, GLog 2차
void CFieldItemManager::SetGLogData(BYTE i_nLogType, ITEM_GENERAL *i_pItemGeneral, BYTE i_nCharLv, int i_nChangeCount, int i_nCharSerial)
{
	if(NULL != i_pItemGeneral && NULL != m_pFieldIOCPSocket )	// 2013-05-30 by jhseol, GLog 시스템 보완
	{
		if( FALSE == i_pItemGeneral->CurrentCount )
		{// SPI 0개면 출력NO
			return;	
		}

		MSG_FL_LOG_ITEM LogItemParameter; 
		util::zero(&LogItemParameter, sizeof(LogItemParameter));	// 2013-05-30 by jhseol, GLog 시스템 보완 - 초기화
		LogItemParameter.LogType			= i_nLogType; 
		LogItemParameter.ItemSerial			= (UID64_t)i_pItemGeneral->ItemNum;	// 2013-05-30 by jhseol, GLog 시스템 보완 - 캐스팅
		LogItemParameter.Count1				= (UID64_t)i_pItemGeneral->CurrentCount;	// 2013-05-30 by jhseol, GLog 시스템 보완 - 캐스팅
		// 2013-05-30 by jhseol, GLog 시스템 보완
//		LogItemParameter.CharSerial			= i_nCharSerial;
//		LogItemParameter.CharLv				= i_nCharLv;
		LogItemParameter.CharSerial			= (UID64_t)m_pFieldIOCPSocket->m_character.CharacterUniqueNumber;
		LogItemParameter.CharLv				= (int)m_pFieldIOCPSocket->m_character.Level;
		// end 2013-05-30 by jhseol, GLog 시스템 보완
		//LogItemParameter.GameServerID		= g_pFieldGlobal->GetMGameServerID();

		if( NULL != i_nChangeCount )
		{// 사용아이템 갯수로 출력(UpdateItemCountByPointer함수 사용시만 적용) 
			LogItemParameter.Count1			= -i_nChangeCount;	
		}	

		// 2013-01-16 by khkim, 상점에서 무기 구매시 총알수 1게 버그 수정
		if( FALSE == IS_COUNTABLE_ITEM(i_pItemGeneral->Kind) )
		{
			LogItemParameter.Count1 = 1;
		}
		// end 2013-01-16 by khkim, 상점에서 무기 구매시 총알수 1게 버그 수정
		
		CAtumLogSender::SendLogItem(&LogItemParameter);
	}
}
// end 2012-11-21 by khkim, GLog 2차

// 2013-05-31 by jhseol,bckim 아머 컬렉션 - ReturnShapeChangeKitItem 함수 구현부
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldItemManager::ReturnShapeChangeKitItem(ITEM_GENERAL *i_pItemG)
/// \brief		아이템에 적용중인 영구제 외형을 확인하고 되돌려 주는 함수
/// \author		jhseol
/// \date		2013-06-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldItemManager::ReturnShapeChangeKitItem(ITEM_GENERAL *i_pItemG)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM	// - 아이템에 적용중인 영구제 외형을 확인하고 되돌려 주는 함수
	if ( NULL == i_pItemG )
	{
		return;
	}
	if ( ITEMKIND_DEFENSE == i_pItemG->Kind					// 아머류 이고
		&& 0 != i_pItemG->ShapeItemNum						// 적용중인 외형이 있고
		&& FALSE == i_pItemG->FixedTermShape.bActive		// 영구제 일 경우
		&& SHAPE_STAT_INIT_LEVEL >= i_pItemG->FixedTermShape.nStatLevel )		// 일반 외변킷으로 변경된 녀석들만.
	{
		ez_map<INT, ITEM*>::iterator itrKitItem = ms_pFieldIOCP2->m_mapItemInfo.begin();
		while (itrKitItem != ms_pFieldIOCP2->m_mapItemInfo.end())
		{
			ITEM *pKitItem = itrKitItem->second;
			if ( ITEMKIND_INGOT == pKitItem->Kind									// 일반 외변킷의 KIND 이어야 하며
				&& i_pItemG->ShapeItemNum == pKitItem->LinkItem						// 같은 외형이어야 하며
				&& TRUE == IS_SAME_UNITKIND(i_pItemG->ItemInfo->ReqUnitKind, pKitItem->ReqUnitKind)	// 동일 한 ReqUnitKind 이어야 하며
				&& TRUE == pKitItem->IsExistDesParam(DES_SHAPE_ITEM)				// 외형 변경 DES를 가지고 있어야 하며
				&& FALSE == pKitItem->IsExistDesParam(DES_FIXED_TERM_SHAPE_TIME) )	// 기간제가 아니어야 한다.
			{
				InsertItemBaseByItemNum(pKitItem->ItemNum, 1, IUT_BONUS_ITEM);
				return;
			}
			itrKitItem++;
		}
	}
#endif
}

bool CFieldItemManager::UpgradePet(UID64_t oldPetUID, ITEM* targetPetItem)
{
	if (oldPetUID < 0 || !targetPetItem)
		return false;

	// Query the Field DB Manager with the upgrade procedure of the Pet
	QPARAM_UPGRADE_PET* upgradePetParams = new QPARAM_UPGRADE_PET;
	upgradePetParams->OldItemUniqueNumber = oldPetUID;
	upgradePetParams->UpgradedPetItemUID = targetPetItem->ItemNum;

	m_pFieldDBManager->MakeAndEnqueueQuery(QT_UpgradePet
		, m_pFieldIOCPSocket
		, m_pFieldIOCPSocket->m_character.AccountUniqueNumber
		, upgradePetParams);

	return true;
}

void CFieldItemManager::InsertUpgradedPet(UID64_t oldPetUID, ITEM_GENERAL* newItemData, tPET_CURRENTINFO* newPetInfo)
{
	if (!newItemData || !newPetInfo || !m_pFieldIOCPSocket)
		return;

	// Delete old PET (without removing its dependent sockets)
	ITEM_GENERAL *i_pItemGeneral = GetItemGeneralByUID(oldPetUID, ITEM_IN_CHARACTER);
	if (!i_pItemGeneral)
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: Failed to upgrade the Pet. The old Pet was not found. Old Pet UID (%d), new Pet UID (%d)", oldPetUID, newItemData->UniqueNumber);
		return;
	}
	mt_auto_lock mtAuto(&m_mapItemGeneral);
	m_mapItemGeneral.deleteNoLock(oldPetUID);
	mtAuto.auto_unlock_cancel();
	// Remove from IF Item List
	this->ItemDeleteByInfinityPlaying(i_pItemGeneral);

	// Send deletion of the Item to client
	INIT_MSG_WITH_BUFFER(MSG_FC_STORE_DELETE_ITEM, T_FC_STORE_DELETE_ITEM, msgDelete, msgDeleteBuf);
	msgDelete->ItemUniqueNumber = i_pItemGeneral->UniqueNumber;
	msgDelete->ItemDeletionType = IUT_MIXING;
	m_pFieldIOCPSocket->SendAddData(msgDeleteBuf, MSG_SIZE(MSG_FC_STORE_DELETE_ITEM));

	// Delete from PET Data
	mt_auto_lock mtauto(&m_vectPetCurrentInfo);
	for (mtvectPetCurrentInfo::iterator itr = m_vectPetCurrentInfo.begin(); itr != m_vectPetCurrentInfo.end(); itr++)
	{
		if ((*itr)->CreatedPetUID == i_pItemGeneral->UniqueNumber)
		{
			// Send deletion of the Pet to client
			INIT_MSG_WITH_BUFFER(MSG_TRADE_PET_DATA, T_FC_TRADE_DELETE_CURRENT_PET_DATA, Petcurrentmsg, sendPetbuff);
			Petcurrentmsg->CreatedPetUID = (*itr)->CreatedPetUID;
			Petcurrentmsg->PetExp = (*itr)->PetExp;
			Petcurrentmsg->PetIndex = (*itr)->PetIndex;
			Petcurrentmsg->PetLevel = (*itr)->PetLevel;
			memcpy(Petcurrentmsg->PetName, (*itr)->PetName, SIZE_MAX_PET_NAME);
			m_pFieldIOCPSocket->SendAddData(sendPetbuff, MSG_SIZE(MSG_TRADE_PET_DATA));

			// Remove from Memory
			util::del(*itr);
			m_vectPetCurrentInfo.erase(itr);
			break;
		}
	}
	mtauto.auto_unlock_cancel();

	// Delete old Item Memory
	util::del(i_pItemGeneral);

	// Copy the Items to Heap
	ITEM_GENERAL* newStoreItem = new ITEM_GENERAL;
	memcpy_s(newStoreItem, sizeof(ITEM_GENERAL), newItemData, sizeof(ITEM_GENERAL));
	tPET_CURRENTINFO* newPetItem = new tPET_CURRENTINFO(newPetInfo);

	// Add Store item (without adding to the DB)
	InsertItemGeneralByPointer(newStoreItem
		, IUT_MIXING
		, FALSE
		, ITEM_IN_CHARACTER
		, FALSE
		, TRUE);

	// Insert the new Pet
	InsertItemPetByPointer(newPetItem);

	// Update the Clients Pet Data
	m_pFieldIOCPSocket->SendPetItemList();
}

// end 2013-05-31 by jhseol,bckim 아머 컬렉션 - ReturnShapeChangeKitItem 함수 구현부


















