// Atum_LoadGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Atum_LoadGen.h"
#include "Atum_LoadGenDlg.h"
#include "md5_lib_src.h"
#include "AnalyzePacketDlg.h"		// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenDlg dialog
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){   return 0;}
CAtum_LoadGenDlg::CAtum_LoadGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAtum_LoadGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAtum_LoadGenDlg)
	m_ctl_strCharacterName = _T("");
	m_ctl_uiCharacterNameStartIndex = 0;
	m_ctl_uiCharacterNameEndIndex = 0;
	m_ctl_uiClientCount = 0;
	m_ctl_uiConnectedCount = 0;
	m_ctl_nX_End = 0;
	m_ctl_nX_Start = 0;
	m_ctl_nY = 0;
	m_ctl_nZ_End = 0;
	m_ctl_nZ_Start = 0;
	m_ctl_uiReconnectTime = 0;
	m_ctl_strFieldServerIP = _T("");
	m_ctl_uiFieldServerPort = 0;
	m_ctl_uiConnectFailed = 0;
	m_ctl_uiSpeed = 0;
	m_ctl_uiSendMoveCount = 0;
	m_ctl_bSendAttackFlag = FALSE;
	m_ctl_bSendAttackSec = FALSE;
	m_ctl_nPriReattakTime = 0;
	m_ctl_nSecReattakTime = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bStartFlag = FALSE;
	m_uiConnectTimerID = 0;
	m_uiSendMoveMSGTimerID = 0;

	m_uiSendAliveTimerID	= 0;	// 2008-03-12 by cmkwon
	m_uiTryConnectCount = 0;
}

void CAtum_LoadGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAtum_LoadGenDlg)
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME, m_ctl_strCharacterName);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME_START, m_ctl_uiCharacterNameStartIndex);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME_END, m_ctl_uiCharacterNameEndIndex);
	DDX_Text(pDX, IDC_EDIT_CLIENT_COUNT, m_ctl_uiClientCount);
	DDX_Text(pDX, IDC_EDIT_CONNECTED_COUNT, m_ctl_uiConnectedCount);
	DDX_Text(pDX, IDC_EDIT_X_END, m_ctl_nX_End);
	DDX_Text(pDX, IDC_EDIT_X_START, m_ctl_nX_Start);
	DDX_Text(pDX, IDC_EDIT_Y, m_ctl_nY);
	DDX_Text(pDX, IDC_EDIT_Z_END, m_ctl_nZ_End);
	DDX_Text(pDX, IDC_EDIT_Z_START, m_ctl_nZ_Start);
	DDX_Text(pDX, IDC_EDIT_CPM, m_ctl_uiReconnectTime);
	DDX_Text(pDX, IDC_EDIT_FIELD_SERVER_IP, m_ctl_strFieldServerIP);
	DDX_Text(pDX, IDC_EDIT_FIELD_SERVER_PORT, m_ctl_uiFieldServerPort);
	DDX_Text(pDX, IDC_EDIT_CONNECTED_FAILED, m_ctl_uiConnectFailed);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_ctl_uiSpeed);
	DDX_Text(pDX, IDC_EDIT_SEND_MOVE_COUNT, m_ctl_uiSendMoveCount);
	DDX_Check(pDX, IDC_CHECK_ATTACK, m_ctl_bSendAttackFlag);
	DDX_Check(pDX, IDC_CHECK_ATTACK2, m_ctl_bSendAttackSec);
	DDX_Text(pDX, IDC_EDIT_REATTACKTIME_1, m_ctl_nPriReattakTime);
	DDX_Text(pDX, IDC_EDIT_REATTACKTIME_2, m_ctl_nSecReattakTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAtum_LoadGenDlg, CDialog)
	//{{AFX_MSG_MAP(CAtum_LoadGenDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SUSPEND, OnBtnSuspend)
	ON_BN_CLICKED(IDC_BTN_ANALYZE_PACKET, OnBtnAnalyzePacket)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PACKET_NOTIFY, OnSocketNotify)
	ON_MESSAGE(WM_ASYNC_EVENT, OnAsyncSocketMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtum_LoadGenDlg message handlers

BOOL CAtum_LoadGenDlg::OnInitDialog()
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
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAtum_LoadGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAtum_LoadGenDlg::OnPaint() 
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
HCURSOR CAtum_LoadGenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAtum_LoadGenDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_ctl_strFieldServerIP.IsEmpty()
		|| m_ctl_uiFieldServerPort == 0
		|| m_ctl_uiClientCount == 0
		|| m_ctl_uiReconnectTime == 0
		|| m_ctl_strCharacterName.IsEmpty()
		|| m_ctl_uiSendMoveCount <= 0
		|| m_ctl_uiSendMoveCount > 10)
	{
		AfxMessageBox("Invalid Parameter !!");
		return;
	}

	m_ctl_uiReconnectTime = max(100, m_ctl_uiReconnectTime);	// 2007-01-23 by cmkwon, 최소 100ms
	m_ctl_uiSendMoveCount = min(10, m_ctl_uiSendMoveCount);		// 2007-01-23 by cmkwon, 초당 최대 10개 Move 전송
	m_ctl_nPriReattakTime = max(100, m_ctl_nPriReattakTime);	// 2007-01-23 by cmkwon, 최소 100ms
	m_ctl_nSecReattakTime = max(100, m_ctl_nSecReattakTime);	// 2007-01-23 by cmkwon, 최소 100ms


	m_ctl_uiCharacterNameEndIndex = m_ctl_uiCharacterNameStartIndex + m_ctl_uiClientCount - 1;
	UpdateData(FALSE);
	((CAtum_LoadGenApp*)AfxGetApp())->WriteProfile();

	m_bStartFlag = TRUE;
	GetDlgItem(IDC_BTN_START)->EnableWindow(!m_bStartFlag);
	GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow(m_bStartFlag);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bStartFlag);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-12 by cmkwon
	if(m_uiSendAliveTimerID)
	{
		KillTimer(TIMERID_SEND_CONNECT_ALIVE);
		m_uiSendAliveTimerID	= 0;
	}
	m_uiSendAliveTimerID = SetTimer(TIMERID_SEND_CONNECT_ALIVE, 15000, NULL);

	if(m_uiConnectTimerID)
	{
		KillTimer(TIMERID_CONNECT);
		m_uiConnectTimerID = 0;
	}

	m_uiConnectTimerID = SetTimer(TIMERID_CONNECT, m_ctl_uiReconnectTime, NULL);		

	OnTimer(TIMERID_CONNECT);
}

