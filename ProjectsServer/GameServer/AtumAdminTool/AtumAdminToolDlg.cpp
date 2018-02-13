// AtumAdminToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumAdminTool.h"
#include "AtumAdminToolDlg.h"
#include "AtumProtocol.h"
#include "AtumError.h"
#include "PWDDlg.h"
#include "AtumAdminTool.h"
#include "SetLanguageDlg.h"
#include "ExpViewerDlg.h"
#include "SCGuildAdminDlg.h"
#include "xortestdlg.h"			// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - ÇØ´õÆÄÀÏ Ãß°¡
#include "PetitionManagementDlg.h"	// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
#include "eventmonstermanagementdlg.h"		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 
#include "cashshopmanagementdlg.h"			// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
#include "wrankingmanagement.h"				// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
// start 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
#include "StaisticsMoneyDlg.h"
// end 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡

#include "ArenaCharSend.h" // 2012-06-16 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - Àü´Þ : Äû¸®¸¸µé±â



#define WM_TIMER_FOR_SEND_ALIVE_PACKET		101

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern GAME_SERVER_INFO_FOR_ADMIN g_arrGameServers[];

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolDlg dialog

CAtumAdminToolDlg::CAtumAdminToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAtumAdminToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAtumAdminToolDlg)
	m_UID = _T("");
	m_PWD = _T("");
	//m_ctl_strLanguageString = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pUserAdminDlg = NULL;
	m_pServerAdminDlg = NULL;
	m_pBadUserAdminDlg = NULL;
	m_pLogAdminDlg = NULL;
	m_pHappyHourEventAdminDlg = NULL;
	m_pExpViewer				= NULL;
	m_pGuildAdminDlg			= NULL;		// 2006-03-07 by cmkwon
	m_szServerName = "";
	m_pItemEventDlg	= NULL;
	m_pAdminPreSocket = NULL;
	m_pStrategyPointDlg = NULL;
	m_pXOREncodeDlg			= NULL;			// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - »ý¼ºÀÚ ÃÊ±âÈ­
	m_pPetitionDlg			= NULL;			// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
	m_pOutPostDlg			= NULL;
	m_pEventMonsterDlg		= NULL;			// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 

	m_pSCMonthlyArmorEventDlg = NULL;		// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó

	// start 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
	m_pStatisticsMoneyDlg	= NULL;
	// end 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡

	m_pCArenaCharSendDlg	= NULL;	// 2012-06-16 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - Àü´Þ : Äû¸®¸¸µé±â

	m_pRenewalStrategyPointDlg	= NULL;	// 2012-11-13 by jhseol, ÀüÀï ½Ã½ºÅÛ ¸®´º¾ó - °ÅÁ¡Àü Åø Ãß°¡

	// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - Admin Auto Notice Management
	m_pAdminAutoNoticeDlg	= NULL;

	m_pCashShopManagementDlg	= NULL;		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
	m_pWRankingManagementDlg	= NULL;		// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 


	util::zero(m_szManagerAccountName, SIZE_MAX_ACCOUNT_NAME);		// 2006-04-15 by cmkwon
	m_usManagerAccountType		= 0;								// 2006-04-15 by cmkwon

#ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
	// 2007-07-06 by cmkwon, SCAdminTool¿¡¼­ OnlyServerAdmin°ü·Ã ¼öÁ¤
	m_UID = _T(SCADMINTOOL_ONLY_SERVER_ADMIN_ACCOUNT_NAME);
	m_PWD = _T(SCADMINTOOL_ONLY_SERVER_ADMIN_PASSWORD);
#endif
}

CAtumAdminToolDlg::~CAtumAdminToolDlg()
{
	util::del(m_pAdminPreSocket);

	util::del(m_pUserAdminDlg);
	util::del(m_pServerAdminDlg);
	util::del(m_pBadUserAdminDlg);
	util::del(m_pLogAdminDlg);
	util::del(m_pHappyHourEventAdminDlg);
	util::del(m_pExpViewer);
	util::del(m_pGuildAdminDlg);
	util::del(m_pItemEventDlg);
	util::del(m_pStrategyPointDlg);
	util::del(m_pXOREncodeDlg);				// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - ¼Ò¸êÀÚ¿¡¼­ »èÁ¦
	util::del(m_pPetitionDlg);				// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
	util::del(m_pOutPostDlg);
	util::del(m_pEventMonsterDlg);			// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 

	util::del(m_pSCMonthlyArmorEventDlg);		// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó
	
	// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - Admin Auto Notice Management
	util::del(m_pAdminAutoNoticeDlg);

	util::del(m_pCashShopManagementDlg);		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
	util::del(m_pWRankingManagementDlg);		// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 

	// start 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
	util::del(m_pStatisticsMoneyDlg);
	// end 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
}

void CAtumAdminToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAtumAdminToolDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER, m_ComboSelectServer);
	DDX_Text(pDX, IDC_EDIT_UID, m_UID);
	DDX_Text(pDX, IDC_EDIT_PWD, m_PWD);
	//DDX_Text(pDX, IDC_EDIT_LANGUAGE, m_ctl_strLanguageString);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAtumAdminToolDlg, CDialog)
	//{{AFX_MSG_MAP(CAtumAdminToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_USER_TOOL, OnButtonUserTool)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SERVER, OnButtonServerTool)
	ON_BN_CLICKED(IDC_BUTTON_LOG_TOOL, OnButtonLogTool)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SCREEN_SHOT_VIEWER, OnButtonScreenShotViewer)
	ON_COMMAND(ID_MENU_TRAY_EXIT, OnMenuTrayExit)
	ON_COMMAND(ID_MENU_TRAY_OPEN, OnMenuTrayOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LOCALIZATION, OnBtnLocalization)
	ON_BN_CLICKED(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL, OnButtonHappyHourEventTool)
	ON_BN_CLICKED(IDC_BTN_EXP_VIEWER, OnBtnExpViewer)
	ON_BN_CLICKED(IDC_BTN_GUILD_TOOL, OnBtnGuildTool)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_EVENT, OnButtonItemEvent)
	ON_BN_CLICKED(IDC_BUTTON_STRATEGYPOINT, OnButtonStrategypoint)
	ON_BN_CLICKED(IDC_BTN_XOR_ENCODE, OnBtnXorEncode)
	ON_BN_CLICKED(IDC_BTN_PETITION_SYTEM, OnBtnPetitionSytem)
	ON_BN_CLICKED(IDC_BUTTON_OUTPOST, OnButtonOutpost)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_MONSTER, OnButtonEventMonster)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_NOTICE, OnButtonAutoNotice)
	ON_BN_CLICKED(IDC_BTN_CASHSHOP_MANAGEMENT, OnBtnCashshopManagement)
	ON_BN_CLICKED(IDC_BTN_WRANKING_MANAGEMENT, OnBtnWrankingManagement)
	ON_BN_CLICKED(IDC_BTN_INFINITY, OnBtnInfinity)
	ON_BN_CLICKED(IDC_BTN_STATISTICS_MONEY, OnBtnStatisticsMoney)
	ON_BN_CLICKED(IDC_BTN_ARENA_CHAR_SEND, OnBtnArenaCharSend)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_MONTHLY_ARMOR_EVENT, OnButtonMonthlyArmorEvent)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PRE_PACKET_NOTIFY, OnSocketNotifyPre)
	ON_MESSAGE(WM_PRE_ASYNC_EVENT, OnAsyncSocketMessage)
//	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumAdminToolDlg message handlers

BOOL CAtumAdminToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//if(!IS_VALID_LANGUAGE_TYPE(this->m_nLanguageType))
	//{
	//	this->m_nLanguageType = LANGUAGE_TYPE_DEFAULT;
	//}
	//if(!m_Localization.LoadConfiguration((LPSTR)(LPCSTR)m_strLocalizationDirectoryPath, m_nLanguageType))
	//{
	//	MessageBox("Load Localization files fail !!\n\n Reset Localization files directory path.");
	//	this->m_nLanguageType = LANGUAGE_TYPE_DEFAULT;
	//}
	//SetLanguageString(this->m_nLanguageType);

#ifdef _ATUM_ONLY_SERVER_ADMIN_TOOL
	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(FALSE);
// 2005-10-11 by cmkwon, ·Î±× °Ë»ö±îÁö °¡´ÉÇÏµµ·Ï ¼öÁ¤
//	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_UID)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCREEN_SHOT_VIEWER)->EnableWindow(FALSE);	
	GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OUTPOST)->EnableWindow(FALSE);

	// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó
	GetDlgItem(IDC_BUTTON_MONTHLY_ARMOR_EVENT)->EnableWindow(FALSE);
	// End. 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó

#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifdef


	int nComboSelIndex = 0;
	for (int i = 0; g_arrGameServers[i].ServerName !=NULL; i++)
	{
		m_ComboSelectServer.AddString(g_arrGameServers[i].ServerName);
		if (m_szServerName == g_arrGameServers[i].ServerName)
		{
			nComboSelIndex = i;
		}
	}

	m_ComboSelectServer.SetCurSel(nComboSelIndex);

	EnableToolControls(FALSE);

	SetTimer(0, 100, NULL);

