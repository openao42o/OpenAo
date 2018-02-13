// INFImageListBox.h: interface for the CINFImageListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGELISTBOX_H__601DF5EB_1481_48CF_8C99_1DAF423CF9BD__INCLUDED_)
#define AFX_INFIMAGELISTBOX_H__601DF5EB_1481_48CF_8C99_1DAF423CF9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"

class CD3DHanFont;

typedef struct 
{	
	int				nLine;
	float			fPosX;
	float			fPosY;
	DWORD			dwColor;
	char			szEditText[1024];
	CD3DHanFont*	pEditFont;	
	
} structEditBoxFont;


class CINFImageListBox  : public CINFBase
{
public:
	CINFImageListBox();
	virtual ~CINFImageListBox();

	HRESULT InitDeviceObjects(int nOnePageLineCnt, DWORD *i_pFlag, int *i_nFontSize);	
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();

	void SetStringCull(int i_nLine, char* pTxt, vector<string> *vecStorage, int i_nMaxLen);
	void SetPosition(int i_nLine, float fPosX, float fPosY, DWORD dwColor);		// 컨트롤 위치 지정 

	void ResetContent();

	void SetTitleText(vector<string>	i_vecMissionName);
	void SetEditText(vector<string>	i_vecMissionName);

	void UpdateScrollPos();

	void SetScrollPos(float fPosX, float fPosY, float fWidth, float fHeight, 
									float fScrollPosX, float fScrollPosY, float fScrollWidth, float fScrollHeight);
	
	BOOL OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam);
	BOOL OnMouseMove(POINT pt);
	BOOL OnLButtonDown(POINT pt);
	BOOL OnLButtonUp(POINT pt);

private:
	structEditBoxFont*	GetListFont(int i_nLine);

	void RenderTitle();
	void RenderNormalString();	
	
private:
	vector<structEditBoxFont*>	m_vecFontTxt;	// 미션 보상아이템 명칭
	// 타이틀
	vector<string>				m_vecTitleText;
	// 설명
	vector<string>				m_vecEditText;
	// 설명
	vector<string>				m_vecBriefText;

	// 스크롤 바 
	CINFArenaScrollBar*			m_pINFScrollBar;

};

#endif // !defined(AFX_INFIMAGELISTBOX_H__601DF5EB_1481_48CF_8C99_1DAF423CF9BD__INCLUDED_)
