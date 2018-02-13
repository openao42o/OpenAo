// InvokingItemManager.h: interface for the CInvokingItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVOKINGITEMMANAGER_H__3CD588EF_7074_4ACD_8B8A_501D0660495A__INCLUDED_)
#define AFX_INVOKINGITEMMANAGER_H__3CD588EF_7074_4ACD_8B8A_501D0660495A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCPSocket;

class CInvokingItemManager  
{
public:
	CInvokingItemManager();
	virtual ~CInvokingItemManager();

	void Init();

	void ApplyInvokingItemDestParamByEquip(ITEM_GENERAL * i_pItemG);
	void ApplyEnchantInvokingItemDestParamByEquip(UID64_t i_ItemUID, ITEM * i_pItem);
	void ApplyInvokingItemDestParamByUse(ITEM_GENERAL * i_pItemG);
	void ApplyEnchantInvokingItemDestParamByUse(UID64_t i_ItemUID, ITEM * i_pItem);
	void ReleaseInvokingItemDestParamByUse(ITEM_GENERAL * i_pItemG);
	void DeleteParamTypeInfoByUse(ItemUID_t i_ItemUID);

	void GetParamRateList(DestParam_t i_DestParam, InvokingItemDestParamTypeList *o_pInvokingItemDestParamTypeList);
	void GetParamRateListByUse(DestParam_t i_DestParam, InvokingItemDestParamTypeList *o_pInvokingItemDestParamTypeList);
	INVOKING_ITEM_DESTPARAM_TYPE * GetParamTypeInfoByEquip(INVOKING_ITEM_DESTPARAM_TYPE *o_pInvokingInfo, ItemUID_t i_ItemUID, DestParam_t i_DestParam);
	INVOKING_ITEM_DESTPARAM_TYPE * GetParamTypeInfoByUse(INVOKING_ITEM_DESTPARAM_TYPE *o_InvokingInfo, ItemUID_t i_ItemUID, DestParam_t i_DestParam);
	void GetParamListByUse(ItemUID_t i_ItemUID, InvokingItemDestParamTypeList *o_pInvokingItemDestParamTypeList);

	BOOL GetPossibleInvokingItem(DestParam_t i_DestParam, CFieldIOCPSocket *i_pFISoc, INVOKING_ITEM_DESTPARAM_TYPE * o_pInvokingItemInfo, BOOL i_bCheckEquip = TRUE);

private:
	mtInvokingItemDestParamTypeList		m_InvokingItemListByEquip;
	mtInvokingItemDestParamTypeList		m_InvokingItemListByUse;

};

#endif // !defined(AFX_INVOKINGITEMMANAGER_H__3CD588EF_7074_4ACD_8B8A_501D0660495A__INCLUDED_)
