// comboboxebx.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "comboboxebx.h"
#include "AtumLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

#define SERVERUSER_W		78
#define SERVERUSER_H		12
#define SERVERUSER_X		127
#define SERVERUSER_Y		3
#define SERVERUSER_COMBO_H	20

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEBX

CComboBoxEBX::CComboBoxEBX()
{
	m_pComboList = (CComboBox*)this;
	m_hBitMap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SERVERUSER));

	BITMAP bit;
	GetObject(m_hBitMap, sizeof(BITMAP), &bit);
	m_nWidth = bit.bmWidth;
	m_nHeight = bit.bmHeight;
}

CComboBoxEBX::~CComboBoxEBX()
{
}


BEGIN_MESSAGE_MAP(CComboBoxEBX, CComboBox)
	//{{AFX_MSG_MAP(CComboBoxEBX)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEBX message handlers

int CComboBoxEBX::InsertItem(int nIndex, LPCTSTR lpszItem, int i_nCrowdedness, COLORREF bg, COLORREF fg, int hal)
{
	int iRet = m_pComboList->InsertString(nIndex, lpszItem);

	if(iRet == LB_ERR)
		return iRet;

	ComboItem *Cbi = new ComboItem;

	Cbi->nIndex = nIndex;
	Cbi->strText = lpszItem;
	Cbi->nCrowdedness = i_nCrowdedness;
	Cbi->cBg = bg;
	Cbi->cFg = fg;
	Cbi->nHal = hal;

	if(m_pComboList->SetItemData(nIndex, (DWORD)Cbi) == CB_ERR)
		return CB_ERR;

	return iRet;
}

int CComboBoxEBX::GetCrowdedness(int nIndex)
{
	int i = Ix(nIndex);

	if(i == -1){return 0;}

	ComboItem *pItem = (ComboItem *)m_pComboList->GetItemData(i);
	if(NULL == pItem)
	{
		return 0;
	}

	return pItem->nCrowdedness;
}

int CComboBoxEBX::Ix(int ix)
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

void CComboBoxEBX::GetTextString(int nIndex, CString& rString)
{
	int i = Ix(nIndex);

	if(i == -1){			return;}

	ComboItem *pItem = (ComboItem *)m_pComboList->GetItemData(i);
	if(NULL == pItem)
	{
		return;
	}

	rString = pItem->strText;
}

void CComboBoxEBX::OnDestroy() 
{
	CComboBox::OnDestroy();
	ResetContent();
	
	// TODO: Add your message handler code here
	
}
BOOL CComboBoxEBX::RemoveAt(int ix)
{
	int i =Ix(ix);

	if(i == -1) return FALSE;

	ComboItem *plbi = (ComboItem*)m_pComboList->GetItemData(i);

	if(m_pComboList->DeleteString(i) == CB_ERR || plbi == NULL)
		return FALSE;

	delete plbi;

	return TRUE;
}

void CComboBoxEBX::ResetContent()
{
	while(GetCount()) RemoveAt(0);
}

CComboBoxEBX::ComboItem* CComboBoxEBX::Item(int ix)
{
	int i =Ix(ix);

	if(i == -1) return NULL;

	return (ComboItem *)m_pComboList->GetItemData(i);
}

void CComboBoxEBX::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	if((int)lpdis->itemID < 0)
		return;

	CDC *pDC = CDC::FromHandle(lpdis->hDC);
	CDC MemDC, RealMemDC;
	MemDC.CreateCompatibleDC(pDC);
	RealMemDC.CreateCompatibleDC(pDC);

	CRect rcCombo(0, 0, lpdis->rcItem.right, SERVERUSER_COMBO_H);

	HBITMAP MemBit = CreateCompatibleBitmap(lpdis->hDC, lpdis->rcItem.right, SERVERUSER_COMBO_H);
	HBITMAP OldMemBit = (HBITMAP)RealMemDC.SelectObject(MemBit);

	HBITMAP hOldbmp = (HBITMAP)MemDC.SelectObject(m_hBitMap);

	// 데이터 준비
	ComboItem	*pItem		= Item((int)lpdis->itemID);		// pointer to data item

	COLORREF bg			= pItem->cBg;
	COLORREF fg			= pItem->cFg;
	int nDrawIndex		= pItem->nHal;

	if(bg == DEF_COL) bg = ::GetSysColor(COLOR_WINDOW);
	if(fg == DEF_COL) fg = ::GetSysColor(COLOR_WINDOWTEXT);

	// 화면 지우기;
	CBrush brush(bg);
	RealMemDC.FillRect(&rcCombo, &brush);
	RealMemDC.FrameRect(&rcCombo, &brush);

	// 글자 찍기
	CString lpszItem;
	GetTextString(lpdis->itemID, lpszItem);
	DrawText(&RealMemDC, lpszItem, CPoint(0, 4), 9, fg);

	// 이미지 넣기
	if(nDrawIndex != -1)
	{
		RealMemDC.StretchBlt(SERVERUSER_X, SERVERUSER_Y, SERVERUSER_W, SERVERUSER_H, 
							&MemDC, 0, m_nHeight - ((nDrawIndex+1) * SERVERUSER_H), SERVERUSER_W, SERVERUSER_H, SRCCOPY);
	}

	// 최종 뿌리기
	pDC->BitBlt(lpdis->rcItem.left, lpdis->rcItem.top, lpdis->rcItem.right, lpdis->rcItem.bottom, &RealMemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(hOldbmp);
	RealMemDC.SelectObject(OldMemBit);
	MemDC.DeleteDC();
	RealMemDC.DeleteDC();
}

void CComboBoxEBX::DrawText(CDC *pDC, CString &str, CPoint pt, int TextSize, DWORD TextColor)
{
	HFONT hFont, hOldFont;
	
	INT nHeight    = -MulDiv( TextSize, (INT)GetDeviceCaps(*pDC, LOGPIXELSY), 72 );
// 2007-02-12 by cmkwon, 아래와 같이 수정함
//#ifdef _VTC_VIET_SERVER		// 2006-05-15 by cmkwon
//	hFont = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, VIETNAMESE_CHARSET, OUT_DEFAULT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , "Times New Roman");
//	hOldFont = (HFONT)pDC->SelectObject(hFont);
//	pDC->SetTextColor(TextColor);
//	pDC->TextOut(0, 1, str);
//	pDC->SelectObject(hOldFont);
//	DeleteObject(hFont);
//#else
//	hFont = CreateFont(nHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , "Times New Roman");
//	hOldFont = (HFONT)pDC->SelectObject(hFont);
//	pDC->SetTextColor(TextColor);
//	pDC->TextOut(0, 1, str);
//	pDC->SelectObject(hOldFont);
//	DeleteObject(hFont);
//#endif
	hFont = CreateFont(nHeight, 0, 0, 0, SG_BOX_FONT_WEIGHT, 0, 0, 0, SG_BOX_FONT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH , SG_BOX_FONT_FACENAME);
	hOldFont = (HFONT)pDC->SelectObject(hFont);
	pDC->SetTextColor(TextColor);
	pDC->TextOut(0, 1, str);
	pDC->SelectObject(hOldFont);
	DeleteObject(hFont);

}