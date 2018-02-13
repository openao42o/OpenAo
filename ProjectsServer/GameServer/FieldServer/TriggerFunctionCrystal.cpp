// TriggerFunctionCrystal.cpp: implementation of the CTriggerFunctionCrystal class.
//
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapTriggerManager.h"
#include "TriggerFunctionCrystal.h"
#include "FieldMapProject.h"
#include "FieldMapChannel.h"
#include "TriggerCrystalGroup.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriggerFunctionCrystal::CTriggerFunctionCrystal()
{
	OnClear();
}

CTriggerFunctionCrystal::~CTriggerFunctionCrystal()
{
	OnDestroy();
}

void CTriggerFunctionCrystal::OnClear()
{
	m_CrystalGroupID = 0;
	m_PeriodTime = 0;
	m_RandomSequence = 0;

	m_bCreatedCrystal = FALSE;
	m_pDefaultCrystalGroup = NULL;
	m_vectorTriggerCrystal.clear();

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);
	m_mtvectTriggerAttackUserDamage.clear();
	
	m_vectorDestroyedSequence.clear();

	ResetEventData();
	m_NextTickEventCreate = 0;
	m_NextTickEvent1st = 0;
	m_NextTickEvent2nd = 0;
	m_ProcessEventID = STRIGEER_CRYSTAL_EVENT_NONE;

	CTriggerFunction::OnClear();
}

BOOL CTriggerFunctionCrystal::OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType)
{
	return CTriggerFunction::OnCreate(pMapTriggerManager, MapTriggerID, MapIndex, MapChannel, FunctionID, TriggerType);
}

void CTriggerFunctionCrystal::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
	if( FALSE == IsCreatedCrystal() )
	{
		if( m_NextTickEvent1st != 0 && m_NextTickEvent1st < GetTickCount() )
		{
			if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject )
			{
				if( STRIGEER_CRYSTAL_EVENT_HELL_WARP == m_ProcessEventID || STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP == m_ProcessEventID )
				{
					StepEvent(m_ProcessEventID, EVENT_OPTION_HELL_WARP_CLOSE);
					m_NextTickEvent1st = 0;
				}
			}
		}

		if( m_NextTickEvent2nd != 0 && m_NextTickEvent2nd < GetTickCount() )
		{
			if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject )
			{
				if( STRIGEER_CRYSTAL_EVENT_HELL_WARP == m_ProcessEventID || STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP == m_ProcessEventID )
				{
					StepEvent(m_ProcessEventID, EVENT_OPTION_HELL_BACK_MAP_WARP);
					m_NextTickEvent2nd = 0;
				}
			}
		}

		if( m_NextTickEventCreate == 0 || m_NextTickEventCreate < GetTickCount() )
		{
			if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject )
			{
				StopEvent(m_ProcessEventID);

				if( TRUE == m_pFieldMapChannel->m_pFieldMapProject->GetNPCServerStartFlag() && TRUE == m_pFieldMapChannel->m_pFieldMapProject->IsNPCServerMonsterCreated() )	// NPC 서버가 연결되어 몬스터 생성이 가능한가?
				{
					if( TRUE == m_RandomSequence )
					{
						MixEvent();
					}

					m_pDefaultCrystalGroup->CreateAllCrystal();
					m_bCreatedCrystal = TRUE;
				}
				else
				{
#ifdef _DEBUG
					if( NULL != m_pFieldMapChannel )
					{
						char szString[256] = { 0, };
						sprintf(szString, "[크리스탈 시스템] NPC 서버 초기화 대기 중");
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
			sprintf(szString, "[크리스탈 시스템] 다음 생성 시간 %d초 후", (m_NextTickEventCreate - GetTickCount()) / 1000);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);
#endif
		}
	}
}

void CTriggerFunctionCrystal::OnEventSkip(BOOL bCheckCreateCrystal/*=TRUE*/)		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
{
	if( FALSE == IsCreatedCrystal() || FALSE == bCheckCreateCrystal )		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
	{
		TimeUnit_t iCurrentTickCount = GetTickCount();

		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
		m_bCreatedCrystal = FALSE;
		ResetLocalData();
		ResetEventData();
		// end 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

		if( 0 != m_NextTickEventCreate ) m_NextTickEventCreate = iCurrentTickCount;
		if( 0 != m_NextTickEvent1st ) m_NextTickEvent1st = iCurrentTickCount;
		if( 0 != m_NextTickEvent2nd ) m_NextTickEvent2nd = iCurrentTickCount;
	}
}

BOOL CTriggerFunctionCrystal::OnMonsterCreate(CFieldMonster *pMonster)
{
	if( NULL == pMonster || NULL == pMonster->MonsterInfoPtr )
	{
		return FALSE;
	}

	if( TRUE == IsBelongCrystal(pMonster->MonsterInfoPtr->MonsterUnitKind) )
	{
		pMonster->InsertTriggerFunction(this);

		return TRUE;
	}

	return FALSE;
}

BOOL CTriggerFunctionCrystal::OnMonsterDead(CFieldMonster *pMonster)
{
	if( NULL == pMonster || NULL == pMonster->MonsterInfoPtr )
	{
		return FALSE;
	}

	if( TRUE == IsBelongCrystal(pMonster->MonsterInfoPtr->MonsterUnitKind) )
	{
		InsertDestroyedSequence(pMonster->MonsterInfoPtr->MonsterUnitKind);

		return TRUE;
	}

	return FALSE;
}

void CTriggerFunctionCrystal::OnDestroy()
{
	for (auto x : m_vectorTriggerCrystal) delete x;

	CTriggerFunction::OnDestroy();
}

void CTriggerFunctionCrystal::SetCrystalInfo(CrystalGroupID_t CrystalGroupID, INT PeriodTime, BYTE RandomSequence)
{
	m_CrystalGroupID = CrystalGroupID;
	m_PeriodTime = PeriodTime;
	m_RandomSequence = RandomSequence;
}

BOOL CTriggerFunctionCrystal::InsertTriggerCrystal(CTriggerCrystalGroup *pTriggerCrystalGroup)
{
	if( NULL == pTriggerCrystalGroup )
	{
		return FALSE;
	}

	m_vectorTriggerCrystal.push_back(pTriggerCrystalGroup);

	return TRUE;
}

BOOL CTriggerFunctionCrystal::IsBelongCrystal(MonIdx_t MonsterIdx)
{
	return m_pDefaultCrystalGroup->IsBelongCrystal(MonsterIdx);
}

