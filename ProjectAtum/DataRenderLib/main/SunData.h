// SunData.h: interface for the CSunData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUNDATA_H__10B96E39_7BA3_4FF9_AEAC_7A146228A0B7__INCLUDED_)
#define AFX_SUNDATA_H__10B96E39_7BA3_4FF9_AEAC_7A146228A0B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CSunRender;
class CCharacterInfo;
class CSunData : public CAtumNode  
{
public:
	CSunData();
	virtual ~CSunData();
	virtual void Tick();
//	virtual void Render();
	void RenderRens();
	FLOAT CheckCollMesh(D3DXMATRIX mat, D3DXVECTOR3 vPos);

	D3DXVECTOR3			m_vPos;
//	D3DXVECTOR3			m_vCenterPos;
	CSunRender	*		m_pRenderer;
	INT					m_nObjScreenX;			// ½ºÅ©¸°»ó X ÁÂÇ¥
	INT					m_nObjScreenY;			// ½ºÅ©¸°»ó Y ÁÂÇ¥
	INT					m_nObjScreenW;			// ½ºÅ©¸°»ó W ÁÂÇ¥
	D3DXVECTOR3			m_vLensPos[10];
	FLOAT				m_fScale[10];
	BYTE				m_bTexture[10];
	FLOAT				m_fMaterialValue;

	D3DXMATRIX					m_mMatrix;
	BodyCond_t					m_bodyCondition;
	CCharacterInfo*				m_pCharacterInfo;
	void LoadCharacterEffect(char* strFileName);
};

#endif // !defined(AFX_SUNDATA_H__10B96E39_7BA3_4FF9_AEAC_7A146228A0B7__INCLUDED_)
