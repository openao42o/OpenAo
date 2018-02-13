// TriggerFunctionNGCInflWar.cpp: implementation of the CTriggerFunctionNGCInflWar class.
//
/// \brief		// 2011-11-07 by hskim, EP4 [트리거 시스템] - NCG 거점전 시스템
/// \author		hskim
/// \date		2011-11-07
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldMapProject.h"
#include "FieldMapChannel.h"
#include "TriggerFunctionNGCInflWar.h"
#include "TriggerNGCInflWarMonsterGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriggerFunctionNGCInflWar::CTriggerFunctionNGCInflWar()
{
	OnClear();
}

CTriggerFunctionNGCInflWar::~CTriggerFunctionNGCInflWar()
{
	OnDestroy();
}

void CTriggerFunctionNGCInflWar::OnClear()
{
	m_bCreatedNGCInflWar = FALSE;
	
	m_NGCInflWarGroupID = 0;
	m_PeriodTime = 0;
	m_BossMonster = 0;
	m_WinInfluenceWP = 0;
	m_LossInfluenceWP = 0;
	m_FirstDamageGuildWP = 0;
	m_FixedTimeType = 0;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);
	m_mtvectTriggerAttackUserDamage.clear();

	m_vectorTriggerNGCInflWarMonster.clear();

	m_NextCreateTick = 0;

	m_FirstCharacterUniqueNumber = 0;
	m_FirstGuildUniqueNumber = 0;
	m_WinInfluenceType = INFLUENCE_TYPE_UNKNOWN;
	m_DefeatInfluenceType = INFLUENCE_TYPE_UNKNOWN;

	// 2013-07-08 by jhseol, 트리거 시스템 확장
	m_bPossibleBossAttack = TRUE;
	m_bBeforePossibleBossAttack = FALSE;
	m_bIsBossDestroy = TRUE;
	m_nDestroyCrystalcount = 0;
	m_vectTriggerNGCInflWarBossKillInfo.clear();
	{
		mt_auto_lock mta(&m_mtvectTriggerNGCInflWarMonsterKillInfo);
		m_mtvectTriggerNGCInflWarMonsterKillInfo.clear();
	}
	// end 2013-07-08 by jhseol, 트리거 시스템 확장

	CTriggerFunction::OnClear();
}

BOOL CTriggerFunctionNGCInflWar::OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType)
{
	return CTriggerFunction::OnCreate(pMapTriggerManager, MapTriggerID, MapIndex, MapChannel, FunctionID, TriggerType);
}

void CTriggerFunctionNGCInflWar::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	if( FALSE == IsCreatedNGCInflWar() )
	{
		if( m_NextCreateTick == 0 || m_NextCreateTick < GetTickCount() )
		{	
			if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject )
			{	
				StopEvent();
				
				if( TRUE == m_pFieldMapChannel->m_pFieldMapProject->GetNPCServerStartFlag() && TRUE == m_pFieldMapChannel->m_pFieldMapProject->IsNPCServerMonsterCreated() )	// NPC 서버가 연결되어 몬스터 생성이 가능한가?
				{
					CreateAllMonster();

					m_bCreatedNGCInflWar = TRUE;
				}
				else
				{
#ifdef _DEBUG
					if( NULL != m_pFieldMapChannel )
					{
						char szString[256] = { 0, };
						sprintf(szString, "[NGC 모선전 시스템] NPC 서버 초기화 대기 중");
						m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);
					}
#endif
				}
			}
		}
		else
		{
#ifdef _DEBUG
			char szString[256] = { 0, };
			sprintf(szString, "[NGC 모선전 시스템] 다음 생성 시간 %d초 후", (m_NextCreateTick - GetTickCount()) / 1000);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);
#endif			
		}
	}
	// 2013-07-08 by jhseol, 트리거 시스템 확장 - 크리스탈 리젠
	else
	{
		CrystalRegen();
	}
	// end 2013-07-08 by jhseol, 트리거 시스템 확장 - 크리스탈 리젠
}

