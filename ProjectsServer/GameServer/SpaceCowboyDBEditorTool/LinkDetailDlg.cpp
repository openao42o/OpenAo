// LinkDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "LinkDetailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinkDetailDlg dialog


CLinkDetailDlg::CLinkDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinkDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinkDetailDlg)
	m_valueEditItemNum = _T("");
	m_valueEditLink = _T("");
	//}}AFX_DATA_INIT
}


void CLinkDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinkDetailDlg)
	DDX_Text(pDX, IDC_IE_EDIT1, m_valueEditItemNum);
	DDX_Text(pDX, IDC_IE_EDIT36, m_valueEditLink);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinkDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CLinkDetailDlg)
	ON_BN_CLICKED(IDC_IE_BUTTON_END, OnButtonEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinkDetailDlg message handlers

void CLinkDetailDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
