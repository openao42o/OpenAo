// WeaponShieldData.cpp: implementation of the CWeaponShieldData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeaponShieldData.h"
#include "AtumApplication.h"
#include "AtumDatabase.h"
#include "AtumData.h"

#define BODYCON_SHIELD_HIT_MASK			BODYCON_FLY_MASK
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWeaponShieldData::CWeaponShieldData(CAtumData* pAttackter,int nItemNum, D3DXVECTOR3 vTarget)
{
	FLOG( "CWeaponShieldData(MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT * pMsg)" );
	m_dwWeaponState = _INITIALIZING;
	m_dwPartType = _SHIELD;
	m_pAttacker = pAttackter;
	m_vTargetPos = vTarget;
	D3DXVec3Normalize(&m_vVel, &(m_vTargetPos - m_pAttacker->m_vPos));
	m_vPos = m_pAttacker->m_vPos;
	m_vUp = m_pAttacker->m_vUp;
	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

	char buf[256];
	wsprintf(buf,"%08d",nItemNum);
	LoadCharacterEffect(buf);
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->ChangeBodyCondition(BODYCON_SHIELD_HIT_MASK);
	}
	else
	{
		DBGOUT("Can't Find Shield Effect (nItemNum = %d).\n",nItemNum);	
		m_bUsing = FALSE;
	}
}

CWeaponShieldData::~CWeaponShieldData()
{
	FLOG( "~CWeaponShieldData()" );
}

void CWeaponShieldData::Tick()
{
	FLOG( "CWeaponShieldData::Tick()" );
	if(m_pAttacker == NULL)
	{
		m_bUsing = FALSE;
		return;
	}
	D3DXVec3Normalize(&m_vVel, &(m_vTargetPos - m_pAttacker->m_vPos));
	m_vPos = m_pAttacker->m_vPos;
	m_vUp = m_pAttacker->m_vUp;

	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->Tick(g_pD3dApp->GetElapsedTime());
		if(!m_pCharacterInfo->IsUsing())
		{
			m_bUsing = FALSE;
		}
	}
	else
	{
		m_bUsing = FALSE;
	}

}

/*
CWeaponShieldData::CWeaponShieldData(MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT * pMsg)
{
	FLOG( "CWeaponShieldData(MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT * pMsg)" );
	m_dwWeaponState = _INITIALIZING;
	m_dwPartType = _SHIELD;
	m_nAttackIndex = pMsg->AttackIndex;
	m_nItemNum = pMsg->ItemNum;
	if(m_nAttackIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex)
	{
	
	  m_pAttacker = g_pShuttleChild;
	}
	else
	{
		CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.find(m_nAttackIndex);
		if(itEnemy != g_pScene->m_mapEnemyList.end())
		{
			m_pAttacker = itEnemy->second;
		}
	}
	if(m_pAttacker)
	{
		m_vPos = m_pAttacker->m_vPos;
		m_vVel = m_pAttacker->m_vVel;
		m_vUp = m_pAttacker->m_vUp;
		D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
		D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );

		char buf[256];
//		wsprintf(buf,"%08d",m_nItemNum);
		wsprintf(buf,"%08d",7900790);// 디코이 때문에 임시로 세팅한 것임 바꿔야 함..~!! 
		LoadCharacterEffect(buf);
		if(m_pCharacterInfo)
		{
			m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
			m_pCharacterInfo->ChangeBodyCondition(BODYCON_FIRE_MASK);
		}
		else
		{
			m_bUsing = FALSE;
		}
	}
	else
	{
		m_bUsing = FALSE;
	}
}

CWeaponShieldData::~CWeaponShieldData()
{
	FLOG( "~CWeaponShieldData()" );
}

void CWeaponShieldData::ShieldOff()
{
	m_dwWeaponState = _EXPLODING;
	m_pCharacterInfo->ChangeBodyCondition(BODYCON_HIT_MASK);
}

void CWeaponShieldData::Tick()
{
	FLOG( "CWeaponShieldData::Tick()" );
	m_vPos = m_pAttacker->m_vPos;
	//m_vVel = m_pAttacker->m_vVel;
	//m_vUp = m_pAttacker->m_vUp;
	m_vUp.x = m_pAttacker->m_mMatrix._21;
	m_vUp.y = m_pAttacker->m_mMatrix._22;
	m_vUp.z = m_pAttacker->m_mMatrix._23;
	m_vVel.x = m_pAttacker->m_mMatrix._31;
	m_vVel.y = m_pAttacker->m_mMatrix._32;
	m_vVel.z = m_pAttacker->m_mMatrix._33;
	m_vVel *= -1;

	D3DXMatrixLookAtRH( &m_mMatrix, &(m_vPos), &(m_vPos + m_vVel), &m_vUp);
	D3DXMatrixInverse( &m_mMatrix, NULL, &m_mMatrix );
	if(m_pCharacterInfo)
	{
		m_pCharacterInfo->SetAllBodyConditionMatrix(m_mMatrix);
		m_pCharacterInfo->Tick(g_pD3dApp->GetElapsedTime());

		switch(m_dwWeaponState)
		{
		case _INITIALIZING:// ON
			{
				if(!m_pCharacterInfo->IsUsing(BODYCON_FIRE_MASK))
				{
					m_pCharacterInfo->ChangeBodyCondition(BODYCON_BULLET_MASK);
					m_dwWeaponState = _NORMAL;
				}
			}
			break;
		case _NORMAL: // ING
			{
			}
			break;
		case _EXPLODING: // OFF
			{
				if(!m_pCharacterInfo->IsUsing(BODYCON_HIT_MASK))
				{
					m_dwWeaponState = _EXPLODED;
					m_bUsing = FALSE;
				}
			}
			break;
		default:
			{
				m_bUsing = FALSE;
			}
			break;
		}
	}
}
*/