// INFComboBoxEX.h: interface for the CINFComboBoxEX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMBOBOXEX_H__9602BEBA_1E6C_4669_A340_274B401C23FD__INCLUDED_)
#define AFX_INFCOMBOBOXEX_H__9602BEBA_1E6C_4669_A340_274B401C23FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define R_OK		1
#define	R_ERR		0

typedef struct _Rect{
	int x;
	int y;
	int w;
	int h;
} RenderRect;


typedef struct _ComboInfo{
	int ItemCount;
	int SlotNum;
	int MaxLine;
	int SelectHeight;
	int SelectWidth;
} ComboInfo;

typedef struct _SCROLLINFO{
	int x;
	int y;
	int xOri;
	int yOri;
	int lenght;
	int barwidth;
	int barheight;
} ScrollBarInfo;

typedef	struct _ComboData{
	int nData;
	char DataName[125];

	BOOL operator < (const _ComboData &stDes)
	{
		if(strcmp(DataName, stDes.DataName) < 0)
		{
			return TRUE;
		}
		return FALSE;
	}
} ComboData;
class CINFComboBoxEX   : public CINFBase 
{
public:
	CINFComboBoxEX();
	CINFComboBoxEX(CAtumNode* pParent, CGameData* m_pGameData);
	virtual ~CINFComboBoxEX();
	void		InitSeting(CAtumNode* pParent, CGameData* m_pGameData);
	int			AddItem(void *pItem);
	int			DelItem(void *pItem);
	
	bool		SetScrollBarInfo(int x,int y, int lenght,int width,int height);
	bool		CreateImage(const char * Ground,const char * Select,const char * Scroll);
	int			ComboRender();
	void		SetComboInfo(int ItemCount,int SlotNum,int SelectWidth,int SelectHeight);
	void		SetComboPt(int x,int y);
	RenderRect	GetComboRect() { return m_rtCombo; 	}

	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);
	int				GetItemIndexByPos(int nPosY);
	void			SetScrollEndLine();
	void			InitScrollLine();
	int				GetScrollLine();
	
	HRESULT		InitDeviceObjects();
	HRESULT		InvalidateDeviceObject();
	HRESULT		RestoreDeviceObject();
	HRESULT		DeleteDeviceObject();

	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderRect		m_rtCombo;						//콤보 박스 전체 
	ComboInfo		m_ComboInfo;					//콤보 박스의 기본 정보.
	bool			m_bScrollLock;
	SIZE			m_sizeItem;						//item Size
	int				m_nScrollIndex;					//스크롤의 위치.
	
	int				m_nActiveItem;					//활성화된 아이템.

	int				m_nScrollMaxCount;				//item max count
	int				m_nScrollLine;
	int				m_nScrollTemp;
	int				m_nSelectIndex;					//선택된 아이템 .
	int				m_nScrollBtn;					//

	ScrollBarInfo	m_ScrollBarInfo;				//스크롤바의 정보..
	int				m_nScrollPosition;				//스크롤의 위치 
	
	CINFImageEx	*	m_pGroundImg;					//콤보 박스 배경.
	CINFImageEx	*	m_pSelectImg;					//선택한 그림.
	CINFImageEx	*	m_pScrollImg;					//스크롤 그림.

	CD3DHanFont *	m_pFontItemName;				//아이템 이름.

	char			m_strGround[16];
	char			m_strSelect[16];
	char			m_strScroll[16];

public:
	vector<ComboData>	m_vecData;					//콤보 박스에 들어가는 Data
	int				m_nSelectDataIndex;					// 선택한 콤보 데이타 인덱스
};

#endif // !defined(AFX_INFCOMBOBOXEX_H__9602BEBA_1E6C_4669_A340_274B401C23FD__INCLUDED_)
