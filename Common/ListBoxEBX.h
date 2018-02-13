
#if !defined(AFX_CLISTBOXEBX_)
#define AFX_CLISTBOXEBX_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//--------------------------------------------------------------------------
//	Copyright 2001 (C) Enver Buzoku, ebuzoku@hotmail.com
//
//	This class extends CListBox.
//	Idea was taken from the CColorListBox class, found at www.codeguru.com,
//	designed and updated by Patrice Godard and Paul M. Meidinger.
//
//	CListBoxEBX features.
//		Background colour.
//		Foreground colour.
//		Text horizontal alignment.
//		Text vertical alignment.
//		Indicating inactive items.
//		Hooking of a DWORD value per item as in original SetItemData.
//		Allows for setting of the item height
//
//	Desired features.
//		Font manipulation.
//		Use of images
//		Horisontal scrolling.
//
//	Points to note:
//	----------------
//	Default index is set to (-1) for all set functions this means that the
//	operation will apply to the currently selected item.
//
//	DEF_COL - definition is used to indicate that the item is using windows
//  colours.
//
//	Required style flags, that must be set, are: 'Owner Draw' and 'Has Strings'
//--------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CListBoxEBX window

#define DEF_COL 17000000

class CListBoxEBX : public CListBox
{
protected:
	struct LItem
	{
		COLORREF	cBg,	cFg;
		int			nHal;
		int			nIndex;
		CString		strText;
		int			nCrowdedness;
		bool		bSelect;
		int			nNetworkState;			// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 추가
		BOOL		bViewNetworkState;		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 추가
	};

	CListBox*	m_pSuper;
	int			m_iItemHeight;
	HBITMAP		m_hBitMap;
	HBITMAP		m_hBitBG;
	HBITMAP		m_hBitIcon;

	int		Ix(int ix);
	LItem*	Item(int ix);

	static bool AlphaBlendU(HDC dcDest, int x, int y, int cx, int cy,
		HDC dcSrc, int sx, int sy, int scx, int scy, int alpha);



	// 2008-12-17 by ckPark 러시아 런쳐
	COLORREF	m_dwColorKey;
	static bool ColorKeyBlt(HDC dcDest, int x, int y, int cx, int cy,
						  HDC dcSrc, int sx, int sy, int scx, int scy,
						  int r, int g, int b);
	// end 2008-12-17 by ckPark 러시아 런쳐




public:
	CListBoxEBX(WORD i_wCrowdednessImageList=0, WORD i_wListItemBGImageList=0, WORD i_wIconImageList=0);
	virtual ~CListBoxEBX();



	// Adds an item at specified index.
	// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 인자추가(int i_nNetworkState=0, BOOL i_bViewNetworkState=FALSE)
	int InsertItem(int nIndex, LPCTSTR lpszItem, int i_nCrowdedness, COLORREF bg, COLORREF fg, int hal, int i_nNetworkState=0, BOOL i_bViewNetworkState=FALSE);
	void GetNetworkStateText(CString *o_pcstr, COLORREF *i_pcolorFont, int i_nNetworkState);		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
	void UpdateNetworkState(int i_nNetworkState);		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -

	// Removes an item
	BOOL RemoveAt(int ix);

	// Removes all items
	void ResetContent();
	void DrawText(CDC *pDC, CString &str, CPoint pt, int TextSize, DWORD TextColor,DWORD TextBgColor);

	HRGN BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor/* = 0*/, COLORREF cTolerance/* = 0x101010*/,int nx,int ny);
	BOOL BitmapRgn( UINT resource, COLORREF TansColor ,int nx,int ny);



	// call this to set item height, it should be called before items are added.
	int SetItemHeight(int h)
	{
		int oldH = m_iItemHeight;
		m_iItemHeight = h;

		return oldH;
	}


	void GetTextString(int nIndex, CString& rString);
	int GetCrowdedness(int nIndex);
	BOOL GetServerCheck(int nIndex);

	int			m_nItemBGWidth;				// 2007-09-11 by cmkwon, 베트남 런처 수정시 추가함
	int			m_nItemBGHeight;			// 2007-09-11 by cmkwon, 베트남 런처 수정시 추가함
	void SetItemBGImageSize(int i_nItemBGWidth, int i_nItemBGHeight);

	int			m_nItemIconPosX;
	int			m_nItemIconPosY;
	int			m_nItemIconWidth;
	int			m_nItemIconHeight;
	void SetItemIconImage(int i_nIconPosX, int i_nIconPosY, int i_nIconWidth, int i_nIconHeight);



	// 2008-12-17 by ckPark 러시아 런쳐
	void SetColorKey(const COLORREF dwColorKey) { m_dwColorKey = dwColorKey; }
	// end 2008-12-17 by ckPark 러시아 런쳐





// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxEBX)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxEBX)
	afx_msg void OnDestroy();
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
