// ClientFTPUpdateManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AtumMonitor.h"
#include "ClientFTPUpdateManagerDlg.h"
#include "ZipArchive/ZipArchive.h"
#include "BrowseForFolder.h"
#include "DbgOut_C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientFTPUpdateManagerDlg dialog

CClientFTPUpdateManagerDlg::CClientFTPUpdateManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientFTPUpdateManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientFTPUpdateManagerDlg)
	m_szFolderToZip = _T("");
	m_mainProgressText = _T("");
	m_szStartVersion = _T("");
	m_szEndVersion = _T("");
	m_szOutPutFolder = _T("");
	m_bUsePreviousVersionZipFile = FALSE;
	m_nCompressLevel = ATUM_DEFAULT_COMPRESSION_LEVEL;
	m_ctl_strVersionListFileName = _T("versionlist.ver");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientFTPUpdateManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientFTPUpdateManagerDlg)
	DDX_Control(pDX, IDC_PROGRESS_GLOBAL, m_mainProgressBar);
	DDX_Text(pDX, IDC_EDIT_FOLDER_TO_ZIP, m_szFolderToZip);
	DDX_Text(pDX, IDC_STATIC_PROGRESS_TEXT, m_mainProgressText);
	DDX_Text(pDX, IDC_EDIT_START_VERSION, m_szStartVersion);
	DDX_Text(pDX, IDC_EDIT_END_VERSION, m_szEndVersion);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_FOLDER, m_szOutPutFolder);
	DDX_Check(pDX, IDC_CHECK_USE_PREVIOUS_ZIP_FILE, m_bUsePreviousVersionZipFile);
	DDX_CBIndex(pDX, IDC_COMBO_COMPRESS_LEVEL, m_nCompressLevel);
	DDX_Text(pDX, IDC_EDIT_VERSION_LIST_FILE_NAME, m_ctl_strVersionListFileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientFTPUpdateManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CClientFTPUpdateManagerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_FOLDER_TO_ZIP_SELECT, OnButtonFolderToZipSelect)
	ON_BN_CLICKED(IDC_BUTTON_ZIP_FOLDER, OnButtonZipFolder)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_FOLDER_SELECT, OnButtonOutputFolderSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPRESS_LEVEL, OnSelchangeComboCompressLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientFTPUpdateManagerDlg message handlers

BOOL CClientFTPUpdateManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

//	// Set the icon for this dialog.  The framework does this automatically
//	//  when the application's main window is not a dialog
//	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientFTPUpdateManagerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientFTPUpdateManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientFTPUpdateManagerDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
//	CZipArchive fin;
//	CZipArchive fout;
//
//	try
//	{
//		// make zip file
//		fin.Open("ttt.zip", CZipArchive::create);
//
//		fin.AddNewFile("temp", 0, true);
//
//		fin.AddNewFile("temp/ReadMe.txt", 1, true);
//
//		fin.Close();
//
//		// extract zip file
//		fout.Open("Projects_20040525_merged.zip", CZipArchive::open);
//
//		for (int i = 0; i < fout.GetNoEntries(); i++)
//		{
//			fout.ExtractFile(i, (LPCTSTR)m_szFolderToZip, true);
//		}
//
//		fout.Close();
//	}
//	catch (CException *e) {
//		e->Delete();
//	}

	WriteProfile();
}

void CClientFTPUpdateManagerDlg::OnButtonFolderToZipSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	WriteProfile();

	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = "Select folder that files for update";
	CString sz;
	if (!bf.GetFolder(sz, (LPCSTR)m_szFolderToZip))
		return;

	GetDlgItem(IDC_EDIT_FOLDER_TO_ZIP)->SetWindowText(sz);

	UpdateData();
	WriteProfile();
}

