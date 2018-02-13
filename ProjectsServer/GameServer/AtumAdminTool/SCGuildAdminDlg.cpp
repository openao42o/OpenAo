// SCGuildAdminDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "SCGuildAdminDlg.h"
#include "AtumAdminToolDlg.h"
#include "ODBCStatement.h"
#include "AtumSJ.h"
#include "EditGuildDlg.h"
#include "guildmarkrecognitiondlg.h"		// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 -


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSCGuildAdminDlg dialog


CSCGuildAdminDlg::CSCGuildAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCGuildAdminDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSCGuildAdminDlg)
	m_ctlstrSearchGuildName = _T("");
	m_ctluiSearchGuildUID = 0;
	//}}AFX_DATA_INIT
	
	m_pMainDlg = (CAtumAdminToolDlg*)AfxGetMainWnd();

	m_pODBCStmt = new CODBCStatement;

	util::zero(&m_CurGUILD, sizeof(m_CurGUILD));
}

CSCGuildAdminDlg::~CSCGuildAdminDlg()
{
	util::del(m_pODBCStmt);
}


void CSCGuildAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCGuildAdminDlg)
	DDX_Text(pDX, IDC_EDIT_SEARCH_GUILDNAME, m_ctlstrSearchGuildName);
	DDX_Text(pDX, IDC_EDIT_SEARCH_GUILDUID, m_ctluiSearchGuildUID);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_GUILD, m_GridGuild);
	DDX_GridControl(pDX, IDC_GRID_GUILDMEMBER,m_GridGuildMembers);
	DDX_GridControl(pDX, IDC_GRID_GUILDITEMS,m_GridGuildItems);
}


BEGIN_MESSAGE_MAP(CSCGuildAdminDlg, CDialog)
	//{{AFX_MSG_MAP(CSCGuildAdminDlg)
	ON_BN_CLICKED(IDC_BTN_SEARCHGUILD, OnBtnSearchguild)
	ON_BN_CLICKED(IDC_BTN_SEARCHGUILD_BYGUID, OnBtnSearchguildByguid)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_EDIT_GUILD, OnBtnEditGuild)
	ON_BN_CLICKED(IDC_BTN_GUILD_MARK_RECOGNITION, OnBtnGuildMarkRecognition)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_GRID_ITEM, OnGridClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCGuildAdminDlg message handlers