BOOL CTriggerFunctionCrystal::OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage)
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	if( FALSE == IsCreatedCrystal() )
	{
		return FALSE;
	}

	if( 0.0f == fDamage )
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

BOOL CTriggerFunctionCrystal::OnIsInvincible(MonIdx_t MonsterIdx, CFieldIOCPSocket *pIOCPSocket)
{
	if( NULL == pIOCPSocket || FALSE == pIOCPSocket->IsValidCharacter(FALSE) )
	{
		return FALSE;
	}

	if( m_pDefaultCrystalGroup->IsBelongCrystal(MonsterIdx) )
	{
		mt_auto_lock mtAuto(&pIOCPSocket->m_ItemManager.m_mapItemGeneral);
		ITEM_GENERAL *pUsingItemG = pIOCPSocket->m_ItemManager.FindUsingItemByItemNumNoLock(7033440);		// 박힌 번호는 추후 DB로 뺄 예정임
		if( NULL == pUsingItemG)				
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 2013-01-23 by jhseol, 전쟁 시 크리스탈만 공격 불가능 하도록 수정
BOOL CTriggerFunctionCrystal::OnIsCrystal()
{
	return TRUE;
}
// end 2013-01-23 by jhseol, 전쟁 시 크리스탈만 공격 불가능 하도록 수정

void CTriggerFunctionCrystal::InsertDestroyedSequence(MonIdx_t MonsterIdx)
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	if( FALSE == IsCreatedCrystal() )
	{
		return ;
	}

	m_vectorDestroyedSequence.push_back(MonsterIdx);
	RunDestroyEvent();
}

BOOL CTriggerFunctionCrystal::RunDestroyEvent()
{
	BOOL bReturn = TRUE;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	for(int i=0; i<m_vectorTriggerCrystal.size(); i++)
	{
		CTriggerCrystalGroup *pTriggerCrystalGroup = m_vectorTriggerCrystal[i];

		if( pTriggerCrystalGroup == NULL || pTriggerCrystalGroup == m_pDefaultCrystalGroup )
		{
			continue;
		}

		if( TRUE == pTriggerCrystalGroup->CheckDestroyEvent() )
		{
			FindFirstDamageUser();
			FindWinnerInfluence();

			ResetLocalData();		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

			// 딱 맞게 부쉈습니다. 고갱님 이벤트 가빈닷

#ifdef _DEBUG
			char szString[256] = { 0, };
			sprintf(szString, "[크리스탈 시스템] 이벤트 발생 = %d", pTriggerCrystalGroup->m_EventID);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);
#endif

			RunEvent(pTriggerCrystalGroup->m_EventID);

			SetNextCreateTime();

			return TRUE;
		}

		if( TRUE == pTriggerCrystalGroup->CheckDestroySequence() )
		{
			bReturn = FALSE;
		}
	}

	if( TRUE == bReturn )
	{
		// 부시는 순서가 틀렸네욤?

#ifdef _DEBUG
		char szString[256] = { 0, };
		sprintf(szString, "[크리스탈 시스템] 파괴 순서 틀림");
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif

		char szSendString[256] = { 0, };
		sprintf(szSendString, STRMSG_120330_0009);
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);

		ResetLocalData();		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
		ResetEventData();

		SetNextCreateTime();
	}

	return bReturn;
}

BOOL CTriggerFunctionCrystal::SetAllShopRemainCountZero()
{
	if( NULL == m_pFieldMapChannel || NULL == m_pFieldMapChannel->m_pFieldMapProject )
	{
		return FALSE;
	}

	if( TRUE == IsMapIndex(STRIGEER_CRYSTAL_MAPINDEX_1ST) )
	{
		CShopInfo* pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}
	}

	if( TRUE == IsMapIndex(STRIGEER_CRYSTAL_MAPINDEX_2ND) )
	{
		CShopInfo* pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		pShopInfo = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX);
		if (pShopInfo)
		{
			pShopInfo->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}
	}

	return TRUE;
}

// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
void CTriggerFunctionCrystal::ResetLocalData()
{
	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	m_pDefaultCrystalGroup->DestroAllyCrystal();
	m_bCreatedCrystal = FALSE;
	m_mtvectTriggerAttackUserDamage.clear();
	m_vectorDestroyedSequence.clear();
}
// end 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용

void CTriggerFunctionCrystal::ResetEventData()
{	
	m_FirstCharacterUniqueNumber = 0;
	m_FirstGuildUniqueNumber = 0;
	m_FirstInfluenceType = INFLUENCE_TYPE_UNKNOWN;
	
	m_WinnerInfluenceType = INFLUENCE_TYPE_UNKNOWN;
}

void CTriggerFunctionCrystal::SetNextCreateTime()
{
	if( m_PeriodTime < STRIGGER_CRYSTAL_NEXT_CREATE_TIME ) 
	{
		m_PeriodTime = STRIGGER_CRYSTAL_NEXT_CREATE_TIME;		// 주기가 STRIGGER_CRYSTAL_NEXT_CREATE_TIME 보다 작다면 STRIGGER_CRYSTAL_NEXT_CREATE_TIME 값으로 설정
	}

	m_NextTickEventCreate = GetTickCount() + (TIMERGAP_TRIGGER_SYSTEM * m_PeriodTime);
	m_NextTickEvent1st = 0;
	m_NextTickEvent2nd = 0;

	if( STRIGEER_CRYSTAL_EVENT_HELL_WARP == m_ProcessEventID || STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP == m_ProcessEventID )
	{
#ifdef _DEBUG
		m_NextTickEvent1st = GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_CREATE_TIME) / 3);			// 20/60 지점
		m_NextTickEvent2nd = GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_CREATE_TIME) * 3 / 4);		// 45/60 지점
#else
		m_NextTickEvent1st = GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_EVENT_1ST));		// 20/60 지점 - 추후 DB 작성
		m_NextTickEvent2nd = GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_EVENT_2ND));		// 45/60 지점 - 추후 DB 작성
#endif
	}
}