//void CClientFTPUpdateManagerDlg::OnButtonFileToZipSelect() 
//{
//	// TODO: Add your control notification handler code here
//	bool bOpen = FALSE;
//
//	CFileDialog fd(bOpen, _T("zip"), bOpen ? NULL : _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Zip Files (*.zip)|*.zip|All Files (*.*)|*.*||"));
//	fd.m_ofn.lpstrInitialDir = m_szFolderToZip;
//	if (fd.DoModal() != IDOK)
//		return;
//	m_szTargetZipFilesFolder = fd.GetPathName();
//
//	GetDlgItem(IDC_EDIT_FILE_TO_ZIP)->SetWindowText(m_szTargetZipFilesFolder);
//
//	UpdateData();
//	WriteProfile();
//
//	return;
//
//}

void CClientFTPUpdateManagerDlg::MakeFileListAndVersionList()
{
	UpdateData();
	SetVersions();
	WriteProfile();

	// reset lists
	m_ListFiles.clear();
	m_VectorVersionInfo.clear();
	vector<VersionInfo*>::iterator itrVersion;

	// make file list & version list
	int nLevel = 0;
	AddFiles(nLevel, m_szFolderToZip, m_szFolderToZip, &m_ListFiles, &m_VectorVersionInfo, TRUE);

	// sort version list
	sort(m_VectorVersionInfo.begin(), m_VectorVersionInfo.end());

	// print version list
	itrVersion = m_VectorVersionInfo.begin();
	while(m_VectorVersionInfo.end() != itrVersion)
	{
		VersionInfo *pVersion = *itrVersion;
		DBGOUT("Version: %s\r\n", pVersion->GetVersionString());
		itrVersion++;
	}
}

