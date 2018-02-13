// wrupdateranking.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "wrupdateranking.h"
#include "wrankingmanagement.h"
#include "AtumAdminToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWRUpdateRanking dialog


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
CWRUpdateRanking::CWRUpdateRanking(CWnd* pParent /*=NULL*/)
	: CDialog(CWRUpdateRanking::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWRUpdateRanking)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pWRankManager		= (CWRankingManagement*)pParent;
}


void CWRUpdateRanking::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWRUpdateRanking)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWRUpdateRanking, CDialog)
	//{{AFX_MSG_MAP(CWRUpdateRanking)
	ON_BN_CLICKED(IDC_BTN_UPDATE_SERVICE, OnBtnUpdateService)
	ON_BN_CLICKED(IDC_BTN_ARRANGE_LOCAL_RANKING, OnBtnArrangeLocalRanking)
	ON_BN_CLICKED(IDC_BTN_UPDATE_WRANKING, OnBtnUpdateWranking)
	ON_BN_CLICKED(IDC_BTN_DOWNLOAD_WRANKING, OnBtnDownloadWranking)
	ON_BN_CLICKED(IDC_BTN_RELOAD_WRANKING, OnBtnReloadWranking)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWRUpdateRanking message handlers

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
void CWRUpdateRanking::OnBtnUpdateService() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}
	
	if(FALSE == m_pWRankManager->DBQ_UpdateAllServiceList())
	{
		AfxMessageBox("DBQ_UpdateAllServiceList fail !!");
		return;
	}

	AfxMessageBox("Success to update all service list.");
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
void CWRUpdateRanking::OnBtnArrangeLocalRanking() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}

	if(FALSE == m_pWRankManager->DBQ_ArrangeAllLocalRanking())
	{
		return;
	}
	AfxMessageBox("Success to arrange local ranking.");
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
void CWRUpdateRanking::OnBtnUpdateWranking() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}
	
	if(FALSE == m_pWRankManager->DBQ_UploadLocalRankingToWorldRankingServer())
	{
		return;
	}
	AfxMessageBox("Success to upload local ranking to world ranking server .");
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
void CWRUpdateRanking::OnBtnDownloadWranking() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}
	
	if(FALSE == m_pWRankManager->DBQ_DownloadWorldRankingFromWorldRankingServer())
	{
		return;
	}
	AfxMessageBox("Success to download world ranking.");
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
void CWRUpdateRanking::OnBtnReloadWranking() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_pWRankManager->m_pMainDlg->IsConnectedToPreServer())
	{
		AfxMessageBox("Disconnected from PreServer !!");
		return;
	}

	m_pWRankManager->m_pMainDlg->SendMsgTypeToPreServer(T_PA_ADMIN_RELOAD_WORLDRANKING);

	AfxMessageBox("Success to send reload world ranking.");
}