BOOL CSCGuildAdminDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

		// Connect DB
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
	)
	{
		char szTemp[1024];
		sprintf(szTemp, STRERR_S_SCADMINTOOL_0021
			, m_pMainDlg->m_pServerInfo4Admin->DBName, m_pMainDlg->m_pServerInfo4Admin->DBIP
			, m_pMainDlg->m_pServerInfo4Admin->DBPort);
		MessageBox(szTemp);
		EndDialog(-1);
		return FALSE;
	}

	// 데이타 로딩
	CAtumDBHelper::LoadItemInfo(m_pODBCStmt, &m_mapItemInfo);
	CAtumDBHelper::LoadRareItemInfo(m_pODBCStmt, &m_mapRareItemInfo);

	this->InitGridGuild();
	this->InitGridGuildMembers();
	this->InitGridGuildItems();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSCGuildAdminDlg::OnBtnSearchguild() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - GuildUID 검색창 초기화
	m_ctluiSearchGuildUID = 0;
	UpdateData(FALSE);


	util::zero(&m_CurGUILD, sizeof(m_CurGUILD));
	this->InitGridGuild();
	this->InitGridGuildMembers();
	this->InitGridGuildItems();

	if(m_ctlstrSearchGuildName.IsEmpty())
	{
		AfxMessageBox("Please, input GuildName");
		return;
	}
	if(m_ctlstrSearchGuildName.GetLength() >= SIZE_MAX_GUILD_NAME)
	{
		AfxMessageBox("Error !!. Invalid GuildName");
		return;
	}

	if(FALSE == this->GetGuildInfo(m_ctlstrSearchGuildName))
	{
		AfxMessageBox("Corresponding guild does not exist !!");
		return;
	}

	this->GetGuildMembersInfo(&m_CurGUILD);
	this->GetGuildItems(&m_CurGUILD);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCGuildAdminDlg::InitGridGuild(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCGuildAdminDlg::InitGridGuild(void)
{
	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - 
	m_ilGuildMarkList.DeleteImageList();
	m_ilGuildMarkList.Create(24, 12, ILC_COLOR32, 1, 1);

	m_GridGuild.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 2;

	m_GridGuild.SetEditable(FALSE);
	m_GridGuild.SetListMode(TRUE);
// 2007-12-06 by cmkwon, SCAdminTool 수정(그리드 멀티라인 선택가능하게 수정) -
//	m_GridGuild.SetSingleRowSelection(TRUE);
	m_GridGuild.EnableSelection(TRUE);
	m_GridGuild.SetFrameFocusCell(FALSE);
	m_GridGuild.SetTrackFocusCell(FALSE);

	m_GridGuild.SetRowCount(m_nRows);
	m_GridGuild.SetColumnCount(m_nCols);
	m_GridGuild.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = m_nCols++;
	Item.strText.Format("Classification");
	m_GridGuild.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Value");
	m_GridGuild.SetItem(&Item);

	// arrange grid
	m_GridGuild.AutoSize();
	m_GridGuild.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridGuild.GetRowCount()-1, m_GridGuild.GetColumnCount()-1);
	m_GridGuild.ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCGuildAdminDlg::InitGridGuildMembers(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCGuildAdminDlg::InitGridGuildMembers(void)
{
	m_GridGuildMembers.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	int m_nCols = 10;

	m_GridGuildMembers.SetEditable(FALSE);
	m_GridGuildMembers.SetListMode(TRUE);
// 2007-12-06 by cmkwon, SCAdminTool 수정(그리드 멀티라인 선택가능하게 수정) -
//	m_GridGuildMembers.SetSingleRowSelection(TRUE);
	m_GridGuildMembers.EnableSelection(TRUE);
	m_GridGuildMembers.SetFrameFocusCell(FALSE);
	m_GridGuildMembers.SetTrackFocusCell(FALSE);

	m_GridGuildMembers.SetRowCount(m_nRows);
	m_GridGuildMembers.SetColumnCount(m_nCols);
	m_GridGuildMembers.SetFixedRowCount(1);

	// 칼럼 만들기
	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = 0;
	Item.strText.Format("Num");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 1;
	Item.strText.Format("Account Name");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 2;
	Item.strText.Format("Character Name");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 3;
	Item.strText.Format("Unitkind");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 4;
	Item.strText.Format("InfluenceType");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 5;
	Item.strText.Format("Level");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 6;
	Item.strText.Format("Fame");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 7;
	Item.strText.Format("PvP Win");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 8;
	Item.strText.Format("PvP Loss");
	m_GridGuildMembers.SetItem(&Item);
	Item.col = 9;
	Item.strText.Format("Guild Rank");
	m_GridGuildMembers.SetItem(&Item);
	
	// arrange grid
	m_GridGuildMembers.AutoSize();
	m_GridGuildMembers.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridGuildMembers.GetRowCount()-1, m_GridGuildMembers.GetColumnCount()-1);
	m_GridGuildMembers.ClearCells(tmpCellRange);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCGuildAdminDlg::InitGridGuildItems(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCGuildAdminDlg::InitGridGuildItems(void)
{
	m_GridGuildItems.SetBkColor(0xFFFFFF);

	int m_nRows = 1;
	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 아래와 같이 수정
	//int m_nCols = 12;		// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	int m_nCols = 13;	

	m_GridGuildItems.SetEditable(FALSE);
	m_GridGuildItems.SetListMode(TRUE);
// 2007-12-06 by cmkwon, SCAdminTool 수정(그리드 멀티라인 선택가능하게 수정) -
//	m_GridGuildItems.SetSingleRowSelection(TRUE);
	m_GridGuildItems.EnableSelection(TRUE);
	m_GridGuildItems.SetHeaderSort(FALSE);
	m_GridGuildItems.SetFrameFocusCell(FALSE);
	m_GridGuildItems.SetTrackFocusCell(FALSE);

	m_GridGuildItems.SetRowCount(m_nRows);
	m_GridGuildItems.SetColumnCount(m_nCols);
	m_GridGuildItems.SetFixedRowCount(1);

	m_nCols = 0;
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = 0;
	Item.nFormat = GRID_CELL_FORMAT;

	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	Item.col = m_nCols++;
	Item.strText.Format(_T("Count"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Item UID"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Item Name"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Item Num"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Rare Prefix"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Rare Suffix"));
	m_GridGuildItems.SetItem(&Item);

	// 2005-11-26 by cmkwon, 추가함
	Item.col = m_nCols++;
	Item.strText.Format(_T("Enchant Count"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Attach Info"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("Item Position");
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Counts"));
	m_GridGuildItems.SetItem(&Item);
// 2005-12-08 by cmkwon, 사용하지 않는 필드임
//	Item.col = m_nCols++;
//	Item.strText.Format(_T("Endurance"));
//	m_GridGuildItems.SetItem(&Item);
	Item.col = m_nCols++;
	Item.strText.Format(_T("EffectItemNum"));	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ColorCode를 EffectItemNum으로 변경해서 사용
	m_GridGuildItems.SetItem(&Item);

	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 12번 열에 추가 함.
	Item.col = m_nCols++;
	Item.strText.Format(_T("ShapeItemNum"));
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format(_T("Created Time"));
	m_GridGuildItems.SetItem(&Item);

	m_GridGuildItems.AutoSize();
	m_GridGuildItems.ExpandColumnsToFit();

	// clean all cells
	CCellRange tmpCellRange(1, 0, m_GridGuildItems.GetRowCount()-1, m_GridGuildItems.GetColumnCount()-1);
	m_GridGuildItems.ClearCells(tmpCellRange);
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, const char *i_szValue)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, const char *i_szValue)
{
	int nCurrentRow = m_GridGuild.GetRowCount();
	m_GridGuild.SetRowCount(nCurrentRow+1);

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = nCurrentRow;
	Item.nFormat = GRID_CELL_FORMAT;

	Item.col = 0;
	Item.strText = i_szKey;
	m_GridGuild.SetItem(&Item);

	Item.col = 1;
	Item.strText = i_szValue;
	m_GridGuild.SetItem(&Item);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, int i_nValue)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, int i_nValue)
{
	CString tmp;
	tmp.Format("%d", i_nValue);

	return AddLineToGridGuildMembers(i_szKey, (LPCSTR)tmp);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, UINT i_uiValue)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, UINT i_uiValue)
{
	CString tmp;
	tmp.Format("%d", i_uiValue);

	return AddLineToGridGuildMembers(i_szKey, (LPCSTR)tmp);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, float i_fValue)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, float i_fValue)
{
	CString tmp;
	tmp.Format("%5.2f", i_fValue);

	return AddLineToGridGuildMembers(i_szKey, (LPCSTR)tmp);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, CBitmap *i_pbmGuildMark)
/// \brief		// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - AddLineToGridGuildMembers() 함수 더 추가
/// \author		cmkwon
/// \date		2007-07-27 ~ 2007-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::AddLineToGridGuildMembers(const char *i_szKey, CBitmap *i_pbmGuildMark)
{
	///////////////////////////////////////////////////////////////////////////////
	int nCurrentRow = m_GridGuild.GetRowCount();
	m_GridGuild.SetRowCount(nCurrentRow+1);

	GV_ITEM Item;
	Item.mask		= GVIF_TEXT|GVIF_FORMAT;
	Item.row		= nCurrentRow;
	Item.iImage		= 0;
	Item.nFormat	= GRID_CELL_FORMAT;

	Item.col		= 0;
	Item.strText	= i_szKey;
	m_GridGuild.SetItem(&Item);

	Item.mask		= GVIF_FORMAT|GVIF_IMAGE|GVIF_BKCLR;
	Item.col		= 1;
	Item.iImage		= m_ilGuildMarkList.GetImageCount();
	Item.crBkClr	= RGB(200, 200, 200);
	m_GridGuild.SetItem(&Item);

	COLORREF crRef = RGB(0,0,0);
	m_ilGuildMarkList.Add(i_pbmGuildMark, crRef);
	m_GridGuild.SetImageList(&m_ilGuildMarkList);
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::DisplayGuildInfo(SGUILD *i_pGUILD, UID32_t i_nGuildMemberCardCashPrice)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::DisplayGuildInfo(SGUILD *i_pGUILD, UID32_t i_nGuildMemberCardCashPrice)
{
	this->InitGridGuild();

	CString str;

	AddLineToGridGuildMembers("GuildName", i_pGUILD->GuildName);
	AddLineToGridGuildMembers("GuildUniqueNumber", i_pGUILD->GuildUID);
	AddLineToGridGuildMembers("GuildMaster CharacterUniqueNumber", i_pGUILD->GuildMasterCharacterUID);
	AddLineToGridGuildMembers("GuildMemberCapacity", i_pGUILD->GuildMemberCapacity);

	switch(i_pGUILD->GuildState)
	{
	case GUILD_STATE_NORMAL:			str.Format("GUILD_STATE_NORMAL");			break;
	case GUILD_STATE_DISMEMBER_READY:	str.Format("GUILD_STATE_DISMEMBER_READY");	break;
	case GUILD_STATE_IN_GUILD_WAR:		str.Format("GUILD_STATE_IN_GUILD_WAR");		break;
	case GUILD_STATE_CITYWAR:			str.Format("GUILD_STATE_CITYWAR");			break;
	default:
		{
			str.Format("GUILD_STATE_UNKNOWN");
		}
	}
	AddLineToGridGuildMembers("GuildState", str);
	AddLineToGridGuildMembers("GuildDismemberDate", i_pGUILD->GuildDismemberDate.GetDateTimeString().GetBuffer());
	AddLineToGridGuildMembers("GuildMarkVersion", i_pGUILD->GuildMarkVersion);
	AddLineToGridGuildMembers("GuildMarkSize", i_pGUILD->GuildMarkSize);
	AddLineToGridGuildMembers("GuildBattleWinCount", i_pGUILD->WarWinPoint);
	AddLineToGridGuildMembers("GuildBattleLossCount", i_pGUILD->WarLossPoint);
	AddLineToGridGuildMembers("GuildTotalFame", i_pGUILD->TotalFame);
	AddLineToGridGuildMembers("GuildMonthlyFame", i_pGUILD->MonthlyFame);
	
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 추가 필드 검색(, GuildMarkState)
	AddLineToGridGuildMembers("GuildMarkState", GetStringGuildMarkState(i_pGUILD->GuildMarkState, TRUE));

	///////////////////////////////////////////////////////////////////////////////
	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - 여단마크그리기
	CBitmap bitmap;
	if(LoadBMPFileFromMemory(&bitmap, i_pGUILD->GuildMarkImage, SIZE_MAX_GUILD_MARK_IMAGE))
	{
		AddLineToGridGuildMembers("GuildMarkImage", &bitmap);		
	}
	else
	{
		AddLineToGridGuildMembers("GuildMarkImage", "None");
	}

	//////////////////////////////////////////////////////////////////////////
	// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	AddLineToGridGuildMembers("GuildMemberCardCashPrice", i_nGuildMemberCardCashPrice);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::GetGuildInfo(const char *i_szGuildName, UID32_t i_guildUID/*=INVALID_UID32*/)
/// \brief			// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - 인자추가(, UID32_t i_guildUID=INVALID_UID32)
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::GetGuildInfo(const char *i_szGuildName, UID32_t i_guildUID/*=INVALID_UID32*/)
{
	if(NULL == i_szGuildName
		&& (INVALID_UID32 == i_guildUID || 0 == i_guildUID) )
	{	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - 체크 추가
		return FALSE;
	}

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 필요 없음
//	CString szSQLQuery;
// 	if(NULL != i_szGuildName)
// 	{
// 		// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템, // 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 추가 필드 검색(, GuildMarkState)
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("SELECT GuildUniqueNumber, GuildName, GuildCommanderUniqueNumber, GuildMemberCapacity, GuildState, \
// 		//	GuildDismemberDate, GuildMarkVersion, GuildMarkSize, WarWinPoint, WarLossPoint, TotalFame, MonthlyFame, GuildMarkImage, GuildMarkState \
// 		//		FROM td_Guild WITH (NOLOCK) WHERE GuildName = \'%s\'", i_szGuildName);
// 		szSQLQuery.Format(QUERY_080702_0017, i_szGuildName);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	}
// 	else
// 	{
// 		// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템, // 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 추가 필드 검색(, GuildMarkState)
// 		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 		//szSQLQuery.Format("SELECT GuildUniqueNumber, GuildName, GuildCommanderUniqueNumber, GuildMemberCapacity, GuildState, \
// 		//	GuildDismemberDate, GuildMarkVersion, GuildMarkSize, WarWinPoint, WarLossPoint, TotalFame, MonthlyFame, GuildMarkImage, GuildMarkState \
// 		//		FROM td_Guild WITH (NOLOCK) WHERE GuildUniqueNumber = %d", i_guildUID);
// 		szSQLQuery.Format(QUERY_080702_0018, i_guildUID);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	}

	// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템
	UID32_t		GuildMemberCardCashPrice = 0;
	SQLINTEGER	cb[16] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
							,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SGUILD		tmGUILD;					util::zero(&tmGUILD, sizeof(tmGUILD));
	SQL_TIMESTAMP_STRUCT tmTimeStamp;		util::zero(&tmTimeStamp, sizeof(tmTimeStamp));

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);

	// 2008-09-10 by cmkwon, 버그 수정
	char sztmGuildName[SIZE_MAX_GUILD_NAME];
	util::zero(sztmGuildName, SIZE_MAX_GUILD_NAME);
	if(NULL != i_szGuildName)
	{
		util::strncpy(sztmGuildName, i_szGuildName, SIZE_MAX_GUILD_NAME);
	}
	if(FALSE == IS_VALID_UNIQUE_NUMBER(i_guildUID))
	{
		i_guildUID = 0;
	}

	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, sztmGuildName, 0,				&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_guildUID, 0,									&arrCB2[2]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0017));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		return FALSE;
	}

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tmGUILD.GuildUID, 0,						&cb[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, tmGUILD.GuildName, SIZE_MAX_GUILD_NAME,		&cb[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG, &tmGUILD.GuildMasterCharacterUID, 0,		&cb[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &tmGUILD.GuildMemberCapacity, 0,			&cb[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &tmGUILD.GuildState, 0,					&cb[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_TIMESTAMP, &tmTimeStamp, 0,						&cb[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_ULONG, &tmGUILD.GuildMarkVersion, 0,				&cb[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_LONG, &tmGUILD.GuildMarkSize, 0,					&cb[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &tmGUILD.WarWinPoint, 0,					&cb[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &tmGUILD.WarLossPoint, 0,					&cb[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_LONG, &tmGUILD.TotalFame, 0,						&cb[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &tmGUILD.MonthlyFame, 0,					&cb[12]);	
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_BINARY, &tmGUILD.GuildMarkImage, SIZE_MAX_GUILD_MARK_IMAGE,	&cb[13]);	// 2007-07-26 by cmkwon, SCAdminTool에 GuildManager 수정 - 여단마크이미지 가져오기
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_TINYINT, &tmGUILD.GuildMarkState, 0,				&cb[14]);	// 2007-08-02 by cmkwon, 여단 마크 심사 시스템 구현 - 추가 필드 검색(, GuildMarkState)
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_ULONG, &GuildMemberCardCashPrice, 0,				&cb[15]);	// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템

	memset(&tmGUILD, 0x00, sizeof(SGUILD));
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	tmGUILD.GuildDismemberDate	= tmTimeStamp;		// 2007-05-15 by cmkwon, 버그 수정
	m_pODBCStmt->FreeStatement();		// clean up
	if (SQL_NO_DATA == bRet)
	{
		return FALSE;		
	}
	
	memcpy(&m_CurGUILD, &tmGUILD, sizeof(SGUILD));
	this->DisplayGuildInfo(&m_CurGUILD, GuildMemberCardCashPrice);	// 2008-05-27 by dhjin, EP3 여단 수정 사항 - 여단원 증가 캐쉬 아이템

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::GetGuildMembersInfo(SGUILD *i_pGUILD)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::GetGuildMembersInfo(SGUILD *i_pGUILD)
{
	CString szSQLQuery;
// 2007-07-31 by cmkwon, SCAdminTool에 여단명 변경 기능 추가 - 여단명 체크는 하지 않음
//	szSQLQuery.Format("SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, \
//		c.level, c.propensity, c.PKWinPoint, c.PKLossPoint, gm.GuildRank \
//			FROM td_character c, td_GuildMember gm WITH (NOLOCK) \
//			WHERE gm.GuildUniqueNumber = %d and c.uniqueNumber = gm.CharacterUniqueNumber and c.GuildName = \'%s\'"
//			, i_pGUILD->GuildUID, i_pGUILD->GuildName);

	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("SELECT c.AccountName, c.CharacterName, dbo.atum_GetGearString(c.unitkind), c.InfluenceType, \
	//	c.level, c.propensity, c.PKWinPoint, c.PKLossPoint, gm.GuildRank \
	//		FROM td_character c, td_GuildMember gm WITH (NOLOCK) \
	//		WHERE gm.GuildUniqueNumber = %d and c.uniqueNumber = gm.CharacterUniqueNumber"
	//		, i_pGUILD->GuildUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0019, i_pGUILD->GuildUID);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG	, SQL_INTEGER, 0, 0, &i_pGUILD->GuildUID, 0,		&arrCB2[1]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0019));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		return FALSE;
	}

	SQLINTEGER	arrCB[10] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	char szAccName[SIZE_MAX_ACCOUNT_NAME];
	char szCharName[SIZE_MAX_CHARACTER_NAME];
	char szUnitkind[10];
	BYTE byInfluenceType = 0;
	BYTE byLevel = 0;
	INT		nPropensity = 0;
	SHORT	sPKWinPoint = 0;
	SHORT	sPKLossPoint = 0;
	BYTE	byGuildRank = 0;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_CHAR, szAccName, SIZE_MAX_ACCOUNT_NAME,		&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, szCharName, SIZE_MAX_CHARACTER_NAME,	&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_CHAR, szUnitkind, 10,							&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_TINYINT, &byInfluenceType, 0,					&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_TINYINT, &byLevel, 0,							&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_LONG, &nPropensity, 0,						&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_SHORT, &sPKWinPoint, 0,						&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_SHORT, &sPKLossPoint, 0,						&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_UTINYINT, &byGuildRank, 0,					&arrCB[9]);
	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE;

	while(SQL_NO_DATA != (bRet = SQLFetch(m_pODBCStmt->m_hstmt)))
	{
		int nNewRowIdx = m_GridGuildMembers.GetRowCount();
		m_GridGuildMembers.SetRowCount(nNewRowIdx+1);

		//select된 값을 GridDetail GridCtrl에 넣어준다.
		Item.row		= nNewRowIdx;
		Item.col		= 0;
		Item.strText.Format("%d", nNewRowIdx);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%s", szAccName);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%s", szCharName);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%s-Gear", szUnitkind);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%s", CAtumSJ::GetInfluenceTypeString(byInfluenceType));
		m_GridGuildMembers.SetItem(&Item);

		Item.col++;		// 2006-03-07 by cmkwon, 5
		Item.strText.Format("%d", byLevel);
		m_GridGuildMembers.SetItem(&Item);		
		Item.col++;
		Item.strText.Format("%d", nPropensity);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%d", sPKWinPoint);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%d", sPKLossPoint);
		m_GridGuildMembers.SetItem(&Item);
		Item.col++;
		Item.strText.Format("%s", CAtumSJ::GetGuildRankString(byGuildRank));
		m_GridGuildMembers.SetItem(&Item);
	}
	m_GridGuildMembers.UpdateData();

	m_pODBCStmt->FreeStatement();	// clean up

	m_GridGuildMembers.AutoSize();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::GetGuildItems(SGUILD *i_pGUILD)
/// \brief		
/// \author		cmkwon
/// \date		2006-03-07 ~ 2006-03-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::GetGuildItems(SGUILD *i_pGUILD)
{
	/************************************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0020
	-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
	--						-- // 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가, 결과 필드 1개 추가
	--------------------------------------------------------------------------------
	************************************************************************/
	CString szSQLQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szSQLQuery.Format("SELECT s.UniqueNumber, i.ItemNum, s.PrefixCodeNum, s.SuffixCodeNum, s.ItemStorage, s.Wear \
// 						, s.ItemWindowIndex, s.CurrentCount, s.ColorCode, s.possess, s.CreatedTime \
// 						FROM td_store s, ti_item i WITH (NOLOCK) WHERE s.Possess = %d AND s.ItemStorage = %d AND s.ItemNum = i.ItemNum"
// 						, i_pGUILD->GuildUID, ITEM_IN_GUILD_STORE);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szSQLQuery.Format(QUERY_080702_0020, i_pGUILD->GuildUID, ITEM_IN_GUILD_STORE);	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	BYTE byItemStorageType = ITEM_IN_GUILD_STORE;
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_pGUILD->GuildUID), 0,		&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(byItemStorageType), 0,		&arrCB2[2]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0020));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		MessageBox("GetGuildItems ExecuteQuery Error !!");
		return FALSE;
	}

	SQLINTEGER	arrCB[13] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS ,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS
								, SQL_NTS,SQL_NTS,SQL_NTS};
	ITEM_GENERAL_READABLE StoreItemR;
	SQL_TIMESTAMP_STRUCT tmpCreatedTime;
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_SBIGINT, &StoreItemR.UniqueNumber, 0,				&arrCB[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG, &StoreItemR.ItemNum, 0,					&arrCB[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_LONG, &StoreItemR.PrefixCodeNum, 0,				&arrCB[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_LONG, &StoreItemR.SuffixCodeNum, 0,				&arrCB[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &StoreItemR.ItemStorage, 0,				&arrCB[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_UTINYINT, &StoreItemR.Wear, 0,					&arrCB[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_SLONG, &StoreItemR.ItemWindowIndex, 0,			&arrCB[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_SLONG, &StoreItemR.CurrentCount, 0,				&arrCB[8]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_LONG, &StoreItemR.ColorCode, 0,					&arrCB[9]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_LONG, &StoreItemR.Possess, 0,					&arrCB[10]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_TIMESTAMP, &tmpCreatedTime, 0,					&arrCB[11]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_LONG, &StoreItemR.ShapeItemNum, 0,				&arrCB[12]);	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - td_Store.ShapeItemNum 필드 추가

	vectITEM_GENERAL_READABLE vectItemRList;
	memset(&StoreItemR, 0x00, sizeof(ITEM_GENERAL_READABLE));
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		StoreItemR.CreatedTime = tmpCreatedTime;
		ITEM *pItemInfo = m_mapItemInfo.findEZ_ptr(StoreItemR.ItemNum);
		if(pItemInfo)
		{
			StoreItemR.Kind		= pItemInfo->Kind;
			StoreItemR.ItemInfo	= pItemInfo;
			util::strncpy(StoreItemR.ItemName, pItemInfo->ItemName, SIZE_MAX_ITEM_NAME);
			vectItemRList.push_back(StoreItemR);
		}
		memset(&StoreItemR, 0x00, sizeof(ITEM_GENERAL_READABLE));
	}
	m_pODBCStmt->FreeStatement();	// clean up
	
	for(int i= 0; i < vectItemRList.size(); i++)
	{
		///////////////////////////////////////////////////////////////////////////////
		// 2005-11-26 by cmkwon, Enchant Count를 가져온다.
		// 2010-02-24 by cmkwon, 아래와 같이 체크 수정
		//if(IS_WEAPON(vectItemRList[i].Kind))
		if(IS_ENCHANT_TARGET_ITEMKIND(vectItemRList[i].Kind))
		{
			// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
			//szSQLQuery.Format("select COUNT(*) from td_Enchant WITH (NOLOCK) where TargetItemUniqueNumber = %I64d", vectItemRList[i].UniqueNumber);
// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
// 			szSQLQuery.Format(QUERY_080702_0021, vectItemRList[i].UniqueNumber);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 			SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &vectItemRList[i].EnchantCount, 0, &arrCB[1]);
// 			BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 			if (!bRet)
// 			{
// 				MessageBox("Error : Get Enchant Count");
// 			}
// 			else
// 			{
// 				SQLFetch(m_pODBCStmt->m_hstmt);
// 			}
			SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
			SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
			SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UBIGINT, SQL_BIGINT, 0, 0, &(vectItemRList[i].UniqueNumber), 0,		&arrCB2[1]);			
			BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0021));
			if(FALSE == bRet)
			{
				MessageBox("Error : Get Enchant Count");
			}
			else
			{
				arrCB2[1] = SQL_NTS;
	 			SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_LONG, &vectItemRList[i].EnchantCount, 0, &arrCB2[1]);
				SQLFetch(m_pODBCStmt->m_hstmt);
			}
			m_pODBCStmt->FreeStatement();			// clean up
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 그리드에 추가한다
		m_GridGuildItems.SetRowCount(i+2);
		InsertItemToGridGuildItems(&vectItemRList[i], i+1);
	}
	m_GridGuildItems.AutoSize();
	m_GridGuildItems.ExpandColumnsToFit();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCGuildAdminDlg::InsertItemToGridGuildItems(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCGuildAdminDlg::InsertItemToGridGuildItems(ITEM_GENERAL_READABLE *i_pItemGeneral, int i_nRow)
{
	// 칼럼 만들기
	int m_nCols = 0;

	GV_ITEM Item;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.row = i_nRow;
	Item.nFormat = GRID_CELL_FORMAT;

	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	Item.col = m_nCols++;
	Item.strText.Format("%d", m_GridGuildItems.GetRowCount()-1);
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%I64d", i_pItemGeneral->UniqueNumber);
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pItemGeneral->ItemName);
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->ItemNum);
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;

	RARE_ITEM_INFO *pRareItemInfo = NULL;
	if(0 == i_pItemGeneral->PrefixCodeNum)
	{
		Item.strText.Format("(%d)", i_pItemGeneral->PrefixCodeNum);
	}
	else
	{
		RARE_ITEM_INFO *pRareItemInfo = m_mapRareItemInfo.findEZ_ptr(i_pItemGeneral->PrefixCodeNum);
		if (pRareItemInfo != NULL)
		{
			Item.strText.Format("%s(%d)", pRareItemInfo->Name, pRareItemInfo->CodeNum);
		}
		else
		{
			Item.strText.Format("Unknown(%d)", i_pItemGeneral->PrefixCodeNum);
		}
	}
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	if(0 == i_pItemGeneral->SuffixCodeNum)
	{
		Item.strText.Format("(%d)", i_pItemGeneral->SuffixCodeNum);
	}
	else
	{	pRareItemInfo = m_mapRareItemInfo.findEZ_ptr(i_pItemGeneral->SuffixCodeNum);
		if (pRareItemInfo != NULL)
		{
			Item.strText.Format("%s(%d)", pRareItemInfo->Name, pRareItemInfo->CodeNum);
		}
		else
		{
			Item.strText.Format("Unknown(%d)", i_pItemGeneral->SuffixCodeNum);
		}
	}
	m_GridGuildItems.SetItem(&Item);

	// 2005-11-26 by cmkwon, 추가됨
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->EnchantCount);
	m_GridGuildItems.SetItem(&Item);


	Item.col = m_nCols++;
	if (i_pItemGeneral->Wear == WEAR_NOT_ATTACHED)
		Item.strText = STRMSG_S_SCAT_COLNAME_0025;
	if (i_pItemGeneral->Wear == WEAR_ATTACHED)
		Item.strText = STRMSG_S_SCAT_COLNAME_0021;
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;

	if(ITEM_IN_STORE == i_pItemGeneral->ItemStorage)		// 2005-12-08 by cmkwon
	{
		Item.strText.Format("Storage");
	}
	else if(ITEM_IN_GUILD_STORE == i_pItemGeneral->ItemStorage)
	{
		Item.strText.Format("Guild Storage");
	}
	else
	{
		// start 2011-09-30 by hskim, 파트너 시스템 2차
		/////////
		// 기존
		// Item.strText.Format("%d", i_pItemGeneral->ItemWindowIndex);

		/////////
		// 수정
		if( POS_HIDDEN_ITEM == i_pItemGeneral->ItemWindowIndex )
		{
			Item.strText.Format("Hidden");
		}
		else
		{
		Item.strText.Format("%d", i_pItemGeneral->ItemWindowIndex);
		}
		// end 2011-09-30 by hskim, 파트너 시스템 2차
	}
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->CurrentCount);
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->ColorCode);
	m_GridGuildItems.SetItem(&Item);

	// 2009-08-26 by cmkwon, 그래픽 리소스 변경 시스템 구현 - ShapeItemNum 필드 추가
	Item.col = m_nCols++;
	Item.strText.Format("%d", i_pItemGeneral->ShapeItemNum);
	m_GridGuildItems.SetItem(&Item);

	Item.col = m_nCols++;
	Item.strText.Format("%s", i_pItemGeneral->CreatedTime.GetDateTimeString().GetBuffer());
	m_GridGuildItems.SetItem(&Item);	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::LoadBMPFileFromMemory(CBitmap *o_pBitmap, char *i_pData, int i_nDataLen)
/// \brief		// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - LoadBMPFileFromMemory() 함수 추가
/// \author		cmkwon
/// \date		2007-07-27 ~ 2007-07-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::LoadBMPFileFromMemory(CBitmap *o_pBitmap, char *i_pData, int i_nDataLen)
{
	BITMAPFILEHEADER *pBmfHeader;
	INT nUsedBytes = 0;

	// Read file header
	if (sizeof(BITMAPFILEHEADER) > i_nDataLen){					return FALSE;}
	pBmfHeader = (BITMAPFILEHEADER*)i_pData;
	nUsedBytes += sizeof(BITMAPFILEHEADER);

	// File type should be 'BM'
	if (pBmfHeader->bfType != ((WORD) ('M' << 8) | 'B')){		return FALSE;}

	if(sizeof(BITMAPINFOHEADER) > i_nDataLen - nUsedBytes
		|| sizeof(BITMAPINFO) > i_nDataLen - nUsedBytes)
	{
		return FALSE;
	}
	
	BITMAPINFOHEADER	*pBmiHeader	= (BITMAPINFOHEADER*)(i_pData + nUsedBytes);
	BITMAPINFO			*pBmInfo	= (BITMAPINFO*)(i_pData + nUsedBytes);

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;

	LPVOID lpDIBBits;
	if( pBmInfo->bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(pBmInfo->bmiColors + pBmInfo->bmiHeader.biClrUsed) +
			((pBmInfo->bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(pBmInfo->bmiColors + nColors);

	CClientDC dc(NULL);
	HBITMAP hBmp = CreateDIBitmap(dc.m_hDC,	pBmiHeader, CBM_INIT, lpDIBBits, pBmInfo, DIB_RGB_COLORS);
	o_pBitmap->Attach(hBmp);
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::SearchGuildNameFromDB(char *i_szGuildName)
/// \brief		// 2007-07-31 by cmkwon, SCAdminTool에 여단명 변경 기능 추가 - DB에서 여단명 검색 함수 추가
/// \author		dhjin
/// \date		2007-07-31 ~ 2007-07-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::SearchGuildNameFromDB(char *i_szGuildName)
{
	CString csQuery;
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//csQuery.Format("SELECT GuildUniqueNumber FROM td_Guild WITH (NOLOCK) \
	//		WHERE GuildName = \'%s\'", i_szGuildName);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	csQuery.Format(QUERY_080702_0022, i_szGuildName);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 

	SQLINTEGER	arrCB[2] = {SQL_NTS,SQL_NTS};
	UID32_t guildUID = 0;

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)csQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, i_szGuildName, 0,		&arrCB2[1]);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0022));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		AfxMessageBox("DB Query error !!");
		return TRUE;	// 해당 여단이 있는 것으로 리턴함
	}
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &guildUID, 0,		&arrCB[1]);

	SQLRETURN nReturn = SQLFetch(m_pODBCStmt->m_hstmt);	
	m_pODBCStmt->FreeStatement();	// clean up
	if(SQL_NO_DATA != nReturn)
	{
		return TRUE;	// 해당 여단이 있는 것으로 리턴함
	}

	return FALSE;	// 해당 여단이 없음
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CSCGuildAdminDlg::UpdateGuild2DB(SGUILD *i_pGuild)
/// \brief		// 2007-07-31 by cmkwon, SCAdminTool에 여단명 변경 기능 추가 - DB 여단 업데이트 함수 추가
/// \author		dhjin
/// \date		2007-07-31 ~ 2007-07-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CSCGuildAdminDlg::UpdateGuild2DB(SGUILD *i_pGuild)
{
	CString csQuery;
	// 2007-08-03 by cmkwon, SCAdminTool에서 여단 마크 상대 변경 기능 구현 - 쿼리 수정
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//csQuery.Format("UPDATE td_Guild SET GuildName = \'%s\', GuildMarkState= %d WHERE GuildUniqueNumber = %d", i_pGuild->GuildName, i_pGuild->GuildMarkState, i_pGuild->GuildUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	csQuery.Format(QUERY_080702_0023, i_pGuild->GuildName, i_pGuild->GuildMarkState, i_pGuild->GuildUID);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//
//	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)csQuery);
	SQLHSTMT hstmt = m_pODBCStmt->GetSTMTHandle();
	SQLINTEGER arrCB2[4] = {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_pGuild->GuildUID), 0,					&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, i_pGuild->GuildName, 0,	&arrCB2[2]);
	SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &(i_pGuild->GuildMarkState), 0,			&arrCB2[3]);	
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0023));
	if (!bRet)
	{
		m_pODBCStmt->FreeStatement();		// clean up
		AfxMessageBox("DB Query error !!(CSCGuildAdminDlg::UpdateGuild2DB_)");
		return FALSE;
	}
	m_pODBCStmt->FreeStatement();	// clean up

	// 여단원들 모든 캐릭터 테이블에 여단명 변경
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//csQuery.Format("UPDATE td_Character SET GuildName = \'%s\' WHERE GuildUniqueNumber = %d", i_pGuild->GuildName, i_pGuild->GuildUID);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	csQuery.Format(QUERY_080702_0024, i_pGuild->GuildName, i_pGuild->GuildUID);		// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
//	bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)csQuery);
	hstmt = m_pODBCStmt->GetSTMTHandle();
	arrCB2[1] = arrCB2[2] = SQL_NTS;
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &(i_pGuild->GuildUID), 0,					&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_GUILD_NAME, 0, i_pGuild->GuildName, 0,	&arrCB2[2]);
	bRet = m_pODBCStmt->ExecuteQuery((char*)(PROCEDURE_080827_0024));
	m_pODBCStmt->FreeStatement();	// clean up
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CSCGuildAdminDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-09-27 ~ 2006-09-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CSCGuildAdminDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;

	if (pItem->iRow != 0)
	{
		return;
	}

	// 2007-12-06 by cmkwon, SCAdminTool 수정(아이템리스트에 인덱스 추가하기) - 
	switch(pItem->iColumn) {
	case 0:
	case 1:		
	case 3:
	case 6:
	case 8:
	case 9:
		m_GridGuildItems.SetCompareFunction(CGridCtrl::pfnCellNumericCompare);
		break;
	default:
		m_GridGuildItems.SetCompareFunction(CGridCtrl::pfnCellTextCompare);
	}

    if (pItem->iColumn == m_GridGuildItems.GetSortColumn())
        m_GridGuildItems.SortItems(pItem->iColumn, !m_GridGuildItems.GetSortAscending());
    else
        m_GridGuildItems.SortItems(pItem->iColumn, TRUE);

	m_GridGuildItems.AutoSize();
	m_GridGuildItems.ExpandColumnsToFit();
}