void CAtum_LoadGenDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	m_bStartFlag = FALSE;
	GetDlgItem(IDC_BTN_START)->EnableWindow(!m_bStartFlag);
	GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow(m_bStartFlag);
	GetDlgItem(IDC_BTN_SUSPEND)->SetWindowText("Suspend");
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(m_bStartFlag);

	if(m_uiSendMoveMSGTimerID)
	{
		KillTimer(TIMERID_SEND_MOVE_MSG);
		m_uiSendMoveMSGTimerID = 0;
	}
	
	if(m_uiConnectTimerID)
	{
		KillTimer(TIMERID_CONNECT);
		m_uiConnectTimerID = 0;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-12 by cmkwon
	if(m_uiSendAliveTimerID)
	{
		KillTimer(TIMERID_SEND_CONNECT_ALIVE);
		m_uiSendAliveTimerID = 0;
	}

	vector<CFieldWinSocket*>::iterator itr(m_vectFieldWinSocketPtr.begin());
	while (itr != m_vectFieldWinSocketPtr.end())
	{		
		(*itr)->CloseSocket(0x10001);
		delete (*itr);
		itr++;
	}
	m_vectFieldWinSocketPtr.clear();

	m_ctl_uiConnectedCount = 0;
	m_ctl_uiConnectFailed = 0;
	m_uiTryConnectCount = 0;

	UpdateData(FALSE);
}

