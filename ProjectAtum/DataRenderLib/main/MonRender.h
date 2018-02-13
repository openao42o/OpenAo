// MonRender.h: interface for the CMonRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONRENDER_H__18BC4CB2_2966_474B_A710_D38224BE8728__INCLUDED_)
#define AFX_MONRENDER_H__18BC4CB2_2966_474B_A710_D38224BE8728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MeshRender.h"

class CUnitData;
class CSkinnedMesh;
class CMonRender : public CMeshRender  
{
public:
	CMonRender();
	virtual ~CMonRender();
	virtual void	RenderShadow(CAtumNode * pNode);
	virtual void	Render(CMonsterData * pMonData);
	virtual void	Tick(float fElapsedTime);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	// 충돌 검사용 함수 나중에 따로 관리할 것.
	COLLISION_RESULT CheckCollMesh(D3DXMATRIX *mat,float *size, float *ColledMonsterSize, ClientIndex_t *o_monsterIndex);
};

#endif // !defined(AFX_MONRENDER_H__18BC4CB2_2966_474B_A710_D38224BE8728__INCLUDED_)
