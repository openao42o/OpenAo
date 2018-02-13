// Water.h: WaterRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WATER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_WATER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class DataHeader;
class CGameData;
class CD3DHanFont;

class CWater: public CINFBase
{

public:
	CWater();
	virtual ~CWater();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Tick();

//protected:
    LPDIRECT3DTEXTURE9      m_pBackgroundTexture;
	D3DCAPS9				m_pd3dCaps;

    LPDIRECT3DTEXTURE9      m_psBumpMap;
    D3DXMATRIXA16           m_matBumpMat;

    LPDIRECT3DTEXTURE9 CreateBumpMap( DWORD, DWORD, D3DFORMAT );
    HRESULT ConfirmDevice( D3DCAPS9*, DWORD, D3DFORMAT, D3DFORMAT );
	HRESULT SetEMBMStates(LPDIRECT3DTEXTURE9*      Texture, int i, int j);

	FLOAT m_fTime;
	
	LPDIRECT3DSURFACE9 m_pFilterSurface;
	LPDIRECT3DTEXTURE9 m_pFilterTexture;

public:
};

#endif
