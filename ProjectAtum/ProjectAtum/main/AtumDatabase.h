// AtumDatabase.h: interface for the CAtumDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMDATABASE_H__3842E2DD_C6C4_4D3B_9CFB_18F7EA62E851__INCLUDED_)
#define AFX_ATUMDATABASE_H__3842E2DD_C6C4_4D3B_9CFB_18F7EA62E851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

// 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템		
#include "AtumParam.h"

// typedef enum 
// {
// 	DB_ITEM,
// 	DB_MONSTER_INFO,
// 	DB_MAPOBJECT,
// 	DB_RARE_ITEM,
// 	DB_BUILDINGNPC,
// 	DB_GUILD_MARK,
// 	DB_MAP_INFO,
// 	DB_MIXING_INFO,
// 	DB_MYSTERY_ITEM_DROP,
// 	// 2009-03-04 by bhsohn 럭키 머신 수정안
// 	DB_LUCKYMACHINE,
// 	// end 2009-03-04 by bhsohn 럭키 머신 수정안
// 	// 2010. 02. 11 by ckPark 발동류 장착아이템
// 	DB_INVOKINGWEARITEM_DPNUM,			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
// 	DB_INVOKINGWEARITEM_DPNUM_BY_USE,	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
// 	// end 2010. 02. 11 by ckPark 발동류 장착아이템
// 
// 	// 2010-08-10 by dgwoo 버닝맵 시스템
// 	DB_BURNING_MAP,
// 	// 2010-08-10 by dgwoo 버닝맵 시스템
// 
// 	// 2010-06-15 by shcho&hslee 펫시스템
// 	DB_PET_BASEDATA,						// 2010-06-15 by shcho&hslee 펫시스템 - 펫 기본 정보.
// 	DB_PET_LEVELDATA,						// 2010-06-15 by shcho&hslee 펫시스템 - 펫 레벨업 관련정보.
// 	DB_OPERATOR_ACTION,						// 2010-06-15 by shcho&hslee 펫시스템 - 오퍼레이터 정보
// 	// End 2010-06-15 by shcho&hslee 펫시스템
// 	DB_DISSOLUTION_ITEM,						// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
// 	DB_MONSTER_ADD_TARGET,	
// 
// } DB_TYPE;

// ITEM
// struct DATA_HEADER
// {
// 	DB_TYPE nType;
// 	int nDataCount;
// };

// end 2011. 03. 08 by jskim 인피3차 구현 - 퀘스트 시스템		

// GUILD_MARK_VERSION FILE STRUCT
//----------------------------------------------------------------------------------------------------
//	          8           |				   	  ARRAY_(GUILD_MARK_VERSION):nNumber                      |
//----------------------------------------------------------------------------------------------------
//	          	          |       4     |      4             |      4       |ARRAY_(char):nSizeOfImage|
//----------------------------------------------------------------------------------------------------
//    nType   |  nNumber  |    nVersion | nGuildUniqueNumber | nSizeOfImage |              char       |
//----------------------------------------------------------------------------------------------------

class CINFImageEx;                     // 2011. 10. 10 by jskim UI시스템 변경
struct GUILD_MARK_VERSION
{
	int nVersion;
	int nGuildUniqueNumber;
	int nSizeOfImage;
	CINFImage* pImage;               // 2011. 10. 10 by jskim UI시스템 변경 
	char* pData;
};

typedef std::vector< OperatorInfo* > vectorPreOperatorInfo;

// 2013-02-26 by bhsohn 인게임 조합 검색 처리
struct structResultMixingInfo		// 결과로 검색 정보 
{
	ITEM_MIXING_INFO struMixingInfo;
	char itemname[SIZE_MAX_ITEM_NAME];
	INT LinkItem;		// 아머 변경 카드 소스 인덱스
};

typedef map<INT,CVectorMixingInfo*>			 CMapMixingSearchTable;
typedef map<INT,CVectorMixingInfo*>::iterator CMapMixingSearchTableIterator;

// 2013-06-14 by bhsohn 인게임 조합식 수정
// typedef std::map<std::string, structResultMixingInfo*>	CMapResultMixingInfo;
// typedef std::map<std::string, structResultMixingInfo*>::iterator CMapResultMixingInfoIterator;
typedef vector<structResultMixingInfo*>					CVectorRInfo;					
typedef vector<structResultMixingInfo*>::iterator		CVectorRInfoIterator;