void CTriggerFunctionCrystal::FindFirstDamageUser()
{
	BOOL bSkip = FALSE;
	float fMaxDamage = 0.0f;
	vector<UID32_t> vectorCalculatedUser;
	vectorCalculatedUser.clear();

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);

	for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
	{
		float fUserDamage = 0.0f;

		bSkip = FALSE;

		for(int d=0; d<vectorCalculatedUser.size(); d++)
		{
			if( vectorCalculatedUser[d] == m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber )
			{
				bSkip = TRUE;
				break;
			}
		}

		if( TRUE == bSkip ) continue;

		for(int b=0; b<m_mtvectTriggerAttackUserDamage.size(); b++)
		{
			if( m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber == m_mtvectTriggerAttackUserDamage[b].CharacterUniqueNumber )
			{
				for(int c=0; c<m_vectorDestroyedSequence.size(); c++)
				{
					if( m_vectorDestroyedSequence[c] == m_mtvectTriggerAttackUserDamage[b].MonsterIdx )
					{
						fUserDamage += m_mtvectTriggerAttackUserDamage[b].fDamage;
#ifdef _DEBUG
	char szString[256] = { 0, };
	sprintf(szString, "[크리스탈 시스템] 데미지 CharacterUID [%d] MonsterUID[%d] Damage [%10.0f] TotalDamage [%10.0f]", 
		m_mtvectTriggerAttackUserDamage[b].CharacterUniqueNumber, m_mtvectTriggerAttackUserDamage[b].MonsterIdx, m_mtvectTriggerAttackUserDamage[b].fDamage, fUserDamage);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif

						break;
					}
				}
			}
		}

		vectorCalculatedUser.push_back(m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber);

		if( fMaxDamage < fUserDamage )
		{
			fMaxDamage = fUserDamage;

			m_FirstCharacterUniqueNumber = m_mtvectTriggerAttackUserDamage[a].CharacterUniqueNumber;
			m_FirstGuildUniqueNumber = m_mtvectTriggerAttackUserDamage[a].GuildUniqueNumber;
			m_FirstInfluenceType =m_mtvectTriggerAttackUserDamage[a].InfluenceType;
		}
	}

#ifdef _DEBUG
	char szString[256] = { 0, };
	sprintf(szString, "[크리스탈 시스템] 1등 CharacterUID [%d] GuildUID [%d] InfluenceType [%d]", m_FirstCharacterUniqueNumber, m_FirstGuildUniqueNumber, m_FirstInfluenceType);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif
}

