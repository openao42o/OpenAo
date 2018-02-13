// zipfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "zipfile1.h"
#include "zipfileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
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
*/

/////////////////////////////////////////////////////////////////////////////
// CZipfileDlg dialog

CZipfileDlg::CZipfileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZipfileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZipfileDlg)
	m_bEncode = FALSE;
	m_strNumber = _T("");
	m_szTargetFileName = _T("");
	m_szEncodeStr = _T("");
	m_szTargetDir = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZipfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZipfileDlg)
	DDX_Check(pDX, IDC_CHECK_ENCODE, m_bEncode);
	DDX_Text(pDX, IDC_STATIC_NUMBER, m_strNumber);
	DDX_Text(pDX, IDC_EDIT_TARGETFILE, m_szTargetFileName);
	DDX_Text(pDX, IDC_EDIT_ENCODESTR, m_szEncodeStr);
	DDX_Text(pDX, IDC_EDIT_TARGETDIR, m_szTargetDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CZipfileDlg, CDialog)
	//{{AFX_MSG_MAP(CZipfileDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_AddFile, OnBUTTONAddFile)
	ON_BN_CLICKED(IDC_BUTTON_Delete, OnBUTTONDelete)
	ON_BN_CLICKED(IDC_BUTTON_Make, OnBUTTONMake)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZipfileDlg message handlers

BOOL CZipfileDlg::OnInitDialog()
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
//		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

//	// Set the icon for this dialog.  The framework does this automatically
//	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	ReadProfile();
	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CZipfileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
//		CAboutDlg dlgAbout;
//		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CZipfileDlg::OnPaint() 
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
HCURSOR CZipfileDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CZipfileDlg::OnBUTTONAddFile() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CString OpenFile,Temp;
//	char strDir[256];
//	GetCurrentDirectory(sizeof(strDir), strDir);

	CFileDialog dlg(true,"*",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, "* Files (*.*)|*.*", NULL);
	dlg.m_ofn.lpstrFile = new TCHAR[102400];
	dlg.m_ofn.lpstrFile[0] = '\0';
	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrInitialDir = m_szTargetDir;
	CListBox* pListBox;
	pListBox = (CListBox*)GetDlgItem(IDC_LIST_FILEPATH);

	int count = 0;
	if(dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();
		while(pos)
		{
			CString strPath = dlg.GetNextPathName( pos );

			char* pdest = strrchr((char*)(LPCTSTR)strPath, '\\');
   			UINT Result = pListBox->FindStringExact( 0, pdest+1 ); 
			if( Result == -1 )
			{
				count++;
				m_strNumber.Format("File number : %d", count);
				UpdateData(FALSE);
				pListBox->AddString(pdest+1);
			}
			else
			{
				MessageBox("File to register is extant already.");	
			}

		}
	}
	delete dlg.m_ofn.lpstrFile;
}

void CZipfileDlg::OnBUTTONDelete() 
{
	// TODO: Add your control notification handler code here
	int nSelItem;
	CListBox* pListBox;
	pListBox = (CListBox*)GetDlgItem(IDC_LIST_FILEPATH);
	nSelItem = pListBox->GetCurSel();
	
	if( nSelItem != LB_ERR )
		pListBox->DeleteString( nSelItem );
	else
        MessageBox("No select");	
}

void CZipfileDlg::OnBUTTONMake() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	Zipfile Encode_file;
	int FileCount;
	
    GetDlgItemText(IDC_EDIT_TARGETFILE, Encode_file.m_EncodeStrFilePath);
	GetDlgItemText(IDC_EDIT_ENCODESTR, Encode_file.m_EncodeString);
	
	CListBox* pListBox;
	pListBox = (CListBox*)GetDlgItem(IDC_LIST_FILEPATH);
	FileCount = pListBox->GetCount();

	CProgressCtrl* pProgress;
	pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_MAKE);
	
	CString openfile[10000];
	for(int i=0 ; i<FileCount ; i++)
	{
		pListBox->GetText(i, openfile[i]);
        //MessageBox(openfile[i]);	
	}
	Encode_file.make_header(openfile,FileCount);
	Encode_file.make_zip_file(openfile,*pProgress, m_bEncode);
	if(m_bEncode)
		Encode_file.make_encode_file(*pProgress);

	MessageBox("Making Completed");

	WriteProfile();
}

void CZipfileDlg::ReadProfile()
{
	CWinApp *pApp = AfxGetApp();

	// registry path: HKEY_CURRENT_USER\Software\Masang Soft\SpaceCowboy Monitor\Configuration
	m_szTargetFileName = pApp->GetProfileString( _T("Configuration"), _T("TexGenTargetFileName"), "quest.tex");
	m_szTargetDir = pApp->GetProfileString( _T("Configuration"), _T("TexGenTargetDir"), "");
	m_szEncodeStr = pApp->GetProfileString( _T("Configuration"), _T("TexGenEncodeStr"), "");
	m_bEncode = pApp->GetProfileInt( _T("Configuration"), _T("TexGenEncoding"), FALSE);
}

void CZipfileDlg::WriteProfile()
{
	CWinApp *pApp = AfxGetApp();

	// registry path: HKEY_CURRENT_USER\Software\Masang Soft\SpaceCowboy Monitor\Configuration
	pApp->WriteProfileString(_T("Configuration"), _T("TexGenTargetFileName"), m_szTargetFileName);
	pApp->WriteProfileString(_T("Configuration"), _T("TexGenTargetDir"), m_szTargetDir);
	pApp->WriteProfileString(_T("Configuration"), _T("TexGenEncodeStr"), m_szEncodeStr);
	pApp->WriteProfileInt(_T("Configuration"), _T("TexGenEncoding"), m_bEncode);
}