typedef std::map<std::string, CVectorRInfo>				CMapResultMixingInfo;
typedef std::map<std::string, CVectorRInfo>::iterator	CMapResultMixingInfoIterator;
// END 2013-06-14 by bhsohn 인게임 조합식 수정
// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
enum
{
	LOOK_MIXING_NONE	= 0,					// 없음
	LOOK_MIXING_PRIMARY_WEAPON_A,				// 무기 외형 - 기본 무기 A 기어
	LOOK_MIXING_PRIMARY_WEAPON_BMI,				// 무기 외형 - 기본 무기 BMI 기어
	LOOK_MIXING_SECOND_WEAPON_A,				// 무기 외형 - 고급 무기 A 기어
	LOOK_MIXING_SECOND_WEAPON_BMI,				// 무기 외형 - 고급 무기 BMI 기어
	LOOK_MIXING_EFFECT_PRIMARY_WEAPON,			// 무기 외형 - 기본 무기 이펙트
	LOOK_MIXING_EFFECT_SECOND_WEAPON,			// 무기 외형 - 고급 무기 이펙트
	LOOK_MIXING_DEFENSE_B,						// 아머 외형 - B 기어
	LOOK_MIXING_DEFENSE_M,						// 아머 외형 - M 기어
	LOOK_MIXING_DEFENSE_A,						// 아머 외형 - A 기어
	LOOK_MIXING_DEFENSE_I,						// 아머 외형 - I 기어	
	LOOK_MIXING_PET_ITEM,						// 파트너 외형 - 파트너 기간제 외형 변경
////////////////////////////// 외형변경 카드류 //////////////////////////////
	LOOK_MIXING_SHAPE_ITEM,						// 외형 카드 - 
	LOOK_MIXING_SHAPE_EFFECT,					// 이펙트 카드- 	
	LOOK_MIXING_SHAPE_PRIMARY_WEAPON_A,			// 무기 외형 - 기본 무기 A 기어
	LOOK_MIXING_SHAPE_PRIMARY_WEAPON_BMI,		// 무기 외형 - 기본 무기 BMI 기어
	LOOK_MIXING_SHAPE_SECOND_WEAPON_A,			// 무기 외형 - 고급 무기 A 기어
	LOOK_MIXING_SHAPE_SECOND_WEAPON_BMI,		// 무기 외형 - 고급 무기 BMI 기어
	LOOK_MIXING_SHAPE_EFFECT_PRIMARY_WEAPON,	// 무기 외형 - 기본 무기 이펙트
	LOOK_MIXING_SHAPE_EFFECT_SECOND_WEAPON,		// 무기 외형 - 고급 무기 이펙트
	LOOK_MIXING_SHAPE_DEFENSE_B,				// 아머 외형 - B 기어
	LOOK_MIXING_SHAPE_DEFENSE_M,				// 아머 외형 - M 기어
	LOOK_MIXING_SHAPE_DEFENSE_A,				// 아머 외형 - A 기어
	LOOK_MIXING_SHAPE_DEFENSE_I,				// 아머 외형 - I 기어
	LOOK_MIXING_SHAPE_DEFENSE_INIT,				// 아머 외형 - 초기화
	LOOK_MIXING_SHAPE_SECOND_EFFECT_INIT,		// 이펙트 외형 - 고급 무기 초기화 
	LOOK_MIXING_SHAPE_PRIMARY_EFFECT_INIT,		// 이펙트 외형 - 기본 무기 초기화 
	LOOK_MIXING_SHAPE_PET_ITEM,					// 파트너 외형 - 파트너 기간제 외형 변경
	MAX_LOOK_MIXING_ITEM						// 마지막 값
};
// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

class CAtumDatabase  
{
public:
	CAtumDatabase();
	virtual ~CAtumDatabase();
	
	void InitDeviceObjects();
	void RestoreDeviceObjects();
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();

