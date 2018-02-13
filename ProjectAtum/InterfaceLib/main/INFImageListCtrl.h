// INFImageListCtrl.h: interface for the CINFImageListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGELISTCTRL_H__27DA6043_6921_40CA_8061_B87F5321F593__INCLUDED_)
#define AFX_INFIMAGELISTCTRL_H__27DA6043_6921_40CA_8061_B87F5321F593__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"
#include "INFArenaScrollBar.h"

typedef struct 
{
	CINFImageEx*	pInfImage;	
	float			fPosX;
	float			fPosY;
	float			fWidth;
	float			fHeight;
	int				nMainIdx;
	int				nSubIdx;
	BOOL			bShow;
	BOOL			bExtend;		// 확장키 
	DWORD			dwColor;
	char			szItemTxt[256];
	int				nEpId;		// 에피소드 ID
	BOOL			bEpTitle;	// 에피소드 Title
} stImageListCtrlIcon;

/////////////////// 확장/축소 /////////////////// 
#define LIST_SHOW_PLUS		0
#define LIST_SHOW_MINUS		1
#define MAX_LIST_SHOW		2

class CINFImageEx;
class CINFImageListCtrl  : public CINFBase
{
public:
	CINFImageListCtrl();
	virtual ~CINFImageListCtrl();

	HRESULT InitDeviceObjects(int nScrollStep);	
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RestoreItemDeviceObjects();

	void InitDeviceEtc(char* pPlus, char* pMinus, char* pSmallIcon, char* pSelIcon);	// ETC 이미지 
	void InserItem(int nEpId, int nMainIdx, char* pItemName, BOOL bEpTitle);
	void InserSubItem(int nEpId, int nMainIdx, int nSubIdx, char *szItemTitle, DWORD dwColor);
	void SetListCtrlPos(float fPosX, float fPosY, 
										float fSubItemStartX, float fSubItemStartY, 
										float fScrollStartX, float fScrollStartY, 
										float fWidth,float fHeight);
	void SortListCtrlItem();
	
	BOOL OnLButtonDown(POINT pt, int* o_nMainIdx, int* o_nSubIdx);	
	BOOL OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam);
	BOOL OnLButtonUp(POINT pt);
	BOOL OnMouseMove(POINT pt);


	void DeleteaAllItem();
	void UpdateItemPos();

	void ResetContent();
	// 특정 메인 인덱스를 팝업시킨다. 
	void PopupItem(BOOL bPopup, int nMainIdx, BOOL bEpTitle, int nEpId);
	void SetSelPoint(int nMainIdx,int nSubIdx);

	void ShowTitleAllEpId(BOOL bShow, int nEpId);

private:
	int GetMaxShowItem();
	
	

	void RenderListItem();
	void UpdateScrollPos();

private:	
	vector<stImageListCtrlIcon*>			m_vecMainImage;


	CINFImageEx*			m_pListShowImage[MAX_LIST_SHOW];
	POINT					m_ptIconSize[MAX_LIST_SHOW];

	CINFImageEx*				m_pSubItemImage;
	POINT					m_ptSubItemSize;

	// 선택 상자
	CINFImageEx*			m_pSelItemImage;

	// 스크롤 바
	CINFArenaScrollBar*		m_pINFScrollBar;
	int						m_nScrollStep;

	CD3DHanFont *			m_pFontSubItem;				

	float			m_fPosX;
	float			m_fPosY;
	float			m_fWidth;
	float			m_fHeight;

	float			m_fSubItemStartX;
	float			m_fSubItemStartY;

	float			m_fScrollStartX;
	float			m_fScrollStartY;

	POINT				m_ptSelPoint;
	

};

#endif // !defined(AFX_INFIMAGELISTCTRL_H__27DA6043_6921_40CA_8061_B87F5321F593__INCLUDED_)
