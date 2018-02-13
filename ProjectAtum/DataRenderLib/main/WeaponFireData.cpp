// WeaponFireData.cpp: implementation of the CWeaponFireData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponFireData.h"
#include "AtumApplication.h"
#include "EnemyData.h"
#include "MonsterData.h"	
#include "CharacterInfo.h"
#include "AtumDatabase.h"
#include "AtumData.h"
#include "dxutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeaponFireData::CWeaponFireData(CAtumData * pAttack,int wtType)
//	:CAtumNode(pParent)
{
	FLOG( "CWeaponFireData(CAtumData * pAttack,int wtType)" );
	m_pAttacker = pAttack;
	m_pCharacterInfo = NULL;

	m_dwPartType = _FIREDATA;
	m_fWeaponLifeTime = 5.0f;

	m_dwWeaponState=_NORMAL;

	D3DXMatrixLookAtRH( &m_mMatrix, &(m_pAttacker->m_vPos), &(m_pAttacker->m_vPos + m_pAttacker->m_vVel), &m_pAttacker->m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

	char buf[256];
	// 2005-08-23 by ispark
	ITEM* pItem = g_pDatabase->GetServerItemInfo(wtType);
//	wsprintf(buf,"%08d",wtType);
	wsprintf(buf,"%08d",pItem->SourceIndex);
	LoadCharacterEffect(buf);
	m_bodyCondition = BODYCON_BULLET_MASK;
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(m_bodyCondition);
	}

	SetShuttleChildOrderTarget();
}


CWeaponFireData::~CWeaponFireData()
{
	FLOG( "~CWeaponFireData()" );
	if(m_pCharacterInfo)//추가
	{
		m_pCharacterInfo->InvalidateDeviceObjects();
		m_pCharacterInfo->DeleteDeviceObjects();
		util::del(m_pCharacterInfo);
	}
}

void CWeaponFireData::Tick()
{
	FLOG( "CWeaponFireData::Tick()" );
	if(m_pAttacker == NULL)
	{
		m_bUsing = FALSE;
		return;
	}
	float fElapsedTime = g_pD3dApp->GetElapsedTime();
	if( m_dwWeaponState == _EXPLODING ) // exploding state
	{
		if(m_pTarget)
		{
			if(!((CUnitData*)m_pTarget)->m_bShielding)
			{
				// 랜덤하게 위치를 조정한다.
				D3DXVECTOR3 vVel;
				D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				vVel = D3DXVECTOR3( vVel.x*((float)RANDI(0, 4)),vVel.y*((float)RANDI(0, 4)),vVel.z*((float)RANDI(0, 4)));
				m_vPos = m_pTarget->m_vPos + vVel;
			}
			else	// 쉴드 발동 중인경우 좌표는 쉴드크기 만큼으로 조정한다.
			{// 쉴드 발동 중일때 쉴드 Hit 이펙트 추가
				D3DXVECTOR3 vVel;
				D3DXVec3Normalize(&vVel, &(m_pAttacker->m_vPos - m_pTarget->m_vPos));
				m_vPos = m_pTarget->m_vPos + vVel*SIZE_OF_SHIELD_EFFECT;
				((CUnitData*)m_pTarget)->CreateSecondaryShieldDamage(m_pAttacker->m_vPos);
			}
		}
		SetBodyCondition(BODYCON_HIT_MASK);
		m_dwWeaponState = _EXPLODED;
	} 
	else if(m_dwWeaponState == _NORMAL)
	{
		if(m_fWeaponLifeTime >= 0.0f)
			m_fWeaponLifeTime -= fElapsedTime;
		if(m_fWeaponLifeTime <= 0.0f)
		{
			m_dwWeaponState = _EXPLODED;
			m_bUsing = FALSE;
		}
		if(m_pAttacker->m_dwState == _NORMAL)
		{
			D3DXMatrixLookAtRH( &m_mMatrix, &(m_pAttacker->m_vPos), &(m_pAttacker->m_vPos + m_pAttacker->m_vVel), &m_pAttacker->m_vUp);
			D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
		}
	}
	else if(m_dwWeaponState == _EXPLODED)
	{
		if(m_pCharacterInfo)
		{
			if(!m_pCharacterInfo->IsUsing())
				m_bUsing = FALSE;
		}
		else
		{
			m_bUsing = FALSE;
		}
	}
	// effect matrix & ticking
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix );
//		m_pCharacterInfo->SetSingleBodyConditionMatrix( BODYCON_FIRE_MASK,m_mMatrix );
		m_pCharacterInfo->Tick(fElapsedTime);
	}
}
