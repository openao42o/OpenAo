// UnitData.cpp: implementation of the CUnitData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitData.h"
#include "StoreData.h"
#include "EnemyData.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "SceneData.h"
#include "WeaponFireData.h"
#include "WeaponFastData.h"
#include "UnitRender.h"
#include "CharacterRender.h"					// 2005-07-21 by ispark
#include "AtumSound.h"
#include "AtumDatabase.h"
#include "WeaponRocketData.h"
#include "WeaponMissileData.h"
#include "ItemInfo.h"
#include "WeaponAllAttackData.h"
#include "dxutil.h"
#include "Skill.h"
#include "SkillEffect.h"
#include "WeaponItemInfo.h"
#include "Camera.h"
#include "MonsterData.h"
#include "ItemData.h"
#include "PetManager.h"	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 데이터를 가져오는 함수
	

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CREATE_PRIMARY_SINGLE_WEAPON(p1, p2) \
	(p1 ? new CWeaponFastData(this,pTarget,p1) :	\
		  new CWeaponFastData(this,pTarget,p2) )
#define CREATE_PRIMARY_DOUBLE_WEAPON(p1, p2, i) \
	(p1 ? new CWeaponFastData(this,pTarget,i,p1) :	\
		  new CWeaponFastData(this,pTarget,i,p2) )
#define CREATE_PRIMARY_FIRE_WEAPON_BY_ITEM_INDEX(p1, p2) \
	(p1 ? new CWeaponFireData(this,p1->WeaponItemNumber) :	\
		  new CWeaponFireData(this,p2->WeaponItemNumber) )
#define CREATE_SECONDARY_WEAPON_ROCKET(p1, p2) \
	(p1 ? new CWeaponRocketData(this,pTarget,p1) :	\
		  new CWeaponRocketData(this,pTarget,p2) )
#define CREATE_SECONDARY_WEAPON_MISSILE(p1, p2) \
	(p1 ? new CWeaponMissileData(this,pTarget,p1) :	\
		  new CWeaponMissileData(this,pTarget,p2) )
#define MSG_PRIMARY_ATTACKINDEX(p1,p2)		((p1) ? p1->AttackIndex : p2->AttackIndex)
#define MSG_PRIMARY_WEAPONITEMNUMBER(p1,p2)		((p1) ? p1->WeaponItemNumber : p2->WeaponItemNumber)
#define MSG_SECONDARY_ATTACKINDEX(p1,p2)		((p1) ? p1->AttackIndex : p2->AttackIndex)
#define MSG_SECONDARY_WEAPONITEMNUMBER(p1,p2)		((p1) ? p1->WeaponItemNumber : p2->WeaponItemNumber)



// 부스터 바디 컨디션 세팅 방법
// BODYCON_BOOSTER1_MASK : 키를 안누르고 비행(평균속도 이하, 속도증가키 안누를때)
// BODYCON_BOOSTER2_MASK : 전진키 누르는 상태(평균속도 이상, 속도증가키 누를때)
// BODYCON_BOOSTER3_MASK : 고속 부스터
// BODYCON_BOOSTER4_MASK : 고속을 끌때 
// BODYCON_BOOSTER5_MASK : 역추진
// BODYCON_BOOSTER_OFF_MASK : 부스터 끔

#define BODYCON_BOOSTER_OFF				(BodyCond_t)0x0000000000000004// 화면에 나타나는 이펙트의 소멸

// 서버에서는 부스터 bodycon이 발동중일때 BODYCON_FLY_MASK을 같이 발동한다.
// 하지만, 클라이언트는 BODYCON_FLY_MASK bodycon이 없다.
// 서버에서 BODYCON_FLY_MASK bodycon이 오면 부스터 bodycon을 확인한다.

CUnitData::CUnitData()
{
	FLOG("CUnitData::CUnitData()");
	ChangeUnitState( _NORMAL );
//	m_bShootingPrimaryWeapon = FALSE;
//	m_fAutoMaticTimer = 0.0f;
//	m_bZigZagWeapon = FALSE;
//	m_vPrimarySidePos = D3DXVECTOR3(0,0,0);
//	m_vPrimarySideBackPos = D3DXVECTOR3(0,0,0);
//	m_vSecondarySidePos = D3DXVECTOR3(0,0,0);
	m_pVBShadow = NULL;
	m_nUnitNum = 0;
	m_nPilotNum = 0;						// 2005-07-13 by ispark	초기화
	m_bCharacter = FALSE;					// 2005-07-13 by ispark

	m_dwPkState = PK_NONE;
	m_bPkAttack = FALSE;
	CUnitData::InitDeviceObjects();

	m_nAlphaValue = SKILL_OBJECT_ALPHA_NONE;// 2006-11-16 by ispark
	m_bySkillStateFlag = CL_SKILL_NONE;		// 2006-11-21 by ispark

	// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크	
	m_byUnitState = 0x00;
	for(int nCnt = 0;nCnt < UNIT_STATE_MAX; nCnt++)
	{		
		m_fUnitStateChgCap[nCnt] = 0;
	}	

	// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	m_bCheckObjectEnemyShow = TRUE;		// 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리
	m_bCheckObject = FALSE;
	n_fCheckObjectTime = 0;
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_LoadingPriority = _NOTHING_PRIORITY;
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	
	// 2010. 03. 18 by jskim 몬스터변신 카드
	m_MonsterTransPrimarySidePos = D3DXVECTOR3(0,0,0);
	m_MonsterTransSecondarySidePos = D3DXVECTOR3(0,0,0);
	m_MonsterTransScale  = 0.0f;
	m_MonsterTransformer = 0;
	//end 2010. 03. 18 by jskim 몬스터변신 카드
}

CUnitData::~CUnitData()
{
	FLOG("~CUnitData::~CUnitData()");
	CUnitData::DeleteDeviceObjects();
	SAFE_RELEASE(m_pVBShadow);
	m_pChaffData.clear();
}

void CUnitData::ChangeUnitState( DWORD dwState )
{
	m_dwState = dwState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BodyCond_t CUnitData::GetCurrentBodyCondition(void)
/// \brief		유닛의 현재 바디컨디션을 반환한다. 서버로 전송시에 사용한다.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	BOOSTER바디컨디션이 실행중일때, BODYCON_FLY_MASK를 넣는다.
///				m_pCharacterInfo == NULL인 경우(몬스터정보가 없는 경우) 발생
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BodyCond_t CUnitData::GetCurrentBodyCondition(void)
{
	FLOG("CUnitData::GetCurrentBodyCondition(void)");
	if(!m_pCharacterInfo)
		return 0;
	BodyCond_t hyBody = 0;
	hyBody = m_pCharacterInfo->m_nCurrentBodyCondition;
	if(hyBody & BODYCON_BOOSTER_EX_STATE_CLEAR_MASK)
	{
		hyBody |= BODYCON_FLY_MASK;
	}
	return hyBody;
}

void CUnitData::TurnSingleBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet)
{
	FLOG("CUnitData::TurnSingleBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet)");
	if(m_pCharacterInfo)
	{
		if(bSet)
		{
			m_pCharacterInfo->TurnOnSingleBodyCondition(hySingleBodyCondition);
		}
		else
		{
			m_pCharacterInfo->TurnOffSingleBodyCondition(hySingleBodyCondition);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::ChangeUnitBodyCondition(BodyCond_t hyBodyCondition)
/// \brief		유닛관련 바디컨디션을 세팅한다.
/// \author		dhkwon
/// \date		2004-04-10 ~ 2004-04-10
/// \warning	BODYCON_CLIENT_SINGLE이 포함된 경우와 ~BODYCON_CLIENT_SINGLE인 경우를 나누어 세팅한다.
///				BODYCON_CLIENT_SINGLE는 하나만 선택된다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeUnitBodyCondition(BodyCond_t hyBodyCondition)
{
	FLOG("CUnitData::ChangeUnitBodyCondition(BodyCond_t hyBodyCondition)");
	// BODYCON_CLIENT_SINGLE
	if(hyBodyCondition & BODYCON_CLIENT_SINGLE)
	{
		ChangeSingleBodyCondition(hyBodyCondition & BODYCON_CLIENT_SINGLE);
	}
	// ~BODYCON_CLIENT_SINGLE
//	if(hyBodyCondition & ~BODYCON_CLIENT_SINGLE)
	{
		ChangeKeepingBodyCondition(hyBodyCondition & ~BODYCON_CLIENT_SINGLE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::ChangeKeepingBodyCondition(BodyCond_t hyBodyCondition)
/// \brief		~BODYCON_CLIENT_SINGLE인 경우를 처리한다.
/// \author		dhkwon
/// \date		2004-04-10 ~ 2004-04-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeKeepingBodyCondition(BodyCond_t hyBodyCondition)
{
	FLOG("CUnitData::ChangeKeepingBodyCondition(BodyCond_t hyBodyCondition)");
	if(m_pCharacterInfo)
	{
		hyBodyCondition &= ~BODYCON_CLIENT_SINGLE;
		BOOL bSet = (BODYCON_DAMAGE1_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_DAMAGE1_MASK, bSet );
		bSet = (BODYCON_DAMAGE2_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_DAMAGE2_MASK, bSet );
		bSet = (BODYCON_DAMAGE3_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_DAMAGE3_MASK, bSet );
		bSet = (BODYCON_NIGHTFLY_MASK & hyBodyCondition) ? TRUE : FALSE;
		TurnSingleBodyCondition(BODYCON_NIGHTFLY_MASK, bSet );
	}
}

void CUnitData::ChangeSingleBodyCondition( BodyCond_t hySingleBodyCondition )
{
	FLOG("CUnitData::ChangeSingleBodyCondition( BodyCond_t hySingleBodyCondition )");
	if(!m_pCharacterInfo)
		return;
	// 이전 바디컨중 BODYCON_CLIENT_SINGLE에 속한 바디컨중 hySingleBodyCondition이 아닌 바디컨은 지운다.
 	hySingleBodyCondition &= ~BODYCON_FLY_MASK;// delete BODYCON_FLY_MASK
	BodyCond_t hyDeleteBodyCon = m_pCharacterInfo->m_nCurrentBodyCondition & (~hySingleBodyCondition & BODYCON_CLIENT_SINGLE);
	// 2006-01-21 by ispark, 캐릭터
	if(hyDeleteBodyCon & BODYCON_CHARACTER_MODE_STOP)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_CHARACTER_MODE_STOP);
	}
	if(hyDeleteBodyCon & BODYCON_CHARACTER_MODE_WALK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_CHARACTER_MODE_WALK);
	}
	if(hyDeleteBodyCon & BODYCON_CHARACTER_MODE_RUN)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_CHARACTER_MODE_RUN);
	}
	// 2006-01-21 by ispark, 기어
	if(hyDeleteBodyCon & BODYCON_EXPLOSION_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_EXPLOSION_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_TAKEOFF_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_TAKEOFF_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_SIEGE_ON_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_SIEGE_ON_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_SIEGE_OFF_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_SIEGE_OFF_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_LANDING_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_LANDING_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_LANDED_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_LANDED_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_DEAD_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_DEAD_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_NOT_USED1_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_NOT_USED1_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER1_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER1_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER2_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER2_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER4_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER4_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER5_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER5_MASK);
	}
	if(hyDeleteBodyCon & BODYCON_BOOSTER3_MASK)
	{
		m_pCharacterInfo->TurnOffSingleBodyCondition(BODYCON_BOOSTER3_MASK);
		if(m_dwPartType == _SHUTTLE)
		{
			CItemInfo * pItemInfo = g_pStoreData->FindItemInInventoryByWindowPos( POS_REAR );
			//ITEM* pITEM = g_pStoreData->GetItemInfoFromItemGeneral((ITEM_GENERAL*)pItemInfo);
			if( pItemInfo )
			{
				ITEM* pITEM = pItemInfo->GetRealItemInfo();
				if(pITEM)
				{
//					g_pShuttleChild->m_fMouseRate = pITEM->RangeAngle; // 엔진의 RangeAngle은 shuttle의 Paramfactor가 적용되지 않는다.
					g_pShuttleChild->m_fMouseRate = CAtumSJ::GetEngineRangeAngle(pITEM, &g_pShuttleChild->m_paramFactor); // 엔진의 RangeAngle은 shuttle의 Paramfactor가 적용되지 않는다.
				}
			}
			CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
			if(pEffect)
			{
				pEffect->ChangeBodyCondition(BODYCON_BOOSTER_OFF);
			}
		}
		// 2006-10-25 by dgwoo
		else if(m_dwPartType == _ADMIN)
		{
			CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
			if(pEffect)
			{
				pEffect->ChangeBodyCondition(BODYCON_BOOSTER_OFF);
			}
		}
		if(m_pRadar)
		{
			// 레이다 바디컨디션 설정( 꺼진 상태 , 기존 디파인 숫자 사용 )
			m_pRadar->ChangeBodyCondition(RADAR_BODYCON_BOOSTER_OFF << GetShiftWeaponBodyconditionByUnitKind());
		}
	}
	if(hySingleBodyCondition & BODYCON_BOOSTER3_MASK)
	{
		if(m_pRadar)
		{
			// 레이다 바디컨디션 설정
			m_pRadar->ResetBodyCondition(RADAR_BODYCON_BOOSTER_ON << GetShiftWeaponBodyconditionByUnitKind());
		}
	}
	// Turn on bodycon
#ifdef _DEBUG
	BodyCond_t hyTemp = 1;
	BodyCond_t hyLast = 0;
	int nBodyConNumber = 0;
	while( hyTemp )
	{
		if(hyTemp & hySingleBodyCondition & BODYCON_CLIENT_SINGLE)
		{
			nBodyConNumber++;
			hyLast = hyTemp;
		}
		hyTemp <<= 1;
	}
	if(nBodyConNumber > 1)
	{
//		DBGOUT("CUnitData::ChangeSingleBodyCondition( %016I64X --> %016I64X) (nBodyConNumber > 1) So Set Last BodyCondition \n",hySingleBodyCondition,hyLast);
		hySingleBodyCondition = hyLast;
	}
#endif // _DEBUG_endif
	m_pCharacterInfo->TurnOnSingleBodyCondition(hySingleBodyCondition);
	if( hySingleBodyCondition & BODYCON_BOOSTER3_MASK || 
		hySingleBodyCondition & BODYCON_BOOSTER2_MASK ||
		hySingleBodyCondition & BODYCON_BOOSTER4_MASK ||
		hySingleBodyCondition & BODYCON_BOOSTER1_MASK)
	{
		int nUnitKind = 0;
		if(m_dwPartType == _SHUTTLE)
		{
			nUnitKind = g_pShuttleChild->m_myShuttleInfo.UnitKind;
			if(!(hySingleBodyCondition & BODYCON_BOOSTER3_MASK))
			{
				if(IS_BT(nUnitKind) || IS_ST(nUnitKind) )
				{
					g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
					// 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_B_I_GEAR, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_B_I_GEAR, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
				}
				else
				{
					g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );
					// 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_M_A_GEAR, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_FLYING_M_A_GEAR, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
				}
			}
			if(hySingleBodyCondition & BODYCON_BOOSTER4_MASK)
			{
				if(IS_BT(nUnitKind) || IS_ST(nUnitKind) )
				{
					// 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_HIGH_BOOSTER_END, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_HIGH_BOOSTER_END, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
				}
				else
				{
					// 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
					//g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LOW_BOOSTER_END, m_vPos, FALSE);
					g_pD3dApp->m_pSound->PlayD3DSound( SOUND_LOW_BOOSTER_END, m_vPos, TRUE);
					//end 2010. 07. 07 by jskim 모선전시 사운드 버그 수정
				}
			}
		}
		else if(m_dwPartType == _ENEMY)
		{
			nUnitKind = ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.UnitKind;
		}

//		float fDistance = (g_pScene->m_fFogEndValue*g_pSOption->sUnitDetail)/MAX_OPTION_VALUE;
		float fDistance = (RANGE_OF_VISION*g_pSOption->sUnitDetail)/MAX_OPTION_VALUE;
		// 2006-01-20 by ispark, 삭제
//		if( this == g_pShuttleChild ||
//		    (g_pD3dApp->m_bDegree != 0 && 
//			 D3DXVec3Length(&(g_pD3dApp->m_pCamera->GetEyePt() - m_vPos)) < fDistance ))
//		{
//			// 기어 꼬리 잔상 이펙트
//			CAppEffectData* pGearTrace = g_pScene->FindEffect(GetGearTrace(nUnitKind), this);
//			if( !pGearTrace )
//			{
//				CAppEffectData* pData = new CAppEffectData(this,GetGearTrace(nUnitKind),D3DXVECTOR3(0,0,0));
//				g_pD3dApp->m_pEffectList->AddChild(pData);
//			}
//		}
	}
	else
	{
		int nUnitKind = 0;
		if(m_dwPartType == _SHUTTLE)
		{
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_M_A_GEAR );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_HIGH_BOOSTER );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_FLYING_B_I_GEAR );
			g_pD3dApp->m_pSound->StopD3DSound( SOUND_LOW_BOOSTER );
			nUnitKind = g_pShuttleChild->m_myShuttleInfo.UnitKind;
		}
		else if(m_dwPartType == _ENEMY)
		{
			nUnitKind = ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.UnitKind;
		}
		
