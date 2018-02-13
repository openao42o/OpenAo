// AtumMonitor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AtumMonitor.h"

#include "MainFrm.h"
#include "AtumMonitorDoc.h"
#include "LeftView.h"
#include "ClientFTPUpdateManagerDlg.h"
#include "WinSocket.h"
#include "VMemPool.h"			// 2008-05-15 by cmkwon, 메모리풀 사용 적용

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// check: MSG를 print하는 level을 조정하기 위해(AtumMonitor로 조절 가능), by kelovon
//BYTE		g_exchangeMsgPrintLevel = PRINTLEVEL_NO_MSG;
//-- CMKWON 2003-07-12 Global.h와 Global.cpp에서 정의함

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorApp

BEGIN_MESSAGE_MAP(CAtumMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CAtumMonitorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorApp construction

CAtumMonitorApp::CAtumMonitorApp()
:CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAtumMonitorApp object

CAtumMonitorApp theApp;

CString			g_szPreServerIPReg;
CString			g_szIMServerIPReg;
CString			g_szLogServerIPReg;
CString			g_szFieldServerIPReg;
CString			g_szNPCServerIPReg;
CString			g_szServerIPHistoryReg;

int				g_nServerPortReg;
vector<string>	g_ServerGroupNameVector;
int				g_nWinLeftReg;
int				g_nWinRightReg;
int				g_nWinTopReg;
int				g_nWinBottomReg;

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorApp initialization

BOOL CAtumMonitorApp::InitInstance()
{
	AfxEnableControlContainer();

	CWinSocket::SocketInit();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	///////////////////////////////////////////////////////////////////////////////
	// 2008-05-15 by cmkwon, 메모리풀 사용 적용
	VMemPool::vmPoolAddObject(sizeof(CSendPacket), 1000);
	VMemPool::vmPoolAddObject(sizeof(CRecvPacket), 100);
	if(VMemPool::vmPoolInit() == FALSE)
	{
		return FALSE;
	}

	memset(m_szExeFileName, 0x00, MAX_PATH);
	GetExecuteFileName(m_szExeFileName);
	SetAppName(m_szExeFileName);

	SetRegistryKey(STRMSG_REG_STRING_REGISTRYKEY_NAME);
	ReadProfile();

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-24 by cmkwon, 서버 정보 암호화 - 암호화 되어 있는 데이터를 복호화 한다
	GDecryptGameServerInfoByXOR();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAtumMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
		// No message handlers
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

// App command to run the dialog
void CAtumMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorApp message handlers


int CAtumMonitorApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	CWinSocket::SocketClean();
	return CWinApp::ExitInstance();
}

#define SERVER_GROUP_NAME			"ServerGroupName"

void CAtumMonitorApp::WriteSizeProfile()
{
	WriteProfileInt(_T("Configuration"), _T("WinLeft"), g_nWinLeftReg);
	WriteProfileInt(_T("Configuration"), _T("WinRight"), g_nWinRightReg);
	WriteProfileInt(_T("Configuration"), _T("WinTop"), g_nWinTopReg);
	WriteProfileInt(_T("Configuration"), _T("WinBottom"), g_nWinBottomReg);
}

void CAtumMonitorApp::ReadSizeProfile()
{
	g_nWinLeftReg = GetProfileInt( _T("Configuration"), _T("WinLeft"), 0);
	g_nWinRightReg = GetProfileInt( _T("Configuration"), _T("WinRight"), 600);
	g_nWinTopReg = GetProfileInt( _T("Configuration"), _T("WinTop"), 0);
	g_nWinBottomReg = GetProfileInt( _T("Configuration"), _T("WinBottom"), 600);
}

void CAtumMonitorApp::WriteProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\Atum Launcher\Configuration
	WriteProfileString(_T("Configuration"), _T("PreServerIP"), g_szPreServerIPReg);
	WriteProfileString(_T("Configuration"), _T("IMServerIP"), g_szIMServerIPReg);
	WriteProfileString(_T("Configuration"), _T("LogServerIP"), g_szLogServerIPReg);
	WriteProfileString(_T("Configuration"), _T("FieldServerIP"), g_szFieldServerIPReg);
	WriteProfileString(_T("Configuration"), _T("NPCServerIP"), g_szNPCServerIPReg);
	WriteProfileString(_T("Configuration"), _T("ServerIPHistory"), g_szServerIPHistoryReg);

	WriteProfileInt(_T("Configuration"), _T("ServerPort"), g_nServerPortReg);
	WriteProfileString(_T("Configuration"), _T("ReinstallVersion"), m_ReinstallVersion.GetVersionString());
	WriteProfileString(_T("Configuration"), _T("UpdateFTPIPAddress"), m_FTPIPAddress);
	WriteProfileString(_T("Configuration"), _T("LatestVersion"), m_LatestVersion.GetVersionString());
	WriteProfileString( _T("Configuration"), _T("InfoGenPath"), m_szInfoGenOutPath);
	WriteProfileString( _T("Configuration"), _T("GameServerName"), m_strGameServerName);
	WriteProfileString( _T("Configuration"), _T("DbgOutMonitorPath"), m_szDbgOutBin);
	WriteProfileString( _T("Configuration"), _T("NoticeFilePath"), m_cstrNoticeFilePath);

	CString		csTemp;
	int i;
	for(i=0; i < g_ServerGroupNameVector.size(); i++)
	{
		csTemp.Format("%s%d", SERVER_GROUP_NAME, i);
//		WriteProfileString(_T("Configuration"), (char*)(LPCSTR)csTemp, g_ServerGroupNameVector[i].c_str());
	}

	for(; i < 100; i++)
	{
		csTemp.Format("%s%d", SERVER_GROUP_NAME, i);
//		WriteProfileString(_T("Configuration"), (char*)(LPCSTR)csTemp, NULL);
	}

	// ClientFTPUpdateManager
	WriteProfileString( _T("Configuration"), _T("UpdateFilesFolder"), m_szFolderToZipReg);
	WriteProfileString( _T("Configuration"), _T("OutputFolder"), m_szOutPutFolderReg);
	WriteProfileString( _T("Configuration"), _T("StartVersion"), m_szStartVersionReg);
	WriteProfileString( _T("Configuration"), _T("EndVersion"), m_szEndVersionReg);
	WriteProfileInt( _T("Configuration"), _T("UsePrevZipFile"), m_bUsePreviousVersionZipFileReg);
	WriteProfileInt( _T("Configuration"), _T("CompressLevel"), m_nCompressLevelReg);
}

