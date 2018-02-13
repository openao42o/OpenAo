// SQLReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "SQLReaderDlg.h"
#include "SQLReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSQLReaderDlg dialog


CSQLReaderDlg::CSQLReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSQLReaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSQLReaderDlg)
	m_szSQLFilePath = _T("");
	m_bCBArray = FALSE;
	//}}AFX_DATA_INIT
}


void CSQLReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSQLReaderDlg)
	DDX_Text(pDX, IDC_EDIT_INFO_SQL_FILE_PATH, m_szSQLFilePath);
	DDX_Check(pDX, IDC_CBARRAY_CHECKBOX, m_bCBArray);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSQLReaderDlg, CDialog)
	//{{AFX_MSG_MAP(CSQLReaderDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSQLReaderDlg message handlers

#include "AtumDBManager.h"

void CSQLReaderDlg::OnOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CSQLReader sqlReader;
	sqlReader.CreateAccessCode((char*)(LPCSTR)m_szSQLFilePath, m_bCBArray);

	GetDlgItem(IDC_EDIT_OUTPUT_CODE)->SetWindowText(sqlReader.m_szOutText.c_str());

	for (int i = 0; i < 100; i++)
	{
		DBGOUT("%3d %s\r\n", i, GetDBQueryTypeString((EnumQueryType)i));
	}
}

