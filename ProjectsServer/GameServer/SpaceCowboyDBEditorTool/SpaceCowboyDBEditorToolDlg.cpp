// SpaceCowboyDBEditorToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "SpaceCowboyDBEditorToolDlg.h"
#include "md5_lib_src.h"
#include "AtumDBHelper.h"
#include "SCLogAdminDlg.h"				// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolDlg dialog

CSpaceCowboyDBEditorToolDlg::CSpaceCowboyDBEditorToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpaceCowboyDBEditorToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpaceCowboyDBEditorToolDlg)
	m_valueEditID = _T("");
	m_pMonsterDlg = NULL;
	m_pMonsterDropDlg = NULL;
	m_pMonsterAttackDlg = NULL;
	m_pQuestEditorDlg = NULL;
	m_pWeaponItemDlg = NULL;
	m_pEngineItemDlg = NULL;
	m_pMapEditorDlg = NULL;
	m_pStatisticDlg = NULL;
	m_valueEditPW = _T("");
	m_ctl_strLanguageString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pODBCStmt = new CODBCStatement;

	m_bEnableEdit			= FALSE;			// 2006-05-08 by cmkwon
	m_pLogAdminDlg			= NULL;				// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -
}

CSpaceCowboyDBEditorToolDlg::~CSpaceCowboyDBEditorToolDlg()
{
	util::del(m_pMonsterDlg);
	util::del(m_pMonsterAttackDlg);
	util::del(m_pMonsterDropDlg);
	util::del(m_pQuestEditorDlg);
	util::del(m_pWeaponItemDlg);
	util::del(m_pEngineItemDlg);
	util::del(m_pMapEditorDlg);
	util::del(m_pStatisticDlg);
	util::del(m_pWriteLog);
	util::del(m_pLogAdminDlg);		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -


	util::del(m_pODBCStmt);
	
}

void CSpaceCowboyDBEditorToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpaceCowboyDBEditorToolDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER, m_ctrlComboServer);
	DDX_Text(pDX, IDC_EDIT_ID, m_valueEditID);
	DDX_Text(pDX, IDC_EDIT_PW, m_valueEditPW);
	DDX_Text(pDX, IDC_EDIT_LANGUAGE, m_ctl_strLanguageString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpaceCowboyDBEditorToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSpaceCowboyDBEditorToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_END, OnButtonEnd)
	ON_BN_CLICKED(IDC_BUTTON_MONSTER, OnButtonMonster)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERATTACK, OnButtonMonsterattack)
	ON_BN_CLICKED(IDC_BUTTON_MONSTERDROP, OnButtonMonsterdrop)
	ON_BN_CLICKED(IDC_BUTTON_QUESTEDITOR, OnButtonQuesteditor)
	ON_BN_CLICKED(IDC_BUTTON_WEAPON, OnButtonWeapon)
	ON_BN_CLICKED(IDC_BUTTON_ENGINE, OnButtonEngine)
	ON_BN_CLICKED(IDC_BUTTON_MAPEDITOR, OnButtonMapeditor)
	ON_BN_CLICKED(IDC_BUTTON_STATISTIC, OnButtonStatistic)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LOG_MANAGEMENT, OnBtnLogManagement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceCowboyDBEditorToolDlg message handlers

BOOL CSpaceCowboyDBEditorToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	int nComboSelIndex = 0;
	for (int i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		m_ctrlComboServer.AddString(g_arrGameServers[i].ServerName);
		if (m_szServerName == g_arrGameServers[i].ServerName)
		{
			nComboSelIndex = i;
		}
	}

	m_pWriteLog = new CWriteLog();

	m_ctrlComboServer.SetCurSel(nComboSelIndex);
	EnableToolControls(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpaceCowboyDBEditorToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpaceCowboyDBEditorToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

HCURSOR CSpaceCowboyDBEditorToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpaceCowboyDBEditorToolDlg::EnableToolControls(BOOL i_bEnable)
{
	GetDlgItem(IDC_BUTTON_MONSTER)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_MONSTERATTACK)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_MONSTERDROP)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_ID))->SetReadOnly(i_bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_PW))->SetReadOnly(i_bEnable);
	GetDlgItem(IDC_BUTTON_SELECT)->EnableWindow(!i_bEnable);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_WEAPON)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_ENGINE)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_QUESTEDITOR)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_MAPEDITOR)->EnableWindow(i_bEnable);

	if(FALSE == i_bEnable
		|| this->GetEnbaleEdit())
	{
		GetDlgItem(IDC_BUTTON_STATISTIC)->EnableWindow(i_bEnable);
	}

	GetDlgItem(IDC_BTN_LOG_MANAGEMENT)->EnableWindow(i_bEnable);		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

}

