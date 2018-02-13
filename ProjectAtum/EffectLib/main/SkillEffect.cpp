// SkillEffect.cpp: implementation of the CSkillEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "SkillEffect.h"
#include "CharacterInfo.h"
#include "UnitData.h"
#include "AtumDatabase.h"
#include "dxutil.h"

extern CAtumDatabase* g_pDatabase;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillEffect::CSkillEffect(CUnitData* pParent)
{
	m_pParent = pParent;
}

CSkillEffect::~CSkillEffect()
{

}

HRESULT	CSkillEffect::InitDeviceObjects()
{
	for (auto& info : m_vecSkillEffect)

		if (info.pCharacterInfo != nullptr) info.pCharacterInfo->InitDeviceObjects();

	return S_OK;
}

HRESULT	CSkillEffect::RestoreDeviceObjects()
{
	for (auto& info : m_vecSkillEffect)

		if (info.pCharacterInfo != nullptr) info.pCharacterInfo->RestoreDeviceObjects();

	return S_OK;
}

HRESULT	CSkillEffect::InvalidateDeviceObjects()
{
	for (auto& info : m_vecSkillEffect)
		
		if (info.pCharacterInfo != nullptr) info.pCharacterInfo->InvalidateDeviceObjects();

	return S_OK;
}

HRESULT	CSkillEffect::DeleteDeviceObjects()
{
	for (auto& info : m_vecSkillEffect)

		if (info.pCharacterInfo != nullptr)
		{
			info.pCharacterInfo->DeleteDeviceObjects();
			util::del(info.pCharacterInfo);
		}

	m_vecSkillEffect.clear();

	return S_OK;
}

void CSkillEffect::Tick(float fElapsedTime)
{
	vector<SkillEffectInfo>::iterator it = m_vecSkillEffect.begin();
	while (it != m_vecSkillEffect.end())
	{
		if (it->pCharacterInfo)
		{
			// 2004-10-12 by jschoi  여기서 이펙트 소멸해주는 코드가 필요하다.
			if (it->pCharacterInfo->IsUsing() == FALSE)
			{
				if (it->pCharacterInfo)
				{
					it->pCharacterInfo->InvalidateDeviceObjects();
					it->pCharacterInfo->DeleteDeviceObjects();
					util::del(it->pCharacterInfo);
				}
				it = m_vecSkillEffect.erase(it);
				continue;
			}
			else
			{
				it->pCharacterInfo->SetSingleBodyConditionMatrix(BODYCON_CHARGING_MASK, m_pParent->m_mMatrix);
				it->pCharacterInfo->SetSingleBodyConditionMatrix(BODYCON_USE_SKILL_MASK, m_pParent->m_mMatrix);
				it->pCharacterInfo->Tick(fElapsedTime);
			}
		}
		it++;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillEffect::CreateSkillEffect(int nSkillNum)
/// \brief		nSkillNum에 해당하는 스킬 이펙트를 생성하여 m_vecSkillEffect에 추가한다.
/// \author		jschoi
/// \date		2004-10-11 ~ 2004-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillEffect::CreateSkillEffect(int nSkillNum, BodyCond_t hySingleBodyCondition)
{
	SkillEffectInfo SkillEffect;
	SkillEffect.nSkillNum = SKILL_BASE_NUM(nSkillNum);
	SkillEffect.pCharacterInfo = new CCharacterInfo();
	char strFileName[32];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(nSkillNum);
	//	wsprintf(strFileName,"%08d",SKILL_BASE_NUM(nSkillNum));
	// 2013-03-13 by bhsohn 스킬 번호 구조 수정
	//	wsprintf(strFileName,"%08d",SKILL_BASE_NUM(pItem->SourceIndex));
	wsprintf(strFileName, "%08d", SKILL_BASE_SOURCEINDEX(pItem->SourceIndex));
	// END 2013-03-13 by bhsohn 스킬 번호 구조 수정
	if (SkillEffect.pCharacterInfo->Load(strFileName))
	{
		SkillEffect.pCharacterInfo->InitDeviceObjects();
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		SkillEffect.pCharacterInfo->m_LoadingPriority = _NOTHING_STEP;
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		SkillEffect.pCharacterInfo->RestoreDeviceObjects();
		SkillEffect.pCharacterInfo->SetAllBodyConditionMatrix(m_pParent->m_mMatrix);
		SkillEffect.pCharacterInfo->ChangeBodyCondition(hySingleBodyCondition);
	}
	else
	{
		util::del(SkillEffect.pCharacterInfo);	// 스킬 이펙트가 없는 스킬
		DBGOUT("CSkillEffect::CreateSkillEffect, Can't Find Effect File.\n");
	}
	m_vecSkillEffect.push_back(SkillEffect);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CSkillEffect::DeleteSkillEffect(int nSKillNum)
/// \brief		m_vecSkillEffect에서 nSkillNum에 해당하는 스킬 이펙트를 삭제한다.
/// \author		jschoi
/// \date		2004-10-11 ~ 2004-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillEffect::DeleteSkillEffect(int nSkillNum)
{
	vector<SkillEffectInfo>::iterator it = m_vecSkillEffect.begin();
	while (it != m_vecSkillEffect.end())
	{
		if (it->nSkillNum == SKILL_BASE_NUM(nSkillNum))
		{
			if (it->pCharacterInfo)
			{
				it->pCharacterInfo->InvalidateDeviceObjects();
				it->pCharacterInfo->DeleteDeviceObjects();
				util::del(it->pCharacterInfo);
			}
			it = m_vecSkillEffect.erase(it);
			continue;
			// 2007-04-05 by dgwoo 이펙트가 남는 버그때문에 이후에도 계속 검색한다.
			//break;
		}
		it++;
	}
}

void CSkillEffect::ChangeBodyCondition(int nSkillNum, BodyCond_t hySingleBodyCondition)
{
	for (auto& info : m_vecSkillEffect)
	{
		if (info.nSkillNum == SKILL_BASE_NUM(nSkillNum))
		{
			if (info.pCharacterInfo != nullptr) info.pCharacterInfo->ChangeBodyCondition(hySingleBodyCondition);

			break;
		}
	}
}

void CSkillEffect::SetBodyConditionMatrix(BodyCond_t hySingleBodyCondition, D3DXMATRIX mat)
{
	for (auto& info : m_vecSkillEffect) if (info.pCharacterInfo != nullptr) info.pCharacterInfo->SetSingleBodyConditionMatrix(hySingleBodyCondition, mat);
}

CCharacterInfo* CSkillEffect::FindSkillEffect(int nSkillNum)
{
	for (auto& info : m_vecSkillEffect) if (info.nSkillNum == SKILL_BASE_NUM(nSkillNum)) return info.pCharacterInfo;

	return nullptr;
}

void CSkillEffect::DeleteChargingStateSkillEffect()
{
	vector<SkillEffectInfo>::iterator it = m_vecSkillEffect.begin();
	while (it != m_vecSkillEffect.end())
	{
		ITEM* pSKILL = g_pDatabase->GetServerItemInfo(it->nSkillNum + 1);

		if (pSKILL && pSKILL->SkillType == SKILLTYPE_CHARGING && it->pCharacterInfo &&
			(COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_CHARGING_MASK) ||
			COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_USE_SKILL_MASK)) &&
			(!COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_BULLET_MASK) &&
			!COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_HIT_MASK)))
		{
			it->pCharacterInfo->InvalidateDeviceObjects();
			it->pCharacterInfo->DeleteDeviceObjects();
			util::del(it->pCharacterInfo);
			it = m_vecSkillEffect.erase(it);
			continue;
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			DeleteCastingStateSkillEffect()
/// \brief		캐스팅 타임이 있는 이펙트를 모두 지운다.
/// \author		dgwoo
/// \date		2006-12-13 ~ 2006-12-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSkillEffect::DeleteCastingStateSkillEffect()
{
	vector<SkillEffectInfo>::iterator it = m_vecSkillEffect.begin();
	while (it != m_vecSkillEffect.end())
	{
		ITEM* pSKILL = g_pDatabase->GetServerItemInfo(it->nSkillNum + 1);

		if (pSKILL &&
			pSKILL->AttackTime > 0 &&
			it->pCharacterInfo &&
			(COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_CHARGING_MASK) ||
			COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_USE_SKILL_MASK)) &&
			(!COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_BULLET_MASK) &&
			!COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_HIT_MASK)))
		{
			it->pCharacterInfo->InvalidateDeviceObjects();
			it->pCharacterInfo->DeleteDeviceObjects();
			util::del(it->pCharacterInfo);
			it = m_vecSkillEffect.erase(it);
			continue;
		}
		it++;
	}
}


