// PringDBGDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "PringDBGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPringDBGDlg dialog


CPringDBGDlg::CPringDBGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPringDBGDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPringDBGDlg)
	m_float1 = 0.0f;
	m_float2 = 0.0f;
	m_int1 = 0;
	m_int2 = 0;
	m_string = _T("");
	//}}AFX_DATA_INIT
}


void CPringDBGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPringDBGDlg)
	DDX_Text(pDX, IDC_EDIT_FLAOT1, m_float1);
	DDX_Text(pDX, IDC_EDIT_FLOAT2, m_float2);
	DDX_Text(pDX, IDC_EDIT_INT1, m_int1);
	DDX_Text(pDX, IDC_EDIT_INT2, m_int2);
	DDX_Text(pDX, IDC_EDIT_STRING, m_string);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPringDBGDlg, CDialog)
	//{{AFX_MSG_MAP(CPringDBGDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPringDBGDlg message handlers
