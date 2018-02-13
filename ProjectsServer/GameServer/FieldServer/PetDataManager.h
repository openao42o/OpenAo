//////////////////////////////////////////////////////////////////////
//	PetDataManager.h: interface for the CPetManager class.
//
//	Create Info : 2010. 06. 16. shCho & hsLee
//
//////////////////////////////////////////////////////////////////////

#ifndef __PET_DATA_MANAGER__
#define __PET_DATA_MANAGER__

//#pragma once

#include "AtumParam.h"
#include "AtumFieldDBManager.h"

class CPetDataManager
{
	public :
		friend CAtumDBHelper;

	public :
				 CPetDataManager();
		virtual ~CPetDataManager();

	void Clear(); // 2011-09-06 by hskim, 파트너 시스템 2차	
		void Initialize ( void );
		void Release ( void );

		BOOL LoadDBData ( CODBCStatement *i_odbcstmt, ez_map<INT, ITEM*> *pMapItemInfo);		// 2011-09-06 by hskim, 파트너 시스템 2차	
		BOOL LoadDB_PetBaseData ( CODBCStatement *i_odbcstmt );
		BOOL LoadDB_PetLevelData ( CODBCStatement *i_odbcstmt );

		const tPET_BASEDATA *GetPetBaseData ( const INT a_iPetIndex );
		const tPET_LEVEL_DATA *GetPetLevelData ( const INT a_iPetIndex , const INT a_Level );
		const tPET_BASE_ALL_DATA *GetPetData ( const INT a_iPetIndex );

		// 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리(펫 공격 아이템 넘버 구하는 함수 추가)
		INT GetPetLevelDataFromAttackItemNum ( const INT a_iPetIndex , const INT a_iPetLevel );
		// END 2010-06-15 by shcho&hslee 펫시스템 - 펫 공격 처리(펫 공격 아이템 넘버 구하는 함수 추가)

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정

	BOOL CreateIndexKitLevel(ez_map<INT, ITEM*> *pMapItemInfo);
	BOOL CreateIndexAutoSkill(ez_map<INT, ITEM*> *pMapItemInfo);

	BOOL IsAvailableAutoKit(DestParam_t DestParam, INT iLevel, SPET_KIT_SLOT_DATA *pAutoKitSlot);
	BOOL IsAvailableAutoSkill(USHORT wUnitKind, SPET_AUTOSKILL_SLOT_DATA *pAutoSkill);

	tPET_LEVEL_DATA *GetPetLevel(INT a_iPetIndex, Experience_t PetExperience);		// 현재 경험치에 맞는 레벨을 돌려준다

	// end 2011-08-17 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	///////////////////////////////////////////////////////////////////////////////////////

	private :
		vectorPetDataInfo m_PetDataInfo;

	///////////////////////////////////////////////////////////////////////////////////////
	// start 2011-09-06 by hskim, 파트너 시스템 2차 - 자료 구조 결정

	vectorPetKitLevel m_KitLevelHP;
	vectorPetKitLevel m_KitLevelShield;
	vectorPetKitLevel m_KitLevelSP;

	vectorPetAutoSkill m_KitLevelAGear;
	vectorPetAutoSkill m_KitLevelBGear;
	vectorPetAutoSkill m_KitLevelIGear;
	vectorPetAutoSkill m_KitLevelMGear;

	// end 2011-09-06 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	///////////////////////////////////////////////////////////////////////////////////////

};

#endif