// 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현
void CSkillEffect::DeleteCastingStateMonsterSkillEffect(void)
{
	vector<SkillEffectInfo>::iterator it = m_vecSkillEffect.begin();
	while (it != m_vecSkillEffect.end())
	{
		ITEM* pSKILL = g_pDatabase->GetServerItemInfo(it->nSkillNum + 1);
		// 2012-03-06 by jhahn 빅붐 이펙트 버그 수정
		bool BigBoom = true; // huh?

		for (auto paramVal : pSKILL->ArrParameterValue) if (paramVal == DES_SKILL_BIG_BOOM) BigBoom = true; // isnt it true anyway?

		// for (int i = 0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++) if (pSKILL->ArrParameterValue[i] == DES_SKILL_BIG_BOOM) BigBoom = true;

		//end 2012-03-06 by jhahn 빅붐 이펙트 버그 수정

		if (pSKILL && pSKILL->AttackTime > 0 && it->pCharacterInfo && (COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_CHARGING_MASK)
			|| COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_USE_SKILL_MASK))
			&& (!COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_BULLET_MASK)
			&& !COMPARE_BODYCON_BIT(it->pCharacterInfo->m_nCurrentBodyCondition, BODYCON_HIT_MASK))
			&& pSKILL->Kind == ITEMKIND_FOR_MON_SKILL || BigBoom) // 2012-03-06 by jhahn 빅붐 이펙트 버그 수정
		{
			it->pCharacterInfo->InvalidateDeviceObjects();
			it->pCharacterInfo->DeleteDeviceObjects();
			util::del(it->pCharacterInfo);
			it = m_vecSkillEffect.erase(it);
			continue;
		}
		it++;
	}
}
// end 2009. 09. 21 by ckPark 인피니티 필드 몬스터 스킬 구현

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSkillEffect::CheckTargetSkillEffect(INT ItemNum)
/// \brief		타겟 스킬 이펙트가 필요한가?
/// \author		ispark
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSkillEffect::CheckTargetSkillEffect(INT ItemNum)
{
	ITEM *pSkillItem = g_pDatabase->GetServerItemInfo(ItemNum);

	if (pSkillItem->IsExistDesParam(DES_SKILL_SCANNING)) return TRUE;

	return FALSE;
}
