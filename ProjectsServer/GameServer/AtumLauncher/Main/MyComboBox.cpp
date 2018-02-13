// MyComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "atumlauncher.h"
#include "MyComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::CMyComboBox
/// \brief	: 생성자 기본 색깔 셋팅한다
/// \author	: CKPark
/// \date	: 5:12:2008   11:50
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyComboBox::CMyComboBox() : CComboBox()
{
	m_TextColor = RGB(254, 254, 254);
	m_BackColor = RGB(0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::~CMyComboBox
/// \brief	: 파괴자 사용한 비트맵 해제
/// \author	: CKPark
/// \date	: 5:12:2008   11:51
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyComboBox::~CMyComboBox()
{
	if(m_EditBitMap.m_hObject)
		m_EditBitMap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMyComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyComboBox message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::OnSelchange
/// \brief	: 선택이 바뀌었을 경우 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:51
/// \warning: 
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	Invalidate(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::OnPaint
/// \brief	: 영역 무효화시 페인팅
/// \author	: CKPark
/// \date	: 5:12:2008   11:52
/// \warning: CComboBox::OnPaint()를 호출안하는 대신 CPaintDC dc(this)를 생성
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::OnPaint()
{	
	CPaintDC	dc(this);
	
	CDC*	pDC		= GetDC();
	CDC		memDC;
	
	if(!pDC)
		return;

	// 비트맵 로딩이 안되있으면 아무것도 안그린다
	if(!m_EditBitMap.m_hObject)
		return;

	// 메모리 dc 생성
	memDC.CreateCompatibleDC(NULL);

	// 클라이언트 영역
	CRect	r;
	GetClientRect(&r);

	// 메모리 dc에 비트맵 설정
	CBitmap*	pMemDCOld;
	pMemDCOld = memDC.SelectObject(&m_EditBitMap);

	// dc에 메모리 dc내용 ㅂ고사
	pDC->BitBlt(0, 0, r.Width(), r.Height(), &memDC, 0, 0, SRCCOPY);

	// 메모리 dc 해제
	memDC.SelectObject(pMemDCOld);
	memDC.DeleteDC();

	// 현재 선택된 아이템을 그려줘야 하므로
	// 선택된 인텍스 얻어옴
	int		nIndex = GetCurSel();
	// 아무것도 선택이 안된 상태가 아닐 경우
	if( nIndex != -1)
	{
		// 색깔 셋팅
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_TextColor);

		// 글자를 그려준다(r.left + 3, r.top + 4 는 적당히 위치를 맞춘 것이다)
		// textRect.right - 20은 오른쪽 화살표에 짤려서 조절한 것이다.
		CRect	textRect = r;
		textRect.left	+= 3;
		textRect.top	+= 4;
		textRect.right	-= 20;

		pDC->DrawText(reinterpret_cast<char*>( GetItemData(nIndex) ),
					  &textRect,
					  DT_LEFT | DT_VCENTER);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::DrawItem
/// \brief	: 리스트박스 내용을 그릴때 호출
/// \author	: CKPark
/// \date	: 5:12:2008   11:56
/// \warning: 현재 선택된 아이템은 OnPaint()에서 그려주므로 건너뛴다
/// \param	: LPDRAWITEMSTRUCT lpDrawItemStruct
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if(!pDC)
		return;

	// 현재 선택된 아이템은 OnPaint()에서 그려주므로 건너뛴다
	if(lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT)
		return;

	// 클라이언트 영역
	CRect r = lpDrawItemStruct->rcItem;

	// 백그라운드 컬러로 영역을 채운다
	pDC->FillSolidRect(r.left, r.top, r.Width(), r.Height(), m_BackColor);
	
	// 아이템을 그린다
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_TextColor);
	pDC->DrawText(reinterpret_cast<char*>(lpDrawItemStruct->itemData), &r, DT_LEFT | DT_VCENTER);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::OnKillFocus
/// \brief	: 포커스 잃을 시 영역 무효화로 다시 그린다
/// \author	: CKPark
/// \date	: 5:12:2008   11:57
/// \warning: 
/// \param	: CWnd * pNewWnd
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here

	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyComboBox::SetBitMap
/// \brief	: 에디트 박스에 쓸 비트맵 설정
/// \author	: CKPark
/// \date	: 5:12:2008   11:58
/// \warning: 
/// \param	: const int nEditBitMapID
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyComboBox::SetBitMap(const int nEditBitMapID)
{
	m_EditBitMap.LoadBitmap(nEditBitMapID);
}

void CMyComboBox::SetTextColor(const COLORREF dwTextColor)
{
	m_TextColor	= dwTextColor;
}

void CMyComboBox::SetBackColor(const COLORREF dwBackColor)
{
	m_BackColor = dwBackColor;
}
