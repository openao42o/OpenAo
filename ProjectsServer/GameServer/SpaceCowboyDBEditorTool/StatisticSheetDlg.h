#if !defined(AFX_STATISTICSHEETDLG_H__58EACBAA_344B_48D1_91C2_856C6C9FF545__INCLUDED_)
#define AFX_STATISTICSHEETDLG_H__58EACBAA_344B_48D1_91C2_856C6C9FF545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatisticSheetDlg.h : header file
//
#include "StatisticPage1Dlg.h"
#include "StatisticPage2Dlg.h"
#include "StatisticPage3Dlg.h"
#include "StatisticPage4Dlg.h"
#include "StatisticPage5Dlg.h"
#include "StatisticPage6Dlg.h"
#include "StatisticPage61Dlg.h"				// 2006-01-19 by cmkwon
#include "StatisticPage62Dlg.h"				// 2006-01-19 by cmkwon
#include "StatisticPage7Dlg.h"
#include "StatisticPage71Dlg.h"				// 2006-01-23 by cmkwon
#include "StatisticPage8Dlg.h"
#include "StatisticPage9Dlg.h"
#include "StatisticPage99Dlg.h"				// 2006-01-23 by cmkwon
#include "StatisticPage98Dlg.h"				// 2006-01-23 by cmkwon
#include "StatisticPage97Dlg.h"				// 2006-02-03 by cmkwon
#include "StatisticPage100Dlg.h"			// 2006-08-07 by dhjin
#include "StatisticPage101Dlg.h"			// 2006-08-07 by dhjin
#include "StatisticPage102Dlg.h"			// 2007-06-27 by dhjin, WarPointRanking
/////////////////////////////////////////////////////////////////////////////
// CStatisticSheetDlg

class CStatisticSheetDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CStatisticSheetDlg)

// Construction
public:
	CStatisticSheetDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CStatisticSheetDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CStatisticPage1Dlg m_page1;
	CStatisticPage2Dlg m_page2;
	CStatisticPage3Dlg m_page3;
	CStatisticPage4Dlg m_page4;
	CStatisticPage5Dlg m_page5;
	CStatisticPage6Dlg m_page6;
	CStatisticPage61Dlg m_page61;			// 2006-01-19 by cmkwon
	CStatisticPage62Dlg m_page62;			// 2006-01-19 by cmkwon
	CStatisticPage7Dlg m_page7;
	CStatisticPage71Dlg m_page71;			// 2006-01-23 by cmkwon
	CStatisticPage8Dlg m_page8;
	CStatisticPage9Dlg m_page9;
	CStatisticPage99Dlg m_page99;			// 2006-01-23 by cmkwon
	CStatisticPage98Dlg m_page98;			// 2006-01-23 by cmkwon
	CStatisticPage97Dlg m_page97;			// 2006-02-03 by cmkwon
	CStatisticPage100Dlg m_page100;			// 2006-08-07 by dhjin
	CStatisticPage101Dlg m_page101;			// 2007-01-23 by dhjin
	CStatisticPage102Dlg m_page102;			// 2007-06-27 by dhjin, WarPointRanking
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatisticSheetDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStatisticSheetDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStatisticSheetDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATISTICSHEETDLG_H__58EACBAA_344B_48D1_91C2_856C6C9FF545__INCLUDED_)
