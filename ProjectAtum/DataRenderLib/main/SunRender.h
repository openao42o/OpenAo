// SunRender.h: interface for the CSunRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUNRENDER_H__D40C58C5_C7EB_4B1B_9553_57493DEF0684__INCLUDED_)
#define AFX_SUNRENDER_H__D40C58C5_C7EB_4B1B_9553_57493DEF0684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CSunRender : public CAtumNode  
{
public:
	CSunRender();
	virtual ~CSunRender();
	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
//	virtual void Render(CAtumNode * pNode);
	void RenderRens(CAtumNode * pNode);

	LPDIRECT3DTEXTURE9			m_pTexture[5];
	LPDIRECT3DVERTEXBUFFER9		m_pVBLens;

};

#endif // !defined(AFX_SUNRENDER_H__D40C58C5_C7EB_4B1B_9553_57493DEF0684__INCLUDED_)
