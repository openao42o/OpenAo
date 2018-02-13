// FtpDownload.h: interface for the CFtpDownload class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPDOWNLOAD_H__65AE2999_3761_4D46_80E5_0F4878908F3D__INCLUDED_)
#define AFX_FTPDOWNLOAD_H__65AE2999_3761_4D46_80E5_0F4878908F3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"
#include "AtumProtocol.h"

class CAtumLauncherDlg;

// notify just after downloading 'UpdateListFile'
#define	WM_USER_BASE					(WM_USER + 222)
#define	WM_DOWNLOAD_UPDATELIST_DONE		(WM_USER_BASE + 10)
#define	WM_DOWNLOAD_GAMEFILES_DONE		(WM_USER_BASE + 11)

typedef enum
{
	DOWNLOAD_FILE_TYPE_UPDATELIST = 0,
	DOWNLOAD_FILE_TYPE_GAMEFILES
} DownloadFileType;

class CFtpDownload  
{
public:
	UINT m_flagThreadFinished;
	UINT m_flagAbort;
	CString m_strResult;

	CStringArray	*m_pListCommand;
	DWORD			m_nTimeout;

	CFtpDownload()
	{
		m_pMsgUpdateInfo = 0;
		m_pListCommand = 0;
	}

	void SetMainDlg(CAtumLauncherDlg *pDlg);
	
	//CFtpDownload(HWND hWnd, CStringArray * array, DWORD timeout = 0);//Constructor, by kelovon
	void ExecuteDownload(HWND hWnd, CStringArray * array, DownloadFileType type, MSG_PC_CONNECT_UPDATE_INFO *pMsg,	DWORD timeout = 0);
	static UINT DownloadFunction(LPVOID lParam);// Thread function

	HWND						m_hMsgWnd;
	DownloadFileType			m_fType;
	MSG_PC_CONNECT_UPDATE_INFO	*m_pMsgUpdateInfo;
	CAtumLauncherDlg			*m_pMainDlg;

};

#endif // !defined(AFX_FTPDOWNLOAD_H__65AE2999_3761_4D46_80E5_0F4878908F3D__INCLUDED_)
