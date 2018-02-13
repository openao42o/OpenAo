// HPAction.cpp: implementation of the CHPAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HPAction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHPAction::CHPAction()
{
	this->Init();
}

CHPAction::~CHPAction()
{

}

void CHPAction::Init() {
	m_NextAttackItemIdx = 0;
	util::zero(&m_CreateTalk, sizeof(HPACTION_TALK_HPRATE));
	util::zero(&m_DeadTalk, sizeof(HPACTION_TALK_HPRATE));
	m_vectHPAction.clear();
	m_vectHPActionAttackHPRate.clear();
	m_vectHPActionTalkHPRate.clear();
	m_vectHPActionTalkDamagedRandom.clear();
	m_vectHPActionTalkAttack.clear();
	util::zero(&m_TargetChangeTalk, sizeof(SIZE_MAX_HPTALK_DESCRIPTION));
	m_HPActionAttackUID = 0;			// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
	m_HPActionAttackTalkUID = 0;			// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
}

void CHPAction::InitHPActionListByDB(vectHPAction * i_pvectHPActionDBValue) {
	this->Init();
	
	m_vectHPAction.assign(i_pvectHPActionDBValue->begin(), i_pvectHPActionDBValue->end());

	vectHPAction::iterator itr = m_vectHPAction.begin();
	for (; itr != m_vectHPAction.end(); itr++) {
		if(0 != itr->UseItemArrayIdx) {
			this->InitHPActionAttackHPRateList(&*itr);
		}
		else if(HPACTION_TALK_CONDITION_HP_RATE == itr->HPTalkCondition) {
			this->InitHPActionTalkHPRateList(&*itr);
		}
		else if(HPACTION_TALK_CONDITION_DAMAGED_RANDOM == itr->HPTalkCondition) {
			this->InitHPActionTalkDamagedRandomList(&*itr);
		}
		else if(HPACTION_TALK_CONDITION_CREATE == itr->HPTalkCondition) {
			this->InitHPActionTalkCreate(&*itr);
		}
		else if(HPACTION_TALK_CONDITION_DEAD == itr->HPTalkCondition) {
			this->InitHPActionTalkDead(&*itr);
		}
		else if(HPACTION_TALK_CONDITION_TARGET_CHANGE == itr->HPTalkCondition) {
			 this->InitHPActionTalkTargetChange(&*itr);
		}
	}
}

