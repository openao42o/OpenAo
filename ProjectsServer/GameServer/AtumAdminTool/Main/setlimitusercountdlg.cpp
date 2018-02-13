// setlimitusercountdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "setlimitusercountdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLimitUserCountDlg dialog


CSetLimitUserCountDlg::CSetLimitUserCountDlg(int i_nLimitUserCount, CWnd* pParent /*=NULL*/)
	: CDialog(CSetLimitUserCountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLimitUserCountDlg)
	m_nUserCount = 0;
	//}}AFX_DATA_INIT

	m_nUserCount = i_nLimitUserCount;		// 2008-04-29 by cmkwon, 서버군 정보 DB에 추가(신규 계정 캐릭터 생성 제한 시스템추가) - 
}


void CSetLimitUserCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLimitUserCountDlg)
	DDX_Text(pDX, IDC_EDIT_USER_COUNT, m_nUserCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLimitUserCountDlg, CDialog)
	//{{AFX_MSG_MAP(CSetLimitUserCountDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLimitUserCountDlg message handlers
