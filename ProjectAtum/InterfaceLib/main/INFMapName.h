// INFMapName.h: interface for the INFMapName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMAPNAME_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFMAPNAME_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class DataHeader;
class CGameData;
class CINFImageEx;

class CINFMapName: public CINFBase
{

public:
	CINFMapName();
	virtual ~CINFMapName();
	virtual HRESULT InitDeviceObjects(char* strTexture);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();

	CINFImageEx* FindMapNameImage(int nType);
	HRESULT	SetResourceFile(char* szFileName) ;
public:
	CGameData * m_pGameData;
	map<int, CINFImageEx*> m_mapMapName;
	BOOL		m_bRestored;

};

#endif
