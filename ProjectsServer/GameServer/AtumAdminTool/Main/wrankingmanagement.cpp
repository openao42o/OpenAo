// wrankingmanagement.cpp : implementation file
//

#include "stdafx.h"
#include "..\atumadmintool.h"
#include "wrankingmanagement.h"
#include "wrsevicesettingdlg.h"
#include "wrupdateranking.h"
#include "AtumDBHelper.h"
#include "AtumAdminToolDlg.h"
#include "dlgmsgbox.h"				// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWRankingManagement dialog

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
CWRankingManagement::CWRankingManagement(CWnd* pParent /*=NULL*/)
	: CDialog(CWRankingManagement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWRankingManagement)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();
}


void CWRankingManagement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWRankingManagement)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWRankingManagement, CDialog)
	//{{AFX_MSG_MAP(CWRankingManagement)
	ON_BN_CLICKED(IDC_BTN_SERVICE_SETTING, OnBtnServiceSetting)
	ON_BN_CLICKED(IDC_BTN_UPDATE_RANKING, OnBtnUpdateRanking)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWRankingManagement message handlers

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
void CWRankingManagement::OnBtnServiceSetting() 
{
	// TODO: Add your control notification handler code here
	CWRSeviceSettingDlg dlg(this);
	dlg.DoModal();
	
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
void CWRankingManagement::OnBtnUpdateRanking() 
{
	// TODO: Add your control notification handler code here
	CWRUpdateRanking dlg(this);
	dlg.DoModal();	
}

namespace
{
	// todo : 2017-01-02 panoskj remove workaround
	unsigned helperSUID = 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_LoadSelfServiceInfo(SWRK_SERVICE_INFO *o_pServiceInfo)
{
	CODBCStatement *pLocalDB = &m_odbcStmtLocal;

	SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();

	///////////////////////////////////////////////////////////////////////////////	
	// 2009-02-19 by cmkwon, 
	BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0312);
	if (FALSE == bRet)
	{
		// 2009-02-19 by cmkwon, 월드랭킹 관련 로컬 설정을 하지 안으면 모든 랭킹 정보 처리하지 않는다.
		pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0300);
		pLocalDB->FreeStatement();
		return FALSE;
	}

	SQLINTEGER cb[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindCol(hstmt, 1, SQL_C_LONG, &helperSUID, 0,										&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, o_pServiceInfo->ServiceName, SIZE_MAX_SERVICE_NAME,					&cb[2]);
	SQLBindCol(hstmt, 3, SQL_C_BINARY, o_pServiceInfo->ServiceSymbolIamge, SIZE_MAX_SERVICE_SYMBOL_IMAGE,	&cb[3]);
	SQLBindCol(hstmt, 4, SQL_C_LONG, &o_pServiceInfo->SymbolImageSize, 0,									&cb[4]);
	util::zero(o_pServiceInfo, sizeof(*o_pServiceInfo));
	RETCODE ret = SQLFetch(hstmt);
	pLocalDB->FreeStatement();

	if(0 >= strlen(o_pServiceInfo->ServiceName))
	{
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_UpdateSelfServiceInfo(SWRK_SERVICE_INFO *i_pServiceInfo)
{
	{
		CODBCStatement *pWRServDB = &m_odbcStmtWRServer;	
		SQLHSTMT hstmt = pWRServDB->GetSTMTHandle();	
		///////////////////////////////////////////////////////////////////////////////	
		// 2009-02-19 by cmkwon, 1. 월드랭킹DB 서버에 업데이트
		SQLINTEGER cb[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,										&cb[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVICE_NAME, 0, i_pServiceInfo->ServiceName, 0,					&cb[2]);
		cb[3] = SIZE_MAX_SERVICE_SYMBOL_IMAGE;
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, SIZE_MAX_SERVICE_SYMBOL_IMAGE, 0, i_pServiceInfo->ServiceSymbolIamge, 0,	&cb[3]);
		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiceInfo->SymbolImageSize, 0,									&cb[4]);
		BOOL bRet = pWRServDB->ExecuteQuery(PROCEDURE_090219_0313);
		if (FALSE == bRet)
		{
			pWRServDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0313);
			pWRServDB->FreeStatement();
			return FALSE;
		}
		pWRServDB->FreeStatement();
	}

	{
		CODBCStatement *pLocalDB = &m_odbcStmtLocal;
		SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();
		SQLINTEGER cb[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,										&cb[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVICE_NAME, 0, i_pServiceInfo->ServiceName, 0,					&cb[2]);
		cb[3] = SIZE_MAX_SERVICE_SYMBOL_IMAGE;
		SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, SIZE_MAX_SERVICE_SYMBOL_IMAGE, 0, i_pServiceInfo->ServiceSymbolIamge, 0,	&cb[3]);
		SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_pServiceInfo->SymbolImageSize, 0,									&cb[4]);
		BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0314);
		if (FALSE == bRet)
		{
			pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0314);
			pLocalDB->FreeStatement();
			return FALSE;
		}
		pLocalDB->FreeStatement();

	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_DeleteSelfServiceInfo(SWRK_SERVICE_INFO *i_pServiceInfo)
{
	{
		CODBCStatement *pWRServDB = &m_odbcStmtWRServer;	
		SQLHSTMT hstmt = pWRServDB->GetSTMTHandle();	
		///////////////////////////////////////////////////////////////////////////////	
		// 2009-02-19 by cmkwon, 1. 월드랭킹DB 서버에 업데이트
		SQLINTEGER cb[4]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,										&cb[1]);
		BOOL bRet = pWRServDB->ExecuteQuery(PROCEDURE_090219_0315);
		if (FALSE == bRet)
		{
			pWRServDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0315);
			pWRServDB->FreeStatement();
			return FALSE;
		}
		pWRServDB->FreeStatement();

		///////////////////////////////////////////////////////////////////////////////
		// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 
		cb[1] = cb[2] = SQL_NTS;
		int nServGroupID = 0;	// 0이면 모든 서버군
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,			&cb[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nServGroupID, 0,						&cb[2]);
		bRet = pWRServDB->ExecuteQuery(PROCEDURE_100114_0410);
		if (FALSE == bRet)
		{
			pWRServDB->ProcessLogMessagesForStmt(PROCEDURE_100114_0410);
			pWRServDB->FreeStatement();
			return FALSE;
		}
		pWRServDB->FreeStatement();

	}
	
	{
		CODBCStatement *pLocalDB = &m_odbcStmtLocal;
		SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();
		BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0316);
		if (FALSE == bRet)
		{
			pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0316);
			pLocalDB->FreeStatement();
			return FALSE;
		}
		pLocalDB->FreeStatement();
		
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_LoadServerGroupInfo(SWRK_SERVER_GROUP *o_pServerGroupInfo)
{
	util::zero(o_pServerGroupInfo, sizeof(*o_pServerGroupInfo));
	
	CODBCStatement *pLocalDB = &m_odbcStmtLocal;
	SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();
	BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0317);
	if (FALSE == bRet)
	{
		pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0317);
		pLocalDB->FreeStatement();
		return FALSE;
	}
	SQLINTEGER cb[3]={SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindCol(hstmt, 1, SQL_C_LONG, &helperSUID, 0,									&cb[1]);
	SQLBindCol(hstmt, 2, SQL_C_CHAR, o_pServerGroupInfo->ServerGroupNameforRK, SIZE_MAX_SERVER_NAME,		&cb[2]);
	SQLRETURN ret = SQLFetch(hstmt);
	pLocalDB->FreeStatement();

	if(0 >= strlen(o_pServerGroupInfo->ServerGroupNameforRK))
	{
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_UpdateServerGroupInfo(SWRK_SERVER_GROUP *i_pServerGroupInfo)
{
	CODBCStatement *pLocalDB = &m_odbcStmtLocal;
	SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();

	SQLINTEGER cb[3]={SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,								&cb[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVER_NAME, 0, i_pServerGroupInfo->ServerGroupNameforRK, 0,		&cb[2]);
	BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0318);
	if(FALSE == bRet)
	{
		pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0318);
		pLocalDB->FreeStatement();
		return FALSE;
	}
	pLocalDB->FreeStatement();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-24 ~ 2009-02-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_DeleteServerGroupInfo(SWRK_SERVER_GROUP *i_pServerGroupInfo, INT i_nServiceUID)
{
	if(0 < i_nServiceUID)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 
		CODBCStatement *pWRServDB = &m_odbcStmtWRServer;	
		SQLHSTMT hstmt = pWRServDB->GetSTMTHandle();	
		SQLINTEGER cb[3]={SQL_NTS,SQL_NTS,SQL_NTS};
		SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nServiceUID, 0,						&cb[1]);
		SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,	&cb[2]);
		BOOL bRet = pWRServDB->ExecuteQuery(PROCEDURE_100114_0410);
		if (FALSE == bRet)
		{
			pWRServDB->ProcessLogMessagesForStmt(PROCEDURE_100114_0410);
			pWRServDB->FreeStatement();
			return FALSE;
		}
		pWRServDB->FreeStatement();
	}

	{
		CODBCStatement *pLocalDB = &m_odbcStmtLocal;
		SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();
		
		///////////////////////////////////////////////////////////////////////////////
		// 2010-01-14 by cmkwon, 월드랭킹시스템 버그 수정(서비스삭제,서버그룹삭제) - 
		if(0 < i_nServiceUID)
		{
			SQLINTEGER cb[3]={SQL_NTS,SQL_NTS,SQL_NTS};
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_nServiceUID, 0,						&cb[1]);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,	&cb[2]);
			BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_100114_0411);
			if (FALSE == bRet)
			{		
				pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_100114_0411);
				pLocalDB->FreeStatement();
				return FALSE;
			}
			pLocalDB->FreeStatement();
		}

		BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0337);
		if(FALSE == bRet)
		{
			pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0337);
			pLocalDB->FreeStatement();
			return FALSE;
		}
		pLocalDB->FreeStatement();
	}
	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_UpdateAllServiceList(void)
{
	mtvectSWRK_SERVICE_INFO vectServiceList;
	{
		CODBCStatement *pWRServDB = &m_odbcStmtWRServer;	
		SQLHSTMT hstmt = pWRServDB->GetSTMTHandle();	
		///////////////////////////////////////////////////////////////////////////////	
		// 2009-02-19 by cmkwon, 1. 월드랭킹DB 서버에서 서비스 리스트를 가져온다
		SQLINTEGER cb[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
		BOOL bRet = pWRServDB->ExecuteQuery(PROCEDURE_090219_0320);
		if (FALSE == bRet)
		{
			pWRServDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0320);
			pWRServDB->FreeStatement();
			return FALSE;
		}
		SWRK_SERVICE_INFO tmService;
		SQLBindCol(hstmt, 1, SQL_C_LONG, &helperSUID, 0,										&cb[1]);
		SQLBindCol(hstmt, 2, SQL_C_CHAR, tmService.ServiceName, SIZE_MAX_SERVICE_NAME,					&cb[2]);
		SQLBindCol(hstmt, 3, SQL_C_BINARY, tmService.ServiceSymbolIamge, SIZE_MAX_SERVICE_SYMBOL_IMAGE,	&cb[3]);
		SQLBindCol(hstmt, 4, SQL_C_LONG, &tmService.SymbolImageSize, 0,									&cb[4]);

		util::zero(&tmService, sizeof(tmService));
		while(TRUE)
		{
			SQLRETURN ret = SQLFetch(hstmt);
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			vectServiceList.push_back(tmService);
			
			util::zero(&tmService, sizeof(tmService));
		}
		pWRServDB->FreeStatement();
	}
	
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 로컬 서버에 기존 서비스 리스트를 모두 삭제한다.
		CODBCStatement *pLocalDB = &m_odbcStmtLocal;
		SQLHSTMT hstmt = pLocalDB->GetSTMTHandle();
		BOOL bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0321);
		if (FALSE == bRet)
		{
			pLocalDB->ProcessLogMessagesForStmt(PROCEDURE_090219_0321);
			pLocalDB->FreeStatement();
			return FALSE;
		}
		pLocalDB->FreeStatement();

		mtvectSWRK_SERVICE_INFO::iterator itr(vectServiceList.begin());
		for(; itr != vectServiceList.end(); itr++)
		{
			SWRK_SERVICE_INFO *pService = &*itr;
			
			SQLINTEGER cb[5]={SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &helperSUID, 0,										&cb[1]);
			SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_SERVICE_NAME, 0, pService->ServiceName, 0,					&cb[2]);
			cb[3] = SIZE_MAX_SERVICE_SYMBOL_IMAGE;
			SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, SIZE_MAX_SERVICE_SYMBOL_IMAGE, 0, pService->ServiceSymbolIamge, 0,	&cb[3]);
			SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &pService->SymbolImageSize, 0,									&cb[4]);
			bRet = pLocalDB->ExecuteQuery(PROCEDURE_090219_0319);
			pLocalDB->FreeStatement();
			if(FALSE == bRet)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-24 ~ 2009-02-24
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_ArrangeAllLocalRanking(void)
{
	SWRK_SERVICE_INFO tmServiInfo;
	util::zero(&tmServiInfo, sizeof(tmServiInfo));
	if(FALSE == this->DBQ_LoadSelfServiceInfo(&tmServiInfo))
	{
		AfxMessageBox("Can't arrange LocalRanking !!, You have to register this Service to WorldRankingServer.");
		return FALSE;
	}

	SWRK_SERVER_GROUP tmServGInfo;
	util::zero(&tmServGInfo, sizeof(tmServGInfo));
	if(FALSE == this->DBQ_LoadServerGroupInfo(&tmServGInfo))
	{
		AfxMessageBox("Can't arrange LocalRanking !!, You have to register this ServerGroup to WorldRankingServer.");
		return FALSE;
	}

	if(FALSE == CAtumDBHelper::WRK_LevelArrangeRanking(&m_odbcStmtLocal, &tmServiInfo, &tmServGInfo))
	{
		AfxMessageBox("Can't arrange Local LevelRanking !!");
		return FALSE;

	}
	if(FALSE == CAtumDBHelper::WRK_FameArrangeRanking(&m_odbcStmtLocal, &tmServiInfo, &tmServGInfo))
	{
		AfxMessageBox("Can't arrange Local FameRanking !!");
		return FALSE;

	}
	if(FALSE == CAtumDBHelper::WRK_PVPArrangeRanking(&m_odbcStmtLocal, &tmServiInfo, &tmServGInfo))
	{
		AfxMessageBox("Can't arrange Local PvPRanking !!");
		return FALSE;
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-02-12 by cmkwon, EP3-3 월드랭킹시스템 구현 - 
/// \author		cmkwon
/// \date		2009-02-23 ~ 2009-02-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CWRankingManagement::DBQ_UploadLocalRankingToWorldRankingServer(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-26 by cmkwon, 1. 로컬 서비스 정보 가져오기
	SWRK_SERVICE_INFO tmServiInfo;
	util::zero(&tmServiInfo, sizeof(tmServiInfo));
	if(FALSE == this->DBQ_LoadSelfServiceInfo(&tmServiInfo))
	{
		AfxMessageBox("Can't update WorldRanking !!, You have to register this Service to WorldRankingServer.");
		return FALSE;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-26 by cmkwon, 2. 로컬 서버군 정보 가져오기
	SWRK_SERVER_GROUP tmServGInfo;
	util::zero(&tmServGInfo, sizeof(tmServGInfo));
	if(FALSE == this->DBQ_LoadServerGroupInfo(&tmServGInfo))
	{
		AfxMessageBox("Can't update WorldRanking !!, You have to register this ServerGroup to WorldRankingServer.");
		return FALSE;
	}

	BYTE byRankScope = RANKING_SCOPE_WORLD;
	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-24 by cmkwon, 3. 로컬 레벨 랭킹을 월드랭킹서버에 업로드한다.
	{
		mtvectSWRK_LEVEL_FOR_DB tmRankerList;
		if(FALSE == CAtumDBHelper::WRK_LevelLoadRanking(&m_odbcStmtLocal, helperSUID, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 2. 월드랭킹서버에 업데이트하기
		if(FALSE == CAtumDBHelper::WRK_LevelUpdateRankingToWRankingServer(&m_odbcStmtWRServer, &tmServiInfo, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-24 by cmkwon, 4. 로컬 명성 랭킹을 월드랭킹서버에 업로드한다.
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 1. 로컬서버에서 월드랭킹을 위한 랭킹 리스트를 가져온다.
		mtvectSWRK_FAME_FOR_DB tmRankerList;
		BYTE byRankScope = RANKING_SCOPE_WORLD;
		if(FALSE == CAtumDBHelper::WRK_FameLoadRanking(&m_odbcStmtLocal, helperSUID, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 2. 월드랭킹서버에 업데이트하기
		if(FALSE == CAtumDBHelper::WRK_FameUpdateRankingToWRankingServer(&m_odbcStmtWRServer, &tmServiInfo, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-24 by cmkwon, 5. 로컬 PvP 랭킹을 월드랭킹서버에 업로드한다.
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 1. 로컬서버에서 월드랭킹을 위한 랭킹 리스트를 가져온다.
		mtvectSWRK_PVP_FOR_DB tmRankerList;
		BYTE byRankScope = RANKING_SCOPE_WORLD;
		if(FALSE == CAtumDBHelper::WRK_PVPLoadRanking(&m_odbcStmtLocal, helperSUID, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 2. 월드랭킹서버에 업데이트하기
		if(FALSE == CAtumDBHelper::WRK_PVPUpdateRankingToWRankingServer(&m_odbcStmtWRServer, &tmServiInfo, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
	}

	return TRUE;
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
BOOL CWRankingManagement::DBQ_DownloadWorldRankingFromWorldRankingServer(void)
{
	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-26 by cmkwon, 1. 로컬 서비스 정보 가져오기
	SWRK_SERVICE_INFO tmServiInfo;
	util::zero(&tmServiInfo, sizeof(tmServiInfo));
	if(FALSE == this->DBQ_LoadSelfServiceInfo(&tmServiInfo))
	{
		AfxMessageBox("Can't update WorldRanking !!, You have to register this Service to WorldRankingServer.");
		return FALSE;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-26 by cmkwon, 2. 로컬 서버군 정보 가져오기
	SWRK_SERVER_GROUP tmServGInfo;
	util::zero(&tmServGInfo, sizeof(tmServGInfo));
	if(FALSE == this->DBQ_LoadServerGroupInfo(&tmServGInfo))
	{
		AfxMessageBox("Can't update WorldRanking !!, You have to register this ServerGroup to WorldRankingServer.");
		return FALSE;
	}


	BYTE byRankScope = RANKING_SCOPE_WORLD;
	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-24 by cmkwon, 3. 월드 레벨 랭킹을 다운로드 받는다.
	{
		mtvectSWRK_LEVEL_FOR_DB tmRankerList;
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 4. 월드랭킹서버에서 랭킹 리스트 가져오기
		if(FALSE == CAtumDBHelper::WRK_LevelLoadRankingToWorldRankingServer(&m_odbcStmtWRServer, helperSUID, &tmRankerList))
		{
			return FALSE;
		}	
		
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 5. 로컬서버에 월드랭킹 업데이트하기
		if(FALSE == CAtumDBHelper::WRK_LevelInsertRanking(&m_odbcStmtLocal, &tmServiInfo, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-24 by cmkwon, 4. 월드 명성 랭킹을 다운로드 받는다.
	{
		mtvectSWRK_FAME_FOR_DB tmRankerList;
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 4. 월드랭킹서버에서 랭킹 리스트 가져오기
		if(FALSE == CAtumDBHelper::WRK_FameLoadRankingToWorldRankingServer(&m_odbcStmtWRServer, helperSUID, &tmRankerList))
		{
			return FALSE;
		}	
		
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 5. 로컬서버에 월드랭킹 업데이트하기
		if(FALSE == CAtumDBHelper::WRK_FameInsertRanking(&m_odbcStmtLocal, &tmServiInfo, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-02-24 by cmkwon, 5. 월드 PvP 랭킹을 다운로드 받는다.
	{
		mtvectSWRK_PVP_FOR_DB tmRankerList;
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 4. 월드랭킹서버에서 랭킹 리스트 가져오기
		if(FALSE == CAtumDBHelper::WRK_PVPLoadRankingToWorldRankingServer(&m_odbcStmtWRServer, helperSUID, &tmRankerList))
		{
			return FALSE;
		}	
		
		///////////////////////////////////////////////////////////////////////////////
		// 2009-02-24 by cmkwon, 5. 로컬서버에 월드랭킹 업데이트하기
		if(FALSE == CAtumDBHelper::WRK_PVPInsertRanking(&m_odbcStmtLocal, &tmServiInfo, byRankScope, &tmRankerList))
		{
			return FALSE;
		}
	}

	return TRUE;
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
BOOL CWRankingManagement::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Connect DB
	if ( FALSE == m_odbcStmtLocal.Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd()) )
	{
		char szTemp[1024];
		sprintf(szTemp, "Can not connect DBServer<%s(%s:%d)> !!"
			, m_pMainDlg->m_pServerInfo4Admin->DBName, m_pMainDlg->m_pServerInfo4Admin->DBIP
			, m_pMainDlg->m_pServerInfo4Admin->DBPort);
		MessageBox(szTemp);
		EndDialog(-1);
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 
	CDlgMsgBox	dlg;
	INT nReted = dlg.DoModal();
	if(IDCANCEL == nReted)
	{
		EndDialog(-1);
		return FALSE;
	}
	// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 본섭/테섭 따로 처리
	// 2009-11-24 by cmkwon, 월드랭킹 실행시 질문 메시지박스 수정 - 
	//if(IDYES == AfxMessageBox("Do you excute WRM(World Ranking Management) for MainServer?", MB_YESNO))
	if(dlg.GetIsForMainServer())
	{
		if ( FALSE == m_odbcStmtWRServer.Init(WRK_DBSERVER_IP, WRK_DBSERVER_PORT, WRK_DBSERVER_DATABASE_NAME, WRK_DBSERVER_ID, WRK_DBSERVER_PWD, GetSafeHwnd()) )
		{
			char szTemp[1024];
			sprintf(szTemp, "Can not connect WRanking DBServer<%s(%s:%d)> !!", WRK_DBSERVER_DATABASE_NAME, WRK_DBSERVER_IP, WRK_DBSERVER_PORT);
			MessageBox(szTemp);
			EndDialog(-1);
			return FALSE;
		}
	}
	else
	{
		// 2009-06-01 by cmkwon, 월드 랭킹 시스템 테스트 기능 구현(for 테섭) - 테섭용 정의를 사용한다.
		if ( FALSE == m_odbcStmtWRServer.Init(WRK_DBSERVER_IP_FOR_TEST_SERVER, WRK_DBSERVER_PORT_FOR_TEST_SERVER, WRK_DBSERVER_DATABASE_NAME, WRK_DBSERVER_ID, WRK_DBSERVER_PWD, GetSafeHwnd()) )
		{
			char szTemp[1024];
			sprintf(szTemp, "Can not connect WRanking DBServer<%s(%s:%d)> for TestServer !!", WRK_DBSERVER_DATABASE_NAME, WRK_DBSERVER_IP_FOR_TEST_SERVER, WRK_DBSERVER_PORT_FOR_TEST_SERVER);
			MessageBox(szTemp);
			EndDialog(-1);
			return FALSE;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
