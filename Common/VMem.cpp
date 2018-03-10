#include "stdafx.h"
#include "VMem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVMem::CVMem() : m_dwSizeObjType { 0 }, m_dwCntCommitedObjPool { 0 }, m_dwCntCurrentAllocObjPool { 0 }, m_dwSizeCommitVMem { 0 }, m_dwTotalCommitedObjCnts { 0 }
{
	InitializeCriticalSection(&m_csLock);
}

CVMem::~CVMem()
{
	DeleteCriticalSection(&m_csLock);
}

BOOL CVMem::AllocVMem(DWORD dwTypeSize, DWORD dwPoolCount)
{	
	LockVMem();
	if(0 == dwTypeSize || 0 == dwPoolCount)
	{
		UnlockVMem();
		
		char	szError[256];
		sprintf(szError, "[Error] CVMem::AllocVMem_1 invalid parameter, TypeSize[%4d] PoolCount[%4d]\r\n"
			, dwTypeSize, dwPoolCount);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}


	const DWORD	dwPAGESIZE = 4096;
	
	//////////////////////////////////////////////////////////////////////
	// 요청한 PoolCount를 Page 크기에 최대한 맞추는 계산
	DWORD dwCalcPoolCnt;
	if((dwTypeSize * dwPoolCount)%dwPAGESIZE != 0)
		dwCalcPoolCnt = (((dwTypeSize * dwPoolCount)/dwPAGESIZE + 1) * dwPAGESIZE)/dwTypeSize;
	else dwCalcPoolCnt = dwPoolCount;
	
	//////////////////////////////////////////////////////////////////////
	// 메모리풀 크기를 Page 단위로 계산
	DWORD dwCommitVMSize = dwTypeSize * dwCalcPoolCnt;
	if(dwCommitVMSize%dwPAGESIZE != 0)
	{	
		dwCommitVMSize = ((dwCommitVMSize/dwPAGESIZE) + 1) * dwPAGESIZE;		
	}
		
	void* pVMem = (char*)VirtualAlloc(NULL, dwCommitVMSize, MEM_RESERVE|MEM_TOP_DOWN, PAGE_READWRITE);
	if(NULL == pVMem)
	{	
		UnlockVMem();

		int nErr = GetLastError();
		SetLastError(0);
		char	szError[256];
		sprintf(szError, "[ERROR] CVMem::AllocVMem VirtualAlloc() LastError[%d] CommitSize[%d]\r\n", nErr, dwCommitVMSize);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		
		return FALSE;
	}
	if(CommitVMem(pVMem, dwCommitVMSize) == FALSE)
	{
		UnlockVMem();
		return FALSE;
	}	
	m_vectorVMemPtr.push_back(pVMem);
	
	char	szError[256];
	sprintf(szError, "CVMem::AllocVMem VirtualAlloc() success, TypeSize[%5d] PoolCnt[%5d] CommitSize[%8d] vectorSize[%d] Address Range(%p ~ %p)\r\n"
		, dwTypeSize, dwCalcPoolCnt, dwCommitVMSize, m_vectorVMemPtr.size()
		, pVMem, (char*)pVMem + dwCommitVMSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	
	m_dwSizeObjType			= dwTypeSize;
	m_dwCntCommitedObjPool	= dwCalcPoolCnt;
	m_dwSizeCommitVMem		= dwCommitVMSize;
	m_dwTotalCommitedObjCnts	+= dwCalcPoolCnt;		// 2008-04-11 by cmkwon, 메모리풀 시스템 로그 추가 - 

	m_vectorMemoryPointer.reserve(m_dwCntCommitedObjPool * 2);
	for(int i = 0; i < m_dwCntCommitedObjPool; i++)
	{
		InsertFree((char*)pVMem + i*m_dwSizeObjType);
	}

	UnlockVMem();
	return TRUE;
}

BOOL CVMem::FreeAllVMem(void)
{
	LockVMem();	

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-11 by cmkwon, 메모리풀 시스템 로그 추가 - 
	g_pGlobal->WriteSystemLogEX(TRUE, "[Notify] CVMem::FreeAllVMem_ TypeSize[%5d] TotalCommitedObjCnts(%6d) CurrentObjCnts(%6d)\r\n"
		, m_dwSizeObjType, m_dwTotalCommitedObjCnts, m_vectorMemoryPointer.size());

	m_vectorMemoryPointer.clear();

	vector<void *>::iterator itr = m_vectorVMemPtr.begin();
	while(itr != m_vectorVMemPtr.end())
	{
		this->FreeVMem(*itr);
		itr++;
	}

	m_dwSizeObjType				= 0;
	m_dwCntCommitedObjPool		= 0;
	m_dwCntCurrentAllocObjPool	= 0;
	m_dwSizeCommitVMem			= 0;	
	UnlockVMem();

	return TRUE;
}

BOOL CVMem::FreeVMem(void * pVMem)
{
	if(NULL == pVMem)
	{
		char	szError[1024];
		sprintf(szError, "CVMem::FreeVMem invalid variable, pVMem[%p]\r\n", pVMem);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	
	if(VirtualFree(pVMem, 0, MEM_RELEASE) == FALSE)
	{
		int nErr = GetLastError();
		SetLastError(0);
		
		char	szError[1024];
		sprintf(szError, "CVMem::FreeVMem VirtualFree Failure LastError[%d] pVMem[%p]\r\n", nErr, pVMem);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
	}
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-05-04 by cmkwon, 메모리풀 상태정보 로그 남기기 - 
/// \author		cmkwon
/// \date		2009-05-04 ~ 2009-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CVMem::SaveCurrentStatus(void)
{
	LockVMem();
	{
		g_pGlobal->WriteSystemLogEX(TRUE, "[Notify] CVMem TypeSize[%5d] TotolCommitedCnts[%6d] CurrentPoolCnts[%6d] \r\n"
			, this->m_dwSizeObjType, this->m_dwTotalCommitedObjCnts, this->m_vectorMemoryPointer.size());
	}
	UnlockVMem();
}
