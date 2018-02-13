// MyFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog

IMPLEMENT_DYNAMIC(CMyFileDialog, CFileDialog)

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
	//{{AFX_MSG_MAP(CMyFileDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFileDialog 에는 왜 EDIT 컨트롤이 생성되지 않는지 알수가 없군요 !!
// 아시는 분은 답변 부탁드립니다.
BOOL CMyFileDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect;

	GetWindowRect(rect);
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height()+40);
	GetParent()->SetWindowText("My File Dialog");

	GetClientRect(rect);
	rect.SetRect(rect.left, rect.bottom-20, rect.right, rect.bottom);

	if( !m_edit.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
		rect, this, 444) )
		AfxMessageBox("EDIT 컨트롤을 만들수 없습니다.");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
