// AnalyzePacketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Atum_LoadGen.h"
#include "AnalyzePacketDlg.h"
#include "FieldWinSocket.h"
#include "BrowseForFolder.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnalyzePacketDlg dialog


CAnalyzePacketDlg::CAnalyzePacketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalyzePacketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnalyzePacketDlg)
	m_ctlcsFilePath = _T("");
	//}}AFX_DATA_INIT
}


void CAnalyzePacketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalyzePacketDlg)
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_ctlcsFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnalyzePacketDlg, CDialog)
	//{{AFX_MSG_MAP(CAnalyzePacketDlg)
	ON_BN_CLICKED(IDC_BTN_FIND_FILE, OnBtnFindFile)
	ON_BN_CLICKED(IDC_BTN_ANALYZE, OnBtnAnalyze)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalyzePacketDlg message handlers

BOOL CAnalyzePacketDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-13 by cmkwon, 패킷분석 유틸 구현 - 
/// \author		cmkwon
/// \date		2009-03-13 ~ 2009-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAnalyzePacketDlg::dividePath(char *i_szPath, char *o_szDirectory, char *o_szFileNameExExtention)
{
	util::strncpy(o_szDirectory, ".\\", MAX_PATH);		// 2009-03-13 by cmkwon, 현재 폴더로 초기화

	char *pFindDelimiterForDir = strrchr(i_szPath, '\\');
	if(NULL == pFindDelimiterForDir)
	{
		pFindDelimiterForDir	= strrchr(i_szPath, '/');
	}

	char szFileName[MAX_PATH];
	util::zero(szFileName, MAX_PATH);
	if(NULL == pFindDelimiterForDir)
	{
		util::strncpy(szFileName, i_szPath, MAX_PATH);
	}
	else
	{
		util::zero(o_szDirectory, MAX_PATH);
		memcpy(o_szDirectory, i_szPath, (pFindDelimiterForDir - i_szPath)+1);
		util::strncpy(szFileName, pFindDelimiterForDir + 1, MAX_PATH)
	}

	char *pFindDelimiterForFile = strrchr(szFileName, '.');
	if(NULL == pFindDelimiterForFile)
	{
		util::strncpy(o_szFileNameExExtention, szFileName, MAX_PATH);
	}
	else
	{
		util::zero(o_szFileNameExExtention, MAX_PATH);
		memcpy(o_szFileNameExExtention, szFileName, pFindDelimiterForFile - szFileName);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		cmkwon
/// \date		2009-03-13 ~ 2009-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CAnalyzePacketDlg::GetFilePathList(CString i_csSourDir, vectCString *o_pFilePathList)
{
	i_csSourDir.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(i_csSourDir + _T("\\*"));
	while (b)
	{
		b = ff.FindNextFile();
				
		if (ff.IsDots()
			|| ff.IsDirectory())
		{
			continue;
		}

		CString csExtension = ff.GetFilePath().Right(4);
		if(0 != csExtension.CollateNoCase(".dat"))
		{// 2009-03-16 by cmkwon, 확장자 체크
			continue;
		}

		// File인 경우		
		o_pFilePathList->push_back(ff.GetFilePath());
	}

	return TRUE;
}


void CAnalyzePacketDlg::OnBtnFindFile() 
{
	// TODO: Add your control notification handler code here

	CBrowseForFolder bf;	
	bf.hWndOwner = this->m_hWnd;
	bf.strTitle = "Select folder to analyze WriteBufData";
	CString sz;
	if (!bf.GetFolder(sz, NULL))
		return;
	
	m_ctlcsFilePath = sz;

	UpdateData(FALSE);
}

void CAnalyzePacketDlg::OnBtnAnalyze() 
{
	// TODO: Add your control notification handler code here

	UpdateData();

	if(m_ctlcsFilePath.IsEmpty())
	{
		AfxMessageBox("Invalid directory !!");
		return;
	}

	vectCString tmFilePathList;
	this->GetFilePathList(m_ctlcsFilePath, &tmFilePathList);	

	if(tmFilePathList.empty())
	{
		return;
	}
	
	vectCString::iterator itr(tmFilePathList.begin());
	for(; itr != tmFilePathList.end(); itr++)
	{
		char szFilePath[MAX_PATH];
		util::zero(szFilePath, MAX_PATH);
		sprintf(szFilePath, "%s", *itr);
		
		char szFileDirectory[MAX_PATH];
		char szFileNameExExtension[MAX_PATH];
		util::zero(szFileDirectory, MAX_PATH);
		util::zero(szFileNameExExtension, MAX_PATH);
		if(FALSE == this->dividePath(szFilePath, szFileDirectory, szFileNameExExtension))
		{
			AfxMessageBox("Invalid File Path !!");
			return ;
		}
		
		char szResultFileName[MAX_PATH];
		util::zero(szResultFileName, MAX_PATH);
		sprintf(szResultFileName, "%s_analyze.txt", szFileNameExExtension);
		
		CAnalyzePacket analyzePK;
		if(FALSE == analyzePK.LoadWriteBufDataFile(szFilePath))
		{
			AfxMessageBox("error in order to load file !!");
			return ;
		}
		
		if(FALSE == analyzePK.Analyze())
		{
			AfxMessageBox("error in order to analyze packet !!");
			return ;
		}
		
		if(FALSE == analyzePK.SaveResult(szFileDirectory, szFileNameExExtension))
		{
			AfxMessageBox("error in order to save result !!");
			return ;
		}
	}

	AfxMessageBox("success");
}
