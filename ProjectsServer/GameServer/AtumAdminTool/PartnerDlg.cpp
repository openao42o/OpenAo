// PartnerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "PartnerDlg.h"
#include "AtumAdminToolDlg.h"
#include "AtumDBLogWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartnerDlg dialog


CPartnerDlg::CPartnerDlg(CAtumAdminToolDlg *i_pMainDlg, CSCUserAdminDlg *i_pUserAdminDlg, UID64_t i_StoreUniqueNumber, CWnd* pParent /*=NULL*/)
: CDialog(CPartnerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPartnerDlg)
	// NOTE: the ClassWizard will add member initialization here
	m_szAccountName = _T("");
	m_szCharacterName = _T("");
	m_szPartnerName = _T("");
	m_nPartnerType = 0;
	m_nPartnerLvMax = 0;
	m_nPartnerLvCurrent = 0;
	m_nPartnerExpMax = 0;
	m_nPartnerExpCurrent = 0;
	m_nPartnerExpPercentage = 0;
	//}}AFX_DATA_INIT
	util::zero(m_nPartnerExpLevel,sizeof(m_nPartnerExpLevel));
	vectSocketUniqueNumber.clear();
	m_pMainDlg = i_pMainDlg;
	m_pUserAdminDlg			= i_pUserAdminDlg;
	m_nStoreUniqueNumber = i_StoreUniqueNumber;
	util::zero(&m_tPartnerInfo,sizeof(m_tPartnerInfo));
	util::zero(&m_tUpdatePartnerInfo,sizeof(m_tUpdatePartnerInfo));
	m_pODBCStmt = new CODBCStatement;
}

CPartnerDlg::~CPartnerDlg()
{
	util::del(m_pODBCStmt);
}


void CPartnerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPartnerDlg)
	DDX_Control(pDX, IDC_COMBO_PARTNER_POSSESS_SOCKET_LIST, m_CComboPartnerProssessSocketList);
	DDX_Control(pDX, IDC_EDIT_PARTNER_LV_CURRENT_SPIN, m_PartnerLvCurrentSpin);
	DDX_Text(pDX, IDC_EDIT_PARTNER_ACCOUNT_NAME, m_szAccountName);
	DDX_Text(pDX, IDC_EDIT_PARTNER_CHARACTER, m_szCharacterName);
	DDX_Text(pDX, IDC_EDIT_PARTNER_NAME, m_szPartnerName);
	DDX_Text(pDX, IDC_EDIT_PARTNER_TYPE, m_nPartnerType);
	DDX_Text(pDX, IDC_EDIT_PARTNER_LV_MAX, m_nPartnerLvMax);
	DDX_Text(pDX, IDC_EDIT_PARTNER_LV_CURRENT, m_nPartnerLvCurrent);
	DDX_Text(pDX, IDC_EDIT_PARTNER_EXP_MAX, m_nPartnerExpMax);
	DDX_Text(pDX, IDC_EDIT_PARTNER_EXP_CURRENT, m_nPartnerExpCurrent);
	DDX_Text(pDX, IDC_EDIT_PARTNER_EXP_PERCENTAGE, m_nPartnerExpPercentage);
	DDX_Control(pDX, IDC_SLIDER_PARTNER, m_PartnerExpSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPartnerDlg, CDialog)
//{{AFX_MSG_MAP(CPartnerDlg)
ON_BN_CLICKED(IDC_BTN_PARTNER_SOCK_DEL, OnSockDel)
	ON_WM_HSCROLL()
	ON_EN_UPDATE(IDC_EDIT_PARTNER_LV_CURRENT, OnUpdateEditPartnerLvCurrent)
	ON_EN_UPDATE(IDC_EDIT_PARTNER_EXP_CURRENT, OnUpdateEditPartnerExpCurrent)
	ON_EN_UPDATE(IDC_EDIT_PARTNER_EXP_PERCENTAGE, OnUpdateEditPartnerExpPercentage)
	ON_EN_UPDATE(IDC_EDIT_PARTNER_NAME, OnUpdateEditPartnerName)
	ON_BN_CLICKED(IDC_BTN_PARTNER_SAVE, OnBtnPartnerSave)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartnerDlg message handlers


