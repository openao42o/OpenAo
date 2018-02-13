
// CrashReporterDlg.cpp
//
#include "stdafx.h"
#include "CrashReporter.h"
#include "CrashReporterDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <windows.h>
#include <wininet.h>
#include <d3d9.h>

#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "d3d9.lib")

using namespace std;

#define _DREAM_ACE

#ifdef _USE_HTTP
#undef _USE_FTP
#endif

#ifdef _USE_FTP
#undef _USE_HTTP
#endif

#ifdef _USE_FTP

#include <process.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "ws2_32")
#endif

#ifdef _USE_HTTP
#include <tchar.h>
#include <iostream>
#include <winsock2.h>
#include <fstream>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



CCrashReporterDlg::CCrashReporterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCrashReporterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCrashReporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCrashReporterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCrashReporterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CCrashReporterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

#ifdef _DEBUG
	AllocConsole();
#endif

	LPWSTR *szArglist;
	int nArgs;
	int i;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
	{
		LOG("CommandLineToArgvW failed\n");
		return 0;
	}
#ifndef _DEBUG
	else if (nArgs == 1)
	{
		exit(0);
	}
#endif
	else for (i = 0; i < nArgs; i++)
	{
		char buf[512];
		sprintf(buf, "%ws", CharLower(szArglist[i]));
		LOG("%d: %s\n", i, buf);
		if (strstr(buf, STARTPRM_DMPFILE) != NULL)
		{
			sprintf(buf, "%ws", CharLower(szArglist[i+1]));
			m_szDmpFile = buf;
		}
		else if (strstr(buf, STARTPRM_ACCNAME) != NULL)
		{
			sprintf(buf, "%ws", CharLower(szArglist[i + 1]));
			m_szAccName = buf;
		}
		else if (strstr(buf, STARTPRM_CHARNUMBER) != NULL)
		{
			sprintf(buf, "%ws", CharLower(szArglist[i + 1]));
			m_iCharNumber = atoi(buf);
		}
		else if (strstr(buf, STARTPRM_MAPNUMBER) != NULL)
		{
			sprintf(buf, "%ws", CharLower(szArglist[i + 1]));
			m_iMapNumber = atoi(buf);
		}
		else if (strstr(buf, STARTPRM_EXCEPTIONCODE) != NULL)
		{
			sprintf(buf, "%ws", CharLower(szArglist[i + 1]));
			m_szExceptionCode = buf;
		}

	}

	if (m_szDmpFile == "" || m_szAccName == "" || m_iCharNumber == NULL || m_iMapNumber == NULL || m_szExceptionCode == "")
	{
		LOG("CommandLine Error!\n");
		exit(0);
	}


#ifdef _DEBUG
	char buf[512];
	sprintf(buf, "Args: %s %s %i %i %s\n", m_szDmpFile.c_str(), m_szAccName.c_str(), m_iCharNumber, m_iMapNumber, m_szExceptionCode.c_str());
	LOG(buf);
#endif

	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree(szArglist);

	CString sText;
	sText.LoadString(IDS_INFO);
	CString sName;
#ifdef _DREAM_ACE
	sName.LoadString(IDS_NAME_DA);
#else
	sName.LoadString(IDS_NAME_ES);
#endif
	CString sFullText;
	sFullText.Format(sText, sName);

	SetDlgItemText(IDC_INFO_LABEL, sFullText);

	CListBox* m_ListBox = (CListBox*)GetDlgItem(IDC_LIST_CRASHINFO);

	char listBuf[512];

	sprintf_s(listBuf, "1. AccountName: %s", m_szAccName.c_str());
	m_ListBox->AddString(CA2W(listBuf));
	sprintf_s(listBuf, "2. MapNumber: %d", m_iMapNumber);
	m_ListBox->AddString(CA2W(listBuf));
	sprintf_s(listBuf, "3. Exceptioncode: %s", m_szExceptionCode.c_str());
	m_ListBox->AddString(CA2W(listBuf));
	sprintf_s(listBuf, "4. CPU Name: %s", GetCPUName().c_str());
	m_ListBox->AddString(CA2W(listBuf));
	sprintf_s(listBuf, "5. GPU Name: %s", GetGPUName().c_str());
	m_ListBox->AddString(CA2W(listBuf));
	sprintf_s(listBuf, "6. RAM Amount: %.1fGB", GetRAMSize());
	m_ListBox->AddString(CA2W(listBuf));
	sprintf_s(listBuf, "7. OS Version: %s", GetOSVersion().c_str());
	m_ListBox->AddString(CA2W(listBuf));

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);


	return TRUE; 
}

void CCrashReporterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void CCrashReporterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CCrashReporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCrashReporterDlg::OnBnClickedOk()
{
	this->UploadFile(m_szDmpFile);
	UploadData();
	MessageBox(L"Thank you for uploading your crashdump!", L"Success", MB_OK);
	CDialogEx::OnOK();
}

BOOL CCrashReporterDlg::CheckDmpFile(const std::string& name)
{
	LOG(name.c_str());
	return PathFileExists((LPWSTR)name.c_str());
}

#ifdef _USE_HTTP
unsigned long WinsockStart()
{
	WSADATA wsa;
	unsigned long ulong;
	struct hostent *host;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
		LOG("Error WinsockStart()");
		WSACleanup();
		return 1;
	}

	if ((host = gethostbyname(HTTP_HOST))<0)
	{
		LOG("Error gethostbyname()");
		WSACleanup();
		return 2;
	}

	ulong = *(unsigned long*)host->h_addr;

	return ulong;
}

void error_exit(string text)
{
	char buf[1024];
	sprintf(buf, "Could not Upload the DMP file\nError: %s", text.c_str());

	MessageBox(NULL, CA2W(buf), L"Error!", MB_OK);

	LOG(text.c_str());
	WSACleanup();
	exit(EXIT_FAILURE);
}
#endif

void CCrashReporterDlg::UploadFile(string szFileName)
{
	if (_access(szFileName.c_str(), 0) == -1)
	{
		MessageBox(L"File does not exists!\nExiting program...", L"Error!", MB_OK);
		exit(0);
	}
#ifdef _USE_FTP
	char sTempHost[1024];
	util::zero(sTempHost, 1024);
	char sTempLogin[1024];
	util::zero(sTempLogin, 1024);
	char sTempPassword[1024];
	util::zero(sTempPassword, 1024);

	XOR::XORDecrypt(sTempHost, FTP_HOST, XOR_KEY);
	XOR::XORDecrypt(sTempLogin, FTP_ACCOUNT, XOR_KEY);
	XOR::XORDecrypt(sTempPassword, FTP_PASSWORD, XOR_KEY);

	LOG("Host: %s Login: %s Password: %s\n", sTempHost, sTempLogin, sTempPassword);

	HINTERNET hOpen, hConnection;

	hOpen = InternetOpen(L"FTP Upload", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

	if (hOpen == NULL)
	{
		MessageBox(L"Could not open the FTP connection!", L"Error!", MB_OK);
		return;
	}
	hConnection = InternetConnect(hOpen, CA2W(sTempHost), INTERNET_DEFAULT_FTP_PORT, CA2W(sTempLogin), CA2W(sTempPassword), INTERNET_SERVICE_FTP, 0, 0);

	if (hConnection == NULL)
	{
		MessageBox(L"Could not connect to the FTP Server!", L"Error!", MB_OK);
		return;
	}

	FtpSetCurrentDirectory(hConnection, L"/");

	size_t lastindex = szFileName.find_last_of(".");
	string rawname = szFileName.substr(0, lastindex);
	string ext = szFileName.substr(lastindex, szFileName.length());

	char buf[128];
	sprintf(buf, "%s_%s_%s%s", rawname.c_str(), m_szAccName.c_str(), GetCurrentDateTime().c_str(), ext.c_str());

	LOG("%s\n", buf);
	BOOL success = FtpPutFile(hConnection, CA2W(szFileName.c_str()), CA2W(buf), FTP_TRANSFER_TYPE_BINARY, 0);

	if (!success)
	{
		MessageBox(L"Could not upload the file to the FTP Server!", L"Error!", MB_OK);
		return;
	}

	InternetCloseHandle(hConnection);
	InternetCloseHandle(hOpen);
#endif
#ifdef	_USE_HTTP
	FILE* uploadFile;
	fopen_s(&uploadFile,szFileName.c_str(), "rb");
	if (uploadFile == NULL)
	{
		MessageBox(L"Could not open the file!", L"Error!", MB_OK);
		return;
	}
	
	fseek(uploadFile, 0, SEEK_END);
	long fsize = ftell(uploadFile);
	fseek(uploadFile, 0, SEEK_SET);

	char *content = new char[fsize + 1]{};
	fread(content, fsize, 1, uploadFile);
	fclose(uploadFile);
	
	SOCKET sock;
	struct sockaddr_in addr;
	unsigned long win = 0;
	int con = 0, gr = 0, send_r = 0, rec = 0;
	char header[2048];
	string to_send(content);

	char buf[512];

	sprintf(buf, "%s_%s",m_szAccName.c_str(), szFileName.c_str());

	string name = (buf);

	win = WinsockStart();
	if (win == 1 || win == 2)
		error_exit("Error WinsockStart()");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(HTTP_PORT);
	addr.sin_addr.s_addr = win;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0)
		error_exit("Error socket()");

	gr = (to_send.size() + name.size() + 287);

	sprintf(header, "POST %s HTTP/1.1\r\n", HTTP_DIRECTORY);
	sprintf(header, "%sHost:%s\r\n", header, HTTP_HOST);
	sprintf(header, "%sConnection: Keep-Alive\r\n", header);
	sprintf(header, "%sContent-Type: multipart/form-data; boundary=---------------------------90721038027008\r\n", header);
	sprintf(header, "%sContent-Length: %d\r\n", header, gr);
	sprintf(header, "%s\r\n", header);
	sprintf(header, "%s-----------------------------90721038027008\r\n", header);
	sprintf(header, "%sContent-Disposition: form-data; name=\"upfile\"; filename=\"%s\"\r\n", header, name.c_str());
	sprintf(header, "%sContent-Type: text/plain\r\n", header);
	sprintf(header, "%s\r\n", header);
	sprintf(header, "%s%s\r\n", header, to_send.c_str());
	sprintf(header, "%s-----------------------------90721038027008\r\n", header);
	sprintf(header, "%sContent-Disposition: form-data; name=\"post\"\r\n", header);
	sprintf(header, "%s\r\n", header);
	sprintf(header, "%supload\r\n\r\n", header);
	sprintf(header, "%s-----------------------------90721038027008--\r\n\r\n\0", header);

	con = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (con < 0)
		error_exit("Error connect()");

	if (send_r = send(sock, header, strlen(header), 0)<0)
		error_exit("Error send()");

	closesocket(sock);
	WSACleanup();

#endif
	LOG("Success!\n");
	remove(szFileName.c_str());
}

