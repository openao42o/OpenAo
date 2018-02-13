#include "stdafx.h"
#include "DropItemManager.h"
#include "FieldIOCP.h"

CDropItemManager::CDropItemManager()
{
	Reset();
}

void CDropItemManager::Reset()
{
	m_pFieldIOCP9		= NULL;
	m_pFieldDBManager	= NULL;	
}

BOOL CDropItemManager::InitDropItemManager(CFieldIOCP *i_pFieldIOCP, CAtumFieldDBManager *i_pFieldDBManager)
{
	m_pFieldIOCP9		= i_pFieldIOCP;
	m_pFieldDBManager	= i_pFieldDBManager;

	return TRUE;
}

void CDropItemManager::CleanDropItemManager()
{
	Reset();
}

BOOL CDropItemManager::DropItem(CFieldMapChannel *i_pFieldMapChannel, D3DXVECTOR3 *i_pDropPosition, INT i_nItemNum, INT i_nCountableItemCount /* = 1 */, BOOL i_bAdminCommand /* = FALSE */)		// 2013-04-10 by hskim, 어드민 명령에 의한 드랍 아이템은 대기 시간 2초 적용
{
	// find item info
	ITEM *pItemInfo = m_pFieldIOCP9->GetItemInfo(i_nItemNum);
	if (pItemInfo == NULL)
	{
		return FALSE;
	}

	// make new item
	ITEM_GENERAL *pNewItemGeneral = new ITEM_GENERAL(pItemInfo);

	if (IS_COUNTABLE_ITEM(pItemInfo->Kind))
	{
		// countable item인 경우
		pNewItemGeneral->CurrentCount = i_nCountableItemCount;
	}

	// drop item 할당
	DROPITEM dropItem;

	dropItem.pItem				= pNewItemGeneral;
	dropItem.dwStartTick		= m_pFieldIOCP9->GetCurrentServerTimeInMilliSeconds();
	dropItem.FirstCharacterUID1	= 0;
	dropItem.Position			= *i_pDropPosition;
	dropItem.bAdminCommand		= i_bAdminCommand;		// 2013-04-10 by hskim, 어드민 명령에 의한 드랍 아이템은 대기 시간 2초 적용

	CMapBlock *pMapBlock = i_pFieldMapChannel->GetBlock(i_pDropPosition->x, i_pDropPosition->z);
	pMapBlock->InsertDropItem(pNewItemGeneral, &dropItem);

	INIT_MSG_WITH_BUFFER(MSG_FC_ITEM_SHOW_ITEM, T_FC_ITEM_SHOW_ITEM, pMsgShowItem, pMsgShowItemBuf);
	pMsgShowItem->ItemFieldIndex	= (UINT)dropItem.pItem;
	pMsgShowItem->ItemNum			= dropItem.pItem->ItemNum;
	pMsgShowItem->FirstCharacterUID	= dropItem.FirstCharacterUID1;
	pMsgShowItem->Amount			= dropItem.pItem->CurrentCount;
	pMsgShowItem->Position			= dropItem.Position;
	pMsgShowItem->DropItemType		= DROP_ITEM_TYPE_PICKUP;
	m_pFieldIOCP9->SendInRangeMessageAroundPosition(*i_pDropPosition, pMsgShowItemBuf, MSG_SIZE(MSG_FC_ITEM_SHOW_ITEM), i_pFieldMapChannel);
	return TRUE;
}

BOOL CDropItemManager::DropItemInRandomRadius(CFieldMapChannel *i_pFieldMapChannel, D3DXVECTOR3 *i_pDropPosition, INT i_nRadius, INT i_nItemNum, INT i_nCountableItemCount /* = 1 */, BOOL i_bAdminCommand /* = FALSE */)		// 2013-04-10 by hskim, 어드민 명령에 의한 드랍 아이템은 대기 시간 2초 적용
{
	D3DXVECTOR3 RadomPosition;
	i_pFieldMapChannel->m_pFieldMapProject->GetRandomPositionInRadius(i_pDropPosition, &RadomPosition, i_nRadius);

	return DropItem(i_pFieldMapChannel, &RadomPosition, i_nItemNum, i_nCountableItemCount, i_bAdminCommand);		// 2013-04-10 by hskim, 어드민 명령에 의한 드랍 아이템은 대기 시간 2초 적용
}
