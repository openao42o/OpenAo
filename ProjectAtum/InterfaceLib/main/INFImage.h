// INFImage.h: interface for the CINFImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGE_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFIMAGE_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

#define ExtractRed(x) ((x>>16)&255)
#define ExtractGreen(x) ((x>>12)&255)
#define ExtractBlue(x) (x&255)

class CINFImage : public CAtumNode  
{

public:
	CINFImage();
	virtual ~CINFImage();

	virtual HRESULT InitDeviceObjects(char* pData, int nSize);
	virtual HRESULT InitDeviceObjectsEx();										// 2005-09-29 by ispark
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT RestoreDeviceObjectsEx(int nImageSizeX, int nImageSizeY);	// 2005-09-29 by ispark
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();

	void Scaling(float x, float y);
	void SetColor(D3DCOLOR color) { m_dwColor = color; }
	void SetTransparency(DWORD dwAlpha);

	void Move( float x, float y );

	void Rotate( float x, float y, float angle );
	void SetDXSprite(LPD3DXSPRITE pSpr) { m_pd3dxSprite = pSpr; }
	LPD3DXSPRITE GetDXSprite() { return m_pd3dxSprite; }
	D3DXVECTOR2 GetTransVector() { return m_v2Trans; }
	void SetRect(long left,long top, long right, long bottom);
	void SetScale( float x, float y ) { m_v2Scaling.x = x; m_v2Scaling.y = y; }
	void Attach(LPDIRECT3DTEXTURE9	pTexture);
	LPDIRECT3DTEXTURE9 Detach();
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }
	POINT GetImgSize(){ return m_poImgSize; }
	D3DXVECTOR2 GetScale() { return m_v2Scaling; }
	void InitRect();

	// 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	POINT GetCurrentScale ( void );
	D3DXVECTOR2 GetCenterTransVector ( void );
	// End. 2010. 05. 12 by hsLee 인피니티 필드 2차 UI 추가 수정. (인게임 거점 방어 단계 표시.)
	// 2011. 10. 10 by jskim UI시스템 변경
	D3DXIMAGE_INFO &GetSrcInfo() { return m_srcInfo; }
	void SetType(int type) { m_nType = type; }
	int GetType() { return m_nType; }
	const PRECT GetPtRect() const { return m_pRect; }
	void SetRect( PRECT prcRect );
	// end 2011. 10. 10 by jskim UI시스템 변경

protected:
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPD3DXSPRITE		m_pd3dxSprite; 
	D3DXVECTOR2			m_v2Scaling, m_v2Rcenter, m_v2Trans;
	RECT			*	m_pRect;
	BOOL				m_bSpriteCrate;
	float				m_fAngle;
	char			*	m_pData;
	int					m_nDataSize;
	D3DCOLOR			m_dwColor;
	POINT				m_poImgSize;	
	// 2011. 10. 10 by jskim UI시스템 변경
	D3DXIMAGE_INFO		m_srcInfo;
	BOOL				m_nType;
	// end 2011. 10. 10 by jskim UI시스템 변경
};

#endif // !defined(AFX_INFIMAGE_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
