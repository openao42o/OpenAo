// ChatRender.h: interface for the CChatRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATRENDER_H__124A8ECC_31F1_4C0C_92AB_232E2D16DF68__INCLUDED_)
#define AFX_CHATRENDER_H__124A8ECC_31F1_4C0C_92AB_232E2D16DF68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CChatData;
class CD3DHanFont;
class CChatRender : public CAtumNode  
{
public:
	CChatRender();
	virtual ~CChatRender();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render(CChatData *pChatData);
	virtual void Tick(D3DXVECTOR2 vStartPos);

protected:
	D3DXVECTOR2 m_vRenderPos;
	CD3DHanFont * m_pFontInput;

};

#endif // !defined(AFX_CHATRENDER_H__124A8ECC_31F1_4C0C_92AB_232E2D16DF68__INCLUDED_)
