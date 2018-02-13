// INFPilotFace.h: interface for the CINFPilotFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFPILOTFACE_H__3A000A96_46CA_4ECB_B074_41325DBAAC23__INCLUDED_)
#define AFX_INFPILOTFACE_H__3A000A96_46CA_4ECB_B074_41325DBAAC23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CGameData;
class CINFImage;
class CINFImageEx;											  // 2011. 10. 10 by jskim UI시스템 변경
class CINFPilotFace : public CAtumNode
{
public:
	CINFPilotFace();
	virtual ~CINFPilotFace();
	virtual HRESULT InitDeviceObjects(char* strTexture);
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();

	int GetPilotFaceNumber();
	CINFImageEx* FindPilotImage(int nType);
	CINFImageEx* GetStartImage();
	CINFImageEx* GetNextImage();

	HRESULT	SetResourceFile(char* szFileName) ;
public:
	CGameData * m_pGameData;
	map<int, CINFImageEx*> m_mapFace;
	BOOL		m_bRestored;

};

#endif // !defined(AFX_INFPILOTFACE_H__3A000A96_46CA_4ECB_B074_41325DBAAC23__INCLUDED_)
