// InvokingItemInfo.cpp: implementation of the CInvokingItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvokingItemInfo.h"
#include "FieldGlobal.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvokingItemInfo::CInvokingItemInfo()
{
	this->Init();
}

CInvokingItemInfo::~CInvokingItemInfo()
{

}

void CInvokingItemInfo::Init() {
	m_ezmapInvokingDPIdDPList.clear();
	m_ezmapInvokingDPIdDPListByUse.clear();
}

void CInvokingItemInfo::SetDBInvokingWearItemDestParam(InvokingWearItemDestParamList * i_pInvokingWearItemDestParamList) {
	if(i_pInvokingWearItemDestParamList) {
		InvokingWearItemDestParamList::iterator itr = i_pInvokingWearItemDestParamList->begin();
		for(; itr != i_pInvokingWearItemDestParamList->end(); itr++) {
			INVOKING_WEAR_ITEM_DESTPARAM *pInvokingWearItemDP = &*itr;
			
			InvokingWearItemDestParamList *pInvokingWearItemDPList = this->GetInvokingWearItemApplyInfoList(pInvokingWearItemDP->InvokingDestParamID);
			if(pInvokingWearItemDPList) {
				pInvokingWearItemDPList->push_back(*pInvokingWearItemDP);
			}
			else {
				InvokingWearItemDestParamList CopyInvokingWearItemDPList;
				CopyInvokingWearItemDPList.clear();
				CopyInvokingWearItemDPList.push_back(*pInvokingWearItemDP);
				
				if(false == m_ezmapInvokingDPIdDPList.insertEZ(pInvokingWearItemDP->InvokingDestParamID, CopyInvokingWearItemDPList)) {
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CInvokingItemInfo::SetDBInvokingWearItemDestParam# insertEZ error !! InvokingDestParamID(%d) \r\n"
						, pInvokingWearItemDP->InvokingDestParamID);
					return;
				}
			}
		}
	}
}

void CInvokingItemInfo::SetDBInvokingWearItemDestParamByUse(InvokingWearItemDestParamList * i_pInvokingWearItemDestParamListByUse) {
	if(i_pInvokingWearItemDestParamListByUse) {
		InvokingWearItemDestParamList::iterator itr = i_pInvokingWearItemDestParamListByUse->begin();
		for(; itr != i_pInvokingWearItemDestParamListByUse->end(); itr++) {
			INVOKING_WEAR_ITEM_DESTPARAM *pInvokingWearItemDP = &*itr;
			
			InvokingWearItemDestParamList *pInvokingWearItemDPList = this->GetInvokingWearItemApplyInfoListByUse(pInvokingWearItemDP->InvokingDestParamID);
			if(pInvokingWearItemDPList){
				pInvokingWearItemDPList->push_back(*pInvokingWearItemDP);
			}
			else {
				InvokingWearItemDestParamList CopyInvokingWearItemDPList;
				CopyInvokingWearItemDPList.push_back(*pInvokingWearItemDP);
				
				if(false == m_ezmapInvokingDPIdDPListByUse.insertEZ(pInvokingWearItemDP->InvokingDestParamID, CopyInvokingWearItemDPList)) {
					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CInvokingItemInfo::SetDBInvokingWearItemDestParam# insertEZ error !! InvokingDestParamID(%d) \r\n"
						, pInvokingWearItemDP->InvokingDestParamID);
				}
			}
		}
	}
}

InvokingWearItemDestParamList * CInvokingItemInfo::GetInvokingWearItemApplyInfoList(InvokingDestParamID_t i_InvokingDestParamID) {
	return m_ezmapInvokingDPIdDPList.findEZ_ptr(i_InvokingDestParamID);
}

InvokingWearItemDestParamList * CInvokingItemInfo::GetInvokingWearItemApplyInfoListByUse(InvokingDestParamID_t i_InvokingDestParamIDByUse) {
	return m_ezmapInvokingDPIdDPListByUse.findEZ_ptr(i_InvokingDestParamIDByUse);
}