	ITEM *	GetServerItemInfo(int nItemNum);
	void GetServerItemInfo(vectorItemInfoList * pItemList, int nKind);
	ITEM * UpdateItemInfo(ITEM &item);
	ITEM * UpdateSimpleItemInfo(MEX_ITEM_INFO &simpleInfo);
	ITEM * GetServerMonsterTransItemInfo(int nMonUintKind);

	MEX_MONSTER_INFO * CheckMonsterInfo(int nMonType);
	MEX_MONSTER_INFO * UpdateMonsterInfo(MEX_MONSTER_INFO* pMonsterInfo);

	void FindMysteryInfo(INT i_nItemNum,vector<MYSTERY_ITEM_DROP*> *pVecBuildingNPC);
	
	// 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가
	MYSTERY_ITEM_DROP* GetMysteryInfo(INT	i_MysteryItemDropNum, INT i_nItemNum);
	// end 2008-12-29 by bhsohn 럭키 머신 성공 이펙트 추가

	MAPOBJECTINFO * CheckObjectInfo(int nObjType);
	MAPOBJECTINFO * UpdateObjectInfo(MAPOBJECTINFO & obj);

	RARE_ITEM_INFO * GetServerRareItemInfo(int nCodeNum);
	// 2010. 04. 21 by jskim 신규 럭키 머신 구현
	CMapRareItemInfoList GetServerRareItemList();
	//end 2010. 04. 21 by jskim 신규 럭키 머신 구현
	RARE_ITEM_INFO * UpdateRareItemInfo(RARE_ITEM_INFO &rareItem);

	BUILDINGNPC * GetServerBuildingNPCInfoFromNPCIndex(int nNPCIndex);
	BUILDINGNPC * GetServerBuildingNPCInfo(int nBuildingIndex);
	BUILDINGNPC * UpdateBuildingNPCInfo(BUILDINGNPC & buildingInfo);

	// 2013-05-07 by bhsohn 세력포인트 개선안 시스템
	BUILDINGNPC * GetBuildingNPCInfo(int nNPCIndex);
	// END 2013-05-07 by bhsohn 세력포인트 개선안 시스템

	CINFImage*	GetGuildMark(UINT nGuildUniqueNumber);
	char* GetGuildName(UINT nGuildUniqueNumber);
	void CheckGuildMarkVersion(int nGuildUniqueNumber, int nGuildVersion );
	void UpdateGuildMark(UINT nGuildUniqueNumber, int nGuildVersion, char* pData, int nSize);

	MEX_OTHER_GUILD_INFO* GetOtherGuildInfo( int nGuildUniqueNumber );
	void UpdateOtherGuildInfo(MEX_OTHER_GUILD_INFO* pMsg);

	MAP_INFO * GetMapInfo(MapIndex_t nMapIndex);
	void FindBuildingNPCListOccupyShop(int nCityMapIndex, vector<BUILDINGNPC*> *pVecItem);

	int GetItemNameRareString(char* pOutFullName, int nItemNumber, int nPrefix, int nSuffix);

	void ReloadDatabase();

	// 2007-12-17 by bhsohn 조합 가격 표시
	// 조합 가격을 알아온다.
	ITEM_MIXING_INFO* GetMixerPrice(ITEM_MIXING_INFO* pMixInfo);

	// 2009-03-04 by bhsohn 럭키 머신 수정안
	int GetLuckyMachineLen();
	LUCKY_MACHINE_OMI * GetLuckyMachineInfo(int nIdx);
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	// 발동류 데스파람 얻어오기
	void	GetInvokingWearItemDPList( CVectorInvokingWearItemDP* pVec, InvokingDestParamID_t nIdx );
	// 사용류 데스파람 얻어오기
	void	GetInvokingWearItemDPByUseList( CVectorInvokingWearItemDP* pVec, InvokingDestParamID_t nIdx );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경
	void	GetOMICheckSum( const char* szFilePath, BYTE omiCheckSum[32], long* pFileSize );
	// end 2009. 06. 09 by ckPark OMI 게임 시작시 체크섬 계산하도록 변경

	// 2010-06-15 by shcho&hslee 펫시스템
	// 펫 전체 정보 포인터 리턴. ( Pet Index 기준. ) Public.
	tPET_BASE_ALL_DATA *GetPtr_PetAllDataByIndex ( const INT a_PetIndex );

