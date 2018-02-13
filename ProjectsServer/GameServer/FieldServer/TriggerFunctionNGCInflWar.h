// TriggerFunctionNGCInflWar.h: interface for the CTriggerFunctionNGCInflWar class.
//
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-11-07
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTIONNGCINFLWAR_H__3705FC12_22C8_4256_9ED5_F1527275D0CF__INCLUDED_)
#define AFX_TRIGGERFUNCTIONNGCINFLWAR_H__3705FC12_22C8_4256_9ED5_F1527275D0CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TriggerFunction.h"

class CTriggerNGCInflWarGroup;

class CTriggerFunctionNGCInflWar : public CTriggerFunction  
{
public:
	CTriggerFunctionNGCInflWar();
	virtual ~CTriggerFunctionNGCInflWar();

	void OnClear();
	void OnDestroy();
	BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);

	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);

	void OnEventSkip(BOOL bCheckCreateCrystal = TRUE);		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

	BOOL OnMonsterCreate(CFieldMonster *pMonster);
	BOOL OnMonsterDead(CFieldMonster *pMonster);
	BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);

public:
	void SetNGCInflWarInfo(NGCInflWarGroupID_t NGCInflWarGroupID, INT PeriodTime, MonIdx_t BossMonster, INT WinInfluenceWP, INT LossInfluenceWP, INT FirstDamageGuildWP, INT FixedTimeType, INT i_nDestroyCrystalcount);	// 2013-07-08 by jhseol, 트리거 시스템 확장
	BOOL InsertNGCInflWarMonster(CTriggerNGCInflWarMonsterGroup *pTriggerNGCInflWarGroup);

	BOOL IsCreatedNGCInflWar()		{ return m_bCreatedNGCInflWar; }

	void ResetLocalData();		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

	void SetNextCreateTime();

	void FindFirstDamageUser();		// 누적 데미지 1등 유저 찾기
	void FindSequenceAmountAttack();	// 보스 본스터 누적 데미지 내림차순 정렬한 유저 리스트
	void FindWinInfluenc();			// 이긴 세력 찾기

	// 2013-07-08 by jhseol, 트리거 시스템 확장
	void InsertBossMonsterKillInfo(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* i_BossKillInfo);
	void InitBossKillPlayStep();
	void CreateCrystalMonster();
	void DestroyCrystalMonster();
	void CrystalRegen();
	BOOL IsPossibleBossAttack();
	BOOL OnIsPossibleBossAttack(MonIdx_t MonsterIdx);
	void RunKeyMonsterKillEvent(MonIdx_t i_nMonsterNum);
	void InsertCrystalBuff(STRIGGER_CRYSTAL_BUFF *i_CrystalBuff);
	INT GetDestroyCrystalcount();
	// end 2013-07-08 by jhseol, 트리거 시스템 확장
protected:
	void RunEvent();
	void StopEvent();

	void CreateAllMonster();
	void DestroyAllMonster();

protected:
	BOOL m_bCreatedNGCInflWar;
	
	NGCInflWarGroupID_t m_NGCInflWarGroupID;
	INT					m_PeriodTime;
	MonIdx_t			m_BossMonster;
	INT					m_WinInfluenceWP;
	INT					m_LossInfluenceWP;
	INT					m_FirstDamageGuildWP;
	INT					m_FixedTimeType;

	mtvectTriggerAttackUserDamage				m_mtvectTriggerAttackUserDamage;		// 각 데미지 정보
	vector<CTriggerNGCInflWarMonsterGroup *>	m_vectorTriggerNGCInflWarMonster;		// 초기 부팅시 설정되고 변경되지 않는다. 동기화 필요 없음

protected:
	TimeUnit_t	m_NextCreateTick;

	UID32_t		m_FirstCharacterUniqueNumber;		// 1등 캐릭터 고유번호
	UID32_t		m_FirstGuildUniqueNumber;			// 1등 길드 길드 번호
	BYTE		m_WinInfluenceType;					// 이긴 세력 타입
	BYTE		m_DefeatInfluenceType;				// 패배 세력 타입

	mtvectTriggerAttackUserDamage				m_mtvectTriggerDamageSortUser;			// 보스 몬스터에 공격한 데미지 순으로 내림차순 정렬 리스트

	// 2013-07-08 by jhseol, 트리거 시스템 확장
	BOOL		m_bPossibleBossAttack;			// 보스몬스터 공격 가능 여부
	BOOL		m_bBeforePossibleBossAttack;	// 이전 보스몬스터 공격 가능 여부
	BOOL		m_bIsBossDestroy;				// 보스 파괴 여부
	INT			m_nDestroyCrystalcount;			// 파괴해야 하는 크리스탈의 수
	vectorTriggerOutPostBossKillInfo			m_vectTriggerNGCInflWarBossKillInfo;
	mtvectorTriggerOutPostMonsterKillInfo		m_mtvectTriggerNGCInflWarMonsterKillInfo;
	vectorTriggerCrystalBuff					m_vectCrystalBuff;						// 크리스탈 버프 리스트
	// end 2013-07-08 by jhseol, 트리거 시스템 확장
};

#endif // !defined(AFX_TRIGGERFUNCTIONNGCINFLWAR_H__3705FC12_22C8_4256_9ED5_F1527275D0CF__INCLUDED_)
