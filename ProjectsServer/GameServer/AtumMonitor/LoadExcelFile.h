// LoadExcelFile.h: interface for the CLoadExcelFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADEXCELFILE_H__C6CD4C45_00FE_48CF_BD8C_BA622A09307C__INCLUDED_)
#define AFX_LOADEXCELFILE_H__C6CD4C45_00FE_48CF_BD8C_BA622A09307C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"
#include "AtumDBhelper.h"		// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")

#define MAX_TABLE_NAME	50

typedef struct{
	_bstr_t szTableName;
	int nRecordCount;
}TABLEINFO;

struct SGAME_INFO_TABEL_INFO		// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 
{
	char *	m_szTableName;
	int		m_nCBCount;
	char *	m_szFieldNames[SIZE_MAX_TB_FIELD_COUNT];
	BOOL CheckFieldNameList(vectstring *i_pvectFieldNameList, char *o_pszErrTxt);	// 2010-04-20 by cmkwon, SCMonitor 엑셀 파일 밀어넣기 에러 정보 추가 - 
};
extern SGAME_INFO_TABEL_INFO *FindGameInfoTableInfoByTableName(char *i_szTableName);	// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 

// 2009-07-09 by jwlee, 게임 정보 테이블 데이터 DB에 밀어넣기 툴 구현 -
class CLoadExcelFile  
{
///////////////////////////////////////////////////////////////////////////////
// public member functions
///////////////////////////////////////////////////////////////////////////////
public:
	CLoadExcelFile();
	virtual ~CLoadExcelFile();
	
	// Connect Excel
	BOOL ConnectExcel(char *i_szFileName);
	
	// Loading TableList and Record Count
	BOOL LoadTableList(void);

	// Load ExcelData
	BOOL LoadExcelData(vector<CString> *i_pvectTables);

///////////////////////////////////////////////////////////////////////////////
// private member functions
///////////////////////////////////////////////////////////////////////////////
private:
	// Loading ExcelTableData
	void LoadExcelItem(void);
	void LoadExcelMonsterItem(void);
	void LoadExcelRareItemInfo(void);
	void LoadExcelShop(void);
	void LoadExcelCityTargetWarpMap(void);
	void LoadExcelItemMixingInfo(void);
	void LoadExcelItemMixingElement(void);		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	void LoadExcelEnchantInfo(void);
	void LoadExcelStrategyPointSummonMapIndex(void);
	void LoadExcelArenaInfo(void);
	void LoadExcelArenaMapinfo(void);
	void LoadExcelMysteryItemDrop(void);
	void LoadExcelInfluenceWar(void);
	void LoadExcelOutPostNextWarTime(void);
	void LoadExcelActionByLevel(void);
	void LoadExcelMapInfo(void);
	void LoadExcelBuildingNPC(void);
	void LoadExcelMonster(void);
	void LoadExcelTutorialInfo(void);
	void LoadExcelMapObject(void);
	void LoadExcelLuckyMachine(void);
	void LoadExcelQuestMatching(void);
	void LoadExcelItemMatching(void);
	void LoadExcelItemInfo(void);
	void LoadExceInfluenceRate(void);		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
	void LoadExcelHPAction(void);
	void LoadExcelCinema(void);
	void LoadExcelInfinityMonster(void);
	void LoadExcelRevision(void);
	void LoadExcelInfinityMode(void);
	void LoadExcelTender(void);	
	void LoadExcelInfinityShop(void);					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	void LoadExcelItemParamOverlap(void);				// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	void LoadExcelInvokingWearItemDPNum(void);				// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	void LoadExcelInvokingWearItemDPNumByUse(void);			// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	void LoadExeclLevelAdjustment(void);			// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	void LoadExeclLevelAdjustmentReward(void);		// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	void LoadExeclBurningMap(void);		// 2010-08-05 by dhjin, 버닝맵 - 
	void LoadExeclPetBaseData(void);		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
	void LoadExeclPetLevelData(void);		// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
	void ExeclLoadOperatorData(void);		// 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
	void LoadExeclDissolutionItemData(void);	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
	void LoadExeclMonsterMultiTargetData(void);		// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	void LoadExeclWayPointData(void);
	void LoadExeclWayPointMoveData(void);
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현

	void LoadExeclTriggerMapBuffData(void);		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	void LoadExeclMapTrigger(void);
	void LoadExeclTriggerFunctionCrystal(void);
	void LoadExeclTriggerCrystalGroup(void);
	void LoadExeclTriggerCrystalDestroyGroup(void);
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

	// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	void LoadExeclTriggerFunctionNGCInflWar(void);
	void LoadExeclTriggerNGCInflWarMonsterGroup(void);
	// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템

	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	void LoadExeclTriggerFunctionNGCOutPostList(void);
	void LoadExeclTriggerOutPostBossKillInfoList(void);
	void LoadExeclTriggerCrystalBuffList(void);
	// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템

	// Delete ExcelTableData
	void DeleteExcelData(void);


	// 2009-10-23 by cmkwon, Monitor 엑셀파일 밀어넣기시 체크 추가 - 
	BOOL GetFieldNameList(vectstring *o_pvectFieldNameList, _RecordsetPtr i_pRecordSet);
	
///////////////////////////////////////////////////////////////////////////////
// public member variables
///////////////////////////////////////////////////////////////////////////////
public:
	vector<ITEM>				vectItem;
	vector<MONSTER2ITEM>		vectMonsterItem;
	vector<RARE_ITEM_INFO>		vectRareItemInfo;
	vectSHOP_INFO				vectShopInfo;
	vectCITY_TARGET_WARPMAP		vectCityTargetWarpMap;
	vector<ITEM_MIXING_INFO_TOOL>		vectItemMixingInfo;			// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	vector<ITEM_MIXING_ELEMENT_TOOL>	vectItemMixingElement;		// 2012-02-20 by hskim, DB 정규화 - ItemMixingInfo
	vector<ENCHANT_INFO>		vectEnchantInfo;
	vectSTRATEGYPOINT_SUMMON_MAPINDEX	vectStrategyPointSummonMapIndex;