//		if(this == g_pShuttleChild)
//		{
//			CAppEffectData* pData = g_pScene->FindEffect(GetGearTrace(nUnitKind),this);
//			if(pData)
//			{
//				pData->m_bUsing = FALSE;
//				pData->Tick();
//			}
//		}
		CAppEffectData* pData = g_pScene->FindEffect(GetGearTrace(nUnitKind),this);
		if(pData)
		{
			pData->m_bUsing = FALSE;
			//pData->Tick();
		}
	}
	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	
	BOOL Update_BodyCon = TRUE;
	if((hySingleBodyCondition & (BODYCON_LANDING_MASK|BODYCON_LANDED_MASK|BODYCON_TAKEOFF_MASK)) &&
		IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		Update_BodyCon = FALSE;
	}
	if(m_pPartner && Update_BodyCon)
	{
        // 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
        //m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		if( g_pD3dApp->m_bCharacter == TRUE &&								// 캐릭터 상에서만 쓰이는 바디 컨디션
			(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK  ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN))
		{
			m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		}
		if (g_pD3dApp->m_bCharacter == FALSE &&
			hySingleBodyCondition != NULL &&
			!(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK  ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN))
		{
			m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		}
	}
	
	if(m_pPartner1 && Update_BodyCon)
	{								
        // 2010-06-15 by shcho&hslee 펫시스템 - 마을 이동 처리
        //m_pPartner->ChangeBodyCondition(hySingleBodyCondition);
		if( hySingleBodyCondition & BODYCON_BOOSTER1_MASK ||
			(g_pD3dApp->m_bCharacter == TRUE &&								// 캐릭터 상에서만 쓰이는 바디 컨디션
			(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN)))
		{
			m_pPartner1->ChangeBodyCondition(hySingleBodyCondition);
		}
		if (g_pD3dApp->m_bCharacter == FALSE &&	
			hySingleBodyCondition != NULL &&
			!(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK  ||
			hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN))
		{
			m_pPartner1->ChangeBodyCondition(hySingleBodyCondition);
		}
	}
	// 2010-06-08 by dgwoo, 펫시스템 추가. 

	// engine bodycon
	if(hySingleBodyCondition & BODYCON_UNIT_TO_ENGINE_LANDED)
	{
		hySingleBodyCondition = BODYCON_LANDED_MASK;
	}
 	if( BODYCON_ENGINE_STATE_MASK & hySingleBodyCondition && m_pEngine)
	{
//		ASSERT_ASSERT(m_pEngine);
		m_pEngine->ResetBodyCondition( BODYCON_ENGINE_STATE_MASK & hySingleBodyCondition );
	}

	// 2006-06-28 by ispark, 시간 제한 없는 악세사리 바디컨디션
	// 2006-08-21 by ispark, 기타 악세사리 추가
	if( hySingleBodyCondition & BODYCON_BOOSTER1_MASK ||
		(g_pD3dApp->m_bCharacter == TRUE &&								// 캐릭터 상에서만 쓰이는 바디 컨디션
		(hySingleBodyCondition & BODYCON_CHARACTER_MODE_STOP ||
		hySingleBodyCondition & BODYCON_CHARACTER_MODE_WALK ||
		hySingleBodyCondition & BODYCON_CHARACTER_MODE_RUN)))
	{
		if(m_pContainer)
		{
			m_pContainer->ChangeBodyCondition(hySingleBodyCondition);
		}
		// 2006-08-18 by ispark, 기타 바디 컨디션
		if(m_pAccessories)
		{
			m_pAccessories->ChangeBodyCondition(hySingleBodyCondition);
		}
		if(m_pWingIn)
		{
			m_pWingIn->ChangeBodyCondition(hySingleBodyCondition);
		}	
	}