CString CClientFTPUpdateManagerDlg::MakeZipFile(VersionInfo *i_pStartVersion, VersionInfo *i_pEndVersion)
{
	///////////////////////////////////////////////////////////////////////////
	// 주의: file list & version list를 생성된 이후에 실행된다고 가정함!
	///////////////////////////////////////////////////////////////////////////

	CZipArchive fin;

	VersionInfo tmpStartVersion(i_pStartVersion->GetVersionString());	// 이전 버전의 zip 파일이 있을 경우 재활용하기 위해 쓰임

	// just file name
	CString szJustFileName = i_pStartVersion->GetVersionString();
	szJustFileName = szJustFileName + "_" + i_pEndVersion->GetVersionString() + ".zip";

	// pull path name
	m_szOutPutFolder.TrimRight(_T("\\"));
	CString szZipFileName = m_szOutPutFolder + "\\" + szJustFileName;

	try
	{
		DBGOUT("\r\nMaking New Zip File From %s To %s...\r\n", i_pStartVersion->GetVersionString(), i_pEndVersion->GetVersionString());

		CString ProgressString;

		// set file name
		if (!CheckVersionValidity(i_pStartVersion, i_pEndVersion))
		{
			MessageBox("Appropriate version does not exist.");
			return "";
		}

		// set current directory
		SetCurrentDirectory(m_szFolderToZip);

		// make zip file
		CString szPrevFileWithFullPath = GetPreviousZipFileNameWithFullPath(i_pStartVersion, i_pEndVersion);
		FILE*fp = NULL;
		errno_t no = fopen_s(&fp, szPrevFileWithFullPath, "rb");
		if (!no && m_bUsePreviousVersionZipFile)
		{
			fclose(fp);
			DeleteFile((LPCSTR)szZipFileName);
			if (!CopyFile((LPCSTR)szPrevFileWithFullPath, (LPCSTR)szZipFileName, FALSE))
			{
				return "";
			}
			fin.Open(szZipFileName, CZipArchive::zipOpen);
			tmpStartVersion.SetVersion(i_pEndVersion->GetVersionString());
			DBGOUT("Using Previous Zip File: %s\r\n", szPrevFileWithFullPath);
		}
		else
		{
			fin.Open(szZipFileName, CZipArchive::zipCreate);
		}
		fin.EnableFindFast();

		int nAddCount = 0;
		int nReplaceCount = 0;
		m_mainProgressBar.SetRange32(0, m_ListFiles.size()-m_VectorVersionInfo.size());
		for (int i = 0; i < m_VectorVersionInfo.size(); i++)
		{
			// set current version
			VersionInfo *pCurrVersion = m_VectorVersionInfo[i];
			if (tmpStartVersion >= *pCurrVersion
				|| *i_pEndVersion < *pCurrVersion)
			{
				DBGOUT("Skipping Version: %s\r\n", pCurrVersion->GetVersionString());
				continue;
			}
#ifdef _DEBUG
			DBGOUT("Adding Version: %s\r\n", pCurrVersion->GetVersionString());
#endif

			// adding files
			set<CString>::iterator itr = m_ListFiles.begin();
			while (itr != m_ListFiles.end()) {
				CString fileName = *itr;

				CString szFilePathToAdd = GetVersionDirEliminatedPath(fileName);
				if (szFilePathToAdd != "")
				{
					// 다른 버전이면 넘김
					VersionInfo fileVersion((LPCSTR)fileName.Left(fileName.Find("\\")));
					if (*pCurrVersion != fileVersion)
					{
						itr++;
						continue;
					}

					// 파일이 이미 존재하면 지우고 다시 추가!
					int nFileIndex = fin.FindFile(szFilePathToAdd);

					if ( nFileIndex != -1) {
						// 한번 추가된 디렉터리는 무시함
						if ((fin.GetFileInfo(nFileIndex))->IsDirectory())
						{
							itr++;
							continue;
						}
						--nAddCount;
						fin.RemoveFile(nFileIndex);
#ifdef _DEBUG
						DBGOUT("Deleting File: %s\r\n", (LPCSTR)*itr);
#endif
						++nReplaceCount;
					}

					// add file
					fin.AddNewFile(szFilePathToAdd, m_nCompressLevel, true);
#ifdef _DEBUG
					DBGOUT("Adding File: %s\r\n", (LPCSTR)*itr);
#endif

					// set progress bar
					m_mainProgressBar.SetPos(++nAddCount);

					// set progress text
					ProgressString = szJustFileName + ", Adding File: " + fileName;
					SetMainProgressText((LPCSTR)ProgressString);

					// redraw dialog box
					InvalidateRect(NULL, TRUE);
				}
				else
				{
					CString newVersionDir = m_szFolderToZip + "\\" + fileName;
					SetCurrentDirectory(newVersionDir);
				}

				itr++;
			}
		}

		fin.Close();

		m_mainProgressBar.SetRange(0,100);
		m_mainProgressBar.SetPos(100);

		// redraw dialog box
		InvalidateRect(NULL, TRUE);

		char szText[256];
		sprintf(szText, "Adding(or Replacing) %d File(s) Completed", nAddCount+nReplaceCount);
		SetMainProgressText(szText);
	}
	catch (CException *e) {
		e->Delete();
	}

	return szZipFileName;
}