void CTriggerFunctionCrystal::FindWinnerInfluence()
{
	BOOL bWinBCU = FALSE;
	float fBCUDamage = 0.0f;
	float fANIDamage = 0.0f;

	mt_auto_lock mta(&m_mtvectTriggerAttackUserDamage);
	
	for(int a=0; a<m_mtvectTriggerAttackUserDamage.size(); a++)
	{
		for(int c=0; c<m_vectorDestroyedSequence.size(); c++)
		{
			if( m_mtvectTriggerAttackUserDamage[a].MonsterIdx == m_vectorDestroyedSequence[c] )
			{
				if( TRUE == IS_VCN_INFLUENCE_TYPE(m_mtvectTriggerAttackUserDamage[a].InfluenceType) )
				{
					fBCUDamage += m_mtvectTriggerAttackUserDamage[a].fDamage;

					break;
				}

				if( TRUE == IS_ANI_INFLUENCE_TYPE(m_mtvectTriggerAttackUserDamage[a].InfluenceType) )
				{
					fANIDamage += m_mtvectTriggerAttackUserDamage[a].fDamage;

					break;
				}
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
		m_WinnerInfluenceType = INFLUENCE_TYPE_VCN;
	}
	else
	{
		m_WinnerInfluenceType = INFLUENCE_TYPE_ANI;		
	}

#ifdef _DEBUG
	char szString[256] = { 0, };
	sprintf(szString, "[크리스탈 시스템] 이긴 세력 [%d] BCU 데미지 [%10.0f] ANI 데미지 [%10.0f]", m_WinnerInfluenceType, fBCUDamage, fANIDamage);
	m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_USER_NOTICE, szString);		
#endif
}

void CTriggerFunctionCrystal::RunEvent(EventID_t EventID)
{
	char szSendString[256] = { 0, };
	char szCharacterName[SIZE_MAX_CHARACTER_NAME] = { 0, };

	CFieldIOCPSocket *pFISock = m_pFieldMapChannel->m_pFieldIOCPMapChannel->GetFieldIOCPSocketByCharacterUniqueNumber(m_FirstCharacterUniqueNumber);
	if( pFISock && pFISock->IsValidCharacter(FALSE) )
	{
		util::strncpy(szCharacterName, pFISock->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	}

	m_ProcessEventID = EventID;

	if( EventID == STRIGEER_CRYSTAL_EVENT_HELL_WARP ||				// 1차 이벤트 - (출력 메시지에 세력이 나와야하는 경우 등록)
		EventID == STRIGGER_CRYSTAL_EVENT_EXP_100 || 
		EventID == STRIGGER_CRYSTAL_EVENT_DROP_100 ||
		EventID == STRIGGER_CRYSTAL_EVENT_EXP_SPI_DROP_100 ||

		EventID == STRIGGER_CRYSTAL_EVENT_2ND_WARPOINT ||			// 2차 이벤트 - (출력 메시지에 세력이 나와야하는 경우 등록)
		EventID == STRIGGER_CRYSTAL_EVENT_2ND_EXP_150 ||
		EventID == STRIGGER_CRYSTAL_EVENT_2ND_DROP_150 ||
		EventID == STRIGGER_CRYSTAL_EVENT_2ND_EXP_SPI_DROP_150 ||
		EventID == STRIGGER_CRYSTAL_EVENT_2ND_CAPSULE_ITEM ||
		EventID == STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP )
	{
		m_FirstCharacterUniqueNumber = 0;
		m_FirstGuildUniqueNumber = 0;
		m_FirstInfluenceType = INFLUENCE_TYPE_UNKNOWN;		
	}
	else
	{
		m_WinnerInfluenceType = INFLUENCE_TYPE_UNKNOWN;
	}
	
	switch( EventID )
	{

	// 1차 이벤트
		
	case STRIGEER_CRYSTAL_EVENT_HELL_WARP:

		EventHellWarp(TRUE);

		sprintf(szSendString, STRMSG_120330_0001, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);

		break;
		
	case STRIGGER_CRYSTAL_EVENT_SUMMON_BOSS_MONSTER:

		EventSummonBossMonster(TRUE);

		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120330_0008, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;
		
	case STRIGGER_CRYSTAL_EVENT_SHOP_KIT:

		EventShopKit(TRUE);

		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120330_0007, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;

	case STRIGGER_CRYSTAL_EVENT_EXP_100:

		EventExp100(TRUE);

		sprintf(szSendString, STRMSG_120330_0002, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);

		break;
		
	case STRIGGER_CRYSTAL_EVENT_SHOP_HYPER_CARD:

		EventShopHyperCard(TRUE);

		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120330_0006, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;
		
	case STRIGGER_CRYSTAL_EVENT_SHOP_SEAL:

		EventShopSeal(TRUE);

		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120330_0005, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;
		
	case STRIGGER_CRYSTAL_EVENT_DROP_100:

		EventDrop100(TRUE);

		sprintf(szSendString, STRMSG_120330_0003, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);

		break;

	case STRIGGER_CRYSTAL_EVENT_EXP_SPI_DROP_100:

		EventExpSPIDrop100(TRUE);

		sprintf(szSendString, STRMSG_120330_0004, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);

		break;				

	// 2차 이벤트

	case STRIGGER_CRYSTAL_EVENT_2ND_WARPOINT:

		Event2NDWarPoint(TRUE);

		sprintf(szSendString, STRMSG_120508_0001, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType), STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_WAYPOINT);
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
	
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_SUMMON_BOSS_MONSTER:

		Event2NDSummonBossMonster(TRUE);
		
		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120508_0002, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_EXP_150:

		Event2NDExp150(TRUE);

		sprintf(szSendString, STRMSG_120508_0003, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_DROP_150:

		Event2NDDrop150(TRUE);

		sprintf(szSendString, STRMSG_120508_0004, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_EXP_SPI_DROP_150:

		Event2NDExpSPIDrop150(TRUE);

		sprintf(szSendString, STRMSG_120508_0005, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_CAPSULE_ITEM:

		Event2NDCapsuleItem(TRUE);

		sprintf(szSendString, STRMSG_120508_0006, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_PREFIX_SUFFIX:

		Event2NDShopPrefixSuffix(TRUE);
		
		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120508_0007, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_KIT:

		Event2NDShopKit(TRUE);
		
		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120508_0008, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_HYPER_CARD:

		Event2NDShopHyperCard(TRUE);
		
		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120508_0009, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_WEAPON:

		Event2NDShopWepone(TRUE);
		
		if( 0 != strlen(szCharacterName) )
		{
			sprintf(szSendString, STRMSG_120508_0010, szCharacterName);
			m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		}

		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP:
		
		Event2NDHellWarp(TRUE);

		sprintf(szSendString, STRMSG_120508_0011, CAtumSJ::GetInfluenceTypeString(m_WinnerInfluenceType));
		m_pFieldMapChannel->SendString128ToAllInChannel(STRING_128_SYSTEM_NOTICE, szSendString);
		
		break;		

	default :
		break;
	}
}

void CTriggerFunctionCrystal::StepEvent(EventID_t EventID, INT EventOption /*= EVENT_OPTION_NONE*/)
{	
	switch( EventID )
	{
	// 1차 이벤트

	case STRIGEER_CRYSTAL_EVENT_HELL_WARP:
		EventHellWarp(FALSE, EventOption);
		break;

	// 2차 이벤트

	case STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP:
		Event2NDHellWarp(FALSE, EventOption);
		break;

	default :
		break;
	}
}

void CTriggerFunctionCrystal::StopEvent(EventID_t EventID)
{	
	SetAllShopRemainCountZero();

	switch( EventID )
	{

	// 1차 이벤트

	case STRIGEER_CRYSTAL_EVENT_HELL_WARP:
		EventHellWarp(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_SUMMON_BOSS_MONSTER:
		EventSummonBossMonster(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_SHOP_KIT:
		EventShopKit(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_EXP_100:
		EventExp100(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_SHOP_HYPER_CARD:
		EventShopHyperCard(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_SHOP_SEAL:
		EventShopSeal(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_DROP_100:
		EventDrop100(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_EXP_SPI_DROP_100:
		EventExpSPIDrop100(FALSE);
		break;				


	// 2차 이벤트

	case STRIGGER_CRYSTAL_EVENT_2ND_WARPOINT:
		Event2NDWarPoint(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_SUMMON_BOSS_MONSTER:
		Event2NDSummonBossMonster(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_EXP_150:
		Event2NDExp150(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_DROP_150:
		Event2NDDrop150(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_EXP_SPI_DROP_150:
		Event2NDExpSPIDrop150(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_CAPSULE_ITEM:
		Event2NDCapsuleItem(FALSE);
		break;
		
	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_PREFIX_SUFFIX:
		Event2NDShopPrefixSuffix(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_KIT:
		Event2NDShopKit(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_HYPER_CARD:
		Event2NDShopHyperCard(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_SHOP_WEAPON:
		Event2NDShopWepone(FALSE);
		break;

	case STRIGGER_CRYSTAL_EVENT_2ND_HELL_WARP:
		Event2NDHellWarp(FALSE);
		break;				

	default :
		break;
	}

	m_ProcessEventID = STRIGEER_CRYSTAL_EVENT_NONE;
}

void CTriggerFunctionCrystal::MixEvent()
{
	EventID_t nEventID = 0;
	int nTriggerCrystalCount = m_vectorTriggerCrystal.size();
	int nRandomCount = m_vectorTriggerCrystal.size() * 5;

	for(int i=0; i<nRandomCount; i++)
	{
		int nFirst = RANDI(0, nTriggerCrystalCount - 1);
		int nSecond = RANDI(0, nTriggerCrystalCount - 1);

		if( nFirst == nSecond ) continue;

		nEventID = m_vectorTriggerCrystal[nFirst]->GetEventID();
		m_vectorTriggerCrystal[nFirst]->SetEventID(m_vectorTriggerCrystal[nSecond]->GetEventID());
		m_vectorTriggerCrystal[nSecond]->SetEventID(nEventID);
	}
}

void CTriggerFunctionCrystal::EventHellWarp(BOOL bStart, INT EventOption /*= EVENT_OPTION_NONE*/)
{
	switch( EventOption )
	{
	case EVENT_OPTION_NONE :

		if( TRUE == bStart )
		{
			// 워프 생성

			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_NUM_CHANGE);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_NUM_CHANGE);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_NUM_CHANGE);

			// 몬스터 생성

			if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject && NULL != m_pFieldMapChannel->m_pFieldMapWorkspace )
			{
				CFieldMapProject *pFMPro = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_1ST);

				if(NULL != pFMPro)
				{
					int m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();

					pFMPro->ResetEnterOneTimeLimited();
					pFMPro->SetEnterOneTimeLimited(TRUE);										// 입장 1회 제한 설정

					//////////////////////////////////////////////////////////////////////
					// 세력 입장으로 변경 (기획 변경)
					//////////
					// 기존
					/*	
					if( 0 == m_FirstGuildUniqueNumber )
					{
						pFMPro->SetEnterGuildLimited(FALSE);
						pFMPro->SetEnterCharacterLimited(TRUE, m_FirstCharacterUniqueNumber);	// 이캐릭만 들어갈 수 있도록 설정
					}
					else
					{
						pFMPro->SetEnterGuildLimited(TRUE, m_FirstGuildUniqueNumber);			// 소속 길드만 들어 갈수 있도록 설정
						pFMPro->SetEnterCharacterLimited(FALSE);
					}
					*/

					//////////
					// 변경
					pFMPro->SetEnterGuildLimited(FALSE);
					pFMPro->SetEnterCharacterLimited(FALSE);
					pFMPro->SetEnterInfluenceLimited(TRUE, m_WinnerInfluenceType);				// 소속 세력만 들어 갈수 있도록 설정
					//////////////////////////////////////////////////////////////////////

					pFMPro->SetEnterMaxiumUserLimited(TRUE);									// 유저 최대 숫자 제한

					for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
					{
						CFieldMapChannel *pTargetFieldMapChannel = pFMPro->GetFieldMapChannelByIndex(ChannelCount);

						if( NULL != pTargetFieldMapChannel )
						{
							//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);		// 혹시나 버그로 유저가 있다면 백맵으로 워프
							pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

							// 새로운 상태 설정

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_01, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_02, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_03, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_04, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_05, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_06, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_07, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_08, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_09, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);

#ifdef _DEBUG
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_CREATE_TIME) * 1 / 2), STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_REWARD_ITEM_UID);
#else
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + (TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_EVENT_2ND), STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_REWARD_ITEM_UID);
#endif
						}
					}
				}

				CFieldMapProject *pFMPro2nd = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_2ND);

				if(NULL != pFMPro2nd)
				{
					int m_MaxChannelPerMap = pFMPro2nd->GetNumberOfMapChannels();
					
					pFMPro2nd->ResetEnterOneTimeLimited();
					pFMPro2nd->SetEnterOneTimeLimited(TRUE);										// 입장 1회 제한 설정
					
					//////////////////////////////////////////////////////////////////////
					// 세력 입장으로 변경 (기획 변경)
					//////////
					// 기존
					/*	
					if( 0 == m_FirstGuildUniqueNumber )
					{
						pFMPro2nd->SetEnterGuildLimited(FALSE);
						pFMPro2nd->SetEnterCharacterLimited(TRUE, m_FirstCharacterUniqueNumber);	// 이캐릭만 들어갈 수 있도록 설정
					}
					else
					{
						pFMPro2nd->SetEnterGuildLimited(TRUE, m_FirstGuildUniqueNumber);			// 소속 길드만 들어 갈수 있도록 설정
						pFMPro2nd->SetEnterCharacterLimited(FALSE);
					}
					*/
					//////////
					// 변경
					pFMPro2nd->SetEnterGuildLimited(FALSE);
					pFMPro2nd->SetEnterCharacterLimited(FALSE);
					pFMPro2nd->SetEnterInfluenceLimited(TRUE, m_WinnerInfluenceType);					// 소속 세력만 들어 갈수 있도록 설정
					//////////////////////////////////////////////////////////////////////

					pFMPro2nd->SetEnterMaxiumUserLimited(TRUE);										// 유저 최대 숫자 제한

					for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
					{
						CFieldMapChannel *pTargetFieldMapChannel = pFMPro2nd->GetFieldMapChannelByIndex(ChannelCount);

						if( NULL != pTargetFieldMapChannel )
						{						
							//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);		// 혹시나 버그로 유저가 있다면 백맵으로 워프
							pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정
							// 새로운 상태 설정

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_01, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_02, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_03, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_04, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_05, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_06, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_07, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_08, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_09, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							
#ifdef _DEBUG
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_CREATE_TIME) * 1 / 2), 0);
#else
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + (TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_EVENT_2ND), 0);
#endif
						}
					}
				}
			}
		}
		
		break;

	case EVENT_OPTION_HELL_WARP_CLOSE :

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_WARP_OBJECT_NUM_DEFAULT);
		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_WARP2_OBJECT_NUM_DEFAULT);
		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_WARP3_OBJECT_NUM_DEFAULT);

		break;

	case EVENT_OPTION_HELL_BACK_MAP_WARP :

		if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject && NULL != m_pFieldMapChannel->m_pFieldMapWorkspace )
		{
			CFieldMapProject *pFMPro = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_1ST);

			if(NULL != pFMPro)
			{
				int m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();

				pFMPro->SetEnterCharacterLimited(TRUE, 0);		// 아무도 못들어가도록 설정

				for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
				{
					CFieldMapChannel *pTargetFieldMapChannel = pFMPro->GetFieldMapChannelByIndex(ChannelCount);

					if( NULL != pTargetFieldMapChannel )
					{
						//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);
						pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

						// 기존 상태 초기화

						pTargetFieldMapChannel->ClearAllMonsterDeadItemPay();

						if( NULL != m_pMapTriggerManager && NULL != m_pMapTriggerManager->GetFieldIOCP() )
						{
							INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf2);
							pMonster->mapChann = pTargetFieldMapChannel->GetMapChannelIndex();
							pMonster->bAllFlag = TRUE;
							((CFieldIOCP*)(m_pMapTriggerManager->GetFieldIOCP()))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));
						}
						
						pTargetFieldMapChannel->ResetDropItemMapChannel();	
					}
				}			
			}

			CFieldMapProject *pFMPro2nd = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_INDEX_2ND);

			if(NULL != pFMPro2nd)
			{
				int m_MaxChannelPerMap = pFMPro2nd->GetNumberOfMapChannels();

				pFMPro2nd->SetEnterCharacterLimited(TRUE, 0);	// 아무도 못들어가도록 설정

				for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
				{
					CFieldMapChannel *pTargetFieldMapChannel = pFMPro2nd->GetFieldMapChannelByIndex(ChannelCount);

					if( NULL != pTargetFieldMapChannel )
					{						
						//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);
						pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

						// 기존 상태 초기화

						pTargetFieldMapChannel->ClearAllMonsterDeadItemPay();
						
						if( NULL != m_pMapTriggerManager && NULL != m_pMapTriggerManager->GetFieldIOCP() )
						{
							INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf2);
							pMonster->mapChann = pTargetFieldMapChannel->GetMapChannelIndex();
							pMonster->bAllFlag = TRUE;
							((CFieldIOCP*)(m_pMapTriggerManager->GetFieldIOCP()))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));
						}
						
						pTargetFieldMapChannel->ResetDropItemMapChannel();
					}
				}
			}
		}

		break;

	default:
		break;
	}
}