// 2007-07-26 by cmkwon, SCAdminTool에 GuildManager 수정 - OnBtnSearchguildByguid()함수 추가
void CSCGuildAdminDlg::OnBtnSearchguildByguid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	///////////////////////////////////////////////////////////////////////////////	
	// 2007-07-27 by cmkwon, SCAdminTool에 GuildManager 수정 - 길드명 검색창 초기화
	m_ctlstrSearchGuildName.Empty();
	UpdateData(FALSE);

	util::zero(&m_CurGUILD, sizeof(m_CurGUILD));
	this->InitGridGuild();
	this->InitGridGuildMembers();
	this->InitGridGuildItems();

	if(0 == m_ctluiSearchGuildUID)
	{
		AfxMessageBox("Please, input GuildUID");
		return;
	}

	if(FALSE == this->GetGuildInfo(NULL, m_ctluiSearchGuildUID))
	{
		AfxMessageBox("Corresponding guild does not exist !!");
		return;
	}

	this->GetGuildMembersInfo(&m_CurGUILD);
	this->GetGuildItems(&m_CurGUILD);
}

void CSCGuildAdminDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CSCGuildAdminDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}

void CSCGuildAdminDlg::OnBtnEditGuild() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == IS_VALID_UNIQUE_NUMBER(m_CurGUILD.GuildUID))
	{
		AfxMessageBox("Guild information is invalid !!\n First, search guild.");
		return;
	}

	if(FALSE == m_pMainDlg->IsManagerAdministrator())
	{// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 
		AfxMessageBox(ADSTRMSG_090204_0001);
		return;
	}
	
	CEditGuildDlg dlg(&m_CurGUILD, m_pMainDlg->IsManagerAdministrator(), this);

	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}

	if(0 != strncmp(m_CurGUILD.GuildName, dlg.m_guildInfo.GuildName, SIZE_MAX_GUILD_NAME)
		|| m_CurGUILD.GuildMarkState != dlg.m_guildInfo.GuildMarkState)
	{// 2007-08-03 by cmkwon, SCAdminTool에서 여단 마크 상대 변경 기능 구현
		if(FALSE == this->UpdateGuild2DB(&dlg.m_guildInfo))
		{
			return;
		}

		m_ctluiSearchGuildUID	= dlg.m_guildInfo.GuildUID;
		UpdateData(FALSE);

		this->OnBtnSearchguildByguid();
	}
}

