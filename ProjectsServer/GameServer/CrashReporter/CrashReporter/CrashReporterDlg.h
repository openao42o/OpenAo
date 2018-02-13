// CrashReporterDlg.h
//

#pragma once


#include <conio.h> 
#include <stdio.h>
#include <string>

#pragma warning(disable: 4996)

#ifdef _USE_FTP
#include <XOR_Lib.h>
#define util::zero(p, Size) (memset((p), 0x00, (Size)))
#endif

#define STARTPRM_DMPFILE "dmpfile"
#define STARTPRM_ACCNAME "accountname"
#define STARTPRM_CHARNUMBER "charnumber"
#define STARTPRM_MAPNUMBER "mapnumber"
#define STARTPRM_EXCEPTIONCODE "code"

#ifdef _USE_FTP
#define XOR_KEY "J60!_J18WaW7%{!FW2}qs};H6WY3m?3zyw4RmWssK8Cn0Dse3g6'g2;*lm2#mEk"
#define FTP_HOST "7B00070F6E7B051666556219144812" //localhost 31.170.160.91
#define FTP_PORT 21
#define FTP_ACCOUNT "2E53525438" //dmpUpload a5976874
#define FTP_PASSWORD "39427D76122D725D13160F75121614"	//dmpUpload stMWMgCeDwXB7m5
#endif
#ifdef _USE_HTTP
#define HTTP_HOST "localhost"
#define HTTP_PORT 80
#define HTTP_DIRECTORY "/dmp/upload.php"
#endif


#ifdef _DEBUG
#define LOG _cprintf
#else
#define LOG __noop
#endif

using namespace std;

class CCrashReporterDlg : public CDialogEx
{
public:
	CCrashReporterDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_CRASHREPORTER_DIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	string m_szDmpFile;
	string m_szAccName;
	int m_iMapNumber;
	int m_iCharNumber;
	string m_szExceptionCode;


	BOOL CheckDmpFile(const std::string& name);
	void UploadFile(string szFileName);
	void UploadData();
	string GetCPUName();
	string GetGPUName();
	string GetOSVersion();
	float GetRAMSize();
	string GetCurrentDateTime();
};
