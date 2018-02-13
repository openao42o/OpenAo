// characterinfodlg.cpp : implementation file
//

#include "stdafx.h"
#include "characterinfodlg.h"
#include "atumsj.h"
#include "ODBCStatement.h"
#include "AtumError.h"				// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfoDlg dialog


CCharacterInfoDlg::CCharacterInfoDlg(BOOL i_bEnableEdit, CODBCStatement *i_pODBCState, CWnd* pParent /*=NULL*/)
	: CDialog(CCharacterInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharacterInfoDlg)
	m_CharacterName = _T("");
	m_DP = 0;
	m_CurDP = 0;
	m_HP = 0;
	m_CurHP = 0;
	m_SP = 0;
	m_CurSP = 0;
	m_EP = 0;
	m_CurEP = 0;
	m_ExpPercentage = 0;
	m_Level = 0;
	m_MapIndex = 0;
	m_nChannelIndex = 0;
	m_PartAttack = 0;
	m_PartDefense = 0;
	m_PartFuel = 0;
	m_PartSoul = 0;
	m_PartShield = 0;
	m_PartDodge = 0;
	m_BonusStat = 0;
	m_BonusStatPoint = 0;
	m_PosX = 0;
	m_PosZ = 0;
	m_Propensity = 0;
	m_RacingPoint = 0;
	m_ctl_doExp = 0.0;
	m_ctl_bInflLeader = FALSE;
	m_ctl_bInflSubLeader = FALSE;
	m_ctl_bInflSub2Leader = FALSE;
	m_PCBangTotalPlayTime = 0;
	m_ArenaCWP = 0;
	m_ArenaDisConnect = 0;
	m_ArenaLose = 0;
	m_ArenaWin = 0;
	m_ArenaWP = 0;
	//}}AFX_DATA_INIT

	m_bEnableEdit			= i_bEnableEdit;		// 2006-04-15 by cmkwon
	m_pODBCStmt3			= i_pODBCState;			// 2006-05-03 by cmkwon
	m_CharacterUID			= 0;					// 2006-05-03 by cmkwon
	m_usUnitKind			= 0;					// 2007-01-11 by cmkwon
	m_AccountUID			= 0;					// 2007-02-13 by dhjin
	m_bReloadCharacterInfo	= FALSE;				// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
}


void CCharacterInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharacterInfoDlg)
	DDX_Text(pDX, IDC_EDIT_CHARACTER_NAME, m_CharacterName);
	DDX_Text(pDX, IDC_EDIT_DP, m_DP);
	DDX_Text(pDX, IDC_EDIT_DP_CUR, m_CurDP);
	DDX_Text(pDX, IDC_EDIT_HP, m_HP);
	DDX_Text(pDX, IDC_EDIT_HP_CUR, m_CurHP);
	DDX_Text(pDX, IDC_EDIT_SP, m_SP);
	DDX_Text(pDX, IDC_EDIT_SP_CUR, m_CurSP);
	DDX_Text(pDX, IDC_EDIT_EP, m_EP);
	DDX_Text(pDX, IDC_EDIT_EP_CUR, m_CurEP);
	DDX_Text(pDX, IDC_EDIT_EXP_PER, m_ExpPercentage);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_Level);
	DDX_Text(pDX, IDC_EDIT_MAP_INDEX, m_MapIndex);
	DDX_Text(pDX, IDC_EDIT_CHANNEL_INDEX, m_nChannelIndex);
	DDX_Text(pDX, IDC_EDIT_PART_ATTACK, m_PartAttack);
	DDX_Text(pDX, IDC_EDIT_PART_DEFENSE, m_PartDefense);
	DDX_Text(pDX, IDC_EDIT_PART_FUEL, m_PartFuel);
	DDX_Text(pDX, IDC_EDIT_PART_SOUL, m_PartSoul);
	DDX_Text(pDX, IDC_EDIT_PART_SHIELD, m_PartShield);
	DDX_Text(pDX, IDC_EDIT_PART_DODGE, m_PartDodge);
	DDX_Text(pDX, IDC_EDIT_BONUS_STAT, m_BonusStat);
	DDX_Text(pDX, IDC_EDIT_BONUS_STAT_POINT, m_BonusStatPoint);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_PosX);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_PosZ);
	DDX_Text(pDX, IDC_EDIT_PROPENSITY, m_Propensity);
	DDX_Text(pDX, IDC_EDIT_RACING_POINT, m_RacingPoint);
	DDX_Text(pDX, IDC_EDIT_EXP, m_ctl_doExp);
	DDX_Check(pDX, IDC_CHECK_SET_INFL_LEADER, m_ctl_bInflLeader);
	DDX_Check(pDX, IDC_CHECKSubLeader, m_ctl_bInflSubLeader);
	DDX_Check(pDX, IDC_CHECKInflSub2Leader, m_ctl_bInflSub2Leader);
	DDX_Text(pDX, IDC_EDIT_PCBangTotalPlayTime, m_PCBangTotalPlayTime);
	DDX_Text(pDX, IDC_EDIT_ARENA_CWP, m_ArenaCWP);
	DDX_Text(pDX, IDC_EDIT_ARENA_DISCONNECT, m_ArenaDisConnect);
	DDX_Text(pDX, IDC_EDIT_ARENA_LOSE, m_ArenaLose);
	DDX_Text(pDX, IDC_EDIT_ARENA_WIN, m_ArenaWin);
	DDX_Text(pDX, IDC_EDIT_ARENA_WP, m_ArenaWP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCharacterInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCharacterInfoDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_CHECK_SET_INFL_LEADER, OnCheckSetInflLeader)
	ON_BN_CLICKED(IDC_CHECKSubLeader, OnCHECKSubLeader)
	ON_BN_CLICKED(IDC_CHECKInflSub2Leader, OnCHECKInflSub2Leader)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_INIT_INFLUENCE, OnBtnInitInfluence)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfoDlg message handlers