void CAtum_LoadGenDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMERID_CONNECT:
		{
			if(m_bStartFlag == FALSE
				|| m_uiTryConnectCount >= m_ctl_uiClientCount)
			{
				KillTimer(TIMERID_CONNECT);
				m_uiConnectTimerID = 0;
			}
			else
			{
				CFieldWinSocket	*pFSocket = new CFieldWinSocket(GetSafeHwnd());
				pFSocket->SetLoadDlg(this);
				pFSocket->m_nArrayIndex = m_ctl_uiCharacterNameStartIndex + m_uiTryConnectCount;
				m_vectFieldWinSocketPtr.push_back(pFSocket);
				pFSocket->Connect(m_ctl_strFieldServerIP, m_ctl_uiFieldServerPort);
				pFSocket->SetConnectTick(timeGetTime());
				m_uiTryConnectCount++;
			}
		}
		break;
	case TIMERID_SEND_MOVE_MSG:
		{
			vector<CFieldWinSocket*>::iterator itr(m_vectFieldWinSocketPtr.begin());
			for(; itr != m_vectFieldWinSocketPtr.end(); itr++)
			{
				(*itr)->CharacterMove(m_ctl_nX_Start, m_ctl_nX_End, m_ctl_nY, m_ctl_nZ_Start, m_ctl_nZ_End, m_ctl_uiSpeed, m_ctl_bSendAttackFlag);
			}
		}
		break;
	case TIMERID_SEND_PRI_ATTACK_MSG:
		{
			if(FALSE == m_bStartFlag)
			{
				ReleaseSendPriAttackTimer();
			}
			else
			{
				vector<CFieldWinSocket*>::iterator itr(m_vectFieldWinSocketPtr.begin());
				for(; itr != m_vectFieldWinSocketPtr.end(); itr++)
				{
					(*itr)->SendPriAttackMsg();
				}
			}
		}
		break;
	case TIMERID_SEND_SEC_ATTACK_MSG:
		{
			if(FALSE == m_bStartFlag)
			{
				ReleaseSendSecAttackTimer();
			}
			else
			{
				vector<CFieldWinSocket*>::iterator itr(m_vectFieldWinSocketPtr.begin());
				for(; itr != m_vectFieldWinSocketPtr.end(); itr++)
				{
					(*itr)->SendSecAttackMsg();
				}
			}
		}
		break;
	case TIMERID_SEND_CONNECT_ALIVE:		// 2008-03-12 by cmkwon
		{
			if(m_bStartFlag)
			{
				vector<CFieldWinSocket*>::iterator itr(m_vectFieldWinSocketPtr.begin());
				for(; itr != m_vectFieldWinSocketPtr.end(); itr++)
				{
					CFieldWinSocket *pFISoc = *itr;
					if(pFISoc->IsConnected())
					{
						pFISoc->WriteMessageType(T_FC_CONNECT_ALIVE);
					}
				}
			}

		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

CFieldWinSocket *CAtum_LoadGenDlg::FindFieldWinSoketPtr(SOCKET hSock)
{
	vector<CFieldWinSocket*>::iterator itr(m_vectFieldWinSocketPtr.begin());
	while(itr != m_vectFieldWinSocketPtr.end())
	{
		if((*itr)->GetSocketHandle() == hSock)
		{
			return *itr;
		}
		itr++;
	}
	return NULL;
}

LONG CAtum_LoadGenDlg::OnAsyncSocketMessage(WPARAM wParam, LPARAM lParam)
{
	CFieldWinSocket *pFSocket = FindFieldWinSoketPtr(wParam);
	if(NULL == pFSocket)
	{
		return FALSE;
	}
	pFSocket->OnAsyncEvent(lParam);
	return TRUE;
}


LONG CAtum_LoadGenDlg::OnSocketNotify(WPARAM wParam, LPARAM lParam)
{
	if(0 == lParam) {		return FALSE;}

	char	SendBuf[1024];
	CFieldWinSocket *pFSocket = (CFieldWinSocket*)lParam;

	switch(LOWORD(wParam))
	{
	case CFieldWinSocket::WS_ERROR:
		{
		}
		break;
	case CFieldWinSocket::WS_CONNECTED:
		{
			if (HIWORD(wParam) == TRUE)
			{
				m_ctl_uiConnectedCount++;
				
				INIT_MSG(MSG_FC_CONNECT_LOGIN, T_FC_CONNECT_LOGIN, pSLogin, SendBuf);
				// 2008-03-19 by cmkwon, 아래와 같이 계정명 시스템을 수정함
				//sprintf(pSLogin->AccountName, "%s%d", m_ctl_strCharacterName, pFSocket->m_nArrayIndex);
				sprintf(pSLogin->AccountName, "%s%03d", m_ctl_strCharacterName, pFSocket->m_nArrayIndex);
				pFSocket->Write(SendBuf, MSG_SIZE(MSG_FC_CONNECT_LOGIN));
				strncpy(pFSocket->GetCharacterPtr()->CharacterName, pSLogin->AccountName, SIZE_MAX_CHARACTER_NAME);	// 2004-03-15 by cmkwon, AccountName과 CharacterName은 같다

				if(0 == m_uiSendMoveMSGTimerID)
				{
					m_uiSendMoveMSGTimerID = SetTimer(TIMERID_SEND_MOVE_MSG, 1000/m_ctl_uiSendMoveCount - 20, NULL);
				}
			}
			else
			{
				m_ctl_uiConnectFailed++;
			}
			UpdateData(FALSE);
		}
		break;
	case CFieldWinSocket::WS_CLOSED:
		{
			m_ctl_uiConnectedCount--;
			UpdateData(FALSE);
		}
		break;
	case CFieldWinSocket::WS_RECEIVED:
		{			
		}
		break;
	}
	return TRUE;
}

void CAtum_LoadGenDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bStartFlag)
	{
		OnBtnStop();
	}
	CDialog::OnOK();
}

