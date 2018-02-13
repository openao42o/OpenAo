// TriggerFunction.cpp: implementation of the CTriggerFunction class.
//
/// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
/// \author		hskim
/// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TriggerFunction.h"
#include "MapTriggerManager.h"
#include "FieldMonster.h"
#include "FieldMapChannel.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriggerFunction::CTriggerFunction()
{
	OnClear();
}

CTriggerFunction::~CTriggerFunction()
{
	OnDestroy();
}

void CTriggerFunction::OnClear()
{
	memset(&m_TriggerInfo, 0, sizeof(m_TriggerInfo));
	m_TriggerType = MAP_TRIGGER_NONE;
	m_pMapTriggerManager = NULL;
	m_pFieldMapChannel = NULL;
}

BOOL CTriggerFunction::OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType)
{
	if( NULL == pMapTriggerManager )
	{
		return FALSE;
	}

	m_TriggerInfo.MapTriggerID = MapTriggerID;
	m_TriggerInfo.MapIndex = MapIndex;
	m_TriggerInfo.MapChannel = MapChannel;
	m_TriggerInfo.FunctionID = FunctionID;

	m_TriggerType = TriggerType;

	m_pMapTriggerManager = pMapTriggerManager;

	return TRUE;
}

void CTriggerFunction::OnDestroy()
{
	OnClear();
}

void CTriggerFunction::DoMinutelyWork(ATUM_DATE_TIME *pDateTime)
{
}

void CTriggerFunction::OnEventSkip(BOOL bCheckCreateCrystal/*=TRUE*/)		// 2012-12-17 by hskim, NPC 서버 재시작시 - 트리거 시스템 초기화 적용
{
}

BOOL CTriggerFunction::IsMapIndex(MapIndex_t MapIndex)
{
	if( MapIndex == GetMapIndex() )
	{
		return TRUE;
	}

	return FALSE;
}

MapIndex_t CTriggerFunction::GetMapIndex()
{
	return m_TriggerInfo.MapIndex;
}

void CTriggerFunction::LinkToFieldMapChannel(CFieldMapChannel	*pFieldMapChannel)
{
	m_pFieldMapChannel = pFieldMapChannel;
}

BOOL CTriggerFunction::OnMonsterCreate(CFieldMonster *pMonster)
{
	return FALSE;		// 아무 처리 없을 경우 FALSE
}

BOOL CTriggerFunction::OnMonsterDead(CFieldMonster *pMonster)
{
	return FALSE;		// 아무 처리 없을 경우 FALSE
}

BOOL CTriggerFunction::OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage)
{
	return FALSE;		// 아무 처리 없을 경우 FALSE
}

BOOL CTriggerFunction::OnIsInvincible(MonIdx_t MonsterIdx, CFieldIOCPSocket *pIOCPSocket)
{
	return FALSE;		// 아무 처리 없을 경우 FALSE
}

// 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 공격 가능 여부 체크 가상함수
BOOL CTriggerFunction::OnIsPossibleBossAttack(MonIdx_t MonsterIdx)
{
	return TRUE;		// 아무 처리 없을 경우 TRUE
}
// end 2013-01-21 by jhseol, NGC 전전기지 트리거 시스템 - 공격 가능 여부 체크 가상함수

// 2013-01-23 by jhseol, 전쟁 시 크리스탈만 공격 불가능 하도록 수정
BOOL CTriggerFunction::OnIsCrystal()
{
	return FALSE;
}
// end 2013-01-23 by jhseol, 전쟁 시 크리스탈만 공격 불가능 하도록 수정
