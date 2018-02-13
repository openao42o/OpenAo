
//////////////////////////////////////////////////////////////////////
//	PetDataManager.cpp: interface for the CPetManager class.
//
//	Create Info : 2010. 06. 16. shCho & hsLee
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PetDataManager.h"

///////////////////////////////////////////////////////////////////////////////////////
// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
//
// 기존 
//
// 
// CPetDataManager :: CPetDataManager()
// {
// 	Initialize();
// }
// 
// CPetDataManager :: ~CPetDataManager()
// {
// 	Release();
// }
// 
// void CPetDataManager :: Initialize ( void )
// {
// 	vectorPetDataInfo::iterator iter = m_PetDataInfo.begin();
// 	for(;iter!=m_PetDataInfo.end();++iter)
// 	{
// 		iter->vectPetLevelDataInfo.clear();
// 	}
// 	m_PetDataInfo.clear();
// }
// 
// BOOL CPetDataManager :: LoadDBData ( CODBCStatement *i_odbcstmt,  ez_map<INT, ITEM*> *pMapItemInfo )		// 2011-09-06 by hskim, 파트너 시스템 2차
// {
// 
// 	if ( NULL == i_odbcstmt )
// 		return FALSE;
// 
// 	if ( FALSE == LoadDB_PetBaseData( i_odbcstmt ) )
// 		return FALSE;
// 
// 	if ( FALSE == LoadDB_PetLevelData( i_odbcstmt ) )
// 		return FALSE;
// 
// 	return TRUE;
// 	
// }
// 
// 
// BOOL CPetDataManager :: LoadDB_PetBaseData ( CODBCStatement *i_odbcstmt )
// {
// 	if ( NULL == i_odbcstmt )
// 		return FALSE;
// 	
// 	Release();
// 
// 	return CAtumDBHelper::DBLoadPetBaseDataInfo( i_odbcstmt , &m_PetDataInfo );
// }
// 
// BOOL CPetDataManager :: LoadDB_PetLevelData ( CODBCStatement *i_odbcstmt )
// {
// 	if ( NULL == i_odbcstmt )
// 		return FALSE;
// 
// 	return CAtumDBHelper::DBLoadPetLevelDataInfo ( i_odbcstmt , &m_PetDataInfo );
// }
// 
// void CPetDataManager :: Release ( void )
// {
// 	vectorPetDataInfo::iterator iter = m_PetDataInfo.begin();
// 	for(;iter!=m_PetDataInfo.end();++iter)
// 	{
// 		iter->vectPetLevelDataInfo.clear();
// 	}
// 	m_PetDataInfo.clear();
// }
// 
// const tPET_LEVEL_DATA *CPetDataManager::GetPetLevelData ( const INT a_iPetIndex , const INT a_iPetLevel )
// {
// 	const tPET_BASE_ALL_DATA *a_pData = GetPetData(a_iPetIndex);
// 	if(NULL == a_pData)
// 	{
// 		return NULL;
// 	}
// 
// 	vectorPetLevelData::const_iterator cst_iter = a_pData->vectPetLevelDataInfo.begin();
// 	for(; cst_iter != a_pData->vectPetLevelDataInfo.end() ; ++cst_iter )
// 	{
// 		if(cst_iter->Level == a_iPetLevel)
// 		{
// 			return cst_iter;
// 		}
// 	}
// 
// 	return NULL;
// 	
// }
// 
// const tPET_BASEDATA *CPetDataManager::GetPetBaseData ( const INT a_iPetIndex )
// {
// 	vectorPetDataInfo::iterator iter = m_PetDataInfo.begin();
// 	for( ; iter != m_PetDataInfo.end() ; ++iter )
// 	{
// 		if( a_iPetIndex == iter->BaseData.PetIndex )	
// 		{
// 			return &iter->BaseData;
// 		}
// 	}
// 	return NULL;
// }
// 
// const tPET_BASE_ALL_DATA *CPetDataManager::GetPetData( const INT a_iPetIndex )
// {
// 	vectorPetDataInfo::iterator it = m_PetDataInfo.begin();
// 	
// 	for (; it != m_PetDataInfo.end(); ++it )
// 	{
// 		if ( it->BaseData.PetIndex == a_iPetIndex )
// 		{
// 			return it;
// 		}
// 	}
// 	return NULL;
// }
// 
// INT CPetDataManager::GetPetLevelDataFromAttackItemNum ( const INT a_iPetIndex , const INT a_iPetLevel )
// {
// 	const tPET_BASE_ALL_DATA *a_pData = GetPetData(a_iPetIndex);
// 	if(NULL == a_pData)
// 	{
// 		return NULL;
// 	}
// 
// 	vectorPetLevelData::const_iterator cst_iter = a_pData->vectPetLevelDataInfo.begin();
// 	for(; cst_iter != a_pData->vectPetLevelDataInfo.end() ; ++cst_iter )
// 	{
// 		if(cst_iter->Level == a_iPetLevel)
// 		{
// 			return cst_iter->PetAttackIndex;
// 		}
// 	}
// 
// 	return NULL;
// 	
// }

