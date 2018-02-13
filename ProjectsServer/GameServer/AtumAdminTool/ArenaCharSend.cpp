// ArenaCharSend.cpp : implementation file
//

#include "stdafx.h"
#include "atumadmintool.h"
#include "ArenaCharSend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArenaCharSend dialog

#define BGEAR	0
#define MGEAR	1
#define IGEAR	2
#define AGEAR	3

#define ATT		0
#define DEF		1
#define DOD		2

#define SEND_ACCOUNT_COUNT		4
#define SEND_CHARACTER_COUNT		3

#define PASSWORD "04387btpmwf25t720vfw74ks8750df"

#define PROCEDURE_GET_CHARACTERINFO (UCHAR*)"{call dbo.arena_Get_Arena_SoruceCharacterInfo(?)}"
#define PROCEDURE_GET_SKILLINFO		(UCHAR*)"{call dbo.arena_Get_Arena_SoruceCharacterInfo_Skill(?)}"
#define PROCEDURE_GET_ITEMINFO		(UCHAR*)"{call dbo.arena_Get_Arena_SoruceCharacterInfo_Item(?)}"
#define PROCEDURE_GET_ENCHANTINFO	(UCHAR*)"{call dbo.arena_Get_Arena_SoruceCharacterInfo_Enchant(?)}"

#define QUARRY_
#define QUARRY_DECLARE_UID "\
DECLARE %s INT\n\
SET %s = NULL\n"
#define QUARRY_SELECT_AUID "\
SET @AUIDCHECK = NULL\n\
SELECT %s = AccountUniqueNumber FROM atum2_db_account.dbo.td_Account WITH (NOLOCK) WHERE AccountName = '%s'\n\
SELECT @AUIDCHECK = Password FROM atum2_db_account.dbo.td_Account WITH (NOLOCK) WHERE AccountUniqueNumber = %s\n\
	IF ( @AUIDCHECK <> '%s')\n\
	BEGIN\n\
		SELECT 'AUID ERROR [%s]'\n\
		RETURN\n\
	END\n"
#define QUARRY_INSERT_ACCOUNT "\
IF (%s IS NULL)\n\
BEGIN\n\
	INSERT INTO atum2_db_account.dbo.td_Account(AccountName, Password, AccountType)\n\
	VALUES('%s','%s', 0)\n\
	SELECT %s = AccountUniqueNumber FROM atum2_db_account.dbo.td_Account WITH (NOLOCK) WHERE AccountName = '%s' AND Password = '%s'\n\
END\n"
#define QUARRY_SELECT_CUID "\
SET @CUIDCHECK = NULL\n\
SELECT %s = UniqueNumber FROM td_Character WITH (NOLOCK) WHERE CharacterName = '%s'\n\
	IF ( %s IS NOT NULL )\n\
	BEGIN\n\
		SELECT @CUIDCHECK = AccountUniqueNumber FROM td_Character WITH (NOLOCK) WHERE UniqueNumber = %s\n\
		IF ( @CUIDCHECK <> %s )\n\
		BEGIN\n\
			SELECT 'CIUD ERROR [%s]'\n\
			RETURN\n\
		END\n\
	END\n"

#define QUARRY_DELETE_CHARACTER_INFO "\
SET @DELETE_CUID = NULL\n\
SET @DELETE_CUID = %s\n\
DELETE FROM td_Enchant 	WHERE TargetItemUniqueNumber IN (SELECT UniqueNumber FROM td_Store WITH (NOLOCK) WHERE Possess = @DELETE_CUID)\n\
DELETE FROM td_Store WHERE Possess = @DELETE_CUID\n\
DELETE FROM td_StoreSkill WHERE Possess = @DELETE_CUID\n\
DELETE FROM td_Character WHERE UniqueNumber = @DELETE_CUID\n\n"

#define QUARRY_INSERT_CHARACTER_INFO "\
INSERT INTO td_Character(CharacterName, AccountName, AccountUniqueNumber, Race, UnitKind, InfluenceType, AttackPart, DefensePart, FuelPart, SoulPart, ShieldPart, DodgePart, Level, Experience, BodyCondition, Propensity, Status, HP, DP, SP, EP, BonusStat, BonusStatPoint)\n\
VALUES	( '%s', '%s', %s, 2, %d, 1, %d, %d, %d, %d, %d, %d, %d, %f, %I64d, %d, %d, %d, %d, %d, %d, %d, %d )\n\
SELECT %s = UniqueNumber FROM td_Character WITH (NOLOCK) WHERE CharacterName = '%s'\n"

#define QUARRY_INSERT_SKILL_INFO "\
INSERT INTO td_StoreSkill (AccountUniqueNumber, Possess, ItemWindowIndex, ItemNum, CreatedTime)\n\
VALUES (%s, %s, 99, %d, GETDATE())\n"

