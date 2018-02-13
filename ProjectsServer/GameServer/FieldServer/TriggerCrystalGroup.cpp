// TriggerCrystalGroup.cpp: implementation of the CTriggerCrystalGroup class.
//
// \brief		// 2011-10-28 by hskim, EP4 [트리거 시스템] - 크리스탈 시스템
// \author		hskim
// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TriggerCrystalGroup.h"
#include "MapTriggerManager.h"
#include "TriggerFunctionCrystal.h"
#include "TriggerCrystalDestroyGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriggerCrystalGroup::CTriggerCrystalGroup()
{
	Clear();
}

CTriggerCrystalGroup::~CTriggerCrystalGroup()
{
	Destroy();
}

void CTriggerCrystalGroup::Clear()
{
	m_DestroyGroupID = 0;
	m_EventID = 0;

	m_pMapTriggerManager = NULL;
	m_pTriggerFunctionCrystal = NULL;

	m_vectorTriggerCrystalDestroy.clear();
}

BOOL CTriggerCrystalGroup::Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunctionCrystal *pTriggerFunctionCrystalDestroyGroupID_t, DestroyGroupID_t DestroyGroupID, EventID_t EventID)
{
	m_DestroyGroupID = DestroyGroupID;
	m_EventID = EventID;

	m_pMapTriggerManager = pMapTriggerManager;
	m_pTriggerFunctionCrystal = pTriggerFunctionCrystalDestroyGroupID_t;

	return TRUE;
}

void CTriggerCrystalGroup::Destroy()
{
	for (auto x : m_vectorTriggerCrystalDestroy) delete x;
	
	Clear();
}

BOOL CTriggerCrystalGroup::InsertTriggerCrystalDestroy(CTriggerCrystalDestroyGroup *pTriggerCrystalDestroy)
{
	if( NULL == pTriggerCrystalDestroy )
	{
		return FALSE;
	}
	
	m_vectorTriggerCrystalDestroy.push_back(pTriggerCrystalDestroy);

	return TRUE;
}

BOOL CTriggerCrystalGroup::IsBelongCrystal(MonIdx_t MonsterIdx)
{
	for(int i=0; i<m_vectorTriggerCrystalDestroy.size(); i++)
	{
		CTriggerCrystalDestroyGroup *pCrystalDestroy = m_vectorTriggerCrystalDestroy[i];

		if( pCrystalDestroy->m_TargetMonster == MonsterIdx )
		{
			return TRUE;
		}
	}

	return FALSE;
}

struct SSort_TRIGGER_CRYSTAL_DESTROY_BY_SEQUENCE_NUMBER: binary_function<CTriggerCrystalDestroyGroup *, CTriggerCrystalDestroyGroup *, bool>
{
	bool operator()(CTriggerCrystalDestroyGroup *pParam1, CTriggerCrystalDestroyGroup *pParam2)
	{
        return pParam1->GetSequenceNumber() < pParam2->GetSequenceNumber();
    };
};

void CTriggerCrystalGroup::SortTriggerCrystalDestroy()
{
	sort(m_vectorTriggerCrystalDestroy.begin(), m_vectorTriggerCrystalDestroy.end(), SSort_TRIGGER_CRYSTAL_DESTROY_BY_SEQUENCE_NUMBER());
}

BOOL CTriggerCrystalGroup::CreateAllCrystal()
{
	for(int i=0; i<m_vectorTriggerCrystalDestroy.size(); i++)
	{
		CTriggerCrystalDestroyGroup *pCrystalDestroy = m_vectorTriggerCrystalDestroy[i];

		pCrystalDestroy->CreateCrystal();
	}

	return TRUE;
}

BOOL CTriggerCrystalGroup::DestroAllyCrystal()
{
	for(int i=0; i<m_vectorTriggerCrystalDestroy.size(); i++)
	{
		CTriggerCrystalDestroyGroup *pCrystalDestroy = m_vectorTriggerCrystalDestroy[i];

		pCrystalDestroy->DestroyCrystal();
	}

	return TRUE;
}

BOOL CTriggerCrystalGroup::CheckDestroyEvent()
{
	if( NULL == m_pMapTriggerManager || NULL == m_pTriggerFunctionCrystal )
	{
		return FALSE;
	}

	if( m_vectorTriggerCrystalDestroy.size() != m_pTriggerFunctionCrystal->m_vectorDestroyedSequence.size() )
	{
		return FALSE;
	}

	for(int i=0; i<m_pTriggerFunctionCrystal->m_vectorDestroyedSequence.size(); i++)
	{
		CTriggerCrystalDestroyGroup *pCrystalDestroy = m_vectorTriggerCrystalDestroy[i];
		MonIdx_t MonIdx = m_pTriggerFunctionCrystal->m_vectorDestroyedSequence[i];

		if( pCrystalDestroy->m_TargetMonster != MonIdx )
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CTriggerCrystalGroup::CheckDestroySequence()
{
	if( NULL == m_pMapTriggerManager || NULL == m_pTriggerFunctionCrystal )
	{
		return FALSE;
	}	

	if( m_vectorTriggerCrystalDestroy.size() <= m_pTriggerFunctionCrystal->m_vectorDestroyedSequence.size() )
	{
		return FALSE;
	}

	for(int i=0; i<m_pTriggerFunctionCrystal->m_vectorDestroyedSequence.size(); i++)
	{
		CTriggerCrystalDestroyGroup *pCrystalDestroy = m_vectorTriggerCrystalDestroy[i];
		MonIdx_t MonIdx = m_pTriggerFunctionCrystal->m_vectorDestroyedSequence[i];

		if( pCrystalDestroy->m_TargetMonster != MonIdx )
		{
			return FALSE;
		}
	}

	return TRUE;
}