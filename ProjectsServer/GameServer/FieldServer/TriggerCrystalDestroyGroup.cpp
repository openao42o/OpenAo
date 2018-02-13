// TriggerCrystalDestroyGroup.cpp: implementation of the CTriggerCrystalDestroyGroup class.
//
// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
// \author		hskim
// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TriggerCrystalDestroyGroup.h"
#include "MapTriggerManager.h"
#include "TriggerFunctionCrystal.h"
#include "TriggerCrystalGroup.h"
#include "FieldMapChannel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriggerCrystalDestroyGroup::CTriggerCrystalDestroyGroup()
{
	Clear();
}

CTriggerCrystalDestroyGroup::~CTriggerCrystalDestroyGroup()
{
	Destroy();
}

void CTriggerCrystalDestroyGroup::Clear()
{
	m_SequenceNumber = 0;
	m_TargetMonster = 0;
	
	m_pMapTriggerManager = NULL;
	m_pTriggerFunctionCrystal = NULL;
	m_pTrigerCrystalGroup = NULL;
}

BOOL CTriggerCrystalDestroyGroup::Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunctionCrystal *pTriggerFunctionCrystal, CTriggerCrystalGroup *pTrigerCrystalGroup, SequenceNumber_t SequenceNumber, MonIdx_t TargetMonster)
{
	m_SequenceNumber = SequenceNumber;
	m_TargetMonster = TargetMonster;

	m_pMapTriggerManager = pMapTriggerManager;
	m_pTriggerFunctionCrystal = pTriggerFunctionCrystal;
	m_pTrigerCrystalGroup = pTrigerCrystalGroup;

	if( 0 == m_SequenceNumber && NULL != m_pTriggerFunctionCrystal && NULL != m_pTrigerCrystalGroup )
	{
		m_pTriggerFunctionCrystal->m_pDefaultCrystalGroup = m_pTrigerCrystalGroup;
	}

	return TRUE;
}

void CTriggerCrystalDestroyGroup::Destroy()
{
	Clear();
}

BOOL CTriggerCrystalDestroyGroup::CreateCrystal()
{
	// NPC Server로 전송 (NPC 서버에 접속 되어 있는지 확인하고 호출)

	if( NULL != m_pTriggerFunctionCrystal && NULL != m_pTriggerFunctionCrystal->m_pFieldMapChannel )
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_ADMIN_SUMMON_MONSTER, T_FN_ADMIN_SUMMON_MONSTER, pMsgSummonMonster, SendBuf);
		pMsgSummonMonster->ChannelIndex			= m_pTriggerFunctionCrystal->m_pFieldMapChannel->GetMapChannelIndex().ChannelIndex;
		util::strncpy(pMsgSummonMonster->CharacterName, "CrystalMonster", SIZE_MAX_CHARACTER_NAME);
		pMsgSummonMonster->MonsterUnitKind		= m_TargetMonster;
		pMsgSummonMonster->MonsterTargetType1	= MONSTER_TARGETTYPE_NORMAL;
		pMsgSummonMonster->TargetTypeData1		= 0;
		pMsgSummonMonster->CltIdxForTargetType1	= 0;
		pMsgSummonMonster->NumOfMonster			= 1;
		pMsgSummonMonster->Position				= AVECTOR3(0, 0, 0);
		pMsgSummonMonster->MonsterBalanceData.Init(0);
		m_pTriggerFunctionCrystal->m_pFieldMapChannel->Send2NPCServerW(SendBuf, MSG_SIZE(MSG_FN_ADMIN_SUMMON_MONSTER));
	}

	return TRUE;
}

BOOL CTriggerCrystalDestroyGroup::DestroyCrystal()
{
	if( NULL != m_pTriggerFunctionCrystal && NULL != m_pTriggerFunctionCrystal->m_pFieldMapChannel )
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pDestroy, SendBufDestroy);
		pDestroy->mapChann			= m_pTriggerFunctionCrystal->m_pFieldMapChannel->GetMapChannelIndex();
		pDestroy->MonsterUnitKind	= m_TargetMonster;
		((CFieldIOCP*)(m_pTriggerFunctionCrystal->m_pFieldMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufDestroy, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY));
	}

	return TRUE;
}
