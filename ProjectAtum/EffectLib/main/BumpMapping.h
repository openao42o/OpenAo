// BumpMapping.h: interface for the CBumpMapping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUMPMAPPING_H__CDB521E0_1471_4313_9D78_E945EE08ECF5__INCLUDED_)
#define AFX_BUMPMAPPING_H__CDB521E0_1471_4313_9D78_E945EE08ECF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBumpMapping  
{
public:
	CBumpMapping();
	virtual ~CBumpMapping();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT Render(LPDIRECT3DTEXTURE9 m_pTexture, int m_nTriangleNumber);
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

public:
	LPDIRECT3DTEXTURE9		m_pNormalMap;	// 법선맵
	LPDIRECT3DVERTEXDECLARATION9	m_pDecl;// 정점선언
	// 셰이더
	LPD3DXEFFECT		    m_pEffect;		// 이펙트
	D3DXHANDLE				m_hTechnique;	// 테크닉
	D3DXHANDLE				m_hmWVP;		// 로컬-투영변환행렬
	D3DXHANDLE				m_hvLightDir;	// 광원방향
	D3DXHANDLE				m_hvColor;		// 정점색
	D3DXHANDLE				m_hvEyePos;		// 시점의 위치
	D3DXHANDLE				m_htDecaleTex;	// 디컬텍스처
	D3DXHANDLE				m_htNormalMap;	// 법선맵


};

#endif // !defined(AFX_BUMPMAPPING_H__CDB521E0_1471_4313_9D78_E945EE08ECF5__INCLUDED_)
