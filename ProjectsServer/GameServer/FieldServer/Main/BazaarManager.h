// BazaarManager.h: interface for the CBazaarManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAZAARMANAGER_H__2B8953CB_8E19_4536_9ED8_94D6D336406A__INCLUDED_)
#define AFX_BAZAARMANAGER_H__2B8953CB_8E19_4536_9ED8_94D6D336406A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



struct SBazaarItem
{
	int				nBuyItemIndex;		// 2006-07-26 by cmkwon, 
	ITEM			*pBazaarItemInfo;
	int				nItemCnts;			// 2006-07-25 by cmkwon, 판매 혹은 구입 개수
	int				nItemPriceOfEach;	// 2006-07-25 by cmkwon, 1개의 가격
	ITEM_GENERAL	*pItemGeneral;		// 2006-07-25 by cmkwon, 판매상점 일 경우만 유효함

	SBazaarItem(int i_nBuyItemIdx, ITEM *i_pItem, int i_nCnts, int i_nPriceOfEach, ITEM_GENERAL *i_pItemG=NULL)
	{
		nBuyItemIndex		= i_nBuyItemIdx;
		pBazaarItemInfo		= i_pItem;
		nItemCnts			= i_nCnts;
		nItemPriceOfEach	= i_nPriceOfEach;
		pItemGeneral		= i_pItemG;
	}
};
typedef mt_vector<SBazaarItem>		mtvectSBazaarItem;

class CFieldIOCPSocket;
class CBazaarManager  
{
public:
	CBazaarManager();
	~CBazaarManager();
	void ResetBazaarManager(void);

	void SetFieldIOCPSocket(CFieldIOCPSocket *i_pFISock);
	mtvectSBazaarItem *GetBazaarItemListPtr(void);
	BYTE GetBazaarType(void);
	void SetBazaarType(BYTE i_byBazaarTy);
	BYTE GetBazaarState(void);
	void SetBazaarState(BYTE i_byBazaarState);
	void SetBazaarTypeAndState(BYTE i_byBazaarTy, BYTE i_byBazaarState);
	void SetBazaarName(char *i_szBazaarName);
	BOOL IsEmptyBazaar(void);

	SBazaarItem *FindBazaarItemByItemNumNoLock(int i_nItemNum);
	SBazaarItem *FindBazaarItemByItemUIDNoLock(int i_itemUID);
	SBazaarItem *FindBazaarItemByBazaarIndexNoLock(int i_nBazaarIdx);
	INT64 GetTotalPrice(void);
	BOOL MakeMSG_FC_BAZAAR_INFO_OK(MSG_FC_BAZAAR_INFO_OK *o_pBazaarInfoOK);
	BOOL DeleteBazaarItem(int i_nBuyItemIndex);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-25 by cmkwon, 판매 상점 관련
	Err_t AddSellItem(ITEM_GENERAL *i_pSellItemG, int i_nSellCnts, int i_nPriceOfEach);
	int MakeMSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK(SBAZAAR_SELL_ITEM *o_pSellItem, vectUID64_t *o_pvectUID64ListForEnchant);
	int MakeMSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK(SBAZAAR_BUY_ITEM *o_pBuyItem);

	///////////////////////////////////////////////////////////////////////////////
	// 2006-07-25 by cmkwon, 구매 상점 관련
	Err_t AddBuyItem(int *o_pBuyItemIndex, ITEM *i_pItem, int i_nBuyCnts, int i_nPriceOfEach);

private:
	ClientIndex_t			m_szClientIdx;			// 2006-07-28 by dhjin
	BYTE					m_byBazaarType;			// 2006-07-25 by cmkwon, BAZAAR_TYPE_XXX
	BYTE					m_byBazaarState;		// 2006-07-26 by cmkwon
	int						m_nBuyItemIndex;		// 2006-07-26 by cmkwon
	char					m_szBazaarName[SIZE_MAX_BAZAAR_NAME];
	mtvectSBazaarItem		m_mtvectSBazaarItemList;
	CFieldIOCPSocket		*m_pFieldIOCPSocket1;		

};

#endif // !defined(AFX_BAZAARMANAGER_H__2B8953CB_8E19_4536_9ED8_94D6D336406A__INCLUDED_)
