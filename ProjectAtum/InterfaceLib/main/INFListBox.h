// INFListBox.h: interface for the CINFListBox class.
// // 2007-06-08 by dgwoo ListBox Class
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLISTBOX_H__3BE779C7_7FA5_4840_843D_05440F356410__INCLUDED_)
#define AFX_INFLISTBOX_H__3BE779C7_7FA5_4840_843D_05440F356410__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//struct STRUCT_LIST_ITEM
//{
//	char		szListItem[512];
//};

#define		MAX_COMBO_ITEM			25

class CINFImageEx;
class CINFListBox : public CINFBase  
{
public:
	CINFListBox(char * cImgMainName, char * cImgElementBGName);
	virtual ~CINFListBox();
	virtual HRESULT InitDeviceObjects();
	HRESULT			InitDeviceObjects_Select();
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void			Render();
	void			SetMainArea(int cx,int cy, int cw,int ch);				// 리스트 창의 영역.
	void			SetElementArea(int cx,int cy,int cw,int ch);			// Item 한개의 영역
	INT				AddElement(char * c);
	char *			GetItem(UINT num);
	INT				GetSelect();											// 0부터 시작.
	void			ItemClear();
	INT				UpdateItem();
	INT				SetSelectItem(UINT nSelect);							// 선택한 아이템을 지정한다.
	VOID			SetMainButtonDisable();
	VOID			SetMainButtonEnisable();

	INT				LButtonDown(POINT pt);
	INT				LButtonUp(POINT pt);
	INT				MouseMove(POINT pt);
	int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL			IsShowItem();
	void			ShowItem(BOOL bItemShow);
	void			SetBGPos(int nBGPosX, int nBGPosY, int nWidth ,int nHeight);

	UINT					m_nSelect;					// 선택한 아이템의 번호.	
	
	// 2008-11-13 by bhsohn 조이스틱 작업
	void			SetUseCulling(BOOL i_bUseCulling);	// 컬링 사용유무

private:
	//vector<STRUCT_LIST_ITEM>			m_vecElement;				// 아이템.
	int						m_nItemSize;					// 아이템의 갯수.

	// 2008-11-13 by bhsohn 조이스틱 작업
	BOOL			m_bUseCulling;	// 컬링 사용유무

	
	BOOL					m_bItemShow;				// 아이템이 보여지고 있는 상태인가?
	UINT					m_nMainStateB;				// 리스트 버튼의 상태.

	RECT					m_rtMainArea;				// 리스트창의 영역.
	RECT					m_rtElementArea;			// 원소들의 영역.	
	CINFImageEx	*			m_pImgMain[BUTTON_STATE_NUMBER];	// 리스트박스의 창 이미지.
	CINFImageEx	*			m_pImgElementBG;			// 리스트창을 클릭할 경우의 원소들의 배경.
	CD3DHanFont *			m_pFontItem;

	// 콤보박스 아이템수 
	char					m_szListItem[MAX_COMBO_ITEM][512];

	char					m_cMainName[512];
	char					m_cElementBGName[512];
	char					m_cViewData[512];

	// 공백버퍼
	char					m_szBlankBuff[512];

	// 배경 위치
	int						m_nBGPosX;
	int						m_nBGPosY;
	int						m_nBGPosWidth;
	int						m_nBGPosHeight;
	

};

#endif // !defined(AFX_INFLISTBOX_H__3BE779C7_7FA5_4840_843D_05440F356410__INCLUDED_)
