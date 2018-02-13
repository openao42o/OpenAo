// RainRender.h: interface for the CRainRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAINRENDER_H__8A398C92_4A0F_42D5_8697_03BFBB7DB489__INCLUDED_)
#define AFX_RAINRENDER_H__8A398C92_4A0F_42D5_8697_03BFBB7DB489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

typedef struct _RAINVERTEX
{
    D3DXVECTOR3 pos; 
    DWORD       color;
} RAINVERTEX;


class CRainRender : public CAtumNode  
{
public:
	CRainRender();
	virtual ~CRainRender();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();

	LPDIRECT3DVERTEXBUFFER9		m_pVB;

};

#endif // !defined(AFX_RAINRENDER_H__8A398C92_4A0F_42D5_8697_03BFBB7DB489__INCLUDED_)
