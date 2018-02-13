// InvokingItemManager.cpp: implementation of the CInvokingItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvokingItemManager.h"
#include "FieldIOCPSocket.h"

#include "Random.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvokingItemManager::CInvokingItemManager()
{
	this->Init();
}

CInvokingItemManager::~CInvokingItemManager()
{

}

void CInvokingItemManager::Init() {
	m_InvokingItemListByEquip.clearLock();
	m_InvokingItemListByUse.clearLock();
}

void CInvokingItemManager::ApplyInvokingItemDestParamByEquip(ITEM_GENERAL * i_pItemG) {
	if(NULL == i_pItemG->ItemInfo->pInvokingDestParamList) {
		return;
	}
	
	mt_auto_lock mta(&m_InvokingItemListByEquip);

	InvokingWearItemDestParamList::iterator itr = i_pItemG->ItemInfo->pInvokingDestParamList->begin();
	for(; itr != i_pItemG->ItemInfo->pInvokingDestParamList->end(); itr++)
	{
		INVOKING_ITEM_DESTPARAM_TYPE * pInvokingItemDestParamTypeInfo = this->GetParamTypeInfoByEquip(NULL, i_pItemG->UniqueNumber, itr->InvokingDestParam);
		if(NULL != pInvokingItemDestParamTypeInfo)
		{
			// 있으면 향상
			pInvokingItemDestParamTypeInfo->InvokingDestParamValue += itr->InvokingDestParamValue;
		}
		else
		{
			// 없으면 추가
			INVOKING_ITEM_DESTPARAM_TYPE InvokingItemDestParamTypeInfo;
			util::zero(&InvokingItemDestParamTypeInfo, sizeof(INVOKING_ITEM_DESTPARAM_TYPE));
			InvokingItemDestParamTypeInfo.InvokingItemUID			= i_pItemG->UniqueNumber;
			InvokingItemDestParamTypeInfo.InvokingDestParam			= itr->InvokingDestParam;
			InvokingItemDestParamTypeInfo.InvokingDestParamValue	= itr->InvokingDestParamValue;
			InvokingItemDestParamTypeInfo.InvokingEffectIdx			= itr->InvokingEffectIdx;
			m_InvokingItemListByEquip.push_back(InvokingItemDestParamTypeInfo);
		}
	}
}

void CInvokingItemManager::ApplyEnchantInvokingItemDestParamByEquip(UID64_t i_ItemUID, ITEM * i_pItem) {
	if(NULL == i_pItem->pInvokingDestParamList) {
		return;
	}
	
	mt_auto_lock mta(&m_InvokingItemListByEquip);
	
	InvokingWearItemDestParamList::iterator itr = i_pItem->pInvokingDestParamList->begin();
	for(; itr != i_pItem->pInvokingDestParamList->end(); itr++)
	{
		INVOKING_ITEM_DESTPARAM_TYPE * pInvokingItemDestParamTypeInfo = this->GetParamTypeInfoByEquip(NULL, i_ItemUID, itr->InvokingDestParam);		
		if(NULL != pInvokingItemDestParamTypeInfo)
		{
			// 있으면 향상
			pInvokingItemDestParamTypeInfo->InvokingDestParamValue += itr->InvokingDestParamValue;
		}
		else
		{
			// 없으면 추가
			INVOKING_ITEM_DESTPARAM_TYPE InvokingItemDestParamTypeInfo;
			util::zero(&InvokingItemDestParamTypeInfo, sizeof(INVOKING_ITEM_DESTPARAM_TYPE));
			InvokingItemDestParamTypeInfo.InvokingItemUID			= i_ItemUID;
			InvokingItemDestParamTypeInfo.InvokingDestParam			= itr->InvokingDestParam;
			InvokingItemDestParamTypeInfo.InvokingDestParamValue	= itr->InvokingDestParamValue;
			InvokingItemDestParamTypeInfo.InvokingEffectIdx			= itr->InvokingEffectIdx;
			m_InvokingItemListByEquip.push_back(InvokingItemDestParamTypeInfo);
		}
	}
}