void CClientFTPUpdateManagerDlg::OnButtonZipFolder() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	m_szFolderToZip.TrimLeft();
	if (m_szFolderToZip == "")
	{
		MessageBox("Input the name of folder to compress!!");
		return;
	}

	m_szOutPutFolder.TrimLeft();
	if (m_szOutPutFolder == "")
	{
		MessageBox("Input the name of folder to display!!");
		return;
	}

	m_szStartVersion.TrimLeft();
	if (m_szStartVersion == "")
	{
		MessageBox("Input beginning version!!");
		return;
	}

	m_szEndVersion.TrimLeft();
	if (m_szEndVersion == "")
	{
		MessageBox("Input end version!!");
		return;
	}

	SetVersions();
	WriteProfile();

	// make file list & version list
	MakeFileListAndVersionList();

	int nSuccessCount = 0;
	for (int i = 0; i < m_VectorVersionInfo.size()-1; i++)
	{
		if (*m_VectorVersionInfo[i] < m_StartVersion)
		{
			continue;
		}

		if (*m_VectorVersionInfo[i] >= m_EndVersion)
		{
			break;
		}

		CString retFileName = MakeZipFile(m_VectorVersionInfo[i], &m_EndVersion);
		if (retFileName != "")
		{
			// 생성 성공
			nSuccessCount++;
			//MessageBox((LPCSTR)(retFileName + "를(을) 성공적으로 압축하였습니다"));
		}
		else
		{
			// 생성 실패
			//MessageBox((LPCSTR)("파일 압축을 실패하였습니다"));
		}
	}

	// delete version list
	vector<VersionInfo*>::iterator itrVersion = m_VectorVersionInfo.begin();
	while(m_VectorVersionInfo.end() != itrVersion)
	{
		delete *itrVersion;
		itrVersion++;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-08 by cmkwon
	if(FALSE == MakeVersionListFile(m_szOutPutFolder, m_ctl_strVersionListFileName))
	{
		return;
	}

	char szText[1024];
	sprintf(szText, "total %d zip file(s) generated...", nSuccessCount);
	SetMainProgressText(szText);

	MessageBox(szText);
}

void CClientFTPUpdateManagerDlg::AddFiles(int &i_nLevel, CString i_szDir, CString i_szRootDir, set<CString> *i_pListFiles, vector<VersionInfo*> *i_pVectorVersion, BOOL i_bIncludeDir /* = TRUE */)
{
	i_szDir.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(i_szDir + _T("\\*"));
	while (b)
	{
		b = ff.FindNextFile();

		CString tmpString = ff.GetFilePath().Mid(i_szRootDir.GetLength());
		tmpString.TrimLeft(_T("\\/"));

		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
#ifdef _DEBUG
			DBGOUT("%d -> D %s\r\n", i_nLevel, (LPCSTR)tmpString);
#endif
			i_pListFiles->insert(tmpString);

			// add version directory
			if (tmpString.Find('\\') == -1)
			{
				i_pVectorVersion->push_back(new VersionInfo((LPCSTR)tmpString));
			}
			AddFiles(++i_nLevel, ff.GetFilePath(), i_szRootDir, i_pListFiles, i_pVectorVersion, i_bIncludeDir);
		}
		else
		{
			// File인 경우
			if (i_nLevel != 0)
			{
#ifdef _DEBUG
				DBGOUT("%d -> F %s\r\n", i_nLevel, (LPCSTR)tmpString);
#endif
				i_pListFiles->insert(tmpString);
			}
		}
	}
	--i_nLevel;
}

void CClientFTPUpdateManagerDlg::SetMainProgressText(const char* i_szText)
{
	CWnd *pWndStatus = GetDlgItem(IDC_STATIC_PROGRESS_TEXT);
	pWndStatus->SetWindowText(i_szText);
}

CString CClientFTPUpdateManagerDlg::GetVersionDirEliminatedPath(CString &i_szFullPathString)
{
	int nPosition = i_szFullPathString.Find('\\');
	if (nPosition == -1)
	{
		return "";
	}

	CString ret = i_szFullPathString.Mid(nPosition+1);

	return ret;
}

void CClientFTPUpdateManagerDlg::SetVersions()
{
	m_StartVersion.SetVersion((LPCSTR)m_szStartVersion);
	m_EndVersion.SetVersion((LPCSTR)m_szEndVersion);
}

BOOL CClientFTPUpdateManagerDlg::CheckVersionValidity(VersionInfo *i_pStartVersion, VersionInfo *i_pEndVersion)
{
	BOOL bRetStartVersion = FALSE;
	BOOL bRetEndVersion = FALSE;

	for (int i = 0; i < m_VectorVersionInfo.size(); i++)
	{
		if (*m_VectorVersionInfo[i] == *i_pStartVersion)
		{
			bRetStartVersion = TRUE;
		}

		if (*m_VectorVersionInfo[i] == *i_pEndVersion)
		{
			bRetEndVersion = TRUE;
		}
	}

	return (bRetStartVersion & bRetEndVersion);
}

