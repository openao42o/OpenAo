// FieldMonster.cpp: implementation of the CFieldMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldMonster.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "TriggerFunctionCrystal.h"		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldMonster::CFieldMonster()
{
	m_usSendCountsMonsterMoveOK			= 0;
	m_pCurrentFieldMapChannelMonster	= NULL;

	m_mtlistDropItem.lock();
	m_mtlistDropItem.clear();
	m_mtlistDropItem.unlock();

	m_mtVectCltIdxForMonsterMoveOK.reserve(100);
	m_mtVectCltIdxForExcludeMonsterMoveOK.reserve(20);

	m_nAttackerCliIdx						= 0;				// 2006-11-07 by cmkwon
	m_dwLastTickSetAttackerCliIdx			= 0;				// 2006-11-07 by cmkwon

	util::zero(&m_bBarrierInfo, sizeof(BARRIER_SKILL_INFO));	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 배리어
	util::zero(&m_ImmediateBarrier, sizeof(IMMEDIATE_BARRIER_SKILL_INFO));	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동)

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	m_mtVectTriggerFunctionPtr.clear();
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
}

CFieldMonster::~CFieldMonster()
{
	m_mtlistDropItem.lock();
	if(m_mtlistDropItem.empty() == false)
	{
		mtlistMonsterDropItem::iterator itr = m_mtlistDropItem.begin();
		while(itr != m_mtlistDropItem.end())
		{
			delete(itr->pItemGeneral);
			itr++;
		}
		m_mtlistDropItem.clear();
	}
	m_mtlistDropItem.unlock();
}	// Heap override (20 bytes)


//////////////////////////////////////////////////////////////////////
// Property
//////////////////////////////////////////////////////////////////////
USHORT CFieldMonster::GetSendCountsMonsterMoveOK(void)
{
	return m_usSendCountsMonsterMoveOK;
}

void CFieldMonster::SetCityWarFieldMonster(CCityWar *i_pWar)
{
	m_pCityWar2 = i_pWar;
}


//////////////////////////////////////////////////////////////////////
// Method /
//////////////////////////////////////////////////////////////////////
void CFieldMonster::LockDropItemList(void)
{
	m_mtlistDropItem.lock();
}
void CFieldMonster::UnlockDropItemList(void)
{
	m_mtlistDropItem.unlock();
}

