// WeaponAllAttackData.cpp: implementation of the CWeaponAllAttackData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponAllAttackData.h"
#include "UnitData.h"
#include "CharacterInfo.h"
#include "ItemData.h"
#include "SceneData.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "ShuttleChild.h"
#include "EnemyData.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// \fn			CWeaponAllAttackData , Construction
/// \brief		
/// \author		dhkwon
/// \date		2004-06-26 ~ 2004-06-26
/// \warning	
///									  
/// \param		
///				CAtumNode * pTarget : NULL이면 공격 이펙트
///									  pTarget->m_dwPartType == _ADMIN,_SHUTTLE, _ENEMY, _MONSTER 이면 유닛 타격 이펙트
///										pTarget->m_bShielding == TRUE 이면 쉴드 이펙트 생성
///									  pTarget->m_dwPartType == _DUMMY 아이템 타격 이펙트
/// \return		
///////////////////////////////////////////////////////////////////////////////

// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
// CWeaponAllAttackData::CWeaponAllAttackData(CAtumData * pAttackter, 
// 										   CAtumData * pTarget, 
// 										   BodyCond_t nBodyCondition, 
// 										   UINT nItemNum)

// CWeaponAllAttackData::CWeaponAllAttackData( CAtumData * pAttackter,
// 										    CAtumData * pTarget,
// 											BodyCond_t nBodyCondition,
// 											UINT nItemNum,
// 											ITEM* pEffectItem /* = NULL */ )
// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
 CWeaponAllAttackData::CWeaponAllAttackData( CAtumData * pAttackter,
 										    CAtumData * pTarget,
 											BodyCond_t nBodyCondition,
 											UINT nItemNum,
 											ITEM* pEffectItem, /* = NULL */ 
											int LoadingPriority )
//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
{
	m_pAttacker = pAttackter;
	m_pTarget = pTarget;
	m_dwWeaponState = _NORMAL;
	m_pItemData = NULL;
	m_bodyCondition = nBodyCondition;//	전체공격은 BODYCON_FIRE_MASK, BODYCON_HIT_MASK 둘중 하나
	m_pItemData = g_pDatabase->GetServerItemInfo(nItemNum);
	if(!m_pItemData)
	{
		DBGOUT("omi.tex error ITEM:%d\n", nItemNum);
		g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
		return;
	}

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	m_pEffectItem = pEffectItem;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

// 이펙트 초기화
	char buf[64];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(nItemNum);

	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
	// 이펙트 아이템이 있다면 그것으로 생성
	if( m_pEffectItem )
		pItem = m_pEffectItem;
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

//	wsprintf(buf,"%08d",nItemNum);
	wsprintf(buf,"%08d",pItem->SourceIndex);
	m_pCharacterInfo = new CCharacterInfo();
	if(m_pCharacterInfo->Load(buf) == FALSE)
	{
		DBGOUT("effectinfo.inf error:%d\n", nItemNum);
		util::del(m_pCharacterInfo);
//		g_pD3dApp->NetworkErrorMsgBox(STRERR_C_RESOURCE_0001);
		return;
	}
	m_pCharacterInfo->InitDeviceObjects();
	m_pCharacterInfo->RestoreDeviceObjects();
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
 	m_pCharacterInfo->m_LoadingPriority = LoadingPriority;
 	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
	m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);

// 자식 타격 이펙트 생성
	if(m_pTarget == NULL && m_bodyCondition == BODYCON_FIRE_MASK)
	{
		//2011-07-08 by jhahn 인피3차 광역공격 수
		//if(D3DXVec3Length( &(pAttackter->m_vPos - g_pShuttleChild->m_vPos) ) < m_pItemData->Range )
		if(D3DXVec3Length( &(pAttackter->m_vPos - g_pShuttleChild->m_vPos) ) < m_pItemData->ExplosionRange )
		//end 2011-07-08 by jhahn 인피3차 광역공격 수정
		{
			CreateHitEffect( g_pShuttleChild );
		}
		CMapEnemyIterator it = g_pScene->m_mapEnemyList.begin();
		while(it != g_pScene->m_mapEnemyList.end())
		{
			CEnemyData* pEnemy = it->second;
			
			//2011-07-08 by jhahn 광역공격 수정
	//		if( D3DXVec3Length( &(pAttackter->m_vPos - pEnemy->m_vPos) ) < m_pItemData->Range )
			if( D3DXVec3Length( &(pAttackter->m_vPos - pEnemy->m_vPos) ) < m_pItemData->ExplosionRange )
			//end 2011-07-08 by jhahn 광역공격 수정
			{
				CreateHitEffect( pEnemy );
			}
			it++;
		}
	}
// 이펙트 위치 설정
	if(m_pTarget && 
		m_pTarget->m_dwPartType <= _MONSTER ) //_ADMIN,_SHUTTLE, _ENEMY, _MONSTER
	{
		m_vVel = m_pTarget->m_vPos - m_pAttacker->m_vPos;
		D3DXVec3Normalize(&m_vVel,&m_vVel);
		// 자식(타격) 이펙트
		if(!m_pTarget->m_bShielding)
		{
			// 랜덤하게 위치를 조정한다.
			D3DXVECTOR3 vVel;
			D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
			vVel = D3DXVECTOR3( vVel.x*((float)(RANDI(0, SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE - 1))),
								vVel.y*((float)(RANDI(0, SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE - 1))),
								vVel.z*((float)(RANDI(0, SIZE_OF_PRIMARY_ATTACK_RANDOM_DISTANCE - 1))));
			m_vPos = m_pTarget->m_vPos + vVel;
		}
		else
		{// 쉴드 Hit 이펙트
			D3DXVECTOR3 vVel;
			D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
			m_vPos = m_pTarget->m_vPos + vVel*SIZE_OF_SHIELD_EFFECT;
			((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_pAttacker->m_vPos);
		}
		DBGOUT("CWeaponAllAttackData Child Create\n");
	}
	else
	{
		// 공격 이펙트
		m_vVel = m_pAttacker->m_vVel;
		m_vPos = m_pAttacker->m_vPos;
		DBGOUT("CWeaponAllAttackData Create\n");
	}
	m_vUp = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
}

CWeaponAllAttackData::~CWeaponAllAttackData()
{
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
	DBGOUT("CWeaponAllAttackData Delete\n");
}

void CWeaponAllAttackData::Tick()
{
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->Tick(g_pD3dApp->GetElapsedTime());
		if(m_pCharacterInfo->IsUsing() == FALSE )
		{
			m_bUsing = FALSE;
		}
	}
}

void CWeaponAllAttackData::CreateHitEffect(CAtumData* pTarget)
{
	// 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현
// 	CWeaponAllAttackData* pData = new CWeaponAllAttackData( m_pAttacker, 
// 															pTarget, 
// 															BODYCON_HIT_MASK, 
// 															m_pItemData->ItemNum);

	CWeaponAllAttackData* pData = new CWeaponAllAttackData( m_pAttacker,
															pTarget,
															BODYCON_HIT_MASK,
															m_pItemData->ItemNum,
															m_pEffectItem );
	// end 2009. 08. 27 by ckPark 그래픽 리소스 변경 시스템 구현

	g_pScene->m_pWeaponData->AddChild( pData );
}
