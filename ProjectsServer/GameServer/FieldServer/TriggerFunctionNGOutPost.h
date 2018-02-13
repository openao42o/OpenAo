// TriggerFunctionNGOutPost.h: interface for the CTriggerFunctionNGOutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTIONNGOUTPOST_H__02F8538D_6693_439E_A335_4C55F13974B6__INCLUDED_)
#define AFX_TRIGGERFUNCTIONNGOUTPOST_H__02F8538D_6693_439E_A335_4C55F13974B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TriggerFunction.h"
#include "MapTriggerManager.h"
#include "OutPost.h"

class CTriggerNGCInflWarGroup;

class CTriggerFunctionNGOutPost : public CTriggerFunction
{
public:
	CTriggerFunctionNGOutPost();
	virtual ~CTriggerFunctionNGOutPost();

	void OnClear();
	void OnDestroy();
	BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);
	
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);
	
	void OnEventSkip(BOOL i_bCheckCreateCrystal = TRUE);
	void OnFixedBuffItem(INT i_nFixedBuffItem = 0);
	
	BOOL OnMonsterCreate(CFieldMonster *pMonster);
	BOOL OnMonsterDead(CFieldMonster *pMonster);
	BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);
	BOOL OnIsPossibleBossAttack(MonIdx_t MonsterIdx);	// 외부 클레스에서 현재 보스몬스터의 공략 가능 여부를 확인할 함수
	
public:
	void SetNGCOutPostInfo(SummonMosterGroupID_t i_nSummonMonsterGroupID, MapIndex_t i_nStandardOutPostMap, MonIdx_t i_nBossMonster, INT i_nWinInfluenceWP, INT i_nLossInfluenceWP, MapIndex_t i_nNextOutPostMap, INT i_nDestroyCrystalcount);
	void InsertBossMonsterKillInfo(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* i_BossKillInfo);
	BOOL InsertNGCOutPostSummonMonster(CTriggerNGCInflWarMonsterGroup *i_pTriggerFnctionNGCOutPostSummonMonster);
	void InsertCrystalBuff(STRIGGER_CRYSTAL_BUFF *i_vectCrystalBuff);

protected:
	void InitBossKillPlayStep();		// 보스몬스터 공략단계를 초기화 함수
	void StopEvent();
	
	BOOL IsPossibleBossAttack();		// 보스몬스터의 공략 여부를 판단하는 함수
	
	void RunKeyMonsterKillEvent(MonIdx_t i_nMonsterNum);	// 크리스탈 파괴시 이벤트를 적용 시키는 함수
	void RunBossKillEvent();							// 보스몬스터 공략 성공시 발동되는 이벤트 함수
	
	BYTE FindWinInfluenc(MonIdx_t i_nMonsterNum);				// 이긴 세력 찾기

	void CreateAllMonster();
	void CreateCrystalMonster();
	void DestroyAllMonster();
	void DestroyCrystalMonster();

	void CrystalRegen();
	
protected:

	CFieldIOCP* m_pFieldIOCP;
	CFieldMapChannel	*m_pFieldNextOutPostMapChannel;
	CFieldMapChannel	*m_pFieldOutPostCityMapChannel;

	BOOL m_bTodayTriggerEventPlay;		// 오늘 전진기지전 트리거가 발동 되었는지 체크할 변수.
	BOOL m_bTriggerOn;					// 트리거 발동 체크변수
	BOOL m_bIsOutPostWarRuning;			// 전진기지전이 진행중이다.
	BOOL m_bCommendReset;				// 명령어로 트리서 리셋
	BOOL m_bPossibleBossAttack;			// 보스몬스터 공격 가능 여부
	BOOL m_bBeforePossibleBossAttack;	// 아전 보스몬스터 공격 가능 여부
	BOOL m_bIsBossDestroy;	// 아전 보스몬스터 공격 가능 여부
	
	SummonMosterGroupID_t		m_nSummonMonsterGroupID;		// 소환 몬스터 그룹 ID
	MapIndex_t					m_nStandardOutPostMap;			// 기준이 될 전진기지 맵, 해당 전진기지가 발생한 다음날 동일 시간에 트리거가 발동 된다.
	MonIdx_t					m_nBossMonster;
	INT							m_nWinInfluenceWP;
	INT							m_nLossInfluenceWP;
	MapIndex_t					m_nNextOutPostMap;				// 다음 전진기지 맵
	INT							m_nDestroyCrystalcount;			// 파괴해야 하는 크리스탈의 수

	INT							m_nFixedBuffItem;				// 테스트용 특정 버프만 나오도록 설정

	mtvectTriggerAttackUserDamage				m_mtvectTriggerAttackUserDamage;		// 보스 몬스터에세 입힌 데미지 정보
	vector<CTriggerNGCInflWarMonsterGroup *>	m_vectorTriggerNGCOutPostSummonMonsterGroup;		// 초기 부팅시 설정되고 변경되지 않는다. 동기화 필요 없음
	vectorTriggerOutPostBossKillInfo			m_vectTriggerOutPostBossKillInfo;		// 보스 몬스터를 잡기 위해 먼저 잡아야 하는 선행 몬스터 정보의 원본 벡터
	mtvectorTriggerOutPostMonsterKillInfo		m_mtvectTriggerOutPostMonsterKillInfo;	// 현재 진행중인 보스몬스터의 공략 단계를 저장할 백터
	vectorTriggerCrystalBuff					m_vectCrystalBuff;						// 크리스탈 버프 리스트
};

#endif // !defined(AFX_TRIGGERFUNCTIONNGOUTPOST_H__02F8538D_6693_439E_A335_4C55F13974B6__INCLUDED_)
