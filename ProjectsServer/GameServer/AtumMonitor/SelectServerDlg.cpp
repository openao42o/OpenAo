// selectserverdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\AtumMonitor\atummonitor.h"
#include "selectserverdlg.h"
#include "BrowseForFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectServerDlg dialog


CSelectServerDlg::CSelectServerDlg(CString i_strServerName, CWnd* pParent /*=NULL*/) :
	CDialog { IDD, pParent },
	m_strServerName { i_strServerName }
{
}


void CSelectServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERVER_LIST, m_ComboServerList);
}


BEGIN_MESSAGE_MAP(CSelectServerDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectServerDlg message handlers

BOOL CSelectServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();



	// TODO: Add extra initialization here
	int nComboSelIndex = 0, i;
	for (i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		m_ComboServerList.AddString(g_arrGameServers[i].ServerName);
		if (0 == m_strServerName.Compare(g_arrGameServers[i].ServerName))
		{
			nComboSelIndex = i;
		}
	}
	m_ComboServerList.SetCurSel(nComboSelIndex);


	GetDlgItem(IDC_EDIT_LANG_INFO)->SetWindowText("");
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CString szServerName;
	m_ComboServerList.GetLBText(m_ComboServerList.GetCurSel(), szServerName);

	for (int i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		if (szServerName == g_arrGameServers[i].ServerName)
		{
			m_strServerName = szServerName;
			m_GameServerInfoForAdmin = g_arrGameServers[i];
			break;
		}
	}
	
	CDialog::OnOK();
}
