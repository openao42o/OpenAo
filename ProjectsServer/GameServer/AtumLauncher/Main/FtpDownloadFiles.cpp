// FtpDownloadFiles.cpp: implementation of the CFtpDownloadFiles class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FtpDownloadFiles.h"
#include "AtumLauncherDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DL_BUFFER_SIZE 4096

CFtpDownloadFiles::CFtpDownloadFiles(CFtpDownload * pFtpI)
{
	m_pFtpI = pFtpI;
	m_hMsgWnd = pFtpI->m_hMsgWnd;
	m_pFtpConnection = NULL;
	if (m_pFtpI->m_nTimeout)
	m_csInternet.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, m_pFtpI->m_nTimeout);//I like 3000

	int sizeOfArray = m_pFtpI->m_pListCommand->GetSize();
	for (int i = 3; i < sizeOfArray; i+=2)
	{
		char buff[SIZE_MAX_FTP_FILE_PATH];		// reused
		m_nTotalFile =  (sizeOfArray-3)/2;
		m_nCurrNumFile = (i-1)/2;
		m_pFtpI->m_pMainDlg->GetFileName((char*)(LPCTSTR)m_pFtpI->m_pListCommand->GetAt(i), buff);
		util::strncpy(m_pFileName, buff, SIZE_MAX_FTP_FILE_PATH);
		// set texts in dialog
		//sprintf(buff, "(%d/%d)", m_nCurrNumFile, m_nTotalFile);
		//m_pFtpI->m_pMainDlg->SetFileNumText(buff);

		m_newVersion.GetVersion()[0] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[0];
		m_newVersion.GetVersion()[1] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[1];
		m_newVersion.GetVersion()[2] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[2];
		m_newVersion.GetVersion()[3] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[3];
		sprintf(buff, STRMSG_S_ATUMLAUNCHER_0010,
					m_newVersion.GetVersionString(), m_pFileName, m_nCurrNumFile, m_nTotalFile);
		m_pFtpI->m_pMainDlg->SetProgressGroupText(buff);

		BOOL ret = ReadFile(m_pFtpI->m_pListCommand->GetAt(i), m_pFtpI->m_pListCommand->GetAt(i+1));
		if (!ret)
		{
			break;
		}
	}

	m_csInternet.Close();
}

// must be set B4 calling this function
BOOL CFtpDownloadFiles::ReadFile(CString &source, CString &dest)
{
	if (m_pFtpI->m_pMainDlg->m_bCancelFlag)
	{
		return FALSE;// Cancel button
	}


//	m_pFtpI->m_dlgProgress.SetHeader(source);
//	wsprintf(m_strStatus, "Connecting to %s", m_pFtpI->m_pListCommand->GetAt(0));
//	UpdateStatus();

	////////////////////////////
	// connect to ftp server
	m_pFtpConnection
		= m_csInternet.GetFtpConnection(m_pFtpI->m_pListCommand->GetAt(0),
										m_pFtpI->m_pListCommand->GetAt(1),
										m_pFtpI->m_pListCommand->GetAt(2)
		  );

//	strcpy(m_strStatus, source);
//	UpdateStatus();

	////////////////////////////////////////////////////////////////////
	// check if remote file exists, if exist set m_nFileRealSize
	if (FtpFileExists(source) == FALSE)
	{
		return FALSE;
	}

	m_pFtpI->m_pMainDlg->SetProgressRange(m_nFileRealSize);

	////////////////////////////
	// open local file
	if (m_fileObject.Open(dest, CFile::modeCreate | CFile::modeWrite, NULL) == FALSE)
	{
		wsprintf(m_strStatus, STRMSG_S_ATUMLAUNCHER_0011, dest);
		AfxMessageBox(m_strStatus);
		throw m_strStatus;
	}

//	m_pFtpI->m_dlgProgress.SetHeader(m_strTempFtpName);
//	wsprintf(m_strStatus, "Opening %s", m_strTempFtpName);
//	UpdateStatus();
	if (m_pFtpI->m_pMainDlg->m_bCancelFlag)
	{
		return FALSE;
	}


	////////////////////////////
	// open remote file
	CInternetFile*  ifp = m_pFtpConnection->OpenFile(source);
	int fileSize = m_nFileRealSize;
	char buffer[DL_BUFFER_SIZE];
	UINT amount_read = DL_BUFFER_SIZE;
	UINT total_read = 0;
	while (amount_read == DL_BUFFER_SIZE)
	{
		amount_read = ifp->Read(buffer, DL_BUFFER_SIZE);
		m_fileObject.Write(buffer, amount_read);//Write this to our data file

		// ysw : total_read
		total_read += amount_read;


		//wsprintf(m_strStatus, "%d of %d  bytes read", total_read, m_nFileSize);
		m_pFtpI->m_pMainDlg->SetProgressPos(total_read);
		if (fileSize != 0)
		{
			char percentBuf[256];
			sprintf(percentBuf, "%d%%", (int)((float)total_read/fileSize*100));
			m_pFtpI->m_pMainDlg->SetFileNumText(percentBuf);

			// ysw : Ãß°¡
			m_pFtpI->m_pMainDlg->Set_Cur_Percent((int)((float)total_read/fileSize*100));


			sprintf(percentBuf, STRMSG_S_ATUMLAUNCHER_0012,
					m_newVersion.GetVersionString(), m_pFileName, m_nCurrNumFile, m_nTotalFile);
			m_pFtpI->m_pMainDlg->SetProgressGroupText(percentBuf);
		}

		if (m_pFtpI->m_pMainDlg->m_bCancelFlag)
		{
			return FALSE;
		}
	}

	m_pFtpI->m_pMainDlg->SetProgressPos(m_nFileRealSize);
	m_pFtpI->m_pMainDlg->SetFileNumText("100%");

	// close file
	m_fileObject.Close();

	//wsprintf(m_strStatus, "Closing connection to %s", m_pFtpI->m_pListCommand->GetAt(0));
	UpdateStatus();
	m_pFtpConnection->Close();
	delete m_pFtpConnection;

	return TRUE;
}

void CFtpDownloadFiles::UpdateStatus(void)
{
//	m_pFtpI->m_dlgProgress.SetStatus(m_strStatus);
	return;
}

BOOL CFtpDownloadFiles::FtpFileExists(CString &source)
{
	wsprintf(m_strStatus, STRMSG_S_ATUMLAUNCHER_0013, source);
	UpdateStatus();

	if (m_pFtpI->m_pMainDlg->m_bCancelFlag)
		return FALSE;
	CFtpFileFind finder(m_pFtpConnection);

	if (m_pFtpI->m_pMainDlg->m_bCancelFlag)
		return FALSE;
	if (finder.FindFile(source) == FALSE)
	{
		FileNotFound(source);
		return FALSE;
	}

	if (m_pFtpI->m_pMainDlg->m_bCancelFlag)
		return FALSE;
	finder.FindNextFile();
	m_strTempFtpName = "";
	m_strTempFtpName = finder.GetFilePath();
	if (m_strTempFtpName.IsEmpty())
		m_strTempFtpName = source;
	m_nFileSize = 0;
	m_nFileSize = m_nFileRealSize = finder.GetLength();
	if (m_nFileSize < DL_BUFFER_SIZE)
		m_nFileSize = 0;

	return TRUE;
}

void CFtpDownloadFiles::FileNotFound(CString & source)
{
	wsprintf(m_strStatus, "File not found on Server: %s", source);
	UpdateStatus();
	AfxMessageBox(m_strStatus);
	return;
}
