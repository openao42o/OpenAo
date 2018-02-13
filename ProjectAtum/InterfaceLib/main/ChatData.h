// ChatData.h: interface for the CChatData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATDATA_H__C4FE273B_5E9B_4C3F_BC20_EA216E6BA6DA__INCLUDED_)
#define AFX_CHATDATA_H__C4FE273B_5E9B_4C3F_BC20_EA216E6BA6DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CD3DHanFont;
class CChatData : public CAtumNode  
{
public:
	CChatData( TCHAR *str,int width, int height,DWORD dwColor);
	virtual ~CChatData();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	virtual void Render();

	TCHAR m_szString[90];

public:
	D3DXVECTOR2		m_vRenderPos;
	CD3DHanFont *	m_pFontInput;
//	CAtumNode	*	m_pRenderer;
	FLOAT			m_fLifeTime;
	DWORD			m_dwChatColor;
//	D3DXVECTOR2 m_vPosition;
};

#endif // !defined(AFX_CHATDATA_H__C4FE273B_5E9B_4C3F_BC20_EA216E6BA6DA__INCLUDED_)