void CCharacterInfoDlg::Info2Character(CHARACTER *o_pCharac)
{
	util::strncpy(o_pCharac->CharacterName, m_CharacterName, SIZE_MAX_CHARACTER_NAME);
	o_pCharac->DP							= m_DP;
	o_pCharac->CurrentDP					= m_CurDP;
	o_pCharac->HP							= m_HP;
	o_pCharac->CurrentHP					= m_CurHP;
	o_pCharac->SP							= m_SP;
	o_pCharac->CurrentSP					= m_CurSP;
	o_pCharac->EP							= m_EP;
	o_pCharac->CurrentEP					= m_CurEP;
// 2005-04-21 by cmkwon, 퍼센트지 설정에서 경험치를 직접 설정으로 변경
//	o_pCharac->Experience					= CAtumSJ::GetExperienceFromPercentage(m_ExpPercentage, m_Level);
	o_pCharac->Experience					= m_ctl_doExp;
	o_pCharac->Level						= m_Level;
	o_pCharac->MapChannelIndex.MapIndex		= m_MapIndex;
	o_pCharac->MapChannelIndex.ChannelIndex	= m_nChannelIndex;
	o_pCharac->GearStat.AttackPart			= m_PartAttack;
	o_pCharac->GearStat.DefensePart			= m_PartDefense;
	o_pCharac->GearStat.FuelPart			= m_PartFuel;
	o_pCharac->GearStat.SoulPart			= m_PartSoul;
	o_pCharac->GearStat.ShieldPart			= m_PartShield;
	o_pCharac->GearStat.DodgePart			= m_PartDodge;
	o_pCharac->BonusStat					= m_BonusStat;
	o_pCharac->BonusStatPoint				= m_BonusStatPoint;			// 2007-01-11 by cmkwon
	o_pCharac->PositionVector.x				= m_PosX;
	o_pCharac->PositionVector.z				= m_PosZ;
	o_pCharac->Race							= (m_Race&(~RACE_ACCOUNT_TYPE_MASK)) | (m_RaceAccType&RACE_ACCOUNT_TYPE_MASK);
	o_pCharac->InfluenceType				= m_byInfluenceType;		// 2005-12-13 by cmkwon
	o_pCharac->Propensity					= m_Propensity;
	o_pCharac->RacingPoint					= m_RacingPoint;
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-08 by dhjin, 아레나 정보와 PCBangTotal시간 추가됨
	o_pCharac->PCBangTotalPlayTime			= m_PCBangTotalPlayTime;
	o_pCharac->WarPoint						= m_ArenaWP;
	o_pCharac->CumulativeWarPoint			= m_ArenaCWP;
	o_pCharac->ArenaWin						= m_ArenaWin;
	o_pCharac->ArenaLose					= m_ArenaLose;
	o_pCharac->ArenaDisConnect				= m_ArenaDisConnect;

}

