// MPOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCBitFlag.h"
#include "MPOptionDlg.h"
#include "AtumParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPOptionDlg dialog


CMPOptionDlg::CMPOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMPOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMPOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	memset(m_ArrMPOptionComment, 0x00, sizeof(m_ArrMPOptionComment[0])*MPOPTION_BIT_COUNTS);
}


void CMPOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPOptionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CMPOptionDlg)
	ON_BN_CLICKED(IDC_CHECK0, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK1, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK2, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK3, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK4, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK5, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK6, OnCheckBtnClicked)
	ON_BN_CLICKED(IDC_CHECK7, OnCheckBtnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPOptionDlg message handlers

BOOL CMPOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// MPOPTION_BIT_DEAD_FALL
	strcpy(m_ArrMPOptionComment[0].szMPOptionComment[0]
		, "MPOPTION_BIT_DEAD_FALL \n                            ==> 죽을 때 폭발");
	strcpy(m_ArrMPOptionComment[0].szMPOptionComment[1]
		, "MPOPTION_BIT_DEAD_FALL \n                            ==> 죽을 때 추락");

	// MPOPTION_BIT_MOVE_PARTY
	strcpy(m_ArrMPOptionComment[1].szMPOptionComment[0]
		, "MPOPTION_BIT_MOVE_PARTY \n                            ==> 편대 비행 불가");
	strcpy(m_ArrMPOptionComment[1].szMPOptionComment[1]
		, "MPOPTION_BIT_MOVE_PARTY \n                            ==> 편대 비행 가능(MPOptionParam1 = 편대 형태, MPOptionParam2 = 최대편대 마리수)");

	// MPOPTION_BIT_PATTERN_MONSTER
	strcpy(m_ArrMPOptionComment[2].szMPOptionComment[0]
		, "MPOPTION_BIT_PATTERN_MONSTER \n                            ==> 패턴 몬스터가 아님");
	strcpy(m_ArrMPOptionComment[2].szMPOptionComment[1]
		, "MPOPTION_BIT_PATTERN_MONSTER \n                            ==> 패턴 몬스터임");
	
	// MPOPTION_BIT_BOSS_MONSTER
	strcpy(m_ArrMPOptionComment[3].szMPOptionComment[0]
		, "MPOPTION_BIT_BOSS_MONSTER \n                            ==> 보스 몬스터 아님");
	strcpy(m_ArrMPOptionComment[3].szMPOptionComment[1]
		, "MPOPTION_BIT_BOSS_MONSTER \n                            ==> 보스 몬스터임");
	
	OnCheckBtnClicked();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMPOptionDlg::OnCheckBtnClicked(void)
{
	int		nDecimalNum = 0;
	int		nID = IDC_CHECK0;
	CWnd	*pWndCheckBtn = NULL;
	for(int i=0; i < MPOPTION_BIT_COUNTS; i++)
	{
		pWndCheckBtn = GetDlgItem(nID+i);
		if(FALSE == this->IsDlgButtonChecked(nID+i))
		{
			pWndCheckBtn->SetWindowText(m_ArrMPOptionComment[i].szMPOptionComment[0]);
		}
		else
		{
			nDecimalNum += (1<<i);
			pWndCheckBtn->SetWindowText(m_ArrMPOptionComment[i].szMPOptionComment[1]);
		}
	}

	char szTemp[10];	
	GetDlgItem(IDC_STATIC_DECIMAL)->SetWindowText(itoa(nDecimalNum, szTemp, 10));
}

