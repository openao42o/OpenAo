
#include "stdafx.h"
#include "resource.h"
#include "ListBoxEBX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxEBX

//-------------------------------------------------------------------
//
// 2007-09-05 by cmkwon, EXE_1에 로그인 서버 선택 인터페이스 수정 - 인자추가 WORD i_wCrowdednessImageList=0, WORD i_wListItemBGImageList=0, WORD i_wIconImageList=0
CListBoxEBX::CListBoxEBX(WORD i_wCrowdednessImageList/*=0*/, WORD i_wListItemBGImageList/*=0*/, WORD i_wIconImageList/*=0*/)
{
	m_pSuper = (CListBox*)this;
	m_iItemHeight = 25;				// 2007-09-05 by cmkwon, 기본값

	m_nItemBGWidth		= 176;		// 2007-09-11 by cmkwon, 기본값, 
	m_nItemBGHeight		= 25;		// 2007-09-11 by cmkwon, 기본값, 

	m_nItemIconPosX		= 3;
	m_nItemIconPosY		= 8;
	m_nItemIconWidth	= 19;
	m_nItemIconHeight	= 10;


	m_hBitMap	= NULL;
	m_hBitBG	= NULL;
	m_hBitIcon	= NULL;

	if(0 != i_wCrowdednessImageList)
	{
		m_hBitMap	= LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(i_wCrowdednessImageList));
	}

	if(0 != i_wListItemBGImageList)
	{
		m_hBitBG	= LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(i_wListItemBGImageList));		
	}
	
	if(0 != i_wIconImageList)
	{
		m_hBitIcon	= LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(i_wIconImageList));
	}

	if(m_hBitMap)
	{
		BITMAP bit;
		GetObject(m_hBitMap, sizeof(BITMAP), &bit);
		m_iItemHeight = bit.bmHeight;
	}
}

CListBoxEBX::~CListBoxEBX(){}

BEGIN_MESSAGE_MAP(CListBoxEBX, CListBox)
	//{{AFX_MSG_MAP(CListBoxEBX)
	ON_WM_DESTROY()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxEBX message handlers

//-------------------------------------------------------------------
//

void CListBoxEBX::OnSysColorChange()
{
	CListBox::OnSysColorChange();
	this->RedrawWindow();
}

void CListBoxEBX::OnDestroy()
{
	//Clean up items and their strings
	ResetContent();

	CListBox::OnDestroy();
}

void CListBoxEBX::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	RECT rtBG;
	this->GetClientRect(&rtBG);
	int nBGWidth	= rtBG.right - rtBG.left;
	int nBGHeight	= rtBG.bottom - rtBG.top;

