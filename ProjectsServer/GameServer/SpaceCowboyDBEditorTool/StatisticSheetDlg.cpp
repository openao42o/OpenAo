// StatisticSheetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "spacecowboydbeditortool.h"
#include "StatisticSheetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStatisticSheetDlg

IMPLEMENT_DYNAMIC(CStatisticSheetDlg, CPropertySheet)

CStatisticSheetDlg::CStatisticSheetDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
	AddPage(&m_page4);
// 2006-07-19 by dhjin
	AddPage(&m_page5);
	AddPage(&m_page6);
	AddPage(&m_page61);
	AddPage(&m_page62);
	AddPage(&m_page7);
	AddPage(&m_page71);
	AddPage(&m_page8);
	AddPage(&m_page9);
	AddPage(&m_page99);
	AddPage(&m_page98);
	AddPage(&m_page97);
	AddPage(&m_page100);
	AddPage(&m_page101);
	AddPage(&m_page102);			// 2007-06-27 by dhjin, WarPointRanking
}

CStatisticSheetDlg::CStatisticSheetDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
	AddPage(&m_page4);
// 2006-07-19 by dhjin
	AddPage(&m_page5);
	AddPage(&m_page6);
	AddPage(&m_page61);
	AddPage(&m_page62);
	AddPage(&m_page7);
	AddPage(&m_page71);
	AddPage(&m_page8);
	AddPage(&m_page9);
	AddPage(&m_page99);
	AddPage(&m_page98);
	AddPage(&m_page97);
	AddPage(&m_page100);
	AddPage(&m_page101);
	AddPage(&m_page102);			// 2007-06-27 by dhjin, WarPointRanking
}

CStatisticSheetDlg::~CStatisticSheetDlg()
{
}


BEGIN_MESSAGE_MAP(CStatisticSheetDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CStatisticSheetDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticSheetDlg message handlers
