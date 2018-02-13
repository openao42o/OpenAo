// FtpDownloadFiles.h: interface for the CFtpDownloadFiles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPDOWNLOADFILES_H__D3F51062_1292_496B_ADDC_A003B585A4A7__INCLUDED_)
#define AFX_FTPDOWNLOADFILES_H__D3F51062_1292_496B_ADDC_A003B585A4A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxinet.h>
#include "FtpDownload.h"



class CFtpDownloadFiles  
{
protected:
	CInternetSession m_csInternet;//All argument defaults are OK.
	CFtpConnection* m_pFtpConnection;//The ftp_connection

	CFtpDownload * m_pFtpI;//Pointer to FTPDownload object in other thread that also holds dialog
	CFile m_fileObject;//CFileObject used to write file

	BOOL ReadFile(CString & source, CString & dest);
	BOOL FtpFileExists(CString & source);
	void FileNotFound(CString & source);

	void UpdateStatus(void);
	char m_strStatus[1024];

	UINT m_nFileSize;
	UINT m_nFileRealSize;
	CString m_strTempFtpName;

	int		m_nTotalFile, m_nCurrNumFile;
	char	m_pFileName[SIZE_MAX_FTP_FILE_PATH];
	VersionInfo m_newVersion;

	HWND m_hMsgWnd;

public:
	CFtpDownloadFiles(CFtpDownload * pFtpI);//Constructor

};

#endif // !defined(AFX_FTPDOWNLOADFILES_H__D3F51062_1292_496B_ADDC_A003B585A4A7__INCLUDED_)
