// WeaponItemDetail.cpp : implementation file
//

#include "stdafx.h"
#include "SpaceCowboyDBEditorTool.h"
#include "WeaponItemDetail.h"
#include "SpaceCowboyDBEditorToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWeaponItemDetail dialog


CWeaponItemDetail::CWeaponItemDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CWeaponItemDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWeaponItemDetail)
	m_valueEditItemNum = _T("");
	m_valueEditAttackMax = _T("0");
	m_valueEditAttackMin = _T("0");
	m_valueEditReqAttack = _T("0");
	m_valueEditReqDefense = _T("0");
	m_valueEditReqFuel = _T("0");
	m_valueEditReqSoul = _T("0");
	m_valueEditReqShield = _T("0");
	m_valueEditReqDodge = _T("0");
	m_valueCheckBGEAR = FALSE;
	m_valueCheckIGEAR = FALSE;
	m_valueCheckMGEAR = FALSE;
	m_valueCheckAGEAR = FALSE;
	m_valueEditMinLevel = _T("0");
	m_valueEditMaxLevel = _T("0");
	m_valueEditWeight = _T("0");
	m_valueEditHitRate = _T("0");
	m_valueEditDefense = _T("0");
	m_valueEditRange = _T("0");
	m_valueEditAbrasionRate = _T("0");
	m_valueEditCharging = _T("0");
	m_valueEditMinTradeQuantity = _T("1");
	m_valueEditPrice = _T("0");
	m_valueEditCashPrice = _T("0");
	m_valueEditReattackTime = _T("0");
	m_valueEditOverHitTime = _T("0");
	m_valueEditMultiTarget = _T("1");
	m_valueEditExplosionRange = _T("0");
	m_valueEditReactionRange = _T("0");
	m_valueEditShotNum = _T("0");
	m_valueEditMultiNum = _T("0");
	m_valueEditAttackTime = _T("0");
	m_valueEditItemName = _T("0");
	m_valueEditDescription = _T("0");
	m_valueCheckCashItem = FALSE;
	m_valueEditCaliber = _T("0");
	m_valueEditOrbitType = _T("0");
	m_valueEditCameraPattern = _T("0");
	m_valueCheckAutoPickUp = FALSE;
	m_valueCheckQuestItem = FALSE;
	m_valueCheckTimeLimite = FALSE;
	m_valueCheckNoTransfer = FALSE;
	m_valueEditRangeAngle = _T("0");
	m_valueEditSourceIndex = _T("0");
	//}}AFX_DATA_INIT
	m_pMainDlg = (CSpaceCowboyDBEditorToolDlg*)AfxGetMainWnd();
	m_pODBCStmt				= m_pMainDlg->m_pODBCStmt;
	m_pWeaponItem = (CWeaponItem*)pParent;
	m_nSelectRow = 0;
	m_pLinkDetailDlg = NULL;
}

CWeaponItemDetail::~CWeaponItemDetail()
{
	util::del(m_pLinkDetailDlg);
// 2006-01-23 by cmkwon
//	util::del(m_pODBCStmt);
}


void CWeaponItemDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeaponItemDetail)
	DDX_Control(pDX, IDC_IE_CHECK_CASHITEM, m_ctrlCheckCashItem);
	DDX_Control(pDX, IDC_IE_COMBO_ITEMATTRIBUTE, m_ctrlComboItemAttribute);
	DDX_Control(pDX, IDC_IE_COMBO_RACE2, m_ctrlComboRace2);
	DDX_Control(pDX, IDC_IE_BUTTON_SAVE, m_ctrlButtonSave);
	DDX_Control(pDX, IDC_IE_BUTTON_CANCEL, m_ctrlButtonCancel);
	DDX_Control(pDX, IDC_IE_BUTTON_MAKE, m_ctrlButtonMake);
	DDX_Control(pDX, IDC_IE_COMBO_LINK, m_ctrlComboLink);
	DDX_Control(pDX, IDC_IE_COMBO_POSITION, m_ctrlComboPosition);
	DDX_Control(pDX, IDC_IE_COMBO_ITEM_KIND, m_ctrlComboItemKind);
	DDX_Control(pDX, IDC_IE_COMBO_RACE, m_ctrlComboRace);
	DDX_Control(pDX, IDC_IE_COMBO_KIND, m_ctrlComboKind);
	DDX_Text(pDX, IDC_IE_EDIT29, m_valueEditItemNum);
	DDX_Text(pDX, IDC_IE_EDIT_ATTACKMAX, m_valueEditAttackMax);
	DDX_Text(pDX, IDC_IE_EDIT_ATTACKMIN, m_valueEditAttackMin);
	DDX_Text(pDX, IDC_IE_EDIT_REQATTACK, m_valueEditReqAttack);
	DDX_Text(pDX, IDC_IE_EDIT_REQDEFENSE, m_valueEditReqDefense);
	DDX_Text(pDX, IDC_IE_EDIT_REQFUEL, m_valueEditReqFuel);
	DDX_Text(pDX, IDC_IE_EDIT_REQSOUL, m_valueEditReqSoul);
	DDX_Text(pDX, IDC_IE_EDIT_REQSHIELD, m_valueEditReqShield);
	DDX_Text(pDX, IDC_IE_EDIT_REQDODGE, m_valueEditReqDodge);
	DDX_Check(pDX, IDC_IE_CHECK_BGEAR, m_valueCheckBGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_IGEAR, m_valueCheckIGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_MGEAR, m_valueCheckMGEAR);
	DDX_Check(pDX, IDC_IE_CHECK_AGEAR, m_valueCheckAGEAR);
	DDX_Text(pDX, IDC_IE_EDIT16, m_valueEditMinLevel);
	DDX_Text(pDX, IDC_IE_EDIT17, m_valueEditMaxLevel);
	DDX_Text(pDX, IDC_IE_EDIT14, m_valueEditWeight);
	DDX_Text(pDX, IDC_IE_EDIT11, m_valueEditHitRate);
	DDX_Text(pDX, IDC_IE_EDIT28, m_valueEditDefense);
	DDX_Text(pDX, IDC_IE_EDIT10, m_valueEditRange);
	DDX_Text(pDX, IDC_IE_EDIT19, m_valueEditAbrasionRate);
	DDX_Text(pDX, IDC_IE_EDIT20, m_valueEditCharging);
	DDX_Text(pDX, IDC_IE_EDIT27, m_valueEditMinTradeQuantity);
	DDX_Text(pDX, IDC_IE_EDIT21, m_valueEditPrice);
	DDX_Text(pDX, IDC_IE_EDIT31, m_valueEditCashPrice);
	DDX_Text(pDX, IDC_IE_EDIT22, m_valueEditReattackTime);
	DDX_Text(pDX, IDC_IE_EDIT23, m_valueEditOverHitTime);
	DDX_Text(pDX, IDC_IE_EDIT26, m_valueEditMultiTarget);
	DDX_Text(pDX, IDC_IE_EDIT24, m_valueEditExplosionRange);
	DDX_Text(pDX, IDC_IE_EDIT25, m_valueEditReactionRange);
	DDX_Text(pDX, IDC_IE_EDIT12, m_valueEditShotNum);
	DDX_Text(pDX, IDC_IE_EDIT13, m_valueEditMultiNum);
	DDX_Text(pDX, IDC_IE_EDIT18, m_valueEditAttackTime);
	DDX_Text(pDX, IDC_IE_EDIT32, m_valueEditItemName);
	DDX_Text(pDX, IDC_IE_EDIT30, m_valueEditDescription);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM, m_valueCheckCashItem);
	DDX_Text(pDX, IDC_IE_EDIT33, m_valueEditCaliber);
	DDX_Text(pDX, IDC_IE_EDIT35, m_valueEditOrbitType);
	DDX_Text(pDX, IDC_IE_EDIT34, m_valueEditCameraPattern);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM2, m_valueCheckAutoPickUp);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM4, m_valueCheckQuestItem);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM5, m_valueCheckTimeLimite);
	DDX_Check(pDX, IDC_IE_CHECK_CASHITEM3, m_valueCheckNoTransfer);
	DDX_Text(pDX, IDC_IE_EDIT37, m_valueEditRangeAngle);
	DDX_Text(pDX, IDC_IE_EDIT38, m_valueEditSourceIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWeaponItemDetail, CDialog)
	//{{AFX_MSG_MAP(CWeaponItemDetail)
	ON_BN_CLICKED(IDC_IE_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_IE_BUTTON_MAKE, OnButtonMake)
	ON_CBN_EDITCHANGE(IDC_IE_COMBO_KIND, OnEditchangeComboKind)
	ON_BN_CLICKED(IDC_IE_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_IE_BUTTON_LINKDETAIL, OnButtonLinkdetail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeaponItemDetail message handlers

BOOL CWeaponItemDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
// 2006-01-23 by cmkwon
//	if (!m_pODBCStmt->Init(m_pMainDlg->m_pServerInfo4Admin->DBIP, m_pMainDlg->m_pServerInfo4Admin->DBPort, m_pMainDlg->m_pServerInfo4Admin->DBName,
//							m_pMainDlg->m_pServerInfo4Admin->DBUID, m_pMainDlg->m_pServerInfo4Admin->DBPWD, GetSafeHwnd())
//	)
//	{
////		MessageBox(STRERR_S_SCADMINTOOL_0013);
//		EndDialog(-1);
//		return FALSE;
//	}

	m_ctrlComboKind.InsertString(0,STRCMD_CS_ITEMKIND_AUTOMATIC);
	m_ctrlComboKind.InsertString(1,STRCMD_CS_ITEMKIND_VULCAN);
	m_ctrlComboKind.InsertString(2,STRCMD_CS_ITEMKIND_DUALIST);
	m_ctrlComboKind.InsertString(3,STRCMD_CS_ITEMKIND_CANNON);
	m_ctrlComboKind.InsertString(4,STRCMD_CS_ITEMKIND_RIFLE);
	m_ctrlComboKind.InsertString(5,STRCMD_CS_ITEMKIND_GATLING);
	m_ctrlComboKind.InsertString(6,STRCMD_CS_ITEMKIND_LAUNCHER);
	m_ctrlComboKind.InsertString(7,STRCMD_CS_ITEMKIND_MASSDRIVE);
	m_ctrlComboKind.InsertString(8,STRCMD_CS_ITEMKIND_ROCKET);
	m_ctrlComboKind.InsertString(9,STRCMD_CS_ITEMKIND_MISSILE);
	m_ctrlComboKind.InsertString(10,STRCMD_CS_ITEMKIND_BUNDLE);
	m_ctrlComboKind.InsertString(11,STRCMD_CS_ITEMKIND_MINE);

	m_ctrlComboRace.InsertString(0,STRCMD_CS_COMMON_RACE_BATTALUS);
	m_ctrlComboRace.InsertString(1,STRCMD_CS_COMMON_RACE_DECA);
	m_ctrlComboRace.InsertString(2,STRCMD_CS_COMMON_RACE_PHILON);
	m_ctrlComboRace.InsertString(3,STRCMD_CS_COMMON_RACE_SHARRINE);
	//콤보 박스 나누기
	m_ctrlComboRace2.InsertString(0,STRCMD_CS_COMMON_RACE_OPERATION);
	m_ctrlComboRace2.InsertString(1,STRCMD_CS_COMMON_RACE_GAMEMASTER);
	m_ctrlComboRace2.InsertString(2,STRCMD_CS_COMMON_RACE_MONITOR);
	m_ctrlComboRace2.InsertString(3,STRCMD_CS_COMMON_RACE_GUEST);
	m_ctrlComboRace2.InsertString(4,STRCMD_CS_COMMON_RACE_DEMO);

	m_ctrlComboItemKind.InsertString(0,STRCMD_CS_ITEMKIND_ALL_WEAPON);
	m_ctrlComboItemKind.InsertString(1,STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_ALL);
	m_ctrlComboItemKind.InsertString(2,STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_1);
	m_ctrlComboItemKind.InsertString(3,STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_2);
	m_ctrlComboItemKind.InsertString(4,STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_ALL);
	m_ctrlComboItemKind.InsertString(5,STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_1);
	m_ctrlComboItemKind.InsertString(6,STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_2);

	m_ctrlComboPosition.InsertString(0,STRCMD_CS_POS_PROW);
	m_ctrlComboPosition.InsertString(1,STRCMD_CS_POS_PROWIN);
	m_ctrlComboPosition.InsertString(2,STRCMD_CS_POS_PROWOUT);
	m_ctrlComboPosition.InsertString(3,STRCMD_CS_POS_WINGIN);
	m_ctrlComboPosition.InsertString(4,STRCMD_CS_POS_WINGOUT);
	m_ctrlComboPosition.InsertString(5,STRCMD_CS_POS_CENTER);
	m_ctrlComboPosition.InsertString(6,STRCMD_CS_POS_REAR);
	m_ctrlComboPosition.InsertString(7,STRCMD_CS_POS_ACCESSORY_UNLIMITED /*STRCMD_CS_POS_ATTACHMENT*/);	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
	m_ctrlComboPosition.InsertString(8,STRCMD_CS_POS_ACCESSORY_TIME_LIMIT /*STRCMD_CS_POS_PET*/);	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
	m_ctrlComboPosition.InsertString(9,STRCMD_CS_POS_PET);											// 2010-06-15 by shcho&hslee 펫시스템 - 팻 추가.

	//DB에서 link 넣기
	CString szSQLQuery;
	
	//kind가 23인 item 가져오기
	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
	//szSQLQuery.Format("select itemnum from ti_item WITH (NOLOCK) where kind = %d"
	//	, ITEMKIND_BULLET);
	szSQLQuery.Format(QUERY_080702_0234
		, ITEMKIND_BULLET);

	SQLINTEGER cb1;

	int tempItemnum;
	CString szItemnum;

	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &tempItemnum, 0, &cb1);

	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
	if (!bRet)
	{
		// clean up
		m_pODBCStmt->FreeStatement();
		MessageBox("Error");
		return FALSE;
	}

	// DB에 값이 없을때까지 loop를 돈다
	while ( (bRet = SQLFetch(m_pODBCStmt->m_hstmt)) != SQL_NO_DATA)
	{
		szItemnum.Format("%d", tempItemnum);
		//select된 값을 넣어준다.
		m_ctrlComboLink.InsertString(0,szItemnum);
	}

	// clean up
	m_pODBCStmt->FreeStatement();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWeaponItemDetail::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CWeaponItemDetail::OnButtonMake() 
{
	if(ValidateCheck()== FALSE)
		return;
	ReflectionToGrid(1);
}

