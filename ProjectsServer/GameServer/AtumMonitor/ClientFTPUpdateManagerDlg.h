// ClientFTPUpdateManagerDlg.h : header file
//

#if !defined(AFX_CLIENTFTPUPDATEMANAGERDLG_H__E72535C8_7745_4068_B085_C8B5D382DEE9__INCLUDED_)
#define AFX_CLIENTFTPUPDATEMANAGERDLG_H__E72535C8_7745_4068_B085_C8B5D382DEE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <vector>
#include <algorithm>
#include "AtumParam.h"
#include "resource.h"

using namespace std;

#define ATUM_DEFAULT_COMPRESSION_LEVEL	6

/////////////////////////////////////////////////////////////////////////////
// CClientFTPUpdateManagerDlg dialog

class CClientFTPUpdateManagerDlg : public CDialog
{
// Construction
public:
	CClientFTPUpdateManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientFTPUpdateManagerDlg)
	enum { IDD = IDD_CLIENTFTPUPDATEMANAGER_DIALOG };
	CProgressCtrl	m_mainProgressBar;
	CString	m_szFolderToZip;
	CString	m_mainProgressText;
	CString	m_szStartVersion;
	CString	m_szEndVersion;
	CString	m_szOutPutFolder;
	BOOL	m_bUsePreviousVersionZipFile;
	int		m_nCompressLevel;
	CString	m_ctl_strVersionListFileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientFTPUpdateManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void AddFiles(int &i_nLevel, CString i_szDir, CString i_szRootDir, set<CString> *i_pListFiles, vector<VersionInfo*> *i_pVectorVersion, BOOL i_bIncludeDir = TRUE);
	void SetMainProgressText(const char* i_szText);
	CString GetVersionDirEliminatedPath(CString &i_szFullPathString);
	void SetVersions();
	void MakeFileListAndVersionList();
	CString MakeZipFile(VersionInfo *i_pStartVersion, VersionInfo *i_pEndVersion);
	BOOL CheckVersionValidity(VersionInfo *i_pStartVersion, VersionInfo *i_pEndVersion);
	CString GetPreviousZipFileNameWithFullPath(VersionInfo *i_pStartVersion, VersionInfo *i_pEndVersion);
	VersionInfo* GetNextVersion(vector<VersionInfo*> *i_pSortedVersionList, VersionInfo *i_pVersion);
	VersionInfo* GetPreviousVersion(vector<VersionInfo*> *i_pSortedVersionList, VersionInfo *i_pVersion);

	BOOL MakeVersionListFile(CString i_strOutputDir, CString i_strVersionListFileName);

public:
	void WriteProfile();
	void ReadProfile();

protected:
	HICON m_hIcon;
	set<CString>			m_ListFiles;
	vector<VersionInfo*>	m_VectorVersionInfo;
	VersionInfo				m_StartVersion;
	VersionInfo				m_EndVersion;

	// Generated message map functions
	//{{AFX_MSG(CClientFTPUpdateManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOk();
	afx_msg void OnButtonFolderToZipSelect();
	afx_msg void OnButtonZipFolder();
	afx_msg void OnButtonOutputFolderSelect();
// 2006-12-28 by cmkwon, 사용하지 않음
//	afx_msg void OnButtonTest();
	afx_msg void OnSelchangeComboCompressLevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTFTPUPDATEMANAGERDLG_H__E72535C8_7745_4068_B085_C8B5D382DEE9__INCLUDED_)
