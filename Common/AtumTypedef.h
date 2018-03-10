// Copyright[2002] MasangSoft
#ifndef COMMON_ATUMTYPEDEF_H_
#define COMMON_ATUMTYPEDEF_H_
#include "mt_stl.h"

class CEnemyData;
class CMonsterData;
class CObjectChild;
class DSound3D;
class CQuest;
class CINFImage;
class CItemInfo;
struct GUILD_MARK_VERSION;
struct MAP_INFO;


typedef vector<CEnemyData *>                CVecEnemyList;            // Block 단위로 다루는 Enemy List의 Vector
typedef vector<CEnemyData *>::iterator        CVecEnemyIterator;        // Block 단위로 다루는 Enemy List의 Vector iterator 
typedef vector<CMonsterData *>                CVecMonsterList;        // Block 단위로 다루는 Monster List의 Vector
typedef vector<CMonsterData *>::iterator    CVecMonsterIterator;    // Block 단위로 다루는 Monster List의 Vector iterator
typedef map<INT,CEnemyData *>                CMapEnemyList;            // Enemy List의 Map
typedef map<INT,CEnemyData *>::iterator        CMapEnemyIterator;        // Enemy List의 Map iterator
typedef map<INT,CMonsterData *>                CMapMonsterList;        // Monster List의 Map
typedef map<INT,CMonsterData *>::iterator    CMapMonsterIterator;    // Monster List의 Map iterator
typedef map<INT,CObjectChild *>                CMapObjectList;            // 렌더링 되어질 Object List의 Map
typedef vector<CObjectChild*>                vectorCObjectChildPtr;
typedef map<INT,CObjectChild *>::iterator    CMapObjectIterator;        // 렌더링 되어질 Object List의 Map iterator
//typedef map<int, DSound3D *>                CMapSound3DList;        // effect sound file map
//typedef map<int, DSound3D *>::iterator        CMapSound3DIterator;        // effect sound file map


typedef map<int, ITEM*>                        CMapItemInfoList;            // 아이템 정보
typedef map<int, ITEM*>::iterator            CMapItemInfoIterator;
typedef map<int, RARE_ITEM_INFO*>            CMapRareItemInfoList;            // 레어 아이템 정보
typedef map<int, RARE_ITEM_INFO*>::iterator    CMapRareItemInfoIterator;
typedef map<int, MEX_MONSTER_INFO *>        CMapMonsterInfoList;// 몬스터 정보
typedef map<int, MEX_MONSTER_INFO *>::iterator CMapMonsterInfoIterator;    
typedef map<int, MAPOBJECTINFO *>            CMapObjectInfoList;            // 오브젝트 정보
typedef map<int, MAPOBJECTINFO *>::iterator    CMapObjectInfoIterator;
typedef map<int, BOOL>                        CMapIsLoadingList;            // 각 Item Data - 정보를 요청했는지
typedef map<int, BOOL>::iterator            CMapIsLoadingIterator;        
typedef map<int, BUILDINGNPC*>                CMapBuildingNPCList;            // building info
typedef map<int, BUILDINGNPC*>::iterator    CMapBuildingNPCIterator;        
typedef map<UINT, GUILD_MARK_VERSION*>        CMapGuildMark;                // 길드 마크
typedef map<UINT, GUILD_MARK_VERSION*>::iterator CMapGuildMarkIterator;
typedef map<int, MEX_OTHER_GUILD_INFO*>        CMapOtherGuildInfo;
typedef map<int, MEX_OTHER_GUILD_INFO*>::iterator CMapOtherGuildInfoIterator;
typedef map<USHORT, MAP_INFO*>                CMapMapInfo;
typedef map<USHORT, MAP_INFO*>::iterator    CMapMapInfoIterator;
typedef map<USHORT, ITEM_MIXING_INFO*>        CMapMixingInfo;
typedef map<USHORT, ITEM_MIXING_INFO*>::iterator CMapMixingInfoIterator;


//typedef map<LONGLONG, ITEM_ENCHANT*>        CMapItemEnchantList;        // enchant info
//typedef map<LONGLONG, ITEM_ENCHANT*>::iterator CMapItemEnchantIterator;
typedef map<UID64_t, CItemInfo*>            CMapItemInventory;            // new inventory
typedef map<UID64_t, CItemInfo*>::iterator    CMapItemInventoryIterator;
typedef map<int, CItemInfo*>                CMapItemWindowInventory;            // new inventory
typedef map<int, CItemInfo*>::iterator        CMapItemWindowInventoryIterator;
typedef vector<CItemInfo*>                    CVectorItemInfo;            // store용
typedef vector<CItemInfo*>::iterator        CVectorItemInfoIterator;

typedef ez_map<INT, CQuest*>                CMapQuest;
typedef ez_map<INT, CQuest*>::iterator        CMapQuestIterator;
typedef map<int,MEX_QUEST_INFO*>            CMapCharacterQuest;
typedef map<int,MEX_QUEST_INFO*>::iterator    CMapCharacterQuestIterator;

typedef vector<WARP_TARGET_MAP_INFO_4_EXCHANGE*> CVectorWarpTargetInfo;
typedef vector<WARP_TARGET_MAP_INFO_4_EXCHANGE*>::iterator CVectorWarpTargetInfoIterator;

typedef vector<MEX_CHANNEL_INFO*>                CVectorChannel;
typedef vector<MEX_CHANNEL_INFO*>::iterator        CVectorChannelIterator;

typedef vector<ITEM_MIXING_INFO*>            CVectorMixingInfo;                    // 2006-06-22 by ispark
typedef vector<ITEM_MIXING_INFO*>::iterator CVectorMixingInfoIterator;

typedef vector<ITEM*>                        vectorItemInfoList;                    // 2006-07-31 by ispark
//typedef map<INT,MYSTERY_ITEM_DROP*>            CMapMysteryItemDrop;                // 2008-11-04 by dgwoo
typedef vector<MYSTERY_ITEM_DROP*>                CVectorMysteryItemDrop;                // 2008-11-04 by dgwoo
typedef vector<MYSTERY_ITEM_DROP*>::iterator    CVectorMysteryItemDropIterator;

// 2009-03-04 by bhsohn 럭키 머신 수정안
typedef vector<LUCKY_MACHINE_OMI*>                CVectorLuckyMachine;    
typedef vector<LUCKY_MACHINE_OMI*>::iterator    CVectorLuckyMachineIterator;
// end 2009-03-04 by bhsohn 럭키 머신 수정안

// 2010. 02. 11 by ckPark 발동류 장착아이템
typedef vector<INVOKING_WEAR_ITEM_DESTPARAM*>            CVectorInvokingWearItemDP;
typedef vector<INVOKING_WEAR_ITEM_DESTPARAM*>::iterator    CVectorInvokingWearItemDPIt;
// end 2010. 02. 11 by ckPark 발동류 장착아이템


typedef vector<tPET_BASE_ALL_DATA*>                CVectorPetAllData;
typedef vector<tPET_BASE_ALL_DATA*>::iterator    CVectorPetAllDataItr;


// 2010-08-10 by dgwoo 버닝맵 시스템
typedef vector<BURNING_MAP_INFO*>                CVectorBurningData;
typedef vector<BURNING_MAP_INFO*>::iterator        CVectorBurningDataItr;

#endif // _ATUM_TYPEDEF_H__32E91AE4_C7F3_424D_B983_17488DDA215B__INCLUDED_endif
#endif // COMMON_ATUMTYPEDEF_H_
