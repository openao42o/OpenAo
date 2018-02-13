// MyCheck.cpp : implementation file
//

#include "stdafx.h"
#include "atumlauncher.h"
#include "MyCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyCheck

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::CMyCheck
/// \brief	: 생성자
/// \author	: CKPark
/// \date	: 5:12:2008   11:40
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyCheck::CMyCheck() : CButton(), m_bMyCheckState(FALSE)
{

}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::~CMyCheck
/// \brief	: 파괴자 체크/언체크시 사용한 비트맵 해제
/// \author	: CKPark
/// \date	: 5:12:2008   11:40
/// \warning: 
/// \return	: 
///////////////////////////////////////////////////////////////////////////////
CMyCheck::~CMyCheck()
{
	for(int i=0; i<CHECKBOX_TOTAL_STATE; ++i)
	{
		if(m_CheckStateBMP[i].m_hObject)
			m_CheckStateBMP[i].DeleteObject();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::GetCheck
/// \brief	: 체크상태 얻어오기
/// \author	: CKPark
/// \date	: 5:12:2008   11:41
/// \warning: 
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CMyCheck::GetCheck()
{
	return m_bMyCheckState;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::SetCheck
/// \brief	: 체크상태 셋팅
/// \author	: CKPark
/// \date	: 5:12:2008   11:41
/// \warning: !nCheck로 셋팅하는 이유는 SetCheck호출시 OnChange가 자동으로 호출
//			  되어 어차피 다시 원래대로 바꾸기 위해서이다
/// \param	: int nCheck
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CMyCheck::SetCheck(int nCheck)
{
	m_bMyCheckState = !nCheck;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::SetBitMap
/// \brief	: 체크, 언체크시 비트맵을 셋팅 한다
/// \author	: CKPark
/// \date	: 5:12:2008   11:42
/// \warning: 
/// \param	: const int nUnCheckedBMPID
/// \param	: const int nCheckedBMPID
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CMyCheck::SetBitMap(const int nUnCheckedBMPID, const int nCheckedBMPID)
{
	m_CheckStateBMP[CHECKBOX_STATE_UNCHECKED].LoadBitmap(nUnCheckedBMPID);
	m_CheckStateBMP[CHECKBOX_STATE_CHECKED].LoadBitmap(nCheckedBMPID);
}


BEGIN_MESSAGE_MAP(CMyCheck, CButton)
 	//{{AFX_MSG_MAP(CMyCheck)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_MOUSEMOVE()
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		ON_WM_ENABLE()
		ON_MESSAGE(BM_SETCHECK, OnChange)
 	//}}AFX_MSG_MAP
 END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyCheck message handlers


///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnPaint
/// \brief	: 페인트 함수
/// \author	: CKPark
/// \date	: 5:12:2008   11:42
/// \warning: CButton::OnPaint()를 호출안하는 대신 CPaintDC dc(this)를 생성한다
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnPaint()
{
	CPaintDC		dc(this);

	// 만약 체크/언체크 비트맵이 로딩되있을 경우만 비트맵으로 그린다
	if(m_CheckStateBMP[CHECKBOX_STATE_UNCHECKED].m_hObject &&
	   m_CheckStateBMP[CHECKBOX_STATE_CHECKED].m_hObject)
	{
		CDC*		pDC	= GetDC();
		if(!pDC)
			return;

		// 매모리 dc생성
		CDC			memDC;
		memDC.CreateCompatibleDC(NULL);
		
		
		// 체크/언체크에 따라 비트맵 설정
		CBitmap*	pOld;
		if(m_bMyCheckState)
			pOld = memDC.SelectObject(&m_CheckStateBMP[CHECKBOX_STATE_CHECKED]);
		else
			pOld = memDC.SelectObject(&m_CheckStateBMP[CHECKBOX_STATE_UNCHECKED]);

		// 클라이언트 영역
		CRect		r;
		GetClientRect(r);

 		// dc에 메모리 dc내용 복사
		pDC->BitBlt(0, 0, r.Width(), r.Height(),
					&memDC, 0, 0, SRCCOPY);

		// 사용한 dc 해제
		memDC.SelectObject(pOld);
		memDC.DeleteDC();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnChange
/// \brief	: 체크, 비체크가 바뀔때 호출됨, m_bMyCheckState 변경, 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:43
/// \warning: 
/// \param	: WPARAM wParam
/// \param	: LPARAM lParam
/// \return	: LRESULT
///////////////////////////////////////////////////////////////////////////////
LRESULT CMyCheck::OnChange(WPARAM wParam, LPARAM lParam) 
{
	m_bMyCheckState = !m_bMyCheckState;
	Invalidate();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnMouseMove
/// \brief	: 마우스 움직일때 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:44
/// \warning: 
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnMouseMove(UINT nFlags, CPoint point)
{
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnLButtonDown
/// \brief	: 마우스 다운될때 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:45
/// \warning: CButton::OnLButtonDown을 호출안하면 OnChange가 호출되지 않는다
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags, point);
	Invalidate();	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnLButtonUp
/// \brief	: 마우스 업될때 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:45
/// \warning: CButton::OnLButtonUp을 호출안하면 OnChange가 호출되지 않는다
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnLButtonUp(UINT nFlags, CPoint point)
{
	CButton::OnLButtonUp(nFlags, point);
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnLButtonDblClk
/// \brief	: 더블클릭시 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:46
/// \warning: 
/// \param	: UINT nFlags
/// \param	: CPoint point
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CMyCheck::OnEnable
/// \brief	: 컨트롤이 활성화 될때 영역 무효화
/// \author	: CKPark
/// \date	: 5:12:2008   11:46
/// \warning: 
/// \param	: BOOL bEnable
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void CMyCheck::OnEnable(BOOL bEnable)
{
	Invalidate();
}