#define QUARRY_INSERT_ITEM_INFO "\
SET @STORE_ITEM_UID = NULL\n\
SET @TARGET_ITEM_NUM = NULL\n\
INSERT INTO td_Store (AccountUniqueNumber,Possess,ItemStorage,Wear,CurrentCount,ItemWindowIndex,ItemNum,NumOfEnchants,PrefixCodeNum,SuffixCodeNum,CurrentEndurance,CreatedTime)\n\
VALUES (%s, %s, %d, %d, %d, %d, %d, 0, %d, %d, %f, GETDATE())\n\
SELECT @STORE_ITEM_UID = @@IDENTITY\n\
SET @TARGET_ITEM_NUM = %d\n"

#define QUARRY_INSERT_ENCHANT_INFO "\
	INSERT INTO td_Enchant (TargetItemUniqueNumber, TargetItemNum, EnchantItemNum)\n\
	VALUES (@STORE_ITEM_UID, @TARGET_ITEM_NUM, %d)\n"


CArenaCharSend::CArenaCharSend(CAtumAdminToolDlg *i_pMainDlg, CWnd* pParent /*=NULL*/)
: CDialog(CArenaCharSend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArenaCharSend)
	m_editBAccName = _T(CHARACTER_B_ACC_NAME);
	m_editBAttName = _T(CHARACTER_TYPE_B_ATT);
	m_editBDefName = _T(CHARACTER_TYPE_B_DEF);
	m_editBDodName = _T(CHARACTER_TYPE_B_DOD);
	
	m_editMAccName = _T(CHARACTER_M_ACC_NAME);
	m_editMAttName = _T(CHARACTER_TYPE_M_ATT);
	m_editMDefName = _T(CHARACTER_TYPE_M_DEF);
	m_editMDodName = _T(CHARACTER_TYPE_M_DOD);
	
	m_editIAccName = _T(CHARACTER_I_ACC_NAME);
	m_editIAttName = _T(CHARACTER_TYPE_I_ATT);
	m_editIDefName = _T(CHARACTER_TYPE_I_DEF);
	m_editIDodName = _T(CHARACTER_TYPE_I_DOD);
	
	m_editAAccName = _T(CHARACTER_A_ACC_NAME);
	m_editAAttName = _T(CHARACTER_TYPE_A_ATT);
	m_editADefName = _T(CHARACTER_TYPE_A_DEF);
	m_editADodName = _T(CHARACTER_TYPE_A_DOD);
	//}}AFX_DATA_INIT
	m_pMainDlg = i_pMainDlg;
	m_pODBCStmt = new CODBCStatement;
}

CArenaCharSend::~CArenaCharSend()
{
	util::del(m_pODBCStmt);
}

void CArenaCharSend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArenaCharSend)
	DDX_Text(pDX, IDC_EDIT_B_ACC_NAME, m_editBAccName);
	DDX_Text(pDX, IDC_EDIT_A_ACC_NAME, m_editAAccName);
	DDX_Text(pDX, IDC_EDIT_I_ACC_NAME, m_editIAccName);
	DDX_Text(pDX, IDC_EDIT_M_ACC_NAME, m_editMAccName);
	DDX_Text(pDX, IDC_EDIT_A_ATT_NAME, m_editAAttName);
	DDX_Text(pDX, IDC_EDIT_B_ATT_NAME, m_editBAttName);
	DDX_Text(pDX, IDC_EDIT_I_ATT_NAME, m_editIAttName);
	DDX_Text(pDX, IDC_EDIT_M_ATT_NAME, m_editMAttName);
	DDX_Text(pDX, IDC_EDIT_A_DEF_NAME, m_editADefName);
	DDX_Text(pDX, IDC_EDIT_B_DEF_NAME, m_editBDefName);
	DDX_Text(pDX, IDC_EDIT_I_DEF_NAME, m_editIDefName);
	DDX_Text(pDX, IDC_EDIT_M_DEF_NAME, m_editMDefName);
	DDX_Text(pDX, IDC_EDIT_A_DOD_NAME, m_editADodName);
	DDX_Text(pDX, IDC_EDIT_B_DOD_NAME, m_editBDodName);
	DDX_Text(pDX, IDC_EDIT_I_DOD_NAME, m_editIDodName);
	DDX_Text(pDX, IDC_EDIT_M_DOD_NAME, m_editMDodName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArenaCharSend, CDialog)
	//{{AFX_MSG_MAP(CArenaCharSend)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, OnButtonCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArenaCharSend message handlers