void CTriggerFunctionCrystal::EventSummonBossMonster(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( NULL != m_pFieldMapChannel )
		{
			INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pMsgSummonMonster, SendBuf);
			pMsgSummonMonster->ChannelIndex			= m_pFieldMapChannel->GetMapChannelIndex().ChannelIndex;
			util::strncpy(pMsgSummonMonster->CharacterName, "CrystalEventBossMonster", SIZE_MAX_CHARACTER_NAME);
			pMsgSummonMonster->MonsterUnitKind		= STRIGGER_CRYSTAL_EVENT_VALUE_SUMMON_BOSS_MONSTER_UID;
			pMsgSummonMonster->MonsterTargetType1	= MONSTER_TARGETTYPE_NORMAL;
			pMsgSummonMonster->TargetTypeData1		= 0;
			pMsgSummonMonster->CltIdxForTargetType1	= 0;
			pMsgSummonMonster->NumOfMonster			= 1;
			pMsgSummonMonster->Position				= AVECTOR3(0, 0, 0);
			pMsgSummonMonster->MonsterBalanceData.Init(0);
			m_pFieldMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
		}
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::EventShopKit(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}

		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SALES_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::EventExp100(BOOL bStart)
{
	if( TRUE == bStart )
	{
		// 2013-01-14 by bckim, 애쉬레인 크리스탈 파괴시, 이긴 세력의 모든 맵에 해피아워가 적용되는 버그 수정(해당맵에 있는 세력에게만 적용되어야 함. 인자 추가:m_TriggerInfo.MapIndex)
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_EXP, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM,m_TriggerInfo.MapIndex);

		SendNotifyHappyHourEvent(STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_EXP, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::EventShopHyperCard(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}

		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SALES_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::EventShopSeal(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}
 
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SALES_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);	
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::EventDrop100(BOOL bStart)
{
	if( TRUE == bStart )
	{
		// 2013-01-14 by bckim, 애쉬레인 크리스탈 파괴시, 이긴 세력의 모든 맵에 해피아워가 적용되는 버그 수정(해당맵에 있는 세력에게만 적용되어야 함. 인자 추가:m_TriggerInfo.MapIndex)
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM,m_TriggerInfo.MapIndex);

		SendNotifyHappyHourEvent(0.0f, 0.0f, 0.0f, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_DROP_ITEM, 0.0f, 0.0f);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::EventExpSPIDrop100(BOOL bStart)
{
	if( TRUE == bStart )
	{
		// 2013-01-14 by bckim, 애쉬레인 크리스탈 파괴시, 이긴 세력의 모든 맵에 해피아워가 적용되는 버그 수정(해당맵에 있는 세력에게만 적용되어야 함. 인자 추가:m_TriggerInfo.MapIndex)
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_EXP, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM,m_TriggerInfo.MapIndex);
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_SPI, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_SPI, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM,m_TriggerInfo.MapIndex);
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM,m_TriggerInfo.MapIndex);

		SendNotifyHappyHourEvent(STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_EXP, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_SPI, 0.0f, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_ADD_DROP_ITEM, 0.0f, 0.0f);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDWarPoint(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( NULL != m_pFieldMapChannel )
		{
			m_pFieldMapChannel->AddWarPointInMap(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_WAYPOINT, TRUE, m_WinnerInfluenceType, WPUT_TRIGGER);
		}
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDSummonBossMonster(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( NULL != m_pFieldMapChannel )
		{
			unsigned int MonsterNumber[STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_TOTAL_COUNT] = 
			{ 
				STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_01,
				STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_02,
				STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_03,
				STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_04,
				STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_05,
				STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_UID_06
			};

			for(int i=0; i<STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_TOTAL_COUNT; i++)
			{
				m_pFieldMapChannel->CreateMonster(MonsterNumber[i], STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SUMMON_BOSS_MONSTER_RANDOM_POS);
			}
		}
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDExp150(BOOL bStart)
{
	if( TRUE == bStart )
	{
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_EXP, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND, m_TriggerInfo.MapIndex);		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - 0 이면 적용 하지 않음

		SendNotifyHappyHourEvent2ND(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_EXP, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDDrop150(BOOL bStart)
{
	if( TRUE == bStart )
	{
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND, m_TriggerInfo.MapIndex);		// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - 0 이면 적용 하지 않음

		SendNotifyHappyHourEvent2ND(0.0f, 0.0f, 0.0f, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_DROP_ITEM, 0.0f, 0.0f);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDExpSPIDrop150(BOOL bStart)
{
	if( TRUE == bStart )
	{
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_EXP, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_EXP, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND, m_TriggerInfo.MapIndex);				// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - 0 이면 적용 하지 않음
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_SPI, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_SPI, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND, m_TriggerInfo.MapIndex);				// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - 0 이면 적용 하지 않음
		m_pFieldMapChannel->m_pFieldIOCPMapChannel->m_gameEventManager.StartGameEvent(GAME_EVENT_TYPE_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_DROP_ITEM, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME, 1, CHARACTER_MAX_LEVEL, m_WinnerInfluenceType, GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND, m_TriggerInfo.MapIndex);	// 2012-06-16 by hskim, [트리거 시스템] - 크리스탈 시스템 2차 : 특정 맵에서만 해피아워 이벤트 적용 - 0 이면 적용 하지 않음

		SendNotifyHappyHourEvent2ND(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_EXP, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_SPI, 0.0f, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_ADD_DROP_ITEM, 0.0f, 0.0f);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDCapsuleItem(BOOL bStart)
{
	if( TRUE == bStart )
	{
		m_pFieldMapChannel->InsertItemInMap(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_CAPSULE_ITEM_UID, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WINNER_CAPSULE_ITEM_COUNT, m_WinnerInfluenceType);
	}
	else
	{
	}
}

void CTriggerFunctionCrystal::Event2NDShopPrefixSuffix(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}

		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_PREFIX_SUFFIX_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_2ND_VALUE_PREFIX_SUFFIX_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::Event2NDShopKit(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}

		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_KIT_SALES_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_2ND_VALUE_KIT_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::Event2NDShopHyperCard(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}

		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SALES_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HYPER_CARD_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::Event2NDShopWepone(BOOL bStart)
{
	if( TRUE == bStart )
	{
		if( 0 == m_FirstCharacterUniqueNumber )
		{
			return ;
		}

		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX);
		if (!pShop)
		{
			return ;
		}

		pShop->SetShopOwner(m_FirstCharacterUniqueNumber);
		pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_VALUE_SEAL_SALES_VOLUME);

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_NUM_CHANGE);
	}
	else
	{
		CShopInfo* pShop = m_pFieldMapChannel->m_pFieldMapProject->GetShopInfoByBuildingNPC(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX);
		if (pShop)
		{
			pShop->SetShopOwner(0);
			pShop->SetAllRemainCountForLimitedEdition(STRIGGER_CRYSTAL_EVENT_SHOP_INIT_VOLUME);
		}

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WEAPON_SHOP_OBJECT_NUM_DEFAULT);

		INIT_MSG_WITH_BUFFER(MSG_FC_SHOP_CLOSE, T_FC_SHOP_CLOSE, pClientShopMsg, ClientShopSendBuf);
		pClientShopMsg->BuildingIndex = STRIGGER_CRYSTAL_EVENT_2ND_VALUE_SEAL_SHOP_ITEM_BUILDING_INDEX;
		m_pFieldMapChannel->SendMessageToAllInChannel(ClientShopSendBuf, MSG_SIZE(MSG_FC_SHOP_CLOSE));
	}
}

void CTriggerFunctionCrystal::Event2NDHellWarp(BOOL bStart, INT EventOption /*= EVENT_OPTION_NONE*/)
{
	switch( EventOption )
	{
	case EVENT_OPTION_NONE :

		if( TRUE == bStart )
		{
			// 워프 생성

			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_NUM_CHANGE);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_NUM_CHANGE);
			m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_NUM_CHANGE);

			// 몬스터 생성

			if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject && NULL != m_pFieldMapChannel->m_pFieldMapWorkspace )
			{
				CFieldMapProject *pFMPro = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_1ST);

				if(NULL != pFMPro)
				{
					int m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();

					pFMPro->ResetEnterOneTimeLimited();
					pFMPro->SetEnterOneTimeLimited(TRUE);										// 입장 1회 제한 설정

					//////////////////////////////////////////////////////////////////////
					// 세력 입장으로 변경 (기획 변경)
					//////////
					// 기존
					/*	
					if( 0 == m_FirstGuildUniqueNumber )
					{
						pFMPro->SetEnterGuildLimited(FALSE);
						pFMPro->SetEnterCharacterLimited(TRUE, m_FirstCharacterUniqueNumber);	// 이캐릭만 들어갈 수 있도록 설정
					}
					else
					{
						pFMPro->SetEnterGuildLimited(TRUE, m_FirstGuildUniqueNumber);			// 소속 길드만 들어 갈수 있도록 설정
						pFMPro->SetEnterCharacterLimited(FALSE);
					}
					*/

					//////////
					// 변경
					pFMPro->SetEnterGuildLimited(FALSE);
					pFMPro->SetEnterCharacterLimited(FALSE);
					pFMPro->SetEnterInfluenceLimited(TRUE, m_WinnerInfluenceType);				// 소속 세력만 들어 갈수 있도록 설정
					//////////////////////////////////////////////////////////////////////

					pFMPro->SetEnterMaxiumUserLimited(TRUE);									// 유저 최대 숫자 제한

					for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
					{
						CFieldMapChannel *pTargetFieldMapChannel = pFMPro->GetFieldMapChannelByIndex(ChannelCount);

						if( NULL != pTargetFieldMapChannel )
						{
							//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);		// 혹시나 버그로 유저가 있다면 백맵으로 워프
							pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

							// 새로운 상태 설정

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_01, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_02, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_03, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_04, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_05, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_06, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_07, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_08, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_MONSTER_UID_09, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);

#ifdef _DEBUG
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_CREATE_TIME) * 1 / 2), STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_REWARD_ITEM_UID);
#else
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + (TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_EVENT_2ND), STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_1ST_REWARD_ITEM_UID);
#endif
						}
					}
				}

				CFieldMapProject *pFMPro2nd = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_2ND);

				if(NULL != pFMPro2nd)
				{
					int m_MaxChannelPerMap = pFMPro2nd->GetNumberOfMapChannels();
					
					pFMPro2nd->ResetEnterOneTimeLimited();
					pFMPro2nd->SetEnterOneTimeLimited(TRUE);										// 입장 1회 제한 설정
					
					//////////////////////////////////////////////////////////////////////
					// 세력 입장으로 변경 (기획 변경)
					//////////
					// 기존
					/*	
					if( 0 == m_FirstGuildUniqueNumber )
					{
						pFMPro2nd->SetEnterGuildLimited(FALSE);
						pFMPro2nd->SetEnterCharacterLimited(TRUE, m_FirstCharacterUniqueNumber);	// 이캐릭만 들어갈 수 있도록 설정
					}
					else
					{
						pFMPro2nd->SetEnterGuildLimited(TRUE, m_FirstGuildUniqueNumber);			// 소속 길드만 들어 갈수 있도록 설정
						pFMPro2nd->SetEnterCharacterLimited(FALSE);
					}
					*/
					//////////
					// 변경
					pFMPro2nd->SetEnterGuildLimited(FALSE);
					pFMPro2nd->SetEnterCharacterLimited(FALSE);
					pFMPro2nd->SetEnterInfluenceLimited(TRUE, m_WinnerInfluenceType);					// 소속 세력만 들어 갈수 있도록 설정
					//////////////////////////////////////////////////////////////////////

					pFMPro2nd->SetEnterMaxiumUserLimited(TRUE);										// 유저 최대 숫자 제한

					for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
					{
						CFieldMapChannel *pTargetFieldMapChannel = pFMPro2nd->GetFieldMapChannelByIndex(ChannelCount);

						if( NULL != pTargetFieldMapChannel )
						{						
							//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);		// 혹시나 버그로 유저가 있다면 백맵으로 워프
							pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

							// 새로운 상태 설정

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_01, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_02, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_03, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_04, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_05, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);

							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_06, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_07, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_08, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							pTargetFieldMapChannel->CreateMonster(STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_MONSTER_UID_09, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_2ND_SUMMON_BOSS_MONSTER_COUNT, STRIGGER_CRYSTAL_EVENT_VALUE_HELL_MAP_SUMMON_MONSTER_RANDOM_POS);
							
#ifdef _DEBUG
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + ((TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_CREATE_TIME) * 1 / 2), 0);
#else
							pTargetFieldMapChannel->SetAllMonsterDeadItemPay(GetTickCount() + (TIMERGAP_TRIGGER_SYSTEM * STRIGGER_CRYSTAL_NEXT_EVENT_2ND), 0);
#endif
						}
					}
				}
			}
		}
		
		break;

	case EVENT_OPTION_HELL_WARP_CLOSE :

		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP_OBJECT_NUM_DEFAULT);
		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP2_OBJECT_NUM_DEFAULT);
		m_pFieldMapChannel->ChangeObject(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_IDX, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_WARP3_OBJECT_NUM_DEFAULT);

		break;

	case EVENT_OPTION_HELL_BACK_MAP_WARP :

		if( NULL != m_pFieldMapChannel && NULL != m_pFieldMapChannel->m_pFieldMapProject && NULL != m_pFieldMapChannel->m_pFieldMapWorkspace )
		{
			CFieldMapProject *pFMPro = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_1ST);

			if(NULL != pFMPro)
			{
				int m_MaxChannelPerMap = pFMPro->GetNumberOfMapChannels();

				pFMPro->SetEnterCharacterLimited(TRUE, 0);		// 아무도 못들어가도록 설정

				for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
				{
					CFieldMapChannel *pTargetFieldMapChannel = pFMPro->GetFieldMapChannelByIndex(ChannelCount);

					if( NULL != pTargetFieldMapChannel )
					{
						//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);
						pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

						// 기존 상태 초기화

						pTargetFieldMapChannel->ClearAllMonsterDeadItemPay();

						if( NULL != m_pMapTriggerManager && NULL != m_pMapTriggerManager->GetFieldIOCP() )
						{
							INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf2);
							pMonster->mapChann = pTargetFieldMapChannel->GetMapChannelIndex();
							pMonster->bAllFlag = TRUE;
							((CFieldIOCP*)(m_pMapTriggerManager->GetFieldIOCP()))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));
						}
						
						pTargetFieldMapChannel->ResetDropItemMapChannel();	
					}
				}			
			}

			CFieldMapProject *pFMPro2nd = (CFieldMapProject *)m_pFieldMapChannel->m_pFieldMapWorkspace->GetMapProjectByMapIndex(STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HELL_MAP_INDEX_2ND);

			if(NULL != pFMPro2nd)
			{
				int m_MaxChannelPerMap = pFMPro2nd->GetNumberOfMapChannels();

				pFMPro2nd->SetEnterCharacterLimited(TRUE, 0);	// 아무도 못들어가도록 설정

				for(int ChannelCount = 0; ChannelCount < m_MaxChannelPerMap; ChannelCount++)
				{
					CFieldMapChannel *pTargetFieldMapChannel = pFMPro2nd->GetFieldMapChannelByIndex(ChannelCount);

					if( NULL != pTargetFieldMapChannel )
					{						
						//pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0);
						pTargetFieldMapChannel->WarpToCityMapFieldMapChannel(pTargetFieldMapChannel->m_pFieldMapProject->m_nBeforeMapIndex, 0, pTargetFieldMapChannel->m_pFieldMapProject->m_nMapIndex);  // 2013-03-21 by bckim, 지옥맵 부활카드 사용시 버그 수정

						// 기존 상태 초기화

						pTargetFieldMapChannel->ClearAllMonsterDeadItemPay();
						
						if( NULL != m_pMapTriggerManager && NULL != m_pMapTriggerManager->GetFieldIOCP() )
						{
							INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pMonster, SendBuf2);
							pMonster->mapChann = pTargetFieldMapChannel->GetMapChannelIndex();
							pMonster->bAllFlag = TRUE;
							((CFieldIOCP*)(m_pMapTriggerManager->GetFieldIOCP()))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL));
						}
						
						pTargetFieldMapChannel->ResetDropItemMapChannel();
					}
				}
			}
		}

		break;

	default:
		break;
	}
}