void CWeaponItemDetail::OnEditchangeComboKind() 
{
	// TODO: Add your control notification handler code here
	return;
}

void CWeaponItemDetail::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	
	if(ValidateCheck()== FALSE)
		return;
	m_pWeaponItem->m_GridCurrent.DeleteRow(m_nSelectRow);
	ReflectionToGrid(2);
	
}

void CWeaponItemDetail::ReflectionToGrid(int type)
{
	// TODO: Add your control notification handler code here
	// 칼럼 만들기
	m_pWeaponItem->m_GridUpdate.SetRowCount(m_pWeaponItem->m_GridUpdate.GetRowCount()+1);

	//각 콘트롤의 값들을 CWeaponItem의 GridUpdate에 넣는다.
	UpdateData();
	
	int m_nCols= 0;
	CString tempValue;
	int n_tempValue = 0;
	if(type == 1)
		m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, "INSERT");
	else if(type == 2)
		m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, "UPDATE");
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemNum);
	m_ctrlComboKind.GetWindowText(tempValue);
	if(tempValue == STRCMD_CS_ITEMKIND_AUTOMATIC)
		tempValue.Format("%d", ITEMKIND_AUTOMATIC);
	else if(tempValue == STRCMD_CS_ITEMKIND_VULCAN)
		tempValue.Format("%d", ITEMKIND_VULCAN);
	else if(tempValue == STRCMD_CS_ITEMKIND_DUALIST)
		tempValue.Format("%d", ITEMKIND_DUALIST);
	else if(tempValue == STRCMD_CS_ITEMKIND_CANNON)
		tempValue.Format("%d", ITEMKIND_CANNON);
	else if(tempValue == STRCMD_CS_ITEMKIND_RIFLE)
		tempValue.Format("%d", ITEMKIND_RIFLE);
	else if(tempValue == STRCMD_CS_ITEMKIND_GATLING)
		tempValue.Format("%d", ITEMKIND_GATLING);
	else if(tempValue == STRCMD_CS_ITEMKIND_LAUNCHER)
		tempValue.Format("%d", ITEMKIND_LAUNCHER);
	else if(tempValue == STRCMD_CS_ITEMKIND_MASSDRIVE)
		tempValue.Format("%d", ITEMKIND_MASSDRIVE);
	else if(tempValue == STRCMD_CS_ITEMKIND_ROCKET)
		tempValue.Format("%d", ITEMKIND_ROCKET);
	else if(tempValue == STRCMD_CS_ITEMKIND_MISSILE)
		tempValue.Format("%d", ITEMKIND_MISSILE);
	else if(tempValue == STRCMD_CS_ITEMKIND_BUNDLE)
		tempValue.Format("%d", ITEMKIND_BUNDLE);
	else if(tempValue == STRCMD_CS_ITEMKIND_MINE)
		tempValue.Format("%d", ITEMKIND_MINE);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditItemName);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAttackMin);//abilitymin
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAttackMax);//abilitymax
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRangeAngle);

	m_ctrlComboRace.GetWindowText(tempValue);
	int tempbit1 = 0;
	int tempbit2 = 0;
	//여기는 바탈~샤린, 관리자~시연 2의 비트 조합으로 DB에 넣는다.
	if(tempValue == STRCMD_CS_COMMON_RACE_BATTALUS)
		tempbit1 = RACE_BATTALUS;
	else if(tempValue == STRCMD_CS_COMMON_RACE_DECA)
		tempbit1 = RACE_DECA;
	else if(tempValue == STRCMD_CS_COMMON_RACE_PHILON)
		tempbit1 = RACE_PHILON;
	else if(tempValue == STRCMD_CS_COMMON_RACE_SHARRINE)
		tempbit1 = RACE_SHARRINE;
	m_ctrlComboRace2.GetWindowText(tempValue);
	if(tempValue == STRCMD_CS_COMMON_RACE_OPERATION)
		tempbit2 = RACE_OPERATION;
	else if(tempValue == STRCMD_CS_COMMON_RACE_GAMEMASTER)
		tempbit2 = RACE_GAMEMASTER;
	else if(tempValue == STRCMD_CS_COMMON_RACE_MONITOR)
		tempbit2 = RACE_MONITOR;
	else if(tempValue == STRCMD_CS_COMMON_RACE_GUEST)
		tempbit2 = RACE_GUEST;
	else if(tempValue == STRCMD_CS_COMMON_RACE_DEMO)
		tempbit2 = RACE_DEMO;

	tempValue.Format("%d", tempbit1+tempbit2);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqAttack);//ReqAttackPart
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqDefense);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqFuel);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqSoul);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqShield);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReqDodge);//
	tempbit1 = 0;

	if(m_valueCheckBGEAR == TRUE)
		tempbit1 += UNITKIND_BGEAR_MASK;
	if(m_valueCheckIGEAR == TRUE)
		tempbit1 += UNITKIND_IGEAR_MASK;
	if(m_valueCheckMGEAR == TRUE)
		tempbit1 += UNITKIND_MGEAR_MASK;
	if(m_valueCheckAGEAR == TRUE)
		tempbit1 += UNITKIND_AGEAR_MASK;
	tempValue.Format("%d", tempbit1+tempbit2);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//ReqUnitItem
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMinLevel);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMaxLevel);//

	m_ctrlComboItemKind.GetWindowText(tempValue);
	if(tempValue == STRCMD_CS_ITEMKIND_ALL_WEAPON)
		tempValue.Format("%d", ITEMKIND_ALL_WEAPON);
	else if(tempValue == STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_ALL)
		tempValue.Format("%d", ITEMKIND_PRIMARY_WEAPON_ALL);
	else if(tempValue == STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_1)
		tempValue.Format("%d", ITEMKIND_PRIMARY_WEAPON_1);
	else if(tempValue == STRCMD_CS_ITEMKIND_PRIMARY_WEAPON_2)
		tempValue.Format("%d", ITEMKIND_PRIMARY_WEAPON_2);
	else if(tempValue == STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_ALL)
		tempValue.Format("%d", ITEMKIND_SECONDARY_WEAPON_ALL);
	else if(tempValue == STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_1)
		tempValue.Format("%d", ITEMKIND_SECONDARY_WEAPON_1);
	else if(tempValue == STRCMD_CS_ITEMKIND_SECONDARY_WEAPON_2)
		tempValue.Format("%d", ITEMKIND_SECONDARY_WEAPON_2);
	else
		tempValue = "0";
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//ReqItemKind
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditWeight);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditHitRate);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDefense);//

	//Description, ItemAttribute
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditRange);//

	m_ctrlComboPosition.GetWindowText(tempValue);
	if(tempValue == STRCMD_CS_POS_PROW)
		tempValue.Format("%d", POS_PROW);
	else if(tempValue == STRCMD_CS_POS_PROWIN)
		tempValue.Format("%d", POS_PROWIN);
	else if(tempValue == STRCMD_CS_POS_PROWOUT)
		tempValue.Format("%d", POS_PROWOUT);
	else if(tempValue == STRCMD_CS_POS_WINGIN)
		tempValue.Format("%d", POS_WINGIN);
	else if(tempValue == STRCMD_CS_POS_WINGOUT)
		tempValue.Format("%d", POS_WINGOUT);
	else if(tempValue == STRCMD_CS_POS_CENTER)
		tempValue.Format("%d", POS_CENTER);
	else if(tempValue == STRCMD_CS_POS_REAR)
		tempValue.Format("%d", POS_REAR);
	else if(tempValue == STRCMD_CS_POS_ACCESSORY_UNLIMITED/*STRCMD_CS_POS_ATTACHMENT*/)	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
		tempValue.Format("%d", POS_ACCESSORY_UNLIMITED /*POS_ATTACHMENT*/);
	else if(tempValue == STRCMD_CS_POS_ACCESSORY_TIME_LIMIT /*STRCMD_CS_POS_PET*/)	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
		tempValue.Format("%d", POS_ACCESSORY_TIME_LIMIT /*POS_PET*/);	// 2010-06-15 by shcho&hslee 펫시스템 - 선언 변경.
	else if ( tempValue == STRCMD_CS_POS_PET )
		tempValue.Format ( "%d" , POS_PET );							// 2010-06-15 by shcho&hslee 펫시스템 - 팻 추가.
	
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//Position
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAbrasionRate);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCharging);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMinTradeQuantity);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditPrice);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCashPrice);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReattackTime);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditOverHitTime);//
	m_ctrlComboLink.GetWindowText(tempValue);
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//
	
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMultiTarget);//MultiTarget
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditExplosionRange);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditReactionRange);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditShotNum);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditMultiNum);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditAttackTime);//
	
	//ItemAttribute
	if(m_valueCheckAutoPickUp == TRUE)
		n_tempValue += 0x01;
	if(m_valueCheckNoTransfer == TRUE)
		n_tempValue += 0x02;
	if(m_valueCheckQuestItem == TRUE)
		n_tempValue += 0x04;
	if(m_valueCheckTimeLimite == TRUE)
		n_tempValue += 0x08;
	if(m_valueCheckCashItem == TRUE)
		n_tempValue += 0x00010000;
	tempValue.Format("%d", n_tempValue);

	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, tempValue);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditDescription);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCaliber);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditOrbitType);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditCameraPattern);//
	m_pWeaponItem->m_GridUpdate.SetItemText(m_pWeaponItem->m_GridUpdate.GetRowCount()-1, m_nCols++, m_valueEditSourceIndex);//

	m_pWeaponItem->m_GridUpdate.Refresh();
	EndDialog(0);
}