// 7/13/2006 by dgwoo list->imglist change
	if((int)lpDIS->itemID < 0)
		return;
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	CDC MemDC, RealMemDC;
	MemDC.CreateCompatibleDC(pDC);
	RealMemDC.CreateCompatibleDC(pDC);


	HBITMAP MemBit;
	HBITMAP OldMemBit;

	HBITMAP hOldbmp ;

	// 데이터 준비
	LItem	*pItem		= Item((int)lpDIS->itemID);		// pointer to data item

	COLORREF bg			= pItem->cBg;
	COLORREF fg			= pItem->cFg;
	int nDrawIndex		= pItem->nHal;
	int nNetState		= pItem->nNetworkState;			// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
	BOOL bViewNetState	= pItem->bViewNetworkState;		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -

	if(bg == DEF_COL) bg = ::GetSysColor(COLOR_WINDOW);
	if(fg == DEF_COL) fg = ::GetSysColor(COLOR_WINDOWTEXT);

	// 화면 지우기;
	CBrush brush(bg);

	
	int state = 0;
	if(lpDIS->itemState&ODS_SELECTED)
	{
		state=1;
	}

	// 이미지 넣기


	if(nDrawIndex != -1)
	{
		MemBit = CreateCompatibleBitmap(lpDIS->hDC, lpDIS->rcItem.right, m_iItemHeight);
		OldMemBit = (HBITMAP)RealMemDC.SelectObject(MemBit);
		hOldbmp = (HBITMAP)MemDC.SelectObject(m_hBitBG);
		
	
		RealMemDC.StretchBlt(0,0, nBGWidth, nBGHeight, &MemDC, state*m_nItemBGWidth, 0, nBGWidth, nBGHeight, SRCCOPY);
		MemDC.SelectObject(hOldbmp);
		if(m_hBitIcon)
		{
			MemDC.SelectObject(m_hBitIcon);
			RealMemDC.StretchBlt(m_nItemIconPosX, m_nItemIconPosY, m_nItemIconWidth, m_nItemIconHeight, &MemDC, state*m_nItemIconWidth, 0, m_nItemIconWidth, m_nItemIconHeight, SRCCOPY);
		}
	}
	else
	{
		MemBit = CreateCompatibleBitmap(lpDIS->hDC, lpDIS->rcItem.right, m_iItemHeight);
		OldMemBit = (HBITMAP)RealMemDC.SelectObject(MemBit);
		hOldbmp = (HBITMAP)MemDC.SelectObject(m_hBitBG);

		RealMemDC.StretchBlt(0,0, nBGWidth, nBGHeight, &MemDC, 2*m_nItemBGWidth, 0, nBGWidth, nBGHeight, SRCCOPY);



		MemDC.SelectObject(hOldbmp);
		if(m_hBitIcon)
		{
			MemDC.SelectObject(m_hBitIcon);
			RealMemDC.StretchBlt(m_nItemIconPosX, m_nItemIconPosY, m_nItemIconWidth, m_nItemIconHeight, &MemDC, 2*m_nItemIconWidth, 0, m_nItemIconWidth, m_nItemIconHeight, SRCCOPY);
		}
	}

	// 글자 찍기
	CString lpszItem;
	GetTextString(lpDIS->itemID, lpszItem);
	if(m_hBitIcon)
	{
		fg = RGB(254, 254, 254);
		DrawText(&RealMemDC, lpszItem, CPoint(27, 9), 10, fg,bg);
	}
	else
	{
		// 2007-10-04 by cmkwon, 2픽셀만큼 뛰운다
		DrawText(&RealMemDC, lpszItem, CPoint(2, 9), 10, fg,bg);
	}

	// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - NetworkState를 보여기가 아니면 혼잡도를 이미지로 표시
	if(FALSE == bViewNetState)
	{
		if(m_hBitMap)
		{
			// 혼잡도 이미지넣기
			if(nDrawIndex != -1)
			{	
				hOldbmp = (HBITMAP)MemDC.SelectObject(m_hBitMap);
				RealMemDC.StretchBlt(140, 8, 60, 14, 
									&MemDC,600 - (nDrawIndex+1)*60, 0, 60, 14, SRCCOPY);
			}else
			{
				hOldbmp = (HBITMAP)MemDC.SelectObject(m_hBitMap);
				RealMemDC.StretchBlt(140, 8, 60, 14, 
									&MemDC, 540, 0 , 60, 14, SRCCOPY);
			}
		}
	}
	else
	{
		CString strNetState;
		COLORREF FontColor;
		GetNetworkStateText(&strNetState, &FontColor, nNetState);
		DrawText(&RealMemDC, strNetState, CPoint(118, 4), 10, FontColor,bg);
	}
	

	// 최종 뿌리기
	// 2008-12-17 by ckPark 러시아 런쳐
	//pDC->BitBlt(lpDIS->rcItem.left, lpDIS->rcItem.top, lpDIS->rcItem.right, lpDIS->rcItem.bottom, &RealMemDC, 0, 0, SRCCOPY);
//#if defined(SERVICE_TYPE_VIETNAMESE_SERVER_1) || defined(SERVICE_TYPE_ENGLISH_SERVER_1)// 러시아 런처 수정
	SetColorKey(RGB(255, 255, 255));
	CBrush br;
	br.CreateStockObject(NULL_BRUSH);
	pDC->FillRect(&lpDIS->rcItem, &br);
	this->ColorKeyBlt(pDC->GetSafeHdc(),
		lpDIS->rcItem.left, lpDIS->rcItem.top,
		lpDIS->rcItem.right - lpDIS->rcItem.left,
		lpDIS->rcItem.bottom - lpDIS->rcItem.top,
		RealMemDC.GetSafeHdc(),
		0, 0,
		lpDIS->rcItem.right - lpDIS->rcItem.left,
		lpDIS->rcItem.bottom - lpDIS->rcItem.top,
		(BYTE)m_dwColorKey, (BYTE)(m_dwColorKey >> 8), (BYTE)(m_dwColorKey >> 16));
	br.DeleteObject();
