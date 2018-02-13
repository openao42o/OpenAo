// screenkeyboarddlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumlauncher.h"
#include "screenkeyboarddlg.h"
#include "AtumLauncherDlg.h"		

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *g_szKeyboardString[] = {
	"1234567890qwertyuiopasdfghjklzxcvbnm",
	"!@#$%^&*()QWERTYUIOPASDFGHJKLZXCVBNM",
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// CScreenKeyboardDlg dialog


CScreenKeyboardDlg::CScreenKeyboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenKeyboardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenKeyboardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bOnShift		= FALSE;
	m_bOnCapsLock	= FALSE;
}


void CScreenKeyboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenKeyboardDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	for(int i=0; i < EXE2_KEY_TYPE_1_ALL_COUNT; i++)
	{
		DDX_Control(pDX, IDC_BTN_KEY_0 + i, m_arrBtn[i]);
	}
	DDX_Control(pDX, IDC_BTN_BACKSPACE, m_bmpBtnBackSpace);
	DDX_Control(pDX, IDC_BTN_CAPS_LOCK, m_bmpBtnCapsLock);
	DDX_Control(pDX, IDC_BTN_ENTER, m_bmpBtnEnter);
	DDX_Control(pDX, IDC_BTN_SHIFT, m_bmpBtnShift);
	DDX_Control(pDX, IDC_BTN_SPACE, m_bmpBtnSpace);
}


BEGIN_MESSAGE_MAP(CScreenKeyboardDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenKeyboardDlg)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_KEY_0, OnKey0)
	ON_BN_CLICKED(IDC_BTN_KEY_1, OnKey1)
	ON_BN_CLICKED(IDC_BTN_KEY_2, OnKey2)
	ON_BN_CLICKED(IDC_BTN_KEY_3, OnKey3)
	ON_BN_CLICKED(IDC_BTN_KEY_4, OnKey4)
	ON_BN_CLICKED(IDC_BTN_KEY_5, OnKey5)
	ON_BN_CLICKED(IDC_BTN_KEY_6, OnKey6)
	ON_BN_CLICKED(IDC_BTN_KEY_7, OnKey7)
	ON_BN_CLICKED(IDC_BTN_KEY_8, OnKey8)
	ON_BN_CLICKED(IDC_BTN_KEY_9, OnKey9)
	ON_BN_CLICKED(IDC_BTN_KEY_10, OnKey10)
	ON_BN_CLICKED(IDC_BTN_KEY_11, OnKey11)
	ON_BN_CLICKED(IDC_BTN_KEY_12, OnKey12)
	ON_BN_CLICKED(IDC_BTN_KEY_13, OnKey13)
	ON_BN_CLICKED(IDC_BTN_KEY_14, OnKey14)
	ON_BN_CLICKED(IDC_BTN_KEY_15, OnKey15)
	ON_BN_CLICKED(IDC_BTN_KEY_16, OnKey16)
	ON_BN_CLICKED(IDC_BTN_KEY_17, OnKey17)
	ON_BN_CLICKED(IDC_BTN_KEY_18, OnKey18)
	ON_BN_CLICKED(IDC_BTN_KEY_19, OnKey19)
	ON_BN_CLICKED(IDC_BTN_KEY_20, OnKey20)
	ON_BN_CLICKED(IDC_BTN_KEY_21, OnKey21)
	ON_BN_CLICKED(IDC_BTN_KEY_22, OnKey22)
	ON_BN_CLICKED(IDC_BTN_KEY_23, OnKey23)
	ON_BN_CLICKED(IDC_BTN_KEY_24, OnKey24)
	ON_BN_CLICKED(IDC_BTN_KEY_25, OnKey25)
	ON_BN_CLICKED(IDC_BTN_KEY_26, OnKey26)
	ON_BN_CLICKED(IDC_BTN_KEY_27, OnKey27)
	ON_BN_CLICKED(IDC_BTN_KEY_28, OnKey28)
	ON_BN_CLICKED(IDC_BTN_KEY_29, OnKey29)
	ON_BN_CLICKED(IDC_BTN_KEY_30, OnKey30)
	ON_BN_CLICKED(IDC_BTN_KEY_31, OnKey31)
	ON_BN_CLICKED(IDC_BTN_KEY_32, OnKey32)
	ON_BN_CLICKED(IDC_BTN_KEY_33, OnKey33)
	ON_BN_CLICKED(IDC_BTN_KEY_34, OnKey34)
	ON_BN_CLICKED(IDC_BTN_KEY_35, OnKey35)

	ON_BN_CLICKED(IDC_BTN_BACKSPACE, OnKeyBackspace)
	ON_BN_CLICKED(IDC_BTN_CAPS_LOCK, OnKeyCapsLock)
	ON_BN_CLICKED(IDC_BTN_ENTER, OnKeyEnter)
	ON_BN_CLICKED(IDC_BTN_SHIFT, OnKeyShift)
	ON_BN_CLICKED(IDC_BTN_SPACE, OnKeySpace)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenKeyboardDlg message handlers