void CClientFTPUpdateManagerDlg::OnButtonOutputFolderSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	WriteProfile();

	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = _T("Select folder to output file of update list!!");
	CString sz;
	if (!bf.GetFolder(sz, (LPCSTR)m_szOutPutFolder))
		return;

	GetDlgItem(IDC_EDIT_OUTPUT_FOLDER)->SetWindowText(sz);

	UpdateData();
	WriteProfile();
}

CString CClientFTPUpdateManagerDlg::GetPreviousZipFileNameWithFullPath(VersionInfo *i_pStartVersion, VersionInfo *i_pEndVersion)
{
	VersionInfo *pPrevEndVersion = GetPreviousVersion(&m_VectorVersionInfo, i_pEndVersion);
	if (pPrevEndVersion == NULL)
	{
		return "";
	}

	CString szZipFileName = m_szOutPutFolder + "\\" + i_pStartVersion->GetVersionString();
	szZipFileName = szZipFileName + "_" + pPrevEndVersion->GetVersionString() + ".zip";

	return szZipFileName;
}

VersionInfo* CClientFTPUpdateManagerDlg::GetNextVersion(vector<VersionInfo*> *i_pSortedVersionList, VersionInfo *i_pVersion)
{
	// sorting이 되어 있다고 가정함!
	for (int i = 0; i < i_pSortedVersionList->size(); i++)
	{
		if (*m_VectorVersionInfo[i] == *i_pVersion)
		{
			if (i+1 >= i_pSortedVersionList->size())
			{
				return NULL;
			}
			else
			{
				return (*i_pSortedVersionList)[i+1];
			}
		}
		else if (*(*i_pSortedVersionList)[i] > *i_pVersion)
		{
			return (*i_pSortedVersionList)[i];
		}
	}

	return NULL;
}

