// MyTabCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////
// This class is provided as is and Ben Hill takes no
// responsibility for any loss of any kind in connection
// to this code.
/////////////////////////////////////////////////////
// Is is meant purely as a educational tool and may
// contain bugs.
/////////////////////////////////////////////////////
// ben@shido.fsnet.co.uk
// http://www.shido.fsnet.co.uk
/////////////////////////////////////////////////////
// Thanks to a mystery poster in the C++ forum on 
// www.codeguru.com I can't find your name to say thanks
// for your Control drawing code. If you are that person 
// thank you very much. I have been able to use some of 
// you ideas to produce this sample application.
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "MyTabCtrl.h"

#include "LogTabConnection.h"
#include "LogTabItem.h"
#include "LogTabUser.h"
#include "LogTabGuildItem.h"
#include "LogTabServerIntegration.h"
#include "logtabblockaccount.h"			// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
}

CMyTabCtrl::~CMyTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}

void CMyTabCtrl::Init(CDialog *i_pMainDlg)
{
	m_tabPages[0] = new CLogTabConnection(i_pMainDlg);
	m_tabPages[1] = new CLogTabUser(i_pMainDlg);
	m_tabPages[2] = new CLogTabItem(i_pMainDlg);
	m_tabPages[3] = new CLogTabGuildItem(i_pMainDlg);
	m_tabPages[4] = new CLogTabServerIntegration(i_pMainDlg);
	m_tabPages[5] = new CLogTabBlockAccount(i_pMainDlg);		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 

	m_nNumberOfPages = NUM_OF_LOG_TAB_PAGES;

	m_tabCurrent=0;

	m_tabPages[0]->Create(IDD_LOG_TAB_CONNECTION, this);
	m_tabPages[1]->Create(IDD_LOG_TAB_USER, this);
	m_tabPages[2]->Create(IDD_LOG_TAB_ITEM, this);
	m_tabPages[3]->Create(IDD_LOG_TAB_GUILDITEM, this);
	m_tabPages[4]->Create(IDD_LOG_TAB_SERVERINTEGRATION, this);
	m_tabPages[5]->Create(IDD_LOG_TAB_BLOCKACCOUNT, this);		// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 

	m_tabPages[0]->ShowWindow(SW_SHOW);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
	m_tabPages[3]->ShowWindow(SW_HIDE);
	m_tabPages[4]->ShowWindow(SW_HIDE);
	m_tabPages[5]->ShowWindow(SW_HIDE);			// 2008-01-30 by cmkwon, 계정 블럭 로그 남기기 구현 - 

	SetRectangle();
}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++)
	{
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers

void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (m_tabCurrent != GetCurFocus())
	{
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();
	}
}
