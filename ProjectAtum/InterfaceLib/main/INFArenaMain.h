// INFArenaMain.h: interface for the CINFArenaMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENAMAIN_H__AC6E994F_44F6_447E_BEBB_6F3C56F5B8E0__INCLUDED_)
#define AFX_INFARENAMAIN_H__AC6E994F_44F6_447E_BEBB_6F3C56F5B8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFGameArea.h"


class CINFArenaMain  
{
public:
	CINFArenaMain();
	virtual ~CINFArenaMain();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();

	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();

	void Render(DWORD dwGameState);

	int WndProc(DWORD dwGameState, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void TickArena(DWORD dwGameState);
	void ShowArenaInteraface(DWORD dwGameState, int nIdx, BOOL bShow);
	

private:
	CINFGameArea*			m_pINFGameArea;	

	int						m_nAreanaState;
protected:	

};

#endif // !defined(AFX_INFARENAMAIN_H__AC6E994F_44F6_447E_BEBB_6F3C56F5B8E0__INCLUDED_)