BOOL CArenaCharSend::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
		m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd()))
	{
		MessageBox(STRERR_S_SCADMINTOOL_0013);
		EndDialog(-1);
		return FALSE;
	}
	return TRUE;
}

void CArenaCharSend::OnButtonCreate() 
{
	// TODO: Add your control notification handler code here

	AccountInfo SendAccount[4];

	util::zero(SendAccount, sizeof(AccountInfo)*4);
	char SourceCharacterType[12][20] = {
		CHARACTER_TYPE_B_ATT,CHARACTER_TYPE_B_DEF,CHARACTER_TYPE_B_DOD,
		CHARACTER_TYPE_M_ATT,CHARACTER_TYPE_M_DEF,CHARACTER_TYPE_M_DOD,
		CHARACTER_TYPE_I_ATT,CHARACTER_TYPE_I_DEF,CHARACTER_TYPE_I_DOD,
		CHARACTER_TYPE_A_ATT,CHARACTER_TYPE_A_DEF,CHARACTER_TYPE_A_DOD};
	char SourceAccountName[4][20] = {
		CHARACTER_B_ACC_NAME, CHARACTER_M_ACC_NAME, CHARACTER_I_ACC_NAME, CHARACTER_A_ACC_NAME };
	char SourcePassword[35] = PASSWORD;
	
	int tempAccountCount = 0;
	int tempCharacterCount = 0;
	for(tempAccountCount = 0; tempAccountCount < SEND_ACCOUNT_COUNT; tempAccountCount++)
	{
		memcpy(SendAccount[tempAccountCount].AccountName, SourceAccountName[tempAccountCount], 20);
		memcpy(SendAccount[tempAccountCount].Password, SourcePassword, 35);
		for(tempCharacterCount = 0; tempCharacterCount < SEND_CHARACTER_COUNT; tempCharacterCount++)
		{
			memcpy(SendAccount[tempAccountCount].SendCharacter[tempCharacterCount].CharacterName, SourceCharacterType[(tempAccountCount*3)+tempCharacterCount], 20);
		}
		GetAccountInfo(&SendAccount[tempAccountCount]);
	}

	char DECLARE_AUID_NAME[4][15] = {"@B_COPY_AUID", "@M_COPY_AUID", "@I_COPY_AUID", "@A_COPY_AUID"};
	char DECLARE_CUID_NAME[12][15] = {
		"@B_ATT_CUID", "@B_DEF_CUID", "@B_DOD_CUID",
		"@M_ATT_CUID", "@M_DEF_CUID", "@M_DOD_CUID",
		"@I_ATT_CUID", "@I_DEF_CUID", "@I_DOD_CUID",
		"@A_ATT_CUID", "@A_DEF_CUID", "@A_DOD_CUID"};

	char *coment = NULL;
	char *EnterToken = "\n";
	FILE* filewrite = NULL;
	ATUM_DATE_TIME GetSaveTime;
	char SaveTime[25];
	GetSaveTime.SetCurrentDateTime(FALSE);
	sprintf(SaveTime, "%d-%d-%d %dH %dM %dS",GetSaveTime.Year, GetSaveTime.Month, GetSaveTime.Day, GetSaveTime.Hour, GetSaveTime.Minute, GetSaveTime.Second);
	char *FileTitle = "Arena Character Send Quarry ";
	char FileName[60] = "";
	sprintf(FileName, "%s%s.sql", FileTitle, SaveTime);
	CString SaveFileName = (LPSTR)FileName;
	filewrite = fopen(SaveFileName.GetBuffer(sizeof(SaveFileName)),"w");
	if( filewrite != NULL ) 
	{
		coment = "\n--계정 UID 변수 선언\n";
		fputs(coment, filewrite);
		{
			char DECLARE_AUID[50] = "";
			for(int DECLARE_AUID_ITR = 0; DECLARE_AUID_ITR < SEND_ACCOUNT_COUNT; DECLARE_AUID_ITR++)
			{
				util::zero(DECLARE_AUID, sizeof(DECLARE_AUID));
				sprintf(DECLARE_AUID, QUARRY_DECLARE_UID, DECLARE_AUID_NAME[DECLARE_AUID_ITR], DECLARE_AUID_NAME[DECLARE_AUID_ITR]);
				fputs(DECLARE_AUID, filewrite);
			}
		}
		
		coment = "\n--케릭터 UID 변수 선언\n";
		fputs(coment, filewrite);
		{
			char DECLARE_CUID[50] = "";
			for(int DECLARE_CUID_ITR = 0; DECLARE_CUID_ITR < SEND_ACCOUNT_COUNT*SEND_CHARACTER_COUNT; DECLARE_CUID_ITR++)
			{
				util::zero(DECLARE_CUID, sizeof(DECLARE_CUID));
				sprintf(DECLARE_CUID, QUARRY_DECLARE_UID, DECLARE_CUID_NAME[DECLARE_CUID_ITR], DECLARE_CUID_NAME[DECLARE_CUID_ITR]);
				fputs(DECLARE_CUID, filewrite);
			}
		}
		
		coment = "\n--계정 UID 파싱\nDECLARE @AUIDCHECK VARCHAR(35)\n";
		fputs(coment, filewrite);
		{
			char SELECT_AUID[1000] = "";
			for (int SELECT_AUID_ITR = 0; SELECT_AUID_ITR < SEND_ACCOUNT_COUNT; SELECT_AUID_ITR++)
			{
				util::zero(SELECT_AUID, sizeof(SELECT_AUID));
				sprintf(SELECT_AUID, QUARRY_SELECT_AUID
					, DECLARE_AUID_NAME[SELECT_AUID_ITR]
					, SendAccount[SELECT_AUID_ITR].AccountName
					, DECLARE_AUID_NAME[SELECT_AUID_ITR]
					, PASSWORD
					, DECLARE_AUID_NAME[SELECT_AUID_ITR]);
				fputs(SELECT_AUID, filewrite);
			}
		}
		
		coment = "\n--계정이 없으면 계정 추가\n";
		fputs(coment, filewrite);
		{
			char INSERT_ACCOUNT[200] = "";
			for (int INSERT_ACCOUNT_ITR = 0; INSERT_ACCOUNT_ITR < SEND_ACCOUNT_COUNT; INSERT_ACCOUNT_ITR++)
			{
				util::zero(INSERT_ACCOUNT, sizeof(INSERT_ACCOUNT));
				sprintf(INSERT_ACCOUNT, QUARRY_INSERT_ACCOUNT
					, DECLARE_AUID_NAME[INSERT_ACCOUNT_ITR]
					, SendAccount[INSERT_ACCOUNT_ITR].AccountName
					, SendAccount[INSERT_ACCOUNT_ITR].Password
					, DECLARE_AUID_NAME[INSERT_ACCOUNT_ITR]
					, SendAccount[INSERT_ACCOUNT_ITR].AccountName
					, PASSWORD);
				fputs(INSERT_ACCOUNT, filewrite);
			}
		}
		
		coment = "\n--기존 케릭터 UID 파싱\nDECLARE @CUIDCHECK int\n";
		fputs(coment, filewrite);
		{
			char SELECT_CUID[1000] = "";
			int SELECT_AUID_ITR = 0;
			int SELECT_CUID_ITR = 0;
			for (SELECT_AUID_ITR = 0; SELECT_AUID_ITR < SEND_ACCOUNT_COUNT; SELECT_AUID_ITR++)
			{
				for (SELECT_CUID_ITR = 0; SELECT_CUID_ITR < SEND_CHARACTER_COUNT; SELECT_CUID_ITR++)
			{
				util::zero(SELECT_CUID, sizeof(SELECT_CUID));
					sprintf(SELECT_CUID, QUARRY_SELECT_CUID
						, DECLARE_CUID_NAME[(SELECT_AUID_ITR*SEND_CHARACTER_COUNT)+SELECT_CUID_ITR]
						, SourceCharacterType[(SELECT_AUID_ITR*SEND_CHARACTER_COUNT)+SELECT_CUID_ITR]
						, DECLARE_CUID_NAME[(SELECT_AUID_ITR*SEND_CHARACTER_COUNT)+SELECT_CUID_ITR]
						, DECLARE_CUID_NAME[(SELECT_AUID_ITR*SEND_CHARACTER_COUNT)+SELECT_CUID_ITR]
						, DECLARE_AUID_NAME[SELECT_AUID_ITR]
						, DECLARE_CUID_NAME[(SELECT_AUID_ITR*SEND_CHARACTER_COUNT)+SELECT_CUID_ITR]);
				fputs(SELECT_CUID, filewrite);
			}
				fputs(EnterToken, filewrite);
			}
		}

		coment = "\n--기존 정보 삭제 : 인첸트, 아이템, 스킬\nDECLARE @DELETE_CUID INT\n\n";
		fputs(coment, filewrite);
		{
			char DELETE_CHARACTER_INFO[1000] = "";
			for (int DELETE_CUID_ITR = 0; DELETE_CUID_ITR < SEND_ACCOUNT_COUNT*SEND_CHARACTER_COUNT; DELETE_CUID_ITR++)
			{
				util::zero(DELETE_CHARACTER_INFO, sizeof(DELETE_CHARACTER_INFO));
				sprintf(DELETE_CHARACTER_INFO, QUARRY_DELETE_CHARACTER_INFO, DECLARE_CUID_NAME[DELETE_CUID_ITR]);
				fputs(DELETE_CHARACTER_INFO, filewrite);
			}
		}

		coment = "\n--케릭터 추가\n";
		fputs(coment, filewrite);
		{
			char INSERT_CHARACTER_INFO[2000] = "";
			int INSERT_CHARACTER_INFO_AUID_ITR = 0;
			int INSERT_CHARACTER_INFO_CUID_ITR = 0;
			for ( INSERT_CHARACTER_INFO_AUID_ITR = 0 ; INSERT_CHARACTER_INFO_AUID_ITR < SEND_ACCOUNT_COUNT; INSERT_CHARACTER_INFO_AUID_ITR++)
			{
				for ( INSERT_CHARACTER_INFO_CUID_ITR = 0 ; INSERT_CHARACTER_INFO_CUID_ITR < SEND_CHARACTER_COUNT; INSERT_CHARACTER_INFO_CUID_ITR++)
				{
					util::zero(INSERT_CHARACTER_INFO, sizeof(INSERT_CHARACTER_INFO));
					sprintf(INSERT_CHARACTER_INFO, QUARRY_INSERT_CHARACTER_INFO
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].CharacterName
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].AccountName
						, DECLARE_AUID_NAME[INSERT_CHARACTER_INFO_AUID_ITR]
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].UnitKind
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].AttackPart
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].DefensePart
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].FuelPart
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].SoulPart
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].ShieldPart
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].DodgePart
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].Level
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].Experience
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].BodyCondition
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].Propensity
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].Status
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].HP
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].DP
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].SP
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].EP
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].BonusStat
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].BonusStatPoint
						, DECLARE_CUID_NAME[(INSERT_CHARACTER_INFO_AUID_ITR*SEND_CHARACTER_COUNT)+INSERT_CHARACTER_INFO_CUID_ITR]
						, SendAccount[INSERT_CHARACTER_INFO_AUID_ITR].SendCharacter[INSERT_CHARACTER_INFO_CUID_ITR].CharacterName);
					fputs(INSERT_CHARACTER_INFO, filewrite);
				}
				fputs(EnterToken, filewrite);
			}
		}

		coment = "\n--스킬 추가\n";
		fputs(coment, filewrite);
		{
			char INSERT_SKILL_INFO[1000] = "";
			int INSERT_SKILL_INFO_AUID_ITR = 0;
			int INSERT_SKILL_INFO_CUID_ITR = 0;
			int SKILL_ITR = 0;
			for ( INSERT_SKILL_INFO_AUID_ITR = 0 ; INSERT_SKILL_INFO_AUID_ITR < SEND_ACCOUNT_COUNT; INSERT_SKILL_INFO_AUID_ITR++)
			{
				for ( INSERT_SKILL_INFO_CUID_ITR = 0 ; INSERT_SKILL_INFO_CUID_ITR < SEND_CHARACTER_COUNT; INSERT_SKILL_INFO_CUID_ITR++)
				{
					for (SKILL_ITR = 0; SKILL_ITR < SendAccount[INSERT_SKILL_INFO_AUID_ITR].SendCharacter[INSERT_SKILL_INFO_CUID_ITR].SkillCount; SKILL_ITR++)
					{
						util::zero(INSERT_SKILL_INFO, sizeof(INSERT_SKILL_INFO));
						sprintf(INSERT_SKILL_INFO, QUARRY_INSERT_SKILL_INFO
							, DECLARE_AUID_NAME[INSERT_SKILL_INFO_AUID_ITR]
							, DECLARE_CUID_NAME[(INSERT_SKILL_INFO_AUID_ITR*SEND_CHARACTER_COUNT)+INSERT_SKILL_INFO_CUID_ITR]
							, SendAccount[INSERT_SKILL_INFO_AUID_ITR].SendCharacter[INSERT_SKILL_INFO_CUID_ITR].SkillItemNum[SKILL_ITR]);
						fputs(INSERT_SKILL_INFO, filewrite);
					}
					fputs(EnterToken, filewrite);
				}
				fputs(EnterToken, filewrite);
			}
		}

		coment = "\n--아이템 추가\nDECLARE @STORE_ITEM_UID bigint\nDECLARE @TARGET_ITEM_NUM int\n";
		fputs(coment, filewrite);
		{
			char INSERT_ITEM_INFO[1000] = "";
			int INSERT_ITEM_INFO_AUID_ITR = 0;
			int INSERT_ITEM_INFO_CUID_ITR = 0;
			int ITEM_ITR = 0;
			char INSERT_ENCHANT_INFO[1000] = "";
			int ENCHANT_ITR = 0;
			int ENCHANT_COUNT = 0;
			for ( INSERT_ITEM_INFO_AUID_ITR = 0 ; INSERT_ITEM_INFO_AUID_ITR < SEND_ACCOUNT_COUNT; INSERT_ITEM_INFO_AUID_ITR++)
			{
				for ( INSERT_ITEM_INFO_CUID_ITR = 0 ; INSERT_ITEM_INFO_CUID_ITR < SEND_CHARACTER_COUNT; INSERT_ITEM_INFO_CUID_ITR++)
				{
					for (ITEM_ITR = 0; ITEM_ITR < SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].ItemCount; ITEM_ITR++)
					{
						util::zero(INSERT_ITEM_INFO, sizeof(INSERT_ITEM_INFO));
						sprintf(INSERT_ITEM_INFO, QUARRY_INSERT_ITEM_INFO
							, DECLARE_AUID_NAME[INSERT_ITEM_INFO_AUID_ITR]
							, DECLARE_CUID_NAME[(INSERT_ITEM_INFO_AUID_ITR*SEND_CHARACTER_COUNT)+INSERT_ITEM_INFO_CUID_ITR]
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].ItemStorage
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].Wear
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].CurrentCount
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].ItemWindowIndex
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].ItemNum
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].PrefixCodeNum
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].SuffixCodeNum
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].CurrentEndurance
							, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].ItemNum);
						fputs(INSERT_ITEM_INFO, filewrite);
						coment = "\t--인첸트 추가\n";
						fputs(coment, filewrite);
						for (ENCHANT_ITR = 0; ENCHANT_ITR < SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].EnchantTypeCount; ENCHANT_ITR++)
						{
							for (ENCHANT_COUNT = 0; ENCHANT_COUNT < SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].SendEnchant[ENCHANT_ITR].EnchantCount; ENCHANT_COUNT++)
							{
								util::zero(INSERT_ENCHANT_INFO, sizeof(INSERT_ENCHANT_INFO));
								sprintf(INSERT_ENCHANT_INFO, QUARRY_INSERT_ENCHANT_INFO
									, SendAccount[INSERT_ITEM_INFO_AUID_ITR].SendCharacter[INSERT_ITEM_INFO_CUID_ITR].SendItem[ITEM_ITR].SendEnchant[ENCHANT_ITR].EnchantItemNum);
								fputs(INSERT_ENCHANT_INFO, filewrite);
							}
						}
						fputs(EnterToken, filewrite);
					}
					fputs(EnterToken, filewrite);
				}
				fputs(EnterToken, filewrite);
			}
		}
		fclose(filewrite);
	}
	MessageBox("소스캐릭터 전달쿼리 생성완료.\r\n완성된 퀴리는 어드민 툴이 있는 곳에 있습니다.");
	EndDialog(-1);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaCharSend::GetAccountInfo(AccountInfo *o_AccountInfo)
