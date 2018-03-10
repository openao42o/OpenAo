// MemPool.h: interface for the CMemPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMPOOLCLIENT_H__50553B14_1B10_47A0_A3BB_2EEAD90A84B7__INCLUDED_)
#define AFX_MEMPOOLCLIENT_H__50553B14_1B10_47A0_A3BB_2EEAD90A84B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VMemClient.h"

typedef struct SOBJECTMEM
{
	DWORD	dwObjTypeSize;					// OBJECT SIZE
	DWORD	dwObjPoolSize;					// OBJECT를 할당한 SIZE
	SOBJECTMEM()
	{
		dwObjTypeSize = 0;
		dwObjPoolSize = 0;
	}
}SObjectMem;

// 2008-03-10 by dgwoo 메모리 풀 오프벡트는 12개까지 가능하다.
#define		DEFUALT_MEMEORYPOOL_OBJECTSIZE						  12

class CMemPoolClient		
{
public:
	CMemPoolClient();
	virtual ~CMemPoolClient();

	static BOOL			AddObject(DWORD i_dwObjTypeSize, DWORD i_dwObjPoolSize = 1024);
	static BOOL			AllDeleteObject();
	static BOOL			InitObjects();
	static void*		ObjectNew(DWORD i_Size);
	static BOOL			ObjectDelete(void *i_p,DWORD i_Size);
	static DWORD		GetObjectValidSize(DWORD i_Size); 

	static VMem_Client*	PoolSearchValid(DWORD i_Size);


	static SObjectMem					m_ObjInfo[DEFUALT_MEMEORYPOOL_OBJECTSIZE];
	static VMem_Client					m_Mem[DEFUALT_MEMEORYPOOL_OBJECTSIZE];
	static DWORD						m_ObjCount;
	static CRITICAL_SECTION				m_cs;

};

#endif // !defined(AFX_MEMPOOLCLIENT_H__50553B14_1B10_47A0_A3BB_2EEAD90A84B7__INCLUDED_)
