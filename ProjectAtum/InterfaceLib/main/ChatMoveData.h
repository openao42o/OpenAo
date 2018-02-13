// ChatMoveData.h: interface for the CChatMoveData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHATMOVEDATA_H__530FFF65_8AEA_4364_8442_86B8379DDB27__INCLUDED_)
#define AFX_CHATMOVEDATA_H__530FFF65_8AEA_4364_8442_86B8379DDB27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CD3DHanFont;
class CChatMoveData : public CAtumNode  
{
public:
	CChatMoveData(CAtumNode* pParent, TCHAR *str,DWORD dwColor,int size,int width,
		int height, int nCullLen = 0, BOOL bBalon = FALSE, DWORD dwFlage = 0);
	virtual ~CChatMoveData();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	virtual void Render(int x, int y);
	virtual void Render(int x, int y, DWORD dwChatColor);

	void ResetHanFontLine(int nFontCnt);
	// 2006-05-18 by ispark
	TCHAR m_szString[SIZE_MAX_CHAT_MESSAGE + SIZE_MAX_CHARACTER_NAME+4];

public:
//	D3DXVECTOR2		m_vRenderPos;
	vector<CD3DHanFont*>		m_vecFontInput;
	
	FLOAT			m_fLifeTime;
	DWORD			m_dwChatColor;
	vector<string>	vecChatCull;
	BOOL			m_bCull;
	BOOL			m_bBalon;

	int				m_nSize;
	int				m_nWidth;
	int				m_nHeight;
	SIZE			m_nMaxStrSize;
//	BYTE			m_bRed;
//	BYTE			m_bGreen;
//	BYTE			m_bBlue;

	int				m_nSizeW;
	int				m_nSizeH;

	DWORD			m_dwFontFlags;

};

#endif // !defined(AFX_CHATMOVEDATA_H__530FFF65_8AEA_4364_8442_86B8379DDB27__INCLUDED_)
