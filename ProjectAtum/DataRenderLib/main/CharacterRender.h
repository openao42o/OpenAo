// CharacterRender.h: interface for the CCharacterRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTERRENDER_H__DBE12C7A_4FD6_4818_88A6_C96B6C9A130E__INCLUDED_)
#define AFX_CHARACTERRENDER_H__DBE12C7A_4FD6_4818_88A6_C96B6C9A130E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MeshRender.h"

class CUnitData;
class CSkinnedMesh;
class CCharacterRender : public CMeshRender   
{
public:
	CCharacterRender();
	virtual ~CCharacterRender();
	virtual void	RenderShadow();
	virtual void	RenderShadow(CAtumNode * pNode);
	virtual void	Render();
	virtual void Render(CUnitData * pNode);
	virtual void	Render(GUIUnitRenderInfo& renderinfo);
	virtual void	Tick(float fElapsedTime);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
};

#endif // !defined(AFX_CHARACTERRENDER_H__DBE12C7A_4FD6_4818_88A6_C96B6C9A130E__INCLUDED_)
