// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTFtp.h
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Ftp파일 전송
// Bug     : 
//######################################################################################
#include "NTFTP.h"
#include <commctrl.h>

#pragma comment(lib, "Wininet.lib")

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 생성자
//######################################################################################
CNTFtp::CNTFtp() :
m_hFtp( NULL ),
m_hInternet( NULL )
{
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 소멸자
//######################################################################################
CNTFtp::~CNTFtp()
{
	DisConnect();
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 연결 끊기
//######################################################################################
void CNTFtp::DisConnect()
{
	InternetCloseHandle(m_hFtp);
	InternetCloseHandle(m_hInternet);
	m_hFtp = NULL;
	m_hInternet = NULL;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 연결
//######################################################################################
bool CNTFtp::Connect(const TCHAR *pszServer, WORD dPort, const TCHAR *pszUserID, const TCHAR *pszUserPassword)
{
	m_hInternet=InternetOpen(_T("NTFTP-MiniDummp"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
	if (m_hInternet == NULL) 
	{
		return false;
	}

	m_hFtp=InternetConnect(m_hInternet,pszServer,dPort, pszUserID,pszUserPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);

	if (m_hFtp==NULL) 
	{
		InternetCloseHandle(m_hInternet);
		return false;
	}

	return true;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Desc    : 파일 전송
//######################################################################################
bool CNTFtp::UpLoad(HWND hWnd, const TCHAR *pszFileName)
{
// 	HINTERNET hRemote;
// 	HANDLE hLocal;
// 	TCHAR ext[_MAX_EXT];
// 	TCHAR Remote[MAX_PATH];
// 	DWORD dwRead,dwWritten,Prog;
// 	DWORD dwFileSize;
// 	BOOL Result;
// 	TCHAR buf[1000];
// 
// 	//_tsplitpath(pszFileName,NULL,NULL,Remote,ext);
// 	_tsplitpath_s(pszFileName,NULL,NULL,NULL,NULL,Remote,sizeof(Remote),ext,sizeof(ext));
// 	_tcscat_s(Remote,ext);
// 	hRemote=FtpOpenFile(m_hFtp,Remote,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY,0);
// 
// 	if( hRemote == NULL )
// 		return false;
// 
// 	hLocal=CreateFile(pszFileName,GENERIC_READ,0,NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
// 
// 	dwFileSize = GetFileSize(hLocal, NULL);
// 
// 	Prog=0;
// 	for (;;) 
// 	{
// 		Result=ReadFile(hLocal,buf,1000,&dwRead,NULL);
// 		if ((Result==TRUE) && (dwRead==0))
// 			break;
// 		InternetWriteFile(hRemote,buf,1000,&dwWritten);
// 		Prog+=dwRead;
// 
// 		if( NULL != hWnd )
// 		{
// 			int nProgValue = (int)( (Prog*100)/dwFileSize );
// 			if( nProgValue < 0 )
// 				nProgValue = 0;
// 			else if( nProgValue > 100 )
// 				nProgValue = 100;
// 
// 			//Sleep(200);
// 			SendMessage( hWnd, PBM_SETPOS, nProgValue , 0 );
// 		}
// 	}
// 
// 	CloseHandle(hLocal);
// 	InternetCloseHandle(hRemote);

	return true;
}



// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송