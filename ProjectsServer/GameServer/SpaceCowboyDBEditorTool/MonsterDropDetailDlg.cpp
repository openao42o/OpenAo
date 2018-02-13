// MonsterDropDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "MonsterDropDetailDlg.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterDropDetailDlg dialog


CMonsterDropDetailDlg::CMonsterDropDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterDropDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterDropDetailDlg)
	m_valueEditMonsterUniqueNumber = _T("75");
	m_valueEditItemNumber = _T("7000000");
	m_valueEditMinCount = _T("0");
	m_valueEditMaxCount = _T("0");
	m_valueEditProbability = _T("1");
	m_valueEditPrefixProbability = _T("1");
	m_valueEditSuffixProbability = _T("1");
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd(); // 메인 다이얼로그 포인터 얻기
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pMonsterDropDlg = (CMonsterDropDlg*)pParent; //부모 다이얼로그 포인터 저장
}


CMonsterDropDetailDlg::~CMonsterDropDetailDlg()
{
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}



void CMonsterDropDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterDropDetailDlg)
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_ctrlButtonSave);
	DDX_Control(pDX, IDC_BUTTON_MAKE, m_ctrlButtonMake);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_ctrlButtonCancel);
	DDX_Text(pDX, IDC_EDIT1, m_valueEditMonsterUniqueNumber);
	DDX_Text(pDX, IDC_EDIT2, m_valueEditItemNumber);
	DDX_Text(pDX, IDC_EDIT3, m_valueEditMinCount);
	DDX_Text(pDX, IDC_EDIT4, m_valueEditMaxCount);
	DDX_Text(pDX, IDC_EDIT5, m_valueEditProbability);
	DDX_Text(pDX, IDC_EDIT6, m_valueEditPrefixProbability);
	DDX_Text(pDX, IDC_EDIT7, m_valueEditSuffixProbability);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterDropDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterDropDetailDlg)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMake)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterDropDetailDlg message handlers

BOOL CMonsterDropDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//DB 접근 초기화
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
//		EndDialog(-1);
//		return FALSE;
//	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMonsterDropDetailDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CMonsterDropDetailDlg::OnButtonMake() 
{
	// TODO: Add your control notification handler code here
	if(ValidateCheck()== FALSE)
		return;
	ReflectionToGrid(1);
}

void CMonsterDropDetailDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if(ValidateCheck()== FALSE)
		return;
	m_pMonsterDropDlg->m_GridCurrent.DeleteRow(m_nSelectRow);
	ReflectionToGrid(2);
}

bool CMonsterDropDetailDlg::ValidateCheck()
{
	UpdateData();

	if(atoi((char*)(LPCTSTR)m_valueEditMonsterUniqueNumber) < 2000000 || atoi((char*)(LPCTSTR)m_valueEditMonsterUniqueNumber) > 7599999)
	{
		AfxMessageBox("Please Check MonsterUniqueNumber Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditItemNumber) < 7000000 || atoi((char*)(LPCTSTR)m_valueEditItemNumber) > 7999999)
	{
		AfxMessageBox("Please Check ItemNum Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMinCount) < 0 || atoi((char*)(LPCTSTR)m_valueEditMinCount) > 90000000)
	{
		AfxMessageBox("Please Check MinCount Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMaxCount) < 0 || atoi((char*)(LPCTSTR)m_valueEditMaxCount) > 99999999)
	{
		AfxMessageBox("Please Check MaxCount Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditProbability) < 0 || atoi((char*)(LPCTSTR)m_valueEditProbability) > 1000000)
	{
		AfxMessageBox("Please Check Probability Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditPrefixProbability) < 0 || atoi((char*)(LPCTSTR)m_valueEditPrefixProbability) > 100000)
	{
		AfxMessageBox("Please Check PrefixProbability Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditSuffixProbability) < 0 || atoi((char*)(LPCTSTR)m_valueEditSuffixProbability) > 100000)
	{
		AfxMessageBox("Please Check PrefixProbability Value");
		return FALSE;
	}

	return TRUE;
}

void CMonsterDropDetailDlg::ReflectionToGrid(int type)
{
	// 칼럼 만들기
	m_pMonsterDropDlg->m_GridUpdate.SetRowCount(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()+1);

	//각 콘트롤의 값들을 CWeaponItem의 GridUpdate에 넣는다.
	UpdateData();
	
	int m_nCols= 0;
	CString tempValue;
	if(type == 1)
		m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "INSERT");
	else if(type == 2)
		m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, "UPDATE");
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMonsterUniqueNumber);
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemNumber);
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMinCount);
	
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMaxCount);
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditProbability);
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditPrefixProbability);
	m_pMonsterDropDlg->m_GridUpdate.SetItemText(m_pMonsterDropDlg->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSuffixProbability);

	m_pMonsterDropDlg->m_GridUpdate.Refresh();

	EndDialog(0);
}