/// \brief		전달 할 소스 케릭터의 계정 및 케릭터 수만큼 정보 로딩 명령 실행
/// \author		jhseol
/// \date		2012-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaCharSend::GetAccountInfo(AccountInfo *o_AccountInfo)
{
	int itrCharacterCount = 0;
	for(itrCharacterCount = 0; itrCharacterCount < SEND_CHARACTER_COUNT; itrCharacterCount++)
	{
		GetCharacterInfo(&o_AccountInfo->SendCharacter[itrCharacterCount]);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaCharSend::GetCharacterInfo(CharacterInfo *o_CharacterInfo)
/// \brief		전달 할 소스 케릭터의 케릭터 속성, 스킬, 아이템 정보 로딩
/// \author		jhseol
/// \date		2012-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaCharSend::GetCharacterInfo(CharacterInfo *o_CharacterInfo)
{
	/************************************************************************
	m_pODBCStmt->FreeStatement();
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_Get_Arena_SoruceCharacterInfo
	-- DESC				: 아레나 리뉴얼 전달 케릭터 정보 가져오기, 마상 내부에만 있는 쿼리
	-- 2012-06-17 by jhseol,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[arena_Get_Arena_SoruceCharacterInfo]
		@i_CharacterName varchar(20)
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT UniqueNumber, Race, UnitKind, AttackPart, DefensePart, FuelPart, SoulPart, ShieldPart, DodgePart, Level, Experience, BodyCondition, Propensity, Status, HP, DP, SP, EP, BonusStat, BonusStatPoint
		FROM td_Character WITH (NOLOCK) WHERE CharacterName = @i_CharacterName
	GO
	************************************************************************/
	SQLBindParameter(m_pODBCStmt->m_hstmt, 1,SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, o_CharacterInfo->CharacterName, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)PROCEDURE_GET_CHARACTERINFO);
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox("ERROR : PROCEDURE_GET_CHARACTERINFO");
		return ;
	}

	SQLBindCol(m_pODBCStmt->m_hstmt,  1, SQL_C_ULONG,	&o_CharacterInfo->CUID,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  2, SQL_C_ULONG,	&o_CharacterInfo->Race,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  3, SQL_C_ULONG,	&o_CharacterInfo->UnitKind,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  4, SQL_C_ULONG,	&o_CharacterInfo->AttackPart,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  5, SQL_C_ULONG,	&o_CharacterInfo->DefensePart,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  6, SQL_C_ULONG,	&o_CharacterInfo->FuelPart,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  7, SQL_C_ULONG,	&o_CharacterInfo->SoulPart,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  8, SQL_C_ULONG,	&o_CharacterInfo->ShieldPart,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  9, SQL_C_ULONG,	&o_CharacterInfo->DodgePart,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_ULONG,	&o_CharacterInfo->Level,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_FLOAT,	&o_CharacterInfo->Experience,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_SBIGINT,	&o_CharacterInfo->BodyCondition,	0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_ULONG,	&o_CharacterInfo->Propensity,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_ULONG,	&o_CharacterInfo->Status,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_ULONG,	&o_CharacterInfo->HP,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_ULONG,	&o_CharacterInfo->DP,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 17, SQL_C_ULONG,	&o_CharacterInfo->SP,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 18, SQL_C_ULONG,	&o_CharacterInfo->EP,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 19, SQL_C_ULONG,	&o_CharacterInfo->BonusStat,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt, 20, SQL_C_ULONG,	&o_CharacterInfo->BonusStatPoint,	0,	NULL);

	SQLRETURN sqlReturn = SQLFetch(m_pODBCStmt->m_hstmt);
	if ( ( sqlReturn != SQL_SUCCESS && sqlReturn != SQL_SUCCESS_WITH_INFO ) || sqlReturn == SQL_NO_DATA )
	{
		m_pODBCStmt->FreeStatement();
		MessageBox("ERROR : Character SQLFetch");
		return ;
	}
	m_pODBCStmt->FreeStatement();

////////////////////스킬//////////////////////////////////////

	/************************************************************************
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_Get_Arena_SoruceCharacterInfo_Skill
	-- DESC				: 아레나 리뉴얼 전달 케릭터 정보 가져오기 스킬, 마상 내부에만 있는 쿼리
	-- 2012-06-17 by jhseol,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[arena_Get_Arena_SoruceCharacterInfo_Skill]
		@i_CUID int
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT ItemNum FROM td_StoreSkill WITH (NOLOCK) WHERE Possess = @i_CUID
	GO
	************************************************************************/
	SQLBindParameter ( m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &o_CharacterInfo->CUID, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)PROCEDURE_GET_SKILLINFO);
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox("ERROR : PROCEDURE_GET_SKILLINFO");
		return ;
	}
	int tempSkillNum = 0;
	int tempSkillCount = 0;

	SQLBindCol(m_pODBCStmt->m_hstmt,  1, SQL_C_ULONG,	&tempSkillNum,				0,	NULL);
	while ((sqlReturn = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (sqlReturn != SQL_SUCCESS)
		{
			m_pODBCStmt->FreeStatement();
			MessageBox("ERROR : Skill SQLFetch");
			return ;
		}
		o_CharacterInfo->SkillItemNum[tempSkillCount] = tempSkillNum;
		tempSkillCount++;
	}
	o_CharacterInfo->SkillCount = tempSkillCount;
	m_pODBCStmt->FreeStatement();

////////////////////////아이템////////////////////////////

	/************************************************************************
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_Get_Arena_SoruceCharacterInfo_Item
	-- DESC				: 아레나 리뉴얼 전달 케릭터 정보 가져오기 아이템, 마상 내부에만 있는 쿼리
	-- 2012-06-17 by jhseol,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[arena_Get_Arena_SoruceCharacterInfo_Item]
		@i_CUID int
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT UniqueNumber, ItemStorage, Wear, CurrentCount, ItemWindowIndex, ItemNum, PrefixCodeNum, SuffixCodeNum, CurrentEndurance
		FROM td_Store WITH (NOLOCK) WHERE Possess = @i_CUID
	GO
	************************************************************************/
	SQLBindParameter ( m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_ULONG, SQL_INTEGER, 0, 0, &o_CharacterInfo->CUID, 0, NULL);
	bRet = m_pODBCStmt->ExecuteQuery((char*)PROCEDURE_GET_ITEMINFO);
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox("ERROR : PROCEDURE_GET_ITEMINFO");
		return ;
	}
	int tempItemCount = 0;
	ItemInfo tempItemInfo;
	util::zero(&tempItemInfo,sizeof(ItemInfo));

	SQLBindCol(m_pODBCStmt->m_hstmt,  1, SQL_C_SBIGINT,	&tempItemInfo.StoreUID,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  2, SQL_C_ULONG,	&tempItemInfo.ItemStorage,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  3, SQL_C_ULONG,	&tempItemInfo.Wear,				0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  4, SQL_C_ULONG,	&tempItemInfo.CurrentCount,		0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  5, SQL_C_ULONG,	&tempItemInfo.ItemWindowIndex,	0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  6, SQL_C_ULONG,	&tempItemInfo.ItemNum,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  7, SQL_C_ULONG,	&tempItemInfo.PrefixCodeNum,	0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  8, SQL_C_ULONG,	&tempItemInfo.SuffixCodeNum,	0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  9, SQL_C_FLOAT,	&tempItemInfo.CurrentEndurance,	0,	NULL);
	while ((sqlReturn = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (sqlReturn != SQL_SUCCESS)
		{
			m_pODBCStmt->FreeStatement();
			MessageBox("ERROR : Item SQLFetch");
			return ;
		}
		o_CharacterInfo->SendItem[tempItemCount] = tempItemInfo;
		tempItemCount++;
	}
	o_CharacterInfo->ItemCount = tempItemCount;
	m_pODBCStmt->FreeStatement();

///////////////////////인첸트//////////////////////////////////////////

	int itrItemCount = 0;
	for(itrItemCount = 0; itrItemCount < o_CharacterInfo->ItemCount; itrItemCount++)
	{
		GetEnchantInfo(&o_CharacterInfo->SendItem[itrItemCount]);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CArenaCharSend::GetEnchantInfo(ItemInfo *o_ItemInfo)
/// \brief		전달 할 소스 케릭터의 아이템 인첸트 정보 로딩
/// \author		jhseol
/// \date		2012-06-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CArenaCharSend::GetEnchantInfo(ItemInfo *o_ItemInfo)
{
	m_pODBCStmt->FreeStatement();
	/************************************************************************
	m_pODBCStmt->FreeStatement();
	-------------------------------------------------------------------------------
	-- PROCEDURE NAME	: dbo.arena_Get_Arena_SoruceCharacterInfo_Enchant
	-- DESC				: 아레나 리뉴얼 전달 케릭터 정보 가져오기 인첸트, 마상 내부에만 있는 쿼리
	-- 2012-06-17 by jhseol,
	--------------------------------------------------------------------------------
	CREATE PROCEDURE [dbo].[arena_Get_Arena_SoruceCharacterInfo_Enchant]
		@i_StoreUID bigint
	-- // 2009-02-17 by cmkwon, MARK for Yedang					-- // 2008-09-04 by cmkwon, 예당 보안관련 숨긴 계정 적용 하기(SQL Server 2005이상만 지원하는 기능) - 
	AS
		SELECT COUNT(*), EnchantItemNum
		FROM td_Enchant WITH (NOLOCK)
		WHERE TargetItemUniqueNumber = @i_StoreUID
		GROUP BY [EnchantItemNum]
	GO
	************************************************************************/
	SQLBindParameter ( m_pODBCStmt->m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_SBIGINT,	SQL_BIGINT, 0, 0, &o_ItemInfo->StoreUID, 0, NULL);
	BOOL bRet = m_pODBCStmt->ExecuteQuery((char*)PROCEDURE_GET_ENCHANTINFO);
	if(FALSE == bRet)
	{
		m_pODBCStmt->FreeStatement();
		MessageBox("ERROR : PROCEDURE_GET_ENCHANTINFO");
		return ;
	}
	int tempEnchantTypeCount = 0;
	EnchantInfo tempEnchantInfo;
	util::zero(&tempEnchantInfo,sizeof(EnchantInfo));
	
	SQLBindCol(m_pODBCStmt->m_hstmt,  1, SQL_C_ULONG,	&tempEnchantInfo.EnchantCount,			0,	NULL);
	SQLBindCol(m_pODBCStmt->m_hstmt,  2, SQL_C_ULONG,	&tempEnchantInfo.EnchantItemNum,		0,	NULL);
	SQLRETURN sqlReturn;
	while ((sqlReturn = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		if (sqlReturn != SQL_SUCCESS)
		{
			m_pODBCStmt->FreeStatement();
			MessageBox("ERROR : Enchant SQLFetch");
			return ;
		}
		o_ItemInfo->SendEnchant[tempEnchantTypeCount] = tempEnchantInfo;
		tempEnchantTypeCount++;
	}
	o_ItemInfo->EnchantTypeCount = tempEnchantTypeCount;
	m_pODBCStmt->FreeStatement();
}