BOOL CWeaponItemDetail::ValidateCheck()
{
	UpdateData();
	
	if(m_valueEditItemNum == "")
	{
		AfxMessageBox("Please Check ItemNum Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAttackMin) > 100000 ||
		atoi((char*)(LPCTSTR)m_valueEditAttackMin) <0)
	{
		AfxMessageBox("Please Check AttackMin Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAttackMax) > 100000 ||
		atoi((char*)(LPCTSTR)m_valueEditAttackMax) <0)
	{
		AfxMessageBox("Please Check AttackMax Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqAttack) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReqAttack) <0)
	{
		AfxMessageBox("Please Check ReqAttack Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqDefense) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReqDefense) <0)
	{
		AfxMessageBox("Please Check ReqDefense Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqFuel) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReqFuel) <0)
	{
		AfxMessageBox("Please Check ReqFuel Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqSoul) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReqSoul) <0)
	{
		AfxMessageBox("Please Check ReqSoul Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqShield) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReqShield) <0)
	{
		AfxMessageBox("Please Check ReqShield Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReqDodge) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReqDodge) <0)
	{
		AfxMessageBox("Please Check ReqDodge Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMinLevel) > 100 ||
		atoi((char*)(LPCTSTR)m_valueEditMinLevel) <0)
	{
		AfxMessageBox("Please Check ReqMinLevel Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMaxLevel) > 100 ||
		atoi((char*)(LPCTSTR)m_valueEditMaxLevel) <0)
	{
		AfxMessageBox("Please Check ReqMaxLevel Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditWeight) > 9999 ||
		atoi((char*)(LPCTSTR)m_valueEditWeight) <0)
	{
		AfxMessageBox("Please Check Weight Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditHitRate) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditHitRate) <0)
	{
		AfxMessageBox("Please Check HitRate Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditDefense) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditDefense) <0)
	{
		AfxMessageBox("Please Check Defense Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditRange) > 9999 ||
		atoi((char*)(LPCTSTR)m_valueEditRange) <0)
	{
		AfxMessageBox("Please Check Range Value");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAbrasionRate) > 255 ||
		atoi((char*)(LPCTSTR)m_valueEditAbrasionRate) <0)
	{
		AfxMessageBox("Please Check Range AbrasionRate");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditCharging) > 9999 ||
		atoi((char*)(LPCTSTR)m_valueEditCharging) <0)
	{
		AfxMessageBox("Please Check Range Charging");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMinTradeQuantity) > 9999 ||
		atoi((char*)(LPCTSTR)m_valueEditMinTradeQuantity) <1)
	{
		AfxMessageBox("Please Check Range MinTradeQuantity");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditPrice) > 999999999 ||
		atoi((char*)(LPCTSTR)m_valueEditPrice) <0)
	{
		AfxMessageBox("Please Check Range Price");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditCashPrice) > 999999999 ||
		atoi((char*)(LPCTSTR)m_valueEditCashPrice) <0)
	{
		AfxMessageBox("Please Check Range CashPrice");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReattackTime) > 99999 ||
		atoi((char*)(LPCTSTR)m_valueEditReattackTime) <0)
	{
		AfxMessageBox("Please Check Range ReattackTime");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditOverHitTime) > 99999 ||
		atoi((char*)(LPCTSTR)m_valueEditOverHitTime) <0)
	{
		AfxMessageBox("Please Check Range OverHitTime");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMultiTarget) > 10 ||
		atoi((char*)(LPCTSTR)m_valueEditMultiTarget) <1)
	{
		AfxMessageBox("Please Check Range MultiTarget");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditExplosionRange) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditExplosionRange) <0)
	{
		AfxMessageBox("Please Check Range ExplosionRange");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditReactionRange) > 999 ||
		atoi((char*)(LPCTSTR)m_valueEditReactionRange) <0)
	{
		AfxMessageBox("Please Check Range ReactionRange");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditShotNum) > 99 ||
		atoi((char*)(LPCTSTR)m_valueEditShotNum) <0)
	{
		AfxMessageBox("Please Check Range ShotNum");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditMultiNum) > 99 ||
		atoi((char*)(LPCTSTR)m_valueEditMultiNum) <0)
	{
		AfxMessageBox("Please Check Range MultiNum");
		return FALSE;
	}
	if(atoi((char*)(LPCTSTR)m_valueEditAttackTime) > 30000 ||
		atoi((char*)(LPCTSTR)m_valueEditAttackTime) <0)
	{
		AfxMessageBox("Please Check Range AttackTime");
		return FALSE;
	}
	if(atof((char*)(LPCTSTR)m_valueEditRangeAngle) > 3.14 ||
		atof((char*)(LPCTSTR)m_valueEditRangeAngle) <0)
	{
		AfxMessageBox("Please Check Range Angle");
		return FALSE;
	}

	if(m_valueEditDescription.GetLength()>200)
	{
		AfxMessageBox("Please Check Description String Length");
		return FALSE;
	}

	//콤보박스가 선택된것이 없으면 에러처리
	CString szCheck;
// 2005-08-08 by cmkwon, 무기류에서는 ReqItemKind는 필요없는 필드임, 체크 필요없음
//	m_ctrlComboItemKind.GetWindowText(szCheck);
//	if(szCheck == "")
//	{
//		AfxMessageBox("Please Check ItemKind Value");
//		return FALSE;
//	}

	m_ctrlComboKind.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Kind Value");
		return FALSE;
	}

	m_ctrlComboLink.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Link Value");
		return FALSE;
	}

	m_ctrlComboPosition.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Position Value");
		return FALSE;
	}

	m_ctrlComboRace.GetWindowText(szCheck);
	if(szCheck == "")
	{
		AfxMessageBox("Please Check Race Value");
		return FALSE;
	}


	return TRUE;
}

void CWeaponItemDetail::OnButtonLinkdetail() 
{
// 2009-04-21 by cmkwon, ITEM에 DesParam 필드 개수 8개로 늘리기 - 현재 사용하지 않으므로 주석 처리
// 	// TODO: Add your control notification handler code here
// 	//현재 선택된 row를 알아온다.
// 	CString tempMonsterItem;
// 	m_ctrlComboLink.GetWindowText(tempMonsterItem);
// 	if(tempMonsterItem== "0")
// 		return;
// 	CString szSQLQuery;
// 	// 2008-07-02 by cmkwon, MySQL 지원 구현 - 
// 	//szSQLQuery.Format("select ItemNum, Itemname, AbilityMin, AbilityMax, ReqRace, ReqMinLevel, Hitrate, SpeedPenalty, Range, Charging, RangeAngle, Shotnum, Multinum, Attacktime, ReqSP, Summonmonster, Orbittype, Kind, sourceindex, DestParameter1, ParameterValue1, DestParameter2, ParameterValue2, DestParameter3, ParameterValue3, DestParameter4, ParameterValue4 from ti_item where itemnum like '75%%' and itemnum in (%s)",
// 	//	tempMonsterItem);
// 	szSQLQuery.Format(QUERY_080702_0235, tempMonsterItem);
// 
// 	SQLINTEGER cb1, cb2, cb3, cb4, cb5, cb6, cb7, cb8, cb9, cb10, cb11, cb12, cb13, cb14, cb15, cb16, cb17, cb18, cb19, cb20, cb21, cb22, cb23, cb24, cb25, cb26, cb27, cb28;
// 
// 	_TI_ITEM		st_Item;
// 	memset(&st_Item, 0, sizeof(st_Item));
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 1, SQL_C_ULONG, &st_Item.ItemNum, 0, &cb1);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 2, SQL_C_CHAR, &st_Item.ItemName, 40, &cb2);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 3, SQL_C_FLOAT, &st_Item.AbilityMin, 0, &cb3);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 4, SQL_C_FLOAT, &st_Item.AbilityMax, 0, &cb4);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 5, SQL_C_UTINYINT, &st_Item.ReqRace, 0, &cb5);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 6, SQL_C_UTINYINT, &st_Item.ReqMinLevel, 0, &cb6);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 7, SQL_C_UTINYINT, &st_Item.HitRate, 0, &cb7);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 8, SQL_C_UTINYINT, &st_Item.SpeedPenalty, 0, &cb8);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 9, SQL_C_SSHORT, &st_Item.Range, 0, &cb9);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 10, SQL_C_SSHORT, &st_Item.Charging, 0, &cb10);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 11, SQL_C_FLOAT, &st_Item.RangeAngle, 0, &cb11);
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 12, SQL_C_UTINYINT, &st_Item.ShotNum, 0, &cb12);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 13, SQL_C_UTINYINT, &st_Item.MultiNum, 0, &cb13);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 14, SQL_C_SSHORT, &st_Item.AttackTime, 0, &cb14);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 15, SQL_C_UTINYINT, &st_Item.ReqSP, 0, &cb15);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 16, SQL_C_ULONG, &st_Item.SummonMonster, 0, &cb16);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 17, SQL_C_UTINYINT, &st_Item.OrbitType, 0, &cb17);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 18, SQL_C_FLOAT, &st_Item.CameraPattern, 0, &cb18);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 19, SQL_C_UTINYINT, &st_Item.Kind, 0, &cb19);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 20, SQL_C_ULONG, &st_Item.SourceIndex, 0, &cb20);
// 
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 21, SQL_C_UTINYINT, &st_Item.DestParameter1, 0, &cb21);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 22, SQL_C_FLOAT, &st_Item.ParameterValue1, 0, &cb22);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 23, SQL_C_UTINYINT, &st_Item.DestParameter2, 0, &cb23);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 24, SQL_C_FLOAT, &st_Item.ParameterValue2, 0, &cb24);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 25, SQL_C_UTINYINT, &st_Item.DestParameter3, 0, &cb25);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 26, SQL_C_FLOAT, &st_Item.ParameterValue3, 0, &cb26);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 27, SQL_C_UTINYINT, &st_Item.DestParameter4, 0, &cb27);
// 	SQLBindCol(m_pODBCStmt->m_hstmt, 28, SQL_C_FLOAT, &st_Item.ParameterValue4, 0, &cb28);
// 	
// 	BOOL bRet = m_pODBCStmt->ExecuteQuery((LPCSTR)szSQLQuery);
// 	if (!bRet)
// 	{
// 		// clean up
// 		m_pODBCStmt->FreeStatement();
// 		MessageBox("Error");
// 		return;
// 	}
// 	SQLFetch(m_pODBCStmt->m_hstmt);
// 
// 	// clean up
// 	m_pODBCStmt->FreeStatement();
// 
// 	if (m_pLinkDetailDlg != NULL)
// 	{
// 		util::del(m_pLinkDetailDlg);
// 	}
// 
// 	if (m_pLinkDetailDlg == NULL)
// 	{
// 		m_pLinkDetailDlg = new CLinkDetailDlg(this);
// 		m_pLinkDetailDlg->Create(IDD_DIALOG_LINKDETAIL, this);
// 	}
// 	SetDetailLinkDetail(&st_Item);
// 	m_pLinkDetailDlg->ShowWindow(SW_SHOW);
}

void CWeaponItemDetail::SetDetailLinkDetail(_TI_ITEM *i_pTi_Item)
{
	m_pLinkDetailDlg->m_valueEditItemNum.Format("%d", i_pTi_Item->ItemNum);
	m_pLinkDetailDlg->UpdateData(FALSE);
}