//	if(!m_TrayIcon.Create(this, WM_ICON_NOTIFY, _T("SpaceCowboy Admin Tool"), NULL, IDR_MENU_TRAY))
//	{
//		return -1;
//	}
//	m_TrayIcon.SetIcon(IDR_MAINFRAME);

	this->SetTimer(WM_TIMER_FOR_SEND_ALIVE_PACKET, 60000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAtumAdminToolDlg::OnPaint() 
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
HCURSOR CAtumAdminToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAtumAdminToolDlg::OnButtonUserTool() 
{
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif

	// TODO: Add your control notification handler code here
	if (m_pUserAdminDlg != NULL)
	{
		util::del(m_pUserAdminDlg);
	}	
	if (m_pUserAdminDlg == NULL)
	{
		m_pUserAdminDlg = new CSCUserAdminDlg();
		m_pUserAdminDlg->Create(IDD_SCUSERADMIN_DIALOG, this);
	}

	m_pUserAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonServerTool() 
{

#ifndef _ATUM_ONLY_SERVER_ADMIN_TOOL
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif// _ATUM_ADMIN_RELEASE_end_ifdef
#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifndef

	// TODO: Add your control notification handler code here
	if (m_pServerAdminDlg != NULL)
	{
		util::del(m_pServerAdminDlg);
	}

	if (m_pServerAdminDlg == NULL)
	{
		m_pServerAdminDlg = new CSCServerAdminDlg;
		m_pServerAdminDlg->Create(IDD_DIALOG_SERVER_ADMIN_TOOL, this);
	}

	m_pServerAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonLogTool() 
{
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif
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

void CAtumAdminToolDlg::OnButtonBadUserTool() 
{
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif
	// TODO: Add your control notification handler code here
	if (m_pBadUserAdminDlg != NULL)
	{
		util::del(m_pBadUserAdminDlg);
	}

	if (m_pBadUserAdminDlg == NULL)
	{
		m_pBadUserAdminDlg = new CSCBadUserAdminDlg;
		m_pBadUserAdminDlg->Create(IDD_DIALOG_BAD_USER_ADMIN_TOOL, this);
	}

	m_pBadUserAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if (m_UID == "")
	{
		MessageBox(STRERR_S_SCADMINTOOL_0004);
		GetDlgItem(IDC_EDIT_UID)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_UID))->SetSel(0, -1);
		return;
	}

	if (m_PWD == "")
	{
		MessageBox(STRERR_S_SCADMINTOOL_0002);
		GetDlgItem(IDC_EDIT_PWD)->SetFocus();
		((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetSel(0, -1);
		return;
	}

	// 2017-01-02 panoskj, todo : remove this quick fix
	//m_pServerInfo4Admin = new GAME_SERVER_INFO_FOR_ADMIN;

	if (m_ComboSelectServer.GetCurSel() != -1)
	{
		m_pServerInfo4Admin = &g_arrGameServers[m_ComboSelectServer.GetCurSel()];
		m_ComboSelectServer.GetLBText(m_ComboSelectServer.GetCurSel(), m_szServerName);
	}
	else
	{
		m_pServerInfo4Admin = NULL;
	}

	//strcpy_s(m_pServerInfo4Admin->ServerIP, "10.10.20.6");
	//strcpy_s(m_pServerInfo4Admin->DBIP, "127.0.0.1");
	//m_pServerInfo4Admin->DBPort = 1433;
	//m_pServerInfo4Admin->DBName = "atum2_db_1";
	//strcpy_s(m_pServerInfo4Admin->DBUID, "atum");
	//strcpy_s(m_pServerInfo4Admin->DBPWD, "DB:atum");

	if (FALSE == ConnectServer())
	{
		return;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-15 by cmkwon
	util::strncpy(m_szManagerAccountName, m_UID, SIZE_MAX_ACCOUNT_NAME);
	m_usManagerAccountType			= 0;

	((CAtumAdminToolApp*)AfxGetApp())->WriteProfile();
}

BOOL CAtumAdminToolDlg::ConnectServer()
{
	if(m_pAdminPreSocket)
	{
		MessageBox("Aready Connecting !!");
		return FALSE;
	}
	// Initialize winsock 2.0
	CWinSocket::SocketInit();

	// Make socket instance & connect
	m_pAdminPreSocket = new CSCAdminPreWinSocket("CAtumAdminToolDlg's PreServer Socket", this, GetSafeHwnd());

	if (!m_pAdminPreSocket->Connect(m_pServerInfo4Admin->ServerIP, PRE_SERVER_PORT))
	{
		int err = GetLastError();
		MessageBox(STRERR_S_SCADMINTOOL_0005);
		util::del(m_pAdminPreSocket);
		EndDialog(-1);
		return FALSE;
	}

	return TRUE;
}

void CAtumAdminToolDlg::EnableToolControls(BOOL i_bEnable)
{
	GetDlgItem(IDC_STATIC_TOOLS)->EnableWindow(i_bEnable);
// 2007-07-06 by cmkwon, Áß±¹Àº ¿î¿µÀÚ´Â ServerAdmin¸¸ Á¢±Ù °¡´É - ¾Æ·¡¿Í °°ÀÌ ¼öÁ¤ÇÔ
//	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//#ifndef _ATUM_ONLY_SERVER_ADMIN_TOOL
//	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
//// 2005-10-11 by cmkwon, ·Î±× °Ë»ö±îÁö °¡´ÉÇÏµµ·Ï ¼öÁ¤
////	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
//#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifndef

// 2007-11-21 by cmkwon, Áß±¹µµ ³ª¸ÓÁö ³ª¶óµé°ú GM ±ÇÇÑÀ» µ¿ÀÏÇÏ°Ô Ã³¸®ÇÑ´Ù. - 
//#if defined(_CHN_S1)		// 2007-07-06 by cmkwon, Áß±¹Àº ¿î¿µÀÚ´Â ServerAdmin¸¸ Á¢±Ù °¡´É
//	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
//	if(FALSE == i_bEnable
//		|| this->IsManagerAdministrator())
//	{
//		GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(i_bEnable);
//		GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(i_bEnable);
//	}
//#elif !defined(_ATUM_ONLY_SERVER_ADMIN_TOOL)
//	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
//	GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(i_bEnable);
//	GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(i_bEnable);
//#endif// _ATUM_ONLY_SERVER_ADMIN_TOOL_end_ifndef
#if !defined(_ATUM_ONLY_SERVER_ADMIN_TOOL)	
	GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow(i_bEnable);
// 2012-01-07 by khkim, º£Æ®³² GM ±ÇÇÑ¼öÁ¤
#ifdef S_MANAGER_ADMIN_HSSON
	if(RACE_GAMEMASTER == m_usManagerAccountType)
	{
		GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
		GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
	}
#endif
// end 2012-01-07 by khkim, º£Æ®³² GM ±ÇÇÑ¼öÁ¤
	GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_COMBO_SERVER)->EnableWindow(!i_bEnable);
	GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BTN_PETITION_SYTEM)->EnableWindow(i_bEnable);		// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
	GetDlgItem(IDC_BUTTON_OUTPOST)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_EVENT_MONSTER)->EnableWindow(i_bEnable);		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 
	GetDlgItem(IDC_BUTTON_AUTO_NOTICE)->EnableWindow(i_bEnable);		// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - Admin Auto Notice Management
	GetDlgItem(IDC_BTN_CASHSHOP_MANAGEMENT)->EnableWindow(i_bEnable);	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
	GetDlgItem(IDC_BTN_WRANKING_MANAGEMENT)->EnableWindow(i_bEnable);	// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
	// start 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
	GetDlgItem(IDC_BTN_STATISTICS_MONEY)->EnableWindow(i_bEnable);
	// end 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡

	// 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó
#ifdef S_MONTHL_ARMOR_EVENT_JHSEOL_BCKIM		// ÀÌº¥Æ® ¸®·Îµå
	GetDlgItem(IDC_BUTTON_MONTHLY_ARMOR_EVENT)->EnableWindow(i_bEnable);
#else
	GetDlgItem(IDC_BUTTON_MONTHLY_ARMOR_EVENT)->EnableWindow(FALSE);
#endif
	// End. 2013-04-18 by jhseol,bckim ÀÌ´ÞÀÇ ¾Æ¸Ó

#endif // END - #if !defined(_ATUM_ONLY_SERVER_ADMIN_TOOL)	


	GetDlgItem(IDC_BUTTON_SERVER)->EnableWindow(i_bEnable);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(i_bEnable);

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(!i_bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_UID))->SetReadOnly(i_bEnable);
	((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetReadOnly(i_bEnable);	
}


/******************************************************************************
**
**	°èÁ¤ Å¸ÀÔº° ¸Þ´º ±ÇÇÑ ºÎ¿©.
**
**	Create Info : 2010. 09. 06. by hsLee.
**
*******************************************************************************/
void CAtumAdminToolDlg :: EnableToolControls ( USHORT usAccountType )
{

	EnableToolControls ( TRUE );

	switch ( usAccountType )
	{
		case RACE_MONITOR :

			GetDlgItem(IDC_BUTTON_USER_TOOL)->EnableWindow( TRUE );
			GetDlgItem(IDC_BTN_GUILD_TOOL)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_LOG_TOOL)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_HAPPY_HOUR_EVENT_TOOL)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_ITEM_EVENT)->EnableWindow( FALSE );
			GetDlgItem(IDC_BTN_PETITION_SYTEM)->EnableWindow( FALSE );		// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
			GetDlgItem(IDC_BUTTON_STRATEGYPOINT)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_OUTPOST)->EnableWindow( FALSE );
			GetDlgItem(IDC_BUTTON_EVENT_MONSTER)->EnableWindow( FALSE );		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 
			GetDlgItem(IDC_COMBO_SERVER)->EnableWindow ( FALSE );
			GetDlgItem(IDC_BUTTON_AUTO_NOTICE)->EnableWindow( FALSE );		// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - Admin Auto Notice Management
			GetDlgItem(IDC_BTN_CASHSHOP_MANAGEMENT)->EnableWindow( FALSE );	// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
			GetDlgItem(IDC_BTN_WRANKING_MANAGEMENT)->EnableWindow( FALSE );	// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
			GetDlgItem(IDC_BUTTON_SERVER)->EnableWindow( FALSE );
			// start 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
			GetDlgItem(IDC_BTN_STATISTICS_MONEY)->EnableWindow(FALSE);
			// end 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡

			break;

		default :
			break;
	}
	
	

}