void CCharacterInfoDlg::Character2Info(CHARACTER *i_pCharac, BOOL i_bInflLeader, BOOL i_bInflSub1Leader, BOOL i_bInflSub2Leader)
{
	m_CharacterName			= i_pCharac->CharacterName;
	m_DP					= i_pCharac->DP;
	m_CurDP					= i_pCharac->CurrentDP;
	m_HP					= i_pCharac->HP;
	m_CurHP					= i_pCharac->CurrentHP;
	m_SP					= i_pCharac->SP;
	m_CurSP					= i_pCharac->CurrentSP;
	m_EP					= i_pCharac->EP;
	m_CurEP					= i_pCharac->CurrentEP;
	m_ctl_doExp				= i_pCharac->Experience;
	m_ExpPercentage			= CAtumSJ::GetPercentageOfExperience(i_pCharac->Experience, i_pCharac->Level);
	m_Level					= i_pCharac->Level;
	m_MapIndex				= i_pCharac->MapChannelIndex.MapIndex;
	m_nChannelIndex			= i_pCharac->MapChannelIndex.ChannelIndex;
	m_PartAttack			= i_pCharac->GearStat.AttackPart;
	m_PartDefense			= i_pCharac->GearStat.DefensePart;
	m_PartFuel				= i_pCharac->GearStat.FuelPart;
	m_PartSoul				= i_pCharac->GearStat.SoulPart;
	m_PartShield			= i_pCharac->GearStat.ShieldPart;
	m_PartDodge				= i_pCharac->GearStat.DodgePart;
	m_BonusStat				= i_pCharac->BonusStat;
	m_BonusStatPoint		= i_pCharac->BonusStatPoint;			// 2007-01-11 by cmkwon
	m_PosX					= i_pCharac->PositionVector.x;
	m_PosZ					= i_pCharac->PositionVector.z;
	m_Race					= i_pCharac->Race&(~RACE_ACCOUNT_TYPE_MASK);
	m_RaceAccType			= i_pCharac->Race&RACE_ACCOUNT_TYPE_MASK;
	m_Propensity			= i_pCharac->Propensity;
	m_RacingPoint			= i_pCharac->RacingPoint;
	m_byInfluenceType		= i_pCharac->InfluenceType;						// 2005-12-13 by cmkwon
	
	///////////////////////////////////////////////////////////////////////////////
	// 2006-05-03 by cmkwon
	m_ctl_bInflLeader		= i_bInflLeader;
	m_ctl_bInflSubLeader	= i_bInflSub1Leader;
	m_ctl_bInflSub2Leader	= i_bInflSub2Leader;
	m_CharacterUID			= i_pCharac->CharacterUniqueNumber;
	m_AccountUID			= i_pCharac->AccountUniqueNumber;
	m_usUnitKind			= i_pCharac->UnitKind;					// 2007-01-11 by cmkwon
	//////////////////////////////////////////////////////////////////////////
	// 2007-06-08 by dhjin, 아레나 정보와 PCBangTotal시간 추가됨
	m_PCBangTotalPlayTime	= i_pCharac->PCBangTotalPlayTime;
	m_ArenaWP				= i_pCharac->WarPoint;
	m_ArenaCWP				= i_pCharac->CumulativeWarPoint;
	m_ArenaWin				= i_pCharac->ArenaWin;
	m_ArenaLose				= i_pCharac->ArenaLose;
	m_ArenaDisConnect		= i_pCharac->ArenaDisConnect;

	m_csAccountName			= i_pCharac->AccountName;			// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CCharacterInfoDlg::StoreInflLeader(BYTE i_byInflTy, UID32_t i_LeaderCharUID)
/// \brief		
/// \author		cmkwon
/// \date		2006-05-03 ~ 2006-05-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCharacterInfoDlg::StoreInflLeader(BYTE i_byInflTy, UID32_t i_LeaderCharUID)
{
	// 2013-01-16 by jhseol, 지도자 권한 박탈 시 지도자 관련정보 초기화
	// 가장 우선적으로 확인해야 한다.
	if ( i_LeaderCharUID == 0 )
	{
		RETCODE ret;
		
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byInflTy, 0, NULL);
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_CharacterUID, 0, NULL);
		/********************************************************************************
		--------------------------------------------------------------------------------
		-- PROCEDURE NAME	: dbo.atum_InitCityLeader
		-- DESC				: 2013-01-16 by jhseol, 지도자 권한 박탈시 정보 수정 프로시저
		--	1. 캐릭터 권한을 일반권한으로 변경
		--	2. td_CityLeaderInfo 정보 삭제
		--	3. 지도자 아이템은 삭제(세력별)
		--	4. 회의룸 입장 리스트 초기화. (BCU:1001 / ANI:1002)
		--------------------------------------------------------------------------------
		CREATE PROCEDURE [dbo].[atum_InitCityLeader]
		@i_Influence	TINYINT,
		@i_CUID			INT
		--WITH EXECUTE AS 'proexe'
		AS
		BEGIN
		IF (@i_Influence = 2)
		BEGIN
		-- BCU 세력 지도자가 맞는지 확인 후 맞으면 초기화 진행
		IF EXISTS ( SELECT CharacterUID	FROM td_CityLeaderInfo	WHERE MapIndex = 2001 AND CharacterUID = @i_CUID )
		BEGIN
		--	1. 캐릭터 권한을 일반권한으로 변경
		UPDATE dbo.td_Character
		SET Race = 2
		WHERE UniqueNumber = @i_CUID
		
		  --	2. td_CityLeaderInfo 정보 초기화(BCU)
		  UPDATE dbo.td_CityLeaderInfo
		  SET	Influence = 0, CharacterUID = 0, ExpediencyFundCumulative = 0, ExpediencyFund = 0, ExpediencyFundRate = 0, Notice = NULL
		  WHERE MapIndex = 2001
		  
			--	3. 지도자 아이템은 삭제(BCU)
			DELETE FROM dbo.td_Store WHERE Itemnum = 7001320 AND Possess = @i_CUID	--	지도자의 광휘(바이제니유)
			DELETE FROM dbo.td_Store WHERE Itemnum = 7001090 AND Possess = @i_CUID	--	세력의 가호
			DELETE FROM dbo.td_Store WHERE Itemnum = 7001110 AND Possess = @i_CUID	--	성장의 시간
			
			  --	4. 회의룸 입장 리스트 초기화. (BCU:1001)
			  DELETE FROM dbo.td_user_list_warpable_to_map WHERE MapIndex = 1001
			  END
			  END
			  ELSE IF (@i_Influence = 4)
			  BEGIN
			  -- ANI 세력 지도자가 맞는지 확인 후 맞으면 초기화 진행
			  IF EXISTS ( SELECT CharacterUID	FROM td_CityLeaderInfo	WHERE MapIndex = 2002 AND CharacterUID = @i_CUID )
			  BEGIN
			  --	1. 캐릭터 권한을 일반권한으로 변경
			  UPDATE dbo.td_Character
			  SET Race = 2
			  WHERE UniqueNumber = @i_CUID
			  
				--	2. td_CityLeaderInfo 정보 초기화(ANI)
				UPDATE dbo.td_CityLeaderInfo
				SET	Influence = 0, CharacterUID = 0, ExpediencyFundCumulative = 0, ExpediencyFund = 0, ExpediencyFundRate = 0, Notice = NULL
				WHERE MapIndex = 2002
				
				  --	3. 지도자 아이템은 삭제(ANI)
				  DELETE FROM dbo.td_Store WHERE Itemnum = 7001330 AND Possess = @i_CUID	--	지도자의 광휘(알링턴)
				  DELETE FROM dbo.td_Store WHERE Itemnum = 7001090 AND Possess = @i_CUID	--	세력의 가호
				  DELETE FROM dbo.td_Store WHERE Itemnum = 7001110 AND Possess = @i_CUID	--	성장의 시간
				  
					--	4. 회의룸 입장 리스트 초기화. (ANI:1002)
					DELETE FROM dbo.td_user_list_warpable_to_map WHERE MapIndex = 1002
					END
					END
					END
					GO
		********************************************************************************/
		ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, PROCEDURE_130116_0001, SQL_NTS);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			MessageBox("DB Execute Error !!(CCharacterInfoDlg::atum_InitCityLeader)");
			m_pODBCStmt3->FreeStatement();
			return FALSE;
		}
		
		m_pODBCStmt3->FreeStatement();	
	}
	// end 2013-01-16 by jhseol, 지도자 권한 박탈 시 지도자 관련정보 초기화

	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("UPDATE td_InfluenceWarData \
// 		SET InflLeaderCharacterUID = %d \
// 		WHERE InfluenceType = %d", i_LeaderCharUID, i_byInflTy);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0004, i_LeaderCharUID, i_byInflTy);
//	BOOL bRet = m_pODBCStmt3->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt3->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byInflTy, 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_LeaderCharUID, 0,		&arrCB2[2]);
	BOOL bRet = m_pODBCStmt3->ExecuteQuery((char*)(PROCEDURE_080827_0004));
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CCharacterInfoDlg::StoreInflLeader_)");
		m_pODBCStmt3->FreeStatement();
		return FALSE;
	}
	m_pODBCStmt3->FreeStatement();				// free statement	

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- TABLE NAME: dbo.atum_InsertLeaderItem
	-- DESC      : 2007-02-13 by dhjin, 지도자 전용 아이템 추가하는 프로시저
	--				7001320	지도자의 광휘(바이제니유)
	--				7001330	지도자의 광휘(알링턴)
	--				7001090	세력의 가호
	--				7001110	성장의 시간
	--------------------------------------------------------------------------------
	**************************************************************************/
	if (i_LeaderCharUID != 0)
	{// 2007-02-13 by dhjin, 지도자 체크란에 체크가 되어야만 업데이트 진행
		RETCODE	ret;
		
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byInflTy, 0, NULL);
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_AccountUID, 0, NULL);
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_LeaderCharUID, 0, NULL);

		ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, PROCEDURE_080822_0001, SQL_NTS);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			MessageBox("DB Execute Error !!(CCharacterInfoDlg::InsertLeaderItem_)");
			m_pODBCStmt3->FreeStatement();
			return FALSE;
		}

		m_pODBCStmt3->FreeStatement();	
	}

	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME: dbo.atum_UpdateCityLeaderByLeaderSet
	-- DESC			 : 지도자 설정 시 td_CityLeaderInfo에 정보 설정
	-- 2007-09-14 by dhjin,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE dbo.atum_UpdateCityLeaderByLeaderSet
		@i_Infltype				TINYINT,
		@i_Possess				INT
	AS
		DECLARE @CountInfl INT
		SET @CountInfl = (SELECT count(*) FROM dbo.td_CityLeaderInfo WHERE Influence = @i_Infltype)

		IF @i_Infltype = 2 -- 바이제니유
		BEGIN
			UPDATE dbo.td_CityLeaderInfo
			SET	Influence = 2, CharacterUID = @i_Possess, ExpediencyFundCumulative = 0, ExpediencyFund = 0, ExpediencyFundRate = 10 + (@CountInfl * 5), Notice = NULL
			WHERE MapIndex = 2001
		END

		ELSE IF @i_Infltype = 4 -- 알링턴
		BEGIN
			UPDATE dbo.td_CityLeaderInfo
			SET	Influence = 4, CharacterUID = @i_Possess, ExpediencyFundCumulative = 0, ExpediencyFund = 0, ExpediencyFundRate = 10 + (@CountInfl * 5), Notice = NULL
			WHERE MapIndex = 2002
		END
	GO
	**************************************************************************/	
	if (i_LeaderCharUID != 0)
	{// 2007-02-13 by dhjin, 지도자 체크란에 체크가 되어야만 업데이트 진행
		RETCODE	ret;
		
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byInflTy, 0, NULL);
		SQLBindParameter(m_pODBCStmt3->m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_LeaderCharUID, 0, NULL);

		ret = SQLExecDirect(m_pODBCStmt3->m_hstmt, PROCEDURE_080822_0002, SQL_NTS);
		if(ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO && ret != SQL_NO_DATA)
		{
			MessageBox("DB Execute Error !!(CCharacterInfoDlg::atum_UpdateCityLeaderByLeaderSet)");
			m_pODBCStmt3->FreeStatement();
			return FALSE;
		}

		m_pODBCStmt3->FreeStatement();	
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CCharacterInfoDlg::StoreInflSubLeader(BYTE i_byInflTy, UID32_t i_SubLeaderCharUID)
/// \brief		
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCharacterInfoDlg::StoreInflSubLeader(BYTE i_byInflTy, UID32_t i_SubLeaderCharUID)
{
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("UPDATE td_InfluenceWarData \
// 		SET InflSub1LeaderCharacterUID = %d \
// 		WHERE InfluenceType = %d", i_SubLeaderCharUID, i_byInflTy);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0005, i_SubLeaderCharUID, i_byInflTy);
//	BOOL bRet = m_pODBCStmt3->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt3->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byInflTy, 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_SubLeaderCharUID, 0,		&arrCB2[2]);
	BOOL bRet = m_pODBCStmt3->ExecuteQuery((char*)(PROCEDURE_080827_0005));
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CCharacterInfoDlg::StoreInflSubLeader_)");
		m_pODBCStmt3->FreeStatement();
		return FALSE;
	}
	m_pODBCStmt3->FreeStatement();				// free statement	
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CCharacterInfoDlg::StoreInflSub2Leader(BYTE i_byInflTy, UID32_t i_Sub2LeaderCharUID)
/// \brief		
/// \author		dhjin
/// \date		2006-12-08 ~ 2006-12-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCharacterInfoDlg::StoreInflSub2Leader(BYTE i_byInflTy, UID32_t i_Sub2LeaderCharUID)
{	
	CString szQuery;
// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	szQuery.Format("UPDATE td_InfluenceWarData \
// 		SET InflSub2LeaderCharacterUID = %d \
// 		WHERE InfluenceType = %d", i_Sub2LeaderCharUID, i_byInflTy);

// 2008-08-27 by cmkwon, 날쿼리를 Procedure로 수정 - 
//	szQuery.Format(QUERY_080702_0006, i_Sub2LeaderCharUID, i_byInflTy);
//	BOOL bRet = m_pODBCStmt3->ExecuteQuery(szQuery);
	SQLHSTMT hstmt = m_pODBCStmt3->GetSTMTHandle();
	SQLINTEGER arrCB2[3] = {SQL_NTS,SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &i_byInflTy, 0,			&arrCB2[1]);
	SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_Sub2LeaderCharUID, 0,	&arrCB2[2]);
	BOOL bRet = m_pODBCStmt3->ExecuteQuery((char*)(PROCEDURE_080827_0006));
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CCharacterInfoDlg::StoreInflSubLeader_)");
		m_pODBCStmt3->FreeStatement();
		return FALSE;
	}
	m_pODBCStmt3->FreeStatement();				// free statement	
	return TRUE;
}

