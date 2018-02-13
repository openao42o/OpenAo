// WSlowData.cpp: implementation of the CWSlowData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSlowData.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "WeaponItemInfo.h"
#include "EnemyData.h"
#include "TutorialSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWSlowData::CWSlowData()
{
	FLOG( "CWSlowData()" );
	m_nItemIndex = 0;	
	m_bEvasion = FALSE;

	// 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
	m_nBlindSpeedDownTime	= 0;

	m_nBlindCumulate		= 0;
	// end 2010. 03. 18 by ckPark 인피니티 필드 2차(몬스터 스킬 추가)
}

CWSlowData::~CWSlowData()
{
	FLOG( "~CWSlowData()" );

}


void CWSlowData::SendFieldSocketBattleAttackFind(int nTargetIndex, int nItemIndex, ClientIndex_t nClientIndex, UINT nItemNum)
{
	MEX_TARGET_INFO target;
	target.TargetIndex = nTargetIndex;
	target.TargetItemFieldIndex = nItemIndex;
	target.TargetPosition = m_vPos;
//	g_pShuttleChild->m_pSecondaryWeapon->SendBattleAttackFind(target, m_nWeaponIndex);	// 주의 WeaponIndex 다시 정의해야 함.
	MSG_FC_BATTLE_ATTACK_FIND sMsg;
	sMsg.AttackIndex = nClientIndex;
	sMsg.ItemNum = nItemNum;
	if(	nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
		g_pShuttleChild->m_pSecondaryWeapon)
	{
		sMsg.AttackType = g_pShuttleChild->m_pSecondaryWeapon->GetAttackMode();
	}
	else
	{
		sMsg.AttackType = ATT_TYPE_GENERAL_SEC;
	}
	sMsg.TargetInfo = target;
	sMsg.WeaponIndex = m_nWeaponIndex;
	// 2008-09-08 by dgwoo 튜토리얼 모드중 메뉴가 활성화 되어있는 상태에서는 데미지를 입지 않는다.
	if(g_pD3dApp->m_pTutorial->IsTutorialMode() && g_pD3dApp->m_pTutorial->IsSystemWin() 
		&& g_pShuttleChild->m_myShuttleInfo.ClientIndex == nTargetIndex)
	{
		return;
	}
#pragma omp critical
	g_pFieldWinSocket->SendMsg(T_FC_BATTLE_ATTACK_FIND, (char*)&sMsg, sizeof(sMsg));
}

//void CWSlowData::SendFieldSocketBattleAttackItemFind(int nTargetIndex, int nItemIndex)
//{
//	MSG_FC_BATTLE_ATTACK_ITEM_FIND sMsg;
//	memset(&sMsg,0x00,sizeof(sMsg));
//	sMsg.AttackIndex = m_nClientIndex;
//	sMsg.TargetIndex = nTargetIndex;
//	sMsg.TargetItemFieldIndex = nItemIndex;
//	sMsg.TargetPosition = m_vPos;
//	sMsg.WeaponIndex = m_nWeaponIndex;
//	g_pD3dApp->m_pFieldWinSocket->SendMsg(T_FC_BATTLE_ATTACK_ITEM_FIND, (char*)&sMsg, sizeof(sMsg));
//}
/*
void CWSlowData::SendFieldSocketBattleMonsterAttackFind(int nTargetIndex)
{
	MSG_FC_BATTLE_MONSTER_ATTACK_FIND sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	sMsg.AttackIndex = m_nClientIndex;
	sMsg.TargetIndex = nTargetIndex;
	sMsg.TargetPosition = m_vPos;
	sMsg.WeaponIndex = m_nWeaponIndex;
	sMsg.WeaponItemNum = m_pItemData->ItemNum;
	g_pFieldWinSocket->SendMsg(T_FC_BATTLE_MONSTER_ATTACK_FIND, (char*)&sMsg, sizeof(sMsg));
}

void CWSlowData::SendFieldSocketBattleMonsterAttackItemFind(int nTargetIndex, int nItemIndex)
{
	MSG_FC_BATTLE_MONSTER_ATTACK_ITEM_FIND sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	sMsg.AttackIndex = m_nClientIndex;
	sMsg.TargetIndex = nTargetIndex;
	sMsg.TargetItemFieldIndex = nItemIndex;
	sMsg.TargetPosition = m_vPos;
	sMsg.WeaponIndex = m_nWeaponIndex;
	sMsg.WeaponItemNum = m_pItemData->ItemNum;
	g_pD3dApp->m_pFieldWinSocket->SendMsg(T_FC_BATTLE_ATTACK_ITEM_FIND, (char*)&sMsg, sizeof(sMsg));
}

*/

///////////////////////////////////////////////////////////////////////////////
/// \fn			CWSlowData::RollingCollision(CEnemyData *pEnemy)
/// \brief		
/// \author		ispark
/// \date		2005-12-10 ~ 2005-12-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWSlowData::RollingCollision(CEnemyData *pEnemy)
{
	if(pEnemy && pEnemy->m_bRollStart)
	{
		if(pEnemy->m_fRollStartTime <= ROLLING_USE_TIME)
		{
			m_pTarget = NULL;
			return FALSE;
		}
//		pEnemy->m_bRollStart = FALSE;
//		pEnemy->m_fRollStartTime = 0.0f;
	}
	else if(m_pTarget && m_pTarget->m_dwPartType == _SHUTTLE && g_pShuttleChild->m_bRollUsed)
	{
		if(g_pShuttleChild->m_fRollTime <= ROLLING_USE_TIME)
		{
			m_pTarget = NULL;
			return FALSE;
		}
//		g_pShuttleChild->m_bRollUsed = FALSE;
//		g_pShuttleChild->m_fRollTime = 0.0f;
	}
	
	return TRUE;
}

void CWSlowData::SendFieldSocketBattleAttackEvasion(int nTargetIndex, int nItemIndex, ClientIndex_t nClientIndex, UINT nItemNum)
{
	MEX_TARGET_INFO target;
	target.TargetIndex = nTargetIndex;
	target.TargetItemFieldIndex = nItemIndex;
	target.TargetPosition = m_vPos;

	MSG_FC_BATTLE_ATTACK_EVASION sMsg;
	sMsg.AttackIndex = nClientIndex;
	sMsg.ItemNum = nItemNum;
	if(	nClientIndex == g_pShuttleChild->m_myShuttleInfo.ClientIndex &&
		g_pShuttleChild->m_pSecondaryWeapon)
	{
		sMsg.AttackType = g_pShuttleChild->m_pSecondaryWeapon->GetAttackMode();
	}
	else
	{
		sMsg.AttackType = ATT_TYPE_GENERAL_SEC;
	}
	sMsg.TargetInfo = target;
	sMsg.WeaponIndex = m_nWeaponIndex;

#pragma omp critical
	g_pFieldWinSocket->SendMsg(T_FC_BATTLE_ATTACK_EVASION, (char*)&sMsg, sizeof(sMsg));
}
