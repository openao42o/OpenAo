// FieldMonster.h: interface for the CFieldMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDMONSTER_H__4AF49C24_9155_4274_A7F2_E3557E7D59B9__INCLUDED_)
#define AFX_FIELDMONSTER_H__4AF49C24_9155_4274_A7F2_E3557E7D59B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "FieldIOCP.h"


typedef mt_list<MONSTER_DROP_ITEM_GENERAL>		mtlistMonsterDropItem;

////////////////////////////////////////////////////////////////////////////////
// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
struct BARRIER_SKILL_INFO {
	BOOL			bBarrierSkillHave;
	BOOL			bBarrierSkillUsing;
	HPHitRate_t		HitRate;
	DWORD			UseTick;
	DWORD			ReAttackTick;
	DWORD			dwLastTick;	
	INT				SkillNum;
};
struct IMMEDIATE_BARRIER_SKILL_INFO {
	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동)
	BOOL			bBarrierSkillUsing;
	DWORD			UseTick;
	DWORD			dwLastTick;	
	INT				SkillNum;
};

class CCityWar;
class CTriggerFunction;				// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
class CTriggerFunctionCrystal;		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

class CFieldMonster : public CMonster
{
	friend class CFieldMapChannel;
	friend class CFieldIOCPSocket;

public:
	CFieldMonster();
	virtual ~CFieldMonster();

	///////////////////////////////////////////////////////////////////////////
	// Property
	USHORT GetSendCountsMonsterMoveOK(void);
	void SetCityWarFieldMonster(CCityWar *i_pWar);

	///////////////////////////////////////////////////////////////////////////
	// Method
	// 맵 패킷 레벨 시스템 관련
	USHORT IncreaseSendCountsMonsterMoveOK(void);

	float IncreaseMonsterCurrentHP(float i_fValue);
	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - DecreaseMonsterCurrentHP 로 통합
//	float DecreaseMonsterCurrentHPByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList);
//	float DecreaseMonsterCurrentHPByMonster(float i_fValue);	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 몬스터에게 공격 당함
//	float DecreaseMonsterCurrentHPByPenalty(float i_fValue);	// 2010-03-31 by dhjin, 인피니티(기지방어) - 패널티 값으로 인하여 현재 HP 감소
	float DecreaseMonsterCurrentHP(float i_fValue, BOOL i_bPenalty=FALSE);	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 -
	void ProcessingDamagedMonsterByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList);		// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 -

	// 이동 관련
	D3DXVECTOR3 GetCurrentPosition(void);

	ITEM * FindItemPtrWithItemNum(INT i_uiItemNum);
	ITEM * FindItemPtrByOrBitTyp(BYTE i_byOrBitType);

	// 폭파시 item 처리 관련
	void LockDropItemList(void);
	void UnlockDropItemList(void);
// 2007-12-07 by cmkwon, 사용하지 않는 함수 임
//	void CreateDropItem(CFieldIOCP *pFieldIOCP, CFieldIOCPSocket *i_pFISockGive2MonsterTopDamage);

	// 2006-11-07 by cmkwon
	BOOL SetAttackerCliIdx(ClientIndex_t i_cliIdx);
	ClientIndex_t GetAttackerCliIdx(void);
// 2009-12-11 by cmkwon, 데미지 어그로로 타겟을 변경하는 몬스터 구현 - CMonster 클래스로 이동
//	BOOL IsValidMonster(BOOL i_bLiveCheck=TRUE);

	// virtual
	virtual void ResetMonster(void);
	virtual void InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo);
	
	// 폭파시 item 처리 관련
	mtlistMonsterDropItem		m_mtlistDropItem;

	mtvectClientIndex_t			m_mtVectCltIdxForMonsterMoveOK;
	mtvectClientIndex_t			m_mtVectCltIdxForExcludeMonsterMoveOK;

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-08 by cmkwon, 2형무기 공격확률,공격력,피어스율 적용 시스템 수정
	mtvectSATTACK_PARAMETER		m_mtvectSAttackParameterList;		// 2007-06-08 by cmkwon, 2형 T_FC_BATTLE_ATTACK시의 정보
	BOOL APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam);
	SATTACK_PARAMETER *APFindAttackParameterNoLock(UID16_t i_WeaponIndex);
	BOOL APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex);
	void APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex);

	///////////////////////////////////////////////////////////////////////////////	
	// 2010-04-05 by cmkwon, 인피2차 M2M 2형 무기 보완 처리 - 
	BOOL APPopOverLifeTimeAttackParameterList(vectSATTACK_PARAMETER *o_pOverTimeAttParamList);

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 -
	void HPRateCalcByMSWarInfoDisPlay(float i_fMonsterMaxHP, float i_fMonsterBeforeHP, float i_fMonsterAfterHP);			// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 표시와 관련된 몬스터 HPRate계산하여 정보 전송
	BOOL MSMonsterCheckWithoutMSBossMonster();			// 2008-03-28 by dhjin, 모선전 정보 표시 기획안 - 모선전 정보 표시와 관련된 몬스터 인지 체크

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	void MonsterSkillApply(MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg, ITEM * i_nSkill);
	void SetBarrier(ITEM * i_pSkill);
	BOOL CheckBarrierHave();	// 배리어 스킬 가지고 있는지 체크 
	BOOL CheckBarrierUsing();	// 배리어 스킬 사용중 체크
	BOOL BarrierUse();			// 배리어 스킬 사용
	void SetImmediateBarrier(ITEM * i_pSkill);		// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동) 설정
	BOOL CheckImmediateBarrierUsing();	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동) 사용중 체크

	CFieldMapChannel * GetCurrentMapChannelMonster();	
	ItemNum_t GetCurrentAttackItemNum();		// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
	ItemNum_t m_CurrentAttackItemNum;					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가

	void ProcessingInserttoAttackedInfoList(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList);	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리
	
	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	void InsertTriggerFunction(CTriggerFunction *pTriggerFunction);
	BOOL IsTriggerFunction();
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

protected:
	D3DXVECTOR3					m_BeforePosition;					//
	DWORD						m_dwTimeLastMoved;					// 몬스터의 마지막 이동 시간

	// 맵 패킷 레벨 시스템
	USHORT						m_usSendCountsMonsterMoveOK;		// Move 패킷 전송 카운트

	CFieldMapChannel			*m_pCurrentFieldMapChannelMonster;	// CFieldMapChannel의 생성자에서 할당해야 함

	// 보스급 몬스터 처리
	DWORD						m_dwTimeCreatedTick;
	CCityWar					*m_pCityWar2;
	DWORD						m_dwLastTickInfluenceBossAttackedMsg;	// 2006-01-20 by cmkwon, 세력전 보스 몬스터가 공격당하는 메시지를 보낸 마시작 시간

	// 2006-11-07 by cmkwon
	ClientIndex_t				m_nAttackerCliIdx;						// 2006-11-07 by cmkwon
	DWORD						m_dwLastTickSetAttackerCliIdx;			// 2006-11-07 by cmkwon

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	BARRIER_SKILL_INFO			m_bBarrierInfo;							// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 배리어
	IMMEDIATE_BARRIER_SKILL_INFO	m_ImmediateBarrier;					// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동)

	mt_vector<CTriggerFunction *>	m_mtVectTriggerFunctionPtr;			// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템	
};

#endif // !defined(AFX_FIELDMONSTER_H__4AF49C24_9155_4274_A7F2_E3557E7D59B9__INCLUDED_)
