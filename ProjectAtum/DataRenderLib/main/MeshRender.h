// MeshRender.h: interface for the CMeshRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHRENDER_H__28791450_5BD7_4A28_9A2B_736B3B0E6ED0__INCLUDED_)
#define AFX_MESHRENDER_H__28791450_5BD7_4A28_9A2B_736B3B0E6ED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"
#include "SkinnedMesh.h"

class CMeshRender : public CAtumNode  
{
public:
	CMeshRender();
	virtual ~CMeshRender();
	// 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	//virtual CSkinnedMesh*	InitData(int nMeshIndex);
	virtual CSkinnedMesh*	InitData(int nMeshIndex, int nType, D3DXVECTOR3 ObjScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));// 2012-05-21 by isshin 맵툴 개선 오브젝트 스케일 적용
	//end 2009. 11. 23 by jskim 리소스 로딩 구조 변경
	virtual void			DeleteData(int nMeshIndex);
	virtual void			RenderShadow(CAtumNode * pNode);
	virtual void			Render();
	virtual void			Tick(float fElapsedTime);
	virtual HRESULT			InitDeviceObjects();
	virtual HRESULT			RestoreDeviceObjects();
	virtual HRESULT			InvalidateDeviceObjects();
	virtual HRESULT			DeleteDeviceObjects();


	map<int,CSkinnedMesh*>		m_mapSkinnedMesh;		// index  : 12030100 ( 12 : degree, 03 : amor, 01 : unitKind )

protected:
	vector<int>					m_vecLoadMeshIndex;		// render중에 로딩이 되지 않은 경우 여기에 넣고 Tick에서 로딩한다.
//	DWORD						m_dwNormalStateBlock;
//	DWORD						m_dwAlphaBlendStateBlock;

};


#endif // !defined(AFX_MESHRENDER_H__28791450_5BD7_4A28_9A2B_736B3B0E6ED0__INCLUDED_)
