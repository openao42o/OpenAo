// BunchCountableItemCount.cpp: implementation of the CBunchCountableItemCount class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BunchCountableItemCount.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBunchCountableItemCount::CBunchCountableItemCount()
{
	m_vectBUNCH_ITEM_COUNTList.clear();
}

CBunchCountableItemCount::~CBunchCountableItemCount()
{
}


int	CBunchCountableItemCount::GetTotalBunchCountableItemCounts(void)
{
	int nRetCounts = 0;

	for(int i=0; i < m_vectBUNCH_ITEM_COUNTList.size(); i++)
	{
		nRetCounts += m_vectBUNCH_ITEM_COUNTList[i].nBunchItemCount;
	}

	return nRetCounts;
}


int CBunchCountableItemCount::GetInventoryCounts(void)
{
	int nRetCounts = 0;
	for(int i=0; i < m_vectBUNCH_ITEM_COUNTList.size(); i++)
	{
		if(0 == m_vectBUNCH_ITEM_COUNTList[i].nBunchItemCount%COUNT_BUNCH_COUNTABLE_ITEM_UNIT)
		{
			nRetCounts += m_vectBUNCH_ITEM_COUNTList[i].nBunchItemCount/COUNT_BUNCH_COUNTABLE_ITEM_UNIT;
		}
		else
		{
			nRetCounts += m_vectBUNCH_ITEM_COUNTList[i].nBunchItemCount/COUNT_BUNCH_COUNTABLE_ITEM_UNIT + 1;
		}
	}

	return nRetCounts;
}

int CBunchCountableItemCount::AddBunchCountableItemByItemNum(int i_nItemNum)
{
	for(int i=0; i < m_vectBUNCH_ITEM_COUNTList.size(); i++)
	{
		if(m_vectBUNCH_ITEM_COUNTList[i].nItemNum == i_nItemNum)
		{
			m_vectBUNCH_ITEM_COUNTList[i].nBunchItemCount++;
			return m_vectBUNCH_ITEM_COUNTList[i].nBunchItemCount;
		}		
	}

	SBUNCH_ITEM_COUNT tmBunchCount(i_nItemNum, 1);
	m_vectBUNCH_ITEM_COUNTList.push_back(tmBunchCount);
	return tmBunchCount.nBunchItemCount;
}