void CTriggerFunctionNGCInflWar::OnEventSkip(BOOL bCheckCreateCrystal/*= TRUE*/)		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
{
	if( FALSE == IsCreatedNGCInflWar() || FALSE == bCheckCreateCrystal )		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
	{
		m_NextCreateTick = GetTickCount();
		m_bCreatedNGCInflWar = FALSE;		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
	}
}

void CTriggerFunctionNGCInflWar::OnDestroy()
{
	for (auto x : m_vectorTriggerNGCInflWarMonster) delete x;
	
	CTriggerFunction::OnDestroy();
}

BOOL CTriggerFunctionNGCInflWar::OnMonsterCreate(CFieldMonster *pMonster)
{
	if( NULL == pMonster || NULL == pMonster->MonsterInfoPtr )
	{
		return FALSE;
	}

	if( m_BossMonster == pMonster->MonsterInfoPtr->MonsterUnitKind )
	{
		pMonster->InsertTriggerFunction(this);

		return TRUE;
	}

	// 2013-07-08 by jhseol, 트리거 시스템 확장
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo = m_vectTriggerNGCInflWarBossKillInfo.begin();		// 보스 몬스터를 잡기 위해 먼저 잡아야 하는 선행 몬스터 정보의 원본 벡터
	for (; itrBossKillInfo != m_vectTriggerNGCInflWarBossKillInfo.end() ; itrBossKillInfo++ )
	{
		if ( pMonster->MonsterInfoPtr->MonsterUnitKind == itrBossKillInfo->CrystalNum )
		{
			pMonster->InsertTriggerFunction(this);
			return TRUE;
		}
	}
	// end 2013-07-08 by jhseol, 트리거 시스템 확장

	return FALSE;
}

BOOL CTriggerFunctionNGCInflWar::OnMonsterDead(CFieldMonster *pMonster)
{
	if( NULL == pMonster || NULL == pMonster->MonsterInfoPtr )
	{
		return FALSE;
	}

	if( m_BossMonster == pMonster->MonsterInfoPtr->MonsterUnitKind )
	{
		RunEvent();
		DestroyCrystalMonster();		// 2013-07-08 by jhseol, 트리거 시스템 확장
		m_bIsBossDestroy = TRUE;		// 2013-07-08 by jhseol, 트리거 시스템 확장
		return TRUE;
	}
	// 2013-07-08 by jhseol, 트리거 시스템 확장 - 크리스탈 파괴
	else
	{
		if ( 0 >= m_nDestroyCrystalcount )
		{
			return FALSE;
		}
		BOOL Rect = FALSE;
		mt_auto_lock mta(&m_mtvectTriggerNGCInflWarMonsterKillInfo);
		mtvectorTriggerOutPostMonsterKillInfo::iterator itrKillMonsterInfo = m_mtvectTriggerNGCInflWarMonsterKillInfo.begin();
		for (; itrKillMonsterInfo != m_mtvectTriggerNGCInflWarMonsterKillInfo.end() ; itrKillMonsterInfo++ )
		{
			if ( itrKillMonsterInfo->CrystalNum == pMonster->MonsterInfoPtr->MonsterUnitKind )
			{
				Rect = TRUE;
				itrKillMonsterInfo->IsDestroy = TRUE;
				itrKillMonsterInfo->DestroyTime.SetCurrentDateTime();
				
				if ( TRUE == itrKillMonsterInfo->IsDestroy )
				{
					RunKeyMonsterKillEvent(itrKillMonsterInfo->CrystalNum);
				}
				break;
			}
		}
		
		m_bPossibleBossAttack = IsPossibleBossAttack();
		return Rect;
	}
	// end 2013-07-08 by jhseol, 트리거 시스템 확장 - 크리스탈 파괴

	return FALSE;
}

