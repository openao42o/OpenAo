// VMemPool.cpp: implementation of the CSaveLastError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VMemPool.h"
#include "VMem.h"

struct SObjectInfo
{
	DWORD	dwObjTypeSize;
	DWORD	dwObjPoolSize;
};

constexpr DWORD	SIZE_MAX_VIRTUALPOOL_ARRAY = 12;
constexpr DWORD SIZE_MAX_OBJECTTYPE = 65536;	// 최대 Object Buffer Size 65536 Bytes

//////////////////////////////////////////////////////////////////////
//	Static Variable
//////////////////////////////////////////////////////////////////////
namespace VMemPool
{
	CVMem* vmSearchValidVMem(DWORD dwObjSize);

	SObjectInfo		ms_ArrayObjInfo[SIZE_MAX_VIRTUALPOOL_ARRAY];	// 메모리 풀을 사용할 Object 정보 배열
	DWORD			ms_dwCountArrayObjInfo = 0;							// 메모리 풀을 사용할 Object 정보 배열 카운트
	CVMem			ms_ArrayVMem[SIZE_MAX_VIRTUALPOOL_ARRAY];		// 메모리 풀 배열
	DWORD			ms_dwSizeArrayVMem = 0;								// 사용중인 메모리 풀 배열 사이즈
	BOOL			ms_bFlagVMemInit = FALSE;								// 메모리풀 초기화 플래그
}