/////////
// 수정

CPetDataManager :: CPetDataManager()
{
	Initialize();
}

CPetDataManager :: ~CPetDataManager()
{
	Release();
}
	
void CPetDataManager :: Clear ( void )
{
	vectorPetDataInfo::iterator iter = m_PetDataInfo.begin();
	for(;iter!=m_PetDataInfo.end();++iter)
	{
		iter->vectPetLevelDataInfo.clear();
	}
	m_PetDataInfo.clear();

	m_KitLevelHP.clear();
	m_KitLevelShield.clear();
	m_KitLevelSP.clear();

	m_KitLevelAGear.clear();
	m_KitLevelBGear.clear();
	m_KitLevelIGear.clear();
	m_KitLevelMGear.clear();
}

void CPetDataManager :: Initialize ( void )
{
	Clear();
}

void CPetDataManager :: Release ( void )
	{
	Clear();
}

BOOL CPetDataManager :: LoadDBData ( CODBCStatement *i_odbcstmt,  ez_map<INT, ITEM*> *pMapItemInfo )		// 2011-09-06 by hskim, 파트너 시스템 2차
{

	if ( NULL == i_odbcstmt )
		return FALSE;

	if ( FALSE == LoadDB_PetBaseData( i_odbcstmt ) )
		return FALSE;

	if ( FALSE == LoadDB_PetLevelData( i_odbcstmt ) )
		return FALSE;

	if ( FALSE == CreateIndexKitLevel(pMapItemInfo) )
		return FALSE;

	if ( FALSE == CreateIndexAutoSkill(pMapItemInfo) )
		return FALSE;

	return TRUE;
	
}

BOOL CPetDataManager :: LoadDB_PetBaseData ( CODBCStatement *i_odbcstmt )
{
	if ( NULL == i_odbcstmt )
		return FALSE;
	
	Release();

	return CAtumDBHelper::DBLoadPetBaseDataInfo( i_odbcstmt , &m_PetDataInfo );
}

BOOL CPetDataManager :: LoadDB_PetLevelData ( CODBCStatement *i_odbcstmt )
{
	if ( NULL == i_odbcstmt )
		return FALSE;

	return CAtumDBHelper::DBLoadPetLevelDataInfo ( i_odbcstmt , &m_PetDataInfo );
}

const tPET_LEVEL_DATA *CPetDataManager::GetPetLevelData ( const INT a_iPetIndex , const INT a_iPetLevel )
{
	const tPET_BASE_ALL_DATA *a_pData = GetPetData(a_iPetIndex);
	if(NULL == a_pData)
	{
		return NULL;
	}

	vectorPetLevelData::const_iterator cst_iter = a_pData->vectPetLevelDataInfo.begin();
	for(; cst_iter != a_pData->vectPetLevelDataInfo.end() ; ++cst_iter )
	{
		if(cst_iter->Level == a_iPetLevel)
		{
			return &*cst_iter;
		}
	}

	return NULL;
	
}

const tPET_BASEDATA *CPetDataManager::GetPetBaseData ( const INT a_iPetIndex )
{
	vectorPetDataInfo::iterator iter = m_PetDataInfo.begin();
	for( ; iter != m_PetDataInfo.end() ; ++iter )
	{
		if( a_iPetIndex == iter->BaseData.PetIndex )	
		{
			return &iter->BaseData;
		}
	}
	return NULL;
}