BOOL CTriggerFunctionNGCInflWar::OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage)
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	if( FALSE == IsCreatedNGCInflWar() )
	{
		return FALSE;
	}

	mtvectTriggerAttackUserDamage::iterator itr = m_mtvectTriggerAttackUserDamage.begin();
	while(itr != m_mtvectTriggerAttackUserDamage.end())
	{
		if( itr->MonsterIdx == MonsterIdx && itr->CharacterUniqueNumber == CharacterUniqueNumber )
		{
			itr->fDamage += fDamage;
			itr->GuildUniqueNumber = GuildUniqueNumber;
			itr->InfluenceType = InfluenceType;

			return TRUE;
		}

		itr++;
	}

	STRIGGER_ATTACK_USER_DAMAGE	tmCrystalDamage;
	tmCrystalDamage.MonsterIdx = MonsterIdx;
	tmCrystalDamage.CharacterUniqueNumber = CharacterUniqueNumber;
	tmCrystalDamage.GuildUniqueNumber = GuildUniqueNumber;
	tmCrystalDamage.InfluenceType = InfluenceType;
	tmCrystalDamage.fDamage = fDamage;
	m_mtvectTriggerAttackUserDamage.push_back(tmCrystalDamage);

	return TRUE;
}

void CTriggerFunctionNGCInflWar::SetNGCInflWarInfo(NGCInflWarGroupID_t NGCInflWarGroupID, INT PeriodTime, MonIdx_t BossMonster, INT WinInfluenceWP, INT LossInfluenceWP, INT FirstDamageGuildWP, INT FixedTimeType, INT i_nDestroyCrystalcount)	// 2013-07-08 by jhseol, 트리거 시스템 확장
{
	m_NGCInflWarGroupID = NGCInflWarGroupID;
	m_PeriodTime = PeriodTime;
	m_BossMonster = BossMonster;
	m_WinInfluenceWP = WinInfluenceWP;
	m_LossInfluenceWP = LossInfluenceWP;
	m_FirstDamageGuildWP = FirstDamageGuildWP;
	m_FixedTimeType = FixedTimeType;
	m_nDestroyCrystalcount = i_nDestroyCrystalcount;					// 2013-07-08 by jhseol, 트리거 시스템 확장
}

BOOL CTriggerFunctionNGCInflWar::InsertNGCInflWarMonster(CTriggerNGCInflWarMonsterGroup *pTriggerNGCInflWarMonster)
{
	if( NULL == pTriggerNGCInflWarMonster )
	{
		return FALSE;
	}

	m_vectorTriggerNGCInflWarMonster.push_back(pTriggerNGCInflWarMonster);

	return TRUE;
}

// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
void CTriggerFunctionNGCInflWar::ResetLocalData()
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);
	m_mtvectTriggerAttackUserDamage.clear();
}
// end 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

void CTriggerFunctionNGCInflWar::SetNextCreateTime()
{
	if( m_FixedTimeType != 0 )		// 추후 해당 Type 더 생기면 DEFINE 및 switch 처리 하세요
	{
		INT nCalcPeriodTime = 0;
		int nDiffMinute = 0;
		ATUM_DATE_TIME atSetTime(true);
		ATUM_DATE_TIME atCurrentTime(true);

		atSetTime.AddDateTime(0,0,0,0,m_PeriodTime,0);
		atSetTime.SetDateTime(atSetTime.Year, atSetTime.Month, atSetTime.Day , 18, 0, 0);		// 선택된날 저녁 6시 기준

		nDiffMinute = atSetTime.GetTimeDiffTimeInMinutes(atCurrentTime);

		nCalcPeriodTime += nDiffMinute;
		nCalcPeriodTime += RANDI(0, 360);														// 6시간중 랜덤 (저녁 6시 ~ 12시 까지 랜덤 : 6 시간 * 60 분  = 360 분)

		m_NextCreateTick = GetTickCount() + (TIMERGAP_ALIVE_CHECK * nCalcPeriodTime);
	}
	else
	{
	m_NextCreateTick = GetTickCount() + (TIMERGAP_ALIVE_CHECK * m_PeriodTime);
}
}