LONG CAtumAdminToolDlg::OnSocketNotifyPre(WPARAM wParam, LPARAM lParam)
{
	CSCAdminWinSocket *pSCAdminWinSocket = (CSCAdminWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CWinSocket::WS_ERROR:
		{
		}
		break;
	case CWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				// ¿¬°á ¼º°ø
				INIT_MSG_WITH_BUFFER(MSG_PA_ADMIN_CONNECT, T_PA_ADMIN_CONNECT, msgConnect, msgConnectBuf);
				util::strncpy(msgConnect->UID, (LPCSTR)m_UID, SIZE_MAX_ACCOUNT_NAME);
				util::strncpy(msgConnect->PWD, (LPCSTR)m_PWD, SIZE_MAX_PASSWORD);
				m_pAdminPreSocket->Write(msgConnectBuf, MSG_SIZE(MSG_PA_ADMIN_CONNECT));		// 2011-07-21 by hskim, ÀÎÁõ ¼­¹ö ±¸Çö - ±âÁ¸ ¼­¹ö¿Í È£È¯ ¾ÈµÇµµ·Ï ±¸Á¶Ã¼ Å©±â ¹Ù²Þ (Ãß°¡ ±âÁ¸ ¹ö±× ¼öÁ¤)
			}
			else
			{
				// ¿¬°á ½ÇÆÐ
				MessageBox(STRERR_S_SCADMINTOOL_0006);
			}
		}
		break;
	case CWinSocket::WS_RECEIVED:
		{
			MessageType_t	msgType;

			char			*pPacket = NULL;
			int				len;
			pSCAdminWinSocket->Read(&pPacket, len);

			if (pPacket)
			{
				msgType = *(MessageType_t*)(pPacket);

				switch(msgType)
				{
				case T_PA_ADMIN_CONNECT_OK:
					{
						MSG_PA_ADMIN_CONNECT_OK *msgConnectOK
							= (MSG_PA_ADMIN_CONNECT_OK*)(pPacket+SIZE_FIELD_TYPE_HEADER);

// 2006-04-15 by cmkwon
//						if (!msgConnectOK->AuthOK)
						if (0 == msgConnectOK->AccountType0)
						{
							// ÀÎÁõ ½ÇÆÐ
							MessageBox("Fail PreServer1 certification!!");
							EndDialog(-1);
						}
						else
						{// ÀÎÁõ ¼º°ø
							
							m_usManagerAccountType	= msgConnectOK->AccountType0;	// 2006-04-15 by cmkwon, Á¢¼ÓÀÚ ±ÇÇÑ ¼³Á¤

							//EnableToolControls(TRUE);

							EnableToolControls ( m_usManagerAccountType );
							// 2012-06-16 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - Àü´Þ : ¸¶»ó ³»ºÎ°¡ ¾Æ´Ï¸é »ç¿ë ºÒ°¡
#ifdef S_ARENA_EX_1ST_JHSEOL
							//if(msgConnectOK->IsMasang)
							//{
							// todo : review what this does
							GetDlgItem(IDC_BTN_ARENA_CHAR_SEND)->ShowWindow(TRUE);	// ±×³É ¼öÁ¤ EnableWindow() -> ShowWindow()
							//}
#endif
							// end 2012-06-16 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - Àü´Þ : ¸¶»ó ³»ºÎ°¡ ¾Æ´Ï¸é »ç¿ë ºÒ°¡
						}
					}
					break;
				case T_ERROR:
					{
						MSG_ERROR *pRecvMsg;
						pRecvMsg = (MSG_ERROR*)(pPacket + SIZE_FIELD_TYPE_HEADER);

						char buf[128];
						Err_t error = pRecvMsg->ErrorCode;

						//DBGOUT("ERROR %s(%#04X) received from %s[%s]\n", GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode, "ST_PRE_SERVER", m_pUpdateWinsocket->m_szPeerIP);

						switch (error)
						{
						case ERR_PROTOCOL_INVALID_PROTOCOL_TYPE:
							{
								MessageBox(STRERR_S_SCADMINTOOL_0008);
								OnCancel();
							}
							break;
						default:
							{
								sprintf(buf, "ERROR: %s(%#04X)", GetErrorString(pRecvMsg->ErrorCode), pRecvMsg->ErrorCode);
								MessageBox(buf);
								OnCancel();
							}
							break;
						}
					}
				default:
					{
					}
					break;
				}
			}

			util::del(pPacket);
		}
		break;
	case CWinSocket::WS_CLOSED:
		{
			// 2008-01-17 by cmkwon, T_A: PreServer¿Í ¿¬°á Á¾·á½Ã Á¾·á µÇµµ·Ï Ã³¸®
			char buf[1024];
			sprintf(buf, "Socket closed by PreServer !!");
			MessageBox(buf);
			OnCancel();
		}
		break;

	}	// end of switch

	return 0;
}

