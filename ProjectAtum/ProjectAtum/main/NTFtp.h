// 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송

//######################################################################################
// File    : NTFtp.h
// Date    : 2007-1-8
// Author  : 고리(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Ftp파일 전송
// Bug     : 
//######################################################################################
#ifndef __NEXT_FTP__
#define __NEXT_FTP__

#include <windows.h>
#include <wininet.h>
#include <tchar.h>

class CNTFtp
{
public:
	CNTFtp();
	~CNTFtp();

	void DisConnect();
	bool Connect(const TCHAR *pszServer, WORD dPort, const TCHAR *pszUserID, const TCHAR *pszUserPassword);
	bool UpLoad(HWND hWnd, const TCHAR *pszFileName);
	HINTERNET GetFtp(void) { return m_hFtp; }

private:

	HINTERNET m_hInternet;
	HINTERNET m_hFtp;
};


#endif //(__NEXT_FTP__)

// end 2011-08-17 by hsson 클라이언트 크래쉬 날때 서버로 클라 정보 전송