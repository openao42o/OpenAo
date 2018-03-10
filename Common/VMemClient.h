// VMem.h: interface for the VMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMEMCLIENT_H__04B409F4_1E43_4A12_84E2_2FD41244A2FB__INCLUDED_)
#define AFX_VMEMCLIENT_H__04B409F4_1E43_4A12_84E2_2FD41244A2FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <vector>
#include <algorithm>

using namespace std;

class VMem_Client  
{
public:
	VMem_Client();
	virtual ~VMem_Client();
		
	BOOL	AllocMem(DWORD i_Size,DWORD i_Count);					// 개수만큼 메모리 풀 생성.
	BOOL	ClearMem();												// 모든 메모리를 Delete
	BOOL	FreeMem(void* i_pMem,DWORD i_size);						// 하나만 Free
	void*	GetMemNew();			
	DWORD	GetInvalidCnt()		{return m_InvalidCnt;}
	DWORD	GetValidCnt()		{return m_ValidCnt;}
	

	vector<void*>				m_vecPtrValid;						// Object가 비어있는 주소값.
	vector<void*>				m_vecPtr;							// 각 오브젝트의 주소값을 가지고 있다.
	
	
	DWORD						m_TotalCnt;
	DWORD						m_ValidCnt;							// 사용중인 개수
	DWORD						m_InvalidCnt;						// 미사용중인 갯수.
	

	DWORD						m_ObjectSize;						// 오브젝트 사이즈.
	

};

#endif // !defined(AFX_VMEMCLIENT_H__04B409F4_1E43_4A12_84E2_2FD41244A2FB__INCLUDED_)
