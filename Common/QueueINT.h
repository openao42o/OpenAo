#pragma once

#include "mt_stl.h"

using mtvectorINT = mt_vector<int>;

class CQueueINT
{
public:
	CQueueINT();
	~CQueueINT();

	bool IsValidDataNum(int i_nNum) const;

	bool InitQueueINT(int i_nMinNum, int i_nMaxNum);
	void ResetQueueINT();

	bool PopQueueINT(int *o_pNum);
	bool PushQueueINT(int i_nNum);

protected:
	mtvectorINT			m_mtvectorINTList;
	int					m_nFrontIndex;
	int					m_nBackIndex;
	int					m_nQueueSize;
	int					m_nQueueMinNum;
	int					m_nQueueMaxNum;
};