void CTriggerFunctionNGCInflWar::RunEvent()
{
	FindSequenceAmountAttack();			// 데미지를 가장 많이 준 유저부터 리스트 확보
	FindFirstDamageUser();		// 데미지 1등 길드 찾기
	FindWinInfluenc();			// 이긴 세력 찾기

	DestroyAllMonster();		// 모든 이벤트 몬스터 삭제

	if( NULL != m_pFieldMapChannel )
	{
		// 보상 지급

		if ( 0 < m_WinInfluenceWP )
		{
			m_pFieldMapChannel->AddWarPointInMap(m_WinInfluenceWP, TRUE, m_WinInfluenceType, WPUT_TRIGGER);
		}
		if ( 0 < m_LossInfluenceWP )
		{
			m_pFieldMapChannel->AddWarPointInMap(m_LossInfluenceWP, TRUE, m_DefeatInfluenceType, WPUT_TRIGGER);
		}

		/*
		// 2011-11-07 by hskim, 기획팀 요청에 의해 길드 추가 WP 삭제
		if( m_FirstGuildUniqueNumber == 0 )
		{
			m_pFieldMapChannel->AddWarPointInMapByCharacterUID(m_FirstDamageGuildWP, TRUE, m_FirstCharacterUniqueNumber, WPUT_TRIGGER);
		}
		else
		{
			m_pFieldMapChannel->AddWarPointInMapByGuildUID(m_FirstDamageGuildWP, TRUE, m_FirstGuildUniqueNumber, WPUT_TRIGGER);
		}
		*/

		if( TRUE == IsMapIndex(STRIGEER_NGCINFLWAR_MAPINDEX_2ND) )
		{
			// 1등부터 15등까지 보상 지급 (현재 맵에 있는 유저기준)

			int iCount = 0;

			mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

			for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
			{
				if( iCount < STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_PAY_RANK )		// 5등 까지
				{
					m_pFieldMapChannel->InsertItemInMap(STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_UID, STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_COUNT, m_mtvectTriggerAttackUserDamage[a].InfluenceType, m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber);

					iCount++;

					continue;
				}

				if( iCount < STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_PAY_RANK )		// 15등 까지
				{
					m_pFieldMapChannel->InsertItemInMap(STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_UID, STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_COUNT, m_mtvectTriggerAttackUserDamage[a].InfluenceType, m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber);

					iCount++;

					continue;
				}

				if( iCount >= STRIGEER_NGCINFLWAR_EVENT_2ND_MIDDLE_GROUP_ITEM_PAY_RANK )
				{
					break;		// 모든 지급 종료 더 이상은 의미 없음
				}
			}

			m_pFieldMapChannel->InsertItemInMap(STRIGEER_NGCINFLWAR_EVENT_2ND_ALL_ITEM_UID, STRIGEER_NGCINFLWAR_EVENT_2ND_ALL_ITEM_COUNT, INFLUENCE_TYPE_ALL_MASK);
		}
		// 2013-07-08 by jhseol, 트리거 시스템 확장 - 아이탬 보상
		else if (TRUE == IsMapIndex(STRIGEER_NGCINFLWAR_WW2_EVENT_MAPINDEX_BCU) || TRUE == IsMapIndex(STRIGEER_NGCINFLWAR_WW2_EVENT_MAPINDEX_ANI))
		{
			m_pFieldMapChannel->InsertItemInMap(STRIGEER_NGCINFLWAR_WW2_EVENT_ITEM_NUM, STRIGEER_NGCINFLWAR_EVENT_2ND_TOP_GROUP_ITEM_COUNT, INFLUENCE_TYPE_ALL_MASK);
		}
		// 2013-07-08 by jhseol, 트리거 시스템 확장 - 아이탬 보상
	}

	SetNextCreateTime();		// 다음 이벤트 설정

	m_bCreatedNGCInflWar = FALSE;
}

