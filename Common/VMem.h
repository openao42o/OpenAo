#pragma once

#include <vector>

#ifndef DBGOUT
#define DBGOUT (void(0))
#endif

using namespace std;

typedef ez_map<void*, BYTE>		ezmapVoidPtr8BYTE;		// 2008-07-24 by cmkwon, 메모리풀 체크 추가(디버그 모드시만) - 

class CVMem
{
public:
	CVMem();
	~CVMem();

	BOOL AllocVMem(DWORD dwTypeSize, DWORD dwPoolCount);
	BOOL FreeAllVMem();
	BOOL FreeVMem(void * pVMem);
	void SaveCurrentStatus(void);			// 2009-05-04 by cmkwon, 메모리풀 상태정보 로그 남기기 - 

	inline void *ObjectNew();
	inline void ObjectDelete(void *p);

private:
	inline BOOL CommitVMem(void *pAddr, DWORD dwSize);
	inline BOOL DecommitVMem(void *pAddr, DWORD dwSize);

	void LockVMem() { EnterCriticalSection(&m_csLock); }
	void UnlockVMem() { LeaveCriticalSection(&m_csLock); }

	inline BOOL InsertFree(void *p);
	inline void* PopFree();

	CRITICAL_SECTION	m_csLock;						// Virtual Memory의 동기화를 위한 크리티컬 섹션
	DWORD				m_dwSizeObjType;				// Virtual Memory에 할당되는 하나의 객체 사이즈 단위
	DWORD				m_dwCntCommitedObjPool;
	DWORD				m_dwCntCurrentAllocObjPool;		//
	DWORD				m_dwSizeCommitVMem;
	DWORD				m_dwTotalCommitedObjCnts;		// 2008-04-11 by cmkwon, 메모리풀 시스템 로그 추가 - 

	vector<void *>		m_vectorVMemPtr;
	vector<void *>		m_vectorMemoryPointer;
#if defined(_DEBUG)
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, 메모리풀 체크 추가(디버그 모드시만) - 
	ezmapVoidPtr8BYTE	m_ezmapUseFlagList;				// 2008-07-24 by cmkwon, 메모리풀 체크 추가(디버그 모드시만) - 
#endif
};


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMem::CommitVMem
// 반환되는 형  : BOOL
// 함 수 인 자  : void *pAddr
// 함 수 인 자  : DWORD dwSize
// 함 수 설 명  : inline 함수
//					예약된 주소를 사용하기 위해 Commit하는 함수
BOOL CVMem::CommitVMem(void *pAddr, DWORD dwSize)
{
	void * pCommit = VirtualAlloc(pAddr, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if(NULL == pCommit)
	{
		int nErr = GetLastError();
		SetLastError(0);

		server::log("CVMem::CommitVMem VirtualAlloc() LastError[%d] pAddr[%p] TotalSize[%d]\r\n", nErr, pAddr, dwSize);

		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMem::DecommitVMem
// 반환되는 형  : BOOL
// 함 수 인 자  : void *pAddr
// 함 수 인 자  : DWORD dwSize
// 함 수 설 명  : inline 함수
//					Commit 중인 메모리를 Decommit하기 위한 함수
BOOL CVMem::DecommitVMem(void *pAddr, DWORD dwSize)
{
	BOOL bRet = VirtualFree(pAddr, dwSize, MEM_DECOMMIT);
	if(FALSE == bRet)
	{
		int nErr = GetLastError();
		SetLastError(0);

		server::log("CVMem::DecommitVMem VirtualAlloc() LastError[%d] pAddr[%p] TotalSize[%d]\r\n", nErr, pAddr, dwSize);

		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMem::InsertFree
// 반환되는 형  : BOOL
// 함 수 인 자  : DWORD id
// 함 수 설 명  : inline 함수
//					메모리가 해제되면 Stack에 해제된 메모리에 해당하는 id를 추가하는 함수
BOOL CVMem::InsertFree(void *p)
{
//	vector<void *>::iterator itr = find(m_vectorMemoryPointer.begin(), m_vectorMemoryPointer.end(), p);
//	if(itr != m_vectorMemoryPointer.end())
//	{
//		return FALSE;
//	}

#if defined(_DEBUG)
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, 메모리풀 체크 추가(디버그 모드시만) - 
	BYTE *pbyUseFlag = m_ezmapUseFlagList.findEZ_ptr(p);
	if(NULL == pbyUseFlag)
	{
		m_ezmapUseFlagList.insertEZ(p, FALSE);
	}
	else
	{
		if(FALSE == *pbyUseFlag)
		{
			server::log(TRUE, "[ERROR] MemPool InsertFree error !!, m_dwSizeObjType(%d)\r\n", m_dwSizeObjType);
			return FALSE;
		}
		*pbyUseFlag = FALSE;
	}
#endif

	m_vectorMemoryPointer.push_back(p);
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMem::PopFree
// 반환되는 형  : DWORD
// 함 수 설 명  : inline 함수
//					메모리 할당을 위해 Stack에서 id를 꺼내어 리턴하는 함수
void * CVMem::PopFree()
{
	if(m_vectorMemoryPointer.empty() == true)
	{	// 비어있다. Underflow

		if(AllocVMem(m_dwSizeObjType, m_dwCntCommitedObjPool) == FALSE)
		{
			char	szError[1024];
			sprintf(szError, "CVMem::PopFree Queue Underflow MemoryPointerSize[%d]\r\n", m_vectorMemoryPointer.size());
			server::log(szError);
			DBGOUT(szError);
			return NULL;
		}
	}
	void *p = m_vectorMemoryPointer.back();
	m_vectorMemoryPointer.pop_back();

#if defined(_DEBUG)
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, 메모리풀 체크 추가(디버그 모드시만) - 
	BYTE *pbyUseFlag = m_ezmapUseFlagList.findEZ_ptr(p);
	if(NULL == pbyUseFlag)
	{
		m_ezmapUseFlagList.insertEZ(p, TRUE);
	}
	else
	{		
		*pbyUseFlag		= TRUE;
	}
#endif

	return p;
}



////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMem::ObjectNew
// 반환되는 형  : void *
// 함 수 설 명  : inline 함수
//					메모리 풀에서 메모리를 하나 할당하여 리턴하는 함수
void * CVMem::ObjectNew()
{
	LockVMem();
	void* pNew = PopFree();					// 쓸수있는 블럭을 주소로 얻는다.
	if(pNew != NULL)
	{
		m_dwCntCurrentAllocObjPool++;
	}
	UnlockVMem();

	return pNew;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : CVMem::ObjectDelete
// 반환되는 형  : void
// 함 수 인 자  : void *p
// 함 수 설 명  : inline 함수
//					사용한 메모리를 해제하는 함수(메모리 풀에 추가됨)
void CVMem::ObjectDelete(void *p)
{
	LockVMem();
	if(InsertFree(p))
	{
		m_dwCntCurrentAllocObjPool--;
	}
	UnlockVMem();
}