#define RACE_COMBO_BATTALUS		0 // (바탈러스)
#define RACE_COMBO_DECA			1 // (데카)
#define RACE_COMBO_PHILON		2 // (필론)
#define RACE_COMBO_SHARRINE		3 // (샤린)

#define RACE_ACC_TYPE_COMBO_NO_TYPE			0 // (일반)
#define RACE_ACC_TYPE_COMBO_OPERATION		1 // (관리자)
#define RACE_ACC_TYPE_COMBO_GAMEMASTER		2 // (게임마스터)
#define RACE_ACC_TYPE_COMBO_MONITOR			3 // (모니터)
#define RACE_ACC_TYPE_COMBO_GUEST			4 // (게스트)
#define RACE_ACC_TYPE_COMBO_DEMO			5 // (데모)

///////////////////////////////////////////////////////////////////////////////
// 2005-12-13 by cmkwon
#define COMBOIndex_INFLUENCETypeNormal		0 // (바이제니유-일반)
#define COMBOIndex_INFLUENCETypeVCN			1 // (바이제니유-정규군)
#define COMBOIndex_INFLUENCETypeANI			2 // (바이제니유-반란군)
#define COMBOIndex_INFLUENCETypeRRP			3 // (바탈러스-연방군)
#define COMBOIndex_INFLUENCETypeUnknown		4 // (알수없음)

