// wrsevicesettingdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "wrsevicesettingdlg.h"
#include "wrankingmanagement.h"
#include "AtumSJ.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWRSeviceSettingDlg dialog


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CWRSeviceSettingDlg::CWRSeviceSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWRSeviceSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWRSeviceSettingDlg)
	m_ctrlcsServiceUID = _T("");
	m_ctrlcsServiceName = _T("");
	m_ctrlcsSymbolImagePath = _T("");
	m_ctrlcsServerGroupName = _T("");
	m_ctrlcsServerGroupID = _T("");
	//}}AFX_DATA_INIT
	m_ctrlcsServiceUID.Format("%d", SERVICE_UID_FOR_WORLD_RANKING);

	m_pWRankManager		= (CWRankingManagement*)pParent;
	util::zero(&m_RegServiceInfo, sizeof(m_RegServiceInfo));
	util::zero(&m_RegServerGroupInfo, sizeof(m_RegServerGroupInfo));
}


void CWRSeviceSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWRSeviceSettingDlg)
	DDX_Text(pDX, IDC_EDIT_SERVICEUID, m_ctrlcsServiceUID);
	DDX_Text(pDX, IDC_EDIT_SERVICE_NAME, m_ctrlcsServiceName);
	DDX_Text(pDX, IDC_EDIT_SYMBOL_IMAGE_PATH, m_ctrlcsSymbolImagePath);
	DDX_Text(pDX, IDC_EDIT_SERVER_GROUP_NAME, m_ctrlcsServerGroupName);
	DDX_Text(pDX, IDC_EDIT_SERVER_GROUP_ID, m_ctrlcsServerGroupID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWRSeviceSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CWRSeviceSettingDlg)
	ON_BN_CLICKED(IDC_FIND_FILE, OnFindFile)
	ON_BN_CLICKED(IDC_BTN_REG_OR_UPDATE, OnBtnRegOrUpdate)
	ON_BN_CLICKED(IDC_BTN_DELETE_SERVICE, OnBtnDeleteService)
	ON_BN_CLICKED(IDC_BTN_DELETE_SERVER_GROUP_INFO, OnBtnDeleteServerGroupInfo)
	ON_BN_CLICKED(IDC_BTN_UPDATE_SERVER_GROUP_NAME, OnBtnUpdateServerGroupName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWRSeviceSettingDlg message handlers

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRSeviceSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	SWRK_SERVICE_INFO tmServiInfo;	util::zero(&tmServiInfo, sizeof(tmServiInfo));
	if(m_pWRankManager->DBQ_LoadSelfServiceInfo(&tmServiInfo))
	{
		m_ctrlcsServiceUID = ""; // 2017-01-02 panoskj
		m_ctrlcsServiceName		= tmServiInfo.ServiceName;
		m_RegServiceInfo		= tmServiInfo;
	}

	SWRK_SERVER_GROUP tmServGroup;	util::zero(&tmServGroup, sizeof(tmServGroup));
	if(m_pWRankManager->DBQ_LoadServerGroupInfo(&tmServGroup))
	{
		m_ctrlcsServerGroupID = ""; // 2017-01-02 panoskj
		m_ctrlcsServerGroupName	= tmServGroup.ServerGroupNameforRK;
		m_RegServerGroupInfo	= tmServGroup;
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRSeviceSettingDlg::OnFindFile() 
{
	// TODO: Add your control notification handler code here
	bool bOpen = TRUE;
	
	CFileDialog fDlg(bOpen, "Find Service Symbole Image", bOpen ? NULL : _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Image Files (*.bmp;*.jpg)|*.bmp;*.jpg|All Files (*.*)|*.*||"));
	fDlg.m_ofn.lpstrInitialDir = ".";
	if (IDOK != fDlg.DoModal())
	{
		return;
	}

	CFile ImageFile;
	if(FALSE == ImageFile.Open(fDlg.GetPathName(), CFile::modeRead))
	{
		AfxMessageBox("Invalid file path !!");
		return;
	}

	DWORD dwLen = ImageFile.GetLength();
	if(SIZE_MAX_SERVICE_SYMBOL_IMAGE < dwLen)
	{
		AfxMessageBox("File size is too big !!(less then 2KBytes)");
		return;
	}
	
	m_ctrlcsSymbolImagePath = fDlg.GetPathName();
	
	GetDlgItem(IDC_EDIT_SYMBOL_IMAGE_PATH)->SetWindowText(m_ctrlcsSymbolImagePath);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRSeviceSettingDlg::OnBtnRegOrUpdate() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}
	
	UpdateData();

	SWRK_SERVICE_INFO tmServiInfo;
	util::zero(&tmServiInfo, sizeof(tmServiInfo));

	if(m_ctrlcsServiceName.IsEmpty())
	{
		AfxMessageBox("Input Service Name !!");
		return;
	}

	util::strncpy(tmServiInfo.ServiceName, m_ctrlcsServiceName, SIZE_MAX_SERVICE_NAME);
	// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 아래와 같이 수정('-','_'허용)
	//if(FALSE == CAtumSJ::IsAlphaNum(tmServiInfo.ServiceName))
	if(FALSE == CAtumSJ::IsValidWRankingServiceName(tmServiInfo.ServiceName))
	{
		AfxMessageBox("Invalid Service Name !!");
		return;
	}

	if(m_ctrlcsSymbolImagePath.IsEmpty())
	{
		memcpy(tmServiInfo.ServiceSymbolIamge, m_RegServiceInfo.ServiceSymbolIamge, SIZE_MAX_SERVICE_SYMBOL_IMAGE);
		tmServiInfo.SymbolImageSize		= m_RegServiceInfo.SymbolImageSize;
	}
	else
	{
		CFile ImageFile;
		if(FALSE == ImageFile.Open(m_ctrlcsSymbolImagePath, CFile::modeRead))
		{
			AfxMessageBox("Invalid image file path !!");
			return;
		}
		
		DWORD dwLen = ImageFile.GetLength();
		if(SIZE_MAX_SERVICE_SYMBOL_IMAGE < dwLen)
		{
			AfxMessageBox("Image file size is too big !!");
			return;
		}

		ImageFile.Read(tmServiInfo.ServiceSymbolIamge, dwLen);
		tmServiInfo.SymbolImageSize		= dwLen;
	}

	if(FALSE == m_pWRankManager->DBQ_UpdateSelfServiceInfo(&tmServiInfo))
	{
		AfxMessageBox("DBQ_UpdateSelfServiceInfo# fail !!");
		return;
	}

	m_RegServiceInfo	= tmServiInfo;

	AfxMessageBox("Success to update SelfServiceInfo !!");	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRSeviceSettingDlg::OnBtnDeleteService() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}

	if(FALSE == this->IsValidServiceInfo())
	{
		AfxMessageBox("You have to register this service to WorldRankingServer !!");
		return;
	}

	UpdateData();

	SWRK_SERVICE_INFO tmServiInfo;
	util::zero(&tmServiInfo, sizeof(tmServiInfo));


	if(FALSE == m_pWRankManager->DBQ_DeleteSelfServiceInfo(&tmServiInfo))
	{
		AfxMessageBox("DBQ_DeleteSelfServiceInfo# fail !!");
		return;
	}

	m_ctrlcsServiceName.Empty();
	m_ctrlcsSymbolImagePath.Empty();
	util::zero(&m_RegServiceInfo, sizeof(m_RegServiceInfo));

	UpdateData(FALSE);

	AfxMessageBox("Success to delete SelfServiceInfo !!");	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRSeviceSettingDlg::OnBtnDeleteServerGroupInfo() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}


	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 아래와 같이 인자로 전달.
	int nServiceUID = (m_ctrlcsServiceUID.IsEmpty()) ? 0 : atoi(m_ctrlcsServiceUID);
	if(FALSE == m_pWRankManager->DBQ_DeleteServerGroupInfo(&m_RegServerGroupInfo, nServiceUID))
	{
		AfxMessageBox("DBQ_DeleteServerGroupInfo fail !!");
		return;
	}

	util::zero(&m_RegServerGroupInfo, sizeof(m_RegServerGroupInfo));
	m_ctrlcsServerGroupID.Empty();
	m_ctrlcsServerGroupName.Empty();

	UpdateData(FALSE);

	AfxMessageBox("Success to delete ServerGroupInfo !!");
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-26 ~ 2009-02-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CWRSeviceSettingDlg::OnBtnUpdateServerGroupName() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}

	if(FALSE == this->IsValidServiceInfo())
	{
		AfxMessageBox("You have to register this service to WorldRankingServer !!");
		return;
	}

	UpdateData();

	SWRK_SERVER_GROUP tmServG;
	util::zero(&tmServG, sizeof(tmServG));

	if(m_ctrlcsServerGroupID.IsEmpty())
	{
		AfxMessageBox("Input Server Group ID !!");
		return;
	}

	if(m_ctrlcsServerGroupName.IsEmpty())
	{
		AfxMessageBox("Input Server Group Name !!");
		return;
	}
	util::strncpy(tmServG.ServerGroupNameforRK, m_ctrlcsServerGroupName, SIZE_MAX_SERVER_NAME);
	// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 아래와 같이 수정('-','_'허용)
	//if(FALSE == CAtumSJ::IsAlphaNum(tmServG.ServerGroupNameforRK))
	if(FALSE == CAtumSJ::IsValidWRankingServiceName(tmServG.ServerGroupNameforRK))
	{
		AfxMessageBox("Invalid Server Group Name !!");
		return;
	}

	if(FALSE == m_pWRankManager->DBQ_UpdateServerGroupInfo(&tmServG))
	{
		AfxMessageBox("DBQ_UpdateServerGroupInfo fail !!");
		return;
	}


	m_RegServerGroupInfo	= tmServG;

	AfxMessageBox("Success to update ServerGroupInfo !!");
}


bool CWRSeviceSettingDlg::IsValidServiceInfo() const
{
	return strlen(m_RegServiceInfo.ServiceName) > 0;
}