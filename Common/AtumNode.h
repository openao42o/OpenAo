// AtumNode.h: interface for the CAtumNode class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include <windows.h>

/*

2015-10-23, PanKJ documentation

AtumNode seems to represent an hierarchy of objects.

AtumNode supports polymorphism and is meant to be used
as a superclass. It defines virtual methods for
initialization, restoration, invalidation and deletion,
as well as rendering and time tick.

Each object can have children, but it only stores
a pointer to the first child. If there are more than one
children, they are linked to each other as a two way list.
This way a child can iterate over its parent's children.

AtumNode has a method that adds children and one that removes
them to/from the linked list.

Apparently masang have managed to make polymorphism meaningless
here because they defined methods that require arguments 
in the derived classes.

For instance:
	CINFDefaultWnd::InitDeviceObjects()
	CINFDefaultWnd::InitDevicedObjects(char*)

This class would be troublesome if you had a CAtumNode* pointer
to an instance of it and you tried to initialize it.

*/

class CAtumNode  
{
public:
	explicit CAtumNode(CAtumNode* pParent);
	CAtumNode() : CAtumNode(nullptr) { };
	virtual ~CAtumNode();
	virtual HRESULT InitDeviceObjects(); // some derived classes define a different signature for InitDeviceObjects()
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render(); // some derived classes define a different signature for Render()
	virtual void Tick(); // some derived classes define a different signature for Tick()
	virtual void TickParallel();
	CAtumNode* AddChild(CAtumNode* pAtumNode);
	void DeleteChild(CAtumNode* pAtumNode);

	CAtumNode*		m_pPrev;		// 이전의 노드
	CAtumNode*		m_pNext;		// 이후의 노드
	CAtumNode*		m_pChild;		// 차일드 노드
	CAtumNode*		m_pParent;		// 부모의 노드
	DWORD			m_dwStateBlock;	// 렌더링상태블럭
	BOOL			m_bUsing;
	DWORD			m_dwPartType;	// enum PartType
};