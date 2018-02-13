// InvokingItemInfo.h: interface for the CInvokingItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVOKINGITEMINFO_H__A13E40F4_25CB_4164_B675_4E1EEF42CA86__INCLUDED_)
#define AFX_INVOKINGITEMINFO_H__A13E40F4_25CB_4164_B675_4E1EEF42CA86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInvokingItemInfo  
{
public:
	CInvokingItemInfo();
	virtual ~CInvokingItemInfo();
	void Init();
	void SetDBInvokingWearItemDestParam(InvokingWearItemDestParamList * i_pInvokingWearItemDestParamList);
	void SetDBInvokingWearItemDestParamByUse(InvokingWearItemDestParamList * i_pInvokingWearItemDestParamByUseList);
	
	InvokingWearItemDestParamList * GetInvokingWearItemApplyInfoList(InvokingDestParamID_t i_InvokingDestParamID);
	InvokingWearItemDestParamList * GetInvokingWearItemApplyInfoListByUse(InvokingDestParamID_t i_InvokingDestParamIDByUse);
	
	
protected:
	ezmapInvokingDPIdDPList		m_ezmapInvokingDPIdDPList;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	ezmapInvokingDPIdDPList		m_ezmapInvokingDPIdDPListByUse;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
};

#endif // !defined(AFX_INVOKINGITEMINFO_H__A13E40F4_25CB_4164_B675_4E1EEF42CA86__INCLUDED_)