	// 펫 레벨 정보 포인터 리턴.
	tPET_LEVEL_DATA *GetPtr_PetLevelData ( const INT a_PetIndex , const INT a_PetLevel );

	OperatorInfo* GetPtr_OperatorAction( const INT FunctionIndex );
	// End 2010-06-15 by shcho&hslee 펫시스템

	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	tDissolutionItemInfo Get_DissolutionitemInfo( const INT itemNum );
	BOOL Is_DissolutionitemInfo( const INT itemNum );
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현

	// 2010-08-10 by dgwoo 버닝맵 시스템
	BURNING_MAP_INFO *GetPtr_BurningMapInfo(const MapIndex_t i_Index);
	// 2010-08-10 by dgwoo 버닝맵 시스템

	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	int GetMultiTargetIndexSize(int nMonsterIndex);
	MONSTER_MULTI_TARGET GetMultiTargetPosion(int nMonsterIndex, int nPosion);
	vectorMonsterMultiTarget GetvecMultiTargetInfo(int nMonsterIndex);
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	// 2011. 11. 17 by jskim EP4 UI 변경
	void DeleteGuildInfo();
	// end 2011. 11. 17 by jskim EP4 UI 변경

	// 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
	CVectorMixingInfo* GetMixingItemList(int nItemNum);
	void InitMixingItemSearchTable();
	void ReleaseMixingItemSearchTable();
	// end 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	int GetMaterialMixingItemList(int nItemCnt, char* pSearchName, vector<structResultMixingInfo> *o_pVecItemList);
	int GetMaterialMixingItemListIndex(INT nItemCnt, INT nSearchItemNum, vector<structResultMixingInfo> *o_pVecItemList);	
	int AddMaterialShapeItemList(int nItemCnt, ITEM* pItem, vector<structResultMixingInfo> *o_pVecItemList); 
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

	// 아이템 결과물 검색
	void InitResultMixingItemSearchTable();
	void ReleaseResultMixingItemSearchTable();		
	int  AddResultShapeItemList(int nItemCnt, ITEM* pItem, vector<structResultMixingInfo> *o_pVecItemList); // 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색
	int  GetResultMixingItemList(int nItemCnt, char* pItemName, vector<structResultMixingInfo> *o_pVecItemList);	
	int  GetResultMixingItemListIndex(int nItemCnt, INT nItemNum, vector<structResultMixingInfo> *o_pVecItemList);

	// 외형 변경 관련 아이템 전체 타입들
	void InitMixingItemListIndex();
	void AddMixingItemListIndex(ITEM* pAddItem, int nItemShapeType);
	void GetMixingItemListIndex(INT nItemNum, vector<structResultMixingInfo> *o_pVecItemList);
	void ReleaseMixingItemListIndex();	
	
	int GetFindLookChangeIdx(ITEM* pItem);	
	void Find_ItemType_To_FindIdx(int nShapeIdx, vector<INT> *pvecShape);

	ITEM* GetItemInfoLoadItemData(INT nItemNum);
//	ITEM_MIXING_INFO* GetResultMixingItemSearchTable(char* pItemName);
	
	// 자동완성에 해당되는 아이템 이름을 알려준다.
	void GetSearchItemList(char* pSearchItem, std::map<INT, std::string> *o_pVecSearchList); 
	void GetSearchItemList_LookChangeInfo(char* pSearchItem, std::map<INT, std::string> *o_pVecSearchList);

	void SearchStringCull(char* pItemName, vector<string> *o_pVecItemList); // 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리

	// 2013-03-13 by bhsohn 스킬 번호 구조 수정
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	int GetSkillBaseNum(int nItemNum);		// #define SKILL_BASE_NUM(x)	((int)x/10)*10	 // 스킬의 ItemNum에서 BaseNum을 구함
	int GetSkillLevel(int nItemNum);		// #define SKILL_LEVEL(x)		(x - ((int)x/10)*10)				// 스킬의 ItemNum에서 SkillLevel을 구함
	int GetSkillKind(int nItemNum);			// #define SKILL_KIND(x)		((int)x/1000) - ((int)x/10000)*10	// Skill 종류(공격: 0, 방어: 1, 컨트롤: 2, 속성|보조: 3)	
#endif
	int GetSkillKindIndex(int nItemNum);
	int GetSkillBaseSourceIndex(int nSourceIndex);
	// END 2013-03-13 by bhsohn 스킬 번호 구조 수정