	vectorSDBARENA_INFO			vectArenaInfo;
	vectorSDBARENA_MAPINFO		vectArenaMapInfo;
	vectMYSTERY_ITEM_DROP		vectMysteryItem;
	vectDB_INFLUENCE_WAR_INFO	vectInfluenceWarInfo;
	vectSOutPostInfo			vectOutPostInfo;
	vectSACTION_BY_LEVEL_DB		vectActoinByLevel;
	vectorMAP_INFO				vectMapInfo;
	vector<BUILDINGNPC>			vectBuildingNpc;
	vectMONSTER_DATA			vectMonsterInfo;
	vectSTutorialInfo			vectTutorialInfo;
	vectMAPOBJECT				vectMapObject;
	vectLUCKY_MACHINE			vectLuckyMachine;
	vectSQUEST_MATCHING			vectQuestMatching;
	vectSITEM_MATCHING			vectItemMatching;
	vectItem_Info				vectItemInfo;
	vectSTI_INFLUENCERATE		vectInfluenceRate;		// 2009-09-16 by cmkwon, 세력 초기화시 어뷰징 방지 구현 - ti_InfluenceRate 테이블 추가, 
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - Excel -> DB 밀기
	vectHPAction				HPActionList;
	vectorCinemaInfo			CinemaInfoList;
	vectorRevisionInfo			RevisionInfoList;
	vectorInfinityMonsterInfo	InfinityMonsterInfoList;
	vectorInfinityModeInfo		InfinityModeInfoList;
	vectorTenderInfo			TenderInfoList;
	vectorInfinityShopInfo		InfinityShopInfoList;		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 상점
	vectSITEM_PARAM_OVERLAP		ItemParamOverlapList;		// 2010-01-18 by cmkwon, 아이템 사용시 Parameter 중복 체크 시스템 구현 - 
	InvokingWearItemDestParamList InvokingItemDPList;		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	InvokingWearItemDestParamList InvokingItemDPByUseList;		// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	vectorInfinity_DifficultyInfo_Monster  LevelAdjustmentList;		// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	vectorInfinity_DifficultyInfo_Bonus  LevelAdjustmentRewardList;	// 2010-05-24 by shcho, 인피니티 난이도 조절 - 인피니티 난이도 조정에 관한 툴 업데이트
	BurningMapInfoList			BurningMapList;			// 2010-08-05 by dhjin, 버닝맵 - 
	vectorPetBaseData			PetBaseData;			// 2010-06-15 by shcho&hslee 펫시스템 - 팻 기본 정보 엑셀 밀기
	vectorPetLevelData			PetLevelData;			// 2010-06-15 by shcho&hslee 펫시스템 - 팻 레벨 정보 엑셀 밀기
	vectorOperatorInfo			OperatorDataList;		// 2010-06-15 by shcho&hslee 펫시스템 - DB 밀기 추가 작업 펫 오퍼레이터 정보.
	vectorDissolutionItemInfo	DissolutionIteminfo;	// 2010-08-31 by shcho&jskim 아이템용해 시스템 - 용해 아이템 정보.
	vectorMonsterMultiTarget	MonsterMultiTargetList;		// 2011-03-17 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
	// start 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	vectorWayPoint				WayPointList;
	vectorWayPointMove			WayPointMoveList;
	// end 2011-05-23 by hskim, 인피니티 3차 - 웨이포인트 구현
	vectorTriggerMapBuff		TriggerMapBuffList;		// 2011-10-10 by hskim, EP4 [트리거 시스템] - 화산재 / 모래 폭풍
	vector<TABLEINFO>			m_vectTableInfo;
	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	vectorTriggerMap					m_vectTriggerMapList;
	vectorTriggerFunctionCrystal		m_vectTriggerFunctionCrystalList;
	vectorTriggerCrystalGroup			m_vectTriggerCrystalGroupList;
	vectorTriggerCrystalDestroyGroup	m_vectTriggerCrystalDestroyGroupList;
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	// start 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	vectorTriggerFunctionNGCInflWar		m_vectTriggerFunctionNGCInflWarList;
	vectorTriggerNGCInflWarMonsterGroup	m_vectTriggerNGCInflWarMonsterGroupList;
	// end 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
	// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템
	vectorTriggerFunctionNGCOutPost		m_vectTriggerFunctionNGCOutPostList;
	vectorTriggerOutPostBossKillInfo	m_vectTriggerOutPostBossKillInfoList;
	vectorTriggerCrystalBuff			m_vectTriggerCrystalBuff;

///////////////////////////////////////////////////////////////////////////////
// private member variables
///////////////////////////////////////////////////////////////////////////////
private:
	char szStrCon[MAX_PATH];
	char szFileName[MAX_PATH];
	char m_szStrTable[MAX_PATH];
	char m_szStrQuery[MAX_PATH];

	_ConnectionPtr	m_Connection;
	_RecordsetPtr	m_RecordSet;
	_RecordsetPtr	m_Sheetname;
};

#endif // !defined(AFX_LOADEXCELFILE_H__C6CD4C45_00FE_48CF_BD8C_BA622A09307C__INCLUDED_)