void CSpaceCowboyDBEditorToolDlg::OnButtonSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_ctrlComboServer.GetCurSel() != -1)
	{
		m_pServerInfo4Admin = &g_arrGameServers[m_ctrlComboServer.GetCurSel()];
		m_ctrlComboServer.GetLBText(m_ctrlComboServer.GetCurSel(), m_szServerName);

		//DB에 select해서 값이 있는지 판단.
		// 2007-11-07 by cmkwon, 로그 DB 서버 따로 구축하기 - 아래와 같이 로그 DB 서버로 연결을 한다
		//if (!m_pODBCStmt->Init(m_pServerInfo4Admin->DBIP, m_pServerInfo4Admin->DBPort, m_pServerInfo4Admin->DBName,
		//						m_pServerInfo4Admin->DBUID, m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
		if (!m_pODBCStmt->Init(m_pServerInfo4Admin->LogDBIP, m_pServerInfo4Admin->LogDBPort, m_pServerInfo4Admin->DBName,
								m_pServerInfo4Admin->LogDBUID, m_pServerInfo4Admin->LogDBPWD, GetSafeHwnd())
		)
		{
			EndDialog(-1);

			char szTemp[1024];
			sprintf(szTemp, "Can't connect to DBServer(%s:%d)!", m_pServerInfo4Admin->LogDBIP, m_pServerInfo4Admin->LogDBPort);
			AfxMessageBox(szTemp);
			return;
		}

		m_bEnableEdit		= FALSE;
		if(FALSE == CheckPasswordAndRace(&m_bEnableEdit, m_pODBCStmt, (LPSTR)(LPCTSTR)m_valueEditID, (LPSTR)(LPCTSTR)m_valueEditPW))
		{
			AfxMessageBox("Invalid AccountName or Password !");
			return;
		}


		// 2006-01-19 by cmkwon
		this->ResetItemInfoANDRareInfo();

		EnableToolControls(TRUE);
	}
	else
	{
		m_pServerInfo4Admin = NULL;
	}

	GetDlgItem(IDC_BUTTON_QUESTEDITOR)->EnableWindow(this->GetEnbaleEdit());	// 2006-05-08 by cmkwon
}

void CSpaceCowboyDBEditorToolDlg::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	EnableToolControls(FALSE);
}

BOOL CSpaceCowboyDBEditorToolDlg::CheckPasswordAndRace(BOOL *o_pbEnableEdit, CODBCStatement *i_pODBCStatement, char *i_szAccName, char *i_szPassword)
{
	*o_pbEnableEdit				= FALSE;		// 2006-05-08 by cmkwon

	CString szSQLQuery;
	MD5 MD5_instance;
	BYTE byMD5Password[SIZE_MAX_PASSWORD_MD5];
	char szMD5Password[SIZE_MAX_PASSWORD_MD5_STRING];
	MD5_instance.MD5Encode(i_szPassword, byMD5Password);
	MD5_instance.MD5Binary2String(byMD5Password, szMD5Password);

	///////////////////////////////////////////////////////////////////////////////
	// 1. MD5 인코딩으로 체크
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select accounttype from td_account where accountname = '%s' and password = '%s'",
	//	i_szAccName, szMD5Password);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 	szSQLQuery.Format(QUERY_080702_0120, i_szAccName, szMD5Password);
// 	SQLINTEGER cb1 = SQL_NTS;
// 	short acounttype=0;
// 	SQLBindCol(i_pODBCStatement->m_hstmt, 1, SQL_C_SSHORT, &acounttype, 0, &cb1);	
// 	BOOL bRet = i_pODBCStatement->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (FALSE == bRet)
// 	{// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Query Error !!");
// 		return FALSE;
// 	}
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, i_szAccName, 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, i_szPassword, 0,	&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PASSWORD_MD5_STRING, 0, szMD5Password, 0,	&arrCB2[3]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0120));
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
 		MessageBox("Query Error !!");
 		return FALSE;
	}
  	short acounttype=0;
	arrCB2[1] = SQL_NTS;
	SQLBindCol(i_pODBCStatement->m_hstmt, 1, SQL_C_SSHORT, &acounttype, 0, &arrCB2[1]);	

	if(SQL_NO_DATA != SQLFetch(m_pODBCStmt->m_hstmt))
	{
		if(COMPARE_RACE(acounttype, RACE_GAMEMASTER | RACE_OPERATION))
		{
			if(COMPARE_RACE(acounttype, RACE_OPERATION))
			{
				*o_pbEnableEdit		= TRUE;
			}
			m_pODBCStmt->FreeStatement();
			return TRUE;
		}
	}
	m_pODBCStmt->FreeStatement();

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 위의 프로시저 하나로 동시에 체크
// 	///////////////////////////////////////////////////////////////////////////////
// 	// 2. 인코딩하지않고 체크
// 	cb1 = SQL_NTS;
// 	acounttype=0;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select accounttype from td_account where accountname = '%s' and password = '%s'",
// 	//	i_szAccName, i_szPassword);
// 	szSQLQuery.Format(QUERY_080702_0121, i_szAccName, i_szPassword);
// 
// 	SQLBindCol(i_pODBCStatement->m_hstmt, 1, SQL_C_SSHORT, &acounttype, 0, &cb1);	
// 	bRet = i_pODBCStatement->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (FALSE == bRet)
// 	{// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Query Error1 !!");
// 		return FALSE;
// 	}
// 	if(SQL_NO_DATA != SQLFetch(m_pODBCStmt->m_hstmt))
// 	{
// 		if(COMPARE_RACE(acounttype, RACE_GAMEMASTER | RACE_OPERATION))
// 		{
// 			if(COMPARE_RACE(acounttype, RACE_OPERATION))
// 			{
// 				*o_pbEnableEdit		= TRUE;
// 			}
// 			m_pODBCStmt->FreeStatement();
// 			return TRUE;
// 		}
// 	}
// 	m_pODBCStmt->FreeStatement();

	return FALSE;
}