void CAtum_LoadGenDlg::OnBtnSuspend() 
{
	// TODO: Add your control notification handler code here

	CString strSuspend;
	GetDlgItem(IDC_BTN_SUSPEND)->GetWindowText(strSuspend);

	if(strSuspend.Compare("Suspend") == 0)
	{
		GetDlgItem(IDC_BTN_SUSPEND)->SetWindowText("Resume");
		if(m_bStartFlag)
		{
			m_bStartFlag = FALSE;
			if(m_uiConnectTimerID)
			{
				KillTimer(TIMERID_CONNECT);
				m_uiConnectTimerID = 0;
			}
		}
	}
	else
	{
		GetDlgItem(IDC_BTN_SUSPEND)->SetWindowText("Suspend");
		if(m_uiTryConnectCount < m_ctl_uiClientCount)
		{
			m_bStartFlag = TRUE;
			if(m_uiConnectTimerID)
			{
				KillTimer(TIMERID_CONNECT);
				m_uiConnectTimerID = 0;
			}
			m_uiConnectTimerID = SetTimer(TIMERID_CONNECT, m_ctl_uiReconnectTime, NULL);			
		}
	}	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtum_LoadGenDlg::StartSendPriAttackTimer(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtum_LoadGenDlg::StartSendPriAttackTimer(void)
{
	if(FALSE == m_ctl_bSendAttackFlag)
	{
		return FALSE;
	}

	SetTimer(TIMERID_SEND_PRI_ATTACK_MSG, m_ctl_nPriReattakTime, NULL);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CAtum_LoadGenDlg::StartSendSecAttackTimer(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAtum_LoadGenDlg::StartSendSecAttackTimer(void)
{
	if(FALSE == m_ctl_bSendAttackSec)
	{
		return FALSE;
	}

	SetTimer(TIMERID_SEND_SEC_ATTACK_MSG, m_ctl_nSecReattakTime, NULL);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtum_LoadGenDlg::ReleaseSendPriAttackTimer(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtum_LoadGenDlg::ReleaseSendPriAttackTimer(void)
{
	if(FALSE == m_ctl_bSendAttackFlag)
	{
		return;
	}

	KillTimer(TIMERID_SEND_PRI_ATTACK_MSG);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CAtum_LoadGenDlg::ReleaseSendSecAttackTimer(void)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-23 ~ 2007-01-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CAtum_LoadGenDlg::ReleaseSendSecAttackTimer(void)
{
	if(FALSE == m_ctl_bSendAttackSec)
	{
		return;
	}

	KillTimer(TIMERID_SEND_SEC_ATTACK_MSG);
}

void CAtum_LoadGenDlg::OnBtnAnalyzePacket() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
	if(false == m_vectFieldWinSocketPtr.empty())
	{
		AfxMessageBox("Can't do that when LoadGenerator is working !!");
		return;
	}

	CAnalyzePacketDlg dlg;

	dlg.DoModal();
}
