// INFAutoEditBox.h: interface for the CINFAutoEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFAUTOEDITBOX_H__7B7ECFAB_287A_4E78_B040_7ED91DF61790__INCLUDED_)
#define AFX_INFAUTOEDITBOX_H__7B7ECFAB_287A_4E78_B040_7ED91DF61790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFEditBox.h"

class CINFArenaScrollBar;

class CINFAutoEditBox  : public CINFEditBox
{
public:
	CINFAutoEditBox();
	virtual ~CINFAutoEditBox();

	virtual HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	virtual void Render();
	virtual void Render(int nScroll, int nOnePageLine);
	virtual void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus);	
	// 2013-06-17 by bhsohn 조합식 내용 2개씩 지워지는 현상 처리
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL, BOOL bAddString=FALSE, BOOL bUseBackSpace=TRUE);
	
	// 버튼 클릭 
	virtual BOOL OnLButtonDown(POINT ptPos);	
	virtual BOOL OnLButtonUp(POINT ptPos);
	
	virtual void Tick();

	BOOL OnMouseMove(POINT ptPos);
	BOOL OnMouseWheel(POINT ptPos, WPARAM wParam, LPARAM lParam);
	

	// by mspark, 인게임 조합
	INT	AddElement(char* c);
	void ItemAddDone();
	void ItemClear();
	void SetElementArea(int cx,int cy,int cw,int ch);
	char * GetItem(UINT num);
	INT	GetSelect();
	void SetSelect(INT nSelect);
	void SetItemShow(bool bItemShow);
	void SetBGPos(int nBGPosX, int nBGPosY, int nWidth ,int nHeight);
	// by mspark, 인게임 조합

	void RenderScroll(int nWndHeight);

	void	SetAutoEditBoxString(char* pEditString);
	char*	GetAutoEditBoxString();

protected:
	
	// by mspark, 인게임 조합
	int						m_nSelect;					// 선택한 아이템의 번호.	
	int						m_nRenderSelect;					// 선택한 아이템의 번호.	
	
	int						m_nItemSize;				// 아이템의 갯수.
	
	RECT					m_rtElementArea;			// 원소들의 영역.	
	CINFImageEx	*			m_pImgElementBG;			// 리스트창을 클릭할 경우의 원소들의 배경.
	CINFImageEx*			m_pImgHeader;			
	CINFImageEx*			m_pImgSelItem;			
	CINFImageEx*			m_pImgBottom;			

	CINFArenaScrollBar*		m_pScroll;

	CD3DHanFont *			m_pFontItem;
	
	// 콤보박스 아이템수 
	BOOL					m_bItemShow;				// 아이템이 보여지고 있는 상태인가?	
//	char					m_szListItem[MAX_SEARCH_ITEM][512];
	vector<string>			m_vecComboItem;
	
	// 공백버퍼
	char					m_szBlankBuff[512];
	
	// 배경 위치
	int						m_nBGPosX;
	int						m_nBGPosY;
	int						m_nBGPosWidth;
	int						m_nBGPosHeight;

	std::string				m_strListString;
	

};

#endif // !defined(AFX_INFAUTOEDITBOX_H__7B7ECFAB_287A_4E78_B040_7ED91DF61790__INCLUDED_)
