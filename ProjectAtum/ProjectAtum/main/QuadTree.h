// QuadTree.h: interface for the CQuadTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADTREE_H__825B456F_BA9B_42CB_B29F_066BD1728E80__INCLUDED_)
#define AFX_QUADTREE_H__825B456F_BA9B_42CB_B29F_066BD1728E80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUAD_CHILD_NUM		4

enum CornerType{ MONE = -1,CORNER_TL,CORNER_TR,CORNER_BL,CORNER_BR};

///////////////////////////////////////////////////////////////////////////////
/// \class		QuadTree
///
/// \brief		쿼드 트리를 작성하기 위한 노드 클래스
/// \version	1.0
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CQuadTree  
{
public:
	CQuadTree();
	// 부모 포인터, 코너 타입, X 좌표 시작 블럭위치, Z 좌표 시작 블럭위치, 최대 크기
	CQuadTree(CQuadTree * pParent
		, CornerType nType
		, short sX
		, short sY
		, int nMax);
	virtual ~CQuadTree();
	virtual HRESULT InitDeviceObjects();		// Init	Device
	virtual HRESULT RestoreDeviceObjects();		// Restore Device
	virtual HRESULT InvalidateDeviceObjects();	// Invalidate Device
	virtual HRESULT DeleteDeviceObjects();		// Delete Device
	virtual void Render();						// Rendering
	virtual void Tick();						// FrameMove
	void AddChild(CQuadTree * pNode);			// 자식 노드 추가

public:
	CQuadTree		*m_pOriParent;				// 데이타를 들고 있는 부모(VB,IB)
	CQuadTree		*m_pChild[QUAD_CHILD_NUM];	// 자식 포인터	0~3 -> CornerType
	CQuadTree		*m_pParent;					// 부모 포인터
	D3DXVECTOR3		m_vPos[4];					// 귀퉁이 4개의 포지션
	CornerType		m_eCornerType;				// 코너 타입
	int				m_nDegree;					// 노드 실행 단계
	int				m_nMaxSize;					// 최대 크기
	short			m_sStartX;					// 시작 위치 x
	short			m_sStartY;					// 시작 위치 y
};

#endif // !defined(AFX_QUADTREE_H__825B456F_BA9B_42CB_B29F_066BD1728E80__INCLUDED_)
