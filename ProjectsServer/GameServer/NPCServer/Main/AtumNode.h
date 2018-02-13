// AtumNode.h: interface for the CAtumNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATUMNODE_H__C4F6F8E5_CA48_46AF_8E84_DD50489CBCF1__INCLUDED_)
#define AFX_ATUMNODE_H__C4F6F8E5_CA48_46AF_8E84_DD50489CBCF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "D3Dfile.h"

class CAtumNode  
{
public:
	CAtumNode();
	CAtumNode(CAtumNode* pParent);
	virtual ~CAtumNode();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();
	virtual void Tick();
	CAtumNode* AddChild( CAtumNode* pAtumNode );
	void DeleteChild( CAtumNode* pAtumNode );

public:
	CAtumNode*		m_pPrev;		// 이전의 노드
	CAtumNode*		m_pNext;		// 이후의 노드
	CAtumNode*		m_pChild;		// 차일드 노드
	CAtumNode*		m_pParent;		// 부모의 노드
	DWORD			m_dwStateBlock;	// 렌더링상태블럭
	BOOL			m_bUsing;
	DWORD			m_dwPartType;	
};

#endif // !defined(AFX_ATUMNODE_H__C4F6F8E5_CA48_46AF_8E84_DD50489CBCF1__INCLUDED_)