//#else
//	pDC->BitBlt(lpDIS->rcItem.left, lpDIS->rcItem.top, lpDIS->rcItem.right, lpDIS->rcItem.bottom, &RealMemDC, 0, 0, SRCCOPY);
//#endif
	// end 2008-12-17 by ckPark 러시아 런쳐







	MemDC.SelectObject(hOldbmp);
	RealMemDC.SelectObject(OldMemBit);
	MemDC.DeleteDC();
	RealMemDC.DeleteDC();
	//	if ((int)lpDIS->itemID < 0)
//		return;
//
//	int		iPenSize	= 1;
//	CDC		*pDC		= CDC::FromHandle(lpDIS->hDC);
//
//	LItem	*pItem		= Item((int)lpDIS->itemID);		// pointer to data item
//
//
//	COLORREF bg			= pItem->cBg;
//	COLORREF fg			= pItem->cFg;
//
//	if(bg == DEF_COL) bg = ::GetSysColor(COLOR_WINDOW);
//	if(fg == DEF_COL) fg = ::GetSysColor(COLOR_WINDOWTEXT);
//
//
//	// Draw background
//	// -------------------------------------------------------------------
//	CBrush brush(bg);
//	pDC->FillRect(&lpDIS->rcItem, &brush);
//
//	// Draw text
//	//--------------------------------------------------------------------
//	UINT nFormat = DT_SINGLELINE | pItem->uVal | pItem->uHal;
//
//	int		 nOldBkMode		= pDC->SetBkMode(TRANSPARENT);
//	COLORREF cOldFg			= pDC->SetTextColor(fg);
//	CString	 sItemText;
//
//	CRect rc(lpDIS->rcItem);
//	rc.DeflateRect(iPenSize, iPenSize);
//
//	//GetText((int)lpDIS->itemID, sItemText);
//	sItemText = pItem->strText;
//	pDC->DrawText(sItemText, -1, &rc, nFormat);
//
//	pDC->SetTextColor(cOldFg);
//	pDC->SetBkMode(nOldBkMode);
//
//	// Draw disabled
//	//--------------------------------------------------------------------
//	if(pItem->bActive == FALSE)
//	{
//		CRect rc(lpDIS->rcItem);
//
//		rc.DeflateRect(iPenSize, iPenSize);
//
//		CDC memDC0;
//
//		memDC0.CreateCompatibleDC(pDC);
//
//		AlphaBlendU(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
//			memDC0, rc.left, rc.top, rc.Width(), rc.Height(), 50);
//
//		memDC0.DeleteDC();
//	}
//	
//	if(lpDIS->itemState & ODS_SELECTED)
//	{
//		CRect rc(lpDIS->rcItem);
//
//		rc.DeflateRect(iPenSize, iPenSize);
//
//		CDC memDC0;
//
//		memDC0.CreateCompatibleDC(pDC);
//
//		AlphaBlendU(*pDC, rc.left, rc.top, rc.Width(), rc.Height(),
//			memDC0, rc.left, rc.top, rc.Width(), rc.Height(), 100);
//
//		memDC0.DeleteDC();
//	}
//	
//	// if selected no 3D border
//	//--------------------------------------------------------------------
//	if((lpDIS->itemAction & ODA_FOCUS) && (lpDIS->itemState & ODS_FOCUS))
//	{
//		CRect rc(lpDIS->rcItem);
//		for(int i = 0; i < iPenSize; i++)
//		{
//			pDC->DrawFocusRect(rc);
//			rc.DeflateRect(1, 1);
//		}
//	}
}

