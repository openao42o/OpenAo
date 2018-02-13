// FtpDownload.cpp: implementation of the CFtpDownload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumLauncherDlg.h"
#include "FtpDownload.h"
#include "FtpDownloadFiles.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CFtpDownload::CFtpDownload(HWND hWnd, CStringArray * array, DWORD timeout)
{
	m_hMsgWnd = hWnd;
	m_listCommand = array;
	m_nTimeout = timeout;
	m_strResult ="";
	m_flagAbort = FALSE;
	m_flagThreadFinished = FALSE;//When this == TRUE the other thread is finished with its job

	CWinThread * dl_thread = AfxBeginThread(DownloadFunction, this);//Start uploading thread

	if (m_strResult.IsEmpty()==FALSE) return;//Then an exception was thrown and caught in the other thread!!
	if (m_flagAbort) m_strResult = "Downloading is canceled";//Now set up result string
	else m_strResult = "Download finished";
	return;
}
*/

void CFtpDownload::ExecuteDownload(HWND hWnd, CStringArray * pCommands, DownloadFileType type, MSG_PC_CONNECT_UPDATE_INFO *pMsg, DWORD timeout)
{
	m_pMsgUpdateInfo = pMsg;
	m_fType = type;
	m_hMsgWnd = hWnd;
	m_pListCommand = pCommands;
	m_nTimeout = timeout;
	m_strResult ="";
	m_flagAbort = FALSE;
	m_flagThreadFinished = FALSE;//When this == TRUE the other thread is finished with its job

/*	m_dlgProgress.Create();//Create the progress dialog box.
	m_dlgProgress.ShowWindow(TRUE);*/

	CWinThread * dl_thread = AfxBeginThread(DownloadFunction, this);//Start uploading thread

/*	while (m_flagThreadFinished == FALSE)
	   if (m_dlgProgress.CheckCancelButton())
		   m_flagAbort = TRUE;//If cancel pushed let other thread know it's time to split*/

	if (m_strResult.IsEmpty()==FALSE) return;//Then an exception was thrown and caught in the other thread!!
	if (m_flagAbort) m_strResult = STRMSG_S_ATUMLAUNCHER_0008;//Now set up result string
	else m_strResult = STRMSG_S_ATUMLAUNCHER_0009;
	return;
}

void CFtpDownload::SetMainDlg(CAtumLauncherDlg *pDlg)
{
	m_pMainDlg = pDlg;
}

// static thread function
UINT CFtpDownload::DownloadFunction(LPVOID lParam)
//UINT DownloadFunction(LPVOID lParam)
{
	CFtpDownload * pFtpI = (CFtpDownload *)lParam;//I want a try block to catch any exceptions
	try
	{
		CFtpDownloadFiles dl(pFtpI);
	} //This performs the downloading
	catch(char * str)//If a file can't be opened this is thrown
	{ 		pFtpI->m_strResult = str; 		}
	catch (CFileException * pEx)
	{ 		pFtpI->m_strResult.Format("File Error %d %s", pEx->m_cause, pEx->m_strFileName); 		}
	catch (CInternetException* pEx)
	{
		pFtpI->m_strResult.Format("Internet Exception Error %d", pEx->m_dwError);
		if (pEx->m_dwError == ERROR_INTERNET_EXTENDED_ERROR)
		{
			char temp_str[1024];
			DWORD length = 1024;
			DWORD temp_int = 0;
			::InternetGetLastResponseInfo(&temp_int, temp_str, &length);
			pFtpI->m_strResult += temp_str;
		}
	}

	if (pFtpI->m_pMainDlg->m_bCancelFlag)
	{
		util::del(pFtpI->m_pListCommand);
		return -1;
	}

	pFtpI->m_flagThreadFinished = TRUE;//Let the main thread know we finished

	util::del(pFtpI->m_pListCommand);

	if (pFtpI->m_fType == DOWNLOAD_FILE_TYPE_UPDATELIST)
	{
		SendMessage(pFtpI->m_hMsgWnd, WM_DOWNLOAD_UPDATELIST_DONE, (WPARAM)pFtpI->m_pMsgUpdateInfo, 0);
	} else if (pFtpI->m_fType == DOWNLOAD_FILE_TYPE_GAMEFILES)
	{
		VersionInfo newVersion;
		newVersion.GetVersion()[0] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[0];
		newVersion.GetVersion()[1] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[1];
		newVersion.GetVersion()[2] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[2];
		newVersion.GetVersion()[3] = pFtpI->m_pMsgUpdateInfo->UpdateVersion[3];

		pFtpI->m_pMainDlg->m_CurrentVersion.SetVersion(newVersion.GetVersionString());

		util::del(pFtpI->m_pMsgUpdateInfo);

		SendMessage(pFtpI->m_hMsgWnd, WM_DOWNLOAD_GAMEFILES_DONE, (WPARAM)pFtpI, 0);
	}

	return 0;//Don't care about return value
}
