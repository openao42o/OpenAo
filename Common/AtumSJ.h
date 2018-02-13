#pragma once

#include "AtumParam.h"
#include "AtumTime.h"

constexpr auto STORE_KEEPING_COST = 30;
constexpr auto DAMAGE_KIND_ANGLE_DISTANCE = 100.0f;	// Target five variables for each comparison, @CalcDamageKind ()

struct LEVEL_EXPERIENCE
{
	INT				Level;
	Experience_t	ExperienceIncreament;		// (Up to that level of experience) can not be used because (best experience at that level -1), inaccurate!
	Experience_t	Experience;					// Initial experience of (its level -1)
	Experience_t	ExperienceLossOnDeath;
	INT				PenaltySPIOnDead;
};

// start 2011-11-15 by hskim, EP4 [트리거 시스템] - 버닝맵 경험치 추가
struct BURNING_MAP_LEVEL_ADD_EXPERIENCE
{
	INT				Level;						// 레벨
	FLOAT			IncrementPercent;			// 추가 경험치량 (퍼센트)
};
// end 2011-11-15 by hskim, EP4 [트리거 시스템] - 버닝맵 경험치 추가

struct GEAR_STAT_CHANGE_VALUE
{
	USHORT		UnitKind;
	GEAR_STAT	GearStat1;
};
struct GEAR_AUTO_STAT_VALUE_PER_LEVEL
{
	INT			AutoStatType1;
	INT			Level;
	GEAR_STAT	GearStat1;
};

// Level 혹은 Stat에 대한 각종 값
struct LEVEL_STAT_VALUE
{
	INT		LevelOrStat;
	double	AttackPart_AttackIncreaseRate;			// 공격 스탯에 의한 attack 증가율, @GetAttackC()
	double	AttackPart_AttackIncreaseProbability;	// 명중률 - 공격스탯에 의한 공격확률과 동일한 기능을 가지는 추가 확률
	INT		AttackPart_PierceAttackProbability;		// 피어스율(256확률) - 상대방 방어율에서 이 값을 차감한것이 실제 상대방 방어율이 된다.
	double	DefensePart_DefenseDecreaseRate;		// 방어 스탯에 의한 defense 감소분, @GetDefenseC()
	double	ShieldPart_DP;							// ShieldPart에 대한 Shield(DP)값
	double	DodgePart_DefenseProbabilityRate;		// 회피 스탯에 의한 방어확률의 비율, @GetDefenseProbabilityC()
	INT		HPOfLevel;								// Level에 대한 HP
	INT		Level_MonsertPierceAttackProbability;	// MonsterLevel에 대한 몬스터 PierceAttack확률
};

struct STATUS_NAME
{
	BYTE		Status;				// STATUS_XXX
	const char	*StatusNameEN;
	const char	*StatusNameKO;
};

struct ITEMKIND_STRING				// 2006-07-28 by cmkwon
{
	BYTE		byItemKind0;	
	char		*szItemKindString;		// 2006-07-28 by cmkwon, ITEMKIND_XXX --> "ITEMKIND_XXX"로 변환
	char		*szItemKindName;		// 2006-07-28 by cmkwon, 유저에게 보여주는 ItemKindString
	BOOL		bIsBazaarItemKind;		// 2006-07-28 by cmkwon, 개인 상점에서 거래 가능 ITEMKIND_XXX
};
extern const ITEMKIND_STRING g_arrItemKindString[];

extern setString				g_setAllLetterList;		// 2007-05-04 by cmkwon

struct CParamFactor;
class CODBCStatement;
struct MEX_TARGET_INFO;

///////////////////////////////////////////////////////////////////////////////
/// \class		CAtumSJ
///
/// \brief		각종 계산식 구하는 Class - 기획 사항 적용
/// \author		kelovon
/// \version
/// \date		2004-04-10 ~ 2004-04-10
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CLocalization;

namespace AtumSJ
{
#ifdef _ATUM_SERVER
	// 각종 수치 데이터 로딩
	BOOL LoadData(CODBCStatement *i_pOdbcStmt);
	vectorMAP_INFO *GetVectPtrALL_MAP_INFO();
#endif // _ATUM_SERVER

	// 맵 정보
	const MAP_INFO* GetMapInfo(MapIndex_t i_nMapIndex);
	const char* GetMapName(MapIndex_t i_nMapIndex);

	// 레벨 & 경험치 관련
	const LEVEL_EXPERIENCE* GetLevelExperience(int level);
	float GetBurningMapAddExperience(int level);		// start 2011-11-15 by hskim, EP4 [트리거 시스템] - 버닝맵 경험치 추가
	Experience_t GetInitialExperienceOfLevel(int level);
	float GetPercentageOfExperience(Experience_t exp, int level);
	Experience_t GetExperienceFromPercentage(float i_fPercentage, int level);