void CAtumMonitorApp::ReadProfile()
{
	// registry path: HKEY_CURRENT_USER\Software\Atum Online\Atum Launcher\Configuration
	g_szPreServerIPReg		= GetProfileString( _T("Configuration"), _T("PreServerIP"), "");
	g_szIMServerIPReg		= GetProfileString( _T("Configuration"), _T("IMServerIP"), "");
	g_szLogServerIPReg		= GetProfileString( _T("Configuration"), _T("LogServerIP"), "");
	g_szFieldServerIPReg	= GetProfileString( _T("Configuration"), _T("FieldServerIP"), "");
	g_szNPCServerIPReg		= GetProfileString( _T("Configuration"), _T("NPCServerIP"), "");
	g_szServerIPHistoryReg	= GetProfileString(_T("Configuration"), _T("ServerIPHistory"), "");

	g_nServerPortReg		= GetProfileInt( _T("Configuration"), _T("ServerPort"), 0);
	m_ReinstallVersion.SetVersion((char*)(LPCSTR)GetProfileString( _T("Configuration"), _T("ReinstallVersion"), "0.1.0.0"));
	m_FTPIPAddress			= GetProfileString(_T("Configuration"), _T("UpdateFTPIPAddress"), "");
	m_LatestVersion.SetVersion((char*)(LPCSTR)GetProfileString( _T("Configuration"), _T("LatestVersion"), "0.1.0.25"));
	m_szInfoGenOutPath		= GetProfileString( _T("Configuration"), _T("InfoGenPath"), ".\\");
	m_strGameServerName		= GetProfileString( _T("Configuration"), _T("GameServerName"), "");
	m_szDbgOutBin			= GetProfileString( _T("Configuration"), _T("DbgOutMonitorPath"), ".\\");
	m_cstrNoticeFilePath	= GetProfileString( _T("Configuration"), _T("NoticeFilePath"), "");

	CString		csServerGroupName, csTemp;
	for(int i=0; i < 100; i++)
	{
		csTemp.Format("%s%d", SERVER_GROUP_NAME, i);
		if(i==0)
		{
			csServerGroupName = GetProfileString(_T("Configuration"), csTemp, "SpaceCowboy");
		}
		else
		{
			csServerGroupName = GetProfileString(_T("Configuration"), csTemp, "");
		}

		if(csServerGroupName.IsEmpty()){		break;}
		g_ServerGroupNameVector.push_back((char*)(LPCSTR)csServerGroupName);
	}

	// ClientFTPUpdateManager
	m_szFolderToZipReg	= GetProfileString( _T("Configuration"), _T("UpdateFilesFolder"), NULL);
	m_szOutPutFolderReg	= GetProfileString( _T("Configuration"), _T("OutputFolder"), NULL);
	m_szStartVersionReg	= GetProfileString( _T("Configuration"), _T("StartVersion"), NULL);
	m_szEndVersionReg	= GetProfileString( _T("Configuration"), _T("EndVersion"), NULL);
	m_bUsePreviousVersionZipFileReg	= GetProfileInt( _T("Configuration"), _T("UsePrevZipFile"), FALSE);
	m_nCompressLevelReg	= ATUM_DEFAULT_COMPRESSION_LEVEL; //GetProfileInt( _T("Configuration"), _T("CompressLevel"), ATUM_DEFAULT_COMPRESSION_LEVEL);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumMonitorApp::SetAppName(char *i_szAppName)
/// \brief		
/// \author		cmkwon
/// \date		2005-08-22 ~ 2005-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumMonitorApp::SetAppName(char *i_szAppName)
{
	free((void*)m_pszAppName);
	m_pszAppName = _tcsdup(i_szAppName);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtumMonitorApp::GetExecuteFileName(char *o_szFileName)
/// \brief		
/// \author		cmkwon
/// \date		2005-08-22 ~ 2005-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtumMonitorApp::GetExecuteFileName(char *o_szFileName)
{
	char szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);
	
	CString strTemp = szTemp;
	if(0 < strTemp.ReverseFind('\\'))
	{
		strcpy(szTemp, &szTemp[strTemp.ReverseFind('\\')+1]);
		strTemp.Format("%s", szTemp);
		if(0 < strTemp.ReverseFind('.'))
		{
			szTemp[strTemp.ReverseFind('.')] = 0;
		}
	}
	strcpy(o_szFileName, szTemp);
}