BOOL CCharacterInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox *pComboRace = (CComboBox*)GetDlgItem(IDC_COMBO_RACE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-02 by cmkwon, AdminTool에서 계정 권한 설정을 번역 한것을 사용함.
	pComboRace->AddString(STRCMD_CS_COMMON_RACE_BATTALUS);
	pComboRace->AddString(STRCMD_CS_COMMON_RACE_DECA);
	pComboRace->AddString(STRCMD_CS_COMMON_RACE_PHILON);
	pComboRace->AddString(STRCMD_CS_COMMON_RACE_SHARRINE);

	if (m_Race == RACE_BATTALUS) pComboRace->SetCurSel(RACE_COMBO_BATTALUS);
	else if (m_Race == RACE_DECA) pComboRace->SetCurSel(RACE_COMBO_DECA);
	else if (m_Race == RACE_PHILON) pComboRace->SetCurSel(RACE_COMBO_PHILON);
	else if (m_Race == RACE_SHARRINE) pComboRace->SetCurSel(RACE_COMBO_SHARRINE);

	CComboBox *pComboRaceAccType = (CComboBox*)GetDlgItem(IDC_COMBO_RACE_ACC_TYPE);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-09-02 by cmkwon, AdminTool에서 계정 권한 설정을 번역 한것을 사용함.
	pComboRaceAccType->AddString(STRCMD_CS_COMMON_RACE_NORMAL);
	pComboRaceAccType->AddString(STRCMD_CS_COMMON_RACE_OPERATION);
	pComboRaceAccType->AddString(STRCMD_CS_COMMON_RACE_GAMEMASTER);
	pComboRaceAccType->AddString(STRCMD_CS_COMMON_RACE_MONITOR);
	pComboRaceAccType->AddString(STRCMD_CS_COMMON_RACE_GUEST);
	pComboRaceAccType->AddString(STRCMD_CS_COMMON_RACE_DEMO);

	if (m_RaceAccType == 0) pComboRaceAccType->SetCurSel(RACE_ACC_TYPE_COMBO_NO_TYPE);
	else if (m_RaceAccType == RACE_OPERATION) pComboRaceAccType->SetCurSel(RACE_ACC_TYPE_COMBO_OPERATION);
	else if (m_RaceAccType == RACE_GAMEMASTER) pComboRaceAccType->SetCurSel(RACE_ACC_TYPE_COMBO_GAMEMASTER);
	else if (m_RaceAccType == RACE_MONITOR) pComboRaceAccType->SetCurSel(RACE_ACC_TYPE_COMBO_MONITOR);
	else if (m_RaceAccType == RACE_GUEST) pComboRaceAccType->SetCurSel(RACE_ACC_TYPE_COMBO_GUEST);
	else if (m_RaceAccType == RACE_DEMO) pComboRaceAccType->SetCurSel(RACE_ACC_TYPE_COMBO_DEMO);

	CComboBox *pComboInfluenceType = (CComboBox*)GetDlgItem(IDC_COMBO_INFLUENCETYPE);
	pComboInfluenceType->AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_NORMAL));
	pComboInfluenceType->AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_VCN));
	pComboInfluenceType->AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_ANI));
	pComboInfluenceType->AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_RRP));
	pComboInfluenceType->AddString(CAtumSJ::GetInfluenceTypeString(INFLUENCE_TYPE_UNKNOWN));
	if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_NORMAL)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeNormal);}
	else if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_VCN)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeVCN);}
	else if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_ANI)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeANI);}
	else if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_RRP)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeRRP);}
	else {																pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeUnknown);}

	if(INFLUENCE_TYPE_VCN != m_byInfluenceType
		&& INFLUENCE_TYPE_ANI != m_byInfluenceType)
	{// 2006-05-03 by cmkwon, 일반군은 세력리더로 설정 불가
		GetDlgItem(IDC_CHECK_SET_INFL_LEADER)->EnableWindow(FALSE);
	}
	if(m_ctl_bInflLeader
		|| m_ctl_bInflSubLeader
		|| m_ctl_bInflSub2Leader)	// 2006-12-08 by dhjin, 부지도자
	{// 2006-05-03 by cmkwon, 세력리더는 세력 변경 불가
		pComboInfluenceType->EnableWindow(FALSE);
	}


	if(FALSE == m_bEnableEdit)
	{
		GetDlgItem(IDC_EDIT_CHARACTER_NAME)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_MAP_INDEX)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_CHANNEL_INDEX)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_POS_X)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_POS_Z)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_COMBO_RACE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_COMBO_RACE_ACC_TYPE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_COMBO_INFLUENCETYPE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_LEVEL)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_EXP)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_HP_CUR)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_HP)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_DP_CUR)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_DP)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_SP_CUR)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_SP)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_EP_CUR)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_EP)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PART_ATTACK)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PART_DEFENSE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PART_SOUL)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PART_SHIELD)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PART_DODGE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PART_FUEL)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_BONUS_STAT)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_BONUS_STAT_POINT)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_PROPENSITY)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_RACING_POINT)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_CHECK_SET_INFL_LEADER)->EnableWindow(FALSE);				// 2006-05-04 by cmkwon
		//////////////////////////////////////////////////////////////////////////
		// 2007-06-08 by dhjin, 아레나 정보와 PCBangTotal시간 추가됨		
		GetDlgItem(IDC_EDIT_PCBangTotalPlayTime)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_ARENA_CWP)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_ARENA_DISCONNECT)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_ARENA_LOSE)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_ARENA_WIN)->EnableWindow(m_bEnableEdit);
		GetDlgItem(IDC_EDIT_ARENA_WP)->EnableWindow(m_bEnableEdit);

		GetDlgItem(IDC_CHECKSubLeader)->EnableWindow(m_bEnableEdit);		// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 캐릭터 정보창
		GetDlgItem(IDC_CHECKInflSub2Leader)->EnableWindow(m_bEnableEdit);	// 2007-10-02 by cmkwon, SCAdminTool 수정 권한 처리 - 캐릭터 정보창

		GetDlgItem(IDC_BTN_INIT_INFLUENCE)->EnableWindow(m_bEnableEdit);	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	}

	// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 세력변경은 불가능하고 이제 해당 계정의 세력 초기화만 가능하다
	GetDlgItem(IDC_COMBO_INFLUENCETYPE)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCharacterInfoDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	if(FALSE == m_bEnableEdit)
	{// 2006-04-15 by cmkwon, 수정 권한 체크
		CDialog::OnCancel();
		return;
	}
	UpdateData();

	CComboBox *pComboRace = (CComboBox*)GetDlgItem(IDC_COMBO_RACE);
	CComboBox *pComboRaceAccType = (CComboBox*)GetDlgItem(IDC_COMBO_RACE_ACC_TYPE);

	if (pComboRace->GetCurSel() == RACE_COMBO_BATTALUS) m_Race = RACE_BATTALUS;
	else if (pComboRace->GetCurSel() == RACE_COMBO_DECA) m_Race = RACE_DECA;
	else if (pComboRace->GetCurSel() == RACE_COMBO_PHILON) m_Race = RACE_PHILON;
	else if (pComboRace->GetCurSel() == RACE_COMBO_SHARRINE) m_Race = RACE_SHARRINE;
	if(RACE_DECA != m_Race)
	{// 2008-09-02 by cmkwon, 체크 추가
		AfxMessageBox("Invalid Race !!");
		return;
	}

	if (pComboRaceAccType->GetCurSel() == RACE_ACC_TYPE_COMBO_NO_TYPE) m_RaceAccType = 0;
	else if (pComboRaceAccType->GetCurSel() == RACE_ACC_TYPE_COMBO_OPERATION) m_RaceAccType = RACE_OPERATION;
	else if (pComboRaceAccType->GetCurSel() == RACE_ACC_TYPE_COMBO_GAMEMASTER) m_RaceAccType = RACE_GAMEMASTER;
	else if (pComboRaceAccType->GetCurSel() == RACE_ACC_TYPE_COMBO_MONITOR) m_RaceAccType = RACE_MONITOR;
	else if (pComboRaceAccType->GetCurSel() == RACE_ACC_TYPE_COMBO_GUEST) m_RaceAccType = RACE_GUEST;
	else if (pComboRaceAccType->GetCurSel() == RACE_ACC_TYPE_COMBO_DEMO) m_RaceAccType = RACE_DEMO;

	///////////////////////////////////////////////////////////////////////////////
	// 2005-12-13 by cmkwon, 세력
	CComboBox *pComboInfluenceTy = (CComboBox*)GetDlgItem(IDC_COMBO_INFLUENCETYPE);
	if(COMBOIndex_INFLUENCETypeNormal == pComboInfluenceTy->GetCurSel()){		m_byInfluenceType = INFLUENCE_TYPE_NORMAL;}
	else if(COMBOIndex_INFLUENCETypeVCN == pComboInfluenceTy->GetCurSel()){		m_byInfluenceType = INFLUENCE_TYPE_VCN;}
	else if(COMBOIndex_INFLUENCETypeANI == pComboInfluenceTy->GetCurSel()){		m_byInfluenceType = INFLUENCE_TYPE_ANI;}
	else if(COMBOIndex_INFLUENCETypeRRP == pComboInfluenceTy->GetCurSel()){		m_byInfluenceType = INFLUENCE_TYPE_RRP;}
	else{																		m_byInfluenceType = INFLUENCE_TYPE_NORMAL;}

	if(FALSE == IS_VALID_INFLUENCE_TYPE(m_byInfluenceType))
	{// 2008-09-02 by cmkwon, 체크 추가
		AfxMessageBox("Invalid Influence !!");
		return;
	}

	Experience_t exp1 = CAtumSJ::GetInitialExperienceOfLevel(m_Level);
	Experience_t exp2 = CAtumSJ::GetInitialExperienceOfLevel(m_Level+1) - 1;

	// 2010-03-11 by cmkwon, 지원레벨 110 상항 처리 빠진것 수정 - 아래와 같이 최대지원 레벨처리 수정
	if(CHARACTER_MAX_LEVEL <= m_Level)
	{
		m_Level		= CHARACTER_MAX_LEVEL;
		m_ctl_doExp	= exp1;
		UpdateData(FALSE);
	}
	else if(m_ctl_doExp < exp1
		|| m_ctl_doExp >= exp2)
	{
		char szTemp[512];
		sprintf(szTemp, STRERR_S_SCADMINTOOL_0011
			, m_Level, exp1, exp2);
		AfxMessageBox(szTemp);
		return;
	}

	if(m_CharacterName.GetLength() >= SIZE_MAX_CHARACTER_NAME)
	{
		char szTemp[512];
		sprintf(szTemp, "Invalid CharacterName length !!(Max Length: %dBytes)", SIZE_MAX_CHARACTER_NAME);
		AfxMessageBox(szTemp);
		return;
	}

	CDialog::OnOK();
}

