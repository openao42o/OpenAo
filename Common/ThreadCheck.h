#pragma once

#include "mt_stl.h"

constexpr auto SIZE_MAX_THREAD_COMMENT = 128;

enum EN_THREAD_CHECK_t
{
	THREAD_CHECK_TYPE_UNKNOWN = 0,
	THREAD_CHECK_TYPE_IOCP_WORKER = 1,
	THREAD_CHECK_TYPE_IOCP_WRITER = 2,
	THREAD_CHECK_TYPE_IOCP_LISTENER = 3,
	THREAD_CHECK_TYPE_DB_WORKER = 4
};

struct SThreadInfo
{
	DWORD				dwThreadId;
	EN_THREAD_CHECK_t	enThreadCheckType;
	BOOL				bThreadUseFlag;
	DWORD				dwLastUseStartTick;
	DWORD				dwSocketIndex;
	DWORD				dwMessageType;
	DWORD				dwCharacterUID;
	int					nQueryArraySize;
	BOOL				bPrintOut;
	int					nParam1;				// 2007-02-21 by cmkwon
	int					nParam2;				// 2007-02-21 by cmkwon

	static const char *GetThreadCheckComment(EN_THREAD_CHECK_t i_enThreadType);
};

class CThreadCheck
{
public:
	CThreadCheck();
	virtual ~CThreadCheck() = default;

	BOOL GetSettingCompletionFlag() const { return m_bSettingCompletionFlag; }
	SThreadInfo* GetThreadInfo(DWORD threadId);
	BOOL AddThreadInfo(SThreadInfo threadInfo);
	SThreadInfo* CheckThreadInfo(DWORD i_dwThreadIdToExclude);
	void SetCheckThreadCounts(int nThreadCounts) { m_nSettingThreadCounts = nThreadCounts; };

protected:
	int							m_nSettingThreadCounts;
	BOOL						m_bSettingCompletionFlag;
	DWORD						m_dwCheckTick;
	mt_vector<SThreadInfo>		m_ThreadInfoDB;
};