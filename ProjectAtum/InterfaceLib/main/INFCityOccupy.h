// INFCityOccupy.h: interface for the INFCityOccupy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYOCCUPY_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFCITYOCCUPY_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define OCCUPY_MENULIST_COUNT			8

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;

class CINFCityOccupy : public CINFBase
{

public:
	CINFCityOccupy(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityOccupy();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	BUILDINGNPC		*		m_pBuildingInfo;

	//CINFImage		*		m_pImgInputBase;
	CD3DHanFont		*		m_pFontMenuList[OCCUPY_MENULIST_COUNT];
	CD3DHanFont		*		m_pFontPopup;

	char			*		m_strIndexName[OCCUPY_MENULIST_COUNT][256];	
	int						m_nSelectText;
public:
	void SortStoreOccupyByVecBuildingNPC(vector<BUILDINGNPC*> *VecBuildingNPC);
	void SetViewMenuList(int nType);

	vector<string>			m_vecIndexName;
	vector<string>			m_vecInfoOccupy;
	vector<int>				vecNotOccupy;
	vector<int>				vecOccupy;

	char					strOccupyGuild[128];
	int						m_nOccupyGuildMasterUID;
	int						m_nOccupyGuildUID;

	ATUM_DATE_TIME			m_pTimeDef;
};

#endif