void CSpaceCowboyDBEditorToolDlg::OnButtonEnd() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CSpaceCowboyDBEditorToolDlg::OnButtonMonster() 
{
	// TODO: Add your control notification handler code here
	if (m_pMonsterDlg != NULL)
	{
		util::del(m_pMonsterDlg);
	}

	if (m_pMonsterDlg == NULL)
	{
		m_pMonsterDlg = new CMonsterDlg(this);
		if(m_pMonsterDlg->Create(IDD_DIALOG_MONSTER, this)== TRUE)
			m_pMonsterDlg->ShowWindow(SW_SHOW);
		else
		{
			AfxMessageBox("DB Connect Fail");
			EnableToolControls(FALSE);
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonMonsterattack() 
{
	// TODO: Add your control notification handler code here
	if (m_pMonsterAttackDlg != NULL)
	{
		util::del(m_pMonsterAttackDlg);
	}

	if (m_pMonsterAttackDlg == NULL)
	{
		m_pMonsterAttackDlg = new CMonsterAttackDlg(MONSTER_ITEMLIST_TYPE_ALL_MONSTER, NULL, this);
		if(m_pMonsterAttackDlg->Create(IDD_DIALOG_MONSTERATTACK, this)== TRUE)
		{
			m_pMonsterAttackDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("DB Connect Fail");
			EnableToolControls(FALSE);
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonMonsterdrop() 
{
	// TODO: Add your control notification handler code here
	if (m_pMonsterDropDlg != NULL)
	{
		util::del(m_pMonsterDropDlg);
	}

	if (m_pMonsterDropDlg == NULL)
	{
		m_pMonsterDropDlg = new CMonsterDropDlg(this);
		if(m_pMonsterDropDlg->Create(IDD_DIALOG_MONSTERDROP, this)== TRUE)
		{
			m_pMonsterDropDlg->GetCurrentList(0);
			m_pMonsterDropDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("DB Connect Fail");
			EnableToolControls(FALSE);
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonQuesteditor() 
{
	// TODO: Add your control notification handler code here
	if (m_pQuestEditorDlg != NULL)
	{
		util::del(m_pQuestEditorDlg);
	}

	if (m_pQuestEditorDlg == NULL)
	{
		m_pQuestEditorDlg = new CQuestEditorDlg(this);
		if(m_pQuestEditorDlg->Create(IDD_DIALOG_QUESTEDITOR, this)== TRUE)
		{
			m_pQuestEditorDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("DB Connect Fail");
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonWeapon() 
{
	// TODO: Add your control notification handler code here
	if (m_pWeaponItemDlg != NULL)
	{
		util::del(m_pWeaponItemDlg);
	}

	if (m_pWeaponItemDlg == NULL)
	{
		m_pWeaponItemDlg = new CWeaponItem(this);
		if(m_pWeaponItemDlg->Create(IDD_DIALOG_WEAPON_ITEM, this)== TRUE)
			m_pWeaponItemDlg->ShowWindow(SW_SHOW);
		else
		{
			AfxMessageBox("DB Connect Fail");
			EnableToolControls(FALSE);
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonEngine() 
{
	// TODO: Add your control notification handler code here
	if (m_pEngineItemDlg != NULL)
	{
		util::del(m_pEngineItemDlg);
	}

	if (m_pEngineItemDlg == NULL)
	{
		m_pEngineItemDlg = new CEngineItemDlg(this);
		if(m_pEngineItemDlg->Create(IDD_DIALOG_ENGINE_ITEM, this)== TRUE)
			m_pEngineItemDlg->ShowWindow(SW_SHOW);
		else
		{
			AfxMessageBox("DB Connect Fail");
			EnableToolControls(FALSE);
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonMapeditor() 
{
	// TODO: Add your control notification handler code here
	if (m_pMapEditorDlg != NULL)
	{
		util::del(m_pMapEditorDlg);
	}

	if (m_pMapEditorDlg == NULL)
	{
		m_pMapEditorDlg = new CMapEditorDlg(this);
		if(m_pMapEditorDlg->Create(IDD_DIALOG_MAP, this)== TRUE)
		{
			m_pMapEditorDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("DB Connect Fail");
		}
	}
}

void CSpaceCowboyDBEditorToolDlg::OnButtonStatistic() 
{
	// TODO: Add your control notification handler code here
/*	if (m_pStatisticDlg != NULL)
	{
		util::del(m_pStatisticDlg);
	}

	if (m_pStatisticDlg == NULL)
	{
		m_pStatisticDlg = new CStatisticSheetDlg;
		if(m_pStatisticDlg->Create(IDD_DIALOG_STATISTIC, this)== TRUE)
		{
			m_pStatisticDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			AfxMessageBox("DB Connect Fail");
		}
	}*/
	CStatisticSheetDlg dlg("STATISTIC");

	if(dlg.DoModal() == IDOK)
	{
//		AfxMessageBox(dlg.UserInput());
	}
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSpaceCowboyDBEditorToolDlg::ResetItemInfoANDRareInfo(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-01-19 ~ 2006-01-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSpaceCowboyDBEditorToolDlg::ResetItemInfoANDRareInfo(void)
{
	// 아이템 데이타 로딩
	CAtumDBHelper::LoadItemInfo(m_pODBCStmt, &m_mapItemInfo, &m_mapItemInfoName);

	CAtumDBHelper::LoadRareItemInfo(m_pODBCStmt, &m_mapRareItemInfo, &m_mapRareItemInfoName);

	CAtumDBHelper::Load_MEX_MONSTER_INFO(m_pODBCStmt, &m_vecMexMonsterInfo);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			MEX_MONSTER_INFO *CSpaceCowboyDBEditorToolDlg::GetMEXMonsterInfo(int i_monsterUnitKind)
/// \brief		
/// \author		cmkwon
/// \date		2006-02-03 ~ 2006-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
MEX_MONSTER_INFO *CSpaceCowboyDBEditorToolDlg::GetMEXMonsterInfo(int i_monsterUnitKind)
{
	for(int i=0; i < m_vecMexMonsterInfo.size(); i++)
	{
		if(i_monsterUnitKind == m_vecMexMonsterInfo[i].MonsterUnitKind)
		{
			return &m_vecMexMonsterInfo[i];
		}
	}

	return NULL;
}

void CSpaceCowboyDBEditorToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	util::del(m_pMonsterDlg);
	util::del(m_pMonsterAttackDlg);
	util::del(m_pMonsterDropDlg);
	util::del(m_pQuestEditorDlg);
	util::del(m_pWeaponItemDlg);
	util::del(m_pEngineItemDlg);
	util::del(m_pMapEditorDlg);
	util::del(m_pStatisticDlg);
	util::del(m_pWriteLog);
	util::del(m_pODBCStmt);
}

void CSpaceCowboyDBEditorToolDlg::OnBtnLogManagement() 
{
	// TODO: Add your control notification handler code here
	if (m_pLogAdminDlg != NULL)
	{
		util::del(m_pLogAdminDlg);
	}

	if (m_pLogAdminDlg == NULL)
	{
		m_pLogAdminDlg = new CSCLogAdminDlg(this);
		m_pLogAdminDlg->Create(IDD_DIALOG_LOG_ADMIN_TOOL, this);
	}

	m_pLogAdminDlg->ShowWindow(SW_SHOW);
	
}

BOOL CSpaceCowboyDBEditorToolDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(m_pLogAdminDlg){			m_pLogAdminDlg->DestroyWindow();}		// 2007-11-08 by cmkwon, 모든 로그 검색 기능 SCDBTool로 이동 -

	return CDialog::DestroyWindow();
}