void CSCGuildAdminDlg::OnOK() 
{
	// TODO: Add extra validation here

	CWnd *pWnd = this->GetFocus();
	if(pWnd)
	{
		int ctrlID = pWnd->GetDlgCtrlID();

		if(IDC_EDIT_SEARCH_GUILDNAME == ctrlID)
		{
			OnBtnSearchguild();
			return;
		}
		else if(IDC_EDIT_SEARCH_GUILDUID == ctrlID)
		{
			OnBtnSearchguildByguid();
			return;
		}
	}
	
	CDialog::OnOK();
}

void CSCGuildAdminDlg::OnBtnGuildMarkRecognition() 
{
	// TODO: Add your control notification handler code here

	// 2009-02-04 by cmkwon, AdminTool의 GM 권한 수정 - 인자추가(, BOOL i_bEnableEdit)
	CGuildMarkRecognitionDlg dlg(m_pODBCStmt, m_pMainDlg->IsManagerAdministrator(), this);
	if(IDCANCEL == dlg.DoModal())
	{
		return;
	}

	if(IS_VALID_UNIQUE_NUMBER(m_CurGUILD.GuildUID))
	{
		m_ctluiSearchGuildUID	= m_CurGUILD.GuildUID;
		UpdateData(FALSE);
		this->OnBtnSearchguildByguid();
	}	
}