void CTriggerFunctionNGCInflWar::StopEvent()
{
	ResetLocalData();		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
	InitBossKillPlayStep();			// 2013-07-08 by jhseol, 트리거 시스템 확장
}

void CTriggerFunctionNGCInflWar::CreateAllMonster()
{
	for(int i=0; i<m_vectorTriggerNGCInflWarMonster.size(); i++)
	{
		CTriggerNGCInflWarMonsterGroup *pNGCInflWar = m_vectorTriggerNGCInflWarMonster[i];

		pNGCInflWar->CreateMonster();
	}

	// 2013-07-08 by jhseol, 트리거 시스템 확장 - 제어기 생성
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return;
	}
	CreateCrystalMonster();
	m_bIsBossDestroy = FALSE;
	m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_CHANGE);
	// end 2013-07-08 by jhseol, 트리거 시스템 확장 - 제어기 생성
}

void CTriggerFunctionNGCInflWar::DestroyAllMonster()
{
	for(int i=0; i<m_vectorTriggerNGCInflWarMonster.size(); i++)
	{
		CTriggerNGCInflWarMonsterGroup *pNGCInflWar = m_vectorTriggerNGCInflWarMonster[i];

		pNGCInflWar->DestroyMonster();
	}

	// 2013-07-08 by jhseol, 트리거 시스템 확장 - 제어기 제거
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return;
	}
	DestroyCrystalMonster();
	m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT);
	// end 2013-07-08 by jhseol, 트리거 시스템 확장 - 제어기 제거
}

void CTriggerFunctionNGCInflWar::FindFirstDamageUser()
{
	float fMaxDamage = 0.0f;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
	{
		if( m_mtvectTriggerAttackUserDamage[a].MonsterIdx == m_BossMonster )
		{
#ifdef _DEBUG
			char szString[256] = { 0, };
			sprintf(szString, "[NGC 모선전 시스템] 데미지 CharacterUID [%d] MonsterUID[%d] Damage [%10.0f]", 
			m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber, m_mtvectTriggerAttackUserDamage[a].MonsterIdx, m_mtvectTriggerAttackUserDamage[a].fDamage);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif

			if( fMaxDamage < m_mtvectTriggerAttackUserDamage[a].fDamage )
			{
				fMaxDamage = m_mtvectTriggerAttackUserDamage[a].fDamage;

				m_FirstCharacterUniqueNumber = m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber;
				m_FirstGuildUniqueNumber = m_mtvectTriggerAttackUserDamage[a].GuildUniqueNumber;
			}
		}
	}

#ifdef _DEBUG
	char szString[256] = { 0, };
	sprintf(szString, "[NGC 모선전 시스템] 1등 GuildUID [%d] CharacterUID [%d] ", m_FirstGuildUniqueNumber, m_FirstCharacterUniqueNumber);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif
}

void CTriggerFunctionNGCInflWar::FindSequenceAmountAttack()
{
	int iCount = 0;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	sort(m_mtvectTriggerAttackUserDamage.begin(), m_mtvectTriggerAttackUserDamage.end(), SSORT_TRIGGER_ATTACK_USER_DAMAGE_BY__DAMAGE());

	m_mtvectTriggerDamageSortUser.clear();

	for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
	{
		if( m_mtvectTriggerAttackUserDamage[a].MonsterIdx == m_BossMonster )
		{
#ifdef _DEBUG
			char szString[256] = { 0, };
			sprintf(szString, "[NGC 모선전 시스템] %4d => 데미지 CharacterUID [%d] MonsterUID[%d] Damage [%12.0f]", 
			iCount + 1, m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber, m_mtvectTriggerAttackUserDamage[a].MonsterIdx, m_mtvectTriggerAttackUserDamage[a].fDamage);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif
			m_mtvectTriggerDamageSortUser.push_back(m_mtvectTriggerAttackUserDamage[a]);
		}
	}
}

