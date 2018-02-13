// CMemoryHackHistory.h: interface for the CMemoryHackHistory class.
// 2012-12-14 by hskim, 메모리핵 자동 블럭 기능 구현
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMORYHACKHISTORY_H__4BBCD952_1F32_42B4_9A8C_B6A141533165__INCLUDED_)
#define AFX_MEMORYHACKHISTORY_H__4BBCD952_1F32_42B4_9A8C_B6A141533165__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MEMORY_HACK_HISTORY_THRESHOLD		20

typedef enum HISTORY_ITEMKIND_TYPE
{
	HISTORY_ITEMKIND_TYPE_PRIMARY_WEAPON	= 0,		// 1형 무기
	HISTORY_ITEMKIND_TYPE_SECONDARY_WEAPON	= 1,		// 2형 무기
};


class CMemoryHackHistory
{
public:
	CMemoryHackHistory();
	virtual ~CMemoryHackHistory();

	void Clear();
	void SetBlockThreshold(int nThreshold);
	void InsertResult(int nHackCheckType, BYTE nItemKind, int bResult);
	BOOL IsBlock(int nHackCheckType, BYTE nItemKind);
	BOOL IsValidHackCheckType(int nHackCheckType);
	BYTE GetItemKindType(BYTE nItemKind);
	BOOL IsIgnore(int nHackCheckType);

private:
	int m_nBlockThreshold;
	BYTE m_nHistory[ITEMKIND_ALL_ITEM_END][HACK_CHECK_TYPE_END];
};

#endif // !defined(AFX_MEMORYHACKHISTORY_H__4BBCD952_1F32_42B4_9A8C_B6A141533165__INCLUDED_)