void CHPAction::InitHPActionAttackHPRateList(HPACTION * i_pHPAction) {
	HPACTION_ATTACK_HPRATE	HPActionAttackHPRate;
	util::zero(&HPActionAttackHPRate, sizeof(HPACTION_ATTACK_HPRATE));
	HPActionAttackHPRate.HPActionUID	= i_pHPAction->HPActionUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
	HPActionAttackHPRate.HPMaxValueRate = i_pHPAction->HPMaxValueRate;
	HPActionAttackHPRate.HPMinValueRate = i_pHPAction->HPMinValueRate;
	HPActionAttackHPRate.HitRate		= i_pHPAction->HitRate;
	HPActionAttackHPRate.UseItemArrayIdx	= i_pHPAction->UseItemArrayIdx;
	HPActionAttackHPRate.NextUseItemArrayIdx = i_pHPAction->NextUseItemArrayIdx;
	HPActionAttackHPRate.UseCount		= i_pHPAction->UseCount;
	m_vectHPActionAttackHPRate.push_back(HPActionAttackHPRate);
	if(HPACTION_TALK_CONDITION_ATTACK == i_pHPAction->HPTalkCondition) {
		HPACTION_TALK_ATTACK	HPActionTalkAttack;
		util::zero(&HPActionTalkAttack, sizeof(HPACTION_TALK_ATTACK));
		HPActionTalkAttack.HPActionUID		= i_pHPAction->HPActionUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
		HPActionTalkAttack.UseItemArrayIdx	= i_pHPAction->UseItemArrayIdx;
		HPActionTalkAttack.PreHPCameraTremble = i_pHPAction->PreHPCameraTremble;
		util::strncpy(HPActionTalkAttack.PreHPTalk, i_pHPAction->PreHPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
		HPActionTalkAttack.HPCameraTremble = i_pHPAction->HPCameraTremble;
		util::strncpy(HPActionTalkAttack.HPTalk, i_pHPAction->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
		m_vectHPActionTalkAttack.push_back(HPActionTalkAttack);
	}
}

void CHPAction::InitHPActionTalkHPRateList(HPACTION * i_pHPAction) {
	HPACTION_TALK_HPRATE	HPActionTalkHPRate;
	util::zero(&HPActionTalkHPRate, sizeof(HPACTION_TALK_HPRATE));
	HPActionTalkHPRate.HPValueRate		= i_pHPAction->HPMaxValueRate;
	HPActionTalkHPRate.HPTalkImportance = i_pHPAction->HPTalkImportance;
	util::strncpy(HPActionTalkHPRate.HPTalk, i_pHPAction->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
	m_vectHPActionTalkHPRate.push_back(HPActionTalkHPRate);
}

void CHPAction::InitHPActionTalkDamagedRandomList(HPACTION * i_pHPAction) {
	HPACTION_TALK_DAMAGED_RANDOM	HPActionTalkDamagedRandom;
	util::zero(&HPActionTalkDamagedRandom, sizeof(HPACTION_TALK_DAMAGED_RANDOM));
	util::strncpy(HPActionTalkDamagedRandom.HPTalk, i_pHPAction->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
	m_vectHPActionTalkDamagedRandom.push_back(HPActionTalkDamagedRandom);
}

void CHPAction::InitHPActionTalkCreate(HPACTION * i_pHPAction) {
	util::zero(&m_CreateTalk, sizeof(HPACTION_TALK_HPRATE));
	m_CreateTalk.HPValueRate		= i_pHPAction->HPMaxValueRate;
	m_CreateTalk.HPTalkImportance = i_pHPAction->HPTalkImportance;
	util::strncpy(m_CreateTalk.HPTalk, i_pHPAction->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
}

void CHPAction::InitHPActionTalkDead(HPACTION * i_pHPAction) {
	util::zero(&m_DeadTalk, sizeof(HPACTION_TALK_HPRATE));
	m_DeadTalk.HPValueRate		= i_pHPAction->HPMaxValueRate;
	m_DeadTalk.HPTalkImportance = i_pHPAction->HPTalkImportance;
	util::strncpy(m_DeadTalk.HPTalk, i_pHPAction->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
}

void CHPAction::InitHPActionTalkTargetChange(HPACTION * i_pHPAction) {
	util::strncpy(m_TargetChangeTalk, i_pHPAction->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
}

BOOL CHPAction::CheckValidSizeAttackItemIdx() {
	if(m_vectHPActionAttackHPRate.size()) {
		return TRUE;
	}

	return FALSE;
}

BOOL CHPAction::CheckValidSizeTalkCreate() {
	if(strlen(m_CreateTalk.HPTalk)) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL CHPAction::CheckValidSizeTalkDead() {
	if(strlen(m_DeadTalk.HPTalk)) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL CHPAction::CheckValidSizeTalkHPRate() {
	if(m_vectHPActionTalkHPRate.size()) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL CHPAction::CheckValidSizeTalkDamagedRadom() {
	if(m_vectHPActionTalkDamagedRandom.size()) {
		return TRUE;
	}
	
	return FALSE;
}

BOOL CHPAction::CheckValidSizeTalkTargetChange() {
	if(strlen(m_TargetChangeTalk)) {
		return TRUE;
	}
	
	return FALSE;	
}

BOOL CHPAction::GetAttackItemIdxHPRate(MonHP_t i_CurrentMonHP, ItemIdx_t * o_pAttackItemIdx, INT * o_pSelectVectIdx) {
	if(NULL == o_pAttackItemIdx 
		|| NULL == o_pSelectVectIdx) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	vectHPActionAttackHPRate::iterator itr = m_vectHPActionAttackHPRate.begin();
	while (itr != m_vectHPActionAttackHPRate.end()) {
		if(0 >= itr->UseCount || itr->HPMinValueRate > i_CurrentMonHP) {
			// 선택 될 수 없는 지나간 공격 수단이면 과감히 삭제!
			itr = m_vectHPActionAttackHPRate.erase(itr);
			continue;
		}
		
		// 2010-07-19 by dhjin, 확률 수식 변경
//		Prob100_t Random = RANDI(1, PROB100_MAX_VALUE+1);		
		Prob100_t Random = RAND100();
		if(itr->HPMaxValueRate >= i_CurrentMonHP && itr->HPMinValueRate < i_CurrentMonHP 
			&& Random <= itr->HitRate && 0 < itr->UseCount) {
			// 조건을 만족하는 공격 수단이 있다면 반환
			*o_pAttackItemIdx = itr->UseItemArrayIdx;
			itr->HPMaxValueRate = 100;	// 한 번 공격 시작된 것은 피가 높아져도 계속 공격 되어야한다. 
			m_HPActionAttackUID = itr->HPActionUID;			// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
			m_HPActionAttackTalkUID = itr->HPActionUID;		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
			return TRUE;
		}
		(*o_pSelectVectIdx)++;
		itr++;
	}

	return FALSE;
}

BOOL CHPAction::GetNextAttackItem(ItemIdx_t * o_pAttackItemIdx) {
	if(NULL == o_pAttackItemIdx) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	if(m_NextAttackItemIdx) {
		// 연속 공격 값이 있다면 연속 공격을 반환하고 초기화 한다.
		*o_pAttackItemIdx = m_NextAttackItemIdx;
		m_NextAttackItemIdx = 0;
		return TRUE;
	}

	return FALSE;
}

// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정, 밑과 같이 수정 HPActionItem 사용시 사용 카운트를 줄인다.
//void CHPAction::SetSuccessAttackItemIdxHPRate(INT i_SelectVectIdx) {
//	m_vectHPActionAttackHPRate[i_SelectVectIdx].UseCount--;
//	m_NextAttackItemIdx = m_vectHPActionAttackHPRate[i_SelectVectIdx].NextUseItemArrayIdx;
void CHPAction::SetSuccessAttackItemIdxHPRate() {
	vectHPActionAttackHPRate::iterator itr = m_vectHPActionAttackHPRate.begin();
	for (; itr != m_vectHPActionAttackHPRate.end(); itr++) {
		if(m_HPActionAttackUID == itr->HPActionUID) {
			itr->UseCount--;
			m_NextAttackItemIdx = itr->NextUseItemArrayIdx;
			m_HPActionAttackUID = 0;
			return;
		}
	}
}

BOOL CHPAction::GetTalkCreate(HPACTION_TALK_HPRATE * o_pTalkHPRate) {
	if(NULL == o_pTalkHPRate) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	if(strlen(m_CreateTalk.HPTalk)) {
		o_pTalkHPRate->HPValueRate = m_CreateTalk.HPValueRate;
		o_pTalkHPRate->HPTalkImportance = m_CreateTalk.HPTalkImportance;
		util::strncpy(o_pTalkHPRate->HPTalk, m_CreateTalk.HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
		return TRUE;
	}

	return FALSE;
}

BOOL CHPAction::GetTalkDead(HPACTION_TALK_HPRATE * o_pTalkHPRate) {
	if(NULL == o_pTalkHPRate) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	if(strlen(m_DeadTalk.HPTalk)) {
		o_pTalkHPRate->HPValueRate = m_DeadTalk.HPValueRate;
		o_pTalkHPRate->HPTalkImportance = m_DeadTalk.HPTalkImportance;
		util::strncpy(o_pTalkHPRate->HPTalk, m_DeadTalk.HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
		return TRUE;
	}

	return FALSE;
}

BOOL CHPAction::GetTalkHPRate(MonHP_t i_CurrentMonHP, HPACTION_TALK_HPRATE * o_pTalkHPRate) {
	if(NULL == o_pTalkHPRate) {
		return FALSE;
	}

	vectHPActionTalkHPRate::iterator itr = m_vectHPActionTalkHPRate.begin();
	while (itr != m_vectHPActionTalkHPRate.end()) {
		if(itr->HPValueRate >= i_CurrentMonHP) {
			o_pTalkHPRate->HPValueRate	= itr->HPValueRate;
			o_pTalkHPRate->HPTalkImportance	= itr->HPTalkImportance;
			util::strncpy(o_pTalkHPRate->HPTalk, itr->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
			itr = m_vectHPActionTalkHPRate.erase(itr);
			return TRUE;
		}
		itr++;
	}

	return FALSE;
}

BOOL CHPAction::GetTalkDamagedRandom(HPTalk_t * o_pTalk) {
	if(NULL == o_pTalk) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	int RandomSize = m_vectHPActionTalkDamagedRandom.size();

	if(0 == RandomSize) {
		return FALSE;
	}
	else {
		// 2010-07-19 by dhjin, 확률 수식 변경
//		Prob100_t TalkAllRandom = RANDI(1, PROB100_MAX_VALUE+1);
		Prob100_t TalkAllRandom = RAND100();
		if(HPTALK_DAMAGED_RANDOM_RATE < TalkAllRandom) {
			return FALSE;
		}
		int Random = RANDI(1, RandomSize);
		util::strncpy(o_pTalk, m_vectHPActionTalkDamagedRandom[Random-1].HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);

		return TRUE;
	}

	return FALSE;
}

BOOL CHPAction::GetTalkTargetChange(HPTalk_t * o_pTalk) {
	if(NULL == o_pTalk) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	if(strlen(m_TargetChangeTalk)) {
		util::strncpy(o_pTalk, m_TargetChangeTalk, SIZE_MAX_HPTALK_DESCRIPTION);
		return TRUE;
	}
	
	return FALSE;
}

void CHPAction::EraseHPActionByUseItemArrayIdx(ItemIdx_t i_UseItemArrayIdx) {
	vectHPActionAttackHPRate::iterator itr = m_vectHPActionAttackHPRate.begin();
	while (itr != m_vectHPActionAttackHPRate.end()) {
		if(i_UseItemArrayIdx == itr->UseItemArrayIdx) {
			itr = m_vectHPActionAttackHPRate.erase(itr);
			return;
		}	
		itr++;
	}
}

void CHPAction::SetHPTalkAttack(ItemIdx_t i_ItemArrayIdx, INT i_ItemNum) {
	vectHPActionTalkAttack::iterator itr = m_vectHPActionTalkAttack.begin();
	// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
	for(; itr != m_vectHPActionTalkAttack.end(); itr++) {
		if(itr->UseItemArrayIdx == i_ItemArrayIdx) {
			itr->ItemNum = i_ItemNum;
		}
	}
}

BOOL CHPAction::GetPreHPTalkAttack(ItemIdx_t i_AttackItemNum, HPTalk_t * o_pPreTalk, MSec_t * o_pPreHPCameraTremble) {
	if(NULL == o_pPreTalk
		|| NULL == o_pPreHPCameraTremble) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	// 준비 공격시 대화 및 카메라 떨림 가져오기
	vectHPActionTalkAttack::iterator itr = m_vectHPActionTalkAttack.begin();
	while (itr != m_vectHPActionTalkAttack.end()) {
		if(itr->ItemNum == i_AttackItemNum
			&& m_HPActionAttackTalkUID == itr->HPActionUID		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
			&& ( (0 < itr->PreHPCameraTremble) || (1 < strlen(itr->PreHPTalk) ) ) ) {
			*o_pPreHPCameraTremble = itr->PreHPCameraTremble;
			util::strncpy(o_pPreTalk, itr->PreHPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
			return TRUE;
		}
		itr++;
	}
	
	return FALSE;
}

BOOL CHPAction::GetHPTalkAttack(ItemIdx_t i_AttackItemNum, HPTalk_t * o_pTalk, MSec_t * o_pHPCameraTremble) {
	if(NULL == o_pTalk
		|| NULL == o_pHPCameraTremble) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return FALSE;
	}

	// 공격시 대화 및 카메라 떨림 가져오기
	vectHPActionTalkAttack::iterator itr = m_vectHPActionTalkAttack.begin();
	while (itr != m_vectHPActionTalkAttack.end()) {
		if(itr->ItemNum == i_AttackItemNum
			&& m_HPActionAttackTalkUID == itr->HPActionUID		// 2009-09-09 ~ 2010-01-13 by dhjin, 인피니티 - 한 아이템으로 여러 대사 가능하게 수정
			&& ( (0 < itr->HPCameraTremble) || (1 < strlen(itr->HPTalk) ) ) ) {
			*o_pHPCameraTremble = itr->HPCameraTremble;
			util::strncpy(o_pTalk, itr->HPTalk, SIZE_MAX_HPTALK_DESCRIPTION);
			m_HPActionAttackTalkUID = 0;
			return TRUE;
		}
		itr++;
	}
	
	return FALSE;
}