// QuadGround.h: interface for the CQuadGround class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUADGROUND_H__BDB11296_38A9_4D09_8CBD_6AC3162A611A__INCLUDED_)
#define AFX_QUADGROUND_H__BDB11296_38A9_4D09_8CBD_6AC3162A611A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuadTree.h"

// 16비트 인덱스 버퍼의 버텍스 인덱스를 저장 할 구조체
struct MYINDEX
{
	WORD _0,_1,_2;
};

///////////////////////////////////////////////////////////////////////////////
/// \class		QuadGround
///
/// \brief		쿼드 트리로 구성된 실제 데이타를 갖을 클래스
/// \version	1.0
/// \date		2004-03-13 ~ 2004-03-13
/// \warning	단계에 따라 데이타사용방법이 틀리다
///////////////////////////////////////////////////////////////////////////////
class CQuadGround : public CQuadTree  
{
public:
	CQuadGround( int nMax );
	// 부모 포인터, 코너 타입, X 좌표 시작 블럭위치, Z 좌표 시작 블럭위치, 최대 크기
	CQuadGround( CQuadTree * pParent,
		CornerType nType,
		short sX,
		short sY,
		int nMax );
	virtual ~CQuadGround();
	virtual HRESULT InitDeviceObjects();			// Init Device
	virtual HRESULT RestoreDeviceObjects();			// Restore Device
	virtual HRESULT InvalidateDeviceObjects();		// Invalidate Device
	virtual HRESULT DeleteDeviceObjects();			// Delete Device
	virtual void Tick();							// Frame Move
	virtual void Render();							// Rendering
	void SubDivide();								// 하위 노드를 분할한다
	void SetIB();									// 렌더링 되어질 면을 구성(IB)
	void GetMinMax(float fMax, float fMin);		// 높이 최대,최소

	LPDIRECT3DTEXTURE9			m_pTexture;			// 텍스쳐
	LPDIRECT3DVERTEXBUFFER9		m_pVBTest;			// 버텍스 버퍼	
	LPDIRECT3DVERTEXBUFFER9		m_pVBTest1;			// 버텍스 버퍼 타일 방식 테스트용
	LPDIRECT3DINDEXBUFFER9		m_pIBTest;			// 인덱스 버퍼
	
	int							m_nSize;			// 현재 노드의 블럭 사이즈
	int							m_nTriangleNumber;	// 렌더링 되어질 면수
	MYINDEX						*m_pIdx;			// 렌더링 되어질 인덱스버퍼의 위치를 셋팅하기 위한 포인터
	vector<int>					m_vecRenderTileList;// 렌더링 되어질 타일 맵 리스트
	float						m_fRadius;			// 이 조각의 반지름 (절두체 컬링을 위해서.)
	
	int							m_nMaxHeight;		// 최대 높이
};

#endif // !defined(AFX_QUADGROUND_H__BDB11296_38A9_4D09_8CBD_6AC3162A611A__INCLUDED_)