void CCharacterInfoDlg::OnCheckSetInflLeader() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	if(INFLUENCE_TYPE_VCN != m_byInfluenceType
		&& INFLUENCE_TYPE_ANI != m_byInfluenceType)
	{
		m_ctl_bInflLeader	= FALSE;
		UpdateData(FALSE);
		return;
	}

	CString str;
	str.Format("IMServer and FieldServer must be shutdown if you want to change Influence Leader setting change.\n\n  IMServer and FieldServer is shutdowned?");
	int nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		m_ctl_bInflLeader	= !m_ctl_bInflLeader;
		UpdateData(FALSE);
		return;
	}

	if(FALSE == m_ctl_bInflLeader)
	{// 2006-05-03 by cmkwon, 세력리더 설정 해제
		str.Format("This character set a \'%s\' Influence Leader.\n\n  Would you cancel this setting? (Will be change immediately)", CAtumSJ::GetInfluenceTypeString(m_byInfluenceType));
	}
	else
	{// 2006-05-03 by cmkwon, 세력리더로 설정
		str.Format("Would you set this characer to Influence Leader of \'%s\'? (Will be change immediately)", CAtumSJ::GetInfluenceTypeString(m_byInfluenceType));
	}
	nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{
		m_ctl_bInflLeader	= !m_ctl_bInflLeader;
		UpdateData(FALSE);
		return;
	}

	m_ctl_bInflSubLeader	= FALSE;
	m_ctl_bInflSub2Leader	= FALSE;
	UpdateData(FALSE);
	StoreInflLeader(m_byInfluenceType, (FALSE==m_ctl_bInflLeader)?0:m_CharacterUID);
}

