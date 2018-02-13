// CINFMissionMasterReg.h: interface for the CINFMissionMasterReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINFMissionMasterReg_H__04E9D5C6_A906_4B8A_8D07_290B19CE6570__INCLUDED_)
#define AFX_CINFMissionMasterReg_H__04E9D5C6_A906_4B8A_8D07_290B19CE6570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"

class CINFGroupImage;
class CINFMissionMasterReg : public CINFBase  
{
public:
	CINFMissionMasterReg();
	virtual ~CINFMissionMasterReg();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void		SetWindowPos(POINT i_ptPos);
	void		SetWindowShow(BOOL i_bShow);

	BOOL					m_bShow;
	POINT					m_ptWindow;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI시스템 변경
	CINFGroupImage*			m_pImgBG;
#else
	CINFImageEx*			m_pImgBG;
#endif

	CINFImageBtn*			m_pOK;
	CINFImageBtn*			m_pCancel;

	

};

#endif // !defined(AFX_CINFMissionMasterReg_H__04E9D5C6_A906_4B8A_8D07_290B19CE6570__INCLUDED_)
