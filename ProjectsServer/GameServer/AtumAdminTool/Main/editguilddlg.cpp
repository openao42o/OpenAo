// editguilddlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "editguilddlg.h"
#include "SCGuildAdminDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditGuildDlg dialog


CEditGuildDlg::CEditGuildDlg(SGUILD *i_pGuildInfo, BOOL i_bEnableEdit, CWnd* pParent /*=NULL*/)
	: CDialog(CEditGuildDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditGuildDlg)
	m_ctluiGuildUID = 0;
	m_ctlcsGuildName = _T("");
	//}}AFX_DATA_INIT

	m_guildInfo			= *i_pGuildInfo;
	if(GUILD_MARK_STATE_NORMAL < m_guildInfo.GuildMarkState)
	{
		m_guildInfo.GuildMarkState	= GUILD_MARK_STATE_NONE;
	}
	m_ctluiGuildUID		= m_guildInfo.GuildUID;
	m_ctlcsGuildName	= m_guildInfo.GuildName;
	
	m_bEnableEdit1		= i_bEnableEdit;	// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 -
}


void CEditGuildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditGuildDlg)
	DDX_Control(pDX, IDC_COMBO_GUILD_MARK_STATE, m_ctlcomboBoxGuildMarkState);
	DDX_Text(pDX, IDC_EDIT_GUILD_UID, m_ctluiGuildUID);
	DDX_Text(pDX, IDC_EDIT_GUILD_NAME, m_ctlcsGuildName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditGuildDlg, CDialog)
	//{{AFX_MSG_MAP(CEditGuildDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditGuildDlg message handlers

void CEditGuildDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	CString csMsg;
	if(m_ctlcsGuildName.IsEmpty())
	{
		AfxMessageBox("Input GuildName !!");
		return;
	}

	if(SIZE_MAX_GUILD_NAME <= m_ctlcsGuildName.GetLength())
	{
		csMsg.Format("GuildName is invalid !!(Max: %dBytes)", SIZE_MAX_GUILD_NAME-1);
		AfxMessageBox(csMsg);
		return;
	}

	CSCGuildAdminDlg *pSCGuildAdminDlg = (CSCGuildAdminDlg*)m_pParentWnd;

	if(0 != m_ctlcsGuildName.Compare(m_guildInfo.GuildName))
	{// 2007-07-31 by cmkwon, SCAdminTool에 여단명 변경 기능 추가 - 여단명이 변경 되었다, 체크해야함
		
		int nRet = AfxMessageBox("GuildName is changed !!\n Are you apply?", MB_OKCANCEL);
		if(IDCANCEL == nRet)
		{
			return;
		}

		if(pSCGuildAdminDlg->SearchGuildNameFromDB((LPSTR)(LPCSTR)m_ctlcsGuildName))
		{
			csMsg.Format("GuildName(%s) is already exist !!", m_ctlcsGuildName);
			AfxMessageBox(csMsg);
			return;
		}

		util::strncpy(m_guildInfo.GuildName, m_ctlcsGuildName, SIZE_MAX_GUILD_NAME);
	}

	m_guildInfo.GuildMarkState = m_ctlcomboBoxGuildMarkState.GetCurSel();
	CDialog::OnOK();
}

BOOL CEditGuildDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctlcomboBoxGuildMarkState.AddString(GetStringGuildMarkState(GUILD_MARK_STATE_NONE, TRUE));
	m_ctlcomboBoxGuildMarkState.AddString(GetStringGuildMarkState(GUILD_MARK_STATE_WAITING_PERMISSION, TRUE));
	m_ctlcomboBoxGuildMarkState.AddString(GetStringGuildMarkState(GUILD_MARK_STATE_NORMAL, TRUE));
	m_ctlcomboBoxGuildMarkState.SetCurSel(m_guildInfo.GuildMarkState);
	
	
	GetDlgItem(IDC_EDIT_GUILD_NAME)->EnableWindow(m_bEnableEdit1);	// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 -
	m_ctlcomboBoxGuildMarkState.EnableWindow(m_bEnableEdit1);		// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