void CTriggerFunctionCrystal::SendNotifyHappyHourEvent(float fEXPRate2, float fSPIRate2, float fEXPRepairRate2, float fDropItemRate2, float fDropRareRate2, float fWarPointRate2)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
	pSHappy->byStateType		= GAME_EVENT_GROUP_CRYSTAL_SYSTEM_START;
	pSHappy->byInfluenceType4	= m_WinnerInfluenceType;
	pSHappy->StartATime.SetCurrentDateTime(TRUE);
	pSHappy->EndATime			= pSHappy->StartATime;
	pSHappy->EndATime.AddDateTime(0, 0, 0, 0, STRIGGER_CRYSTAL_EVENT_VALUE_HOUR_EVENT_APPLY_TIME);
	pSHappy->fEXPRate2			= fEXPRate2;
	pSHappy->fSPIRate2			= fSPIRate2;
	pSHappy->fEXPRepairRate2	= fEXPRepairRate2;
	pSHappy->fDropItemRate2		= fDropItemRate2;
	pSHappy->fDropRareRate2		= fDropRareRate2;
	pSHappy->fWarPointRate2		= fWarPointRate2;
	m_pFieldMapChannel->m_pFieldIOCPMapChannel->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pSHappy->byInfluenceType4);
}

void CTriggerFunctionCrystal::SendNotifyHappyHourEvent2ND(float fEXPRate2, float fSPIRate2, float fEXPRepairRate2, float fDropItemRate2, float fDropRareRate2, float fWarPointRate2)
{
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, T_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK, pSHappy, SendBuf);
	pSHappy->byStateType		= GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND_START;
	pSHappy->byInfluenceType4	= m_WinnerInfluenceType;
	pSHappy->StartATime.SetCurrentDateTime(TRUE);
	pSHappy->EndATime			= pSHappy->StartATime;
	pSHappy->EndATime.AddDateTime(0, 0, 0, 0, STRIGGER_CRYSTAL_EVENT_2ND_VALUE_HOUR_EVENT_APPLY_TIME);
	pSHappy->fEXPRate2			= fEXPRate2;
	pSHappy->fSPIRate2			= fSPIRate2;
	pSHappy->fEXPRepairRate2	= fEXPRepairRate2;
	pSHappy->fDropItemRate2		= fDropItemRate2;
	pSHappy->fDropRareRate2		= fDropRareRate2;
	pSHappy->fWarPointRate2		= fWarPointRate2;
	m_pFieldMapChannel->m_pFieldIOCPMapChannel->SendMessageToAllClients(SendBuf, MSG_SIZE(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK), pSHappy->byInfluenceType4);
}