BOOL CScreenKeyboardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	this->LoadImageOfKeyButtons();
	this->MoveKeyButtons();

	m_bmpKeyboard.LoadBitmap(IDB_SCREEN_KEYBOARD);
	BITMAP bmp;
	m_bmpKeyboard.GetBitmap(&bmp );

	SetWindowPos( NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER );
	BitmapRgn(IDB_SCREEN_KEYBOARD_MASK, RGB( 255,255,255 ) );

	RandomMoveWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CScreenKeyboardDlg::BitmapRgn( UINT resource, COLORREF TansColor )
{
	HBITMAP			m_hBack;
	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_hBack = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(resource));
	::SetWindowRgn( m_hWnd, BitmapToRegion( m_hBack, TansColor, TansColor ),TRUE);

	return TRUE;
}

BOOL CScreenKeyboardDlg::BitmapRgn( LPCTSTR resource, COLORREF TansColor )
{
	HBITMAP			m_hBack;
	HINSTANCE hInstance = AfxGetInstanceHandle();

	HANDLE handle = ::LoadImage( hInstance, resource, IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADFROMFILE );
   	
	if ( !handle ) return FALSE;

	m_hBack = (HBITMAP)handle;
	::SetWindowRgn( m_hWnd, BitmapToRegion( m_hBack, TansColor, TansColor ),TRUE);

	return TRUE;
}

HRGN CScreenKeyboardDlg::BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor/* = 0*/, COLORREF cTolerance/* = 0x101010*/)
{
	HRGN hRgn = NULL;

	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
					sizeof(BITMAPINFOHEADER),	// biSize 
					bm.bmWidth,					// biWidth; 
					bm.bmHeight,				// biHeight; 
					1,							// biPlanes; 
					32,							// biBitCount 
					BI_RGB,						// biCompression; 
					0,							// biSizeImage; 
					0,							// biXPelsPerMeter; 
					0,							// biYPelsPerMeter; 
					0,							// biClrUsed; 
					0							// biClrImportant; 
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
					#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangles
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
					GlobalFree(hData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}	
	}

	return hRgn;
}