void CCharacterInfoDlg::OnCHECKSubLeader() 
{
	// TODO: Add your control notification handler code here
	CString str;
	// 2006-12-13 by dhjin, 40레벨 미만은 부지도자가 될 수 없다.
	if (40 > m_Level)
	{
		str.Format("Required Level 40");
		MessageBox(str, NULL, MB_YESNO);
		UpdateData(FALSE);
		return;
	}	

	UpdateData();

	if(INFLUENCE_TYPE_VCN != m_byInfluenceType
		&& INFLUENCE_TYPE_ANI != m_byInfluenceType)
	{
		m_ctl_bInflLeader	= FALSE;
		UpdateData(FALSE);
		return;
	}

//	CString str;
	str.Format("IMServer and FieldServer must be shutdown if you want to change Influence SubLeader setting change.\n\n  IMServer and FieldServer is shutdowned?");
	int nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		m_ctl_bInflSubLeader	= !m_ctl_bInflSubLeader;
		UpdateData(FALSE);
		return;
	}

	if(FALSE == m_ctl_bInflSubLeader)
	{// 2006-05-03 by cmkwon, 세력리더 설정 해제
		str.Format("This character set a \'%s\' Influence SubLeader.\n\n  Would you cancel this setting? (Will be change immediately)", CAtumSJ::GetInfluenceTypeString(m_byInfluenceType));
	}
	else
	{// 2006-05-03 by cmkwon, 세력리더로 설정
		str.Format("Would you set this characer to Influence SubLeader of \'%s\'? (Will be change immediately)", CAtumSJ::GetInfluenceTypeString(m_byInfluenceType));
	}
	nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{
		m_ctl_bInflSubLeader	= !m_ctl_bInflSubLeader;
		UpdateData(FALSE);
		return;
	}

	m_ctl_bInflLeader	= FALSE;
	m_ctl_bInflSub2Leader	= FALSE;
	UpdateData(FALSE);
	StoreInflSubLeader(m_byInfluenceType, (FALSE==m_ctl_bInflSubLeader)?0:m_CharacterUID);	
}

