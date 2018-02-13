// WayPointManager.cpp: implementation of the CWayPointManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WayPointManager.h"
#include "AtumDBHelper.h"
#include "GlobalGameServer.h"

//////////////////////////////////////////////////////////////////////
// CWayPoint
//////////////////////////////////////////////////////////////////////

CWayPointMove::CWayPointMove()
{
	Clean();
}

CWayPointMove::CWayPointMove(UINT dwIdx, BYTE byCAction)
{
	Clean();

	dwIndex = dwIdx;
	byCompletionAction = byCAction;
}

CWayPointMove::~CWayPointMove()
{
	Clean();
}

void CWayPointMove::Clean()
{
	m_vectWayPointList.clear();
	dwIndex = 0;
	byCompletionAction = WAY_POINT_POSITION_ATTRIBUTE_ABSOLUTE;
}

BOOL CWayPointMove::Add(SWAY_POINT *pData)
{
	m_vectWayPointList.push_back(*pData);

	return TRUE;
}

SWAY_POINT *CWayPointMove::GetWayPoint(int iStep)
{
	return &m_vectWayPointList[iStep];
}

INT CWayPointMove::GetCompletionAction()
{
	return byCompletionAction;
}

INT CWayPointMove::GetSize()
{
	return m_vectWayPointList.size();
}

void CWayPointMove::Sort()
{
	sort(m_vectWayPointList.begin(), m_vectWayPointList.end(), SSORT_WAY_POINT_BY_SEQUENCE_NUMBER());
}

//////////////////////////////////////////////////////////////////////
// CWayPointManager
//////////////////////////////////////////////////////////////////////

CWayPointManager::CWayPointManager()
{
	Clean();
}

CWayPointManager::~CWayPointManager()
{
	Release();
}

void CWayPointManager::Clean()
{
	m_vectWPPatternList.clear();
}

BOOL CWayPointManager::Create()
{
	return TRUE;
}

void CWayPointManager::Release()
{
	Clean();
}

BOOL CWayPointManager::LoadDBWayPointData(CODBCStatement *i_odbcstmt)
{
	if ( NULL == i_odbcstmt )
	{
		return FALSE;
	}

	Release();

	CAtumDBHelper::DBLoadWayPointMoveDataInfo(i_odbcstmt , this, FALSE);
	CAtumDBHelper::DBLoadWayPointDataInfo(i_odbcstmt , this, FALSE);

	Sort();		// Á¤·Ä

	return TRUE;
}

BOOL CWayPointManager::AddPattern(CWayPointMove *pData)
{
	m_vectWPPatternList.push_back(*pData);

	return TRUE;
}

BOOL CWayPointManager::AddWayPoint(SWAY_POINT *pWayPoint)
{
	vectorWayPointPattern::iterator itrPattern = m_vectWPPatternList.begin();
	
	while(itrPattern != m_vectWPPatternList.end()) 
	{
		if( itrPattern->GetIndex() == pWayPoint->dwPatternIndex )
		{
			itrPattern->Add(pWayPoint);
			
			return TRUE;
		}

		itrPattern++;
	}

	return FALSE;
}

SWAY_POINT *CWayPointManager::GetWayPointInfo(int iIndex, int iStep)
{
	vectorWayPointPattern::iterator itrPattern = m_vectWPPatternList.begin();
	
	while(itrPattern != m_vectWPPatternList.end()) 
	{
		if( itrPattern->GetIndex() == iIndex )
		{
			return itrPattern->GetWayPoint(iStep);
		}
	
		itrPattern++;
	}

	return NULL;
}

CWayPointMove *CWayPointManager::GetWayPointPattern(int iIndex)
{
	vectorWayPointPattern::iterator itrPattern = m_vectWPPatternList.begin();

	while(itrPattern != m_vectWPPatternList.end()) 
	{
		if( itrPattern->GetIndex() == iIndex )
		{
			return &*itrPattern;
		}

		itrPattern++;
	}
	
	return NULL;
}

INT CWayPointManager::GetCompletionAction(int iIndex)
{
	CWayPointMove *pPattern = GetWayPointPattern(iIndex);

	if( pPattern == NULL )
	{
		char	szSysLog[1024];
		sprintf(szSysLog, "[Error] CWayPointManager::GetCompletionAction() WayPointMove is NULL\r\n");
		g_pGlobal->WriteSystemLog(szSysLog);
		DBGOUT(szSysLog);

		return 0;	
	}

	return pPattern->GetCompletionAction();
}

INT CWayPointManager::GetTotalCountPattern()
{
	return m_vectWPPatternList.size();
}

INT CWayPointManager::GetTotalCountData()
{
	int iTotalCount = 0;

	vectorWayPointPattern::iterator itrPattern = m_vectWPPatternList.begin();
	
	while(itrPattern != m_vectWPPatternList.end()) 
	{
		iTotalCount += itrPattern->GetSize();

		itrPattern++;
	}

	return iTotalCount;
}

BOOL CWayPointManager::IsLastStep(int iIndex, int iStep)
{
	CWayPointMove *pPattern = GetWayPointPattern(iIndex);
	
	if( pPattern == NULL )
	{
		char	szSysLog[1024];
		sprintf(szSysLog, "[Error] CWayPointManager::IsLastStep() WayPointMove is NULL\r\n");
		g_pGlobal->WriteSystemLog(szSysLog);
		DBGOUT(szSysLog);

		return FALSE;
	}

	if( pPattern->GetSize() == (iStep + 1) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CWayPointManager::IsFirstStep(int iStep)
{
	if( iStep == 0)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CWayPointManager::IsCollision(int iIndex, int iStep, D3DXVECTOR3 vecPosition)
{
	CWayPointMove *pPattern = GetWayPointPattern(iIndex);
	
	if (pPattern == nullptr)
	{
		char	szSysLog[1024];
		sprintf(szSysLog, "[Error] CWayPointManager::IsCollision() WayPointMove is NULL\r\n");
		g_pGlobal->WriteSystemLog(szSysLog);
		DBGOUT(szSysLog);

		return FALSE;
	}

	D3DXVECTOR3 vecDist = (pPattern->GetWayPoint(iStep)->vPoint - vecPosition);

	if( D3DXVec3Length(&vecDist) < WAY_POINT_COLLISION_THRESHOLD ) return TRUE;

	return FALSE;
}

void CWayPointManager::Sort()
{
	for (auto& pattern : m_vectWPPatternList) pattern.Sort();

	sort(m_vectWPPatternList.begin(), m_vectWPPatternList.end(), [](auto& a, auto& b) { return a.GetIndex() < b.GetIndex(); });
}