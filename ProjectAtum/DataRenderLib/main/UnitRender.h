// UnitRender.h: interface for the CUnitRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNITRENDER_H__7F473CDE_2B31_40FE_92E9_8CA99C07FFBD__INCLUDED_)
#define AFX_UNITRENDER_H__7F473CDE_2B31_40FE_92E9_8CA99C07FFBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MeshRender.h"

class CUnitData;
class CSkinnedMesh;
class CUnitRender : public CMeshRender  
{
public:
	CUnitRender();
	virtual ~CUnitRender();
	virtual void	RenderShadow(CAtumNode * pNode);
	virtual void	Render(CUnitData * pNode, int nAmorColor);	// 2006-02-17 by ispark

	// 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	//virtual void	Render(GUIUnitRenderInfo& renderinfo);
	// 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
	//virtual void	Render( GUIUnitRenderInfo& renderinfo, const BOOL bUseSubMtrl = FALSE );
	virtual void	Render( GUIUnitRenderInfo& renderinfo, float fTick = 40.0f ,const BOOL bUseSubMtrl = FALSE );
	//end 2010. 06. 09 by jskim 기어 선택창 애니메이션 버그 수정
	// end 2009. 07. 07 by ckPark 로봇기어 요청사항(롤링, 선택화면, 무기, A기어포대)
	
	CSkinnedMesh*   GetUnitMesh(int nUnitNum);

	virtual void	Tick(float fElapsedTime);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	void RenderDecalObj(CAtumNode * pNode);
	int m_nSelectStateUnitColor;
};

#endif // !defined(AFX_UNITRENDER_H__7F473CDE_2B31_40FE_92E9_8CA99C07FFBD__INCLUDED_)
