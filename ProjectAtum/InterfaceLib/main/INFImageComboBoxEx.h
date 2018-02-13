// INFImageComboBoxEx.h: interface for the CINFImageComboBoxEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGECOMBOBOXEX_H__4300DD8A_5EE3_4A47_9338_4A9B52542B9B__INCLUDED_)
#define AFX_INFIMAGECOMBOBOXEX_H__4300DD8A_5EE3_4A47_9338_4A9B52542B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 아이템 리턴상황
#define		LBTN_COMBOEX_NONE			-1
#define		LBTN_COMBOEX_SHOWITEM		1		// 아이템 보이기
#define		LBTN_COMBOEX_HIDEITEM		2		// 아이템 숨기기
#define		LBTN_COMBOEX_CHANGEITEM		3		// 커서변경
#define		LBTN_COMBOEX_SELSCROLL		4		// 스크롤 변경

class CINFImage;
class CINFImageEx;
class CINFArenaScrollBar;
struct structComboInfo
{
	int				nIdx;			// 이미지
	CINFImage*		pInfImage;			// 이미지
	char			chComboTxt[256];	// 스트링정보
	BOOL			bShowItem;			// 아이템일떄 보일지 여부
};

class CINFImageComboBoxEx  : public CINFBase  
{
public:
	CINFImageComboBoxEx();
	virtual ~CINFImageComboBoxEx();

	virtual HRESULT InitDeviceObjects(POINT ptPos, int nWidth, int nHeight, 
												char* i_pBkImg, 
												char* i_pSelBkImg, 
												char* i_pSelImg, 
												char* i_pScroll);
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void			Render();
	
	void			AddItem(int nCnt, char *pComboTxt, CINFImage* pInfImage, BOOL bShowItem);
	void			UpdateItemInfo(int nIdx, BOOL bShowItem);

	BOOL			OnLButtonDown(POINT ptPos);
	BOOL			OnMouseMove(POINT ptPos);
	BOOL			OnLButtonUp(POINT ptPos);
	
	BOOL			IsShowWindow();
	void			ShowWindow(BOOL bShowWnd, POINT *pPos);
	
	BOOL			IsShowItem();
	void			ShowItem(BOOL bShowItem);

	int				GetCurSel();
	void			SetCurSel(int nCurSel);

	void			ResetItem();
	void			SetPos(POINT ptPos);

	BOOL			OnMouseWheel(POINT ptPos, WPARAM wParam, LPARAM lParam);

private:
	vector<structComboInfo*>		m_vecComboInfo;
	
	POINT			m_ptPos;
	int				m_nWidth;
	int				m_nHeight;

	BOOL			m_bShowItem;
	BOOL			m_bShowWnd;

	CD3DHanFont	*	m_pFontTxt;
	int				m_nCurSel;
	int				m_nCurSelItem;

	CINFImageBtn*		m_pImgComboBtn;
	CINFImageEx*		m_pImgSelect;
	CINFImageEx*		m_pImgSelectBk;

	CINFArenaScrollBar*			m_pScroll;	

};

#endif // !defined(AFX_INFIMAGECOMBOBOXEX_H__4300DD8A_5EE3_4A47_9338_4A9B52542B9B__INCLUDED_)