void CTriggerFunctionNGCInflWar::FindWinInfluenc()
{
	BOOL bWinBCU = FALSE;
	float fBCUDamage = 0.0f;
	float fANIDamage = 0.0f;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
	{
		if( m_mtvectTriggerAttackUserDamage[a].MonsterIdx == m_BossMonster )
		{
			if( TRUE == IS_VCN_INFLUENCE_TYPE(m_mtvectTriggerAttackUserDamage[a].InfluenceType) )
			{
				fBCUDamage += m_mtvectTriggerAttackUserDamage[a].fDamage;
			}

			if( TRUE == IS_ANI_INFLUENCE_TYPE(m_mtvectTriggerAttackUserDamage[a].InfluenceType) )
			{
				fANIDamage += m_mtvectTriggerAttackUserDamage[a].fDamage;
			}
		}
	}

	if( fBCUDamage == fANIDamage )		// 설마 같다?
	{
		if( ((int)fBCUDamage % 2) == 0 )		// 운좋은 세력이 이긴세력
		{
			bWinBCU = TRUE;
		}
	}
	else
	{
		if( fBCUDamage > fANIDamage )
		{
			bWinBCU = TRUE;
		}
	}

	if( TRUE == bWinBCU )
	{
		m_WinInfluenceType = INFLUENCE_TYPE_VCN;
		m_DefeatInfluenceType = INFLUENCE_TYPE_ANI;
	}
	else
	{
		m_WinInfluenceType = INFLUENCE_TYPE_ANI;
		m_DefeatInfluenceType = INFLUENCE_TYPE_VCN;		
	}

#ifdef _DEBUG
	char szString[256] = { 0, };
	sprintf(szString, "[NGC 모선전 시스템] 이긴 세력 [%d] 패배 세력 [%d]", m_WinInfluenceType, m_DefeatInfluenceType);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif
}

// 2013-07-08 by jhseol, 트리거 시스템 확장
void CTriggerFunctionNGCInflWar::InsertBossMonsterKillInfo(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* i_BossKillInfo)
{
	if( NULL != i_BossKillInfo )
	{
		STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO tmBossKillInfo;
		util::zero(&tmBossKillInfo, sizeof(tmBossKillInfo));
		memcpy(&tmBossKillInfo, i_BossKillInfo, sizeof(tmBossKillInfo));
		m_vectTriggerNGCInflWarBossKillInfo.push_back(tmBossKillInfo);			// 보스몬스터 공략 정보를 원본 벡터에 추가.
		
		mt_auto_lock mta(&m_mtvectTriggerNGCInflWarMonsterKillInfo);
		m_mtvectTriggerNGCInflWarMonsterKillInfo.push_back(tmBossKillInfo);	// 이어서 진행단계를 저장할 백터에 동일한 정보를 추가.
	}
}

void CTriggerFunctionNGCInflWar::InitBossKillPlayStep()
{
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return;
	}
	m_bPossibleBossAttack = FALSE;
	
	mt_auto_lock mta(&m_mtvectTriggerNGCInflWarMonsterKillInfo);
	m_mtvectTriggerNGCInflWarMonsterKillInfo.clear();
	
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo =  m_vectTriggerNGCInflWarBossKillInfo.begin();
	for (; itrBossKillInfo != m_vectTriggerNGCInflWarBossKillInfo.end() ; itrBossKillInfo++ )
	{
		STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO tmBossKillInfo;
		util::zero(&tmBossKillInfo, sizeof(tmBossKillInfo));
		memcpy(&tmBossKillInfo, &*itrBossKillInfo, sizeof(tmBossKillInfo));
		m_mtvectTriggerNGCInflWarMonsterKillInfo.push_back(tmBossKillInfo);
	}
}

