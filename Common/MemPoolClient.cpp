// MemPool.cpp: implementation of the CMemPool class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "MemPoolClient.h"



SObjectMem					CMemPoolClient::m_ObjInfo[DEFUALT_MEMEORYPOOL_OBJECTSIZE];
VMem_Client					CMemPoolClient::m_Mem[DEFUALT_MEMEORYPOOL_OBJECTSIZE];
DWORD						CMemPoolClient::m_ObjCount;
CRITICAL_SECTION			CMemPoolClient::m_cs;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemPoolClient::CMemPoolClient()
{
	m_ObjCount		= 0;
//	memset(m_ObjInfo,0x00,sizeof(SObjectMem * DEFUALT_MEMEORYPOOL_OBJECTSIZE));
//	memset(m_Mem,0x00,sizeof(VMem * DEFUALT_MEMEORYPOOL_OBJECTSIZE));
}

CMemPoolClient::~CMemPoolClient()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		오브젝트를 등록한다.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMemPoolClient::AddObject(DWORD i_dwObjTypeSize, DWORD i_dwObjPoolSize/* = 1024*/)
{
	int i,nCount = m_ObjCount;

	for(i = 0; i < nCount; i++)
	{
		if(m_ObjInfo[i].dwObjTypeSize == i_dwObjTypeSize)
		{// 같은 크기의 오브젝트가 있을경우엔 같은 오브젝트를 사용한다.
			m_ObjInfo[i].dwObjPoolSize += i_dwObjPoolSize;
			return FALSE;
		}
		else 
		{
		}
	}
	//i -= 1;
	m_ObjInfo[i].dwObjTypeSize = i_dwObjTypeSize;
	m_ObjInfo[i].dwObjPoolSize = i_dwObjPoolSize;
	m_ObjCount++;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메모리 풀 생성.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMemPoolClient::InitObjects()
{
	int i;
	for(i = 0; i < m_ObjCount ; i++)
	{
		m_Mem[i].AllocMem(m_ObjInfo[i].dwObjTypeSize,m_ObjInfo[i].dwObjPoolSize);
	}
	InitializeCriticalSection(&m_cs);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		모든 동작이 끝난후 메모리 Delete.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMemPoolClient::AllDeleteObject()
{
	int i;
	for(i = 0 ; i < DEFUALT_MEMEORYPOOL_OBJECTSIZE ; i++)
	{
		if(m_ObjInfo[i].dwObjTypeSize)
		{
			memset(m_ObjInfo,0x00,sizeof(SObjectMem));
			m_Mem[i].ClearMem();
		}
	}
	DeleteCriticalSection( &m_cs );
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		입력받은 사이즈의 오브젝트에 사용가능한 오브젝트의 주소값을 반환.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void* CMemPoolClient::ObjectNew(DWORD i_Size)
{
	EnterCriticalSection(&m_cs);
	void * pResult;
	VMem_Client * p = PoolSearchValid(i_Size);
	if(p)
	{
		pResult = p->GetMemNew();
		LeaveCriticalSection(&m_cs);
		return pResult;
	}
	LeaveCriticalSection(&m_cs);
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		오브젝트에 유효하도록 변경 및 초기화.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMemPoolClient::ObjectDelete(void *i_p,DWORD i_Size)
{
	EnterCriticalSection(&m_cs);
	VMem_Client * p = PoolSearchValid(i_Size);
	if(p)
	{
		p->FreeMem(i_p,i_Size);
		LeaveCriticalSection(&m_cs);
		return TRUE;
	}
	LeaveCriticalSection(&m_cs);
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		오브젝트에 사용중인 갯수.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CMemPoolClient::GetObjectValidSize(DWORD i_Size)
{
	VMem_Client * p = PoolSearchValid(i_Size);
	if(p)
	{
		return p->GetInvalidCnt();
	}
	return 0;

}
VMem_Client* CMemPoolClient::PoolSearchValid(DWORD i_Size)
{
	int i;
	for(i = 0; i < m_ObjCount; i++)
	{
		if(m_ObjInfo[i].dwObjTypeSize == i_Size)
		{
			return &m_Mem[i];
		}
	}
	return NULL;
	
}