void CCharacterInfoDlg::OnCHECKInflSub2Leader() 
{
	// TODO: Add your control notification handler code here
	CString str;
	// 2006-12-13 by dhjin, 40레벨 미만은 부지도자가 될 수 없다.
	if (40 > m_Level)
	{
		str.Format("Required Level 40");
		MessageBox(str, NULL, MB_YESNO);
		UpdateData(FALSE);
		return;
	}	

	UpdateData();
	
	if(INFLUENCE_TYPE_VCN != m_byInfluenceType
		&& INFLUENCE_TYPE_ANI != m_byInfluenceType)
	{
		m_ctl_bInflLeader	= FALSE;
		UpdateData(FALSE);
		return;
	}

//	CString str;
	str.Format("IMServer and FieldServer must be shutdown if you want to change Influence SubLeader setting change.\n\n  IMServer and FieldServer is shutdowned?");
	int nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{// 2006-05-03 by cmkwon, IMServer와 FieldServer가 종료 상태에만 변경해야 한다.
		m_ctl_bInflSub2Leader= !m_ctl_bInflSub2Leader;
		UpdateData(FALSE);
		return;
	}

	if(FALSE == m_ctl_bInflSub2Leader)
	{// 2006-05-03 by cmkwon, 세력리더 설정 해제
		str.Format("This character set a \'%s\' Influence SubLeader.\n\n  Would you cancel this setting? (Will be change immediately)", CAtumSJ::GetInfluenceTypeString(m_byInfluenceType));
	}
	else
	{// 2006-05-03 by cmkwon, 세력리더로 설정
		str.Format("Would you set this characer to Influence SubLeader of \'%s\'? (Will be change immediately)", CAtumSJ::GetInfluenceTypeString(m_byInfluenceType));
	}
	nRet = MessageBox(str, NULL, MB_YESNO);
	if(IDNO == nRet)
	{
		m_ctl_bInflSub2Leader	= !m_ctl_bInflSub2Leader;
		UpdateData(FALSE);
		return;
	}

	m_ctl_bInflSubLeader	= FALSE;
	m_ctl_bInflLeader	= FALSE;
	UpdateData(FALSE);
	StoreInflSub2Leader(m_byInfluenceType, (FALSE==m_ctl_bInflSub2Leader)?0:m_CharacterUID);	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCharacterInfoDlg::DBQ_CheckEnableChangeInfluence(void)
{
	CString szQuery;
	SQLHSTMT hstmt = m_pODBCStmt3->GetSTMTHandle();
	SQLINTEGER arrCB2[2] = {SQL_NTS,SQL_NTS};
	SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &m_AccountUID, 0,	&arrCB2[1]);
	BOOL bRet = m_pODBCStmt3->ExecuteQuery(PROCEDURE_090401_0347);
	if (!bRet)
	{
		MessageBox("DB Execute Error !!(CCharacterInfoDlg::DBQ_CheckEnableChangeInfluence#)");
		m_pODBCStmt3->FreeStatement();
		return FALSE;
	}

	int nErrorCode = ERR_COMMON_UNKNOWN_ERROR;
	arrCB2[1] = SQL_NTS;
	SQLBindCol(hstmt, 1, SQL_C_LONG, &nErrorCode, 0,				&arrCB2[1]);
	SQLFetch(m_pODBCStmt3->m_hstmt);
	m_pODBCStmt3->FreeStatement();				// free statement	

	if(ERR_NO_ERROR != nErrorCode)
	{
		CString csMsg;
		switch (nErrorCode)
		{
		case ERR_DELETE_CHARACTER_GUILDCOMMANDER:
			csMsg.Format("Account can't change influece because you had a character of guild master !!");
			break;
		case ERR_PROTOCOL_SUBLEADER_SET_ALEADY:
			csMsg.Format("Account can't change influece because you had a character of leader or sub leader !!");
			break;
		case ERR_REQ_REG_LEADER_CANDIDATE:
			csMsg.Format("Account can't change influece because you had a character of leader candidate !!");
			break;
		default:
			csMsg.Format("Account can't change influece !!");
		}
		AfxMessageBox(csMsg);
		return FALSE;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CCharacterInfoDlg::DBQ_InitializeInfluenceAccount(UID32_t i_AccUID)
{
	/*[Stored Query Definition]************************************************
	--------------------------------------------------------------------------------
	-- PROCEDURE NAME	: atum_Init_InfluenceType_ByAccountUID
	-- DESC				: // 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
	--						// 2009-10-12 by cmkwon, 프리스카 제거 방안 적용 - 
	--						
	--						#define QUEST_STATE_NONE			(BYTE)0		// 시작하지 않은 상태
	--						#define QUEST_STATE_IN_PROGRESS		(BYTE)1		// 진행중
	--						#define QUEST_STATE_COMPLETED		(BYTE)2		// 완료
	--
	--						#define QUEST_INDEX_OF_SELECT_INFLUENCE				112
	--------------------------------------------------------------------------------
	**************************************************************************/
	SQLBindParameter(m_pODBCStmt3->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &i_AccUID, 0, NULL);
	BOOL bRet = m_pODBCStmt3->ExecuteQuery(PROCEDURE_090401_0345);
	m_pODBCStmt3->FreeStatement();
	if (!bRet)
	{		
		return FALSE;
	}

	return TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 
/// \author		cmkwon
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CCharacterInfoDlg::OnBtnInitInfluence() 
{
	// TODO: Add your control notification handler code here

	///////////////////////////////////////////////////////////////////////////////
	// 2009-04-02 by cmkwon, 세력 변경이 가능한지 체크
	if(FALSE == DBQ_CheckEnableChangeInfluence())
	{
		AfxMessageBox("Fail to update character infomation !!");
		return;	
	}

	if(FALSE == this->DBQ_InitializeInfluenceAccount(m_AccountUID))
	{
		AfxMessageBox("Fail to change influence account !!");
		return;
	}

	m_byInfluenceType		= INFLUENCE_TYPE_NORMAL;
	m_bReloadCharacterInfo	= TRUE;						// 2009-03-31 by cmkwon, 세력초기화 시스템 구현 - 초기화 처리를 진행함.

	CComboBox *pComboInfluenceType = (CComboBox*)GetDlgItem(IDC_COMBO_INFLUENCETYPE);
	if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_NORMAL)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeNormal);}
	else if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_VCN)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeVCN);}
	else if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_ANI)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeANI);}
	else if(COMPARE_INFLUENCE(m_byInfluenceType, INFLUENCE_TYPE_RRP)){	pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeRRP);}
	else {																pComboInfluenceType->SetCurSel(COMBOIndex_INFLUENCETypeUnknown);}

	AfxMessageBox("Success !!");

	OnCancel();		// 2009-04-30 by cmkwon, 세력 초기화 관련 버그 수정 - 여기에서 캔슬로 기존 캐릭터 정보가 업데이트 되면 안된다.
}
