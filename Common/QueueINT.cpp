// QueueINT.cpp: implementation of the CQueueINT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QueueINT.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueueINT::CQueueINT()
{
	m_mtvectorINTList.clear();
	m_nFrontIndex = 0;
	m_nBackIndex = 0;
	m_nQueueSize = 0;
	m_nQueueMinNum = 0;
	m_nQueueMaxNum = 0;
}

CQueueINT::~CQueueINT()
{
	mt_auto_lock mtAuto(&m_mtvectorINTList);
}

bool CQueueINT::IsValidDataNum(int i_nNum) const
{
	if (m_nQueueMinNum > i_nNum
		|| m_nQueueMaxNum < i_nNum)
	{// 유효한 데이타가 아니다
		return FALSE;
	}
	return TRUE;
}

bool CQueueINT::InitQueueINT(int i_nMinNum, int i_nMaxNum)
{
	if (i_nMinNum >= i_nMaxNum)
	{
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mtvectorINTList);
	m_mtvectorINTList.clear();

	m_nFrontIndex = 0;
	m_nBackIndex = 0;
	m_nQueueMinNum = i_nMinNum;
	m_nQueueMaxNum = i_nMaxNum;
	m_nQueueSize = 2 + i_nMaxNum - i_nMinNum;
	m_mtvectorINTList.reserve(m_nQueueSize);
	for (int i = 0; i < m_nQueueSize; i++)
	{
		m_mtvectorINTList.push_back(0);
	}
	for (int i = i_nMinNum; i <= i_nMaxNum; i++)
	{
		this->PushQueueINT(i);
	}

	return TRUE;
}

void CQueueINT::ResetQueueINT(void)
{
	InitQueueINT(m_nQueueMinNum, m_nQueueMaxNum);
}

bool CQueueINT::PopQueueINT(int *o_pNum)
{
	mt_auto_lock mtAuto(&m_mtvectorINTList);
	if (m_nFrontIndex == m_nBackIndex)
	{// Queue is empty

		return FALSE;
	}

	*o_pNum = m_mtvectorINTList[m_nBackIndex];
	m_nBackIndex = ++m_nBackIndex%m_nQueueSize;
	return TRUE;
}

bool CQueueINT::PushQueueINT(int i_nNum)
{
	if (FALSE == IsValidDataNum(i_nNum))
	{// 유효한 데이타가 아니다
		return FALSE;
	}

	mt_auto_lock mtAuto(&m_mtvectorINTList);
	int nFIdx = (m_nFrontIndex + 1) % m_nQueueSize;
	if (nFIdx == m_nBackIndex)
	{// Queue가 이미 다 찼다

		return FALSE;
	}

	m_mtvectorINTList[m_nFrontIndex] = i_nNum;
	m_nFrontIndex = ++m_nFrontIndex%m_nQueueSize;
	return TRUE;
}


//#include "stdafx.h"
//#include "QueueINT.h"
//
//bool CQueueINT::InitQueueINT(int i_nMinNum, int i_nMaxNum)
//{
//	if (i_nMinNum >= i_nMaxNum) return false;
//
//	mt_auto_lock mtAuto { m_mtvectorINTList };
//
//	m_mtvectorINTList.clear();
//
//	m_nFrontIndex		= 0;
//	m_nBackIndex		= 0;
//	m_nQueueMinNum		= i_nMinNum;
//	m_nQueueMaxNum		= i_nMaxNum;
//	m_nQueueSize		= 2 + i_nMaxNum - i_nMinNum;
//
//	m_mtvectorINTList.reserve(m_nQueueSize);
//
//	for (auto i = i_nMinNum; i <= i_nMaxNum; i++) PushQueueINT(i);
//
//	return true;
//}
//
//void CQueueINT::ResetQueueINT()
//{
//	InitQueueINT(m_nQueueMinNum, m_nQueueMaxNum);
//}
//
//bool CQueueINT::PopQueueINT(int* o_pNum)
//{	
//	mt_auto_lock mtAuto { m_mtvectorINTList };
//
//	if (m_nFrontIndex == m_nBackIndex) return false;
//	
//	*o_pNum = m_mtvectorINTList[m_nBackIndex];
//
//	m_nBackIndex = ++m_nBackIndex % m_nQueueSize;
//
//	return true;
//}
//
//bool CQueueINT::PushQueueINT(int i_nNum)
//{
//	if (!IsValidDataNum(i_nNum)) return false;
//
//	mt_auto_lock mtAuto { m_mtvectorINTList };
//
//	auto nFIdx = (m_nFrontIndex + 1) % m_nQueueSize;
//	
//	if (nFIdx == m_nBackIndex) return false;
//
//	m_mtvectorINTList[m_nFrontIndex] = i_nNum;
//
//	m_nFrontIndex = ++m_nFrontIndex % m_nQueueSize;
//
//	return true;
//}