// BazaarManager.cpp: implementation of the CBazaarManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BazaarManager.h"
#include "AtumError.h"
#include "FieldIOCPSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBazaarManager::CBazaarManager()
{
	this->ResetBazaarManager();
}

CBazaarManager::~CBazaarManager()
{

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBazaarManager::ResetBazaarManager(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBazaarManager::ResetBazaarManager(void)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	m_byBazaarType			= BAZAAR_TYPE_NONE;
	m_byBazaarState			= BAZAAR_STATE_NONE;
	m_nBuyItemIndex			= 0;
//	m_szClientIdx			= 0;

	util::zero(m_szBazaarName, SIZE_MAX_BAZAAR_NAME);
	m_mtvectSBazaarItemList.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBazaarManager::SetFieldIOCPSocket(CFieldIOCPSocket *i_pFISock)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-31 ~ 2006-07-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBazaarManager::SetFieldIOCPSocket(CFieldIOCPSocket *i_pFISock)
{
	m_pFieldIOCPSocket1		= i_pFISock;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			mtvectSBazaarItem *CBazaarManager::GetBazaarItemListPtr(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
mtvectSBazaarItem *CBazaarManager::GetBazaarItemListPtr(void)
{
	return &m_mtvectSBazaarItemList;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CBazaarManager::GetBazaarType(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CBazaarManager::GetBazaarType(void)
{
	return m_byBazaarType;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBazaarManager::SetBazaarType(BYTE i_byBazaarTy)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBazaarManager::SetBazaarType(BYTE i_byBazaarTy)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);
	m_byBazaarType		= i_byBazaarTy;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BYTE CBazaarManager::GetBazaarState(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CBazaarManager::GetBazaarState(void)
{
	return m_byBazaarState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBazaarManager::SetBazaarState(BYTE i_byBazaarState)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBazaarManager::SetBazaarState(BYTE i_byBazaarState)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);
	m_byBazaarState			= i_byBazaarState;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBazaarManager::SetBazaarTypeAndState(BYTE i_byBazaarTy, BYTE i_byBazaarState)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBazaarManager::SetBazaarTypeAndState(BYTE i_byBazaarTy, BYTE i_byBazaarState)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);
	this->SetBazaarType(i_byBazaarTy);
	this->SetBazaarState(i_byBazaarState);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CBazaarManager::SetBazaarName(char *i_szBazaarName)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CBazaarManager::SetBazaarName(char *i_szBazaarName)
{	
	util::strncpy(m_szBazaarName, i_szBazaarName, SIZE_MAX_BAZAAR_NAME);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CBazaarManager::IsEmptyBazaar(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CBazaarManager::IsEmptyBazaar(void)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);
	return m_mtvectSBazaarItemList.empty();
}



struct Sfind_if_SBazaarItem_BY_ItemNum
{
	Sfind_if_SBazaarItem_BY_ItemNum(int i_nItemNum): m_nItemNum0(i_nItemNum){};
	bool operator()(const SBazaarItem bazaarItem)
	{
		return bazaarItem.pBazaarItemInfo->ItemNum == m_nItemNum0;
	}
	int m_nItemNum0;
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			SBazaarItem *CBazaarManager::FindBazaarItemByItemNumNoLock(int i_nItemNum)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SBazaarItem *CBazaarManager::FindBazaarItemByItemNumNoLock(int i_nItemNum)
{
	mtvectSBazaarItem::iterator itr = find_if(m_mtvectSBazaarItemList.begin(), m_mtvectSBazaarItemList.end()
		, Sfind_if_SBazaarItem_BY_ItemNum(i_nItemNum));
	if(itr == m_mtvectSBazaarItemList.end())
	{
		return NULL;
	}

	return &*itr;
}

struct Sfind_if_SBazaarItem_BY_ItemUID
{
	Sfind_if_SBazaarItem_BY_ItemUID(UID64_t i_itemUID): m_ItemUID0(i_itemUID){};
	bool operator()(const SBazaarItem bazaarItem)
	{
		return bazaarItem.pItemGeneral->UniqueNumber == m_ItemUID0;
	}
	UID64_t		m_ItemUID0;
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			SBazaarItem *CBazaarManager::FindBazaarItemByItemUIDNoLock(int i_itemUID)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SBazaarItem *CBazaarManager::FindBazaarItemByItemUIDNoLock(int i_itemUID)
{
	if(BAZAAR_TYPE_SELL != GetBazaarType())
	{
		return NULL;
	}

	mtvectSBazaarItem::iterator itr = find_if(m_mtvectSBazaarItemList.begin(), m_mtvectSBazaarItemList.end()
		, Sfind_if_SBazaarItem_BY_ItemUID(i_itemUID));
	if(itr == m_mtvectSBazaarItemList.end())
	{
		return NULL;
	}

	return &*itr;
}

struct Sfind_if_SBazaarItem_BY_BuyItemIndex
{
	Sfind_if_SBazaarItem_BY_BuyItemIndex(int i_itemIdx): m_nBuyItemIndex(i_itemIdx){};
	bool operator()(const SBazaarItem bazaarItem)
	{
		return bazaarItem.nBuyItemIndex == m_nBuyItemIndex;
	}
	int		m_nBuyItemIndex;
};
///////////////////////////////////////////////////////////////////////////////
/// \fn			SBazaarItem *CBazaarManager::FindBazaarItemByBazaarIndexNoLock(int i_nBazaarIdx)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-27 ~ 2006-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SBazaarItem *CBazaarManager::FindBazaarItemByBazaarIndexNoLock(int i_nBazaarIdx)
{
	mtvectSBazaarItem::iterator itr = find_if(m_mtvectSBazaarItemList.begin(), m_mtvectSBazaarItemList.end()
		, Sfind_if_SBazaarItem_BY_BuyItemIndex(i_nBazaarIdx));
	if(itr == m_mtvectSBazaarItemList.end())
	{
		return NULL;
	}

	return &*itr;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			INT64 CBazaarManager::GetTotalPrice(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT64 CBazaarManager::GetTotalPrice(void)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	INT64 n64RetTotalPrice = 0;
	for(int i=0; i < m_mtvectSBazaarItemList.size(); i++)
	{
		n64RetTotalPrice += (m_mtvectSBazaarItemList[i].nItemCnts * m_mtvectSBazaarItemList[i].nItemPriceOfEach);
	}

	return n64RetTotalPrice;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CBazaarManager::MakeMSG_FC_BAZAAR_INFO_OK(MSG_FC_BAZAAR_INFO_OK *o_pBazaarInfoOK)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CBazaarManager::MakeMSG_FC_BAZAAR_INFO_OK(MSG_FC_BAZAAR_INFO_OK *o_pBazaarInfoOK)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);
	if(BAZAAR_TYPE_NONE == GetBazaarType()
		|| BAZAAR_STATE_ACTIVE != GetBazaarState())
	{
		return FALSE;
	}
	
	o_pBazaarInfoOK->clientIndex0  = m_pFieldIOCPSocket1->GetCharacter()->ClientIndex;
	o_pBazaarInfoOK->byBazaarType	= GetBazaarType();
	util::strncpy(o_pBazaarInfoOK->szBazaarName, m_szBazaarName, SIZE_MAX_BAZAAR_NAME);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CBazaarManager::DeleteBazaarItem(int i_nBuyItemIndex)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CBazaarManager::DeleteBazaarItem(int i_nBuyItemIndex)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	mtvectSBazaarItem::iterator itr = find_if(m_mtvectSBazaarItemList.begin(), m_mtvectSBazaarItemList.end()
		, Sfind_if_SBazaarItem_BY_BuyItemIndex(i_nBuyItemIndex));
	if(itr == m_mtvectSBazaarItemList.end())
	{
		return FALSE;
	}

	m_mtvectSBazaarItemList.erase(itr);

	if(BAZAAR_STATE_ACTIVE == GetBazaarState()
		&& IsEmptyBazaar())
	{
		SetBazaarState(BAZAAR_STATE_END);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CBazaarManager::AddSellItem(ITEM_GENERAL *i_pSellItemG, int i_nSellCnts, int i_nPriceOfEach)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CBazaarManager::AddSellItem(ITEM_GENERAL *i_pSellItemG, int i_nSellCnts, int i_nPriceOfEach)
{
	if(NULL == i_pSellItemG
		|| 0 >= i_nSellCnts
		|| 0 >= i_nPriceOfEach)
	{
		return ERR_DB_INVALID_PARAMETER;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-25 by cmkwon
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	if(BAZAAR_TYPE_SELL != GetBazaarType()
		|| BAZAAR_STATE_READY != GetBazaarState())
	{
		return ERR_PROTOCOL_INVALID_CHARACTER_STATE;
	}

	if(SIZE_MAX_BAZAAR_ITEM_COUNT <= m_mtvectSBazaarItemList.size())
	{
		return ERR_OVER_COUNT;
	}
	
	if(FindBazaarItemByItemUIDNoLock(i_pSellItemG->UniqueNumber))
	{
		return ERR_PROTOCOL_ALREADY_TRADE_ITEM;
	}
	
	m_mtvectSBazaarItemList.push_back(SBazaarItem(m_nBuyItemIndex++, i_pSellItemG->ItemInfo, i_nSellCnts, i_nPriceOfEach, i_pSellItemG));
	return ERR_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CBazaarManager::MakeMSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK(SBAZAAR_SELL_ITEM *o_pSellItem, vectUID64_t *o_pvectUID64ListForEnchant)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CBazaarManager::MakeMSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK(SBAZAAR_SELL_ITEM *o_pSellItem, vectUID64_t *o_pvectUID64ListForEnchant)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	if(BAZAAR_STATE_ACTIVE != GetBazaarState()
		|| BAZAAR_TYPE_SELL != GetBazaarType()
		|| IsEmptyBazaar())
	{
		return 0;
	}

	int nRetItemCnts	= 0;
	int nVectSize		= m_mtvectSBazaarItemList.size();
	for(int i=0; i < nVectSize; i++)
	{
		if(FALSE == IS_COUNTABLE_ITEM(m_mtvectSBazaarItemList[i].pItemGeneral->Kind))
		{
			o_pvectUID64ListForEnchant->push_back(m_mtvectSBazaarItemList[i].pItemGeneral->UniqueNumber);
		}

		if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK) + sizeof(SBAZAAR_SELL_ITEM) * (i+1))
		{// 2007-07-09 by cmkwon, 버퍼 최대 크기 체크
			break;
		}

		o_pSellItem[nRetItemCnts].nSellItemNum0		= m_mtvectSBazaarItemList[i].pBazaarItemInfo->ItemNum;
		o_pSellItem[nRetItemCnts].nSellAmount0		= m_mtvectSBazaarItemList[i].nItemCnts;
		o_pSellItem[nRetItemCnts].nSellEachPrice0	= m_mtvectSBazaarItemList[i].nItemPriceOfEach;
		o_pSellItem[nRetItemCnts].itemUID			= m_mtvectSBazaarItemList[i].pItemGeneral->UniqueNumber;		
		o_pSellItem[nRetItemCnts].PrefixCodeNum0	= m_mtvectSBazaarItemList[i].pItemGeneral->PrefixCodeNum;	
		o_pSellItem[nRetItemCnts].SuffixCodeNum0	= m_mtvectSBazaarItemList[i].pItemGeneral->SuffixCodeNum;	
		// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
		//o_pSellItem[nRetItemCnts].ColorCode0		= m_mtvectSBazaarItemList[i].pItemGeneral->ColorCode;
		o_pSellItem[nRetItemCnts].ShapeItemNum0		= m_mtvectSBazaarItemList[i].pItemGeneral->GetShapeItemNum();	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
		o_pSellItem[nRetItemCnts].EffectItemNum0	= m_mtvectSBazaarItemList[i].pItemGeneral->GetEffectItemNum();	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - 
		o_pSellItem[nRetItemCnts].CoolingTime		= m_mtvectSBazaarItemList[i].pItemGeneral->CoolingTimeStamp;	// 2009-09-09 ~ 2010-02-10 by dhjin, 인피니티 - 발동류장착아이템
	
		memcpy(&o_pSellItem[nRetItemCnts].FixedTermShape, &(m_mtvectSBazaarItemList[i].pItemGeneral->FixedTermShape), sizeof((m_mtvectSBazaarItemList[i].pItemGeneral->FixedTermShape)));		// 2012-10-10 by hskim, 기간제 속성 구현 (기간제 외형)

		nRetItemCnts++;
	}

	return nRetItemCnts;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CBazaarManager::MakeMSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK(SBAZAAR_BUY_ITEM *o_pBuyItem)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-26 ~ 2006-07-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CBazaarManager::MakeMSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK(SBAZAAR_BUY_ITEM *o_pBuyItem)
{
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	if(BAZAAR_STATE_ACTIVE != GetBazaarState()
		|| BAZAAR_TYPE_BUY != GetBazaarType()
		|| IsEmptyBazaar())
	{
		return 0;
	}

	int nRetItemCnts	= 0;
	int nVectSize		= m_mtvectSBazaarItemList.size();
	for(int i=0; i < nVectSize; i++)
	{
		if(SIZE_MAX_PACKET < MSG_SIZE(MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK) + sizeof(SBAZAAR_BUY_ITEM) * (i+1))
		{// 2007-07-09 by cmkwon, 버퍼 최대 크기 체크
			break;
		}

		o_pBuyItem[nRetItemCnts].nBuyItemIndex0		= m_mtvectSBazaarItemList[i].nBuyItemIndex;
		o_pBuyItem[nRetItemCnts].nBuyItemNum0		= m_mtvectSBazaarItemList[i].pBazaarItemInfo->ItemNum;
		o_pBuyItem[nRetItemCnts].nBuyAmount0		= m_mtvectSBazaarItemList[i].nItemCnts;
		o_pBuyItem[nRetItemCnts].nBuyEachPrice0		= m_mtvectSBazaarItemList[i].nItemPriceOfEach;
		nRetItemCnts++;
	}

	return nRetItemCnts;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			Err_t CBazaarManager::AddBuyItem(int *o_pBuyItemIndex, ITEM *i_pItem, int i_nBuyCnts, int i_nPriceOfEach)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-25 ~ 2006-07-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
Err_t CBazaarManager::AddBuyItem(int *o_pBuyItemIndex, ITEM *i_pItem, int i_nBuyCnts, int i_nPriceOfEach)
{
	if(NULL == i_pItem
		|| 0 >= i_nBuyCnts
		|| 0 >= i_nPriceOfEach
		|| MAX_ITEM_COUNTS < i_nBuyCnts * i_nPriceOfEach)
	{
		return ERR_DB_INVALID_PARAMETER;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-25 by cmkwon
	mt_auto_lock mtA(&m_mtvectSBazaarItemList);

	if(BAZAAR_TYPE_BUY != GetBazaarType()
		|| BAZAAR_STATE_READY != GetBazaarState())
	{
		return ERR_PROTOCOL_INVALID_CHARACTER_STATE;
	}

	if(SIZE_MAX_BAZAAR_ITEM_COUNT <= m_mtvectSBazaarItemList.size())
	{
		return ERR_OVER_COUNT;
	}

	if(IS_COUNTABLE_ITEM(i_pItem->Kind)
		&& FindBazaarItemByItemNumNoLock(i_pItem->ItemNum))
	{
		return ERR_PROTOCOL_ALREADY_TRADE_ITEM;
	}

	*o_pBuyItemIndex		= m_nBuyItemIndex;
	m_mtvectSBazaarItemList.push_back(SBazaarItem(m_nBuyItemIndex++, i_pItem, i_nBuyCnts, i_nPriceOfEach));
	return ERR_NO_ERROR;
}