// 2007-12-07 by cmkwon, 사용하지 않는 함수 임
//void CFieldMonster::CreateDropItem(CFieldIOCP *pFieldIOCP, CFieldIOCPSocket *i_pFISockGive2MonsterTopDamage)
//{
//	// 퀘스트용 아이템은 미리 생성해두고, 몬스터 폭파시, 아이템을 먹을 
//	// 수 있는 퀘스트를 가지고 있으면 주고, 아님 무시하는 식으로
//	// 처리하기로 결정, 20040220, kelovon
//
//	// 퀘스트용 드랍 아이템 때문에 퀘스트 리로딩할 때는 무효
//	if (!pFieldIOCP->m_ServicePauseManager.GetQuestServiceState())
//	{
//		return;
//	}
//
//	multimap<int, MONSTER2ITEM*>*			pMapMonster2Item	= &pFieldIOCP->m_mapMonster2Item;
//	multimap<int, MONSTER2ITEM*>::iterator	itr					= pMapMonster2Item->find(MonsterInfoPtr->MonsterUnitKind);
//	if (itr == pMapMonster2Item->end())
//	{
//		return;
//	}
//	
//// 2007-10-30 by cmkwon, 세력별 해피아워 이벤트 구현 - 아래와 같이 함수가 수정됨
////	float fPlusSPI			= pFieldIOCP->m_gameEventManager.GetSPIRate(INFLUENCE_TYPE_NORMAL, FALSE);
////	float fPlusDropItem		= pFieldIOCP->m_gameEventManager.GetDropItemRate(INFLUENCE_TYPE_NORMAL, FALSE) - 1.0f;
////	float fPlusDropRareItem	= pFieldIOCP->m_gameEventManager.GetDropRareItemRate(INFLUENCE_TYPE_NORMAL, FALSE) - 1.0f;
//	float fSPIRate			= 1.0f + pFieldIOCP->m_gameEventManager.GetSPIPlusRate(INFLUENCE_TYPE_NORMAL, FALSE);
//	float fPlusDropItem		= pFieldIOCP->m_gameEventManager.GetDropItemPlusRate(INFLUENCE_TYPE_NORMAL, FALSE);
//	float fPlusDropRareItem	= pFieldIOCP->m_gameEventManager.GetDropRareItemPlusRate(INFLUENCE_TYPE_NORMAL, FALSE);
//	if(i_pFISockGive2MonsterTopDamage
//		&& i_pFISockGive2MonsterTopDamage->IsValidCharacter())
//	{
//		fSPIRate			= i_pFISockGive2MonsterTopDamage->GetPlusRateSPI();
//		fPlusDropItem		= i_pFISockGive2MonsterTopDamage->GetPlusRateDropItem() - 1.0f;
//		fPlusDropRareItem	= i_pFISockGive2MonsterTopDamage->GetPlusRateDropRareItem() - 1.0f;
//	}
//
//	do
//	{
//		MONSTER2ITEM	*pMonster2Item	= itr->second;
//		ITEM			*pItem			= pFieldIOCP->GetItemInfo(pMonster2Item->ItemNumber);
//
//		if (pItem == NULL)
//		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error1, MonsterUnitKind(%8d) ItemNumber(%8d)\r\n"
//				, MonsterInfoPtr->MonsterUnitKind, pMonster2Item->ItemNumber);
//			itr++;
//			continue;
//		}
//
//		int pValue = RAND1000K();
//		if (pMonster2Item->Probability < pValue - (Prob1000K_t)(pMonster2Item->Probability*fPlusDropItem))
//		{// 생성 실패
//			
//			itr++;
//			continue;
//		}
//
//		if(pMonster2Item->MinCount < 0
//			|| pMonster2Item->MaxCount < 0
//			|| pMonster2Item->MaxCount < pMonster2Item->MinCount)
//		{
//			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] Monster DropItem Error2, MonsterUnitKind(%8d) ItemNumber(%8d) MinCount(%4d) MaxCount(%8d)\r\n"
//				, MonsterInfoPtr->MonsterUnitKind, pMonster2Item->ItemNumber, pMonster2Item->MinCount, pMonster2Item->MaxCount);
//			itr++;
//			continue;
//		}
//
//		INT nAmount = RANDI(pMonster2Item->MinCount, pMonster2Item->MaxCount);
//		if(0 >= nAmount)
//		{
//			itr++;
//			continue;
//		}
//
//		if(g_pFieldGlobal->InMasangServer())
//		{// 2006-01-02 by cmkwon, 테스트 코드
//			char szTemp[1024];
//			sprintf(szTemp, "	Mon(%s:%d) DromItem ==> Item(%s:%d), Counts(%d)\r\n"
//				, MonsterInfoPtr->MonsterName, MonsterIndex, pItem->ItemName, pItem->ItemNum, nAmount);
//			DBGOUT(szTemp);
//			g_pFieldGlobal->WriteSystemLog(szTemp);
//		}
//		
//		if (IS_COUNTABLE_ITEM(pItem->Kind))
//		{// Countable Item인 경우
//
//			ITEM_GENERAL *pNewItem = new ITEM_GENERAL(pItem);
//			if(MONEY_ITEM_NUMBER == pItem->ItemNum)
//			{
//				nAmount *= fSPIRate;
//			}
//
//			pNewItem->CurrentCount			= nAmount;
//			MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItem;
//			tmpMonsterDropItem.pItemGeneral	= pNewItem;
//			tmpMonsterDropItem.QuestIndex	= pMonster2Item->QuestIndex;
//			m_mtlistDropItem.push_back(tmpMonsterDropItem);
//		}
//		else
//		{// 나머지 일반 아이템인 경우(Countable Item이 아닌경우)
//			
//			for (int i = 0; i < nAmount; i++)
//			{
//				ITEM_GENERAL *pStoreItem = new ITEM_GENERAL(pItem);
//				CFieldIOCPSocket::ApplyRareItem(this, pMonster2Item, fPlusDropRareItem
//					, CFieldIOCPSocket::ms_pFieldIOCP, pStoreItem);
//
//				//DBGOUT("%d(%#x) ", pStoreItem->ItemNum, pStoreItem);
//				MONSTER_DROP_ITEM_GENERAL tmpMonsterDropItem;
//				tmpMonsterDropItem.pItemGeneral = pStoreItem;
//				tmpMonsterDropItem.QuestIndex = pMonster2Item->QuestIndex;
//
//				m_mtlistDropItem.push_back(tmpMonsterDropItem);
//			}
//		}// end
//
//		itr++;
//	} while(itr != pMapMonster2Item->upper_bound(MonsterInfoPtr->MonsterUnitKind));	
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMonster::SetAttackerCliIdx(ClientIndex_t i_cliIdx)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-07 ~ 2006-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::SetAttackerCliIdx(ClientIndex_t i_cliIdx)
{
	if(BELL_NOTATTACK == this->MonsterInfoPtr->Belligerence
		|| IS_STRATEGYPOINT_MONSTER(this->MonsterInfoPtr->Belligerence))	// 2006-12-13 by cmkwon, 전략포인트 몬스터도 절대 공격하지 않는다
	{// 2006-11-24 by cmkwon, 비공격 몬스터
		return FALSE;
	}

	if(0 != m_nTargetIndex)
	{
		return FALSE;
	}

	DWORD dwCur = timeGetTime();
	if(0 == m_nAttackerCliIdx
		|| dwCur - m_dwLastTickSetAttackerCliIdx > 10000)
	{
		m_nAttackerCliIdx				= i_cliIdx;
		m_dwLastTickSetAttackerCliIdx	= dwCur;
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ClientIndex_t CFieldMonster::GetAttackerCliIdx(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-11-07 ~ 2006-11-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ClientIndex_t CFieldMonster::GetAttackerCliIdx(void)
{
	if(0 != m_nTargetIndex)
	{
		return m_nTargetIndex;
	}

	DWORD dwCur = timeGetTime();
	if(0 != m_nAttackerCliIdx
		&& dwCur - m_dwLastTickSetAttackerCliIdx < 10000)
	{
		return m_nAttackerCliIdx;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CMonster::GetCurrentPosition
// 반환되는 형  : D3DXVECTOR3
// 함 수 인 자  : void
// 함 수 설 명  : inline 함수
//					몬스터의 실제 PositionVector를 리턴하는 함수로 속도가 빠른
//					몬스터의 PositionVector 오차를 줄이기 위한 함수로 Attck시에
//					맞았는지의 유무에 사용됨
D3DXVECTOR3 CFieldMonster::GetCurrentPosition(void)
{
	if (m_BeforePosition == PositionVector)
	{
		return m_BeforePosition;
	}

	DWORD		dwCurrentGap = timeGetTime() - m_dwTimeLastMoved;
	D3DXVECTOR3 TempVector3;

	D3DXVec3Normalize(&TempVector3, &(PositionVector - m_BeforePosition));
	return m_BeforePosition + TempVector3 * dwCurrentGap/MONSTER_UPDATE_MOVE_TERM_TICK;
}


ITEM * CFieldMonster::FindItemPtrWithItemNum(INT i_uiItemNum)
{
	if(NULL == MonsterInfoPtr){				return NULL;}
	

	for(int i = 0; i < ARRAY_SIZE_MONSTER_ITEM; i++)
	{
		if(this->MonsterInfoPtr->ItemInfo[i].pItemInfo
			&& MonsterInfoPtr->ItemInfo[i].pItemInfo->ItemNum == i_uiItemNum)
		{
			return MonsterInfoPtr->ItemInfo[i].pItemInfo;
		}
	}
	return NULL;
}

ITEM * CFieldMonster::FindItemPtrByOrBitTyp(BYTE i_byOrBitType)
{
	if(NULL == MonsterInfoPtr){				return NULL;}
	

	for(int i = 0; i < ARRAY_SIZE_MONSTER_ITEM; i++)
	{
		if(this->MonsterInfoPtr->ItemInfo[i].pItemInfo
			&& MonsterInfoPtr->ItemInfo[i].pItemInfo->OrbitType == i_byOrBitType)
		{
			return MonsterInfoPtr->ItemInfo[i].pItemInfo;
		}
	}
	return NULL;
}

USHORT CFieldMonster::IncreaseSendCountsMonsterMoveOK(void)
{
	m_usSendCountsMonsterMoveOK++;
	return m_usSendCountsMonsterMoveOK;
}

float CFieldMonster::IncreaseMonsterCurrentHP(float i_fValue)
{
	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	/*if(CurrentHP <= 0.0f
		|| FALSE == this->IsValidMonster())
	{
		return CurrentHP;
	}*/
	if( this->CurrentHP <= 0.0f
		|| FALSE == this->IsValidMonster())
	{
		return this->CurrentHP;
	}
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )


	float	BeforeHP = 0.0f;
	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	/*BeforeHP	= this->CurrentHP;
	CurrentHP	= min(MonsterInfoPtr->MonsterHP, CurrentHP + i_fValue);
	
	float AfterHP = 0.0f;
	AfterHP		= this->CurrentHP;*/

	BeforeHP		= this->CurrentHP;
	this->CurrentHP	= min(this->MonsterInfoExtend.fMaxHP , this->CurrentHP + i_fValue );
	
	float AfterHP = 0.0f;
	AfterHP			= this->CurrentHP;
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	mtAuto.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
		&& this->MSMonsterCheckWithoutMSBossMonster())
	{
		// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
		//this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
		this->HPRateCalcByMSWarInfoDisPlay( this->MonsterInfoExtend.fMaxHP , BeforeHP , AfterHP );
		// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	}
	else if (IS_STRATEGYPOINT_MONSTER(MonsterInfoPtr->Belligerence))
	{
		INT AfterHPRate = CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, AfterHP, MS_WAR_HP_RATE);
		if (AfterHPRate != CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, BeforeHP, MS_WAR_HP_RATE))
		{
			m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.UpdateSummonStrategicPointInfoHP(m_pCurrentFieldMapChannelMonster->GetMapChannelIndex().MapIndex, AfterHPRate);
		}
	}
	
	return AfterHP;
}

// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - DecreaseMonsterCurrentHP 로 통합 - 주석처리
// float CFieldMonster::DecreaseMonsterCurrentHPByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
// {
// 	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
// 	if(this->CurrentHP <= 0.0f
// 		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
// 		|| MS_PLAYING != this->m_enMonsterState)
// 	{
// 		return this->CurrentHP;
// 	}
// 
// 	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - 주석 처리 ProcessingDamagedMonsterByCharacter 옮김
// // 	//////////////////////////////////////////////////////////////////////////
// // 	// 2008-12-23 by dhjin, 전쟁 보상 추가안
// // 	if(IS_INFLWAR_MONSTER(this->MonsterInfoPtr->Belligerence)
// // 		&& (i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckSummonBossAndStrategyPoint()
// // 			|| i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckALLOutPostWaring())
// // 		)
// // 	{
// // 		UID32_t WarContribution;
// // 		if(this->CurrentHP <= i_fValue)
// // 		{// 2008-12-23 by dhjin, 폭파라면 현재 피로 넣는다.
// // 			WarContribution = (UID32_t)this->CurrentHP;
// // 		}
// // 		else
// // 		{
// // 			WarContribution = (UID32_t)i_fValue;
// // 		}
// // 		i_AttackFISock->ms_pFieldIOCP->WarContributionCalculation(i_AttackFISock, WarContribution);
// // 	}
// 		
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
// 	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
// 		&& this->MSMonsterCheckWithoutMSBossMonster())
// 	{
// 		float	BeforeHP, AfterHP;
// 		BeforeHP = this->CurrentHP;
// 
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 
// 		AfterHP = this->CurrentHP;
// 		this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
// 	}
// 	else
// 	{
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 	}
// 
// 	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - 주석 처리 ProcessingDamagedMonsterByCharacter 옮김
// // 	///////////////////////////////////////////////////////////////////////////////
// // 	// 공격 당한 정보를 추가한다
// // 	this->InserttoAttackedInfoList(&ActionInfo(i_AttackFISock->GetCharacter()->ClientIndex, timeGetTime(), i_fValue));
// // 
// // 	///////////////////////////////////////////////////////////////////////////////
// // 	// 2006-07-21 by cmkwon, Aggro 추가하기
// // 	for(int i=0; i < i_pAInfoAggroList->size(); i++)
// // 	{
// // 		this->InserttoAttackedInfoList( &((*i_pAInfoAggroList)[i]) );
// // 	}
// 	
// 	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - 사용 안하는 시스템 주석 처리
// // 	CCityWar *ptmWar = m_pCityWar2;
// // 	if(ptmWar
// // 		&& CITYWAR_STATE_STARTED == ptmWar->GetCityWarState()
// // 		&& this->MonsterInfoPtr->MonsterUnitKind == ptmWar->CityWarQuestMonsterUID
// // 		&& IS_VALID_UNIQUE_NUMBER(i_AttackFISock->GetCharacter()->GuildUniqueNumber))
// // 	{
// // 		ptmWar->AddSumOfDamageCityWar(i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_fValue);
// // 	}
// 
// 	// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - 사용 안하는 시스템 주석 처리
// // 	// 2007-08-24 by dhjin, 전진기지 보호막이면 여단과 데미지를 저장한다.
// // 	if((BELL_ATTACK_OUTPOST_PROTECTOR == this->MonsterInfoPtr->Belligerence
// // 		|| BELL_OUTPOST_PROTECTOR_VCN == this->MonsterInfoPtr->Belligerence
// // 		|| BELL_OUTPOST_PROTECTOR_ANI == this->MonsterInfoPtr->Belligerence)
// // 		&& IS_VALID_UNIQUE_NUMBER(i_AttackFISock->m_character.GuildUniqueNumber)		// 2008-04-02 by dhjin, 전진기지 프로텍터 데미지 계산에 지도자 후보 여단 제외 - 유효한 여단인지 체크 루틴 추가
// // 		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckLeader_SubLeaderGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-09-07 by dhjin, 지도자 여단, 여단장이 부지도자인 여단은 제외
// // 		&& (!i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckOutPostPossessByGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-11-29 by dhjin, 전진기지 소유 여단은 다른 전진기지를 소유할 수 없으므로 제외한다.	
// // 		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckRegLeaderCandidateGuild(i_AttackFISock->m_character.GuildUniqueNumber))	// 2008-04-02 by dhjin, 전진기지 프로텍터 데미지 계산에 지도자 후보 여단 제외 - 지도자 후보 등록한 여단인지 체크
// // 		)
// // 	{
// // 		i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.AddDamageOfProtectorW(i_AttackFISock->GetCharacter()->MapChannelIndex.MapIndex, i_AttackFISock->GetCharacter()->InfluenceType
// // 																				, i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_AttackFISock->m_character.GuildName, i_fValue);
// // 	}
// 
// 	return this->CurrentHP;
// }

// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - DecreaseMonsterCurrentHP 로 통합 - 주석처리
///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldMonster::DecreaseMonsterCurrentHPByMonster(float i_fValue)
/// \brief		인피니티 - 몬스터에게 공격 당함 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// float CFieldMonster::DecreaseMonsterCurrentHPByMonster(float i_fValue) {
// 	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
// 	if(this->CurrentHP <= 0.0f
// 		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
// 		|| MS_PLAYING != this->m_enMonsterState) {
// 		return this->CurrentHP;
// 	}
// 
// 	//////////////////////////////////////////////////////////////////////////
// 	// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
// 	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
// 		&& this->MSMonsterCheckWithoutMSBossMonster()) {
// 		float	BeforeHP, AfterHP;
// 		BeforeHP = this->CurrentHP;
// 		
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 		
// 		AfterHP = this->CurrentHP;
// 		this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
// 	}
// 	else {
// 		i_fValue = min(i_fValue, this->CurrentHP);
// 		this->CurrentHP -= i_fValue;
// 	}
// 	
// 	return this->CurrentHP;
// }

// 2010-03-18 by dhjin, 몬스터 피 감소 처리 루틴 정리 - DecreaseMonsterCurrentHP 로 통합 - 주석처리
///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldMonster::DecreaseMonsterCurrentHPByPenalty(float i_fValue)
/// \brief		인피니티(기지방어) - 패널티 값으로 인하여 현재 HP 감소
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// float CFieldMonster::DecreaseMonsterCurrentHPByPenalty(float i_fValue)
// {
// 	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
// 	if(this->CurrentHP <= 0.0f
// 		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
// 		|| MS_PLAYING != this->m_enMonsterState) {
// 		return this->CurrentHP;
// 	}
// 
// 	i_fValue = min(i_fValue, this->CurrentHP);
// 	this->CurrentHP -= i_fValue;
// 	this->CurrentHP = max(this->CurrentHP, PENALTY_REMAIN_HP); // 패널티로 인한 HP감소는 최소 1로 설정해준다.
// 
// 	return this->CurrentHP;
// }

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CFieldMonster::DecreaseMonsterCurrentHP(float i_fValue, BOOL i_bPenalty/*=FALSE*/)
/// \brief		몬스터 피 감소 처리 루틴 정리 - DecreaseMonsterCurrentHP 로 통합
/// \author		dhjin
/// \date		2010-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CFieldMonster::DecreaseMonsterCurrentHP(float i_fValue, BOOL i_bPenalty/*=FALSE*/)
{
	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
	if(this->CurrentHP <= 0.0f
		|| FALSE == this->IsValidMonster()) {
		return this->CurrentHP;
	}
	
	if(i_bPenalty)
	{
		// 패널티 처리
		i_fValue = min(i_fValue, this->CurrentHP);
		this->CurrentHP -= i_fValue;
		this->CurrentHP = max(this->CurrentHP, PENALTY_REMAIN_HP); // 패널티로 인한 HP감소는 최소 PENALTY_REMAIN_HP로 설정해준다.
		
		return this->CurrentHP;
	}

	float	BeforeHP;

	BeforeHP = this->CurrentHP;	
	i_fValue = min(i_fValue, this->CurrentHP);
	this->CurrentHP -= i_fValue;
	
	float AfterHP = this->CurrentHP;
	mtAuto.auto_unlock_cancel();

	//////////////////////////////////////////////////////////////////////////
	// 2008-03-27 by dhjin, 모선전 정보 표시 기획안 - 
	if(m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.CheckSummonBoss()
		&& this->MSMonsterCheckWithoutMSBossMonster())
	{
		// 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
		//this->HPRateCalcByMSWarInfoDisPlay(MonsterInfoPtr->MonsterHP, BeforeHP, AfterHP);
		this->HPRateCalcByMSWarInfoDisPlay ( this->MonsterInfoExtend.fMaxHP , BeforeHP , AfterHP );
		// End 2010. 05. 31 by hsLee 인피니티 필드 2차 난이도 조절. (오브젝트 몬스터 밸런스 적용 문제 수정.)
	}
	else if (IS_STRATEGYPOINT_MONSTER(MonsterInfoPtr->Belligerence))
	{
		INT AfterHPRate = CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, AfterHP, MS_WAR_HP_RATE);
		if (AfterHPRate != CalcGradeByRate(this->MonsterInfoExtend.fMaxHP, BeforeHP, MS_WAR_HP_RATE))
		{
			m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.UpdateSummonStrategicPointInfoHP(m_pCurrentFieldMapChannelMonster->GetMapChannelIndex().MapIndex, AfterHPRate);
		}
	}

	return AfterHP;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::ProcessingDamagedMonsterByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
/// \brief		인피니티(기지방어) - 몬스터 피 감소 처리 루틴 정리 -
/// \author		dhjin
/// \date		2010-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::ProcessingDamagedMonsterByCharacter(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
{
	if(NULL == i_AttackFISock
		|| this->CurrentHP <= 0.0f
		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
		|| MS_PLAYING != this->m_enMonsterState)
	{
		return;
	}
	
	this->ProcessingInserttoAttackedInfoList(i_fValue, i_AttackFISock, i_pAInfoAggroList);
	
	//////////////////////////////////////////////////////////////////////////
	// 2008-12-23 by dhjin, 전쟁 보상 추가안
	if(IS_INFLWAR_MONSTER(this->MonsterInfoPtr->Belligerence)
		&& (	i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckSummonBossAndStrategyPoint()
				&& !IS_ONEY_ATTACK_MONSTER(this->MonsterInfoPtr->Belligerence)	// 2010-07-06 by jskim, 기여도 없는 세력 몬스터 추가
				|| i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckALLOutPostWaring()	)
		)
	{
		UID32_t WarContribution;
		if(this->CurrentHP <= i_fValue)
		{// 2008-12-23 by dhjin, 폭파라면 현재 피로 넣는다.
			WarContribution = (UID32_t)this->CurrentHP;
		}
		else
		{
			WarContribution = (UID32_t)i_fValue;
		}

		i_AttackFISock->ms_pFieldIOCP->WarContributionCalculation(i_AttackFISock, WarContribution);
	}
	
	// 2007-08-24 by dhjin, 전진기지 보호막이면 여단과 데미지를 저장한다.
	if((BELL_ATTACK_OUTPOST_PROTECTOR == this->MonsterInfoPtr->Belligerence
		|| BELL_OUTPOST_PROTECTOR_VCN == this->MonsterInfoPtr->Belligerence
		|| BELL_OUTPOST_PROTECTOR_ANI == this->MonsterInfoPtr->Belligerence)
		&& IS_VALID_UNIQUE_NUMBER(i_AttackFISock->m_character.GuildUniqueNumber)		// 2008-04-02 by dhjin, 전진기지 프로텍터 데미지 계산에 지도자 후보 여단 제외 - 유효한 여단인지 체크 루틴 추가
		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckLeader_SubLeaderGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-09-07 by dhjin, 지도자 여단, 여단장이 부지도자인 여단은 제외
		&& (!i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.CheckOutPostPossessByGuildUID(i_AttackFISock->m_character.GuildUniqueNumber)) // 2007-11-29 by dhjin, 전진기지 소유 여단은 다른 전진기지를 소유할 수 없으므로 제외한다.	
		&& (!i_AttackFISock->ms_pFieldIOCP->m_InflWarManager.CheckRegLeaderCandidateGuild(i_AttackFISock->m_character.GuildUniqueNumber))	// 2008-04-02 by dhjin, 전진기지 프로텍터 데미지 계산에 지도자 후보 여단 제외 - 지도자 후보 등록한 여단인지 체크
		)
	{
		i_AttackFISock->ms_pFieldIOCP->m_OutPostManager.AddDamageOfProtectorW(i_AttackFISock->GetCharacter()->MapChannelIndex.MapIndex, i_AttackFISock->GetCharacter()->InfluenceType
			, i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_AttackFISock->m_character.GuildName, i_fValue);
	}

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	if( TRUE == IsTriggerFunction() )
	{
		// 이 몬스터는 트리거 시스템과 연결되어 있다.
		mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
		
		for(int i=0; i<m_mtVectTriggerFunctionPtr.size(); i++)
		{
			CTriggerFunction *pTriggerFunction = m_mtVectTriggerFunctionPtr[i];
			pTriggerFunction->OnAddDamage(this->MonsterInfoPtr->MonsterUnitKind, i_AttackFISock->GetCharacter()->CharacterUniqueNumber, i_AttackFISock->GetCharacter()->GuildUniqueNumber, i_AttackFISock->GetCharacter()->InfluenceType, i_fValue);
		}
	}
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
}

///////////////////////////////////////////////////////////////////////////////
// virtual
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::ResetMonster(void)
{
	CMonster::ResetMonster();

	m_usSendCountsMonsterMoveOK	= 0;

	m_mtlistDropItem.lock();
	if(m_mtlistDropItem.empty() == false)
	{
		mtlistMonsterDropItem::iterator itr = m_mtlistDropItem.begin();
		while(itr != m_mtlistDropItem.end())
		{
			delete(itr->pItemGeneral);
			itr++;
		}
	}
	m_mtlistDropItem.clear();
	m_mtlistDropItem.unlock();

	m_dwTimeCreatedTick						= 0;
	m_pCityWar2								= NULL;
	m_dwLastTickInfluenceBossAttackedMsg	= 0;


	m_nAttackerCliIdx						= 0;				// 2006-11-07 by cmkwon
	m_dwLastTickSetAttackerCliIdx			= 0;				// 2006-11-07 by cmkwon

	m_mtvectSAttackParameterList.clearLock();

	// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	m_mtVectTriggerFunctionPtr.clear();
	// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
}

void CFieldMonster::InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo)
{
	mt_auto_lock mtAuto(&m_mtvectorAttackedInfoPtr);
	if(MS_PLAYING != this->m_enMonsterState)
	{
		return;
	}

	mtvectorActionInfoPtr::iterator it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(),
		listActionInfoPtr_UnitIndex_Find_Object(i_pAttackedInfo->UnitIndex));
	if(it != m_mtvectorAttackedInfoPtr.end())
	{	// 리스트에 UnitIndex가 같은 정보가 있음

		(*it)->dwCurTick = i_pAttackedInfo->dwCurTick;
		(*it)->fSumDamage += i_pAttackedInfo->fSumDamage;
	}
	else
	{
		ActionInfo *ptmAInfo = new ActionInfo;
		*ptmAInfo = *i_pAttackedInfo;
		m_mtvectorAttackedInfoPtr.push_back(ptmAInfo);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMonster::APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::APInsertAttackParameter(SATTACK_PARAMETER *i_pAttParam)
{
	mt_auto_lock mtA(&m_mtvectSAttackParameterList);

	if(APFindAttackParameterNoLock(i_pAttParam->WeaponIndex))
	{
		return FALSE;
	}

	m_mtvectSAttackParameterList.push_back(*i_pAttParam);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SATTACK_PARAMETER *CFieldMonster::APFindAttackParameterNoLock(UID16_t i_WeaponIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SATTACK_PARAMETER *CFieldMonster::APFindAttackParameterNoLock(UID16_t i_WeaponIndex)
{
	mtvectSATTACK_PARAMETER::iterator itr(m_mtvectSAttackParameterList.begin());
	for(; itr != m_mtvectSAttackParameterList.end(); itr++)
	{
		if(itr->WeaponIndex == i_WeaponIndex)
		{
			return &*itr;
		}
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CFieldMonster::APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::APPopAttackParameter(SATTACK_PARAMETER *o_pAttParam, UID16_t i_WeaponIndex)
{
	mt_auto_lock mtA(&m_mtvectSAttackParameterList);

	DWORD dwCurTick = timeGetTime();
	mtvectSATTACK_PARAMETER::iterator itr(m_mtvectSAttackParameterList.begin());
	while(itr != m_mtvectSAttackParameterList.end())
	{
		// start 2011-06-04 by hskim, 인피니티 3차 - 주포용 긴 시간 2형 탄두 무기
		DWORD dwLifeTime = TICKGAP_WARHEAD_LIFETIME;	// 2011-06-04 by hskim, 인피니티 3차 - 주포용 긴 시간 2형 탄두 무기

		if( itr->pWeaponItemInfo->Kind == ITEMKIND_FOR_MON_MAIN_ARMAMENT  )
		{
			dwLifeTime = TICKGAP_WARHEAD_LIFETIME_LONG;
		}
		// end 2011-06-04 by hskim, 인피니티 3차 - 주포용 긴 시간 2형 탄두 무기		
		
		if(dwCurTick - itr->dwShotTick > dwLifeTime)		// 2011-06-04 by hskim, 인피니티 3차 - 주포용 긴 시간 2형 탄두 무기 - 적용 수정
		{
			itr = m_mtvectSAttackParameterList.erase(itr);
			continue;
		}
		else if(itr->WeaponIndex == i_WeaponIndex)
		{
			*o_pAttParam = *itr;

			itr = m_mtvectSAttackParameterList.erase(itr);
			return TRUE;
		}

		itr++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-06-08 ~ 2007-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::APCalcAttckParameter(SATTACK_PARAMETER *o_pAttParam, ITEM *i_pWeaponItemInfo, UID16_t i_WeaponIndex)
{
	if(IS_PRIMARY_WEAPON_MONSTER(i_pWeaponItemInfo->Kind))
	{
		o_pAttParam->dwShotTick				= 0;
	}
	else
	{// 2007-06-08 by cmkwon, 2형 탄두만 처리
		o_pAttParam->dwShotTick				= timeGetTime();
	}
	o_pAttParam->WeaponIndex				= i_WeaponIndex;
	o_pAttParam->pWeaponItemInfo			= i_pWeaponItemInfo;

	// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (몬스터 공격력 추가 수정, Pierce확률과 적중률 적용. ) - 몬스터 밸런스 인자 추가.
	o_pAttParam->fAttackProbability			= CAtumSJ::GetAttackProbabilityM ( MonsterInfoPtr , i_pWeaponItemInfo , &this->MonsterInfoExtend );
	//o_pAttParam->fAttackProbability			= CAtumSJ::GetAttackProbabilityM(MonsterInfoPtr, i_pWeaponItemInfo);

	// 2010. 06. 14 by hsLee 인피니티 필드 2차 난이도 조절. (몬스터 공격력 추가 수정, Pierce확률과 적중률 적용. ) - 몬스터 밸런스 인자 추가.
	o_pAttParam->fPierceAttackProbability	= CAtumSJ::GetPierceAttackProbabilityM ( MonsterInfoPtr->Level , i_pWeaponItemInfo , &this->MonsterInfoExtend );
	// 2009-12-17 by cmkwon, 데미지 계산식 관련한 필드(방어력,회피,피어스,확률)에 255이상 값을 설정 가능하게 수정 - 아래와 같이 공격무기를 인자로 넣는다
	//o_pAttParam->fPierceAttackProbability	= CAtumSJ::GetPierceAttackProbabilityM(MonsterInfoPtr->Level) + i_pWeaponItemInfo->FractionResistance;		// 2009-02-13 by cmkwon, 몬스터 공격 무기에 FractionResistance 을 적용 - 선전포고와 같이 업데이트 될 예정
	//o_pAttParam->fPierceAttackProbability	= CAtumSJ::GetPierceAttackProbabilityM(MonsterInfoPtr->Level, i_pWeaponItemInfo);

	// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
	//o_pAttParam->fAttack					= CAtumSJ::GetAttackM(i_pWeaponItemInfo);
	//o_pAttParam->fMaxAttack					= CAtumSJ::GetMaxAttackM(i_pWeaponItemInfo);

	o_pAttParam->fAttack					= CAtumSJ::GetAttackM(i_pWeaponItemInfo , &this->MonsterInfoExtend );
	o_pAttParam->fMaxAttack					= CAtumSJ::GetMaxAttackM(i_pWeaponItemInfo , &this->MonsterInfoExtend );
	// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )

	m_CurrentAttackItemNum	= i_pWeaponItemInfo->ItemNum;					// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2010-04-05 by cmkwon, 인피2차 M2M 2형 무기 보완 처리 - 
/// \author		cmkwon
/// \date		2010-04-05 ~ 2010-04-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::APPopOverLifeTimeAttackParameterList(vectSATTACK_PARAMETER *o_pOverTimeAttParamList)
{
	mt_auto_lock mtA(&m_mtvectSAttackParameterList);
	
	DWORD dwCurTick = timeGetTime();
	mtvectSATTACK_PARAMETER::iterator itr(m_mtvectSAttackParameterList.begin());
	while(itr != m_mtvectSAttackParameterList.end())
	{
		SATTACK_PARAMETER *pAttParam = &*itr;

		// start 2011-03-28 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
		DWORD dwStandardShotTick = TICKGAP_WARHEAD_LIFETIME; 

		if( pAttParam->pWeaponItemInfo->Kind == ITEMKIND_FOR_MON_MAIN_ARMAMENT )		// 인피니티 3차 모선 주포는 따로 처리 - 기존 2형보다 LifeTime이 길다
		{
			dwStandardShotTick = TICKGAP_WARHEAD_LIFETIME_LONG;
		}
		// end 2011-03-28 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가

		if(dwCurTick - pAttParam->dwShotTick > dwStandardShotTick)		// 2011-03-28 by hskim, 인피니티 3차 - 몬스터 멀티 타겟팅 기능 추가
		{
			o_pOverTimeAttParamList->push_back(*pAttParam);

			itr = m_mtvectSAttackParameterList.erase(itr);
			continue;
		}		
		itr++;
	}

	return !o_pOverTimeAttParamList->empty();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::HPRateCalcByMSWarInfoDisPlay(float i_fMonsterMaxHP, float i_fMonsterBeforeHP, float i_fMonsterAfterHP)
/// \brief		모선전 정보 표시 기획안 - 모선전 정보 표시와 관련된 몬스터 HPRate계산하여 정보 전송
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::HPRateCalcByMSWarInfoDisPlay(float i_fMonsterMaxHP, float i_fMonsterBeforeHP, float i_fMonsterAfterHP)
{
	INT		BeforeHPRate, AfterHPRate;

	BeforeHPRate = CalcGradeByRate(i_fMonsterMaxHP, i_fMonsterBeforeHP, MS_WAR_HP_RATE);
	AfterHPRate	 = CalcGradeByRate(i_fMonsterMaxHP, i_fMonsterAfterHP, MS_WAR_HP_RATE);
	if(BeforeHPRate != AfterHPRate)
	{
		m_pCurrentFieldMapChannelMonster->m_pFieldIOCPMapChannel->m_InflWarManager.SetMSWarInfo(m_pCurrentFieldMapChannelMonster->GetMapChannelIndex().MapIndex, T_MSWARINFO_MODIFY_HPRATE, AfterHPRate);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			모선전 정보 표시 기획안 - 모선전 정보 표시와 관련된 몬스터 인지 체크
/// \brief		BOOL CFieldMonster::MSMonsterCheckWithoutMSBossMonster()
/// \author		dhjin
/// \date		2008-03-28 ~ 2008-03-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldMonster::MSMonsterCheckWithoutMSBossMonster()
{
	if(IS_STRATEGYPOINT_MONSTER(this->MonsterInfoPtr->Belligerence)
		|| IS_TELEPORT_MONSTER(this->MonsterInfoPtr->Belligerence)
		)
	{
		return TRUE;
	}
		
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CFieldMonster::MonsterSkillApply(MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg, ITEM * i_nSkill)
/// \brief		인피니티 - 몬스터 스킬 적용
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::MonsterSkillApply(MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg, ITEM * i_nSkill) {
	if(m_enMonsterState != MS_PLAYING
		|| TRUE == COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK)
		|| CurrentHP <= 0.0f) {
		return;
	}

	switch(i_nSkill->ArrDestParameter[0])
	{
		case DES_SKILL_FULL_RECOVERY: 
			{
				// 완전회복
				// 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
				//this->CurrentHP = this->MonsterInfoPtr->MonsterHP;
				this->CurrentHP = this->MonsterInfoExtend.fMaxHP;
				// End 2010. 05. 19 by hsLee 인피니티 필드 2차 난이도 조절. (신호처리 + 몬스터 처리(서버) )
				m_pCurrentFieldMapChannelMonster->FieldSendMonsterChangeInfo(this, T_FN_MONSTER_CHANGE_HP);
			}
			break;

		case DES_WARP : 
			{
				// 순간 이동
				this->m_nTargetIndex		= i_pMsg->MapInfo.ChannelIndex;
				D3DXVECTOR3	tmVector3Pos	= A2DX(i_pMsg->PositionVector);
				
				// update block position
				this->m_pCurrentFieldMapChannelMonster->UpdateBlockPosition(this->PositionVector.x, this->PositionVector.z,
					tmVector3Pos.x, tmVector3Pos.z, this->MonsterIndex);
				
				// update position
				this->PositionVector		= A2DX(i_pMsg->PositionVector);
				D3DXVec3Normalize(&this->TargetVector, &(A2DX(i_pMsg->TargetVector)));			
			}
			break;

		case DES_SKILL_INVINCIBLE : 
			{
				// 배리어 스킬
				this->SetBarrier(i_nSkill);
			}
			break;

		case DES_SKILL_BARRIER : 
			{
				// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동)
				this->SetImmediateBarrier(i_nSkill);
				INIT_MSG_WITH_BUFFER(MSG_FC_BATTLE_MONSTER_BARRIER_USE, T_FC_BATTLE_MONSTER_BARRIER_USE, pSendMsg, SendBuf);
				pSendMsg->MonsterIndex		= this->MonsterIndex;
				pSendMsg->SkillItemNum		= this->m_ImmediateBarrier.SkillNum;
				m_pCurrentFieldMapChannelMonster->SendMessageToAllInChannel(SendBuf,  MSG_SIZE(MSG_FC_BATTLE_MONSTER_BARRIER_USE));
			}
			break;

		case DES_IMMEDIATE_HP_OR_DP_UP : 
			{
				this->IncreaseMonsterCurrentHP(i_nSkill->ArrParameterValue[0]);
				m_pCurrentFieldMapChannelMonster->FieldSendMonsterChangeInfo(this, T_FN_MONSTER_CHANGE_HP);
			}
			break;

	}

}

void CFieldMonster::SetBarrier(ITEM * i_pSkill) {
	m_bBarrierInfo.bBarrierSkillHave	= TRUE;
	m_bBarrierInfo.bBarrierSkillUsing	= FALSE;
	m_bBarrierInfo.HitRate				= i_pSkill->Charging;
	m_bBarrierInfo.dwLastTick			= timeGetTime();
	m_bBarrierInfo.ReAttackTick			= i_pSkill->ReAttacktime;
	m_bBarrierInfo.UseTick				= i_pSkill->Time;
	m_bBarrierInfo.SkillNum				= i_pSkill->ItemNum;
}

BOOL CFieldMonster::CheckBarrierHave() {
	if(!m_bBarrierInfo.bBarrierSkillHave) {
		// 스킬 사용 불가 
		return FALSE;
	}
	return TRUE;
}

BOOL CFieldMonster::CheckBarrierUsing() {
	DWORD dwCurTick = timeGetTime();
	DWORD dwElapseTick = dwCurTick - m_bBarrierInfo.dwLastTick;
	if(m_bBarrierInfo.bBarrierSkillUsing && dwElapseTick <= m_bBarrierInfo.UseTick)	{
		// 스킬 사용중 
		return TRUE;
	}
	else {
		m_bBarrierInfo.bBarrierSkillUsing = FALSE;
	}

	return FALSE;
}

BOOL CFieldMonster::BarrierUse() {
	DWORD dwCurTick = timeGetTime();	
	DWORD dwElapseTick = dwCurTick - m_bBarrierInfo.dwLastTick;
	if(dwElapseTick <= m_bBarrierInfo.ReAttackTick+m_bBarrierInfo.UseTick) {
		// 스킬 재사용 시간
		return FALSE;
	}
	

	Prob100_t nRand = RAND100();
	if(m_bBarrierInfo.HitRate < nRand) return FALSE;
	
	m_bBarrierInfo.dwLastTick	= dwCurTick;
	m_bBarrierInfo.bBarrierSkillUsing	= TRUE;
	return TRUE;
}

void CFieldMonster::SetImmediateBarrier(ITEM * i_pSkill) {
	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동) 설정
	m_ImmediateBarrier.bBarrierSkillUsing	= TRUE;
	m_ImmediateBarrier.dwLastTick			= timeGetTime();
	m_ImmediateBarrier.UseTick				= i_pSkill->Time;
	m_ImmediateBarrier.SkillNum				= i_pSkill->ItemNum;
}

BOOL CFieldMonster::CheckImmediateBarrierUsing() {
	// 2009-09-09 ~ 2010-01-15 by dhjin, 인피니티 - 몬스터 즉시 시전 배리어 스킬(고급무기만발동) 사용중 체크
	if(FALSE == m_ImmediateBarrier.bBarrierSkillUsing) {
		return FALSE;
	}

	DWORD dwCurTick = timeGetTime();
	DWORD dwElapseTick = dwCurTick - m_ImmediateBarrier.dwLastTick;
	if(m_ImmediateBarrier.bBarrierSkillUsing && dwElapseTick <= m_ImmediateBarrier.UseTick)	{
		// 스킬 사용중 
		return TRUE;
	}
	else {
		m_ImmediateBarrier.bBarrierSkillUsing = FALSE;
	}
	
	return FALSE;
}

CFieldMapChannel * CFieldMonster::GetCurrentMapChannelMonster() {
	return this->m_pCurrentFieldMapChannelMonster;
}

ItemNum_t CFieldMonster::GetCurrentAttackItemNum() {
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 인피 게임로그 추가
	return m_CurrentAttackItemNum;
}	

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CFieldMonster::ProcessingInserttoAttackedInfoList(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
/// \brief		몬스터 피 감소 처리 루틴 정리 - 
/// \author		dhjin
/// \date		2010-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CFieldMonster::ProcessingInserttoAttackedInfoList(float i_fValue, CFieldIOCPSocket *i_AttackFISock, vectActionInfo *i_pAInfoAggroList)
{
	if(NULL == i_AttackFISock
		|| NULL == i_pAInfoAggroList)
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 공격 당한 정보를 추가한다
	this->InserttoAttackedInfoList(&ActionInfo(i_AttackFISock->GetCharacter()->ClientIndex, timeGetTime(), i_fValue));
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-21 by cmkwon, Aggro 추가하기
	for(int i=0; i < i_pAInfoAggroList->size(); i++)
	{
		this->InserttoAttackedInfoList( &((*i_pAInfoAggroList)[i]) );
	}
}

// start 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
void CFieldMonster::InsertTriggerFunction(CTriggerFunction *pTriggerFunction)
{
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	m_mtVectTriggerFunctionPtr.push_back(pTriggerFunction);
}

BOOL CFieldMonster::IsTriggerFunction()
{
	mt_auto_lock mtAuto(&m_mtVectTriggerFunctionPtr);
	return NULL != m_mtVectTriggerFunctionPtr.size();
}
// end 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템