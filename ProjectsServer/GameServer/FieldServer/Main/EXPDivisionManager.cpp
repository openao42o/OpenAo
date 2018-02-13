// EXPDivisionManager.cpp: implementation of the CEXPDivisionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EXPDivisionManager.h"
#include "FieldIOCPSocket.h"
#include "FieldParty.h"
#include "FieldGlobal.h"		// 2009-12-14 by cmkwon, 2009 크리스마스이벤트(한명당 아이템 하나씩) - 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEXPDivisionManager::CEXPDivisionManager(CFieldIOCP *i_pFIOCP, CFieldMapChannel *i_pFMChann)
{
	m_pFieldIOCP15		= i_pFIOCP;
	m_pFieldMapChannel1	= i_pFMChann;

	m_fTotalOfDamage	= 0.0f;

}

CEXPDivisionManager::~CEXPDivisionManager()
{
	listMonsterDropItem::iterator itr(m_litMonsterDropItem.begin());
	for(; itr != m_litMonsterDropItem.end(); itr++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itr;
		util::del(tmpMonsterDropItemGeneral.pItemGeneral);		
	}
	m_litMonsterDropItem.clear();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			float CEXPDivisionManager::GetTotalOfDamage(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CEXPDivisionManager::GetTotalOfDamage(void)
{
	return m_fTotalOfDamage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CEXPDivisionManager::GetvectSEXP_DIVISIONListSize(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CEXPDivisionManager::GetvectSEXP_DIVISIONListSize(void)
{
	return m_vectSEXP_DIVISIONList.size();
};


///////////////////////////////////////////////////////////////////////////////
/// \fn			listMonsterDropItem *CEXPDivisionManager::GetlitMonsterDropItemPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
listMonsterDropItem *CEXPDivisionManager::GetlitMonsterDropItemPtr(void)
{
	return &m_litMonsterDropItem;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::AddEXPDivision(CFieldIOCPSocket *i_pFISock, float i_fDamage, float i_fAggro)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::AddEXPDivision(CFieldIOCPSocket *i_pFISock, float i_fDamage, float i_fAggro)
{
	m_fTotalOfDamage	+= i_fDamage;

	CFieldParty *pFParty = i_pFISock->m_pFieldParty;
	SEXP_DIVISION tmEXPDivision(i_pFISock, pFParty, i_fDamage, i_fAggro);
	if(NULL == pFParty)
	{
		m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 분배 타입에 따라 처리
	switch(pFParty->GetExpDistributeType())
	{
	case EXP_DISTRIBUTE_TYPE_DAMAGE:
		{
			tmEXPDivision.bIsApplyPartyBonus	= TRUE;		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 항상 TRUE
			SEXP_DIVISION *pPartyExpDivision = FindEXPDivisionByFPartyPtr(pFParty);
			if(NULL == pPartyExpDivision)
			{
				m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);
				return;
			}

			pPartyExpDivision->AddPartyMember(&tmEXPDivision);
		}
		break;
	case EXP_DISTRIBUTE_TYPE_EQUALITY:
		{
			BYTE byPMaxLevel = pFParty->GetMaxMemberLevelInSameMap(m_pFieldMapChannel1);
			if(MAX_LEVEL_GAP_APPLY_PARTY_BONUS >= byPMaxLevel - i_pFISock->GetCharacter()->Level)
			{// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 파티 보너스 유효 사용자

				tmEXPDivision.bIsApplyPartyBonus	= TRUE;		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 -
				SEXP_DIVISION *pPartyExpDivision = FindEXPDivisionByFPartyPtr(pFParty);
				if(NULL == pPartyExpDivision)
				{
					m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);
					return;
				}

				pPartyExpDivision->AddPartyMember(&tmEXPDivision);
			}
			else
			{// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 파티 보너스 제외 사용자
			
				tmEXPDivision.bIsApplyPartyBonus	= FALSE;		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 -
				m_vectSEXP_DIVISIONList.push_back(tmEXPDivision);	// 2007-06-27 by cmkwon, 파티 검색 없이 추가로 처리
			}
		}
	default:
		{
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SEXP_DIVISION *CEXPDivisionManager::FindEXPDivisionByFPartyPtr(CFieldParty *i_pFParty)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SEXP_DIVISION *CEXPDivisionManager::FindEXPDivisionByFPartyPtr(CFieldParty *i_pFParty)
{
	if(NULL == i_pFParty){					return NULL;}

	int nCnts = m_vectSEXP_DIVISIONList.size();
	for(int i=0; i < nCnts; i++)
	{
		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 아래와 같이 수정, 파티 경험치 보너스가 유효한 것만 리턴함
		//if(i_pFParty == m_vectSEXP_DIVISIONList[i].pFParty0)
		if(i_pFParty == m_vectSEXP_DIVISIONList[i].pFParty0
			&& m_vectSEXP_DIVISIONList[i].bIsApplyPartyBonus)
		{
			return &m_vectSEXP_DIVISIONList[i];
		}
	}

	return NULL;
}


struct SSort_SEXP_DIVISION_BY_DAMAGE: binary_function<SEXP_DIVISION, SEXP_DIVISION, bool>
{
	bool operator()(SEXP_DIVISION expDivision1, SEXP_DIVISION expDivision2)
	{
        return expDivision1.fSumOfDamage > expDivision2.fSumOfDamage;		// 내림 차순 정렬을 위해 반대로 함
    };
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::SortByDamageDecrease(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::SortByDamageDecrease(void)
{
	sort(m_vectSEXP_DIVISIONList.begin(), m_vectSEXP_DIVISIONList.end(), SSort_SEXP_DIVISION_BY_DAMAGE());	
}

struct SSort_SEXP_PARTYMEMBER_BY_DAMAGE: binary_function<SEXP_PARTYMEMBER, SEXP_PARTYMEMBER, bool>
{
	bool operator()(SEXP_PARTYMEMBER param1, SEXP_PARTYMEMBER param2)
	{
        return param1.fDamage > param2.fDamage;		// 내림 차순 정렬을 위해 반대로 함
    };
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CEXPDivisionManager::GetSEXP_DIVISIONOfTopDamage(SEXP_DIVISION *o_pExpDivision)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-25 ~ 2006-08-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CEXPDivisionManager::GetSEXP_DIVISIONOfTopDamage(SEXP_DIVISION *o_pExpDivision)
{
	if(m_vectSEXP_DIVISIONList.empty())
	{
		return FALSE;
	}

	SEXP_DIVISION *pExpDivision = &m_vectSEXP_DIVISIONList[0];
	if(pExpDivision->pFParty0
		&& 2 <= pExpDivision->nPartyMemberCnts)
	{
		sort(&(pExpDivision->ArrSEXP_PARTYMEMBER[0]), &(pExpDivision->ArrSEXP_PARTYMEMBER[pExpDivision->nPartyMemberCnts]), SSort_SEXP_PARTYMEMBER_BY_DAMAGE());
	}
	
	*o_pExpDivision	= m_vectSEXP_DIVISIONList[0];
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessEXPDivision(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster, CFieldMonster *pTargetMonster/*=NULL*/)
/// \brief		데미지별 경험치 분배 처리, 파티 보너스율 계산 완료
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CEXPDivisionManager::ProcessEXPDivision(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster,CFieldMonster *pTargetMonster/*=NULL*/)
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
{
	int nCountForBossMonsterLog		= 0;
	float fTempDelayRatio			= 0.0f;
	int nCnt						= m_vectSEXP_DIVISIONList.size();
	for(int i=0; i < nCnt; i++)	
	{
		SEXP_DIVISION *pExpDivision = &m_vectSEXP_DIVISIONList[i];
		CFieldIOCPSocket *pFISock	= pExpDivision->ArrSEXP_PARTYMEMBER[0].pFISock0;

		// 경험치 계산
		Experience_t fExp			= i_exp * pExpDivision->fSumOfDamage/m_fTotalOfDamage;
		if (NULL == pExpDivision->pFParty0 
			|| 1 >= pExpDivision->pFParty0->m_ListPartyMember.size()) //혼자 사냥중일때
		{
			// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
			pFISock->ChangeExperience(fExp, TRUE, TRUE, TRUE,1,pTargetMonster);		// 2011-09-02 by hskim, 파트너 시스템 2차
			// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
			
			
			// Drop Item Delay관련
			if (o_pDropItem->priorityInfoCount < SIZE_MAX_DROP_ITEM_PRIORITY_INFO)
			{
				o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].CharacterUniqueNumber	= pFISock->GetCharacter()->CharacterUniqueNumber;
				o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].dwDelay					= (DWORD)((float)MAX_DROPITEM_DELAY * fTempDelayRatio);
				o_pDropItem->priorityInfoCount++;
			}

			if (i_bBossMonster
				&& pFISock
				&& pFISock->IsValidCharacter()
				&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
			{
				util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
				nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, 버그 수정
			}
		}
		else	// 파티 사냥중일때
		{
			// 파티원 경험치 분배
			CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
			util::zero(ArrayPartyMember, sizeof(CFieldIOCPSocket*)*SIZE_MAX_PARTY_MEMBER);
			
			int nPartyBonusMemberCnts	= 0;
			BYTE byPartyMaxLevel		= 1;
			int nMemberCnts				= 0;
			
			// 2007-06-26 by cmkwon, 파티 경험치 관련 수정 - 아래와 같이 죽은 사람도 포함
			//nMemberCnts = pExpDivision->pFParty0->GetMembersInSameMap(ArrayPartyMember, m_pFieldMapChannel1, NULL, 0.0f, &byPartyMaxLevel);
			nMemberCnts = pExpDivision->pFParty0->GetMembersInSameMap(ArrayPartyMember, m_pFieldMapChannel1, FALSE, &byPartyMaxLevel);
			int j;
			for(j=0; j < nMemberCnts; j++)
			{
				CFieldIOCPSocket *p2FISock	= ArrayPartyMember[j];
				if(NULL == p2FISock
					|| FALSE == p2FISock->IsValidCharacter())
				{// 2006-07-18 by cmkwon, 유효 파티원 체크
					continue;
				}

				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, 파티보상 인원수 체크
				// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 아래와 같이 수정
				//if(byPartyMaxLevel-7 <= p2FISock->GetCharacter()->Level)
				if(MAX_LEVEL_GAP_APPLY_PARTY_BONUS >= byPartyMaxLevel-p2FISock->GetCharacter()->Level)
				{
					nPartyBonusMemberCnts++;
				}

				///////////////////////////////////////////////////////////////////////////////
				// Drop Item Delay관련 - Delay는 모든 파티원에게 동일하게 설정
				if (o_pDropItem->priorityInfoCount < SIZE_MAX_DROP_ITEM_PRIORITY_INFO)
				{
					o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].CharacterUniqueNumber	= p2FISock->GetCharacter()->CharacterUniqueNumber;
					o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].dwDelay					= (DWORD)((float)MAX_DROPITEM_DELAY * fTempDelayRatio);
					o_pDropItem->priorityInfoCount++;
				}
			}

			///////////////////////////////////////////////////////////////////////////////
			// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 경험치 분배 타입별로 경험치 분배 처리
			if(pExpDivision->bIsApplyPartyBonus) //파티원 보너스 경험치를 줘야 할 경우
			{
				pExpDivision->fPartyBonus	= CAtumSJ::GetPartyBonusExpWeight(nPartyBonusMemberCnts);
				// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
#else
				fExp						*= pExpDivision->fPartyBonus;
#endif //S_BONUSEXPSYSTEM_RENEWAL
				// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
				switch(pExpDivision->pFParty0->GetExpDistributeType())
				{
				case EXP_DISTRIBUTE_TYPE_DAMAGE:
					{
						float fPartySumOfDamage		= pExpDivision->fSumOfDamage + pExpDivision->fSumOfAggro;		// 2006-07-21 by cmkwon, 데미지합과 어그로의 합
						// Drop Item 관련
						for (j = 0; j < pExpDivision->nPartyMemberCnts; j++)
						{
							CFieldIOCPSocket *p2FISock = pExpDivision->ArrSEXP_PARTYMEMBER[j].pFISock0;

							if(NULL == p2FISock
								|| FALSE == p2FISock->IsValidCharacter())
							{// 2006-07-18 by cmkwon, 유효 파티원 체크
								continue;
							}

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, 경험치 보상
							// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
							p2FISock->ChangeExperience(fExp * (pExpDivision->ArrSEXP_PARTYMEMBER[j].GetPartyDamage()/fPartySumOfDamage), TRUE, TRUE, TRUE, nMemberCnts, pTargetMonster);		// 2011-09-02 by hskim, 파트너 시스템 2차
							// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, 보스몬스터 공격 로그
							if (i_bBossMonster
								&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
							{
								util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], p2FISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
								nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, 버그 수정
							}
						}
					}
					break;
				case EXP_DISTRIBUTE_TYPE_EQUALITY:
					{
						for(int j=0; j < nMemberCnts; j++)
						{
							CFieldIOCPSocket *p2FISock = ArrayPartyMember[j];
							if(NULL == p2FISock
								|| FALSE == p2FISock->IsValidCharacter()
								|| MAX_LEVEL_GAP_APPLY_PARTY_BONUS < byPartyMaxLevel-p2FISock->GetCharacter()->Level)
							{// 2006-07-18 by cmkwon, 유효 파티원 체크
								continue;
							}

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, 경험치 보상
							// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
							p2FISock->ChangeExperience(fExp / max(nPartyBonusMemberCnts,1), TRUE, TRUE, TRUE, nMemberCnts, pTargetMonster);		// 2011-09-02 by hskim, 파트너 시스템 2차
							// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼

							///////////////////////////////////////////////////////////////////////////////
							// 2006-07-18 by cmkwon, 보스몬스터 공격 로그
							if (i_bBossMonster
								&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
							{
								util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], p2FISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
								nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, 버그 수정
							}
						}
					}
					break;
				}
			}
			else
			{
				// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
				pFISock->ChangeExperience(fExp, TRUE, TRUE, TRUE, 1, pTargetMonster);		// 2011-09-02 by hskim, 파트너 시스템 2차
				// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
				///////////////////////////////////////////////////////////////////////////////
				// 2006-07-18 by cmkwon, 보스몬스터 공격 로그
				if (i_bBossMonster
					&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
				{
					util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
					nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, 버그 수정
				}
			}
		}

		fTempDelayRatio += (pExpDivision->fSumOfDamage/m_fTotalOfDamage);		// Delay 설정을 위해 합산
	}// END - for(int i=0; i < nCnt; i++)
}


struct DROP_ITEM_PRIORITY
{
	ITEM_GENERAL* pDropItemGeneral;
	int iPriority;
};

struct SORT_DROP_ITEM_PRIORITY
{
	bool operator()(DROP_ITEM_PRIORITY op1, DROP_ITEM_PRIORITY op2)
	{
		return op1.iPriority < op2.iPriority;
	}
};	

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessPickUpDropItems(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CFieldIOCPSocket *i_pFISockTopDamage)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-13 ~ 2006-07-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::ProcessPickUpDropItems(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CFieldIOCPSocket *i_pFISockTopDamage)
{
	if(1 > m_litMonsterDropItem.size())
	{
		return;
	}

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	D3DXVECTOR3 vec3MonPos = i_pDeadFMonster->PositionVector;
	if(0.0f > vec3MonPos.x || 0.0f > vec3MonPos.z)
	{		
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
		return;
	}

	MONSTER_INFO *pMonsterInfo = i_pDeadFMonster->MonsterInfoPtr;
	if(NULL == pMonsterInfo)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-14 by cmkwon, 2009 크리스마스이벤트(한명당 아이템 하나씩) - 크레이지스노우맨(2060600), 순차적 AutoPickup
	BOOL bAutoPickUpOfXMasDrop	= FALSE;
	vector<DROP_ITEM_PRIORITY> v_DropItemGeneral;
	// 2010-06-21 by jskim, 2010 이벤트(한명당 아이템 하나씩) - 오리진(2098200), 순차적 AutoPickup
	BOOL bAutoPickUpOfEventDrop	= FALSE;
	if(2060600 == pMonsterInfo->MonsterUnitKind)
	{
		bAutoPickUpOfXMasDrop	= TRUE;
	}
	// 2010-06-21 by jskim, 2010 이벤트(한명당 아이템 하나씩) - 오리진(2098200), 순차적 AutoPickup
	if(2098200 == pMonsterInfo->MonsterUnitKind)
	{
		bAutoPickUpOfEventDrop	= TRUE;
	}

	mtlistMonsterDropItem::iterator itrDropItem = m_litMonsterDropItem.begin();
	for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itrDropItem;
		ITEM_GENERAL *pDropItemGeneral		= tmpMonsterDropItemGeneral.pItemGeneral;
		ITEM *pDropItemInfo					= tmpMonsterDropItemGeneral.pItemGeneral->ItemInfo;
		
		if (COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER))
		{
			*o_pStrDropItemlist += pDropItemInfo->ItemName;
			*o_pStrDropItemlist += "|";
		}

		BYTE ItemUpdateType = IUT_DROP_ITEM;
		if (COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_AUTO_PICK_UP)
			//|| bAutoPickUpOfXMasDrop)
			|| bAutoPickUpOfXMasDrop || bAutoPickUpOfEventDrop)		// 2009-12-14 by cmkwon, 2009 크리스마스이벤트(한명당 아이템 하나씩) - // 2010-06-21 by jskim, 2010 이벤트(한명당 아이템 하나씩) - 오리진(2098200)
		{
			///////////////////////////////////////////////////////////////
			// auto pickup인 경우
			///////////////////////////////////////////////////////////////
			if (NULL == i_pFISockTopDamage
				|| FALSE == i_pFISockTopDamage->IsValidCharacter())
// 2006-11-16 by cmkwon, 시스템 변경으로 필요 없음
//				|| FALSE == i_pFISockTopDamage->IsValidGameUser())		// 2006-06-14 by cmkwon, 베트남 정액제 체크 - 자동 습득 아이템
			{
				util::del(pDropItemGeneral);		// just delete item
				continue;
			}

			BOOL bPickUP = FALSE;
			if (COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_QUEST_ITEM) || pDropItemInfo->Kind == ITEMKIND_QUEST)
			{
				///////////////////////////////////////////////////////
				// quest item인 경우
				bPickUP = TRUE;
				ItemUpdateType = IUT_QUEST;
			}
			else
			{
				///////////////////////////////////////////////////////
				// quest item이 아닌 경우
				if (pDropItemGeneral->ItemNum == MONEY_ITEM_NUMBER
					&& FALSE == CAtumSJ::CanPickUpDropMoney(i_pFISockTopDamage->m_character.Level, pMonsterInfo))
				{
					bPickUP = FALSE;
				}
				else
				{
					bPickUP = TRUE;
					ItemUpdateType = IUT_DROP_ITEM;
				}
			}

			if (FALSE == bPickUP)
			{// 2006-03-17 by cmkwon, 
				util::del(pDropItemGeneral);		// just delete item
				continue;
			}

			pDropItemGeneral->AccountUniqueNumber	= i_pFISockTopDamage->m_character.AccountUniqueNumber;
			pDropItemGeneral->Possess				= i_pFISockTopDamage->m_character.CharacterUniqueNumber;
// 2009-08-25 by cmkwon, 사용하지 않는 필드 제거(td_Store.ScarcityNumber) - 
//			pDropItemGeneral->ScarcityNumber		= 0;
			pDropItemGeneral->Wear					= WEAR_NOT_ATTACHED;
			pDropItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;	// 서버에서는 아이템 창의 첫 slot에 넣어두고, 정리는 client에서 한다.
			pDropItemGeneral->NumOfEnchants			= 0;

			if (pDropItemInfo->ItemNum == MONEY_ITEM_NUMBER)
			{
				CFieldParty *pFirstAttackCharacterFParty = i_pFISockTopDamage->m_pFieldParty;
				if(NULL == pFirstAttackCharacterFParty)
				{
					i_pFISockTopDamage->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount, ItemUpdateType);
				}
				else
				{
					SEXP_DIVISION *pExpDivision = this->FindEXPDivisionByFPartyPtr(pFirstAttackCharacterFParty);
					if(NULL == pExpDivision
						|| 1 >= pExpDivision->nPartyMemberCnts
						|| FALSE == pExpDivision->bIsApplyPartyBonus)	// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 파티 보너스 적용 여부 체크 추가
					{
						i_pFISockTopDamage->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount, ItemUpdateType);
					}
					else
					{
						///////////////////////////////////////////////////////////////////////////////					
						// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 경험치 분배 타입별로 SPI 분배 처리
						switch(pFirstAttackCharacterFParty->GetExpDistributeType())
						{
						case EXP_DISTRIBUTE_TYPE_DAMAGE:
							{
								for (int i = 0; i < pExpDivision->nPartyMemberCnts; i++)
								{
									if(pExpDivision->ArrSEXP_PARTYMEMBER[i].pFISock0
										&& pExpDivision->ArrSEXP_PARTYMEMBER[i].pFISock0->IsValidCharacter())
									{
										pExpDivision->ArrSEXP_PARTYMEMBER[i].pFISock0->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount * pExpDivision->ArrSEXP_PARTYMEMBER[i].fDamage/pExpDivision->fSumOfDamage, ItemUpdateType);
									}
								}
							}
							break;
						case EXP_DISTRIBUTE_TYPE_EQUALITY:
							{
								// 2007-06-27 by cmkwon, 유효한 파티원 모두에게 균등하게 분배
								CFieldIOCPSocket *arrFPMem[SIZE_MAX_PARTY_MEMBER] = {NULL,};
								int nPFMemCnts = pFirstAttackCharacterFParty->GetMembersInSameMap(arrFPMem, m_pFieldMapChannel1, TRUE);
								for(int i=0; i < nPFMemCnts; i++)
								{
									CFieldIOCPSocket *pFISoc = arrFPMem[i];
									if(NULL == pFISoc
										|| FALSE == pFISoc->IsValidCharacter())
									{
										continue;
									}
									pFISoc->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount / nPFMemCnts, ItemUpdateType);
								}
							}
							break;
 						}
					}
				}
				util::del(pDropItemGeneral);
				continue;
			}

			if (tmpMonsterDropItemGeneral.QuestIndex == 0)
			{// 퀘스트 아이템이 아닌 일반 아이템인 경우

				///////////////////////////////////////////////////////////////////////////////
				// 2009-12-14 by cmkwon, 2009 크리스마스이벤트(한명당 아이템 하나씩) - 한명당 하나의 아이템을 습득하게 처리
				if(bAutoPickUpOfXMasDrop)
				{
					//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[TEMP] 091214 %s Item(%d:%d)\r\n"
					//	, GetCharacterString(i_pFISockTopDamage->GetCharacter(), string()), pDropItemGeneral->ItemNum, pDropItemGeneral->CurrentCount);
					
					if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
					{
						util::del(pDropItemGeneral);
					}
					
					i_pFISockTopDamage = this->GetNextUser(i_pFISockTopDamage, FALSE);
					continue;
				}
				if(bAutoPickUpOfEventDrop)	 // 2010-06-21 by jskim, 2010 이벤트(한명당 아이템 하나씩) - 오리진(2098200)
				{
					DROP_ITEM_PRIORITY TempPriority;
					
					switch(pDropItemGeneral->ItemNum)
					{
					case 7029820 :
						{
							TempPriority.iPriority = 1;
						}
						break;
					case 7029830 :
						{
							TempPriority.iPriority = 2;
						}
						break;
					case 7029910 :
						{
							TempPriority.iPriority = 3;
						}
						break;
					case 7029920 :
						{
							TempPriority.iPriority = 4;
						}
						break;
					case 7029840 :
						{
							TempPriority.iPriority = 5;
						}
						break;
					case 7029930 :
						{
							TempPriority.iPriority = 6;
						}
						break;
					case 7029940 :
						{
							TempPriority.iPriority = 7;
						}
						break;
					case 7029850 :
						{
							TempPriority.iPriority = 8;
						}
						break;
					case 7029950 :
						{
							TempPriority.iPriority = 9;
						}
						break;
					case 7029960 :
						{
							TempPriority.iPriority = 10;
						}
						break;
					default:
						continue;
					}

					TempPriority.pDropItemGeneral = pDropItemGeneral;
					v_DropItemGeneral.push_back(TempPriority);
					continue;
				}


				//////////////////////////////////////////////////////////////////////////
				// 2008-06-03 by dhjin, EP3 편대 수정 -	아이템 분배 방식 추가로 밑과 같이 수정
				CFieldParty *pFirstAttackCharacterFParty = i_pFISockTopDamage->m_pFieldParty;

				// 2009-01-21 by dhjin, 개인이 올수도 있으므로 수정한다.
				if(NULL == pFirstAttackCharacterFParty)
				{
					if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
					{
						util::del(pDropItemGeneral);
					}
					continue;
				}

				switch(pFirstAttackCharacterFParty->GetItemDistributeType())
				{
				case ITEM_DISTRIBUTE_TYPE_DAMAGE:
					{
						// 2006-09-27 by cmkwon, 퀘스트 아이템은 인벤이 가득차도 들어올 수 있도록 조정
						if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
						{
							util::del(pDropItemGeneral);
						}
					}
					break;
				case ITEM_DISTRIBUTE_TYPE_ORDER:
					{
						BYTE tmPivotPartyMember = 0;
						CFieldIOCPSocket *pFISoc = pFirstAttackCharacterFParty->GetItemByOrderDistributeType(m_pFieldMapChannel1, &tmPivotPartyMember);
						if(NULL == pFISoc
							|| FALSE == pFISoc->IsValidCharacter())
						{
							util::del(pDropItemGeneral);
							continue;
						}
						// 2006-09-27 by cmkwon, 퀘스트 아이템은 인벤이 가득차도 들어올 수 있도록 조정
						if(FALSE == pFISoc->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
						{
							util::del(pDropItemGeneral);
						}						
		
						pFirstAttackCharacterFParty->IncreasePivotPartyMember(tmPivotPartyMember);
					}
					break;
				case ITEM_DISTRIBUTE_TYPE_RANDOM:
					{
						CFieldIOCPSocket *pFISoc = pFirstAttackCharacterFParty->GetItemByRandomDistributeType(m_pFieldMapChannel1);
						if(NULL == pFISoc
							|| FALSE == pFISoc->IsValidCharacter())
						{
							util::del(pDropItemGeneral);
							continue;
						}
						// 2006-09-27 by cmkwon, 퀘스트 아이템은 인벤이 가득차도 들어올 수 있도록 조정
						if(FALSE == pFISoc->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType))
						{
							util::del(pDropItemGeneral);
						}
					}
					break;		
				}				
				continue;
			}
			
			///////////////////////////////////////////////////////////////////////////////
			// 퀘스트 아이템인 경우
			CQuest *pQuestInfo	= m_pFieldIOCP15->m_mapQuestInfoFromScript.findEZ(tmpMonsterDropItemGeneral.QuestIndex);
			if(NULL == pQuestInfo)
			{
				util::del(pDropItemGeneral);
				continue;
			}

			if (pQuestInfo->DropItemToAllAttackCharacter == FALSE)
			{// 최고 데미지 캐릭에게만 주는 경우
				
				if (FALSE == i_pFISockTopDamage->IsQuestStateInProgressByQuestIndex(tmpMonsterDropItemGeneral.QuestIndex))
				{
					util::del(pDropItemGeneral);
					continue;
				}

				if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(pDropItemGeneral, ItemUpdateType, TRUE))
				{
					util::del(pDropItemGeneral);
				}
				continue;				
			}

			///////////////////////////////////////////////////////////////////////////////
			// 파티원 모두에게 다 주는 퀘스트 아이템
			// 일단 아이템은 할당 후 지우고 시작
			INT nQuestItemNum		= pDropItemGeneral->ItemNum;
			INT nQuestItemCount		= pDropItemGeneral->CurrentCount;
			ITEM *pQuestItemInfo	= pDropItemInfo;
			util::del(pDropItemGeneral);

			// 나누어 주기
			for(int i=0; i < m_vectSEXP_DIVISIONList.size(); i++)
			{
				SEXP_DIVISION *pExpDivision = &m_vectSEXP_DIVISIONList[i];
				CFieldIOCPSocket *pFISock	= pExpDivision->ArrSEXP_PARTYMEMBER[0].pFISock0;
				
				// 2008-01-18 by cmkwon, S_F: 퀘스트 아이템 습득 방식 버그 수정 - 퀘스트 보너스하고는 상관 없는 부분임, bIsApplyPartyBonus 는 체크 필요 없음
				//if(NULL == pExpDivision->pFParty0
				//	|| FALSE == pExpDivision->bIsApplyPartyBonus)		// 2007-06-27 by cmkwon, 파티원 경험치 분배 방식 시스템 추가 - 파티 보너스 적용 여부 체크 추가
				if(NULL == pExpDivision->pFParty0)
				{
					if(pFISock
						&& pFISock->IsValidCharacter())
					{
						if(FALSE == pQuestInfo->IsPartyQuest())
						{// 2006-03-24 by cmkwon, 파티퀘스트가 아닐때만 아이템이 추가된다.

							if (pFISock->IsQuestStateInProgressByQuestIndex(tmpMonsterDropItemGeneral.QuestIndex))
							{
								pFISock->m_ItemManager.InsertItemBaseByItemNum(nQuestItemNum, nQuestItemCount, IUT_QUEST, TRUE);
							}
						}
					}
				}
				else
				{
					CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
					util::zero(ArrayPartyMember, sizeof(ArrayPartyMember[0])*SIZE_MAX_PARTY_MEMBER);
					// 2007-06-26 by cmkwon, 파티 경험치 관련 수정 - 살아 있는 캐릭터만 처리
					int nMemCount = pExpDivision->pFParty0->GetMembersInSameMap(ArrayPartyMember, pFISock->m_pCurrentFieldMapChannel, TRUE);
					for(int j=0; j < nMemCount; j++)
					{
						if (ArrayPartyMember[j]->IsQuestStateInProgressByQuestIndex(tmpMonsterDropItemGeneral.QuestIndex))
						{
							ArrayPartyMember[j]->m_ItemManager.InsertItemBaseByItemNum(nQuestItemNum, nQuestItemCount, IUT_QUEST, TRUE);
						}
					}
				}
			}// end_for(int i=0; i < m_vectSEXP_DIVISIONList.size(); i++)
		}// end_if (COMPARE_BIT_FLAG(pDropItemInfo->ItemAttribute, ITEM_ATTR_AUTO_PICK_UP))
		else
		{
			///////////////////////////////////////////////////////////////
			// auto pickup이 아닌 경우
			///////////////////////////////////////////////////////////////
			if (NULL == i_pFISockTopDamage
				|| FALSE == i_pFISockTopDamage->IsValidCharacter()
				|| FALSE == CAtumSJ::CanPickUpDropItem(i_pFISockTopDamage->m_character.Level, pMonsterInfo))
			{
				util::del(pDropItemGeneral);		// just delete item
			}
			else
			{
				// Rafix 여부 검사
				if (FALSE == CAtumSJ::CanPickUpDropRare(i_pFISockTopDamage->m_character.Level, pMonsterInfo))
				{
					pDropItemGeneral->PrefixCodeNum = 0;
					pDropItemGeneral->SuffixCodeNum = 0;
				}

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
// 				if (i_pDeadFMonster->PositionVector.x == 0.0f)
// 				{	// check: monster의 좌표가 (0,0,0)일 경우 이유를 찾아야 함, 우선 땜빵...
// 
// 					//DbgOut("Monster's Position is 0: (%d)\n", i_pDeadFMonster->MonsterIndex);
// 					g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
// 					break;
// 				}
 				D3DXVECTOR3 pos;			

				CMapBlock *pMapBlock = NULL;
				m_pFieldMapChannel1->m_pFieldMapProject->GetRandomPositionInRadius(&vec3MonPos, &pos, DROP_ITEM_RADIUS);
				pMapBlock						= m_pFieldMapChannel1->GetBlock(pos.x, pos.z);
				i_pDropItem->Position			= pos;
				i_pDropItem->dwStartTick		= m_pFieldIOCP15->GetCurrentServerTimeInMilliSeconds();
				i_pDropItem->pItem				= pDropItemGeneral;
				i_pDropItem->FirstCharacterUID1	= i_pFISockTopDamage->m_character.CharacterUniqueNumber;
				pMapBlock->InsertDropItem(pDropItemGeneral, i_pDropItem);

				INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
				pMsgShowItem->ItemFieldIndex	= (UINT)i_pDropItem->pItem;
				pMsgShowItem->ItemNum			= i_pDropItem->pItem->ItemNum;
				pMsgShowItem->FirstCharacterUID	= i_pDropItem->FirstCharacterUID1;
				pMsgShowItem->Amount			= i_pDropItem->pItem->CurrentCount;
				pMsgShowItem->Position			= i_pDropItem->Position;
				pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;

// 2007-06-18 by cmkwon, 테스트				DBGOUT("ShowItem(%d): %#x[%d][%d]\n", i_pDropItem->pItem->ItemNum, pMsgShowItem->ItemFieldIndex, pMapBlock->m_x, pMapBlock->m_z);

				if(IS_TUTORIAL_MAP_INDEX(m_pFieldMapChannel1->GetMapChannelIndex().MapIndex))
				{
					mt_auto_lock mtCT(&i_pDeadFMonster->m_mtvectClientIdxForTargetType);
					m_pFieldIOCP15->SendInRangeClientIndexVectorAroundPosition(pos, &i_pDeadFMonster->m_mtvectClientIdxForTargetType
						, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1, m_pFieldMapChannel1->GetUserVisibleDiameterW());
				}
				else
				{
					m_pFieldIOCP15->SendInRangeMessageAroundPosition(pos, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1);
				}
			}
		} // END - NOT AUTO PICKUP
	} // END - for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	// 2010-06-21 by jskim, 2010 이벤트(한명당 아이템 하나씩) - 오리진(2098200)
	if(bAutoPickUpOfEventDrop && !v_DropItemGeneral.empty())
	{
		sort( v_DropItemGeneral.begin(), v_DropItemGeneral.end(), SORT_DROP_ITEM_PRIORITY() );
		for(int i=0; i < v_DropItemGeneral.size(); i++)
		{
			if (NULL != i_pFISockTopDamage)
			{
				v_DropItemGeneral[i].pDropItemGeneral->AccountUniqueNumber	= i_pFISockTopDamage->m_character.AccountUniqueNumber;
				v_DropItemGeneral[i].pDropItemGeneral->Possess				= i_pFISockTopDamage->m_character.CharacterUniqueNumber;
				if(FALSE == i_pFISockTopDamage->m_ItemManager.InsertItemGeneralByPointer(v_DropItemGeneral[i].pDropItemGeneral, IUT_DROP_ITEM))
				{
					util::del(v_DropItemGeneral[i].pDropItemGeneral);
				}
				i_pFISockTopDamage = this->GetNextUser(i_pFISockTopDamage, FALSE);
			}
		}		
	}
	m_litMonsterDropItem.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessEXPDivisionInfinityMap(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster, CFieldMonster *pTargetMonster/*=NULL*/)
/// \brief		인피니티 - 인피 맵에선 파티 상관없이 맵 모든 유저 경험치 균등분배.
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
void CEXPDivisionManager::ProcessEXPDivisionInfinityMap(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster, CFieldMonster *pTargetMonster /*=NULL*/)
// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
{
	if(NULL == o_pDropItem
		|| NULL == o_pLogMonsterBoss) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	int nCountForBossMonsterLog		= 0;

	// 경험치 계산
	if(0 >= m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.size()) {
		return;
	}
	Experience_t fExp			= i_exp / m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.size();

	mt_auto_lock mta(&m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject);
	mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.begin();
	for (; itrChar != m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.end(); itrChar++) {
		
		CFieldIOCPSocket *pFISock = itrChar->second;

		// 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
		pFISock->ChangeExperience(fExp, TRUE, TRUE, TRUE, 1, pTargetMonster);		// 2011-09-02 by hskim, 파트너 시스템 2차
		// end 2012-10-12 by jhjang 해피 아워 경험치 보너스 시스템 리뉴얼
		
		// Drop Item Delay관련
		if (o_pDropItem->priorityInfoCount < SIZE_MAX_DROP_ITEM_PRIORITY_INFO)
		{
			o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].CharacterUniqueNumber	= pFISock->GetCharacter()->CharacterUniqueNumber;
			o_pDropItem->priorityInfoArray[o_pDropItem->priorityInfoCount].dwDelay					= 0;
			o_pDropItem->priorityInfoCount++;
		}

		if (i_bBossMonster
			&& pFISock
			&& pFISock->IsValidCharacter()
			&& nCountForBossMonsterLog < SIZE_MAX_USERLIST_ON_BOSS_MONSTER_DEAD)
		{
			util::strncpy(o_pLogMonsterBoss->AttackUser[nCountForBossMonsterLog], pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
			nCountForBossMonsterLog++;		// 2007-01-16 by cmkwon, 버그 수정
		}
	}// END - for(int i=0; i < nCnt; i++)
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CEXPDivisionManager::ProcessEXPDivisionInfinityMap(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, mtTenderItemList * o_pTenderItemList)
/// \brief		인피니티 - 인피 맵에서 아이템 처리, Tender는 InfinityBase에서 처리한다.
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::ProcessPickUpDropItemsInfinityMap(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CInfinityManager *i_pInfiMan)
{
	if(NULL == o_pStrDropItemlist
		|| NULL == i_pDropItem
		|| NULL == i_pDeadFMonster) {
		// 2009-09-09 ~ 2010-01 by dhjin, 인피니티 - 소스 체크
		return;
	}

	if(1 > m_litMonsterDropItem.size())
	{
		return;
	}

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	D3DXVECTOR3 vec3MonPos = i_pDeadFMonster->PositionVector;
	if(0.0f > vec3MonPos.x || 0.0f > vec3MonPos.z)
	{		
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
		return;
	}

	MONSTER_INFO *pMonsterInfo = i_pDeadFMonster->MonsterInfoPtr;
	if(NULL == pMonsterInfo)
	{
		return;
	}

	mtlistMonsterDropItem::iterator itrDropItem = m_litMonsterDropItem.begin();
	for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itrDropItem;
		ITEM_GENERAL *pDropItemGeneral		= tmpMonsterDropItemGeneral.pItemGeneral;
		ITEM *pDropItemInfo					= tmpMonsterDropItemGeneral.pItemGeneral->ItemInfo;
		
		if (COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER))
		{
			*o_pStrDropItemlist += pDropItemInfo->ItemName;
			*o_pStrDropItemlist += "|";
		}

		BYTE ItemUpdateType = IUT_DROP_ITEM;

		pDropItemGeneral->Wear					= WEAR_NOT_ATTACHED;
		pDropItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;	// 서버에서는 아이템 창의 첫 slot에 넣어두고, 정리는 client에서 한다.
		pDropItemGeneral->NumOfEnchants			= 0;

		// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
		//if (TRUE == m_pFieldIOCP15->CheckTenderItem(pDropItemInfo->ItemNum))
		if(pDropItemInfo->IsTenderDropItem)
		{
			// Tender Item 처리
			D3DXVECTOR3 pos;			
			
			CMapBlock *pMapBlock = NULL;
			m_pFieldMapChannel1->m_pFieldMapProject->GetRandomPositionInRadius(&vec3MonPos, &pos, DROP_ITEM_RADIUS);
			pMapBlock						= m_pFieldMapChannel1->GetBlock(pos.x, pos.z);
			i_pDropItem->Position			= pos;
			i_pDropItem->dwStartTick		= m_pFieldIOCP15->GetCurrentServerTimeInMilliSeconds();
			i_pDropItem->pItem				= pDropItemGeneral;
			i_pDropItem->FirstCharacterUID1	= 0;
			i_pDropItem->bTendering			= TRUE;				// Tender 중인 아이템으로 설정
			pMapBlock->InsertDropItem(pDropItemGeneral, i_pDropItem);

			CTenderItemInfo InsertTenderInfo;
			InsertTenderInfo.m_ItemFieldIndex		= (UINT)i_pDropItem->pItem;
			InsertTenderInfo.m_TenderItemPosition	= pos;
			InsertTenderInfo.m_TenderItemNum		= pDropItemGeneral->ItemNum;
			InsertTenderInfo.m_TenderItemCount		= pDropItemGeneral->CurrentCount;
			
			if(ERR_NO_ERROR != i_pInfiMan->PushTenderItemW(m_pFieldMapChannel1->GetMapChannelIndex().MapIndex, m_pFieldMapChannel1->GetMapChannelIndex().ChannelIndex, &InsertTenderInfo))
			{
				g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CEXPDivisionManager::ProcessPickUpDropItemsInfinityMap# 10000 ! Map(%s) Tender(ItemNum:%s ItemCnt:%d) \r\n"
					, GetMapString(m_pFieldMapChannel1->GetMapChannelIndex(), string()), InsertTenderInfo.m_TenderItemNum, InsertTenderInfo.m_TenderItemCount);
			}

			// Tender Item 정보 전송
			INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_DROPITEM_INFO, T_FC_INFINITY_TENDER_DROPITEM_INFO, pMsgTenderItem, pMsgTenderItemBuf);
			pMsgTenderItem->ItemFieldIndex		= (UINT)i_pDropItem->pItem;
			pMsgTenderItem->TenderDropItemNum	= i_pDropItem->pItem->ItemNum;
			pMsgTenderItem->Count				= i_pDropItem->pItem->CurrentCount;
			pMsgTenderItem->PrefixCodeNum		= pDropItemGeneral->PrefixCodeNum;
			pMsgTenderItem->SuffixCodeNum		= pDropItemGeneral->SuffixCodeNum;
			pMsgTenderItem->TenderStartTime = InsertTenderInfo.m_TenderStartTime;
			m_pFieldMapChannel1->SendMessageToAllInChannel(pMsgTenderItemBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_DROPITEM_INFO));


			INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
			pMsgShowItem->ItemFieldIndex	= (UINT)i_pDropItem->pItem;
			pMsgShowItem->ItemNum			= i_pDropItem->pItem->ItemNum;
			pMsgShowItem->FirstCharacterUID	= 0;
			pMsgShowItem->Amount			= i_pDropItem->pItem->CurrentCount;
			pMsgShowItem->Position			= i_pDropItem->Position;
			pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;
			m_pFieldIOCP15->SendInRangeMessageAroundPosition(pos, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1);
			continue;
		}
		else if (MONEY_ITEM_NUMBER == pDropItemGeneral->ItemNum)
		{
			// 돈 처리 - 공평하게 1/n
			pDropItemGeneral->CurrentCount = (pDropItemGeneral->CurrentCount) / max(1, (m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.size()));

			mt_auto_lock mta(&m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject);
			mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.begin();
			for (; itrChar != m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.end(); itrChar++) {
				CFieldIOCPSocket *pFISock = itrChar->second;
				if(NULL != pFISock) {
					pDropItemGeneral->AccountUniqueNumber	= pFISock->m_character.AccountUniqueNumber;
					pDropItemGeneral->Possess				= pFISock->m_character.CharacterUniqueNumber;
					pFISock->m_ItemManager.InsertItemBaseByItemNum(MONEY_ITEM_NUMBER, pDropItemGeneral->CurrentCount, ItemUpdateType);
				}
			}// END - for(int i=0; i < nCnt; i++)
			util::del(pDropItemGeneral);
			continue;
		}
		else
		{
			// 공평하게 맵 모든 유저에게 지급
			mt_auto_lock mta(&m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject);
			mtmapUID2FieldIOCPSocket::iterator itrChar = m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.begin();
			for (; itrChar != m_pFieldMapChannel1->m_mapCharacterUniqueNumberMapProject.end(); itrChar++) {
				CFieldIOCPSocket *pFISock = itrChar->second;
				if(NULL != pFISock) {
					if (IS_COUNTABLE_ITEM(pDropItemGeneral->Kind)) {
						// 2009-09-09 ~ 2010-02-23 by dhjin, 인피니티 - 인피니티 맵 일반 드랍 아이템 수량 버그 수정
						pFISock->m_ItemManager.InsertItemBaseByItemNum(pDropItemGeneral->ItemNum, pDropItemGeneral->CurrentCount, ItemUpdateType);
					}
					else {
						pFISock->m_ItemManager.InsertItemBaseByItemNum(pDropItemGeneral->ItemNum, 1, ItemUpdateType);
					}
				}
			}// END - for(int i=0; i < nCnt; i++)
			util::del(pDropItemGeneral);
			continue;
		}
	}
	m_litMonsterDropItem.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-14 by cmkwon, 2009 크리스마스이벤트(한명당 아이템 하나씩) - 
