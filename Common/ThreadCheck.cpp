// ThreadCheck.cpp: implementation of the CThreadCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadCheck.h"
#include "timeapi.h"

const char* SThreadInfo::GetThreadCheckComment(EN_THREAD_CHECK_t i_enThreadType)
{
	switch (i_enThreadType)
	{
	case THREAD_CHECK_TYPE_IOCP_WORKER:
		return "THREAD_CHECK_TYPE_IOCP_WORKER";
	case THREAD_CHECK_TYPE_IOCP_WRITER:
		return "THREAD_CHECK_TYPE_IOCP_WRITER";
	case THREAD_CHECK_TYPE_IOCP_LISTENER:
		return "THREAD_CHECK_TYPE_IOCP_LISTENER";
	case THREAD_CHECK_TYPE_DB_WORKER:
		return "THREAD_CHECK_TYPE_DB_WORKER";

	default:
		return "THREAD_CHECK_TYPE_UNKNOWN";
	}
}

CThreadCheck::CThreadCheck()
{
	m_nSettingThreadCounts = 0;
	m_bSettingCompletionFlag = FALSE;
	m_dwCheckTick = 30000;	// 30ÃÊ
	m_ThreadInfoDB.reserve(10);
}

SThreadInfo* CThreadCheck::GetThreadInfo(DWORD threadId)
{
	if (!m_bSettingCompletionFlag) return nullptr;

	for (auto& info : m_ThreadInfoDB)

		if (info.dwThreadId == threadId) return &info;

	return nullptr;
}

BOOL CThreadCheck::AddThreadInfo(SThreadInfo threadInfo)
{
	m_ThreadInfoDB.lock();

	if (m_bSettingCompletionFlag || GetThreadInfo(threadInfo.dwThreadId))
	{
		m_ThreadInfoDB.unlock();
		return FALSE;
	}

	m_ThreadInfoDB.push_back(threadInfo);

	if (m_nSettingThreadCounts == m_ThreadInfoDB.size())

		m_bSettingCompletionFlag = TRUE;

	m_ThreadInfoDB.unlock();

	return TRUE;
}

SThreadInfo* CThreadCheck::CheckThreadInfo(DWORD i_dwThreadIdToExclude)
{
	if (!m_bSettingCompletionFlag) return nullptr;

	auto bCheckValid = i_dwThreadIdToExclude == 0;
	auto dwCurTick = timeGetTime();

	for (auto& info : m_ThreadInfoDB)
	{
		if (bCheckValid)
		{
			if (info.dwThreadId != i_dwThreadIdToExclude && info.bThreadUseFlag
				&& (dwCurTick - info.dwLastUseStartTick) > m_dwCheckTick)
				
				return &info;
		}
		else if (info.dwThreadId == i_dwThreadIdToExclude) bCheckValid = true;
	}

	return nullptr;
}