	// 2013-05-28 by bhsohn 아머 컬렉션 시스템
	void InitArmorCollectionInfo();
	// END 2013-05-28 by bhsohn 아머 컬렉션 시스템

#ifdef _REWORKED_COLORSHOP
	std::vector<ITEM*> GetColourItemsByReqUnitKind(int ReqUnitKind);
#endif
		
protected:
	BOOL LoadItemData(FILE* fd, int nCount);
	BOOL LoadMonsterInfo(FILE* fd, int nCount);
	BOOL LoadMapObjectInfo(FILE* fd, int nCount);
	BOOL LoadRareItemData(FILE* fd, int nCount);
	BOOL LoadBuildingNPC(FILE* fd, int nCount);
	BOOL LoadGuildMark(FILE* fd, int nCount);
	BOOL LoadMapInfo(FILE* fd, int nCount);
	BOOL LoadMixingInfo(FILE* fd, int nCount);
	BOOL LoadMysteryInfo(FILE* fd, int nCount);
	// 2009-03-04 by bhsohn 럭키 머신 수정안
	BOOL LoadLuckyMachineInfo(FILE* fd, int nCount);
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	BOOL LoadInvokingWearItemDPNum( FILE* fd, int nCount );
	BOOL LoadInvokingWearItemDPNumByUse( FILE* fd, int nCount );
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2010-06-15 by shcho&hslee 펫시스템
	BOOL LoadPetBaseData ( FILE* fd , int nCount );
	BOOL LoadPetLevelData ( FILE* fd , int nCount );
	// 2010-08-10 by dgwoo 버닝맵 시스템
	BOOL LoadBurningMapData ( FILE* fd , int nCount );
	BOOL LoadOperatorData ( FILE* fd , int nCount );
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	BOOL LoadMultiTargetIndexData( FILE* fd , int nCount );
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	tPET_BASE_ALL_DATA *GetPtr_PetAllData ( const INT a_PetUID );

	void DeletePetData ( void );
	// End 2010-06-15 by shcho&hslee 펫시스템
	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	BOOL LoadDissolutionItemData( FILE* fd , int nCount );
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현

	BOOL SaveItemData(FILE* fd);
	BOOL SaveMonsterInfo(FILE* fd);
	BOOL SaveMapObjectInfo(FILE* fd);
	BOOL SaveRareItemData(FILE* fd);
	BOOL SaveBuildingNPC(FILE* fd);
	BOOL SaveGuildMark(FILE* fd);
	BOOL SaveMapInfo(FILE* fd);
	BOOL SaveMixingInfo(FILE* fd);