/*	if( this == g_pShuttleChild )
	{
		if( hySingleBodyCondition == BODYCON_BOOSTER4_MASK )
		{
			CAppEffectData* pEffect = g_pScene->FindEffect( RC_EFF_BOOSTER );
			if(pEffect)
			{
				pEffect->ChangeBodyCondition(BODYCON_BOOSTER_OFF);
			}
		}
	}*/
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetBodyCondition( BodyCond_t hyBody ),ChangeBodyCondition
/// \brief		유닛의 bodycon을 바꾸고, 원래 있던 bodycon은 애니메이션을 다시 시작한다.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	BODYCON_CLIENT_SINGLE중 하나만 들어가고,
///				BODYCON_FLY_MASK는 삭제한다.
///				BOOSTER bodycon은 엔진 바디 컨디션도 바꾼다.
///				BODYCON_CLIENT_SINGLE가 아닌 바디컨은 처리하지 않는다.(TurnOn, TurnOff를 이용해야 한다.)
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::ResetBodyCondition( BodyCond_t hyBody )");
	if(m_pCharacterInfo)
	{
		hyBody &= ~BODYCON_FLY_MASK;// delete BODYCON_FLY_MASK
	//	if(BODYCON_CLIENT_SINGLE & hyBody )
	//	{
	//		BodyCond_t hyTemp = m_pCharacterInfo->m_nCurrentBodyCondition;
	//		SET_CLIENT_BODYCON_BIT(hyTemp,hyBody);
	//		m_pCharacterInfo->ResetBodyCondition(hyTemp);
	//	}
		BodyCond_t hySingleBodyCon = BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;
		if( BODYCON_CLIENT_SINGLE & hyBody )
		{
			SET_CLIENT_BODYCON_BIT(hySingleBodyCon,BODYCON_CLIENT_SINGLE & hyBody);
		}
		BodyCond_t hyTemp = ~BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;
		DBGOUT("ResetBODYCONDITION 0x%016I64x\n",hySingleBodyCon | hyTemp);
		m_pCharacterInfo->ResetBodyCondition(hySingleBodyCon | hyTemp);
		if(hyBody & BODYCON_UNIT_TO_ENGINE_LANDED)
		{
			hyBody = BODYCON_LANDED_MASK;
		}
		if( (BODYCON_ENGINE_STATE_MASK & hyBody) && m_pEngine)
		{
			m_pEngine->ResetBodyCondition( BODYCON_ENGINE_STATE_MASK & hyBody );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangeBodyCondition( BodyCond_t hyBody )
/// \brief		유닛의 바디컨디션을 바꾼다.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	BODYCON_BOOSTER_EX_STATE_CLEAR_MASK,BODYCON_EX_STATE_CLEAR_MASK중 하나만 들어가고,
///				BODYCON_FLY_MASK는 삭제한다.
///				BOOSTER bodycon은 엔진 바디 컨디션도 바꾼다.
///				BODYCON_BOOSTER_EX_STATE_CLEAR_MASK,BODYCON_EX_STATE_CLEAR_MASK가 아닌 바디컨은 처리하지 않는다.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::ChangeBodyCondition( BodyCond_t hyBody )");
	if(m_pCharacterInfo)
	{
		hyBody &= ~BODYCON_FLY_MASK;// delete BODYCON_FLY_MASK
		BodyCond_t hySingleBodyCon = BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;
		if( BODYCON_CLIENT_SINGLE & hyBody )
		{
			SET_CLIENT_BODYCON_BIT(hySingleBodyCon,BODYCON_CLIENT_SINGLE & hyBody);
		}
		// 2008-04-15 by bhsohn 바디 컨디션 잘 안되는 문제 해결
		//BodyCond_t hyTemp = ~BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition;		
		// 과거 바디 컨디션이 아닌 새로온 바디 컨디션으로 셋팅해주어야한다.
		BodyCond_t hyTemp = ~BODYCON_CLIENT_SINGLE & hyBody;		
		//hyTemp |= (~BODYCON_CLIENT_SINGLE & m_pCharacterInfo->m_nCurrentBodyCondition);
		// end 2008-04-15 by bhsohn 바디 컨디션 잘 안되는 문제 해결
		
		//DBGOUT("ChangeBODYCONDITION 0x%016I64x\n",hySingleBodyCon | hyTemp);

		m_pCharacterInfo->ChangeBodyCondition(hySingleBodyCon | hyTemp);
		if(hyBody & BODYCON_UNIT_TO_ENGINE_LANDED)
		{
			hyBody = BODYCON_LANDED_MASK;
		}
		if( (BODYCON_ENGINE_STATE_MASK & hyBody) && m_pEngine )
		{
			m_pEngine->ChangeBodyCondition( BODYCON_ENGINE_STATE_MASK & hyBody );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::SetFlyBodyCondition( BodyCond_t hyBody )
/// \brief		유닛 컨트롤시에 비행 바디컨디션을 강제로 세팅한다.
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	Booster bodycon만 셋이 된다. 기존 다른(BOOSTER가 아닌) 바디컨디션은 유지된다.
///				인자는 Booster bodycon중 하나만 들어간다.
/// \param		hyBody : BODYCON_BOOSTERx_MASK(x:1,2,3,4,5) 중 하나만 들어간다.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetFlyBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::SetFlyBodyCondition( BodyCond_t hyBody )");
	ASSERT_IMPLIES(m_pCharacterInfo, ((BODYCON_BOOSTER_EX_STATE_CLEAR_MASK & hyBody) | !hyBody) );
	BodyCond_t hyTemp = m_pCharacterInfo->m_nCurrentBodyCondition;
	if(hyTemp & hyBody)
	{
		DBGOUT("Already SetBodyCondition (this:0x%08x, bodycon:%016I64x)\n", this, hyBody);//같은 바디컨디션 세팅
		return;
	}

#ifdef _DEBUG
	if(hyBody & BODYCON_FLY_MASK)
	{
		DBGOUT("Don't Set BODYCON_FLY_MASK in CUnitData::SetFlyBodyCondition \n");//CUnitData::SetFlyBodyCondition에서는 BODYCON_FLY_MASK을 셋해서는 안된다.\n
		hyBody |= ~BODYCON_FLY_MASK;
	}
#else
	hyBody |= ~BODYCON_FLY_MASK;
#endif // _DEBUG_endif

	CLEAR_BODYCON_BIT(hyTemp, BODYCON_BOOSTER_EX_STATE_CLEAR_MASK);
	m_pCharacterInfo->ResetBodyCondition(hyTemp | hyBody);// Reset만 한다.

	if(m_pEngine)
	{
		m_pEngine->ChangeBodyCondition( hyBody );
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::SetExBodyCondition( BodyCond_t hyBody )
/// \brief		유닛의 일반 바디컨디션을 셋한다.(BodyCond_t)
///				(~BODYCON_FLY_MASK | BODYCON_LANDING_MASK | BODYCON_LANDED_MASK | BODYCON_DEAD_MASK | BODYCON_NOT_USED1_MASK)
/// \author		dhkwon
/// \date		2004-03-28 ~ 2004-03-28
/// \warning	BODYCON_FLY_MASK는 삭제한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetExBodyCondition( BodyCond_t hyBody )
{
	FLOG("CUnitData::SetStateBodyCondition( BodyCond_t hyBody )");
	ASSERT_IMPLIES(m_pCharacterInfo, ((BODYCON_EX_STATE_CLEAR_MASK & hyBody) | !hyBody) );
	BodyCond_t hyTemp = m_pCharacterInfo->m_nCurrentBodyCondition;
	if(hyTemp & hyBody)
	{
		DBGOUT("Already SetBodyCondition (this:0x%08x, bodycon:%016I64x)\n", this, hyBody);
		return;
	}
	CLEAR_BODYCON_BIT(hyTemp, BODYCON_EX_STATE_CLEAR_MASK);
#ifdef _DEBUG
	if(hyBody & BODYCON_FLY_MASK)
	{
		//CUnitData::SetExBodyCondition에서는 BODYCON_FLY_MASK을 셋해서는 안된다.현재 부스터바디컨디션:0x%016I64d\n
		DBGOUT("Don't Set BODYCON_FLY_MASK in CUnitData::SetExBodyCondition. Current Booster Bodycondition:0x%016I64d\n", 
			m_pCharacterInfo->m_nCurrentBodyCondition & BODYCON_BOOSTER_EX_STATE_CLEAR_MASK);
		hyBody |= ~BODYCON_FLY_MASK;
	}
#endif // _DEBUG_endif
	//m_pCharacterInfo->m_nCurrentBodyCondition |= hyBody;
	m_pCharacterInfo->ResetBodyCondition(hyTemp | hyBody);// Reset만 한다.
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			float CUnitData::GetCurrentAnimationTime(void)
/// \brief		유닛의 현재 애니메이션 타임
/// \author		dhkwon
/// \date		2004-03-26 ~ 2004-03-26
/// \warning	m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_CHARACTER_ANIMATION_TIME);
///				위의 함수를 유닛초기화시에 실행시켜 줘야 한다.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CUnitData::GetCurrentAnimationTime(void)
{
	FLOG("CUnitData::GetCurrentAnimationTime(void)");
#ifdef _DEBUG
/*	if(this == g_pShuttleChild)
	{
		wsprintf( g_pD3dApp->m_strDebug, "AniBodyCon[0x%08X, %016I64X] CurrentBodyCon[%016I64X]", 
			m_pCharacterInfo->m_pCharacterAnimationBodyCondition,
			m_pCharacterInfo->m_pCharacterAnimationBodyCondition ? m_pCharacterInfo->m_pCharacterAnimationBodyCondition->m_nBodyCondition : 0, 
			m_pCharacterInfo->m_nCurrentBodyCondition );
	}*/
#endif // _DEBUG_endif
	return m_pCharacterInfo ? m_pCharacterInfo->GetCurrentCharacterAnimationTime() : 0;
}

float CUnitData::GetCurrentBodyConditionEndAnimationTime(void)
{
	FLOG("CUnitData::GetCurrentAnimationTime(void)");
	return m_pCharacterInfo ? m_pCharacterInfo->GetCurrentBodyConditionEndAnimationTime() : 0;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreatePrimaryWeaponItem(ITEM* pItem) 
/// \brief		Primary weapon을 생성한다.(1-1, 1-2)
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	아이템이 없는 경우 포트를 로드한다. 무기 발사 좌표를 가져온다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//void CUnitData::CreatePrimaryWeaponItem(ITEM* pItem)
void CUnitData::CreatePrimaryWeaponItem( ITEM* pItem, ITEM* pShapeItem )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

{
	FLOG("CUnitData::CreatePrimaryWeaponItem(ITEM* pItem) ");
	ITEM* pNULL = NULL;

	int type = 0;
	if(m_dwPartType == _SHUTTLE)
		type = GetUnitTypeDecimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind);
	else if(m_dwPartType == _ENEMY)// || pNode->m_dwPartType == _PARTY)
		type = GetUnitTypeDecimal(((CEnemyData *)this)->m_infoCharacter.CharacterInfo.UnitKind);

	if(NULL == pItem)
	{
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_11 );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_12 );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11(pNULL, type) );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_12, EFFECT_NUMBER_WEAR_WEAPON_12(pNULL, type) );
	}
	else if(IS_PRIMARY_WEAPON_1(pItem->Kind))
	{
		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		//CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11(pItem, type) );

		if( pShapeItem )
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11( pShapeItem, type ) );
		else
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11( pItem, type ) );
		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_12 );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_12, EFFECT_NUMBER_WEAR_WEAPON_12(pNULL, type) );
		if( m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo)
		{
			// 2006-01-12 by ispark, 삭제
// 			m_PrimaryAttack.vSidePos = m_pWeapon1_1_1->m_pCharacterInfo->GetEffectPos( 
// 				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
 			m_PrimaryAttack.vSideBackPos = m_pWeapon1_1_1->m_pCharacterInfo->GetEffectPos( 
 				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_BACK_POSITION );
			// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
			//SetWeaponPosition();
			SetPrimaryWeaponPosition();
			// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
			
			// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
			if( IsRobotArmor() )
			{
				// 로봇아머일 경우 기존 이펙트를 감추고
				ShowWeapon( FALSE );

				// A기어일 경우
				if( type == 2 )
				{
					// A기어용 임시 오브젝트를 붙임
					ITEM* pAGearWeapon = g_pDatabase->GetServerItemInfo( A_GEAR_ROBOT_PRIMARY_WEAPON_ITEM_NUM );
					if( pAGearWeapon )
					{
						CreateRobotAGearWeapon( pAGearWeapon, type );
					}
				}
			}
			// 2010. 03. 18 by jskim 몬스터변신 카드
			else if(m_MonsterTransformer)
			{
				ShowWeapon( FALSE );
			}
			//end 2010. 03. 18 by jskim 몬스터변신 카드
			// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
		}
	}
	else if(IS_PRIMARY_WEAPON_2(pItem->Kind))
	{
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_11 );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_11, EFFECT_NUMBER_WEAR_WEAPON_11(pNULL, type) );
		CreateWearItem( WEAR_ITEM_KIND_WEAPON_12, EFFECT_NUMBER_WEAR_WEAPON_12(pItem, type) );
		if( m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo)
		{
			m_PrimaryAttack.vSidePos = m_pWeapon1_2->m_pCharacterInfo->GetEffectPos( 
				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
			m_PrimaryAttack.vSideBackPos = m_pWeapon1_2->m_pCharacterInfo->GetEffectPos( 
				WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_BACK_POSITION );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateSecondaryWeaponItem(ITEM* pItem) 
/// \brief		Secondary weapon을 생성한다.(2-1, 2-2)
/// \author		dhkwon
/// \date		2004-03-27 ~ 2004-03-27
/// \warning	아이템이 없는 경우 포트를 로드한다. 무기 발사 좌표를 가져온다.(아직 구현 안함)
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//void CUnitData::CreateSecondaryWeaponItem(ITEM* pItem)
void CUnitData::CreateSecondaryWeaponItem( ITEM* pItem, ITEM* pShapeItem )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

{
	FLOG("CUnitData::CreateSecondaryWeaponItem(ITEM* pItem)");
	ITEM* pNULL = NULL;

	int type = 0;
	if(m_dwPartType == _SHUTTLE)
		type = GGetUnitKindHexToDeimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind);
//		type = GetUnitTypeDecimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind);
	else if(m_dwPartType == _ENEMY)// || pNode->m_dwPartType == _PARTY)
		type = GGetUnitKindHexToDeimal(((CEnemyData *)this)->m_infoCharacter.CharacterInfo.UnitKind);
//		type = GetUnitTypeDecimal(((CEnemyData *)this)->m_infoCharacter.CharacterInfo.UnitKind);

	if(NULL == pItem)
	{
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21(pNULL, type) );
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_22, EFFECT_NUMBER_WEAR_WEAPON_22(pNULL, type) );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_21 );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_22 );
	}
	else if(IS_SECONDARY_WEAPON_1(pItem->Kind))
	{
		
		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		//CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21(pItem, type) );
		if( pShapeItem )
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21( pShapeItem, type) );
		else
			CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21( pItem, type) );
		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_22, EFFECT_NUMBER_WEAR_WEAPON_22(pNULL, type) );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_22 );
		// 2006-01-12 by ispark, 삭제
//		if( m_pWeapon2_1_1 && m_pWeapon2_1_1->m_pCharacterInfo)
//		{
//			m_SecondaryAttack.vSidePos = m_pWeapon2_1_1->m_pCharacterInfo->GetEffectPos( WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
//		}
		// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
		//SetWeaponPosition();
		SetSecondaryWeaponPosition();
		// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

		// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
		// 2010. 03. 18 by jskim 몬스터변신 카드
// 		if( IsRobotArmor() )
// 		{
// 			// 로봇아머일 경우 2형 무기는 항상 보이지 않음
// 			ShowWeapon( FALSE );
// 		}
		if( IsRobotArmor() || ((CShuttleChild *)this)->GetMonsterTransformer())
		{
			// 로봇아머일 경우 2형 무기는 항상 보이지 않음
			ShowWeapon( FALSE );
		}
		//end 2010. 03. 18 by jskim 몬스터변신 카드
		// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	}
	else if(IS_SECONDARY_WEAPON_2(pItem->Kind))
	{
//		CreateWearItem( WEAR_ITEM_KIND_WEAPON_21, EFFECT_NUMBER_WEAR_WEAPON_21(pNULL, type) );
		DeleteWearItem( WEAR_ITEM_KIND_WEAPON_21 );
		CreateWearItem( WEAR_ITEM_KIND_WEAPON_22, EFFECT_NUMBER_WEAR_WEAPON_22(pItem, type) );
		if( m_pWeapon2_2 && m_pWeapon2_2->m_pCharacterInfo)
		{
			m_SecondaryAttack.vSidePos = m_pWeapon2_2->m_pCharacterInfo->GetEffectPos( WEAPON_BODYCON_FIRE_1 << GetShiftWeaponBodyconditionByUnitKind(), RC_EFF_DUMMY_POSITION );
		}

		// 토글형인 경우 사용중이면 TOGGLE_WEAPON_BODYCON_USING, 사용중이 아니면 TOGGLE_WEAPON_BODYCON_NORMAL
		if(m_dwPartType == _SHUTTLE)
		{
			if( pItem->Kind == ITEMKIND_SHIELD || pItem->Kind == ITEMKIND_DECOY ) // 토글형 2-2형 무기아이템 )
			{
				if(g_pShuttleChild->m_pSecondaryWeapon->IsUsingToggleWeapon() == TRUE)
				{ // 토글 무기 사용중
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_USING);
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_USING, FALSE);
				}
				else
				{ // 토글 무기 사용중아님
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL);
					ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL, FALSE);
				}
			}
		}
		else if( m_dwPartType == _ENEMY )
		{
			ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL);
			ResetSecondaryWeaponBodyCondition(TOGGLE_WEAPON_BODYCON_NORMAL, FALSE);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateWearItem( int nWearItemKind, int nEffectNum, BOOL bCharacter /*= FALSE*/ )
