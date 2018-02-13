// AtumMonitor.h : main header file for the ATUMMONITOR application
//

#if !defined(AFX_ATUMMONITOR_H__757CB96C_D769_4FFC_A1DB_ECE747E3A3D6__INCLUDED_)
#define AFX_ATUMMONITOR_H__757CB96C_D769_4FFC_A1DB_ECE747E3A3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CAtumMonitorApp:
// See AtumMonitor.cpp for the implementation of this class
//

#include "AtumParam.h"

class CAtumMonitorApp : public CWinApp
{
public:
	CAtumMonitorApp();

	void WriteProfile();
	void ReadProfile();
	void WriteSizeProfile();
	void ReadSizeProfile();

	void GetExecuteFileName(char *o_szFileName);
	void SetAppName(char *i_szAppName);
	char		m_szExeFileName[MAX_PATH];

	VersionInfo m_ReinstallVersion;
	VersionInfo m_LatestVersion;
	CString		m_FTPIPAddress;
	CString m_szInfoGenOutPath;		// InfoGen이 생성하는 파일 경로
	CString		m_strGameServerName;		// Game Server Name(DB)
	CString m_szDbgOutBin;			// DbgOut 파일 경로
	CString		m_cstrNoticeFilePath;

	// ClientFTPUpdateManager
	CString	m_szFolderToZipReg;
	CString	m_szOutPutFolderReg;
	CString	m_szStartVersionReg;
	CString	m_szEndVersionReg;
	BOOL	m_bUsePreviousVersionZipFileReg;
	INT		m_nCompressLevelReg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtumMonitorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CAtumMonitorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CString			g_szPreServerIPReg;
extern CString			g_szIMServerIPReg;
extern CString			g_szLogServerIPReg;
extern CString			g_szFieldServerIPReg;
extern CString			g_szNPCServerIPReg;
extern CString			g_szServerIPHistoryReg;

extern int				g_nServerPortReg;
extern vector<string>	g_ServerGroupNameVector;
extern int				g_nWinLeftReg;
extern int				g_nWinRightReg;
extern int				g_nWinTopReg;
extern int				g_nWinBottomReg;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATUMMONITOR_H__757CB96C_D769_4FFC_A1DB_ECE747E3A3D6__INCLUDED_)