/// \author		cmkwon
/// \date		2009-12-14 ~ 2009-12-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldIOCPSocket * CEXPDivisionManager::GetNextUser(CFieldIOCPSocket *i_pCurFISoc, BOOL i_bLiveCheck/*=TRUE*/)
{
	if(NULL == i_pCurFISoc
		|| m_vectSEXP_DIVISIONList.empty())
	{
		return NULL;
	}

	BOOL bFindedCurUser = FALSE;
	vectSEXP_DIVISION::iterator itr(m_vectSEXP_DIVISIONList.begin());
	for(; itr != m_vectSEXP_DIVISIONList.end(); itr++)
	{
		SEXP_DIVISION *pExpDiv = &*itr;
		if(bFindedCurUser
			&& pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0
			&& pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0->IsValidCharacter(i_bLiveCheck))
		{
			return pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0;
		}

		if(NULL == pExpDiv->pFParty0
			|| 1 >= pExpDiv->nPartyMemberCnts)
		{
			if(i_pCurFISoc == pExpDiv->ArrSEXP_PARTYMEMBER[0].pFISock0)
			{
				bFindedCurUser = TRUE;
			}
		}
		else
		{
			for (int j = 0; j < pExpDiv->nPartyMemberCnts; j++)
			{
				CFieldIOCPSocket *p2FISock = pExpDiv->ArrSEXP_PARTYMEMBER[j].pFISock0;
				
				if(NULL == p2FISock
					|| FALSE == p2FISock->IsValidCharacter(i_bLiveCheck))
				{// 2006-07-18 by cmkwon, 유효 파티원 체크
					continue;
				}
				
				if(bFindedCurUser)
				{
					return p2FISock;
				}

				if(i_pCurFISoc == p2FISock)
				{
					bFindedCurUser = TRUE;
				}
			}
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2010-04-12 ~ 2010-04-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CEXPDivisionManager::ProcessPickUpTenderItemsInInfinityMap(string *o_pStrDropItemlist, CFieldMonster *i_pDeadFMonster, CInfinityManager *i_pInfiMan)
{
	if(NULL == m_pFieldMapChannel1 
		|| m_litMonsterDropItem.empty())
	{
		return;
	}

	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
	D3DXVECTOR3 vec3MonPos = i_pDeadFMonster->PositionVector;
	if(0.0f > vec3MonPos.x || 0.0f > vec3MonPos.z)
	{		
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] DropItem error !! MonsterIdx(%d) Pos(%f, %f, %f) \r\n", i_pDeadFMonster->MonsterIndex, i_pDeadFMonster->PositionVector.x, i_pDeadFMonster->PositionVector.y, i_pDeadFMonster->PositionVector.z);
		return;
	}
	MONSTER_INFO *pMonsterInfo = i_pDeadFMonster->MonsterInfoPtr;
	if(NULL == pMonsterInfo)
	{
		return;
	}

	mtlistMonsterDropItem::iterator itrDropItem = m_litMonsterDropItem.begin();
	for(; itrDropItem != m_litMonsterDropItem.end(); itrDropItem++)
	{
		MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItemGeneral = *itrDropItem;
		ITEM_GENERAL *pDropItemGeneral		= tmpMonsterDropItemGeneral.pItemGeneral;
		ITEM *pDropItemInfo					= tmpMonsterDropItemGeneral.pItemGeneral->ItemInfo;
		
		if (COMPARE_MPOPTION_BIT(pMonsterInfo->MPOption, MPOPTION_BIT_BOSS_MONSTER|MPOPTION_BIT_KEY_MONSTER|MPOPTION_BIT_KEY_MONSTER_ALIVE_FOR_GAMECLEAR))
		{
			*o_pStrDropItemlist += pDropItemInfo->ItemName;
			*o_pStrDropItemlist += "|";
		}

		pDropItemGeneral->Wear					= WEAR_NOT_ATTACHED;
		pDropItemGeneral->ItemWindowIndex		= POS_INVALID_POSITION;	// 서버에서는 아이템 창의 첫 slot에 넣어두고, 정리는 client에서 한다.
		pDropItemGeneral->NumOfEnchants			= 0;

		
		DROPITEM			dropItem;
		CMapBlock *pMapBlock = NULL;
		D3DXVECTOR3 pos;			
		m_pFieldMapChannel1->m_pFieldMapProject->GetRandomPositionInRadius(&vec3MonPos, &pos, DROP_ITEM_RADIUS);
		pMapBlock					= m_pFieldMapChannel1->GetBlock(pos.x, pos.z);
		dropItem.Position			= pos;
		dropItem.dwStartTick		= m_pFieldIOCP15->GetCurrentServerTimeInMilliSeconds();
		dropItem.pItem				= pDropItemGeneral;
		dropItem.bProcessing		= FALSE;
		dropItem.FirstCharacterUID1	= 0;
		dropItem.bTendering			= TRUE;				// Tender 중인 아이템으로 설정
		pMapBlock->InsertDropItem(pDropItemGeneral, &dropItem);

		CTenderItemInfo InsertTenderInfo;
		InsertTenderInfo.m_ItemFieldIndex		= (UINT)(dropItem.pItem);
		InsertTenderInfo.m_TenderItemPosition	= pos;
		InsertTenderInfo.m_TenderItemNum		= pDropItemGeneral->ItemNum;
		InsertTenderInfo.m_TenderItemCount		= pDropItemGeneral->CurrentCount;
		if(ERR_NO_ERROR != i_pInfiMan->PushTenderItemW(m_pFieldMapChannel1->GetMapChannelIndex().MapIndex, m_pFieldMapChannel1->GetMapChannelIndex().ChannelIndex, &InsertTenderInfo))
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CEXPDivisionManager::ProcessPickUpDropItemsInfinityMap# 10000 ! Map(%s) Tender(ItemNum:%s ItemCnt:%d) \r\n"
				, GetMapString(m_pFieldMapChannel1->GetMapChannelIndex(), string()), InsertTenderInfo.m_TenderItemNum, InsertTenderInfo.m_TenderItemCount);
		}

		// Tender Item 정보 전송
		INIT_MSG_WITH_BUFFER(MSG_FC_INFINITY_TENDER_DROPITEM_INFO, T_FC_INFINITY_TENDER_DROPITEM_INFO, pMsgTenderItem, pMsgTenderItemBuf);
		pMsgTenderItem->ItemFieldIndex		= (UINT)(dropItem.pItem);
		pMsgTenderItem->TenderDropItemNum	= dropItem.pItem->ItemNum;
		pMsgTenderItem->Count				= dropItem.pItem->CurrentCount;
		pMsgTenderItem->PrefixCodeNum		= pDropItemGeneral->PrefixCodeNum;
		pMsgTenderItem->SuffixCodeNum		= pDropItemGeneral->SuffixCodeNum;
		pMsgTenderItem->TenderStartTime = InsertTenderInfo.m_TenderStartTime;
		m_pFieldMapChannel1->SendMessageToAllInChannel(pMsgTenderItemBuf, MSG_SIZE(MSG_FC_INFINITY_TENDER_DROPITEM_INFO));

		INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
		pMsgShowItem->ItemFieldIndex	= (UINT)(dropItem.pItem);
		pMsgShowItem->ItemNum			= dropItem.pItem->ItemNum;
		pMsgShowItem->FirstCharacterUID	= 0;
		pMsgShowItem->Amount			= dropItem.pItem->CurrentCount;
		pMsgShowItem->Position			= dropItem.Position;
		pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;
		m_pFieldIOCP15->SendInRangeMessageAroundPosition(pos, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), m_pFieldMapChannel1);
	}
	m_litMonsterDropItem.clear();
}