void CScreenKeyboardDlg::RandomMoveWindow(void)
{
	int nCXScreen = GetSystemMetrics(SM_CXSCREEN);
	int nCYScreen = GetSystemMetrics(SM_CYSCREEN);
	
	RECT rt;
	GetWindowRect(&rt);
	int nWidth	= rt.right - rt.left;
	int nHeight	= rt.bottom - rt.top;
	
	RECT rtParent;
	GetParent()->GetWindowRect(&rtParent);

	int nLeftMargin			= 210;
	int nRightMargin		= 0;
	int nDownMargin			= 40;
	if(nCXScreen > nWidth)
	{
		// 2007-09-18 by cmkwon, 화상키보드 수정 - X좌표는 부모윈도우 기준으로 좌우 랜덤
		int	nRandRange = max(1, (rtParent.right - rtParent.left - (nLeftMargin+nRightMargin)) - nWidth);
		rt.left		= rtParent.left + nLeftMargin + RANDI(0, nRandRange);
		rt.right	= rt.left + nWidth;
	}

	if(nCYScreen > nHeight)
	{
		// 2007-09-18 by cmkwon, 화상키보드 수정 - Y좌표는 고정
		rt.top		= max(0, rtParent.bottom - nHeight - nDownMargin);
		rt.bottom	= rt.top + nHeight;
	}

	MoveWindow(&rt);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CScreenKeyboardDlg::MoveKeyButtons(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-10 ~ 2007-09-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CScreenKeyboardDlg::MoveKeyButtons(void)
{
	int nRow	= 0;
	int nCol	= 0;
	for(int i=0; i < EXE2_KEY_TYPE_1_ALL_COUNT; i++)
	{
		int nPosX = 0;
		int nPosY = 0;

		switch(nRow)
		{
		case 0:
			{
				nPosX = 92;
				nPosY = 106;
			}
			break;
		case 1:
			{
				nPosX = 107;
				nPosY = 129;
			}
			break;
		case 2:
			{
				nPosX = 119;
				nPosY = 151;
			}
			break;
		default:
			{
				nPosX = 131;
				nPosY = 174;
			}
		}
		m_arrBtn[i].MoveWindow(nPosX + nCol * 27, nPosY, 26, 21);

		nCol++;
		switch(i)
		{
		case EXE2_KEY_TYPE_1_R1_COUNT-1:
		case EXE2_KEY_TYPE_1_R2_COUNT-1:
		case EXE2_KEY_TYPE_1_R3_COUNT-1:
			{
				nRow++;
				nCol = 0;
			}
			break;
		}
	}
	GetDlgItem(IDC_BTN_BACKSPACE)->MoveWindow(362, 106, 57, 21);
	GetDlgItem(IDC_BTN_CAPS_LOCK)->MoveWindow(66, 151, 51, 21);
	GetDlgItem(IDC_BTN_ENTER)->MoveWindow(362, 151, 57, 21);
	GetDlgItem(IDC_BTN_SHIFT)->MoveWindow(66, 174, 63, 21);
	GetDlgItem(IDC_BTN_SPACE)->MoveWindow(177, 196, 160, 21);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CScreenKeyboardDlg::LoadImageOfKeyButtons(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CScreenKeyboardDlg::LoadImageOfKeyButtons(void)
{
// 2007-09-18 by cmkwon, 화상키보드 수정 - 특수문자 처리 관련 수정, 아래와 같이 수정함
//	UINT uiStartRID = 0;
//	if(FALSE == IsCapticalLetter())
//	{
//		uiStartRID = IDB_KEY_1;
//	}
//	else
//	{
//		uiStartRID = IDB_KEY_1_ON_SHIFT;
//	}
//
//	for(int i=0; i < EXE2_KEY_TYPE_1_ALL_COUNT; i++)
//	{
//		m_arrBtn[i].LoadBitmaps(uiStartRID + i);
//		m_arrBtn[i].Invalidate();
//	}
	UINT uiNumStartRID = 0;
	if(FALSE == this->IsSpecialLetter())
	{
		uiNumStartRID = IDB_KEY_1;
	}
	else
	{
		uiNumStartRID = IDB_KEY_1_ON_SHIFT;
	}
	UINT uiEnglishLetterStartRID = 0;
	if(FALSE == this->IsCapticalLetter())
	{
		uiEnglishLetterStartRID = IDB_KEY_1;
	}
	else
	{
		uiEnglishLetterStartRID = IDB_KEY_1_ON_SHIFT;
	}
	for(int i=0; i < EXE2_KEY_TYPE_1_ALL_COUNT; i++)
	{
		if(EXE2_KEY_TYPE_1_R1_COUNT > i)
		{
			m_arrBtn[i].LoadBitmaps(uiNumStartRID + i);
			m_arrBtn[i].Invalidate();
		}
		else
		{
			m_arrBtn[i].LoadBitmaps(uiEnglishLetterStartRID + i);
			m_arrBtn[i].Invalidate();
		}
	}

	if(FALSE == m_bOnCapsLock)
	{
		m_bmpBtnCapsLock.LoadBitmaps(IDB_KEY_CAPSLOCK);
	}
	else
	{
		m_bmpBtnCapsLock.LoadBitmaps(IDB_KEY_CAPSLOCK_ON);
	}
	m_bmpBtnCapsLock.Invalidate();
	
	if(FALSE == m_bOnShift)
	{
		m_bmpBtnShift.LoadBitmaps(IDB_KEY_SHIFT);
	}
	else
	{
		m_bmpBtnShift.LoadBitmaps(IDB_KEY_SHIFT_ON);
	}
	m_bmpBtnShift.Invalidate();
	
	m_bmpBtnBackSpace.LoadBitmaps(IDB_KEY_BACKSPACE);
	m_bmpBtnEnter.LoadBitmaps(IDB_KEY_ENTER);
	m_bmpBtnSpace.LoadBitmaps(IDB_KEY_SPACE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CScreenKeyboardDlg::IsCapticalLetter(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CScreenKeyboardDlg::IsCapticalLetter(void)
{
	if( (m_bOnCapsLock && m_bOnShift)
		|| (FALSE == m_bOnCapsLock && FALSE == m_bOnShift) )
	{
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			char CScreenKeyboardDlg::GetKeyChar(int i_nKeyIndex)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
char CScreenKeyboardDlg::GetKeyChar(int i_nKeyIndex)
{
	char *strKeyChar = NULL;

	// 2007-09-18 by cmkwon, 화상키보드 수정 - CapsLock 상태일때에는 숫자키는 정상적으로 나오게 수정
	if(EXE2_KEY_TYPE_1_R1_COUNT > i_nKeyIndex)
	{
		if(FALSE == this->IsSpecialLetter())
		{
			strKeyChar = g_szKeyboardString[0];
		}
		else
		{
			strKeyChar = g_szKeyboardString[1];
		}
	}
	else
	{
		if(FALSE == IsCapticalLetter())
		{
			strKeyChar = g_szKeyboardString[0];
		}
		else
		{
			strKeyChar = g_szKeyboardString[1];
		}
	}

	if(i_nKeyIndex >= strlen(strKeyChar))
	{
		return NULL;
	}

	return strKeyChar[i_nKeyIndex];
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CScreenKeyboardDlg::SendKeyChar(char i_cKeyChar)
/// \brief		
/// \author		cmkwon
/// \date		2007-09-11 ~ 2007-09-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CScreenKeyboardDlg::SendKeyChar(char i_cKeyChar)
{
	if(NULL == i_cKeyChar){				return;}

	CAtumLauncherDlg *pLauncherDlg =  (CAtumLauncherDlg *)GetParent();
	pLauncherDlg->PushCharFromScreenKeyboard(i_cKeyChar);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CScreenKeyboardDlg::IsSpecialLetter(void)
/// \brief		// 2007-09-18 by cmkwon, 화상키보드 수정 - 특수문자 관련
/// \author		cmkwon
/// \date		2007-09-18 ~ 2007-09-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CScreenKeyboardDlg::IsSpecialLetter(void)
{	
	return m_bOnShift;
}


BOOL CScreenKeyboardDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rc;
	GetClientRect(&rc);

	CDC MemDC; 
	MemDC.CreateCompatibleDC(pDC);

	
	CBitmap* pbmpOld = MemDC.SelectObject( &m_bmpKeyboard ); 
	pDC->BitBlt(0, 0, rc.right, rc.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject( pbmpOld );
	ReleaseDC(pDC);
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CScreenKeyboardDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);

		// 마우스로 끌어서 윈도우 이동
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}


void CScreenKeyboardDlg::OnKey0()
{
	SendKeyChar(GetKeyChar(0));
}
void CScreenKeyboardDlg::OnKey1()
{
	SendKeyChar(GetKeyChar(1));
}
void CScreenKeyboardDlg::OnKey2()
{
	SendKeyChar(GetKeyChar(2));
}
void CScreenKeyboardDlg::OnKey3()
{
	SendKeyChar(GetKeyChar(3));
}
void CScreenKeyboardDlg::OnKey4()
{
	SendKeyChar(GetKeyChar(4));
}
void CScreenKeyboardDlg::OnKey5()
{
	SendKeyChar(GetKeyChar(5));
}
void CScreenKeyboardDlg::OnKey6()
{
	SendKeyChar(GetKeyChar(6));
}
void CScreenKeyboardDlg::OnKey7()
{
	SendKeyChar(GetKeyChar(7));
}
void CScreenKeyboardDlg::OnKey8()
{
	SendKeyChar(GetKeyChar(8));
}
void CScreenKeyboardDlg::OnKey9()
{
	SendKeyChar(GetKeyChar(9));
}
void CScreenKeyboardDlg::OnKey10()
{
	SendKeyChar(GetKeyChar(10));
}
void CScreenKeyboardDlg::OnKey11()
{
	SendKeyChar(GetKeyChar(11));
}
void CScreenKeyboardDlg::OnKey12()
{
	SendKeyChar(GetKeyChar(12));
}
void CScreenKeyboardDlg::OnKey13()
{
	SendKeyChar(GetKeyChar(13));
}
void CScreenKeyboardDlg::OnKey14()
{
	SendKeyChar(GetKeyChar(14));
}
void CScreenKeyboardDlg::OnKey15()
{
	SendKeyChar(GetKeyChar(15));
}
void CScreenKeyboardDlg::OnKey16()
{
	SendKeyChar(GetKeyChar(16));
}
void CScreenKeyboardDlg::OnKey17()
{
	SendKeyChar(GetKeyChar(17));
}
void CScreenKeyboardDlg::OnKey18()
{
	SendKeyChar(GetKeyChar(18));
}
void CScreenKeyboardDlg::OnKey19()
{
	SendKeyChar(GetKeyChar(19));
}
void CScreenKeyboardDlg::OnKey20()
{
	SendKeyChar(GetKeyChar(20));
}
void CScreenKeyboardDlg::OnKey21()
{
	SendKeyChar(GetKeyChar(21));
}
void CScreenKeyboardDlg::OnKey22()
{
	SendKeyChar(GetKeyChar(22));
}
void CScreenKeyboardDlg::OnKey23()
{
	SendKeyChar(GetKeyChar(23));
}
void CScreenKeyboardDlg::OnKey24()
{
	SendKeyChar(GetKeyChar(24));
}
void CScreenKeyboardDlg::OnKey25()
{
	SendKeyChar(GetKeyChar(25));
}
void CScreenKeyboardDlg::OnKey26()
{
	SendKeyChar(GetKeyChar(26));
}
void CScreenKeyboardDlg::OnKey27()
{
	SendKeyChar(GetKeyChar(27));
}
void CScreenKeyboardDlg::OnKey28()
{
	SendKeyChar(GetKeyChar(28));
}
void CScreenKeyboardDlg::OnKey29()
{
	SendKeyChar(GetKeyChar(29));
}
void CScreenKeyboardDlg::OnKey30()
{
	SendKeyChar(GetKeyChar(30));
}
void CScreenKeyboardDlg::OnKey31()
{
	SendKeyChar(GetKeyChar(31));
}
void CScreenKeyboardDlg::OnKey32()
{
	SendKeyChar(GetKeyChar(32));
}
void CScreenKeyboardDlg::OnKey33()
{
	SendKeyChar(GetKeyChar(33));
}
void CScreenKeyboardDlg::OnKey34()
{
	SendKeyChar(GetKeyChar(34));
}
void CScreenKeyboardDlg::OnKey35()
{
	SendKeyChar(GetKeyChar(35));
}

void CScreenKeyboardDlg::OnKeyBackspace()
{
	CAtumLauncherDlg *pLauncherDlg =  (CAtumLauncherDlg *)GetParent();
	pLauncherDlg->DeleteCharFromScreenKeyboard();
}
void CScreenKeyboardDlg::OnKeyCapsLock()
{
	if(FALSE == m_bOnCapsLock)
	{
		m_bOnCapsLock	= TRUE;
	}
	else
	{
		m_bOnCapsLock	= FALSE;
	}
	this->LoadImageOfKeyButtons();
}

void CScreenKeyboardDlg::OnKeyEnter()
{
	// 2007-09-18 by cmkwon, 화상키보드 수정 - 화상키보드 윈도우에서 Hide 시킬경우 처리를 위해
	CAtumLauncherDlg *pLauncherDlg =  (CAtumLauncherDlg *)GetParent();
	pLauncherDlg->HideScreenKeyboardByScreenKeyboardWindow();
	
	this->ShowWindow(SW_HIDE);
}

void CScreenKeyboardDlg::OnKeyShift()
{
	if(FALSE == m_bOnShift)
	{
		m_bOnShift	= TRUE;
	}
	else
	{
		m_bOnShift	= FALSE;
	}
	this->LoadImageOfKeyButtons();
}

void CScreenKeyboardDlg::OnKeySpace()
{
	SendKeyChar(' ');
}

void CScreenKeyboardDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CScreenKeyboardDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// 2007-09-18 by cmkwon, 화상키보드 수정 - 화상키보드 윈도우에서 Hide 시킬경우 처리를 위해
	CAtumLauncherDlg *pLauncherDlg =  (CAtumLauncherDlg *)GetParent();
	pLauncherDlg->HideScreenKeyboardByScreenKeyboardWindow();

	this->ShowWindow(SW_HIDE);
	return;
	
	CDialog::OnCancel();
}

void CScreenKeyboardDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 2007-09-18 by cmkwon, 화상키보드 수정 - 화상키보드 윈도우에서 Hide 시킬경우 처리를 위해
	CAtumLauncherDlg *pLauncherDlg =  (CAtumLauncherDlg *)GetParent();
	pLauncherDlg->HideScreenKeyboardByScreenKeyboardWindow();

	this->ShowWindow(SW_HIDE);
	return;
	
	CDialog::OnOK();
}

void CScreenKeyboardDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	RandomMoveWindow();
}