void CTriggerFunctionNGCInflWar::CreateCrystalMonster()
{
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return;
	}
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo = m_vectTriggerNGCInflWarBossKillInfo.begin();
	for (; itrBossKillInfo != m_vectTriggerNGCInflWarBossKillInfo.end() ; itrBossKillInfo++ )
	{
		if( NULL != m_pFieldMapChannel )
		{
			INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pMsgSummonMonster, SendBuf);
			pMsgSummonMonster->ChannelIndex			= m_pFieldMapChannel->GetMapChannelIndex().ChannelIndex;
			util::strncpy(pMsgSummonMonster->CharacterName, "CrystalMonster", SIZE_MAX_CHARACTER_NAME);
			pMsgSummonMonster->MonsterUnitKind		= itrBossKillInfo->CrystalNum;
			pMsgSummonMonster->MonsterTargetType1	= MONSTER_TARGETTYPE_NORMAL;
			pMsgSummonMonster->TargetTypeData1		= 0;
			pMsgSummonMonster->CltIdxForTargetType1	= 0;
			pMsgSummonMonster->NumOfMonster			= 1;
			pMsgSummonMonster->Position				= AVECTOR3(0, 0, 0);
			pMsgSummonMonster->MonsterBalanceData.Init(0);
			m_pFieldMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
		}
	}
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130708_0003);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130708_0001);
}

void CTriggerFunctionNGCInflWar::DestroyCrystalMonster()
{
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return;
	}
	vectorTriggerOutPostBossKillInfo::iterator itrBossKillInfo = m_vectTriggerNGCInflWarBossKillInfo.begin();
	for (; itrBossKillInfo != m_vectTriggerNGCInflWarBossKillInfo.end() ; itrBossKillInfo++ )
	{
		if( NULL != m_pFieldMapChannel )
		{
			INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pDestroy, SendBufDestroy);
			pDestroy->mapChann			= m_pFieldMapChannel->GetMapChannelIndex();
			pDestroy->MonsterUnitKind	= itrBossKillInfo->CrystalNum;
			((CFieldIOCP*)(m_pFieldMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufDestroy, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY));
		}
	}
}

void CTriggerFunctionNGCInflWar::CrystalRegen()
{
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return;
	}
	ATUM_DATE_TIME tmCurrentDateTime { true };
	mt_auto_lock mta(&m_mtvectTriggerNGCInflWarMonsterKillInfo);
	mtvectorTriggerOutPostMonsterKillInfo::iterator itrKillMonsterInfo = m_mtvectTriggerNGCInflWarMonsterKillInfo.begin();
	for (; itrKillMonsterInfo != m_mtvectTriggerNGCInflWarMonsterKillInfo.end() ; itrKillMonsterInfo++ )
	{
		if ( TRUE == itrKillMonsterInfo->IsDestroy )
		{
			if ( RANDI(itrKillMonsterInfo->RegenMinTimeMinutes*60, itrKillMonsterInfo->RegenMaxTimeMinutes*60) < tmCurrentDateTime.GetTimeDiffTimeInSeconds(itrKillMonsterInfo->DestroyTime) )
			{
				if( NULL != m_pFieldMapChannel )
				{
					itrKillMonsterInfo->IsDestroy = FALSE;
					INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pMsgSummonMonster, SendBuf);
					pMsgSummonMonster->ChannelIndex			= m_pFieldMapChannel->GetMapChannelIndex().ChannelIndex;
					util::strncpy(pMsgSummonMonster->CharacterName, "CrystalMonster", SIZE_MAX_CHARACTER_NAME);
					pMsgSummonMonster->MonsterUnitKind		= itrKillMonsterInfo->CrystalNum;
					pMsgSummonMonster->MonsterTargetType1	= MONSTER_TARGETTYPE_NORMAL;
					pMsgSummonMonster->TargetTypeData1		= 0;
					pMsgSummonMonster->CltIdxForTargetType1	= 0;
					pMsgSummonMonster->NumOfMonster			= 1;
					pMsgSummonMonster->Position				= AVECTOR3(0, 0, 0);
					pMsgSummonMonster->MonsterBalanceData.Init(0);
					m_pFieldMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
					
					m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130121_0002);
				}
			}
		}
	}
	m_bPossibleBossAttack = IsPossibleBossAttack();
}

