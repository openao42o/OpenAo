// ExpViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "ExpViewerDlg.h"
#include "AtumSJ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpViewerDlg dialog


CExpViewerDlg::CExpViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpViewerDlg)
	m_ctl_doTotalExp = 0.0;
	m_ctl_nLevel = 0;
	m_ctl_fPercent = 0.0f;
	//}}AFX_DATA_INIT
}


void CExpViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpViewerDlg)
	DDX_Text(pDX, IDC_EDIT_TOTAL_EXP, m_ctl_doTotalExp);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_ctl_nLevel);
	DDX_Text(pDX, IDC_EDIT_PERCENT, m_ctl_fPercent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CExpViewerDlg)
	ON_BN_CLICKED(IDC_BTN_TO_LEVEL, OnBtnToLevel)
	ON_BN_CLICKED(IDC_BTN_TO_EXP, OnBtnToExp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpViewerDlg message handlers


void CExpViewerDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnCancel();
//	CDialog::OnOK();
}

void CExpViewerDlg::OnBtnToLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	m_ctl_doTotalExp = max(0.0f, m_ctl_doTotalExp);

	m_ctl_nLevel = 1;
	for(int i=1; i <= CHARACTER_MAX_LEVEL; i++)		// 2010-03-11 by cmkwon, 지원레벨 110 상항 처리 빠진것 수정 - 
	{
		if(m_ctl_doTotalExp == CAtumSJ::GetLevelExperience(i)->Experience)
		{
			m_ctl_nLevel = i+1;
			break;
		}
		else if(m_ctl_doTotalExp < CAtumSJ::GetLevelExperience(i)->Experience)
		{
			break;
		}
		m_ctl_nLevel = i+1;
	}

	m_ctl_fPercent = CAtumSJ::GetPercentageOfExperience(m_ctl_doTotalExp, m_ctl_nLevel);
	
	UpdateData(FALSE);
}

void CExpViewerDlg::OnBtnToExp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_ctl_nLevel = max(1, m_ctl_nLevel);
	m_ctl_nLevel = min(CHARACTER_MAX_LEVEL, m_ctl_nLevel);	// 2010-03-11 by cmkwon, 지원레벨 110 상항 처리 빠진것 수정 - 

	m_ctl_fPercent = max(0.0f, m_ctl_fPercent);
	m_ctl_fPercent = min(100.0f, m_ctl_fPercent);

	m_ctl_doTotalExp = CAtumSJ::GetExperienceFromPercentage(m_ctl_fPercent, m_ctl_nLevel);
	
	UpdateData(FALSE);
}