const tPET_BASE_ALL_DATA *CPetDataManager::GetPetData( const INT a_iPetIndex )
{
	vectorPetDataInfo::iterator it = m_PetDataInfo.begin();
	
	for (; it != m_PetDataInfo.end(); ++it )
	{
		if ( it->BaseData.PetIndex == a_iPetIndex )
		{
			return &*it;
		}
	}
	return NULL;
}

INT CPetDataManager::GetPetLevelDataFromAttackItemNum ( const INT a_iPetIndex , const INT a_iPetLevel )
{
	const tPET_BASE_ALL_DATA *a_pData = GetPetData(a_iPetIndex);
	if(NULL == a_pData)
	{
		return NULL;
	}

	vectorPetLevelData::const_iterator cst_iter = a_pData->vectPetLevelDataInfo.begin();
	for(; cst_iter != a_pData->vectPetLevelDataInfo.end() ; ++cst_iter )
	{
		if(cst_iter->Level == a_iPetLevel)
		{
			return cst_iter->UseWeaponIndex;
		}
	}

	return NULL;
	
}

tPET_LEVEL_DATA *CPetDataManager::GetPetLevel(INT a_iPetIndex, Experience_t PetExperience)
{
	const tPET_BASE_ALL_DATA *a_pData = GetPetData(a_iPetIndex);
	if(NULL == a_pData)
	{
		return NULL;
	}

	vectorPetLevelData::const_iterator cst_iter = a_pData->vectPetLevelDataInfo.begin();
	for( ; cst_iter != a_pData->vectPetLevelDataInfo.end(); cst_iter++)
	{
		if( PetExperience < cst_iter->NeedExp || PET_LEVEL_MAX <= cst_iter->Level )
		{
			return (tPET_LEVEL_DATA *)&*cst_iter;
		}
	}

	return NULL;
}

BOOL CPetDataManager::CreateIndexKitLevel(ez_map<INT, ITEM*> *pMapItemInfo)
{
	ez_map<INT, ITEM *>::iterator itr = pMapItemInfo->begin();

	for(; itr != pMapItemInfo->end(); itr++)
	{
		ITEM *pItem = itr->second;

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOKIT_HP) )
		{
			SPET_KIT_SLOT_ITEM_LEVEL tmSlotItemLevel;

			tmSlotItemLevel.ItemNum = pItem->ItemNum;
			tmSlotItemLevel.Level = pItem->GetParameterValue(DES_PET_SLOT_ITEM_AUTOKIT_HP);

			m_KitLevelHP.push_back(tmSlotItemLevel);
		}

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOKIT_SHIELD) )
		{
			SPET_KIT_SLOT_ITEM_LEVEL tmSlotItemLevel;

			tmSlotItemLevel.ItemNum = pItem->ItemNum;
			tmSlotItemLevel.Level = pItem->GetParameterValue(DES_PET_SLOT_ITEM_AUTOKIT_SHIELD);

			m_KitLevelShield.push_back(tmSlotItemLevel);
		}

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOKIT_SP) )
		{
			SPET_KIT_SLOT_ITEM_LEVEL tmSlotItemLevel;

			tmSlotItemLevel.ItemNum = pItem->ItemNum;
			tmSlotItemLevel.Level = pItem->GetParameterValue(DES_PET_SLOT_ITEM_AUTOKIT_SP);

			m_KitLevelSP.push_back(tmSlotItemLevel);
		}
	}

	// 오름차순으로 정렬

	sort(m_KitLevelHP.begin(), m_KitLevelHP.end(), SSort_KIT_SLOT_ITEM_LEVEL_BY_LEVEL());
	sort(m_KitLevelShield.begin(), m_KitLevelShield.end(), SSort_KIT_SLOT_ITEM_LEVEL_BY_LEVEL());
	sort(m_KitLevelSP.begin(), m_KitLevelSP.end(), SSort_KIT_SLOT_ITEM_LEVEL_BY_LEVEL());

	return TRUE;
}