BOOL CTriggerFunctionNGCInflWar::IsPossibleBossAttack()
{
	if ( 0 >= m_nDestroyCrystalcount )
	{
		return TRUE;
	}

	if ( TRUE == m_bIsBossDestroy )
	{
		return FALSE;
	}
	int DestoryCrystalCount = 0;
	mt_auto_lock mta(&m_mtvectTriggerNGCInflWarMonsterKillInfo);
	mtvectorTriggerOutPostMonsterKillInfo::iterator itrKillMonsterInfo = m_mtvectTriggerNGCInflWarMonsterKillInfo.begin();
	for (; itrKillMonsterInfo != m_mtvectTriggerNGCInflWarMonsterKillInfo.end() ; itrKillMonsterInfo++ )
	{
		if ( TRUE == itrKillMonsterInfo->IsDestroy )
		{
			DestoryCrystalCount++;
		}
	}
	
	if ( m_nDestroyCrystalcount <= DestoryCrystalCount )
	{
		if ( FALSE == m_bBeforePossibleBossAttack && NULL != m_pFieldMapChannel )
		{
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130708_0002);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_DEFAULT);
			m_bBeforePossibleBossAttack = TRUE;
		}
		return TRUE;
	}
	else
	{
		if ( TRUE == m_bBeforePossibleBossAttack  && NULL != m_pFieldMapChannel )
		{
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130708_0001);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_TRIGGER_EX_1ST_VALUE_BOSS_PROTECTIVE_FILM_OBJECT_NUM_CHANGE);
			m_bBeforePossibleBossAttack = FALSE;
		}
		return FALSE;
	}
}

BOOL CTriggerFunctionNGCInflWar::OnIsPossibleBossAttack(MonIdx_t MonsterIdx)
{
	if ( m_BossMonster == MonsterIdx )
	{	// 보스몬스터만 여부를 확인하고
		return m_bPossibleBossAttack;
	}
	else
	{	// 아니라면 무조건 공격이 가능하다
		return TRUE;
	}
}

void CTriggerFunctionNGCInflWar::RunKeyMonsterKillEvent(MonIdx_t i_nMonsterNum)
{
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, STRMSG_130708_0004);
	
	if ( TRUE != m_vectCrystalBuff.empty() && NULL != m_pFieldMapChannel )
	{
		random_shuffle(m_vectCrystalBuff.begin(), m_vectCrystalBuff.end());
		vectorTriggerCrystalBuff::iterator itrCrystalBuff = m_vectCrystalBuff.begin();
		m_pFieldMapChannel->ApplySkillToAllInChannel(itrCrystalBuff->SkillItemNum, INFLUENCE_TYPE_ALL_MASK);
	}
	
	return;
}

void CTriggerFunctionNGCInflWar::InsertCrystalBuff(STRIGGER_CRYSTAL_BUFF *i_CrystalBuff)
{
	if( NULL != i_CrystalBuff )
	{
		STRIGGER_CRYSTAL_BUFF tmCrystalBuff;
		util::zero(&tmCrystalBuff, sizeof(tmCrystalBuff));
		memcpy(&tmCrystalBuff, i_CrystalBuff, sizeof(tmCrystalBuff));
		m_vectCrystalBuff.push_back(tmCrystalBuff);
	}
}

INT CTriggerFunctionNGCInflWar::GetDestroyCrystalcount()
{
	return m_nDestroyCrystalcount;
}
// end 2013-07-08 by jhseol, 트리거 시스템 확장