void CListBoxEBX::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	if(m_iItemHeight == 0)
	{
		CPaintDC dc(this);
		CRect r;
		dc.DrawText("M", -1, &r, DT_CALCRECT);
		lpMIS->itemHeight = r.Height() + 6;
	}
	else
		lpMIS->itemHeight = m_iItemHeight;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CListBoxEBX::SetItemBGImageSize(int i_nItemBGWidth, int i_nItemBGHeight)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CListBoxEBX::SetItemBGImageSize(int i_nItemBGWidth, int i_nItemBGHeight)
{
	m_nItemBGWidth	= i_nItemBGWidth;
	m_nItemBGHeight	= i_nItemBGHeight;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CListBoxEBX::SetItemIconImage(int i_nIconPosX, int i_nIconPosY, int i_nIconWidth, int i_nIconHeight)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CListBoxEBX::SetItemIconImage(int i_nIconPosX, int i_nIconPosY, int i_nIconWidth, int i_nIconHeight)
{
	m_nItemIconPosX		= i_nIconPosX;
	m_nItemIconPosY		= i_nIconPosY;
	m_nItemIconWidth	= i_nIconWidth;
	m_nItemIconHeight	= i_nIconHeight;
}

// private
int CListBoxEBX::Ix(int ix)
{
	if(GetCount() == 0)					// not populated yet
		return -1;

	if(ix == -1)						// asking for the selected item
		return GetCurSel();
	else if(ix < 0 || ix >= GetCount())	// out of range
		return -1;
	else
		return ix;
}

// private
CListBoxEBX::LItem* CListBoxEBX::Item(int ix)
{
	int i =Ix(ix);

	if(i == -1) return NULL;

	return (LItem *)m_pSuper->GetItemData(i);
}

// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 인자추가(int i_nNetworkState/*=0*/, BOOL i_bViewNetworkState/*=FALSE*/)
int CListBoxEBX::InsertItem(int nIndex, LPCTSTR lpszItem, int i_nCrowdedness, COLORREF bg, COLORREF fg, int hal, int i_nNetworkState/*=0*/, BOOL i_bViewNetworkState/*=FALSE*/)
{
	//int iRet = InsertString(nIndex, lpszItem);
	int iRet = AddString(lpszItem);
	

	if(iRet == LB_ERR)
		return iRet;

	LItem *Cbi = new LItem;

	Cbi->nIndex = nIndex;
	Cbi->strText = lpszItem;
	Cbi->nCrowdedness = i_nCrowdedness;
	Cbi->cBg = bg;
	Cbi->cFg = fg;
	Cbi->nHal = hal;
	Cbi->bSelect = false;
	Cbi->nNetworkState		= i_nNetworkState;			// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 추가
	Cbi->bViewNetworkState	= i_bViewNetworkState;		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 - 추가

	if(SetItemData(nIndex, (DWORD)Cbi) == LB_ERR)
		return LB_ERR;

	return iRet;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CListBoxEBX::GetNetworkStateText(CString *o_pcstr, COLORREF *i_pcolorFont, int i_nNetworkState)
/// \brief		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
/// \author		cmkwon
/// \date		2007-06-22 ~ 2007-06-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CListBoxEBX::GetNetworkStateText(CString *o_pcstr, COLORREF *i_pcolorFont, int i_nNetworkState)
{
	o_pcstr->Format("<%3dms>", i_nNetworkState);

	if(100 >= i_nNetworkState)
	{
		*i_pcolorFont = RGB(167, 235, 90);
	}
	else if(300 >= i_nNetworkState)
	{
		*i_pcolorFont = RGB(253, 163, 75);
	}
	else
	{
		*i_pcolorFont = RGB(255, 22, 74);
	}	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CListBoxEBX::UpdateNetworkState(int i_nNetworkState)
/// \brief		// 2007-06-22 by cmkwon, 중국 네트워크 상태 보여주기 -
/// \author		cmkwon
/// \date		2007-06-22 ~ 2007-06-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CListBoxEBX::UpdateNetworkState(int i_nNetworkState)
{
	int nCnts = GetCount();
	if(0 >= nCnts)
	{
		return;
	}

	for(int i=0; i < nCnts; i++)
	{
		LItem * plItem = this->Item(i);
		if(plItem)
		{
			plItem->nNetworkState	= i_nNetworkState;
		}
	}
}

BOOL CListBoxEBX::RemoveAt(int ix)
{
	int i =Ix(ix);

	if(i == -1) return FALSE;

	LItem *plbi = (LItem*)m_pSuper->GetItemData(i);

	if(m_pSuper->DeleteString(i) == LB_ERR || plbi == NULL)
		return FALSE;

	delete plbi;

	return TRUE;
}

void CListBoxEBX::ResetContent()
{
	while(GetCount()) RemoveAt(0);
}


// private
bool CListBoxEBX::AlphaBlendU
	(
		HDC dcDest, int x, int y, int cx, int cy,
		HDC dcSrc, int sx, int sy, int scx, int scy, int alpha
	)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;

	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection (dcSrc, (BITMAPINFO *)&BMI,
		DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);

	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection (dcDest, (BITMAPINFO *)&BMI,
		DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);


	// Copy our source and destination bitmaps onto our DIBSections,
	// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection

	HDC dc = CreateCompatibleDC(NULL);

	HBITMAP dcOld = (HBITMAP) SelectObject(dc, hbmSrc);

	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;

	SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;

	SelectObject(dc, dcOld);
	DeleteDC(dc);

    for (int j = 0; j < cy; ++j)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];

		for (int i = 0; i < cx; ++i)
		{
			pbSrcRGB[0]=(pbDestRGB[0] * (255-alpha) + pbSrcRGB[0] * alpha)>>8;
			pbSrcRGB[1]=(pbDestRGB[1] * (255-alpha) + pbSrcRGB[1] * alpha)>>8;
			pbSrcRGB[2]=(pbDestRGB[2] * (255-alpha) + pbSrcRGB[2] * alpha)>>8;
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}

	dc = CreateCompatibleDC(NULL);

	dcOld = (HBITMAP) SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, SRCCOPY))
		return false;
	DeleteDC(dc);

	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);

	return true;
}