BOOL CPetDataManager::CreateIndexAutoSkill(ez_map<INT, ITEM*> *pMapItemInfo)
{
	ez_map<INT, ITEM *>::iterator itr = pMapItemInfo->begin();

	for(; itr != pMapItemInfo->end(); itr++)
	{
		ITEM *pItem = itr->second;

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR) )
		{
			SPET_AUTOSKILL_SLOT_ITEM tmItem;

			tmItem.ItemNum = pItem->ItemNum;

			m_KitLevelAGear.push_back(tmItem);
		}

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR) )
		{
			SPET_AUTOSKILL_SLOT_ITEM tmItem;

			tmItem.ItemNum = pItem->ItemNum;

			m_KitLevelBGear.push_back(tmItem);
		}

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR) )
		{
			SPET_AUTOSKILL_SLOT_ITEM tmItem;

			tmItem.ItemNum = pItem->ItemNum;

			m_KitLevelIGear.push_back(tmItem);
		}

		if( pItem->IsExistDesParam(DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR) )
		{
			SPET_AUTOSKILL_SLOT_ITEM tmItem;

			tmItem.ItemNum = pItem->ItemNum;

			m_KitLevelMGear.push_back(tmItem);
		}
	}

	return TRUE;
}

BOOL CPetDataManager::IsAvailableAutoKit(DestParam_t DestParam, INT iLevel, SPET_KIT_SLOT_DATA *pAutoKitSlot)
{
	vectorPetKitLevel *pKitLevel = NULL;

	if( pAutoKitSlot->TriggerValue < 0.0f || pAutoKitSlot->TriggerValue > 100.0f ) 
	{
		return FALSE;
	}

	if( 0 == pAutoKitSlot->ItemNum )
	{
		return TRUE;
	}

	switch( DestParam )
	{
	case DES_PET_SLOT_ITEM_AUTOKIT_HP :
		
		pKitLevel = &m_KitLevelHP;
		
		break;
		
	case DES_PET_SLOT_ITEM_AUTOKIT_SHIELD :
		
		pKitLevel = &m_KitLevelShield;
		
		break;
		
	case DES_PET_SLOT_ITEM_AUTOKIT_SP :
		
		pKitLevel = &m_KitLevelSP;
		
		break;
		
	default :
		
		return FALSE;
	}

	vectorPetKitLevel::const_iterator it = pKitLevel->begin();

	for( ; it != pKitLevel->end(); it++)
	{
		if( pAutoKitSlot->ItemNum == it->ItemNum ) 
		{
			return TRUE;
		}

		if( it->Level > iLevel )		// 더이상의 검색은 의미 없음 (데이타 정렬된 상태)
		{
			return FALSE;
		}
	}

	return FALSE;
}

BOOL CPetDataManager::IsAvailableAutoSkill(USHORT wUnitKind, SPET_AUTOSKILL_SLOT_DATA *pAutoSkillSlot)
{
	int a = 0;
	vectorPetAutoSkill *pvectorPetAutoSkill = NULL;
	
	switch( wUnitKind )
	{
	case UNITKIND_AGEAR :
		
		pvectorPetAutoSkill = &m_KitLevelAGear;
		
		break;
		
	case UNITKIND_BGEAR :
		
		pvectorPetAutoSkill = &m_KitLevelBGear;
		
		break;
		
	case UNITKIND_IGEAR :
		
		pvectorPetAutoSkill = &m_KitLevelIGear;
		
		break;
		
	case UNITKIND_MGEAR :
		
		pvectorPetAutoSkill = &m_KitLevelMGear;
		
		break;

	default:

		return FALSE;
	}

	for(a=0; a<SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; a++)
	{
		if( 0 != pAutoSkillSlot->ItemNum[a] )
		{
			BOOL bTrue = FALSE;
			vectorPetAutoSkill::const_iterator it = pvectorPetAutoSkill->begin();

			for( ; it != pvectorPetAutoSkill->end(); it++)
			{
				if( pAutoSkillSlot->ItemNum[a] == it->ItemNum ) 
				{
					bTrue = TRUE;
					break;
				}
			}

			if( TRUE != bTrue ) return FALSE;
		}
	}

	return TRUE;
}

// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
///////////////////////////////////////////////////////////////////////////////////////