LONG CAtumAdminToolDlg::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	m_pAdminPreSocket->OnAsyncEvent(lParam);

	return 0;
}


void CAtumAdminToolDlg::OnButtonDisconnect() 
{
	// TODO: Add your control notification handler code here
	m_pAdminPreSocket->CloseSocket();
	util::del(m_pAdminPreSocket);
	util::del(m_pUserAdminDlg);
	EnableToolControls(FALSE);
}

void CAtumAdminToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		KillTimer(0);

		if (m_UID != "")
		{
			GetDlgItem(IDC_EDIT_PWD)->SetFocus();
			((CEdit*)GetDlgItem(IDC_EDIT_PWD))->SetSel(0, -1);
		}
	}
	else if(nIDEvent == WM_TIMER_FOR_SEND_ALIVE_PACKET)
	{
		if(m_pAdminPreSocket
			&& m_pAdminPreSocket->IsConnected())
		{
			// 2005-01-02 by cmkwon, ÀÓ½Ã ¹æÆíÀ¸·Î ´Ù¸¥°ÍÀ» º¸³½´Ù
			m_pAdminPreSocket->WriteMessageType(T_PM_CONNECT_ALIVE);
		}

		if(m_pUserAdminDlg)
		{
			m_pUserAdminDlg->OnTimerForSendAlivePacket();
		}

		if(m_pServerAdminDlg)
		{
			m_pServerAdminDlg->OnTimerForSendAlivePacket();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CAtumAdminToolDlg::OnButtonScreenShotViewer() 
{
	// TODO: Add your control notification handler code here
	char buff[40];
	GetCurrentDirectory(40, buff);
	UINT ret = WinExec("Vista.exe", SW_SHOWNORMAL);
}


//LONG CAtumAdminToolDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
//{
//	return m_TrayIcon.OnTrayNotification(wParam, lParam);
//}

BOOL CAtumAdminToolDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	m_TrayIcon.RemoveIcon();

	if(m_pAdminPreSocket){		m_pAdminPreSocket->CloseSocket();}

	if(m_pUserAdminDlg){		m_pUserAdminDlg->DestroyWindow();}
	if(m_pServerAdminDlg){		m_pServerAdminDlg->DestroyWindow();}
	if(m_pBadUserAdminDlg){		m_pBadUserAdminDlg->DestroyWindow();}
	if(m_pLogAdminDlg){			m_pLogAdminDlg->DestroyWindow();}
	if(m_pHappyHourEventAdminDlg){	m_pHappyHourEventAdminDlg->DestroyWindow();}
	if(m_pExpViewer){				m_pExpViewer->DestroyWindow();}
	if(m_pItemEventDlg){		m_pItemEventDlg->DestroyWindow();}
	if(m_pStrategyPointDlg){	m_pStrategyPointDlg->DestroyWindow();}
	if(m_pXOREncodeDlg){			m_pXOREncodeDlg->DestroyWindow();}		// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - CAtumAdminToolDlg::DestroyWindow() ÇÔ¼ö¿¡¼­ Á¾·á Ã³¸®
	if(m_pPetitionDlg){				m_pPetitionDlg->DestroyWindow();}		// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
	if(m_pOutPostDlg){			m_pOutPostDlg->DestroyWindow();}
	if(m_pEventMonsterDlg){		m_pEventMonsterDlg->DestroyWindow();}		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - 

	if(m_pRenewalStrategyPointDlg){m_pRenewalStrategyPointDlg->DestroyWindow();}	// 2012-11-13 by jhseol, ÀüÀï ½Ã½ºÅÛ ¸®´º¾ó - °ÅÁ¡Àü Åø Ãß°¡


	// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - Admin Auto Notice Management
	if(m_pAdminAutoNoticeDlg){	m_pAdminAutoNoticeDlg->DestroyWindow();}

	if(m_pCashShopManagementDlg){	m_pCashShopManagementDlg->DestroyWindow();}		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
	if(m_pWRankingManagementDlg){	m_pWRankingManagementDlg->DestroyWindow();}		// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
	
	return CDialog::DestroyWindow();
}

void CAtumAdminToolDlg::OnMenuTrayExit() 
{
	// TODO: Add your command handler code here
	OnOK();
}

void CAtumAdminToolDlg::OnMenuTrayOpen() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_SHOW);
}


void CAtumAdminToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
//	if(nID == SC_MINIMIZE)
//	{
//		ShowWindow(SW_HIDE);
//	}
//	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAtumAdminToolDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CAtumAdminToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAtumAdminToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(WM_TIMER_FOR_SEND_ALIVE_PACKET);
}