/// \brief		장착된 각 부분별로 생성한다., 캐릭터 구별이 필요한것이 있으면 3번째 인자를 넣는다..
///				기본으로 기어 상태(FALSE)이다.
///				현재 구별 대상자는(WEAR_ITEM_KIND_WINGIN, WEAR_ITEM_KIND_ACCESSORIES, WEAR_ITEM_KIND_ATTACHMENT)
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	nWearItemKind : 장착된 부위, 
///				차후에는 CreateWearItemEffect함수로 가야한다.(m_pWearEffect[] ->배열 형식으로 가야함)
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateWearItem( int nWearItemKind, int nEffectNum, BOOL bCharacter /*= FALSE*/ )
{
	FLOG("CUnitData::CreateWearItem( int nWearItemKind, int nEffectNum )");
	// 2005-10-21 by ispark
//	if(m_dwPartType == _ADMIN)
//		return;

	switch( nWearItemKind )
	{
	case WEAR_ITEM_KIND_WEAPON_11:
		{
//			DBGOUT("1형 아이템 생성(KIND:%d)(NUM:%d)\n", nWearItemKind, nEffectNum);
			// 2006-01-02 by ispark, 무기 모듈
			m_pWeapon1_1_1 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_1_1_1 + ENEMY_WEAPON_INDEX_DUMMY);	  // 2012-09-20 by jhahn 아이템 미리보기 버그 수정 - 마크, 파트너 렌더링		
			m_pWeapon1_1_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum + 1, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_1_1_2 +  ENEMY_WEAPON_INDEX_DUMMY);  // 2012-09-20 by jhahn 아이템 미리보기 버그 수정 - 마크, 파트너 렌더링		
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_12:
		{
			m_pWeapon1_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING );
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_21:
		{
			// 2006-01-02 by ispark, 무기 모듈
			m_pWeapon2_1_1 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_2_1_1 +ENEMY_WEAPON_INDEX_DUMMY);	 // 2012-09-20 by jhahn 아이템 미리보기 버그 수정 - 마크, 파트너 렌더링		
			m_pWeapon2_1_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum + 1, 
												 D3DXVECTOR3(0,0,0), 
												 WEAPON_BODYCON_LOADING,
												 WEAPON_POSITION_2_1_2 +ENEMY_WEAPON_INDEX_DUMMY);	 // 2012-09-20 by jhahn 아이템 미리보기 버그 수정 - 마크, 파트너 렌더링		
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_22:
		{
			m_pWeapon2_2 = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0),
												 WEAPON_BODYCON_LOADING );
		}
		break;
	case WEAR_ITEM_KIND_PROW:
		{
			if(m_dwPartType == _ADMIN)
				return;

			m_pRadar = CreateWearItemEffect( nWearItemKind, 
											 nEffectNum, 
											 D3DXVECTOR3(0,0,0),
											 WEAR_ITEM_BODYCON_NORMAL );
		}
		break;
	case WEAR_ITEM_KIND_CENTER:
		{
//			m_pArmor = CreateWearItemEffect( nWearItemKind, 
//											 nEffectNum, 
//											 D3DXVECTOR3(0,0,0), 
//											 WEAR_ITEM_BODYCON_NORMAL );
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_UNLIMITED:
		{
			m_pContainer = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0),
												 WEAR_ITEM_BODYCON_NORMAL);
			if(bCharacter)
			{
				if(m_pContainer)
				{
					m_pContainer->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				}
			}
			else
			{
				if(m_pContainer)
				{
					m_pContainer->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
				}
			}
		}
		break;
	case WEAR_ITEM_KIND_ENGINE:
		{
			m_pEngine = CreateWearItemEffect( nWearItemKind,
											  nEffectNum, 
											  D3DXVECTOR3(0,0,0), 
											  WEAR_ITEM_BODYCON_NORMAL,
											  ENGINE_POSITION );
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT:
		{				
			m_pAccessories = CreateWearItemEffect( nWearItemKind,
											  nEffectNum, 
											  D3DXVECTOR3(0,0,0), 
											  WEAR_ITEM_BODYCON_NORMAL);

			if(bCharacter)
			{
				if(m_pAccessories)
				{
					m_pAccessories->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				}
			}
			else
			{
				if(m_pAccessories)
				{
					m_pAccessories->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
				}
			}
		}
		break;
	case WEAR_ITEM_KIND_WINGIN:
		{
			m_pWingIn = CreateWearItemEffect( nWearItemKind, 
												 nEffectNum, 
												 D3DXVECTOR3(0,0,0),
												 WEAR_ITEM_BODYCON_NORMAL);
			if(bCharacter)
			{
				if(m_pWingIn)
				{
					m_pWingIn->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
				}
			}
			else
			{
				if(m_pWingIn)
				{
					m_pWingIn->ChangeBodyCondition(BODYCON_BOOSTER1_MASK);
				}
			}
		}
		break;

	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	case WEAR_ITEM_KIND_PET:
		{
			if(bCharacter)
			{				
				m_pPartner = CreateWearItemEffect( nWearItemKind,
													nEffectNum, 
													D3DXVECTOR3(0,0,0), 
 													WEAR_ITEM_BODYCON_NORMAL);
 				if ( m_pPartner )
 					m_pPartner->ChangeBodyCondition(BODYCON_CHARACTER_MODE_STOP);
			}
			else
			{
				m_pPartner = CreateWearItemEffect( nWearItemKind,
													nEffectNum, 
													D3DXVECTOR3(0,0,0), 
 													WEAR_ITEM_BODYCON_NORMAL);
				
				m_pPartner1 = CreateWearItemEffect( nWearItemKind,
													nEffectNum, 
													D3DXVECTOR3(0,0,0), 
 													WEAR_ITEM_BODYCON_NORMAL);

				BodyCond_t Update_BodyCond = 0;
				if((g_pShuttleChild->GetCurrentBodyCondition() & (BODYCON_LANDING_MASK|BODYCON_LANDED_MASK)) &&
					!IS_DT(g_pShuttleChild->m_myShuttleInfo.UnitKind))
				{
					Update_BodyCond = BODYCON_LANDED_MASK;
				}
				else
				{
					Update_BodyCond = BODYCON_BOOSTER1_MASK;
				}

				if ( m_pPartner )
					m_pPartner->ChangeBodyCondition( Update_BodyCond );
	
				if ( m_pPartner1 )
 					m_pPartner1->ChangeBodyCondition( Update_BodyCond );

				CItemInfo* pAllItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET ); 
				if( pAllItem == NULL )
				{
					return;
				}

				tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pAllItem->UniqueNumber );
				if( tempCurrentInfo == NULL )
				{
					return;
				}	
				tPET_LEVEL_DATA* tempLevelData = g_pDatabase->GetPtr_PetLevelData( tempCurrentInfo->PetIndex,tempCurrentInfo->PetLevel );
				if( tempLevelData == NULL )
				{
					return;
				}

				//2011-10-06 by jhahn 파트너 성장형 시스템
				ITEM * pITEM = g_pDatabase->GetServerItemInfo( tempLevelData->UseWeaponIndex );	
				int nEffectNum2 = EFFECT_NUMBER_WEAR_WEAPON_11( pITEM, GetUnitTypeDecimal(((CShuttleChild *)this)->m_myShuttleInfo.UnitKind) );

				m_pDummyPartner = CreateWearItemEffect( nWearItemKind,
														nEffectNum2, 					   // 2011-10-06 by jhahn 파트너 성장형 시스템
														D3DXVECTOR3(0,0,0), 
 														WEAR_ITEM_BODYCON_NORMAL);

				m_pDummyPartner1 = CreateWearItemEffect( nWearItemKind,
														nEffectNum2, 					   // 2011-10-06 by jhahn 파트너 성장형 시스템
														D3DXVECTOR3(0,0,0), 
														WEAR_ITEM_BODYCON_NORMAL);
			}
				//end 2011-10-06 by jhahn 파트너 성장형 시스템
		}
		break;

	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAttEffectData * CUnitData::CreateWearItemEffect( int nWearItemKind, int nEffectNum, D3DXVECTOR3 vPos, int nBodyCondition, CAtumNode* pNode )