void CInvokingItemManager::ApplyInvokingItemDestParamByUse(ITEM_GENERAL * i_pItemG) {
	if(NULL == i_pItemG->ItemInfo->pInvokingDestParamByUseList) {
		return;
	}

	mt_auto_lock mta(&m_InvokingItemListByUse);

	InvokingWearItemDestParamList::iterator itr = i_pItemG->ItemInfo->pInvokingDestParamByUseList->begin();
	for(; itr != i_pItemG->ItemInfo->pInvokingDestParamByUseList->end(); itr++)
	{
		INVOKING_ITEM_DESTPARAM_TYPE * pInvokingItemDestParamTypeInfo = this->GetParamTypeInfoByUse(NULL, i_pItemG->UniqueNumber, itr->InvokingDestParam);
		if(NULL != pInvokingItemDestParamTypeInfo)
		{
			// 있으면 향상
			pInvokingItemDestParamTypeInfo->InvokingDestParamValue += itr->InvokingDestParamValue;
		}
		else
		{
			// 없으면 추가
			INVOKING_ITEM_DESTPARAM_TYPE InvokingItemDestParamTypeInfo;
			util::zero(&InvokingItemDestParamTypeInfo, sizeof(INVOKING_ITEM_DESTPARAM_TYPE));
			InvokingItemDestParamTypeInfo.InvokingItemUID			= i_pItemG->UniqueNumber;
			InvokingItemDestParamTypeInfo.InvokingDestParam			= itr->InvokingDestParam;
			InvokingItemDestParamTypeInfo.InvokingDestParamValue	= itr->InvokingDestParamValue;
			InvokingItemDestParamTypeInfo.InvokingEffectIdx			= itr->InvokingEffectIdx;
			m_InvokingItemListByUse.push_back(InvokingItemDestParamTypeInfo);
		}
	}
}

void CInvokingItemManager::ApplyEnchantInvokingItemDestParamByUse(UID64_t i_ItemUID, ITEM * i_pItem) {
	if(NULL == i_pItem->pInvokingDestParamByUseList) {
		return;
	}

	mt_auto_lock mta(&m_InvokingItemListByUse);
	
	InvokingWearItemDestParamList::iterator itr = i_pItem->pInvokingDestParamByUseList->begin();
	for(; itr != i_pItem->pInvokingDestParamByUseList->end(); itr++)
	{
		INVOKING_ITEM_DESTPARAM_TYPE * pInvokingItemDestParamTypeInfo = this->GetParamTypeInfoByUse(NULL, i_ItemUID, itr->InvokingDestParam);		
		if(NULL != pInvokingItemDestParamTypeInfo)
		{
			// 있으면 향상
			pInvokingItemDestParamTypeInfo->InvokingDestParamValue += itr->InvokingDestParamValue;
		}
		else
		{
			// 없으면 추가
			INVOKING_ITEM_DESTPARAM_TYPE InvokingItemDestParamTypeInfo;
			util::zero(&InvokingItemDestParamTypeInfo, sizeof(INVOKING_ITEM_DESTPARAM_TYPE));
			InvokingItemDestParamTypeInfo.InvokingItemUID			= i_ItemUID;
			InvokingItemDestParamTypeInfo.InvokingDestParam			= itr->InvokingDestParam;
			InvokingItemDestParamTypeInfo.InvokingDestParamValue	= itr->InvokingDestParamValue;
			InvokingItemDestParamTypeInfo.InvokingEffectIdx			= itr->InvokingEffectIdx;
			m_InvokingItemListByUse.push_back(InvokingItemDestParamTypeInfo);
		}
	}
}

void CInvokingItemManager::ReleaseInvokingItemDestParamByUse(ITEM_GENERAL * i_pItemG) {
	if(NULL == i_pItemG->ItemInfo->pInvokingDestParamByUseList) {
		return;
	}

	this->DeleteParamTypeInfoByUse(i_pItemG->UniqueNumber);
}

void CInvokingItemManager::DeleteParamTypeInfoByUse(ItemUID_t i_ItemUID)
 {
	mt_auto_lock mta(&m_InvokingItemListByUse);
	mtInvokingItemDestParamTypeList::iterator itr = m_InvokingItemListByUse.begin();
	while(itr != m_InvokingItemListByUse.end())
	{
		if(i_ItemUID == itr->InvokingItemUID)
		{
			// 장착 해제 이므로 존재하는 데스파람이 있다면 바로 삭제
			itr = m_InvokingItemListByUse.erase(itr);
			continue;
		}
		itr++;
	}
}

void CInvokingItemManager::GetParamRateList(DestParam_t i_DestParam, InvokingItemDestParamTypeList *o_pInvokingItemDestParamTypeList) {
	if(NULL == o_pInvokingItemDestParamTypeList){
		return;
	}
	mt_auto_lock mta(&m_InvokingItemListByEquip);
	mtInvokingItemDestParamTypeList::iterator itr = m_InvokingItemListByEquip.begin();
	for(; itr != m_InvokingItemListByEquip.end(); itr++) {
		if(i_DestParam == itr->InvokingDestParam) {
			o_pInvokingItemDestParamTypeList->push_back(*itr);
		}
	}
}

void CInvokingItemManager::GetParamRateListByUse(DestParam_t i_DestParam, InvokingItemDestParamTypeList *o_pInvokingItemDestParamTypeList) {
	if(NULL == o_pInvokingItemDestParamTypeList){
		return;
	}
	mt_auto_lock mta(&m_InvokingItemListByUse);
	mtInvokingItemDestParamTypeList::iterator itr = m_InvokingItemListByUse.begin();
	for(; itr != m_InvokingItemListByUse.end(); itr++) {
		if(i_DestParam == itr->InvokingDestParam) {
			o_pInvokingItemDestParamTypeList->push_back(*itr);
		}
	}
}

