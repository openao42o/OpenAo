#include "stdafx.h"
#include "MapBlock.h"
//#include "MapChannel.h"

int CMapBlock::ms_nSIZE_BLOCK = 0;

CMapBlock::CMapBlock()
{
}

CMapBlock::~CMapBlock()
{
}

// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
DROPITEM* CMapBlock::GetDROPITEM_SetProcess(ITEM_GENERAL* pStoreItem, BOOL i_bPickupTenderItem/*=FALSE*/)
{
	m_DropItemMtmap.lock();
	mtmapDropItem::iterator itr = m_DropItemMtmap.find(pStoreItem);
	if (itr != m_DropItemMtmap.end())
	{
		DROPITEM *pDropItem = &itr->second;
		if (FALSE == pDropItem->bProcessing
			&& (i_bPickupTenderItem || FALSE == pDropItem->bTendering))	// 2010-04-09 by cmkwon, 인피2차 추가 수정(단계별 보상 추가) - 
		{
			pDropItem->bProcessing = TRUE;
		}
		else
		{
			pDropItem = NULL;
		}
		m_DropItemMtmap.unlock();
		return pDropItem;
	}
	m_DropItemMtmap.unlock();

	return NULL;
}

void CMapBlock::AllDeleteDropItem()
{
	// 2009-09-09 ~ 2010 by dhjin, 인피니티 - 
	mt_auto_lock mtA(&m_DropItemMtmap);
	mtmapDropItem::iterator itr = m_DropItemMtmap.begin();
	for(; itr!=m_DropItemMtmap.end(); itr++)
	{
		DROPITEM *pDropItem = &itr->second;
		util::del(pDropItem->pItem);
	}
	m_DropItemMtmap.clear();
}

// 2012-03-05 by hskim, 드랍 아이템 일정 시간후 삭제
void CMapBlock::DeleteTimeoutDropItem(DWORD dwNowTick, vectorDeleteDropItem *pDeleteDropItem /*= NULL*/)
{
	mt_auto_lock mtA(&m_DropItemMtmap);
	mtmapDropItem::iterator itr = m_DropItemMtmap.begin();

	while( itr!=m_DropItemMtmap.end() ) 
	{
		DROPITEM *pDropItem = &itr->second;

		if( ( dwNowTick - pDropItem->dwStartTick ) >= TICKGAP_DROP_ITEM_TIMEOUT )
		{
			if( NULL != pDeleteDropItem )
			{
				SDELETE_DROP_ITEM DeleteDropItem;

				DeleteDropItem.ItemFieldIndex = (UINT)pDropItem->pItem;
				DeleteDropItem.Position = pDropItem->Position;
				pDeleteDropItem->push_back(DeleteDropItem);
			}
			
			util::del(pDropItem->pItem);
			m_DropItemMtmap.erase(itr++);
		}
		else
		{
			itr++;
		}
	}
}
// 2012-03-05 by hskim, 드랍 아이템 일정 시간후 삭제
