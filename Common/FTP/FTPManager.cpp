#include "stdafx.h"
#include "FTPManager.h"
#include "AtumError.h"		// 2007-01-05 by cmkwon

DWORD WINAPI DownloadThread(LPVOID lpParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef DBGOUT
#define DBGOUT	((void)0)
#endif

BOOL CFTPManager::ConnectToServer(const char *i_pServerName, int i_nServerPort, const char *i_pUserName, const char *i_pPassword)
{
	m_hInternet = InternetOpen("Atum Pre Server", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (m_hInternet == NULL)
	{
		DbgOut("InternetOpen ERROR: %d\r\n", GetLastError());
		return FALSE;
	}

	m_hFtpConnect = InternetConnect(m_hInternet, i_pServerName, i_nServerPort, i_pUserName, i_pPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (m_hFtpConnect == NULL)
	{
		DbgOut("InternetConnect ERROR: %d\r\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL CFTPManager::ChangeCurrentDirectory(const char *i_pDirName)
{
	// change directory
	if ( FtpSetCurrentDirectory(m_hFtpConnect, i_pDirName) == FALSE )
	{
		// check: error
		DbgOut("Changer Dir ERROR: %d\r\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

char* CFTPManager::GetCurrentDirectory()
{
	return NULL;
}

HINTERNET CFTPManager::GetFileInfo(const char *i_pRemoteFileName, WIN32_FIND_DATA *o_pRemoteFileInfo)
{
	HINTERNET hFile = NULL;
	memset(o_pRemoteFileInfo, 0, sizeof(WIN32_FIND_DATA));

	if ( !(hFile = FtpFindFirstFile (m_hFtpConnect, i_pRemoteFileName, o_pRemoteFileInfo, 0, 0) ) )
	{
		if (GetLastError()  == ERROR_NO_MORE_FILES)
		{
			// 해당 파일이 존재하지 않음
			return NULL;
		}
		else
		{
			DbgOut("Get File Info ERROR: %d\n", GetLastError());
			return NULL;
		}
	}

	// close handle
	InternetCloseHandle(hFile);

	return hFile;
}

BOOL CFTPManager::DownloadFile(const char *i_pRemoteFileName, const char *i_pLocalFileName /*= NULL*/, CProgressCtrl *i_pProgressControl /*=NULL*/, HWND i_hWnd /*=NULL*/)
{
	if (i_hWnd)
	{
		if (m_threadDownload || i_hWnd == NULL || i_pRemoteFileName == NULL) return false;

		m_hUpdateWindow = i_hWnd;
		
		strcpy(m_szRemoteFileName, i_pRemoteFileName);
		
		if (i_pLocalFileName) strcpy(m_szLocalFileName, i_pLocalFileName);

		try
		{
			//unsigned thrdaddr;
			m_threadDownload = new thread { DownloadThread, LPVOID(this) }; // ch BEGINTHREADEX (NULL, 0, DownloadThread, LPVOID(this), 0, &thrdaddr);
		}
		catch (const system_error& error)
		{
			return false;
		}
		
		return true;
	}

	////////////////////////////
	// open remote file
	HINTERNET hRemoteFile = FtpOpenFile(m_hFtpConnect, i_pRemoteFileName, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);
	if (hRemoteFile == NULL)
	{
		return FALSE;
	}

	////////////////////////////
	// open local file
	CFile fileObject;
	CFileException ex;
	CString szLocalFileName;
	if (i_pLocalFileName == NULL)
	{
		szLocalFileName = i_pRemoteFileName;
		if (szLocalFileName.ReverseFind('\\') != -1)
		{
			szLocalFileName = szLocalFileName.Right(szLocalFileName.ReverseFind('\\'));
		}
		else if (szLocalFileName.ReverseFind('/') != -1)
		{
			szLocalFileName = szLocalFileName.Right(szLocalFileName.GetLength() - szLocalFileName.ReverseFind('/') - 1);
		}
	}
	else
	{
		szLocalFileName = i_pLocalFileName;
	}

	if (fileObject.Open(szLocalFileName, CFile::modeCreate | CFile::modeWrite, &ex) == FALSE)
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		DbgOut("Unable to create file: %s\r\n", szError);
		return FALSE;
	}

	// download file
	DWORD nDownloadBufferSize = 4096;
	char *buffer = new char[nDownloadBufferSize];
	DWORD amount_read = nDownloadBufferSize;
	UINT total_read = 0;

	while (TRUE)
	{
		if (!InternetReadFile(hRemoteFile, buffer, nDownloadBufferSize, &amount_read))
		{
			// error
			DbgOut("InternetReadFile ERROR: %d\n", GetLastError());
			return FALSE;
		}

		if(0 == amount_read)
		{// 2006-06-30 by cmkwon, 완료시 0 Bytes를 리턴한다.

			break;
		}

		fileObject.Write(buffer, amount_read);	// Write this to our data file
		total_read += amount_read;

		// progress control 처리
		if (i_pProgressControl != NULL)
		{
			i_pProgressControl->SetPos(total_read);
		}

		// 2009-01-21 by cmkwon, 자동 업데이트 다운로드 속도 개선 - FTP, Sleep(10)을 주석 처리함.
		//Sleep(10);
	}
	if (i_pProgressControl != NULL)
	{// 2006-06-30 by cmkwon, 프로그레스바를 100%로 설정
		int nLower, nUpper;
		i_pProgressControl->GetRange(nLower, nUpper);
		i_pProgressControl->SetPos(nUpper);
	}

	delete buffer;				// delete buffer
	fileObject.Close();			// close resources
// 2006-07-03 by cmkwon
//	InternetCloseHandle(hRemoteFile);
	return TRUE;
}

BOOL CFTPManager::DownloadFileByThread(const char *i_pRemoteFileName, const char *i_pLocalFileName, HWND i_hWnd)
{
	if(NULL == i_hWnd
		|| 0 == strcmp(i_pRemoteFileName, ""))
	{
		return FALSE;
	}

	////////////////////////////
	// open remote file
	HINTERNET hRemoteFile = FtpOpenFile(m_hFtpConnect, i_pRemoteFileName, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, NULL);
	if (hRemoteFile == NULL)
	{
		::PostMessage(i_hWnd, WM_UPDATEFILE_DOWNLOAD_ERROR, ERR_UPDATE_FILE_NOT_FOUND, 0);
		return FALSE;
	}

	////////////////////////////
	// open local file
	CFile fileObject;
	CFileException ex;
	CString szLocalFileName;
	if (i_pLocalFileName == NULL)
	{
		szLocalFileName = i_pRemoteFileName;
		if (szLocalFileName.ReverseFind('\\') != -1)
		{
			szLocalFileName = szLocalFileName.Right(szLocalFileName.ReverseFind('\\'));
		}
		else if (szLocalFileName.ReverseFind('/') != -1)
		{
			szLocalFileName = szLocalFileName.Right(szLocalFileName.GetLength() - szLocalFileName.ReverseFind('/') - 1);
		}
	}
	else
	{
		szLocalFileName = i_pLocalFileName;
	}

	if (fileObject.Open(szLocalFileName, CFile::modeCreate | CFile::modeWrite, &ex) == FALSE)
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		DbgOut("Unable to create file: %s\r\n", szError);
		::PostMessage(i_hWnd, WM_UPDATEFILE_DOWNLOAD_ERROR, ERR_LOCAL_FILE_CREATE_FAIL, 0);
		return FALSE;
	}

	// download file
	DWORD nDownloadBufferSize = 4096;
	char *buffer = new char[nDownloadBufferSize];
	DWORD amount_read = nDownloadBufferSize;
	UINT total_read = 0;

	while (amount_read == nDownloadBufferSize)
	{
		if (!InternetReadFile(hRemoteFile, buffer, nDownloadBufferSize, &amount_read))
		{
			// error
			DbgOut("InternetReadFile ERROR: %d\n", GetLastError());
			::PostMessage(i_hWnd, WM_UPDATEFILE_DOWNLOAD_ERROR, ERR_UPDATE_FILE_DOWNLOADING_FAIL, 0);
			return FALSE;
		}
		if(0 == amount_read)
		{// 2006-06-30 by cmkwon
			break;
		}

		fileObject.Write(buffer, amount_read);	// Write this to our data file

		total_read += amount_read;

		::PostMessage(i_hWnd, WM_UPDATEFILE_DOWNLOAD_PROGRESS, total_read, 0);
		if(m_bDownloadThreadCancelFlag)
		{		
			delete buffer;			// delete buffer
			fileObject.Close();		// close resources
			// 2006-07-03 by cmkwon
			//InternetCloseHandle(hRemoteFile);
			return FALSE;
		}
	}

	delete buffer;				// delete buffer
	fileObject.Close();			// close resources
// 2006-07-03 by cmkwon
//	InternetCloseHandle(hRemoteFile);
	::PostMessage(i_hWnd, WM_UPDATEFILE_DOWNLOAD_OK, 0, 0);
	return TRUE;
}

DWORD WINAPI DownloadThread(LPVOID lpParam)
{
	auto pFTP = reinterpret_cast<CFTPManager*>(lpParam);
	
	if (pFTP->m_szLocalFileName[0] == '\0')
		
		pFTP->DownloadFileByThread(pFTP->m_szRemoteFileName, nullptr, pFTP->m_hUpdateWindow);

	else pFTP->DownloadFileByThread(pFTP->m_szRemoteFileName, pFTP->m_szLocalFileName, pFTP->m_hUpdateWindow);

	
	pFTP->CloseConnection();

	return 0x20;
}

int CFTPManager::GetFileSize(const char *i_pRemoteFileName, HINTERNET &i_hFile)
{
	// Find file info
	WIN32_FIND_DATA fileInfo;
	i_hFile = GetFileInfo(i_pRemoteFileName, &fileInfo);
	if (i_hFile == NULL)
	{
		return -1;
	}

	// close handle
	InternetCloseHandle(i_hFile);

	// set file length
	return fileInfo.nFileSizeLow;
}

bool CFTPManager::CloseConnection()
{
	if (m_hFtpConnect)
	{
// 2006-07-31 by cmkwon
//		InternetCloseHandle(m_hFtpConnect);
		m_hFtpConnect		= NULL;
	}

	if (m_hInternet)
	{
// 2006-07-31 by cmkwon
//		InternetCloseHandle(m_hInternet);
		m_hInternet			= NULL;
	}

	return true;
}