INVOKING_ITEM_DESTPARAM_TYPE * CInvokingItemManager::GetParamTypeInfoByEquip(INVOKING_ITEM_DESTPARAM_TYPE *o_pInvokingInfo, ItemUID_t i_ItemUID, DestParam_t i_DestParam) {
	mt_auto_lock mta(&m_InvokingItemListByEquip);
	mtInvokingItemDestParamTypeList::iterator itr = m_InvokingItemListByEquip.begin();
	for(; itr != m_InvokingItemListByEquip.end(); itr++) 
	{
		INVOKING_ITEM_DESTPARAM_TYPE *pInvoInfo = &*itr;
		if(i_ItemUID == pInvoInfo->InvokingItemUID
			&& i_DestParam == pInvoInfo->InvokingDestParam)
		{
			if(o_pInvokingInfo)
			{
				*o_pInvokingInfo = *pInvoInfo;
			}
			return pInvoInfo;
		}
	}
	return NULL;
}

INVOKING_ITEM_DESTPARAM_TYPE * CInvokingItemManager::GetParamTypeInfoByUse(INVOKING_ITEM_DESTPARAM_TYPE *o_InvokingInfo, ItemUID_t i_ItemUID, DestParam_t i_DestParam) {
	mt_auto_lock mta(&m_InvokingItemListByUse);
	mtInvokingItemDestParamTypeList::iterator itr = m_InvokingItemListByUse.begin();
	for(; itr != m_InvokingItemListByUse.end(); itr++) {
		INVOKING_ITEM_DESTPARAM_TYPE *pInvoInfo = &*itr;
		if(i_ItemUID == itr->InvokingItemUID
			&& i_DestParam == itr->InvokingDestParam)
		{
			if(o_InvokingInfo)
			{
				*o_InvokingInfo = *pInvoInfo;
			}
			return pInvoInfo;
		}
	}
	return NULL;
}

void CInvokingItemManager::GetParamListByUse(ItemUID_t i_ItemUID, InvokingItemDestParamTypeList *o_pInvokingItemDestParamTypeList) {
	if(NULL == o_pInvokingItemDestParamTypeList){
		return;
	}
	mt_auto_lock mta(&m_InvokingItemListByUse);
	mtInvokingItemDestParamTypeList::iterator itr = m_InvokingItemListByUse.begin();
	for(; itr != m_InvokingItemListByUse.end(); itr++) {
		if(i_ItemUID == itr->InvokingItemUID) {
			o_pInvokingItemDestParamTypeList->push_back(*itr);
		}
	}
}

BOOL CInvokingItemManager::GetPossibleInvokingItem(DestParam_t i_DestParam, CFieldIOCPSocket *i_pFISoc, INVOKING_ITEM_DESTPARAM_TYPE * o_pInvokingItemInfo, BOOL i_bCheckEquip/*=TRUE*/) {
	if(NULL == i_pFISoc) {
		return FALSE;
	}

	InvokingItemDestParamTypeList InvokingItemRateList;
	if(i_bCheckEquip) {
		i_pFISoc->m_InvokingItemManager.GetParamRateList(i_DestParam, &InvokingItemRateList);
	}
	else {
		i_pFISoc->m_InvokingItemManager.GetParamRateListByUse(i_DestParam, &InvokingItemRateList);
	}
	if(0 >= InvokingItemRateList.size()) {
		return FALSE;
	}
	
	InvokingItemDestParamTypeList::iterator itrRateList = InvokingItemRateList.begin();
	for(; itrRateList != InvokingItemRateList.end(); itrRateList++)
	{
		INVOKING_ITEM_DESTPARAM_TYPE *pInvokingDestInfo = &*itrRateList;	
		// 2010-07-19 by dhjin, 확률 수식 변경
		Prob100_t Random = RAND100();
//		if(Random < pInvokingDestInfo->InvokingDestParamValue)
		if(Random <= pInvokingDestInfo->InvokingDestParamValue)
		{
			// 확률 및 쿨타임 처리
			if(FALSE == i_bCheckEquip)
			{// 2010-03-04 by cmkwon, 사용류는 사용시 쿨타임체크
				*o_pInvokingItemInfo = *pInvokingDestInfo;
				return TRUE;
			}

			if(FALSE == i_pFISoc->m_ItemManager.CheckCoolingTimeInvokingItem(pInvokingDestInfo->InvokingItemUID))
			{
				*o_pInvokingItemInfo = *pInvokingDestInfo;
				return TRUE;
			}	
		}
	}
	
	return FALSE;
}