/// \brief		장착된 아이템의 생성
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	pEffect의 Effect파일이 없는 경우(m_pCharacterInfo == NULL)이면 바로 삭제된다.
///				Static 함수로 Create,Select 등 다른 위치에서도 생성 가능하다.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CAppEffectData * CUnitData::CreateWearItemEffect( int nWearItemKind, 
												  int nEffectNum, 
												  D3DXVECTOR3 vPos, 
												  int nBodyCondition,
												  int nWeaponPositionIndex )
{
	FLOG("CUnitData::CreateWearItemEffect( int nWearItemKind, ... )");
	DBGOUT_EFFECT("		CreateWearItemEffect [nWearItemKind:%d, nEffectNum:%d, bodycon:%I64X]\n", 
		nWearItemKind, nEffectNum, nBodyCondition );
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	int temp = _NOTHING_PRIORITY;
	if(g_pShuttleChild->GetUnitNum() == m_nUnitNum && 
		// 2010-06-15 by shcho&hslee 펫시스템 - 장착 아이템 Kind값 처리 수정.
		(nWearItemKind >= 0 && nWearItemKind < 11 ))
		//(nWearItemKind >= 0 && nWearItemKind < 10))
	{
		temp = _MY_CHARACTER_PRIORITY;
	}
	//CAppEffectData * pEffect = new CAppEffectData(this,nEffectNum,vPos, nWeaponPositionIndex);
	CAppEffectData * pEffect = new CAppEffectData(this,nEffectNum,vPos, nWeaponPositionIndex, temp );
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->AddChild(pEffect);
	if(pEffect)
	{
		ResetWearItemBodyCondition( nWearItemKind, pEffect, nBodyCondition );
	}
	return pEffect;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CAppEffectData * CreateWearItemEffectAtSelectMenu( GUIUnitRenderInfo *pInfo, int nEffectNum, int nBodyCondition )
/// \brief		select, create menu에서 아이템 장착을 위해 사용하는 함수
/// \author		dhkwon 
/// \date		2004-03-24 ~ 2004-03-24
/// \warning	static 함수
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CAppEffectData * CUnitData::CreateWearItemEffectAtSelectMenu( GUIUnitRenderInfo *pInfo, 
												   int nEffectNum,
												   BodyCond_t nBodyCondition )
{
	FLOG("CUnitData::CreateWearItemEffect( GUIUnitRenderInfo *pInfo, ... )");
	CAppEffectData * pEffect = new CAppEffectData(pInfo,nEffectNum);
	if(pEffect->m_pCharacterInfo)
		pEffect->m_pCharacterInfo->ChangeBodyCondition(nBodyCondition);
	pEffect = (CAppEffectData*)g_pD3dApp->m_pEffectList->AddChild(pEffect);
#ifdef DBGOUT_EFFECT
	if(!pEffect)
	{
		DBGOUT_EFFECT("		==>Delete (Non Existent BodyCondition)[Effect Number:%d, bodycon:%I64X]\n", nEffectNum, nBodyCondition );//	==>삭제 (바디컨디션없음)[이펙트번호:%d, bodycon:%I64X]\n
	}
#endif
	return pEffect;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetWearItemBodyCondition( CAttEffectData * pEffect, BodyCond_t hyBodyCon)
/// \brief		장착된 아이템의 바디컨디션 세팅. 유닛간 공유 지원.
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	엔진은 제외된다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetWearItemBodyCondition( int nWearItemKind, CAppEffectData * pEffect, BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ResetWearItemBodyCondition( int nWearItemKind, CAppEffectData * pEffect, BodyCond_t hyBodyCon)");
	if(pEffect->m_pCharacterInfo)
	{
		int nShiftValue = GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE;

		pEffect->m_bodyCondition = hyBodyCon << nShiftValue;//GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE;
		pEffect->m_pCharacterInfo->ResetBodyCondition(hyBodyCon <<  nShiftValue);//GET_SHIFT_BODYCON_OF_UNITKIND_EXCEPT_ENGINE);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DeleteWearItem( int nWearItemKind )
/// \brief		해당 위치의 아이템을 지운다.
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	후에 CreateWearItem과 같은 맥락으로 DeleteWearItemEffect와 합쳐야 한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DeleteWearItem( int nWearItemKind )
{
	FLOG("CUnitData::DeleteWearItem( int nWearItemKind )");
	switch( nWearItemKind )
	{
	case WEAR_ITEM_KIND_WEAPON_11:
		{
			DeleteWearItemEffect( m_pWeapon1_1_1 );
			m_pWeapon1_1_1 = NULL;
			DeleteWearItemEffect( m_pWeapon1_1_2 );
			m_pWeapon1_1_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_12:
		{
			DeleteWearItemEffect( m_pWeapon1_2 );
			m_pWeapon1_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_21:
		{
			DeleteWearItemEffect( m_pWeapon2_1_1 );
			m_pWeapon2_1_1 = NULL;
			DeleteWearItemEffect( m_pWeapon2_1_2 );
			m_pWeapon2_1_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WEAPON_22:
		{
			DeleteWearItemEffect( m_pWeapon2_2 );
			m_pWeapon2_2 = NULL;
		}
		break;
	case WEAR_ITEM_KIND_PROW:
		{
			DeleteWearItemEffect( m_pRadar );
			m_pRadar = NULL;
		}
		break;
	case WEAR_ITEM_KIND_CENTER:
		{
//			DeleteWearItemEffect( m_pArmor );
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_UNLIMITED:
		{
			DeleteWearItemEffect( m_pContainer );
			m_pContainer = NULL;
		}
		break;
	case WEAR_ITEM_KIND_ENGINE:
		{
			DeleteWearItemEffect( m_pEngine );
			m_pEngine = NULL;
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT:
		{
			DeleteWearItemEffect( m_pAccessories );
			m_pAccessories = NULL;
		}
		break;
	case WEAR_ITEM_KIND_WINGIN:
		{
			DeleteWearItemEffect( m_pWingIn );
			m_pWingIn = NULL;
		}
		break;
	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	case WEAR_ITEM_KIND_PET:
		{
			if(m_pPartner)
			{
				DeleteWearItemEffect(m_pPartner);	
			}
			if(m_pPartner1)
			{
				DeleteWearItemEffect(m_pPartner1);
			}

			if(m_pDummyPartner)
			{
				DeleteWearItemEffect(m_pDummyPartner);
			}

			if(m_pDummyPartner1)
			{
				DeleteWearItemEffect(m_pDummyPartner1);
			}
			//util::del(m_pPartner);
			m_pPartner = NULL;
			m_pPartner1 = NULL;
			m_pDummyPartner = NULL;
			m_pDummyPartner1 = NULL;
		}
		break;
	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	}


}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DeleteWearItemEffect( int nWearItemType )
/// \brief		장착된 아이템의 삭제
/// \author		dhkwon
/// \date		2004-03-23 ~ 2004-03-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DeleteWearItemEffect( CAppEffectData * pEffect )
{
	FLOG("CUnitData::DeleteWearItemEffect( CAppEffectData * pEffect )");
	if(pEffect)
	{
		pEffect->m_bUsing = FALSE;
		pEffect = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreatePrimaryWeaponByFieldServer( CUnitData* pTarget, 
///												  MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
///												  MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
/// \brief		Field server에서 Primaryweapon생성의 허락이 떨어졌을 경우 생성하는 코드
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	pPrimary, pItemPrimary 둘중 하나만 NULL이 아니어야 한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
/*
void CUnitData::CreatePrimaryWeaponByFieldServer( CUnitData* pTarget, 
												  MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
												  MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
{
	FLOG("CUnitData::CreatePrimaryWeaponByFieldServer( CUnitData* pTarget, ...");
	ASSERT_ASSERT( (pPrimary && !pItemPrimary) || (!pPrimary && pItemPrimary) );
	ITEM * pItem = g_pDatabase->GetServerItemInfo(MSG_PRIMARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
	if(!pItem)
	{
		DBGOUT("서버에 아이템 정보 요청으로 무기 생성 안됨(index:%d)\n",MSG_PRIMARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
		return;
	}
	if(pPrimary)
	{
		if(m_dwPartType == _MONSTER && m_PrimaryAttackData1.WeaponItemNumber != pPrimary->WeaponItemNumber)
		{
			m_PrimaryAttack.fCheckAttackTime = 0.0f;
		}
		memcpy(&m_PrimaryAttackData1,pPrimary,sizeof(MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY));
		m_bIsItemAttack = FALSE;
	}
	else
	{
		memcpy(&m_PrimaryAttackData2,pItemPrimary,sizeof(MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY));
		m_bIsItemAttack = TRUE;
	}
	m_pPrimaryAttackTarget = pTarget;
	if(this == g_pShuttleChild || m_dwPartType == _ENEMY)
	{
		CheckPrimaryAttack(TRUE, NULL);
	}
	else// monster
	{
		CheckPrimaryAttack(TRUE, pItem);
	}
}
*/
///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::CreateWeaponByFieldServer( MSG_FC_BATTLE_ATTACK_OK* pMsg )
/// \brief		Attack 결과 처리(화면에 총알 이펙트 생성)
///				1. Attack index로 공격자 판별 : CAtumApplication::FieldSocketBattleAttackOk
///				3. 1,2형 구분 ( this function )
///				2. AttackType으로 공격방법 판별 : CUnitData::CheckAttack
///				4. Target 처리(아이템,유닛) : Weapon constructor
/// \author		dhkwon
/// \date		2004-09-07 ~ 2004-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateWeaponByFieldServer( MSG_FC_BATTLE_ATTACK_OK* pMsg )
{
	if(IS_PRIMARY_ATT_TYPE(pMsg->AttackType))
	{// 1형
		m_PrimaryAttack.AttackData = *pMsg;

		// 2004-10-26 by jschoi
		// 내가 발사한 1형 무기는 발사 시점에서 이미 총알을 만들었으므로 더이상 이곳에서 생성하지 않는다.
//		if( m_dwPartType == _SHUTTLE || 
		if(	m_dwPartType == _ENEMY || 
			m_dwPartType == _ADMIN)
		{
			CheckAttack( m_PrimaryAttack, TRUE, NULL );			
		}
		else if(m_dwPartType != _SHUTTLE) // monster
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			if(pWeaponITEM)
			{
				CheckAttack( m_PrimaryAttack, TRUE, pWeaponITEM);
			}
			else
			{
				DBGOUT("Primary Monster Weapon Not DB(%d)\n", pMsg->ItemNum);
			}
		}
	}
	else if(IS_SECONDARY_ATT_TYPE(pMsg->AttackType))
	{// 2형
		// 2005-08-25 by ispark
		// 최소 프레임시 나에게 해당 하지 않는 2형무기들은 삭제
		// 2006-09-27 by ispark, 스크린샷용이 아니라면 밑에 조건문 실행
//#ifndef _SCREENSHOT
		if((g_pSOption->sLowQuality ||
			g_pD3dApp->GetFPS() < 20.0f) &&
			(pMsg->TargetInfo.TargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex 
			&& m_nTargetIndex != g_pShuttleChild->m_myShuttleInfo.ClientIndex
			&& m_dwPartType != _SHUTTLE
			&& !pMsg->SkillNum// 스킬이 있는 2형무기는 무조건 보인다.// 2008-03-14 by bhsohn 공폭 스킬 안보이는 문제 수정
			))
		{
			return;
		}
//#endif
		m_SecondaryAttack.AttackData = *pMsg;

		if( m_dwPartType == _SHUTTLE || 
			m_dwPartType == _ENEMY || 
			m_dwPartType == _ADMIN)
		{
			CheckAttack( m_SecondaryAttack, TRUE, NULL);
		}
		else // monster
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			if(pWeaponITEM)
			{
				CheckAttack( m_SecondaryAttack, TRUE, pWeaponITEM );
			}
			else
			{
				DBGOUT("Secondary Monster Weapon Not DB(%d)\n", pMsg->ItemNum);
			}
		}
	}
	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	else if( ATT_TYPE_PET_GENERAL == pMsg->AttackType )
	{
		// 2010-06-15 by shcho&hslee 펫시스템
		m_PetAttack.AttackData = *pMsg;

		if(	m_dwPartType == _ENEMY || 
			m_dwPartType == _ADMIN)
		{
			CheckAttack( m_PetAttack, TRUE, NULL );			
		}
		else if(m_dwPartType != _SHUTTLE) // monster
		{
			ITEM* pWeaponITEM = g_pDatabase->GetServerItemInfo(pMsg->ItemNum);
			if(pWeaponITEM)
			{
				CheckAttack( m_PetAttack, TRUE, pWeaponITEM);
			}
			else
			{
				DBGOUT("Pet Not DB(%d)\n", pMsg->ItemNum);
			}
		}
	}
	// end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	#ifdef _DEBUG
	else
	{
		DBGOUT("ATTACK TYPE Is Wrong.\n");//ATTACK TYPE 잘못됨.\n
	}
	#endif // _DEBUG_endif
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreatePrimaryWeapon( CUnitData* pTarget, 
///									 ITEM* pItem,
///									 MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
///									 MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
/// \brief		1형 무기를 생성해서, Scene의 m_pWeaponData에 붙인다.
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	pPrimary와 pItemPrimary는 둘중 하나는 NULL이야 한다. 둘다 NULL은 안된다.
///				둘중 하나가 NULL이 아니면 Protocol에 의한 처리이고,
///				하나라도 NULL이라면 클라이언트 처리이다.
///				ATTACK_RESULT, ATTACK_ITEM_RESULT 를 구분할것.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
/*
void CUnitData::CreatePrimaryWeapon( CUnitData* pTarget, 
									 ITEM* pItem,
									 MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
									 MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
{
	FLOG("CUnitData::CreatePrimaryWeapon( CUnitData* pTarget, ...");
	ASSERT_ASSERT( (pPrimary && !pItemPrimary) || (!pPrimary && pItemPrimary) );
	switch(pItem->OrbitType)
	{
	case ORBOT_SINGLE_CROSS_FIRE:
	case ORBIT_SINGLE_FIRE_200:
	case ORBIT_SINGLE_FIRE_250:
	case ORBIT_SINGLE_FIRE_300:
	case ORBIT_SINGLE_FIRE_320:
	case ORBIT_SINGLE_FIRE_350:
		{
			CWeaponFastData * pWeapon = CREATE_PRIMARY_SINGLE_WEAPON(pPrimary, pItemPrimary);
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
		}
		break;
	case ORBIT_TWIN_CROSS_FIRE:
	case ORBIT_TWIN_FIRE_200:
	case ORBIT_TWIN_FIRE_250:
	case ORBIT_TWIN_FIRE_300:
	case ORBIT_TWIN_FIRE_320:
	case ORBIT_TWIN_FIRE_350:
		{
			CWeaponFastData * pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 0);			
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 1);			
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
		}
		break;
	case ORBIT_CROSS_FIRE:
		{
			int nWeaponIndex = pPrimary ? pPrimary->WeaponIndex : pItemPrimary->WeaponIndex;
			m_PrimaryAttack.bZigZagWeapon = !m_PrimaryAttack.bZigZagWeapon;
			if(m_PrimaryAttack.bZigZagWeapon)
			{
				CWeaponFastData * pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 0);			
				g_pScene->m_pWeaponData->AddChild(pWeapon);
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else
			{
				CWeaponFastData * pWeapon = CREATE_PRIMARY_DOUBLE_WEAPON(pPrimary, pItemPrimary, 1);			
				g_pScene->m_pWeaponData->AddChild(pWeapon);
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_2);
			}
		}
		break;
	case ORBIT_FIRE:
		{
			CWeaponFireData * pWeapon = CREATE_PRIMARY_FIRE_WEAPON_BY_ITEM_INDEX(pPrimary, pItemPrimary);			
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
		}
		break;
	case ORBIT_ATTACK_ALL:
		{
			int nItemNum = 0;
			if(pPrimary)
				nItemNum = pPrimary->WeaponItemNumber;
			else if(pItemPrimary)
				nItemNum = pItemPrimary->WeaponItemNumber;
			CWeaponAllAttackData* pWeapon = new CWeaponAllAttackData(this, 
																	NULL, 
																	BODYCON_FIRE_MASK, 
																	nItemNum );
			g_pScene->m_pWeaponData->AddChild(pWeapon);
		}
		break;
	default:
		{
			DBGOUT("\n\nOrbitType이 잘못됨(ItemNum:%d, OrbitType:%d)\n\n", pItem->ItemNum, pItem->OrbitType);
			return;
		}
	}
}

*/
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		1형 무기의 바디컨디션을 리셋한다. (ResetBodycondition : 현재진행중인 바디컨디션이면
///				애니메이션을 다시 시작한다.)
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo)
	{
		m_pWeapon1_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_1_2 && m_pWeapon1_1_2->m_pCharacterInfo)
	{
		m_pWeapon1_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo)
	{
		m_pWeapon1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		1형 무기의 바디컨디션을 교체한다. (ChangeBodycondition : 현재진행중인 바디컨은 그냥 둔다.)
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ChangePrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo)
	{
		m_pWeapon1_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_1->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_1_2 && m_pWeapon1_1_2->m_pCharacterInfo)
	{
		m_pWeapon1_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo)
	{
		m_pWeapon1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		2형 무기의 바디컨디션을 리셋한다. (ResetBodycondition : 현재진행중인 바디컨디션이면
///				애니메이션을 다시 시작한다.)
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon, BOOL i_bIsLeft/*=TRUE*/)
{
	FLOG("CUnitData::ResetSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)");

	if(i_bIsLeft && m_pWeapon2_1_1 && m_pWeapon2_1_1->m_pCharacterInfo)
	{
		m_pWeapon2_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(FALSE == i_bIsLeft && m_pWeapon2_1_2 && m_pWeapon2_1_2->m_pCharacterInfo)
	{
		m_pWeapon2_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon2_2 && m_pWeapon2_2->m_pCharacterInfo)
	{
		m_pWeapon2_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
//	DBGOUT("CUnitData::ResetSecondaryWeaponBodyCondition(%016I64X)\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		2형 무기의 바디컨디션을 교체한다. (ChangeBodycondition : 현재진행중인 바디컨은 그냥 둔다.)
/// \author		dhkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning	사용되지 않음
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)
{
	FLOG("CUnitData::ChangeSecondaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon2_1_1 && m_pWeapon2_1_1->m_pCharacterInfo)
	{
		m_pWeapon2_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_1->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon2_1_2 && m_pWeapon2_1_2->m_pCharacterInfo)
	{
		m_pWeapon2_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_1_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon2_2 && m_pWeapon2_2->m_pCharacterInfo)
	{
		m_pWeapon2_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon2_2->m_pCharacterInfo->ChangeBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	DBGOUT("CUnitData::ChangeSecondaryWeaponBodyCondition(%016I64X)\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ResetPetWeaponBodyCondition( BodyCond_t hyBodyCon)
/// \brief		펫 무기의 바디컨디션을 리셋한다. (ResetBodycondition : 현재진행중인 바디컨디션이면
///				애니메이션을 다시 시작한다.)
/// \author		shcho&hslee
/// \date		2010-06-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetPetWeaponBodyCondition(BodyCond_t hyBodyCon, BOOL i_bIsLeft/* =TRUE */)
{
	FLOG("CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pDummyPartner && m_pDummyPartner->m_pCharacterInfo && i_bIsLeft)
	{
		m_pDummyPartner->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pDummyPartner->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pDummyPartner1 && m_pDummyPartner1->m_pCharacterInfo && i_bIsLeft)
	{
		m_pDummyPartner1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pDummyPartner1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
}

/*
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateSecondaryWeaponByFieldServer( CUnitData* pTarget, 
///												  MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY *pPrimary, 
///												  MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pItemPrimary  )
/// \brief		Field server에서 Secondary weapon생성의 허락이 떨어졌을 경우 생성하는 코드
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	pPrimary, pItemPrimary 둘중 하나만 NULL이 아니어야 한다.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateSecondaryWeaponByFieldServer( CUnitData* pTarget, 
												    MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY *pPrimary, 
												    MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pItemPrimary  )
{
	FLOG("CUnitData::CreateSecondaryWeaponByFieldServer(CUnitData * pTarget,...)");

	ITEM * pItem = g_pDatabase->GetServerItemInfo(MSG_SECONDARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
	if(!pItem)
	{
		DBGOUT("서버에 아이템 정보 요청으로 무기 생성 안됨(index:%d)\n",MSG_SECONDARY_WEAPONITEMNUMBER(pPrimary,pItemPrimary));
		return;
	}
	CreateSecondaryWeapon( pTarget, pItem, pPrimary, pItemPrimary );
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CreateSecondaryWeapon( CUnitData* pTarget, 
//									 ITEM* pItem,
///									 MSG_FC_BATTLE_ATTACK_RESULT_PRIMARY *pPrimary, 
///									 MSG_FC_BATTLE_ATTACK_ITEM_RESULT_PRIMARY* pItemPrimary  )
/// \brief		2형 무기를 생성해서, Scene의 m_pWeaponData에 붙인다.
/// \author		dhkwon
/// \date		2004-03-20 ~ 2004-03-20
/// \warning	pPrimary와 pItemPrimary는 둘중 하나는 NULL이야 한다. 둘다 NULL은 안된다.
///				ATTACK_RESULT, ATTACK_ITEM_RESULT 를 구분할것.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CreateSecondaryWeapon( CUnitData* pTarget, 
									   ITEM* pItem,
									   MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY *pPrimary, 
									   MSG_FC_BATTLE_ATTACK_ITEM_RESULT_SECONDARY* pItemPrimary  )
{
	FLOG("CUnitData::CreateSecondaryWeapon( CUnitData* pTarget, ");
	ASSERT_ASSERT( g_pScene && g_pScene->m_pWeaponData && ((pPrimary && !pItemPrimary) || (!pPrimary && pItemPrimary)) );
	CWeapon* pWeapon = NULL;
	switch(pItem->OrbitType)
	{
	case ORBIT_STRAIGHT_ROCKET_250:
	case ORBIT_FALL_ROCKET_250:
	case ORBIT_LEFT_ROCKET_200:
	case ORBIT_RIGHT_ROCKET_200:
	case ORBIT_THREE_ROCKET:
	case ORBIT_CHILD_ROCKET:
		{
			CWeaponRocketData *pRocketWeapon = CREATE_SECONDARY_WEAPON_ROCKET(pPrimary, pItemPrimary);
			pRocketWeapon->m_pItemData = pItem;
			pRocketWeapon->InitData();
			pWeapon = (CWeapon*)pRocketWeapon;
		}
		break;
	case ORBIT_STRAIGHT_MISSILE_300:
	case ORBIT_FALL_MISSILE_300:
	case ORBIT_LEFT_MISSILE_300:
	case ORBIT_RIGHT_MISSILE_270:
	case ORBIT_UPDOWN_ROCKET_270:
	case ORBIT_UPDOWN_MISSILE_300:
		{
			CWeaponMissileData *pMissileWeapon = CREATE_SECONDARY_WEAPON_MISSILE(pPrimary, pItemPrimary);
			pMissileWeapon->m_pItemData = pItem;
			pMissileWeapon->InitData();
			pWeapon = (CWeapon*)pMissileWeapon;
		}
		break;
	default:
		{
			DBGOUT("\n\nOrbitType이 잘못됨(ItemNum:%d, OrbitType:%d)\n\n", pItem->ItemNum, pItem->OrbitType);
			return;
		}
	}
	g_pScene->m_pWeaponData->AddChild(pWeapon);
	ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
}
*/

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//void CUnitData::CreateWeapon(ATTACK_DATA& attackData, ITEM* pWeaponITEM)
//void CUnitData::CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
void CUnitData::CreateWeapon( ATTACK_DATA& attackData, ITEM* pWeaponITEM, ITEM* pEffectItem /* = NULL */, int LoadingPriority )
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
{
	if(!pWeaponITEM)
		return;										//2013-06-03 by ssjung Canada 익셉션 안전코드 추가 

	// 2013-08-06 by bhsohn 캐나다 안전코드 추가
	if(!g_pScene || !g_pScene->m_pWeaponData)
	{
		return;
	}
	// END 2013-08-06 by bhsohn 캐나다 안전코드 추가
 	CWeapon* pWeapon = NULL;
	switch(pWeaponITEM->OrbitType)
	{
	case ORBIT_OBJBEAM:
	case ORBIT_SINGLE_CROSS_FIRE:
	case ORBIT_SINGLE_FIRE_100:
	case ORBIT_SINGLE_FIRE_150:
	case ORBIT_SINGLE_FIRE_200:
	case ORBIT_SINGLE_FIRE_250:
	case ORBIT_SINGLE_FIRE_300:
	case ORBIT_SINGLE_FIRE_320:
	case ORBIT_SINGLE_FIRE_350:
		{
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
			//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
		}
		break;
	case ORBIT_TWIN_CROSS_FIRE:
	case ORBIT_TWIN_FIRE_100:
	case ORBIT_TWIN_FIRE_150:
	case ORBIT_TWIN_FIRE_200:
	case ORBIT_TWIN_FIRE_250:
	case ORBIT_TWIN_FIRE_300:
	case ORBIT_TWIN_FIRE_320:
	case ORBIT_TWIN_FIRE_350:
		{
			attackData.bZigZagWeapon = FALSE;

 			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경


			g_pScene->m_pWeaponData->AddChild(pWeapon);
			attackData.bZigZagWeapon = TRUE;
			if( attackData.AttackData.SkillNum == 0 )
			{
				
			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

				g_pScene->m_pWeaponData->AddChild(pWeapon);
				// 2010-06-15 by shcho&hslee 펫시스템 - 오버히트 공격 처리
				//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
				if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
				{
					ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
				}
				else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
				{
					ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
				}
				// end 2010-06-15 by shcho&hslee 펫시스템 - 오버히트 공격 처리
				
			}
			else
			{
				// 다시 볼것. by dhkwon, 2006-10-19
				if(m_dwPartType == _SHUTTLE && 
					attackData.AttackData.AttackIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
				{
					// 스킬이 있는 총알 발사시에는 ReattackTime동안 총알을 쏠수 없도록 한다.
					g_pShuttleChild->m_pPrimaryWeapon->SetReattackCheckTime(0);
					g_pShuttleChild->m_pPrimaryWeapon->SetAttackCount(0);
				}
				attackData.AttackData.SkillNum = 0;
			}
		}
		break;
	case ORBIT_CROSS_FIRE:
		{
			attackData.bZigZagWeapon = !attackData.bZigZagWeapon;
 			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			if(attackData.bZigZagWeapon)
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, attackData.bZigZagWeapon);
//				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_2);
			}
			else
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, attackData.bZigZagWeapon);
			}
			// 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가

			if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
		}
		break;
	case ORBIT_FIRE:
		{
 			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
			//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
		}
		break;
	case ORBIT_BODYSLAM:
		{
 			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponFastData(this, pWeaponITEM, attackData);
			//pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			pWeapon = new CWeaponFastData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
			//ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			if( IS_PRIMARY_ATT_TYPE(attackData.AttackData.AttackType) )
			{
				ResetPrimaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			else if(attackData.AttackData.AttackType == ATT_TYPE_PET_GENERAL)
			{
				ResetPetWeaponBodyCondition(WEAPON_BODYCON_FIRE_1);
			}
			// end 2010-06-15 by shcho&hslee 펫시스템 - 무기 이팩트 추가
		}
		break;
	case ORBIT_ATTACK_ALL:
		{
			int nItemNum = attackData.AttackData.ItemNum;

			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//pWeapon = new CWeaponAllAttackData(this, NULL, BODYCON_FIRE_MASK, nItemNum);
			//pWeapon = new CWeaponAllAttackData( this, NULL, BODYCON_FIRE_MASK, nItemNum, pEffectItem );
			pWeapon = new CWeaponAllAttackData( this, NULL, BODYCON_FIRE_MASK, nItemNum, pEffectItem, LoadingPriority);
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			g_pScene->m_pWeaponData->AddChild(pWeapon);
		}
		break;
	case ORBIT_STRAIGHT_ROCKET_250:
	case ORBIT_FALL_ROCKET_250:
	case ORBIT_LEFT_ROCKET_200:
	case ORBIT_RIGHT_ROCKET_200:
	case ORBIT_THREE_ROCKET:
	case ORBIT_CHILD_ROCKET:
		{
			attackData.bZigZagWeapon = !attackData.bZigZagWeapon;
			if(attackData.AttackData.FirePosition.x < 0)
			{
				attackData.bZigZagWeapon = TRUE;
			}
			else if(attackData.AttackData.FirePosition.x > 0)
			{
				attackData.bZigZagWeapon = FALSE;
			}
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);

			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData(this, pWeaponITEM, attackData);
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem);
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem, LoadingPriority);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경


			pWeapon = (CWeapon*)pWeaponRocketData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
//			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);
			// 2006-10-09 by ispark, 로켓에서 bZigZagWeapon값은 위 생성과 반대 반향이다.
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, !attackData.bZigZagWeapon);
//			DBGOUT("지그재그 %d\n", attackData.bZigZagWeapon);
			// 2006-10-19 by ispark, 개수 체크
			// 다시 한번 볼것. by dhkwon, 2006-10-19
			if(m_dwPartType == _SHUTTLE &&
				g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0)
			{
				ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD, !attackData.bZigZagWeapon);
			}
		}
		break;
	case ORBIT_STRAIGHT_MISSILE_300:
	case ORBIT_FALL_MISSILE_300:
	case ORBIT_LEFT_MISSILE_300:
	case ORBIT_RIGHT_MISSILE_270:

//	case ORBIT_UPDOWN_MISSILE_300:
		{
			attackData.bZigZagWeapon = !attackData.bZigZagWeapon;
			if(attackData.AttackData.FirePosition.x < 0)
			{
				attackData.bZigZagWeapon = FALSE;
			}
			else if(attackData.AttackData.FirePosition.x > 0)
			{
				attackData.bZigZagWeapon = TRUE;
			}
			attackData.fExplosionRange = CAtumSJ::GetExplosionRange(pWeaponITEM, &g_pShuttleChild->m_paramFactor);
			// 2007-06-15 by dgwoo 아래 함수로 만들것.
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);
			//pWeaponITEM->RepeatTime * (1.0f+m_paramFactor.pfm_WARHEAD_SPEED);

			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData(this, pWeaponITEM, attackData);
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem,LoadingPriority);	
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			pWeapon = (CWeapon*)pWeaponMissileData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			// 2006-10-09 by ispark, 미사일에서 WEAPON_BODYCON_FIRE_1는 의미가 없다.
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);

			// 2006-10-19 by ispark, 개수 체크
			// 다시 한번 볼것, 2006-10-19 by dhkwon
			if(m_dwPartType == _SHUTTLE &&
				g_pShuttleChild->m_pSecondaryWeapon->GetItemGeneral()->CurrentCount <= 0)
			{
				ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD);
				ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_RELOAD, FALSE);
			}
		}
		break;
	case ORBIT_UPDOWN_ROCKET_270:
		{
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);

			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData(this, pWeaponITEM, attackData);
			//CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem);
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			CWeaponRocketData* pWeaponRocketData = new CWeaponRocketData( this, pWeaponITEM, attackData, pEffectItem);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			pWeapon = (CWeapon*)pWeaponRocketData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);
		}
		break;
	case ORBIT_UPDOWN_MISSILE_300:
		{
			// 2006-08-14 by ispark, 로켓형과 같이 쓰이는 몹이 있다. 그러므로 임시 저장후 다시 셋팅
			BOOL bSaveZigZagTemp = attackData.bZigZagWeapon;
			attackData.bZigZagWeapon = 0;
			attackData.fWarheadSpeed = CAtumSJ::GetWarHeadSpeed(pWeaponITEM,&m_paramFactor);

			// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
			// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData(this, pWeaponITEM, attackData);
			//CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem );
			// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
			CWeaponMissileData *pWeaponMissileData = new CWeaponMissileData( this, pWeaponITEM, attackData, pEffectItem);
			//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

			pWeapon = (CWeapon*)pWeaponMissileData;
			g_pScene->m_pWeaponData->AddChild(pWeapon);
			ResetSecondaryWeaponBodyCondition(WEAPON_BODYCON_FIRE_1, (attackData.AttackData.FirePosition.x<0)?TRUE:FALSE);
			// 다시 셋팅
			attackData.bZigZagWeapon = bSaveZigZagTemp;
		}
		break;
	default:
		{
			DBGOUT("\n\nOrbitType is Wrong(ItemNum:%d, OrbitType:%d)\n\n", pWeaponITEM->ItemNum, pWeaponITEM->OrbitType);
			return;
		}
	}
}

float CUnitData::GetAutomaticAttackTime( BYTE nOrbitType )
{
	switch(nOrbitType)
	{
	case ORBIT_SINGLE_FIRE_100:
	case ORBIT_TWIN_FIRE_100:
		return 0.1f;
	case ORBIT_TWIN_FIRE_150:
	case ORBIT_SINGLE_FIRE_150:
		return 0.15f;
	case ORBIT_SINGLE_FIRE_200:
	case ORBIT_TWIN_FIRE_200:
	case ORBIT_LEFT_ROCKET_200:
	case ORBIT_RIGHT_ROCKET_200:
		return 0.2f;
	case ORBIT_SINGLE_FIRE_250:
	case ORBIT_TWIN_FIRE_250:
	case ORBIT_STRAIGHT_ROCKET_250:
	case ORBIT_FALL_ROCKET_250:
		return 0.25f;
	case ORBIT_RIGHT_MISSILE_270:
	case ORBIT_UPDOWN_ROCKET_270:
		return 0.27f;
	case ORBIT_SINGLE_FIRE_300:
	case ORBIT_STRAIGHT_MISSILE_300:
	case ORBIT_FALL_MISSILE_300:
	case ORBIT_LEFT_MISSILE_300:
	case ORBIT_TWIN_FIRE_300:
	case ORBIT_UPDOWN_MISSILE_300:
	case ORBIT_MINE_300:
	case ORBIT_RANGE_ROCKET_300:
		return 0.3f;
	case ORBIT_SINGLE_FIRE_320:
	case ORBIT_TWIN_FIRE_320:
		return 0.32f;
	case ORBIT_SINGLE_FIRE_350:
	case ORBIT_TWIN_FIRE_350:
		return 0.35f;
	case ORBIT_MINE_400:
	case ORBIT_RANGE_ROCKET_400:
		return 0.4f;
	default:
		return 0.3f;
	}
}

/*
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)
/// \brief		Primary attack을 체크하여 무기 생성
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	CShuttleChild, CEnemy, CMonster
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)
{
	FLOG("CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)");

	ASSERT_ASSERT(pPrimary);
	if(bReset)
	{
		if( IS_CLIENT_SET_AUTOMATIC_TIMER(pPrimary->OrbitType) )
		{
			m_PrimaryAttack.bShootingWeapon = TRUE;
		}
		else
		{
			m_PrimaryAttack.nAttackCount = CAtumSJ::GetTotalPrimaryShotCountPerReattackTime(pPrimary, &m_paramFactor);
			m_fPrimaryAttackTime = CAtumSJ::GetShotCountReattackTime( pPrimary, &m_paramFactor );
		}
		//return;m_bPrimaryWeaponPrepare,m_bPrimaryAttackOrder
	}
	if(IS_CLIENT_SET_AUTOMATIC_TIMER(pPrimary->OrbitType))
	{
//		m_fAutoMaticTimer -= g_pD3dApp->GetElapsedTime();
		if(m_PrimaryAttack.fCheckAutoMaticTimer <= 0.0f && 
			m_PrimaryAttack.bShootingWeapon && 
			m_PrimaryAttack.nAttackCount <= 0)
		{
			m_PrimaryAttack.nAttackCount = CAtumSJ::GetTotalPrimaryShotCountPerReattackTime(pPrimary, &m_paramFactor);
			m_fPrimaryAttackTime = GetPrimaryAttackTime(pPrimary->OrbitType);
			m_PrimaryAttack.fCheckAutoMaticTimer = (float)CAtumSJ::GetReattackTime(pPrimary, &m_paramFactor)/1000.0f;
		}
	}
//	if(m_fCheckPrimaryAttackTime > 0.0f)
//		m_fCheckPrimaryAttackTime -= g_pD3dApp->GetElapsedTime();
	if(m_PrimaryAttack.nAttackCount>0 && m_PrimaryAttack.fCheckAttackTime <= 0.0f)
	{
		m_PrimaryAttack.fCheckAttackTime = m_fPrimaryAttackTime;
		if(!m_bIsItemAttack)
		{
			CreatePrimaryWeapon( (CUnitData *)m_pPrimaryAttackTarget, pPrimary, &m_PrimaryAttackData1, NULL );
		}
		else
		{
			CreatePrimaryWeapon( (CUnitData *)m_pPrimaryAttackTarget, pPrimary, NULL, &m_PrimaryAttackData2 );
		}
		m_PrimaryAttack.nAttackCount--;
		if(m_PrimaryAttack.nAttackCount == 0)
		{
			m_PrimaryAttack.bShootingWeapon = FALSE;
		}
	}
}
*/
USHORT CUnitData::GetShiftWeaponBodyconditionByUnitKind()
{
	FLOG("CUnitData::GetShiftWeaponBodyconditionByUnitKind()");
	return UNIT_KIND_SHIFT_DEFAULT;
}


void CUnitData::DeleteItemPoint()
{
	FLOG( "CUnitData::DeleteItemPoint()" );
//	if(m_pWeapon1_1)
//	{
//		m_pWeapon1_1->m_bUsing = FALSE;
//		m_pWeapon1_1 = NULL;
//	}
	if(m_pWeapon1_1_1)
	{
		m_pWeapon1_1_1->m_bUsing = FALSE;
		m_pWeapon1_1_1 = NULL;
	}
	if(m_pWeapon1_1_2)
	{
		m_pWeapon1_1_2->m_bUsing = FALSE;
		m_pWeapon1_1_2 = NULL;
	}
	if(m_pWeapon1_2)
	{
		m_pWeapon1_2->m_bUsing = FALSE;
		m_pWeapon1_2 = NULL;
	}
//	if(m_pWeapon2_1)
//	{
//		m_pWeapon2_1->m_bUsing = FALSE;
//		m_pWeapon2_1 = NULL;
//	}
	if(m_pWeapon2_1_1)
	{
		m_pWeapon2_1_1->m_bUsing = FALSE;
		m_pWeapon2_1_1 = NULL;
	}
	if(m_pWeapon2_1_2)
	{
		m_pWeapon2_1_2->m_bUsing = FALSE;
		m_pWeapon2_1_2 = NULL;
	}
	if(m_pWeapon2_2)
	{
		m_pWeapon2_2->m_bUsing = FALSE;
		m_pWeapon2_2 = NULL;
	}

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	if( m_pRobotAGearWeapon_1 )
	{
		m_pRobotAGearWeapon_1->m_bUsing		= FALSE;
		m_pRobotAGearWeapon_1				= NULL;
	}
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)


	if(m_pEngine)
	{
		m_pEngine->m_bUsing = FALSE;
		m_pEngine = NULL;
	}
	if(m_pRadar)
	{
		m_pRadar->m_bUsing = FALSE;
		m_pRadar = NULL;
	}
//	if(m_pArmor)
//	{
//		m_pArmor->m_bUsing = FALSE;
//		m_pArmor = NULL;
//	}
	if(m_pContainer)
	{
		m_pContainer->m_bUsing = FALSE;
		m_pContainer = NULL;
	}
	if(m_pAccessories)
	{
		m_pAccessories->m_bUsing = FALSE;
		m_pAccessories = NULL;
	}
	if(m_pWingIn)
	{
		m_pWingIn->m_bUsing = FALSE;
		m_pWingIn = NULL;
	}
	if(m_pPartner)
	{
		m_pPartner->m_bUsing = FALSE;
		m_pPartner = NULL;
	}
    // 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
	if(m_pPartner1)
	{
		m_pPartner1->m_bUsing = FALSE;
		m_pPartner1 = NULL;
	}

	if(m_pDummyPartner)
	{
		m_pDummyPartner->m_bUsing = FALSE;
		m_pDummyPartner = NULL;
	}
	if(m_pDummyPartner1)
	{
		m_pDummyPartner1->m_bUsing = FALSE;
		m_pDummyPartner1 = NULL;
	}
    // end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
}

// 2010. 03. 18 by jskim 몬스터변신 카드
//void CUnitData::ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter)
void CUnitData::ChangeUnitCharacterInfo(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, BOOL bMonsterTransformer)
//end 2010. 03. 18 by jskim 몬스터변신 카드
{
	int nTemp;
//	if(m_dwPartType == _ADMIN)
//	{
//		nTemp = 10000000;
//	}
//	else
	// 2010. 03. 18 by jskim 몬스터변신 카드
//	{
//		nTemp = ::GetUnitNum( m_bDegree, nDefenseItemNum, nUnitKind, bCharacter);
//	}
	if(!bMonsterTransformer)
	{
		nTemp = ::GetUnitNum( m_bDegree, nDefenseItemNum, nUnitKind, bCharacter);
	}
	else
	{
		nTemp = nDefenseItemNum;
	}
	//end 2010. 03. 18 by jskim 몬스터변신 카드
	if(nTemp != m_nUnitNum)
	{
   		m_nUnitNum = nTemp;
		char strFileName[16];
		wsprintf( strFileName, "%08d", m_nUnitNum );
		// 2010. 03. 18 by jskim 몬스터변신 카드
		//LoadCharacterEffect(strFileName);
		LoadCharacterEffect(strFileName, m_MonsterTransformer, m_MonsterTransScale);
		//end 2010. 03. 18 by jskim 몬스터변신 카드
		if(m_pCharacterInfo)
		{
			// 2010. 03. 18 by jskim 몬스터변신 카드
			if(bMonsterTransformer)
			{
				m_pCharacterInfo->m_MonsterTransformer = m_MonsterTransformer;
				m_pCharacterInfo->m_MonsterTransScale  = m_MonsterTransScale;
 			}
			//end 2010. 03. 18 by jskim 몬스터변신 카드
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->SetCharacterAnimationBodyConditionMask(BODYCON_CHARACTER_ANIMATION_TIME);
			m_pCharacterInfo->SetBodyCondition(nBodyCon);
			// 2006-01-13 by ispark
			if(bCharacter == FALSE && !bMonsterTransformer)
			{
			// 2010. 03. 18 by jskim 몬스터변신 카드
				//SetWeaponPosition();
				SetPrimaryWeaponPosition();
				SetSecondaryWeaponPosition();
				SetPetPosition();
				SetMarkPosition(); // 2011-03-21 by jhAhn 마크시스템 마크 위치 생성
			}
			//end 2010. 03. 18 by jskim 몬스터변신 카드
		}
	}
}

int CUnitData::GetUnitNumFromCharacter(int nDefenseItemNum, int nUnitKind, BodyCond_t nBodyCon, BOOL bCharacter, int nDegree)
{
	int nTemp = 0;
//	if(m_dwPartType == _ADMIN)
//	{
//		nTemp = 10000000;
//	}
//	else
	{
		nTemp = ::GetUnitNum( nDegree, nDefenseItemNum, nUnitKind, bCharacter);
	}

	return nTemp;
	
}

HRESULT CUnitData::InitDeviceObjects()
{
	// skill effect 생성
	m_pSkillEffect = new CSkillEffect(this);
	m_pSkillEffect->InitDeviceObjects();

	m_pChaffData.clear();

	return S_OK;
}

HRESULT CUnitData::RestoreDeviceObjects()
{
	// shadow
	RestoreShadow();
	// skill effect
	m_pSkillEffect->RestoreDeviceObjects();
	return S_OK;
}

HRESULT CUnitData::InvalidateDeviceObjects()
{
	// shadow
	InvalidateShadow();
	// skill effect
	m_pSkillEffect->InvalidateDeviceObjects();

	// 삭제
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		util::del(*it);
		it++;
	}
	m_pChaffData.clear();
	return S_OK;
}

HRESULT CUnitData::DeleteDeviceObjects()
{
 	m_pSkillEffect->DeleteDeviceObjects();
	util::del(m_pSkillEffect);

	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		util::del(*it);
		it++;
	}
	m_pChaffData.clear();

	return S_OK;
}

HRESULT CUnitData::RestoreShadow()
{
	DWORD	dwColor = 0xFFFFFFFF;
	if( FAILED( g_pD3dDev->CreateVertexBuffer( 4*sizeof( SPRITEVERTEX ),
		0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &m_pVBShadow,NULL ) ) )
	{
		g_pD3dApp->CheckError(CLIENT_ERR_SHUTTLE_EFFECTRESTORE);
		return E_FAIL;
	}
	SPRITEVERTEX* v;
	m_pVBShadow->Lock( 0, 0, (void**)&v, 0 );
	v[0].p = D3DXVECTOR3(0,0,0);  v[0].color=dwColor;	v[0].tu=0; v[0].tv=1;	
	v[1].p = D3DXVECTOR3(0,0,0);  v[1].color=dwColor;	v[1].tu=0; v[1].tv=0;	
	v[2].p = D3DXVECTOR3(0,0,0);  v[2].color=dwColor;	v[2].tu=1; v[2].tv=1;	
	v[3].p = D3DXVECTOR3(0,0,0);  v[3].color=dwColor;	v[3].tu=1; v[3].tv=0;	
	m_pVBShadow->Unlock();
	return S_OK;
}

HRESULT CUnitData::InvalidateShadow()
{
	SAFE_RELEASE(m_pVBShadow);
	return S_OK;
}

void CUnitData::SetPkState( DWORD dwPkFlag, BOOL bSet )
{
	if(bSet)
	{
		m_dwPkState |= dwPkFlag;
	}
	else
	{
		m_dwPkState &= ~dwPkFlag;
	}
#ifdef _DEBUG
	if(m_dwPartType == _SHUTTLE)
	{
		DBGOUT("ShuttleChild PK Setting(%d)(0x%08x)\n", bSet, m_dwPkState);
	}
	else if(m_dwPartType == _ENEMY)
	{
		DBGOUT("Enemy(%s) PK Setting(%d)(0x%08x)\n", ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.CharacterName, bSet, m_dwPkState);
	}
#endif
}

BOOL CUnitData::IsPkState( DWORD dwPkFlag )
{
	if(m_dwPkState & dwPkFlag )
	{
		return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pPrimary)
/// \brief		1,2형 CheckAttack
/// \author		dhkwon
/// \date		2004-09-07 ~ 2004-09-07
/// \warning	참골치아픈 함수
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
//void CUnitData::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM)
void CUnitData::CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM/* = NULL*/, ITEM* pEffectItem/* = NULL*/)
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
{
	FLOG("CUnitData::CheckPrimaryAttack(BOOL bReset, ITEM* pPrimary)");

	if(pWeaponITEM)											//2013-06-03 by ssjung Canada 익셉션 안전코드 추가 
		ASSERT_ASSERT(pWeaponITEM);

	if(bReset && attackData.bShootingWeapon==FALSE)
	{
		attackData.bShootingWeapon = TRUE;
		if(pWeaponITEM && IS_CLIENT_SET_AUTOMATIC_TIMER(pWeaponITEM->OrbitType) == FALSE)			//2013-06-03 by ssjung Canada 익셉션 안전코드 추가 
		{
			attackData.nAttackCount = GetTotalShotNumPerReattackTime(pWeaponITEM, &m_paramFactor);
		}
	}
	if(pWeaponITEM && IS_CLIENT_SET_AUTOMATIC_TIMER(pWeaponITEM->OrbitType))						//2013-06-03 by ssjung Canada 익셉션 안전코드 추가 
	{
		if( attackData.fCheckAutoMaticTimer <= 0.0f && 
			attackData.bShootingWeapon && 
			attackData.nAttackCount <= 0)
		{
			attackData.nAttackCount = GetTotalShotNumPerReattackTime(pWeaponITEM, &m_paramFactor);
			attackData.fCheckAutoMaticTimer = (float)CAtumSJ::GetReattackTime(pWeaponITEM, &m_paramFactor)/1000.0f;
		}
	}
	if( (attackData.nAttackCount>0 && 
		attackData.fCheckAttackTime <= 0.0f &&
		attackData.bShootingWeapon==TRUE) ||
//		((m_dwPartType == _ENEMY || m_dwPartType == _SHUTTLE ) && 
//		IS_SECONDARY_WEAPON(pWeaponITEM->Kind)==TRUE))
		(bReset && IS_SECONDARY_ATT_TYPE(attackData.AttackData.AttackType)))
	{
		
		// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
		// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		//CreateWeapon( attackData, pWeaponITEM );
		int temp = _BULLET_PRIORITY;
		if(g_pShuttleChild->GetUnitNum() == m_nUnitNum)
		{
			temp = _MY_CHARACTER_PRIORITY;
		}		
		//CreateWeapon( attackData, pWeaponITEM, pEffectItem );
		// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
		CreateWeapon( attackData, pWeaponITEM, pEffectItem, temp);
		//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경

		attackData.nAttackCount--;
		if(attackData.nMultiNumCount>0)
		{
			attackData.nMultiNumCount--;
		}
		if(attackData.nAttackCount == 0)
		{
			attackData.bShootingWeapon = FALSE;
		}
		if(attackData.nMultiNumCount <= 0)
		{
			if(pWeaponITEM)							//2013-06-03 by ssjung Canada 익셉션 안전코드 추가 
			{	
				if(IS_CLIENT_SET_AUTOMATIC_TIMER(pWeaponITEM->OrbitType))
				{
					attackData.fCheckAttackTime = GetAutomaticAttackTime(pWeaponITEM->OrbitType);
				}
				else
				{
					attackData.fCheckAttackTime = CAtumSJ::GetShotCountReattackTime( pWeaponITEM, &m_paramFactor );
				}
				attackData.nMultiNumCount = GetMultiNum( pWeaponITEM, &m_paramFactor );
			}
		}
	}
}
 

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::GetTotalShotNumPerReattackTime(ITEM* pWeaponITEM, CParamFactor *pParamFactor)
/// \brief		ReattackTime 동안 발사되는 발사 수(샷넘)
/// \author		dhkwon
/// \date		2004-09-07 ~ 2004-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CUnitData::GetTotalShotNumPerReattackTime(ITEM* pWeaponITEM, CParamFactor *pParamFactor)
{
	if(IS_PRIMARY_WEAPON(pWeaponITEM->Kind))
		
		return CAtumSJ::GetTotalPrimaryShotNumPerReattackTime(pWeaponITEM, pParamFactor);

	else return CAtumSJ::GetTotalSecondaryShotNumPerReattackTime(pWeaponITEM, pParamFactor);
}

int CUnitData::GetMultiNum(ITEM* pWeaponITEM, CParamFactor *pParamFactor)
{
	if(IS_PRIMARY_WEAPON(pWeaponITEM->Kind))
	{
		return pWeaponITEM->MultiNum + pParamFactor->pfp_MULTINUM_01;
	}
	// 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성
// 	else(
// 	{
// 		return pWeaponITEM->MultiNum + pParamFactor->pfp_MULTINUM_02;
// 	}
	else if(IS_SECONDARY_WEAPON(pWeaponITEM->Kind))
	{
		return pWeaponITEM->MultiNum + pParamFactor->pfp_MULTINUM_02;
	}
	else if(pWeaponITEM->Kind == ITEMKIND_PET_ITEM)
	{
		return pWeaponITEM->MultiNum;
	}
	//end 2010-06-15 by shcho&hslee 펫시스템 - 펫 무기 생성

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::SetWeaponVelFromServer( D3DXVECTOR3 vWeaponVel )
/// \brief		Enemy,(Monster)의 WeaponVel세팅
/// \author		dhkwon
/// \date		2004-10-18 ~ 2004-10-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetWeaponVelFromServer( D3DXVECTOR3 vWeaponVel )
{
	m_vWeaponVel = vWeaponVel;
}
// 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
void CUnitData::SetPetVelFromServer( D3DXVECTOR3 vPetLeftVel, D3DXVECTOR3 vPetRightVel )
{
	m_vPetLeftVel = vPetLeftVel;
	m_vPetRightVel = vPetRightVel;
}
// end 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
void	CUnitData::ShowWeapon( const BOOL bShow )
{
	if( m_pWeapon1_1_1 )
		m_pWeapon1_1_1->m_bRender	= bShow;

	if( m_pWeapon1_1_2 )
		m_pWeapon1_1_2->m_bRender	= bShow;

	if( m_pWeapon1_2 )
		m_pWeapon1_2->m_bRender		= bShow;

	if( m_pWeapon2_1_1 )
		m_pWeapon2_1_1->m_bRender	= bShow;

	if( m_pWeapon2_1_2 )
		m_pWeapon2_1_2->m_bRender	= bShow;

	if( m_pWeapon2_2 )
		m_pWeapon2_2->m_bRender		= bShow;
}
// 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치
void CUnitData::HidePet( const BOOL bhide)
{
	if(m_pPartner)
	{
		m_pPartner->m_bRender		= !bhide;		
	}
	if(m_pDummyPartner)
	{
		m_pDummyPartner->m_bRender	= !bhide;
	}
	if(m_pPartner1)
	{
		m_pPartner1->m_bRender		= !bhide;		
	}
	if(m_pDummyPartner1)
	{
		m_pDummyPartner1->m_bRender	= !bhide;
	}
}
// end 2010-06-15 by shcho&hslee 펫시스템 - 무기 위치

void	CUnitData::CreateRobotAGearWeapon( ITEM* pAGearWeapon, const UINT nUnitType )
{
	DeleteRobotAGearWeapon();

	m_pRobotAGearWeapon_1 = CreateWearItemEffect( WEAR_ITEM_KIND_WEAPON_11, 
												  EFFECT_NUMBER_WEAR_WEAPON_11( pAGearWeapon, nUnitType ),
												  D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
												  WEAPON_BODYCON_LOADING,
												  WEAPON_POSITION_1_1_1 );
}

void	CUnitData::DeleteRobotAGearWeapon( void )
{
	DeleteWearItemEffect( m_pRobotAGearWeapon_1 );
	m_pRobotAGearWeapon_1 = NULL;
}
// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

///////////////////////////////////////////////////////////////////////////////
/// \fn			CEnemyData::DelSecondWeapon()
/// \brief		죽었을 때 자신의 2형무기 타겟 삭제(몬스터, 적기어만 해당)
/// \author		ispark
/// \date		2005-12-14 ~ 2005-12-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DelSecondWeapon()
{
	ClientIndex_t AttackIndex = 0; 
	if(m_dwPartType == _ENEMY)
	{
		AttackIndex = ((CEnemyData*)this)->m_infoCharacter.CharacterInfo.ClientIndex;
	}
	else if(m_dwPartType == _MONSTER)
	{
		AttackIndex = ((CMonsterData*)this)->m_info.MonsterIndex;
	}
	else
	{
		// 위 해당사항이 아니므로 나감
		return;
	}
	
	if(!g_pScene->m_pWeaponData)
		return;
	CWeapon * pNode = (CWeapon *)g_pScene->m_pWeaponData->m_pChild;
	while(pNode)
	{
		if( (pNode->m_dwPartType == _MISSILE || pNode->m_dwPartType == _ROCKET ) &&
			AttackIndex == ((CWSlowData*)pNode)->m_nClientIndex)
		{
			((CWSlowData*)pNode)->m_pTarget = NULL;
		}

		pNode = (CWeapon *)pNode->m_pNext;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CUnitData::ResetPrimaryWeaponBodyCondition(BodyCond_t hyBodyCon, BOOL bZig)
/// \brief		지그재그 1형 무기 설정
/// \author		ispark
/// \date		2006-01-03 ~ 2006-01-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ResetPrimaryWeaponBodyCondition(BodyCond_t hyBodyCon, BOOL bZig)
{
	FLOG("CUnitData::ResetPrimaryWeaponBodyCondition( BodyCond_t hyBodyCon)");
	if(m_pWeapon1_1_1 && m_pWeapon1_1_1->m_pCharacterInfo && bZig == TRUE)
	{
		m_pWeapon1_1_1->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_1->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
	if(m_pWeapon1_1_2 && m_pWeapon1_1_2->m_pCharacterInfo && bZig == FALSE)
	{
		m_pWeapon1_1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
		m_pWeapon1_1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//		DBGOUT("CUnitData::ResetPrimaryWeaponBodyCondition( %016I64X )\n", hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
	}
//	if(m_pWeapon1_2 && m_pWeapon1_2->m_pCharacterInfo && bZig == FALSE)
//	{
//		m_pWeapon1_2->m_bodyCondition = hyBodyCon << GetShiftWeaponBodyconditionByUnitKind();
//		m_pWeapon1_2->m_pCharacterInfo->ResetBodyCondition(hyBodyCon << GetShiftWeaponBodyconditionByUnitKind());
//	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChangeWearItemBodyCondition(int nWearItemKind, BodyCond_t hyBodyCondition)
/// \brief		1형, 2형 무기를 제외한 다른 아이템 바디컨디션 체인지
/// \author		ispark
/// \date		2006-06-28 ~ 2006-06-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChangeWearItemBodyCondition(int nWearItemKind, BodyCond_t hyBodyCondition)
{
	switch( nWearItemKind )
	{
	case WEAR_ITEM_KIND_PROW:
		{
		}
		break;
	case WEAR_ITEM_KIND_CENTER:
		{
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_UNLIMITED:
		{
			if(m_pContainer)
			{
				m_pContainer->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	case WEAR_ITEM_KIND_ENGINE:
		{
		}
		break;
	case WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT:
		{
			if(m_pAccessories)
			{
				m_pAccessories->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	case WEAR_ITEM_KIND_WINGIN:
		{
			if(m_pWingIn)
			{
				m_pWingIn->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	case WEAR_ITEM_KIND_PET:
		{
			if(m_pPartner && !COMPARE_BODYCON_BIT( m_pPartner->m_bodyCondition, hyBodyCondition ) )
			{
				m_pPartner->ChangeBodyCondition(hyBodyCondition);
			}
			if(m_pPartner1 && !COMPARE_BODYCON_BIT( m_pPartner1->m_bodyCondition, hyBodyCondition ) )
			{
				m_pPartner1->ChangeBodyCondition(hyBodyCondition);
			}
			if(m_pDummyPartner)
			{
				m_pDummyPartner->ChangeBodyCondition(hyBodyCondition);
			}
			if(m_pDummyPartner1)
			{
				m_pDummyPartner1->ChangeBodyCondition(hyBodyCondition);
			}
		}
		break;
	// 2010-06-08 by dgwoo, 펫시스템 추가. 
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::SetUnitAlpha(BYTE bySkillState, int nAlphaValue)
/// \brief		알파값 처리
/// \author		ispark
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::SetUnitAlpha(BYTE bySkillState, int nAlphaValue)
{
	m_bySkillStateFlag = bySkillState;
	m_nAlphaValue = nAlphaValue;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::InitUnitAlpha()
/// \brief		알파값 초기화
/// \author		ispark
/// \date		2006-11-21 ~ 2006-11-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::InitUnitAlpha()
{
	m_bySkillStateFlag = CL_SKILL_NONE;
	m_nAlphaValue = SKILL_OBJECT_ALPHA_NONE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::AddChaffData(CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg)
/// \brief		체프 추가
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::AddChaffData(CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg)
{
	UINT *pChaffIndex = (UINT *)((char *)pMsg + sizeof(MSG_FC_BATTLE_DROP_DUMMY_OK));
	for(int i = 0 ; i < pMsg->DummyCounts ; i++)
	{
		//CItemData *pChaff = FindChaffDataByFieldIndex( pMsg->ItemFieldIndex );
		CItemData *pChaff = FindChaffDataByFieldIndex(*pChaffIndex);
		if(!pChaff)
		{
			pChaff = new CItemData(pUnitData, pMsg->ItemNum,*pChaffIndex);
			m_pChaffData.push_back(pChaff);
		}
		pChaffIndex++;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DelChaffData(UINT nItemIndex)
/// \brief		체프 삭제
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DelChaffData(UINT nItemIndex)
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		if((*it)->m_nItemIndex == nItemIndex)
		{
			util::del(*it);
			m_pChaffData.erase(it);
			
			return;
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::DelChaffDataAll()
/// \brief		체프를 모두 삭제한다.
/// \author		dgwoo
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::DelChaffDataAll()
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		util::del(*it);
		it = m_pChaffData.erase(it);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemData* CUnitData::FindChaffDataByFieldIndex( UINT nFieldIndex )
/// \brief		체프 인덱스 검색
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemData* CUnitData::FindChaffDataByFieldIndex( UINT nFieldIndex )
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		if((*it)->m_nItemIndex == nFieldIndex)
			return *it;

		it++;
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::ChaffDataTick()
/// \brief		
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CUnitData::ChaffDataTick()
{
	vector<CItemData*>::iterator it = m_pChaffData.begin();
	while(it != m_pChaffData.end())
	{
		(*it)->Tick();
		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int	CUnitData::GetChaffDataCound()
/// \brief		
/// \author		ispark
/// \date		2006-12-04 ~ 2006-12-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CUnitData::GetChaffDataCound()
{
	return m_pChaffData.size();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			FLOAT CUnitData::GetSkillChgTime()
/// \brief		
/// \author		// 2007-04-02 by bhsohn Move패킷으로 적기 상태 체크
/// \date		2007-04-02 ~ 2007-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
FLOAT CUnitData::GetSkillChgTime(int nIdx)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return 0 ;
	}
	return m_fUnitStateChgCap[nIdx];	
}

void CUnitData::AddSkillChgTime(int nIdx, FLOAT fUnitStateChgCap)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return;
	}
	if(fUnitStateChgCap < 0)
	{
		// 변수 초기화 
		m_fUnitStateChgCap[nIdx] = 0;		
	}
	else
	{
		m_fUnitStateChgCap[nIdx] += fUnitStateChgCap;	
	}	
}

bool CUnitData::IsUnitState(int nIdx)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return false;
	}
	if(m_byUnitState & (1 << nIdx))
	{
		return true;
	}
	return false;
}

void CUnitData::SetUnitSkillState(int nIdx, bool bOnOff)
{
	if(nIdx>= UNIT_STATE_MAX || nIdx < 0)
	{
		return ;
	}
	if(bOnOff)
	{
		m_byUnitState |= (1<< nIdx);
	}
	else
	{
		m_byUnitState &= ~(1<< nIdx);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CUnitData::IsCheckObjectShow()
/// \brief		오브젝트 충돌 관련 여부 판단 
/// \author		// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CUnitData::IsCheckObjectShow()
{
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CUnitData::IsCheckObjectColl()
/// \brief		오브젝트 충돌 관련 여부 판단 
/// \author		// 2007-05-17 by bhsohn 오브젝트 뒤에 숨었을시 에 대한 처 검사 처리	
/// \date		2007-05-17 ~ 2007-05-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CUnitData::IsCheckObjectColl()
{
	return m_bCheckObject;
}

// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
// 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
BOOL	CUnitData::IsRobotArmor_Old( void ) // 2013-01-08 by jhjang 기존 로봇 아머 체크 함수
{
	// 아머 아이템을 가져와 아이템에 로봇아머 플래그가 있으면 true, 아니면 false
	BOOL bRobotAmor = FALSE;
	if( m_dwPartType == _SHUTTLE )
	{
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find( POS_CENTER );
		if( it != g_pStoreData->m_mapItemWindowPosition.end()
			&& COMPARE_BIT_FLAG(it->second->ItemInfo->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR )
			// 2010. 03. 18 by jskim 몬스터변신 카드
			&& !m_MonsterTransformer )
			//end 2010. 03. 18 by jskim 몬스터변신 카드
			bRobotAmor = TRUE;
	}
	else if( m_dwPartType == _ENEMY )
	{
		ITEM* pItem = g_pDatabase->GetServerItemInfo( ((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center );
		if( pItem && COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) 
			// 2010. 03. 18 by jskim 몬스터변신 카드
			&& !m_MonsterTransformer )
			//end 2010. 03. 18 by jskim 몬스터변신 카드
			bRobotAmor = TRUE;
	}

	return bRobotAmor;
}
// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)

BOOL	CUnitData::IsRobotArmor( void ) // 2013-01-08 by jhjang 외형 변경 관련 로봇 아머 체크 함수
{
	// 아머 아이템을 가져와 아이템에 로봇아머 플래그가 있으면 true, 아니면 false
	BOOL bRobotAmor = FALSE;
	ITEM* pItem = NULL;

	if( m_dwPartType == _SHUTTLE )
	{
		CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find( POS_CENTER );
		if( it != g_pStoreData->m_mapItemWindowPosition.end() )
		{
			CItemInfo *pItemInfo = it->second;
			if(pItemInfo->ShapeItemNum)
				pItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
			else
				pItem = it->second->ItemInfo;
		}
	}
	else if( m_dwPartType == _ENEMY )
	{
		if(((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center_ShapeItemNum)
			pItem = g_pDatabase->GetServerItemInfo( ((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center_ShapeItemNum );
		else
			pItem = g_pDatabase->GetServerItemInfo( ((CEnemyData *)this)->m_infoCharacter.CharacterRenderInfo.RI_Center );
	}
	//ITEM_GENERAL* pGeneralItem = 
	if( pItem && COMPARE_BIT_FLAG( pItem->ItemAttribute, ITEM_ATTR_ROBOT_ARMOR ) 
		&& !m_MonsterTransformer )// 2010. 03. 18 by jskim 몬스터변신 카드
	{
		bRobotAmor = TRUE;
	}

	return bRobotAmor;
}
// end 2013-01-08 by jhjang 로봇아머에 외형 변경 킷을 사용했을때 문제가 발생하던 부분 수정
