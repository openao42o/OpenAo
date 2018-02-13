// NPCSkillManager.cpp: implementation of the CNPCSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NPCSkillManager.h"

#include <timeapi.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCSkillManager::CNPCSkillManager()
{
	this->InitSkill();
}

CNPCSkillManager::~CNPCSkillManager()
{

}

void CNPCSkillManager::InitSkill()
{
	m_TimeLimitSkillList.clear();
}


/**********************************************************************************************************
**
**	적용할 버프 스킬 등록처리.
**
**	Create Info : ??. ??. ??
**
**	Update Info : 2010. 06. 08. by hsLee. - 스킬 효과 증가율 인자 추가. ( a_fIncreasePowerRatio );
**
***********************************************************************************************************/
void CNPCSkillManager::ApplyTimeLimitSkill( ITEM * i_UseSkill , const float a_fIncreasePowerRatio /*= 1.0f*/ )
{
	if(NULL == i_UseSkill
		|| ORBIT_SKILL_DEBUFF_TIMELIMIT == i_UseSkill->OrbitType
		|| ORBIT_SKILL_DOT_TIMELIMIT == i_UseSkill->OrbitType)
	{	// 몬스터 자신에게 걸리는 타임 버프만 설정해준다.
		return;
	}

	DWORD dwCurTick = timeGetTime();
	
	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	for(; itr != m_TimeLimitSkillList.end(); itr++)
	{
		if(i_UseSkill->ItemNum == itr->Skill->ItemNum)
		{	// 이미 사용중이면 시간 다시 초기화 시킨다.
			itr->dwSkillUseTick	= dwCurTick;

			// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
			itr->fIncreasePowerRation = a_fIncreasePowerRatio;

			return;
		}
	}
	mta.auto_unlock_cancel();

	MONSTER_SKILL_INFO ApplySkill;
	util::zero(&ApplySkill, sizeof(MONSTER_SKILL_INFO));
	ApplySkill.Skill				= i_UseSkill;
	ApplySkill.dwSkillUseTick		= dwCurTick;

	// 2010. 06. 08 by hsLee 인피니티 필드 2차 난이도 조절. (아군 동일 밸런스 적용.) - 몬스터 스킬 밸런스 적용.
	ApplySkill.fIncreasePowerRation = a_fIncreasePowerRatio;

	m_TimeLimitSkillList.pushBackLock(ApplySkill);
}

ItemNum_t CNPCSkillManager::ReleaseTimeLimitSkill(ItemNum_t i_ExpireSkillNum)
{
	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	for(; itr != m_TimeLimitSkillList.end(); itr++)
	{
		if(i_ExpireSkillNum == itr->Skill->ItemNum)
		{
			m_TimeLimitSkillList.erase(itr);
			return i_ExpireSkillNum;
		}
	}

	return FALSE;
}

BOOL CNPCSkillManager::CheckExpireTimeLimitSkill(ItemNum_t i_CheckExpireSkillNum)
{
	DWORD dwCurTick = timeGetTime();

	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	for(; itr != m_TimeLimitSkillList.end(); itr++)
	{
		if(i_CheckExpireSkillNum == itr->Skill->ItemNum
			&& (dwCurTick - itr->dwSkillUseTick) < itr->Skill->Time )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CNPCSkillManager::AllReleaseTimeLimitSkill(vector<ItemNum_t> * o_pReleaseSkillNumList)
{
	DWORD dwCurTick = timeGetTime();
	
	mt_auto_lock mta(&m_TimeLimitSkillList);
	mtMonsterSkillInfoList::iterator itr = m_TimeLimitSkillList.begin();
	while(itr != m_TimeLimitSkillList.end())
	{
		if((dwCurTick - itr->dwSkillUseTick) > itr->Skill->Time)
		{
			o_pReleaseSkillNumList->push_back(itr->Skill->ItemNum);
			itr = m_TimeLimitSkillList.erase(itr);
			continue;
		}
		itr++;
	}
}