//////////////////////////////////////////////////////////////////////
// Global Variable/Function
//////////////////////////////////////////////////////////////////////
bool operator<(SObjectInfo lhObj, SObjectInfo rhObj)
{
	return lhObj.dwObjTypeSize < rhObj.dwObjTypeSize;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : VMemPool::vmObjectNew
// 반환되는 형  : void *
// 함 수 인 자  : DWORD dwObjSize
// 함 수 설 명  : inline static 함수
//					메모리풀에서 사이즈에 맞는 메모리를 할당하고 그 주소를 리턴한다.
void* VMemPool::vmObjectNew(DWORD dwObjSize)
{
	auto pVMem = vmSearchValidVMem(dwObjSize);
	if (pVMem) return pVMem->ObjectNew();

	char	szError[1024];
	sprintf(szError, "VMemPool::vmObjectNew error, FlagVMemInit[%2d] SizeArrayVMem[%2d] ObjSize[%4d]\r\n"
		, ms_bFlagVMemInit, ms_dwSizeArrayVMem, dwObjSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : VMemPool::vmObjectDelete
// 반환되는 형  : void
// 함 수 인 자  : void *p
// 함 수 인 자  : DWORD dwObjSize
// 함 수 설 명  : inline static 함수
//					메모리풀에서 인자로 받은 주소를 해제한다.
void VMemPool::vmObjectDelete(void *p, DWORD dwObjSize)
{
	auto pVMem = vmSearchValidVMem(dwObjSize);

	if (pVMem)
	{
		pVMem->ObjectDelete(p);
		return;
	}

	char szError[1024];
	sprintf(szError, "VMemPool::vmObjectDelete error, FlagVMemInit[%2d] SizeArrayVMem[%2d] p[%p] ObjSize[%4d]\r\n", ms_bFlagVMemInit, ms_dwSizeArrayVMem, p, dwObjSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
}


////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : VMemPool::vmSearchValidVMem
// 반환되는 형  : CVMem*
// 함 수 인 자  : DWORD dwObjSize
// 함 수 설 명  : inline static 함수
//					메모리 풀에서 ObjectTypeSize에 맞는 CVMem의 객체를 찾아서 리턴하는 함수
CVMem* VMemPool::vmSearchValidVMem(DWORD dwObjSize)
{
	for (auto i = 0; i < ms_dwSizeArrayVMem; i++)

		if (ms_ArrayObjInfo[i].dwObjTypeSize == dwObjSize) return &ms_ArrayVMem[i];

	char szError[1024];
	char szTemp[512];
	sprintf(szError, "VMemPool::vmSearchValidVMem error, FlagVMemInit[%2d] SizeArrayVMem[%2d],", ms_bFlagVMemInit, ms_dwSizeArrayVMem);

	for (auto i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		sprintf(szTemp, " %d", ms_ArrayObjInfo[i].dwObjTypeSize);
		strcat(szError, szTemp);
	}
	strcat(szError, "\r\n");
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : VMemPool::vmPoolAddObject
// 반환되는 형  : BOOL
// 함 수 인 자  : DWORD dwObjTypeSize
// 함 수 인 자  : DWORD dwObjPoolSize
// 함 수 설 명  : static 함수
//					메로리 풀을 할당하기 전에 메모리를 할당할 Object의 타입과 사이즈를 추가하는 함수
BOOL VMemPool::vmPoolAddObject(DWORD dwObjTypeSize, DWORD dwObjPoolSize)
{
	if (dwObjTypeSize == 0 || (dwObjTypeSize & 3) != 0 || dwObjTypeSize > SIZE_MAX_OBJECTTYPE || dwObjTypeSize * dwObjPoolSize > 1024 * 1024 * 100)
	{
		// bad parameters
		
		char	szError[1024];
		sprintf(szError, "VMemPool::vmPoolAddObject invalid parameter, ObjTypeSize[%4d] ObjPoolSize[%d]\r\n", dwObjTypeSize, dwObjPoolSize);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	
	BOOL bAddedFlag = FALSE;	
	for(DWORD i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		if(ms_ArrayObjInfo[i].dwObjTypeSize == dwObjTypeSize)
		{
			if(ms_ArrayObjInfo[i].dwObjTypeSize * (ms_ArrayObjInfo[i].dwObjPoolSize + dwObjPoolSize) > 1024*1024*100)
			{
				return FALSE;
			}
			ms_ArrayObjInfo[i].dwObjPoolSize += dwObjPoolSize;
			bAddedFlag = TRUE;
			break;
		}
	}

	if (!bAddedFlag && ms_dwCountArrayObjInfo < SIZE_MAX_VIRTUALPOOL_ARRAY - 1)
	{
		ms_ArrayObjInfo[ms_dwCountArrayObjInfo].dwObjTypeSize = dwObjTypeSize;
		ms_ArrayObjInfo[ms_dwCountArrayObjInfo].dwObjPoolSize = dwObjPoolSize;
		ms_dwCountArrayObjInfo++;
		bAddedFlag = TRUE;
		
		if (ms_dwCountArrayObjInfo > 1) sort(&ms_ArrayObjInfo[0], &ms_ArrayObjInfo[ms_dwCountArrayObjInfo]);
	}

	return bAddedFlag;
}

////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : VMemPool::vmPoolInit
// 반환되는 형  : BOOL
// 함 수 인 자  : void
// 함 수 설 명  : static 함수
//					메모리 풀을 위한 메모리를 할당한다.
BOOL VMemPool::vmPoolInit()
{
	if(ms_bFlagVMemInit 
		|| !ms_dwCountArrayObjInfo 
		|| ms_dwCountArrayObjInfo >= SIZE_MAX_VIRTUALPOOL_ARRAY)
	{
		char	szError[1024];
		sprintf(szError, "VMemPool::vmPoolInit invalid variable, FlagVMemInit[%2d] CountArrayObjInfo[%2d]\r\n"
			, ms_bFlagVMemInit, ms_dwCountArrayObjInfo);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	BOOL bRet;
	for(UINT i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		bRet = ms_ArrayVMem[i].AllocVMem(ms_ArrayObjInfo[i].dwObjTypeSize, ms_ArrayObjInfo[i].dwObjPoolSize);
		if(FALSE == bRet)
		{
			return FALSE;
		}
	}
	ms_dwSizeArrayVMem	= ms_dwCountArrayObjInfo;
	ms_bFlagVMemInit	= TRUE;

	return ms_bFlagVMemInit;
}



////////////////////////////////////////////////////////////////////////////////
//
// 함 수 이 름  : VMemPool::vmPoolClean
// 반환되는 형  : void
// 함 수 설 명  : 메모리 풀의 메로리를 해제한다.
//
void VMemPool::vmPoolClean()
{
	if(FALSE == ms_bFlagVMemInit)
	{
		char	szError[1024];
		sprintf(szError, "VMemPool::vmPoolClean invalid variable, FlagVMemInit[%2d]\r\n"
			, ms_bFlagVMemInit);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);		
		return;
	}
	ms_bFlagVMemInit = FALSE;
	for(UINT i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		ms_ArrayVMem[i].FreeAllVMem();
	}
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
void VMemPool::vmPoolSaveCurrentStatus()
{
	for(DWORD i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		CVMem *pVMem = &ms_ArrayVMem[i];
		pVMem->SaveCurrentStatus();
	}	
}