void CAtumAdminToolDlg::OnBtnLocalization() 
{
	// TODO: Add your control notification handler code here

	//CSetLanguageDlg dlg(this->m_strLocalizationDirectoryPath, this->m_nLanguageType);
	//if(IDCANCEL == dlg.DoModal())
	//{
	//	return;
	//}
	
	//m_Localization.ResetLocalization();
	//
	//if(FALSE == m_Localization.LoadConfiguration((LPSTR)(LPCSTR)dlg.m_ctl_strLocalPath, dlg.m_nLanguageType))
	//{
	//	MessageBox("Setting Localization files directory fail !!\n\nRetry.");
	//	return;
	//}
	//this->m_strLocalizationDirectoryPath	= dlg.m_ctl_strLocalPath;
	//this->m_nLanguageType					= dlg.m_nLanguageType;
	/*((CAtumAdminToolApp*)AfxGetApp())->WriteProfile();*/
		
	//this->SetLanguageString(this->m_nLanguageType);
	//UpdateData(FALSE);
}

//void CAtumAdminToolDlg::SetLanguageString(int i_nLanguageType)
//{
//	// 2008-04-25 by cmkwon, Áö¿ø ¾ð¾î¿¡ µ¶ÀÏ¾î Ãß°¡ - GET_LANGUAGE_TYPE_STRING() ÇÔ¼ö·Î Ã³¸®
//	m_ctl_strLanguageString = GET_LANGUAGE_TYPE_STRING(i_nLanguageType);
//
//	UpdateData(FALSE);
//}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAdminToolDlg::IsManagerAdministrator(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-15 ~ 2006-04-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::IsManagerAdministrator(void)
{
#ifdef S_MANAGER_ADMIN_HSSON
	return COMPARE_RACE(m_usManagerAccountType, RACE_OPERATION | RACE_GAMEMASTER);
#else 
	return COMPARE_RACE(m_usManagerAccountType, RACE_OPERATION);
#endif	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAdminToolDlg::SendMsgToPreServer(BYTE *i_pbyData, int i_nDataLen)
/// \brief		// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::SendMsgToPreServer(BYTE *i_pbyData, int i_nDataLen)
{
	if(NULL == m_pAdminPreSocket
		|| FALSE == m_pAdminPreSocket->IsConnected())
	{
		return FALSE;
	}

	return m_pAdminPreSocket->Write(i_pbyData, i_nDataLen);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtumAdminToolDlg::SendMsgTypeToPreServer(MessageType_t i_nMsgTy)
/// \brief		// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::SendMsgTypeToPreServer(MessageType_t i_nMsgTy)
{
	if(NULL == m_pAdminPreSocket
		|| FALSE == m_pAdminPreSocket->IsConnected())
	{
		return FALSE;
	}
	
	return m_pAdminPreSocket->WriteMessageType(i_nMsgTy);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
/// \author		cmkwon
/// \date		2009-02-25 ~ 2009-02-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtumAdminToolDlg::IsConnectedToPreServer(void)
{
	if(NULL == m_pAdminPreSocket)
	{
		return FALSE;
	}
	
	return m_pAdminPreSocket->IsConnected();
}

void CAtumAdminToolDlg::OnButtonHappyHourEventTool() 
{
	// TODO: Add your control notification handler code here
	
#ifdef _ATUM_ADMIN_RELEASE
	CPWDDlg dlg;
	if(IDOK != dlg.DoModal())
	{
		return;
	}
	else if(dlg.m_ctlStrPassword.IsEmpty())
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0002);
		return;
	}
	else if(0 != m_PWD.Compare(dlg.m_ctlStrPassword))
	{
		AfxMessageBox(STRERR_S_SCADMINTOOL_0003);
		return;
	}
#endif
	// TODO: Add your control notification handler code here
	if (m_pHappyHourEventAdminDlg != NULL)
	{
		util::del(m_pHappyHourEventAdminDlg);
	}

	if (m_pHappyHourEventAdminDlg == NULL)
	{
		m_pHappyHourEventAdminDlg = new CSCHappyHourEventAdminDlg(this);
		m_pHappyHourEventAdminDlg->Create(IDD_DIALOG_HAPPY_HOUR_EVENT_ADMIN, this);
	}

	m_pHappyHourEventAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnBtnExpViewer() 
{
	// TODO: Add your control notification handler code here
	if(NULL == m_pExpViewer)
	{
		m_pExpViewer = new CExpViewerDlg;
		m_pExpViewer->Create(IDD_DLG_EXP_VIEWER, this);
	}

	m_pExpViewer->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnBtnGuildTool() 
{
	// TODO: Add your control notification handler code here
	if (m_pGuildAdminDlg != NULL)
	{
		util::del(m_pGuildAdminDlg);
	}

	if (m_pGuildAdminDlg == NULL)
	{
		m_pGuildAdminDlg = new CSCGuildAdminDlg(this);
		m_pGuildAdminDlg->Create(IDD_DLG_GUILD_ADMIN, this);
	}

	m_pGuildAdminDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonItemEvent() 
{
	// TODO: Add your control notification handler code here
	if (m_pItemEventDlg != NULL)
	{
		util::del(m_pItemEventDlg);
	}

	if (m_pItemEventDlg == NULL)
	{
		m_pItemEventDlg = new CSCItemEventDlg(this);
		m_pItemEventDlg->Create(IDD_DIALOG_ITEM_EVENT_ADMIN, this);
	}

	m_pItemEventDlg->ShowWindow(SW_SHOW);
}

void CAtumAdminToolDlg::OnButtonStrategypoint() 
{
	// TODO: Add your control notification handler code here

// 2009-02-04 by cmkwon, AdminToolÀÇ GM ±ÇÇÑ ¼öÁ¤ - Àü·«Æ÷ÀÎÆ®Àü Á¤º¸ °Ë»öÀº °¡´É, ¼öÁ¤ ºÒ°¡
// 	// 2007-10-02 by cmkwon, SCAdminTool ¼öÁ¤ ±ÇÇÑ Ã³¸® - Àü·«Æ÷ÀÎÆ® ¼ÒÈ¯ ¼³Á¤ Ã³¸®
// 	if(FALSE == this->IsManagerAdministrator())
// 	{
// 		AfxMessageBox("You are not have permission !!");
// 		return;
// 	}

	// 2012-11-13 by jhseol, ÀüÀï ½Ã½ºÅÛ ¸®´º¾ó - °ÅÁ¡Àü Åø Ãß°¡
#ifdef S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL
	if (m_pRenewalStrategyPointDlg != NULL)
	{
		util::del(m_pRenewalStrategyPointDlg);
	}
	
	if (m_pRenewalStrategyPointDlg == NULL)
	{
		m_pRenewalStrategyPointDlg = new CRenewalStrategyPointAdminDlg(this, m_pAdminPreSocket);
		m_pRenewalStrategyPointDlg->Create(IDD_DIALOG_RENEWAL_STRATEGYPOINT, this);
	}
	
	m_pRenewalStrategyPointDlg->ShowWindow(SW_SHOW);	
#else
	if (m_pStrategyPointDlg != NULL)
	{
		util::del(m_pStrategyPointDlg);
	}

	if (m_pStrategyPointDlg == NULL)
	{
		m_pStrategyPointDlg = new CSCStrategyPointAdminDlg(this);
		m_pStrategyPointDlg->Create(IDD_DIALOG_STRATEGYPOINT_ADMIN_TOOL, this);
	}

	m_pStrategyPointDlg->ShowWindow(SW_SHOW);	
#endif	// #ifdef S_WAR_SYSTEM_RENEWAL_STRATEGYPOINT_JHSEOL
	// 2012-11-13 by jhseol, ÀüÀï ½Ã½ºÅÛ ¸®´º¾ó - °ÅÁ¡Àü Åø Ãß°¡
}

void CAtumAdminToolDlg::OnBtnXorEncode() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-24 by cmkwon, ¼­¹ö Á¤º¸ ¾ÏÈ£È­ - CAtumAdminToolDlg::OnBtnXorEncode() ¿¡¼­ À©µµ¿ì »ý¼º ¹× º¸¿©ÁÖ±â
	if(NULL == m_pXOREncodeDlg)
	{
		m_pXOREncodeDlg = new CXORTestDlg;
		m_pXOREncodeDlg->Create(IDD_DLG_XOR_TEST, this);
	}

	m_pXOREncodeDlg->ShowWindow(SW_SHOW);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumAdminToolDlg::OnBtnPetitionSytem() 
/// \brief		// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - CAtumAdminToolDlg::OnBtnPetitionSytem() Ãß°¡
/// \author		cmkwon
/// \date		2007-11-20 ~ 2007-11-20
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnBtnPetitionSytem() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2007-11-19 by cmkwon, ÁøÁ¤½Ã½ºÅÛ ¾÷µ¥ÀÌÆ® - 
	if(NULL == m_pPetitionDlg)
	{
		m_pPetitionDlg = new CPetitionManagementDlg;
		m_pPetitionDlg->Create(IDD_DLG_PETITION_MANAGEMENT, this);
	}

	m_pPetitionDlg->ShowWindow(SW_SHOW);	
}

void CAtumAdminToolDlg::OnButtonOutpost() 
{
	// TODO: Add your control notification handler code here
// 2009-02-04 by cmkwon, AdminToolÀÇ GM ±ÇÇÑ ¼öÁ¤ - ÀüÁø±âÁöÀü Á¤º¸ °Ë»öÀº °¡´É, ¼öÁ¤ ºÒ°¡
// 	if(FALSE == this->IsManagerAdministrator())
// 	{
// 		AfxMessageBox("You are not have permission !!");
// 		return;
// 	}

	if (m_pOutPostDlg != NULL)
	{
		util::del(m_pOutPostDlg);
	}

	if (m_pOutPostDlg == NULL)
	{
		m_pOutPostDlg = new CSCOutPostDlg(this);
		m_pOutPostDlg->Create(IDD_DLG_OUTPOST, this);
	}

	m_pOutPostDlg->ShowWindow(SW_SHOW);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumAdminToolDlg::OnButtonEventMonster()
/// \brief		// 2008-04-16 by cmkwon, ¸ó½ºÅÍ »ç¸Á ½Ã ¸ó½ºÅÍ ¼ÒÈ¯ ÀÌº¥Æ® ½Ã½ºÅÛ ±¸Çö - CAtumAdminToolDlg::OnButtonEventMonster() Ãß°¡
/// \author		cmkwon
/// \date		2008-04-17 ~ 2008-04-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnButtonEventMonster() 
{
	// TODO: Add your control notification handler code here
	
	if (m_pEventMonsterDlg != NULL)
	{
		util::del(m_pEventMonsterDlg);
	}

	if (m_pEventMonsterDlg == NULL)
	{
		m_pEventMonsterDlg = new CEventMonsterManagementDlg(this);
		m_pEventMonsterDlg->Create(IDD_DLG_EVENT_MONSTER_MANAGEMENT, this);
	}

	m_pEventMonsterDlg->ShowWindow(SW_SHOW);	
}



// 2009-01-14 by cmkwon, ¿î¿µÀÚ ÀÚµ¿ °øÁö ½Ã½ºÅÛ ±¸Çö - Admin Auto Notice Management
void CAtumAdminToolDlg::OnButtonAutoNotice() 
{
	if (m_pAdminAutoNoticeDlg != NULL)
	{
		util::del(m_pAdminAutoNoticeDlg);
	}
	
	if (m_pAdminAutoNoticeDlg == NULL)
	{
		m_pAdminAutoNoticeDlg = new CAdminAutoNoticeDlg(this);
		if(FALSE == m_pAdminAutoNoticeDlg->Create(IDD_DIALOG_ADMIN_AUTO_NOTICE, this))
		{
			return;
		}
	}
	
	m_pAdminAutoNoticeDlg->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumAdminToolDlg::OnBtnCashshopManagement()
/// \brief		// 2009-01-28 by cmkwon, Ä³½¬˜?¼öÁ¤(ÃßÃµÅÇ,½Å»óÇ° Ãß°¡) - 
/// \author		cmkwon
/// \date		2009-01-29 ~ 2009-01-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnBtnCashshopManagement()
{
	if (m_pCashShopManagementDlg != NULL)
	{
		util::del(m_pCashShopManagementDlg);
	}
	
	if (m_pCashShopManagementDlg == NULL)
	{
		m_pCashShopManagementDlg = new CCashShopManagementDlg(this);
		if(FALSE == m_pCashShopManagementDlg->Create(IDD_DLG_CASHSHOP_MANAGEMENT, this))
		{
			return;
		}
	}
	
	m_pCashShopManagementDlg->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 ¿ùµå·©Å·½Ã½ºÅÛ ±¸Çö - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumAdminToolDlg::OnBtnWrankingManagement() 
{
	// TODO: Add your control notification handler code here
	if (m_pWRankingManagementDlg != NULL)
	{
		util::del(m_pWRankingManagementDlg);
	}
	
	if (m_pWRankingManagementDlg == NULL)
	{
		m_pWRankingManagementDlg = new CWRankingManagement(this);
		if(FALSE == m_pWRankingManagementDlg->Create(IDD_DLG_WRK_MANAGEMENT, this))
		{
			return;
		}
	}
	
	m_pWRankingManagementDlg->ShowWindow(SW_SHOW);	
}

void CAtumAdminToolDlg::OnBtnInfinity()
{// 2009-09-09 ~ 2010 by dhjin, ÀÎÇÇ´ÏÆ¼ - ±Í¼Ó Á¤º¸ ¸®¼Â
	// TODO: Add your control notification handler code here
	if(FALSE == this->IsManagerAdministrator()) {
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}
	
	// TODO: Add your control notification handler code here
	if(IDCANCEL == AfxMessageBox("If you want to update, you must restart FieldServer", MB_OKCANCEL)) {
		return;
	}

	if(IDCANCEL == AfxMessageBox("Do you want to initialize all Infinity belonging information of the server?", MB_OKCANCEL)) {
		return;
	}

	CODBCStatement * m_pODBCStmt = new CODBCStatement;
	if (!m_pODBCStmt->Init(this->m_pServerInfo4Admin->DBIP, this->m_pServerInfo4Admin->DBPort, this->m_pServerInfo4Admin->DBName, this->m_pServerInfo4Admin->DBUID, this->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())) {
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return;
	}
	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_090909_0531));
	if (!bRet) {
		m_pODBCStmt->FreeStatement();		// cleanup
		
		AfxMessageBox("Infinity Initialize error !!");
		return;
	}
	m_pODBCStmt->FreeStatement();	// cleanup	
	util::del(m_pODBCStmt);

	AfxMessageBox("Success !!");
}

// start 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡
void CAtumAdminToolDlg::OnBtnStatisticsMoney() 
{
	// TODO: Add your control notification handler code here
	if (m_pStatisticsMoneyDlg != NULL)
	{
		util::del(m_pStatisticsMoneyDlg);
	}	
	if (m_pStatisticsMoneyDlg == NULL)
	{
		m_pStatisticsMoneyDlg = new CStaisticsMoneyDlg(this);
		m_pStatisticsMoneyDlg->Create(IDD_DIALOG_STATISTICS_MONEY, this);
	}
	
	m_pStatisticsMoneyDlg->ShowWindow(SW_SHOW);

}
// end 2012-02-27 by jhseol, È­ÆóÅë°èÅø ±â´É Ãß°¡

// 2012-06-16 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - Àü´Þ : Äû¸®¸¸µé±â
void CAtumAdminToolDlg::OnBtnArenaCharSend() 
{
	// TODO: Add your control notification handler code here
	if (m_pCArenaCharSendDlg != NULL)
	{
		util::del(m_pCArenaCharSendDlg);
	}	
	if (m_pCArenaCharSendDlg == NULL)
	{
		m_pCArenaCharSendDlg = new CArenaCharSend(this);
		m_pCArenaCharSendDlg->Create(IDD_DIALOG_ARENA_CHAR_SEND, this);
	}
	
	m_pCArenaCharSendDlg->ShowWindow(SW_SHOW);
	
}
// end 2012-06-16 by jhseol, ¾Æ·¹³ª Ãß°¡°³¹ßpart2 - Àü´Þ : Äû¸®¸¸µé±â


// 2013-04-30 by bckim. º£Æ®³² ¿î¿µÅø°ü·Ã Ã³¸®ExchangeAccountDBName 
void CAtumAdminToolDlg::ExchangeAccountDBName(CString *szTmpQuery)
{
#if defined(INTECOM_VIET)
	if( m_pServerInfo4Admin != NULL )
	{
		if ( strlen(m_pServerInfo4Admin->DBIP) == strlen("117.103.194.75") )
		{
			if( 0 == strcmp( m_pServerInfo4Admin->DBIP, "117.103.194.75") )
			{
				CString strBeforeAccount = "atum2_db_account";
				CString strAfterAccount = "db02.atum2_db_account";
				szTmpQuery->Replace( (LPCTSTR)strBeforeAccount, (LPCTSTR)strAfterAccount );
				// 			DBGOUT(*szTmpQuery);
				// 			DBGOUT("\r\n");
			}
		}
	}
#endif
}
// End. 2013-04-30 by bckim. º£Æ®³² ¿î¿µÅø°ü·Ã Ã³¸® 

void CAtumAdminToolDlg::OnButtonMonthlyArmorEvent() 
{
	// TODO: Add your control notification handler code here
	if (m_pSCMonthlyArmorEventDlg != NULL)
	{
		util::del(m_pSCMonthlyArmorEventDlg);
	}	
	if (m_pSCMonthlyArmorEventDlg == NULL)
	{
		m_pSCMonthlyArmorEventDlg = new SCMonthlyArmorEventDlg(this);
		m_pSCMonthlyArmorEventDlg->Create(IDD_DLG_MONTHLY_ARMOR_EVENT, this);
	}
	
	m_pSCMonthlyArmorEventDlg->ShowWindow(SW_SHOW);
}