	// 캐릭터 속성 관련
	USHORT GetTransport(CHARACTER *pCharacter);				// 운반력
	float GetSpeed(CHARACTER *pCharacter, ITEM *pItem);		// 이동 속도

	// 2007-11-02 by cmkwon, 중량 구하는 시스템 변경 - 리턴값 자료형 변경(<--int)
	float GetItemWeight(ITEM *i_pItem, int i_nCountableItemCount, float i_fDesWeight = 0.0f);	// 아이템의 무게를 반환, countable item인 경우만 i_nCountableItemCount가 의미를 가짐
	BYTE GetStatusByPropensity(INT i_nPropensity);
	const char*  GetStatusByLevel(BYTE i_byLevel);				// 2008-06-20 by dhjin, EP3 유저정보옵션 -
	const char* GetStatusNameKO(BYTE i_nStatus);
	const char* GetStatusNameEN(BYTE i_nStatus);

	// 공격 관련
	BOOL CheckTargetErrorAngle(CHARACTER* i_pAttackCharacter, D3DXVECTOR3& i_ServerAttackPosition, D3DXVECTOR3& i_ServerTargetPosition, D3DXVECTOR3& i_ClientTargetPosition, float& o_fServerDistance);
	float GetAttackProbabilityC(ITEM *pItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, Stat_t i_nAttackPart, BYTE i_byLevel, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선 - // 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// 캐릭터의 공격 확률
	float GetDefenseProbabilityC(CHARACTER *pCharacter, ITEM *pDefenseItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선 - // 캐릭터의 방어 확률
	float GetAttackProbabilityM(const MONSTER_INFO *a_pMonsterInfo, const ITEM *a_pItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);	// 몬스터의 공격 확률	// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (몬스터 공격력 추가 수정, Pierce확률과 적중률 적용. )
	float GetDefenseProbabilityM(const MONSTER_INFO *a_pMonster, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);	// 몬스터의 방어 확률 // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 확장 정보 인자 추가.

	float GetAttackC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가		// 2013-05-09 by hskim, 세력 포인트 개선 - // 캐릭터의 공격력
	float GetMinAttackC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선 - // 캐릭터의 최소공격력
	float GetMaxAttackC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선 - // 캐릭터의 최대공격력
	
	// returns defense percentage in [0, 100]
	float GetDefenseC(CHARACTER *pCharacter, const CParamFactor *pParamFactor,
		bool bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);		
	
	float GetAttackM(const ITEM *a_pItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);							// 몬스터의 공격력 // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 확장 정보 인자 추가.
	float GetMinAttackM(const ITEM *a_pMonsterAttackItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);			// 몬스터의 최소 공격력 // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 확장 정보 인자 추가.
	float GetMaxAttackM(const ITEM *a_pMonsterAttackItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);			// 몬스터의 최대 공격력 // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 확장 정보 인자 추가.
	float GetDefenseM(const MONSTER_INFO *a_pMonster, BOOL i_bIsArenaServer = FALSE, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);		// 몬스터의 방어력 // 2009-12-24 by cmkwon, 몬스터 Defense를 인피니티서버(아레나포함)에서 고정값으로 처리 - // 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) ) - 몬스터 확장정보 인자 추가.

	// 2009-12-17 by cmkwon, 데미지 계산식 관련한 필드(방어력,회피,피어스,확률)에 255이상 값을 설정 가능하게 수정 - 무기 인자 추가
	float GetPierceAttackProbabilityM(const int i_MonsterLevel, const ITEM *i_pAttItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);	// 몬스터의 PierceAttack확률 // 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (몬스터 공격력 추가 수정, Pierce확률과 적중률 적용. )

	float GetMinAttackPerSecondC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선 - // 캐릭터의 초당 최소 공격력
	float GetMaxAttackPerSecondC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가	// 2013-05-09 by hskim, 세력 포인트 개선 - // 캐릭터의 초당 최대 공격력

	// 아이템 속성 관련
	int GetTotalPrimaryShotCountPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);	// 단위 Reattack Time 동안 쏠 수 있는 1형 무기의 최대 총알 수
	int GetTotalSecondaryShotCountPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);	// 단위 Reattack Time 동안 쏠 수 있는 2형 무기의 최대 총알 수
	int GetTotalPrimaryShotNumPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);		// 단위 Reattack Time 동안 쏠 수 있는 1형 무기의 최대 발사 횟수
	int GetTotalSecondaryShotNumPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);	// 단위 Reattack Time 동안 쏠 수 있는 2형 무기의 최대 발사 횟수
	int GetSecondaryMultiNumCount(ITEM *pItem, const CParamFactor *pParamFactor);		// 한 번에 나가는 총알 수
	DWORD GetReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);				// 재공격 시간
	float GetAttackRange(ITEM *pItem, const CParamFactor *pParamFactor);				// 공격 범위
	float GetOverheatTime(ITEM *pItem, const CParamFactor *pParamFactor);				// 과열 시간
	float GetShotCountReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);		// shotnum 이 각각 발사시 사이 시간
	float GetShotNumReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);		// ReattackTime 동안 ShotNum 만큼 발사 될때 사이 간격 시간
	float GetPrepareTime(ITEM *pItem, const CParamFactor *pParamFactor);				// 발동 시간
	int GetMultiTargetNum(ITEM *pItem, const CParamFactor *pParamFactor);				// Multi Target 수
	float GetAttackRangeAngle(ITEM *pItem, const CParamFactor *pParamFactor);			// 화망 각도
	UINT GetItemSellingPriceAtShop(ITEM* pItem);									// 아이템 상점에 팔때 가격
	float GetItemSellingPriceRate();												// 아이템 상점에 팔때 비율
	float GetExplosionRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// 폭파 반경
	int GetCountMineAtATime(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// 한 캐릭터당 뿌려질 수 있는 최대 마인의 개수
	USHORT GetReactionRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// 반응 반경
	float GetMinAttackPerSecond(ITEM *i_pRealItem);								// 아이템의 초당 최소 공격력
	float GetMaxAttackPerSecond(ITEM *i_pRealItem);								// 아이템의 초당 최대 공격력
	float GetPrimaryRadarRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// 1형 레이더 사정 거리
	float GetSecondaryRadarRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);	// 2형 레이더 사정 거리
	float GetCircularAngle(ITEM *i_pItem, const CParamFactor *i_pParamFactor);				// 하이퍼 무빙(반응속도), 2005-12-02 by cmkwon
	float GetEngineRangeAngle(ITEM *i_pItem, const CParamFactor *i_pParamFactor);			// 하이퍼 무빙(선회각), 2005-12-02 by cmkwon
	float GetEngineBoosterRangeAngle(ITEM *i_pItem, const CParamFactor *i_pParamFactor);	// 하이퍼 무빙(부스터 선회각), 2005-12-02 by cmkwon
	float GetWarHeadSpeed(ITEM *i_pItem, const CParamFactor *i_pParamFactor);				// 176, 2007-06-11 by cmkwon, 탄두의 속도


	// 경험치 관련
	float GetMonsterDeadExpWeight(int i_nCharacterLevel, int i_nMonsterLevel);	// 몬스터가 죽었을 때 분배하는 경험치에 대한 weight 관련
	float GetPartyBonusExpWeight(int nPartyMemberCnts);							// 

	// 몬스터 드랍 아이텝 습득 시 레벨 차에 의한 습득 여부
	BOOL CanPickUpDropMoney(int i_nCharacterLevel, MONSTER_INFO *i_pMonInfo);
	BOOL CanPickUpDropItem(int i_nCharacterLevel, MONSTER_INFO *i_pMonInfo);
	BOOL CanPickUpDropRare(int i_nCharacterLevel, MONSTER_INFO *i_pMonInfo);

	// 보너스 스탯 적용 수치 관련
	const GEAR_STAT_CHANGE_VALUE* GetGearStatChangeValueStruct(USHORT i_UnitKind);
	Stat_t GetGearStatChangeValue(USHORT i_UnitKind, BYTE i_nStatKind);
	BOOL GetGearStatBYAutoStatTypeAndLevel(GEAR_AUTO_STAT_VALUE_PER_LEVEL *o_pAutoStatValue, INT i_autoStatType, INT i_level);
	char * GetAutoStatTypeString(INT i_autoStatType);

	// 스트링 변환