BOOL CPartnerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pODBCStmt->FreeStatement();
	m_szAccountName		= m_pUserAdminDlg->m_szAccountName;
	m_szCharacterName	= m_pUserAdminDlg->m_szCurrentCharacterName;
	m_CComboPartnerProssessSocketList.Clear();
	vectSocketUniqueNumber.clear();
	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd()))
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}
	
	SQLINTEGER arrCB2 = SQL_NTS;
	SQLBindParameter ( m_pODBCStmt->m_hstmt , 1 , SQL_PARAM_INPUT , SQL_C_UBIGINT , SQL_BIGINT , 0 , 0 , &m_nStoreUniqueNumber , 0 , NULL );
	/************************************************************
	-------------------------------------------------------------
	-----관련 프로시저
	-- // 2010-06-15 by shcho&hslee 펫시스템 - 개인 펫 정보 로딩 
	-- // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Load_td_petInfo]
	@i_ItemUID BIGINT
	AS
	BEGIN
	SELECT ItemUID, PetIndex, Name, [Level], Experience, ExpRatio, EnableSocketCount,
	SocketItem1, SocketItem2, SocketItem3, SocketItem4, SocketItem5, SocketItem6,
	HPKitItemNum, HPKitTriggerValue, ShieldKitItemNum, ShieldKitTriggerValue, SPKitItemNum, SPKitTriggerValue,
	AutoSkilItemNum1, AutoSkilItemNum2, AutoSkilItemNum3, AutoSkilItemNum4, AutoSkilItemNum5, AutoSkilItemNum6 
	FROM dbo.td_pet  WHERE ItemUID = @i_ItemUID;
	END
	GO
	*************************************************************/
	BOOL bRet = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_100615_0573, SQL_NTS);
	if ( bRet != SQL_SUCCESS && bRet != SQL_SUCCESS_WITH_INFO )
	{
		MessageBox(STRERR_S_SCADMINTOOL_0050);
		return FALSE;
	}
	

	SQLINTEGER cb[25] = { 0, };

	int iIndex = 0, iArrIndex = 0;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,	&m_tPartnerInfo.ItemUID,				0,					&cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&m_tPartnerInfo.PartnerIndex,			0, 					&cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_CHAR,		 m_tPartnerInfo.name,					SIZE_MAX_PET_NAME,	&cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&m_tPartnerInfo.level,					0, 					&cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&m_tPartnerInfo.Experience,				0, 					&cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&m_tPartnerInfo.ExpRatio,				0, 					&cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_ULONG,		&m_tPartnerInfo.EnableSocketCount,		0, 					&cb[iIndex]);	iIndex++;
	for(iArrIndex = 0; iArrIndex < SIZE_MAX_PETSOCKET; iArrIndex++)
	{
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,	&m_tPartnerInfo.SocketItem[iArrIndex],	0, &cb[iIndex]);	iIndex++;
	}
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,		&m_tPartnerInfo.HPKitItemNum,			0, &cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_FLOAT,			&m_tPartnerInfo.HPKitTriggerValue,		0, &cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,		&m_tPartnerInfo.ShieldKitItemNum,		0, &cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_FLOAT,			&m_tPartnerInfo.ShieldKitTriggerValue,	0, &cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,		&m_tPartnerInfo.SPKitItemNum,			0, &cb[iIndex]);	iIndex++;
	SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_FLOAT,			&m_tPartnerInfo.SPKitTriggerValue,		0, &cb[iIndex]);	iIndex++;
	for(iArrIndex = 0; iArrIndex < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; iArrIndex++)
	{
		SQLBindCol(m_pODBCStmt->m_hstmt, iIndex+1, SQL_C_SBIGINT,	&m_tPartnerInfo.AutoSkilItemNum[iArrIndex],		0, &cb[iIndex]);	iIndex++;
	}
	
	bRet = SQLFetch(m_pODBCStmt->m_hstmt);
	
	if ( ( bRet != SQL_SUCCESS && bRet != SQL_SUCCESS_WITH_INFO ) || bRet == SQL_NO_DATA )
	{
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		MessageBox("ERROR : SQLFetch");
		return FALSE;
	}
	SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);

	int tmpPartnerIndex = PARTNER_TYPE_GROWTH_SOCKET_1;
	if(PARTNER_TYPE_DEFAULT == m_tPartnerInfo.PartnerIndex)
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &tmpPartnerIndex, 0, NULL);
	else
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &m_tPartnerInfo.PartnerIndex, 0, NULL);
	/************************************************************
	-------------------------------------------------------------
	-----관련 프로시저
	-- // 2010-06-15 by shcho&hslee 펫시스템 - 레벨 베이스 테이블 정보 로드( 엑셀밀기 툴관련 처리 )
	-- // 2011-08-05 by hskim, 파트너 시스템 2차 - 자료 구조 결정
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Load_ti_petLevel]
	@i_PetIndex INT
	AS
	BEGIN
	SELECT PetIndex, [Level], NeedExp, UseWeaponIndex, SlotCountSkill, SlotCountSocket, KitLevelHP, KitLevelShield, KitLevelSP 
	FROM atum2_db_account.dbo.ti_PetLevel WHERE PetIndex = @i_PetIndex;
	END
	GO
	*************************************************************/
	bRet = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_100615_0570, SQL_NTS);
	if ( bRet != SQL_SUCCESS && bRet != SQL_SUCCESS_WITH_INFO )
	{
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		MessageBox(STRERR_S_SCADMINTOOL_0055);
		return FALSE;
	}
	PARTNER_LIVEL_INFO tmpPartnerLevelInfo;
	util::zero(cb, sizeof(cb));
	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG,		&tmpPartnerLevelInfo.nPartherIndex,		1,	&cb[0]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_ULONG,		&tmpPartnerLevelInfo.nPartherLevel,		1,	&cb[1]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_ULONG,		&tmpPartnerLevelInfo.nExperience,		1,	&cb[2]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_ULONG,		&tmpPartnerLevelInfo.nUseWeaponIndex,	1,	&cb[3]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_ULONG,		&tmpPartnerLevelInfo.nSlotCountSkill,	1,	&cb[4]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_ULONG,		&tmpPartnerLevelInfo.nSlotCountSocket,	1,	&cb[5]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_ULONG,		&tmpPartnerLevelInfo.nKitLevelHP,		1,	&cb[6]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_ULONG,		&tmpPartnerLevelInfo.nKitLevelShield,	1,	&cb[7]);
	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_ULONG,		&tmpPartnerLevelInfo.nKitLevelSP,		1,	&cb[8]);
	SQLRETURN sqlReturn;
	while ((sqlReturn = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (sqlReturn != SQL_SUCCESS)
		{
			SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
			MessageBox("ERROR : SQLFetch");
			return FALSE;
		}
		m_nPartnerExpLevel[tmpPartnerLevelInfo.nPartherLevel] = tmpPartnerLevelInfo.nExperience;
	}
	SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);

	m_tUpdatePartnerInfo = m_tPartnerInfo;
	m_PartnerLvCurrentSpin.SetRange(1,25);
	m_szPartnerName			= (CString)m_tUpdatePartnerInfo.name;
	m_nPartnerType			= m_tUpdatePartnerInfo.PartnerIndex;
	m_nPartnerLvMax			= PARTNER_MAX_LEVEL;
	m_nPartnerLvCurrent		= m_tUpdatePartnerInfo.level;
	m_nPartnerExpCurrent	= m_tUpdatePartnerInfo.Experience;
	m_nPartnerExpMax		= m_nPartnerExpLevel[m_nPartnerLvCurrent];
	if(m_nPartnerLvCurrent > 1) 
	{
		m_nPartnerExpCurrent = m_nPartnerExpCurrent	-m_nPartnerExpLevel[m_nPartnerLvCurrent-1];
		m_nPartnerExpMax	 = m_nPartnerExpMax		-m_nPartnerExpLevel[m_nPartnerLvCurrent-1];
	}
	m_nPartnerExpPercentage = m_nPartnerExpCurrent/(m_nPartnerExpMax/100);
	if(m_nPartnerExpCurrent < 0) m_nPartnerExpCurrent = 0;
	m_PartnerExpSlider.SetRange(0,99);
	m_PartnerExpSlider.SetPos(m_nPartnerExpPercentage);
	if(PARTNER_TYPE_DEFAULT == m_tUpdatePartnerInfo.PartnerIndex)
	{
		GetDlgItem(IDC_EDIT_PARTNER_LV_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PARTNER_EXP_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PARTNER_EXP_PERCENTAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_PARTNER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PARTNER_SOCK_DEL)->EnableWindow(FALSE);
		m_CComboPartnerProssessSocketList.AddString("Non-Use");
		m_CComboPartnerProssessSocketList.SetCurSel(0);
		m_CComboPartnerProssessSocketList.EnableWindow(FALSE);
		vectSocketUniqueNumber.push_back(0);
	}
	else
	{
		CString itempProssessSocketUniqueNumber = _T("");
		m_CComboPartnerProssessSocketList.AddString("Empty");
		vectSocketUniqueNumber.push_back(0);
		int i;
		for(i= 0; i < m_pUserAdminDlg->m_vectCurrentCharacterGetItemList.size(); i++)
		{
			if(PARTNER_SOCKET_ITEM_KIT == m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].ItemNum )
			{
				if(ITEM_HIDDEN != m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].ItemWindowIndex)
				{
					itempProssessSocketUniqueNumber.Format(_T("%12d"), m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
					m_CComboPartnerProssessSocketList.AddString("Kit   ("+itempProssessSocketUniqueNumber+")");
					vectSocketUniqueNumber.push_back(m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
				}
				else if(m_tUpdatePartnerInfo.SocketItem[0] == m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber)
				{
					itempProssessSocketUniqueNumber.Format(_T("%12d"), m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
					m_CComboPartnerProssessSocketList.AddString("Kit   ("+itempProssessSocketUniqueNumber+")");
					vectSocketUniqueNumber.push_back(m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
				}
			}
			else if(PARTNER_SOCKET_ITEM_BUFF == m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].ItemNum)
			{
				if(ITEM_HIDDEN != m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].ItemWindowIndex)
				{
					itempProssessSocketUniqueNumber.Format(_T("%12d"), m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
					m_CComboPartnerProssessSocketList.AddString("Buff ("+itempProssessSocketUniqueNumber+")");
					vectSocketUniqueNumber.push_back(m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
				}
				else if(m_tUpdatePartnerInfo.SocketItem[0] == m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber)
				{
					itempProssessSocketUniqueNumber.Format(_T("%12d"), m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
					m_CComboPartnerProssessSocketList.AddString("Buff ("+itempProssessSocketUniqueNumber+")");
					vectSocketUniqueNumber.push_back(m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber);
				}
			}
		}
		
		for( i= 0; i < vectSocketUniqueNumber.size(); i++)
		{
			if(m_tUpdatePartnerInfo.SocketItem[0] == vectSocketUniqueNumber[i])
			{
				m_CComboPartnerProssessSocketList.SetCurSel(i);
				break;
			}
		}
	}
	UpdateData(FALSE);
	return  TRUE;
}

void CPartnerDlg::OnBtnPartnerSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	RETCODE ret = SQL_ERROR;
	if(m_nPartnerExpCurrent < 0) m_nPartnerExpCurrent = 0;
	m_tUpdatePartnerInfo.level	= m_nPartnerLvCurrent;
	m_tUpdatePartnerInfo.Experience = m_nPartnerExpCurrent+m_nPartnerExpLevel[m_nPartnerLvCurrent-1];
	

	m_tUpdatePartnerInfo.SocketItem[0] = vectSocketUniqueNumber[m_CComboPartnerProssessSocketList.GetCurSel()];
	
	UINT64 iUniqueNumber = m_tUpdatePartnerInfo.SocketItem[0];
	UINT64 iOriUniqueNumber = m_tPartnerInfo.SocketItem[0];
	
	if(m_tPartnerInfo.SocketItem[0] != m_tUpdatePartnerInfo.SocketItem[0])
	{
		int i;
		for( i = 1; i < SIZE_MAX_PETSOCKET; i++) m_tUpdatePartnerInfo.SocketItem[i] = 0;
		for( i = 0; i < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; i++) m_tUpdatePartnerInfo.AutoSkilItemNum[i] = 0;
		m_tUpdatePartnerInfo.HPKitItemNum = 0;
		m_tUpdatePartnerInfo.ShieldKitItemNum = 0;
		m_tUpdatePartnerInfo.SPKitItemNum = 0;
		if(PARTNER_SOCKET_NON_USE != m_tPartnerInfo.SocketItem[0] ||
			PARTNER_SOCKET_EMPTY != m_tPartnerInfo.SocketItem[0])
		{
			SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &m_tPartnerInfo.SocketItem[0], 0,		NULL);
			/*[Query Definition]************************************************
			--------------------------------------------------------------------------------
			-- PROCEDURE NAME	: dbo.atum_PROCEDURE_080827_0033
			-- DESC				: -- // 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
			--------------------------------------------------------------------------------
			ALTER PROCEDURE [dbo].[atum_PROCEDURE_080827_0033]
			@i_ItemUID					BIGINT
			-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
			AS
			DELETE
			FROM td_Store
			WHERE uniquenumber = @i_ItemUID;
			Go
			**************************************************************/	
			ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080827_0033, SQL_NTS);
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				MessageBox(STRERR_S_SCADMINTOOL_0051);
				SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
				return;
			}
			SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
			
			
			// write log  :  아이템 삭제 로그
			FL_ITEM_LOG_BASE logBase;
			logBase.CharacterUniqueNumber = m_pUserAdminDlg->m_CurrentCharacter.CharacterUniqueNumber;
			logBase.CurrentMapIndex = 0;
			ITEM_FOR_LOG logItem;
			for(int i= 0; i < m_pUserAdminDlg->m_vectCurrentCharacterGetItemList.size(); i++)
			{
				if(m_tPartnerInfo.SocketItem[0] == m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].UniqueNumber)
				{
					logItem.ItemNum = m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].ItemNum;
					logItem.ItemUniqueNumber = m_tPartnerInfo.SocketItem[0];	
					logItem.CurrentCount = m_pUserAdminDlg->m_vectCurrentCharacterGetItemList[i].CurrentCount;
					CAtumDBLogWriter::InsertLog_Item_Trade(m_pODBCStmt, T1_FL_LOG_ITEM_DEL_ITEM_BY_ADMIN, &logBase, &logItem, 0, (char*)(LPCSTR)m_pMainDlg->m_UID, 0);
				}
			}
		}
		
		if(PARTNER_SOCKET_NON_USE != m_tUpdatePartnerInfo.SocketItem[0] ||
			PARTNER_SOCKET_EMPTY != m_tUpdatePartnerInfo.SocketItem[0])
		{
			int iWear = ITEM_NOTWEAR, iWinIdx = ITEM_HIDDEN;
			SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0,	&m_tUpdatePartnerInfo.SocketItem[0], 0, NULL);
			SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_TINYINT, 0, 0,	&iWear, 0, NULL);
			SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0,	&iWinIdx, 0, NULL);
			/*[Query Definition]************************************************
			-------------------------------------------------------------
			-- !!!!
			-- Name:
			-- Desc:
			-- ====
			CREATE PROCEDURE dbo.atum_UpdateItemWindowList
			@UniqueNumber	BIGINT,
			@Wear			TINYINT,
			@WndIdx			INT
			-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
			AS
			UPDATE td_Store
			SET ItemWindowIndex = @WndIdx, Wear = @Wear
			WHERE UniqueNumber = @UniqueNumber;
			GO
			**********************************************************************/
			ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_080822_0109, SQL_NTS);
			
			if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				MessageBox(STRERR_S_SCADMINTOOL_0052);
				SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
				return;
			}
			
			SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		}
		
		for( i = 0; i < SIZE_MAX_PETSOCKET; i++)
		{
			SQLBindParameter(m_pODBCStmt->m_hstmt, i + 2, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &m_tUpdatePartnerInfo.SocketItem[i], 0, NULL);	
		}
		/*[Query Definition]************************************************
		-------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_Update_PetSocket
		-- DESC				: 펫 소켓 아이템 설정
		-- // 2011-08-30 by hskim, 파트너 시스템 2차
		-------------------------------------------------------------
		CREATE PROC [dbo].[atum_Update_PetSocket]
		@i_ItemUID		BIGINT,
		@i_SocketItem1	INT,
		@i_SocketItem2	INT,
		@i_SocketItem3	INT,
		@i_SocketItem4	INT,
		@i_SocketItem5	INT,
		@i_SocketItem6	INT
		AS
		BEGIN
		UPDATE [dbo].[td_pet] 
		SET [SocketItem1] = @i_SocketItem1, [SocketItem2] = @i_SocketItem2, [SocketItem3] = @i_SocketItem3, 
		[SocketItem4] = @i_SocketItem4, [SocketItem5] = @i_SocketItem5, [SocketItem6] = @i_SocketItem6 WHERE [ItemUID] = @i_ItemUID;
		END
		GO
		**************************************************************************/
		SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &m_tUpdatePartnerInfo.ItemUID, 0, NULL);
		ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_110824_0005, SQL_NTS);
		if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
		{
			MessageBox(STRERR_S_SCADMINTOOL_0053);
			SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
			return;
		}
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);

	}

	if(m_tPartnerInfo.level != m_tUpdatePartnerInfo.level)
	{
		for(int i = 0; i < SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; i++) m_tUpdatePartnerInfo.AutoSkilItemNum[i] = 0;
		m_tUpdatePartnerInfo.HPKitItemNum = 0;
		m_tUpdatePartnerInfo.ShieldKitItemNum = 0;
		m_tUpdatePartnerInfo.SPKitItemNum = 0;
	}
	
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &m_tUpdatePartnerInfo.ItemUID, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_FLOAT, 0, 0, &m_tUpdatePartnerInfo.Experience, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_tUpdatePartnerInfo.level, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, m_tUpdatePartnerInfo.name, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_TINYINT, 0, 0, &m_tUpdatePartnerInfo.ExpRatio, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 6, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_TINYINT, 0, 0, &m_tUpdatePartnerInfo.EnableSocketCount, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 7, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_INTEGER, 0, 0, &m_tUpdatePartnerInfo.HPKitItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_tUpdatePartnerInfo.HPKitTriggerValue, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 9, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_INTEGER, 0, 0, &m_tUpdatePartnerInfo.ShieldKitItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_tUpdatePartnerInfo.ShieldKitTriggerValue, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_INTEGER, 0, 0, &m_tUpdatePartnerInfo.SPKitItemNum, 0, NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &m_tUpdatePartnerInfo.SPKitTriggerValue, 0, NULL);
	for(int a=0; a< SIZE_MAX_PETSOCKET_AUTOSKILL_SLOT; a++)
	{
		SQLBindParameter(m_pODBCStmt->m_hstmt, a + 13, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_INTEGER, 0, 0, &m_tUpdatePartnerInfo.AutoSkilItemNum[a], 0, NULL);
	}
	/*[Query Definition]************************************************
	-------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.atum_Update_PetInfo
	-- DESC				: 펫 정보 업데이트
	-- // 2011-09-30 by hskim, 파트너 시스템
	-------------------------------------------------------------
	CREATE PROC [dbo].[atum_Update_PetInfo]
		@i_ItemUID					BIGINT,
		@i_Experience				FLOAT,
		@i_Level					INT,
		@i_Name						VARCHAR(40),
		@i_ExpRatio					TINYINT,
		@i_EnableSocketCount		TINYINT,
		@i_HPKitItemNum				INT,
		@i_HPKitTriggerValue		FLOAT,
		@i_ShieldKitItemNum			INT,
		@i_ShieldKitTriggerValue	FLOAT,
		@i_SPKitItemNum				INT,
		@i_SPKitTriggerValue		FLOAT,
		@i_AutoSkilItemNum1			INT,
		@i_AutoSkilItemNum2			INT,
		@i_AutoSkilItemNum3			INT,
		@i_AutoSkilItemNum4			INT,
		@i_AutoSkilItemNum5			INT,
		@i_AutoSkilItemNum6			INT
	AS
	BEGIN
		UPDATE [dbo].[td_pet] 
			SET Experience = @i_Experience, [Level] = @i_Level, Name = @i_Name, ExpRatio = @i_ExpRatio, 
				EnableSocketCount = @i_EnableSocketCount,
				HPKitItemNum = @i_HPKitItemNum, HPKitTriggerValue = @i_HPKitTriggerValue, 
				ShieldKitItemNum = @i_ShieldKitItemNum, ShieldKitTriggerValue = @i_ShieldKitTriggerValue,
				SPKitItemNum = @i_SPKitItemNum, SPKitTriggerValue = @i_SPKitTriggerValue,
				AutoSkilItemNum1 = @i_AutoSkilItemNum1, AutoSkilItemNum2 = @i_AutoSkilItemNum2,
				AutoSkilItemNum3 = @i_AutoSkilItemNum3, AutoSkilItemNum4 = @i_AutoSkilItemNum4,
				AutoSkilItemNum5 = @i_AutoSkilItemNum5, AutoSkilItemNum6 = @i_AutoSkilItemNum6
			WHERE [ItemUID] = @i_ItemUID;		
	END
	GO
	**************************************************************************/
	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_110824_0008, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO )
	{
		MessageBox(STRERR_S_SCADMINTOOL_0054);
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
	
	
	SQLBindParameter(m_pODBCStmt->m_hstmt,  1, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER, 0, 0, &m_pUserAdminDlg->m_uidAccountUID, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  2, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER, 0, 0, &m_pUserAdminDlg->m_CurrentCharacter.CharacterUniqueNumber, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  3, SQL_PARAM_INPUT, SQL_C_CHAR,		SQL_VARCHAR, SIZE_MAX_ACCOUNT_NAME, 0, m_pMainDlg->m_szManagerAccountName, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  4, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,	 0, 0, &m_tPartnerInfo.ItemUID, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  5, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER, 0, 0, &m_tPartnerInfo.PartnerIndex, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  6, SQL_PARAM_INPUT, SQL_C_CHAR,		SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, m_tPartnerInfo.name, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  7, SQL_PARAM_INPUT, SQL_C_CHAR,		SQL_VARCHAR, SIZE_MAX_PET_NAME, 0, m_tUpdatePartnerInfo.name, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  8, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER, 0, 0, &m_tPartnerInfo.level, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt,  9, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_INTEGER, 0, 0, &m_tUpdatePartnerInfo.level, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 10, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_FLOAT,	 0, 0, &m_tPartnerInfo.Experience, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 11, SQL_PARAM_INPUT, SQL_C_ULONG,	SQL_FLOAT,	 0, 0, &m_tUpdatePartnerInfo.Experience, 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 12, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tPartnerInfo.SocketItem[0], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 13, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tUpdatePartnerInfo.SocketItem[0], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 14, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tPartnerInfo.SocketItem[1], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 15, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tUpdatePartnerInfo.SocketItem[1], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 16, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tPartnerInfo.SocketItem[2], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 17, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tUpdatePartnerInfo.SocketItem[2], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 18, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tPartnerInfo.SocketItem[3], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 19, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tUpdatePartnerInfo.SocketItem[3], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 20, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tPartnerInfo.SocketItem[4], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 21, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tUpdatePartnerInfo.SocketItem[4], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 22, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tPartnerInfo.SocketItem[5], 0,		NULL);
	SQLBindParameter(m_pODBCStmt->m_hstmt, 23, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT,  0, 0, &m_tUpdatePartnerInfo.SocketItem[5], 0,		NULL);
	/*****************************************************************************
	--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: atum_log_Insert_pet_info_updata
		-- DESC				: // 2012-03-15 by jhseol, 파트너 - 파트너 수정정보 로그 저장 프로시저
	--------------------------------------------------------------------------------
		CREATE PROCEDURE [dbo].[atum_log_Insert_pet_info_updata]
		@i_AccountUID			INT,
		@i_CharacterUID			INT,
		@i_AdministratorName	VARCHAR(20),
		@i_PetItemUID			BIGINT,
		@i_PetIndex				INT,
		@i_PetName				VARCHAR(40),
		@i_PetUpdataName		VARCHAR(40),
		@i_PetLevel				INT,
		@i_PetUpdataLevel		INT,
		@i_PetExperience		FLOAT,
		@i_PetUpdataExperience	FLOAT,
		@i_PetSocketItem1		BIGINT,
		@i_PetUpdataSocketItem1	BIGINT,
		@i_PetSocketItem2		BIGINT,
		@i_PetUpdataSocketItem2	BIGINT,
		@i_PetSocketItem3		BIGINT,
		@i_PetUpdataSocketItem3	BIGINT,
		@i_PetSocketItem4		BIGINT,
		@i_PetUpdataSocketItem4	BIGINT,
		@i_PetSocketItem5		BIGINT,
		@i_PetUpdataSocketItem5	BIGINT,
		@i_PetSocketItem6		BIGINT,
		@i_PetUpdataSocketItem6	BIGINT
		-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
		AS
		BEGIN
		INSERT INTO [atum2_db_1].[dbo].[atum_log_pet_info_updata] ([Time], [AccountUID], [CharacterUID], [AdministratorName],
		[PetItemUID], [PetIndex],
		[PetName], [PetUpdataName],
		[PetLevel], [PetUpdataLevel],
		[PetExperience], [PetUpdataExperience],
		[PetSocketItem1], [PetUpdataSocketItem1], 
		[PetSocketItem2], [PetUpdataSocketItem2], 
		[PetSocketItem3], [PetUpdataSocketItem3], 
		[PetSocketItem4], [PetUpdataSocketItem4], 
		[PetSocketItem5], [PetUpdataSocketItem5], 
		[PetSocketItem6], [PetUpdataSocketItem6])
		VALUES(getdate(), @i_AccountUID, @i_CharacterUID, @i_AdministratorName, 
		@i_PetItemUID, @i_PetIndex, 
		@i_PetName, @i_PetUpdataName, 
		@i_PetLevel, @i_PetUpdataLevel, 
		@i_PetExperience, @i_PetUpdataExperience, 
		@i_PetSocketItem1, @i_PetUpdataSocketItem1, 
		@i_PetSocketItem2, @i_PetUpdataSocketItem2, 
		@i_PetSocketItem3, @i_PetUpdataSocketItem3, 
		@i_PetSocketItem4, @i_PetUpdataSocketItem4, 
		@i_PetSocketItem5, @i_PetUpdataSocketItem5, 
		@i_PetSocketItem6, @i_PetUpdataSocketItem6)
		END
		GO
	*****************************************************************************/
	ret = SQLExecDirect(m_pODBCStmt->m_hstmt, PROCEDURE_120315_0001, SQL_NTS);
	if ( ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
	{
		MessageBox(STRERR_S_SCADMINTOOL_0049);
		SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
		return;
	}
	SQLFreeStmt(m_pODBCStmt->m_hstmt, SQL_CLOSE);
	
	MessageBox("Partner Info UpDate Successfully!!");

	CDialog::OnOK();
}


void CPartnerDlg::OnOK()
{
	m_PartnerExpSlider.SetPos(m_nPartnerExpPercentage);
	UpdateData(FALSE);
}

void CPartnerDlg::OnSockDel()
{
	m_CComboPartnerProssessSocketList.SetCurSel(0);
	UpdateData(FALSE);
}

void CPartnerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(PARTNER_MAX_LEVEL <= m_nPartnerLvCurrent )
	{
		m_PartnerExpSlider.SetPos(0);
		m_nPartnerExpMax		= 0;
	}
	int m_nSliderIndex = m_PartnerExpSlider.GetPos();
	m_nPartnerExpPercentage = m_nSliderIndex;
	m_nPartnerExpCurrent = m_nPartnerExpPercentage*(m_nPartnerExpMax/100);
	if(m_nPartnerExpCurrent < 0) m_nPartnerExpCurrent = 0;
	
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPartnerDlg::OnUpdateEditPartnerLvCurrent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	char tempString[3];
	GetDlgItemText( IDC_EDIT_PARTNER_LV_CURRENT, tempString, 3);
	m_nPartnerLvCurrent = atoi(tempString);
	if(PARTNER_MAX_LEVEL <= m_nPartnerLvCurrent)
	{
		m_nPartnerLvCurrent = 25;
		m_nPartnerExpPercentage = 0;
		m_nPartnerExpCurrent	= 0;
		m_nPartnerExpMax		= 0;
	}
	else if(m_nPartnerLvCurrent < 1)
	{
		m_nPartnerLvCurrent = 1;
		m_nPartnerExpMax		= m_nPartnerExpLevel[m_nPartnerLvCurrent];
	}
	else m_nPartnerExpMax	= m_nPartnerExpLevel[m_nPartnerLvCurrent] - m_nPartnerExpLevel[m_nPartnerLvCurrent-1];
	m_nPartnerExpCurrent	= m_nPartnerExpPercentage*(m_nPartnerExpMax/100);
	if(m_nPartnerExpCurrent < 0) m_nPartnerExpCurrent = 0;
}

