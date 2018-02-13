// BunchCountableItemCount.h: interface for the CBunchCountableItemCount class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUNCHCOUNTABLEITEMCOUNT_H__712362E9_AACB_4F0B_A6CC_AC90A5F5D646__INCLUDED_)
#define AFX_BUNCHCOUNTABLEITEMCOUNT_H__712362E9_AACB_4F0B_A6CC_AC90A5F5D646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SBUNCH_ITEM_COUNT
{
	INT			nItemNum;
	INT			nBunchItemCount;

	SBUNCH_ITEM_COUNT(int i_nItemNum, int i_nCounts)
	{
		nItemNum			= i_nItemNum;
		nBunchItemCount		= i_nCounts;
	}
};
typedef vector<SBUNCH_ITEM_COUNT>		vectBUNCH_ITEM_COUNT;

class CBunchCountableItemCount  
{
public:
	CBunchCountableItemCount();
	~CBunchCountableItemCount();

	int	GetTotalBunchCountableItemCounts(void);
	int GetInventoryCounts(void);

	int AddBunchCountableItemByItemNum(int i_nItemNum);

protected:
	vectBUNCH_ITEM_COUNT	m_vectBUNCH_ITEM_COUNTList;	
};

#endif // !defined(AFX_BUNCHCOUNTABLEITEMCOUNT_H__712362E9_AACB_4F0B_A6CC_AC90A5F5D646__INCLUDED_)