// 2008-12-17 by ckPark 러시아 런쳐
bool CListBoxEBX::ColorKeyBlt
(
 HDC dcDest, int x, int y, int cx, int cy,
 HDC dcSrc, int sx, int sy, int scx, int scy, int r, int g, int b
	)
{
	BITMAPINFOHEADER BMI;
	// Fill in the header info.
	BMI.biSize = sizeof(BITMAPINFOHEADER);
	BMI.biWidth = cx;
	BMI.biHeight = cy;
	BMI.biPlanes = 1;
	BMI.biBitCount = 32;
	BMI.biCompression = BI_RGB;   // No compression
	BMI.biSizeImage = 0;
	BMI.biXPelsPerMeter = 0;
	BMI.biYPelsPerMeter = 0;
	BMI.biClrUsed = 0;           // Always use the whole palette.
	BMI.biClrImportant = 0;
	
	BYTE * pSrcBits;
	HBITMAP hbmSrc;
	// Create DIB section in shared memory
	hbmSrc = CreateDIBSection (dcSrc, (BITMAPINFO *)&BMI,
		DIB_RGB_COLORS, (void **)&pSrcBits, 0, 0l);
	
	BYTE * pDestBits;
	HBITMAP hbmDest;
	// Create DIB section in shared memory
	hbmDest = CreateDIBSection (dcDest, (BITMAPINFO *)&BMI,
		DIB_RGB_COLORS, (void **)&pDestBits, 0, 0l);
	
	
	// Copy our source and destination bitmaps onto our DIBSections,
	// so we can get access to their bits using the BYTE *'s we passed into CreateDIBSection
	
	HDC dc = CreateCompatibleDC(NULL);
	
	HBITMAP dcOld = (HBITMAP) SelectObject(dc, hbmSrc);
	
	if (!StretchBlt(dc, 0, 0, cx, cy, dcSrc, sx, sy, scx, scy, SRCCOPY))
		return false;
	
	SelectObject(dc, hbmDest);
	if (!StretchBlt(dc, 0, 0, cx, cy, dcDest, x, y, cx, cy, SRCCOPY))
		return false;
	
	SelectObject(dc, dcOld);
	DeleteDC(dc);
	
    for (int j = 0; j < cy; ++j)
    {
		LPBYTE pbDestRGB = (LPBYTE)&((DWORD*)pDestBits)[j * cx];
		LPBYTE pbSrcRGB = (LPBYTE)&((DWORD*)pSrcBits)[j * cx];
		
		for (int i = 0; i < cx; ++i)
		{
			if(pbSrcRGB[0] == b && pbSrcRGB[1] == g && pbSrcRGB[2] == r)
			{
				pbSrcRGB[0]=pbDestRGB[0];
				pbSrcRGB[1]=pbDestRGB[1];
				pbSrcRGB[2]=pbDestRGB[2];
			}
			pbSrcRGB += 4;
			pbDestRGB += 4;
		}
	}
	
	dc = CreateCompatibleDC(NULL);
	
	dcOld = (HBITMAP) SelectObject(dc, hbmSrc);
	if (!BitBlt(dcDest, x, y, cx, cy, dc, 0, 0, SRCCOPY))
		return false;
	DeleteDC(dc);
	
	DeleteObject(hbmSrc);
	DeleteObject(hbmDest);
	
	return true;
}
// end 2008-12-17 by ckPark 러시아 런쳐