	// 2010-06-15 by shcho&hslee 펫시스템
	BOOL SavePetBaseData ( FILE* fd );
	BOOL SavePetLevelData ( FILE* fd );
	// End 2010-06-15 by shcho&hslee 펫시스템

protected:
	CMapItemInfoList			m_mapItemInfo;					// 각 Item Data
	CMapItemInfoList			m_mapItemInfoTemp;				// 각 Item Data 임시
	CMapIsLoadingList			m_mapItemGetInfo;				// 각 Item Data - 정보를 요청했는지 
	CMapRareItemInfoList		m_mapRareItemInfo;				// 각 Rare Item Data
	CMapRareItemInfoList		m_mapRareItemInfoTemp;			// 각 Rare Item Data 임시
	CMapIsLoadingList			m_mapRareItemGetInfo;			// 각 Rare Item Data - 정보를 요청했는지 
	CMapMonsterInfoList			m_mapMonsterInfo;				// 각 Monster Data
	CMapMonsterInfoList			m_mapMonsterInfoTemp;			// 각 Monster Data 임시
	CMapIsLoadingList			m_mapMonsterGetInfo;			// 각 Monster Data - 정보를 요청했는지 
	CMapObjectInfoList			m_mapObjectInfo;				// 각 Object Data
	CMapObjectInfoList			m_mapObjectInfoTemp;			// 각 Object Data 임시
	CMapIsLoadingList			m_mapObjectGetInfo;				// 각 Object Data - 정보를 요청했는지 
	CMapBuildingNPCList			m_mapBuildingNPCInfo;			// 각 BUILDINGNPC
	CMapBuildingNPCList			m_mapBuildingNPCInfoTemp;		// 각 BUILDINGNPC 임시
	CMapIsLoadingList			m_mapBuildingNPCGetInfo;		// 각 BUILDINGNPC - 정보를 요청했는지
	CMapGuildMark				m_mapGuildMark;					// 길드 문장 정보
	CMapIsLoadingList			m_mapGuildMarkGetInfo;			// 길드 문장 정보 - 정보를 요청했는지
	CMapOtherGuildInfo			m_mapOtherGuildInfo;			// 다른 캐릭 길드 정보
	CMapIsLoadingList			m_mapOtherGuildInfoGetInfo;		// 다른 캐릭 길드 정보
	CMapMapInfo					m_mapMapInfo;					// 맵 정보(맵이름)
	CVectorMysteryItemDrop		m_VecMysteryItem;				// 미스터리 아이템, 럭키 머신 아이템.
	CVectorMixingInfo			m_VecMixingInfo;				// 조합 정보
	// 2009-03-04 by bhsohn 럭키 머신 수정안
	CVectorLuckyMachine			m_VecLuckyMachine;				// 럭키 머신 아이템 수정안
	// end 2009-03-04 by bhsohn 럭키 머신 수정안

	// 2013-02-26 by bhsohn 인게임 조합 검색 처리
	CMapResultMixingInfo*		m_pMapMapResultMixingInfo;	
	// END 2013-02-26 by bhsohn 인게임 조합 검색 처리


	// 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정
	CMapMixingSearchTable*		m_pMapMixingSearchTable;			//인게임 조합식 검색용 정보.
	// end 2013-01-24 by jhjang 인게임 조합식 임시로 커맨드로 채팅창에 출력할수 있게 수정

	// 2010. 02. 11 by ckPark 발동류 장착아이템
	CVectorInvokingWearItemDP	m_VecInvokingWearItemDPNum;
	CVectorInvokingWearItemDP	m_VecInvokingWearItemDPNumByUse;
	// end 2010. 02. 11 by ckPark 발동류 장착아이템

	// 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색	
	CMapItemInfoList*		m_pMapItemShapeType[MAX_LOOK_MIXING_ITEM];			
	// END 2013-04-11 by bhsohn 인게임 조합창 외형 변경 인첸트 시스템 검색

	// 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기

	MAP_INFO					m_DefMapInfo;

	// end 2009. 01. 12 by ckPark MapInfo 없는 맵은 모선전 정보 안띄우기

	// 2010-06-15 by shcho&hslee 펫시스템
	CVectorPetAllData			m_VecPetAllData;	
	vectorPreOperatorInfo		m_vecOptratorAction;
	// End 2010-06-15 by shcho&hslee 펫시스템

	// 2010-08-10 by dgwoo 버닝맵 시스템
	CVectorBurningData			m_VecBurningMapData;

	// 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	vectorDissolutionItemInfo	m_vecDissolutionItemInfo;
	// end 2010-08-31 by shcho&&jskim, 용해 시스템 구현
	// 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템
	map<INT,vectorMonsterMultiTarget>	m_mapMultiTagetInfo;
	// end 2011. 03. 08 by jskim 인피3차 구현 - 넌 타겟 시스템

};

// 2013-03-13 by bhsohn 스킬 번호 구조 수정
#ifdef C_SKILL_NUMBERING_RENEWAL_BHSOHN
	int SKILL_BASE_NUM(int nItemNum);
	int SKILL_LEVEL(int nItemNum);
	int SKILL_KIND(int nItemNum);	
#endif
	int SKILL_KIND_INDEX(int nItemNum);
	int SKILL_BASE_SOURCEINDEX(int nSourceIndex);
// END 2013-03-13 by bhsohn 스킬 번호 구조 수정

#endif // !defined(AFX_ATUMDATABASE_H__3842E2DD_C6C4_4D3B_9CFB_18F7EA62E851__INCLUDED_)