// 2006-07-28 by cmkwon, 필요 없는 함수
//	const char *GetItemKindString(BYTE i_nKind);
	const char *GetItemKindName(BYTE i_nKind);
	BYTE GetItemKindByItemKindName(char *i_szItemKindString);
	const char *GetRaceString(USHORT i_nRace);
	const char *GetUnitKindString(USHORT i_nUnitKind);
	const char *GetStatString(BYTE i_nStatKind);
	const char *GetTimerTypeString(TimerEventType i_nTimerEventType);
	const char *GetInfluenceTypeString(BYTE i_byInfluenceTy);
	const char *GetPositionString(BYTE i_byPosition);
	const char *GetReqUnitKindString(USHORT i_usUnitKind);
	const char *GetGuildRankString(BYTE i_byGuildRank);

	// 스탯 관련
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT GetCharacterHP(BYTE i_nLevel);
	INT GetCharacterDP(Stat_t i_nShieldPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// DP(쉴드)량
#else
	SHORT GetCharacterHP(BYTE i_nLevel);
	SHORT GetCharacterDP(Stat_t i_nShieldPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// DP(쉴드)량
#endif	
	SHORT GetCharacterSP(Stat_t i_nSoulPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)
	SHORT GetCharacterEP(Stat_t i_nFuelPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)
	float GetAttackC_IncreaseRate(Stat_t i_nAttackPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)		// AttackC 증가율
	float GetAttackC_IncreaseProbability(Stat_t i_nAttackPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// 명중률 - 공격스탯에 의한 공격확률과 동일한 기능을 가지는 추가 확률
	float GetAttackC_PierceAttackProbability(Stat_t i_nAttackPart, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, BYTE i_byLevel, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, 역전의 버프 리뉴얼 - i_PvPBuffPercent 추가		// 2013-05-09 by hskim, 세력 포인트 개선 - // 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// 2008-09-22 by dhjin, 신규 인첸트, // PierceAttack율 - 공격스탯에 의한 방어율과 상쇄가 되는 공격 방법 중 하나
	double GetDefenseC_DecreaseRate(Stat_t i_nDefensePart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// DefenseC에 의한 Damage 감소 비율
	double GetDefenseProbabilityC_Rate(Stat_t i_nDodgePart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100이하 레벨 최대스탯 수치 버그 수정 - 인자추가(, BYTE i_byLevel)// 방어 확률의 비율
	const LEVEL_STAT_VALUE* GetLevelStatValue(int i_nLevelOrStat);

	BOOL GetRepairCost(int *o_pnCost, int *o_pnRealAmountToRepair, int i_nDesParam, int i_nRequestAmountToRepair, ITEM *i_pBulletItem = NULL);	// 수리 비용 계산(단위:spi)

	enumAttackToTarget GetAttackToTarget(ClientIndex_t i_AttackIndex, MEX_TARGET_INFO &i_MexTargetInfo);
	enumAttackToTarget GetAttackToTarget(ClientIndex_t i_AttackIndex, ClientIndex_t i_TargetIndex, UINT i_TargetItemFieldIndex);
	BOOL IsCharacterAttacker(enumAttackToTarget i_nAttackToTarget);
	BOOL IsMonsterAttacker(enumAttackToTarget i_nAttackToTarget);
	BOOL IsCharacterTarget(enumAttackToTarget i_nAttackToTarget);
	BOOL IsMonsterTarget(enumAttackToTarget i_nAttackToTarget);
	BOOL IsItemTarget(enumAttackToTarget i_nAttackToTarget);

	BOOL CheckReqItemKind(BYTE i_nReqItemKind, BYTE i_nTargetItemKind);

	void GetAtumTime(AtumTime *o_pAtumTime);

	INT64 GetCityWarTex(INT64 i_n64ItemPrice, float i_fTexPercent);
	INT64 GetItemTexSPI(INT64 i_n64ItemPrice, float i_fTexPercent);

	float GetPenaltyCollisionBYSpeed(INT i_nCurSpeed);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 인자추가(MapIndex_t i_nStartCityMapIdx)
	MapIndex_t GetCityMapIndexByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx = VCN_CITY_MAP_INDEX);

	// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - CAtumSJ::IsOtherInfluenceMap#, 추가
	MapIndex_t GetGargenMapIndexByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx = VCN_CITY_MAP_INDEX, BOOL i_bMotherShipWar = FALSE);		// 2012-07-02 by hskim, 모선전 개선 - 모선전 진행시 스톤즈 루인 진입 불가	
	BOOL IsOtherInfluenceMap(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx, int i_nMapInflTy);
	BOOL IsSameCharacterInfluence8MapInfluence(BYTE i_byInflTy, int i_nMapInflTy);

	D3DXVECTOR3 GetRandomXZVec3(D3DXVECTOR3 *i_pVec3Center, int i_nRandomRadiusDistance);

	BOOL IsAlphaNum(char i_cParam);
	BOOL IsValidCharacterName(char *i_szCharName);
	BOOL IsValidGuildName(char *i_szGuildName);
	BOOL LoadAllLetterList(const char *i_szFilePath);
	BOOL IsValidOneByteCharacter(const char *i_szSource);	// 2007-12-05 by cmkwon, 캐릭터명 체크시 특수문자 체크 루틴 수정 - CAtumSJ::IsValidOneByteCharacter() 추가

	// 2006-09-05 by cmkwon
	INT GetSPIPenaltyOnDead(int i_nLevel);

	// 2006-09-14 by dhjin, 맴버쉽 서비스시 적용과 서비스 없을 때 적용되는 인벤토리, 창고, 여단원 맥스값 얻어오기
// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 인자 변경
//	INT GetMaxInventorySize(BOOL i_IsMembership);
//	INT GetMaxStoreSize(BOOL i_IsMembership);
	INT GetMaxInventorySize(BOOL i_IsMembership, int i_nAddedCnt);		// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 인자 변경
	INT GetMaxStoreSize(BOOL i_IsMembership, int i_nAddedCnt);			// 2009-11-02 by cmkwon, 캐쉬(인벤/창고 확장) 아이템 추가 구현 - 인자 변경
	INT GetMaxGuildSize(INT i_nGuildMemberCapacity, BOOL i_IsMembership);	// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템

	// 2007-04-10 by cmkwon
	char * GetJamboreePreAddCharacterString(int i_nNumber);

	// 2007-11-21 by cmkwon, PilotFace 변경 카드 구현 - 
	BOOL IsValidPiotFace(BYTE i_byPilotFace);

	// 2008-11-26 by cmkwon, 사용 후 시간(절대시간) 제한 아이템 구현 - 
	// 2009-10-22 by cmkwon, 시간 제한형류 아이템 시스템 정리 - CAtumSJ::GetTimeSecondByItemKind8ItemAttribute#, 수정
	INT GetTimeSecondByItemKind8ItemAttribute(ITEM *i_pItemInfo, TIME_TERM_t i_enTimeTermTy = TIME_TERM_USING_ITEM);
	INT GetTimeMinuteByItemKind8ItemAttribute(ITEM *i_pItemInfo, TIME_TERM_t i_enTimeTermTy = TIME_TERM_USING_ITEM);

	BOOL IsAlphaNum(char *i_szString);				// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
	BOOL IsValidNickName(char *i_szNickName);		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 
	INT FindPartyFormationSkillNum(BYTE i_byFormationTy);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 
	BOOL IsValidWRankingServiceName(char *i_szServiceName);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-20 by cmkwon, 신규 러키 머신 구현 - 
	int GetLuckyMachineSlotCount(int i_machineKind);
	int GetRareItemInfoList(vectRARE_ITEM_INFOPtrList *o_pValidRareList, BOOL i_bIsPrefix, mapRARE_ITEM_INFOPtrList *i_pRareItemInfoList, ITEM *i_pTargetItemInfo, ITEM *i_pRareItemInfo);
	// 2010-06-01 by shcho, GLogDB 관련 -
	const char* GetInfluenceTypeGLogString(BYTE i_byInfluenceTy);
	const char* GetGLOGUnitKindString(USHORT i_nUnitKind);

	void GetDHMSFromTimeInS(const LONGLONG timeInSecs, UINT* pastDays, BYTE* pastHours, BYTE* pastMinutes, BYTE* pastSeconds);

	BYTE GetOppositeNation(BYTE byInflType);	// 2015-12-17 Future - retrival of the opposite Nation
}

namespace CAtumSJ = AtumSJ;

///////////////////////////////////////////////////////////////////////////////
// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 
struct BitS_CParamFactor
{
	UINT	pfb_SKILL_ROLLING_TIME			:1;		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 
	UINT	pfb_SKILL_Barrier				:1;		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 
	UINT	pfb_SKILL_SHIELD_PARALYZE		:1;		// 2009-09-09 ~ 2010-02-18 by dhjin, 인피니티 - Debuff 체크도 추가 // 2009-09-09 ~ 2010 by dhjin, 인피니티 - Debuff 체크로 변경	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 
	UINT	pfb_SKILL_Invincible			:1;		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 
	UINT	pfb_SKILL_DamageDistribution	:1;		// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 추가함
	UINT	pfb_ITEM_IgnoreDefence			:1;		// 203, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 방어력 무시 아이템 사용 체크
	UINT	pfb_ITEM_IgnoreAvoid			:1;		// 204, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 회피력 무시 아이템 사용 체크
};


///////////////////////////////////////////////////////////////////////////////
/// \class		CParamFactor
///
/// \brief		캐릭터의 각종 파라미터에 적용될 factors, DES_XXX에 대응됨
///				pfp:Plus 해주는 Param, Param을 더해준는 처리
///				pfp:Multiple 해주는 Param, Param을 곱해주는 처리
/// \author		kelovon
/// \version
/// \date		2004-04-10 ~ 2004-04-10
/// \warning	캐릭터의 ParamFactor는 ITEM에 적용시에 장전된 무기에만 해당된다.
///////////////////////////////////////////////////////////////////////////////
struct CParamFactor
{
	// DevX removed this constructor to make it POD
	// CParamFactor() { Reset(); }

	void Reset() { util::zero(this, sizeof(CParamFactor)); }

	CParamFactor operator+(const CParamFactor &rhs) const;
	CParamFactor& operator+=(const CParamFactor &rhs);
	CParamFactor operator-(const CParamFactor &rhs) const;
	CParamFactor& operator-=(const CParamFactor &rhs);

	float		pfp_ATTACK_PART;			// 1, 공격 파트
	float		pfp_DEFENSE_PART;			// 2, 방어 파트
	float		pfp_FUEL_PART;				// 3, 연료 파트
	float		pfp_SOUL_PART;				// 4, 감응 파트
	float		pfp_SHIELD_PART;			// 5, 쉴드 파트
	float		pfp_DODGE_PART;				// 6, 회피 파트
	float		pfp_BODYCONDITION;			// 7, 몸상태
	float		pfp_ENDURANCE_01 ;			// 8, 내구도 01
	float		pfp_ENDURANCE_02;			// 9, 내구도 02
	float		pfp_CHARGING_01;			// 10, 장탄수 01
	float		pfp_CHARGING_02;			// 11, 장탄수 02
	INT			pfp_PROPENSITY;				// 12, 성향
	float		pfp_HP;						// 13, 히트포인트
	float		pfp_SP;						// 14, 소울포인트, 일정 시간 후에 SP를 증가시키기만 하면 됨
	float		pfp_DP;						// 89, 쉴드(DEFENSE)포인트
	float		pfp_EP;						// 15, 엔진포인트
	float		pfp_SPRECOVERY;				// 16, 소울포인트회복력
	float		pfp_HPRECOVERY;				// 17, 에너지포인트회복력
	float		pfm_MINATTACK_01;			// 18, (*) 최소 공격력 01
	float		pfm_MAXATTACK_01;			// 71, (*) 최대 공격력 02
	float		pfm_MINATTACK_02;			// 19, (*) 최소 공격력 01
	float		pfm_MAXATTACK_02;			// 72, (*) 최대 공격력 02
	float		pfp_ATTACKPROBABILITY_01;	// 20, // 2010-07-19 by dhjin, 확률 수식 변경 공격확률 01, 0 ~ 255	
	float		pfp_ATTACKPROBABILITY_02;	// 21, // 2010-07-19 by dhjin, 확률 수식 변경공격확률 02, 0 ~ 255
	float		pfp_DEFENSE_01;				// 22, // 2010-07-19 by dhjin, 확률 수식 변경방어력 01, 0 ~ 255
	float		pfp_DEFENSE_02;				// 23, // 2010-07-19 by dhjin, 확률 수식 변경방어력 02, 0 ~ 255
	float		pfp_DEFENSEPROBABILITY_01;	// 24, // 2010-07-19 by dhjin, 확률 수식 변경방어확률 01, 0 ~ 255
	float		pfp_DEFENSEPROBABILITY_02;	// 25, // 2010-07-19 by dhjin, 확률 수식 변경방어확률 02, 0 ~ 255
	Prob255_t	pfp_SKILLPROBABILITY;		// 26, 스킬공격확률, 0 ~ 255
	Prob255_t	pfp_FACTIONRESISTANCE;		// 27, 속성저항력, 0 ~ 255
	float		pfm_SPEED;					// 28, (*) 이동속도
	float		pfp_TRANSPORT;				// 29, 운반력
	float		pfp_MATERIAL;				// 30, 재질
	float		pfm_REATTACKTIME_01;		// 31, (*) 리어택타임 01
	float		pfm_REATTACKTIME_02;		// 32, (*) 리어택타임 02
	Prob255_t	pfp_ABRASIONRATE_01;		// 33, 마모율 01, 0 ~ 255
	Prob255_t	pfp_ABRASIONRATE_02;		// 34, 마모율 02, 0 ~ 255
	float		pfm_RANGE_01;				// 35, (*) 유효거리 01
	float		pfm_RANGE_02;				// 36, (*) 유효거리 02
	float		pfp_RANGEANGLE_01;			// 37, 유효각도 01, 서버에선 불필요
	float		pfp_RANGEANGLE_02;			// 38, 유효각도 02, 서버에선 불필요
	int			pfp_MULTITAGET;				// 39, 멀티타겟, 서버에선 불필요
	float		pfp_EXPLOSIONRANGE_01;		// 40, 폭발반경 01
	float		pfp_EXPLOSIONRANGE_02;		// 67, 폭발반경 02
	float		pfp_UNIT;					// 41, 유닛의 종류 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
	float		pfp_REVISION;				// 42, 유닛의 보정값 (28 ~ 29이 같이 쓰여 유닛마다의 보정값으로 사용됨)
	Prob255_t	pfp_FACTIONPROBABILITY;		// 43, 속성에 대한 방어확률, 0 ~ 255
	int			pfp_SHOTNUM_01;				// 44, 일점사 당 발사 수 01
	int			pfp_SHOTNUM_02;				// 69, 일점사 당 발사 수 02
	int			pfp_MULTINUM_01;			// 45, 동시 발사 수 01
	int			pfp_MULTINUM_02;			// 70, 동시 발사 수 02
	float		pfp_ATTACKTIME_01;			// 46, 처음 공격 시의 타임 01
	float		pfp_ATTACKTIME_02;			// 47, 처음 공격 시의 타임 02
	float		pfm_TIME_01;				// 48, (*) 지속 시간 01
	float		pfm_TIME_02;				// 49, (*) 지속 시간 02
	float		pfm_WEIGHT_01;				// 75, (*) 무게 01
	float		pfm_WEIGHT_02;				// 76, (*) 무게 02
	USHORT		pfp_REACTION_RANGE;			// 91, ITEM의 ReactionRange 변경
	int			pfp_REQ_MIN_LEVEL;			// 96, (서버미사용)아이템장착 요구 MinLevel을 낮춘다
	int			pfp_REQ_MAX_LEVEL;			// 97, (서버미사용)아이템장착 요구 MaxLevel을 낮춘다
	float		pfm_SKILL_REDUCE_SHIELD_DAMAGE;		// 121, 2005-11-21 by cmkwon - 쉴드에 가는 데미지를 줄여준다.(*)
	float		pfm_ATTACK_RANGE_01;				// 129, 2005-11-21 by cmkwon - 레이다의 기본 무기 조준 거리를 증가시킨다.(*)
	float		pfm_ATTACK_RANGE_02;				// 130, 2005-11-21 by cmkwon - 레이다의 고급 무기 조준 거리를 증가시킨다.(*)
// 2005-12-02 by cmkwon	float		pfm_SKILL_HYPERMOVING;				// 132, 2005-11-28 by cmkwon - 유닛의 전체 이동속도가 좋아지고, 부스터 사용이 되지 않는다.(*)
	float		pfm_SKILL_COLLISIONDAMAGE_DOWN;		// 140, 2005-11-21 by cmkwon - 배경 오브젝트,바닥 충돌시 데미지를 감소시킨다.(*)
	float		pfm_SKILL_SMARTSP;					// 148, 2005-11-21 by cmkwon - 자신이 사용하는 모든 스킬의 사용량을 줄여준다.(*)	
	float		pfm_SKILL_REACTIONSPEED;			// 154, 2005-12-02 by cmkwon - 하이퍼 무빙(반응속도)
	float		pfm_SKILL_ENGINEANGLE;				// 155, 2005-12-02 by cmkwon - 하이퍼 무빙(선회각)
	float		pfm_SKILL_ENGINEBOOSTERANGLE;		// 156, 2005-12-02 by cmkwon - 하이퍼 무빙(부스터 선회각)

	float		pfm_DROP_EXP;						// 157, 2006-03-30 by cmkwon - 경험치, 프리미엄과 중첩 불가
	float		pfm_DROP_SPI;						// 158, 2006-03-30 by cmkwon - SPI, 프리미엄과 중첩 불가
	float		pfm_DROP_ITEM;						// 159, 2006-03-30 by cmkwon - 아이템 드랍율, 프리미엄과 중첩 불가
	float		pfm_DROP_RAREITEM;					// 238, 2010-11-30 by shcho, 레어아이템 드랍 확률 증가 아이템 구현
	float		pfm_HP_REPAIR_RATE_FLIGHTING;		// 160, 2006-03-30 by cmkwon - 비행시 HP 회복율
	float		pfm_DP_REPAIR_RATE;					// 161, 2006-03-30 by cmkwon - DP 회복율
	float		pfm_SP_REPAIR_RATE;					// 162, 2006-03-30 by cmkwon - SP 회복율
// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 아래와 같이 비트구조체로 변경
// 	BOOL		pfb_SKILL_ROLLING_TIME;				// 170, 2006-11-17 by dhjin - 30초간 롤링 재사용 시간을 없애준다 
// 	BOOL		pfb_SKILL_Barrier;					// 173, 2006-11-24 by dhjin - A기어의 고급무기에 대한 무적스킬
// 	BOOL		pfb_SKILL_SHIELD_PARALYZE;			// 175,	2006-11-24 by dhjin - A기어의 쉴드마비 스킬, 상대기어의 쉴드 회복율을 0으로 만든다
// 	BOOL		pfb_SKILL_Invincible;				// 122, 2006-11-27 by dhjin - M기어의 무적 스킬
	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 편대 대형 스킬 구현 - 기존 BOOL 변수를 비트구조체로 변경, 
	BitS_CParamFactor	pfb_BitFlag;				

	float		pf_SKILL_Big_Boom_damage;			// 174, 2006-11-29 by dhjin - A기어의 기본무기 차징샷, 스플래쉬 데미지는 Range

	float		pfm_WARHEAD_SPEED;					// 176, 2007-06-11 by cmkwon, 탄두의 속도

	// 2008-09-22 by dhjin, 신규 인첸트
	float		pfn_ENGINE_BOOSTER_TIME_UP;			// 178		// 2008-09-22 by dhjin, 부스터 시간 증가
	float		pfn_ENGINE_MAX_SPEED_UP;			// 179,	// 2008-09-22 by dhjin, 엔진 일반속도(최대) 증가
	float		pfn_ENGINE_MIN_SPEED_UP;			// 180,	// 2008-09-22 by dhjin, 엔진 일반속도(최소) 증가
	float		pfn_ENGINE_BOOSTER_SPEED_UP;		// 181,	// 2008-09-22 by dhjin, 엔진 부스터속도 증가
	float		pfn_ENGINE_GROUND_SPEED_UP;			// 182,	// 2008-09-22 by dhjin, 엔진 지상속도 증가
	int			pfn_RADAR_OBJECT_DETECT_RANGE;		// 183, // 2008-09-22 by dhjin, 레이더 물체 감지 반경
	float		pfm_PIERCE_UP_01;					// 184, // 2008-09-22 by dhjin, 기본무기 피어스율 증가 카드
	float		pfm_PIERCE_UP_02;					// 185, // 2008-09-22 by dhjin, 고급무기 피어스율 증가 카드
	float		pfm_ENGINE_ANGLE_UP;				// 186,	// 2008-09-30 by dhjin, 엔진 회전각 증가 카드
	float		pfm_ENGINE_BOOSTERANGLE_UP;			// 187,	// 2008-09-30 by dhjin, 엔진 부스터 회전각 증가 카드	
	ParamValue_t	pf_ITEM_Resistance;				// 201, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 저항 아이템 사용 체크 
	ParamValue_t	pf_ITEM_AddAttack;				// 202, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 타격치 아이템 사용 체크 
	ParamValue_t	pf_ITEM_ReduceDamage;			// 205, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 데미지 절대값 감소 아이템 사용 체크
	ParamValue_t	pf_ITEM_AddAttack_Sec;			// 206, // 2009-09-09 ~ 2010 by dhjin, 인피니티 - 절대값 타격치 아이템 고급무기용(기무와 분류)
	ParamValue_t	pf_ITEM_OnceResistance;			// 207, // 2009-09-09 ~ 2010-01-19 by dhjin, 인피니티 - 한 번만 저항하고 없어지는 저항 아이템 추가
	float			pfm_PLUS_WARPOINT_RATE;			// 234, // 2010-05-18 by cmkwon, WarPoint 증가 아이템 구현(일본요청) - 
	ParamValue_t	pf_ITEMS_MOVE_SPEED_RATE;		// 239, // 2010-12-21 by jskim, 마을 이동 속도 증가 아이템 구현

	ParamValue_t	pfp_SKILL_BUFF_MON_ATTACK_POWER;			// 300, // 2013-05-09 by hskim, 세력 포인트 개선 - // 몬스터 공격시 - 공격력 증가 : Value 증가 %
	ParamValue_t	pfp_SKILL_BUFF_MON_ATTACK_PROBABILITY;		// 301, // 2013-05-09 by hskim, 세력 포인트 개선 - // 몬스터 공격시 - 공격력 확율 : Value 증가 %
	ParamValue_t	pfp_SKILL_BUFF_MON_ATTACK_PIERCE;			// 302, // 2013-05-09 by hskim, 세력 포인트 개선 - // 몬스터 공격시 - 피어스 증가 : Value 증가 %
	ParamValue_t	pfp_SKILL_BUFF_MON_DEFENCE;					// 303, // 2013-05-09 by hskim, 세력 포인트 개선 - // 몬스터 방어시 - 방어력 증가 : Value 증가 %
	ParamValue_t	pfp_SKILL_BUFF_MON_DEFENCE_AVOID;			// 304, // 2013-05-09 by hskim, 세력 포인트 개선 - // 몬스터 방어시 - 회피력 증가 : Value 증가 %
	ParamValue_t	pfp_SKILL_BUFF_PVP_ATTACK;					// 305, // 2013-05-09 by hskim, 세력 포인트 개선 - // PVP - 공격력 증가 : Value 증가 %
	ParamValue_t	pfp_SKILL_BUFF_PVP_ATTACK_PROBABILITY;		// 306,	// PVP - 명중률 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	ParamValue_t	pfp_SKILL_BUFF_PVP_ATTACK_PIERCE;			// 307,	// PVP - 피어스 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	ParamValue_t	pfp_SKILL_BUFF_PVP_DEFENCE;					// 308,	// PVP - 방어력 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
	ParamValue_t	pfp_SKILL_BUFF_PVP_DEFENCE_PROBABILITY;		// 309,	// PVP - 회피력 증가 : Value 증가 %		// 2013-08-01 by jhseol, 역전의 버프 리뉴얼
};