void CListBoxEBX::GetTextString(int nIndex, CString& rString)
{
	int i = Ix(nIndex);

	if(i == -1){			return;}

	LItem *pItem = (LItem *)m_pSuper->GetItemData(i);
	if(NULL == pItem)
	{
		return;
	}

	rString = pItem->strText;
}

int CListBoxEBX::GetCrowdedness(int nIndex)
{
	int i = Ix(nIndex);

	if(i == -1){			return 0;}

	LItem *pItem = (LItem *)m_pSuper->GetItemData(i);
	if(NULL == pItem)
	{
		return 0;
	}

	return pItem->nCrowdedness;
}
///////////////////////////////////////////////////////////////////////////////
/// \class		BOOL CListBoxEBX::GetServerCheck(int nIndex)
///
/// \brief		파일이 현재 죽어있는지 확인해주는 함수.
/// \author		dgwoo
/// \version	
/// \date		7/13/2006 ~ 7/13/2006
/// \warning	
///////////////////////////////////////////////////////////////////////////////
BOOL CListBoxEBX::GetServerCheck(int nIndex)
{
	LItem *pItem = Item(nIndex);
	if(pItem->nHal < 0)
		return false;
	return true;
	 
}

void CListBoxEBX::DrawText(CDC *pDC, CString &str, CPoint pt, int TextSize, DWORD TextColor,DWORD TextBgColor)
{
	HFONT hFont, hOldFont;
	
	INT nHeight    = -MulDiv( TextSize, (INT)GetDeviceCaps(*pDC, LOGPIXELSY), 72 );
// 2007-02-12 by cmkwon, 아래와 같이 수정함
//#ifdef _VTC_VIET_SERVER		// 2006-05-15 by cmkwon
//	hFont = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, VIETNAMESE_CHARSET, OUT_DEFAULT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , "Times New Roman");
//	hOldFont = (HFONT)pDC->SelectObject(hFont);
//	pDC->SetTextColor(TextColor);
//	pDC->SetBkMode(TRANSPARENT);
//	pDC->TextOut(pt.x, pt.y, str);
//	pDC->SelectObject(hOldFont);
//	DeleteObject(hFont);
//#else
//	hFont = CreateFont(nHeight, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , "Times New Roman");
//	hOldFont = (HFONT)pDC->SelectObject(hFont);
//	pDC->SetTextColor(TextColor);
//	pDC->SetBkMode(TRANSPARENT);
//	pDC->TextOut(pt.x, pt.y, str);
//	pDC->SelectObject(hOldFont);
//	DeleteObject(hFont);
//#endif

	hFont = CreateFont(nHeight, 0, 0, 0, SG_BOX_FONT_WEIGHT, 0, 0, 0, SG_BOX_FONT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , SG_BOX_FONT_FACENAME);
	hOldFont = (HFONT)pDC->SelectObject(hFont);
	pDC->SetTextColor(TextColor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(pt.x, pt.y, str);
	pDC->SelectObject(hOldFont);
	DeleteObject(hFont);

}