VersionInfo* CClientFTPUpdateManagerDlg::GetPreviousVersion(vector<VersionInfo*> *i_pSortedVersionList, VersionInfo *i_pVersion)
{
	// sorting이 되어 있다고 가정함!
	if (*i_pSortedVersionList->back() < *i_pVersion)
	{
		return i_pSortedVersionList->back();
	}

	for (int i = 0; i < i_pSortedVersionList->size(); i++)
	{
		if (*(*i_pSortedVersionList)[i] >= *i_pVersion)
		{
			if (i-1 < 0)
			{
				return NULL;
			}
			else
			{
				return (*i_pSortedVersionList)[i-1];
			}
		}
	}

	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CClientFTPUpdateManagerDlg::MakeVersionListFile(CString i_strOutputDir, CString i_strVersionListFileName)
/// \brief		
/// \author		cmkwon
/// \date		2007-01-08 ~ 2007-01-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CClientFTPUpdateManagerDlg::MakeVersionListFile(CString i_strOutputDir, CString i_strVersionListFileName)
{
	CString			strVersionListPath;
	CFile			fileObject;
	CFileException	ex;
	ez_map<VersionInfo, VersionInfo> mapVersionOld2New;
	vector<CString>					 vectDelfilesVersionInfo;
	strVersionListPath.Format("%s\\%s", i_strOutputDir, i_strVersionListFileName);
	if (FALSE == fileObject.Open(strVersionListPath, CFile::modeCreate | CFile::modeWrite, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		DbgOut("Unable to create file(%s): %s\r\n", strVersionListPath, szError);
		return FALSE;
	}
	GetDlgItem(IDC_STATIC_VERSION_FILE_FULL_PATH)->SetWindowText(strVersionListPath);

	CString strOutputDir = i_strOutputDir;
	strOutputDir.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(strOutputDir + _T("\\*"));
	while (b)
	{
		b = ff.FindNextFile();

		CString tmpString = ff.GetFilePath().Mid(i_strOutputDir.GetLength());
		tmpString.TrimLeft(_T("\\/"));

		if (ff.IsDots()
			|| ff.IsDirectory())
		{
			continue;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-08 by cmkwon, Files
		const int	SIZE_BUFF = 1024;
		char		buffLine[SIZE_BUFF];
		char		seps[] = "_zip";
		util::strncpy(buffLine, tmpString, SIZE_BUFF);

		// from version
		char *token1 = strtok(buffLine, seps);
		// to version
		char *token2 = strtok(NULL, seps);

		if(token1 && token2)
		{
			VersionInfo OldVersion(token1);
			VersionInfo NewVersion(token2);
			if(OldVersion.IsValidVersionInfo()
				&& NewVersion.IsValidVersionInfo()
				&& OldVersion < NewVersion)
			{
				
				VersionInfo *pVersion = mapVersionOld2New.findEZ_ptr(OldVersion);
				if(NULL == pVersion)
				{
					mapVersionOld2New.insertEZ(OldVersion, NewVersion);
				}
				else
				{
					if (*pVersion < NewVersion)
					{
						// 2007-01-09 by cmkwon, 이전 버전은 필요 없다						
						CString	strTm;
						strTm.Format("%s_%s.zip", OldVersion.GetVersionString(), pVersion->GetVersionString());
						vectDelfilesVersionInfo.push_back(strTm);

						*pVersion = NewVersion;
					}
					else
					{
						vectDelfilesVersionInfo.push_back(tmpString);
					}
				}
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-09 by cmkwon
	VersionInfo LatestVer = mapVersionOld2New.rbegin()->second;
	ez_map<VersionInfo, VersionInfo>::iterator itrV(mapVersionOld2New.begin());
	for(; itrV != mapVersionOld2New.end(); itrV++)
	{
		if (LatestVer != itrV->second)
		{
			VersionInfo OldVersion = (VersionInfo&)itrV->first;
			VersionInfo *pNewVersion = mapVersionOld2New.findEZ_ptr(OldVersion);
			while(TRUE)
			{
				if(NULL == pNewVersion)
				{
					char szText[1024];
					sprintf(szText, "Update file list error !!!, OldVersion(%s)", OldVersion.GetVersionString());
					SetMainProgressText(szText);

					MessageBox(szText);
					return FALSE;
				}
				
				if(LatestVer != *pNewVersion) break;

				OldVersion		= *pNewVersion;
				pNewVersion		= mapVersionOld2New.findEZ_ptr(*pNewVersion);
			}
		}

		char writeBuff[1024];
		sprintf(writeBuff, "%s_%s.zip\r\n", ((VersionInfo&)itrV->first).GetVersionString(), itrV->second.GetVersionString());
		fileObject.Write(writeBuff, strlen(writeBuff));
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2007-01-09 by cmkwon, 필요 없는 파일 주석으로 추가
	vector<CString>::iterator itrDV(vectDelfilesVersionInfo.begin());
	for(; itrDV != vectDelfilesVersionInfo.end(); itrDV++)
	{
		char writeBuff[1024];
		sprintf(writeBuff, "# 필요없음(unnecessary)		%s\r\n", (*itrDV));
		fileObject.Write(writeBuff, strlen(writeBuff));
	}	
	fileObject.Close();

	return TRUE;
}

// 2006-12-28 by cmkwon, 사용하지 않음
//void CClientFTPUpdateManagerDlg::OnButtonTest() 
//{
//	// TODO: Add your control notification handler code here
//	UpdateData();
//	SetVersions();
//	WriteProfile();
//
//	m_szFolderToZip.TrimLeft();
//	if (m_szFolderToZip == "")
//	{
//		MessageBox("Input the name of folder to compress!!");
//		return;
//	}
//
//	m_szOutPutFolder.TrimLeft();
//	if (m_szOutPutFolder == "")
//	{
//		MessageBox("Input the name of folder to display!!");
//		return;
//	}
//
//	// make file list & version list
//	MakeFileListAndVersionList();
//
//	VersionInfo *pRetVersion = NULL;
//	VersionInfo tmpVersion("0.1.0.6");
//	pRetVersion = GetNextVersion(&m_VectorVersionInfo, &tmpVersion);
//	pRetVersion = GetPreviousVersion(&m_VectorVersionInfo, &tmpVersion);
//
//	tmpVersion.SetVersion("0.1.0.4");
//	pRetVersion = GetNextVersion(&m_VectorVersionInfo, &tmpVersion);
//	pRetVersion = GetPreviousVersion(&m_VectorVersionInfo, &tmpVersion);
//
//	tmpVersion.SetVersion("0.1.0.1");
//	pRetVersion = GetNextVersion(&m_VectorVersionInfo, &tmpVersion);
//	pRetVersion = GetPreviousVersion(&m_VectorVersionInfo, &tmpVersion);
//
//	tmpVersion.SetVersion("0.1.0.3");
//	pRetVersion = GetNextVersion(&m_VectorVersionInfo, &tmpVersion);
//	pRetVersion = GetPreviousVersion(&m_VectorVersionInfo, &tmpVersion);
//
//	tmpVersion.SetVersion("0.1.0.8");
//	pRetVersion = GetNextVersion(&m_VectorVersionInfo, &tmpVersion);
//	pRetVersion = GetPreviousVersion(&m_VectorVersionInfo, &tmpVersion);
//
//	tmpVersion.SetVersion("0.1.0.100");
//	pRetVersion = GetNextVersion(&m_VectorVersionInfo, &tmpVersion);
//	pRetVersion = GetPreviousVersion(&m_VectorVersionInfo, &tmpVersion);
//}

void CClientFTPUpdateManagerDlg::OnSelchangeComboCompressLevel() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	SetVersions();
	WriteProfile();
}

void CClientFTPUpdateManagerDlg::WriteProfile()
{
	((CAtumMonitorApp*)AfxGetApp())->m_szFolderToZipReg					= m_szFolderToZip;	
	((CAtumMonitorApp*)AfxGetApp())->m_szOutPutFolderReg				= m_szOutPutFolder;
	((CAtumMonitorApp*)AfxGetApp())->m_szStartVersionReg				= m_szStartVersion;
	((CAtumMonitorApp*)AfxGetApp())->m_szEndVersionReg					= m_szEndVersion;
	((CAtumMonitorApp*)AfxGetApp())->m_bUsePreviousVersionZipFileReg	= m_bUsePreviousVersionZipFile;
	((CAtumMonitorApp*)AfxGetApp())->m_nCompressLevelReg				= m_nCompressLevel;

	((CAtumMonitorApp*)AfxGetApp())->WriteProfile();
}

void CClientFTPUpdateManagerDlg::ReadProfile()
{
	((CAtumMonitorApp*)AfxGetApp())->ReadProfile();

	m_szFolderToZip					= ((CAtumMonitorApp*)AfxGetApp())->m_szFolderToZipReg;
	m_szOutPutFolder				= ((CAtumMonitorApp*)AfxGetApp())->m_szOutPutFolderReg;
	m_szStartVersion				= ((CAtumMonitorApp*)AfxGetApp())->m_szStartVersionReg;
	m_szEndVersion					= ((CAtumMonitorApp*)AfxGetApp())->m_szEndVersionReg;
	m_bUsePreviousVersionZipFile	= ((CAtumMonitorApp*)AfxGetApp())->m_bUsePreviousVersionZipFileReg;
	m_nCompressLevel				= ((CAtumMonitorApp*)AfxGetApp())->m_nCompressLevelReg;
}
