#include "StdAfx.h"
#include "MyEdit.h"

CMyEdit::CMyEdit() : CEdit()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::~CMyEdit
/// \brief	: 파괴자 에디트 박스에 사용한 비트맵 삭제
/// \author	: CKPark
/// \date	: 5:12:2008   12:00
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyEdit::~CMyEdit()
{
	if(m_Bmp.m_hObject)
		m_Bmp.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::SetBitMap
/// \brief	: 에디트 박스에 사용할 비트맵 설정
/// \author	: CKPark
/// \date	: 5:12:2008   12:01
/// \warning: 
/// \param	: const int nBitMapID
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CMyEdit::SetBitMap(const int nBitMapID)
{
	m_Bmp.LoadBitmap(nBitMapID);
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
//{{AFX_MSG_MAP(CMyEdit)
ON_WM_PAINT()
ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::OnPaint
/// \brief	: 영역무효화시 페인팅
/// \author	: CKPark
/// \date	: 5:12:2008   12:01
/// \warning: CEdit::OnPaint()를 페인팅이 끝난 뒤 호출한다. 그렇지 않다면
///			  직접 텍스트를 그려줘야 한다
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyEdit::OnPaint() 
{
	// 비트맵이 로딩됬을 경우만 그려줌
	// 비트맵이 로딩되지 않았을 경우
	// CEdit::OnPaint()에 의해 기본 에디트 박스가 그려진다
	if( m_Bmp.m_hObject)
	{
		CDC			*pDC	= GetDC();
		if(!pDC)
			return;

		// 메모리 dc 생성
		CDC			memDC;
		memDC.CreateCompatibleDC(pDC);

		// 메모리 dc 비트맵 설정
		CBitmap *pOld	= memDC.SelectObject(&m_Bmp);
		
		// 클라이언트 영역
		CRect		r;
		GetClientRect(r);

		// 비트맵 헤더
		BITMAP bmp;
		m_Bmp.GetObject(sizeof(bmp), &bmp);

		// -3, -3, 5, 5는 적당히 맞춰준 숫자임
		pDC->StretchBlt(-3, -3, r.right + 5, r.bottom + 5,
						&memDC, 0, 0,
						bmp.bmWidth, bmp.bmHeight, SRCCOPY);

		// 메모리 dc 해제
		memDC.SelectObject(pOld);
		memDC.DeleteDC();
	}

	CEdit::OnPaint();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::OnChange
/// \brief	: 에디트 박스 내용이 바뀌었을 경우 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   12:04
/// \warning: 
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyEdit::OnChange() 
{
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyEdit::OnSetfocus
/// \brief	: 포커스를 얻었을 시 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   12:04
/// \warning: 
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyEdit::OnSetfocus() 
{
	Invalidate();
}