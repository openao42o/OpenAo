// VTCGuardManager.h: interface for the CVTCGuardManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VTCGUARDMANAGER_H__CE968D48_DF32_4AB3_941B_4A03C6FAF61E__INCLUDED_)
#define AFX_VTCGUARDMANAGER_H__CE968D48_DF32_4AB3_941B_4A03C6FAF61E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVTCGuardManager  
{
public:
	CVTCGuardManager();
	virtual ~CVTCGuardManager();

	HANDLE GetUpdateThreadHandle(void);
	void SetUpdateThreadCancelFlag(BOOL i_bCancelFlag);
	BOOL ThreadEnd(DWORD i_dwWaitTimems);

	Err_t Update(BOOL i_bUseThread=FALSE, HWND i_hUpdateWindow=0);
	Err_t _Update(void);

	void Run(char *i_szAppPath, char *i_szCmdParam);

	void PostMessage2UpdateWindow(UINT i_uiMsgTy, WPARAM i_wParam, LPARAM i_lParam);

private:
	HANDLE		m_hUpdateThread;
	BOOL		m_bUpdateThreadCancelFlag;

	HWND		m_hUpdateWindow;

	char		m_szVTCAutoUpdateMsg[512];
	char		m_szFolderGame[MAX_PATH];
};

#endif // !defined(AFX_VTCGUARDMANAGER_H__CE968D48_DF32_4AB3_941B_4A03C6FAF61E__INCLUDED_)