void CPartnerDlg::OnUpdateEditPartnerExpCurrent() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	char tempString[9];
	GetDlgItemText( IDC_EDIT_PARTNER_EXP_CURRENT, tempString, sizeof(tempString));
	m_nPartnerExpCurrent = atoi(tempString);
	if(PARTNER_MAX_LEVEL <= m_nPartnerLvCurrent)
	{
		m_nPartnerExpPercentage = 0;
		m_nPartnerExpCurrent	= 0;
		m_nPartnerExpMax		= 0;
	}
	else
	{
		
		if(m_nPartnerExpCurrent >= m_nPartnerExpMax) m_nPartnerExpCurrent = m_nPartnerExpMax - 1;
		else if(m_nPartnerExpCurrent <= 0) m_nPartnerExpCurrent = 0;
		m_nPartnerExpPercentage = m_nPartnerExpCurrent/(m_nPartnerExpMax/100);
	}
}

void CPartnerDlg::OnUpdateEditPartnerExpPercentage() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	char tempString[4];
	GetDlgItemText( IDC_EDIT_PARTNER_EXP_PERCENTAGE, tempString, sizeof(tempString));
	m_nPartnerExpPercentage = atoi(tempString);
	if(PARTNER_MAX_LEVEL <= m_nPartnerLvCurrent)
	{
		m_nPartnerExpPercentage = 0;
		m_nPartnerExpCurrent	= 0;
		m_nPartnerExpMax		= 0;
	}
	if(m_nPartnerExpPercentage > 99) m_nPartnerExpPercentage = 99;
	else if(m_nPartnerExpPercentage <= 0) m_nPartnerExpPercentage = 0;
	m_nPartnerExpCurrent = m_nPartnerExpPercentage*(m_nPartnerExpMax/100);
	if(m_nPartnerExpCurrent < 0) m_nPartnerExpCurrent = 0;
}

void CPartnerDlg::OnUpdateEditPartnerName() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	char tempString[SIZE_MAX_PET_NAME];
	GetDlgItemText( IDC_EDIT_PARTNER_NAME, tempString, SIZE_MAX_PET_NAME);
	memcpy(m_tUpdatePartnerInfo.name, tempString, SIZE_MAX_PET_NAME);
	m_szPartnerName			= (CString)tempString;
}


void CPartnerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(PARTNER_MAX_LEVEL <= m_nPartnerLvCurrent ) m_PartnerExpSlider.SetPos(0);
	UpdateData(FALSE);
	CDialog::OnLButtonDown(nFlags, point);
}
