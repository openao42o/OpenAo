#ifndef _MGR_FIELD_ARMOR_COLLECTION_H_
#define _MGR_FIELD_ARMOR_COLLECTION_H_

#include "CollectionManager.h"

class CArmorCollection  : public CCollectionManager
{
public:
	CArmorCollection();
	void CollectionLoad();
	void CollectionUpdate(COLLECTION_INFO *i_pCollectioninfo);
	void CollectionAdd(COLLECTION_INFO* i_pCollectioninfo, BOOL i_bDB_Update = FALSE);
	void CollectionEnchant(COLLECTION_INFO* i_pCollectioninfo, BYTE i_nAddEnchantLevel, UINT i_nRemainSeconds);
	COLLECTION_INFO* ChangeActivedCount(ItemNum_t i_nShapeNum, BOOL i_bIncrease);
	COLLECTION_INFO* GetCollectionInfoPtr(ItemNum_t i_nShapeNum);
	void SetEndTime(COLLECTION_INFO* i_pCollectionInfo);
	void ReFreshRemainSeconds(COLLECTION_INFO* i_pCollectionInfo);
 	ATUM_DATE_TIME GetEndTime(ItemNum_t i_nShapeNum);
 	void CollectionFinish();
	void SendCollectionInfoAllData();

protected:
	COLLECTION_LIST		m_CollectionList;
};

#endif // _MGR_FIELD_ARMOR_COLLECTION_H_