void CCrashReporterDlg::UploadData()
{
	size_t lastindex = m_szDmpFile.find_last_of(".");
	string rawname = m_szDmpFile.substr(0, lastindex);
	string fileName(rawname + ".txt");
	CString data;
	CListBox* listBox = (CListBox*)GetDlgItem(IDC_LIST_CRASHINFO);
	ofstream myFile;
	myFile.open(fileName);

	int itemCount = listBox->GetCount();

	for (int i = 0; i < itemCount; i++)
	{
		listBox->GetText(i, data);

		char buf[512];
		sprintf(buf, "%ws\n", (char*)data.GetString());

		myFile << buf;
	}
	myFile.close();

	this->UploadFile(fileName);
}

string CCrashReporterDlg::GetCPUName()
{
	int CPUInfo[4] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	unsigned int nExIds = CPUInfo[0];

	// Get the information associated with each extended ID.
	char CPUBrandString[0x40] = { 0 };
	for (unsigned int i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);

		// Interpret CPU brand string and cache information.
		if (i == 0x80000002)
		{
			memcpy(CPUBrandString,
				CPUInfo,
				sizeof(CPUInfo));
		}
		else if (i == 0x80000003)
		{
			memcpy(CPUBrandString + 16,
				CPUInfo,
				sizeof(CPUInfo));
		}
		else if (i == 0x80000004)
		{
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
		}
	}

	return CPUBrandString;
}

string CCrashReporterDlg::GetOSVersion()
{
	DWORD dwVersion = GetVersion();
	DWORD dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
	DWORD dwBuild = 0;

	if (dwVersion < 0x80000000)
		dwBuild = (DWORD)(HIWORD(dwVersion));

	char buf[512];
	sprintf(buf, "%d.%d (%d)", dwMajorVersion,
		dwMinorVersion,
		dwBuild);

	string ver = buf;
	return ver;
}

string CCrashReporterDlg::GetGPUName()
{
	IDirect3D9 *pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DADAPTER_IDENTIFIER9 info;
	pD3d->GetAdapterIdentifier(0, 0, &info);

	return info.Description;
}

float CCrashReporterDlg::GetRAMSize()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	return (float)statex.ullTotalPhys / (1024 * 1024 * 1024);
}

string CCrashReporterDlg::GetCurrentDateTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "_%I_%M__%d_%m_%Y", timeinfo);
	std::string str(buffer);

	return str;
}