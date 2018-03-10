// VMem.cpp: implementation of the VMem class.
//
//////////////////////////////////////////////////////////////////////

#include "VMemClient.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VMem_Client::VMem_Client()
{
	m_TotalCnt				= 0;
	m_ValidCnt				= 0;
	m_InvalidCnt			= 0;
	m_ObjectSize			= 0;
}

VMem_Client::~VMem_Client()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		메모리풀 생성.
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////		
BOOL VMem_Client::AllocMem(DWORD i_Size,DWORD i_Count)
{
	int i;
	void *p = NULL;
	m_ObjectSize = i_Size;
	for(i = 0; i < i_Count; i++)
	{
		p = (void*)new char[m_ObjectSize];
		memset(p,0x00,m_ObjectSize);
		m_vecPtr.push_back(p);
		m_vecPtrValid.push_back(p);
		m_ValidCnt ++;
		m_TotalCnt ++;
	}
	m_InvalidCnt = m_TotalCnt - m_ValidCnt;
	DBGOUT("VMem_Client::AllocMem Size(%d) --- (Tot(%d),Val(%d),Inval(%d)\n",i_Size,m_TotalCnt,m_ValidCnt,m_InvalidCnt);
	return TRUE;
}

BOOL VMem_Client::ClearMem()
{
	BOOL bBool = TRUE;
	vector<void*>::iterator it = m_vecPtr.begin();
	while(it != m_vecPtr.end())
	{
		void* pVec = (*it);
		delete []pVec;
		it = m_vecPtr.erase(it);
	}
	m_vecPtrValid.clear();
	return bBool;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		delete
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL VMem_Client::FreeMem(void* i_pMem,DWORD i_size)
{
	memset(i_pMem,0x00,i_size);
	m_vecPtrValid.push_back(i_pMem);
	m_ValidCnt++;
	m_InvalidCnt--;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		new
/// \author		dgwoo
/// \date		2008-03-11 ~ 2008-03-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void* VMem_Client::GetMemNew()
{
	if(m_ValidCnt < 10)
	{// 여유 용량이 부족하면 새로 만든다.
		AllocMem(m_ObjectSize,m_TotalCnt);
	}
	void *p = m_vecPtrValid.back();
	m_vecPtrValid.pop_back();
	m_ValidCnt--;
	m_InvalidCnt++;
	return p;
}

